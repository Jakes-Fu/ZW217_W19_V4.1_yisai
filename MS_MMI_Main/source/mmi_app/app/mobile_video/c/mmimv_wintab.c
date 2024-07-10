/*****************************************************************************
** File Name:      mmimv_wintab.c                                           *
** Author:                                                                   *
** Date:           05/2010                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       		     Creat
******************************************************************************/

#define _MMIMV_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmimv_id.h"
#include "mmimv_export.h"
#include "mv_input_api.h"
#include "mmk_type.h"
#include "mv_base_api.h"
#include "mv_player_api.h"
#include "mv_timer_api.h"
#include "window_parse.h"
#include "mmi_default.h"
#include "mv_ui_api.h"
#include "mmidisplay_data.h"
#include "mmipb_export.h"
#include "mmi_appmsg.h"
#include "mmisms_export.h"
#include "mv_contact_api.h"
#include "mmisms_app.h"
#include "mv_debug.h"
#include "mv_mm_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
PUBLIC BOOLEAN g_is_normal_exit = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle the mobile video main window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(
                                    MMI_WIN_ID_T        win_id, //window id
                                    MMI_MESSAGE_ID_E	msg_id, //the identify of message
                                    DPARAM              param   //the param of the message
                                    );

/************************************************************************/
//编辑空件窗口的回调函数
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
LOCAL void CreateEditBox(MMI_STRING_T *text_ptr,
						  uint16      max_ucs2_len,
						  uint32      allow_inputmethod,
						  MMI_WIN_ID_T win_id);


/************************************************************************/
//编辑空件窗口的回调函数
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
LOCAL MMI_RESULT_E HandleEditCommonWinMsg(
										  MMI_WIN_ID_T win_id,
										  MMI_MESSAGE_ID_E msg_id,
										  DPARAM param
										  );
/************************************************************************/
//contactdialog 窗口的回调函数
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
LOCAL MMI_RESULT_E HandleContactWinMsg(
									   MMI_WIN_ID_T win_id,
									   MMI_MESSAGE_ID_E msg_id,
									   DPARAM param
									   );
/************************************************************************/
//contactdialog 
//Global resource dependence
//Author:
//note:
/************************************************************************/
LOCAL void SelectContactCallback(MMIPB_HANDLE_T handle);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
// the window for mobile video main win
WINDOW_TABLE( MMIMV_MAIN_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMainWinMsg ),    
    WIN_ID( MMIMV_MAIN_WIN_ID ),    
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};


WINDOW_TABLE(MMI_EDITCOMMON_WIN_TAB) =
{
	WIN_FUNC((uint32)HandleEditCommonWinMsg),
	WIN_ID(MMI_EDITCOMMON_WIN_ID),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
	WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

WINDOW_TABLE(MMI_CONTACT_WIN_TAB) =
{
	WIN_FUNC((uint32)HandleContactWinMsg),
	WIN_ID(MMI_CONTACT_WIN_ID),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
	END_WIN
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/************************************************************************/
//contactdialog 
//Global resource dependence
//Author:
//note:
/************************************************************************/
LOCAL void SelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMI_CONTACT_WIN_ID, MMI_SELECT_CONTACT_CNF, handle);   
}

