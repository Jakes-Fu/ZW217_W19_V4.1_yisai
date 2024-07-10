/****************************************************************************
** File Name:      mmisms_filewin.c                                         *
** Author:                                                                  *
** Date:           08/15/2012                                               *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe sms file UI related.       *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
**   DATE              NAME             DESCRIPTION                        *
** 08/15/2012        jixin.xu          Create for sms reconstruction
** 
****************************************************************************/
#include "mmi_app_sms_trc.h"
#if defined(MMI_SMS_EXPORT_SUPPORT)

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmisms_id.h"
#include "mmisms_filewin.h"
#include "guiform.h"
#include "mmifmm_export.h"
#include "mmisms_text.h"
#include "guilabel.h"
#include "guidropdownlist.h"
#include "mmisms_internal.h"
#include "mmidisplay_data.h"
#include "mmisms_image.h"
//#include "mmismsapp_wintab.h"
#include "mmipub.h"
#include "mmisms_order.h"
#include "mmisd_export.h"
#include "gui_ucs2b_converter.h"
#include "mmisms_read.h"
//#include "mmipb_text.h"
#include "mmimms_internal.h"
#include "mmisms_securitywin.h"
#include "mmisms_appcontrol.h"
#include "mmisms_commonui.h"

#ifdef MMI_PDA_SUPPORT
#include "guisetlist.h"
#endif
/**---------------------------------------------------------------------------*
**                         Macro Declaration    							  *
**---------------------------------------------------------------------------*/
#ifdef CHATMODE_ONLY_SUPPORT
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#define SMS_SETTING_EXPORT_DROPPAGE_NUM   4
#else
#define SMS_SETTING_EXPORT_DROPPAGE_NUM   3
#endif
#else
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#define SMS_SETTING_EXPORT_DROPPAGE_NUM   6
#else
#define SMS_SETTING_EXPORT_DROPPAGE_NUM   5
#endif
#endif
#define MMISMS_VERTICAL_SPACE 30
#define SMS_SETTING_EXPORT_LOC_DROPPAGE_NUM   2

/**---------------------------------------------------------------------------*
**                         TYPE AND CONSTANT    							  *
**---------------------------------------------------------------------------*/

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
    MMISMS_PDA_SMS_EXPORT_INIT,
    MMISMS_PDA_SMS_EXPORT_CONTENT,    
    MMISMS_PDA_SMS_EXPORT_PATH,   
    MMISMS_PDA_SMS_EXPORT_MAX    
}MMISMS_PDA_EXPORT_SETTING_TYPE_E;
#endif

typedef struct
{
    uint8 s_cur_box_selection;//可以去掉
    MMISMS_PDA_EXPORT_SETTING_TYPE_E export_setting_type;
    uint32 s_mmisms_export_total_num;
    wchar s_dsp_path_str[MMIFILE_FULL_PATH_MAX_LEN + 1];
    BOOLEAN g_is_password_ok;
} MMISMS_FILE_WIN_PARAM_T;

/**--------------------------------------------------------------------------*
**                          EXTERN DECLARE                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :sms export setting  win handle fun
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingExportWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
//  Description :sms export setting location win handle fun
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingExportLocWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 );

/*****************************************************************************/
//  Description :Set sms export setting dropdownlist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetExportDropDownList( void );



#ifdef MMI_PDA_SUPPORT
LOCAL void SetExportDropSetlist( void );
#endif
/*****************************************************************************/
//  Description :Set sms export setting location dropdownlist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetExportLocationDropDownList( MMI_WIN_ID_T win_id );

/*****************************************************************************/
// 	Description : set export location param
//	Global resource dependence : 
//  Author: fengming.huang
//	Note: 
/*****************************************************************************/
LOCAL void SetExportPath(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, MMISMS_FILE_WIN_PARAM_T *param_to_create_export_path_win);

/*****************************************************************************/
// 	Description : Export Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenExportPathCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
// 	Description : Export Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Export_OpenLabelCallBack(BOOLEAN is_success, FILEARRAY file_array, MMI_HANDLE_T ctrl_handle);

/*****************************************************************************/
//  Description : open the SMS export all query window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void OpenExportQueryWin(MMI_WIN_ID_T query_win_id);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : 
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void SetBoxSoftkeyDisplay(MMISMS_BOX_TYPE_E box_type, GUIFORM_CHILD_DISPLAY_E display_type);
#endif

#ifndef MMI_GUI_STYLE_TYPICAL  
LOCAL MMI_RESULT_E  HandleSMSExportListWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                           );
#endif
/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jixin.xu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ExportMainWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : to operate the export
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void MMISMS_OperateExportAll(MMISMS_FILE_WIN_PARAM_T * add_date);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
//Select Export Box

#ifndef MMI_GUI_STYLE_TYPICAL  
WINDOW_TABLE( MMISMS_SETTING_EXPORT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSettingExportWinMsg),    
    WIN_ID(MMISMS_SETTING_EXPORT_WIN_ID),
    WIN_TITLE(TXT_SMS_EXPORT),
    
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif
    #ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMM_EXPORT, TXT_NULL, STXT_CANCEL),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SETTING_EXPORT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISMS_EXPORT_LIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSMSExportListWinMsg),    
    WIN_ID(MMISMS_EXPORT_LIST_WIN_ID),
    WIN_TITLE(TXT_SMS_EXPORT),
#ifdef MMI_PDA_SUPPORT   
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SMS_SETTING_EXPORTLIST_CTRL_ID),
    END_WIN
};

#else
WINDOW_TABLE( MMISMS_SETTING_EXPORT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSettingExportWinMsg),    
    WIN_ID(MMISMS_SETTING_EXPORT_WIN_ID),
    WIN_TITLE(TXT_SMS_EXPORT),
    
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_SETTING_EXPORT_CTRL_ID),
    
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_SETTING_EXPORT_FORM_CTRL_ID,MMISMS_SETTING_EXPORT_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_SELECTEXPORTWIN_CTRL_ID,MMISMS_SETTING_EXPORT_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SETLIST_CTRL(TRUE, MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID, MMISMS_SETTING_EXPORT_FORM_CTRL_ID),
#else
    CHILD_DROPDOWNLIST_CTRL(TRUE,MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID,MMISMS_SETTING_EXPORT_FORM_CTRL_ID),
#endif
    END_WIN
};
#endif

