#ifndef __MDPPLATFORMLIB_H__
#define __MDPPLATFORMLIB_H__
/*=============================================================================
 
  File: MDPPlatformLib.h
 
  Header file for MDP platform functions
  
 
  Copyright (c) 2011-2020 Qualcomm Technologies, Inc.  
  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include <Uefi.h>
#include <Protocol/EFIPlatformInfo.h>
#include <Protocol/EFIChipInfo.h>
#include "MDPTypes.h"

/*===========================================================================
                        RamDUmp Defines
===========================================================================*/
#define RAMDUMP_FRAME_BUFFER_ADDRESS    0x9C000000
#define RAMDUMP_FRAME_BUFFER_SIZE       0x02400000
#define RAMDUMP_ALLOC_POOL_SIZE         0x8000       /* 32k */

/*===========================================================================
                        Defines
===========================================================================*/


/* Bit fields for uFlags in PlatformDSIDetectParams structure. 
    Currently, it is used to define the clock lane config but this can be grown in future.
 */
#define MDPPLATFORM_PANEL_DETECT_FLAG_CLOCK_FORCEHS      0x00000001  /*Force clock lane in HS*/

/* Panel Reset related information
*/

/* Bit Fields for uFlags in the below structure */
#define MDDPLATFORM_PANEL_RESET_FLAG_USE_CUSTOM_DELAY    0x00000001  /* Wait for the specified delay instead of the default after reset */
#define MDDPLATFORM_PANEL_RESET_FLAG_FORCE_RESET         0x00000002  /* Force the panel reset */



/* Display Variable flags (DISPLAYCONFIG environment variable)
 */
#define   DISPLAY_FLAG_AMOLED                            0x80000000  /* Force AMOLED power configuration */

/* Maximum number of panels supported for a chip */
#define   MDPPLATFORM_PANELS_SUPPORTED_MAX               32

/* Maximum length of panel name
*/
#define DISPLAY_PANEL_NAME_MAX_LEN                       64

/* Maximum length of Device Tree tag
*/
#define DISPLAY_PANEL_DT_LABEL_MAX_LEN                   64

/* Maximum number of display topology
*/
#define DISPLAY_MODE_MAX                                 3

/* panel reset timing */
#define MDP_GPIO_RESET_ASSERT_TIME                       10
#define MDP_GPIO_PRE_RESET_TIME                          1000
#define MDP_GPIO_POST_RESET_TIME                         10000

/* Panel override - Stored in a environment variable
 */
#define PANEL_OVERRIDE_STRINGNAME                        L"DISPLAYID"
#define PANEL_OVERRIDE_PANEL_INDEX_ENABLE_MASK           (0x80000000)
#define PANEL_OVERRIDE_PANEL_INDEX(_ID_)                 (~PANEL_OVERRIDE_PANEL_INDEX_ENABLE_MASK & (_ID_))
#define PANEL_OVERRIDE_PANEL_ID_ENABLE_MASK              (0x40000000)
#define PANEL_OVERRIDE_PANEL_ID(_ID_)                    (~PANEL_OVERRIDE_PANEL_ID_ENABLE_MASK & (_ID_))
#define PANEL_OVERRIDE_VALID_PANEL_ID(_ID_)              (((_ID_) & PANEL_OVERRIDE_PANEL_ID_ENABLE_MASK) ? PANEL_OVERRIDE_PANEL_ID((_ID_)) : 0)

/* Bonded mode config - Environment variable mirror of PcdBondedModeConfig
 * Bit(s) [0] : Bonded mode enable flag
 */
#define BONDED_MODE_CONFIG_STRINGNAME                    L"DISPLAYBONDEDMODECONFIG"
#define BONDED_MODE_CONFIG_ENABLE_MASK                   (0x80000000)

/* Supported display list - TRUE: Enabled, FALSE: Disabled. */
extern bool32 gSupportedDisplayList[MDP_DISPLAY_MAX];

#define MDP_DISPLAY_INTERNAL(eDisplayId) (gSupportedDisplayList[eDisplayId])

#define CDP_PLATFORM_SUBTYPE_1_ID                        1

/*===========================================================================
                        Enumerations
===========================================================================*/

