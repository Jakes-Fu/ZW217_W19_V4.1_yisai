/*****************************************************************************
** File Name:      adultwatch_bt_name_win.c                                  *
** Author:         xiaotong.su                                               *
** Date:           2021.04.16                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    show password window                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2021.04.16     xiaotong.su            Create                              *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmicom_trace.h"
#include "ctrlform_export.h"
#include "ctrlim_base.h"
#include "guiedit.h"
#include "guibutton.h"
#include "mmidisplay_data.h"
#include "mmi_common.h"
#include "watch_common.h"
#include "watch_common_btn.h"
#include "mmiset_id.h"
#include "adultwatch_bt_name_win.h"
#include "ual_bt_dev.h"
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define NAME_EDIT_RECT_LEFT   DP2PX_VALUE(0)
#define NAME_EDIT_RECT_RIGHT  DP2PX_VALUE(240)
#define NAME_EDIT_RECT_TOP    DP2PX_VALUE(10)
#define NAME_EDIT_RECT_BOTTOM DP2PX_VALUE(160)
#define NAME_EDIT_MARGIN_LEFTRIGHT DP2PX_VALUE(37)
#define NAME_EDIT_MARGIN_TOPBOTTOM DP2PX_VALUE(5)

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION DEFINITION                        *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                    *
**---------------------------------------------------------------------------*/
LOCAL void Bt_Name_APP_OK( MMI_WIN_ID_T win_id )
{
    uint8               nameChar[BT_LOCAL_NAME_LEN+1] = { 0 };
    MMI_STRING_T        mmiNameStr = { 0 };
    MMI_CTRL_ID_T       editCtrlId =  MMISET_BT_NAME_EDIT_CTRL_ID;

    GUIEDIT_GetString( editCtrlId, &mmiNameStr );
    if( 0 < mmiNameStr.wstr_len )
    {
        MMI_WSTRNTOSTR( nameChar, BT_LOCAL_NAME_LEN, mmiNameStr.wstr_ptr, mmiNameStr.wstr_len, mmiNameStr.wstr_len );
    }
    // to do: set nv
#ifdef BBM_ROLE_SUPPORT
    Bbm_SetBTLocalName(mmiNameStr.wstr_ptr);
#endif
    ual_bt_dev_set_local_name(mmiNameStr.wstr_ptr);
    MMK_CloseWin( win_id );
}

