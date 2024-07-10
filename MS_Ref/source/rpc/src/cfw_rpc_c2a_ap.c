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

#ifndef DISABLE_RPC_CFW_GetSSimFileInfoFromAP
void RPC_CFW_GetSSimFileInfoFromAP(void *in, void *out)
{
    struct IPAR_CFW_GetSSimFileInfoFromAP *ipar = (struct IPAR_CFW_GetSSimFileInfoFromAP *)in;
    struct OPAR_CFW_GetSSimFileInfoFromAP *opar = (struct OPAR_CFW_GetSSimFileInfoFromAP *)out;
    CFW_SSIM_FILE_INFO_T *ssim_info;
    CFW_SIM_ID nSimID;
    ssim_info = &opar->ssim_info;
    nSimID = ipar->nSimID;
    opar->result = CFW_GetSSimFileInfoFromAP(ssim_info, nSimID);
}
#endif

#ifndef DISABLE_RPC_CFW_GetDftPdnInfoFromAP
void RPC_CFW_GetDftPdnInfoFromAP(void *in, void *out)
{
    struct IPAR_CFW_GetDftPdnInfoFromAP *ipar = (struct IPAR_CFW_GetDftPdnInfoFromAP *)in;
    struct OPAR_CFW_GetDftPdnInfoFromAP *opar = (struct OPAR_CFW_GetDftPdnInfoFromAP *)out;
    CFW_GPRS_DFTPDN_INFO *nDftPdn;
    CFW_SIM_ID nSimID;
    nDftPdn = &opar->nDftPdn;
    nSimID = ipar->nSimID;
    opar->result = CFW_GetDftPdnInfoFromAP(nDftPdn, nSimID);
}
#endif

#ifndef DISABLE_RPC_CFW_GetIMEIFromAP
void RPC_CFW_GetIMEIFromAP(void *in, void *out)
{
    struct IPAR_CFW_GetIMEIFromAP *ipar = (struct IPAR_CFW_GetIMEIFromAP *)in;
    struct OPAR_CFW_GetIMEIFromAP *opar = (struct OPAR_CFW_GetIMEIFromAP *)out;
    uint8_t *pImei;
    uint8_t *pImeiLen;
    CFW_SIM_ID nSimID;
    pImei = opar->pImei;
    pImeiLen = &opar->pImeiLen;
    nSimID = ipar->nSimID;
    CFW_GetIMEIFromAP(pImei, pImeiLen, nSimID);
}
#endif

#ifndef DISABLE_RPC_CFW_GetIMEISVFromAP
void RPC_CFW_GetIMEISVFromAP(void *in, void *out)
{
    struct IPAR_CFW_GetIMEISVFromAP *ipar = (struct IPAR_CFW_GetIMEISVFromAP *)in;
    struct OPAR_CFW_GetIMEISVFromAP *opar = (struct OPAR_CFW_GetIMEISVFromAP *)out;
    uint8_t *pImei;
    uint8_t *pImeiLen;
    CFW_SIM_ID nSimID;
    pImei = opar->pImei;
    pImeiLen = &opar->pImeiLen;
    nSimID = ipar->nSimID;
    CFW_GetIMEISVFromAP(pImei, pImeiLen, nSimID);
}
#endif

#ifndef DISABLE_RPC_CFW_GetBootCausesFromAP
void RPC_CFW_GetBootCausesFromAP(void *in, void *out)
{
    struct OPAR_CFW_GetBootCausesFromAP *opar = (struct OPAR_CFW_GetBootCausesFromAP *)out;

    opar->result = CFW_GetBootCausesFromAP();
}
#endif

#ifndef DISABLE_RPC_CFW_WifiSentestGetResult
void RPC_CFW_WifiSentestGetResult(void *in, void *out)
{
    struct IPAR_CFW_WifiSentestGetResult *ipar = (struct IPAR_CFW_WifiSentestGetResult *)in;
    struct OPAR_CFW_WifiSentestGetResult *opar = (struct OPAR_CFW_WifiSentestGetResult *)out;
    CFW_WIFISENTEST_RES wifi_res;
    wifi_res = ipar->wifi_res;
    opar->result = CFW_WifiSentestGetResult(wifi_res);
}
#endif

