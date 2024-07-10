/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_NOTIFICATIONS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
//#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
//#include "mmiset_about.h"
#include "guires.h"
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiudisk_export.h"
#include "guiownerdraw.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "guisetlist.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_livewallpaper_text.h"
#endif
#include "mmk_tp.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_func.h"
#include "mmi_resource.h"
#include "mmiset_export.h"
#include "mmiset_security.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_export.h"
#endif
#ifdef UI_P3D_SUPPORT
#include "mmi_winse.h"
#endif
#include "mmiset_internal.h"
#include "mmiset_lock_effect_internal.h"

//#ifdef PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM
#include "mmiset_mainmenu_export.h"
//#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmiset_lock_effect_export.h"
#endif
#include "guitext.h"
#include "guiwin.h"
#include "mmi_theme.h"
#include "guirichtext.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmieng_export.h"
#ifdef MMI_DEVICELOCK_SUPPORT
#include "mmi_devicelock_export.h"
#endif
#include "guilabel.h"
#include "version.h"
#include "production_test.h"
#include "mmicom_banner.h"
#include "mmipb_app.h"
#include "device_info.h"
#ifdef MSA_SUPPORT
#include "mmimsa_export.h"
#endif
#ifdef NPS_SUPPORT
#include "nps_id.h"
#endif

/*----------------------------------------------------------------------------*/
/*                         Data/Variables                                     */
/*----------------------------------------------------------------------------*/
LOCAL THEMELIST_ITEM_STYLE_T new_style = {0};
PUBLIC void MMIZDT_OpenSettingWin();

//about setting
enum {
    ID_ABOUT_PRODUCT_INFO,
    ID_ABOUT_RESET,
    ID_ABOUT_SEND_FEEDBACK
};
LOCAL MMISET_LOCKSCREEN_SECURITY_CODE_T devicelock_code ={0};

/**--------------------------------------------------------------------------*
**                function declaration                                       *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetAboutWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);
LOCAL MMI_RESULT_E HandleSetProductInfoWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);
LOCAL MMI_RESULT_E HandleSetUpdateWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);
#ifdef NPS_SUPPORT
LOCAL MMI_RESULT_E HandleSetFeedbackWindow(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);
#endif
/*****************************************************************************/
//  Description : handle input reset factory password win msg
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAboutInputResetFactoryPwd(
                                               MMI_WIN_ID_T    win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                               );
/*****************************************************************************/
//  Description : 处理恢复出厂设置和清楚用户数据的等待窗口
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetOrCleanDataWaitWin(
                                                 MMI_WIN_ID_T    win_id, // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                 DPARAM             param   // 相应消息的参数
                                                 );

 #ifndef FLASH_SUPPORT
/*****************************************************************************/
//  Description : handle prompt win of theme
//  Global resource dependence :
//  Author: lu.life
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePromptCompleteThemeWin(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                );
#endif

LOCAL MMI_RESULT_E handleConfirmationDialog(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                );

//lulifei add for the reset UI
PUBLIC void MMIAPISET_OpenAboutResetPwdWin(BOOLEAN      is_clean_all_data);

PUBLIC BOOLEAN MMIIDLE_ResetIsCorrectCode(const wchar *code_ptr, uint16 pwd_len);


/*****************************************************************************/
// the window for select font
WINDOW_TABLE( MMISET_SET_ABOUT_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetAboutWindow ),
    WIN_ID(MMISET_SET_ABOUT_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_ABOUT_CTRL_ID),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

    END_WIN
};

WINDOW_TABLE( MMISET_SET_PRODUCT_INFO_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetProductInfoWindow ),
    WIN_ID(MMISET_SET_PRODUCT_INFO_WIN_ID),
    CREATE_RICHTEXT_CTRL(MMISET_SET_PRODUCT_INFO_CTRL_ID),
    WIN_SOFTKEY(NULL, NULL, TXT_COMMON_COMMON_BACK),

    END_WIN
};

WINDOW_TABLE( MMISET_SET_UPDATE_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetUpdateWindow ),
    WIN_ID(MMISET_SET_UPDATE_WIN_ID),
    CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_HORIZONTAL_PROCESS, MMISET_SET_UPDATE_CTRL_ID),
    WIN_SOFTKEY(NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

    END_WIN
};

#ifdef NPS_SUPPORT

WINDOW_TABLE( MMISET_SET_FEEDBACK_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetFeedbackWindow ),
    WIN_ID(MMISET_SET_FEEDBACK_WIN_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_SET_FEEDBACK_FORM_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE,MMISET_SET_FEEDBACK_TEXT_CTRL_ID,MMISET_SET_FEEDBACK_FORM_CTRL_ID),
    //CREATE_EDIT_DIGITAL_CTRL(3, MMISET_SET_FEEDBACK_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,2,MMISET_SET_FEEDBACK_CTRL_ID,MMISET_SET_FEEDBACK_FORM_CTRL_ID),
    WIN_SOFTKEY(NULL, TXT_SEND, TXT_COMMON_COMMON_BACK),

    END_WIN
};

#endif

