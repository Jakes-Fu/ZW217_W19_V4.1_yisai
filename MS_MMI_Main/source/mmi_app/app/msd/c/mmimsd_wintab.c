/*****************************************************************************
** File Name:      mmimsd_wintab.c                                                   *
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
#if defined(MMI_MSD_SUPPORT)
#define _MSD_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mn_type.h"
#include "mn_api.h"
#include "tb_dal.h"
//#include "efs.h"
#include "mmk_type.h"
#include "mmk_msg.h"
#include "mmk_timer.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "mmialarm_export.h"
#include "mmimsd_export.h"
#include "mmiphone_export.h"
#include "mmiidle_export.h"
#include "mmipub.h"
#include "guiedit.h"
//#include "guires.h"
#include "guictrl_api.h"
#include "mmimsd_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

LOCAL wchar* first_input_psw_ptr = NULL;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern MMIMSD_NV_INFO_T	g_mmimsd_current_nv;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdInitInputPswWinMsg (
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 );
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdSetupInputPswWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 );

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdSetupInputInfoTelWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     );
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdCloseInputPswWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 );
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdModPswInputOldPswWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     );

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdModPswInputNewPswWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     );

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdModPswInputPswAgainWinMsg(
                                                       MMI_WIN_ID_T win_id, 
                                                       MMI_MESSAGE_ID_E msg_id, 
                                                       DPARAM param
                                                       );
#if 0
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdShutoffWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
#endif
/*****************************************************************************/
// 	Description : to handle alert window closed message
//	Global resource dependence : 
//  Author:Tracy Zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShutoffAlertClosedMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : handle the message of
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/


