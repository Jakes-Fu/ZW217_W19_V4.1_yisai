/*****************************************************************************
** File Name:      mmistk_app.c                                                  *
** Author:                                                                   *
** Date:           08/2004                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2004         Allen

******************************************************************************/


/**--------------------------------------------------------------------------*
Include Files      
**--------------------------------------------------------------------------*/
#include "mmi_app_stk_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mn_type.h"
#include "simat_data_object.h"
#include "simat_proactive.h"
#include "simat_api.h"
#include "sig_code.h"
#include "mmistk_export.h"
#include "mmistk_app.h"
#include "mmistk_win.h"
#include "mmistk_id.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiphone_export.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmisms_export.h"

/**--------------------------------------------------------------------------*
                                            MACRO DEFINITION
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
                                            STATIC DEFINITION
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
                                            TYPE AND CONSTANT
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
                                            GLOBAL DEFINITION
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
                                            EXTERNAL DECLARE
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
                                            LOCAL FUNCTION DECLARE
**--------------------------------------------------------------------------*/

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : Check current card is CMCC card or not
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL BOOLEAN MMISTK_IsCMCCSIMCard(MN_DUAL_SYS_E dual_sys);
#endif

/*===============================================================================
                                            IMPLEMENTATION
===============================================================================*/

/********************************************************************************
NAME:			MMIAPISTK_AppInit
DESCRIPTION:	initialize the application 's message handler
PARAM IN:		None
PARAM OUT:		None
AUTHOR:		allen
DATE:			2004.08.17
********************************************************************************/
PUBLIC void MMIAPISTK_AppInit(void)
{
    MMISTK_AppInit();
    MMISTK_RegWinIdNameArr();
}

/********************************************************************************
NAME:			MMIAPISTK_IsStkCard
DESCRIPTION:	whether STK card
PARAM IN:		None
PARAM OUT:		
AUTHOR:		allen
DATE:			2004.08.18
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsStkCard(MN_DUAL_SYS_E dual_sys)
{
    return MMISTK_IsStkCard(dual_sys);
}

/********************************************************************************
NAME:			MMIAPISTK_IsSendSms
DESCRIPTION:	whether send SMS by STK
PARAM IN:		None
PARAM OUT:		
AUTHOR:		allen
DATE:			2004.11.18
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsSendSms(void)
{
    return MMISTK_IsSendSms();
}

/********************************************************************************
NAME:			MMIAPISTK_SendSMSPDUCnf
DESCRIPTION:	send SMS Cnf to STK layer
PARAM IN:		
PARAM OUT:		None
AUTHOR:		allen
DATE:			2004.09.01
********************************************************************************/
void MMIAPISTK_SendSMSPDUCnf(MN_DUAL_SYS_E dual_sys, MN_SMS_CAUSE_E send_result)
{
    MMISTK_SendSMSPDUCnf(dual_sys, send_result);
}

/********************************************************************************
NAME:			MMIAPISTK_EnterToplevelMenu
DESCRIPTION:	enter STK top level menu from main menu
PARAM IN:		None
PARAM OUT:		None
AUTHOR:		allen
DATE:			2004.08.17
********************************************************************************/
void MMIAPISTK_EnterToplevelMenu(MN_DUAL_SYS_E dual_sys)
{
    if (MMIAPISET_GetFlyMode()
        || SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
        || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }

    if(!MMIAPISMS_IsOrderOk())
    {
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
        return;
    }

    MMK_CloseMiddleWin(MMISTK_WIN_ID_START, MMISTK_MAX_WIN_ID);

    MMISTK_EnterToplevelMenu(dual_sys);
}

/*****************************************************************************/
// 	Description : info sim card the idle screen available
//	Global resource dependence : 
//   Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_CnfIdleScrAva(void)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        SIMAT_IndIdleScrAvaEx(i);
    }

    //SCI_TRACE_LOW:"[MMI_STK]:MMIAPISTK_CnfIdleScrAva"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_APP_145_112_2_18_2_55_50_0,(uint8*)"");
    return TRUE;
}

/*****************************************************************************/
// 	Description : info sim card the language type
//	Global resource dependence : 
//   Author: songbin.zeng
//	Note:@ISO639-1
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_CnfSimLangSel(void)
{
    SIMAT_LANG_SELECTION_IND_T	sel_lang = {0};
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    MMISTK_GetStkLang(&sel_lang.lang);

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        SIMAT_IndLangSelEx(i, &sel_lang);
    }
    
    //SCI_TRACE_LOW:"[MMI_STK]:MMIAPISTK_CnfSimLangSel"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_APP_165_112_2_18_2_55_51_1,(uint8*)"");
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : info sim card the user active
//	Global resource dependence : 
//   Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_CnfSimUserAct(void)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        SIMAT_IndUserActEx(i);
    }
    
    return TRUE;
}

