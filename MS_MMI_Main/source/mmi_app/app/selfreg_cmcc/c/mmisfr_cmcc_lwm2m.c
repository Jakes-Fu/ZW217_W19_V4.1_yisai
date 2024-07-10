/*****************************************************************************
** Copyright 2023 Unisoc(Shanghai) Technologies Co.,Ltd                      *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*****************************************************************************
** File Name:      mmisfr_cmcc_lwm2m.c                                       *
** Author:         miao.liu2                                                 *
** Date:           3/4/2023                                                  *
** Description:    This file is used to define cmcc self register function   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2023      miao.liu2             Create                                 *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef LIBLWM2M_SUPPORT
#include "lwm2m_api.h"
#endif
#include "mn_type.h"
#include "mmiutil.h"
#include "dm_endpoint.h"
#include "ual_tele_sim.h"
#include "mmisfr_cmcc_lwm2m.h"
#include "mmk_timer.h"
#include "mmicom_trace.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SFRCMCC_IMEI_MAX_LENGTH (16)
#define SFRCMCC_IMSI_MAX_LENGTH (16)

extern BLOCK_ID g_liblwm2m_MESSAGE_TASK;
/**-------------------------------------------------------------------------*
**                         STATIC DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL int8  s_miplconnectref = -1;
LOCAL BOOLEAN s_sfr_register_succ = FALSE;
LOCAL cmcc_self_register_callback s_sfr_cmcc_callback = PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :mipl的回调函数
//  Param :uint32, void
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void SFR_CMCC_MiplIndProcessEntry(uint32 argc, void *p_argv);

/*****************************************************************************/
//  Description :真正进行自注册流程的入口
//  Param :none
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SFR_CMCC_Register(void);
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :发送注册请求
//  Param :ref:实例ID lifetime:设备生命周期  timeout:注册超时时间
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SFR_CMCC_MiplOpen(int ref, uint32 lifetime, uint32 timeout)
{
    TRACE_SFR_CMCC("ref :%d lifetime : %d timeout: %d\n", ref, lifetime, timeout);
    if (s_miplconnectref != ref)
    {
        TRACE_SFR_CMCC("s_miplconnectref != ref\n");
        return FALSE;
    }

    if (0 != liblwm2m_MessageHandleTask((osiCallback_t)SFR_CMCC_MiplIndProcessEntry))
    {
        TRACE_SFR_CMCC("MessageHandleTask error\n");
        return FALSE;
    }

    if (LWM2M_RET_OK == lwm2m_register(s_miplconnectref, lifetime, timeout))
    {
        TRACE_SFR_CMCC("register success");
    }
    else
    {
        TRACE_SFR_CMCC("register failed");
        return FALSE;
    }
    return TRUE;

}

