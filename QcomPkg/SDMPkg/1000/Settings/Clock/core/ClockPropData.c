/*
===========================================================================
*/
/**
  @file ClockPropData.c

DESCRIPTION:
  This file contains the clock driver Properties data.

*/
/*
  ====================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================
  ====================================================================
*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "Drivers/ClockDxe/ClockBSP.h"
#include "../../Library/ClockLib/ClockApps.h"
#include "../../Library/HALclkLib/HALclkHWIO.h"
#include "HALhwio.h"


/*=========================================================================
      Data
==========================================================================*/

/*
 * Clock Log Default Configuration.
 *
 */
const ClockLogType ClockLogDefaultConfig[] =
{
  {
    .nLogSize        = 4096,
    .nGlobalLogFlags = 0x90ff
  }
};

/*
 * Initial rail voltage BSP data.
 */
ClockRailConfigType ClockRailConfig[] =
{
  { "/arc/client/rail_cx",   RAIL_VOLTAGE_LEVEL_NOM },
  { "/vcs/vdd_gx",           RAIL_VOLTAGE_LEVEL_NOM },
  { "/arc/client/rail_mmcx", RAIL_VOLTAGE_LEVEL_NOM },
  { "/arc/client/rail_mx",   RAIL_VOLTAGE_LEVEL_NOM },
#ifdef ENABLE_AUTO_PLAT
  { "/vcs/vdd_ssc_int",RAIL_VOLTAGE_LEVEL_NOM },
#endif
  { NULL }
};

/* List of clocks that are needed to access the clocks in the clock controllers owned by APCS */
ClockNameListType ClockaClocksRequiredOnList[] =
{
 {"gcc_camera_ahb_clk"            },
 {"gcc_gpu_cfg_ahb_clk"           },
 {"gcc_disp_ahb_clk"              },
 {"gcc_video_ahb_clk"             },
 {"gcc_npu_cfg_ahb_clk"           },
 {"gcc_pcie_0_cfg_ahb_clk"        },
 {"gcc_pcie_1_cfg_ahb_clk"        },
/*
 {"gcc_sdcc2_ahb_clk"            },
 {"gcc_sdcc4_ahb_clk"            },
 {"gcc_boot_rom_ahb_clk"         },
 {"gcc_prng_ahb_clk"             },
 {"gcc_qupv3_wrap_0_m_ahb_clk"   },
 {"gcc_qupv3_wrap_0_s_ahb_clk"   },
 {"gcc_qupv3_wrap_1_m_ahb_clk"   },
 {"gcc_qupv3_wrap_1_s_ahb_clk"   },
 {"gcc_qupv3_wrap_2_m_ahb_clk"   },
 {"gcc_qupv3_wrap_2_s_ahb_clk"   },
 {"gcc_emac_slv_ahb_clk"         },
 {"gcc_qmip_camera_nrt_ahb_clk"  },
 {"gcc_qmip_camera_rt_ahb_clk"   },
 {"gcc_qmip_disp_ahb_clk"        },
 {"gcc_qmip_video_cvp_ahb_clk"   },
 {"gcc_qmip_video_vcodec_ahb_clk"},
 {"gcc_pdm_ahb_clk"              },
 {"gcc_qspi_cnoc_periph_ahb_clk" },
 {"gcc_tsif_ahb_clk"             },
 {"gcc_ufs_card_ahb_clk"         },
 {"gcc_ufs_phy_ahb_clk"          },
*/

{NULL}
};

/*
 * List of clocks allowed to be suppressible.
 */
