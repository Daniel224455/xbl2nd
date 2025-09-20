/*=====================================================================================================================

  File: dp_aux.c

  DESCRIPTION: Implementation of DP AUX/I2C transaction logic.

  Copyright (c) 2016-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  =====================================================================================================================*/

/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "hal_dp_phy.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------
 * Defines
 *-------------------------------------------------------------------------------------------------------------------*/
#define DP_AUX_CMD_HEADER_SIZE_IN_BYTE            4                    /* AUX packet header size in byte            */

#define DP_AUX_MAX_I2C_RETRY                      7                    /* Number of retry attempts if AUX/I2C I/O defers (DP spec recommends 7)  */

#define DP_AUX_TRANS_TIMEOUT_MS                   5                    /* in ms: how long to poll for AUX Transaction Done before timing out */
#define DP_AUX_DONE_POLL_INTERVAL_US              500                  /* in Us: interval between successive polls for AUX DONE of a transaction */   

// All the AUX transaction based interrupts
#define DP_AUX_INTERRUPT                        (HAL_DP_INTR_TYPE_AUX_I2C_DONE              |\
                                                 HAL_DP_INTR_TYPE_WRONG_ADDRESS             |\
                                                 HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT       |\
                                                 HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER    |\
                                                 HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT      |\
                                                 HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C       |\
                                                 HAL_DP_INTR_TYPE_AUX_DEFER_DURING_I2C      |\
                                                 HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR)


/* Define the AUX/I2C packet struct to be send to DP controller H/W                                                 */
typedef struct  
{
  bool32                           bWriteCmd;                          /* 1: this is a write command, otherwise read*/
  uint32                           uCmdAddress;                        /* The read/write address for this command   */
  uint8                           *puCmdData;                          /* Command data pointer                      */
  uint32                           uLength;                            /* Length of command                         */
}DP_PackCmdType;

/*Define the LUT to convert DP_TransactionModeType to HAL_DP_AuxTransactionModeType */
HAL_DP_AuxTransactionModeType geHostTransTypeToHALTransType[HAL_DP_AUX_TRANSACTION_MODE_MAX]= 
{
  HAL_DP_AUX_TRANSACTION_MODE_NONE,    //DP_TRANSACTION_MODE_NONE
  HAL_DP_AUX_TRANSACTION_MODE_NATIVE,  //DP_TRANSACTION_MODE_AUX
  HAL_DP_AUX_TRANSACTION_MODE_I2C,     //DP_TRANSACTION_MODE_I2C
};


