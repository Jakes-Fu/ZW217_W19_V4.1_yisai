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

#ifndef _RPC_DAEMON_H_
#define _RPC_DAEMON_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT
#include <os_api.h>
#endif



#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_RPC_PACKET_SIZE_MAX 768
#define CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT 0
#define CONFIG_RPC_EVENT_RANGE_COUNT 4

struct rpcChannel;
typedef struct rpcChannel rpcChannel_t;

typedef struct
{
    uint32_t opcode; // operation in RPC packet
    uint32_t size;   // the whole RPC packet size
} rpcHeader_t;

typedef struct
{
    rpcHeader_t h;
    uint32_t api_tag;        // tag for RPC function
    uint32_t caller_sync;    // sync primitive of caller, usually is a semaphore
    uint32_t caller_rsp_ptr; // response pointer of caller
    uint16_t seq;            // sequence number, just for debug
    uint16_t rsp_size;       // response size
} rpcCallHeader_t;

typedef struct
{
    rpcHeader_t h;
    uint32_t api_tag;        // tag for RPC function, copied from "call", just for debug
    uint32_t caller_sync;    // sync primitive of caller, copied from "call"
    uint32_t caller_rsp_ptr; // response pointer of caller
    uint16_t seq;            // sequence number, just for debug
    uint16_t rpc_error_code; // 0 or ENOENT(2)
} rpcRespHeader_t;

typedef struct
{
    rpcHeader_t h;
    uint32_t id; // it is cos event from here, just to reduce header file dependency
    uint32_t par1;
    uint32_t par2;
    uint32_t par3;
} rpcEventHeader_t;

#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT
typedef struct osiEvent
{
    uint32_t id;     ///< event identifier
    uint32_t param1; ///< 1st parameter
    uint32_t param2; ///< 2nd parameter
    uint32_t param3; ///< 3rd parameter
} osiEvent_t;

typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    uint32_t id;     ///< event identifier
    uint32_t param1; ///< 1st parameter
    uint32_t param2; ///< 2nd parameter
    uint32_t param3; ///< 3rd parameter
}OSIEvent_MSG_T ;

#endif

typedef void (*rpcEventRouter_t)(void *ctx, const void *event);
typedef void (*rpcFunction_t)(void *in, void *out);
typedef void (*rpcEventSender_t)(rpcChannel_t *ch, const void *event);
typedef bool (*rpcEventUnpacker_t)(rpcEventHeader_t *event);

/**
 * @brief open RPC channel
 *
 * Though only one RPC channel is designed now (that is the reason there
 * are no parameter at open), RPC channel pointer is used in all RPC APIs.
 *
 * RPC channel can be shared. So, it will return the same pointer at
 * further open.
 *
 * There are no "close" API. In current design, RPC channel won't be closed.
 *
 * @return the RPC channel pointer
 */
rpcChannel_t *rpcChannelOpen(void);

/**
 * @brief send an event to peer
 *
 * The event is generic event (4 words), and the first word MUST be event ID.
 * Sending method will depend on event ID.
 *
 * @param ch    the RPC channel
 * @param event the event to be send
 */
void rpcSendEvent(rpcChannel_t *ch, const void *event);

/**
 * @brief register event from peer router
 *
 * RPC daemon itself doesn't know how to route the event from peer.
 * Application should register router for specified event range.
 *
 * The router should only send the event to coresponding thread.
 * The router itself shouldn't handle the event.
 *
 * When range and callback are existed, no duplicated router will be added.
 * It is application's duty to decide whether overlapped ranges are permitted.
 *
 * @param ch        the RPC channel
 * @param start     event range start (including)
 * @param end       event range end (including)
 * @param router    event router
 * @return
 *      - true  registeration successed
 *      - false registeration failed, due to invalid parameters or
 *              there are too many routers.
 */
bool rpcRegisterEvents(rpcChannel_t *ch, uint32_t start, uint32_t end,
                       rpcEventRouter_t router, void *router_ctx);

