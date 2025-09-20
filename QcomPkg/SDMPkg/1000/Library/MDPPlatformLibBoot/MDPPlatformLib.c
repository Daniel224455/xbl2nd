/*=============================================================================
 
  File: MDPPlatformLib.c
 
  MDP platform specific functions
  
  Copyright (c) 2016-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#include <PlatformInfo.h>
#include <Library/PcdLib.h>
#include <api/pmic/pm/pm_lcdb.h>
#include <api/pmic/pm/pm_wled.h>
#include <api/pmic/pm/pm_ldo.h>
#include <api/pmic/pm/pm_smps.h>
#include <api/pmic/pm/pm_boot.h>
#include <api/pmic/pm/pm_version.h>
#include <api/pmic/pm/pm_boot.h>
#include <api/pmic/pm/pm_gpio.h>
#include <api/pmic/pm/pm_pwm.h>
#include <api/pmic/pm/pm_err_flags.h>
#include "MDPTypes.h"
#include "MDPPlatformLib.h"
#include "MDPSystem.h"
#include "DDITlmm.h"
#include "pmapp_npa.h"
#include "npa.h"
#include "boot_extern_platforminfo_interface.h"




#define GPIO_EDP_DP_HPD               10
#define CLS_GPIO_VLCM_3V3_EN          130
#define PM_GPIO_EDP_PWM_EN            PM_GPIO_8
#define PM_GPIO_EDP_PWM_PULSE         PM_GPIO_10
#define PMIC_C_DEV_INDEX              PMIC_C 
#define PMIC_E_DEV_INDEX              PMIC_E 
#define PMIC_RSRC_INDEX               PM_PWM_RESOURCE_PWM_2

#define MAXIMUM_VALUE(bits)           ((1<<bits)-1)
#define PERCENT_MAX                   100
#define CEIL(a,b)                     (((a)+(b-1))/b)


/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */
#define PCD_PANEL_TYPE_OEM                1

#define DSI_READ_ADDRESS_SIZE             2
#define DSI_READ_READBACK_SIZE            8

#define PLATFORM_PANEL_ID_MAX_COMMANDS    3       // maximum panel ID read commands

#define DISP_MAXIMUM_VALUE(bits)               ((1<<bits)-1)
#define DISP_PERCENT_MAX                       100
#define DISP_CEIL(a,b)                         (((a)+(b-1))/b)
#define DISP_ABS_DELTA(_a_,_b_)                (((_a_)>(_b_))?((_a_)-(_b_)):((_b_)-(_a_)))

// PMIC Device Indices
#define PMIC_PMI_DEV_INDEX                1       // PMIC interface device (IBB/LAB, GPIO,  WLED)

#define PMIC_IBBLAB_READYTIME             25      // Maximum of milliseconds to wait for IBB/LAB to stabilize (Can be up to 8.8ms +/-16%)

/* 8998 Main Panel CDP specific defines
*/
#define CDP_DISP_RESET_GPIO                7      //GPIO number for LCD0_RESET
#define CDP_DISP_TE_GPIO                   8      //GPIO number for TE
#define CDP_DISP_MODE_SEL                  6      //GPIO for panel mode select 0 = Dual Port, 1 = Single Port (DSC)

/*
*  Macro definition for PWM desired frequency default
*/
#define MDP_PMIC_PWM_DESIRED_FREQ_DEFAULT_HZ   19200     // Defaulted to 19200 HZ

/* PWM freq related info
*/
#define PWM_PREDIVIDER_INDEX_MAX               3         // max value of predivider index
#define PWM_EXPONENT_MAX                       7         // max value of the PWM exponent
#define PWM_MASTER_CLOCK_FREQ_19MHZ            19200000  // master clock frequency

/* -----------------------------------------------------------------------
** Types
** ----------------------------------------------------------------------- */

/* Panel command ID list for dynamic panel detection
*/

typedef struct {
  uint8      address[DSI_READ_ADDRESS_SIZE];                                        // DCS command for panel ID
  uint8      expectedReadback[DSI_READ_READBACK_SIZE];                              // expected readback
} PlatformPanelIDCommandInfo;

/* Dynamic panel ID detection entry 
 */
typedef struct {
  uint8                         uCmdType;                                             // data type for panel ID DSI read
  uint32                        uTotalRetry;                                          // number of retry if DSI read fails
  PlatformPanelIDCommandInfo    panelIdCommands[PLATFORM_PANEL_ID_MAX_COMMANDS];      // commands for panel ID and expected readback
  uint32                        uLaneRemapOrder;                                      // Lane remap order
  const int8                   *psPanelCfg;                                           // pointer to the panel configuration
  uint32                        uPanelCfgSize;                                        // size of panel configuration data
  uint32                        uFlags;                                               // flags to set clock config for now, can set other config in future
} PlatformDSIDetectParams;

/* Power configuration 
 */
typedef struct
{
  MDP_PmicModuleControlType     ePMICSecondaryPower[MDP_DISPLAY_MAX];                 // Configuration for PMIC based secondary power source
} Panel_PowerCtrlParams;
/* NPA Client info
*/
typedef struct
{
  char*                         cResourceName;
  char*                         cClientName;
}NPAClientName;


/*
 * Structure for PWM freq info
 */
typedef struct
{
  uint32     uRequestedFrequency;    // Requested PWM freq
  uint32     uPreDivider;            // PWM freq Pre divider
  uint32     uExponent;              // PWM freq exponent
  uint32     uActualFrequency;       // Actual PWM freq given
  uint32     uPwmBitWidth;           // PWM Bit Width
} MDP_PMICPWMConfigurationType;
/* -----------------------------------------------------------------------
** Local functions
** ----------------------------------------------------------------------- */

/* Platform detection 
*/
static MDP_Status ReadPlatformIDAndChipID(EFI_PLATFORMINFO_PLATFORM_INFO_TYPE *pPlatformInfo, EFIChipInfoIdType *pChipSetId, EFIChipInfoFamilyType *pChiSetFamily);

static void SetupPlatformPanelConfig(MDP_Display_IDType eDisplayId, MDPPlatformPanelInfo *pPlatformPanel, MDPPlatformInfo      *pPlatformInfo);

/* 8998 Main Panel CDP Functions
*/
static MDP_Status Panel_CDP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
static MDP_Status Panel_CDP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
static MDP_Status Panel_CDP_Reset(void);
static MDP_Status Panel_CDP_BacklightLevel(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);
static MDP_Status Panel_CDP_PeripheralPower(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp);

