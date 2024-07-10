/*****************************************************************************
** File Name:      mmisearch_app.c                                           *
** Author:                                                                   *
** Date:           2012-01	                                                 *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe control layer of search.    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        tao.xue           Create									                 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_search_trc.h"
#ifdef SEARCH_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmi_modu_main.h"
#include "mmisearch_id.h"
#include "mmipub.h"
#include "mmk_type.h"
#include "mmk_msg.h"

#include "mmisearch_menutable.h"
#include "mmisearch_internal.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifmm_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmisms_export.h"
#include "mmicl_export.h"
#include "mmiudisk_export.h"
#if defined(BROWSER_SUPPORT) && !defined(BROWSER_SEARCH_SUPPORT)
#include "gui_ucs2b_converter.h "
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#if defined(BROWSER_SUPPORT) && !defined(BROWSER_SEARCH_SUPPORT)
#define MMISEARCH_BRSCFG_BAIDU_SEARCH_HEAD    ("http://m.baidu.com/s?word=")
#define MMISEARCH_BRSCFG_BAIDU_SEARCH_TAIL     ("&from=1214a")
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef struct
{
    MMISEARCH_APP_OPEN_PFUNC open_pfunc;
    uint32 user_data;
}MMISEARCH_SEARCH_STOPPED_CALLBACK_T;

typedef struct
{
    MMISEARCH_TYPE_E select_type;//当前选择的搜索类型
    BOOLEAN is_need_restart[MMISEARCH_TYPE_MAX];//数据无效，需要重新搜索?
    MMISEARCH_STATUS_E search_status[MMISEARCH_TYPE_MAX];

    uint32 matched_item_num;//搜索到的匹配数据个数
    uint32 displayed_item_num;//已在界面上显示的匹配数据个数
    MMI_STRING_T search_keyword_str;
    MMISEARCH_MATCH_ITEM_T* matched_item_ptr;
    BOOLEAN is_append_list_item_msg_send;
    MMI_HANDLE_T  search_win_handle;

    BOOLEAN is_need_run_callback;
    MMISEARCH_SEARCH_STOPPED_CALLBACK_T search_stopped_callback;//回调函数(打开操作)结构体
    BOOLEAN is_only_mms;        //is only search mms
}MMISEARCH_CONTEXT_T;

typedef enum
{
    SEARCH_SAVE_SUCCESS,
    SEARCH_SAVE_SUCCESS_SEGMENT,
    SEARCH_SAVE_INVALID_DATA,
    SEARCH_SAVE_NO_SPACE,
    SEARCH_SAVE_ERROR,
    SEARCH_SAVE_RESULT_MAX
}MMISEARCH_SAVE_RESULT_E;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
LOCAL MMISEARCH_CONTEXT_T s_search_context;//当前search模块的总体情况
LOCAL BOOLEAN             s_istyle_first_entry = FALSE;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init search module data
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void Search_Init(void);

/*****************************************************************************/
//  Description : 申请匹配数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void AllocSearchBuffer(void);

/*****************************************************************************/
//  Description : 初始化匹配数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void InitSearchBuffer(void);

/*****************************************************************************/
//  Description : 释放匹配数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void FreeSearchBuffer(void);

/*****************************************************************************/
//  Description : 保存匹配数据至数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMISEARCH_SAVE_RESULT_E SaveMatchItems(MMISEARCH_DATA_T match_data);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void UpdateUIDisplay(MMI_MESSAGE_ID_E msg_id, BOOLEAN is_post);

