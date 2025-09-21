/** @file SataUtil.h
   
  XBL SATA driver header file
  
  Copyright (c) 2014,2018 Qualcomm Technologies, Inc. All Rights Reserved. 
**/

/*=============================================================================
                              EDIT HISTORY

when         who     what, where, why
----------   -----   ----------------------------------------------------------- 
2014/11/17   rm      Initial Version

=============================================================================*/

#ifndef _SATA_UTIL_H_
#define _SATA_UTIL_H_

/**                                                                 
  Get AHCI controller base address
          
  @param  ControllerNum             AHCI controller number.
  @param  Address                   The base address of the memory operations.                                  
                                    
  @retval EFI_SUCCESS               The valid address is returned.  
  @retval EFI_INVALID_PARAMETER     Controller number is out of range 
                                    or Address is NULL
                                   
**/
EFI_STATUS
GetAhciBaseAddress (
  IN  UINT32         ControllerNum,
  OUT UINTN         *Address
  );

/**                                                                 
  Initialize SATA PHY
          
  @retval EFI_SUCCESS               The valid address is returned.  
  @retval EFI_DEVICE_ERROR          Failed to Initialize PHY 
                                   
**/  
EFI_STATUS
SataPhyInitialization (VOID);

#endif /* _SATA_UTIL_H_ */














