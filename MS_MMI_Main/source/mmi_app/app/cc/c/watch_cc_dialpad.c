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
#include "guires.h"
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

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#if (defined MAINLCD_DEV_SIZE_240X320) || (defined MAINLCD_DEV_SIZE_240X240)
// for 240*240 UI
#define DIALPAD_NUM_BUTTON_ROW_MAX              4
#define DIALPAD_NUM_BUTTON_COLUMN_MAX           3
#define DIALDAP_EDITBOX_INPUT_MAX_LEN           255
#ifdef SCREEN_SHAPE_CIRCULAR
#define DIALPAD_DELETE_BUTTON_RECT              DP2PX_RECT(184,34,208,60)
#define DIALPAD_EDITBOX_BIG_FONT                SONG_FONT_20
#define DIALPAD_EDITBOX_SMALL_FONT              SONG_FONT_14
#define DIALPAD_EDITBOX_RECT_LEFT               DP2PX_VALUE(56)
#define DIALPAD_EDITBOX_RECT_RIGHT              DP2PX_VALUE(184)
#define DIALPAD_EDITBOX_RECT_TOP                DP2PX_VALUE(32)
#define DIALPAD_EDITBOX_RECT_BOTTOM             DP2PX_VALUE(60)
#else
#define DIALPAD_DELETE_BUTTON_RECT              {210,8,234,32}
#define DIALPAD_EDITBOX_BIG_FONT                SONG_FONT_24
#define DIALPAD_EDITBOX_SMALL_FONT              SONG_FONT_16
#define DIALPAD_EDITBOX_RECT_LEFT               5
#define DIALPAD_EDITBOX_RECT_RIGHT              202
#define DIALPAD_EDITBOX_RECT_TOP                0
#define DIALPAD_EDITBOX_RECT_BOTTOM             39
#endif
#else
// for 128*128 UI
#define DIALPAD_NUM_BUTTON_ROW_MAX              4
#define DIALPAD_NUM_BUTTON_COLUMN_MAX           3
#define DIALDAP_EDITBOX_INPUT_MAX_LEN           255
#define DIALPAD_DELETE_BUTTON_RECT              {167,24,190,47}
#define DIALPAD_EDITBOX_BIG_FONT                SONG_FONT_16
#define DIALPAD_EDITBOX_SMALL_FONT              SONG_FONT_12
#define DIALPAD_EDITBOX_RECT_LEFT        		42
#define DIALPAD_EDITBOX_RECT_RIGHT       		167
#define DIALPAD_EDITBOX_RECT_TOP         		21
#define DIALPAD_EDITBOX_RECT_BOTTOM      		50
#endif



/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL MMI_IMAGE_ID_T s_dial_num_button_image_order[DIALPAD_NUM_BUTTON_ROW_MAX][DIALPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {IAMGE_CALL_DIALPAD_NUM1, IAMGE_CALL_DIALPAD_NUM2, IAMGE_CALL_DIALPAD_NUM3},
    {IAMGE_CALL_DIALPAD_NUM4, IAMGE_CALL_DIALPAD_NUM5, IAMGE_CALL_DIALPAD_NUM6},
    {IAMGE_CALL_DIALPAD_NUM7, IAMGE_CALL_DIALPAD_NUM8, IAMGE_CALL_DIALPAD_NUM9},
#ifndef SCREEN_SHAPE_CIRCULAR
    {IAMGE_CALL_DIALPAD_NUM0, IAMGE_CALL_DIALPAD_DIAL, IAMGE_CALL_DIALPAD_SYMBOL},
#else
    {IAMGE_CALL_DIALPAD_ASTERISK, IAMGE_CALL_DIALPAD_NUM0, IAMGE_CALL_DIALPAD_POUND},
#endif
};

LOCAL MMI_CTRL_ID_T s_dial_num_button_ctrl_order[DIALPAD_NUM_BUTTON_ROW_MAX][DIALPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {WATCH_DIALPAD_NUM1_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM2_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM3_BUTTON_CTRL_ID},
    {WATCH_DIALPAD_NUM4_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM5_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM6_BUTTON_CTRL_ID},
    {WATCH_DIALPAD_NUM7_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM8_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM9_BUTTON_CTRL_ID},
#ifndef SCREEN_SHAPE_CIRCULAR
    {WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID, WATCH_DIALPAD_DIAL_BUTTON_CTRL_ID, WATCH_DIALPAD_SYMBOL_BUTTON_CTRL_ID},
#else
    {WATCH_DIALPAD_ASTERISK_BUTTON_CTRL_ID, WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID, WATCH_DIALPAD_POUND_BUTTON_CTRL_ID},
#endif
};


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
    CREATE_BUTTON_CTRL(IAMGE_CALL_DIALPAD_DELETE, WATCH_DIALPAD_DELETE_BUTTON_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};//dialpad win


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

#ifdef SCREEN_SHAPE_CIRCULAR
    GUIRES_GetImgWidthHeight(&imgWidth, &imgHeight, IAMGE_CALL_DIALPAD_NUM0, win_id);
