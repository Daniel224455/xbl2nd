
/*=============================================================================
  Readme file for configuring UEFI log buffer size.

  Copyright (c) 2016, 2019 Qualcomm Technologies, Inc. 
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

=============================================================================*/

UEFI log buffer size can be configured by uncommenting the following in uefiplat.cfg

#UARTLogBufferSize = 0x19000   

/* ===============================================================================*/
Size bound:
Min UART log buffer size supported is : 32KB.
If UART log buffer size is configured less than 32KB, by default the size will be set to 32KB.

Max UART log buffer size supported is : 1MB.
If UART log buffer size is configured more than 1MB, by default the size will be set to 1MB.