#ifndef DISABLE_RPC_CFW_MeDeleteMessageV2
void RPC_CFW_MeDeleteMessageV2(void *in, void *out)
{
    struct IPAR_CFW_MeDeleteMessageV2 *ipar = (struct IPAR_CFW_MeDeleteMessageV2 *)in;
    struct OPAR_CFW_MeDeleteMessageV2 *opar = (struct OPAR_CFW_MeDeleteMessageV2 *)out;
    CFW_SIM_ID nSimId;
    uint16_t nIndex;
    uint8_t nStatus;
    uint8_t *nType;
    uint32_t *pErrcode;
    uint32_t *HaveFreeSlot;
    nSimId = ipar->nSimId;
    nIndex = ipar->nIndex;
    nStatus = ipar->nStatus;
    nType = &opar->nType;
    pErrcode = &opar->pErrcode;
    HaveFreeSlot = &opar->HaveFreeSlot;
    opar->result = CFW_MeDeleteMessageV2(nSimId, nIndex, nStatus, nType, pErrcode, HaveFreeSlot);
}
#endif

#ifndef DISABLE_RPC_CFW_MeWriteMessageV2
void RPC_CFW_MeWriteMessageV2(void *in, void *out)
{
    struct IPAR_CFW_MeWriteMessageV2 *ipar = (struct IPAR_CFW_MeWriteMessageV2 *)in;
    struct OPAR_CFW_MeWriteMessageV2 *opar = (struct OPAR_CFW_MeWriteMessageV2 *)out;
    CFW_SIM_ID nSimId;
    uint16_t nIndex;
    uint8_t *pData;
    uint32_t nTime;
    uint32_t nAppInt32;
    uint8_t *nType;
    uint32_t *pErrcode;
    uint32_t *HaveFreeSlot;
    nSimId = ipar->nSimId;
    nIndex = ipar->nIndex;
    pData = ipar->pData;
    nTime = ipar->nTime;
    nAppInt32 = ipar->nAppInt32;
    nType = &opar->nType;
    pErrcode = &opar->pErrcode;
    HaveFreeSlot = &opar->HaveFreeSlot;
    opar->result = CFW_MeWriteMessageV2(nSimId, nIndex, pData, nTime, nAppInt32, nType, pErrcode, HaveFreeSlot);
}
#endif

#ifndef DISABLE_RPC_CFW_MeReadMessageV2
void RPC_CFW_MeReadMessageV2(void *in, void *out)
{
    struct IPAR_CFW_MeReadMessageV2 *ipar = (struct IPAR_CFW_MeReadMessageV2 *)in;
    struct OPAR_CFW_MeReadMessageV2 *opar = (struct OPAR_CFW_MeReadMessageV2 *)out;
    uint16_t nLocation;
    uint16_t nIndex;
    uint8_t *pData;
    uint8_t *nType;
    uint32_t *pErrcode;
    nLocation = ipar->nLocation;
    nIndex = ipar->nIndex;
    pData = opar->pData;
    nType = &opar->nType;
    pErrcode = &opar->pErrcode;
    opar->result = CFW_MeReadMessageV2(nLocation, nIndex, pData, nType, pErrcode);
}
#endif

#ifndef DISABLE_RPC_CFW_MeListMessageV2
void RPC_CFW_MeListMessageV2(void *in, void *out)
{
    struct IPAR_CFW_MeListMessageV2 *ipar = (struct IPAR_CFW_MeListMessageV2 *)in;
    struct OPAR_CFW_MeListMessageV2 *opar = (struct OPAR_CFW_MeListMessageV2 *)out;
    CFW_SIM_ID nSimId;
    uint8_t nStatus;
    uint16_t nCount;
    uint16_t nIndex;
    uint8_t *pData;
    uint8_t *nType;
    uint32_t *pErrcode;
    uint32_t *storageCount;
    nSimId = ipar->nSimId;
    nStatus = ipar->nStatus;
    nCount = ipar->nCount;
    nIndex = ipar->nIndex;
    pData = opar->pData;
    nType = &opar->nType;
    pErrcode = &opar->pErrcode;
    storageCount = &opar->storageCount;
    opar->result = CFW_MeListMessageV2(nSimId, nStatus, nCount, nIndex, pData, nType, pErrcode, storageCount);
}
#endif

