/*****************************************************************************
** File Name:      mmimsd.c                                                   *
** Author:         Phoebe.Chen                                                            *
** Date:           15/11/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe multimedia                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2004       Phoebe.Chen       Create
******************************************************************************/
#include "mmi_app_msd_trc.h"
#if defined(MMI_MSD_SUPPORT)
#define _MSD_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guitext.h"
#include "mmi_text.h"
#include "mmi_image.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "mmialarm_export.h"
#include "mmimsd_export.h"
#include "mn_api.h"
#include "mmiphone_export.h"
#include "mmicc_export.h"

#ifndef WIN32
#include "arm_reg.h"
#endif

#include "mmisms_export.h"
#include "mmimsd_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MAX_MSD_MSG_LEN		100
#define MAX_RESEND_TIMES    10
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMIMSD_NV_INFO_T g_mmimsd_current_nv;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_msd_send_sms;		//是否等待发地下短信，可能是产生这样一个要求未处理
LOCAL uint32 s_hour_count;			//小时发计数，开机满24小时计数满，发送一次。
LOCAL uint16 s_minute_count;
LOCAL uint16 s_retry_count;
LOCAL BOOLEAN s_delay_msd_check;
LOCAL BOOLEAN s_is_illegal_use = FALSE;		//卡是非法卡，而且开机输入密码还错了，表示不是机主或者机主的朋友用手机
LOCAL BOOLEAN s_sim_legal[MMI_DUAL_SYS_MAX];//卡是否是正常卡
const uint16 retry_timer[MAX_RESEND_TIMES] = {1,2,5,10,30,60,60,60,60,60};//间隔总时间不超过24HR

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : msd send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MsdSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm);