ClockNameListType ClockSuppressibleList[] =
{
  // Serial Debugger:

  // USB 2.0 Debugger:

  // USB 3.0 Debugger:
  { "gcc_aggre_usb3_prim_axi_clk"                },
  { "gcc_aggre_usb3_sec_axi_clk"                 },
  { "gcc_cfg_noc_usb3_prim_axi_clk"              },
  { "gcc_cfg_noc_usb3_sec_axi_clk"               },
  { "gcc_usb30_mp_sleep_clk"                     },
  { "gcc_usb30_prim_master_clk"                  },
  { "gcc_usb30_prim_mock_utmi_clk"               },
  { "gcc_usb30_prim_sleep_clk"                   },
  { "gcc_usb30_sec_master_clk"                   },
  { "gcc_usb30_sec_mock_utmi_clk"                },
  { "gcc_usb30_sec_sleep_clk"                    },
  { "gcc_usb3_mp_phy_com_aux_clk"                },
  { "gcc_usb3_mp_phy_pipe_0_clk"                 },
  { "gcc_usb3_mp_phy_pipe_1_clk"                 },
  { "gcc_usb3_prim_clkref_en"                    },
  { "gcc_usb3_prim_phy_aux_clk"                  },
  { "gcc_usb3_prim_phy_com_aux_clk"              },
  { "gcc_usb3_prim_phy_pipe_clk"                 },
  { "gcc_usb3_sec_clkref_en"                     },
  { "gcc_usb3_sec_phy_aux_clk"                   },
  { "gcc_usb3_sec_phy_com_aux_clk"               },
  { "gcc_usb3_sec_phy_pipe_clk"                  },

  // WinDbg UART:
  { "gcc_qupv3_wrap1_core_2x_clk"                },
  { "gcc_qupv3_wrap1_core_clk"                   },
  { "gcc_qupv3_wrap1_s1_clk"                     },
  { "gcc_qupv3_wrap1_s4_clk"                     },
  { "gcc_qupv3_wrap_1_m_ahb_clk"                 },
  { "gcc_qupv3_wrap_1_s_ahb_clk"                 },

  // Modem clocks:
  { "gcc_boot_rom_ahb_clk"                       },

  // Fix for CLK_REF/PCIe, Poipu V1 HW clocks:
  { "gcc_rx2_qlink_clkref_en"                    },
  { "gcc_ufs_card_clkref_en"                     },
  { "gcc_ufs_mem_clkref_en"                      },
  
  // Display clocks:
  { "gcc_disp_ahb_clk"                           },

  { NULL }
};

/*
 * List of clocks allowed to be always on.
 */
ClockNameListType ClockAlwaysOnList[] =
{
  // USB 3.0 Debugger:
  { "gcc_aggre_usb3_prim_axi_clk"                },
  { "gcc_aggre_usb3_sec_axi_clk"                 },
  { "gcc_cfg_noc_usb3_prim_axi_clk"              },
  { "gcc_cfg_noc_usb3_sec_axi_clk"               },
  { "gcc_usb30_mp_sleep_clk"                     },
  { "gcc_usb30_prim_master_clk"                  },
  { "gcc_usb30_prim_mock_utmi_clk"               },
  { "gcc_usb30_prim_sleep_clk"                   },
  { "gcc_usb30_sec_master_clk"                   },
  { "gcc_usb30_sec_mock_utmi_clk"                },
  { "gcc_usb30_sec_sleep_clk"                    },
  { "gcc_usb3_mp_phy_com_aux_clk"                },
  { "gcc_usb3_mp_phy_pipe_0_clk"                 },
  { "gcc_usb3_mp_phy_pipe_1_clk"                 },
  { "gcc_usb3_prim_clkref_en"                    },
  { "gcc_usb3_prim_phy_aux_clk"                  },
  { "gcc_usb3_prim_phy_com_aux_clk"              },
  { "gcc_usb3_prim_phy_pipe_clk"                 },
  { "gcc_usb3_sec_clkref_en"                     },
  { "gcc_usb3_sec_phy_aux_clk"                   },
  { "gcc_usb3_sec_phy_com_aux_clk"               },
  { "gcc_usb3_sec_phy_pipe_clk"                  },

  // WinDbg UART:
  { "gcc_qupv3_wrap1_core_2x_clk"                },
  { "gcc_qupv3_wrap1_core_clk"                   },
  { "gcc_qupv3_wrap1_s1_clk"                     },
  { "gcc_qupv3_wrap1_s4_clk"                     },
  { "gcc_qupv3_wrap_1_m_ahb_clk"                 },
  { "gcc_qupv3_wrap_1_s_ahb_clk"                 },

  // Modem clocks:
  { "gcc_boot_rom_ahb_clk"                       },

  // Fix for CLK_REF/PCIe, Poipu V1 HW clocks:
  { "gcc_rx2_qlink_clkref_en"                    },
  { "gcc_ufs_card_clkref_en"                     },
  { "gcc_ufs_mem_clkref_en"                      },
  
  // Display clocks:
  { "gcc_disp_ahb_clk"                           },

  { NULL }
};

/*
 * List of clocks allowed to be reference counter disabled.
 */
