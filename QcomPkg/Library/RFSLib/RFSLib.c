/** @file RFSLib.c

  RFS library implements the functions to dynamically populate the DSDT with base
  address and size parameters for Remotefs and RFSA Shared buffers. This file
  depends on the memory map configuration defined in uefiplat.cfg file.

  Copyright (c) 2014-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

**/
/*=============================================================================
                EDIT HISTORY


 when       who    what, where, why
 --------   ---     ----------------------------------------------------------
 04/19/18   jt      Make SCM call to share EFS region between APPS and MPSS 
 09/25/15   mj      Handle missing ADSP EFS partition in memory map.
 05/07/15   dks     Updated MPSS EFS memory name to look for.
 11/26/14   vm      Added debug logs for callback registration failures.
 06/20/14   vm      Created initial version.
 =============================================================================*/

#include <Uefi.h>
#include <Protocol/EFIAcpiPlatform.h>
#include <Protocol/EFIScm.h>
#include <Library/UefiCfgLib.h>
#include <Library/DebugLib.h>
#include <Guid/Acpi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Include/scm_sip_interface.h>

STATIC EFI_STATUS EFIAPI AmlUpdateRmtbBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
);

STATIC EFI_STATUS EFIAPI AmlUpdateRmtxBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
);

STATIC EFI_STATUS EFIAPI AmlUpdateRfmbBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
);

STATIC EFI_STATUS EFIAPI AmlUpdateRfmsBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
);

STATIC EFI_STATUS EFIAPI AmlUpdateRfabBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
);


