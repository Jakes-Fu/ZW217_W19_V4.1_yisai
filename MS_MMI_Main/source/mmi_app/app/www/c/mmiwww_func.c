/******************************************************************************
** File Name:      mmiwww_func.c                                              *
** Author:                                                                    *
** Date:           2009/06/26                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe www browser func             *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 06/2009        kun.yu           Create                                     *
******************************************************************************/
#define _MMIWWW_FUC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmiwww_internal.h"
#include "mmiwww_slim_peer.h"
#include "mmiwww_func.h"
#include "mmiwww_bookmark.h"
#include "mmi_nv.h"
#include "mmi_resource.h"
#include "mmk_timer.h" 
#include "mmiwww_netfront.h"
#include "gui_ucs2b_converter.h"
#include "mmiwww_image.h"
#include "mmiidle_export.h"
#include "mmiconnection_export.h"
#include "mmifmm_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mime_type.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#include "mmicc_export.h"
#include "mmimms_export.h"
#include "mmipub.h"
#include "mmi_default.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmi_resource.h"
#include "mmiwww_export.h"


#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIWWW_ZOOM_TIMER_PERIOD    500
#define MMIWWW_BGSOUND_INFINITE 0xFFFF

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL char * s_www_user_agent_ptr = PNULL;

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const MMIBROWSER_PROCESS_T g_netfront_proc = {
            MMIAPIWWW_InitModule,
            MMIAPIWWW_InitWWW, 
            MMIAPIWWW_UninitApp,
            MMIAPIWWW_Entry,
            MMIAPIWWW_Close,
            PNULL,
            MMIAPIWWW_GetVersion,
            PNULL,
            PNULL,
            PNULL,
            MMIWWW_SetNetSettingIndex,
            MMIAPIWWW_GetNetSettingIndex,
            MMIAPIWWW_IsRunning,
            MMIAPIWWW_IsActive,
            MMIWWW_ResetDefaultSettings,
            MMIWWW_CleanUserInfo,
            MMIAPIWWW_RegMimeFunc,
            PNULL,
            MMIAPIWWW_DeleteCompletedDownloadTask,
            MMIAPIWWW_CheckTimerDisable,
//begin add new
            MMIAPIWWW_SetDefaultBookmartk,
//end:add new
//begin:for AT TEST
            PNULL,
            PNULL,
            PNULL
//end:for AT TEST
            };

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : Upload Open file Callback
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void Upload_OpenFileCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
//  Description : update www net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_ConnectionCallback(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : handle message wether setup call
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWwwMakeCall(MMI_WIN_ID_T      win_id, 
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param);

/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DEFINITION                       *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : add one item to listbox for www mdu 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendOneListItem(
                                    MMI_CTRL_ID_T ctrl_id,
                                    MMI_STRING_T *text_str,
                                    MMI_TEXT_ID_T left_softkey_id,
                                    MMI_TEXT_ID_T center_softkey_id,
                                    MMI_TEXT_ID_T right_softkey_id
                                   )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = center_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = text_str->wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_str->wstr_ptr;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : add item to listbox for www mdu 
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendListItem(
                                  MMI_WIN_ID_T win_id, 
                                  MMI_CTRL_ID_T ctrl_id, 
                                  MMI_TEXT_ID_T *append_arr,
                                  uint16 max_item, 
                                  uint16 cur_item 
                                  )
{
    int32 i = 0;
    MMI_STRING_T text_str = {0};

    for( i = 0; i < max_item; i++ )
    {
        MMIRES_GetText( *( append_arr + i ), win_id, &text_str );
        MMIWWW_AppendOneListItem(ctrl_id, &text_str, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    }

    //set selected item
    GUILIST_SetSelectedItem( ctrl_id, cur_item, TRUE );
    
    //set current item
    GUILIST_SetCurItemIndex( ctrl_id, cur_item );
    
}

/*****************************************************************************/
//  Description : add dynamic item to listbox when item_num is not zero
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendListboxitem( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 item_num, MMI_TEXT_ID_T text1_id, MMI_TEXT_ID_T text2_id, MMI_TEXT_ID_T text3_id )
{
    uint16 i=0;
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);

    if( item_num > 0)
    {
        uint16 cur_index = 0;
        uint16 top_index = 0;

        cur_index = GUILIST_GetCurItemIndex(ctrl_handle);
        top_index = GUILIST_GetTopItemIndex(ctrl_handle);
        if (GUILIST_GetTotalItemNum(ctrl_handle) > 0)
        {
            GUILIST_RemoveAllItems(ctrl_handle);
        }
        GUILIST_SetMaxItem( ctrl_handle, item_num, TRUE );

        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
        item_t.item_data_ptr = &item_data;

        for( i = 0; i < item_num; i++ )
        {
            GUILIST_AppendItem( ctrl_handle, &item_t );
        }
        GUILIST_SetTopItemIndex(ctrl_handle, top_index);
        GUILIST_SetCurItemIndex(ctrl_handle, cur_index);
    }
    else
    {
        GUILIST_SetMaxItem( ctrl_handle, 1, FALSE );

        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text1_id;

        item_data.softkey_id[0] = text2_id;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = text3_id;

        GUILIST_AppendItem( ctrl_handle, &item_t );  
    }
}

/*****************************************************************************/
//  Description : add history title item to listbox for www mdu 
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_AppendTitleItemData( MMI_CTRL_ID_T ctrl_id, uint16 index,  MMI_STRING_T string, MMI_IMAGE_ID_T image_id, MMI_TEXT_ID_T text1_id, MMI_TEXT_ID_T text2_id, MMI_TEXT_ID_T text3_id )
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 str_len = string.wstr_len;

    item_t.item_style                        = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr                     = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.softkey_id[0] = text1_id;
    item_data.softkey_id[1] = text2_id;
    item_data.softkey_id[2] = text3_id;
    item_data.item_content[0].item_data.image_id = image_id;

    item_data.item_content[1].item_data.text_buffer.wstr_len = str_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = string.wstr_ptr;//SCI_ALLOCA((str_len + 1) * sizeof(wchar));
    GUILIST_SetItemData( ctrl_id, &item_data, index );
//     if (PNULL != item_data.item_content[1].item_data.text_buffer.wstr_ptr)
//     {
//         MMI_WSTRNCPY(item_data.item_content[1].item_data.text_buffer.wstr_ptr,
//             item_data.item_content[1].item_data.text_buffer.wstr_len,
//             string.wstr_ptr,
//             item_data.item_content[1].item_data.text_buffer.wstr_len,
//             item_data.item_content[1].item_data.text_buffer.wstr_len);
// 
//         GUILIST_SetItemData( ctrl_id, &item_data, index );
//         SCI_FREE(item_data.item_content[1].item_data.text_buffer.wstr_ptr);
//     }
} 

/*****************************************************************************/
// Description : Start zoom operation timer
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_StartZoomTimer(MMI_HANDLE_T  win_handle, BOOLEAN is_zoomin)
{
    MMIWWW_PAGEINFO_T    *pageinfo_ptr = PNULL;

    pageinfo_ptr = (MMIWWW_PAGEINFO_T *)MMK_GetWinAddDataPtr(win_handle);

    if (PNULL == pageinfo_ptr)
    {
        return;
    }

    pageinfo_ptr->timer_id = MMK_CreateTimer(MMIWWW_ZOOM_TIMER_PERIOD, FALSE);
    if (is_zoomin)
    {
        pageinfo_ptr->zoom_mode = MMIWWW_ZOOMIN;
    }
    else
    {
        pageinfo_ptr->zoom_mode = MMIWWW_ZOOMOUT;
    }
    return;
}

/*****************************************************************************/
// Description : Stop zoom operation timer
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_StopZoomTimer(MMI_HANDLE_T  win_handle)
{
    MMIWWW_PAGEINFO_T    *pageinfo_ptr = PNULL;

    pageinfo_ptr = (MMIWWW_PAGEINFO_T *)MMK_GetWinAddDataPtr(win_handle);

    if (PNULL == pageinfo_ptr)
    {
        return;
    }

    if (pageinfo_ptr->timer_id > 0)
    {
        MMK_StopTimer(pageinfo_ptr->timer_id);
        pageinfo_ptr->timer_id = 0;
    }
    return;
}

/*****************************************************************************/
// Description : Get Match key num
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetMatchKeyNum(wchar *key_name_ptr)
{
    uint16 i; 
    uint16 match_num = 0;
    MMIWWW_KEY_T key = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    wchar *temp_input_key = NULL;
    uint16 len = 0;
    
    if((NULL == key_name_ptr )||( 0 == (len = MMIAPICOM_Wstrlen(key_name_ptr ))))
    {
         return 0;
    }

    temp_input_key = SCI_ALLOCA(sizeof(wchar) * (len + 1));  
    if(NULL == temp_input_key)
    {
       return 0;
    }
    SCI_MEMSET(temp_input_key, 0, sizeof(wchar) *  (len + 1));
    MMIAPICOM_Wstrncpy(temp_input_key, key_name_ptr, len);
    MMIAPICOM_Wstrlower(temp_input_key);
    
    MMINV_READ( MMINV_WWW_KEY, &key, return_value );

    for( i = 0; i < key.num; i++ )
    {
        MMIAPICOM_Wstrlower(key.key_arr[i].wstr);
        if(!MMIAPICOM_Wstrncmp( key.key_arr[i].wstr, temp_input_key, MMIAPICOM_Wstrlen(temp_input_key)))
        {            
            match_num++;
        }
    }
    
    SCI_FREE(temp_input_key);
    return match_num;
}

/*****************************************************************************/
// Description : List match key name
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ListMatchKeyName( wchar *key_name_ptr, MMI_HANDLE_T ctr_handle )
{
    uint16 i; 
    uint16 match_num = 0;
    MMIWWW_KEY_T key = {0};
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    wchar *temp_input_key = NULL;
    uint16 len = 0;
    
    if((NULL == key_name_ptr )||( 0 == (len = MMIAPICOM_Wstrlen(key_name_ptr ))))
    {
         return 0;
    }
    
    temp_input_key = SCI_ALLOCA(sizeof(wchar) * (len + 1));  
    if(NULL == temp_input_key)
    {
       return 0;
    }
    
    SCI_MEMSET(temp_input_key, 0, sizeof(wchar) *  (len + 1));
    MMIAPICOM_Wstrncpy(temp_input_key, key_name_ptr, len);
    MMIAPICOM_Wstrlower(temp_input_key);
    
    MMINV_READ( MMINV_WWW_KEY, &key, return_value );

    GUILIST_SetMaxItem( ctr_handle, MMIWWW_KEYNAME_NUM, FALSE );

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    for( i = 0; i < key.num; i++ )
    {
        wchar *temp_key_ptr = NULL; 
        uint key_len = key.key_arr[i].wstr_len;        
        temp_key_ptr = SCI_ALLOCA(sizeof(wchar) * (key_len + 1));
        
        if(NULL == temp_key_ptr)
        {
           break;
        }
        SCI_MEMSET(temp_key_ptr, 0, sizeof(wchar) *  (key_len + 1));
        
        MMIAPICOM_Wstrncpy(temp_key_ptr, key.key_arr[i].wstr,  key_len);       
        MMIAPICOM_Wstrlower(temp_key_ptr);  
        
        if(!MMIAPICOM_Wstrncmp( temp_key_ptr, temp_input_key, MMIAPICOM_Wstrlen(temp_input_key)))
        {            
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
            
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = key.key_arr[i].wstr_len;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = key.key_arr[i].wstr;
            
            GUILIST_AppendItem(ctr_handle, &item_t);
            match_num++;
        }
        SCI_FREE(temp_key_ptr);
    }
    
    SCI_FREE(temp_input_key);
    return match_num;
}

