/**************************************************************************
 * FILE: nvme_api.c
 *
 * NVMe Driver for Loader
 *
 * Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when        who   what, where, why
----------  ---   ---------------------------------------------------------
2019-05-08  wek   Add support for Format and Write Zeroes.
2018-11-08  wek   Create. (2:20-m)

===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "nvme_api.h"
#include "NvmeBsp.h"
#include "comdef.h"
#include "nvme_loader_bsp.h"
#include "busywait.h"

#ifndef ASSERT
#define ASSERT(x) do{ if(!(x)) while(1); }while(0)
#endif


#include "pcie_host.h"

void nvme_pcie_probe(pcie_host_dev_t *pdev, pcie_host_probe_cb_payload_t *data )
{
  uint32 devvendid;
  pcie_host_config_rw(pdev, PCIE_HOST_OPER_READ, PCIE_CFG_DEVVENDID, &devvendid);
  pcie_host_enable_device(pdev,0x7);
}

void *nvme_pcie_init(void)
{
  pcie_host_probe_cb_payload_t nvme_probe_cb_payload;
  pcie_host_res_t nvme_res;
  pcie_host_device_id_t device_id;
  pcie_host_probe_info_t probe_cb = {
      .drvname = "NVMe",
      .probe_fn = nvme_pcie_probe,
      .cb_data = &nvme_probe_cb_payload
  };
  //Device is NVMe:
  //Class Code : BCC{23:16} = 0x1; SCC{15:08} = 0x08; PI {07:00} = 0x2
  device_id.class_id = 0x10802;
  device_id.device_id = PCIE_HOST_ID_ANY;
  device_id.vendor_id = PCIE_HOST_ID_ANY;

  nvme_probe_cb_payload.user_data = NULL;
  nvme_probe_cb_payload.res = &nvme_res;

  pcie_host_register_device(device_id, probe_cb);

  return (void *)nvme_res.bars[0].pa;
}

void *pcie_get_mlbar(uint32 port)
{
  void *mlbar;
  mlbar = nvme_pcie_init();
  return mlbar;
}


/* 3.1.1 Offset 00h: CAP - Controller Capabilities */
PACKED struct nvme_capabilities
{
  uint16 max_queue_entries;/* Maximum Queue Entries Supported (MQES) */
  uint8  cont_queue_req:1; /* Contiguous Queues Required (CQR) */
  uint8  arb_supported:2;  /* Arbitration Mechanism Supported (AMS) */
  uint8  reserved1:5;      /* Reserved */
  uint8  timeout;          /* Timeout (TO), Units of 500ms */
  uint16 doorbell_stride:4;/* Doorbell Stride (DSTRD) (size of each doorbell) */
  uint16 reset_supported:1;/* NVM Subsystem Reset Supported (NSSRS) */
  uint16 cmd_set_sup:1;    /* Command Sets Supported (CSS) */
  uint16 reserved2:7;      /* Reserved */
  uint16 boot_part_sup:1;  /* Boot Partition Support (BPS) */
  uint16 reserved3:2;      /* Reserved */
  uint8  mem_pagesz_min:4; /* Memory Page Size Minimum (MPSMIN) */
  uint8  mem_pagesz_max:4; /* Memory Page Size Maximum (MPSMAX) */
  uint8  reserved4;        /* Reserved */
}PACKED_POST;

PACKED struct nvme_version
{
  uint8  ternary; /* Tertiary Version Number (TER). */
  uint8  minor;   /* Minor Version Number (MNR)     */
  uint16 major;   /* Major Version Number (MJR), typically 1. */
}PACKED_POST;

struct nvme_interrupt_set
{
  uint32 interrupt_mask_set;
};

struct nvme_interrupt_clear
{
  uint32 interrupt_mask_clear;
};

PACKED struct nvme_controller_configuration
{
  uint16 enable:1;    /* Enable the controller (EN) */
  uint16 reserved1:3;
  uint16 cmd_set:3;   /* I/O Command set selected (CSS), typically b000 for NVMe */
  uint16 mem_page_size:4; /* Memory Page Size (MPS) */
  uint16 arb_selected:3; /* Arbitration Mechanism Selected (AMS), b000 for Round Robin.  */
  uint16 shutdown_note:2; /* Shutdown Notification (SHN). */
  uint8 subq_entry_size:4; /* I/O Submission Queue Entry Size (IOSQES) */
  uint8 comq_entry_size:4; /* I/O Completion Queue Entry Size (IOCQES) */
  uint8 reserved2;
}PACKED_POST;

PACKED struct nvme_controller_status
{
  uint32 ready:1;          /* Ready (RDY) */
  uint32 fatal_error:1;    /* Controller Fatal Status (CFS) */
  uint32 shtudown_status:2;/* Shutdown Status (SHST) */
  uint32 reset_occurred:1; /* NVM Subsystem Reset Occurred (NSSRO) */
  uint32 cmd_paused:1;     /* Processing Paused (PP) */
  uint32 reserved:26;      /* Reserved */
}PACKED_POST;

struct nvme_subsystem_reset
{
  uint32 initiate_reset; /* NVM Subsystem Reset Control (NSSRC) */
};

PACKED struct nvme_admin_queue_attr
{
  uint16 submit_queue_sz:12; /* Admin Submission Queue Size (ASQS) */
  uint16 reserved1:4;
  uint16 complete_queue_sz:12; /* Admin Completion Queue Size (ACQS) */
  uint16 reserved2:4;
}PACKED_POST;

PACKED struct nvme_controller_mem_buffer_loc
{
  uint32 base_indicator:3; /* Base Indicator Register (BIR) */
  uint32 reserved:9;
  uint32 offset:20;        /* Offset (OFST) */
}PACKED_POST;

PACKED struct nvme_controller_mem_buffer_sz
{
  uint32 sub_queue_sup:1; /* Submission Queue Support (SQS) */
  uint32 com_queue_sup:1; /* Completion Queue Support (CQS) */
  uint32 prp_sgl_sup:1;   /* PRP SGL List Support (LISTS) */
  uint32 read_data_sup:1; /* Read Data Support (RDS) */
  uint32 write_data_sup:1;/* Write Data Support (WDS) */
  uint32 reserved:3;
  uint32 size_units:4;    /* Size Units (SZU) */
  uint32 size:20;         /* Size (SZ) */
}PACKED_POST;

PACKED struct nvme_boot_partition_info
{
  uint32 boot_part_size:15; /* Boot Partition Size (BPSZ) */
  uint32 reserved1:9;
  uint32 boot_read_status:2; /* Boot Read Status (BRS) */
  uint32 reserved2:5;
  uint32 active_boot_id:1; /* Active Boot Partition ID (ABPID) */
}PACKED_POST;

PACKED struct nvme_boot_partition_read_sel
{
  uint32 read_size:10;    /* Boot Partition Read Size (BPRSZ) */
  uint32 read_offset:20; /* Boot Partition Read Offset (BPROF) */
  uint32 reserved:1;
  uint32 boot_identifier:1;/* Boot Partition Identifier (BPID) */
}PACKED_POST;

PACKED struct nvme_boot_partition_mem_buf_loc
{
  uint64 reserved:12;
  uint64 base_addr:52; /* Boot Partition Memory Buffer Base Address (BMBBA) */
}PACKED_POST;


/* Submission and Completion queue tail/head doorbell structure. */
PACKED struct nvme_queue_doorbell
{
  uint16 queue;
  uint16 reserved;
}PACKED_POT;

PACKED struct nvme_controller_registers
{
  struct nvme_capabilities cap;
  struct nvme_version ver;
  struct nvme_interrupt_set intms;
  struct nvme_interrupt_clear intmc;
  struct nvme_controller_configuration cc;
  uint32 reserved1;
  struct nvme_controller_status csts;
  struct nvme_subsystem_reset nssr;
  struct nvme_admin_queue_attr aqa;
  uint64 admin_subq_base; /* Must be 4K aligned, last 12 bits are reserved */
  uint64 admin_comq_base; /* Must be 4K aligned, last 12 bits are reserved */
  struct nvme_controller_mem_buffer_loc cmbloc;
  struct nvme_controller_mem_buffer_sz cmbsz;
  struct nvme_boot_partition_info bpinfo;
  struct nvme_boot_partition_read_sel bprsel;
  struct nvme_boot_partition_mem_buf_loc bpmbl;
  uint8 reserved2[0xEB0]; /* 0xEB0 = 3760 */
  uint8 command_set_specific[256];
  struct nvme_queue_doorbell subq_tail_dbl; /* Submit Queue Tail Doorbell. */
}PACKED_POST;

PACKED struct nvme_command_dword0
{
  uint8 opcode;    /* Opcode (OPC) */
  uint8 fused:2;   /* Fused Operation (FUSE) */
  uint8 reserved:4;
  uint8 psdt:2;    /* PRP or SGL for Data Transfer (PSDT) */
  uint16 cmd_id;   /* Command Identifier (CID) */
}PACKED_POST;

PACKED struct nvme_sgl_descriptor
{
  uint64 address;
  uint32 length;
  uint8  reserved[3];
  uint8  type:4;
  uint8  sub_type:4;
}PACKED_POST;

PACKED struct nvme_submit_entry
{
  struct nvme_command_dword0 cmd;
  uint32 namespace_id;   /* Namespace Identifier (NSID) */
  uint64 reserved;
  uint64 meta_pointer;   /* Metadata Pointer (MPTR) */
  uint64 prp1;           /* Data Pointer (DPTR) PRP Entry 1 (PRP1) */
  uint64 prp2;           /* Data Pointer (DPTR) PRP Entry 2 (PRP2) */
  uint32 cdw10;          /* Command Dword 10 (CDW10) */
  uint32 cdw11;          /* Command Dword 11 (CDW11) */
  uint32 cdw12;          /* Command Dword 12 (CDW12) */
  uint32 cdw13;          /* Command Dword 13 (CDW13) */
  uint32 cdw14;          /* Command Dword 14 (CDW14) */
  uint32 cdw15;          /* Command Dword 15 (CDW15) */
}PACKED_POST;


PACKED struct nvme_complete_entry
{
  uint32 cmd_specific;
  uint32 reserved1;

  /* Completion Queue Entry: DW 2 */
  uint16 subq_headp; /* SQ Head Pointer (SQHD) */
  uint16 subq_id;    /* SQ Identifier (SQID) */

  /* Completion Queue Entry: DW 3 */
  uint16 cmd_id;       /* Command Identifier (CID) */
  uint16 phase_tag:1;  /* Phase Tag (P) */
  uint16 status_code:8;/* Status Code (SC) */
  uint16 status_code_type:3; /* Status Code Type (SCT) */
  uint16 reserved2:2;
  uint16 more:1;       /* More (M) */
  uint16 do_not_retry:1; /* Do Not Retry (DNR) */
}PACKED_POST;

PACKED struct nvme_fru_guid
{
  uint8 vseid[8]; /* 112|Vendor Specific extension identifier */
  uint8 oiu[3];   /* 120|Organizationally Unique Identifier */
  uint8 oiue[5];  /* 123|OUI Extension */
}PACKED_POST;

