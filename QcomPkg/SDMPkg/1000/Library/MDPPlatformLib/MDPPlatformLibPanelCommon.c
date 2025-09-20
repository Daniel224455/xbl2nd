/*=============================================================================

  File: MDPPlatformLibPanelCommon.c

  MDP default platform panel functions

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
#include <Protocol/EFIPmicVreg.h>
#include <Protocol/EFIPmicWled.h>
#include <Protocol/EFIPmicPwm.h>
#include <Protocol/EFIPmicIbb.h>
#include <Protocol/EFIPmicLab.h>
#include <Protocol/EFIPmicAb.h>
#include <Protocol/EFIPmicLcdb.h>
#include <Protocol/EFIPmicVersion.h>
#include "DDIChipInfo.h"
#include "MDPSystem.h"
#include "MDPPeripherals.h"
#include "MDPPlatformLibPanelCommon.h"
#include "DisplayUtils.h"
#include "pmapp_npa.h"
#include "DDITlmm.h"
#include "HALDSILib.h"


#define PM855L_INDEX      2


/* General helper functions */
static void       Panel_LCDB_WaitForReady(EFI_QCOM_PMIC_LCDB_PROTOCOL   *PmicLCDBProtocol);
static MDP_Status GetPmicIBBLABMode(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);

/**********************************************************************************************
*
* FUNCTION: Panel_ADP_PowerUp 
*
* DESCRIPTION:
*   Panel power up sequence for default Auto platform, such as ADP.
*
***********************************************************************************************/
MDP_Status Panel_ADP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                   Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL           *TLMMProtocol     = NULL;
  UINT32                       gpio_num;

  if (MDP_STATUS_OK != (Status =  PlatformClientInit(eDisplayId, pPowerParams)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to initialize handle for Primary display NPA node.\n"));
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

    MDP_OSAL_DELAYMS(10);   /* delay 10ms to allow power grid to settle */

    gpio_num = (eDisplayId == MDP_DISPLAY_SECONDARY) ? ADP_SA8195P_DISP_1_DISP_EN_GPIO : ADP_SA8195P_DISP_0_DISP_EN_GPIO;

    /* Display Power Bridge Chip Enable */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(gpio_num, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_8MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for Bridge Power Enable line failed %d\n",gpio_num));
    }
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(gpio_num, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Bridge Power EN line FAILED to set HIGH!\n"));
    }
  }
  return Status;
}

/**********************************************************************************************
*
* FUNCTION:  Panel_ADP_PowerDown
*
* DESCRIPTION:
*   Panel power down sequence for default Auto platform, such as ADP.
*
***********************************************************************************************/
MDP_Status Panel_ADP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                   Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL           *TLMMProtocol     = NULL;
  UINT32                       gpio_num;

  if (NULL == pPowerParams->sNPAClient[eDisplayId])
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: NULL Handle for Primary display NPA node.\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /* 
     * GPIOs & Vregs
     */

    gpio_num = (eDisplayId == MDP_DISPLAY_SECONDARY) ? ADP_SA8195P_DISP_1_DISP_EN_GPIO : ADP_SA8195P_DISP_0_DISP_EN_GPIO; 
 
    /* Display Bridge Chip DISP_EN pin */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(gpio_num, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Failed to configure Bridge GPIO %d  for Bridge Power EN line\n",gpio_num));
    }

    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(gpio_num, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Failed to set Bridge POWEN EN gpio to disable state (LOW)!\n"));
    }

    /* Display Bridge Chip RESET_EN pin */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(pPowerParams->uResetGpio, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Failed to disable GPIO %d for Reset_N line\n", pPowerParams->uResetGpio));
    }

    /*
     * Voltage Regulators
     */
    npa_complete_request(pPowerParams->sNPAClient[eDisplayId]);           // Complete the request to power rails
  }
  return Status;
}