/**
 * @brief send function call to peer
 *
 * The event is generic event (4 words), and the first word MUST be event ID.
 * Sending method will depend on event ID.
 *
 * Most likely, it won't be called directly by application. It will be
 * called only in RPC daemon and RPC stubs.
 *
 * @param ch    the RPC channel
 * @param call  the constructed call header
 * @return
 *      - 0         successed
 *      - others    error. Only when peer can't find the function, it will
 *                  return error.
 */
int rpcSendCall(rpcChannel_t *ch, rpcCallHeader_t *call);

/**
 * @brief send a packed event to peer
 *
 * When all content needed to send to peer are packed after the header, it
 * is "packed event". event->h.size is the data size needed to send to peer.
 *
 * RPC packets are 8 bytes aligned. However, it is permitted that event->h.size
 * is not 8 bytes aligned. In this case, the padding bytes will be sent to
 * peer.
 *
 * Most likely, it won't be called directly by application. It will be
 * called only in RPC daemon and RPC stubs.
 *
 * @param ch    the RPC channel
 * @param event the packed event
 */
void rpcSendPackedEvent(rpcChannel_t *ch, rpcEventHeader_t *event);

/**
 * @ brief send a plain event to peer
 *
 * "plain event" is event without pointer. The 4 words can be send to peer
 * directly.
 *
 * Most likely, it won't be called directly by application. It will be
 * called only in RPC daemon and RPC stubs.
 *
 * @param ch    the RPC channel
 * @param event the event to be send
 */
void rpcSendPlainEvent(rpcChannel_t *ch, const void *event);

/**
 * @brief send an event with pointer to peer
 *
 * "pointer event" is event with pointer. The content of the pointer shall
 * be sent to peer rather than the pointer itself. Also, the pointer should
 * be freed after the event is sent to peer (not after peer handled the event).
 *
 * When par1 or par2 is not a pointer, or the pointer is NULL, the parameter
 * ptr1_size or ptr2_size must be zero.
 *
 * 32 bits are used for pointer size in RPC packet due to packet format shall
 * be more stable, and 16 bits are used in the parameter due to it is enough.
 *
 * Though it is possible to get the allocated size of pointer, and event
 * the manual packer will use this method, it is not recommended programming
 * style here (can be regarded as a hack).
 *
 * Most likely, it won't be called directly by application. It will be
 * called only in RPC daemon and RPC stubs.
 *
 * @param ch        the RPC channel
 * @param event     the event to be send
 * @param ptr1_size the memory size when par1 is a pointer, otherwise 0
 * @param ptr2_size the memory size when par1 is a pointer, otherwise 0
 */
void rpcSendPointerEvent(rpcChannel_t *ch, const void *event,
                         uint32_t ptr1_size, uint32_t ptr2_size);

/**
 * @brief route the event from peer
 *
 * Search registered router, and call all matched routers.
 *
 * Most likely, it won't be called directly by application. It will be
 * called only in RPC daemon and RPC stubs (such as customized router).
 *
 * @param ch        the RPC channel
 * @param event     the unpacked event
 */
void rpcRouteEvent(rpcChannel_t *ch, rpcEventHeader_t *event);

/**
 * @brief unpack pointer event
 *
 * Unpack pointer event from peer. The event itself is "packed" event.
 * The pointer content will be copied to local malloc memory.
 *
 * Most likely, it won't be called directly by application. It will be
 * called only in RPC daemon and RPC stubs.
 *
 * @param ch    the RPC channel
 * @param event the event to be send
 * @return
 *      -true   successed
 *      -false  unpack fail, may due to incorrect event header, or
 *              malloc failed.
 */
bool rpcUnpackPointerEvent(rpcEventHeader_t *event,
                           uint32_t ptr1_size, uint32_t ptr2_size);
/**
 * @brief get supported api tag count by peer
 *
 * @return supported api tag count locally
 */

#ifdef CONFIG_ANTIREG_LICENSE_SUPPORT
extern uint32_t PCNgisYfirev(uint8_t input[50]);
#endif

#ifdef __cplusplus
}
#endif
#endif