/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartClSearching(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartAppSearching(void);

/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartPbSearching(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartSmsSearching(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : file searching 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartFileSearching(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void StartAllSearching(BOOLEAN is_validate_privacy_passed);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void SEARCH_ValidatePrivacyCallBack(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : entry of browser
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void OpenBrowserMatchedItem(uint32 index);

/*****************************************************************************/
//  Description : load browser search item data 
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
LOCAL void SetBrowserSearchData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data);

/*****************************************************************************/
//  Description : insert fix match data of browser
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
LOCAL void InsertBrowserSearchItem(void);

/*****************************************************************************/
//  Description : load prompt no match item data 
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
LOCAL void SetPromptNoMatchesData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data);

 /*****************************************************************************/
//  Description : indication the window if there is new item needed to be append
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void AppendItemToMatchedList(void);

/*****************************************************************************/
//  Description : indication the window if there is new item needed to be append
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void ReAppendItemToMatchedList(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void HandlePBEventFunc(MMIEVENT_PB_EVENT_E event);

/*****************************************************************************/
//  Description : init search module data
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void Search_Init(void)
{  
    if(PNULL != s_search_context.search_keyword_str.wstr_ptr)
    {
        SCI_FREE(s_search_context.search_keyword_str.wstr_ptr);
    }
    SCI_MEMSET(&s_search_context, 0x00, sizeof(s_search_context));
    s_search_context.select_type = MMISEARCH_TYPE_ALL;

    s_search_context.search_keyword_str.wstr_ptr = SCI_ALLOCAZ((MMISEARCH_KEYWORD_LENGTH_MAX + 1)*sizeof(wchar));
    
    if (PNULL == s_search_context.search_keyword_str.wstr_ptr)
    {
        return ;
    }
    s_search_context.search_keyword_str.wstr_len = 0;

    s_search_context.search_win_handle = MMISEARCH_MAIN_WIN_ID;

    MMK_PBRegisterEvent(HandlePBEventFunc);
    //申请匹配数据缓冲数据区
    AllocSearchBuffer();
}

/*****************************************************************************/
//  Description : 初始化匹配数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_Free(void)
{
    //释放数据区申请的内存
    FreeSearchBuffer();
    
    //调用各个应用的资源释放函数
    MMIAPIAPP_FreeLocalSearchResource();
    MMIAPICL_FreeLocalSearchResource();
    MMIAPIPB_FreeLocalSearchResource();
    MMIAPISMS_FreeLocalSearchResource();
    MMIAPIFMM_FreeLocalSearchResource();

    if(PNULL != s_search_context.search_keyword_str.wstr_ptr)
    {
        SCI_FREE(s_search_context.search_keyword_str.wstr_ptr);
    }
    SCI_MEMSET(&s_search_context, 0x00, sizeof(s_search_context));
}

/*****************************************************************************/
//  Description : 申请匹配数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void AllocSearchBuffer(void)
{
    if (PNULL != s_search_context.matched_item_ptr)
    {
        SCI_MEMSET((void *)s_search_context.matched_item_ptr, 0x00, MMISEARCH_MATCHED_NUM_MAX*sizeof(MMISEARCH_MATCH_ITEM_T));
    }
    else
    {
        s_search_context.matched_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(MMISEARCH_MATCHED_NUM_MAX*sizeof(MMISEARCH_MATCH_ITEM_T));
    }

    if (PNULL == s_search_context.matched_item_ptr)
    {
        return ;
    }
}

/*****************************************************************************/
//  Description : 初始化匹配数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void InitSearchBuffer(void)
{
    s_search_context.matched_item_num = 0;
    s_search_context.displayed_item_num = 0;
    SCI_MEMSET(s_search_context.search_status, 0x00, MMISEARCH_TYPE_MAX*sizeof(MMISEARCH_STATUS_E));

    //网络搜索入口默认为第一匹配项
    if (!s_search_context.is_only_mms)
        InsertBrowserSearchItem();
}

/*****************************************************************************/
//  Description : 释放匹配数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void FreeSearchBuffer(void)
{
    if (PNULL != s_search_context.matched_item_ptr)
    {
        SCI_FREE(s_search_context.matched_item_ptr);
    }
}

/*****************************************************************************/
//  Description : 获取匹配数据个数
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISEARCH_GetMatchItemsNum(void)
{
    if ( s_search_context.matched_item_num > MMISEARCH_MATCHED_NUM_MAX )
    {
        return MMISEARCH_MATCHED_NUM_MAX;
    }

    return s_search_context.matched_item_num;
}

/*****************************************************************************/
//  Description :设置 已在界面上显示的匹配数据个数
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetDisplayItemsNum(uint32 displayed_num)
{
    s_search_context.displayed_item_num = displayed_num;
}

/*****************************************************************************/
//  Description :获得 已在界面上显示的匹配数据个数
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISEARCH_GetDisplayItemsNum(void)
{
    return s_search_context.displayed_item_num;
}

/*****************************************************************************/
//  Description :是否匹配数据已显示完毕?
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_IsDisplayItemsEnd(void)
{
    if (s_search_context.displayed_item_num == s_search_context.matched_item_num
        && MMISEARCH_IsAsyncUpdataTimerStopped())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 保存匹配数据至数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMISEARCH_SAVE_RESULT_E SaveMatchItems(MMISEARCH_DATA_T match_data)
{
    if ( PNULL ==  s_search_context.matched_item_ptr)
    {
        return SEARCH_SAVE_ERROR;
    }
    
    if (PNULL == match_data.match_item_ptr|| 0 == match_data.match_num)
    {
        return SEARCH_SAVE_INVALID_DATA;
    }
    
    if (match_data.match_num <= (MMISEARCH_MATCHED_NUM_MAX - s_search_context.matched_item_num))
    {
        SCI_MEMCPY((void *)(s_search_context.matched_item_ptr + s_search_context.matched_item_num), (void *)match_data.match_item_ptr, match_data.match_num*sizeof(MMISEARCH_MATCH_ITEM_T));
         s_search_context.matched_item_num += match_data.match_num;
         return SEARCH_SAVE_SUCCESS;
    }
    else if (s_search_context.matched_item_num < MMISEARCH_MATCHED_NUM_MAX)
    {
        uint32 match_num_can_save = MMISEARCH_MATCHED_NUM_MAX - s_search_context.matched_item_num;

        SCI_MEMCPY((void *)(s_search_context.matched_item_ptr + s_search_context.matched_item_num), (void *)match_data.match_item_ptr, match_num_can_save*sizeof(MMISEARCH_MATCH_ITEM_T));
         s_search_context.matched_item_num += match_num_can_save;
        return SEARCH_SAVE_SUCCESS_SEGMENT;
    }
    else
    {
        return SEARCH_SAVE_NO_SPACE;
    }
}

/*****************************************************************************/
//  Description :get one item from match data buffer by index
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC MMISEARCH_MATCH_ITEM_T* MMISEARCH_GetOneMatchItemPtr(uint32 index)
{
    if ( index > s_search_context.matched_item_num || PNULL == s_search_context.matched_item_ptr)
    {
        return PNULL;
    }
    else
    {
        return &s_search_context.matched_item_ptr[index];
    }
}

/*****************************************************************************/
//  Description :get start and end index of match item by type
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMatchItemsAreaByType(MMISEARCH_TYPE_E type, uint32* type_start_index, uint32* type_end_index)
{
    uint32 index = 0;
    uint32 start_index = 0;
    uint32 end_index = 0;
    uint32 total_match_num = MMISEARCH_GetMatchItemsNum();
    MMISEARCH_MATCH_ITEM_T *match_item_ptr = PNULL;

    if ( PNULL ==  s_search_context.matched_item_ptr)
    {
        return FALSE;
    }
    
    if ( MMISEARCH_TYPE_MAX < type )
    {
        return FALSE;
    }

    for (index = 0; index < total_match_num; index++)
    {
        match_item_ptr = MMISEARCH_GetOneMatchItemPtr(index);
        if ( match_item_ptr->match_item_type == type)
        {
            start_index = index;
            break;
        }
    }

    for (index = start_index; index < total_match_num; index++)
    {
        match_item_ptr = MMISEARCH_GetOneMatchItemPtr(index);
        if ( match_item_ptr->match_item_type != type)
        {
            end_index = index - 1;
            break;
        }
    }
    
    if ( 0 == end_index)
    {
        end_index = total_match_num - 1;
    }

    *type_start_index = start_index;
    *type_end_index = end_index;
    return TRUE;
}

/*****************************************************************************/
//  Description : 删除某类型的匹配数据
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_DeleteMatchItemsByType(MMISEARCH_TYPE_E type)
{
    uint32 start_index = 0;
    uint32 end_index = 0;
    
    if ( PNULL ==  s_search_context.matched_item_ptr)
    {
        return ;
    }
    
    if ( MMISEARCH_TYPE_MAX < type )
    {
        return ;
    }

    if (GetMatchItemsAreaByType(type, &start_index, &end_index))
    {
        SCI_MEMCPY((void*)MMISEARCH_GetOneMatchItemPtr(start_index), (void*)MMISEARCH_GetOneMatchItemPtr(end_index + 1), (s_search_context.matched_item_num - end_index - 1)*sizeof (MMISEARCH_MATCH_ITEM_T));
        s_search_context.matched_item_num -= (end_index - start_index + 1);

        ReAppendItemToMatchedList();

    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void UpdateUIDisplay(MMI_MESSAGE_ID_E msg_id, BOOLEAN is_post)
{
    MMI_WIN_ID_T   search_win_handle = MMISEARCH_GetSearchWinHandle();
    
    if (is_post)
    {
        MMK_PostMsg(search_win_handle, msg_id, PNULL, 0);
    }
    else
    {
        MMK_SendMsg(search_win_handle, msg_id, PNULL);
    }
}

/*****************************************************************************/
//  Description : start app searching
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartAppSearching(void)
{
    MMISEARCH_DATA_T    match_data = {0};
    uint32 max_num = MIN(MMIAPP_SEARCH_MAX_NUM, MMISEARCH_MATCHED_NUM_MAX);
    
    match_data.match_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(max_num*sizeof(MMISEARCH_MATCH_ITEM_T));

    if (PNULL != match_data.match_item_ptr)
    {
        BOOLEAN ret = TRUE;
        MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_APP, FALSE);
        
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_APP, MMISEARCH_STATUS_START);
        MMISEARCH_LocalSearchAPP(&s_search_context.search_keyword_str, &match_data, max_num);
        if (match_data.match_num != 0)
        {
            ret=MMIAPISEARCH_SaveMatchItems(match_data);
        }        
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_APP, MMISEARCH_STATUS_END);
        SCI_FREE(match_data.match_item_ptr);
        return ret;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : start cl searching
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartClSearching(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMISEARCH_DATA_T match_data = {0};
    uint32 max_num = MIN(MMICL_RECORD_MAX_NUM, MMISEARCH_MATCHED_NUM_MAX);

    if (!is_permitted)
    {
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_CL, MMISEARCH_STATUS_END);
        return TRUE;    //Return TRUE means operation successed
    }
    
    match_data.match_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(max_num*sizeof(MMISEARCH_MATCH_ITEM_T));

    if (PNULL != match_data.match_item_ptr)
    {
        BOOLEAN ret = TRUE;
        MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_CL, FALSE);

        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_CL, MMISEARCH_STATUS_START);
        MMIAPICL_LocalSearch(&s_search_context.search_keyword_str, &match_data, max_num);
        if (match_data.match_num != 0)
        {
            ret=MMIAPISEARCH_SaveMatchItems(match_data);
        }  
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_CL, MMISEARCH_STATUS_END);
        SCI_FREE(match_data.match_item_ptr);
        return ret;
    }
    else
    {
        return FALSE;   //Return TRUE means operation failed
    }
}

/*****************************************************************************/
//  Description : start pb searching
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartPbSearching(BOOLEAN is_permitted, DPARAM param_ptr)
{
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    if (!is_permitted)
    {
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_PB, MMISEARCH_STATUS_END);
        return TRUE;
    }
    
    if (!MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_PB))
    {
        MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_PB, FALSE);

        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_PB, MMISEARCH_STATUS_START);
        MMIAPIPB_LocalAsyncSearch(&s_search_context.search_keyword_str);
        UpdateUIDisplay(MSG_SEARCH_PB_START_UPDATE_TIMER, FALSE);
    }
    return TRUE;