PACKED struct nvme_oacs /* optional_admin_cmd_sup*/
{
  uint16 security_cmds:1;/* bit 0: Security Send and Security Receive supported */
  uint16 format:1;       /* bit 1: Is format supported. */
  uint16 fw_update:1;    /* bit 2: Supports Fimware Comit and Firmware Image Download Cmds. */
  uint16 ns_mang:1;      /* bit 3: Supports Namespace Management. */
  uint16 dev_selftest:1; /* bit 4: Supports Device Self Test */
  uint16 directives:1;   /* bit 5: Supports Directives*/
  uint16 nvme_mi:1;      /* Supports NVMe-MI send and receive commands */
  uint16 virt_mang:1;    /* Supports Virtualization Management. */
  uint16 drbl_bcfg:1;    /* Doorbell Buffer Config command. */
  uint16 reserved:7;
}PACKED_POST;

PACKED struct nvme_frmw /* Firmware Updates */
{
  uint8 read_only:1; /* First firmware slot is read only. */
  uint8 fw_count:3;  /* Number of firmware slots supported */
  uint8 fw_no_reset:1; /* Supports FW update without a reset */
  uint8 reserved:3;
}PACKED_POST;

PACKED struct rpmb_bits
{
  uint16 num_units:3;   /* Number of RPMB Units */
  uint16 auth_method:3; /* Authentication Method */
  uint16 reserved:10;
  uint8  total_size;   /* Size of each RPMB size in 128KB units. a value of 0 indicates size of 128Kb */
  uint8  access_size;  /* In units of 512. This is a 0’s based value. A value of 0h indicates a size of 512B*/
}PACKED_POST;

PACKED struct pwr_state_desc
{
  uint16 max_power;   /* Maximum Power (MP) */
  uint8  reserved1;
  uint8  max_power_scale:1; /* Max Power Scale (MXPS) */
  uint8  non_operational:1; /* Non-Operational State (NOPS) */
  uint8  reserved2:6;
  uint32 entry_latency;    /* Entry Latency (ENLAT) */
  uint32 exit_latency;     /* Exit Latency (EXLAT) */
  uint8  rel_read_tp:5;    /* Relative Read Throughput (RRT) */
  uint8  reserved3:3;
  uint8  rel_read_lat:5;   /* Relative Read Latency (RRL) */
  uint8  reserved4:3;
  uint8  rel_write_tp:5;    /* Relative Write Throughput (RWT) */
  uint8  reserved5:3;
  uint8  rel_write_lat:5;   /* Relative Write Latency (RWL) */
  uint8  reserved6:3;
  uint16 idle_power;        /* Idle Power (IDLP) */
  uint8  reserved7:6;
  uint8  idle_power_scale:2; /* Idle Power Scale (IPS) */
  uint8  reserved8;
  uint16 active_power;      /* Active Power (ACTP) */
  uint8  act_power_workload:2; /* Active Power Workload (APW) */
  uint8  reserved9:3;
  uint8  act_power_scale:2;  /* Active Power Scale (APS) */
  uint8  reserved10[9];
}PACKED_POST;

/* 128 bit fields used for device capacity. */
PACKED struct _uint128
{
  uint64 low;
  uint64 high;
}PACKED_POST;

PACKED struct nvme_oncs
{
  uint16 compare:1;             /* Supports Compare command (0x05) */
  uint16 write_uncorrectable:1; /* Supports Write Uncorrectable command (0x04)*/
  uint16 dataset_management:1; /* Supports Dataset Management command (0x09) */
  uint16 write_zeroes:1; /* Supports write_zeroes command (0x08) */
  uint16 save_select_field:1; /* Supports Save field for set features, select field for get features command (0x) */
  uint16 reservations:1; /* Supports Reservations commands (0x0D,0x0E,0x11,0x15) */
  uint16 timestamp:1; /* Supports Timestamp Feature */
  uint16 reserved:9; /* Reserved */
}PACKED_POST;

PACKED struct nvme_fna
{
  uint8 global_format:1; /* If 0 format per  namespace is supported. If 1 format applies to all namespaces */
  uint8 global_secure_erase:1;/* If 0 secure erase per namespace is supported. If 1 secure erase applies to all namespaces */
  uint8 cryptograpic_erase:1; /* Is cryptographic erase supported*/
  uint8 reserved:5;
}PACKED_POST;


/* This data structure is 4Kbytes in size. Figure 109. */
PACKED struct nvme_identify_controller
{
  /*                            offset                                        */
  uint16 vendor_id;           /*    0|PCI Vendor ID (VID) */
  uint16 subsys_vid;          /*    2|PCI sub-system vendor ID (SSVID) */
  uint8  serial[20];          /*    4|Product serial number (SN) */
  uint8  model_num[40];       /*   24|Product model number  (MN) */
  uint8  fw_rev[8];           /*   64|Firmware Revision (FR) */
  uint8  rec_arb_burst;       /*   72|Recommended Arbitration Burst (RAB) */
  uint8  ieee_oui[3];         /*   73|IEEE Organization Unique Identifier (IEEE) */
  uint8  cmic;                /*   76|Controller Multi-Path I/O and Namespace Sharing Capabilities (CMIC) */
  uint8  max_data_trans_size; /*   77|Maximum Data Transfer Size (MDTS) */
  uint16 controller_id;       /*   78|Controller ID (CNTLID) */
  struct nvme_version version; /*   80|Version (VER) */
  uint32 rtd3_resume_lat;     /*   84|RTD3 Resume Latency (RTD3R) */
  uint32 rtd3_entry_lat;      /*   88|RTD3 Entry Latency (RTD3E) */
  uint32 oaes;                /*   92|Optional Asynchronous Events Supported (OAES) */
  uint32 controller_atrib;    /*   96|Controller Attributes (CTRATT) */
  uint8  reserved1[12];       /*  100|Reserved */
  struct nvme_fru_guid fguid;  /*  112|FRU Globally Unique Identifier (FGUID) */
  uint8  reserved2[112];      /*  128|Reserved */
  uint8  nvme_management[16]; /*  240|NVMe-MI */
  struct nvme_oacs opt_cmd_sup;/*  256|Optional Admin Command Support (OACS) */
  uint8  abort_cmd_limit;     /*  258|Abort Command Limit (ACL) */
  uint8  async_event_rlimit;  /*  259|Asynchronous Event Request Limit (AERL) */
  struct nvme_frmw fw_update;  /*  260|Firmware Updates (FRMW) */
  uint8  log_page_attr;       /*  261|Log Page Attributes (LPA) */
  uint8  err_logp_entries;    /*  262|Error Log Page Entries (ELPE) */
  uint8  num_power_states;    /*  263|Number of Power States Support (NPSS) */
  uint8  avscc;               /*  264|Admin Vendor Specific Command Configuration (AVSCC) */
  uint8  auto_power_trans;    /*  265|Autonomous Power State Transition Attributes (APSTA) */
  uint16 wctemp;              /*  266|Warning Composite Temperature Threshold (WCTEMP) */
  uint16 cctemp;              /*  268|Critical Composite Temperature Threshold (CCTEMP) */
  uint16 max_time_fw_act;     /*  270|Maximum Time for Firmware Activation (MTFA) */
  uint32 hmpre;               /*  272|Host Memory Buffer Preferred Size (HMPRE) */
  uint32 hmmin;               /*  276|Host Memory Buffer Minimum Size (HMMIN) */
  struct _uint128 tot_nvme_size;/*280|Total NVM Capacity (TNVMCAP) */
  struct _uint128 una_nvme_size;/*296|Unallocated NVM Capacity (UNVMCAP) */
  struct rpmb_bits rpmb_sup;  /*  312|Replay Protected Memory Block Support (RPMBS) */
  uint16 extend_self_test;    /*  316|Extended Device Self-test Time (EDSTT) */
  uint8  self_test_opt;       /*  318|Device Self-test Options (DSTO) */
  uint8  fw_upd_granularity;  /*  319|Firmware Update Granularity (FWUG) */
  uint16 keep_alive_sup;      /*  320|Keep Alive Support (KAS) */
  uint16 hctma;               /*  322|Host Controlled Thermal Management Attributes (HCTMA) */
  uint16 mntmt;               /*  324|Minimum Thermal Management Temperature (MNTMT) */
  uint16 mxtmt;               /*  326|Maximum Thermal Management Temperature (MXTMT) */
  uint32 sanitize_cap;        /*  328|Sanitize Capabilities (SANICAP) */
  uint8  reserved3[180];      /*  332|Reserved bytes in NVM Express 1.3 Spec  */
  uint8  subq_entry_size;     /*  512|Submission Queue Entry Size (SQES) */
  uint8  comq_entry_size;     /*  513|Completion Queue Entry Size (CQES) */
  uint16 max_outstand_cmd;    /*  514|Maximum Outstanding Commands (MAXCMD) */
  uint32 num_namespace;       /*  516|Number of Namespaces (NN) */
  struct nvme_oncs opt_nvm_cmd;/* 520|Optional NVM Command Support (ONCS) */
  uint16 fused_oper_sup;      /*  522|Fused Operation Support (FUSES) */
  struct nvme_fna format_nvm_attr;/*524|Format NVM Attributes (FNA) */
  uint8  volatile_write_cache;/*  525|Volatile Write Cache (VWC) */
  uint16 awun;                /*  526|Atomic Write Unit Normal (AWUN) */
  uint16 awupf;               /*  528|Atomic Write Unit Power Fail (AWUPF) */
  uint8  nvscc;               /*  530|NVM Vendor Specific Command Configuration (NVSCC) */
  uint8  reserved4;           /*  531|Reserved bytes in NVM Express 1.3 Spec  */
  uint16 acwu;                /*  532|Atomic Compare & Write Unit (ACWU) */
  uint16 reserved5;           /*  534|Reserved bytes in NVM Express 1.3 Spec  */
  uint32 sgl_support;         /*  536|SGL Support (SGLS) */
  uint8  reserved6[228];      /*  540|Reserved bytes in NVM Express 1.3 Spec  */
  uint8  nvm_ss_name[256];    /*  768|NVM Subsystem NVMe Qualified Name (SUBNQN) */
  uint8  reserved7[768];      /* 1024|Reserved bytes in Nvm Express 1.3 Spec */
  uint8  nvme_fabrics[256];   /* 1792|Special section of NVMe over Fabrics */

  struct pwr_state_desc psd[32];/*2048|Power State X Descriptor (PSDX) */

  uint8  vendor_specific[1024];/*3073|Vendor Specific. */
}PACKED_POST;

/* Figure 115 of NVM Express 1.3 Spec. */
PACKED struct nvme_lba_format
{
  uint16 metadata_size;   /* Metadata Size (MS) */
  uint8  lba_size_shift;  /* LBA Data Size (LBADS) in power of 2. 9 means 2^9=512 */
  uint8  relative_perf:2; /* Relative Performance (RP) */
  uint8  reserved:6;
}PACKED_POST;

PACKED struct nvme_flbas_type
{
  uint8 index:4;  /* Index into the LBA Format table in nvme_indetify_namepsace*/
  uint8 metadata_present:1; /* Is the metadata sent as part of the transfers. */
  uint8 reserved:3;
}PACKED_POST;

