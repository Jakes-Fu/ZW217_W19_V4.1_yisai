/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "rpc_hello.h"
#include "rpc_verify.h"

#ifndef ALIGNUP8
#define ALIGNUP8(n) (((n) + 7) & ~7)
#endif

#define DISABLE_RPC_rpcGetPeerTags

uint32_t rpcHelloOnAP(uint32_t n)
{
   uint8_t ret = 0;
#ifdef HALO_SUPPORT
   ret = AcknowledgeCPshakehands();
#endif
   return ret;
}

#define RPC_CHANNEL rpcChannelOpen()
#include "rpc_stub_defs.h"
#include "rpc_a2c_par.h"
#include "rpc_a2c_ap.c"
#include "rpc_c2a_par.h"
#include "rpc_c2a_ap.c"
