/*****************************************************************************
** File Name:      mmiwifi_password_win.c                                    *
** Author:         bin.wang1                                                 *
** Date:           2020.04.15                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    show password window                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2020.04.15     bin.wang1            Create                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"

#include "mmicom_trace.h"
#include "mmiwlan_image.h"
#include "mmiwlan_text.h"
#include "mmiwlan_id.h"

#include "mmiwifi_export.h"
#include "mmiwlan_profile.h"
#include "ctrlform_export.h"
#include "ctrlim_base.h"
#include "guiedit.h"
#include "guibutton.h"
#include "mmidisplay_data.h"
#include "mmi_common.h"
#include "watch_common.h"
#include "watch_common_btn.h"

#define PASSWORD_EDIT_RECT_LEFT DP2PX_VALUE(0)
#define PASSWORD_EDIT_RECT_RIGHT DP2PX_VALUE(240)
#define PASSWORD_EDIT_RECT_TOP DP2PX_VALUE(10)
#define PASSWORD_EDIT_RECT_BOTTOM DP2PX_VALUE(160)
#ifdef ADULT_WATCH_SUPPORT
#define PASSWORD_EDIT_MARGIN_LEFTRIGHT DP2PX_VALUE(37)
#define PASSWORD_EDIT_MARGIN_TOPBOTTOM DP2PX_VALUE(5)
#endif

LOCAL void Wifi_Password_APP_OK( MMI_WIN_ID_T win_id )
{
    uint8               pskChar[WIFISUPP_WPA_PSK_LEN+1] = { 0 };
    BOOLEAN             reCode = 0;
    MMIWIFI_PROFILE_T*  pProfile = PNULL;
    MMI_STRING_T        mmiPSKStr = { 0 };
    MMI_CTRL_ID_T       editCtrlId =  MMIWIFI_PASSWORD_EDIT_CTRL_ID;

    TRACE_APP_WLAN(" Enter");

    pProfile = (MMIWIFI_PROFILE_T*)MMK_GetWinAddDataPtr( win_id );

    if( PNULL== pProfile )
    {
        TRACE_APP_WLAN("pProfile is PNULL!!");
        return;
    }

    GUIEDIT_GetString( editCtrlId, &mmiPSKStr );
    MMI_WSTRNTOSTR( pskChar, WIFISUPP_WPA_PSK_LEN, mmiPSKStr.wstr_ptr, mmiPSKStr.wstr_len, mmiPSKStr.wstr_len );
    SCI_MEMSET( pProfile->config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk, 0, WIFISUPP_WPA_PSK_LEN );
    SCI_MEMCPY( pProfile->config_info.credential.wpa_credential.credential_info.wpa_psk_info.psk, pskChar, WIFISUPP_WPA_PSK_LEN );

    reCode = MMIWIFI_DirectConnect( &(pProfile->config_info) );
    pProfile->is_editing = FALSE;
    MMIWIFI_OpenConnectWaitWin();

    MMK_CloseWin( win_id );
    MMK_CloseWin( MMIWIFI_AP_CONNECT_WIN_ID );
}

LOCAL void Wifi_Password_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E _Callback_WifiPwdBtnCancel( void )
{
    Wifi_Password_APP_CANCEL( MMIWIFI_PASSWORD_WIN_ID );
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E _Callback_WifiPwdBtnOK( void )
{
    Wifi_Password_APP_OK( MMIWIFI_PASSWORD_WIN_ID );
    return MMI_RESULT_TRUE;
}

LOCAL void Wifi_Password_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T      editRect = { PASSWORD_EDIT_RECT_LEFT,
                                 PASSWORD_EDIT_RECT_TOP,
                                 PASSWORD_EDIT_RECT_RIGHT, 
                                 PASSWORD_EDIT_RECT_BOTTOM};
    MMI_CTRL_ID_T   editCtrlId =  MMIWIFI_PASSWORD_EDIT_CTRL_ID;

    BOOLEAN         reCode = FALSE;
    MMI_CTRL_ID_T   cancelBtn = MMIWIFI_PASSWORD_CANCEL_BTN_CTRL_ID;
    MMI_CTRL_ID_T   okBtn = MMIWIFI_PASSWORD_OK_BTN_CTRL_ID;
    GUI_FONT_T      tempFont = WATCH_COMMONWIN_NORMAL_FONT;

    TRACE_APP_WLAN(" Enter");

    GUIEDIT_SetRect( editCtrlId, &editRect );
#ifdef ADULT_WATCH_SUPPORT
    GUIEDIT_SetIm( editCtrlId, GUIIM_TYPE_SYS, GUIIM_TYPE_SYS );
    GUIEDIT_SetPasswordStyle(editCtrlId, GUIEDIT_PASSWORD_STYLE_ALL);
#else
    GUIEDIT_SetIm( editCtrlId, GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC );
#endif

    WatchCOM_Btn_SetPos_2Btn( okBtn, cancelBtn );
    GUIBUTTON_SetTextId( okBtn, TXT_COMMON_OK );
    GUIBUTTON_SetTextAlign( okBtn, ALIGN_HVMIDDLE );
    GUIBUTTON_SetFont( okBtn, &tempFont );
    GUIBUTTON_SetFontColor( okBtn, WATCH_COMMONWIN_FONT_COLOR );
    reCode = GUIBUTTON_SetCallBackFunc( okBtn, _Callback_WifiPwdBtnOK );
    if( reCode != TRUE )
    {
        TRACE_APP_WLAN("GUIBUTTON_SetCallBackFunc okBtn set error");
    }
    GUIBUTTON_SetTextId( cancelBtn, STXT_RETURN );
    GUIBUTTON_SetTextAlign( cancelBtn, ALIGN_HVMIDDLE );
    GUIBUTTON_SetFont( cancelBtn, &tempFont );
    GUIBUTTON_SetFontColor( cancelBtn, WATCH_COMMONWIN_FONT_COLOR );
    reCode = GUIBUTTON_SetCallBackFunc( cancelBtn, _Callback_WifiPwdBtnCancel );
    if( reCode != TRUE )
    {
        TRACE_APP_WLAN("GUIBUTTON_SetCallBackFunc cancelBtn set error");
    }
#ifdef ADULT_WATCH_SUPPORT
    GUIEDIT_PermitBorder(editCtrlId,FALSE);//Bug 1621298
    CTRLBASEEDIT_SetAlign( editCtrlId,ALIGN_HVMIDDLE );//Bug 1621298
    CTRLBASEEDIT_SetMargin( editCtrlId,PASSWORD_EDIT_MARGIN_LEFTRIGHT,PASSWORD_EDIT_MARGIN_TOPBOTTOM );
#endif
    MMK_SetAtvCtrl( win_id, editCtrlId );
}

LOCAL void Wifi_Password_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    //bg color
    WATCHCOM_DisplayBackground( win_id );
}

LOCAL MMI_RESULT_E  HandleWifiPasswordWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

    //TRACE_APP_WLAN(" Enter msg_id %08x", msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Wifi_Password_OPEN_WINDOW( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            Wifi_Password_FULL_PAINT( win_id );
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Wifi_Password_APP_OK( win_id );
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Wifi_Password_APP_CANCEL( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

WINDOW_TABLE( MMIWIFI_PASSWORD_WIN_TAB ) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWifiPasswordWindow ),
    WIN_ID( MMIWIFI_PASSWORD_WIN_ID ),
    CREATE_EDIT_PASSWORD_CTRL( WIFISUPP_WPA_PSK_LEN, MMIWIFI_PASSWORD_EDIT_CTRL_ID ),//Bug 1654476  
    CREATE_BUTTON_CTRL( IMAGE_NULL, MMIWIFI_PASSWORD_OK_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_NULL, MMIWIFI_PASSWORD_CANCEL_BTN_CTRL_ID ),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY( TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

/****************************************************************************/
//  Description : enter wifi main window
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void WIFI_PasswordWin_Enter( MMIWIFI_PROFILE_T* profile_ptr )
{
    TRACE_APP_WLAN(" Enter");

    MMK_CreateWin( (uint32*)MMIWIFI_PASSWORD_WIN_TAB, (ADD_DATA)profile_ptr );
}

