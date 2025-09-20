/*=============================================================================

@file   axicfgint.c

@brief  Implements ddiaxicfgint.h

when         who     what, where, why
----------   ---     --------------------------------------------------------
2018/04/02   sds     Support version differences
2018/03/22   sds     Active set TCS optimization
2018/02/26   sds     Target disable support
2017/08/02   sds     Fix query for vector/latency nodes
2017/06/26   sds     Add deinit
2017/04/07   sds     Workaround for QCTDD03989679
2017/03/16   sds     Add support for vector BCMs
2017/03/08   sds     Add RAW handling for single shot BCM requests
2017/02/24   sds     Add single shot BCM requests
2016/11/08   sds     Add BCM query functions
2016/10/19   sds     RPMh explicitly managed mode
2016/10/06   sds     BCM init state restructure
2016/09/13   sds     Update for new NoC QoS
2016/02/22   sds     Branch for Hoya restructure.
2011/12/05   dj      Created file

===========================================================================
            Copyright (c) 2011-2018 QUALCOMM Technologies, Incorporated.
                           All Rights Reserved.
                        QUALCOMM Proprietary/GTDR
===========================================================================
*/

/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */
#include "axicfgint.h"
#include "axicfgos.h"
#include "HALnoc.h"
#include "npa_resource.h"
#include "stringl.h"
#include "rpmh_client.h"
#include "cmd_db.h"
#include "cmd_db_bcm.h"
#include "HALhwio.h"
#include "BCMhwio.h"
#include "CorePool.h"
#include "busywait.h"

/* -----------------------------------------------------------------------
** Definitions
** ----------------------------------------------------------------------- */
#define CEILING_DIV(dividend, divisor) (((dividend)+(divisor)-1)/(divisor))
#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif /* MAX */

/* RPM message id update macro
 * Ignore new message ids if zero.
 */
#define RPM_MSG_ID_UPDATE(msg_id, new_msg_id) (msg_id) = (new_msg_id)? (new_msg_id): (msg_id)

/* -----------------------------------------------------------------------
** Definitions
** ----------------------------------------------------------------------- */
#define BCM_VOTETABLE_COMMIT_BMSK                               0x40000000
#define BCM_VOTETABLE_COMMIT_SHFT                                     0x1e
#define BCM_VOTETABLE_VOTE_VALID_BMSK                           0x20000000
#define BCM_VOTETABLE_VOTE_VALID_SHFT                                 0x1d
#define BCM_VOTETABLE_VOTE_X_BMSK                                0xfffc000
#define BCM_VOTETABLE_VOTE_X_SHFT                                      0xe
#define BCM_VOTETABLE_VOTE_X_MAX                                    0x3fff
#define BCM_VOTETABLE_VOTE_Y_BMSK                                   0x3fff
#define BCM_VOTETABLE_VOTE_Y_SHFT                                      0x0
#define BCM_VOTETABLE_VOTE_Y_MAX                                    0x3fff

#define BCM_HW_VERSION_1_0                                      0x00010000
#define BCM_HW_VERSION_1_1                                      0x00010100
#define BCM_HW_VERSION_1_5                                      0x00010500

#define DEFER_POOL_INITIAL_SIZE                                          5

#define AXICFGINT_MAX_WAIT_US                                         1000
#define AXICFGINT_WAIT_SIZE                                              5

/* -----------------------------------------------------------------------
** Types
** ----------------------------------------------------------------------- */
typedef enum
{
  AXICFGINT_STATE_UNINITIALIZED = 0, /* Initialization has not be attempted. */
  AXICFGINT_STATE_INITIALIZING,      /* Initialization in progress. */
  AXICFGINT_STATE_INITIALIZED,       /* Initialization completed. */
} AxiCfgInt_StateType;

typedef struct AxiCfgInt_DeferType AxiCfgInt_DeferType;
typedef struct AxiCfgInt_DeferType
{
  AxiCfgInt_DeferType *next;
  icb_master_def_type *master;
  icb_slave_def_type * slave;
} AxiCfgInt_DeferType;

/* -----------------------------------------------------------------------
** Data
** ----------------------------------------------------------------------- */
static AxiCfgInt_StateType state = AXICFGINT_STATE_UNINITIALIZED;
static icb_info_type *icb_info = NULL;

static icb_rpmh_request_type *active_queue = NULL; /**< active/wake queue */
static icb_rpmh_request_type *sleep_queue = NULL;

static bool is_defer_pool_initialized = false;
static CorePoolType defer_pool;
static AxiCfgInt_DeferType *defer_queue = NULL;

/* -----------------------------------------------------------------------
** Helper function predeclarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Private functions
** ----------------------------------------------------------------------- */
/**
  @brief  Used to update local qos settings.

  @pre The node *must* be local. This function will not check.

  @param[in]  master    A pointer to the master definition structure.

 */
static icb_qos_policy_type *_AxiCfgInt_QoSGetPolicy
(
  icb_master_def_type *master
)
{
  icb_qos_policy_type *qos_policy;

  /* If we have an overide policy, use that. */
  if( master->qos_policy_override )
  {
    qos_policy = master->qos_policy_override;
  }
  /* Otherwise, use the default policy, if we have one. */
  else if( master->qos_policy )
  {
    qos_policy = master->qos_policy;
  }
  /* No set policy. */
  else
  {
    qos_policy = NULL;
  }

  return qos_policy;
}

/**
  @brief  Used to update local qos settings.

  @pre The node *must* be local. This function will not check.

  @param[in]  master    A pointer to the master definition structure.

 */