#ifndef DISABLE_RPC_CFW_MeGetStorageInfoV2
void RPC_CFW_MeGetStorageInfoV2(void *in, void *out)
{
    struct IPAR_CFW_MeGetStorageInfoV2 *ipar = (struct IPAR_CFW_MeGetStorageInfoV2 *)in;
    struct OPAR_CFW_MeGetStorageInfoV2 *opar = (struct OPAR_CFW_MeGetStorageInfoV2 *)out;
    CFW_SIM_ID nSimId;
    uint8_t nStatus;
    uint8_t *nType;
    uint32_t *pErrcode;
    uint16_t *pStorageId;
    uint16_t *pTotalSlot;
    uint16_t *pUsedSlot;
    nSimId = ipar->nSimId;
    nStatus = ipar->nStatus;
    nType = &opar->nType;
    pErrcode = &opar->pErrcode;
    pStorageId = &opar->pStorageId;
    pTotalSlot = &opar->pTotalSlot;
    pUsedSlot = &opar->pUsedSlot;
    opar->result = CFW_MeGetStorageInfoV2(nSimId, nStatus, nType, pErrcode, pStorageId, pTotalSlot, pUsedSlot);
}
#endif

#ifndef DISABLE_RPC_SMS_DM_Init_V2
void RPC_SMS_DM_Init_V2(void *in, void *out)
{
    struct OPAR_SMS_DM_Init_V2 *opar = (struct OPAR_SMS_DM_Init_V2 *)out;

    opar->result = SMS_DM_Init_V2();
}
#endif

#ifndef DISABLE_RPC_CFW_SetSoftSimDefaultInfo
void RPC_CFW_SetSoftSimDefaultInfo(void *in, void *out)
{
    struct IPAR_CFW_SetSoftSimDefaultInfo *ipar = (struct IPAR_CFW_SetSoftSimDefaultInfo *)in;
    struct OPAR_CFW_SetSoftSimDefaultInfo *opar = (struct OPAR_CFW_SetSoftSimDefaultInfo *)out;
    uint8_t *nImsi;
    uint8_t *nKey;
    uint8_t *nPlmn;
    nImsi = ipar->nImsi;
    nKey = ipar->nKey;
    nPlmn = ipar->nPlmn;
    opar->result = CFW_SetSoftSimDefaultInfo(nImsi, nKey, nPlmn);
}
#endif

#ifndef DISABLE_RPC_CFW_GetDefaultApn
void RPC_CFW_GetDefaultApn(void *in, void *out)
{
    struct OPAR_CFW_GetDefaultApn *opar = (struct OPAR_CFW_GetDefaultApn *)out;
    OPER_DEFAULT_APN_INFO_V2 *nDefaultApn;
    nDefaultApn = opar->nDefaultApn;
    opar->result = CFW_GetDefaultApn(nDefaultApn);
}
#endif

#ifndef DISABLE_RPC_CFW_GetApnInfo
void RPC_CFW_GetApnInfo(void *in, void *out)
{
    struct IPAR_CFW_GetApnInfo *ipar = (struct IPAR_CFW_GetApnInfo *)in;
    struct OPAR_CFW_GetApnInfo *opar = (struct OPAR_CFW_GetApnInfo *)out;
    CFW_APNS_UNAME_UPWD *apn;
    uint8_t *plmn;
    apn = &opar->apn;
    plmn = ipar->plmn;
    opar->result = CFW_GetApnInfo(apn, plmn);
}
#endif

#ifndef DISABLE_RPC_CFW_GetSimLockStatusFromAP
void RPC_CFW_GetSimLockStatusFromAP(void *in, void *out)
{
    struct OPAR_CFW_GetSimLockStatusFromAP *opar = (struct OPAR_CFW_GetSimLockStatusFromAP *)out;
    CFW_SRV_SIMLOCKDATA_T *data;
    data = &opar->data;
    opar->result = CFW_GetSimLockStatusFromAP(data);
}
#endif

#ifndef DISABLE_RPC_CFW_GetDetectMBS
void RPC_CFW_GetDetectMBS(void *in, void *out)
{
    struct OPAR_CFW_GetDetectMBS *opar = (struct OPAR_CFW_GetDetectMBS *)out;
    uint8_t *pMode;
    pMode = &opar->pMode;
    opar->result = CFW_GetDetectMBS(pMode);
}
#endif

