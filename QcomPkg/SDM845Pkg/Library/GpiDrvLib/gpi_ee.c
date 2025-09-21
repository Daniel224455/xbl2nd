/**
  @file gpi_ee.c
  @brief
  This file contains implementation of interfaces that provide a thin
  abstraction to various OS-specific APIs used by the GPI driver.
*/

/*
===============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------
01/31/17   ts      file ported to UEFI.
02/19/16   ah      Created

===============================================================================
                   Copyright (c) 2016-2017 Qualcomm Technologies Incorporated.
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

#include "gpi_utils.h"
#include "gpitgtcfgdata.h"

#ifdef GPI_XML_PROPS
static tgt_gpi_config_type *gpi_tgt_config_ptr = NULL;
static tgt_gpii_config_type *gpii_tgt_config_ptr = NULL;
DALSYS_PROPERTY_HANDLE_DECLARE(hPropGPI);
#else
static const tgt_gpi_config_type *gpi_tgt_config_ptr = tgt_gpi_config;
static const tgt_gpii_config_type *gpii_tgt_config_ptr = tgt_gpii_config;
#endif

#ifdef GPI_DALSYS
DalDeviceHandle *hGpiDALInterrupt = NULL;
DalDeviceHandle *hGpiDALClock = NULL;
DalDeviceHandle *hGpiHWIO;
#endif

uint32 gpi_ee_init_done = 0;

/* ULog */
#ifdef GPI_DEBUG
ULogHandle gpiulog;                                    /* ULog global handle */
ULogResult gpi_ulog_enable = ULOG_ERR_INITINCOMPLETE;  /* Ulog enabled flag */

static void gpi_uloginit(void);

/**
 * Ulog message logging interface.
 * Allocates buffer for logging.On successful initialization it will return the Ulog handle.
 *
 * @param    None.
 *
 * @return   None.
 */
void gpi_uloginit(void)
{
   gpi_log(GPI_DEBUG, "gpi_uloginit() <-\n");

   /* Ulog Handle and buffer intialization. */
   if (gpi_ulog_enable == ULOG_ERR_INITINCOMPLETE)
   {
      gpi_ulog_enable = ULogFront_RealTimeInit(&gpiulog, "GPI",
                                               GPI_ULOG_BUFFER_MAX,
                                               ULOG_MEMORY_LOCAL,
                                               ULOG_LOCK_NONE);
   }
   gpi_log(GPI_DEBUG, "gpi_uloginit() ->\n");
}

/**
 * Ulog message logging interface De-initialization.
 *
 * @param    None.
 *
 * @return   None.
 */
void gpi_ulogdeinit(void)
{
   gpi_log(GPI_DEBUG, "gpi_ulogdeinit() <-\n");
   gpi_ulog_enable = ULOG_ERR_INITINCOMPLETE;
   gpi_log(GPI_DEBUG, "gpi_ulogdeinit() ->\n");
}
#endif

/**
 * Initializes EE Interfaces.
 * Performs any intiialization pertaining to the OS specific APIs.
 *
 * @param    None
 *
 * @return   None.
 */
GPI_RETURN_STATUS gpi_ee_init(void)
{
   gpi_log(GPI_DEBUG, "gpi_ee_init() <-\n");

   if (gpi_ee_init_done)
   {
      gpi_log(GPI_DEBUG, "gpi_ee_init already done, returning\n");
      return GPI_STATUS_SUCCESS;
   }

#if (defined GPI_DALSYS || defined GPI_XML_PROPS)
   DALSYS_InitMod(NULL);
#endif

#ifdef GPI_DEBUG
   gpi_uloginit();
#endif

   gpi_ee_init_done = 1;
   gpi_log(GPI_DEBUG, "gpi_ee_init() ->\n");

   return GPI_STATUS_SUCCESS;
}

/**
 * De-initializes OS specific Interfaces.
 *
 * Performs any De-initialization pertaining to the OS specific APIs.
 *
 * @param    None
 *
 * @return   None.
 */
