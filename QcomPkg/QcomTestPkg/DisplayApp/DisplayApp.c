/** @file DisplayApp.c
   
  Test application for DisplayDxe graphics output protocol

  Copyright (c) 2010-2018 Qualcomm Technologies, Inc.  
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  
**/


/*=========================================================================
      Include Files
==========================================================================*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EFIDisplayPwrCtrl.h>
#include <Protocol/EFIDisplayPwr.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFIDisplayUtils.h>


/*=========================================================================
   Defines
==========================================================================*/

#define BITMAP_IMAGE_WIDTH                  640
#define BITMAP_IMAGE_HEIGHT                 480

#define MAX_STRING                          64

#define BACKGROUND_R                        0x00
#define BACKGROUND_G                        0x66
#define BACKGROUND_B                        0xCC
#define BACKGROUND_A                        0xFF

#define DEFAULT_WAIT                        (10*16*1000)    // ~10  Frames
#define DEFAULT_LONG_WAIT                   (30*16*1000)    // ~30 Frames

#define PANEL_OVERRIDE_ENABLE_MASK          (0x80000000)
#define PANEL_OVERRIDE_PANELID(_ID_)        (~PANEL_OVERRIDE_ENABLE_MASK & (_ID_))


/*=========================================================================
      Type Definitions
==========================================================================*/

/* Enumeration of display variables
 */
typedef enum {
  DISPLAY_VAR_DISABLEDISPLAY = 0,
  DISPLAY_VAR_DISPLAYID,
  DISPLAY_VAR_MAX,
} EnvVariables;

/* Display Variable flags 
 */
#define   DISPLAY_FLAG_AMOLED         0x80000000
#define   DISPLAY_FLAG_PANELOVERRIDE  0x40000000

/* Test case entry 
 */
typedef struct {
  CHAR8        sTestName[MAX_STRING];
  EFI_STATUS (*pTestFn)(void);
} TestCaseEntry;


/* Environment variable list
 */
typedef struct {
  CHAR16      sVarName[MAX_STRING];
  UINTN       nVarSize;
} EnvVarEntry;


/* Clock list entry
 */
typedef struct {
  CHAR8       sClockName[MAX_STRING];
} ClockListEntry;

/*=========================================================================
     Local Function Declarations
==========================================================================*/

/* Display mode information test */
static EFI_STATUS DisplayInfoTest(void);
/* Blt operation test */
static EFI_STATUS DisplayBltOperationTest(void);
/* Blt Performance test */
static EFI_STATUS DisplayBltPerformanceTest(void);
/* Backlight brightness control test */
static EFI_STATUS DisplayBacklightBrightnessLevelControlTest(void);
/* Backlight brightness status test */
static EFI_STATUS DisplayBacklightBrightnessLevelStatusTest(void);
/* Display panel power control test */
static EFI_STATUS DisplayPanelPowerControlTest(void);
/* Display panel power status test */
static EFI_STATUS DisplayPanelPowerStatusTest(void);
/* Display panel power state test */
static EFI_STATUS DisplayPanelPowerStateTest(void);
/* Display internal Panel set mode test */
static EFI_STATUS DisplaySetModeTest(void);
/* Display external Panel set mode test */
static EFI_STATUS DisplaySetModeExtTest(void);


/* Local helper functions */

/* Output display app help menu */
static void DisplayAppHelp(void);
/* Display BVT */
EFI_STATUS DisplayBVT(UINT32 uTestNumber);
/* Commandline parser helper */
static UINTN DisplayParseCommand(CHAR8 *pCommandLine, CHAR8 *pParam, CHAR8 *pValue);
/* Clock Status helper */
static void DisplayClockStatus(CHAR8 *pClockName);
/* Clock dumper helper */
static void DisplayDumpClocks();
/* Display Panel List Utility */
static UINT32 DisplayListPanels(uint32 uCurrentIndex, UINTN bShow);
/* Dummy event for power event test */
static VOID DummyEventHandle(EFI_EVENT  Event, VOID *Context);
/* Panel power state changes */
static EFI_STATUS PowerState(UINT32 uPowerState);
/* Mode Set helper */
static EFI_STATUS SetMode(UINT32 uModeIndex);
/*  Set an environment variable */
static EFI_STATUS DisplaySetEnvVar(const CHAR16 *pVarname, UINT32 uValue, UINTN uSize);
/*  Get an environment variable */
static EFI_STATUS DisplayGetEnvVar(const CHAR16 *pVarname, UINT32 *pValue, UINTN uSize);
/* Event based Display power control test */
static EFI_STATUS DisplayPowerEventTest(void);
/* Generate a checkerboard pattern bitmap */
static EFI_STATUS DisplayGenerateBitmap(void);



/*=========================================================================
      Globals
==========================================================================*/

extern EFI_GUID gQcomTokenSpaceGuid;
void           *pBitmapImage = NULL;

/* Total number of test cases */
#define NUM_TEST_CASES        (sizeof(TestCaseList)/sizeof(TestCaseEntry))

/* List of display test cases 
 */
TestCaseEntry TestCaseList[] = 
{
  {"Display Info",       &DisplayInfoTest},
  {"Blt Performance",    &DisplayBltPerformanceTest},
  {"Blt Operation",      &DisplayBltOperationTest},
  {"Power Control",      &DisplayPanelPowerControlTest},
  {"Power State",        &DisplayPanelPowerStateTest},
  {"Power Status",       &DisplayPanelPowerStatusTest},
  {"Brightness Control", &DisplayBacklightBrightnessLevelControlTest},  
  {"Brightness Status",  &DisplayBacklightBrightnessLevelStatusTest},  
  {"Power Event",        &DisplayPowerEventTest},
  {"External Display",   &DisplaySetModeExtTest},
  {"Mode Enumeration",   &DisplaySetModeTest}
};

/* List of display environment variables
 */
EnvVarEntry EnvVarList[] =
{
  {L"DISABLEDISPLAY", sizeof(UINT8)},     // DISPLAY_VAR_DISABLEDISPLAY
  {L"DISPLAYID",      sizeof(UINT32)},    // DISPLAY_VAR_DISPLAYID  
};  


/* Total number of clock entries */
#define NUM_CLOCK_ENTRIES         (sizeof(ClockList)/sizeof(ClockListEntry))    

/* List of all mdss clocks 
 */
ClockListEntry ClockList[] = 
{
   {"gcc_disp_ahb_clk"},
   {"disp_cc_mdss_ahb_clk"},
   {"disp_cc_mdss_mdp_clk"},
   {"disp_cc_mdss_vsync_clk"},
   {"disp_cc_mdss_esc0_clk"},
   {"disp_cc_mdss_byte0_clk"},
   {"disp_cc_mdss_byte0_intf_clk"},
   {"disp_cc_mdss_pclk0_clk"},
   {"disp_cc_mdss_esc1_clk"},   
   {"disp_cc_mdss_byte1_clk"},
   {"disp_cc_mdss_byte1_intf_clk"},
   {"disp_cc_mdss_pclk1_clk"},
};

/*=========================================================================
     Global Functions
==========================================================================*/

