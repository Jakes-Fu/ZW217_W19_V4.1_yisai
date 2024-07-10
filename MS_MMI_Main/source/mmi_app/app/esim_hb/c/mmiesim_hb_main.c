#ifdef WIN32
#include "std_header.h"
#endif
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "os_api.h"
#include "sci_types.h"
#include "guilistbox.h"
#include "guitext.h" 
#include "watch_common_list.h"
#include "watch_commonwin_export.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmicom_trace.h"
#include "mmi_common.h"
#include "mmi_image.h"
#include "mmitheme_text.h"
#include "mmiesim_hb_id.h"
#include "mmiesim_hb_text.h"
#include "mmiesim_hb_image.h"
#include "mmidisplay_color.h"
#include "mmi_appmsg.h"
#include "hb_api.h"
#include "hb_export.h"

#ifndef UAL_BT_BLE_DEV_SUPPORT_NONE
#include "ual_bt_ble_dev.h"
#include "ual_bt_ble_private.h"
#endif
#include "mn_api.h"

#define MMI_ESIM_HB_MAX_TEXT_LENGTH  (256*2)
#define MMI_ESIM_HB_INFO_TEXT_LEFT 60
#define MMI_ESIM_HB_INFO_TEXT_TOP 60
#define MMI_ESIM_HB_INFO_TEXT_RIGHT 300
#define MMI_ESIM_HB_INFO_TEXT_BOTTOM 300

LOCAL void MMI_Esim_EUICC_Info_Enter(void);
LOCAL void MMI_Esim_ICCID_Info_Enter(void);

LOCAL MMI_RESULT_E  HandleEsimInformationWinMsg(
                                     MMI_WIN_ID_T win_id,
                                     MMI_MESSAGE_ID_E msg_id,
                                     DPARAM param
                                     );
LOCAL MMI_RESULT_E  HandleEUICCInfoWinMsg(
                                     MMI_WIN_ID_T win_id,
                                     MMI_MESSAGE_ID_E msg_id,
                                     DPARAM param
                                     );
LOCAL MMI_RESULT_E  HandleICCIDInfoWinMsg(
                                     MMI_WIN_ID_T win_id,
                                     MMI_MESSAGE_ID_E msg_id,
                                     DPARAM param
                                     );

LOCAL void ESIM_DrawMainListItem(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id );

LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST esim_main_list_item_iccid = {TXT_ESIM_ICCID,IMAGE_ESIM};
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST esim_main_list_item_euicc = {TXT_ESIM_EUICC,IMAGE_ESIM};
LOCAL const WATCHCOM_LIST_ITEM__ST esim_main_list_info[] =
{
    { WatchCOM_List_Item_Visible_Default,GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH,&esim_main_list_item_iccid,MMI_Esim_ICCID_Info_Enter},
    { WatchCOM_List_Item_Visible_Default,GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH,&esim_main_list_item_euicc,MMI_Esim_EUICC_Info_Enter},
};

WINDOW_TABLE( MMI_ESIM_INFORMATION_WINTAB ) =
{
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32) HandleEsimInformationWinMsg),
        WIN_ID(ESIM_INFO_MENU_WIN_ID ),
        WIN_TITLE(TXT_ESIM_INFO),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, ESIM_INFO_LIST_CTRL_ID),
        //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
        END_WIN
};
WINDOW_TABLE( MMI_ESIM_EUICC_INFO_WINTAB ) =
{
        WIN_FUNC((uint32) HandleEUICCInfoWinMsg),
        WIN_ID( ESIM_EUICC_WIN_ID ),
        WIN_HIDE_STATUS,
        WIN_TITLE(TXT_ESIM_EUICC),
        CREATE_TEXT_CTRL( ESIM_EUICC_CTRL_ID),
        //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
        END_WIN
};
WINDOW_TABLE( MMI_ESIM_ICCID_INFO_WINTAB ) =
{
        WIN_FUNC((uint32) HandleICCIDInfoWinMsg),
        WIN_ID( ESIM_ICCID_WIN_ID ),
        WIN_HIDE_STATUS,
        WIN_TITLE(TXT_ESIM_ICCID),
        CREATE_TEXT_CTRL( ESIM_ICCID_CTRL_ID),
        //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
        END_WIN
};

LOCAL void ESIM_DrawMainListItem(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id )
{
    uint32 listNum = 0;
    listNum = sizeof(esim_main_list_info)/sizeof(WATCHCOM_LIST_ITEM__ST);
    WatchCOM_TextList_Create( esim_main_list_info,listNum,ctrl_id);
    MMK_SetAtvCtrl(win_id,ctrl_id);
}

