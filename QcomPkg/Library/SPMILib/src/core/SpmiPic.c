/**
 * @file:  SpmiPic.c
 *
 * Copyright (c) 2013-2017 by Qualcomm Technologies Incorporated. All Rights Reserved.
 *
 * $Change: 30389851 $
 */


#include "SpmiPic.h"
#include "PmicArb.h"
#include "SpmiOs.h"
#include "SpmiLogs.h"

typedef struct _SpmiBusIsrConfig
{
    //SpmiIsr isr;
    const void* ctx;
    uint8  bid;
    uint16 chan;
    uint8  mask;
    uint16 id;
    struct _SpmiBusIsrConfig* next;
} SpmiBusIsrConfig;


//******************************************************************************
// Local Helper Functions
//******************************************************************************


static boolean isValidChannel(uint8 *base_addr, uint16 channel)
{
    uint8 sid = 0;
    uint8 addr= 0;
    uint32 regChan = 0;

    regChan = SPMI_HWIO_IN( HWIO_PMIC_ARB_REG_ADDRp_ADDR( base_addr, channel));
    sid = (uint8)SPMI_HWIO_GET_FIELD_VALUE(regChan, HWIO_PMIC_ARB_REG_ADDRp_SID);
    addr = (uint8)SPMI_HWIO_GET_FIELD_VALUE(regChan, HWIO_PMIC_ARB_REG_ADDRp_ADDRESS);

    if ((addr == 0) && (sid == 0)) {
       return FALSE;
    }
    return TRUE;
}

static boolean isInterruptOwner(SpmiDevInfo *pDev, uint8 owner, uint16 channel)
{
    ChannelMapEntry* entry = &pDev->channelMap[channel];

    if ((owner == entry->owner) && (entry->irqOwnerFlag == 0x1)) {

        return TRUE;
    }

    return FALSE;
}

//******************************************************************************
// Public API Functions
//******************************************************************************

Spmi_Result SpmiPic_Init(SpmiDevInfo *pDev, uint8 owner)
{
    uint32 i = 0;
    uint8 *base_addr = pDev->baseAddrs;

    SpmiPic_InterruptClearAll(pDev, owner);

    for(i = 0; i < pDev->pGenerics->mNumChannels; i++)
    {
        if (isValidChannel(base_addr, (uint16)i)) {
           SpmiPic_SetInterruptEnabled(pDev, owner, (uint16)i, FALSE);
        }
    }

    return SPMI_SUCCESS;
}

Spmi_Result SpmiPic_IsInterruptEnabled(SpmiDevInfo *pDev, uint16 channel, boolean* enabled)
{
    uint8 * base_addr = pDev->baseAddrs;

    *enabled = SPMI_HWIO_IN_FIELD( HWIO_SPMI_PIC_ACC_ENABLEn_ADDR( base_addr, channel ),
                                   HWIO_SPMI_PIC_ACC_ENABLEn_INT_ACC_ENABLE );

    return SPMI_SUCCESS;
}

Spmi_Result SpmiPic_ExtendedInterruptStatus(SpmiDevInfo *pDev, uint16 channel, uint32* statusMask)
{
    uint8 * base_addr = pDev->baseAddrs;
    *statusMask = SPMI_HWIO_IN_FIELD( HWIO_SPMI_PIC_IRQ_STATUSn_ADDR( base_addr, channel ),
                                      HWIO_SPMI_PIC_IRQ_STATUSn_INT_STATUS );

    return SPMI_SUCCESS;
}




Spmi_Result SpmiPic_SetInterruptEnabled(SpmiDevInfo *pDev, uint8 owner, uint16 channel, boolean enabled)
{
    uint8 * base_addr = pDev->baseAddrs;   
    
    if(!isInterruptOwner( pDev, owner, channel )) {
        return SPMI_FAILURE_COMMAND_NOT_ALLOWED;
    }

    if(channel >= SPMI_NUM_CHANNELS_SUPPORTED(pDev)) {
       return SPMI_FAILURE_INVALID_CHAN_NUM;
    }

    SPMI_LOG_VERBOSE( "ISR enabled: %d, Bus addr: 0x%p, Chan: %d, ", enabled, base_addr, channel );

    SPMI_HWIO_OUT_FIELD( HWIO_SPMI_PIC_ACC_ENABLEn_ADDR( base_addr, channel ),
                         HWIO_SPMI_PIC_ACC_ENABLEn_INT_ACC_ENABLE,
                         enabled );

    return SPMI_SUCCESS;
}

Spmi_Result SpmiPic_InterruptClear(SpmiDevInfo *pDev, uint16 channel, uint32 statusMask)
{
    uint8 * base_addr = pDev->baseAddrs;
    if(channel > pDev->pGenerics->mNumChannels) {
        return SPMI_FAILURE_INVALID_CHAN_NUM;
    }

    //uint8 mask = SPMI_HWIO_IN_FIELD( HWIO_SPMI_PIC_IRQ_STATUSn_ADDR( base_addr, channel ),
    //                                             HWIO_SPMI_PIC_IRQ_STATUSn_INT_STATUS );

    SPMI_HWIO_OUT_FIELD( HWIO_SPMI_PIC_IRQ_CLEARn_ADDR( base_addr, channel ),
                         HWIO_SPMI_PIC_IRQ_CLEARn_INT_CLEAR, statusMask );

    return SPMI_SUCCESS;
}

Spmi_Result SpmiPic_InterruptClearAll(SpmiDevInfo *pDev, uint8 owner)
{
    uint32 i = 0;
    uint32 k = 0;
    uint8 *base_addr = pDev->baseAddrs;

    for(i = 0; i < SPMI_NUM_PIC_ACC_STATUS_REGS(pDev); i++)
    {
        // Each bit in these registers represents a peripheral
        uint32 accStatus = SPMI_HWIO_IN( HWIO_SPMI_PIC_OWNERm_ACC_STATUSn_ADDR( base_addr, owner, i ) );

        for(k = 0; (k < 32) && (accStatus != 0); k++)
        {
            if((accStatus >> k) & 0x1)
            {
                // This peripheral (channel) triggered the interrupt.  Check its individual mask
                uint16 channel = (uint16)((i * 32) + k);
                uint8 mask = SPMI_HWIO_IN_FIELD( HWIO_SPMI_PIC_IRQ_STATUSn_ADDR( base_addr, channel ),
                                                 HWIO_SPMI_PIC_IRQ_STATUSn_INT_STATUS );

                // Clear the status
                SPMI_HWIO_OUT_FIELD( HWIO_SPMI_PIC_IRQ_CLEARn_ADDR( base_addr, channel ),
                                     HWIO_SPMI_PIC_IRQ_CLEARn_INT_CLEAR, mask );
            }
        }
    }

    return SPMI_SUCCESS;
}


