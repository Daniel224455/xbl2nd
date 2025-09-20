 /*! @file pm_fg_sram.c
 *
 * PMIC Battery Profile SRAM/OTP access functionalities 
 *
 * Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved. 
 * Qualcomm Technologies Proprietary and Confidential.
 */

/*=============================================================================
                              EDIT HISTORY


 when          who     what, where, why
 --------   ---     -----------------------------------------------------------
 12/22/17   pxm     Add SRAM access DMA support
 08/22/17   czq     Detect the bad conditions of FG SRAM (trim registers) and issue VDD hard reset on PM/PMi to recover
 07/10/17   pxm     Add function PmicFgSram_ImaWriteSingle
 11/01/16   va      Updated new IMA clear sequence and clear IMA by default 
 09/13/16   aab     Updated logging text
 08/24/16   aab     Updated PmicFgSram_Init()
 07/11/16   aab     Created to support PMI8998
=============================================================================*/
#include "pm_fg_sram.h"
#include "pm_fg_mem_if.h"
#include "pm_target_information.h"
#include "pm_app_chgr.h"
#include "pm_utils.h"
#include "CoreVerify.h"
#include "pm_log_utils.h"
#include "smem.h"
#include "pm_app_pon.h"


/*===========================================================================
                               MACROS
===========================================================================*/
#define FG_MEM_AVAILABLE_RT_STS_POLL_MIN_TIME 30    //in us
#define FG_MEM_AVAILABLE_RT_STS_POLL_MAX_TIME 249990  // in us (~10 ms)
#define FG_SRAM_PROFILE_INTEGRITY_CHK_ADDR    299  //refer MDOS for address, data values
#define FG_SRAM_PROFILE_INTEGRITY_OFFSET      0
#define FG_SRAM_PROFILE_LOAD_BY_SOFTWARE_MASK PM_BIT(0)
#define PM_FG_NO_OF_IMA_RETRIES               3
#define PM_FG_NUM_OF_DMA_ERR_STS_POLL         3
#define FG_SRAM_IMA_EXCP_ERR_HANDLING_MAX_CNT 5

#define FG_SRAM_SP_UN_USED_ADDR   20 /* Un used by SW and HW address of SRAM */ 
#define FG_SRAM_SP_UN_USED_ZERO   0x0  /* If reg is zeroed */

#define FG_SRAM_PAUSE_DELAY                   30
#define FG_SRAM_PAUSE_DELAY_MAX_TIME          9990 // in us (~10 ms)

/*=========================================================================
                            GLOBAL VARIABLES
===========================================================================*/
static fg_sram_state_type SramState = FG_SRAM_STATUS_INVALID;

/*===========================================================================
                               TYPE DEFINITIONS
===========================================================================*/



/*==========================================================================
                        LOCAL API PROTOTYPES
===========================================================================*/

fg_sram_state_type pm_fg_sram_get_state(void);
pm_err_flag_type pm_fg_sram_ima_read_single_offset(uint32 pmic_index, uint16 address, uint8 *data, uint8 offset);
pm_err_flag_type pm_fg_sram_ima_read_single(uint32 pmic_index, uint16 address, uint16 *fg_memif_data, boolean *beat_count_match);
pm_err_flag_type pm_fg_sram_request_fg_sram_access(uint32  pmic_index, pm_fg_mem_if_mem_intf_cfg  mem_intf_cfg);
pm_err_flag_type pm_fg_sram_release_ima_access (uint32  pmic_index, boolean *exception_occurred);
pm_err_flag_type pm_fg_sram_handle_if_ima_exception(uint32  pmic_index, boolean *exception_occurred);
pm_err_flag_type pm_fg_sram_set_state(fg_sram_state_type state);
pm_err_flag_type pm_fg_sram_reset_ima(uint32 pmic_index);
pm_err_flag_type pm_fg_sram_poll_ima_ready(uint32 pmic_index, boolean *ima_accessible);
pm_err_flag_type pm_fg_sram_content_verify(uint32 pmic_index, boolean *unexpected_match);
pm_err_flag_type pm_fg_get_dma_address(uint16 sram_address, uint16 *addr, uint16* max_bytes);
pm_err_flag_type pm_fg_sram_clear_dma_log(uint32 pmic_index);
pm_err_flag_type pm_fg_sram_release_dma_access(uint32 pmic_index);
pm_err_flag_type pm_fg_sram_poll_dma_ready(uint32 pmic_index, boolean *dma_accessible);
pm_err_flag_type pm_fg_sram_dma_read(uint32 pmic_index, uint16 address, uint16 *data, uint16 length);
pm_err_flag_type pm_fg_sram_dma_read_single_offset(uint32 pmic_index, uint16 address, uint8 *data, uint8 offset);
pm_err_flag_type pm_fg_sram_dma_write(uint32 pmic_index, uint16 address, uint16 data);
pm_err_flag_type pm_fg_sram_dma_write_single_offset(uint32 pmic_index, uint16 address, uint8 data, uint8 offset);
pm_err_flag_type pm_fg_sram_release_fg_sram_dma_access (uint32  pmic_index);


