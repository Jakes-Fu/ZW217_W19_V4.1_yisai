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

#include "rpc_daemon.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
// #include "md_ipc.h"
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include "deepsleep_drvapi.h"
#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT
#include "antireg.h"
//#include "hal_config.h"
#include "cfw_event.h"
//#include "kernel_config.h"
#endif
#include "cmddef.h"

#define SMD_CH_AT2           2
#define CH_READ_AVAIL        0x1
#define CH_WRITE_AVAIL       0x2
#define CH_OPENED            0x4
#define CH_RW_MASK           (CH_READ_AVAIL | CH_WRITE_AVAIL)

struct smd_ch;
extern int32 ipc_ch_open(int32 ch_id, struct smd_ch **ch, void *priv,
			void (*notify)(void *, uint32));
extern int32 ipc_ch_read(struct smd_ch *ch, void *data, uint32 len);
extern int32 ipc_ch_write(struct smd_ch *ch, const void *data, uint32 len);
extern int32 ipc_ch_close(struct smd_ch *ch);
extern int32 ipc_ch_read_avail(struct smd_ch *ch);
extern int32 ipc_ch_write_avail(struct smd_ch *ch);

#define MAKE_TAG(a, b, c, d) ((unsigned)(a) | ((unsigned)(b) << 8) | ((unsigned)(c) << 16) | ((unsigned)(d) << 24))

#define RPC_CODE_CALL MAKE_TAG('C', 'A', 'L', 'L')
#define RPC_CODE_RSP MAKE_TAG('R', 'E', 'S', 'P')
#define RPC_CODE_EVENT MAKE_TAG('E', 'V', 'N', 'T')

#define ALIGN_UP(v, a) (((v) + (a)-1) & ~((a)-1))

#define RPC_THREAD_PRIORITY (20)
#define RPC_STACK_SIZE (2048)
#define RPC_EVENT_QUEUE_SIZE (32)

#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT
#define DAEMON_THREAD_PRIORITY (RPC_THREAD_PRIORITY)
#define DAEMON_STACK_SIZE (1024 * 4)
#define DAEMON_EVENT_QUEUE_SIZE (4)
#endif

extern void *rpcFindFunctionByTag(unsigned tag); // auto generated
extern void *rpcFindEventSender(unsigned id);    // auto generated
extern void *rpcFindEventUnpacker(unsigned id);  // auto generated

typedef struct
{
    SIGNAL_VARS
    uint32_t event;
} rpcEventSig_t;

#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT
static BLOCK_ID gDaemonTaskID = SCI_INVALID_BLOCK_ID;
static void _daemonTaskEntry(void *param)
{
    uint32_t olen = 0;
    uint8_t aout[50] = {0};

    BLOCK_ID task_id = SCI_IdentifyThread();
    xSignalHeader sig_ptr	= SCI_NULL;
    osiEvent_t *msg_head_ptr = SCI_NULL;

    for (;;)
    {
        sig_ptr = SCI_GetSignal(task_id);
        if (SCI_NULL == sig_ptr)    continue;
        msg_head_ptr = (osiEvent_t *)(sig_ptr + 1);
        if (msg_head_ptr->id == EV_DM_DAEMON_IND)
        {
            SCI_TRACE_LOW("RPC: ItnaNgisNeg start");
            olen = ItnaNgisNeg((uint8_t *)msg_head_ptr->param1, aout);
            free((void *)msg_head_ptr->param1);
            SCI_TRACE_LOW("[ANTIREG] result[%ld]: %s", olen, aout);
            PCNgisYfirev(aout);
            SCI_FREE(sig_ptr);
        }
    }
}
#endif