/*****************************************************************************/
// Description : Get Match key num
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetMatchURLNum(wchar *url_ptr)
{
    uint16 i; 
    uint16 match_num = 0;
    uint16 InputURL_num = (uint16)MMIWWW_Netfront_GetInputURLHistoryNumber();
    MMI_STRING_T url_str_info = {0};

    wchar *temp_input_url = NULL;

    uint16 len = 0;
    if((NULL == url_ptr )||( 0 == (len = MMIAPICOM_Wstrlen(url_ptr ))))
    {
         return 0;
    }

    temp_input_url = SCI_ALLOCA(sizeof(wchar) * (len + 1));  
    if(NULL == temp_input_url)
    {
       return 0;
    }
    
    SCI_MEMSET(temp_input_url, 0, sizeof(wchar) *  (len + 1));
    MMIAPICOM_Wstrncpy(temp_input_url, url_ptr, len);
    MMIAPICOM_Wstrlower(temp_input_url);
   
    for( i = 0; i < InputURL_num; i++ )
    {
        SCI_MEMSET(&url_str_info, 0, sizeof(MMI_STRING_T));
        MMIWWW_Netfront_GetInputURLHistoryUrl(i, &url_str_info);
        
        MMIAPICOM_Wstrlower(url_str_info.wstr_ptr);    
        if(!MMIAPICOM_Wstrncmp( url_str_info.wstr_ptr, temp_input_url, MMIAPICOM_Wstrlen(temp_input_url)))
        {            
            match_num++;
        }

        if (PNULL != url_str_info.wstr_ptr)
        {
            SCI_FREE(url_str_info.wstr_ptr);
        }

    }
    SCI_FREE(temp_input_url);
    return match_num;
}

/*****************************************************************************/
// Description : List match URL 
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ListMatchURLName( wchar *url_ptr, MMI_HANDLE_T ctr_handle )
{
    uint16 i;
    uint16 match_num = 0;
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 InputURL_num = (uint16)MMIWWW_Netfront_GetInputURLHistoryNumber();

    wchar *temp_input_url = NULL;

    uint16 len = 0;
    if((NULL == url_ptr )||( 0 == (len = MMIAPICOM_Wstrlen(url_ptr ))))
    {
         return 0;
    }

    temp_input_url = SCI_ALLOCA(sizeof(wchar) * (len + 1));  
    if(NULL == temp_input_url)
    {
       return 0;
    }
    SCI_MEMSET(temp_input_url, 0, sizeof(wchar) *  (len + 1));

    MMIAPICOM_Wstrncpy(temp_input_url, url_ptr, len);
    
    GUILIST_SetMaxItem( ctr_handle, MMIWWW_INPUTHISTORY_URL_NUM_MAX, FALSE );

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    MMIAPICOM_Wstrlower(temp_input_url);

    for( i = 0; i < InputURL_num; i++ )
    {
        wchar *url_his_ptr = NULL; 
        MMI_STRING_T url_str_info = {0};
        MMIWWW_Netfront_GetInputURLHistoryUrl(i, &url_str_info);

        url_his_ptr = SCI_ALLOCA(sizeof(wchar) * (url_str_info.wstr_len + 1));  
        if(NULL == url_his_ptr)
        {
           break;
        }
        SCI_MEMSET(url_his_ptr, 0, sizeof(wchar) *  (url_str_info.wstr_len + 1));
        
        MMIAPICOM_Wstrncpy(url_his_ptr, url_str_info.wstr_ptr,  url_str_info.wstr_len);       
        MMIAPICOM_Wstrlower(url_his_ptr);  

        if(!MMIAPICOM_Wstrncmp(url_his_ptr , temp_input_url, MMIAPICOM_Wstrlen(temp_input_url)))
        {            
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
            
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = url_str_info.wstr_len;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = url_str_info.wstr_ptr;
            
            GUILIST_AppendItem(ctr_handle, &item_t);
            match_num++;
        }
        
        SCI_FREE(url_his_ptr);
        SCI_FREE(url_str_info.wstr_ptr);
    }
    SCI_FREE(temp_input_url);
    return match_num;
}

/*****************************************************************************/
//  Description : Set url change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetUrlChange(BOOLEAN is_change)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->is_url_change = is_change;
    }
}

/*****************************************************************************/
//  Description : Get url change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetUrlChange(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_url_change;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Set keywork change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetKeywordChange(BOOLEAN is_change)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->is_keyword_change = is_change;
    }
}

/*****************************************************************************/
//  Description : Get keywork change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetKeywordChange(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_keyword_change;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Set bookmark change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetBookmarkChange(BOOLEAN is_change)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->is_bookmark_change = is_change;
    }
}

/*****************************************************************************/
//  Description : Get bookmark change flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetBookmarkChange(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_bookmark_change;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Set PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetPDPInfo(MMIWWW_PDP_INFO_T *pdp_info)
{
    MMIWWW_INSTANCE_T * www_instance =MMIWWW_INSTANCE();
    
    if (PNULL != pdp_info && PNULL != www_instance)
    {
        SCI_MEMCPY(&www_instance->pdp_info.connect_setting, &pdp_info->connect_setting, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
    }
}

/*****************************************************************************/
//  Description : Set added url address
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetAddedUrl(uint8 *in_url)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->added_url = in_url;
    }
}

/*****************************************************************************/
//  Description : Get added url address
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_NetfrontGetAddedUrl(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->added_url;
    }

    return PNULL;
}

/*****************************************************************************/
//  Description : Set added title
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetAddedTitle(uint8 *in_title)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->added_title = in_title;
    }
}

/*****************************************************************************/
//  Description : Get added title
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_NetfrontGetAddedTitle(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->added_title;
    }

    return PNULL;
}

/*****************************************************************************/
//  Description : Generate search web
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 * MMIWWW_GenerateSearchWeb(MMIWWW_SEARCH_ENGINE_MODE_E search_engine, MMI_STRING_T * search_str)
{
    uint16 str_len = 0;
    uint8 *str = PNULL;
    uint8 *str_temp = PNULL;
    uint16 url_len = 0;
    uint8 *url = PNULL;
    uint8 *url_temp = PNULL;
    char *search_head = PNULL;
    char *search_tail = PNULL;

    //SCI_TRACE_LOW:"MMIWWW_GenerateSearchWeb search_engine = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_738_112_2_18_3_11_34_45,(uint8*)"d", search_engine);

    if (MMIWWW_SEARCH_ENGINE_GOOGLE == search_engine)
    {
        search_head = (char *)BRSCFG_GOOGLE_SEARCH_HEAD;
        search_tail = (char*)BRSCFG_GOOGLE_UTF8_SEARCH_TAIL;
    }
    else if (MMIWWW_SEARCH_ENGINE_CMCC == search_engine)
    {
        search_head = (char *)BRSCFG_CMCC_SEARCH_HEAD;
        search_tail = (char*)BRSCFG_CMCC_UTF8_SEARCH_TAIL;
    }
    else if (MMIWWW_SEARCH_ENGINE_BAIDU == search_engine)
    {
        search_head = (char *)BRSCFG_BAIDU_SEARCH_HEAD;
        search_tail = (char*)BRSCFG_BAIDU_GB2312_SEARCH_TAIL;
    }
    else
    {
        search_head = (char *)BRSCFG_YAHOO_SEARCH_HEAD;
        search_tail = (char*)BRSCFG_YAHOO_UTF8_SEARCH_TAIL;
    }

    if (PNULL != search_str->wstr_ptr && search_str->wstr_len > 0)
    {
        str_len = (uint16)(search_str->wstr_len * 3 + 1);
        str = SCI_ALLOCA(str_len);
        if (PNULL != str)
        {
            SCI_MEMSET(str, 0x00, str_len);
            if (MMIWWW_SEARCH_ENGINE_GOOGLE == search_engine)
            {
                GUI_WstrToUTF8(str, str_len, search_str->wstr_ptr, search_str->wstr_len);
            }
            else if (MMIWWW_SEARCH_ENGINE_CMCC == search_engine)
            {
                GUI_WstrToUTF8(str, str_len, search_str->wstr_ptr, search_str->wstr_len);
            }
            else if (MMIWWW_SEARCH_ENGINE_BAIDU == search_engine)
            {
                GUI_WstrToGB(str, search_str->wstr_ptr, search_str->wstr_len);
            }
            else
            {
                GUI_WstrToUTF8(str, str_len, search_str->wstr_ptr, search_str->wstr_len);
            }
            
            url_len = (uint16)(SCI_STRLEN(search_head) + 3 * SCI_STRLEN((char *)str) + strlen(search_tail)+ 1);
            url = SCI_ALLOCA(url_len);
            if (PNULL != url)
            {
                SCI_MEMSET((char *)url, 0x00, url_len);
                strncpy((char *)url, search_head, SCI_STRLEN(search_head));
                str_temp = str;
                url_temp = url + SCI_STRLEN((char *)url);
                while(*str_temp)
                {
                    *url_temp++ = '%';
                    sprintf((char *)url_temp, "%02x", *str_temp++);

                    url_temp = (uint8 *)MMIAPICOM_Strupper((char *)url_temp);
                    
                    url_temp += 2;
                }
                *url_temp = '\0';
                strcat((char *)url, search_tail);
            }
            SCI_FREE(str);
            return url;
        }
    }
    return PNULL;
}

/*****************************************************************************/
//  Description : Check the URL validity
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsURLValid(MMI_STRING_T *url_str)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : Go to URL
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_ActionGoToURL(MMI_STRING_T *url_str)
{
    uint16 str_len = 0;
    uint8 *str_temp = PNULL;

    if (PNULL == url_str)
    {
        return;
    }

    if (PNULL != url_str->wstr_ptr && url_str->wstr_len > 0)
    {
        str_len = (uint16)(url_str->wstr_len * 3 + 1);
        str_temp = SCI_ALLOCA(str_len);
        if (PNULL != str_temp)
        {
            SCI_MEMSET(str_temp, 0x00, str_len);
            GUI_WstrToUTF8(str_temp, str_len, url_str->wstr_ptr, url_str->wstr_len);
            MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)str_temp);
            SCI_FREE(str_temp);
        }
    }
}

/*****************************************************************************/
//  Description : Get link button status
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetLinkButtonDisabled(void)
{
    return (BOOLEAN)!MMIWWW_NetfrontGetUrlChange();
}

/*****************************************************************************/
//  Description : Get search button status
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetSearchButtonDisabled(void)
{
    return (BOOLEAN)!MMIWWW_NetfrontGetKeywordChange();
}

/*****************************************************************************/
//  Description : Set focus ctrl of main menu 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetMainMenuFocusCtrl(MMIWWW_CTRL_E in_ctrl)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->focus_ctrl = in_ctrl;
    }
}

/*****************************************************************************/
//  Description : Get focus ctrl of main menu
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIWWW_CTRL_E MMIWWW_NetfrontGetMainMenuFocusCtrl(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->focus_ctrl;
    }

    return MMIWWW_CTRL_NULL;
}

/*****************************************************************************/
//  Description : Get www font style
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetFontStyle(void)
{
    uint16 font_style = 0;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_WWW_FONT_STYLE, &font_style, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        font_style = (uint16)MMIWWW_FONT_TYPE_NORMAL;
        MMINV_WRITE(MMINV_WWW_FONT_STYLE, &font_style);
    }
    return font_style;
}

/*****************************************************************************/
//  Description : Set charset
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetCharsetIndex(slim_int  index, uint16 charset_index)
{
    int32 i = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        if (MMIWWW_BROWSER_MAX_TAB == index)
        {
            for (i = 0; i < MMIWWW_BROWSER_MAX_TAB; i++)
            {
                www_instance->charset_index[i] = charset_index;
            }
        }
        else
        {
            www_instance->charset_index[index] = charset_index;
        }
    }
}

/*****************************************************************************/
//  Description : Get charset
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetCharsetIndex(slim_int  index)
{
    uint16 charset_index = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
 
    if (MMIWWW_BROWSER_MAX_TAB > index && PNULL != www_instance)
    {
        charset_index = www_instance->charset_index[index] ;
    }
    return charset_index;
}

/*****************************************************************************/
//  Description : Set charset change
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetCharsetChange(slim_int  index, BOOLEAN is_change)
{
    int32 i = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        if (MMIWWW_BROWSER_MAX_TAB == index)
        {
            for (i = 0; i < MMIWWW_BROWSER_MAX_TAB; i++)
            {
                www_instance->is_charset_change[i] = is_change;
            }
        }
        else
        {
            www_instance->is_charset_change[index] = is_change;
        }
    }
}

/*****************************************************************************/
//  Description : Get charset change
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetCharsetChange(slim_int  index)
{
    BOOLEAN is_charset_change = FALSE;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
 
    if (MMIWWW_BROWSER_MAX_TAB > index && PNULL != www_instance)
    {
        is_charset_change = www_instance->is_charset_change[index] ;
    }
    return is_charset_change;
}