/* This data structure is 4Kbytes in size. Figure 114 of NVM Express 1.3 Spec. */
PACKED struct nvme_identify_namespace
{
  /*                            offset                                        */
  uint64 size_blocks;          /*   0|Namespace Size (NSZE)                  */
  uint64 capacity_blocks;      /*   8|Namespace Capacity (NCAP)              */
  uint64 utilization_blocks;   /*  16|Namespace Utilization (NUSE)           */
  uint8  features;             /*  24|Namespace Features (NSFEAT)            */
  uint8  num_lba_formats;      /*  25|Number of LBA Formats (NLBAF) 0's based */
  struct nvme_flbas_type lba_format;/*  26|Formatted LBA Size (FLBAS)             */
  uint8  metadata_capabilites; /*  27|Metadata Capabilities (MC)             */
  uint8  e2e_data_proc_cap;    /*  28|End-to-end Data Protection Capabilities (DPC) */
  uint8  e2e_data_proc_setting;/*  29|End-to-end Data Protection Type Settings (DPS) */
  uint8  nmic;                 /*  30|Namespace Multi-path I/O and Namespace Sharing Capabilities (NMIC) */
  uint8  reserve_cap;          /*  31|Reservation Capabilities (RESCAP)      */
  uint8  format_progress;      /*  32|Format Progress Indicator (FPI)        */
  uint8  dealloc_feature;      /*  33|Deallocate Logical Block Features (DLFEAT) */
  uint16 ns_atomic_wu_normal;  /*  34|Namespace Atomic Write Unit Normal (NAWUN) */
  uint16 ns_atomic_wu_pfail;   /*  36|Namespace Atomic Write Unit Power Fail (NAWUPF) */
  uint16 ns_atomic_cwu;        /*  38|Namespace Atomic Compare & Write Unit (NACWU) */
  uint16 ns_atomic_bsn;        /*  40|Namespace Atomic Boundary Size Normal (NABSN) */
  uint16 ns_atomic_offset;     /*  42|Namespace Atomic Boundary Offset (NABO) */
  uint16 ns_atomic_bspf;       /*  44|Namespace Atomic Boundary Size Power Fail (NABSPF) */
  uint16 ns_optimal_io_boundary;/* 46|Namespace Optimal IO Boundary (NOIOB)   */
  struct _uint128 ns_size;     /*  48|NVM Capacity (NVMCAP): Size in bytes    */
  uint8  reserved1[40];        /*  64|Reserved bytes in NVM Express 1.3 Spec  */
  uint8  guid[16];             /* 104|Namespace Globally Unique Identifier (NGUID) */
  uint64 ieee_uid;             /* 120|IEEE Extended Unique Identifier (EUI64) */
  struct nvme_lba_format lbaf[16]; /* 128|LBA Format  0-15 Support (LBAF0-LBAF15)           */
  uint8  reserved2[192];       /* 192|Reserved bytes in NVM Express 1.3 Spec  */
  uint8  vendor_specific[3712];/* 384|Vendor Specific                         */
}PACKED_POST;





#define NVME_MAX_ERROR_LOG 16

#define NVME_SUBQ_ENTRY_SIZE   64  /* Defined in the spec section 4.2 */
#define NVME_COMQ_ENTRY_SIZE   16  /* Defined in the spec section 4.6 */
#define NVME_IO_MAX_BLOCK_COUNT 0x10000 /* The max value of this field is uint16 + 1 */

#define NVME_ADMIN_CMD_OPCODE_DELETE_IO_SUBMISSION_QUEUE (0x00)
#define NVME_ADMIN_CMD_OPCODE_CREATE_IO_SUBMISSION_QUEUE (0x01)
#define NVME_ADMIN_CMD_OPCODE_GET_LOG_PAGE               (0x02)
#define NVME_ADMIN_CMD_OPCODE_DELETE_IO_COMPLETION_QUEUE (0x04)
#define NVME_ADMIN_CMD_OPCODE_CREATE_IO_COMPLETION_QUEUE (0x05)
#define NVME_ADMIN_CMD_OPCODE_IDENTIFY                   (0x06)
#define NVME_ADMIN_CMD_OPCODE_ABORT                      (0x08)
#define NVME_ADMIN_CMD_OPCODE_SET_FEATURES               (0x09)
#define NVME_ADMIN_CMD_OPCODE_GET_FEATURES               (0x0A)
#define NVME_ADMIN_CMD_OPCODE_ASYNC_EVENT_REQUEST        (0x0C)
#define NVME_ADMIN_CMD_OPCODE_NAMESPACE_MANAGEMENT       (0x0D)
#define NVME_ADMIN_CMD_OPCODE_FIRMWARE_COMMIT            (0x10)
#define NVME_ADMIN_CMD_OPCODE_FIRMWARE_IMAGE_DOWNLOAD    (0x11)
#define NVME_ADMIN_CMD_OPCODE_DEVICE_SELFTEST            (0x14)
#define NVME_ADMIN_CMD_OPCODE_NAMESPACE_ATTACHMENT       (0x15)
#define NVME_ADMIN_CMD_OPCODE_KEEP_ALIVE                 (0x18)
#define NVME_ADMIN_CMD_OPCODE_FORMAT_NVM                 (0x80)
#define NVME_ADMIN_CMD_OPCODE_SECURITY_SEND              (0x81)
#define NVME_ADMIN_CMD_OPCODE_SECURITY_RECEIVE           (0x82)
#define NVME_ADMIN_CMD_OPCODE_DIRECTIVE_SEND             (0x19)
#define NVME_ADMIN_CMD_OPCODE_DIRECTIVE_RECEIVE          (0x1A)
#define NVME_ADMIN_CMD_OPCODE_VIRTUALIZATION_MANAGEMENT  (0x1C)
#define NVME_ADMIN_CMD_OPCODE_NVME_MI_SEND               (0x1D)
#define NVME_ADMIN_CMD_OPCODE_NVME_MI_RECEIVE            (0x1E)
#define NVME_ADMIN_CMD_OPCODE_DOORBELL_BUFFER_CONFIG     (0x7C)

#define NVME_ADMIN_CMD_OPCODE_FORMAT_NVM                 (0x80)
#define NVME_ADMIN_CMD_OPCODE_SECURITY_SEND              (0x81)
#define NVME_ADMIN_CMD_OPCODE_SECURITY_RECEIVE           (0x82)
#define NVME_ADMIN_CMD_OPCODE_SANITIZE                   (0x84)


#define NVME_ADMIN_CMD_NOT_FUSED                         (0)
#define NVME_ADMIN_CMD_FUSED_FIRST                       (b01)
#define NVME_ADMIN_CMD_FUSED_SECOND                      (b10)

#define NVME_ADMIN_CMD_PRP_USED                          (0)
#define NVME_ADMIN_CMD_SGL_SINGLE                        (b01)
#define NVME_ADMIN_CMD_SGL_SEGMENT                       (b10)

/* NVM Command set, figure 188 (section 6) */
#define NVME_IO_CMD_FLUSH                                (0x00)
#define NVME_IO_CMD_WRITE                                (0x01)
#define NVME_IO_CMD_READ                                 (0x02)
#define NVME_IO_CMD_WRITE_ZEROES                         (0x08)
#define NVME_IO_CMD_DATASET_MANAGEMENT                   (0x09)


/* All device error status code will start with 0x08000000 */
/* Status Code - Generic Command Status Values, Figure 31. */
#define NVME_DEVICE_GENERIC_STATUS_CODE(x)  (0x08000000 | x)
/* All device error status code will start with 0x08000000 */
#define NVME_INVALID_COMMAND_OPCODE                  0x08000001
#define NVME_INVALID_FIELD_IN_COMMAND                0x08000002
#define NVME_COMMAND_ID_CONFLICT                     0x08000003
#define NVME_DATA_TRANSFER_ERROR                     0x08000004
#define NVME_COMMANDS_ABORTED_DUE_TO_POWER_LOSS_NOTIFICATION 0x08000005
#define NVME_INTERNAL_ERROR                          0x08000006
#define NVME_COMMAND_ABORT_REQUESTED                 0x08000007
#define NVME_COMMAND_ABORTED_DUE_TO_SQ_DELETION      0x08000008
#define NVME_COMMAND_ABORTED_DUE_TO_FAILED_FUSED_COMMAND  0x08000009
#define NVME_COMMAND_ABORTED_DUE_TO_MISSING_FUSED_COMMAND 0x0800000A
#define NVME_INVALID_NAMESPACE_OR_FORMAT             0x0800000B
#define NVME_COMMAND_SEQUENCE_ERROR                  0x0800000C
#define NVME_INVALID_SGL_SEGMENT_DESCRIPTOR          0x0800000D
#define NVME_INVALID_NUMBER_OF_SGL_DESCRIPTORS       0x0800000E
#define NVME_DATA_SGL_LENGTH_INVALID                 0x0800000F
#define NVME_METADATA_SGL_LENGTH_INVALID             0x08000010
#define NVME_SGL_DESCRIPTOR_TYPE_INVALID             0x08000011
#define NVME_INVALID_USE_OF_CONTROLLER_MEMORY_BUFFER 0x08000012
#define NVME_PRP_OFFSET_INVALID                      0x08000013
#define NVME_ATOMIC_WRITE_UNIT_EXCEEDED              0x08000014
#define NVME_OPERATION_DENIED                        0x08000015
#define NVME_SGL_OFFSET_INVALID                      0x08000016
#define NVME_HOST_IDENTIFIER_INCONSISTENT_FORMAT     0x08000018
#define NVME_KEEP_ALIVE_TIMEOUT_EXPIRED              0x08000019
#define NVME_KEEP_ALIVE_TIMEOUT_INVALID              0x0800001A
#define NVME_COMMAND_ABORTED_DUE_TO_PREEMPT_AND_ABORT 0x0800001B
#define NVME_SANITIZE_FAILED                         0x0800001C
#define NVME_SANITIZE_IN_PROGRESS                    0x0800001D
#define NVME_SGL_DATA_BLOCK_GRANULARITY_INVALID      0x0800001E
#define NVME_COMMAND_NOT_SUPPORTED_FOR_QUEUE_IN_CMB  0x0800001F
#define NVME_LBA_OUT_OF_RANGE                        0x08000080
#define NVME_CAPACITY_EXCEEDED                       0x08000081
#define NVME_NAMESPACE_NOT_READY                     0x08000082
#define NVME_RESERVATION_CONFLICT                    0x08000083
#define NVME_FORMAT_IN_PROGRESS                      0x08000084