/*==========================================================================
                        GLOBAL API DEFINITION
===========================================================================*/

/*
 * This SRAM partition to DMA address partition mapping remains identical for
 * PMICs that use GEN5 FG.
 */
static pm_fg_dma_partition_type pm_fg_gen4_addr_map[FG_SRAM_DMA_PARTITIONS] = {

	{
		.partition_start = 0, // include
		.partition_end = 64, // exclude
		.spmi_addr_base = 0x4400,
	},

	{
		.partition_start = 64, // include
		.partition_end = 170, // exclude
		.spmi_addr_base = 0x4500,
	},

	{
		.partition_start = 170, // include
		.partition_end =  275, // exclude
		.spmi_addr_base = 0x4600,
	},

	{
		.partition_start = 275, // include
		.partition_end = 300, // exclude
		.spmi_addr_base = 0x4700,
	},
	
	{
		.partition_start = 300, // include
		.partition_end = 406, // exclude
		.spmi_addr_base = 0x4800,
	},
	
	{
		.partition_start = 406, // include
		.partition_end =  480, // exclude
		.spmi_addr_base = 0x4900,
	}

};

/**
pm_fg_sram_init()

@brief
Initializes Sram State
*/
pm_err_flag_type 
pm_fg_sram_init(uint32 pmic_index, pm_fg_mem_if_memory_access_slct_type access_type)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    boolean exception = FALSE;

    err_flag = pm_fg_sram_clear_dma_log(pmic_index);
    if (err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    if(FG_MEMORY_ACCESS_SLCT_IMA == access_type)
    {
        err_flag = pm_fg_sram_reset_ima(pmic_index);
    }

    SramState = FG_SRAM_STATUS_INIT;

    err_flag |= pm_fg_sram_release_ima_access(pmic_index, &exception);
    if(exception == TRUE)
    {
        pm_log_message("IMAException -IMA seq cleared");
    }
    return err_flag;
}



/**
pm_fg_sram_clear_dma_log()

@brief
Clears DMA Log. Required as a work around routine to fix issue 
on PMI8998 v2.0 
*/
pm_err_flag_type 
pm_fg_sram_clear_dma_log(uint32 pmic_index)
{
   pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
   boolean dma_err_sts = FALSE;
   uint8 poll_count = 0;

   err_flag |= pm_fg_memif_get_dma_err_sts(pmic_index, &dma_err_sts); //Check for DMA error
   if (err_flag != PM_ERR_FLAG_SUCCESS)
   {
      return err_flag;
   }

   if (TRUE == dma_err_sts)
   {
      err_flag |= pm_fg_memif_dma_clear(pmic_index, TRUE); //Clear DMA
      for (poll_count=0; poll_count < PM_FG_NUM_OF_DMA_ERR_STS_POLL; poll_count++) //Poll for DMA err status
      {
         err_flag |= pm_fg_memif_get_dma_err_sts(pmic_index, &dma_err_sts);
         if ((dma_err_sts == FALSE) || (err_flag != PM_ERR_FLAG_SUCCESS))
         {
            break;
         }
      }

      if ((dma_err_sts == TRUE) || (err_flag != PM_ERR_FLAG_SUCCESS))
      {
         pm_log_message("ERROR, DMA Clear Log failed"); 
         return err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
      }
      err_flag |= pm_fg_memif_dma_clear(pmic_index, FALSE);//reset
   }
   return err_flag;
}

fg_sram_state_type pm_fg_sram_get_state()
{
    return SramState;
}


/**
pm_fg_sram_ima_read_single_offset()

@brief
Read 1 Offset byte from given Sram Address
*/
pm_err_flag_type 
pm_fg_sram_ima_read_single_offset(uint32 pmic_index, uint16 sram_address, uint8 *Data, uint8 offset)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint16 read_data = 0;

    if(NULL == Data
        || FG_SRAM_ADDRESS_IS_INVALID(sram_address)
        || offset >= NUM_BYTE_IN_SRAM_DATA)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    err_flag = pm_fg_sram_ima_read(pmic_index, sram_address, &read_data);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        pm_log_message("Single IMA Read Failed");
    }
    else
    {
        *Data = (uint8)(read_data >> (offset * NUM_BITS_IN_BYTE));
    }

    return err_flag;
}

pm_err_flag_type 
pm_fg_sram_ima_read(uint32 pmic_index, uint16 sram_address, uint16 *fg_memif_data)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    uint16 read_data = 0;
    boolean beat_count_match = FALSE;
    uint8 ima_retry_count = 0;

    if(NULL == fg_memif_data || FG_SRAM_ADDRESS_IS_INVALID(sram_address))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    do{
        /* Read Word*/
        err_flag = pm_fg_sram_ima_read_single(pmic_index, sram_address, &read_data, &beat_count_match);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            return err_flag;
        }

        if(TRUE == beat_count_match)
        {
            break;
        }
        else
        {
            ima_retry_count++; /* Upkeep counter */
            read_data = 0; /* Reset Input */
        }
    } while(ima_retry_count < PM_FG_NO_OF_IMA_RETRIES);

    if((ima_retry_count >= PM_FG_NO_OF_IMA_RETRIES ) || (beat_count_match == FALSE))
    {
        //pm_log_message("Single Ima Read FG Beat Count did not match Read Failure = %d \n\r", __FUNCTION__));
        //err_flag = EFI_DEVICE_ERROR;
        err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
    }
    else
    {
        /* Read is good and Assign Read Data */
        *fg_memif_data = read_data;
    }

    return err_flag;
}



