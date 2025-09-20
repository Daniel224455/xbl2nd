/*=============================================================================

  File: MDPPlatformLibPanelEdp.c

  MDP platform EDP panel functions

  Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
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

#define CLS_GPIO_VLCM_3V3_EN          130
#define PM_GPIO_EDP_12V_SOURCE        EFI_PM_GPIO_1
#define PM_GPIO_EDP_PWM_EN            EFI_PM_GPIO_8
#define PM_GPIO_EDP_PWM_PULSE         EFI_PM_GPIO_10
#define PMIC_C_DEV_INDEX              PMIC_C 
#define PMIC_E_DEV_INDEX              PMIC_E 
#define PMIC_RSRC_INDEX               EFI_PM_PWM_RSRC_PWM_2

#define MAXIMUM_VALUE(bits)           ((1<<bits)-1)
#define PERCENT_MAX                   100
#define CEIL(a,b)                     (((a)+(b-1))/b)

/**********************************************************************************************
*
* FUNCTION: Panel_CLS_PowerUp()
*
* DESCRIPTION:
*   Panel power up sequence for CLS Platform.
*
***********************************************************************************************/
MDP_Status Panel_CLS_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                   Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL           *TLMMProtocol     = NULL;
  EFI_QCOM_PMIC_GPIO_PROTOCOL *PmicGpioProtocol = NULL;

  if (NULL == pPowerParams->sEdpNPAClient)
  {
    pPowerParams->sEdpNPAClient = npa_create_sync_client(PMIC_NPA_GROUP_ID_DISP_EDP, "DisplayEdp", NPA_CLIENT_REQUIRED);
  }

  if (NULL == pPowerParams->sEdpNPAClient)
  {
    DEBUG ((EFI_D_ERROR, "DisplayDxe: Failed to retrieve NPA Display Handle for EDP Display\n"));
    Status =  MDP_STATUS_FAILED;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicGpioProtocolGuid, NULL, (VOID **)&PmicGpioProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate PMIC GPIO protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /*
     * Voting for Display NPA node to be ON
     */
    npa_issue_required_request(pPowerParams->sEdpNPAClient, PMIC_NPA_MODE_ID_GENERIC_ACTIVE);

    MDP_OSAL_DELAYMS(10);   /* delay 10ms to allow power grid to settle */

    /* Enable pmic gpio */
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_C_DEV_INDEX,  
                                                             PM_GPIO_EDP_PWM_EN,
                                                             EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                             EFI_PM_GPIO_VIN0,
                                                             EFI_PM_GPIO_SOURCE_GND,
                                                             EFI_PM_GPIO_OUT_BUFFER_LOW,
                                                             TRUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed!\n"));
    }

    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_C_DEV_INDEX, PM_GPIO_EDP_PWM_PULSE,
                                                            EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            EFI_PM_GPIO_VIN0,
                                                            EFI_PM_GPIO_SOURCE_SPECIAL_FUNCTION1,
                                                            EFI_PM_GPIO_OUT_BUFFER_LOW,
                                                            FALSE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed! = %d \n", Status));
    }

    /* enable 12V source */
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_E_DEV_INDEX, PM_GPIO_EDP_12V_SOURCE,
                                                            EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            EFI_PM_GPIO_VIN1,
                                                            EFI_PM_GPIO_SOURCE_GND,
                                                            EFI_PM_GPIO_OUT_BUFFER_LOW,
                                                            TRUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed! = %d \n", Status));
    }
    
    MDP_OSAL_DELAYUS(200);
    
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_VLCM_3V3_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for VLCM_3V3_EN failed!\n", CLS_GPIO_VLCM_3V3_EN));
    }
    /* Pull high GPIO 88 to enable VLCM_3V3 */
    else if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_VLCM_3V3_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Enabling GPIO %d for VLCM_3V3_EN line high failed!\n", CLS_GPIO_VLCM_3V3_EN));
    }

    /* route HDP signal from panel to eDP controller */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_EDP_HPD, 1, GPIO_INPUT, GPIO_NO_PULL, GPIO_2MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for HPD line failed\n", GPIO_EDP_HPD));
    }
  }
  
  return Status;
}

