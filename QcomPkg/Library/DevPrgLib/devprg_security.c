/**************************************************************************
 * FILE: devprg_security.c
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

#include "comdef.h"
#include "devprg_log.h"
#include "devprg_sha.h"
#include "devprg_security.h"


#ifdef FEATURE_DEVPRG_SOFTWARE_HASH  /* No hardware SHA, use software. */

struct devprg_sha256_data
{
  uint8 in_use;
  struct __sechsh_ctx_s context;
};

struct devprg_sha256_data devprg_sha256_instance;

struct devprg_sha256_data *devprg_sha256_init(void)
{
  struct devprg_sha256_data *data = &devprg_sha256_instance;

  if (data->in_use != 0)
    return NULL;

  data->in_use = 1;
  sechsharm_sha256_init  (&data->context);

  return data;
}

int devprg_sha256_update(struct devprg_sha256_data *handle, uint8 *in, uint32 len)
{
  if (handle == NULL || in == NULL)
    return -1;

  sechsharm_sha256_update(&handle->context,
                          handle->context.leftover,
                          &(handle->context.leftover_size),
                          in, len);
  return 0;
}

int devprg_sha256_final(struct devprg_sha256_data *handle, uint8 *out, uint32 len)
{
  if (handle == NULL)
    return -1;

  handle->in_use = 0;
  if (len < HASH_DIGEST_SIZE_SHA256 || out == NULL)
    return -1;

  sechsharm_sha256_final (&handle->context,
                          handle->context.leftover,
                          &(handle->context.leftover_size),
                          out);
  return 0;
}

#else
struct devprg_sha256_data
{
  uint8 in_use;
  CeMLCntxHandle* cntx;
};

struct devprg_sha256_data devprg_sha256_instance;

struct devprg_sha256_data *devprg_sha256_init(void)
{
  struct devprg_sha256_data *data = &devprg_sha256_instance;

  if (data->in_use != 0)
    return NULL;

  data->in_use = 1;
  data->cntx = NULL;
  CeMLInit();
  CeMLHashInit(&data->cntx, CEML_HASH_ALGO_SHA256);

  return data;
}

int devprg_sha256_update(struct devprg_sha256_data *handle, uint8 *in, uint32 len)
{
  CEMLIovecListType ioVecIn;
  CEMLIovecType IovecIn;

  if (handle == NULL || in == NULL)
    return -1;

  /* Input IOVEC */
  ioVecIn.size = 1;
  ioVecIn.iov = &IovecIn;
  ioVecIn.iov[0].dwLen = len; //msg_len;
  ioVecIn.iov[0].pvBase = in; //msg;

  CeMLHashUpdate(handle->cntx, ioVecIn);

  return 0;
}

int devprg_sha256_final(struct devprg_sha256_data *handle, uint8 *out, uint32 len)
{
  CEMLIovecListType ioVecOut;
  CEMLIovecType IovecOut;
  uint32 digest_len = CEML_HASH_DIGEST_SIZE_SHA256;

  if (handle == NULL)
    return -1;

  handle->in_use = 0;
  if (len < digest_len || out == NULL)
    return -1;

  /* Output IOVEC */
  ioVecOut.size = 1;
  ioVecOut.iov = &IovecOut;
  ioVecOut.iov[0].dwLen = digest_len;
  ioVecOut.iov[0].pvBase = out; //result_digest;

  CeMLHashFinal(handle->cntx, &ioVecOut);

  CeMLHashDeInit(&handle->cntx);

  CeMLDeInit();

  return 0;
}


#endif

void devprg_sha256_direct(uint8* input, uint32 len, uint8* sha_out)
{
  struct devprg_sha256_data *handle;
  handle = devprg_sha256_init();
  if (handle == NULL)
  {
    DP_LOGE("Too many concurrent open SHA handles %d", 1);
    memset(sha_out, 0, HASH_DIGEST_SIZE_SHA256);
    return;
  }
  devprg_sha256_update(handle, input, len);
  devprg_sha256_final(handle, sha_out, HASH_DIGEST_SIZE_SHA256);
}