/*****************************************************************************/
//  Description : open phone setting shortcut window
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_EnterSettingAboutWin(void) {
    MMK_CreateWin((uint32 *)MMISET_SET_ABOUT_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : add a hyper text sting
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL void AddRichTextItem(MMI_CTRL_ID_T ctrl_id,wchar *w_str,uint8 type) {
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              index = 0;

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;
    item_data.tag_type = type;
    item_data.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
    item_data.text_data.buf.str_ptr = w_str;
    item_data.text_set_font = TRUE;
    item_data.setting_font = MMI_DEFAULT_BIG_FONT;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

}
/*****************************************************************************/
//  Description : 判断焦点是否在网址上
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFocusOnURL(MMI_CTRL_ID_T ctrl_id) {
#ifdef BROWSER_SUPPORT
    GUIRICHTEXT_FOCUS_ELEM_T focus_element = {0};
    BOOLEAN result = FALSE;

    GUIRICHTEXT_GetFocusElement(ctrl_id, &focus_element);

    if (GUIRICHTEXT_TAG_URL == focus_element.type) {
        result = TRUE;
    }

    return result;
#else
    return FALSE;
#endif
}
/*****************************************************************************/
//  Description : 打开网址
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void GoToFocusedURL(MMI_CTRL_ID_T ctrl_id,MMI_STRING_T url_str) {
#ifdef BROWSER_SUPPORT
    GUIRICHTEXT_FOCUS_ELEM_T focus_element = {0};
    MMIBROWSER_ENTRY_PARAM entry_para = {0};
    uint8 * url_ptr = PNULL;
    uint16 url_len = 0;

    GUIRICHTEXT_GetFocusElement(ctrl_id, &focus_element);

    if (GUIRICHTEXT_TAG_URL == focus_element.type) {
        if (PNULL != focus_element.data.str_ptr) {
            url_len = url_str.wstr_len * 3 + 1;
            url_ptr = SCI_ALLOCA(url_len * sizeof(uint8));
            if (PNULL == url_ptr) {
                return;
            }

            SCI_MEMSET(url_ptr, 0x00, (url_str.wstr_len * 3 + 1) * sizeof(uint8));
            GUI_WstrToUTF8(url_ptr, url_len, url_str.wstr_ptr, url_str.wstr_len);
            entry_para.url_ptr = (char*)url_ptr;
            entry_para.type = MMIBROWSER_ACCESS_URL;
            entry_para.dual_sys = MN_DUAL_SYS_MAX;
            entry_para.user_agent_ptr = NULL;
            MMIAPIBROWSER_Entry(&entry_para);
        }
    }
#endif
}
/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetAboutWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T               str_id[] = {
		STR_SETT_LIST_PRODUCT_EXT01,
		STR_SETT_LIST_RESET_SETTINGS_EXT01,
#ifdef NPS_SUPPORT
		STR_NPS_ILIST_FEEDBACK_JT
#endif
		};
    int16                       i = 0;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint16                      max_items = 0;
    uint16                      cur_item = 0;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_ABOUT_CTRL_ID;
    MMI_WIN_ID_T                dialog_win_id = MMIPUB_ALERT_WIN_ID;
    MMI_STRING_T temp_str={0};

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ctrl_id);
#ifdef NPS_SUPPORT
            max_items = 3;
#else
            max_items = 2;
#endif
            cur_item  = 0;
            //SCI_TRACE_LOW:"[MMIDC] HandleDcStorageOptWinMsg max_items=%d,file_dev=%d"
            GUILIST_SetMaxItem(ctrl_id,max_items, FALSE );
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
            item_t.item_data_ptr = &item_data;

            for(i = 0; i < max_items; i++) {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = str_id[i];

                if(!GUILIST_InsertItem(ctrl_id, &item_t, i)) {
                    break;
                }
            }

            GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_item);
            break;
        case MSG_FULL_PAINT: 
#ifdef NPS_SUPPORT
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            if (!(cur_item == ID_ABOUT_SEND_FEEDBACK && MMK_GetFocusWinId() == MMINPS_MAIN_SELECT_WIN_ID)) 
            {
                //reset status bar
                MMI_STRING_T    temp_str = {0};
                MMI_GetLabelTextByLang(STR_SETT_HDR_ABOUT_EXT01, &temp_str);
                if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
                    GUIWIN_UpdateStb();
                }
            }
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            switch (cur_item) {
                case ID_ABOUT_PRODUCT_INFO:
                    MMK_CreateWin((uint32 *)MMISET_SET_PRODUCT_INFO_WIN_TAB, PNULL);
                    break;

                //case ID_ABOUT_CHECK_FOR_UPDATE://
                    //to do.....
                //    MMK_CreateWin((uint32 *)MMISET_SET_UPDATE_WIN_TAB, PNULL);
                //    break;
                case ID_ABOUT_RESET:
                    MMIAPISET_OpenAboutResetPwdWin(TRUE);
                    break;

#ifdef NPS_SUPPORT
                case ID_ABOUT_SEND_FEEDBACK://
					//MMIAPINPS_OpenMainMenuWindow(FALSE);
                    MMIAPINPS_OpenMainMenuSelectWindow(TRUE);
                    //MMK_CreateWin((uint32 *)MMISET_SET_FEEDBACK_WIN_TAB, PNULL);
                    break;
#endif
                default:
                    break;
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
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetProductInfoWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_STRING_T                productinfo_string = {0};
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_PRODUCT_INFO_CTRL_ID;
    wchar           			unicode_cr = 0x0d;
    MMI_STRING_T    			cr_s = {0};
    uint16          			uint16_str[300 + 1] = {0};
    uint16          			uint16_str_len = 0;
    uint16          			uint16_privacy_str[300 + 1] = {0};
    uint16          			uint16_privacy_str_len = 0;
    uint16          			uint16_service_str[300 + 1] = {0};
    uint16          			uint16_service_str_len = 0;
    char*           			temp_ptr = PNULL;
	char*           			temp_p = PNULL;
    uint16          			temp_len = 0;
	MMI_STRING_T    			src_str = {0};
	MMI_STRING_T    			result_string = {0};
	wchar 						result_wchar[200] = {0};
	PRODUCTION_TEST_INFO_T 		s_mmieng_product = {0};
    uint16 						sim_sys = MN_DUAL_SYS_1;
    wchar                       operator_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
    MMI_STRING_T                operator_str = {0};
	result_string.wstr_ptr 	= result_wchar;
    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id,ctrl_id);
            GUIRICHTEXT_SetFocusMode(ctrl_id,GUIRICHTEXT_FOCUS_SHIFT_ADVANCED);
			MMIAPIPHONE_GetCarrierName(MN_DUAL_SYS_1);
			//Model
		    MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_DETAILS_MODEL_EXT01);
#ifndef _WIN32
			temp_ptr = PRODUCT_TYPE_Read();
#else
			temp_ptr = (char*)"RM-1182";
#endif
		    temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			//Carrier
		    MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_DETAILS_CARRIER_EXT01);
            operator_str.wstr_ptr = (wchar *)operator_name;
            operator_str.wstr_len = 0;  
			if(MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1)>0)
			{
                MMIPHONE_GetNetworkNameString(MMIAPISET_GetActiveSim(),&operator_str);
                MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &operator_str);
			}
			else
			{
		    	MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_NSM_IND_NO_SIM);
			}
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			//SW Version
			MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_DETAILS_SOFTWARE_EXT01);
#ifndef _WIN32
		    temp_ptr = VERSION_GetInfo(PROJECT_VERSION);
			temp_p = strchr(temp_ptr,':')+1;
			temp_len = SCI_STRLEN(temp_p);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_p, temp_len);
#else
		    temp_ptr = (char*)"xxxx";
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
#endif
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			//OS Version
			MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_DETAILS_OS_VERSION_EXT01);
#ifndef _WIN32
			temp_ptr = VERSION_GetInfo(PLATFORM_VERSION);
			temp_p = strchr(temp_ptr,':')+1;
			temp_len = SCI_STRLEN(temp_p);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_p, temp_len);
#else
			temp_ptr = (char*)"xxxx";
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
#endif
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			//FW Version
			MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_PROMPT_FIRMWARE_NUM_EXT01);
#ifndef _WIN32
			temp_ptr = VERSION_GetInfo(HW_VERSION);
			temp_p = strchr(temp_ptr,':')+1;
			temp_len = SCI_STRLEN(temp_p);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_p, temp_len);
#else
			temp_ptr = (char*)"xxxx";
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
#endif
			MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			//HW Number
		    MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_DETAILS_HARDWARE_EXT01);
