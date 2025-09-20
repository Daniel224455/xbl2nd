/** @file
  Static wrapper functions to augment the existing public interface.
   
  Copyright (c) 2017 - 2019 Qualcomm Technologies Inc. All rights reserved.
  
**/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 04/03/18   yg      Add additional API's
 06/14/17   yg      Initial version

=============================================================================*/
#ifndef __ST_WRAPPERS_H__
#define __ST_WRAPPERS_H__

/* API's that have deviated from LK impl and have wrappers to cover */

#define optional

#include "kernel/thread.h"
#include "kernel/mutex.h"
#include "kernel/timer.h"
#include "kernel/semaphore.h"
#include "kernel/event.h"
#include "kernel/spinlock.h"

mutex_t* init_mutex(optional uint64_t id);
bool mutex_held(mutex_t *m);

event_t* init_event(optional uint64_t id, bool initial, uint flags);

semaphore_t* init_sem(optional uint64_t id, unsigned int);

timer_t* initialize_timer(void);

wait_queue_t* init_wait_queue(void);

int thread_get_curr_cpu(thread_t* t);
struct thread *thread_get_current(void);
int thread_get_pinned_cpu(thread_t* t);
void gthread_set_curr_cpu(thread_t* t, int cpu);
void gthread_set_pinned_cpu(thread_t* t, int cpu);

/* TODO: Search appropriate common header for these */
struct sched_stats* get_cpu_sched_stats (uint cpu, struct sched_stats* csp);
struct thread_stats* get_thread_exec_stats (thread_t *thr, struct thread_stats* tsp);

uint32_t mpcore_get_curr_cpu(void);
uint32_t mpcore_get_max_cpu_count (void);
uint32_t mpcore_get_avail_cpu_count (void);
uint32_t mpcore_init_deferred_cores (uint32_t cpu_mask);
uint32_t mpcore_is_cpu_active (uint32_t cpu);
uint32_t mpcore_is_cpu_idle (uint32_t cpu);
void mpcore_set_cpu_idle (uint32_t cpu);
void mpcore_set_cpu_busy (uint32_t cpu);
uint32_t mpcore_get_idle_mask (void);
uint32_t mpcore_get_active_mask (void);
void mpcore_set_cpu_realtime (uint32_t cpu);
void mpcore_set_cpu_non_realtime (uint32_t cpu);
uint32_t mpcore_get_realtime_mask (void);
void mpcore_shutdown_sched (void);
void mpcore_shutdown_cpu (uint32_t cpu_mask);
void mpcore_powerup_cpu (uint32_t cpu_mask);

uint64_t current_time_hires(void);
uint32_t get_sched_lib_version (void);

#endif  /* __ST_WRAPPERS_H__  */