struct rpcChannel
{
    struct smd_ch *ch;
    SCI_THREAD_PTR task_id;
    SCI_MUTEX_PTR lock;
    SCI_SEMAPHORE_PTR call_sync;
    uint32_t seq;
    int range_count;
    struct
    {
        uint32_t start;
        uint32_t end;
        rpcEventRouter_t router;
        void *ctx;
    } ranges[CONFIG_RPC_EVENT_RANGE_COUNT];
    uint64_t recv_ipar[CONFIG_RPC_PACKET_SIZE_MAX / 8];
    uint64_t recv_opar[CONFIG_RPC_PACKET_SIZE_MAX / 8];
};

static uint8 gMutex = 0;

static rpcChannel_t *gRpcChannel = NULL;

static void _taskRead(rpcChannel_t *p)
{
    for (;;)
    {
        int avail = ipc_ch_read_avail(p->ch);
        uint32 status;
        rpcHeader_t *h;
        rpcRespHeader_t *rsp;
        rpcRespHeader_t *real_rsp;
        rpcCallHeader_t *call;
        size_t rsp_data_size;
        rpcFunction_t func;
        rpcEventHeader_t *event;
        rpcEventUnpacker_t unpacker;

        if (avail < sizeof(rpcHeader_t))
            return;

        h = (rpcHeader_t *)&p->recv_ipar[0];
        ipc_ch_read(p->ch, h, sizeof(rpcHeader_t));
        SCI_ASSERT(avail >= h->size);

        if (h->opcode == RPC_CODE_RSP)
        {
            SCI_ASSERT(h->size >= sizeof(rpcRespHeader_t));

            rsp = (rpcRespHeader_t *)&p->recv_ipar[0];
            ipc_ch_read(p->ch, (char *)rsp + sizeof(rpcHeader_t),
                        sizeof(rpcRespHeader_t) - sizeof(rpcHeader_t));

            real_rsp = (rpcRespHeader_t *)rsp->caller_rsp_ptr;
            SCI_TRACE_LOW("RPC: recv response tag/0x%08x(%s) rsp/0x%p, seq/0x%x", rsp->api_tag, rpcFindNameByTag(rsp->api_tag), real_rsp, rsp->seq);
            *real_rsp = *rsp;

            rsp_data_size = real_rsp->h.size - sizeof(rpcRespHeader_t);
            if (rsp_data_size > 0)
                ipc_ch_read(p->ch, (char *)real_rsp + sizeof(rpcRespHeader_t), rsp_data_size);

            SCI_PutSemaphore((SCI_SEMAPHORE_PTR)real_rsp->caller_sync);
        }
        else if (h->opcode == RPC_CODE_CALL)
        {
            SCI_ASSERT(h->size >= sizeof(rpcCallHeader_t));

            call = (rpcCallHeader_t *)&p->recv_ipar[0];
            ipc_ch_read(p->ch, (char *)call + sizeof(rpcHeader_t),
                        h->size - sizeof(rpcHeader_t));

            rsp = (rpcRespHeader_t *)&p->recv_opar[0];
            rsp->h.opcode = RPC_CODE_RSP;
            rsp->h.size = call->rsp_size;
            rsp->api_tag = call->api_tag;
            rsp->caller_rsp_ptr = call->caller_rsp_ptr;
            rsp->caller_sync = call->caller_sync;
            rsp->seq = call->seq;
            rsp->rpc_error_code = 2; // ENOENT;

            func = (rpcFunction_t)rpcFindFunctionByTag(call->api_tag);
            SCI_TRACE_LOW("RPC: recv call tag/0x%08x(%s) func/0x%p, seq/0x%x", call->api_tag, rpcFindNameByTag(rsp->api_tag), func, call->seq);
            if (func != NULL)
            {
                rsp->rpc_error_code = 0;
                func((char *)call + sizeof(rpcCallHeader_t),
                     (char *)rsp + sizeof(rpcRespHeader_t));
            }
	    else {
		SCI_TraceLow("rpc: no find function by tag 0x%x \n",call->api_tag);
	    }

            //SCI_GetMutex(p->lock, SCI_WAIT_FOREVER);
            SCI_DISABLE_IRQ
            ipc_ch_write(p->ch, rsp, rsp->h.size);
            //SCI_PutMutex(p->lock);
            SCI_RESTORE_IRQ
        }
        else if (h->opcode == RPC_CODE_EVENT)
        {
            SCI_ASSERT(h->size >= sizeof(rpcEventHeader_t));

            event = (rpcEventHeader_t *)&p->recv_ipar[0];
            ipc_ch_read(p->ch, (char *)event + sizeof(rpcHeader_t),
                        h->size - sizeof(rpcHeader_t));

            unpacker = rpcFindEventUnpacker(event->id);
            if (unpacker != NULL)
                unpacker(event);
            SCI_TRACE_LOW("RPC: receive event from cp %d/0x%08x/0x%08x/0x%08x", event->id, event->par1, event->par2, event->par3);


#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT
            if (event->id == EV_DM_DAEMON_IND)
            {
                OSIEvent_MSG_T* reve = SCI_NULL;
                reve = (OSIEvent_MSG_T *)SCI_ALLOC_APP(sizeof(OSIEvent_MSG_T));
                if (reve == SCI_NULL) {
                    SCI_ASSERT(0);
                }
                SCI_MEMSET(reve, 0, sizeof(OSIEvent_MSG_T));
                //osiEvent_t reve;
                reve->SignalSize = sizeof(OSIEvent_MSG_T);
                reve->Sender = SCI_IdentifyThread();
                reve->id = event->id;
                reve->param1 = event->par1;
                SCI_TRACE_LOW("RPC: PCEtaercNgis done, send event to RPC");
                //osiEventSend(gDaemonTaskID, &reve);
                status = SCI_SendSignal((xSignalHeader)reve, gDaemonTaskID);
            }
            else
#endif
            rpcRouteEvent(p, event);
        }
        else
        {
            SCI_ASSERT(FALSE);
        }
    }
}