//Select Export Location
WINDOW_TABLE( MMISMS_SETTING_EXPORT_LOC_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSettingExportLocWinMsg),    
    WIN_ID(MMISMS_SETTING_EXPORT_LOC_WIN_ID),
    WIN_TITLE(TXT_COMM_EXPORT),
    WIN_SOFTKEY(TXT_COMM_EXPORT, TXT_NULL, STXT_RETURN),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_SETTING_EXPORT_LOC_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_SETTING_EXPORT_LOC_FORM1_CTRL_ID,MMISMS_SETTING_EXPORT_LOC_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISMS_SETTING_EXPORT_SELLOC_CTRL_ID,MMISMS_SETTING_EXPORT_LOC_FORM1_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID,MMISMS_SETTING_EXPORT_LOC_FORM1_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_SETTING_EXPORT_LOC_FORM2_CTRL_ID,MMISMS_SETTING_EXPORT_LOC_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISMS_SETTING_EXPORT_SAVEPOS_CTRL_ID,MMISMS_SETTING_EXPORT_LOC_FORM2_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :security pwd window call back
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSecurityPWDResult(uint32 validate_result)	
{
	//MMISMS_BOX_TYPE_E export_box = MMISMS_BOX_NONE;
	uint32 export_num = 0;
    MMISMS_FILE_WIN_PARAM_T *param_to_create_export_main_win = PNULL;
    MMI_WIN_ID_T win_id = MMISMS_SETTING_EXPORT_WIN_ID;
    
    param_to_create_export_main_win = (MMISMS_FILE_WIN_PARAM_T *)MMK_GetWinUserData(win_id);

	
	if (validate_result)		//If check is ok
	{
		//export all sms 
		if(MMISMS_BOX_ALL == MMISMS_GetExportBoxType())
		{
			export_num = MMISMS_SetExportAllBoxMsgMarked(MMISMS_GetExportSecurityBoxFlag(),TRUE);
			MMISMS_SetExportTotalNum(export_num);
			if(0 == export_num)
			{
				MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
			}
			else
			{
#ifndef MMI_GUI_STYLE_TYPICAL
                param_to_create_export_main_win->g_is_password_ok = TRUE;
#else                            
                MMISMS_OpenSetExportPathWin();
#endif
			}
		}
		//export securtiy box sms
		else
		{
			export_num = MMISMS_SetExportBoxMsgMarked(MMISMS_GetExportBoxType(),TRUE);
			MMISMS_SetExportTotalNum(export_num);
			if(0 == export_num)
			{
				MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
			}
			else
			{
#ifndef MMI_GUI_STYLE_TYPICAL  
				param_to_create_export_main_win->g_is_password_ok = TRUE;
#else
				MMISMS_OpenSetExportPathWin();
#endif
			}
		}
	}
	else
	{
		//do nothing is ok
	}
	return TRUE;
	
}  //end of HandleSecurityPWDResult

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :add list item 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaExportItem1Line(MMI_CTRL_ID_T       ctrl_id,
                                      GUIITEM_STYLE_E    item_style,
                                      MMI_TEXT_ID_T       left_softkey_id,
                                      MMI_IMAGE_ID_T      icon_id,
                                      wchar*        wstr_ptr,
                                      uint16              wstr_len,
                                      uint16              pos,
                                      BOOLEAN             is_update
                                      )
{
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16   i                    = 0;
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if( 0 != icon_id )
    {
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = icon_id;
        i++;
    }
    
    item_data.item_content[i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[i].item_data.text_buffer.wstr_len = wstr_len;
    
    item_data.item_content[i].item_data.text_buffer.wstr_ptr = wstr_ptr;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}


LOCAL MMI_RESULT_E  HandleSMSExportListWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    uint16 index                                                     = 0;
    uint16 cur_index                                                 = 0;
    MMISMS_FILE_WIN_PARAM_T* add_data = 0;

    SMS_MMI_TEXT_ID_E  export_text[SMS_SETTING_EXPORT_DROPPAGE_NUM] = 
    {
#ifdef CHATMODE_ONLY_SUPPORT
        TXT_SMS_EXPORT_CHATBOX, 
#else
        TXT_SMS_EXPORT_INBOX, 
        TXT_SMS_EXPORT_SENTBOX,
        TXT_SMS_EXPORT_OUTBOX,
#endif
        TXT_SMS_EXPORT_DRAFTBOX,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        TXT_SMS_EXPORT_SECURITYBOX,
#endif
        TXT_SMS_EXPORT_ALLBOX};
    MMI_TEXT_ID_T  path_text[2]   = {TXT_EXPORT_LOC_DEFAULT, TXT_EXPORT_LOC_CUSTOM};
    MMI_CTRL_ID_T  ctrl_id = MMISMS_SMS_SETTING_EXPORTLIST_CTRL_ID;

    add_data = ((MMISMS_FILE_WIN_PARAM_T*)MMK_GetWinAddDataPtr(win_id));

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:       
        GUILIST_SetMaxItem(MMISMS_SMS_SETTING_EXPORTLIST_CTRL_ID, SMS_SETTING_EXPORT_DROPPAGE_NUM, FALSE);
        if(MMISMS_PDA_SMS_EXPORT_CONTENT == add_data->export_setting_type)
        {
            GUIWIN_SetTitleTextId(win_id,TXT_SMS_EXPORT_SECLECT_BOX,FALSE);
            for (index = 0; index < SMS_SETTING_EXPORT_DROPPAGE_NUM; index++)
            {                
                MMI_STRING_T text_str = {0};
                MMI_GetLabelTextByLang(export_text[index], &text_str);
                AppendPdaExportItem1Line(ctrl_id,
                GUIITEM_STYLE_ONE_LINE_RADIO,
                TXT_COMMON_OK,  0,  text_str.wstr_ptr, 
                text_str.wstr_len, (uint16)index, FALSE); 
            }
            cur_index = add_data->s_cur_box_selection;
        }
        else if(MMISMS_PDA_SMS_EXPORT_PATH == add_data->export_setting_type)
        {
            MMISMS_SETTING_PATH_TYPE method = MMISMS_SETTING_PATH_DEFAULT;
            GUIWIN_SetTitleTextId(win_id,TXT_EXPORT_SETTING_LOC,FALSE);
            for (index = 0; index< 2; index++)
            {                
                MMI_STRING_T text_str = {0};
                MMI_GetLabelTextByLang(path_text[index], &text_str); 
                AppendPdaExportItem1Line(ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO, TXT_COMMON_OK, 0, text_str.wstr_ptr, text_str.wstr_len, (uint16)index, FALSE);
            }
            MMISMS_ReadExportMethodType(&method);
            cur_index =  (MMISMS_SETTING_PATH_DEFAULT == method) ? 0 : 1;
        }

        GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);	
        MMK_SetAtvCtrl(win_id, ctrl_id);    
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            if(MMISMS_PDA_SMS_EXPORT_CONTENT == add_data->export_setting_type)
            {
	           add_data->s_cur_box_selection = cur_index;
            }
            else if(MMISMS_PDA_SMS_EXPORT_PATH == add_data->export_setting_type )
            {                        
                MMI_STRING_T   path_str  = {0};
                MMI_STRING_T   real_path_str = {0}; 
                //MMISMS_SETTING_PATH_TYPE method = MMISMS_SETTING_PATH_DEFAULT;
                wchar real_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
                real_path_str.wstr_ptr = real_path;
                real_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                
                path_str.wstr_ptr = add_data->s_dsp_path_str;
                path_str.wstr_len = MMIAPICOM_Wstrlen(add_data->s_dsp_path_str);
                if(0 != cur_index)
                {
                    MMISMS_GetRealPathText(&path_str, real_path_str.wstr_ptr, &real_path_str.wstr_len);
                    MMISMS_AppointExportSavePath(real_path_str.wstr_ptr, real_path_str.wstr_len, OpenExportPathCallBack);
                }
                MMISMS_WriteExportMethodType((MMISMS_SETTING_PATH_TYPE)(cur_index));
            }
            MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;
}

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaExportItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = text_str.wstr_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = text_str.wstr_len;
    }
    
    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}



LOCAL void  InitPdaSmsExportListCtrl(MMISMS_FILE_WIN_PARAM_T *param_to_create_export_main_win)
{
    MMI_CTRL_ID_T       ctrl_id = MMISMS_SETTING_EXPORT_CTRL_ID;
    MMISMS_SETTING_PATH_TYPE method = MMISMS_SETTING_PATH_DEFAULT;
    MMI_STRING_T text_str= {0};
    SMS_MMI_TEXT_ID_E  export_text[SMS_SETTING_EXPORT_DROPPAGE_NUM] = 
    {
#ifdef CHATMODE_ONLY_SUPPORT
        TXT_SMS_EXPORT_CHATBOX, 
#else
        TXT_SMS_EXPORT_INBOX, 
        TXT_SMS_EXPORT_SENTBOX,
        TXT_SMS_EXPORT_OUTBOX,
#endif
        TXT_SMS_EXPORT_DRAFTBOX,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        TXT_SMS_EXPORT_SECURITYBOX,
#endif        
        TXT_SMS_EXPORT_ALLBOX
    };
    MMI_STRING_T path_str = {0};
    MMI_STRING_T custom_path_str = {0};
    MMI_STRING_T dsp_path_str = {0};
    //uint16 cur_index = 0;
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar custom_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar dsp_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    BOOLEAN is_sd_dev_exist = FALSE;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    
    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_SMS_EXPORT_MAX, FALSE );           
    MMI_GetLabelTextByLang(export_text[param_to_create_export_main_win->s_cur_box_selection], &text_str);

    AppendPdaExportItem2Line(            
    ctrl_id,
    TXT_SMS_EXPORT_SECLECT_BOX,
    text_str,
    IMAGE_COMMON_RIGHT_ARROW,
    (uint32)MMISMS_PDA_SMS_EXPORT_CONTENT);

    path_str.wstr_ptr = path;
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    custom_path_str.wstr_ptr = custom_path;
    custom_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    dsp_path_str.wstr_ptr = dsp_path;
    dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;

    MMISMS_ReadExportMethodType(&method);

    if (MMISMS_SETTING_PATH_CUSTOM == method)
    {//for cr246240 当sd卡拔出时候，要设置成默认的路径
        SCI_MEMSET(custom_path, 0x0, sizeof(custom_path));
        custom_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMISMS_GetCustomizedExportPath(custom_path_str.wstr_ptr, &custom_path_str.wstr_len);
        if(custom_path_str.wstr_len > 0)
        {
            dev = MMIAPIFMM_GetDeviceTypeByPath(custom_path_str.wstr_ptr, MMIFILE_DEVICE_LEN);
            if (MMI_DEVICE_NUM > dev && MMIAPIFMM_GetDeviceTypeStatus(dev))
            {
                is_sd_dev_exist = TRUE;
            }
        }

         if (is_sd_dev_exist)
        {
            MMISMS_GetDisplayPathText(&custom_path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        }
        else
        {
            SCI_MEMSET(path, 0x0, sizeof(path));
            path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
            SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
            dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
            if(MMISMS_GetDefaultExportPath(path_str.wstr_ptr, &path_str.wstr_len))
            {
                MMISMS_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE); 
                return;
            }
        }      
    }
    else
    {        
        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        if(MMISMS_GetDefaultExportPath(path_str.wstr_ptr, &path_str.wstr_len))
        {
            MMISMS_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);    
        }
    }
