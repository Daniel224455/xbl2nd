/*==========================================================================================================
  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================================================*/

#ifndef _DISPLAYDIAG_H_
#define _DISPLAYDIAG_H_


/* Definition of the log buffer global
  */
#define DISPLAY_DIAG_LOG_BUFFER        gpDiagLogBuffer
#define DISPLAY_DIAG_GETBUFFER()       (&(DISPLAY_DIAG_LOG_BUFFER))


/*****
 * Diagnostic structures
 */

/* Diag Header
 */
typedef struct 
{
   char         uMagic[4];    // Magic header ('DDL#') Display Diagnostic Log Version
   unsigned int uEntrySize;   // Size of one log entry in bytes.
   unsigned int uStructSize;  // Entire size of the structure including this header.
   unsigned int uNumEntries;  // Number of entries in the log
} DisplayDiagHeader;


/* Diag entry 
 */
typedef struct
{
    // Sequence number or timestamp of the event
    unsigned int     uSequenceNum;

    // Bitmask that represents the module that generated the entry.
    struct 
    {
        unsigned int uDisplayId:4;
        unsigned int uDPLinkStats:1;
        unsigned int uReserved:26;
    } uModule;


    // Union of the structure per module.
    union 
    
    {
        struct
        
        {
            struct 
            {
               unsigned int uReserved:4;
               unsigned int uMaxLinkRate:14;  // Link rate (Mhz)
               unsigned int uMaxNumLanes:4;   // Number of lanes supported
               unsigned int uMaxSwing:5;   // Maxium voltage level swing
               unsigned int uMaxPreEmph:5; // Maxium preempyasis level 
            } uSinkCaps;
            struct 
            {
               unsigned int uTrainingType:3;
               unsigned int uTrainingPhase:1;    
               unsigned int uLinkRate:14;   // Configured link rate
               unsigned int uNumLanes:4;   // Configured number of lanes
               unsigned int uMaxSwing:5;   // Configured voltage level swing
               unsigned int uMaxPreEmph:5; // Configured preempyasis level 
            } uSourceConfig;
            struct 
            {
               unsigned int uLaneStatus0_1:8;
               unsigned int uLaneStatus2_3:8;  
               unsigned int uLaneAlignStatus:8; // Channel equilization
               unsigned int uSinkStatus:8;
            } uResult1;
            struct 
            {
               unsigned int uAdjustRequest0_1:8;
               unsigned int uAdjustRequest2_3:8;  
               unsigned int uReserved:16; 
            } uResult2;            
        } sDPLinkStats;
        unsigned int uData[4];
    }uDiag;
} DisplayDiagEntry;

/* Top level diag structure */
typedef struct
{
   DisplayDiagHeader sHeader;
   DisplayDiagEntry  DisplayDiagEntry[32];
} DisplayDiagLogType;


/* Global log buffer variable */
extern DisplayDiagLogType DISPLAY_DIAG_LOG_BUFFER;


/**********************************************************************************************************************

FUNCTION: DiagLog_Init()
   
   Initialize the diagnostic buffer

Parameters:
   pLog   -[in] Pointer allocation to the diagnostic log.

Return: 
   1 if successful.
    

**********************************************************************************************************************/
static inline uint32 DiagLog_Init(DisplayDiagLogType *pLog)
{
    uint32 result = 0;
    
    if (NULL != pLog)
    {
       pLog->sHeader.uMagic[0]   = 'D';
       pLog->sHeader.uMagic[1]   = 'D';
       pLog->sHeader.uMagic[2]   = 'L';
       pLog->sHeader.uMagic[3]   = 1;        
       pLog->sHeader.uEntrySize  = sizeof(DisplayDiagEntry);
       pLog->sHeader.uStructSize = sizeof(DisplayDiagLogType);
       pLog->sHeader.uNumEntries = 0;
       result = 1;
    }

    return result;
}