typedef enum {
  MDPPLATFORM_PANEL_NONE =0x0,                            /* No panel specified                     */
  MDPPLATFORM_PANEL_SIM_VIDEO_PANEL,                      /* Single DSI video mode simulation panel */
  MDPPLATFORM_PANEL_SIM_DUALDSI_VIDEO_PANEL,              /* Dual DSI video mode simulation panel   */
  MDPPLATFORM_PANEL_SIM_CMD_PANEL,                        /* Single DSI CMD mode simulation panel */
  MDPPLATFORM_PANEL_SIM_DUALDSI_CMD_PANEL,                /* Dual DSI CMD mode simulation panel   */
  MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_CMD,                  /* Truly 2k cmd panel DSC mode single dsi     */
  MDPPLATFORM_PANEL_TRULY_WQXGA_DSC_VIDEO,                /* Truly 2k video panel DSC mode single dsi     */
  MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_CMD,              /* Truly 2k cmd panel non-DSC mode      */
  MDPPLATFORM_PANEL_TRULY_WQXGA_DUALDSI_VIDEO,            /* Truly 2k video panel non-DSC mode      */
  MDPPLATFORM_PANEL_SHARP_4K_DSC_CMD,                     /* NT35950 4k command panel DSC mode      */
  MDPPLATFORM_PANEL_SHARP_4K_DSC_VIDEO,                   /* NT35950 4k video panel DSC mode        */
  MDPPLATFORM_PANEL_SHARP_4K_EDP,                         /* Sharp 4k edp panel */
  MDPPLATFORM_PANEL_SHARP_1080P_CMD,                      /* Sharp 1080P cmd panel */
  MDPPLATFORM_PANEL_TRULY_1080P_CMD,                      /* Truly 1080P cmd panel */
  MDPPLATFORM_PANEL_TRULY_1080P_VIDEO,                    /* Truly 1080P cmd panel */
  MDPPLATFORM_PANEL_BOE_AMOLED_WQHD_DSC_CMD,              /* BOE AMOLED wqhd cmd panel DSC mode single dsi */
  MDPPLATFORM_PANEL_BOE_AMOLED_WQHD_DSC_VIDEO,            /* BOE AMOLED wqhd video panel DSC mode single dsi */
  MDPPLATFORM_PANEL_AUO_1080P_EDP,                        /* AUO 1080P eDP panel */
  MDPPLATFORM_PANEL_SIM_DSC_375_CMD_PANEL,                /* Single DSI CMD mode with DSC 3.75 compression simulation panel */
  MDPPLATFORM_PANEL_SIM_DUAL_DSC_375_CMD_PANEL,           /* Dual DSI CMD mode with DSC 3.75 compression simulation panel   */
  MDPPLATFORM_PRI_PANEL_SHARP_1080P_CMD,                  /* Primary Sharp 1080P cmd panel */
  MDPPLATFORM_SEC_PANEL_TRULY_1080P_CMD,                  /* Secondary Truly 1080P cmd panel */
  MDPPLATFORM_PANEL_NT36850_TRULY_WQHD_CMD,               /* NT36850 Truly wqhd cmd panel */
  MDPPLATFORM_PRI_PANEL_ANX7625_DSI2DP_FHD_VIDEO,         /* Primary Analogix DSI to DP FHD Video Bridge Chip */
  MDPPLATFORM_SEC_PANEL_ANX7625_DSI2DP_FHD_VIDEO,         /* Secondary Analogix DSI to DP FHD Video Bridge Chip */
  MDPPLATFORM_PANEL_MAX
} MDPPlatformPanelType;


/* List of support platform specific configurations 
 */
typedef enum
{
  MDPPLATFORM_CONFIG_INIT           = 0x0,               /* Initialize MDP resources that should be done once */
  MDPPLATFORM_CONFIG_GETPANELCONFIG,                     /* Retrieve panel configuration */
  MDPPLATFORM_CONFIG_POWERUP,                            /* Power up power grid */
  MDPPLATFORM_CONFIG_POWERDOWN,                          /* Power down power grid */
  MDPPLATFORM_CONFIG_GETPANELID,                         /* Return the panel identifier */
  MDPPLATFORM_CONFIG_SETBACKLIGHT,                       /* Set a brightness or backlight level */
  MDPPLATFORM_CONFIG_GETPLATFORMINFO,                    /* Return platform configuration information */
  MDPPLATFORM_CONFIG_SW_RENDERER,                        /* Return SW render mode information */
  MDPPLATFORM_CONFIG_RESETPANEL,                         /* Physical reset of the panel */
  MDPPLATFORM_CONFIG_SETGPIOSTATE,                       /* Set the state of a GPIO */
  MDPPLATFORM_CONFIG_GETPANELLIST,                       /* Get list of supported panels */ 
  MDPPLATFORM_CONFIG_GETPANELDTINFO,                     /* Get list of panels DT Info*/
  MDPPLATFORM_CONFIG_GETPANELSUPPORTFLAGS,               /* Check if a supplied panel is supported in bootloader */
  MDPPLATFORM_CONFIG_BYPASS,                             /* Configuration bypass */
  MDPPLATFORM_CONFIG_GETPANELXMLINFO,                    /* Get a specific panel configuration (xml config) */   
  MDPPLATFORM_CONFIG_MAX
} MDPPlatformConfigType;

