/*****************************************************************************
** File Name:      watch_set_call_forwarding.c                               *
** Author:         xiaoju.cheng                                              *
** Date:           01/06/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description: Display wear settings items                                  *
******************************************************************************
**                         Important Edit History
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 01/06/2020           xiaoju.cheng             Create                      *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "guires.h"
#include "mmicom_trace.h"
#include "watch_common.h"
#include "mmisrveng.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmicc_image.h"
#include "mmicc_text.h"
#include "mmidisplay_color.h"
#include "watch_commonwin_export.h"
#ifdef UAL_SS_SUPPORT
#include "ual_tele.h"
#endif
#include "watch_set_call_forwarding.h"
#include "set_ss_flow.h"
#include "mmiset_id.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_export.h"
#include "mmiphone_export.h"
#include "watch_notifycenter.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define DIALPAD_NUM_BUTTON_ROW_MAX               4
#define DIALPAD_NUM_BUTTON_COLUMN_MAX            3
#define DIALDAP_EDITBOX_INPUT_MAX_LEN            255
#define DIALPAD_DELETE_BUTTON_RECT               DP2PX_RECT(184,34,208,58)
#define DIALPAD_EDITBOX_BIG_FONT                 DP_FONT_20
#define DIALPAD_EDITBOX_SMALL_FONT               DP_FONT_14
#define DIALPAD_EDITBOX_RECT_LEFT        DP2PX_VALUE(56)
#define DIALPAD_EDITBOX_RECT_RIGHT       DP2PX_VALUE(184)
#define DIALPAD_EDITBOX_RECT_TOP         DP2PX_VALUE(34)
#define DIALPAD_EDITBOX_RECT_BOTTOM      DP2PX_VALUE(57)

LOCAL MMI_STRING_T s_callforwarding_number = {0};
LOCAL uint8  s_operation_wait_ps_timer_id      = NULL; // 等待PS是否好的timer，此timer 设置2s消失
LOCAL uint8  s_interrogate_wait_ps_timer_id      = NULL; // 等待PS是否好的timer，此timer 设置2s消失
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define SETTINGS_CALLFORWARDING_ITEM_MAX                (2)
#define SETTINGS_CALLFORWARDING_NUMBER_MAX_LEN          (40)
#define TIMEOUT_WAIT_PS_MSG                             (2*60*1000)//一直PS开启，请求超时的timeout

typedef enum
{
    SETTINGS_CALLFORWARDING_ON,
    SETTINGS_CALLFORWARDING_OFF,
    SETTINGS_CALLFORWARDING_MAX
}
SETTINGS_DIVERT_LIST_ITEM_E;

LOCAL MMI_IMAGE_ID_T s_dial_num_button_image_order[DIALPAD_NUM_BUTTON_ROW_MAX][DIALPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {res_aw_dialpad_ic_num1, res_aw_dialpad_ic_num2, res_aw_dialpad_ic_num3},
    {res_aw_dialpad_ic_num4, res_aw_dialpad_ic_num5, res_aw_dialpad_ic_num6},
    {res_aw_dialpad_ic_num7, res_aw_dialpad_ic_num8, res_aw_dialpad_ic_num9},
    {res_aw_dialpad_ic_asterisk, res_aw_dialpad_ic_num0, res_aw_dialpad_ic_pound},
};

LOCAL MMI_CTRL_ID_T s_dial_num_button_ctrl_order[DIALPAD_NUM_BUTTON_ROW_MAX][DIALPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {DIVERT_PAD_NUM1_BUTTON_CTRL_ID, DIVERT_PAD_NUM2_BUTTON_CTRL_ID, DIVERT_PAD_NUM3_BUTTON_CTRL_ID},
    {DIVERT_PAD_NUM4_BUTTON_CTRL_ID, DIVERT_PAD_NUM5_BUTTON_CTRL_ID, DIVERT_PAD_NUM6_BUTTON_CTRL_ID},
    {DIVERT_PAD_NUM7_BUTTON_CTRL_ID, DIVERT_PAD_NUM8_BUTTON_CTRL_ID, DIVERT_PAD_NUM9_BUTTON_CTRL_ID},
    {DIVERT_PAD_ASTERISK_BUTTON_CTRL_ID, DIVERT_PAD_NUM0_BUTTON_CTRL_ID, DIVERT_PAD_POUND_BUTTON_CTRL_ID},
};

LOCAL BOOLEAN s_operation_is_doing = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E HandleNumberpadWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    );

LOCAL MMI_RESULT_E  HandleSettingsCallForwardingWindow(
                                     MMI_WIN_ID_T    win_id,
                                     MMI_MESSAGE_ID_E   msg_id,
                                     DPARAM             param
                                     );

LOCAL void Settings_CallForwarding_PenOK_Web( void );

LOCAL void Settings_SS_Operation_StopWaitPSTimer(void);

LOCAL void Settings_SS_Interrogate_StopWaitPSTimer(void);

/*---------------------------------------------------------------------------*/
/*                          WINDOW TABLE DEFINITION                          */
/*---------------------------------------------------------------------------*/