#ifndef DISABLE_RPC_CFW_GetDefaultLteCidCtoA
void RPC_CFW_GetDefaultLteCidCtoA(void *in, void *out)
{
    struct OPAR_CFW_GetDefaultLteCidCtoA *opar = (struct OPAR_CFW_GetDefaultLteCidCtoA *)out;

    opar->result = CFW_GetDefaultLteCidCtoA();
}
#endif

#ifndef DISABLE_RPC_sim_GetSimCardModeCB
void RPC_sim_GetSimCardModeCB(void *in, void *out)
{
    struct IPAR_sim_GetSimCardModeCB *ipar = (struct IPAR_sim_GetSimCardModeCB *)in;
    struct OPAR_sim_GetSimCardModeCB *opar = (struct OPAR_sim_GetSimCardModeCB *)out;
    uint8_t nSimID;
    nSimID = ipar->nSimID;
    opar->result = sim_GetSimCardModeCB(nSimID);
}
#endif

#ifndef DISABLE_RPC_sim_VsimResetCB
void RPC_sim_VsimResetCB(void *in, void *out)
{
    struct IPAR_sim_VsimResetCB *ipar = (struct IPAR_sim_VsimResetCB *)in;
    struct OPAR_sim_VsimResetCB *opar = (struct OPAR_sim_VsimResetCB *)out;
    uint8_t *ATRData;
    uint8_t *ATRSize;
    uint8_t nSimID;
    ATRData = opar->ATRData;
    ATRSize = &opar->ATRSize;
    opar->ATRSize = ipar->ATRSize;
    nSimID = ipar->nSimID;
    opar->result = sim_VsimResetCB(ATRData, ATRSize, nSimID);
}
#endif

#ifndef DISABLE_RPC_sim_VsimProcessApduCB
void RPC_sim_VsimProcessApduCB(void *in, void *out)
{
    struct IPAR_sim_VsimProcessApduCB *ipar = (struct IPAR_sim_VsimProcessApduCB *)in;
    struct OPAR_sim_VsimProcessApduCB *opar = (struct OPAR_sim_VsimProcessApduCB *)out;
    char *ivar_ptr = (char *)in + sizeof(struct IPAR_sim_VsimProcessApduCB);
    uint8_t* TxData;
    uint16_t TxSize;
    uint8_t *RxData;
    uint16_t *RxSize;
    uint8_t nSimID;
    TxSize = ipar->TxSize;
    RxData = opar->RxData;
    RxSize = opar->RxSize;
    memcpy(opar->RxSize, ipar->RxSize, sizeof(opar->RxSize));
    nSimID = ipar->nSimID;
    TxData = (uint8_t*)ivar_ptr;
    ivar_ptr += ALIGNUP8(TxSize + 1);
    opar->result = sim_VsimProcessApduCB(TxData, TxSize, RxData, RxSize, nSimID);
}
#endif

#ifndef DISABLE_RPC_CFW_WriteLongSmsInfo
void RPC_CFW_WriteLongSmsInfo(void *in, void *out)
{
    struct IPAR_CFW_WriteLongSmsInfo *ipar = (struct IPAR_CFW_WriteLongSmsInfo *)in;
    struct OPAR_CFW_WriteLongSmsInfo *opar = (struct OPAR_CFW_WriteLongSmsInfo *)out;
    uint8_t nStorage;
    uint8_t nStatus;
    uint32_t nParam1;
    uint32_t nParam2;
    uint8_t nSimID;
    nStorage = ipar->nStorage;
    nStatus = ipar->nStatus;
    nParam1 = ipar->nParam1;
    nParam2 = ipar->nParam2;
    nSimID = ipar->nSimID;
    opar->result = CFW_WriteLongSmsInfo(nStorage, nStatus, nParam1, nParam2, nSimID);
}
#endif

#ifndef DISABLE_RPC_CFW_NwGetLockBCCHCtoA
void RPC_CFW_NwGetLockBCCHCtoA(void *in, void *out)
{
    struct OPAR_CFW_NwGetLockBCCHCtoA *opar = (struct OPAR_CFW_NwGetLockBCCHCtoA *)out;

    opar->result = CFW_NwGetLockBCCHCtoA();
}
#endif

