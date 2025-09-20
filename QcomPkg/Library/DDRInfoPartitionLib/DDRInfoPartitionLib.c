/*
 * @file DDRInfoPartitionLib.c
 *
 * @brief DDRInfoPartitionLib functions
 *
 * Copyright (c) 2017-2018 by Qualcomm Technologies, Inc. 
 * All Rights Reserved.
 *
 */
/*=======================================================================
                        Edit History

when       who     what, where, why
--------   ----    --------------------------------------------------- 
09/26/17   daisond Calling ddr_get_max_channels to get max ddr channels
04/27/17   ddk     Initital Version
========================================================================*/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DDRInfoPartitionLib.h>
#include <com_dtypes.h>
#include <Library/smem.h> 
#include <MemRegionInfo.h>
#include "boot_util.h"
#include "ddr_drivers.h"
#include <Library/npa.h>
#include <api/systemdrivers/icbarb.h>
#include <api/systemdrivers/busywait.h>
#include <Library/npa_resource.h>

npa_client_handle reqClientDDR = NULL;
ICBArb_MasterSlaveType ddrMasterSlave[] =
    {{ICBID_MASTER_APPSS_PROC, ICBID_SLAVE_EBI1}};

/*==============================================================================

  FUNCTION      GetDDRDetails

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRDetails in Protocol/EFIDDRGetConfig.h

==============================================================================*/

EFI_STATUS
GetDDRDetails (IN OUT struct ddr_details_entry_info *ddr_detail)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 DDRInfoBuffSz = 0;
  VOID   *pDDRTable = NULL;

  if ((ddr_detail == NULL))
    return EFI_INVALID_PARAMETER;
  /* Get the DDR details table */
  pDDRTable = smem_get_addr(DDRINFO_UEFI_SMEM_ID, (uint32*)&DDRInfoBuffSz);
  if (pDDRTable == NULL)
  {
    /*NOTE: We should be here only if SMEM is not initialized (virtio, SmemNullLib)*/
    DEBUG ((EFI_D_WARN, "WARNING: Unable to read DDR Info table from SMEM\n"));
    return EFI_NOT_READY;
  }

  qmemscpy((void *)ddr_detail, sizeof(ddr_details_entry), pDDRTable, sizeof(ddr_details_entry));   

  return Status;
}



/*==============================================================================

  FUNCTION      EFI_GetDDRFreqTable

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRFreqTable in Protocol/EFIDDRGetConfig.h

==============================================================================*/

EFI_STATUS
GetDDRFreqTable (IN OUT struct ddr_freq_plan_entry_info   *clk_plan_tbl)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 DDRInfoBuffSz = 0;
  VOID   *pDDRFreqTbl = NULL;
  VOID   *pDDRInfoBase = NULL;
  
  

  if ((clk_plan_tbl == NULL))
    return EFI_INVALID_PARAMETER;
  /* Get the DDR details table */
  pDDRInfoBase = smem_get_addr(DDRINFO_UEFI_SMEM_ID, (uint32*)&DDRInfoBuffSz);
  pDDRFreqTbl = (void*)&(((ddr_details_entry*)pDDRInfoBase)->ddr_freq_tbl);
  
  if (pDDRFreqTbl == NULL)
  {
    /*NOTE: We should be here only if SMEM is not initialized (virtio, SmemNullLib)*/
    DEBUG ((EFI_D_WARN, "WARNING: Unable to read DDR Freq Plan table from SMEM\n"));
    return EFI_NOT_READY;
  }

  qmemscpy((void *)clk_plan_tbl, (sizeof(struct ddr_freq_plan_entry_info)), pDDRFreqTbl, (sizeof(struct ddr_freq_plan_entry_info))); 
  
  return Status;
}


/*==============================================================================

  FUNCTION      EFI_GetDDRFreq

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRFreq in Protocol/EFIDDRGetConfig.h

==============================================================================*/

EFI_STATUS
GetDDRFreq (IN OUT UINT32    *ddr_freq)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 DDRInfoBuffSz = 0;
  VOID   *pDDRFreqTbl = NULL;
  VOID   *pDDRInfoBase = NULL;
  

  if ((ddr_freq == NULL))
    return EFI_INVALID_PARAMETER;
  /* Get the DDR details table */
  pDDRInfoBase = smem_get_addr(DDRINFO_UEFI_SMEM_ID, (uint32*)&DDRInfoBuffSz);
  pDDRFreqTbl = (void*)&(((ddr_details_entry*)pDDRInfoBase)->ddr_freq_tbl);

  if (pDDRInfoBase == NULL)
  {
    /*NOTE: We should be here only if SMEM is not initialized (virtio, SmemNullLib)*/
    DEBUG ((EFI_D_WARN, "WARNING: Unable to read DDR Info table from SMEM\n"));
    return EFI_NOT_READY;
  }

  *ddr_freq = 1000000 / *(UINT32*)(((struct ddr_freq_plan_entry_info *)pDDRFreqTbl)->clk_period_address);	
  
  return Status;
}