#else
    MMISEARCH_DATA_T match_data = {0};
    uint32 pb_max_num = 0;
    uint32 max_num = 0;

    if (!is_permitted)
    {
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_PB, MMISEARCH_STATUS_END);
        return TRUE;
    }
    
    if (!MMIAPIPB_IsPbReady())
    {
        return TRUE;
    }
    
    pb_max_num = MMIAPIPB_GetMaxContactNum();
    max_num = MIN(pb_max_num, MMISEARCH_MATCHED_NUM_MAX);

    match_data.match_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(max_num*sizeof(MMISEARCH_MATCH_ITEM_T));

    if (PNULL != match_data.match_item_ptr)
    {
        BOOLEAN ret = TRUE;
        MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_PB, FALSE);

        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_PB, MMISEARCH_STATUS_START);
        MMIAPIPB_LocalSearch(&s_search_context.search_keyword_str, &match_data, max_num);
        if (match_data.match_num != 0)
        {
            ret=MMIAPISEARCH_SaveMatchItems(match_data);
        }         
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_PB, MMISEARCH_STATUS_END);
        SCI_FREE(match_data.match_item_ptr);
        return ret;        
    }
    else
    {
        return FALSE;
    }
#endif
}

/*****************************************************************************/
//  Description : sms searching 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartSmsSearching(BOOLEAN is_permitted, DPARAM param_ptr)
{
    if (!is_permitted)
    {
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_SMS, MMISEARCH_STATUS_END);
        return TRUE;
    }

    if (!MMIAPISMS_IsOrderOk())
    {
        return TRUE;
    }

    if (!MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_SMS))
    {
        MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_SMS, FALSE);

        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_SMS, MMISEARCH_STATUS_START);
        MMIAPISMS_StartLocalSearch(&s_search_context.search_keyword_str,s_search_context.is_only_mms);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : file searching 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartFileSearching(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMI_CTRL_ID_T   search_win_handle = MMISEARCH_GetSearchWinHandle();

    if (!is_permitted)
    {
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_FILE, MMISEARCH_STATUS_END);
        return TRUE;
    }
    
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_FILE, MMISEARCH_STATUS_END);
        MMIAPISEARCH_ClearCurrFileMatchNum();
        MMISearch_AddNotificationItemToFile(TXT_COMMON_UDISK_USING);
        //Add TXT_COMMON_UDISK_USING item to file search area
        return FALSE;
    }
    
    MMIAPISEARCH_ClearCurrFileMatchNum();
    
    if (!MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_FILE))
    {
        MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_FILE, FALSE);

        MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_FILE, MMISEARCH_STATUS_START);
        MMIAPIFMM_StartLocalSearch(&s_search_context.search_keyword_str, search_win_handle, MSG_SEARCH_FILE_LOAD_ALL_LIST_FINISH);
        UpdateUIDisplay(MSG_SEARCH_FILE_START_UPDATE_TIMER, FALSE);
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//   Global resource dependence : 
//   Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void SEARCH_ValidatePrivacyCallBack(BOOLEAN is_permitted, DPARAM param_ptr)
{  
    StartAllSearching(is_permitted);
}

