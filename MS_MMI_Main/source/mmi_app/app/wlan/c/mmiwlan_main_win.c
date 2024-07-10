/*****************************************************************************
** File Name:      watch_wlan_main_win.c                                        *
** Author:         bin.wang1                                                 *
** Date:           04/09/2020                                                *
** Copyright:      All Rights Reserved.                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2020      bin.wang1              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


//#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"

#include "guilistbox.h"
//#include "guilabel.h"
//#include "guibutton.h"
//#include "guitext.h"
//#include "version.h"
#include "watch_common_list.h"
//#include "mmidisplay_data.h"
#include "mmicom_trace.h"
#include "mmiset_image.h"
//#include "mmiset_text.h"
//#include "mmiset_id.h"
#include "mmiwlan_image.h"
#include "mmiwlan_text.h"
#include "mmiwlan_id.h"
//#include "mmiset_export.h"
#include "mmiwifi_export.h"
#include "mmiwlan_profile.h"
#include "mmiwlan_manager.h"
#include "watch_commonwin_export.h"
#include "gui_ucs2b_converter.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define WIFI_MAIN_LIST_PERMANENT_ITEM_NUM       (1)
#define WIFI_MAIN_LIST_WLAN_ITEM_INDEX          (0)

typedef void ( *WLAN_ITEM_FUNC )( void );

/**--------------------------------------------------------------------------*
 **                         RECT DEFINITION                                  *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

//Bug 1489852
LOCAL uint32 WifiSsidToWchar( wchar* wstr_ptr, uint32 wstr_len,
                                    uint8* ssid_ptr, uint32 ssid_len, uint32 convert_len )
{
    uint32               codeLen = 0;
    GUI_CHAR_CODE_TYPE_E codeType = GUI_CHAR_CODE_UTF8;

    if( PNULL == wstr_ptr || PNULL == ssid_ptr )
    {
        TRACE_APP_WLAN(" wstr_ptr or ssid_ptr is PNULL!!");
        return codeLen;
    }

    codeType = GUI_GetCodeType( ssid_ptr, ssid_len );
    if( GUI_CHAR_CODE_UTF8 == codeType )
    {
        codeLen = GUI_UTF8ToWstr( wstr_ptr, wstr_len, ssid_ptr, convert_len );
    }
    else
    {
        if( codeType != GUI_CHAR_CODE_ANSI )
        {
            TRACE_APP_WLAN("not support code type %d",codeType);
        }
        codeLen = GUI_GBToWstr( wstr_ptr, ssid_ptr, convert_len );
    }
    return codeLen;
}

//================================================================================/
//====================== wlan main->connect AP query win =========================/
//================================================================================/
LOCAL MMI_RESULT_E _Callback_ConnectAPQueryWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    BOOLEAN                         reCode = 0;
    BOOLEAN                         isConnected = FALSE;
    MMI_RESULT_E                    recode =  MMI_RESULT_TRUE;
    MMIWIFI_PROFILE_T*              pProfile = PNULL;
    WIFISUPP_ENCRYP_PROTOCOL_E      encrypProtocol = WIFISUPP_ENCRYP_PROTOCOL_OPENSYS;

    TRACE_APP_WLAN("Enter msg_id is 0x%x",msg_id);

    pProfile = (MMIWIFI_PROFILE_T*)MMK_GetWinUserData( win_id );
    if( PNULL == pProfile )
    {
        TRACE_APP_WLAN("pProfile is PNULL!!");
        return MMI_RESULT_FALSE;
    }

    TRACE_APP_WLAN("pProfile->state %d pProfile->is_trusted %d", pProfile->state, pProfile->is_trusted);

    switch( msg_id )
    {
#ifdef ADULT_WATCH_SUPPORT
        case MSG_NOTIFY_CANCEL:
#else
        case MSG_NOTIFY_OK:
#endif
        {
            if( MMIWIFI_PROFILE_STATE_CONNECTED == pProfile->state || MMIWIFI_PROFILE_STATE_DHCPED == pProfile->state )
            {
                MMIWIFI_DisConnect( pProfile );

                pProfile->is_editing = FALSE;
                pProfile->is_auto_connect = FALSE; //Bug 1333825
                TRACE_APP_WLAN("profile_ptr->is_auto_connect = FALSE;");
                pProfile->is_trusted = FALSE; //Bug 1343080
                TRACE_APP_WLAN("profile_ptr->is_trusted = FALSE;");

                MMIWIFI_UpdateProfileFile();

                MMK_CloseWin( win_id );
                break;  // !!!! forget break;
            }

            encrypProtocol = pProfile->config_info.encryp_protocol;
            MMIWIFI_SetCurrProfile( pProfile );

            if( TRUE == pProfile->is_trusted  )
            {
                reCode = MMIWIFI_DirectConnect( &(pProfile->config_info) );
                pProfile->is_editing = FALSE;
                MMK_CloseWin( win_id );
                MMIWIFI_OpenConnectWaitWin();
            }
            else
            {
                if( WIFISUPP_ENCRYP_PROTOCOL_OPENSYS == encrypProtocol )
                {
                    reCode = MMIWIFI_DirectConnect( &(pProfile->config_info) );
                    pProfile->is_editing = FALSE;
                    MMK_CloseWin( win_id );
                    MMIWIFI_OpenConnectWaitWin();
                }
                else if( WIFISUPP_ENCRYP_PROTOCOL_WPA == encrypProtocol || WIFISUPP_ENCRYP_PROTOCOL_WPA2 == encrypProtocol
                    || WIFISUPP_ENCRYP_PROTOCOL_WPA_WPA2 == encrypProtocol || WIFISUPP_ENCRYP_PROTOCOL_WPA3_SAE == encrypProtocol
                    || WIFISUPP_ENCRYP_PROTOCOL_WPA2_WPA3_SAE == encrypProtocol || WIFISUPP_ENCRYP_PROTOCOL_WPA2_PSK_SHA256 == encrypProtocol)//Bug 1343913
                {
                    if( WIFISUPP_WPA_CREDENTIAL_TYPE_PSK == pProfile->config_info.credential.wpa_credential.credential_type )
                    {
                        WIFI_PasswordWin_Enter( pProfile );
                    }
                    else
                    {
                        TRACE_APP_WLAN("Not support credential_type!!");
                    }
                }
                else
                {
                    TRACE_APP_WLAN("Not support encryp_protocol!!");
                }
            }
            break;
        }
#ifdef ADULT_WATCH_SUPPORT
        case MSG_NOTIFY_OK:
#else
        case MSG_NOTIFY_CANCEL:
#endif
        {
            MMK_CloseWin( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            pProfile->is_editing = FALSE;
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

LOCAL void WIFI_ConnectAPQueryWin_Enter( MMIWIFI_PROFILE_T* profile_ptr )
{
    BOOLEAN                     isConnected = FALSE;
    MMI_STRING_T                mmiTipStr = { 0 };
    wchar                       dispTipWchar[WIFISUPP_SSID_MAX_LEN+1] = { 0 };
    MMI_STRING_T                mmiLeftBtnStr = { 0 };
    MMI_STRING_T                mmiRightBtnStr = { 0 };
    MMI_STRING_T                text_str = {0};
    MMI_WIN_ID_T                winId = MMIWIFI_AP_CONNECT_WIN_ID;
    WATCH_SOFTKEY_TEXT_ID_T     softkey = { TXT_COMMON_CONNECT_COMN, TXT_NULL, STXT_RETURN };

    TRACE_APP_WLAN("Enter profile_ptr 0x%x",profile_ptr);

    if ( PNULL == profile_ptr )
    {
        TRACE_APP_WLAN("profile_ptr is PNULL!!!");
        return;
    }

    if (FALSE == profile_ptr->is_on_air)//Bug 1592136
    {
#ifdef ADULT_WATCH_SUPPORT
        MMI_GetLabelTextByLang( TXT_WIFI_NOT_IN_RANGE, &text_str );
        Adult_WatchCOM_NoteWin_1Line_Toast_Enter( MMIWIFI_CONNECT_BUSY_WIN_ID,&text_str,PNULL,PNULL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_WIFI_NOT_IN_RANGE);
#endif
        return;
    }
    profile_ptr->is_editing = TRUE; /* Cannot to be deleted by Auto Scan */

    mmiTipStr.wstr_len = WifiSsidToWchar( dispTipWchar, WIFISUPP_SSID_MAX_LEN, profile_ptr->config_info.ssid.ssid,
                                profile_ptr->config_info.ssid.ssid_len, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN) );
    mmiTipStr.wstr_ptr = dispTipWchar;

    if( (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) || (MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state) )
    {
        isConnected = TRUE;
    }

    if ( TRUE == isConnected )
    {
#ifdef ADULT_WATCH_SUPPORT
        MMI_GetLabelTextByLang( TXT_WLAN_DISCONNECT_NETWORK, &mmiTipStr );
#else
        MMI_GetLabelTextByLang( TXT_WIFI_AP_FORGET, &mmiLeftBtnStr );
#endif
        softkey.softkeyLeft = TXT_WIFI_AP_FORGET;
    }
    else
    {
#ifdef ADULT_WATCH_SUPPORT
        MMI_GetLabelTextByLang( TXT_WLAN_CONNECT_NETWORK, &mmiTipStr );
#else
        MMI_GetLabelTextByLang( TXT_COMMON_CONNECT_COMN, &mmiLeftBtnStr );
#endif
        softkey.softkeyLeft = TXT_COMMON_CONNECT_COMN;
    }

    MMI_GetLabelTextByLang( STXT_RETURN, &mmiRightBtnStr );