WINDOW_TABLE(CALLFORWARDING_NUMBER_WIN_TAB) =
{
    WIN_ID(CALLFORWARDING_NUMBER_WIN_ID),
    WIN_FUNC((uint32)HandleNumberpadWinMsg),
    WIN_HIDE_STATUS,
    CREATE_BUTTON_CTRL(res_aw_dialpad_ic_delete, DIVERT_NUMBER_DELETE_BUTTON_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISET_DIVERT_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleSettingsCallForwardingWindow ),
    WIN_ID( MMISET_CALLFORWARDING_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SET_CALL_FORWARDING),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CALLFORWARDING_LIST_CTRL_ID),
    END_WIN
};


/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/
LOCAL void HandleCallForwardingPenOkMsg(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_CTRL_ID_T     ctrl_id
                                              )
{
    MMI_MESSAGE_ID_E    keyMsgId = 0;
    MMI_STRING_T        alert_str = {0};
    wchar temp[SETTINGS_CALLFORWARDING_NUMBER_MAX_LEN/2+1] = {0};
    MMI_CTRL_ID_T       editCtrlId = DIVERT_NUMBER_EDITBOX_CTRL_ID;

    switch (ctrl_id)
    {
        case DIVERT_PAD_NUM1_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM2_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM3_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM4_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM5_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM6_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM7_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM8_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM9_BUTTON_CTRL_ID:
        case DIVERT_PAD_NUM0_BUTTON_CTRL_ID:
        case DIVERT_PAD_ASTERISK_BUTTON_CTRL_ID:
        case DIVERT_PAD_POUND_BUTTON_CTRL_ID:
        {
            keyMsgId = MSG_APP_1 + ctrl_id - DIVERT_PAD_NUM1_BUTTON_CTRL_ID;
            //handle key
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),keyMsgId,PNULL);
            break;
        }
        case DIVERT_NUMBERPAD_BUTTON_CTRL_ID:
        {
			Settings_CallForwarding_PenOK_Web();
            break;
        }
        case DIVERT_NUMBER_DELETE_BUTTON_CTRL_ID:
        {
			MMI_STRING_T		editStr = {0};

			//get edit string
			GUIEDIT_GetString(editCtrlId,&editStr);

			if (0 == editStr.wstr_len)
            {
                MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_APP_CANCEL,PNULL);
            }
            else
            {
                MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_NOTIFY_IM_BACKSPACE,PNULL);
            }
            break;
        }
        default:
            break;
    }
}
/*****************************************************************************/
//  Description : 创建拨号盘中的编辑框
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateNumberpadEditBox(MMI_WIN_ID_T win_id, const GUI_BOTH_RECT_T *edit_rect_ptr)
{
    MMI_CONTROL_CREATE_T        editCtrl = {0};
    GUI_BG_T                    bgInfo = {0};
    MMI_CTRL_ID_T               editCtrlId = DIVERT_NUMBER_EDITBOX_CTRL_ID;
    GUIEDIT_INIT_DATA_T         editInit = {0};
    GUIEDIT_FONT_T              editFont ={0};
    MMI_HANDLE_T                editHandle = 0;

    SCI_MEMCPY(&editInit.both_rect, edit_rect_ptr, sizeof(GUI_BOTH_RECT_T));
    editInit.type        = GUIEDIT_TYPE_PHONENUM;
    editInit.str_max_len = SETTINGS_CALLFORWARDING_NUMBER_MAX_LEN;
    editCtrl.ctrl_id           = editCtrlId;
    editCtrl.guid              = SPRD_GUI_EDITBOX_ID;
    editCtrl.init_data_ptr     = &editInit;
    editCtrl.parent_win_handle = win_id;
    MMK_CreateControl(&editCtrl);

    editHandle = MMK_GetCtrlHandleByWin(win_id, editCtrlId);

    //set im not handle tp
    GUIEDIT_SetImTp(editHandle, FALSE);
    //set delete after full
    GUIEDIT_SetPhoneNumStyle(FALSE, editHandle);
    //set display direction
    GUIEDIT_SetPhoneNumDir(editHandle,GUIEDIT_DISPLAY_DIR_LT);
    GUIEDIT_SetStyle(editHandle, GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetAlign(editHandle, ALIGN_HVMIDDLE);
    //set cursor hide
    GUIEDIT_SetCursorHideState(editHandle, TRUE);

    editFont.is_valid = TRUE;
    editFont.big_font = DIALPAD_EDITBOX_BIG_FONT;
    editFont.small_font = DIALPAD_EDITBOX_SMALL_FONT;
    GUIEDIT_SetFontEx(editHandle, &editFont);
    GUIEDIT_SetFontColor(editHandle, MMI_WHITE_COLOR);

    bgInfo.bg_type = GUI_BG_COLOR;
    bgInfo.color = MMI_BLACK_COLOR;
    GUIEDIT_SetBg(editHandle,&bgInfo);

}

/*****************************************************************************/
//  Description : 创建拨号盘中的button控件
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateNumberpadButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T button_ctrl, MMI_IMAGE_ID_T button_image, const GUI_BOTH_RECT_T *rect_ptr)
{
    GUI_BG_T                    bgInfo = {0};
    GUIBUTTON_INIT_DATA_T       buttonInit = {0};
    MMI_CONTROL_CREATE_T        buttonCtrl = {0};

    bgInfo.bg_type = GUI_BG_IMG;
    SCI_MEMCPY(&buttonInit.both_rect, rect_ptr, sizeof(GUI_BOTH_RECT_T));

    buttonCtrl.ctrl_id           = button_ctrl;
    buttonCtrl.guid              = SPRD_GUI_BUTTON_ID;
    buttonCtrl.init_data_ptr     = &buttonInit;
    buttonCtrl.parent_win_handle = win_id;
    MMK_CreateControl(&buttonCtrl);

    GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id,button_ctrl), TRUE);
    bgInfo.img_id = button_image;
    GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id,button_ctrl),&bgInfo);

}