/*****************************************************************************/
//  Description : start searching all type
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void StartAllSearching(BOOLEAN is_validate_privacy_passed)
{
    // 同步搜索，根据返回值确认是否需要终止搜索
    if (!StartAppSearching())
    {
        return;
    }
    
    if (!StartClSearching((BOOLEAN)(is_validate_privacy_passed || !MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)), PNULL))
    {
        return;
    }
    
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    StartPbSearching((BOOLEAN)(is_validate_privacy_passed || !MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB)), PNULL);
#else
    if (!StartPbSearching((BOOLEAN)(is_validate_privacy_passed || !MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB)), PNULL))
    {
        return;
    }
#endif

    //异步
    StartSmsSearching((BOOLEAN)(is_validate_privacy_passed || !MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS)), PNULL);

    StartFileSearching((BOOLEAN)(is_validate_privacy_passed || !MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_MYDOC)), PNULL);
}

#if defined(BROWSER_SUPPORT) && !defined(BROWSER_SEARCH_SUPPORT)
/*****************************************************************************/
//  Description : Generate search web;copy form MMIWWW_GenerateSearchWeb
//  Global resource dependence : 
//  Author: fen.xie
//  Note:RETURN need to FREE outside
/*****************************************************************************/
LOCAL uint8 * GenerateSearchWeb(const MMI_STRING_T *search_str)
{
    uint16 str_len = 0;
    uint8 *str_ptr = PNULL;
    uint8 *str_temp_ptr = PNULL;
    uint16 url_len = 0;
    uint8 *url_ptr = PNULL;
    uint8 *url_temp_ptr = PNULL;
    char *search_head_ptr = PNULL;
    char *search_tail_ptr = PNULL;
        
    if ((PNULL != search_str) && (PNULL != search_str->wstr_ptr) && (search_str->wstr_len > 0))
    {
        search_head_ptr = MMISEARCH_BRSCFG_BAIDU_SEARCH_HEAD;
        search_tail_ptr = MMISEARCH_BRSCFG_BAIDU_SEARCH_TAIL;
        
        str_len = (uint16)(search_str->wstr_len * 3 + 1);
        str_ptr = SCI_ALLOCA(str_len);
        if (PNULL != str_ptr)
        {
            SCI_MEMSET(str_ptr, 0x00, str_len);
            
            GUI_WstrToUTF8(str_ptr, str_len, search_str->wstr_ptr, search_str->wstr_len);
            
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
                SCI_TRACE_LOW("GenerateSearchWeb:url_ptr alloc fail");
            }
            
            SCI_FREE(str_ptr);
            return url_ptr;
        }
        else//abort
        {
            SCI_TRACE_LOW("GenerateSearchWeb:str_ptr alloc fail");
        }
    }
    else
    {
        SCI_TRACE_LOW("GenerateSearchWeb:PARAM ERROR");
    }
    
    return PNULL;
}
#endif
/*****************************************************************************/
//  Description : entry of browser
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void OpenBrowserMatchedItem(uint32 index)
{
#ifdef BROWSER_SUPPORT
#ifdef BROWSER_SEARCH_SUPPORT//only open in dorado browser
    MMIBROWSER_ENTRY_PARAM entry_param = {0};
            
    entry_param.type = MMIBROWSER_ACCESS_SEARCH;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.is_browser_type_customized = TRUE;
    entry_param.browser_type = MMIBROWSER_TYPE_DORADO;
 //   entry_param.search_ptr = s_search_context.search_keyword_str.wstr_ptr;
    MMIAPIBROWSER_Entry(&entry_param);
#else
    MMIBROWSER_ENTRY_PARAM entry_param = {0};
    MMI_STRING_T    text_str = {0};
    uint8   *url_ptr = PNULL;
            
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.type = MMIBROWSER_ACCESS_URL;

    MMIAPISEARCH_GetEditBoxString(&text_str);
    if ((PNULL != text_str.wstr_ptr) && (text_str.wstr_len > 0))
    {
        url_ptr = GenerateSearchWeb( &text_str);
    }
    
    if (url_ptr != PNULL)
    {
        entry_param.url_ptr = (char*)url_ptr;
        MMIAPIBROWSER_Entry(&entry_param);
        
        SCI_FREE(url_ptr);        
    }
    else
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
#endif/*BROWSER_SEARCH_SUPPORT*/
#else
    MMIPUB_OpenAlertFailWin(TXT_ERROR);
#endif
}

