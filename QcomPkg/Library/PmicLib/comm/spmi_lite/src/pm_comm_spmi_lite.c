/*! \file 
*  \n
*  \brief pm_comm_spmi_lite.c ---- 
*  \n
*  \n &copy; Copyright 2014-2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/
/* =======================================================================
Edit History
This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/02/18   aab     Updated pm_comm_write_byte_mask() to use Read/Modify/Write SPMI API 
01/22/18   pxm     Minor change for code style to make it tidy
12/05/17   aab     Added support to log number of SPMI transaction
09/13/16   aab     Updated logging text
08/04/16   aab     Added debug helper functions to get and print number of SPMI transaction
07/01/15   al      Change dummy write address   
07/16/14   akm     created  
========================================================================== */

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/
#include "pm_comm.h"  
#include "SpmiBus.h"
#include "CoreVerify.h"
#include "pm_utils.h"

/*===========================================================================

                        GLOBALS and TYPE DEFINITIONS 

===========================================================================*/
static boolean spmi_initialized = FALSE;
static uint32 num_spmi_transaction = 0;


/*===========================================================================

                        FUNCTION DEFINITIONS 

===========================================================================*/
pm_err_flag_type pm_comm_channel_init_internal(void)
{
    SpmiBus_ResultType   spmi_result = SPMI_BUS_SUCCESS;
    pm_err_flag_type     err = PM_ERR_FLAG_SUCCESS ;
    
	if(FALSE == spmi_initialized)
	{
        // initialize the SPMI BUS
        spmi_result = SpmiBus_Init();

        if(SPMI_BUS_SUCCESS  == spmi_result)
        {
            spmi_initialized = TRUE;

            //Dummy write to clear SPMI BUS
            err = pm_comm_write_byte(0,0x8D0,0x0,0);
        }
        else
        {
            CORE_VERIFY(0); //if spmi_bus init fails err_fatal
        }
    }
    return err;
}

pm_err_flag_type pm_comm_read_byte(uint32 slave_id, uint16 addr, uint8* data, uint8 priority)
{
    pm_err_flag_type     err = PM_ERR_FLAG_SUCCESS ;
    SpmiBus_ResultType   spmi_result = SPMI_BUS_SUCCESS;

    if((TRUE == spmi_initialized) && (addr <= PM_MAX_REGS))
    {
        spmi_result = SpmiBus_ReadLong(slave_id, (SpmiBus_AccessPriorityType)priority, addr, data, 1, NULL);
        num_spmi_transaction++;
        if ( spmi_result != SPMI_BUS_SUCCESS ) { err = PM_ERR_FLAG_BUS_ERR ; }
    }
    else
    {
        err = PM_ERR_FLAG_BUS_ERR ;
    }

    return err ;
}

pm_err_flag_type pm_comm_read_byte_mask(uint32 slave_id, uint16 addr, uint8 mask, uint8* data, uint8 priority)
{
    pm_err_flag_type     err = PM_ERR_FLAG_SUCCESS ;

    err = pm_comm_read_byte(slave_id, addr, data, priority);
    if (PM_ERR_FLAG_SUCCESS == err)
    {
        *data = *data & mask; 
    }
    else
    { 
        err = PM_ERR_FLAG_BUS_ERR ; 
    }

    return err ;
}

pm_err_flag_type pm_comm_write_byte(uint32 slave_id, uint16 addr, uint8 data, uint8 priority)
{
    pm_err_flag_type     err = PM_ERR_FLAG_SUCCESS ;
    SpmiBus_ResultType   spmi_result = SPMI_BUS_SUCCESS;

    if((TRUE == spmi_initialized) && (addr <= PM_MAX_REGS))
    {
        spmi_result = SpmiBus_WriteLong(slave_id, (SpmiBus_AccessPriorityType)priority, addr, &data, 1);
        num_spmi_transaction++;
        if ( spmi_result != SPMI_BUS_SUCCESS ) { err = PM_ERR_FLAG_BUS_ERR ; }
    }
    else
    {
        err = PM_ERR_FLAG_BUS_ERR ;
    }

    return err ;
}





pm_err_flag_type pm_comm_write_byte_mask(uint32 slave_id, uint16 addr, uint8 mask, uint8 data, uint8 priority)
{
    pm_err_flag_type         err = PM_ERR_FLAG_SUCCESS ;
    SpmiBus_ResultType       spmi_result = SPMI_BUS_SUCCESS;
    uint8                    byte_written;

    if((TRUE == spmi_initialized) && (addr <= PM_MAX_REGS))
    {
       /* Read the data from the SPMI and leave all the bits other than the mask to the previous state */
       spmi_result = SpmiBus_ReadModifyWriteLongByte(slave_id,(SpmiBus_AccessPriorityType)priority, addr, data, mask, &byte_written);
       if ( spmi_result != SPMI_BUS_SUCCESS )
       {
          err = PM_ERR_FLAG_BUS_ERR ; 
       }
       num_spmi_transaction += 2;
    }
    else
    {
        err = PM_ERR_FLAG_BUS_ERR ;
    }

    return err ;
}