/**********************************************************************************************
*
* FUNCTION: Panel_CLS_PowerDown()
*
* DESCRIPTION:
*   Panel power down sequence for CLS platform.
*
***********************************************************************************************/
MDP_Status Panel_CLS_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                       Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL               *TLMMProtocol     = NULL;
  EFI_QCOM_PMIC_GPIO_PROTOCOL     *PmicGpioProtocol = NULL;
  
  if (NULL == pPowerParams->sEdpNPAClient)
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: NULL Handle for Primary display NPA node.\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicGpioProtocolGuid, NULL, (VOID **)&PmicGpioProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate PMIC GPIO protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /* VLCM_3V3 (disable pull up for BL_PWM of LCD panel): Turn off GPIO130 */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_VLCM_3V3_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), GPIO_LOW_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: pull GPIO %d for VLCM_3V3_EN line low failed!\n", CLS_GPIO_VLCM_3V3_EN));
    }
    else if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_VLCM_3V3_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for CLS_GPIO_VLCM_3V3_EN failed!\n", CLS_GPIO_VLCM_3V3_EN));
    }
  
    MDP_OSAL_DELAYUS(100);

    /* Enable pmic gpio */
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_C_DEV_INDEX,  
                                                             PM_GPIO_EDP_PWM_EN,
                                                             EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                             EFI_PM_GPIO_VIN0,
                                                             EFI_PM_GPIO_SOURCE_GND,
                                                             EFI_PM_GPIO_OUT_BUFFER_LOW,
                                                             FALSE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed!\n"));
    }
    
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_C_DEV_INDEX, 
                                                            PM_GPIO_EDP_PWM_PULSE,
                                                            EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            EFI_PM_GPIO_VIN0,
                                                            EFI_PM_GPIO_SOURCE_SPECIAL_FUNCTION1,
                                                            EFI_PM_GPIO_OUT_BUFFER_LOW,
                                                            FALSE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed! = %d \n", Status));
    }

    /* disable edp 12V source */
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_E_DEV_INDEX, PM_GPIO_EDP_12V_SOURCE,
                                                            EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            EFI_PM_GPIO_VIN1,
                                                            EFI_PM_GPIO_SOURCE_GND,
                                                            EFI_PM_GPIO_OUT_BUFFER_LOW,
                                                            FALSE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed! = %d \n", Status));
    }
    
    MDP_OSAL_DELAYUS(100);
    
    /*
    * Voltage Regulators
    */
    npa_complete_request(pPowerParams->sEdpNPAClient);           //Complete the request to power rails
    
    MDP_OSAL_DELAYUS(100);
  }
  
  return Status;
}

/**********************************************************************************************
*
* FUNCTION: Panel_CLS_Peripheral_Power()
*
* DESCRIPTION:
*   Secondary power sequence for other PMIC modules specific to CLS platform.
*
***********************************************************************************************/
MDP_Status Panel_CLS_Peripheral_Power(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp)
{
  MDP_Status Status = MDP_STATUS_OK;
  
  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_CLS_Brightness_Enable()
*
* DESCRIPTION:
*   Enable panel brightness for the CLS platform.
*
***********************************************************************************************/
MDP_Status Panel_CLS_Brightness_Enable(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                  Status          = MDP_STATUS_OK;
  
  /* GPIOs for PWM are already enabled in PowerUp and PWM config is done as part of Brightness_Level */
  
  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_CLS_Brightness_Level()
*
* DESCRIPTION:
*   The function can control the panel brightness level.
*
***********************************************************************************************/
MDP_Status Panel_CLS_Brightness_Level(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig)
{
  MDP_Status                   Status          = MDP_STATUS_OK;
  EFI_QCOM_PMIC_PWM_PROTOCOL  *PmicPwmProtocol = NULL;
  MDP_PMICPWMConfigurationType sPWMConfiguration;

  MDP_OSAL_MEMZERO(&sPWMConfiguration, sizeof(MDP_PMICPWMConfigurationType));
  
  if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicPwmProtocolGuid, NULL, (VOID **)&PmicPwmProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate PMIC GPIO protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    uint32 uLevel = CEIL(pBacklightConfig->uLevel*MAXIMUM_VALUE(9), PERCENT_MAX);  /* Convert level to a 9bit value */
   
    /* Get PWM freq-Predivider,Exponent */
    if (EFI_SUCCESS != (Status = MDP_CalculatePWMValue(pBacklightConfig->uPwmDesiredFreqHz, &sPWMConfiguration, pBacklightConfig->uNumBits)))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: MDP_CalculatePWMValue failed!\n"));
    }
    else if (EFI_SUCCESS != (Status = PmicPwmProtocol->PwmConfig(PMIC_C_DEV_INDEX, PMIC_RSRC_INDEX, 0, sPWMConfiguration.uPreDivider, sPWMConfiguration.uExponent, EFI_PM_PWM_CLK_19_2_MHZ, sPWMConfiguration.uPwmBitWidth)))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: PWM configuration for PMIC_RSRC_INDEX failed!\n"));
    }
    else if (EFI_SUCCESS != (Status = PmicPwmProtocol->SetPwmValue(PMIC_C_DEV_INDEX, PMIC_RSRC_INDEX, uLevel)))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: PWM set value for PMIC_RSRC_INDEX failed!\n"));
    }
  }

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_CLS_Reset()
*
* DESCRIPTION:
*   Panel reset sequence for CLS Platform.
*
***********************************************************************************************/
MDP_Status Panel_CLS_Reset(MDPPlatformPanelResetInfo *pResetInfo)
{
  MDP_Status                    Status            = MDP_STATUS_OK;
  
  //#TODO#IMPLEMENT
  return Status;
}


#ifdef __cplusplus
}
#endif