LOCAL void CreateNumberpadCtrl(MMI_WIN_ID_T win_id)
{
    GUI_BOTH_RECT_T     editBoxRect = {0};
    GUI_BOTH_RECT_T     buttonRect = {0};
    GUI_RECT_T          delBtnRect = DIALPAD_DELETE_BUTTON_RECT;
    int32               row = 0;
    int32               column = 0;
    uint16              imgHeight = 0;
    uint16              imgWidth = 0;

    GUIRES_GetImgWidthHeight(&imgWidth, &imgHeight, res_aw_dialpad_ic_num0, win_id);
    // edit控件
    editBoxRect.v_rect.left = DIALPAD_EDITBOX_RECT_LEFT;
    editBoxRect.v_rect.right = DIALPAD_EDITBOX_RECT_RIGHT;
    editBoxRect.v_rect.top = DIALPAD_EDITBOX_RECT_TOP;
    editBoxRect.v_rect.bottom = DIALPAD_EDITBOX_RECT_BOTTOM;
    CreateNumberpadEditBox(win_id, &editBoxRect);
    // backspace按键
    GUIBUTTON_SetRect(DIVERT_NUMBER_DELETE_BUTTON_CTRL_ID, &delBtnRect);

    // button控件
    for (row = 0; row < DIALPAD_NUM_BUTTON_ROW_MAX; row++)
    {
        for (column = 0; column < DIALPAD_NUM_BUTTON_COLUMN_MAX; column++)
        {
            buttonRect.v_rect.left = DP2PX_VALUE(44) + (imgWidth + DP2PX_VALUE(4))* column;
            buttonRect.v_rect.top = DP2PX_VALUE(64) + (imgHeight + DP2PX_VALUE(4))* row ;
            buttonRect.v_rect.right = buttonRect.v_rect.left + imgWidth;
            buttonRect.v_rect.bottom = buttonRect.v_rect.top + imgHeight;

            CreateNumberpadButton(win_id, s_dial_num_button_ctrl_order[row][column], s_dial_num_button_image_order[row][column], &buttonRect);
        }
    }

    //Create dial button
    buttonRect.v_rect.left = DP2PX_VALUE(104);
    buttonRect.v_rect.top = DP2PX_VALUE(196);
    buttonRect.v_rect.right = DP2PX_VALUE(136);
    buttonRect.v_rect.bottom = DP2PX_VALUE(228);
    CreateNumberpadButton(win_id,DIVERT_NUMBERPAD_BUTTON_CTRL_ID,IMAGE_SET_DIVERT_AW_CONFIRM,&buttonRect);

}