static void _AxiCfgInt_QoSSetLocal
(
  icb_master_def_type *master
)
{
  icb_qos_policy_type *qos_policy;

  /* If we have no QoS ports, bail out. */
  if( NULL == master->qos_port_ids )
  {
    return;
  }

  /* We're about to modify hardware, acquire our lock. */
  AxiCfgOs_MutexLock(); 

  /* Get the current policy. */
  qos_policy = _AxiCfgInt_QoSGetPolicy(master);
  if( NULL == qos_policy )
  {
    /* Unlock and return if we have no policy to apply. */
    AxiCfgOs_MutexUnlock();
    return;
  }

  switch( master->bus->type )
  {
    /* If this is a NOC master, handle it that way. */
    case ICB_BUS_NOC:
    {
      uint32_t port;
      icb_noc_master_policy_type *qos = qos_policy->noc;

      /* Make sure we have a valid policy */
      if( NULL == qos )
      {
        break;
      }

      /* If this setting has been previously applied, bail. */
      if( qos->is_applied )
      {
        break;
      }

      /* In the case of multiple ports per master, program them both. */
      for( port = 0; port < master->num_qos_ports; port++ )
      {
        HAL_noc_RegulatorModeType regulator_mode = {0,0};

        HAL_noc_SetDefaultPriority( master->bus->info, 
                                    master->qos_port_ids[port],
                                    qos->default_priority );

        /* Disable the limiter first.
         * There's no guarantee that this can be atomically configured. */
        HAL_noc_EnableLimiter( master->bus->info,
                               master->qos_port_ids[port],
                               false );

        HAL_noc_SetLimiter( master->bus->info,
                            master->qos_port_ids[port],
                            &qos->limiter );

        HAL_noc_EnableLimiter( master->bus->info,
                               master->qos_port_ids[port],
                               qos->limiter_enabled );

        /* Disable the regulator first.
         * There's no guarantee that this can be atomically configured. */
        HAL_noc_SetRegulatorMode( master->bus->info,
                                  master->qos_port_ids[port],
                                  &regulator_mode );

        HAL_noc_SetRegulator( master->bus->info,
                              master->qos_port_ids[port],
                              &qos->regulator );

        HAL_noc_SetRegulatorMode( master->bus->info,
                                  master->qos_port_ids[port],
                                  &qos->regulator_mode );

        HAL_noc_EnableUrgencyForwarding( master->bus->info,
                                         master->qos_port_ids[port],
                                         qos->urgency_forwarding_enabled );
      }

      qos->is_applied = true;
      break;
    }
    case ICB_BUS_VIRT:
      /* No hardware backs this type. */
      break;
    default:
      /* Shouldn't get here, we only have one master type that touches hardware. */
      break;
  }

  /* Done, release our lock. */
  AxiCfgOs_MutexUnlock();
}

/**
  @brief  Used to set QOS master defaults
  @remark Only the default topology will ever set QoS.
 */
static void _AxiCfgInt_SetMasterDefaults( void )
{
  uint32_t i;
  for( i = 0; i < icb_info->topologies[0]->num_masters; i++ )
  {
    icb_master_def_type *master = icb_info->topologies[0]->masters[i];

    /* If the master is locally owned and not deferred, configure QoS. */
    if( master->is_locally_owned &&
        master->qos_policy &&
        !master->qos_policy->is_deferred )
    {
      _AxiCfgInt_QoSSetLocal(master);
    }
  }
}

/**
  @brief  Enable all config clocks for AxiCfgInt_Reset.
  @remark Only the default topology will ever set QoS.
 */
void _AxiCfgInt_EnableAllConfigClocks( void )
{
  uint32_t bus_idx, pwr_idx, clk_idx;
  icb_bus_def_type *bus;

  /* Enable all clocks we need */
  for( bus_idx = 0;
       bus_idx < icb_info->topologies[0]->num_buses;
       bus_idx++ )
  {
    bus = icb_info->topologies[0]->buses[bus_idx];
    for( pwr_idx = 0;
         pwr_idx < bus->num_pwr_domains;
         pwr_idx++ )
    {
      AxiCfgOs_EnablePowerDomain( bus->pwr_domain_ids[pwr_idx] );
    }
    for( clk_idx = 0;
         clk_idx < bus->num_en_cfg_clks;
         clk_idx++ )
    {
      AxiCfgOs_EnableClock( bus->en_cfg_clk_ids[clk_idx] );
    }
  }
}

/**
  @brief  Disable all config clocks after AxiCfgInt_Reset.
  @remark Only the default topology will ever set QoS.
 */
void _AxiCfgInt_DisableAllConfigClocks( void )
{
  uint32_t bus_idx, clk_idx, pwr_idx;
  icb_bus_def_type *bus;

  /* Disable any clocks we enabled before.
   * Note: We do NOT revoke our BCM requests from EnableAllConfigClocks().
   * UL will overwrite to the correct values when we enter RUN mode. */
  for( bus_idx = 0;
       bus_idx < icb_info->topologies[0]->num_buses;
       bus_idx++ )
  {
    bus = icb_info->topologies[0]->buses[bus_idx];
    for( clk_idx = 0;
         clk_idx < bus->num_en_cfg_clks;
         clk_idx++ )
    {
      AxiCfgOs_DisableClock( bus->en_cfg_clk_ids[clk_idx] );
    }
    for( pwr_idx = 0;
         pwr_idx < bus->num_pwr_domains;
         pwr_idx++ )
    {
      AxiCfgOs_DisablePowerDomain( bus->pwr_domain_ids[pwr_idx] );
    }
  }
}

void _AxiCfgInt_ConvertQosPolicy
(
  icb_master_def_type * master,
  AxiCfg_QosPolicyType *qos_policy
)
{
  /* Convert to our internal bus specific type. */
  switch( master->bus->type )
  {
    case ICB_BUS_NOC:
    {
      icb_noc_master_policy_type *override = master->qos_policy_override->noc;
      memset(override, 0, sizeof(icb_noc_master_policy_type));
      override->default_priority           = qos_policy->noc.defaultPriority;
      override->limiter.bandwidth          = qos_policy->noc.limiter.bandwidth;
      override->limiter.saturation         = qos_policy->noc.limiter.saturation;
      override->limiter_enabled            = qos_policy->noc.limiterEnabled;
      override->regulator.low_priority     = qos_policy->noc.regulator.low_priority;
      override->regulator.high_priority    = qos_policy->noc.regulator.high_priority;
      override->regulator.bandwidth        = qos_policy->noc.regulator.bandwidth;
      override->regulator.saturation       = qos_policy->noc.regulator.saturation;
      override->regulator_mode.read        = qos_policy->noc.regulatorMode.read;
      override->regulator_mode.write       = qos_policy->noc.regulatorMode.write;
      override->urgency_forwarding_enabled = qos_policy->noc.urgencyForwardingEnabled;
      break;
    }
    default:
      /* Do nothing. */
      break;
  }
}

/**
  @brief Set or clear target disable on the passed slave.
 */
