/*=============================================================================

  File: Panel_cls_sn65dsix6_wqhd_vid.c

  MDP platform specific functions

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
#include "MDPPlatformLibPanelCommon.h"
#include "MDPPeripherals.h"
#include "DisplayUtils.h"
#include "pmapp_npa.h"
#include "DDITlmm.h"
#include "HALDSILib.h"

/* SDM845 Main Panel CLS specific defines
*/
#define CLS_GPIO_VLCM_3V3_EN                        88    // GPIO number for eDP 3.0v:  VLCM_3V3(source:VPH_PWR, enable: CLS_GPIO_88)
#define CLS_GPIO_VBG_VCC_EN                         9     // VCC_EDP_1V2:  SW_EDP_1P2(source: VREG_L2A_1P2 , enable: PMIC CLS_GPIO_09)
#define CLS_GPIO_DIV_CLK_EN                         16    // 19.2 Mhz ref clk for TI:DIV_CLK_CLS_GPIO_16 (PMIC CLS_GPIO_16)
#define CLS_GPIO_BRIDGE_EN                          96    // Enable Bridge chip, active high: BRIDGE_EN (CLS_GPIO_96)
#define CLS_GPIO_BACKLIGHT_SOURCE_EN                11    // Enable backlight: VLCM_BL_SOURCE (source:VLCM_3V3_SOURCE , enable: BL_EN_MSM)
#define CLS_GPIO_BACKLIGHT_PWM_EN                   8     // Use GPIO 8 as Backlight PWM signal
#define CLS_GPIO_BRIDGE_GPIO1_SUSPEND               128   // TI_GPIO1

#define CLS_I2C_BACKLIGHT_COMMANDS                  2     // Number of I2C backlight commands
#define CLS_I2C_BACKLIGHT_COMMAND_PACKETS           3     // Number of bytes in one I2C backlight command


/* I2C backlight commands, to change backlight value */
static uint8 geDP_BacklightCommands[CLS_I2C_BACKLIGHT_COMMANDS][CLS_I2C_BACKLIGHT_COMMAND_PACKETS] =
{
  {0x2C, 0xA3, 0x00},  // Lower  8 bits of BACKLIGHT register in eDP bridge
  {0x2C, 0xA4, 0x00},  // Higher 8 bits of BACKLIGHT register in eDP bridge
};