ClockNameListType ClockRefCountSuspendedList[] =
{
  // USB 2.0

  // USB 3.0
  { "gcc_usb30_prim_mock_utmi_clk"               },
  { "gcc_usb30_sec_mock_utmi_clk"                },

  { NULL }
};

/*
 * List of clocks allowed to be configured with the
 * FORCE_MEM_CORE and FORCE_MEM_PERIPH parameters.
 */
ClockNameListType ClockForceMemCoreAndPeriphList[] =
{
  { "cam_cc_bps_clk"                             },
  { "cam_cc_camnoc_axi_clk"                      },
  { "cam_cc_fd_core_clk"                         },
  { "cam_cc_icp_clk"                             },
  { "cam_cc_ife_0_clk"                           },
  { "cam_cc_ife_0_csid_clk"                      },
  { "cam_cc_ife_1_clk"                           },
  { "cam_cc_ife_1_csid_clk"                      },
  { "cam_cc_ife_2_clk"                           },
  { "cam_cc_ife_2_csid_clk"                      },
  { "cam_cc_ife_3_clk"                           },
  { "cam_cc_ife_3_csid_clk"                      },
  { "cam_cc_ife_lite_0_csid_clk"                 },
  { "cam_cc_ife_lite_1_csid_clk"                 },
  { "cam_cc_ife_lite_2_csid_clk"                 },
  { "cam_cc_ife_lite_3_csid_clk"                 },
  { "cam_cc_ipe_0_clk"                           },
  { "cam_cc_ipe_1_clk"                           },
  { "cam_cc_jpeg_clk"                            },
  { "cam_cc_lrme_clk"                            },
  { "disp_cc_mdss_mdp_clk"                       },
  { "disp_cc_mdss_mdp_lut_clk"                   },
  { "disp_cc_mdss_rot_clk"                       },
  { "gcc_aggre_noc_pcie_tbu_clk"                 },
  { "gcc_boot_rom_ahb_clk"                       },
  { "gcc_disp_hf_axi_clk"                        },
  { "gcc_disp_sf_axi_clk"                        },
  { "gcc_emac_axi_clk"                           },
  { "gcc_emac_rgmii_clk"                         },
  { "gcc_gpu_memnoc_gfx_clk"                     },
  { "gcc_pcie_0_mstr_axi_clk"                    },
  { "gcc_pcie_0_pipe_clk"                        },
  { "gcc_pcie_0_slv_axi_clk"                     },
  { "gcc_pcie_1_mstr_axi_clk"                    },
  { "gcc_pcie_1_pipe_clk"                        },
  { "gcc_pcie_1_slv_axi_clk"                     },
  { "gcc_pcie_2_mstr_axi_clk"                    },
  { "gcc_pcie_2_pipe_clk"                        },
  { "gcc_pcie_2_slv_axi_clk"                     },
  { "gcc_pcie_3_mstr_axi_clk"                    },
  { "gcc_pcie_3_pipe_clk"                        },
  { "gcc_pcie_3_slv_axi_clk"                     },
  { "gcc_qupv3_wrap0_core_2x_clk"                },
  { "gcc_qupv3_wrap0_core_clk"                   },
  { "gcc_qupv3_wrap0_s0_clk"                     },
  { "gcc_qupv3_wrap0_s1_clk"                     },
  { "gcc_qupv3_wrap0_s2_clk"                     },
  { "gcc_qupv3_wrap0_s3_clk"                     },
  { "gcc_qupv3_wrap0_s4_clk"                     },
  { "gcc_qupv3_wrap0_s5_clk"                     },
  { "gcc_qupv3_wrap0_s6_clk"                     },
  { "gcc_qupv3_wrap0_s7_clk"                     },
  { "gcc_qupv3_wrap1_core_2x_clk"                },
  { "gcc_qupv3_wrap1_core_clk"                   },
  { "gcc_qupv3_wrap1_s0_clk"                     },
  { "gcc_qupv3_wrap1_s1_clk"                     },
  { "gcc_qupv3_wrap1_s2_clk"                     },
  { "gcc_qupv3_wrap1_s3_clk"                     },
  { "gcc_qupv3_wrap1_s4_clk"                     },
  { "gcc_qupv3_wrap1_s5_clk"                     },
  { "gcc_qupv3_wrap2_core_2x_clk"                },
  { "gcc_qupv3_wrap2_core_clk"                   },
  { "gcc_qupv3_wrap2_s0_clk"                     },
  { "gcc_qupv3_wrap2_s1_clk"                     },
  { "gcc_qupv3_wrap2_s2_clk"                     },
  { "gcc_qupv3_wrap2_s3_clk"                     },
  { "gcc_qupv3_wrap2_s4_clk"                     },
  { "gcc_qupv3_wrap2_s5_clk"                     },
  { "gcc_sdcc2_apps_clk"                         },
  { "gcc_sdcc4_apps_clk"                         },
  { "gcc_tsif_ahb_clk"                           },
  { "gcc_ufs_card_2_axi_clk"                     },
  { "gcc_ufs_card_2_ice_core_clk"                },
  { "gcc_ufs_card_2_unipro_core_clk"             },
  { "gcc_ufs_card_axi_clk"                       },
  { "gcc_ufs_card_ice_core_clk"                  },
  { "gcc_ufs_card_unipro_core_clk"               },
  { "gcc_ufs_phy_axi_clk"                        },
  { "gcc_ufs_phy_ice_core_clk"                   },
  { "gcc_ufs_phy_unipro_core_clk"                },
  { "gcc_usb30_mp_master_clk"                    },
  { "gcc_usb30_prim_master_clk"                  },
  { "gcc_usb30_sec_master_clk"                   },
  { "gcc_video_axi0_clk"                         },
  { "gcc_video_axi1_clk"                         },
  { "npu_cc_cal_dp_clk"                          },
  { "npu_cc_comp_noc_axi_clk"                    },
  { "npu_cc_npu_core_clk"                        },
  { "video_cc_mvs0_core_clk"                     },
  { "video_cc_mvs1_core_clk"                     },
  { "video_cc_mvsc_core_clk"                     },

  { NULL }
};

