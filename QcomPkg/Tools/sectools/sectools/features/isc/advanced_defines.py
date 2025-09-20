# ===============================================================================
#
#  Copyright (c) 2013-2017 Qualcomm Technologies, Inc.
#  All Rights Reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
#
# ===============================================================================

SECBOOT_VERSION_2_0 = '2.0'
SECBOOT_VERSION_3_0 = '3.0'

DEST_DEBUG_DIR_ENCDEC = 'encdec'

DEST_DEBUG_FILE_PARSEGEN_ENCRYPTED = '_encrypted'
DEST_DEBUG_FILE_ENCRYPTION_PARAMETERS = "enc_param.debug"
DEST_DEBUG_FILE_ENCRYPTED_L2_KEY = "l2_enc.bin"
DEST_DEBUG_FILE_ENCRYPTED_L3_KEY = "l3_enc.bin"
DEST_DEBUG_FILE_L2_IMAGE_IV = "l2_img_iv.bin"
DEST_DEBUG_FILE_L3_IMAGE_IV = "l3_img_iv.bin"
DEST_DEBUG_FILE_L2_ADD = "l2_aad.bin"
DEST_DEBUG_FILE_L3_ADD = "l3_aad.bin"

DEST_DEBUG_FILE_SIGNER_QTI_SIGNATURE = 'signature_qti.dat'
DEST_DEBUG_FILE_SIGNER_QTI_CERT_CHAIN = 'cert_chain_qti.cer'

DEST_FILE_DECRYPTED = '_decrypted'

AUTHORITY_QTI = 'QTI'
MULTI_SERIAL_NUMBERS_TAG = "multi_serial_numbers"


def multi_image_string():
    return "Multi-Image Sign & Integrity"
