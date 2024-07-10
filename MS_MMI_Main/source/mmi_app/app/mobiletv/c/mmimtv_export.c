/*****************************************************************************
** File Name:      mmimtv_export.c                                           *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv app                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#include "mmimtv_export.h"
#ifdef CMMB_SUPPORT
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmi_modu_main.h"
#include "mmimtv_nv.h"
#include "mmimtv_main.h"
#include "mmimtv_internal.h"
#include "mmimtv_id.h"
#include "mmimtv_menutable.h"
#include "mmimtv_osd_display.h"
//#include "dtl_export.h"
#include "mmimtv_text.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmicc_export.h"

#include "Mmk_type.h"
#include "Mmk_msg.h"
#ifdef MBBMS_SUPPORT
#include "mmimbbms_wintab.h"
#include "mmimbbms_main.h"
#endif
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#include "mmiwlan_text.h"
#endif
#if (defined MCARE_V31_SUPPORT)
#include "McfInterface.h"
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*lint -e552 -e765*/
MMI_APPLICATION_T g_mtv_app = {0};
/*lint -e552 -e765*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : MTV app init
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_Init(void)
{
    //初始化APP处理函数 MMIMTV_HandlePSMsg()
    
    g_mtv_app.ProcessMsg = MMIMTV_HandlePSMsg;
    g_mtv_app.component_type = CT_APPLICATION;
    //DTLSVC_Init();
    CMMB_Task_Init();
    
    MMIMTV_InitNetworkNotifyInfo();
}

/*****************************************************************************/
// 	Description : MTV module init
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_InitModule(void)
{
    MMIMTV_RegMenuWin();
    MMIMTV_RegWinIdNameArr();
    MMIMTV_RegMtvNv();
    //MMIMTV_RegSettingNv();  
}

/*****************************************************************************/
// 	Description : open MTV player
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_OpenMTVPlayer(void)
{
    MMISRV_HANDLE_T audio_handle = MMIMTV_GetAudioHandle();
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIAPIMTV_OpenMTVPlayer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_EXPORT_95_112_2_18_2_40_20_596,(uint8*)"");



#ifdef MCARE_V31_SUPPORT
	if(MCareV31_Exit_Confirm())
	{
		return FALSE;	///具体的return值不同，和添加出的函数相关
	}
#endif    

#ifdef MMI_CALL_MINIMIZE_SUPPORT
    //SCI_TRACE_LOW:"[MMIMTV]: MMIAPIMTV_OpenMTVPlayer call running =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_EXPORT_107_112_2_18_2_40_20_597,(uint8*)"d", MMIAPICC_IsInState(CC_IN_CALL_STATE));
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin( TXT_CALL_USING);
        return FALSE;
    }
#endif   

    if(MMIMTV_IsVRunning())
    {
        //SCI_TRACE_LOW:"[MMIMTV]: MMIAPIMTV_OpenMTVPlayer app is opened!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_EXPORT_117_112_2_18_2_40_20_598,(uint8*)"");
        return FALSE;
    }
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }
    
    if(MMIAPIUdisk_VUARTIsRun()) //pc数据通信中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_VUART_USING);
        return FALSE;
    }

#ifdef MMI_WIFI_SUPPORT  
    if(MMIAPIWIFI_GetIsWlanOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMM_HAS_OPEN);
        return FALSE;
    }
#endif

    if(audio_handle > 0)
    {
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
        MMIMTV_SetAudioHandle(0);
    }
    audio_handle = MMIMTV_RequestHandle();
    if(audio_handle > 0)
    {
        MMIMTV_SetAudioHandle(audio_handle);
    }
    else
    {
        return FALSE;
    }
    //mbbms2.0 MBBMS/CMMB共用一套主界面
    MMIMBBMS_EnterMBBMS();

    return TRUE;
}

/*****************************************************************************/
// 	Description : whether mtv player is opened
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsMTVOpen(void)
{
    //mbbms2.0
    return MMIMBBMS_IsMBBMSOpen();
    //return MMIMTV_IsMTVOpen();
}

/*****************************************************************************/
// 	Description : whether 6600V chip is running
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsMTVRunning(void)
{
    //mbbms2.0
    return MMIMBBMS_IsMBBMSOpen();
    //return MMIMTV_IsVRunning();
}