/*****************************************************************************/
//  Description : load browser search item data 
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
LOCAL void SetBrowserSearchData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data)
{ 
    GUILIST_ITEM_DATA_T item_data = {0};
  
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_NETWORK_SEARCHING;

    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_COMMON_OPEN;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
	
    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
}
/*****************************************************************************/
//  Description : insert fix match data of browser
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
LOCAL void InsertBrowserSearchItem(void)
{
    uint32 index = 0;
    MMISEARCH_DATA_T match_data = {0};
    MMISEARCH_MATCH_ITEM_T* matched_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(sizeof(MMISEARCH_MATCH_ITEM_T));

    for (index = 0; index < MMISEARCH_GetMatchItemsNum(); index++)
    {
        if (MMISEARCH_GetOneMatchItemPtr(index)->match_item_type == MMISEARCH_TYPE_WEB)
        {
            SCI_TRACE_LOW("InsertBrowserSearchItem Already exist!");
            return ;
        }
    }
    
    if (PNULL != matched_item_ptr)
    {
        matched_item_ptr[0].match_item_type = MMISEARCH_TYPE_WEB;
        matched_item_ptr[0].match_item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        matched_item_ptr[0].open_pfunc = OpenBrowserMatchedItem;
        matched_item_ptr[0].set_data_pfunc = SetBrowserSearchData;

        match_data.match_item_ptr = matched_item_ptr;
        match_data.match_num = 1;

        MMIAPISEARCH_SaveMatchItems(match_data);
        SCI_FREE(matched_item_ptr);
    }
}
/*****************************************************************************/
//  Description : load prompt no match item data 
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
LOCAL void SetPromptNoMatchesData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data)
{ 
    GUILIST_ITEM_DATA_T item_data = {0};
  
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_SEARCH_NO_MATCHES;

    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
	
    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
}
/*****************************************************************************/
//  Description : insert item of " No matches"
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_InsertPromptNoMatchesItem(void)
{
    uint32 index = 0;
    MMISEARCH_DATA_T match_data = {0};
    MMISEARCH_MATCH_ITEM_T* matched_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(sizeof(MMISEARCH_MATCH_ITEM_T));
    MMI_WIN_ID_T   search_win_handle = MMISEARCH_GetSearchWinHandle();

    for (index = 0; index < MMISEARCH_GetMatchItemsNum(); index++)
    {
		if (PNULL != MMISEARCH_GetOneMatchItemPtr(index))
		{
			if (MMISEARCH_GetOneMatchItemPtr(index)->match_item_type == MMISEARCH_TYPE_PROMPT_EMPTY)
			{
				SCI_TRACE_LOW("MMISEARCH_InsertPromptNoMatchesItem Already exist!");
				return ;
			}
		}
    }

    if (PNULL != matched_item_ptr)
    {
        matched_item_ptr[0].match_item_type = MMISEARCH_TYPE_PROMPT_EMPTY;
        matched_item_ptr[0].match_item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        matched_item_ptr[0].open_pfunc = PNULL;
        matched_item_ptr[0].set_data_pfunc = SetPromptNoMatchesData;

        match_data.match_item_ptr = matched_item_ptr;
        match_data.match_num = 1;

        MMIAPISEARCH_SaveMatchItems(match_data);
        SCI_FREE(matched_item_ptr);
        MMK_PostMsg(search_win_handle, MSG_SEARCH_SET_PROMPT_ITEM_INVALID, PNULL, 0);
    }
}

/*****************************************************************************/
//  Description : indication the window if there is new item needed to be append
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void AppendItemToMatchedList(void)
{
    if (!s_search_context.is_append_list_item_msg_send)
    {
        UpdateUIDisplay(MSG_SEARCH_LIST_APPEND_MATCH_ITEM, TRUE);
        s_search_context.is_append_list_item_msg_send = TRUE;
    }
}

