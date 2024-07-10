/*****************************************************************************
** File Name:      mmiphone_pinpuk.c                                         *
** Author:                                                                   *
** Date:           05/2020                                                   *
** Copyright:                                                                *
** Description:    This file is used to describe phone pinpuk module         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2020       fangfang.yao       Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_appmsg.h"
#include "mmi_id.h"
#include "mmipub.h"
#include "guitext.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guicommon.h"
#include "guistring.h"
#include "mmidisplay_data.h"
#include "mmicc_export.h"
#include "mmicc_internal.h"
#include "mmicom_string.h"
#include "mmiset_export.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "watch_common.h"
#include "watch_commonwin_export.h"
#include "watch_cc_view.h"
#include "mmieng_export.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct  //定义自己的时间类型
{
    MMI_CTRL_ID_T editCtrl;
    PROCESSMSG_FUNC pEditFunc;
}WATCHCOM_EDITPAD_INFO_T;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
// TIP
#define EDITPAD_LABEL_FONT              WATCH_DEFAULT_NORMAL_FONT
#define EDITPAD_LABEL_FONT_COLOR        MMI_WHITE_COLOR

//EDITBOX
#define EDITPAD_EDITBOX_INPUT_MAX_LEN   8
#define EDITPAD_EDITBOX_INPUT_MIN_LEN   4
#define EDITPAD_EDITBOX_FONT            WATCH_DEFAULT_NORMAL_FONT

#if defined SCREEN_SHAPE_CIRCULAR
#define EDITPAD_LABEL_RECT              DP2PX_RECT(36, 0, 203, 40)
#define EDITPAD_EDITBOX_RECT_LEFT       DP2PX_VALUE(36)
#define EDITPAD_EDITBOX_RECT_RIGHT      DP2PX_VALUE(203)
#define EDITPAD_EDITBOX_RECT_TOP        DP2PX_VALUE(44)
#define EDITPAD_EDITBOX_RECT_BOTTOM     DP2PX_VALUE(84)
#define EDITPAD_BUTTON_TO_BUTTON_SPACE  DP2PX_VALUE(4)
#define EDITPAD_BUTTON_RECT_LEFT        DP2PX_VALUE(44)
#elif defined SCREEN_SHAPE_SQUARE
#define EDITPAD_LABEL_RECT              { 6, 8, 233, 31}
#define EDITPAD_EDITBOX_RECT_LEFT       6
#define EDITPAD_EDITBOX_RECT_RIGHT      233
#define EDITPAD_EDITBOX_RECT_TOP        40
#define EDITPAD_EDITBOX_RECT_BOTTOM     79
#define EDITPAD_BUTTON_TO_BUTTON_SPACE  6
#endif

//BUTTON
#define EDITPAD_BUTTON_TOP_MIN          88


#define EDITPAD_NUM_BUTTON_ROW_MAX      4
#define EDITPAD_NUM_BUTTON_COLUMN_MAX   3

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL MMI_IMAGE_ID_T s_edit_num_button_image_order[EDITPAD_NUM_BUTTON_ROW_MAX][EDITPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {IAMGE_EDITPAD_NUM1,        IAMGE_EDITPAD_NUM2,     IAMGE_EDITPAD_NUM3},
    {IAMGE_EDITPAD_NUM4,        IAMGE_EDITPAD_NUM5,     IAMGE_EDITPAD_NUM6},
    {IAMGE_EDITPAD_NUM7,        IAMGE_EDITPAD_NUM8,     IAMGE_EDITPAD_NUM9},
    {IAMGE_EDITPAD_OK_DISABLE,  IAMGE_EDITPAD_NUM0,     IAMGE_EDITPAD_CLEAN_DISABLE},
};

LOCAL MMI_CTRL_ID_T s_edit_num_button_ctrl_order[EDITPAD_NUM_BUTTON_ROW_MAX][EDITPAD_NUM_BUTTON_COLUMN_MAX] =
{
    {WATCH_EDITPAD_NUM1_BUTTON_CTRL_ID, WATCH_EDITPAD_NUM2_BUTTON_CTRL_ID, WATCH_EDITPAD_NUM3_BUTTON_CTRL_ID},
    {WATCH_EDITPAD_NUM4_BUTTON_CTRL_ID, WATCH_EDITPAD_NUM5_BUTTON_CTRL_ID, WATCH_EDITPAD_NUM6_BUTTON_CTRL_ID},
    {WATCH_EDITPAD_NUM7_BUTTON_CTRL_ID, WATCH_EDITPAD_NUM8_BUTTON_CTRL_ID, WATCH_EDITPAD_NUM9_BUTTON_CTRL_ID},
    {WATCH_EDITPAD_OK_BUTTON_CTRL_ID,   WATCH_EDITPAD_NUM0_BUTTON_CTRL_ID, WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID},
};

LOCAL MMIPHONE_PIN_PUK_STATE_TYPE_E s_pinpuk_state = MMIPHONE_PIN_PUK_STATE_WAIT;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

// editpad start ======================================================================================================

/*****************************************************************************/
//  Description : edit window left softkey
//  Parameter: [In] str_ptr
//             [In] str_len
//             [In] text_id_ptr
//             [Out] text_id_ptr
//             [Return] None
//  Author: fangfang.yao
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN EditpadLeftWinSoftkey(
                                      wchar         *str_ptr,
                                      uint16        str_len,
                                      MMI_TEXT_ID_T *text_id_ptr
                                      );

/*****************************************************************************/
//  Description : edit window right softkey
//  Parameter: [In] str_ptr
//             [In] str_len
//             [In] text_id_ptr
//             [Out] text_id_ptr
//             [Return] None
//  Author: fangfang.yao
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN EditpadRightWinSoftkey(
                                      wchar         *str_ptr,
                                      uint16        str_len,
                                      MMI_TEXT_ID_T *text_id_ptr
                                      );

/*****************************************************************************/
//  Description : handle Editpad win message
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditpadWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description: 创建拨号盘中的文本框
//  Parameter: [In] win_id : win id
//             [In] tip_str_ptr : label display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadLabel(
                                    MMI_WIN_ID_T    win_id,
                                    MMI_STRING_T    *tip_str_ptr
                                   );

/*****************************************************************************/
//  Description : 创建拨号盘中的编辑框
//  Parameter: [In] win_id : win_id
//             [In] edit_ctrl :editbox ctrl id
//             [In] edit_rect_ptr :editbox display rect
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadEditBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T edit_ctrl,const GUI_BOTH_RECT_T *edit_rect_ptr);