#ifndef _WIN32

			if (SCI_SUCCESS == SCI_GetProductionTestInfo(&s_mmieng_product))
			{
				uint8 text_buf[151] = {0};
				temp_len = SCI_STRLEN(s_mmieng_product.serial_num);
				temp_ptr = (char*) s_mmieng_product.serial_num;
				MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
				MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			}
#else
			temp_ptr = (char*)"xxxx";
			temp_len = SCI_STRLEN((char*)temp_ptr);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
#endif
		//IMEI SV
		{
			MN_IMEISVN_T imeisvn = {0};
			MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,TXT_SET_IMEI_SV);
			if(MNNV_GetIMEISVNEx(MN_DUAL_SYS_1, &imeisvn))
			{
		          if (imeisvn.svn_flag)
		          {
		            sprintf((char *)temp_ptr, " %02d", imeisvn.svn);
		            temp_len = (uint16)SCI_STRLEN((char *)temp_ptr);
					MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
					MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
		          }else{
		            temp_ptr = (char*)"00";
		            temp_len = (uint16)SCI_STRLEN((char *)temp_ptr);
					MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
					MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
				}
			}
			else
			{
			     temp_ptr = (char*)"00";
		            temp_len = (uint16)SCI_STRLEN((char *)temp_ptr);
					MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
					MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			}
		}
			//details ms
			{
				MMI_GetLabelTextByLang(STR_DETAILS_YYYY_MS_EXT01, &src_str);
				MMIAPICOM_CustomGenerateReplaceStr(&src_str, L"%U", L"@2015", 200, &result_string);
			}
            MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
            MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
			//MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_str_len,STR_DETAILS_YYYY_MS_EXT01);

            //AddRichTextItem(ctrl_id,L"Model:xxxx\nCarrier:xxxx\nSoftware:xxxx\nOS version:xxxx\nFirmware number:xxxx\nHardware number:xxxx\n@2015 Microsoft Corp.",GUIRICHTEXT_TAG_TXT_MASK);
            AddRichTextItem(ctrl_id,&uint16_str,GUIRICHTEXT_TAG_NONE);

			//privacy statement
		    //MMIAPICOM_StrcatFromTextIDToUCS2(uint16_service_str, &uint16_service_str_len,STR_SERVICES_AGREEMENT);
			//AddRichTextItem(ctrl_id,&uint16_service_str,GUIRICHTEXT_TAG_URL);
            GUIRICHTEXT_SetLineSpace(ctrl_id,0);
            CTRLRICHTEXT_SetItemSpace(ctrl_id,0);
			//privacy statement
		    //MMIAPICOM_StrcatFromTextIDToUCS2(uint16_privacy_str, &uint16_privacy_str_len,STR_READ_ABOUT_PRIVACY_DH);
			//MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_privacy_str, &uint16_privacy_str_len, &cr_s);//换行
		    //MMIAPICOM_StrcatFromTextIDToUCS2(uint16_privacy_str, &uint16_privacy_str_len,STR_READ_ABOUT_PRIVACY_EXT02);
            //AddRichTextItem(ctrl_id,L"Terms of use\nPrivacy Statement",GUIRICHTEXT_TAG_URL);
            //AddRichTextItem(ctrl_id,&uint16_privacy_str,GUIRICHTEXT_TAG_URL);
			//productinfo_string.wstr_ptr = L"Model:xxxx\nCarrier:xxxx\nSoftware:xxxx\nOS version\nFirmware number:xxxx\nHardware number:xxxx\n@2015 Microsoft Corp.\n\nTerms of use\nPrivacy Statement";
            //productinfo_string.wstr_len = BRW_WStrlen(productinfo_string.wstr_ptr);
            GUIRICHTEXT_SetFocusItem(ctrl_id,1);
			GUICTRL_SetProgress(MMK_GetCtrlPtr(ctrl_id),FALSE);//hide scroll bar
            break;
		case MSG_FULL_PAINT:
			{
				//reset status bar
				MMI_STRING_T    temp_str = {0};
				MMI_GetLabelTextByLang(STR_SETT_HDR_PRODUCT_EXT01, &temp_str);
				if(MMIAPICOM_StatusAreaSetViewHeaderinfo(temp_str.wstr_ptr, temp_str.wstr_len)) {
				    GUIWIN_UpdateStb();
				}
			}
			break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            if (IsFocusOnURL(ctrl_id)) {
                MMI_STRING_T sPrivacy_url = {0};
				GUIRICHTEXT_FOCUS_ELEM_T focus_element = {0};
				MMI_STRING_T text_string = {0};

				//MMI_GetLabelTextByLang(STR_READ_ABOUT_PRIVACY_DH,&text_string);
				GUIRICHTEXT_GetFocusElement(ctrl_id, &focus_element);
				if(MMIAPICOM_Wstrncmp(focus_element.data.str_ptr,text_string.wstr_ptr,text_string.wstr_len)==0)
				{
	                sPrivacy_url.wstr_ptr = L"http://aka.ms/privacy";
	                sPrivacy_url.wstr_len = MMIAPICOM_Wstrlen(sPrivacy_url.wstr_ptr);
				}else{
	                sPrivacy_url.wstr_ptr = L"http://aka.ms/msa";
	                sPrivacy_url.wstr_len = MMIAPICOM_Wstrlen(sPrivacy_url.wstr_ptr);
				}
                GoToFocusedURL(ctrl_id,sPrivacy_url);
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
//  Description :HandleSetUpdateWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSetUpdateWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_UPDATE_CTRL_ID;
    GUI_BOTH_RECT_T             both_prg_rect= {0};
    int progress = 0;
    switch(msg_id) {
        case MSG_OPEN_WINDOW: {
            GUI_BOTH_RECT_T title_both_rect=MMITHEME_GetWinTitleBothRect(win_id);
            GUI_BOTH_RECT_T screen_both_rect=MMITHEME_GetFullScreenBothRect();
            uint16 height = 67;//GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT);

            //MMK_SetAtvCtrl(win_id,ctrl_id);
            both_prg_rect.v_rect.left =15;
            both_prg_rect.v_rect.right = screen_both_rect.v_rect.right-both_prg_rect.v_rect.left ;
            both_prg_rect.v_rect.top = 200;
            both_prg_rect.v_rect.bottom = both_prg_rect.v_rect.top+height;

            both_prg_rect.h_rect.left = 15;
            both_prg_rect.h_rect.right =screen_both_rect.h_rect.right-both_prg_rect.h_rect.left  ;
            both_prg_rect.h_rect.top = 230;
            both_prg_rect.h_rect.bottom =230+14;

            GUIAPICTRL_SetBothRect(ctrl_id,&both_prg_rect);
            GUIPRGBOX_SetParamByCtrl(ctrl_id, 100, 1);
            GUIPRGBOX_SetPosByCtrl(TRUE, 0, 0, ctrl_id);
        }
        break;
        case MSG_FULL_PAINT:
            //GUIPRGBOX_SetParamByCtrl(ctrl_id, 100, 1);
            //GUIPRGBOX_SetPosByCtrl(TRUE, progress++, 0, ctrl_id);
            //if(progress!=100)
            //  MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
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
//  Description : create EDITBOX
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateFeedbackEditBox(const MMI_STRING_T *text_ptr,
                         uint16       max_len,
                         uint8        allow_inputmethod,
                         MMI_WIN_ID_T win_id) {
    MMISET_LANGUAGE_TYPE_E  lang_type = MMISET_LANGUAGE_ENGLISH;
    GUIIM_TYPE_T         input_mode = 0;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    wchar                   dot_str[] = {'.'};
#ifndef MMI_PDA_SUPPORT
    MMI_CONTROL_CREATE_T    create = {0};
#endif
#ifdef MMI_PDA_SUPPORT
    GUIFORM_DYNA_CHILD_T  child_data= {0};
    GUISOFTKEY_INIT_DATA_T      softkey_init = {0};
#endif
    uint16  layer_width = 0;
    uint16  layer_height = 0;
	GUI_RECT_T editbox_rect = {0};

    if(GUIIM_TYPE_DIGITAL == allow_inputmethod) {
        //only digital mode
        input_mode = GUIIM_TYPE_DIGITAL;
    } else {
        MMIAPISET_GetLanguageType(&lang_type);
        if (MMISET_LANGUAGE_SIMP_CHINESE == lang_type) {
            input_mode = GUIIM_TYPE_SMART;
        } else {
            input_mode = GUIIM_TYPE_ABC;
        }
    }
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);

    editbox_rect.left = 0;
    editbox_rect.top = 200;
    editbox_rect.right = layer_width;
    editbox_rect.bottom = editbox_rect.top + 40;

    init_data.type = GUIEDIT_TYPE_TEXT;
    init_data.both_rect= MMITHEME_GetWinClientBothRect(win_id);
    init_data.str_max_len = max_len;

    create.ctrl_id = MMISET_SET_FEEDBACK_CTRL_ID;
    create.guid    = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = win_id;//MMIPB_EDITBOX_WIN_ID;
    create.init_data_ptr = &init_data;
    MMK_CreateControl( &create );

    GUIEDIT_SetIm(create.ctrl_id,allow_inputmethod,input_mode);
    if(GUIIM_TYPE_DIGITAL == input_mode) {
        GUIEDIT_ConfigImCustomKey(create.ctrl_id, input_mode, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(create.ctrl_id, GUIIM_TAG_DOT_NUM);
    }
    if (PNULL != text_ptr) {
        GUIEDIT_SetString(create.ctrl_id,text_ptr->wstr_ptr,text_ptr->wstr_len);
    }
}

/*****************************************************************************/
//  Description :HandleSetUpdateWindow
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
#ifdef NPS_SUPPORT

LOCAL MMI_RESULT_E  HandleSetFeedbackWindow(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) {
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_FEEDBACK_FORM_CTRL_ID;
	MMI_CTRL_ID_T               editctrl_id = MMISET_SET_FEEDBACK_CTRL_ID;
	MMI_CTRL_ID_T               textctrl_id = MMISET_SET_FEEDBACK_TEXT_CTRL_ID;
    GUI_BOTH_RECT_T             both_prg_rect= {0};
    MMI_STRING_T                label_string = {0};
	BOOLEAN 					is_disp_im_icon = TRUE;
	BOOLEAN 					is_disp_num_info = FALSE;
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
			{
				wchar *str_ptr = L"How likely are you to recommend this phone to a friend or colleague?\n\n10:Extremely likely\n0:Not at all likely";
				//L"How likely are you to recommend this phone to a friend or colleague?\n10:Extremely likely\n0:Not at all likely";
				GUIFORM_CHILD_HEIGHT_T      child_height = {0};
    			GUI_BG_T    				gui_bg = {0};
				GUI_FONT_T 					font = CAF_FONT_20;
				GUI_COLOR_T 				font_color = MMI_WHITE_COLOR;
				MMI_STRING_T                string_dst = {0};
				MMI_STRING_T                string_src = {0};
				MMI_STRING_T                string_1 = {0};
				MMI_STRING_T                string_2 = {0};
				MMI_STRING_T                string_3 = {0};
				/*
				MMI_GetLabelTextByLang(STR_NPS_QUERY_FEEDBACK_EXT02, &string_src);
				MMI_GetLabelTextByLang(TXT_SET_N, &string_1);
				MMI_GetLabelTextByLang(STR_NPS_LIST_0_EXT02, &string_3);
				MMI_GetLabelTextByLang(STR_NPS_LIST_10_EXT02, &string_2);

				//MMIAPICOM_CatTwoString(&string_dst,&string_src, &string_1);
				MMIAPICOM_Wstrcat(string_src.wstr_ptr,L"\n\n");
				string_src.wstr_len = string_src.wstr_len+4;
				//SCI_MEMSET(string_src.wstr_ptr, 0, ((string_src.wstr_len + 1) * sizeof(wchar)));
				//MMIAPICOM_CatTwoString(&string_src,&string_dst, &string_1);
				//SCI_MEMSET(string_dst.wstr_ptr, 0, ((string_dst.wstr_len + 1) * sizeof(wchar)));
				MMIAPICOM_CatTwoString(&string_dst,&string_src, &string_2);
				MMIAPICOM_Wstrcat(string_dst.wstr_ptr,L"\n");
				string_dst.wstr_len = string_dst.wstr_len+2;
				SCI_MEMSET(string_src.wstr_ptr, 0, ((string_src.wstr_len + 1) * sizeof(wchar)));
				string_src.wstr_len = 0;
				MMIAPICOM_CatTwoString(&string_src,&string_dst, &string_3);
				MMIAPICOM_Wstrcat(string_src.wstr_ptr,L"\n");
				string_src.wstr_len = string_src.wstr_len+2;
				*/
				gui_bg.bg_type = GUI_BG_COLOR;
			    gui_bg.color = MMI_BLACK_COLOR;
			    GUIFORM_SetBg(ctrl_id, &gui_bg);
	            GUIFORM_IsSlide(ctrl_id,FALSE);

				GUIFORM_PermitChildBg(ctrl_id, FALSE);
        		//GUIFORM_SetStyle(ctrl_id,GUIFORM_STYLE_UNIT);
	            GUIFORM_SetType(ctrl_id,GUIFORM_TYPE_TP);
	            GUIFORM_PermitChildBorder(ctrl_id, FALSE);
				GUIFORM_PermitChildFont(ctrl_id,FALSE);
	            GUIFORM_SetMargin(ctrl_id, 0);
	            GUIFORM_SetDividingLine(ctrl_id,0,PNULL);
	            child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
	            child_height.add_data=180;
	            GUIFORM_SetChildHeight(ctrl_id, textctrl_id, &child_height);
				GUITEXT_SetBg(textctrl_id,&gui_bg);
				GUITEXT_SetFont(textctrl_id,&font,&font_color);
				GUITEXT_SetString(textctrl_id,str_ptr,MMIAPICOM_Wstrlen(str_ptr),FALSE);
				//GUITEXT_SetString(textctrl_id,string_src.wstr_ptr,string_src.wstr_len,FALSE);
			}

			CTRLBASEEDIT_SetLabelId(win_id,editctrl_id,STR_NPS_GIVE_SCORE_EXT02,FALSE);
			GUIEDIT_SetIm (editctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
		    GUIEDIT_SetDispImIcon(editctrl_id,&is_disp_im_icon,&is_disp_num_info);
		    GUIEDIT_SetStyle(editctrl_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
            GUIEDIT_GetString(editctrl_id, &label_string);
            GUIEDIT_SetFont(editctrl_id,SONG_FONT_20);
			GUIEDIT_SetFontColor(editctrl_id,MMI_WHITE_COLOR);
            if(PNULL != label_string.wstr_ptr && 0 != label_string.wstr_len)
			{
                GUIEDIT_SetString(editctrl_id, label_string.wstr_ptr, label_string.wstr_len);
            }
			CTRLBASEEDIT_SetHintText(win_id,editctrl_id,L"type a number",(uint16)MMIAPICOM_Wstrlen(L"type a number"),TRUE);
            MMK_SetAtvCtrl(win_id, editctrl_id);
            break;
		case MSG_FULL_PAINT:
			GUIEDIT_SetSoftkeyEx(editctrl_id,
				2,
				1,
				TXT_COMMON_COMMON_BACK,
				STR_LIST_DELETE,
				PNULL,
				PNULL,
				EDIT_BUTTON_TEXT_ID,
				PNULL
				);
			break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
			MMIAPINPS_OpenMainMenuWindow(FALSE);
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

#endif

/*****************************************************************************/
//  Description : handle input reset factory password win msg
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAboutInputResetFactoryPwd(
                                               MMI_WIN_ID_T    win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                               )
{
    MMI_STRING_T                old_pwd_value           =   {0};
    MMI_RESULT_E                result                  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id                 =   MMISET_INPUT_RESET_FACTORY_PWD_CTRL_ID;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;
    MMI_WIN_ID_T                wait_win_id             =   0;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUIIM_TYPE_T         input_mode = GUIIM_TYPE_DIGITAL;
    wchar                   dot_str[] = {'.'};
    MMI_CTRL_ID_T ctrl_list_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID );

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if(PNULL != add_data_ptr)
    {
        is_delete_all_data= (uint32)add_data_ptr->user_data;
    }

    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:
        GUIEDIT_ConfigImCustomKey(ctrl_list_id, input_mode, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(ctrl_list_id, GUIIM_TAG_DOT_NUM);
        MMIPUB_SetDialogBgLayerAlpha(win_id, 127);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
    break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_KEYUP_OK:
	break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    //case MSG_APP_MENU:
    case MSG_KEYUP_WEB:
        // get the value about PIN
        GUIEDIT_GetString(MMIPUB_INPUTFIELD_CTRL_ID,&old_pwd_value);
        
        if (PRIVACY_PROTECT_PWD_LEN_IS_IN_RANGE(old_pwd_value.wstr_len))
        {
            //密码是否正确
            if( MMIIDLE_ResetIsCorrectCode(old_pwd_value.wstr_ptr,old_pwd_value.wstr_len))
                //if (MMISET_IsCorrectPrivacyProtectPwd( old_pwd_value.wstr_ptr,old_pwd_value.wstr_len))
            {
                if (is_delete_all_data)
                {
                    //询问是否清楚用户数据!
                    //MMIPUB_OpenQueryWinByTextId(TXT_SET_CLEAN_USER_DATA,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,STR_QUERY_RESET_SETTINGS_EXT01,PNULL,PNULL,&win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,handleConfirmationDialog,(uint32)is_delete_all_data);
                }
                else
                {
                    //询问是否恢复出厂设置!!
                    //MMIPUB_OpenQueryWinByTextId(TXT_COMMON_RESTORE_FACTORY_SETTINGS,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_SET_CLEAN_USER_DATA,PNULL,PNULL,&win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,handleConfirmationDialog,(uint32)is_delete_all_data);
                }
                
            }
            else
            {
                //清空password, yuanl modify 2009.9.4 for cr151094
                GUIEDIT_ClearAllStr(MMIPUB_INPUTFIELD_CTRL_ID);

                //提示密码错误!
                //MMIPUB_OpenAlertWarningWin(STR_LOCK_CODE_REJE_EXT01);
                banner_info.banner_type = MMI_BANNER_TEXT;
                banner_info.text1_id = STR_LOCK_CODE_REJE_EXT01;
                //banner_info.start_point = start_point;
               MMIAPICOM_OpenBannerWin(win_id, banner_info);
#if 0//Del by yuanl  2009.9.4 for cr151094
                //清空password
                GUIEDIT_ClearAllStr(MMIPUB_INPUTFIELD_CTRL_ID);
#endif
            }
        }
        break;

    case MSG_SET_CLEAN_DATE_IND:
#if defined(MSA_SUPPORT)		
        if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
        {
            MMIAPIMSA_sign_off(MSA_APP_TYPE_CAMERA_ROLL);
            MMK_CloseWin(win_id);
        }
        else
#endif
        {
            if (is_delete_all_data)
            {
                //清除所有数据
                MMISET_CleanUserData();
                MMISET_ResetFactorySetting();
                MMK_CloseWin(win_id);
            }
            else
            {
                //进行恢复出厂设置
                MMISET_ResetFactorySetting();
                MMK_CloseWin(win_id);
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(PNULL);

        //进入等待界面
        wait_win_id = MMISET_RESET_FACTORY_WAITING_WIN_ID;
        MMIPUB_OpenAlertWinByTextId(&time_out,STR_NOTE_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,
            &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,
            HandleResetOrCleanDataWaitWin);

        MMK_PostMsg(win_id, MSG_SET_CLEAN_DATE_IND, PNULL,PNULL);
        break;

    case MSG_PROMPTWIN_CANCEL:
        //关闭询问窗口
        //MMIDEFAULT_EnableKeyRing(TRUE);
        //MMIDEFAULT_EnableTpRing(TRUE);
        //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_REEEESET_FACT, TRUE);
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_FULL_PAINT:
        {
            MMI_STRING_T text_content= {0};
            GUIEDIT_GetString(ctrl_id,&text_content);
            if(0 == text_content.wstr_len)
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,2,TRUE);
            }
            else
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id,STR_LIST_DELETE,2,TRUE);
            }
            GUIWIN_SetSoftkeyBtnState(win_id,1,FALSE,FALSE);
        }
        break;

    case MSG_CLOSE_WINDOW:
        //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RESET_FACT, TRUE);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        GUIEDIT_SetIm(ctrl_list_id,input_mode,input_mode);
        break;
    }

    return (result);


}
/*****************************************************************************/
//  Description : 处理恢复出厂设置和清楚用户数据的等待窗口
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetOrCleanDataWaitWin(
                                                 MMI_WIN_ID_T    win_id, // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                 DPARAM             param   // 相应消息的参数
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    static uint32   s_waitting_num = 0;
    static BOOLEAN is_cleandata =FALSE;
    SCI_TRACE_LOW("mmiset_about.c HandleResetOrCleanDataWaitWin: msg_id=0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_waitting_num = 0;
       GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL,RIGHT_BUTTON,TRUE);
       GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
       GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
        //recode  =   MMI_RESULT_FALSE;
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
//yangyu add begin
    case MSG_FULL_PAINT:
    {
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        GUI_RECT_T rect = {0,0, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT- 1};
        
        GUI_RECT_T	text_rect={0,MMI_MAINSCREEN_HEIGHT/2-15, MMI_MAINSCREEN_WIDTH - 1 , MMI_MAINSCREEN_HEIGHT/2+15};
    	MMI_STRING_T tmp_string = {0};
    	MMI_STRING_T        content_str = {0};
    	wchar               content_text[100 + 1] = {0};
    	uint16 uint16_str_len = 0;
    	GUISTR_STYLE_T      text_style = {0};
    	GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    	text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    	text_style.font_color = MMI_GREEN_COLOR;
    	text_style.angle = ANGLE_0;
    	text_style.char_space = 0;
    	text_style.effect = FONT_EFFECT_CUSTOM;
    	text_style.edge_color = 0;
#if defined(ZDT_MMI_USE_SMALL_FONT)
    	text_style.font = SONG_FONT_22;//SONG_FONT_36*SCALE;//SONG_FONT_26;
#else
    	text_style.font = SONG_FONT_28;//SONG_FONT_36*SCALE;//SONG_FONT_26;
#endif    	
    	text_style.line_space = 0;
    	text_style.region_num = 0;
    	text_style.region_ptr = PNULL;	
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WATCH_RESETING,&tmp_string);
    	text_style.align = ALIGN_HVMIDDLE;
    	text_style.font_color = MMI_WHITE_COLOR;
	#if defined(ZDT_MMI_USE_SMALL_FONT)
    	text_style.font = SONG_FONT_22;//SONG_FONT_26*SCALE;	
	#else
    	text_style.font = SONG_FONT_28;//SONG_FONT_26*SCALE;
    	#endif
    	
    	GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    	GUISTR_DrawTextToLCDInRect( 
    		        MMITHEME_GetDefaultLcdDev(),
    		        (const GUI_RECT_T      *)&text_rect,       //the fixed display area
    		        (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
    		        (const MMI_STRING_T    *)&tmp_string,
    		        &text_style,
    		        state,
    		        GUISTR_TEXT_DIR_AUTO
    		        );	
    }
        break;
//yangyu end
    case MSG_APP_RED:
        break;
    case MSG_SET_RESET_NEED_WAIT_IND:
        //add one wait action
        s_waitting_num++;
        SCI_TRACE_LOW("mmiset_about.c HandleResetOrCleanDataWaitWin: add s_waitting_num=%d", s_waitting_num);
        break;
    case MSG_SET_RESET_FACTORY_OVER_IND:
        if(s_waitting_num > 0)
        {
            //finished one, wait number -1
            s_waitting_num --;
        }
        SCI_TRACE_LOW("mmiset_about.c HandleResetOrCleanDataWaitWin: minus s_waitting_num=%d, is_cleandata=%d", s_waitting_num, is_cleandata);
        //no one to wait, finished
        if(s_waitting_num == 0)
        {
            if(is_cleandata)
            {
                //MMK_CloseWin(win_id);
                //MMIAPIPHONE_PowerReset();//yangyu modify
                MMIAPIPHONE_PowerResetByFactoryReset();
                
            }
            else
            {
                //MMK_CloseWin(win_id);
                MMK_ReturnIdleWin(); //crMS00187558 @murphy.xie
                //success!
#ifndef FLASH_SUPPORT
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePromptCompleteThemeWin);
#endif
            }
        }
        break;

    case MSG_SET_CLEAN_USER_DATA_OVER_IND:
        is_cleandata = TRUE;
        break;

    case MSG_CLOSE_WINDOW:
        //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RESET_FACT, TRUE);
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_ClearAllDevice();
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}
#ifndef FLASH_SUPPORT
/*****************************************************************************/
//  Description : handle prompt win of theme
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePromptCompleteThemeWin(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;

    switch(msg_id)
    {
    case MSG_CLOSE_WINDOW:
        if(MMK_IsOpenWin(MMISET_INPUT_RESET_FACTORY_PWD_WIN_ID))
        {
            MMK_CloseWin(MMISET_INPUT_RESET_FACTORY_PWD_WIN_ID);
        }
#ifdef MULTI_THEME_SUPPORT
        else if(MMK_IsOpenWin(MMISET_SET_THEME_PREVIEW_WIN_ID))
        {
            MMK_CloseWin(MMISET_SET_THEME_PREVIEW_WIN_ID);
        }
        //        else if(MMK_IsOpenWin(MMISET_SET_THEME_WIN_ID))
        //        {
        //            MMK_CloseWin(MMISET_SET_THEME_WIN_ID);
        //         }
#endif
        break;

    default:
        break;
    }

    result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    return (result);
}
#endif

/*****************************************************************************/
//  Description : handle comfrimation win of reset
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E handleConfirmationDialog(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    MMI_WIN_ID_T                wait_win_id             =   0;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL != add_data_ptr)
    {
        is_delete_all_data= (uint32)add_data_ptr->user_data;
    }

    switch (msg_id)
    {
    //zhuyongjing add
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
        break;
    case MSG_SET_CLEAN_DATE_IND:
#if defined(MSA_SUPPORT)		
        if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
        {
            MMIAPIMSA_sign_off(MSA_APP_TYPE_CAMERA_ROLL);
            MMK_CloseWin(win_id);
        }
        else
#endif
        {
            if (is_delete_all_data)
            {
                //清除所有数据
                SCI_TRACE_LOW("mmiset_about_handleConfirmationDialog clearalldata reset");
                MMISET_CleanUserData();
                MMISET_ResetFactorySetting();
                MMK_CloseWin(win_id);
            }
            else
            {
                //进行恢复出厂设置
                MMISET_ResetFactorySetting();
                MMK_CloseWin(win_id);
            }
        }
        break;
    //add end
    case MSG_APP_OK:
        //break;
    case MSG_APP_WEB:
        MMIPUB_CloseQuerytWin(PNULL);
		//进入等待界面
        wait_win_id = MMISET_RESET_FACTORY_WAITING_WIN_ID;
//          MMIPUB_OpenAlertWinByTextId(&time_out,STR_NOTE_WAITING,TXT_NULL,ANIM_PUBWIN_WAIT,
//              &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,
//              HandleResetOrCleanDataWaitWin);

        {
            MMI_STRING_T    wait_text = {0};
            MMI_GetLabelTextByLang (STR_NOTE_WAITING, &wait_text);
            MMIPUB_OpenWaitWin (1, &wait_text, PNULL, PNULL, wait_win_id,
                COMMON_IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL,
                MMIPUB_SOFTKEY_NONE, HandleResetOrCleanDataWaitWin);
        }

        MMK_PostMsg(win_id, MSG_SET_CLEAN_DATE_IND, PNULL,PNULL);
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

   result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    return (result);
}

//lulifei add for the reset input dialog
PUBLIC void MMIAPISET_OpenAboutResetPwdWin(BOOLEAN      is_clean_all_data)
{
   MMI_STRING_T temp_str={0};
   MMISET_LOCKSCREEN_SECURITY_CODE_T device_lock_pwd = {0};
   MMI_CTRL_ID_T               ctrl_id = MMISET_SET_ABOUT_CTRL_ID;
   MMI_WIN_ID_T                dialog_win_id = MMIPUB_ALERT_WIN_ID;
   uint32  tmp     =   is_clean_all_data;


   if(MMIDEVICELOCKSCREEN_ReadPwd(&device_lock_pwd))
   {
       if(device_lock_pwd.is_enable)
       {
           MMI_GetLabelTextByLang(STR_SETT_ILIST_SECURITY_CODE_EXT01, &temp_str);
           //MMIPUB_OpenDialogInputFieldWinByTextPtrEx(MMK_GetFirstAppletHandle(),&temp_str,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleAboutInputResetFactoryPwd,(uint32)tmp);
           MMIPUB_OpenDialogPasswordWinByTextPtrEx(MMK_GetFirstAppletHandle(),&temp_str,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleAboutInputResetFactoryPwd,(uint32)tmp);
       }
       else
       {
           MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,STR_QUERY_RESET_SETTINGS_EXT01,PNULL,PNULL,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handleConfirmationDialog,(uint32)tmp);
       }
   }
   else
   {
       // the pwd is not set. Could ask user directly
       MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,STR_QUERY_RESET_SETTINGS_EXT01,PNULL,PNULL,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handleConfirmationDialog,(uint32)tmp);
   }
}