LOCAL void Settings_CallForwarding_PenOK_Web( void )
{
  MMI_STRING_T	  editStr = {0};
  MMI_STRING_T		  alert_str = {0};
  MMI_CTRL_ID_T       editCtrlId = DIVERT_NUMBER_EDITBOX_CTRL_ID;

  SCI_TRACE_LOW("[divert]:Settings_CallForwarding_PenOK_Web,enter");

  //get edit string
  GUIEDIT_GetString(editCtrlId,&editStr);

	if (PNULL == editStr.wstr_ptr)
	{
		SCI_TRACE_LOW("[divert]:Settings_CallForwarding_PenOK_Web editStr is null");
		return;
	}
	else if (MMIAPICOM_IsValidNumberStringEx(&editStr))//enter call forwarding
	{
		uint32 i = 0;
		BOOLEAN is_valid = TRUE;

		for (i = 0; i < editStr.wstr_len; i++)
		{
			if (('*' == editStr.wstr_ptr[i])||('#' == editStr.wstr_ptr[i]))
			{
				is_valid = FALSE;
				SCI_TRACE_LOW("[divert]:Settings_CallForwarding_PenOK_Web editStr invalid1");
				break;
			}
		}
		if (is_valid)
		{
            if(PNULL == s_callforwarding_number.wstr_ptr)
            {
                s_callforwarding_number.wstr_ptr = SCI_ALLOC_APP(SETTINGS_CALLFORWARDING_NUMBER_MAX_LEN);
            }
            else
            {
                SCI_MEMSET(&s_callforwarding_number,0,sizeof(MMI_STRING_T));
            }
			//s_callforwarding_number.wstr_ptr = temp;
			SCI_MEMCPY(s_callforwarding_number.wstr_ptr,editStr.wstr_ptr,SETTINGS_CALLFORWARDING_NUMBER_MAX_LEN);
			s_callforwarding_number.wstr_len = editStr.wstr_len;
#ifdef UAL_SS_SUPPORT
			Watch_Settings_SS_Start_Operation(UAL_SIM_NUMBER_1);
#endif
		}
		else
		{
			MMI_GetLabelTextByLang(TXT_CC_INVALID_NUM, &alert_str);
			Adult_WatchCOM_NoteWin_1Line_Toast_Enter( CALLFORWARDING_ALERT_WIN_ID,&alert_str,PNULL,PNULL);
		}
	}
	else
	{
		MMI_GetLabelTextByLang(TXT_CC_INVALID_NUM, &alert_str);
		Adult_WatchCOM_NoteWin_1Line_Toast_Enter( CALLFORWARDING_ALERT_WIN_ID,&alert_str,PNULL,PNULL);
		SCI_TRACE_LOW("[divert]:Settings_CallForwarding_PenOK_Web editStr invalid2");
	}
}

