/*=============================================================================

  File: MDPPlatformLibPanelCommon.c

  MDP default platform panel functions

  Copyright (c) 2018 Qualcomm Technologies, Inc.
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
#include <Protocol/EFIPmicIbb.h>
#include <Protocol/EFIPmicLab.h>
#include "DDIChipInfo.h"
#include "MDPSystem.h"
#include "MDPPeripherals.h"
#include "MDPPlatformLibPanelCommon.h"
#include "DisplayUtils.h"
#include "pmapp_npa.h"
#include "DDITlmm.h"
#include "HALDSILib.h"

/* General helper functions */
static void       Panel_IBBLAB_WaitForReady(EFI_QCOM_PMIC_IBB_PROTOCOL   *PmicIBBProtocol, EFI_QCOM_PMIC_LAB_PROTOCOL   *PmicLABProtocol);
static MDP_Status GetPmicIBBLABMode(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);


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

  if (MDP_STATUS_OK != (Status = PlatformClientInit(eDisplayId, pPowerParams)))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: failed to initialize handle for Primary display NPA node.\n"));
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

    MDP_OSAL_DELAYMS(10); // Delay 10ms to allow power grid to settle

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
      DEBUG((EFI_D_WARN, "DisplayDxe: failed to disable GPIO %d for Reset_N line\n", pPowerParams->uResetGpio));
    }

    /* Display TE pin */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio(EFI_GPIO_CFG(Default_DISP_TE_GPIO, 1, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: failed to disable GPIO %d for TE pin\n", Default_DISP_TE_GPIO));
    }

    /*
    * Voltage Regulators
    */
    npa_complete_request(pPowerParams->sNPAClient[eDisplayId]);           //Complete the request to power rails
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
  EFI_PM_IBB_STATUS_TYPE     sPmicIBBStatus;
  EFI_PM_LAB_STATUS_TYPE     sPmicLABStatus;
  EFI_QCOM_PMIC_IBB_PROTOCOL *PmicIBBProtocol = NULL;
  EFI_QCOM_PMIC_LAB_PROTOCOL *PmicLABProtocol = NULL;
  MDP_Status                 eStatus          = MDP_STATUS_OK;

  if (NULL == pPowerParams)
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode pPowerParams is NULL!\n"));
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicIbbProtocolGuid, NULL, (VOID **)&PmicIBBProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode Locate PMIC IBB Protocol failed!\n"));
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicLabProtocolGuid, NULL, (VOID **)&PmicLABProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode Locate PMIC LAB Protocol failed!\n"));
    eStatus = MDP_STATUS_NO_RESOURCES;
  }
  else if (EFI_SUCCESS != PmicIBBProtocol->GetStatus(PMIC_PMI_DEV_INDEX, &sPmicIBBStatus))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode Get IBB Status failed!\n"));
    eStatus = MDP_STATUS_FAILED;
  }
  else if (EFI_SUCCESS != PmicLABProtocol->GetStatus(PMIC_PMI_DEV_INDEX, &sPmicLABStatus))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode Get LAB Status failed!\n"));
    eStatus = MDP_STATUS_FAILED;
  }
  else
  {
    if ((TRUE == sPmicIBBStatus.AmoledModeEn) &&
        (TRUE == sPmicLABStatus.AmoledModeEn))
    {
      // Get AMOLED mode by reading IBB/LAB status, configure PMIC as AMOLED mode.
      pPowerParams->ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED;
      pPowerParams->eOLEDPowerMode                  = MDPPLATFORM_LABIBB_MODE_SWIRE;
    }
    else if ((FALSE == sPmicIBBStatus.AmoledModeEn) &&
             (FALSE == sPmicLABStatus.AmoledModeEn))
    {
      // Get LCD mode by reading IBB/LAB status, configure PMIC as LCD mode.
      pPowerParams->ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD;
    }
    else
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: GetPmicIBBLABMode IBB/LAB get different modes, skip power configuration!\n"));
      eStatus = MDP_STATUS_FAILED;
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
  EFI_QCOM_PMIC_WLED_PROTOCOL  *PmicWledProtocol = NULL;
  MDP_Status Status = MDP_STATUS_OK;

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
    if (pPowerParams->ePMICSecondaryPower[eDisplayId] == MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED)
    {
      // 0xD844             EN_AMOLED                 0x80       Enable AMOLED mode
      if (EFI_SUCCESS != PmicWledProtocol->EnAmoled(PMIC_PMI_DEV_INDEX, TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnAmoled failed\n"));
      }

      // 0xD84D             WLED_OVP                  0x11       For this panel, 7.84V OVP is sufficient
      if (EFI_SUCCESS != PmicWledProtocol->SetOvpThreshold(PMIC_PMI_DEV_INDEX, 0, EFI_PM_WLED_OVP_THR_7P84V))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SetOvpThreshold failed\n"));
      }

      // 0xD84E             WLED_ILIM                 0x02       525 mA current limit setting for fsw = 1.6 MHz
      if (EFI_SUCCESS != PmicWledProtocol->SetBoostIlimit(PMIC_PMI_DEV_INDEX, 0, EFI_PM_WLED_BST_ILIMIT_525mA))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SetBoostIlimit failed\n"));
      }

      // 0xD84F             EN_CURRENT_SINK           0xE0       Enable string 2
      if (EFI_SUCCESS != PmicWledProtocol->EnableCurrentSink(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_LED2))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableCurrentSink EFI_PM_WLED_LED2 failed\n"));
      }

      // 0xD84F             EN_CURRENT_SINK           0xE0       Enable string 3
      if (EFI_SUCCESS != PmicWledProtocol->EnableCurrentSink(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_LED3))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableCurrentSink EFI_PM_WLED_LED3 failed\n"));
      }

      // 0xD854             SLEW_RATE_CTRL            0x03       Set slew rate to 5nS
      if (EFI_SUCCESS != PmicWledProtocol->SelectSlewRate(PMIC_PMI_DEV_INDEX, 0, EFI_PM_WLED_SLEW_RATE_5nS))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SelectSlewRate failed\n"));
      }

      // 0xD84C             SWITCHING_FREQUENCY       0x11       Set boost switching frequency to 800KHz.
      if (EFI_SUCCESS != PmicWledProtocol->SetFswCtrl(PMIC_PMI_DEV_INDEX, 0, 800))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SetFswCtrl failed\n"));
      }

      if (EFI_SUCCESS != PmicWledProtocol->EnableModulator(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_ALL,TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableModulator failed\n"));
      }

      if (EFI_SUCCESS != PmicWledProtocol->EnableSync(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_ALL, TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableSync failed\n"));
      }

      if (MDPPLATFORM_LABIBB_MODE_SWIRE == pPowerParams->eOLEDPowerMode)
      {
        // Configure WLED to be controlled by SWIRE
        if (EFI_SUCCESS != PmicWledProtocol->SetAmoledVout(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_AMOLED_CTRL_TYPE_SWIRE, 7600))
        {
          DEBUG((EFI_D_ERROR, "Wled Protocol SetAmoledVout failed\n"));
        }
      }
      else if (MDPPLATFORM_LABIBB_MODE_SPMI == pPowerParams->eOLEDPowerMode)
      {
        // Configure WLED to be controlled by SPMI
        if (EFI_SUCCESS != PmicWledProtocol->SetAmoledVout(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_AMOLED_CTRL_TYPE_SPMI, 7600))
        {
          DEBUG((EFI_D_ERROR, "Wled Protocol SetAmoledVout failed\n"));
        }
      }
    }
    else /* MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD */
    {
      //0xD84D             WLED_OVP          0x11       For this panel, 29.6V OVP is sufficient
      if (EFI_SUCCESS != PmicWledProtocol->SetOvpThreshold(PMIC_PMI_DEV_INDEX, 0, EFI_PM_WLED_OVP_THR_29P6V))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SetOvpThreshold failed\n"));
      }

      //0xD84E             WLED_ILIM         0x02       525 mA current limit setting for fsw = 1.6 MHz
      if (EFI_SUCCESS != PmicWledProtocol->SetBoostIlimit(PMIC_PMI_DEV_INDEX, 0, EFI_PM_WLED_BST_ILIMIT_525mA))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SetBoostIlimit failed\n"));
      }

      //0xD84F             EN_CURRENT_SINK         0xE0       Enable string 2
      if (EFI_SUCCESS != PmicWledProtocol->EnableCurrentSink(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_LED2))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableCurrentSink EFI_PM_WLED_LED2 failed\n"));
      }

      //0xD84F             EN_CURRENT_SINK         0xE0       Enable string 3
      if (EFI_SUCCESS != PmicWledProtocol->EnableCurrentSink(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_LED3))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableCurrentSink EFI_PM_WLED_LED3 failed\n"));
      }

	  //0xD854             SLEW_RATE_CTRL            0x02       At default
      if (EFI_SUCCESS != PmicWledProtocol->SelectSlewRate(PMIC_PMI_DEV_INDEX, 0, EFI_PM_WLED_SLEW_RATE_10nS))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SelectSlewRate failed\n"));
      }

      //0xD84C             SWITCHING_FREQUENCY       0x11      Set boost switching frequency to 800KHz.
      if (EFI_SUCCESS != PmicWledProtocol->SetFswCtrl(PMIC_PMI_DEV_INDEX, 0, 800))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol SetFswCtrl failed\n"));
      }

      if (EFI_SUCCESS != PmicWledProtocol->EnableModulator(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_ALL,TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableModulator failed\n"));
      }    

      if (EFI_SUCCESS != PmicWledProtocol->EnableSync(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_ALL, TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableSync failed\n"));
      }

      //0xD846             MODULE_EN      0x80       Enable the entire module
      if (EFI_SUCCESS != PmicWledProtocol->EnableWled(PMIC_PMI_DEV_INDEX, 0, TRUE))
      {
        DEBUG((EFI_D_ERROR, "Wled Protocol EnableWled failed\n"));
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
    EFI_QCOM_PMIC_LAB_PROTOCOL   *PmicLABProtocol  = NULL;

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
    else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicLabProtocolGuid, NULL, (VOID **)&PmicLABProtocol))
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: Locate PMIC LAB Protocol failed!\n"));
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
          //Enable LCD mode
          if (EFI_SUCCESS != PmicIBBProtocol->LcdAmoledSel(PMIC_PMI_DEV_INDEX, FALSE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config IBB to LCD mode\n"));
          }
          // Soft start charging register = 64k Ohms
          if (EFI_SUCCESS != PmicIBBProtocol->ChgrResistor(PMIC_PMI_DEV_INDEX, 64))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to set start charging registe with ChgrResistor\n"));
          }
          // Finish VDISP Config
          if (EFI_SUCCESS != PmicIBBProtocol->ModuleRdy(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config IBB to Ready\n"));
          }
          // Turn on IBB
          if (EFI_SUCCESS != PmicIBBProtocol->ConfigIbbCtrl(PMIC_PMI_DEV_INDEX, TRUE, FALSE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to enable IBB\n"));
          }

          //Change default Lab Current Sense setting, ISense_Tap = 1x, ISense_Gain = 1.5x
          if (EFI_SUCCESS != PmicLABProtocol->ConfigCurrentSense(PMIC_PMI_DEV_INDEX, EFI_PM_LAB_ISENSE_1P0, EFI_PM_LAB_ISENSE_1P5))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB Current Sense\n"));
          }

          //Change default Lab Pulse skip setting, Enabled, 40mA threshold.
          if (EFI_SUCCESS != PmicLABProtocol->ConfigPulseSkipCtrl(PMIC_PMI_DEV_INDEX, 40, 0, 0, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB Pulse skip setting\n"));
          }

          //Enable LCD mode
          if (EFI_SUCCESS != PmicLABProtocol->LcdAmoledSel(PMIC_PMI_DEV_INDEX, FALSE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB to LCD mode\n"));
          }

          // Finish VDISN Config
          if (EFI_SUCCESS != PmicLABProtocol->ModuleRdy(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB to Ready\n"));
          }

          //Turn on LAB for VDISN
          if (EFI_SUCCESS != PmicLABProtocol->IbbRdyEn(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to enable LAB\n"));
          }

          // Wait for IBB/LAB to become ready
          Panel_IBBLAB_WaitForReady(PmicIBBProtocol, PmicLABProtocol);
        }
        break;

      case MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED:
        {
          // Power up

          /************************************************************************
           * Turn On IBB first, then turn on LAB                                  *
           ************************************************************************/
          // Enable AMOLED mode
          if (EFI_SUCCESS != PmicIBBProtocol->LcdAmoledSel(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config IBB to AMOLED mode\n"));
          }

          // Soft start charging resistor = 64k Ohms
          if (EFI_SUCCESS != PmicIBBProtocol->ChgrResistor(PMIC_PMI_DEV_INDEX, 64))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to set start charging resistor\n"));
          }

          // Finish VDISP Config
          if (EFI_SUCCESS != PmicIBBProtocol->ModuleRdy(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config IBB to Ready\n"));
          }

          if (MDPPLATFORM_LABIBB_MODE_SWIRE == pPowerParams->eOLEDPowerMode)
          {
            // Configure IBB to be controlled by SWIRE
            if (EFI_SUCCESS != PmicIBBProtocol->ConfigIbbCtrl(PMIC_PMI_DEV_INDEX, FALSE, TRUE))
            {
              DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config IBB control\n"));
            }
          }
          else if (MDPPLATFORM_LABIBB_MODE_SPMI == pPowerParams->eOLEDPowerMode)
          {
            // Configure IBB to be controlled by SPMI
            if (EFI_SUCCESS != PmicIBBProtocol->ConfigIbbCtrl(PMIC_PMI_DEV_INDEX, TRUE, FALSE))
            {
              DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config IBB control\n"));
            }
          }

          // Change default LAB Current Sense setting, ISense_Tap = 1x, ISense_Gain = 1.5x
          if (EFI_SUCCESS != PmicLABProtocol->ConfigCurrentSense(PMIC_PMI_DEV_INDEX, EFI_PM_LAB_ISENSE_1P0, EFI_PM_LAB_ISENSE_1P5))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB Current Sense\n"));
          }

          // Disable LAB Pulse skip setting
          if (EFI_SUCCESS != PmicLABProtocol->ConfigPulseSkipCtrl(PMIC_PMI_DEV_INDEX, 20, 0, 0, FALSE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB Pulse skip setting\n"));
          }

          // Enable AMOLED mode
          if (EFI_SUCCESS != PmicLABProtocol->LcdAmoledSel(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB to AMOLED mode\n"));
          }

          // Finish VDISN Config
          if (EFI_SUCCESS != PmicLABProtocol->ModuleRdy(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to config LAB to Ready\n"));
          }

          // Turn on LAB for VDISN
          if (EFI_SUCCESS != PmicLABProtocol->IbbRdyEn(PMIC_PMI_DEV_INDEX, TRUE))
          {
            DEBUG((EFI_D_ERROR, "DisplayDxe: Error to enable LAB\n"));
          }

          if (MDPPLATFORM_LABIBB_MODE_SPMI == pPowerParams->eOLEDPowerMode)
          {
            /* Wait for IBB/LAB to become ready for SPMI Control Type.
             * This is not valid for SWIRE because the ready will be triggered
             * after the panel sends the appropriate SWIRE commands.
             */
            Panel_IBBLAB_WaitForReady(PmicIBBProtocol, PmicLABProtocol);
          }
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
        if (EFI_SUCCESS != PmicIBBProtocol->ConfigIbbCtrl(PMIC_PMI_DEV_INDEX, FALSE, FALSE))
        {
          DEBUG((EFI_D_ERROR, "DisplayDxe: Error to disable IBB\n"));
        }

        if (EFI_SUCCESS != PmicWledProtocol->EnableWled(PMIC_PMI_DEV_INDEX, 0, FALSE))
        {
          DEBUG((EFI_D_ERROR, "Wled Protocol EnableWled failed\n"));
        }
        break;

      case MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_OLED:
        // Power down
        if (EFI_SUCCESS != PmicIBBProtocol->ConfigIbbCtrl(PMIC_PMI_DEV_INDEX, FALSE, FALSE))
        {
          DEBUG((EFI_D_ERROR, "DisplayDxe: Error to disable IBB\n"));
        }	
        break;

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
  MDP_Status        Status   = MDP_STATUS_OK;

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

          if (EFI_SUCCESS != PmicWledProtocol->SetLedDutyCycle(PMIC_PMI_DEV_INDEX, EFI_PM_WLED_ALL, uWLEDValue))
          {
            DEBUG((EFI_D_ERROR, "Wled Protocol SetLedDutyCycle failed\n"));
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

    Status = DSIDriver_Write(0x15,
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
* FUNCTION: Panel_IBBLAB_WaitForReady()
*
* DESCRIPTION:
*   Polling loop to wait for IBB/LAB modules to be ready.
*   VDISN, VDISP will reach the proper voltage upon ready.
*
***********************************************************************************************/
static void Panel_IBBLAB_WaitForReady(EFI_QCOM_PMIC_IBB_PROTOCOL   *PmicIBBProtocol,
                                      EFI_QCOM_PMIC_LAB_PROTOCOL   *PmicLABProtocol)
{
  EFI_PM_IBB_STATUS_TYPE sIBBStatus;
  EFI_PM_LAB_STATUS_TYPE sLABStatus;
  uint32                 uCount;

  // Poll for ready on IBB & LAB VReg Ready
  for (uCount = 0; uCount < PMIC_IBBLAB_READYTIME; uCount++)
  {
    if ((EFI_SUCCESS == PmicLABProtocol->GetStatus(PMIC_PMI_DEV_INDEX, &sLABStatus)) &&
        (EFI_SUCCESS == PmicIBBProtocol->GetStatus(PMIC_PMI_DEV_INDEX, &sIBBStatus)))
    {
      if ((TRUE == sLABStatus.LabVregOk) &&
          (TRUE == sIBBStatus.IbbVregOk))
      {
        break;
      }
      else if (PMIC_IBBLAB_READYTIME == (uCount + 1))
      {
        DEBUG((EFI_D_ERROR, "DisplayDxe: LAB/IBB GetStatus(LabVregOk) failed after %dms\n", PMIC_IBBLAB_READYTIME));
      }

      // Wait 1ms between checks
      MDP_OSAL_DELAYMS(1);
    }
    else
    {
      DEBUG((EFI_D_ERROR, "DisplayDxe: IBB/LAB GetStatus() failed\n"));
      break;
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
    DEBUG((EFI_D_ERROR, "DisplayDxe: failed to initialize handle for External (HDMI) display NPA node.\n"));
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

        /* Configure USB_DP_OE_N (EN_N) GPIO */
        if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(43, 1, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), DAL_TLMM_GPIO_ENABLE))
        {
          DEBUG((EFI_D_ERROR, "DP: failed to configure USB_DP_OE_N GPIO as output \n"));        
          Status = MDP_STATUS_FAILED;
        }
        if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(43, 1, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_LOW_VALUE))
        {
          DEBUG((EFI_D_ERROR, "DP: failed to drive USB_DP_OE_N GPIO \n"));  
          Status = MDP_STATUS_FAILED;
        }
           
        /* Configure gpio for chip select OE_N of USB-DP Mux logic (SBU_SW_OE) */      
        if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(51, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), DAL_TLMM_GPIO_ENABLE))
        {
          DEBUG((EFI_D_ERROR, "DP: failed to configure USB_EURO_SEL_DC GPIO as output \n"));      
          Status = MDP_STATUS_FAILED;
        }
        if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(51, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), GPIO_LOW_VALUE))
        {
          DEBUG((EFI_D_ERROR, "DP: failed to drive USB_EURO_SEL_DC GPIO \n"));
          Status = MDP_STATUS_FAILED;
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
        break;
 
      default:
        break;
    }
  }

  return Status;
}

#ifdef __cplusplus
}
#endif
