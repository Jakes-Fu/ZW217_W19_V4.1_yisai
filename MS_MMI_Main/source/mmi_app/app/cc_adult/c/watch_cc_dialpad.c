/*****************************************************************************
** File Name:      watch_commonwin_dialpad.c                                 *
** Author:         yuming.yang                                               *
** Date:           04/04/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw dialpad                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 04/2020               yuming.yang            Create                       *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_cc_trc.h"
#include "std_header.h"
#include "mmi_appmsg.h"
#include "mmi_resource.h"
#include "mmi_id.h"
#include "guitext.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guicommon.h"
#include "guistring.h"
#include "mmidisplay_data.h"
#include "mmicc_export.h"
#include "mmicc_internal.h"
#include "mmicom_string.h"
#include "watch_common.h"
#include "mmiset_export.h"
#include "watch_commonwin_export.h"
#include "mmicom_trace.h"
#include "watch_cc_view.h"
#include "mmicc_id.h"
#include "mmicc_image.h"
#include "guires.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define DIALPAD_NUM_BUTTON_ROW_MAX               4
#define DIALPAD_NUM_BUTTON_COLUMN_MAX            3
#define DIALDAP_EDITBOX_INPUT_MAX_LEN            255
#define DIALPAD_DELETE_BUTTON_RECT               DP2PX_RECT(184,32,208,60)
#define DIALPAD_EDITBOX_BIG_FONT                 DP_FONT_20
#define DIALPAD_EDITBOX_SMALL_FONT               DP_FONT_14
#define DIALPAD_EDITBOX_RECT_LEFT        DP2PX_VALUE(56)
#define DIALPAD_EDITBOX_RECT_RIGHT       DP2PX_VALUE(184)
#define DIALPAD_EDITBOX_RECT_TOP         DP2PX_VALUE(34)
#define DIALPAD_EDITBOX_RECT_BOTTOM      DP2PX_VALUE(57)



/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL MMI_IMAGE_ID_T s_dial_num_button_image_order[DIALPAD_NUM_BUTTON_ROW_MAX][DIALPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {res_aw_dialpad_ic_num1, res_aw_dialpad_ic_num2, res_aw_dialpad_ic_num3},
    {res_aw_dialpad_ic_num4, res_aw_dialpad_ic_num5, res_aw_dialpad_ic_num6},
    {res_aw_dialpad_ic_num7, res_aw_dialpad_ic_num8, res_aw_dialpad_ic_num9},
    {res_aw_dialpad_ic_asterisk, res_aw_dialpad_ic_num0, res_aw_dialpad_ic_pound},
};

LOCAL MMI_CTRL_ID_T s_dial_num_button_ctrl_order[DIALPAD_NUM_BUTTON_ROW_MAX][DIALPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {WATCH_DIALPAD_NUM1_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM2_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM3_BUTTON_CTRL_ID},
    {WATCH_DIALPAD_NUM4_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM5_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM6_BUTTON_CTRL_ID},
    {WATCH_DIALPAD_NUM7_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM8_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM9_BUTTON_CTRL_ID},
    {WATCH_DIALPAD_ASTERISK_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID, WATCH_DIALPAD_POUND_BUTTON_CTRL_ID},
};

LOCAL MMI_IMAGE_ID_T s_dial_num_button_dial_image = res_aw_dialpad_ic_dial;
LOCAL MMI_CTRL_ID_T  s_dial_num_button_dial_id = WATCH_DIALPAD_DIAL_BUTTON_CTRL_ID;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 创建拨号盘中的编辑框
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateDialpadEditBox(MMI_WIN_ID_T win_id, const GUI_BOTH_RECT_T *edit_rect_ptr);

/*****************************************************************************/
//  Description : 创建拨号盘中的button控件
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateDialpadButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T button_ctrl, MMI_IMAGE_ID_T button_image, const GUI_BOTH_RECT_T *rect_ptr);
/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateDialpadCtrl(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件(不含dial和delete)
//  Global resource dependence : 
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateDialpadCtrlWithoutDialAndDelete(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : handle dialpad win message
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleDialpadWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : handle dialpad win message
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialpadWinInCallMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : handle dial window pen ok msg
//  Global resource dependence : 
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void HandleWatchDialPenOkMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_CTRL_ID_T     edit_ctrl_id,
                                          MMI_CTRL_ID_T     ctrl_id
                                          );


/*---------------------------------------------------------------------------*/
/*                          WINDOW TABLE DEFINITION                          */
/*---------------------------------------------------------------------------*/

WINDOW_TABLE(WATCH_DIALPAD_WIN_TAB) =
{
    WIN_ID(WATCH_DIALPAD_WIN_ID),
    WIN_FUNC((uint32)HandleDialpadWinMsg),
    WIN_HIDE_STATUS,
    CREATE_BUTTON_CTRL(res_aw_dialpad_ic_delete, WATCH_DIALPAD_DELETE_BUTTON_CTRL_ID),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};//dialpad win

    WINDOW_TABLE(WATCH_DIALPAD_IN_CALL_WIN_TAB) =
{
    WIN_ID(WATCH_DIALPAD_IN_CALL_WIN_ID),
    WIN_FUNC((uint32)HandleDialpadWinInCallMsg),
    WIN_HIDE_STATUS,
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};//dialpad win without dial and delete
/*---------------------------------------------------------------------------*/
/*                             LOCAL FUNCTION                                */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建拨号盘中的编辑框
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateDialpadEditBox(MMI_WIN_ID_T win_id, const GUI_BOTH_RECT_T *edit_rect_ptr)
{
    MMI_CONTROL_CREATE_T        editCtrl = {0};
    GUI_BG_T                    bgInfo = {0};
    MMI_CTRL_ID_T               editCtrlId = WATCH_DIALPAD_EDITBOX_CTRL_ID;
    GUIEDIT_INIT_DATA_T         editInit = {0};
    GUIEDIT_FONT_T              editFont ={0};
    MMI_HANDLE_T                editHandle = 0;

    SCI_MEMCPY(&editInit.both_rect, edit_rect_ptr, sizeof(GUI_BOTH_RECT_T));
    editInit.type        = GUIEDIT_TYPE_PHONENUM;
    editInit.str_max_len = DIALDAP_EDITBOX_INPUT_MAX_LEN;
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
    //GUIEDIT_SetCursorHideState(editHandle, TRUE);
    //set SetClipboard Disenabled  bug 1623976
    CTRLBASEFLEX_SetClipboardEnabled(editCtrlId, FALSE);

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
LOCAL void CreateDialpadButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T button_ctrl, MMI_IMAGE_ID_T button_image, const GUI_BOTH_RECT_T *rect_ptr)
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

/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件
//  Global resource dependence : 
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateDialpadCtrl(MMI_WIN_ID_T win_id)
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
    CreateDialpadEditBox(win_id, &editBoxRect);
    // backspace按键
    GUIBUTTON_SetRect(WATCH_DIALPAD_DELETE_BUTTON_CTRL_ID, &delBtnRect);



    // button控件
    for (row = 0; row < DIALPAD_NUM_BUTTON_ROW_MAX; row++)
    {
        for (column = 0; column < DIALPAD_NUM_BUTTON_COLUMN_MAX; column++)
        {
            buttonRect.v_rect.left = DP2PX_VALUE(44) + (imgWidth + DP2PX_VALUE(4))* column;
            buttonRect.v_rect.top = DP2PX_VALUE(64) + (imgHeight + DP2PX_VALUE(4))* row ;
            buttonRect.v_rect.right = buttonRect.v_rect.left + imgWidth;
            buttonRect.v_rect.bottom = buttonRect.v_rect.top + imgHeight;

            CreateDialpadButton(win_id, s_dial_num_button_ctrl_order[row][column], s_dial_num_button_image_order[row][column], &buttonRect);
        }
    }

    //Create dial button 
    buttonRect.v_rect.left = DP2PX_VALUE(0);
    buttonRect.v_rect.top = DP2PX_VALUE(196);
    buttonRect.v_rect.right = DP2PX_VALUE(240);
    buttonRect.v_rect.bottom = DP2PX_VALUE(228);
    CreateDialpadButton(win_id,s_dial_num_button_dial_id,s_dial_num_button_dial_image,&buttonRect);

}
/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件(不含dial和delete)
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CreateDialpadCtrlWithoutDialAndDelete(MMI_WIN_ID_T win_id)
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
    CreateDialpadEditBox(win_id, &editBoxRect);



    // button控件
    for (row = 0; row < DIALPAD_NUM_BUTTON_ROW_MAX; row++)
    {
        for (column = 0; column < DIALPAD_NUM_BUTTON_COLUMN_MAX; column++)
        {
            buttonRect.v_rect.left = DP2PX_VALUE(44) + (imgWidth + DP2PX_VALUE(4))* column;
            buttonRect.v_rect.top = DP2PX_VALUE(64) + (imgHeight + DP2PX_VALUE(4))* row ;
            buttonRect.v_rect.right = buttonRect.v_rect.left + imgWidth;
            buttonRect.v_rect.bottom = buttonRect.v_rect.top + imgHeight;

            CreateDialpadButton(win_id, s_dial_num_button_ctrl_order[row][column], s_dial_num_button_image_order[row][column], &buttonRect);
        }
    }
}