static MDP_Status Panel_CLS_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
static MDP_Status Panel_CLS_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
MDP_Status Panel_CLS_Peripheral_Power(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp);
static MDP_Status Panel_CLS_Backlight_Enable(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);

static void Panel_IBBLAB_WaitForReady();

/* Main Panel CDP/MTP Functions
*/


/* General helper functions */
MDP_Status SetDefaultGPIOState(GpioStateType *pGPIOList);


/*===========================================================================
 Local Configuration Definitions
 ===========================================================================*/
/******************************************************************
 *                   Sharp 4K  eDP Panel                          *
 *                                                                *
 *                   eDP panel                                    *
 ******************************************************************/
const static int8 Sharp_4k_eDP_xmldata[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"<PanelName>LQ133D1JW33B</PanelName>"
"<PanelDescription>Sharp 4K eDP Video Mode Panel(2160x3840 24bpp)</PanelDescription>"
"<Group id=\"Active Timing\">"
" <HorizontalActive>0</HorizontalActive>"
" <HorizontalFrontPorch>0</HorizontalFrontPorch>"
" <HorizontalBackPorch>0</HorizontalBackPorch>"
" <HorizontalSyncPulse>0</HorizontalSyncPulse>"
" <HorizontalSyncSkew>0</HorizontalSyncSkew>"
" <HorizontalLeftBorder>0</HorizontalLeftBorder>"
" <HorizontalRightBorder>0</HorizontalRightBorder>"
" <VerticalActive>0</VerticalActive>"
" <VerticalFrontPorch>0</VerticalFrontPorch>"
" <VerticalBackPorch>0</VerticalBackPorch>"
" <VerticalSyncPulse>0</VerticalSyncPulse>"
" <VerticalSyncSkew>0</VerticalSyncSkew>"
" <VerticalTopBorder>0</VerticalTopBorder>"
" <VerticalBottomBorder>0</VerticalBottomBorder>"
" <InvertDataPolarity>False</InvertDataPolarity>"
" <InvertVsyncPolairty>False</InvertVsyncPolairty>"
" <InvertHsyncPolarity>False</InvertHsyncPolarity>"
" <BorderColor>0x0</BorderColor>"
"</Group>"
"<Group id=\"Display Interface\">"
" <InterfaceType>18</InterfaceType>"
" <InterfaceColorFormat>3</InterfaceColorFormat>"
"</Group>"
"<Group id=\"eDP Interface\">"
"  <EDPRefreshRate>0x3c0000</EDPRefreshRate>"
"  <EDPTraining>0</EDPTraining>"
"  <EDPVoltageSwingLevel>0</EDPVoltageSwingLevel>"
"  <EDPPreemphasisLevel>0</EDPPreemphasisLevel>"
"  <EDPDPCDRead>False</EDPDPCDRead>"
"  <EDPEDIDRead>False</EDPEDIDRead>"
"  <EDPNumberOfLanes>4</EDPNumberOfLanes>"
"  <EDPLinkRate>540000</EDPLinkRate>"
"  <EDPHPDActiveLow>True</EDPHPDActiveLow>"
"  <EDPPowerUpWaitInMs>0</EDPPowerUpWaitInMs>"
"  <EDPMaxAuxRetry>20</EDPMaxAuxRetry>"
"</Group>";

/******************************************************************
 *                   Sharp 5.5" 4k Panel                            *
 *                                                                *
 *                   nt35950 DDIC                                 *
 *                   2160x3840                                    *
 *                   Dual-DSI(DSI0 and DSI1)                      *
 *                   Video mode DSC                               *
 ******************************************************************/
const static int8 Sharp_4k_video_dsc_xmldata[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"<PanelName>LS055D1SX04</PanelName>"
"<PanelDescription>Sharp Dual DSI Video Mode Panel with DSC (2160x3840 24bpp)</PanelDescription>"
"<Group id=\"Active Timing\">"
" <HorizontalActive>2160</HorizontalActive>"
" <HorizontalFrontPorch>30</HorizontalFrontPorch>"
" <HorizontalBackPorch>100</HorizontalBackPorch>"
" <HorizontalSyncPulse>4</HorizontalSyncPulse>"
" <HorizontalSyncSkew>0</HorizontalSyncSkew>"
" <HorizontalLeftBorder>0</HorizontalLeftBorder>"
" <HorizontalRightBorder>0</HorizontalRightBorder>"
" <VerticalActive>3840</VerticalActive>"
" <VerticalFrontPorch>8</VerticalFrontPorch>"
" <VerticalBackPorch>7</VerticalBackPorch>"
" <VerticalSyncPulse>1</VerticalSyncPulse>"
" <VerticalSyncSkew>0</VerticalSyncSkew>"
" <VerticalTopBorder>0</VerticalTopBorder>"
" <VerticalBottomBorder>0</VerticalBottomBorder>"
" <InvertDataPolarity>False</InvertDataPolarity>"
" <InvertVsyncPolairty>False</InvertVsyncPolairty>"
" <InvertHsyncPolarity>False</InvertHsyncPolarity>"
" <BorderColor>0x0</BorderColor>"
"</Group>"
"<Group id=\"Display Interface\">"
" <InterfaceType>8</InterfaceType>"
" <InterfaceColorFormat>3</InterfaceColorFormat>"
"</Group>"
"<Group id=\"DSI Interface\">"
" <DSIChannelId>1</DSIChannelId>"
" <DSIVirtualId>0</DSIVirtualId>"
" <DSIColorFormat>36</DSIColorFormat>"
" <DSITrafficMode>1</DSITrafficMode>"
" <DSIDSCEnable>True</DSIDSCEnable>"
" <DSIDSCMajor>1</DSIDSCMajor>"
" <DSIDSCMinor>1</DSIDSCMinor>"
" <DSIDSCScr>0</DSIDSCScr>"
" <DSIDSCProfileID>4</DSIDSCProfileID>"
" <DSIDSCSliceWidth>1080</DSIDSCSliceWidth>"
" <DSIDSCSliceHeight>32</DSIDSCSliceHeight>"
" <DSILanes>4</DSILanes>"
  " <DSIHsaHseAfterVsVe>False</DSIHsaHseAfterVsVe>\n"
  " <DSILowPowerModeInHFP>False</DSILowPowerModeInHFP>\n"
  " <DSILowPowerModeInHBP>False</DSILowPowerModeInHBP>\n"
  " <DSILowPowerModeInHSA>False</DSILowPowerModeInHSA>\n"
  " <DSILowPowerModeInBLLPEOF>True</DSILowPowerModeInBLLPEOF>\n"
  " <DSILowPowerModeInBLLP>True</DSILowPowerModeInBLLP>\n"
  " <DSIRefreshRate>0x3C0000</DSIRefreshRate>\n"
  " <DSIPhyDCDCMode>True</DSIPhyDCDCMode>\n"
" <DSIControllerMapping>\n"
"  00 01\n"
" </DSIControllerMapping>\n" 
"</Group>"
"<DSIInitSequence>"
" 39 91 09 20 00 20 02 00 03 1c 04 21 00 0f 03 19 01 97\n"
" 39 92 10 f0\n"
" 15 90 03\n"
" 15 03 01\n"
" 39 f0 55 aa 52 08 04\n"
" 15 c0 03\n"
" 39 f0 55 aa 52 08 07\n"
" 15 ef 01\n"
" 39 f0 55 aa 52 08 00\n"
" 15 b4 10\n"  /* DSC */
" 15 35 00\n"
" 39 f0 55 aa 52 08 01\n"
" 39 ff aa 55 a5 80\n"
" 15 6f 01\n"
" 15 f3 10\n"
" 39 ff aa 55 a5 00\n"
" 05 11\n"
" ff 78\n"
" 05 29\n"
" ff 78\n"
"</DSIInitSequence>"
"<DSITermSequence>"
" 05 28\n"
" 05 10\n"
"</DSITermSequence>"
"<TLMMGPIODefaultLow>34</TLMMGPIODefaultLow>"
"<Group id='Backlight Configuration'>"
" <BacklightType>1</BacklightType>\n"
" <BacklightPmicControlType>2</BacklightPmicControlType>\n"  
" <DisplayResetInfo>0 10 1000 10000 0</DisplayResetInfo>\n"
"</Group>\n";

/*
* Dummy panel configuration, default fallback mode.
*
*/
const static int8 dummy_xmldata[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"    <PanelName>VirtioDSI</PanelName>"
"    <PanelDescription>DSI Model (640x480 24bpp)</PanelDescription>"
"    <Group id=\"Active Timing\">"
"        <HorizontalActive units=\"Dot Clocks\">640</HorizontalActive>"
"        <HorizontalFrontPorch units=\"Dot Clocks\">13</HorizontalFrontPorch>"
"        <HorizontalBackPorch units=\"Dot Clocks\">14</HorizontalBackPorch>"
"        <HorizontalSyncPulse units=\"Dot Clocks\">15</HorizontalSyncPulse>"
"        <HorizontalSyncSkew units=\"Dot Clocks\">0</HorizontalSyncSkew>"
"        <HorizontalLeftBorder units=\"Dot Clocks\">0</HorizontalLeftBorder>"
"        <HorizontalRightBorder units=\"Dot Clocks\">0</HorizontalRightBorder>"
"        <VerticalActive units=\"Dot Clocks\">480</VerticalActive>"
"        <VerticalFrontPorch units=\"Lines\">11</VerticalFrontPorch>"
"        <VerticalBackPorch units=\"Lines\">12</VerticalBackPorch>"
"        <VerticalSyncPulse units=\"Lines\">13</VerticalSyncPulse>"
"        <VerticalSyncSkew units=\"Lines\">0</VerticalSyncSkew>"
"        <VerticalTopBorder units=\"Lines\">0</VerticalTopBorder>"
"        <VerticalBottomBorder units=\"Lines\">0</VerticalBottomBorder>"
"        <InvertDataPolarity>False</InvertDataPolarity>"
"        <InvertVsyncPolairty>False</InvertVsyncPolairty>"
"        <InvertHsyncPolarity>False</InvertHsyncPolarity>"
"        <BorderColor>0x0</BorderColor>"   
"    </Group>"
"    <Group id=\"Display Interface\">"
"        <InterfaceType units=\"QDI_DisplayConnectType\">8</InterfaceType>"
"        <InterfaceColorFormat units=\"QDI_PixelFormatType\">3</InterfaceColorFormat>"
"    </Group>"
"    <Group id=\"DSI Interface\">"
"        <DSIChannelId units=\"DSI_Channel_IDType\">1</DSIChannelId>"     
"        <DSIVirtualId units=\"DSI_Display_VCType\">0</DSIVirtualId>"     
"        <DSIColorFormat units=\"DSI_ColorFormatType\">36</DSIColorFormat>"     
"        <DSITrafficMode units=\"DSI_TrafficModeType\">0</DSITrafficMode>"
"        <DSILanes units=\"integer\">2</DSILanes>"
"        <DSIHsaHseAfterVsVe units=\"Bool\">False</DSIHsaHseAfterVsVe>"
"        <DSILowPowerModeInHFP units=\"Bool\">False</DSILowPowerModeInHFP>"
"        <DSILowPowerModeInHBP units=\"Bool\">False</DSILowPowerModeInHBP>"
"        <DSILowPowerModeInHSA units=\"Bool\">False</DSILowPowerModeInHSA>"
"        <DSILowPowerModeInBLLPEOF units=\"Bool\">False</DSILowPowerModeInBLLPEOF>"
"        <DSILowPowerModeInBLLP units=\"Bool\">False</DSILowPowerModeInBLLP>"
"        <DSIRefreshRate>0x3c0000</DSIRefreshRate>" 
"    </Group>"
"        <DisplayPrimaryFlags>0x2</DisplayPrimaryFlags>"
"    <DSIInitSequence>"
"    32 00 00\n"
"    </DSIInitSequence>";

/*===========================================================================
Function Definitions
===========================================================================*/

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: PlatformClientInit()
** 
** DESCRIPTION:
**        Initialize NPA client
**
*//* -------------------------------------------------------------------- */
static MDP_Status PlatformClientInit(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status    eStatus = MDP_STATUS_OK;

  return eStatus;
}



/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_PowerUp()
** 
** DESCRIPTION:
**        Panel power up sequence for CDP
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status        eStatus        = MDP_STATUS_OK;

  if (MDP_STATUS_OK != (eStatus =  PlatformClientInit(eDisplayId, pPowerParams)))
  {
    MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayLib: Failed to initialize handle for Primary display NPA node.\n");
  }
  else
  {
    DALGpioSignalType uGPIOCfg = 0;

    /* LDO1, LDO2 for DSI PHY/PLL are already turned on by system driver.*/

    /*Turn on LDO14 for V_LCD_VDDI_1P8 which is used by panel */
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_volt_level(PMIC_C, PM_LDO_3, 1200000, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error configuring LDO to 1.8V\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_volt_level(PMIC_E, PM_LDO_5, 880000, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error configuring LDO to 1.8V\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_volt_level(PMIC_C, PM_LDO_1, 1800000, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error configuring LDO to 1.8V\n");
    }

    if (PM_ERR_FLAG__SUCCESS != pm_ldo_sw_enable(PMIC_C, PM_LDO_3, PM_ON, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO3\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_sw_enable(PMIC_E, PM_LDO_5, PM_ON, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO5\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_sw_enable(PMIC_C, PM_LDO_1, PM_ON, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO14\n");
    }


    MDP_OSAL_DELAYMS(10);   /* delay 10ms to allow power grid to settle */

    /* only support dual dsi mode */
    uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CDP_DISP_MODE_SEL, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_LOW_VALUE);
    Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

    MDP_OSAL_DELAYMS(10);   /* delay 10ms for panel to reset (from panel spec) */

    uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG(CDP_DISP_TE_GPIO, 1, DAL_GPIO_INPUT, DAL_GPIO_NO_PULL, DAL_GPIO_LOW_VALUE);
    Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);
  }

  return eStatus;
}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_PowerDown()
** 
** DESCRIPTION:
**        Panel power down sequence for CDP
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status          eStatus      = MDP_STATUS_OK;
  DALGpioSignalType   uGPIOCfg     = 0;
  
  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CDP_DISP_RESET_GPIO, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_LOW_VALUE);
  eStatus  = Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_Reset()
** 
** DESCRIPTION:
**        Panel reset sequence for CDP
**
*/ /* -------------------------------------------------------------------- */
MDP_Status Panel_CDP_Reset(void)
{
  MDP_Status          eStatus     = MDP_STATUS_OK;
  DALGpioSignalType   uGPIOCfg    = 0;

  MDP_OSAL_DELAYMS(10);

  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CDP_DISP_RESET_GPIO, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_HIGH_VALUE);
  Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

  MDP_OSAL_DELAYMS(10);
  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CDP_DISP_RESET_GPIO, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_LOW_VALUE);
  Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

  MDP_OSAL_DELAYMS(10);
    
  uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CDP_DISP_RESET_GPIO, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_HIGH_VALUE);
  Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);
  
  MDP_OSAL_DELAYMS(10);
  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_PeripheralPower()
** 
** DESCRIPTION:
**        Secondary power sequence for other PMIC modules such as IBB/LAB
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_PeripheralPower(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp)
{
  MDP_Status eStatus = MDP_STATUS_OK;

  if (MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD == pPowerParams->ePMICSecondaryPower[eDisplayId])
  {
    if (TRUE == bPowerUp)
    {
      if (PM_ERR_FLAG__SUCCESS != pm_lcdb_set_output_volt(PMIC_C, 5500,-5500, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Lcdb 5.5V failed\n");
      }

      if (PM_ERR_FLAG__SUCCESS != pm_lcdb_config_enable_ctrl(PMIC_C, PM_LCDB_ENABLE_CTL_MODULE_EN, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Lcdb Enable failed\n");
      }

      // Wait for IBB/LAB to become ready
      Panel_IBBLAB_WaitForReady();

      /************************************************************************
       *  Enable brightness power source
       ************************************************************************/
      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_modulator(PMIC_C, PM_MODULATOR_TYPE_A,TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled EnableModulatorA failed\n");
      }

      if (PM_ERR_FLAG__SUCCESS != pm_wled_set_led_duty_cycle(PMIC_C, PM_MODULATOR_TYPE_A,0x1ff))  /* duty cycle is 12 bits */
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled SetLedDutyCycle failed\n");
      }

      if (PM_ERR_FLAG__SUCCESS != pm_wled_modulator_src_sel(PMIC_C, PM_MODULATOR_TYPE_A, PM_MODULATOR_SRC_DRV_HIGH))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled ModulatorSrcSelect failed\n");
      }

      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_module(PMIC_C, 0, TRUE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled EnableWled failed\n");
      }
   
      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_current_sink(PMIC_C, PM_WLED_LED1|PM_WLED_LED2|PM_WLED_LED3))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled EnableCurrentSink failed\n");
      }
    }
    else
    {
      // ********** Brightness Power-down Sequence **********
      // Power down
      if (PM_ERR_FLAG__SUCCESS !=   pm_lcdb_config_enable_ctrl(PMIC_C, PM_LCDB_ENABLE_CTL_MODULE_EN, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Error to disable LCDB\n");
      }

      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_current_sink(PMIC_C, PM_WLED_NONE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled EnableCurrentSink failed\n");
      }
   
      if (PM_ERR_FLAG__SUCCESS != pm_wled_enable_module(PMIC_C, 0, FALSE))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled EnableWled failed\n");
      }

    }
  }
  else
  {
    /* do nothing for amoled panel */
  }
  
  return eStatus;
}