LOCAL MMI_RESULT_E HandleNumberpadWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           editCtrlId = DIVERT_NUMBER_EDITBOX_CTRL_ID;

    SCI_TRACE_LOW("[divert] HandleNumberpadWinMsg msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            CreateNumberpadCtrl(win_id);
            MMK_SetAtvCtrl(win_id,editCtrlId);
            break;
        }

        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayBackground(win_id);
            break;
        }

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
		{
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CTL_PENOK:
		{
            if(PNULL != param)
            {
                HandleCallForwardingPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id);
            }
			else
			{
				SCI_TRACE_LOW("[divert] HandleNumberpadWinMsg param is null");
			}
            break;
        }

		case MSG_KEYUP_WEB:
		case MSG_CTL_MIDSK:
		{
			Settings_CallForwarding_PenOK_Web();
			break;
		}

        default:
		{
            result = MMI_RESULT_FALSE;
            break;
        }

    }
    return result;
}
#ifdef UAL_SS_SUPPORT
LOCAL MMI_RESULT_E HandleNetworkConnectMsg(
                                              MMI_WIN_ID_T          win_id, // 窗口的ID
                                              MMI_MESSAGE_ID_E 	    msg_id, // 窗口的内部消息ID
                                              DPARAM                param	// 相应消息的参数
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
#ifdef UAL_SS_SUPPORT//for bug1635672
        //release ss
        ual_tele_ss_cancle();
#endif
        //for bug1625643
        Settings_SS_Operation_StopWaitPSTimer();
        Settings_SS_Interrogate_StopWaitPSTimer();
        if(SETTINGS_SS_POWERON_PS_NONE != Settings_SS_Get_PowerOn_PS_Type())
        {
            Settings_SS_Set_PowerOn_PS_Type(SETTINGS_SS_POWERON_PS_NONE);
            MMIAPIPHONE_PowerOffModem();
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

LOCAL BOOLEAN HandleSSOperation_CallBack(ual_cms_msg_t param)
{
    MMI_STRING_T text_str = {0};
    ual_ss_message_e message = param.msg_id;
	ual_ss_param_t param_body = {0};
	ual_ss_data_result_e ss_result = UAL_SS_RESULT_MAX;

    SCI_TRACE_LOW("[divert]:HandleSSOperation_CallBack begin,is_succss = %d",param.msg_id);
	param_body = *(ual_ss_param_t *)param.p_body;

	switch(message)
    {
        case UAL_SS_SUCCESS_CNF:
        {
			ss_result = param_body.ss_data_result;
            SCI_TRACE_LOW("[divert]:HandleSSOperation_CallBack else TRUE,result_type = %d",ss_result);

            switch (ss_result)
            {
                case UAL_SS_ACTIVE:
                {
                    MMI_GetLabelTextByLang(TXT_SET_SS_OPEN_SUCCESS, &text_str);
                    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,IMAGE_SET_DIVERT_AW_SET_SUCCESS,PNULL);
                }
                    break;

                case UAL_SS_DEACTIVE:
                {
                    MMI_GetLabelTextByLang(TXT_SET_SS_CLOSE_SUCCESS, &text_str);
                    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,IMAGE_SET_DIVERT_AW_CLOSE_SUCCESS,PNULL);
                }
                break;
                default:
                {
                    MMI_GetLabelTextByLang(TXT_SUCCESS, &text_str);
                    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,PNULL,PNULL);
                }
                    break;
            }
            break;
        }
        case UAL_SS_FAIL_CNF:
        {
            ss_result = param_body.ss_data_result;
            SCI_TRACE_LOW("[divert]:HandleSSOperation_CallBack else FALSE,error_code = %d",ss_result);

            switch (ss_result)
            {
                case UAL_SS_ACTIVE_FAIL:
                {
                    MMI_GetLabelTextByLang(TXT_SET_SS_OPEN_FAIL, &text_str);
                    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,IMAGE_SET_DIVERT_AW_SET_FAIL,PNULL);
                }
                    break;
                case UAL_SS_DEACTIVE_FAIL:
                {
                    MMI_GetLabelTextByLang(TXT_SET_SS_CLOSE_FAIL, &text_str);
                    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,IMAGE_SET_DIVERT_AW_CLOSE_FAIL,PNULL);
                }
                default:
                {
                    MMI_GetLabelTextByLang(TXT_ERROR, &text_str);
                    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,image_watch_warning_info,PNULL);
                }
                break;
            }
            break;
        }
            break;
        default:
            break;
    }
    if(SETTINGS_SS_POWERON_PS_OPERATION == Settings_SS_Get_PowerOn_PS_Type())
    {
        Settings_SS_Set_PowerOn_PS_Type(SETTINGS_SS_POWERON_PS_NONE);
        MMIAPIPHONE_PowerOffModem();
    }
    Watch_Settings_SS_Set_Operation_Status(FALSE);

    //关闭网络连接窗口
    if(MMK_IsOpenWin(CALLFORWARDING_NETWORK_WIN_ID))
    {
        MMK_CloseWin(CALLFORWARDING_NETWORK_WIN_ID);
    }

    return TRUE;
}
#endif
LOCAL void Settings_CallForwarding_APP_OK( void )
{
    uint16 index = GUILIST_GetCurItemIndex( MMISET_CALLFORWARDING_LIST_CTRL_ID );

    switch (index)
    {
        case SETTINGS_CALLFORWARDING_ON:
        {
            MMK_CreateWin((uint32 *)CALLFORWARDING_NUMBER_WIN_TAB, PNULL);
            break;
        }

        case SETTINGS_CALLFORWARDING_OFF:
        {
            Watch_Settings_SS_Start_Operation(MN_DUAL_SYS_1);
            break;
        }

        default:
        {
            SCI_TRACE_LOW("[divert]:Settings_CallForwarding_APP_OK index invalid");
            break;
        }
    }
}