void gpi_ee_deinit(void)
{
   gpi_log(GPI_DEBUG, "gpi_ee_deinit() <-\n");

   if (!gpi_ee_init_done)
   {
      return;
   }

#ifdef GPI_DALSYS
   DAL_DeviceDetach(hGpiDALInterrupt);
   hGpiDALInterrupt = NULL;
   DAL_DeviceDetach(hGpiDALClock);
   hGpiDALClock = NULL;
   DAL_DeviceDetach(hGpiHWIO);
   hGpiHWIO = NULL;
   DALSYS_DeInitMod();
#endif

#ifdef GPI_DEBUG
   gpi_ulogdeinit();
#endif

   gpi_ee_init_done = 0;

   gpi_log(GPI_DEBUG, "gpi_ee_deinit() ->\n");
}

/**
 * @brief Initializes GPI target info.
 *
 * @param[in,out]    gpi     Pointer to GPI context
 *
 * @return           Success of the operation.
 */
GPI_RETURN_STATUS gpi_init_tgt_info(gpi_ctxt *gpi)
{
   uint8 i;

   gpi_log(GPI_DEBUG, "gpi_init_tgt_info() <-\n");

#ifdef GPI_IRQ_MUXING
   gpi->irq_muxing = TRUE;
#endif

#ifdef GPI_DALSYS
   if (DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER, &hGpiDALInterrupt))
   {
      return GPI_STATUS_ERROR;
   }

   if (DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_CLOCK, &hGpiDALClock))
   {
      return GPI_STATUS_ERROR;
   }

   if (DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_HWIO, &hGpiHWIO))
   {
      return GPI_STATUS_ERROR;
   }
#endif

#ifdef GPI_XML_PROPS
   DALSYSPropertyVar gpiprop;
   
   if (DAL_SUCCESS != DALSYS_GetDALPropertyHandleStr("/core/hwengines/gpi", hPropGPI))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_GetDALPropertyHandleStr() failed!\n");
      return GPI_STATUS_ERROR;
   }

   if (DAL_SUCCESS != DALSYS_GetPropertyValue(hPropGPI, "tgt_gpi_config", 0, &gpiprop))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_GetPropertyValue() failed!\n");
      return GPI_STATUS_ERROR;
   }
   gpi_tgt_config_ptr = (tgt_gpi_config_type *)gpiprop.Val.pStruct;

   if (DAL_SUCCESS != DALSYS_GetPropertyValue(hPropGPI, "tgt_gpii_config", 0, &gpiprop))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_GetPropertyValue() failed!\n");
      return GPI_STATUS_ERROR;
   }
   gpii_tgt_config_ptr = (tgt_gpii_config_type *)gpiprop.Val.pStruct;

   if (DAL_SUCCESS != DALSYS_GetPropertyValue(hPropGPI, "tgt_num_gpii", 0, &gpiprop))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_GetPropertyValue() failed!\n");
      return GPI_STATUS_ERROR;
   }
   gpi->num_gpii = (uint32)gpiprop.Val.dwVal;
#else
   gpi->num_gpii = TGT_NUM_GPII;