/*****************************************************************************/
//  Description : 创建编辑框中的button控件
//  Parameter: [In] win_id : win_id
//             [In] button_ctrl :button ctrl id
//             [In] button_image : button image
//             [In] rect_ptr : button display rect
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadButton( MMI_WIN_ID_T win_id,
                                        MMI_CTRL_ID_T button_ctrl,
                                        MMI_IMAGE_ID_T button_image,
                                        const GUI_BOTH_RECT_T *rect_ptr
                                        );

/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件
//  Parameter: [In] win_id : win_id
//             [In] edit_ctrl :editbox ctrl id
//             [In] tip_str_ptr :label ctrl id
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadCtrl(MMI_WIN_ID_T win_id , MMI_CTRL_ID_T edit_ctrl, MMI_STRING_T *tip_str_ptr);

/*****************************************************************************/
//  Description : 销毁拨号盘中的窗口数据
//  Parameter: [In] win_id
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CommonWin_EditpadWin_DestoryData( MMI_WIN_ID_T win_id );

/*****************************************************************************/
//  Description : 创建拨号盘中的窗口
//  Parameter: [In] win_id : win_id
//             [In] userData :app user data
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CommonWin_EditpadWin_CreateWin( MMI_WIN_ID_T win_id , WATCHCOM_EDITPAD_INFO_T *userData);

/*****************************************************************************/
//  Description : Open Editpad window
//  Parameter: [In] win_id : commonwin ID
//             [In] edit_ctrl : editbox display string
//             [In] tip_str : label display string
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void PHONE_EditpadWin_Enter(
                                                    MMI_WIN_ID_T    win_id,
                                                    MMI_CTRL_ID_T   edit_ctrl,
                                                    MMI_STRING_T    *tip_str,
                                                    PROCESSMSG_FUNC editpadFunc
                                                );

// editpad end ========================================================================================================