#ifdef  ADULT_WATCH_SUPPORT
    AdultWatchCOM_QueryWin_2Btn_Enter( winId, &mmiTipStr, PNULL, PNULL, IMAGE_SET_COM_AW_BTN_CANCEL, IMAGE_SET_COM_AW_BTN_CONFIRM, _Callback_ConnectAPQueryWin );
#else
    WatchCOM_QueryWin_2Btn_Enter( winId, &mmiTipStr, &mmiLeftBtnStr, &mmiRightBtnStr, PNULL, PNULL, softkey, _Callback_ConnectAPQueryWin );
#endif
    MMK_SetWinUserData( winId, (void*)profile_ptr );
}


//================================================================================/
//================================= wlan main win=================================/
//================================================================================/

LOCAL void WifiMainHandleWifiOnOff( void )
{
    BOOLEAN isWlanOn = MMIWIFI_GetIsWlanOn();;

    TRACE_APP_WLAN("Enter isWlanOn %d", (int32)isWlanOn);

    if ( TRUE == isWlanOn )
    {
        MMIAPIWIFI_Off();
    }
    else
    {
        MMIAPIWIFI_On( TRUE );
    }
}

LOCAL uint32 WifiMainGetListUserdata( MMI_CTRL_ID_T list_ctrl_id, uint16 index )
{
    uint32 userData = 0;

    GUILIST_GetItemData( list_ctrl_id, index, (uint32*)(&userData) );

    return userData;
}

