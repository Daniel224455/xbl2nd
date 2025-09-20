/* @file fh_cobs.h

  Abstraction layer for Consistent Overhead Byte Stuffing (COBS)

  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

*/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   -----   ----------------------------------------------------------
2019-08-05   gp      Initial version

=============================================================================*/

#ifndef __FH_COBS_H__
#define __FH_COBS_H__

enum fh_cobs_error_t
{
  COBS_SUCCESS,
  COBS_ERROR_NULL_SRC_BUFFER,
  COBS_ERROR_NULL_DST_BUFFER,
  COBS_ERROR_INVALID_LENGTH,
  COBS_ERROR_INVALID_STUFFING,
  COBS_ERROR_DST_BUFFER_OVERFLOW
};

/******************************************************************************
* Name: fh_cobs_get_error
*
* Description:
* This function returns the description of error
*
* Arguments:
*   error        [IN]: Error code

Returns:
* Returns the description of error
*
******************************************************************************/
const char* fh_cobs_get_error(enum fh_cobs_error_t error);

/******************************************************************************
* Name: fh_cobs_stuff_bytes
*
* Description:
* This function perform consistent overhead byte stuffing on the source buffer
* and stores the result in destination buffer
*
* Arguments:
*   *src        [IN]: Pointer to source buffer
*   src_length  [IN]: Length of data in source buffer
*   *dst        [IN]: Pointer to destination buffer
*   dst_length  [IN]: Length of destination buffer
*   *error      [OUT]: Error code

Returns:
* Returns the length of data on the destination buffer provided the error code
* is SUCCESS.
*
******************************************************************************/
uint32 fh_cobs_stuff_bytes(unsigned char *src, uint32 src_length,
                           unsigned char *dst, uint32 dst_length, enum fh_cobs_error_t *error);

/******************************************************************************
* Name: fh_cobs_unstuff_bytes
*
* Description:
* This function performs unstuffing over consistent overhead byte stuffed data
* in the source buffer and stores the result in destination buffer
*
* Arguments:
*   *src    [IN]: Pointer to source buffer
*   src_length  [IN]: Length of data in source buffer
*   *dst        [IN]: Pointer to destination buffer
*   dst_length  [IN]: Length of destination buffer
*   *error  [OUT]: Error code

Returns:
* Returns the length of data on the destination buffer provided the error code
* is SUCCESS.
*
******************************************************************************/
uint32 fh_cobs_unstuff_bytes(unsigned char *src, uint32 src_length,
                             unsigned char *dst, uint32 dst_length, enum fh_cobs_error_t *error);

#endif