#endif

   if (gpi->num_gpii <= 0 || gpi->num_gpii > MAX_GPI_CONFIG)
   {
      gpi_log(GPI_ERROR, "ERROR - gpi->num_gpii: %d!\n", gpi->num_gpii);
      return GPI_STATUS_ERROR;
   }

   if (gpi_tgt_config_ptr == NULL || gpii_tgt_config_ptr == NULL)
   {
      gpi_log(GPI_ERROR, "ERROR - NULL tgt_config_ptr!\n");
      return GPI_STATUS_ERROR;
   }

   for (i = 0; i < MAX_NUM_QUP; i++)
   {
      gpi->qup[i].active = gpi_tgt_config_ptr[i].active;

      if (gpi->qup[i].active)
      {
         gpi->qup[i].type = (QUP_BLOCK_TYPE)gpi_tgt_config_ptr[i].type;
         gpi->qup[i].qup_irq = gpi_tgt_config_ptr[i].irq;
#ifdef MAPPED_HWIO
         if (DAL_SUCCESS != DalHWIO_MapRegionByAddress(hGpiHWIO, 
                                                       (uint8 *)(gpi_tgt_config_ptr[i].gsi_pa), 
                                                       (uint8 **)&(gpi->qup[i].gsi_pa)))
         {
            gpi_log(GPI_ERROR, "ERROR - DalHWIO_MapRegionByAddress() failed!\n");
            return GPI_STATUS_ERROR;
         }
#else
         gpi->qup[i].gsi_pa = gpi_tgt_config_ptr[i].gsi_pa;
#endif
      }
   }

   for (i = 0; i < gpi->num_gpii; i++)
   {
      gpi->gpii[i].qup_type = (QUP_BLOCK_TYPE)gpii_tgt_config_ptr[i].qup_type;
      gpi->gpii[i].id = gpii_tgt_config_ptr[i].gpii_id;
      gpi->gpii[i].protocol = (GPI_PROTOCOL)gpii_tgt_config_ptr[i].protocol;
      gpi->gpii[i].options = gpii_tgt_config_ptr[i].options;
      gpi->gpii[i].ee = gpii_tgt_config_ptr[i].ee;
      gpi->gpii[i].se = gpii_tgt_config_ptr[i].se;
      gpi->gpii[i].irq = gpii_tgt_config_ptr[i].irq;

      gpi_log(GPI_DEBUG, "gpii[%d] id: %d\n",
              i, gpii_tgt_config_ptr[i].gpii_id);
      gpi_log(GPI_DEBUG, "gpii[%d] protocol: %d\n",
              i, gpii_tgt_config_ptr[i].protocol);
      gpi_log(GPI_DEBUG, "gpii[%d] options: %d\n",
              i, gpii_tgt_config_ptr[i].options);
      gpi_log(GPI_DEBUG, "gpii[%d] ee: %d\n",
              i, gpii_tgt_config_ptr[i].ee);
      gpi_log(GPI_DEBUG, "gpii[%d] se: %d\n",
              i, gpii_tgt_config_ptr[i].se);
      gpi_log(GPI_DEBUG, "gpii[%d] irq: %d\n",
              i, gpii_tgt_config_ptr[i].irq);
   }

   gpi_log(GPI_DEBUG, "gpi_init_tgt_info() ->\n");
   return GPI_STATUS_SUCCESS;
}

#ifdef GPI_ST
#define GPI_TASK_STACK_SIZE       1024
#define GPI_WORK_SIG_MASK         0x1

rex_stack_word_type gpi_task_stack[GPI_TASK_STACK_SIZE / sizeof(rex_stack_word_type)];
static gpi_ctxt *gpi_info;

/**
 * GPI task handler.
 * 
 * @param[in]    None.
 *
 * @return       None.
 */
static void gpi_task(dword ignored)
{
   for (;;)
   {
      rex_wait(GPI_WORK_SIG_MASK);
      rex_clr_sigs(&gpi_info->gpi_task_tcb, GPI_WORK_SIG_MASK);

      /* Process work items */
      handle_gpi_evts(gpi_info);
   }
}

/**
 * Registers a GPI task.
 *
 * @param    None
 *
 * @return   None.
 */
void gpi_reg_task(gpi_ctxt *gpi)
{
   gpi_info = gpi;

   /* Task registration */
   rex_def_task_ext(
                    (rex_tcb_type *)&gpi->gpi_task_tcb,   /* tcb        */
                    (unsigned char *)&gpi_task_stack[0],  /* stack      */
                    GPI_TASK_STACK_SIZE,                  /* stack size */
                    0x60,                                 /* priority   */
                    (rex_task_func_type)gpi_task,         /* function   */
                    0,                                    /* arguments  */
                    "GPI Task",                           /* task name  */
                    FALSE                                 /* suspended  */
                   );
}

/**
 * @brief      Signal task
 *
 * @param[in]     handle
 *
 * @return        None
 */
void gpi_signal_task(void)
{
   rex_set_sigs(&gpi_info->gpi_task_tcb, GPI_WORK_SIG_MASK);
}