EFI_STATUS
RFSLib_AMLVariableRegister (VOID)
{
  EFI_QCOM_ACPIPLATFORM_PROTOCOL *pEfiAcpiPlatformProtocol = NULL;
  EFI_STATUS Status;

  /* Variable for remotefs */
  CHAR8 AmlVariableRmtb[AML_NAMESTRING_LENGTH] =  {'R','M','T','B'};
  /* Variable for remotefs */
  CHAR8 AmlVariableRmtx[AML_NAMESTRING_LENGTH] =  {'R','M','T','X'};
  /* Variable for rfsa on mpss */
  CHAR8 AmlVariableRfmb[AML_NAMESTRING_LENGTH] =  {'R','F','M','B'};
  /* Variable for rfsa on mpss */
  CHAR8 AmlVariableRfms[AML_NAMESTRING_LENGTH] =  {'R','F','M','S'};
  /* Variable for rfsa on adsp */
  CHAR8 AmlVariableRfab[AML_NAMESTRING_LENGTH] =  {'R','F','A','B'};
  /* Variable for rfsa on adsp */
  CHAR8 AmlVariableRfas[AML_NAMESTRING_LENGTH] =  {'R','F','A','S'};


  Status = gBS->LocateProtocol(&gQcomAcpiPlatformProtocolGuid,
                                NULL, (VOID**) &pEfiAcpiPlatformProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "AML: Locate ACPI Protocol failed, Status = (0x%x)\r\n", Status));
    return Status;
  }

  Status = pEfiAcpiPlatformProtocol->AmlVariableRegister(
                                          pEfiAcpiPlatformProtocol,
                                          AmlVariableRmtb,
                                          GETMODE_PREFIX,
                                          NULL,
                                          AmlUpdateRmtbBufInfo);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "AML/RFS: CB register RmtbBufInfo failed, Status = (0x%x)\r\n", Status));
  }

  Status = pEfiAcpiPlatformProtocol->AmlVariableRegister(
                                          pEfiAcpiPlatformProtocol,
                                          AmlVariableRmtx,
                                          GETMODE_PREFIX,
                                          NULL,
                                          AmlUpdateRmtxBufInfo);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "AML/RFS: CB register RmtxBufInfo failed, Status = (0x%x)\r\n", Status));
  }

  Status = pEfiAcpiPlatformProtocol->AmlVariableRegister(
                                          pEfiAcpiPlatformProtocol,
                                          AmlVariableRfmb,
                                          GETMODE_PREFIX,
                                          NULL,
                                          AmlUpdateRfmbBufInfo);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "AML/RFS: CB register RfmbBufInfo failed, Status = (0x%x)\r\n", Status));
  }

  Status = pEfiAcpiPlatformProtocol->AmlVariableRegister(
                                          pEfiAcpiPlatformProtocol,
                                          AmlVariableRfms,
                                          GETMODE_PREFIX,
                                          NULL,
                                          AmlUpdateRfmsBufInfo);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "AML/RFS: CB register RfmsBufInfo failed, Status = (0x%x)\r\n", Status));
  }

  Status = pEfiAcpiPlatformProtocol->AmlVariableRegister(
                                          pEfiAcpiPlatformProtocol,
                                          AmlVariableRfab,
                                          GETMODE_PREFIX,
                                          NULL,
                                          AmlUpdateRfabBufInfo);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "AML/RFS: CB register RfabBufInfo failed, Status = (0x%x)\r\n", Status));
  }

  Status = pEfiAcpiPlatformProtocol->AmlVariableRegister(
                                          pEfiAcpiPlatformProtocol,
                                          AmlVariableRfas,
                                          GETMODE_PREFIX,
                                          NULL,
                                          AmlUpdateRfmsBufInfo);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_WARN, "AML/RFS: CB register RfmsBufInfo failed, Status = (0x%x)\r\n", Status));
  }

  return Status;
}



 /**
 In this callback function, client should update AML global variables obtained from
 ACPIPlatformDXE and pass it back. Since loading of ACPI tables into system
 memory is blocked till the callback functions complete, client drivers should
 return from callback functions as soon as possible.
 The memory to hold the variable entry is allocated AND released by ACPIPlatfromDXE.
 The variable entry should be modified in place.
 If the registered variable name is not valid, AmlVariableBufferSize will be 0, and
 AcpiTable is a NULL pointer. If this is an error condition, the callback
 function should print an error message and return an appropriate error code.
 It should not halt the system.

 AmlUpdateRmtbBufInfo is called by AcpiPlatformDxe to update the remotefs entry in DSDT
 table. This function sets the RemoteFS Shared buffer address in DSDT table.


 @param[in, out] AmlVariableBuffer         The entry in DSDT table to be updated.
 @param[in, out] AmlVariableBufferSize  The size of the buffer containing DSDT entry.

 @retval EFI_SUCCESS      The function executed successfully.
 @retval EFI_INVALID_PARAMETER AcpiTable is NULL or AcpiTableSize is
                          NULL or AcpiTableSize is less than the size of
                          ACPI_DSDT_TABLE
*/

/* Labels for Shared EFS regions, as defined in memory map for uefi from uefiplatWP.cfg */
STATIC CHAR8* RMFSLabel = "MPSS_EFS";
STATIC CHAR8* RFAALabel = "ADSP_EFS";

