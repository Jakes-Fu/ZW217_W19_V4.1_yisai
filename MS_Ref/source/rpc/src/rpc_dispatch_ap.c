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
#define DISABLE_RPC_AUD_SetCodecOpStatus
// AUTO GENERATED

extern void *bsearch(const void *key, const void *base,
                     unsigned nmemb, unsigned size,
                     int (*compar)(const void *, const void *));

typedef struct
{
    unsigned tag;
    void *function;
} tagFunction_t;

typedef struct
{
    unsigned tag;
    const char *name;
} tagName_t;

static int _tagCompare(const void *key, const void *p)
{
    unsigned tag = (unsigned)(unsigned long)key;
    const tagFunction_t *ct = (const tagFunction_t *)p;
    return (tag > ct->tag) ? 1 : (tag < ct->tag) ? -1 : 0;
}

# define DECLARE_API(api) void api(void)

DECLARE_API(RPC_sim_VsimProcessApduCB);
DECLARE_API(RPC_sim_GetSimCardModeCB);
DECLARE_API(RPC_AUD_ToneStopEnd);
DECLARE_API(RPC_audevPlayVoiceTone);
DECLARE_API(RPC_CFW_GetSSimFileInfoFromAP);
DECLARE_API(RPC_CFW_NwGetLockBCCHCtoA);
DECLARE_API(RPC_CFW_MeWriteMessageV2);
DECLARE_API(RPC_CFW_SetSoftSimDefaultInfo);
DECLARE_API(RPC_CFW_GetApnInfo);
DECLARE_API(RPC_sim_VsimResetCB);
DECLARE_API(RPC_CFW_MeDeleteMessageV2);
DECLARE_API(RPC_CFW_GetIMEIFromAP);
DECLARE_API(RPC_rpcHelloOnAP);
DECLARE_API(RPC_CFW_MeGetStorageInfoV2);
DECLARE_API(RPC_CFW_GetDefaultLteCidCtoA);
DECLARE_API(RPC_CFW_MeListMessageV2);
DECLARE_API(RPC_SMS_DM_Init_V2);
DECLARE_API(RPC_CFW_GetBootCausesFromAP);
DECLARE_API(RPC_AUD_VoiceCoderChangeInd);
DECLARE_API(RPC_CFW_GetIMEISVFromAP);
DECLARE_API(RPC_CFW_GetDetectMBS);
DECLARE_API(RPC_CFW_MeReadMessageV2);
DECLARE_API(RPC_AUD_SetCodecOpStatus);
DECLARE_API(RPC_CFW_WriteLongSmsInfo);
DECLARE_API(RPC_CFW_GetDefaultApn);
DECLARE_API(RPC_CFW_GetSimLockStatusFromAP);
DECLARE_API(RPC_CFW_NwIsUseISIM);
DECLARE_API(RPC_CFW_WifiSentestGetResult);
DECLARE_API(RPC_CFW_GetDftPdnInfoFromAP);

static const tagFunction_t gRpcApiTable[] = {
    #ifndef DISABLE_RPC_sim_VsimProcessApduCB
    {0x03e96fac, RPC_sim_VsimProcessApduCB},
    #endif
    #ifndef DISABLE_RPC_sim_GetSimCardModeCB
    {0x07749b4d, RPC_sim_GetSimCardModeCB},
    #endif
    #ifndef DISABLE_RPC_AUD_ToneStopEnd
    {0x10ead86d, RPC_AUD_ToneStopEnd},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSSimFileInfoFromAP
    {0x1cf8a768, RPC_CFW_GetSSimFileInfoFromAP},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetLockBCCHCtoA
    {0x1eb02fe9, RPC_CFW_NwGetLockBCCHCtoA},
    #endif
    #ifndef DISABLE_RPC_CFW_MeWriteMessageV2
    {0x20849a1a, RPC_CFW_MeWriteMessageV2},
    #endif
    #ifndef DISABLE_RPC_CFW_SetSoftSimDefaultInfo
    {0x23a48243, RPC_CFW_SetSoftSimDefaultInfo},
    #endif
    #ifndef DISABLE_RPC_CFW_GetApnInfo
    {0x32ba1bb3, RPC_CFW_GetApnInfo},
    #endif
    #ifndef DISABLE_RPC_sim_VsimResetCB
    {0x414a3352, RPC_sim_VsimResetCB},
    #endif
    #ifndef DISABLE_RPC_CFW_MeDeleteMessageV2
    {0x438ef180, RPC_CFW_MeDeleteMessageV2},
    #endif
    #ifndef DISABLE_RPC_CFW_GetIMEIFromAP
    {0x62e05df9, RPC_CFW_GetIMEIFromAP},
    #endif
    #ifndef DISABLE_RPC_rpcHelloOnAP
    {0x63ad8ff5, RPC_rpcHelloOnAP},
    #endif
    #ifndef DISABLE_RPC_CFW_MeGetStorageInfoV2
    {0x65a43caf, RPC_CFW_MeGetStorageInfoV2},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDefaultLteCidCtoA
    {0x73456edd, RPC_CFW_GetDefaultLteCidCtoA},
    #endif
    #ifndef DISABLE_RPC_CFW_MeListMessageV2
    {0x814d4f02, RPC_CFW_MeListMessageV2},
    #endif
    #ifndef DISABLE_RPC_audevPlayVoiceTone
    {0x8ec00598, RPC_audevPlayVoiceTone},
    #endif
    #ifndef DISABLE_RPC_SMS_DM_Init_V2
    {0x9a20ffcf, RPC_SMS_DM_Init_V2},
    #endif
    #ifndef DISABLE_RPC_CFW_GetBootCausesFromAP
    {0x9e161177, RPC_CFW_GetBootCausesFromAP},
    #endif
    #ifndef DISABLE_RPC_AUD_VoiceCoderChangeInd
    {0xa800f417, RPC_AUD_VoiceCoderChangeInd},
    #endif
    #ifndef DISABLE_RPC_CFW_GetIMEISVFromAP
    {0xab8a16e3, RPC_CFW_GetIMEISVFromAP},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDetectMBS
    {0xb691ecb2, RPC_CFW_GetDetectMBS},
    #endif
    #ifndef DISABLE_RPC_CFW_MeReadMessageV2
    {0xbb8bcea8, RPC_CFW_MeReadMessageV2},
    #endif
    #ifndef DISABLE_RPC_AUD_SetCodecOpStatus
    {0xc070ad19, RPC_AUD_SetCodecOpStatus},
    #endif
    #ifndef DISABLE_RPC_CFW_WriteLongSmsInfo
    {0xd2f01f11, RPC_CFW_WriteLongSmsInfo},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDefaultApn
    {0xda5560c6, RPC_CFW_GetDefaultApn},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimLockStatusFromAP
    {0xde45ef05, RPC_CFW_GetSimLockStatusFromAP},
    #endif
    #ifndef DISABLE_RPC_CFW_NwIsUseISIM
    {0xe0a7fc13, RPC_CFW_NwIsUseISIM},
    #endif
    #ifndef DISABLE_RPC_CFW_WifiSentestGetResult
    {0xf6a88248, RPC_CFW_WifiSentestGetResult},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDftPdnInfoFromAP
    {0xfc57b97f, RPC_CFW_GetDftPdnInfoFromAP},
    #endif
};

void *rpcFindFunctionByTag(unsigned tag)
{
    void *p = bsearch((const void *)(unsigned long)tag,
                      gRpcApiTable,
                      sizeof(gRpcApiTable) / sizeof(gRpcApiTable[0]),
                      sizeof(gRpcApiTable[0]),
                      _tagCompare);

    return (p != (void *)0) ? ((tagFunction_t *)p)->function : (void *)0;
}


void *rpcFindEventSender(unsigned id) { return (void *)0; }

DECLARE_API(RECV_EV_DM_DAEMON_IND);
DECLARE_API(RECV_EV_CFW_NW_SET_REGISTRATION_RSP);
DECLARE_API(RECV_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP);
DECLARE_API(RECV_EV_CFW_NW_GET_QSCANF_RSP);
DECLARE_API(RECV_EV_CFW_NW_GET_FREQSCAN_RSP);
DECLARE_API(RECV_EV_CFW_NW_GET_JAMMING_RSP);
DECLARE_API(RECV_EV_CFW_NW_MEASURE_RESULT_RSP);
DECLARE_API(RECV_EV_CFW_NW_JAMMING_RSSI_RSP);
DECLARE_API(RECV_EV_CFW_BLKLIST_GSM_PARAM_GET_IND);
DECLARE_API(RECV_EV_CFW_NW_NET_DL_PSM_PARAM_RSP);
DECLARE_API(RECV_EV_CFW_BLKLIST_LTE_PARAM_GET_IND);
DECLARE_API(RECV_EV_CFW_ERRC_IDL_MEAS_OFFSET_READ_RSP);
DECLARE_API(RECV_EV_CFW_NW_GET_FREQSCAN_V2_RSP);
DECLARE_API(RECV_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP);
DECLARE_API(RECV_EV_CFW_SS_SET_CALL_FORWARDING_RSP);
DECLARE_API(RECV_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP);
DECLARE_API(RECV_EV_CFW_SS_SEND_USSD_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PBK_ENTRY_RSP);
DECLARE_API(RECV_EV_CFW_SIM_LIST_PBK_ENTRY_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PBK_STRORAGE_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PROVIDER_ID_RSP);
DECLARE_API(RECV_EV_CFW_SIM_CHANGE_PWD_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_MESSAGE_RSP);
DECLARE_API(RECV_EV_CFW_SIM_LIST_MESSAGE_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_BINARY_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP);
DECLARE_API(RECV_EV_CFW_SAT_RESPONSE_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_FILE_STATUS_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_RECORD_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_ICCID_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_VOICEMIAL_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_FILES_RSP);
DECLARE_API(RECV_EV_CFW_SIM_TPDU_COMMAND_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_DF_STATUS_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_EID_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_CDMA_IMSI_RSP);
DECLARE_API(RECV_EV_CFW_SIM_SET_SUSPEND_RSP);
DECLARE_API(RECV_EV_CFW_SMS_READ_MESSAGE_RSP);
DECLARE_API(RECV_EV_CFW_SAT_REFRESH_FILE_CHG_RSP);
DECLARE_API(RECV_EV_CFW_CC_SPEECH_CALL_IND);
DECLARE_API(RECV_EV_CFW_NEW_SMS_IND);
DECLARE_API(RECV_EV_CFW_SS_USSD_IND);
DECLARE_API(RECV_EV_CFW_NW_NETWORKINFO_IND);
DECLARE_API(RECV_EV_CFW_GPRS_DATA_IND);
DECLARE_API(RECV_EV_CFW_MBS_CALL_INFO_IND);
DECLARE_API(RECV_EV_CFW_NEW_EMS_IND);
DECLARE_API(RECV_EV_CFW_NW_JAMMING_DETECT_IND);
DECLARE_API(RECV_EV_CFW_CB_PAGE_IND);
DECLARE_API(RECV_EV_CFW_EMC_NUM_LIST_IND);
DECLARE_API(RECV_EV_CFW_ERRC_CONNSTATUS_V2_IND);
DECLARE_API(RECV_EV_CFW_ENG_EVENT_RPT_IND);
DECLARE_API(RECV_EV_CFW_ENG_STATIS_RPT_IND);
DECLARE_API(RECV_EV_CFW_GSM_RACH_RESULT_IND);
DECLARE_API(RECV_EV_CFW_ENG_STATIS_RPT_EL1_IND);
DECLARE_API(RECV_EV_CFW_ENG_EVENT_RPT_V2_IND);

