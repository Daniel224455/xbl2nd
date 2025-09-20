/*! \file */

/*
===========================================================================

FILE:         hal_mdp_interface.c

DESCRIPTION: Manages the MDP (Display) Interface

===========================================================================
Copyright (c) 2012-2018 Qualcomm Technologies, Inc. All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================
*/

#include "hal_mdp_i.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
*
** FUNCTION: HAL_MDP_Get_PhyInterface_BaseOffset()
*/
/*!
* \brief
*     Returns offset of the data path
*
* \param [in] eIntefaceId         - INTERFACE ID (0, 1, 2, and 3)
*
* \retval uintPtr
*
****************************************************************************/
static uintPtr HAL_MDP_Get_PhyInterface_BaseOffset(HAL_MDP_PhyInterfaceId  ePhyInterfaceId)
{
   uintPtr   uRegOffset  = 0x0;

   if((HAL_MDP_PHYSICAL_INTERFACE_ID_NONE      == ePhyInterfaceId) ||
      (HAL_MDP_PHYSICAL_INTERFACE_MAX          <= ePhyInterfaceId))
   {
      HAL_MDP_ASSERT();
   }
   else
   {
      uRegOffset = uMDPPhyInterfaceRegBaseOffset[ePhyInterfaceId];
   }
   return uRegOffset;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Get_TE_BaseOffset()
*/
/*!
* \brief
*     Returns offset of the data path with respect to INTF_1
*
* \param [in] eIntefaceId         - INTERFACE ID (0, 1, 2, and 3)
*
* \retval uintPtr
*
****************************************************************************/
static uintPtr HAL_MDP_Get_TE_BaseOffset(HAL_MDP_PhyInterfaceId  ePhyInterfaceId)
{
   uintPtr   uRegOffset  = 0x0;

   if ((HAL_MDP_PHYSICAL_INTERFACE_1     > ePhyInterfaceId) ||
       (HAL_MDP_PHYSICAL_INTERFACE_MAX   <= ePhyInterfaceId))
   {
      HAL_MDP_ASSERT();
   }
   else
   {
      uRegOffset = uMDPPhyInterfaceRegBaseOffset[ePhyInterfaceId] - uMDPPhyInterfaceRegBaseOffset[HAL_MDP_PHYSICAL_INTERFACE_1];
   }
   
   return uRegOffset;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Get_PhyInterfaceStream_BaseOffset()
*/
/*!
* \brief
*     Returns the offset difference of the given stream ID from the base stream ID (VIDEO_0)
*
* \param [in] eStreamId         - Stream ID
*
* \retval uintPtr
*
****************************************************************************/
static uintPtr HAL_MDP_Get_PhyInterfaceStream_BaseOffset(HAL_MDP_InterfaceStreamId  eStreamId)
{
   uintPtr   uRegOffset  = 0x00000000;

   if (HAL_MDP_INTERFACE_STREAM_MAX <= eStreamId)
   {
      HAL_MDP_ASSERT();
   }
   else
   {
      uRegOffset = uMDPIntfMisrStreamRegBaseOffset[eStreamId];
   }
   return uRegOffset;
}


/****************************************************************************
*
** FUNCTION: HAL_MDP_Get_MISRCTRLOffset()
*/
/*!
* \brief
*     Returns MISR status (or) CRC value
*
* \param [in]   eInterfaceId      - Interface id
* \param [out]  pIntfMisrStatus   - MISR status 
* \param [in]   eMisrStatusId     - MISR status id to read signature or to read CRC generate complete status
* \param [in]   eConnectType      - Interface connect type
* \param [in]   eStreamId         - Stream identification
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_MISR_Status(HAL_MDP_InterfaceId              eInterfaceId,
                                          HAL_MDP_Interface_MISRReturnType *pIntfMISRReturnType,
                                          HAL_MDP_MISR_Status_Id           eMisrStatusId,
                                          HAL_MDP_InterfaceConnectionType  eConnectType,
                                          HAL_MDP_InterfaceStreamId        eStreamId)
{
   HAL_MDP_Interface_MISRInfoType   *pMISR_CRC_Info         = NULL;
   HAL_MDP_Interface_MISRStatusType *pMISR_Status_Info      = NULL;
   uint32                           uMisrSignatureRegValue  = 0x00000000;
   uint32                           uRegValue               = 0x00000000;
   uintPtr                          uRegOffset              = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);
   uintPtr                          uRegStreamOffset        = HAL_MDP_Get_PhyInterfaceStream_BaseOffset(eStreamId);
 
   uRegValue  = in_dword(HWIO_MDP_INTF_0_MISR_CTRL_ADDR + uRegOffset + uRegStreamOffset);
 
   // Read signature
   if(HAL_MDP_MISR_STATUS_READ_SIGNATURE == eMisrStatusId) 
   {
      uMisrSignatureRegValue = in_dword(HWIO_MDP_INTF_0_MISR_SIGNATURE_ADDR + uRegOffset + uRegStreamOffset);
      pMISR_CRC_Info         = (HAL_MDP_Interface_MISRInfoType *)pIntfMISRReturnType->pMISR_CRC_Info;

      // Return the signature
      pMISR_CRC_Info->uCRCValue = uMisrSignatureRegValue;
 
      // Reset/Clear the CRC after signature read
      uRegValue      = HWIO_OUT_FLD(uRegValue,
                                    MDP_INTF_0_MISR_CTRL,
                                    STATUS_CLEAR, 
                                    0x01);
 
      out_dword(HWIO_MDP_INTF_0_MISR_CTRL_ADDR + uRegOffset + uRegStreamOffset, uRegValue);
 
      uRegValue      = HWIO_OUT_FLD(uRegValue,
                                    MDP_INTF_0_MISR_CTRL,
                                    FRAME_COUNT, 
                                    0x00);
 
      out_dword(HWIO_MDP_INTF_0_MISR_CTRL_ADDR + uRegOffset + uRegStreamOffset, uRegValue);
 
      uRegValue      = HWIO_OUT_FLD(uRegValue,
                                    MDP_INTF_0_MISR_CTRL,
                                    ENABLE, 
                                    0x00);
 
      out_dword(HWIO_MDP_INTF_0_MISR_CTRL_ADDR + uRegOffset + uRegStreamOffset, uRegValue);
   }
   // Read CRC completion status
   else if(HAL_MDP_MISR_STATUS_READ_STATE == eMisrStatusId)
   {
      pMISR_Status_Info = (HAL_MDP_Interface_MISRStatusType *)pIntfMISRReturnType->pMISR_Status_Info;
 
      if(0x00 != (uRegValue & HWIO_MDP_INTF_0_MISR_CTRL_STATUS_BMSK))
      {
         pMISR_Status_Info->bIsCompleted = TRUE;
      }
      else
      {
         pMISR_Status_Info->bIsCompleted = FALSE;
      }
   }
}

 /****************************************************************************
 *
 ** FUNCTION: HAL_MDP_Interface_MISR_Config()
 */
 /*!
 * \brief
 *     Configure the interface MISR (CRC - Generator) Block
 *
 * \param [in]   eInterfaceId      - Interface id
 * \param [in]   pIntfMisrConfig   - MISR configuration information
 * \param [in]   uSetupFlags       - Reserved
 *
 * \retval NONE
 *
 ****************************************************************************/
static void HAL_MDP_Interface_MISR_Config(HAL_MDP_InterfaceId                eInterfaceId,
                                          HAL_MDP_Interface_MISRConfigType   *pIntfMisrConfig,
                                          uint32                             uSetupFlags)
{
   uint32      uMisrRegValue     = 0x00000000;
   uintPtr     uRegOffset        = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);
   uintPtr     uRegStreamOffset  = HAL_MDP_Get_PhyInterfaceStream_BaseOffset(pIntfMisrConfig->eStreamId);

   uMisrRegValue  = HWIO_OUT_FLD(uMisrRegValue,
                                 MDP_INTF_0_MISR_CTRL,
                                 FRAME_COUNT, 
                                 pIntfMisrConfig->uFrameCnt);
   
   if(HAL_MDP_INTERFACE_STREAM_VIDEO_0 == pIntfMisrConfig->eStreamId)
   {
     /*
      * INPUT_SEL field is used to select active panel data or active panel data+timing info 
      * as source for MISR value generation, which doesn't apply in command mode panel.
      */
     uMisrRegValue  = HWIO_OUT_FLD(uMisrRegValue,
                                   MDP_INTF_0_MISR_CTRL,
                                   INPUT_SEL, 
                                   0x1);    // Active pixel data only
   }
 
   if (TRUE == pIntfMisrConfig->bEnable)
   {
     uMisrRegValue  = HWIO_OUT_FLD(uMisrRegValue,
                                   MDP_INTF_0_MISR_CTRL,
                                   ENABLE, 
                                   TRUE);
   }
   else 
   {
     uMisrRegValue  = HWIO_OUT_FLD(uMisrRegValue,
                                   MDP_INTF_0_MISR_CTRL,
                                   ENABLE, 
                                   FALSE);
   }
 
   out_dword(HWIO_MDP_INTF_0_MISR_CTRL_ADDR + uRegOffset + uRegStreamOffset, uMisrRegValue);
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_RefreshInfo()
*/
/*!
* \brief
*     Returns the frame refresh information of the interface
*
* \param [in]   eInterfaceId        - Interface id
* \param [out]  pFrameRefreshInfo   - Frame refresh information
* \param [in]   uFlags              - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_RefreshInfo(HAL_MDP_InterfaceId                       eInterfaceId,
                                          HAL_MDP_Interface_FrameRefreshInfoType    *pFrameRefreshInfo,
                                          uint32                                    uFlags)
{
   uintPtr  uRegOffset        = 0x0;
   uint32   uFrameCount       = 0x00000000;
   uint32   uLineCount        = 0x00000000;

   uRegOffset = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

   // Support only for physical interfaces
   if(HAL_MDP_INTERFACE_WRITEBACK_TYPE_BASE > eInterfaceId)
   {
      uFrameCount = in_dword(HWIO_MDP_INTF_0_FRAME_COUNT_ADDR + uRegOffset);     // Frame counter
      uLineCount  = in_dword(HWIO_MDP_INTF_0_LINE_COUNT_ADDR  + uRegOffset);      // Line counter
   }

   pFrameRefreshInfo->uCurrentFrameCnt    = uFrameCount;
   pFrameRefreshInfo->uCurrentLineCnt     = uLineCount;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_AutoRefresh_Enable()
*/
/*!
* \brief
*     Enable/Disable auto-refresh for the interface block
*
* \param [in]   eInterfaceId        - Interface id
* \param [in]   pAutoRefreshEnable  - Configuration information
* \param [in]   uSetupFlags         - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_AutoRefresh_Enable(HAL_MDP_InterfaceId                         eInterfaceId,
                                                 HAL_MDP_Interface_AutoRefreshEnableType    *pAutoRefreshEnable,
                                                 uint32                                      uSetupFlags)
{
   uintPtr          uRegOffset = 0;
   uint32           uRegVal    = 0;

   // Get register base offset
   uRegOffset = HAL_MDP_Get_TE_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

   // Enable the vsync counter
   uRegVal = in_dword(HWIO_MDP_INTF_1_TEAR_AUTOREFRESH_CONFIG_ADDR + uRegOffset);
   uRegVal = HWIO_OUT_FLD(uRegVal, MDP_INTF_1_TEAR_AUTOREFRESH_CONFIG, ENABLE, ((TRUE == pAutoRefreshEnable->bEnable) ? 1 : 0));

   out_dword(HWIO_MDP_INTF_1_TEAR_AUTOREFRESH_CONFIG_ADDR + uRegOffset, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_TearRefreshInfo()
*/
/*!
* \brief
*     Returns the frame refresh information of Tear block of interface module
*
* \param [in]   eInterfaceId       - Interface id
* \param [out]  pFrameRefreshInfo  - Frame refresh information
* \param [in]   uFlags             - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_TearRefreshInfo(HAL_MDP_InterfaceId                      eInterfaceId,
                                              HAL_MDP_Interface_FrameRefreshInfoType   *pFrameRefreshInfo,
                                              uint32                                    uFlags)
{
  uintPtr          uRegOffset = 0;
  uint32           uRegVal    = 0;

  // Get register base offset
  uRegOffset = HAL_MDP_Get_TE_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

  uRegVal = in_dword(HWIO_MDP_INTF_1_TEAR_INT_COUNT_VAL_ADDR + uRegOffset);

  pFrameRefreshInfo->uCurrentFrameCnt = HWIO_GETVAL(MDP_INTF_1_TEAR_INT_COUNT_VAL, uRegVal, FRAME_COUNT);
  pFrameRefreshInfo->uCurrentLineCnt  = HWIO_GETVAL(MDP_INTF_1_TEAR_INT_COUNT_VAL, uRegVal, LINE_COUNT);
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_TearEngineStatus()
*/
/*!
* \brief
*     Returns the status of the VSYNC counter status of interface module
*
* \param [in]   eInterfaceId       - Interface id
* \param [out]  pTearEngineStatus  - engine status information
* \param [in]   uFlags             - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_TearEngineStatus(HAL_MDP_InterfaceId                  eInterfaceId,
                                               HAL_MDP_Interface_EngineStatusType  *pTearEngineStatus,
                                               uint32                               uFlags)
{
  uintPtr          uRegOffset = 0;
  uint32           uRegVal    = 0;

  // Get register base offset
  uRegOffset = HAL_MDP_Get_TE_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

  uRegVal = in_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC_ADDR + uRegOffset);
  uRegVal = HWIO_GETVAL(MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC, uRegVal, VSYNC_COUNTER_EN);
  
  pTearEngineStatus->bIsEngineOn = (1 == uRegVal)? TRUE : FALSE;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_Vsync_Enable()
*/
/*!
* \brief
*     Enable/Disable vsync counter for the interface block
*
* \param [in]   eInterfaceId      - Interface id
* \param [in]   pIntrVsyncEnable  - Configuration information
* \param [in]   uSetupFlags       - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_Vsync_Enable(HAL_MDP_InterfaceId                  eInterfaceId,
                                           HAL_MDP_Interface_VsyncEnableType   *pIntrVsyncEnable,
                                           uint32                               uSetupFlags)
{
  uintPtr          uRegOffset = 0;
  uint32           uRegVal    = 0;

  // Get register base offset
  uRegOffset = HAL_MDP_Get_TE_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

  // Enable/Disable the vsync counter
  uRegVal = in_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC_ADDR + uRegOffset);
  uRegVal = HWIO_OUT_FLD(uRegVal, MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC, VSYNC_COUNTER_EN, ((TRUE == pIntrVsyncEnable->bEnable) ? 1 : 0));

  out_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC_ADDR + uRegOffset, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_TE_Enable()
*/
/*!
* \brief
*     Enable/Disable tear check for given interface
*
* \param [in]   eInterfaceId     - Interface id
* \param [in]   pTEEnable        - Configuration information
* \param [in]   uSetupFlags      - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_TE_Enable(HAL_MDP_InterfaceId                  eInterfaceId,
                                        HAL_MDP_Interface_TEEnableType      *pTEEnable,
                                        uint32                               uSetupFlags)
{
  uintPtr           uRegOffset = 0;

  // Get register base offset
  uRegOffset = HAL_MDP_Get_TE_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

  out_dword(HWIO_MDP_INTF_1_TEAR_TEAR_CHECK_EN_ADDR + uRegOffset, ((TRUE == pTEEnable->bEnable) ? 1 : 0));
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_Vsync_Config()
*/
/*!
* \brief
*     Set up vsync count for given interface block
*
* \param [in]   eInterfaceId      - Interface id
* \param [in]   pVsyncConfig      - Configuration information
* \param [in]   uSetupFlags       - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_Vsync_Config(HAL_MDP_InterfaceId                  eInterfaceId,
                                           HAL_MDP_Interface_VsyncConfigType   *pVsyncConfig,
                                           uint32                               uSetupFlags)
{
  uintPtr      uRegOffset              = 0x0;
  uint32       uRegVal                 = 0x0;

  // Get register base offset
  uRegOffset = HAL_MDP_Get_TE_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

  /*========================================================================
  * (1) enable auto-refresh
  *========================================================================*/
  if (TRUE == pVsyncConfig->bEnableAutoRefresh)
  {
    //Actual Refresh Rate = uMdpRefreshrate/uAutoRefreshFrameNumber
    uRegVal = HWIO_OUT_FLD(0,       MDP_INTF_1_TEAR_AUTOREFRESH_CONFIG, ENABLE,      1);
    uRegVal = HWIO_OUT_FLD(uRegVal, MDP_INTF_1_TEAR_AUTOREFRESH_CONFIG, FRAME_COUNT, pVsyncConfig->uAutoRefreshFrameNumDiv);
    out_dword(HWIO_MDP_INTF_1_TEAR_AUTOREFRESH_CONFIG_ADDR + uRegOffset, uRegVal);
  }
  else
  {
    out_dword(HWIO_MDP_INTF_1_TEAR_AUTOREFRESH_CONFIG_ADDR + uRegOffset, 0);
  }

  /*========================================================================
  * (2) program vsync counter
  *========================================================================*/
  uRegVal = in_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC_ADDR + uRegOffset);

  // VSYNC_COUNT
  uRegVal = HWIO_OUT_FLD(uRegVal, MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC, VSYNC_COUNT, pVsyncConfig->uVsyncCount);
  out_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC_ADDR + uRegOffset, uRegVal);

  // VSYNC COUNT HEIGHT
  uRegVal = HWIO_OUT_FLD(0, MDP_INTF_1_TEAR_SYNC_CONFIG_HEIGHT, HEIGHT, pVsyncConfig->uVsyncHeight);
  out_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_HEIGHT_ADDR + uRegOffset, uRegVal);

  /*========================================================================
  * (3) program read-pointer IRQ line
  *========================================================================*/
  uRegVal = HWIO_OUT_FLD(0, MDP_INTF_1_TEAR_RD_PTR_IRQ, IRQ_LINE, pVsyncConfig->uVsyncRdPtrIrqLine);
  out_dword(HWIO_MDP_INTF_1_TEAR_RD_PTR_IRQ_ADDR + uRegOffset, uRegVal);

  /*========================================================================
  * (4) program write-pointer IRQ line
  *========================================================================*/
  uRegVal = HWIO_OUT_FLD(0, MDP_INTF_1_TEAR_WR_PTR_IRQ, IRQ_LINE, pVsyncConfig->uVsyncWrPtrIrqLine);
  out_dword(HWIO_MDP_INTF_1_TEAR_WR_PTR_IRQ_ADDR + uRegOffset, uRegVal);

  /*========================================================================
  * (5) program vsync init value
  *========================================================================*/
  out_dword(HWIO_MDP_INTF_1_TEAR_VSYNC_INIT_VAL_ADDR + uRegOffset, pVsyncConfig->uVsyncInitValue);
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_TE_Config()
*/
/*!
* \brief
*     Set up tear check for given interface block
*
* \param [in]   eInterfaceId     - Interface id
* \param [in]   pTEConfig        - Configuration information
* \param [in]   uSetupFlags      - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_TE_Config(HAL_MDP_InterfaceId                  eInterfaceId,
                                        HAL_MDP_Interface_TEConfigType      *pTEConfig,
                                        uint32                               uSetupFlags)
{
  uintPtr          uRegOffset             = 0;
  uint32           uRegVal                = 0;

  // Get register base offset
  uRegOffset = HAL_MDP_Get_TE_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

  // program vsync input 
  uRegVal = in_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC_ADDR + uRegOffset);
  
  if (TRUE == pTEConfig->bDedicatedTEPin)
  {
    uRegVal = HWIO_OUT_FLD(uRegVal, MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC, VSYNC_IN_EN, 1);
  }
  else
  {
    uRegVal = HWIO_OUT_FLD(uRegVal, MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC, VSYNC_IN_EN, 0);
  }
  out_dword(HWIO_MDP_INTF_1_TEAR_SYNC_CONFIG_VSYNC_ADDR + uRegOffset, uRegVal);

  // program sync threshold
  uRegVal = HWIO_OUT_FLD(0,       MDP_INTF_1_TEAR_SYNC_THRESH, CONTINUE_THRESHOLD, pTEConfig->uContinueThreshold);
  uRegVal = HWIO_OUT_FLD(uRegVal, MDP_INTF_1_TEAR_SYNC_THRESH, START_THRESHOLD,    pTEConfig->uStartThreshold);
  out_dword(HWIO_MDP_INTF_1_TEAR_SYNC_THRESH_ADDR + uRegOffset, uRegVal);

  // program start position
  uRegVal = HWIO_OUT_FLD(0, MDP_INTF_1_TEAR_START_POS, POS, pTEConfig->uStartPosition);
  out_dword(HWIO_MDP_INTF_1_TEAR_START_POS_ADDR + uRegOffset, uRegVal);

  // vsync source select
  uRegVal = HWIO_OUT_FLD(0, MDP_INTF_1_TEAR_MDP_VSYNC_SEL, VSYNC_SEL, pTEConfig->eVsyncSelect);
  out_dword(HWIO_MDP_INTF_1_TEAR_MDP_VSYNC_SEL_ADDR + uRegOffset, uRegVal);
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_EngineStatus()
*/
/*!
* \brief
*     Returns the engine status
*
* \param [in]   eInterfaceId        - Interface id
* \param [out]  pEngineStatus       - Frame refresh information
* \param [in]   uFlags              - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_EngineStatus(HAL_MDP_InterfaceId                 eInterfaceId,
                                           HAL_MDP_Interface_EngineStatusType  *pEngineStatus,
                                           uint32                              uFlags)
{
   uintPtr   uRegOffset        = 0x0;

   uRegOffset = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

   // Support only for physical interfaces
   if (HAL_MDP_INTERFACE_WRITEBACK_TYPE_BASE > eInterfaceId)
   {
      uint32 uRegVal                  = in_dword(HWIO_MDP_INTF_0_INTF_STATUS_ADDR + uRegOffset);
      uint32 uIntfDone                = in_dword(HWIO_MDP_INTF_0_INTF_INTR_STATUS_ADDR + uRegOffset);

      // Interface On/Off state
      pEngineStatus->bIsEngineOn      = HWIO_GETVAL(MDP_INTF_0_INTF_STATUS, uRegVal, TIMING_ENGINE_EN_STATUS);
      // Interface done status
      pEngineStatus->bIsInterfaceDone = HWIO_GETVAL(MDP_INTF_0_INTF_INTR_STATUS, uIntfDone, INTF_DONE);
   }
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_TimingGenInfo()
*/
/*!
* \brief
*     Returns the timing generator configuration parameters
*
* \param [in]   eInterfaceId        - Interface id
* \param [out]  pTimingGenInfo      - Frame refresh information
* \param [in]   uFlags              - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_TimingGenInfo(HAL_MDP_InterfaceId                  eInterfaceId,
                                            HAL_MDP_PhyIntf_TimingGenConfigType  *pTimingGenInfo,
                                            uint32                               uFlags)
{
   uintPtr                 uRegOffset           = 0x0;
   uint32                  uTimingRegValue      = 0x00000000;
   uint32                  uHCycle              = 0x00000000;
   uint32                  uVCycle              = 0x00000000;
   uint32                  uHsyncStartX         = 0x00000000;
   uint32                  uHsyncEndX           = 0x00000000;
   uint32                  uVsyncStart          = 0x00000000;
   uint32                  uVsyncEnd            = 0x00000000;
   uint32                  uActiveVStart        = 0x00000000;
   uint32                  uActiveVEnd          = 0x00000000;
   uint32                  uInterfaceConfig     = 0x00000000;

   
   pTimingGenInfo->eInterfaceStandard = HAL_MDP_INTERFACE_STANDARD_NONE;

   // @@ TODO @@ How to extract eInterfaceMode information i.e Video Mode/Command Mode
   // Does HAL driver need to handle this eInterfaceMode ??
   pTimingGenInfo->eInterfaceMode            = HAL_MDP_INTERFACE_MODE_VIDEO;                    // Interface Mode
   pTimingGenInfo->bInterlacedMode           = FALSE;                                           // No support for interlaced
   pTimingGenInfo->uVsyncHeightInPixelsF1    = 0x00;
   pTimingGenInfo->uVsyncFrontPorchInLinesF1 = 0x00;
   pTimingGenInfo->uVsyncBackPorchInLinesF1  = 0x00;
   pTimingGenInfo->uVsyncPulseInLinesF1      = 0x00;

   uRegOffset = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

   // Extract video engine timing parameters

   // Signal Polarity Configuration
   uTimingRegValue = in_dword(HWIO_MDP_INTF_0_POLARITY_CTL_ADDR + uRegOffset);

   pTimingGenInfo->eDataEnableSignal = (HAL_MDP_SignalPolarityType) HWIO_GETVAL(MDP_INTF_0_POLARITY_CTL, uTimingRegValue, DEN);        // DATA_EN Polarity
   pTimingGenInfo->eVsyncSignal      = (HAL_MDP_SignalPolarityType) HWIO_GETVAL(MDP_INTF_0_POLARITY_CTL, uTimingRegValue, VSYNC);      // VSYNC Polarity
   pTimingGenInfo->eHsyncSignal      = (HAL_MDP_SignalPolarityType) HWIO_GETVAL(MDP_INTF_0_POLARITY_CTL, uTimingRegValue, HSYNC);      // HSYNC Polarity

   // HSYNC SKEW
   pTimingGenInfo->uHysncSkewInPixels = in_dword(HWIO_MDP_INTF_0_HSYNC_SKEW_ADDR + uRegOffset);

   // Border Color
   pTimingGenInfo->uBorderColorInRGB888 = in_dword(HWIO_MDP_INTF_0_BORDER_COLOR_ADDR + uRegOffset);

   // Underflow color
   pTimingGenInfo->uUnderflowColorInRGB888 = in_dword(HWIO_MDP_INTF_0_UNDERFLOW_COLOR_ADDR + uRegOffset);

   // HSYNC related parameters
   uTimingRegValue   = in_dword(HWIO_MDP_INTF_0_HSYNC_CTL_ADDR + uRegOffset);                                      // HSYNC_CTL Register

   uHCycle                              = HWIO_GETVAL(MDP_INTF_0_HSYNC_CTL, uTimingRegValue, HSYNC_PERIOD);             // H-CYCLE
   pTimingGenInfo->uHsyncPulseInPixels  = HWIO_GETVAL(MDP_INTF_0_HSYNC_CTL, uTimingRegValue, PULSE_WIDTH);              // HSYNC Pulse Width

   uTimingRegValue   = in_dword(HWIO_MDP_INTF_0_DISPLAY_HCTL_ADDR + uRegOffset);                                   // DISPLAY_HCTL Register
   uHsyncStartX      = HWIO_GETVAL(MDP_INTF_0_DISPLAY_HCTL, uTimingRegValue, DISPLAY_START_X);                          // HSYNC Start X
   uHsyncEndX        = (HWIO_GETVAL(MDP_INTF_0_DISPLAY_HCTL, uTimingRegValue, DISPLAY_END_X) + 1);                      // HSYNC End X

   uTimingRegValue   = in_dword(HWIO_MDP_INTF_0_ACTIVE_HCTL_ADDR + uRegOffset);                                    // ACTIVE_HCTL
   uInterfaceConfig  = in_dword(HWIO_MDP_INTF_0_INTF_CONFIG_ADDR + uRegOffset);

   pTimingGenInfo->uHRightBorderInPixels  = 0x00;
   pTimingGenInfo->uHLeftBorderInPixels   = 0x00;

   pTimingGenInfo->uHsyncBackPorchInPixels   = (uHsyncStartX - pTimingGenInfo->uHsyncPulseInPixels);                    // HSYNC BackPorch
   pTimingGenInfo->uVisibleWidthInPixels     = (uHsyncEndX - (pTimingGenInfo->uHsyncBackPorchInPixels  +
                                                              pTimingGenInfo->uHsyncPulseInPixels      +
                                                              pTimingGenInfo->uHLeftBorderInPixels     +
                                                              pTimingGenInfo->uHRightBorderInPixels));                  // Visible Width

   pTimingGenInfo->uHsyncFrontPorchInPixels  = (uHCycle - uHsyncEndX);                                                  // HSYNC FrontPorch

   // VSYNC related parameters
   if (0==uHCycle)
   {
     pTimingGenInfo->uVsyncPulseInLines      = 0;
     pTimingGenInfo->uVTopBorderInLines      = 0;
     pTimingGenInfo->uVsyncBackPorchInLines  = 0;
     pTimingGenInfo->uVsyncFrontPorchInLines = 0;

   }
   else
   {
      uVCycle                            = (in_dword(HWIO_MDP_INTF_0_VSYNC_PERIOD_F0_ADDR + uRegOffset)/uHCycle);          // V-CYCLE in lines
      pTimingGenInfo->uVsyncPulseInLines = (in_dword(HWIO_MDP_INTF_0_VSYNC_PULSE_WIDTH_F0_ADDR + uRegOffset)/uHCycle);     // VSYNC Pulse Width in lines
      uVsyncStart                        = (in_dword(HWIO_MDP_INTF_0_DISPLAY_V_START_F0_ADDR + uRegOffset)/uHCycle);       // V-Start in lines
      uVsyncEnd                          = ((in_dword(HWIO_MDP_INTF_0_DISPLAY_V_END_F0_ADDR + uRegOffset) + 1)/uHCycle);   // V-End in lines
      uActiveVStart                       = uVsyncStart;
      uActiveVEnd                         = uVsyncEnd;

      pTimingGenInfo->uVTopBorderInLines     = (uActiveVStart - uVsyncStart);                                              // VSYNC Top Border in lines
      pTimingGenInfo->uVBottomBorderInLines  = (uVsyncEnd - uActiveVEnd);                                                  // VSYNC Bottom Border in lines

      pTimingGenInfo->uVsyncBackPorchInLines = (uVsyncStart - pTimingGenInfo->uVsyncPulseInLines);                         // VSYNC BackPorch in lines
      pTimingGenInfo->uVisibleHeightInPixels = (uActiveVEnd - (pTimingGenInfo->uVsyncPulseInLines     +
                                                               pTimingGenInfo->uVsyncBackPorchInLines +
                                                               pTimingGenInfo->uVTopBorderInLines) );                      // Visual Height in pixels
      pTimingGenInfo->uVsyncFrontPorchInLines= (uVCycle - uVsyncEnd);                                                      // VSYNC Front Port in lines
   }
}


/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_GetDualInterfaceInfo()
*/
/*!
* \brief
*     Get Split Display Mode
*
* \param [in]   eInterfaceId              - Interface id
* \param [out]  pbDualInterfaceEnabled    - TRUE: Dual interface (Split Display)is enabled
* \param [in]   uFlags                    - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_GetDualInterfaceInfo(HAL_MDP_InterfaceId                eInterfaceId,
                                                   bool32                            *pbDualInterfaceEnabled,
                                                   uint32                             uFlags)
{
#if 0 //Poipu #TODO#IMPLEMENT - control path change
  uint32 uReg = in_dword(HWIO_MDP_SPLIT_DISPLAY_EN_ADDR);
  if (0 != uReg)
  {
    *pbDualInterfaceEnabled = TRUE;
  }
  else
  {
    *pbDualInterfaceEnabled = FALSE;
  }
#endif  
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_GetStateInfo()
*/
/*!
* \brief
*     Returns the current state of the given interface id.
*
* \param [in]   eInterfaceId        - Interface id
* \param [out]  pbActive            - TRUE : If module is active
* \param [in]   uFlags              - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_GetStateInfo(HAL_MDP_InterfaceId    eInterfaceId,
                                           bool32                *pbActive,
                                           uint32                 uFlags)
{
   uint32                        uRegValue         = 0x00;
   uint32                        uWBIntfId         = 0x00;
   HAL_MDP_ModuleStatusRegInfo  *psModuleRegInfo   = NULL;

   *(pbActive)        = FALSE;          // Default state

   if(HAL_MDP_INTERFACE_WRITEBACK_TYPE_BASE < eInterfaceId)    // Valid only for write-back interfaces
   {
      psModuleRegInfo = (HAL_MDP_ModuleStatusRegInfo  *)pgsMdpHwInfo->gpsModuleStatusRegInfo[HAL_MDP_MODULE_WRITEBACK_INTERFACE];

      if(NULL != psModuleRegInfo)
      {
         uWBIntfId   = (eInterfaceId - HAL_MDP_INTERFACE_WRITEBACK_TYPE_BASE);
         uRegValue   = in_dword(MDSS_REG_BASE + psModuleRegInfo[uWBIntfId].uRegAddress);

         if(0x00 != (uRegValue & psModuleRegInfo[uWBIntfId].uBitFieldMask) )
         {
            *(pbActive)  = TRUE;        // Module is in active state
         }
      }
   }
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_TPG_Config()
*/
/*!
* \brief
*     Configures the test pattern
*
* \param [in]   eInterfaceId     - Interface id
* \param [in]   pTPGConfig       - TPG configuration information
* \param [in]   uSetupFlags      - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_TPG_Config(HAL_MDP_PhyInterfaceId             eInterfaceId,
                                         HAL_MDP_Interface_TPGConfigType    *pTPGConfig,
                                         uint32                             uSetupFlags)
{
  //TPG is deprecated
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_TimingGen_Config()
*/
/*!
* \brief
*     Set up timing generation for given interface
*
* \param [in]   eInterfaceId             - Interface id
* \param [in]   eChromaDownModuleId      - Chroma down module id
* \param [in]   pInterfaceConfig         - Configuration information
* \param [in]   uSetupFlags              - Reserved
*
* \retval NONE
*
****************************************************************************/
static void HAL_MDP_Interface_TimingGen_Config(HAL_MDP_PhyInterfaceId                eInterfaceId,
                                               HAL_MDP_ChromaDownModuleId            eChromaDownModuleId,
                                               HAL_MDP_PhyIntf_TimingGenConfigType   *pInterfaceConfig,
                                               uint32                                uSetupFlags)
{
   uintPtr                             uRegOffset            = 0x0;
   uint32                              uTimingRegValue       = 0x00000000;
   uint32                              uHCycle               = 0x00000000;  // To prepare the register value
   uint32                              uVCycle               = 0x00000000;
   uint32                              uHsyncStartX          = 0x00000000;
   uint32                              uHsyncEndX            = 0x00000000;
   uint32                              uVsyncStart           = 0x00000000;
   uint32                              uVsyncEnd             = 0x00000000;
   uint32                              uActiveVStart         = 0x00000000;
   uint32                              uActiveVEnd           = 0x00000000;
   uint32                              uPanelFormat          = 0x00000000;
   uint32                              uPanelIntfConfig      = 0x00000000;
   HAL_MDP_PhyIntf_TimingGenConfigType *pInterfaceCfg        = pInterfaceConfig;
   HAL_MDP_PhyIntf_TimingGenConfigType sModifiedInterfaceCfg;
   bool32                              bDualPixelMode        = FALSE;
   uint32                              uRegValue             = 0;
   uint32                              uDTOCount             = 0;
   uint32                              uDenom                = 3;

   if ((HAL_MDP_INTERFACE_STANDARD_DP  == pInterfaceCfg->eInterfaceStandard) ||
       (HAL_MDP_INTERFACE_STANDARD_eDP == pInterfaceCfg->eInterfaceStandard))
   {
     /* Enable 2 Pixels per clock for all DP interfaces */
     bDualPixelMode = TRUE;
   }

   // Configure the video engine timing parameter if inteface is active display type
   if(HAL_MDP_INTERFACE_MODE_VIDEO == pInterfaceConfig->eInterfaceMode)
   {
      uRegOffset = HAL_MDP_Get_PhyInterface_BaseOffset(eInterfaceId);

      // For dual pixel mode - halve all horizontal params
      if (TRUE == bDualPixelMode)
      {
        sModifiedInterfaceCfg = *pInterfaceConfig;

        sModifiedInterfaceCfg.uVisibleWidthInPixels    >>=1;
        sModifiedInterfaceCfg.uHsyncFrontPorchInPixels >>=1;
        sModifiedInterfaceCfg.uHsyncBackPorchInPixels  >>=1;
        sModifiedInterfaceCfg.uHsyncPulseInPixels      >>=1;
        sModifiedInterfaceCfg.uHysncSkewInPixels       >>=1;
        sModifiedInterfaceCfg.uHLeftBorderInPixels     >>=1;

        // Use the modified Timing generation config 
        pInterfaceCfg = &sModifiedInterfaceCfg;
      }

      // HSYNC_CTL (HSYNC PERIO and HSYNC PULSE WIDTH)
      uTimingRegValue      = 0x00000000;

      uHCycle              = (pInterfaceCfg->uHsyncBackPorchInPixels    +
                              pInterfaceCfg->uHsyncFrontPorchInPixels   +
                              pInterfaceCfg->uHsyncPulseInPixels        +
                              pInterfaceCfg->uVisibleWidthInPixels      +
                              pInterfaceCfg->uHLeftBorderInPixels       +
                              pInterfaceCfg->uHRightBorderInPixels);

      uVCycle        = ((pInterfaceCfg->uVisibleHeightInPixels     +
                         pInterfaceCfg->uVsyncBackPorchInLines     +
                         pInterfaceCfg->uVsyncPulseInLines         +
                         pInterfaceCfg->uVsyncFrontPorchInLines    +
                         pInterfaceCfg->uVTopBorderInLines         +
                         pInterfaceCfg->uVBottomBorderInLines) * uHCycle);

      uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue, 
                                          MDP_INTF_0_HSYNC_CTL, 
                                          HSYNC_PERIOD, 
                                          uHCycle);

      uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue, 
                                          MDP_INTF_0_HSYNC_CTL, 
                                          PULSE_WIDTH, 
                                          pInterfaceCfg->uHsyncPulseInPixels);

      out_dword(HWIO_MDP_INTF_0_HSYNC_CTL_ADDR + uRegOffset, uTimingRegValue);            // HSYNC_CTL

      // VSYNC PERIOD
      out_dword(HWIO_MDP_INTF_0_VSYNC_PERIOD_F0_ADDR + uRegOffset, uVCycle);  // VSYNC_PERIOD_F0

      // VSYNC_PULSE_WIDTH (F0)
      uTimingRegValue = pInterfaceCfg->uVsyncPulseInLines * uHCycle;
      out_dword(HWIO_MDP_INTF_0_VSYNC_PULSE_WIDTH_F0_ADDR + uRegOffset, uTimingRegValue);  // VSYNC_PULSE_WIDTH_F0


      /*
       * DP timing requires the active region to be defined in the bottom-right corner of the frame dimensions
       * It is required to work with correct packet/audio transfers, dynamic refresh, AVR, etc
       */
      if((HAL_MDP_INTERFACE_STANDARD_DP  == pInterfaceCfg->eInterfaceStandard) ||
         (HAL_MDP_INTERFACE_STANDARD_eDP == pInterfaceCfg->eInterfaceStandard))
      {
         pInterfaceCfg->uHsyncBackPorchInPixels += pInterfaceCfg->uHsyncFrontPorchInPixels;
         pInterfaceCfg->uHsyncFrontPorchInPixels = 0;

         pInterfaceCfg->uVsyncBackPorchInLines += pInterfaceCfg->uVsyncFrontPorchInLines;
         pInterfaceCfg->uVsyncFrontPorchInLines = 0;
      }

      // DISPLAY_V_START
      uVsyncStart    = ((pInterfaceCfg->uVsyncBackPorchInLines    +
                         pInterfaceCfg->uVsyncPulseInLines ) * uHCycle);
      
      uVsyncEnd      = (((pInterfaceCfg->uVsyncBackPorchInLines    +
                          pInterfaceCfg->uVsyncPulseInLines        +
                          pInterfaceCfg->uVisibleHeightInPixels    +
                          pInterfaceCfg->uVBottomBorderInLines     +
                          pInterfaceCfg->uVTopBorderInLines ) * uHCycle) - 1);
      
      if((HAL_MDP_INTERFACE_STANDARD_DP  == pInterfaceCfg->eInterfaceStandard) ||
         (HAL_MDP_INTERFACE_STANDARD_eDP == pInterfaceCfg->eInterfaceStandard))
      {
         uVsyncStart    += (pInterfaceCfg->uHsyncBackPorchInPixels)    +
                           (pInterfaceCfg->uHsyncPulseInPixels);
         uVsyncEnd      -= (pInterfaceCfg->uHsyncFrontPorchInPixels);
      }
      out_dword(HWIO_MDP_INTF_0_DISPLAY_V_START_F0_ADDR + uRegOffset, uVsyncStart);   // DISPLAY_V_START_F0

      // DISPLAY_V_END
      out_dword(HWIO_MDP_INTF_0_DISPLAY_V_END_F0_ADDR + uRegOffset, uVsyncEnd);     // DISPLAY_V_END_F0

      // ACTIVE_V_START 
      uActiveVStart      = 0x00;
      uActiveVEnd        = 0x00;

      if((0x00 != pInterfaceCfg->uVBottomBorderInLines) || 
         (0x00 != pInterfaceCfg->uVTopBorderInLines) )
      {
         uActiveVStart  =  HWIO_OUT_FLD(uActiveVStart, 
                                        MDP_INTF_0_ACTIVE_V_START_F0,
                                        ACTIVE_START_Y,
                                        (uVsyncStart + (pInterfaceCfg->uVTopBorderInLines * uHCycle)));
         uActiveVEnd    = (uVsyncEnd - (pInterfaceCfg->uVBottomBorderInLines * uHCycle));
      }

      out_dword(HWIO_MDP_INTF_0_ACTIVE_V_START_F0_ADDR + uRegOffset, uActiveVStart);     // ACTIVE_V_START_F0

      // ACTIVE_V_END
      out_dword(HWIO_MDP_INTF_0_ACTIVE_V_END_F0_ADDR + uRegOffset, uActiveVEnd);       // ACTIVE_V_END_F0

      // DISPLAY_HCTL
      uHsyncStartX         = (pInterfaceCfg->uHsyncBackPorchInPixels +
                              pInterfaceCfg->uHsyncPulseInPixels);

      uTimingRegValue      = 0x00;
      uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue, 
                                          MDP_INTF_0_DISPLAY_HCTL,
                                          DISPLAY_START_X,
                                          uHsyncStartX);

      uHsyncEndX           = (pInterfaceCfg->uHsyncBackPorchInPixels  +
                              pInterfaceCfg->uHsyncPulseInPixels      +
                              pInterfaceCfg->uHLeftBorderInPixels     +
                              pInterfaceCfg->uHRightBorderInPixels    +
                              pInterfaceCfg->uVisibleWidthInPixels    - 1);

      uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue, 
                                          MDP_INTF_0_DISPLAY_HCTL,
                                          DISPLAY_END_X,
                                          uHsyncEndX);

      out_dword(HWIO_MDP_INTF_0_DISPLAY_HCTL_ADDR + uRegOffset, uTimingRegValue);       // DISPLAY_HCTL

      // ACTIVE_HCTL
      uTimingRegValue      = 0x00;

      if ((0 != pInterfaceCfg->uHLeftBorderInPixels ) || 
          (0 != pInterfaceCfg->uHRightBorderInPixels) )
      {
         uTimingRegValue   = HWIO_OUT_FLD(uTimingRegValue, 
                                          MDP_INTF_0_ACTIVE_HCTL,
                                          ACTIVE_END_X,
                                          (uHsyncEndX - pInterfaceCfg->uHRightBorderInPixels));

         uTimingRegValue   = HWIO_OUT_FLD(uTimingRegValue,
                                          MDP_INTF_0_ACTIVE_HCTL,
                                          ACTIVE_START_X,
                                          (uHsyncStartX + pInterfaceCfg->uHLeftBorderInPixels));
      }

      out_dword(HWIO_MDP_INTF_0_ACTIVE_HCTL_ADDR + uRegOffset, uTimingRegValue);       // ACTIVE_HCTL

      // Border Color
      out_dword(HWIO_MDP_INTF_0_BORDER_COLOR_ADDR + uRegOffset, pInterfaceCfg->uBorderColorInRGB888);

      // Underflow color
      out_dword(HWIO_MDP_INTF_0_UNDERFLOW_COLOR_ADDR + uRegOffset, pInterfaceCfg->uUnderflowColorInRGB888);

      // HSYNC SKEW
      out_dword(HWIO_MDP_INTF_0_HSYNC_SKEW_ADDR + uRegOffset, pInterfaceCfg->uHysncSkewInPixels);

      // Signal Polarity Selection
      uTimingRegValue      = 0x00000000;
      uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue,
                                          MDP_INTF_0_POLARITY_CTL,
                                          DEN,
                                          pInterfaceCfg->eDataEnableSignal);

      uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue,
                                          MDP_INTF_0_POLARITY_CTL,
                                          VSYNC,
                                          pInterfaceCfg->eVsyncSignal);

      uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue,
                                          MDP_INTF_0_POLARITY_CTL,
                                          HSYNC,
                                          pInterfaceCfg->eHsyncSignal);

      out_dword(HWIO_MDP_INTF_0_POLARITY_CTL_ADDR + uRegOffset, uTimingRegValue);

      uPanelFormat         = in_dword(HWIO_MDP_INTF_0_PANEL_FORMAT_ADDR + uRegOffset);

      // Interface only supports RGB color space
      // For YUV color space we set the interface pixel format to RGB888
      if (HAL_MDP_IS_MDP_PIXEL_YUV_FORMAT(pInterfaceCfg->eInterfacePixelFormat))
      {
        uPanelFormat        |= guPanelFormatInfo[HAL_MDP_PIXEL_FORMAT_RGB_888_24BPP];
      }
      else
      {
        uPanelFormat        |= guPanelFormatInfo[pInterfaceCfg->eInterfacePixelFormat];
      }

      /* Ensure  DSI_VIDEO_STOP_MODE is set for all interfaces*/
      uPanelIntfConfig  = HWIO_OUT_FLD(uPanelIntfConfig,
                                       MDP_INTF_0_INTF_CONFIG,
                                       DSI_VIDEO_STOP_MODE,
                                       TRUE);

      out_dword(HWIO_MDP_INTF_0_PANEL_FORMAT_ADDR + uRegOffset, uPanelFormat);
      out_dword(HWIO_MDP_INTF_0_INTF_CONFIG_ADDR + uRegOffset, uPanelIntfConfig);

      if (TRUE == bDualPixelMode)
      {
          // Dual pixel mode
          uRegValue = in_dword(HWIO_MDP_INTF_0_INTF_CONFIG2_ADDR);

          uRegValue = HWIO_OUT_FLD(uRegValue, MDP_INTF_0_INTF_CONFIG2, DATABUS_WIDEN, (bDualPixelMode ? 1 : 0));

           /*
            * enable HCTL when DTO enabled which only happen at DP with dsc enabled
            */
           if (pInterfaceCfg->bDTOEnable == TRUE)
           {
               uRegValue = HWIO_OUT_FLD(uRegValue, MDP_INTF_0_INTF_CONFIG2, DATA_HCTL_EN, 1);
           }
           else
           {
               uRegValue = HWIO_OUT_FLD(uRegValue, MDP_INTF_0_INTF_CONFIG2, DATA_HCTL_EN, 0);
           }

           pInterfaceCfg->uVisibleWidthInPixels <<= 1; /* restore back to original value */
           uDenom = 6;  /* 48 bits per pclk transmit at dp_intf */

           out_dword(HWIO_MDP_INTF_0_INTF_CONFIG2_ADDR + uRegOffset, uRegValue);
       }


        if (pInterfaceCfg->bDTOEnable == TRUE)
        {
             uTimingRegValue      = 0x00;
             uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue, 
                                          MDP_INTF_0_DISPLAY_DATA_HCTL,
                                          DISPLAY_DATA_START_X,
                                          uHsyncStartX);
             uDTOCount = pInterfaceCfg->uVisibleWidthInPixels  * 3 / pInterfaceCfg->uCompressionRatio;
             uDTOCount /= uDenom;
             uHsyncEndX          =  uHsyncStartX + uDTOCount;

             uTimingRegValue      = HWIO_OUT_FLD(uTimingRegValue, 
                                          MDP_INTF_0_DISPLAY_DATA_HCTL,
                                          DISPLAY_DATA_END_X,
                                          uHsyncEndX - 1);

             out_dword(HWIO_MDP_INTF_0_DISPLAY_DATA_HCTL_ADDR + uRegOffset, uTimingRegValue);       // DISPLAY_DATA_HCTL
             out_dword(HWIO_MDP_INTF_0_ACTIVE_DATA_HCTL_ADDR + uRegOffset, uTimingRegValue);       // ACTIVE_DATA_HCTL
         }
   }

   // Program chroma block 
   if (HAL_MDP_CDM_NONE != eChromaDownModuleId) 
   {

     // 
     // For RGB color space Reset the chroma down module,
     // For YUV color space Program the CSC and down sampler 
     // 

       (void)HAL_MDP_CDM_Output_Format_Config(eChromaDownModuleId, (HAL_MDP_InterfaceId)eInterfaceId, pInterfaceConfig->eInterfacePixelFormat, 0x00); 

       if (HAL_MDP_IS_MDP_PIXEL_YUV_FORMAT(pInterfaceConfig->eInterfacePixelFormat))
       {
         HAL_MDP_RectType            sRectConfig;

         // Configure the down sampler source width and height
         sRectConfig.uWidthInPixels  = pInterfaceConfig->uVisibleWidthInPixels;
         sRectConfig.uHeightInPixels = pInterfaceConfig->uVisibleHeightInPixels;

         (void)HAL_MDP_CDM_OutRect_Config(eChromaDownModuleId, &sRectConfig,0x00);

       }
   }
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_ProgFetchStart_Config()
*/
/*!
* \brief
*     Set up info for Programmable Fetch Start
*
* \param [in]   eInterfaceId         - Interface id
* \param [in]   pIntfProgFetchStart  - Configuration for Programmable Fetch Start
* \param [in]   uSetupFlags          - Reserved
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
static HAL_MDSS_ErrorType HAL_MDP_Interface_ProgFetchStart_Config(HAL_MDP_InterfaceId                  eInterfaceId,
                                                           HAL_MDP_Interface_ProgFetchStartConfigType *pIntfProgFetchStart,
                                                           uint32                                      uSetupFLags)
{
 HAL_MDSS_ErrorType  eStatus = HAL_MDSS_STATUS_SUCCESS;

 // Configure only if feature is enabled for the chipset
 if ((0 != (HAL_MDP_FLAG_FEATURE_PROGRAMMABLE_FETCH_START & pgsMdpHwInfo->sHwInfo.uFeatureFlags)) &&
	 (HAL_MDP_INTERFACE_WRITEBACK_TYPE_BASE > eInterfaceId))
 {
   uint32  uIntfConfig	   = 0x0;
   uint32  uProgFetchStart = 0x0;
   uintPtr uRegOffset	   = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

   // If feature is disabled configure 0, otherwise configure the value provided in pIntfProgFetchStart
   uProgFetchStart = HWIO_OUT_FLD(uProgFetchStart,
								  MDP_INTF_0_PROG_FETCH_START,
								  VALUE,
								  ((pIntfProgFetchStart->bProgFetchStartEnable) ? pIntfProgFetchStart->uProgFetchStartValue : 0));
   out_dword(HWIO_MDP_INTF_0_PROG_FETCH_START_ADDR + uRegOffset, uProgFetchStart);

   // Enable/Disable programmable fetch start
   uIntfConfig = in_dword(HWIO_MDP_INTF_0_INTF_CONFIG_ADDR + uRegOffset);
   uIntfConfig = HWIO_OUT_FLD(uIntfConfig,
							  MDP_INTF_0_INTF_CONFIG,
							  PROG_FETCH_START_EN,
							  pIntfProgFetchStart->bProgFetchStartEnable);
   out_dword(HWIO_MDP_INTF_0_INTF_CONFIG_ADDR + uRegOffset, uIntfConfig);
 }

 return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_DualInterface_Config()
*/
/*!
* \brief
*     Set up configurations for a particular interface
*
* \param [in]   pDualInterfaceConfig  - Dual interface Configuration
* \param [in]   uSetupFlags           - Reserved.
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_Interface_DualInterface_Config(HAL_MDP_Interface_DualInferfaceConfigType *pDualInterfaceConfig,
                                                          uint32                                     uSetupFlags)
{
  HAL_MDSS_ErrorType  eStatus       = HAL_MDSS_STATUS_SUCCESS;
  uint32              uSplitCTRLVal = 0;
  uint32              uFreeRun      = 0;

  if(pDualInterfaceConfig->bEnableDualInterface)
  {
    if (HAL_MDP_INTERFACE_MODE_COMMAND == pDualInterfaceConfig->eInterfaceMode &&
        pDualInterfaceConfig->bEnablePingPongSplit)
    {
      uFreeRun = 0x1;
    }
    
    uSplitCTRLVal = 0;
    uSplitCTRLVal = HWIO_OUT_FLD(uSplitCTRLVal,MDP_SPLIT_DISPLAY_LOWER_PIPE_CTRL, SMART_PANEL_FREE_RUN,       uFreeRun);
    uSplitCTRLVal = HWIO_OUT_FLD(uSplitCTRLVal,MDP_SPLIT_DISPLAY_LOWER_PIPE_CTRL, TE_LINE_INTERVAL_WATERMARK, 0x0);
    out_dword(HWIO_MDP_SPLIT_DISPLAY_LOWER_PIPE_CTRL_ADDR, uSplitCTRLVal);
    
  }
  return eStatus;
}


/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_Setup()
*/
/*!
* \brief
*     Set up configurations for a particular interface
*
* \param [in]   eInterfaceId      - Interface id
* \param [in]   pInterfaceConfig  - Configuration for the selected interface
* \param [in]   uSetupFlags       - Reserved.
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_Interface_Setup(HAL_MDP_InterfaceId           eInterfaceId,
                                           HAL_MDP_InterfaceConfigType   *pInterfaceConfig,
                                           uint32                        uSetupFlags)
{
  HAL_MDSS_ErrorType      eStatus  = HAL_MDSS_STATUS_SUCCESS;

  // Interface Timing Generator Configuration
  if(NULL != pInterfaceConfig->pIntrTimingGenConfig)
  {
    if(HAL_MDP_INTERFACE_WRITEBACK_TYPE_BASE > eInterfaceId) // Physical interface
    {
      uint32 uRegOffset = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

      // Configure Ping Pong Mux in the interface
      out_dword(HWIO_MDP_INTF_0_MUX_ADDR + uRegOffset, pInterfaceConfig->ePingPongId - HAL_MDP_PINGPONG_0);

      HAL_MDP_Interface_TimingGen_Config((HAL_MDP_PhyInterfaceId)eInterfaceId,
                                         pInterfaceConfig->eChromaDownModuleId,
                                         pInterfaceConfig->pIntrTimingGenConfig,
                                         0x00);
    }
    else
    {
      // Configure Ping Pong Mux in the interface
      out_dword(HWIO_MDP_WB_2_MUX_ADDR, pInterfaceConfig->ePingPongId - HAL_MDP_PINGPONG_0);

      // NO timing generator support for write-back interfaces
    }
  }

  // Interface CRC configuration
  if(NULL != pInterfaceConfig->pIntrMISRConfig)
  {
    HAL_MDP_Interface_MISR_Config(eInterfaceId,
                                  pInterfaceConfig->pIntrMISRConfig,
                                  0x00);
  }

  // TPG - Test Pattern Generation
  if(NULL != pInterfaceConfig->pIntrTPGConfig)
  {
    HAL_MDP_Interface_TPG_Config((HAL_MDP_PhyInterfaceId)eInterfaceId,
                                 pInterfaceConfig->pIntrTPGConfig,
                                 0x00);
  }

  // Split display configuration
  if(NULL != pInterfaceConfig->pDualInterfaceConfig)
  {
    HAL_MDP_Interface_DualInterface_Config(pInterfaceConfig->pDualInterfaceConfig,
                                           uSetupFlags);
  }

  if (NULL != pInterfaceConfig->pIntfProgFetchStartConfig)
  {
    eStatus = HAL_MDP_Interface_ProgFetchStart_Config(eInterfaceId, 
                                                      pInterfaceConfig->pIntfProgFetchStartConfig, 
                                                      0x00);
  }

  // Chroma module configuration 
  if (NULL != pInterfaceConfig->pChromaDownConfig)
  {
    eStatus = HAL_MDP_CDM_Setup(pInterfaceConfig->eChromaDownModuleId, pInterfaceConfig->pChromaDownConfig, 0x00);
  }

  // Auto-refresh enable for smart panel
  if(NULL != pInterfaceConfig->pIntrAutoRefreshEnable)
  {
    HAL_MDP_Interface_AutoRefresh_Enable(eInterfaceId, pInterfaceConfig->pIntrAutoRefreshEnable, 0x00);
  }

  // Tear check enable for smart panel
  if(NULL != pInterfaceConfig->pIntrTEEnable)
  {
     HAL_MDP_Interface_TE_Enable(eInterfaceId, pInterfaceConfig->pIntrTEEnable, 0x00);
  }

  // Vsync counter enable for smart panel
  if(NULL != pInterfaceConfig->pIntrVsyncEnable)
  {
     HAL_MDP_Interface_Vsync_Enable(eInterfaceId, pInterfaceConfig->pIntrVsyncEnable, 0x00);
  }
  
  // Vsync counter configuration for smart panel
  if(NULL != pInterfaceConfig->pIntrVsyncConfig)
  {
     HAL_MDP_Interface_Vsync_Config(eInterfaceId, pInterfaceConfig->pIntrVsyncConfig, 0x00);
  }
  
  // Tear check configuration for smart panel
  if(NULL != pInterfaceConfig->pIntrTEConfig)
  {
     HAL_MDP_Interface_TE_Config(eInterfaceId, pInterfaceConfig->pIntrTEConfig, 0x00);
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_GetProperty()
*/
/*!
* \brief
*     Retrieve information for a particular interface
*
* \param [in]   eInterfaceId          - Interface id
* \param [out]  pInterfaceInfoData    - Info data
* \param [in]   uGetPropertyFlags     - Reserved
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_Interface_GetProperty(HAL_MDP_InterfaceId             eInterfaceId,
                                                 HAL_MDP_InterfaceInfoParamsType *pInterfaceInfoData,
                                                 uint32                          uGetPropertyFlags)
{
  HAL_MDSS_ErrorType   eStatus  = HAL_MDSS_STATUS_SUCCESS;

  if (NULL != pInterfaceInfoData->pMISRReturnType)
  {
    // MISR Complete status
    if (NULL != pInterfaceInfoData->pMISRReturnType->pMISR_Status_Info)
    {     
        HAL_MDP_Interface_MISR_Status(eInterfaceId,
                                      pInterfaceInfoData->pMISRReturnType,
                                      HAL_MDP_MISR_STATUS_READ_STATE,
                                      pInterfaceInfoData->pMISRReturnType->pMISR_Status_Info->eConnectType,
                                      pInterfaceInfoData->pMISRReturnType->pMISR_Status_Info->eStreamId);
    }

    // MISR Signature
    if (NULL != pInterfaceInfoData->pMISRReturnType->pMISR_CRC_Info)
    {
        HAL_MDP_Interface_MISR_Status(eInterfaceId,
                                      pInterfaceInfoData->pMISRReturnType,
                                      HAL_MDP_MISR_STATUS_READ_SIGNATURE,
                                      pInterfaceInfoData->pMISRReturnType->pMISR_CRC_Info->eConnectType,
                                      pInterfaceInfoData->pMISRReturnType->pMISR_CRC_Info->eStreamId);
    } 
  }

  if (NULL != pInterfaceInfoData->pFrameRefreshInfo)
  {    
    // Video mode interface Frame Rate Information
    if (HAL_MDP_INTERFACE_MODE_VIDEO == pInterfaceInfoData->eInterfaceMode)
    {
      HAL_MDP_Interface_RefreshInfo(eInterfaceId,
                                    pInterfaceInfoData->pFrameRefreshInfo,
                                    0x00);
    }
    else if (HAL_MDP_INTERFACE_MODE_COMMAND == pInterfaceInfoData->eInterfaceMode)
    {
      // Tear block frame rate information
      HAL_MDP_Interface_TearRefreshInfo(eInterfaceId,
                                        pInterfaceInfoData->pFrameRefreshInfo,
                                        0x00);
    }
  }

  if (NULL != pInterfaceInfoData->pEngineStatus)
  {
    // Video mode interface Engine status
    if (HAL_MDP_INTERFACE_MODE_VIDEO == pInterfaceInfoData->eInterfaceMode)
    {
      HAL_MDP_Interface_EngineStatus(eInterfaceId,
                                     pInterfaceInfoData->pEngineStatus,
                                     0x00);
    }
    else if (HAL_MDP_INTERFACE_MODE_COMMAND == pInterfaceInfoData->eInterfaceMode)
    {
      // VSYNC counter status info
      HAL_MDP_Interface_TearEngineStatus(eInterfaceId, 
                                         pInterfaceInfoData->pEngineStatus, 
                                         0x00);
    }
  }

  // Interface Timing Generation Parameters
  if(NULL != pInterfaceInfoData->pTimingGenInfo)
  {
    HAL_MDP_Interface_TimingGenInfo(eInterfaceId,
                                    pInterfaceInfoData->pTimingGenInfo,
                                    0x00);
  }

  // Interface status
  if(NULL != pInterfaceInfoData->pbActive)
  {
    HAL_MDP_Interface_GetStateInfo( eInterfaceId,
                                    pInterfaceInfoData->pbActive,
                                    0x00);
  }

  // Dual Interface (split display) status
  if(NULL != pInterfaceInfoData->pbDualInterfaceEnabled)
  {
    HAL_MDP_Interface_GetDualInterfaceInfo( eInterfaceId,
                                            pInterfaceInfoData->pbDualInterfaceEnabled,
                                            0x00);
  }

  return eStatus;
}

/****************************************************************************
*
** FUNCTION: HAL_MDP_Interface_Enable()
*/
/*!
* \brief
*     Enable/Disable a particular interface
*
* \param [in]   eInterfaceId            - Interface id
* \param [in]   bEnable                 - True for enable, false for disable 
* \param [in]   uEnableFlags            - Reserved
*
* \retval HAL_MDSS_ErrorType
*
****************************************************************************/
HAL_MDSS_ErrorType HAL_MDP_Interface_Enable(HAL_MDP_InterfaceId   eInterfaceId,
                                            bool32                bEnable,
                                            uint32                uEnableFlags)
{
   HAL_MDSS_ErrorType      eStatus              = HAL_MDSS_STATUS_SUCCESS;
   uintPtr                 uRegOffset           = 0x0;

   if(HAL_MDP_INTERFACE_WRITEBACK_TYPE_BASE > eInterfaceId) // Physical interface
   {
      uRegOffset = HAL_MDP_Get_PhyInterface_BaseOffset((HAL_MDP_PhyInterfaceId)eInterfaceId);

      out_dword(HWIO_MDP_INTF_0_TIMING_ENGINE_EN_ADDR + uRegOffset, bEnable);
   }

   return eStatus;
}

#ifdef __cplusplus
}
#endif
