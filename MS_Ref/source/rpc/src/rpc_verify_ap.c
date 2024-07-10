// AUTO GENERATED

uint8 verify_ApSide(ap_pkt_t *verifycode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_verify_ApSide) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_verify_ApSide);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_verify_ApSide *ipar = (struct IPAR_verify_ApSide *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_verify_ApSide *opar = (struct OPAR_verify_ApSide *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8 result;

    call->h.size = call_size;
    call->api_tag = TAG_verify_ApSide;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->verifycode = *verifycode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8 verify_CpSide(ap_pkt_t *verifycode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_verify_CpSide);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_verify_CpSide *opar = (struct OPAR_verify_CpSide *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8 result;

    call->h.size = call_size;
    call->api_tag = TAG_verify_CpSide;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *verifycode = opar->verifycode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8 verify_Cp_result(uint8 result)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_verify_Cp_result) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_verify_Cp_result);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_verify_Cp_result *ipar = (struct IPAR_verify_Cp_result *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_verify_Cp_result *opar = (struct OPAR_verify_Cp_result *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8 result1;

    call->h.size = call_size;
    call->api_tag = TAG_verify_Cp_result;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->result = result;
    rpcSendCall(RPC_CHANNEL, call);

    result1 = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result1;
}


// AUTO GENERATED END
