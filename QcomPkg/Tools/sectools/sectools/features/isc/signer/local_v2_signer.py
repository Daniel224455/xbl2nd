# ===============================================================================
#
#  Copyright (c) 2013-2017 Qualcomm Technologies, Inc.
#  All Rights Reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
#
# ===============================================================================

from sectools.common import crypto
from sectools.common.utils import c_path
from sectools.common.utils.c_attribute import Attribute, BaseAttribute
from sectools.common.utils.c_logging import logger
from sectools.features.isc.advanced_defines import AUTHORITY_QTI
from sectools.features.isc.advanced_defines import SECBOOT_VERSION_3_0
from sectools.features.isc.cfgparser.defines import MRC_1_0_CHIPSETS
from sectools.features.isc.cfgparser.defines import MRC_2_0_CHIPSETS
from sectools.features.isc.defines import AUTHORITY_OEM
from sectools.features.isc.hasher.hasher import HmacParams
from sectools.features.isc.signer.signerutils import ExtractedAttributes
from sectools.features.isc.signer.signerutils import get_msm_id
from sectools.features.isc.signer.signerutils import get_sw_id
from sectools.features.isc.signer.signerutils.attribute_extractor import \
    AttributeExtractor
from sectools.features.isc.signer.signerutils.attributes import SigningAttributes
from sectools.features.isc.signer.signerutils.certificate import Certificate
from sectools.features.isc.signer.signerutils.certificate import SignAttr
from sectools.features.isc.signer.signerutils import certconfigparser
from sectools.features.isc.signer.utils import misc as utils_misc
from sectools.features.isc.signer.utils.hmac import HMAC
from .base_signer import BaseSigner
from .signer_errors import ConfigError
from .signer_output import SignerOutput


