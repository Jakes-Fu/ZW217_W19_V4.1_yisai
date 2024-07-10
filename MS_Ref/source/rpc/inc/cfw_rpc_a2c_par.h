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

#ifndef _CFW_RPC_A2C_PAR_H_
#define _CFW_RPC_A2C_PAR_H_

#define TAG_CFW_IsFreeUTI 0xac2b1b66
struct IPAR_CFW_IsFreeUTI
{
    uint16_t nUTI;
    uint32_t nServiceId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_IsFreeUTI));

struct OPAR_CFW_IsFreeUTI
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_IsFreeUTI));

#define TAG_CFW_GetFreeUTI 0x8a0c270b
struct IPAR_CFW_GetFreeUTI
{
    uint32_t nServiceId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetFreeUTI));

struct OPAR_CFW_GetFreeUTI
{
    uint32_t result;
    uint8_t nUTI;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetFreeUTI));

#define TAG_CFW_ShellControl 0xe4a00223
struct IPAR_CFW_ShellControl
{
    uint8_t nCommand;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ShellControl));

struct OPAR_CFW_ShellControl
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ShellControl));

#define TAG_CFW_ShellControl_V2 0x5d0ce3b9
struct IPAR_CFW_ShellControl_V2
{
    uint16_t nUTI;
    uint8_t nCommand;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ShellControl_V2));

struct OPAR_CFW_ShellControl_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ShellControl_V2));

#define TAG_CFW_CfgSetPhoneFunctionality 0xb4319259
struct IPAR_CFW_CfgSetPhoneFunctionality
{
    uint8_t nLevel;
    uint8_t nRst;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPhoneFunctionality));

struct OPAR_CFW_CfgSetPhoneFunctionality
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPhoneFunctionality));

#define TAG_CFW_CfgGetPhoneFunctionality 0xa046cd5e
struct OPAR_CFW_CfgGetPhoneFunctionality
{
    uint32_t result;
    uint8_t nLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPhoneFunctionality));

#define TAG_CFW_CfgStoreUserProfile 0xa40eca33
struct OPAR_CFW_CfgStoreUserProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgStoreUserProfile));

#define TAG_CFW_CfgGetUserProfile 0xa53c084f
struct OPAR_CFW_CfgGetUserProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetUserProfile));

#define TAG_CFW_CfgRestoreFactoryProfile 0x074a8a28
struct OPAR_CFW_CfgRestoreFactoryProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgRestoreFactoryProfile));

#define TAG_CFW_CfgGetCharacterSet 0x69bee605
struct OPAR_CFW_CfgGetCharacterSet
{
    uint32_t result;
    uint8_t chset;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetCharacterSet));

#define TAG_CFW_CfgSetCharacterSet 0x991e05de
struct IPAR_CFW_CfgSetCharacterSet
{
    uint8_t chset;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetCharacterSet));

struct OPAR_CFW_CfgSetCharacterSet
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetCharacterSet));

#define TAG_CFW_CfgSetIndicatorEvent 0xcbe35db9
struct IPAR_CFW_CfgSetIndicatorEvent
{
    uint16_t ind;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetIndicatorEvent));

struct OPAR_CFW_CfgSetIndicatorEvent
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetIndicatorEvent));

#define TAG_CFW_CfgGetIndicatorEvent 0x103eecc1
struct IPAR_CFW_CfgGetIndicatorEvent
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetIndicatorEvent));

struct OPAR_CFW_CfgGetIndicatorEvent
{
    uint32_t result;
    uint16_t ind;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetIndicatorEvent));

#define TAG_CFW_SetQualReport 0xd7c9260b
struct IPAR_CFW_SetQualReport
{
    bool bStart;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetQualReport));

struct OPAR_CFW_SetQualReport
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetQualReport));

#define TAG_CFW_CfgSetFirstPowerOnFlag 0x424b0ac3
struct IPAR_CFW_CfgSetFirstPowerOnFlag
{
    bool bFPO;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetFirstPowerOnFlag));

struct OPAR_CFW_CfgSetFirstPowerOnFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetFirstPowerOnFlag));

#define TAG_CFW_CfgCheckFirstPowerOnFlag 0xdfab74a6
struct OPAR_CFW_CfgCheckFirstPowerOnFlag
{
    uint32_t result;
    bool bFPO;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgCheckFirstPowerOnFlag));

#define TAG_CFW_CfgGetEventReport 0xfcada50f
struct OPAR_CFW_CfgGetEventReport
{
    uint32_t result;
    uint16_t pMer;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetEventReport));

#define TAG_CFW_CfgSetEventReport 0xf37b9eb1
struct IPAR_CFW_CfgSetEventReport
{
    uint16_t pMer;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetEventReport));

struct OPAR_CFW_CfgSetEventReport
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetEventReport));

#define TAG_CFW_CfgSetResultCodePresentMode 0x9b269a8d
struct IPAR_CFW_CfgSetResultCodePresentMode
{
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetResultCodePresentMode));

struct OPAR_CFW_CfgSetResultCodePresentMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetResultCodePresentMode));

#define TAG_CFW_CfgGetResultCodePresentMode 0x6fc3900c
struct OPAR_CFW_CfgGetResultCodePresentMode
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetResultCodePresentMode));

#define TAG_CFW_CfgSetResultCodeFormatMode 0xa7d4e623
struct IPAR_CFW_CfgSetResultCodeFormatMode
{
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetResultCodeFormatMode));

struct OPAR_CFW_CfgSetResultCodeFormatMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetResultCodeFormatMode));

#define TAG_CFW_CfgGetResultCodeFormatMode 0xc0bd302c
struct OPAR_CFW_CfgGetResultCodeFormatMode
{
    uint32_t result;
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetResultCodeFormatMode));

#define TAG_CFW_CfgSetEchoMode 0xd5b14f3b
struct IPAR_CFW_CfgSetEchoMode
{
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetEchoMode));

struct OPAR_CFW_CfgSetEchoMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetEchoMode));

#define TAG_CFW_CfgGetEchoMode 0x7a5ee337
struct OPAR_CFW_CfgGetEchoMode
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetEchoMode));

#define TAG_CFW_CfgGetExtendedErrorV1 0x72ad454c
struct OPAR_CFW_CfgGetExtendedErrorV1
{
    uint32_t result;
    CFW_EXTENDED_ERROR pExtendedError;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetExtendedErrorV1));

#define TAG_CFW_CfgGetErrorReport 0xcbc0852d
struct OPAR_CFW_CfgGetErrorReport
{
    uint32_t result;
    uint8_t pLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetErrorReport));

#define TAG_CFW_CfgSetErrorReport 0x0f7a5643
struct IPAR_CFW_CfgSetErrorReport
{
    uint8_t pLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetErrorReport));

struct OPAR_CFW_CfgSetErrorReport
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetErrorReport));

#define TAG_CFW_CfgDeviceSwithOff 0x71ed90b2
struct OPAR_CFW_CfgDeviceSwithOff
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgDeviceSwithOff));

#define TAG_CFW_CfgSetConnectResultFormatCode 0xaa3996b2
struct IPAR_CFW_CfgSetConnectResultFormatCode
{
    uint8_t nValue;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetConnectResultFormatCode));

struct OPAR_CFW_CfgSetConnectResultFormatCode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetConnectResultFormatCode));

#define TAG_CFW_CfgGetConnectResultFormatCode 0x92ddeb2c
struct OPAR_CFW_CfgGetConnectResultFormatCode
{
    uint32_t result;
    uint8_t nValue;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetConnectResultFormatCode));

#define TAG_CFW_CfgSetSoftReset 0xb02b949a
struct IPAR_CFW_CfgSetSoftReset
{
    uint8_t cause;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSoftReset));

struct OPAR_CFW_CfgSetSoftReset
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSoftReset));

#define TAG_CFW_CfgGetSoftReset 0xc122799c
struct OPAR_CFW_CfgGetSoftReset
{
    uint32_t result;
    uint8_t cause;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSoftReset));

#define TAG_CFW_CfgSetSmsStorage 0x3f3995c2
struct IPAR_CFW_CfgSetSmsStorage
{
    uint8_t nStorage;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsStorage));

struct OPAR_CFW_CfgSetSmsStorage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsStorage));

#define TAG_CFW_CfgGetSmsStorage 0xa0dae657
struct IPAR_CFW_CfgGetSmsStorage
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsStorage));

struct OPAR_CFW_CfgGetSmsStorage
{
    uint32_t result;
    uint8_t nStorage;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsStorage));

#define TAG_CFW_CfgSetNewSmsOption 0x592995fb
struct IPAR_CFW_CfgSetNewSmsOption
{
    uint8_t nOption;
    uint8_t nNewSmsStorage;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetNewSmsOption));

struct OPAR_CFW_CfgSetNewSmsOption
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetNewSmsOption));

#define TAG_CFW_CfgGetNewSmsOption 0x1ea60113
struct IPAR_CFW_CfgGetNewSmsOption
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetNewSmsOption));

struct OPAR_CFW_CfgGetNewSmsOption
{
    uint32_t result;
    uint8_t nOption;
    uint8_t nNewSmsStorage;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetNewSmsOption));

#define TAG_CFW_CfgSetSmsOverflowInd 0xb4b7a4da
struct IPAR_CFW_CfgSetSmsOverflowInd
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsOverflowInd));

struct OPAR_CFW_CfgSetSmsOverflowInd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsOverflowInd));

#define TAG_CFW_CfgGetSmsOverflowInd 0x7fcfef26
struct IPAR_CFW_CfgGetSmsOverflowInd
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsOverflowInd));

struct OPAR_CFW_CfgGetSmsOverflowInd
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsOverflowInd));

#define TAG_CFW_CfgSetSmsFormat 0xa9f316f2
struct IPAR_CFW_CfgSetSmsFormat
{
    uint8_t nFormat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsFormat));

struct OPAR_CFW_CfgSetSmsFormat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsFormat));

#define TAG_CFW_CfgGetSmsFormat 0x203e9990
struct IPAR_CFW_CfgGetSmsFormat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsFormat));

struct OPAR_CFW_CfgGetSmsFormat
{
    uint32_t result;
    uint8_t nFormat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsFormat));

#define TAG_CFW_CfgSetSmsShowTextModeParam 0x1503ad06
struct IPAR_CFW_CfgSetSmsShowTextModeParam
{
    uint8_t nShow;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsShowTextModeParam));

struct OPAR_CFW_CfgSetSmsShowTextModeParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsShowTextModeParam));

#define TAG_CFW_CfgGetSmsShowTextModeParam 0xcaec4d3b
struct IPAR_CFW_CfgGetSmsShowTextModeParam
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsShowTextModeParam));

struct OPAR_CFW_CfgGetSmsShowTextModeParam
{
    uint32_t result;
    uint8_t nShow;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsShowTextModeParam));

#define TAG_CFW_CfgSetSmsParam 0xd480bf02
struct IPAR_CFW_CfgSetSmsParam
{
    CFW_SMS_PARAMETER pInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsParam));

struct OPAR_CFW_CfgSetSmsParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsParam));

#define TAG_CFW_CfgGetSmsParam 0xf2b23c71
struct IPAR_CFW_CfgGetSmsParam
{
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsParam));

struct OPAR_CFW_CfgGetSmsParam
{
    uint32_t result;
    CFW_SMS_PARAMETER pInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsParam));

#define TAG_CFW_CfgSetDefaultSmsParam 0xe6b2eaa7
struct IPAR_CFW_CfgSetDefaultSmsParam
{
    CFW_SMS_PARAMETER pInfo;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetDefaultSmsParam));

struct OPAR_CFW_CfgSetDefaultSmsParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetDefaultSmsParam));

#define TAG_CFW_CfgGetDefaultSmsParam 0xbe60af25
struct IPAR_CFW_CfgGetDefaultSmsParam
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetDefaultSmsParam));

struct OPAR_CFW_CfgGetDefaultSmsParam
{
    uint32_t result;
    CFW_SMS_PARAMETER pInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetDefaultSmsParam));

#define TAG_CFW_CfgSetSmsStorageInfo 0x7e01cec5
struct IPAR_CFW_CfgSetSmsStorageInfo
{
    CFW_SMS_STORAGE_INFO pStorageInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsStorageInfo));

struct OPAR_CFW_CfgSetSmsStorageInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsStorageInfo));

#define TAG_CFW_CfgGetSmsStorageInfo 0x2e2a7c57
struct IPAR_CFW_CfgGetSmsStorageInfo
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsStorageInfo));

struct OPAR_CFW_CfgGetSmsStorageInfo
{
    uint32_t result;
    CFW_SMS_STORAGE_INFO pStorageInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsStorageInfo));

#define TAG_CFW_SimGetSmsTotalNum 0x8c9061a8
struct IPAR_CFW_SimGetSmsTotalNum
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetSmsTotalNum));

struct OPAR_CFW_SimGetSmsTotalNum
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetSmsTotalNum));

#define TAG_CFW_CfgSelectSmsService 0xe1d549f5
struct IPAR_CFW_CfgSelectSmsService
{
    uint8_t nService;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSelectSmsService));

struct OPAR_CFW_CfgSelectSmsService
{
    uint32_t result;
    uint8_t pSupportedType;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSelectSmsService));

#define TAG_CFW_CfgQuerySmsService 0x398b2a95
struct OPAR_CFW_CfgQuerySmsService
{
    uint32_t result;
    uint8_t nService;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgQuerySmsService));

#define TAG_CFW_CfgSetSmsCB 0xc3a3200e
struct IPAR_CFW_CfgSetSmsCB
{
    uint8_t mode;
    uint8_t mids[6];
    uint8_t dcss[6];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsCB));

struct OPAR_CFW_CfgSetSmsCB
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsCB));

#define TAG_CFW_CfgGetSmsCB 0xc74fce6d
struct OPAR_CFW_CfgGetSmsCB
{
    uint32_t result;
    uint8_t pMode;
    uint8_t mids[6];
    uint8_t dcss[6];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsCB));

#define TAG_CFW_GetSimPHYType 0xddaae762
struct IPAR_CFW_GetSimPHYType
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimPHYType));

struct OPAR_CFW_GetSimPHYType
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimPHYType));

#define TAG_CFW_SetSimStatus 0x610691f7
struct IPAR_CFW_SetSimStatus
{
    CFW_SIM_ID nSimID;
    CFW_SIM_STATUS nSimStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSimStatus));

#define TAG_CFW_GetSimStatus 0xeeef512f
struct IPAR_CFW_GetSimStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimStatus));

struct OPAR_CFW_GetSimStatus
{
    CFW_SIM_STATUS result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimStatus));

#define TAG_CFW_GetSimType 0xadfbc835
struct IPAR_CFW_GetSimType
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimType));

struct OPAR_CFW_GetSimType
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimType));

#define TAG_CFW_ImsSsUtAddrSet 0x8d3ed232
struct IPAR_CFW_ImsSsUtAddrSet
{
    uint8_t nCid;
    uint8_t nSize;
    uint8_t * paddr;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSsUtAddrSet));

struct OPAR_CFW_ImsSsUtAddrSet
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSsUtAddrSet));

#define TAG_CFW_SimClose 0x2fd3f031
struct IPAR_CFW_SimClose
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimClose));

struct OPAR_CFW_SimClose
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimClose));

#define TAG_CFW_GetStackSimFileID 0xb0ff5bb2
struct IPAR_CFW_GetStackSimFileID
{
    uint16_t n3GppFileID;
    uint16_t EFPath;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetStackSimFileID));

struct OPAR_CFW_GetStackSimFileID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetStackSimFileID));

#define TAG_CFW_GetStackSimFileIDByPath_V2 0x39fc26f4
struct IPAR_CFW_GetStackSimFileIDByPath_V2
{
    uint16_t n3GppFileID;
    uint8_t iEfPathLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetStackSimFileIDByPath_V2));

struct OPAR_CFW_GetStackSimFileIDByPath_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetStackSimFileIDByPath_V2));

#define TAG_CFW_UsimDecodeEFFcp 0xa97b3bc7
struct IPAR_CFW_UsimDecodeEFFcp
{
    uint8_t RespData[256];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_UsimDecodeEFFcp));

struct OPAR_CFW_UsimDecodeEFFcp
{
    CFW_UsimEfStatus pSimEfStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_UsimDecodeEFFcp));

#define TAG_CFW_UsimDecodeEFFcpV2 0x7c191a7f
struct IPAR_CFW_UsimDecodeEFFcpV2
{
    uint16_t iLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_UsimDecodeEFFcpV2));

struct OPAR_CFW_UsimDecodeEFFcpV2
{
    uint32_t result;
    CFW_UsimEfStatus pSimEfStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_UsimDecodeEFFcpV2));

#define TAG_CFW_SimReadRecordWithLen 0x7754395f
struct IPAR_CFW_SimReadRecordWithLen
{
    uint8_t nFileID;
    uint8_t nRecordNum;
    uint16_t nLen;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadRecordWithLen));

struct OPAR_CFW_SimReadRecordWithLen
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadRecordWithLen));

#define TAG_CFW_SimReadRecordAllParam 0x15b5d059
struct IPAR_CFW_SimReadRecordAllParam
{
    uint8_t nFileID;
    uint8_t nRecordNum;
    uint8_t nRecordMode;
    uint16_t nLen;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadRecordAllParam));

struct OPAR_CFW_SimReadRecordAllParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadRecordAllParam));

#define TAG_CFW_SimGetDFStatus 0x9fa5b5f2
struct IPAR_CFW_SimGetDFStatus
{
    uint8_t nFile;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetDFStatus));

struct OPAR_CFW_SimGetDFStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetDFStatus));

#define TAG_CFW_SimReadBinary 0x6482e52d
struct IPAR_CFW_SimReadBinary
{
    uint8_t nFileId;
    uint8_t nOffset;
    uint8_t nBytesToRead;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadBinary));

struct OPAR_CFW_SimReadBinary
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadBinary));

#define TAG_CFW_SimReadElementary 0x15b30ba0
struct IPAR_CFW_SimReadElementary
{
    uint8_t nFileId;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadElementary));

struct OPAR_CFW_SimReadElementary
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadElementary));

#define TAG_CFW_SimGetOperationTimes 0xe6561ada
struct OPAR_CFW_SimGetOperationTimes
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetOperationTimes));

#define TAG_CFW_SimUpdateBinary 0xeb4d03f5
struct IPAR_CFW_SimUpdateBinary
{
    uint8_t nFileId;
    uint8_t nOffset;
    uint8_t nBytesToWrite;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimUpdateBinary));

struct OPAR_CFW_SimUpdateBinary
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimUpdateBinary));

#define TAG_CFW_SimReadRecord 0xd1edf62e
struct IPAR_CFW_SimReadRecord
{
    uint8_t nFileID;
    uint8_t nRecordNum;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadRecord));

struct OPAR_CFW_SimReadRecord
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadRecord));

#define TAG_CFW_SimUpdateRecord 0x080cd798
struct IPAR_CFW_SimUpdateRecord
{
    uint8_t nFileID;
    uint8_t nRecordNum;
    uint8_t nRecordSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimUpdateRecord));

struct OPAR_CFW_SimUpdateRecord
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimUpdateRecord));

#define TAG_CFW_SimUpdateRecordAllParam 0x9c96a6bb
struct IPAR_CFW_SimUpdateRecordAllParam
{
    uint8_t nFileID;
    uint8_t nRecordNum;
    uint8_t nRecordSize;
    uint8_t nRecordMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimUpdateRecordAllParam));

struct OPAR_CFW_SimUpdateRecordAllParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimUpdateRecordAllParam));

#define TAG_CFW_SimGetFileStatus 0x0c820824
struct IPAR_CFW_SimGetFileStatus
{
    uint8_t nFileID;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetFileStatus));

struct OPAR_CFW_SimGetFileStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetFileStatus));

#define TAG_CFW_SimSetProiorityResetFlag 0xa7b92ac0
struct IPAR_CFW_SimSetProiorityResetFlag
{
    uint8_t nResetFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetProiorityResetFlag));

#define TAG_CFW_SmsMoInit 0xe5fdf9e0
struct IPAR_CFW_SmsMoInit
{
    uint16_t nUti;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMoInit));

struct OPAR_CFW_SmsMoInit
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMoInit));

#define TAG_CFW_SimInit 0x0a272beb
struct IPAR_CFW_SimInit
{
    bool bRstSim;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimInit));

struct OPAR_CFW_SimInit
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimInit));

#define TAG_CFW_CfgSetSimSwitch 0x14e06e43
struct IPAR_CFW_CfgSetSimSwitch
{
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSimSwitch));

struct OPAR_CFW_CfgSetSimSwitch
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSimSwitch));

#define TAG_CFW_CfgGetSimSwitch 0xb9697fc4
struct OPAR_CFW_CfgGetSimSwitch
{
    uint32_t result;
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSimSwitch));

#define TAG_CFW_SwitchDualSimSlot 0x7e035c81
struct IPAR_CFW_SwitchDualSimSlot
{
    uint8_t nSimSlot;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SwitchDualSimSlot));

struct OPAR_CFW_SwitchDualSimSlot
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SwitchDualSimSlot));

#define TAG_SimSwitch 0x665f6ca9
struct IPAR_SimSwitch
{
    uint8_t SimNum;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_SimSwitch));

#define TAG_CFW_CfgSetPbkStrorageInfo 0x73af8928
struct IPAR_CFW_CfgSetPbkStrorageInfo
{
    CFW_PBK_STRORAGE_INFO pStorageInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPbkStrorageInfo));

struct OPAR_CFW_CfgSetPbkStrorageInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPbkStrorageInfo));

#define TAG_CFW_CfgGetPbkStrorageInfo 0xeee2d88b
struct OPAR_CFW_CfgGetPbkStrorageInfo
{
    uint32_t result;
    CFW_PBK_STRORAGE_INFO pStorageInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPbkStrorageInfo));

#define TAG_CFW_CfgSetPbkStorage 0x2a2c1f97
struct IPAR_CFW_CfgSetPbkStorage
{
    uint8_t nStorage;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPbkStorage));

struct OPAR_CFW_CfgSetPbkStorage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPbkStorage));

#define TAG_CFW_CfgGetPbkStorage 0xcd436267
struct IPAR_CFW_CfgGetPbkStorage
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetPbkStorage));

struct OPAR_CFW_CfgGetPbkStorage
{
    uint32_t result;
    uint8_t nStorage;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPbkStorage));

#define TAG_CFW_CfgSetStoredPlmnList 0x3ce9c17e
struct IPAR_CFW_CfgSetStoredPlmnList
{
    CFW_StoredPlmnList pPlmnL;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetStoredPlmnList));

struct OPAR_CFW_CfgSetStoredPlmnList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetStoredPlmnList));

#define TAG_CFW_CfgGetStoredPlmnList 0xe96c6a34
struct IPAR_CFW_CfgGetStoredPlmnList
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetStoredPlmnList));

struct OPAR_CFW_CfgGetStoredPlmnList
{
    uint32_t result;
    CFW_StoredPlmnList pPlmnL;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetStoredPlmnList));

#define TAG_CFW_CfgNwSetLockedFrequencyBand 0x7b6bbded
struct IPAR_CFW_CfgNwSetLockedFrequencyBand
{
    uint8_t nBand;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwSetLockedFrequencyBand));

struct OPAR_CFW_CfgNwSetLockedFrequencyBand
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwSetLockedFrequencyBand));

#define TAG_CFW_CfgNwGetLockedFrequencyBand 0x64c27811
struct IPAR_CFW_CfgNwGetLockedFrequencyBand
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetLockedFrequencyBand));

struct OPAR_CFW_CfgNwGetLockedFrequencyBand
{
    uint32_t result;
    uint8_t pBand;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetLockedFrequencyBand));

#define TAG_CFW_CfgNwSetFrequencyBand 0xd2cedcbb
struct IPAR_CFW_CfgNwSetFrequencyBand
{
    uint8_t nBand;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwSetFrequencyBand));

struct OPAR_CFW_CfgNwSetFrequencyBand
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwSetFrequencyBand));

#define TAG_CFW_CfgNwGetFrequencyBand 0xa3e4654b
struct IPAR_CFW_CfgNwGetFrequencyBand
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetFrequencyBand));

struct OPAR_CFW_CfgNwGetFrequencyBand
{
    uint32_t result;
    uint8_t nBand;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetFrequencyBand));

#define TAG_CFW_CfgNwSetNetWorkMode 0xc40a65b9
struct IPAR_CFW_CfgNwSetNetWorkMode
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwSetNetWorkMode));

struct OPAR_CFW_CfgNwSetNetWorkMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwSetNetWorkMode));

#define TAG_CFW_CfgNwGetNetWorkMode 0x23c4e97f
struct IPAR_CFW_CfgNwGetNetWorkMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetNetWorkMode));

struct OPAR_CFW_CfgNwGetNetWorkMode
{
    uint32_t result;
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetNetWorkMode));

#define TAG_CFW_CfgSetNwStatus 0x9dccde75
struct IPAR_CFW_CfgSetNwStatus
{
    uint8_t n;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetNwStatus));

struct OPAR_CFW_CfgSetNwStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetNwStatus));