LOCAL void WifiMainAddWlanListItem( MMI_CTRL_ID_T list_ctrl_id )
{
    MMI_IMAGE_ID_T      imageId = IMAGE_CHECK_UNSELECTED_ICON;
    BOOLEAN             isWlanOn = FALSE;
    MMI_STRING_T        itemStr1 = { 0 };
    MMI_STRING_T        itemStr2 = { 0 };
    uint32              userData = 0;

    TRACE_APP_WLAN("Enter");
#ifdef ADULT_WATCH_SUPPORT
    MMI_GetLabelTextByLang( TXT_SET_WIFI_WLAN, &itemStr1 );
#else
    MMI_GetLabelTextByLang( TXT_WIFI_WLAN, &itemStr1 );
#endif
    isWlanOn = MMIWIFI_GetIsWlanOn();

    // status image
    if( TRUE == isWlanOn )
    {
        imageId = IMAGE_CHECK_SELECTED_ICON;
    }

    // status text
    if( TRUE == isWlanOn )
    {
        if ( TRUE == MMIWIFI_GetScanStatus() )
        {
#ifdef ADULT_WATCH_SUPPORT
            MMI_GetLabelTextByLang( TXT_COMMON_SEARCHING, &itemStr2 );//Bug 1568936
#else 
            MMI_GetLabelTextByLang( TXT_SCANING_WLAN, &itemStr2 );
#endif
        }
        else
        {
            MMI_GetLabelTextByLang( TXT_COMMON_OPEN, &itemStr2 );
        }
    }
    else
    {
        MMI_GetLabelTextByLang( TXT_CLOSE, &itemStr2 );
    }
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line( list_ctrl_id, itemStr1, itemStr2, imageId );
#else
    WatchCOM_ListItem_Draw_2Str_1Icon( list_ctrl_id, itemStr1, itemStr2, imageId );
#endif
    userData = (uint32)WifiMainHandleWifiOnOff;
    GUILIST_SetItemUserData( list_ctrl_id, WIFI_MAIN_LIST_WLAN_ITEM_INDEX, &userData );

}

