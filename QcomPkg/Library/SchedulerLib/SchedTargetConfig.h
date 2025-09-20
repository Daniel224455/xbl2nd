/** @file
 *   Target configuration for scheduler lib
 * 
 *   Copyright (c) 2018 Qualcomm Technologies, Inc. All rights reserved.
 *
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 02/22/18   yg      Initial version

=============================================================================*/
#ifndef __TARGET_CONFIG_H__
#define __TARGET_CONFIG_H__

typedef struct 
{
    uint32_t    version;

    uint32_t    mpidr_to_cpu_num_bits;
    uint32_t    mpidr_to_cpu_num_shift;
    uint32_t    mpidr_to_clust_num_shift;

    // Need 2 consecutive SGI interrupts accessible from NS side
    uint32_t    ipi_sgi_base_num;
    uint32_t    arch_timer_interrupt_num;

    uint64_t    gic_distributor_offset;
    uint64_t    gic_redistributor_offset;
    uint64_t    gic_redistributor_stride;

} sched_target_config;

int update_sched_target_config (sched_target_config* target_config);

#endif  /* __TARGET_CONFIG_H__  */