/********************************************************************************
NAME:			MMIAPISTK_OnSendRegisterPaswdCnf
DESCRIPTION:	
PARAM IN:		
PARAM OUT:		None
AUTHOR:		gang.tong
DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMIAPISTK_OnSendRegisterPaswdCnf(void *arg)
{
    return MMISTK_OnSendRegisterPaswdCnf(arg);
}

/********************************************************************************
NAME:			MMIAPISTK_OnSendSSCnf
DESCRIPTION:	
PARAM IN:		
PARAM OUT:		None
AUTHOR:		gang.tong
DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMIAPISTK_OnSendSSCnf(void *arg)
{
    return MMISTK_OnSendSSCnf(arg);
}

/********************************************************************************
 NAME:      MMIAPISTK_OnRelUSSD
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_OnRelUSSD(MN_DUAL_SYS_E dual_sys)
{
    return MMISTK_OnRelUSSD(dual_sys);
}

/********************************************************************************
 NAME:      MMIAPISTK_IsInSTK
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsInSTK(void)
{
    return MMK_IsOpenWin(MMISTK_MAINMENU_WIN_ID);
}

/********************************************************************************
NAME:			MMISTK_OnSendUSSCnf
DESCRIPTION:	
PARAM IN:		
PARAM OUT:		None
AUTHOR:		gang.tong
DATE:			2005.11.23
********************************************************************************/
BOOLEAN MMIAPISTK_OnSendUSSDCnf(void *arg)
{
    return MMISTK_OnSendUSSDCnf(arg);
}

/*****************************************************************************/
// 	Description : MMIAPISTK_IsStkRefresh
//	Global resource dependence : 
//   Author: wancna.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsStkRefresh(void)
{
    return MMISTK_IsStkRefresh();
}

/********************************************************************************
 NAME:      MMISTK_SetupCall
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_SetupCall(MN_DUAL_SYS_E dual_sys, 
                                    BOOLEAN is_oper_cnf,
                                    BOOLEAN is_call_active,
                                    MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
    return MMISTK_SetupCall(dual_sys, is_oper_cnf, is_call_active, disc_cause);
}

/********************************************************************************
 NAME:      MMIAPISTK_BrowserTermination
 DESCRIPTION:
 AUTHOR: kun.yu
 DATE:		
********************************************************************************/
PUBLIC void MMIAPISTK_BrowserTermination(MN_DUAL_SYS_E dual_sys)
{
    MMISTK_BrowserTermination(dual_sys);
}

/*****************************************************************************/
//     Description : Get Stk Name
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_GetStkName(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *stk_name_ptr)
{
#ifdef CMCC_UI_STYLE
    MMI_GetLabelTextByLang(TXT_COMMON_MAIN_STK, stk_name_ptr);
    return  TRUE;
#else
    return MMISTK_GetStkName(dual_sys, stk_name_ptr);
#endif
}

/********************************************************************************
NAME:                  MMISTK_AppHandlePsMsg
DESCRIPTION:	
PARAM IN:
PARAM OUT:
AUTHOR:                allen
DATE:                  2004.08.17
********************************************************************************/
PUBLIC MMI_RESULT_E MMISTK_AppHandlePsMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"[MMI_STK] enter MMISTK_AppHandlePsMsg: msg_id = 0x%04X, APP_MN_STK_SIGNAL_START =0x%04X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_APP_293_112_2_18_2_55_51_2,(uint8*)"dd", msg_id, APP_MN_STK_SIGNAL_START);
#ifdef ZDT_APP_SUPPORT
    return MMI_RESULT_FALSE;