void _AxiCfgInt_DisableSlave
(
  icb_slave_def_type *slave,
  bool                disable
)
{
  uint32_t i, waited;

  /* Set or clear all disables first. */
  for( i = 0; i < slave->num_disables; i++ )
  {
    HAL_noc_DisableTarget( slave->bus->info,
                           slave->disable_ids[i],
                           disable );
  }

  /* If we just disabled this slave, wait for the ack. */
  if( true == disable )
  {
    for( i = 0, waited = 0;
         i < slave->num_disables && waited < (AXICFGINT_MAX_WAIT_US / AXICFGINT_WAIT_SIZE);
         i++ )
    {
      while( false == HAL_noc_IsTargetDisabled( slave->bus->info, slave->disable_ids[i] ) &&
             waited < (AXICFGINT_MAX_WAIT_US / AXICFGINT_WAIT_SIZE) )
      {
        busywait(AXICFGINT_WAIT_SIZE);
        waited += 1;
      }
    }

    /* Set disabled if we didn't time-out */
    if( waited < (AXICFGINT_MAX_WAIT_US / AXICFGINT_WAIT_SIZE) )
    {
      slave->is_disabled = true;
    }
    /* If we failed, unwind the operation. */
    else
    {
      for( i = 0; i < slave->num_disables; i++ )
      {
        HAL_noc_DisableTarget( slave->bus->info,
                               slave->disable_ids[i],
                               false );
      }
    }
  }
  /* Otherwise, clear disabled. */
  else
  {
    slave->is_disabled = false;
  }
}

/**
  @brief Apply any updates in the config set.
 */
void _AxiCfgInt_ApplyUpdates
(
  icb_variant_config_type *config
)
{
  uint32_t i;

  for( i = 0;
       i < config->num_entries;
       i++ )
  {
    out_dword( config->data[i].addr, config->data[i].val );
  }
}

/**
  @brief Update target data based on version or register information.
 */
bool _AxiCfgInt_UpdateInfo( void )
{
  icb_variant_list_type *variants = icb_info->variants;
  DalChipInfoFamilyType  chip_family;
  DalChipInfoVersionType chip_version;
  uint32_t               i;

  /* First, check if we have any alternate versions to apply. */
  if( NULL == variants )
  {
    return true;
  }

  /* Fetch register base address, if necessary. */
  for( i = 0; i < variants->num_configs; i++ )
  {
    icb_variant_config_type *config = variants->configs[i];

    if( NULL != config->reg_hw.base_name )
    {
      config->reg_hw.base_addr = AxiCfgOs_Mmap( config->reg_hw.base_name,
                                                config->reg_hw.base_addr,
                                                config->reg_hw.region_size );
      if( NULL == config->reg_hw.base_addr )
      {
        return false;
      }
    }
  }

  /* Fetch chip information. */
  chip_family  = DalChipInfo_ChipFamily();
  chip_version = DalChipInfo_ChipVersion();

  /* Go through the list of configurations for updates we need to apply. */
  for( i = 0; i < variants->num_configs; i++ )
  {
    icb_variant_config_type *config = variants->configs[i];

    /* We've found a matching version if:
     * The chip family matches *AND*
     *   We have an exact version match *OR*
     *   We allow matching all versions after the specified version. */
    if( chip_family == config->family &&
        (chip_version == config->version ||
         (false == config->match && chip_version >= config->version)) )
    {
      /* We may need to check a register, too.
       * We succeed if:
       *   We've no register to read (reg base_name == NULL) *OR*
       *   The masked register read equals register_val. */
      if( NULL == config->reg_hw.base_name ||
          config->reg_val == 
            (in_dword(config->reg_hw.base_addr + config->reg_offs) &
             config->reg_mask) )
      {
        _AxiCfgInt_ApplyUpdates( config );
      }
    }
  }

  return true;
}

/*============================================================================
                     STATIC FUNCTIONS - Queue processing
============================================================================*/
/**
 * request_enqueue
 *
 * @brief Queue a request to the appropriate request queue.
 *
 * @param A pointer to the head of the queue
 * @param A pointer to the request to queue
*/
static void request_enqueue
(
  icb_rpmh_request_type **queue_head,
  icb_rpmh_request_type * req
)
{
  icb_rpmh_request_type *iter, *prev;

  /* Only add requests once */
  if( req->is_queued )
  {
    return;
  }

  /* Sorted insert by clock id */
  for( iter = *queue_head, prev = NULL;
       iter != NULL;
       prev = iter, iter = iter->next )
  {
    /* Only break out if the clock id of the node to be inserted
     * is less than the current node to be compared.
     * We may want to also compare node names at some point. */
    if( req->node->clk_id < iter->node->clk_id )
    {
      break;
    }
  }

  /* Find out where we're inserting, as it changes what we do.
   * If we're inserting before the head, we need to replace it. */
  if( iter == *queue_head )
  {
    req->next = *queue_head;
    *queue_head = req;
  }
  /* Insertion somewhere else in the list.
   * Note, this works even if we're inserting at the end, as iter
   * ends up being NULL. */
  else
  {
    prev->next = req;
    req->next  = iter;
  }
  req->is_queued = true;
}

/**
 * get_rpmh_command_set
 *
 * @brief Fetch a populated command set from the given queue
 *
 * @param The RPMh handle to use
 * @param The set to send
 * @param A pointer to the head of the queue to process
 * @param Does the client want us to wait for completion?
 *
 * @returns The number of commands in the command set
*/
static uint32_t get_rpmh_command_set
(
  icb_rpmh_handle_type *  rpmh,
  icb_rpmh_request_type **queue_head,
  rpmh_set_enum           set,
  bool                    completion,
  rpmh_command_set_t *    command_set
)
{
  icb_rpmh_request_type *iter;
  uint32_t MAX_CMDS = rpmh_tcs_num_cmds(rpmh->rsc);
  uint32_t cmd_idx = 0, vcd_ff_mask = 0;

  /* If this is a WAKE set request, get the FF mask. */
  if( RPMH_SET_WAKE == set )
  {
    vcd_ff_mask = AxiCfgOs_GetWakeSetFFMask();
  }

  /* First, clear our command set. */
  memset(command_set, 0, sizeof(rpmh_command_set_t));

  /* Go through our list of items. */
  while( NULL != *queue_head )
  {
    /* Pop the head of the queue. */
    iter = *queue_head;
    iter->is_queued = false;
    *queue_head = iter->next;
    iter->next = NULL;

    /* Store this command. */
    command_set->commands[cmd_idx].address = iter->node->hw_id;
    command_set->commands[cmd_idx].data    = iter->val;
    cmd_idx++;

    /* Set the commit bit if:
     *   We've reached the end of the queue
     *   The next node is on a different clock domain
     *   We're at the maximum supported commands for this target
     */
    if( NULL == *queue_head ||
        iter->node->clk_id != (*queue_head)->node->clk_id ||
        cmd_idx >= MAX_CMDS )
    {
      bool cmd_completion;

      /* If this is the wake set, check to see if RPMh driver
       * wants us to not set completion for this VCD. */
      if( RPMH_SET_WAKE == set )
      {
        cmd_completion = completion &&
          ((1 << iter->node->clk_id) & vcd_ff_mask) == 0;
      }
      /* Use passed completion indication. */
      else
      {
        cmd_completion = completion;
      }

      /* Set the commit bit since we're at the end of a VCD.
       * Set the completion flag if we need to wait for a response.
       * Update bookkeeping in the command set. */
      command_set->commands[cmd_idx-1].data       |= BCM_VOTETABLE_COMMIT_BMSK;
      command_set->commands[cmd_idx-1].completion  = cmd_completion;
      command_set->set                             = set;
      command_set->num_commands                    = cmd_idx;

      /* Now determine if we're done with this command set.
       * We're done if any of the following are true:
       *    We're at the maximum supported commands for this target
       *    If we're not committing the active set AND the next node is on a different clock domain.
       * NOTE: This means that ACTIVE set requests are packed, while WAKE/SLEEP are not.
       * This is intentional. RPMh driver needs to handle WAKE/SLEEP packing itself. */
      if( NULL == *queue_head ||
          (set != RPMH_SET_ACTIVE && iter->node->clk_id != (*queue_head)->node->clk_id) ||
          cmd_idx >= MAX_CMDS )
      {
        break;
      }
    }
  }

  return cmd_idx;
}