/*****************************************************************************/
// 	Description : whether mtv is playing 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsPlaying(void)
{
    if(MMIMBBMS_IsMBBMSOpen()
        && MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : MMIAPIMTV_Exit
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_Exit(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]:MMIAPIMTV_Exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_EXPORT_207_112_2_18_2_40_20_599,(uint8*)"");
    
    //exit mbbms 
    if(MMK_IsOpenWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID);
    }
    else
    {
        MMIMBBMS_ExitMBBMS();
    }
}

/*****************************************************************************/
// 	Description : quick exit mbbms
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMTV_VTExit(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]:MMIAPIMTV_VTExit "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_EXPORT_226_112_2_18_2_40_20_600,(uint8*)"");
    
    if(MMIAPIMTV_IsMTVOpen())
    {
        if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type)
        {
            //播放中快速停止播放
            MMIMBBMS_QuickStopPlay();
        }
        else
        {
            if(MMIMBBMS_FLOW_MBBMS_ACTIVE != MMIMBBMS_GetFlowInfo()->flow_type &&
				MMIMBBMS_FLOW_SG_PARSE != MMIMBBMS_GetFlowInfo()->flow_type)//mobileTV启动阶段不停止流程  
            MMIMBBMS_StopFlow();
        }
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_SetDelayPowerOff(BOOLEAN is_delay)
{
    MMIMTV_SetDelayPowerOff(is_delay);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_SettingInfoSetFactory(void)
{
    MMIMTV_SettingInfoSetFactory();
}

/*****************************************************************************/
// 	Description : is have unread EB msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsHaveTobeReadEBMsg(void)
{
    return MMIMTV_IsHaveTobeReadEBMsg();
}

/*****************************************************************************/
// 	Description : get tv signal level value 
//	Global resource dependence : none
//  Author: xin.li
//	Note: level 0-5
//        MBBMS开启时获取TV信号强度，否则返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMTV_GetTVSignalLevel(uint8 *signal_value)
{
    if(PNULL == signal_value)
    {
        return FALSE;
    }
    
    if(MMIMTV_IsMTVOpen()
#ifdef MBBMS_SUPPORT
        || MMIMBBMS_IsMBBMSOpen()
#endif 
        )
    {
        if(MMIMTV_IsVRunning())
        {
            *signal_value = MMIMTV_GetTVSignalLevel();
        } 
        else
        {
            *signal_value = 0;
        }
        return TRUE;
    }
    else
    {
        //mbbms已关闭
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : get signal icon id for status bar
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIMTV_GetTVStatusImageID(uint32 signal)
{
    return MMIMTV_GetTVStatusImageID(signal);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_OpenEBList(void)
{
    //处理EB的回调函数
}

/*****************************************************************************/
// 	Description : MBBMS pre-exit callback function
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_PreExitCallback(void)
{
#ifdef MBBMS_SUPPORT
    MMIMBBMS_PreExitCallback();
#endif
}

#else
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_Init(void)
{
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_InitModule(void)
{
}

/*****************************************************************************/
// 	Description : open MTV player
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_OpenMTVPlayer(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : IsMTVRunning
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsMTVRunning(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : whether mtv is playing 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsPlaying(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_Exit(void)
{
    
}

/*****************************************************************************/
// 	Description : close v-chip first when exit, called by CC when vt come in
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_VTExit(void)
{

}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_SetDelayPowerOff(BOOLEAN is_delay)
{

}
/*****************************************************************************/
// 	Description : is mtv player open
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsMTVOpen(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_SettingInfoSetFactory(void)
{
}

/*****************************************************************************/
// 	Description : is have unread EB msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsHaveTobeReadEBMsg(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : get tv signal level value 
//	Global resource dependence : none
//  Author: xin.li
//	Note: level 0-5
//        只有V Chip工作时才能获得TV信号，否则返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMTV_GetTVSignalLevel(uint8 *signal_value)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : get signal icon id for status bar
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIMTV_GetTVStatusImageID(uint32 signal)
{
    return 0;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_OpenEBList(void)
{
}

/*****************************************************************************/
// 	Description : MBBMS pre-exit callback function
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_PreExitCallback(void)
{
}
#endif