/*==============================================================================

  FUNCTION      EFI_GetDDRMaxEnabledFreq

  DESCRIPTION   See EFI_DDRGetInfo_GetDDRMaxEnabledFreq in Protocol/EFIDDRGetConfig.h

==============================================================================*/

EFI_STATUS
GetDDRMaxEnabledFreq (IN OUT UINT32    *ddr_freq)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 DDRInfoBuffSz = 0;
  VOID   *pDDRFreqTbl = NULL;
  VOID   *pDDRInfoBase = NULL;
  UINT32  idx=0;
  

  if ((ddr_freq == NULL))
    return EFI_INVALID_PARAMETER;
  /* Get the DDR details table */
  pDDRInfoBase = smem_get_addr(DDRINFO_UEFI_SMEM_ID, (uint32*)&DDRInfoBuffSz);
  pDDRFreqTbl = (void*)&(((ddr_details_entry*)pDDRInfoBase)->ddr_freq_tbl);

  if (pDDRInfoBase == NULL)
  {
    /*NOTE: We should be here only if SMEM is not initialized (virtio, SmemNullLib)*/
    DEBUG ((EFI_D_WARN, "WARNING: Unable to read DDR Info table from SMEM\n"));
    return EFI_NOT_READY;
  }

  idx = (((struct ddr_freq_plan_entry_info *)pDDRFreqTbl)->num_ddr_freqs);
  do{
       *ddr_freq =  (((struct ddr_freq_plan_entry_info *)pDDRFreqTbl)->ddr_freq[idx].freq_khz);	
  }while(((struct ddr_freq_plan_entry_info *)pDDRFreqTbl)->ddr_freq[idx--].enable != 1);
  
  return Status;
}


/**
  Remove DDR Request Client for DDR frequency switching
**/
void deinit_ddr_clk()
{
  // destroy the npa clients
  npa_complete_request(reqClientDDR);
}

/**
  Switch DDR frequency
**/
UINT32 switch_ddr_freq(UINT32 freq_khz)
{
  UINT32 set_freq_khz = 0;
  UINT64 bandwidth = 0;
  ICBArb_RequestType aRequest[1];

  if (reqClientDDR == NULL)
  {
    // init the npa request nodes
    reqClientDDR = npa_create_sync_client_ex("/icb/arbiter",
                                             "DDR UEFI req client",
                                             NPA_CLIENT_VECTOR,
                                             sizeof(ddrMasterSlave),
                                             &ddrMasterSlave);
  }
  ICBArb_Request3Type type3;
  aRequest[0].arbType = ICBARB_REQUEST_TYPE_3;
  /* Bandwidth is 95% of (DDR_Frequency(in MHz) * 4) */
  bandwidth = (((freq_khz / 1000) * 4 * 95) / 100); 
  type3.uIb =  bandwidth << 20; 
  type3.uAb = 0;
  type3.uLatencyNs = 0;
  aRequest[0].arbData = (ICBArb_RequestUnionType)type3;

  npa_issue_vector_request(reqClientDDR,
                           sizeof(aRequest) / sizeof(npa_resource_state),
                           (npa_resource_state *)aRequest);

  return set_freq_khz;
}

/*==============================================================================

  FUNCTION      EFI_SetDDRFreq

  DESCRIPTION   See EFI_DDRGetInfo_SetDDRFreq in Protocol/EFIDDRGetConfig.h

==============================================================================*/

EFI_STATUS
SetDDRFreq (IN UINT32    ddr_freq)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 DDRInfoBuffSz = 0;
  VOID       *pDDRTable = NULL;

 
  /* Get the DDR details table */
  pDDRTable = smem_get_addr(DDRINFO_UEFI_SMEM_ID, (uint32*)&DDRInfoBuffSz);
  if (pDDRTable == NULL)
  {
    /*NOTE: We should be here only if SMEM is not initialized (virtio, SmemNullLib)*/
    DEBUG ((EFI_D_WARN, "WARNING: Unable to read DDR Info table from SMEM\n"));
    return EFI_NOT_READY;
  }
  
  switch_ddr_freq(ddr_freq);

  return Status;
}