/* PWM freq related info
*/
#define PWM_PREDIVIDER_INDEX_MAX              3         // max value of predivider index
#define PWM_EXPONENT_MAX                      7         // max value of the PWM exponent
#define PWM_MASTER_CLOCK_FREQ_19MHZ           19200000  // master clock frequency

/**********************************************************************************************
*
* FUNCTION: Panel_Default_PowerUp()
*
* DESCRIPTION:
*   Panel power up sequence for default platform, such as CDP/MTP/QRD.
*
***********************************************************************************************/
MDP_Status Panel_Default_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                   Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL           *TLMMProtocol     = NULL;
  EFI_QCOM_PMIC_GPIO_PROTOCOL *PmicGpioProtocol = NULL;

  if (MDP_STATUS_OK != (Status =  PlatformClientInit(eDisplayId, pPowerParams)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to initialize handle for Primary display NPA node.\n"));
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
    npa_issue_required_request(pPowerParams->sNPAClient[eDisplayId], PMIC_NPA_MODE_ID_GENERIC_ACTIVE);

    MDP_OSAL_DELAYMS(10);   /* delay 10ms to allow power grid to settle */


    /* Set display mode select (dual DSI or single DSI /w DSC) */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(Default_DISP_MODE_SEL, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for DISP_MODE_SEL failed!\n", Default_DISP_MODE_SEL));
    }

    /* Default to low, dual DSI */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(Default_DISP_MODE_SEL, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), GPIO_LOW_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: DISP_MODE_SEL line Low failed!\n"));
    }

    /* Display TE pin */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(Default_DISP_TE_GPIO, 1, GPIO_INPUT, GPIO_NO_PULL, GPIO_2MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for TE line failed %d\n", Default_DISP_TE_GPIO));
    }

  }

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_Default_PowerDown()
*
* DESCRIPTION:
*   Panel power down sequence for default platform, such as CDP/MTP/QRD.
*
***********************************************************************************************/
MDP_Status Panel_Default_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                   Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL           *TLMMProtocol     = NULL;
  EFI_QCOM_PMIC_GPIO_PROTOCOL *PmicGpioProtocol = NULL;

  if (NULL == pPowerParams->sNPAClient[eDisplayId])
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
    /* 
     * GPIOs & VRegs
     */
    /* Display RESET_N pin */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(pPowerParams->uResetGpio, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Failed to disable GPIO %d for Reset_N line\n", pPowerParams->uResetGpio));
    }

    /* Display TE pin */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(Default_DISP_TE_GPIO, 1, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Failed to disable GPIO %d for TE pin\n", Default_DISP_TE_GPIO));
    }


    /*
    * Voltage Regulators
    */
    npa_complete_request(pPowerParams->sNPAClient[eDisplayId]);           // Complete the request to power rails
  }

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_ResetStateExec()
*
* DESCRIPTION:
*   Toggle reset gpio base on states transition.
*
***********************************************************************************************/
static void Panel_ResetStateExec(EFI_TLMM_PROTOCOL *tlmm, MDPPlatformPanelResetInfo *pResetInfo, uint32 state0, uint32 state1, uint32 state2)
{
    if (EFI_SUCCESS != tlmm->GpioOut((UINT32)EFI_GPIO_CFG(pResetInfo->uResetGpio, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_8MA), state0))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Reset_N line FAILED at state-0!\n"));
    }

    MDP_OSAL_DELAYUS(pResetInfo->uPreResetTimeUs);

    if (EFI_SUCCESS != tlmm->GpioOut((UINT32)EFI_GPIO_CFG(pResetInfo->uResetGpio, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_8MA), state1))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Reset_N line FAILED at state-1!\n"));
    }

    MDP_OSAL_DELAYUS(pResetInfo->uResetAssertTimeUs);

    if (EFI_SUCCESS != tlmm->GpioOut((UINT32)EFI_GPIO_CFG(pResetInfo->uResetGpio, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), state2))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Reset_N line FAILED at state-2!\n"));
    }
}