/*****************************************************************************/
//  Description :发送注销请求
//  Param :ref:实例ID
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SFR_CMCC_MiplClose(int ref)
{
    TRACE_SFR_CMCC("ref = %d, s_miplconnectref = %d\n", ref, s_miplconnectref);

    if (ref != s_miplconnectref || -1 == s_miplconnectref)
    {
        TRACE_SFR_CMCC("ref error\n");
        return FALSE;
    }

    if (LWM2M_RET_OK == lwm2m_unregister(s_miplconnectref))
    {
        TRACE_SFR_CMCC("unregister success");
    }
    else
    {
        TRACE_SFR_CMCC("unregister failed");
        return FALSE;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description :创建一个通信套件实例
//  Param :p_host:网址信息 p_port:端口号 p_device_name:endpoint name p_dmupdevice_name:ndpoint name p_dm_pwd:appkey
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL int SFR_CMCC_MiplCreate(char *p_host, char *p_port, char *p_device_name, char *p_dmupdevice_name, char *p_dm_pwd)
{
    char cmdline[512] = {0};

    if(PNULL == p_host || PNULL == p_port || PNULL == p_device_name || PNULL == p_dmupdevice_name || PNULL == p_dm_pwd)
    {
        TRACE_SFR_CMCC("PARAM error");
        return -1;
    }
    sprintf(cmdline, "-e %d -n %s -D %s -W %s -h %s -p %s -4",
            LWM2M_SERVER_ONENET, p_device_name, p_dmupdevice_name, p_dm_pwd, p_host, p_port);


    TRACE_SFR_CMCC("cmdline:%s, len = %d\n", cmdline, sizeof(cmdline));
    s_miplconnectref = lwm2m_new_config((uint8_t *)cmdline);
    if (0 <= s_miplconnectref)
    {
        TRACE_SFR_CMCC("gMiplConnectRef:%d\n",s_miplconnectref);
        return s_miplconnectref;
    }
    else
    {
        TRACE_SFR_CMCC("lwm2m new config failed ");
        return -1;
    }
}

/*****************************************************************************/
//  Description :注销一个通信套件实例
//  Param :ref:实例ID
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SFR_CMCC_MiplDelete (int ref)
{
    if(ref != s_miplconnectref)
    {
        TRACE_SFR_CMCC("s_miplconnectref !=ref");
        return FALSE;
    }
     if(-1 != s_miplconnectref)
    {
        lwm2m_free_config(s_miplconnectref);
        s_miplconnectref = -1;
        TRACE_SFR_CMCC("DELETE success");
        return TRUE;
    }
    else
    {
        TRACE_SFR_CMCC("DELETE error");
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :clear lwm2m相关数据
//  Param :ref:实例ID
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_ClearLwM2M(void)
{
    BOOLEAN result = FALSE;
    TRACE_SFR_CMCC("enter");

    if(PNULL != s_sfr_cmcc_callback)
    {
        s_sfr_cmcc_callback = PNULL;
    }
    result = SFR_CMCC_MiplClose(s_miplconnectref);
    result = SFR_CMCC_MiplDelete(s_miplconnectref);
    TRACE_SFR_CMCC("result = %d", result);
}

/*****************************************************************************/
//  Description :mipl的回调函数
//  Param :uint32, void
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void SFR_CMCC_MiplIndProcessEntry(uint32 argc, void *p_argv)
{
    xSignalHeaderRec *p_sig = PNULL;
    lwm2mEventSIG_t *p_event = PNULL;
    uint8 ref = 0;
    lwm2mEvent_t *p_lwm2mev = PNULL;
    uint32 result = 0;
    uint32 result_param1 = 0;
    uint32 result_param2 = 0;

    TRACE_SFR_CMCC("enter\n");
    if(PNULL == p_argv)
    {
        TRACE_SFR_CMCC("p_argv is pnull");
    }
    g_liblwm2m_MESSAGE_TASK = SCI_IdentifyThread();
    while (1)
    {
        p_sig = SCI_GetSignal(g_liblwm2m_MESSAGE_TASK);
        if (PNULL != p_sig)
        {
            p_event = (lwm2mEventSIG_t*)p_sig;
        }

        ref = (uint8)p_event->param2;
        p_lwm2mev = (lwm2mEvent_t *)p_event->param3;

        if(PNULL == p_lwm2mev)
        {
            TRACE_SFR_CMCC("p_lwm2mev is pnull");
            return;
        }
        result = p_lwm2mev->param1;
        result_param1 = p_lwm2mev->param2;
        result_param2 = p_lwm2mev->param3;
        TRACE_SFR_CMCC("nEventId=%d, result=%d, result_param1=%d, result_param2=%d", p_event->id, result, result_param1, result_param2);

        switch (p_event->id)
        {
            case EVETN_IND:
            {
                if (LWM2M_EVENT_UPDATE_NEED == result)
                {
                    lwm2m_update(60, false, ref);
                }
                else if(LWM2M_EVENT_CONNECT_SUCCESS == result || LWM2M_EVENT_REG_SUCCESS == result)
                {
                    s_sfr_register_succ = TRUE;
                    if(PNULL != s_sfr_cmcc_callback)
                    {
                        TRACE_SFR_CMCC("register success");
                        s_sfr_cmcc_callback(TRUE);
                        s_sfr_cmcc_callback = PNULL;
                    }
                }
                else if(LWM2M_EVENT_REG_FAILED == result || LWM2M_EVENT_REG_TIMEOUT == result || LWM2M_EVENT_CONNECT_FAILED == result)
                {
                    s_sfr_register_succ = FALSE;
                    if(PNULL != s_sfr_cmcc_callback)
                    {
                        TRACE_SFR_CMCC("register fail");
                        s_sfr_cmcc_callback(FALSE);
                        s_sfr_cmcc_callback = PNULL;
                    }
                }
                TRACE_SFR_CMCC("s_sfr_register_succ = %d", s_sfr_register_succ);
                break;
            }
            case OBJ_OPERATE_RSP_IND:
            case SERVER_REGCMD_RESULT_IND:
                break;
            default:
            {
                TRACE_SFR_CMCC("UNPROCESS EVENT %d", p_event->id);
                break;
            }
        }
        if(PNULL != p_lwm2mev)
        {
            SCI_Free(p_lwm2mev);
            p_lwm2mev = PNULL;
        }
        if(PNULL != p_event)
        {
            SCI_Free(p_event);
            p_event = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description :free 指针
//  Param :p_device_name:endpoint name p_dmupdevice_name:ndpoint name p_dm_pwd:appkey
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void SFR_CMCC_FreeDeviceName(char *p_device_name, char *p_dmupdevice_name, char *p_pwd)
{
    if (PNULL != p_device_name)
    {
        TRACE_SFR_CMCC("p_device_name is not pnull");
        SCI_Free(p_device_name);
        p_device_name = PNULL;
    }

    if (PNULL != p_dmupdevice_name)
    {
        TRACE_SFR_CMCC("p_dmupdevice_name is not pnull");
        SCI_Free(p_dmupdevice_name);
        p_dmupdevice_name = PNULL;
    }

    if (PNULL != p_pwd)
    {
        TRACE_SFR_CMCC("p_pwd is not pnull");
        SCI_Free(p_pwd);
        p_pwd = PNULL;
    }
}
/*****************************************************************************/
//  Description :真正进行自注册流程的入口
//  Param :none
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SFR_CMCC_Register(void)
{
    Options dm_config = {0};
    int ref = 0;
    char *p_host = "m.fxltsbl.com";//host or ip: shipei.fxltsbl.com(test)/m.fxltsbl.com(release)
    char *p_port = "5683";
    uint32 lifetime = 86400;    //update interval 300s(test),86400s(rel)
    uint32 timeout = 60;
    MN_IMEISVN_T imeisvn = {0};
    char *p_device_name = PNULL;
    char *p_dmupdevice_name = PNULL;
    char *p_dm_pwd = PNULL;
    ual_tele_sim_result_e result = UAL_TELE_SIM_RES_SUCCESS;
    char imsi_str[UAL_TELE_SIM_IMSI_MAX_LEN+1] = {0};

    TRACE_SFR_CMCC("enter");
    SCI_MEMCPY(dm_config.szCMEI_IMEI, (char *)"CMEI_IMEI", strlen((char *)"CMEI_IMEI"));
    SCI_MEMCPY(dm_config.szIMSI, (char *)"IMSI", strlen((char *)"IMSI"));
    SCI_MEMCPY(dm_config.szDMv, (char *)"v2.0", strlen((char *)"v2.0"));
    SCI_MEMCPY(dm_config.szAppKey, (char *)"M100000058", strlen((char *)"M100000058"));
    SCI_MEMCPY(dm_config.szPwd, (char *)"s2n7TgPkg3IT2oo51T630F3f6U5XJ6vj", strlen((char *)"s2n7TgPkg3IT2oo51T630F3f6U5XJ6vj"));

    if (TRUE == MNNV_GetIMEISVNEx(0, &imeisvn))
    {
        char ori_imei[(MN_MAX_IMEI_LENGTH << 1) + 1] = {0};
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei, MN_MAX_IMEI_LENGTH << 1, ori_imei);
        TRACE_SFR_CMCC("ori_imei len=%d", strlen(ori_imei));
        SCI_MEMCPY(dm_config.szCMEI_IMEI, &ori_imei[1], strlen(ori_imei) - 1);//first is '*'
    }
    result = ual_tele_sim_get_imsi(SIM_ID_1, imsi_str);
    SCI_MEMCPY(dm_config.szIMSI, imsi_str, strlen(imsi_str));//first is '9'

    ref = genDmRegEndpointName("I", &p_device_name, &dm_config);

    if (0 > ref)
    {
        TRACE_SFR_CMCC("Get device name error from IMEI/IMSI.");
        SFR_CMCC_FreeDeviceName(p_device_name, p_dmupdevice_name, p_dm_pwd);
    }
    ref = genDmUpdateEndpointName("I", &p_dmupdevice_name, &dm_config);

    if (0 > ref)
    {
        TRACE_SFR_CMCC("Get dmupdevice name error from IMEI/IMSI.");
        SFR_CMCC_FreeDeviceName(p_device_name, p_dmupdevice_name, p_dm_pwd);
    }

    p_dm_pwd = malloc(strlen((&dm_config)->szPwd) + 1);
    if (PNULL == p_dm_pwd)
    {
        TRACE_SFR_CMCC("p_dm_pwd malloc fail");
        SFR_CMCC_FreeDeviceName(p_device_name, p_dmupdevice_name, p_dm_pwd);
    }
    SCI_MEMSET(p_dm_pwd, 0x00, strlen((&dm_config)->szPwd) + 1);
    SCI_MEMCPY(p_dm_pwd, (&dm_config)->szPwd, strlen((&dm_config)->szPwd));

    ref = SFR_CMCC_MiplCreate(p_host, p_port, p_device_name, p_dmupdevice_name, p_dm_pwd);
    TRACE_SFR_CMCC("ref = %d", ref);
    if(-1 == ref)
    {
        TRACE_SFR_CMCC("create error");
        SFR_CMCC_FreeDeviceName(p_device_name, p_dmupdevice_name, p_dm_pwd);
        return FALSE;

    }
    if(FALSE == SFR_CMCC_MiplOpen(ref,lifetime,timeout))
    {
        TRACE_SFR_CMCC("register error\n");
        SFR_CMCC_FreeDeviceName(p_device_name, p_dmupdevice_name, p_dm_pwd);
        return FALSE;
    }
    else
    {
        TRACE_SFR_CMCC("register succ\n");
    }
    SFR_CMCC_FreeDeviceName(p_device_name, p_dmupdevice_name, p_dm_pwd);
    return TRUE;
}
/*****************************************************************************/
//  Description :真正进行自注册流程的入口
//  Param :none
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_OpenLwm2m(cmcc_self_register_callback p_callback)
{
    BOOLEAN result = FALSE;
    if(PNULL == p_callback)
    {
        TRACE_SFR_CMCC("p_callback is pnull\n");
        return;
    }
    if(PNULL != s_sfr_cmcc_callback)
    {
        TRACE_SFR_CMCC("p_callback is not pnull\n");
        return;
    }
    if(TRUE == s_sfr_register_succ)
    {
        TRACE_SFR_CMCC("register has done\n");
    }
    else
    {
        result = SFR_CMCC_Register();
        TRACE_SFR_CMCC("result = %d\n", result);
        s_sfr_cmcc_callback = p_callback;
    }
    return;
}

