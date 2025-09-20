/**

@file   icb_sdi.c

@brief  ICB support for SDI. Save/restore MEMNOC address translators
        
===========================================================================

                             Edit History


when         who     what, where, why
----------   ---     --------------------------------------------------------
2018/07/23   sds     Update for Poipu v2 4-ch variant
2017/10/19   sds     Branch for Poipu
2017/09/29   sds     Clean up for Hana
2017/05/09   sds     Updated for v2
2016/11/18   sds     Created 

===========================================================================
       Copyright (c) 2016-2018 QUALCOMM Technologies Incorporated.
                       All Rights Reserved.
                       QUALCOMM Proprietary
===========================================================================
*/

/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../../../../Include/HALhwio.h"
#include "../../Include/msmhwiobase.h"
#include "../../../../Library/ICBLib/icbcfg_hwio.h"
#include "icbcfg_hwio_target.h"
#include "ChipInfo.h"
#include "string.h"

/* -----------------------------------------------------------------------
** Definitions
** ----------------------------------------------------------------------- */
#define MAX_CHANNELS 8
#define SEGMENTS 6
#define DDR_CH4_OR_CH8_SEL 0x2

/* -----------------------------------------------------------------------
** Types
** ----------------------------------------------------------------------- */
typedef struct
{
  uint32_t base_low;
  uint32_t base_high;
  uint32_t offset_low;
  uint32_t offset_high;
} icb_segment_t;

typedef struct
{
  uint32_t      enable_low;
  uint32_t      error_low;
  icb_segment_t segs[SEGMENTS];
} icb_sdi_trans_t;

/* -----------------------------------------------------------------------
** Data
** ----------------------------------------------------------------------- */
static bool valid = false;
static uint32_t channels = 8;
static icb_sdi_trans_t trans[MAX_CHANNELS];
static uint32_t trans_base_regs[MAX_CHANNELS] =
{
  HWIO_ADDR(LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  HWIO_ADDR(LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  HWIO_ADDR(LLCC2_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  HWIO_ADDR(LLCC3_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  HWIO_ADDR(LLCC4_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  HWIO_ADDR(LLCC5_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  HWIO_ADDR(LLCC6_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  HWIO_ADDR(LLCC7_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
};
static uint32_t *trans_bases = trans_base_regs;

/* -----------------------------------------------------------------------
** Functions
** ----------------------------------------------------------------------- */

//*============================================================================*/
/**
@brief 
      Save addr translators
*/ 
/*============================================================================*/
void icb_sdi_save( void )
{
  uint32_t ch, seg;

  /* Check the hardware version and update some variables. */
  if( CHIPINFO_FAMILY_SDM1000 == ChipInfo_GetChipFamily() &&
      CHIPINFO_VERSION(2,0) <= ChipInfo_GetChipVersion() &&
      DDR_CH4_OR_CH8_SEL == HWIO_INM(TCSR_TIMEOUT_INTR_STATUS, DDR_CH4_OR_CH8_SEL) )
  {
    /* Poipu v2, 4-channel variant. */
    channels = 4;
    trans_bases = &trans_base_regs[4];
  }

  /* Save segment decode registers. */
  memset(&trans, 0, sizeof(trans));
  for( ch = 0; ch < channels; ch ++ )
  {
    trans[ch].enable_low = HWIO_INXF(trans_bases[ch],LLCC_BEAC_ADDR_TRANSLATOR_CFG,ENABLE);
    trans[ch].error_low  = HWIO_INXF(trans_bases[ch],LLCC_BEAC_ADDR_TRANSLATOR_CFG,ERROR);
    for( seg = 0; seg < SEGMENTS; seg ++ )
    {
      if( seg != 0 )
      {
        trans[ch].segs[seg].base_low  = HWIO_INXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG2,seg);
        trans[ch].segs[seg].base_high = HWIO_INXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG3,seg);
      }
      trans[ch].segs[seg].offset_low  = HWIO_INXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG0,seg);
      trans[ch].segs[seg].offset_high = HWIO_INXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG1,seg);
    }
  }

  valid = true;
}

//*============================================================================*/
/**
@brief 
      Restore addr translators

@dependencies
      icb_sdi_save() must have been called first.
 
@return    
      true if successful
      false if no valid data to restore
*/ 
/*============================================================================*/
bool icb_sdi_restore( void )
{
  uint32_t ch, seg;

  if( false == valid )
  {
    return false;
  }

  /* First, disable all address translators. */
  for( ch = 0; ch < channels; ch ++ )
  {
    HWIO_OUTXF(trans_bases[ch],LLCC_BEAC_ADDR_TRANSLATOR_CFG,ENABLE,0);
  }

  /* Write translator configuration. */
  for( ch = 0; ch < channels; ch ++ )
  {
    HWIO_OUTXF(trans_bases[ch],LLCC_BEAC_ADDR_TRANSLATOR_CFG,ERROR,trans[ch].error_low);
    for( seg = 0; seg < SEGMENTS; seg ++ )
    {
      if( seg != 0 )
      {
        HWIO_OUTXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG2,seg,trans[ch].segs[seg].base_low);
        HWIO_OUTXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG3,seg,trans[ch].segs[seg].base_high);
      }
      HWIO_OUTXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG0,seg,trans[ch].segs[seg].offset_low);
      HWIO_OUTXI(trans_bases[ch],LLCC_BEAC_ADDR_REGIONn_CFG1,seg,trans[ch].segs[seg].offset_high);
    }
  }

  /* Finally, set enable bits according to saved state. */
  for( ch = 0; ch < channels; ch ++ )
  {
    HWIO_OUTXF(trans_bases[ch],LLCC_BEAC_ADDR_TRANSLATOR_CFG,ENABLE,trans[ch].enable_low);
  }

  return true;
}