/*****************************************************************************/
//  Description : Convert index to charset
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIWWW_ConvertCharsetIndexToCharset(uint16 charset_index)
{
    int32 charset = -1;
 
    switch(charset_index)
    {
    case 0:
        charset = -1;
        break;

    case 1:
        charset = SLIM_CHARSET_US_ASCII;
        break;

    case 2:
        charset = SLIM_CHARSET_GB2312;
        break;

    case 3:
        charset = SLIM_CHARSET_GBK;
        break;
    case 4:
        charset = SLIM_CHARSET_GB18030;
        break;

    case 5:
        charset = SLIM_CHARSET_BIG5;
        break;

    case 6:
       charset = SLIM_CHARSET_UTF_8;
        break;

    case 7:
        charset = SLIM_CHARSET_UTF_16BE;
        break;

    default:
        charset = -1;
        break;
    }
    return charset;
}

/*****************************************************************************/
//  Description : Get www view mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetViewMode(void)
{
    uint16 view_mode = 0;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    slim_int nf_view_mode[] = {SETTINGS_VIEW_ORIGINAL, SETTINGS_VIEW_TEXT, SETTINGS_VIEW_SMARTFIT};
    
    MMINV_READ(MMINV_WWW_VIEW_MODE, &view_mode, return_value );

    if (MN_RETURN_SUCCESS != return_value || view_mode >= SETTINGS_VIEW_TEXT)
    {
        view_mode = 2;//smart fit

        MMINV_WRITE(MMINV_WWW_VIEW_MODE, &view_mode);
    }

    return (uint16)nf_view_mode[view_mode];
}

/*****************************************************************************/
//  Description : Get www cookie mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetCookieMode(void)
{
    uint16 cookie_mode = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;
    
    MMINV_READ( MMINV_WWW_COOKIE_MODE, &cookie_mode, return_value );
    if (MN_RETURN_SUCCESS != return_value)
    {
        cookie_mode = SETTINGS_COOKIE_MODE_ACCEPT_ALL;
    }

    return cookie_mode;
}

/*****************************************************************************/
//  Description : Get www cache mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetCacheMode(void)
{
    uint16 cache_mode = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;
    
    MMINV_READ( MMINV_WWW_CACHE_MODE, &cache_mode, return_value );
    if (MN_RETURN_SUCCESS != return_value)
    {
        cache_mode = SETTINGS_CACHE_MODE_CACHE_FIRST;
    }

    return cache_mode;
}

/*****************************************************************************/
//  Description : Get www disk cache mode
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetDiskCacheMode(void)
{
    uint16 disk_cache_mode = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_WWW_DISKCACHE_MODE, &disk_cache_mode, return_value );
    if (MN_RETURN_SUCCESS != return_value)
    {
        disk_cache_mode = 0;
    }

    return disk_cache_mode;
}

/*****************************************************************************/
//  Description : Get www save WebData mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetWebDataMode(void)
{
#ifdef MMIWWW_SETTINGS_USE_WEBDATA
    uint16 item_set = 0;
    BOOLEAN ret = FALSE;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ( MMINV_WWW_WEBDATA_MODE, &item_set, return_value );
    if ((MN_RETURN_SUCCESS != return_value)||(0 == item_set))
    {
        return TRUE;
    }
#endif
    return FALSE;
}

/*****************************************************************************/
//  Description : Get www script mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetScriptMode(void)
{
    uint16 script_mode = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ( MMINV_WWW_SCRIPT_MODE, &script_mode, return_value );

    if (MN_RETURN_SUCCESS != return_value)
    {
        script_mode = SETTINGS_JAVASCRIPT;
    }

    return script_mode;
}

/*****************************************************************************/
//  Description : Get www image mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetImageMode(void)
{
    uint16 image_mode = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ( MMINV_WWW_PHOTO_MODE, &image_mode, return_value );

    if ((MN_RETURN_SUCCESS != return_value) || (image_mode == 0))
    {
        image_mode = SETTINGS_IMAGES_DISPLAY;
    }
    else
    {
        image_mode = SETTINGS_IMAGES_NOT_DISPLAY;
    }
    return image_mode;
}

/*****************************************************************************/
//  Description : Get www Animate mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetAnimateMode(void)
{
    uint16 animate_mode = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ( MMINV_WWW_ANIMATION_MODE, &animate_mode, return_value );

    if ((MN_RETURN_SUCCESS != return_value)||(0 == animate_mode))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Get www bgsound mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBGSoundMode(void)
{
    uint16 bgsound_mode = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ( MMINV_WWW_BGMUSIC_MODE, &bgsound_mode, return_value );

    if ((MN_RETURN_SUCCESS != return_value)||(0 == bgsound_mode))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Get www table mode
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetTableMode(void)
{
    uint16 table_mode = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ( MMINV_WWW_TABLE_MODE, &table_mode, return_value );

    if ((MN_RETURN_SUCCESS != return_value)||(0 == table_mode))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// Description : Set browser state
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetWWWState(slim_int  index, MMIWWW_STATE_T  state)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance && index < MMIWWW_BROWSER_MAX_TAB && state < MMIWWW_STATE_MAX)
    {
        www_instance->www_state[index] = state;
    }

    return;
}

/*****************************************************************************/
// Description : Get browser state
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMIWWW_STATE_T MMIWWW_GetWWWState(slim_int  index)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance && index < MMIWWW_BROWSER_MAX_TAB)
    {
        return www_instance->www_state[index];
    }

    return MMIWWW_STATE_MAX;
}

/*****************************************************************************/
// Description : Set browser fullscreen display flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetFullDisplayFlag(BOOLEAN  is_full_display)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->is_full_display = is_full_display;
    }

    return;
}

/*****************************************************************************/
// Description : Get browser fullscreen display flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsFullDisplay(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_full_display;
    }

    return FALSE;
}

/*****************************************************************************/
// Description : Set exit browser flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetExitFlag(BOOLEAN  is_exit)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->is_exit = is_exit;
    }

    return;
}

/*****************************************************************************/
// Description : Get exit browser flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsExit(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_exit;
    }

    return FALSE;
}

/*****************************************************************************/
// Description : Set page secure flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetPageSecureFlag(slim_int  index, slim_bool is_secure_page)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (MMIWWW_BROWSER_MAX_TAB > index && PNULL != www_instance)
    {
        www_instance->secure_page[index] = is_secure_page;
    }

    return;
}

/*****************************************************************************/
// Description : Get page secure flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_GetPageSecureFlag(slim_int  index)
{
    slim_bool    ret = FALSE;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (index < MMIWWW_BROWSER_MAX_TAB && PNULL != www_instance)
    {
       ret = www_instance->secure_page[index];
    }

    return ret;
}

/*****************************************************************************/
// Description : Set cuurent page zoom percent
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetCurrentPageZoomPercent(MMIWWW_ZOOM_E zoom_percent)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int act_index = 0;

    act_index = MMIWWW_Netfront_GetActWinIndex();

    if (PNULL != www_instance && act_index < MMIWWW_BROWSER_MAX_TAB)
    {
        www_instance->peer_data.fZoom_percent[act_index] = zoom_percent;
    }
    //SCI_TRACE_LOW:"MMIWWW_SetCurrentPageZoomPercent act_index = %d zoom_percent = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1383_112_2_18_3_11_36_46,(uint8*)"dd", act_index, zoom_percent);
}

/*****************************************************************************/
// Description : Get cuurent page zoom percent
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC MMIWWW_ZOOM_E MMIWWW_GetCurrentPageZoomPercent(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int act_index = 0;
    MMIWWW_ZOOM_E zoom_percent = MMIWWW_ZOOM_ORG;

    act_index = MMIWWW_Netfront_GetActWinIndex();
    //SCI_TRACE_LOW:"MMIWWW_GetCurrentPageZoomPercent act_index = %d zoom_percent = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1397_112_2_18_3_11_36_47,(uint8*)"dd", act_index, zoom_percent);
    if (PNULL != www_instance && act_index < MMIWWW_BROWSER_MAX_TAB)
    {
        zoom_percent = (MMIWWW_ZOOM_E)www_instance->peer_data.fZoom_percent[act_index];
    }
    //SCI_TRACE_LOW:"MMIWWW_GetCurrentPageZoomPercent act_index = %d zoom_percent = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1402_112_2_18_3_11_36_48,(uint8*)"dd", act_index, zoom_percent);
    return zoom_percent;
}

/*****************************************************************************/
//  Description :Play Background sound callback
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void WwwPlayBgsoundCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"WwwPlayBgsoundCallBack, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1412_112_2_18_3_11_36_49,(uint8*)"d", result);
    if(result != MMISRVAUD_REPORT_RESULT_STOP)
    {    
    }
}

/*****************************************************************************/
//  Description : Play BG sound
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Play_BG_sound(slim_int in_index)
{
    NFBrowserBGSoundInfo bg_info = {0};/*lint !e64*/
    BOOLEAN ret = FALSE;
    uint32 play_times = 0;
    MMISRVAUD_RING_FMT_E audio_type = MMISRVAUD_RING_FMT_MAX;

    ret = (BOOLEAN)MMIWWW_Netfront_GetBGSoundInfo(in_index, 0, &bg_info);
    //SCI_TRACE_LOW:"MMIWWW_Play_BG_sound ret=%d, fVolume=%d,fLength=%d,fLoop=%d,fMimeType=%d,fDataSize=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1431_112_2_18_3_11_36_50,(uint8*)"dddddd",ret, bg_info.fVolume, bg_info.fLength, bg_info.fLoop, bg_info.fMimeType, bg_info.fContent.fDataSize);
  
    if (ret)
    {
        if (0 > bg_info.fLoop)
        {
            play_times = MMIWWW_BGSOUND_INFINITE;
        }
        else
        {
            play_times = bg_info.fLoop;
        }

#ifdef DL_SUPPORT
        audio_type = MMIAPIDL_GetAudioTypeByNFMimeType(bg_info.fMimeType);
#endif

        if (MMISRVAUD_RING_FMT_MAX != audio_type)
        {
            /* Pause BG Play */
            MMIAPISET_PlayRingByPtr(MMISET_RING_TYPE_OTHER,
                                    MMISET_VOL_TWO,
                                    bg_info.fContent.fData,
                                    bg_info.fContent.fDataSize,
                                    play_times,
                                    audio_type,
                                    WwwPlayBgsoundCallBack
                                    );
        }
    }

    NFBrowser_ReleaseSlimMem(&bg_info.fContent);
    NFBrowser_ReleaseSlimMem(&bg_info.fName);
    return ret;
}

/*****************************************************************************/
//  Description : Calculate days from current year 1.1 to current day
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_CountDate_CurYear(SCI_DATE_T sys_date)
{
    int days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31}; 
    int i,sum=0; 
    for(i=0;i<sys_date.mon;i++) 
       sum+=days[i]; 
    if(sys_date.mon>2)  /*如果是闰年则2月加一天*/ 
        if((sys_date.year%4==0)&&(sys_date.year%100!=0)||(sys_date.year%400==0)) 
            sum+=1; 
       sum += (sys_date.mday - 1); /* 1.1 return 0 */ 
    return sum;
}

/*****************************************************************************/
//  Description : Calculate days from 1970.1.1 to current day
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_CountDate_From1970(SCI_DATE_T sys_date)
{
    uint32 days_tmp = 0;
    uint32 year_tmp = 1970;
    uint32 toldays_num = 0;
    
    toldays_num = MMIWWW_CountDate_CurYear(sys_date);

    while(year_tmp < sys_date.year)
    {
        days_tmp = 365;
        if((year_tmp%4==0)&&(year_tmp%100!=0)||(year_tmp%400==0)) 
             days_tmp+=1; 
        toldays_num += days_tmp;
        year_tmp++;
    }

    return toldays_num;
}

/*****************************************************************************/
//  Description : Get search engine
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIWWW_GetSearchEngine(void)
{
    uint8 search_engine = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_WWW_SEARCH_ENGINE, &search_engine, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        search_engine = 0;
    }

    return (uint8)search_engine;
}

/*****************************************************************************/
//  Description : Set search engine
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetSearchEngine(uint8* search_engine)
{
    MMINV_WRITE(MMINV_WWW_SEARCH_ENGINE, search_engine);
}

/*****************************************************************************/
//  Description : Get first entry flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetFirstEntry(void)
{
    BOOLEAN first_entry= FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_WWW_FIRST_ENTRY, &first_entry, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        first_entry = TRUE;
        MMINV_WRITE(MMINV_WWW_FIRST_ENTRY, &first_entry);
    }

    return first_entry;
}

/*****************************************************************************/
//  Description : Set first entry flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetFirstEntry(BOOLEAN first_entry)
{
    MMINV_WRITE(MMINV_WWW_FIRST_ENTRY, &first_entry);
}

