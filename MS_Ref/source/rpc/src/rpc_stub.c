#include "sci_types.h"
#include "rpc_daemon.h"
#include "os_api.h"
#include "cfw.h"
#include "cfw_event.h"
#include "audio_api.h"
#include "voice_process.h"

#include <stdlib.h>
#include <string.h>
#include <alloca.h>

uint32_t CFW_GetSSimFileInfoFromAP(CFW_SSIM_FILE_INFO_T *cfw_ssim_file_info, CFW_SIM_ID nSimID)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_SetSoftSimDefaultInfo(uint8_t *nImsi, uint8_t *nKey, uint8_t *nPlmn)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_GetDftPdnInfoFromAP(CFW_GPRS_DFTPDN_INFO *nDftPdn, CFW_SIM_ID nSimID)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_GetBootCausesFromAP()
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_WifiSentestGetResult(CFW_WIFISENTEST_RES wifi_res)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_MeDeleteMessageV2(CFW_SIM_ID nSimId, uint16_t nIndex, uint8_t nStatus, uint8_t *nType, uint32_t *pErrcode, uint32_t *HaveFreeSlot)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_MeWriteMessageV2(CFW_SIM_ID nSimId, uint16_t nIndex, uint8_t *pData, uint32_t nTime, uint32_t nAppInt32, uint8_t *nType, uint32_t *pErrcode, uint32_t *HaveFreeSlot)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_MeReadMessageV2(uint16_t nLocation, uint16_t nIndex, uint8_t *pData, uint8_t *nType, uint32_t *pErrcode)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_MeListMessageV2(CFW_SIM_ID nSimId, uint8_t nStatus, uint16_t nCount, uint16_t nIndex, uint8_t *pData, uint8_t *nType, uint32_t *pErrcode, uint32_t *storageCount)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_MeGetStorageInfoV2(CFW_SIM_ID nSimId, uint8_t nStatus, uint8_t *nType, uint32_t *pErrcode, uint16_t *pStorageId, uint16_t *pTotalSlot, uint16_t *pUsedSlot)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t SMS_DM_Init_V2(void)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_GetDefaultApn(OPER_DEFAULT_APN_INFO_V2 nDefaultApn[AT_DEFAULT_APN_MAX_NUM])
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_GetApnInfo(CFW_APNS_UNAME_UPWD *apn, char *plmn)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_GetSimLockStatusFromAP(CFW_SRV_SIMLOCKDATA_T *data)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint32_t CFW_GetDetectMBS(uint8_t *pMode)
{
    SCI_TRACE_LOW("not support now");
    return 0;
}
uint8_t CFW_GetDefaultLteCidCtoA()
{
    SCI_TRACE_LOW("not support now");
    return 5;
}
uint16_t sim_GetSimCardModeCB(uint8_t nSimID)
{
    SCI_TRACE_LOW("not support now");
    return 0;
}
uint16_t sim_VsimResetCB(uint8_t *ATRData, uint8_t *ATRSize, uint8_t nSimID)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint16_t sim_VsimProcessApduCB(uint8_t *TxData, uint16_t TxSize, uint8_t *RxData, uint16_t *RxSize, uint8_t nSimID)
{
    SCI_TRACE_LOW("not support now");
    return 0x9000;
}
uint32_t CFW_WriteLongSmsInfo(uint8_t nStorage, uint8_t nStatus, uint32_t nParam1, uint32_t nParam2, uint8_t nSimID)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
uint8_t CFW_NwGetLockBCCHCtoA()
{
    SCI_TRACE_LOW("not support now");
    return 0;
}
uint8_t CFW_NwIsUseISIM(char *plmnStr)
{
    SCI_TRACE_LOW("not support now");
    return 1;
}
void AUD_VoiceCoderChangeInd(void)
{
    SCI_TRACE_LOW("%s",__FUNCTION__);

#ifdef VOICE_PROCESS_NEW
    voice_process_set_param(0, AUDIO_MODE_APP_TYPE_VOICE, AUDIO_DEVICE_MODE_HANDFREE, AUDIO_VOLUME_CUR);
#endif
    return;
}