/**********************************************************************************************************************

FUNCTION: DiagLog_LinkTraining()
   
   Log a diagnostic entry for link training

Parameters:
   pLog             -[in] Pointer allocation to the diagnostic log.
   uDisplayId       -[in] Display id
   uTrainingType    -[in] Training type, none, full, fast.
   uTrainingPhase   -[in] Phase of the training (clock recovery, channel equalization)
   uMaxLinkRateKhz  -[in] Maxiumum link rate supported by sink.
   uMaxLanes        -[in] Maxiumum lanes supported by sink.
   uMaxSwing        -[in] Maxiumum swing level supported by sink.
   uMaxPreemph      -[in] Maxiumum preemphyasis level supported by sink.
   uLinkRateKhz     -[in] Link rate during training.
   uLanes           -[in] Number of lanes during training.
   uSwing           -[in] Swing level during training.
   uPreemph         -[in] Premphyasis level rate during training.
   pSinkStatus      -[in] 6 byte array of link status (DPCD registers 202h-207h)
   
Return: 
   1 if successful.
    
**********************************************************************************************************************/
static inline uint32 DiagLog_LinkTraining(
                      DisplayDiagLogType *pLog,
                      uint32 uDisplayId,
                      uint8  uTrainingType,
                      uint8  uTrainingPhase,
                      uint32 uMaxLinkRateKhz,
                      uint8  uMaxLanes,
                      uint8  uMaxSwing,
                      uint8  uMaxPreemph,
                      uint32 uLinkRateKhz,
                      uint8  uLanes,
                      uint8  uSwing,
                      uint8  uPreemph,
                      uint8 *pSinkStatus)
{
   uint32 result            = 0;
   
   if ((NULL != pLog) &&
       (sizeof(DisplayDiagEntry)   == pLog->sHeader.uEntrySize) &&
       (sizeof(DisplayDiagLogType) == pLog->sHeader.uStructSize))
   { 
       uint32            uTableSize = (pLog->sHeader.uStructSize - sizeof(DisplayDiagHeader)) / sizeof(DisplayDiagEntry);
       uint32            uIndex     = pLog->sHeader.uNumEntries % uTableSize; 
       DisplayDiagEntry *pEntry     = &pLog->DisplayDiagEntry[uIndex];

       pEntry->uSequenceNum                                    = ++pLog->sHeader.uNumEntries;
       pEntry->uModule.uDisplayId                              = uDisplayId;
       pEntry->uModule.uDPLinkStats                            = 1;
       pEntry->uDiag.sDPLinkStats.uSinkCaps.uMaxLinkRate       = uMaxLinkRateKhz/100;
       pEntry->uDiag.sDPLinkStats.uSinkCaps.uMaxNumLanes       = uMaxLanes;
       pEntry->uDiag.sDPLinkStats.uSinkCaps.uMaxSwing          = uMaxSwing;
       pEntry->uDiag.sDPLinkStats.uSinkCaps.uMaxPreEmph        = uMaxPreemph;
       
       pEntry->uDiag.sDPLinkStats.uSourceConfig.uTrainingType  = uTrainingType;
       pEntry->uDiag.sDPLinkStats.uSourceConfig.uTrainingPhase = uTrainingPhase;
       pEntry->uDiag.sDPLinkStats.uSourceConfig.uLinkRate      = uLinkRateKhz/100;
       pEntry->uDiag.sDPLinkStats.uSourceConfig.uNumLanes      = uLanes;
       pEntry->uDiag.sDPLinkStats.uSourceConfig.uMaxSwing      = uSwing;
       pEntry->uDiag.sDPLinkStats.uSourceConfig.uMaxPreEmph    = uPreemph;

       if (NULL != pSinkStatus)
       {
         pEntry->uDiag.sDPLinkStats.uResult1.uLaneStatus0_1    = pSinkStatus[0];
         pEntry->uDiag.sDPLinkStats.uResult1.uLaneStatus2_3    = pSinkStatus[1];
         pEntry->uDiag.sDPLinkStats.uResult1.uLaneAlignStatus  = pSinkStatus[2];
         pEntry->uDiag.sDPLinkStats.uResult1.uSinkStatus       = pSinkStatus[3];
         pEntry->uDiag.sDPLinkStats.uResult2.uAdjustRequest0_1 = pSinkStatus[4];
         pEntry->uDiag.sDPLinkStats.uResult2.uAdjustRequest2_3 = pSinkStatus[5];
       }
       result = 1;
   }   

   return result;
}


#endif // _DISPLAYDIAG_H_