/*****************************************************************************/
//  Description : Get Pointer Mode flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIWWW_POINTER_MODE_E MMIWWW_GetPointerMode(void)
{
    MMIWWW_POINTER_MODE_E pointer_mode = MMIWWW_POINTER_MODE_NORMAL;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_WWW_POINTER_MODE, &pointer_mode, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        pointer_mode = MMIWWW_POINTER_MODE_NORMAL;
        MMINV_WRITE(MMINV_WWW_POINTER_MODE, &pointer_mode);
    }

    return pointer_mode;
}

/*****************************************************************************/
//  Description : Set Pointer Mode flag
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetPointerMode(MMIWWW_POINTER_MODE_E pointer_mode)
{
    MMINV_WRITE(MMINV_WWW_POINTER_MODE, &pointer_mode);
}

/*****************************************************************************/
//  Description : Set default user dada
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetDefaultUserData(MMIWWW_INSTANCE_T *self)
{
    MMIWWW_SetDefaultBookmartk(self);
}

/*****************************************************************************/
//  Description : Set default bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetDefaultBookmartk(MMIWWW_INSTANCE_T *self)
{
    int32 index;
    int32 arrary_num = 0;
    int32 i = 0;
    wchar wchar_title[MMIWWW_MAX_DEFAULT_BOOKMARK_NAME_LEN + 1] = {0};
    BOOLEAN ret = FALSE;
    MMI_STRING_T url_str = {0};
    MMI_STRING_T title_str = {0};
    BOOLEAN is_res_data_exist = FALSE;
    MMIRES_BOOKMARK_INFO_T res_info = {0};

    char *url[] = {
                        "http://www.baidu.com",
//                        "http://www.google.cn",
                        "http://wap.yahoo.com.cn",
                        "http://wap.sohu.com",
                        "http://3g.sina.com.cn",
                        "http://www.hao123.com",
#ifdef CMCC_UI_STYLE
                        "http://s.139.com",
                        "http://wap.monternet.com/?cp22=v22monternet"
#else
                        "http://wap.139.com",
                        "http://wap.monternet.com"
#endif
                  };

    /* The num of Charector Do Not more than MMIWWW_MAX_DEFAULT_BOOKMARK_NAME_LEN */
    char *title[] = {
                        "百度",
//                        "谷歌",
                        "雅虎中国",
                        "搜狐",
                        "新浪",
                        "网站导航",
                        "移动搜索",
                        "移动梦网Web门户"
                    };

    is_res_data_exist = MMIRES_LoadBookmarkFromRes(&res_info);

    if (is_res_data_exist && PNULL != res_info.res_item_ptr)
    {
        for (i = 0; i < res_info.res_num; i++)
        {
            title_str.wstr_len = res_info.res_item_ptr[i].name_len;
            title_str.wstr_ptr = res_info.res_item_ptr[i].name;
            url_str.wstr_len = res_info.res_item_ptr[i].url_len;
            url_str.wstr_ptr = SCI_ALLOCA((url_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL != url_str.wstr_ptr)
            {
                SCI_MEMSET(url_str.wstr_ptr, 0x00, ((url_str.wstr_len + 1) * sizeof(wchar)));
                GUI_GBToWstr(url_str.wstr_ptr, (const uint8*)res_info.res_item_ptr[i].url, url_str.wstr_len);
                ret = MMIWWW_AddBookmark(url_str, title_str, TRUE, (uint16 *)&index);

                SCI_FREE(url_str.wstr_ptr);
          
                //SCI_TRACE_LOW:"MMIWWW_SetDefaultBookmartk ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1662_112_2_18_3_11_36_51,(uint8*)"d", ret);
            }
            else
            {
                MMIRES_UnloadBookmarkInfo(&res_info);
            
                //SCI_TRACE_LOW:"MMIWWW_SetDefaultBookmartk NO memory"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1668_112_2_18_3_11_36_52,(uint8*)"");
                return;
            }
        }

        MMIRES_UnloadBookmarkInfo(&res_info);
    }
    else
    {
        arrary_num = sizeof(url) / sizeof(url[0]);

        for (i = 0; i < arrary_num; i++)
        {
            SCI_MEMSET(wchar_title, 0x0, (MMIWWW_MAX_DEFAULT_BOOKMARK_NAME_LEN + 1) * sizeof(wchar));
            title_str.wstr_len = GUI_GBToWstr(wchar_title, (const uint8*)title[i], strlen(title[i]));
            title_str.wstr_ptr = wchar_title;

            url_str.wstr_len = strlen(url[i]);
            url_str.wstr_ptr = SCI_ALLOCA((url_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL != url_str .wstr_ptr)
            {
                SCI_MEMSET(url_str.wstr_ptr, 0x00, ((url_str.wstr_len + 1) * sizeof(wchar)));
                GUI_GBToWstr(url_str.wstr_ptr, (const uint8*)url[i], url_str.wstr_len);
                ret = MMIWWW_AddBookmark(url_str, title_str, TRUE, (uint16 *)&index);

                SCI_FREE(url_str.wstr_ptr);
          
                //SCI_TRACE_LOW:"MMIWWW_SetDefaultBookmartk ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1695_112_2_18_3_11_37_53,(uint8*)"d", ret);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIWWW_SetDefaultBookmartk NO memory"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1699_112_2_18_3_11_37_54,(uint8*)"");
                return;
            }
        }
    }
}

/*****************************************************************************/
// Description : Set browser first open flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetIsFirstOpen(BOOLEAN  flag)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

#ifndef MMIWWW_FIRST_CONNECT_QUERY
    flag = FALSE;
#endif

    if (PNULL != www_instance)
    {
        www_instance->is_first_open = flag;
    }

    return;
}

/*****************************************************************************/
// Description : Get browser first open flag
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_IsFirstOpen(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_first_open;
    }

    return FALSE;
}

/*****************************************************************************/
// Description : Get saved url data
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 *MMIWWW_GetSavedUrl(NFBrowserStartType  *out_type)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == out_type || PNULL == www_instance)
    {
        return PNULL;
    }
    
    *out_type = www_instance->first_type;
    return www_instance->first_url;
}

/*****************************************************************************/
// Description : Save url data
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SaveUrl(NFBrowserStartType in_type, char *in_url)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    uint8                *save_ptr = PNULL;
    uint16                url_len = 0;

    if (PNULL == www_instance)
    {
        return;
    }

    www_instance->first_type = in_type;
    if (PNULL != in_url)
    {
        url_len = (uint16)SCI_STRLEN(in_url);
        save_ptr = SCI_ALLOCA(url_len+1);

        if (PNULL == save_ptr)
        {
            return;
        }

        SCI_MEMSET(save_ptr, 0x00, (url_len+1));
        SCI_MEMCPY(save_ptr, in_url, url_len);
        MMIWWW_FreeFirstURL();
        www_instance->first_url = save_ptr;
    }
    return;
}

/*****************************************************************************/
//  Description : init connection setting 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_InitConnectionSetting(void)
{
    MMICONNECTION_NOTIFY_INFO_T notify_info = {0};

    notify_info.module_id = MMI_MODULE_WWW;
    notify_info.notify_func = MMIWWW_ConnectionCallback;
    MMIAPICONNECTION_RegisterNotifyInfo(&notify_info);
}

/*****************************************************************************/
// Description : Reset Default Settings
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_ResetDefaultSettings(void)
{
    uint16 item = 0;
    MMIWWW_LINK_SETTING_T link_setting = {0};
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    uint8 search_engine = 0;
    uint16 view_org_value = 0;
#ifdef MMIWWW_SETTINGS_USE_FONT
    uint16 font_org_value = 0;
#endif
    BOOLEAN need_reload = FALSE;

    //script mode
    MMINV_WRITE( MMINV_WWW_SCRIPT_MODE, &item );

    //animation mode
    MMINV_WRITE( MMINV_WWW_ANIMATION_MODE, &item );

    //photo mode
    MMINV_WRITE( MMINV_WWW_PHOTO_MODE, &item );

    //table mode
    MMINV_WRITE( MMINV_WWW_TABLE_MODE, &item );

    //BGmusic mode
    MMINV_WRITE( MMINV_WWW_BGMUSIC_MODE, &item );

    //popup win
    MMINV_WRITE( MMINV_WWW_POPUPWIN_MODE, &item );

    //cookie mode
    MMINV_WRITE( MMINV_WWW_COOKIE_MODE, &item );

#ifdef MMIWWW_SETTINGS_USE_WEBDATA
    //web data mode
    MMINV_WRITE( MMINV_WWW_WEBDATA_MODE, &item );
#endif

    //disk cache mode
    MMINV_WRITE( MMINV_WWW_DISKCACHE_MODE, &item );

    //link settings
    link_setting.sim1_index = (uint16)MMIAPICONNECTION_GetAccountDefaultIndex(MN_DUAL_SYS_1, MMICONNECTION_ACCOUNT_BROWSER);
    MMINV_WRITE( MMINV_WWW_LINK_SETTING, &link_setting );

    //search engine
    MMINV_WRITE( MMINV_WWW_SEARCH_ENGINE, &search_engine );

    //cache mode
    item = 1; /* NetFirst */
    MMINV_WRITE( MMINV_WWW_CACHE_MODE, &item );

    //Homepage
    MMIWWW_SetHomepageURL(MMIWWW_CMCC_URL);

    //Pointer mode
    MMIWWW_SetPointerMode(MMIWWW_POINTER_MODE_NORMAL);

    //view mode /* Smart fit*/
    view_org_value = MMIWWW_GetViewMode();
    if (SETTINGS_VIEW_SMARTFIT != view_org_value)
    {
        need_reload = TRUE;
    }

    item= SETTINGS_VIEW_SMARTFIT;
    MMINV_WRITE( MMINV_WWW_VIEW_MODE, &item );
    
#ifdef MMIWWW_SETTINGS_USE_FONT
    //normal font
    if (!need_reload)
    {
        font_org_value = MMIWWW_GetFontStyle();
        if (MMIWWW_FONT_TYPE_NORMAL != font_org_value)
        {
            need_reload = TRUE;
        }
    }
    item = MMIWWW_FONT_TYPE_NORMAL;
    MMINV_WRITE( MMINV_WWW_FONT_STYLE, &item );
#endif

    if (PNULL != www_applet_ptr)
    {
        int32 i = 0;

        //MMIWWW_ResetNetConnect();
        MMIWWW_Netfront_ViewMode(MMIWWW_GetViewMode());
        MMIWWW_Netfront_SetScriptMode(MMIWWW_GetScriptMode());
        MMIWWW_Netfront_SetDisplayAnimateImage(MMIWWW_GetAnimateMode());
        MMIWWW_Netfront_SetImageDisplayMode(MMIWWW_GetImageMode());
        MMIWWW_Netfront_SetDisplayTable(MMIWWW_GetTableMode());
        MMIWWW_Netfront_SetEnableBGSound(MMIWWW_GetBGSoundMode());
        MMIWWW_Netfront_SetCookieMode(MMIWWW_GetCookieMode());
        MMIWWW_Netfront_SetCacheMode(MMIWWW_GetCacheMode());
#ifdef MMIWWW_SETTINGS_USE_WEBDATA
        MMIWWW_Netfront_SetWebData(MMIWWW_GetWebDataMode());
#endif
        MMIWWW_Netfront_SetUseDiskCacheMode(MMIWWW_GetDiskCacheMode());
        MMIWWW_Netfront_SetPointerMode(MMIWWW_GetPointerMode());

        if (need_reload)
        {
            MMIWWW_SetBrowserReloadFlag(MMIWWW_BROWSER_MAX_TAB, TRUE);
        }
        else
        {
            for (i = 0; i < MMIWWW_BROWSER_MAX_TAB; i++)
            {
                if (MMIWWW_GetCharsetIndex(i) != 0)
                {
                    MMIWWW_SetBrowserReloadFlag((uint8)i, TRUE);
                }
            }

        }

        MMIWWW_SetCharsetIndex(MMIWWW_BROWSER_MAX_TAB, 0);
    }

    MMIAPICONNECTION_AdaptingAccount(MMI_DUAL_SYS_MAX, MMICONNECTION_ACCOUNT_BROWSER);
}

/*****************************************************************************/
//  Description : Get www net setting index 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIWWW_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    uint8 index = 0;
    MMIWWW_LINK_SETTING_T link_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"MMIAPIWWW_GetNetSettingIndex dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1943_112_2_18_3_11_37_55,(uint8*)"d", dual_sys);
    
    MMINV_READ(MMINV_WWW_LINK_SETTING, &link_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        return 0;
    }
    if(MN_DUAL_SYS_1 == dual_sys)
    {
        index = link_setting.sim1_index;
    }
    else
    {
        index = link_setting.sim2_index;
    }
    return index;
}