/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_CDP_BacklightLevel()
**
** DESCRIPTION:
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status Panel_CDP_BacklightLevel(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig)
{
  MDP_Status                    eStatus = MDP_STATUS_OK;

  if (MDP_PANEL_BACKLIGHTTYPE_PMIC == pBacklightConfig->eBacklightType)
  {
    // Configure the power grid based on the module type
    switch (pBacklightConfig->uBacklightCntrl.eBacklightCtrl)
    {
    case MDP_PMIC_BACKLIGHT_CONTROLTYPE_WLED:
      {

        uint32 uWLEDValue = (0xFFF * pBacklightConfig->uLevel) / 100; // Calculate duty cycle based on 12 bit mode

        if (PM_ERR_FLAG__SUCCESS != pm_wled_set_led_duty_cycle(PMIC_C, PM_MODULATOR_TYPE_A, uWLEDValue))  /* PM_WLED_ALL */
        {
          MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: Wled pm_wled_set_led_duty_cycle failed\n");
        }
      }
      break;
    default:
      break;
    }
  }
  else
  {
    // Nothing to do for other configurations
  }

  return eStatus;
}

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: Panel_IBBLAB_WaitForReady()
** 
** DESCRIPTION:
**  Polling loop to wait for IBB/LAB modules to be ready.
**  VDISN, VDISP will reach the proper voltage upon ready.
**/
static void Panel_IBBLAB_WaitForReady()
{

  boolean lcdbStatus;
  
  uint32                uCount;

  for (uCount = 0; uCount < PMIC_IBBLAB_READYTIME; uCount++)
  {
    if (PM_ERR_FLAG__SUCCESS == pm_lcdb_vreg_ok(PMIC_C, &lcdbStatus)) 
    {
      if (lcdbStatus)
      {
        break;
      }
      else if (PMIC_IBBLAB_READYTIME == (uCount + 1))
      {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: LAB/IBB GetStatus(LabVregOk) failed after %dms\n", PMIC_IBBLAB_READYTIME);
      }

      // Wait 1ms between checks
      MDP_OSAL_DELAYMS(1);
    }
    else
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: IBB/LAB GetStatus() Failed\n");
      break;
    }
  }
}