static const tagFunction_t gRpcEventRecvTable[] = {
    #ifndef DISABLE_RPC_EV_DM_DAEMON_IND
    {0x00000034, RECV_EV_DM_DAEMON_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_SET_REGISTRATION_RSP
    {0x000001f8, RECV_EV_CFW_NW_SET_REGISTRATION_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP
    {0x000001fa, RECV_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_GET_QSCANF_RSP
    {0x00000200, RECV_EV_CFW_NW_GET_QSCANF_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_GET_FREQSCAN_RSP
    {0x00000203, RECV_EV_CFW_NW_GET_FREQSCAN_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_GET_JAMMING_RSP
    {0x00000204, RECV_EV_CFW_NW_GET_JAMMING_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_MEASURE_RESULT_RSP
    {0x00000206, RECV_EV_CFW_NW_MEASURE_RESULT_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_JAMMING_RSSI_RSP
    {0x00000207, RECV_EV_CFW_NW_JAMMING_RSSI_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_BLKLIST_GSM_PARAM_GET_IND
    {0x00000208, RECV_EV_CFW_BLKLIST_GSM_PARAM_GET_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_NET_DL_PSM_PARAM_RSP
    {0x00000209, RECV_EV_CFW_NW_NET_DL_PSM_PARAM_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_BLKLIST_LTE_PARAM_GET_IND
    {0x0000020a, RECV_EV_CFW_BLKLIST_LTE_PARAM_GET_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_ERRC_IDL_MEAS_OFFSET_READ_RSP
    {0x0000020b, RECV_EV_CFW_ERRC_IDL_MEAS_OFFSET_READ_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_GET_FREQSCAN_V2_RSP
    {0x0000020c, RECV_EV_CFW_NW_GET_FREQSCAN_V2_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP
    {0x00000229, RECV_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SS_SET_CALL_FORWARDING_RSP
    {0x0000025c, RECV_EV_CFW_SS_SET_CALL_FORWARDING_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP
    {0x0000025d, RECV_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SS_SEND_USSD_RSP
    {0x00000264, RECV_EV_CFW_SS_SEND_USSD_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_PBK_ENTRY_RSP
    {0x000002bf, RECV_EV_CFW_SIM_GET_PBK_ENTRY_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_LIST_PBK_ENTRY_RSP
    {0x000002c0, RECV_EV_CFW_SIM_LIST_PBK_ENTRY_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_PBK_STRORAGE_RSP
    {0x000002c1, RECV_EV_CFW_SIM_GET_PBK_STRORAGE_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP
    {0x000002c2, RECV_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_PROVIDER_ID_RSP
    {0x000002c3, RECV_EV_CFW_SIM_GET_PROVIDER_ID_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_CHANGE_PWD_RSP
    {0x000002c4, RECV_EV_CFW_SIM_CHANGE_PWD_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_READ_MESSAGE_RSP
    {0x000002cd, RECV_EV_CFW_SIM_READ_MESSAGE_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_LIST_MESSAGE_RSP
    {0x000002ce, RECV_EV_CFW_SIM_LIST_MESSAGE_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_READ_BINARY_RSP
    {0x000002d0, RECV_EV_CFW_SIM_READ_BINARY_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP
    {0x000002d2, RECV_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SAT_RESPONSE_RSP
    {0x000002d9, RECV_EV_CFW_SAT_RESPONSE_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP
    {0x000002dc, RECV_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_FILE_STATUS_RSP
    {0x000002ea, RECV_EV_CFW_SIM_GET_FILE_STATUS_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_READ_RECORD_RSP
    {0x000002eb, RECV_EV_CFW_SIM_READ_RECORD_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_ICCID_RSP
    {0x000002ef, RECV_EV_CFW_SIM_GET_ICCID_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_READ_VOICEMIAL_RSP
    {0x000002f5, RECV_EV_CFW_SIM_READ_VOICEMIAL_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_READ_FILES_RSP
    {0x000002f8, RECV_EV_CFW_SIM_READ_FILES_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_TPDU_COMMAND_RSP
    {0x000002f9, RECV_EV_CFW_SIM_TPDU_COMMAND_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_DF_STATUS_RSP
    {0x000002fc, RECV_EV_CFW_SIM_GET_DF_STATUS_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_EID_RSP
    {0x000002fd, RECV_EV_CFW_SIM_GET_EID_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_GET_CDMA_IMSI_RSP
    {0x000002fe, RECV_EV_CFW_SIM_GET_CDMA_IMSI_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SIM_SET_SUSPEND_RSP
    {0x000002ff, RECV_EV_CFW_SIM_SET_SUSPEND_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SMS_READ_MESSAGE_RSP
    {0x00000307, RECV_EV_CFW_SMS_READ_MESSAGE_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SAT_REFRESH_FILE_CHG_RSP
    {0x000007d0, RECV_EV_CFW_SAT_REFRESH_FILE_CHG_RSP},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_CC_SPEECH_CALL_IND
    {0x0000138e, RECV_EV_CFW_CC_SPEECH_CALL_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NEW_SMS_IND
    {0x00001390, RECV_EV_CFW_NEW_SMS_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_SS_USSD_IND
    {0x00001394, RECV_EV_CFW_SS_USSD_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_NETWORKINFO_IND
    {0x00001399, RECV_EV_CFW_NW_NETWORKINFO_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_GPRS_DATA_IND
    {0x0000139e, RECV_EV_CFW_GPRS_DATA_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_MBS_CALL_INFO_IND
    {0x000013aa, RECV_EV_CFW_MBS_CALL_INFO_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NEW_EMS_IND
    {0x000013ab, RECV_EV_CFW_NEW_EMS_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_NW_JAMMING_DETECT_IND
    {0x000013af, RECV_EV_CFW_NW_JAMMING_DETECT_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_CB_PAGE_IND
    {0x000013b1, RECV_EV_CFW_CB_PAGE_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_EMC_NUM_LIST_IND
    {0x000013bf, RECV_EV_CFW_EMC_NUM_LIST_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_ERRC_CONNSTATUS_V2_IND
    {0x000013d4, RECV_EV_CFW_ERRC_CONNSTATUS_V2_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_ENG_EVENT_RPT_IND
    {0x000013d6, RECV_EV_CFW_ENG_EVENT_RPT_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_ENG_STATIS_RPT_IND
    {0x000013d7, RECV_EV_CFW_ENG_STATIS_RPT_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_GSM_RACH_RESULT_IND
    {0x000013d8, RECV_EV_CFW_GSM_RACH_RESULT_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_ENG_STATIS_RPT_EL1_IND
    {0x000013d9, RECV_EV_CFW_ENG_STATIS_RPT_EL1_IND},
    #endif
    #ifndef DISABLE_RPC_EV_CFW_ENG_EVENT_RPT_V2_IND
    {0x000013db, RECV_EV_CFW_ENG_EVENT_RPT_V2_IND},
    #endif
};

void *rpcFindEventUnpacker(unsigned tag)
{
    void *p = bsearch((const void *)(unsigned long)tag,
                      gRpcEventRecvTable,
                      sizeof(gRpcEventRecvTable) / sizeof(gRpcEventRecvTable[0]),
                      sizeof(gRpcEventRecvTable[0]),
                      _tagCompare);

    return (p != (void *)0) ? ((tagFunction_t *)p)->function : (void *)0;
}

static const tagName_t gApiNameTable[] = {
    #ifndef DISABLE_RPC_aud_SetCloseDelayFlag
    {0x01833648, "aud_SetCloseDelayFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetNetinfo
    {0x01a48c19, "CFW_GetNetinfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimManageChannel
    {0x01d08e7c, "CFW_SimManageChannel"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_SetPdnActTimerAndMaxCount
    {0x01daf8a8, "CFW_SetPdnActTimerAndMaxCount"},
    #endif
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockImeiVerify
    {0x02e02b8b, "CFW_SimlockImeiVerify"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetLteLockBand
    {0x036f213c, "CFW_EmodSetLteLockBand"},
    #endif
    #ifndef DISABLE_RPC_CSW_SetAudioZspVqeCalibParam
    {0x03d69255, "CSW_SetAudioZspVqeCalibParam"},
    #endif
    #ifndef DISABLE_RPC_sim_VsimProcessApduCB
    {0x03e96fac, "sim_VsimProcessApduCB"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_getDnsServerbyPdp
    {0x0425d22f, "CFW_getDnsServerbyPdp"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimAddPbkEntry
    {0x045ceb49, "CFW_SimAddPbkEntry"},
    #endif
    #ifndef DISABLE_RPC_CFW_CbSetType
    {0x04f77c3d, "CFW_CbSetType"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSubMenuCurrentItem
    {0x05ece985, "CFW_SatGetSubMenuCurrentItem"},
    #endif
    #ifndef DISABLE_RPC_hal_zspVoicePlayStop
    {0x06426192, "hal_zspVoicePlayStop"},
    #endif
    #ifndef DISABLE_RPC_SimPollReq
    {0x0722f33b, "SimPollReq"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgRestoreFactoryProfile
    {0x074a8a28, "CFW_CfgRestoreFactoryProfile"},
    #endif
    #ifndef DISABLE_RPC_CSW_WriteCalibParamToFlash
    {0x07600c35, "CSW_WriteCalibParamToFlash"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetPsmT3412
    {0x07648919, "CFW_nvSetPsmT3412"},
    #endif
    #ifndef DISABLE_RPC_sim_GetSimCardModeCB
    {0x07749b4d, "sim_GetSimCardModeCB"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcGetCsfbmtFlag
    {0x07a8fc4f, "CFW_CcGetCsfbmtFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetICCID
    {0x0807f6e5, "CFW_SimGetICCID"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimUpdateRecord
    {0x080cd798, "CFW_SimUpdateRecord"},
    #endif
    #ifndef DISABLE_RPC_CFW_SwitchUSBShareNetwork
    {0x08761de0, "CFW_SwitchUSBShareNetwork"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcPlayTone
    {0x08790a03, "CFW_CcPlayTone"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcEmcDial
    {0x096b6d42, "CFW_CcEmcDial"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetPsmEnable
    {0x099903fe, "CFW_nvSetPsmEnable"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetNvVersion
    {0x0a1a9480, "CFW_EmodGetNvVersion"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimInit
    {0x0a272beb, "CFW_SimInit"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetFreeCID
    {0x0a402eac, "CFW_GetFreeCID"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvmWriteStatic
    {0x0a517e38, "CFW_nvmWriteStatic"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GetGprsActState
    {0x0a671c97, "CFW_GetGprsActState"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetNWTimerOutFlag
    {0x0a79d13a, "CFW_GetNWTimerOutFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetRFTemperature
    {0x0a9305e1, "CFW_GetRFTemperature"},
    #endif
    #ifndef DISABLE_RPC_CFW_ErrcSetMeasThreshold
    {0x0abd51ef, "CFW_ErrcSetMeasThreshold"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsInitComplete
    {0x0aeb7786, "CFW_SmsInitComplete"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsSetSrvccMode
    {0x0b56f356, "CFW_ImsSetSrvccMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodEngineerCmd
    {0x0bb7f08e, "CFW_EmodEngineerCmd"},
    #endif
    #ifndef DISABLE_RPC_CFW_StackInit
    {0x0c80c52d, "CFW_StackInit"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetFileStatus
    {0x0c820824, "CFW_SimGetFileStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_SSCheckTestSim
    {0x0c87edd1, "CFW_SSCheckTestSim"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetStaticScene
    {0x0ceef0da, "CFW_SetStaticScene"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendSmsSCInfo
    {0x0d2e78ff, "CFW_SatGetSendSmsSCInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetOperatorInfo
    {0x0d3fa799, "CFW_CfgNwGetOperatorInfo"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsSetCevdp
    {0x0d905f27, "CFW_ImsSetCevdp"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GnssStartHandshakeStack
    {0x0dc49886, "CFW_GnssStartHandshakeStack"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSubMenuTitle
    {0x0e0e533d, "CFW_SatGetSubMenuTitle"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsMtSmsPPAckReq
    {0x0e138a39, "CFW_SmsMtSmsPPAckReq"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetGsmGprsOnly
    {0x0e31fbed, "CFW_SetGsmGprsOnly"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetSimFileUpdateCountMode
    {0x0e931937, "CFW_SetSimFileUpdateCountMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetLteLockCell
    {0x0ede542a, "CFW_EmodGetLteLockCell"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetJammingDetect
    {0x0f48160f, "CFW_GetJammingDetect"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetErrorReport
    {0x0f7a5643, "CFW_CfgSetErrorReport"},
    #endif
    #ifndef DISABLE_RPC_CFW_WifiSentestOff
    {0x0f7dab89, "CFW_WifiSentestOff"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetIndicatorEvent
    {0x103eecc1, "CFW_CfgGetIndicatorEvent"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GetGprsSum
    {0x105b539b, "CFW_GetGprsSum"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CcExtConfCallAdd
    {0x10aa25e7, "CFW_CcExtConfCallAdd"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcInitiateSpeechCall_V2
    {0x10d83a33, "CFW_CcInitiateSpeechCall_V2"},
    #endif
    #ifndef DISABLE_RPC_AUD_ToneStopEnd
    {0x10ead86d, "AUD_ToneStopEnd"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetCiotApnRateCtrl
    {0x120546f4, "CFW_nvSetCiotApnRateCtrl"},
    #endif
    #ifndef DISABLE_RPC_CSW_SetVqeCalibParamSwitch
    {0x1496afc9, "CSW_SetVqeCalibParamSwitch"},
    #endif
    #ifndef DISABLE_RPC_CFW_GprsSetInternalPdpCtx
    {0x14aedc7b, "CFW_GprsSetInternalPdpCtx"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSimSwitch
    {0x14e06e43, "CFW_CfgSetSimSwitch"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSmsShowTextModeParam
    {0x1503ad06, "CFW_CfgSetSmsShowTextModeParam"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsManualAcc
    {0x1514c385, "CFW_GprsManualAcc"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSaveNvData
    {0x152320c3, "CFW_EmodSaveNvData"},
    #endif
    #ifndef DISABLE_RPC_TM_KillAlarm
    {0x1538f990, "TM_KillAlarm"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadElementary
    {0x15b30ba0, "CFW_SimReadElementary"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadRecordAllParam
    {0x15b5d059, "CFW_SimReadRecordAllParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_GprsGetDynamicIP
    {0x16a17495, "CFW_GprsGetDynamicIP"},
    #endif
    #ifndef DISABLE_RPC_rpcPeerTagSupported
    {0x16be2d30, "rpcPeerTagSupported"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSMSConcat
    {0x16e18027, "CFW_GetSMSConcat"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSendDataV2
    {0x1776f7b9, "CFW_GprsSendDataV2"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_ImsEnVolte
    {0x17c0fa00, "CFW_ImsEnVolte"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetSmsSeviceMode
    {0x18df9276, "CFW_SetSmsSeviceMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendDTMFText
    {0x18dfc76c, "CFW_SatGetSendDTMFText"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetPlmn
    {0x19a6b578, "CFW_SetPlmn"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetClip
    {0x19b6c44a, "CFW_CfgGetClip"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSetupCallAddr
    {0x1a0bd347, "CFW_SatGetSetupCallAddr"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GetPdnDeactTimerAndMaxCount
    {0x1a437b5b, "CFW_GetPdnDeactTimerAndMaxCount"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetBatteryInfo
    {0x1a469219, "CFW_EmodGetBatteryInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimUpdateVoiceMailInfo
    {0x1a63f26a, "CFW_SimUpdateVoiceMailInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetEleFence
    {0x1a897db4, "CFW_SetEleFence"},
    #endif
    #ifndef DISABLE_RPC_aud_SetAudDeviceCFG
    {0x1b0b9a67, "aud_SetAudDeviceCFG"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetACMMax
    {0x1b686aea, "CFW_SimSetACMMax"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimAdnId
    {0x1bbe3c37, "CFW_GetSimAdnId"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodUpdateNwBind
    {0x1bcf7121, "CFW_EmodUpdateNwBind"},
    #endif
    #ifndef DISABLE_RPC_aud_StreamStop
    {0x1c29ffec, "aud_StreamStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetEleFence
    {0x1c41c70e, "CFW_GetEleFence"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSSimFileInfoFromAP
    {0x1cf8a768, "CFW_GetSSimFileInfoFromAP"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetGetInputDefaultText
    {0x1d1787bb, "CFW_SatGetGetInputDefaultText"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetPsmT3412
    {0x1d1f91db, "CFW_nvGetPsmT3412"},
    #endif
    #ifndef DISABLE_RPC_CFW_StartNstMode
    {0x1d286e43, "CFW_StartNstMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetAudioAudioMode
    {0x1e00e28a, "CFW_CfgSetAudioAudioMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetNewSmsOption
    {0x1ea60113, "CFW_CfgGetNewSmsOption"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetLockBCCHCtoA
    {0x1eb02fe9, "CFW_NwGetLockBCCHCtoA"},
    #endif
    #ifndef DISABLE_RPC_hal_zspVoiceRecordStart
    {0x1ecdf88b, "hal_zspVoiceRecordStart"},
    #endif
    #ifndef DISABLE_RPC_CSW_DumpPcmDataToTflash
    {0x1f016809, "CSW_DumpPcmDataToTflash"},
    #endif
    #ifndef DISABLE_RPC_DM_VoisTestModeSetup
    {0x1f954906, "DM_VoisTestModeSetup"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetPrefOperatorList
    {0x1f9e7d3c, "CFW_SimSetPrefOperatorList"},
    #endif
    #ifndef DISABLE_RPC_DM_ZspMusicVibrateStop
    {0x2027253a, "DM_ZspMusicVibrateStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSmsFormat
    {0x203e9990, "CFW_CfgGetSmsFormat"},
    #endif
    #ifndef DISABLE_RPC_CFW_SendMtSmsAckPPError
    {0x204a3ca8, "CFW_SendMtSmsAckPPError"},
    #endif
    #ifndef DISABLE_RPC_CFW_MeWriteMessageV2
    {0x20849a1a, "CFW_MeWriteMessageV2"},
    #endif
    #ifndef DISABLE_RPC_hal_zspVoicePlayStart
    {0x20bc27e8, "hal_zspVoicePlayStart"},
    #endif
    #ifndef DISABLE_RPC_aud_StreamStart
    {0x20f7d018, "aud_StreamStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetLteLockTddOrFdd
    {0x20f88847, "CFW_EmodSetLteLockTddOrFdd"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSpCleanInfo
    {0x21813e4a, "CFW_EmodSpCleanInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcCallHoldMultiparty_V2
    {0x21fec076, "CFW_CcCallHoldMultiparty_V2"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmWriteDynamic
    {0x226599f3, "CFW_RpmWriteDynamic"},
    #endif
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetCtxEQos
    {0x22a1f04c, "CFW_GprsSetCtxEQos"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetEleFenceTimer
    {0x22bee7d1, "CFW_SetEleFenceTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetCopsTimer
    {0x22d5d04f, "CFW_nvGetCopsTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetCurrentOperator
    {0x235d1b94, "CFW_NwGetCurrentOperator"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmResetRpmFunction
    {0x2395da66, "CFW_RpmResetRpmFunction"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetSoftSimDefaultInfo
    {0x23a48243, "CFW_SetSoftSimDefaultInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetNetWorkMode
    {0x23c4e97f, "CFW_CfgNwGetNetWorkMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetNasTimer
    {0x23dada90, "CFW_nvGetNasTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetToneDuration
    {0x23e5688e, "CFW_CfgGetToneDuration"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetEdrxValue
    {0x24577c87, "CFW_nvSetEdrxValue"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetCallWaiting
    {0x24883270, "CFW_CfgSetCallWaiting"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmAllowPdpActivating
    {0x2497d885, "CFW_RpmAllowPdpActivating"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadMessage
    {0x24e30979, "CFW_SimReadMessage"},
    #endif
    #ifndef DISABLE_RPC_CFW_LocGetWifiscanRes
    {0x2533c918, "CFW_LocGetWifiscanRes"},
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetEdrx
    {0x255ef262, "CFW_GprsSetEdrx"},
    #endif
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ClearGprsSum
    {0x25e3db36, "CFW_ClearGprsSum"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSpLteDummyPara
    {0x2689cabc, "CFW_EmodSpLteDummyPara"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsSetFacilityLock
    {0x26f63727, "CFW_SsSetFacilityLock"},
    #endif
    #ifndef DISABLE_RPC_hal_zspMusicPlayStop
    {0x2718f091, "hal_zspMusicPlayStop"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_SetDnsServerbyPdp
    {0x27543604, "CFW_SetDnsServerbyPdp"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetCcCount
    {0x276ee189, "CFW_GetCcCount"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSSN
    {0x27d28c5a, "CFW_CfgSetSSN"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetAudioAudioMode
    {0x28488c88, "CFW_CfgGetAudioAudioMode"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetReqQosUmts
    {0x2853b066, "CFW_GprsSetReqQosUmts"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSetupEventList
    {0x28a20d81, "CFW_SatGetSetupEventList"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetPdpCxtV2
    {0x28c73dae, "CFW_GprsGetPdpCxtV2"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSetupIdleModeText
    {0x28f76343, "CFW_SatGetSetupIdleModeText"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetLocPriority_V2
    {0x29628bd4, "CFW_nvSetLocPriority_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSpecialNvProcess
    {0x2994f59c, "CFW_EmodSpecialNvProcess"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetPbkStorage
    {0x2a2c1f97, "CFW_CfgSetPbkStorage"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetPeriodTauAftLocalRel
    {0x2a472b28, "CFW_CfgSetPeriodTauAftLocalRel"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadFiles
    {0x2a7cf76d, "CFW_SimReadFiles"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcAcceptSpeechCall
    {0x2b480237, "CFW_CcAcceptSpeechCall"},
    #endif
    #ifndef DISABLE_RPC_CFW_GnssStopHandshakeStack
    {0x2b6ef2ba, "CFW_GnssStopHandshakeStack"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetBlackListTimer
    {0x2b876dad, "CFW_NwSetBlackListTimer"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsCheckStatusAfterCc
    {0x2b9844f1, "CFW_ImsCheckStatusAfterCc"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetRRCRelEx
    {0x2c1cb7e6, "CFW_GetRRCRelEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSaveIMEI
    {0x2c611403, "CFW_EmodSaveIMEI"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetNetMode
    {0x2c64c51d, "CFW_NwSetNetMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsWriteMessage_V2
    {0x2c80e79f, "CFW_SmsWriteMessage_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetIMSI
    {0x2d03185d, "CFW_CfgGetIMSI"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetPsmT3324
    {0x2d84f034, "CFW_nvGetPsmT3324"},
    #endif
    #ifndef DISABLE_RPC_CFW_SwitchPort
    {0x2d9c14db, "CFW_SwitchPort"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatActivation
    {0x2d9d2455, "CFW_SatActivation"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsQueryCallWaiting
    {0x2e1489d8, "CFW_SsQueryCallWaiting"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSmsStorageInfo
    {0x2e2a7c57, "CFW_CfgGetSmsStorageInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetCdrxNoSleepEnable
    {0x2e7a6514, "CFW_CfgSetCdrxNoSleepEnable"},
    #endif
    #ifndef DISABLE_RPC_CSW_SetAudioZspPreProcessCalibParam
    {0x2ebfacc0, "CSW_SetAudioZspPreProcessCalibParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetEdrxEnable
    {0x2ecd031c, "CFW_nvSetEdrxEnable"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodImsNvParamSet
    {0x2f55a10e, "CFW_EmodImsNvParamSet"},
    #endif
    #ifndef DISABLE_RPC_CFW_ImsGetSrvccCap
    {0x2f976c7e, "CFW_ImsGetSrvccCap"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetPrefOperatorList
    {0x2fc7c1ba, "CFW_SimGetPrefOperatorList"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimClose
    {0x2fd3f031, "CFW_SimClose"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetCiotErwopdn
    {0x2fd93b53, "CFW_nvSetCiotErwopdn"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetColp
    {0x2fd9ebda, "CFW_CfgSetColp"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetUsrHisFreqInfo
    {0x30587993, "CFW_SetUsrHisFreqInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodRrdmParam
    {0x307d94f2, "CFW_EmodRrdmParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetCiotCpciot
    {0x31593bac, "CFW_nvSetCiotCpciot"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimRefreshFiles
    {0x32232a5c, "CFW_SimRefreshFiles"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetEdrxValue
    {0x324c8613, "CFW_nvGetEdrxValue"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetApnInfo
    {0x32ba1bb3, "CFW_GetApnInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetChannelID
    {0x334b9df1, "CFW_SatGetChannelID"},
    #endif
    #ifndef DISABLE_RPC_CFW_WifiSentestSetParam
    {0x33663f69, "CFW_WifiSentestSetParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetRatePriority
    {0x33e9d7ee, "CFW_SetRatePriority"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmNvGetDefaultPdnSettingApn
    {0x3496ac1d, "CFW_RpmNvGetDefaultPdnSettingApn"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetBlackList_V2
    {0x34ae5835, "CFW_NwSetBlackList_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendSmsAlpha
    {0x34ba6071, "CFW_SatGetSendSmsAlpha"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetCDACValue
    {0x34cca851, "CFW_SetCDACValue"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetAudioOutputParam
    {0x356b9b3e, "CFW_CfgGetAudioOutputParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendSmsData
    {0x359f7fd6, "CFW_SatGetSendSmsData"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockDataCheckUnlock
    {0x35e180c8, "CFW_SimlockDataCheckUnlock"},
    #endif
    #endif
    #ifndef DISABLE_RPC_rpcGetPeerTags
    {0x36271b1c, "rpcGetPeerTags"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSimGetMeProfile
    {0x365a1560, "CFW_CfgSimGetMeProfile"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_SetPdnDeactTimerAndMaxCount
    {0x366e4680, "CFW_SetPdnDeactTimerAndMaxCount"},
    #endif
    #endif
    #ifndef DISABLE_RPC_DM_StartAudioEx
    {0x37317995, "DM_StartAudioEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetOpenChannelAPN
    {0x37aef5e0, "CFW_SatGetOpenChannelAPN"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetLoudspeakerVolumeLevel
    {0x37be59b4, "CFW_CfgGetLoudspeakerVolumeLevel"},
    #endif
    #ifndef DISABLE_RPC_CFW_SendMtSmsAckPPErrorWithData
    {0x37e910c9, "CFW_SendMtSmsAckPPErrorWithData"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSATIndFormate
    {0x381dbef1, "CFW_GetSATIndFormate"},
    #endif
    #ifndef DISABLE_RPC_rpcHelloOnCP
    {0x38ba7a57, "rpcHelloOnCP"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetStaticScene
    {0x3940f8aa, "CFW_GetStaticScene"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgQuerySmsService
    {0x398b2a95, "CFW_CfgQuerySmsService"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSimSetMeProfile
    {0x398eddda, "CFW_CfgSimSetMeProfile"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetGetInputLengthRange
    {0x39ee6a8f, "CFW_SatGetGetInputLengthRange"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetStackSimFileIDByPath_V2
    {0x39fc26f4, "CFW_GetStackSimFileIDByPath_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetCSPagingFlag
    {0x3a238517, "CFW_GetCSPagingFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetAoHandle
    {0x3a43271f, "CFW_GetAoHandle"},
    #endif
    #ifndef DISABLE_RPC_SimSendStatusReq
    {0x3b8046f0, "SimSendStatusReq"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetCdrxNoSleepEnable
    {0x3baecbd5, "CFW_CfgGetCdrxNoSleepEnable"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetProviderId
    {0x3bc96b1f, "CFW_SimGetProviderId"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcSetCsfbmtFlag
    {0x3c1e6a02, "CFW_CcSetCsfbmtFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_NWSetStackRat
    {0x3cdf3da7, "CFW_NWSetStackRat"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetStoredPlmnList
    {0x3ce9c17e, "CFW_CfgSetStoredPlmnList"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetClir
    {0x3cf96f15, "CFW_CfgSetClir"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsAbortSendMessage
    {0x3d6b4b93, "CFW_SmsAbortSendMessage"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetLCTimerEx
    {0x3ddd4301, "CFW_SetLCTimerEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetColp
    {0x3e336470, "CFW_CfgGetColp"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetMenuTitle
    {0x3e3a67a0, "CFW_SatGetMenuTitle"},
    #endif
    #ifndef DISABLE_RPC_aud_SetBtClk
    {0x3e50af7d, "aud_SetBtClk"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatSetFacility
    {0x3e53e123, "CFW_SatSetFacility"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSysConfig
    {0x3e65984a, "CFW_NwSysConfig"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetRFFreqPwrRange
    {0x3f237e1f, "CFW_SetRFFreqPwrRange"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSmsStorage
    {0x3f3995c2, "CFW_CfgSetSmsStorage"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetAuthenticationStatus
    {0x3f6c8048, "CFW_SimGetAuthenticationStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetAllDetach
    {0x3fd424e9, "CFW_NwGetAllDetach"},
    #endif
    #ifndef DISABLE_RPC_DM_ZspMusicPlayStart
    {0x3fe7d300, "DM_ZspMusicPlayStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendUSSDDisplayInfo
    {0x400d76b9, "CFW_SatGetSendUSSDDisplayInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendSSDisplayInfo
    {0x406221ef, "CFW_SatGetSendSSDisplayInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetSimSwich
    {0x4073e5e7, "CFW_NwSetSimSwich"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsQueryFacilityLock
    {0x40860195, "CFW_SsQueryFacilityLock"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetSMSConcat
    {0x41075729, "CFW_SetSMSConcat"},
    #endif
    #ifndef DISABLE_RPC_sim_VsimResetCB
    {0x414a3352, "sim_VsimResetCB"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetFirstPowerOnFlag
    {0x424b0ac3, "CFW_CfgSetFirstPowerOnFlag"},
    #endif
    #if defined(_SIM_HOT_PLUG_)
    #ifndef DISABLE_RPC_CFW_SetPollingTimer
    {0x42aa254a, "CFW_SetPollingTimer"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimPollReq
    {0x434692a3, "CFW_SimPollReq"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetOpenChannelUserInfo
    {0x43754e24, "CFW_SatGetOpenChannelUserInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_MeDeleteMessageV2
    {0x438ef180, "CFW_MeDeleteMessageV2"},
    #endif
    #ifndef DISABLE_RPC_aud_HeadsetConfig
    {0x43c78c4d, "aud_HeadsetConfig"},
    #endif
    #ifndef DISABLE_RPC_aud_StreamRecordStop
    {0x440eda14, "aud_StreamRecordStop"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_SetDnsServerbyPdpV2
    {0x443c5edf, "CFW_SetDnsServerbyPdpV2"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimSelectApplication
    {0x44499329, "CFW_SimSelectApplication"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetBindInfo
    {0x44bf3c87, "CFW_EmodSetBindInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_ImsGetCevdp
    {0x44f31984, "CFW_ImsGetCevdp"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatTerminalEnvelope
    {0x452041d8, "CFW_SatTerminalEnvelope"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetOpenChannelBearerDes
    {0x4550c36f, "CFW_SatGetOpenChannelBearerDes"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcGetCallStatus
    {0x46843945, "CFW_CcGetCallStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSmsSeviceMode
    {0x46e369b5, "CFW_GetSmsSeviceMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSyncInfoTest
    {0x471bc7fc, "CFW_EmodSyncInfoTest"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetCiotNonip
    {0x472b9c60, "CFW_nvGetCiotNonip"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwDeRegister
    {0x47c56cdb, "CFW_NwDeRegister"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetDefaultPdnInfo
    {0x48347890, "CFW_GprsGetDefaultPdnInfo"},
    #endif
    #endif
    #ifndef DISABLE_RPC_DM_ZspMusicPlayStop
    {0x4859dfbc, "DM_ZspMusicPlayStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcEmcDialEx
    {0x48762fb4, "CFW_CcEmcDialEx"},
    #endif
    #ifndef DISABLE_RPC_aud_bbatPcmBufferPlayStop
    {0x4889e3fb, "aud_bbatPcmBufferPlayStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetPlayTone
    {0x48f0aa6d, "CFW_SatGetPlayTone"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetBlackListEnable
    {0x495f144a, "CFW_nvSetBlackListEnable"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetPdpIpv6Addr
    {0x498f813d, "CFW_GprsGetPdpIpv6Addr"},
    #endif
    #endif
    #ifndef DISABLE_RPC_hal_btVoiceStart
    {0x49aeb77b, "hal_btVoiceStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_StopNstMode
    {0x4a9d236e, "CFW_StopNstMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetPbkStorage
    {0x4ab2b8c4, "CFW_SimGetPbkStorage"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetRRCRel
    {0x4ace1ca9, "CFW_GetRRCRel"},
    #endif
    #ifndef DISABLE_RPC_aud_bbatPcmBufferPlayStart
    {0x4b09eff6, "aud_bbatPcmBufferPlayStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetMaxAttFailForIratReq
    {0x4b18a9b9, "CFW_NwSetMaxAttFailForIratReq"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetMenuCurrentItem
    {0x4c241761, "CFW_SatGetMenuCurrentItem"},
    #endif
    #ifndef DISABLE_RPC_DM_AudSetSharkingMode
    {0x4c6aff6a, "DM_AudSetSharkingMode"},
    #endif
    #ifndef DISABLE_RPC_DM_ZspVoiceVibrateStart
    {0x4c8dde44, "DM_ZspVoiceVibrateStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_SaveSsimFileInfo
    {0x4dfb73ad, "CFW_SaveSsimFileInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatResponse
    {0x4fb39c55, "CFW_SatResponse"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsDeleteMessage
    {0x50cab25c, "CFW_SmsDeleteMessage"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetPsmEnable
    {0x5190aaa0, "CFW_nvGetPsmEnable"},
    #endif
    #ifndef DISABLE_RPC_CFW_GprsGetDynamicAPN
    {0x51a0de23, "CFW_GprsGetDynamicAPN"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_rpmNvGetLastRat
    {0x5246b62b, "CFW_rpmNvGetLastRat"},
    #endif
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsCtxEpsModify
    {0x526ea981, "CFW_GprsCtxEpsModify"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadPreferPLMN
    {0x52f296ca, "CFW_SimReadPreferPLMN"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetPlayToneDuration
    {0x53255770, "CFW_SatGetPlayToneDuration"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetMInQos
    {0x53a8e0fb, "CFW_GprsGetMInQos"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SmsCbGetNode
    {0x53bd5477, "CFW_SmsCbGetNode"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetACMMax
    {0x5447da9e, "CFW_SimGetACMMax"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsSetDapsCallback
    {0x554d3316, "CFW_ImsSetDapsCallback"},
    #endif
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsDoSavedEmcDail
    {0x555cee71, "CFW_ImsDoSavedEmcDail"},
    #endif
    #endif
    #if defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_CcReleaseCallX
    {0x55e31e22, "CFW_CcReleaseCallX"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetLteLockBandV2
    {0x56e376ff, "CFW_EmodSetLteLockBandV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetMeProfile
    {0x574ae0f5, "CFW_SimGetMeProfile"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmWriteStatic
    {0x5756a514, "CFW_RpmWriteStatic"},
    #endif
    #endif
    #if defined(CSG_SUPPORT)
    #ifndef DISABLE_RPC_CFW_NwGetCsgMode
    {0x5885250b, "CFW_NwGetCsgMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwSetFM
    {0x58f04fb2, "CFW_CfgNwSetFM"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetNewSmsOption
    {0x592995fb, "CFW_CfgSetNewSmsOption"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDrxCyc
    {0x59500382, "CFW_GetDrxCyc"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcCallMultipartyFunction
    {0x59557ac7, "CFW_CcCallMultipartyFunction"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLmtMaxLen
    {0x5a52fd76, "CFW_GetLmtMaxLen"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetCurCMD
    {0x5a8598b0, "CFW_SatGetCurCMD"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetNwOperDispFormat
    {0x5b070593, "CFW_CfgGetNwOperDispFormat"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcGetMptyNum
    {0x5be547bc, "CFW_CcGetMptyNum"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetCiotCpciot
    {0x5be63aff, "CFW_nvGetCiotCpciot"},
    #endif
    #ifndef DISABLE_RPC_CFW_AttachCSAndPS
    {0x5be8fc29, "CFW_AttachCSAndPS"},
    #endif
    #ifndef DISABLE_RPC_CFW_UpdateNV
    {0x5c00f490, "CFW_UpdateNV"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetLteCalibInfo
    {0x5c3b6a0e, "CFW_EmodGetLteCalibInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_ShellControl_V2
    {0x5d0ce3b9, "CFW_ShellControl_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_WifiScanStop
    {0x5dc8d617, "CFW_WifiScanStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetDefaultPcoControl
    {0x5e1f4cd3, "CFW_SetDefaultPcoControl"},
    #endif
    #ifndef DISABLE_RPC_TM_ListAlarm
    {0x5e72c269, "TM_ListAlarm"},
    #endif
    #ifndef DISABLE_RPC_DM_AudStreamStop
    {0x5e757a68, "DM_AudStreamStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimListCountPbkEntries
    {0x5e84647a, "CFW_SimListCountPbkEntries"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetBindInfo
    {0x5eb87c1b, "CFW_EmodGetBindInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetFacility
    {0x5fc9f415, "CFW_SatGetFacility"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockSetVersion
    {0x6021b27e, "CFW_SimlockSetVersion"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimWriteMessage
    {0x606ae82f, "CFW_SimWriteMessage"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetFrequencyBand
    {0x60c38c93, "CFW_NwSetFrequencyBand"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetSimStatus
    {0x610691f7, "CFW_SetSimStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetPocPara
    {0x6136a6fd, "CFW_EmodGetPocPara"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcStopTone
    {0x615f0347, "CFW_CcStopTone"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsQueryClip
    {0x618b132d, "CFW_SsQueryClip"},
    #endif
    #ifndef DISABLE_RPC_CFW_GprsGetNsapi
    {0x6198a170, "CFW_GprsGetNsapi"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatSetEventList
    {0x624e8997, "CFW_SatSetEventList"},
    #endif
    #ifndef DISABLE_RPC_CFW_DetachCSAndPS
    {0x62d52415, "CFW_DetachCSAndPS"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetIMEIFromAP
    {0x62e05df9, "CFW_GetIMEIFromAP"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockToolGetUid
    {0x62e37403, "CFW_SimlockToolGetUid"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimListPbkEntries
    {0x62e3977e, "CFW_SimListPbkEntries"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetDefaultPdnMode
    {0x631195a3, "CFW_GprsSetDefaultPdnMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_rpcHelloOnAP
    {0x63ad8ff5, "rpcHelloOnAP"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsAct
    {0x63af5d12, "CFW_GprsAct"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodClearUserInfo
    {0x63df1884, "CFW_EmodClearUserInfo"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmCR1
    {0x641206e8, "CFW_RpmGetRpmCR1"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_isPdpActiveAvailable
    {0x641e4ad6, "CFW_isPdpActiveAvailable"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadBinary
    {0x6482e52d, "CFW_SimReadBinary"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetLockedFrequencyBand
    {0x64c27811, "CFW_CfgNwGetLockedFrequencyBand"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetLteLockTddOrFdd
    {0x654ba1cb, "CFW_EmodGetLteLockTddOrFdd"},
    #endif
    #ifndef DISABLE_RPC_CFW_MeGetStorageInfoV2
    {0x65a43caf, "CFW_MeGetStorageInfoV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLETRealTimeInfo
    {0x66010143, "CFW_GetLETRealTimeInfo"},
    #endif
    #ifndef DISABLE_RPC_SimSwitch
    {0x665f6ca9, "SimSwitch"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetLteLockCell
    {0x66b3e0aa, "CFW_EmodSetLteLockCell"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetServiceProviderName
    {0x67735c93, "CFW_SimGetServiceProviderName"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsListFree
    {0x6791096e, "CFW_SmsListFree"},
    #endif
    #ifndef DISABLE_RPC_CFW_DecodeCDMAImsi
    {0x685d1b5a, "CFW_DecodeCDMAImsi"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetGetInputText
    {0x69789adc, "CFW_SatGetGetInputText"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetCharacterSet
    {0x69bee605, "CFW_CfgGetCharacterSet"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetJammingDetectFlag
    {0x69ca86ef, "CFW_SetJammingDetectFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_GprsHostAddress
    {0x69e9af8d, "CFW_GprsHostAddress"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetCellInfoV2
    {0x6a47af00, "CFW_GetCellInfoV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetBlackListEnable
    {0x6a9c01e4, "CFW_nvGetBlackListEnable"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetProductInfo
    {0x6b0b2278, "CFW_EmodGetProductInfo"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsSetPdpStatus
    {0x6b251398, "CFW_ImsSetPdpStatus"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GprsSendCtxCfg_V2
    {0x6b3761da, "CFW_GprsSendCtxCfg_V2"},
    #endif
    #ifndef DISABLE_RPC_DM_VoisSetCfg
    {0x6b3f93ca, "DM_VoisSetCfg"},
    #endif
    #if defined(_SIM_HOT_PLUG_)
    #ifndef DISABLE_RPC_CFW_GetPollingTimer
    {0x6b497865, "CFW_GetPollingTimer"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetCEMode
    {0x6b687af8, "CFW_EmodGetCEMode"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetstatus
    {0x6cd777a5, "CFW_GprsSetstatus"},
    #endif
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetstatus
    {0x6d2f4987, "CFW_GprsGetstatus"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SmsSendMessage_V3
    {0x6d3c7e9f, "CFW_SmsSendMessage_V3"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetPdpCxtV2
    {0x6e3e849a, "CFW_GprsSetPdpCxtV2"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CbReadContext
    {0x6ebf94e1, "CFW_CbReadContext"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_getDNSServer
    {0x6f20505a, "getDNSServer"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwSetCgclassType
    {0x6f2b39a6, "CFW_CfgNwSetCgclassType"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetCEMode
    {0x6f36868c, "CFW_EmodSetCEMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetCgclassType
    {0x6f446fb3, "CFW_CfgNwGetCgclassType"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetReceiveData
    {0x6fb9f6ef, "CFW_SatGetReceiveData"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetResultCodePresentMode
    {0x6fc3900c, "CFW_CfgGetResultCodePresentMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwAbortListOperators
    {0x6fcc1282, "CFW_NwAbortListOperators"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetRRCRel
    {0x7121b58d, "CFW_SetRRCRel"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetCurrPreferPLMNList
    {0x71304b64, "CFW_SetCurrPreferPLMNList"},
    #endif
    #ifndef DISABLE_RPC_CSW_VersionControl
    {0x717536f2, "CSW_VersionControl"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgDeviceSwithOff
    {0x71ed90b2, "CFW_CfgDeviceSwithOff"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetPocEnable
    {0x723720a5, "CFW_CfgGetPocEnable"},
    #endif
    #ifndef DISABLE_RPC_CSW_ExportCalibOrImportCalibFlashParam
    {0x725848a9, "CSW_ExportCalibOrImportCalibFlashParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSetupCallCapCfg
    {0x726026aa, "CFW_SatGetSetupCallCapCfg"},
    #endif
    #ifndef DISABLE_RPC_hal_zspVoiceDtmfStop
    {0x7260692e, "hal_zspVoiceDtmfStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetNvData
    {0x727f5ca5, "CFW_EmodSetNvData"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetExtendedErrorV1
    {0x72ad454c, "CFW_CfgGetExtendedErrorV1"},
    #endif
    #ifndef DISABLE_RPC_CFW_CbGetContext
    {0x733f8641, "CFW_CbGetContext"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDefaultLteCidCtoA
    {0x73456edd, "CFW_GetDefaultLteCidCtoA"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GetPdnActTimerAndMaxCount
    {0x7373b075, "CFW_GetPdnActTimerAndMaxCount"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetPbkStrorageInfo
    {0x73af8928, "CFW_CfgSetPbkStrorageInfo"},
    #endif
    #ifndef DISABLE_RPC_DM_PlayToneEx
    {0x742087f6, "DM_PlayToneEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetOperatorName
    {0x74dc9c53, "CFW_CfgNwGetOperatorName"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetToneDuration
    {0x75013fe5, "CFW_CfgSetToneDuration"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmFx
    {0x7527d1c4, "CFW_RpmGetRpmFx"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSetupCallText
    {0x75599008, "CFW_SatGetSetupCallText"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendSSInfo
    {0x75b5e809, "CFW_SatGetSendSSInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetIMEI
    {0x75bf0c54, "CFW_EmodGetIMEI"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsCtxModifyRej
    {0x75d9a654, "CFW_GprsCtxModifyRej"},
    #endif
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSecPdpAct
    {0x761ecef8, "CFW_GprsSecPdpAct"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetTraceFlag
    {0x7658ccd8, "CFW_EmodGetTraceFlag"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsActEx
    {0x76a3cadf, "CFW_GprsActEx"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetBaseBandVersionV1
    {0x76b6efdf, "CFW_EmodGetBaseBandVersionV1"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetAvailableOperators
    {0x76d14edf, "CFW_NwGetAvailableOperators"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatResponseV2
    {0x7706ee90, "CFW_SatResponseV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_CusPrivatePsm
    {0x7747c1fb, "CFW_CusPrivatePsm"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadRecordWithLen
    {0x7754395f, "CFW_SimReadRecordWithLen"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetCiotSmsWoCombAtt
    {0x778928d7, "CFW_nvSetCiotSmsWoCombAtt"},
    #endif
    #ifndef DISABLE_RPC_DM_RestartAudioEx
    {0x77d483d6, "DM_RestartAudioEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetLoudspeakerVolumeLevelRange
    {0x78fe7b0d, "CFW_CfgGetLoudspeakerVolumeLevelRange"},
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetCtxTFT
    {0x79d77a2b, "CFW_GprsSetCtxTFT"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetEventList
    {0x79df0b80, "CFW_SatGetEventList"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetAutoAttachFlag
    {0x7a4fe641, "CFW_NwGetAutoAttachFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetEchoMode
    {0x7a5ee337, "CFW_CfgGetEchoMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodOutfieldTestStartV2
    {0x7a7b7369, "CFW_EmodOutfieldTestStartV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLETRealTimeInfoV2
    {0x7ab1b708, "CFW_GetLETRealTimeInfoV2"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsCtxModifyAcc
    {0x7ab90486, "CFW_GprsCtxModifyAcc"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetICCID
    {0x7b5f3507, "CFW_GetICCID"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwSetLockedFrequencyBand
    {0x7b6bbded, "CFW_CfgNwSetLockedFrequencyBand"},
    #endif
    #ifndef DISABLE_RPC_CFW_UsimDecodeEFFcpV2
    {0x7c191a7f, "CFW_UsimDecodeEFFcpV2"},
    #endif
    #ifndef DISABLE_RPC_DM_ZspMusicPlayStopEx
    {0x7c7b3493, "DM_ZspMusicPlayStopEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_CbDeAct
    {0x7d7969f5, "CFW_CbDeAct"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsSetCallForwarding
    {0x7dbb6ec7, "CFW_SsSetCallForwarding"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSmsStorageInfo
    {0x7e01cec5, "CFW_CfgSetSmsStorageInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SwitchDualSimSlot
    {0x7e035c81, "CFW_SwitchDualSimSlot"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReset
    {0x7e6dec45, "CFW_SimReset"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetStatisticRpt
    {0x7e6f2cb2, "CFW_NwGetStatisticRpt"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSmsOverflowInd
    {0x7fcfef26, "CFW_CfgGetSmsOverflowInd"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockDataCheckLock
    {0x7fdfe70c, "CFW_SimlockDataCheckLock"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CcEmcDialEx_V2
    {0x80243b3f, "CFW_CcEmcDialEx_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetEnhancePwrSweepNonCE
    {0x80909c55, "CFW_CfgSetEnhancePwrSweepNonCE"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSubMenuItemByID
    {0x80c5ea5a, "CFW_SatGetSubMenuItemByID"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLimiteTimer
    {0x8148e1cb, "CFW_GetLimiteTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetOpenChannelNetInfo
    {0x814c3abc, "CFW_SatGetOpenChannelNetInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_MeListMessageV2
    {0x814d4f02, "CFW_MeListMessageV2"},
    #endif
    #ifndef DISABLE_RPC_hal_WriteGsmCalib
    {0x81c945ab, "hal_WriteGsmCalib"},
    #endif
    #ifndef DISABLE_RPC_hal_zspVoiceDtmfStart
    {0x81fab3a7, "hal_zspVoiceDtmfStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetLteSignalQuality
    {0x81fd8a88, "CFW_NwGetLteSignalQuality"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetRegistration
    {0x825d1b6c, "CFW_NwSetRegistration"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsCtxModify
    {0x8271116b, "CFW_GprsCtxModify"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetQualReport
    {0x8291ead7, "CFW_NwGetQualReport"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetNewSmsOptionMT
    {0x82b90fdb, "CFW_CfgSetNewSmsOptionMT"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodReadBindSIM
    {0x82ccd1ad, "CFW_EmodReadBindSIM"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetLoudspeakerVolumeLevel
    {0x832b47ad, "CFW_CfgSetLoudspeakerVolumeLevel"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetCSMode
    {0x8372d825, "CFW_NwSetCSMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetPsDataAttribute
    {0x84beca5c, "CFW_SetPsDataAttribute"},
    #endif
    #if defined(CSG_SUPPORT)
    #ifndef DISABLE_RPC_CFW_NwSetCsgMode
    {0x84c8a6b6, "CFW_NwSetCsgMode"},
    #endif
    #endif
    #if defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_NwSetEpsMode
    {0x84d40ffe, "CFW_NwSetEpsMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_NWGetRat
    {0x85e2fdf5, "CFW_NWGetRat"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetRatePriority
    {0x85f6ac6b, "CFW_GetRatePriority"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetCiotUpciot
    {0x85fad279, "CFW_nvGetCiotUpciot"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimWakeup
    {0x85fc0cc2, "CFW_SimWakeup"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetTimerManagement
    {0x860bd0dd, "CFW_SatGetTimerManagement"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetPdpAddr
    {0x86a85765, "CFW_GprsGetPdpAddr"},
    #endif
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmLRX
    {0x86e9ee4d, "CFW_RpmGetRpmLRX"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetNvUpdateFlag
    {0x87232781, "CFW_SetNvUpdateFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsGetSmsType
    {0x87cb3392, "CFW_SmsGetSmsType"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetPbkStrorageInfo
    {0x87cf01e4, "CFW_SimGetPbkStrorageInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetCopsTimer
    {0x87f5eb5f, "CFW_nvSetCopsTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetCurrentCmd
    {0x880f39b1, "CFW_SatGetCurrentCmd"},
    #endif
    #ifndef DISABLE_RPC_CSW_SetAudioCodecCalibParam
    {0x8855397e, "CSW_SetAudioCodecCalibParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_GprsIpDnsInfo
    {0x88c37c19, "CFW_GprsIpDnsInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetIMSI
    {0x88d9b097, "CFW_CfgSetIMSI"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockDataProcess
    {0x88f002d6, "CFW_SimlockDataProcess"},
    #endif
    #endif
    #ifndef DISABLE_RPC_hal_btVoiceStop
    {0x8945fea8, "hal_btVoiceStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsQueryColp
    {0x895b63e7, "CFW_SsQueryColp"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsRemovePdpCxt
    {0x898b83e5, "CFW_GprsRemovePdpCxt"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CcReleaseCall_V2
    {0x89cdc100, "CFW_CcReleaseCall_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsSendUSSD
    {0x89f8a01a, "CFW_SsSendUSSD"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetFreeUTI
    {0x8a0c270b, "CFW_GetFreeUTI"},
    #endif
    #ifndef DISABLE_RPC_SimSelectApplicationReq
    {0x8a2c09bb, "SimSelectApplicationReq"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmCpdpx
    {0x8a89e7a2, "CFW_RpmGetRpmCpdpx"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetLTEFreqPwrRange
    {0x8a98fad2, "CFW_SetLTEFreqPwrRange"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendData
    {0x8ac9c0f6, "CFW_SatGetSendData"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetMenuNextItem
    {0x8b78bf05, "CFW_SatGetMenuNextItem"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetCiotErwopdn
    {0x8b872a87, "CFW_nvGetCiotErwopdn"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSetupIdleMode
    {0x8bc4f81c, "CFW_SatGetSetupIdleMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetCiotApnRateCtrl
    {0x8bd8eef9, "CFW_nvGetCiotApnRateCtrl"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetAutoAttachFlag
    {0x8beea828, "CFW_NwSetAutoAttachFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcReleaseCall
    {0x8c4457d3, "CFW_CcReleaseCall"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetSmsTotalNum
    {0x8c9061a8, "CFW_SimGetSmsTotalNum"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetStatusInfo
    {0x8ca9e9f1, "CFW_GetStatusInfo"},
    #endif
    #if defined(CSG_SUPPORT)
    #ifndef DISABLE_RPC_CFW_NwCSGSetRegistration
    {0x8d09aa3f, "CFW_NwCSGSetRegistration"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_ImsSsUtAddrSet
    {0x8d3ed232, "CFW_ImsSsUtAddrSet"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcGetCurrentCall
    {0x8da891d9, "CFW_CcGetCurrentCall"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetAntenna
    {0x8dae8ade, "CFW_CfgSetAntenna"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetClip
    {0x8ddaf837, "CFW_CfgSetClip"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetHNBName
    {0x8e089634, "CFW_NwGetHNBName"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetEdrxPtw
    {0x8e6db5f0, "CFW_nvGetEdrxPtw"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetPsmT3324
    {0x8eb3f586, "CFW_nvSetPsmT3324"},
    #endif
    #ifndef DISABLE_RPC_audevPlayVoiceTone
    {0x8ec00598, "audevPlayVoiceTone"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetRRCRelEx
    {0x8ed2d272, "CFW_SetRRCRelEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetEdrxEnable
    {0x8efa91ee, "CFW_nvGetEdrxEnable"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetCdmaImsi
    {0x8f00356e, "CFW_CfgGetCdmaImsi"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetEleFenceTimer
    {0x8f35ead7, "CFW_GetEleFenceTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetLockBCCH
    {0x8f8129ef, "CFW_NwSetLockBCCH"},
    #endif
    #ifndef DISABLE_RPC_CFW_ImsSrvccCapSet
    {0x8fdc72bf, "CFW_ImsSrvccCapSet"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetLockBCCH
    {0x9052629e, "CFW_NwGetLockBCCH"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetLteLockBandV2
    {0x906a0cc4, "CFW_EmodGetLteLockBandV2"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetDefaultPdnMode
    {0x90c3c37c, "CFW_GprsGetDefaultPdnMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetCiotUpciot
    {0x9189bba7, "CFW_nvSetCiotUpciot"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimDeleteMessage_V2
    {0x91e34f6a, "CFW_SimDeleteMessage_V2"},
    #endif
    #ifndef DISABLE_RPC_DM_AudSetToneMode
    {0x91f12bb2, "DM_AudSetToneMode"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsInit
    {0x92d862ef, "CFW_ImsInit"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetConnectResultFormatCode
    {0x92ddeb2c, "CFW_CfgGetConnectResultFormatCode"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodResetImsDelta
    {0x931bd91a, "CFW_EmodResetImsDelta"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetMeProfile
    {0x932a095d, "CFW_SimSetMeProfile"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmN1
    {0x9354d639, "CFW_RpmGetRpmN1"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_WifiSentestOn
    {0x93880df6, "CFW_WifiSentestOn"},
    #endif
    #ifndef DISABLE_RPC_aud_Setup
    {0x940784bc, "aud_Setup"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetFreqScanAllInfo
    {0x94229cdd, "CFW_GetFreqScanAllInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetOpenChannelDestAddress
    {0x944ddde9, "CFW_SatGetOpenChannelDestAddress"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetReqQos
    {0x945f113e, "CFW_GprsSetReqQos"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetSignalQuatityFirst
    {0x94665baf, "CFW_SetSignalQuatityFirst"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimCloseEx
    {0x946ce97f, "CFW_SimCloseEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetLocPriority
    {0x94701277, "CFW_nvGetLocPriority"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetFreqScanEnd
    {0x94ee01ff, "CFW_GetFreqScanEnd"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetGetInKeyText
    {0x94f6a9de, "CFW_SatGetGetInKeyText"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendDTMFInfo
    {0x954447f6, "CFW_SatGetSendDTMFInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetGsmGprsOnly
    {0x9587b351, "CFW_GetGsmGprsOnly"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetMNCLen
    {0x95b333c9, "CFW_GetMNCLen"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetLteLockBand
    {0x95cd0c5f, "CFW_EmodGetLteLockBand"},
    #endif
    #if defined(CSG_SUPPORT)
    #ifndef DISABLE_RPC_CFW_NwGetCsgOperators
    {0x95df4b83, "CFW_NwGetCsgOperators"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetLCTimerEx
    {0x95e3238d, "CFW_GetLCTimerEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetACM
    {0x95e61408, "CFW_SimSetACM"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendDTMF
    {0x9618247d, "CFW_SatGetSendDTMF"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwChangeRat
    {0x96567f1f, "CFW_NwChangeRat"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetComm
    {0x965f544e, "CFW_GetComm"},
    #endif
    #ifndef DISABLE_RPC_vois_TestModeSetup
    {0x97137f27, "vois_TestModeSetup"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetSignalQuality
    {0x9723220b, "CFW_NwGetSignalQuality"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodOutfieldTestStart
    {0x972830bc, "CFW_EmodOutfieldTestStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetUsimAID
    {0x972bf726, "CFW_GetUsimAID"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetOperatorIdWithIndex
    {0x98d26da8, "CFW_CfgNwGetOperatorIdWithIndex"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetNvData
    {0x9912b42b, "CFW_EmodGetNvData"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetCharacterSet
    {0x991e05de, "CFW_CfgSetCharacterSet"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetGcfTest
    {0x994da05e, "CFW_SetGcfTest"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodAudioTestStart
    {0x995d38e0, "CFW_EmodAudioTestStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_CbWriteContext
    {0x997a2ae4, "CFW_CbWriteContext"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimTPDUCommand
    {0x99d7b65b, "CFW_SimTPDUCommand"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcAcceptSpeechCallEx
    {0x9a1144fe, "CFW_CcAcceptSpeechCallEx"},
    #endif
    #ifndef DISABLE_RPC_SMS_DM_Init_V2
    {0x9a20ffcf, "SMS_DM_Init_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetInitWithSim
    {0x9a24d26a, "CFW_SetInitWithSim"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetPSMThd
    {0x9aca73b9, "CFW_EmodSetPSMThd"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsMMAReq
    {0x9ae2bc1a, "CFW_SmsMMAReq"},
    #endif
    #ifndef DISABLE_RPC_CFW_CheckSupportLteBand
    {0x9ae742a6, "CFW_CheckSupportLteBand"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetStatus
    {0x9af699bd, "CFW_NwGetStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetResultCodePresentMode
    {0x9b269a8d, "CFW_CfgSetResultCodePresentMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLctMaxLen
    {0x9bbb10e9, "CFW_GetLctMaxLen"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockSetHuk
    {0x9bd52aeb, "CFW_SimlockSetHuk"},
    #endif
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsDisable
    {0x9c72a751, "CFW_ImsDisable"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimUpdateRecordAllParam
    {0x9c96a6bb, "CFW_SimUpdateRecordAllParam"},
    #endif
    #ifndef DISABLE_RPC_DM_StopAudioEx
    {0x9cfd69ae, "DM_StopAudioEx"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_SsSetClip
    {0x9d929a96, "CFW_SsSetClip"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetNwStatus
    {0x9dccde75, "CFW_CfgSetNwStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_GenerateSoftSimImsi
    {0x9defd872, "CFW_GenerateSoftSimImsi"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetBootCausesFromAP
    {0x9e161177, "CFW_GetBootCausesFromAP"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsReadMessage
    {0x9e37b0ea, "CFW_SmsReadMessage"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsIsSet
    {0x9f1f9c07, "CFW_ImsIsSet"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetDFStatus
    {0x9fa5b5f2, "CFW_SimGetDFStatus"},
    #endif
    #ifndef DISABLE_RPC_hal_zspVoiceRecordStop
    {0xa043759e, "hal_zspVoiceRecordStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetPhoneFunctionality
    {0xa046cd5e, "CFW_CfgGetPhoneFunctionality"},
    #endif
    #ifndef DISABLE_RPC_CFW_GprsGetInternalPdpCtx
    {0xa09ae088, "CFW_GprsGetInternalPdpCtx"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodImsNvParamGet
    {0xa0c22862, "CFW_EmodImsNvParamGet"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSmsStorage
    {0xa0dae657, "CFW_CfgGetSmsStorage"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsGetSrvccMode
    {0xa1821af0, "CFW_ImsGetSrvccMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_LcsMtlrRsp
    {0xa19ed215, "CFW_LcsMtlrRsp"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetCDACValue
    {0xa1ad370d, "CFW_GetCDACValue"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetRauAfterCallFlag
    {0xa228dde6, "CFW_SetRauAfterCallFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetOperatorName
    {0xa2528a6c, "CFW_SimGetOperatorName"},
    #endif
    #ifndef DISABLE_RPC_CFW_NWGetStackRat
    {0xa2cd3816, "CFW_NWGetStackRat"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetPowerMode
    {0xa2faad2c, "CFW_EmodSetPowerMode"},
    #endif
    #ifndef DISABLE_RPC_vois_GetSampleRate
    {0xa380ab55, "vois_GetSampleRate"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetFrequencyBand
    {0xa3e4654b, "CFW_CfgNwGetFrequencyBand"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgStoreUserProfile
    {0xa40eca33, "CFW_CfgStoreUserProfile"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwJammingRssiEnquiry
    {0xa41e015d, "CFW_NwJammingRssiEnquiry"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsSuspend
    {0xa45579e4, "CFW_ImsSuspend"},
    #endif
    #endif
    #ifndef DISABLE_RPC_vois_SetBtNRECFlag
    {0xa4d73f30, "vois_SetBtNRECFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetUserProfile
    {0xa53c084f, "CFW_CfgGetUserProfile"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendSmsServiceCenterAddr
    {0xa54f5bc9, "CFW_SatGetSendSmsServiceCenterAddr"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmSetRpmLRX
    {0xa5d0fd53, "CFW_RpmSetRpmLRX"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimDeletePbkEntry
    {0xa64720ca, "CFW_SimDeletePbkEntry"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSendData
    {0xa66b1aa9, "CFW_GprsSendData"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SsQueryClir
    {0xa6720283, "CFW_SsQueryClir"},
    #endif
    #ifndef DISABLE_RPC_aud_SetLoopbackGain
    {0xa685caf5, "aud_SetLoopbackGain"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetSmsParameters
    {0xa6965bff, "CFW_SimGetSmsParameters"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetSuspend
    {0xa6d772db, "CFW_SimSetSuspend"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodReadBindNW
    {0xa6eed8f0, "CFW_EmodReadBindNW"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetPlayToneText
    {0xa7a14524, "CFW_SatGetPlayToneText"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetPrefOperatorListMaxNum
    {0xa7a1c43e, "CFW_SimGetPrefOperatorListMaxNum"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetProiorityResetFlag
    {0xa7b92ac0, "CFW_SimSetProiorityResetFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetResultCodeFormatMode
    {0xa7d4e623, "CFW_CfgSetResultCodeFormatMode"},
    #endif
    #ifndef DISABLE_RPC_AUD_VoiceCoderChangeInd
    {0xa800f417, "AUD_VoiceCoderChangeInd"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsSetUnSent2Sent
    {0xa8596703, "CFW_SmsSetUnSent2Sent"},
    #endif
    #ifndef DISABLE_RPC_CFW_UsimDecodeEFFcp
    {0xa97b3bc7, "CFW_UsimDecodeEFFcp"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmSetRpmN1
    {0xa9923452, "CFW_RpmSetRpmN1"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSSN
    {0xa99abe2e, "CFW_CfgGetSSN"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSmsFormat
    {0xa9f316f2, "CFW_CfgSetSmsFormat"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetResume
    {0xaa3206ac, "CFW_SimSetResume"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetConnectResultFormatCode
    {0xaa3996b2, "CFW_CfgSetConnectResultFormatCode"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetPocPara
    {0xaa43af92, "CFW_EmodSetPocPara"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetPbkEntry
    {0xaa8591be, "CFW_SimGetPbkEntry"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsGetTerminalMode
    {0xab3d76b5, "CFW_ImsGetTerminalMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetNwSearchPrior
    {0xab42d767, "CFW_nvSetNwSearchPrior"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmCBR1
    {0xab47683b, "CFW_RpmGetRpmCBR1"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetIncomingCallResultMode
    {0xab6bc7f1, "CFW_CfgGetIncomingCallResultMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetLteLockFreq
    {0xab7975c4, "CFW_EmodGetLteLockFreq"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetIMEISVFromAP
    {0xab8a16e3, "CFW_GetIMEISVFromAP"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsSendMessage_V2
    {0xab91f894, "CFW_SmsSendMessage_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetNwStatus
    {0xabfd1d2f, "CFW_CfgGetNwStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetNetDLPsmParam
    {0xac0be1d6, "CFW_GetNetDLPsmParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_IsFreeUTI
    {0xac2b1b66, "CFW_IsFreeUTI"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsSendUSSD_V2
    {0xac92524d, "CFW_SsSendUSSD_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetAvailableOperators_V2
    {0xac9a08fc, "CFW_NwGetAvailableOperators_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcRejectCall
    {0xace9ca72, "CFW_CcRejectCall"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_SsSetClir
    {0xad7d0f90, "CFW_SsSetClir"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimType
    {0xadfbc835, "CFW_GetSimType"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodAudioTestEnd
    {0xae454ab0, "CFW_EmodAudioTestEnd"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmSetRpmT1
    {0xae5d7c49, "CFW_RpmSetRpmT1"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetNewSmsOptionMT
    {0xae86e23a, "CFW_CfgGetNewSmsOptionMT"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetBlackList
    {0xaf486c1a, "CFW_NwSetBlackList"},
    #endif
    #ifndef DISABLE_RPC_aud_StreamRecord
    {0xafcd013d, "aud_StreamRecord"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSoftReset
    {0xb02b949a, "CFW_CfgSetSoftReset"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLteBlkList
    {0xb06bbc57, "CFW_GetLteBlkList"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetStackSimFileID
    {0xb0ff5bb2, "CFW_GetStackSimFileID"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetPSMThd
    {0xb1320865, "CFW_EmodGetPSMThd"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetTracePort
    {0xb17252a7, "CFW_EmodSetTracePort"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetReqQosUmts
    {0xb195ac35, "CFW_GprsGetReqQosUmts"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSubMenuNextItem
    {0xb27d5400, "CFW_SatGetSubMenuNextItem"},
    #endif
    #ifndef DISABLE_RPC_SimSelectApplicationReqV2
    {0xb2f6762f, "SimSelectApplicationReqV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_InvalideTmsiPTmis
    {0xb30bafe9, "CFW_InvalideTmsiPTmis"},
    #endif
    #ifndef DISABLE_RPC_CFW_WifiSentestGetSingleRes
    {0xb30c96c5, "CFW_WifiSentestGetSingleRes"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimChangePassword
    {0xb3513926, "CFW_SimChangePassword"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetCallWaiting
    {0xb36a4c39, "CFW_CfgGetCallWaiting"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetLctLoopCout
    {0xb37d6dc0, "CFW_SetLctLoopCout"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGrrLteFreq
    {0xb3b37874, "CFW_EmodGrrLteFreq"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetPhoneFunctionality
    {0xb4319259, "CFW_CfgSetPhoneFunctionality"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSmsOverflowInd
    {0xb4b7a4da, "CFW_CfgSetSmsOverflowInd"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimAddPbkEntry_V2
    {0xb5beb0cf, "CFW_SimAddPbkEntry_V2"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDetectMBS
    {0xb691ecb2, "CFW_GetDetectMBS"},
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetPsm
    {0xb70c6d65, "CFW_GprsSetPsm"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SsChangePassword
    {0xb7128488, "CFW_SsChangePassword"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSignalQuatityFirst
    {0xb73a2d50, "CFW_GetSignalQuatityFirst"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsResume
    {0xb76466e9, "CFW_ImsResume"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_NWSetRat
    {0xb7e77ecb, "CFW_NWSetRat"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetCdmaImsi
    {0xb80b31fa, "CFW_SimGetCdmaImsi"},
    #endif
    #ifndef DISABLE_RPC_CFW_ErrcGetMeasThreshold
    {0xb824518f, "CFW_ErrcGetMeasThreshold"},
    #endif
    #ifndef DISABLE_RPC_CSW_SetAndGetMicGain
    {0xb872117e, "CSW_SetAndGetMicGain"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSimSwitch
    {0xb9697fc4, "CFW_CfgGetSimSwitch"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetVoicePrefer
    {0xb9a37a07, "CFW_EmodSetVoicePrefer"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetACM
    {0xba3d62fe, "CFW_SimGetACM"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmFlag
    {0xba690c45, "CFW_RpmGetRpmFlag"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetLanguage
    {0xba963a67, "CFW_SimGetLanguage"},
    #endif
    #ifndef DISABLE_RPC_CFW_CheckSupportLteFreq
    {0xbad51467, "CFW_CheckSupportLteFreq"},
    #endif
    #ifndef DISABLE_RPC_DM_StopToneEx
    {0xbb249b21, "DM_StopToneEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsMtSmsPPAckReqWithData
    {0xbb4b1efb, "CFW_SmsMtSmsPPAckReqWithData"},
    #endif
    #ifndef DISABLE_RPC_CFW_MeReadMessageV2
    {0xbb8bcea8, "CFW_MeReadMessageV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetMenuFirstItem
    {0xbbaeb22f, "CFW_SatGetMenuFirstItem"},
    #endif
    #ifndef DISABLE_RPC_CSW_SetAudioZspPostProcessCalibParam
    {0xbc760df9, "CSW_SetAudioZspPostProcessCalibParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetNMR
    {0xbcbd6bab, "CFW_GetNMR"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetPdpIpv4Addr
    {0xbccda50b, "CFW_GprsGetPdpIpv4Addr"},
    #endif
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmGetRpmT1
    {0xbd21ed6e, "CFW_RpmGetRpmT1"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetStatus
    {0xbdaa958e, "CFW_SimGetStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimSPN
    {0xbdd83a6a, "CFW_GetSimSPN"},
    #endif
    #ifndef DISABLE_RPC_DM_AudSetAudDeviceCFG
    {0xbdea98d9, "DM_AudSetAudDeviceCFG"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetDefaultSmsParam
    {0xbe60af25, "CFW_CfgGetDefaultSmsParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetNetTAU
    {0xbf10cb36, "CFW_NwSetNetTAU"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetLocalRelease
    {0xbf73f746, "CFW_NwSetLocalRelease"},
    #endif
    #ifndef DISABLE_RPC_DM_ZspVoiceVibrateStop
    {0xbf76903d, "DM_ZspVoiceVibrateStop"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmCheckAllowRebootingPdpActiving
    {0xbf9e8a21, "CFW_RpmCheckAllowRebootingPdpActiving"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SmsGetMessageNodeV2
    {0xc019ab50, "CFW_SmsGetMessageNodeV2"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsSendPduMessage
    {0xc021092d, "CFW_SmsSendPduMessage"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetLmtLoopCout
    {0xc0392db9, "CFW_SetLmtLoopCout"},
    #endif
    #ifndef DISABLE_RPC_AUD_SetCodecOpStatus
    {0xc070ad19, "AUD_SetCodecOpStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetResultCodeFormatMode
    {0xc0bd302c, "CFW_CfgGetResultCodeFormatMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetOpenChannelNetInfoV2
    {0xc0c96aaf, "CFW_SatGetOpenChannelNetInfoV2"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockAuthStart
    {0xc110d14b, "CFW_SimlockAuthStart"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSoftReset
    {0xc122799c, "CFW_CfgGetSoftReset"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_AttDetach
    {0xc1988543, "CFW_AttDetach"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSubMenuFirstItem
    {0xc1dcd2f0, "CFW_SatGetSubMenuFirstItem"},
    #endif
    #ifndef DISABLE_RPC_rpcPeerTagCount
    {0xc206373a, "rpcPeerTagCount"},
    #endif
    #ifndef DISABLE_RPC_SimPollOffReq
    {0xc24a1cf0, "SimPollOffReq"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmSetRpmFx
    {0xc26e014f, "CFW_RpmSetRpmFx"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetCellInfo
    {0xc361faa4, "CFW_GetCellInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetFTA
    {0xc37f54c0, "CFW_NwGetFTA"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSmsCB
    {0xc3a3200e, "CFW_CfgSetSmsCB"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwSetNetWorkMode
    {0xc40a65b9, "CFW_CfgNwSetNetWorkMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetIncomingCallResultMode
    {0xc4bee657, "CFW_CfgSetIncomingCallResultMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetLpsFlag
    {0xc4c58443, "CFW_EmodGetLpsFlag"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_SsSetColp
    {0xc637a64f, "CFW_SsSetColp"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodL1Param
    {0xc69c0723, "CFW_EmodL1Param"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockAuthEnd
    {0xc6b42805, "CFW_SimlockAuthEnd"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SmsGetMessageNodeEx
    {0xc6e5fb72, "CFW_SmsGetMessageNodeEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSmsCB
    {0xc74fce6d, "CFW_CfgGetSmsCB"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsSetTerminalMode
    {0xc79f2975, "CFW_ImsSetTerminalMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetEID
    {0xc7bcf189, "CFW_SimGetEID"},
    #endif
    #ifndef DISABLE_RPC_DM_AudStreamStart
    {0xc82b02a0, "DM_AudStreamStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimInitStage1
    {0xc82ff59a, "CFW_SimInitStage1"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetJammingDetectParamEx
    {0xc8470e23, "CFW_SetJammingDetectParamEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimWritePreferPLMN
    {0xc88ef4b9, "CFW_SimWritePreferPLMN"},
    #endif
    #ifndef DISABLE_RPC_CFW_WifiScanStart
    {0xc8d170b5, "CFW_WifiScanStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetNwSearchPrior
    {0xc916fa48, "CFW_nvGetNwSearchPrior"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetCID
    {0xc921d725, "CFW_SetCID"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSmsShowTextModeParam
    {0xcaec4d3b, "CFW_CfgGetSmsShowTextModeParam"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockImeiProcess
    {0xcb17fa6c, "CFW_SimlockImeiProcess"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetDefaultPcoControl
    {0xcb1adce5, "CFW_GetDefaultPcoControl"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadVoiceMailInfo
    {0xcb66a059, "CFW_SimReadVoiceMailInfo"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_rpmNvSetLastRat
    {0xcb9c77c8, "CFW_rpmNvSetLastRat"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetRFCalibrationInfo
    {0xcbb6d554, "CFW_EmodGetRFCalibrationInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetErrorReport
    {0xcbc0852d, "CFW_CfgGetErrorReport"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetIndicatorEvent
    {0xcbe35db9, "CFW_CfgSetIndicatorEvent"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsManualRej
    {0xcc80601e, "CFW_GprsManualRej"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetVoicePrefer
    {0xccbe3de8, "CFW_EmodGetVoicePrefer"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetPbkStorage
    {0xcd436267, "CFW_CfgGetPbkStorage"},
    #endif
    #ifndef DISABLE_RPC_hal_zspMusicPlayStart
    {0xce8bf0bd, "hal_zspMusicPlayStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetSysConfig
    {0xcfe5dd7b, "CFW_NwGetSysConfig"},
    #endif
    #ifndef DISABLE_RPC_CFW_CbAct
    {0xd02d8d7a, "CFW_CbAct"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetSmsParameters
    {0xd0b065ed, "CFW_SimSetSmsParameters"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcGetCurrentCallV2
    {0xd0d78967, "CFW_CcGetCurrentCallV2"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmWriteDynamicEx
    {0xd17755fb, "CFW_RpmWriteDynamicEx"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetBlackWhiteList
    {0xd17ca50b, "CFW_NwSetBlackWhiteList"},
    #endif
    #ifndef DISABLE_RPC_CFW_ImsSetUaVersionInfo
    {0xd182b1b4, "CFW_ImsSetUaVersionInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetPUCT
    {0xd1853aaf, "CFW_SimGetPUCT"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetRawData
    {0xd1dc69f6, "CFW_SatGetRawData"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimReadRecord
    {0xd1edf62e, "CFW_SimReadRecord"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetSmsSeviceMode
    {0xd21e74d0, "CFW_GprsSetSmsSeviceMode"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetFreqScanInfo
    {0xd27ec527, "CFW_GetFreqScanInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetFacilityLock
    {0xd28c1f50, "CFW_SimGetFacilityLock"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcCallHoldMultiparty
    {0xd2b4a305, "CFW_CcCallHoldMultiparty"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwSetFrequencyBand
    {0xd2cedcbb, "CFW_CfgNwSetFrequencyBand"},
    #endif
    #ifndef DISABLE_RPC_CFW_WriteLongSmsInfo
    {0xd2f01f11, "CFW_WriteLongSmsInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetSmsParam
    {0xd480bf02, "CFW_CfgSetSmsParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetCiotNonip
    {0xd4f02abd, "CFW_nvSetCiotNonip"},
    #endif
    #ifndef DISABLE_RPC_aud_SetSharkingMode
    {0xd5432923, "aud_SetSharkingMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetEchoMode
    {0xd5b14f3b, "CFW_CfgSetEchoMode"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetPowerOffFlag
    {0xd5d4a325, "CFW_SetPowerOffFlag"},
    #endif
    #ifndef DISABLE_RPC_DM_AudStreamRecord
    {0xd62f0efc, "DM_AudStreamRecord"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetLctMaxLen
    {0xd639038c, "CFW_SetLctMaxLen"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetClir
    {0xd6d006b2, "CFW_CfgGetClir"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsQueryColr
    {0xd6d9fa1b, "CFW_SsQueryColr"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetFacilityLock
    {0xd6fab239, "CFW_SimSetFacilityLock"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetRauAfterCallFlag
    {0xd74b689f, "CFW_GetRauAfterCallFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsMemoryFullReq
    {0xd7655a97, "CFW_SmsMemoryFullReq"},
    #endif
    #if defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_CcReleaseCallX_V2
    {0xd788206a, "CFW_CcReleaseCallX_V2"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetQualReport
    {0xd7c9260b, "CFW_SetQualReport"},
    #endif
    #ifndef DISABLE_RPC_CFW_NstGetrssi
    {0xd892ac3c, "CFW_NstGetrssi"},
    #endif
    #ifndef DISABLE_RPC_aud_CodecNvInit
    {0xd8dd866b, "aud_CodecNvInit"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetScanRssiInfo
    {0xd908dbba, "CFW_GetScanRssiInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetEnhancePwrSweepNonCE
    {0xd9326b3e, "CFW_CfgGetEnhancePwrSweepNonCE"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetPocEnable
    {0xd934d4ab, "CFW_CfgSetPocEnable"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetTracePort
    {0xd942df90, "CFW_EmodGetTracePort"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetBandFromFreq
    {0xd9707705, "CFW_GetBandFromFreq"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetSimSwitch
    {0xd97f8dc8, "CFW_NwGetSimSwitch"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDefaultApn
    {0xda5560c6, "CFW_GetDefaultApn"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimEnterAuthentication
    {0xda558016, "CFW_SimEnterAuthentication"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLmtLoopCout
    {0xdb1c569e, "CFW_GetLmtLoopCout"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsListMessages
    {0xdb8ad850, "CFW_SmsListMessages"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsIsRegistered
    {0xdbf659dd, "CFW_ImsIsRegistered"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetPollIntervalDuration
    {0xdc6c3250, "CFW_SatGetPollIntervalDuration"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetNasTimer
    {0xdc96ae6b, "CFW_nvSetNasTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetFTA
    {0xdcd3a5a9, "CFW_NwSetFTA"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmSetRpmFlag
    {0xdd4f1cb3, "CFW_RpmSetRpmFlag"},
    #endif
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsSetMinQos
    {0xdd6267c5, "CFW_GprsSetMinQos"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimPHYType
    {0xddaae762, "CFW_GetSimPHYType"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatTerminalResponse
    {0xde092a0a, "CFW_SatTerminalResponse"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimLockStatusFromAP
    {0xde45ef05, "CFW_GetSimLockStatusFromAP"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsSetUnRead2Read
    {0xde75aebe, "CFW_SmsSetUnRead2Read"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetLctLoopCout
    {0xdec27c03, "CFW_GetLctLoopCout"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetNetinfoStatistic
    {0xdf39cd5c, "CFW_GetNetinfoStatistic"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetMoSmsRetryCount
    {0xdf530c31, "CFW_SetMoSmsRetryCount"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgCheckFirstPowerOnFlag
    {0xdfab74a6, "CFW_CfgCheckFirstPowerOnFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendSmsTPDU
    {0xdfdc87cc, "CFW_SatGetSendSmsTPDU"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetDisplayText
    {0xdfe1b99e, "CFW_SatGetDisplayText"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GetGprsAttState
    {0xe07bd895, "CFW_GetGprsAttState"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatRefreshImsiChange
    {0xe0991e80, "CFW_SatRefreshImsiChange"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwIsUseISIM
    {0xe0a7fc13, "CFW_NwIsUseISIM"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSelectSmsService
    {0xe1d549f5, "CFW_CfgSelectSmsService"},
    #endif
    #ifndef DISABLE_RPC_DM_AudStreamRecordStop
    {0xe2469570, "DM_AudStreamRecordStop"},
    #endif
    #ifndef DISABLE_RPC_CFW_LcsMolrReq
    {0xe25ebe5c, "CFW_LcsMolrReq"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvGetCiotSmsWoCombAtt
    {0xe2f19f4e, "CFW_nvGetCiotSmsWoCombAtt"},
    #endif
    #ifndef DISABLE_RPC_PCNgisYfirev
    {0xe377c554, "PCNgisYfirev"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsGetPdpStatus
    {0xe3bacaaf, "CFW_ImsGetPdpStatus"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetJammingDetectParam
    {0xe49bfc11, "CFW_SetJammingDetectParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_ShellControl
    {0xe4a00223, "CFW_ShellControl"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsAtt
    {0xe4cc5064, "CFW_GprsAtt"},
    #endif
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsEnable
    {0xe4ed0761, "CFW_ImsEnable"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetNwOperDispFormat
    {0xe503676d, "CFW_CfgSetNwOperDispFormat"},
    #endif
    #ifndef DISABLE_RPC_CFW_EnableLpsLock
    {0xe5455e3a, "CFW_EnableLpsLock"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetSendUSSDInfo
    {0xe58d210f, "CFW_SatGetSendUSSDInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsMoInit
    {0xe5fdf9e0, "CFW_SmsMoInit"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimInitStage3
    {0xe600a85c, "CFW_SimInitStage3"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsActImsPdp
    {0xe60f5b9d, "CFW_ImsActImsPdp"},
    #endif
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockEncryptKeys
    {0xe6161356, "CFW_SimlockEncryptKeys"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SimGetOperationTimes
    {0xe6561ada, "CFW_SimGetOperationTimes"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetDefaultSmsParam
    {0xe6b2eaa7, "CFW_CfgSetDefaultSmsParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetT3302
    {0xe6ce07c9, "CFW_SetT3302"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetPlmn
    {0xe803ca57, "CFW_GetPlmn"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodGetGsmCalibInfo
    {0xe8f8e512, "CFW_EmodGetGsmCalibInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsSetCallWaiting
    {0xe9225666, "CFW_SsSetCallWaiting"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcExtConfCallDial
    {0xe94b0133, "CFW_CcExtConfCallDial"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetStoredPlmnList
    {0xe96c6a34, "CFW_CfgGetStoredPlmnList"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetLocPriority
    {0xe9bd1df9, "CFW_nvSetLocPriority"},
    #endif
    #ifndef DISABLE_RPC_vois_SetCfg
    {0xe9d0fbdf, "vois_SetCfg"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_CheckTestSim
    {0xea05e9c2, "CFW_CheckTestSim"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GetT3302
    {0xea2a7b41, "CFW_GetT3302"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetSmsSeviceMode
    {0xea3e6cae, "CFW_GprsGetSmsSeviceMode"},
    #endif
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetCtxEQos
    {0xea5153a8, "CFW_GprsGetCtxEQos"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GprsClrIpDns
    {0xeb30b3e0, "CFW_GprsClrIpDns"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimUpdateBinary
    {0xeb4d03f5, "CFW_SimUpdateBinary"},
    #endif
    #if defined(RPM_SUPPORT)
    #ifndef DISABLE_RPC_CFW_RpmSetRpmCR1
    {0xebd14092, "CFW_RpmSetRpmCR1"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CSW_SetAudioZspNxpCalibParam
    {0xec6140b2, "CSW_SetAudioZspNxpCalibParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_CheckIfAllowedRecvSms
    {0xee15e996, "CFW_CheckIfAllowedRecvSms"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetLteLockFreq
    {0xee64a452, "CFW_EmodSetLteLockFreq"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetPbkStrorageInfo
    {0xeee2d88b, "CFW_CfgGetPbkStrorageInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimStatus
    {0xeeef512f, "CFW_GetSimStatus"},
    #endif
    #ifndef DISABLE_RPC_CFW_CbSetContext
    {0xef062af4, "CFW_CbSetContext"},
    #endif
    #ifndef DISABLE_RPC_DM_ZspMusicVibrateStart
    {0xef602d9c, "DM_ZspMusicVibrateStart"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodRrtmParam
    {0xef959240, "CFW_EmodRrtmParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwErrcStatisCfg
    {0xefd5858c, "CFW_NwErrcStatisCfg"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetReqQos
    {0xf0322972, "CFW_GprsGetReqQos"},
    #endif
    #endif
    #ifndef DISABLE_RPC_cfw_SimReadElementaryFile
    {0xf15912cb, "cfw_SimReadElementaryFile"},
    #endif
    #ifndef DISABLE_RPC_CFW_ReleaseCID
    {0xf1dcf009, "CFW_ReleaseCID"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetLmtMaxLen
    {0xf236f32c, "CFW_SetLmtMaxLen"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetSimFileUpdateCount
    {0xf25dd62c, "CFW_GetSimFileUpdateCount"},
    #endif
    #ifndef DISABLE_RPC_Cfw_SetTimezone
    {0xf2ae4a97, "Cfw_SetTimezone"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetSmsParam
    {0xf2b23c71, "CFW_CfgGetSmsParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetEventReport
    {0xf37b9eb1, "CFW_CfgSetEventReport"},
    #endif
    #ifndef DISABLE_RPC_CFW_CcInitiateSpeechCallEx_V2
    {0xf3beb108, "CFW_CcInitiateSpeechCallEx_V2"},
    #endif
    #ifndef DISABLE_RPC_DM_ZspMusicPlayStartEx
    {0xf3e17fe9, "DM_ZspMusicPlayStartEx"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetGsmBlkList
    {0xf40e815a, "CFW_GetGsmBlkList"},
    #endif
    #ifndef DISABLE_RPC_CFW_SmsSetRead2UnRead
    {0xf4a2809f, "CFW_SmsSetRead2UnRead"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimDeleteMessage
    {0xf4b2c147, "CFW_SimDeleteMessage"},
    #endif
    #ifndef DISABLE_RPC_aud_ClrBtClk
    {0xf514b639, "aud_ClrBtClk"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsSetVolte
    {0xf52fc960, "CFW_ImsSetVolte"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetRefreshFilesList
    {0xf559d063, "CFW_SatGetRefreshFilesList"},
    #endif
    #ifndef DISABLE_RPC_CFW_SsQueryCallForwarding
    {0xf5ab108b, "CFW_SsQueryCallForwarding"},
    #endif
    #if defined(FUNC_SUPPORT_SIMLOCK)
    #ifndef DISABLE_RPC_CFW_SimlockFacauthPubkey
    {0xf5d63e1a, "CFW_SimlockFacauthPubkey"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_GprsSendDataAvaliable
    {0xf5f97017, "CFW_GprsSendDataAvaliable"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetNWTimerOutFlag
    {0xf65e019c, "CFW_SetNWTimerOutFlag"},
    #endif
    #ifndef DISABLE_RPC_hal_WriteLteCalib
    {0xf6723896, "hal_WriteLteCalib"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodUpdateSimBind
    {0xf6a7246e, "CFW_EmodUpdateSimBind"},
    #endif
    #ifndef DISABLE_RPC_CFW_WifiSentestGetResult
    {0xf6a88248, "CFW_WifiSentestGetResult"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgNwGetOperatorId
    {0xf6dd0cb5, "CFW_CfgNwGetOperatorId"},
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_GprsGetNetEQos
    {0xf7523c18, "CFW_GprsGetNetEQos"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_SetCSPagingFlag
    {0xf7676344, "CFW_SetCSPagingFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgSetAudioOutputParam
    {0xf851b8fc, "CFW_CfgSetAudioOutputParam"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetLimiteTimer
    {0xf853cc90, "CFW_SetLimiteTimer"},
    #endif
    #ifndef DISABLE_RPC_CFW_SimSetPUCT
    {0xf85926dc, "CFW_SimSetPUCT"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodSetTraceFlag
    {0xf90dd102, "CFW_EmodSetTraceFlag"},
    #endif
    #ifndef DISABLE_RPC_aud_SetLdoVB
    {0xf91b984b, "aud_SetLdoVB"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatRefreshFileChange
    {0xf95de818, "CFW_SatRefreshFileChange"},
    #endif
    #ifndef DISABLE_RPC_CFW_nvSetEdrxPtw
    {0xf9f90144, "CFW_nvSetEdrxPtw"},
    #endif
    #ifndef DISABLE_RPC_DM_AudSetup
    {0xfaa5faa3, "DM_AudSetup"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwGetImei
    {0xfab521ed, "CFW_NwGetImei"},
    #endif
    #ifndef DISABLE_RPC_CFW_SetComm
    {0xfae6ff84, "CFW_SetComm"},
    #endif
    #ifndef DISABLE_RPC_DM_VoisSetBtNRECFlag
    {0xfaea4225, "DM_VoisSetBtNRECFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetBSF
    {0xfb1a71d8, "CFW_NwSetBSF"},
    #endif
    #ifndef DISABLE_RPC_CFW_NstConfig
    {0xfb2f1070, "CFW_NstConfig"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetnSecurityUsedFlag
    {0xfbb6e127, "CFW_NwSetnSecurityUsedFlag"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetUsrHisFreqInfo
    {0xfbda517a, "CFW_GetUsrHisFreqInfo"},
    #endif
    #ifndef DISABLE_RPC_CFW_GetDftPdnInfoFromAP
    {0xfc57b97f, "CFW_GetDftPdnInfoFromAP"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetMenuItemByID
    {0xfc7f5f83, "CFW_SatGetMenuItemByID"},
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetEventReport
    {0xfcada50f, "CFW_CfgGetEventReport"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetPocSimIndex
    {0xfcb71a0f, "CFW_NwSetPocSimIndex"},
    #endif
    #ifndef DISABLE_RPC_CFW_NwSetXCAP
    {0xfd41ebd7, "CFW_NwSetXCAP"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    #ifndef DISABLE_RPC_CFW_ImsFreeUti
    {0xfeb9adf0, "CFW_ImsFreeUti"},
    #endif
    #endif
    #ifndef DISABLE_RPC_CFW_CfgGetPeriodTauAftLocalRel
    {0xff364c32, "CFW_CfgGetPeriodTauAftLocalRel"},
    #endif
    #ifndef DISABLE_RPC_CFW_SatGetLanguageNotification
    {0xff9ba171, "CFW_SatGetLanguageNotification"},
    #endif
    #ifndef DISABLE_RPC_CFW_EmodOutfieldTestEnd
    {0xffea09ab, "CFW_EmodOutfieldTestEnd"},
    #endif
};

const char *rpcFindNameByTag(unsigned tag)
{
    void *p = bsearch((const void *)(unsigned long)tag,
                      gApiNameTable,
                      sizeof(gApiNameTable) / sizeof(gApiNameTable[0]),
                      sizeof(gApiNameTable[0]),
                      _tagCompare);

    return (p != (void *)0) ? ((tagName_t *)p)->name : "";
}

int rpcTagCount(void)
{
    return sizeof(gRpcApiTable) / sizeof(gRpcApiTable[0]);
}

unsigned char rpcTagSupported(unsigned tag)
{
    void *p = bsearch((const void *)(unsigned long)tag,
                      gRpcApiTable,
                      sizeof(gRpcApiTable) / sizeof(gRpcApiTable[0]),
                      sizeof(gRpcApiTable[0]),
                      _tagCompare);
    return (p != (void *)0) ? 1 : 0;
}

int rpcGetTags(unsigned *tags, unsigned offset, unsigned count)
{
    unsigned total = sizeof(gRpcApiTable) / sizeof(gRpcApiTable[0]);
    unsigned end = offset + count;
    unsigned n = 0;
    if (end > total)
        end = total;
    if (offset >= end)
        return 0;
    if (tags == (void *)0)
        return -1;

    for (n = offset; n < end; n++)
        tags[n] = gRpcApiTable[n].tag;
    return end - offset;
}

// AUTO GENERATED END