/**
 * process_rpmh_requests
 *
 * @brief Process a queue and commit to hardware
 *
 * @param The RPMh handle to use
 * @param The set to send
 * @param A pointer to the head of the queue to process
 * @param Does the client want us to wait for completion?
 *
 * @returns The barrier id
*/
static uint32_t process_rpmh_requests
(
  icb_rpmh_handle_type   *rpmh,
  icb_rpmh_request_type **queue_head,
  rpmh_set_enum           set,
  bool                    wait_for_completion
)
{
  rpmh_command_set_t command_set;
  uint32_t msg_id = 0, barrier_id = 0;
  bool completion;

  /* Don't commit empty queues. */
  if( NULL == *queue_head )
  {
    return 0;
  }

  /* Never wait for completion on SLEEP set requests.
   * Always wait for completion on WAKE set requests.
   * Only use what the client wants for ACTIVE set requests. */
  if( RPMH_SET_ACTIVE == set )
  {
    completion = wait_for_completion;
  }
  else if( RPMH_SET_WAKE == set )
  {
    completion = true;
  }
  else /* SLEEP set */
  {
    completion = false;
  }

  /* While the command queue is not empty, generate command sets. */
  while( NULL != *queue_head )
  {
    /* Fetch the next command set. */
    get_rpmh_command_set( rpmh, queue_head, set, completion, &command_set );

    /* Issue commands and update barrier id. */
    msg_id = rpmh_issue_command_set(rpmh->handle, &command_set);
    RPM_MSG_ID_UPDATE(barrier_id, msg_id);
  }

  return barrier_id;
}

/**
 * encode_vote_value
 *
 * @brief Encode scaled Ib/Ab values into a BCM request
 *
 * @param ib the scaled ib to use
 * @param ab the scaled ab to use
*/
uint32_t encode_vote_value
(
  uint64_t ib,
  uint64_t ab
)
{
  /* Limit the voted request to the size of our voted field. */
  uint32_t ceil_ab = (ab <= BCM_VOTETABLE_VOTE_X_MAX? ab: BCM_VOTETABLE_VOTE_X_MAX) & BCM_VOTETABLE_VOTE_X_MAX;
  uint32_t ceil_ib = (ib <= BCM_VOTETABLE_VOTE_Y_MAX? ib: BCM_VOTETABLE_VOTE_Y_MAX) & BCM_VOTETABLE_VOTE_Y_MAX;

  /* Compose the command word. */
  uint32_t val = (ceil_ab << BCM_VOTETABLE_VOTE_X_SHFT) | (ceil_ib << BCM_VOTETABLE_VOTE_Y_SHFT);

  /* Only set the VALID bit if our command word is non-zero. */
  if( 0 != val )
  {
    val |= BCM_VOTETABLE_VOTE_VALID_BMSK;
  }

  return val;
}

/**
 * encode_latency_value
 *
 * @brief Encode a latency value into a BCM request
 *
 * @param the latency in ns to use
*/
uint32_t encode_latency_value
(
  uint32_t latency
)
{
  /* Limit the voted request to the size of our voted field. */
  uint32_t ceil_latency = (latency <= BCM_VOTETABLE_VOTE_X_MAX? latency: BCM_VOTETABLE_VOTE_X_MAX) & BCM_VOTETABLE_VOTE_X_MAX;

  /* Compose the command word. */
  uint32_t val = (ceil_latency << BCM_VOTETABLE_VOTE_X_SHFT);

  /* Workaround for QCTDD03989679, fixed on BCM v1.5.
   * On versions less than v1.5, the only way to remove a vote is to vote for maximum. */
  if( BCM_HW_VERSION_1_5 > icb_info->hw.version && 0 == val )
  {
    val = BCM_VOTETABLE_VOTE_X_MAX << BCM_VOTETABLE_VOTE_X_SHFT;
  }

  /* Only set the VALID bit if our command word is non-zero. */
  if( 0 != val )
  {
    val |= BCM_VOTETABLE_VOTE_VALID_BMSK;
  }

  return val;
}

/**
 * generate_wake_sleep_requests
 *
 * @brief Generate all wake and sleep set requests
*/
static void generate_wake_sleep_requests
(
  icb_topology_type *topology
)
{
  uint32_t i;

  for( i = 0; i < topology->num_hw_nodes; i++ )
  {
    icb_hw_node_type *hw_node = topology->hw_nodes[i];
    icb_node_state_type *state = &hw_node->state;

    switch( hw_node->type )
    {
      case ICB_HW_NODE_KIND_BANDWIDTH:
        if( state->supp_ib != 0 || state->supp_ab != 0 )
        {
          uint64_t ib, ab;

          /* Queue wake set request */
          AxiCfgInt_SetBandwidth(hw_node, state);

          /* Queue sleep set request */
          ib = CEILING_DIV(state->ib, hw_node->bw_unit);
          ab = CEILING_DIV(state->ab, hw_node->bw_unit);
          hw_node->sleep.val = encode_vote_value(ib, ab);
          request_enqueue(&sleep_queue, &hw_node->sleep);
        }
        break;
      case ICB_HW_NODE_KIND_LATENCY:
        if( state->supp_latency_ns != 0 )
        {
          /* Queue wake set request */
          AxiCfgInt_SetLatency(hw_node, state);

          /* Queue sleep set request */
          hw_node->sleep.val = encode_latency_value(state->latency_ns);
          request_enqueue(&sleep_queue, &hw_node->sleep);
        }
        break;
      case ICB_HW_NODE_KIND_VECTOR:
        /* We only need to do this if we have a suppressible vote, but no required votes.
         * If we have both, the ACTIVE request is ON, so both WAKE and SLEEP would also be ON.
         * If we have neither, the ACTIVE request is OFF, so both WAKE and SLEEP would also be OFF.
         * Otherwise, WAKE is ON and SLEEP must be OFF. */
        if( ( state->supp_ib != 0 || state->supp_ab != 0 ) &&
            ( state->ib == 0 || state->ab == 0 ) )
        {
          /* Queue wake request */
          hw_node->active.val = hw_node->output | BCM_VOTETABLE_VOTE_VALID_BMSK;
          request_enqueue( &active_queue, &hw_node->active );

          /* Queue sleep request */
          hw_node->sleep.val = 0;
          request_enqueue( &sleep_queue, &hw_node->sleep );
        }
        break;
      default:
        break;
    }
  }
}

