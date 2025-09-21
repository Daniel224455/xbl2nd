/*==========================================================================================================
 
  File: dp_link.c
 
  Source file for DP main link related functionality 
   
  Copyright (c) 2016-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
===========================================================================================================*/

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
#define EXTRA_PIXCLK_CYCLE_DELAY 5
#define MAX_LINK_TRAINING_LOOP   5
#define RATIO_SCALE              1.01


#ifndef MAX
#define MAX(a,b)                 (a > b ? a : b)
#endif

#define fabs(a)                  ((a > 0 ? a : -a))

#define MAX_16BITS_VALUE         ((1 << 16) - 1)     /* 16 bits value */

#define DP_NVID_FACTOR_1_62G     1
#define DP_NVID_FACTOR_2_70G     1
#define DP_NVID_FACTOR_5_40G     2
#define DP_NVID_FACTOR_8_10G     3

/*---------------------------------------------------------------------------------------------------------------------
 * Global Data Definitions
 *-------------------------------------------------------------------------------------------------------------------*/

DP_PixelFormat_Info       gsDPPixelFormatInfo[DP_PIXEL_FORMAT_MAX] = 
{
  /*uBitsPerPixel   uBitsPerComponent*/
  {  0,             0  },       /*DP_PIXEL_FORMAT_NONE          */
  { 18,             6  },       /*DP_PIXEL_FORMAT_RGB_666_18BPP */
  { 24,             8  },       /*DP_PIXEL_FORMAT_RGB_888_24BPP */
  { 30,            10  },       /*DP_PIXEL_FORMAT_RGB_101010_30BPP */
};

/*---------------------------------------------------------------------------------------------------------------------
 * Function pre-declarations
 *-------------------------------------------------------------------------------------------------------------------*/

/* contains input and outtput of M/N/D calculation */
typedef struct
{
   uint64    uSrcRate;
   uint64    uTgtRate;
   uint32    M;
   uint32    N;
   uint32    D;
   uint32    not_N_minus_M;
   uint32    not_2D;
} MND_DESC;


/**********************************************************************************************************************

FUNCTION: mn_reduce_bits()
          -- prevent both M and N not greater than  16 BIts value

Parameters:

Return: 

**********************************************************************************************************************/
void mn_reduce_bits(uint32 *m_val, uint32 *n_val, float l_ratio)
{
    uint32 l_m_val = *m_val;
    uint32 l_n_val = *n_val;
    
    uint32 l_m_tmp0;
    uint32 l_m_tmp1;
    uint32 l_n_tmp0;
    uint32 l_n_tmp1;
    uint32 l_n_minus_m0;
    uint32 l_n_minus_m1;
    
    float l_err_d;
    float l_err_u;

    // Round Down
    l_m_tmp0 = (uint32)(l_m_val/2);    /* right shit one bit */
    l_err_d = ((float)l_m_tmp0) / ((float)l_m_val / 2.0F); // Error by truncating
    l_n_tmp0 = (uint32)(((float)l_n_val) / 2.0 * l_err_d + 0.5 );
    

    // Round Up
    l_m_tmp1 = (uint32) ((float)(l_m_val) / 2.0) + 1;    /* right shit one bit */
    l_err_u = ((float)l_m_tmp1) / ((float)l_m_val / 2.0F); // Error by truncation and round
    l_n_tmp1 = (uint32) (((float)l_n_val) / 2.0 * l_err_u + 0.5);
    
    if((l_m_tmp0 <= MAX_16BITS_VALUE) && (l_m_tmp1 > MAX_16BITS_VALUE))  /* less than 16 bits */
    {
        *m_val = l_m_tmp0;
        *n_val = l_n_tmp0;
        return; 
    }

    // Do the checking for n-m
    l_n_minus_m0 = l_n_tmp0 - l_m_tmp0;
    l_n_minus_m1 = l_n_tmp1 - l_m_tmp1;
    if((l_n_minus_m0 <= MAX_16BITS_VALUE) && (l_n_minus_m1 > MAX_16BITS_VALUE)) 
    {
        *m_val = l_m_tmp0;
        *n_val = l_n_tmp0;
        return; 
    }
    
    if(fabs((float)l_ratio - ((float)l_m_tmp0/(float)l_n_tmp0)) < fabs((float)l_ratio - ((float)l_m_tmp1/(float)l_n_tmp1))) 
    {
        *m_val = l_m_tmp0;
        *n_val = l_n_tmp0;
        return;
    }
    
    *m_val = l_m_tmp1;
    *n_val = l_n_tmp1;
    
    return;
}

/**********************************************************************************************************************

FUNCTION: mn_value_search()
          -- M, N finder

Parameters:

Return: 

**********************************************************************************************************************/
void mn_value_search(uint32 *m_val, uint32 *n_val)
{
    uint32 l_m_val = *m_val;
    uint32 l_n_val = *n_val;
    float  l_ratio;
    
    uint32 l_n_tmp;
    
    l_ratio = (float)*m_val / (float)*n_val;

    while (l_m_val > MAX_16BITS_VALUE)    /* more than 16 bits */
    {
        mn_reduce_bits(&l_m_val, &l_n_val, l_ratio);
    }
    
    // Check N for overflow.
    // N is programmed as ~(N-M)
    l_n_tmp = (l_n_val - l_m_val);
    while(l_n_tmp > MAX_16BITS_VALUE) 
    {    
       mn_reduce_bits(&l_m_val, &l_n_val, l_ratio);           
       l_n_tmp = (l_n_val - l_m_val);
    }
    
    *m_val = l_m_val;
    *n_val = l_n_val;
}

/**********************************************************************************************************************

FUNCTION: mm_value_gcd()
       -- GCD calculation

Parameters:

Return: 
   uint64

**********************************************************************************************************************/
uint64 mn_value_gcd(uint64 numerator, uint64 denominator)
{
    uint64 num1;
    uint64 num2;
    uint64 tmp;

    num1 = numerator;
    num2 = denominator;
    tmp  = 0;
    while (num2 != 0)
    {
        tmp  = num2; 
        num2 = num1 % num2;  
        num1 = tmp;
    }

    DP_LOG_MESSAGE_INFO("Display: mn_value_gcd: numerator=%llu denominator=%llu gcd=%llu\n", numerator, denominator, num1);
    return num1;

}

