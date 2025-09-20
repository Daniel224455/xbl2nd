/*============================================================================
  FILE:         DDRInfoDxe.c

  OVERVIEW:     Implementation of the DDRGetInfo EFI protocol interface

  DEPENDENCIES: None

                Copyright (c) 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Confidential and Proprietary,
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  when        who      what, where, why
  ----------  ---      -----------------------------------------------------------
  2017-04-19  daisond  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFIDDRGetConfig.h>
#include <Library/DDRInfoPartitionLib.h>


/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

/*===========================================================================*/
/* EFI_GetDDRDetails */ 
/**
  @par Summary
  Gets information about the DDR

  @param[in]  This          Pointer to the EFI_DDRGETINFO_PROTOCOL instance.
  @param[out] PlatformInfo  Pointer to the DDR information

  @return
  EFI_SUCCESS        -- Function completed successfully.  \n
*/


static EFI_STATUS
EFI_GetDDRDetails(
	IN EFI_DDRGETINFO_PROTOCOL *This,
	OUT struct ddr_details_entry_info	*DdrDetails
	);

static EFI_STATUS
EFI_GetDDRFreqTable(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT struct ddr_freq_plan_entry_info   *clk_plan_tbl
   );
   
static EFI_STATUS
EFI_GetDDRFreq(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT UINT32                 *ddr_freq
   );
   
static EFI_STATUS
EFI_SetDDRFreq(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   IN UINT32                   ddr_freq
   );
   
static EFI_STATUS
EFI_GetDDRMaxEnabledFreq(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   IN UINT32                  *ddr_freq
   );
  
/*----------------------------------------------------------------------------
 * Function Definitions
 * -------------------------------------------------------------------------*/

/*==============================================================================

  FUNCTION      EFI_GetDDRDetails

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRDetails in Protocol/EFIDDRGetConfig.h

==============================================================================*/
EFI_STATUS
EFI_GetDDRDetails (
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT struct ddr_details_entry_info    *DdrDetails
   )
{
  EFI_STATUS	  Status;
  if (DdrDetails == NULL)
    return EFI_INVALID_PARAMETER;
  Status = GetDDRDetails(DdrDetails);

  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;
  

#ifdef PRE_SIL
  return EFI_UNSUPPORTED;
#else
  return EFI_SUCCESS;
#endif
}

/*==============================================================================

  FUNCTION      EFI_GetDDRFreqTable

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRFreqTable in Protocol/EFIDDRGetConfig.h

==============================================================================*/
EFI_STATUS
EFI_GetDDRFreqTable(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT struct ddr_freq_plan_entry_info   *clk_plan_tbl
   )
{
  EFI_STATUS	  Status;
  if (clk_plan_tbl == NULL)
    return EFI_INVALID_PARAMETER;
  Status = GetDDRFreqTable(clk_plan_tbl);

  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;


#ifdef PRE_SIL
  return EFI_UNSUPPORTED;
#else
  return EFI_SUCCESS;
#endif
}

/*==============================================================================

  FUNCTION      EFI_GetDDRMaxEnabledFreq

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRMaxEnabledFreq in Protocol/EFIDDRGetConfig.h

==============================================================================*/
EFI_STATUS
EFI_GetDDRMaxEnabledFreq(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT UINT32                 *ddr_freq
   )
{
  EFI_STATUS	  Status;
  if (ddr_freq == NULL)
    return EFI_INVALID_PARAMETER;
  Status = GetDDRMaxEnabledFreq(ddr_freq);

  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;
  

#ifdef PRE_SIL
  return EFI_UNSUPPORTED;
#else
  return EFI_SUCCESS;
#endif
}

/*==============================================================================

  FUNCTION      EFI_GetDDRFreq

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRFreq in Protocol/EFIDDRGetConfig.h

==============================================================================*/
EFI_STATUS
EFI_GetDDRFreq(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT UINT32                 *ddr_freq
   )
{
  EFI_STATUS	  Status;
  if (ddr_freq == NULL)
    return EFI_INVALID_PARAMETER;
  Status = GetDDRFreq(ddr_freq);

  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;
  

#ifdef PRE_SIL
  return EFI_UNSUPPORTED;
#else
  return EFI_SUCCESS;
#endif
}

/*==============================================================================

  FUNCTION      EFI_SetDDRFreq

  DESCRIPTION   See EFI_DDRGetInfo_SetDDRFreq in Protocol/EFIDDRGetConfig.h

==============================================================================*/
EFI_STATUS
EFI_SetDDRFreq(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   IN UINT32                   ddr_freq
   )
{
  EFI_STATUS	  Status;
  
  Status = SetDDRFreq(ddr_freq);

  if (Status != EFI_SUCCESS)
    return EFI_UNSUPPORTED;
  

#ifdef PRE_SIL
  return EFI_UNSUPPORTED;
#else
  return EFI_SUCCESS;
#endif
}


STATIC EFI_DDRGETINFO_PROTOCOL DDRGetInfoProtocol = 
{
  EFI_DDRGETINFO_PROTOCOL_REVISION,
  EFI_GetDDRDetails,
  EFI_GetDDRFreqTable,
  EFI_GetDDRFreq,
  EFI_SetDDRFreq,
  EFI_GetDDRMaxEnabledFreq
};



/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
DDRInfoEntryPoint(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{

   EFI_STATUS Status;
   
   Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, 
													&gEfiDDRGetInfoProtocolGuid, 
													&DDRGetInfoProtocol, 
													NULL, NULL);
   return Status;

}