/*****************************************************************************/
//  Description : indication the window if there is new item needed to be append
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void ReAppendItemToMatchedList(void)
{
    UpdateUIDisplay(MSG_SEARCH_LIST_REAPPEND_MATCH_ITEM, TRUE);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_ResetAppendFlag(void)
{
    s_search_context.is_append_list_item_msg_send = FALSE;
}

/*****************************************************************************/
//  Description : start searching with type
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_EntrySearching(MMI_STRING_T *keyword_str_ptr)
{
    if(PNULL != s_search_context.search_keyword_str.wstr_ptr)
    {
        SCI_MEMSET(s_search_context.search_keyword_str.wstr_ptr, 0x00, (MMISEARCH_KEYWORD_LENGTH_MAX + 1)*sizeof(wchar));
        SCI_MEMCPY(s_search_context.search_keyword_str.wstr_ptr, keyword_str_ptr->wstr_ptr, MIN(keyword_str_ptr->wstr_len, MMISEARCH_KEYWORD_LENGTH_MAX)*sizeof(wchar) );
        s_search_context.search_keyword_str.wstr_len = keyword_str_ptr->wstr_len;
    }

    InitSearchBuffer();
    MMISEARCH_ResetSplitNum();
    
    switch (s_search_context.select_type)
    {
        case MMISEARCH_TYPE_APP:
            StartAppSearching();
            break;
            
        case MMISEARCH_TYPE_CL:
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, (MMISET_PRIVACY_CALLBACK_PFUNC)StartClSearching, PNULL, 0);
            break;
                        
        case MMISEARCH_TYPE_PB:
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_PB, (MMISET_PRIVACY_CALLBACK_PFUNC)StartPbSearching, PNULL, 0);
            break;
            
        case MMISEARCH_TYPE_SMS:
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, (MMISET_PRIVACY_CALLBACK_PFUNC)StartSmsSearching, PNULL, 0);
            break;
            
        case MMISEARCH_TYPE_FILE:
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_MYDOC, (MMISET_PRIVACY_CALLBACK_PFUNC)StartFileSearching, PNULL, 0);
            break;
            
        case MMISEARCH_TYPE_ALL:
            {
                if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL))
                {
                    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, SEARCH_ValidatePrivacyCallBack, PNULL, 0);
                }
                else if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB))
                {
                    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_PB, SEARCH_ValidatePrivacyCallBack, PNULL, 0);
                }
                else if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
                {
                    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, SEARCH_ValidatePrivacyCallBack, PNULL, 0);
                }
                else if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_MYDOC))
                {
                    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_MYDOC, SEARCH_ValidatePrivacyCallBack, PNULL, 0);
                }
                else
                {
                    StartAllSearching(TRUE);
                }
            }
            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : set search type
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetSearchType(MMISEARCH_TYPE_E type)
{
    if (MMISEARCH_TYPE_APP <= type && type <= MMISEARCH_TYPE_ALL)
    {
        s_search_context.select_type = type;
    }
}
/*****************************************************************************/
//  Description : get search type
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
PUBLIC MMISEARCH_TYPE_E MMISEARCH_GetSearchType(void)
{
    return s_search_context.select_type;
}
#ifdef MMISEARCH_SUPPORT_ONLYMMS     //if mms only,couldn't changed type
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_isOnlyMMSSearch(void)
{
    return s_search_context.is_only_mms;
}
#endif

/*****************************************************************************/
//  Description : to get if search process is end  
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_IsSearchEnd(void)
{
    BOOLEAN is_end = FALSE;
    MMISEARCH_TYPE_E search_type = MMISEARCH_GetSearchType();

    switch (search_type)
    {
        case MMISEARCH_TYPE_APP:
        case MMISEARCH_TYPE_CL:
        case MMISEARCH_TYPE_PB:
        case MMISEARCH_TYPE_SMS:
        case MMISEARCH_TYPE_FILE:
            if (MMISEARCH_STATUS_END == MMISEARCH_GetSearchStatus(search_type))
            {
                is_end = TRUE;
            }
            break;
            
        case MMISEARCH_TYPE_ALL:
            if (MMISEARCH_STATUS_END == MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_APP)
            && MMISEARCH_STATUS_END == MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_CL)
            && MMISEARCH_STATUS_END == MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_PB)
            && MMISEARCH_STATUS_END == MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_SMS)
            && MMISEARCH_STATUS_END == MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_FILE))
            {
                is_end = TRUE;
            }
            break;

        default:
            break;
    }

    return is_end;
    
}

/*****************************************************************************/
//  Description : to set search status by type 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_E type, MMISEARCH_STATUS_E search_status)
{
    BOOLEAN is_pre_async_searching = FALSE;
    BOOLEAN is_current_async_searching = FALSE;
    
    if (type == MMISEARCH_TYPE_SMS || type == MMISEARCH_TYPE_FILE
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
        || type == MMISEARCH_TYPE_PB
#endif
    )
    {
        is_pre_async_searching = MMIAPISEARCH_IsAsyncSearching();
    }
    
    s_search_context.search_status[type] = search_status;

    if (type == MMISEARCH_TYPE_SMS || type == MMISEARCH_TYPE_FILE
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
        || type == MMISEARCH_TYPE_PB
#endif
    )
    {
        is_current_async_searching = MMIAPISEARCH_IsAsyncSearching();
        if (is_pre_async_searching != is_current_async_searching)
        {
            if ((is_pre_async_searching == FALSE && is_current_async_searching)
                || (!is_current_async_searching && MMISEARCH_IsDisplayItemsEnd()))
            {
                UpdateUIDisplay(MSG_SEARCH_UPDATE_BUTTON_VIEW, TRUE);
            }
        }
    }

    if (MMISEARCH_IsSearchEnd()&&MMISEARCH_GetMatchItemsNum() <= 1 &&MMISEARCH_IsDisplayItemsEnd())
    {
        MMISEARCH_InsertPromptNoMatchesItem();
    }
}

/*****************************************************************************/
//  Description : to get search status by type 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC MMISEARCH_STATUS_E MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_E type)
{
    return s_search_context.search_status[type];
}

