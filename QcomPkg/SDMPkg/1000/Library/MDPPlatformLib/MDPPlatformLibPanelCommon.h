/*=============================================================================

  File: MDPPlatformLibPanelCommon.h

  MDP platform specific types

  Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifndef __MDP_PLATFORM_LIB_PANEL_COMMON__H__
#define __MDP_PLATFORM_LIB_PANEL_COMMON__H__

#include "MDPTypes.h"
#include "MDPPlatformLib.h"
#include "npa.h"
/* -----------------------------------------------------------------------
** Defines
** ----------------------------------------------------------------------- */

#define DP_PLUG_INVERTED_POLARITY         FALSE   // set to TRUE if USB type-c cable is plugged in reverse

#define PCD_PANEL_TYPE_OEM                1

#define DSI_READ_ADDRESS_SIZE             2
#define DSI_READ_READBACK_SIZE            8
#define DSI_WRITE_BACKLIGHT_SIZE          2

#define PLATFORM_PANEL_ID_MAX_COMMANDS    3       // maximum panel ID read commands

#define PANEL_LIST_MAX_COUNT              16      // maximum number of panels supported

/* PMIC Device Indices
 */
#define PMIC_PMI_DEV_INDEX                2       // PMIC interface device (IBB/LAB, GPIO,  WLED)

#define PMIC_IBBLAB_READYTIME             25      // Maximum of milliseconds to wait for IBB/LAB to stabilize (Can be up to 8.8ms +/-16%)

#define ADP_SA8195P_DISP_0_DISP_EN_GPIO   83      //GPIO number for POWER ENABLE pin for DSI-0
#define ADP_SA8195P_DISP_1_DISP_EN_GPIO   87      //GPIO number for POWER ENABLE pin for DSI-1 

/* SDM845 Main Panel default platform specific defines
 */
#define Default_DISP_RESET_GPIO           7       //GPIO number for LCD0_RESET
#define Default_DISP_TE_GPIO              8       //GPIO number for TE
#define Default_DISP_MODE_SEL             6       //GPIO for panel mode select 0 = Dual Port, 1 = Single Port (DSC)

#define GPIO_EDP_HPD                      10    // eDP HPD gpio
#define GPIO_DP_HPD                       189   // DP HPD gpio
#define GPIO_USB_HPD                      7     // DP Alt-mode HPD gpio

#define GPIO_USB_DP1_SW_EN                188    // usb1 type-c aux switch  enable pin
#define GPIO_USB_DP1_SW_SEL               187    // usb1 type-c aux switch  select pin 
#define GPIO_USB_DP0_FSA_EN               100    // usb1 type-c aux switch  select pin 

/*
 *  Macro definition for PWM desired frequency default
 */
#define MDP_PMIC_PWM_DESIRED_FREQ_DEFAULT_HZ   19200         // Defaulted to 19200 HZ

/* Helper macro for supported panels table entry PanelDTInfoType
 */
#define PANEL_CREATE_ENTRY(_name_,_id_,_dtlable_,_interface_,_cfg_,_timing_,_pll_, _top0_, _top1_, _top2_)     \
                                          {(_name_),                            \
                                           (_id_),                              \
                                           (_dtlable_),                         \
                                           (_interface_),                       \
                                           (_cfg_),                             \
                                           (_timing_),                          \
                                           (_pll_),                             \
                                           {(_top0_), (_top1_), (_top2_)}}

#define PANEL_LIST_LENGTH(_LIST_)         (sizeof(_LIST_)/sizeof((_LIST_)[0]))

/* Macro to do round up of (x / y) to nearest integer
 */
#define DISP_DIV_ROUND_UP(x, y)                  (((x) + (y/2)) / (y))
#define DISP_MAXIMUM_VALUE(bits)                 ((1<<bits)-1)
#define DISP_PERCENT_MAX                         100
#define DISP_CEIL(a,b)                           (((a)+(b-1))/b)
#define DISP_ABS_DELTA(_a_,_b_)                  (((_a_)>(_b_))?((_a_)-(_b_)):((_b_)-(_a_)))

/* Panel command ID list for dynamic panel detection
 */
typedef struct {
  uint8      address[DSI_READ_ADDRESS_SIZE];                                          // DCS command for panel ID
  uint8      expectedReadback[DSI_READ_READBACK_SIZE];                                // expected readback
} PlatformPanelIDCommandInfo;

/* Dynamic panel ID detection entry
 */