/**********************************************************************************************************************

FUNCTION: mn_value_finder()

Parameters:

Return: 
   int

**********************************************************************************************************************/
DP_Status mn_value_finder(uint64 src_rate, uint64 target_rate, uint32 *numerator, uint32 *denominator)
{
    uint32 num  = 1;
    uint32 den  = 1;

    uint64 gcd;;

    uint64 src_freq;   
    uint64 target_freq;
    double  error = 0;

    if ((float)target_rate > (2.0/3.0*(float)src_rate))
    {
        DP_LOG_MESSAGE_ERROR("Target Clock frequency (%llu) must be less than 2/3 Source clock frequency (%llu) in Dual-edge mode\n", target_rate, src_rate);
   	return DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
    }
    
    src_freq    = (uint64)(src_rate * 1000000);
    target_freq = (uint64)(target_rate * 1000000);

    gcd = mn_value_gcd(target_freq, src_freq);
    num  = (uint32)(target_freq/gcd);
    den  = (uint32)(src_freq/gcd);

    mn_value_search(&num, &den);

    error = (double)target_freq - ((double)src_freq * (double)num / (double)den);

    if (error != 0)
    {
        DP_LOG_MESSAGE_ERROR("mn_value_finder: can NOT find best M=%d N=%d\n" , num, den);
    }
    
    *numerator  = num;
    *denominator  = den;
    
    return DP_STATUS_SUCCESS;
}

/**********************************************************************************************************************

FUNCTION: mnd_finder()
       -- only support 16 bits of M, N and D 

Parameters:
   mnd           -[in ] 

Return: 
   int

**********************************************************************************************************************/
int mnd_finder(MND_DESC *mnd)
{
    uint32 error = 0;
    uint32 N;
    uint32 M;

    error =  mn_value_finder(mnd->uSrcRate,mnd->uTgtRate, &M, &N);

    mnd->M             = M;
    mnd->N             = N;
    mnd->D             = N / 2;
    mnd->not_N_minus_M = ~(N - M) & 0xffff; 
    mnd->not_2D        = ~N & 0xffff; 

    return error;
}

/**********************************************************************************************************************

FUNCTION: Div_RoundUp()
   
   This function will do divide first and round up

Parameters:
   num           -[in ] 
   div           -[in ] 

Return: 
   int

**********************************************************************************************************************/
static int Div_RoundUp(double num, int div)
{
  double k2;

  k2 = num / div;
  if (num - (int)k2*div)
  {
    k2 = (int)k2 + 1;
  }
  return (int)k2 * div;
}

/**********************************************************************************************************************

FUNCTION: Ceil()
   
   This function will do round up

Parameters:
   Number                           -[in ] 

Return: 
   int

**********************************************************************************************************************/
int Ceil(double Number)
{
  if(Number >= 0)
  {
    if(Number == (int)Number )
      return (int)Number;
    else
      return (int)Number + 1;
  }
  else
  {
    return (int)Number;
  }
}