/* Status Code - Command Specific Status Values, Figure 33. */
/* Command specific status codes. */
#define NVME_DEVICE_CMD_STATUS_CODE(x)         (0x08100000 | x)
/* Command specific status codes. */
#define NVME_COMPLETION_QUEUE_INVALID               0x08100000
#define NVME_INVALID_QUEUE_IDENTIFIER               0x08100001
#define NVME_INVALID_QUEUE_SIZE                     0x08100002
#define NVME_ABORT_COMMAND_LIMIT_EXCEEDED           0x08100003
/* RESERVED                                        0x08100004 */
#define NVME_ASYNCHRONOUS_EVENT_REQUEST_LIMIT_EXCEEDED 0x08100005
#define NVME_INVALID_FIRMWARE_SLOT                  0x08100006
#define NVME_INVALID_FIRMWARE_IMAGE                 0x08100007
#define NVME_INVALID_INTERRUPT_VECTOR               0x08100008
#define NVME_INVALID_LOG_PAGE                       0x08100009
#define NVME_INVALID_FORMAT                         0x0810000A
#define NVME_FIRMWARE_ACTIVATION_REQUIRES_CONVENTIONAL_RESET 0x0810000B
#define NVME_INVALID_QUEUE_DELETION                 0x0810000C
#define NVME_FEATURE_IDENTIFIER_NOT_SAVEABLE        0x0810000D
#define NVME_FEATURE_NOT_CHANGEABLE                 0x0810000E
#define NVME_FEATURE_NOT_NAMESPACE_SPECIFIC         0x0810000F
#define NVME_FIRMWARE_ACTIVATION_REQUIRES_NVME_SUBSYSTEM_RESET 0x08100010
#define NVME_FIRMWARE_ACTIVATION_REQUIRES_RESET     0x08100011
#define NVME_FIRMWARE_ACTIVATION_REQUIRES_MAXIMUM_TIME_VIOLATION 0x08100012
#define NVME_FIRMWARE_ACTIVATION_PROHIBITED         0x08100013
#define NVME_OVERLAPPING_RANGE                      0x08100014
#define NVME_NAMESPACE_INSUFFICIENT_CAPACITY        0x08100015
#define NVME_NAMESPACE_IDENTIFIER_UNAVAILABLE       0x08100016
#define NVME_NAMESPACE_ALREADY_ATTACHED             0x08100018
#define NVME_NAMESPACE_IS_PRIVATE                   0x08100019
#define NVME_NAMESPACE_NOT_ATTACHED                 0x0810001A
#define NVME_THIN_PROVISIONING_NOT_SUPPORTED        0x0810001B
#define NVME_CONTROLLER_LIST_INVALID                0x0810001C
#define NVME_DEVICE_SELF_TEST_IN_PROGRESS           0x0810001D
#define NVME_BOOT_PARTITION_WRITE_PROHIBITED        0x0810001E
#define NVME_INVALID_CONTROLLER_IDENTIFIER          0x0810001F
#define NVME_INVALID_SECONDARY_CONTROLLER_STATE     0x08100020
#define NVME_INVALID_NUMBER_OF_CONTROLLER_RESOURCES 0x08100021
#define NVME_INVALID_RESOURCE_IDENTIFIER            0x08100022

/* Media and Data Integrity */
#define NVME_DEVICE_DATA_STATUS_CODE(x)     (0x08200000 | x)
#define NVME_DEVICE_RESERVED_STATUS_CODE(x) (0x08F00000 | x)
#define NVME_DEVICE_VENDOR_STATUS_CODE(x)   (0x08700000 | x)


struct nvme_error_entry
{
  uint32 error;
  const char *file;
  uint32 line;
  const char *func;
};

struct nvme_error_log
{
  uint64 err_count;
  size_t next_index;
  struct nvme_error_entry errors[NVME_MAX_ERROR_LOG];
};

struct nvme_queue
{
  volatile uint8  *base;
  uint16 count;
  uint16 head;
  uint16 tail;
  uint16 entry_sz;
  uint8  phase_tag:1;
  uint8  reserved:7;
};

int nvme_queue_init(struct nvme_queue *queue, volatile uint8 *base, uint16 count,
                   uint16 entry_sz)
{
  if (queue == NULL || base == NULL || count == 0 || entry_sz == 0)
    return -1;

  queue->base = base;
  queue->count = count;
  queue->head = 0;
  queue->tail = 0;
  queue->entry_sz = entry_sz;
  queue->phase_tag = 0;

  return 0;
}

uint16 nvme_queue_size_in_entries(struct nvme_queue *queue)
{
  if (queue == NULL)
    return 0;

  return queue->count;
}

uint16 nvme_queue_tail_index(struct nvme_queue *queue)
{
  if (queue == NULL)
    return 0;

  return queue->tail;
}

uint16 nvme_queue_head_index(struct nvme_queue *queue)
{
  if (queue == NULL)
    return 0;

  return queue->head;
}

int nvme_queue_is_full(struct nvme_queue *queue)
{
  int is_full;
  uint16 head, tail;
  int avail;

  if (queue == NULL)
    return 1;

  head = queue->head;
  tail = queue->tail;
  if (head > tail)
  {
    avail = head - tail;
  }
  else
  {
    int diff;
    diff = tail - head;
    avail = queue->count - diff;
  }

  if (avail > 1)
    is_full = 0;
  else
    is_full = 1;

  return is_full;
}

int nvme_queue_is_empty(struct nvme_queue *queue)
{
  if (queue == NULL)
    return 0;

  if (queue->head == queue->tail)
    return 1;
  else
    return 0;
}

static int volatile_memcpy(volatile uint8* out, volatile uint8* in, size_t size)
{
  size_t i;
  for (i = 0; i < size; i++)
    out[i] = in[i];

  return i;
}

int nvme_queue_inc_tail(struct nvme_queue *queue)
{
  if (queue == NULL)
    return -1;

  queue->tail++;
  if(queue->tail >= queue->count)
    queue->tail = 0;

  return queue->tail;
}

int nvme_queue_enqueue(struct nvme_queue *queue, uint8 *entry)
{
  volatile uint8 *next;
  int tail;

  if (queue == NULL || entry == NULL)
    return -1;

  if (nvme_queue_is_full(queue) == 1)
    return -1;

  next = queue->base + queue->entry_sz * queue->tail;
  volatile_memcpy(next, entry, queue->entry_sz);
  nvme_bsp_cache_flush ((void *)next, queue->entry_sz);

  /* Increment the tail to indicate the entry added */
  tail = nvme_queue_inc_tail(queue);

  return tail;
}



volatile uint8 *nvme_queue_peek(struct nvme_queue *queue)
{
  volatile uint8 *next;

  if (queue == NULL)
    return NULL;

  if (nvme_queue_is_empty(queue) == 1)
    next = NULL;
  else
    next = queue->base + queue->entry_sz * queue->head;

  return next;
}


int nvme_queue_dequeue(struct nvme_queue *queue, uint8 *entry)
{
  volatile uint8 *next;
  int result = -1;;

  if (queue == NULL)
    return -1;

  next = nvme_queue_peek(queue);
  if (next != NULL)
  {
    if (entry != NULL)
      volatile_memcpy (entry, next, queue->entry_sz);

    queue->head++;
    if (queue->head >= queue->count)
    {
      queue->head = 0;
      queue->phase_tag = 1 - queue->phase_tag; /* 0 --> 1, 1 --> 0 */
    }

    result = 0;
  }

  return result;
}


#define NVME_QUEUE_ENTRY_COUNT    64
#define NVME_NUM_DEV_SUPPORTED    1
#define NVME_SPEC_ENABLE_TIMEOUT_MUL_MS 500 /* NVM spec says timeout is specified in multiples of 500MS*/
#define NVME_MIN_PAGE_SIZE 4096
#define NVME_MAX_QUEUES    2   /* 2 is minimum, 1 Admin and 1 IO. */

enum nvme_queue_index
{
  ADMIN_QUEUE = 0,
  IO_QUEUE = 1,
  MAX_QUEUES = 2,
};


struct nvme_queue_pair
{
  struct nvme_queue subq;
  struct nvme_queue comq;
};

/* Struct to hold all data associated with NVMe. Allows easy debugging by
   dumping this structure. */
struct nvme_device
{
  uint8 in_use:1; /* Is this structure in use or not. */
  uint8 pci_init:1; /* Was the PCIe HW initialized */
  uint8 disable_ctrl:1; /* Was the controller disabled in order to set up the
                           queues */
  uint8 enable_ctrl:1; /* Where the queues set up and requested to enable to
                          controller */
  uint8 identify_ctrl:1; /* Did identify controller run and return success. */
  uint8 init_complete:1; /* NVM is ready for IO (Queues were set up); */
  uint8 reserved:2;
  uint16 cmd_id;
  uint32 bdf; /* PCIe Bus/Device/Function where the NVM is connected. */
  uint32 num_namespace;
  uint64 max_transfer_size_pages; /* 4K memory pages. */
  uint16 dbell_struct_size;/* Doorbell Stride (DSTRD) (size of each doorbell) */
  struct nvme_oacs opt_cmd_sup;/* Optional Admin Command Support (OACS) */
  struct nvme_oncs optional_support; /* Optional NVMe features. */
  struct nvme_fna format_nvm_attr;/* Format NVM Attributes (FNA) */
  struct nvme_error_log err_log;
  struct nvme_queue_pair queue[NVME_MAX_QUEUES];
  struct nvme_controller_registers *ctrl_reg;
  void *scratch_buffer; /* Buffer used for Identify Controller and Identify Namespace*/
};

/* Admin Submit and complete queue buffers. */
volatile static uint8 nvme_adminq_sub_buf[NVME_NUM_DEV_SUPPORTED][NVME_MIN_PAGE_SIZE] __attribute__((aligned(NVME_MIN_PAGE_SIZE)));
volatile static uint8 nvme_adminq_com_buf[NVME_NUM_DEV_SUPPORTED][NVME_MIN_PAGE_SIZE] __attribute__((aligned(NVME_MIN_PAGE_SIZE)));

volatile static uint8 nvme_ioq_sub_buf[NVME_NUM_DEV_SUPPORTED][NVME_MIN_PAGE_SIZE] __attribute__((aligned(NVME_MIN_PAGE_SIZE)));
volatile static uint8 nvme_ioq_com_buf[NVME_NUM_DEV_SUPPORTED][NVME_MIN_PAGE_SIZE] __attribute__((aligned(NVME_MIN_PAGE_SIZE)));

static uint8 nvme_scratch_buffer[NVME_MIN_PAGE_SIZE];


struct nvme_device static_nvme_dev_array[NVME_NUM_DEV_SUPPORTED];

void nvme_log_print_error(struct nvme_error_entry *entry)
{
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "nvme_err:%d(%s:%d)", entry->error, entry->file, entry->line);
  return;
}

void nvme_error_log(struct nvme_device *nvme_dev, uint32 error,
                    const char *file, uint32 line, const char* func)
{
  struct nvme_error_log *log = &nvme_dev->err_log;
  struct nvme_error_entry *entry = &log->errors[log->next_index];
  log->err_count++;
  entry->error = error;
  entry->file = file;
  entry->line = line;
  entry->func = func;

  log->next_index++;
  if (log->next_index > NVME_MAX_ERROR_LOG)
    log->next_index = 0;

  nvme_log_print_error(entry);
}

#define NVME_LOG_ERROR(nvme_dev,error) nvme_error_log(nvme_dev, error, "nvme_api.c", __LINE__, __FUNCTION__)


static inline void nvme_write_register(void* destination, void* source, size_t bytes)
{
  uint32 *dest;
  uint32 *src;
  size_t i;
  /* We should always be writing 32 bits at a time.*/
  ASSERT((bytes % 4) == 0);
  ASSERT(((uint64)destination % 4) == 0);

  dest = (uint32 *)destination;
  src = (uint32 *)source;

  for (i = 0; i < bytes; i +=4)
    *(dest++) = *(src++);
}

static inline void nvme_read_register(void* destination, void* source, size_t bytes)
{
  uint32 *dest;
  uint32 *src;
  size_t i;
  /* We should always be writing 32 bits at a time.*/
  ASSERT((bytes % 4) == 0);
  ASSERT(((uint64)source % 4) == 0);

  dest = (uint32 *)destination;
  src = (uint32 *)source;

  for (i = 0; i < bytes; i +=4)
    *(dest++) = *(src++);
}