#define TAG_CFW_CfgGetNwStatus 0xabfd1d2f
struct IPAR_CFW_CfgGetNwStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetNwStatus));

struct OPAR_CFW_CfgGetNwStatus
{
    uint32_t result;
    uint8_t pCmd;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetNwStatus));

#define TAG_CFW_CfgNwSetCgclassType 0x6f2b39a6
struct IPAR_CFW_CfgNwSetCgclassType
{
    uint8_t nCgclassType;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwSetCgclassType));

struct OPAR_CFW_CfgNwSetCgclassType
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwSetCgclassType));

#define TAG_CFW_CfgNwGetCgclassType 0x6f446fb3
struct IPAR_CFW_CfgNwGetCgclassType
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetCgclassType));

struct OPAR_CFW_CfgNwGetCgclassType
{
    uint32_t result;
    uint8_t nCgclassType;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetCgclassType));

#define TAG_CFW_CfgNwSetFM 0x58f04fb2
struct IPAR_CFW_CfgNwSetFM
{
    CFW_COMM_MODE bMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwSetFM));

struct OPAR_CFW_CfgNwSetFM
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwSetFM));

#define TAG_CFW_CfgNwGetOperatorInfo 0x0d3fa799
struct IPAR_CFW_CfgNwGetOperatorInfo
{
    uint32_t nIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetOperatorInfo));

struct OPAR_CFW_CfgNwGetOperatorInfo
{
    uint32_t result;
    uint8_t * pOperatorId;
    uint8_t * pOperatorName;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetOperatorInfo));

#define TAG_CFW_CfgNwGetOperatorId 0xf6dd0cb5
struct IPAR_CFW_CfgNwGetOperatorId
{
    uint8_t format;
    uint32_t pOperatorNameSize;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetOperatorId));

struct OPAR_CFW_CfgNwGetOperatorId
{
    uint32_t result;
    uint8_t * pOperatorId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetOperatorId));

#define TAG_CFW_CfgNwGetOperatorIdWithIndex 0x98d26da8
struct IPAR_CFW_CfgNwGetOperatorIdWithIndex
{
    uint32_t pOperatorNameSize;
    uint8_t format;
    uint32_t nIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetOperatorIdWithIndex));

struct OPAR_CFW_CfgNwGetOperatorIdWithIndex
{
    uint32_t result;
    uint8_t pOperatorId[6];
    uint32_t nIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetOperatorIdWithIndex));

#define TAG_CFW_CfgNwGetOperatorName 0x74dc9c53
struct IPAR_CFW_CfgNwGetOperatorName
{
    uint8_t pOperatorId[6];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetOperatorName));

struct OPAR_CFW_CfgNwGetOperatorName
{
    uint32_t result;
    uint8_t * pOperatorName;
    uint8_t * pOperatorShortName;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetOperatorName));

#define TAG_CFW_CfgSetNwOperDispFormat 0xe503676d
struct IPAR_CFW_CfgSetNwOperDispFormat
{
    uint8_t nFormat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetNwOperDispFormat));

struct OPAR_CFW_CfgSetNwOperDispFormat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetNwOperDispFormat));

#define TAG_CFW_CfgGetNwOperDispFormat 0x5b070593
struct OPAR_CFW_CfgGetNwOperDispFormat
{
    uint32_t result;
    uint8_t nFormat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetNwOperDispFormat));

#define TAG_CFW_SetJammingDetectFlag 0x69ca86ef
struct IPAR_CFW_SetJammingDetectFlag
{
    uint8_t nFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetJammingDetectFlag));

struct OPAR_CFW_SetJammingDetectFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetJammingDetectFlag));

#define TAG_CFW_SetJammingDetectParam 0xe49bfc11
struct IPAR_CFW_SetJammingDetectParam
{
    uint8_t nFlag;
    int16_t nLteThreshold;
    int16_t nGsmThreshold;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetJammingDetectParam));

struct OPAR_CFW_SetJammingDetectParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetJammingDetectParam));

#define TAG_CFW_NwJammingRssiEnquiry 0xa41e015d
struct IPAR_CFW_NwJammingRssiEnquiry
{
    uint8_t nMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwJammingRssiEnquiry));

struct OPAR_CFW_NwJammingRssiEnquiry
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwJammingRssiEnquiry));

#define TAG_CFW_GetJammingDetect 0x0f48160f
struct IPAR_CFW_GetJammingDetect
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetJammingDetect));

struct OPAR_CFW_GetJammingDetect
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetJammingDetect));

#define TAG_CFW_SetJammingDetectParamEx 0xc8470e23
struct IPAR_CFW_SetJammingDetectParamEx
{
    uint8_t bEnable;
    uint8_t detectPeriod;
    int16_t rssiThresholdLte;
    int16_t rsrpThreshold;
    int16_t rsrqThreshold;
    int16_t rssiThreshold;
    int16_t snrThreshold;
    uint16_t minCh;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetJammingDetectParamEx));

struct OPAR_CFW_SetJammingDetectParamEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetJammingDetectParamEx));

#define TAG_CFW_GetFreqScanInfo 0xd27ec527
struct IPAR_CFW_GetFreqScanInfo
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetFreqScanInfo));

struct OPAR_CFW_GetFreqScanInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetFreqScanInfo));

#define TAG_CFW_GetFreqScanEnd 0x94ee01ff
struct IPAR_CFW_GetFreqScanEnd
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetFreqScanEnd));

struct OPAR_CFW_GetFreqScanEnd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetFreqScanEnd));

#define TAG_CFW_GetFreqScanAllInfo 0x94229cdd
struct IPAR_CFW_GetFreqScanAllInfo
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetFreqScanAllInfo));

struct OPAR_CFW_GetFreqScanAllInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetFreqScanAllInfo));

#define TAG_CFW_CheckSupportLteBand 0x9ae742a6
struct IPAR_CFW_CheckSupportLteBand
{
    uint16_t nBand;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CheckSupportLteBand));

struct OPAR_CFW_CheckSupportLteBand
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CheckSupportLteBand));

#define TAG_CFW_CheckSupportLteFreq 0xbad51467
struct IPAR_CFW_CheckSupportLteFreq
{
    uint32_t nFreq;
    uint8_t dlOrUl;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CheckSupportLteFreq));

struct OPAR_CFW_CheckSupportLteFreq
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CheckSupportLteFreq));

#define TAG_CFW_CfgSetSSN 0x27d28c5a
struct IPAR_CFW_CfgSetSSN
{
    uint8_t nCSSI;
    uint8_t nCSSU;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSSN));

struct OPAR_CFW_CfgSetSSN
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSSN));

#define TAG_CFW_CfgGetSSN 0xa99abe2e
struct IPAR_CFW_CfgGetSSN
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSSN));

struct OPAR_CFW_CfgGetSSN
{
    uint32_t result;
    uint8_t nCSSI;
    uint8_t nCSSU;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSSN));

#define TAG_CFW_CfgSetClir 0x3cf96f15
struct IPAR_CFW_CfgSetClir
{
    uint8_t nClir;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetClir));

struct OPAR_CFW_CfgSetClir
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetClir));

#define TAG_CFW_CfgGetClir 0xd6d006b2
struct IPAR_CFW_CfgGetClir
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetClir));

struct OPAR_CFW_CfgGetClir
{
    uint32_t result;
    uint8_t nClir;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetClir));

#define TAG_CFW_CfgSetClip 0x8ddaf837
struct IPAR_CFW_CfgSetClip
{
    uint8_t nEnable;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetClip));

struct OPAR_CFW_CfgSetClip
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetClip));

#define TAG_CFW_CfgGetClip 0x19b6c44a
struct IPAR_CFW_CfgGetClip
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetClip));

struct OPAR_CFW_CfgGetClip
{
    uint32_t result;
    uint8_t nEnable;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetClip));

#define TAG_CFW_CfgSetColp 0x2fd9ebda
struct IPAR_CFW_CfgSetColp
{
    uint8_t n;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetColp));

struct OPAR_CFW_CfgSetColp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetColp));

#define TAG_CFW_CfgGetColp 0x3e336470
struct IPAR_CFW_CfgGetColp
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetColp));

struct OPAR_CFW_CfgGetColp
{
    uint32_t result;
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetColp));

#define TAG_CFW_CfgSetCallWaiting 0x24883270
struct IPAR_CFW_CfgSetCallWaiting
{
    uint8_t n;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetCallWaiting));

struct OPAR_CFW_CfgSetCallWaiting
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetCallWaiting));

#define TAG_CFW_CfgGetCallWaiting 0xb36a4c39
struct IPAR_CFW_CfgGetCallWaiting
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetCallWaiting));

struct OPAR_CFW_CfgGetCallWaiting
{
    uint32_t result;
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetCallWaiting));

#define TAG_CFW_CfgGetIMSI 0x2d03185d
struct IPAR_CFW_CfgGetIMSI
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetIMSI));

struct OPAR_CFW_CfgGetIMSI
{
    uint32_t result;
    uint8_t pIMSI[15];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetIMSI));

#define TAG_CFW_CfgSetIMSI 0x88d9b097
struct IPAR_CFW_CfgSetIMSI
{
    uint8_t pIMSI[15];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetIMSI));

struct OPAR_CFW_CfgSetIMSI
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetIMSI));

#define TAG_CFW_CfgGetCdmaImsi 0x8f00356e
struct IPAR_CFW_CfgGetCdmaImsi
{
    uint8_t pCdmaImsi[10];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetCdmaImsi));

struct OPAR_CFW_CfgGetCdmaImsi
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetCdmaImsi));

#define TAG_CFW_NwSetEpsMode 0x84d40ffe
struct IPAR_CFW_NwSetEpsMode
{
    uint8_t eps_mode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetEpsMode));

struct OPAR_CFW_NwSetEpsMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetEpsMode));

#define TAG_CFW_CfgSetToneDuration 0x75013fe5
struct IPAR_CFW_CfgSetToneDuration
{
    uint8_t nToneDuration;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetToneDuration));

struct OPAR_CFW_CfgSetToneDuration
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetToneDuration));

#define TAG_CFW_CfgGetToneDuration 0x23e5688e
struct OPAR_CFW_CfgGetToneDuration
{
    uint32_t result;
    uint8_t pToneDuration;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetToneDuration));

#define TAG_CFW_CfgSetIncomingCallResultMode 0xc4bee657
struct IPAR_CFW_CfgSetIncomingCallResultMode
{
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetIncomingCallResultMode));

struct OPAR_CFW_CfgSetIncomingCallResultMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetIncomingCallResultMode));

#define TAG_CFW_CfgGetIncomingCallResultMode 0xab6bc7f1
struct OPAR_CFW_CfgGetIncomingCallResultMode
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetIncomingCallResultMode));

#define TAG_CFW_CfgSetAudioOutputParam 0xf851b8fc
struct IPAR_CFW_CfgSetAudioOutputParam
{
    CFW_AUD_OUT_PARAMETER pAudOutParam;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetAudioOutputParam));

struct OPAR_CFW_CfgSetAudioOutputParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetAudioOutputParam));

#define TAG_CFW_CfgGetAudioOutputParam 0x356b9b3e
struct OPAR_CFW_CfgGetAudioOutputParam
{
    uint32_t result;
    CFW_AUD_OUT_PARAMETER pAudOutParam;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetAudioOutputParam));

#define TAG_CFW_CfgSetAudioAudioMode 0x1e00e28a
struct IPAR_CFW_CfgSetAudioAudioMode
{
    uint8_t nAudioMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetAudioAudioMode));

struct OPAR_CFW_CfgSetAudioAudioMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetAudioAudioMode));

#define TAG_CFW_CfgGetAudioAudioMode 0x28488c88
struct OPAR_CFW_CfgGetAudioAudioMode
{
    uint32_t result;
    uint8_t pAudioMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetAudioAudioMode));

#define TAG_CFW_CfgSetLoudspeakerVolumeLevel 0x832b47ad
struct IPAR_CFW_CfgSetLoudspeakerVolumeLevel
{
    uint8_t nVolumeLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetLoudspeakerVolumeLevel));

struct OPAR_CFW_CfgSetLoudspeakerVolumeLevel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetLoudspeakerVolumeLevel));

#define TAG_CFW_CfgGetLoudspeakerVolumeLevel 0x37be59b4
struct OPAR_CFW_CfgGetLoudspeakerVolumeLevel
{
    uint32_t result;
    uint8_t pVolumeLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetLoudspeakerVolumeLevel));

#define TAG_CFW_CfgGetLoudspeakerVolumeLevelRange 0x78fe7b0d
struct OPAR_CFW_CfgGetLoudspeakerVolumeLevelRange
{
    uint32_t result;
    uint8_t pMinVolumeLevel;
    uint8_t pMaxVolumeLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetLoudspeakerVolumeLevelRange));

#define TAG_CFW_ImsInit 0x92d862ef
struct IPAR_CFW_ImsInit
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsInit));

struct OPAR_CFW_ImsInit
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsInit));

#define TAG_CFW_ImsEnable 0xe4ed0761
struct IPAR_CFW_ImsEnable
{
    uint8_t isEmergency;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsEnable));

struct OPAR_CFW_ImsEnable
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsEnable));

#define TAG_CFW_ImsDisable 0x9c72a751
struct IPAR_CFW_ImsDisable
{
    uint8_t isEmergency;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsDisable));

struct OPAR_CFW_ImsDisable
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsDisable));

#define TAG_CFW_ImsSuspend 0xa45579e4
struct IPAR_CFW_ImsSuspend
{
    CFW_SIM_ID nSimID;
    CFW_IMS_SUSPEND_TYPE nType;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSuspend));

struct OPAR_CFW_ImsSuspend
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSuspend));

#define TAG_CFW_ImsResume 0xb76466e9
struct IPAR_CFW_ImsResume
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsResume));

struct OPAR_CFW_ImsResume
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsResume));

#define TAG_CFW_GprsGetNsapi 0x6198a170
struct IPAR_CFW_GprsGetNsapi
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetNsapi));

struct OPAR_CFW_GprsGetNsapi
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetNsapi));

#define TAG_CFW_ImsSetDapsCallback 0x554d3316
struct IPAR_CFW_ImsSetDapsCallback
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetDapsCallback));

struct OPAR_CFW_ImsSetDapsCallback
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetDapsCallback));

#define TAG_CFW_ImsSetVolte 0xf52fc960
struct IPAR_CFW_ImsSetVolte
{
    uint8_t bSet;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetVolte));

struct OPAR_CFW_ImsSetVolte
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetVolte));

#define TAG_CFW_ImsSetCevdp 0x0d905f27
struct IPAR_CFW_ImsSetCevdp
{
    uint8_t setting;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetCevdp));

struct OPAR_CFW_ImsSetCevdp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetCevdp));

#define TAG_CFW_ImsGetCevdp 0x44f31984
struct IPAR_CFW_ImsGetCevdp
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsGetCevdp));

struct OPAR_CFW_ImsGetCevdp
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsGetCevdp));

#define TAG_CFW_ImsSetTerminalMode 0xc79f2975
struct IPAR_CFW_ImsSetTerminalMode
{
    uint8_t setting;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetTerminalMode));

struct OPAR_CFW_ImsSetTerminalMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetTerminalMode));

#define TAG_CFW_ImsGetTerminalMode 0xab3d76b5
struct IPAR_CFW_ImsGetTerminalMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsGetTerminalMode));

struct OPAR_CFW_ImsGetTerminalMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsGetTerminalMode));

#define TAG_CFW_ImsSetSrvccMode 0x0b56f356
struct IPAR_CFW_ImsSetSrvccMode
{
    uint8_t ueSrvcc;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetSrvccMode));

struct OPAR_CFW_ImsSetSrvccMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetSrvccMode));

#define TAG_CFW_ImsGetSrvccMode 0xa1821af0
struct IPAR_CFW_ImsGetSrvccMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsGetSrvccMode));

struct OPAR_CFW_ImsGetSrvccMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsGetSrvccMode));

#define TAG_CFW_ImsIsSet 0x9f1f9c07
struct IPAR_CFW_ImsIsSet
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsIsSet));

struct OPAR_CFW_ImsIsSet
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsIsSet));

#define TAG_CFW_ImsFreeUti 0xfeb9adf0
struct OPAR_CFW_ImsFreeUti
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsFreeUti));

#define TAG_CFW_ImsActImsPdp 0xe60f5b9d
struct IPAR_CFW_ImsActImsPdp
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsActImsPdp));

struct OPAR_CFW_ImsActImsPdp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsActImsPdp));

#define TAG_CFW_ImsDoSavedEmcDail 0x555cee71
struct OPAR_CFW_ImsDoSavedEmcDail
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsDoSavedEmcDail));

#define TAG_CFW_ImsSetPdpStatus 0x6b251398
struct IPAR_CFW_ImsSetPdpStatus
{
    uint8_t nStatus;
    uint8_t isEmergency;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetPdpStatus));

struct OPAR_CFW_ImsSetPdpStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetPdpStatus));

#define TAG_CFW_ImsGetPdpStatus 0xe3bacaaf
struct IPAR_CFW_ImsGetPdpStatus
{
    uint8_t isEmergency;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsGetPdpStatus));

struct OPAR_CFW_ImsGetPdpStatus
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsGetPdpStatus));

#define TAG_CFW_ImsCheckStatusAfterCc 0x2b9844f1
struct IPAR_CFW_ImsCheckStatusAfterCc
{
    uint8_t nCsImsFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsCheckStatusAfterCc));

struct OPAR_CFW_ImsCheckStatusAfterCc
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsCheckStatusAfterCc));

#define TAG_CFW_SsSetClip 0x9d929a96
struct IPAR_CFW_SsSetClip
{
    uint8_t set;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetClip));

struct OPAR_CFW_SsSetClip
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetClip));

#define TAG_CFW_SsSetClir 0xad7d0f90
struct IPAR_CFW_SsSetClir
{
    uint8_t set;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetClir));

struct OPAR_CFW_SsSetClir
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetClir));

#define TAG_CFW_SsSetColp 0xc637a64f
struct IPAR_CFW_SsSetColp
{
    uint8_t set;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetColp));

struct OPAR_CFW_SsSetColp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetColp));

#define TAG_CFW_ImsIsRegistered 0xdbf659dd
struct IPAR_CFW_ImsIsRegistered
{
    uint8_t isEmergency;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsIsRegistered));

struct OPAR_CFW_ImsIsRegistered
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsIsRegistered));

#define TAG_CFW_NWSetRat 0xb7e77ecb
struct IPAR_CFW_NWSetRat
{
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWSetRat));

struct OPAR_CFW_NWSetRat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWSetRat));

#define TAG_CFW_NWGetRat 0x85e2fdf5
struct IPAR_CFW_NWGetRat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWGetRat));

struct OPAR_CFW_NWGetRat
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWGetRat));

#define TAG_CFW_NWSetStackRat 0x3cdf3da7
struct IPAR_CFW_NWSetStackRat
{
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWSetStackRat));

struct OPAR_CFW_NWSetStackRat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWSetStackRat));

#define TAG_CFW_NWGetStackRat 0xa2cd3816
struct IPAR_CFW_NWGetStackRat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWGetStackRat));

struct OPAR_CFW_NWGetStackRat
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWGetStackRat));

#define TAG_CFW_NwSetNetMode 0x2c64c51d
struct IPAR_CFW_NwSetNetMode
{
    uint8_t nNetMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetNetMode));

struct OPAR_CFW_NwSetNetMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetNetMode));

#define TAG_CFW_NwGetAvailableOperators 0x76d14edf
struct IPAR_CFW_NwGetAvailableOperators
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetAvailableOperators));

struct OPAR_CFW_NwGetAvailableOperators
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetAvailableOperators));

#define TAG_CFW_NwGetAvailableOperators_V2 0xac9a08fc
struct IPAR_CFW_NwGetAvailableOperators_V2
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetAvailableOperators_V2));

struct OPAR_CFW_NwGetAvailableOperators_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetAvailableOperators_V2));

#define TAG_CFW_NwGetCurrentOperator 0x235d1b94
struct IPAR_CFW_NwGetCurrentOperator
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetCurrentOperator));

struct OPAR_CFW_NwGetCurrentOperator
{
    uint32_t result;
    uint8_t OperatorId[6];
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetCurrentOperator));

#define TAG_CFW_NwGetImei 0xfab521ed
struct IPAR_CFW_NwGetImei
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetImei));

struct OPAR_CFW_NwGetImei
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetImei));

#define TAG_CFW_NwGetSignalQuality 0x9723220b
struct IPAR_CFW_NwGetSignalQuality
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetSignalQuality));

struct OPAR_CFW_NwGetSignalQuality
{
    uint32_t result;
    uint8_t pSignalLevel;
    uint8_t pBitError;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetSignalQuality));

#define TAG_CFW_NwGetLteSignalQuality 0x81fd8a88
struct IPAR_CFW_NwGetLteSignalQuality
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetLteSignalQuality));

struct OPAR_CFW_NwGetLteSignalQuality
{
    uint32_t result;
    uint8_t pSignalLevel;
    uint8_t pBitError;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetLteSignalQuality));

#define TAG_CFW_NwGetStatus 0x9af699bd
struct IPAR_CFW_NwGetStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetStatus));

struct OPAR_CFW_NwGetStatus
{
    uint32_t result;
    CFW_NW_STATUS_INFO pStatusInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetStatus));

#define TAG_CFW_NwGetStatisticRpt 0x7e6f2cb2
struct IPAR_CFW_NwGetStatisticRpt
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetStatisticRpt));

struct OPAR_CFW_NwGetStatisticRpt
{
    uint32_t result;
    CFW_ENG_STATIS_RPT pEngStatisRpt;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetStatisticRpt));

#define TAG_CFW_NwSetRegistration 0x825d1b6c
struct IPAR_CFW_NwSetRegistration
{
    uint8_t nOperatorId[6];
    uint8_t nMode;
    uint8_t nRat;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetRegistration));

struct OPAR_CFW_NwSetRegistration
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetRegistration));

#define TAG_CFW_NwDeRegister 0x47c56cdb
struct IPAR_CFW_NwDeRegister
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwDeRegister));

struct OPAR_CFW_NwDeRegister
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwDeRegister));

#define TAG_CFW_DetachCSAndPS 0x62d52415
struct IPAR_CFW_DetachCSAndPS
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_DetachCSAndPS));

struct OPAR_CFW_DetachCSAndPS
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_DetachCSAndPS));

#define TAG_CFW_AttachCSAndPS 0x5be8fc29
struct IPAR_CFW_AttachCSAndPS
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_AttachCSAndPS));

struct OPAR_CFW_AttachCSAndPS
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_AttachCSAndPS));

#define TAG_CFW_NwGetAllDetach 0x3fd424e9
struct IPAR_CFW_NwGetAllDetach
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetAllDetach));

struct OPAR_CFW_NwGetAllDetach
{
    uint32_t result;
    uint8_t nAttDetach;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetAllDetach));

#define TAG_CFW_NwGetHNBName 0x8e089634
struct IPAR_CFW_NwGetHNBName
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetHNBName));

struct OPAR_CFW_NwGetHNBName
{
    uint32_t result;
    uint8_t pLength;
    uint8_t pHNBName[48];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetHNBName));

#define TAG_CFW_NwCSGSetRegistration 0x8d09aa3f
struct IPAR_CFW_NwCSGSetRegistration
{
    uint8_t nOperatorId[6];
    uint8_t nMode;
    uint8_t nRat;
    uint32_t nCsgId;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwCSGSetRegistration));

struct OPAR_CFW_NwCSGSetRegistration
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwCSGSetRegistration));

#define TAG_CFW_NwGetCsgOperators 0x95df4b83
struct IPAR_CFW_NwGetCsgOperators
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetCsgOperators));

struct OPAR_CFW_NwGetCsgOperators
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetCsgOperators));

#define TAG_CFW_SetComm 0xfae6ff84
struct IPAR_CFW_SetComm
{
    CFW_COMM_MODE nMode;
    uint8_t nStorageFlag;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetComm));

struct OPAR_CFW_SetComm
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetComm));

#define TAG_CFW_GetComm 0x965f544e
struct IPAR_CFW_GetComm
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetComm));

struct OPAR_CFW_GetComm
{
    uint32_t result;
    CFW_COMM_MODE nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetComm));

#define TAG_CFW_NwSetCSMode 0x8372d825
struct IPAR_CFW_NwSetCSMode
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetCSMode));

struct OPAR_CFW_NwSetCSMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetCSMode));

#define TAG_CFW_NwSetFrequencyBand 0x60c38c93
struct IPAR_CFW_NwSetFrequencyBand
{
    uint8_t nBand;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetFrequencyBand));

struct OPAR_CFW_NwSetFrequencyBand
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetFrequencyBand));

#define TAG_CFW_NwAbortListOperators 0x6fcc1282
struct IPAR_CFW_NwAbortListOperators
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwAbortListOperators));

struct OPAR_CFW_NwAbortListOperators
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwAbortListOperators));

#define TAG_CFW_GetNWTimerOutFlag 0x0a79d13a
struct IPAR_CFW_GetNWTimerOutFlag
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNWTimerOutFlag));

struct OPAR_CFW_GetNWTimerOutFlag
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNWTimerOutFlag));

