/*****************************************************************************
** File Name:      mmibrowser_wintable.c                                     *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
#include "mmi_app_browser_trc.h"
#ifdef BROWSER_SEARCH_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guimenu.h"
#include "guibutton.h"
#include "guires.h"
#include "guiform.h"
#include "cafcontrol.h"
#include "gui_ucs2b_converter.h"
#include "mmi_resource.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiidle_export.h"
#include "mmi_mainmenu_export.h"
#include "mmibrowser_id.h"
#include "browser_text.h"
#include "mmibrowser_menutable.h"
#include "cfl_wchar.h"
#include "mmibrowser_export.h"
#include "browser_image.h"
#include "mmisms_export.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_wintable.h"
#include "mmibrowser_nv.h"
#include "cfl_wchar.h"
#include "mmibrowser_func.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef BROWSER_SEARCH_SUPPORT
#define BRSCFG_BAIDU_SEARCH_HEAD    ("http://m.baidu.com/s?word=")//wap
#define BRSCFG_SOSO_SEARCH_HEAD    ("http://fwd.wap.soso.com:8080/forward.jsp?bid=417&key=")//wap
#define BRSCFG_BAIDU_SEARCH_TAIL     ("&from=1214a")
#define BRSCFG_SOSO_SEARCH_TAIL     ("&cd_f=70373")

#if 0
#define BRSCFG_CMCC_SEARCH_HEAD   ("http://s.139.com/search.do?q=")
#define BRSCFG_GOOGLE_SEARCH_HEAD   ("http://www.google.cn/search?hl=zh-CN&q=")
#define BRSCFG_YAHOO_SEARCH_HEAD    ("http://one.cn.yahoo.com/s?p=")
#define BRSCFG_CMCC_UTF8_SEARCH_TAIL     ("&category=downloadable%7Cweb%7Cbrowseable&tid=2123%2C2124%2C2125%2C2126&fr=portalcustom2")
#define BRSCFG_CMCC_GB2312_SEARCH_TAIL     ("&category=downloadable%7Cweb%7Cbrowseable&tid=2123%2C2124%2C2125%2C2126&fr=portalcustom2")
#define BRSCFG_GOOGLE_UTF8_SEARCH_TAIL     ("&ie=utf-8&oe=utf-8")
#define BRSCFG_GOOGLE_GB2312_SEARCH_TAIL     ("&ie=gb2312&oe=gb2312")
#define BRSCFG_YAHOO_UTF8_SEARCH_TAIL     ("&ei=UTF-8")
#define BRSCFG_YAHOO_GB2312_SEARCH_TAIL     ("&ei=gb2312")
#endif
#endif
     
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//the order must match with MMIBROWSER_SEARCH_ENGINE_MODE_E
LOCAL const  MMIBROWSER_SEARCH_INFO_T s_search_info[MMIBROWSER_SEARCH_ENGINE_MAX_NUM] = 
{
    {TXT_MAINMENU_ICON_BAIDU, IMAGE_BROWSER_LOG_BAIDU, BRSCFG_BAIDU_SEARCH_HEAD, BRSCFG_BAIDU_SEARCH_TAIL},
    {TXT_WWW_SOSO, IMAGE_BROWSER_LOG_SOSO, BRSCFG_SOSO_SEARCH_HEAD, BRSCFG_SOSO_SEARCH_TAIL},
    //{TXT_WWW_CMCC_SEARCH, IMAGE_BROWSER_LOG_CMCC, BRSCFG_CMCC_SEARCH_HEAD, BRSCFG_CMCC_UTF8_SEARCH_TAIL},
    //{TXT_WWW_YAHOO, IMAGE_BROWSER_LOG_YAHOO, BRSCFG_YAHOO_SEARCH_HEAD, BRSCFG_YAHOO_UTF8_SEARCH_TAIL},
    //{TXT_WWW_GOOGLE, IMAGE_BROWSER_LOG_GOOGLE, BRSCFG_GOOGLE_SEARCH_HEAD, BRSCFG_GOOGLE_UTF8_SEARCH_TAIL},
};

LOCAL MMIBROWSER_MATCH_KEY_T s_match_key_t = {0};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init the reference infomation of brower position
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void InitBrwSearchMain(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : display search icon
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySearchLog(MMI_CTRL_ID_T ctrl_id);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : the callback of search button
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SearchButtonCallBack(void);
#endif

/*****************************************************************************/
//  Description : handle edit url window message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSearchMainWinMsg(
                                   MMI_WIN_ID_T	win_id, 
                                   MMI_MESSAGE_ID_E	msg_id, 
                                   DPARAM		param
                                   );

/*****************************************************************************/
//  Description : handle search list window message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSearchListWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           );


