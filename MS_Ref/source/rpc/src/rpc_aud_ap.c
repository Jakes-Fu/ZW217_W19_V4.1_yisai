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

AUD_ERR_T aud_StreamStart(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_StreamStart) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_StreamStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_StreamStart *ipar = (struct IPAR_aud_StreamStart *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_StreamStart *opar = (struct OPAR_aud_StreamStart *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_StreamStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->stream = *stream;
    ipar->cfg = *cfg;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_StreamStop(SND_ITF_T itf)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_StreamStop) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_StreamStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_StreamStop *ipar = (struct IPAR_aud_StreamStop *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_StreamStop *opar = (struct OPAR_aud_StreamStop *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_StreamStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_Setup(SND_ITF_T itf, AUD_LEVEL_T *cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_Setup) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_Setup);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_Setup *ipar = (struct IPAR_aud_Setup *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_Setup *opar = (struct OPAR_aud_Setup *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_Setup;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->cfg = *cfg;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_StreamRecord(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_StreamRecord) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_StreamRecord);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_StreamRecord *ipar = (struct IPAR_aud_StreamRecord *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_StreamRecord *opar = (struct OPAR_aud_StreamRecord *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_StreamRecord;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->stream = *stream;
    ipar->cfg = *cfg;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_StreamRecordStop(SND_ITF_T itf)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_StreamRecordStop) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_StreamRecordStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_StreamRecordStop *ipar = (struct IPAR_aud_StreamRecordStop *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_StreamRecordStop *opar = (struct OPAR_aud_StreamRecordStop *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_StreamRecordStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

HAL_ERR_T hal_zspMusicPlayStart(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_hal_zspMusicPlayStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_hal_zspMusicPlayStart *opar = (struct OPAR_hal_zspMusicPlayStart *)((char*)mem + sizeof(rpcRespHeader_t));
    HAL_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_hal_zspMusicPlayStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

HAL_ERR_T hal_zspMusicPlayStop(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_hal_zspMusicPlayStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_hal_zspMusicPlayStop *opar = (struct OPAR_hal_zspMusicPlayStop *)((char*)mem + sizeof(rpcRespHeader_t));
    HAL_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_hal_zspMusicPlayStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void aud_SetAudDeviceCFG(AUD_DEVICE_CFG_EXT_T cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_SetAudDeviceCFG) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_SetAudDeviceCFG *ipar = (struct IPAR_aud_SetAudDeviceCFG *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_aud_SetAudDeviceCFG;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->cfg = cfg;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

VOIS_ERR_T vois_SetCfg(AUD_ITF_T itf, VOIS_AUDIO_CFG_T *cfg, AUD_DEVICE_CFG_EXT_T devicecfg, SND_BT_WORK_MODE_T btworkmode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_vois_SetCfg) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_vois_SetCfg);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_vois_SetCfg *ipar = (struct IPAR_vois_SetCfg *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_vois_SetCfg *opar = (struct OPAR_vois_SetCfg *)((char*)mem + sizeof(rpcRespHeader_t));
    VOIS_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_vois_SetCfg;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->cfg = *cfg;
    ipar->devicecfg = devicecfg;
    ipar->btworkmode = btworkmode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void vois_SetBtNRECFlag(UINT32 flag)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_vois_SetBtNRECFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_vois_SetBtNRECFlag *ipar = (struct IPAR_vois_SetBtNRECFlag *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_vois_SetBtNRECFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->flag = flag;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

AUD_ERR_T vois_TestModeSetup(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg, AUD_TEST_T mode, UINT32 voismode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_vois_TestModeSetup) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_vois_TestModeSetup);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_vois_TestModeSetup *ipar = (struct IPAR_vois_TestModeSetup *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_vois_TestModeSetup *opar = (struct OPAR_vois_TestModeSetup *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_vois_TestModeSetup;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->stream = *stream;
    ipar->cfg = *cfg;
    ipar->mode = mode;
    ipar->voismode = voismode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

unsigned int DSPExt_GetVoiceCoderType(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DSPExt_GetVoiceCoderType);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DSPExt_GetVoiceCoderType *opar = (struct OPAR_DSPExt_GetVoiceCoderType *)((char*)mem + sizeof(rpcRespHeader_t));
    unsigned int  result;

    call->h.size = call_size;
    call->api_tag = TAG_DSPExt_GetVoiceCoderType;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_StartAudioEx(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_StartAudioEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_StartAudioEx *opar = (struct OPAR_DM_StartAudioEx *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_StartAudioEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_RestartAudioEx(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_RestartAudioEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_RestartAudioEx *opar = (struct OPAR_DM_RestartAudioEx *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_RestartAudioEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_StopAudioEx(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_StopAudioEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_StopAudioEx *opar = (struct OPAR_DM_StopAudioEx *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_StopAudioEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_PlayToneEx(UINT8 nToneType, UINT8 nToneAttenuation, UINT16 nDuration, DM_SpeakerGain volumn)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_PlayToneEx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_PlayToneEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_PlayToneEx *ipar = (struct IPAR_DM_PlayToneEx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_PlayToneEx *opar = (struct OPAR_DM_PlayToneEx *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_PlayToneEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nToneType = nToneType;
    ipar->nToneAttenuation = nToneAttenuation;
    ipar->nDuration = nDuration;
    ipar->volumn = volumn;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_StopToneEx(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_StopToneEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_StopToneEx *opar = (struct OPAR_DM_StopToneEx *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_StopToneEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

HAL_ERR_T hal_zspVoiceRecordStart(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_hal_zspVoiceRecordStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_hal_zspVoiceRecordStart *opar = (struct OPAR_hal_zspVoiceRecordStart *)((char*)mem + sizeof(rpcRespHeader_t));
    HAL_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_hal_zspVoiceRecordStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

HAL_ERR_T hal_zspVoiceRecordStop(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_hal_zspVoiceRecordStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_hal_zspVoiceRecordStop *opar = (struct OPAR_hal_zspVoiceRecordStop *)((char*)mem + sizeof(rpcRespHeader_t));
    HAL_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_hal_zspVoiceRecordStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_SetBtClk(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_SetBtClk);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_aud_SetBtClk *opar = (struct OPAR_aud_SetBtClk *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_SetBtClk;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_ClrBtClk(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_ClrBtClk);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_aud_ClrBtClk *opar = (struct OPAR_aud_ClrBtClk *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_ClrBtClk;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_AudStreamStart(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudStreamStart) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_AudStreamStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudStreamStart *ipar = (struct IPAR_DM_AudStreamStart *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_AudStreamStart *opar = (struct OPAR_DM_AudStreamStart *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudStreamStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->stream = *stream;
    ipar->cfg = *cfg;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_AudStreamStop(SND_ITF_T itf)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudStreamStop) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_AudStreamStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudStreamStop *ipar = (struct IPAR_DM_AudStreamStop *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_AudStreamStop *opar = (struct OPAR_DM_AudStreamStop *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudStreamStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_AudSetup(SND_ITF_T itf, AUD_LEVEL_T *cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudSetup) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_AudSetup);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudSetup *ipar = (struct IPAR_DM_AudSetup *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_AudSetup *opar = (struct OPAR_DM_AudSetup *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudSetup;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->cfg = *cfg;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_AudStreamRecord(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudStreamRecord) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_AudStreamRecord);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudStreamRecord *ipar = (struct IPAR_DM_AudStreamRecord *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_AudStreamRecord *opar = (struct OPAR_DM_AudStreamRecord *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudStreamRecord;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->stream = *stream;
    ipar->cfg = *cfg;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_VoisSetCfg(AUD_ITF_T itf, VOIS_AUDIO_CFG_T *cfg, AUD_DEVICE_CFG_EXT_T devicecfg, SND_BT_WORK_MODE_T btworkmode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_VoisSetCfg) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_VoisSetCfg);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_VoisSetCfg *ipar = (struct IPAR_DM_VoisSetCfg *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_VoisSetCfg *opar = (struct OPAR_DM_VoisSetCfg *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_VoisSetCfg;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->cfg = *cfg;
    ipar->devicecfg = devicecfg;
    ipar->btworkmode = btworkmode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_AudSetAudDeviceCFG(AUD_DEVICE_CFG_EXT_T cfg)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudSetAudDeviceCFG) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_AudSetAudDeviceCFG);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudSetAudDeviceCFG *ipar = (struct IPAR_DM_AudSetAudDeviceCFG *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_AudSetAudDeviceCFG *opar = (struct OPAR_DM_AudSetAudDeviceCFG *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudSetAudDeviceCFG;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->cfg = cfg;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_VoisSetBtNRECFlag(UINT32 flag)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_VoisSetBtNRECFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_VoisSetBtNRECFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_VoisSetBtNRECFlag *ipar = (struct IPAR_DM_VoisSetBtNRECFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_VoisSetBtNRECFlag *opar = (struct OPAR_DM_VoisSetBtNRECFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_VoisSetBtNRECFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->flag = flag;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_VoisTestModeSetup(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg, AUD_TEST_T mode, UINT32 voismode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_VoisTestModeSetup) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_VoisTestModeSetup);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_VoisTestModeSetup *ipar = (struct IPAR_DM_VoisTestModeSetup *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_VoisTestModeSetup *opar = (struct OPAR_DM_VoisTestModeSetup *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_VoisTestModeSetup;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    ipar->stream = *stream;
    ipar->cfg = *cfg;
    ipar->mode = mode;
    ipar->voismode = voismode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_HeadsetConfig(UINT32 isInsert, UINT32 isBbat)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_HeadsetConfig) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_HeadsetConfig);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_HeadsetConfig *ipar = (struct IPAR_aud_HeadsetConfig *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_HeadsetConfig *opar = (struct OPAR_aud_HeadsetConfig *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_HeadsetConfig;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isInsert = isInsert;
    ipar->isBbat = isBbat;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_ZspMusicPlayStart(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspMusicPlayStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_ZspMusicPlayStart *opar = (struct OPAR_DM_ZspMusicPlayStart *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspMusicPlayStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_ZspMusicPlayStop(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspMusicPlayStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_ZspMusicPlayStop *opar = (struct OPAR_DM_ZspMusicPlayStop *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspMusicPlayStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void aud_SetSharkingMode(UINT32 isSharkingMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_SetSharkingMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_SetSharkingMode *ipar = (struct IPAR_aud_SetSharkingMode *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_aud_SetSharkingMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isSharkingMode = isSharkingMode;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

AUD_ERR_T aud_bbatPcmBufferPlayStart(SND_ITF_T itf, BBAT_PCM_STREAM_T * playbuffer)
{
    size_t ivar_size = ALIGNUP8(sizeof(BBAT_PCM_STREAM_T) + playbuffer->uiLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_bbatPcmBufferPlayStart) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_bbatPcmBufferPlayStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_bbatPcmBufferPlayStart *ipar = (struct IPAR_aud_bbatPcmBufferPlayStart *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_bbatPcmBufferPlayStart *opar = (struct OPAR_aud_bbatPcmBufferPlayStart *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_bbatPcmBufferPlayStart);
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_bbatPcmBufferPlayStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    memcpy(ivar_ptr, playbuffer, sizeof(BBAT_PCM_STREAM_T) + playbuffer->uiLength);
    ivar_ptr[sizeof(BBAT_PCM_STREAM_T) + playbuffer->uiLength] = (char)0;
    ivar_ptr += ALIGNUP8(sizeof(BBAT_PCM_STREAM_T) + playbuffer->uiLength + 1);
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

AUD_ERR_T aud_bbatPcmBufferPlayStop(SND_ITF_T itf)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_aud_bbatPcmBufferPlayStop) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_aud_bbatPcmBufferPlayStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_aud_bbatPcmBufferPlayStop *ipar = (struct IPAR_aud_bbatPcmBufferPlayStop *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_aud_bbatPcmBufferPlayStop *opar = (struct OPAR_aud_bbatPcmBufferPlayStop *)((char*)mem + sizeof(rpcRespHeader_t));
    AUD_ERR_T result;

    call->h.size = call_size;
    call->api_tag = TAG_aud_bbatPcmBufferPlayStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_AudStreamRecordStop(SND_ITF_T itf)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudStreamRecordStop) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_AudStreamRecordStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudStreamRecordStop *ipar = (struct IPAR_DM_AudStreamRecordStop *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_AudStreamRecordStop *opar = (struct OPAR_DM_AudStreamRecordStop *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudStreamRecordStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->itf = itf;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void DM_AudSetSharkingMode(UINT32 isSharkingMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudSetSharkingMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudSetSharkingMode *ipar = (struct IPAR_DM_AudSetSharkingMode *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudSetSharkingMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isSharkingMode = isSharkingMode;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

INT32 DM_ZspVoiceVibrateStart(UINT32 time)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_ZspVoiceVibrateStart) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspVoiceVibrateStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_ZspVoiceVibrateStart *ipar = (struct IPAR_DM_ZspVoiceVibrateStart *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_ZspVoiceVibrateStart *opar = (struct OPAR_DM_ZspVoiceVibrateStart *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspVoiceVibrateStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->time = time;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_ZspVoiceVibrateStop(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspVoiceVibrateStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_ZspVoiceVibrateStop *opar = (struct OPAR_DM_ZspVoiceVibrateStop *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspVoiceVibrateStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_ZspMusicVibrateStart(UINT32 time)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_ZspMusicVibrateStart) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspMusicVibrateStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_ZspMusicVibrateStart *ipar = (struct IPAR_DM_ZspMusicVibrateStart *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_ZspMusicVibrateStart *opar = (struct OPAR_DM_ZspMusicVibrateStart *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspMusicVibrateStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->time = time;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_ZspMusicVibrateStop(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspMusicVibrateStop);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_DM_ZspMusicVibrateStop *opar = (struct OPAR_DM_ZspMusicVibrateStop *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspMusicVibrateStop;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_ZspMusicPlayStartEx(AUD_AP2CP_PARA_T *Aud_Para)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_ZspMusicPlayStartEx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspMusicPlayStartEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_ZspMusicPlayStartEx *ipar = (struct IPAR_DM_ZspMusicPlayStartEx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_ZspMusicPlayStartEx *opar = (struct OPAR_DM_ZspMusicPlayStartEx *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspMusicPlayStartEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->Aud_Para = *Aud_Para;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

INT32 DM_ZspMusicPlayStopEx(AUD_AP2CP_PARA_T *Aud_Para)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_ZspMusicPlayStopEx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_DM_ZspMusicPlayStopEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_ZspMusicPlayStopEx *ipar = (struct IPAR_DM_ZspMusicPlayStopEx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_DM_ZspMusicPlayStopEx *opar = (struct OPAR_DM_ZspMusicPlayStopEx *)((char*)mem + sizeof(rpcRespHeader_t));
    INT32 result;

    call->h.size = call_size;
    call->api_tag = TAG_DM_ZspMusicPlayStopEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->Aud_Para = *Aud_Para;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void DM_AudSetToneMode(UINT32 isToneMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudSetToneMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_DM_AudSetToneMode *ipar = (struct IPAR_DM_AudSetToneMode *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_DM_AudSetToneMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isToneMode = isToneMode;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void VTSP_startVolteCodec(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudSetToneMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;

    call->h.size = call_size;
    call->api_tag = TAG_VTSP_startVolteCodec;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void VTSP_stopVolteCodec(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_DM_AudSetToneMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;

    call->h.size = call_size;
    call->api_tag = TAG_VTSP_stopVolteCodec;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}


// AUTO GENERATED END