/*****************************************************************************/
//  Description : Set www net setting index 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetNetSettingIndex(MN_DUAL_SYS_E dual_sys, uint8 index)
{
    MMIWWW_LINK_SETTING_T link_setting = {0};

    //SCI_TRACE_LOW:"MMIWWW_SetNetSettingIndex dual_sys = %d, index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_1969_112_2_18_3_11_37_56,(uint8*)"dd", dual_sys, index);
    if (MN_DUAL_SYS_1 == dual_sys)
    {
        link_setting.sim1_index = index;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        link_setting.sim2_index = MMIAPIWWW_GetNetSettingIndex(MN_DUAL_SYS_2);
#endif
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else if(MN_DUAL_SYS_2 == dual_sys)
    {
        link_setting.sim1_index = MMIAPIWWW_GetNetSettingIndex(MN_DUAL_SYS_1);
        link_setting.sim2_index = index;
    }
#endif
    else
    {
        //do nothing
    }    
    MMINV_WRITE(MMINV_WWW_LINK_SETTING, &link_setting);
}

/*****************************************************************************/
//  Description : update www net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIWWW_ConnectionCallback(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN need_write = FALSE;
    MMIWWW_LINK_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    uint8 index = (uint8)param;

    //SCI_TRACE_LOW:"MMIWWW_ConnectionCallback,index=%d, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2002_112_2_18_3_11_37_57,(uint8*)"dd",index, dual_sys);
    
    MMINV_READ(MMINV_WWW_LINK_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        need_write = TRUE;
        nv_setting.sim1_index = (uint16)MMIAPICONNECTION_GetAccountDefaultIndex(MN_DUAL_SYS_1, MMICONNECTION_ACCOUNT_BROWSER);
    }

    switch(notify_event)
    {
    case MMICONNECTION_NOTIFY_EVENT_DEL_ITEM:
        if (MN_DUAL_SYS_1 == dual_sys)
        {
            if(nv_setting.sim1_index == index)
            {
                need_write = TRUE;
                nv_setting.sim1_index = 0;
            }
            else if(nv_setting.sim1_index > index)
            {
                need_write = TRUE;
                nv_setting.sim1_index--;
            }
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else if (MN_DUAL_SYS_2 == dual_sys)
        {
            if(nv_setting.sim2_index == index)
            {
                need_write = TRUE;
                nv_setting.sim2_index = 0;
            }
            else if(nv_setting.sim2_index > index)
            {
                need_write = TRUE;
                nv_setting.sim2_index--;
            }
        }
#endif
        if (need_write)
        {
            MMINV_WRITE(MMINV_WWW_LINK_SETTING, &nv_setting);
            MMIAPICONNECTION_AdaptingAccount(dual_sys, MMICONNECTION_ACCOUNT_BROWSER);
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Get PDP info
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetPDPInfo(MMIWWW_PDP_INFO_T *pdp_info)
{
    if (PNULL != pdp_info)
    {
        MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

        if (PNULL != www_instance)
        {
            SCI_MEMCPY(pdp_info, &www_instance->pdp_info, sizeof(MMIWWW_PDP_INFO_T));
        }
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Get PDP info of settings
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetSettingsPDPInfo(MMIWWW_PDP_INFO_T *pdp_info, MN_DUAL_SYS_E e_dual_sys)
{
    uint8 index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;

    if (PNULL == pdp_info)
    {
        return FALSE;
    }

    index = MMIAPIWWW_GetNetSettingIndex(e_dual_sys);

    link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(e_dual_sys, index);

    SCI_MEMSET(&pdp_info->connect_setting, 0x0, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
    SCI_MEMCPY(&pdp_info->connect_setting, link_setting_ptr, sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

    return TRUE;
}
/*****************************************************************************/
//  Description : Get num from URL
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_GetNumFromURL(char *str_dest, const char* str_src, char *str, BOOLEAN is_dtmf)
{
    uint8 i = 0;
    uint8 str_len = 0;
    char *str_cpy = str_dest;

    if( (PNULL == str_src) || (PNULL == str) )
    {
        return;
    }
    
    str_len = strlen(str);

    for(i = 0; i < str_len; i++)
    {
        str_src++;
    }
    while((*str_src >= '0' && *str_src <= '9')||(*str_src == '*')||(*str_src == '#')
        ||(*str_src == 'W')||(*str_src == 'P')||(*str_src == 'w')||(*str_src == 'p'))
    {
        if (is_dtmf && (*str_src == '*'))//dtmf string
        {
            *str_cpy++ = 'P';
            *str_src++;
        }
        else
        {
            *str_cpy++ = *str_src++;
        }
    }

    return;
}


/*****************************************************************************/
//     Description : Upload Open file Callback
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void Upload_OpenFileCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    uint32 file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    uint16 i = 0;
    uint16 path_len = 0;
    slim_char *file_name = PNULL;
    BOOLEAN is_add_success = FALSE;

    //SCI_TRACE_LOW:"Upload_OpenFileCallBack is_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2149_112_2_18_3_11_37_58,(uint8*)"d", is_success);

    if (PNULL == www_instance)
    {
        if (PNULL != file_array)
        {
            MMIAPIFILEARRAY_Destroy(&file_array);
        }
        return;
    }

    if (is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);

        //SCI_TRACE_LOW:"Upload_OpenFileCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2160_112_2_18_3_11_37_59,(uint8*)"d", file_num);

        if (file_num > 0 && MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data))
        {
            is_add_success = TRUE;

            //read user selected dir

            //SCI_TRACE_LOW:"Upload_OpenFileCallBack pathlen = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2168_112_2_18_3_11_37_60,(uint8*)"d", file_array_data.name_len);

            if (PNULL != www_instance->upload_param.upload_path)
            {
                SCI_FREE(www_instance->upload_param.upload_path);
                www_instance->upload_param.upload_path = PNULL;
            }

            www_instance->upload_param.upload_path = SCI_ALLOCA(file_array_data.name_len * 3 + 1);

            if (PNULL == www_instance->upload_param.upload_path)
            {
                if (PNULL != file_array)
                {
                    MMIAPIFILEARRAY_Destroy(&file_array);
                }
                return;
            }

            SCI_MEMSET(www_instance->upload_param.upload_path, 0x00, (file_array_data.name_len * 3 + 1));

            GUI_WstrToUTF8((uint8 *)www_instance->upload_param.upload_path,
                                    file_array_data.name_len * 3 + 1,
                                    file_array_data.filename,
                                    file_array_data.name_len);

            path_len = strlen((char *)www_instance->upload_param.upload_path);

            for (i = path_len - 1; i > 0; i --)
            {
                if ('\\' == *(www_instance->upload_param.upload_path + i)
                    || '/' == *(www_instance->upload_param.upload_path + i))
                {
                    break;
                }
            }

            if (i < path_len - 1)
            {
                i = i + 1;
            }

            file_name = (slim_char *)(www_instance->upload_param.upload_path + i);
        }
    }

    if (is_add_success)
    {
        MMIWWW_Netfront_SetFileSelectFullPath(www_instance->upload_param.in_select, www_instance->upload_param.upload_path);
        MMIWWW_Netfront_SetFileSelcetDisplayName(www_instance->upload_param.in_select, file_name);
    }
    else
    {
        MMIWWW_Netfront_SetFileSelectFullPath(www_instance->upload_param.in_select, PNULL);
        MMIWWW_Netfront_SetFileSelcetDisplayName(www_instance->upload_param.in_select, PNULL);
    }
    MMIWWW_Netfront_LeaveTextControlEditMode(file_name);
    //释放列表
    if (PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        file_array = PNULL;
    }
}

/*****************************************************************************/
//  Description : Upload Enter Explorer
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_UploadEnterExplorer(DPARAM param)
{
    FILEARRAY ret_array = PNULL;
    MMIFMM_FILTER_T filter = {0};
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int in_select = *(slim_int *)param;

    if (PNULL != www_instance)
    {
        www_instance->upload_param.in_select = in_select;
    }

    ret_array = MMIAPIFILEARRAY_Create();

    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

    if (FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL,
                                                                    0,
                                                                    &filter,
                                                                    FUNC_FIND_FILE_AND_FOLDER,
                                                                    FALSE,
                                                                    NULL,
                                                                    ret_array,
                                                                    Upload_OpenFileCallBack,
                                                                    FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
}

/*****************************************************************************/
//  Description : Get weather mainmenu get focus
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetfrontGetMainMenuFocusState(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_mainmenu_focus;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Set mainmenu focus state
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetfrontSetMainMenuFocusState( BOOLEAN bfocus )
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->is_mainmenu_focus = bfocus;
    }
}

/*****************************************************************************/
//  Description : Get selected mime type from content ptr
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_GetSelectedImageMIMEIndex(void)
{
    uint8 *content_type_ptr = PNULL;
    BOOLEAN ret = FALSE;
    uint32 mime_index = 0xFFFFFFFF;
    uint8 *ext = PNULL;

    ret = MMIWWW_Netfront_GetImageContentType(&content_type_ptr);

    if (ret && PNULL != content_type_ptr)
    {
        ext = (uint8*)mime_to_mime_ext((const char *)content_type_ptr);
        if (PNULL != ext)
        {
            mime_index = MMIAPIDL_MimeExtToMimeIndex((const char *)ext); /* NF mime array index */
        }
        SCI_FREE(content_type_ptr);
    }
    return mime_index;
}

/*****************************************************************************/
//  Description : Clean Usert Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_CleanUserInfo(void)
{
    MMIWWW_KEY_T key_data = {0};

    //clean netfront data
    MMIWWW_CleanNetfrontData();

    //clean input data
    MMINV_WRITE( MMINV_WWW_KEY, &key_data );

    //Set first entry flag
    MMIWWW_SetFirstEntry(TRUE);
}

/*****************************************************************************/
//  Description : Set Http Proxy
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetHTTPProxy(char *in_url)
{
#ifdef MMI_WIFI_SUPPORT
    MMIWLAN_PROXY_SETTING_DETAIL_T detail_struct = {0};
#endif
    MMIWWW_PDP_INFO_T pdp_info = {0};
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return;
    }

    MMIWWW_NetfrontGetPDPInfo(&pdp_info);

    //SCI_TRACE_LOW:"MMIWWW_SetHTTPProxy gateway=%s, gateway_len=%d, port=%d, pdp_state=%d,bear_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2357_112_2_18_3_11_38_61,(uint8*)"sdddd",pdp_info.connect_setting.gateway, pdp_info.connect_setting.gateway_len, pdp_info.connect_setting.port,www_instance->pdp_info.pdp_state, www_instance->customized_connection.bearer_type);
#ifndef WIN32
    if (MMIPDP_INTERFACE_GPRS == www_instance->customized_connection.bearer_type)
    {
        if (0 != pdp_info.connect_setting.gateway_len
            && 0 != strcmp((char *)pdp_info.connect_setting.gateway, (char *)MMIWWW_INVALID_GATEWAY))
        {
#ifndef MMIWWW_PDP_APPLET_CONNECT
            if (MMIWWW_PDP_ACTIVING != www_instance->pdp_info.pdp_state
                && MMIWWW_PDP_ACTIVE_OK != www_instance->pdp_info.pdp_state)
#endif
            {
                NFBrowser_SetHTTPProxy(www_instance->browser_instance,
                                                        (slim_char *)pdp_info.connect_setting.gateway,
                                                        (slim_int)pdp_info.connect_setting.gateway_len,
                                                        (slim_int)pdp_info.connect_setting.port,
                                                        (slim_int)pdp_info.connect_setting.port);
            }
        }
        else
        {
#ifndef MMIWWW_PDP_APPLET_CONNECT
            if (MMIWWW_PDP_ACTIVING != www_instance->pdp_info.pdp_state
                && MMIWWW_PDP_ACTIVE_OK != www_instance->pdp_info.pdp_state)
#endif
            {
                NFBrowser_SetHTTPProxy(www_instance->browser_instance,
                                                        slim_nil,
                                                        0,
                                                        0,
                                                        0);
            }
        }
    }
#ifdef MMI_WIFI_SUPPORT
    else if(MMIPDP_INTERFACE_WIFI == www_instance->customized_connection.bearer_type)
    {
        MMIAPIWIFI_GetProxySettingDetail(&detail_struct);

        //SCI_TRACE_LOW:"MMIPDP_INTERFACE_WIFI gateway=%s, is_use_proxy = %d, detail_struct.gateway, detail_struct.is_use_proxy"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2396_112_2_18_3_11_38_62,(uint8*)"sd");

        if(detail_struct.is_use_proxy && !MMIAPIWIFI_IsExistInExcludingList(in_url))
        {
            NFBrowser_SetHTTPProxy(www_instance->browser_instance,
                                                    (slim_char *)detail_struct.gateway,
                                                    (slim_int)detail_struct.gateway_len,
                                                    (slim_int)detail_struct.port,
                                                    (slim_int)detail_struct.port);
        }
        else
        {
            NFBrowser_SetHTTPProxy(www_instance->browser_instance,
                                                    slim_nil,
                                                    0,
                                                    0,
                                                    0);
        }
    }
#endif
#endif
}

/*****************************************************************************/
//  Description : Set Init User Agent
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetInitUA(const char * user_agent_ptr)
{
    MMIWWW_FreeInitUA();

    if (PNULL != user_agent_ptr)
    {
        uint32 ua_len = strlen(user_agent_ptr);

        //SCI_TRACE_LOW:"MMIWWW_SetInitUA ua_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2431_112_2_18_3_11_38_63,(uint8*)"d", ua_len);

        if (ua_len > 0 && ua_len <= MAX_WEB_URL_LEN)
        {
            s_www_user_agent_ptr = SCI_ALLOCA(ua_len + 1);

            if (PNULL != s_www_user_agent_ptr)
            {
                SCI_MEMSET(s_www_user_agent_ptr, 0x00, (ua_len + 1));

                SCI_MEMCPY(s_www_user_agent_ptr, user_agent_ptr, ua_len);
            }
        }
    }
}

/*****************************************************************************/
//  Description : Get Init User Agent
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIWWW_GetInitUA(void)
{
    return s_www_user_agent_ptr;
}

/*****************************************************************************/
//  Description : Free Init User Agent
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeInitUA(void)
{
    if (PNULL != s_www_user_agent_ptr)
    {
        SCI_FREE(s_www_user_agent_ptr);
        s_www_user_agent_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : free phone num memory
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIWWW_NetfrontGetPagememoSortType(void)
{
    uint8 pagememo_sort_type = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_WWW_PAGEMEMO_SORT_TYPE, &pagememo_sort_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        pagememo_sort_type = (uint8)MMIWWW_PAGEMEMO_SORT_BY_DATE;
        MMINV_WRITE( MMINV_WWW_PAGEMEMO_SORT_TYPE, &pagememo_sort_type );
    }

    return (uint8)pagememo_sort_type;
}

/*****************************************************************************/
//  Description : Set Search String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetSearchString(MMI_STRING_T *search_string_ptr)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    MMIWWW_FreeSearchString();

    if (PNULL != search_string_ptr && PNULL != www_instance)
    {
        if (PNULL != search_string_ptr->wstr_ptr && 0 != search_string_ptr->wstr_len)
        {
            www_instance->search_string.wstr_len = search_string_ptr->wstr_len;
            www_instance->search_string.wstr_ptr = SCI_ALLOCA((www_instance->search_string.wstr_len + 1) * sizeof(wchar));

            if (PNULL != www_instance->search_string.wstr_ptr)
            {
                SCI_MEMSET(www_instance->search_string.wstr_ptr, 0x00, ((www_instance->search_string.wstr_len + 1)*sizeof(wchar)));

                SCI_MEMCPY(www_instance->search_string.wstr_ptr, search_string_ptr->wstr_ptr, (www_instance->search_string.wstr_len * sizeof(wchar)));
            }
        }
    }
}

/*****************************************************************************/
//  Description : Get Search String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMI_STRING_T * MMIWWW_GetSearchString(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return &www_instance->search_string;
    }

    return PNULL;
}

/*****************************************************************************/
//  Description : Free Search String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeSearchString(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        if (PNULL != www_instance->search_string.wstr_ptr)
        {
            SCI_FREE(www_instance->search_string.wstr_ptr);
            www_instance->search_string.wstr_ptr = PNULL;
            www_instance->search_string.wstr_len = 0;
        }
    }
}