/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIBROWSER_SEARCH_MAIN_WIN_TAB) = 
{
    WIN_FUNC((uint32)BrwHandleSearchMainWinMsg),
    WIN_ID(MMIBROWSER_SEARCH_MAIN_WIN_ID),
    WIN_STATUSBAR,

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_BROWSER_LOG_BAIDU, MMIBROWSER_SEARCH_MAIN_LOG_CTRL_ID, MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMIBROWSER_MAX_KEYNAME_LEN, 
                    MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID, MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID),
#ifdef TOUCH_PANEL_SUPPORT
    CHILD_BUTTON_CTRL(FALSE, IMAGE_BROWSER_SEARCH, MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID, MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID),
#endif
    CHILD_LIST_CTRL(TRUE, GUILIST_RADIOLIST_E, MMIBROWSER_SEARCH_MAIN_LIST_CTRL_ID, MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_WWW_SUBMENU_HISTORY, TXT_NULL,MMICOMMON_SOFTKEY_CTRL_ID,MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_WWW_SUBMENU_HISTORY, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN
}; 


WINDOW_TABLE(BRW_SEARCHLIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)BrwHandleSearchListWinMsg),  
    WIN_ID(MMIBROWSER_SEARCH_LIST_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_BROWSER_SEARCH_HISTORY),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBROWSER_HISTORY_LIST_CTRL_ID),
    
    END_WIN
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get search_engine
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_SEARCH_ENGINE_MODE_E MMIBROWSER_GetSearchEngine(void)
{
    MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine = MMIBROWSER_SEARCH_ENGINE_DEFAULT;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_BROWSER_SEARCH_ENGINE, &search_engine, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        search_engine = MMIBROWSER_SEARCH_ENGINE_DEFAULT;
        MMINV_WRITE(MMINV_BROWSER_SEARCH_ENGINE, &search_engine);
    }
    
    if (search_engine >= MMIBROWSER_SEARCH_ENGINE_MAX_NUM)//protect
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetSearchEngine search_engine[err] = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_196_112_2_18_2_6_38_299,(uint8*)"d", search_engine);
        search_engine = MMIBROWSER_SEARCH_ENGINE_DEFAULT;//default
    }
    
    return search_engine;
}

/*****************************************************************************/
//  Description : Set search_engine
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetSearchEngine(MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine)
{        
    MMINV_WRITE(MMINV_BROWSER_SEARCH_ENGINE, &search_engine);    
}

/*****************************************************************************/
// 	Description : clean the key info in NV
//	Global resource dependence : none
//   Author: fen.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_CleanKeyInfo(void)
{
    MMIBROWSER_KEY_T key_data = {0};
    
    MMINV_WRITE( MMINV_BROWSER_KEY, &key_data );
    
}

/*****************************************************************************/
//  Description : get the key info in NV 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_GetKeyInfo(MMIBROWSER_KEY_T *key_data_ptr)
{
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
    
    if (PNULL == key_data_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetKeyInfo:key_data_ptr PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_234_112_2_18_2_6_38_300,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(key_data_ptr, 0x00, sizeof(MMIBROWSER_KEY_T));
    MMINV_READ( MMINV_BROWSER_KEY, key_data_ptr, nv_value );
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        SCI_MEMSET(key_data_ptr, 0x00, sizeof(MMIBROWSER_KEY_T));
        MMINV_WRITE( MMINV_BROWSER_KEY, key_data_ptr );
    }
}

/*****************************************************************************/
//  Description : Set the key info in NV 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKeyInfo(MMIBROWSER_KEY_T *key_data_ptr)
{        
    if (key_data_ptr != PNULL)
    {
        MMINV_WRITE( MMINV_BROWSER_KEY, key_data_ptr);
    }
}

