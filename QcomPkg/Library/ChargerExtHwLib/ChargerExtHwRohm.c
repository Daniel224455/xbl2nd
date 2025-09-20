/**
@file  ChargerExtHwRohm.c
@brief Implementation of ChargerExtHwLib API for Rohm Charger module
*/
/*=============================================================================
Copyright (c) 2017 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================

                    EDIT HISTORY

when       who     what, where, why
--------   ---     -----------------------------------------------------------
08/10/17   mr      Initial Release
=============================================================================*/

/*===========================================================================*/
/*                  INCLUDE FILES                                            */
/*===========================================================================*/
#include <Uefi.h>

/**
  Library Dependencies
*/
#include <Library/UefiBootServicesTableLib.h>

/**
  Protocol Dependencies
*/
#include <Protocol/EFITlmm.h>

/**
  Internal Dependencies
*/
#include "ChargerExtHwLibConfig.h"
#include "RohmBD99954.h"

/*===========================================================================*/
/*                  TYPEDEF AND MACRO DEFINITIONS                            */
/*===========================================================================*/


/*===========================================================================*/
/*                  FUNCTIONS PROTOTYPES                                     */
/*===========================================================================*/
EFI_STATUS Rohm_PreInit(CHARGER_EXT_HW_EXT_HW_DEP *RohmExtDep);

/* Local API to 'write with mask' for Rohm Charger */
i2c_status Rohm_i2c_write_mask(UINT8 offset, UINT16 mask, UINT16 data_in, UINT16 size_in);

/* Rohm Charger Functionalities */
EFI_STATUS Rohm_GetVbusDetect(BOOLEAN *bVbusDetect);
EFI_STATUS Rohm_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType);
EFI_STATUS Rohm_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA);


/*===========================================================================*/
/*                  LOCAL VARIABLE DECLARATIONS                              */
/*===========================================================================*/
static ChargerExtHw_Device gRohmDevice = {0};

CHARGER_EXT_HW_FUNC gRohmFunc =
{
    Rohm_GetVbusDetect,
    Rohm_GetHSUsbPortType,
    Rohm_SetSnkMaxInputCurrent
};


/*===========================================================================*/
/*                  FUNCTIONS DEFINITIONS                                    */
/*===========================================================================*/
/**
  Initialize the interface to External Rohm Charger module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
  EFI_UNSUPPORTED:              The physical device does not support this function
*/
EFI_STATUS
ChargerExtHwLibRohm_Init(CHARGER_EXT_HW_INFO *pHwInfo)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 rohm_sel_cmd_set = 0;

    Status = Rohm_PreInit(pHwInfo->ChgHwExtDep);
    if(EFI_ERROR(Status)) {return Status;}

    gRohmDevice.hI2CHandle = NULL;
    gRohmDevice.I2CInstance = pHwInfo->ChgrExtHwIntf.Index;

    gRohmDevice.I2CSlaveCfg.bus_frequency_khz = 400;  /* 400 KHz */
    gRohmDevice.I2CSlaveCfg.slave_address = pHwInfo->ChgrExtHwIntf.Address;
    gRohmDevice.I2CSlaveCfg.mode = I2C;
    gRohmDevice.I2CSlaveCfg.slave_max_clock_stretch_us = 0;
    gRohmDevice.I2CSlaveCfg.core_configuration1 = 0;
    gRohmDevice.I2CSlaveCfg.core_configuration2 = 0;

    gRohmDevice.I2CSlaveRegMap = ROHM_CMD_SIZE;

    gRohmDevice.TimeoutMs = 0;

    i2c_sts = i2c_open(gRohmDevice.I2CInstance, &gRohmDevice.hI2CHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a i2c_open Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    /* Detect H/w revision/version and do specific initialization */
    pHwInfo->Func = &gRohmFunc;

    /* Any Charger Init sequence at Boot-up */
    rohm_sel_cmd_set = 0x1;  /* Select EXTENDED Command Set */
    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_CMD_SEL_MAP_SET, (UINT8 *)&rohm_sel_cmd_set, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a CMD_SEL_MAP set Failed \r\n", __FUNCTION__));
        return EFI_DEVICE_ERROR;
    }

    CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a SUCCESS \r\n", __FUNCTION__));

    return Status;
}

/**
  De-initialize the interface to Rohm Charger module

  @param none

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS
ChargerExtHwLibRohm_Deinit()
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;

    i2c_sts = i2c_close(gRohmDevice.hI2CHandle);
    if(I2C_SUCCESS != i2c_sts)
    {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

/**
  Get Vbus Detect result from Rohm Charger Module

  @param[OUT]   bVbusDetect     Return the result of Vbus Detect

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_GetVbusDetect(BOOLEAN *bVbusDetect)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if (bVbusDetect == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    /* VBus detect status here */
    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_VBUS_VCC_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

    *bVbusDetect = (VBUS_VCC_STS_VBUS_DET & data) ? TRUE : FALSE;
    /* For VBus valid, Check VBUS_UCD_STATUS <7>VBUS_VLD */

    return Status;
}