/**
  The user entry point for the display application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
DisplayAppMain (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS   eStatus  = EFI_INVALID_PARAMETER;
  UINTN        Argc     = 0;
  UINTN        HasValue = 0;
  CHAR8      **Argv;
  CHAR8        Param[MAX_STRING], Value[MAX_STRING];

  AsciiPrint("DisplayApp (Compiled on: %a)\n\n", __DATE__);
  
  // Get the command line arguments
  if ((EFI_SUCCESS == GetCmdLineArgs (ImageHandle, &Argc, &Argv)) &&
      (Argc > 0))
  {
    // Parse the arguments
    HasValue = DisplayParseCommand((CHAR8*)Argv[0], (CHAR8*)&Param, (CHAR8*)&Value);

    // If we have at least one parameter
    if (HasValue)
    {
      // Command line passed -<Param>:<Value>, we really just want to know if <Value> was passed for the function below so subtract the number of
      // parased commands in the command line.
      HasValue--;
      eStatus = EFI_SUCCESS;
    }
  }

  // Print help on failure
  if (EFI_SUCCESS != eStatus)
  {
    DisplayAppHelp();
    return 0;
  }


  // Execute the BVT
  if (0 == AsciiStrCmp("bvt", Param))
  {
    UINT32 uTestCase = AsciiStrDecimalToUintn(Value);
    
    // Execute the BVT
    DisplayBVT(uTestCase);
  }
  else if (0==AsciiStriCmp("disabledisplay", Param))
  {
      UINTN uValue = AsciiStrDecimalToUintn(Value);  

      // Configure or display the environment variable
      if (EFI_SUCCESS != DisplaySetEnvVar((CHAR16*)&EnvVarList[DISPLAY_VAR_DISABLEDISPLAY].sVarName, uValue, EnvVarList[DISPLAY_VAR_DISABLEDISPLAY].nVarSize))
      {
         AsciiPrint("Error: Failed to set environment variable for disabledisplay.\n");
      }
  }
  else if (0==AsciiStriCmp("setmode", Param))
  {
      UINTN uValue = AsciiStrDecimalToUintn(Value);  

      // Set a specific display mode
      SetMode(uValue);
  }
  else if (0==AsciiStriCmp("setpower", Param))
  {
      UINTN uValue = AsciiStrDecimalToUintn(Value);  

      // Set a display power state
      PowerState(uValue);
  }
  else if (0==AsciiStriCmp("panelid", Param))
  {
      UINTN  bNext       = (0==AsciiStriCmp("next", Value))?1:0;
      UINTN  uValue      = AsciiStrHexToUintn(Value);
      UINT32 nNumPanels  = 0;
      UINTN  uPanelIndex = 0;
      UINT32 uPanelId    = 0;

      // Read the current Panel ID
      if (EFI_SUCCESS == DisplayGetEnvVar((CHAR16*)&EnvVarList[DISPLAY_VAR_DISPLAYID].sVarName, &uPanelId, EnvVarList[DISPLAY_VAR_DISPLAYID].nVarSize))
      {
        // Ensure the enable bit is set
        if (uPanelId & PANEL_OVERRIDE_ENABLE_MASK)
        {
            // Offset the ID by 1 so we report 0 as the default (no override) correctly
            uPanelIndex = PANEL_OVERRIDE_PANELID(uPanelId) + 1;
        }
      }

      // Update the variable if there is a request to change it.
      if (HasValue)
      {
         // Query the list of valid panels
         nNumPanels = DisplayListPanels(uPanelIndex, 0);
      
         // Relative adjustment, go to the next panel
         if (uValue<=nNumPanels)
         {

           // Request is to just increment the selection (i.e next)
           if (bNext)
           {
             // Clear the flag, and go to th next ID.
             uPanelId =  PANEL_OVERRIDE_PANELID(uPanelId); 
             uPanelId++;
  
             // Wrap around
             if (uPanelId >= nNumPanels)
             {
               uPanelId = 0x0;
             }
  
             // Reapply the mask
             uPanelId = (PANEL_OVERRIDE_ENABLE_MASK | uPanelId); 
           }
           else 
           {
             // Absolute adjustment, go a specific index
             uPanelId = (0==uValue)?0:(PANEL_OVERRIDE_ENABLE_MASK | (uValue-1));
           }
           
           // Configure or display the environment variable, 0 has a special meaning which is auto detect (no variable set)
           if (EFI_SUCCESS == DisplaySetEnvVar((CHAR16*)&EnvVarList[DISPLAY_VAR_DISPLAYID].sVarName, uPanelId, EnvVarList[DISPLAY_VAR_DISPLAYID].nVarSize))
           {
              // Update with the new panel id and show the updated table, no override means auto-detect
              if (uPanelId & PANEL_OVERRIDE_ENABLE_MASK)
              {
                uPanelIndex = PANEL_OVERRIDE_PANELID(uPanelId) + 1;
              }
              else
              {
                uPanelIndex = 0;
              }
              
              DisplayListPanels(uPanelIndex, 1);
           }
           else
           {
             AsciiPrint("Error: Failed to set environment variable for panelid.\n");
           }
        }   
        else
        {
             AsciiPrint("Error: Invalid panel selection (%d).\n", uValue);        
        }
      }
      else
      {
          // Show the list of panels, including the selected one.
          nNumPanels = DisplayListPanels(uPanelIndex, 1);
      }

  }
  else
  {
    AsciiPrint("Error: %a is an invalid parameter\n", Param);
  }


  return eStatus;
}

/*=========================================================================
     Local Functions
==========================================================================*/


/* Backlight brightness level control test */
EFI_STATUS DisplayBVT(UINT32 uTestNumber)
{
  EFI_STATUS eStatus     = EFI_SUCCESS;
  UINT32     uTestStart  = 0;    
  UINT32     uTestEnd    = NUM_TEST_CASES;
  UINT32     uTestCount  = 0;
  UINT32     uTestPassed = 0;
  UINT32     uCount;  
  
  eStatus = DisplayGenerateBitmap();
  
  if (EFI_SUCCESS != eStatus)
  {
    AsciiPrint("Unable to generate bitmap, cannot continue!\n\n");
    return eStatus;
  }

  if ((uTestNumber > 0) &&
      (uTestNumber <= NUM_TEST_CASES))
  {
    uTestStart = uTestNumber-1;
    uTestEnd   = uTestStart+1;
  }

  // Loop all tests
  for (uCount=uTestStart;uCount<uTestEnd;uCount++)
  {
    AsciiPrint("======] Test Case Start   : %a\n\n", TestCaseList[uCount].sTestName);

    // Increment the number of tests run
    uTestCount++;
    if (EFI_SUCCESS == (eStatus = TestCaseList[uCount].pTestFn()))
    {
      // Increment the test pass count
      uTestPassed++;
    }

    AsciiPrint("\n======] Test Case Complete: %a\n", (EFI_SUCCESS==eStatus)?"Passed":"Failed");
  }

  if (uTestCount)
  {
    AsciiPrint("\n\nDisplay BVT Result: %d tests run (%d%% pass rate)\n", uTestCount, (100*uTestPassed)/uTestCount);
  }

  // Free the image buffer
  if (NULL != pBitmapImage)
  {
    gBS->FreePool (pBitmapImage);    
  }

  return eStatus;
}