/*
 * List of clocks needed to be enabled prior to running the BIST.
 */
ClockNameListType ClockBistDependencies[] =
{
  { "gcc_camera_ahb_clk"                         },
  { "gcc_disp_ahb_clk"                           },
  { "gcc_video_ahb_clk"                          },
  { "cam_cc_camnoc_dcd_xo_clk"                   },
  { "cam_cc_qdss_debug_xo_clk"                   },
  { "disp_cc_xo_clk"                             },
  { "gcc_camera_xo_clk"                          },
  { "gcc_disp_xo_clk"                            },
  { "gcc_video_xo_clk"                           },
  { "npu_cc_xo_clk"                              },
  { "video_cc_xo_clk"                            },

  { NULL }
};

/*
 * List of clocks anticipated to fail the BIST.
 */
ClockNameListType ClockBistExceptions[] =
{
  { "apcs_gold_sysleaf_clk"                      },
  { "apcs_l3_sysleaf_clk"                        },
  { "apcs_silver_sysleaf_clk"                    },
  { "cam_cc_ife_0_atspeedreg_clk"                },
  { "cam_cc_ife_0_dsp_clk"                       },
  { "cam_cc_ife_1_atspeedreg_clk"                },
  { "cam_cc_ife_1_dsp_clk"                       },
  { "cam_cc_ife_2_atspeedreg_clk"                },
  { "cam_cc_ife_2_dsp_clk"                       },
  { "cam_cc_ife_3_atspeedreg_clk"                },
  { "cam_cc_ife_3_dsp_clk"                       },
  { "cam_cc_ipe_1_clk"                           },
  { "cam_cc_pll_test_clk"                        },
  { "cam_cc_spdm_bps_clk"                        },
  { "cam_cc_spdm_ife_0_clk"                      },
  { "cam_cc_spdm_ife_0_csid_clk"                 },
  { "cam_cc_spdm_ife_2_clk"                      },
  { "cam_cc_spdm_ife_2_csid_clk"                 },
  { "cam_cc_spdm_ife_3_clk"                      },
  { "cam_cc_spdm_ife_3_csid_clk"                 },
  { "cam_cc_spdm_ipe_0_clk"                      },
  { "cam_cc_spdm_ipe_1_clk"                      },
  { "cam_cc_spdm_jpeg_clk"                       },
  { "disp_cc_mdss_dp_crypto1_clk"                },
  { "disp_cc_mdss_dp_crypto_clk"                 },
  { "disp_cc_mdss_dp_link1_intf_clk"             },
  { "disp_cc_mdss_dp_link_intf_clk"              },
  { "disp_cc_mdss_edp_link_intf_clk"             },
  { "disp_cc_mdss_spdm_dp_crypto_clk"            },
  { "disp_cc_mdss_spdm_dp_pixel1_clk"            },
  { "disp_cc_mdss_spdm_dp_pixel_clk"             },
  { "disp_cc_mdss_spdm_mdp_clk"                  },
  { "disp_cc_mdss_spdm_pclk0_clk"                },
  { "disp_cc_mdss_spdm_pclk1_clk"                },
  { "disp_cc_mdss_spdm_rot_clk"                  },
  { "disp_cc_pll_test_clk"                       },
  { "gcc_npu_axi_clk"                            },
  { "gcc_pcie_0_link_down_bcr"                   },
  { "gcc_pcie_0_nocsr_com_phy_bcr"               },
  { "gcc_pcie_0_phy_bcr"                         },
  { "gcc_pcie_0_phy_nocsr_com_phy_bcr"           },
  { "gcc_pcie_0_pipe_clk"                        },
  { "gcc_pcie_1_link_down_bcr"                   },
  { "gcc_pcie_1_nocsr_com_phy_bcr"               },
  { "gcc_pcie_1_phy_bcr"                         },
  { "gcc_pcie_1_phy_nocsr_com_phy_bcr"           },
  { "gcc_pcie_2_link_down_bcr"                   },
  { "gcc_pcie_2_nocsr_com_phy_bcr"               },
  { "gcc_pcie_2_phy_bcr"                         },
  { "gcc_pcie_2_phy_nocsr_com_phy_bcr"           },
  { "gcc_pcie_2_pipe_clk"                        },
  { "gcc_pcie_3_link_down_bcr"                   },
  { "gcc_pcie_3_nocsr_com_phy_bcr"               },
  { "gcc_pcie_3_phy_bcr"                         },
  { "gcc_pcie_3_phy_nocsr_com_phy_bcr"           },
  { "gcc_pcie_phy_cfg_ahb_bcr"                   },
  { "gcc_pcie_phy_com_bcr"                       },
  { "gcc_qusb2phy_5_bcr"                         },
  { "gcc_qusb2phy_mp0_bcr"                       },
  { "gcc_qusb2phy_mp1_bcr"                       },
  { "gcc_qusb2phy_prim_bcr"                      },
  { "gcc_qusb2phy_sec_bcr"                       },
  { "gcc_ufs_card_2_rx_symbol_0_clk"             },
  { "gcc_ufs_card_2_rx_symbol_1_clk"             },
  { "gcc_ufs_card_2_tx_symbol_0_clk"             },
  { "gcc_ufs_card_rx_symbol_0_clk"               },
  { "gcc_ufs_card_rx_symbol_1_clk"               },
  { "gcc_ufs_card_tx_symbol_0_clk"               },
  { "gcc_ufs_phy_rx_symbol_0_clk"                },
  { "gcc_ufs_phy_rx_symbol_1_clk"                },
  { "gcc_ufs_phy_tx_symbol_0_clk"                },
  { "gcc_usb3_dp_phy_prim_sp0_bcr"               },
  { "gcc_usb3_dp_phy_prim_sp1_bcr"               },
  { "gcc_usb3_dp_phy_sec_bcr"                    },
  { "gcc_usb3_phy_prim_sp0_bcr"                  },
  { "gcc_usb3_phy_prim_sp1_bcr"                  },
  { "gcc_usb3_phy_sec_bcr"                       },
  { "gcc_usb3_sec_phy_pipe_clk"                  },
  { "gcc_usb3_uniphy_mp0_bcr"                    },
  { "gcc_usb3_uniphy_mp1_bcr"                    },
  { "gcc_usb3phy_phy_prim_sp0_bcr"               },
  { "gcc_usb3phy_phy_prim_sp1_bcr"               },
  { "gcc_usb3phy_phy_sec_bcr"                    },
  { "gcc_usb3uniphy_phy_mp0_bcr"                 },
  { "gcc_usb3uniphy_phy_mp1_bcr"                 },
  { "gpu_cc_cx_apb_clk"                          },
  { "gpu_cc_cx_gmu_clk"                          },
  { "gpu_cc_cx_qdss_at_clk"                      },
  { "gpu_cc_cx_qdss_trig_clk"                    },
  { "gpu_cc_cx_qdss_tsctr_clk"                   },
  { "gpu_cc_gx_gmu_clk"                          },
  { "gpu_cc_gx_qdss_tsctr_clk"                   },
  { "gpu_cc_gx_vsense_clk"                       },
  { "gpu_cc_pll_test_clk"                        },
  { "npu_cc_armwic_core_clk"                     },
  { "npu_cc_bto_core_clk"                        },
  { "npu_cc_bwmon_clk"                           },
  { "npu_cc_cal_dp_cdc_clk"                      },
  { "npu_cc_cal_dp_clk"                          },
  { "npu_cc_comp_noc_axi_clk"                    },
  { "npu_cc_conf_noc_ahb_clk"                    },
  { "npu_cc_npu_core_apb_clk"                    },
  { "npu_cc_npu_core_atb_clk"                    },
  { "npu_cc_npu_core_clk"                        },
  { "npu_cc_npu_core_cti_clk"                    },
  { "npu_cc_npu_cpc_clk"                         },
  { "npu_cc_npu_cpc_timer_clk"                   },
  { "npu_cc_perf_cnt_clk"                        },
  { "npu_cc_pll_test_clk"                        },
  { "npu_cc_qtimer_core_clk"                     },
  { "npu_cc_sleep_clk"                           },
  { "video_cc_debug_clk"                         },

  { NULL }
};

