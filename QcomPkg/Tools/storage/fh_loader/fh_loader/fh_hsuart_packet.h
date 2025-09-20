/* @file fh_hsuart_packet.h

  Abstraction layer for encoding and decoding HSUART packets

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

#ifndef __FH_HSUART_PACKET_H__
#define __FH_HSUART_PACKET_H__

#include "fh_packet_id.h"

#define MAX_HSUART_PACKET_SIZE (4*1024)

#define PACKET_ID_SIZE 1

enum fh_hsuart_packet_error_t
{
  HSUART_PACKET_SUCCESS,
  HSUART_INVALID_PARAM_ERROR,
  HSUART_PACKET_CORRUPTION_ERROR,
  HSUART_COBS_ERROR
};

/******************************************************************************
* Name: fh_hsuart_packet_encode
*
* Description:
* Encodes the HSUART packet with packet id and CRC and perform stuffing
*
* Arguments:
*   packet_id   [IN]: Packet ID
*   *src        [IN]: Pointer to source buffer
*   src_length  [IN]: Length of data in source buffer
*   *dst        [OUT]: Pointer to destination buffer
*   dst_length  [IN]: Length of destination buffer
*   *error      [OUT]: Error code

Returns:
* Returns the length of data on the destination buffer provided the error code
* is SUCCESS.
*
******************************************************************************/
uint32 fh_hsuart_packet_encode(enum fh_packet_id_t packet_id,
                               unsigned char *src, uint32 src_length, unsigned char *dst,
                               uint32 dst_length, enum fh_hsuart_packet_error_t *error);

/******************************************************************************
* Name: fh_packet_decode
*
* Description:
* Unstuff the HSUART packet and then decode to determine the packet id and
* check CRC
*
* Arguments:
*   *packet_id  [OUT]: Packet ID
*   *src        [IN]: Pointer to source buffer
*   length      [IN]: Length of data in source buffer
*   *dst        [OUT]: Pointer to destination buffer
*   *error      [OUT]: Error code

Returns:
* Returns the length of data on the destination buffer provided the error code
* is SUCCESS.
*
******************************************************************************/
uint32 fh_hsuart_packet_decode(enum fh_packet_id_t *packet_id,
                               unsigned char *src, uint32 length, unsigned char *dst, uint32 dst_length,
                               enum fh_hsuart_packet_error_t *error);

#endif