LOCAL void Bt_Name_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E _Callback_BtNameBtnCancel( void )
{
    Bt_Name_APP_CANCEL( MMISET_BT_NAME_WIN_ID );
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E _Callback_BtNameBtnOK( void )
{
    Bt_Name_APP_OK( MMISET_BT_NAME_WIN_ID );
    return MMI_RESULT_TRUE;
}

LOCAL void Bt_Name_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T      editRect = { NAME_EDIT_RECT_LEFT,
                                 NAME_EDIT_RECT_TOP,
                                 NAME_EDIT_RECT_RIGHT,
                                 NAME_EDIT_RECT_BOTTOM};

    MMI_CTRL_ID_T   editCtrlId = MMISET_BT_NAME_EDIT_CTRL_ID;

    BOOLEAN         reCode = FALSE;
    MMI_CTRL_ID_T   cancelBtn = MMISET_BT_NAME_CANCEL_BTN_CTRL_ID;
    MMI_CTRL_ID_T   okBtn = MMISET_BT_NAME_OK_BTN_CTRL_ID;
    GUI_FONT_ALL_T  tempFont = {0};
    GUI_BG_T        bg = {0};

    wchar           wstr_bt_name[BT_LOCAL_NAME_LEN + 1] = {0};
    MMI_STRING_T    bt_name = {0};
    uint16          name_len = 0;

    TRACE_APP_BT(" Enter");

    GUIEDIT_SetRect( editCtrlId, &editRect );
    GUIEDIT_SetIm( editCtrlId, GUIIM_TYPE_SYS, GUIIM_TYPE_SYS );

    WatchCOM_Btn_SetPos_2Btn( okBtn, cancelBtn );
    GUIBUTTON_SetTextId( okBtn, TXT_COMMON_OK );
    GUIBUTTON_SetTextAlign( okBtn, ALIGN_HVMIDDLE );
    tempFont.font = WATCH_COMMONWIN_NORMAL_FONT;
    GUIBUTTON_SetFont( okBtn, &tempFont );
    GUIBUTTON_SetFontColor( okBtn, WATCH_COMMONWIN_FONT_COLOR );
    reCode = GUIBUTTON_SetCallBackFunc( okBtn, _Callback_BtNameBtnOK );
    if( reCode != TRUE )
    {
        TRACE_APP_BT("GUIBUTTON_SetCallBackFunc okBtn set error");
    }
    GUIBUTTON_SetTextId( cancelBtn, STXT_RETURN );
    GUIBUTTON_SetTextAlign( cancelBtn, ALIGN_HVMIDDLE );
    tempFont.font = WATCH_COMMONWIN_NORMAL_FONT;
    GUIBUTTON_SetFont( cancelBtn, &tempFont );
    GUIBUTTON_SetFontColor( cancelBtn, WATCH_COMMONWIN_FONT_COLOR );
    reCode = GUIBUTTON_SetCallBackFunc( cancelBtn, _Callback_BtNameBtnCancel );
    if( reCode != TRUE )
    {
        TRACE_APP_BT("GUIBUTTON_SetCallBackFunc cancelBtn set error");
    }
    //set bg
    bg.bg_type       = GUI_BG_COLOR;
    bg.color         = MMI_WHITE_COLOR;
    bg.shape         = GUI_SHAPE_ROUNDED_RECT;
    GUIEDIT_SetBg(editCtrlId,&bg);
    //set algin
    GUIEDIT_SetStyle( MMISET_BT_NAME_EDIT_CTRL_ID, GUIEDIT_STYLE_SINGLE );
    CTRLBASEEDIT_SetAlign( MMISET_BT_NAME_EDIT_CTRL_ID,ALIGN_HVMIDDLE );
    CTRLBASEEDIT_SetMargin( MMISET_BT_NAME_EDIT_CTRL_ID,NAME_EDIT_MARGIN_LEFTRIGHT,NAME_EDIT_MARGIN_TOPBOTTOM );
#ifdef BBM_ROLE_SUPPORT
    Bbm_GetBTLocalName(wstr_bt_name,BT_LOCAL_NAME_LEN);
#endif
    name_len = MMIAPICOM_Wstrlen(wstr_bt_name);
    TRACE_APP_BT("name_len = %d",name_len);
    if(0 == name_len)
    {
        ual_bt_dev_get_local_name(wstr_bt_name,BT_LOCAL_NAME_LEN+1);
    }
    name_len = MMIAPICOM_Wstrlen(wstr_bt_name);
    TRACE_APP_BT("name_len = %d",name_len);
    GUIEDIT_SetString(editCtrlId,wstr_bt_name,name_len);
    MMK_SetAtvCtrl( win_id, editCtrlId );
}

LOCAL void Bt_Name_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    //bg color
    WATCHCOM_DisplayBackground( win_id );
}

LOCAL MMI_RESULT_E  HandleBtNameWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;
    TRACE_APP_BT(" Enter msg_id %08x", msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Bt_Name_OPEN_WINDOW( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            Bt_Name_FULL_PAINT( win_id );
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Bt_Name_APP_OK( win_id );
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Bt_Name_APP_CANCEL( win_id );
            break;
        }

        default:
        {
            reCode = MMI_RESULT_FALSE;
            break;
        }
    }

    return reCode;
}

/*window table*/
WINDOW_TABLE( MMISET_BT_NAME_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleBtNameWindow ),
    WIN_ID( MMISET_BT_NAME_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_EDIT_TEXT_CTRL( BT_LOCAL_NAME_LEN, MMISET_BT_NAME_EDIT_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_NULL, MMISET_BT_NAME_OK_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_NULL, MMISET_BT_NAME_CANCEL_BTN_CTRL_ID ),
    END_WIN
};

/****************************************************************************/
//  Description : enter BT1 device name window
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:
/****************************************************************************/
PUBLIC void Bt_NameWin_Enter(void)
{
    TRACE_APP_BT(" Enter");

    MMK_CreateWin( (uint32*)MMISET_BT_NAME_WIN_TAB,PNULL);
}