/*****************************************************************************/
//  Description : Set Js Promt String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetJSPromtString(slim_char *in_message, slim_char *in_value)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    uint32 len = 0;

    if (PNULL == www_instance)
    {
        return;
    }

    MMIWWW_FreeJSPromtString();

    if (PNULL != in_message)
    {
        len = strlen((char *)in_message);
        www_instance->js_promt.in_message = SCI_ALLOCA((len + 1) * sizeof(wchar));

        if (PNULL != www_instance->js_promt.in_message)
        {
            SCI_MEMSET(www_instance->js_promt.in_message, 0x00, ((len + 1) * sizeof(wchar)));

            SCI_MEMCPY(www_instance->js_promt.in_message, in_message, ((len + 1) * sizeof(wchar)));
        }
    }

    if (PNULL != in_value)
    {
        len = strlen((char *)in_value);
        www_instance->js_promt.in_value = SCI_ALLOCA((len + 1) * sizeof(wchar));

        if (PNULL != www_instance->js_promt.in_value)
        {
            SCI_MEMSET(www_instance->js_promt.in_value, 0x00, ((len + 1) * sizeof(wchar)));

            SCI_MEMCPY(www_instance->js_promt.in_value, in_value, ((len + 1) * sizeof(wchar)));
        }
    }
}

/*****************************************************************************/
//  Description : Get Js Promt String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_GetJSPromtString(slim_char  **in_message, slim_char  **in_value)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == in_message || PNULL == in_value)
    {
        return;
    }

    if (PNULL != www_instance)
    {
        *in_message = www_instance->js_promt.in_message;
        *in_value = www_instance->js_promt.in_value;
    }
}

/*****************************************************************************/
//  Description : Free Js Promt String
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeJSPromtString(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        if (PNULL != www_instance->js_promt.in_message)
        {
            SCI_FREE(www_instance->js_promt.in_message);
            www_instance->js_promt.in_message = PNULL;
        }

        if (PNULL != www_instance->js_promt.in_value)
        {
            SCI_FREE(www_instance->js_promt.in_value);
            www_instance->js_promt.in_value = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description : Set Customized Connection
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetCustomizedConnection(DPARAM param)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    MMIWWW_CUSTOMIZED_CONNECTION_T *customized_connection_ptr = (MMIWWW_CUSTOMIZED_CONNECTION_T *) param;

    if (PNULL != customized_connection_ptr && PNULL != www_instance)
    {
        uint8 default_connection_index = 0;
        MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

        //SCI_TRACE_LOW:"MMIWWW_SetCustomizedConnection is_connection_customized=%d,connection_index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2652_112_2_18_3_11_38_64,(uint8*)"dd",customized_connection_ptr->is_connection_customized, customized_connection_ptr->connection_index);

        www_instance->customized_connection.bearer_type = customized_connection_ptr->bearer_type;
        www_instance->customized_connection.dual_sys = customized_connection_ptr->dual_sys;
        dual_sys = customized_connection_ptr->dual_sys;
        
        if (customized_connection_ptr->is_connection_customized && customized_connection_ptr->connection_index < MMICONNECTION_MAX_SETTING_NUM)
        {
            default_connection_index = MMIAPIWWW_GetNetSettingIndex(dual_sys);

            www_instance->customized_connection.is_connection_customized = customized_connection_ptr->is_connection_customized;
            www_instance->customized_connection.connection_index = customized_connection_ptr->connection_index;
            www_instance->customized_connection.default_connection_index = default_connection_index;//save old connection index

            MMIWWW_SetNetSettingIndex(dual_sys, www_instance->customized_connection.connection_index);//set new index
        }
    }
}

/*****************************************************************************/
//  Description : Get Customized Connection
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetCustomizedConnection(uint8 * connection_index_ptr)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    BOOLEAN is_connection_customized = FALSE;

    if (PNULL == connection_index_ptr)
    {
        return FALSE;
    }

    if (PNULL != www_instance && www_instance->customized_connection.is_connection_customized)
    {
        is_connection_customized = www_instance->customized_connection.is_connection_customized;
        *connection_index_ptr = www_instance->customized_connection.connection_index;
    }

    //SCI_TRACE_LOW:"MMIWWW_GetCustomizedConnection is_connection_customized=%d,connection_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2692_112_2_18_3_11_39_65,(uint8*)"dd",is_connection_customized, *connection_index_ptr);

    return is_connection_customized;
}

/*****************************************************************************/
//  Description : Reset Net Connect
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_ResetNetConnect(void)
{
    uint8 index = 0;
    BOOLEAN is_connection_customized = FALSE;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if(PNULL == www_instance)
    {
        //SCI_TRACE_LOW:"MMIWWW_ResetNetConnect null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2709_112_2_18_3_11_39_66,(uint8*)"");
        return;
    }

    is_connection_customized = MMIWWW_GetCustomizedConnection(&index);

    if (!is_connection_customized)
    {
        if (MMIAPIPHONE_IsSimAvailable(www_instance->customized_connection.dual_sys)) //TODO
        {
            MMIWWW_NETDISCONNECT();
            MMIWWW_NETCONNECT();
        }
        MMIWWW_SetHTTPProxy(PNULL);//set http proxy
    }
    else
    {
        uint8 default_connection_index = 0;
        MN_DUAL_SYS_E dual_sys = www_instance->customized_connection.dual_sys;

        default_connection_index = MMIAPIWWW_GetNetSettingIndex(dual_sys);

        www_instance->customized_connection.default_connection_index = default_connection_index;//set default index

    }
}

/*****************************************************************************/
//  Description : Reset Connection Settings
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_ResetConnectionSettings(void)
{
    uint8 index = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance)
    {
        return;
    }

    if (www_instance->customized_connection.is_connection_customized)
    {
        dual_sys = www_instance->dual_sys;
    }
    
    index = MMIAPIWWW_GetNetSettingIndex(dual_sys);

    //SCI_TRACE_LOW:"MMIWWW_ResetConnectionSettings index=%d,is_connection_customized=%d,connection_index=%d,default=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2761_112_2_18_3_11_39_67,(uint8*)"dddd",index, www_instance->customized_connection.is_connection_customized,www_instance->customized_connection.connection_index,www_instance->customized_connection.default_connection_index);

    if (www_instance->customized_connection.is_connection_customized
        && www_instance->customized_connection.default_connection_index < MMICONNECTION_MAX_SETTING_NUM)
    {
        MMIWWW_SetNetSettingIndex(dual_sys, www_instance->customized_connection.default_connection_index);//save default index
    }
}

/*****************************************************************************/
//  Description : Set Browser Reload Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetBrowserReloadFlag(uint8 act_index, BOOLEAN is_reload)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    //SCI_TRACE_LOW:"MMIWWW_SetBrowserReloadFlag act_index=%d,is_reload=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2778_112_2_18_3_11_39_68,(uint8*)"dd", act_index, is_reload);

    if (PNULL != www_instance)
    {
        if (act_index < MMIWWW_BROWSER_MAX_TAB)
        {
            www_instance->is_reload[act_index] = is_reload;
        }
        else //set all tab
        {
            uint32 i = 0;

            for (i = 0; i <MMIWWW_BROWSER_MAX_TAB; i++)
            {
                www_instance->is_reload[i] = is_reload;
            }
        }
    }
}

/*****************************************************************************/
//  Description : Get Browser Reload Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBrowserReloadFlag(uint8 act_index)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (act_index < MMIWWW_BROWSER_MAX_TAB && PNULL != www_instance)
    {
        return www_instance->is_reload[act_index];
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Set Browser Focus Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetBrowserFocusFlag(BOOLEAN is_focus)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    //SCI_TRACE_LOW:"MMIWWW_SetBrowserFocusFlag is_focus=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2822_112_2_18_3_11_39_69,(uint8*)"d", is_focus);

    if (PNULL != www_instance)
    {
        www_instance->is_focus = is_focus;
    }
}

/*****************************************************************************/
//  Description : Get Browser Focus Flag
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBrowserFocusFlag(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_focus;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Set Browser stop Flag
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetBrowserstopFlag(BOOLEAN is_stop)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        www_instance->is_stop_nf = is_stop;
    }
}