LOCAL MMI_RESULT_E  HandleEsimInformationWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16    index = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id = ESIM_INFO_LIST_CTRL_ID;
    TRACE_APP_ESIM_HB("HandleEsimInformationWinMsg:msg_id=%d> ",msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        case MSG_GET_FOCUS:
            {
                ESIM_DrawMainListItem(win_id, ctrl_id);
                break;
            }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            {
                WatchCOM_List_Item_CallBack(ctrl_id);
                break;
            }
        case MSG_LOSE_FOCUS:
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                MMK_CloseWin(win_id);
                break;
            }
        case MSG_CLOSE_WINDOW:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleEUICCInfoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id = ESIM_EUICC_CTRL_ID;
    MMI_STRING_T    text_string = {0};
    MMI_STRING_T    temp_string = {0};
    GUI_RECT_T      text_rect = {MMI_ESIM_HB_INFO_TEXT_LEFT,MMI_ESIM_HB_INFO_TEXT_TOP,MMI_ESIM_HB_INFO_TEXT_RIGHT,MMI_ESIM_HB_INFO_TEXT_BOTTOM};
    char eid_str[HB_PRIVATE_EID_STR_LENGTH] = {0};
    wchar eid_wstr[HB_PRIVATE_EID_STR_LENGTH] = {0};
    wchar temp_wstr[MMI_ESIM_HB_MAX_TEXT_LENGTH]={0};
    BOOLEAN eid_ret= FALSE;
    TRACE_APP_ESIM_HB("HandleEUICCInfoWinMsg:msg_id=%d> ",msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
#ifndef WIN32
                eid_ret = ESIM_HB_GetEIDInfo(&eid_str);// TODO: get eid from W307 platform api
#endif
                if(TRUE == eid_ret)
                {
                    MMI_GetLabelTextByLang(TXT_ESIM_EID,&temp_string);
                    MMIAPICOM_Wstrncpy(temp_wstr, temp_string.wstr_ptr, temp_string.wstr_len);
                    MMIAPICOM_StrToWstr(&eid_str,&eid_wstr);
                    MMIAPICOM_Wstrncat(temp_wstr,eid_wstr,HB_PRIVATE_EID_STR_LENGTH);
                    text_string.wstr_ptr = temp_wstr;
                    text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_ESIM_ERROR, &text_string);
                    GUITEXT_SetString(ctrl_id, text_string.wstr_ptr,text_string.wstr_len,FALSE);
                }
                GUITEXT_SetString(ctrl_id, text_string.wstr_ptr,text_string.wstr_len,FALSE);
                GUITEXT_SetRect(ctrl_id,&text_rect);
                MMK_SetAtvCtrl(win_id, ctrl_id); 
                TRACE_APP_ESIM_HB("HandleEUICCInfoWinMsg:eid_ret=%d,text_string.wstr_len length=%d",eid_ret,text_string.wstr_ptr);
                break;
            }
        case MSG_GET_FOCUS:
                break;
        case MSG_LOSE_FOCUS:
                break;
        case MSG_FULL_PAINT:
                break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            {
                MMK_CloseWin(win_id);
                break;
            }
        case MSG_CLOSE_WINDOW:
                break;
        default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleICCIDInfoWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id = ESIM_ICCID_CTRL_ID;
    MMI_STRING_T    text_string = {0};
    MMI_STRING_T    temp_string = {0};
    GUI_RECT_T      text_rect = {MMI_ESIM_HB_INFO_TEXT_LEFT,MMI_ESIM_HB_INFO_TEXT_TOP,MMI_ESIM_HB_INFO_TEXT_RIGHT,MMI_ESIM_HB_INFO_TEXT_BOTTOM};

    ERR_MNDATAMAG_CODE_E iccid_ret = ERR_MNDATAMAG_RETURN_ERROR;
    uint8 iccid_num = 0;
    char iccid_str[MNSIM_ICCID_ID_NUM_LEN*2] = {0};
    wchar iccid_wstr[MNSIM_ICCID_ID_NUM_LEN*2] = {0};
    wchar temp_wstr[MMI_ESIM_HB_MAX_TEXT_LENGTH] = {0};
    wchar temp_n[2]= {0x0A,0};
    int i = 0;
    MNSIM_ICCID_T iccid = {0};

    TRACE_APP_ESIM_HB("HandleICCIDInfoWinMsg:msg_id=%d> ",msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
#ifndef WIN32
                iccid_ret = MNSIM_GetICCIDEx(MN_SYS_SIM_1,&iccid);
#endif
                TRACE_APP_ESIM_HB(" iccid_ret: %d ", iccid_ret);
                for(i=0; i<MNSIM_ICCID_ID_NUM_LEN; i++)
                {
                    TRACE_APP_ESIM_HB("ICCID: 0x%x ", iccid.id_num[i]);
                }

                if(ERR_MNDATAMAG_NO_ERR == iccid_ret)
                {
                    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, iccid.id_num, MNSIM_ICCID_ID_NUM_LEN*2, iccid_str);
                    TRACE_APP_ESIM_HB("COMMOND iccid_str: %s ", iccid_str);
                    MMIAPICOM_StrToWstr(iccid_str, iccid_wstr);
                    text_string.wstr_ptr = iccid_wstr;
                    text_string.wstr_len = MNSIM_ICCID_ID_NUM_LEN*2;
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_ESIM_ERROR,&text_string);
                }
                GUITEXT_SetString(ctrl_id, text_string.wstr_ptr,text_string.wstr_len,FALSE);
                GUITEXT_SetRect(ctrl_id,&text_rect);
                MMK_SetAtvCtrl(win_id,ctrl_id);
                break;
            }
        case MSG_GET_FOCUS:
                break;
        case MSG_LOSE_FOCUS:
                break;
        case MSG_FULL_PAINT:
                break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
                {
                    MMK_CloseWin(win_id);
                    break;
                }
        case MSG_CLOSE_WINDOW:
                break;
        default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

LOCAL void MMI_Esim_EUICC_Info_Enter(void)
{
    MMK_CreateWin((uint32*)MMI_ESIM_EUICC_INFO_WINTAB, PNULL);
}
LOCAL void MMI_Esim_ICCID_Info_Enter(void)
{
    MMK_CreateWin((uint32*)MMI_ESIM_ICCID_INFO_WINTAB, PNULL);
}
PUBLIC void MMI_Esim_HB_Information_Enter(void)
{
    MMK_CreateWin((uint32*)MMI_ESIM_INFORMATION_WINTAB, PNULL);
}
