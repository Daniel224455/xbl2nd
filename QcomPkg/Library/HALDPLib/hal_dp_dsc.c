/*=====================================================================================================================
 
  File: hal_dp_dsc.c
 
  Source file for DP DSC related functionality 
   
  Copyright (c) 2018 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=======================================================================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
 * Include Files
 *-------------------------------------------------------------------------------------------------------------------*/
#include "dp_i.h"
#include "hal_dp_i.h"
#include "hal_mdp.h"
#include "dp_dsc.h"

#ifdef __cplusplus
extern "C" {
#endif


uint64 DP_MNValueGcd(uint64 uNumerator, uint64 uDenominator);

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_DSC_CompressionMode()
*/
/*!
* \DESCRIPTION
*      Calculate and configure DP DSC compression mode register
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void HAL_DP_Host_DSC_CompressionMode(DP_DeviceCtxType *pDeviceCtx)
{
   uint32 uChunk;
   uint32 uSlice;
   uint32 uEOLByteNum;
   uintPtr uOffset = HAL_DP_GetRegBaseOffset(pDeviceCtx->eControllerID);
   uint32 uRegValue = 0;

   DP_LOG_MESSAGE_INFO("HAL_DP_DSC_CompressionMode:  bpp=%d bpc=%d w=%d h=%d slice=%d\n", pDeviceCtx->sDSC.ubpc, pDeviceCtx->sDSC.ubpp, pDeviceCtx->sDSC.uSliceWidth, pDeviceCtx->sDSC.uSliceHeight, pDeviceCtx->sDSC.uSlicePerLine);


    uSlice = pDeviceCtx->sDSC.uSlicePerLine;
    /*
     *  Chunk_size_ bytes = 
     * orig_width_pixels / Slice_Per_Line * orig_bpp (24 bits / 8) / compression ratio
     */
    uChunk = pDeviceCtx->sDSC.uChunkSize;
    uEOLByteNum = uChunk % 3;

   /*
    * enable dsc databus_widen, 48 bpp instead of 24 bpp per pclk at disp_intf
    * NOTE: need to match with MDSS_MDP_INTF_0_INTF_CONFIG2: DATABUS_WIDEN
    */
   if (pDeviceCtx->uPixelPerClock == 2)
   {
      uRegValue = in_dword(uOffset + HWIO_DP_P0CLK_INTF_CONFIG_ADDR);  /* 0x0AE98A14 */
      uRegValue = HWIO_OUT_FLD(uRegValue, DP_P0CLK_INTF_CONFIG, DATABUS_WIDEN, 1);
      out_dword(uOffset + HWIO_DP_P0CLK_INTF_CONFIG_ADDR, uRegValue);
   }

   uRegValue = 0;
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_COMPRESSION_MODE_CTRL, BYTE_COUNT, uChunk);
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_COMPRESSION_MODE_CTRL, SLICES_PER_LINE, uSlice - 1);
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_COMPRESSION_MODE_CTRL, EOL_BYTE_NUM, uEOLByteNum);
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_COMPRESSION_MODE_CTRL, EN, 1);
   out_dword(uOffset + HWIO_DP_COMPRESSION_MODE_CTRL_ADDR, uRegValue);  /* 0x0AE98580 */
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_DSC_DTO()
*/
/*!
* \DESCRIPTION
*      Calculate and configure DP DSC dto mode register
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void HAL_DP_Host_DSC_DTO(DP_DeviceCtxType *pDeviceCtx)
{
   DP_PanelInfoType       *psPanelInfo  = &(pDeviceCtx->sPanelInfo[pDeviceCtx->uModeIndex]);
   uint32 num;
   uint32 denom;
   uint32 gcd;
   uint32 uRegValue = 0;
   uintPtr uOffset = HAL_DP_GetRegBaseOffset(pDeviceCtx->eControllerID);

   /*
    *    2 / CompressionRatio * uncompressed bits per transfer / compressed bit per transfer
    */
   num   = 2;
   denom = pDeviceCtx->sDSC.uCompressionRatio;

   num *= 24;  /* uncompressed bit per transfer at disp_intf */

   if (pDeviceCtx->uPixelPerClock == 2)
   {
      denom *= 48;  /* compressed bit per transfer at disp_intf */
   }
   else
   {
      denom *= 24;  /* compressed bit per transfer at disp_intf */
   }

   gcd = DP_MNValueGcd(num, denom);

   num /= gcd;
   denom /= gcd;
   num &= 0x7f;     /* 7 bits */
   denom &= 0x7f;   /* 7 bits */

   uRegValue   = 0;
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_P0CLK_TIMING_ENGINE_EN, FRAME_CRC_EN, 1);
   out_dword(uOffset + HWIO_DP_P0CLK_TIMING_ENGINE_EN_ADDR, uRegValue);

   uRegValue = in_dword(uOffset + HWIO_DP_P0CLK_DSC_DTO_ADDR);  /* 0x0AE98A7C */
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_P0CLK_DSC_DTO, NUMERATOR, num);
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_P0CLK_DSC_DTO, DENOMINATOR, denom);
   uRegValue   = HWIO_OUT_FLD(uRegValue, DP_P0CLK_DSC_DTO, DTO_EN, 1);
   out_dword(uOffset + HWIO_DP_P0CLK_DSC_DTO_ADDR, uRegValue);

   /* DTO_COUNT */
   uRegValue = (psPanelInfo->uVisibleWidthInPixels * 3) / pDeviceCtx->sDSC.uCompressionRatio;
   if (pDeviceCtx->uPixelPerClock == 2)
   {
      denom = 6;  /* disp_intf transfer 48 bits per pclk */
   }
   else
   {
      denom = 3;  /* disp_intf transfer 24 bits per pclk */
   }

   uRegValue /= denom;

   out_dword(uOffset + HWIO_DP_P0CLK_DSC_DTO_COUNT_ADDR, uRegValue - 1);
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_FEC_Enabl()
*/
/*!
* \DESCRIPTION
*      Enable host FEC function
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
void HAL_DP_Host_FEC_Enable(DP_DeviceCtxType *pDeviceCtx)
{
   uintPtr uOffset = HAL_DP_GetRegBaseOffset(pDeviceCtx->eControllerID);
   uint32 uRegValue = 0;

   uRegValue = in_dword(uOffset + HWIO_DP_MAINLINK_CTRL_ADDR);  /* 0x0AE90400 */
   uRegValue = HWIO_OUT_FLD(uRegValue, DP_MAINLINK_CTRL, FLUSH_MODE, 3);
   uRegValue = HWIO_OUT_FLD(uRegValue, DP_MAINLINK_CTRL, FEC_SEQ_MODE, 1);
   uRegValue = HWIO_OUT_FLD(uRegValue, DP_MAINLINK_CTRL, FB_BOUNDARY_SEL, 1);
   uRegValue = HWIO_OUT_FLD(uRegValue, DP_MAINLINK_CTRL, FEC_EN, 1);
   out_dword(uOffset + HWIO_DP_MAINLINK_CTRL_ADDR, uRegValue);  /* 0x0AE90400 */

}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_DSC_PPS()
*/
/*!
* \DESCRIPTION
*      Commit PPS from buffer to DP controller registers
*
*   pDeviceCtx                       -[in ] Pointer to device context
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  HAL_DP_Host_DSC_PPS(DP_DeviceCtxType  *pDeviceCtx)
{
  DP_Status  eStatus      = DP_STATUS_FAIL; 
  uint8      uHB0;
  uint8      uHB1;
  uint8      uHB2;
  uint8      uHB3;
  uint8      uPB0;
  uint8      uPB1;
  uint8      uPB2;
  uint8      uPB3;
  uint8      uPBList[32];
  uint32     i;
  uint32     j;
  uint8      *bp;
  uint32     uDataByte;
  uintPtr    uOffset = HAL_DP_GetRegBaseOffset(pDeviceCtx->eControllerID);

   bp = pDeviceCtx->auPPSBuffer;
   DP_OSAL_MemSet(uPBList, 0x00, sizeof(uPBList));

   uHB0 = 0;
   uPB0 = DP_Host_DSC_Calc_ECC(uHB0, 1);
   uHB1 = 0x10;
   uPB1 = DP_Host_DSC_Calc_ECC(uHB1, 1);
   uHB2 = 0x7f;
   uPB2 = DP_Host_DSC_Calc_ECC(uHB2, 1);
   uHB3 = 0;
   uPB3 = DP_Host_DSC_Calc_ECC(uHB3, 1);

   out_dword(uOffset + HWIO_DP_PPS_HB_0_3_ADDR, ((uHB3 << 24) | (uHB2 << 16) | (uHB1 << 8) | uHB0));
   out_dword(uOffset + HWIO_DP_PPS_PB_0_3_ADDR, ((uPB3 << 24) | (uPB2 << 16) | (uPB1 << 8) | uPB0));

   /*
    * pps_0 to pps_87: 88 bytes of data
    * pps_88 to pps_127 are all 0
    */
   for (i = 0; i < 22; i++)    /* total 88 data bytes */
   {
       uDataByte = 0;
       for (j = 0; j < 4; j++)
       {
          uDataByte |= (*bp << (j * 8));
          bp++;
       }
       uPBList[i] = DP_Host_DSC_Calc_ECC(uDataByte, 4);
       out_dword(uOffset + HWIO_DP_PPS_PPS_0_3_ADDR + (4 * i), uDataByte);
   }

   /*
    * 1 parity byte for every 4 data byte
    * 22 parity bytes derived from above
    * 4 parity bytes per register
    * 6 registers with two 0 parity bytes paded
    * however dp control have 7 registers, each register contains 4 parity bytes 
    */
   bp = uPBList;
   for (i = 0; i < 7; i++)    /* 7 register of 28 parity bytes */
   {
       uDataByte = 0;
       for (j = 0; j < 4; j++)
       {
          uDataByte |= (*bp << (j * 8));
          bp++;
       }

       out_dword(uOffset + HWIO_DP_PPS_PB_4_7_ADDR + (4 * i), uDataByte);
   }

   return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: HAL_DP_Host_PPS_Flush()
*/
/*!
* \DESCRIPTION
*      Flush PPS Peripheral and Interface  so that PPS will be Sent at  next vsync
*
* \param [in]  eControllerId - DP core ID
*
* \retval void
*
**********************************************************************************************/
void  HAL_DP_Host_DSC_PPS_Flush(DP_DeviceCtxType  *pDeviceCtx)
{
  uintPtr uOffset = HAL_DP_GetRegBaseOffset(pDeviceCtx->eControllerID);

   /* flush PPS */
   out_dword(uOffset + HWIO_DP_DP_FLUSH_ADDR, 0x01);

}

#ifdef __cplusplus
}
#endif