#ifndef DISABLE_RPC_CFW_NwIsUseISIM
void RPC_CFW_NwIsUseISIM(void *in, void *out)
{
    struct IPAR_CFW_NwIsUseISIM *ipar = (struct IPAR_CFW_NwIsUseISIM *)in;
    struct OPAR_CFW_NwIsUseISIM *opar = (struct OPAR_CFW_NwIsUseISIM *)out;
    char *plmnStr;
    plmnStr = ipar->plmnStr;
    opar->result = CFW_NwIsUseISIM(plmnStr);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_CC_SPEECH_CALL_IND
bool RECV_EV_CFW_CC_SPEECH_CALL_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_SPEECH_CALL_IND), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP
bool RECV_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE1(sizeof(CFW_TELNUMBER)), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NEW_SMS_IND
bool RECV_EV_CFW_NEW_SMS_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_NEW_SMS_NODE_EX)), CONCAT_SMS_INFO_SIZE);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NEW_EMS_IND
bool RECV_EV_CFW_NEW_EMS_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_NEW_SMS_NODE_EX)), CONCAT_SMS_INFO_SIZE);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SS_USSD_IND
bool RECV_EV_CFW_SS_USSD_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SS_USSD_IND_INFO_V2)), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_NETWORKINFO_IND
bool RECV_EV_CFW_NW_NETWORKINFO_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_NETWORK_INFO), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_SET_REGISTRATION_RSP
bool RECV_EV_CFW_NW_SET_REGISTRATION_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(6), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_GPRS_DATA_IND
bool RECV_EV_CFW_GPRS_DATA_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, 0, CFW_GPRS_DATA_SIZE);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_JAMMING_DETECT_IND
bool RECV_EV_CFW_NW_JAMMING_DETECT_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_JAMMING_DETECT_GET_IND), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_MBS_CALL_INFO_IND
bool RECV_EV_CFW_MBS_CALL_INFO_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_MBS_CELL_INFO), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_PBK_ENTRY_RSP
bool RECV_EV_CFW_SIM_GET_PBK_ENTRY_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16L8_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_LIST_PBK_ENTRY_RSP
bool RECV_EV_CFW_SIM_LIST_PBK_ENTRY_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, COUNT_PARAM2_IF_TYPE0(sizeof(CFW_SIM_PBK_ENTRY_INFO)+SIM_PBK_EXTR_SIZE), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_PBK_STRORAGE_RSP
bool RECV_EV_CFW_SIM_GET_PBK_STRORAGE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP
bool RECV_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2LO16_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_PROVIDER_ID_RSP
bool RECV_EV_CFW_SIM_GET_PROVIDER_ID_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_CHANGE_PWD_RSP
bool RECV_EV_CFW_SIM_CHANGE_PWD_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_READ_BINARY_RSP
bool RECV_EV_CFW_SIM_READ_BINARY_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP
bool RECV_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_FILE_STATUS_RSP
bool RECV_EV_CFW_SIM_GET_FILE_STATUS_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_READ_RECORD_RSP
bool RECV_EV_CFW_SIM_READ_RECORD_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_READ_MESSAGE_RSP
bool RECV_EV_CFW_SIM_READ_MESSAGE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SMS_PDU_LEN_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_LIST_MESSAGE_RSP
bool RECV_EV_CFW_SIM_LIST_MESSAGE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SMS_PDU_LEN_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SMS_READ_MESSAGE_RSP
bool RECV_EV_CFW_SMS_READ_MESSAGE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, EV_CFW_SMS_READ_MESSAGE_RSP_PTR1_SIZE, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_TPDU_COMMAND_RSP
bool RECV_EV_CFW_SIM_TPDU_COMMAND_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2LO16_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_DF_STATUS_RSP
bool RECV_EV_CFW_SIM_GET_DF_STATUS_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_CDMA_IMSI_RSP
bool RECV_EV_CFW_SIM_GET_CDMA_IMSI_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP
bool RECV_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, EV_CFW_NW_GET_AVAIL_OPERATOR_RSP_PTR1_SIZE, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_GET_QSCANF_RSP
bool RECV_EV_CFW_NW_GET_QSCANF_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, EV_CFW_NW_GET_QSCANF_RSP_PTR1_SIZE, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_GET_FREQSCAN_RSP
bool RECV_EV_CFW_NW_GET_FREQSCAN_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_GET_FREQSCAN_V2_RSP
bool RECV_EV_CFW_NW_GET_FREQSCAN_V2_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_GET_JAMMING_RSP
bool RECV_EV_CFW_NW_GET_JAMMING_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_JAMMING_DETECT_GET_IND), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_MEASURE_RESULT_RSP
bool RECV_EV_CFW_NW_MEASURE_RESULT_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_READ_VOICEMIAL_RSP
bool RECV_EV_CFW_SIM_READ_VOICEMIAL_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_READ_FILES_RSP
bool RECV_EV_CFW_SIM_READ_FILES_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_CB_PAGE_IND
bool RECV_EV_CFW_CB_PAGE_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP
bool RECV_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SIM_SMS_PARAMETERS)), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP
bool RECV_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SS_QUERY_CALLFORWARDING_RSP)), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SS_SET_CALL_FORWARDING_RSP
bool RECV_EV_CFW_SS_SET_CALL_FORWARDING_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SS_SET_CALLFORWARDING_INFO)), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SS_SEND_USSD_RSP
bool RECV_EV_CFW_SS_SEND_USSD_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SS_USSD_IND_INFO_V2)), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_EMC_NUM_LIST_IND
bool RECV_EV_CFW_EMC_NUM_LIST_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_NW_EMC_NUM_LIST_INFO)), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SAT_REFRESH_FILE_CHG_RSP
bool RECV_EV_CFW_SAT_REFRESH_FILE_CHG_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_SAT_REFRESH_FILE_CHG_CNF_T), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SAT_RESPONSE_RSP
bool RECV_EV_CFW_SAT_RESPONSE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, 0, SAT_RSP_RSP_IF_TYPE0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_EID_RSP
bool RECV_EV_CFW_SIM_GET_EID_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_SET_SUSPEND_RSP
bool RECV_EV_CFW_SIM_SET_SUSPEND_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_SIM_GET_ICCID_RSP
bool RECV_EV_CFW_SIM_GET_ICCID_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_JAMMING_RSSI_RSP
bool RECV_EV_CFW_NW_JAMMING_RSSI_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_JAMMING_RSSI_ENQUIRY_RSP), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_BLKLIST_GSM_PARAM_GET_IND
bool RECV_EV_CFW_BLKLIST_GSM_PARAM_GET_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_BLKLIST_GSM_INFO), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_NW_NET_DL_PSM_PARAM_RSP
bool RECV_EV_CFW_NW_NET_DL_PSM_PARAM_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NET_DL_PSM_PARAM_T), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_BLKLIST_LTE_PARAM_GET_IND
bool RECV_EV_CFW_BLKLIST_LTE_PARAM_GET_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_BLKLIST_LTE_INFO), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_ERRC_IDL_MEAS_OFFSET_READ_RSP
bool RECV_EV_CFW_ERRC_IDL_MEAS_OFFSET_READ_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_MEAS_OFFSET_T), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_ERRC_CONNSTATUS_V2_IND
bool RECV_EV_CFW_ERRC_CONNSTATUS_V2_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_CONN_STATUS_INFO), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_ENG_EVENT_RPT_IND
bool RECV_EV_CFW_ENG_EVENT_RPT_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_ENG_EVENT_RPT), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_ENG_STATIS_RPT_IND
bool RECV_EV_CFW_ENG_STATIS_RPT_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_ENG_STATIS_RPT), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_GSM_RACH_RESULT_IND
bool RECV_EV_CFW_GSM_RACH_RESULT_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_RACH_RESULT_IND), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_ENG_STATIS_RPT_EL1_IND
bool RECV_EV_CFW_ENG_STATIS_RPT_EL1_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_ENG_STATIS_RPT_EL1), 0);
}
#endif

#ifndef DISABLE_RPC_EV_CFW_ENG_EVENT_RPT_V2_IND
bool RECV_EV_CFW_ENG_EVENT_RPT_V2_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_ENG_EVENT_RPT_V2), 0);
}
#endif

#ifndef DISABLE_RPC_EV_DM_DAEMON_IND
bool RECV_EV_DM_DAEMON_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}
#endif

// AUTO GENERATED END