LOCAL MMI_RESULT_E HandleComfirmPinInputNumber(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL MMI_RESULT_E HandleNewPinInputNumber(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL MMI_RESULT_E HandlePukInputNumber(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL MMI_RESULT_E HandlePinInputNumber(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
LOCAL MMI_RESULT_E HandlePinIsBlocked(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );
LOCAL MMI_RESULT_E HandleMsgPinPukNoteWinWindow(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );

LOCAL void PHONE_CheckPukAndInputNewPin( MMI_STRING_T pukStr, MMI_STRING_T pinStr);
LOCAL MMI_RESULT_E PHONE_OpenInputPinWindow(APP_MN_SIM_GET_PIN_IND_T sim_get_pin_ind);

LOCAL BOOLEAN PHONE_ClosePinPukWaitingWin_CallBack( uint8 timer_id );

LOCAL void PHONE_ClosePinPukWin(void);

LOCAL BOOLEAN PHONE_OpenWaitingPinPukWin( void );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

// editpad start ======================================================================================================

/*****************************************************************************/
//  Description : edit window left softkey
//  Parameter: [In] str_ptr
//             [In] str_len
//             [In] text_id_ptr
//             [Out] text_id_ptr
//             [Return] None
//  Author: fangfang.yao
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN EditpadLeftWinSoftkey(
                                      wchar         *str_ptr,
                                      uint16        str_len,
                                      MMI_TEXT_ID_T *text_id_ptr
                                      )
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_input_emergency_call = FALSE;
    uint16          emergency_call_len = 0;
    MMI_STRING_T    str_info = {0};

    //set string info
    str_info.wstr_ptr = str_ptr;
    str_info.wstr_len = str_len;

    is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMI_DUAL_SYS_MAX);
    if ((is_input_emergency_call) && (str_len == emergency_call_len))
    {
        *text_id_ptr = TXT_SOS;
        result = TRUE;
    }
    else if( str_len >= EDITPAD_EDITBOX_INPUT_MIN_LEN )
    {
        *text_id_ptr = STXT_OK;
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : edit window right softkey
//  Parameter: [In] str_ptr
//             [In] str_len
//             [In] text_id_ptr
//             [Out] text_id_ptr
//             [Return] None
//  Author: fangfang.yao
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN EditpadRightWinSoftkey(
                                      wchar         *str_ptr,
                                      uint16        str_len,
                                      MMI_TEXT_ID_T *text_id_ptr
                                      )
{
    BOOLEAN         result = FALSE;
    if ( str_len > 0 )
    {
        *text_id_ptr = TXT_DELETE;
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle Editpad win message
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditpadWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E    result      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   editCtrlId  = 0;
    GUI_BG_T        btnInfo     = {0};
    MMI_STRING_T    editStr     = {0};
    BOOLEAN         is_input_emergency_call = FALSE;
    uint16          emergency_call_len = 0;
    WATCHCOM_EDITPAD_INFO_T *editInfo = (WATCHCOM_EDITPAD_INFO_T *)MMK_GetWinAddDataPtr( win_id );

    if( editInfo != PNULL)
    {
        editCtrlId = editInfo->editCtrl;
    }
    btnInfo.bg_type = GUI_BG_IMG;

    SCI_TRACE_LOW("[Editpad] HandleEditpadWinMsg msg_id = 0x%x", msg_id);

#ifdef TOUCH_PANEL_SUPPORT
    switch (msg_id)
    {
        case MSG_CTL_PENOK:
        {
            if( param != PNULL)
            {
                MMI_CTRL_ID_T   ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
                switch (ctrl_id)
                {
                    case WATCH_EDITPAD_NUM1_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM2_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM3_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM4_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM5_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM6_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM7_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM8_BUTTON_CTRL_ID:
                    case WATCH_EDITPAD_NUM9_BUTTON_CTRL_ID:
                    {
                        msg_id = MSG_APP_1 + ctrl_id - WATCH_EDITPAD_NUM1_BUTTON_CTRL_ID;
                        //handle key
                        MMK_SendMsg(editCtrlId,msg_id,PNULL);

                        msg_id = MSG_KEYUP_1 + ctrl_id - WATCH_EDITPAD_NUM1_BUTTON_CTRL_ID;
                        break;
                    }
                    case WATCH_EDITPAD_NUM0_BUTTON_CTRL_ID:
                    {
                        msg_id = MSG_KEYUP_0;
                        //handle key
                        MMK_SendMsg(editCtrlId,MSG_APP_0,PNULL);
                        break;
                    }
                    case WATCH_EDITPAD_OK_BUTTON_CTRL_ID:
                    {
                        //handle key
                        MMK_SendMsg(editCtrlId,MSG_APP_OK,PNULL);
                        msg_id = MSG_KEYUP_OK;
                        break;
                    }
                    case WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID:
                    {
                        //handle key
                        MMK_SendMsg(editCtrlId,MSG_APP_CANCEL,PNULL);
                        msg_id = MSG_KEYUP_CANCEL;
                        break;
                    }
                    default:
                        break;
                }
            }
            break;

            default:
            break;;
        }
    }
#endif

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id,editCtrlId);
            break;
        }

        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayBackground(win_id);
            //get edit string
            GUIEDIT_GetString(editCtrlId,&editStr);
            if(editStr.wstr_len == 0)
            {
                btnInfo.img_id  = IAMGE_EDITPAD_CLEAN_DISABLE;
                GUIBUTTON_SetBg( WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID, &btnInfo );
                CTRLBUTTON_Update( WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID);

                btnInfo.img_id  = IAMGE_EDITPAD_OK_DISABLE;
                GUIBUTTON_SetBg( WATCH_EDITPAD_OK_BUTTON_CTRL_ID, &btnInfo );
                CTRLBUTTON_Update( WATCH_EDITPAD_OK_BUTTON_CTRL_ID);
            }
            break;
        }

        // BUG-1353091 设置窗体接收但不处理键值
        case MSG_APP_UP:
        case MSG_KEYUP_UP:
        case MSG_APP_DOWN:
        case MSG_KEYUP_DOWN:
            break;

        case MSG_KEYUP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            //get edit string
            GUIEDIT_GetString(editCtrlId,&editStr);

            if(editStr.wstr_len == 0)
            {
                btnInfo.img_id  = IAMGE_EDITPAD_CLEAN_DISABLE;
                GUIBUTTON_SetBg( WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID, &btnInfo );
                CTRLBUTTON_Update( WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID);
            }

            if(editStr.wstr_len != 0)
            {
                is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&editStr, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
                if ((is_input_emergency_call) &&
                    (editStr.wstr_len == emergency_call_len))
                {
#if defined PLATFORM_UWS6121E
                    btnInfo.img_id  = IAMGE_EDITPAD_OK_DISABLE;
#elif defined PLATFORM_ANTISW3
                    btnInfo.img_id  = IAMGE_EDITPAD_SOS;
#endif
                    GUIBUTTON_SetBg( WATCH_EDITPAD_OK_BUTTON_CTRL_ID, &btnInfo );
                    CTRLBUTTON_Update( WATCH_EDITPAD_OK_BUTTON_CTRL_ID);
                }
                else if(editStr.wstr_len < EDITPAD_EDITBOX_INPUT_MIN_LEN)
                {
                    btnInfo.img_id  = IAMGE_EDITPAD_OK_DISABLE;
                    GUIBUTTON_SetBg( WATCH_EDITPAD_OK_BUTTON_CTRL_ID, &btnInfo );
                    CTRLBUTTON_Update( WATCH_EDITPAD_OK_BUTTON_CTRL_ID);
                }
            }
            break;
        }

        case MSG_KEYUP_OK:
        {
            SCI_TRACE_LOW("[Editpad] HandleEditpadWinMsg editCtrlId = 0x%x", editCtrlId);
            //get edit string
            GUIEDIT_GetString(editCtrlId,&editStr);
            if(editStr.wstr_len != 0)
            {
               is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&editStr, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
                if ((is_input_emergency_call) &&
                    (editStr.wstr_len == emergency_call_len))
                {
                    uint8   editNumStr[EDITPAD_EDITBOX_INPUT_MAX_LEN] = {0};
                    //convert string
                    MMI_WSTRNTOSTR((uint8 *)editNumStr,EDITPAD_EDITBOX_INPUT_MAX_LEN,
                        editStr.wstr_ptr,editStr.wstr_len,
                        editStr.wstr_len);

                    WatchAPICC_MakeCall(
                                            MMIAPIPHONE_GetCurResponsePinSimID(),
                                            editNumStr,
                                            (uint8)editStr.wstr_len,
                                            PNULL,
                                            PNULL,
                                            MMIAPICC_GetCallSimType(MMIAPIPHONE_GetCurResponsePinSimID()),
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );
                    break;
                }
            }

            if(editStr.wstr_len >= EDITPAD_EDITBOX_INPUT_MIN_LEN)
            {
                if( editInfo != PNULL)
                {
                     editInfo->pEditFunc( win_id, msg_id, param);
                }
            }
            break;
        }

        case MSG_KEYUP_0:
        case MSG_KEYUP_1:
        case MSG_KEYUP_2:
        case MSG_KEYUP_3:
        case MSG_KEYUP_4:
        case MSG_KEYUP_5:
        case MSG_KEYUP_6:
        case MSG_KEYUP_7:
        case MSG_KEYUP_8:
        case MSG_KEYUP_9:
        {
            //get edit string
            GUIEDIT_GetString(editCtrlId,&editStr);

            if(editStr.wstr_len == 1)
            {
                btnInfo.img_id  = IAMGE_EDITPAD_CLEAN;
                GUIBUTTON_SetBg( WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID, &btnInfo );
                CTRLBUTTON_Update( WATCH_EDITPAD_CLEAN_BUTTON_CTRL_ID);
            }

            if(editStr.wstr_len == EDITPAD_EDITBOX_INPUT_MIN_LEN )
            {
                btnInfo.img_id  = IAMGE_EDITPAD_OK;
                GUIBUTTON_SetBg( WATCH_EDITPAD_OK_BUTTON_CTRL_ID, &btnInfo );
                CTRLBUTTON_Update( WATCH_EDITPAD_OK_BUTTON_CTRL_ID);
            }

            if(editStr.wstr_len != 0)
            {
                is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&editStr, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
                if ((is_input_emergency_call) &&
                    (editStr.wstr_len == emergency_call_len))
                {
#if defined PLATFORM_UWS6121E
                    btnInfo.img_id  = IAMGE_EDITPAD_OK_DISABLE;
#elif defined PLATFORM_ANTISW3
                    btnInfo.img_id  = IAMGE_EDITPAD_SOS;
#endif
                    GUIBUTTON_SetBg( WATCH_EDITPAD_OK_BUTTON_CTRL_ID, &btnInfo );
                    CTRLBUTTON_Update( WATCH_EDITPAD_OK_BUTTON_CTRL_ID);
                }
            }
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            CommonWin_EditpadWin_DestoryData( win_id );
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //Bug 2069218
        case MSG_TP_PRESS_FLING:
        {
            SCI_TRACE_LOW("[Editpad] HandleEditpadWinMsg to prevent the on screen");
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
//  Description: 创建拨号盘中的文本框
//  Parameter: [In] win_id : win id
//             [In] tip_str_ptr : label display string
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadLabel(
                                    MMI_WIN_ID_T    win_id,
                                    MMI_STRING_T    *tip_str_ptr
                                   )
{
    GUI_BG_T                lableBg    = {0};
    MMI_CTRL_ID_T           ctrl_id    = WATCH_EDITPAD_TIP_LABEL_CTRL_ID;
    GUI_FONT_T              tempFont   = GUI_MAX_FONT_NUM;
    GUI_RECT_T              text_rect  = EDITPAD_LABEL_RECT;
    MMI_CONTROL_CREATE_T    create     = {0};
    GUILABEL_INIT_DATA_T    initData   = {0};

    SCI_TRACE_LOW("[Editpad] CreateEditpadLabel ENTER");

    create.ctrl_id              = ctrl_id;
    create.guid                 = SPRD_GUI_LABEL_ID;
    create.parent_win_handle    = win_id;
    create.init_data_ptr        = &initData;

    MMK_CreateControl( &create );

    // set style
    lableBg.bg_type = GUI_BG_COLOR;
    lableBg.color   = MMI_BLACK_COLOR;

#if defined SCREEN_SHAPE_CIRCULAR
    tempFont = DP_FONT_16;
    GUILABEL_SetAlign( ctrl_id, GUILABEL_ALIGN_MIDDLE);
#elif defined SCREEN_SHAPE_SQUARE
    tempFont = EDITPAD_LABEL_FONT;
    GUILABEL_SetAlign( ctrl_id, GUILABEL_ALIGN_LEFT);
#endif
    if( PNULL != tip_str_ptr )
    {
        GUILABEL_SetText( ctrl_id, tip_str_ptr, TRUE );
    }
    else
    {
        SCI_TRACE_LOW("[Editpad] CreateEditpadLabel tip_str_ptr is PNULL");
    }

    GUILABEL_SetFont( ctrl_id, tempFont, EDITPAD_LABEL_FONT_COLOR );
    GUILABEL_SetRect( ctrl_id, &text_rect, TRUE );
    GUILABEL_SetBg( ctrl_id, &lableBg );
    GUILABEL_SetMargin( ctrl_id, 0 );
}

/*****************************************************************************/
//  Description : 创建拨号盘中的编辑框
//  Parameter: [In] win_id : win_id
//             [In] edit_ctrl :editbox ctrl id
//             [In] edit_rect_ptr :editbox display rect
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadEditBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T edit_ctrl,const GUI_BOTH_RECT_T *edit_rect_ptr)
{
    MMI_CONTROL_CREATE_T    editCtrl    = {0};
    GUI_BG_T                bgInfo      = {0};
    MMI_CTRL_ID_T           editCtrlId  = edit_ctrl;
    GUIEDIT_INIT_DATA_T     editInit    = {0};
    GUI_FONT_T              editFont    = GUI_MAX_FONT_NUM;

    SCI_TRACE_LOW("[Editpad] CreateEditpadEditBox ENTER");

    SCI_MEMCPY(&editInit.both_rect, edit_rect_ptr, sizeof(GUI_BOTH_RECT_T));
    editInit.type              = GUIEDIT_TYPE_PASSWORD;
    editInit.str_max_len       = EDITPAD_EDITBOX_INPUT_MAX_LEN;
    editCtrl.ctrl_id           = editCtrlId;
    editCtrl.guid              = SPRD_GUI_EDITBOX_ID;
    editCtrl.init_data_ptr     = &editInit;
    editCtrl.parent_win_handle = win_id;
    MMK_CreateControl(&editCtrl);

    //set im not handle tp
    GUIEDIT_SetImTp(editCtrlId, FALSE);

    //set cursor hide
    GUIEDIT_SetCursorHideState(editCtrlId, TRUE);

    //set pwdedit style
    CTRLPSWEDIT_SetPasswordStyle(editCtrlId,GUIEDIT_PASSWORD_STYLE_DIGITAL);
#if defined SCREEN_SHAPE_CIRCULAR
    editFont = DP_FONT_20;
#elif defined SCREEN_SHAPE_SQUARE
    editFont = EDITPAD_EDITBOX_FONT;
#endif
    GUIEDIT_SetFont(editCtrlId, editFont);
    GUIEDIT_SetFontColor(editCtrlId, MMI_WHITE_COLOR);

    bgInfo.bg_type = GUI_BG_COLOR;
    bgInfo.color = MMI_BLACK_COLOR;
    GUIEDIT_SetBg(editCtrlId,&bgInfo);

    GUIEDIT_SetSoftkey(edit_ctrl,0,0,TXT_NULL,STXT_OK,EditpadLeftWinSoftkey); // Bug 1350604
    GUIEDIT_SetSoftkey(edit_ctrl,2,0,TXT_NULL,TXT_DELETE,EditpadRightWinSoftkey); //Bug 1350604

}

/*****************************************************************************/
//  Description : 创建拨号盘中的button控件
//  Parameter: [In] win_id : win_id
//             [In] button_ctrl :button ctrl id
//             [In] button_image : button image
//             [In] rect_ptr : button display rect
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadButton(   MMI_WIN_ID_T win_id,
                                        MMI_CTRL_ID_T button_ctrl,
                                        MMI_IMAGE_ID_T button_image,
                                        const GUI_BOTH_RECT_T *rect_ptr
                                    )
{
    GUI_BG_T                    bgInfo = {0};
    GUIBUTTON_INIT_DATA_T       buttonInit = {0};
    MMI_CONTROL_CREATE_T        buttonCtrl = {0};

    SCI_TRACE_LOW("[Editpad] CreateEditpadButton ENTER");

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
//  Parameter: [In] win_id : win_id
//             [In] edit_ctrl :editbox ctrl id
//             [In] tip_str_ptr :label ctrl id
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CreateEditpadCtrl(MMI_WIN_ID_T win_id , MMI_CTRL_ID_T edit_ctrl, MMI_STRING_T *tip_str_ptr)
{
    int32           row         = 0;
    int32           column      = 0;
    uint16          imgHeight   = 0;
    uint16          imgWidth    = 0;
    uint8           space       = EDITPAD_BUTTON_TO_BUTTON_SPACE;
    GUI_BOTH_RECT_T editBoxRect = {0};
    GUI_BOTH_RECT_T buttonRect  = {0};

    SCI_TRACE_LOW("[Editpad] CreateEditpadCtrl ENTER");

    // tip
    CreateEditpadLabel( win_id, tip_str_ptr);

    // edit
    editBoxRect.v_rect.left = EDITPAD_EDITBOX_RECT_LEFT;
    editBoxRect.v_rect.right = EDITPAD_EDITBOX_RECT_RIGHT;
    editBoxRect.v_rect.top = EDITPAD_EDITBOX_RECT_TOP;
    editBoxRect.v_rect.bottom = EDITPAD_EDITBOX_RECT_BOTTOM;
    CreateEditpadEditBox(win_id, edit_ctrl,&editBoxRect);

    // button
    GUIRES_GetImgWidthHeight(&imgWidth, &imgHeight, IAMGE_EDITPAD_OK, win_id);
    for (row = 0; row < EDITPAD_NUM_BUTTON_ROW_MAX; row++)
    {
        for (column = 0; column < EDITPAD_NUM_BUTTON_COLUMN_MAX; column++)
        {
#if defined SCREEN_SHAPE_CIRCULAR
            buttonRect.v_rect.left = EDITPAD_BUTTON_RECT_LEFT + (imgWidth + space)* column;
#elif defined SCREEN_SHAPE_SQUARE
            buttonRect.v_rect.left = space + (imgWidth + space)* column;
#endif
            buttonRect.v_rect.top = EDITPAD_BUTTON_TOP_MIN + (imgHeight + space)* row ;
            buttonRect.v_rect.right = buttonRect.v_rect.left + imgWidth;
            buttonRect.v_rect.bottom = buttonRect.v_rect.top + imgHeight;

            CreateEditpadButton(win_id, s_edit_num_button_ctrl_order[row][column], s_edit_num_button_image_order[row][column], &buttonRect);
        }
    }
}

/*****************************************************************************/
//  Description : 销毁拨号盘中的窗口数据
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CommonWin_EditpadWin_DestoryData( MMI_WIN_ID_T win_id )
{
    WATCHCOM_EDITPAD_INFO_T *editCallback = (WATCHCOM_EDITPAD_INFO_T *)MMK_GetWinAddDataPtr( win_id );
    if( editCallback != PNULL)
    {
        SCI_FREE(editCallback);
        editCallback = PNULL;
    }
    return;
}

/*****************************************************************************/
//  Description : 创建拨号盘中的窗口
//  Parameter: [In] win_id : win_id
//             [In] userData :app user data
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void CommonWin_EditpadWin_CreateWin( MMI_WIN_ID_T win_id , WATCHCOM_EDITPAD_INFO_T *userData)
{
    MMI_WINDOW_CREATE_T winCreate = {0};

    SCI_TRACE_LOW("[Editpad] CommonWin_EditpadWin_CreateWin ENTER");

    GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,LCD_ANGLE_0);
    winCreate.applet_handle = MMK_GetFirstAppletHandle();
    winCreate.win_id        = win_id;
    winCreate.add_data_ptr  = userData;
    winCreate.func          = HandleEditpadWinMsg;

    winCreate.win_priority  = WIN_ONE_LEVEL;
    winCreate.window_style  = WS_HAS_TRANSPARENT;

    //禁止硬图标响应
    winCreate.window_style |= WS_DISABLE_HWICON;

    winCreate.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;

    MMK_CreateWindow( &winCreate );

}

/*****************************************************************************/
//  Description : Open Editpad window
//  Parameter: [In] win_id : commonwin ID
//             [In] edit_ctrl : editbox display string
//             [In] tip_str : label display string
//             [In] userFunc : custom callback API
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL void PHONE_EditpadWin_Enter(
                                                    MMI_WIN_ID_T    win_id,
                                                    MMI_CTRL_ID_T   edit_ctrl,
                                                    MMI_STRING_T    *tip_str,
                                                    PROCESSMSG_FUNC editpadFunc
                                                )
{
    WATCHCOM_EDITPAD_INFO_T *pEditpadInfo = PNULL;
    if(MMK_IsOpenWin(win_id))
    {
        SCI_TRACE_LOW("[Editpad] PHONE_EditpadWin_Enter is open");
        return;
    }

    SCI_TRACE_LOW("[Editpad]PHONE_EditpadWin_Enter win_id is 0x%x", win_id);

    //alloc space
    pEditpadInfo = SCI_ALLOC_APP(sizeof(WATCHCOM_EDITPAD_INFO_T));
    if(PNULL == pEditpadInfo)
    {
        SCI_TRACE_LOW("[Editpad]PHONE_EditpadWin_Enter The pEditpadInfo is PNULL");
        return;
    }
    SCI_MEMSET(pEditpadInfo,0,sizeof(WATCHCOM_EDITPAD_INFO_T));

    pEditpadInfo->editCtrl = edit_ctrl;

    if( editpadFunc != PNULL)
    {
        pEditpadInfo->pEditFunc = editpadFunc;
    }

    CommonWin_EditpadWin_CreateWin( win_id, pEditpadInfo);

    //Create ctrl
    CreateEditpadCtrl( win_id, edit_ctrl, tip_str);

    //create softkey
    GUIWIN_CreateSoftkeyDyn( win_id, TXT_NULL, TXT_NULL, TXT_NULL );
}

// editpad end ========================================================================================================

LOCAL void PHONE_CheckPukAndInputNewPin( MMI_STRING_T pukStr, MMI_STRING_T pinStr)
{
    MN_BLOCK_T      puk_value = {0};
    MN_BLOCK_T      pin_value = {0};
    MMI_STRING_T    wait_text = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    ERR_MNPHONE_CODE_E result_error =  ERR_MNDATAMAG_NO_ERR;

    //转换PUK的结构
    puk_value.blocklen = pukStr.wstr_len;

    MMI_WSTRNTOSTR((uint8 *)puk_value.blockbuf,
            MN_MAX_BLOCK_LEN,
            pukStr.wstr_ptr,
            puk_value.blocklen,
            puk_value.blocklen);

    //转换PIN的结构
    pin_value.blocklen = pinStr.wstr_len;

    MMI_WSTRNTOSTR((uint8 *)pin_value.blockbuf,
            MN_MAX_BLOCK_LEN,
            pinStr.wstr_ptr,
            pin_value.blocklen,
            pin_value.blocklen);

    //response pin before receive signal of sim is ready
    result_error = MNPHONE_ResponsePinEx(MMIAPIPHONE_GetCurResponsePinSimID(), MNSIM_PIN1, TRUE, &pin_value, &puk_value);// TRUE : pin is lock

    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);

    //进入等待界面
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    WatchCOM_WaitingWin_1Line_Enter(PHONE_PIN_WAIT_WIN_ID, &wait_text, 0, softKey, PNULL);
    SCI_TRACE_LOW("PHONE_CheckPukAndInputNewPin: result_error = %d",result_error);
}

LOCAL MMI_RESULT_E HandleComfirmPinInputNumber( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("HandleComfirmPinInputNumber: Enter");
    switch (msg_id)
    {
        case MSG_KEYUP_OK:
        {
            MMI_STRING_T newPinStr = {0};
            MMI_STRING_T againPinStr = {0};

            // get the value about new PIN again
            GUIEDIT_GetString(WATCH_EDITPAD_EDITBOX_NEW_PIN_CTRL_ID,&newPinStr);

            // get the value about new PIN
            GUIEDIT_GetString(WATCH_EDITPAD_EDITBOX_AGAIN_PIN_CTRL_ID,&againPinStr);

            if( MMIAPICOM_StrCompare( &newPinStr, &againPinStr ) )
            {
                MMI_STRING_T pukStr = {0};
                SCI_TRACE_LOW("HandleComfirmPinInputNumber: MMIAPICOM_StrCompare true");
                // get the value about PUK
                GUIEDIT_GetString(WATCH_EDITPAD_EDITBOX_PUK_CTRL_ID,&pukStr);
                PHONE_CheckPukAndInputNewPin(pukStr,newPinStr);

                //close input new pin and input new pin again window
                MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
                MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
            }
            else
            {
                //提示错误窗口,两次输入的PIN不一致
                MMI_STRING_T txt_tip = {0};
                MMI_STRING_T txt_button = {0};
                MMI_IMAGE_ID_T btnImage = 0;
                WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_COMMON_OK,TXT_NULL};
                SCI_TRACE_LOW("HandleComfirmPinInputNumber: MMIAPICOM_StrCompare false");
                MMI_GetLabelTextByLang(TXT_MSGBOX_PIN_CONFLICT, &txt_tip);
#if defined SCREEN_SHAPE_CIRCULAR
                btnImage = res_setting_btn_confirm;
#elif defined SCREEN_SHAPE_SQUARE
                MMI_GetLabelTextByLang(TXT_COMMON_OK, &txt_button);
#endif

                WatchCOM_QueryWin_1Btn_Enter(WATCH_EDITPAD_QUERY_WIN_ID, &txt_tip, &txt_button,btnImage, softkey, PNULL );

                //清空password
                GUIEDIT_ClearAllStr(WATCH_EDITPAD_EDITBOX_NEW_PIN_CTRL_ID);
                MMK_SendMsg(PHONE_PUK_INPUT_WIN_ID,MSG_FULL_PAINT,PNULL);

                //close input new pin again window
                MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
            }

        }
        break;

        default:
            break;
    }

    return (result);
}

LOCAL MMI_RESULT_E HandleNewPinInputNumber( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("HandleNewPinInputNumber: Enter");
    switch (msg_id)
    {
        case MSG_KEYUP_OK:
        {
            MMI_STRING_T txt_tip = {0};

            // 打开输入AGAIN PIN的界面
            MMI_GetLabelTextByLang(TXT_ENTER_PIN_CNF, &txt_tip);
            PHONE_EditpadWin_Enter( PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID, WATCH_EDITPAD_EDITBOX_AGAIN_PIN_CTRL_ID, &txt_tip, HandleComfirmPinInputNumber);
        }
        break;

        default:
            break;
    }

    return (result);
}

LOCAL MMI_RESULT_E HandlePukInputNumber( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("HandlePukInputNumber: Enter");
    switch (msg_id)
    {
        case MSG_KEYUP_OK:
        {
            MMI_STRING_T txt_tip = {0};

            MMK_SetWinDisplayStyleState(PHONE_PUK_INPUT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
            // 打开输入NEW PIN的界面
            MMI_GetLabelTextByLang(TXT_ENTER_NEW_PIN, &txt_tip);
            PHONE_EditpadWin_Enter( PHONE_NEW_PIN_INPUT_WIN_ID, WATCH_EDITPAD_EDITBOX_NEW_PIN_CTRL_ID, &txt_tip, HandleNewPinInputNumber);
        }
        break;

        default:
            break;
    }

    return (result);
}

LOCAL MMI_RESULT_E HandlePinIsBlocked(
                                    MMI_WIN_ID_T        win_id,    // Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                    DPARAM              param       // 相应消息的参数
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("HandlePinIsBlocked open,msg_id = %x",msg_id);

    switch (msg_id)
    {
        case MSG_NOTIFY_CANCEL://pin被锁输入界面不允许返回
        case MSG_NOTIFY_OK:
        case MSG_NOTIFY_MIDSK:
        {
            MMK_CloseWin(win_id);
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
            MMI_STRING_T txt_tip = {0};
            // close input pin window
            MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);

            MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
            // 打开输入PUK的界面
            MMI_GetLabelTextByLang(TXT_ENTER_PUK, &txt_tip);
            PHONE_EditpadWin_Enter( PHONE_PUK_INPUT_WIN_ID, WATCH_EDITPAD_EDITBOX_PUK_CTRL_ID, &txt_tip, HandlePukInputNumber);
        }
        break;
        default:
            break;
    }

    return (result);
}

LOCAL MMI_RESULT_E HandlePinInputNumber( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("HandlePinInputNumber: Enter");
    switch (msg_id)
    {
        case MSG_KEYUP_OK:
        {
            MMI_STRING_T    editNumStr = {0};
            MN_BLOCK_T      pin_value = {0};
            MMI_STRING_T    wait_text = {0};
            WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
            ERR_MNPHONE_CODE_E result_error =  ERR_MNDATAMAG_NO_ERR;

            GUIEDIT_GetString(WATCH_EDITPAD_EDITBOX_PIN_CTRL_ID,&editNumStr);

            //转换PIN的结构
            pin_value.blocklen = editNumStr.wstr_len;

            MMI_WSTRNTOSTR((uint8 *)pin_value.blockbuf,
                MN_MAX_BLOCK_LEN,
                editNumStr.wstr_ptr,
                pin_value.blocklen,
                pin_value.blocklen);

            //response pin before receive signal of sim is ready
            result_error = MNPHONE_ResponsePinEx(MMIAPIPHONE_GetCurResponsePinSimID(), MNSIM_PIN1, FALSE, &pin_value, PNULL);// FALSE : pin is not lock

            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);

            //进入等待界面
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);

            WatchCOM_WaitingWin_1Line_Enter(PHONE_PIN_WAIT_WIN_ID, &wait_text, 0, softKey, PNULL);

            MMK_SetWinDisplayStyleState(PHONE_PIN_WAIT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
            SCI_TRACE_LOW("HandlePinInputNumber: result_error = %d",result_error);
        }
        break;

        default:
            break;
    }

    return (result);
}
LOCAL MMI_RESULT_E PHONE_OpenInputPinWindow(APP_MN_SIM_GET_PIN_IND_T sim_get_pin_ind)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T alert_win = WATCH_EDITPAD_QUERY_WIN_ID;

    SCI_TRACE_LOW("PHONE_OpenInputPinWindow open");

    if (MNSIM_PIN1 == sim_get_pin_ind.pin_num)
    {
        if(MMK_IsOpenWin(alert_win))
        {
            MMK_CloseWin(alert_win);
        }

        if (sim_get_pin_ind.pin_is_blocked)//PIN被锁，要求输入PUK
        {
            SCI_TRACE_LOW("PHONE_OpenInputPinWindow PIN is blocked");
            if (MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
            {
                MMI_STRING_T alert_str = {0};
                MMI_STRING_T pin_puk_remain_str = {0};
                MMI_STRING_T txt_button = {0};
                MMI_IMAGE_ID_T btnImage = 0;
                WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_COMMON_OK,TXT_NULL};

                MMI_GetLabelTextByLang(TXT_PUK_ERROR, &alert_str);

#if defined SCREEN_SHAPE_CIRCULAR
                btnImage = res_setting_btn_confirm;
#elif defined SCREEN_SHAPE_SQUARE
                MMI_GetLabelTextByLang(TXT_COMMON_OK, &txt_button);
#endif

                SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
                MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurResponsePinSimID(), &pin_puk_remain_str, MNSIM_PIN1, TRUE);// TRUE : pin is lock

                //提示错误窗口,PUK错误!
                WatchCOM_QueryWin_1Btn_With2Str_Enter(alert_win,&alert_str,&pin_puk_remain_str, &txt_button,btnImage, softkey, PNULL );

                //清空password
                GUIEDIT_ClearAllStr(WATCH_EDITPAD_EDITBOX_PUK_CTRL_ID);
                MMK_SendMsg(PHONE_PUK_INPUT_WIN_ID,MSG_FULL_PAINT,PNULL);
            }
            else
            {
                //提示错误窗口,PIN被锁!
                MMI_STRING_T txt_tip = {0};
                MMI_STRING_T txt_button = {0};
                MMI_IMAGE_ID_T btnImage = 0;
                WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_COMMON_OK,TXT_NULL};
                MMI_GetLabelTextByLang(TXT_PIN_BLOCKED, &txt_tip);
#if defined SCREEN_SHAPE_CIRCULAR
                btnImage = res_setting_btn_confirm;
#elif defined SCREEN_SHAPE_SQUARE
                MMI_GetLabelTextByLang(TXT_COMMON_OK, &txt_button);
#endif
                WatchCOM_QueryWin_1Btn_Enter(alert_win, &txt_tip, &txt_button,btnImage, softkey, HandlePinIsBlocked );
            }
        }
        else//PIN没有被锁，要求输入PIN
        {
            SCI_TRACE_LOW("PHONE_OpenInputPinWindow PIN is not blocked");
            if (MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID))
            {
                MMI_STRING_T alert_str = {0};
                MMI_STRING_T pin_puk_remain_str = {0};
                MMI_STRING_T txt_button = {0};
                MMI_IMAGE_ID_T btnImage = 0;
                WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_COMMON_OK,TXT_NULL};