/*
 * List of power domain names NOT to be enabled prior to running the BIST.
 */
ClockNameListType ClockBistExcludedPowerDomainNames[] =
{
  { "gcc_hlos1_vote_all_smmu_mmu_gds"            },
  { "gcc_hlos1_vote_mmnoc_mmu_tbu_hf0_gds"       },
  { "gcc_hlos1_vote_mmnoc_mmu_tbu_hf1_gds"       },
  { "gcc_hlos1_vote_mmnoc_mmu_tbu_sf_gds"        },
  { "gcc_hlos1_vote_mmu_tcu_gds"                 },
  { "gcc_hlos1_vote_turing_mmu_tbu0_gds"         },
  { "gcc_hlos1_vote_turing_mmu_tbu1_gds"         },
  { "ufs_card_gdsc"                              },
  { "gx_gdsc"                                    },
  { "npu_core_gdsc"                              },

  { NULL }
};

/*
 * List of clocks excluded from the BIST.
 */
ClockNameListType ClockExcludeFromBist[] =
{
  { "gcc_gp4_clk"                                },
  { "gcc_gp5_clk"                                },
  { "gcc_qspi_1_cnoc_periph_ahb_clk"             },
  { "gcc_qspi_1_core_clk"                        },
  { "gcc_cxo2_tx1_clkref_en1"                    },
  { "gcc_qspi_cnoc_periph_ahb_clk"               },
  { "gcc_qspi_core_clk"                          },
  { "npu_cc_bto_core_clk",                       },
  { "npu_cc_bwmon_clk",                          },
  { "npu_cc_npu_core_apb_clk",                   },
  { "npu_cc_npu_cpc_timer_clk",                  },
  { "npu_cc_qtimer_core_clk",                    },
  { "npu_cc_xo_clk",                             },
  { "npu_cc_comp_noc_axi_clk",                   },
  { "npu_cc_conf_noc_ahb_clk",                   },
  { "npu_cc_npu_core_atb_clk",                   },
  { "npu_cc_npu_core_cti_clk",                   },
  { "npu_cc_sleep_clk",                          },
  { "gcc_npu_cfg_ahb_clk",                       },
  { "gcc_npu_axi_clk",                           },
  { "gcc_npu_at_clk",                            },
  { "gcc_npu_trig_clk",                          },
  { "npu_cc_cal_dp_cdc_clk",                     },
  { "npu_cc_cal_dp_clk",                         },
  { "npu_cc_perf_cnt_clk",                       },
  { "npu_cc_debug_clk",                          },
  { "npu_cc_armwic_core_clk",                    },
  { "npu_cc_npu_core_clk",                       },
  { "npu_cc_npu_cpc_clk",                        },
  { "npu_cc_pll_test_clk",                       },

  { NULL }
};

