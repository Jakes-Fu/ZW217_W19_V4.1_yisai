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

#include "sci_types.h"
#include "rpc_daemon.h"
#include "os_api.h"
#include "cfw.h"
#include "cfw_event.h"

#include <stdlib.h>
#include <string.h>
#include <alloca.h>

#define DISABLE_RPC_CFW_UsimDecodeEFFcp
#define DISABLE_RPC_rpcGetPeerTags

rpcChannel_t *gCfwRpcChannel = NULL;

extern void MN_SendCfwEventToMn(void * pEvent);
extern void rpcCpInitDone(void);
static void CFW_RpcSendEvent(void *ctx, const void *event)
{
	uint32* pEvent = SCI_ALLOC_APP(4*sizeof(uint32));
	memcpy(pEvent, event, 4*sizeof(uint32));

	SCI_TraceLow("CFW_RpcSendEvent, id:%d, p1:0x%x, p(0x%x)",*((uint32*)pEvent), *((uint32*)pEvent + 1),pEvent);
	if(*((uint32*)pEvent) == 51)
	{
		rpcCpInitDone();
	}
	MN_SendCfwEventToMn(pEvent);
}

void CFW_RpcRegisterEvents()
{
	if (!rpcRegisterEvents(gCfwRpcChannel, 0, 0xffffffff, CFW_RpcSendEvent, NULL))
	; // cosPanic();

}
void CFW_RpcInit(void)
{
    gCfwRpcChannel = rpcChannelOpen();
}

// auto generated stub functions

#include "rpc_stub_defs.h"

#define RPC_CHANNEL gCfwRpcChannel
#define ALIGNUP8(n) (((n) + 7) & ~7)
#define EVENT_PAR1 (event->par1)
#define EVENT_PAR2 (event->par2)
#define EVENT_PAR3 (event->par3)
#define EVENT_PAR2_HI16 (EVENT_PAR2 >> 16)
#define EVENT_PAR2_LO16 (EVENT_PAR2 & 0xffff)
#define EVENT_TYPE ((event->par3 >> 8) & 0xff)
#define CFW_GPRS_DATA_SIZE (sizeof(CFW_GPRS_DATA) + ((CFW_GPRS_DATA *)EVENT_PAR1)->nDataLength)
#define SIZE_PARAM2_IF_TYPE0 (EVENT_TYPE == 0 ? EVENT_PAR2 : 0)
#define SIZE_PARAM2_IF_TYPE1 (EVENT_TYPE == 1 ? EVENT_PAR2 : 0)
#define SIZE_PARAM2HI16_IF_TYPE0 (EVENT_TYPE == 0 ? EVENT_PAR2_HI16 : 0)
#define SIZE_PARAM2HI16L8_IF_TYPE0 (EVENT_TYPE == 0 ? (EVENT_PAR2_HI16 & 0xFF) : 0)
#define SIZE_PARAM2LO16_IF_TYPE0 (EVENT_TYPE == 0 ? EVENT_PAR2_LO16 : 0)
#define COUNT_PARAM2_IF_TYPE0(s) (EVENT_TYPE == 0 ? EVENT_PAR2 * (s) : 0)
#define COUNT_PARAM2LO16_IF_TYPE0(s) (EVENT_TYPE == 0 ? EVENT_PAR2_LO16 * (s) : 0)
#define SIZE_IF_TYPE0(s) (EVENT_TYPE == 0 ? (s) : 0)
#define SIZE_IF_TYPE1(s) (EVENT_TYPE == 1 ? (s) : 0)
#define EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP_PTR1_SIZE (6 * EVENT_PAR2)
#define EV_CFW_NW_GET_AVAIL_OPERATOR_RSP_PTR1_SIZE COUNT_PARAM2LO16_IF_TYPE0((SIZE_PARAM2HI16_IF_TYPE0 == 0) ? sizeof(CFW_NW_OPERATOR_INFO) : sizeof(CFW_NW_OPERATOR_INFO_V2))
#define EV_CFW_NW_GET_QSCANF_RSP_PTR1_SIZE COUNT_PARAM2_IF_TYPE0(sizeof(uint32_t))
#define EV_CFW_SMS_READ_MESSAGE_RSP_PTR1_SIZE SIZE_IF_TYPE0(sizeof(CFW_SMS_NODE) + sizeof(CFW_SMS_INFO_UNION))
#define SMS_PDU_LEN_IF_TYPE0 (EVENT_TYPE == 0 ? (EVENT_PAR1 == 0 ? 0 : 176) : 0)
#define SAT_RSP_RSP_IF_TYPE0 (EVENT_TYPE == 0 ? ((EVENT_PAR1 == 0xD4 || EVENT_PAR1 == 0xFE) ? (sizeof(CFW_SAT_TERMINAL_ENVELOPE_CNF)) : 0) : 0)
#define CONCAT_SMS_INFO_SIZE ((EVENT_TYPE == 0 && EVENT_PAR2 != 0) ? sizeof(CFW_EMS_INFO) : 0)
#define COUNT_PARAM2LO16_AND_SIZE_STRUCT (sizeof(CFW_WIFISCAN_AP_RESULT) * EVENT_PAR2_LO16)

#define SMS_CB_LEN_IF_TYPE0 (EVENT_TYPE == 0 ? (EVENT_PAR1 == 0 ? 0 : 93) : 0)

#include "cfw_rpc_a2c_par.h"
#include "cfw_rpc_a2c_ap.c"
#include "cfw_rpc_c2a_par.h"
#include "cfw_rpc_c2a_ap.c"
