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

// AUTO GENERATED

#ifndef DISABLE_RPC_rpcHelloOnCP
uint32_t rpcHelloOnCP(uint32_t n)
{
    uint32_t ivar_size, ovar_size, call_size, rsp_size, mem_size;

    void *mem;
    rpcCallHeader_t *call;
    int rpc_result;
    struct IPAR_rpcHelloOnCP *ipar;
    struct OPAR_rpcHelloOnCP *opar;
    uint32_t result;


    ivar_size = 0;
    ovar_size = 0;
    call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_rpcHelloOnCP) + ivar_size;
    rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_rpcHelloOnCP) + ovar_size;
    mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    call = (rpcCallHeader_t *)mem;
    ipar = (struct IPAR_rpcHelloOnCP *)((char *)mem + sizeof(rpcCallHeader_t));
    opar = (struct OPAR_rpcHelloOnCP *)((char *)mem + sizeof(rpcRespHeader_t));
    call->h.size = call_size;
    call->api_tag = TAG_rpcHelloOnCP;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;
    ipar->n = n;
    rpc_result = rpcSendCall(RPC_CHANNEL, call);
    (void)rpc_result;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
#endif

#ifndef DISABLE_RPC_rpcPeerTagSupported
bool rpcPeerTagSupported(uint32_t tag)
{
    uint32_t ivar_size, ovar_size, call_size, rsp_size, mem_size;

    void *mem;
    rpcCallHeader_t *call;
    int rpc_result;
    struct IPAR_rpcPeerTagSupported *ipar;
    struct OPAR_rpcPeerTagSupported *opar;
    bool result;


    ivar_size = 0;
    ovar_size = 0;
    call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_rpcPeerTagSupported) + ivar_size;
    rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_rpcPeerTagSupported) + ovar_size;
    mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    call = (rpcCallHeader_t *)mem;
    ipar = (struct IPAR_rpcPeerTagSupported *)((char *)mem + sizeof(rpcCallHeader_t));
    opar = (struct OPAR_rpcPeerTagSupported *)((char *)mem + sizeof(rpcRespHeader_t));
    call->h.size = call_size;
    call->api_tag = TAG_rpcPeerTagSupported;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;
    ipar->tag = tag;
    rpc_result = rpcSendCall(RPC_CHANNEL, call);
    (void)rpc_result;
    result = opar->result;
    if (rpc_result == -2 /*ENOENT*/)
        result = false;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
#endif

#ifndef DISABLE_RPC_rpcPeerTagCount
int rpcPeerTagCount(void)
{
    uint32_t ivar_size, ovar_size, call_size, rsp_size, mem_size;

    void *mem;
    rpcCallHeader_t *call;
    int rpc_result;
    struct OPAR_rpcPeerTagCount *opar;
    int result;


    ivar_size = 0;
    ovar_size = 0;
    call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_rpcPeerTagCount) + ovar_size;
    mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    call = (rpcCallHeader_t *)mem;
    opar = (struct OPAR_rpcPeerTagCount *)((char *)mem + sizeof(rpcRespHeader_t));
    call->h.size = call_size;
    call->api_tag = TAG_rpcPeerTagCount;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpc_result = rpcSendCall(RPC_CHANNEL, call);
    (void)rpc_result;
    result = opar->result;
    if (rpc_result == -2 /*ENOENT*/)
        result = -1;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
#endif

#ifndef DISABLE_RPC_rpcGetPeerTags
int rpcGetPeerTags(uint32_t* tags, uint32_t offset, uint32_t count)
{
    uint32_t ivar_size, ovar_size, call_size, rsp_size, mem_size;

    void *mem;
    rpcCallHeader_t *call;
    int rpc_result;
    struct IPAR_rpcGetPeerTags *ipar;
    struct OPAR_rpcGetPeerTags *opar;
    char *ovar_ptr;
    int result;


    ivar_size = 0;
    ovar_size = ALIGNUP8(count*4+1);
    call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_rpcGetPeerTags) + ivar_size;
    rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_rpcGetPeerTags) + ovar_size;
    mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    call = (rpcCallHeader_t *)mem;
    ipar = (struct IPAR_rpcGetPeerTags *)((char *)mem + sizeof(rpcCallHeader_t));
    opar = (struct OPAR_rpcGetPeerTags *)((char *)mem + sizeof(rpcRespHeader_t));
    ovar_ptr = (char *)mem + sizeof(rpcRespHeader_t) + sizeof(struct OPAR_rpcGetPeerTags);
    call->h.size = call_size;
    call->api_tag = TAG_rpcGetPeerTags;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;
    ipar->offset = offset;
    ipar->count = count;
    rpc_result = rpcSendCall(RPC_CHANNEL, call);
    (void)rpc_result;
    memcpy(tags, ovar_ptr, count*4);
    ovar_ptr[count*4] = (char)0;
    ovar_ptr += ALIGNUP8(count*4 + 1);
    result = opar->result;
    if (rpc_result == -2 /*ENOENT*/)
        result = -1;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
#endif

// AUTO GENERATED END