/**********************************************************************************************
*
* FUNCTION: Panel_Default_Reset()
*
* DESCRIPTION:
*   Panel reset sequence for default platform, such as CDP/MTP/QRD.
*
***********************************************************************************************/
MDP_Status Panel_Default_Reset(MDPPlatformPanelResetInfo *pResetInfo)
{
  MDP_Status                    Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL            *TLMMProtocol     = NULL;

  if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /* Enable Display Reset pin*/
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(pResetInfo->uResetGpio, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_8MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for Reset_N line failed!\n", pResetInfo->uResetGpio));
    }

    switch (pResetInfo->uAssertState)
    {
      case MDP_RESET_STATE_EDGE_LOW2HIGH:
         Panel_ResetStateExec(TLMMProtocol, pResetInfo, GPIO_LOW_VALUE, GPIO_HIGH_VALUE, GPIO_HIGH_VALUE);
       break;
      case MDP_RESET_STATE_EDGE_HIGH2LOW:
         Panel_ResetStateExec(TLMMProtocol, pResetInfo, GPIO_HIGH_VALUE, GPIO_LOW_VALUE, GPIO_LOW_VALUE);
       break;
      case MDP_RESET_STATE_LEVEL_HIGH:
         Panel_ResetStateExec(TLMMProtocol, pResetInfo, GPIO_LOW_VALUE, GPIO_HIGH_VALUE, GPIO_LOW_VALUE);
       break;
      case MDP_RESET_STATE_LEVEL_LOW:
      default:
         Panel_ResetStateExec(TLMMProtocol, pResetInfo, GPIO_HIGH_VALUE, GPIO_LOW_VALUE, GPIO_HIGH_VALUE);
       break;
    }

    MDP_OSAL_DELAYUS(pResetInfo->uPostResetTimeUs);

    /*
     * post reset time: Timing space between end of reset to start sending init dcs cmds
     * NOTE:
     *   use DSIInitMasterTime to extend post reset time control if required
     */
  }

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: GetPmicIBBLABMode()
*
* DESCRIPTION:
*   This function can get the PMIC AMOLED/LCD mode configuration.
*
***********************************************************************************************/
static MDP_Status GetPmicIBBLABMode(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  EFI_QCOM_PMIC_VERSION_PROTOCOL *PmicVersionProtocol = NULL;
  EFI_PM_DEVICE_INFO_TYPE         PmicDeviceInfo;
  MDP_Status                      eStatus             = MDP_STATUS_OK;


  if (NULL == pPowerParams)
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode pPowerParams is NULL!\n"));
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicVersionProtocolGuid, NULL, (VOID **)&PmicVersionProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode Locate PMIC Version Protocol failed!\n"));
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
     if (EFI_SUCCESS != PmicVersionProtocol->GetPmicInfo(PM855L_INDEX, &PmicDeviceInfo))
     {
        DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicInfo failed!\n"));
        eStatus = MDP_STATUS_NO_RESOURCES;
     }
     else
     {
        if(PmicDeviceInfo.PmicModel == EFI_PMIC_IS_PM855L)
        {
           pPowerParams->ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD; 
        }
        else if (PmicDeviceInfo.PmicModel == EFI_PMIC_IS_PM855A) 
        {
           pPowerParams->ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED; 
        }
        else
        {
           pPowerParams->ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_NONE; 
           eStatus = MDP_STATUS_NO_RESOURCES;
        }
     }
   }

  return eStatus;
}