/*============================================================================
                  STATIC FUNCTIONS - Deferral queue processing
============================================================================*/
/**
  @brief Queue a deferred master configuration.

  @param[in]  master      Master structure
 */
static void deferral_enqueue_master
(
  icb_master_def_type *master
)
{
  AxiCfgInt_DeferType *iter, *deferral;

  /* Insert deferral at the end and check for multiple queue attempts. */
  for( iter = defer_queue;
       iter != NULL && iter->next != NULL;
       iter = iter->next )
  {
    /* If already queued, bail out. */
    if( master == iter->master )
    {
      return;
    }
  }

  /* If we've gotten here, we're going to insert.
   * Allocate from our deferral pool. */
  deferral = (AxiCfgInt_DeferType *)Core_PoolAlloc(&defer_pool);

  /* If we allocate successfully, queue the request. */
  if( deferral )
  {
    deferral->master = master;
    deferral->slave  = NULL;
    deferral->next   = NULL;

    /* Find out where we're inserting, as it changes what we do.
    * If we're inserting before the head, we need to replace it. */
    if( NULL == iter )
    {
      defer_queue = deferral;
    }
    /* Insertion somewhere else in the list.
    * Note, this works even if we're inserting at the end, as iter
    * ends up being NULL. */
    else
    {
      iter->next = deferral;
    }
  }
}

/**
  @brief Queue a deferred master configuration.

  @param[in]  slave      Slave structure
 */
static void deferral_enqueue_slave
(
  icb_slave_def_type *slave
)
{
  AxiCfgInt_DeferType *iter, *deferral;

  /* Insert deferral at the end and check for multiple queue attempts. */
  for( iter = defer_queue;
       iter != NULL && iter->next != NULL;
       iter = iter->next )
  {
    /* If already queued, bail out. */
    if( slave == iter->slave )
    {
      return;
    }
  }

  /* If we've gotten here, we're going to insert.
   * Allocate from our deferral pool. */
  deferral = (AxiCfgInt_DeferType *)Core_PoolAlloc(&defer_pool);

  /* If we allocate successfully, queue the request. */
  if( deferral )
  {
    deferral->slave  = slave;
    deferral->master = NULL;
    deferral->next   = NULL;

    /* Find out where we're inserting, as it changes what we do.
    * If we're inserting before the head, we need to replace it. */
    if( NULL == iter )
    {
      defer_queue = deferral;
    }
    /* Insertion somewhere else in the list.
    * Note, this works even if we're inserting at the end, as iter
    * ends up being NULL. */
    else
    {
      iter->next = deferral;
    }
  }
}

/**
  @brief Process deferred QoS configuration.

  @param[in]  is_nas  Is this a NAS request?
 */
void process_defer_queue
(
  bool is_nas
)
{
  /* Process each element of the defer queue.*/
  while( defer_queue )
  {
    AxiCfgInt_DeferType *next = defer_queue->next;

    /* Is this a master deferral? */
    if( NULL != defer_queue->master )
    {
      /* If we aren't NAS, actually process this.
      * With NAS, the associated bandwidth request won't complete until
      * next wakeup, which means we cannot do this now. */
      if( !is_nas )
      {
        _AxiCfgInt_QoSSetLocal( defer_queue->master );
      }
    }
    /* Is this a slave deferral? */
    else if( NULL != defer_queue->slave )
    {
      _AxiCfgInt_DisableSlave( defer_queue->slave, false );
    }

    /* Unlink the current head of the queue. */
    defer_queue->next = NULL;
    defer_queue = next;
  }
}

/*============================================================================
                      PUBLIC FUNCTIONS
============================================================================*/
/**
  @brief  The method called to reprogram the hardware to default/reset values.

  @return bool
      @retval true  if successful
      @retval false if failed
 */
bool AxiCfgInt_Reset( void )
{
  if ( AXICFGINT_STATE_INITIALIZED == state )
  {
    /* Request all clocks. */
    _AxiCfgInt_EnableAllConfigClocks();

    _AxiCfgInt_SetMasterDefaults();

    /* Remove our config clock requests. */
    _AxiCfgInt_DisableAllConfigClocks();

    return true;
  }
  return false; 
}

/**
  @brief  The first method called to initialize AxiCfg driver. 
          Will set up all the required resources including locks, memory maps

  @return bool
      @retval true  if successful
      @retval false if failed or not yet complete
 */
