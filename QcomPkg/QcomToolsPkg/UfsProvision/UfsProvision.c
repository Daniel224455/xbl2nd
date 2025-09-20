/** @file UfsProvision.c
   
  UFS provision Application. 
 
  Copyright (c) 2018 Qualcomm Technologies, Inc. 
  All Rights Reserved. 
  Qualcomm Technologies Proprietary and Confidential.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 10/30/18   jt      Initial version
 
=============================================================================*/

#include <Library/QcomLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/EFIUfsProvision.h>

/**
  Entry point for UFS Provision. 

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS           Provision successful
  @retval other                 Failed to provision
**/
EFI_STATUS
EFIAPI
UfsProvisionMain (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
   CHAR8 **Argv;
   EFI_STATUS Status = EFI_DEVICE_ERROR; 
   EFI_UFS_PROV_PROTOCOL *UfsProvProtocol = NULL; 
   UINTN Argc;

   Status = gBS->LocateProtocol(&gEfiUfsProvProtocolGuid,
                                NULL,
                                (VOID **)&UfsProvProtocol);
   if (Status != EFI_SUCCESS)
   {
      DEBUG((EFI_D_ERROR, "Unable to locate gEfiUfsProvProtocolGuid, status 0x%x\n", Status));
      return Status; 
   }

   Status = GetCmdLineArgs (ImageHandle, &Argc, &Argv); 
   if (Status != EFI_SUCCESS)
   {
      DEBUG((EFI_D_ERROR, "UFSProvision: Failed to get command line arguments, status 0x%x\n", Status));
      return Status; 
   }

   if (1 != Argc)
   {
      Status = EFI_INVALID_PARAMETER; 
      goto End; 
   }

   Status = UfsProvProtocol->ProvisionUfs(UfsProvProtocol, Argv[0]); 
   if (Status != EFI_SUCCESS)
   {
      DEBUG((EFI_D_ERROR, "Failed to provision UFS, status 0x%x\n", Status));
      return Status; 
   }
   else
   {
      DEBUG((EFI_D_ERROR, "UFS Provisioning successful! Please power cycle the device.\n\n"));
   }

End:
   if (EFI_SUCCESS != Status)
   {
      DEBUG((EFI_D_ERROR, "\nUFSProvision Usage: start UfsProvision.efi [cfg file]\n\n"));
      DEBUG((EFI_D_ERROR, "Example: start UfsProvision.efi UfsProvision.cfg\n\n"));
      DEBUG((EFI_D_ERROR, "Note that the cfg file needs to be present in an FV\n\n"));
      DEBUG((EFI_D_ERROR, "If you want to use the default cfg file, just run: start UfsProvision.efi default\n\n"));
   }

   return EFI_SUCCESS; 
}