/**
pm_fg_sram_ima_write_single()

@brief
Writes 4 bytes (uint32) in given Sram address using IMA handshake
*/
pm_err_flag_type pm_fg_sram_ima_write_single(uint32 pmic_index, uint16 address, uint16 fg_memif_data)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    boolean mem_available = FALSE;
    boolean exception_occurred = FALSE;
    uint32 exception_count = 0;

    do{
        /* Request FG_MEM_IF access i.e. RIF_MEM_ACCESS_REQ = 1 */
        err_flag = pm_fg_sram_request_fg_sram_access(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_MEM_ACCESS_REQ);

        /* Select IMA access type  */
        err_flag |= pm_fg_mem_if_set_mem_intf_cfg(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_IACS_SLCT, FG_MEMORY_ACCESS_SLCT_IMA);

        /* Poll for IMA readiness  */
        err_flag |= pm_fg_sram_poll_ima_ready(pmic_index, &mem_available);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            pm_log_message("IMA Polling Failed for initiating write xaction = %r \r\n", err_flag);
        }

        if(mem_available)
        {
            /* Request Single access */
            err_flag |= pm_fg_mem_if_set_mem_intf_ctl(pmic_index, PM_FG_MEMIF_MEM_INTF_CTL_WR_BURST, FG_MEMORY_ACCESS_INTF_CTL_MEM_ACS_SINGLE);

            /* Requet read access */
            err_flag |= pm_fg_mem_if_set_mem_intf_ctl(pmic_index, PM_FG_MEMIF_MEM_INTF_CTL_WR_EN, FG_MEMORY_ACCESS_INTF_CTL_WRITE_ACCESS);

            /* Write Location Address */
            err_flag |= pm_fg_mem_if_write_addr(pmic_index, address);

            err_flag |= pm_fg_mem_if_set_ima_byte_en_cfg(pmic_index, PM_FG_MEMIF_IMA_BYTE_EN_ALL, TRUE);

            /*855 todo need to rewrite Write Data */
            //err_flag |=  pm_fg_mem_if_write_data(pmic_index, fg_memif_data);

            mem_available = FALSE; /* Reset Var */

            /* Poll for IMA readiness  */
            err_flag = pm_fg_sram_poll_ima_ready(pmic_index, &mem_available);
            if(err_flag != PM_ERR_FLAG_SUCCESS)
            {
                pm_log_message("IMA Polling Failed = %r \r\n", err_flag);
            }

            if(!mem_available)
            {
                pm_log_message("IMA Polling Failed after write \r\n");
                /*Check Integrity of data by exception status */
                err_flag = pm_fg_sram_handle_if_ima_exception(pmic_index, &exception_occurred);
                if(exception_occurred)
                {
                    if(exception_count < FG_SRAM_IMA_EXCP_ERR_HANDLING_MAX_CNT)
                    {
                        pm_log_message("IMA Exception Retry to single Write Again here \r\n");
                        /* 3 trials before giving up burst read  use continue with 3 static count and update write addr */
                        pm_log_message("IMA Exception Retry CurrAddr = %d, exception_count = %d \r\n", __FUNCTION__, address, exception_count);
                        /* Write Location Address again to make sure we are re writing to same address where IMA exception occurred */
                        err_flag = pm_fg_mem_if_write_addr(pmic_index, address);
                        continue;
                    }
                    else
                    {
                        pm_log_message("IMA Exception Retry Exceeded exception_count = %d \r\n", exception_count);
                        err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;/* Failure report */
                        break;
                    }
                }
            }
        }
        else
        {
            pm_log_message("IMA Polling Failed before write \r\n");
        }
        /* Make sure FG_MEM_IF is available i.e. RIF_MEM_ACCESS_REQ = 0 */
        pm_fg_sram_release_ima_access(pmic_index, &exception_occurred);
    }while((exception_occurred == TRUE ) && (++exception_count < FG_SRAM_IMA_EXCP_ERR_HANDLING_MAX_CNT));

    return err_flag;
}