/*
 * List of clocks allowed to enable/disable clock branch only control.
 */
ClockNameListType ClockBranchOnlyRawControlList[] =
{
  { "disp_cc_mdss_ahb_clk"                       },
  { "disp_cc_mdss_byte0_clk"                     },
  { "disp_cc_mdss_byte0_intf_clk"                },
  { "disp_cc_mdss_byte1_clk"                     },
  { "disp_cc_mdss_byte1_intf_clk"                },
  { "disp_cc_mdss_dp_aux1_clk"                   },
  { "disp_cc_mdss_dp_aux_clk"                    },
  { "disp_cc_mdss_dp_crypto1_clk"                },
  { "disp_cc_mdss_dp_crypto_clk"                 },
  { "disp_cc_mdss_dp_link1_clk"                  },
  { "disp_cc_mdss_dp_link1_intf_clk"             },
  { "disp_cc_mdss_dp_link_clk"                   },
  { "disp_cc_mdss_dp_link_intf_clk"              },
  { "disp_cc_mdss_dp_pixel1_clk"                 },
  { "disp_cc_mdss_dp_pixel2_clk"                 },
  { "disp_cc_mdss_dp_pixel_clk"                  },
  { "disp_cc_mdss_edp_aux_clk"                   },
  { "disp_cc_mdss_edp_gtc_clk"                   },
  { "disp_cc_mdss_edp_link_clk"                  },
  { "disp_cc_mdss_edp_link_intf_clk"             },
  { "disp_cc_mdss_edp_pixel_clk"                 },
  { "disp_cc_mdss_esc0_clk"                      },
  { "disp_cc_mdss_esc1_clk"                      },
  { "disp_cc_mdss_mdp_clk"                       },
  { "disp_cc_mdss_mdp_lut_clk"                   },
  { "disp_cc_mdss_non_gdsc_ahb_clk"              },
  { "disp_cc_mdss_pclk0_clk"                     },
  { "disp_cc_mdss_pclk1_clk"                     },
  { "disp_cc_mdss_rot_clk"                       },
  { "disp_cc_mdss_rscc_ahb_clk"                  },
  { "disp_cc_mdss_rscc_vsync_clk"                },
  { "disp_cc_mdss_spdm_debug_clk"                },
  { "disp_cc_mdss_spdm_dp_crypto_clk"            },
  { "disp_cc_mdss_spdm_dp_pixel1_clk"            },
  { "disp_cc_mdss_spdm_dp_pixel_clk"             },
  { "disp_cc_mdss_spdm_mdp_clk"                  },
  { "disp_cc_mdss_spdm_pclk0_clk"                },
  { "disp_cc_mdss_spdm_pclk1_clk"                },
  { "disp_cc_mdss_spdm_rot_clk"                  },
  { "disp_cc_mdss_vsync_clk"                     },

  { NULL }
};