/*****************************************************************************/
// 	Description : write key name to nv list
//	Global resource dependence : none
//   Author: fen.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_WriteKeyItem( MMI_STRING_T *in_url_ptr )
{
    uint16 i = 0;
    uint16 str_len = 0;
    MMIBROWSER_KEY_T *key_ptr = PNULL;
    uint16  begin_item = 0;
    BOOLEAN is_same = FALSE;
    
    if (PNULL == in_url_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_WriteKeyItem:in_url_ptr PNULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_274_112_2_18_2_6_38_301,(uint8*)"");
        return;
    }
    
    key_ptr = SCI_ALLOCA(sizeof(MMIBROWSER_KEY_T));
    if (PNULL == key_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_WriteKeyItem:key_ptr Alloc FAIL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_281_112_2_18_2_6_38_302,(uint8*)"");
        return;
    }
    MMIBROWSER_GetKeyInfo(key_ptr);
    
    for( i = 0; i < key_ptr->num; i++ )
    {
        str_len = (uint16)MAX(key_ptr->key_arr[i].wstr_len,in_url_ptr->wstr_len);
        if( !MMIAPICOM_Wstrncmp(key_ptr->key_arr[i].wstr, in_url_ptr->wstr_ptr, str_len ) )
        {
            begin_item = i;
            is_same = TRUE;
            if (0 == i)//the first, do nothing
            {
                SCI_FREE(key_ptr);
                key_ptr = PNULL;
                return;
            }
        }
    }
    
    if (!is_same)
    {
        if( key_ptr->num < MMIBROWSER_KEYNAME_NUM )//add new
        {
            begin_item = key_ptr->num;
            key_ptr->num++;
        }
        else//add but full,so remove the last
        {
            begin_item = MMIBROWSER_KEYNAME_NUM - 1;
        }
    }
    
    for( i = begin_item; i > 0; i-- )
    {
        SCI_MEMSET(key_ptr->key_arr[i].wstr, 0, ((MMIBROWSER_MAX_KEYNAME_LEN+1)*sizeof(wchar)));
        str_len = MIN(key_ptr->key_arr[i-1].wstr_len, MMIBROWSER_MAX_KEYNAME_LEN);
        MMI_WSTRNCPY(
            key_ptr->key_arr[i].wstr,
            MMIBROWSER_MAX_KEYNAME_LEN,
            key_ptr->key_arr[i-1].wstr,
            MMIBROWSER_MAX_KEYNAME_LEN,
            str_len
            );
        key_ptr->key_arr[i].wstr_len = key_ptr->key_arr[i-1].wstr_len;
    }
    SCI_MEMSET(key_ptr->key_arr[0].wstr, 0, ((MMIBROWSER_MAX_KEYNAME_LEN+1)*sizeof(wchar)));
    str_len = MIN(in_url_ptr->wstr_len, MMIBROWSER_MAX_KEYNAME_LEN);
    MMI_WSTRNCPY( 
        key_ptr->key_arr[0].wstr,
        MMIBROWSER_MAX_KEYNAME_LEN,
        in_url_ptr->wstr_ptr,
        in_url_ptr->wstr_len,
        str_len
        );
    key_ptr->key_arr[0].wstr_len = in_url_ptr->wstr_len;
    
    MMIBROWSER_SetKeyInfo(key_ptr);
    
    SCI_FREE(key_ptr);
}
#if 0
/*****************************************************************************/
// Description : Get Match key num
// Global resource dependence :
// Author: fen.xie
// Note: useless now
/*****************************************************************************/
PUBLIC uint16 MMIBROWSER_GetMatchKeyNum(wchar *key_name_ptr)
{
    uint16 i; 
    uint16 match_num = 0;
    MMIBROWSER_KEY_T *key_ptr = PNULL;
    wchar *temp_input_key = PNULL;
    uint16 len = 0;
    
    if((PNULL == key_name_ptr )||( 0 == (len = MMIAPICOM_Wstrlen(key_name_ptr ))))
    {
        return 0;
    }
    
    temp_input_key = SCI_ALLOCA(sizeof(wchar) * (len + 1));  
    if(PNULL == temp_input_key)
    {
        return 0;
    }
    SCI_MEMSET(temp_input_key, 0, sizeof(wchar) *  (len + 1));
    MMIAPICOM_Wstrncpy(temp_input_key, key_name_ptr, len);
    MMIAPICOM_Wstrlower(temp_input_key);
    
    key_ptr = SCI_ALLOCA(sizeof(MMIBROWSER_KEY_T));
    if (PNULL == key_ptr)
    {
        SCI_FREE(temp_input_key);
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetMatchKeyNum:key_ptr Alloc FAIL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_374_112_2_18_2_6_38_303,(uint8*)"");
        return 0;
    }
    MMIBROWSER_GetKeyInfo(key_ptr);
    
    for( i = 0; i < key_ptr->num; i++ )
    {
        MMIAPICOM_Wstrlower(key_ptr->key_arr[i].wstr);
        if(!MMIAPICOM_Wstrncmp( key_ptr->key_arr[i].wstr, temp_input_key, MMIAPICOM_Wstrlen(temp_input_key)))
        {            
            match_num++;
        }
    }
    
    SCI_FREE(temp_input_key);
    SCI_FREE(key_ptr);
    
    return match_num;
}
#endif