PUBLIC BOOLEAN MMIIDLE_ResetIsCorrectCode(const wchar *code_ptr, uint16 pwd_len)
{
    BOOLEAN                 result = FALSE;

	    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;

		   MMINV_READ(MMINV_DEVICELOCKSCREEN_PWD,&devicelock_code,return_value);
		   if (MN_RETURN_SUCCESS == return_value){

		   if( devicelock_code.pwd_len == pwd_len
			   && 0 == MMIAPICOM_WstrStrncmp(code_ptr, (uint8 *)devicelock_code.pwd, pwd_len))
		   	{
		   	   result = TRUE;

		   }else {

				result = FALSE;

		   }
		   	}else{
			   result = FALSE;

			}

    return (result);
}

//===============yangyu add begin, for new setting ui=================
LOCAL MMI_RESULT_E  HandleZDT_SettingWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );



WINDOW_TABLE( MMIZDT_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_SettingWinMsg),    
    WIN_ID( MMIZDT_SETTING_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZDT_OpenSettingWin()
{
   if(MMK_GetFocusWinId() != MMIZDT_SETTING_WIN_ID)
   {
        MMK_CreateWin((uint32*)MMIZDT_SETTING_WIN_TAB,PNULL);
   }
}

#ifdef FOTA_SUPPORT
#define LIST_COUNT  8
#else
#define LIST_COUNT  7
#endif
void  MMIZDT_ShowSettingList(MMI_WIN_ID_T win_id)
{
    BOOLEAN ret = FALSE;
    MMI_STRING_T    cur_str_t   = {0};
    uint16 tempStr[101] ={0};
    uint16 i = 0, j = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T bg_img_id = IMAGE_SETTING_LIST_ITEM_BG;
    uint16 position = 0;
    MMI_TEXT_ID_T text_id_list[LIST_COUNT] = 
    {
        text_ringing_type,
        zdt_backlight_timeout,
        TXT_COMMON_COMMON_BRIGHTNESS,
        text_volume_settings,
        //TEXT_WATCH_INFO,
        qtn_sett_list_reset_settings_bc,
        text_setting_poweroff,
        //text_setting_powerrestart,
        TEXT_WATCH_INFO
#ifdef FOTA_SUPPORT
		,
        TXT_SETTINGS_ABUP_FOTA_CHECK
#endif
        
    };
    MMI_IMAGE_ID_T img_id_list[LIST_COUNT] = 
    {
        IMAGE_RINGTONE_SET_ICON,
        IMAGE_DISPLAYTIME_SET_ICON,
        IMAGE_BRIGHTNESS_SET_ICON,
        IMAGE_VOLUME_SET_ICON,
        //IMAGE_ABOUT_PHONE_ICON,
        IMAGE_RESTORE_ICON,
        IMAGE_POWER_OFF_ICON,
        //IMAGE_RESTART_ICON,
        IMAGE_ABOUT_PHONE_ICON
#ifdef FOTA_SUPPORT
		,
        IMAGE_ABUP_FOTA_SET_ICON
#endif
        
    };


    GUILIST_ITEM_T       		item_t    =  {0};
    GUILIST_ITEM_DATA_T  		item_data = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint16 total_item_num = 0;

    #if 1//def NEW_LEFT_RIGHT_STYLE   
    const uint16 h_gap = (80-60)/2;//total item height 80
    const uint16 w_gap = 20;
    const uint16 img_height = 60;
    const uint16 img_width = 60;
    const uint16 height = img_height + 2*h_gap;
    GUI_RECT_T l_rect0 = { 0, 0,  MMI_MAINSCREEN_WIDTH -1,  height -1};//item bg
    GUI_RECT_T l_rect1 = { w_gap, h_gap,  img_width + w_gap,  img_height+h_gap};// icon bg
    GUI_RECT_T l_rect2 = { img_width+w_gap+6,  h_gap,  MMI_MAINSCREEN_WIDTH-2*h_gap , img_height+h_gap};//text
    //GUI_RECT_T r_rect0 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap, h_gap,   MMI_MAINSCREEN_WIDTH - w_gap,  img_height+h_gap};//bg
    //GUI_RECT_T r_rect1 = { MMI_MAINSCREEN_WIDTH - img_width- 2*w_gap+2,  h_gap,  MMI_MAINSCREEN_WIDTH - 2*w_gap , img_height+h_gap};//text
    
    
    //uint32 i = GUILIST_GetTotalItemNum(ctrl_id);
    THEMELIST_ITEM_STYLE_T* pItemStyle = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS);
    SCI_MEMCPY(&new_style, pItemStyle, sizeof(*pItemStyle));
   
    new_style.height = new_style.height_focus = height;
   
    
    new_style.content[0].rect = new_style.content[0].rect_focus = l_rect0;
    new_style.content[1].rect = new_style.content[1].rect_focus = l_rect1;
  
    new_style.content[2].rect = new_style.content[2].rect_focus = l_rect2;
    #if defined(ZDT_MMI_USE_SMALL_FONT)	
    new_style.content[2].font = new_style.content[2].font_focus = SONG_FONT_22;	
    #else	
    new_style.content[2].font = new_style.content[2].font_focus = SONG_FONT_28;
    #endif
  
    #endif

    
    
    item_t.item_data_ptr = &item_data; 
    item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;//GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;//GUIITEM_STYLE_ONE_LINE_TEXT; 
    #if 1//def NEW_LEFT_RIGHT_STYLE 
    item_t.item_style =  GUIITEM_STYLE_CUSTOMIZE;
    #endif



    
    if(0)//if(isLeft)
    {
        //item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS; 
        //item_t.left_item_style = GUIITEM_STYLE_ONE_LINE_TEXT; // no effect
        //item_data.item_content[0].font_color_id=MMITHEME_COLOR_9; // no effect
        
        item_t.left_item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS;
    }
    else
    {
        //item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS; 
        //item_t.right_item_style = GUIITEM_STYLE_ONE_LINE_TEXT;// no effect
        //item_data.item_content[0].font_color_id=MMITHEME_COLOR_BLUE;// no effect
        item_t.right_item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS;
    }
     //item_t.left_item_style = GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS;

     

   
    GUILIST_RemoveAllItems(MMIZDT_SETTING_LIST_CTRL_ID);

    for(i = 0 ;i < LIST_COUNT ; i++)
    {

        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[0].item_data.image_id = bg_img_id;
        
         item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[1].item_data.image_id = img_id_list[i];
        

        
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
        
        item_data.item_content[2].item_data.text_id = text_id_list[i];
       
        ret = GUILIST_AppendItem (MMIZDT_SETTING_LIST_CTRL_ID, &item_t);
        if(ret)
        {
            CTRLLIST_SetItemUserData(MMIZDT_SETTING_LIST_CTRL_ID, position, &i); 
            #if 1//def NEW_LEFT_RIGHT_STYLE 
            GUILIST_SetItemStyleEx(MMIZDT_SETTING_LIST_CTRL_ID, position, &new_style );
            #endif
            position++;   
        }
        
    }

    
}