#define TAG_CFW_SetNWTimerOutFlag 0xf65e019c
struct IPAR_CFW_SetNWTimerOutFlag
{
    bool bClear;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetNWTimerOutFlag));

#define TAG_CFW_CcAcceptSpeechCall 0x2b480237
struct IPAR_CFW_CcAcceptSpeechCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcAcceptSpeechCall));

struct OPAR_CFW_CcAcceptSpeechCall
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcAcceptSpeechCall));

#define TAG_CFW_CcGetCallStatus 0x46843945
struct IPAR_CFW_CcGetCallStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcGetCallStatus));

struct OPAR_CFW_CcGetCallStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcGetCallStatus));

#define TAG_CFW_GetCcCount 0x276ee189
struct IPAR_CFW_GetCcCount
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetCcCount));

struct OPAR_CFW_GetCcCount
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetCcCount));

#define TAG_CFW_CcGetCurrentCall 0x8da891d9
struct IPAR_CFW_CcGetCurrentCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcGetCurrentCall));

struct OPAR_CFW_CcGetCurrentCall
{
    uint32_t result;
    CFW_CC_CURRENT_CALL_INFO CallInfo[7];
    uint8_t pCnt;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcGetCurrentCall));

#define TAG_CFW_CcGetCurrentCallV2 0xd0d78967
struct IPAR_CFW_CcGetCurrentCallV2
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcGetCurrentCallV2));

struct OPAR_CFW_CcGetCurrentCallV2
{
    uint32_t result;
    CFW_CC_CURRENT_CALL_INFO_V2 CallInfo[7];
    uint8_t pCnt;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcGetCurrentCallV2));

#define TAG_CFW_CcCallHoldMultiparty 0xd2b4a305
struct IPAR_CFW_CcCallHoldMultiparty
{
    uint8_t nCmd;
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcCallHoldMultiparty));

struct OPAR_CFW_CcCallHoldMultiparty
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcCallHoldMultiparty));

#define TAG_CFW_CcCallHoldMultiparty_V2 0x21fec076
struct IPAR_CFW_CcCallHoldMultiparty_V2
{
    uint16_t nUTI;
    uint8_t nCmd;
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcCallHoldMultiparty_V2));

struct OPAR_CFW_CcCallHoldMultiparty_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcCallHoldMultiparty_V2));

#define TAG_CFW_CcReleaseCallX 0x55e31e22
struct IPAR_CFW_CcReleaseCallX
{
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcReleaseCallX));

struct OPAR_CFW_CcReleaseCallX
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcReleaseCallX));

#define TAG_CFW_CcReleaseCallX_V2 0xd788206a
struct IPAR_CFW_CcReleaseCallX_V2
{
    uint16_t nUTI;
    uint8_t nIndex;
    uint8_t cause;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcReleaseCallX_V2));

struct OPAR_CFW_CcReleaseCallX_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcReleaseCallX_V2));

#define TAG_CFW_CcInitiateSpeechCall_V2 0x10d83a33
struct IPAR_CFW_CcInitiateSpeechCall_V2
{
    CFW_DIALNUMBER_V2 pDialNumber;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcInitiateSpeechCall_V2));

struct OPAR_CFW_CcInitiateSpeechCall_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcInitiateSpeechCall_V2));

#define TAG_CFW_CcInitiateSpeechCallEx_V2 0xf3beb108
struct IPAR_CFW_CcInitiateSpeechCallEx_V2
{
    CFW_DIALNUMBER_V2 pDialNumber;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcInitiateSpeechCallEx_V2));

struct OPAR_CFW_CcInitiateSpeechCallEx_V2
{
    uint32_t result;
    uint8_t pIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcInitiateSpeechCallEx_V2));

#define TAG_CFW_CcEmcDial 0x096b6d42
struct IPAR_CFW_CcEmcDial
{
    uint8_t nSize;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcEmcDial));

struct OPAR_CFW_CcEmcDial
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcEmcDial));

#define TAG_CFW_CcEmcDialEx 0x48762fb4
struct IPAR_CFW_CcEmcDialEx
{
    uint8_t nDialNumberSize;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcEmcDialEx));

struct OPAR_CFW_CcEmcDialEx
{
    uint32_t result;
    uint8_t pIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcEmcDialEx));

#define TAG_CFW_CcReleaseCall 0x8c4457d3
struct IPAR_CFW_CcReleaseCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcReleaseCall));

struct OPAR_CFW_CcReleaseCall
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcReleaseCall));

#define TAG_CFW_CcReleaseCall_V2 0x89cdc100
struct IPAR_CFW_CcReleaseCall_V2
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcReleaseCall_V2));

struct OPAR_CFW_CcReleaseCall_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcReleaseCall_V2));

#define TAG_CFW_CcRejectCall 0xace9ca72
struct IPAR_CFW_CcRejectCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcRejectCall));

struct OPAR_CFW_CcRejectCall
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcRejectCall));

#define TAG_CFW_CcPlayTone 0x08790a03
struct IPAR_CFW_CcPlayTone
{
    int8_t iTone;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcPlayTone));

struct OPAR_CFW_CcPlayTone
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcPlayTone));

#define TAG_CFW_CcStopTone 0x615f0347
struct IPAR_CFW_CcStopTone
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcStopTone));

struct OPAR_CFW_CcStopTone
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcStopTone));

#define TAG_CFW_CcSetCsfbmtFlag 0x3c1e6a02
struct IPAR_CFW_CcSetCsfbmtFlag
{
    uint8_t nFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcSetCsfbmtFlag));

struct OPAR_CFW_CcSetCsfbmtFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcSetCsfbmtFlag));

#define TAG_CFW_CcGetCsfbmtFlag 0x07a8fc4f
struct IPAR_CFW_CcGetCsfbmtFlag
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcGetCsfbmtFlag));

struct OPAR_CFW_CcGetCsfbmtFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcGetCsfbmtFlag));

#define TAG_CFW_SsQueryCallWaiting 0x2e1489d8
struct IPAR_CFW_SsQueryCallWaiting
{
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryCallWaiting));

struct OPAR_CFW_SsQueryCallWaiting
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryCallWaiting));

#define TAG_CFW_SsSetCallWaiting 0xe9225666
struct IPAR_CFW_SsSetCallWaiting
{
    uint8_t nMode;
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetCallWaiting));

struct OPAR_CFW_SsSetCallWaiting
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetCallWaiting));

#define TAG_CFW_SsSetCallForwarding 0x7dbb6ec7
struct IPAR_CFW_SsSetCallForwarding
{
    CFW_SS_SET_CALLFORWARDING_INFO pCallForwarding;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetCallForwarding));

struct OPAR_CFW_SsSetCallForwarding
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetCallForwarding));

#define TAG_CFW_SsQueryCallForwarding 0xf5ab108b
struct IPAR_CFW_SsQueryCallForwarding
{
    uint8_t nReason;
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryCallForwarding));

struct OPAR_CFW_SsQueryCallForwarding
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryCallForwarding));

#define TAG_CFW_SsSetFacilityLock 0x26f63727
struct IPAR_CFW_SsSetFacilityLock
{
    uint16_t nFac;
    uint8_t nPwdSize;
    uint8_t nClass;
    uint8_t nMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetFacilityLock));

struct OPAR_CFW_SsSetFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetFacilityLock));

#define TAG_CFW_SsQueryFacilityLock 0x40860195
struct IPAR_CFW_SsQueryFacilityLock
{
    uint16_t nFac;
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryFacilityLock));

struct OPAR_CFW_SsQueryFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryFacilityLock));

#define TAG_CFW_SsChangePassword 0xb7128488
struct IPAR_CFW_SsChangePassword
{
    uint16_t nFac;
    uint8_t nOldPwdSize;
    uint8_t nNewPwdSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsChangePassword));

struct OPAR_CFW_SsChangePassword
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsChangePassword));

#define TAG_CFW_SsQueryClip 0x618b132d
struct IPAR_CFW_SsQueryClip
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryClip));

struct OPAR_CFW_SsQueryClip
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryClip));

#define TAG_CFW_SsQueryClir 0xa6720283
struct IPAR_CFW_SsQueryClir
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryClir));

struct OPAR_CFW_SsQueryClir
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryClir));

#define TAG_CFW_SsQueryColp 0x895b63e7
struct IPAR_CFW_SsQueryColp
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryColp));

struct OPAR_CFW_SsQueryColp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryColp));

#define TAG_CFW_SsQueryColr 0xd6d9fa1b
struct IPAR_CFW_SsQueryColr
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryColr));

struct OPAR_CFW_SsQueryColr
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryColr));

#define TAG_CFW_SsSendUSSD 0x89f8a01a
struct IPAR_CFW_SsSendUSSD
{
    uint8_t nUsdStringSize;
    uint8_t nOption;
    uint8_t nDcs;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSendUSSD));

struct OPAR_CFW_SsSendUSSD
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSendUSSD));

#define TAG_CFW_SsSendUSSD_V2 0xac92524d
struct IPAR_CFW_SsSendUSSD_V2
{
    uint8_t nUsdStringSize;
    uint8_t nOption;
    uint8_t nDcs;
    uint16_t nUTI;
    uint16_t nTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSendUSSD_V2));

struct OPAR_CFW_SsSendUSSD_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSendUSSD_V2));

#define TAG_CFW_SimChangePassword 0xb3513926
struct IPAR_CFW_SimChangePassword
{
    uint16_t nFac;
    uint8_t nOldPwdSize;
    uint8_t nNewPwdSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimChangePassword));

struct OPAR_CFW_SimChangePassword
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimChangePassword));

#define TAG_CFW_SimGetAuthenticationStatus 0x3f6c8048
struct IPAR_CFW_SimGetAuthenticationStatus
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetAuthenticationStatus));

struct OPAR_CFW_SimGetAuthenticationStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetAuthenticationStatus));

#define TAG_CFW_SimEnterAuthentication 0xda558016
struct IPAR_CFW_SimEnterAuthentication
{
    uint8_t nPinSize;
    uint8_t nNewPinSize;
    uint8_t nOption;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimEnterAuthentication));

struct OPAR_CFW_SimEnterAuthentication
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimEnterAuthentication));

#define TAG_CFW_SimSetFacilityLock 0xd6fab239
struct IPAR_CFW_SimSetFacilityLock
{
    uint16_t nFac;
    uint8_t nPwdSize;
    uint8_t nMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetFacilityLock));

struct OPAR_CFW_SimSetFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetFacilityLock));

#define TAG_CFW_SimGetFacilityLock 0xd28c1f50
struct IPAR_CFW_SimGetFacilityLock
{
    uint16_t nFac;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetFacilityLock));

struct OPAR_CFW_SimGetFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetFacilityLock));

#define TAG_CFW_SimGetProviderId 0x3bc96b1f
struct IPAR_CFW_SimGetProviderId
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetProviderId));

struct OPAR_CFW_SimGetProviderId
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetProviderId));

#define TAG_CFW_SetCurrPreferPLMNList 0x71304b64
struct IPAR_CFW_SetCurrPreferPLMNList
{
    uint8_t iList;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetCurrPreferPLMNList));

#define TAG_CFW_SimWritePreferPLMN 0xc88ef4b9
struct IPAR_CFW_SimWritePreferPLMN
{
    uint8_t index;
    uint8_t nSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimWritePreferPLMN));

struct OPAR_CFW_SimWritePreferPLMN
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimWritePreferPLMN));

#define TAG_CFW_SimSetPrefOperatorList 0x1f9e7d3c
struct IPAR_CFW_SimSetPrefOperatorList
{
    uint8_t nSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetPrefOperatorList));

struct OPAR_CFW_SimSetPrefOperatorList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetPrefOperatorList));

#define TAG_CFW_SimReadPreferPLMN 0x52f296ca
struct IPAR_CFW_SimReadPreferPLMN
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadPreferPLMN));

struct OPAR_CFW_SimReadPreferPLMN
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadPreferPLMN));

#define TAG_CFW_SimGetPrefOperatorListMaxNum 0xa7a1c43e
struct IPAR_CFW_SimGetPrefOperatorListMaxNum
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPrefOperatorListMaxNum));

struct OPAR_CFW_SimGetPrefOperatorListMaxNum
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPrefOperatorListMaxNum));

#define TAG_CFW_SimGetPrefOperatorList 0x2fc7c1ba
struct IPAR_CFW_SimGetPrefOperatorList
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPrefOperatorList));

struct OPAR_CFW_SimGetPrefOperatorList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPrefOperatorList));

#define TAG_CFW_SimAddPbkEntry 0x045ceb49
struct IPAR_CFW_SimAddPbkEntry
{
    uint8_t nStorage;
    CFW_SIM_PBK_ENTRY_INFO pEntryInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimAddPbkEntry));

struct OPAR_CFW_SimAddPbkEntry
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimAddPbkEntry));

#define TAG_CFW_SimAddPbkEntry_V2 0xb5beb0cf
struct IPAR_CFW_SimAddPbkEntry_V2
{
    uint8_t nStorage;
    CFW_SIM_PBK_ENTRY_INFO_V2 pEntryInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimAddPbkEntry_V2));

struct OPAR_CFW_SimAddPbkEntry_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimAddPbkEntry_V2));

#define TAG_CFW_SimDeletePbkEntry 0xa64720ca
struct IPAR_CFW_SimDeletePbkEntry
{
    uint8_t nStorage;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimDeletePbkEntry));

struct OPAR_CFW_SimDeletePbkEntry
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimDeletePbkEntry));

#define TAG_CFW_SimGetPbkEntry 0xaa8591be
struct IPAR_CFW_SimGetPbkEntry
{
    uint8_t nStorage;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPbkEntry));

struct OPAR_CFW_SimGetPbkEntry
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPbkEntry));

#define TAG_CFW_SimListPbkEntries 0x62e3977e
struct IPAR_CFW_SimListPbkEntries
{
    uint8_t nStorage;
    uint16_t nIndexStart;
    uint16_t nIndexEnd;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimListPbkEntries));

struct OPAR_CFW_SimListPbkEntries
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimListPbkEntries));

#define TAG_CFW_SimListCountPbkEntries 0x5e84647a
struct IPAR_CFW_SimListCountPbkEntries
{
    uint8_t nStorage;
    uint16_t nIndexStart;
    uint16_t nCount;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimListCountPbkEntries));

struct OPAR_CFW_SimListCountPbkEntries
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimListCountPbkEntries));

#define TAG_CFW_SimGetPbkStrorageInfo 0x87cf01e4
struct IPAR_CFW_SimGetPbkStrorageInfo
{
    uint8_t nStorage;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPbkStrorageInfo));

struct OPAR_CFW_SimGetPbkStrorageInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPbkStrorageInfo));

#define TAG_CFW_SimGetPbkStorage 0x4ab2b8c4
struct IPAR_CFW_SimGetPbkStorage
{
    uint8_t nStorage;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPbkStorage));

struct OPAR_CFW_SimGetPbkStorage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPbkStorage));

#define TAG_CFW_SatActivation 0x2d9d2455
struct IPAR_CFW_SatActivation
{
    uint8_t nMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatActivation));

struct OPAR_CFW_SatActivation
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatActivation));

#define TAG_CFW_SatResponse 0x4fb39c55
struct IPAR_CFW_SatResponse
{
    uint8_t nCmdType;
    uint8_t nStatus;
    uint8_t nItemId;
    uint8_t InputStrLen;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatResponse));

struct OPAR_CFW_SatResponse
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatResponse));

#define TAG_CFW_SatResponseV2 0x7706ee90
struct IPAR_CFW_SatResponseV2
{
    uint8_t nCmdType;
    uint8_t nStatus;
    uint8_t nItemId;
    uint16_t InputStrLen;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatResponseV2));

struct OPAR_CFW_SatResponseV2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatResponseV2));

#define TAG_CFW_SatGetCurCMD 0x5a8598b0
struct IPAR_CFW_SatGetCurCMD
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetCurCMD));

struct OPAR_CFW_SatGetCurCMD
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetCurCMD));

#define TAG_CFW_GetMNCLen 0x95b333c9
struct IPAR_CFW_GetMNCLen
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetMNCLen));

struct OPAR_CFW_GetMNCLen
{
    uint8_t pLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetMNCLen));

#define TAG_CFW_CfgSimGetMeProfile 0x365a1560
struct OPAR_CFW_CfgSimGetMeProfile
{
    uint32_t result;
    CFW_PROFILE pMeProfile;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSimGetMeProfile));

#define TAG_CFW_CfgSimSetMeProfile 0x398eddda
struct IPAR_CFW_CfgSimSetMeProfile
{
    CFW_PROFILE pMeProfile;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSimSetMeProfile));

struct OPAR_CFW_CfgSimSetMeProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSimSetMeProfile));

#define TAG_CFW_SimGetICCID 0x0807f6e5
struct IPAR_CFW_SimGetICCID
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetICCID));

struct OPAR_CFW_SimGetICCID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetICCID));

#define TAG_CFW_SmsAbortSendMessage 0x3d6b4b93
struct IPAR_CFW_SmsAbortSendMessage
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsAbortSendMessage));

struct OPAR_CFW_SmsAbortSendMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsAbortSendMessage));

#define TAG_CFW_SmsDeleteMessage 0x50cab25c
struct IPAR_CFW_SmsDeleteMessage
{
    uint16_t nIndex;
    uint8_t nStatus;
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsDeleteMessage));

struct OPAR_CFW_SmsDeleteMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsDeleteMessage));

#define TAG_CFW_SmsSetUnRead2Read 0xde75aebe
struct IPAR_CFW_SmsSetUnRead2Read
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSetUnRead2Read));

struct OPAR_CFW_SmsSetUnRead2Read
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSetUnRead2Read));

#define TAG_CFW_SmsListMessages 0xdb8ad850
struct IPAR_CFW_SmsListMessages
{
    CFW_SMS_LIST pListInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsListMessages));

struct OPAR_CFW_SmsListMessages
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsListMessages));

#define TAG_CFW_SmsReadMessage 0x9e37b0ea
struct IPAR_CFW_SmsReadMessage
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsReadMessage));

struct OPAR_CFW_SmsReadMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsReadMessage));

#define TAG_CFW_SimGetServiceProviderName 0x67735c93
struct IPAR_CFW_SimGetServiceProviderName
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetServiceProviderName));

struct OPAR_CFW_SimGetServiceProviderName
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetServiceProviderName));

#define TAG_CFW_SmsSetRead2UnRead 0xf4a2809f
struct IPAR_CFW_SmsSetRead2UnRead
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSetRead2UnRead));

struct OPAR_CFW_SmsSetRead2UnRead
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSetRead2UnRead));

#define TAG_CFW_CheckIfAllowedRecvSms 0xee15e996
struct IPAR_CFW_CheckIfAllowedRecvSms
{
    bool bIfAllowed;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CheckIfAllowedRecvSms));

#define TAG_CFW_SmsSendPduMessage 0xc021092d
struct IPAR_CFW_SmsSendPduMessage
{
    uint16_t nDataSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSendPduMessage));

struct OPAR_CFW_SmsSendPduMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSendPduMessage));

#define TAG_CFW_SmsSendMessage_V2 0xab91f894
struct IPAR_CFW_SmsSendMessage_V2
{
    CFW_DIALNUMBER_V2 pNumber;
    uint16_t nDataSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSendMessage_V2));

struct OPAR_CFW_SmsSendMessage_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSendMessage_V2));

#define TAG_CFW_SmsSendMessage_V3 0x6d3c7e9f
struct IPAR_CFW_SmsSendMessage_V3
{
    CFW_DIALNUMBER_V2 pNumber;
    uint16_t nDataSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
    uint8_t nFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSendMessage_V3));

struct OPAR_CFW_SmsSendMessage_V3
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSendMessage_V3));

#define TAG_CFW_SmsWriteMessage_V2 0x2c80e79f
struct IPAR_CFW_SmsWriteMessage_V2
{
    CFW_DIALNUMBER_V2 pNumber;
    uint16_t nDataSize;
    uint16_t nIndex;
    uint8_t nStorage;
    uint8_t nType;
    uint8_t nStatus;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsWriteMessage_V2));

struct OPAR_CFW_SmsWriteMessage_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsWriteMessage_V2));

#define TAG_CFW_SmsSetUnSent2Sent 0xa8596703
struct IPAR_CFW_SmsSetUnSent2Sent
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint8_t nSendStatus;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSetUnSent2Sent));

struct OPAR_CFW_SmsSetUnSent2Sent
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSetUnSent2Sent));

#define TAG_CFW_SmsGetMessageNodeEx 0xc6e5fb72
struct IPAR_CFW_SmsGetMessageNodeEx
{
    void * pListResult;
    uint16_t nIndex;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsGetMessageNodeEx));

struct OPAR_CFW_SmsGetMessageNodeEx
{
    uint32_t result;
    CFW_SMS_NODE_EX pNode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsGetMessageNodeEx));

#define TAG_CFW_SmsGetMessageNodeV2 0xc019ab50
struct IPAR_CFW_SmsGetMessageNodeV2
{
    void * pListResult;
    uint16_t nIndex;
    uint8_t nFormat;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsGetMessageNodeV2));

struct OPAR_CFW_SmsGetMessageNodeV2
{
    uint32_t result;
    CFW_SMS_NODE_EX pNode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsGetMessageNodeV2));

#define TAG_CFW_SmsInitComplete 0x0aeb7786
struct IPAR_CFW_SmsInitComplete
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsInitComplete));

struct OPAR_CFW_SmsInitComplete
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsInitComplete));

#define TAG_CFW_SetSMSConcat 0x41075729
struct IPAR_CFW_SetSMSConcat
{
    bool bConcat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSMSConcat));

struct OPAR_CFW_SetSMSConcat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetSMSConcat));

#define TAG_CFW_GetSMSConcat 0x16e18027
struct IPAR_CFW_GetSMSConcat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSMSConcat));

struct OPAR_CFW_GetSMSConcat
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSMSConcat));

#define TAG_CFW_SimReadMessage 0x24e30979
struct IPAR_CFW_SimReadMessage
{
    uint16_t nLocation;
    uint32_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadMessage));

struct OPAR_CFW_SimReadMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadMessage));

#define TAG_CFW_SimWriteMessage 0x606ae82f
struct IPAR_CFW_SimWriteMessage
{
    uint16_t nLocation;
    uint32_t nIndex;
    uint8_t nDataSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimWriteMessage));

struct OPAR_CFW_SimWriteMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimWriteMessage));

#define TAG_CFW_GprsSetPdpCxtV2 0x6e3e849a
struct IPAR_CFW_GprsSetPdpCxtV2
{
    uint8_t nCid;
    CFW_GPRS_PDPCONT_INFO_V2 pPdpCont;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetPdpCxtV2));

struct OPAR_CFW_GprsSetPdpCxtV2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetPdpCxtV2));

#define TAG_CFW_GprsGetDefaultPdnInfo 0x48347890
struct IPAR_CFW_GprsGetDefaultPdnInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetDefaultPdnInfo));

struct OPAR_CFW_GprsGetDefaultPdnInfo
{
    uint8_t result;
    CFW_GPRS_PDPCONT_DFTPDN_INFO PdnInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetDefaultPdnInfo));

#define TAG_CFW_GprsGetPdpCxtV2 0x28c73dae
struct IPAR_CFW_GprsGetPdpCxtV2
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetPdpCxtV2));

struct OPAR_CFW_GprsGetPdpCxtV2
{
    uint32_t result;
    CFW_GPRS_PDPCONT_INFO_V2 pPdpCont;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetPdpCxtV2));

#define TAG_CFW_GprsSetDefaultPdnMode 0x631195a3
struct IPAR_CFW_GprsSetDefaultPdnMode
{
    uint32_t mode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetDefaultPdnMode));

#define TAG_CFW_GprsGetDefaultPdnMode 0x90c3c37c
struct IPAR_CFW_GprsGetDefaultPdnMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetDefaultPdnMode));

struct OPAR_CFW_GprsGetDefaultPdnMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetDefaultPdnMode));

#define TAG_CFW_GprsRemovePdpCxt 0x898b83e5
struct IPAR_CFW_GprsRemovePdpCxt
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsRemovePdpCxt));

struct OPAR_CFW_GprsRemovePdpCxt
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsRemovePdpCxt));

#define TAG_CFW_GprsSetMinQos 0xdd6267c5
struct IPAR_CFW_GprsSetMinQos
{
    uint8_t nCid;
    CFW_GPRS_QOS pQos;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetMinQos));

struct OPAR_CFW_GprsSetMinQos
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetMinQos));

#define TAG_CFW_GprsGetMInQos 0x53a8e0fb
struct IPAR_CFW_GprsGetMInQos
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetMInQos));

struct OPAR_CFW_GprsGetMInQos
{
    uint32_t result;
    CFW_GPRS_QOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetMInQos));

#define TAG_CFW_GprsSetReqQos 0x945f113e
struct IPAR_CFW_GprsSetReqQos
{
    uint8_t nCid;
    CFW_GPRS_QOS pQos;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetReqQos));

struct OPAR_CFW_GprsSetReqQos
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetReqQos));

#define TAG_CFW_GprsGetReqQos 0xf0322972
struct IPAR_CFW_GprsGetReqQos
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetReqQos));