/**
  Get HSUSB Charger Port Type

  @param[OUT]   ChgPortType     Return Charger Port Type

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_GetHSUsbPortType(CHARGER_EXT_HW_HSUSB_CHGPORT_TYPE *ChgPortType)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if (ChgPortType == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, ROHM_EXT_CMD_VBUS_UCD_STATUS, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

    *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_UNKNOWN;

    if(VBUS_UCD_STS_PORT_SDP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_SDP;
    }
    else if(VBUS_UCD_STS_PORT_CDP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_CDP;
    }
    else if(VBUS_UCD_STS_PORT_DCP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = CHARGER_EXT_HW_HSUSB_CHGPORT_DCP;
    }
    else if(VBUS_UCD_STS_PORT_PULLUP == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = VBUS_UCD_STS_PORT_PULLUP;
    }
    else if(VBUS_UCD_STS_PORT_OPEN == (data & VBUS_UCD_STS_PORT_DET_MASK))
    {
        *ChgPortType = VBUS_UCD_STS_PORT_OPEN;
    }

    return Status;
}

/**
  Set Sink Max Input Current

  @param[IN]    MaxCurrentMA    Value to set Max Input Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_SetSnkMaxInputCurrent(UINT32 MaxCurrentMA)
{
    EFI_STATUS Status = EFI_SUCCESS;
    i2c_status i2c_sts = I2C_SUCCESS;
    UINT16 data = 0;

    if(MaxCurrentMA > IBUS_LIM_MAX)
        MaxCurrentMA = IBUS_LIM_MAX;

    data  = (MaxCurrentMA + (IBUS_LIM_STEP/2));
    data &= IBUS_LIM_SET_MASK;

    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, ROHM_EXT_CMD_IBUS_LIM_SET, (UINT8 *)&data, ROHM_DATA_SIZE);
    if(I2C_SUCCESS != i2c_sts) {return EFI_DEVICE_ERROR;}

    return Status;
}

/**
  Set Source Max Output Current

  @param[IN]    MaxCurrentMA    Value to set Max Output Current

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_SetSrcMaxOutputCurrent(UINT32 MaxCurrentMA)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /* Set Source max current here */

  return Status;
}

/**
  Enable/Disable Vbus

  @param[IN]    bVbusEn         Enable/Disable Vbus

  @return
  EFI_SUCCESS:                  Function returned successfully.
  EFI_INVALID_PARAMETER:        A Parameter was incorrect.
  EFI_DEVICE_ERROR:             The physical device reported an error.
*/
EFI_STATUS Rohm_SetVbusEn(UINT32 bVbusEn)
{
  EFI_STATUS Status = EFI_SUCCESS;

  /* Set VBus enable here */

  return Status;
}


/*===========================================================================*/
/*                  INTERNAL FUNCTIONS DEFINITIONS                           */
/*===========================================================================*/
EFI_STATUS Rohm_PreInit(CHARGER_EXT_HW_EXT_HW_DEP *RohmExtDep)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_TLMM_PROTOCOL *TLMMProtocol = NULL;
    UINT8 NumDep = 0;

    if(NULL == RohmExtDep) {return EFI_INVALID_PARAMETER;}

    Status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void**)&TLMMProtocol);
    if(EFI_ERROR(Status) || NULL == TLMMProtocol)
    {
        CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a LocateProtocol Failed, Status [%r] \r\n", __FUNCTION__, Status));
        return Status;
    }

    for(NumDep = 0; NumDep < CHARGER_EXT_HW_EXT_HW_DEP_MAX; NumDep++)
    {
        switch(RohmExtDep[NumDep].ExtDepResource)
        {
          case CHARGER_EXT_HW_EXT_HW_DEP_MSM_RESOURCE:
            {
              Status  = TLMMProtocol->ConfigGpio(
                          (UINT32)EFI_GPIO_CFG(RohmExtDep[NumDep].ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          TLMM_GPIO_ENABLE);

              Status |= TLMMProtocol->GpioOut(
                          (UINT32)EFI_GPIO_CFG(RohmExtDep[NumDep].ResourceID, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),
                          ((CHARGER_EXT_HW_EXT_HW_DEP_ENABLE_HIGH == RohmExtDep[NumDep].InitState) ? GPIO_HIGH_VALUE : GPIO_LOW_VALUE));

              if(EFI_ERROR(Status))
              {
                  CHARGEREXTHW_LIB_DEBUG((EFI_D_ERROR, "%a TLMMProtocol APIs Failed, Status [%r] \r\n", __FUNCTION__, Status));
                  return Status;
              }

              CHARGEREXTHW_LIB_DEBUG((EFI_D_WARN, "%a Resource[%d] Enable Success \r\n", __FUNCTION__, RohmExtDep[NumDep].ResourceID));
              // gBS->Stall((uint32)(1000));  /* 1 Msec wait */

            }
            break;

          case CHARGER_EXT_HW_EXT_HW_DEP_PMIC_RESOURCE:
          default:
            continue;
        }
    }

    return Status;
}

i2c_status Rohm_i2c_write_mask(UINT8 offset, UINT16 mask, UINT16 data_in, UINT16 size_in)
{
    i2c_status i2c_sts = I2C_TRANSFER_INVALID;
    UINT16 data = 0;

    i2c_sts = ChargerExtHw_i2c_read(&gRohmDevice, offset, (UINT8 *)&data, sizeof(data));
    if(I2C_SUCCESS != i2c_sts) {return i2c_sts;}

    data &= (~mask);
    data |= (data_in & mask);

    i2c_sts = ChargerExtHw_i2c_write(&gRohmDevice, offset, (UINT8 *)&data, sizeof(data));

    return i2c_sts;
}