LOCAL BOOLEAN WifiMainGetAPItemIndex( MMI_CTRL_ID_T ctrl_id, MMIWIFI_PROFILE_T* profile_ptr, uint16* index_ptr )
{
    uint16              index = WIFI_MAIN_LIST_PERMANENT_ITEM_NUM;  //AP item start after wlan item
    uint16              itemNum = 0;
    MMIWIFI_PROFILE_T   *pProfileInList = PNULL;

    if( PNULL == profile_ptr )
    {
        TRACE_APP_WLAN("profile_ptr is PNULL!!! ");
        return FALSE;
    }

    TRACE_APP_WLAN("Enter ssid %s", profile_ptr->config_info.ssid.ssid);

    if( PNULL == index_ptr )
    {
        TRACE_APP_WLAN("index_ptr is PNULL!!! ");
        return FALSE;
    }

    itemNum = GUILIST_GetTotalItemNum( ctrl_id );

    if( (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) || (MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state) )
    {
        *index_ptr = index;
        TRACE_APP_WLAN("Exit index %d", index);
        return TRUE;
    }

    if( FALSE == profile_ptr->is_on_air )
    {
        *index_ptr = itemNum;
        TRACE_APP_WLAN("Exit index %d", itemNum);
        return TRUE;
    }

    for( index = WIFI_MAIN_LIST_PERMANENT_ITEM_NUM; index < itemNum; index++ )
    {
        pProfileInList = (MMIWIFI_PROFILE_T*)WifiMainGetListUserdata( ctrl_id, index );
        if( PNULL == pProfileInList )
        {
            TRACE_APP_WLAN("pProfileInList is PNULL!!! ");
            return FALSE;
        }

        if( (MMIWIFI_PROFILE_STATE_CONNECTED == pProfileInList->state) || (MMIWIFI_PROFILE_STATE_DHCPED == pProfileInList->state) )
        {
            continue; // connected ap is index 1, other after it
        }

        //compare air->trusted->rssi
        if  ( (PNULL == profile_ptr) ||
             (profile_ptr->is_on_air && !pProfileInList->is_on_air) ||
             (profile_ptr->is_on_air && pProfileInList->is_on_air && profile_ptr->is_trusted && !pProfileInList->is_trusted) ||
             (profile_ptr->is_on_air && pProfileInList->is_on_air && profile_ptr->is_trusted && pProfileInList->is_trusted && profile_ptr->rssi > pProfileInList->rssi) ||
             (profile_ptr->is_on_air && pProfileInList->is_on_air && !profile_ptr->is_trusted && !pProfileInList->is_trusted && profile_ptr->rssi > pProfileInList->rssi) )
        {
            break;
        }
    }

    *index_ptr = index;
    TRACE_APP_WLAN("Exit index %d", index);
    return TRUE;
}