uint16 nvme_get_next_cmd_id(struct nvme_device *nvme_dev)
{
  uint16 id = nvme_dev->cmd_id;
  nvme_dev->cmd_id++;
  return id;
}



int nvme_cmd_submit (struct nvme_device *nvme_dev, enum nvme_queue_index qindex,
                    struct nvme_submit_entry *entry)
{
  struct nvme_queue_doorbell doorbell;
  uint8 *dloc;
  int tail;
  struct nvme_queue *subq, *comq;
  subq = &nvme_dev->queue[qindex].subq;
  comq = &nvme_dev->queue[qindex].comq;

  tail = nvme_queue_enqueue(subq, (uint8 *)entry);

  dloc = (uint8 *)&nvme_dev->ctrl_reg->subq_tail_dbl;
  dloc = dloc + (2 * qindex * nvme_dev->dbell_struct_size);
  doorbell.queue = tail;
  doorbell.reserved = 0;
  nvme_write_register(dloc, &doorbell, sizeof(doorbell));

  /* We submitted a command, we expect an entry in the completion queue. Add an
     entry to the completion queue, completion code will check when it is
     populated. */
  tail = nvme_queue_inc_tail(comq);

  return 0;
}

#define NVME_WAIT_COMPLETE_TIMEOUT_MS  5000

int
nvme_cmd_complete (struct nvme_device *nvme_dev, enum nvme_queue_index qindex,
                  struct nvme_complete_entry *out)
{
  volatile struct nvme_complete_entry *entry;
  struct nvme_queue_doorbell doorbell;
  uint8 *dloc;
  int chead, res;
  struct nvme_queue *subq, *comq;
  int timeout_count = 1;
  uint32 poll_wait_us = 10;

  subq = &nvme_dev->queue[qindex].subq;
  comq = &nvme_dev->queue[qindex].comq;

  ASSERT(comq != NULL);
  entry = (volatile struct nvme_complete_entry *)nvme_queue_peek (comq);
  if (entry == NULL)
  {
    return NVME_SW_ECOMQ_EMPTY;
  }

  timeout_count = (NVME_WAIT_COMPLETE_TIMEOUT_MS * 1000) / poll_wait_us;

  nvme_bsp_cache_inval((void *)entry, sizeof(struct nvme_complete_entry));
  while (entry->phase_tag == comq->phase_tag)
  {
    timeout_count--;
    if (timeout_count == 0)
      return NVME_SW_ECOMQ_TIMEOUT;

    nvme_bsp_busy_wait(poll_wait_us);
    nvme_bsp_cache_inval((void *)entry, sizeof(struct nvme_complete_entry));
  }

  if (entry->subq_headp > subq->count)
    return NVME_SW_ECOMQ_INVAL_SUBQ_HEAD;

  /* Remove entries from the submission queue that have already
   * been processed.
   * We currently don't support more than one concurrent command,
   * therefore dequeue the only entry placed in the queue.
   * If more than one command is allowed in the queue, then we need to
   * check the completion entry (device response) to see how far was
   * the submit queue head moved. */
  res = nvme_queue_dequeue(subq, NULL);
  if (res < 0)
    return NVME_SW_ECOMQ_RANGE_SUBQ_HEAD;

  nvme_queue_dequeue(comq, (uint8 *)out);
  chead = nvme_queue_head_index(comq);

  dloc = (uint8 *)&nvme_dev->ctrl_reg->subq_tail_dbl;
  dloc = dloc + (((2 * qindex) + 1) * nvme_dev->dbell_struct_size);
  doorbell.queue = chead;
  doorbell.reserved = 0;
  nvme_write_register(dloc, &doorbell, sizeof(doorbell));

  return 0;
}

int nvme_convert_status_code(struct nvme_complete_entry *com_entry)
{
  uint8 status_code = com_entry->status_code;

  switch (com_entry->status_code_type)
  {
  case 0: /* Figure 30: Generic Command Status */
    if (status_code == 0)
      return 0;
    else
      return NVME_DEVICE_GENERIC_STATUS_CODE(status_code);

  case 1: /* Command Specific Status */
    return NVME_DEVICE_CMD_STATUS_CODE(status_code);

  case 2: /* Media and Data Integrity Errors */
    return NVME_DEVICE_DATA_STATUS_CODE(status_code);

  case 7: /* Vendor Specific. */
    return NVME_DEVICE_VENDOR_STATUS_CODE(status_code);

  default:
    return NVME_DEVICE_RESERVED_STATUS_CODE (status_code);
  }

  return NVME_DEVICE_RESERVED_STATUS_CODE (status_code);
}

int nvme_submit_complete_single(struct nvme_device *nvme_dev,
                               enum nvme_queue_index qindex,
                               struct nvme_submit_entry *entry,
                               struct nvme_complete_entry *com_entry)
{
  int result;
  /* Add to the submit queue and ring the doorbell. */
  result = nvme_cmd_submit (nvme_dev, qindex, entry);
  if (result == 0)
    result = nvme_cmd_complete(nvme_dev, qindex, com_entry);

  if (result == 0)
    result = nvme_convert_status_code(com_entry);

  return result;
}

/* Build a PRP list if necessary.

  param nvme_dev  Used to keep track of the command ID between commands.
  param entry     Submit Entry command to populate.
  param buffer    Buffer used for the transfer.
  param size      Size of the buffer.

*/
static int
nvme_prp_setup (struct nvme_device *nvme_dev, struct nvme_submit_entry *entry,
                void *buffer, size_t size)
{
  uint64 offset, page_sz;
  uint64 prp_next;
  page_sz = NVME_MIN_PAGE_SIZE;

  entry->cmd.psdt = NVME_ADMIN_CMD_PRP_USED; /* PRPs are used for this transfer. */
  entry->prp1 = (uint64)buffer;
  prp_next = (entry->prp1 + page_sz) & ~(page_sz - 1);

  offset = entry->prp1 & (page_sz - 1);
  offset += size;
  if (offset > page_sz * 2)
  {
    uint64 *prp_list = (uint64 *)nvme_dev->scratch_buffer;
    uint32 entries_per_page = page_sz / sizeof(uint64);
    size_t size2 = offset - page_sz; /* Remove the page already in PRP1 */
    uint32 num_entries = (size2 + page_sz - 1) / page_sz; /* Round up to the next page boundary*/
    if (num_entries > entries_per_page)
    {
      /* We yet don't support building a PRP list larger than 1 page.*/
      NVME_LOG_ERROR(nvme_dev, page_sz * 2);
      ASSERT(0);
      return -1;
    }
    else
    {
      uint32 i;
      memset(prp_list, 0, page_sz);
      for (i = 0; i < num_entries; i++)
      {
        *(prp_list + i) = prp_next;
        prp_next += page_sz;
      }
      entry->prp2 = (uint64)prp_list;
      nvme_bsp_cache_flush (prp_list, page_sz);
    }
  }
  else if (offset > page_sz)
  {/* The buffer uses two pages, populate prp2. */
    entry->prp2 = prp_next;
  }
  return 0;
}

/* Get the next entry in the submit queue, pre-populating certain fields.

  param nvme_dev  Used to keep track of the command ID between commands.
  param opcode    Opcode of the command.
  param namespace Namespace for the command.
  param buffer    Buffer used for the transfer.
  param size      Size of the buffer.

*/
static int
nvme_cmd_setup (struct nvme_device *nvme_dev, struct nvme_submit_entry *entry,
               uint8 opcode, uint32 namespace, void *buffer, size_t size)
{
  int result = 0;

  if (entry == NULL)
    return -1;

  memset(entry, 0, sizeof(struct nvme_submit_entry));
  entry->cmd.opcode = opcode;
  entry->cmd.fused = NVME_ADMIN_CMD_NOT_FUSED; /* Normal Operation. */
  entry->cmd.cmd_id = nvme_get_next_cmd_id(nvme_dev);
  entry->namespace_id = namespace;

  result = nvme_prp_setup(nvme_dev, entry, buffer, size);

  return result;
}


#define NVME_CMD_IDENTIFY_NAMESPACE_CNS      (0)
#define NVME_CMD_IDENTIFY_CONTROLLER_CNS     (1)
#define NVME_CMD_IDENTIFY_LIST_NAMESPACE_CNS (2)
/* Figure 108. */
PACKED struct nvme_identify_cdw10
{
  uint8  cns;      /* Controller or Namespace Structure (CNS) */
  uint8  reserved; /* Reserved */
  uint16 controller_id; /* Controller Identifier (CNTID) */
}PACKED_POST;
/*
  param  namespace  Namespace to identify, if 0 returns identify controller
*/
int nvme_cmd_identify_controller (struct nvme_device *nvme_dev,
                                  struct nvme_identify_controller *ic)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  int result = -1;
  struct nvme_identify_cdw10 *cdw10;
  uint32 namespace = 0;

  memset(ic, 0, sizeof(struct nvme_identify_controller));
  nvme_bsp_cache_flush (ic, sizeof(struct nvme_identify_controller));
  result = nvme_cmd_setup(nvme_dev, entry,
                          NVME_ADMIN_CMD_OPCODE_IDENTIFY,
                          namespace, ic,
                          sizeof(struct nvme_identify_controller));
  if (result != 0)
  {
    goto cleanup;
  }

  cdw10 = (struct nvme_identify_cdw10 *)&entry->cdw10;
  cdw10->cns = NVME_CMD_IDENTIFY_CONTROLLER_CNS;
  cdw10->controller_id = 0;

  result = nvme_submit_complete_single(nvme_dev, ADMIN_QUEUE, entry, &com_entry);
  nvme_bsp_cache_inval (ic, sizeof(struct nvme_identify_controller));

cleanup:
  return result;
}

int nvme_cmd_identify_namespace (struct nvme_device *nvme_dev,
                                struct nvme_identify_namespace *ins,
                                uint32 namespace)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  int result = -1;
  struct nvme_identify_cdw10 *cdw10;

  memset(ins, 0, sizeof(struct nvme_identify_namespace));
  nvme_bsp_cache_flush (ins, sizeof(struct nvme_identify_namespace));

  result = nvme_cmd_setup(nvme_dev, entry,
                        NVME_ADMIN_CMD_OPCODE_IDENTIFY,
                        namespace, ins,
                        sizeof(struct nvme_identify_namespace));
  if (result != 0)
  {
    goto cleanup;
  }

  cdw10 = (struct nvme_identify_cdw10 *)&entry->cdw10;
  cdw10->cns = NVME_CMD_IDENTIFY_NAMESPACE_CNS;
  cdw10->controller_id = 0;

  result = nvme_submit_complete_single(nvme_dev, ADMIN_QUEUE, entry, &com_entry);
  nvme_bsp_cache_inval (ins, sizeof(struct nvme_identify_namespace));

cleanup:
  return result;
}


/* Figure 52. */
PACKED struct nvme_create_io_completion_queue_cdw10
{
  uint16 qid;   /* Queue Identifier */
  uint16 qsize; /* Queue Size */
}PACKED_POST;
/* Figure 53 */
PACKED struct nvme_create_io_completion_queue_cdw11
{
  uint16 pcontiguous:1;  /* Physically Contiguous (PC) */
  uint16 inter_enable:1; /* Interrupts Enabled (IEN) */
  uint16 reserved:14;    /* Interrupts Enabled (IEN) */
  uint16 inter_vector;   /* Interrupt Vector (IV) */
}PACKED_POST;