/*********************************************************************************************
*
** FUNCTION: DP_CalculateTU()
*/
/*!
* \DESCRIPTION
*     This function will calculate the TU based the the lane #, link clock, pixel clock, color format.
*
* \param [in]   psDisplayInfo         - panel info
* \param [out]  psTUConfig            - The TU parameters
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_CalculateTU(DP_TUSizePararmType             *psDisplayInfo,
                          HAL_DP_TUConfigType             *psTUConfig)
{
  DP_Status  eStatus              = DP_STATUS_SUCCESS;

  bool32   bEvenDistribution;

  double   fError;
  double   fNewError;
  double   fOldError;
  double   fRatio;
  double   fOriginalRatio;
  double   fAverageValid;
  double   fResultingValid;
  double   fEffectiveValid;
  double   fRemainderTUs;
  double   fRemainderTUsUpper;
  double   fRemainderTUsLower;

  int32    iTUDesired                  = 0;
  int32    iTotalValid                 = 0;
  int32    iNewValidBoundaryLink       = 0;
  int32    iNumberOfTUsPerLine         = 0;
  int32    iNumberOfTUs                = 0; 
  int32    iPairedTUs                  = 0;
  int32    iExtraRequiredBytesNew      = 0;
  int32    iExtraPixClkCycles          = 0;
  int32    iExtraPixClkCyclesInLinkClk = 0;
  int32    iFillerSize                 = 0;
  int32    iTUIndex                    = 0;
  int32    iUpperBoundaryIndex         = 0;
  int32    iLowerBoundaryIndex         = 0;
  int32    extra_buffer_margin         = Ceil((double)psDisplayInfo->uLinkClockInKhz * EXTRA_PIXCLK_CYCLE_DELAY  / psDisplayInfo->uPixelClockInKhz);

  fRatio                           = ((double)psDisplayInfo->uPixelClockInKhz * psDisplayInfo->uBPP) / 
                                     ((double)psDisplayInfo->uLinkClockInKhz  * psDisplayInfo->uNumberOfLanes * 8);

  fOriginalRatio                   = fRatio;

  if( (psDisplayInfo->uVisWidth % psDisplayInfo->uNumberOfLanes != 0) && 
      (fRatio < 1))
  {
    fRatio = fRatio * RATIO_SCALE;
    fRatio = fRatio < 1 ? fRatio : 1;
  }

  DP_LOG_MESSAGE_INFO("DP_calculateTU: linkrate=%d pclk=%d\n", psDisplayInfo->uLinkClockInKhz,psDisplayInfo->uPixelClockInKhz);

  fError                           = 1000;

  for(iTUIndex = 32; iTUIndex <= 64; iTUIndex++)
  {
     fNewError = Ceil(fRatio * iTUIndex) -  (fRatio * iTUIndex);
     if(fNewError < fError)
     {
        fError = fNewError;
        DP_LOG_MESSAGE_INFO("DP_CalculateTU: FOUND tu=%d\n", iTUIndex);
        iTUDesired = iTUIndex;
     }
  }

  if (fError == 0)
  {
      iNewValidBoundaryLink       = Ceil(fRatio * iTUDesired);
      iNumberOfTUs    = (int)((psDisplayInfo->uVisWidth * psDisplayInfo->uBPP / 8) / iNewValidBoundaryLink);
      iExtraRequiredBytesNew = Ceil((iNumberOfTUs + 1) * (iNewValidBoundaryLink - fOriginalRatio * iTUDesired));
      iExtraPixClkCycles          = Ceil((double)iExtraRequiredBytesNew * 8/ psDisplayInfo->uBPP); 
      iExtraPixClkCyclesInLinkClk = Ceil((double)iExtraPixClkCycles * psDisplayInfo->uLinkClockInKhz / psDisplayInfo->uPixelClockInKhz);
      iFillerSize                 = Ceil(iTUDesired - iNewValidBoundaryLink);

      psTUConfig->uDelayStartLink = (MAX(iExtraRequiredBytesNew, iExtraPixClkCyclesInLinkClk) + iFillerSize + extra_buffer_margin);
      psTUConfig->uValidBoundaryLink  = iNewValidBoundaryLink;
      psTUConfig->bBoundaryModEnable  = FALSE;
      psTUConfig->uUpperBoundaryCount = 0;
      psTUConfig->uLowerBoundaryCount = 0;
      psTUConfig->uValidLowerBoundary = 0;
      psTUConfig->uTuSize             = iTUDesired;
      DP_LOG_MESSAGE_INFO("DP_CalculateTU: tu=%d valid_boundary_link=%d delay_start_link=%d\n", iTUDesired, psTUConfig->uValidBoundaryLink, psTUConfig->uDelayStartLink);
  }
  else 
  {
     fError                           = 1000;

     for(iTUIndex = 32; iTUIndex <= 64; iTUIndex++)
     {
        for( iUpperBoundaryIndex = 1; iUpperBoundaryIndex<= 15; iUpperBoundaryIndex++)
        {
           for(iLowerBoundaryIndex = 1; iLowerBoundaryIndex<= 15; iLowerBoundaryIndex++)
           {
             iNewValidBoundaryLink   = Ceil(fRatio * iTUIndex);
             fAverageValid           = (double)(iUpperBoundaryIndex *  iNewValidBoundaryLink + 
                                           iLowerBoundaryIndex * (iNewValidBoundaryLink - 1)) /
                                          (iUpperBoundaryIndex + iLowerBoundaryIndex);
             iNumberOfTUs        = (int)((psDisplayInfo->uVisWidth * psDisplayInfo->uBPP / 8) / fAverageValid);
             iNumberOfTUsPerLine = (int)((double)iNumberOfTUs/ psDisplayInfo->uNumberOfLanes);
             iPairedTUs          = (int)((double)iNumberOfTUsPerLine / (iUpperBoundaryIndex + iLowerBoundaryIndex));
             fRemainderTUs       = iNumberOfTUsPerLine - iPairedTUs * (iUpperBoundaryIndex + iLowerBoundaryIndex);

             //finding how many upper fRemainderTUs boundary TUs left
             if ((fRemainderTUs - iUpperBoundaryIndex) > 0) 
             {
                fRemainderTUsUpper = iUpperBoundaryIndex;
                fRemainderTUsLower = fRemainderTUs - iUpperBoundaryIndex;
             }
             else
             {
                fRemainderTUsUpper = fRemainderTUs;
                fRemainderTUsLower = 0;
             }
             iTotalValid = (int32)(iPairedTUs   * 
                      (iUpperBoundaryIndex * iNewValidBoundaryLink + 
                      iLowerBoundaryIndex  * (iNewValidBoundaryLink - 1)) +
                      (fRemainderTUsUpper  * iNewValidBoundaryLink) + 
                      (fRemainderTUsLower  * (iNewValidBoundaryLink - 1)));

             if(iNumberOfTUsPerLine == 0)
             {
                fEffectiveValid = 0;
             }
             else
             {
                fEffectiveValid = iTotalValid*1.0 / iNumberOfTUsPerLine;
             }

             fNewError = fEffectiveValid - fRatio * iTUIndex;         /*this is error with considering line width   */
             fOldError = fAverageValid   - fRatio * iTUIndex;         /*this is error without considering line width*/
             if((iNumberOfTUs % psDisplayInfo->uNumberOfLanes) == 0)
             {
                bEvenDistribution = TRUE;
             }
             else
             {
                bEvenDistribution = FALSE;
             }

             /*
              * to reduce complexity of calculation, checking "hporch < delay_start"
              * is not enforce here since horizontal porch is very large and delay_start
              * usually is small. On the other words, the possibility of fail at checking
              * "hporch < delay_start" is very rare and it can be ignored.
              */
             if ( (bEvenDistribution == TRUE      ) &&
                  (fOldError > 0                  ) && 
                  (fNewError > 0                  ) && 
                  (fNewError < fError             ) && 
                  ((iNewValidBoundaryLink - 1) > 0) )
             {
                 fError                          = fNewError;
                 psTUConfig->bBoundaryModEnable  = TRUE;
                 psTUConfig->uUpperBoundaryCount = iUpperBoundaryIndex;
                 psTUConfig->uLowerBoundaryCount = iLowerBoundaryIndex;
                 iTUDesired                      = iTUIndex; 
                 psTUConfig->uValidBoundaryLink  = iNewValidBoundaryLink;
                 psTUConfig->uValidLowerBoundary = iNewValidBoundaryLink -1;
             }
           }
        }

        if (psTUConfig->bBoundaryModEnable == TRUE)
        {    
           fResultingValid = ((double)(psTUConfig->uUpperBoundaryCount * psTUConfig->uValidBoundaryLink + 
                                  psTUConfig->uLowerBoundaryCount * (psTUConfig->uValidBoundaryLink - 1))) / 
                                 (psTUConfig->uUpperBoundaryCount + psTUConfig->uLowerBoundaryCount);
           iNumberOfTUs    = (int)((psDisplayInfo->uVisWidth * psDisplayInfo->uBPP / 8) / fResultingValid);
           iExtraRequiredBytesNew = Ceil((iNumberOfTUs + 1) * (fResultingValid - fOriginalRatio * iTUDesired)) + 
                                 Ceil(psTUConfig->uUpperBoundaryCount                        * 
                                    (psTUConfig->uValidBoundaryLink - fOriginalRatio * iTUDesired) *
                                    psDisplayInfo->uNumberOfLanes);
           iExtraPixClkCycles          = Ceil((double)iExtraRequiredBytesNew * 8/ psDisplayInfo->uBPP); 
           iExtraPixClkCyclesInLinkClk = Ceil((double)iExtraPixClkCycles * psDisplayInfo->uLinkClockInKhz /
                                         psDisplayInfo->uPixelClockInKhz);
           iFillerSize                 = Ceil(iTUDesired - psTUConfig->uValidBoundaryLink);
           psTUConfig->uDelayStartLink = (MAX(iExtraRequiredBytesNew, iExtraPixClkCyclesInLinkClk) + 
                                    iFillerSize + 1 + extra_buffer_margin); //recalculating delay start
           psTUConfig->uTuSize         = iTUDesired;
        }
        else
        {
            eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
        }
     }
  }

  DP_LOG_MESSAGE_INFO("DP_CalculateTU: tu=%d\n", psTUConfig->uTuSize);

   return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: DP_CalculateLink()