bool AxiCfgInt_Init( void )
{
  uint32_t index, i; 

  /* If we've never tried to initialize, and the OS layer initialized successfully,
   * continue with our initialization. */
  if( AXICFGINT_STATE_UNINITIALIZED == state && AxiCfgOs_Init() )
  {
    state = AXICFGINT_STATE_INITIALIZING;

    icb_info = (icb_info_type *)AxiCfgInt_GetProperty( "icb_info" );

    /* Fail if not found. */
    if( NULL == icb_info )
    {
      return false;
    }

    /* Check for and apply any version/register differences. */
    if( !_AxiCfgInt_UpdateInfo() )
    {
      return false;
    }

    /* Fetch BCM hardware version. */
    icb_info->hw.base_addr = AxiCfgOs_Mmap( icb_info->hw.base_name,
                                            icb_info->hw.base_addr,
                                            icb_info->hw.region_size );

    if( NULL != icb_info->hw.base_addr )
    {
      icb_info->hw.version = 
        HWIO_INXM( icb_info->hw.base_addr, 
                   RPMH_BCM_HW_VERSION,
                   HWIO_FMSK(RPMH_BCM_HW_VERSION,MAJOR) |
                    HWIO_FMSK(RPMH_BCM_HW_VERSION, MINOR) );
    }
    else
    {
      /* Unable to map, error. */
      return false;
    }

    for( index = 0; index < icb_info->num_topologies; index++ )
    {
      icb_topology_type *topology = icb_info->topologies[index];

      /* HWIO map the bus space */
      for( i = 0; i < topology->num_buses; i++ )
      {
        uint32_t pwr_idx, clk_idx;
        icb_bus_def_type *bus = topology->buses[i];

        /* Validate bus parameters */
        if( NULL == bus || 
            (NULL == bus->info && ICB_BUS_VIRT != bus->type) )
        {
          /* Invalid bus information. */
          return false;
        }

        /* Get power domain IDs for any power domains for this bus */
        for( pwr_idx = 0;
            pwr_idx < bus->num_pwr_domains;
            pwr_idx++ )
        {
          AxiCfgOs_GetPowerDomainId( bus->pwr_domain_names[pwr_idx],
                                    &bus->pwr_domain_ids[pwr_idx] );
        }

        /* Get clock IDs for any enable/disable clocks for this bus */
        for( clk_idx = 0;
            clk_idx < bus->num_en_cfg_clks;
            clk_idx++ )
        {
          AxiCfgOs_GetClockId( bus->en_cfg_clk_names[clk_idx],
                              &bus->en_cfg_clk_ids[clk_idx] );
        }

        /* Map memory for the bus device registers, if necessary. */
        switch( bus->type )
        {
          case ICB_BUS_NOC:
            /* Skip mapping if no base name present.
            * Indicates no need to for hwio mapping. */
            if( NULL == bus->base_name )
            {
              continue;
            }

            ((HAL_noc_InfoType*)bus->info)->base_addr = AxiCfgOs_Mmap( bus->base_name,
                                                                       bus->base_addr,
                                                                       bus->region_size );
            if( NULL != ((HAL_noc_InfoType*)bus->info)->base_addr )
            {
              AxiCfgOs_MutexLock( ); 
              HAL_noc_Init( (HAL_noc_InfoType*)bus->info );
              AxiCfgOs_MutexUnlock( ); 
            }
            else
            {
              /* Unable to map, error. */
              return false;
            }
            break;
          case ICB_BUS_VIRT:
            /* Nothing to do here. */
            continue;
          default:
            /* Invalid bus type. */
            return false;
        }
      }

      /* Create rpmh client handles for this topology, using explicity managed mode. */
      topology->rpmh_handle.handle =
        rpmh_create_handle_explicit( topology->rpmh_handle.rsc, "ICB" );

      /* Check for handle creation failure. */
      if( NULL == topology->rpmh_handle.handle )
      {
        return false;
      }

      /* Fetch BCM mapping data and initialize request structures. */
      for( i = 0; i < topology->num_hw_nodes; i++ )
      {
        icb_hw_node_type *hw_node = topology->hw_nodes[i];
        struct bcm_db bcm_aux;
        uint8_t len = sizeof(bcm_aux);

        hw_node->active.node = hw_node;
        hw_node->sleep.node  = hw_node;

        /* Get BCM information for all BCMs.
        * Use aux fetch to check if the BCM exists. */
        if( CMD_DB_SUCCESS == cmd_db_query_aux_data(hw_node->id, &len, (uint8_t *)&bcm_aux) )
        {
          hw_node->hw_id   = cmd_db_query_addr( hw_node->id );
          hw_node->width   = bcm_aux.bcm_port;
          hw_node->bw_unit = bcm_aux.bw_unit;
          hw_node->clk_id  = bcm_aux.clk_id;
        }
        /* Unable to fetch from cmd_db, error. */
        else
        {
          return false;
        }
      }
    }

    /* Create normal rpmh handles for any bcm-as-clock nodes. */
    for( i = 0; i < icb_info->num_standalone_nodes; i++ )
    {
      icb_standalone_node_type *node = icb_info->standalone_node_definitions[i];
      icb_standalone_data_type *data = (icb_standalone_data_type *)node->node_definition.resources[0].data;
      icb_rpmh_handle_type *rpmh_handle = &data->rpmh_handle;

      rpmh_handle->handle = rpmh_create_handle(rpmh_handle->rsc, "ICB Standalone Node");

      /* Check for handle creation failure. */
      if( NULL == rpmh_handle->handle )
      {
        return false;
      }
    }

    /* Initialize our defer pool. */
    if( !is_defer_pool_initialized )
    {
      Core_PoolCreate(&defer_pool,
                      DEFER_POOL_INITIAL_SIZE,
                      sizeof(AxiCfgInt_DeferType) );
      is_defer_pool_initialized = true;
    }

    state = AXICFGINT_STATE_INITIALIZED;
  }

  return state == AXICFGINT_STATE_INITIALIZED;
}

/**
  @brief  Deinitialize the driver, cleaning up state.
 */
void AxiCfgInt_Deinit( void )
{
  uint32_t topo_idx, bus_idx, clk_idx, pwr_idx, idx;
  icb_bus_def_type *bus;
  icb_topology_type *topology;

  state = AXICFGINT_STATE_UNINITIALIZED;

  /* Clean up queue heads. */
  defer_queue  = NULL;
  active_queue = NULL;
  sleep_queue  = NULL;

  /* Clean up per topology data. */
  for( topo_idx = 0;
       topo_idx < icb_info->num_topologies;
       topo_idx++ )
  {
    topology = icb_info->topologies[topo_idx];
    for( bus_idx = 0;
         bus_idx < topology->num_buses;
         bus_idx++ )
    {
      bus = topology->buses[bus_idx];
      for( clk_idx = 0;
           clk_idx < bus->num_en_cfg_clks;
           clk_idx++ )
      {
        bus->en_cfg_clk_ids[clk_idx] = 0;
      }
      for( pwr_idx = 0;
           pwr_idx < bus->num_pwr_domains;
           pwr_idx++ )
      {
        bus->pwr_domain_ids[pwr_idx] = 0;
      }
    }

    /* Clean up RPMh handle. */
    topology->rpmh_handle.handle = NULL;
  }

  /* Clean up QoS but maintain overrides.
   * QoS is only applied by the default topology. */
  topology = icb_info->topologies[0];
  for( idx = 0;
       idx < topology->num_masters;
       idx++ )
  {
    icb_master_def_type *master = topology->masters[idx];
    
    /* Nothing to do unless we have defaults to begin with. */
    if( NULL != master->qos_policy && NULL != master->qos_port_ids )
    {
      icb_qos_policy_type *policy;

      /* Clear applied field for the active qos policy. */
      if( NULL == master->qos_policy_override )
      {
        policy = master->qos_policy;
      }
      /* Using override QoS policy. */
      else
      {
        policy = master->qos_policy_override;
      }

      /* Now, clear applied flags. */
      policy->noc->is_applied = false;
    }
  }

  /* Clean up any standalone nodes. */
  for( idx = 0; idx < icb_info->num_standalone_nodes; idx++ )
  {
    icb_standalone_node_type *node = icb_info->standalone_node_definitions[idx];
    icb_standalone_data_type *data = (icb_standalone_data_type *)node->node_definition.resources[0].data;
    data->rpmh_handle.handle = NULL;
  }
  
  /* Clear SoC info. */
  icb_info = NULL;

  /* Deinit OS layer */
  AxiCfgOs_Deinit();
}