pm_err_flag_type pm_comm_write_byte_array(uint32 slave_id, uint16 addr, uint32 num_bytes, uint8* data, uint8 priority) 
{
    pm_err_flag_type     err = PM_ERR_FLAG_SUCCESS ;
    SpmiBus_ResultType   spmi_result = SPMI_BUS_SUCCESS;

    if((TRUE == spmi_initialized) && (addr <= PM_MAX_REGS))
    {
        spmi_result = SpmiBus_WriteLong(slave_id, (SpmiBus_AccessPriorityType)priority, addr, data, num_bytes);
        num_spmi_transaction++;
        if ( spmi_result != SPMI_BUS_SUCCESS ) { err = PM_ERR_FLAG_BUS_ERR ; }
    }
    else
    {
        err = PM_ERR_FLAG_BUS_ERR ;
    }

    return err ;
}

pm_err_flag_type pm_comm_read_byte_array (uint32 slave_id, uint16 addr, uint32 num_bytes, uint8* data, uint8 priority) 
{
    pm_err_flag_type     err = PM_ERR_FLAG_SUCCESS ;
    SpmiBus_ResultType   spmi_result = SPMI_BUS_SUCCESS;

    if((TRUE == spmi_initialized) && (addr <= PM_MAX_REGS))
    {
        spmi_result = SpmiBus_ReadLong(slave_id, (SpmiBus_AccessPriorityType)priority, addr, data, num_bytes, NULL);
        num_spmi_transaction++;
        if ( spmi_result != SPMI_BUS_SUCCESS ) { err = PM_ERR_FLAG_BUS_ERR ; }
    }
    else
    {
        err = PM_ERR_FLAG_BUS_ERR ;
    }

    return err ;
}


pm_err_flag_type pm_comm_spmi_irq_clear_all(void)
{
  pm_err_flag_type     err            = PM_ERR_FLAG_SUCCESS;
  SpmiBus_ResultType   spmi_result    = SPMI_BUS_SUCCESS;

  spmi_result = SpmiIntrCtlr_InterruptClearAll();
                          
  if (spmi_result != SPMI_BUS_SUCCESS)  
  { 
    err = PM_ERR_FLAG_BUS_ERR ; 
  }

  return err ;
}


uint32
pm_get_num_spmi_transaction(uint32 initial_num_spmi_transaction) 
{
   uint32 num_spmi_trans_delta = num_spmi_transaction - initial_num_spmi_transaction;

   return num_spmi_trans_delta;
}


pm_err_flag_type  
pm_comm_write_byte_array_dbg( uint32 slave_id, uint16 addr, uint32 numBytes, uint8* data, uint8 priority ) 
{
    pm_err_flag_type     err         = PM_ERR_FLAG_SUCCESS ;
    SpmiBus_ResultType   spmi_result = SPMI_BUS_SUCCESS;

    if ((spmi_initialized != TRUE) || (addr > PM_MAX_REGS))
    {
        return PM_ERR_FLAG_BUS_ERR;
    }

    spmi_result = SpmiBus_WriteLongDebug ( slave_id, 
                            (SpmiBus_AccessPriorityType)priority, 
                            addr, 
                            data, 
                            numBytes );

    if (SPMI_BUS_SUCCESS == spmi_result)  
    { 
        return err; 
    }
    spmi_result = SpmiBus_WriteLong(slave_id, 
                         (SpmiBus_AccessPriorityType)priority, 
                          addr, 
                          data, 
                          numBytes);

    if (SPMI_BUS_SUCCESS != spmi_result)  
    { 
        err = PM_ERR_FLAG_BUS_ERR ; 
    }

    return err ;
}


pm_err_flag_type pm_comm_read_byte_array_dbg( uint32 slave_id, uint16 addr, uint32 numBytes, uint8* data, uint8 priority ) 
{
    pm_err_flag_type     err            = PM_ERR_FLAG_SUCCESS;
    SpmiBus_ResultType   spmi_result    = SPMI_BUS_SUCCESS;

    if ((spmi_initialized != TRUE) || (addr > PM_MAX_REGS))
    {
        return PM_ERR_FLAG_BUS_ERR;
    }

    spmi_result = SpmiBus_ReadLongDebug (slave_id, 
                            (SpmiBus_AccessPriorityType)priority, 
                            addr, 
                            data, 
                            numBytes);

    if (SPMI_BUS_SUCCESS == spmi_result)  
    { 
        return err; 
    }
    spmi_result = SpmiBus_ReadLong(slave_id, 
                         (SpmiBus_AccessPriorityType)priority, 
                          addr, 
                          data, 
                          numBytes, 
                          NULL);

    if (SPMI_BUS_SUCCESS != spmi_result)  
    { 
        err = PM_ERR_FLAG_BUS_ERR; 
    }

    return err ;
}