static int
nvme_cmd_create_io_completion_queue (struct nvme_device *nvme_dev,
                                   void *queue_buffer, size_t queue_size)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  int result = -1;
  struct nvme_create_io_completion_queue_cdw10 *cdw10;
  struct nvme_create_io_completion_queue_cdw11 *cdw11;
  uint32 namespace = 0;

  result = nvme_cmd_setup(nvme_dev, entry,
                         NVME_ADMIN_CMD_OPCODE_CREATE_IO_COMPLETION_QUEUE,
                         namespace, queue_buffer, queue_size);
  if (result != 0)
  {
    goto cleanup;
  }

  cdw10 = (struct nvme_create_io_completion_queue_cdw10 *)&entry->cdw10;
  cdw10->qid = 1;
  cdw10->qsize = nvme_queue_size_in_entries(&nvme_dev->queue[IO_QUEUE].comq) - 1;
  cdw11 = (struct nvme_create_io_completion_queue_cdw11 *)&entry->cdw11;
  cdw11->pcontiguous = 1;
  cdw11->inter_enable = 0;
  cdw11->inter_vector = 0;
  cdw11->reserved = 0;

  result = nvme_submit_complete_single(nvme_dev, ADMIN_QUEUE, entry, &com_entry);

cleanup:
  return result;
}

/* Figure 56. */
PACKED struct nvme_create_io_submission_queue_cdw10
{
  uint16 qid;   /* Queue Identifier */
  uint16 qsize; /* Queue Size */
}PACKED_POST;
/* Figure 57 */
PACKED struct nvme_create_io_submission_queue_cdw11
{
  uint16 pcontiguous:1;     /* Physically Contiguous (PC) */
  uint16 queue_prio:2;      /* Queue Priority (QPRIO) */
  uint16 reserved:13;       /* Reserved */
  uint16 complete_queue_id; /* Completion Queue Identifier (CQID) */
}PACKED_POST;

static int
nvme_cmd_create_io_submission_queue (struct nvme_device *nvme_dev,
                                    void *queue_buffer, size_t queue_size)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  int result = -1;
  struct nvme_create_io_submission_queue_cdw10 *cdw10;
  struct nvme_create_io_submission_queue_cdw11 *cdw11;
  uint32 namespace = 0;

  result = nvme_cmd_setup(nvme_dev, entry,
                         NVME_ADMIN_CMD_OPCODE_CREATE_IO_SUBMISSION_QUEUE,
                         namespace, queue_buffer, queue_size);
  if (result != 0)
  {
    goto cleanup;
  }

  cdw10 = (struct nvme_create_io_submission_queue_cdw10 *)&entry->cdw10;
  cdw10->qid = 1;
  cdw10->qsize = nvme_queue_size_in_entries(&nvme_dev->queue[IO_QUEUE].subq) - 1;
  cdw11 = (struct nvme_create_io_submission_queue_cdw11 *)&entry->cdw11;
  cdw11->pcontiguous = 1;
  cdw11->queue_prio = 0;
  cdw11->complete_queue_id = 1;
  cdw11->reserved = 0;

  result = nvme_submit_complete_single(nvme_dev, ADMIN_QUEUE, entry, &com_entry);

cleanup:
  return result;
}


void nvme_dev_struct_init (struct nvme_device *nvme_dev, size_t index)
{
  uint16 count = NVME_QUEUE_ENTRY_COUNT; /* Admin queue and IO queue don't need to
                                           have the same size. */

  memset(nvme_dev, 0, sizeof(struct nvme_device));

  /* Hook up the static arrays to our structure. */
  nvme_queue_init(&nvme_dev->queue[ADMIN_QUEUE].subq, &nvme_adminq_sub_buf[index][0], count,
                 sizeof(struct nvme_submit_entry));
  nvme_queue_init(&nvme_dev->queue[ADMIN_QUEUE].comq, &nvme_adminq_com_buf[index][0],
                 count, sizeof(struct nvme_complete_entry));

  nvme_queue_init(&nvme_dev->queue[IO_QUEUE].subq, &nvme_ioq_sub_buf[index][0], count,
                 sizeof(struct nvme_submit_entry));
  nvme_queue_init(&nvme_dev->queue[IO_QUEUE].comq, &nvme_ioq_com_buf[index][0],
                 count, sizeof(struct nvme_complete_entry));

  nvme_dev->scratch_buffer = &nvme_scratch_buffer[0];

  /* Make sure the last 12 bits are zero for the base address. */
  ASSERT(((uint64)(nvme_dev->queue[ADMIN_QUEUE].subq.base) & 0xfff) == 0x0);
  ASSERT(((uint64)(nvme_dev->queue[ADMIN_QUEUE].comq.base) & 0xfff) == 0x0);
  ASSERT(count <= NVME_MIN_PAGE_SIZE / sizeof(struct nvme_submit_entry));
  ASSERT(count <= NVME_MIN_PAGE_SIZE / sizeof(struct nvme_complete_entry));
  ASSERT(((uint64)(nvme_dev->queue[IO_QUEUE].subq.base) & 0xfff) == 0x0);
  ASSERT(((uint64)(nvme_dev->queue[IO_QUEUE].comq.base) & 0xfff) == 0x0);
}

struct nvme_handle
{
  uint8 in_use;
  struct nvme_device *nvme_dev;
  uint64 namespace;
  uint64 last_lba;
  uint64 block_size;
};

struct nvme_handle static_handle[1];

static uint32 nvme_init(struct nvme_device *nvme_dev)
{
  int result = 0;
  struct nvme_controller_registers *ctrl_reg;
  uint64 timeout_ms = NVME_SPEC_ENABLE_TIMEOUT_MUL_MS; /* 500 ms */
  uint8 timeout_mul;
  uint64 timeout_ms_total;
  int port = 0;
  struct nvme_capabilities cap;
  struct nvme_controller_configuration cc = {0};

  cc.cmd_set = 0;     /* NVM Command Set. */
  cc.mem_page_size = 0; /* Host 4K page size supported. (2^(mem_page_size+12)) */
  cc.arb_selected = 0; /* Round Robin. */
  cc.subq_entry_size = 6; /* 6 is the minimum value given by the
                             spec in Identfy Controller (SQES) */
  cc.comq_entry_size = 4; /* 4 is the minimum value given by the
                             spec in Identfy Controller (CQES) */

  /* Some simple checks to verify sizes are correct. */
  ASSERT (sizeof(struct nvme_identify_controller) == 4096);
  ASSERT (sizeof(struct nvme_identify_namespace) == 4096);
  /* The code has an static buffer of MIN_PAGE_SIZE to hold the identify
     temporally structures */
  ASSERT (sizeof(struct nvme_identify_controller) <= NVME_MIN_PAGE_SIZE);

  if (nvme_dev == NULL)
    return NVME_SW_EINVAL_PARAM;

  if (nvme_dev->in_use == 0)
    nvme_dev_struct_init (nvme_dev, port);

  nvme_dev->in_use = 1;

  if (nvme_dev->init_complete == 1)
    return NVME_SUCCESS;

  if (nvme_dev->pci_init == 0)
  {
    result = nvme_core_target_power_on(port);
    if (result != 0)
    {
      result = NVME_HW_ENVME_PWR;
      goto done;
    }

    /* DO PCIe Initialization */
    nvme_dev->ctrl_reg = (struct nvme_controller_registers *)pcie_get_mlbar(nvme_dev->bdf);
    if (nvme_dev->ctrl_reg == NULL)
    {
      result = NVME_HW_ENVME_PCIE;
      goto done;
    }
    nvme_dev->pci_init = 1;
  }

  ctrl_reg = nvme_dev->ctrl_reg;

  nvme_read_register(&cap, &ctrl_reg->cap, sizeof(cap));

  /* Check the capabilities/command set supported to verify it is NVMe */
  if (cap.cmd_set_sup == 0)
  {
    result = NVME_HW_ENOT_NVME;
    goto done;
  }

  timeout_mul = cap.timeout;
  timeout_ms_total = (NVME_SPEC_ENABLE_TIMEOUT_MUL_MS * timeout_mul);
  nvme_dev->dbell_struct_size = (4 << cap.doorbell_stride);

  /* Disable NVM to setup the Admin Queues. */
  if (nvme_dev->disable_ctrl == 0)
  {
    cc.enable = 0;
    nvme_write_register(&ctrl_reg->cc, &cc, sizeof(cc));
    nvme_dev->disable_ctrl = 1;
  }

  if (nvme_dev->enable_ctrl == 0)
  {
    struct nvme_controller_status csts;
    struct nvme_admin_queue_attr aqa;
    /* Wait for the NVMe device controller to set ready to zero. */
    timeout_ms = timeout_ms_total;
    nvme_read_register(&csts, &ctrl_reg->csts, sizeof(csts));
    while(csts.ready != 0)
    {
      uint32 wait_us = 1000;
      if (timeout_ms == 0)
      {
        result = NVME_HW_ETIMEOUT_DISABLE;
        goto done;
      }
      busywait(wait_us);
      timeout_ms--;
      nvme_read_register(&csts, &ctrl_reg->csts, sizeof(csts));
    }

    /* Setup the Admin submit and complete queues. */
    nvme_write_register (&ctrl_reg->admin_comq_base,
                         &nvme_dev->queue[ADMIN_QUEUE].comq.base,
                         sizeof(uint64));
    nvme_write_register (&ctrl_reg->admin_subq_base,
                         &nvme_dev->queue[ADMIN_QUEUE].subq.base,
                         sizeof(uint64));

    aqa.complete_queue_sz = nvme_dev->queue[ADMIN_QUEUE].comq.count;
    aqa.submit_queue_sz = nvme_dev->queue[ADMIN_QUEUE].subq.count;
    nvme_write_register (&ctrl_reg->aqa, &aqa, sizeof(aqa));
    /* Things have been setup, enable the controller */
    cc.enable = 1;
    nvme_write_register(&ctrl_reg->cc, &cc, sizeof(cc));

    nvme_dev->enable_ctrl = 1;
  }

  if (nvme_dev->identify_ctrl == 0)
  {
    struct nvme_identify_controller *ic;
    struct nvme_controller_status csts;
    /* Wait for the NVMe device controller to set ready to one. */
    timeout_ms = timeout_ms_total;
    nvme_read_register(&csts, &ctrl_reg->csts, sizeof(csts));
    while(csts.ready == 0)
    {
      uint32 wait_us = 1000;
      if (timeout_ms == 0)
      {
        result = NVME_HW_ETIMEOUT_ENABLE;
        goto done;
      }
      busywait(wait_us);
      timeout_ms--;
      nvme_read_register(&csts, &ctrl_reg->csts, sizeof(csts));
    }

    ic = (struct nvme_identify_controller *) nvme_dev->scratch_buffer;
    result = nvme_cmd_identify_controller(nvme_dev, ic);
    if (result != NVME_SUCCESS)
    {
      goto done;
    }
    else if (ic->vendor_id == 0)
    {
      /* During initial driver bringup there were instances when identify
         controller returned success, but the structure was all zeros.
         This is a simple check to make sure something was updated. */

      result = NVME_HW_ENVME_PCIE;
      goto done;
    }
    else /* NVME_SUCCESS!*/
    {
      uint8 mdts = ic->max_data_trans_size;
      /* If 0 there is no limit.*/
      if (mdts == 0)
        nvme_dev->max_transfer_size_pages = 0x10000000;
      /* Check overflow*/
      else if(mdts > (sizeof(nvme_dev->max_transfer_size_pages) * 8))
        nvme_dev->max_transfer_size_pages = 0x10000000;
      else
        nvme_dev->max_transfer_size_pages = (1 << mdts);

      if (cap.mem_pagesz_min != 0)
      {
        /* Compute the memory (DDR) page size in units of 4k chunks*/
        uint64 mem_ps = (1 << (1 + cap.mem_pagesz_min));
        nvme_dev->max_transfer_size_pages *= mem_ps;
      }
      nvme_dev->num_namespace = ic->num_namespace;
      nvme_dev->optional_support = ic->opt_nvm_cmd;
      nvme_dev->opt_cmd_sup = ic->opt_cmd_sup;
      nvme_dev->format_nvm_attr = ic->format_nvm_attr;
      nvme_dev->identify_ctrl = 1;
    }

  }

  if (nvme_dev->init_complete == 0)
  {
    /* Create one IO Completion queue */
    result = nvme_cmd_create_io_completion_queue (nvme_dev,
              (uint8 *)nvme_dev->queue[IO_QUEUE].comq.base, NVME_MIN_PAGE_SIZE);
    if (result != NVME_SUCCESS)
      goto done;

    /* Create one IO Submission queue */
    result = nvme_cmd_create_io_submission_queue (nvme_dev,
              (uint8 *)nvme_dev->queue[IO_QUEUE].subq.base, NVME_MIN_PAGE_SIZE);
    if (result != NVME_SUCCESS)
      goto done;


    nvme_dev->init_complete = 1;
  }
  result = NVME_SUCCESS;

  /* Test code */
  /* memset (&identify_contrl2, 0, sizeof(identify_contrl2)); */
  /* nvme_cmd_identify_controller (nvme_dev, &identify_contrl2); */

  /* nvme_cmd_identify_namespace (nvme_dev, &identify_ns, 1); */

  /* static_handle[0].nvme_dev = nvme_dev; */
  /* static_handle[0].namespace = 1; */

  /* { */
  /*   static uint8 buffer_in[4096]; */
  /*   static uint8 buffer_out[4096]; */
  /*   size_t i; */
  /*   for(i = 0; i < sizeof(buffer_in); i++) */
  /*     buffer_in[i] = 0xFF - (0xFF & i); */
  /*   nvme_write(&static_handle[0], buffer_in, 0, 1); */
  /*   memset(buffer_out, 0, sizeof(buffer_out)); */
  /*   nvme_read(&static_handle[0], buffer_out, 0, 1); */
  /* } */

  /* end test code */

done:
  return result;
}