struct OPAR_CFW_GprsGetReqQos
{
    uint32_t result;
    CFW_GPRS_QOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetReqQos));

#define TAG_CFW_GprsSetReqQosUmts 0x2853b066
struct IPAR_CFW_GprsSetReqQosUmts
{
    uint8_t nCid;
    CFW_GPRS_QOS pQos;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetReqQosUmts));

struct OPAR_CFW_GprsSetReqQosUmts
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetReqQosUmts));

#define TAG_CFW_GprsGetReqQosUmts 0xb195ac35
struct IPAR_CFW_GprsGetReqQosUmts
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetReqQosUmts));

struct OPAR_CFW_GprsGetReqQosUmts
{
    uint32_t result;
    CFW_GPRS_QOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetReqQosUmts));

#define TAG_CFW_GprsGetPdpAddr 0x86a85765
struct IPAR_CFW_GprsGetPdpAddr
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetPdpAddr));

struct OPAR_CFW_GprsGetPdpAddr
{
    uint32_t result;
    uint8_t nLength;
    uint8_t pPdpAdd[THE_PDP_ADDR_MAX_LEN];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetPdpAddr));

#define TAG_CFW_GprsGetPdpIpv4Addr 0xbccda50b
struct IPAR_CFW_GprsGetPdpIpv4Addr
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetPdpIpv4Addr));

struct OPAR_CFW_GprsGetPdpIpv4Addr
{
    uint32_t result;
    uint8_t nLength;
    uint8_t ipv4Addr[THE_PDP_ADDR_MAX_LEN];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetPdpIpv4Addr));

#define TAG_CFW_GprsGetPdpIpv6Addr 0x498f813d
struct IPAR_CFW_GprsGetPdpIpv6Addr
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetPdpIpv6Addr));

struct OPAR_CFW_GprsGetPdpIpv6Addr
{
    uint32_t result;
    uint8_t nLength;
    uint8_t ipv6Addr[THE_PDP_ADDR_MAX_LEN];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetPdpIpv6Addr));

#define TAG_CFW_GprsGetstatus 0x6d2f4987
struct IPAR_CFW_GprsGetstatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetstatus));

struct OPAR_CFW_GprsGetstatus
{
    uint32_t result;
    CFW_NW_STATUS_INFO pStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetstatus));

#define TAG_CFW_GprsSetstatus 0x6cd777a5
struct IPAR_CFW_GprsSetstatus
{
    CFW_NW_STATUS_INFO pStatus;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetstatus));

struct OPAR_CFW_GprsSetstatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetstatus));

#define TAG_CFW_GetGprsAttState 0xe07bd895
struct IPAR_CFW_GetGprsAttState
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGprsAttState));

struct OPAR_CFW_GetGprsAttState
{
    uint32_t result;
    uint8_t pState;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGprsAttState));

#define TAG_CFW_GetGprsActState 0x0a671c97
struct IPAR_CFW_GetGprsActState
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGprsActState));

struct OPAR_CFW_GetGprsActState
{
    uint32_t result;
    uint8_t pState;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGprsActState));

#define TAG_CFW_GprsAct 0x63af5d12
struct IPAR_CFW_GprsAct
{
    uint8_t nState;
    uint8_t nCid;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsAct));

struct OPAR_CFW_GprsAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsAct));

#define TAG_CFW_GprsActEx 0x76a3cadf
struct IPAR_CFW_GprsActEx
{
    uint8_t nState;
    uint8_t nCid;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
    bool SavedInApp;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsActEx));

struct OPAR_CFW_GprsActEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsActEx));

#define TAG_CFW_AttDetach 0xc1988543
struct IPAR_CFW_AttDetach
{
    uint8_t nState;
    uint16_t nUTI;
    uint8_t AttDetachType;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_AttDetach));

struct OPAR_CFW_AttDetach
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_AttDetach));

#define TAG_CFW_GprsSecPdpAct 0x761ecef8
struct IPAR_CFW_GprsSecPdpAct
{
    uint8_t nState;
    uint8_t nCid;
    uint8_t nPCid;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSecPdpAct));

struct OPAR_CFW_GprsSecPdpAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSecPdpAct));

#define TAG_CFW_GprsSetPsm 0xb70c6d65
struct IPAR_CFW_GprsSetPsm
{
    uint8_t psmEnable;
    uint8_t bitmap;
    uint8_t t3324;
    uint8_t t3412;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetPsm));

struct OPAR_CFW_GprsSetPsm
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetPsm));

#define TAG_CFW_GprsSetEdrx 0x255ef262
struct IPAR_CFW_GprsSetEdrx
{
    uint8_t edrxEnable;
    uint8_t edrxType;
    uint8_t edrxValue;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetEdrx));

struct OPAR_CFW_GprsSetEdrx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetEdrx));

#define TAG_CFW_GprsSetCtxTFT 0x79d77a2b
struct IPAR_CFW_GprsSetCtxTFT
{
    uint8_t nCid;
    CFW_TFT_SET pTft;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetCtxTFT));

struct OPAR_CFW_GprsSetCtxTFT
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetCtxTFT));

#define TAG_CFW_GprsSetCtxEQos 0x22a1f04c
struct IPAR_CFW_GprsSetCtxEQos
{
    uint8_t nCid;
    CFW_EQOS pQos;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetCtxEQos));

struct OPAR_CFW_GprsSetCtxEQos
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetCtxEQos));

#define TAG_CFW_GprsGetCtxEQos 0xea5153a8
struct IPAR_CFW_GprsGetCtxEQos
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetCtxEQos));

struct OPAR_CFW_GprsGetCtxEQos
{
    uint32_t result;
    CFW_EQOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetCtxEQos));

#define TAG_CFW_GprsGetNetEQos 0xf7523c18
struct IPAR_CFW_GprsGetNetEQos
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetNetEQos));

struct OPAR_CFW_GprsGetNetEQos
{
    uint32_t result;
    CFW_EQOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetNetEQos));

#define TAG_CFW_GprsCtxEpsModify 0x526ea981
struct IPAR_CFW_GprsCtxEpsModify
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxEpsModify));

struct OPAR_CFW_GprsCtxEpsModify
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxEpsModify));

#define TAG_CFW_GprsManualAcc 0x1514c385
struct IPAR_CFW_GprsManualAcc
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsManualAcc));

struct OPAR_CFW_GprsManualAcc
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsManualAcc));

#define TAG_CFW_GprsManualRej 0xcc80601e
struct IPAR_CFW_GprsManualRej
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsManualRej));

struct OPAR_CFW_GprsManualRej
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsManualRej));

#define TAG_CFW_GprsCtxModify 0x8271116b
struct IPAR_CFW_GprsCtxModify
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_GPRS_QOS Qos;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxModify));

struct OPAR_CFW_GprsCtxModify
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxModify));

#define TAG_CFW_GprsCtxModifyAcc 0x7ab90486
struct IPAR_CFW_GprsCtxModifyAcc
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxModifyAcc));

struct OPAR_CFW_GprsCtxModifyAcc
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxModifyAcc));

#define TAG_CFW_GprsCtxModifyRej 0x75d9a654
struct IPAR_CFW_GprsCtxModifyRej
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxModifyRej));

struct OPAR_CFW_GprsCtxModifyRej
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxModifyRej));

#define TAG_CFW_GprsSendData 0xa66b1aa9
struct IPAR_CFW_GprsSendData
{
    uint8_t nCid;
    uint8_t rai;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSendData));

struct OPAR_CFW_GprsSendData
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSendData));

#define TAG_CFW_GprsSendDataV2 0x1776f7b9
struct IPAR_CFW_GprsSendDataV2
{
    uint8_t nSim;
    uint8_t nCid;
    uint16_t nDataLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSendDataV2));

struct OPAR_CFW_GprsSendDataV2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSendDataV2));

#define TAG_CFW_GprsAtt 0xe4cc5064
struct IPAR_CFW_GprsAtt
{
    uint8_t nState;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsAtt));

struct OPAR_CFW_GprsAtt
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsAtt));

#define TAG_CFW_GetGprsSum 0x105b539b
struct IPAR_CFW_GetGprsSum
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGprsSum));

struct OPAR_CFW_GetGprsSum
{
    uint32_t result;
    int32_t upsum;
    int32_t downsum;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGprsSum));

#define TAG_CFW_ClearGprsSum 0x25e3db36
struct IPAR_CFW_ClearGprsSum
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ClearGprsSum));

struct OPAR_CFW_ClearGprsSum
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ClearGprsSum));

#define TAG_CFW_CheckTestSim 0xea05e9c2
struct IPAR_CFW_CheckTestSim
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CheckTestSim));

struct OPAR_CFW_CheckTestSim
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CheckTestSim));

#define TAG_CFW_GprsSetSmsSeviceMode 0xd21e74d0
struct IPAR_CFW_GprsSetSmsSeviceMode
{
    uint8_t nService;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetSmsSeviceMode));

struct OPAR_CFW_GprsSetSmsSeviceMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetSmsSeviceMode));

#define TAG_CFW_GprsGetSmsSeviceMode 0xea3e6cae
struct OPAR_CFW_GprsGetSmsSeviceMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetSmsSeviceMode));

#define TAG_CFW_SetDnsServerbyPdp 0x27543604
struct IPAR_CFW_SetDnsServerbyPdp
{
    uint8_t nCid;
    uint8_t nSimID;
    uint8_t Pro_DnsIp[4];
    uint8_t Sec_DnsIp[4];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetDnsServerbyPdp));

struct OPAR_CFW_SetDnsServerbyPdp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetDnsServerbyPdp));

#define TAG_CFW_SetDnsServerbyPdpV2 0x443c5edf
struct IPAR_CFW_SetDnsServerbyPdpV2
{
    uint8_t nCid;
    uint8_t nSimID;
    uint8_t Pro_DnsIp[16];
    uint8_t Sec_DnsIp[16];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetDnsServerbyPdpV2));

struct OPAR_CFW_SetDnsServerbyPdpV2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetDnsServerbyPdpV2));

#define TAG_CFW_getDnsServerbyPdp 0x0425d22f
struct IPAR_CFW_getDnsServerbyPdp
{
    uint8_t nCid;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_getDnsServerbyPdp));

struct OPAR_CFW_getDnsServerbyPdp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_getDnsServerbyPdp));

#define TAG_getDNSServer 0x6f20505a
struct IPAR_getDNSServer
{
    uint8_t nCid;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_getDNSServer));

struct OPAR_getDNSServer
{
    uint32_t* result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_getDNSServer));

#define TAG_CFW_EmodGetBatteryInfo 0x1a469219
struct OPAR_CFW_EmodGetBatteryInfo
{
    bool result;
    CFW_EMOD_BATTERY_INFO pBatInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetBatteryInfo));

#define TAG_CFW_EmodAudioTestStart 0x995d38e0
struct IPAR_CFW_EmodAudioTestStart
{
    uint8_t type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodAudioTestStart));

#define TAG_CFW_EmodAudioTestEnd 0xae454ab0
#define TAG_CFW_EmodClearUserInfo 0x63df1884
struct OPAR_CFW_EmodClearUserInfo
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodClearUserInfo));

#define TAG_CFW_EmodGetIMEI 0x75bf0c54
struct IPAR_CFW_EmodGetIMEI
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetIMEI));

struct OPAR_CFW_EmodGetIMEI
{
    uint8_t pImei[16];
    uint8_t pImeiLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetIMEI));

#define TAG_CFW_EmodSaveIMEI 0x2c611403
struct IPAR_CFW_EmodSaveIMEI
{
    uint8_t pImeiLen;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSaveIMEI));

#define TAG_CFW_EmodGetNvVersion 0x0a1a9480
struct OPAR_CFW_EmodGetNvVersion
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetNvVersion));

#define TAG_CFW_EmodGetBaseBandVersionV1 0x76b6efdf
struct OPAR_CFW_EmodGetBaseBandVersionV1
{
    uint8_t * result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetBaseBandVersionV1));

#define TAG_CFW_EmodGetNvData 0x9912b42b
struct IPAR_CFW_EmodGetNvData
{
    uint8_t dataType;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetNvData));

struct OPAR_CFW_EmodGetNvData
{
    uint8_t * result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetNvData));

#define TAG_CFW_EmodSetNvData 0x727f5ca5
struct IPAR_CFW_EmodSetNvData
{
    uint8_t dataType;
    uint16_t offset;
    uint16_t dataLength;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetNvData));

struct OPAR_CFW_EmodSetNvData
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSetNvData));

#define TAG_CFW_EmodSaveNvData 0x152320c3
struct IPAR_CFW_EmodSaveNvData
{
    uint8_t dataType;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSaveNvData));

struct OPAR_CFW_EmodSaveNvData
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSaveNvData));

#define TAG_CFW_EmodSpecialNvProcess 0x2994f59c
struct IPAR_CFW_EmodSpecialNvProcess
{
    uint8_t operType;
    uint8_t dataType;
    uint16_t offset;
    uint16_t dataLength;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSpecialNvProcess));

struct OPAR_CFW_EmodSpecialNvProcess
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSpecialNvProcess));

#define TAG_CFW_EmodEngineerCmd 0x0bb7f08e
struct IPAR_CFW_EmodEngineerCmd
{
    uint32_t module;
    uint32_t cmd;
    uint32_t v1;
    uint32_t v2;
    uint32_t v3;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodEngineerCmd));

struct OPAR_CFW_EmodEngineerCmd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodEngineerCmd));

#define TAG_CFW_EmodReadBindSIM 0x82ccd1ad
struct OPAR_CFW_EmodReadBindSIM
{
    CFW_EMOD_READ_BIND_SIM pReadBindSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodReadBindSIM));

#define TAG_CFW_EmodReadBindNW 0xa6eed8f0
struct OPAR_CFW_EmodReadBindNW
{
    CFW_EMOD_READ_BIND_NW pReadBindNw;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodReadBindNW));

#define TAG_CFW_EmodGetBindInfo 0x5eb87c1b
struct OPAR_CFW_EmodGetBindInfo
{
    bool bSim;
    bool bNetWork;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetBindInfo));

#define TAG_CFW_EmodSetBindInfo 0x44bf3c87
struct IPAR_CFW_EmodSetBindInfo
{
    bool bSim;
    bool bNetWork;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetBindInfo));

struct OPAR_CFW_EmodSetBindInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSetBindInfo));

#define TAG_CFW_EmodUpdateSimBind 0xf6a7246e
struct IPAR_CFW_EmodUpdateSimBind
{
    uint8_t nIndex;
    uint8_t nLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodUpdateSimBind));

struct OPAR_CFW_EmodUpdateSimBind
{
    CFW_EMOD_UPDATE_RETURN result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodUpdateSimBind));

#define TAG_CFW_EmodUpdateNwBind 0x1bcf7121
struct IPAR_CFW_EmodUpdateNwBind
{
    uint8_t nIndex;
    uint8_t nLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodUpdateNwBind));

struct OPAR_CFW_EmodUpdateNwBind
{
    CFW_EMOD_UPDATE_RETURN result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodUpdateNwBind));

#define TAG_CFW_EmodGetProductInfo 0x6b0b2278
struct OPAR_CFW_EmodGetProductInfo
{
    CFW_EMOD_LOCT_PRODUCT_INFO pProduct;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetProductInfo));

#define TAG_CFW_EmodGetRFCalibrationInfo 0xcbb6d554
struct OPAR_CFW_EmodGetRFCalibrationInfo
{
    CFW_EMOD_RF_CALIB_INFO pRfCalib;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetRFCalibrationInfo));

#define TAG_CFW_EmodOutfieldTestStart 0x972830bc
struct IPAR_CFW_EmodOutfieldTestStart
{
    CFW_TSM_FUNCTION_SELECT pSelecFUN;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodOutfieldTestStart));

struct OPAR_CFW_EmodOutfieldTestStart
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodOutfieldTestStart));

#define TAG_CFW_EmodOutfieldTestStartV2 0x7a7b7369
struct IPAR_CFW_EmodOutfieldTestStartV2
{
    CFW_TSM_FUNCTION_SELECT pSelecFUN;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodOutfieldTestStartV2));

struct OPAR_CFW_EmodOutfieldTestStartV2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodOutfieldTestStartV2));

#define TAG_CFW_EmodOutfieldTestEnd 0xffea09ab
struct IPAR_CFW_EmodOutfieldTestEnd
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodOutfieldTestEnd));

struct OPAR_CFW_EmodOutfieldTestEnd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodOutfieldTestEnd));

#define TAG_CFW_GetCellInfo 0xc361faa4
struct IPAR_CFW_GetCellInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetCellInfo));

struct OPAR_CFW_GetCellInfo
{
    uint32_t result;
    CFW_TSM_CURR_CELL_INFO pCurrCellInfo;
    CFW_TSM_ALL_NEBCELL_INFO pNeighborCellInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetCellInfo));

#define TAG_CFW_GetCellInfoV2 0x6a47af00
struct IPAR_CFW_GetCellInfoV2
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetCellInfoV2));

struct OPAR_CFW_GetCellInfoV2
{
    uint32_t result;
    CFW_TSM_CURR_CELL_INFO_V2 pCurrCellInfo;
    CFW_TSM_ALL_NEBCELL_INFO_V2 pNeighborCellInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetCellInfoV2));

#define TAG_CFW_EmodSyncInfoTest 0x471bc7fc
struct IPAR_CFW_EmodSyncInfoTest
{
    bool bStart;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSyncInfoTest));

struct OPAR_CFW_EmodSyncInfoTest
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSyncInfoTest));

#define TAG_CFW_GetFreeCID 0x0a402eac
struct IPAR_CFW_GetFreeCID
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetFreeCID));

struct OPAR_CFW_GetFreeCID
{
    uint32_t result;
    uint8_t pCID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetFreeCID));

#define TAG_CFW_ReleaseCID 0xf1dcf009
struct IPAR_CFW_ReleaseCID
{
    uint8_t nCID;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ReleaseCID));

struct OPAR_CFW_ReleaseCID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ReleaseCID));

#define TAG_CFW_SetCID 0xc921d725
struct IPAR_CFW_SetCID
{
    uint8_t nCID;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetCID));

struct OPAR_CFW_SetCID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetCID));

#define TAG_CFW_isPdpActiveAvailable 0x641e4ad6
struct IPAR_CFW_isPdpActiveAvailable
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_isPdpActiveAvailable));

struct OPAR_CFW_isPdpActiveAvailable
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_isPdpActiveAvailable));

#define TAG_cfw_SimReadElementaryFile 0xf15912cb
struct IPAR_cfw_SimReadElementaryFile
{
    uint16_t nUTI;
    uint32_t fileId;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_cfw_SimReadElementaryFile));

struct OPAR_cfw_SimReadElementaryFile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_cfw_SimReadElementaryFile));

#define TAG_CFW_SimGetACMMax 0x5447da9e
struct IPAR_CFW_SimGetACMMax
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetACMMax));

struct OPAR_CFW_SimGetACMMax
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetACMMax));

#define TAG_CFW_SimSetACMMax 0x1b686aea
struct IPAR_CFW_SimSetACMMax
{
    uint32_t iACMMaxValue;
    uint8_t nPinSize;
    uint16_t Nuti;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetACMMax));

struct OPAR_CFW_SimSetACMMax
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetACMMax));

#define TAG_CFW_SimGetACM 0xba3d62fe
struct IPAR_CFW_SimGetACM
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetACM));

struct OPAR_CFW_SimGetACM
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetACM));

#define TAG_CFW_SimSetACM 0x95e61408
struct IPAR_CFW_SimSetACM
{
    uint32_t iCurValue;
    uint8_t nPinSize;
    uint16_t Nuti;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetACM));

struct OPAR_CFW_SimSetACM
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetACM));

#define TAG_CFW_SimGetPUCT 0xd1853aaf
struct IPAR_CFW_SimGetPUCT
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPUCT));

struct OPAR_CFW_SimGetPUCT
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPUCT));

#define TAG_CFW_SimSetPUCT 0xf85926dc
struct IPAR_CFW_SimSetPUCT
{
    CFW_SIM_PUCT_INFO pPUCT;
    uint8_t nPinSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetPUCT));

struct OPAR_CFW_SimSetPUCT
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetPUCT));

#define TAG_CFW_EmodSetLteLockFreq 0xee64a452
struct IPAR_CFW_EmodSetLteLockFreq
{
    uint8_t number;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetLteLockFreq));

#define TAG_CFW_EmodGetLteLockFreq 0xab7975c4
struct IPAR_CFW_EmodGetLteLockFreq
{
    uint32_t freq[9];
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetLteLockFreq));

struct OPAR_CFW_EmodGetLteLockFreq
{
    uint8_t number;
    uint32_t freq[9];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteLockFreq));

#define TAG_CFW_EmodSetLteLockBand 0x036f213c
struct IPAR_CFW_EmodSetLteLockBand
{
    uint8_t number;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetLteLockBand));

#define TAG_CFW_EmodGetLteLockBand 0x95cd0c5f
struct IPAR_CFW_EmodGetLteLockBand
{
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetLteLockBand));

struct OPAR_CFW_EmodGetLteLockBand
{
    uint8_t number;
    uint16_t band[20];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteLockBand));

#define TAG_CFW_EmodSetLteLockCell 0x66b3e0aa
struct IPAR_CFW_EmodSetLteLockCell
{
    uint16_t Cell;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetLteLockCell));

#define TAG_CFW_EmodGetLteLockCell 0x0ede542a
struct IPAR_CFW_EmodGetLteLockCell
{
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetLteLockCell));

struct OPAR_CFW_EmodGetLteLockCell
{
    uint16_t Cell;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteLockCell));

#define TAG_CFW_EmodSetLteLockBandV2 0x56e376ff
struct IPAR_CFW_EmodSetLteLockBandV2
{
    uint8_t number;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetLteLockBandV2));

#define TAG_CFW_GetBandFromFreq 0xd9707705
struct IPAR_CFW_GetBandFromFreq
{
    uint32_t freq;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetBandFromFreq));

struct OPAR_CFW_GetBandFromFreq
{
    uint16_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetBandFromFreq));

#define TAG_CFW_EmodGetLteLockBandV2 0x906a0cc4
struct IPAR_CFW_EmodGetLteLockBandV2
{
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetLteLockBandV2));

struct OPAR_CFW_EmodGetLteLockBandV2
{
    uint8_t number;
    uint16_t band[20];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteLockBandV2));

#define TAG_CFW_EmodGetLteLockTddOrFdd 0x654ba1cb
struct IPAR_CFW_EmodGetLteLockTddOrFdd
{
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetLteLockTddOrFdd));

struct OPAR_CFW_EmodGetLteLockTddOrFdd
{
    uint8_t tddfdd;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteLockTddOrFdd));

#define TAG_CFW_EmodSetLteLockTddOrFdd 0x20f88847
struct IPAR_CFW_EmodSetLteLockTddOrFdd
{
    uint8_t tddfdd;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetLteLockTddOrFdd));

#define TAG_CFW_EmodGetCEMode 0x6b687af8
struct IPAR_CFW_EmodGetCEMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetCEMode));

struct OPAR_CFW_EmodGetCEMode
{
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetCEMode));

#define TAG_CFW_EmodSetCEMode 0x6f36868c
struct IPAR_CFW_EmodSetCEMode
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetCEMode));

#define TAG_CFW_EmodGetVoicePrefer 0xccbe3de8
struct IPAR_CFW_EmodGetVoicePrefer
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetVoicePrefer));

struct OPAR_CFW_EmodGetVoicePrefer
{
    uint8_t pVoicePrefer;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetVoicePrefer));

#define TAG_CFW_EmodSetVoicePrefer 0xb9a37a07
struct IPAR_CFW_EmodSetVoicePrefer
{
    uint8_t nVoicePrefer;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetVoicePrefer));

#define TAG_CFW_SetPollingTimer 0x42aa254a
struct IPAR_CFW_SetPollingTimer
{
    uint8_t PollingInterval;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPollingTimer));

#define TAG_CFW_GetPollingTimer 0x6b497865
struct IPAR_CFW_GetPollingTimer
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetPollingTimer));

struct OPAR_CFW_GetPollingTimer
{
    uint8_t PollingInterval;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetPollingTimer));

#define TAG_CSW_SetAudioCodecCalibParam 0x8855397e
struct IPAR_CSW_SetAudioCodecCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioCodecCalibParam));

struct OPAR_CSW_SetAudioCodecCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioCodecCalibParam));

#define TAG_CSW_SetAudioZspVqeCalibParam 0x03d69255
struct IPAR_CSW_SetAudioZspVqeCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspVqeCalibParam));

struct OPAR_CSW_SetAudioZspVqeCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspVqeCalibParam));

#define TAG_CSW_SetAudioZspPreProcessCalibParam 0x2ebfacc0
struct IPAR_CSW_SetAudioZspPreProcessCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspPreProcessCalibParam));

struct OPAR_CSW_SetAudioZspPreProcessCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspPreProcessCalibParam));

#define TAG_CSW_SetAudioZspPostProcessCalibParam 0xbc760df9
struct IPAR_CSW_SetAudioZspPostProcessCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspPostProcessCalibParam));

struct OPAR_CSW_SetAudioZspPostProcessCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspPostProcessCalibParam));

