/* @file fh_packet_id.h

  Packet IDs for HSUART packets

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

#ifndef __FH_PACKET_ID_H__
#define __FH_PACKET_ID_H__

enum fh_packet_id_t {
    PROTOCOL_PACKET = 0xF0,
    END_OF_TRANSFER_PACKET = 0x55,
    ACK_PACKET = 0x06,
    NAK_PACKET = 0x09,
    VERSION_PACKET = 0xAA,
    READY_TO_READ_PACKET = 0x0F,
};

#endif