// TODO: Need to re-write if we want IMA still be supported
/**
pm_fg_sram_ima_read_single()

@brief
Reads 2 bytes (uint16) in given Sram address using IMA handshake
*/
pm_err_flag_type 
pm_fg_sram_ima_read_single(uint32 pmic_index, uint16 sram_address, uint16 *fg_memif_data, boolean *beat_count_match)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    boolean mem_available = FALSE;
    boolean exception_occured = FALSE;
    uint8 beat_cnt1 = 0, beat_cnt2 = 0;
    uint32 exception_cnt = 0;

    if(NULL == beat_count_match || NULL == fg_memif_data || FG_SRAM_ADDRESS_IS_INVALID(sram_address))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    *beat_count_match = FALSE;

    /* Request FG_MEM_IF access i.e. RIF_MEM_ACCESS_REQ = 1 */
    err_flag = pm_fg_sram_request_fg_sram_access(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_MEM_ACCESS_REQ);

    /* Select IMA access type  */
    err_flag |= pm_fg_mem_if_set_mem_intf_cfg(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_IACS_SLCT, FG_MEMORY_ACCESS_SLCT_IMA);

    /* Request Burst access */
    err_flag |= pm_fg_mem_if_set_mem_intf_ctl(pmic_index, PM_FG_MEMIF_MEM_INTF_CTL_WR_BURST, FG_MEMORY_ACCESS_INTF_CTL_MEM_ACS_SINGLE);

    /* Requet read access */
    err_flag |= pm_fg_mem_if_set_mem_intf_ctl(pmic_index, PM_FG_MEMIF_MEM_INTF_CTL_WR_EN, FG_MEMORY_ACCESS_INTF_CTL_READ_ACCESS);
    do
    {
        mem_available = FALSE;

        /* Poll for IMA readiness  */
        err_flag |= pm_fg_sram_poll_ima_ready(pmic_index, &mem_available);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            pm_log_message("ERROR: ImaRead Polling Failed = %d", err_flag);
        }

        if(mem_available)
        {
            /* Enable all byte read */
            err_flag |= pm_fg_mem_if_set_ima_byte_en_cfg(pmic_index, PM_FG_MEMIF_IMA_BYTE_EN_ALL, TRUE);

            err_flag |= pm_fg_mem_if_get_fg_beat_count(pmic_index, &beat_cnt1);

            /* Write Read Location Address */
            err_flag |=  pm_fg_mem_if_write_addr(pmic_index, sram_address);

            mem_available = FALSE; /* Reset Var */

            /* Poll for IMA readiness  */
            err_flag |= pm_fg_sram_poll_ima_ready(pmic_index, &mem_available);
            if(err_flag != PM_ERR_FLAG_SUCCESS)
            {
                pm_log_message("ImaRead Polling Failed = %d ", err_flag);
            }

            err_flag |= pm_fg_sram_handle_if_ima_exception(pmic_index, &exception_occured);
            if(exception_occured)
            {
                pm_log_message("ImaException Occured \n\r");
                if(exception_cnt < FG_SRAM_IMA_EXCP_ERR_HANDLING_MAX_CNT)
                {
                    pm_log_message("IMAException Retry to Read Again");
                    /* 3 trials before giving up burst read  use continue with 3 static count and update write addr */
                    pm_log_message("IMAException Retry ReadAddr = %d, exception_cnt = %d", sram_address, exception_cnt);
                    continue;
                }
                else
                {
                    pm_log_message("IMAException Retry Exceeded exception_cnt = %d ", exception_cnt);
                    err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
                    break;
                }
            }

            if(mem_available)
            {
                /* Read Data */
                err_flag |=  pm_fg_mem_if_read_data_reg(pmic_index, fg_memif_data);
                err_flag |= pm_fg_mem_if_get_fg_beat_count(pmic_index, &beat_cnt2);
                /*Check Integrity of data by exception status */
            }
            else
            {
                pm_log_message("IMA Polling Failed updating read address");
            }
        }
        else
        {
            pm_log_message("IMA Polling Failed for read action");
        }
    }while((exception_occured == TRUE ) && (++exception_cnt < FG_SRAM_IMA_EXCP_ERR_HANDLING_MAX_CNT));

    err_flag |= pm_fg_sram_release_ima_access(pmic_index,  &exception_occured);

    /* Update FG beat count for data read inegrity typically if beat count reads are not same then Redo the read again */
    *beat_count_match = (beat_cnt1 == beat_cnt2) ? TRUE : FALSE;

    return err_flag;
}


/**
pm_fg_sram_request_fg_sram_access()

@brief
Request Sram access, Sets memory access bit 
*/
pm_err_flag_type 
pm_fg_sram_request_fg_sram_access(uint32 pmic_index, pm_fg_mem_if_mem_intf_cfg  mem_intf_cfg)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    /*Enables RIF memory interface and the RIF Memory Access Mode.  1 */
    err_flag |= pm_fg_mem_if_set_mem_intf_cfg(pmic_index, mem_intf_cfg, FG_MEMORY_ACCESS_REQ_TRUE);

    //Set Sram module internal state 
    err_flag |= pm_fg_sram_set_state(FG_SRAM_STATUS_IN_USE);

    return err_flag;
}

