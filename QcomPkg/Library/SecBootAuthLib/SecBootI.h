/*=====================================================================================

                         S E C U R I T Y    S E R V I C E S

                 S E C U R E   B O O T   A U T H E N T I C A T I O N

                       I N T E R N A L   H E A D E R  F I L E

FILE: secboot_i.h

DESCRIPTION: This is the secure boot internal header file.

 Hierarchy of dependencies in files for the secure boot process

                      secboot.h (this file, the entry point for this API)
                          |
                      secboot.c--------- secboot_i.h/c (internal, helper
                                                        functions)
                          |
               +--------------+-------+----------------------+
               |                      |                      |
  secboot_x509.h/c       secboot_rsa_math.h/c        secboot_sha1.h/c
   ( x509 parser)       (RSA operations)        (SHA1 interfaces)
         |
         |
  secboot_asn1.h/c
   ( asn1 library)

EXTERNALIZED FUNCTIONS
  None

  Copyright (c)2004, 2006, 2012-2015 Qualcomm Technologies Inc. All Rights Reserved.
===================================================================================*/

/*=========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ------------------------------------------------ 
03/24/15   hw      Add Dynamic Binding feature support
08/05/14   hw      Add QPSI code review fix and 64bit support
05/28/14   hw      Added RoT support
08/28/13   hw      Added retail unlock feature code
03/06/13   vg      Fuse info structure moved to secboot.h header file.
02/10/12   sm      Reverted back to dynamic size check for context type 
05/02/06   AMW     Initial SC2X port.
08/25/04   SJD     Initial revision.  Original SW from the security team.

============================================================================*/
#ifndef SECBOOT_I_H_
#define SECBOOT_I_H_

/* Define the platform here ... */
#if defined(WIN32) || defined(_WIN32)
  #define USE_WINDOWS 1
#endif
/*===========================================================================

                              INCLUDE FILES

============================================================================*/
#define BREAKIF(cond)  { if (cond) break; }

#ifdef USE_WINDOWS
  #include <stdlib.h> // debugging for printfs
  #include <stdio.h>
#endif

#include "secboot.h"
#include "SecMath.h"
#include "boot_util.h"
#include "CeML.h"
/*============================================================================
                               Definitions
=============================================================================*/
#define MEMSET  qmemset
#define MEMCPY  qmemcpy
#define MEMCMP  bByteCompare
#define MEMMOVE qmemcpy
#define BLOAD8  bLoad8
#define BSTOR8  bStor8

#define CEML_PBL_HASHCTX_SIZE      200 // size of buffer pointed to by pClientCtx that PBL crypto needs from caller
                                       // as PBL crypto does not allocate memory from internal static buffer but
                                       // relies on caller to pass it the memory it needs. Using a bigger value
                                       // than the actual size we need, for any future changes

#ifdef USE_WINDOWS
  #define TRUE       1
  #define FALSE      0
  typedef unsigned __int64 uint64;
  typedef unsigned __int32 uint32;
  typedef unsigned __int16 uint16;
  typedef unsigned __int8  uint8;
  typedef signed __int64 int64;
  typedef signed __int32 int32;
  typedef signed __int16 int16;
  typedef signed __int8  int8;
//  typedef int boolean;
  #define SECBOOT_PRINT printf
#else
  #define SECBOOT_PRINT
#endif

/* we stick to a small size to save memory whereever possible */
#define ATTEST_CERT_INDEX        0
#define CA_CERT_INDEX            1
#define ROOT_CERT_INDEX          2

/* Valid OU field options of IN_USE_SOC_HW_VERSION in signed image */
#define SECBOOT_IN_USE_SOC_HW_VER_DISABLE 0x0 /**< Bit value 0 - do not use soc hw version */                                     
#define SECBOOT_IN_USE_SOC_HW_VER_ENABLE  0x1 /**< Bit value 1 - use soc hw version */

/* Valid OU field options of USE_SERIAL_NUMBER_IN_SIGNING in signed image */
#define SECBOOT_OU_USE_SERIAL_NUMBER_DISABLE 0x0 /**< Bit value 0 - do not use serial number */                                     
#define SECBOOT_OU_USE_SERIAL_NUMBER_ENABLE  0x1 /**< Bit value 1 - use serial number */