/*****************************************************************************/
//  Description : to handle the mobile video main window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(
                                    MMI_WIN_ID_T        win_id, //window id
                                    MMI_MESSAGE_ID_E	msg_id, //the identify of message
                                    DPARAM              param   //the param of the message
                                    )
{
    MMI_RESULT_E recode	= MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    


    UNUSED(param);
    UNUSED(win_id);

    //MV_TRACE_LOW:"[MV] HandleMainWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMV_WINTAB_159_112_2_18_2_39_8_2,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        g_is_normal_exit = FALSE;
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);    
        MMIAPISET_StopAllRing(FALSE);     
        break;        

    case MSG_GET_FOCUS:    
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);    
        MV_MediaPlayer_GetFocus();
        break;

    case MSG_LOSE_FOCUS:      
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);        
        MV_MediaPlayer_LoseFocus();
        break;
        
    case MSG_KEYDOWN_UP:
        CM_Key_Asyn_CallBack(CM_KEY_UP, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;

    case MSG_KEYDOWN_DOWN:
        CM_Key_Asyn_CallBack(CM_KEY_DOWN, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;
        
    case MSG_KEYDOWN_LEFT:
        CM_Key_Asyn_CallBack(CM_KEY_LEFT, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;

    case MSG_KEYDOWN_RIGHT:
        CM_Key_Asyn_CallBack(CM_KEY_RIGHT, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;

    case MSG_KEYDOWN_GREEN:
        CM_Key_Asyn_CallBack(CM_KEY_SEND, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;

    case MSG_KEYDOWN_CAMERA:
        CM_Key_Asyn_CallBack(CM_KEY_CAMERA, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;
        
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9: 
        CM_Key_Asyn_CallBack(CM_KEY_DIGIT, CM_DIGIT_1+ msg_id -MSG_KEYDOWN_1, CM_SHORT_PRESS);    
        break;
    
    case MSG_KEYDOWN_0:
        CM_Key_Asyn_CallBack(CM_KEY_DIGIT, CM_DIGIT_0, CM_SHORT_PRESS);
        break;
        
    case MSG_KEYDOWN_HASH:
        CM_Key_Asyn_CallBack(CM_KEY_HASH, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;
        
    case MSG_KEYDOWN_STAR:
        CM_Key_Asyn_CallBack(CM_KEY_STAR, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;
        
    case MSG_KEYDOWN_OK:
        CM_Key_Asyn_CallBack(CM_KEY_SKL, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;            
        
    case MSG_KEYDOWN_CANCEL:
        CM_Key_Asyn_CallBack(CM_KEY_SKR, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;

#ifdef FLIP_PHONE_SUPPORT        
	case MSG_KEYDOWN_FLIP://增加合盖处理 2011-06-02
#endif	
    case MSG_KEYDOWN_RED://按挂机键(电源键)
    case CHR_MSG_SHUTDOWN_IND:
		CM_WriteLogs(1, "HandleMainWinMsg() :: case CHR_MSG_SHUTDOWN_IND");
        CM_Key_Asyn_CallBack(CM_KEY_END, CM_NO_DIGIT, CM_SHORT_PRESS);
		break;
    case MSG_KEYDOWN_UPSIDE:
        CM_Key_Asyn_CallBack(CM_KEY_SIDEUP, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;
        
    case MSG_KEYDOWN_DOWNSIDE:
        CM_Key_Asyn_CallBack(CM_KEY_SIDEDOWN, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;
        
    case MSG_KEYDOWN_WEB:
        CM_Key_Asyn_CallBack(CM_KEY_OK, CM_NO_DIGIT, CM_SHORT_PRESS);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:            
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        CM_Touch_CallBack(CM_TP_DOWN,point.x,point.y,PNULL);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        CM_Touch_CallBack(CM_TP_UP,point.x,point.y,PNULL);        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CLOSE_WINDOW:
        if (!g_is_normal_exit)
        {        
#ifndef WIN32    
            CM_MobileTV_Terminate();
#endif
        }
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);   
        //恢复3d效果
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_MV);
#endif          
        //MV_DeleteMemHandle();
        break;

   default:     
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Enter into mobile video
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMV_OpenMainWin(void)
{
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_MV);
#endif 
    MMK_CreateWin((uint32*)MMIMV_MAIN_WIN_TAB, PNULL);    
}


/*****************************************************************************/
//  Description : to handle the mobile video main window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditCommonWinMsg(
										  MMI_WIN_ID_T win_id,
										  MMI_MESSAGE_ID_E msg_id,
										  DPARAM param
										  )
{
	MMI_STRING_T        strvalue = {0};
	MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id = MMI_EDITCTRL_ID;
    MMIMV_TEXTEDITBOX_INIT_DATA_T init_data = MV_GetEditDialogInitData();
    
	//MV_TRACE_LOW:"[MV] run in handleeditcommonwinmsg msg_id = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMV_WINTAB_319_112_2_18_2_39_8_3,(uint8*)"d", msg_id);

    UNUSED(param);    
    
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW: 
        //MV_TRACE_LOW:"[MV] run in allow_inputmethod = %d, max_len = %d, callback = %x, defaul_len = %d, title_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMV_WINTAB_328_112_2_18_2_39_8_4,(uint8*)"ddddd", init_data.allow_inputmethod, init_data.max_ucs2_len,init_data.CallbackProc,init_data.default_text.wstr_len,init_data.title.wstr_len);
		CreateEditBox(&(init_data.default_text), init_data.max_ucs2_len, init_data.allow_inputmethod, win_id);
		MMK_SetAtvCtrl(win_id,ctrl_id);	        
        if (0 != init_data.title.wstr_len)
        {
            GUITITLE_SetText(win_id,MMITHEME_GetTitleCtrlId(),init_data.title.wstr_ptr,init_data.title.wstr_len,FALSE);
        }
		break;

	case MSG_CTL_OK:
	case MSG_APP_OK:		
		GUIEDIT_GetString(ctrl_id,&strvalue);
		init_data.CallbackProc((_HANDLE)win_id, init_data.CallbackProcParam, EEditDialogResult_OK,strvalue.wstr_ptr);
        MMK_CloseWin(win_id);
		break;
        
    case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:		
		init_data.CallbackProc((_HANDLE)win_id,init_data.CallbackProcParam,EEditDialogResult_Cancel,strvalue.wstr_ptr);
		MMK_CloseWin(win_id);
		break;

	default:
		recode = MMI_RESULT_FALSE;
	}

	return recode;

}

/*****************************************************************************/
//  Description : CreateEditBox
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditBox(MMI_STRING_T *text_ptr,
						  uint16      max_ucs2_len,
						  uint32      allow_inputmethod,
						  MMI_WIN_ID_T win_id)
{
	BOOLEAN   is_edit_text = FALSE;
	GUIIM_TYPE_T input_mode;
	GUI_FONT_T   font_type = MMI_DEFAULT_TEXT_FONT;

	MMI_CONTROL_CREATE_T create = {0};
	GUIEDIT_INIT_DATA_T  init_data = {0};
	void * edit_ptr = PNULL;

    init_data.both_rect.v_rect = MMITHEME_GetClientRect();
    init_data.both_rect.h_rect = MMITHEME_GetClientRect();

	//MV_TRACE_LOW:"[MV] run in createeditbox allow_inputmethod = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMV_WINTAB_378_112_2_18_2_39_8_5,(uint8*)"d",allow_inputmethod);
	switch(allow_inputmethod)
	{
	case EEditDialogOptions_Numeric:
        is_edit_text = TRUE;
        input_mode = GUIIM_TYPE_DIGITAL ;
        init_data.type = GUIEDIT_TYPE_TEXT;	
	    break;

	case EEditDialogOptions_English:
		input_mode = GUIIM_TYPE_ENGLISH;
		is_edit_text = TRUE;
        init_data.type = GUIEDIT_TYPE_TEXT;	
		break;

    case EEditDialogOptions_Chinese:
		is_edit_text = TRUE;
		input_mode = GUIIM_TYPE_ABC;	
        init_data.type = GUIEDIT_TYPE_TEXT;	
        break;

	case EEditDialogOptionsAll:
		is_edit_text = TRUE;
		input_mode = GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_STROKE | GUIIM_TYPE_SMART;
	    init_data.type = GUIEDIT_TYPE_TEXT;	
		break;
        
	default:
		is_edit_text = FALSE;
		input_mode = GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH;
        init_data.type = GUIEDIT_TYPE_TEXT;	
		break;
	}

    
	init_data.str_max_len = max_ucs2_len;
	create.ctrl_id = MMI_EDITCTRL_ID;
	create.guid = SPRD_GUI_EDITBOX_ID;
	create.parent_win_handle = win_id;
	create.init_data_ptr = &init_data;
	edit_ptr = MMK_CreateControl(&create);

	GUIEDIT_SetIm(create.ctrl_id,input_mode,input_mode);

	if (is_edit_text)
	{	
		GUIEDIT_SetTextMaxLen(create.ctrl_id,max_ucs2_len,max_ucs2_len);
	}
	GUIEDIT_SetFont(create.ctrl_id,font_type);
	if (PNULL != text_ptr && PNULL != text_ptr->wstr_ptr)
	{	
		GUIEDIT_SetString(create.ctrl_id,text_ptr->wstr_ptr,text_ptr->wstr_len);
	}
}


/*****************************************************************************/
//  Description : HandleContactWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactWinMsg(
									   MMI_WIN_ID_T win_id,
									   MMI_MESSAGE_ID_E msg_id,
									   DPARAM param
									   )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;

    UNUSED(param);

	//MV_TRACE_LOW:"[MV] run in handlecontactwinmsg msg_id = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMV_WINTAB_448_112_2_18_2_39_9_6,(uint8*)"d", msg_id);
    
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		//MMIAPIPB_OpenNumberList(MMIPB_LIST_FOR_SMS,MMIPB_LIST_TYPE_SINGLE,1,PNULL);
        MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_RADIO, //IN: 
                MMIPB_LIST_FIELD_WHOLE,
                1,//
                PNULL,
                SelectContactCallback
                );		
		break;

	case MMI_SELECT_CONTACT_CNF:
		MV_GetContactInfo((MMIPB_HANDLE_T)param, MMISMS_MAX_MESSAGE_LEN);	
		MV_ContactDialogCallbackProc(EContactDialogResult_OK);
        MMK_CloseWin(win_id);
		break;
        
	case MMI_PB_SELECT_CANCLE:
		MV_ContactDialogCallbackProc(EContactDialogResult_Cancel);
		MMK_CloseWin(win_id);
		break;

    case MSG_CLOSE_WINDOW:
	    break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
	}

	return recode;
}


/*****************************************************************************/
//  Description : MV_CreateContactDialog
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_CreateContactDialog(void)
{
    MMK_CreateWin((uint32*)MMI_CONTACT_WIN_TAB,NULL);
   
}

/*****************************************************************************/
//  Description : cancel contact dialog 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMV_CancelContactDialog(void)
{
    if(MMK_IsOpenWin(MMI_CONTACT_WIN_ID))
    {
        MMK_SendMsg(MMI_CONTACT_WIN_ID, MMI_PB_SELECT_CANCLE, PNULL);
    }   
}

/*****************************************************************************/
//  Description : MV_CreateEditWin
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_CreateEditWin(void)
{
	MMK_CreateWin((uint32 *)MMI_EDITCOMMON_WIN_TAB,PNULL);	
}



/*Edit by script, ignore 2 case. Thu Apr 26 19:01:00 2012*/ //IGNORE9527