*/
/*!
* \DESCRIPTION
*     This function will Calculate the Link settings, such as clock freq, number of lane needed.
*
* \param [in]   eDeviceId             - DP device Id
* \param [in]   psDPCDSinkCap         - Pointer to sink DPCD capabilities
* \param [in]   psPanelInfo           - Pointer to panel info corresponding to current mode
* \param [out]  pPixClkInfo           - Pointer to pixel clock info (dividers, etc.)
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status  DP_CalculateLink(DP_DeviceIDType           eDeviceId,
                            DP_DPCDSinkCapType       *psDPCDSinkCap,
                            DP_PanelInfoType         *psPanelInfo,
                            DP_PixelClk_Info         *pPixClkInfo)
{
  DP_Status             eStatus              = DP_STATUS_SUCCESS;
  DP_DeviceCtxType     *pDeviceCtx           = DPHOST_GET_CTX(eDeviceId);  
  uint32                uLinkRate            = 0;
  uint32                uMinDelta            = 0x7fffffff;
  uint32                uDesiredLaneNumber   = 0;
  uint32                uInputClk            = 0;
  uint32                uBitRate;
  uint32                uLaneNumber;     
  uint32                uLaneRate;
  uint32                uPclkRate;
  uint64                uTempQ16;
  uint32                uTotalWidth;
  uint32                uTotalHeight;  
  uint32                uNvidFactor = 0;
  MND_DESC              mnd;

  if (DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_162MBps == pDeviceCtx->uLinkRateInKhz)
  {
    /* Preferred link rate is 162MHz */
    uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_162MBps;
    uInputClk         = DP_PLL_OUTPUT_RATE_810MHZ;
    uNvidFactor       = DP_NVID_FACTOR_1_62G;
  }
  else if (DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_270MBps == pDeviceCtx->uLinkRateInKhz)
  {
    /* Preferred link rate is 270MHz */
    uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_270MBps; 
    uInputClk         = DP_PLL_OUTPUT_RATE_1350MHZ;
    uNvidFactor       = DP_NVID_FACTOR_2_70G;
  }
  else if (DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_540MBps == pDeviceCtx->uLinkRateInKhz)
  {
    /* Preferred link rate is 540MHz */
    uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_540MBps; 
    uInputClk         = DP_PLL_OUTPUT_RATE_1350MHZ;
    uNvidFactor       = DP_NVID_FACTOR_5_40G;
  }
  else if (DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_810MBps == pDeviceCtx->uLinkRateInKhz)
  {
    /* Preferred link rate is 810MHz */
    uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_810MBps; 
    uInputClk         = DP_PLL_OUTPUT_RATE_1350MHZ;
    uNvidFactor       = DP_NVID_FACTOR_8_10G;
  }
  else
  {
    DP_LOG_MESSAGE_INFO("Display: DP_CalculateLink: DPCD link-rate=%d\n", psDPCDSinkCap->uMaxLinkRateInKhz);

    // Check receiver link rate capability in DPCD
    if (psDPCDSinkCap->uMaxLinkRateInKhz <= DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_162MBps)
    {
      DP_LOG_MESSAGE_INFO("DP_CalculateLink: 162MBps\n");
      uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_162MBps;      
      uInputClk         = DP_PLL_OUTPUT_RATE_810MHZ;
      uNvidFactor       = DP_NVID_FACTOR_1_62G;
    }
    else if (psDPCDSinkCap->uMaxLinkRateInKhz <= DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_270MBps)
    {
      DP_LOG_MESSAGE_INFO("DP_CalculateLink: 270MBps\n");
      uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_270MBps;
      uInputClk         = DP_PLL_OUTPUT_RATE_1350MHZ;
      uNvidFactor       = DP_NVID_FACTOR_2_70G;
    }
    else if (psDPCDSinkCap->uMaxLinkRateInKhz <= DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_540MBps)
    {
      DP_LOG_MESSAGE_INFO("DP_CalculateLink: 540MBps\n");
      uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_540MBps;
      uInputClk         = DP_PLL_OUTPUT_RATE_1350MHZ;
      uNvidFactor       = DP_NVID_FACTOR_5_40G;
    }
    else if (psDPCDSinkCap->uMaxLinkRateInKhz <= DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_810MBps)
    {
      DP_LOG_MESSAGE_INFO("DP_CalculateLink: 810MBps\n");
      uLinkRate         = DP_LINK_SYMBOL_RATE_KHZ_PER_LANE_810MBps;
      uInputClk         = DP_PLL_OUTPUT_RATE_1350MHZ;
      uNvidFactor       = DP_NVID_FACTOR_8_10G;
    }
    else
    {
       DP_LOG_MESSAGE_ERROR("DP_CalculateLink: LinkRtae=%d is NOT Supported\n", psDPCDSinkCap->uMaxLinkRateInKhz);
    }
  }

  uTotalWidth = psPanelInfo->uVisibleWidthInPixels    + psPanelInfo->uHLeftBorderInPixels    +
                psPanelInfo->uHRightBorderInPixels    + psPanelInfo->uHsyncBackPorchInPixels +
                psPanelInfo->uHsyncFrontPorchInPixels + psPanelInfo->uHsyncPulseInPixels;

  uTotalHeight = psPanelInfo->uVisibleHeightInPixels  + psPanelInfo->uVBottomBorderInLines  + 
                 psPanelInfo->uVTopBorderInLines      + psPanelInfo->uVsyncBackPorchInLines + 
                 psPanelInfo->uVsyncFrontPorchInLines + psPanelInfo->uVsyncPulseInLines;
 
  // Calculate the total number of pixel in Q16 (need 64bits to represent this number)
  uTempQ16 = ((uint64)(uTotalWidth * uTotalHeight)) << 16; 
  
  // Multiple by refresh rate, 
  uTempQ16 = (uint64)psPanelInfo->uRefreshRate * (uint64)uTempQ16;
  
  // Round back Q16 to a whole number
  uPclkRate = (uint32)(uTempQ16 >> 32);

  uPclkRate = psPanelInfo->uPclkFreq; /* use pclk from global table */

  DP_LOG_MESSAGE_INFO("DP_CalculateLink: h=%d bph=%d fph=%d hs=%d, v=%d vfp=%d vbp=%d vs=%d, frameRate=%x\n",
psPanelInfo->uVisibleWidthInPixels, psPanelInfo->uHsyncBackPorchInPixels, psPanelInfo->uHsyncFrontPorchInPixels, psPanelInfo->uHsyncPulseInPixels, psPanelInfo->uVisibleHeightInPixels, psPanelInfo->uVsyncBackPorchInLines, psPanelInfo->uVsyncFrontPorchInLines, psPanelInfo->uVsyncPulseInLines, psPanelInfo->uRefreshRate);

  mnd.uSrcRate = uInputClk * 1000000;
  mnd.uTgtRate = uPclkRate;
  mnd_finder(&mnd);


  // Calculate the bit rate
  uBitRate = (uPclkRate * DPHOST_GETPIXELFMT(psPanelInfo->eColorFormat).uBitsPerPixel) / 1000;

  // Calculate the desired number of lanes
  for (uLaneNumber = 1; uLaneNumber <= psDPCDSinkCap->uMaxLaneCount; uLaneNumber++)
  {
      /* Desired Lane Number has already been set */
      if ((0           != pDeviceCtx->uNumLanes) &&
          (uLaneNumber != pDeviceCtx->uNumLanes))
      {
        continue;
      }
      else if (uLaneNumber == 3)
      {
        /* Lane Number cannot be 3 */
        continue;
      }

      uLaneRate = (uint32)(uLinkRate * uLaneNumber * 8);

      if ((uLaneRate > uBitRate) && ((uLaneRate - uBitRate) < uMinDelta))
      {
        uMinDelta               = uLaneRate- uBitRate;
        uDesiredLaneNumber      = uLaneNumber;
      }
  }

  if (uMinDelta != 0x7fffffff)
  {
      pDeviceCtx->uNumLanes                  =  uDesiredLaneNumber;
      pDeviceCtx->uLinkRateInKhz             =  uLinkRate;
      pDeviceCtx->uPixelClkInKhz             =  uPclkRate / 1000;

      DP_LOG_MESSAGE_INFO("DP_CalculateLink: calculatedm lane=%d rate=%d pclk=%d\n", pDeviceCtx->uNumLanes, pDeviceCtx->uLinkRateInKhz, pDeviceCtx->uPixelClkInKhz);
      
      pPixClkInfo->uPixelClkMNDInputMhz      =  uInputClk;
      pPixClkInfo->uPixelClk_M               =  mnd.M;
      pPixClkInfo->uPixelClk_N               =  mnd.N;
      pPixClkInfo->uPixelClk_2D              =  mnd.D;
      pPixClkInfo->uPixelDivider             =  1;


      if (pDeviceCtx->bSynchronousClk)
      {
        //  Mvid/Nvid = PixelClk/LinkFreq . In sync mode, we have pixel_clk = (link * 5 * M)/ (uPreDiv * N)
        pPixClkInfo->uSWMVid                 =  mnd.M * 5;        
        pPixClkInfo->uSWNVid                 =  mnd.N * uNvidFactor;
        DP_LOG_MESSAGE_INFO("DP_CalculateLink: synchrousClock=%d mvid=%x nvid=%x\n", pDeviceCtx->bSynchronousClk, pPixClkInfo->uSWMVid, pPixClkInfo->uSWNVid );
      }
  }
  else
  {
      DP_LOG_MESSAGE_WARN("Cannot find a jitter free link speed!\n");
      eStatus = DP_STATUS_FAILED_NOT_SUPPORTED;
  }

  return eStatus;
}