/*****************************************************************************/
//  Description : Get Browser stop Flag
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBrowserstopFlag(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->is_stop_nf;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : handle message wether setup call
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWwwMakeCall(MMI_WIN_ID_T      win_id, 
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            uint8 *phone_num = PNULL;
            MMIPUB_INFO_T * win_info_ptr = PNULL;

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                phone_num = (uint8 *)win_info_ptr->user_data;

                if (PNULL != phone_num)
                {
                    MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, phone_num, strlen((char *)phone_num), PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
                }
            }
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            uint8 *phone_num = PNULL;
            MMIPUB_INFO_T * win_info_ptr = PNULL;

            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if (PNULL != win_info_ptr)
            {
                phone_num = (uint8 *)win_info_ptr->user_data;

                if (PNULL != phone_num)
                {
                    SCI_FREE(phone_num);
                }
            }
        }
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Handle special scheme
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_HandleExScheme(uint8 *in_url)
#if 1//@fen.xie
{
    return MMIAPIBROWSER_HandleExScheme(in_url);
}
#else
{
    uint16 url_len = 0;
    uint8 *str_ptr = PNULL;
    uint8 *temp_str = PNULL;
    MMIWWW_CC_INFO_T cc_info = {0};

    if (PNULL == in_url)
    {
        return FALSE;
    }
    
    while(' ' == *in_url)
    {
        in_url++;
    }

    url_len = strlen((char *)in_url);

    if (0 == url_len)
    {
        //SCI_TRACE_LOW:"MMIWWW_HandleExScheme: in_url empty"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2972_112_2_18_3_11_39_70,(uint8*)"");
        return FALSE;
    }
    
    str_ptr = SCI_ALLOCA(url_len + 1);
    if (PNULL == str_ptr)
    {
        //SCI_TRACE_LOW:"MMIWWW_HandleExScheme: Err No memory "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2979_112_2_18_3_11_39_71,(uint8*)"");
        return FALSE;
    }
    temp_str= SCI_ALLOCA(url_len + 1);
    if (PNULL == temp_str)
    {
        //SCI_TRACE_LOW:"MMIWWW_HandleExScheme: Err No memory "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_2985_112_2_18_3_11_39_72,(uint8*)"");
        SCI_FREE(str_ptr);
        return FALSE;
    }

    SCI_MEMSET(str_ptr, 0, (url_len + 1));
    SCI_MEMSET(temp_str, 0, (url_len + 1));
    SCI_MEMCPY(temp_str, in_url, url_len);

    temp_str = (uint8 *)MMIAPICOM_Strlower((char *)temp_str);

    if(strncmp((char *)temp_str, "tel:", strlen("tel:")) == 0)
    {
        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "tel:", FALSE);
        cc_info.cc_option = MMIWWW_CC_OPTION_VOICE | MMIWWW_CC_OPTION_IP;/*lint !e655*/
        cc_info.phone_num_ptr = str_ptr;
        MMIWWW_OpenCCSelectWin(&cc_info);
        SCI_FREE(str_ptr);
    }
    else if(strncmp((char *)temp_str, "wtai://wp/mc", strlen((char *)in_url)) == 0)
    {
        MMIAPIIDLE_EnterDialWin();
        SCI_FREE(str_ptr);
    }
    else if(strncmp((char *)temp_str, "wtai://wp/mc;", strlen("wtai://wp/mc;")) == 0)
    {
        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "wtai://wp/mc;", FALSE);
        cc_info.cc_option = MMIWWW_CC_OPTION_VOICE | MMIWWW_CC_OPTION_IP | MMIWWW_CC_OPTION_VIDEO;/*lint !e655*/
        cc_info.phone_num_ptr = str_ptr;
        MMIWWW_OpenCCSelectWin(&cc_info);
        SCI_FREE(str_ptr);
    }
    else if(strncmp((char *)temp_str, "wtai://wp/sd;", strlen("wtai://wp/sd;")) == 0)
    {
        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "wtai://wp/sd;", TRUE);
        cc_info.cc_option = MMIWWW_CC_OPTION_VOICE | MMIWWW_CC_OPTION_IP;/*lint !e655*/
        cc_info.phone_num_ptr = str_ptr;
        MMIWWW_OpenCCSelectWin(&cc_info);
        SCI_FREE(str_ptr);
    }
    else if(strncmp((char *)temp_str, "wtai://wp/ap", strlen((char *)in_url)) ==0)
    {
        MMIPB_BCD_NUMBER_T pb_bcd = {0};

        //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
        MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
        SCI_FREE(str_ptr);
    }
    else if(strncmp((char *)temp_str, "wtai://wp/ap;", strlen("wtai://wp/ap;")) ==0)
    {
        MMIPB_BCD_NUMBER_T pb_bcd = {0};
        MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
        
        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "wtai://wp/ap;", FALSE);
        MMIAPICOM_GenPartyNumber(str_ptr, strlen((char *)str_ptr), &party_num);
        pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
        pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
        SCI_MEMCPY( pb_bcd.number,  party_num.bcd_num, pb_bcd.number_len );
        //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
        MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
        SCI_FREE(str_ptr);
    }
    else if(strncmp((char *)temp_str, "sms:", strlen("sms:")) ==0)
    {
        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "sms:", FALSE);
        MMIAPISMS_WriteNewMessage( MN_DUAL_SYS_MAX, PNULL, str_ptr, strlen((char *)str_ptr) );
        SCI_FREE(str_ptr);
    }
    else if(strncmp((char *)temp_str, "smsto:", strlen("smsto:")) ==0)
    {
        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "smsto:", FALSE);
        MMIAPISMS_WriteNewMessage( MN_DUAL_SYS_MAX, PNULL, str_ptr, strlen((char *)str_ptr) );
        SCI_FREE(str_ptr);
    }
    else if(0 != strncmp((char *)temp_str, "mms://", strlen("mms://")) && strncmp((char *)temp_str, "mms:", strlen("mms:")) ==0)
    {
#ifdef MMS_SUPPORT
        MMI_STRING_T string_to = {0};

        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "mms:", FALSE);
        MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
        string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(strlen((char *)str_ptr) * sizeof(wchar));
        MMI_STRNTOWSTR(string_to.wstr_ptr, strlen((char *)str_ptr), (uint8 *)str_ptr, strlen((char *)str_ptr), strlen((char *)str_ptr));
        string_to.wstr_len = strlen((char *)str_ptr);
        MMIAPIMMS_AnswerMMS(MN_DUAL_SYS_MAX,PNULL,&string_to);
        SCI_FREE(string_to.wstr_ptr); 
        SCI_FREE(str_ptr);
#else
        MMIWWW_OpenWWWAlertWin(PNULL, TXT_COMM_TYPE_NOT_SUPPORT, PNULL, IMAGE_PUBWIN_WARNING);
        SCI_FREE(temp_str);
        SCI_FREE(str_ptr);
        return TRUE;
#endif
    }
    else if( strncmp((char *)temp_str, "mmsto:", strlen("mmsto:")) ==0 )
    {
#ifdef MMS_SUPPORT
        MMI_STRING_T string_to = {0};

        MMIWWW_Netfront_GetNumFromURL((char *)str_ptr, (char *)in_url, "mmsto:", FALSE);
        MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
        string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(strlen((char *)str_ptr) * sizeof(wchar));
        MMI_STRNTOWSTR(string_to.wstr_ptr, strlen((char *)str_ptr), (uint8 *)str_ptr, strlen((char *)str_ptr), strlen((char *)str_ptr));
        string_to.wstr_len = strlen((char *)str_ptr);
        MMIAPIMMS_AnswerMMS(MN_DUAL_SYS_MAX,PNULL,&string_to);
        SCI_FREE(string_to.wstr_ptr);
        SCI_FREE(str_ptr);
#else
        MMIWWW_OpenWWWAlertWin(PNULL, TXT_COMM_TYPE_NOT_SUPPORT, PNULL, IMAGE_PUBWIN_WARNING);
        SCI_FREE(temp_str);
        SCI_FREE(str_ptr);
        return TRUE;
#endif
    }
    else if( (strncmp((char *)temp_str, "mail:", strlen("mail:")) ==0) ||
            (strncmp((char *)temp_str, "mailto:", strlen("mailto:")) ==0) )
    {
#ifdef PUSH_EMAIL_SUPPORT
        if( strncmp((char *)temp_str, "mail:", strlen("mail:")) ==0 )
        {
            MMIAPIMAIL_Enter(MAIL_ENTER_NEWMAIL, PNULL);
        }
        else if (strncmp((char *)temp_str, "mailto:", strlen("mailto:")) ==0)
        {
            char *address = PNULL;

            address = (char *)(temp_str + strlen("mailto:"));
            MMIAPIMAIL_Enter(MAIL_ENTER_NEWMAIL, strlen(address) > 0 ? address : PNULL);
        }
        else
        {
            //Do nothind
            ;
        }
#else
        MMIWWW_OpenWWWAlertWin(PNULL, TXT_COMM_TYPE_NOT_SUPPORT, PNULL, IMAGE_PUBWIN_WARNING);
#endif /* PUSH_EMAIL_SUPPORT  */


        SCI_FREE(temp_str);
        SCI_FREE(str_ptr);
        return TRUE;
    }
#ifdef STREAMING_HS_SUPPORT // chenxiang 20100223
    else if( strncmp((char *)temp_str, "rtsp://", strlen("rtsp://")) ==0 )
    {
        //SCI_TRACE_MID:"mmiwww smp temp_str = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3131_112_2_18_3_11_39_73,(uint8*)"s", temp_str);
        MMIAPISMP_StartUp(SMP_STARTUP_AUTO, (char *)in_url);
        SCI_FREE(str_ptr);
    }
#endif // STREAMING_HS_SUPPORT

#ifdef MMI_STREAMING_PLAYER_SUPPORT
    else if( (strncmp((char *)temp_str, "rtsp://", strlen("rtsp://")) ==0))
    {        
        MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_NET,PNULL,0,in_url);        
        SCI_FREE(str_ptr);
    }
#endif
    else if((strncmp((char *)temp_str, "http:", strlen("http:")) ==0) || (strncmp((char *)temp_str, "https:", strlen("https:")) ==0))
    {//使用WLAN上网时，使用代理的处理
        MMIWWW_SetHTTPProxy((char *)temp_str);
        SCI_FREE(temp_str);
        SCI_FREE(str_ptr);
        return FALSE;
    }
    else
    {
        SCI_FREE(temp_str);
        SCI_FREE(str_ptr);
        return FALSE;
    }
    SCI_FREE(temp_str);
    return TRUE;
}
#endif

#ifdef USE_EXT_DISPLAY_BUFFER
/*****************************************************************************/
//  Description : Handle special scheme
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeDisplayBuf(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance && PNULL != www_instance->display_buf_ptr)    
    {
        SCI_FREE(www_instance->display_buf_ptr);

        www_instance->display_buf_ptr = PNULL;
        www_instance->display_buf_len = 0;
    }
}
#endif

/*****************************************************************************/
//  Description : Set Auth Param
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetAuthParam(slim_char *in_url, slim_char *in_realm, slim_char *in_user, slim_char *in_pass)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    uint32 len = 0;

    if (PNULL == www_instance)
    {
        return;
    }

    MMIWWW_FreeAuthParam();

    if (PNULL != in_url)
    {
        len = strlen((char *)in_url);
        www_instance->auth_param.in_url = SCI_ALLOCA((len + 1) * sizeof(wchar));

        if (PNULL != www_instance->auth_param.in_url)
        {
            SCI_MEMSET(www_instance->auth_param.in_url, 0x00, ((len + 1) * sizeof(wchar)));

            SCI_MEMCPY(www_instance->auth_param.in_url, in_url, ((len + 1) * sizeof(wchar)));
        }
    }

    if (PNULL != in_realm)
    {
        len = strlen((char *)in_realm);
        www_instance->auth_param.in_realm = SCI_ALLOCA((len + 1) * sizeof(wchar));

        if (PNULL != www_instance->auth_param.in_realm)
        {
            SCI_MEMSET(www_instance->auth_param.in_realm, 0x00, ((len + 1) * sizeof(wchar)));

            SCI_MEMCPY(www_instance->auth_param.in_realm, in_realm, ((len + 1) * sizeof(wchar)));
        }
    }

    if (PNULL != in_user)
    {
        len = strlen((char *)in_user);
        www_instance->auth_param.in_user = SCI_ALLOCA((len + 1) * sizeof(wchar));

        if (PNULL != www_instance->auth_param.in_user)
        {
            SCI_MEMSET(www_instance->auth_param.in_user, 0x00, ((len + 1) * sizeof(wchar)));

            SCI_MEMCPY(www_instance->auth_param.in_user, in_user, ((len + 1) * sizeof(wchar)));
        }
    }

    if (PNULL != in_pass)
    {
        len = strlen((char *)in_pass);
        www_instance->auth_param.in_pass = SCI_ALLOCA((len + 1) * sizeof(wchar));

        if (PNULL != www_instance->auth_param.in_pass)
        {
            SCI_MEMSET(www_instance->auth_param.in_pass, 0x00, ((len + 1) * sizeof(wchar)));

            SCI_MEMCPY(www_instance->auth_param.in_pass, in_pass, ((len + 1) * sizeof(wchar)));
        }
    }
}