/*****************************************************************************/
//  Description : to check if search is need to be restart, because some match item is invalid
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsNeedSearchAgain(void)
{
    MMISEARCH_TYPE_E type = MMISEARCH_TYPE_MAX;
    
    for(type = MMISEARCH_TYPE_APP; type < MMISEARCH_TYPE_MAX; type++ )
    {
        if (s_search_context.is_need_restart[type] 
            && (s_search_context.select_type == type || s_search_context.select_type == MMISEARCH_TYPE_ALL))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : set is need to be restart
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_E type, BOOLEAN is_need_restart)
{
    s_search_context.is_need_restart[type] = is_need_restart;
}

/*****************************************************************************/
//  Description : get is need to be restart
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_GetIsNeedRestart(MMISEARCH_TYPE_E type)
{
    return s_search_context.is_need_restart[type];
}

/*****************************************************************************/
//  Description : set is need to be restart
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_ResetAllIsNeedRestart(void)
{
    SCI_MEMSET(s_search_context.is_need_restart, 0x00, MMISEARCH_TYPE_MAX*sizeof(BOOLEAN));
}

/*****************************************************************************/
//  Description : set callback func after async search stopped
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetSearchStoppedCallBack(MMISEARCH_APP_OPEN_PFUNC open_pfunc, uint32 user_data)
{
    if (PNULL != open_pfunc)
    {
        s_search_context.is_need_run_callback = TRUE;
        s_search_context.search_stopped_callback.open_pfunc = open_pfunc;
        s_search_context.search_stopped_callback.user_data = user_data;
    }
    else
    {
        s_search_context.is_need_run_callback = FALSE;
    }
}

/*****************************************************************************/
//  Description : run callback func after async search stopped
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_RunSearchStoppedCallBack(void)
{
    MMISEARCH_APP_OPEN_PFUNC open_pfunc = s_search_context.search_stopped_callback.open_pfunc;
    uint32 user_data = s_search_context.search_stopped_callback.user_data;
    BOOLEAN is_need_run_callback = s_search_context.is_need_run_callback;
    
    if (is_need_run_callback && PNULL != open_pfunc)
    {
        open_pfunc(user_data);
        s_search_context.is_need_run_callback = FALSE;
    }
}

/*****************************************************************************/
//  Description : main entry of local search
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_Entry(void)
{  
#ifdef MMI_ISTYLE_SUPPORT
	if ( MMIAPIMENU_IsInIsyleSearch() )
	{
		return ;
	}
#endif

    if (MMK_IsOpenWin(MMISEARCH_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMISEARCH_MAIN_WIN_ID);
        MMK_CloseWin(MMISEARCH_TYPE_OPT_WIN_ID);
#if  !defined MMI_PDA_SUPPORT // hermann liu
        MMK_CloseWin(MMISEARCH_TYPE_OPT_MENU_WIN_ID);
#endif
    }
    
    Search_Init();
    
    MMISEARCH_OpenMainWin();
}

#ifdef MMISEARCH_SUPPORT_ONLYMMS
/*****************************************************************************/
//  Description : main entry of local search
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCHMMS_Entry(void)
{  
#if defined(SEARCH_SUPPORT)&&defined(MMI_ISTYLE_SUPPORT)
	if ( MMIAPIMENU_IsInIsyleSearch() )
	{
		return ;
	}
#endif

    if (MMK_IsOpenWin(MMISEARCH_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMISEARCH_MAIN_WIN_ID);
        MMK_CloseWin(MMISEARCH_TYPE_OPT_WIN_ID);
#if  !defined MMI_PDA_SUPPORT // hermann liu
        MMK_CloseWin(MMISEARCH_TYPE_OPT_MENU_WIN_ID);
#endif
    }
    
    Search_Init();
    s_search_context.select_type = MMISEARCH_TYPE_SMS;
    s_search_context.is_only_mms = TRUE;
    MMISEARCH_OpenMainWin();
}
#endif

/*****************************************************************************/
//  Description : 保存匹配数据至数据区
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_SaveMatchItems(MMISEARCH_DATA_T match_data)
{
    MMISEARCH_SAVE_RESULT_E save_result = SEARCH_SAVE_RESULT_MAX;
    BOOLEAN ret=FALSE;
    save_result = SaveMatchItems(match_data);

    switch (save_result)
    {
        case SEARCH_SAVE_SUCCESS:
            AppendItemToMatchedList();
            ret = TRUE;
            break;
            
        case SEARCH_SAVE_SUCCESS_SEGMENT:
            AppendItemToMatchedList();
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SEARCH_LIST_IS_FULL,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;

        case SEARCH_SAVE_INVALID_DATA:
            //SCI_TRACE_LOW:"MMIAPISEARCH_SaveMatchItems: SAVE match data error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISEARCH_APP_1089_112_2_18_2_49_56_0,(uint8*)"");
            break;
            
        case SEARCH_SAVE_NO_SPACE:
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SEARCH_LIST_IS_FULL,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        
        case SEARCH_SAVE_RESULT_MAX:
        default:
            break;
    }

    return ret;
}

/*****************************************************************************/
//  Description : 启动搜索
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_StartSearch(MMISEARCH_TYPE_E type, MMI_STRING_T *keyword_str)
{  
    if (type > MMISEARCH_TYPE_ALL 
        || PNULL == keyword_str || PNULL == keyword_str->wstr_ptr || 0 == keyword_str->wstr_len)
    {
        return ;
    }
    
    MMIAPISEARCH_Entry();

    s_search_context.select_type = type;
    UpdateUIDisplay(MSG_SEARCH_START_SEARCH, TRUE);
}

/*****************************************************************************/
//  Description : to check if process of search is doing by type, especially for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_E type)
{
    return (MMISEARCH_STATUS_START == s_search_context.search_status[type]);
}

/*****************************************************************************/
//  Description : to check if process of search is doing or done already by type, especially for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsSearched(MMISEARCH_TYPE_E type)
{
    if ( (MMISEARCH_TYPE_ALL == s_search_context.select_type || type == s_search_context.select_type)
         && (MMISEARCH_STATUS_START == s_search_context.search_status[type] || MMISEARCH_STATUS_END == s_search_context.search_status[type]))
     {
        return TRUE;
     }
     else
     {
        return FALSE;
     }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL void HandlePBEventFunc(MMIEVENT_PB_EVENT_E event)
{
    if((MMIEVENT_PB_ADD == event)
       || (MMIEVENT_PB_DELETE == event)
       || (MMIEVENT_PB_UPDATE == event))
    {
        MMIAPISEARCH_AppUpdateInd(MMISEARCH_TYPE_PB);
    }
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_AppUpdateInd(MMISEARCH_TYPE_E type)
{ 
    if (MMIAPISEARCH_IsSearched(type))
    {
        MMISEARCH_SetIsNeedRestart(type, TRUE);

        switch (type)
        {
            case MMISEARCH_TYPE_APP:
            case MMISEARCH_TYPE_CL:
#ifndef MMIPB_SEARCH_CONTACT_CONTENT
            case MMISEARCH_TYPE_PB:
#endif
                break;
                
            case MMISEARCH_TYPE_SMS:
            case MMISEARCH_TYPE_FILE:
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
            case MMISEARCH_TYPE_PB:
#endif
                if (MMIAPISEARCH_IsSearching(type))
                {
                    MMIAPISEARCH_StopSearching(type);
                }                
                break;
                
            case MMISEARCH_TYPE_ALL:
                break;
                
            default:
                break;
        }
    }
}

/*****************************************************************************/
//  Description : to exit search operation especially for asyn-search (stop)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_ExitSearching(MMISEARCH_TYPE_E type)
{
//    MMK_CloseWin(MMIFMM_PICTURE_PREVIEW_WIN_ID);
    MMIAPISET_CloseValidatePrivacyWin(MMISET_PROTECT_MYDOC);
    
    if (MMIAPISEARCH_IsSearched(type))
    {
        MMIAPISEARCH_StopSearching(MMISEARCH_TYPE_FILE);
        
        MMISEARCH_DeleteMatchItemsByType(type);
    }
}

/*****************************************************************************/
//  Description : async search call this func after search stopped 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_E type)
{
    switch (type)
    {
        case MMISEARCH_TYPE_SMS:          
        case MMISEARCH_TYPE_FILE:
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
        case MMISEARCH_TYPE_PB:
#endif
            MMISEARCH_SetSearchStatus(type, MMISEARCH_STATUS_END);
            break;

        default:
            break;
    }

    if (MMISEARCH_IsSearchEnd() && s_search_context.is_need_run_callback)
    {
        MMISEARCH_RunSearchStoppedCallBack();
    }

}

/*****************************************************************************/
//  Description : to stop search operation especially for asyn-search 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_StopSearching(MMISEARCH_TYPE_E type)
{	
    switch (type)
    {
        case MMISEARCH_TYPE_APP:
        case MMISEARCH_TYPE_CL:
#ifndef MMIPB_SEARCH_CONTACT_CONTENT
        case MMISEARCH_TYPE_PB:
#endif
        case MMISEARCH_TYPE_ALL:      
            MMISEARCH_SetSearchStatus(type, MMISEARCH_STATUS_END);
            break;
            
        case MMISEARCH_TYPE_SMS:
            if (MMIAPISEARCH_IsSearching(type))
            {
                MMIAPISMS_StopSearchSms();
            }
            break;
            
        case MMISEARCH_TYPE_FILE:
            if (MMIAPISEARCH_IsSearching(type))
            {
                MMISEARCH_StopAsyncUpdataTimer();
                MMIAPIFMM_SearchFileStop();
            }
            break;

#ifdef MMIPB_SEARCH_CONTACT_CONTENT
        case MMISEARCH_TYPE_PB:
            if (MMIAPISEARCH_IsSearching(type))
            {
                MMISEARCH_StopPbAsyncUpdataTimer();
                MMIAPIPB_StopPbAsyncSearch();
            }
            break;
#endif
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : to get if async searching is runing 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsAsyncSearching(void)
{
    if (MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_SMS) || MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_FILE)
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
        || MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_PB)
#endif
    )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_StopAsyncSearch(void)
{
    MMIAPISEARCH_StopSearching(MMISEARCH_TYPE_SMS);
    MMIAPISEARCH_StopSearching(MMISEARCH_TYPE_FILE);
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    MMIAPISEARCH_StopSearching(MMISEARCH_TYPE_PB);
#endif
}

/*****************************************************************************/
//  Description : set search window handle and list handle
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_SetSearchWinHandle(
                                        MMI_HANDLE_T    win_handle
                                        )
{
    win_handle = MMK_ConvertIdToHandle( win_handle );
	
    s_search_context.search_win_handle = win_handle;
}

/*****************************************************************************/
//  Description : set search window handle and list handle
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMISEARCH_GetSearchWinHandle(
                                          void
                                          )
{
    return s_search_context.search_win_handle;
}

/*****************************************************************************/
//  Description : istyle search entry of local search
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_IstyleEntry(void)
{  
    if (!s_istyle_first_entry)
    {
        Search_Init();

        s_istyle_first_entry = TRUE;
    }
}

/*****************************************************************************/
//  Description : stop search for i-style
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_IstyleStop(void)
{  
    MMISEARCH_StopAsyncSearch(); 
}

/*****************************************************************************/
//  Description : leave i-style search
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_IstyleExit(void)
{  
    MMISEARCH_StopAsyncSearch(); 
    MMISEARCH_Free();
    s_istyle_first_entry = FALSE;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_STRING_T MMISEARCH_GetKeyWord()
{
    return s_search_context.search_keyword_str;
}
#endif