typedef struct {
  uint8                         uCmdType;                                             // data type for panel ID DSI read
  uint32                        uTotalRetry;                                          // number of retry if DSI read fails
  PlatformPanelIDCommandInfo    panelIdCommands[PLATFORM_PANEL_ID_MAX_COMMANDS];      // commands for panel ID and expected readback
  uint32                        uLaneRemapOrder;                                      // Lane remap order
  MDPPlatformPanelType          eSelectedPanel;                                       // index of the selected panel
  uint32                        uFlags;                                               // flags to set clock config for now, can set other config in future
} PlatformDSIDetectParams;

/* Panel backlight DCS command
 */
typedef struct {
  uint8      command[DSI_WRITE_BACKLIGHT_SIZE];                                       // DCS command for backlight control
} PlatformPanelBacklightControl;

/* Power configuration
 */
typedef struct
{
  npa_client_handle             sNPAClient[MDP_DISPLAY_MAX];                          // NPA client handles for power control of panels.
  npa_client_handle             sTouchNPAClient;                                      // NPA client handles for power control of Touch.
  npa_client_handle             sEdpNPAClient;                                        // NPA client handle for power control of EDP panel
  MDP_PmicModuleControlType     ePMICSecondaryPower[MDP_DISPLAY_MAX];                 // Configuration for PMIC based secondary power source
  MDPPlatformLABIBBModeType     eOLEDPowerMode;                                       // Swire or spmi mode
  uint32                        uResetGpio;
  bool32                        bBacklightEnabled;
} Panel_PowerCtrlParams;

/* NPA Client info
 */
typedef struct
{
  MDP_Display_IDType            eMapDisplayId;   /* The re-mapped display id for NPA client */ 
  char*                         cResourceName;
  char*                         cClientName;
} NPAClientName;

/* Panel support map type
 */
typedef struct
{
  MDPPlatformPanelType          ePanel;          /* Panel ID */
  int8                         *pPanelXMLConfig; /* Matching XML configuration */
  uint32                        uConfigSize;     /* Size of XML configuration */
  uint32                        uOverrideFlags;  /* Panel config flags from DisplayOverideFlagType */
} PanelMappingType;

/* Panel power / brightness control function type
 */
typedef struct
{
  /* The default panel selected when setting up platform panel configuration.
   */
  MDPPlatformPanelType ePanelSelected; 

  /* The name of the configuration xml file for the panel selected.
   */
  CHAR8               *pPanelXml;

  /* Panel power up function 
   * – Enable all rails, gpios necessary to start driving the panel.
   */
  MDP_Status (*pPanel_PowerUp)            (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);

  /* Panel power down function
   * – Disable all rails, gpios, and backlight to put the peripheral in to the lowest power state.
   */
  MDP_Status (*pPanel_PowerDown)          (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);
  
  /* Panel reset
   * – Toggling the panels reset pin to reset the peripheral back to a idle state.
   */
  MDP_Status (*pPanel_Reset)              (MDPPlatformPanelResetInfo *pResetInfo);

  /* Panel peripheral function (Optional)
   * – Secondary power up to enable any auxiliary power supplies like IBB/LAB bias voltages.
   */
  MDP_Status (*pPanel_Peripheral_Power)   (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams, bool32 bPowerUp);

  /* Panel brightness enable (Optional)
   * – Configuration of the components required to drive the brightness,
   *   but should leave output level as 0% at exit of this function.
   */
  MDP_Status (*pPanel_Brightness_Enable)  (MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);

  /* Panel brightness level (Optional)
   * – Configure the brightness level of the panel.
   */
  MDP_Status (*pPanel_Brightness_Level)   (MDP_Display_IDType eDisplayId, BacklightConfigType *pBacklightConfig);
} MDPPlatformPanelFunctionTable;

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

/* The platform client init common function
 */
MDP_Status PlatformClientInit(MDP_Display_IDType eDisplayId, Panel_PowerCtrlParams *pPowerParams);

/* Aux switch enable function
 */
void DPAuxSwitchEnable(void);

/****************************************************************************
*
** FUNCTION: MDP_CalculatePWMValue()
*/
/*!
* \brief
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
MDP_Status MDP_CalculatePWMValue(uint32 uPwmDesiredFreq, MDP_PMICPWMConfigurationType *pPWMConfiguration, uint32 uPwmBitWidth);

#endif // __MDP_PLATFORM_LIB_PANEL_COMMON__H__