#if defined SCREEN_SHAPE_CIRCULAR
                btnImage = res_setting_btn_confirm;
#elif defined SCREEN_SHAPE_SQUARE
                MMI_GetLabelTextByLang(TXT_COMMON_OK, &txt_button);
#endif
                MMI_GetLabelTextByLang(TXT_PIN_ERROR, &alert_str);


                SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
                MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurResponsePinSimID(), &pin_puk_remain_str, MNSIM_PIN1, FALSE);// FALSE : pin is not lock

                //提示错误窗口,PIN 错误!
                WatchCOM_QueryWin_1Btn_With2Str_Enter(alert_win,&alert_str,&pin_puk_remain_str, &txt_button,btnImage, softkey, PNULL );

                //清空password
                GUIEDIT_ClearAllStr(WATCH_EDITPAD_EDITBOX_PIN_CTRL_ID);
                MMK_SendMsg(PHONE_PIN_INPUT_WIN_ID,MSG_FULL_PAINT,PNULL);
            }
            else
            {
                // 打开输入PIN的界面
                MMI_STRING_T txt_tip = {0};

                MMI_GetLabelTextByLang(TXT_ENTER_PIN, &txt_tip);
                PHONE_EditpadWin_Enter( PHONE_PIN_INPUT_WIN_ID, WATCH_EDITPAD_EDITBOX_PIN_CTRL_ID,&txt_tip, HandlePinInputNumber);
            }
        }
    }

    // 关闭等待窗口
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);

    return (result);
}

