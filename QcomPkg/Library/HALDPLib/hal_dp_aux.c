/*=============================================================================
 
  File: hal_dp_aux.c
 
  Source file for DP AUX hardware functionality 
   
  Copyright (c) 2016-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/


/*------------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include "hal_dp_i.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************

FUNCTION:HAL_DP_Write()
 
  This API write AUX packet to CMD FIFO and trigger the transaction.

Parameters:
  eControllerId              -[in] DP core ID
  psWritePacket              -[in] pointer to data struct to be write out.
  uAUXWriteFlags             -[in] Reserved

Return: 
  HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Write(DP_ControllerIDType           eControllerId,
                                HAL_DP_WritePacketType       *puWriteData,
                                uint32                        uAUXWriteFifoFlags)
{
  uint32  i                 = 0;
  uint32  uAUXDataReg       = 0;
  uint32  uAUXTransCtrlReg  = 0;
  uint8  *puByte            = puWriteData->puWriteData;
  uintPtr uOffset           = HAL_DP_GetRegBaseOffset(eControllerId);

  /* Write data into CMD FIFO */
  for (i = 0; (i < puWriteData->uWriteDataSize) && (i < HAL_DP_AUX_CMD_FIFO_LEN); i++)
  {
    uAUXDataReg = HWIO_OUT_FLD(0, DP_AUX_DATA, DATA, puByte[i]);
    if (0 == i)
    {
      /* For the first write, enable INDEX_WRITE with INDEX = 0 
       * Address will auto-increment for the rest of the writes
       */
      uAUXDataReg |= HWIO_DP_AUX_DATA_INDEX_WRITE_BMSK;
    }
    out_dword(uOffset + HWIO_DP_AUX_DATA_ADDR, uAUXDataReg);
  }

  /* Set Transaction details */
  uAUXTransCtrlReg = HWIO_OUT_FLD(0, DP_AUX_TRANS_CTRL, NUM_TR, (puWriteData->uNumofTransactions - 1));

  if(HAL_DP_AUX_TRANSACTION_MODE_I2C == puWriteData->eTransactionType )
  {
    uAUXTransCtrlReg = HWIO_OUT_FLD(uAUXTransCtrlReg, DP_AUX_TRANS_CTRL, TR_TYPE, 1);
  }

  if (HAL_DP_AUXFLAGS_READ & uAUXWriteFifoFlags )
  {
    uAUXTransCtrlReg = HWIO_OUT_FLD(uAUXTransCtrlReg, DP_AUX_TRANS_CTRL, NO_SEND_STOP, 0);
    uAUXTransCtrlReg = HWIO_OUT_FLD(uAUXTransCtrlReg, DP_AUX_TRANS_CTRL, NO_SEND_ADDR, 1);
  }
  else
  {
    uAUXTransCtrlReg = HWIO_OUT_FLD(uAUXTransCtrlReg, DP_AUX_TRANS_CTRL, NO_SEND_STOP, 1);
    uAUXTransCtrlReg = HWIO_OUT_FLD(uAUXTransCtrlReg, DP_AUX_TRANS_CTRL, NO_SEND_ADDR, 1);
  }

  /* Set GO to start the transactions */
  uAUXTransCtrlReg = HWIO_OUT_FLD(uAUXTransCtrlReg, DP_AUX_TRANS_CTRL, GO, 1);

  out_dword(uOffset + HWIO_DP_AUX_TRANS_CTRL_ADDR, uAUXTransCtrlReg);

  puWriteData->uBytesWritten = i;

  return HAL_MDSS_STATUS_SUCCESS;
}