/**********************************************************************************************
*
* FUNCTION: Panel_Default_Brightness_Enable()
*
* DESCRIPTION:
*   Configure wled registers and enable it.
*
***********************************************************************************************/
MDP_Status Panel_Default_Brightness_Enable(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                   Status           = MDP_STATUS_OK;
  EFI_QCOM_PMIC_WLED_PROTOCOL *PmicWledProtocol = NULL;

  if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicWledProtocolGuid, NULL, (VOID **)&PmicWledProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate WLED Protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /************************************************************************
     *  Enable secondary power source
     ************************************************************************/
    if (MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED == pPowerParams->ePMICSecondaryPower[eDisplayId])
    {
      /* No need to enable brightness manually for OLED panel */
    }
    else /* MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD */
    {


      if (EFI_SUCCESS != PmicWledProtocol->EnableModulator(PM855L_INDEX, EFI_PM_MODULATOR_TYPE_A,TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableModulatorA failed\n"));
      }

      if (EFI_SUCCESS != PmicWledProtocol->SetLedDutyCycle(PM855L_INDEX, EFI_PM_MODULATOR_TYPE_A,0x1ff))  /* duty cycle is 12 bits */
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SetLedDutyCycle failed\n"));
      }

      if (EFI_SUCCESS != PmicWledProtocol->ModulatorSrcSel(PM855L_INDEX, EFI_PM_MODULATOR_TYPE_A, EFI_PM_MODULATOR_SRC_DRV_HIGH))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol ModulatorSrcSelect failed\n"));
      }

      if (EFI_SUCCESS != PmicWledProtocol->EnableWled(PM855L_INDEX, 0, TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableWled failed\n"));
      }

      if (EFI_SUCCESS != PmicWledProtocol->EnableCurrentSink(PM855L_INDEX, EFI_PM_WLED_LED1|EFI_PM_WLED_LED2|EFI_PM_WLED_LED3))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableCurrentSink failed\n"));
      }

    }
  }
  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_Default_Peripheral_Power()