/**
  @brief Configures any hardware related to a master that is modified on a bandwidth change.

  @param[in] master   Master port structure
 */
void AxiCfgInt_UpdateMasterBW
( 
  icb_master_def_type *master
)
{
  icb_qos_policy_type *qos_policy = _AxiCfgInt_QoSGetPolicy(master);

  /* If the master is locally owned and we have a set policy,
   * we may have QoS to update. */
  if( master->is_locally_owned && NULL != qos_policy )
  {
    /* If we don't need to defer, set it now. */
    if( !qos_policy->is_deferred )
    {
      _AxiCfgInt_QoSSetLocal(master);
    }
    /* If we need to defer only queue if:
     *  BW is non-zero and
     *  QoS hasn't been applied. */
    else if( 0 != master->state.total_bw &&
             false == qos_policy->noc->is_applied )
    {
      deferral_enqueue_master(master);
    }
  }
}

/**
  @brief Configures any hardware related to a slave that is modified on a bandwidth change.

  @param[in] slave   Slave port structure
 */
void AxiCfgInt_UpdateSlaveBW
( 
  icb_slave_def_type *slave
)
{
  /* We only have something to do here if the slave has any disables. */
  if( 0 != slave->num_disables )
  {
    /* If we're disabling, we need to do that now. */
    if( false == slave->is_disabled && 0 == slave->state.total_bw )
    {
      _AxiCfgInt_DisableSlave( slave, true );
    }
    /* If we're enabling, we need to defer this until after the clock is on. */
    else if( true == slave->is_disabled && 0 != slave->state.total_bw )
    {
      deferral_enqueue_slave( slave );
    }
    /* If we fall through here, there's nothing to do. */
  }
}

/**
  @brief Configures the bandwidth request for a hardware node.

  @param[in]  hw_node Pointer to the hardware node
  @param[in]  state   Pointer to the hardware node state to use
 */
void AxiCfgInt_SetBandwidth
( 
  icb_hw_node_type *   hw_node,
  icb_node_state_type *state
)
{
  uint64_t ib = CEILING_DIV(MAX(state->ib,state->supp_ib), hw_node->bw_unit);
  uint64_t ab = CEILING_DIV((state->ab + state->supp_ab), hw_node->bw_unit);
  
  hw_node->active.val = encode_vote_value(ib, ab);
  request_enqueue( &active_queue, &hw_node->active );
}

/**
  @brief Configures the request for a hardware vector node.

  @param[in]  hw_node Pointer to the hardware node
  @param[in]  state   Pointer to the hardware node state to use
 */
void AxiCfgInt_SetVector
( 
  icb_hw_node_type *   hw_node,
  icb_node_state_type *state
)
{
  /* Vector resources send their output value if total_bw is non-zero.
   * They send zero otherwise. */
  if( 0 != state->total_bw )
  {
    hw_node->active.val = hw_node->output | BCM_VOTETABLE_VOTE_VALID_BMSK;
  }
  else
  {
    hw_node->active.val = 0;
  }
  request_enqueue( &active_queue, &hw_node->active );
}

/**
  @brief Configures the latency request for a hardware node.

  @param[in]  hw_node Pointer to the hardware node
  @param[in]  state   Pointer to the hardware node state to use
 */
void AxiCfgInt_SetLatency
(
  icb_hw_node_type *   hw_node,
  icb_node_state_type *state
)
{
  uint32_t val = 0;
  
  /* Determine which latency to send.
   * Send the lowest non-zero value, or zero if both are. */
  if( state->latency_ns != 0 )
  {
    val = state->latency_ns;
  }
  if( state->supp_latency_ns != 0 &&
      state->supp_latency_ns < val )
  {
    val = state->supp_latency_ns;
  }

  hw_node->active.val = encode_latency_value( val );
  request_enqueue( &active_queue, &hw_node->active );
}

/**
 @copydoc AxiCfgInt_Commit
 */
void AxiCfgInt_Commit
(
  icb_topology_type *topology,
  unsigned int       request_attr
)
{
  uint32_t req_id            = 0;
  bool wait_for_completion   = !(request_attr & NPA_REQUEST_FIRE_AND_FORGET);
  bool is_nas                = request_attr & NPA_REQUEST_NEXT_AWAKE;
  icb_rpmh_handle_type *rpmh = &topology->rpmh_handle;

  /* Process requests for the active set, which we skip if nas. */
  if( !is_nas )
  {
    req_id = process_rpmh_requests( rpmh, &active_queue, RPMH_SET_ACTIVE, wait_for_completion );
  }

  /* Process wake/sleep set requests */
  rpmh_invalidate_explicit_cmds( rpmh->handle );
  generate_wake_sleep_requests( topology );
  (void)process_rpmh_requests( rpmh, &active_queue, RPMH_SET_WAKE, wait_for_completion );
  (void)process_rpmh_requests( rpmh, &sleep_queue, RPMH_SET_SLEEP, wait_for_completion );

  /* If we have nothing to do, leave completion unchanged.
   * If we have something to do and we're not NAS, set completion to true. */
  if( NULL != defer_queue && !is_nas )
  {
    wait_for_completion = true;
  }

  /* Barrier if necessary. */
  if( wait_for_completion )
  {
    rpmh_barrier_all( rpmh->handle, req_id );
  }

  /* Process defer queue. */
  process_defer_queue( is_nas );
}

/**
 @copydoc AxiCfgInt_CommitSingle
 */