LOCAL void Settings_CallForwarding_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_CallForwarding_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16          curSelection = 0;
    int32           listOffset = 0;
    uint16          ringId = 0;
    uint16          modeIndex = 0;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    MMI_TEXT_ID_T*  pStr2TextId = TXT_NULL;
    MMI_STRING_T    itemStr = { 0 };
    MMI_CTRL_ID_T   listCtrlId =  MMISET_CALLFORWARDING_LIST_CTRL_ID;

    // get top item offset
    GUILIST_GetTopItemOffset( listCtrlId, &listOffset );
    // get current select item index
    curSelection = GUILIST_GetCurItemIndex( listCtrlId );
    GUILIST_RemoveAllItems( listCtrlId );

    //on
    MMI_GetLabelTextByLang( TXT_COMMON_OPEN, &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

    //off
    MMI_GetLabelTextByLang( TXT_COMMON_CLOSE, &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

    //in monkey mode,user can't do ss.
    if(TRUE == MMIENG_IsMonkeyTestMode())
    {
        GUILIST_SetItemGray(listCtrlId,SETTINGS_CALLFORWARDING_ON,TRUE);
        GUILIST_SetItemGray(listCtrlId,SETTINGS_CALLFORWARDING_OFF,TRUE);
    }

    //set current item
    GUILIST_SetCurItemIndex( listCtrlId, curSelection );
    //set top item offset
    GUILIST_SetTopItemOffset( listCtrlId, listOffset );
}

LOCAL void Settings_CallForwarding_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T   listCtrlId =  MMISET_CALLFORWARDING_LIST_CTRL_ID;

    GUILIST_SetMaxItem( listCtrlId, SETTINGS_CALLFORWARDING_ITEM_MAX,  FALSE );
    MMK_SetAtvCtrl( win_id, listCtrlId );
}

/*****************************************************************************/
//  Description : to handle Settings window message
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingsCallForwardingWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Settings_CallForwarding_OPEN_WINDOW( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            Settings_CallForwarding_FULL_PAINT( win_id );
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_CallForwarding_APP_OK();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_CallForwarding_APP_CANCEL( win_id );
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#ifdef UAL_SS_SUPPORT
LOCAL BOOLEAN HandleSSInterrogate_CallBack(ual_cms_msg_t param)
{
    ual_ss_message_e msg = (uint32)param.msg_id;
	ual_ss_param_t param_body = {0};

	param_body = *(ual_ss_param_t *)param.p_body;

	SCI_TRACE_LOW("[divert]:HandleSSInterrogate_CallBack,message is %d",msg);
	switch(msg)
    {
        case UAL_SS_SUCCESS_CNF:
        {
            ual_ss_data_result_e ss_result = param_body.ss_data_result;
			ual_ss_code_e ss_code = param_body.ss_code;

            SCI_TRACE_LOW("[divert]:HandleSSInterrogate_CallBack,ss_code is %d,ss_result is %d",ss_code,ss_result);
            if((UAL_SS_CFNRC == ss_code)&&
                ((UAL_SS_DEACTIVE == ss_result)||
                (UAL_SS_NOT_PROVITION == ss_result)||
                (UAL_SS_NOT_REGISTER == ss_result)))
            {
                NOTIFYCENTER_NOTIFY_INFO_T divert_notify_info = {0};
                MMI_STRING_T subject = {0};
                MMI_STRING_T name = {0}; 
                MMI_STRING_T content = {0};

                MMI_GetLabelTextByLang( TXT_SET_CALL_FORWARDING, &name);
                MMI_GetLabelTextByLang( TXT_SET_CALL_FORWARDING_DESC, &subject);
                MMI_GetLabelTextByLang( TXT_SET_CALL_FORWARDING_DETAIL, &content);

                divert_notify_info.notify_image = IMAGE_SET_DIVERT_AW_INFO;
                SCI_MEMCPY(divert_notify_info.app_name_wstr,name.wstr_ptr,name.wstr_len*2);
                SCI_MEMCPY(divert_notify_info.title_wstr,name.wstr_ptr,name.wstr_len*2);
                SCI_MEMCPY(divert_notify_info.short_desc_wstr,subject.wstr_ptr,subject.wstr_len*2);
                SCI_MEMCPY(divert_notify_info.detail_desc_wstr,content.wstr_ptr,content.wstr_len*2);
                //通知Notification
                NotifyCenter_Add(&divert_notify_info);

            }
            else
            {
                SCI_TRACE_LOW("[divert]:HandleSSInterrogate_CallBack,ss status is error");
            }
            break;
        }
        default:
        {
            SCI_TRACE_LOW("[divert]:HandleSSInterrogate_CallBack,ss result is error");
            break;
        }
    }

    if(SETTINGS_SS_POWERON_PS_INTERROGATE == Settings_SS_Get_PowerOn_PS_Type())
    {
        Settings_SS_Set_PowerOn_PS_Type(SETTINGS_SS_POWERON_PS_NONE);
        MMIAPIPHONE_PowerOffModem();
    }
    return TRUE;
}

LOCAL void SS_Operation_HandleWaitPSTimer(uint8 timer_id,uint32 param)
{
    MMI_STRING_T        text_str = {0};

    if (s_operation_wait_ps_timer_id == timer_id)
    {
        MMK_StopTimer(s_operation_wait_ps_timer_id);
        s_operation_wait_ps_timer_id = 0;
        //timeout 的event
        SCI_TRACE_LOW("[divert]:SS_Operation_HandleWaitPSTimer wait PS timeout");
        if(SETTINGS_SS_POWERON_PS_OPERATION == Settings_SS_Get_PowerOn_PS_Type())
        {
            Settings_SS_Set_PowerOn_PS_Type(SETTINGS_SS_POWERON_PS_NONE);
            MMIAPIPHONE_PowerOffModem();
        }
        if(TRUE == MMK_IsOpenWin(CALLFORWARDING_NETWORK_WIN_ID))
        {
            MMK_CloseWin(CALLFORWARDING_NETWORK_WIN_ID);
        }
        MMI_GetLabelTextByLang(TXT_ERROR, &text_str);
        Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,image_watch_warning_info,PNULL);
    }
}