MDP_Status MDPPlatformConfigure(MDP_Display_IDType eDisplayId, MDPPlatformConfigType eConfig, MDPPlatformParams *pPlatformParams)
{
  MDP_Status  eStatus  = MDP_STATUS_OK;
//  uint32      uPanelID = 0;

  /* Static information, initial once during the first call */
  static MDPPlatformInfo        sPlatformInfo;
  static Panel_PowerCtrlParams  sPanelPowerCtrl;
  static MDPPlatformPanelInfo   sPlatformPanel;

  switch (eConfig)
  {
  case MDPPLATFORM_CONFIG_INIT:
    MDP_OSAL_MEMZERO(&sPlatformInfo,    sizeof(MDPPlatformInfo));
    MDP_OSAL_MEMZERO(&sPanelPowerCtrl,  sizeof(Panel_PowerCtrlParams));
    MDP_OSAL_MEMZERO(&sPlatformPanel,   sizeof(MDPPlatformPanelInfo));


    // Read the platform ID once
    if (MDP_STATUS_OK != ReadPlatformIDAndChipID(&sPlatformInfo.sEFIPlatformType, &sPlatformInfo.sEFIChipSetId, &sPlatformInfo.sEFIChipSetFamily))
    {
        MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxeBoot: ReadChipID failed\n");
    }

    SetupPlatformPanelConfig(eDisplayId, &sPlatformPanel, &sPlatformInfo);

    /* Enable VDDMM so that MMCX Arc can be configured */
    if (PM_ERR_FLAG__SUCCESS != pm_smps_volt_level(PMIC_C, PM_SMPS_5, 752*1000, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error set VDDMM level \n");
    }
    if (PM_ERR_FLAG__SUCCESS != pm_smps_sw_enable(PMIC_C, PM_SMPS_5, PM_ON, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enable VDDMM\n");
    }
    break;

  case MDPPLATFORM_CONFIG_SW_RENDERER:

    if ((EFI_PLATFORMINFO_TYPE_UNKNOWN == sPlatformInfo.sEFIPlatformType.platform) ||
        (EFI_PLATFORMINFO_TYPE_VIRTIO  == sPlatformInfo.sEFIPlatformType.platform) ||
        (EFI_PLATFORMINFO_TYPE_RUMI    == sPlatformInfo.sEFIPlatformType.platform) ||
        (TRUE == PcdGetBool(PcdDisplayForceSwRenderer)))
    {
      // Treat unknown platforms as a SW model only
      pPlatformParams->sPlatformInfo.bSWRender = TRUE;
      MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayDxeBoot: SW renderer only\n");
    }
    else
    {
       pPlatformParams->sPlatformInfo.bSWRender = FALSE;
    }
    break;
  case MDPPLATFORM_CONFIG_GETPANELCONFIG:
    {
      pPlatformParams->sPlatformPanel.uDefaultVendor   = 0;
      pPlatformParams->sPlatformPanel.uDefaultRevision = 0;
      pPlatformParams->sPlatformPanel.uPanelId         = 0;

      // Retrieve panel configuration (could be dependent on the interface)
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_CLS:
          /* Copy the panel configurations to pPlatformParams->sPlatformPanel */
          MDP_OSAL_MEMCPY(&pPlatformParams->sPlatformPanel, &sPlatformPanel, sizeof(MDPPlatformPanelInfo));
          break;
        default:
          pPlatformParams->sPlatformPanel.pPanelXMLConfig = (int8*)dummy_xmldata;
          pPlatformParams->sPlatformPanel.uConfigSize     = sizeof(dummy_xmldata);
          break;
        }
        break;

      case MDP_DISPLAY_EXTERNAL:
          pPlatformParams->sPlatformPanel.pPanelXMLConfig = (int8*)dummy_xmldata;
          pPlatformParams->sPlatformPanel.uConfigSize     = sizeof(dummy_xmldata);
          break;
      default:
        break;
      }
    }
    break;
  case MDPPLATFORM_CONFIG_POWERUP:
    {
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:

        // Config based on the platform
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_CLS:
          sPanelPowerCtrl.ePMICSecondaryPower[eDisplayId] = MDP_PMIC_MODULE_CONTROLTYPE_IBB_LAB_LCD;   // LCD Power Configuration (IBB/LAB)

          /* Check for common power or panel specific power,
           * Common Power : pPlatformParams is NULL or uFlags is 0x0,
           * Panel Power  : uFlags is set to panel specific power flag.
           */
          if ((NULL                          != pPlatformParams) &&
              (POWERCONFIG_FLAGS_PANEL_POWER  & pPlatformParams->sPowerConfig.uFlags))
          {
            switch (sPlatformInfo.uPrimaryPanelId)
            {
            default:
              // OEM to populate panel specific power up.
              break;
            }
          }
          else
          {
            /*
             * Ramdump mode Primary Power Sequence:
             * MDPPLATFORM_CONFIG_POWERDOWN is not be called at both enter and exit ramdump mode
             * Therefore panel power down is called first before power up panel to obey panel
             * power up sequence
             */
            if (sPlatformPanel.eSelectedPanel == MDPPLATFORM_PANEL_SHARP_4K_EDP)
            {
               if (MDP_STATUS_OK != (eStatus = Panel_CLS_PowerDown(eDisplayId, &sPanelPowerCtrl)))
               {
                  MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: Panel_PowerDown: Primary Power Down Sequence Failed (%d)\n", eStatus);
               }
               else if (MDP_STATUS_OK != (eStatus = Panel_CLS_Peripheral_Power(eDisplayId, &sPanelPowerCtrl, FALSE)))  // Secondary Power Sequence
               {
                 MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: Panel_PowerDown: Secondary Power Down Sequence Failed (%d)\n", eStatus);
               }
            }
            else
            {
               if (MDP_STATUS_OK != (eStatus = Panel_CDP_PowerDown(eDisplayId, &sPanelPowerCtrl)))
               {
                  MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: Panel_PowerDown: Primary Power Down Sequence Failed (%d)\n", eStatus);
               }
               else if (MDP_STATUS_OK != (eStatus = Panel_CDP_PeripheralPower(eDisplayId, &sPanelPowerCtrl, FALSE)))  // Secondary Power Sequence
               {
                 MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump: Panel_PowerDown: Secondary Power Down Sequence Failed (%d)\n", eStatus);
               }
            }

            MDP_OSAL_DELAYMS(10);   /* delay 10ms to allow power grid to settle */

            // Primary Power Sequence
            if (sPlatformPanel.eSelectedPanel == MDPPLATFORM_PANEL_SHARP_4K_EDP)
            {
               if (MDP_STATUS_OK != (eStatus = Panel_CLS_PowerUp(eDisplayId, &sPanelPowerCtrl)))
               {
                  MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayLib: Primary Power Up Sequence Failed (%d)\n", eStatus);
               }
               else if (MDP_STATUS_OK != (eStatus = Panel_CLS_Peripheral_Power(eDisplayId, &sPanelPowerCtrl, TRUE)))  // Secondary Power Sequence
               {
                 MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayLib: Secondary Power Up Sequence Failed (%d)\n", eStatus);
               }
            }
            else
            {
               if (MDP_STATUS_OK != (eStatus = Panel_CDP_PowerUp(eDisplayId, &sPanelPowerCtrl)))
               {
                 MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayLib: Primary Power Up Sequence Failed (%d)\n", eStatus);
               }
               else if (MDP_STATUS_OK != (eStatus = Panel_CDP_PeripheralPower(eDisplayId, &sPanelPowerCtrl, TRUE)))  // Secondary Power Sequence
               {
                 MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayLib: Secondary Power Up Sequence Failed (%d)\n", eStatus);
               }
            }
          }
          break;

        default:
          break;
        }

        break;

      case MDP_DISPLAY_EXTERNAL:

        // Config based on the platform
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
          break;
        default:
          break;
        }
        break;

      default:
        break;

      }
    }
    break;

  case MDPPLATFORM_CONFIG_POWERDOWN:
    {
       MDP_Log_Message(MDP_LOGLEVEL_WARN, "DisplayDxeBoot: Power Down not supported in this mode\n");
    }
    break;
  case MDPPLATFORM_CONFIG_SETBACKLIGHT:
    {
      // Handle backlight level
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_CLS:
            if (sPlatformPanel.eSelectedPanel == MDPPLATFORM_PANEL_SHARP_4K_EDP)
            {
               eStatus = Panel_CLS_Backlight_Enable(eDisplayId, &pPlatformParams->sBacklightConfig);
            }
            else
            {
               eStatus = Panel_CDP_BacklightLevel(eDisplayId, &pPlatformParams->sBacklightConfig);
            }
        default:
          break;
        }
        break;
      case MDP_DISPLAY_EXTERNAL:
        eStatus = MDP_STATUS_NOT_SUPPORTED;
        break;
      default:
        break;
      }
    }
    break;
  case MDPPLATFORM_CONFIG_GETPANELID:
    {
    }
    break;
  case MDPPLATFORM_CONFIG_GETPLATFORMINFO:
    {
      //
      // Return platform information
      //
      MDP_OSAL_MEMCPY(&pPlatformParams->sPlatformInfo, &sPlatformInfo, sizeof(MDPPlatformInfo));
    }
    break;
  case MDPPLATFORM_CONFIG_RESETPANEL:
    {
      // Handle power down
      switch (eDisplayId)
      {
      case MDP_DISPLAY_PRIMARY:

        // Config based on the platform
        switch (sPlatformInfo.sEFIPlatformType.platform)
        {
        case EFI_PLATFORMINFO_TYPE_CDP:
        case EFI_PLATFORMINFO_TYPE_MTP:
        case EFI_PLATFORMINFO_TYPE_CLS:
          eStatus = Panel_CDP_Reset();
        default:
          break;
        }

        break;
      case MDP_DISPLAY_EXTERNAL:
        eStatus = MDP_STATUS_NOT_SUPPORTED;
        break;
      default:
        break;
      }
    }
    break;

  case MDPPLATFORM_CONFIG_SETGPIOSTATE:
    {
      eStatus = SetDefaultGPIOState(&pPlatformParams->sGPIOState);
    }
    break;
      
  default:
    eStatus = MDP_STATUS_BAD_PARAM;
    break;
  }
  
  return eStatus;
}