/* GPIO Types */
typedef enum
{
  MDPPLATFORM_GPIOTYPE_TLMM = 0x0,                        /* TLMM GPIO */
  MDPPLATFORM_GPIOTYPE_PMIC,                              /* PMIC GPIO */
  MDPPLATFORM_GPIOTYPE_PMI,                               /* PMI GPIO */
} MDPPlatformGPIOType;

typedef enum
{
  DISP_INTF_NONE                          = 0x0,          /* No interface specified */
  DISP_INTF_HDMI                          = 0x1,          /* HDMI */
  DISP_INTF_DSI                           = 0x2,          /* DSI */
  DISP_INTF_DP                            = 0x3           /* DisplayPort */
} DisplayInterfaceType;

/* AMOLED Control Type
 */
typedef enum
{
  MDPPLATFORM_LABIBB_MODE_SWIRE           = 0x0,          /* SWIRE Control(Default) */ 
  MDPPLATFORM_LABIBB_MODE_SPMI                            /* SPMI Control */
} MDPPlatformLABIBBModeType;

/* Power Config Flag
 */
typedef enum
{
  POWERCONFIG_FLAGS_PANEL_POWER           = 0x1,          /* Panel Specific Power */
} PowerConfig_Flags;

/*===========================================================================
                        Structs
===========================================================================*/
/* Platform identification information
 */
typedef struct 
{
   EFI_PLATFORMINFO_PLATFORM_INFO_TYPE  sEFIPlatformType;                   /* [out] EFI Platform information */
   EFIChipInfoIdType                    sEFIChipSetId;                      /* [out] EFI Chipset information */
   EFIChipInfoFamilyType                sEFIChipSetFamily;                  /* [out] EFI Chipset family information */ 
   
   uint32                               uPrimaryPanelId;                    /* [out] Primary panel ID */
   bool32                               bSWRender;                          /* [out] Indicates if the displayDxe is SW renderer only */
   uint32                               uPanelIdOverride;                   /* [out] Panel ID Override configuration */
   bool32                               bSWRenderOverrride;                 /* [in]  Force SW render mode */
   MDP_RotateFlipType                   uPanelOrientationOverride;          /* [out] Panel orientation override */
   bool32                               bPanelInitialized[MDP_DISPLAY_MAX]; /* [out] Indicates if panel configuration was done */
   uint32                               uBondedModeConfig;                  /* [out] Bonded mode configuration flag */
} MDPPlatformInfo;


typedef struct
{
    uint32 uResetGpio;            /* reset gpio pin number */
    uint32 uAssertState;          /* LEVEL_LOW, LEVEL_HIGH, EDGE_LOW2HIGH, EDGE_HIGH2LOW */
    uint32 uPreResetTimeUs;       /* Time from regulator ready to start of reset pin asserted */
    uint32 uResetAssertTimeUs;    /* Time of reset gpio pin asserted */
    uint32 uPostResetTimeUs;      /* Time between reset gpio pin de-asserted, can be extended by InitMaster */
    uint32 uAssertOnPowerDown;    /* assert during power down */
}MDPPlatformPanelResetInfo;

/* Panel XML configuration 
 */
typedef struct
{
    int8                   *pPanelXMLConfig;          /* xml configuration for panel */
    uint32                  uConfigSize;              /* size of the xml configuration data in bytes*/
    uint8                   uDefaultVendor;           /* vendor ID */
    uint8                   uDefaultRevision;         /* revision */ 
    uint32                  uPanelId;                 /* if set, find the panel that matches this panel ID */
    uint32                  uPowerGridId;             /* power grid id */
    MDPPlatformPanelType    eSelectedPanel;           /* selected panel ID */
    uint32                  uSelectedPanelIndex;      /* index to the panel function table */
    bool32                  bDetectPanel;             /* boolean to indicate whether dynamic detection is needed for the panel */
} MDPPlatformPanelInfo;


/* HDMI configuration*/
typedef struct
{
  uint32 uCecDataGpio;
  uint32 uDDCDataGpio;
  uint32 uDDDClkGpio;
  uint32 uHPDGpio;
} HDMI_PinInfo;


/* Backlight configuration */
typedef struct
{
    bool32                          bEnable;               /* Enable/disable the backlight/brightness power grid */
    uint32                          uLevel;                /* New brightness level in percentage, 0-100% */
    uint32                          uNumBits;              /* The bit width to control dcs brightness level, default is 8 for 8 bits */
    MDP_Panel_BacklightType         eBacklightType;        /* Configuration of the backlight interface module */
    uint32                          uPwmDesiredFreqHz;     /* PMIC PWM desired frequency in Hz*/
    union {
        MDP_PmicBacklightControlType  eBacklightCtrl;      /* PMIC module for backlight configuration  */
        MDP_I2C_Configuration         sI2CConfig;          /* I2C  module for backlight configuration  */
    } uBacklightCntrl;

} BacklightConfigType;