LOCAL void Settings_SS_Operation_StartWaitPSTimer(void)
{
    if (NULL != s_operation_wait_ps_timer_id)
    {
        MMK_StopTimer(s_operation_wait_ps_timer_id);
        s_operation_wait_ps_timer_id = 0;
    }
    s_operation_wait_ps_timer_id = MMK_CreateTimerCallback(TIMEOUT_WAIT_PS_MSG, SS_Operation_HandleWaitPSTimer, PNULL, FALSE);
}

LOCAL void Settings_SS_Operation_StopWaitPSTimer(void)
{
    if (NULL != s_operation_wait_ps_timer_id)
    {
        MMK_StopTimer(s_operation_wait_ps_timer_id);
        s_operation_wait_ps_timer_id = 0;
    }
}

LOCAL void SS_Interrogate_HandleWaitPSTimer(uint8 timer_id,uint32 param)
{
    MMI_STRING_T        text_str = {0};

    if (s_interrogate_wait_ps_timer_id == timer_id)
    {
        MMK_StopTimer(s_interrogate_wait_ps_timer_id);
        s_interrogate_wait_ps_timer_id = 0;
        //timeout 的event
        SCI_TRACE_LOW("[divert]:SS_Interrogate_HandleWaitPSTimer wait PS timeout");
        if(SETTINGS_SS_POWERON_PS_INTERROGATE == Settings_SS_Get_PowerOn_PS_Type())
        {
            Settings_SS_Set_PowerOn_PS_Type(SETTINGS_SS_POWERON_PS_NONE);
            MMIAPIPHONE_PowerOffModem();
        }
    }
}

/*****************************************************************************/
//  Discription: start interrorate wait PS timer                             *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_SS_Interrogate_StartWaitPSTimer(void)
{
    if (NULL != s_interrogate_wait_ps_timer_id)
    {
        MMK_StopTimer(s_interrogate_wait_ps_timer_id);
        s_interrogate_wait_ps_timer_id = 0;
    }
    s_interrogate_wait_ps_timer_id = MMK_CreateTimerCallback(TIMEOUT_WAIT_PS_MSG, SS_Interrogate_HandleWaitPSTimer, PNULL, FALSE);
}