#endif

/**
 * Copies memory of a specofied size to the destination.
 *
 * @param[in] dst Pointer to the destination memory address
 * @param[in] src Pointer to the source memory address
 * @param[in] size Number of bytes to copy
 *
 * @return   None.
 */
void gpi_memcpy(void *dst, void *src, uint32 size)
{
#ifdef GPI_DALSYS
   DALSYS_memcpy(dst, src, size);
#else
   memcpy(dst, src, size);
#endif
}

/**
 *  Sets memory of specified size to specified value.
 *
 *  @param[in] mem Pointer to the block of memory to fill
 *  @param[in] val Value of byte to fill the memory with
 *  @param[in] size Number of bytes to fill
 *
 *  @return   None.
 */
void gpi_memset(void *mem, uint8 val, uint32 size)
{
#ifdef GPI_DALSYS
   DALSYS_memset(mem, (int)val, size);
#else
   memset(mem, val, size);
#endif
}

#ifndef GPI_SDC
/**
 * SDC EE needs to translate local to system addresses.
 * For SDC this function is defined in system driver.
 *
 * @param[in]   addr    Address to translate
 *
 * @return      Translated address.
 */
ptr_type get_system_address(ptr_type addr)
{
   return addr;
}

/**
 * SDC EE needs to translate system to local addresses.
 * For SDC this function is defined in system driver.
 *
 * @param[in]   addr    Address to translate
 *
 * @return      Translated address.
 */
ptr_type get_local_address(ptr_type addr)
{
   return addr;
}
#endif

#ifdef GPI_MMU
/**
 * Performs a cache maintanence operation.
 *
 * @param[in]  cache_op    Cache Operation to perform (clean/flush/invalidate)
 * @param[in]  address     Memory for which cache maintanence to be performed.
 * @param[in]  size        Size of the memory
 *
 * @return   None.
 */
void gpi_cachecmd(gpi_cache_op_type cache_op, ptr_type address, uint32 size)
{
   gpi_log(GPI_DEBUG, "gpi_cachecmd() <-\n");

#if (defined GPI_DALSYS)
   uint32 cmd;

   switch (cache_op)
   {
      case GPI_CACHE_INVALIDATE:
         gpi_log(GPI_DEBUG, "GPI_CACHE_INVALIDATE!\n");
         cmd = DALSYS_CACHE_CMD_INVALIDATE;
         break;
      case GPI_CACHE_FLUSH:
         gpi_log(GPI_DEBUG, "GPI_CACHE_FLUSH!\n");
         cmd = DALSYS_CACHE_CMD_FLUSH;
         break;
      case GPI_CACHE_CLEAN:
         gpi_log(GPI_DEBUG, "GPI_CACHE_CLEAN!\n");
         cmd = DALSYS_CACHE_CMD_CLEAN;
         break;
      default:
         gpi_log(GPI_ERROR, "ERROR - default cache_op!\n");
         return;
   }

   DALSYS_CacheCommand(cmd, address, size);
#elif (defined __UEFI_ENV__)
   WriteBackInvalidateDataCacheRange((void *)address, size);
#else
   gpi_log(GPI_DEBUG, "cachecmd not available in this environment!\n");
#endif

   gpi_log(GPI_DEBUG, "gpi_cachecmd() ->\n");
}
#endif

/**
 * Performs a processor specific memory barrier operation.
 *
 * @param    None
 *
 * @return   None.
 */
void gpi_memorybarrier(void)
{
#ifdef GPI_DALSYS
   DALFW_MemoryBarrier();
#elif GPI_TZ
   memory_barrier();
#elif GPI_BOOT
   __asm__ __volatile__( "barrier\n" );
#else
   gpi_log(GPI_DEBUG, "memorybarrier not available in this environment!\n");
#endif
}

/**
 * Registers with the interrupt controller, an ISR to service an IRQ id.
 *
 * @param[in]      irq          IRQ ID to be registered for
 * @param[in,out]  isr          ISR to be invoked to the service the irq
 * @param[in,out]  data         data to be supplied to the isr upon invocation
 *
 * @return   Success of the operation. 
 */