/*****************************************************************************/
//  Description : handle dialpad win message
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialpadWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           editCtrlId = WATCH_DIALPAD_EDITBOX_CTRL_ID;

    SCI_TRACE_LOW("[WatchCOM] HandleDialpadWinMsg msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            CreateDialpadCtrl(win_id);
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
            MMK_ReturnIdleWin();
            //MMK_CloseWin(win_id);
            break;

       case MSG_CTL_PENOK:
            if(PNULL != param)
            {
                HandleWatchDialPenOkMsg(win_id,editCtrlId,((MMI_NOTIFY_T*)param)->src_id);
            }
            break;

        case MSG_CTL_PENLONGOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                    case WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID:
                    {
                        MMK_SendMsg(editCtrlId,MSG_APP_PLUS,PNULL);
                        break;
                    }
                    case WATCH_DIALPAD_DELETE_BUTTON_CTRL_ID:
                    {
                        //MMK_SendMsg(editCtrlId,MSG_NOTIFY_IM_LONG_DEL,PNULL);
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }

        case MSG_APP_GREEN:
		{
            uint8           tempStr[DIALDAP_EDITBOX_INPUT_MAX_LEN]  = {0};
            MMI_STRING_T    editStr = {0};

            //get edit string
            GUIEDIT_GetString(editCtrlId,&editStr);

            if (0 == editStr.wstr_len)
            {
                break;
            }
            else
            {
                //convert string
                MMI_WSTRNTOSTR((uint8 *)tempStr,DIALDAP_EDITBOX_INPUT_MAX_LEN,
                    editStr.wstr_ptr,editStr.wstr_len,
                    editStr.wstr_len);
#ifndef ADULT_WATCH_SUPPORT
                WatchAPICC_MakeCall(MN_DUAL_SYS_1, tempStr, (uint8)editStr.wstr_len,
                                    PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
#else
                WatchAPICC_DialNumber(tempStr, (uint8)editStr.wstr_len);
#endif
                GUIEDIT_ClearAllStr(editCtrlId); //bug 1325878
            }
			break;
		}
        default:
            result = MMI_RESULT_FALSE;
            break;

    }
    return result;
}
/*****************************************************************************/
//  Description : handle dialpad win message
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialpadWinInCallMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           editCtrlId = WATCH_DIALPAD_EDITBOX_CTRL_ID;

    SCI_TRACE_LOW("[WatchCOM] HandleDialpadWinMsg msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            CreateDialpadCtrlWithoutDialAndDelete(win_id);
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
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_PENOK:
		if(PNULL != param)
		{
			HandleWatchDialPenOkMsg(win_id,editCtrlId,((MMI_NOTIFY_T*)param)->src_id);
		}
        break;

    case MSG_CTL_PENLONGOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                case WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID:
                    {
                        MMK_SendMsg(editCtrlId,MSG_APP_PLUS,PNULL);
                        break;
                    }
                case WATCH_DIALPAD_DELETE_BUTTON_CTRL_ID:
                    {
                        //MMK_SendMsg(editCtrlId,MSG_NOTIFY_IM_LONG_DEL,PNULL);
                        break;
                    }
                default:
                    break;
                }
            }
            break;
        }
