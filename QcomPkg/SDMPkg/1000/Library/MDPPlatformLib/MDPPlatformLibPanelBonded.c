/*=============================================================================
  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/EFIPmicGpio.h>
#include <Protocol/EFIPmicClkBuff.h>
#include <Protocol/EFITlmm.h>
#include <Protocol/EFIPmicLpg.h>
#include <Protocol/EFIPmicGpio.h>
#include <Protocol/EFIPmicPwm.h>
#include <Protocol/EFIPmicVreg.h>
#include <Protocol/EFIPmicWled.h>
#include <Protocol/EFIPmicIbb.h>
#include <Protocol/EFIPmicLab.h>
#include <Protocol/EFIPmicAb.h>
#include <Protocol/EFIPmicLcdb.h>
#include <Protocol/EFIPmicVersion.h>
#include <api/pmic/pm/pm_version.h>
#include "DDIChipInfo.h"
#include "MDPSystem.h"
#include "MDPPeripherals.h"
#include "MDPPlatformLibPanelCommon.h"
#include "DisplayUtils.h"
#include "pmapp_npa.h"
#include "DDITlmm.h"

static MDP_Status InterfaceResourcesPowerUp(uint32 uDPDeviceIDMapping, EFI_TLMM_PROTOCOL *TLMMProtocol);
static MDP_Status InterfaceResourcesPowerDown(uint32 uDPDeviceIDMapping, EFI_TLMM_PROTOCOL *TLMMProtocol);

/**********************************************************************************************
*
* FUNCTION: Panel_BondedDP_PowerUp()
*
* DESCRIPTION:
*   The power up function for DP bonded mode panels; must be in sync with parameters set in
*   panel XML file. 
*
***********************************************************************************************/
MDP_Status Panel_BondedDP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                    Status            = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL            *TLMMProtocol      = NULL;

  if (MDP_STATUS_OK != (Status =  PlatformClientInit(eDisplayId, pPowerParams)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to initialize handle for internal DP bonded display NPA node.\n"));
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /*
    * Voting for Display NPA node to be ON
    */
    npa_issue_required_request(pPowerParams->sNPAClient[eDisplayId], PMIC_NPA_MODE_ID_GENERIC_ACTIVE);

    if (MDP_STATUS_OK != InterfaceResourcesPowerUp(MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1, TLMMProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to power up resources for DP device %d!\n", MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1));
      Status = MDP_STATUS_FAILED;
    }

    if (MDP_STATUS_OK != InterfaceResourcesPowerUp(MDP_DRIVER_DP_DEVICE_ID_DP, TLMMProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to power up resources for DP device %d!\n", MDP_DRIVER_DP_DEVICE_ID_DP));
      Status = MDP_STATUS_FAILED;
    }
  }

  return Status;
}

/**********************************************************************************************
*
* FUNCTION: Panel_BondedDP_PowerDown()
*
* DESCRIPTION:
*   The power down function for DP bonded mode panels; must be in sync with parameters set in
*   panel XML file. 
*
***********************************************************************************************/
MDP_Status Panel_BondedDP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                    Status            = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL             *TLMMProtocol     = NULL;

  if (NULL == pPowerParams->sNPAClient[eDisplayId])
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: NULL Handle for internal DP bonded display NPA node.\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    if (MDP_STATUS_OK != InterfaceResourcesPowerDown(MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1, TLMMProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to power down resources for DP device %d!\n", MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1));
      Status = MDP_STATUS_FAILED;
    }

    if (MDP_STATUS_OK != InterfaceResourcesPowerDown(MDP_DRIVER_DP_DEVICE_ID_DP, TLMMProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to power down resources for DP device %d!\n", MDP_DRIVER_DP_DEVICE_ID_DP));
      Status = MDP_STATUS_FAILED;
    }
  }

  return Status;
}

/**********************************************************************************************
*
* FUNCTION: InterfaceResourcesPowerUp()
*
* DESCRIPTION:
*   The power up routines for the DP devices.
*
***********************************************************************************************/
static MDP_Status InterfaceResourcesPowerUp(uint32 uDPDeviceIDMapping, EFI_TLMM_PROTOCOL *TLMMProtocol)
{
  MDP_Status Status = MDP_STATUS_OK;
  
  switch (uDPDeviceIDMapping)
  {
    case MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_0:
      /* Configure gpio for chip select OE_N of USB-DP Mux logic (SBU_SW_OE) */      
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(GPIO_USB_DP0_FSA_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), DAL_TLMM_GPIO_ENABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for USB_DPo_FSA_EN failed!, uDPDeviceIDMapping=%d\n", GPIO_USB_DP0_FSA_EN, MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_0));
        Status = MDP_STATUS_FAILED;
      }
      if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(GPIO_USB_DP0_FSA_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_LOW_VALUE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: GPIO_USB_DP0_FSA_EN line Low failed!, uDPDeviceIDMapping=%d\n", MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_0));
        Status = MDP_STATUS_FAILED;
      }
	  
      /* Configure AUX switch chip through I2C protocol */
      DPAuxSwitchEnable();

      /* Route HPD signal from panel to DP controller */
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_HPD, 2, GPIO_INPUT, GPIO_NO_PULL, GPIO_2MA), TLMM_GPIO_ENABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for HPD line failed!, uDPDeviceIDMapping=%d\n", GPIO_USB_HPD, MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_0));
        Status = MDP_STATUS_FAILED;
      }
    break;
    case MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1:
      /* configure AUX switch chip through two GPIOs */
      /* Configure USB_DP_OE_N (EN_N) GPIO */
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), DAL_TLMM_GPIO_ENABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for USB_DP1_SW_EN failed!, uDPDeviceIDMapping=%d\n", GPIO_USB_DP1_SW_EN, MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1));
        Status = MDP_STATUS_FAILED;
      }

      if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_LOW_VALUE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: GPIO_USB_DP1_SW_EN line Low failed!, uDPDeviceIDMapping=%d\n", MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1));
        Status = MDP_STATUS_FAILED;
      }
      
      /* Configure gpio for chip select OE_N of USB-DP Mux logic (SBU_SW_OE) */      
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_SEL, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), DAL_TLMM_GPIO_ENABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for USB_DP1_SW_SEL failed!, uDPDeviceIDMapping=%d\n", GPIO_USB_DP1_SW_SEL, MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1));
        Status = MDP_STATUS_FAILED;
      }

      if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_SEL, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_LOW_VALUE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: GPIO_USB_DP1_SW_SEL line Low failed!, uDPDeviceIDMapping=%d\n", MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1));
        Status = MDP_STATUS_FAILED;
      }

      /* Route HPD signal from panel to DP controller */
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_HPD, 2, GPIO_INPUT, GPIO_NO_PULL, GPIO_2MA), TLMM_GPIO_ENABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for HPD line failed!, uDPDeviceIDMapping=%d\n", GPIO_USB_HPD, MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1));
        Status = MDP_STATUS_FAILED;
      }
    break;
    case MDP_DRIVER_DP_DEVICE_ID_DP:
      /* Route HPD signal from panel to DP controller */
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_DP_HPD, 1, GPIO_INPUT, GPIO_NO_PULL, GPIO_2MA), TLMM_GPIO_ENABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for HPD line failed!, uDPDeviceIDMapping=%d\n,", GPIO_DP_HPD, MDP_DRIVER_DP_DEVICE_ID_DP));
        Status = MDP_STATUS_FAILED;
      }
    break;
   }

  return Status;
}