/* Display info and mode enumeration  */
static EFI_STATUS DisplayInfoTest(void)
{
  EFI_STATUS                     eStatus        = EFI_SUCCESS;
  EFI_GUID                       sGfxOutputGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGfxProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&sGfxOutputGUID, 
                                                     NULL, 
                                                     (VOID **)&pGfxProtocol)))                     
  {
    AsciiPrint ("DisplayInfoTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *pModeInfo;
    UINTN                                   nModeInfoSize = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
    UINT32                                  mode = 0;

    AsciiPrint("Available Display Modes:\n\n");
    do {    
      if (EFI_SUCCESS == (eStatus = pGfxProtocol->QueryMode(pGfxProtocol, mode, &nModeInfoSize, &pModeInfo)))
      {
         AsciiPrint("%d] %dx%d (Format:%d)\n", mode, pModeInfo->HorizontalResolution, pModeInfo->VerticalResolution, (UINT32)pModeInfo->PixelFormat);
         mode++;
      }
    } while (EFI_SUCCESS == eStatus);

    // Succes if we have more than one mode 
    if (mode>0)
    {
      eStatus = EFI_SUCCESS;
    }
    AsciiPrint("\nTotal number of modes enumerated: %d\n\n", mode);

    if (NULL != pGfxProtocol->Mode)
    {
        AsciiPrint("DisplayDxe Mode Information:\n");
        AsciiPrint("  Number of modes exposed: %d\n", pGfxProtocol->Mode->MaxMode);
        AsciiPrint("  Current mode index     : %d\n", pGfxProtocol->Mode->Mode);
        if (NULL != pGfxProtocol->Mode->Info)
        {
          AsciiPrint("  Current mode details   : %dx%d (Format:%d)\n", pGfxProtocol->Mode->Info->HorizontalResolution, pGfxProtocol->Mode->Info->VerticalResolution, (UINT32)pGfxProtocol->Mode->Info->PixelFormat);
        }
        else
        {
          AsciiPrint("  Current mode details   : Unavailable\n");
        }
        AsciiPrint("  Frame buffer address   : 0x%08x\n", (UINT32) pGfxProtocol->Mode->FrameBufferBase);
        AsciiPrint("  Frame buffer size      : %d bytes\n", pGfxProtocol->Mode->FrameBufferSize);
    }
    else
    {
      AsciiPrint("Error: Current mode not set!\n");
    }
  }

  return eStatus;
}

/* Backlight brightness level control test */
static EFI_STATUS DisplayBacklightBrightnessLevelControlTest(void)
{
  EFI_STATUS                           eStatus = EFI_SUCCESS;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL  *pDisplayPwrCtrlProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&gQcomDisplayPwrCtrlProtocolGuid, 
                                                     NULL, 
                                                     (VOID **)&pDisplayPwrCtrlProtocol)))                     
  {
    AsciiPrint ("DisplayBacklightBrightnessLevelControlTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
    UINT32            uBackLightLevel = 0;
    EFI_DISPLAY_TYPE  eDisplayType    = EFI_DISPLAY_TYPE_PRIMARY;
    
    AsciiPrint ("DisplayBacklightBrightnessLevelControlTest: Decrease the display backlight...\n");
    /* Decrease the backlight gradually */ 
    for (uBackLightLevel = 100; uBackLightLevel > 0; uBackLightLevel--)
    {
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayBacklightBrightnessLevelControl(eDisplayType, uBackLightLevel)))
      {
        AsciiPrint ("DisplayBacklightBrightnessLevelControlTest: DisplayBacklightBrightnessLevelControl() failed.\n");
        return eStatus;
      }   
      gBS->Stall(DEFAULT_WAIT); 
    }
    AsciiPrint ("DisplayBacklightBrightnessLevelControlTest: Increase the display backlight...\n");
    /* Increase the backlight gradually */ 
    for (uBackLightLevel = 0; uBackLightLevel <= 100; uBackLightLevel++)
    {
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayBacklightBrightnessLevelControl(eDisplayType, uBackLightLevel)))
      {
        AsciiPrint ("DisplayBacklightBrightnessLevelControlTest: DisplayBacklightBrightnessLevelControl() failed.\n");
        return eStatus;
      }
      gBS->Stall(DEFAULT_WAIT);
    }         
  }
  return eStatus;
}

/* Backlight brightness status test */
static EFI_STATUS DisplayBacklightBrightnessLevelStatusTest(void)
{
  EFI_STATUS                           eStatus = EFI_SUCCESS;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL  *pDisplayPwrCtrlProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&gQcomDisplayPwrCtrlProtocolGuid, 
                                                     NULL, 
                                                     (VOID **)&pDisplayPwrCtrlProtocol)))                     
  {
    AsciiPrint ("DisplayBacklightBrightnessLevelStatusTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
    UINT32            uRetBackLightLevel  = 0;
    UINT32            uBackLightLevel     = 0;
    EFI_DISPLAY_TYPE  eDisplayType        = EFI_DISPLAY_TYPE_PRIMARY;
    
    AsciiPrint ("DisplayBacklightBrightnessLevelStatusTest: Decrease the display backlight...\n");
    /* Decrease the backlight gradually */ 
    for (uBackLightLevel = 0; uBackLightLevel <= 100; uBackLightLevel++)
    {
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayBacklightBrightnessLevelControl(eDisplayType, uBackLightLevel)))
      {
        AsciiPrint ("DisplayBacklightBrightnessLevelStatusTest: DisplayBacklightBrightnessLevelControl() failed.\n");
        return eStatus;
      }
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayBacklightBrightnessLevelStatus(eDisplayType, &uRetBackLightLevel)))
      {
        AsciiPrint ("DisplayBacklightBrightnessLevelStatusTest: DisplayBackLightBrightnessLevelStatus() failed.\n");
        return eStatus;
      }
      AsciiPrint ("DisplayBacklightBrightnessLevelStatusTest: Backlight level - %d\n", uRetBackLightLevel);
      gBS->Stall(DEFAULT_WAIT); 
    }
  }
  return eStatus;
}

/* Panel power control test */
static EFI_STATUS DisplayPanelPowerControlTest(void)
{
  EFI_STATUS                           eStatus = EFI_SUCCESS;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL  *pDisplayPwrCtrlProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&gQcomDisplayPwrCtrlProtocolGuid, 
                                                     NULL, 
                                                     (VOID **)&pDisplayPwrCtrlProtocol)))                     
  {
    AsciiPrint ("DisplayPanelPowerControlTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
    UINT32                         cntr = 0;
    EFI_DISPLAY_TYPE               eDisplayType = EFI_DISPLAY_TYPE_PRIMARY;
    EFI_DISPLAY_POWER_CTRL_STATE   ePowerState  = EFI_DISPLAY_POWER_STATE_OFF;    
    
    for (cntr = 0; cntr < 2; cntr++)
    {
      AsciiPrint ("DisplayPanelPowerControlTest: Display panel power off...\n");
      /* Turn off the display */
      ePowerState  = EFI_DISPLAY_POWER_STATE_OFF;
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayPanelPowerControl(eDisplayType, ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerControlTest: DisplayPanelPowerControl() failed.\n");
        return eStatus;
      } 
      gBS->Stall(DEFAULT_LONG_WAIT);
    
      /* Dump Clock Status */
      DisplayDumpClocks();
    
      AsciiPrint ("DisplayPanelPowerControlTest: Display panel power on...\n");
      /* Turn on the display */
      ePowerState  = EFI_DISPLAY_POWER_STATE_ON;    
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayPanelPowerControl(eDisplayType, ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerControlTest: DisplayPanelPowerControl() failed.\n");
        return eStatus;
      }
      gBS->Stall(DEFAULT_LONG_WAIT);
    
      /* Dump Clock Status */
      DisplayDumpClocks();
    }
  }
  return eStatus;
}