/* Power configuration */
typedef struct
{
    MDP_PmicModuleControlType       ePMICSecondaryPower;      /* Configuration for PMIC based secondary power source */
    uint32                          uFlags;                   /* uFlags used to indicate the power type that needs to be powered up / down */
} PowerConfigType;


/* GPIO State */
typedef struct
{
    MDPPlatformGPIOType             eGPIOType;       /* Type of GPIO to be configured */
    uint32                         *pGPIOList;       /* Pointer to an list of GPIO's to be modified */
    uint32                          uListSize;       /* Number of entries in the list */
    uint32                          uState;          /* GPIO state */
} GpioStateType;


/* List of panels supported, sorted by Panel index */
typedef struct
{
  uint32                       uCount;                                     /* Number of panels */
  MDPPlatformPanelType         ePanel[MDPPLATFORM_PANELS_SUPPORTED_MAX];   /* Panel indices */
} PanelListType;

/* Panel support flags for the panel */
typedef struct
{
  MDPPlatformPanelType ePanel;                      /* [in]  Panel index */
  uint32               uModeIndex;                  /* [in]  panel mode index */
  uint32               uFlags;                      /* [out] associated flags */
} PanelSupportType;

/* Panel info structure for the device tree overrides */
typedef struct
{
  CHAR8                         name[DISPLAY_PANEL_NAME_MAX_LEN];        /* Name used by fastboot oem command */
  MDPPlatformPanelType          ePanel;                                  /* Panel ID */
  CHAR8                         DTLabel[DISPLAY_PANEL_DT_LABEL_MAX_LEN]; /* Device tree node name */
  DisplayInterfaceType          eInterface;                              /* Interface type */
  uint32                        uConfigNum;                              /* Topology configuration index */
  uint32                        uTimingNum;                              /* Topology configuration index */
  uint32                        uPLLOverride;                            /* PLL Override*/
  uint32                        uModeFlags[DISPLAY_MODE_MAX];            /* Display operating modes for the panel, refer DisplayModeType */
} PanelDTInfoType;


/* Display port configuration for Type-C */
typedef struct
{
  bool32                     bInvertPlugPolarity;          /* cable plug orientation: only valid for DP over USB-type C */
  bool32                     bReversePhyDataPath;          /* DP PHY lane to pad mapping is reverse of DP alt mode on USB type-C spec */
} DP_InterfaceInfo;


/* Panel information and override struture  */
typedef struct 
{
    uint32   uIndex;                                         /* Panel index to be queried */
    int8    *pPanelXMLConfig;                                /* Panel configuration (raw). */
    uint32   uPanelXMLSize;                                  /* Size of the panel configuration in bytes */
} PanelXmlInfoType;


/* Union of panel properties passed to MDPPlatformConfigure() function
*/
typedef union
{
    MDPPlatformPanelInfo       sPlatformPanel;      /* MDPPLATFORM_CONFIG_GETPANELCONFIG(Primary) */
    HDMI_PinInfo               sHDMIPinConfig;      /* MDPPLATFORM_CONFIG_GETPANELCONFIG(HDMI) */    
    DP_InterfaceInfo           sDPConfig;           /* MDPPLATFORM_CONFIG_GETPANELCONFIG(DP) */
    BacklightConfigType        sBacklightConfig;    /* MDPPLATFORM_CONFIG_SETBACKLIGHT */
    MDPPlatformInfo            sPlatformInfo;       /* MDPPLATFORM_CONFIG_GETPLATFORMINFO */
    MDPPlatformPanelResetInfo  sPlatformPanelReset; /* MDPPLATFORM_CONFIG_RESETPANEL */
    PowerConfigType            sPowerConfig;        /* MDPPLATFORM_CONFIG_POWERUP/MDPPLATFORM_CONFIG_POWERDOWN */
    GpioStateType              sGPIOState;          /* MDPPLATFORM_CONFIG_SETGPIOSTATE */
    PanelListType              sPanelList;          /* MDPPLATFORM_CONFIG_GETPANELLIST */
    PanelDTInfoType           *psPanelDTInfo;       /* MDPPLATFORM_CONFIG_GETPANELDTINFO */
    PanelSupportType           sPanelSupport;       /* MDPPLATFORM_CONFIG_GETPANELSUPPORTFLAGS */
    PanelXmlInfoType           sPanelXMLInfo;       /* MDPPLATFORM_CONFIG_GETPANELXMLINFO */  
} MDPPlatformParams;


/*===========================================================================

                        Public Functions

===========================================================================*/

/* MDP platform specific configuration function 
 */
MDP_Status MDPPlatformConfigure(MDP_Display_IDType eDisplayId, MDPPlatformConfigType eConfig, MDPPlatformParams *pPlatformParams);


#endif // __MDPLIB_H__


