# ===============================================================================
#
#  Copyright (c) 2013-2017 Qualcomm Technologies, Inc.
#  All Rights Reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
#
# ===============================================================================

from functools import partial

from sectools.common.utils import c_path
from sectools.common.utils.c_base import ValPolicy
from sectools.common.utils.c_logging import logger
from sectools.common.utils.c_misc import extract_image_id_from_sw_id
from sectools.features.isc import advanced_defines
from sectools.features.isc import defines
from sectools.features.isc.advanced_defines import SECBOOT_VERSION_3_0
from sectools.features.isc.cfgparser.defines import JTAGID_SIGN_IDS
from sectools.features.isc.cfgparser.defines import MRC_1_0_CHIPSETS
from sectools.features.isc.cfgparser.defines import MRC_2_0_CHIPSETS
from sectools.features.isc.cfgparser.defines import SECBOOT_1_IMAGE_TYPES
from sectools.features.isc.cfgparser.defines import SECBOOT_2_0_DOUBLE_SIGN_CHIPSETS
from sectools.features.isc.cfgparser.defines import SECBOOT_3_0_DOUBLE_SIGN_CHIPSETS
from sectools.features.isc.defines import SECBOOT_VERSION_1_0
from sectools.features.isc.encryption_service.unified.encdec import UIE_1_0_L2_1_1_IDENT
from sectools.features.isc.encryption_service.unified.encdec import UIE_2_0_L2_1_2_IDENT
from sectools.features.isc.encryption_service.unified.encdec import UNIFIED_ENCRYPTION_IDENT
from .imageinfo_base import DestImagePathBase
from .imageinfo_base import ImageInfoBase
from .imageinfo_base import ImageStatusBase
from .imageinfo_base import debug_only
from .imageinfo_utils import StatusInfo
from ..cfgparser import auto_gen_obj_config as agoc
from ..cfgparser import defines as cfgdef
from ..parsegen.config import auto_gen_obj_config as pgagoc

POLICY_OEM_ID_0 = ValPolicy(ValPolicy.WARN)
MULTI_IMAGE_FILENAME = advanced_defines.multi_image_string() + " image"
OEM_MULTI_IMAGE_SIGN_ID = "multi_image"
QTI_MULTI_IMAGE_SIGN_ID = "multi_image_qti"
MULTI_IMAGE_SIGN_ID = {
    defines.AUTHORITY_OEM: OEM_MULTI_IMAGE_SIGN_ID,
    advanced_defines.AUTHORITY_QTI: QTI_MULTI_IMAGE_SIGN_ID,
}


class DestImagePath(DestImagePathBase):

    @property
    def decrypted_file(self):
        file_name = (self.image_name_base +
                     advanced_defines.DEST_FILE_DECRYPTED +
                     self.image_name_ext)
        return c_path.join(self.image_dir, file_name)

    @property
    @debug_only
    def debug_dir_encdec(self):
        return c_path.join(self.debug_dir, self._mid, advanced_defines.DEST_DEBUG_DIR_ENCDEC)

    @property
    @debug_only
    def debug_file_parsegen_encrypted(self):
        file_name = (self.image_name_base +
                     advanced_defines.DEST_DEBUG_FILE_PARSEGEN_ENCRYPTED +
                     self.image_name_ext)
        return c_path.join(self.debug_dir_parsegen, file_name)

    @property
    @debug_only
    def debug_file_signer_qti_signature(self):
        return c_path.join(self.debug_dir_signer,
                           advanced_defines.DEST_DEBUG_FILE_SIGNER_QTI_SIGNATURE)

    @property
    @debug_only
    def debug_file_signer_qti_cert_chain(self):
        return c_path.join(self.debug_dir_signer,
                           advanced_defines.DEST_DEBUG_FILE_SIGNER_QTI_CERT_CHAIN)


class ImageStatus(ImageStatusBase):

    def __init__(self):
        super(ImageStatus, self).__init__()
        self.encrypt = StatusInfo()
        self.validate_encrypt = StatusInfo()