/****************************************************************************

FUNCTION: HAL_DP_Read()
   
  This API allows users to read data from DATA FIFO, the read command has 
  already sent to panel by HAL_DP_AUXWriteFifo, and triggered.

Parameters:
  eControllerId              -[in] DP core ID
  psReadPacket               -[in] pointer to data struct to be read.
  uAUXReadFlags              -[in] Reserved

Return: 
  HAL_MDSS_ErrorType

****************************************************************************/
HAL_MDSS_ErrorType HAL_DP_Read(DP_ControllerIDType     eControllerId,
                               HAL_DP_ReadPacketType  *psReadPacket,
                               uint32                  uAUXReadFlags)
{
  uint32  i           = 0;
  uint32  uAUXDataReg = (HWIO_DP_AUX_DATA_DATA_RW_BMSK | HWIO_DP_AUX_DATA_INDEX_WRITE_BMSK);
  uintPtr uOffset     = HAL_DP_GetRegBaseOffset(eControllerId);

  /* Indicate that it is a read transaction */
  out_dword(uOffset +  HWIO_DP_AUX_DATA_ADDR, uAUXDataReg);

  /* Discard the first byte read since it is the garbage byte */
  uAUXDataReg = in_dword(uOffset + HWIO_DP_AUX_DATA_ADDR);

  /* Read the data from the DATA FIFO */
  for (i = 0; i < psReadPacket->uReadSize; i++)
  {
    uAUXDataReg                       = in_dword(uOffset + HWIO_DP_AUX_DATA_ADDR);
    psReadPacket->puReadDataBuffer[i] = HWIO_GETVAL(DP_AUX_DATA, uAUXDataReg, DATA);
  }
  psReadPacket->uBytesRead = i;

  return HAL_MDSS_STATUS_SUCCESS;
}

/****************************************************************************

FUNCTION: HAL_DP_AUXChannel_Enable()
   
  This API allows users to enable/disable AUX channel. 

Parameters:
  eControllerId         -[in] DP core ID
  bEnable               -[in] TRUE = enable, FALSE = disable

Return: 
  void

****************************************************************************/
void HAL_DP_AUXChannel_Enable(DP_ControllerIDType eControllerId,
                              bool32              bEnable)
{
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32  uRegVal = in_dword(uOffset + HWIO_DP_AUX_CTRL_ADDR);

  if (TRUE == bEnable)
  {
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, AUX_ENABLE, 1);

    // Do not stop controller on AUX error
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, AUX_DPPHY_ERROR_STOP_TX, 1);
  }
  else
  {
    uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, AUX_ENABLE, 0);
  }

  out_dword(uOffset + HWIO_DP_AUX_CTRL_ADDR, uRegVal);
}

/****************************************************************************

FUNCTION: HAL_DP_AUX_Status()
   
  This API allows users to get AUX channel status. 
  It also clears previous transaction details when done

Parameters:
   eControllerId              -[in] DP core ID

Return: 
  AUX status

****************************************************************************/
HAL_DP_AUXStatusType HAL_DP_AUX_Status(DP_ControllerIDType eControllerId)
{
  uintPtr uOffset    = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32  uAuxStatus = in_dword(uOffset + HWIO_DP_AUX_STATUS_ADDR);

  return (HAL_DP_AUXStatusType)uAuxStatus;
}

/****************************************************************************

FUNCTION: HAL_DP_AUX_ClearTransaction()
   
  This API allows users to clear previous AUX transaction. 

Parameters:
   eControllerId              -[in] DP core ID

Return: 
  void

****************************************************************************/
void HAL_DP_AUX_ClearTransaction(DP_ControllerIDType eControllerId)
{
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(eControllerId);

  // Clear GO etc. of previous transaction
  out_dword(uOffset + HWIO_DP_AUX_TRANS_CTRL_ADDR, 0);
}

/****************************************************************************

FUNCTION: HAL_DP_AUX_GetHwCaps()
   
  This API allows users to get AUX HW capabilities. 

Parameters:
  eControllerId         -[in] DP core ID
  psHwCap               -[out] pointer to hw caps

Return: 
  void

****************************************************************************/
void HAL_DP_AUX_GetHwCaps(DP_ControllerIDType    eControllerId,
                          HAL_DP_AuxHwCapsType  *psHwCap)
{
  if (psHwCap)
  {
    psHwCap->uAUXCmdFifoLen     = HAL_DP_AUX_CMD_FIFO_LEN;
    psHwCap->uMaxAUXCmdLen      = HAL_DP_AUX_CMD_MAX;
    psHwCap->uMaxI2CCmdLen      = HAL_DP_AUX_CMD_I2C_MAX;
  }
}