LOCAL void PHONE_ClosePinPukWin(void)
{
    if(MMK_IsOpenWin( PHONE_PIN_PUK_WAIT_WIN_ID ))
    {
        MMK_CloseWin( PHONE_PIN_PUK_WAIT_WIN_ID );
    }

    // TODO: close other pin/puk win
}
LOCAL BOOLEAN PHONE_OpenWaitingPinPukWin( void )
{
    MMI_WIN_ID_T     wait_win_id = PHONE_PIN_PUK_WAIT_WIN_ID;
    WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};

    SCI_TRACE_LOW("PHONE_OpenWaitingPinPukWin: Enter");

    WatchCOM_WaitingWin_FullScreen_Enter(wait_win_id,PNULL,0,softket_test,PNULL);
    return TRUE;
}

#ifdef ADULT_WATCH_SUPPORT
LOCAL void PHONE_OpenWarningPinPukWin(void)
{
    MMI_WIN_ID_T    warning_win_id = PHONE_PIN_ALERT_WIN_ID;
    MMI_STRING_T text_str = {0};

    SCI_TRACE_LOW("PHONE_OpenWarningPinPukWin: Enter"); 
    MMI_GetLabelTextByLang(TXT_PIN_PUK_LOCK,&text_str);
    AdultWatchCOM_WaitingWin_1Icon_1Str_Enter(warning_win_id, &text_str, image_watch_warning_info, PNULL);

}
#endif