/*****************************************************************************/
// Description : List match key name
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIBROWSER_ListMatchKeyName( wchar *key_name_ptr)
{
    uint16 i= 0; 
    uint16 match_num = 0;
    MMIBROWSER_KEY_T *key_ptr = PNULL;
    wchar *temp_input_key = NULL;
    uint16 len = 0;
    uint32  match_key_arr[MMIBROWSER_KEYNAME_NUM] = {0};
    
    do
    {
        //init
        if (s_match_key_t.match_key_ptr != PNULL)
        {
            SCI_FREE(s_match_key_t.match_key_ptr);
            s_match_key_t.match_key_ptr= PNULL;
        }
        SCI_MEMSET(&s_match_key_t, 0, sizeof(MMIBROWSER_MATCH_KEY_T));
        
        key_ptr = SCI_ALLOCA(sizeof(MMIBROWSER_KEY_T));
        if (PNULL == key_ptr)
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_ListMatchKeyName:key_ptr Alloc FAIL "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_421_112_2_18_2_6_38_304,(uint8*)"");
            break;//err
        }
        MMIBROWSER_GetKeyInfo(key_ptr);
        
        if (0 == key_ptr->num)
        {
            break;//no record so stop to match
        }
        
        if ((PNULL == key_name_ptr )|| (0 == (len = MMIAPICOM_Wstrlen(key_name_ptr ))))//list all
        {
            match_num = key_ptr->num;
            
            s_match_key_t.match_key_ptr = SCI_ALLOCA(sizeof(MMIBROWSER_KEY_NAME_T) * match_num);  
            if(PNULL == s_match_key_t.match_key_ptr)
            {
                match_num = 0;//reset
                //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_ListMatchKeyName:match_key_ptr Alloc FAIL "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_439_112_2_18_2_6_38_305,(uint8*)"");
                break;//err
            }
            SCI_MEMSET(s_match_key_t.match_key_ptr, 0, sizeof(MMIBROWSER_KEY_NAME_T) * match_num);
            
            for( i = 0; i < match_num; i++ )
            {
                SCI_MEMCPY(&s_match_key_t.match_key_ptr[i], &key_ptr->key_arr[i], sizeof(MMIBROWSER_KEY_NAME_T) );
            }
            
            break;//success, list all
        }
        else//match
        {
            temp_input_key = SCI_ALLOCA(sizeof(wchar) * (len + 1));  
            if(PNULL == temp_input_key)
            {
                //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_ListMatchKeyName:temp_input_key Alloc FAIL "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_456_112_2_18_2_6_38_306,(uint8*)"");
                break;//err
            }
            
            SCI_MEMSET(temp_input_key, 0, sizeof(wchar) *  (len + 1));
            MMIAPICOM_Wstrncpy(temp_input_key, key_name_ptr, len);
            MMIAPICOM_Wstrlower(temp_input_key);
            
            for( i = 0; i < key_ptr->num; i++ )//match
            {
                wchar *temp_key_ptr = PNULL; 
                uint16  key_len = key_ptr->key_arr[i].wstr_len;   
                
                temp_key_ptr = SCI_ALLOCA(sizeof(wchar) * (key_len + 1));
                
                if (PNULL == temp_key_ptr)
                {
                    //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_ListMatchKeyName:temp_key_ptr Alloc FAIL and i=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_473_112_2_18_2_6_38_307,(uint8*)"d",i);
                    break;//err, stop to match and list the found records
                }
                SCI_MEMSET(temp_key_ptr, 0, sizeof(wchar) *  (key_len + 1));
                
                MMIAPICOM_Wstrncpy(temp_key_ptr, key_ptr->key_arr[i].wstr,  key_len);       
                MMIAPICOM_Wstrlower(temp_key_ptr);  
                
                if(!MMIAPICOM_Wstrncmp( temp_key_ptr, temp_input_key, MMIAPICOM_Wstrlen(temp_input_key)))
                {               
                    match_key_arr[match_num] = i;
                    match_num++;
                }
                SCI_FREE(temp_key_ptr);
            }
            SCI_FREE(temp_input_key);
            
            if (match_num > 0)
            {
                s_match_key_t.match_key_ptr = SCI_ALLOCA(sizeof(MMIBROWSER_KEY_NAME_T) * match_num);  
                if (PNULL == s_match_key_t.match_key_ptr)
                {
                    match_num = 0;//reset
                    //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_ListMatchKeyName:match_key_ptr Alloc FAIL "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_496_112_2_18_2_6_38_308,(uint8*)"");
                    break;//err
                }
                SCI_MEMSET(s_match_key_t.match_key_ptr, 0, sizeof(MMIBROWSER_KEY_NAME_T) * match_num);
                
                for( i = 0; i < match_num; i++ )
                {
                    SCI_MEMCPY(&s_match_key_t.match_key_ptr[i], &key_ptr->key_arr[match_key_arr[i]], sizeof(MMIBROWSER_KEY_NAME_T) );
                }
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_ListMatchKeyName:no match "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_508_112_2_18_2_6_38_309,(uint8*)"");
            }
            
        }
    }
    while(0);
    
    if (key_ptr != PNULL)
    {
        SCI_FREE(key_ptr);
    }
    if (temp_input_key != PNULL)
    {
        SCI_FREE(temp_input_key);
    }
    
    s_match_key_t.match_num = match_num;
    return match_num;
}