/**********************************************************************************************************************
FUNCTION: HAL_DP_AUXConfig()
   
  This API Config AUX for AUX/I2C transaction.

Parameters: 
  eControllerId              -[in] DP core ID
  psWritePacket              -[in] Pointer to data struct to be write out.
  uAUXConfigFlags            -[in] Reserved

Return:  
  HAL_MDSS_ErrorType

**********************************************************************************************************************/
HAL_MDSS_ErrorType HAL_DP_AUXConfig(DP_ControllerIDType        eControllerId,
                                    HAL_DP_AUXConfigType      *psAUXCtrlConfig,
                                    uint32                     uAUXConfigFlags)
{
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(eControllerId);
  uint32  uRegVal = in_dword(uOffset + HWIO_DP_AUX_CTRL_ADDR);

  if (NULL != psAUXCtrlConfig->psAUXAttrib)
  {
    if (NULL != psAUXCtrlConfig->psAUXAttrib->pbTXLittleEndian)
    {
      uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, AUX_TX_ENDIANNESS, *(psAUXCtrlConfig->psAUXAttrib->pbTXLittleEndian));
    }
    if (NULL != psAUXCtrlConfig->psAUXAttrib->pbRXLittleEndian)
    {
      uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, AUX_RX_ENDIANNESS, *(psAUXCtrlConfig->psAUXAttrib->pbRXLittleEndian));
    }
    if (NULL != psAUXCtrlConfig->psAUXAttrib->pbStopTXOnPhyErr)
    {
      uRegVal = HWIO_OUT_FLD(uRegVal, DP_AUX_CTRL, AUX_DPPHY_ERROR_STOP_TX, *(psAUXCtrlConfig->psAUXAttrib->pbStopTXOnPhyErr));
    }

    out_dword(uOffset + HWIO_DP_AUX_CTRL_ADDR, uRegVal);

    /* AUX controller timeout period */
    uRegVal = 0;
    if (NULL != psAUXCtrlConfig->psAUXAttrib->puTimeoutCount)
    {
      uRegVal = HWIO_OUT_FLD(uRegVal, DP_TIMEOUT_COUNT, TIMEOUT_COUNT, *(psAUXCtrlConfig->psAUXAttrib->puTimeoutCount));
      out_dword(uOffset +  HWIO_DP_TIMEOUT_COUNT_ADDR, uRegVal);
    }

    /* AUX limits for an interrupt  */
    uRegVal  = in_dword(uOffset + HWIO_DP_AUX_LIMITS_ADDR);
    if (NULL != psAUXCtrlConfig->psAUXAttrib->puTimeoutLimit)
    {
      uRegVal  = HWIO_OUT_FLD(uRegVal, DP_AUX_LIMITS, TIMEOUTS_LIMIT,      *(psAUXCtrlConfig->psAUXAttrib->puTimeoutLimit));
    }
    if (NULL != psAUXCtrlConfig->psAUXAttrib->puReadZeroLimit)
    {
      uRegVal  = HWIO_OUT_FLD(uRegVal, DP_AUX_LIMITS, AUX_READ_ZERO_LIMIT, *(psAUXCtrlConfig->psAUXAttrib->puReadZeroLimit));
    }
    if (NULL != psAUXCtrlConfig->psAUXAttrib->puNackDeferLimit)
    {
      uRegVal  = HWIO_OUT_FLD(uRegVal, DP_AUX_LIMITS, AUX_NACK_DEFER_LIMIT, *(psAUXCtrlConfig->psAUXAttrib->puNackDeferLimit));
    }
    if (NULL != psAUXCtrlConfig->psAUXAttrib->puI2CDeferLimit)
    {
      uRegVal  = HWIO_OUT_FLD(uRegVal, DP_AUX_LIMITS, I2C_DEFER_LIMIT,     *(psAUXCtrlConfig->psAUXAttrib->puI2CDeferLimit));
    }

    out_dword(uOffset + HWIO_DP_AUX_LIMITS_ADDR, uRegVal);
  }

  return HAL_MDSS_STATUS_SUCCESS;
}

#ifdef __cplusplus
}
#endif