/**********************************************************************************************************************
FUNCTION: DP_PackCmd

    This API will pack the AUX/I2C read/write packet into the format required by DP controller H/W. 

Parameters:
    pDeviceCtx                       -[in] Pointer to device context
    psPackCmd                        -[in] the packet to be pack.

Return: 
    DP_Status 

**********************************************************************************************************************/
static DP_Status DP_PackCmd(DP_DeviceCtxType    *pDeviceCtx,
                            DP_PackCmdType      *psPackCmd)
{
  DP_Status          eStatus     = DP_STATUS_SUCCESS;  
  uint32             uLen        = 0;  
  uint32             i           = 0;
  uint8             *pData       = NULL;

  if (NULL == pDeviceCtx)
  {
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }
  else
  {
    if (TRUE == psPackCmd->bWriteCmd)
    {
      uLen = DP_AUX_CMD_HEADER_SIZE_IN_BYTE + psPackCmd->uLength;
    }
    else
    {
      uLen = DP_AUX_CMD_HEADER_SIZE_IN_BYTE;
    }

    if (pDeviceCtx->uAUXCmdBufferCurrentSize + uLen < pDeviceCtx->uMaxAUXCmdFifoLen)
    {
      pData = &(pDeviceCtx->uAUXCmdBuffer[pDeviceCtx->uAUXCmdBufferCurrentSize]); 

      /*   COMM[3:1]   COMM[0]     ADDR[19:16]
       *  --------------------------------------
       * | Reserved  |  Read   | Address[19:16] |
       *  --------------------------------------    */  
      *pData = (psPackCmd->uCmdAddress>>16) & 0xF;      
      if (FALSE == psPackCmd->bWriteCmd)
      {
        *pData |= 0x10;
      }
      
      /* ADDR[15:8] */
      *(++pData) = (psPackCmd->uCmdAddress>>8) & 0xFF;

      /* ADDR[7:0] */
      *(++pData) =  psPackCmd->uCmdAddress & 0xFF;

      /* LEN [7:0] */
      *(++pData) = (psPackCmd->uLength-1) & 0x0ff;

      /* Data bytes in case of write */
      if (TRUE == psPackCmd->bWriteCmd ) 
      {
        for (i = 0; i < psPackCmd->uLength; i++)
        {
          *(++pData) = psPackCmd->puCmdData[i];
        }
      }
      
      pDeviceCtx->uAUXCmdBufferCurrentSize += uLen;
      pDeviceCtx->uAUXNumOfTransaction++;
    }
    else
    {
      eStatus  = DP_STATUS_FAILED_PKT_LENGTH_EXCEED;
      DP_LOG_MESSAGE_ERROR("DP_PackCmd() : Transaction size is too large for the CMD FIFO\n");
    }
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_WaitAUXDone()
   
   This function will Wait until the AUX transaction is done or time out. 

Parameters:
   pDeviceCtx                       -[in] Pointer to device context

Return: 
   DP_Status

**********************************************************************************************************************/
static DP_Status DP_WaitAUXDone(DP_DeviceCtxType    *pDeviceCtx)
{
  DP_Status                       eStatus        = DP_STATUS_FAILED_TIMEOUT;  
  uint32                          i              = 0;
  uint32                          uIntrStatus    = 0;
  uint32                          uPollCount     = 100; /* 100 ms */


  /*
   * some panels may take longer to completed transaction.
   * to avoid missing interrupt status bit (status bit set after polling had completed)
   * make polling period of 100ms here.
   * most case, interrupt status bit should be set lkess than 5ms
   */
  for(i = 0; i < uPollCount; ++i)
  { 
    HAL_DP_Interrupt_GetStatus(pDeviceCtx->eControllerID, (HAL_DP_InterruptType *) &uIntrStatus, 0);

    if (uIntrStatus & DP_AUX_INTERRUPT)
    {
      HAL_DP_AUX_ClearTransaction(pDeviceCtx->eControllerID);

      // Success
      
      if (HAL_DP_INTR_TYPE_AUX_I2C_DONE & uIntrStatus)
      {
        eStatus = DP_STATUS_SUCCESS;
      }
      
      // Fatal failures
      
      else if (HAL_DP_INTR_TYPE_WRONG_ADDRESS & uIntrStatus)
      {
        DP_LOG_MESSAGE_ERROR("Wrong address in AUX/I2C transaction\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_FATAL_ERROR;
      }
      else if (HAL_DP_INTR_TYPE_CONSECUTIVE_TIMEOUT & uIntrStatus)
      {
        DP_LOG_MESSAGE_ERROR("AUX/I2C transaction has timed out\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_FATAL_ERROR;
      }
      else if (HAL_DP_INTR_TYPE_WRONG_READDATA_COUNT & uIntrStatus)
      {
        DP_LOG_MESSAGE_ERROR("num of read bytes does not match requested\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_FATAL_ERROR;
      }
      else if (HAL_DP_INTR_TYPE_DPPHY_AUX_ERROR & uIntrStatus)
      {
        DP_LOG_MESSAGE_ERROR("DPPHY AUX error\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_FATAL_ERROR;
      }

      // Nack / Defer
      
      else if (HAL_DP_INTR_TYPE_CONSECUTIVE_NACK_DEFER & uIntrStatus)
      {
        DP_LOG_MESSAGE_INFO("AUX/I2C transaction has NACKed / DEFERed\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_NOT_READY;
      }
      else if (HAL_DP_INTR_TYPE_AUX_NACK_DURING_I2C & uIntrStatus)
      {
        DP_LOG_MESSAGE_INFO("AUX NACKed during an I2C transaction\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_NOT_READY;
      }
      else if (HAL_DP_INTR_TYPE_AUX_DEFER_DURING_I2C & uIntrStatus)
      {
        DP_LOG_MESSAGE_INFO("AUX DEFERed during an I2C transaction\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_NOT_READY;
      }

      HAL_DP_Interrupt_Clear(pDeviceCtx->eControllerID, (HAL_DP_InterruptType) uIntrStatus, 0);
      
      break;
    }
    
    DP_OSAL_SleepMs(1);
  }

  if (0 == (uIntrStatus & DP_AUX_INTERRUPT))
  {
    /* SW timeout - reset the AUX controller */
    HAL_DP_Reset(pDeviceCtx->eControllerID, HAL_DP_MODULE_TYPE_AUX, 0);
  }

  return eStatus;
}


/**********************************************************************************************************************

FUNCTION: DP_Host_ClientRead()

  This function will read from AUX/I2c channel 

Parameters:
  pDeviceCtx                       -[in]  Pointer to device context
  psReadPacket                     -[out] the read packet

Return: 
  DP_Status 

**********************************************************************************************************************/
DP_Status  DP_Host_ClientRead(DP_DeviceCtxType    *pDeviceCtx,
                              DP_ReadPacketType   *psReadPacket)
{
  DP_Status           eStatus     = DP_STATUS_SUCCESS;  

  if ((NULL == pDeviceCtx) || 
      (NULL == psReadPacket))
  {
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }
  else if ((pDeviceCtx->eAUXTransactionTypeInQueue != DP_TRANSACTION_MODE_NONE) &&
           (pDeviceCtx->eAUXTransactionTypeInQueue != psReadPacket->eTransactionType))
  {    
    DP_LOG_MESSAGE_ERROR("DP_Host_ClientRead() : All transactions in the queued batch must be of same type\n");
    eStatus  = DP_STATUS_FAILED_NOT_SUPPORTED;
  }
  else
  {    
    DP_PackCmdType               sPackCmd;

    sPackCmd.bWriteCmd            = FALSE;
    sPackCmd.uCmdAddress          = psReadPacket->uAddress;
    sPackCmd.puCmdData            = NULL;
    sPackCmd.uLength              = psReadPacket->uReadSize;

    /* Pack into software queue */
    eStatus = DP_PackCmd(pDeviceCtx, &sPackCmd);
    
    pDeviceCtx->eAUXTransactionTypeInQueue = psReadPacket->eTransactionType;
    
    psReadPacket->uBytesRead = 0;

    if ((DP_STATUS_SUCCESS == eStatus) &&
        (FALSE             == psReadPacket->bQueueOnly))
    {
      uint32                  uAuxPhyCfgIdx = 0;
      uint32                  uRetryNum      = 0;
      HAL_DP_AuxPhyCfgType    sAuxPhyCfg;
      HAL_DP_WritePacketType  sHALWritePacket;
      HAL_DP_ReadPacketType   sHALReadPacket;

      sAuxPhyCfg.pDPAuxPhyCfg1Index = &uAuxPhyCfgIdx;

      for (;;)
      {
        for (uRetryNum = 0; uRetryNum <= DP_AUX_MAX_I2C_RETRY; uRetryNum++)
        {
          sHALWritePacket.eTransactionType       = geHostTransTypeToHALTransType[psReadPacket->eTransactionType];
          sHALWritePacket.puWriteData            = pDeviceCtx->uAUXCmdBuffer;
          sHALWritePacket.uNumofTransactions     = pDeviceCtx->uAUXNumOfTransaction;
          sHALWritePacket.uWriteDataSize         = pDeviceCtx->uAUXCmdBufferCurrentSize;
        
          /* Send the command */
          HAL_DP_Write(pDeviceCtx->eControllerID, &sHALWritePacket, HAL_DP_AUXFLAGS_READ);

          eStatus =  DP_WaitAUXDone(pDeviceCtx);
          if (DP_STATUS_SUCCESS == eStatus)
          {
            break;
          }
        }

        if (DP_STATUS_SUCCESS == eStatus)
        {
          break;
        }
        else
        {
          /* AUX Write failed, try next AUX PHY setting */
          uAuxPhyCfgIdx++;
          if (HAL_MDSS_STATUS_SUCCESS != HAL_DP_PHY_AUXConfig(pDeviceCtx->eDeviceID, &sAuxPhyCfg))
          {
            /* Tried all AUX PHY settings */
            break;
          }

            /* HW team recommended to add a sleep of 1ms after setting the new AUX config */
            DP_OSAL_SleepMs(1);
        }
      }

      /* Reset AUX related context */
      pDeviceCtx->uAUXCmdBufferCurrentSize      = 0;
      pDeviceCtx->uAUXNumOfTransaction          = 0;
      pDeviceCtx->eAUXTransactionTypeInQueue    = DP_TRANSACTION_MODE_NONE;

      if (DP_STATUS_SUCCESS == eStatus)
      {
        /* Read the reply: data should be in the DATA FIFO now */
        sHALReadPacket.puReadDataBuffer     = psReadPacket->puReadDataBuffer;
        sHALReadPacket.uReadSize            = psReadPacket->uReadSize;        
        HAL_DP_Read(pDeviceCtx->eControllerID, &sHALReadPacket,0);
        
        psReadPacket->uBytesRead            = sHALReadPacket.uBytesRead;
      
      }
      else
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_ClientRead() : DP_WaitAUXDone Failed (%d attempts)\n", uRetryNum);
      }
    }
  }

  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_Host_ClientWrite()

  This API allows users to send the eDP command out to DP Panels. 

Parameters:
  pDeviceCtx                       -[in]  Pointer to device context
  pWritePacket                     -[out] Command package to be sent out to DP panels

Return: 
  DP_Status 

**********************************************************************************************************************/
DP_Status DP_Host_ClientWrite(DP_DeviceCtxType    *pDeviceCtx,
                              DP_WritePacketType  *psWritePacket)
{
  DP_Status            eStatus     = DP_STATUS_SUCCESS;  

  if((NULL == pDeviceCtx) ||
     (NULL == psWritePacket))
  {
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }
  else if ((pDeviceCtx->eAUXTransactionTypeInQueue != DP_TRANSACTION_MODE_NONE) &&
           (pDeviceCtx->eAUXTransactionTypeInQueue != psWritePacket->eTransactionType))
  {    
    DP_LOG_MESSAGE_ERROR("DP_Host_ClientWrite() : All transactions in the queued batch must be of same type\n");
    eStatus  = DP_STATUS_FAILED_NOT_SUPPORTED;
  }
  else
  {
    HAL_DP_WritePacketType       sHALWritePacket;
    DP_PackCmdType               sPackCmd;

    sPackCmd.bWriteCmd            = TRUE;
    sPackCmd.uCmdAddress          = psWritePacket->uAddress;
    sPackCmd.puCmdData            = psWritePacket->puWriteData;
    sPackCmd.uLength              = psWritePacket->uWriteDataSize;

    pDeviceCtx->eAUXTransactionTypeInQueue = psWritePacket->eTransactionType;
    psWritePacket->uBytesWritten            = 0;

    DP_OSAL_MemSet(&sHALWritePacket, 0, sizeof(HAL_DP_WritePacketType));

    eStatus  = DP_PackCmd(pDeviceCtx, &sPackCmd);

    if ((DP_STATUS_SUCCESS == eStatus) &&
        (FALSE == psWritePacket->bQueueOnly))
    {
      uint32                  uAuxPhyCfgIdx = 0;
      uint32                  uRetryNum     = 0;
      HAL_DP_AuxPhyCfgType    sAuxPhyCfg;

      sAuxPhyCfg.pDPAuxPhyCfg1Index = &uAuxPhyCfgIdx;

      for (;;)
      {
        for (uRetryNum = 0; uRetryNum <= DP_AUX_MAX_I2C_RETRY; uRetryNum++)
        {
          sHALWritePacket.eTransactionType   = geHostTransTypeToHALTransType[psWritePacket->eTransactionType];
          sHALWritePacket.puWriteData        = pDeviceCtx->uAUXCmdBuffer;
          sHALWritePacket.uNumofTransactions = pDeviceCtx->uAUXNumOfTransaction;
          sHALWritePacket.uWriteDataSize     = pDeviceCtx->uAUXCmdBufferCurrentSize;

          /* Send the command */
          HAL_DP_Write(pDeviceCtx->eControllerID, &sHALWritePacket, 0);

          eStatus = DP_WaitAUXDone(pDeviceCtx);
          if (DP_STATUS_SUCCESS == eStatus)
          {
            psWritePacket->uBytesWritten = sHALWritePacket.uBytesWritten;
            break;
          }
        }

        if (DP_STATUS_SUCCESS == eStatus)
        {
          break;
        }
        else
        {
          /* AUX Write failed, try next AUX PHY setting */
          uAuxPhyCfgIdx++;
          if (HAL_MDSS_STATUS_SUCCESS != HAL_DP_PHY_AUXConfig(pDeviceCtx->eDeviceID, &sAuxPhyCfg))
          {
            /* Tried all AUX PHY settings */
            break;
          }

            /* HW team recommended to add a sleep of 1ms after setting the new AUX config */
            DP_OSAL_SleepMs(1);
        }
      }
      
      /* Reset AUX related context */
      pDeviceCtx->uAUXCmdBufferCurrentSize    = 0;
      pDeviceCtx->uAUXNumOfTransaction        = 0;
      pDeviceCtx->eAUXTransactionTypeInQueue  = DP_TRANSACTION_MODE_NONE;

      if (DP_STATUS_SUCCESS != eStatus )      
      {
        DP_LOG_MESSAGE_ERROR("DP_Host_ClientWrite(): DP_WaitAUXDone failed  (%d attempts)\n", uRetryNum);
      }
      else if (sHALWritePacket.uBytesWritten != sHALWritePacket.uWriteDataSize)
      {
        eStatus  = DP_STATUS_FAILED_RESOURCE_FAILED;
        DP_LOG_MESSAGE_ERROR("DP_Host_ClientWrite() : Bytes Written(0x%x) is not expected\n", sHALWritePacket.uBytesWritten);
      }
    }
  }

  return eStatus;
}

#ifdef __cplusplus
}
#endif