void AxiCfgInt_CommitSingle
(
  icb_rpmh_handle_type *rpmh,
  icb_hw_node_type *    hw_node,
  unsigned int          request_attr
)
{
  uint32_t               req_id              = 0;
  bool                   wait_for_completion = !(request_attr & NPA_REQUEST_FIRE_AND_FORGET);
  bool                   is_nas              = request_attr & NPA_REQUEST_NEXT_AWAKE;
  icb_node_state_type *  state               = &hw_node->state;
  icb_rpmh_request_type *queue               = NULL;
  rpmh_set_enum          set;

  /* Generate active/wake request. */
  switch( hw_node->type )
  {
    case ICB_HW_NODE_KIND_BANDWIDTH:
    {
      uint64_t ib = CEILING_DIV(MAX(state->ib,state->supp_ib), hw_node->bw_unit);
      uint64_t ab = CEILING_DIV((state->ab + state->supp_ab), hw_node->bw_unit);
      hw_node->active.val = encode_vote_value(ib, ab);
      request_enqueue( &queue, &hw_node->active );
      break;
    }
    case ICB_HW_NODE_KIND_RAW:
    {
      hw_node->active.val = encode_vote_value(state->supp_ib, state->supp_ab);
      request_enqueue( &queue, &hw_node->active );
      break;
    }
    default:
      return;
  }

  /* Send the request to WAKE if NAS, ACTIVE otherwise. */
  set = is_nas? RPMH_SET_WAKE: RPMH_SET_ACTIVE;
  req_id = process_rpmh_requests( rpmh, &queue, set, wait_for_completion );

  /* Process sleep set request. */
  switch( hw_node->type )
  {
    case ICB_HW_NODE_KIND_BANDWIDTH:
    {
      uint64_t ib = CEILING_DIV(state->ib, hw_node->bw_unit);
      uint64_t ab = CEILING_DIV(state->ab, hw_node->bw_unit);
      hw_node->sleep.val = encode_vote_value(ib, ab);
      request_enqueue(&queue, &hw_node->sleep);
      break;
    }
    case ICB_HW_NODE_KIND_RAW:
    {
      hw_node->sleep.val = encode_vote_value(state->ib, state->ab);
      request_enqueue(&queue, &hw_node->sleep);
      break;
    }
    default:
      return;
  }
  (void)process_rpmh_requests( rpmh, &queue, RPMH_SET_SLEEP, wait_for_completion );

  /* Barrier if necessary. */
  if( wait_for_completion )
  {
    rpmh_barrier_all( rpmh->handle, req_id );
  }
}

/**
  @copydoc AxiCfgInt_SetQosPolicy
 */
AxiCfg_ReturnCodeType AxiCfgInt_SetQosPolicy
(
  icb_master_def_type * master,
  AxiCfg_QosPolicyType *qos_policy
)
{
  AxiCfg_ReturnCodeType ret = AXICFG_ERROR_INVALID;

  AxiCfgOs_MutexLock();
  do
  {
    /* Verify we have previous default settings and ports to set. */
    if( ( NULL != master->qos_policy ) &&
        ( NULL != master->qos_port_ids ) )
    {
      /* Check for policy clear. */
      if( NULL == qos_policy )
      {
        /* Nothing to do unless we had a previous override. */
        if( NULL != master->qos_policy_override )
        {
          switch( master->bus->type )
          {
            case ICB_BUS_NOC:
              DALSYS_Free(master->qos_policy_override->noc);
              break;
            default:
              /* Should get here, but bail if we do. */
              AxiCfgOs_MutexUnlock();
              return ret;
          }
          DALSYS_Free(master->qos_policy_override);
          master->qos_policy_override = NULL;

          /* Reconfigure the old defaults. */
          _AxiCfgInt_QoSSetLocal( master );
        }
      }
      else
      {
        /* If we did not have a previous override, allocate a new one.
        * Otherwise, just overwrite it. */
        if( NULL == master->qos_policy_override )
        {
          uint32_t size;

          if(DAL_SUCCESS !=
            DALSYS_Malloc( sizeof(icb_qos_policy_type), (void **)&master->qos_policy_override ))
          {
            break;
          }
          
          switch( master->bus->type )
          {
            case ICB_BUS_NOC:
              size = sizeof(icb_noc_master_policy_type);
              break;
            default:
              return ret;
          }

          if(DAL_SUCCESS !=
            DALSYS_Malloc( size, (void **)&master->qos_policy_override->noc ))
          {
            /* Clean-up previous allocation */
            DALSYS_Free(master->qos_policy_override);
            break;
          }
        }

        /* Convert and configure the new defaults. */
        _AxiCfgInt_ConvertQosPolicy( master, qos_policy );
        _AxiCfgInt_QoSSetLocal( master );
      }

      ret = AXICFG_SUCCESS;
    }
  } while(false);
  AxiCfgOs_MutexUnlock();

  return ret;
}

/**
 @copydoc AxiCfgInt_GetQueueLength
 */
uint32_t AxiCfgInt_GetQueueLength
(
  icb_topology_type *topology
)
{
  icb_rpmh_request_type *iter = active_queue;
  uint32_t MAX_CMDS = rpmh_tcs_num_cmds(topology->rpmh_handle.rsc);
  uint32_t cmds = 0, cmd_sets = 0;

  /* Walk the active queue and determine how many
   * command sets are required. */
  while( NULL != iter )
  {
    cmds++;

    /* Add a count:
     *   If we've reached the end of the queue
     *   If we're at the maximum supported commands for this target
     * NOTE: This assumes ACTIVE set request.
     */
    if( NULL == iter->next || cmds >= MAX_CMDS )
    {
      cmd_sets++;
      cmds = 0;
    }

    iter = iter->next;
  }

  return cmd_sets;
}

/**
 @copydoc AxiCfgInt_GetQueue
 */
uint32_t AxiCfgInt_GetQueue
(
  icb_topology_type * topology,
  rpmh_command_set_t *command_sets,
  uint32_t            num_command_sets
)
{
  uint32_t set_idx = 0;
  icb_rpmh_handle_type *rpmh = &topology->rpmh_handle;

  /* While the command queue is not empty and we still have room,
   * generate command sets. */
  for( set_idx = 0;
       set_idx < num_command_sets && NULL != active_queue;
       set_idx++ )
  {
    uint32_t commands;

    /* Fetch the next command set. */
    commands = get_rpmh_command_set( rpmh, 
                                     &active_queue,
                                     RPMH_SET_ACTIVE,
                                     true,
                                     &command_sets[set_idx] );

    /* If we have an empty command set, we're done. */
    if( 0 == commands )
    {
      break;
    }
  }

  return set_idx;
}

/**
 @copydoc AxiCfgInt_Flush
 */
void AxiCfgInt_Flush( void )
{
  icb_rpmh_request_type *iter;

  /* Walk the active queue, unlinking everything. */
  while( NULL != active_queue )
  {
    /* Pop the head of the queue. */
    iter = active_queue;
    iter->is_queued = false;
    active_queue = iter->next;
    iter->next = NULL;
  }
}

