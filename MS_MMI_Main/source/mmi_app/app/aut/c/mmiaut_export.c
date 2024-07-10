/****************************************************************************
** File Name:      mmiaut_api.c
** Author:                                                                  
** Date:           2010/11/01
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is mmi auto update time api file. AUT is short for auto update time
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 11/01/2010        xiaoming             Create
** 01/20/2012        dave.ruan            modify
** 
****************************************************************************/

#include "mmi_app_aut_trc.h"
#ifdef  MMI_UPDATE_TIME_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiaut_export.h"
#include "mmiaut_internal.h"
#include "mmicom_time.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*是否处理带网络时间信息的netstatusind ,以更新手机当前的系统时间目的:不频繁更新网络时间为系统时间*/
LOCAL BOOLEAN   s_update_time_by_netstatusind          = FALSE;
LOCAL BOOLEAN   s_is_manual_update_time_running        = FALSE;      /*正在手动更新时间*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : set 是否处理本次的网络时间将其更新到系统时间上
//	Global resource dependence : s_update_time_by_netstatusind
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_SetIsNeedUpdateTimeByNet(BOOLEAN is_need_update_time)
{
    SCI_TRACE_LOW("MMIAPIAUT_SetIsNeedUpdateTimeByNet:: is_need_update_time = %d", is_need_update_time);
    s_update_time_by_netstatusind = is_need_update_time;
}

/*****************************************************************************/
// 	Description : get s_update_time_by_netstatusind
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_GetIsNeedUpdateTimeByNet(void)
{
    //SCI_TRACE_LOW:"MMIAPIAUT_GetIsNeedUpdateTimeByNet:::s_update_time_by_netstatusind = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_EXPORT_61_112_2_18_2_5_21_0,(uint8*)"d",s_update_time_by_netstatusind);
    return s_update_time_by_netstatusind;
}

/*****************************************************************************/
// 	Description : 是否正在手动更新时间
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_IsManualUpdateTimeRunning(void)
{
    //SCI_TRACE_LOW:"[MMIAUT] s_is_manual_update_time_running= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_EXPORT_71_112_2_18_2_5_21_1,(uint8*)"d",s_is_manual_update_time_running);
    return s_is_manual_update_time_running;
}

/*****************************************************************************/
// 	Description : 设置正在手动更新时间标志
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAUT_SetManualUpdateTimeRunning(BOOLEAN is_running)
{
    //SCI_TRACE_LOW:"[MMIAUT] s_is_manual_update_time_running = s_running =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_EXPORT_81_112_2_18_2_5_21_2,(uint8*)"d",is_running);
    s_is_manual_update_time_running = is_running;
}

/*****************************************************************************/
// 	Description : get need update time enable
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_GetNeedUpdateTimeEnable(void)
{
    BOOLEAN result = FALSE;
    
    if (MMIAPISET_GetIsAutoUpdateTime() || MMIAPIAUT_IsManualUpdateTimeRunning())
    {
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 关闭等待窗口
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIAUT_CloseManualUpdateWaitWin(void)
{
    MMI_WIN_ID_T wait_win_id          = MMIAUT_GetManualUpdateWaitWinID();

    if(MMIAPIAUT_IsManualUpdateTimeRunning())
    {
        MMIAUT_SetManualUpdateTimeRunning(FALSE);
    }

    if (MMK_IsOpenWin(wait_win_id))
    {
        MMK_CloseWin(wait_win_id);
        
        if (!MMIAUT_IsUserCancel())
        {
            /*点亮背光*/
            MMIDEFAULT_TurnOnLCDBackLight();
        }
    }
}

/*****************************************************************************/
// 	Description : 自动更新时间相关参数初始化
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_AutoUpdateTimeInit(void)
{
    MMIAUT_SetManualUpdateTimeRunning(FALSE);
    //如果开关打开, 每次开机都需要初始化自动更新flag
    if(MMIAPISET_GetIsAutoUpdateTime())
    {
        MMIAPIAUT_SetIsNeedUpdateTimeByNet(TRUE);
    }
    //初始化参数
    MMIAUT_UpdateTimeInit();
}

#ifdef MMI_NET_TIME_SUPPORT
/*****************************************************************************/
// 	Description : 从网络信息header中提取时间信息，然后设置到系统
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_GetTimeFromRecvData(const char* header_buf, int32 content_length)
{
    BOOLEAN ret = FALSE;
    
    //SCI_TRACE_LOW:"[MMIAUT]::: MMIAPIAUT_GetTimeFromRecvData(), content_length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_EXPORT_151_112_2_18_2_5_21_3,(uint8*)"d", content_length);

    if (MMIAPIAUT_GetIsNeedUpdateTimeByNet() && content_length > NET_TIME_STRING_LEN)
    {
        if (MMIAPISET_GetIsAutoUpdateTime())
        {
            uint32 net_second = 0;
            if (MMIAPIAUT_FindNetTimeInNetBuf((char*)header_buf, content_length, &net_second))
            {
                MMIAPIAUT_SetIsNeedUpdateTimeByNet(FALSE);
                //校准时间
                MMIAUT_UpdateSysTimeByNetTM(net_second);
                ret = TRUE; 
            }
        }
        else
        {
            MMIAPIAUT_SetIsNeedUpdateTimeByNet(FALSE);
        }
    }
    return ret;
}    
#endif/*MMI_NET_TIME_SUPPORT*/

#ifdef MMI_SNTP_SUPPORT
/*****************************************************************************/
// 	Description : MMIAPIAUT_ManualUpdateTimeHandleSNTPcnf
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:获取MM infor 后的处理，只有自动处理，不会手动更新
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIAUT_ManualUpdateTimeHandleSNTPcnf( MMI_MESSAGE_ID_E msg_id,void * param)
{   
    return MMIAUT_HandleSNTPCnfMsg(msg_id, param);
}

/*****************************************************************************/
// 	Description : MMIAPIAUT  Start Manual Update System Time
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:开始使用SNTP手动更新时间
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIAUT_StartManualUpdateSystemTime( void)
{   
    return MMIAUT_StartManualUpdateSystemTime();
}
#endif
#endif/* MMI_UPDATE_TIME_SUPPORT*/