#else
    GUIRES_GetImgWidthHeight(&imgWidth, &imgHeight, IAMGE_CALL_DIALPAD_DIAL, win_id);
#endif
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
#if (defined MAINLCD_DEV_SIZE_240X320) || (defined MAINLCD_DEV_SIZE_240X240)
#ifndef SCREEN_SHAPE_CIRCULAR
            buttonRect.v_rect.left = 6 + (imgWidth + 6)* column;
            buttonRect.v_rect.top = 40 + (imgHeight + 6)* row ;
#else
            buttonRect.v_rect.left = DP2PX_VALUE(44) + (imgWidth + DP2PX_VALUE(4))* column;
            buttonRect.v_rect.top = DP2PX_VALUE(64) + (imgHeight + DP2PX_VALUE(4))* row ;
#endif
#else
            buttonRect.v_rect.left = 36 + (imgWidth + 2)* column;
            buttonRect.v_rect.top = 52 + (imgHeight + 2)* row ;
#endif
            buttonRect.v_rect.right = buttonRect.v_rect.left + imgWidth;
            buttonRect.v_rect.bottom = buttonRect.v_rect.top + imgHeight;

            CreateDialpadButton(win_id, s_dial_num_button_ctrl_order[row][column], s_dial_num_button_image_order[row][column], &buttonRect);
        }
    }
#ifdef SCREEN_SHAPE_CIRCULAR
    //Create dial button
    buttonRect.v_rect.left = DP2PX_VALUE(96);
    buttonRect.v_rect.top = DP2PX_VALUE(192);
    buttonRect.v_rect.right = DP2PX_VALUE(144);
    buttonRect.v_rect.bottom = DP2PX_VALUE(240);
    CreateDialpadButton(win_id,WATCH_DIALPAD_DIAL_BUTTON_CTRL_ID,IAMGE_CALL_DIALPAD_DIAL,&buttonRect);
#endif
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
            MMK_CloseWin(win_id);
            break;

       case MSG_CTL_PENOK:
            HandleWatchDialPenOkMsg(win_id,editCtrlId,((MMI_NOTIFY_T*)param)->src_id);
            break;

        case MSG_CTL_PENLONGOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                    case WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID:
                    {
#ifndef SCREEN_SHAPE_CIRCULAR
                        MMK_SendMsg(editCtrlId,MSG_APP_PLUS,PNULL);
#else
                        MMK_SendMsg(win_id,MSG_APP_PLUS,PNULL);
#endif
                        break;
                    }
                    case WATCH_DIALPAD_SYMBOL_BUTTON_CTRL_ID:
                    {
                        MMK_SendMsg(editCtrlId,MSG_APP_HASH,PNULL);
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
                WatchAPICC_MakeCall(MN_DUAL_SYS_1, tempStr, (uint8)editStr.wstr_len,
                                    PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
                GUIEDIT_ClearAllStr(editCtrlId); //bug 1325878
            }
            break;
        }

#ifdef SCREEN_SHAPE_CIRCULAR
        case MSG_APP_PLUS:
        {
            MMI_STRING_T editStr = {0};
            uint8        curStr[DIALDAP_EDITBOX_INPUT_MAX_LEN] = {0};
            uint8        tempStr[DIALDAP_EDITBOX_INPUT_MAX_LEN] = {0};
            wchar        tempwstr[DIALDAP_EDITBOX_INPUT_MAX_LEN] = {0};
            GUIEDIT_GetString(editCtrlId, &editStr);
            MMI_WSTRNTOSTR((uint8 *)curStr, DIALDAP_EDITBOX_INPUT_MAX_LEN,
                editStr.wstr_ptr, editStr.wstr_len,
                editStr.wstr_len);
            sprintf((uint8 *)tempStr, "%s+", (uint8 *)curStr);
            MMI_STRNTOWSTR(tempwstr, DIALDAP_EDITBOX_INPUT_MAX_LEN, (uint8 *)tempStr, SCI_STRLEN(tempStr), SCI_STRLEN(tempStr));
            editStr.wstr_ptr = tempwstr;
            editStr.wstr_len = MMIAPICOM_Wstrlen(tempwstr);
            GUIEDIT_SetString(editCtrlId, editStr.wstr_ptr, editStr.wstr_len);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break; // pclint bug 2099193
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
#ifdef SCREEN_SHAPE_CIRCULAR
        case WATCH_DIALPAD_ASTERISK_BUTTON_CTRL_ID:
        case WATCH_DIALPAD_POUND_BUTTON_CTRL_ID:
#endif
        {
            keyMsgId = MSG_APP_1 + ctrl_id - WATCH_DIALPAD_NUM1_BUTTON_CTRL_ID;
            //handle key
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),keyMsgId,PNULL);
            break;
        }
        case WATCH_DIALPAD_NUM0_BUTTON_CTRL_ID:
        {
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_APP_0,PNULL);
            break;
        }
        case WATCH_DIALPAD_SYMBOL_BUTTON_CTRL_ID:
        {
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_APP_STAR,PNULL);
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
                WatchAPICC_MakeCall(MN_DUAL_SYS_1, editNumStr, (uint8)editStr.wstr_len,
                                    PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
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