LOCAL void Settings_SS_Interrogate_StopWaitPSTimer(void)
{
    if (NULL != s_interrogate_wait_ps_timer_id)
    {
        MMK_StopTimer(s_interrogate_wait_ps_timer_id);
        s_interrogate_wait_ps_timer_id = 0;
    }
}

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
// idle -> settings -> system -> divert
/*****************************************************************************/
//  Discription: start divert function                                       *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_SS_Start_Operation(ual_sim_number_e     sim_num)
{
    ual_ss_error_e ss_err_code = UAL_SS_ERROR_MAX;
    MMI_STRING_T        text_str = {0};
    uint16 index = GUILIST_GetCurItemIndex( MMISET_CALLFORWARDING_LIST_CTRL_ID );

    SCI_TRACE_LOW("[divert]:Watch_Settings_SS_Start_Operation begin");

    if(FALSE == MMIAPIPHONE_GetIsPsReady(sim_num))
    {
        SCI_TRACE_LOW("[divert]:Watch_Settings_SS_Start_Operation PS is not ready");
        MMIAPIPHONE_PowerOnModem();
        Settings_SS_Operation_StartWaitPSTimer();//for bug1625643
        Settings_SS_Set_PowerOn_PS_Type(SETTINGS_SS_POWERON_PS_OPERATION);
        
        if(FALSE == MMK_IsOpenWin(CALLFORWARDING_NETWORK_WIN_ID))
        {
           //进入网络连接界面
           MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &text_str);

           AdultWatchCOM_WaitingWin_1str_WaveIcon_Enter(CALLFORWARDING_NETWORK_WIN_ID,&text_str,HandleNetworkConnectMsg);
        }
    }
    else
    {
#ifdef UAL_SS_SUPPORT
        Settings_SS_Operation_StopWaitPSTimer();//for bug1625643

        switch (index)
        {
            case SETTINGS_CALLFORWARDING_ON:
            {
                ss_err_code = Settings_SS_Start_Request(sim_num,UAL_SS_CFNRC,UAL_SS_REGISTERSS,UAL_SS_MAX_RETRYTIMES,&s_callforwarding_number,HandleSSOperation_CallBack);
                break;
            }
        
            case SETTINGS_CALLFORWARDING_OFF:
            {
                ss_err_code = Settings_SS_Start_Request(sim_num,UAL_SS_CFNRC,UAL_SS_ERASESS,UAL_SS_MAX_RETRYTIMES,PNULL,HandleSSOperation_CallBack);
                break;
            }
        
            default:
            {
                SCI_TRACE_LOW("[divert]:Watch_Settings_SS_Start_Operation index invalid");
                break;
            }
        }

        if (UAL_SS_SUCESS == ss_err_code)
        {
            if(FALSE == MMK_IsOpenWin(CALLFORWARDING_NETWORK_WIN_ID))
            {
               //进入网络连接界面
               MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &text_str);

               AdultWatchCOM_WaitingWin_1str_WaveIcon_Enter(CALLFORWARDING_NETWORK_WIN_ID,&text_str,HandleNetworkConnectMsg);
            }
        }
        else if (UAL_SS_IS_BUSY == ss_err_code)
        {
           MMI_GetLabelTextByLang(TXT_SYS_WAITING_AND_TRY_LATER, &text_str);
           Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,image_watch_warning_info,PNULL);
        }
        else
        {
           MMI_GetLabelTextByLang(TXT_ERROR, &text_str);
           Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,image_watch_warning_info,PNULL);
        }

        if(PNULL != s_callforwarding_number.wstr_ptr)
        {
            SCI_FREE(s_callforwarding_number.wstr_ptr);
        }
#endif
    }
    MMK_CloseWin(CALLFORWARDING_NUMBER_WIN_ID);
}

/*****************************************************************************/
//  Discription: start divert interrogate                                    *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_SS_Start_Interrogate(ual_sim_number_e     sim_num)
{
    Settings_SS_Interrogate_StopWaitPSTimer();//for bug1625643
    Settings_SS_Start_Request(sim_num,UAL_SS_CFNRC,UAL_SS_INTERROGATESS,UAL_SS_MAX_RETRYTIMES,PNULL,HandleSSInterrogate_CallBack);
}
#endif
/*****************************************************************************/
//  Discription: enter setttings->call forwarding window                     *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_CallForwardingWin_Enter( void )
{
    MMI_STRING_T text_str = {0};

    if (0 >= MMIAPIPHONE_GetSimExistedNum(PNULL, 0))
    {
        MMI_GetLabelTextByLang(TXT_CC_NOTE_INSERT_SIM, &text_str);
        Adult_WatchCOM_NoteWin_1Icon_1Str_Enter( CALLFORWARDING_ALERT_WIN_ID,&text_str,IMAGE_SET_DIVERT_AW_CARD_ALERT,PNULL);
    }
    else
    {
        MMK_CreateWin( (uint32 *)MMISET_DIVERT_WIN_TAB, PNULL );
    }
}

/*****************************************************************************/
//  Discription: set ss operation status                                     *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Watch_Settings_SS_Set_Operation_Status(BOOLEAN status)
{
    s_operation_is_doing = status;
    SCI_TRACE_LOW("[divert]:Watch_Settings_SS_Set_Operation_Status,status = %d",status);
}

/*****************************************************************************/
//  Discription: get ss operation status                                     *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC BOOLEAN Watch_Settings_SS_Get_Operation_Status(void)
{
    SCI_TRACE_LOW("[divert]:Watch_Settings_SS_Get_Operation_Status,s_operation_is_doing = %d",s_operation_is_doing);
    return s_operation_is_doing;
}