static void _taskWrite(rpcChannel_t *p)
{
    // do nothing
}

static void _notify(void *priv, uint32_t event)
{
    rpcChannel_t *p = (rpcChannel_t *)priv;
    rpcEventSig_t *event_sig = NULL;

    SCI_TRACE_LOW("RPC notif 0x%08x", event);
    event_sig = (rpcEventSig_t *)SCI_ALLOC_APP(sizeof(rpcEventSig_t));
    SCI_ASSERT(SCI_NULL != event_sig);

    event_sig->SignalSize = sizeof(rpcEventSig_t);
    event_sig->Sender = SCI_IdentifyThread();
    event_sig->event = event;
    SCI_SendSignal((xSignalHeader)event_sig, p->task_id);
}

void _taskEntry(uint32_t argc, void *argv)
{
    rpcChannel_t *p = (rpcChannel_t *)argv;
    rpcEventSig_t *event_sig = NULL;
	CFW_RpcRegisterEvents();

	if(CALIBRATION_MODE != POWER_GetResetMode())
		SCI_GetMutex(p->lock, SCI_WAIT_FOREVER);
   gMutex = 1;
    for (;;)
    {
        event_sig = (rpcEventSig_t *)SCI_GetSignal(p->task_id);
        if (event_sig->event & CH_READ_AVAIL)
            _taskRead(p);
        if (event_sig->event & CH_WRITE_AVAIL)
            _taskWrite(p);
        SCI_FREE(event_sig);
    }
}