/*===========================================================================
Private Function Definitions
===========================================================================*/

/* ---------------------------------------------------------------------- */
/**
** FUNCTION: ReadPlatformIDAndChipID()
** 
** DESCRIPTION:
**
*/ /* -------------------------------------------------------------------- */
static MDP_Status ReadPlatformIDAndChipID(EFI_PLATFORMINFO_PLATFORM_INFO_TYPE *pPlatformInfo, EFIChipInfoIdType *pChipSetId, EFIChipInfoFamilyType *pChipSetFamily)
{
  DalPlatformInfoPlatformInfoType      dalPlatformInfo;
  DALResult                            eDalStatus = DAL_SUCCESS;
  MDP_Status                           eStatus    = MDP_STATUS_OK;
  
  MDP_OSAL_MEMZERO(&dalPlatformInfo,    sizeof(DalPlatformInfoPlatformInfoType));
  
  eDalStatus = boot_DalPlatformInfo_PlatformInfo(&dalPlatformInfo);
  if (eDalStatus != DAL_SUCCESS)
  {
     MDP_Log_Message(MDP_LOGLEVEL_ERROR, "Ramdump:Display:ReadPlatformIDAndChipId: Failed\n");

     eStatus    = MDP_STATUS_FAILED;
  }
  else
  {
    pPlatformInfo->platform = (EFI_PLATFORMINFO_PLATFORM_TYPE)dalPlatformInfo.platform;
    pPlatformInfo->version  = dalPlatformInfo.version;
    pPlatformInfo->subtype  = dalPlatformInfo.subtype;
    pPlatformInfo->fusion   = dalPlatformInfo.fusion;

    eDalStatus = boot_DalPlatformInfo_ChipId(pChipSetId);
    eDalStatus = boot_DalPlatformInfo_ChipFamily(pChipSetFamily);
  }


  return eStatus;
}

