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

#ifndef _CFW_RPC_C2A_PAR_H_
#define _CFW_RPC_C2A_PAR_H_

#define TAG_CFW_GetSSimFileInfoFromAP 0x1cf8a768
struct IPAR_CFW_GetSSimFileInfoFromAP
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSSimFileInfoFromAP));

struct OPAR_CFW_GetSSimFileInfoFromAP
{
    uint32_t result;
    CFW_SSIM_FILE_INFO_T ssim_info;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSSimFileInfoFromAP));

#define TAG_CFW_GetDftPdnInfoFromAP 0xfc57b97f
struct IPAR_CFW_GetDftPdnInfoFromAP
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetDftPdnInfoFromAP));

struct OPAR_CFW_GetDftPdnInfoFromAP
{
    uint32_t result;
    CFW_GPRS_DFTPDN_INFO nDftPdn;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetDftPdnInfoFromAP));

#define TAG_CFW_GetIMEIFromAP 0x62e05df9
struct IPAR_CFW_GetIMEIFromAP
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetIMEIFromAP));

struct OPAR_CFW_GetIMEIFromAP
{
    uint8_t pImei[16];
    uint8_t pImeiLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetIMEIFromAP));

#define TAG_CFW_GetIMEISVFromAP 0xab8a16e3
struct IPAR_CFW_GetIMEISVFromAP
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetIMEISVFromAP));

struct OPAR_CFW_GetIMEISVFromAP
{
    uint8_t pImei[16];
    uint8_t pImeiLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetIMEISVFromAP));

#define TAG_CFW_GetBootCausesFromAP 0x9e161177
struct OPAR_CFW_GetBootCausesFromAP
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetBootCausesFromAP));

#define TAG_CFW_WifiSentestGetResult 0xf6a88248
struct IPAR_CFW_WifiSentestGetResult
{
    CFW_WIFISENTEST_RES wifi_res;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_WifiSentestGetResult));

struct OPAR_CFW_WifiSentestGetResult
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_WifiSentestGetResult));

#define TAG_CFW_MeDeleteMessageV2 0x438ef180
struct IPAR_CFW_MeDeleteMessageV2
{
    CFW_SIM_ID nSimId;
    uint16_t nIndex;
    uint8_t nStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_MeDeleteMessageV2));

struct OPAR_CFW_MeDeleteMessageV2
{
    uint32_t result;
    uint8_t nType;
    uint32_t pErrcode;
    uint32_t HaveFreeSlot;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_MeDeleteMessageV2));

#define TAG_CFW_MeWriteMessageV2 0x20849a1a
struct IPAR_CFW_MeWriteMessageV2
{
    CFW_SIM_ID nSimId;
    uint16_t nIndex;
    uint8_t pData[184];
    uint32_t nTime;
    uint32_t nAppInt32;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_MeWriteMessageV2));

struct OPAR_CFW_MeWriteMessageV2
{
    uint32_t result;
    uint8_t nType;
    uint32_t pErrcode;
    uint32_t HaveFreeSlot;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_MeWriteMessageV2));

#define TAG_CFW_MeReadMessageV2 0xbb8bcea8
struct IPAR_CFW_MeReadMessageV2
{
    uint16_t nLocation;
    uint16_t nIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_MeReadMessageV2));

struct OPAR_CFW_MeReadMessageV2
{
    uint32_t result;
    uint8_t pData[184];
    uint8_t nType;
    uint32_t pErrcode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_MeReadMessageV2));

#define TAG_CFW_MeListMessageV2 0x814d4f02
struct IPAR_CFW_MeListMessageV2
{
    CFW_SIM_ID nSimId;
    uint8_t nStatus;
    uint16_t nCount;
    uint16_t nIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_MeListMessageV2));

struct OPAR_CFW_MeListMessageV2
{
    uint32_t result;
    uint8_t pData[184];
    uint8_t nType;
    uint32_t pErrcode;
    uint32_t storageCount;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_MeListMessageV2));

#define TAG_CFW_MeGetStorageInfoV2 0x65a43caf
struct IPAR_CFW_MeGetStorageInfoV2
{
    CFW_SIM_ID nSimId;
    uint8_t nStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_MeGetStorageInfoV2));