/*****************************************************************************/
//  Description : enter pin or puk win
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_OpenPinPukWin( void )
{
    if( s_pinpuk_state == MMIPHONE_PIN_PUK_STATE_WAIT)
    {
#ifdef ADULT_WATCH_SUPPORT
        if(FALSE == MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID))
#endif
        {
            PHONE_OpenWaitingPinPukWin();
        }
    }
    else if( s_pinpuk_state == MMIPHONE_PIN_PUK_STATE_INPUT)
    {
        PHONE_OpenInputPinWindow(MMIPHONE_GetCurHandlingPinSimInfo());
    }
    else
    {
        SCI_TRACE_LOW("MMIPHONE_OpenPinPukWin can not open win, s_pinpuk_state = %d",s_pinpuk_state);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : MMIPHONE_SetPinPukState
//  Parameter: [In] pinpuk_state
//             [Out] None
//             [Return] None
//  Author: jichuan.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetPinPukState(MMIPHONE_PIN_PUK_STATE_TYPE_E pinpuk_state)
{
    SCI_TRACE_LOW("MMIPHONE_SetPinPukState: s_pinpuk_state=%d,pinpuk_state=%d",s_pinpuk_state,pinpuk_state);
    s_pinpuk_state = pinpuk_state;
}

/*****************************************************************************/
//  Description : get pin or puk ready status
//  Parameter: [In] None
//             [Out] None
//             [Return] is_needPinPuk : TRUE(input pin or puk win);FALSE(not input pin or puk win)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_IsNeedPinPuk( void )
{
    BOOLEAN is_needPinPuk = TRUE;
    BOOLEAN is_register_ps_service = FALSE;
    is_register_ps_service = MMIAPIENG_GetRegisterPsServiceEnable();
    if(FALSE == is_register_ps_service)
    {
        SCI_TRACE_LOW("MMIPHONE_IsNeedPinPuk: not register ps service, return false");
        return FALSE;
    }
    if(s_pinpuk_state == MMIPHONE_PIN_PUK_STATE_FINISH)
    {
        is_needPinPuk = FALSE;
    }

    SCI_TRACE_LOW("MMIPHONE_IsNeedPinPuk: is_needPinPuk = %d",is_needPinPuk);
    #ifndef WIN32
    return is_needPinPuk;
    #else
    return TRUE;
    #endif
}

/*****************************************************************************/
//  Description : 启机流程的控制函数
//  Parameter: [In]event: 当前完成的事件
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_PinOrPukProccess( MMIPHONE_PIN_PUK_EVENT_TYPE_E event_id)
{
    SCI_TRACE_LOW("MMIPHONE_PinOrPukProccess: s_pinpuk_state = %d,event_id = %d",s_pinpuk_state,event_id);

#ifdef ADULT_WATCH_SUPPORT
    if(MMK_IsOpenWin(PHONE_PIN_PUK_WAIT_WIN_ID))
    {
        MMK_CloseWin(PHONE_PIN_PUK_WAIT_WIN_ID);
    }

    if(event_id == MMIPHONE_PIN_PUK_EVENT_INPUT)
    {
        if(FALSE == MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID))
        {
            PHONE_OpenWarningPinPukWin();
        }
    }
    else
    {
        if(MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID))
        {
            MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);
        }
        s_pinpuk_state = MMIPHONE_PIN_PUK_STATE_FINISH;
        MMIAPI_StartupCtrl( MMI_STARTUP_EVENT_PINPUK_FINISH );
    }
