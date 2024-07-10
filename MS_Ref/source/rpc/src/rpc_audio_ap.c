#include "sci_types.h"
#include "rpc_daemon.h"
#include "os_api.h"

#include <stdlib.h>
#include <string.h>
#include <alloca.h>

#include "rpc_stub_defs.h"
#include "rpc_aud.h"
#include "rpc_aud_par.h"
extern rpcChannel_t *gCfwRpcChannel;
#define RPC_CHANNEL gCfwRpcChannel
#ifndef ALIGNUP8
#define ALIGNUP8(n) (((n) + 7) & ~7)
#endif
#define DISABLE_RPC_AUD_SetCodecOpStatus
// auto generated code
#include "rpc_aud_ap.c"

#include "rpc_aud_c2a_par.h"
#include "rpc_aud_c2a_ap.c"