/****************************************************************************
*
** FUNCTION: SetDefaultGPIOState()
*/
/*!
* \brief
*   The \b SetDefaultGPIOState function toggles a list of GPIO's based on the requested configuration
**
* \retval MDP_Status
*
****************************************************************************/
MDP_Status SetDefaultGPIOState(GpioStateType *pGPIOList)
{
  MDP_Status          eStatus = MDP_STATUS_OK;

  if ((NULL == pGPIOList) ||
      (NULL == pGPIOList->pGPIOList))
  {
    eStatus = MDP_STATUS_BAD_PARAM;
  }
  else
  {
    // Handle each type of GPIO differently
    switch (pGPIOList->eGPIOType)
    {
    case MDPPLATFORM_GPIOTYPE_TLMM:
    {
      uint32              uCount;

      // Loop the list to configure all GPIOs.
      for (uCount = 0;uCount < pGPIOList->uListSize;uCount++)
      {
        // Only try to program GPIOs that are within range, TLMM Macro (EFI_GPIO_CFG) supports up to 0x3FF
        if (pGPIOList->pGPIOList[uCount] < 0x3FF)
        {

          DALGpioSignalType   uGPIOCfg = 0;
          uint32              uValue  = (pGPIOList->uState > 0) ? DAL_GPIO_HIGH_VALUE : DAL_GPIO_LOW_VALUE;

          uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(pGPIOList->pGPIOList[uCount], 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, uValue);
          Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);
        }
      }
    }
    break;
    default:
      // Others (PMIC, PMI) not supported
      break;
    }
  }

  return eStatus;
}