LOCAL void WifiMainInsertAPListItem( MMI_CTRL_ID_T list_ctrl_id, MMIWIFI_PROFILE_T* profile_ptr )
{
    MMI_IMAGE_ID_T          imageId = IMAGE_WIFI_AP_OPENSYS_ICON;
    MMI_TEXT_ID_T           textId = TXT_NULL;
    MMI_STRING_T            itemStr1 = {0};
    MMI_STRING_T            itemStr2 = {0};
    uint16                  pWchar[WIFISUPP_SSID_MAX_LEN+1] = { 0 };
    uint32                  userData = 0;
    uint32                  wcharLen = 0;
    uint16                  itemIndex = 0;
    BOOLEAN                 reCode = FALSE;

    //TRACE_APP_WLAN(" Enter");

    if ( PNULL == profile_ptr )
    {
        TRACE_APP_WLAN("profile_ptr is PNULL!!");
        return;
    }

    // image id
    if( WIFISUPP_ENCRYP_PROTOCOL_OPENSYS != profile_ptr->config_info.encryp_protocol )
    {
        imageId = IMAGE_WIFI_AP_LOCK_ICON;
    }

    // ssid
    wcharLen = WifiSsidToWchar( pWchar, WIFISUPP_SSID_MAX_LEN, profile_ptr->config_info.ssid.ssid,
                                profile_ptr->config_info.ssid.ssid_len, MIN(profile_ptr->config_info.ssid.ssid_len, WIFISUPP_SSID_MAX_LEN) );
    itemStr1.wstr_len = (uint16)wcharLen;
    itemStr1.wstr_ptr = pWchar;

    // status
    if( TRUE == profile_ptr->is_on_air )
    {
        if( (MMIWIFI_PROFILE_STATE_CONNECTED == profile_ptr->state) || (MMIWIFI_PROFILE_STATE_DHCPED == profile_ptr->state) ) 
        {
            textId = TXT_COMMON_CONNECTED;
        }
        else
        {
            switch( profile_ptr->config_info.encryp_protocol ) 
            {
                case WIFISUPP_ENCRYP_PROTOCOL_OPENSYS:
                    textId = TXT_WIFI_AUTH_TYPE_OPENSYS;
                    break;

                case WIFISUPP_ENCRYP_PROTOCOL_WPA:
                    textId = TXT_WIFI_AUTH_TYPE_WPA;
                    break;

                case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
                    textId = TXT_WIFI_AUTH_TYPE_WPA2;
                    break;

                case WIFISUPP_ENCRYP_PROTOCOL_WPA_WPA2:
                {
                    textId = TXT_WIFI_AUTH_TYPE_WPA_WPA2;
                    break;
                }

                case WIFISUPP_ENCRYP_PROTOCOL_WPA3_SAE:
                {
                    textId = TXT_WIFI_AUTH_TYPE_WPA3_SAE;
                    break;
                }

                case WIFISUPP_ENCRYP_PROTOCOL_WPA2_PSK_SHA256:
                {
                    textId = TXT_WIFI_AUTH_TYPE_WPA2_PSK_SHA256;
                    break;
                }

                case WIFISUPP_ENCRYP_PROTOCOL_WPA2_WPA3_SAE:
                {
                    textId = TXT_WIFI_AUTH_TYPE_WPA2_WPA3_SAE;
                    break;
                }
                default:
                {
                    TRACE_APP_WLAN("not support encryp_protocol!!!");
                    return;
                }
            }
        }
    }
    else
    {
        if ( TRUE == profile_ptr->is_ssid_hide )
        {
            textId = TXT_UNKNOWN;
        }
        else
        {
            textId = TXT_WIFI_NOT_IN_RANGE;
        } 
    }

    MMI_GetLabelTextByLang( textId, &itemStr2 );

    reCode = WifiMainGetAPItemIndex( list_ctrl_id, profile_ptr, &itemIndex );
    if( TRUE == reCode )
    {
#ifdef ADULT_WATCH_SUPPORT
        AdultWatchCOM_ListItem_Insert_1Icon_2Str_2line( list_ctrl_id, imageId, itemStr1, itemStr2, itemIndex );
#else
        WatchCOM_ListItem_Insert_1Icon_2Str( list_ctrl_id, imageId, itemStr1, itemStr2, itemIndex );
#endif
        userData = (uint32)profile_ptr;
        GUILIST_SetItemUserData( list_ctrl_id, itemIndex, &userData );
    }
}