/**********************************************************************************************************

FUNCTION: DP_CheckTrainingResult()
   
   This function will get the max (among all lanes) drive setting requested by the sink 

Parameters:
   eDeviceId                          -[in ] DP device ID
   psDPCDSinkStatus                   -[in ] Pointer to DPCP sink status
   eTrainingType                      -[in ] type of link training done

Return: 
   bool32                             - TRUE = training successful 

************************************************************************************************************/
static bool32  DP_CheckTrainingResult(DP_DeviceIDType              eDeviceId,
                                      DP_DPCDSinkStatusType       *psDPCDSinkStatus,
                                      DP_LinkTrainingStatusType    eTrainingType)
{  
  DP_DeviceCtxType *pDeviceCtx  = DPHOST_GET_CTX(eDeviceId);  
  bool32            bDone       = TRUE;
  uint32            i           = 0;

  if (DP_LINK_TRAINING_STATUS_CLK_RECOVERY_DONE & eTrainingType)
  {
    for (i = 0; i< pDeviceCtx->uNumLanes; ++i)
    {
      if (0 == (psDPCDSinkStatus->eLaneStatus[i] & DP_DPCD_LANE_STATUS_CR_DONE))
      {
        bDone = FALSE;
        goto exit;
      }
    }
  }

  if (DP_LINK_TRAINING_STATUS_CHANNEL_EQ_DONE & eTrainingType)
  {
    for (i = 0; i< pDeviceCtx->uNumLanes; ++i)
    {
      if (0 == (psDPCDSinkStatus->eLaneStatus[i] & DP_DPCD_LANE_STATUS_CHANNEL_EQ_DONE))
      {
        bDone = FALSE;
        goto exit;
      }
    }
  }

  if (DP_LINK_TRAINING_STATUS_SYMBOL_LOCKED & eTrainingType)
  {
    for (i = 0; i< pDeviceCtx->uNumLanes; ++i)
    {
      if (0 == (psDPCDSinkStatus->eLaneStatus[i] & DP_DPCD_LANE_STATUS_SYMBOL_LOCKED))
      {
        bDone = FALSE;
        goto exit;
      }
    }
  }

  if (DP_LINK_TRAINING_STATUS_INTERLANE_ALIGN_DONE & eTrainingType)
  {  
    if (0 == (psDPCDSinkStatus->eLinkStatusBitArray & DP_DPCD_BIT_STATUS_INTERLANE_ALIGN_DONE))
    {
      bDone = FALSE;
      goto exit;
    }
  }

exit:

  return bDone;
}