*
* DESCRIPTION:
*   Secondary power sequence for other PMIC modules such as IBB/LAB.
*
***********************************************************************************************/
MDP_Status Panel_Default_Peripheral_Power(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp)
{
  MDP_Status Status = MDP_STATUS_OK;

  if (MDP_STATUS_OK != (Status = GetPmicIBBLABMode(eDisplayId, pPowerParams)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Getting PMIC mode failed with error(%d), skipping peripheral power configuration!\n", Status));
  }
  else
  {
    EFI_QCOM_PMIC_WLED_PROTOCOL  *PmicWledProtocol = NULL;
    EFI_QCOM_PMIC_IBB_PROTOCOL   *PmicIBBProtocol  = NULL;
    EFI_QCOM_PMIC_AB_PROTOCOL    *PmicABProtocol   = NULL;
    EFI_QCOM_PMIC_LCDB_PROTOCOL  *PmicLCDBProtocol = NULL;

    if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicWledProtocolGuid, NULL, (VOID **)&PmicWledProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Locate WLED Protocol failed!\n"));
      Status = MDP_STATUS_NO_RESOURCES;
    }
    else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicIbbProtocolGuid, NULL, (VOID **)&PmicIBBProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Locate PMIC IBB Protocol failed!\n"));
      Status = MDP_STATUS_NO_RESOURCES;
    }
    else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicAbProtocolGuid, NULL, (VOID **)&PmicABProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Locate PMIC AB Protocol failed!\n"));
      Status = MDP_STATUS_NO_RESOURCES;
    }
    else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicLcdbProtocolGuid, NULL, (VOID **)&PmicLCDBProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Locate PMIC LCDB Protocol failed!\n"));
      Status = MDP_STATUS_NO_RESOURCES;
    }
    else if (TRUE == bPowerUp)
    {
      // ********** Brightness Power-up Sequence **********

      // Configure the power grid based on the module type
      switch (pPowerParams->ePMICSecondaryPower[eDisplayId])
      {
      case MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD:
        {
          // Power up
          /************************************************************************/
          /* Turn On IBB(+5.5v) first, wait for 8ms, turn on LAB(-5.5v)           */
          /************************************************************************/
          // Enable LCD mode
          
          // Turn on LCDB
          
          if (EFI_SUCCESS != PmicLCDBProtocol->SetOutput(PM855L_INDEX,5500,-5500,TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to enable LCDB\n"));
          }

          if (EFI_SUCCESS != PmicLCDBProtocol->ConfigEnableCtrl(PM855L_INDEX, EFI_PM_LCDB_ENABLE_CTL_MODULE_EN, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to enable LCDB\n"));
          }


          // Wait for IBB/LAB to become ready
          Panel_LCDB_WaitForReady(PmicLCDBProtocol);
        }
        break;
        case MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED:
        {
          /* No need to setup LAB/IBB manually for OLED panel as that is controlled by swire */
        }
        break;
      default:
        break;
      }
    }
    else
    {
      // ********** Brightness Power-down Sequence **********

      // Configure the power grid based on the module type
      switch (pPowerParams->ePMICSecondaryPower[eDisplayId])
      {
      case MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD:
        // Power down
        if (EFI_SUCCESS != PmicLCDBProtocol->ConfigEnableCtrl(PM855L_INDEX, EFI_PM_LCDB_ENABLE_CTL_MODULE_EN, FALSE))
        {
          DEBUG((EFI_D_ERROR, "DisplayDxe: Error to disable LCDB\n"));
        }

        if (EFI_SUCCESS != PmicWledProtocol->EnableCurrentSink(PM855L_INDEX, EFI_PM_WLED_NONE))
        {
            DEBUG((EFI_D_ERROR, "Wled Protocol EnableCurrentSink failed\n"));
        }

        if (EFI_SUCCESS != PmicWledProtocol->EnableWled(PMIC_PMI_DEV_INDEX, 0, FALSE))
        {
          DEBUG((EFI_D_ERROR, "Wled Protocol EnableWled failed\n"));
        }

        break;
      case MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED:
      default:
        break;
      }
    }
  }
  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_Default_Brightness_Level()
*
* DESCRIPTION:
*   The function can control the panel brightness level.
*
***********************************************************************************************/
MDP_Status Panel_Default_Brightness_Level(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig)
{
  MDP_Status Status = MDP_STATUS_OK;

  if (MDP_PANEL_BACKLIGHTTYPE_PMIC == pBacklightConfig->eBacklightType)
  {
    // Configure the power grid based on the module type
    switch (pBacklightConfig->uBacklightCntrl.eBacklightCtrl)
    {
    case MDP_PMIC_BACKLIGHT_CONTROLTYPE_WLED:
      {
        EFI_QCOM_PMIC_WLED_PROTOCOL  *PmicWledProtocol = NULL;

        if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicWledProtocolGuid, NULL, (VOID **)&PmicWledProtocol))
        {
          DEBUG((EFI_D_ERROR, "DisplayDxe: Locate WLED Protocol failed!\n"));
          Status = MDP_STATUS_NO_RESOURCES;
        }
        else
        {
          uint32 uWLEDValue = (0xFFF * pBacklightConfig->uLevel) / 100; // Calculate duty cycle based on 12 bit mode

          if (EFI_SUCCESS != PmicWledProtocol->SetLedDutyCycle(PMIC_PMI_DEV_INDEX, EFI_PM_MODULATOR_TYPE_A, uWLEDValue))
          {
            DEBUG((EFI_D_ERROR, "Wled Protocol SetLedDutyCycle for ModulatorA failed\n"));
          }

          if (EFI_SUCCESS != PmicWledProtocol->SetLedDutyCycle(PMIC_PMI_DEV_INDEX, EFI_PM_MODULATOR_TYPE_B, uWLEDValue))
          {
            DEBUG((EFI_D_ERROR, "Wled Protocol SetLedDutyCycle for ModulatorB failed\n"));
          }
        }
      }
      break;
    default:
      break;
    }
  }
  else if (MDP_PANEL_BACKLIGHTTYPE_DSI == pBacklightConfig->eBacklightType)
  {
    PlatformPanelBacklightControl panelBacklightControl = {
      {0x51, 0x80}, // Using standard MIPI DCS commands to control brightness
    };

    // Calculate brightness based on 8 bit mode
    panelBacklightControl.command[1] = DISP_DIV_ROUND_UP(0xFF * pBacklightConfig->uLevel, 100);

    Status = DSIDriver_Write(eDisplayId,
                             0x15,
                             panelBacklightControl.command,
                             sizeof(panelBacklightControl.command));
    if (MDP_STATUS_OK != Status)
    {
      DEBUG((EFI_D_ERROR, "Backlight brightness DCS command send failed\n"));
    }
  }
  else
  {
    // Nothing to do for other configurations
  }
  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_LCDB_WaitForReady()