#endif
    switch (msg_id) 
    {
    case APP_MN_SIMAT_DISPLAY_TEXT_IND:
        MMISTK_OnDisplayTextCmd(param);
        break;
        
    case APP_MN_SIMAT_GET_INKEY_IND:
        MMISTK_OnGetInkeyCmd(param);
        break;
        
    case APP_MN_SIMAT_GET_INPUT_IND:
        MMISTK_OnGetInputCmd(param);
        break;
        
    case APP_MN_SIMAT_MORE_TIME_IND:
        MMISTK_OnMoreTimeCmd(param);
        break;
        
    case APP_MN_SIMAT_SETUP_MENU_IND:
        MMISTK_OnSetupMenuCmd(param);
        break;
        
    case APP_MN_SIMAT_SETUP_CALL_IND:
        MMISTK_OnSetupCallCmd(param);
        break;
        
    case APP_MN_SIMAT_SELECT_ITEM_IND:
        MMISTK_OnSelectItemCmd(param);
        break;
        
    case APP_MN_SIMAT_SEND_SMS_IND:
        MMISTK_OnSendSmsCmd(param);
        break;
        
    case APP_MN_SIMAT_SEND_SS_IND:
        MMISTK_OnSendSSCmd(param);
        break;
        
    case APP_MN_SIMAT_SEND_USSD_IND:
        MMISTK_OnSendUSSDCmd(param);
        break;
        
    case APP_MN_SIMAT_PLAY_TONE_IND:
        MMISTK_OnPlayToneCmd(param);
        break;
        
    case APP_MN_SIMAT_PRO_LOCAL_INFO_IND:
        MMISTK_OnProvideLICmd(param);
        break;
        
    case APP_MN_SIMAT_REFRESH_IND:
        MMISTK_OnRefreshCmd(param);
        break;
        
    case APP_MN_SIMAT_REFRESH_CNF:
        MMISTK_OnRefreshCnf(param);
        break;    
        
    case SIMAT_SETUP_EVENT_LIST_REQ:
        MMISTK_OnSetupEventlistCmd(param);
        break;
        
    case APP_MN_SIMAT_SETUP_IDLE_TEXT_IND:
        MMISTK_OnSetupIdletextCmd(param);
        break;
        
    case APP_MN_SIMAT_SEND_DTMF_IND:
        MMISTK_OnSendDtmfCmd(param);
        break;
        
    case APP_MN_SIMAT_LANG_NOTIFY_IND:
        MMISTK_OnLangNotifyCmd(param);
        break;

    case APP_MN_SIMAT_CC_TO_MMI_IND:
        MMISTK_OnCcToMmiCmd(param);
        break;

    case APP_MN_SIMAT_SMS_CTL_TO_MMI_IND:
        MMISTK_OnSmsCtlToMmiCmd(param);
        break;
        
    case SIMAT_MO_SS_CONTROL_RSP:
        MMISTK_OnSSControlRspCmd(param);
        break;
        
    case SIMAT_MO_USSD_CONTROL_RSP:
        MMISTK_OnUSSDControlRspCmd(param);
        break;

    case SIMAT_SMS_PP_RSP:
        MMISTK_OnSMSP2PRspCmd(param);
        break;
        
    case SIMAT_SMS_CB_RSP:
        MMISTK_OnSMSCBRspCmd(param);
        break;
        
    case APP_MN_SIMAT_LAUNCH_BROWSER_IND:
        MMISTK_OnLaunchBrowserCmd(param);
        break;
        
    case SIMAT_SESSION_TERMINATED_IND:
        MMISTK_OnSessionTerminateCmd(param);
        break;

    case APP_SIMAT_OPEN_CHANNEL_IND:
        MMISTK_OnOpenChannelCmd(param);
        break;

    case APP_SIMAT_CLOSE_CHANNEL_IND:
        MMISTK_OnCloseChannelCmd(param);
        break;

    case APP_SIMAT_GET_CHANNEL_STATUS_IND:
        MMISTK_OnGetChannelStatusCmd(param);
        break;

    case APP_MN_SIMAT_SEND_DATA_IND:
        MMISTK_OnSendDataCmd(param);
        break;

    case APP_MN_SIMAT_RECEIVE_DATA_IND:
        MMISTK_OnReceiveDataCmd(param);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    //SCI_TRACE_LOW:"[MMI_STK] leave MMISTK_AppHandlePsMsg: result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTK_APP_425_112_2_18_2_55_51_3,(uint8*)"d", result);
    
    return (result);
}

/*****************************************************************************/
// 	Description : Get STK card existed status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPISTK_GetStkCardNum(uint16 *idx_ptr, uint16 array_len)
{
#if 0
    return MMIAPISTK_GetCMCCStkCardNum(idx_ptr, array_len);
#else
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMISTK_IsStkCard(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
#endif
}


#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : Check current card is CMCC card or not
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL BOOLEAN MMISTK_IsCMCCSIMCard(MN_DUAL_SYS_E dual_sys)
{
    PHONE_PLMN_NAME_E  plmn = PLMN_TEST;

    plmn = MMIAPIPHONE_GetNetworkName(dual_sys);

    if ( PLMN_CMCC == plmn)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : Get CMCC STK card existed status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32  MMIAPISTK_GetCMCCStkCardNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMISTK_IsStkCard(i) &&MMISTK_IsCMCCSIMCard(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}
#endif

/*****************************************************************************/
//  Description : Select SIM API of STK app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISTK_SelectSIMFunc(void)
{
    return MMISTK_SelectSIMFunc();
}

/*****************************************************************************/
//  Description : get type of STK refresh
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC SIMAT_REFRESH_TYPE_E MMIAPISTK_GetStkRefreshType(MN_DUAL_SYS_E dual_sys)
{
    return MMISTK_GetStkRefreshType(dual_sys);
}

/*****************************************************************************/
//  Description : set type of STK refresh
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISTK_SetStkRefreshType(MN_DUAL_SYS_E dual_sys, int value)
{
    MMISTK_SetStkRefreshType(dual_sys, value);
}

/*****************************************************************************/
//  Description : close refresh waiting win
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISTK_CloseRefreshWaitingWin(void)
{
    MMISTK_CloseRefreshWaitingWin();
}