void rpcCpInitDone(void)
{
	if(gMutex && gRpcChannel)
	{
		SCI_PutMutex(gRpcChannel->lock);
		gMutex = 0;
	}
}
rpcChannel_t *rpcChannelOpen(void)
{
    rpcChannel_t *p;
    int res;

    if (gRpcChannel != NULL)
        return gRpcChannel;

    // COS_LOGI(0, "RPC channel open");
    p = (rpcChannel_t *)calloc(1, sizeof(rpcChannel_t));
    if (p == NULL)
        return NULL;

    p->lock = SCI_CreateMutex("rpc_lock", SCI_INHERIT);
    p->call_sync = SCI_CreateSemaphore("rpc_sync", 0);

    p->task_id = SCI_CreateThread(
                    "rpc_task",
                    "rpc_queue",
                    _taskEntry,
                    1,
                    p,
                    RPC_STACK_SIZE,
                    RPC_EVENT_QUEUE_SIZE,
                    RPC_THREAD_PRIORITY,
                    SCI_PREEMPT,
                    SCI_AUTO_START);

#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT

    gDaemonTaskID = SCI_CreateThread(
                    "daemon_task",
                    "daemon_queue",
                    _daemonTaskEntry,
                    0,
                    NULL,
                    DAEMON_STACK_SIZE,
                    DAEMON_EVENT_QUEUE_SIZE,
                    DAEMON_THREAD_PRIORITY,
                    SCI_PREEMPT,
                    SCI_AUTO_START);

#endif


    res = ipc_ch_open(SMD_CH_AT2, &p->ch, p, _notify);
    SCI_ASSERT(res >= 0);

    // TODO: md_ipc.h can'r be included now
    // p->ch->event_mask = CH_READ_AVAIL;
    // ipc_enable_ravail_event(p->ch);
    gRpcChannel = p;
    return gRpcChannel;
}

int rpcSendCall(rpcChannel_t *p, rpcCallHeader_t *call)
{
    uint32_t tag;
    uint16_t seq;
    rpcRespHeader_t *rsp;

    SCI_GetMutex(p->lock, SCI_WAIT_FOREVER);
    tag = call->api_tag;
    seq = p->seq++;

    call->h.size = ALIGN_UP(call->h.size, 8);
    call->h.opcode = RPC_CODE_CALL;
    call->caller_sync = (uint32_t)p->call_sync;
    call->seq = seq;

    SCI_TRACE_LOW("RPC: send call 0x%08x(%s), seq/0x%x", tag, rpcFindNameByTag(tag), seq);
    SCI_DISABLE_IRQ
    ipc_ch_write(p->ch, call, call->h.size);
    SCI_RESTORE_IRQ

    SCI_RPC_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    SCI_GetSemaphore(p->call_sync, SCI_WAIT_FOREVER);

    SCI_RPC_EnableDeepSleep(ENABLE_DEEP_SLEEP);

    SCI_PutMutex(p->lock);

    rsp = (rpcRespHeader_t *)call->caller_rsp_ptr;
    if (rsp->rpc_error_code != 0)
        ; // COS_LOGE(0, "RPC send call get error code %d from peer", rsp->rpc_error_code);
    return (rsp->rpc_error_code == 0) ? 0 : -2 /*-ENOENT*/;
}

void rpcSendPackedEvent(rpcChannel_t *p, rpcEventHeader_t *event)
{
    SCI_GetMutex(p->lock, SCI_WAIT_FOREVER);
    SCI_TRACE_LOW("RPC: send event id/%d size/%d", event->id, event->h.size);

    event->h.opcode = RPC_CODE_EVENT;
    event->h.size = ALIGN_UP(event->h.size, 8);
    SCI_DISABLE_IRQ
    ipc_ch_write(p->ch, event, event->h.size);
    SCI_RESTORE_IRQ
    SCI_PutMutex(p->lock);
}

void rpcSendPlainEvent(rpcChannel_t *p, const void *event)
{
    const uint32_t *in = (const uint32_t *)event;
    rpcEventHeader_t packed;
    packed.h.size = sizeof(rpcEventHeader_t);
    packed.id = in[0];
    packed.par1 = in[1];
    packed.par2 = in[2];
    packed.par3 = in[3];
    rpcSendPackedEvent(p, &packed);
}