*
* DESCRIPTION:
*   Polling loop to wait for IBB/LAB modules to be ready.
*   VDISN, VDISP will reach the proper voltage upon ready.
*
***********************************************************************************************/
static void Panel_LCDB_WaitForReady(EFI_QCOM_PMIC_LCDB_PROTOCOL   *PmicLCDBProtocol)
{

  BOOLEAN lcdbStatus;
  uint32                 uCount;

  for (uCount = 0; uCount < PMIC_IBBLAB_READYTIME; uCount++)
  {
      if (EFI_SUCCESS == PmicLCDBProtocol->VregOk(PM855L_INDEX, &lcdbStatus))
      {
          if (TRUE == lcdbStatus)
             break;
      }
      else
      {
        DEBUG((EFI_D_ERROR, "DisplayDxe: LCDB VregOK() failed\n"));
        break;
      }
      // Wait 1ms between checks
      MDP_OSAL_DELAYMS(1);
  }

}
/**********************************************************************************************
*
* FUNCTION: DPAuxSwitchEnable()
*
* DESCRIPTION:
*     Enable DP USB0 Aux channel switch
*
***********************************************************************************************/

void DPAuxSwitchEnable(void)
{
   MDP_Status             eStatus        =  MDP_STATUS_OK;
   uint8                  i2c_buf[8];
   MDP_I2C_Configuration  sI2CConfig;
   MDP_I2C_Configuration *pI2CConfig;

   MDP_OSAL_MEMZERO(&sI2CConfig, sizeof(MDP_I2C_Configuration));

   pI2CConfig                          = &sI2CConfig;
   pI2CConfig->aI2CSlaveAddressList[0] = 0x43;   /* 7 bits */
   pI2CConfig->uI2CNumOfSlaves         = 1;
   pI2CConfig->uI2CBusFrequency        = 0;      /* use default */
   pI2CConfig->uI2CGSBICore            = 5;      /* for sda = gpio 51 and scl = gpio 52 */
   pI2CConfig->uI2COffsetLengthInByte  = 1;      /* 1 byte addr */

   i2c_buf[0]                          = 0x43;    /* slave addr */
   i2c_buf[1]                          = 0x04;    /* reg addr */
   i2c_buf[2]                          = 0xe0;    /* device enable, switch enable for SBU1_H <-> SBUx, SBU2_H <-> SBUx */ 
   i2c_buf[3]                          = 0x00;    /* SBU1_H <-> SBU1, SBU2_H <-> SBU2 */ 

   if(EFI_SUCCESS != (eStatus = I2CDriver_Init(pI2CConfig)))
   {         
      DEBUG((EFI_D_WARN, "DisplayDxe: I2C init failed!\n"));
   }
   else
   {
      pI2CConfig->pI2CInitSequence     = i2c_buf;
      pI2CConfig->uI2CBufLengthInByte  = 4;
      if (EFI_SUCCESS != I2CPanelSendCommandSequence(pI2CConfig, MDP_PANEL_COMMAND_CUSTOM, NULL, 0))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Backlight I2C send command failed!\n"));
      }
   }
}


