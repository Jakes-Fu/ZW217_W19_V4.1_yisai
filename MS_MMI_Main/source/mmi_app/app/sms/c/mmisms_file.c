/****************************************************************************
** File Name:      mmisms_file.c                                            *
** Author:                                                                  *
** Date:           06/12/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe sms file.                *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
**   DATE              NAME             DESCRIPTION                             *
** 06/12/2010     fengming.huang           Create
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
#include "mmisms_file.h"
#include "guiform.h"
#include "mmifmm_export.h"
#include "mmisms_text.h"
#include "guilabel.h"
#include "guidropdownlist.h"
#include "mmisms_internal.h"
#include "mmidisplay_data.h"
#include "mmisms_image.h"

#include "mmipub.h"
#include "mmisms_order.h"
#include "mmisd_export.h"
#include "gui_ucs2b_converter.h"
#include "mmisms_read.h"
#include "mmipb_text.h"
#include "mmimms_internal.h"

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

#define MMISMS_DEFAULT_EXPORT_DIR (g_mmisms_default_export_dir_str)
#define MMISMS_DEFAULT_EXPORT_DIR_LEN  3//in the unit of uint8
#define MMISMS_DIR  ""
#define MMISMS_DIR_LEN  0
#define MMISMS_FILE_TIME_LEN 40

#define MMISMS_LEFT_BRACKET  0x28  //"("
#define MMISMS_RIGHT_BRACKET 0x29  //")"

#define MMISMS_FILE_MIN_SIZE 50

/**---------------------------------------------------------------------------*
**                         TYPE AND CONSTANT    							  *
**---------------------------------------------------------------------------*/
const wchar g_mmisms_default_export_dir_str[] = { 'S', 'M', 'S', 0 };

/**--------------------------------------------------------------------------*
**                          EXTERN DECLARE                                *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;   // SMS的全局变量

                                          /**--------------------------------------------------------------------------*
                                          **                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMIFILE_HANDLE s_export_file_handle = 0;
LOCAL wchar *s_full_name_ptr = PNULL;  
LOCAL uint32 s_mmisms_export_total_num = 0;
LOCAL uint32 s_mmisms_export_current_num = 0;
LOCAL  BOOLEAN s_mmisms_export_securitybox = FALSE;
LOCAL  MMISMS_BOX_TYPE_E s_mmisms_cur_boxtype = MMISMS_BOX_NONE;
#ifndef MMI_GUI_STYLE_TYPICAL  
LOCAL uint8 s_cur_box_selection = 0;
LOCAL wchar s_dsp_path_str[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
PUBLIC BOOLEAN g_is_password_ok = FALSE;
#endif
LOCAL wchar s_sms_default_cr_lf[] = {0x0d, 0x0a, 0};

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
LOCAL void SetExportPath(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : get device name
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL wchar *ExportFile_GetDeviceName(uint16 *device_name_len);

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
//  Description : to create SMS file
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateExportMsgFile(void);

/*****************************************************************************/
//  Description : set export file content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetExportFileContent(MMI_STRING_T  *string_ptr);

/*****************************************************************************/
//  Description : Get transmitter display string
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileTransmitterDispString(
                                           MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                           MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                           MMI_STRING_T            *string_ptr            //OUT:
                                           );

/*****************************************************************************/
//  Description : write the SMS box title
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WriteExportFileBoxType(BOOLEAN is_box_head);

/*****************************************************************************/
//  Description : open the SMS export all query window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void OpenExportQueryWin(MMI_WIN_ID_T query_win_id);

#ifndef MMI_GUI_STYLE_TYPICAL  
LOCAL MMI_RESULT_E  HandleSMSExportListWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                           );
#endif

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
                g_is_password_ok = TRUE;
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
				g_is_password_ok = TRUE;
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
typedef enum
{
    MMISMS_PDA_SMS_EXPORT_INIT,
    MMISMS_PDA_SMS_EXPORT_CONTENT,    
    MMISMS_PDA_SMS_EXPORT_PATH,   
    MMISMS_PDA_SMS_EXPORT_MAX    
}MMISMS_PDA_EXPORT_SETTING_TYPE_E;

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
    MMISMS_PDA_EXPORT_SETTING_TYPE_E add_data = 0;

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

    add_data = ((uint32)MMK_GetWinAddDataPtr(win_id));

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:       
        GUILIST_SetMaxItem(MMISMS_SMS_SETTING_EXPORTLIST_CTRL_ID, SMS_SETTING_EXPORT_DROPPAGE_NUM, FALSE);
        if(MMISMS_PDA_SMS_EXPORT_CONTENT == add_data)
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
            cur_index = s_cur_box_selection;
        }
        else if(MMISMS_PDA_SMS_EXPORT_PATH == add_data)
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
            if(MMISMS_PDA_SMS_EXPORT_CONTENT == add_data)
            {
	           s_cur_box_selection = cur_index;
            }
            else if(MMISMS_PDA_SMS_EXPORT_PATH == add_data )
            {                        
                MMI_STRING_T   path_str  = {0};
                MMI_STRING_T   real_path_str = {0}; 
                //MMISMS_SETTING_PATH_TYPE method = MMISMS_SETTING_PATH_DEFAULT;
                wchar real_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
                real_path_str.wstr_ptr = real_path;
                real_path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                
                path_str.wstr_ptr = s_dsp_path_str;
                path_str.wstr_len = MMIAPICOM_Wstrlen(s_dsp_path_str);
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



LOCAL void  InitPdaSmsExportListCtrl(uint8 curr_index)
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
    MMI_GetLabelTextByLang(export_text[curr_index], &text_str);

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

    SCI_MEMSET(s_dsp_path_str, 0, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    MMI_WSTRNCPY(s_dsp_path_str,
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
    uint32 user_data = 0;
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMI_STRING_T dsp_path_str = {0};
    MMI_STRING_T path_str = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_cur_box_selection = 0;
        InitPdaSmsExportListCtrl(s_cur_box_selection);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        if(g_is_password_ok)  //pass word ok
        {
            g_is_password_ok = FALSE;
            if(0 == s_mmisms_export_total_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
            }
            else
            {
                path_str.wstr_ptr = path;
                path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                //GUILABEL_GetText(path_value_ctrl, &dsp_path_str);
                dsp_path_str.wstr_ptr = s_dsp_path_str;
                dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(s_dsp_path_str);
                MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
                MMISMS_WriteExportPath(path_str.wstr_ptr);
                
                if(MMISMS_BOX_NONE != MMISMS_GetExportBoxType())
                {
                    s_mmisms_export_total_num = MMISMS_GetExportTotalNum();                                    
                    //no sms export
                    if(0 == s_mmisms_export_total_num)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                        break;
                    }
                    else
                    {
                        MMISMS_SetCurType(MMISMS_GetExportBoxType());
                    }
                }
                
                if(CreateExportMsgFile())
                {
                    MMISMS_OperateExportAll();
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
            InitPdaSmsExportListCtrl(s_cur_box_selection);
        }
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
    #ifdef CHATMODE_ONLY_SUPPORT
            if(0 == s_cur_box_selection)
            {
                MMISMS_SetExportBoxType(MMISMS_BOX_CHAT);
            }
            else
            {
                MMISMS_SetExportBoxType(s_cur_box_selection + MMISMS_BOX_SENDFAIL);
            }
    #else
            MMISMS_SetExportBoxType(s_cur_box_selection + MMISMS_BOX_MT);
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
                            path_str.wstr_ptr = path;
                            path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                            //GUILABEL_GetText(path_value_ctrl, &dsp_path_str);
                            dsp_path_str.wstr_ptr = s_dsp_path_str;
                            dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(s_dsp_path_str);
                            MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
                            MMISMS_WriteExportPath(path_str.wstr_ptr);
                            
                            if(MMISMS_BOX_NONE != MMISMS_GetExportBoxType())
                            {
                                s_mmisms_export_total_num = MMISMS_GetExportTotalNum();                                    
                                //no sms export
                                if(0 == s_mmisms_export_total_num)
                                {
                                    MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                                    break;
                                }
                                else
                                {
                                    MMISMS_SetCurType(MMISMS_GetExportBoxType());
                                }
                            }
                            
                            if(CreateExportMsgFile())
                            {
                                MMISMS_OperateExportAll();
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
                    if(0 == s_cur_box_selection)
                    {
                        MMISMS_SetExportBoxType(MMISMS_BOX_CHAT);
                    }
                    else
                    {
                        MMISMS_SetExportBoxType(s_cur_box_selection + MMISMS_BOX_SENDFAIL);
                    }
            #else
                     MMISMS_SetExportBoxType(s_cur_box_selection + MMISMS_BOX_MT);
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
                                path_str.wstr_ptr = path;
                                path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
                                //GUILABEL_GetText(path_value_ctrl, &dsp_path_str);
                                dsp_path_str.wstr_ptr = s_dsp_path_str;
                                dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(s_dsp_path_str);
                                MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);
                                MMISMS_WriteExportPath(path_str.wstr_ptr);
                                
                                if(MMISMS_BOX_NONE != MMISMS_GetExportBoxType())
                                {
                                    s_mmisms_export_total_num = MMISMS_GetExportTotalNum();                                    
                                    //no sms export
                                    if(0 == s_mmisms_export_total_num)
                                    {
                                        MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                                        break;
                                    }
                                    else
                                    {
                                        MMISMS_SetCurType(MMISMS_GetExportBoxType());
                                    }
                                }
                                
                                if(CreateExportMsgFile())
                                {
                                    MMISMS_OperateExportAll();
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
                    switch((MMISMS_PDA_EXPORT_SETTING_TYPE_E)user_data)
                    {
                        case MMISMS_PDA_SMS_EXPORT_CONTENT:                
                        case MMISMS_PDA_SMS_EXPORT_PATH:
                            MMK_CreatePubListWin((uint32*)MMISMS_EXPORT_LIST_WIN_TAB, (ADD_DATA)user_data);            
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
        g_is_password_ok = FALSE;
        MMIAPISMS_OpenValidateSecruityBox(HandleSecurityPWDResult);
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //不需要导出安全信箱信息
         g_is_password_ok = FALSE;
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
#endif

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
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
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILABEL_SetTextById(MMISMS_SETTING_EXPORT_SELLOC_CTRL_ID, TXT_EXPORT_SETTING_LOC,FALSE);
        SetExportLocationDropDownList(win_id);
        SetExportPath(win_id,msg_id);
        MMK_SetAtvCtrl(win_id, MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID);
        break;
        
    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        SetExportPath(win_id,msg_id);
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //因为这个notify消息是在fullpaint后面来的，所以还是要刷新一次
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
                s_mmisms_export_total_num = MMISMS_GetExportTotalNum();
            }
            
            //no sms export
            if(0 == s_mmisms_export_total_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SMS_EXPORT);
                break;
            }
            else
            {
                MMISMS_SetCurType(MMISMS_GetExportBoxType());
            }
        }
        
        if(CreateExportMsgFile())
        {
            MMISMS_OperateExportAll();
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
            MMISMS_SetExportCurrentNum(0);
            MMISMS_SetExportCurBoxType(MMISMS_BOX_NONE);
            MMISMS_SetExportAllBoxMsgMarked(MMISMS_GetExportSecurityBoxFlag(),FALSE);
            MMISMS_SetMsgMarked(FALSE);
            MMISMS_SetExportSecurityBoxFlag(FALSE);
            MMISMS_FreeExportFilePtr();
            //去除标记
            cur_ctrl_id = MMISMS_GetCurActiveListCtrlId();
            GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
            MMISMS_SetMarkedMsgNum(0);
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
//     Description : to open the window to set the export position
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSetExportPosWin(void)
{
    uint16 device_len = 0;
    wchar* device_name_ptr = PNULL;

    device_name_ptr = ExportFile_GetDeviceName(&device_len);

    if (PNULL == device_name_ptr)
    {
        MMIPUB_OpenAlertWarningWin( TXT_PLEASE_INSERT_SD);
        return;
    }

    MMK_CreateWin((uint32*)MMISMS_SETTING_EXPORT_WIN_TAB, PNULL);
}

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
LOCAL void SetExportPath(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
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
    SCI_MEMSET(s_dsp_path_str, 0, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    MMI_WSTRNCPY(s_dsp_path_str,
        MMIFILE_FULL_PATH_MAX_LEN,
        dsp_path_str.wstr_ptr,
        dsp_path_str.wstr_len,
        dsp_path_str.wstr_len);    
#endif
    GUIDROPDOWNLIST_SetCurItemIndex(loc_value_ctrl,(uint16)method);
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDefaultExportPath(
                                           wchar      *full_path_ptr,     //out
                                           uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                           )
{
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    const wchar *storage_dir_ptr = PNULL;
    uint16 storage_dir_len = 0;
    BOOLEAN ret = TRUE;
    
    if (PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetDefaultExportPath full_path_ptr full_path_len_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1639_112_2_18_2_52_42_58,(uint8*)"");
        return FALSE;
    }    
    device_name_ptr = ExportFile_GetDeviceName(&device_name_len);
    
    if (PNULL == device_name_ptr || 0 == device_name_len)
    {
        return FALSE;
    }
    
    storage_dir_ptr = MMISMS_DEFAULT_EXPORT_DIR;
    storage_dir_len = MMISMS_DEFAULT_EXPORT_DIR_LEN;
    
    if (!MMIAPIFMM_CombineFullPath(device_name_ptr, device_name_len,
        (wchar*)MMISMS_DIR, MMISMS_DIR_LEN,
        storage_dir_ptr, storage_dir_len,
        full_path_ptr, full_path_len_ptr))
    {
        ret = FALSE;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : get device name
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL wchar *ExportFile_GetDeviceName(uint16 *device_name_len)
{
    MMIFILE_DEVICE_E file_type = MMI_DEVICE_SDCARD;
    
    *device_name_len = 0;
    
    file_type  = MMIAPIFMM_GetValidateDevice(file_type);

    if (MMI_DEVICE_NUM != file_type)
    {
        *device_name_len = MMIAPIFMM_GetDevicePathLen((file_type));
        return MMIAPIFMM_GetDevicePath((file_type));
    }
    else
    {
        return PNULL;
    }
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
    //MMI_STRING_T dsp_str = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    //wchar dsp_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    
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
//  Description : to export the SMS box called by window
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppExportMsgBox(
                                                MMISMS_BOX_TYPE_E    type,    //IN:
                                                uint16* msg_type,  //out
                                                MMISMS_FOLDER_TYPE_E folder_type
                                                )
{
    MMISMS_OPER_ERR_E ret_val = MMISMS_EMPTY_BOX;
    // BOOLEAN is_right = FALSE;
    MMISMS_ORDER_ID_T first_pos = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    int32 i = 0;
    BOOLEAN is_need_security = MMISMS_GetExportSecurityBoxFlag();
    
    g_mmisms_global.operation.cur_order_index = 0;
    SCI_MEMSET(g_mmisms_global.operation.cur_order_id_arr, 0, 
        sizeof(g_mmisms_global.operation.cur_order_id_arr)); 
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_AppExportMsgBox type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1875_112_2_18_2_52_42_63,(uint8*)"d", type);
    
    MMISMS_SetOperStatus(MMISMS_EXPORT_SMS);
    
    switch (type)        
    {
    case MMISMS_BOX_SENDSUCC://已发信箱
        first_pos = MMISMS_FindValidSecurityMoSMSForDelAll(type);
        break;
        
    case MMISMS_BOX_SENDFAIL://发件箱
        first_pos = MMISMS_FindValidSecurityMoSMSForDelAll(type);
        break;
        
    case MMISMS_BOX_NOSEND://草稿箱
        first_pos = MMISMS_FindValidSecurityMoSMSForDelAll(type);
        break;
        
    case MMISMS_BOX_MT://收件箱
        first_pos = MMISMS_FindValidSecurityMTSMSForDelAll();
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY://安全件箱
        first_pos = MMISMS_FindValidSecuritySMSForDelAll();
        break;        
#endif        
    case MMISMS_BOX_ALL:
        first_pos = MMISMS_FindValidSMSForExportAll(type,is_need_security);
        break;

#ifdef MMI_SMS_CHAT_SUPPORT    
    case MMISMS_BOX_CHAT:
        first_pos = MMISMS_FindValidChatSMSForDelAll();
        break;
#endif

    default:
        break;
    }
    
    // the index is valid
    if (PNULL != first_pos)
    {
        //设置当前order_id到全局变量数组
        if (first_pos->flag.is_concatenate_sms) //is long sms
        {
            cur_order_id = first_pos;
            while (cur_order_id != PNULL)
            {
                MMISMS_SetCurOrderId(i,cur_order_id); 
                cur_order_id  = cur_order_id->next_long_sms_ptr;
                i++;
            }
        }
        else //is normal sms
        {
            MMISMS_SetCurOrderId(0,first_pos); 
        } 
        
        first_pos->flag.is_marked = FALSE;// 找到后将该标记清除，避免反复找到同一条信息
        *msg_type = first_pos->flag.msg_type;
        //SCI_TRACE_LOW:"MMISMS_AppExportMsgBox: msg_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1936_112_2_18_2_52_43_64,(uint8*)"d",first_pos->flag.msg_type);
        switch (first_pos->flag.msg_type)
        {
        case MMISMS_TYPE_SMS:
            //清空变量
            MMISMS_ReadyReadSms();
            MNSMS_ReadSmsEx(
                (MN_DUAL_SYS_E)first_pos->flag.dual_sys, 
                (MN_SMS_STORAGE_E)first_pos->flag.storage, 
                first_pos->record_id 
                );            
            ret_val  = MMISMS_NO_ERR;
            break;
            
        default: 
            MMK_PostMsg(MMISMS_EXPORT_ANIM_WIN_ID, MMISMS_MSG_EXPORT_NEXT, PNULL,0);
            ret_val = MMISMS_NO_ERR;            
            break;            
        }        
    }
    else
    {
        //SCI_TRACE_LOW:"MMISMS: s_export_file_handle: The SmsBox is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_1958_112_2_18_2_52_43_65,(uint8*)"");
        ret_val = MMISMS_FINISH_OPER;
    }
    
    return (ret_val);
}
                                                
/*****************************************************************************/
//  Description : to create SMS file
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateExportMsgFile(void)
{
    uint32 access_mode = 0;
    uint16  full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T file_name_value_string = {0};
    uint8 time_len = 0;
    wchar file_name[MMISMS_FILE_TIME_LEN] = {0};
    SCI_TM_T file_time = {0};
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    uint8  str_str[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMI_STRING_T dsp_path_str = {0};
    MMI_STRING_T path_str = {0};
    //BOOLEAN is_enough_space = FALSE;
    //uint32 free_high_word = 0;
    //uint32 free_low_word = 0;
    //wchar*	device_name_ptr = PNULL;
    //uint16	device_name_len = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    
    //获取路径    
    path_str.wstr_ptr = path;
    path_str.wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    
    #ifndef MMI_GUI_STYLE_TYPICAL
    dsp_path_str.wstr_ptr = s_dsp_path_str;
    dsp_path_str.wstr_len = MMIAPICOM_Wstrlen(s_dsp_path_str);
    #else
    GUILABEL_GetText(MMISMS_SETTING_EXPORT_SAVEPOS_CTRL_ID, &dsp_path_str);
    #endif
    MMISMS_GetRealPathText(&dsp_path_str, path_str.wstr_ptr, &path_str.wstr_len);

    dev = MMIAPIFMM_GetDeviceTypeByPath(path_str.wstr_ptr, MMIFILE_DEVICE_LEN);
    if(MMI_DEVICE_NUM == dev)
    {
        return FALSE;
    }
    
    if(!MMIAPIFMM_IsDevEnoughSpace(MMISMS_FILE_MIN_SIZE, dev))
    {
        MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
        return FALSE;
    }
    else
    {
        //用系统时间作文件名
        TM_GetSysDate(&sys_date);
        TM_GetSysTime(&sys_time);  
        file_time.tm_year=sys_date.year;
        file_time.tm_mday=sys_date.mday;   
        file_time.tm_mon=sys_date.mon;
        
        file_time.tm_hour=sys_time.hour;
        file_time.tm_min=sys_time.min;
        file_time.tm_sec = sys_time.sec;
        
        //生成文件名   
        sprintf((char*)str_str, "%04d%02d%02d_%02d%02d%02d",
            file_time.tm_year,file_time.tm_mon,\
            file_time.tm_mday, file_time.tm_hour,\
            file_time.tm_min, file_time.tm_sec);
        
        time_len = SCI_STRLEN((char *)str_str);
        time_len = MIN(time_len,MMISMS_FILE_TIME_LEN);
        
        MMI_STRNTOWSTR(
            file_name,
            time_len,
            (uint8 *)str_str,
            time_len,
            time_len
            );
        
        file_name_value_string.wstr_ptr = file_name;
        file_name_value_string.wstr_len = MMIAPICOM_Wstrlen(file_name);
        if(PNULL == s_full_name_ptr)
        {
            s_full_name_ptr = SCI_ALLOCA((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
            if(PNULL == s_full_name_ptr)
            {
                MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                return FALSE;
            }
        }
        SCI_MEMSET(s_full_name_ptr,0x00,(MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
        
        //合成文件全路径
        if (MMISMS_CombineExportFile(
            &path_str,
            &file_name_value_string,
            s_full_name_ptr,
            &full_name_len,
            TRUE
            ))
        {
            s_export_file_handle = 0;
            access_mode = SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS | SFS_MODE_APPEND;/*lint !e655*/
            s_export_file_handle = MMIAPIFMM_CreateFile(s_full_name_ptr, access_mode, NULL, NULL);
            //SCI_TRACE_LOW:"CreateExportMsgFile s_export_file_handle is 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2069_112_2_18_2_52_43_66,(uint8*)"d",s_export_file_handle);
            if(0 != s_export_file_handle)
            {
                if(!WriteExportFileBoxType(TRUE))
                {
                    return FALSE;
                }
            }
            else
            {
                //创建文件失败
                MMIPUB_OpenAlertWarningWin( TXT_ERROR);
                return FALSE;            
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin( TXT_COM_FILE_NAME_TOO_LONG);
            return FALSE;
        } 
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : to export the SMS to file
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ExportMsgToFile(void)
{
    MMIFILE_HANDLE file_handle = 0;
    BOOLEAN ret = FALSE;
    MMI_TEXT_ID_T text_id = TXT_ERROR;
    MMI_IMAGE_ID_T  image_id = IMAGE_PUBWIN_SUCCESS;
    uint32 written = 0;
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    MMI_STRING_T content_str = {0};
    uint8 *  file_text = PNULL;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMISMS_BOX_TYPE_E       cur_box_type = MMISMS_BOX_NONE;
    
    SCI_MEMSET(&content_str, 0, sizeof (content_str));
    
    //get file content
    if(SetExportFileContent(&content_str))
    {
        file_text = (uint8 *)SCI_ALLOCA(content_str.wstr_len*3+1);
        if(PNULL == file_text)
        {
            if(PNULL != content_str.wstr_ptr)
            {
                SCI_FREE(content_str.wstr_ptr);
                content_str.wstr_ptr = PNULL;        
            }
            return FALSE;
        }
        SCI_MEMSET(file_text,0,(content_str.wstr_len*3+1));
        GUI_WstrToUTF8( file_text, content_str.wstr_len*3+1, content_str.wstr_ptr, content_str.wstr_len );
        
        file_handle = MMISMS_GetExportFileHandle();
        if (0 != file_handle)
        {
            //if export all msg,every box type should be added
            if(MMISMS_BOX_ALL == MMISMS_GetExportBoxType())
            {
                cur_order_id = MMISMS_GetCurOperationOrderId();
                cur_box_type = MMISMS_GetSmsStateBoxtype(cur_order_id);
                if(s_mmisms_cur_boxtype !=  cur_box_type)
                {
                    s_mmisms_cur_boxtype = cur_box_type;
                    if(!WriteExportFileBoxType(FALSE))
                    {
												SCI_FREE(content_str.wstr_ptr);
												content_str.wstr_ptr = PNULL; 
												
												SCI_FREE(file_text);
												file_text = PNULL; 
                        return FALSE;
                    }
                }
            }
            
            error = MMIAPIFMM_WriteFile(file_handle, file_text,strlen((char *)file_text), &written, PNULL);
            //SCI_TRACE_LOW:"MMISMS_ExportMsgToFile: error is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2153_112_2_18_2_52_43_67,(uint8*)"d",error);
            if (SFS_NO_ERROR == error)
            {
                s_mmisms_export_current_num++;
                //SCI_TRACE_LOW:"MMISMS_ExportMsgToFile: s_mmisms_export_current_num is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2157_112_2_18_2_52_43_68,(uint8*)"d",s_mmisms_export_current_num);
                ret = TRUE;
            }
            else if(SFS_ERROR_NO_SPACE == error)
            {
                MMIAPIFMM_CloseFile(file_handle);
                MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
            }
            else
            {
                //write file error
                text_id  = TXT_ERROR;
                image_id = IMAGE_PUBWIN_WARNING;
                MMIAPIFMM_CloseFile(file_handle);
                MMIAPIFMM_DeleteFile(s_full_name_ptr, PNULL);
                //show the result
                MMIPUB_OpenAlertWinByTextId(PNULL, text_id, TXT_NULL, image_id, PNULL,
                    PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        else
        {
            //open file error
            text_id  = TXT_ERROR;
            image_id = IMAGE_PUBWIN_WARNING;
            //show the result
            MMIPUB_OpenAlertWinByTextId(PNULL, text_id, TXT_NULL, image_id, PNULL,
                PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }   
    }

    SCI_FREE(content_str.wstr_ptr);
    content_str.wstr_ptr = PNULL;  
    
    SCI_FREE(file_text);
    file_text = PNULL;
    
    return ret;
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
//  Description : set export file handle
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportFileHandle(MMIFILE_HANDLE file_handle)
{
    s_export_file_handle = file_handle;
    //SCI_TRACE_LOW:"MMISMS_SetExportFileHandle: file_handle is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2282_112_2_18_2_52_43_71,(uint8*)"d",file_handle);
}

/*****************************************************************************/
//  Description : get export file handle
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMISMS_GetExportFileHandle(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportFileHandle: s_export_file_handle is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2291_112_2_18_2_52_43_72,(uint8*)"d",s_export_file_handle);
    return s_export_file_handle;
}

/*****************************************************************************/
//  Description : set export file content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetExportFileContent(MMI_STRING_T  *string_ptr)
{
    MMISMS_READ_MSG_T    *read_msg_ptr = &(g_mmisms_global.read_msg);
    MN_SMS_ALPHABET_TYPE_E  alphabet = MN_SMS_DEFAULT_ALPHABET;
    uint16 msg_len = 0;
    uint8 *msg_data_ptr = PNULL;
    // BOOLEAN find_result = FALSE;
    MMI_STRING_T num_name = {0};
    uint16 copy_offset = 0;
    uint16 convert_msg_len = 0;
    uint8 time_len = 0;
    wchar time[MMISMS_FILE_TIME_LEN] = {0};
    uint8 cr_len = MMIAPICOM_Wstrlen(s_sms_default_cr_lf);
    uint16 year = 0;
    uint8 year_len = 0;
    uint8 year_temp[6] = {0};

    if(PNULL == string_ptr)
    {
        return FALSE;
    }
        
    alphabet = read_msg_ptr->read_content.alphabet;
    msg_len = read_msg_ptr->read_content.length;
    msg_data_ptr = read_msg_ptr->read_content.data;
    
    num_name.wstr_ptr = SCI_ALLOCA((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar));
    if(PNULL == num_name.wstr_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET((uint8 *)num_name.wstr_ptr, 0, ((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar)));
    
    //号码显示
    //PB中未找到匹配项,只显示号码
    //PB中找到匹配项，显示 姓名(号码)
    
    GetFileTransmitterDispString(
        alphabet,
        &(read_msg_ptr->address),
        &num_name
        );
    
    //设置年份
    if(MMISMS_READ_MTSMS == g_mmisms_global.read_msg.read_type)
    {
        year = read_msg_ptr->time_stamp.year + MMISMS_MT_OFFSERT_YEAR;
    }
    else
    {
        year = read_msg_ptr->time_stamp.year + MMISMS_OFFSERT_YEAR;;
    }
 
    year_len = sprintf(
        (char *)year_temp, 
        "%.4d/", 
        (uint16)year
        );
    time_len = year_len;
    
    MMI_STRNTOWSTR(
        time,
        time_len,
        (uint8 *)year_temp,
        time_len,
        time_len
        );
    
    //设置月，日，时间    
    time_len += MMISMS_GetTimeDispString(&(read_msg_ptr->time_stamp), &time[time_len]);
    
    // the length of SMS content will be too large to allocate memory, so we just calculate the length.
    if (alphabet != MN_SMS_UCS2_ALPHABET)
    {
        convert_msg_len = msg_len;
    }
    else
    {
        convert_msg_len = msg_len/sizeof(wchar);
    }
    
    // in first line, display address
    // in second line, display time
    // and then display the SMS content.
    
    // in order to allocate memory for string, calculate the string length.
    
    if (0 < time_len)
    {
        string_ptr->wstr_len = cr_len + num_name.wstr_len + cr_len + time_len + cr_len + convert_msg_len + cr_len;
    }
    else
    {
        string_ptr->wstr_len = cr_len + num_name.wstr_len + cr_len + convert_msg_len + cr_len;
    }
    
    // allocate memory
    string_ptr->wstr_ptr = SCI_ALLOCA((string_ptr->wstr_len+1)*(sizeof(wchar)));
    if(PNULL == string_ptr->wstr_ptr)
    {
        SCI_FREE(num_name.wstr_ptr);
        num_name.wstr_ptr = PNULL;
        return FALSE;
    }
    SCI_MEMSET((uint8 *)string_ptr->wstr_ptr, 0x00, ((string_ptr->wstr_len+1)*sizeof(wchar)));
    
    copy_offset = 0;
    
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        s_sms_default_cr_lf,
        cr_len,
        cr_len
        );
    copy_offset += cr_len;
    
    // address
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        num_name.wstr_len,
        num_name.wstr_ptr,
        num_name.wstr_len,
        num_name.wstr_len
        );
    
    copy_offset += num_name.wstr_len;
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        s_sms_default_cr_lf,
        cr_len,
        cr_len
        );
    
    copy_offset += cr_len;
    
    if (0 < time_len)
    {
        // time
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            time_len,
            time,
            time_len,
            time_len
            );
        
        copy_offset += time_len;
        // CR
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            cr_len,
            s_sms_default_cr_lf,
            cr_len,
            cr_len
            );
        copy_offset += cr_len;
    }
    
    // message content
    if (convert_msg_len != msg_len)
    {
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        MMI_MEMCPY( (uint8 *)&(string_ptr->wstr_ptr[copy_offset]),
            convert_msg_len*sizeof(wchar),
            msg_data_ptr,
            convert_msg_len*sizeof(wchar),
            convert_msg_len*sizeof(wchar)
            );
#else
        GUI_UCS2L2UCS2B((uint8 *)&(string_ptr->wstr_ptr[copy_offset]),
            convert_msg_len * sizeof(wchar),
            msg_data_ptr,
            convert_msg_len * sizeof(wchar));
#endif
    }
    else
    {
        MMI_STRNTOWSTR(
            &(string_ptr->wstr_ptr[copy_offset]),
            convert_msg_len,
            (uint8 *)msg_data_ptr,
            convert_msg_len,
            convert_msg_len
            );
    }
    
    copy_offset += convert_msg_len; 
    
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        s_sms_default_cr_lf,
        cr_len,
        cr_len
        );
    copy_offset += cr_len;    
    
    //SCI_TRACE_LOW:"MMISMS: SetExportFileContent length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2501_112_2_18_2_52_44_73,(uint8*)"d", string_ptr->wstr_len);
    
    SCI_FREE(num_name.wstr_ptr);
    num_name.wstr_ptr = PNULL;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Get transmitter display string
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileTransmitterDispString(
                                           MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                           MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                           MMI_STRING_T            *string_ptr            //OUT:
                                           )
{
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 2] = {0};
    uint16 num_len = 0;
    MMI_STRING_T num_name = {0};
    BOOLEAN find_result = FALSE;
    
     if (PNULL == orginal_addr_ptr || PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:GetFileTransmitterDispString orginal_addr_ptr string_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2526_112_2_18_2_52_44_74,(uint8*)"");
        return FALSE;
    }
    //存在号码   
    if (orginal_addr_ptr->num_len > 0)
    {
        num_name.wstr_len  = 0;
        num_name.wstr_ptr = SCI_ALLOCAZ((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar));
        if(PNULL == num_name.wstr_ptr)
        {
            //SCI_TRACE_LOW:"GetFileTransmitterDispString num_name.wstr_ptr alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2536_112_2_18_2_52_44_75,(uint8*)"");
            return find_result;
        }
        //SCI_MEMSET(num_name.wstr_ptr,0x00,((MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar)));
        
        if(PNULL == num_name.wstr_ptr)
        {
            return FALSE;
        }
        //indicate the phonebook info of the address is not saved.
        //SCI_TRACE_LOW:"MMISMS: GetFileTransmitterDispString number_type = %d, number_plan = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2547_112_2_18_2_52_44_76,(uint8*)"dd",orginal_addr_ptr->number_type, orginal_addr_ptr->number_plan);
        
        num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(orginal_addr_ptr->number_type),
            (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
            orginal_addr_ptr->num_len),
            orginal_addr_ptr->party_num,
            tele_num,
            (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
            );
        
        find_result = MMISMS_GetNameByNumberFromPB(tele_num,&num_name, MMISMS_PBNAME_MAX_LEN);    
        
        //未找到PB匹配项，只显示号码        
        if ((!find_result) 
            || (0 == num_name.wstr_len))
        {
            MMI_STRNTOWSTR(num_name.wstr_ptr,
                MMISMS_PBNAME_MAX_LEN,
                (uint8 *)tele_num,
                MMISMS_PBNAME_MAX_LEN,
                strlen((char *)tele_num));
            
            num_name.wstr_len = strlen((char *)tele_num);
        }
        //找到PB匹配项，显示姓名(号码)
        else
        {
            //左括号
            num_name.wstr_ptr[num_name.wstr_len] = MMISMS_LEFT_BRACKET;
            num_name.wstr_len++;
            
            //号码            
            MMI_STRNTOWSTR( &num_name.wstr_ptr[num_name.wstr_len],
                MMISMS_PBNAME_MAX_LEN,
                (uint8 *)tele_num,
                MMISMS_PBNAME_MAX_LEN,
                strlen((char *)tele_num));    
            num_name.wstr_len += strlen((char *)tele_num);
            
            //右括号
            num_name.wstr_ptr[num_name.wstr_len] = MMISMS_RIGHT_BRACKET;
            num_name.wstr_len++;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_NO_NAME, &num_name);
    }
    
    string_ptr->wstr_len  = num_name.wstr_len;
    
    MMI_WSTRNCPY(
        string_ptr->wstr_ptr,
        (MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar),
        num_name.wstr_ptr,
        (MMISMS_PBNAME_MAX_LEN + MMISMS_PBNAME_MAX_LEN + 2 + 2 + 1) *sizeof(wchar),
        string_ptr->wstr_len
        );
    
    if((PNULL != num_name.wstr_ptr) && (orginal_addr_ptr->num_len > 0))
    {
        SCI_FREE(num_name.wstr_ptr);
        num_name.wstr_ptr = PNULL;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : write the SMS box title
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WriteExportFileBoxType(BOOLEAN is_box_head)
{
    MMI_STRING_T file_box_type = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    wchar *file_box_ptr = PNULL;
    uint8 cr_len = MMIAPICOM_Wstrlen(s_sms_default_cr_lf);    
    uint8 len = 0;
    uint8 *  file_box_text = PNULL;
    MMIFILE_HANDLE file_handle = PNULL;
    uint32 written = 0;
    BOOLEAN ret = FALSE;
    // MMI_IMAGE_ID_T  image_id = IMAGE_PUBWIN_SUCCESS;
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
    uint32 type_write_len = 0;
    uint8 edcode_type[] = {0xEF, 0xBB, 0xBF, 0};

    if(is_box_head)
    {
       box_type = MMISMS_GetExportBoxType();
    }
    else
    {
       box_type = MMISMS_GetExportCurBoxType();
    }

    //SCI_TRACE_LOW:"WriteExportFileBoxType box_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2645_112_2_18_2_52_44_77,(uint8*)"d",box_type);

    //未获得导出信箱位置
    if(MMISMS_BOX_NONE == box_type)
    {
       return FALSE;
    }
    //设置短信文件
    else
    {
       switch(box_type)
       {
       case MMISMS_BOX_MT:
           text_id = TXT_SMS_EXPORT_INBOX;
           break;
       
       case MMISMS_BOX_SENDFAIL:
           text_id = TXT_SMS_EXPORT_OUTBOX;
           break;
       
       case MMISMS_BOX_NOSEND:
           text_id = TXT_SMS_EXPORT_DRAFTBOX;
           break;    
       
       case MMISMS_BOX_SENDSUCC:
           text_id = TXT_SMS_EXPORT_SENTBOX;
           break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
       case MMISMS_BOX_SECURITY:
           text_id = TXT_SMS_EXPORT_SECURITYBOX;
           break;
#endif

       case MMISMS_BOX_ALL:
           text_id = TXT_SMS_EXPORT_ALLBOX;
           break;

#ifdef CHATMODE_ONLY_SUPPORT
        case MMISMS_BOX_CHAT:
            text_id = TXT_SMS_EXPORT_CHATBOX;
            break;
#endif

        default:
           break;
       }
       MMI_GetLabelTextByLang(text_id, &file_box_type);
   
       file_box_ptr = SCI_ALLOCA(sizeof(wchar) * (cr_len + file_box_type.wstr_len + cr_len + 1));
   
       if(PNULL == file_box_ptr)
       {
           return FALSE;
       }
       SCI_MEMSET(file_box_ptr,0x00,(sizeof(wchar) * (cr_len + file_box_type.wstr_len + cr_len + 1)));
   
       MMI_WSTRNCPY(
           file_box_ptr,
           cr_len,
           s_sms_default_cr_lf,
           cr_len,
           cr_len
           ); 
   
       len = file_box_type.wstr_len;
   
       MMI_WSTRNCPY(
           &file_box_ptr[cr_len],
           len,
           file_box_type.wstr_ptr,
           len,
           len
           ); 
   
       len = len + cr_len;
   
       MMI_WSTRNCPY(
           &file_box_ptr[len],
           len,
           s_sms_default_cr_lf,
           cr_len,
           cr_len
           ); 

       len = len + cr_len;
   
       file_box_text = (uint8 *)SCI_ALLOCA(len*3+1);

       if(PNULL == file_box_text)
       {
           SCI_FREE(file_box_ptr);
           file_box_ptr = PNULL;        
           return FALSE;
       }
       SCI_MEMSET(file_box_text,0,(len*3+1));

       GUI_WstrToUTF8( file_box_text, len*3+1, file_box_ptr, len );
   
       file_handle = MMISMS_GetExportFileHandle();
       if (PNULL != file_handle)
       {
           error = MMIAPIFMM_WriteFile(file_handle, edcode_type,
               strlen((char *)edcode_type), &type_write_len, PNULL);
            error = MMIAPIFMM_WriteFile(file_handle, file_box_text,
               strlen((char *)file_box_text), &written, PNULL);
            //SCI_TRACE_LOW:"WriteExportFileBoxType error is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2750_112_2_18_2_52_44_78,(uint8*)"d",error);
            if (SFS_NO_ERROR == error)
            {
               ret = TRUE;
            }
            else if(SFS_ERROR_NO_SPACE == error)
            {
                MMIAPIFMM_CloseFile(file_handle);
                MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
            }            
            else
            {
                //写文件失败
                MMIAPIFMM_CloseFile(file_handle);
                MMIAPIFMM_DeleteFile(s_full_name_ptr, PNULL);


                //显示结果
                MMIPUB_OpenAlertWarningWin( TXT_ERROR);
           }
       }
       else
       {
			//打开文件失败
            MMIPUB_OpenAlertWarningWin( TXT_ERROR);
       }
   
       SCI_FREE(file_box_ptr);
       file_box_ptr = PNULL;        
   
       SCI_FREE(file_box_text);
       file_box_text = PNULL;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : set export sms total num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportTotalNum(uint32 total_num)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportTotalNum: total_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2793_112_2_18_2_52_44_79,(uint8*)"d",total_num);
    s_mmisms_export_total_num = total_num;
}

/*****************************************************************************/
//  Description : get export sms total num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportTotalNum(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportTotalNum: s_mmisms_export_total_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2803_112_2_18_2_52_44_80,(uint8*)"d",s_mmisms_export_total_num);
    return s_mmisms_export_total_num;
}

/*****************************************************************************/
//  Description : set export sms current num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportCurrentNum(uint32 current_num)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportCurrentNum: current_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2813_112_2_18_2_52_44_81,(uint8*)"d",current_num);
    s_mmisms_export_current_num = current_num;
}

/*****************************************************************************/
//  Description : get export sms current num
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetExportCurrentNum(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportCurrentNum: s_mmisms_export_current_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2823_112_2_18_2_52_45_82,(uint8*)"d",s_mmisms_export_current_num);
    return s_mmisms_export_current_num;
}

/*****************************************************************************/
//  Description : set export sms current box type
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportCurBoxType(MMISMS_BOX_TYPE_E box_type)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportCurBoxType: box_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2833_112_2_18_2_52_45_83,(uint8*)"d",box_type);
    s_mmisms_cur_boxtype = box_type;
}

/*****************************************************************************/
//  Description : get export sms current box type
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_GetExportCurBoxType(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportCurBoxType: s_mmisms_cur_boxtype is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2843_112_2_18_2_52_45_84,(uint8*)"d",s_mmisms_cur_boxtype);
    return s_mmisms_cur_boxtype;
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

/*****************************************************************************/
//  Description : set export security sms flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetExportSecurityBoxFlag(BOOLEAN is_export)
{
    //SCI_TRACE_LOW:"MMISMS_SetExportSecurityBoxFlag: is_export is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2866_112_2_18_2_52_45_85,(uint8*)"d",is_export);
    s_mmisms_export_securitybox = is_export;
}

/*****************************************************************************/
//  Description : get export security sms flag
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetExportSecurityBoxFlag(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetExportSecurityBoxFlag: s_mmisms_export_securitybox is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2876_112_2_18_2_52_45_86,(uint8*)"d",s_mmisms_export_securitybox);
    return s_mmisms_export_securitybox;
}

/*****************************************************************************/
//  Description : free export file ptr
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_FreeExportFilePtr(void)
{
    if(PNULL != s_full_name_ptr)
    {
        SCI_FREE(s_full_name_ptr);
        s_full_name_ptr = PNULL;
        //SCI_TRACE_LOW:"MMISMS_FreeExportFilePtr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_FILE_2890_112_2_18_2_52_45_87,(uint8*)"");
    }
}
#endif //#if defined(MMI_SMS_EXPORT_SUPPORT)


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