/* Display panel power status test */
static EFI_STATUS DisplayPanelPowerStatusTest(void)
{
  EFI_STATUS                           eStatus = EFI_SUCCESS;
  EFI_QCOM_DISPLAY_PWR_CTRL_PROTOCOL  *pDisplayPwrCtrlProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&gQcomDisplayPwrCtrlProtocolGuid, 
                                                     NULL, 
                                                     (VOID **)&pDisplayPwrCtrlProtocol)))                     
  {
    AsciiPrint ("DisplayPanelPowerStatusTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
    UINT32                         cntr            = 0;
    EFI_DISPLAY_POWER_CTRL_STATE   uPanelPwrState  = (EFI_DISPLAY_POWER_CTRL_STATE)0;
    EFI_DISPLAY_TYPE               eDisplayType    = EFI_DISPLAY_TYPE_PRIMARY;
    EFI_DISPLAY_POWER_CTRL_STATE   ePowerState     = EFI_DISPLAY_POWER_STATE_OFF;    
    
    for (cntr = 0; cntr < 3; cntr++)
    {
      /* Turn off the display */
      ePowerState  = EFI_DISPLAY_POWER_STATE_OFF;
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayPanelPowerControl(eDisplayType, ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerStatusTest: DisplayPanelPowerControl() failed.\n");
        return eStatus;
      } 
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayPanelPowerStatus(eDisplayType, &uPanelPwrState)))
      {
        AsciiPrint ("DisplayPanelPowerStatusTest: DisplayPanelPowerStatus() failed.\n");
        return eStatus;
      } 
      AsciiPrint ("DisplayPanelPowerStatusTest: Display panel power state - %d\n", uPanelPwrState ? 1 : 0);
      gBS->Stall(DEFAULT_LONG_WAIT);

      /* Dump Clock Status */
      DisplayDumpClocks();    
    
      /* Turn on the display */
      ePowerState  = EFI_DISPLAY_POWER_STATE_ON;    
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayPanelPowerControl(eDisplayType, ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerStatusTest: DisplayPanelPowerControl() failed.\n");
        return eStatus;
      }
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->DisplayPanelPowerStatus(eDisplayType, &uPanelPwrState)))
      {
        AsciiPrint ("DisplayPanelPowerStatusTest: DisplayPanelPowerStatus() failed.\n");
        return eStatus;
      } 
      AsciiPrint ("DisplayPanelPowerStatusTest: Display panel power state - %d\n", uPanelPwrState ? 1 : 0);
      gBS->Stall(DEFAULT_LONG_WAIT);
    
      /* Dump Clock Status */
      DisplayDumpClocks();    
    }
  }
  return eStatus;
}

/* BLT operation test */
static EFI_STATUS DisplayBltOperationTest(void)
{
  EFI_STATUS                     eStatus        = EFI_SUCCESS;
  EFI_GUID                       sGfxOutputGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGfxProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&sGfxOutputGUID, 
                                                     NULL, 
                                                     (VOID **)&pGfxProtocol)))                     
  {
    AsciiPrint ("DisplayBltOperationTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *pModeInfo;
    UINTN                                   nModeInfoSize;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL           bgPixel;
    UINTN                                   centerX;
    UINTN                                   centerY;
    UINT32                                  mode = 0;
    
    if (EFI_SUCCESS != (eStatus = pGfxProtocol->SetMode(pGfxProtocol, mode)))
    {
      AsciiPrint ("DisplayBltOperationTest: SetMode() failed.\n");
      return eStatus;
    }
    AsciiPrint ("DisplayBltOperationTest: Set mode - %d\n", mode);
    if (EFI_SUCCESS != (eStatus = pGfxProtocol->QueryMode(pGfxProtocol, 0, &nModeInfoSize, &pModeInfo)))
    {
      AsciiPrint ("DisplayBltOperationTest: QueryMode() failed.\n");
      return eStatus;
    }
    AsciiPrint ("DisplayBltOperationTest: Query mode - %dx%d\n", pModeInfo->HorizontalResolution, pModeInfo->VerticalResolution);
    /* Fill background as Blue */
    bgPixel.Blue     = BACKGROUND_B;
    bgPixel.Green    = BACKGROUND_G;
    bgPixel.Red      = BACKGROUND_R;
    bgPixel.Reserved = BACKGROUND_A;
    
    AsciiPrint ("DisplayBltOperationTest: Blt(EfiBltVideoFill)\n");
    if (EFI_SUCCESS != (eStatus = pGfxProtocol->Blt(pGfxProtocol, 
                                                    (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)&bgPixel,
                                                    EfiBltVideoFill,
                                                    0, 0,
                                                    0, 0, 
                                                    pModeInfo->HorizontalResolution,
                                                    pModeInfo->VerticalResolution,
                                                    0)))
    {
      AsciiPrint ("DisplayBltOperationTest: Blt(EfiBltVideoFill) failed.\n");
      return eStatus;
    }
    gBS->Stall(DEFAULT_LONG_WAIT);
    /* Copy image to center */
    centerX = (pModeInfo->HorizontalResolution>>1) - (BITMAP_IMAGE_WIDTH>>1);
    centerY = (pModeInfo->VerticalResolution>>1) - (BITMAP_IMAGE_HEIGHT>>1);
    AsciiPrint ("DisplayBltOperationTest: Blt(EfiBltBufferToVideo)\n");
    if (EFI_SUCCESS != (eStatus = pGfxProtocol->Blt(pGfxProtocol, 
                                                    (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)pBitmapImage,
                                                    EfiBltBufferToVideo,
                                                    0, 0,
                                                    centerX, centerY,
                                                    BITMAP_IMAGE_WIDTH,
                                                    BITMAP_IMAGE_HEIGHT,
                                                    0)))
    {
      AsciiPrint ("DisplayBltOperationTest: Blt(EfiBltBufferToVideo) failed.\n");
      return eStatus;
    }
    gBS->Stall(DEFAULT_LONG_WAIT);
    /* Copy image to top left */
    AsciiPrint ("DisplayBltOperationTest: Blt(EfiBltVideoToVideo)\n");
    if (EFI_SUCCESS != (eStatus = pGfxProtocol->Blt(pGfxProtocol, 
                                                    NULL,
                                                    EfiBltVideoToVideo,
                                                    centerX, centerY,
                                                    0, 0,                      
                                                    BITMAP_IMAGE_WIDTH,
                                                    BITMAP_IMAGE_HEIGHT,
                                                    0)))
    {
      AsciiPrint ("DisplayBltOperationTest: Blt(EfiBltVideoToVideo) failed.\n");
      return eStatus;
    }
    gBS->Stall(DEFAULT_LONG_WAIT);
  }
  return eStatus; 
}
  
/* Panel power state test */
static EFI_STATUS DisplayPanelPowerStateTest(void)
{
  EFI_STATUS                           eStatus = EFI_SUCCESS;
  EFI_DISPLAY_POWER_PROTOCOL          *pDisplayPwrCtrlProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&gEfiDisplayPowerStateProtocolGuid, 
                                                     NULL, 
                                                     (VOID **)&pDisplayPwrCtrlProtocol)))                     
  {
    AsciiPrint ("DisplayPanelPowerStateTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
    UINT32                         cntr = 0;
    EFI_DISPLAY_POWER_STATE        ePowerState  = EfiDisplayPowerStateOff;    
    
    for (cntr = 0; cntr < 2; cntr++)
    {
      AsciiPrint ("DisplayPanelPowerStateTest: Display panel power off...\n");
      /* Turn off the display */
      ePowerState  = EfiDisplayPowerStateOff;
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->SetDisplayPowerState(NULL, ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerStateTest: SetDisplayPowerState() failed.\n");
        return eStatus;
      } 
      /* Retrieve the power status */ 
      ePowerState = EfiDisplayPowerStateUnknown;
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->GetDisplayPowerState(NULL, &ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerStateTest: GetDisplayPowerState() failed.\n");
        return eStatus;
      } 
      AsciiPrint ("DisplayPanelPowerStateTest: Display panel power status : %d\n", ePowerState);
      gBS->Stall(DEFAULT_LONG_WAIT);
      AsciiPrint ("DisplayPanelPowerStateTest: Display panel power on...\n");
      /* Turn on the display and backlight */
      ePowerState  = EfiDisplayPowerStateMaximum;   
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->SetDisplayPowerState(NULL, ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerStateTest: SetDisplayPowerState() failed.\n");
        return eStatus;
      }
      /* Retrieve the power status */ 
      ePowerState = EfiDisplayPowerStateUnknown;
      if (EFI_SUCCESS != (eStatus = pDisplayPwrCtrlProtocol->GetDisplayPowerState(NULL, &ePowerState)))
      {
        AsciiPrint ("DisplayPanelPowerStateTest: GetDisplayPowerState() failed.\n");
        return eStatus;
      } 
      AsciiPrint ("DisplayPanelPowerStateTest: Display panel power status : %d\n", ePowerState);
      gBS->Stall(DEFAULT_LONG_WAIT);
    }
  }
  return eStatus;
}


/* Panel power state changes */
static EFI_STATUS PowerState(UINT32 uPowerState)
{
  EFI_STATUS                           eStatus = EFI_SUCCESS;
  EFI_DISPLAY_POWER_PROTOCOL          *pDisplayPwrCtrlProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&gEfiDisplayPowerStateProtocolGuid, 
                                                     NULL, 
                                                     (VOID **)&pDisplayPwrCtrlProtocol)))                     
  {
    AsciiPrint ("PowerState: LocateProtocol() failed.\n");
    return eStatus;
  }
  else 
  {
     EFI_DISPLAY_POWER_STATE ePowerState  = (uPowerState==0)?EfiDisplayPowerStateOff:EfiDisplayPowerStateMaximum;

     if (EFI_SUCCESS == (eStatus = pDisplayPwrCtrlProtocol->SetDisplayPowerState(NULL, ePowerState)))
     {
        AsciiPrint("PowerState: Successfully set power state to %d.\n", ePowerState);
     }
     else
     {
        AsciiPrint("PowerState: Failed (%d).\n", eStatus);
     }

  }

  return eStatus;
}


/* DisplaySetEnvVar()
    Set a generic display variable
 */
static EFI_STATUS DisplaySetEnvVar(const CHAR16 *pVarname, UINT32 uValue, UINTN uSize)
{
  EFI_STATUS    eStatus     = EFI_SUCCESS;
 
  // Set value    
  eStatus = gRT->SetVariable((CHAR16*)pVarname,
                             &gQcomTokenSpaceGuid,
                             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                             uSize,
                             (void*)&uValue);
  return eStatus;
}


/* DisplayGetEnvVar()
    Get a generic display variable
 */
static EFI_STATUS DisplayGetEnvVar(const CHAR16 *pVarname, UINT32 *pValue, UINTN uSize)
{
  EFI_STATUS eStatus      = EFI_SUCCESS;
  UINT32     uAttributes  = 0;
  UINTN      uStorageSize = uSize;
    
  // Query value
  eStatus = gRT->GetVariable((CHAR16*)pVarname, 
                             &gQcomTokenSpaceGuid,
                             &uAttributes,
                             (UINTN*)&uStorageSize,
                             (void*)pValue);

  return eStatus;
}


/** 
 Helper to print results in simplified units 
 **/
static void DisplayBltPrintResults(CHAR8 *testName, UINT64 byteProcessed, UINT32 totalTime, UINT32 blockSize)
{
    UINT64 throughput; 
    
    if (byteProcessed >= (10*1024))  
    { 
      throughput = byteProcessed / 1024;
      AsciiPrint("  %-20a:%8dKB/s (blocksize: %d, test duration: %dms)\n", testName, throughput, blockSize, totalTime);
    }
    else
    {
      AsciiPrint("  %-20a:%8dBytes/s (blocksize: %d, test duration: %dms)\n", testName, byteProcessed, blockSize, totalTime);
    }
}

/**
Performance test for Blt operation. 5KB, 100KB and 1MB blocks of data are copied from Buffer to Video, Video to Video and Blt Fill.

**/
static EFI_STATUS DisplayBltPerformanceTest(void)
{
  EFI_STATUS                     eStatus        = EFI_SUCCESS;
  UINT32                         uStartTime;
  UINT32                         uEndTime;
  UINT32                         uTotalFuncTime = 0; 
  UINT64                         perfno;  
  EFI_GUID                       sGfxOutputGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGfxProtocol;
  
  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol(&sGfxOutputGUID,
      NULL,
      (VOID **)&pGfxProtocol)))
  {
      AsciiPrint("DisplayBltOperationTest: LocateProtocol() failed.\n");
  }
  else
  {
      EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *pModeInfo;
      UINTN                                   nModeInfoSize;
      EFI_GRAPHICS_OUTPUT_BLT_PIXEL           bgPixel;
      UINT32                                 *BltBuffer = NULL;
      UINTN                                   BufferLen;
      UINT32                                  mode = 0;
      UINT32                                  inplace;

      if (EFI_SUCCESS != (eStatus = pGfxProtocol->SetMode(pGfxProtocol, mode)))
      {
          AsciiPrint("DisplayBltOperationTest: SetMode() failed.\n");
      }
      else if (EFI_SUCCESS != (eStatus = pGfxProtocol->QueryMode(pGfxProtocol, 0, &nModeInfoSize, &pModeInfo)))
      {
          AsciiPrint("DisplayBltOperationTest: QueryMode() failed.\n");
      }
      else
      {
          // Pixel sizes to execute (each operation is 4 bytes/32bpp)
          UINT32 blocksize[]      = { 1, 64, 256, 512, 1024 };
          // Test loop times of items from blocksize[]
          UINT32 iterationArray[] = { 1000 * 1000, 200 * 1000, 50 * 1000, 20 * 1000, 10 * 1000 };
          UINT32 blockLength      = sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
          UINT32 verti            = 1;
          UINT32 loopv;
          UINT32 yPos;
          UINT32 iter;

          for (inplace = 0; inplace < 2; inplace++)
          {
              AsciiPrint("\nPerformance numbers %a: \n\n", (0 == inplace) ? "incrementing addresses" : "in-place addresses");

              
              AsciiPrint("\nVideo Buffer fill (EfiBltVideoFill)\n");
              
              for (iter = 0; iter < (sizeof(blocksize) / sizeof(blocksize[0])); iter++)
              {
                  UINT32 hori = blocksize[iter];

                  // Limit the block size to the horizontal resolution
                  if (blocksize[iter] > pModeInfo->HorizontalResolution)
                  {
                      blocksize[iter] = pModeInfo->HorizontalResolution;
                  }

                  /* Fill background as Blue */
                  bgPixel.Blue     = BACKGROUND_B;
                  bgPixel.Green    = BACKGROUND_G;
                  bgPixel.Red      = BACKGROUND_R;
                  bgPixel.Reserved = BACKGROUND_A;


                  // Start Time   
                  yPos = 0;
                  uStartTime = GetTimerCountms();
                  for (loopv = 0; loopv < iterationArray[iter]; loopv++)
                  {
                      if (EFI_SUCCESS != (eStatus = pGfxProtocol->Blt(pGfxProtocol,
                                                                      (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)&bgPixel,
                                                                      EfiBltVideoFill,
                                                                      0, 0,
                                                                      0, yPos,
                                                                      hori,
                                                                      verti,
                                                                      0)))
                      {
                          break;
                      }

                      // For non-inplace cases, keep moving the YPosition
                      if (!inplace)
                      {
                          yPos++;
                          if (yPos >= pModeInfo->VerticalResolution)
                          {
                              yPos = 0;
                          }
                      }
                  }

                  if (eStatus == EFI_SUCCESS)
                  {
                      // End time
                      uEndTime = GetTimerCountms();
                      //Function execution time(ms)
                      uTotalFuncTime = (uEndTime - uStartTime);

                      if (uTotalFuncTime == 0)
                      {
                          AsciiPrint("Total time taken for BltVideoFill: 0ms\n");
                      }
                      else
                      {
                          perfno = (blocksize[iter] * iterationArray[iter] * blockLength * 1000) / uTotalFuncTime;

                          DisplayBltPrintResults("BltVideoFill", perfno, uTotalFuncTime, blocksize[iter]);
                      }
                  }
                  else
                  {
                      AsciiPrint("DisplayBltOperationTest: Blt(EfiBltVideoFill) %d pixels failed. \n", blocksize[iter]);
                      break;
                  }
              }


              // Buffer to Video
              AsciiPrint("\nSystem buffer to video buffer copy (EfiBltBufferToVideo)\n");
              for (iter = 0; iter < (sizeof(blocksize) / sizeof(blocksize[0])); iter++)
              {
                  UINT32 hori = blocksize[iter];
                  
                  BufferLen = hori * verti * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);

                  eStatus = gBS->AllocatePool(EfiBootServicesData, (BufferLen), (VOID**)&BltBuffer);
                  if ((eStatus != EFI_SUCCESS) || (BltBuffer == NULL))
                  {
                      eStatus = !EFI_SUCCESS;
                      break;
                  }
                  else
                  {
                      //Zero out the buffer to make it black
                      ZeroMem(BltBuffer, (BufferLen));

                      // Start Time
                      yPos = 0;
                      uStartTime = GetTimerCountms();
                      for (loopv = 0; loopv < iterationArray[iter]; loopv++)
                      {
                          if (EFI_SUCCESS != (eStatus = pGfxProtocol->Blt(pGfxProtocol,
                                                                          (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)BltBuffer,
                                                                          EfiBltBufferToVideo,
                                                                          0, 0,
                                                                          0, yPos,
                                                                          hori,
                                                                          verti,
                                                                          0)))
                          {
                              break;
                          }

                          // For non-inplace cases, keep moving the YPosition
                          if (!inplace)
                          {
                              yPos++;
                              if (yPos >= pModeInfo->VerticalResolution)
                              {
                                  yPos = 0;
                              }
                          }
                      }

                      if (EFI_SUCCESS == eStatus)
                      {
                          // End time
                          uEndTime = GetTimerCountms();
                          //Function execution time(ms)
                          uTotalFuncTime = (uEndTime - uStartTime);
                          if (uTotalFuncTime == 0)
                          {
                              AsciiPrint("Total time taken for BltBufferToVideo: 0ms\n");
                          }
                          else
                          {
                              perfno = (blocksize[iter] * iterationArray[iter] * blockLength * 1000) / (uTotalFuncTime);

                              DisplayBltPrintResults("BltBufferToVideo", perfno, uTotalFuncTime, blocksize[iter]);
                          }
                      }
                      else
                      {
                          AsciiPrint("DisplayBltOperationTest: Blt(EfiBltBufferToVideo) %d pixels failed. \n", blocksize[iter]);
                          break;
                      }

                      gBS->FreePool(BltBuffer);
                  }
              }

              // Video to Video copy 
              AsciiPrint("\nVideo buffer to video buffer copy (EfiBltVideoToVideo)\n");
              for (iter = 0; iter < (sizeof(blocksize) / sizeof(blocksize[0])); iter++)
              {
                  UINT32 hori = blocksize[iter];
                  
                  // Start Time
                  yPos = 0;
                  uStartTime = GetTimerCountms();
                  for (loopv = 0; loopv < iterationArray[iter]; loopv++)
                  {
                      if (EFI_SUCCESS != (eStatus = pGfxProtocol->Blt(pGfxProtocol, NULL,
                                                                      EfiBltVideoToVideo,
                                                                      0, 0,
                                                                      0, yPos,
                                                                      hori,
                                                                      verti,
                                                                      0)))
                      {
                          break;
                      }

                      // For non-inplace cases, keep moving the YPosition
                      if (!inplace)
                      {
                          yPos++;
                          if (yPos >= pModeInfo->VerticalResolution)
                          {
                              yPos = 0;
                          }
                      }
                  }

                  if (EFI_SUCCESS == eStatus)
                  {
                      // End time
                      uEndTime = GetTimerCountms();
                      //Function execution time(ms)
                      uTotalFuncTime = (UINT32)(uEndTime - uStartTime);
                      if (uTotalFuncTime == 0)
                      {
                          AsciiPrint("Total time taken for BltVideoToVideo(%d pixels): 0ms\n", blocksize[iter]);
                      }
                      else
                      {
                          perfno = (blocksize[iter] * iterationArray[iter] * blockLength * 1000) / uTotalFuncTime;

                          DisplayBltPrintResults("BltVideoToVideo", perfno, uTotalFuncTime, blocksize[iter]);
                      }
                  }
                  else
                  {
                      AsciiPrint("DisplayBltOperationTest: Blt(EfiBltVideoToVideo) %d pixels failed. \n", blocksize[iter]);
                  }
              }
          }
      }
  }

  return eStatus; 
}



/**
 Mode test for the primary/internal display
**/
static EFI_STATUS DisplaySetModeTest()
{
  EFI_STATUS                     eStatus        = EFI_SUCCESS;
  EFI_GUID                       sGfxOutputGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGfxProtocol;

  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&sGfxOutputGUID, 
                                                     NULL, 
                                                     (VOID **)&pGfxProtocol)))                 
  {
    AsciiPrint ("DisplaySetModeTest: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
     uint32 uLastMode = 0;
     uint32 uModeIndex;

     for (uModeIndex = 0; uModeIndex < pGfxProtocol->Mode->MaxMode; uModeIndex++)
     {
       if (EFI_SUCCESS == (eStatus = pGfxProtocol->SetMode(pGfxProtocol, (UINT32) uModeIndex)))
       {
         AsciiPrint("DisplaySetModeTest : Mode#%d succesfully changed (Current Mode %dx%d)\n",
                (UINT32)uModeIndex,
                pGfxProtocol->Mode->Info->HorizontalResolution,
                pGfxProtocol->Mode->Info->VerticalResolution);

         uLastMode = uModeIndex;
       }
       else
       {
         AsciiPrint("DisplaySetModeTest : Mode#%d failed! (status:%d)\n",
                (UINT32)uModeIndex,
                eStatus);
       }
       gBS->Stall(DEFAULT_LONG_WAIT);       
     }

     // Reset back to mode 0 at the end
     if (0 != uLastMode)
     {
        pGfxProtocol->SetMode(pGfxProtocol, 0);
     }
  }

  return eStatus;
}


/**
Display Set Mode
**/
static EFI_STATUS SetMode(UINT32 uModeIndex)
{
  EFI_STATUS                     eStatus        = EFI_SUCCESS;
  EFI_GUID                       sGfxOutputGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *pGfxProtocol;

  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol (&sGfxOutputGUID, 
                                                     NULL, 
                                                     (VOID **)&pGfxProtocol)))                 
  {
    AsciiPrint ("SetMode: LocateProtocol() failed.\n");
    return eStatus;
  }
  else
  {
     if (EFI_SUCCESS == (eStatus = pGfxProtocol->SetMode(pGfxProtocol, (UINT32) uModeIndex)))
     {
       AsciiPrint("DisplaySetModeTest : Mode#%d succesfully changed (Current Mode %dx%d)\n",
              (UINT32)uModeIndex,
              pGfxProtocol->Mode->Info->HorizontalResolution,
              pGfxProtocol->Mode->Info->VerticalResolution);
     }
     else
     {
        AsciiPrint("SetMode : Mode#%d failed! (status:%d)\n",
              (UINT32)uModeIndex,
              eStatus);
     }
  }    

  return eStatus;
}


/**
Mode test for external wired display

**/
static EFI_STATUS DisplaySetModeExtTest(void)
{
  EFI_STATUS                        eStatus            = EFI_SUCCESS;
  EFI_GUID                          sDisplayUtilGUID   = EFI_DISPLAYUTILS_PROTOCOL_GUID;
  EFI_QCOM_DISPLAY_UTILS_PROTOCOL  *pDisplayUtilProtocol;
  UINT32                            DisplayId          = 2; /* external display */
  UINT32                            ModeNum            = 0;
  UINT32                            Flags              = 0;

  AsciiPrint("\nNote: For this test a Type-C to DP (without PD) cable must be plugged in to a monitor.\n");
  AsciiPrint("      Plug polarity is not supported, so please try both cable directions.\n");
  AsciiPrint("      This test will fail if a monitor cannot be detected.\n\n");

  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol(&sDisplayUtilGUID,
                                                     NULL,
                                                     (VOID **)&pDisplayUtilProtocol)))
  {
     AsciiPrint("DisplaySetModeExtTest: LocateProtocol() failed.\n");
  }
  else 
  {
      if (EFI_SUCCESS == (eStatus = pDisplayUtilProtocol->DisplayUtilsSetMode(DisplayId, ModeNum, Flags)))
      {
          AsciiPrint("DisplaySetModeExtTest: Success to set external display mode\n");
      }
      else 
      {
          AsciiPrint("DisplaySetModeExtTest: Failed to set external display mode\n");
      }

  }

  return eStatus;
}


/* Event based display power control test.
*  1. Create UI_Active and UI_Idle events based on the event GUID.
*  2. Signal UI_Idle event, display is expected to be off, all power rails, PLL, clock, GPIO should be turned off
*  3. Signal UI_Acitve event, display is expected to be on, all power rails, PLL, clock, GPIO should be turned on
*  4. Repeat step 2 & 3 one more time
*/
static EFI_STATUS DisplayPowerEventTest(void)
{
  EFI_STATUS    eStatus            = EFI_SUCCESS;
  EFI_GUID      UIActiveEventGuid  = EFI_UI_ACTIVE_EVENT_GUID;
  EFI_GUID      UIIdleEventGuid    = EFI_UI_IDLE_EVENT_GUID;
  EFI_EVENT     UIActiveEvent      = NULL;
  EFI_EVENT     UIIdleEvent        = NULL;


  //Create UI_Acitve/UI_Idle events
  if (EFI_SUCCESS != (eStatus = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyEventHandle, NULL, &UIActiveEventGuid, &UIActiveEvent)))
  {
    AsciiPrint("DisplayPowerEventTest: Failed to create event UI_Active\n");
  }
  else if (EFI_SUCCESS != (eStatus = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyEventHandle, NULL, &UIIdleEventGuid, &UIIdleEvent)))
  {
    AsciiPrint("DisplayPowerEventTest: Failed to create event UI_Idle\n");
  }
  else
  {
    UINT32                         cntr = 0;

    //Loop to turn of and on display multiply times.
    for (cntr = 0; cntr < 2; cntr++)
    {
      /* Turn off the display */
      AsciiPrint("DisplayPowerEventTest: Display panel power off...\n");
      if (EFI_SUCCESS != (eStatus = gBS->SignalEvent(UIIdleEvent)))
      {
        AsciiPrint("DisplayPowerEventTest: failed signal event UI_Idle.\n");
        break;
      }
      gBS->Stall(DEFAULT_LONG_WAIT);

      /* Turn on the display */
      AsciiPrint("DisplayPowerEventTest: Display panel power on...\n");
      if (EFI_SUCCESS != (eStatus = gBS->SignalEvent(UIActiveEvent)))
      {
        AsciiPrint("DisplayPowerEventTest: failed signal event UI_Active.\n");
        break;
      }
      gBS->Stall(DEFAULT_LONG_WAIT);
    }
  }

  //Close Event handles
  if (NULL != UIActiveEvent)
  {
    if (EFI_SUCCESS != (eStatus = gBS->CloseEvent(UIActiveEvent)))
    {
      AsciiPrint("DisplayPowerEventTest: Failed to Close event UI_Active\n");
    }
  }
  if (NULL != UIIdleEvent)
  {
    if (EFI_SUCCESS != (eStatus = gBS->CloseEvent(UIIdleEvent)))
    {
      AsciiPrint("DisplayPowerEventTest: Failed to Close event UI_Idle\n");
    }
  }

  return eStatus;
}


/* Generate a checkerboard bitmap image
*/
static EFI_STATUS DisplayGenerateBitmap(void)
{
  EFI_STATUS    eStatus            = EFI_SUCCESS;

  eStatus = gBS->AllocatePool(EfiBootServicesData, (BITMAP_IMAGE_WIDTH*BITMAP_IMAGE_HEIGHT*4), (VOID**)&pBitmapImage);
  
  if ((eStatus != EFI_SUCCESS) || (pBitmapImage == NULL))
  {
    eStatus = EFI_OUT_OF_RESOURCES;
  }
  else
  {
      UINT32  pattern[2]   = {0xFF993300, 0xFFFFFFF};
      UINT32  *pData       = (UINT32*)pBitmapImage;
      UINT32  uPatternSize = 4;
      UINT32  x, y;

      
      for (y=0; y<BITMAP_IMAGE_HEIGHT ;y++)
      {
        for (x=0; x<BITMAP_IMAGE_WIDTH; x++)
        {
          *pData++ = pattern[((y>>uPatternSize)+(x>>uPatternSize)) & 1];
        }
      }  
  }

  return eStatus;
}



/* Dummy Power Event handle function needed to create local events for UI_Active/UI_Idle*/
static VOID DummyEventHandle(EFI_EVENT  Event, VOID *Context)
{
  /* Dummy Power Event handle function, do nothing here*/
}


/* DisplayApp Help
*/
static void DisplayAppHelp(void)
{
  UINT32 uCount;
  
  AsciiPrint ("Usage:\n");
  AsciiPrint (" DisplayApp -[option]:[value]\n\n");
  AsciiPrint (" options:\n");  
  AsciiPrint ("   -bvt            : Execute the display BVT test or a specific test number\n");  
  AsciiPrint ("   -disabledisplay : Set or clear the disable display variable\n");
  AsciiPrint ("   -panelid        : Set or clear the override panel ID, with no parameters it will list the IDs\n");
  AsciiPrint ("                     Use the keyword 'next' to configure the next available panel ID\n");
  AsciiPrint ("   -setpower       : Change the power state of the primary display (1-On, 0-Off).\n");    
  AsciiPrint ("   -setmode        : Change the display mode (virtual mode).\n\n");  
    
  AsciiPrint ("\nTest Case List:\n");

  for (uCount=0;uCount<NUM_TEST_CASES;uCount++)
  {
    AsciiPrint ("  %d : %a\n", uCount+1, TestCaseList[uCount].sTestName);
  }
  
  AsciiPrint ("\n\n");  
}