void rpcSendPointerEvent(rpcChannel_t *p, const void *event,
                         uint32_t ptr1_size, uint32_t ptr2_size)
{
    size_t mem_size = sizeof(rpcEventHeader_t) + ptr1_size + ptr2_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
                    ? SCI_ALLOC_APP(mem_size)
                    : alloca(mem_size);

    const uint32_t *in = (const uint32_t *)event;
    rpcEventHeader_t *packed = (rpcEventHeader_t *)mem;
    packed->h.opcode = RPC_CODE_EVENT;
    packed->h.size = mem_size;
    packed->id = in[0];
    packed->par1 = in[1];
    packed->par2 = in[2];
    packed->par3 = in[3];

    if (ptr1_size != 0 && (void *)packed->par1 != NULL)
    {
        memcpy((char *)mem + sizeof(rpcEventHeader_t),
               (void *)packed->par1, ptr1_size);
        SCI_FREE((void *)packed->par1);
    }
    if (ptr2_size != 0 && (void *)packed->par2 != NULL)
    {
        memcpy((char *)mem + sizeof(rpcEventHeader_t) + ptr1_size,
               (void *)packed->par2, ptr2_size);
        SCI_FREE((void *)packed->par2);
    }

    rpcSendPackedEvent(p, (rpcEventHeader_t *)packed);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
    {
        SCI_FREE(mem);
    }
}

bool rpcUnpackPointerEvent(rpcEventHeader_t *event,
                           uint32_t ptr1_size, uint32_t ptr2_size)
{
    char *in_mem = (char *)event + sizeof(rpcEventHeader_t);
    void *mem1 = NULL;
    void *mem2 = NULL;

    if (event->h.size != ALIGN_UP(sizeof(rpcEventHeader_t) + ptr1_size + ptr2_size, 8))
        return false;

    if (ptr1_size != 0)
    {
        mem1 = SCI_ALLOC_APP(ptr1_size);
        if (mem1 == NULL)
            return false;

        memcpy(mem1, in_mem, ptr1_size);
        event->par1 = (uint32_t)mem1;
        in_mem += ptr1_size;
    }
    if (ptr2_size != 0)
    {
        mem2 = SCI_ALLOC_APP(ptr2_size);
        if (mem2 == NULL)
        {
            SCI_FREE(mem1);
            return false;
        }
        memcpy(mem2, in_mem, ptr2_size);
        event->par2 = (uint32_t)mem2;
        in_mem += ptr2_size;
    }

    return true;
}

void rpcSendEvent(rpcChannel_t *p, const void *event)
{
    uint32_t id = *(const uint32_t *)event;
    rpcEventSender_t sender = rpcFindEventSender(id);
    if (sender != NULL)
        sender(p, event);
    else
        rpcSendPlainEvent(p, event);
}

void rpcRouteEvent(rpcChannel_t *p, rpcEventHeader_t *event)
{
    int n;
    for (n = 0; n < CONFIG_RPC_EVENT_RANGE_COUNT; n++)
    {
        if (event->id >= p->ranges[n].start &&
            event->id <= p->ranges[n].end &&
            p->ranges[n].router != NULL)
            p->ranges[n].router(p->ranges[n].ctx, &event->id);
    }
}

bool rpcRegisterEvents(rpcChannel_t *p, uint32_t start, uint32_t end,
                       rpcEventRouter_t router, void *router_ctx)
{
    int n;
    if (router == NULL || start > end)
        return false;

    SCI_GetMutex(p->lock, SCI_WAIT_FOREVER);
    for (n = 0; n < CONFIG_RPC_EVENT_RANGE_COUNT; n++)
    {
        if (p->ranges[n].start == start &&
            p->ranges[n].end == end &&
            p->ranges[n].router == router &&
            p->ranges[n].ctx == router_ctx)
        {
            SCI_PutMutex(p->lock);
            return true;
        }
    }

    for (n = 0; n < CONFIG_RPC_EVENT_RANGE_COUNT; n++)
    {
        if (p->ranges[n].router == NULL)
        {
            p->ranges[n].start = start;
            p->ranges[n].end = end;
            p->ranges[n].router = router;
            p->ranges[n].ctx = router_ctx;
            SCI_PutMutex(p->lock);
            return true;
        }
    }

    SCI_PutMutex(p->lock);
    return false;
}
