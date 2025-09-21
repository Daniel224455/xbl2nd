 /* ===========================================================================

                                 C C I D r i v e r

                                    (CCI_HAL)

                               D e f i n i t i o n s
 *//** @file CCI_HAL.c                                                                                                                                                                                                                                                                      
                                                                                                                                                                                                                                                                                                    
 @par EXTERNALIZED FUNCTIONS                                                                                                                                                                                           
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 @par INITIALIZATION AND SEQUENCING REQUIREMENTS                                                                                                                                                                       
 None                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                    
 Copyright (c) 2012 - 2017 Qualcomm Technologies, Inc. All Rights Reserved.                                                                                                                                                  
 Qualcomm Proprietary and Confidential.                                                                                                                                                                                                                                                             
 Export of this technology or software is regulated by the U.S. Government.                                                                                                                                                                                                                         
 Diversion contrary to U.S. law prohibited.                                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                                                                    
** =========================================================================== */                                                                                                                                        
                                                                                                                                                                                                                                                                                                   
/* ===========================================================================                                                                                                                                        
**                                                                                                                                                                                                                                                                                                 
** =========================================================================== */                                                                                                                                                                                                                  
                                                                                                                                                                                                                                                                                                   
/* ===========================================================================                                                                                                                                        
**                         INCLUDE FILES FOR MODULE                                                                                                                                                                                                                                                
**============================================================================ */                                                                                                                                                                                          
#include "CCI_HAL.h"
#include "CCI_HAL_HWIO.h"
#include "CCI_HAL_Util.h"
#include "CCIService.h"

/* ---------------------------------------------------------------------------                                                                                                                                        
** Macro Definitions                                                                                                                                                                                                                                                                            
** --------------------------------------------------------------------------- */                                                                                                                                                                                                                  

/* ---------------------------------------------------------------------------                                                                                                                                                                                                                   
** Local Object Definitions                                                                                                                                                                                                                                                                    
** --------------------------------------------------------------------------- */                                                                                                                                                                                                                   

struct _CCI CCICtxMemmory;
CCICTX CCI = &CCICtxMemmory;

/* --------------------------------------------------------------------------
** Global Object Definitions
** ----------------------------------------------------------------------- */

CCIHWDetaisType CCIInfo = { 
                              #include "CCI_HAL_PLATFORM_HWIO.h"
                          };


CCIStatus CCI_HAL_UEFI_Init(void)
{
    CCIStatus status = CCI_Status_OK;

    CCICtrlInit(CCI, &CCIInfo);

    return status;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCI_HAL_Read_I2C_Data
 *    DESCRIPTION     
 *    DEPENDENCIES    None                                           
 *    PARAMETERS      
 *    RETURN VALUE    
 *    SIDE EFFECTS    CCIStatus 
 * ------------------------------------------------------------------------ */
CCIStatus CCI_HAL_Read_I2C_Data(QueueInfo queueInfo, UINT8* buff, UINT32 len, CCIStatus* eStatus)
{
    UINT32 read_data = 0x0, i = 0; 

    if (!queueInfo || NULL == buff)
        return *eStatus = CCI_Status_BAD_PARAM;

    if(CCIInfo.mx_read_fifo_len < len || 0 == len) 
        return *eStatus = CCI_Status_BAD_PARAM;

    if (CCIInfo.max_num_of_i2c_queue <= queueInfo->masterId)
        return *eStatus = CCI_Status_BAD_PARAM;

    //check buf level
    if ((CCICtrl_GetI2CReadBufLevel(CCI, queueInfo->masterId) * CCIInfo.cciword_sz) < len)
        return *eStatus = CCI_Status_BAD_PARAM;

    read_data = CCICtrl_GetI2CReadData(CCI, queueInfo->masterId);

    // Trucate Slave Address
    read_data >>= 8;

    for(i = 0; i < 3 && i < len; i++)
    { 
        buff[i] = (UINT8)((read_data & (0x000000FF << i*8)) >> 8*i);
    }

    if(len > 3)
    { 
        UINT32 j = 0, iteration = 0;

        while(i < len)
        {
            if(0 == iteration)
            {
                read_data = CCICtrl_GetI2CReadData(CCI, queueInfo->masterId);
            }

            buff[i] = (UINT8)((read_data & (0x000000FF << j * 8)) >> 8 * j);

            iteration++;
            iteration %= 4;

            j++;
            j %= 4;
            i++;
        }
    }

    *eStatus = CCI_Status_OK;

    return *eStatus;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCI_HAL_SetFrequency
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCI_HAL_SetFrequency(CCIMasterType masterId, CameraSensorI2C_SpeedType efrequency)
{
    I2CFrequencyParams I2CBusParams = NULL; 

    if (CCIInfo.max_num_of_master <= masterId)
        return CCI_Status_BAD_PARAM;

    switch (efrequency)
    {
        case CAMSENSOR_I2C_100KHZ:
            I2CBusParams = &CCIInfo.cci_i2c_frequency_params[frqcy_100khz];
            break;
        case CAMSENSOR_I2C_400KHZ:
            I2CBusParams = &CCIInfo.cci_i2c_frequency_params[frqcy_400khz];
            break;
        case CAMSENSOR_I2C_1000KHZ:
            if (2 > CCIInfo.ni2c_paramsz)
                return CCI_Status_NO_RESOURCE;
            I2CBusParams = &CCIInfo.cci_i2c_frequency_params[frqcy_1000khz];
            break;
        default:
            return CCI_Status_BAD_PARAM;
    }
    // configures i2c bus according to user request
    CCICtrl_SetSCL(CCI, masterId, (I2CBusParams->THIGH << 16 | I2CBusParams->TLOW));
    CCICtrl_SetSDA0(CCI, masterId, (I2CBusParams->TSUSTO << 16 | I2CBusParams->TSUSTA));
    CCICtrl_SetSDA1(CCI, masterId, (I2CBusParams->THDDAT << 16 | I2CBusParams->THDSTA));
    CCICtrl_SetSDA2(CCI, masterId, (I2CBusParams->TBUF));
    CCICtrl_SetMiscCtl(CCI, masterId, ((I2CBusParams->HW_SCL_STRETCH_EN << 8) |
        ((I2CBusParams->HW_RDHLD << 4) | I2CBusParams->HW_TSP)));

     return CCI_Status_OK;
}


/* ---------------------------------------------------------------------------
 *    FUNCTION        CCI_HAL_get_queueInfo
 *    DESCRIPTION     sets pointer to QueueInfo struct 
 *    DEPENDENCIES    
 *    PARAMETERS
 *    RETURN VALUE    None
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCIStatus CCI_HAL_get_queueInfo(QueueInfo* queueInfo, CCICommandQueueType eQueueType)
{ 
    if (CCIInfo.max_num_of_queues <= eQueueType)
        return CCI_Status_NOT_SUPPORTED;
    *queueInfo = &CCIInfo.queue_info_list[eQueueType];
    return CCI_Status_OK;
}

/* ---------------------------------------------------------------------------
*    FUNCTION        CCI_HAL_Get_CTRLReg
*    DESCRIPTION     returns pointer to CCI Control registers 
*    DEPENDENCIES
*    PARAMETERS      None 
*    RETURN VALUE    returns pointer to CCI Control registers 
*    SIDE EFFECTS    None
* ------------------------------------------------------------------------ */
CCIHWCTRL CCI_HAL_Get_CTRLReg(void)
{ 
    return CCI->CCICMNCTRL;
}