#define TAG_CSW_WriteCalibParamToFlash 0x07600c35
struct IPAR_CSW_WriteCalibParamToFlash
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_WriteCalibParamToFlash));

struct OPAR_CSW_WriteCalibParamToFlash
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_WriteCalibParamToFlash));

#define TAG_CSW_ExportCalibOrImportCalibFlashParam 0x725848a9
struct IPAR_CSW_ExportCalibOrImportCalibFlashParam
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nOffset;
    uint16_t nlength;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_ExportCalibOrImportCalibFlashParam));

struct OPAR_CSW_ExportCalibOrImportCalibFlashParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_ExportCalibOrImportCalibFlashParam));

#define TAG_CSW_DumpPcmDataToTflash 0x1f016809
struct IPAR_CSW_DumpPcmDataToTflash
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_DumpPcmDataToTflash));

struct OPAR_CSW_DumpPcmDataToTflash
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_DumpPcmDataToTflash));

#define TAG_CSW_VersionControl 0x717536f2
struct IPAR_CSW_VersionControl
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_VersionControl));

struct OPAR_CSW_VersionControl
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_VersionControl));

#define TAG_CSW_SetAudioZspNxpCalibParam 0xec6140b2
struct IPAR_CSW_SetAudioZspNxpCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspNxpCalibParam));

struct OPAR_CSW_SetAudioZspNxpCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspNxpCalibParam));

#define TAG_CFW_NwSetFTA 0xdcd3a5a9
struct IPAR_CFW_NwSetFTA
{
    uint8_t nFta;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetFTA));

struct OPAR_CFW_NwSetFTA
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetFTA));

#define TAG_CFW_NwSetnSecurityUsedFlag 0xfbb6e127
struct IPAR_CFW_NwSetnSecurityUsedFlag
{
    uint8_t nSecurityUsedFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetnSecurityUsedFlag));

struct OPAR_CFW_NwSetnSecurityUsedFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetnSecurityUsedFlag));

#define TAG_CFW_GprsIpDnsInfo 0x88c37c19
struct IPAR_CFW_GprsIpDnsInfo
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsIpDnsInfo));

struct OPAR_CFW_GprsIpDnsInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsIpDnsInfo));

#define TAG_CFW_NwSetBlackWhiteList 0xd17ca50b
struct IPAR_CFW_NwSetBlackWhiteList
{
    CFW_BLACK_WHITE_LIST pBlackWhiteList;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetBlackWhiteList));

struct OPAR_CFW_NwSetBlackWhiteList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetBlackWhiteList));

#define TAG_CFW_GetSimAdnId 0x1bbe3c37
struct IPAR_CFW_GetSimAdnId
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimAdnId));

struct OPAR_CFW_GetSimAdnId
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimAdnId));

#define TAG_CFW_GprsClrIpDns 0xeb30b3e0
struct OPAR_CFW_GprsClrIpDns
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsClrIpDns));

#define TAG_CFW_GetICCID 0x7b5f3507
struct IPAR_CFW_GetICCID
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetICCID));

struct OPAR_CFW_GetICCID
{
    uint8_t * result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetICCID));

#define TAG_hal_WriteGsmCalib 0x81c945ab
#define TAG_hal_WriteLteCalib 0xf6723896
#define TAG_CFW_CcCallMultipartyFunction 0x59557ac7
struct IPAR_CFW_CcCallMultipartyFunction
{
    uint8_t nFun;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcCallMultipartyFunction));

struct OPAR_CFW_CcCallMultipartyFunction
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcCallMultipartyFunction));

#define TAG_CFW_SimReadVoiceMailInfo 0xcb66a059
struct IPAR_CFW_SimReadVoiceMailInfo
{
    uint8_t nLineId;
    CFW_VOICE_MAIL_TYPE_E nType;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadVoiceMailInfo));

struct OPAR_CFW_SimReadVoiceMailInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadVoiceMailInfo));

#define TAG_CFW_SimUpdateVoiceMailInfo 0x1a63f26a
struct IPAR_CFW_SimUpdateVoiceMailInfo
{
    uint8_t nLineId;
    CFW_VOICE_MAIL_TYPE_E nType;
    CFW_SIM_INFO_VOICEMAIL pVoiceMail;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimUpdateVoiceMailInfo));

struct OPAR_CFW_SimUpdateVoiceMailInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimUpdateVoiceMailInfo));

#define TAG_CFW_CbSetContext 0xef062af4
struct IPAR_CFW_CbSetContext
{
    uint8_t CtxData[sizeof(CB_CTX_T)];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbSetContext));

struct OPAR_CFW_CbSetContext
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbSetContext));

#define TAG_CFW_CbGetContext 0x733f8641
struct IPAR_CFW_CbGetContext
{
    uint8_t CtxData[sizeof(CB_CTX_T)];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbGetContext));

struct OPAR_CFW_CbGetContext
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbGetContext));

#define TAG_CFW_CbWriteContext 0x997a2ae4
struct IPAR_CFW_CbWriteContext
{
    CB_CTX_T CtxData;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbWriteContext));

struct OPAR_CFW_CbWriteContext
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbWriteContext));

#define TAG_CFW_CbReadContext 0x6ebf94e1
struct IPAR_CFW_CbReadContext
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbReadContext));

struct OPAR_CFW_CbReadContext
{
    uint32_t result;
    CB_CTX_T CtxData;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbReadContext));

#define TAG_CFW_SetPsDataAttribute 0x84beca5c
struct IPAR_CFW_SetPsDataAttribute
{
    CFW_DATA_ATTRIBUTE_T pAttribute;
    uint8_t rai;
    uint8_t exception_data;
    uint16_t exempted_service_type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPsDataAttribute));

#define TAG_CFW_CbSetType 0x04f77c3d
struct IPAR_CFW_CbSetType
{
    uint8_t nCbType;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbSetType));

struct OPAR_CFW_CbSetType
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbSetType));

#define TAG_CFW_CbAct 0xd02d8d7a
struct IPAR_CFW_CbAct
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbAct));

struct OPAR_CFW_CbAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbAct));

#define TAG_CFW_CbDeAct 0x7d7969f5
struct IPAR_CFW_CbDeAct
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbDeAct));

struct OPAR_CFW_CbDeAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbDeAct));

#define TAG_CFW_SetPowerOffFlag 0xd5d4a325
struct IPAR_CFW_SetPowerOffFlag
{
    uint8_t bOff;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPowerOffFlag));

struct OPAR_CFW_SetPowerOffFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetPowerOffFlag));

#define TAG_CFW_NwSysConfig 0x3e65984a
struct IPAR_CFW_NwSysConfig
{
    uint8_t nMode;
    uint8_t nAcqorder;
    uint8_t nRoam;
    uint8_t nSrvdomain;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSysConfig));

struct OPAR_CFW_NwSysConfig
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSysConfig));

#define TAG_CFW_NwGetSysConfig 0xcfe5dd7b
struct IPAR_CFW_NwGetSysConfig
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetSysConfig));

struct OPAR_CFW_NwGetSysConfig
{
    uint32_t result;
    uint8_t pMode;
    uint8_t pAcqorder;
    uint8_t pRoam;
    uint8_t pSrvdomain;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetSysConfig));

#define TAG_CFW_SimGetSmsParameters 0xa6965bff
struct IPAR_CFW_SimGetSmsParameters
{
    uint8_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetSmsParameters));

struct OPAR_CFW_SimGetSmsParameters
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetSmsParameters));

#define TAG_CFW_SimSetSmsParameters 0xd0b065ed
struct IPAR_CFW_SimSetSmsParameters
{
    CFW_SIM_SMS_PARAMETERS pSmsParamArray;
    uint8_t nArrayCount;
    uint8_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetSmsParameters));

struct OPAR_CFW_SimSetSmsParameters
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetSmsParameters));

#define TAG_CFW_SimDeleteMessage 0xf4b2c147
struct IPAR_CFW_SimDeleteMessage
{
    uint16_t nLocation;
    uint16_t nIndex;
    uint16_t nStatus;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimDeleteMessage));

struct OPAR_CFW_SimDeleteMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimDeleteMessage));

#define TAG_CFW_EmodSpLteDummyPara 0x2689cabc
struct IPAR_CFW_EmodSpLteDummyPara
{
    CFW_EmodSpLteDummyPara_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSpLteDummyPara));

struct OPAR_CFW_EmodSpLteDummyPara
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSpLteDummyPara));

#define TAG_CFW_EmodRrtmParam 0xef959240
struct IPAR_CFW_EmodRrtmParam
{
    CFW_EmodRrtmParam_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodRrtmParam));

struct OPAR_CFW_EmodRrtmParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodRrtmParam));

#define TAG_CFW_EmodGrrLteFreq 0xb3b37874
struct IPAR_CFW_EmodGrrLteFreq
{
    CFW_EmodGrrLteFreq_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGrrLteFreq));

struct OPAR_CFW_EmodGrrLteFreq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGrrLteFreq));

#define TAG_CFW_EmodRrdmParam 0x307d94f2
struct IPAR_CFW_EmodRrdmParam
{
    CFW_EmodRrdmParam_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodRrdmParam));

struct OPAR_CFW_EmodRrdmParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodRrdmParam));

#define TAG_CFW_EmodL1Param 0xc69c0723
struct IPAR_CFW_EmodL1Param
{
    CFW_EmodL1Param_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodL1Param));

struct OPAR_CFW_EmodL1Param
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodL1Param));

#define TAG_CFW_EmodSpCleanInfo 0x21813e4a
struct IPAR_CFW_EmodSpCleanInfo
{
    CFW_EmodSpCleanInfo_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSpCleanInfo));

struct OPAR_CFW_EmodSpCleanInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSpCleanInfo));

#define TAG_CFW_EmodGetGsmCalibInfo 0xe8f8e512
struct OPAR_CFW_EmodGetGsmCalibInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetGsmCalibInfo));

#define TAG_CFW_EmodGetLteCalibInfo 0x5c3b6a0e
struct OPAR_CFW_EmodGetLteCalibInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteCalibInfo));

#define TAG_CFW_NwSetXCAP 0xfd41ebd7
struct IPAR_CFW_NwSetXCAP
{
    uint32_t nLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetXCAP));

struct OPAR_CFW_NwSetXCAP
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetXCAP));

#define TAG_CFW_NwSetBSF 0xfb1a71d8
struct IPAR_CFW_NwSetBSF
{
    uint32_t nLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetBSF));

struct OPAR_CFW_NwSetBSF
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetBSF));

#define TAG_CFW_EmodSetTracePort 0xb17252a7
struct IPAR_CFW_EmodSetTracePort
{
    uint8_t port;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetTracePort));

#define TAG_CFW_EmodGetTracePort 0xd942df90
struct OPAR_CFW_EmodGetTracePort
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetTracePort));

#define TAG_CFW_EmodSetTraceFlag 0xf90dd102
struct IPAR_CFW_EmodSetTraceFlag
{
    uint8_t bStop;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetTraceFlag));

#define TAG_CFW_EmodGetTraceFlag 0x7658ccd8
struct OPAR_CFW_EmodGetTraceFlag
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetTraceFlag));

#define TAG_CFW_ImsSrvccCapSet 0x8fdc72bf
struct IPAR_CFW_ImsSrvccCapSet
{
    uint8_t bSupport;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSrvccCapSet));

struct OPAR_CFW_ImsSrvccCapSet
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSrvccCapSet));

#define TAG_CFW_ImsGetSrvccCap 0x2f976c7e
struct IPAR_CFW_ImsGetSrvccCap
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsGetSrvccCap));

struct OPAR_CFW_ImsGetSrvccCap
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsGetSrvccCap));

#define TAG_CFW_SetSmsSeviceMode 0x18df9276
struct IPAR_CFW_SetSmsSeviceMode
{
    uint8_t service;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSmsSeviceMode));

struct OPAR_CFW_SetSmsSeviceMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetSmsSeviceMode));

#define TAG_CFW_GetSmsSeviceMode 0x46e369b5
struct OPAR_CFW_GetSmsSeviceMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSmsSeviceMode));

#define TAG_TM_KillAlarm 0x1538f990
struct IPAR_TM_KillAlarm
{
    uint8_t nIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_TM_KillAlarm));

struct OPAR_TM_KillAlarm
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_TM_KillAlarm));

#define TAG_TM_ListAlarm 0x5e72c269
struct OPAR_TM_ListAlarm
{
    bool result;
    uint8_t pArrayCnt;
    TM_ALARM * pAlarmArray;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_TM_ListAlarm));

#define TAG_CFW_NwChangeRat 0x96567f1f
struct IPAR_CFW_NwChangeRat
{
    uint8_t nRat;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwChangeRat));

struct OPAR_CFW_NwChangeRat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwChangeRat));

#define TAG_CFW_GetNetinfo 0x01a48c19
struct IPAR_CFW_GetNetinfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNetinfo));

struct OPAR_CFW_GetNetinfo
{
    uint32_t result;
    CFW_NET_INFO_T pNetinfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNetinfo));

#define TAG_CFW_GetLETRealTimeInfo 0x66010143
struct IPAR_CFW_GetLETRealTimeInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLETRealTimeInfo));

struct OPAR_CFW_GetLETRealTimeInfo
{
    uint32_t result;
    CFW_NET_INFO_T pNetinfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLETRealTimeInfo));

#define TAG_CFW_GetLETRealTimeInfoV2 0x7ab1b708
struct IPAR_CFW_GetLETRealTimeInfoV2
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLETRealTimeInfoV2));

struct OPAR_CFW_GetLETRealTimeInfoV2
{
    uint32_t result;
    CFW_NET_INFO_T_V2 pNetinfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLETRealTimeInfoV2));

#define TAG_CFW_GetNetinfoStatistic 0xdf39cd5c
struct IPAR_CFW_GetNetinfoStatistic
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNetinfoStatistic));

struct OPAR_CFW_GetNetinfoStatistic
{
    uint32_t result;
    CFW_NET_INFO_STATISTIC_T pNetinfoStatistic;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNetinfoStatistic));

#define TAG_CFW_SetInitWithSim 0x9a24d26a
struct IPAR_CFW_SetInitWithSim
{
    bool bWith;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetInitWithSim));

#define TAG_CFW_GetSimSPN 0xbdd83a6a
struct IPAR_CFW_GetSimSPN
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimSPN));

struct OPAR_CFW_GetSimSPN
{
    uint32_t result;
    CFW_SPN_INFO pSPNInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimSPN));

#define TAG_CFW_CfgSetAntenna 0x8dae8ade
struct IPAR_CFW_CfgSetAntenna
{
    uint16_t len;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetAntenna));

struct OPAR_CFW_CfgSetAntenna
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetAntenna));

#define TAG_CFW_ImsEnVolte 0x17c0fa00
struct IPAR_CFW_ImsEnVolte
{
    uint8_t bEn;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsEnVolte));

struct OPAR_CFW_ImsEnVolte
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsEnVolte));

#define TAG_CFW_SwitchUSBShareNetwork 0x08761de0
struct IPAR_CFW_SwitchUSBShareNetwork
{
    bool flag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SwitchUSBShareNetwork));

struct OPAR_CFW_SwitchUSBShareNetwork
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SwitchUSBShareNetwork));

#define TAG_CFW_NwGetQualReport 0x8291ead7
struct IPAR_CFW_NwGetQualReport
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetQualReport));

struct OPAR_CFW_NwGetQualReport
{
    uint32_t result;
    CFW_NW_QUAL_INFO pQualReport;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetQualReport));

#define TAG_CFW_nvGetPsmEnable 0x5190aaa0
struct IPAR_CFW_nvGetPsmEnable
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetPsmEnable));

struct OPAR_CFW_nvGetPsmEnable
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetPsmEnable));

#define TAG_CFW_nvGetPsmT3412 0x1d1f91db
struct IPAR_CFW_nvGetPsmT3412
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetPsmT3412));

struct OPAR_CFW_nvGetPsmT3412
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetPsmT3412));

#define TAG_CFW_nvGetPsmT3324 0x2d84f034
struct IPAR_CFW_nvGetPsmT3324
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetPsmT3324));

struct OPAR_CFW_nvGetPsmT3324
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetPsmT3324));

#define TAG_CFW_nvSetPsmEnable 0x099903fe
struct IPAR_CFW_nvSetPsmEnable
{
    uint8_t psmEnable;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetPsmEnable));

#define TAG_CFW_nvSetPsmT3412 0x07648919
struct IPAR_CFW_nvSetPsmT3412
{
    uint8_t psmEnable;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetPsmT3412));

#define TAG_CFW_nvSetPsmT3324 0x8eb3f586
struct IPAR_CFW_nvSetPsmT3324
{
    uint8_t psmEnable;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetPsmT3324));

#define TAG_CFW_nvGetEdrxEnable 0x8efa91ee
struct IPAR_CFW_nvGetEdrxEnable
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetEdrxEnable));

struct OPAR_CFW_nvGetEdrxEnable
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetEdrxEnable));

#define TAG_CFW_nvGetEdrxValue 0x324c8613
struct IPAR_CFW_nvGetEdrxValue
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetEdrxValue));

struct OPAR_CFW_nvGetEdrxValue
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetEdrxValue));

#define TAG_CFW_nvGetEdrxPtw 0x8e6db5f0
struct IPAR_CFW_nvGetEdrxPtw
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetEdrxPtw));

struct OPAR_CFW_nvGetEdrxPtw
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetEdrxPtw));

#define TAG_CFW_nvSetEdrxEnable 0x2ecd031c
struct IPAR_CFW_nvSetEdrxEnable
{
    uint8_t psmEnable;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetEdrxEnable));

#define TAG_CFW_nvSetEdrxValue 0x24577c87
struct IPAR_CFW_nvSetEdrxValue
{
    uint8_t psmEnable;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetEdrxValue));

#define TAG_CFW_nvSetEdrxPtw 0xf9f90144
struct IPAR_CFW_nvSetEdrxPtw
{
    uint8_t psmEnable;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetEdrxPtw));

#define TAG_CFW_SimGetStatus 0xbdaa958e
struct IPAR_CFW_SimGetStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetStatus));

struct OPAR_CFW_SimGetStatus
{
    CFW_SIMSTATUS result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetStatus));

#define TAG_CFW_nvSetCiotNonip 0xd4f02abd
struct IPAR_CFW_nvSetCiotNonip
{
    uint8_t nonip;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetCiotNonip));

#define TAG_CFW_nvSetCiotCpciot 0x31593bac
struct IPAR_CFW_nvSetCiotCpciot
{
    uint8_t cpciot;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetCiotCpciot));

#define TAG_CFW_nvSetCiotUpciot 0x9189bba7
struct IPAR_CFW_nvSetCiotUpciot
{
    uint8_t upciot;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetCiotUpciot));

#define TAG_CFW_nvSetCiotErwopdn 0x2fd93b53
struct IPAR_CFW_nvSetCiotErwopdn
{
    uint8_t erwopdn;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetCiotErwopdn));

#define TAG_CFW_nvSetCiotSmsWoCombAtt 0x778928d7
struct IPAR_CFW_nvSetCiotSmsWoCombAtt
{
    uint8_t sms_wo_comb_att;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetCiotSmsWoCombAtt));

#define TAG_CFW_nvSetCiotApnRateCtrl 0x120546f4
struct IPAR_CFW_nvSetCiotApnRateCtrl
{
    uint8_t apn_rate_control;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetCiotApnRateCtrl));

#define TAG_CFW_nvmWriteStatic 0x0a517e38
struct IPAR_CFW_nvmWriteStatic
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvmWriteStatic));

#define TAG_CFW_nvGetCiotNonip 0x472b9c60
struct IPAR_CFW_nvGetCiotNonip
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetCiotNonip));

struct OPAR_CFW_nvGetCiotNonip
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetCiotNonip));

#define TAG_CFW_nvGetCiotCpciot 0x5be63aff
struct IPAR_CFW_nvGetCiotCpciot
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetCiotCpciot));

struct OPAR_CFW_nvGetCiotCpciot
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetCiotCpciot));

#define TAG_CFW_nvGetCiotUpciot 0x85fad279
struct IPAR_CFW_nvGetCiotUpciot
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetCiotUpciot));

struct OPAR_CFW_nvGetCiotUpciot
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetCiotUpciot));

#define TAG_CFW_nvGetCiotErwopdn 0x8b872a87
struct IPAR_CFW_nvGetCiotErwopdn
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetCiotErwopdn));

struct OPAR_CFW_nvGetCiotErwopdn
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetCiotErwopdn));

#define TAG_CFW_nvGetCiotSmsWoCombAtt 0xe2f19f4e
struct IPAR_CFW_nvGetCiotSmsWoCombAtt
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetCiotSmsWoCombAtt));

struct OPAR_CFW_nvGetCiotSmsWoCombAtt
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetCiotSmsWoCombAtt));

#define TAG_CFW_nvGetCiotApnRateCtrl 0x8bd8eef9
struct IPAR_CFW_nvGetCiotApnRateCtrl
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetCiotApnRateCtrl));

struct OPAR_CFW_nvGetCiotApnRateCtrl
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetCiotApnRateCtrl));

#define TAG_CFW_SimReadFiles 0x2a7cf76d
struct IPAR_CFW_SimReadFiles
{
    uint8_t nSimFile;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadFiles));

struct OPAR_CFW_SimReadFiles
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadFiles));

#define TAG_CFW_SimGetOperatorName 0xa2528a6c
struct IPAR_CFW_SimGetOperatorName
{
    uint8_t pOperatorId[6];
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetOperatorName));

struct OPAR_CFW_SimGetOperatorName
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetOperatorName));

#define TAG_CFW_NwSetLockBCCH 0x8f8129ef
struct IPAR_CFW_NwSetLockBCCH
{
    bool bLock;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetLockBCCH));

struct OPAR_CFW_NwSetLockBCCH
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetLockBCCH));

#define TAG_CFW_NwGetLockBCCH 0x9052629e
struct OPAR_CFW_NwGetLockBCCH
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetLockBCCH));

#define TAG_CFW_SatRefreshFileChange 0xf95de818
struct IPAR_CFW_SatRefreshFileChange
{
    CFW_SAT_REFRESH_FILE_CHG_REQ_T FileChangeReq;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatRefreshFileChange));

struct OPAR_CFW_SatRefreshFileChange
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatRefreshFileChange));

#define TAG_CFW_SatRefreshImsiChange 0xe0991e80
struct IPAR_CFW_SatRefreshImsiChange
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatRefreshImsiChange));

struct OPAR_CFW_SatRefreshImsiChange
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatRefreshImsiChange));

#define TAG_CFW_SSCheckTestSim 0x0c87edd1
struct IPAR_CFW_SSCheckTestSim
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SSCheckTestSim));

struct OPAR_CFW_SSCheckTestSim
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SSCheckTestSim));

#define TAG_CFW_CcAcceptSpeechCallEx 0x9a1144fe
struct IPAR_CFW_CcAcceptSpeechCallEx
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcAcceptSpeechCallEx));

struct OPAR_CFW_CcAcceptSpeechCallEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcAcceptSpeechCallEx));

#define TAG_CFW_SmsMtSmsPPAckReq 0x0e138a39
struct IPAR_CFW_SmsMtSmsPPAckReq
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMtSmsPPAckReq));

struct OPAR_CFW_SmsMtSmsPPAckReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMtSmsPPAckReq));

#define TAG_CFW_SmsMtSmsPPAckReqWithData 0xbb4b1efb
struct IPAR_CFW_SmsMtSmsPPAckReqWithData
{
    uint16_t length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMtSmsPPAckReqWithData));

struct OPAR_CFW_SmsMtSmsPPAckReqWithData
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMtSmsPPAckReqWithData));

#define TAG_CFW_SimReset 0x7e6dec45
struct IPAR_CFW_SimReset
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReset));

struct OPAR_CFW_SimReset
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReset));

#define TAG_CFW_SimWakeup 0x85fc0cc2
struct IPAR_CFW_SimWakeup
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
    bool flag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimWakeup));

struct OPAR_CFW_SimWakeup
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimWakeup));

#define TAG_CFW_SimCloseEx 0x946ce97f
struct IPAR_CFW_SimCloseEx
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
    bool flag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimCloseEx));

struct OPAR_CFW_SimCloseEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimCloseEx));

#define TAG_CFW_SmsMemoryFullReq 0xd7655a97
struct IPAR_CFW_SmsMemoryFullReq
{
    uint8_t nCause;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMemoryFullReq));

struct OPAR_CFW_SmsMemoryFullReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMemoryFullReq));

#define TAG_CFW_SmsMMAReq 0x9ae2bc1a
struct IPAR_CFW_SmsMMAReq
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMMAReq));

struct OPAR_CFW_SmsMMAReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMMAReq));

#define TAG_CFW_GprsSendDataAvaliable 0xf5f97017
struct IPAR_CFW_GprsSendDataAvaliable
{
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSendDataAvaliable));

struct OPAR_CFW_GprsSendDataAvaliable
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSendDataAvaliable));

#define TAG_CFW_NwGetFTA 0xc37f54c0
struct IPAR_CFW_NwGetFTA
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetFTA));

struct OPAR_CFW_NwGetFTA
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetFTA));

