/** @file CCICmdBuilder.c

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc. All Rights Reserved.
Qualcomm Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

**/

#include "CCI_HAL_HWIO.h"
#include "CCI_HAL.h"
#include "CCICmdBuilder.h"
#include "CCIResourceManager.h"
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>


/*===========================================================================
                            EXTERNAL API DECLARATIONS
=========================================================================== */
/* ---------------------------------------------------------------------------
*    FUNCTION        CCICmdBuilder_Init
*    DESCRIPTION     Initializes CCICmdBuilder
*    DEPENDENCIES
*    PARAMETERS
*    RETURN VALUE    CCIStatus
*    SIDE EFFECTS    None
* ------------------------------------------------------------------------ */

CCIStatus CCICmdBuilder_Init(CCICmdBuilder* self, CCICapability* capability)
{
    self->cmd.nSz = 0;
    self->cmd.idx = 0;
    /// copies cci capability to CCICmdbuilder Object
   // std_memcpy_s(&self->capability, sizeof(CCICapability), capability, sizeof(CCICapability));
    CopyMem(&self->capability,capability,sizeof(CCICapability));
    return self->state = CCI_Status_OK;
}

/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICmd_BuildI2CWrite
 *    DESCRIPTION     Configures I2C write commands 
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCICmd* CCICmdBuilder_I2CWrite(CCICmdBuilder* self, CCII2CConfig* config)
{ 
    const UINTN reg_sz = sizeof(self->cmd.ccmd[0]);
    UINTN idx = 0, nbytecnt = config->nregsz; 
    UINT8* buff = (UINT8*)self->cmd.ccmd;
    ZeroMem(self->cmd.ccmd,sizeof(self->cmd.ccmd));
    self->cmd.ccmd[0] = (config->brptstart || 0 == config->ndatasz) ? CCI_HAL_CMD_I2C_WRITE_DP : CCI_HAL_CMD_I2C_WRITE;

    /// if register address is needed to be part of i2c write transaction
    if (0 != config->nregsz)
    {
        buff[1]      = (UINT8)(config->nregaddr >> 8 * (config->nregsz - 1));
        // second byte is don't care if register size is 1
        buff[config->nregsz] = (UINT8)(config->nregaddr);
        // advances poiter for next operation
        buff += config->nregsz;
    }

    ++buff;

    while (nbytecnt < config->max_write_cnts && config->ndatasz > 0)
    {
        if (0 == (nbytecnt + 1) % reg_sz)
        {
            buff = ((UINT8*)&self->cmd.ccmd[++idx]);
        }

        *buff++ = *config->pdata++;
        ++nbytecnt;
        --config->ndatasz;
        ++config->nregaddr;
    }
    nbytecnt = (config->bcontinuous && config->ndatasz) ? 0x0F : nbytecnt;
    self->cmd.ccmd[0] |= (nbytecnt << 4);
    self->cmd.idx = 0;
    self->cmd.nSz = idx + 1;
    self->state = CCI_Status_OK;
    return &self->cmd;
    //end of looop
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICmd_BuildI2CLock
 *    DESCRIPTION     populates cmd with lock command 
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCICmd* CCICmdBuilder_I2CLock(CCICmdBuilder* self)
{
    return ccicmd_init_with_value(&self->cmd, CCI_HAL_CMD_I2C_LOCK);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICmd_BuildI2CUnlock
 *    DESCRIPTION     populates cmd with lock command 
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCICmd* CCICmdBuilder_I2CUnlock(CCICmdBuilder* self)
{
    return ccicmd_init_with_value(&self->cmd, CCI_HAL_CMD_I2C_UNLOCK);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCI_HAL_I2CRead
 *    DESCRIPTION     
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
 
CCICmd* CCICmdBuilder_I2CRead(CCICmdBuilder* self, UINT32 len)
{
    if (0 == len || (UINT16)self->capability.mx_read_trnsf_unt < len)
    {
        self->state = CCI_Status_BAD_PARAM;
        return NULL;
    }
    return ccicmd_init_with_value(&self->cmd, (len << 4) | CCI_HAL_CMD_I2C_READ);
}
/* ---------------------------------------------------------------------------
 *    FUNCTION        CCICmd_BuildI2CSetParam
 *    DESCRIPTION     populates cmd with set param command 
 *    DEPENDENCIES    
 *    PARAMETERS      
 *    RETURN VALUE    CCIStatus 
 *    SIDE EFFECTS    None
 * ------------------------------------------------------------------------ */
CCICmd* CCICmdBuilder_I2CSetParam(CCICmdBuilder* self, UINT8 ncid, UINT8 nslaveaddr)
{
    CCISetparamCmdType setparam;

    if (self->capability.mx_cid <= ncid || 0xFF < nslaveaddr)
    {
        self->state = CCI_Status_BAD_PARAM;
        return NULL;
    }

    setparam.data = 0;
    // sets up a setparam command                
    setparam.bits.ncid = ncid;
    setparam.bits.nSlave = nslaveaddr >> 1;
    setparam.bits.id = CCI_HAL_CMD_SET_PARAM; 
 
    return ccicmd_init_with_value(&self->cmd, setparam.data);
}
