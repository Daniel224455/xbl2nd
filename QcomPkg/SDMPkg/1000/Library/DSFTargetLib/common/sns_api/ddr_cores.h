#ifndef __DDR_CORES_H__
#define __DDR_CORES_H__

/*******************************************************************************
 QUALCOMM Proprietary Design Data
 Copyright (c) 2014,2018, Qualcomm Technologies Incorporated.
 All rights reserved.
 ******************************************************************************/
/*==============================================================================
                              EDIT HISTORY
==============================================================================*/

// Core Version
#define TARGET_CORE_ARCH_SHIFT  24
#define TARGET_CORE_MAJOR_SHIFT 16
#define TARGET_CORE_MINOR_SHIFT 8

#define TARGET_CORE_VER(arch, major, minor, patch) \
( \
    (arch << TARGET_CORE_ARCH_SHIFT) | \
    (major << TARGET_CORE_MAJOR_SHIFT) | \
    (minor << TARGET_CORE_MINOR_SHIFT) | \
    patch \
)


#endif /* __DDR_CORES_H__ */