#define TAG_CFW_NwSetAutoAttachFlag 0x8beea828
struct IPAR_CFW_NwSetAutoAttachFlag
{
    uint8_t flag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetAutoAttachFlag));

struct OPAR_CFW_NwSetAutoAttachFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetAutoAttachFlag));

#define TAG_CFW_NwGetAutoAttachFlag 0x7a4fe641
struct IPAR_CFW_NwGetAutoAttachFlag
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetAutoAttachFlag));

struct OPAR_CFW_NwGetAutoAttachFlag
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetAutoAttachFlag));

#define TAG_CFW_GetStatusInfo 0x8ca9e9f1
struct IPAR_CFW_GetStatusInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetStatusInfo));

struct OPAR_CFW_GetStatusInfo
{
    CFW_TUE_STATUS_INFO nStatusInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetStatusInfo));

#define TAG_CFW_GetScanRssiInfo 0xd908dbba
struct IPAR_CFW_GetScanRssiInfo
{
    uint8_t band;
    uint8_t index;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetScanRssiInfo));

struct OPAR_CFW_GetScanRssiInfo
{
    bool result;
    uint16_t pFreqsNums;
    int16_t rssi[400];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetScanRssiInfo));

#define TAG_CFW_GprsSetInternalPdpCtx 0x14aedc7b
struct IPAR_CFW_GprsSetInternalPdpCtx
{
    CFW_INTERNAL_PDP_CTX_TYPE nType;
    CFW_GPRS_PDPCONT_INFO_V2 pPdpCont;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetInternalPdpCtx));

struct OPAR_CFW_GprsSetInternalPdpCtx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetInternalPdpCtx));

#define TAG_CFW_GprsGetInternalPdpCtx 0xa09ae088
struct IPAR_CFW_GprsGetInternalPdpCtx
{
    CFW_INTERNAL_PDP_CTX_TYPE nType;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetInternalPdpCtx));

struct OPAR_CFW_GprsGetInternalPdpCtx
{
    CFW_GPRS_PDPCONT_INFO_V2* result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetInternalPdpCtx));

#define TAG_CFW_EmodImsNvParamSet 0x2f55a10e
struct IPAR_CFW_EmodImsNvParamSet
{
    uint16_t param_id;
    uint16_t value_len;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodImsNvParamSet));

struct OPAR_CFW_EmodImsNvParamSet
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodImsNvParamSet));

#define TAG_CFW_EmodImsNvParamGet 0xa0c22862
struct IPAR_CFW_EmodImsNvParamGet
{
    uint16_t param_id;
    uint8_t value_ptr[256];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodImsNvParamGet));

struct OPAR_CFW_EmodImsNvParamGet
{
    uint32_t result;
    uint8_t value_ptr[256];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodImsNvParamGet));

#define TAG_CFW_ImsSetUaVersionInfo 0xd182b1b4
struct IPAR_CFW_ImsSetUaVersionInfo
{
    uint8_t nLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetUaVersionInfo));

#define TAG_CFW_SaveSsimFileInfo 0x4dfb73ad
struct IPAR_CFW_SaveSsimFileInfo
{
    CFW_SSIM_FILE_INFO_T ssim_file_info;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SaveSsimFileInfo));

struct OPAR_CFW_SaveSsimFileInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SaveSsimFileInfo));

#define TAG_CFW_EmodResetImsDelta 0x931bd91a
struct OPAR_CFW_EmodResetImsDelta
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodResetImsDelta));

#define TAG_CFW_CcEmcDialEx_V2 0x80243b3f
struct IPAR_CFW_CcEmcDialEx_V2
{
    uint8_t nDialNumberSize;
    CFW_CcEmergCateg emergCateg;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcEmcDialEx_V2));

struct OPAR_CFW_CcEmcDialEx_V2
{
    uint32_t result;
    uint8_t pIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcEmcDialEx_V2));

#define TAG_CFW_SimDeleteMessage_V2 0x91e34f6a
struct IPAR_CFW_SimDeleteMessage_V2
{
    uint16_t nLocation;
    uint16_t nTotal;
    uint16_t nIndex;
    uint16_t nStatus;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimDeleteMessage_V2));

struct OPAR_CFW_SimDeleteMessage_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimDeleteMessage_V2));

#define TAG_CFW_GenerateSoftSimImsi 0x9defd872
struct IPAR_CFW_GenerateSoftSimImsi
{
    uint8_t nPlmn[7];
    uint8_t nImei[16];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GenerateSoftSimImsi));

struct OPAR_CFW_GenerateSoftSimImsi
{
    uint32_t result;
    uint8_t nImsi[16];
    uint8_t nKey[33];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GenerateSoftSimImsi));

#define TAG_CFW_SetGcfTest 0x994da05e
struct IPAR_CFW_SetGcfTest
{
    CFW_SIM_ID nSimID;
    bool nFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetGcfTest));

struct OPAR_CFW_SetGcfTest
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetGcfTest));

#define TAG_CFW_SmsGetSmsType 0x87cb3392
struct IPAR_CFW_SmsGetSmsType
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsGetSmsType));

struct OPAR_CFW_SmsGetSmsType
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsGetSmsType));

#define TAG_CFW_SwitchPort 0x2d9c14db
struct IPAR_CFW_SwitchPort
{
    uint8_t nFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SwitchPort));

struct OPAR_CFW_SwitchPort
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SwitchPort));

#define TAG_CFW_SimSelectApplication 0x44499329
struct IPAR_CFW_SimSelectApplication
{
    uint8_t length;
    uint8_t channel;
    uint16_t nUIT;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSelectApplication));

struct OPAR_CFW_SimSelectApplication
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSelectApplication));

#define TAG_CFW_SimManageChannel 0x01d08e7c
struct IPAR_CFW_SimManageChannel
{
    uint8_t command;
    uint8_t channel;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimManageChannel));

struct OPAR_CFW_SimManageChannel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimManageChannel));

#define TAG_CFW_SimTPDUCommand 0x99d7b65b
struct IPAR_CFW_SimTPDUCommand
{
    uint16_t length;
    uint8_t channel;
    uint16_t nUIT;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimTPDUCommand));

struct OPAR_CFW_SimTPDUCommand
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimTPDUCommand));

#define TAG_CFW_GetRFTemperature 0x0a9305e1
struct OPAR_CFW_GetRFTemperature
{
    uint32_t result;
    uint32_t temp;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetRFTemperature));

#define TAG_CFW_SetLTEFreqPwrRange 0x8a98fad2
struct IPAR_CFW_SetLTEFreqPwrRange
{
    uint16_t freqlow;
    uint16_t freqhigh;
    uint16_t power;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetLTEFreqPwrRange));

struct OPAR_CFW_SetLTEFreqPwrRange
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetLTEFreqPwrRange));

#define TAG_CFW_SetRFFreqPwrRange 0x3f237e1f
struct IPAR_CFW_SetRFFreqPwrRange
{
    uint16_t mode;
    uint16_t band;
    uint16_t powerlow;
    uint16_t powerhigh;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetRFFreqPwrRange));

struct OPAR_CFW_SetRFFreqPwrRange
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetRFFreqPwrRange));

#define TAG_CFW_NwSetBlackList 0xaf486c1a
struct IPAR_CFW_NwSetBlackList
{
    CFW_BlackCellList nBlackCellList;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetBlackList));

struct OPAR_CFW_NwSetBlackList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetBlackList));

#define TAG_CFW_NwSetBlackList_V2 0x34ae5835
struct IPAR_CFW_NwSetBlackList_V2
{
    CFW_BlackCellList_V2 nBlackCellList;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetBlackList_V2));

struct OPAR_CFW_NwSetBlackList_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetBlackList_V2));

#define TAG_CFW_nvGetBlackListEnable 0x6a9c01e4
struct IPAR_CFW_nvGetBlackListEnable
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetBlackListEnable));

struct OPAR_CFW_nvGetBlackListEnable
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetBlackListEnable));

#define TAG_CFW_nvSetBlackListEnable 0x495f144a
struct IPAR_CFW_nvSetBlackListEnable
{
    uint8_t BlacklistEnable;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetBlackListEnable));

struct OPAR_CFW_nvSetBlackListEnable
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvSetBlackListEnable));

#define TAG_CFW_SetPdnActTimerAndMaxCount 0x01daf8a8
struct IPAR_CFW_SetPdnActTimerAndMaxCount
{
    CFW_PDN_TIMER_MAXCOUNT_INFO pdnTimerAndMaxCount;
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPdnActTimerAndMaxCount));

struct OPAR_CFW_SetPdnActTimerAndMaxCount
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetPdnActTimerAndMaxCount));

#define TAG_CFW_GetPdnActTimerAndMaxCount 0x7373b075
struct IPAR_CFW_GetPdnActTimerAndMaxCount
{
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetPdnActTimerAndMaxCount));

struct OPAR_CFW_GetPdnActTimerAndMaxCount
{
    uint32_t result;
    CFW_PDN_TIMER_MAXCOUNT_INFO pdnTimerAndMaxCount;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetPdnActTimerAndMaxCount));

#define TAG_CFW_SetPdnDeactTimerAndMaxCount 0x366e4680
struct IPAR_CFW_SetPdnDeactTimerAndMaxCount
{
    CFW_PDN_TIMER_MAXCOUNT_INFO pdnTimerAndMaxCount;
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPdnDeactTimerAndMaxCount));

struct OPAR_CFW_SetPdnDeactTimerAndMaxCount
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetPdnDeactTimerAndMaxCount));

#define TAG_CFW_GetPdnDeactTimerAndMaxCount 0x1a437b5b
struct IPAR_CFW_GetPdnDeactTimerAndMaxCount
{
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetPdnDeactTimerAndMaxCount));

struct OPAR_CFW_GetPdnDeactTimerAndMaxCount
{
    uint32_t result;
    CFW_PDN_TIMER_MAXCOUNT_INFO pdnTimerAndMaxCount;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetPdnDeactTimerAndMaxCount));

#define TAG_CFW_SetCSPagingFlag 0xf7676344
struct IPAR_CFW_SetCSPagingFlag
{
    bool nFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetCSPagingFlag));

struct OPAR_CFW_SetCSPagingFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetCSPagingFlag));

#define TAG_CFW_GetCSPagingFlag 0x3a238517
struct IPAR_CFW_GetCSPagingFlag
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetCSPagingFlag));

struct OPAR_CFW_GetCSPagingFlag
{
    uint32_t result;
    uint8_t nFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetCSPagingFlag));

#define TAG_CFW_SetSimFileUpdateCountMode 0x0e931937
struct IPAR_CFW_SetSimFileUpdateCountMode
{
    uint8_t op_mode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSimFileUpdateCountMode));

struct OPAR_CFW_SetSimFileUpdateCountMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetSimFileUpdateCountMode));

#define TAG_CFW_GetSimFileUpdateCount 0xf25dd62c
struct IPAR_CFW_GetSimFileUpdateCount
{
    uint8_t nFileID;
    uint8_t bReal;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimFileUpdateCount));

struct OPAR_CFW_GetSimFileUpdateCount
{
    uint16_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimFileUpdateCount));

#define TAG_CFW_GprsSendCtxCfg_V2 0x6b3761da
struct IPAR_CFW_GprsSendCtxCfg_V2
{
    CFW_GPRS_DFTPDN_INFO pDftPdn;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSendCtxCfg_V2));

struct OPAR_CFW_GprsSendCtxCfg_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSendCtxCfg_V2));

#define TAG_CFW_GetAoHandle 0x3a43271f
struct IPAR_CFW_GetAoHandle
{
    uint8_t nIndex;
    uint32_t nState;
    uint32_t nServiceId;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetAoHandle));

struct OPAR_CFW_GetAoHandle
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetAoHandle));

#define TAG_CFW_SendMtSmsAckPPError 0x204a3ca8
struct IPAR_CFW_SendMtSmsAckPPError
{
    uint8_t nCause;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SendMtSmsAckPPError));

struct OPAR_CFW_SendMtSmsAckPPError
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SendMtSmsAckPPError));

#define TAG_CFW_CfgSetNewSmsOptionMT 0x82b90fdb
struct IPAR_CFW_CfgSetNewSmsOptionMT
{
    uint8_t nOption;
    uint8_t nNewSmsStorage;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetNewSmsOptionMT));

struct OPAR_CFW_CfgSetNewSmsOptionMT
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetNewSmsOptionMT));

#define TAG_CFW_CfgGetNewSmsOptionMT 0xae86e23a
struct IPAR_CFW_CfgGetNewSmsOptionMT
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetNewSmsOptionMT));

struct OPAR_CFW_CfgGetNewSmsOptionMT
{
    uint32_t result;
    uint8_t nOption;
    uint8_t nNewSmsStorage;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetNewSmsOptionMT));

#define TAG_CFW_StartNstMode 0x1d286e43
struct IPAR_CFW_StartNstMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_StartNstMode));

struct OPAR_CFW_StartNstMode
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_StartNstMode));

#define TAG_CFW_StopNstMode 0x4a9d236e
struct IPAR_CFW_StopNstMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_StopNstMode));

struct OPAR_CFW_StopNstMode
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_StopNstMode));

#define TAG_CFW_NstConfig 0xfb2f1070
struct IPAR_CFW_NstConfig
{
    CFW_NW_NST_TX_PARA_CONFIG nst_para_config;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NstConfig));

struct OPAR_CFW_NstConfig
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NstConfig));

#define TAG_CFW_NstGetrssi 0xd892ac3c
struct OPAR_CFW_NstGetrssi
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NstGetrssi));

#define TAG_CFW_SetStaticScene 0x0ceef0da
struct IPAR_CFW_SetStaticScene
{
    uint8_t iScene;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetStaticScene));

struct OPAR_CFW_SetStaticScene
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetStaticScene));

#define TAG_CFW_GetStaticScene 0x3940f8aa
struct IPAR_CFW_GetStaticScene
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetStaticScene));

struct OPAR_CFW_GetStaticScene
{
    uint32_t result;
    uint8_t iScene;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetStaticScene));

#define TAG_CFW_SetRatePriority 0x33e9d7ee
struct IPAR_CFW_SetRatePriority
{
    uint8_t iPriority;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetRatePriority));

struct OPAR_CFW_SetRatePriority
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetRatePriority));

#define TAG_CFW_GetRatePriority 0x85f6ac6b
struct IPAR_CFW_GetRatePriority
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetRatePriority));

struct OPAR_CFW_GetRatePriority
{
    uint32_t result;
    uint8_t iPriority;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetRatePriority));

#define TAG_CFW_GprsGetDynamicAPN 0x51a0de23
struct IPAR_CFW_GprsGetDynamicAPN
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetDynamicAPN));

struct OPAR_CFW_GprsGetDynamicAPN
{
    uint32_t result;
    CFW_GPRS_DYNAMIC_APN_INFO pPdpCont;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetDynamicAPN));

#define TAG_CFW_GprsGetDynamicIP 0x16a17495
struct IPAR_CFW_GprsGetDynamicIP
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetDynamicIP));

struct OPAR_CFW_GprsGetDynamicIP
{
    uint32_t result;
    CFW_GPRS_DYNAMIC_IP_INFO pDyncIP;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetDynamicIP));

#define TAG_CFW_SetT3302 0xe6ce07c9
struct IPAR_CFW_SetT3302
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetT3302));

struct OPAR_CFW_SetT3302
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetT3302));

#define TAG_CFW_GetT3302 0xea2a7b41
struct IPAR_CFW_GetT3302
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetT3302));

struct OPAR_CFW_GetT3302
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetT3302));

#define TAG_CFW_SetRRCRel 0x7121b58d
struct IPAR_CFW_SetRRCRel
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetRRCRel));

struct OPAR_CFW_SetRRCRel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetRRCRel));

#define TAG_CFW_GetRRCRel 0x4ace1ca9
struct IPAR_CFW_GetRRCRel
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetRRCRel));

struct OPAR_CFW_GetRRCRel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetRRCRel));

#define TAG_CFW_SetRRCRelEx 0x8ed2d272
struct IPAR_CFW_SetRRCRelEx
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetRRCRelEx));

struct OPAR_CFW_SetRRCRelEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetRRCRelEx));

#define TAG_CFW_GetRRCRelEx 0x2c1cb7e6
struct IPAR_CFW_GetRRCRelEx
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetRRCRelEx));

struct OPAR_CFW_GetRRCRelEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetRRCRelEx));

#define TAG_CFW_GetDrxCyc 0x59500382
struct IPAR_CFW_GetDrxCyc
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetDrxCyc));

struct OPAR_CFW_GetDrxCyc
{
    uint16_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetDrxCyc));

#define TAG_CSW_SetAndGetMicGain 0xb872117e
struct IPAR_CSW_SetAndGetMicGain
{
    uint8_t mode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAndGetMicGain));

struct OPAR_CSW_SetAndGetMicGain
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[16];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAndGetMicGain));

#define TAG_CFW_SimGetMeProfile 0x574ae0f5
struct IPAR_CFW_SimGetMeProfile
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetMeProfile));

struct OPAR_CFW_SimGetMeProfile
{
    uint32_t result;
    CFW_PROFILE pMeProfile;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetMeProfile));

#define TAG_SimSelectApplicationReqV2 0xb2f6762f
struct IPAR_SimSelectApplicationReqV2
{
    uint8_t * AID;
    uint8_t AIDLength;
    uint8_t ChannelID;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_SimSelectApplicationReqV2));

struct OPAR_SimSelectApplicationReqV2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_SimSelectApplicationReqV2));

#define TAG_SimSelectApplicationReq 0x8a2c09bb
struct IPAR_SimSelectApplicationReq
{
    uint8_t * AID;
    uint8_t AIDLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_SimSelectApplicationReq));

struct OPAR_SimSelectApplicationReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_SimSelectApplicationReq));

#define TAG_CFW_InvalideTmsiPTmis 0xb30bafe9
struct IPAR_CFW_InvalideTmsiPTmis
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_InvalideTmsiPTmis));

#define TAG_CFW_SimInitStage1 0xc82ff59a
struct IPAR_CFW_SimInitStage1
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimInitStage1));

struct OPAR_CFW_SimInitStage1
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimInitStage1));

#define TAG_CFW_SimInitStage3 0xe600a85c
struct IPAR_CFW_SimInitStage3
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimInitStage3));

struct OPAR_CFW_SimInitStage3
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimInitStage3));

#define TAG_SimPollReq 0x0722f33b
struct IPAR_SimPollReq
{
    uint8_t nTimerOut;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_SimPollReq));

struct OPAR_SimPollReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_SimPollReq));

#define TAG_SimPollOffReq 0xc24a1cf0
struct IPAR_SimPollOffReq
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_SimPollOffReq));

struct OPAR_SimPollOffReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_SimPollOffReq));

#define TAG_CFW_GetUsimAID 0x972bf726
struct IPAR_CFW_GetUsimAID
{
    uint8_t length[1];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetUsimAID));

struct OPAR_CFW_GetUsimAID
{
    uint8_t aid[20];
    uint8_t length[1];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetUsimAID));

#define TAG_CFW_SatGetCurrentCmd 0x880f39b1
struct IPAR_CFW_SatGetCurrentCmd
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetCurrentCmd));

struct OPAR_CFW_SatGetCurrentCmd
{
    bool result;
    uint8_t nCmd;
    uint8_t nQualifier;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetCurrentCmd));

#define TAG_CFW_SatGetSetupEventList 0x28a20d81
struct IPAR_CFW_SatGetSetupEventList
{
    uint8_t pNum[1];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSetupEventList));

struct OPAR_CFW_SatGetSetupEventList
{
    uint8_t result;
    uint8_t pEventList[32];
    uint8_t pNum[1];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSetupEventList));

#define TAG_CFW_SatSetEventList 0x624e8997
struct IPAR_CFW_SatSetEventList
{
    uint32_t event_list;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatSetEventList));

#define TAG_CFW_SatGetEventList 0x79df0b80
struct IPAR_CFW_SatGetEventList
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetEventList));

struct OPAR_CFW_SatGetEventList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetEventList));

#define TAG_CFW_SatGetSendSmsData 0x359f7fd6
struct IPAR_CFW_SatGetSendSmsData
{
    uint8_t pDataLen[1];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendSmsData));

struct OPAR_CFW_SatGetSendSmsData
{
    bool result;
    uint8_t pPduData[255];
    uint8_t pDataLen[1];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendSmsData));

#define TAG_CFW_SatGetSendSmsSCInfo 0x0d2e78ff
struct IPAR_CFW_SatGetSendSmsSCInfo
{
    uint8_t pSCLen[1];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendSmsSCInfo));

struct OPAR_CFW_SatGetSendSmsSCInfo
{
    bool result;
    uint8_t pSCAddress[32];
    uint8_t pSCLen[1];
    uint8_t pType[1];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendSmsSCInfo));

#define TAG_CFW_SatGetRefreshFilesList 0xf559d063
struct IPAR_CFW_SatGetRefreshFilesList
{
    uint8_t pFileNum[1];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetRefreshFilesList));

struct OPAR_CFW_SatGetRefreshFilesList
{
    bool result;
    uint8_t pFilesList[32];
    uint8_t pFileNum[1];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetRefreshFilesList));

#define TAG_CFW_GetSATIndFormate 0x381dbef1
struct OPAR_CFW_GetSATIndFormate
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSATIndFormate));

#define TAG_CFW_SimRefreshFiles 0x32232a5c
struct IPAR_CFW_SimRefreshFiles
{
    uint8_t pFiles[32];
    uint8_t nFileNum;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimRefreshFiles));

struct OPAR_CFW_SimRefreshFiles
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimRefreshFiles));

#define TAG_CFW_SimGetLanguage 0xba963a67
struct IPAR_CFW_SimGetLanguage
{
    uint8_t pNum;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetLanguage));

struct OPAR_CFW_SimGetLanguage
{
    bool result;
    uint8_t pLanguage[8];
    uint8_t pNum;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetLanguage));

#define TAG_CFW_SatGetSetupIdleModeText 0x28f76343
struct IPAR_CFW_SatGetSetupIdleModeText
{
    uint8_t pLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSetupIdleModeText));

struct OPAR_CFW_SatGetSetupIdleModeText
{
    bool result;
    uint8_t pText[64];
    uint8_t pLength;
    uint8_t pScheme;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSetupIdleModeText));

#define TAG_CFW_SatGetPlayToneText 0xa7a14524
struct IPAR_CFW_SatGetPlayToneText
{
    uint8_t pLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetPlayToneText));

struct OPAR_CFW_SatGetPlayToneText
{
    bool result;
    uint8_t pText[64];
    uint8_t pLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetPlayToneText));

#define TAG_CFW_SatGetPlayToneDuration 0x53255770
struct IPAR_CFW_SatGetPlayToneDuration
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetPlayToneDuration));

struct OPAR_CFW_SatGetPlayToneDuration
{
    bool result;
    uint8_t pTone;
    uint8_t pUnit;
    uint8_t pInterval;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetPlayToneDuration));

#define TAG_CFW_SatGetPollIntervalDuration 0xdc6c3250
struct IPAR_CFW_SatGetPollIntervalDuration
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetPollIntervalDuration));

struct OPAR_CFW_SatGetPollIntervalDuration
{
    bool result;
    uint8_t pUnit;
    uint8_t pInterval;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetPollIntervalDuration));

#define TAG_CFW_SatGetTimerManagement 0x860bd0dd
struct IPAR_CFW_SatGetTimerManagement
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetTimerManagement));

struct OPAR_CFW_SatGetTimerManagement
{
    bool result;
    uint8_t pTimerID;
    uint8_t pHour;
    uint8_t pMinute;
    uint8_t pSecond;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetTimerManagement));

#define TAG_CFW_SatGetReceiveData 0x6fb9f6ef
struct IPAR_CFW_SatGetReceiveData
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetReceiveData));

struct OPAR_CFW_SatGetReceiveData
{
    bool result;
    uint8_t pChannelDataLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetReceiveData));

#define TAG_CFW_SatGetSendData 0x8ac9c0f6
struct IPAR_CFW_SatGetSendData
{
    uint8_t pDataLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendData));

struct OPAR_CFW_SatGetSendData
{
    bool result;
    uint8_t pData[255];
    uint8_t pDataLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendData));

#define TAG_CFW_SatGetOpenChannelUserInfo 0x43754e24
struct IPAR_CFW_SatGetOpenChannelUserInfo
{
    uint8_t pUserLen;
    uint8_t pPasswdLen;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetOpenChannelUserInfo));

struct OPAR_CFW_SatGetOpenChannelUserInfo
{
    bool result;
    uint8_t pUser[50];
    uint8_t pUserLen;
    uint8_t pPasswd[32];
    uint8_t pPasswdLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetOpenChannelUserInfo));

#define TAG_CFW_SatGetOpenChannelAPN 0x37aef5e0
struct IPAR_CFW_SatGetOpenChannelAPN
{
    uint8_t pApnLen;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetOpenChannelAPN));

struct OPAR_CFW_SatGetOpenChannelAPN
{
    bool result;
    uint8_t pAPN[50];
    uint8_t pApnLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetOpenChannelAPN));

#define TAG_CFW_SatGetOpenChannelDestAddress 0x944ddde9
struct IPAR_CFW_SatGetOpenChannelDestAddress
{
    uint8_t pAddressLen;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetOpenChannelDestAddress));