class ImageInfo(ImageInfoBase):

    image_status_class = ImageStatus

    def __init__(self, image_path, sign_id, img_config_block, img_config_parser,
                 parsegen_config, gen_multi_image=False, authority=None, **kwargs):
        if not gen_multi_image:
            assert isinstance(image_path, str)

        super(ImageInfo, self).__init__(
            image_path, sign_id, img_config_block, img_config_parser,
            parsegen_config, gen_multi_image=gen_multi_image, authority=authority,
            agoc=agoc, pgagoc=pgagoc, cfgdef=cfgdef,
            run_before_sanitize=partial(self.run_before_sanitize, img_config_block))

    def run_before_sanitize(self, img_config_block):
        self.dest_image = DestImagePath()
        self.encdec = None

        # Properties obtained from the config file
        self.pil_split = img_config_block.pil_split
        self.post_process_commands = img_config_block.post_process_commands

        self.is_multi_image = self.sign_id in MULTI_IMAGE_SIGN_ID.values()
        self.image_under_operation = (self.src_image.image_path if
                                      self.src_image.image_path else MULTI_IMAGE_FILENAME)

    #--------------------------------------------------------------------------
    # Helper functions
    #--------------------------------------------------------------------------
    def _sanitize_general_properties(self):

        def _sanitize_mrc_value(value, num_root_certs, secboot_version):
            sanitized_value = value
            if value is not None and secboot_version != SECBOOT_VERSION_3_0:
                lower_8_bytes = int(value, 16) & 0xFFFFFFFF
                # if not MRC use case, to pass validation, enforce value to be zero because attest cert will not have it
                if num_root_certs == 1:
                    sanitized_value = "0x0000000000000000"
                # If last 8 bytes == 0 (disabled), then override value to None (since OU field wont be present)
                elif lower_8_bytes == 0:
                    sanitized_value = None
            return sanitized_value

        # Sanitize secboot_version
        if (self.general_properties.secboot_version is None or
                self.image_type.id in SECBOOT_1_IMAGE_TYPES):
            self.general_properties.secboot_version = SECBOOT_VERSION_1_0

        # Sanitize sw_id
        if (self.general_properties.secboot_version != SECBOOT_VERSION_3_0 and
                    self.general_properties.anti_rollback_version is not None):
            if (int(self.general_properties.sw_id, 16) & 0xFFFFFFFF00000000) == 0:
                hex_sw_id = (hex(int(self.general_properties.sw_id, 16) |
                                 (int(self.general_properties.anti_rollback_version, 16) << 32)))
                self.general_properties.sw_id = hex_sw_id[:-1] if "L" in hex_sw_id else hex_sw_id
                self.general_properties.anti_rollback_version = None

        if len(self.general_properties.sw_id) < 18:
            padding_len = 18 - len(self.general_properties.sw_id)
            self.general_properties.sw_id = self.general_properties.sw_id[:2] + "0" * padding_len + \
                                            self.general_properties.sw_id[2:]

        # Sanitize soc_vers and handle interdependence with in_use_soc_hw_version (for Secboot 1 and 2)
        self.general_properties.ignored_general_properties["soc_vers"] = False
        if self.general_properties.soc_vers is not None:
            self.general_properties.soc_vers = self.general_properties.soc_vers.split()
            if self.general_properties.secboot_version != SECBOOT_VERSION_3_0 and self.general_properties.in_use_soc_hw_version != 1:
                self.general_properties.soc_vers = None
                self.general_properties.ignored_general_properties["soc_vers"] = True

        # soc_hw_version only uses Family and Device number
        if self.general_properties.soc_hw_version is not None:
            self.general_properties.soc_hw_version = self.general_properties.soc_hw_version[:6] + "0000"
            # Remove soc_hw_version entries from soc_vers for secboot 1.0 and secboot 2.0
            if self.general_properties.secboot_version != SECBOOT_VERSION_3_0 and \
               self.general_properties.soc_vers and \
               self.general_properties.soc_hw_version[:6] in self.general_properties.soc_vers:
                self.general_properties.soc_vers.remove(self.general_properties.soc_hw_version[:6])
            # Disable in_use_soc_hw_version if chipset needs to be signed with JTAG ID
            if (self.chipset in JTAGID_SIGN_IDS) and (self.sign_id in JTAGID_SIGN_IDS[self.chipset]):
                self.general_properties.in_use_soc_hw_version = 0

        # Sanitize debug
        if self.general_properties.debug is not None and len(self.general_properties.debug) < 18:
            padding_len = 18 - len(self.general_properties.debug)
            self.general_properties.debug = self.general_properties.debug[:2] + \
                                            "0" * padding_len + self.general_properties.debug[2:]

        # Sanitize multi_serial_numbers
        if self.general_properties.multi_serial_numbers is not None:
            self.general_properties.multi_serial_numbers.serial = [
                serial for serial in self.general_properties.multi_serial_numbers.serial if int(serial, 16) != 0]

        # Apply OEM independent rule: if oem_id_independent == 1, make oem_id=1 regardless
        if self.general_properties.oem_id_independent == 1:
            self.general_properties.oem_id = '0x0001'
            self.general_properties.model_id = '0x0000'

        # Sanitize revocation_enablement
        self.general_properties.revocation_enablement = _sanitize_mrc_value(
            self.general_properties.revocation_enablement, self.general_properties.num_root_certs,
            self.general_properties.secboot_version)

        # Sanitize activation_enablement
        self.general_properties.activation_enablement = _sanitize_mrc_value(
            self.general_properties.activation_enablement, self.general_properties.num_root_certs,
            self.general_properties.secboot_version)

        # Sanitize root_revoke_activate_enable
        self.general_properties.root_revoke_activate_enable = _sanitize_mrc_value(
            self.general_properties.root_revoke_activate_enable, self.general_properties.num_root_certs,
            self.general_properties.secboot_version)

        # Sanitize uie_key_switch_enable
        if (self.general_properties.uie_key_switch_enable is not None and
                    (int(self.general_properties.uie_key_switch_enable, 16) & 0xFFFFFFFF) == 0):
            # If last 8 bytes == 0 (disabled), then override value to None (since OU field wont be present)
            self.general_properties.uie_key_switch_enable = None

        # Sanitize hash_algorithm
        if self.general_properties.hash_algorithm is None:
            self.general_properties.hash_algorithm = "sha256"

        # Sanitize segment hash_algorithm
        if self.general_properties.segment_hash_algorithm is None:
            self.general_properties.segment_hash_algorithm = "sha256"

        # Sanitize dsa_type
        if self.general_properties.dsa_type is None:
            self.general_properties.dsa_type = "rsa"

        # Sanitize rsa_padding
        if self.general_properties.rsa_padding is None:
            self.general_properties.rsa_padding = "pkcs"

        # Sanitize hmac
        self.general_properties.hmac = self.general_properties.hmac or self.general_properties.hmac is None

        # Sanitize selected_encryptor
        if (self.general_properties.selected_encryptor == UNIFIED_ENCRYPTION_IDENT and
                extract_image_id_from_sw_id(self.general_properties.sw_id) > 0x1F):
            # Upgrade the encryptor to one that supports larger sw_id values.
            self.general_properties.selected_encryptor = UIE_1_0_L2_1_1_IDENT

        # Sanitize app_id for IP Protector image
        if self.general_properties.selected_encryptor == UIE_2_0_L2_1_2_IDENT:
            # Set app_id to feature_id
            self.general_properties.app_id = self.general_properties.UIE_feature_id

    def _populate_general_properties(self):
        super(ImageInfo, self)._populate_general_properties()

        # Private overrides
        if self.config_parser.spec_overrides is not None:
            for tag, override in self.config_parser.spec_overrides.get_properties(has_value=True).items():
                if tag == 'cass_password':
                    self.config.signing.signer_attributes.cass_signer_attributes.user_identity.token_password = override.value

    def _validate_properties(self):
        self.errors = []
        sa = self.signing_attributes

        if not POLICY_OEM_ID_0.is_ignore():
            if int(sa.oem_id, 16) == 0:
                POLICY_OEM_ID_0.run('OEM ID is set to 0 for sign_id "' + str(self.sign_id) + '"')

        # Secboot v1 requires oem permissions only
        if sa.secboot_version == SECBOOT_VERSION_1_0:
            if not sa.oem_sign:
                # oem_sign cannot be false for secboot v1
                self._add_error('OEM operations cannot be disabled for a secboot version ' +
                                str(SECBOOT_VERSION_1_0) + ' image.')
            if sa.qti_sign:
                # qti_sign cannot be true for secboot v1
                self._add_error('Cannot perform QTI operations on a secboot version ' +
                                str(SECBOOT_VERSION_1_0) + ' image.')
        # If all authority permissions are disabled, throw error
        elif not sa.qti_sign and not sa.oem_sign:
            self._add_error('Cannot perform image operations because OEM and QTI operations are disabled for image.')

        # Raise warnings for Secboot v2.1 and greater chipsets
        if self.chipset in SECBOOT_2_0_DOUBLE_SIGN_CHIPSETS + SECBOOT_3_0_DOUBLE_SIGN_CHIPSETS:
            if sa.rot_en is not None and int(sa.rot_en, 16) == 1:
                logger.warning("rot_en should not be set to \"{0}\" for chipset \"{1}\"."
                               .format(sa.rot_en, self.chipset))
            if sa.hash_algorithm == "sha1":
                logger.warning("hash_algorithm should not be set to \"{0}\" for chipset \"{1}\"."
                               .format(sa.hash_algorithm, self.chipset))
            if sa.exponent == 3:
                logger.warning("exponent should not be set to \"{0}\" for chipset \"{1}\"."
                               .format(sa.exponent, self.chipset))

            if sa.dsa_type == "ecdsa":
                if sa.rsa_padding is not None:
                    sa.rsa_padding = None
            else:
                if sa.rsa_padding == "pkcs":
                    logger.warning("rsa_padding should not be set to RSAPKCS for chipset \"{0}\"."
                                   .format(self.chipset))

        # Ignore max_num_root_certs value if target is not MRC v2.0 double sign target
        if not (self.chipset in MRC_2_0_CHIPSETS and sa.qti_sign and sa.oem_sign):
            sa.max_num_root_certs = None

        if self.is_multi_image and sa.multi_image_segment_addr is None:
            self._add_error(advanced_defines.multi_image_string() +
                            " operation requires multi_image_segment_addr config value.")

        # Based on secboot_version allowed formats differ for sw_id, anti_rollback_version, rot_en,
        # uie_key_switch_enable, root_revoke_activate_enable, & debug.
        # Validation must happen outside of rule.py because images of different secboot versions can
        # coexist in the same config file.

        # Only Secboot 3 supports anti_rollback_version as its own value
        if sa.secboot_version == SECBOOT_VERSION_3_0:
            if sa.anti_rollback_version is None:
                self._add_error("Secboot {0} requires anti_rollback_version field.".format(sa.secboot_version))
            if (int(sa.sw_id, 16) & 0xFFFFFFFF00000000) != 0:
                self._add_error("Provide anti-rollback version in anti_rollback_version field for secboot {0} chipset.".format(sa.secboot_version))
                self._add_error("sw_id value {0} exceeds maximum allowed length of 10 characters.".format(sa.sw_id))
            if sa.anti_rollback_version is not None and (int(sa.anti_rollback_version, 16) & 0xFFFFFFFF00000000) != 0:
                self._add_error("anti_rollback_version value {0} exceeds maximum allowed length of 10 characters.".format(sa.anti_rollback_version))
            # sha1 is disallowed for Secboot 3
            if sa.hash_algorithm == "sha1":
                self._add_error("sha1 hash_algorithm is not allowed for secboot {0} images.".format(sa.secboot_version))
            if sa.segment_hash_algorithm == "sha1":
                self._add_error("sha1 segment_hash_algorithm is not allowed for secboot {0} images.".format(sa.secboot_version))

        # rot_en rule, LSB 32bits must not be greater than 1
        multi_serial_numbers = sa.multi_serial_numbers.serial if sa.multi_serial_numbers is not None else []
        self._validate_serial_bound_value(sa.rot_en,
                                          "rot_en",
                                          self.chipset,
                                          ignore_num_root_certs=True,
                                          disable_in_value_binding=sa.secboot_version == SECBOOT_VERSION_3_0,
                                          additional_serials_are_bindable=len(multi_serial_numbers) > 0,
                                          allowed_lsb_values=[0, 1])

        # Validate uie_key_switch_enable
        self._validate_serial_bound_value(sa.uie_key_switch_enable,
                                          "uie_key_switch_enable",
                                          self.chipset,
                                          ignore_num_root_certs=True,
                                          disable_in_value_binding=sa.secboot_version == SECBOOT_VERSION_3_0,
                                          additional_serials_are_bindable=len(multi_serial_numbers) > 0)

        # MRC 1.0 use case
        # Validate revocation_enablement
        self._validate_serial_bound_value(sa.revocation_enablement,
                                          "revocation_enablement",
                                          self.chipset,
                                          MRC_1_0_CHIPSETS,
                                          sa.num_root_certs)
        # Validate activation_enablement
        self._validate_serial_bound_value(sa.activation_enablement,
                                          "activation_enablement",
                                          self.chipset,
                                          MRC_1_0_CHIPSETS,
                                          sa.num_root_certs)
        # MRC 2.0 use case
        # Validate root_revoke_activate_enable
        self._validate_serial_bound_value(sa.root_revoke_activate_enable,
                                          "root_revoke_activate_enable",
                                          self.chipset,
                                          MRC_2_0_CHIPSETS,
                                          sa.num_root_certs,
                                          disable_in_value_binding=sa.secboot_version == SECBOOT_VERSION_3_0,
                                          additional_serials_are_bindable=len(multi_serial_numbers) > 0)

        self._validate_serial_bound_value(sa.debug,
                                          "debug",
                                          self.chipset,
                                          ignore_num_root_certs=True,
                                          disable_in_value_binding=sa.secboot_version == SECBOOT_VERSION_3_0,
                                          additional_serials_are_bindable=len(multi_serial_numbers) > 0,
                                          allowed_lsb_values=[0, 2, 3] if sa.secboot_version != SECBOOT_VERSION_3_0 else None)

        if self.errors:
            raise RuntimeError("".join(self.errors))

    def _validate_serial_bound_value(self,
                                     value,
                                     value_name,
                                     chipset=None,
                                     allowed_chipsets=None,
                                     num_root_certs=None,
                                     ignore_num_root_certs=False,
                                     disable_in_value_binding=False,
                                     additional_serials_are_bindable=False,
                                     allowed_lsb_values=None):
        if value is not None:
            if chipset is not None and allowed_chipsets is not None and chipset not in allowed_chipsets:
                self._add_error("{0} is only allowed to be set for chipsets: {1}."
                                .format(value_name, ", ".join(allowed_chipsets)))
            elif ignore_num_root_certs or num_root_certs > 1:
                upper_8_bytes = int(value, 16) & 0xFFFFFFFF00000000
                lower_8_bytes = int(value, 16) & 0xFFFFFFFF

                if allowed_lsb_values is None:
                    allowed_lsb_values = [0, 1, 2]
                allowed_lsb_hex_values = ", ".join([hex(x) for x in allowed_lsb_values])
                if disable_in_value_binding:
                    if int(value, 16) not in allowed_lsb_values:
                        self._add_error("{0} must be be set to one of the following: {1}. {0} = {2}."
                                        .format(value_name, allowed_lsb_hex_values, value))
                    if (upper_8_bytes != 0 or lower_8_bytes == allowed_lsb_values[len(allowed_lsb_values) - 1] and
                            not additional_serials_are_bindable):
                        self._add_error("to enable {0} serial number binding, add serial to multi_serial_numbers."
                                        .format(value_name))
                else:
                    if lower_8_bytes not in allowed_lsb_values:
                        self._add_error("{0} must end with one of the following: {1}. {0} = {2}."
                                        .format(value_name, ", ".join(str(v) for v in allowed_lsb_values), value))
                    # if last 8 bytes == 1, enabling with no serial binding so use as is
                    # if last 8 bytes == 2, enabling with serial binding so check upper 8 bytes are valid serial
                    if upper_8_bytes == 0 and lower_8_bytes == allowed_lsb_values[len(allowed_lsb_values) - 1]:
                        self._add_error(
                            "to enable {0} with binding to serial number, upper 8 bytes must contain serial number. "
                            "{0} = {1}.".format(value_name, value))
