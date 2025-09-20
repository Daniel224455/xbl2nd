/*=============================================================================
Copyright (c) 2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/
#ifndef __DISPLAYMULTICORE_H__
#define __DISPLAYMULTICORE_H__
  
/**********************************************************************************************************************
 * Defines
 **/


/**********************************************************************************************************************
 * Macros
 **/

/**********************************************************************************************************************
 *   Function Prototypes.
 **/


/**
DisplayMCDxe_DisplayInitialize

Install Display Dxe protocols and create thread

  @param  None

  @retval EFI_SUCCESS           Protocol registered
  @retval EFI_UNSUPPORTED       Error during protocol registration or thread create

**/
EFI_STATUS DisplayMCDxe_DisplayInitialize(void);

/**
  Helper function to read the Display Dxe execution mode
**/
EFI_STATUS DisplayMCDxe_ReadMultiCoreInfo(void);

/**
DisplayMCDxe_SetMode

EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE Handler

  @param  This                  pointer to the DisplayMCDxe protocol function
  @param  ModeNumber            Mode index

  @retval EFI_SUCCESS           return success, As set mode is done internally

**/
EFI_STATUS
DisplayMCDxe_SetMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  UINT32                        ModeNumber);

/**
DisplayMCDxe_QueryMode

EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE Handler

  @param  This                  pointer to the DisplayMCDxe protocol function
  @param  ModeNumber            Mode index
  @param  SizeOfInfo            size of mode information
  @param  Info                  Mode information

  @retval EFI_UNSUPPORTED       return unsupported, As set mode handling is done internally

**/
EFI_STATUS
DisplayMCDxe_QueryMode (
  EFI_GRAPHICS_OUTPUT_PROTOCOL           *This,
  UINT32                                  ModeNumber,
  UINTN                                  *SizeOfInfo,
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info);

/**********************************************************************************************************************
 *   Structures
 **/

#endif // __DISPLAYMULTICORE_H__
