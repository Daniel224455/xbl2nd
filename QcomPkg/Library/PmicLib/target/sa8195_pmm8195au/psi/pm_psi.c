/*===========================================================================
GENERAL DESCRIPTION
  This file is just to give an entry point to the pmic elf image

 &copy; Copyright 2019 Qualcomm Technologies Inc, All Rights Reserved
 Qualcomm Confidential and Proprietary

============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



when         who     what, where, why
----------   ---     ----------------------------------------------------------
04/20/18     pxm     Created for psii elf image creation
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/ 

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "com_dtypes.h"
#include "pm_config_sbl.h"
/*===========================================================================

                           DEFINITIONS

===========================================================================*/
extern pm_sbl_config_info_type   pm_sbl_config_info[];

extern pm_sbl_seq_type           pm_sbl_seq[];
extern pm_pbs_ram_data_type      pm_pbs_seq[];
extern pm_sbl_cond_seq_type      pm_sbl_cond_seq[];
extern uint8                     pm_sdam_seq[];
pm_sbl_pdm_data_type    pm_psi_data_hdr   = {pm_sbl_seq, pm_pbs_seq, pm_sbl_cond_seq, pm_sdam_seq};


pm_sbl_pdm_config_type pm_psi_header __attribute__((section("pm_psi_info"))) =
                                     { 
                                       pm_sbl_config_info,
                                       &pm_psi_data_hdr
                                     };
