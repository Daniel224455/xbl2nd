/* @file fh_crc.h

  This file implements header file for CRC16 calculation

  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

*/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   -----   -----------------------------------------------------------
2019-02-27   gp      Initial version

=============================================================================*/
#ifndef __FH_CRC_H__
#define __FH_CRC_H__

typedef uint16 word;         /* Unsinged 16 bit value type. */
typedef uint8 byte;

#ifndef NULL
#define NULL 0
#endif
/*
* Residual CRC value to compare against a return value of fh_crc_calculate_crc16().
* Use fh_crc_calculate_crc16() to calculate a 16-bit CRC and append it to the
* buffer. When fh_crc_calculate_crc16() is applied to the unchanged result, it
* returns CRC_16_OK.
*/
#define CRC_16_OK               0xE2F0

/******************************************************************************
* Name: crc_16_calc
*
* Description:
* This function calculates a 16-bit CRC over a specified number of data
* bits. It can be used to produce a CRC and to check a CRC.
*
* Arguments:
*    *buf_ptr [IN]: Pointer to bytes containing the data to CRC. The bit stream
*                   starts in the MS bit of the first byte.
*    len      [IN]: Number of data bytes to calculate the CRC over

Returns:
* Returns a word holding 16 bits which are the contents of the CRC register as
* calculated over the specified data bits.  If this function is being used to
* check a CRC, then the return value will be equal to CRC_16_OK (defined in
* crc.h) if the CRC checks correctly.
*
******************************************************************************/
word fh_crc_calculate_crc16(byte *buf_ptr, uint32 len);

#endif