/*
 * Clock property table for use in non-DAL environments.
 *
 * NOTE:
 * Please add clock under proper subsystem and place it in order (i.e.,
 * each sub-list is sorted based on the string clock name).
 *
 */
const ClockPropertyType Clock_aProperties[] =
{
  /*
   * Clock Log Defaults.
   */
  { "ClockLogDefaults",                          ClockLogDefaultConfig },

  /*
   * Initial rail voltage BSP data.
   */
  { "ClockRailConfig",                           ClockRailConfig },

  /*
   * Clocks allowed to be suppressible.
   */
  { "SuppressibleList",                          ClockSuppressibleList },

  /*
   * Clocks allowed to be always on.
   */
  { "ClockAlwaysOnList",                         ClockAlwaysOnList },

  /*
   * Clocks allowed to be reference counter disabled.
   */
  { "RefCountSuspendedList",                     ClockRefCountSuspendedList },

  /*
   * Clocks allowed to be configured with the
   * FORCE_MEM_CORE and FORCE_MEM_PERIPH parameters.
   */
  { "ForceMemCoreAndPeriphList",                 ClockForceMemCoreAndPeriphList },

  /*
   * List of clocks needed to be enabled prior to running the BIST.
   */
  { "BistDependencies",                          ClockBistDependencies },

  /*
   * Clock BIST Exceptions.
   */
  { "BISTExceptions",                            ClockBistExceptions },

  /*
   * List of power domain names NOT to be enabled prior to running the BIST.
   */
  { "ExcludedPowerDomains",                      ClockBistExcludedPowerDomainNames },

  /*
   * List of clocks excluded from BIST.
   */
  { "ExcludeFromBist",                           ClockExcludeFromBist },

  /*
   * List of clocks allowed to enable/disable clock branch only control.
   */
  { "BranchOnlyRawControlList",                  ClockBranchOnlyRawControlList },

  { NULL, NULL }
};