/**
pm_fg_sram_release_ima_access()

@brief
Release Sram access, Clears memory access bit 
*/
pm_err_flag_type 
pm_fg_sram_release_ima_access (uint32  pmic_index, boolean *exception_occurred)
{
    pm_err_flag_type err_flag  = PM_ERR_FLAG_SUCCESS;

    if(!exception_occurred)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    /* After write completion clear access bit request i.e. RIF_MEM_ACCESS_REQ = 0 */
    err_flag |= pm_fg_mem_if_set_mem_intf_cfg(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_MEM_ACCESS_REQ, FG_MEMORY_ACCESS_REQ_FALSE);

    /* Handle if IMA exception  */
    err_flag |= pm_fg_sram_handle_if_ima_exception(pmic_index, exception_occurred);

    /* Make sure to enable All byte write again in case previosuly APIs changed */
    err_flag |= pm_fg_mem_if_set_ima_byte_en_cfg(pmic_index, PM_FG_MEMIF_IMA_BYTE_EN_ALL, TRUE);

    /* Request Single access */
    err_flag |=  pm_fg_mem_if_set_mem_intf_ctl(pmic_index, PM_FG_MEMIF_MEM_INTF_CTL_WR_BURST, FG_MEMORY_ACCESS_INTF_CTL_MEM_ACS_SINGLE);

    /* Set Sram module internal state */
    err_flag |= pm_fg_sram_set_state(FG_SRAM_STATUS_AVAILABLE);

    return err_flag;
}


/**
pm_fg_sram_handle_if_ima_exception()

@brief
handle IMA access exception 
*/
pm_err_flag_type 
pm_fg_sram_handle_if_ima_exception(uint32 pmic_index, boolean *exception_occurred)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_fg_memif_ima_sts fg_ima_sts ;

    boolean mem_exception_rt_sts = FALSE;
    boolean exception_sts = FALSE;

    if(!exception_occurred)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    DALSYS_memset(&fg_ima_sts, 0, sizeof(pm_fg_memif_ima_sts));

    *exception_occurred = FALSE;

    /* Handle DMA error if any */
    err_flag  = pm_fg_sram_clear_dma_log(pmic_index);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        return err_flag;
    }

    /* Handle if IMA exception  */
    err_flag |= pm_fg_mem_if_get_ima_sts(pmic_index, &fg_ima_sts);
    err_flag |= pm_fg_mem_if_irq_status(pmic_index, PM_FG_MEM_IF_IMA_XCP, PM_IRQ_STATUS_RT, &mem_exception_rt_sts);
    if(err_flag != PM_ERR_FLAG_SUCCESS)
    {
        pm_log_message("Error = %d bMemExceptRtSts = %d ", err_flag, mem_exception_rt_sts );
    }

    exception_sts = fg_ima_sts.excep_sts.iacs_err | fg_ima_sts.excep_sts.xct_type_err | fg_ima_sts.excep_sts.be_burstwr_warn |
          fg_ima_sts.excep_sts.data_rd_err | fg_ima_sts.excep_sts.data_wr_err | fg_ima_sts.excep_sts.addr_burst_wrap |
          fg_ima_sts.excep_sts.addr_stable_err;

    if(exception_sts || mem_exception_rt_sts)
    {
        *exception_occurred = TRUE;
        err_flag |= pm_fg_sram_reset_ima(pmic_index);
    }

    return err_flag;
}


/**
pm_fg_sram_set_state()

@brief
Set Sram State
*/
pm_err_flag_type 
pm_fg_sram_set_state(fg_sram_state_type state)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;

    if(FG_SRAM_STATUS_INVALID == SramState)
    {
        err_flag = PM_ERR_FLAG_PERIPHERAL_ERR; 
    }
    else
    {
        SramState = state;
    }

    return err_flag;
}


/**
pm_fg_sram_reset_ima()

@brief
Reset IMA sequence for clearing IMA exception status
*/
pm_err_flag_type pm_fg_sram_reset_ima(uint32 pmic_index)
{
  pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
  pm_fg_memif_ima_sts fg_ima_sts;
  uint32 pause_delay = 0;
  boolean exception_sts = FALSE;
  uint8 fg_memif_data = 0;

  pm_log_message("Trigger FG IMA Reset");

  /* Set AICS Clear */
  err_flag  = pm_fg_mem_if_set_ima_cfg_value(pmic_index, 0x14);
  err_flag |= pm_fg_mem_if_set_mem_intf_ctl_value(pmic_index, 0x00);
  err_flag |= pm_fg_mem_if_set_mem_intf_cfg_value(pmic_index, 0xA0);

  /* Pause for 40 us*/
  pm_busy_wait(FG_SRAM_PAUSE_DELAY);

  do
  {
    fg_memif_data = 0;
    err_flag |= pm_fg_mem_if_write_addr_msb(pmic_index, 0x00);
    err_flag |= pm_fg_mem_if_write_data1(pmic_index, fg_memif_data);
    err_flag |= pm_fg_mem_if_read_data_reg1(pmic_index, &fg_memif_data);

    /* Pause for 40 us*/
    pm_busy_wait(FG_SRAM_PAUSE_DELAY);
    err_flag |= pm_fg_mem_if_get_ima_sts(pmic_index, &fg_ima_sts);
    pause_delay += FG_SRAM_PAUSE_DELAY;
    if(pause_delay > FG_SRAM_PAUSE_DELAY_MAX_TIME)
    {
      pm_log_message("IMA Reset.Timeout pause_delay ");
      err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
      break;
    }

    exception_sts = fg_ima_sts.excep_sts.iacs_err | fg_ima_sts.excep_sts.xct_type_err | fg_ima_sts.excep_sts.be_burstwr_warn |
                    fg_ima_sts.excep_sts.data_rd_err | fg_ima_sts.excep_sts.data_wr_err | fg_ima_sts.excep_sts.addr_burst_wrap | fg_ima_sts.excep_sts.addr_stable_err;

    fg_memif_data = 0;

    err_flag |= pm_fg_mem_if_get_ima_hw_sts(pmic_index, &fg_memif_data);

  }while((err_flag == PM_ERR_FLAG_SUCCESS ) && ((exception_sts == TRUE ) || (fg_memif_data != 0x00)));

  /* Make sure to have Read access */
  err_flag  = pm_fg_mem_if_set_ima_cfg_value(pmic_index, 0x10);

  /* Make sure to have Single access */
  err_flag |= pm_fg_mem_if_set_mem_intf_ctl_value(pmic_index, 0x00);

  /* Pause for 40 us*/
  pm_busy_wait(FG_SRAM_PAUSE_DELAY);

  pm_log_message("Trigger FG IMA Reset.Completed");

  return err_flag;
}