struct OPAR_CFW_SatGetOpenChannelDestAddress
{
    bool result;
    uint8_t pAddress[16];
    uint8_t pAddressLen;
    uint8_t pAddressType;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetOpenChannelDestAddress));

#define TAG_CFW_SatGetOpenChannelNetInfoV2 0xc0c96aaf
struct IPAR_CFW_SatGetOpenChannelNetInfoV2
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetOpenChannelNetInfoV2));

struct OPAR_CFW_SatGetOpenChannelNetInfoV2
{
    bool result;
    uint16_t pBufferSize;
    uint8_t pBearerType;
    uint8_t pTranType;
    uint16_t pTranPort;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetOpenChannelNetInfoV2));

#define TAG_CFW_SatGetOpenChannelNetInfo 0x814c3abc
struct IPAR_CFW_SatGetOpenChannelNetInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetOpenChannelNetInfo));

struct OPAR_CFW_SatGetOpenChannelNetInfo
{
    bool result;
    uint8_t pBufferSize;
    uint8_t pBearerType;
    uint8_t pTranType;
    uint8_t pTranPort;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetOpenChannelNetInfo));

#define TAG_CFW_GetEleFence 0x1c41c70e
struct IPAR_CFW_GetEleFence
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetEleFence));

struct OPAR_CFW_GetEleFence
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetEleFence));

#define TAG_CFW_SetEleFence 0x1a897db4
struct IPAR_CFW_SetEleFence
{
    uint8_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetEleFence));

struct OPAR_CFW_SetEleFence
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetEleFence));

#define TAG_CFW_GetEleFenceTimer 0x8f35ead7
struct IPAR_CFW_GetEleFenceTimer
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetEleFenceTimer));

struct OPAR_CFW_GetEleFenceTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetEleFenceTimer));

#define TAG_CFW_SetEleFenceTimer 0x22bee7d1
struct IPAR_CFW_SetEleFenceTimer
{
    uint8_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetEleFenceTimer));

struct OPAR_CFW_SetEleFenceTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetEleFenceTimer));

#define TAG_CFW_SimGetEID 0xc7bcf189
struct IPAR_CFW_SimGetEID
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetEID));

struct OPAR_CFW_SimGetEID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetEID));

#define TAG_CFW_SendMtSmsAckPPErrorWithData 0x37e910c9
struct IPAR_CFW_SendMtSmsAckPPErrorWithData
{
    uint8_t nCause;
    uint16_t length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SendMtSmsAckPPErrorWithData));

struct OPAR_CFW_SendMtSmsAckPPErrorWithData
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SendMtSmsAckPPErrorWithData));

#define TAG_CFW_GprsHostAddress 0x69e9af8d
struct IPAR_CFW_GprsHostAddress
{
    uint8_t nLength;
    uint8_t nIPType;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsHostAddress));

struct OPAR_CFW_GprsHostAddress
{
    uint32_t result;
    uint8_t pIPAddress[21];
    uint8_t nLength;
    uint8_t nIPType;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsHostAddress));

#define TAG_SimSendStatusReq 0x3b8046f0
struct IPAR_SimSendStatusReq
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_SimSendStatusReq));

struct OPAR_SimSendStatusReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_SimSendStatusReq));

#define TAG_Cfw_SetTimezone 0xf2ae4a97
struct IPAR_Cfw_SetTimezone
{
    int8_t timezone;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_Cfw_SetTimezone));

struct OPAR_Cfw_SetTimezone
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_Cfw_SetTimezone));

#define TAG_CFW_CcGetMptyNum 0x5be547bc
struct IPAR_CFW_CcGetMptyNum
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcGetMptyNum));

struct OPAR_CFW_CcGetMptyNum
{
    uint32_t result;
    uint8_t pMptyCnt;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcGetMptyNum));

#define TAG_CFW_CcExtConfCallDial 0xe94b0133
struct IPAR_CFW_CcExtConfCallDial
{
    CFW_DIAL_EXT_CONF_CT pExtConfCxt;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcExtConfCallDial));

struct OPAR_CFW_CcExtConfCallDial
{
    uint32_t result;
    uint8_t pIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcExtConfCallDial));

#define TAG_CFW_CcExtConfCallAdd 0x10aa25e7
struct IPAR_CFW_CcExtConfCallAdd
{
    CFW_DIAL_EXT_CONF_CT pExtConfCxt;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcExtConfCallAdd));

struct OPAR_CFW_CcExtConfCallAdd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcExtConfCallAdd));

#define TAG_CFW_NwSetBlackListTimer 0x2b876dad
struct IPAR_CFW_NwSetBlackListTimer
{
    CFW_BlackListCntReq nBlackListCnt;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetBlackListTimer));

struct OPAR_CFW_NwSetBlackListTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetBlackListTimer));

#define TAG_CFW_SetPlmn 0x19a6b578
struct IPAR_CFW_SetPlmn
{
    CFW_NV_PLMN nNvPlmn;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPlmn));

struct OPAR_CFW_SetPlmn
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetPlmn));

#define TAG_CFW_GetPlmn 0xe803ca57
struct IPAR_CFW_GetPlmn
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetPlmn));

struct OPAR_CFW_GetPlmn
{
    uint32_t result;
    CFW_NV_PLMN nNvPlmn;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetPlmn));

#define TAG_CFW_SmsListFree 0x6791096e
struct IPAR_CFW_SmsListFree
{
    void * pListResult;
    uint8_t nCount;
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsListFree));

struct OPAR_CFW_SmsListFree
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsListFree));

#define TAG_CFW_CfgSetPocEnable 0xd934d4ab
struct IPAR_CFW_CfgSetPocEnable
{
    uint8_t bEnable;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPocEnable));

struct OPAR_CFW_CfgSetPocEnable
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPocEnable));

#define TAG_CFW_CfgGetPocEnable 0x723720a5
struct OPAR_CFW_CfgGetPocEnable
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPocEnable));

#define TAG_CFW_CfgSetEnhancePwrSweepNonCE 0x80909c55
struct IPAR_CFW_CfgSetEnhancePwrSweepNonCE
{
    uint8_t bEnable;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetEnhancePwrSweepNonCE));

struct OPAR_CFW_CfgSetEnhancePwrSweepNonCE
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetEnhancePwrSweepNonCE));

#define TAG_CFW_CfgGetEnhancePwrSweepNonCE 0xd9326b3e
struct IPAR_CFW_CfgGetEnhancePwrSweepNonCE
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetEnhancePwrSweepNonCE));

struct OPAR_CFW_CfgGetEnhancePwrSweepNonCE
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetEnhancePwrSweepNonCE));

#define TAG_CFW_NwSetSimSwich 0x4073e5e7
struct IPAR_CFW_NwSetSimSwich
{
    uint8_t bOnGoing;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetSimSwich));

struct OPAR_CFW_NwSetSimSwich
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetSimSwich));

#define TAG_CFW_NwGetSimSwitch 0xd97f8dc8
struct OPAR_CFW_NwGetSimSwitch
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetSimSwitch));

#define TAG_CFW_NwSetPocSimIndex 0xfcb71a0f
struct IPAR_CFW_NwSetPocSimIndex
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetPocSimIndex));

struct OPAR_CFW_NwSetPocSimIndex
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetPocSimIndex));

#define TAG_CFW_SetSignalQuatityFirst 0x94665baf
struct IPAR_CFW_SetSignalQuatityFirst
{
    uint32_t signalQuatityFirst;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSignalQuatityFirst));

struct OPAR_CFW_SetSignalQuatityFirst
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetSignalQuatityFirst));

#define TAG_CFW_GetSignalQuatityFirst 0xb73a2d50
struct IPAR_CFW_GetSignalQuatityFirst
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSignalQuatityFirst));

struct OPAR_CFW_GetSignalQuatityFirst
{
    uint32_t result;
    uint32_t signalQuatityFirst;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSignalQuatityFirst));

#define TAG_CFW_EnableLpsLock 0xe5455e3a
struct IPAR_CFW_EnableLpsLock
{
    bool nEnable;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EnableLpsLock));

struct OPAR_CFW_EnableLpsLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EnableLpsLock));

#define TAG_CFW_EmodSetPowerMode 0xa2faad2c
struct IPAR_CFW_EmodSetPowerMode
{
    CSCLK_PM_ENUM nValue;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetPowerMode));

struct OPAR_CFW_EmodSetPowerMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSetPowerMode));

#define TAG_CFW_UpdateNV 0x5c00f490
struct IPAR_CFW_UpdateNV
{
    bool bForce;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_UpdateNV));

struct OPAR_CFW_UpdateNV
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_UpdateNV));

#define TAG_CFW_SetNvUpdateFlag 0x87232781
struct IPAR_CFW_SetNvUpdateFlag
{
    uint32_t nFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetNvUpdateFlag));

struct OPAR_CFW_SetNvUpdateFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetNvUpdateFlag));

#define TAG_CFW_RpmNvGetDefaultPdnSettingApn 0x3496ac1d
struct IPAR_CFW_RpmNvGetDefaultPdnSettingApn
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmNvGetDefaultPdnSettingApn));

struct OPAR_CFW_RpmNvGetDefaultPdnSettingApn
{
    uint32_t result;
    uint8_t outputApn[30];
    uint8_t outApnLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmNvGetDefaultPdnSettingApn));

#define TAG_CFW_RpmCheckAllowRebootingPdpActiving 0xbf9e8a21
struct IPAR_CFW_RpmCheckAllowRebootingPdpActiving
{
    uint8_t apn[30];
    uint32_t apnLen;
    uint8_t trb;
    uint8_t addFx;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmCheckAllowRebootingPdpActiving));

struct OPAR_CFW_RpmCheckAllowRebootingPdpActiving
{
    uint32_t result;
    bool outFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmCheckAllowRebootingPdpActiving));

#define TAG_CFW_RpmAllowPdpActivating 0x2497d885
struct IPAR_CFW_RpmAllowPdpActivating
{
    uint8_t apn[30];
    uint32_t apnLen;
    uint8_t trb;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmAllowPdpActivating));

struct OPAR_CFW_RpmAllowPdpActivating
{
    uint32_t result;
    bool outFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmAllowPdpActivating));

#define TAG_CFW_RpmGetRpmFlag 0xba690c45
struct IPAR_CFW_RpmGetRpmFlag
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmFlag));

struct OPAR_CFW_RpmGetRpmFlag
{
    uint32_t result;
    uint8_t outFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmFlag));

#define TAG_CFW_RpmSetRpmFlag 0xdd4f1cb3
struct IPAR_CFW_RpmSetRpmFlag
{
    uint8_t rpmFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmSetRpmFlag));

struct OPAR_CFW_RpmSetRpmFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmSetRpmFlag));

#define TAG_CFW_RpmWriteStatic 0x5756a514
struct IPAR_CFW_RpmWriteStatic
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmWriteStatic));

struct OPAR_CFW_RpmWriteStatic
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmWriteStatic));

#define TAG_CFW_RpmWriteDynamic 0x226599f3
struct IPAR_CFW_RpmWriteDynamic
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmWriteDynamic));

struct OPAR_CFW_RpmWriteDynamic
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmWriteDynamic));

#define TAG_CFW_RpmWriteDynamicEx 0xd17755fb
struct IPAR_CFW_RpmWriteDynamicEx
{
    int32_t BeReBoot;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmWriteDynamicEx));

struct OPAR_CFW_RpmWriteDynamicEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmWriteDynamicEx));

#define TAG_CFW_RpmResetRpmFunction 0x2395da66
struct IPAR_CFW_RpmResetRpmFunction
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmResetRpmFunction));

struct OPAR_CFW_RpmResetRpmFunction
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmResetRpmFunction));

#define TAG_CFW_RpmGetRpmN1 0x9354d639
struct IPAR_CFW_RpmGetRpmN1
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmN1));

struct OPAR_CFW_RpmGetRpmN1
{
    uint32_t result;
    uint8_t rpmN1;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmN1));

#define TAG_CFW_RpmSetRpmN1 0xa9923452
struct IPAR_CFW_RpmSetRpmN1
{
    uint8_t rpmN1;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmSetRpmN1));

struct OPAR_CFW_RpmSetRpmN1
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmSetRpmN1));

#define TAG_CFW_RpmGetRpmT1 0xbd21ed6e
struct IPAR_CFW_RpmGetRpmT1
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmT1));

struct OPAR_CFW_RpmGetRpmT1
{
    uint32_t result;
    uint8_t rpmT1;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmT1));

#define TAG_CFW_RpmSetRpmT1 0xae5d7c49
struct IPAR_CFW_RpmSetRpmT1
{
    uint8_t rpmT1;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmSetRpmT1));

struct OPAR_CFW_RpmSetRpmT1
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmSetRpmT1));

#define TAG_CFW_RpmGetRpmFx 0x7527d1c4
struct IPAR_CFW_RpmGetRpmFx
{
    uint8_t fx;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmFx));

struct OPAR_CFW_RpmGetRpmFx
{
    uint32_t result;
    uint8_t rpmFx;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmFx));

#define TAG_CFW_RpmSetRpmFx 0xc26e014f
struct IPAR_CFW_RpmSetRpmFx
{
    uint8_t fx;
    uint8_t rpmValue;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmSetRpmFx));

struct OPAR_CFW_RpmSetRpmFx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmSetRpmFx));

#define TAG_CFW_RpmGetRpmLRX 0x86e9ee4d
struct IPAR_CFW_RpmGetRpmLRX
{
    uint8_t lrx;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmLRX));

struct OPAR_CFW_RpmGetRpmLRX
{
    uint32_t result;
    uint8_t rpmLRX;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmLRX));

#define TAG_CFW_RpmSetRpmLRX 0xa5d0fd53
struct IPAR_CFW_RpmSetRpmLRX
{
    uint8_t lrx;
    uint8_t rpmValue;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmSetRpmLRX));

struct OPAR_CFW_RpmSetRpmLRX
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmSetRpmLRX));

#define TAG_CFW_RpmGetRpmCBR1 0xab47683b
struct IPAR_CFW_RpmGetRpmCBR1
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmCBR1));

struct OPAR_CFW_RpmGetRpmCBR1
{
    uint32_t result;
    uint8_t rpmCBR1;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmCBR1));

#define TAG_CFW_RpmGetRpmCR1 0x641206e8
struct IPAR_CFW_RpmGetRpmCR1
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmCR1));

struct OPAR_CFW_RpmGetRpmCR1
{
    uint32_t result;
    uint8_t rpmCR1;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmCR1));

#define TAG_CFW_RpmSetRpmCR1 0xebd14092
struct IPAR_CFW_RpmSetRpmCR1
{
    uint8_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmSetRpmCR1));

struct OPAR_CFW_RpmSetRpmCR1
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmSetRpmCR1));

#define TAG_CFW_RpmGetRpmCpdpx 0x8a89e7a2
struct IPAR_CFW_RpmGetRpmCpdpx
{
    uint8_t apn[30];
    uint8_t apnLen;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_RpmGetRpmCpdpx));

struct OPAR_CFW_RpmGetRpmCpdpx
{
    uint32_t result;
    uint8_t cpdp1;
    uint8_t cpdp2;
    uint8_t cpdp3;
    uint8_t cpdp4;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_RpmGetRpmCpdpx));

#define TAG_CFW_rpmNvGetLastRat 0x5246b62b
struct IPAR_CFW_rpmNvGetLastRat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_rpmNvGetLastRat));

struct OPAR_CFW_rpmNvGetLastRat
{
    uint32_t result;
    uint8_t nLastRat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_rpmNvGetLastRat));

#define TAG_CFW_rpmNvSetLastRat 0xcb9c77c8
struct IPAR_CFW_rpmNvSetLastRat
{
    uint8_t rat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_rpmNvSetLastRat));

struct OPAR_CFW_rpmNvSetLastRat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_rpmNvSetLastRat));

#define TAG_CFW_SetRauAfterCallFlag 0xa228dde6
struct IPAR_CFW_SetRauAfterCallFlag
{
    uint16_t flag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetRauAfterCallFlag));

struct OPAR_CFW_SetRauAfterCallFlag
{
    uint16_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetRauAfterCallFlag));

#define TAG_CFW_GetRauAfterCallFlag 0xd74b689f
struct OPAR_CFW_GetRauAfterCallFlag
{
    uint16_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetRauAfterCallFlag));

#define TAG_CFW_EmodGetPocPara 0x6136a6fd
struct IPAR_CFW_EmodGetPocPara
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetPocPara));

struct OPAR_CFW_EmodGetPocPara
{
    uint32_t result;
    CFW_EMOD_POC_PARA_T pParam;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetPocPara));

#define TAG_CFW_EmodSetPocPara 0xaa43af92
struct IPAR_CFW_EmodSetPocPara
{
    CFW_EMOD_POC_PARA_T pParam;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetPocPara));

struct OPAR_CFW_EmodSetPocPara
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSetPocPara));

#define TAG_CFW_EmodSetPSMThd 0x9aca73b9
struct IPAR_CFW_EmodSetPSMThd
{
    uint32_t nMinute;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetPSMThd));

#define TAG_CFW_EmodGetPSMThd 0xb1320865
struct OPAR_CFW_EmodGetPSMThd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetPSMThd));

#define TAG_CFW_EmodGetLpsFlag 0xc4c58443
struct OPAR_CFW_EmodGetLpsFlag
{
    uint16_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLpsFlag));

#define TAG_CFW_SatGetOpenChannelBearerDes 0x4550c36f
struct IPAR_CFW_SatGetOpenChannelBearerDes
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetOpenChannelBearerDes));

struct OPAR_CFW_SatGetOpenChannelBearerDes
{
    bool result;
    CFW_GPRS_QOS qos;
    uint8_t pdp_type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetOpenChannelBearerDes));

#define TAG_CFW_SatGetChannelID 0x334b9df1
struct IPAR_CFW_SatGetChannelID
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetChannelID));

struct OPAR_CFW_SatGetChannelID
{
    bool result;
    uint8_t channel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetChannelID));

#define TAG_CFW_SimPollReq 0x434692a3
struct IPAR_CFW_SimPollReq
{
    uint32_t nTimerOut;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimPollReq));

struct OPAR_CFW_SimPollReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimPollReq));

#define TAG_CFW_SetLCTimerEx 0x3ddd4301
struct IPAR_CFW_SetLCTimerEx
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetLCTimerEx));

struct OPAR_CFW_SetLCTimerEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetLCTimerEx));

#define TAG_CFW_GetLCTimerEx 0x95e3238d
struct IPAR_CFW_GetLCTimerEx
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLCTimerEx));

struct OPAR_CFW_GetLCTimerEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLCTimerEx));

#define TAG_CFW_GetLimiteTimer 0x8148e1cb
struct IPAR_CFW_GetLimiteTimer
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLimiteTimer));

struct OPAR_CFW_GetLimiteTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLimiteTimer));

#define TAG_CFW_SetLimiteTimer 0xf853cc90
struct IPAR_CFW_SetLimiteTimer
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetLimiteTimer));

struct OPAR_CFW_SetLimiteTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetLimiteTimer));

#define TAG_CFW_GetLctLoopCout 0xdec27c03
struct IPAR_CFW_GetLctLoopCout
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLctLoopCout));

struct OPAR_CFW_GetLctLoopCout
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLctLoopCout));

#define TAG_CFW_SetLctLoopCout 0xb37d6dc0
struct IPAR_CFW_SetLctLoopCout
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetLctLoopCout));

struct OPAR_CFW_SetLctLoopCout
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetLctLoopCout));

#define TAG_CFW_GetLmtLoopCout 0xdb1c569e
struct IPAR_CFW_GetLmtLoopCout
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLmtLoopCout));

struct OPAR_CFW_GetLmtLoopCout
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLmtLoopCout));

#define TAG_CFW_SetLmtLoopCout 0xc0392db9
struct IPAR_CFW_SetLmtLoopCout
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetLmtLoopCout));

struct OPAR_CFW_SetLmtLoopCout
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetLmtLoopCout));

#define TAG_CFW_GetLctMaxLen 0x9bbb10e9
struct IPAR_CFW_GetLctMaxLen
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLctMaxLen));

struct OPAR_CFW_GetLctMaxLen
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLctMaxLen));

#define TAG_CFW_SetLctMaxLen 0xd639038c
struct IPAR_CFW_SetLctMaxLen
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetLctMaxLen));

struct OPAR_CFW_SetLctMaxLen
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetLctMaxLen));

#define TAG_CFW_GetLmtMaxLen 0x5a52fd76
struct IPAR_CFW_GetLmtMaxLen
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLmtMaxLen));

struct OPAR_CFW_GetLmtMaxLen
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLmtMaxLen));

#define TAG_CFW_SetLmtMaxLen 0xf236f32c
struct IPAR_CFW_SetLmtMaxLen
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetLmtMaxLen));

struct OPAR_CFW_SetLmtMaxLen
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetLmtMaxLen));

#define TAG_CFW_CfgSetCdrxNoSleepEnable 0x2e7a6514
struct IPAR_CFW_CfgSetCdrxNoSleepEnable
{
    uint8_t bEnable;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetCdrxNoSleepEnable));

struct OPAR_CFW_CfgSetCdrxNoSleepEnable
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetCdrxNoSleepEnable));

#define TAG_CFW_CfgGetCdrxNoSleepEnable 0x3baecbd5
struct IPAR_CFW_CfgGetCdrxNoSleepEnable
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetCdrxNoSleepEnable));

struct OPAR_CFW_CfgGetCdrxNoSleepEnable
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetCdrxNoSleepEnable));

#define TAG_CFW_CfgSetPeriodTauAftLocalRel 0x2a472b28
struct IPAR_CFW_CfgSetPeriodTauAftLocalRel
{
    uint8_t bEnable;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPeriodTauAftLocalRel));

struct OPAR_CFW_CfgSetPeriodTauAftLocalRel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPeriodTauAftLocalRel));

#define TAG_CFW_CfgGetPeriodTauAftLocalRel 0xff364c32
struct IPAR_CFW_CfgGetPeriodTauAftLocalRel
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetPeriodTauAftLocalRel));

struct OPAR_CFW_CfgGetPeriodTauAftLocalRel
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPeriodTauAftLocalRel));

#define TAG_CFW_WifiScanStart 0xc8d170b5
struct IPAR_CFW_WifiScanStart
{
    CFW_WIFISCAN_REQ wifiscan_param;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_WifiScanStart));

struct OPAR_CFW_WifiScanStart
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_WifiScanStart));

#define TAG_CFW_WifiScanStop 0x5dc8d617
struct IPAR_CFW_WifiScanStop
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_WifiScanStop));

struct OPAR_CFW_WifiScanStop
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_WifiScanStop));

#define TAG_CFW_GnssStartHandshakeStack 0x0dc49886
struct IPAR_CFW_GnssStartHandshakeStack
{
    uint8_t nServiceType;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GnssStartHandshakeStack));

struct OPAR_CFW_GnssStartHandshakeStack
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GnssStartHandshakeStack));

#define TAG_CFW_GnssStopHandshakeStack 0x2b6ef2ba
struct IPAR_CFW_GnssStopHandshakeStack
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GnssStopHandshakeStack));

struct OPAR_CFW_GnssStopHandshakeStack
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GnssStopHandshakeStack));

#define TAG_CFW_nvSetLocPriority 0xe9bd1df9
struct IPAR_CFW_nvSetLocPriority
{
    uint8_t locPriority;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetLocPriority));

#define TAG_CFW_nvSetLocPriority_V2 0x29628bd4
struct IPAR_CFW_nvSetLocPriority_V2
{
    uint8_t locPriority;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetLocPriority_V2));

struct OPAR_CFW_nvSetLocPriority_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvSetLocPriority_V2));

#define TAG_CFW_nvGetLocPriority 0x94701277
struct IPAR_CFW_nvGetLocPriority
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetLocPriority));

struct OPAR_CFW_nvGetLocPriority
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetLocPriority));

#define TAG_CFW_LocGetWifiscanRes 0x2533c918
struct IPAR_CFW_LocGetWifiscanRes
{
    void * listAddr;
    uint8_t index;
    uint8_t end;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_LocGetWifiscanRes));

struct OPAR_CFW_LocGetWifiscanRes
{
    uint32_t result;
    CFW_WIFISCAN_AP_RESULT scan_result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_LocGetWifiscanRes));

#define TAG_CFW_LcsMolrReq 0xe25ebe5c
struct IPAR_CFW_LcsMolrReq
{
    CFW_LCS_MOLR_INFO molrInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_LcsMolrReq));

struct OPAR_CFW_LcsMolrReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_LcsMolrReq));

#define TAG_CFW_LcsMtlrRsp 0xa19ed215
struct IPAR_CFW_LcsMtlrRsp
{
    uint8_t handle_id;
    uint8_t response;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_LcsMtlrRsp));

struct OPAR_CFW_LcsMtlrRsp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_LcsMtlrRsp));

#define TAG_CFW_WifiSentestOn 0x93880df6
#define TAG_CFW_WifiSentestOff 0x0f7dab89
#define TAG_CFW_WifiSentestSetParam 0x33663f69
struct IPAR_CFW_WifiSentestSetParam
{
    CFW_WIFISENTEST_PARAM sentest_cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_WifiSentestSetParam));

