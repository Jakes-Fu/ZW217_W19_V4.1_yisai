/*****************************************************************************
** File Name:      mmisfr_lwm2m.c
** Author:
** Copyright (C) 2019 Unisoc Communications Inc.
** Description:    Transmit data by LwM2M sdk
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE				NAME				DESCRIPTION
** 2020/07/17		mark.zhang1			Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#include <stdio.h>
#include "os_api.h"
#include "mmi_module.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmisfr_lwm2m.h"
#ifdef LWM2M_SUPPORT
#include "lwm2msdk.h"
#endif
#include "mn_type.h"
#include "mmiutil.h"
#include "mmisfr_main.h"
#include "mmicc_export.h"
#include "mmipb_export.h"
#include "mmipb_common.h"
#include "mmiconnection_export.h"

#if defined LWM2M_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define RETRY_TIME_INTERVAL 60000 //milliseconds
#define MAX_RETRY_TIMES  3

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern MN_IMSI_T MNSIM_GetImsiEx(MN_DUAL_SYS_E dual_sys);

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
uint8 s_register_timer;
uint8 s_retry_times;
BOOLEAN s_run_result = FALSE;
BOOLEAN s_in_register = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL Options MMISFR_GenerateData(void);

/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL OptFuncs MMISFR_InitCallback(void);

/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL void MMISFR_PostLwM2MData(void);

/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL void MMISFR_HandleLwM2MTimer(uint8  timer_id, uint32 param);

/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL void MMISFR_ResetFlags(void);

/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL void MMISFR_ClearLwM2M(BOOLEAN s_need_stop);

/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL void MMISFR_NextLoop(BOOLEAN s_successful);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
PUBLIC void MMISFR_LwM2MEnter(void)
{
    SCI_TRACE_LOW("[lwm2m]MMISFR_LwM2MEnter ...");
    MMISFR_ResetFlags();
    if (LWM2M_SDK_IS_RUN())
    {
        s_in_register = FALSE;
        LWM2M_SDK_UPDATE_REG();
    }
    else
    {
        Options opt = MMISFR_GenerateData();
        OptFuncs optfuncs = MMISFR_InitCallback();
        int ret = LWM2M_SDK_INIT(&opt, &optfuncs);
        MMISFR_PostLwM2MData();
    }
}

LOCAL Options MMISFR_GenerateData(void)
{
    Options opt =
    {
        .szCMEI_IMEI = "",         //CMEI/IMEI
        .szCMEI_IMEI2 = "",       //CMEI/IMEI2
        .szIMSI = "",                 //IMSI
        .szDMv = "v2.0",           //DM version,v2.0
        .szAppKey = "M100000001",     //appkey
        .szPwd = "ff8lv9P6I64fB51Y30n128K858rob353", //pwd password
        .nAddressFamily = 4,                // 4: ipv4-fixed; 6:ipv6;
        .szSrvIP = "m.fxltsbl.com", //host or ip: shipei.fxltsbl.com(test)/m.fxltsbl.com(release)
        .nSrvPort = 5683,             //server port¸ 5683 generally
        .nLifetime = 86400,              //update interval 300s(test),86400s(rel)
        .nLocalPort = 56831,            //local port
        .nBootstrap = 0,                 //booststrap 0,not use; other n,use; set current value to 0
        .bUseSDKRegUpdate = 0
    };

    MN_IMEISVN_T imeisvn = {0};
    if (MNNV_GetIMEISVNEx(0, &imeisvn))
    {
        char ori_imei[(MN_MAX_IMEI_LENGTH << 1) + 1] = {0};
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei,
                           MN_MAX_IMEI_LENGTH << 1, ori_imei);
        SCI_TRACE_LOW("MMISFR_GenerateData ori_imei len=%d", strlen(ori_imei));
        SCI_MEMCPY(opt.szCMEI_IMEI, &ori_imei[1], strlen(ori_imei) - 1);//first is '*'
    }

    MN_IMSI_T imsi = MNSIM_GetImsiEx(0);
    char imsi_val[(MN_MAX_IMSI_ARR_LEN << 1) + 1] = { 0 };
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, imsi.imsi_val, MN_MAX_IMSI_ARR_LEN << 1,
                       imsi_val);
    SCI_TRACE_LOW("MMISFR_GenerateData imsi_val len=%d", strlen(imsi_val));
    SCI_MEMCPY(opt.szIMSI, &imsi_val[1],   strlen(imsi_val) - 1);//first is '9'

    return opt;
}

LOCAL void  MMISFR_LwM2MNotifyMsg(OptNotifyParam *optNotifyParam)
{
    if (optNotifyParam == NULL)
        return ;

    SCI_TRACE_LOW("[lwm2m]MMISFR_LwM2MNotifyMsg:type=%0x,code=%d,msg=%s\n",
                  optNotifyParam->notify_type, optNotifyParam->notify_code,
                  optNotifyParam->notify_msg);
    switch (optNotifyParam->notify_type)
    {
        case NOTIFY_TYPE_SYS:
            if (s_run_result && NOTIFY_CODE_OK == optNotifyParam->notify_code)
            {
                MMISFR_ClearLwM2M(FALSE);
                MMISFR_NextLoop(TRUE);
            }
            break;

        case NOTIFY_TYPE_REGISTER:
            if (NOTIFY_CODE_OK == optNotifyParam->notify_code)
            {
                s_run_result = TRUE;
            }
            else
            {
                s_run_result = FALSE;
                if (s_retry_times++ < MAX_RETRY_TIMES)
                {
                    s_register_timer = MMK_CreateTimerCallback(RETRY_TIME_INTERVAL,
                                       MMISFR_HandleLwM2MTimer, NULL,
                                       FALSE);
                }
                else
                {
                    MMISFR_ClearLwM2M(TRUE);
                    MMISFR_NextLoop(FALSE);
                }
            }
            break;

        case NOTIFY_TYPE_DEREGISTER://HeartBeat result
            s_run_result = FALSE;
            break;

        case NOTIFY_TYPE_REG_UPDATE:
            if (NOTIFY_CODE_OK == optNotifyParam->notify_code)
            {
                s_run_result = TRUE;
            }
            else
            {
                s_run_result = FALSE;
                MMISFR_ClearLwM2M(TRUE);
                MMISFR_NextLoop(FALSE);
            }
            break;

        default:
            break;
    }
}

LOCAL uint8 *MMISFR_GetNetAccount(void)
{
    MMICONNECTION_LINKSETTING_DETAIL_T *linksetting = PNULL;
    static uint8 NetAccount[MMICONNECTION_MAX_APN_LEN + 1] = {0};
    SCI_TRACE_LOW("MMISFR_GetNetAccount()");
    uint8 linkNum = 0;
    linkNum = MMIAPICONNECTION_GetLinkSettingNum(0);
    for (int i = 0; i < linkNum; i++)
    {
        linksetting = PNULL;
        linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(0, i);

        // print apn information and plmn
        if (PNULL != linksetting && 0 != linksetting->apn_len)
        {
            SCI_MEMCPY(NetAccount, linksetting->apn, strlen(linksetting->apn));
            break;
        }
    }
    return NetAccount;
}

LOCAL uint8 *MMISFR_GetNetworkType(void)
{
    MN_GMMREG_RAT_E gmmreg_rat = MNPHONE_GetConfiguredRATEx(0);
    static uint8 nettype[3] = {0};
    switch (gmmreg_rat)
    {
        case MN_GMMREG_RAT_GPRS_3G_LTE:
        case MN_GMMREG_RAT_LTE:
        case MN_GMMREG_RAT_GPRS_LTE:
            SCI_MEMCPY(nettype, "4G", 3);
            break;
        case MN_GMMREG_RAT_3G:
        //  case MN_GMMREG_RAT_TD:
        case MN_GMMREG_RAT_GPRS_AND_3G:
        case MN_GMMREG_RAT_VDS_GSM:
            SCI_MEMCPY(nettype, "3G", 3);
            break;
        case MN_GMMREG_RAT_GPRS:
        case MN_GMMREG_RAT_CDMA:
            SCI_MEMCPY(nettype, "2G", 3);
            break;
        default:
            SCI_MEMCPY(nettype, "2G", 3);
            break;
    }
    return nettype;

}

LOCAL uint8 *MMISFR_GETWifiMac(void)
{
    static uint8   addr_str[20] = {0};
    char    wifi_addr[6]      = {0};  //ETH_ALEN
    wifi_get_mac(wifi_addr);
    sprintf((char *)addr_str, "%02x:%02x:%02x:%02x:%02x:%02x", wifi_addr[0],
            wifi_addr[1], wifi_addr[2], wifi_addr[3], wifi_addr[4], wifi_addr[5]);
    SCI_TRACE_LOW("MMISFR_GETWifiMac()WIFI mac %02x %02x %02x %02x %02x %02x",
                  wifi_addr[0], wifi_addr[1], wifi_addr[2], wifi_addr[3], wifi_addr[4],
                  wifi_addr[5]);
    SCI_TRACE_LOW("MMISFR_GETWifiMac=%s", addr_str);
    return addr_str;
}

LOCAL int MMISFR_DMReadInfo(int resId, char **outbuff)
{
    int iret = 0;
    *outbuff = NULL;
    BOOLEAN  is_volte_state;
    char buff[512];
    int  buflen = sizeof(buff);
    memset(buff, 0, sizeof(buff));

    snprintf(buff, buflen, "%s", "unknown"); //default
    SCI_TRACE_LOW("[lwm2m]MMISFR_DMReadInfo:resId=%d",
                  resId);
    switch (resId)
    {
        case 6601://devinfo
            snprintf(buff, buflen, "%s", "");
            break;
        case 6602://appinfo
            snprintf(buff, buflen,
                     "app_name1|pak_name1|1234000|3\r\napp_name2|pak_name2|2234000|5\r\n");
            break;
        case 6603://mac
            snprintf(buff, buflen, MMISFR_GETWifiMac());
            break;
        case 6604://rom
            snprintf(buff, buflen, "128MB");
            break;
        case 6605://ram
            snprintf(buff, buflen, "128MB");
            break;
        case 6606://CPU
            snprintf(buff, buflen, "ARM A53");
            break;
        case 6607://SYS VERSION
            snprintf(buff, buflen, "ThreadX");
            break;
        case 6608://FIRMWARE VERSION
            snprintf(buff, buflen, "MOCOR_317_W19.37.5_Release");
            break;
        case 6609://FIRMWARE NAME
            snprintf(buff, buflen, "AntisW3");
            break;
        case 6610://Volte
            is_volte_state = MMIAPIPHONE_GetVolteStatus(0);
            if (is_volte_state == TRUE)
            {
                snprintf(buff, buflen, "0");
            }
            else
            {
                snprintf(buff, buflen, "1");
            }
            break;
        case 6611://NetType
            snprintf(buff, buflen, MMISFR_GetNetworkType());
            break;
        case 6612://Net Account
            snprintf(buff, buflen, MMISFR_GetNetAccount());
            break;
        case 6613://PhoneNumber
            snprintf(buff, buflen, "");
            break;
        case 6614://Location
            snprintf(buff, buflen, "");
            break;

        default:
            iret = -1;
            break;
    }

    if (iret == 0)
    {
        //apply for buffer, sdk will free buffer
        *outbuff = (char *)malloc(strlen(buff) + 1);
        strcpy(*outbuff, buff);
    }

    SCI_TRACE_LOW("[lwm2m]MMISFR_DMReadInfo,ID=%d,value=%s,iret=%d\n", resId, buff,
                  iret);
    return iret;
}

LOCAL OptFuncs MMISFR_InitCallback(void)
{
    OptFuncs optfuncs = {NULL, NULL};
    optfuncs.NotifyMsg  = MMISFR_LwM2MNotifyMsg;
    optfuncs.DMReadInfo = MMISFR_DMReadInfo;

    return optfuncs;
}

LOCAL void MMISFR_PostLwM2MData(void)
{
    SCI_TRACE_LOW("[lwm2m]MMISFR_PostLwM2MData s_retry_times = %d",
                  s_retry_times);
    int ret_stop = LWM2M_SDK_STOP();
    int ret_run = LWM2M_SDK_RUN(1);
}

LOCAL void MMISFR_HandleLwM2MTimer(uint8  timer_id, uint32 param)
{
    MMK_StopTimer(timer_id);
    MMISFR_PostLwM2MData();
}

LOCAL void MMISFR_ResetFlags()
{
    s_register_timer = 0;
    s_in_register = TRUE;
    s_retry_times = 0;
    s_run_result = FALSE;
}

LOCAL void MMISFR_ClearLwM2M(BOOLEAN s_need_stop)
{
    if (s_register_timer > 0)
    {
        MMK_StopTimer(s_register_timer);
    }

    if (s_need_stop)
    {
        LWM2M_SDK_STOP();
    }
    LWM2M_SDK_FINI();
}

LOCAL void MMISFR_NextLoop(BOOLEAN s_successful)
{
    if (s_successful)
    {
        MMISFR_RetryUpdate(s_in_register);
    }
    else
    {
        MMISFR_RetryRegister(s_in_register);
    }
}

PUBLIC void MMISFR_ReceivePdpDeactiveInd(void)
{
    MMISFR_ClearLwM2M(TRUE);
    MMISFR_NextLoop(FALSE);
}

#endif