PACKED struct nvme_format_cdw10
{
  uint32 lba_format:4; /* Index to LBA format to use in Identify Namespace. */
  uint32 metadata_settings:1; /* Should be set to 0. Driver does not handle metadata. */
  uint32 protection_info:3; /* Sould be set to 0. Driver does not support end-to-end data protection. */
  uint32 protection_info_loc:1; /* Sould be set to 0. Driver does not support end-to-end data protection. */
  uint32 secure_erase_setting:3; /* Do secure erase or not (Figure 176) */
  uint32 reserved:20;
}PACKED_POST;

uint32 nvme_cmd_format (struct nvme_device *nvme_dev,
                        uint32 namespace,
                        uint32 lba_index)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  int result = -1;
  struct nvme_format_cdw10 *cdw10;

  result = nvme_cmd_setup(nvme_dev, entry,
                          NVME_ADMIN_CMD_OPCODE_FORMAT_NVM,
                          namespace, NULL, 0);
  if (result != 0)
  {
    goto cleanup;
  }

  cdw10 = (struct nvme_format_cdw10 *)&entry->cdw10;
  memset(cdw10, 0, sizeof(struct nvme_format_cdw10));
  cdw10->lba_format = lba_index;

  result = nvme_submit_complete_single(nvme_dev, ADMIN_QUEUE, entry, &com_entry);

cleanup:
  return result;
}

uint32 nvme_format (struct nvme_handle *handle,
                    uint32 lba_size)
{
  uint32 result;
  struct nvme_identify_namespace *ins;
  struct nvme_device *nvme_d;

  if (handle == NULL || lba_size < 512 )
    return NVME_SW_EINVAL_PARAM;

  if (handle->in_use == 0 || handle->nvme_dev == NULL)
    return NVME_SW_EBAD_HANDLE;

  nvme_d = handle->nvme_dev;

  if (nvme_d->opt_cmd_sup.format == 0)
    return NVME_HW_ENOT_SUPPORTED;

  if (nvme_d->format_nvm_attr.global_format == 1)
    return NVME_HW_ENOT_SUPPORTED;

  ins = (struct nvme_identify_namespace *) nvme_d->scratch_buffer;
  result = nvme_cmd_identify_namespace(nvme_d, ins, handle->namespace);
  if (result == NVME_SUCCESS)
  {
    size_t max_index = ins->num_lba_formats + 1;
    size_t max_entries = sizeof(ins->lbaf)/sizeof(struct nvme_lba_format);
    size_t i, format_index = max_entries;

    if(max_index > max_entries)
      return NVME_HW_EBAD_NS_INFO;

    for (i = 0; i < max_index; i++)
    {
      uint32 entry_lba_size = 1 << ins->lbaf[i].lba_size_shift;
      if (lba_size == entry_lba_size)
      {
        format_index = i;
        break;
      }
    }
    if (format_index == max_entries)
      return NVME_SW_EINVAL_LBA;

    result = nvme_cmd_format(nvme_d, handle->namespace, format_index);
  }

  return result;
}

static struct nvme_device *nvme_get_device_handle (uint32 port, uint32 *error)
{
  struct nvme_device *nvme_dev = NULL;
  uint32 result = NVME_SW_EINVAL_PARAM;
  /* We don't support multiple devices. */
  if (port != 0)
  {
    goto done;
  }

  nvme_dev = &static_nvme_dev_array[port];
  if (nvme_dev->init_complete == 0)
  {
    result = nvme_init(nvme_dev);
    if (result != NVME_SUCCESS)
    {
      nvme_dev = NULL;
    }
  }
done:
  *error = result;
  return nvme_dev;
}


/* Figure 213. */
PACKED struct nvme_read_cdw12
{
  uint16 nlb;   /* Number of Logical Blocks (NLB) */
  uint16 reserved:10;
  uint16 prinfo:4; /* Protection Information Field (PRINFO) */
  uint16 fua:1; /* Force Unit Access (FUA) */
  uint16 limited_retry:1; /* Limited Retry (LR) */
}PACKED_POST;

static uint32
nvme_read_sectors(struct nvme_device *nvme_dev,
                  void            *buff,
                  uint32          namespace,
                  uint64          lba,
                  uint32          num_blocks,
                  uint32          block_size)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  struct nvme_read_cdw12 *cdw12;
  int result = -1;

  /* This command implementation supports only up to 2^16 blocks to be flashed
   * at once*/
  if (num_blocks > NVME_IO_MAX_BLOCK_COUNT)
  {
    result = NVME_SW_EINVAL_PARAM;
    goto cleanup;
  }

  result = nvme_cmd_setup(nvme_dev, entry,
                         NVME_IO_CMD_READ,
                         namespace, buff,
                         num_blocks * block_size);
  if (result != 0)
  {
    goto cleanup;
  }

  entry->cdw10 = (uint32)lba;
  entry->cdw11 = (uint32)(lba >> 32);
  cdw12 = (struct nvme_read_cdw12 *)&entry->cdw12;
  memset(cdw12, 0, sizeof(struct nvme_read_cdw12));
  cdw12->nlb = num_blocks - 1; /* Zero based value, nlb=0 means read 1 block.*/

  result = nvme_submit_complete_single(nvme_dev, IO_QUEUE, entry, &com_entry);

cleanup:
  return result;
}

uint32 nvme_read(struct nvme_handle *handle,
                 void               *buff,
                 uint64             lba,
                 uint64             num_blocks)
{
  uint64 last_lba, namespace;
  uint64 remain = num_blocks;
  uint64 curr_lba = lba;
  uint32 count, result;
  uint8 *curr_buf = (uint8 *)buff;
  struct nvme_device *nvme_dev;
  uint64 max_io_blocks, max_transfer_blocks, blocks_per_page;
  uint64 max_prp_entries = (NVME_MIN_PAGE_SIZE / (sizeof (uint64)));
  uint64 max_prp_blocks;
  /* The driver currently only supports prp entries from IO commands (no SGL yet
     since not all devices support SGL. max_prp_blocks is the maximum number of
     blocks we can do IO using the current driver limit for prp list. */

  if (handle == NULL)
    return NVME_SW_EINVAL_PARAM;

  if (handle->in_use == 0)
    return NVME_SW_EBAD_HANDLE;

  last_lba = lba + num_blocks;

  /* Check for wrap around. */
  if (last_lba <= lba)
    return NVME_SW_EINVAL_PARAM;

  if (last_lba > handle->last_lba)
    return NVME_SW_EINVAL_LBA;

  nvme_dev = handle->nvme_dev;
  namespace = handle->namespace;

  blocks_per_page = (NVME_MIN_PAGE_SIZE / handle->block_size);
  max_prp_blocks = max_prp_entries * blocks_per_page;
  max_transfer_blocks = nvme_dev->max_transfer_size_pages * blocks_per_page;

  max_io_blocks = (NVME_IO_MAX_BLOCK_COUNT < max_prp_blocks) ? NVME_IO_MAX_BLOCK_COUNT : max_prp_blocks;
  max_io_blocks = (max_io_blocks < max_transfer_blocks) ? max_io_blocks : max_transfer_blocks;

  do
  {
    count = (remain < max_io_blocks) ? remain : max_io_blocks;

    nvme_bsp_cache_inval (curr_buf, count * handle->block_size);
    result = nvme_read_sectors(nvme_dev, curr_buf, namespace, curr_lba, count, handle->block_size);
    if (result != NVME_SUCCESS)
      return result;
    curr_lba += count;
    remain -= count;
    curr_buf += count * handle->block_size;
  } while (remain > 0);

  return NVME_SUCCESS;
}


/* Figure 238. */
PACKED struct nvme_write_cdw12
{
  uint16 nlb;   /* Number of Logical Blocks (NLB) */
  uint16 reserved:4;
  uint16 dtype:4;  /* Directive Type (DTYPE) */
  uint16 reserved2:2;
  uint16 prinfo:4; /* Protection Information Field (PRINFO) */
  uint16 fua:1; /* Force Unit Access (FUA) */
  uint16 limited_retry:1; /* Limited Retry (LR) */
}PACKED_POST;