/**********************************************************************************************
*
* FUNCTION: Panel_PowerUp_Cls_Sn65dsix6_Wqhd_Vid()
*
* DESCRIPTION:
*   Panel power up sequence for CLS Sn65dsix6_Wqhd_Vid panel.
*
***********************************************************************************************/
MDP_Status Panel_PowerUp_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                         Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL                 *TLMMProtocol     = NULL;
  EFI_QCOM_PMIC_GPIO_PROTOCOL       *PmicGpioProtocol = NULL;
  EFI_QCOM_PMIC_CLKBUFF_PROTOCOL    *ClkBuffProtocol  = NULL;

  if (MDP_STATUS_OK != (Status =  PlatformClientInit(eDisplayId, pPowerParams)))
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
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicClkBuffProtocolGuid, NULL, (void **)&ClkBuffProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: gQcomPmicClkBuffProtocolGuid failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /*
    * Voting for Display NPA node to be ON
    */
    npa_issue_required_request(pPowerParams->sNPAClient[eDisplayId], PMIC_NPA_MODE_ID_GENERIC_ACTIVE);

    /* Delay 10ms to allow power grid to settle */
    MDP_OSAL_DELAYMS(10);

    /*
    1. eDP 3.0v:  VLCM_3V3(source:VPH_PWR, enable: GPIO_88)
    2. turn on VCC_EDP_1V2:  SW_EDP_1P2(source: VREG_L2A_1P2 , enable: PMIC gpio_09)
    3. VCCIO_EDP_1V8/VPLL_EDP_1V8:  turn on LDO VREG_L14A_1P88 by PMIC?
    4. turn on 19.2 Mhz ref clk for TI:DIV_CLK_GPIO_16 (PMIC gpio_16) --> how to config PMIC gpio to 19.2Mhz source
    5. delay(200us) to allow HPD to be high
    6. Enable Bridge chip, active high: BRIDGE_EN (gpio_96)
    7. delay(100us) allow panel to initialize after the reset
    8. Enable backlight: VLCM_BL_SOURCE (source:VLCM_3V3_SOURCE , enable: BL_EN_MSM) GPIO_8 as PWM signal for PWM source (5.  PWM for the panel is from MSM GPIO_08 and eDP bridge chip GPIO_04 is a backup option)
    9. Start to send DCS commands
    */
    /* VLCM_3V3: Turn on GPIO88 and assume VPH_PWR already on */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_VLCM_3V3_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for VLCM_3V3_EN failed!\n", CLS_GPIO_VLCM_3V3_EN));
    }
    /* Pull high GPIO 88 to enable VLCM_3V3 */
    else if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_VLCM_3V3_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Enabling GPIO %d for VLCM_3V3_EN line high failed!\n", CLS_GPIO_VLCM_3V3_EN));
    }

    /* Enable pmic gpio */
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(0,  EFI_PM_GPIO_9,
                                                                 EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                                 EFI_PM_GPIO_VIN0,
                                                                 EFI_PM_GPIO_SOURCE_GND,
                                                                 EFI_PM_GPIO_OUT_BUFFER_LOW,
                                                                 GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed!\n"));
    }

    // Turn on 19.2 Mhz ref clk for TI:DIV_CLK_GPIO_16 (PMIC gpio_16) 1.8v
    // Turn on TI:DIV_CLK_GPIO_16
    if (EFI_SUCCESS != ClkBuffProtocol->ClkBuffEnable(0, EFI_PM_CLK_DIV_3, TRUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: ClkBuffEnable failed!\n"));
    }

    if(EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(0, EFI_PM_GPIO_16,
                                                            EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            EFI_PM_GPIO_VIN0,
                                                            EFI_PM_GPIO_SOURCE_SPECIAL_FUNCTION2,
                                                            EFI_PM_GPIO_OUT_BUFFER_HIGH,
                                                            GPIO_LOW_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull PMIC GPIO protocol failed! = %d \n", Status));
    }

    MDP_OSAL_DELAYUS(200);

    /* Enable Bridge gpio suspend, active high: BRIDGE_GPIO1_SUSPEND (gpio_128) */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_GPIO1_SUSPEND, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for BRIDGE_GPIO1_SUSPEND failed!\n", CLS_GPIO_BRIDGE_GPIO1_SUSPEND));
    }

    /* Pull high GPIO 128 to enable BRIDGE_EN */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_GPIO1_SUSPEND, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: GPIO %d line high failed!\n", CLS_GPIO_BRIDGE_GPIO1_SUSPEND));
    }

    /* Enable Bridge chip, active high: BRIDGE_EN (gpio_96) */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for BRIDGE_EN failed!\n", CLS_GPIO_BRIDGE_EN));
    }

    /* Pull high GPIO 96 to enable BRIDGE_EN */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: GPIO %d line high failed!\n", CLS_GPIO_BRIDGE_EN));
    }

    MDP_OSAL_DELAYUS(100);

  }

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_PowerDown_Cls_Sn65dsix6_Wqhd_Vid()
*
* DESCRIPTION:
*   Panel power down sequence for CLS Sn65dsix6_Wqhd_Vid panel.
*
***********************************************************************************************/
MDP_Status Panel_PowerDown_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                       Status           = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL               *TLMMProtocol     = NULL;
  EFI_QCOM_PMIC_GPIO_PROTOCOL     *PmicGpioProtocol = NULL;
  EFI_QCOM_PMIC_CLKBUFF_PROTOCOL  *ClkBuffProtocol  = NULL;

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
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gQcomPmicClkBuffProtocolGuid, NULL, (void **)&ClkBuffProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: gQcomPmicClkBuffProtocolGuid failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
	 /* Disable LED_EN (3.0V) to LCD panel, active high */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BACKLIGHT_SOURCE_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for GPIO_BACKLIGHT_SOURCE_EN failed!\n", CLS_GPIO_BACKLIGHT_SOURCE_EN));
    }

	/* VLCM_3V3 (disable pull up for BL_PWM of LCD panel): Turn off GPIO88 */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_VLCM_3V3_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for CLS_GPIO_VLCM_3V3_EN failed!\n", CLS_GPIO_VLCM_3V3_EN));
    }

    MDP_OSAL_DELAYUS(100);

    /*
    * Voltage Regulators
    */
    npa_complete_request(pPowerParams->sNPAClient[eDisplayId]);           //Complete the request to power rails

	MDP_OSAL_DELAYUS(100);

	/* Disable Bridge chip, active high */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for DISP_MODE_SEL failed!\n", CLS_GPIO_BRIDGE_EN));
    }

    /* Disable bridge gpio suspend, active high: BRIDGE_GPIO1_SUSPEND (gpio_128) */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_GPIO1_SUSPEND, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for CLS_GPIO_BRIDGE_GPIO1_SUSPEND failed!\n", CLS_GPIO_BRIDGE_GPIO1_SUSPEND));
    }

    /* Disable backlight PWM signal (GPIO 8) of MSM chip */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BACKLIGHT_PWM_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_DISABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for CLS_GPIO_BACKLIGHT_PWM_EN failed!\n", CLS_GPIO_BACKLIGHT_PWM_EN));
    }

    /* Disable PMIC gpio_16 */
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_DEV_INDEX, EFI_PM_GPIO_16, EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
        EFI_PM_GPIO_VIN0, EFI_PM_GPIO_SOURCE_GND, EFI_PM_GPIO_OUT_BUFFER_LOW, TRUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure PMIC GPIO %d for WLED disable failed\n", EFI_PM_GPIO_16));
    }

    /* Turn off 19.2 Mhz ref clk */ 
    if (EFI_SUCCESS != ClkBuffProtocol->ClkBuffEnable(0, EFI_PM_CLK_DIV_3, FALSE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: ClkBuffEnable failed to disable!\n"));
    }

    /* Disable PMIC gpio 9 */
    if (EFI_SUCCESS != PmicGpioProtocol->ConfigDigitalOutput(PMIC_DEV_INDEX, EFI_PM_GPIO_9, EFI_PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
        EFI_PM_GPIO_VIN0, EFI_PM_GPIO_SOURCE_GND, EFI_PM_GPIO_OUT_BUFFER_LOW, TRUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure PMIC GPIO %d for WLED disable failed\n", EFI_PM_GPIO_16));
    }
  }

  // Deinit I2C interface for backlight
  I2CDriver_Deinit(&pPowerParams->sI2CConfig);

  return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_Brightness_Enable_Cls_Sn65dsix6_Wqhd_Vid()
*
* DESCRIPTION:
*   Initialize GPIO and I2C driver to enable brightness for CLS Sn65dsix6_Wqhd_Vid panel.
*
***********************************************************************************************/
MDP_Status Panel_Brightness_Enable_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status         Status       = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL *TLMMProtocol = NULL;

  // Initialize I2C context for backlight
  if(EFI_SUCCESS != I2CDriver_Init(&pPowerParams->sI2CConfig))
  {
    DEBUG((EFI_D_WARN, "DisplayDxe: I2C init failed!\n"));
  }

  //  Enable backlight: VLCM_BL_SOURCE (source:VLCM_3V3_SOURCE , enable: BL_EN_MSM), PWM for the panel is from eDP bridge chip GPIO_04
  if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /* Here, We are using PWM from TI SN65DSI86 bridge */
    /* Enable backlight PWM signal: config GPIO_8 as output always first */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BACKLIGHT_PWM_EN, 2, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for GP_PDM_BKLIGHT failed!\n", CLS_GPIO_BACKLIGHT_PWM_EN));
    }

    /* Pull high GPIO 8 to enable BACKLIGHT_PWM_EN */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_BACKLIGHT_PWM_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: GPIO %d line high failed!\n", CLS_GPIO_BACKLIGHT_PWM_EN));
    }
      
    /* Enable backlight: VLCM_BL_SOURCE (source:VLCM_3V3_SOURCE , enable: BL_EN_MSM) */
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BACKLIGHT_SOURCE_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for BL_EN_MSM failed!\n", CLS_GPIO_BACKLIGHT_SOURCE_EN));
    }

    /* Pull high GPIO 11 to enable backlight */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_BACKLIGHT_SOURCE_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: GPIO %d line high failed!\n", CLS_GPIO_BACKLIGHT_SOURCE_EN));
    }
  }

   return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_Brightness_Level_Cls_Sn65dsix6_Wqhd_Vid()