/**--------------------------------------------------------------------------*
WINDOW  DEFINITION    
**--------------------------------------------------------------------------*/
WINDOW_TABLE( MMIMSD_INIT_INPUT_PSW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdInitInputPswWinMsg ), 
    WIN_ID( MMIMSD_INIT_INPUT_PSW_WIN_ID ),
    WIN_TITLE(TXT_MSD_INPUT_MSD_PSW),
    CREATE_EDIT_PASSWORD_CTRL(MMIMSD_PSW_LEN, MMIMSD_INIT_INPUT_PSW_PASSWORD_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIMSD_SETUP_INPUT_PSW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdSetupInputPswWinMsg ), 
    WIN_ID( MMIMSD_SETUP_INPUT_PSW_WIN_ID ),
    WIN_TITLE(TXT_MSD_INPUT_PASSWORD),
    CREATE_EDIT_PASSWORD_CTRL(MMIMSD_PSW_LEN, MMIMSD_SETUP_INPUT_PSW_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIMSD_SETUP_INPUT_INFO_TEL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdSetupInputInfoTelWinMsg ), 
    WIN_ID( MMIMSD_SETUP_INPUT_INFO_TEL_WIN_ID ),
    WIN_TITLE(TXT_MSD_INPUT_INFO_NUM),
    CREATE_EDIT_PHONENUM_CTRL(MMIMSD_MAX_TEL_MUM , MMIMSD_SETUP_INPUT_INFO_TEL_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIMSD_CLOSE_INPUT_PSW_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdCloseInputPswWinMsg), 
    WIN_ID( MMIMSD_CLOSE_INPUT_PSW_WIN_ID),
    WIN_TITLE(TXT_MSD_INPUT_PASSWORD),
    CREATE_EDIT_PASSWORD_CTRL(MMIMSD_PSW_LEN, MMIMSD_CLOSE_INPUT_PSW_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIMSD_MODPSW_INPUT_OLD_PSW_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdModPswInputOldPswWinMsg), 
    WIN_ID( MMIMSD_MODPSW_INPUT_OLD_PSW_WIN_ID),
    WIN_TITLE(TXT_MSD_INPUT_PASSWORD),
    CREATE_EDIT_PASSWORD_CTRL(MMIMSD_PSW_LEN, MMIMSD_MODPSW_INPUT_OLD_PSW_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIMSD_MODPSW_INPUT_NEW_PSW_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdModPswInputNewPswWinMsg), 
    WIN_ID( MMIMSD_MODPSW_INPUT_NEW_PSW_WIN_ID),
    WIN_TITLE(TXT_MSD_INPUT_NEW_PSW),
    CREATE_EDIT_PASSWORD_CTRL(MMIMSD_PSW_LEN, MMIMSD_MODPSW_INPUT_NEW_PSW_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIMSD_MODPSW_INPUT_PSW_AGAIN_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleMsdModPswInputPswAgainWinMsg), 
    WIN_ID( MMIMSD_MODPSW_INPUT_PSW_AGAIN_WIN_ID),
    WIN_TITLE(TXT_MSD_MAKESURE_PSW),
    CREATE_EDIT_PASSWORD_CTRL(MMIMSD_PSW_LEN, MMIMSD_MODPSW_INPUT_PSW_AGAIN_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/

void MMIMSD_OpenInitInputPswWin(void)
{
    MMIMSD_ResetErrorInputPswCount();// 开机总让它输入三次密码.如果想把以前输入错误的记录保留下来,去掉这句就可以了
    #ifndef FLASH_SUPPORT//这个地方不应该进入IDLE
    MMIAPIIDLE_OpenIdleWin();
	#endif
    MMIMSD_SetIllegalUseFlag(TRUE);
    MMK_CreatePubEditWin((uint32*)MMIMSD_INIT_INPUT_PSW_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : handle the message of main menu MSD
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdInitInputPswWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMSD_INIT_INPUT_PSW_PASSWORD_CTRL_ID; 
    uint8 relate_tel [MMIMSD_MAX_TEL_MUM + 1] = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        break;
        
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07     
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMIMSD_PSW_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T psw_info = {0};
            GUIEDIT_GetString(
                ctrl_id,
                &psw_info
                );
            if(MMIMSD_IsPswRight(&psw_info))
            {
                MMIMSD_SetIllegalUseFlag(FALSE);
                MMIMSD_ResetErrorInputPswCount();
                MMIMSD_ResetSendMsgCount();
                MMIPUB_OpenQueryWinByTextId(TXT_MSD_BIND_NEW_PHONE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            else
            {
                MMIMSD_PswWrongPlus();				
                if (MMIMSD_IfPswWrongTooMany())
                {
                    //关闭开机动画，打开idle界面
                    MMK_CloseWin(win_id);
                    MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
                    MMIAPIIDLE_OpenIdleWin();
                    MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                    GUIEDIT_ClearAllStr(ctrl_id);
                }
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        MMIAPIIDLE_OpenIdleWin();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
    case MSG_APP_FLIP:
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        MMIAPIIDLE_OpenIdleWin();
        MMK_CloseWin(win_id);
        recode = MMI_RESULT_FALSE;
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMI_MEMCPY(relate_tel, MMIMSD_MAX_TEL_MUM + 1, g_mmimsd_current_nv.relate_tel, MMIMSD_MAX_TEL_MUM, g_mmimsd_current_nv.relate_tel_len);
        //设置新的启动信息
        MMIMSD_MsdSetNewInfo(relate_tel, g_mmimsd_current_nv.relate_tel_len, MMIMSD_SETUP);
        //关闭开机动画，打开idle界面
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        MMIAPIIDLE_OpenIdleWin();
        
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        
        //关闭开机动画，打开idle界面	
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
        MMIAPIIDLE_OpenIdleWin();
        break;

    case MSG_CLOSE_WINDOW:
        if (MMIMSD_IfIllegalUse())
        {
            MMIMSD_SendMsdMsg();
        }
#ifdef FLASH_SUPPORT
		MMIAPIPHONE_ScrDelCallBack(win_id);
#endif
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_OpenSetupInputPswWin(void)
{
    if(FALSE == MMIMSD_IfIllegalUse())
    {
        if(TRUE == MMIMSD_IsPswNotNull())
        {
            if(FALSE == MMIAPIMSD_IsMsdStartup())
            {
                if (MMIAPIPHONE_GetSimCardOkNum(PNULL, 0) > 0)
                {
                    MMK_CreatePubEditWin((uint32*)MMIMSD_SETUP_INPUT_PSW_WIN_TAB,PNULL);
                }
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_MSD_HAVE_OPEN);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_MSD_MODIFYPSW_FIRST);				
        }
    }
    else
    {
        //提示无权使用
        MMIPUB_OpenAlertWarningWin(TXT_MSD_NO_RIGHT);				
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdSetupInputPswWinMsg (
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMSD_SETUP_INPUT_PSW_EDITBOX_CTRL_ID; 
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif
        break;
        
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMIMSD_PSW_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T  psw_info = {0};
            GUIEDIT_GetString(
                ctrl_id,
                &psw_info
                );	
            if(MMIMSD_PSW_LEN == psw_info.wstr_len)
            {
                if(TRUE == MMIMSD_IsPswRight(&psw_info))
                {
                    MMK_CreatePubEditWin((uint32*)MMIMSD_SETUP_INPUT_INFO_TEL_WIN_TAB, PNULL);
                    
                    MMIMSD_ResetErrorInputPswCount();
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIMSD_PswWrongPlus();
                    if(TRUE == MMIMSD_IfPswWrongTooMany())
                    {					
                        MMK_ReturnIdleWin();
                        MMIMSD_ResetErrorInputPswCount();
                    }
                    else
                    {
                        //提示密码错误
                        GUIEDIT_ClearAllStr(ctrl_id);
                    }
                    //提示密码错误
                    MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                }
            }
            
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}	
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdSetupInputInfoTelWinMsg (
                                                      MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param
                                                      )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMSD_SETUP_INPUT_INFO_TEL_EDITBOX_CTRL_ID; 
    wchar relate_tel[MMIMSD_MAX_TEL_MUM + 1] = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BG_T        edit_bg = {0};
    GUI_BORDER_T    edit_border = {0};
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
#if defined (MMI_GUI_STYLE_TYPICAL) ||defined (MMI_CALLSET_MINI_SUPPORT)
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_DIVERT);
#endif
        break;
        
    case MSG_OPEN_WINDOW:
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        MMK_SetAtvCtrl(MMIMSD_SETUP_INPUT_INFO_TEL_WIN_ID, MMIMSD_SETUP_INPUT_INFO_TEL_EDITBOX_CTRL_ID);
        MMI_STRNTOWSTR(relate_tel,
                                    MMIMSD_MAX_TEL_MUM,
                                    (uint8 *)g_mmimsd_current_nv.relate_tel,
                                    g_mmimsd_current_nv.relate_tel_len,
                                    g_mmimsd_current_nv.relate_tel_len);
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(ctrl_id,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(ctrl_id,&edit_border);

        //set font
        GUIEDIT_SetFontColor(ctrl_id,MMI_BLACK_COLOR);

        GUIEDIT_SetString(ctrl_id, relate_tel, g_mmimsd_current_nv.relate_tel_len);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            //获得输入的电话号码
            MMI_STRING_T tel_info = {0};
            GUIEDIT_GetString(
                ctrl_id,
                &tel_info
                );
            if(tel_info.wstr_len == 0)
            {
                //提示通知号码不能为空！
                MMIPUB_OpenAlertWarningWin(TXT_COMM_INPUT_NULL);
            }
            else if (MMIAPICOM_IsValidNumberStringEx(&tel_info))
            {
                if (MMIAPIPHONE_GetSimCardOkNum(PNULL, 0)>0)
                {
                    uint8 relate_tel_arry[MMIMSD_MAX_TEL_MUM + 1] = {0};
                    //设置新的启动信息

                    MMI_WSTRNTOSTR((uint8 *)relate_tel_arry,
                                                MMIMSD_MAX_TEL_MUM,
                                                tel_info.wstr_ptr,
                                                MMIMSD_MAX_TEL_MUM,
                                                tel_info.wstr_len);
                    
                    MMIMSD_MsdSetNewInfo(relate_tel_arry, tel_info.wstr_len, MMIMSD_SETUP);
                    //SCI_TRACE_LOW("HandleMsdSetupInputInfoTelWinMsg: the tele num is %s,the length is %d",tel_info.str_ptr,tel_info.str_len);
                    //提示已完成
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertSuccessWin(TXT_MSD_COMPLETED);
#endif
                }
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIMSD_SETUP_INPUT_PSW_WIN_ID);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_OpenCloseInputPswWin(void)
{
    if(FALSE == MMIMSD_IfIllegalUse()) //不是非法使用
    {	
        if(TRUE == MMIMSD_IsPswNotNull())
        {
            if(TRUE == MMIAPIMSD_IsMsdStartup())
            {
                MMK_CreatePubEditWin((uint32*)MMIMSD_CLOSE_INPUT_PSW_WIN_TAB, PNULL);
            }
            else
            {	
                MMIPUB_OpenAlertWarningWin(TXT_MSD_HAVE_CLOSE);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_MSD_MODIFYPSW_FIRST);
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_MSD_NO_RIGHT);
    }
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdCloseInputPswWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMSD_CLOSE_INPUT_PSW_EDITBOX_CTRL_ID; 
    GUI_BOTH_RECT_T edit_both_rect = {0};

    switch(msg_id)
    {
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif
        break;
        
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMIMSD_PSW_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T  psw_info = {0};
            GUIEDIT_GetString(
                ctrl_id,
                &psw_info
                );	

            if(MMIMSD_PSW_LEN == psw_info.wstr_len)
            {
                if(TRUE == MMIMSD_IsPswRight(&psw_info))
                {
                    MMIMSD_ResetErrorInputPswCount();
                    g_mmimsd_current_nv.open &= (~MMIMSD_OPEN_MASK); 
                    MMIMSD_SetInfoToNv(&g_mmimsd_current_nv);
                    //SCI_TRACE_LOW("HandleMsdCloseInputPswWinMsg: The tele num is %s,length is %d",g_mmimsd_current_nv.relate_tel,g_mmimsd_current_nv.relate_tel_len);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertSuccessWin(TXT_MSD_COMPLETED);
#endif
                    MMK_CloseWin(MMIMSD_CLOSE_INPUT_PSW_WIN_ID);
                }
                else
                {
                    MMIMSD_PswWrongPlus();
                    if(TRUE == MMIMSD_IfPswWrongTooMany())
                    {					
                        MMK_ReturnIdleWin();
                        MMIMSD_ResetErrorInputPswCount();
                    }
                    else
                    {	
                        GUIEDIT_ClearAllStr(ctrl_id);
                    }
                    
                    //提示密码错误
                    MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                }
            }
            
        }
        
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;	
    
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdModPswInputOldPswWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMSD_MODPSW_INPUT_OLD_PSW_EDITBOX_CTRL_ID; 
    GUI_BOTH_RECT_T edit_both_rect = {0};

    switch(msg_id)
    {
    case MSG_FULL_PAINT:
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif
        break;
        
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMIMSD_PSW_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T  psw_info = {0};
            GUIEDIT_GetString(
                ctrl_id,
                &psw_info
                );	
            if(MMIMSD_PSW_LEN == psw_info.wstr_len)
            {
                if(TRUE == MMIMSD_IsPswRight(&psw_info))
                {
                    MMK_CreatePubEditWin((uint32*)MMIMSD_MODPSW_INPUT_NEW_PSW_WIN_TAB, PNULL);
                    MMIMSD_ResetErrorInputPswCount();				
                }
                else
                {
                    MMIMSD_PswWrongPlus();
                    
                    if(TRUE == MMIMSD_IfPswWrongTooMany())
                    {					
                        MMK_ReturnIdleWin();
                        MMIMSD_ResetErrorInputPswCount();
                    }
                    else
                    {
                        //提示密码错误
                        GUIEDIT_ClearAllStr(ctrl_id);
                    }
                    //提示密码错误
                    MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);														
                }
            }
            
        }
        
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}	
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_ModPswInputPswWin(void)
{
    if(FALSE == MMIMSD_IfIllegalUse())
    {
        if(FALSE == MMIMSD_IsPswNotNull())
        {
            //打开输入新密码窗口
            MMK_CreatePubEditWin((uint32*)MMIMSD_MODPSW_INPUT_NEW_PSW_WIN_TAB, PNULL);
        }
        else
        {
            //打开输入旧密码窗口
            MMK_CreatePubEditWin((uint32*)MMIMSD_MODPSW_INPUT_OLD_PSW_WIN_TAB, PNULL);
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_MSD_NO_RIGHT);
    }
    
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdModPswInputNewPswWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMSD_MODPSW_INPUT_NEW_PSW_EDITBOX_CTRL_ID; 
    GUI_BOTH_RECT_T edit_both_rect = {0};

    switch(msg_id)
    {
    case MSG_FULL_PAINT:
 #ifndef MMIPUBWIN_PDASTYLE_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
 #endif
        break;
        
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMIMSD_PSW_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T  psw_info = {0};
            GUIEDIT_GetString(
                ctrl_id,
                &psw_info
                );	
            if(MMIMSD_PSW_LEN == psw_info.wstr_len)
            {
                first_input_psw_ptr = SCI_ALLOC_APP((MMIMSD_PSW_LEN+1)*sizeof(wchar));
                SCI_MEMSET((uint8 *)first_input_psw_ptr, 0x00, ((MMIMSD_PSW_LEN+1)*sizeof(wchar)));

                MMI_WSTRNCPY(first_input_psw_ptr, MMIMSD_PSW_LEN, psw_info.wstr_ptr, psw_info.wstr_len, psw_info.wstr_len);
                MMK_CreatePubEditWin((uint32*)MMIMSD_MODPSW_INPUT_PSW_AGAIN_WIN_TAB, PNULL);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if(first_input_psw_ptr != PNULL)
        {
            SCI_FREE(first_input_psw_ptr);
            first_input_psw_ptr = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsdModPswInputPswAgainWinMsg(
                                                       MMI_WIN_ID_T win_id, 
                                                       MMI_MESSAGE_ID_E msg_id, 
                                                       DPARAM param
                                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMSD_MODPSW_INPUT_PSW_AGAIN_EDITBOX_CTRL_ID;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
        MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
#endif
        break;
        
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
		 MMIAPISET_SetPwTitleRect(win_id);
        edit_both_rect = MMIAPISET_GetPwEditRectForDphone();
#else
        edit_both_rect = MMIAPISET_GetPwEditRect(win_id );
#endif 
        GUIAPICTRL_SetBothRect(ctrl_id, &edit_both_rect);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,MMIMSD_PSW_LEN,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T  psw_info = {0};
            GUIEDIT_GetString(
                ctrl_id,
                &psw_info
                );	
            if(MMIMSD_PSW_LEN == psw_info.wstr_len)
            {
                if(0 == MMIAPICOM_CompareTwoWstr(psw_info.wstr_ptr, psw_info.wstr_len, first_input_psw_ptr, MMIMSD_PSW_LEN))
                {
                    // cr86413: psw_info.str_ptr没有字符串结束符，不能传入函数MMIMSD_UpdatePsw
                    wchar str[MMIMSD_PSW_LEN + 1] = {0};
                    
                    MMI_WSTRNCPY(str, MMIMSD_PSW_LEN + 1, psw_info.wstr_ptr, MMIMSD_PSW_LEN, MMIMSD_PSW_LEN);
                    str[MMIMSD_PSW_LEN] = 0x0000;
                    MMIMSD_UpdatePsw(str);
                    
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertSuccessWin(TXT_MSD_COMPLETED);
#endif
                    MMK_CloseWin(win_id);
                    MMK_CloseWin(MMIMSD_MODPSW_INPUT_NEW_PSW_WIN_ID);
                    if(MMK_IsOpenWin(MMIMSD_MODPSW_INPUT_OLD_PSW_WIN_ID))
                    {
                        MMK_CloseWin(MMIMSD_MODPSW_INPUT_OLD_PSW_WIN_ID);
                    }				
                }
                else 
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PWD_CONFLICT);
                    MMK_CloseWin(win_id);
                    GUIEDIT_ClearAllStr(MMIMSD_MODPSW_INPUT_NEW_PSW_EDITBOX_CTRL_ID);				
                }
            }
        }
        
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;	
}

/*****************************************************************************/
// 	Description : to handle alert window 
//	Global resource dependence : 
//  Author:phoebe.chen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShutoffAlertClosedMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    
    if (MSG_CLOSE_WINDOW == msg_id)
    {      
        MMIAPIPHONE_PowerOff();
    }
    
    return recode;
}  
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_OpenShutOffWin (void)
{
    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MSD_ILLEGAL_USE_AND_CLOSE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleShutoffAlertClosedMsg);
}
#endif



/*Edit by script, ignore 7 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