static uint32
nvme_write_sectors(struct nvme_device *nvme_dev,
                   void            *buff,
                   uint32          namespace,
                   uint64          lba,
                   uint32          num_blocks,
                   uint32          block_size)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  struct nvme_write_cdw12 *cdw12;
  int result = -1;

  /* This command implementation supports only up to 2^16 blocks to be flashed
   * at once*/
  if (num_blocks > NVME_IO_MAX_BLOCK_COUNT)
  {
    result = NVME_SW_EINVAL_PARAM;
    goto cleanup;
  }

  result = nvme_cmd_setup(nvme_dev, entry,
                         NVME_IO_CMD_WRITE,
                         namespace, buff,
                         num_blocks * block_size);
  if (result != 0)
  {
    goto cleanup;
  }

  entry->cdw10 = (uint32)lba;
  entry->cdw11 = (uint32)(lba >> 32);
  cdw12 = (struct nvme_write_cdw12 *)&entry->cdw12;
  memset(cdw12, 0, sizeof(struct nvme_write_cdw12));
  cdw12->nlb = num_blocks - 1; /* Zero based value, nlb=0 means read 1 block.*/

  result = nvme_submit_complete_single(nvme_dev, IO_QUEUE, entry, &com_entry);

cleanup:
  return result;
}

uint32 nvme_write(struct nvme_handle *handle,
                  void               *buff,
                  uint64             lba,
                  uint64             num_blocks)
{
  uint64 last_lba, namespace;
  uint64 remain = num_blocks;
  uint64 curr_lba = lba;
  uint32 count, result;
  uint8 *curr_buf = (uint8 *)buff;
  struct nvme_device *nvme_dev;
  uint64 max_io_blocks, max_transfer_blocks, blocks_per_page;
  uint64 max_prp_entries = (NVME_MIN_PAGE_SIZE / (sizeof (uint64)));
  uint64 max_prp_blocks;
  /* The driver currently only supports prp entries from IO commands (no SGL yet
     since not all devices support SGL. max_prp_blocks is the maximum number of
     blocks we can do IO using the current driver limit for prp list. */

  if (handle == NULL)
    return NVME_SW_EINVAL_PARAM;

  if (handle->in_use == 0)
    return NVME_SW_EBAD_HANDLE;

  last_lba = lba + num_blocks;

  /* Check for wrap around. */
  if (last_lba <= lba)
    return NVME_SW_EINVAL_PARAM;

  if (last_lba > handle->last_lba)
    return NVME_SW_EINVAL_LBA;

  nvme_bsp_cache_flush (buff, num_blocks * handle->block_size);

  nvme_dev = handle->nvme_dev;
  namespace = handle->namespace;

  blocks_per_page = (NVME_MIN_PAGE_SIZE / handle->block_size);
  max_prp_blocks = max_prp_entries * blocks_per_page;
  max_transfer_blocks = nvme_dev->max_transfer_size_pages * blocks_per_page;

  max_io_blocks = (NVME_IO_MAX_BLOCK_COUNT < max_prp_blocks) ? NVME_IO_MAX_BLOCK_COUNT : max_prp_blocks;
  max_io_blocks = (max_io_blocks < max_transfer_blocks) ? max_io_blocks : max_transfer_blocks;

  do
  {
    count = (remain < max_io_blocks) ? remain : max_io_blocks;
    result = nvme_write_sectors(nvme_dev, curr_buf, namespace,
                                curr_lba, count, handle->block_size);
    if (result != NVME_SUCCESS)
      return result;
    curr_lba += count;
    remain -= count;
    curr_buf += count * handle->block_size;
  } while (remain > 0);

  return NVME_SUCCESS;
}

static uint32
nvme_flush_cmd(struct nvme_device *nvme_dev,
               uint32              namespace)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  int result = -1;

  result = nvme_cmd_setup(nvme_dev, entry,
                          NVME_IO_CMD_FLUSH,
                          namespace, NULL,
                          0);
  if (result != 0)
  {
    goto cleanup;
  }

  result = nvme_submit_complete_single(nvme_dev, IO_QUEUE, entry, &com_entry);

cleanup:
  return result;
}

uint32 nvme_flush(struct nvme_handle *handle)
{
  struct nvme_device *nvme_dev;
  uint64 namespace;

  if (handle == NULL)
    return NVME_SW_EINVAL_PARAM;

  if (handle->in_use == 0)
    return NVME_SW_EBAD_HANDLE;

  nvme_dev = handle->nvme_dev;
  namespace = handle->namespace;

  return nvme_flush_cmd(nvme_dev, namespace);
}

/* Figure 247 */
PACKED struct nvme_write_zeroes_cdw12
{
  uint16 nlb;   /* Number of Logical Blocks (NLB) */
  uint16 reserved:9;
  uint16 deallocate:1;  /* Deallocate (DEAC) */
  uint16 prinfo:4; /* Protection Information Field (PRINFO) */
  uint16 fua:1; /* Force Unit Access (FUA) */
  uint16 limited_retry:1; /* Limited Retry (LR) */
}PACKED_POST;

static uint32
nvme_write_zeroes_cmd(struct nvme_device *nvme_dev,
                      uint32              namespace,
                      uint64              lba,
                      uint32              num_blocks)
{
  struct nvme_submit_entry entry_data;
  struct nvme_submit_entry *entry = &entry_data;
  struct nvme_complete_entry com_entry;
  struct nvme_write_zeroes_cdw12 *cdw12;
  int result = -1;

  if (num_blocks > NVME_IO_MAX_BLOCK_COUNT)
  {
    result = NVME_SW_EINVAL_PARAM;
    goto cleanup;
  }

  result = nvme_cmd_setup(nvme_dev, entry,
                          NVME_IO_CMD_WRITE_ZEROES,
                          namespace, NULL,
                          0);
  if (result != 0)
  {
    goto cleanup;
  }

  entry->cdw10 = (uint32)lba;
  entry->cdw11 = (uint32)(lba >> 32);
  cdw12 = (struct nvme_write_zeroes_cdw12 *)&entry->cdw12;
  memset(cdw12, 0, sizeof(struct nvme_write_zeroes_cdw12));
  cdw12->nlb = num_blocks - 1; /* Zero based value, nlb=0 means read 1 block.*/

  result = nvme_submit_complete_single(nvme_dev, IO_QUEUE, entry, &com_entry);

cleanup:
  return result;
}

uint32 nvme_write_zeroes(struct nvme_handle *handle,
                         uint64             lba,
                         uint64             num_blocks)
{
  uint64 last_lba, namespace;
  uint64 remain = num_blocks;
  uint64 curr_lba = lba;
  uint32 count, result;
  struct nvme_device *nvme_dev;
  uint64 max_io_blocks;

  if (handle == NULL)
    return NVME_SW_EINVAL_PARAM;

  if (handle->in_use == 0)
    return NVME_SW_EBAD_HANDLE;

  last_lba = lba + num_blocks;

  /* Check for wrap around. */
  if (last_lba <= lba)
    return NVME_SW_EINVAL_PARAM;

  if (last_lba > handle->last_lba)
    return NVME_SW_EINVAL_LBA;

  nvme_dev = handle->nvme_dev;
  namespace = handle->namespace;

  if (nvme_dev->optional_support.write_zeroes == 0)
    return NVME_HW_ENOT_SUPPORTED;

  max_io_blocks = NVME_IO_MAX_BLOCK_COUNT;
  do
  {
    count = (remain < max_io_blocks) ? remain : max_io_blocks;
    result = nvme_write_zeroes_cmd(nvme_dev, namespace,
                                   curr_lba, count);
    if (result != NVME_SUCCESS)
      return result;
    curr_lba += count;
    remain -= count;
  } while (remain > 0);

  return NVME_SUCCESS;
}

uint32 nvme_get_device_info (uint32 id,
                             struct nvme_device_info *info)
{
  uint32 result, error;
  struct nvme_device *nvme_d;
  struct nvme_identify_controller *ic;

  if (info == NULL)
    return NVME_SW_EINVAL_PARAM;

  nvme_d = nvme_get_device_handle (id, &error);
  if (nvme_d == NULL)
    return error;

  ic = (struct nvme_identify_controller *) nvme_d->scratch_buffer;
  result = nvme_cmd_identify_controller(nvme_d, ic);
  if (result == NVME_SUCCESS)
  {
    info->vendor_id = ic->vendor_id;
    info->subsys_vid = ic->subsys_vid;
    memcpy(info->serial, ic->serial, 20);
    memcpy(info->model_num, ic->model_num, 40);
    memcpy(info->fw_rev, ic->fw_rev, 8);
    info->num_namespace = ic->num_namespace;
  }

  return result;
}


struct nvme_handle *nvme_open (uint32 id, uint32 namespace_num, uint32 *error_num)
{
  uint32 result;
  struct nvme_device *nvme_d;
  struct nvme_handle *handle = NULL;
  struct nvme_namespace_sizes ns_size;

  if (error_num == NULL)
    return NULL;

  nvme_d = nvme_get_device_handle (id, &result);
  if (nvme_d == NULL)
    goto done;

  if (namespace_num > nvme_d->num_namespace || namespace_num == 0)
  {
    result = NVME_SW_EINVAL_NAMESPACE;
    goto done;
  }

  handle = &static_handle[0];
  if (handle->in_use == 1)
  {
    result = NVME_SW_EMAX_HANDLE;
    handle = NULL;
    goto done;
  }
  handle->nvme_dev = nvme_d;
  handle->in_use = 1;
  handle->namespace = namespace_num;
  handle->last_lba = 0xFFFFFFFFFFFFFFFF;
  result = nvme_get_namespace_sizes(handle, &ns_size);
  if (result == NVME_SUCCESS)
  {
    handle->last_lba = ns_size.size_blocks;
    handle->block_size = ns_size.block_size;
  }
  else
  {
    handle->in_use = 0;
    handle = NULL;
  }

done:
  *error_num = result;
  return handle;
}

uint32 nvme_close (struct nvme_handle *handle)
{
  if (handle->in_use == 0)
    return NVME_SW_EBAD_HANDLE;

  memset (handle, 0, sizeof(struct nvme_handle));
  return NVME_SUCCESS;
}

uint32 nvme_get_namespace_sizes (struct nvme_handle *handle,
                                 struct nvme_namespace_sizes *info)
{
  uint32 result;
  struct nvme_identify_namespace *ins;
  struct nvme_device *nvme_d;

  if (handle == NULL || info == NULL)
    return NVME_SW_EINVAL_PARAM;

  if (handle->in_use == 0)
    return NVME_SW_EBAD_HANDLE;

  memset(info, 0, sizeof(struct nvme_namespace_sizes));
  nvme_d = handle->nvme_dev;
  ins = (struct nvme_identify_namespace *) nvme_d->scratch_buffer;
  result = nvme_cmd_identify_namespace(nvme_d, ins, handle->namespace);
  if (result == NVME_SUCCESS)
  {
    size_t format_index = ins->lba_format.index;
    uint8 lba_size_shift;
    info->size_blocks = ins->size_blocks;
    info->capcaity_blocks = ins->capacity_blocks;
    info->utilization_blocks = ins->utilization_blocks;
    memcpy(info->guid, ins->guid, 16);
    if(ins->num_lba_formats < ins->lba_format.index)
      return NVME_HW_EBAD_NS_INFO;

    lba_size_shift = ins->lbaf[format_index].lba_size_shift;
    if (lba_size_shift >= 32) /* The field is uint32, check overflow. */
      return NVME_HW_EBAD_NS_INFO;

    info->block_size = (1 << lba_size_shift);
  }

  return result;
}