/**********************************************************************************************************

FUNCTION: DP_GetMaxSinkPeSwRequest()
   
   This function will get the max (among all lanes) drive setting requested by the sink 

Parameters:
   eDeviceId                          -[in ] DP device ID
   psDPCDSinkStatus                   -[in ] Pointer to DPCP sink status
   uVoltSwingLvl                      -[out ] max Voltage swing level requested
   uPreemphLvl                        -[out ] max Pre-emphasis level requested

Return: 
   DP_Status 

************************************************************************************************************/
static void DP_GetMaxSinkPeSwRequest(DP_DeviceIDType                  eDeviceId,
                                     DP_DPCDSinkStatusType           *psDPCDSinkStatus, 
                                     uint32                          *uVoltSwingLvl,
                                     uint32                          *uPreemphLvl)
{
  DP_DeviceCtxType  *pDeviceCtx  = DPHOST_GET_CTX(eDeviceId);
  uint32             i           = 0;
  uint32             uSwLvl      = 0;
  uint32             uPeLvl      = 0;

  /*  We do not support per-lane drive settings.
   *  If sink makes such a request, pick the highest pre-emphasis and swing of all lanes
   */

  uSwLvl  = DPCD_SPEC_MIN_VOLTAGE_SWING_LEVEL;
  for (i = 0; i < pDeviceCtx->uNumLanes; i++)
  {
    if (uSwLvl < psDPCDSinkStatus->uVoltageSwingLevel[i])
    {
      uSwLvl = psDPCDSinkStatus->uVoltageSwingLevel[i];
    }
  }

  uPeLvl  = DPCD_SPEC_MIN_PRE_EMPHASIS_LEVEL;
  for (i = 0; i< pDeviceCtx->uNumLanes; i++)
  {
    if (uPeLvl < psDPCDSinkStatus->uPreEmphasisLevel[i])
    {
      uPeLvl = psDPCDSinkStatus->uPreEmphasisLevel[i];
    }
  }

  *uVoltSwingLvl = uSwLvl;
  *uPreemphLvl   = uPeLvl;
}

/**********************************************************************************************************

FUNCTION: DP_LinkSetupPreemphSwing()
   
   This function will set up the nearest possible premphasis / voltage swing to teh requested level 

Parameters:
   eDeviceId                        -[in ] DP device ID
   uVoltSwingLvl                    -[in ] Voltage swing level to set
   uPreemphasisLvl                  -[in ] Pre-emphasis level to set

Return: 
   DP_Status 

************************************************************************************************************/
static DP_Status DP_LinkSetupPreemphSwing(DP_DeviceIDType  eDeviceId,
                                          uint32           uVoltSwingLvl,
                                          uint32           uPreemphasisLvl)
{
  DP_Status         eStatus                 = DP_STATUS_SUCCESS;
  DP_DeviceCtxType *pDeviceCtx              = DPHOST_GET_CTX(eDeviceId);
  
  HAL_DP_PeSwConfigType  sPeSwConfig;

  /* Check if request is outside the limits of DP spec*/
  if (uVoltSwingLvl > DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL)
  {
    uVoltSwingLvl = DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL;
  }
  if (uPreemphasisLvl > DPCD_SPEC_MAX_PRE_EMPHASIS_LEVEL)
  {
    uPreemphasisLvl = DPCD_SPEC_MAX_PRE_EMPHASIS_LEVEL;
  }

  sPeSwConfig.uLaneSwingLevel       = uVoltSwingLvl;
  sPeSwConfig.uPreemphasisLevel     = uPreemphasisLvl;
  sPeSwConfig.bPreemphNotSupported  = FALSE;
  sPeSwConfig.bSwingNotsupported    = FALSE;

  /* Set the requested or closest possible settings */
  
  while (HAL_MDSS_STATUS_SUCCESS != HAL_DP_SetupPHYPeSwLevel(&sPeSwConfig))
  {    
    if ((DPCD_SPEC_MIN_PRE_EMPHASIS_LEVEL  == sPeSwConfig.uPreemphasisLevel) &&
        (DPCD_SPEC_MIN_VOLTAGE_SWING_LEVEL == sPeSwConfig.uLaneSwingLevel))
    {
      // We should never hit this in practice
      DP_LOG_MESSAGE_ERROR("Failed to set lane drv settings\n");
      eStatus = DP_STATUS_FAIL;
      break;
    }
    
    if (sPeSwConfig.bPreemphNotSupported)
    {
     /* pre-emphasis level exceeds our capability - choose the next highest level we support */
      --(sPeSwConfig.uPreemphasisLevel);
    }
    else if (sPeSwConfig.bSwingNotsupported)
    {      
      /* voltage swing level exceeds our capability for this pre-emphasis 
       *     - choose the highest swing we can support at this pre-emphasis level 
       */
      --(sPeSwConfig.uLaneSwingLevel);
    }
    else
    {
      // We should never hit this in practice
      DP_LOG_MESSAGE_ERROR("Hardware failed to set lane drv settings\n");
      eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
      break;
    }
  }

  if (DP_STATUS_SUCCESS == eStatus)
  {
    pDeviceCtx->uVoltageSwingLevel = sPeSwConfig.uLaneSwingLevel;
    pDeviceCtx->uPreEmphasisLevel  = sPeSwConfig.uPreemphasisLevel;
  }
  
  return eStatus;
}