#if 0
    SCI_MEMSET(s_dsp_path_str, 0, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    MMI_WSTRNCPY(s_dsp_path_str,
        MMIFILE_FULL_PATH_MAX_LEN,
        dsp_path_str.wstr_ptr,
        dsp_path_str.wstr_len,
        dsp_path_str.wstr_len);
#endif
    SCI_MEMSET(param_to_create_export_main_win->s_dsp_path_str, 0, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    MMI_WSTRNCPY(param_to_create_export_main_win->s_dsp_path_str,
        MMIFILE_FULL_PATH_MAX_LEN,
        dsp_path_str.wstr_ptr,
        dsp_path_str.wstr_len,
        dsp_path_str.wstr_len);
        
    AppendPdaExportItem2Line(            
    ctrl_id,
    TXT_EXPORT_SETTING_LOC,
    dsp_path_str,
    IMAGE_COMMON_RIGHT_ARROW,
    (uint32)MMISMS_PDA_SMS_EXPORT_PATH);
}

/*****************************************************************************/
//  Description :sms export setting  win handle fun
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingExportWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
#endif    
    MMISMS_BOX_TYPE_E export_box = MMISMS_BOX_NONE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SETTING_EXPORT_CTRL_ID;
    uint16 index = 0;
    //uint32 user_data = 0;
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMI_STRING_T dsp_path_str = {0};
    MMI_STRING_T path_str = {0};
    MMISMS_FILE_WIN_PARAM_T *param_to_create_export_main_win = PNULL;
    uint32 user_data = 0;
    
    param_to_create_export_main_win = (MMISMS_FILE_WIN_PARAM_T *)MMK_GetWinUserData(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        param_to_create_export_main_win->s_cur_box_selection = 0;
        InitPdaSmsExportListCtrl(param_to_create_export_main_win);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        if(param_to_create_export_main_win->g_is_password_ok)  //pass word ok
        {
            param_to_create_export_main_win->g_is_password_ok = FALSE;
            param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_GetExportTotalNum();
            if(0 == param_to_create_export_main_win->s_mmisms_export_total_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
            }
            else
            {
                path_str.wstr_ptr = path;
                path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                //GUILABEL_GetText(path_value_ctrl, &dsp_path_str);
                dsp_path_str.wstr_ptr = param_to_create_export_main_win->s_dsp_path_str;
                dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(param_to_create_export_main_win->s_dsp_path_str);
                MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
                MMISMS_WriteExportPath(path_str.wstr_ptr);
                
                if(MMISMS_BOX_NONE != MMISMS_GetExportBoxType())
                {
                    //param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_GetExportTotalNum();                                    
                    //no sms export
                    if(0 == param_to_create_export_main_win->s_mmisms_export_total_num)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                        break;
                    }
                    else
                    {
                        MMISMS_SetCurType(MMISMS_GetExportBoxType());
                    }
                }
                
                if(MMISMS_CreateExportMsgFile(param_to_create_export_main_win->s_dsp_path_str))
                {
                    MMISMS_OperateExportAll(param_to_create_export_main_win);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                }
                
            }
        }
        else
        {
            GUILIST_RemoveAllItems(ctrl_id);
            InitPdaSmsExportListCtrl(param_to_create_export_main_win);
        }
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
    #ifdef CHATMODE_ONLY_SUPPORT
            if(0 == param_to_create_export_main_win->s_cur_box_selection)
            {
                MMISMS_SetExportBoxType(MMISMS_BOX_CHAT);
            }
            else
            {
                MMISMS_SetExportBoxType(param_to_create_export_main_win->s_cur_box_selection + MMISMS_BOX_SENDFAIL);
            }
    #else
            MMISMS_SetExportBoxType(param_to_create_export_main_win->s_cur_box_selection + MMISMS_BOX_MT);
    #endif
            export_box = MMISMS_GetExportBoxType();
        
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                 //导出安全信箱信息需输入密码
                if(MMISMS_BOX_SECURITY == export_box)
                {                        
                    MMIAPISMS_OpenValidateSecruityBox(HandleSecurityPWDResult);
                }
                        //导出全部信息
                else if (MMISMS_BOX_ALL == export_box)
                {
                     //是否需要导出安全信箱信息
                    OpenExportQueryWin(MMISMS_QUERY_WIN_ID);
                }
                else
#endif
                {
                        if(MMISMS_BOX_ALL == export_box)
                        {
                            //s_mmisms_export_total_num = MMISMS_SetExportAllBoxMsgMarked(FALSE,TRUE);
                            param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_SetExportAllBoxMsgMarked(FALSE,TRUE);
                        }
                        else
                        {
                            //s_mmisms_export_total_num = MMISMS_SetExportBoxMsgMarked(export_box,TRUE);
                            param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_SetExportBoxMsgMarked(export_box,TRUE);
                        }    
                        
                        if(0 == param_to_create_export_main_win->s_mmisms_export_total_num)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                        }
                        else
                        {
                            path_str.wstr_ptr = path;
                            path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                            //GUILABEL_GetText(path_value_ctrl, &dsp_path_str);
                            dsp_path_str.wstr_ptr = param_to_create_export_main_win->s_dsp_path_str;
                            dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(param_to_create_export_main_win->s_dsp_path_str);
                            MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
                            MMISMS_WriteExportPath(path_str.wstr_ptr);
                            
                            if(MMISMS_BOX_NONE != MMISMS_GetExportBoxType())
                            {
                                //param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_GetExportTotalNum();                                    
                                //no sms export
                                if(0 == param_to_create_export_main_win->s_mmisms_export_total_num)
                                {
                                    MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                                    break;
                                }
                                else
                                {
                                    MMISMS_SetCurType(MMISMS_GetExportBoxType());
                                }
                            }
                            
                            if(MMISMS_CreateExportMsgFile(param_to_create_export_main_win->s_dsp_path_str))
                            {
                                MMISMS_OperateExportAll(param_to_create_export_main_win);
                            }
                            else
                            {
                                MMISMS_SetExportBoxMsgMarked(export_box,FALSE);
                                MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                            }        
                        }
                    }
            }
            break;    
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {                
            if (PNULL != param)
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                
                switch (src_id)
                {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    {
           #ifdef CHATMODE_ONLY_SUPPORT
                    if(0 == param_to_create_export_main_win->s_cur_box_selection)
                    {
                        MMISMS_SetExportBoxType(MMISMS_BOX_CHAT);
                    }
                    else
                    {
                        MMISMS_SetExportBoxType(param_to_create_export_main_win->s_cur_box_selection + MMISMS_BOX_SENDFAIL);
                    }
            #else
                     MMISMS_SetExportBoxType(param_to_create_export_main_win->s_cur_box_selection + MMISMS_BOX_MT);
            #endif
                        export_box = MMISMS_GetExportBoxType();

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                        //导出安全信箱信息需输入密码
                        if(MMISMS_BOX_SECURITY == export_box)
                        {                        
                            MMIAPISMS_OpenValidateSecruityBox(HandleSecurityPWDResult);
                        }
                        //导出全部信息
                        else if (MMISMS_BOX_ALL == export_box)
                        {
                            //是否需要导出安全信箱信息
                            OpenExportQueryWin(MMISMS_QUERY_WIN_ID);
                        }
                        else
#endif
                        {
                            if(MMISMS_BOX_ALL == export_box)
                            {
                                //s_mmisms_export_total_num = MMISMS_SetExportAllBoxMsgMarked(FALSE,TRUE);
                                param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_SetExportAllBoxMsgMarked(FALSE,TRUE);
                            }
                            else
                            {
                                //s_mmisms_export_total_num = MMISMS_SetExportBoxMsgMarked(export_box,TRUE);
                                param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_SetExportBoxMsgMarked(export_box,TRUE);
                            }    
                            
                            if(0 == param_to_create_export_main_win->s_mmisms_export_total_num)
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                            }
                            else
                            {
                                path_str.wstr_ptr = path;
                                path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                                //GUILABEL_GetText(path_value_ctrl, &dsp_path_str);
                                dsp_path_str.wstr_ptr = param_to_create_export_main_win->s_dsp_path_str;
                                dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(param_to_create_export_main_win->s_dsp_path_str);
                                MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
                                MMISMS_WriteExportPath(path_str.wstr_ptr);
                                
                                if(MMISMS_BOX_NONE != MMISMS_GetExportBoxType())
                                {
                                    //param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_GetExportTotalNum();                                    
                                    //no sms export
                                    if(0 == param_to_create_export_main_win->s_mmisms_export_total_num)
                                    {
                                        MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                                        break;
                                    }
                                    else
                                    {
                                        MMISMS_SetCurType(MMISMS_GetExportBoxType());
                                    }
                                }
                                
                                if(MMISMS_CreateExportMsgFile(param_to_create_export_main_win->s_dsp_path_str))
                                {
                                    MMISMS_OperateExportAll(param_to_create_export_main_win);
                                }
                                else
                                {
                                    MMISMS_SetExportBoxMsgMarked(export_box,FALSE);
                                    MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                                }        
                            }
                        }   
                    }
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_CloseWin(win_id);
                    break;            
                default:
                    index = GUILIST_GetCurItemIndex(ctrl_id);
                    GUILIST_GetItemData(ctrl_id, index, &user_data);
                    param_to_create_export_main_win->export_setting_type = (MMISMS_PDA_EXPORT_SETTING_TYPE_E)user_data;
                    
                    switch(param_to_create_export_main_win->export_setting_type)
                    {
                        case MMISMS_PDA_SMS_EXPORT_CONTENT:                
                        case MMISMS_PDA_SMS_EXPORT_PATH:
                            MMK_CreatePubListWin((uint32*)MMISMS_EXPORT_LIST_WIN_TAB, (ADD_DATA)param_to_create_export_main_win);
                            //MMK_SetWinUserData(MMI_HANDLE_T win_handle, void * data_ptr)
                        break;
                        default:
                        break;
                    }
                    break;
                }              
            } 
        }
        break;            
    
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MSG_PROMPTWIN_OK:
        //需要导出安全信箱信息，打开输入密码窗口
        MMISMS_SetExportSecurityBoxFlag(TRUE);      
        param_to_create_export_main_win->g_is_password_ok = FALSE;
        MMIAPISMS_OpenValidateSecruityBox(HandleSecurityPWDResult);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //不需要导出安全信箱信息
         param_to_create_export_main_win->g_is_password_ok = FALSE;
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
#endif

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        ExportMainWinCloseCtrlFunc(win_id);
    break;
    
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;
}


#else
/*****************************************************************************/
//  Description :sms export setting  win handle fun
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingExportWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    uint16 cur_selection       = 0;
    // uint16 save_pos_index                = 0;
    // uint16 ver_space = MMISMS_VERTICAL_SPACE;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_BOX_TYPE_E export_box = MMISMS_BOX_NONE;
    MMISMS_FILE_WIN_PARAM_T *param_to_create_export_main_win = PNULL;
    
    param_to_create_export_main_win = (MMISMS_FILE_WIN_PARAM_T *)MMK_GetWinUserData(win_id);

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMISMS_SETTING_EXPORT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        GUILABEL_SetTextById(MMISMS_SELECTEXPORTWIN_CTRL_ID, TXT_SMS_EXPORT_SECLECT_BOX,FALSE);
        #ifdef MMI_PDA_SUPPORT
        SetExportDropSetlist();
        #else
        SetExportDropDownList();
        #endif
        MMK_SetAtvCtrl(win_id, MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID);
        break;

#ifdef MMI_SMS_MINI_SUPPORT
    case MSG_CTL_MIDSK:   
    case MSG_APP_WEB:
        {            
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(SPRD_GUI_DROPDOWNLIST_ID == IGUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(ctrl_id)))
            {
                GUIDROPDOWNLIST_OpenList(win_id,ctrl_id);
            }    
        }
        break;
#endif
    #ifdef     MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            if (PNULL != param)
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                
                switch (src_id)
                {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    {
                        cur_selection = GUISETLIST_GetCurIndex(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID); 
                        
               #ifdef CHATMODE_ONLY_SUPPORT
                        if(0 == cur_selection)
                        {
                            MMISMS_SetExportBoxType(MMISMS_BOX_CHAT);
                        }
                        else
                        {
                            MMISMS_SetExportBoxType(s_cur_box_selection + MMISMS_BOX_SENDFAIL);
                        }
                #else
                        MMISMS_SetExportBoxType(cur_selection + MMISMS_BOX_MT);
                #endif
                        export_box = MMISMS_GetExportBoxType();
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                        //导出安全信箱信息需输入密码
                        if(MMISMS_BOX_SECURITY == export_box)
                        {                            
                            MMIAPISMS_OpenValidateSecruityBox(HandleSecurityPWDResult);
                        }
                        //导出全部信息
                        else if (MMISMS_BOX_ALL == export_box)
                        {
                            //是否需要导出安全信箱信息
                            OpenExportQueryWin(MMISMS_QUERY_WIN_ID);
                        }
                        else
#endif
                        {
                            if(MMISMS_BOX_ALL == export_box)
                            {
                                s_mmisms_export_total_num = MMISMS_SetExportAllBoxMsgMarked(FALSE,TRUE);
                            }
                            else
                            {
                                s_mmisms_export_total_num = MMISMS_SetExportBoxMsgMarked(export_box,TRUE);
                            }    
                        
                            if(0 == s_mmisms_export_total_num)
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                            }
                            else
                            {
                                MMK_CreateWin((uint32*)MMISMS_SETTING_EXPORT_LOC_WIN_TAB, PNULL);
                            }
                        }
                    }
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_CloseWin(win_id);
                    break;            
                default:
                    break;
                }
            }           
        }
        break;            
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
    #else
    case MSG_CTL_OK:
    case MSG_APP_OK:
        cur_selection = GUIDROPDOWNLIST_GetCurItemIndex(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID);
#ifdef CHATMODE_ONLY_SUPPORT
        if(0 == cur_selection)
        {
            MMISMS_SetExportBoxType(MMISMS_BOX_CHAT);
        }
        else
        {
            MMISMS_SetExportBoxType(s_cur_box_selection + MMISMS_BOX_SENDFAIL);
        }
#else
        MMISMS_SetExportBoxType(cur_selection + MMISMS_BOX_MT);
#endif
        export_box = MMISMS_GetExportBoxType();
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        //导出安全信箱信息需输入密码
        if(MMISMS_BOX_SECURITY == export_box)
        {        
             MMIAPISMS_OpenValidateSecruityBox(HandleSecurityPWDResult);
        }
        //导出全部信息
        else 
#endif
        if(MMISMS_BOX_ALL == export_box)
        {
            //是否需要导出安全信箱信息
            OpenExportQueryWin(MMISMS_QUERY_WIN_ID);
        }
        else
        {
            if(MMISMS_BOX_ALL == export_box)
            {
                s_mmisms_export_total_num = MMISMS_SetExportAllBoxMsgMarked(FALSE,TRUE);
            }
            else
            {
                s_mmisms_export_total_num = MMISMS_SetExportBoxMsgMarked(export_box,TRUE);
            }    
            
            if(0 == s_mmisms_export_total_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
            }
            else
            {
                MMK_CreateWin((uint32*)MMISMS_SETTING_EXPORT_LOC_WIN_TAB, PNULL);
            }
        }
        break;
    #endif
    
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MSG_PROMPTWIN_OK:
        //需要导出安全信箱信息，打开输入密码窗口
        MMISMS_SetExportSecurityBoxFlag(TRUE);        
        MMIAPISMS_OpenValidateSecruityBox(HandleSecurityPWDResult);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //不需要导出安全信箱信息
        MMISMS_SetExportSecurityBoxFlag(FALSE);
        s_mmisms_export_total_num = MMISMS_SetExportAllBoxMsgMarked(MMISMS_GetExportSecurityBoxFlag(),TRUE);
        if(0 == s_mmisms_export_total_num)
        {
            MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
        }
        else
        {
            MMK_CreateWin((uint32*)MMISMS_SETTING_EXPORT_LOC_WIN_TAB, PNULL);
        }
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
#endif

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        ExportMainWinCloseCtrlFunc(win_id);
    break;
    
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;
}
#endif
/*****************************************************************************/
//  Description :sms export setting location win handle fun
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingExportLocWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    uint16 cur_selection       = 0;
    MMI_HANDLE_T active_ctrl = MMK_GetActiveCtrlId(win_id); 
    MMI_STRING_T path_str = {0};
    MMI_STRING_T real_path_str = {0};
    wchar real_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
    MMI_HANDLE_T path_value_ctrl = MMISMS_SETTING_EXPORT_SAVEPOS_CTRL_ID;
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMI_STRING_T dsp_path_str = {0};
    MMI_CTRL_ID_T cur_ctrl_id = 0;
    MMISMS_FILE_WIN_PARAM_T *param_to_create_export_path_win = PNULL;
    
    param_to_create_export_path_win = (MMISMS_FILE_WIN_PARAM_T *)MMK_GetWinUserData(win_id);

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILABEL_SetTextById(MMISMS_SETTING_EXPORT_SELLOC_CTRL_ID, TXT_EXPORT_SETTING_LOC,FALSE);
        SetExportLocationDropDownList(win_id);
        SetExportPath(win_id,msg_id,param_to_create_export_path_win);
        MMK_SetAtvCtrl(win_id, MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID);
        break;
        
    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        SetExportPath(win_id,msg_id,param_to_create_export_path_win);
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //因为这个notify消息是在fullpaint后面来的，所以还是要刷新一次
        break;

    case MSG_FULL_PAINT:
        SetExportPath(win_id,msg_id,param_to_create_export_path_win);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
        //customer setting path
        if(path_value_ctrl == active_ctrl)
        {
            path_str.wstr_ptr = path;
            path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
            GUILABEL_GetText(path_value_ctrl, &dsp_path_str);
            MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
            MMISMS_WriteExportPath(path_str.wstr_ptr);
        }
        
        if(MMISMS_BOX_NONE != MMISMS_GetExportBoxType())
        {
            if(!MMK_IsOpenWin(MMISMS_SETTING_EXPORT_WIN_ID))
            {
                param_to_create_export_path_win->s_mmisms_export_total_num = MMISMS_GetExportTotalNum();
            }
            
            //no sms export
            if(0 == param_to_create_export_path_win->s_mmisms_export_total_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                break;
            }
            else
            {
                MMISMS_SetCurType(MMISMS_GetExportBoxType());
            }
        }
        
        if(MMISMS_CreateExportMsgFile(param_to_create_export_path_win->s_dsp_path_str))
        {
            MMISMS_OperateExportAll(param_to_create_export_path_win);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_ERROR);
        }
                
        cur_selection = GUIDROPDOWNLIST_GetCurItemIndex(MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID);
        MMISMS_WriteExportMethodType((MMISMS_SETTING_PATH_TYPE)cur_selection);
        break;
        
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        if(path_value_ctrl == active_ctrl)
        {
            real_path_str.wstr_ptr = real_path;
            real_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
            GUILABEL_GetText(active_ctrl, &path_str);
            MMISMS_GetRealPathText(&path_str, real_path_str.wstr_ptr, &real_path_str.wstr_len);
            MMISMS_AppointExportSavePath(real_path_str.wstr_ptr, real_path_str.wstr_len, OpenExportPathCallBack);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(!MMK_IsOpenWin(MMISMS_SETTING_EXPORT_WIN_ID))
        {
            //初始化变量
            MMISMS_SetExportBoxType(MMISMS_BOX_NONE);
            MMISMS_SetExportTotalNum(0);
            param_to_create_export_path_win->s_mmisms_export_total_num = 0;
            MMISMS_SetExportCurrentNum(0);
            MMISMS_SetExportCurBoxType(MMISMS_BOX_NONE);
            MMISMS_SetExportAllBoxMsgMarked(MMISMS_GetExportSecurityBoxFlag(),FALSE);
            MMISMS_SetMsgMarked(FALSE);
            MMISMS_SetExportSecurityBoxFlag(FALSE);
            MMISMS_FreeExportFilePtr();
            //去除标记
            cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
            GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
            ExportMainWinCloseCtrlFunc(win_id);
        }
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;
}

/*****************************************************************************/
//  Description :Set sms export setting dropdownlist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetExportDropDownList( void )
{
    uint8 index                                                             = 0;
    uint16 export_dropdownlist_index                                        = 0;
    MMI_STRING_T export_dropdownlist[SMS_SETTING_EXPORT_DROPPAGE_NUM] = {0};

    SMS_MMI_TEXT_ID_E  export_text[SMS_SETTING_EXPORT_DROPPAGE_NUM] = 
    {
#ifdef CHATMODE_ONLY_SUPPORT
        TXT_SMS_EXPORT_CHATBOX, 
#else
        TXT_SMS_EXPORT_INBOX, 
        TXT_SMS_EXPORT_SENTBOX,
        TXT_SMS_EXPORT_OUTBOX,
#endif
        TXT_SMS_EXPORT_DRAFTBOX,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        TXT_SMS_EXPORT_SECURITYBOX,
#endif        
        TXT_SMS_EXPORT_ALLBOX
    };

    for (index = 0; index < SMS_SETTING_EXPORT_DROPPAGE_NUM; index++)
    {
        MMI_GetLabelTextByLang(export_text[index], &export_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID, export_dropdownlist, SMS_SETTING_EXPORT_DROPPAGE_NUM);
    GUIDROPDOWNLIST_SetCurItemIndex(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID, export_dropdownlist_index);
}

#ifdef MMI_PDA_SUPPORT
LOCAL void SetExportDropSetlist( void )
{

   //uint16 index = 0;
    uint32 loop = 0;
    GUI_BG_T  bg_id = {0};

    SMS_MMI_TEXT_ID_E  text_id[SMS_SETTING_EXPORT_DROPPAGE_NUM] = 
    {
#ifdef CHATMODE_ONLY_SUPPORT
        TXT_SMS_EXPORT_CHATBOX, 
#else
        TXT_SMS_EXPORT_INBOX, 
        TXT_SMS_EXPORT_SENTBOX,
        TXT_SMS_EXPORT_OUTBOX,
#endif
        TXT_SMS_EXPORT_DRAFTBOX,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        TXT_SMS_EXPORT_SECURITYBOX,
#endif        
        TXT_SMS_EXPORT_ALLBOX
    };
    
    bg_id.img_id = IMAGE_THEME_BG;

    GUISETLIST_SetFontAlign(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID, ALIGN_LVMIDDLE);
    
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID, text_id[loop]);
    }
    
    GUISETLIST_SetBackGround(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID, &bg_id);
    GUISETLIST_SetCtrlState(MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
}
#endif

/*****************************************************************************/
//  Description :Set sms export setting location dropdownlist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetExportLocationDropDownList( MMI_WIN_ID_T win_id )
{
    uint8 index                                                             = 0;
    uint16 export_dropdownlist_index                                        = 0;
    MMI_STRING_T export_dropdownlist[SMS_SETTING_EXPORT_LOC_DROPPAGE_NUM] = {0};
    MMI_TEXT_ID_T  export_text[SMS_SETTING_EXPORT_LOC_DROPPAGE_NUM] = {TXT_EXPORT_LOC_DEFAULT, 
        TXT_EXPORT_LOC_CUSTOM};

    GUIFORM_SetStyle(MMISMS_SETTING_EXPORT_LOC_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    for (index = 0; index < SMS_SETTING_EXPORT_LOC_DROPPAGE_NUM; index++)
    {
        MMI_GetLabelTextByLang(export_text[index], &export_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID, export_dropdownlist, SMS_SETTING_EXPORT_LOC_DROPPAGE_NUM);
    GUIDROPDOWNLIST_SetCurItemIndex(MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID, export_dropdownlist_index);
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jixin.xu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ExportMainWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{

    MMISMS_FILE_WIN_PARAM_T *win_data_p = (MMISMS_FILE_WIN_PARAM_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : to open the window to set the export position
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterExportMainWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMI_HANDLE_T win_handle = 0;
    uint16 device_len = 0;
    wchar* device_name_ptr = PNULL;
    MMISMS_FILE_WIN_PARAM_T *param_to_create_export_main_win = PNULL;
    
    if (MMK_IsOpenWin(MMISMS_SETTING_EXPORT_WIN_ID))
    {
        MMK_CloseWin(MMISMS_SETTING_EXPORT_WIN_ID);
    }

    device_name_ptr = MMISMS_ExportFile_GetDeviceName(&device_len);
    if (PNULL == device_name_ptr)
    {
        MMIPUB_OpenAlertWarningWin( TXT_PLEASE_INSERT_SD);
        return result;
    }
    param_to_create_export_main_win = (MMISMS_FILE_WIN_PARAM_T *) SCI_ALLOC_APPZ(sizeof (MMISMS_FILE_WIN_PARAM_T));
    param_to_create_export_main_win->s_mmisms_export_total_num = MMISMS_GetExportTotalNum();
    win_handle = MMK_CreateWin((uint32 *)MMISMS_SETTING_EXPORT_WIN_TAB, PNULL);
    MMK_SetWinUserData(win_handle, (void*)param_to_create_export_main_win);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (param_to_create_export_main_win);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;
}
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to open the window to set the export path
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSetExportPathWin(void)
{
    MMK_CreateWin((uint32*)MMISMS_SETTING_EXPORT_LOC_WIN_TAB, PNULL);
}
#else
/*****************************************************************************/
//     Description : to open the window to set the export path
//    Global resource dependence : 
//  Author: jixin.xu
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSetExportPathWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMI_HANDLE_T win_handle = 0;
    MMISMS_FILE_WIN_PARAM_T *param_to_create_export_path_win = PNULL;
    
    if (MMK_IsOpenWin(MMISMS_SETTING_EXPORT_LOC_WIN_ID))
    {
        MMK_CloseWin(MMISMS_SETTING_EXPORT_LOC_WIN_ID);
    }

    param_to_create_export_path_win = (MMISMS_FILE_WIN_PARAM_T *) SCI_ALLOC_APPZ(sizeof (MMISMS_FILE_WIN_PARAM_T));
    param_to_create_export_path_win->s_mmisms_export_total_num = MMISMS_GetExportTotalNum();
    win_handle = MMK_CreateWin((uint32*)MMISMS_SETTING_EXPORT_LOC_WIN_TAB, PNULL);
    MMK_SetWinUserData(win_handle, (void*)param_to_create_export_path_win);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (param_to_create_export_path_win);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }
}
#endif

/*****************************************************************************/
//  Description : Appoint export save path text
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_AppointExportSavePath(wchar *path_ptr, uint32 path_len, MMISMS_OpenFileCallBack callback)
{
    FILEARRAY ret_array = PNULL;
    MMIFMM_FILTER_T filter = {0};
    MMIFMM_OPENWIN_RET_E ret = FMM_OPENWIN_RET_NULL;

    ret_array = MMIAPIFILEARRAY_Create();

    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

    ret = MMIAPIFMM_OpenFileWinByLayer(path_ptr,
                                    (uint16)path_len,
                                    &filter,
                                    FUNC_FIND_FOLDER,
                                    FALSE,
                                    NULL,
                                    ret_array,
                                    callback,
                                    FALSE);

    if (FMM_OPENWIN_SUCCESS != ret)
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
}

/*****************************************************************************/
//  Description : Get export text
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetDisplayPathText(
                                      MMI_STRING_T *path_value_str_ptr,
                                      wchar *path_name_ptr,
                                      uint16 *path_name_len_ptr)
{
    MMI_STRING_T     path_string = {0};
    MMIFILE_DEVICE_E dev         = MMI_DEVICE_NUM;
    
    if (PNULL == path_value_str_ptr || PNULL == path_name_ptr || PNULL == path_name_len_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetDisplayPathText path_value_str_ptr path_name_ptr path_name_len_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1449_112_2_18_2_52_41_55,(uint8*)"");
        return;
    }
   

    dev = MMIAPIFMM_GetDeviceTypeByPath(path_value_str_ptr->wstr_ptr, MMIFILE_DEVICE_LEN);
    if(MMI_DEVICE_NUM == dev)
    {
        return;
    }   
    
    MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(dev), &path_string);

    *path_name_len_ptr = (uint16)MIN(path_string.wstr_len, MMIFILE_FULL_PATH_MAX_LEN);
    
    MMI_WSTRNCPY(path_name_ptr,
        MMIFILE_FULL_PATH_MAX_LEN,
        path_string.wstr_ptr,
        path_string.wstr_len,
        *path_name_len_ptr);
    
    MMI_WSTRNCPY(path_name_ptr + *path_name_len_ptr,
        MMIFILE_FULL_PATH_MAX_LEN,
        path_value_str_ptr->wstr_ptr + 1,
        path_value_str_ptr->wstr_len - 1,
        MIN(path_value_str_ptr->wstr_len - 1, MMIFILE_FULL_PATH_MAX_LEN - *path_name_len_ptr));
    
    *path_name_len_ptr += MIN(path_value_str_ptr->wstr_len - 1, MMIFILE_FULL_PATH_MAX_LEN - *path_name_len_ptr);
}


/*****************************************************************************/
//  Description : get export path assigned by user
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_GetCustomizedExportPath(
                                              wchar      *full_path_ptr,     //out
                                              uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                              )
{
    if (PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetCustomizedExportPath full_path_ptr full_path_len_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1491_112_2_18_2_52_41_56,(uint8*)"");
        return;
    }    
    MMISMS_ReadExportPath(full_path_ptr);
    
    *full_path_len_ptr = MMIAPICOM_Wstrlen(full_path_ptr);
    
    //SCI_TRACE_LOW:"MMISMS_GetCustomizedExportPath len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1498_112_2_18_2_52_41_57,(uint8*)"d", *full_path_len_ptr);
    
    return;
}


/*****************************************************************************/
// 	Description : set export location param
//	Global resource dependence : 
//  Author: fengming.huang
//	Note: 
/*****************************************************************************/
LOCAL void SetExportPath(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, MMISMS_FILE_WIN_PARAM_T *param_to_create_export_path_win)
{
    MMI_HANDLE_T loc_value_ctrl = MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID;
    MMI_HANDLE_T path_list_ctrl = MMISMS_SETTING_EXPORT_SAVEPOS_CTRL_ID;
    MMI_STRING_T path_str = {0};
    MMI_STRING_T custom_path_str = {0};
    MMI_STRING_T dsp_path_str = {0};
    uint16 cur_index = 0;
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar custom_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar dsp_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMISMS_SETTING_PATH_TYPE method = MMISMS_SETTING_PATH_DEFAULT;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    BOOLEAN is_sd_dev_exist = FALSE;

    GUIFORM_SetStyle(MMISMS_SETTING_EXPORT_LOC_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    path_str.wstr_ptr = path;
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    custom_path_str.wstr_ptr = custom_path;
    custom_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    dsp_path_str.wstr_ptr = dsp_path;
    dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    if(MSG_OPEN_WINDOW == msg_id)
    {
        MMISMS_ReadExportMethodType(&method);
    }
    else
    {
        cur_index = GUIDROPDOWNLIST_GetCurItemIndex(loc_value_ctrl);
        method = (MMISMS_SETTING_PATH_TYPE)cur_index;
    }
    if (MMISMS_SETTING_PATH_CUSTOM == method)
    {//for cr246240 当sd卡拔出时候，要设置成默认的路径
        SCI_MEMSET(custom_path, 0x0, sizeof(custom_path));
        custom_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMISMS_GetCustomizedExportPath(custom_path_str.wstr_ptr, &custom_path_str.wstr_len);
        if(custom_path_str.wstr_len > 0)
        {
            dev = MMIAPIFMM_GetDeviceTypeByPath(custom_path_str.wstr_ptr, MMIFILE_DEVICE_LEN);
            if (MMI_DEVICE_NUM > dev && MMIAPIFMM_GetDeviceTypeStatus(dev))
            {
                is_sd_dev_exist = TRUE;
            }
        }
    }
    if (MMISMS_SETTING_PATH_CUSTOM == method)
    {
        //Set value
        GUILABEL_SetIcon(path_list_ctrl, IMAGE_SMS_FOLDER_ICON);
        GUIFORM_SetChildDisplay(MMISMS_SETTING_EXPORT_LOC_CTRL_ID, MMISMS_SETTING_EXPORT_LOC_FORM2_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMM_EXPORT, TXT_SPECIFY, STXT_RETURN, FALSE);

        if (is_sd_dev_exist)
        {
            MMISMS_GetDisplayPathText(&custom_path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
            GUILABEL_SetText(path_list_ctrl, &dsp_path_str, FALSE);
        }
        else
        {
            SCI_MEMSET(path, 0x0, sizeof(path));
            path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
            SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
            dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
            if(MMISMS_GetDefaultExportPath(path_str.wstr_ptr, &path_str.wstr_len))
            {
                MMISMS_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
                GUILABEL_SetText(path_list_ctrl, &dsp_path_str, FALSE);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE); 
                return;
            }
        }        
    }
    else
    {
        //set value
        GUILABEL_SetIcon(path_list_ctrl, IMAGE_SMS_FOLDER_ICON_DISABLED);
        GUIFORM_SetChildDisplay(MMISMS_SETTING_EXPORT_LOC_CTRL_ID, MMISMS_SETTING_EXPORT_LOC_FORM2_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMM_EXPORT, TXT_NULL, STXT_RETURN, FALSE);
        
        SCI_MEMSET(path, 0x0, sizeof(path));
        path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        SCI_MEMSET(dsp_path, 0x0, sizeof(dsp_path));
        dsp_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
        if(MMISMS_GetDefaultExportPath(path_str.wstr_ptr, &path_str.wstr_len))
        {
            MMISMS_GetDisplayPathText(&path_str, dsp_path_str.wstr_ptr, &dsp_path_str.wstr_len);
            GUILABEL_SetText(path_list_ctrl, &dsp_path_str, FALSE);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);    
            MMK_CloseWin(MMISMS_SETTING_EXPORT_LOC_WIN_ID);
            return;
        }
    }
    
#ifndef MMI_GUI_STYLE_TYPICAL
#if 0
    SCI_MEMSET(s_dsp_path_str, 0, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    MMI_WSTRNCPY(s_dsp_path_str,
        MMIFILE_FULL_PATH_MAX_LEN,
        dsp_path_str.wstr_ptr,
        dsp_path_str.wstr_len,
        dsp_path_str.wstr_len);
#endif
    SCI_MEMSET(param_to_create_export_path_win->s_dsp_path_str, 0, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    MMI_WSTRNCPY(param_to_create_export_path_win->s_dsp_path_str,
        MMIFILE_FULL_PATH_MAX_LEN,
        dsp_path_str.wstr_ptr,
        dsp_path_str.wstr_len,
        dsp_path_str.wstr_len);
#endif
    GUIDROPDOWNLIST_SetCurItemIndex(loc_value_ctrl,(uint16)method);
}

/*****************************************************************************/
// 	Description : Export Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenExportPathCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    Export_OpenLabelCallBack(is_success, file_array, MMISMS_SETTING_EXPORT_SAVEPOS_CTRL_ID);
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : Export Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Export_OpenLabelCallBack(BOOLEAN is_success, FILEARRAY file_array, MMI_HANDLE_T ctrl_handle)
{
    uint32 file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    MMI_STRING_T path_str = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    //SCI_TRACE_LOW:"Export_OpenLabelCallBack is_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1709_112_2_18_2_52_42_59,(uint8*)"d", is_success);
    
    if (is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        
        //SCI_TRACE_LOW:"Export_OpenLabelCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1715_112_2_18_2_52_42_60,(uint8*)"d", file_num);
        
        if (file_num > 0 && ( MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data)))
        {
            //read user selected dir
            
            path_str.wstr_len         = file_array_data.name_len;
            
            MMI_WSTRNCPY(path, MMIFILE_FULL_PATH_MAX_LEN, file_array_data.filename, file_array_data.name_len, file_array_data.name_len);
            path_str.wstr_ptr = path;
            if (PNULL != ctrl_handle)
            {
                MMISMS_WriteExportPath(path_str.wstr_ptr);                
            }
        }
    }
    
    //释放列表
    if (PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        file_array = PNULL;
    }
}
#else
/*****************************************************************************/
// 	Description : Export Open file Callback
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void Export_OpenLabelCallBack(BOOLEAN is_success, FILEARRAY file_array, MMI_HANDLE_T ctrl_handle)
{
    uint32 file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    MMI_STRING_T path_str = {0};
    MMI_STRING_T dsp_str = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar dsp_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    //SCI_TRACE_LOW:"Export_OpenLabelCallBack is_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1753_112_2_18_2_52_42_61,(uint8*)"d", is_success);
    
    if (is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        
        //SCI_TRACE_LOW:"Export_OpenLabelCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1759_112_2_18_2_52_42_62,(uint8*)"d", file_num);
        
        if (file_num > 0 && ( MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data)))
        {
            //read user selected dir
            
            path_str.wstr_len         = file_array_data.name_len;
            
            MMI_WSTRNCPY(path, MMIFILE_FULL_PATH_MAX_LEN, file_array_data.filename, file_array_data.name_len, file_array_data.name_len);
            path_str.wstr_ptr = path;
            if (PNULL != ctrl_handle)
            {
                dsp_str.wstr_ptr = dsp_path;
                dsp_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                MMISMS_GetDisplayPathText(&path_str, dsp_str.wstr_ptr, &dsp_str.wstr_len);
                GUILABEL_SetText(ctrl_handle, &dsp_str, FALSE);
            }
        }
    }
    
    //释放列表
    if (PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        file_array = PNULL;
    }
}
#endif
/*****************************************************************************/
//  Description : Get Real path text
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetRealPathText(MMI_STRING_T *path_value_str_ptr,
                                   wchar *path_name_ptr,
                                   uint16 *path_name_len_ptr)
{
    MMI_STRING_T path_str = {0};
    int32 i = 0;
    uint16* p = PNULL;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    wchar *temp_path_wstr = PNULL;
    wchar semicon_wst[] = {':', 0};
    
    if (PNULL == path_value_str_ptr || PNULL == path_name_ptr || 
        PNULL == path_name_len_ptr || PNULL == path_value_str_ptr->wstr_ptr)
    {
        return;
    }
     
    for(; dev<MMI_DEVICE_NUM; dev++)
    {
        MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(dev), &path_str);
        temp_path_wstr = (wchar *)SCI_ALLOCA((path_str.wstr_len + 2) * sizeof(wchar));
        if (PNULL == temp_path_wstr)
        {
            break;
        }
        SCI_MEMSET((char *)temp_path_wstr, 0 , (path_str.wstr_len + 2) * sizeof(wchar));
        MMIAPICOM_Wstrncpy(temp_path_wstr, path_str.wstr_ptr, path_str.wstr_len);
        MMIAPICOM_Wstrncpy(temp_path_wstr + path_str.wstr_len, semicon_wst, 1);
        if(0 == MMIAPICOM_Wstrncmp(path_value_str_ptr->wstr_ptr, temp_path_wstr, path_str.wstr_len + 1))
        {
			if(MMIAPIFMM_GetDevicePath(dev) != PNULL)
			{
                MMIAPICOM_Wstrncpy(path_name_ptr,MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)) ;
			}
            SCI_FREE(temp_path_wstr);
            temp_path_wstr = PNULL;
            break;
        }
        SCI_FREE(temp_path_wstr);
        temp_path_wstr = PNULL;
    }
    if(MMI_DEVICE_NUM == dev)
    {
        return;
    }
    
    *path_name_len_ptr = 1;
    
    p = path_value_str_ptr->wstr_ptr;
    while((0 != p[i])&&( ':' != p[i]))
    {
        i++;
    }
    
    MMI_WSTRNCPY(path_name_ptr + 1,
        MMIFILE_FULL_PATH_MAX_LEN - 1,
        path_value_str_ptr->wstr_ptr + i,
        path_value_str_ptr->wstr_len - i,
        MIN(path_value_str_ptr->wstr_len - i, MMIFILE_FULL_PATH_MAX_LEN - 1));
    
    *path_name_len_ptr += MIN(path_value_str_ptr->wstr_len - i, MMIFILE_FULL_PATH_MAX_LEN - 1);
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_CombineExportFile(
                                        MMI_STRING_T *path_ptr,
                                        MMI_STRING_T *name_ptr,
                                        wchar      *full_path_ptr,     //out
                                        uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                                        BOOLEAN is_combine_tmp_file
                                        )
{
    wchar dir_mark[] = {'\\', 0};
    uint16 dir_mark_len = 1;
    uint16 tmp_ext_len = 0;
    
    if (PNULL == path_ptr || PNULL == name_ptr || PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetDisplayPathText path_ptr name_ptr full_path_ptr full_path_len_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2216_112_2_18_2_52_43_69,(uint8*)"");
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"MMISMS_CombineExportFile name_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2221_112_2_18_2_52_43_70,(uint8*)"ddd", name_ptr->wstr_len, path_ptr->wstr_len,is_combine_tmp_file);
    
    if (is_combine_tmp_file)
    {
        tmp_ext_len = strlen(MMISMS_FILE_EXT);
    }
    
    if (((name_ptr->wstr_len + path_ptr->wstr_len + dir_mark_len + tmp_ext_len)) < MMIFILE_FULL_PATH_MAX_LEN)
    {
        //路径
        MMI_WSTRNCPY(full_path_ptr, 
            MMIFILE_FULL_PATH_MAX_LEN,
            path_ptr->wstr_ptr,
            path_ptr->wstr_len,
            path_ptr->wstr_len);
        
        *full_path_len_ptr = path_ptr->wstr_len;
        
        MMI_WSTRNCPY(full_path_ptr + *full_path_len_ptr, 
            MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
            dir_mark,
            dir_mark_len,
            dir_mark_len);
        
        *full_path_len_ptr += dir_mark_len;
        
        //文件名
        MMI_WSTRNCPY(full_path_ptr + *full_path_len_ptr, 
            MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
            name_ptr->wstr_ptr,
            name_ptr->wstr_len,
            name_ptr->wstr_len);
        
        *full_path_len_ptr += name_ptr->wstr_len;
        
        if (is_combine_tmp_file)
        {
            MMI_STRNTOWSTR(full_path_ptr + *full_path_len_ptr, 
                MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
                (uint8 *)MMISMS_FILE_EXT,
                tmp_ext_len,
                tmp_ext_len);
            
            *full_path_len_ptr += tmp_ext_len;
        }
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : open the SMS export all query window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void OpenExportQueryWin(MMI_WIN_ID_T query_win_id)
{
    MMI_STRING_T  prompt_str1 = {0};

    
    MMI_GetLabelTextByLang(TXT_SMS_EXPORT_ALL_QUERY, &prompt_str1); 
    MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL );      
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : 
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void SetBoxSoftkeyDisplay(MMISMS_BOX_TYPE_E box_type, GUIFORM_CHILD_DISPLAY_E display_type)
{
    MMI_CTRL_ID_T parent_ctrl_id = 0;

    switch (box_type)
    {
    case MMISMS_BOX_MT:
        parent_ctrl_id = MMISMS_MTBOX_FORM_CTRL_ID;
        break;

    case MMISMS_BOX_SENDSUCC:
        parent_ctrl_id = MMISMS_SENDSUCCBOX_FORM_CTRL_ID;
        break;

    case MMISMS_BOX_SENDFAIL:
        parent_ctrl_id = MMISMS_SENDFAILBOX_FORM_CTRL_ID;
        break;

    case MMISMS_BOX_NOSEND:
        parent_ctrl_id = MMISMS_DRAFTBOX_FORM_CTRL_ID;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        parent_ctrl_id = MMISMS_SECURITYBOX_FORM_CTRL_ID;
        break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT    
    case MMISMS_BOX_CHAT:
        parent_ctrl_id = MMISMS_CHATBOX_FORM_CTRL_ID;
        break;
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    case MMISMS_BT_BOX_MT:
        parent_ctrl_id = MMISMS_BT_MTBOX_FORM_CTRL_ID;
        break;
    case MMISMS_BT_BOX_SENDSUCC:
        parent_ctrl_id = MMISMS_BT_SENDSUCCBOX_FORM_CTRL_ID;
        break;
    case MMISMS_BT_BOX_SENDFAIL:
        parent_ctrl_id = MMISMS_BT_SENDFAILBOX_FORM_CTRL_ID;
        break;
    case MMISMS_BT_BOX_NOSEND:
        parent_ctrl_id = MMISMS_BT_DRAFTBOX_FORM_CTRL_ID;
        break;
#endif

    default:
        break;
    }

    GUIFORM_SetChildDisplay(parent_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, display_type);
}
#endif

/*****************************************************************************/
//  Description : to operate the export
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OperateExportAll(MMISMS_FILE_WIN_PARAM_T * add_date)
{
    MMI_STRING_T prompt_str = {0};
    MMI_WIN_ID_T    win_id = MMISMS_EXPORT_ANIM_WIN_ID;

#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif
    
    //MMISMS_SetExportAllFlag(TRUE);
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    MMIPUB_OpenProgressWinByTextId(TXT_PUB_SAVING, &win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, MMISMS_HandleExportWaitWinMsg);
    MMIPUB_SetWinAddData(win_id, (uint32)add_date);
    MMK_PostMsg(MMISMS_EXPORT_ANIM_WIN_ID, MMISMS_MSG_EXPORT_NEXT, PNULL,0);
}

/*****************************************************************************/
//  Description : to export all mared message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ExportAllMarkedMsg( 
                      MMISMS_BOX_TYPE_E    box_type
                      )
{
    //避免反复写彩信索引文件
    LOCAL BOOLEAN is_mms_exist = FALSE;
    uint16 msg_type=MMISMS_TYPE_MAX;

    if (MMISMS_FINISH_OPER == MMISMS_AppExportMsgBox(box_type,&msg_type,MMISMS_FOLDER_NORMAL)) //begin deleting sms
    {
        //finish the export
        if(is_mms_exist)
        {
            MMIAPIMMS_SaveListInfo();
            is_mms_exist=FALSE;
        }
        //MMISMS_OpenMsgBox(TXT_COMPLETE, IMAGE_PUBWIN_SUCCESS);
        MMIPUB_CloseWaitWin(MMISMS_EXPORT_ANIM_WIN_ID);
        MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
        MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
        MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
     }
    else if(msg_type==MMISMS_TYPE_MMS)
    {
        is_mms_exist=TRUE;
    }
}

/*****************************************************************************/
//  Description : to handle the message of export waiting window    
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_HandleExportWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
    MMIFILE_HANDLE file_handle = 0;    
    MMIPUB_INFO_T *pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    //uint16 total_num = MMISMS_GetExportTotalNum();
    uint16 total_num = 0;
    uint16 current_num = MMISMS_GetExportCurrentNum(); //实时获取当前导出的短信条数
    MMISMS_FILE_WIN_PARAM_T *user_date_ptr = PNULL;

    if(PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    user_date_ptr = (MMISMS_FILE_WIN_PARAM_T *)pubwin_info_ptr->user_data;
    total_num = user_date_ptr->s_mmisms_export_total_num;
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);	
        MMIPUB_SetProgressTotalSize(&win_id, total_num);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;

    case MMISMS_MSG_EXPORT_NEXT:    
        //////////////////////////////////////////////////////////////////////////
        // fixed by feng.xiao for NEWMS00180461
        MMIPUB_UpdateProgressBarEx( &win_id, current_num, TRUE );
//         MMIPUB_UpdateProgressBar(&win_id,current_num);
        MMIPUB_SetProgressTotalSize(&win_id, total_num);
        MMISMS_ExportAllMarkedMsg(MMISMS_GetExportBoxType());
        break;

    case MSG_CLOSE_WINDOW:
        //s_marked_sms_num = 0;
        if (!MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID) && MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
        {
            GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetBoxSoftkeyDisplay(MMISMS_GetCurBoxType(), GUIFORM_CHILD_DISP_HIDE);
#endif
            MMISMS_SetAllMsgMarked(MMISMS_GetCurBoxType(), FALSE);
        }
        //关闭文件
        file_handle = MMISMS_GetExportFileHandle();
        if(0 != file_handle)
        {
            MMIAPIFMM_CloseFile(file_handle);
        }
        MMISMS_SetExportBoxType(MMISMS_BOX_NONE);
        MMISMS_SetExportTotalNum(0);
        //user_date_ptr->s_mmisms_export_total_num = 0;
        MMISMS_SetExportCurrentNum(0);
        MMISMS_SetExportCurBoxType(MMISMS_BOX_NONE);
        MMISMS_SetExportAllBoxMsgMarked(MMISMS_GetExportSecurityBoxFlag(),FALSE);
        MMISMS_SetExportSecurityBoxFlag(FALSE);
        MMISMS_SetMsgMarked(FALSE);
        MMISMS_FreeExportFilePtr();
        MMK_CloseWin(MMISMS_SETTING_EXPORT_WIN_ID);
        MMK_CloseWin(MMISMS_SETTING_EXPORT_LOC_WIN_ID);
        
        MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
        MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
        MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
        MMK_CloseWin(MMISMS_MOBOX_WIN_ID);

        MMISMS_SetDelAllFlag(FALSE);
        //全部导出完毕，将未处理链表中的sms插入到相应的链表中
        MMISMS_InsertUnhandledSmsToOrder();
        MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

#endif //#if defined(MMI_SMS_EXPORT_SUPPORT)


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