/**
 * @brief ROT settings which are specified in the ROT OU field of the 
 *        attestation certificate to indicate whether ROT transfer fuse is blown in
 *        the specific phone. The ROT OU field is 64 bits, lower 32 bits
 *        denote what ROT transfer settings the attestation certifiate has been
 *        signed with and the higer 32 bits are serial num. when RoT is enabled 
 *        (SECBOOT_ROT_ENABLE), the serial number embedded in the OU field must match the 
 *        fuse, if rot use serial num fuse is blown. Otherwise, it is not used.  Bits 0  
 *        of the RoT settings denote the boolean flag. 0 means disabled and 1 means 
 *        enabled. Absense of the field means disabled. (option values are chosen by 
 *        the signing tool)
  */
#define SECBOOT_ROT_OU_FIELD_DISABLE 0x0 /**< Bit value 0 - ROT transfer is turned off */                                     
#define SECBOOT_ROT_OU_FIELD_ENABLE  0x1 /**< Bit value 1 - ROT transfer is turned on */

#define SECBOOT_MAX_KEY_SIZE_IN_BITS 4096 /* Secmath has to support this size */
#define SECBOOT_MIN_KEY_SIZE_IN_BITS 2048 /* Secmath has to support this size */
#define SECBOOT_MAX_PUB_EXP_KEY_SIZE_IN_BITS 32 /* in bits, but we expect only exp=3 (2 bits) or worst 65537 (17 bits)
                                    ** the generic case would be = SECBOOT_MAX_KEY_SIZE_IN_BITS but for
                                    ** memory reasons
                                    ** we stick to a small size to save memory whereever possible */

/**
 * @brief Masks for attestation certificates debug enable OU field
 *        This is a 64 bit field, with lower (0:31) bits to indicate
 *        if debugging is to be enabled, and upper (32:63) to indicate
 *        the chip (based on serial number) that the debug cert was
 *        mean for i.e the chip that debugging is to be enabled on
 */

/**<  Mask to get the debug enable/disable bits in the debug OU field */
#define SECBOOT_DEBUG_SETTINGS_FIELD_MASK     (0xFFFFFFFF)
/**<  Shift to get at the serial number from upper 32 bits of the debug enable OU field*/
#define SECBOOT_DEBUG_SERIALNUM_FIELD_SHFT    (0x20)

/**<  Mask to get the crash dump enable/disable bits in the crash dump OU field */
#define SECBOOT_CRASH_DUMP_SETTINGS_FIELD_MASK     (0xFFFFFFFFUL)
/**<  Shift to get at the serial number from upper 32 bits of the crash dump enable OU field*/
#define SECBOOT_CRASH_DUMP_SERIALNUM_FIELD_SHFT    (0x20)

/**<  Mask to get the ROT enable/disable bits in the ROT OU field */
#define SECBOOT_ROT_SETTINGS_FIELD_MASK     (0xFFFFFFFFUL)
/**<  Shift to get at the serial number from upper 32 bits of the ROT enable OU field*/
#define SECBOOT_ROT_SERIALNUM_FIELD_SHFT    (0x20)

/**<  Zero out the upper 32bit of msm_hw_id for soc hw version, and keep the lower 32bit unchanged */
#define SECBOOT_SOC_HW_VER_FIELD_MASK     (0xFFFFFFFFULL)
/**<  Shift the soc hw version to upper 32 bits of msm_hw_id */
#define SECBOOT_SOC_HW_VER_FIELD_SHFT    (0x20)

/**<  Zero out the lower 32bit of msm_hw_id for serial number, and keep the upper 32bit unchanged */
#define SECBOOT_OU_USE_SERIAL_NUMBER_FIELD_MASK     (0xFFFFFFFF00000000ULL)

/* Error log structure to store constant data describing error location */
typedef struct secboot_error_log_type
{
  uint32                   linenum;
  uintptr_t                data1;
  uintptr_t                data2;
  uintptr_t                data3;
} secboot_error_log_type;


#define CONTEXT_MAGIC_NUMBER 0x50425342 //Ascii values for PBSB - PBL Secure Boot

//Make sure this structure is uint32 aligned
typedef struct secboot_context_type
{
    uint32                         magic_number;
    secboot_crypto_hash_ftbl_type  crypto_hash_ftbl;
    uint32                         version; //version of secboot release
    secboot_error_log_type         error_log;      
    secboot_fuse_info_type         fuses;
    boolean                        use_supplied_fuses; /* This value will be set to TRUE,
                                                     if fuse values are supplied from externally */
}secboot_context_type;

#endif

