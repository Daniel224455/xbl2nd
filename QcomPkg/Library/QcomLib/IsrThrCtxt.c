/** @file
  Convert ISR context into thread context to support handler.

  Copyright (c) 2018, Qualcomm Technologies, Inc. All Rights Reserved.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 05/01/18   yg      Initial revision.

=============================================================================*/

#include <PiDxe.h>

#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/EfiFileLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortShLib.h>
#include <Library/QcomLib.h>
#include <Library/QcomUtilsLib.h>
#include <Library/QcomBaseLib.h>
#include <Library/QcomTargetLib.h>

#include <Library/UefiCfgLib.h>
#include <Include/UefiInfoBlk.h>

#include <Library/KernelLib.h>
#include <Library/LockLib.h>


typedef struct {
   ThrCtxtIntrHandler      HandlerFunc;
   VOID                   *HandlerArg;
   UINT32                  VectorNum;
   Event                  *SyncEvent;
} ThrCtxtIsrInfo;

static
enum HandlerStatus ISRIntrHandler (VOID *Arg)
{
  ThrCtxtIsrInfo* IsrCtxt = (ThrCtxtIsrInfo*)Arg;
  Event* ISRHandleEvt = IsrCtxt->SyncEvent;

  // required to transition into active state
  UINT32 Vect = IsrCtxt->VectorNum;

  // Reschedule param has to be false in ISR
  EventSignal (ISRHandleEvt, FALSE); 

  // After handling the callback need to re-enable
  MaskInterrupt (Vect);

  return HANDLER_RESCHEDULE;
}

static
INT32 IntrThreadEntry (VOID* Arg)
{
  ThrCtxtIsrInfo* IsrCtxt = (ThrCtxtIsrInfo*)Arg;
  Event* ISRHandleEvt = IsrCtxt->SyncEvent;

  while (1)
  {
    EventWait (ISRHandleEvt);

    if (IsrCtxt->HandlerFunc)
      IsrCtxt->HandlerFunc (IsrCtxt->HandlerArg);
  }
}

/* This function can only be called from driver context where the driver
 * stays resident in memory. Since App memory is removed we cannot have
 * this registration happen if the image gets unloaded
 *
 * This function translates ISR handler into a thread context handler.
 * ie anything can be done in the callback function that's valid in a
 * thread context. ISR handlers usually have a lot of limitations on what
 * can be done in ISR context, usually pretty minimal 
 *
 * TODO: Optimize to potentially start only one thread and then registering
 * many handlers and handling from the same one thread
 */
EFI_STATUS RegisterThrCtxtIntrHandler (UINT32 Vector, ThrCtxtIntrHandler Cbfn, VOID* Arg)
{
  ThrCtxtIsrInfo *IsrCtxt;
  Thread* Thr;
  Event* ISRHandleEvt;
  CHAR8   ThrName[16];

  IsrCtxt = AllocatePool (sizeof(*IsrCtxt));

  if (IsrCtxt == NULL)
    return EFI_OUT_OF_RESOURCES;

  ISRHandleEvt = EventInit (0, SCHED_EVENT_STATE_UNSIGNALED, SCHED_EVENT_FLAG_AUTOUNSIGNAL);

  IsrCtxt->HandlerFunc = Cbfn;
  IsrCtxt->HandlerArg = Arg;
  IsrCtxt->VectorNum = Vector;
  IsrCtxt->SyncEvent = ISRHandleEvt;

  AsciiSPrint (ThrName, sizeof(ThrName), "IsrThr_%03d", Vector);

  Thr = ThreadCreate (ThrName, &IntrThreadEntry, (VOID*)IsrCtxt, HIGHEST_PRIORITY - 4, DEFAULT_STACK_SIZE);

  if (Thr == 0)
  {
    DEBUG ((EFI_D_WARN, "Unable to create ISR Handler thread for %d\n", Vector));
    
    EventDestroy (ISRHandleEvt);

    FreePool (IsrCtxt);

    return EFI_OUT_OF_RESOURCES;
  }

  ThreadSetPinnedCpu (Thr, 0);  // make sure this thread runs only core 0
  ThreadDetach (Thr);

  RegisterIntrHandler (Vector, &ISRIntrHandler, (VOID*)IsrCtxt);

  ThreadResume (Thr);

  return EFI_SUCCESS;
}

