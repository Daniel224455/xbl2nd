/**************************************************************************
 * FILE: devprg_security.h
 *
 * Abstraction layer for the security APIs.
 *
 * Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Qualcomm Proprietary
 *
 *************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when         who   what, where, why
----------   ---   ---------------------------------------------------------
2018-01-07   wek   Add functions to compute sha256 on data of any size.
2017-07-14   sl    Move functions that change between targets to target file.
2016-11-22   wek   Create.

===========================================================================*/

#ifndef __DEVPRG_SECURITY_H__
#define __DEVPRG_SECURITY_H__

#include "comdef.h"

/* Compute the sha256 of a given buffer.
 *
 * param input   input data to compute the SHA256
 * param len     Length of input
 * param sha_out SHA256 output, the buffer MUST be at least 32 bytes.
 *
 */
void devprg_sha256_direct(uint8* input, uint32 len, uint8* sha_out);

struct devprg_sha256_data;
/* Initialize the structures for computing SHA256.
 *
 * return   A handle to use for the update and final sha256 calls
 **/
struct devprg_sha256_data *devprg_sha256_init(void);

/* Feed new input data to update the sha256 value.
 *
 * param handle  Pointer returned by init.
 * param in      Pointer to the input data to hash.
 * param len     Length in bytes of the input data.
 *
 * return    0 on Success, -1 on error.
 */
int devprg_sha256_update(struct devprg_sha256_data *handle, uint8 *in, uint32 len);

/* Finalize computing the sha256 and free the handle.
 *
 * param handle  Pointer returned by init.
 * param in      Pointer to the output buffer to place the sha256 value. Must be
 *               at least 32 bytes of length.
 * param len     Length in bytes of the output data.
 *
 * return    0 on Success, -1 on error.
 */
int devprg_sha256_final(struct devprg_sha256_data *handle, uint8 *out, uint32 len);

#endif /* __DEVPRG_SECURITY_H__ */