/*****************************************************************************/
//  Description : Get Auth Param
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_GetAuthParam(slim_char **in_url, slim_char **in_realm, slim_char **in_user, slim_char **in_pass)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == in_url || PNULL == in_realm || PNULL == in_user || PNULL == in_pass)
    {
        return;
    }

    if (PNULL != www_instance)
    {
        *in_url = www_instance->auth_param.in_url;
        *in_realm = www_instance->auth_param.in_realm;
        *in_user = www_instance->auth_param.in_user;
        *in_pass = www_instance->auth_param.in_pass;
    }
}

/*****************************************************************************/
//  Description : Free Auth Param
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeAuthParam(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        if (PNULL != www_instance->auth_param.in_url)
        {
            SCI_FREE(www_instance->auth_param.in_url);
            www_instance->auth_param.in_url = PNULL;
        }

        if (PNULL != www_instance->auth_param.in_realm)
        {
            SCI_FREE(www_instance->auth_param.in_realm);
            www_instance->auth_param.in_realm = PNULL;
        }

        if (PNULL != www_instance->auth_param.in_user)
        {
            SCI_FREE(www_instance->auth_param.in_user);
            www_instance->auth_param.in_user = PNULL;
        }

        if (PNULL != www_instance->auth_param.in_pass)
        {
            SCI_FREE(www_instance->auth_param.in_pass);
            www_instance->auth_param.in_pass = PNULL;
        }
    }
}

/*****************************************************************************/
// Description : Set image displaying mode.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetImageDisplayMode(NFBrowserImageMode in_mode)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    //SCI_TRACE_LOW:"MMIWWW_SetImageDisplayMode: in_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3316_112_2_18_3_11_40_74,(uint8*)"d", in_mode);

    if (PNULL != www_instance)
    {
        www_instance->image_display_mode = in_mode;
    }
}

/*****************************************************************************/
// Description : Get image displaying mode.
// Global resource dependence :
// Author: kun.yu
// Note: 
/*****************************************************************************/
PUBLIC NFBrowserImageMode MMIWWW_GetImageDisplayMode(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        return www_instance->image_display_mode;
    }

    return SETTINGS_IMAGES_DISPLAY;
}

/*****************************************************************************/
// Description : Convert str to Escape str(%X)
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: be sure of the range of dst
/*****************************************************************************/
PUBLIC char* MMIAPICOM_StrToEscapeStr(char *dst, const char *src)
{
    char *ret_str = dst;

    if (PNULL == dst || PNULL == src)
    {
        return dst;
    }

    while (*src)
    {
        if (0x7f < *(uint8 *)src) /* is ASCII*/
        {
            *dst++ = '%';
            sprintf((char *)dst, "%02X", *((uint8 *)src)++);/*lint !e52*/
            dst +=2;
        }
        else
        {
            *dst++ = *src++;
        }
    }

    *dst = 0;

    return ret_str;
}

/*****************************************************************************/
// Description : Set copy mode. 
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetCopyMode(MMIWWW_COPY_MODE_E in_mode)
{
#ifdef TOUCH_PANEL_SUPPORT
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    //SCI_TRACE_LOW:"MMIWWW_SetCopyMode: in_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3381_112_2_18_3_11_40_75,(uint8*)"d", in_mode);

    if (PNULL != www_instance)
    {

        www_instance->copy_mode = in_mode;
    }
#endif /* TOUCH_PANEL_SUPPORT */
}

/*****************************************************************************/
// Description : Get copy mode.
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC MMIWWW_COPY_MODE_E MMIWWW_GetCopyMode(void)
{
    MMIWWW_COPY_MODE_E copy_mode = MMIWWW_COPY_MODE_NONE;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance)
    {
        copy_mode = www_instance->copy_mode;
    }

    //SCI_TRACE_LOW:"MMIWWW_GetCopyMode: copy_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3405_112_2_18_3_11_40_76,(uint8*)"d", copy_mode);

    return copy_mode;
}

/*****************************************************************************/
// Description : get valid file name 
// Global resource dependence : 
// Author: Jiaoyou.wu
// Note:
/*****************************************************************************/ 
PUBLIC void MMIWWW_GetValidFileName(wchar *file_name, uint32 file_len, wchar *new_file_name, uint32 new_file_len)
{
    const uint8 invalid_char[] = {'/', '*', '?', '"', '<', '>', '|'};
    uint32 chr_cnt = 0;
    uint32 i = 0;
    uint32 j = 0;

    if (PNULL == file_name || PNULL == new_file_name)
    {
        return;
    }

    for (i = 0; i < file_len; i++)
    {
        for (j = 0; j < sizeof(invalid_char); j++)
        {
            if (file_name[i] == invalid_char[j])
            {
                break;
            }
        }

        if (j == sizeof(invalid_char)) /* Not invalid char*/
        {
            new_file_name[chr_cnt++] = file_name[i];
            if (chr_cnt >= new_file_len)
            {
                break;
            }
        }
    }

    return;
}

/*****************************************************************************/
// Description : Get User input msg
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_GetUserInputMSG(MMI_USER_INPUT_MESSAGE_T *input_msg)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance && PNULL != input_msg)
    {
        SCI_MEMCPY(input_msg, &www_instance->user_input_msg, sizeof(MMI_USER_INPUT_MESSAGE_T))
    }

    //SCI_TRACE_LOW:"MMIWWW_GetUserInputMSG"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3462_112_2_18_3_11_40_77,(uint8*)"");
}

/*****************************************************************************/
// Description : Set User input msg
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SetUserInputMSG(MMI_USER_INPUT_MESSAGE_T *input_msg)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance && PNULL != input_msg)
    {
        SCI_MEMCPY(&www_instance->user_input_msg, input_msg, sizeof(MMI_USER_INPUT_MESSAGE_T))
    }

    //SCI_TRACE_LOW:"MMIWWW_SetUserInputMSG"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3478_112_2_18_3_11_40_78,(uint8*)"");
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_FreeFirstURL(void)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL != www_instance && PNULL != www_instance->first_url)    
    {
        SCI_FREE(www_instance->first_url);

        www_instance->first_url = PNULL;
    }
}

/*****************************************************************************/
//  Description : Check availability of Netfront Timer
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_CheckTimerDisable(void)
{
    slim_int index = 0;
    slim_int tab_num = 0; 
    BOOLEAN is_all_page_load_finish = TRUE;
    MMIWWW_INSTANCE_T *www_instance    = MMIWWW_INSTANCE();

    //SCI_TRACE_LOW:"MMIAPIWWW_CheckTimerDisable"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3508_112_2_18_3_11_40_79,(uint8*)"");

    if(PNULL == www_instance)
    {
        //SCI_TRACE_LOW:"MMIAPIWWW_CheckTimerDisable No instance"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3512_112_2_18_3_11_40_80,(uint8*)"");
        return;
    }

    tab_num = MMIWWW_Netfront_GetWinNum();

    if(tab_num > MMIWWW_BROWSER_MAX_TAB || tab_num <= 0)
    {
        //SCI_TRACE_LOW:"MMIAPIWWW_CheckTimerDisable tab error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3520_112_2_18_3_11_40_81,(uint8*)"");
        return;
    }

    for(index = 0; index < tab_num; index++)
    {
        if(MMIWWW_STATE_PAGE_LOADING == www_instance->www_state[index])
        {
            is_all_page_load_finish = FALSE;
            break;
        }
    }

    if(!MMIDEFAULT_IsBacklightOn() && is_all_page_load_finish)
    {
        www_instance->is_disable_timer = TRUE;
    }
    else
    {
        www_instance->is_disable_timer = FALSE;
        NFBrowser_Progress(www_instance->browser_instance);
    }
}

/*****************************************************************************/
//  Description : Get Homepage
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_GetHomepageURL(char *url, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != url)
    {
        MMINV_READ(MMINV_WWW_HOMEPAGE, url, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWWW_CMCC_URL);
            len =  MIN(len, max_len);
            SCI_MEMCPY(url, MMIWWW_CMCC_URL, len);
            MMIWWW_SetHomepageURL(MMIWWW_CMCC_URL);
        }
    }
}

/*****************************************************************************/
//  Description : Get Homepage
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetHomepageURL(char *url)
{
    char *temp_url = PNULL;
    uint16 len = 0;

    temp_url = SCI_ALLOCA(MAX_WEB_URL_LEN + 1);

    if (PNULL == temp_url)
    {
        //SCI_TRACE_LOW:"MMIWWW_SetHomepageURL No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_FUNC_3579_112_2_18_3_11_40_82,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_url, 0, MAX_WEB_URL_LEN + 1);

    if (PNULL != url)
    {
        len = strlen(url);
        len =  MIN(len, MAX_WEB_URL_LEN);
        SCI_MEMCPY(temp_url, url, len);
    }

    MMINV_WRITE(MMINV_WWW_HOMEPAGE, temp_url);
    SCI_FREE(temp_url);
}


/*****************************************************************************/
//  Description : Wstricmp
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMIWWW_Wstricmp(const wchar * string1, const wchar * string2)
{
#define MMIWWW_ERR_BAD_PARAM    (-2)
    wchar * str1_temp = PNULL;
    wchar * str2_temp = PNULL;
    uint16 str1_len = 0;
    uint16 str2_len = 0;
    int ret = 0;


    if (PNULL == string1 || PNULL == string2)
    {
        return MMIWWW_ERR_BAD_PARAM;
    }

    str1_len = MMIAPICOM_Wstrlen(string1) + 1;
    str2_len = MMIAPICOM_Wstrlen(string2) + 1;

    str1_temp = SCI_ALLOCA(str1_len * sizeof(wchar));
    if (PNULL == str1_temp)
    {
        return MMIWWW_ERR_BAD_PARAM;
    }

    str2_temp = SCI_ALLOCA(str2_len * sizeof(wchar));
    if (PNULL == str2_temp)
    {
        SCI_FREE(str1_temp);
        return MMIWWW_ERR_BAD_PARAM;
    }

    SCI_MEMSET(str1_temp, 0, sizeof(wchar) * str1_len);
    SCI_MEMSET(str2_temp, 0, sizeof(wchar) * str2_len);
    MMIAPICOM_Wstrncpy(str1_temp, string1, str1_len); //Including '\0'
    MMIAPICOM_Wstrncpy(str2_temp, string2, str2_len);
    MMIAPICOM_Wstrupper(str1_temp);
    MMIAPICOM_Wstrupper(str2_temp);
    ret = MMIAPICOM_Wstrcmp(str1_temp, str2_temp);

    SCI_FREE(str1_temp);
    SCI_FREE(str2_temp);
    return ret;
}

/*****************************************************************************/
//  Description : Set Font Setting
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetFontSetting(slim_int font_set)
{
    MMIWWW_INSTANCE_T * www_instance = MMIWWW_INSTANCE();
    if(PNULL == www_instance)
    {
        return;
    }

    www_instance->peer_data.fFontSetting = font_set;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetIsCreateNewWin(void)
{
    BOOLEAN is_cretae = TRUE;
    MMIWWW_INSTANCE_T * www_instance = PNULL;

    www_instance = MMIWWW_INSTANCE();

    if(PNULL == www_instance)
    {
        return TRUE;
    }

    is_cretae = !www_instance->is_in_cur_win;

    return is_cretae;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetIsCreateNewWin(BOOLEAN is_new_win)
{
    MMIWWW_INSTANCE_T * www_instance = PNULL;

    www_instance = MMIWWW_INSTANCE();

    if(PNULL == www_instance)
    {
        return;
    }

    www_instance->is_in_cur_win = !is_new_win;
}

/*****************************************************************************/
//  Description : Set default bookmark
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_SetDefaultBookmartk(void)
{
    MMIWWW_INSTANCE_T *www_instance = PNULL;

    www_instance = MMIWWW_INSTANCE();
    
    MMIWWW_SetDefaultBookmartk(www_instance);
}
#endif //#ifdef BROWSER_SUPPORT_NETFRONT



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