class LocalV2Signer(BaseSigner):

    signing_attributes_class = SigningAttributes

    def __init__(self, config=None):
        self.config_overrides = utils_misc.get_signing_config_overrides()
        super(LocalV2Signer, self).__init__(config=config)
        # Override get_cert_data
        self.get_cert_data = certconfigparser.get_cert_data

    @classmethod
    def signer_id(cls):
        return 'local_v2'

    def _attribute_extractor(self, cert_data=None, attributes=None,
                             hash_segment_metadata=None, parsegen=None):
        if hash_segment_metadata is None and parsegen is not None:
            hash_segment_metadata = parsegen.get_hash_segment_metadata()
        return AttributeExtractor(
            cert_data=cert_data, attributes=attributes,
            hash_segment_metadata=hash_segment_metadata)

    def _print_attestation_cert_props(self, attestation_cert, parsegen=None):
        # Print certificate properties (to make tests pass and give good debug information)
        if (hasattr(parsegen, "get_hash_segment_metadata") and
                parsegen.get_hash_segment_metadata() is None):
            logger.info('\nAttestation Certificate Properties:\n' +
                        str(Certificate(attestation_cert)))

    # This routine only supports 2-level and 3-level cert chain
    def _get_signer_output(self, signature, cert_chain_list):
        signer_output = SignerOutput()
        signer_output.attestation_cert = cert_chain_list[0]
        if len(cert_chain_list) == 3:
            signer_output.attestation_ca_cert = cert_chain_list[1]
            signer_output.root_cert = cert_chain_list[2]
        elif len(cert_chain_list) == 2:
            signer_output.root_cert = cert_chain_list[1]
        else:
            raise RuntimeError("Only 2-level or 3-level cert chain is supported. Number of certificates found = {0}\n"
                               .format(len(cert_chain_list)))

        cert_chain = crypto.cert.create_cert_chain_bin(cert_chain_list)

        signer_output.signature = signature
        signer_output.cert_chain = cert_chain

        return signer_output

    def create_subject_params_attest(self, in_params):
        # Set exfile
        if self._is_oid_supported(self.signing_attributes):
            if not self.validate_oid_from_config(self.certs_info.ca.cert_path, self.signing_attributes):
                raise ConfigError('{0} min and max are not set correctly in configuration.'
                                  'Signing will not continue.'.format(self.signing_attributes.object_id.name))
            self.certs[self.ATTEST].extfile = self._generate_attestation_certificate_extensions(self.openssl_info.attest_ca_xts,
                                                                                                self.signing_attributes.object_id.name,
                                                                                                self.signing_attributes.object_id.min,
                                                                                                self.signing_attributes.object_id.max)
        else:
            self.certs[self.ATTEST].extfile = self.openssl_info.attest_ca_xts

        # Only allow OU fields to be added to attest cert if they are not being added to hash segment
        if self.signing_attributes.secboot_version in [SECBOOT_VERSION_3_0]:
            # Don't add OU fields
            logger.debug("Skipping adding OU fields to attest certificate.")
            return dict(in_params)

        def create_ou_field_from_hex_list(ou_num, ou_name, hex_list, remove_0x, max_num_items_in_ou):
            item_length = 0
            ou_field = str(ou_num)
            for val in hex_list:
                item_length = len(val)-2 if remove_0x else len(val)
                ou_field += " " + (val[2:] if remove_0x else val)
            # fill remainder of OU field with zeros
            zeros = (" " + "0" * item_length) * (max_num_items_in_ou - len(hex_list))
            ou_field += zeros
            ou_field += " " + ou_name
            return ou_field

        # GET SIGNING ATTRIBUTE DATA
        num_root_certs = int(self.signing_attributes.num_root_certs) if self.signing_attributes.num_root_certs is not None else None
        debug_val = int(self.signing_attributes.debug, 16) if self.signing_attributes.debug is not None else None
        multi_serial_numbers = self.signing_attributes.multi_serial_numbers.serial if self.signing_attributes.multi_serial_numbers is not None else []
        oem_id = int(self.signing_attributes.oem_id, 16) & 0xFFFF
        model_id = int(self.signing_attributes.model_id, 16) & 0xFFFF
        app_id = int(self.signing_attributes.app_id, 16) if self.signing_attributes.app_id is not None else None
        crash_dump = int(self.signing_attributes.crash_dump, 16) if self.signing_attributes.crash_dump is not None else None
        rot_en = int(self.signing_attributes.rot_en, 16) if self.signing_attributes.rot_en is not None else None
        mask_soc_hw_version = int(self.signing_attributes.mask_soc_hw_version, 16) if self.signing_attributes.mask_soc_hw_version is not None else None
        in_use_soc_hw_version = self.signing_attributes.in_use_soc_hw_version if self.signing_attributes.in_use_soc_hw_version is not None else None
        soc_vers = self.signing_attributes.soc_vers
        use_serial_number_in_signing = self.signing_attributes.use_serial_number_in_signing if self.signing_attributes.use_serial_number_in_signing is not None else None
        oem_id_independent = self.signing_attributes.oem_id_independent if self.signing_attributes.oem_id_independent is not None else None
        revocation_enablement = int(self.signing_attributes.revocation_enablement, 16) if self.signing_attributes.revocation_enablement is not None else None
        activation_enablement = int(self.signing_attributes.activation_enablement, 16) if self.signing_attributes.activation_enablement is not None else None
        root_revoke_activate_enable = int(self.signing_attributes.root_revoke_activate_enable, 16) if self.signing_attributes.root_revoke_activate_enable is not None else None
        uie_key_switch_enable = int(self.signing_attributes.uie_key_switch_enable, 16) if self.signing_attributes.uie_key_switch_enable is not None else None

        # Get the binary to sign length
        if self.data_to_sign_len is None:
            if self.hash_to_sign is not None:
                self.data_to_sign_len = len(self.hash_to_sign)
            else:
                raise RuntimeError('Length of binary could not be computed')

        logger.debug('Generating new Attestation certificate and a random key')
        hmac_params = HMAC()
        hmac_params.init_from_config(self.signing_attributes)
        certificate_ou_sw_id = '01 ' + '%.16X' % hmac_params.sw_id + ' SW_ID'
        certificate_ou_hw_id = '02 ' + '%.16X' % hmac_params.msm_id + ' HW_ID'
        certificate_ou_oem_id = '04 ' + '%0.4X' % oem_id + ' OEM_ID'
        certificate_ou_sw_size = '05 ' + '%0.8X' % self.data_to_sign_len + ' SW_SIZE'
        certificate_ou_model_id = '06 ' + '%0.4X' % model_id + ' MODEL_ID'
        certificate_hash_alg = self.SHA_OU_MAP[self.signing_attributes.hash_algorithm]

        certificate_ou = [
            certificate_ou_sw_id,
            certificate_ou_hw_id,
            certificate_ou_oem_id,
            certificate_ou_sw_size,
            certificate_ou_model_id,
            certificate_hash_alg
        ]

        # Optional attributes
        if debug_val is not None:
            certificate_ou_debug_id = '03 ' + '%0.16X' % debug_val + ' DEBUG'
            certificate_ou.append(certificate_ou_debug_id)
        if app_id is not None:
            certificate_app_id = '08 ' + '%0.16X' % app_id + ' APP_ID'
            certificate_ou.append(certificate_app_id)
        if crash_dump is not None:
            certificate_crash_dump = '09 ' + '%0.16X' % crash_dump + ' CRASH_DUMP'
            certificate_ou.append(certificate_crash_dump)
        if rot_en is not None:
            certificate_rot_en = '10 ' + '%0.16X' % rot_en + ' ROT_EN'
            certificate_ou.append(certificate_rot_en)
        if mask_soc_hw_version is not None:
            certificate_mask_soc_hw_version = '12 ' + '%0.4X' % mask_soc_hw_version + ' MASK_SOC_HW_VERSION'
            certificate_ou.append(certificate_mask_soc_hw_version)
        if in_use_soc_hw_version == 1:
            certificate_in_use_soc_hw_version = '13 ' + '%0.4X' % in_use_soc_hw_version + ' IN_USE_SOC_HW_VERSION'
            certificate_ou.append(certificate_in_use_soc_hw_version)
        if use_serial_number_in_signing == 1:
            certificate_use_serial_number_in_signing = '14 ' + '%0.4X' % use_serial_number_in_signing +\
                                                       ' USE_SERIAL_NUMBER_IN_SIGNING'
            certificate_ou.append(certificate_use_serial_number_in_signing)
        if oem_id_independent == 1:
            certificate_oem_id_independent = '15 ' + '%0.4X' % oem_id_independent + ' OEM_ID_INDEPENDENT'
            certificate_ou.append(certificate_oem_id_independent)

        # multiple debug serial use case
        certificate_ou_sn_list = []
        for index in xrange(0, len(multi_serial_numbers), 6):
            serial_sublist = multi_serial_numbers[index: index+6]
            certificate_ou_sn_list.append(create_ou_field_from_hex_list(16, "SN", serial_sublist, True, 6))
        certificate_ou_sn_list.reverse()
        certificate_ou.extend(certificate_ou_sn_list)

        # multiple soc hw version use case
        if soc_vers:
            if self.padding != self.PAD_PSS:
                logger.warning("soc_vers should be used with RSAPSS")
            certificate_ou.append(create_ou_field_from_hex_list(11, "SOC_VERS", soc_vers, True, 10))
        elif (hasattr(self.signing_attributes, "ignored_general_properties") and
                self.signing_attributes.ignored_general_properties["soc_vers"]):
            logger.debug("soc_vers were provided but in_use_soc_hw_version was set to \"0\"")

        # MRC 1.0 use case
        if num_root_certs > 1 and self.config.metadata.chipset in MRC_1_0_CHIPSETS:
            certificate_root_cert_sel = '17 ' + '%0.4X' % self.signing_attributes.mrc_index + ' ROOT_CERT_SEL'
            certificate_ou.append(certificate_root_cert_sel)
            if revocation_enablement is not None and revocation_enablement != 0:
                certificate_revocation_enablement = '18 ' + '%0.16X' % revocation_enablement + ' REVOCATION_ENABLEMENT'
                certificate_ou.append(certificate_revocation_enablement)
            if activation_enablement is not None and activation_enablement != 0:
                certificate_activation_enablement = '19 ' + '%0.16X' % activation_enablement + ' ACTIVATION_ENABLEMENT'
                certificate_ou.append(certificate_activation_enablement)

        # MRC 2.0 use case
        if num_root_certs > 1 and self.config.metadata.chipset in MRC_2_0_CHIPSETS:
            certificate_root_cert_sel = '17 ' + '%0.4X' % self.signing_attributes.mrc_index + ' ROOT_CERT_SEL'
            certificate_ou.append(certificate_root_cert_sel)
            if root_revoke_activate_enable is not None and root_revoke_activate_enable != 0:
                certificate_root_revoke_activate_enable =\
                    '20 ' + '%0.16X' % root_revoke_activate_enable + ' ROOT_REVOKE_ACTIVATE_ENABLE'
                certificate_ou.append(certificate_root_revoke_activate_enable)

        if uie_key_switch_enable is not None and uie_key_switch_enable != 0:
            certificate_uie_key_switch_enable = '21 ' + '%0.16X' % uie_key_switch_enable + ' UIE_KEY_SWITCH_ENABLE'
            certificate_ou.append(certificate_uie_key_switch_enable)

        # Handle OU property binding
        params = dict(in_params)
        if 'OU' in params.keys():
            if type(params['OU']) == list:
                for item in params['OU']:
                    certificate_ou.append(item)
            else:
                certificate_ou.append(params['OU'])

        # Add OU fields
        params['OU'] = certificate_ou
        logger.debug("Adding OU fields to attest certificate.")

        return params

    def _generate_oid_config(self, oid_name, min_str, max_str):
        min_attr = Attribute.init(num_bits=32, string=min_str)
        max_attr = Attribute.init(num_bits=32, string=max_str)

        oid_str = '%.8X%.8X' % (min_attr.value, max_attr.value)
        oid_cfg = '\n%s=DER:%s:%s:%s:%s:%s:%s:%s:%s' % \
                  (Certificate.GetOIDByName(oid_name), oid_str[0:2], oid_str[2:4], oid_str[4:6], oid_str[6:8],
                   oid_str[8:10], oid_str[10:12], oid_str[12:14], oid_str[14:16])

        return oid_cfg

    def _generate_attestation_certificate_extensions(self,
                                                     attestation_certificate_extensions_path,
                                                     oid_name,
                                                     min_str,
                                                     max_str):
        v3_attest_file = c_path.load_data_from_file(attestation_certificate_extensions_path)
        v3_attest_file_new = v3_attest_file + self._generate_oid_config(oid_name, min_str, max_str)
        v3_attest_file_temp = c_path.create_tmp_file(data=v3_attest_file_new)

        return v3_attest_file_temp

    def signed_authority_message_function(self, signed_authority, extracted_image_attributes):
        rsa_padding_message = dict(pss="RSAPSS", pkcs="PKCS")
        message = "Image is signed with "
        if signed_authority:
            message = signed_authority + " signed image with "
        if extracted_image_attributes.dsa_type == crypto.cert.SIGN_ALGO_ECDSA:
            return message + "ECDSA"
        return message + rsa_padding_message[extracted_image_attributes.rsa_padding]

    def validate_sig_using_hash(self, image_hash, signature, cert_chain_der,
                                signed_authority=None, extracted_image_attributes=None,
                                message_function=None):
        return super(LocalV2Signer, self).validate_sig_using_hash(
            image_hash, signature, cert_chain_der, signed_authority=signed_authority,
            extracted_image_attributes=extracted_image_attributes,
            message_function=self.signed_authority_message_function)

    def validate_signing_attributes(
            self, cert_chain_der, imageinfo, extracted_image_attributes,
            excluded_attributes=None):
        excluded_attributes = tuple(extracted_image_attributes.EXCLUDE_NON_ATTEST)
        # For ECDSA, exponent, key_size and rsa_padding attributes are excluded from
        # signing attribute comparison. For RSA, ecdsa_curve attribute is excluded.
        dsa_type_exclusion = {
            "ecdsa": ("exponent", "key_size", "rsa_padding"), "rsa": ("ecdsa_curve",)}
        excluded_attributes += dsa_type_exclusion.get(
            extracted_image_attributes.dsa_type, ())
        if imageinfo.general_properties.secboot_version == SECBOOT_VERSION_3_0:
            # All signing attributes are in metadata. Under conditions outlined in
            # the in_use_soc_hw_version_exclusion dictionary, the corresponding
            # metadata attributes' comparison result is not relevant. So they
            # are excluded from the comparison.
            in_use_soc_hw_version_exclusion = {0: ("soc_vers",), 1: ("hw_id",)}
            excluded_attributes += in_use_soc_hw_version_exclusion.get(
                extracted_image_attributes.in_use_soc_hw_version, ())
        return super(LocalV2Signer, self).validate_signing_attributes(
            cert_chain_der, imageinfo, extracted_image_attributes,
            excluded_attributes=excluded_attributes)

    def validate(self, parsegen, root_cert_hash=None, imageinfo=None):
        if parsegen.is_signed():
            # Create error string
            errstr = []

            # Extracted hash segment metadata for all authorities
            metadata = {AUTHORITY_QTI: None, AUTHORITY_OEM: None}
            key_size = {AUTHORITY_QTI: None, AUTHORITY_OEM: None}

            for (signed_authority, data_to_sign, data_signature, cert_chain,
                    hash_segment_metadata) in parsegen.get_signing_assets():
                # Save extracted hash segment metadata for signing attribute validation
                metadata[signed_authority] = hash_segment_metadata
                key_size[signed_authority] = len(data_signature) * 8

                # Check if empty
                if not data_signature and not cert_chain:
                    if signed_authority != parsegen.authority:
                        logger.warning(signed_authority + ' signature is not present')
                    else:
                        raise RuntimeError(signed_authority + ' signature is not present')
                    continue

                # Extract the cert chain list
                cert_chain_der = crypto.cert.split_cert_chain_bin(cert_chain)

                # Extract signing attributes from image
                extracted_image_attributes = AttributeExtractor(
                    cert_data=cert_chain_der[0], attributes=self.signing_attributes_class(),
                    hash_segment_metadata=hash_segment_metadata).attributes

                # Signature verification
                if not self.validate_sig(
                        data_to_sign, data_signature, cert_chain_der, signed_authority=signed_authority,
                        extracted_image_attributes=extracted_image_attributes, imageinfo=imageinfo):
                    errstr.append(signed_authority + ' signature is invalid')

                # OID Validation
                if len(cert_chain_der) == 3:
                    if not self.validate_oid_from_certs(cert_chain_der[1], cert_chain_der[0]):
                        errstr.append('OID values in the certificate are invalid')

            # Extract the cert chain list
            cert_chain_blob = parsegen.cert_chain
            cert_chain_der = crypto.cert.split_cert_chain_bin(cert_chain_blob)

            # Extract signing attributes from image
            attributes = self.signing_attributes_class()
            attributes.key_size = key_size[parsegen.authority]
            attribute_extractor = AttributeExtractor(
                cert_data=cert_chain_der[0],
                hash_segment_metadata=metadata[parsegen.authority],
                attributes=attributes)

            self.validate_more(parsegen, imageinfo, attribute_extractor, root_cert_hash, cert_chain_der, errstr)
            return True
        else:
            raise RuntimeError("Image supplied is not signed.")

    def _is_oid_supported(self, signing_attributes):
        isSupported = False
        if signing_attributes.object_id:
            isSupported = True
        return isSupported

    def _validate_oid_values(self, signing_attributes, general_properties, mandatory=True):
        if self._is_oid_supported(signing_attributes) is False:
            return

        oid_name = signing_attributes.object_id.name
        oid_min = signing_attributes.object_id.min
        oid_max = signing_attributes.object_id.max

        config_params_32bits = {
            "min": oid_min,
            "max": oid_max,
        }

        for key in config_params_32bits.keys():
            valid = False
            if config_params_32bits[key] and (Attribute.validate(num_bits=32, string=config_params_32bits[key]) is True):
                valid = True
            elif (not config_params_32bits[key]) and (mandatory is False):
                valid = True

            if valid is False:
                raise ConfigError(self.MSG_INVALID_32_INTEGER.
                                  format(oid_name,
                                         key, config_params_32bits[key]))

        if oid_min and oid_max and (Attribute.init(num_bits=32, string=oid_min).value > Attribute.init(num_bits=32, string=oid_max).value):
            raise ConfigError('{0} min must be less than max, min={1} max={2}'.format(oid_name, oid_min, oid_max))

    def _validate_attributes_with_oid_rule(self, attest_cert_obj):
        is_valid = True
        # Enforce TCG rules
        attributes_zero_list = [
            SignAttr.SW_ID,
            SignAttr.HW_ID,
            SignAttr.OEM_ID,
            SignAttr.MODEL_ID,
        ]

        attributes_none_list = [
            SignAttr.APP_ID,
            SignAttr.CRASH_DUMP,
            SignAttr.ROT_EN,
        ]

        if attest_cert_obj.tcg_min and attest_cert_obj.tcg_max:
            # Only enforce TCG rules currently
            for attr_name in attributes_zero_list:
                attr = attest_cert_obj.get_attr(attr_name)
                if attr.value != 0:
                    logger.debug("{0} should be 0 under TCG validation rules. Current value is {1}". \
                                 format(attr_name, attr.str))
                    is_valid = False

            for attr_name in attributes_none_list:
                attr = attest_cert_obj.get_attr(attr_name)
                if attr is not None:
                    logger.debug("{0} should be None under TCG validation rules. Current value is {1}". \
                                 format(attr_name, attr.str))
                    is_valid = False

            attr = attest_cert_obj.get_attr(SignAttr.DEBUG)
            if attr is not None and attr.value != self.DEBUG_DISABLED:
                logger.debug("{0} should be 2 under TCG validation rules. Current value is {1}".format(SignAttr.DEBUG, attr.str))
                is_valid = False

        return is_valid

    def _validate_oid_raw(self, min_attest, max_attest, min_ca, max_ca):
        tcg_ok = False

        if min_attest is not None:
            assert isinstance(min_attest, BaseAttribute)
        if max_attest is not None:
            assert isinstance(max_attest, BaseAttribute)
        if min_ca is not None:
            assert isinstance(min_ca, BaseAttribute)
        if max_ca is not None:
            assert isinstance(max_ca, BaseAttribute)

        if ((min_attest is None) and (max_attest is None) and
                (min_ca is None) and (max_ca is None)):
            # This is ok. No TCGs in attest cert.
            tcg_ok = True
            logger.debug("\nNo TCGs found in Attestation cert or CA cert. This is OK.")
        elif (min_attest is not None) and (max_attest is not None) and (min_ca is None) and (max_ca is None):
            logger.error("\nTCGs found in Attestation cert, but not in CA cert. This is invalid.")
        elif (min_attest is None) and (max_attest is None) and (min_ca is not None) and (max_ca is not None):
            logger.error("\nNo TCGs found in Attestation cert, but there are TCGs in CA cert. This is invalid.")
        elif (min_attest is not None) and (max_attest is not None) and (min_ca is not None) and (max_ca is not None):
            if (min_ca.value <= min_attest.value <=
                    max_attest.value <= max_ca.value):
                tcg_ok = True
                logger.debug("\nTCG values fall within CA constraints.")
            else:
                logger.error("\nTCG values are outside the CA constraints.")
        else:
            logger.error("\nInvalid TCG values")

        tcg_log_mesg = "\nAttestation cert : tcg_min={0} tcg_max={1}".format(min_attest, max_attest) + \
                       "\nCA cert (allowed): tcg_min={0} tcg_max={1}\n".format(min_ca, max_ca)
        if tcg_ok is False:
            logger.error(tcg_log_mesg)
        else:
            logger.debug(tcg_log_mesg)

        return tcg_ok

    def validate_oid_from_certs(self, ca_cert_der, attest_cert_der):
        attest_cert_obj = Certificate(attest_cert_der)
        ca_cert_obj = Certificate(ca_cert_der)
        is_valid = (self._validate_attributes_with_oid_rule(attest_cert_obj) and
                    self._validate_oid_raw(attest_cert_obj.tcg_min,
                                           attest_cert_obj.tcg_max,
                                           ca_cert_obj.tcg_min,
                                           ca_cert_obj.tcg_max))
        return is_valid

    def validate_config(self, imageinfo):
        sa = super(LocalV2Signer, self).validate_config(imageinfo)
        if sa.mrc_index and sa.mrc_index >= sa.num_root_certs:
            raise ConfigError('Multirootcert index ' + str(sa.mrc_index) +
                              ' must be smaller than the number of root certs ' +
                              str(sa.num_root_certs))
        self._validate_oid_values(sa, sa)

    def validate_oid_from_config(self, ca_cert_path, signing_attributes):
        ca_cert_obj = Certificate(path=ca_cert_path)

        min_str = signing_attributes.object_id.min
        max_str = signing_attributes.object_id.max

        min_attest = Attribute.init(num_bits=32, string=min_str)
        max_attest = Attribute.init(num_bits=32, string=max_str)

        min_ca = None
        max_ca = None
        if signing_attributes.object_id.name == "tcg":
            min_ca = ca_cert_obj.tcg_min
            max_ca = ca_cert_obj.tcg_max

        return self._validate_oid_raw(min_attest,
                                      max_attest,
                                      min_ca,
                                      max_ca)

    def get_openssl_subject_params(self, attest_cert_params):
        return crypto.cert.get_subject_from_params(attest_cert_params)

    @staticmethod
    def get_hmac_params_from_extracted_attributes(extracted_attributes):
        """
        :returns: A HMAC object with the HMAC parameters from the MBN header values.
        :rtype: obj
        """
        ea = ExtractedAttributes()
        ea.in_use_soc_hw_version = 0
        ea.msm_part = extracted_attributes.hw_id
        ea.mask_soc_hw_version = None
        ea.use_serial_number_in_signing = extracted_attributes.use_serial_number_in_signing
        ea.oem_id = extracted_attributes.oem_id
        ea.model_id = extracted_attributes.model_id
        if extracted_attributes.multi_serial_numbers:
            ea.serial_number = extracted_attributes.multi_serial_numbers[0]
        ea.sw_id = extracted_attributes.sw_id
        ea.anti_rollback_version = extracted_attributes.anti_rollback_version
        msm_id = get_msm_id(ea)
        sw_id = get_sw_id(ea)
        return HmacParams(msm_id, sw_id)