/**********************************************************************************************
*
* FUNCTION: DP_USBPlugInvertedOrientation()
*
* DESCRIPTION:
*   Get DP plug polarity (TRUE = inverted, FALSE = normal).
*
***********************************************************************************************/
bool32 DP_USBPlugInvertedOrientation (void)
{ 
  /* USB driver support for detecting orientation is not available. 
   * Use Hardcoded cable polarity 
   */
  bool32  bPolarity = DP_PLUG_INVERTED_POLARITY;

  // For Klocwork - will never be true when this fn. is called
  if (PCD_EXTERNAL_DISPLAY_DP != PcdGet32(PcdExtDisplayType))
  {
     bPolarity = !bPolarity;
  }

  return bPolarity;
}


/**********************************************************************************************
*
* FUNCTION: ExternalPanel_Default_PowerUp()
*
* DESCRIPTION:
*   The default power up function for external display.
*
***********************************************************************************************/
MDP_Status ExternalPanel_Default_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                    Status            = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL            *TLMMProtocol      = NULL;


  if (MDP_STATUS_OK != (Status =  PlatformClientInit(eDisplayId, pPowerParams)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Failed to initialize handle for External (HDMI) display NPA node.\n"));
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
    npa_issue_required_request(pPowerParams->sNPAClient[eDisplayId], PMIC_NPA_MODE_ID_GENERIC_ACTIVE );

    switch (PcdGet32(PcdExtDisplayType))
    {
      case PCD_EXTERNAL_DISPLAY_DP:
        npa_issue_required_request(pPowerParams->sNPAClient[eDisplayId], PMIC_NPA_MODE_ID_GENERIC_ACTIVE );

        if (eDisplayId == MDP_DISPLAY_EXTERNAL3) /* DP over Type-C port #1 */
        {
           /* configure AUX switch chip through two GPIOs */
           /* Configure USB_DP_OE_N (EN_N) GPIO */
           if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), DAL_TLMM_GPIO_ENABLE))
           {
             DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for USB_DP1_SW_EN failed!\n", GPIO_USB_DP1_SW_EN));
             Status = MDP_STATUS_FAILED;
           }
           if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_LOW_VALUE))
           {
             DEBUG((EFI_D_WARN, "DisplayDxe: GPIO_USB_DP1_SW_EN line Low failed!\n"));
             Status = MDP_STATUS_FAILED;
           }
           
           /* Configure gpio for chip select OE_N of USB-DP Mux logic (SBU_SW_OE) */      
           if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_SEL, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), DAL_TLMM_GPIO_ENABLE))
           {
             DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for USB_DP1_SW_SEL failed!\n", GPIO_USB_DP1_SW_SEL));
             Status = MDP_STATUS_FAILED;
           }
           if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(GPIO_USB_DP1_SW_SEL, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_LOW_VALUE))
           {
             DEBUG((EFI_D_WARN, "DisplayDxe: GPIO_USB_DP1_SW_SEL line Low failed!\n"));
             Status = MDP_STATUS_FAILED;
           }
         }
         else if (eDisplayId == MDP_DISPLAY_EXTERNAL2)
         {
           /* Configure gpio for chip select OE_N of USB-DP Mux logic (SBU_SW_OE) */      
           if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(GPIO_USB_DP0_FSA_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), DAL_TLMM_GPIO_ENABLE))
           {
             DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for USB_DPo_FSA_EN failed!\n", GPIO_USB_DP0_FSA_EN));
             Status = MDP_STATUS_FAILED;
           }
           if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(GPIO_USB_DP0_FSA_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_LOW_VALUE))
           {
             DEBUG((EFI_D_WARN, "DisplayDxe: GPIO_USB_DP0_FSA_EN line Low failed!\n"));
             Status = MDP_STATUS_FAILED;
           }

           /* configure AUX switch chip through I2c protocol */
           DPAuxSwitchEnable();
         }
        break;
  
      default:
        break;
    }

  }

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: ExternalPanel_Default_PowerDown()
*
* DESCRIPTION:
*   The default power down function for external display.
*
***********************************************************************************************/
MDP_Status ExternalPanel_Default_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                    Status            = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL             *TLMMProtocol     = NULL;

  if (NULL == pPowerParams->sNPAClient[eDisplayId])
  {
    DEBUG ((EFI_D_ERROR, "DisplayDxe: NULL Handle for HDMI NPA node.\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    switch (PcdGet32(PcdExtDisplayType))
    {
      case PCD_EXTERNAL_DISPLAY_DP:
        if (eDisplayId == MDP_DISPLAY_EXTERNAL3) /* DP over Type-C port #1 */
        {
            if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_DP1_SW_SEL, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
            {
               DEBUG((EFI_D_WARN, "DisplayDxe: Failed to disable GPIO %d for DP1_SW_SEL pin, DisplayId=%d\n", GPIO_USB_DP1_SW_SEL, eDisplayId));
            }
            if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(GPIO_USB_DP1_SW_EN, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
            {
               DEBUG((EFI_D_WARN, "DisplayDxe: Failed to disable GPIO %d for DP1_SW_EN pin, DisplayId=%d\n", GPIO_USB_DP1_SW_EN, eDisplayId));
            }
        }
        break;
 
      default:
        break;
    }
  }

  return Status;
}

/****************************************************************************
*
* FUNCTION: MDP_CalculatePWMValue()
*
* DESCRIPTION:
*   This function calculates the predivider , exponent for desired PWM frequency
*
* \param
*   uPwmDesiredFreq   - Desired PWM frequency
*   pPWMConfiguration - PWM freq condifuration like actual freq, requested freq, predivider, exponent
*   uPwmBitWidth      - PWM bit width
*
* \retval MDP_STATUS_SUCCESS
*
****************************************************************************/
MDP_Status MDP_CalculatePWMValue(uint32 uPwmDesiredFreq, MDP_PMICPWMConfigurationType *pPWMConfiguration, uint32 uPwmBitWidth)
{
  MDP_Status  eStatus           = MDP_STATUS_OK;
  uint32      uPreDivider[4]    = {1,3,5,6};          // can take values 1,3,5,6
  uint32      uMinFreqDiff      = (uint32)-1;         // uint32 max
  uint32      uCalcFreq         = 0;
  uint32      uFreqDiff         = 0;
  uint32      uBitWidth         = uPwmBitWidth;
  uint32      uDesiredFreq      = (uPwmDesiredFreq > 0)?uPwmDesiredFreq:MDP_PMIC_PWM_DESIRED_FREQ_DEFAULT_HZ;    // Default PWM desired frequency if it is ZERO

  // Default PWM bit width to 9 if bit Width is other than 6,9
  switch (uBitWidth)
  {
    case 6:
      pPWMConfiguration->uPwmBitWidth = EFI_PM_PWM_SIZE_6BIT;
      break;
    case 9:
      pPWMConfiguration->uPwmBitWidth = EFI_PM_PWM_SIZE_9BIT;
      break;
    default:
      uBitWidth = 9;
      pPWMConfiguration->uPwmBitWidth = EFI_PM_PWM_SIZE_9BIT;
      break;
  }

  for (uint32 uIndex = 0; uIndex <= PWM_PREDIVIDER_INDEX_MAX; uIndex++)
  {
    for (uint32 uExponent = 0; uExponent <= PWM_EXPONENT_MAX; uExponent++)
    {
      uCalcFreq = (PWM_MASTER_CLOCK_FREQ_19MHZ / ((1 << uBitWidth) * uPreDivider[uIndex] * (1 << uExponent)));

      uFreqDiff = DISP_ABS_DELTA(uCalcFreq, uDesiredFreq);

      if (uFreqDiff < uMinFreqDiff)
      {
        pPWMConfiguration->uPreDivider         = uPreDivider[uIndex];
        pPWMConfiguration->uExponent           = uExponent;
        pPWMConfiguration->uActualFrequency    = uCalcFreq;
        pPWMConfiguration->uRequestedFrequency = uDesiredFreq;
        uMinFreqDiff                           = uFreqDiff;
      }
    }
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif
