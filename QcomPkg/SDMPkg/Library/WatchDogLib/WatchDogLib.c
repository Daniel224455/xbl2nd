/**@file WatchDogLib.c

  Copyright (c) 2017-2019, Qualcomm Technologies Inc. All rights reserved.<BR>

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 
 --------   ---     -----------------------------------------------------------
 08/30/18   yg      Modify API interface
 08/04/17   c_mku   Added Watchdog pet Mechanism
 06/13/16   vk      Initial version
=============================================================================*/
#include <Library/ArmLib.h>
#include <Library/WatchdogLib.h>
#include <WatchDogLibHWIO.h>

#define SLEEP_CLK_FREQ             32768

#define MAX_WDOG_COUNTER_VAL       (1 << 20)

#define MS_TO_WDOG_COUNTER_VAL(x)  ((((x) * SLEEP_CLK_FREQ) / 1000) - 1)

#define MAX_WDOG_BITE_VAL          (MAX_WDOG_COUNTER_VAL - 1)

#define FORCED_BITE_TIME_MS        4

/*==============================================================================
**  Functions
**============================================================================*/

/*==============================================================================
  FUNCTION      WatchDogEnable

  DESCRIPTION   Set Bit 0 in WDOG_CTL to Enable WatchDogReset 
  

==============================================================================*/

VOID
WatchDogEnable (VOID)
{
  HWIO_APSS_WDT_TMR1_WDOG_CTL_OUT (HWIO_APSS_WDT_TMR1_WDOG_CTL_ENABLE_ENABLE_FVAL);
  ArmInstructionSynchronizationBarrier ();
}

/*==============================================================================
  FUNCTION      WatchDogDisable

  DESCRIPTION   Reset Bit 0 in WDOG_CTL to Disable WatchDogReset 
  
==============================================================================*/

VOID
WatchDogDisable (VOID)
{
  HWIO_APSS_WDT_TMR1_WDOG_CTL_OUT (HWIO_APSS_WDT_TMR1_WDOG_CTL_ENABLE_DISABLE_FVAL);
  ArmInstructionSynchronizationBarrier ();
}

/*==============================================================================
  FUNCTION      WatchDogSetBiteTimeout

  DESCRIPTION   Set the WDOG_BITE_TIME with the input BiteTime
  
  BITETIME      IN PARAM - time in msec
  
  RETVAL        EFI_SUCCESS upon success
                EFI_INVALID_PARAMTER if BITETIME > max time 
  
  Note: If the watchdog Bite timer value is changed while wdog is enabled,
        the new Bite timer value will take effect immediately
==============================================================================*/

EFI_STATUS
WatchDogSetBiteCounter (UINT32 BiteCounterVal)
{
  if ((BiteCounterVal == 0) || (BiteCounterVal > MAX_WDOG_BITE_VAL))
    return EFI_INVALID_PARAMETER;
  
  HWIO_APSS_WDT_TMR1_WDOG_BITE_TIME_OUT (BiteCounterVal);

  ArmInstructionSynchronizationBarrier ();
  
  WatchDogPet ();
  
  return EFI_SUCCESS;  
}


/*==============================================================================
  FUNCTION      WatchDogSetBiteTimeout

  DESCRIPTION   Set the WDOG_BITE_TIME with the input BiteTime
  
  BITETIME      IN PARAM - time in msec
  
  RETVAL        EFI_SUCCESS upon success
                EFI_INVALID_PARAMTER if BITETIME > max time 
  
  Note: If the watchdog Bite timer value is changed while wdog is enabled,
        the new Bite timer value will take effect immediately
==============================================================================*/

EFI_STATUS
WatchDogSetBiteTimeout (UINT32 BiteTimeMSec)
{
  UINT32 BiteTimeCounterVal = MS_TO_WDOG_COUNTER_VAL (BiteTimeMSec);

  return WatchDogSetBiteCounter (BiteTimeCounterVal);
}

/*==============================================================================
  FUNCTION      WatchDogPet

  DESCRIPTION   Set the WDOG_RESET register to reset the count in 
                WDOG_STATUS register
==============================================================================*/

VOID
WatchDogPet (VOID)
{
  //Set the WDOG_RESET reg 
  // WDOG_STATUS[20:1] will reset to 0 after this
  HWIO_APSS_WDT_TMR1_WDOG_RESET_OUT (HWIO_APSS_WDT_TMR1_WDOG_RESET_RESET_BMSK);
  ArmInstructionSynchronizationBarrier ();
}

/*==============================================================================
  FUNCTION      TriggerWatchDogReboot

  DESCRIPTION   Enable WatchDog and force immediate target reboot
==============================================================================*/

VOID
WatchDogTriggerBite (VOID)
{
  /* CTL, Enable */
  //WatchDogEnable();
  
  WatchDogPet ();

  HWIO_APSS_WDT_TMR1_WDOG_BITE_TIME_OUT (FORCED_BITE_TIME_MS);
  ArmInstructionSynchronizationBarrier ();
  
  while (1);
}

EFI_STATUS WatchDogSetPetTimerPeriod (UINT32 PeriodMSec)
{
  return EFI_UNSUPPORTED;
}

VOID WatchDogForcePet (VOID)
{
  WatchDogPet ();
}

VOID WatchDogForceBite (VOID)
{
  WatchDogTriggerBite ();
}