pm_err_flag_type 
pm_fg_sram_poll_ima_ready(uint32 pmic_index, boolean *ima_accessible)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG_SUCCESS;
    pm_fg_memif_ima_sts fg_ima_sts;
    uint32 ttl_spent_time_in_polling = 0;
    boolean mem_available_status = FALSE;
    //boolean             bExceptionOccured = FALSE;

    if(NULL == ima_accessible)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    *ima_accessible = FALSE;

    //Set Sram module internal state 
    pm_fg_sram_set_state(FG_SRAM_STATUS_POLLING);

    //Poll FG_MEM_AVAIL_RT_STS = 1 
    do
    {
        /* mem available best time is 30us */
        err_flag  = pm_fg_mem_if_get_ima_sts(pmic_index, &fg_ima_sts);
        err_flag |= pm_fg_mem_if_irq_status(pmic_index, PM_FG_MEM_IF_IMA_RDY, PM_IRQ_STATUS_RT, &mem_available_status);
        if(err_flag != PM_ERR_FLAG_SUCCESS)
        {
            //pm_log_message("PmicDxe: %a pm_fg_mem_if_get_ima_sts Error= %d \n\r", __FUNCTION__, err_flag));
            break;
        }
        if ((TRUE == fg_ima_sts.opr_sts.iacs_rdy) || (TRUE == mem_available_status))
        {
            *ima_accessible = TRUE;
            //PMIC_FILE_DEBUG("PmicDxe: %a PM_FG_MEM_IF_IMA_RDY \n\r", __FUNCTION__));
            break;
        }//check for error condition as we do not want to loop forever
        else if (ttl_spent_time_in_polling >= FG_MEM_AVAILABLE_RT_STS_POLL_MAX_TIME)
        {
            err_flag = PM_ERR_FLAG_PERIPHERAL_ERR;
            pm_log_message("TimeOut: ttl_spent_time_in_polling = %d ms ", ttl_spent_time_in_polling);
            break;
        }

        /*wait for 30 us before querying mem available status again */
        pm_busy_wait(FG_MEM_AVAILABLE_RT_STS_POLL_MIN_TIME); 
        ttl_spent_time_in_polling += FG_MEM_AVAILABLE_RT_STS_POLL_MIN_TIME;
    }while(TRUE);

    //Set Sram module internal state 
    if (FALSE == *ima_accessible)
    {
        //Clear memory access bit request i.e. RIF_MEM_ACCESS_REQ = 0
        //pm_fg_sram_release_ima_access(pmic_index, &bExceptionOccured);

        //might need to read cycle streach bit and clear it here
        pm_fg_sram_set_state(FG_SRAM_STATUS_AVAILABLE); //when time out for polling access request 
    }
    else 
    {
        pm_fg_sram_set_state(FG_SRAM_STATUS_IN_USE);
    }

    return err_flag;
}
 
 /*
 * Convert SRAM address to SPMI address for DMA access, and get the max bytes can be read consecutively on this SRAM address
 * @sram_addr: SRAM address
 * @dma_addr: spmi address used for DMA access
 * @max_bytes: the max bytes allow to read consecutively due to limitation of partition.
 * @return PM_ERR_FLAG_INVALID_PARAMETER: parameter error
 *		   PM_ERR_FLAG_FAILURE: failure. what's wrong?
 *         PM_ERR_FLAG__SUCCESS
 */
pm_err_flag_type pm_fg_get_dma_address(uint16 sram_addr, uint16 *dma_addr, uint16* max_bytes)
{
	uint32 partition_index;
	uint16 sram_start_addr, sram_end_addr;
	pm_fg_dma_partition_type partition;

	if(NULL == dma_addr || NULL == max_bytes
		|| sram_addr < pm_fg_gen4_addr_map[0].partition_start
		|| sram_addr >= pm_fg_gen4_addr_map[FG_SRAM_DMA_PARTITIONS - 1].partition_end)
	{
		return PM_ERR_FLAG_INVALID_PARAMETER;
	}

	for(partition_index = 0; partition_index < FG_SRAM_DMA_PARTITIONS; ++partition_index)
	{
		partition = pm_fg_gen4_addr_map[partition_index];
		sram_start_addr = partition.partition_start;
		sram_end_addr = partition.partition_end;
		if(sram_addr >= sram_start_addr && sram_addr < sram_end_addr)
		{
			*dma_addr = (sram_addr - sram_start_addr) * NUM_BYTE_IN_SRAM_DATA + partition.spmi_addr_base;
			*max_bytes = (sram_end_addr - sram_addr) * NUM_BYTE_IN_SRAM_DATA;
			return PM_ERR_FLAG__SUCCESS;
		}
	}

	return PM_ERR_FLAG_FAILURE;
}