*
* DESCRIPTION:
*   I2C brightness control for CLS Sn65dsix6_Wqhd_Vid panel.
*
***********************************************************************************************/
MDP_Status Panel_Brightness_Level_Cls_Sn65dsix6_Wqhd_Vid(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig)
{
  MDP_Status            Status            = MDP_STATUS_OK;
  uint32                uBacklightValue   = 0;
  uint32                uIndex            = 0;

  // Configure backlight PWM 
  if ( (pBacklightConfig != NULL) && (MDP_PANEL_BACKLIGHTTYPE_I2C == pBacklightConfig->eBacklightType))
  {
    // BACKLIGHT = Ceil[(PWM_DUTY (%) / 100)*(BACKLIGHT_SCALE)]
    uBacklightValue = DISP_DIV_ROUND_UP(pBacklightConfig->uLevel * 0xFF, 100);

    // Configure backlight in I2C command, here backlight value is placed 3rd in the command, for example: {SLAVE_ADDRESS, REG_ADDRESS, BACKLIGHT_VALUE}
    geDP_BacklightCommands[0][CLS_I2C_BACKLIGHT_COMMAND_PACKETS-1] = uBacklightValue;

    for (uIndex = 0; uIndex < CLS_I2C_BACKLIGHT_COMMANDS; uIndex++)
    {
      pBacklightConfig->uBacklightCntrl.sI2CConfig.pI2CInitSequence = (uint8*)&geDP_BacklightCommands[uIndex];

      // Send I2C command to program PWM in eDP bridge, to control CLS backlight
      if (EFI_SUCCESS != I2CPanelSendCommandSequence(&pBacklightConfig->uBacklightCntrl.sI2CConfig, MDP_PANEL_COMMAND_CUSTOM, NULL, 0))
      {
        DEBUG((EFI_D_WARN, "DisplayDxe: Backlight I2C send command failed!\n"));
      }
    }
  }

   return Status;
}