GPI_RETURN_STATUS gpi_isrinstall(uint32 irq, isr_fct isr, void *data)
{
   gpi_log(GPI_DEBUG, "gpi_isrinstall() <-\n");

#if (defined GPI_DALSYS)
   /* Set this first as interrupt could have already fire.
    * Alternatively mask off all GPI interrupts first,
    * register the interrupt, and then enable interrupts */
   if (DAL_SUCCESS != DalInterruptController_RegisterISR(hGpiDALInterrupt,
                                                         irq,
                                                         (DALISR)isr,
                                                         (DALISRCtx)data,
                                                         DALINTRCTRL_ENABLE_DEFAULT_SETTINGS))
   {
      gpi_log(GPI_ERROR, "ERROR - DalInterruptController_RegisterISR() failed!\n");
      return GPI_STATUS_ERROR;
   }
#elif (defined GPI_XML_PROPS)
   return GPI_STATUS_SUCCESS;
#else
   /* SDC - Trigger Level-High */
   InterruptController_RegisterInterrupt(irq, isr, data, INTCF_TRIGGER_HIGH);
#endif

   gpi_log(GPI_DEBUG, "gpi_isrinstall() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Deregisters the ISR from the interrupt controller.
 *
 * @param[in,out]  irq    The specific GPII irq
 *
 * @return   None.
 */
void gpi_isruninstall(uint32 irq)
{
#if (defined GPI_DALSYS)
   DalInterruptController_Unregister(hGpiDALInterrupt, irq);
#elif (defined GPI_XML_PROPS)
   return;
#else
   InterruptController_DeregisterInterrupt(irq);
#endif
}

/**
 * @brief      Enable IRQ
 *
 * @param[in,out]  irq    The specific GPII irq
 *
 * @return     GPI_STATUS_SUCCESS/GPI_STATUS_ERROR
 */
GPI_RETURN_STATUS gpi_enable_irq(uint32 irq)
{
   gpi_log(GPI_DEBUG, "gpi_enable_irq() <-\n");

#ifdef GPI_DALSYS
   if (DAL_ERROR == DalInterruptController_InterruptEnable(hGpiDALInterrupt, irq))
   {
      gpi_log(GPI_ERROR, "ERROR - DalInterruptController_InterruptEnable() failed!\n");
      return GPI_STATUS_ERROR;
   }
#else
   gpi_log(GPI_DEBUG, "enable_irq not available in this environment!\n");
#endif

   gpi_log(GPI_DEBUG, "gpi_enable_irq() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * @brief      Disable IRQ
 *
 * @param[in,out]  irq    The specific GPII irq
 *
 * @return     GPI_STATUS_SUCCESS/GPI_STATUS_ERROR
 */
GPI_RETURN_STATUS gpi_disable_irq(uint32 irq)
{
   gpi_log(GPI_DEBUG, "gpi_disable_irq() <-\n");

#ifdef GPI_DALSYS
   if (DAL_ERROR == DalInterruptController_InterruptDisable(hGpiDALInterrupt, irq))
   {
      gpi_log(GPI_ERROR, "ERROR - DalInterruptController_InterruptDisable() failed!\n");
      return GPI_STATUS_ERROR;
   }
#else
   gpi_log(GPI_DEBUG, "disable_irq not available in this environment!\n");
#endif

   gpi_log(GPI_DEBUG, "gpi_disable_irq() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * @brief      Mask Interrupt
 *
 * @param[in,out]  irq    The specific GPII irq
 *
 * @return     None
 */
void gpi_isrmask(uint32 irq)
{
#ifdef GPI_DALSYS
   DalInterruptController_InterruptDisable(hGpiDALInterrupt, irq);
#else
   gpi_log(GPI_DEBUG, "isrmask not available in this environment!\n");
#endif
}

/**
 * @brief      Unmask interrupt
 *
 * @param[in,out]  irq    The specific GPII irq
 *
 * @return     None
 */
void gpi_isrunmask(uint32 irq)
{
#ifdef GPI_DALSYS
   DalInterruptController_InterruptEnable(hGpiDALInterrupt, irq);
#else
   gpi_log(GPI_DEBUG, "isrunmask not available in this environment!\n");
#endif
}

/**
 * Acknowledges the IRQ. Some OS implementations require the ISR to ack the IRQ
 * to re-activate the IRQ.
 *
 * @param[in,out]  irq    The specific GPII irq
 *
 * @return   None.
 */
void gpi_israck(uint32 irq)
{
#ifdef GPI_DALSYS
   DalInterruptController_InterruptDone(hGpiDALInterrupt, irq);
#else
   gpi_log(GPI_DEBUG, "israck not available in this environment!\n");
#endif
}

/**
 * @brief      Check with the interrupt controller if the supplied IRQ is set
 *
 * @param[in,out]  irq    The specific GPII irq
 *
 * @return     TRUE - Interrupt is set
 *             FALSE - Interrpt is not set
 */
boolean gpi_is_irq_set(uint32 irq)
{
#ifdef GPI_DALSYS
   if (DAL_INTERRUPT_SET == DalInterruptController_InterruptStatus(hGpiDALInterrupt, irq))
      return TRUE;
   else
#endif
      return FALSE;
}


#ifdef GPI_CLOCKS
/**
 * Initializes a clock control object.
 *
 * @param[in,out] clock_ctl     Pointer to the object to be used for clock control
 * @param[in,out] clock_id      Pointer to the object to identify the clock
 *
 * @return   Success of the operation.
 */
GPI_RETURN_STATUS gpi_clock_ctl_init(gpi_clock_ctl_type *clock_ctl, void *clock_id)
{
   gpi_log(GPI_DEBUG, "gpi_clock_ctl_init() <-\n");

   if (!clock_ctl)
   {
      gpi_log(GPI_ERROR, "ERROR - clock_ctl == NULL!\n");
      return GPI_STATUS_ERROR;
   }

   if (DAL_SUCCESS != DalClock_GetClockId(hGpiDALClock, (char *)clock_id, &clock_ctl->clock_id))
   {
      gpi_log(GPI_ERROR, "ERROR - DalClock_GetClockId() failed!\n");
      return GPI_STATUS_ERROR;
   }

   gpi_log(GPI_DEBUG, "gpi_clock_ctl_init() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Turn on clock.
 *
 * @param[in,out]  clock_ctl    clock control object to be used
 *
 * @return   Success of the operation.
 */
GPI_RETURN_STATUS gpi_clock_ctl_enable(gpi_clock_ctl_type *clock_ctl)
{
   gpi_log(GPI_DEBUG, "gpi_clock_ctl_enable() <-\n");

   if (!clock_ctl)
   {
      gpi_log(GPI_ERROR, "ERROR - clock_ctl == NULL!\n");
      return GPI_STATUS_ERROR;
   }

   if (DAL_SUCCESS != DalClock_EnableClock(hGpiDALClock, clock_ctl->clock_id))
   {
      gpi_log(GPI_ERROR, "ERROR - DalClock_EnableClock() failed!\n");
      return GPI_STATUS_ERROR;
   }

   gpi_log(GPI_DEBUG, "gpi_clock_ctl_enable() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Turn off clock.
 *
 * @param[in,out]  clock_ctl    clock control object to be used
 *
 * @return   Success of the operation.
 */
GPI_RETURN_STATUS gpi_clock_ctl_disable(gpi_clock_ctl_type *clock_ctl)
{
   gpi_log(GPI_DEBUG, "gpi_clock_ctl_disable() <-\n");

   if (!clock_ctl)
   {
      gpi_log(GPI_ERROR, "ERROR - clock_ctl == NULL!\n");
      return GPI_STATUS_ERROR;
   }

   if (DalClock_DisableClock(hGpiDALClock, clock_ctl->clock_id) != DAL_SUCCESS)
   {
      gpi_log(GPI_ERROR, "ERROR - DalClock_DisableClock() failed!\n");
      return GPI_STATUS_ERROR;
   }

   gpi_log(GPI_DEBUG, "gpi_clock_ctl_disable() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Deinitializes a clock control object.
 *
 * @param[in,out] clock_ctl     Pointer to the object to be used for clock control
 *
 * @return   Success of the operation.
 */
GPI_RETURN_STATUS gpi_clock_ctl_deinit(gpi_clock_ctl_type *clock_ctl)
{
   return GPI_STATUS_SUCCESS;
}
#endif

/**
 * @brief      Get physical address of a given virtual address
 *
 * @param[in]  buf - Virtual address
 *
 * @return     Physical address
 */
void* gpi_va_to_pa(void *buf)
{
#ifdef GPI_QURT
   return ((void *)qurt_lookup_physaddr((qurt_addr_t)buf));
#else
   return buf;
#endif
}

#ifdef GPI_MT
/**
 * @brief Initializes a sync object.
 *
 * @param[in,out] sync     Pointer to the object to be used for sync
 *
 * @return Success of the operation.
*/
GPI_RETURN_STATUS gpi_syncinit(gpi_sync_type *sync)
{
   gpi_log(GPI_DEBUG, "gpi_syncinit() <-\n");

   if (DAL_SUCCESS != DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE_INTERRUPT,
                                        (DALSYSSyncHandle *)sync,
                                        NULL))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_SyncCreate() failed!\n");
      *(DALSYSSyncHandle *)sync = NULL;
      return GPI_STATUS_ERROR;
   }

   gpi_log(GPI_DEBUG, "gpi_syncinit() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Destroys a synchronization object.
 *
 * @param[in,out]  sync    Sync object to be destroyed
 *
 * @return         None.
 */
GPI_RETURN_STATUS gpi_syncdestroy(gpi_sync_type *sync)
{
   gpi_log(GPI_DEBUG, "gpi_syncdestroy() <-\n");

   DALSYS_DestroyObject(*((DALSYSObjHandle *)sync));
   *(DALSYSSyncHandle *)sync = NULL;

   gpi_log(GPI_DEBUG, "gpi_syncdestroy() ->\n");
   return GPI_STATUS_SUCCESS;
}

/**
 * Enters into a synchronized context.
 *
 * @param[in,out]  sync    Sync object to be used
 *
 * @return         None.
 */
void gpi_syncenter(gpi_sync_type *sync)
{
   DALSYS_SyncEnter(*((DALSYSSyncHandle *)sync));
}

/**
 * Leaves a synchronized context.
 *
 * @param[in,out]  sync    Sync object to be used
 *
 * @return         None.
 */
void gpi_syncleave(gpi_sync_type *sync)
{
   DALSYS_SyncLeave(*((DALSYSSyncHandle *)sync));
}

/**
 * @brief      Wait for an event to be signalled
 *
 * @param[in]     handle - handle to event
 *
 * @return       None
 */
void gpi_eventwait(DALSYSEventHandle hEvent)
{
   DALSYS_EventWait(hEvent);
}

/**
 * @brief      Signal event
 *
 * @param[in]     handle
 *
 * @return        None
 */
void gpi_eventsignal(DALSYSEventHandle hEvent)
{
   DALSYS_EventCtrl(hEvent, DALSYS_EVENT_CTRL_TRIGGER);
}

/**
 * @brief      Signal event
 *
 * @param[in]     handle
 *
 * @return        None
 */
void gpi_eventreset(DALSYSEventHandle hEvent)
{
   DALSYS_EventCtrl(hEvent, DALSYS_EVENT_CTRL_RESET);
}

/**
 * @brief      Spawn a worker thread
 *
 * @param[in]  *obj   - Thread object
 * @param[in]  *name  - Name of thread
 * @param[in]  *entry - Entry point function
 * @param[in]  *ctx   - Context to pass to entry
 * @param[in]  priority - Priority of thread
 *
 * @return        None
 */
GPI_RETURN_STATUS gpi_spawn_thread(generic_thread_type *thread_obj,
                                   char *name, void *entry, void *ctx, uint32 priority)
{
   gpi_log(GPI_DEBUG, "gpi_spawn_thread() <-\n");

   /* Create event for sync between ISR and workloop */
   if (DAL_SUCCESS != DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT, &thread_obj->hEvent, NULL))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_EventCreate(hEvent) failed!\n");
      return GPI_STATUS_ERROR;
   }

   /* Create event to kick off the worker thread */
   if (DAL_SUCCESS != DALSYS_EventCreate(DALSYS_EVENT_ATTR_WORKLOOP_EVENT, &thread_obj->hStartEvent, NULL))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_EventCreate() failed!\n");
      return GPI_STATUS_ERROR;
   }

   /* Register work loop */
   if (DAL_SUCCESS != DALSYS_RegisterWorkLoopEx(name, GPI_STACK_SIZE, priority, 2, &thread_obj->hWorkLoop, NULL))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_RegisterWorkLoopEx() failed!\n");
      return GPI_STATUS_ERROR;
   }

   /* Add worker thread event to work loop */
   if (DAL_SUCCESS != DALSYS_AddEventToWorkLoop(thread_obj->hWorkLoop, entry, ctx, thread_obj->hStartEvent, NULL))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_AddEventToWorkLoop() failed!\n");
      return GPI_STATUS_ERROR;
   }

   /* Kick off the workloop */
   if (DAL_SUCCESS != DALSYS_EventCtrl(thread_obj->hStartEvent, DALSYS_EVENT_CTRL_TRIGGER))
   {
      gpi_log(GPI_ERROR, "ERROR - DALSYS_EventCtrl() failed!\n");
      return GPI_STATUS_ERROR;
   }

   gpi_log(GPI_DEBUG, "gpi_spawn_thread() ->\n");
   return GPI_STATUS_SUCCESS;
}
#endif

/**
 * @brief Signal a worker thread or task
 *
 * @param[in,out ]     gpi_ctxt           GPI context 
 *
 * @return       None.
 *
 */
void gpi_signal_worker(gpi_ctxt *gpi)
{
   if (gpi->exec_mode == GPI_TASK)
   {
      /* Schedule GPI main task */
      gpi_signal_task();
   }
   else
   {
      /* Trigger GPI worker thread */
      gpi_eventsignal(gpi->gpi_thread.thread.hEvent);
   }
}

/**
  Sleep function.

  @param[in]  wait    Sleep amount (usec for Q6)

  @return  None.
*/
void gpi_sleep(uint32 wait)
{
#ifdef GPI_QURT
   qurt_timer_sleep(wait);
#elif GPI_REX
   rex_sleep(wait);
#else
   gpi_log(GPI_ERROR, "ERROR - sleep not supported!\n");
#endif
}

/**
  Debug message logging interface.

  Debug message logging interface. Depending upon the OSAL implementation, it
  could be printed to a term window or sent as a diag msg or saved to a file

  @param[in]  fmt    Format similar to printf
  @param[in]  ...    Variable list of arguments to be processed

  @return  None.
*/
#ifdef GPI_DEBUG
void gpi_log(gpi_msg_type msgLevel, const char *fmt, ...)
{
   va_list arglist;
   const char *p = fmt;
   uint32 datacount = 0;

   if (msgLevel < GPI_MSG_LEVEL)
      return;

   va_start(arglist, fmt);


#if (defined(GPI_BOOT) || defined(GPI_SDC))
   // Nothing
#elif GPI_TZ
   tzbsp_log_with_ap(msgLevel, fmt, arglist);
#else
   if (!gpi_ulog_enable)
   {
      (void)vprintf(fmt, arglist);
   }
   else
   {
      /* ulog message data count */
      while (*p != '\0')
      {
         if ((*p == '%') && (*(p + 1) == '%'))
         {
            p += 2;
         }
         else
         {
            if (*p == '%')
            {
               datacount++;
            }
            p++;
         }
      }

      /* Ulog messages print */
      ULogFront_RealTimeVprintf(gpiulog, datacount, fmt, arglist);
   }
#endif

   va_end(arglist);
}
#endif