LOCAL MMI_RESULT_E  HandleZDT_SettingWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIZDT_SETTING_LIST_CTRL_ID;
    BOOLEAN ret = FALSE;
    uint32 pos_user_data = 0; // position user data
    uint8 * p_index = PNULL;
    GUILIST_INIT_DATA_T list_init = {0}; 
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
            //MMIZDT_PB_ShowInit();
            list_init.both_rect.v_rect.left = 0;
            list_init.both_rect.v_rect.right = MMI_MAINSCREEN_WIDTH-1;
            list_init.both_rect.v_rect.top = 0;
            list_init.both_rect.v_rect.bottom = MMI_MAINSCREEN_HEIGHT-1;//240;//180;

            list_init.both_rect.h_rect.left = 0;
            list_init.both_rect.h_rect.right = MMI_MAINSCREEN_HEIGHT-1;//240;//180;
            list_init.both_rect.h_rect.top = 0;
            list_init.both_rect.h_rect.bottom = MMI_MAINSCREEN_WIDTH-1;
            
            list_init.type = GUILIST_TEXTLIST_E;
                        
            GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);  
            ret = GUILIST_SetMaxItem(ctrl_id, 10, FALSE);
            //GUILIST_SetRect(MMK_ConvertIdToHandle(ctrl_id), &rect);
            MMK_SetAtvCtrl(win_id,ctrl_id);

            MMIZDT_ShowSettingList(win_id);
        }
        break;
        
        case MSG_FULL_PAINT:
                //MMIZDT_PB_Show(win_id);
                //MMIZDT_ShowSettingList(win_id);
            break;
            
        case MSG_GET_FOCUS:
                //MMIZDT_PB_Show(win_id);
            break;
            
        case MSG_LOSE_FOCUS:
            break;
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        {
        //case MSG_KEYDOWN_WEB:
            uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &pos_user_data);// user_data stand position
            //MMIZDT_PB_Call_Start(pos_user_data+1);
            //SCI_TRACE_LOW("HandleZDT_TinyChatWinMsg MSG_CTL_MIDSK  index=%d, pos_user_data=%d" , index, pos_user_data);
            switch(index)
            {
                case 0:
                    MMIENVZDT_OpenMainMenuWindow();//ring
                    break;
                case 1:
                    MMIAPISET_ZdtDisplayTimeSettingWin();

                    break;
                case 2:
                    MMIAPISET_ZdtBrightnessWin();
                    break;
                case 3:
                    MMIAPISET_ZdtVolumeWin();
                    break;

                case 4:
                    MMIAPISET_OpenInputResetPwdWin(TRUE);
                    break;
                case 5:
                    MMIAPISET_ZdtPowerOffWin();
                    break;
				#if 0
                case 6:
                    MMIAPISET_ZdtPowerRestartWin();
                    break; 
				#endif
				case 6:
	                MMIAPISET_ZdtWatchInfoWin();
	                break;
                    
				case 7:
	                MMIAPISET_EnterFotaWin();
	                break;
                
                    
            }
        }

        break;
        
            
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            /*s_pb_tp_x = point.x;
            s_pb_tp_y = point.y;
            s_is_pb_tp_down = TRUE;
            */
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            /*if(point.x - s_pb_tp_x > 120 && z_abs(point.y - s_pb_tp_y) < 100)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }*/
        
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
                
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_WEB:
            //MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        break;
        

        
    case MSG_KEYDOWN_DOWN:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,1);
        break;
        
    case MSG_KEYDOWN_UP:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,0);
        break;

    case MSG_KEYDOWN_LEFT:

        break;

    case MSG_KEYDOWN_RIGHT:

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
       
        break;
    
    case MSG_CLOSE_WINDOW:
        if(p_index != PNULL)
        {
            SCI_FREE(p_index);
        }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}




//===============yangyu end, for new setting ui=================