#else

    switch( s_pinpuk_state )
    {
        case MMIPHONE_PIN_PUK_STATE_WAIT:
        {
            switch( event_id )
            {
                case MMIPHONE_PIN_PUK_EVENT_INPUT:
                {
                    s_pinpuk_state = MMIPHONE_PIN_PUK_STATE_INPUT;
                    if(MMK_IsOpenWin( PHONE_PIN_PUK_WAIT_WIN_ID ))
                    {
                        MMK_CloseWin( PHONE_PIN_PUK_WAIT_WIN_ID );
                        PHONE_OpenInputPinWindow(MMIPHONE_GetCurHandlingPinSimInfo());
                    }
#ifdef WIN32
                    if(!MMK_IsOpenWin( PHONE_PIN_PUK_WAIT_WIN_ID ))
                    {
                        MMK_CloseWin( PHONE_PIN_PUK_WAIT_WIN_ID );
                        PHONE_OpenInputPinWindow(MMIPHONE_GetCurHandlingPinSimInfo());
                    }
#endif
                }
                break;
                case MMIPHONE_PIN_PUK_EVENT_READY:
                case MMIPHONE_PIN_PUK_EVENT_NOT_READY:
                {
                    s_pinpuk_state = MMIPHONE_PIN_PUK_STATE_FINISH;
                    if(MMK_IsOpenWin( PHONE_PIN_PUK_WAIT_WIN_ID ))
                    {
                        PHONE_ClosePinPukWin();
                        MMIAPI_StartupCtrl( MMI_STARTUP_EVENT_PINPUK_FINISH );
                    }
                }
                break;
                default:
                    break;
            }
        }
        break;
        case MMIPHONE_PIN_PUK_STATE_INPUT:
        {
            switch( event_id )
            {
                case MMIPHONE_PIN_PUK_EVENT_INPUT:
                {
                    s_pinpuk_state = MMIPHONE_PIN_PUK_STATE_INPUT;
                    PHONE_OpenInputPinWindow(MMIPHONE_GetCurHandlingPinSimInfo());
                }
                break;
                case MMIPHONE_PIN_PUK_EVENT_READY:
                case MMIPHONE_PIN_PUK_EVENT_NOT_READY:
                {
                    s_pinpuk_state = MMIPHONE_PIN_PUK_STATE_FINISH;
                    PHONE_ClosePinPukWin();
                    MMIAPI_StartupCtrl( MMI_STARTUP_EVENT_PINPUK_FINISH );
                }
                break;
                default:
                    break;
            }
        }
        break;
        default:
            break;
    }
#endif
}