pm_err_flag_type pm_fg_sram_release_dma_access(uint32 pmic_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

    // set memory access request as FALSE
    err_flag = pm_fg_mem_if_set_mem_intf_cfg(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_MEM_ACCESS_REQ, FG_MEMORY_ACCESS_REQ_FALSE);
    
    err_flag |= pm_fg_memif_arb_ctl(pmic_index, PM_FG_MEMIF_ARB_REQ, FALSE);
    err_flag |= pm_fg_memif_arb_ctl(pmic_index, PM_FG_MEMIF_ARB_LO_LATENCY_EN, FALSE);
    
    /* Set Sram module internal state */
    err_flag |= pm_fg_sram_set_state(FG_SRAM_STATUS_AVAILABLE);

    return err_flag;
}

pm_err_flag_type pm_fg_sram_poll_dma_ready(uint32 pmic_index, boolean *dma_accessible)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    uint32 polling_time = 0, retry_cnt = 0;
    boolean mem_available = FALSE, memory_used = TRUE;

    if(NULL == dma_accessible)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    *dma_accessible = FALSE;

    while(TRUE == memory_used && (retry_cnt++ < FG_SRAM_MAX_RETRY))
    {
        err_flag |= pm_fg_mem_if_get_mem_intf_cfg(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_MEM_ACCESS_REQ, &memory_used);
        err_flag |= pm_fg_sram_release_dma_access(pmic_index);
    }

    if(TRUE == memory_used)
    {
        *dma_accessible = FALSE;
        pm_log_message("check dma accesible timeout");
        return err_flag;
    }

    /* Set DMA ctl as partition layout to access DMA partition */
    err_flag = pm_fg_memif_dma_ctl(pmic_index, PM_FG_MEMIF_DMA_CTL_PARTITION);

    /* Select DMA access type  */
    err_flag |= pm_fg_mem_if_set_mem_intf_cfg(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_IACS_SLCT, FG_MEMORY_ACCESS_SLCT_DMA);

    /* Request FG_MEM_IF access i.e. RIF_MEM_ACCESS_REQ = 1 */
    err_flag |= pm_fg_sram_request_fg_sram_access(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_MEM_ACCESS_REQ);

    /* Pause Mem Arb and set lo latency for mem arb and at the same time mem_clr_log is cleared.*/
    err_flag |= pm_fg_memif_arb_ctl(pmic_index, PM_FG_MEMIF_ARB_REQ, TRUE);
    err_flag |= pm_fg_memif_arb_ctl(pmic_index, PM_FG_MEMIF_ARB_LO_LATENCY_EN, TRUE);

    err_flag |= pm_fg_sram_set_state(FG_SRAM_STATUS_POLLING);

    //Poll FG_MEM_AVAIL_RT_STS = 1
    do
    {
        /* mem available best time is 30us */
        err_flag = pm_fg_mem_if_irq_status(pmic_index, PM_FG_MEM_IF_MEM_GNT, PM_IRQ_STATUS_RT, &mem_available);
        if(PM_ERR_FLAG__SUCCESS != err_flag)
        {
            return err_flag;
        }

        if(TRUE == mem_available)
        {
            *dma_accessible = TRUE;
            break;
        }
        else if(polling_time >= FG_MEM_AVAILABLE_RT_STS_POLL_MAX_TIME)
        {
            pm_log_message("Polling memory available timeout");
            break;
        }
        
        /*wait for 30 us before querying mem available status again */
        pm_busy_wait(FG_MEM_AVAILABLE_RT_STS_POLL_MIN_TIME);

        polling_time += FG_MEM_AVAILABLE_RT_STS_POLL_MIN_TIME;
    } while(TRUE);

    if(FALSE == *dma_accessible)
    {
        err_flag |= pm_fg_sram_set_state(FG_SRAM_STATUS_AVAILABLE);
    }
    else
    {
        err_flag |= pm_fg_sram_set_state(FG_SRAM_STATUS_IN_USE);
    }

    return err_flag;
}