/**********************************************************************************************
*
* FUNCTION: InterfaceResourcesPowerDown()
*
* DESCRIPTION:
*   The power down routines for the DP devices.
*
***********************************************************************************************/
static MDP_Status InterfaceResourcesPowerDown(uint32 uDPDeviceIDMapping, EFI_TLMM_PROTOCOL *TLMMProtocol)
{
  MDP_Status Status = MDP_STATUS_OK;
  
  switch (uDPDeviceIDMapping)
  {
    case MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_0:
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_HPD, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for HPD line failed\n", GPIO_USB_HPD));
        Status = MDP_STATUS_FAILED;
      }
    break;
    case MDP_DRIVER_DP_DEVICE_ID_ALT_MODE_1:
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_DP1_SW_SEL, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Failed to disable GPIO %d for DP1_SW_SEL pin\n", GPIO_USB_DP1_SW_SEL));
        Status = MDP_STATUS_FAILED;
      }

      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_DP1_SW_EN, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Failed to disable GPIO %d for DP1_SW_EN pin\n", GPIO_USB_DP1_SW_EN));
        Status = MDP_STATUS_FAILED;
      }

      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_HPD, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for HPD line failed\n", GPIO_USB_HPD));
        Status = MDP_STATUS_FAILED;
      }
    break;
    case MDP_DRIVER_DP_DEVICE_ID_DP:
      if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_DP_HPD, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for HPD line failed\n", GPIO_DP_HPD));
        Status = MDP_STATUS_FAILED;
      }
    break;
   }

  return Status;
}

#ifdef __cplusplus
}
#endif