/**********************************************************************************************
*
* FUNCTION: Panel_CLS_PowerUp()
*
* DESCRIPTION:
*   Panel power up sequence for CLS Platform.
*
***********************************************************************************************/
static MDP_Status Panel_CLS_PowerUp(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status        eStatus        = MDP_STATUS_OK;

  {
    DALGpioSignalType uGPIOCfg = 0;

    /* LDO1, LDO2 for DSI PHY/PLL are already turned on by system driver.*/

    /*Turn on LDO14 for V_LCD_VDDI_1P8 which is used by panel */
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_volt_level(PMIC_C, PM_LDO_3, 1200000, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error configuring LDO to 1.8V\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_volt_level(PMIC_E, PM_LDO_5, 880000, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error configuring LDO to 1.8V\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_volt_level(PMIC_C, PM_LDO_1, 1800000, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error configuring LDO to 1.8V\n");
    }

    if (PM_ERR_FLAG__SUCCESS != pm_ldo_sw_enable(PMIC_C, PM_LDO_3, PM_ON, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO3\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_sw_enable(PMIC_E, PM_LDO_5, PM_ON, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO5\n");
    }
    
    if (PM_ERR_FLAG__SUCCESS != pm_ldo_sw_enable(PMIC_C, PM_LDO_1, PM_ON, TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Error enabling LDO14\n");
    }

    MDP_OSAL_DELAYMS(10);   /* delay 10ms to allow power grid to settle */

    /* Enable pmic gpio */
   if (PM_ERR_FLAG_SUCCESS  != pm_gpio_config_digital_output(PMIC_C_DEV_INDEX,  
                                                             PM_GPIO_EDP_PWM_EN,
                                                             PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                             PM_GPIO_VIN1,
                                                             PM_GPIO_SOURCE_GND,
                                                             PM_GPIO_OUT_BUFFER_LOW,
                                                             TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Pull PMIC GPIO protocol failed!\n");
    }

   if (PM_ERR_FLAG_SUCCESS  != pm_gpio_config_digital_output(PMIC_C_DEV_INDEX, PM_GPIO_EDP_PWM_PULSE,
                                                            PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            PM_GPIO_VIN0,
                                                            PM_GPIO_SOURCE_SPECIAL_FUNCTION1,
                                                            PM_GPIO_OUT_BUFFER_LOW,
                                                            FALSE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Pull PMIC GPIO protocol failed!\n");
    }

   if (PM_ERR_FLAG_SUCCESS  != pm_gpio_config_digital_output(PMIC_E_DEV_INDEX, PM_GPIO_1,
                                                            PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            PM_GPIO_VIN1,
                                                            PM_GPIO_SOURCE_GND,
                                                            PM_GPIO_OUT_BUFFER_LOW,
                                                            TRUE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Pull PMIC GPIO protocol failed!\n");
    }


    MDP_OSAL_DELAYMS(200);

    uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CLS_GPIO_VLCM_3V3_EN, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_HIGH_VALUE);
    Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);

    /* route HPD signal to eDP controller */
    uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG(GPIO_EDP_DP_HPD, 1, DAL_GPIO_INPUT, DAL_GPIO_NO_PULL, DAL_GPIO_LOW_VALUE);
    Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_ENABLE, &uGPIOCfg, 1);
  }

  return eStatus;
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
  MDP_Status                    Status            = MDP_STATUS_OK;
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
static MDP_Status Panel_CLS_PowerDown(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams)
{
  MDP_Status                       Status           = MDP_STATUS_OK;
  DALGpioSignalType uGPIOCfg = 0;
  
    /* VLCM_3V3 (disable pull up for BL_PWM of LCD panel): Turn off GPIO88 */
    uGPIOCfg = (DALGpioSignalType)DAL_GPIO_CFG_OUT(CLS_GPIO_VLCM_3V3_EN, 0, DAL_GPIO_OUTPUT, DAL_GPIO_NO_PULL, DAL_GPIO_16MA, DAL_GPIO_LOW_VALUE);
    Tlmm_ConfigGpioGroup(DAL_TLMM_GPIO_DISABLE, &uGPIOCfg, 1);
  
    MDP_OSAL_DELAYUS(100);

  
    if (PM_ERR_FLAG_SUCCESS  != pm_gpio_config_digital_output(PMIC_C_DEV_INDEX,  
                                                             PM_GPIO_EDP_PWM_EN,
                                                             PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                             PM_GPIO_VIN1,
                                                             PM_GPIO_SOURCE_GND,
                                                             PM_GPIO_OUT_BUFFER_LOW,
                                                             FALSE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Disable PMIC GPIO protocol failed!\n");
    }
    
   if (PM_ERR_FLAG_SUCCESS  != pm_gpio_config_digital_output(PMIC_C_DEV_INDEX, PM_GPIO_EDP_PWM_PULSE,
                                                            PM_GPIO_OUT_BUFFER_CONFIG_CMOS,
                                                            PM_GPIO_VIN0,
                                                            PM_GPIO_SOURCE_SPECIAL_FUNCTION1,
                                                            PM_GPIO_OUT_BUFFER_LOW,
                                                            FALSE))
    {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: Pull PMIC GPIO protocol failed!\n");
    }
    
    MDP_OSAL_DELAYUS(100);
  
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
      pPWMConfiguration->uPwmBitWidth = PM_PWM_SIZE__6BIT;
      break;
    case 9:
      pPWMConfiguration->uPwmBitWidth = PM_PWM_SIZE__9BIT;
      break;
    default:
      uBitWidth = 9;
      pPWMConfiguration->uPwmBitWidth = PM_PWM_SIZE__9BIT;
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

/**********************************************************************************************
*
* FUNCTION: Panel_CLS_Brightness_Enable()
*
* DESCRIPTION:
*   Enable panel brightness for the CLS platform.
*
***********************************************************************************************/
static MDP_Status Panel_CLS_Backlight_Enable(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig)
{
   MDP_Status                      Status          = MDP_STATUS_OK;
   MDP_PMICPWMConfigurationType    sPWMConfiguration;

   MDP_OSAL_MEMZERO(&sPWMConfiguration, sizeof(MDP_PMICPWMConfigurationType));
   /*Get PWM freq-Predivider,Exponent */
   
   if (EFI_SUCCESS != MDP_CalculatePWMValue(pBacklightConfig->uPwmDesiredFreqHz, &sPWMConfiguration, pBacklightConfig->uNumBits))
   {
     MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: MDP_CalculatePWMValue:  failed!\n");
   }
   else if (PM_ERR_FLAG_SUCCESS != pm_pwm_clock_config(PMIC_C_DEV_INDEX, PMIC_RSRC_INDEX, sPWMConfiguration.uPwmBitWidth, PM_PWM_CLK__19_2_MHZ, sPWMConfiguration.uPreDivider, sPWMConfiguration.uExponent))
   {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: pm_pwm_clock_config:  failed!\n");
   }

   if (PM_ERR_FLAG_SUCCESS  != pm_pwm_enable_pwm(PMIC_C_DEV_INDEX, PMIC_RSRC_INDEX, TRUE))
   {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: pm_pwm_enable_pwm:  failed!\n");
   }

   if (PM_ERR_FLAG_SUCCESS  != pm_pwm_set_pwm_value(PMIC_C_DEV_INDEX, PMIC_RSRC_INDEX, 0x10))
   {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: pm_pwm_set_pwm_avle:  failed!\n");
   }

  
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
static MDP_Status Panel_CLS_Brightness_Level(MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig)
{
  MDP_Status                  Status          = MDP_STATUS_OK;
  uint32                      uLevel;

  uLevel = pBacklightConfig->uLevel;  /* 0 to 100 */
  uLevel = CEIL(uLevel*MAXIMUM_VALUE(9), PERCENT_MAX);
  if (PM_ERR_FLAG_SUCCESS  != pm_pwm_set_pwm_value(PMIC_C_DEV_INDEX, PMIC_RSRC_INDEX, uLevel))
  {
      MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayDxe: pm_pwm_set_pwm_avle:  failed!\n");
  }

  return Status;
}
/*************************************************************************/
static void SetupPlatformPanelConfig(MDP_Display_IDType    eDisplayId,
                                           MDPPlatformPanelInfo *pPlatformPanel,
                                           MDPPlatformInfo      *pPlatformInfo)
{

  switch (pPlatformInfo->sEFIPlatformType.platform)
  {
      case EFI_PLATFORMINFO_TYPE_CDP:
      case EFI_PLATFORMINFO_TYPE_MTP:
      case EFI_PLATFORMINFO_TYPE_QRD:
         if (pPlatformInfo->sEFIPlatformType.subtype == 1)
         {
            pPlatformPanel->pPanelXMLConfig    = (int8 *)Sharp_4k_eDP_xmldata;
            pPlatformPanel->uConfigSize        = sizeof(Sharp_4k_eDP_xmldata);
            pPlatformPanel->eSelectedPanel     = MDPPLATFORM_PANEL_SHARP_4K_EDP;
          }
          else
          {
            // default to Sharp 4k Dual DSI DSC Video Mode
            pPlatformPanel->pPanelXMLConfig    = (int8 *)Sharp_4k_video_dsc_xmldata;
            pPlatformPanel->uConfigSize        = sizeof(Sharp_4k_video_dsc_xmldata);
            pPlatformPanel->eSelectedPanel     = MDPPLATFORM_PANEL_SHARP_4K_DSC_VIDEO;
            pPlatformPanel->uPanelId           = 0x00008056;
            pPlatformInfo->uPrimaryPanelId     = 0x00008056;
          }
          break;
        case EFI_PLATFORMINFO_TYPE_CLS:
            pPlatformPanel->pPanelXMLConfig    = (int8 *)Sharp_4k_eDP_xmldata;
            pPlatformPanel->uConfigSize        = sizeof(Sharp_4k_eDP_xmldata);
            pPlatformPanel->eSelectedPanel     = MDPPLATFORM_PANEL_SHARP_4K_EDP;
          break;
        default:
          MDP_Log_Message(MDP_LOGLEVEL_ERROR, "DisplayBoot: Platform subtype=%d not supported!\n", pPlatformInfo->sEFIPlatformType.subtype);
          pPlatformPanel->pPanelXMLConfig      = (int8*)dummy_xmldata;
          pPlatformPanel->uConfigSize          = sizeof(dummy_xmldata);
          break;
    }
}

#ifdef __cplusplus
}
#endif