/**********************************************************************************************
*
* FUNCTION: Panel_Reset_Cls_Sn65dsix6_Wqhd_Vid()
*
* DESCRIPTION:
*   Panel reset sequence for CLS Sn65dsix6_Wqhd_Vid panel.
*
***********************************************************************************************/
MDP_Status Panel_Reset_Cls_Sn65dsix6_Wqhd_Vid(MDPPlatformPanelResetInfo *pResetInfo)
{
  MDP_Status                    Status       = MDP_STATUS_OK;
  EFI_TLMM_PROTOCOL            *TLMMProtocol = NULL;

  if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol))
  {
    DEBUG((EFI_D_ERROR, "DisplayDxe: Locate TLMM protocol failed!\n"));
    Status = MDP_STATUS_NO_RESOURCES;
  }
  else
  {
    /*
    * Toggle Reset
    */

    /* Enable Display EN pin*/
    if (EFI_SUCCESS != TLMMProtocol->ConfigGpio((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_EN, 0, GPIO_OUTPUT, GPIO_PULL_UP, GPIO_16MA), TLMM_GPIO_ENABLE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Configure GPIO %d for CLS_GPIO_BRIDGE_EN line failed!\n", CLS_GPIO_BRIDGE_EN));
    }

    /* Pull CLS_DISP_BRIDGE_EN Low */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), GPIO_LOW_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: Pull CLS_GPIO_BRIDGE_EN Low failed!\n"));
    }

    /* Wait 10us, CLS_DISP_BRIDGE_EN Need to be low for at least 10us to trigger reset */
    MDP_OSAL_DELAYUS(10);

    /* Set CLS_DISP_BRIDGE_EN line HIGH */
    if (EFI_SUCCESS != TLMMProtocol->GpioOut((UINT32)EFI_GPIO_CFG(CLS_GPIO_BRIDGE_EN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_16MA), GPIO_HIGH_VALUE))
    {
      DEBUG((EFI_D_WARN, "DisplayDxe: CLS_GPIO_BRIDGE_EN line HIGH failed!\n"));
    }

    /* 100us to allow panel to initialize after the reset*/
    MDP_OSAL_DELAYUS(100);
  }

  return Status;
}

#ifdef __cplusplus
}
#endif