pm_err_flag_type pm_fg_sram_dma_read(uint32 pmic_index, uint16 sram_address, uint16 *data, uint16 length)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    uint16 dma_address = 0, max_bytes = 0, retry_cnt = 0;
    boolean mem_available = FALSE;
    boolean dma_err_sts = FALSE;
    
    err_flag = pm_fg_get_dma_address(sram_address, &dma_address, &max_bytes);

    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        return err_flag;
    }

    /* Validate Input Params */
    if(NULL == data
      || dma_address > FG_SRAM_END_DMA_ADDR
      || dma_address < FG_SRAM_START_DMA_ADDR
      || max_bytes < length * sizeof(uint16))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }
    
    do {
        /* Poll for DMA readiness  */
        err_flag |= pm_fg_sram_poll_dma_ready(pmic_index, &mem_available);
        if(PM_ERR_FLAG__SUCCESS != err_flag)
        {
            return err_flag;
        }

        if (TRUE == mem_available)
        {
            err_flag = pm_fg_memif_dma_read_long(pmic_index, dma_address, (uint8*)data, sizeof(uint16) * length);
        }

        /* Make sure FG_MEM_IF is available i.e. RIF_MEM_ACCESS_REQ = 0 */
        pm_fg_sram_release_fg_sram_dma_access(pmic_index);

        pm_fg_memif_get_dma_err_sts(pmic_index, &dma_err_sts);
    } while(dma_err_sts && ++retry_cnt < FG_SRAM_MAX_RETRY);
    
    if(dma_err_sts)
    {
        err_flag = PM_ERR_FLAG_FAILURE;
    }
    
    return err_flag;
}

pm_err_flag_type pm_fg_sram_dma_read_single_offset(uint32 pmic_index, uint16 sram_address, uint8 *data, uint8 offset)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    uint16 read_value = 0;

    // parameter check will be done in pm_fg_sram_dma_read so we do NOT necessary check here.
    // read 1 word in specific sram address
    err_flag = pm_fg_sram_dma_read(pmic_index, sram_address, &read_value, 1);
    if(PM_ERR_FLAG__SUCCESS != err_flag)
    {
        return err_flag;
    }

    *data = (uint8)((read_value >> (offset * NUM_BITS_IN_BYTE)) & 0xFF);

    return err_flag;
}

pm_err_flag_type pm_fg_sram_dma_write(uint32 pmic_index, uint16 sram_address, uint16 data)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    uint16 dma_address = 0, max_bytes = 0, retry_cnt = 0;
    boolean mem_available = FALSE;
    boolean dma_err_sts = FALSE;

    err_flag = pm_fg_get_dma_address(sram_address, &dma_address, &max_bytes);
    /* Validate Input Params */
    if (dma_address > FG_SRAM_END_DMA_ADDR
      || dma_address < FG_SRAM_START_DMA_ADDR
      || max_bytes < sizeof(uint16))
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }
    
    do {
        /* Poll for DMA readiness  */
        err_flag |= pm_fg_sram_poll_dma_ready(pmic_index, &mem_available);
        if(PM_ERR_FLAG__SUCCESS != err_flag)
        {
            return err_flag;
        }

        if(TRUE == mem_available)
        {
            err_flag = pm_fg_memif_dma_write_long(pmic_index, dma_address, (uint8*)(&data), sizeof(uint16));
        }

        /* Make sure FG_MEM_IF is available i.e. RIF_MEM_ACCESS_REQ = 0 */
        pm_fg_sram_release_fg_sram_dma_access(pmic_index);

        pm_fg_memif_get_dma_err_sts(pmic_index, &dma_err_sts);
    } while(dma_err_sts && ++retry_cnt < FG_SRAM_MAX_RETRY);
    
    if (dma_err_sts)
    {
        err_flag = PM_ERR_FLAG_FAILURE;
    }

    return err_flag;
}

pm_err_flag_type pm_fg_sram_dma_write_single_offset(uint32 pmic_index, uint16 sram_address, uint8 data, uint8 offset)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;
    uint16 read_data = 0, write_data = 0, mask = 0;

    if(offset >= NUM_BYTE_IN_SRAM_DATA)
    {
        return PM_ERR_FLAG_INVALID_PARAMETER;
    }

    err_flag = pm_fg_sram_dma_read(pmic_index, sram_address, &read_data, 1);
    if(PM_ERR_FLAG__SUCCESS == err_flag)
    {
        mask = 0xFF << (offset * NUM_BITS_IN_BYTE);
        write_data = (read_data & ~mask) | (data << (offset * NUM_BITS_IN_BYTE));
        err_flag = pm_fg_sram_dma_write(pmic_index, sram_address, write_data);
    }

    return err_flag;
}

pm_err_flag_type pm_fg_sram_release_fg_sram_dma_access(uint32 pmic_index)
{
    pm_err_flag_type err_flag = PM_ERR_FLAG__SUCCESS;

    /* After write completion clear access bit request i.e. RIF_MEM_ACCESS_REQ = 0 */
    err_flag = pm_fg_mem_if_set_mem_intf_cfg(pmic_index, PM_FG_MEMIF_MEM_INTF_CFG_MEM_ACCESS_REQ, FG_MEMORY_ACCESS_REQ_FALSE);
    err_flag |= pm_fg_memif_arb_ctl(pmic_index, PM_FG_MEMIF_ARB_REQ, FALSE);
    err_flag |= pm_fg_memif_arb_ctl(pmic_index, PM_FG_MEMIF_ARB_LO_LATENCY_EN, FALSE);

    /* Set Sram module internal state */
    err_flag |= pm_fg_sram_set_state(FG_SRAM_STATUS_AVAILABLE);

    return err_flag;
}