/*****************************************************************************/
//  Description : Generate search web;cppy form MMIWWW_GenerateSearchWeb
//  Global resource dependence : 
//  Author: fen.xie
//  Note:RETURN need to FREE outside
/*****************************************************************************/
PUBLIC uint8 * MMIBROWSER_GenerateSearchWeb(
                                            MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine, 
                                            const MMI_STRING_T *search_str
                                            )
{
    uint16 str_len = 0;
    uint8 *str_ptr = PNULL;
    uint8 *str_temp_ptr = PNULL;
    uint16 url_len = 0;
    uint8 *url_ptr = PNULL;
    uint8 *url_temp_ptr = PNULL;
    char *search_head_ptr = PNULL;
    char *search_tail_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_GenerateSearchWeb search_engine = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_546_112_2_18_2_6_39_310,(uint8*)"d", search_engine);
    search_engine = (search_engine < MMIBROWSER_SEARCH_ENGINE_MAX_NUM)? search_engine: 0;//protect
    
    if ((PNULL != search_str) && (PNULL != search_str->wstr_ptr) && (search_str->wstr_len > 0))
    {
        search_head_ptr = s_search_info[search_engine].search_head_ptr;
        search_tail_ptr = s_search_info[search_engine].search_tail_ptr;
        
        str_len = (uint16)(search_str->wstr_len * 3 + 1);
        str_ptr = SCI_ALLOCA(str_len);
        if (PNULL != str_ptr)
        {
            SCI_MEMSET(str_ptr, 0x00, str_len);
            
            switch (search_engine)
            {
            case MMIBROWSER_SEARCH_ENGINE_BAIDU:      
            case MMIBROWSER_SEARCH_ENGINE_SOSO:
                //case MMIBROWSER_SEARCH_ENGINE_GOOGLE:
                //case MMIBROWSER_SEARCH_ENGINE_CMCC:
                //case MMIBROWSER_SEARCH_ENGINE_YAHOO:
                GUI_WstrToUTF8(str_ptr, str_len, search_str->wstr_ptr, search_str->wstr_len);
                break;
                
            default:
                //GUI_WstrToGB(str_ptr, search_str->wstr_ptr, search_str->wstr_len);
                GUI_WstrToUTF8(str_ptr, str_len, search_str->wstr_ptr, search_str->wstr_len);
                break;
            }
            
            url_len = (uint16)(SCI_STRLEN(search_head_ptr) + 3 * SCI_STRLEN((char *)str_ptr) + strlen(search_tail_ptr)+ 1);
            url_ptr = SCI_ALLOCA(url_len);
            if (PNULL != url_ptr)
            {
                SCI_MEMSET((char *)url_ptr, 0x00, url_len);
                strncpy((char *)url_ptr, search_head_ptr, SCI_STRLEN(search_head_ptr));
                str_temp_ptr = str_ptr;
                url_temp_ptr = url_ptr + SCI_STRLEN((char *)url_ptr);
                while(*str_temp_ptr)
                {
                    *url_temp_ptr++ = '%';
                    sprintf((char *)url_temp_ptr, "%02x", *str_temp_ptr++);
                    
                    url_temp_ptr = (uint8 *)MMIAPICOM_Strupper((char *)url_temp_ptr);
                    
                    url_temp_ptr += 2;
                }
                *url_temp_ptr = '\0';
                strcat((char *)url_ptr, search_tail_ptr);
            }
            else//abort
            {
                //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GenerateSearchWeb url_ptr alloc"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_598_112_2_18_2_6_39_311,(uint8*)"");
            }
            
            SCI_FREE(str_ptr);
            return url_ptr;
        }
        else//abort
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GenerateSearchWeb str_ptr alloc"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_606_112_2_18_2_6_39_312,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GenerateSearchWeb PARAM ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_611_112_2_18_2_6_39_313,(uint8*)"");
    }
    
    return PNULL;
}
/*****************************************************************************/
//  Description : init the reference infomation of brower position
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void InitBrwSearchMain(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_WIDTH_T   from_width = {0};
    GUIFORM_CHILD_HEIGHT_T  from_height = {0};
    uint16  img_width = 0;
    MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine = MMIBROWSER_GetSearchEngine();
    GUI_BG_T bg = {0};
    MMI_CTRL_ID_T   edit_ctrl_id = MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID;
    GUILIST_ITEM_T      item_t      =   {0};
    GUILIST_ITEM_DATA_T item_data   =   {0};
    MMI_CTRL_ID_T   list_ctrl_id = MMIBROWSER_SEARCH_MAIN_LIST_CTRL_ID;
    uint32  i = 0;
    
    //FORM
    GUIFORM_SetMargin(MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID, 0);
    GUIFORM_IsSlide(MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID, FALSE);
    
#ifdef SEARCH_SUPPORT
    {
        MMI_STRING_T  keyword_str = {0};        
        if( MMIBROWSER_ACCESS_SEARCH ==  MMIBROWSER_GetEntryType())
        {
            MMIAPISEARCH_GetEditBoxString(&keyword_str);
            if(0 != keyword_str.wstr_len &&  PNULL != keyword_str.wstr_ptr)
            {
                GUIEDIT_SetString(edit_ctrl_id, keyword_str.wstr_ptr, keyword_str.wstr_len);
            }            
        }
    }
#endif
    
    GUIFORM_SetType(MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    //GUIEDIT_PermitBorder(edit_ctrl_id, FALSE);
    GUIFORM_PermitChildBg(MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID, FALSE);
    
    from_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    from_height.add_data = MMIBROWSER_STARTPAGE_FORM1_HEIGHT;
    
    GUIFORM_SetChildHeight(MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID, MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, &from_height);
    
    GUIFORM_SetMargin(MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, 3);

    //SCI_MEMSET(&bg, 0x00, sizeof(bg));
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_BROWSER_SEARCH_BAR_BG;
    GUIFORM_SetBg( MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, &bg);
    
    from_height.type     = GUIFORM_CHILD_HEIGHT_PERCENT;
    from_height.add_data = 95;
    GUIFORM_SetChildHeight(MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID, &from_height);
    GUIFORM_SetChildHeight(MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID, &from_height);

    GUIRES_GetImgWidthHeight(&img_width, PNULL, IMAGE_BROWSER_LOG_BAIDU, win_id);
    from_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    from_width.add_data = img_width + 2;
    GUIFORM_SetChildWidth(MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, MMIBROWSER_SEARCH_MAIN_LOG_CTRL_ID, &from_width);

    GUIBUTTON_SetRunSheen(MMIBROWSER_SEARCH_MAIN_LOG_CTRL_ID, FALSE);
    //SCI_MEMSET(&bg, 0x00, sizeof(bg));
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = s_search_info[search_engine].img_id;
    GUIBUTTON_SetGrayed(MMIBROWSER_SEARCH_MAIN_LOG_CTRL_ID, TRUE, &bg, FALSE);            
    
    //SCI_MEMSET(&bg, 0x00, sizeof(bg));
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_BROWSER_STARTPAGE_LABEL_BG_FOCUSED;
    GUIEDIT_SetBg(edit_ctrl_id, &bg);
    
#ifdef TOUCH_PANEL_SUPPORT
    //Search button
    GUIRES_GetImgWidthHeight(&img_width, PNULL, IMAGE_BROWSER_SEARCH, win_id);
    from_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    from_width.add_data = img_width;
    GUIFORM_SetChildWidth(MMIBROWSER_SEARCH_MAIN_FORM1_CTRL_ID, MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID, &from_width);
    GUIBUTTON_SetCallBackFunc(MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID, SearchButtonCallBack);
    
    //SCI_MEMSET(&bg, 0x00, sizeof(bg));
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_BROWSER_SEARCH_GRAY;
    GUIBUTTON_SetGrayed(MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID, TRUE, &bg, FALSE);
#endif
    
    //set im
    GUIEDIT_SetIm(edit_ctrl_id, GUIIM_TYPE_SET_ALL, GUIIM_TYPE_NONE);
    GUIEDIT_SetStyle(edit_ctrl_id, GUIEDIT_STYLE_SINGLE);
#ifndef MMI_PDA_SUPPORT
    GUIEDIT_SetSoftkey(edit_ctrl_id, 0, 1, TXT_WWW_SUBMENU_HISTORY, TXT_WWW_SUBMENU_HISTORY, PNULL);
    GUIEDIT_SetSoftkey(edit_ctrl_id, 1, 1, TXT_NULL, STXT_SEARCH,PNULL);
    GUIEDIT_SetSoftkey(edit_ctrl_id, 2, 1, STXT_CANCEL, TXT_COMMON_CLEAR, PNULL);
#endif

    //set list
    SCI_MEMSET(&from_height, 0x00, sizeof(from_height));
    from_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(MMIBROWSER_SEARCH_MAIN_FORM_CTRL_ID, list_ctrl_id, &from_height);
    
    GUILIST_SetMaxItem(list_ctrl_id, 2, FALSE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    
#ifndef MMI_PDA_SUPPORT
    item_data.softkey_id[0] = TXT_WWW_SUBMENU_HISTORY;
    item_data.softkey_id[1] = TXT_COMMON_OK;
    item_data.softkey_id[2] = STXT_CANCEL;
#endif
    for (i=0; i< MMIBROWSER_SEARCH_ENGINE_MAX_NUM; i++)
    {
        item_data.item_content[0].item_data.text_id = s_search_info[i].text_id;
        item_t.user_data = i;
        GUILIST_AppendItem(list_ctrl_id, &item_t);      
    }
    GUILIST_SetCurItemIndex(list_ctrl_id, 0);
    GUILIST_SetSelectedItem(list_ctrl_id, search_engine, TRUE );
#ifdef MMI_PDA_SUPPORT
    GUILIST_SetUserBg(list_ctrl_id, FALSE);
#else
    GUILIST_SetOwnSofterKey(list_ctrl_id, TRUE);
#endif
    GUILIST_SetSlideState(list_ctrl_id, FALSE);  
}

/*****************************************************************************/
//  Description : display search icon
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySearchLog(MMI_CTRL_ID_T ctrl_id)
{
    MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine = MMIBROWSER_SEARCH_ENGINE_DEFAULT;
    uint16  selected_index = 0;
    GUI_BG_T bg = {0};
    
    GUILIST_GetSelectedItemIndex(MMIBROWSER_SEARCH_MAIN_LIST_CTRL_ID, &selected_index, 1);
    GUILIST_GetItemData(MMIBROWSER_SEARCH_MAIN_LIST_CTRL_ID, selected_index, &search_engine);
    search_engine = (search_engine < MMIBROWSER_SEARCH_ENGINE_MAX_NUM)? search_engine: 0;//protect
    
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = s_search_info[search_engine].img_id;
    
    GUIBUTTON_SetGrayed(ctrl_id, TRUE, &bg, TRUE);            
}

/*****************************************************************************/
//  Description : search the word in browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_Search(void)
{
    uint8   *url_ptr = PNULL;
    MMI_STRING_T    text_str = {0};
    MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine = MMIBROWSER_SEARCH_ENGINE_DEFAULT;
    uint16  selected_index = 0;
    
    GUIEDIT_GetString(MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID,&text_str);
    
    GUILIST_GetSelectedItemIndex(MMIBROWSER_SEARCH_MAIN_LIST_CTRL_ID, &selected_index, 1);
    GUILIST_GetItemData(MMIBROWSER_SEARCH_MAIN_LIST_CTRL_ID, selected_index, &search_engine);
    
    if ((PNULL != text_str.wstr_ptr) && (text_str.wstr_len > 0))
    {
        url_ptr = MMIBROWSER_GenerateSearchWeb(search_engine, &text_str);
    }
    
    if (url_ptr != PNULL)
    {
        MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, url_ptr, PNULL);
        MMIBROWSER_WriteKeyItem(&text_str);
        
        SCI_FREE(url_ptr);        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : the callback of search button
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E SearchButtonCallBack(void)
{
    if (MMIBROWSER_Search())
    {
        MMK_CloseWin(MMIBROWSER_SEARCH_MAIN_WIN_ID);
    }
    return MMI_RESULT_TRUE;
}
#endif

/*****************************************************************************/
//  Description : handle search window message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSearchMainWinMsg(
                                             MMI_WIN_ID_T	win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM		param
                                             )
{
    MMI_RESULT_E    ret=MMI_RESULT_TRUE;    
    MMI_CTRL_ID_T   list_ctrl_id = MMIBROWSER_SEARCH_MAIN_LIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitBrwSearchMain(win_id);
        MMK_SetAtvCtrl(win_id,MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID);
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T src_ctrl_id = 0;
            
            if (PNULL != param)
            {
                src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
                
                if (src_ctrl_id == list_ctrl_id)
                {
                    GUILIST_SetSelectedItem(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), TRUE);
                    MMK_SendMsg(list_ctrl_id, MSG_CTL_PAINT, PNULL);//update the list
                    DisplaySearchLog(MMIBROWSER_SEARCH_MAIN_LOG_CTRL_ID);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (src_ctrl_id == MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID)
                {
                    if (MMIBROWSER_Search())
                    {
                        MMK_CloseWin(win_id);
                    }
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */
        
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif /* MMI_PDA_SUPPORT */
    case MSG_CTL_MIDSK:
        if (list_ctrl_id == MMK_GetActiveCtrlId(win_id))//»Ö¸´Ä¬ÈÏÉèÖÃ
        {
            GUILIST_SetSelectedItem(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), TRUE);
            MMK_SendMsg(list_ctrl_id, MSG_CTL_PAINT, PNULL);//update the list
            DisplaySearchLog(MMIBROWSER_SEARCH_MAIN_LOG_CTRL_ID);
        }
#ifdef TOUCH_PANEL_SUPPORT
//        else if (MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        else if (MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID == MMK_GetActiveCtrlId(win_id))
#else
        else if (MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID == MMK_GetActiveCtrlId(win_id))
#endif
        {
            if (MMIBROWSER_Search())
            {
                MMK_CloseWin(win_id);
            }
        }
        break;
        
    case MSG_APP_OK://the searched history
    case MSG_CTL_OK:
        MMK_CreateWin((uint32*)BRW_SEARCHLIST_WIN_TAB,PNULL);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL: 
        MMK_CloseWin(win_id);
        break;
        
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            MMI_CTRL_ID_T ctrl_id = 0;
            GUI_BG_T bg = {0};
            
            ctrl_id = MMK_GetActiveCtrlId(win_id);
            
            bg.bg_type = GUI_BG_IMG;
            bg.img_id = IMAGE_BROWSER_STARTPAGE_LABEL_BG;
            
            if (ctrl_id == MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID)
            {
                bg.img_id = IMAGE_BROWSER_STARTPAGE_LABEL_BG_FOCUSED;
                GUIEDIT_SetBg(MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID, &bg);
                
                GUILIST_SetCurItemIndex(list_ctrl_id, 0);
            }
            else
            {
                bg.img_id = IMAGE_BROWSER_STARTPAGE_LABEL_BG;
                GUIEDIT_SetBg(MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID, &bg);
            }
            
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }    
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
    case MSG_GET_FOCUS:
        {
            MMI_STRING_T    text_str = {0};
            
            GUIEDIT_GetString(MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID,&text_str);
            
            if ((text_str.wstr_ptr != PNULL) && (text_str.wstr_len > 0))
            {
                GUIBUTTON_SetGrayed(MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID,FALSE,PNULL,(MMK_IsFocusWin(win_id)));
            }
            else
            {
                GUIBUTTON_SetGrayed(MMIBROWSER_SEARCH_MAIN_BUTTON_CTRL_ID,TRUE,PNULL,(MMK_IsFocusWin(win_id)));
            }
        }
        break;
#endif    
        
    case MSG_CLOSE_WINDOW:
        {
            MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine = MMIBROWSER_SEARCH_ENGINE_DEFAULT;
            uint16  selected_index = 0;
            
            //save the search engine
            GUILIST_GetSelectedItemIndex(list_ctrl_id, &selected_index, 1);
            GUILIST_GetItemData(list_ctrl_id, selected_index, &search_engine);
            MMIBROWSER_SetSearchEngine(search_engine);
        }
#ifdef SEARCH_SUPPORT
        if( MMIBROWSER_ACCESS_SEARCH ==  MMIBROWSER_GetEntryType() && !MMIDORADO_IsRunning())
        {
            MMIBROWSER_Exit();
        }
#endif
        break;
        
    default:
        ret = MMI_RESULT_FALSE;
        break;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : handle search list window message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleSearchListWinMsg(
                                             MMI_WIN_ID_T	win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM param
                                             )
{
    MMI_CTRL_ID_T                       list_ctrl_id = MMIBROWSER_HISTORY_LIST_CTRL_ID;
    MMI_STRING_T                        name_str={0};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_index = 0; 
    GUILIST_ITEM_T *cur_list_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            uint16 i = 0; 
            uint16 match_num = 0;
            
            match_num = MMIBROWSER_ListMatchKeyName(PNULL);
            
            //init the guilist
            if (GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
            {
                GUILIST_RemoveAllItems(list_ctrl_id);
            }
            
            if (match_num > 0)
            {
                GUILIST_SetMaxItem( list_ctrl_id, match_num, TRUE);//dynamic
                
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
                item_t.item_data_ptr = &item_data;
                
                for( i = 0; i < match_num; i++ )
                {
                    GUILIST_AppendItem( list_ctrl_id, &item_t );
                }
                GUILIST_SetTopItemIndex(list_ctrl_id, 0);
                GUILIST_SetCurItemIndex(list_ctrl_id, 0);
                
                GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            
            GUILIST_SetTitleIndexType(list_ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
            
            MMK_SetAtvCtrl(win_id,list_ctrl_id);  
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            uint32  item_index = 0;
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            
            if( PNULL == need_item_data_ptr )
            {
                //SCI_TRACE_LOW:"[BRW]W BrwHandleSearchListWinMsg PNULL == need_item_data_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_1088_112_2_18_2_6_40_314,(uint8*)"");
                break;
            }
            
            item_index = need_item_data_ptr->item_index; 
            if ((PNULL == s_match_key_t.match_key_ptr) || (item_index >= s_match_key_t.match_num))
            {
                //SCI_TRACE_LOW:"[BRW]W BrwHandleSearchListWinMsg:match_key_ptr = 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_1095_112_2_18_2_6_40_315,(uint8*)"d", s_match_key_t.match_key_ptr);
                break;
            }
            
            //set info
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;
            
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
            
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            
            if (s_match_key_t.match_key_ptr[item_index].wstr_len > 0)
            {
                item_data.item_content[0].item_data.text_buffer.wstr_len = s_match_key_t.match_key_ptr[item_index].wstr_len;
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = s_match_key_t.match_key_ptr[item_index].wstr;
            }
            
            GUILIST_SetItemData(list_ctrl_id, &item_data, need_item_data_ptr->item_index);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        { 
            cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            cur_list_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(list_ctrl_id, cur_index);
            
            if (PNULL == cur_list_ptr)
            {
                //do nothing
                //SCI_TRACE_LOW:"[BRW]W BrwHandleSearchListWinMsg:cur_list_ptr PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_SEARCH_1131_112_2_18_2_6_40_316,(uint8*)"");
                break;
            }
            
            name_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
            name_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
            
            GUIEDIT_SetString(MMIBROWSER_SEARCH_MAIN_EDIT_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len);
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (s_match_key_t.match_key_ptr != PNULL)
        {
            SCI_FREE(s_match_key_t.match_key_ptr);
            s_match_key_t.match_key_ptr= PNULL;
        }
        SCI_MEMSET(&s_match_key_t, 0, sizeof(MMIBROWSER_MATCH_KEY_T));
        
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : open the search window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSearchWin(void)
{
    MMK_CreateWin((uint32*)MMIBROWSER_SEARCH_MAIN_WIN_TAB,PNULL);
}


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif/*BROWSER_SEARCH_SUPPORT*/
    
    


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