#if 0
	case MSG_APP_GREEN:
        {
            uint8           tempStr[DIALDAP_EDITBOX_INPUT_MAX_LEN]  = {0};
            MMI_STRING_T    editStr = {0};

            //get edit string
            GUIEDIT_GetString(editCtrlId,&editStr);

            if (0 == editStr.wstr_len)
            {
                break;
            }
            else
            {
                //convert string
                MMI_WSTRNTOSTR((uint8 *)tempStr,DIALDAP_EDITBOX_INPUT_MAX_LEN,
                    editStr.wstr_ptr,editStr.wstr_len,
                    editStr.wstr_len);
                WatchAPICC_MakeCall(MN_DUAL_SYS_1, tempStr, (uint8)editStr.wstr_len,
                    PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
                GUIEDIT_ClearAllStr(editCtrlId); //bug 1325878
            }
            break;
        }
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;

    }
    return result;
}
/*****************************************************************************/
//  Description : handle dial window pen ok msg
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void HandleWatchDialPenOkMsg(
                                              MMI_WIN_ID_T      win_id,
                                              MMI_CTRL_ID_T     edit_ctrl_id,
                                              MMI_CTRL_ID_T     ctrl_id
                                              )
{
    uint8               editNumStr[DIALDAP_EDITBOX_INPUT_MAX_LEN] = {0};
    MMI_STRING_T        editStr = {0};
    MMI_MESSAGE_ID_E    keyMsgId = 0;

    //get edit string
    GUIEDIT_GetString(edit_ctrl_id,&editStr);

    MMI_WSTRNTOSTR((uint8 *)editNumStr,DIALDAP_EDITBOX_INPUT_MAX_LEN,
        editStr.wstr_ptr,editStr.wstr_len,
        editStr.wstr_len);


    switch (ctrl_id)
    {
        case WATCH_DIALPAD_NUM1_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM2_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM3_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM4_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM5_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM6_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM7_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM8_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM9_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_ASTERISK_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_POUND_BUTTON_CTRL_ID:
        {
            keyMsgId = MSG_APP_1 + ctrl_id - WATCH_DIALPAD_NUM1_BUTTON_CTRL_ID;
            //handle key
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),keyMsgId,PNULL);
            MMIAPIIDLE_PlayDialTone(keyMsgId, TRUE);
            MMICC_SendSingleDtmf(keyMsgId);
            break;
        }
        case WATCH_DIALPAD_DIAL_BUTTON_CTRL_ID:
        {
            if (0 == editStr.wstr_len)
            {
                break;
            }
            else
            {
#ifndef ADULT_WATCH_SUPPORT
                WatchAPICC_MakeCall(MN_DUAL_SYS_1, editNumStr, (uint8)editStr.wstr_len,
                                    PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
#else
                WatchAPICC_DialNumber(editNumStr, (uint8)editStr.wstr_len);
#endif
            }
            break;
        }
        case WATCH_DIALPAD_DELETE_BUTTON_CTRL_ID:
        {
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


/*---------------------------------------------------------------------------*/
/*                             PUBLIC FUNCTION                               */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Open cc dialpad window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_DialpadWin_Enter(void)
{
    MMK_CreateWin((uint32 *)WATCH_DIALPAD_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : Open cc dialpad window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void Adult_WatchCC_DialpadWin_Enter(void)
{
    MMK_CreateWin((uint32 *)WATCH_DIALPAD_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : Exit cc dialpad window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void Adult_WatchCC_DialpadWin_Exit(void)
{
    MMK_CloseWin(WATCH_DIALPAD_WIN_ID);
}

/*****************************************************************************/
//  Description : Open cc dialpad window  whether not need Dial and Delete
//  Parameter: [In] is_not_need_dial_and_delete
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void Adult_WatchCC_DialpadWinInCall_Enter(void)
{
    MMK_CreateWin((uint32 *)WATCH_DIALPAD_IN_CALL_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : Exit cc dialpad window  whether not need Dial and Delete
//  Parameter: [In] is_not_need_dial_and_delete
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void Adult_WatchCC_DialpadWinInCall_Exit(void)
{
    MMK_CloseWin(WATCH_DIALPAD_IN_CALL_WIN_ID);
}