STATIC EFI_STATUS EFIAPI AmlUpdateRmtbBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  MemRegionInfo SharedEFSMemRegionInfo;
  UINTN RmfsAddress;

  if ((AmlVariableBuffer == NULL) || (*AmlVariableBuffer == NULL))
  {
    DEBUG ((EFI_D_WARN, " Could not find the AML variable for Remotefs buffer address in DSDT.\r\n"));
    return Status;
  }

  Status = GetMemRegionInfoByName (RMFSLabel, &SharedEFSMemRegionInfo);

  if (Status == EFI_SUCCESS)
  {
    RmfsAddress = SharedEFSMemRegionInfo.MemBase;

    /* Update the entries of the DSDT table in-place, by updating the buffer values */
    *((UINTN *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = RmfsAddress;
  }
  else
  {
    DEBUG ((EFI_D_WARN, " Could not find the MPSS_EFS region.\r\n"));
  }

  return Status;
}


STATIC EFI_STATUS EFIAPI AmlUpdateRmtxBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  hyp_memprot_assign_t *ScmParameters = NULL; 
  hyp_memprot_dstVM_perm_info_t DstVM[2] = {{AC_VM_HLOS, (VM_PERM_R|VM_PERM_W), (UINT64)NULL, 0}, 
                                            {AC_VM_MSS_MSA, (VM_PERM_R|VM_PERM_W), (UINT64)NULL, 0}}; 
  hyp_memprot_ipa_info_t Ipa; 
  MemRegionInfo SharedEFSMemRegionInfo;
  QCOM_SCM_PROTOCOL *pQcomScmProtocol = NULL; 
  UINT32 ScmBufferSize = 0; 
  UINT32 SrcVM = AC_VM_HLOS; 
  UINT64 RmfsSize;
  UINT64 Parameters[SCM_MAX_NUM_PARAMETERS] = {0};
  UINT64 Results[SCM_MAX_NUM_RESULTS] = {0};
  VOID *ScmData = NULL; 

  ScmParameters = (hyp_memprot_assign_t *)Parameters; 

  if ((AmlVariableBuffer == NULL) || (*AmlVariableBuffer == NULL))
  {
    DEBUG ((EFI_D_WARN, " Could not find the AML variable for Remotefs buffer length in DSDT.\r\n"));
    return Status;
  }

  Status = GetMemRegionInfoByName (RMFSLabel, &SharedEFSMemRegionInfo);

  if (Status == EFI_SUCCESS)
  {
    RmfsSize = SharedEFSMemRegionInfo.MemSize;

    /* Update the entries of the DSDT table in-place, by updating the buffer values */
    *((UINTN *) ((CHAR8 *)(*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = RmfsSize;

    /* Assign this memory region to HLOS and MPSS by issuing TZ SCM call  */
    Status = gBS->LocateProtocol(&gQcomScmProtocolGuid, NULL, (VOID**) &pQcomScmProtocol);
    if (EFI_ERROR(Status))
    {
       DEBUG((EFI_D_ERROR, "Locate SCM protocol failed, Status = (0x%x)\r\n", Status));
       return Status;
    }

    Ipa.IPAaddr = SharedEFSMemRegionInfo.MemBase;  
    Ipa.IPAsize = SharedEFSMemRegionInfo.MemSize; 

    /* Add 4 bytes to the total memory to be 64-bit aligned */
    ScmBufferSize = sizeof(hyp_memprot_ipa_info_t) + sizeof(SrcVM) + (2 * sizeof(hyp_memprot_dstVM_perm_info_t)) + 4; 

    ScmData = UncachedAllocatePool(ScmBufferSize); 
    if (ScmData == NULL)
    {
       DEBUG((EFI_D_ERROR, "MPSS_EFS ScmData NULL\r\n"));
       return EFI_OUT_OF_RESOURCES;
    }

    ScmParameters->IPAinfolist = (UINT64)ScmData; 
    CopyMem((VOID *)ScmParameters->IPAinfolist, &Ipa, sizeof(hyp_memprot_ipa_info_t));
    ScmParameters->IPAinfolistsize = sizeof(hyp_memprot_ipa_info_t); 

    ScmParameters->sourceVMlist = (UINT64)ScmData + sizeof(hyp_memprot_ipa_info_t); 
    CopyMem((VOID *)ScmParameters->sourceVMlist, &SrcVM, sizeof(SrcVM)); 
    ScmParameters->srcVMlistsize = sizeof(SrcVM);

    ScmParameters->destVMlist = (UINT64)ScmData + sizeof(hyp_memprot_ipa_info_t) + sizeof(SrcVM) + 4; // add 4 bytes to be 64-bit aligned 
    CopyMem((VOID *)ScmParameters->destVMlist, DstVM, 2 * sizeof(hyp_memprot_dstVM_perm_info_t)); 
    ScmParameters->destVMlistsize = 2 * sizeof(hyp_memprot_dstVM_perm_info_t);  
    ScmParameters->spare = 0; 

    Status = pQcomScmProtocol->ScmSipSysCall(pQcomScmProtocol, HYP_MEM_PROTECT_ASSIGN, HYP_MEM_PROTECT_ASSIGN_PARAM_ID,
                                             Parameters, Results);
    if (EFI_ERROR (Status))
    {
       DEBUG((EFI_D_ERROR, "MPSS_EFS ScmSipSysCall() failed, Status = (0x%x)\r\n", Status));
       return Status; 
    }
  }
  else
  {
    DEBUG ((EFI_D_WARN, " Could not find the MPSS_EFS region .\r\n"));
  }

  return Status;
}


STATIC EFI_STATUS EFIAPI AmlUpdateRfmbBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  MemRegionInfo SharedEFSMemRegionInfo;
  UINTN RfaaAddress;
  UINT64 RfaaSize;

  if ((AmlVariableBuffer == NULL) || (*AmlVariableBuffer == NULL))
  {
    DEBUG ((EFI_D_WARN, " Could not find the AML variable for MPSS RFSA buffer address in DSDT.\r\n"));
    return Status;
  }

  Status = GetMemRegionInfoByName (RFAALabel, &SharedEFSMemRegionInfo);

  if (Status == EFI_SUCCESS)
  {
    RfaaAddress = SharedEFSMemRegionInfo.MemBase;
    RfaaSize = SharedEFSMemRegionInfo.MemSize/2;

    /* Update the entries of the DSDT table in-place, by updating the buffer values */
    *((UINTN *) ((CHAR8 *) (*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = RfaaAddress + RfaaSize;
  }
  else
  {
	*((UINTN *) ((CHAR8 *) (*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINTN) 0x0;
    DEBUG ((EFI_D_WARN, " Could not find the ADSP_EFS .\r\n"));
  }

  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI AmlUpdateRfmsBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  MemRegionInfo SharedEFSMemRegionInfo;
  UINT64 RfmaSize;

  if ((AmlVariableBuffer == NULL) || (*AmlVariableBuffer == NULL))
  {
    DEBUG ((EFI_D_WARN, " Could not find the AML variable for RFSA buffer length in DSDT.\r\n"));
    return Status;
  }

  Status = GetMemRegionInfoByName (RFAALabel, &SharedEFSMemRegionInfo);

  if (Status == EFI_SUCCESS)
  {
    RfmaSize = SharedEFSMemRegionInfo.MemSize/2;

    /* Update the entries of the DSDT table in-place, by updating the buffer values */
    *((UINTN *) ((CHAR8 *) (*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = RfmaSize;
  }
  else
  {
	*((UINTN *) ((CHAR8 *) (*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINTN) 0x0;
    DEBUG ((EFI_D_WARN, " Could not find the ADSP_EFS .\r\n"));
  }

  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI AmlUpdateRfabBufInfo (
  IN OUT VOID         **AmlVariableBuffer,
  IN OUT UINTN          AmlVariableBufferSize
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  MemRegionInfo SharedEFSMemRegionInfo;
  UINTN RfaaAddress;

  if ((AmlVariableBuffer == NULL) || (*AmlVariableBuffer == NULL))
  {
    DEBUG ((EFI_D_WARN, " Could not find the AML variable for ADSP RFSA buffer address in DSDT.\r\n"));
    return Status;
  }

  Status = GetMemRegionInfoByName (RFAALabel, &SharedEFSMemRegionInfo);

  if (Status == EFI_SUCCESS)
  {
    RfaaAddress = SharedEFSMemRegionInfo.MemBase;

    /* Update the entries of the DSDT table in-place, by updating the buffer values */
    *((UINTN *) ((CHAR8 *) (*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = RfaaAddress;
  }
  else
  {
	*((UINTN *) ((CHAR8 *) (*AmlVariableBuffer) + AML_NAMESPACE_HEADER_SIZE)) = (UINTN) 0x0;
    DEBUG ((EFI_D_WARN, " Could not find the ADSP_EFS .\r\n"));
  }

  return EFI_SUCCESS;
}