struct OPAR_CFW_WifiSentestSetParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_WifiSentestSetParam));

#define TAG_CFW_WifiSentestGetSingleRes 0xb30c96c5
struct OPAR_CFW_WifiSentestGetSingleRes
{
    uint32_t result;
    CFW_WIFISENTEST_RES sentest_res;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_WifiSentestGetSingleRes));

#define TAG_CFW_nvSetNasTimer 0xdc96ae6b
struct IPAR_CFW_nvSetNasTimer
{
    uint8_t nType;
    uint8_t nName;
    uint16_t nValue;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetNasTimer));

struct OPAR_CFW_nvSetNasTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvSetNasTimer));

#define TAG_CFW_nvGetNasTimer 0x23dada90
struct IPAR_CFW_nvGetNasTimer
{
    uint8_t nType;
    uint8_t nName;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetNasTimer));

struct OPAR_CFW_nvGetNasTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetNasTimer));

#define TAG_CFW_nvSetNwSearchPrior 0xab42d767
struct IPAR_CFW_nvSetNwSearchPrior
{
    uint8_t mode;
    uint32_t enable;
    uint8_t band_num;
    uint8_t band_list[20];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetNwSearchPrior));

struct OPAR_CFW_nvSetNwSearchPrior
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvSetNwSearchPrior));

#define TAG_CFW_nvGetNwSearchPrior 0xc916fa48
struct IPAR_CFW_nvGetNwSearchPrior
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvGetNwSearchPrior));

struct OPAR_CFW_nvGetNwSearchPrior
{
    uint32_t result;
    uint32_t enable;
    uint8_t band_num;
    uint8_t band_list[20];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetNwSearchPrior));

#define TAG_CFW_ErrcSetMeasThreshold 0x0abd51ef
struct IPAR_CFW_ErrcSetMeasThreshold
{
    CFW_MEAS_OFFSET_T meas_cfg;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ErrcSetMeasThreshold));

struct OPAR_CFW_ErrcSetMeasThreshold
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ErrcSetMeasThreshold));

#define TAG_CFW_ErrcGetMeasThreshold 0xb824518f
struct IPAR_CFW_ErrcGetMeasThreshold
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ErrcGetMeasThreshold));

struct OPAR_CFW_ErrcGetMeasThreshold
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ErrcGetMeasThreshold));

#define TAG_CFW_NwErrcStatisCfg 0xefd5858c
struct IPAR_CFW_NwErrcStatisCfg
{
    CFW_STATIS_INFO_CFG_RPT cfg_rpt;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwErrcStatisCfg));

struct OPAR_CFW_NwErrcStatisCfg
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwErrcStatisCfg));

#define TAG_CFW_SimSetResume 0xaa3206ac
struct IPAR_CFW_SimSetResume
{
    uint8_t nResumeToken[8];
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetResume));

struct OPAR_CFW_SimSetResume
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetResume));

#define TAG_CFW_SimSetSuspend 0xa6d772db
struct IPAR_CFW_SimSetSuspend
{
    CFW_SUSP_FUNC_SELECT pSelecFUN;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetSuspend));

struct OPAR_CFW_SimSetSuspend
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetSuspend));

#define TAG_CFW_NwSetLocalRelease 0xbf73f746
struct IPAR_CFW_NwSetLocalRelease
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetLocalRelease));

struct OPAR_CFW_NwSetLocalRelease
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetLocalRelease));

#define TAG_CFW_SatGetRawData 0xd1dc69f6
struct IPAR_CFW_SatGetRawData
{
    uint16_t nRawDataLen;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetRawData));

struct OPAR_CFW_SatGetRawData
{
    bool result;
    uint8_t pRawData[260];
    uint16_t nRawDataLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetRawData));

#define TAG_CFW_SatTerminalResponse 0xde092a0a
struct IPAR_CFW_SatTerminalResponse
{
    uint16_t length;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatTerminalResponse));

struct OPAR_CFW_SatTerminalResponse
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatTerminalResponse));

#define TAG_CFW_SatTerminalEnvelope 0x452041d8
struct IPAR_CFW_SatTerminalEnvelope
{
    uint16_t length;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatTerminalEnvelope));

struct OPAR_CFW_SatTerminalEnvelope
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatTerminalEnvelope));

#define TAG_CFW_SimSetMeProfile 0x932a095d
struct IPAR_CFW_SimSetMeProfile
{
    CFW_PROFILE pMeProfile;
    uint8_t uicc_type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetMeProfile));

struct OPAR_CFW_SimSetMeProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetMeProfile));

#define TAG_CFW_SatSetFacility 0x3e53e123
struct IPAR_CFW_SatSetFacility
{
    uint8_t status;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatSetFacility));

struct OPAR_CFW_SatSetFacility
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatSetFacility));

#define TAG_CFW_SatGetFacility 0x5fc9f415
struct IPAR_CFW_SatGetFacility
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetFacility));

struct OPAR_CFW_SatGetFacility
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetFacility));

#define TAG_CFW_StackInit 0x0c80c52d
struct IPAR_CFW_StackInit
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_StackInit));

struct OPAR_CFW_StackInit
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_StackInit));

#define TAG_CFW_SimlockDataProcess 0x88f002d6
struct IPAR_CFW_SimlockDataProcess
{
    uint16_t nDataSize;
    uint8_t nFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockDataProcess));

struct OPAR_CFW_SimlockDataProcess
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockDataProcess));

#define TAG_CFW_SimlockEncryptKeys 0xe6161356
struct IPAR_CFW_SimlockEncryptKeys
{
    uint16_t nDataSize;
    uint16_t en_len;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockEncryptKeys));

struct OPAR_CFW_SimlockEncryptKeys
{
    uint32_t result;
    char encrypt_data[800];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockEncryptKeys));

#define TAG_CFW_SimlockImeiProcess 0xcb17fa6c
struct IPAR_CFW_SimlockImeiProcess
{
    uint16_t nDataSize;
    uint8_t nFlag;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockImeiProcess));

struct OPAR_CFW_SimlockImeiProcess
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockImeiProcess));

#define TAG_CFW_SimlockSetHuk 0x9bd52aeb
struct IPAR_CFW_SimlockSetHuk
{
    uint16_t nDataSize;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockSetHuk));

struct OPAR_CFW_SimlockSetHuk
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockSetHuk));

#define TAG_CFW_SimlockFacauthPubkey 0xf5d63e1a
struct IPAR_CFW_SimlockFacauthPubkey
{
    uint16_t nPubkeySize;
    uint16_t nSignSize;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockFacauthPubkey));

struct OPAR_CFW_SimlockFacauthPubkey
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockFacauthPubkey));

#define TAG_CFW_SimlockAuthStart 0xc110d14b
struct IPAR_CFW_SimlockAuthStart
{
    uint32_t len;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockAuthStart));

struct OPAR_CFW_SimlockAuthStart
{
    uint32_t result;
    char data[512];
    uint32_t len;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockAuthStart));

#define TAG_CFW_SimlockAuthEnd 0xc6b42805
struct IPAR_CFW_SimlockAuthEnd
{
    uint32_t len;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockAuthEnd));

struct OPAR_CFW_SimlockAuthEnd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockAuthEnd));

#define TAG_CFW_SimlockToolGetUid 0x62e37403
struct IPAR_CFW_SimlockToolGetUid
{
    uint8_t len;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockToolGetUid));

struct OPAR_CFW_SimlockToolGetUid
{
    uint32_t result;
    uint8_t uidData[128];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockToolGetUid));

#define TAG_CFW_SimlockDataCheckUnlock 0x35e180c8
struct IPAR_CFW_SimlockDataCheckUnlock
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockDataCheckUnlock));

struct OPAR_CFW_SimlockDataCheckUnlock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockDataCheckUnlock));

#define TAG_CFW_SimlockDataCheckLock 0x7fdfe70c
struct IPAR_CFW_SimlockDataCheckLock
{
    uint16_t len;
    uint8_t nFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockDataCheckLock));

struct OPAR_CFW_SimlockDataCheckLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockDataCheckLock));

#define TAG_CFW_SimlockImeiVerify 0x02e02b8b
struct IPAR_CFW_SimlockImeiVerify
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockImeiVerify));

struct OPAR_CFW_SimlockImeiVerify
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockImeiVerify));

#define TAG_CFW_SimlockSetVersion 0x6021b27e
struct IPAR_CFW_SimlockSetVersion
{
    char * data;
    uint16_t len;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimlockSetVersion));

struct OPAR_CFW_SimlockSetVersion
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimlockSetVersion));

#define TAG_CFW_SetMoSmsRetryCount 0xdf530c31
struct IPAR_CFW_SetMoSmsRetryCount
{
    uint8_t smc_count;
    uint8_t smr_count;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetMoSmsRetryCount));

struct OPAR_CFW_SetMoSmsRetryCount
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetMoSmsRetryCount));

#define TAG_CFW_DecodeCDMAImsi 0x685d1b5a
struct IPAR_CFW_DecodeCDMAImsi
{
    uint8_t pCdmaImsiM[10];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_DecodeCDMAImsi));

struct OPAR_CFW_DecodeCDMAImsi
{
    bool result;
    uint8_t pImsi[15];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_DecodeCDMAImsi));

#define TAG_CFW_SimGetCdmaImsi 0xb80b31fa
struct IPAR_CFW_SimGetCdmaImsi
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetCdmaImsi));

struct OPAR_CFW_SimGetCdmaImsi
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetCdmaImsi));

#define TAG_CFW_GetNMR 0xbcbd6bab
struct IPAR_CFW_GetNMR
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNMR));

struct OPAR_CFW_GetNMR
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNMR));

#define TAG_CFW_SmsCbGetNode 0x53bd5477
struct IPAR_CFW_SmsCbGetNode
{
    void * pCBList;
    uint8_t nIndex;
    uint8_t nCount;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsCbGetNode));

struct OPAR_CFW_SmsCbGetNode
{
    uint32_t result;
    CFW_SMS_CB_MSG_NODE_T pNode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsCbGetNode));

#define TAG_CFW_GetGsmBlkList 0xf40e815a
struct IPAR_CFW_GetGsmBlkList
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGsmBlkList));

struct OPAR_CFW_GetGsmBlkList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGsmBlkList));

#define TAG_CFW_CusPrivatePsm 0x7747c1fb
struct IPAR_CFW_CusPrivatePsm
{
    uint8_t flag;
    uint16_t time;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CusPrivatePsm));

struct OPAR_CFW_CusPrivatePsm
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CusPrivatePsm));

#define TAG_CFW_SetCDACValue 0x34cca851
struct IPAR_CFW_SetCDACValue
{
    uint8_t nCdac;
    bool bSaveFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetCDACValue));

struct OPAR_CFW_SetCDACValue
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetCDACValue));

#define TAG_CFW_GetCDACValue 0xa1ad370d
struct OPAR_CFW_GetCDACValue
{
    uint32_t result;
    uint8_t pCdac;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetCDACValue));

#define TAG_CFW_GetNetDLPsmParam 0xac0be1d6
struct IPAR_CFW_GetNetDLPsmParam
{
    uint16_t nUti;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNetDLPsmParam));

struct OPAR_CFW_GetNetDLPsmParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNetDLPsmParam));

#define TAG_CFW_GetLteBlkList 0xb06bbc57
struct IPAR_CFW_GetLteBlkList
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLteBlkList));

struct OPAR_CFW_GetLteBlkList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLteBlkList));

#define TAG_CFW_NwSetNetTAU 0xbf10cb36
struct IPAR_CFW_NwSetNetTAU
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetNetTAU));

struct OPAR_CFW_NwSetNetTAU
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetNetTAU));

#define TAG_CFW_SetGsmGprsOnly 0x0e31fbed
struct IPAR_CFW_SetGsmGprsOnly
{
    uint8_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetGsmGprsOnly));

struct OPAR_CFW_SetGsmGprsOnly
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetGsmGprsOnly));

#define TAG_CFW_GetGsmGprsOnly 0x9587b351
struct IPAR_CFW_GetGsmGprsOnly
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGsmGprsOnly));

struct OPAR_CFW_GetGsmGprsOnly
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGsmGprsOnly));

#define TAG_CFW_GetUsrHisFreqInfo 0xfbda517a
struct IPAR_CFW_GetUsrHisFreqInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetUsrHisFreqInfo));

struct OPAR_CFW_GetUsrHisFreqInfo
{
    uint32_t result;
    CFW_UsrHisFreqInfo pHisFreqInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetUsrHisFreqInfo));

#define TAG_CFW_SetUsrHisFreqInfo 0x30587993
struct IPAR_CFW_SetUsrHisFreqInfo
{
    CFW_StoredUsrFreqInfo pHisFreqInfo;
    uint8_t bOperFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetUsrHisFreqInfo));

struct OPAR_CFW_SetUsrHisFreqInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetUsrHisFreqInfo));

#define TAG_CFW_SatGetMenuTitle 0x3e3a67a0
struct IPAR_CFW_SatGetMenuTitle
{
    uint8_t title_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetMenuTitle));

struct OPAR_CFW_SatGetMenuTitle
{
    bool result;
    uint8_t title[255];
    uint8_t title_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetMenuTitle));

#define TAG_CFW_SatGetMenuItemByID 0xfc7f5f83
struct IPAR_CFW_SatGetMenuItemByID
{
    uint8_t item_id;
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetMenuItemByID));

struct OPAR_CFW_SatGetMenuItemByID
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetMenuItemByID));

#define TAG_CFW_SatGetMenuCurrentItem 0x4c241761
struct IPAR_CFW_SatGetMenuCurrentItem
{
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetMenuCurrentItem));

struct OPAR_CFW_SatGetMenuCurrentItem
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
    uint8_t item_id;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetMenuCurrentItem));

#define TAG_CFW_SatGetMenuFirstItem 0xbbaeb22f
struct IPAR_CFW_SatGetMenuFirstItem
{
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetMenuFirstItem));

struct OPAR_CFW_SatGetMenuFirstItem
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
    uint8_t item_id;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetMenuFirstItem));

#define TAG_CFW_SatGetMenuNextItem 0x8b78bf05
struct IPAR_CFW_SatGetMenuNextItem
{
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetMenuNextItem));

struct OPAR_CFW_SatGetMenuNextItem
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
    uint8_t item_id;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetMenuNextItem));

#define TAG_CFW_SatGetSubMenuTitle 0x0e0e533d
struct IPAR_CFW_SatGetSubMenuTitle
{
    uint8_t title_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSubMenuTitle));

struct OPAR_CFW_SatGetSubMenuTitle
{
    bool result;
    uint8_t title[255];
    uint8_t title_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSubMenuTitle));

#define TAG_CFW_SatGetSubMenuItemByID 0x80c5ea5a
struct IPAR_CFW_SatGetSubMenuItemByID
{
    uint8_t item_id;
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSubMenuItemByID));

struct OPAR_CFW_SatGetSubMenuItemByID
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSubMenuItemByID));

#define TAG_CFW_SatGetSubMenuCurrentItem 0x05ece985
struct IPAR_CFW_SatGetSubMenuCurrentItem
{
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSubMenuCurrentItem));

struct OPAR_CFW_SatGetSubMenuCurrentItem
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
    uint8_t item_id;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSubMenuCurrentItem));

#define TAG_CFW_SatGetSubMenuFirstItem 0xc1dcd2f0
struct IPAR_CFW_SatGetSubMenuFirstItem
{
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSubMenuFirstItem));

struct OPAR_CFW_SatGetSubMenuFirstItem
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
    uint8_t item_id;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSubMenuFirstItem));

#define TAG_CFW_SatGetSubMenuNextItem 0xb27d5400
struct IPAR_CFW_SatGetSubMenuNextItem
{
    uint8_t item_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSubMenuNextItem));

struct OPAR_CFW_SatGetSubMenuNextItem
{
    bool result;
    uint8_t item[255];
    uint8_t item_length;
    uint8_t item_id;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSubMenuNextItem));

#define TAG_CFW_SatGetSendSmsServiceCenterAddr 0xa54f5bc9
struct IPAR_CFW_SatGetSendSmsServiceCenterAddr
{
    uint8_t addr_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendSmsServiceCenterAddr));

struct OPAR_CFW_SatGetSendSmsServiceCenterAddr
{
    bool result;
    uint8_t addr[255];
    uint8_t addr_length;
    uint8_t addr_type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendSmsServiceCenterAddr));

#define TAG_CFW_SatGetSendSmsTPDU 0xdfdc87cc
struct IPAR_CFW_SatGetSendSmsTPDU
{
    uint8_t tpdu_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendSmsTPDU));

struct OPAR_CFW_SatGetSendSmsTPDU
{
    bool result;
    uint8_t tpdu[255];
    uint8_t tpdu_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendSmsTPDU));

#define TAG_CFW_SatGetSendSmsAlpha 0x34ba6071
struct IPAR_CFW_SatGetSendSmsAlpha
{
    uint8_t alpha_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendSmsAlpha));

struct OPAR_CFW_SatGetSendSmsAlpha
{
    bool result;
    uint8_t alpha[255];
    uint8_t alpha_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendSmsAlpha));

#define TAG_CFW_SatGetDisplayText 0xdfe1b99e
struct IPAR_CFW_SatGetDisplayText
{
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetDisplayText));

struct OPAR_CFW_SatGetDisplayText
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
    uint8_t dcs;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetDisplayText));

#define TAG_CFW_SatGetGetInKeyText 0x94f6a9de
struct IPAR_CFW_SatGetGetInKeyText
{
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetGetInKeyText));

struct OPAR_CFW_SatGetGetInKeyText
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
    uint8_t dcs;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetGetInKeyText));

#define TAG_CFW_SatGetGetInputText 0x69789adc
struct IPAR_CFW_SatGetGetInputText
{
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetGetInputText));

struct OPAR_CFW_SatGetGetInputText
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
    uint8_t dcs;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetGetInputText));

#define TAG_CFW_SatGetGetInputLengthRange 0x39ee6a8f
struct IPAR_CFW_SatGetGetInputLengthRange
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetGetInputLengthRange));

struct OPAR_CFW_SatGetGetInputLengthRange
{
    bool result;
    uint8_t max_len;
    uint8_t min_len;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetGetInputLengthRange));

#define TAG_CFW_SatGetGetInputDefaultText 0x1d1787bb
struct IPAR_CFW_SatGetGetInputDefaultText
{
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetGetInputDefaultText));

struct OPAR_CFW_SatGetGetInputDefaultText
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
    uint8_t dcs;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetGetInputDefaultText));

#define TAG_CFW_SatGetSetupCallText 0x75599008
struct IPAR_CFW_SatGetSetupCallText
{
    uint8_t stage;
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSetupCallText));

struct OPAR_CFW_SatGetSetupCallText
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSetupCallText));

#define TAG_CFW_SatGetSetupCallAddr 0x1a0bd347
struct IPAR_CFW_SatGetSetupCallAddr
{
    uint8_t which;
    uint8_t addr_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSetupCallAddr));

struct OPAR_CFW_SatGetSetupCallAddr
{
    bool result;
    uint8_t addr[255];
    uint8_t addr_length;
    uint8_t addr_type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSetupCallAddr));

#define TAG_CFW_SatGetSetupCallCapCfg 0x726026aa
struct IPAR_CFW_SatGetSetupCallCapCfg
{
    uint8_t cfg_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSetupCallCapCfg));

struct OPAR_CFW_SatGetSetupCallCapCfg
{
    bool result;
    uint8_t cfg[255];
    uint8_t cfg_length;
    uint8_t unit;
    uint8_t interval;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSetupCallCapCfg));

#define TAG_CFW_SatGetSendSSDisplayInfo 0x406221ef
struct IPAR_CFW_SatGetSendSSDisplayInfo
{
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendSSDisplayInfo));

struct OPAR_CFW_SatGetSendSSDisplayInfo
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
    uint8_t icon_identifier;
    uint8_t icon_qualifier;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendSSDisplayInfo));

#define TAG_CFW_SatGetSendSSInfo 0x75b5e809
struct IPAR_CFW_SatGetSendSSInfo
{
    uint8_t ss_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendSSInfo));

struct OPAR_CFW_SatGetSendSSInfo
{
    bool result;
    uint8_t ss[255];
    uint8_t ss_length;
    uint8_t ss_type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendSSInfo));

#define TAG_CFW_SatGetSendUSSDDisplayInfo 0x400d76b9
struct IPAR_CFW_SatGetSendUSSDDisplayInfo
{
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendUSSDDisplayInfo));

struct OPAR_CFW_SatGetSendUSSDDisplayInfo
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
    uint8_t icon_identifier;
    uint8_t icon_qualifier;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendUSSDDisplayInfo));

#define TAG_CFW_SatGetSendUSSDInfo 0xe58d210f
struct IPAR_CFW_SatGetSendUSSDInfo
{
    uint8_t ussd_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendUSSDInfo));

struct OPAR_CFW_SatGetSendUSSDInfo
{
    bool result;
    uint8_t ussd[255];
    uint8_t ussd_length;
    uint8_t dcs;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendUSSDInfo));

#define TAG_CFW_SatGetSendDTMFInfo 0x954447f6
struct IPAR_CFW_SatGetSendDTMFInfo
{
    uint8_t text_length;
    uint8_t dtmf_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendDTMFInfo));

struct OPAR_CFW_SatGetSendDTMFInfo
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
    uint8_t dtmf[255];
    uint8_t dtmf_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendDTMFInfo));

#define TAG_CFW_SatGetSendDTMFText 0x18dfc76c
struct IPAR_CFW_SatGetSendDTMFText
{
    uint8_t text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendDTMFText));

struct OPAR_CFW_SatGetSendDTMFText
{
    bool result;
    uint8_t text[255];
    uint8_t text_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendDTMFText));

#define TAG_CFW_SatGetSendDTMF 0x9618247d
struct IPAR_CFW_SatGetSendDTMF
{
    uint8_t dtmf_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSendDTMF));

struct OPAR_CFW_SatGetSendDTMF
{
    bool result;
    uint8_t dtmf[255];
    uint8_t dtmf_length;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSendDTMF));

#define TAG_CFW_SatGetSetupIdleMode 0x8bc4f81c
struct IPAR_CFW_SatGetSetupIdleMode
{
    uint8_t mode_text_length;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetSetupIdleMode));

struct OPAR_CFW_SatGetSetupIdleMode
{
    bool result;
    uint8_t mode_text[255];
    uint8_t mode_text_length;
    uint8_t dcs;
    uint8_t icon_identifier;
    uint8_t icon_qualifier;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetSetupIdleMode));

#define TAG_CFW_SatGetLanguageNotification 0xff9ba171
struct IPAR_CFW_SatGetLanguageNotification
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetLanguageNotification));

struct OPAR_CFW_SatGetLanguageNotification
{
    bool result;
    uint8_t language[2];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetLanguageNotification));

#define TAG_CFW_SatGetPlayTone 0x48f0aa6d
struct IPAR_CFW_SatGetPlayTone
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetPlayTone));

struct OPAR_CFW_SatGetPlayTone
{
    bool result;
    uint8_t tone;
    uint8_t unit;
    uint8_t interval;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetPlayTone));

#define TAG_CFW_nvSetCopsTimer 0x87f5eb5f
struct IPAR_CFW_nvSetCopsTimer
{
    uint8_t nValue;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_nvSetCopsTimer));

struct OPAR_CFW_nvSetCopsTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvSetCopsTimer));

#define TAG_CFW_nvGetCopsTimer 0x22d5d04f
struct OPAR_CFW_nvGetCopsTimer
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_nvGetCopsTimer));

#define TAG_CFW_NwSetCsgMode 0x84c8a6b6
struct IPAR_CFW_NwSetCsgMode
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetCsgMode));

struct OPAR_CFW_NwSetCsgMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetCsgMode));

#define TAG_CFW_NwGetCsgMode 0x5885250b
struct IPAR_CFW_NwGetCsgMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetCsgMode));

struct OPAR_CFW_NwGetCsgMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetCsgMode));

#define TAG_CFW_NwSetMaxAttFailForIratReq 0x4b18a9b9
struct IPAR_CFW_NwSetMaxAttFailForIratReq
{
    uint8_t nMaxNum;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetMaxAttFailForIratReq));

struct OPAR_CFW_NwSetMaxAttFailForIratReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetMaxAttFailForIratReq));

#define TAG_CFW_GetDefaultPcoControl 0xcb1adce5
struct IPAR_CFW_GetDefaultPcoControl
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetDefaultPcoControl));

struct OPAR_CFW_GetDefaultPcoControl
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetDefaultPcoControl));

#define TAG_CFW_SetDefaultPcoControl 0x5e1f4cd3
struct IPAR_CFW_SetDefaultPcoControl
{
    uint8_t nValue;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetDefaultPcoControl));

struct OPAR_CFW_SetDefaultPcoControl
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetDefaultPcoControl));


#define TAG_PCNgisYfirev 0xe377c554
struct IPAR_PCNgisYfirev
{
    uint8_t input[50];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_PCNgisYfirev));

struct OPAR_PCNgisYfirev
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_PCNgisYfirev));


#endif // _CFW_RPC_A2C_PAR_H_
// AUTO GENERATED END