struct OPAR_CFW_MeGetStorageInfoV2
{
    uint32_t result;
    uint8_t nType;
    uint32_t pErrcode;
    uint16_t pStorageId;
    uint16_t pTotalSlot;
    uint16_t pUsedSlot;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_MeGetStorageInfoV2));

#define TAG_SMS_DM_Init_V2 0x9a20ffcf
struct OPAR_SMS_DM_Init_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_SMS_DM_Init_V2));

#define TAG_CFW_SetSoftSimDefaultInfo 0x23a48243
struct IPAR_CFW_SetSoftSimDefaultInfo
{
    uint8_t nImsi[16];
    uint8_t nKey[33];
    uint8_t nPlmn[7];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSoftSimDefaultInfo));

struct OPAR_CFW_SetSoftSimDefaultInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetSoftSimDefaultInfo));

#define TAG_CFW_GetDefaultApn 0xda5560c6
struct OPAR_CFW_GetDefaultApn
{
    uint32_t result;
    OPER_DEFAULT_APN_INFO_V2 nDefaultApn[AT_DEFAULT_APN_MAX_NUM];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetDefaultApn));

#define TAG_CFW_GetApnInfo 0x32ba1bb3
struct IPAR_CFW_GetApnInfo
{
    uint8_t plmn[7];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetApnInfo));

struct OPAR_CFW_GetApnInfo
{
    uint32_t result;
    CFW_APNS_UNAME_UPWD apn;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetApnInfo));

#define TAG_CFW_GetSimLockStatusFromAP 0xde45ef05
struct OPAR_CFW_GetSimLockStatusFromAP
{
    uint32_t result;
    CFW_SRV_SIMLOCKDATA_T data;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimLockStatusFromAP));

#define TAG_CFW_GetDetectMBS 0xb691ecb2
struct OPAR_CFW_GetDetectMBS
{
    uint32_t result;
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetDetectMBS));

#define TAG_CFW_GetDefaultLteCidCtoA 0x73456edd
struct OPAR_CFW_GetDefaultLteCidCtoA
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetDefaultLteCidCtoA));

#define TAG_sim_GetSimCardModeCB 0x07749b4d
struct IPAR_sim_GetSimCardModeCB
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_sim_GetSimCardModeCB));

struct OPAR_sim_GetSimCardModeCB
{
    uint16_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_sim_GetSimCardModeCB));

#define TAG_sim_VsimResetCB 0x414a3352
struct IPAR_sim_VsimResetCB
{
    uint8_t ATRSize;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_sim_VsimResetCB));

struct OPAR_sim_VsimResetCB
{
    uint16_t result;
    uint8_t ATRData[100];
    uint8_t ATRSize;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_sim_VsimResetCB));

#define TAG_sim_VsimProcessApduCB 0x03e96fac
struct IPAR_sim_VsimProcessApduCB
{
    uint16_t TxSize;
    uint16_t RxSize[1];
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_sim_VsimProcessApduCB));

struct OPAR_sim_VsimProcessApduCB
{
    uint16_t result;
    uint8_t RxData[280];
    uint16_t RxSize[1];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_sim_VsimProcessApduCB));

#define TAG_CFW_WriteLongSmsInfo 0xd2f01f11
struct IPAR_CFW_WriteLongSmsInfo
{
    uint8_t nStorage;
    uint8_t nStatus;
    uint32_t nParam1;
    uint32_t nParam2;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_WriteLongSmsInfo));

struct OPAR_CFW_WriteLongSmsInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_WriteLongSmsInfo));

#define TAG_CFW_NwGetLockBCCHCtoA 0x1eb02fe9
struct OPAR_CFW_NwGetLockBCCHCtoA
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetLockBCCHCtoA));

#define TAG_CFW_NwIsUseISIM 0xe0a7fc13
struct IPAR_CFW_NwIsUseISIM
{
    char plmnStr[7];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwIsUseISIM));

struct OPAR_CFW_NwIsUseISIM
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwIsUseISIM));


#endif // _CFW_RPC_C2A_PAR_H_
// AUTO GENERATED END