/**********************************************************************************************************************

FUNCTION: DP_LinkTraining_ClockRecovery()
   
   This function will do link training with pattern TPS1 

Parameters:
   eDeviceId                        -[in ] DP device ID
   uFlags                           -[in ] Reserved

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_LinkTraining_ClockRecovery(DP_DeviceIDType    eDeviceId,
                                         DP_FlagsType       uFlags)
{
  DP_Status                       eStatus            = DP_STATUS_SUCCESS;
  DP_DeviceCtxType               *pDeviceCtx         = DPHOST_GET_CTX(eDeviceId); 
  uint32                          uLoopNumber        = 0;
  uint32                          uRetries           = 0;
  uint32                          uVoltSwingLvl      = 0;
  uint32                          uPreemphLvl        = 0;
  uint32                          uPrevVoltSwingLvl  = 0;
  uint32                          uPrevPreemphLvl    = 0;
  DP_DPCDSinkStatusType           sSinkDPCDStatus; 
 
  if(DP_STATUS_SUCCESS == eStatus)
  {
    // Start sending Training Pattern 1
    HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN1);

    // Inform sink about the Training Pattern we are using
    eStatus  = DP_WriteSinkDPCDTrainingPattern(eDeviceId, DP_DPCD_TRAININGPATTERNSET_PATTERN1);      

    // Start with minimum pre-emphasis & voltage swing  
    DP_LinkSetupPreemphSwing(eDeviceId, DPCD_SPEC_MIN_VOLTAGE_SWING_LEVEL, DPCD_SPEC_MIN_PRE_EMPHASIS_LEVEL);
    eStatus = DP_WriteSinkDPCDLevels(eDeviceId, pDeviceCtx->uVoltageSwingLevel, pDeviceCtx->uPreEmphasisLevel);

  }

  if(DP_STATUS_SUCCESS == eStatus)
  {
    while (uRetries < MAX_LINK_TRAINING_LOOP)
    {
      // Wait for a period defined by the DPCD 
      DP_OSAL_SleepUs(pDeviceCtx->sSinkDPCDCap.uTrainingIntervalInUs);

      eStatus = DP_ReadSinkDPCDStatus(eDeviceId, &sSinkDPCDStatus);
      if (DP_STATUS_SUCCESS != eStatus)
      {
        DP_LOG_MESSAGE_ERROR("DP_LinkTraining_ClockRecovery: Failed to read sink status from DPCD\n");
        break;
      }
            
      if (TRUE == DP_CheckTrainingResult(eDeviceId, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_CLK_RECOVERY_DONE))
      {       
        // Link Training successful        
        break;
      }

      // Get Sink requested pre-emphasis and voltage swing
      DP_GetMaxSinkPeSwRequest(eDeviceId, &sSinkDPCDStatus, &uVoltSwingLvl, &uPreemphLvl);

      if ((uPrevVoltSwingLvl == uVoltSwingLvl) &&
          (uPrevPreemphLvl   == uPreemphLvl))
      {
        // Requested settings already set - try again with same setting 
        ++uRetries;

        // If Monitor does not request the highest swing level/pre-emphasis level, try to boost them for link training.
        if ((MAX_LINK_TRAINING_LOOP >= uRetries) &&
           ((uVoltSwingLvl < DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL) ||
            (uPreemphLvl   < DPCD_SPEC_MAX_PRE_EMPHASIS_LEVEL)))
        {
          uVoltSwingLvl++;
          uPreemphLvl++;

          // Apply newly requested settings
          eStatus = DP_LinkSetupPreemphSwing(eDeviceId, uVoltSwingLvl, uPreemphLvl);
          if (DP_STATUS_SUCCESS == eStatus)
          {
            DP_WriteSinkDPCDLevels(eDeviceId, pDeviceCtx->uVoltageSwingLevel, pDeviceCtx->uPreEmphasisLevel);
          }
          uRetries = 0;
        }
      }
      else
      {
        // Apply newly requested settings
        eStatus = DP_LinkSetupPreemphSwing(eDeviceId, uVoltSwingLvl, uPreemphLvl);
        if (DP_STATUS_SUCCESS == eStatus)
        {
          eStatus = DP_WriteSinkDPCDLevels(eDeviceId, pDeviceCtx->uVoltageSwingLevel, pDeviceCtx->uPreEmphasisLevel);
        } 
        
        if(DP_STATUS_SUCCESS != eStatus)
        {
          break;
        }

        // Reset loop count
        uRetries = 0;
        uPrevVoltSwingLvl = uVoltSwingLvl;
        uPrevPreemphLvl   = uPreemphLvl;
      }

      // Defensive - to avoid infinite loop      
      ++ uLoopNumber;
      if (uLoopNumber >= 100)
      {
        DP_LOG_MESSAGE_ERROR("Link Training (clock recovery) taking too long\n");
        eStatus = DP_STATUS_FAILED_TIMEOUT;
        break;
      }
    }

    if (uRetries >= MAX_LINK_TRAINING_LOOP)
    {      
      DP_LOG_MESSAGE_ERROR("Link Training (clock recovery) failed (5 attempts)\n");
      eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
    }    
  }
  
  if(DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("Link training 1 failed with eStatus=0x%x\n",eStatus);
  }

  return eStatus;
}


/**********************************************************************************************************************

FUNCTION: DP_LinkTraing_Pattern23()
   
   This function will do link training with pattern TPS2 or TPS3 

Parameters:
   eDeviceId                        -[in ] DP device ID
   uFlags                           -[in ] Reserved

Return: 
   DP_Status 

**********************************************************************************************************************/
DP_Status  DP_LinkTraining_ChannelEqualization(DP_DeviceIDType       eDeviceId,
                                               DP_FlagsType          uFlags)
{
  DP_Status                eStatus            = DP_STATUS_SUCCESS;  
  DP_DeviceCtxType        *pDeviceCtx         = DPHOST_GET_CTX(eDeviceId);  
  uint32                   uLoopNumber        = 0;
  uint32                   uRetries           = 0;
  uint32                   uVoltSwingLvl      = 0;
  uint32                   uPreemphLvl        = 0;
  uint32                   uPrevVoltSwingLvl  = 0;
  uint32                   uPrevPreemphLvl    = 0;
  DP_DPCDSinkStatusType    sSinkDPCDStatus;

  if (pDeviceCtx->sSinkDPCDCap.eDPCDCap & DP_DPCD_CAP_SUPPORT_TRAININGPATTERN3)
  {
    // Start sending Training Pattern 3
    HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN3);

    // Inform sink about the Training Pattern we are using
    eStatus = DP_WriteSinkDPCDTrainingPattern(eDeviceId, DP_DPCD_TRAININGPATTERNSET_PATTERN3);
  }
  else
  {

    // Start sending Training Pattern 2
    HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN2);

    // Inform sink about the Training Pattern we are using
    eStatus = DP_WriteSinkDPCDTrainingPattern(eDeviceId, DP_DPCD_TRAININGPATTERNSET_PATTERN2);
  }

  DP_LinkSetupPreemphSwing(eDeviceId, pDeviceCtx->uVoltageSwingLevel, pDeviceCtx->uPreEmphasisLevel);
  eStatus = DP_WriteSinkDPCDLevels(eDeviceId, pDeviceCtx->uVoltageSwingLevel, pDeviceCtx->uPreEmphasisLevel);


  if (DP_STATUS_SUCCESS == eStatus)
  {
    while (uRetries < MAX_LINK_TRAINING_LOOP)
    {
      /* Wait for a period defined by the DPCD 0x0E. */
      DP_OSAL_SleepUs(pDeviceCtx->sSinkDPCDCap.uTrainingIntervalInUs);

      /* Read the DPCP Link Status */
      eStatus  = DP_ReadSinkDPCDStatus(eDeviceId, &sSinkDPCDStatus);
      if(DP_STATUS_SUCCESS != eStatus)
      {    
        DP_LOG_MESSAGE_ERROR("DP_LinkTraining_ChannelEqualization: Failed to read sink status from DPCD\n");
        break;
      }
      
      if(FALSE == DP_CheckTrainingResult(eDeviceId, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_CLK_RECOVERY_DONE))
      {
        DP_LOG_MESSAGE_ERROR("DP_LinkTraining_ChannelEqualization(): Clock Recovery Failed\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
        break;
      }

      if((TRUE == DP_CheckTrainingResult(eDeviceId, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_CHANNEL_EQ_DONE)) &&
         (TRUE == DP_CheckTrainingResult(eDeviceId, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_SYMBOL_LOCKED))   &&
         (TRUE == DP_CheckTrainingResult(eDeviceId, &sSinkDPCDStatus, DP_LINK_TRAINING_STATUS_INTERLANE_ALIGN_DONE)))
      {
        /* Link Training 2/3 done successfully */
        DP_LOG_MESSAGE_INFO("Link training: Channel EQ , Symbol Lock and Inter-lane align Done\n ");
        break;
      }

      // Get Sink requested pre-emphasis and voltage swing
      DP_GetMaxSinkPeSwRequest(eDeviceId, &sSinkDPCDStatus, &uVoltSwingLvl, &uPreemphLvl);

      
      if ((uPrevVoltSwingLvl == uVoltSwingLvl) &&
          (uPrevPreemphLvl   == uPreemphLvl))
      {
        // Requested settings already set - try again with same setting 
        ++uRetries;
        if ((MAX_LINK_TRAINING_LOOP >= uRetries) &&
           ((uVoltSwingLvl < DPCD_SPEC_MAX_VOLTAGE_SWING_LEVEL) ||
            (uPreemphLvl   < DPCD_SPEC_MAX_PRE_EMPHASIS_LEVEL)))
        {
          uVoltSwingLvl++;
          uPreemphLvl++;

          // Apply newly requested settings
          eStatus = DP_LinkSetupPreemphSwing(eDeviceId, uVoltSwingLvl, uPreemphLvl);
          if (DP_STATUS_SUCCESS == eStatus)
          {
            DP_WriteSinkDPCDLevels(eDeviceId, pDeviceCtx->uVoltageSwingLevel, pDeviceCtx->uPreEmphasisLevel);
          }
          uRetries = 0;
        }
      }
      else
      {        
        // Apply newly requested settings
        eStatus = DP_LinkSetupPreemphSwing(eDeviceId, uVoltSwingLvl, uPreemphLvl);
        if (DP_STATUS_SUCCESS == eStatus)
        {
          eStatus = DP_WriteSinkDPCDLevels(eDeviceId, 
                                           pDeviceCtx->uVoltageSwingLevel, 
                                           pDeviceCtx->uPreEmphasisLevel);
        } 
        
        if(DP_STATUS_SUCCESS != eStatus)
        {
          break;
        }

        // Reset loop count
        uRetries = 0;
        uPrevVoltSwingLvl = uVoltSwingLvl;
        uPrevPreemphLvl   = uPreemphLvl;
      }
      
      if (uRetries >= MAX_LINK_TRAINING_LOOP)
      {      
        DP_LOG_MESSAGE_ERROR("Link Training (channel Eq) failed (5 attempts)\n");
        eStatus = DP_STATUS_FAILED_RESOURCE_FAILED;
      }

      // Defensive - to avoid infinite loop      
      ++ uLoopNumber;
      if (uLoopNumber >= 100)
      {
        DP_LOG_MESSAGE_ERROR("Link Training 2/3 taking too long\n");
        eStatus = DP_STATUS_FAILED_TIMEOUT;
        break;
      }
    }
  }

  if(DP_STATUS_SUCCESS != eStatus)
  {
    DP_LOG_MESSAGE_ERROR("Link training 2 failed with eStatus=0x%x\n",eStatus);
  }

  // Stop training
  HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_NONE);
  DP_WriteSinkDPCDTrainingPattern(eDeviceId, DP_DPCD_TRAININGPATTERNSET_NONE);

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: DP_LinkTraining()
*/
/*!
* \DESCRIPTION
*     This function will read from AUX/I2c channel
*
* \param [in]   eDeviceId               - DP device ID
* \param [in]   eLinkTrainingType       - The type of Link training
* \param [in]   uFlags                  - Reserved
*
* \retval DP_Status
*
**********************************************************************************************/
DP_Status DP_LinkTraining(DP_DeviceIDType           eDeviceId,
                          DP_LinkTrainingType       eLinkTrainingType,
                          DP_FlagsType              uFlags)
{
  DP_Status             eStatus     = DP_STATUS_SUCCESS;

  if (DP_LINK_TRAINING_TYPE_FULL_TRAINING == eLinkTrainingType)
  {
    
    // TPS1
    if (eStatus == DP_STATUS_SUCCESS)
    {
      eStatus = DP_LinkTraining_ClockRecovery(eDeviceId, uFlags);
    }
    
    // TPS2 or TPS3
    if (eStatus == DP_STATUS_SUCCESS)
    {
      eStatus = DP_LinkTraining_ChannelEqualization(eDeviceId, uFlags);      
    }
  }
  else if (DP_LINK_TRAINING_TYPE_FAST_TRAINING == eLinkTrainingType)
  {
    /* In the fast training approach, we send each training pattern for a certain amount of time.
     * We do not do any AUX transactions in this training mode
     */
     
    // TPS 1
    HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN1);
    DP_OSAL_SleepMs(1);

    // TPS2
    HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_SEND_PATTERN2);
    DP_OSAL_SleepMs(1);

    // Stop training
    HAL_DP_SetLinkState(HAL_DP_LINKCOMMAND_TYPE_NONE);    
  }
  else if (DP_LINK_TRAINING_TYPE_NO_TRAINING == eLinkTrainingType)
  {
    // Nothing to do
  }
  else
  {
    DP_LOG_MESSAGE_ERROR("DP_LinkTraining(): invalid training type = %d\n", (uint32)eLinkTrainingType);
    eStatus = DP_STATUS_FAILED_INVALID_INPUT_PARAMETER;
  }

  return eStatus;
}

/*********************************************************************************************
*
** FUNCTION: DP_CalculateMisc0()
*/
/*!
* \DESCRIPTION
*     This function will calculate the MISCO value according to DP Spec
*
* \param [in]   eColorFormat             - Sink color format
*
* \retval  uint8 Misc0 value
*
**********************************************************************************************/
uint8  DP_CalculateMisc0(DP_PixelFormatType eColorFormat )
{
  uint8 uMisc0;

  if (DP_PIXEL_FORMAT_RGB_666_18BPP == eColorFormat )
  {
    uMisc0 = 0;
  }
  else if (DP_PIXEL_FORMAT_RGB_101010_30BPP == eColorFormat )
  {
    uMisc0 = DP_SPEC_MSA_MISCO_COLOR_10BPC_BMSK;
  }
  else
  {
    uMisc0 = DP_SPEC_MSA_MISCO_COLOR_8BPC_BMSK;
  }
  return uMisc0;
}

#ifdef __cplusplus
}
#endif