LOCAL void Wifi_Main_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T listCtrlId =  MMIWIFI_MAIN_LIST_CTRL_ID;

    TRACE_APP_WLAN("Enter");

    MMK_SetAtvCtrl( win_id, listCtrlId );

    GUILIST_SetMaxItem( listCtrlId, (MMIWIFI_MAX_PROFILE_NUM+WIFI_MAIN_LIST_PERMANENT_ITEM_NUM), FALSE );
#ifdef ADULT_WATCH_SUPPORT
    GUILIST_SetListState( listCtrlId, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );//fix bug1594837 1595403 1586984
#endif
    MMIWIFI_StartAutoScan();
}

LOCAL void Wifi_Main_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16              index = 0;
    uint16              curIndex = 0;
    uint16              topIndex = 0;
    uint32              num = 0;
    BOOLEAN             reCode = FALSE;
    BOOLEAN             isWlanOn = FALSE;
    MMI_CTRL_ID_T       listCtrlId =  MMIWIFI_MAIN_LIST_CTRL_ID;
    MMIWIFI_PROFILE_T*  pProfile = PNULL;

    TRACE_APP_WLAN("Enter");

    curIndex = GUILIST_GetCurItemIndex( listCtrlId );
    topIndex = GUILIST_GetTopItemIndex( listCtrlId );
    GUILIST_RemoveAllItems( listCtrlId );

    // WLAN item
    WifiMainAddWlanListItem( listCtrlId );

    // AP items
    isWlanOn = MMIWIFI_GetIsWlanOn();
    if( TRUE == isWlanOn )
    {
        num = MMIWIFI_GetProfileNum();
        for( index = 0; index < num; index++ )
        {
            pProfile = MMIWIFI_GetProfileByIndex( index );
            WifiMainInsertAPListItem( listCtrlId, pProfile );
        }
    }

    GUILIST_SetTopItemIndex( listCtrlId, topIndex );
    GUILIST_SetCurItemIndex( listCtrlId, curIndex );
}

LOCAL void Wifi_Main_APP_OK( MMI_WIN_ID_T win_id )
{
    uint16              index = 0;
    uint32              userData = 0;
    BOOLEAN             reCode = FALSE;
    WLAN_ITEM_FUNC      itemFunc = PNULL;
    MMI_CTRL_ID_T       listCtrlId =  MMIWIFI_MAIN_LIST_CTRL_ID;
    MMIWIFI_PROFILE_T*  pProfile = PNULL;

    TRACE_APP_WLAN("Enter");

    index = GUILIST_GetCurItemIndex( listCtrlId );
    userData = WifiMainGetListUserdata( listCtrlId, index );

    if( WIFI_MAIN_LIST_WLAN_ITEM_INDEX == index )
    {
        itemFunc = (WLAN_ITEM_FUNC)userData;
        if ( PNULL != itemFunc )
        {
            (*itemFunc)();
        }
    }
    else
    {
        pProfile = (MMIWIFI_PROFILE_T*)userData;
        WIFI_ConnectAPQueryWin_Enter( pProfile );
    }
}

LOCAL void Wifi_Main_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    TRACE_APP_WLAN("Enter");

    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E  HandleWifiMainWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

    TRACE_APP_WLAN("msg_id 0x%x", msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Wifi_Main_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            Wifi_Main_FULL_PAINT( win_id );
            break;
        }

        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Wifi_Main_APP_OK( win_id );
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Wifi_Main_APP_CANCEL( win_id );
            break;
        }
#ifdef  ADULT_WATCH_SUPPORT
        case MSG_KEYDOWN_USER_FUNC://fix bug1594837
        {
            MMK_SendMsg(win_id, MSG_APP_DOWN, PNULL);
            break;
        }
#endif

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

WINDOW_TABLE( MMIWIFI_MAIN_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleWifiMainWindow ),
    WIN_ID( MMIWIFI_MAIN_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_SET_WIFI_WLAN),
#endif
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIWIFI_MAIN_LIST_CTRL_ID ),
#ifndef ADULT_WATCH_SUPPORT
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

/****************************************************************************/
//  Description : enter wifi main window
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void WatchWIFI_MainWin_Enter( void )
{
    TRACE_APP_WLAN("Enter");

    MMK_CreateWin((uint32*)MMIWIFI_MAIN_WIN_TAB, PNULL);
}