/**--------------------------------------------------------------------------*
**                             FUNCTION                               *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsPswRight(MMI_STRING_T *psw_string)
{
    if(PNULL != psw_string && 0 == MMIAPICOM_CompareTwoWstr(psw_string->wstr_ptr, psw_string->wstr_len, g_mmimsd_current_nv.psw, MMIMSD_PSW_LEN))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_Init(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //初始化全局变量g_mmimsd_current_nv	
    //和静态变量s_is_msd_send_sms  ， s_hour_count
    
    SCI_MEMSET(&g_mmimsd_current_nv, 0, sizeof(g_mmimsd_current_nv));
    
    MMINV_READ(MMINV_MSD_SETTING_INFO, &g_mmimsd_current_nv, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&g_mmimsd_current_nv, 0, sizeof(g_mmimsd_current_nv));
        //write to NV
        MMINV_WRITE(MMINV_MSD_SETTING_INFO, &g_mmimsd_current_nv); 
    }
    s_is_illegal_use = FALSE; //开机的时候先设置为不是非法卡，等待SIM READY再检测
    s_is_msd_send_sms = FALSE;
    s_hour_count = 0;
    s_delay_msd_check = FALSE;
    s_retry_count = 0;
    s_minute_count = 0;
    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMSD_SetFactoryValue(void)//@zhaohui,cr95392
{
    SCI_MEMSET(&g_mmimsd_current_nv,0,sizeof(MMIMSD_NV_INFO_T));
    //write to NV
    MMINV_WRITE(MMINV_MSD_SETTING_INFO, &g_mmimsd_current_nv); 
    
    s_is_illegal_use = FALSE; //开机的时候先设置为不是非法卡，等待SIM READY再检测
    s_is_msd_send_sms = FALSE;
    s_hour_count = 0;
    s_delay_msd_check = FALSE;
    s_retry_count = 0;
    s_minute_count = 0;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SetInfoToNv(MMIMSD_NV_INFO_T *msd_info_ptr)
{    
    if (PNULL != msd_info_ptr)
    {
        MMINV_WRITE(MMINV_MSD_SETTING_INFO, msd_info_ptr);
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_SimLegalityCheck(void)
{
    if (MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID))
    {
        return;
    }
    
    if((MMIAPIMSD_IsMsdStartup()) && MMIAPIPHONE_GetStartUpCondition() == STARTUP_NORMAL)//正常开机才算
    {
        if(!MMIMSD_IsSimLegal())
        {
            if(g_mmimsd_current_nv.count >= MMIMSD_MAX_MSG_COUNT)
            {
                MMIMSD_OpenShutOffWin();
            }
            else
            {
                MMIMSD_OpenInitInputPswWin();
            }
        }
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMSD_IsInNVImsiSet(MN_IMSI_T *input_imsi)
{
    uint32 i = 0;
    BOOLEAN ret = FALSE;

    for(i = 0; i<MMI_DUAL_SYS_MAX; i++)
    {
        if(0 == GUI_StringCompare( input_imsi->imsi_val, input_imsi->imsi_len, g_mmimsd_current_nv.msd_imsi[i].imsi_val, g_mmimsd_current_nv.msd_imsi[i].imsi_len))
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsSimLegal(void)
{
    MN_IMSI_T   imsi_str = {0};
    uint32      i = 0;

    SCI_MEMSET(s_sim_legal, 0x00, sizeof(s_sim_legal));
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
#ifdef WIN32
        imsi_str.imsi_len = 6;
        if (i == 0)
        {
            strncpy((char*)imsi_str.imsi_val,(const char*)"543267",imsi_str.imsi_len);
        }
        else
        {
            strncpy((char*)imsi_str.imsi_val,(const char*)"654321",imsi_str.imsi_len);
        }
#else
        imsi_str = MNSIM_GetImsiEx(i);
#endif

        if(!MMIAPIPHONE_GetSimExistedStatus(i))
        {
            s_sim_legal[i] = TRUE;
            //SCI_TRACE_LOW:"MMIMSD_IsSimLegal s_sim_legal[%d]=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMSD_220_112_2_18_2_40_55_0,(uint8*)"dd", i, s_sim_legal[i] );
            continue;
        }

        s_sim_legal[i] = MMIMSD_IsInNVImsiSet(&imsi_str);
        
        //SCI_TRACE_LOW:"MMIMSD_IsSimLegal s_sim_legal[%d]=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMSD_226_112_2_18_2_40_55_1,(uint8*)"dd", i, s_sim_legal[i] );
    }
    
    
    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(!s_sim_legal[i])
        {
            return FALSE;
        }
    }
    
    return TRUE;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIMSD_IsMsdStartup(void)
{
    if (g_mmimsd_current_nv.open & MMIMSD_OPEN_MASK)
    {
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_ResetSendMsgCount(void)
{
    s_is_msd_send_sms = FALSE;
    g_mmimsd_current_nv.count = 0;
    g_mmimsd_current_nv.open &= (~MMIMSD_TRY_TIME_MASK);
    MMIMSD_SetInfoToNv(&g_mmimsd_current_nv);
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_ResetErrorInputPswCount(void)
{
    g_mmimsd_current_nv.open &= (~MMIMSD_TRY_TIME_MASK); 
    MMIMSD_SetInfoToNv(&g_mmimsd_current_nv);
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IfPswWrongTooMany(void)
{
    if((g_mmimsd_current_nv.open & MMIMSD_TRY_TIME_MASK) >= MMIMSD_MAX_PSW_TRY_COUNT)
    {
        g_mmimsd_current_nv.open &= (~MMIMSD_TRY_TIME_MASK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
// 	Description : 
//	Global resour1ce dependence :                                
//      Author: Phoebe.chen
//	Note: mode is 1 ,means active msd, else deactive msd
/*****************************************************************************/
void MMIMSD_MsdSetNewInfo(uint8* tel_num, 
                          uint8 tel_len,
                          MMIMSD_FUNC_E mode)
{
    MN_IMSI_T   imsi = {0};
    uint8		len = 0;
    uint32      i = 0;
    
    if (MMIMSD_SETUP == mode)    // activate MSD service
    {
        SCI_MEMSET(g_mmimsd_current_nv.relate_tel, 0, sizeof(g_mmimsd_current_nv.relate_tel));
        if (tel_num != PNULL)
        {
            MMI_MEMCPY(g_mmimsd_current_nv.relate_tel, MMIMSD_MAX_TEL_MUM, tel_num, tel_len, tel_len);
        }
        g_mmimsd_current_nv.relate_tel_len = tel_len;
        g_mmimsd_current_nv.open = MMIMSD_OPEN_MASK;   // clear error count AND START UP THIS FUN
        g_mmimsd_current_nv.count = 0;
        
        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
#ifdef WIN32
            imsi.imsi_len = 6;
            if ( i == 0)
            {
                strncpy((char*)imsi.imsi_val, (const char*)"654321",imsi.imsi_len);
            }
            else
            {
                strncpy((char*)imsi.imsi_val, (const char*)"543267",imsi.imsi_len);
            }
#else
            imsi = MNSIM_GetImsiEx(i);
#endif
            SCI_MEMSET(&(g_mmimsd_current_nv.msd_imsi[i]), 0, sizeof(MN_IMSI_T));
            len = (imsi.imsi_len > MN_MAX_IMSI_ARR_LEN) ? MN_MAX_IMSI_ARR_LEN : imsi.imsi_len;
            MMI_MEMCPY(g_mmimsd_current_nv.msd_imsi[i].imsi_val, MN_MAX_IMSI_ARR_LEN, imsi.imsi_val, MN_MAX_IMSI_ARR_LEN, len);
            g_mmimsd_current_nv.msd_imsi[i].imsi_len = len;
        }
    }
    else
    {
        g_mmimsd_current_nv.open = 0;                // clear error count
        g_mmimsd_current_nv.count = 0;               // clear sms sending count
    }
    
    MMIMSD_SetInfoToNv(&g_mmimsd_current_nv);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_ClearSmsFlag(void)
{
    s_is_msd_send_sms = FALSE;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_PswWrongPlus(void)
{
    uint8 psw_count = 0;
    uint8 msd_open = 0;
    psw_count = g_mmimsd_current_nv.open & MMIMSD_TRY_TIME_MASK;
    msd_open = g_mmimsd_current_nv.open & MMIMSD_OPEN_MASK;
    psw_count++;
    g_mmimsd_current_nv.open = psw_count | msd_open;//再保存多少次
    MMIMSD_SetInfoToNv(&g_mmimsd_current_nv);
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsPswNotNull(void)
{
    if (MMIAPICOM_Wstrlen(g_mmimsd_current_nv.psw) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_UpdatePsw(wchar *new_psw)
{
    
    //PSW IS TOO SHORT
    if (MMIAPICOM_Wstrlen(new_psw) != MMIMSD_PSW_LEN)  
    {
        return FALSE;
    }	
    
    MMI_WSTRNCPY(g_mmimsd_current_nv.psw, (MMIMSD_PSW_LEN + 2), new_psw, MMIMSD_PSW_LEN, MMIMSD_PSW_LEN);
    g_mmimsd_current_nv.count = 0;
    
    MMIMSD_SetInfoToNv(&g_mmimsd_current_nv);
    
    return TRUE;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_HourCheck(void)
{
    if(!MMIAPIMSD_IsMsdStartup())
    {
        return;
    }
    if(!MMIMSD_IfIllegalUse())
    {
        return;
    }
    if (MMIAPIPHONE_GetSimCardOkNum(PNULL, 0) == 0)
    {
        s_delay_msd_check = TRUE;
        return;
    }
    
    //非法卡，msd启动了,而且此时SIM有效
    if( s_hour_count < MMIMSD_HOUR_MAX_COUNT)
    {
        s_hour_count++;
        return;
    }
    else
    {
        s_is_msd_send_sms = TRUE;
        s_minute_count = 0;
        s_retry_count = 0;
        s_hour_count = 0;
    }
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_MinuteCheck(void)
{
    if(!MMIAPIMSD_IsMsdStartup())
    {
        return;
    }
    if(!MMIMSD_IfIllegalUse())
    {
        return;
    }
    if (s_delay_msd_check) //因为sim卡没有准备好而delay进行的东西,重新
    {
        MMIAPIMSD_HourCheck();
    }

    if (s_is_msd_send_sms) 
    {
        s_minute_count++;    
    }
    
    if (s_is_msd_send_sms && (s_retry_count < MAX_RESEND_TIMES) && (s_minute_count == retry_timer[s_retry_count])) //有设定的告警短信要发送，或者没有上次没有发送成功
    {

        MMIMSD_SendMsdMsg();
        s_retry_count++;
    }
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SendMsgSuccessCallback(void)
{
    s_is_msd_send_sms = FALSE;
    s_minute_count = 0;
    s_retry_count = 0;
    g_mmimsd_current_nv.count++;
    MMIMSD_SetInfoToNv(&g_mmimsd_current_nv);
    //短信次数超过15次就关机
    if(g_mmimsd_current_nv.count >= MMIMSD_MAX_MSG_COUNT)
    {
        MMIMSD_OpenShutOffWin();
    }
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SendMsgFailCallback(void)
{
    s_minute_count = 0; 
    s_is_msd_send_sms = TRUE;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsSimAvailable(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_dual_sys = FALSE;
    
    is_dual_sys = MMIAPIPHONE_IsSimCardOk(dual_sys);
    
    return is_dual_sys;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SetIllegalUseFlag(BOOLEAN is_illegal)
{
    s_is_illegal_use = is_illegal;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IfIllegalUse(void)
{
    return s_is_illegal_use;
}

/*****************************************************************************/
//  Description : msd send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MsdSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm)
{//dm 不需要做任何处理，直接结束流程
    BOOLEAN result = TRUE;

    if (is_succss)
    {
        MMIMSD_SendMsgSuccessCallback();
    }
    else
    {
        MMIMSD_SendMsgFailCallback();
    }
    return result;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SendMsdMsg(void)
{
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};/*lint !e64*/
    uint8 msg_text[2 * MAX_MSD_MSG_LEN] = {0};
    uint16	msg_length = 0;
    MMI_STRING_T msd_msg_string = {0};
    MMI_STRING_T msd_notify_string = {0};
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    MMISMS_SEND_DATA_T send_data = {0};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 sim_ok_array[MMI_DUAL_SYS_MAX] = {0,};
    uint32 sim_ok_num = 0;
    uint32 i = 0;

    SCI_MEMSET(sim_ok_array, 0x00, sizeof(sim_ok_array));

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(MMIAPIPHONE_IsSimOk((MN_DUAL_SYS_E)i))
        {
            if(!s_sim_legal[i])
            {
                dual_sys = (MN_DUAL_SYS_E)i;
                break;
            }
        
            sim_ok_array[sim_ok_num] = (MN_DUAL_SYS_E)i;
            sim_ok_num ++;
        }
    }

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = (MN_DUAL_SYS_E)(sim_ok_array[0]);   //else use the ok sim. or if no ok, user the sim1.(sim_ok have inited with 0. )
    }
#else
    dual_sys = MN_DUAL_SYS_1;
#endif

    //SCI_TRACE_LOW:"mmimsd.c enter SendMsdMsg dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMSD_576_112_2_18_2_40_56_2,(uint8*)"d", dual_sys);

    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));

    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    sprintf((char*)(msg_text + msg_length), "\nMCC:%d; MNC:%d; CELLID:%d; LAC:%d", \
        plmn_info.mcc, plmn_info.mnc, plmn_info.cell_id, plmn_info.lac);
    
    msg_length = strlen((char*)msg_text);

    MMI_GetLabelTextByLang(TXT_MSD_NOTIFY_INFO, &msd_notify_string);

    msd_msg_string.wstr_ptr = SCI_ALLOC_APP((msg_length + msd_notify_string.wstr_len + 1)*sizeof(wchar));
    SCI_MEMSET((uint8 *)msd_msg_string.wstr_ptr, 0x00, ((msg_length + msd_notify_string.wstr_len + 1)*sizeof(wchar)));

    MMI_WSTRNCPY(msd_msg_string.wstr_ptr,
                            (msg_length + msd_notify_string.wstr_len),
                            msd_notify_string.wstr_ptr,
                            msd_notify_string.wstr_len,
                            msd_notify_string.wstr_len);

    msd_msg_string.wstr_len = msd_notify_string.wstr_len;

    MMI_STRNTOWSTR(msd_msg_string.wstr_ptr + msd_msg_string.wstr_len,
                                msg_length,
                                (uint8 *)msg_text,
                                msg_length,
                                msg_length);

    msd_msg_string.wstr_len += msg_length;

    send_data.dual_sys = dual_sys;
    send_data.sms_content_ptr = &msd_msg_string;
    send_data.dst_port = 0;
    send_data.src_port = 0;
    send_data.dest_addr_ptr = g_mmimsd_current_nv.relate_tel;
    send_data.dest_addr_len = g_mmimsd_current_nv.relate_tel_len;
    send_data.is_need_packet = TRUE;
    send_data.send_callback = MsdSMSSendCnfCallback;
    
    if (!MMIAPISMS_SendSmsForOther(&send_data))
    {
        //SCI_TRACE_LOW:"mmimsd.c MMIMSD_SendMsdMsg send fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMSD_619_112_2_18_2_40_56_3,(uint8*)"");

        MMIMSD_SendMsgFailCallback();
    }

    SCI_FREE(msd_msg_string.wstr_ptr);
    msd_msg_string.wstr_ptr = PNULL;
}
#endif