/* Extract out XML tag for panel names from panel configuration
*/
static void GetPanelName(CHAR8 *pConfig, uint32 uConfigLength, CHAR8 *pPanelName, uint32 uPanelNameLength)
{
   CHAR8  *pTagStart;
   CHAR8  *pTagEnd;   

   ZeroMem(pPanelName, uPanelNameLength);
   
   if ((NULL != (pTagStart = AsciiStrStr(pConfig, "<PanelDescription>"))) &&
       (NULL != (pTagEnd   = AsciiStrStr(pConfig, "</PanelDescription>"))))
   {
     CHAR8   *pNameStart = pTagStart+AsciiStrLen("<PanelDescription>");
     uint32  uLength     = (pTagEnd-pNameStart);
     uint32  uCopyAmount = (uLength<uPanelNameLength)?uLength:uPanelNameLength;
     
     AsciiStrnCpy(pPanelName, pNameStart, uCopyAmount);        
   }
   else if ((NULL != (pTagStart = AsciiStrStr(pConfig, "<PanelName>"))) &&
            (NULL != (pTagEnd   = AsciiStrStr(pConfig, "</PanelName>"))))
   {
     CHAR8   *pNameStart = pTagStart+AsciiStrLen("<PanelName>");
     uint32  uLength     = (pTagEnd-pNameStart);
     uint32  uCopyAmount = (uLength<uPanelNameLength)?uLength:uPanelNameLength;
     
     AsciiStrnCpy(pPanelName, pNameStart, uCopyAmount);        
   }
   else    
   {
     AsciiStrnCpy(pPanelName, "Panel (Unknown configuration)", AsciiStrLen("Panel (Unknown configuration)"));
   }
}


/* DisplayListPanels Help
*/
static UINT32 DisplayListPanels(uint32 uCurrentIndex, UINTN bShow)
{
  EFI_STATUS                        eStatus            = EFI_SUCCESS;
  UINT32                            uNumPanels         = 0;  
  EFI_GUID                          sDisplayUtilGUID   = EFI_DISPLAYUTILS_PROTOCOL_GUID;
  EFI_QCOM_DISPLAY_UTILS_PROTOCOL  *pDisplayUtilProtocol;

  if (EFI_SUCCESS != (eStatus = gBS->LocateProtocol(&sDisplayUtilGUID,
                                                     NULL,
                                                     (VOID **)&pDisplayUtilProtocol)))
  {
     AsciiPrint("DisplayListPanels: LocateProtocol() failed.\n");
  }
  else 
  {
    EFI_DISPLAY_UTILS_PANEL_CONFIG_PARAMS sPanelConfig;
    UINT32                                uIndex      = 0;
    UINT32                                uParamSize  = sizeof(sPanelConfig);

    if (bShow)
    {
      AsciiPrint ("Available Panel List:\n");
      AsciiPrint ("%2a 0] Default Panel (Auto detect)\n", (uCurrentIndex==0)?"*":"");
    } 

    do
    {
      ZeroMem(&sPanelConfig, sizeof(sPanelConfig));
      sPanelConfig.uPanelIndex = uIndex;
      if (EFI_SUCCESS == (eStatus = pDisplayUtilProtocol->DisplayUtilsGetProperty(EFI_DISPLAY_UTILS_PANEL_CONFIG, (VOID*)&sPanelConfig, &uParamSize)))
      {
         if (bShow)
         {
           char sName[128];

           GetPanelName((CHAR8*)sPanelConfig.pPanelConfig, sPanelConfig.uPanelConfigLength, (CHAR8*)&sName, sizeof(sName));
         
            AsciiPrint ("%2a %d] %a\n", (uCurrentIndex==(uIndex+1))?"*":"", uIndex+1, sName);
         }

         uIndex++;
         uNumPanels++;
      }
    }  while (eStatus == EFI_SUCCESS);  
  }


  if (bShow)
  {
    AsciiPrint ("\n\n"); 
    AsciiPrint ("* - Current override selection"); 
    AsciiPrint ("\n\n");  
  }

  return uNumPanels;
}



/* DisplayApp Command line parser
 * 
                                0123456789012345
                                 1234567 1234567
 * Parse a string in the format -<Param>:<Value>
*/
static UINTN DisplayParseCommand(CHAR8 *pCommandLine, CHAR8 *pParam, CHAR8 *pValue)
{
  UINTN params = 0;
  UINTN strlength = AsciiStrLen(pCommandLine);

  // Initialize output to null
  pParam[0] = '\0';
  pValue[0] = '\0';

  // If we detect the swich and at least 1 valid character
  if ((pCommandLine[0] == '-') &&
      (pCommandLine[0] != ':') &&
      (pCommandLine[0] != ' ') &&
      (strlength > 2))
  {
    UINTN uCount;

    // At least one param detected
    params++;

    // Find the length of the parameter
    for (uCount = 1; uCount<strlength; uCount++)
    {
      if ((pCommandLine[uCount] == ':') ||
        (pCommandLine[uCount] == ' '))
      {
        break;
      }
    }

    // Copy the paramter
    AsciiStrnCpy(pParam, &pCommandLine[1], uCount - 1);
    pParam[uCount-1] = '\0';

    // Copy the optional value
    if ((pCommandLine[uCount] == ':') &&
      ((uCount + 1)<strlength))
    {
      params++;

      AsciiStrnCpy(pValue, &pCommandLine[uCount + 1], strlength - uCount - 1);
      pValue[strlength-uCount-1] = '\0';
    }
  }

  return params;
}

/* DisplayDumpClocks 
 * - Helper to dump all clocks
 * 
 */
static void DisplayDumpClocks()
{
  uint32 i;
  
  AsciiPrint("%-30a %4a %10a\n", "Clock Name", "State", "Frequency");
  for (i=0;i<NUM_CLOCK_ENTRIES;i++)
  {
     DisplayClockStatus(ClockList[i].sClockName);
  }
}

/* DisplayClockStatus 
 * - Display the status of a clock 
 * 
 */
static void DisplayClockStatus(CHAR8 *pClockName)
{
  EFI_CLOCK_PROTOCOL  *mClockProtocol;
  UINTN                uClockId;
  
  // Grab the clock protocol
  if (NULL == pClockName)
  {
    AsciiPrint("ERROR: Cannot query invalid clock\n");
  }
  else if (EFI_SUCCESS != gBS->LocateProtocol(&gEfiClockProtocolGuid,
                                         NULL,
                                         (VOID **) &mClockProtocol))
  {
    AsciiPrint("ERROR: Failed to read clock %a\n", pClockName);
  }
  else if (EFI_SUCCESS != mClockProtocol->GetClockID(mClockProtocol,  pClockName, &uClockId))
  {
    AsciiPrint("ERROR: %a is not a valid clock\n", pClockName);
  }
  else 
  {
    BOOLEAN bClockOn = FALSE; 
    UINT32  uFreq    = 0;
    
      if (EFI_SUCCESS != mClockProtocol->IsClockOn(mClockProtocol, uClockId, &bClockOn))
    {
         AsciiPrint("ERROR: Failed to query clock status of %a\n", pClockName);    
    }
    
    if (EFI_SUCCESS != mClockProtocol->GetClockFreqHz(mClockProtocol, uClockId, &uFreq))
    {
         AsciiPrint("ERROR: Failed to query clock frequency of %a\n", pClockName);    
    }
    
    AsciiPrint("%-30a %3a    %dHz\n", pClockName, (bClockOn)?"ON":"OFF", (bClockOn)?uFreq:0);  
  }
}
