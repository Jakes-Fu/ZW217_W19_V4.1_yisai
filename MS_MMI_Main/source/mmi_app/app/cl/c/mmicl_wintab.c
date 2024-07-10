/*****************************************************************************
** File Name:      mmicl_wintab.c                                            *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng     Creat
******************************************************************************/

#define _MMICL_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cl_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmicl_export.h"
#include "mmi_nv.h"
#include "guilistbox.h"
#include "mmipb_export.h"
//#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiphone_export.h"
#include "guimsgbox.h"
#include "mmisms_export.h"
#include "mmimms_export.h"
#include "mmi_textfun.h"
#include "mmisms_text.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "mmicc_id.h"
#include "mmicc_menutable.h"
#include "mmicc_nv.h"
//#include "mmiset_nv.h"
#include "mmi_text.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmicl_internal.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "guitab.h"
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
#include "mmimp_location.h"
#include "mmiphonesearch_export.h"
#endif
#include "mmi_mainmenu_export.h"
#include "mmiudisk_export.h"
#include "mmivt_export.h"
#include "mmiussd_export.h"
#include "mmk_app.h"
#include "guirichtext.h"
#include "mmicom_string.h"
//#include "mmiset_wintab.h"
//#include "mmiset_display.h"
#ifdef ASP_SUPPORT
#include "Mmiasp_cortrol_layer.h"
#include "mmiasp_export.h"
#endif
#include "guictrl_api.h"

#ifdef MMI_PDA_SUPPORT
#include "guibutton.h"
#include "mmipb_adapt.h"
#endif


#include "guitoolbar.h"

#ifdef MMI_GPRS_SUPPORT
#include "mmipdp_export.h"
#endif

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
#include "mmibt_export.h"
#endif
#ifdef MMI_PDA_SUPPORT
#include "guires.h"
#endif
#include "mmipb_text.h"
#include "mmibt_text.h"
#include "mmipb_image.h"

#include "guidropdownlist.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h" 
#endif
#include "mmipb_view.h"

#ifdef BT_DIALER_SUPPORT
#include "mmicc_internal.h"
#include "mmibt_export.h"
#include "mmibt_id.h"
#ifdef WIN32
#include "mmipb_task.h"
#endif
#endif

//#include "mmipb_adapt.h"
#ifdef MMI_CL_TOOLBAR_SUPPORT
#undef MMI_CL_TOOLBAR_SUPPORT
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define     MMICL_DISPLAY_TIME_LEN  9
#define     MMICL_DISPLAY_DATE_LEN  11

#define     MMICL_TIMER_MAX (3600*1000-1) //call time display ma

#define     MMICL_DETAIL_HEADING_WIDTH			90
#define     MMICL_DETAIL_HEADING_RIGHT              (MMICL_DETAIL_HEADING_WIDTH - 10)

#define     MMICL_DETAIL_LEFT_MARGIN    5
#define     MMICL_DETAIL_RIGHT_MARGIN   5
#define     MMICL_DETAIL_LINE_SPACE     5


#define MMICL_TOOLBAR_ICON_GAP_TOP      5
#define MMICL_TOOLBAR_ICON_GAP_RIGHT    10
#define MMICL_TOOLBAR_PANEL_GAP         25

#define MAX_CALL_TIME_LEN   4

// 通话计时的item的高度
#define		MMI_CL_ITEM_HIGHT					30
#define     MMI_CL_CALL_TIME_LEFT               5
#ifdef VT_SUPPORT
//@zhaohui add
#define		MMICL_IMAGE_INC				(IMAGE_CL_DIALED_VTCALL_ICON - IMAGE_CL_DIALED_CALL_ICON)//通话记录中可视电话小图标image_id相对于普通电话小图标image_id的增量
#endif

#define MMICL_MAX_SEARCH_LEN     20

/**--------------------------------------------------------------------------*
**                         PDA  MultiSIM                                     *
    ICON    NAME/NUMBER              |-|
            SimName Date    Time     |-|
**                         PDA  SingleSIM                                     *
    ICON    NAME/NUMBER              |-|
            Date    Time             |-|
**                         TK  MultiSIM                                      *


**                         BAR  MultiSIM                                      *


**--------------------------------------------------------------------------*/

//call log list item中各项显示序列
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
#ifdef TOUCH_PANEL_SUPPORT
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#define CL_LIST_ITEM_TYPE_INDEX 0               //Image ID
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 1        //Contact Name or Number
#define CL_LIST_ITEM_SIM_INDEX  2                //Sim Name
#define CL_LIST_ITEM_DATE_INDEX 3               //Date
#define CL_LIST_ITEM_TIME_INDEX 4               //Time
#else
#define CL_LIST_ITEM_TYPE_INDEX 0               //Image ID
#define CL_LIST_ITEM_SIM_INDEX 1                //Sim Name
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 2        //Contact Name or Number
#define CL_LIST_ITEM_DATE_INDEX 3               //Date
#define CL_LIST_ITEM_TIME_INDEX 4               //Time
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#endif
#else
#ifdef TOUCH_PANEL_SUPPORT
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#define CL_LIST_ITEM_TYPE_INDEX 0               //Image ID
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 1        //Contact Name or Number
#define CL_LIST_ITEM_SIM_INDEX  4                //Sim Name shouldnot be displayed
#define CL_LIST_ITEM_DATE_INDEX 2               //Date
#define CL_LIST_ITEM_TIME_INDEX 3               //Time
#else
#define CL_LIST_ITEM_TYPE_INDEX 0
#define CL_LIST_ITEM_SIM_INDEX 0
#define CL_LIST_ITEM_NUM_OR_NAME_INDEX 1
#define CL_LIST_ITEM_DATE_INDEX 2
#define CL_LIST_ITEM_TIME_INDEX 3
#define CL_LIST_ITEM_ICON_INDEX 5               //ICON/Selected ICON
#endif
#endif

#ifdef CALL_TIMES_SUPPORT
#define CL_CT_LIST_ITEM_TYPE_INDEX 1
#define CL_CT_LIST_ITEM_SIM_INDEX 3
#define CL_CT_LIST_ITEM_CONTACT_INDEX 2
#define CL_CT_LIST_ITEM_DATE_INDEX 4
#define CL_CT_LIST_ITEM_TIME_INDEX 5

#endif
#ifdef MMI_GPRS_SUPPORT//for gprs flowrate
#define MMICL_ONE_K_LEN                    1024
#define MMICL_DSP_STR_LEN                  16
#endif

#ifdef MMI_PDA_SUPPORT
#define CL_SAVE_MAX_LEN (PHONE_NUM_MAX_LEN + 10)
//#define CL_SELECT_LABLE_LEN 55
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
//add by bingjie.chen
typedef enum
{
    MISSED_NUMBER1,
    DAILED_NUMBER2,
    RECEIVED_NUMBER3,
#ifdef MMI_BLACKLIST_SUPPORT
    REJECTED_NUMBER4,
#endif
    DELETE_NUMBER5,
    TIMER_NUMBER6,
#ifdef MMI_GPRS_SUPPORT
    GPRS_NUMBER7, 
#endif
    NUMBER_MAX
}CL_NUMBER_LIST_E;
#endif

typedef enum
{
    CL_TOOLBAR_TYPE_INIT,
    CL_TOOLBAR_DIAL_AUDIO,
    #ifdef VT_SUPPORT
    CL_TOOLBAR_DIAL_VIDEO,
    #else
    CL_TOOLBAR_DIAL_IPCALL,
    #endif
    CL_TOOLBAR_DIAL_SMS,
    CL_TOOLBAR_DIAL_VIEW
}CL_TOOLBAR_TYPE_E;

typedef struct
{
    BOOLEAN			need_select_return;
    MMI_WIN_ID_T	return_win_id;
}MMICL_SELECT_AND_RETURN_INFO_T;

typedef struct 
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2];
    MN_DUAL_SYS_E dual_sys;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMICL_NAME_T    name; 
#endif
}MMICL_DETAIL_INFO_T;

#ifdef MMI_PDA_SUPPORT
/*for support add contacts from calllog to message-edit win*/ 
typedef struct 
{
    BOOLEAN is_valid;
    //uint16 list_index;
}MMICL_SELECT_ITEM_INFO_T;

typedef struct 
{
    uint16 sel_count;
    //BOOLEAN is_all_selected;
    MMICL_SELECT_ITEM_INFO_T sel_item[MMICL_RECORD_MAX_NUM + 1];
}MMICL_ALL_SELECT_INFO_T;
#endif

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
//call log窗口返回信息，主要处理按键之后，是否需要返回，以及返回哪个子窗口
LOCAL MMICL_SELECT_AND_RETURN_INFO_T s_select_return_info = {0}; 

//记录call log详情窗口中的相关信息
LOCAL MMICL_DETAIL_INFO_T *s_call_log_detail_info_ptr = PNULL;

//根据设定更新WIN
typedef struct 
{
    BOOLEAN is_cl_need_update;
    MMI_HANDLE_T    updated_win_handle;
}MMICL_UPDATED_DATA_TAG;

LOCAL MMICL_UPDATED_DATA_TAG g_cl_updated_Data  = {0};

#ifdef MMI_PDA_SUPPORT
//从msg界面进入call log时，记录所有被选中的item的相关信息
LOCAL MMICL_ALL_SELECT_INFO_T s_call_log_all_select_info = {0};
#endif

#ifdef MMI_CL_MINI_SUPPORT
typedef struct 
{
    BOOLEAN       is_select;    //记录是否进入标记状态
    BOOLEAN log_selected_index[MMICL_RECORD_TOTAL_NUM]; //记录控件中每个电话记录的标记状态
}MMICL_SELECT_INFO_T;

LOCAL MMICL_SELECT_INFO_T s_select_info = {0};

#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL THEMELIST_ITEM_STYLE_T*   s_itemstyle_ptr = PNULL;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMICL_CALL_TYPE_E   s_cur_sel_log_list_type;//当前被选中的log list的类型

//call log data 
typedef struct
{
    MMICL_CALL_TYPE_E   g_cur_sel_filter_type;                      //List Filter  Type
    MMIPB_CONTACT_T *   matched_contact;                            //联系人信息 
    uint16              contact_matched_number;                     //联系人匹配的数目
    MMI_HANDLE_T        cl_list_handle ;                            //Applet机制需要的Handle
    MMI_HANDLE_T        main_win_id;
    BOOLEAN             isSelectingItem;                            //是否正在进行删除选择
    BOOLEAN             contact_selected_index[MMICL_MAX_MATCH_ITEM];//通话记录列表选择
    BOOLEAN             isCalllogSelected;
}MMICL_PB_CALLLOG_T;

MMICL_PB_CALLLOG_T  *g_pbcalllog_data=PNULL;        //全局变量
LOCAL uint32 s_mmicl_match_item_index = 0;
LOCAL MMICL_MATCH_TOTAL_INFO_T *s_match_call_info_ptr = PNULL; //CL本地搜索到的匹配记录

LOCAL wchar* s_match_key_ptr = PNULL; //CL本地搜索关键字字符串
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
LOCAL BOOLEAN s_is_multiselecting = FALSE;
LOCAL BOOLEAN s_log_selected_index[MMICL_MAX_MATCH_ITEM]={0};
#endif

#ifdef BT_PBAP_SUPPORT
#define MMIAPICLBT_PBAP_VCARD_FORMAT_21         BT_VCARD_FORMAT_21         
#define BT_PBAP_TIMER_OUT            (30*1000)       //同步联系人请求响应过时保护时长

LOCAL BOOLEAN s_is_cancel_pull = FALSE; 
LOCAL BOOLEAN s_is_missed_allow_sync = TRUE;
LOCAL BOOLEAN s_is_dialed_allow_sync = TRUE;
LOCAL BOOLEAN s_is_received_allow_sync = TRUE;
LOCAL uint8 s_pbap_timeout_timer = 0;

LOCAL const char s_ich_name[] = {'t','e','l','e','c','o','m','\/','i','c','h','.','v','c','f',0}; /*lint !e606*/
LOCAL const char s_och_name[] = {'t','e','l','e','c','o','m','\/','o','c','h','.','v','c','f',0}; /*lint !e606*/
LOCAL const char s_mch_name[] = {'t','e','l','e','c','o','m','\/','m','c','h','.','v','c','f',0}; /*lint !e606*/
LOCAL const char s_cch_name[] = {'t','e','l','e','c','o','m','\/','c','c','h','.','v','c','f',0}; /*lint !e606*/
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : SetAllLogFillter
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetAllLogFillter(MMICL_CALL_TYPE_E filter_type);
#endif

/*****************************************************************************/
// 	Description : GetAllLogFillter
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMICL_CALL_TYPE_E GetAllLogFillter(void);

#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get current tool bar log list control id
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL CC_CONTROL_ID_E GetLogToolBarCtrlId(MMI_WIN_ID_T  win_id);
#endif
#endif
#endif

#ifdef MMI_PDA_SUPPORT
LOCAL BOOLEAN SetAllLogFillterPBNumber(MMIPB_CONTACT_T    *contact_ptr);
LOCAL MMIPB_BCD_NUMBER_T* GetAllLogFillterPBNumber(uint16 index);   
#endif

#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
/*****************************************************************************/
// 	Description : start multiselect 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void BeginMultiSelecting(void);
/*****************************************************************************/
// 	Description : exit multiselect
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitMultiSelecting(void);
/*****************************************************************************/
// 	Description : to is multiselecting
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isMultiSelecting(void);
/*****************************************************************************/
// 	Description : check current index is selecting or not
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isCurrentSelected(uint16 index);
/*****************************************************************************/
// 	Description : Set current index 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected);
#endif

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleBtCall(void);
#endif

#ifdef BT_DIALER_SUPPORT
#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : CreatePbapTimer
//  Global resource dependence : 
//  Author: cheney.wu
//  Note: 创建超时定时器
/*****************************************************************************/
LOCAL void CreatePbapTimer(void);

/******************************************************************************/
// Description: ProcessPbapTimerOver
// Global resource dependence: 
// Author: cheney.wu
// Note: 超时保护机制
/******************************************************************************/
LOCAL void ProcessPbapTimerOver(uint8 timer_id, uint32 param);
/*****************************************************************************/
//  Description : to initiate a transport disconnection. 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note: BT_PbapDisconnect will cause the transport to be brought down automatically, 
//  but if the transport must be brought down immediately, perhaps due an error condition, 
//  this routine may be used. 
/*****************************************************************************/
LOCAL void PbapClientTpDisconnect(void);
#endif
#endif

/*****************************************************************************/
// 	Description : API for call log data control
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
//    Call log data
//        Init/Free   API
//        Get/Set APIS
//            ListHandle
//            AllLogFillter
//            AllLogFillterPBNumber
//            GetAllLogFillterPBNumberType
//        InitMatchedItem/AddMatchedItem/GetMatchedItemCount
/*****************************************************************************/
LOCAL BOOLEAN FreePbCalllogData(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    //Don't need free contact ptr    
    SCI_FREE(g_pbcalllog_data);
    
    return TRUE;
}
LOCAL MMI_HANDLE_T  CalllogPBData_GetMainWinID(void)
{
    if (g_pbcalllog_data ==PNULL)
        return PNULL;

    return g_pbcalllog_data->main_win_id;
}

LOCAL void  CalllogPBData_SetMainWinID(MMI_HANDLE_T main_win_id)
{
    if (g_pbcalllog_data ==PNULL)
        return;

    g_pbcalllog_data->main_win_id= main_win_id;
    return;

}

LOCAL BOOLEAN InitPbCalllogData(MMI_WIN_ID_T parent_win_id)
{
    if (g_pbcalllog_data !=PNULL)
    {   
       FreePbCalllogData();    
    }
    
    g_pbcalllog_data=SCI_ALLOC_APPZ(sizeof(MMICL_PB_CALLLOG_T));
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
    
    CalllogPBData_SetMainWinID(parent_win_id);
    return TRUE;
}


LOCAL MMI_HANDLE_T   CalllogPBData_GetListHandle(void)
{
    if (g_pbcalllog_data!=PNULL)
    {
        return g_pbcalllog_data->cl_list_handle;
    }
    return PNULL;
}

LOCAL BOOLEAN   CalllogPBData_SetListHandle(MMI_HANDLE_T list_handle)
{
    if ((g_pbcalllog_data!=PNULL) &&(list_handle!=PNULL))
    {
        g_pbcalllog_data->cl_list_handle =list_handle ;
        return TRUE;
    }
    return FALSE;
}

LOCAL BOOLEAN SetAllLogFillter(MMICL_CALL_TYPE_E filter_type)
{

    if (filter_type >=MMICL_CALL_TYPE_MAX)
        return FALSE;

    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    g_pbcalllog_data->g_cur_sel_filter_type = filter_type;

    if (MMICL_CALL_PB_NUMBER_MATCHED ==filter_type)
    {
        g_pbcalllog_data->contact_matched_number= 0;
    }
    
    return TRUE;
}

LOCAL MMICL_CALL_TYPE_E GetAllLogFillter(void)
{
    if (g_pbcalllog_data ==PNULL)
        return MMICL_CALL_DIALED;

    return g_pbcalllog_data->g_cur_sel_filter_type;
}

LOCAL uint16    GetCalllogPBData_MatchedItemCount(void)
{
    if (g_pbcalllog_data==PNULL)
        return 0;
    return  g_pbcalllog_data->contact_matched_number;
}

LOCAL BOOLEAN SetAllLogFillterPBNumber(MMIPB_CONTACT_T    *contact_ptr)
{

    if (contact_ptr ==PNULL)
        return FALSE;
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    g_pbcalllog_data->matched_contact= contact_ptr;
    return TRUE;
}

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
LOCAL MMIPB_BCD_NUMBER_T* GetAllLogFillterPBNumber(uint16 index)
{
    if (MMICL_CALL_PB_NUMBER_MATCHED != GetAllLogFillter())
        return PNULL;

    if (g_pbcalllog_data ==PNULL)
        return PNULL;
        
    if (PNULL == g_pbcalllog_data->matched_contact)
        return PNULL;

    if (index>MMIPB_MAX_NV_PHONE_NUM)
        index = 0;
        
    return &(g_pbcalllog_data->matched_contact->number[index]);
}
LOCAL uint8 GetAllLogFillterPBNumberType(uint16 index)
{
    uint8   number_type;
    if (MMICL_CALL_PB_NUMBER_MATCHED != GetAllLogFillter())
        return PNULL;

    if (g_pbcalllog_data ==PNULL)
        return FALSE;
        
    if (PNULL == g_pbcalllog_data->matched_contact)
        return PNULL;

    if (index>=MMIPB_MAX_NV_PHONE_NUM)
        index = 0;

    number_type = g_pbcalllog_data->matched_contact->num_type[index];
    
    //if(MMIPB_IsUsim(g_pbcalllog_data->matched_contact->storage_id))
        {  
    //        number_type = number_type-1;
        }    
        
    return  number_type;
}

LOCAL void CalllogPBData_InitMatchedItem(void)
{
    if (g_pbcalllog_data ==PNULL)
        return ;
     g_pbcalllog_data->contact_matched_number=0 ;
}

LOCAL BOOLEAN CalllogPBData_AddMatchedItemCount(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
     if (g_pbcalllog_data->contact_matched_number>=MMICL_MAX_MATCH_ITEM)
        return FALSE;
     g_pbcalllog_data->contact_matched_number++ ;
        
    return TRUE;
}
#endif


/*****************************************************************************/
// 	Description : API for call log     Call log selection
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
//    Control APIS
//        Start/Stop/IsSelecting
//    Get/Set APIS
//        SetAllSelected
//        SelectedByIndex
/*****************************************************************************/
LOCAL BOOLEAN   CalllogPBData_StartSelection(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    g_pbcalllog_data->isSelectingItem = TRUE;
    SCI_MEMSET(g_pbcalllog_data->contact_selected_index,0x00,sizeof(g_pbcalllog_data->contact_selected_index) );
    return TRUE;
}
LOCAL BOOLEAN   CalllogPBData_StopSelection(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    g_pbcalllog_data->isSelectingItem = FALSE;

    return TRUE;
}
LOCAL BOOLEAN CalllogPBData_IsSelecting(void)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
    return g_pbcalllog_data->isSelectingItem;
}


LOCAL BOOLEAN CalllogPBData_GetIsSelectedByIndex(uint16 index)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    return g_pbcalllog_data->contact_selected_index[index];

}

LOCAL BOOLEAN CalllogPBData_SetSelectedByIndex(uint16 index,BOOLEAN isSelected)
{
    if (g_pbcalllog_data ==PNULL)
        return FALSE;
    g_pbcalllog_data->contact_selected_index[index] = isSelected;

    return TRUE;
}

LOCAL BOOLEAN CalllogPBData_SetAllSelected(BOOLEAN isSelected)
{
    uint16  i;
    if (g_pbcalllog_data ==PNULL)
        return FALSE;

    for (i=0;i<MMICL_MAX_MATCH_ITEM;i++)
    {
        g_pbcalllog_data->contact_selected_index[i] = isSelected;
    }
    return TRUE;
}

#ifndef MMI_CALLLOG_HIDE_MAINMENU
/*****************************************************************************/
// 	Description : to handle the call log main menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCLMainMenuWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
#endif

/*****************************************************************************/
// 	Description : to set call log list item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListItem(void);

/*****************************************************************************/
// 	Description : load call log list into listbox
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             );
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to set call log select list item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void SetLogSelectListItem(void);

/*****************************************************************************/
// 	Description : load call log list into listbox for sms/mms
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItemForMessage(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             );
/*****************************************************************************/
//  Description : MMICC_CreateNewSoftKey
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMICC_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_HANDLE_T form_id,
                            MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, 
                            MMI_TEXT_ID_T rightkey_id);
#endif

/*****************************************************************************/
// 	Description : to handle the call log list menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add the num to blacklist   
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void AddNumToBlackList(
                        MMIPB_BCD_NUMBER_T  pb_bcd    //IN:
                        );
#endif

/*****************************************************************************/
// 	Description : to handle the call log list window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListWindow(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );

/*****************************************************************************/
// 	Description : to handle the child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleCallLogChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#if 0
/*****************************************************************************/
// 	Description : to handle the dialed child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDialedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to handle the received child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceivedChildWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : to handle the missed child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMissedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : to handle the refused child window message
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRefusedChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
#endif
#endif

#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
#if defined MMI_ISTYLE_SUPPORT
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
#endif
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleSelectAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );

/*****************************************************************************/
// 	Description : to handle select contact from call log child window message
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );


#if 0
// 	Description : set call log title button background
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_SetButtonBG(MMI_CTRL_ID_T ctrl_id, uint32 button_index);
#endif

#endif

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint16 GetCurrTypeNum(MMICL_CALL_TYPE_E      call_type, MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info);

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Get menu offset by calltype
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint16 GetMenuIDOffsetByCallType(MMICL_CALL_TYPE_E call_type);
#endif

/*****************************************************************************/
//	Description : the handle function of call list delete menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_Istyle_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#endif

/*****************************************************************************/
// 	Description : set detail of call log list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListDetail(void);

/*****************************************************************************/
// 	Description : to handle the call log list detail window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListDetailWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : delete current recode item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteCurrentRecord(void);

/*****************************************************************************/
// 	Description : ring back (回复电话)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBack(CC_CALL_SIM_TYPE_E call_sim_type, 
                                                BOOLEAN is_video_call
                                                );
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
// 	Description : add phone number to name 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAddNumToName(void);
#endif

#if defined(CL_SAVE_NAME_SUPPORT)
/*****************************************************************************/
// 	Description : get display phone name of current log list 
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayPhoneName(
                              MMI_STRING_T*   tele_num//out
                              );
#endif

/*****************************************************************************/
// 	Description : get display phone number of current log list 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void GetDisplayPhoneNum(
                              uint8*   tele_num, //out
                              uint8	   max_tele_len,
                              MN_DUAL_SYS_E *dual_sys_ptr
                              );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get dual sys and isvideocall of current log list 
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void GetCallLogInfoDetail(MN_DUAL_SYS_E *dual_sys_ptr,BOOLEAN *isVideoCall);
#endif

/*****************************************************************************/
// 	Description : distill phone number from call log 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleDistillPhoneNum(void);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : add a number into forbidden number list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleForbiddenNum(void);
#endif

/*****************************************************************************/
// 	Description : get bcd phone number of current log list 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void GetPartyPhoneNum(
                            MMIPB_BCD_NUMBER_T*   bcd_number//out
                            );

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerSms(void);

//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : ring back (回复电话)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBackByType(uint32 call_type);
//#endif

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerMms(void);

/*****************************************************************************/
// 	Description : delete all call log with menu id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteAllCallLog(
                            MMICL_CALL_TYPE_E  call_type//in
                            );

/*****************************************************************************/
// 	Description : 获取当前选择的Calllog在arrayInfo中的索引值
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL uint16 CL_GetArrayIndexofCurSelected(void);

/*****************************************************************************/
// 	Description : 打开当前选择的Calllog的时间列表窗口
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void CL_OpenCallTimeListOrDetailWin(MMICL_CALL_ARRAY_BCDINFO_T * callinfo);

/*****************************************************************************/
// 	Description : display the time of last call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimerWindow(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : display call time in control
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            uint8 line, 
                            CC_NV_ITEM_E   nv_item, //@$discuss 6800????CC_NV_ITEM_E,????????MMI_NV_ITME_E
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            );
#else //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : set label text about last call time
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            CC_NV_ITEM_E   nv_item,
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            );
#endif //MMI_CSP_SUPPORT

/*****************************************************************************/
// 	Description : 转换uint32的时间为时：分：秒的格式
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void ConvertTimeToDisplay(uint32  call_time,//in
                                uint8   *time//out
                                );

/*****************************************************************************/
// 	Description : reset the timer
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void ResetCalllogTimer(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
// 	Description : ip call
//	Global resource dependence : 
//  Author:lin.lin
//	Note: 
/*****************************************************************************/
LOCAL void HandleIpCall(void);
#endif

/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows(void);

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
// 	Global resource dependence : 
// Author: Cheney.Wu
// 	Note: 
/*****************************************************************************/
LOCAL void  AddSeparateTabsAndChildWindows(void);

/*****************************************************************************/
// 	Description : Get ctrl id for different win id
// 	Global resource dependence : 
// Author: cheney.wu
//	 Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsBTCalllogWinId(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : delete all call log with menu id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteAllPbapCallLog(void);
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : to handle sim select window
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  CallTimeSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

#endif

/*****************************************************************************/
// 	Description : set one item contained head & content in richtext
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //标题文字id
                               MMI_CTRL_ID_T ctrl_id,            //所在的控件容器
                               MMI_STRING_T content_str,         //显示内容
                               uint16   *index
                               ); 

#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : handle toolbar item sel 
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_HandleToolBarItemSel(CL_TOOLBAR_TYPE_E node_id);

/*****************************************************************************/
// 	Description : createtoolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
// 	Description : update cl toolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_UpdateCLWinToolbar(MMI_WIN_ID_T      win_id,MMI_CTRL_ID_T       list_ctrl_id);
#endif
#endif
#endif

#if defined MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E  HandleLogListMenukeyWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#ifdef MMI_PDA_SUPPORT
LOCAL void DrawListItemIconData(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info,GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr);
#endif

#ifndef MMI_CALLLOG_HIDE_MAINMENU
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCallLogItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallLogListCtrl(void);
#endif

#endif



#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : display gprs flow rate
//	Global resource dependence : 
//  Author:li.li
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGprsFlowrateWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : handle message of sim list window 
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectForGprsCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

#endif                                                

/*****************************************************************************/
//  Description : set the gprs flowrate window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGprsFlowrateWindow(MMI_CTRL_ID_T ctrl_id,MN_DUAL_SYS_E  dual_sys);

/*****************************************************************************/
// 	Description : the gprs flowrate size
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
BOOLEAN FlowrateFormSize(
                    MMI_STRING_T* str_dst,        //out.
                    uint32 size
                    );
#endif
#ifdef CALL_TIMES_SUPPORT

/*****************************************************************************/
// 	Description : load call times list into listbox
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void AppendCallTimeListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info,
                             uint16     selected_call_log_index
                             );

/*****************************************************************************/
// 	Description : to set call time list item
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListItem(MMI_CTRL_ID_T call_times_list_ctrl_id);

/*****************************************************************************/
// 	Description : set call time list title window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimeListTitle(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : to handle call time list window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimeListWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  );

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : to handle call time list option window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimesListOptMenuTitle(uint16 call_times_index);

/*****************************************************************************/
// 	Description : to handle the call times list option window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesListOptWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void DeleteOneCallTimesItem(uint16 select_index);

#endif

/*****************************************************************************/
// 	Description : set detail of call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimesListDetail(uint16 select_index);

/*****************************************************************************/
// 	Description : to handle the call times detail window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesDetailWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to handle the contact call log list menu window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailLogListMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to handle the call log list menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllLogListPopMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
#endif                                            

/*****************************************************************************/
// 	Description : to handle pub edit win to edit number before call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBeforeDialWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

                                            
/*****************************************************************************/
// 	Description : get record num by type and get record content to arraycall_info
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint8 GetRecordNumByType(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info_ptr, MMICL_CALL_TYPE_E e_call_type);

/*****************************************************************************/
// 	Description : Get Current Record Info for next operation
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void GetCurrentRecordInfo(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info , uint16* sel_item_index);

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 指定MMICL_CALL_BCDINFO_T中生成MMIPB_BCD_NUMBER_T
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_ConvertBCDNumberFromCallInfo(MMIPB_BCD_NUMBER_T* pb_num_ptr,MMICL_CALL_BCDINFO_T* callinfo);

/*****************************************************************************/
// 	Description : 从当前的MMICL_CALL_BCDINFO_T中生成MMIPB_BCD_NUMBER_T  
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetCurrentBCDNumber(MMIPB_BCD_NUMBER_T* pb_num_ptr);

/*****************************************************************************/
// 	Description : 从指定的MMICL_CALL_BCDINFO_T,获取此号码对应的号码类别
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetNumberTypeStrByNumber(wchar* name_str,MMICL_CALL_BCDINFO_T* callinfo);

/*****************************************************************************/
// 	Description : 从当前的MMICL_CALL_BCDINFO_T，获取当前号码并判断是否在联系人列表中
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckCurrentNumberInContact(void);

/*****************************************************************************/
// 	Description : 从当前的MMICL_CALL_BCDINFO_T，打开联系人详情
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void  OpenNumberInContact(void);
#endif

/*****************************************************************************/
// 	Description : 获得当前需要更新的LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T MMICL_GetUpdatedWinHandle();


/*****************************************************************************/
// 	Description : 获取通话记录的联系人姓名或者号码显示
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  需要加入 Calltimes
          //如果是联系人的通话记录，需要转换为号码类别
/*****************************************************************************/
LOCAL BOOLEAN GetNameOrNumberContent(wchar *in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,
                                    BOOLEAN isAddCalltimes,BOOLEAN isUseNumberType);
                                    
/*****************************************************************************/
// 	Description : 获取Call type 的 ICON ID
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetCallTypeImageID(MMICL_CALL_BCDINFO_T *callinfo);

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 获取日期显示的WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  今天/昨天用资源字串表示，其他的日期用 Mon/Day 表示   不加入年份
/*****************************************************************************/
LOCAL BOOLEAN GetDateContent(MMI_STRING_T* in_str,uint16 max_len,
                MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index,BOOLEAN isNeedYear);

#endif
/*****************************************************************************/
// 	Description : 获取时间显示的WSTRING( HOUR:MIN)
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTimeContent(MMI_STRING_T* in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description :当前的记录是否需要加入到列表中
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 用于PDA下的PB中的通话记录列表
/*****************************************************************************/
LOCAL BOOLEAN IsAddCurrentLogToList(MMICL_CALL_TYPE_E call_type,MMICL_CALL_BCDINFO_T *callinfo);

/*****************************************************************************/
// 	Description : to handle CallTime pop list win
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimePopListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
#endif

#ifdef CALL_TIMES_SUPPORT
/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_DeleteOneCallTimesItem(
                                        MMICL_CALL_TYPE_E    call_type,  //call 类型
                                        uint16               select_index
                                        );
#endif

#ifdef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Set log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected);

/*****************************************************************************/
// 	Description : Set all log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetAllSelect(BOOLEAN isSelected);

/*****************************************************************************/
// 	Description : Exit mark for Set all log list item
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitSelect(void);

/*****************************************************************************/
// 	Description : to delete marked call log record item
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void DeleteRecord(void);

/*****************************************************************************/
// 	Description : menu grayed
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_LogListMenuGrayed(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : ALL item is marked
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTotalItemIsMark(void);
#endif

#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
/*****************************************************************************/
// 	Description : multiselecting  win handler
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildCallLogSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL void SetItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index);
#endif

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : Is CL LDN Record
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSimRecord(MMICL_CALL_ARRAY_BCDINFO_T *callinfo);

#endif

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
#ifndef MMI_CALLLOG_HIDE_MAINMENU
#ifdef TOUCH_PANEL_SUPPORT
// the window for call log main menu
WINDOW_TABLE( MMICL_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),
    WIN_TITLE(STXT_MAIN_CL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_MAIN_MENU_CTRL_ID),
    END_WIN
};
#else
// the window for call log main menu
WINDOW_TABLE( MMICL_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_CL, MMICL_MAIN_MENU_CTRL_ID),
    END_WIN
};
#endif

#if defined (TOUCH_PANEL_SUPPORT) || defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
// the window for call log icon main menu
WINDOW_TABLE( MMICL_ICON_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),  
    WIN_TITLE(STXT_MAIN_CL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_MAIN_MENU_CTRL_ID),   
    END_WIN
};
#else
// the window for call log icon main menu
WINDOW_TABLE( MMICL_ICON_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCLMainMenuWindow ), 
    WIN_ID( MMICL_MAIN_MENU_WIN_ID ),  
#ifdef DPHONE_SUPPORT//syy modify 2010.11.27 -begin

#else
    WIN_STATUSBAR,
#endif//syy modify 2010.11.27 -end
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_ICON_CL, MMICL_MAIN_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#endif
// the window for call log
WINDOW_TABLE( MMICL_LOG_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleLogListWindow),    
    WIN_ID( MMICL_LOG_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if defined MMI_GUI_STYLE_TYPICAL || defined MMI_CL_MINI_SUPPORT || defined MMI_GUI_STYLE_MINISCREEN
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMICL_LOG_TAB_CTRL_ID),
#else
	CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMICL_LOG_TAB_CTRL_ID),
#endif
    END_WIN
};

// the child window for dialed
WINDOW_TABLE( MMICL_DIALED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_DIALED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_DIALED_LIST_CTRL_ID),
    END_WIN
};

// the child window for received
WINDOW_TABLE( MMICL_RECEIVED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_RECEIVED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_RECEIVED_LIST_CTRL_ID),
    END_WIN
};

// the child window for missed
WINDOW_TABLE( MMICL_MISSED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_MISSED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_MISSED_LIST_CTRL_ID),
    END_WIN
};

#ifdef BT_DIALER_SUPPORT
// the child window for dialed
WINDOW_TABLE( MMICL_BT_DIALED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
        WIN_ID( MMICL_BT_DIALED_CHILD_WIN_ID ),
        //	WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_BT_DIALED_LIST_CTRL_ID),
        END_WIN
};

// the child window for received
WINDOW_TABLE( MMICL_BT_RECEIVED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
        WIN_ID( MMICL_BT_RECEIVED_CHILD_WIN_ID ),
        //	WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_BT_RECEIVED_LIST_CTRL_ID),
        END_WIN
};

// the child window for missed
WINDOW_TABLE( MMICL_BT_MISSED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
        WIN_ID( MMICL_BT_MISSED_CHILD_WIN_ID ),
        //	WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_BT_MISSED_LIST_CTRL_ID),
        END_WIN
};
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
// the child window for refused
WINDOW_TABLE( MMICL_REFUSED_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallLogChildWindow),    
    WIN_ID( MMICL_REFUSED_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_REFUSED_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_PDA_SUPPORT
// the child window for refused
WINDOW_TABLE( MMICL_ALL_CHILD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleAllChildWindow),    
    WIN_ID( MMICL_ALL_CHILD_WIN_ID ),
//	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMICL_ALL_LOG_LIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID,MMICL_ALL_LOG_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMICL_ALL_LIST_CTRL_ID,MMICL_ALL_LOG_FORM_CTRL_ID),
    
/*
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMICL_ALL_LOG_LIST_DROPLIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID,MMICL_ALL_LOG_LIST_DROPLIST_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMICL_ALL_LOG_LIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),    
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMICL_ALL_LIST_CTRL_ID,MMICL_ALL_LOG_LIST_FORM_CTRL_ID),
*/    
    END_WIN
};

/*for support add contacts from calllog to message-edit win*/ 
WINDOW_TABLE( MMICL_ALL_SELECT_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSelectAllChildWindow),    
    WIN_ID( MMICL_ALL_SELECT_CHILD_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMICL_ALL_LOG_LIST_FORM_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID, MMICL_ALL_LOG_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_CHECKLIST_E, MMICL_ALL_LIST_CTRL_ID, MMICL_ALL_LOG_FORM_CTRL_ID),
    END_WIN
};

#endif


// the window for call log list detail menu
WINDOW_TABLE( MMICL_LOG_LIST_DETAIIL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleLogListDetailWindow ),    
    WIN_ID( MMICL_LOG_LIST_DETAIIL_WIN_ID ),
    WIN_TITLE( STXT_DETAIL ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    #ifdef MMI_PDA_SUPPORT    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_LOG_LIST_DETAIL_CTRL_ID),
    #else
    CREATE_RICHTEXT_CTRL(MMICL_LOG_LIST_DETAIL_CTRL_ID),
    #endif
    END_WIN
};

// the window for display last call time
WINDOW_TABLE( MMICL_CALL_TIMER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallTimerWindow ),    
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_ID( MMICL_CALL_TIMER_WIN_ID ),
    WIN_TITLE(TXT_NULL),

    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_CL_RESET, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIMER_CTRL_ID),
    #else    
    CREATE_RICHTEXT_CTRL(MMICL_CALL_TIMER_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMICL_LOG_LIST_MENUKEY_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenukeyWindow),
    WIN_ID(MMICL_LOG_LIST_MENUKEY_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_CL_MEMU_KEY,MMICL_LOG_LIST_MENUKEY_CTRL_ID),
   
    END_WIN
};
#endif


WINDOW_TABLE( MMICL_LOG_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_LOG_LIST_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#else    
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_LOG_LIST_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif 
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMICL_LOG_LIST_MENU_WIN2_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_LOG_LIST_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#else   
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_LOG_LIST_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

#if defined(MMI_BLACKLIST_SUPPORT)
WINDOW_TABLE( MMICL_REFUSE_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_REFUSE_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#else   
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_REFUSE_OPT,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMICL_REFUSE_LIST_MENU_WIN2_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLogListMenuWindow),
    WIN_ID(MMICL_LOG_LIST_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_REFUSE_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#else 
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_REFUSE_OPT2,MMICL_LOG_LIST_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

#ifdef CALL_TIMES_SUPPORT
WINDOW_TABLE( MMICL_CALL_TIMES_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallTimeListWinMsg),    
    WIN_ID( MMICL_CALL_TIMES_WIN_ID ),
#ifndef MMI_PDA_SUPPORT
    WIN_TITLE(STXT_MAIN_CL),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_CALL_TIMES_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE, MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE, MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_TOOLBAR_ADD, MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID),
#endif
#ifdef TOUCH_PANEL_SUPPORT    
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_CALL_BUTTON,MMICL_CALL_TIMES_BTN_CALL_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
#ifdef MMI_IP_CALL_SUPPORT
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_IPCALL_BUTTON,MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
#endif
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_MESSAGE_BUTTON,MMICL_CALL_TIMES_BTN_MSG_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_CL_DETAIL_DELETE_BUTTON,MMICL_CALL_TIMES_BTN_DEL_CTRL_ID ,MMICL_CALL_TIMES_BTN_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMICL_CALL_TIMES_LIST_FORM_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMICL_CALL_TIMES_MENU_CTRL_ID, MMICL_CALL_TIMES_FORM_CTRL_ID),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIMES_MENU_CTRL_ID),
    END_WIN
};

#ifndef MMI_CL_MINI_SUPPORT
WINDOW_TABLE( MMICL_CALL_TIMES_LIST_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCallTimesListOptWindow),
    WIN_ID(MMICL_CALL_TIMES_LIST_OPTION_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CL_CALL_TIMES_LIST_OPT,MMICL_CALL_TIMES_LIST_OPT_CTRL_ID),
#else 
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CL_CALL_TIMES_LIST_OPT,MMICL_CALL_TIMES_LIST_OPT_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

// the window for call log list detail menu
WINDOW_TABLE( MMICL_CALL_TIMES_DETAIIL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallTimesDetailWindow),    
    WIN_ID( MMICL_CALL_TIMES_DETAIIL_WIN_ID ),
    WIN_TITLE( STXT_DETAIL ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    #ifdef MMI_PDA_SUPPORT    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIMES_DETAIL_CTRL_ID),
    #else
    CREATE_RICHTEXT_CTRL(MMICL_CALL_TIMES_DETAIL_CTRL_ID),
    #endif
    END_WIN
};
#endif

#ifdef MMI_GPRS_SUPPORT
WINDOW_TABLE( MMICL_GPRS_FLOWRATE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleGprsFlowrateWinMsg ),    
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_ID( MMICL_GPRS_FLOWRATE_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_CL_RESET, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_GPRS_FLOWRATE_CTRL_ID),
    #else    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMICL_GPRS_FLOWRATE_CTRL_ID),
    #endif
    
   
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMICL_PB_DETAIL_LOG_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePBDetailLogListMenuWindow),
    WIN_ID(MMICL_PB_DETAIL_CALLLOG_MENU_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_CL_PB_DETAIL_MEMU,MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMICL_ALL_LIST_POP_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAllLogListPopMenuWindow),
    WIN_ID(MMICL_ALL_LIST_POP_MENU_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_ALL_LIST_POPUP_MEMU,MMICL_ALL_LIST_POPUP_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#endif
//Use pubedit at first
WINDOW_TABLE( MMICL_EDIT_BEFORE_DIAL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditBeforeDialWindow ),    
        WIN_ID( MMICL_EDIT_BEFORE_DIAL_WIN_ID ),
        WIN_TITLE( TXT_EDIT_NUMBER_BEFORE_DIAL ),
        CREATE_EDIT_PHONENUM_CTRL(PHONE_NUM_MAX_LEN, MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
        END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMICL_CALL_TIME_POP_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCallTimePopListWindow),    
    WIN_ID( MMICL_CALL_TIME_POP_WIN_ID ),
    WIN_TITLE( TXT_ADDCONTACT ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CALL_TIME_POP_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
WINDOW_TABLE( MMICL_CHILD_SELECT_WIN_TAB ) = 
{
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleChildCallLogSelectWindow),    
    WIN_ID( MMICL_CHILD_SELECT_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICL_CHILD_SELECT_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to handle the contact call log list window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailCallLogListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : to handle the contact call log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

// the window for call log icon main menu
WINDOW_TABLE( MMICL_PBDETAIL_CALLLOG_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePBDetailCallLogListWindow ), 
    WIN_ID( MMICL_PB_DETAIL_CALLLOG_WIN_ID ),  
//    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_PB_DETAIIL_CALLLOG_LIST_CTRL_ID),   
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_ALL_LOG_PB_DETAIL_LIST_CTRL_ID),   

    END_WIN
};

// the window for call log icon main menu
WINDOW_TABLE( MMICL_PBDETAIL_SELECT_WIN_TAB ) = 
{

    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePBDetailSelectWindow ), 
    WIN_ID( MMICL_PB_DETAIL_SELECT_WIN_ID ),  
    WIN_TITLE(TXT_DELETE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS, MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID, MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMICL_PB_DETAIL_SELECT_TITLE_LABEL_CTRL_ID, MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_CHECKBOX_CTRL(TRUE, MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID, MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_CHECKLIST_E,MMICL_PB_DETAIL_SELECT_LIST_CTRL_ID,MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID),

    END_WIN
};


#endif



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : if enter into call log main menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL void OpenCl_PDAMain(BOOLEAN is_permit,DPARAM  param_ptr)
{
    if (is_permit)
    {
        MMIPB_SetCLUnProtect(TRUE);
        MMIAPIPB_OpenMainWin(
                    MMIPB_CALLLOG_APPLICATION,//
                    MMIPB_LIST_NORMAL_MAIN, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER,
                    0,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    PNULL
                    );
    }
    else
    {
    }
    return;
}
#endif

/*****************************************************************************/
// 	Description : if enter into call log main menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_OpenMainMenuWindow(void)
{
    s_select_return_info.need_select_return = FALSE;
    s_select_return_info.return_win_id = 0;

#ifdef MMI_PDA_SUPPORT
    if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
    {
        MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, OpenCl_PDAMain, PNULL, 0);
    }
    else
    {
    MMIAPIPB_OpenMainWin(
                MMIPB_CALLLOG_APPLICATION,//
                MMIPB_LIST_NORMAL_MAIN, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_NUMBER,
                0,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                PNULL
                );
    }

#else
    //隐私保护
    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CL, (MMISET_PRIVACY_APP_ENTRY_CALLBACK_PFUNC)MMICL_OpenMainMenuWindowExt);
#endif 
    return TRUE;
}

/*****************************************************************************/
// 	Description : to handle enter missed,received and dialed calls menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenCallsListWindow(
                                         MMICL_CALL_TYPE_E call_type
                                         )
{
    s_select_return_info.need_select_return = FALSE;
    s_select_return_info.return_win_id = 0;
    
    MMICL_HandleEnterCallsMenu(call_type);
}


/*****************************************************************************/
// 	Description : Some local functions
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
//  Cl_WSTRAddChar  add a char in to a exist wchar array (!!注意加入的长度不要超出)
//  Cl_ConvertBCDNumberFromCallInfo //从指定MMICL_CALL_BCDINFO_T中生成MMIPB_BCD_NUMBER_T
//  Cl_GetCurrentBCDNumber          //从当前的MMICL_CALL_BCDINFO_T中生成MMIPB_BCD_NUMBER_T    
//  Cl_GetNumberTypeStrByNumber     //从指定的MMICL_CALL_BCDINFO_T,获取此号码对应的号码类别
//  CheckCurrentNumberInContact     //从当前的MMICL_CALL_BCDINFO_T，获取当前号码并判断是否在联系人列表中
//  OpenNumberInContact             //从当前的MMICL_CALL_BCDINFO_T，打开联系人详情
/*****************************************************************************/

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 指定MMICL_CALL_BCDINFO_T中生成MMIPB_BCD_NUMBER_T
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_ConvertBCDNumberFromCallInfo(MMIPB_BCD_NUMBER_T* pb_num_ptr,MMICL_CALL_BCDINFO_T* callinfo)
{
    if (pb_num_ptr == PNULL ||callinfo == PNULL)
    {
        return FALSE;
    }
    
    SCI_MEMSET(pb_num_ptr,0x00, sizeof(MMIPB_BCD_NUMBER_T));
    
    pb_num_ptr->number_len = MIN(callinfo->number_len,MMIPB_BCD_NUMBER_MAX_LEN);
    pb_num_ptr->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(callinfo->number_type,
		callinfo->number_plan);
    MMI_MEMCPY(pb_num_ptr->number,MMIPB_BCD_NUMBER_MAX_LEN,callinfo->number,MMIPB_BCD_NUMBER_MAX_LEN,pb_num_ptr->number_len);

    return TRUE;
}

/*****************************************************************************/
// 	Description : 从当前的MMICL_CALL_BCDINFO_T中生成MMIPB_BCD_NUMBER_T  
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetCurrentBCDNumber(MMIPB_BCD_NUMBER_T* pb_num_ptr)
{
    uint16 array_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
	if (pb_num_ptr ==PNULL)
	{
        return FALSE;	
	}

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_1835_112_2_18_2_11_31_52,(uint8*)"");
        return FALSE;
    }

    array_index = CL_GetArrayIndexofCurSelected();

    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);

    Cl_ConvertBCDNumberFromCallInfo(pb_num_ptr,&(arraycall_info->call_bcdinfo[array_index]));

    SCI_FREE(arraycall_info);

    return TRUE;

}
/*****************************************************************************/
// 	Description : 从指定的MMICL_CALL_BCDINFO_T,获取此号码对应的号码类别
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN Cl_GetNumberTypeStrByNumber(wchar* name_str,MMICL_CALL_BCDINFO_T* callinfo)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 tele_num_len = 0;
    wchar type_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar num_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
//    wchar out_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_STRING_T    type_string_id={0};
    MMI_STRING_T    num_string_id={0};
    uint16 typestr_len = 0;

    type_string_id.wstr_ptr =type_wstr;
    num_string_id.wstr_ptr =num_wstr;

    SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));
    //get number
    tele_num_len = MMICL_GenDispNumber(callinfo->call_type,
        callinfo->number_type,
        callinfo->number_len,
        callinfo->number, 
        tele_num,
        PHONE_NUM_MAX_LEN + 2);

    Cl_ConvertBCDNumberFromCallInfo(&pb_num,callinfo);
    
    if (MMIAPIPB_GetNumberTypeByNumber(&pb_num, type_wstr, GUILIST_STRING_MAX_NUM))
    {
        typestr_len = MMIAPICOM_Wstrlen(type_wstr);
        MMI_WSTRNCPY(name_str,GUILIST_STRING_MAX_NUM,(const wchar *)type_wstr,typestr_len,typestr_len);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 从当前的MMICL_CALL_BCDINFO_T，获取当前号码并判断是否在联系人列表中
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckCurrentNumberInContact(void)
{

    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar type_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    Cl_GetCurrentBCDNumber(&pb_num);

    return MMIAPIPB_GetNumberTypeByNumber(&pb_num, type_wstr, GUILIST_STRING_MAX_NUM);
}
/*****************************************************************************/
// 	Description : 从当前的MMICL_CALL_BCDINFO_T，打开联系人详情
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void  OpenNumberInContact(void)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};

    Cl_GetCurrentBCDNumber(&pb_num);

    MMIAPIPB_OpenNumberContactWin(&pb_num);

	return;
}
#endif


/*****************************************************************************/
// 	Description : 设置当前需要更新的LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SetUpdatedWinHandle(MMI_HANDLE_T  updated_win_handle,BOOLEAN isNeedUpdate)
{
    if (isNeedUpdate)
    {
        g_cl_updated_Data.updated_win_handle = updated_win_handle;
        g_cl_updated_Data.is_cl_need_update = FALSE;            
    }
    else
    {
        g_cl_updated_Data.updated_win_handle = 0;
        g_cl_updated_Data.is_cl_need_update = FALSE;        
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : 获得当前需要更新的LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T MMICL_GetUpdatedWinHandle()
{
    return g_cl_updated_Data.updated_win_handle;
}


#if 0
/*****************************************************************************/
// 	Description : 获得当前需要更新的LOG WINHANDLE
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMICL_GetIsNeedUpdated(void)
{
    return g_cl_updated_Data.is_cl_need_update;
}
#endif

/*****************************************************************************/
// 	Description : 获取当前选择的Calllog在arrayInfo中的索引值
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL uint16 CL_GetArrayIndexofCurSelected(void)
{
    uint32 array_index = 0;
#ifdef MMI_PDA_SUPPORT    
    uint16 sel_item_index= 0;

    sel_item_index = GUILIST_GetCurItemIndex(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type));
    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), sel_item_index, &array_index);
#else
    array_index =(uint32) GUILIST_GetCurItemIndex(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type));  
#endif    

    return (uint16)array_index;
}

/*****************************************************************************/
// 	Description : 打开当前选择的Calllog的时间列表窗口
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void CL_OpenCallTimeListOrDetailWin(MMICL_CALL_ARRAY_BCDINFO_T * callinfo)
{
    uint16 array_index =0;
    if (callinfo == PNULL)
    {
        return;
    }
    array_index = CL_GetArrayIndexofCurSelected();
    //U200中，如果从电话本进入的通话记录直接显示通话时间列表
#ifdef CALL_TIMES_SUPPORT    
//    if((callinfo->call_bcdinfo[array_index].call_times>1)||s_cur_sel_log_list_type==MMICL_CALL_ALL)
    if(callinfo->call_bcdinfo[array_index].call_times>0)    
    {
        MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
    }
    else
#endif    
    {
        MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
    }
}
#ifndef MMI_CALLLOG_HIDE_MAINMENU
#if defined (TOUCH_PANEL_SUPPORT) || defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaCallLogItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[40 + 2] = {0};
    wchar temp_len = 0;

    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        MMI_WSTRNCPY(temp_wstr,40+2,(const wchar*)text_str.wstr_ptr,text_str.wstr_len,text_str.wstr_len);
        temp_len = text_str.wstr_len;
    }

#ifdef MMI_GUI_STYLE_MINISCREEN
    item_t.item_style    = GUIITEM_STYLE_CUSTOMIZE;
#else
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT;
#endif

    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = image_id;

    //text 1
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_1;

    //text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }
        
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaCallLogListCtrl(void)
{
    //MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    //uint8 i = 0;
    //BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMICL_MAIN_MENU_CTRL_ID;
    MMI_STRING_T  text_str = {0};
    //uint8 curr_index = 0;
    //MMI_TEXT_ID_T  text_id = TXT_NULL;
    MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info = PNULL;
    uint16 call_num = 0;
    uint8 num_string[3+ 1] = {0};
    uint16 num_string_len = 0;
    wchar num_wstring[3 + 1]= {0};
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    MMI_TEXT_ID_T List_Number[] = {IMAGE_NUMBER_1,
                                   IMAGE_NUMBER_2,
                                   IMAGE_NUMBER_3,
                                   IMAGE_NUMBER_4,
                                   IMAGE_NUMBER_5,
                                   IMAGE_NUMBER_6,
                                   IMAGE_NUMBER_7
    };
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    uint16  i = 0;
#endif        
    GUILIST_SetMaxItem(ctrl_id, 20, FALSE );   
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetAllRecordsNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2076_112_2_18_2_11_31_53,(uint8*)"");
        return;
    }
   
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);

    call_num = GetCurrTypeNum( MMICL_CALL_MISSED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN) 
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_MISSED_CALLS,text_str, List_Number[MISSED_NUMBER1],ID_CL_MISSED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_MISSED_CALLS,text_str, IMAGE_SECMENU_ICON_CL_MISS,ID_CL_MISSED_CALLS);
#endif
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));
    
    call_num = GetCurrTypeNum( MMICL_CALL_DIALED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_DIALED_CALLS,text_str,List_Number[DAILED_NUMBER2], ID_CL_DIALED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_DIALED_CALLS,text_str,IMAGE_SECMENU_ICON_CL_DIAL, ID_CL_DIALED_CALLS);
#endif
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));

    call_num = GetCurrTypeNum( MMICL_CALL_RECEIVED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_RECEIVED_CALLS,text_str,List_Number[RECEIVED_NUMBER3], ID_CL_RECEIVED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_RECEIVED_CALLS,text_str,IMAGE_SECMENU_ICON_CL_RECEIVE, ID_CL_RECEIVED_CALLS);
#endif
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));
    
#if defined(MMI_BLACKLIST_SUPPORT)
    call_num = GetCurrTypeNum( MMICL_CALL_REFUSED,arraycall_info);    
    num_string_len = sprintf((char *)num_string, "%d", call_num);
    MMI_STRNTOWSTR(num_wstring, 3, (const uint8 *)num_string, 3, num_string_len);    
    text_str.wstr_ptr = num_wstring;
    text_str.wstr_len = MMIAPICOM_Wstrlen(num_wstring);
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_REFUSED_CALLS,text_str,List_Number[REJECTED_NUMBER4],ID_CL_REFUSED_CALLS);
    SetItemStyle(ctrl_id, i++);
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_REFUSED_CALLS,text_str,IMAGE_SECMENU_ICON_CL_REJECT,ID_CL_REFUSED_CALLS);
#endif    
    SCI_MEMSET(num_string,0,sizeof(num_string));
    SCI_MEMSET(num_wstring,0,sizeof(num_wstring));
#endif

#ifdef ASP_SUPPORT
    SCI_MEMSET(&text_str,0x00,sizeof(MMI_STRING_T));
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CC_ANSWER_RECORD,text_str,IMAGE_SECMENU_ICON_CALL_RECORD,ID_CL_ASP);
#endif
#if defined (MMI_CL_MINI_SUPPORT) || defined (MMI_GUI_STYLE_MINISCREEN)
    AppendPdaCallLogItem2Line(ctrl_id,TXT_DELALL,text_str,List_Number[DELETE_NUMBER5],ID_CL_DELETE_ALL_RECORDS);
    SetItemStyle(ctrl_id, i++);
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_CALL_TIMERS,text_str,List_Number[TIMER_NUMBER6],ID_CL_CALL_TIMERS);
    SetItemStyle(ctrl_id, i++);
#ifdef MMI_GPRS_SUPPORT
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_GPRS_FLOWRATE,text_str,List_Number[GPRS_NUMBER7],ID_CL_GPRS_FLOWRATE);
    SetItemStyle(ctrl_id, i++);
#endif
#else
    AppendPdaCallLogItem2Line(ctrl_id,TXT_DELALL,text_str,IMAGE_SECMENU_ICON_CL_DELALL,ID_CL_DELETE_ALL_RECORDS);
#ifndef CMCC_UI_STYLE
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_CALL_TIMERS,text_str,IMAGE_SECMENU_ICON_CL_TIME,ID_CL_CALL_TIMERS);
    AppendPdaCallLogItem2Line(ctrl_id,TXT_CL_GPRS_FLOWRATE,text_str,IMAGE_SECMENU_ICON_CL_GPRS,ID_CL_GPRS_FLOWRATE);
#endif
#endif

    SCI_FREE(arraycall_info);

}

/*****************************************************************************/
// 	Description : to handle the call log main menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCLMainMenuWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    uint16 index = 0;
    uint32 user_data = 0;
    MMI_CTRL_ID_T ctrl_id = MMICL_MAIN_MENU_CTRL_ID;
    //GUILIST_POS_INFO_T pos_info = {0};
//add by bingjie.chen
    static uint16 index_pos = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_GUI_STYLE_MINISCREEN
        if (!s_itemstyle_ptr)
        {
            s_itemstyle_ptr = (THEMELIST_ITEM_STYLE_T*)SCI_ALLOC_APPZ(sizeof(*s_itemstyle_ptr));
        }
#endif
        InitPdaCallLogListCtrl();
#ifdef MMI_CL_MINI_SUPPORT
        ExitSelect();
#endif
        MMK_SetAtvCtrl(win_id, MMICL_MAIN_MENU_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:           
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &user_data);

            switch (user_data)
            {
            case ID_CL_DIALED_CALLS:
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
                break;
                
            case ID_CL_MISSED_CALLS:
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
                break;
                
            case ID_CL_RECEIVED_CALLS:
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                MMIAPICL_OpenCallsListWindow(MMICL_CALL_RECEIVED);
                break;

#if defined(MMI_BLACKLIST_SUPPORT)
            case ID_CL_REFUSED_CALLS:
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                MMIAPICL_OpenCallsListWindow(MMICL_CALL_REFUSED);
                break;
#endif

    #ifdef ASP_SUPPORT            
            case ID_CL_ASP:
            {
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                MMIAPIASP_EnterAsp();
                break;
            }
    #endif  
    
            case ID_CL_DELETE_ALL_RECORDS:
                //get all records number
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                if (0 == MMICL_GetAllRecordsNum()) //没有记录
                {
                    MMIPUB_OpenAlertWarningWin(TXT_NO_ENTRY);
                }
                else
                {
                    if (MMK_IsOpenWin(MMICL_QUERY_WIN_ID))
                    {
                        MMK_CloseWin(MMICL_QUERY_WIN_ID);
                    }
                    //询问是否删除!! 
                    MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
                break;
                
            case ID_CL_CALL_TIMERS:
                {            
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
                    uint16 sim_card_ok = 0 ;
                    uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);
    
                    index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                    if( 0 == sim_card_ok_num )
                    {
                        MMIAPIPHONE_AlertSimNorOKStatus();
                    }
                    else if ( 1 == sim_card_ok_num )
                    {
                        MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                    }            
                    else
                    {
                        MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                    }
    #else
                    index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
    #endif
                }
                break;
                
            case ID_CL_GPRS_FLOWRATE:

    #ifdef MMI_GPRS_SUPPORT

                MMIAPICL_OpenGprsFlowRateWin();
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
    #endif
                break;
    
            case ID_CL_TIMER_RESET:
                //询问是否清零!!
                index_pos = GUILIST_GetCurItemIndex(ctrl_id);
                MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                break;
                
            default:
                break;
            }
        break;
        
        case MMI_CL_MAINMENU_UPDATE_INDEX:
            index_pos = *(uint32 *)param;
            break;
            
        case MSG_GET_FOCUS:

            //top_index = GUILIST_GetTopItemIndex(ctrl_id);
#ifdef MMI_CL_MINI_SUPPORT
            ExitSelect();
#endif
            InitPdaCallLogListCtrl();
#ifdef MMI_CL_MINI_SUPPORT
            GUILIST_SetCurItemIndex(ctrl_id, index_pos);
#else
            //Set list focused index for 
            if (s_cur_sel_log_list_type != MMICL_CALL_ALL && index_pos>=ID_CL_CALLLOG_MENUS)
            {
                GUILIST_SetTopItemIndex(ctrl_id, GetMenuIDOffsetByCallType(s_cur_sel_log_list_type));
                GUILIST_SetCurItemIndex(ctrl_id, GetMenuIDOffsetByCallType(s_cur_sel_log_list_type));
                s_cur_sel_log_list_type = MMICL_CALL_ALL;
            }
            else
            {
                GUILIST_SetTopItemIndex(ctrl_id, index_pos);
                GUILIST_SetCurItemIndex(ctrl_id, index_pos);
            }
#endif
            break;
    
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_PROMPTWIN_OK:
//fixed by bingjie.chen
            MMIAPICC_ClearMissedCallNum();
            DeleteAllCallLog(MMICL_CALL_ALL);
            MMIPUB_CloseQuerytWin(&query_win_id);
			
            break;
            
    case MSG_PROMPTWIN_CANCEL:
         //关闭询问窗口
         MMIPUB_CloseQuerytWin(&query_win_id);
         break;
#ifdef MMI_GUI_STYLE_MINISCREEN
    case MSG_CLOSE_WINDOW:
        {
            if (s_itemstyle_ptr)
            {
                SCI_FREE(s_itemstyle_ptr);
                s_itemstyle_ptr = PNULL;
            }
            break;
        }
#endif
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#else
/*****************************************************************************/
// 	Description : to handle the call log main menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCLMainMenuWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, MMICL_MAIN_MENU_CTRL_ID);
#ifdef BT_DIALER_SUPPORT
        s_cur_sel_log_list_type = MMICL_CALL_ALL;   //reset flag here
#endif
        break;

    case MSG_FULL_PAINT:
#ifndef BT_DIALER_SUPPORT
        s_cur_sel_log_list_type = MMICL_CALL_ALL;   //reset flag here
#endif
        break;
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT  // menu 键不相应
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        GUIMENU_GetId(MMICL_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_DIALED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
            break;
            
        case ID_CL_MISSED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
            break;
            
        case ID_CL_RECEIVED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_RECEIVED);
            break;
            
#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_REFUSED_CALLS:
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_REFUSED);
            break;
#endif
			
#ifdef ASP_SUPPORT            
        case ID_CL_ASP:
        {
             MMIAPIASP_EnterAsp();
            break;
        }
#endif  

        case ID_CL_DELETE_ALL_RECORDS:
            //get all records number
            if (0 == MMICL_GetAllRecordsNum()) //没有记录
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_ENTRY);
            }
            else
            {
                if (MMK_IsOpenWin(MMICL_QUERY_WIN_ID))
                {
                    MMK_CloseWin(MMICL_QUERY_WIN_ID);
                }
                //询问是否删除!! 
                MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            }
            break;
            
        case ID_CL_CALL_TIMERS:
            {            
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint16 sim_card_ok = 0 ;
                uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);

                if( 0 == sim_card_ok_num )
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if ( 1 == sim_card_ok_num )
                {
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                }            
                else
                {
                    MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                }
#else
                MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
#endif
            }
            break;
            
        case ID_CL_GPRS_FLOWRATE:
#ifdef MMI_GPRS_SUPPORT
            MMIAPICL_OpenGprsFlowRateWin();
#endif
            break;
            
#ifdef AOC_SUPPORT
        case ID_CL_COSTS_TOTAL:
        case ID_CL_COSTS_MAX:
        case ID_CL_COSTS_RATE:
        case ID_CL_COSTS_RESET:
            MMICL_COSTS_HandleMainMenuMsg(menu_id);
            break;
#endif

        case ID_CL_TIMER_RESET:
            //询问是否清零!!
            MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            break;
            
        default:
            break;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        GUIMENU_GetId(MMICL_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_DELETE_ALL_RECORDS:
            //关闭询问窗口
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMIAPICC_ClearMissedCallNum();
//lzk mark
#ifdef BT_DIALER_SUPPORT			
            DeleteAllCallLog(MMICL_CALL_ALL);
#else
            DeleteAllCallLog(s_cur_sel_log_list_type);
#endif
            //success!
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
            break;
            
        case ID_CL_TIMER_RESET:
            //关闭询问窗口
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMIAPICL_SetDefaultCallTimeToNV();
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            break;
            
        default:
            break;
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#endif
/*****************************************************************************/
// 	Description : to handle enter missed,received and dialed calls menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
void MMICL_HandleEnterCallsMenu(
                                MMICL_CALL_TYPE_E call_type
                                )
{
#ifdef MMI_LDN_SUPPORT
    if (!MMICL_IsCallInfoReady())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);

        return;
    }
#endif

    s_cur_sel_log_list_type = call_type;
    
    //隐私保护
    if (MMIAPISET_IsOpenPrivacyProtect( MMISET_PROTECT_CL ) && !MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID) && !MMIAPISET_IsValidatePrivacyWinOpen(MMISET_PROTECT_CL))
    {
#if defined(BT_DIALER_SUPPORT)
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CL, MMIAPICL_OpenBTCLWindow);
#else
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CL, MMIAPICL_OpenCLWindow);
#endif
    }
    else
    {
#if defined(BT_DIALER_SUPPORT)
        MMIAPICL_OpenBTCLWindow();
#else
        MMIAPICL_OpenCLWindow();
#endif
    } 
}
#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
// 	Description : MMICL_CreateAllLogChildWin
//	Global resource dependence : 
//  Author:rong.gu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreateAllLogChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMICL_ALL_CHILD_WIN_TAB, (ADD_DATA)parent_win_id);
    SetAllLogFillter(MMICL_CALL_ALL);
}

/*****************************************************************************/
// 	Description : create calllog wintab for sms/mms to select receiver
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreateAllLogSelectChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMICL_ALL_SELECT_CHILD_WIN_TAB, (ADD_DATA)parent_win_id);
    SetAllLogFillter(MMICL_CALL_ALL);
}

/*****************************************************************************/
// 	Description : MMICL_CreatePBDetailLogChildWin
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreatePBDetailLogChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMICL_PBDETAIL_CALLLOG_WIN_TAB, (ADD_DATA)parent_win_id);
    SetAllLogFillter(MMICL_CALL_PB_NUMBER_MATCHED);
}


#endif
/*****************************************************************************/
// 	Description : MMIAPICL_OpenCLWindow
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenCLWindow(void)
{
    MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);//@samboo cr102623 20071207
    //close the window of call log list menu and detail win
    MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
    MMK_CloseWin(MMICL_QUERY_WIN_ID);
#ifdef CALL_TIMES_SUPPORT
    MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif

	MMK_CreateParentWin((uint32 *)MMICL_LOG_LIST_WIN_TAB, PNULL);

#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_DIALED_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_RECEIVED_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_MISSED_CHILD_WIN_TAB, NULL);
#else
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_DIALED_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_RECEIVED_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_MISSED_CHILD_WIN_TAB, NULL);
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_REFUSED_CHILD_WIN_TAB, NULL);
#endif

    switch (s_cur_sel_log_list_type)
    {
#if defined(BT_DIALER_SUPPORT)
    case MMICL_BT_CALL_DIALED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_BT_DIALED_CHILD_WIN_ID);
        break;
        
    case MMICL_BT_CALL_RECEIVED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_BT_RECEIVED_CHILD_WIN_ID);
        break;
        
    case MMICL_BT_CALL_MISSED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_BT_MISSED_CHILD_WIN_ID);
        break;
#endif
    case MMICL_CALL_DIALED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_DIALED_CHILD_WIN_ID);
        break;
        
    case MMICL_CALL_RECEIVED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_RECEIVED_CHILD_WIN_ID);
        break;
        
    case MMICL_CALL_MISSED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_MISSED_CHILD_WIN_ID);
        break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_REFUSED_CHILD_WIN_ID);
        break;		
#endif

    default:
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_DIALED_CHILD_WIN_ID);
        break;
    }
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : MMIAPICL_OpenBTCLWindow
//	Global resource dependence : 
//  Author:Cheney.Wu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenBTCLWindow(void)
{
    MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);//@samboo cr102623 20071207
    //close the window of call log list menu and detail win
    MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
    MMK_CloseWin(MMICL_QUERY_WIN_ID);
#ifdef CALL_TIMES_SUPPORT
    MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
	
	
	MMK_CreateParentWin((uint32 *)MMICL_LOG_LIST_WIN_TAB, PNULL);	

    switch (s_cur_sel_log_list_type)
    {
    case MMICL_CALL_DIALED:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_DIALED_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_DIALED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_DIALED_CHILD_WIN_ID);
        break;

    case MMICL_CALL_RECEIVED:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_RECEIVED_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_RECEIVED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_RECEIVED_CHILD_WIN_ID);
        break;

    case MMICL_CALL_MISSED:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_MISSED_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_MISSED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_MISSED_CHILD_WIN_ID);
        break;

#ifndef MMI_MULTI_SIM_SYS_NONE
    case MMICL_BT_CALL_DIALED:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_DIALED_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_DIALED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_BT_DIALED_CHILD_WIN_ID);
        break;

    case MMICL_BT_CALL_RECEIVED:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_RECEIVED_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_RECEIVED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_BT_RECEIVED_CHILD_WIN_ID);
        break;

    case MMICL_BT_CALL_MISSED:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_MISSED_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_MISSED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_BT_MISSED_CHILD_WIN_ID);
        break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_REFUSED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_REFUSED_CHILD_WIN_ID);
        break;		
#endif

    default:
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_DIALED_CHILD_WIN_TAB, NULL);
        MMK_CreateChildWin(MMICL_LOG_LIST_WIN_ID, (uint32*)MMICL_BT_DIALED_CHILD_WIN_TAB, NULL);
        MMK_SetChildWinFocus(MMICL_LOG_LIST_WIN_ID, MMICL_DIALED_CHILD_WIN_ID);
        break;
    }
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: append spliter
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendSplitListItem(	 MMI_CTRL_ID_T    ctrl_id, wchar first_letter)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;
    item_t.first_letter = first_letter;
    item_t.item_data_ptr = &item_data;
    item_t.item_state |=GUIITEM_STATE_SPLIT;
    item_t.user_data = 0xffff;

    GUILIST_SetSplitOnside(ctrl_id, TRUE);

    GUILIST_AppendItem( ctrl_id, &item_t );

}
#endif

/*****************************************************************************/
// 	Description : to set call log list item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListItem(void)
{
    uint16 sel_item_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_HANDLE_T    list_handle = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2626_112_2_18_2_11_32_54,(uint8*)"");
        return;
    }
    
    //get the current selected item index
    //当电话记录列表已经打开时，重新刷新列表需要重新定位当前被选中的item!
    
    list_handle = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    sel_item_index = GUILIST_GetCurItemIndex(list_handle);
    //read logs of calls from NV
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    
    //load log list
    GUILIST_SetMaxItem(list_handle, MMICL_RECORD_MAX_NUM, FALSE );//max item 20

#ifdef MMI_PDA_SUPPORT
    CalllogPBData_InitMatchedItem();
    AppendLogListItem(list_handle, s_cur_sel_log_list_type, arraycall_info);
#if defined(MMI_ISTYLE_SUPPORT)	
    if (MMITHEME_IsIstyle())
    {
         GUILIST_SetTextFont(list_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
    }
#endif	
#else
    AppendLogListItem(list_handle, s_cur_sel_log_list_type, arraycall_info);
#endif
    GUILIST_SetCurItemIndex(list_handle, sel_item_index);
    
    SCI_FREE(arraycall_info);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to set call log select list item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void SetLogSelectListItem(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_HANDLE_T    list_handle = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogSelectListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2667_112_2_18_2_11_32_55,(uint8*)"");
        return;
    }
    list_handle = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    GUILIST_SetMaxItem(list_handle, MMICL_RECORD_MAX_NUM, FALSE );//max item 20
    //read logs of calls from NV
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    AppendLogListItemForMessage(list_handle, s_cur_sel_log_list_type, arraycall_info);
    
    SCI_FREE(arraycall_info);
}
#endif

/*****************************************************************************/
// 	Description : to set call log list item
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_SetLogListItem(void)
{
    SetLogListItem();
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint16 GetCurrTypeNum(MMICL_CALL_TYPE_E      call_type, MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info)
{
    uint16 i = 0;
    uint16 count = 0;

    if(arraycall_info != PNULL)
    {
        if(MMICL_CALL_ALL == call_type)
        {
            count = arraycall_info->record_num;
        }    
        else
        {
            for ( i = 0; i < arraycall_info->record_num; i++ )
            {
                if(call_type == arraycall_info->call_bcdinfo[i].call_type)
                {
                    count++;
                }
            }
        }
    }
    return count;    
}
#ifdef MMI_PDA_SUPPORT

#if 0
/*****************************************************************************/
// 	Description : to set call log list item for istyle,missed call
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void IStyle_MissCallSetLogListItem(void)
{
    uint16 sel_item_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_CTRL_ID_T list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;

	if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle()) 
	{
		return;
	}
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2739_112_2_18_2_11_33_56,(uint8*)"");
        return;
    }
        
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_MISSED, arraycall_info);
    
    //load log list
    GUILIST_SetMaxItem(list_ctrl_id, MMICL_RECORD_MAX_NUM, FALSE );//max item 20
    GUILIST_RemoveAllItems(list_ctrl_id);
    AppendLogListItem(list_ctrl_id, MMICL_CALL_MISSED, arraycall_info);

    GUILIST_SetCurItemIndex(list_ctrl_id, sel_item_index);
    
    SCI_FREE(arraycall_info);

}

/*****************************************************************************/
// 	Description : load call log list into listbox for istyle
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_AppendLogListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             )
{
    uint8 i = 0;
    GUILIST_ITEM_T item_t                          = {0};
    GUILIST_ITEM_DATA_T item_data                       = {0};

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[MMIPB_NAME_MAX_LEN + 1]  ={0};     
    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};
    MMISET_SIM_NAME_T   sim_name = {0};

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    BOOLEAN     isUseNumberType = FALSE;

    MMICL_CALL_BCDINFO_T *callinfo=PNULL;

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON;				

    item_t.item_data_ptr = &item_data;

    //set list item
    if ( arraycall_info->record_num == 0
        #ifdef MMI_PDA_SUPPORT
        || (call_type != MMICL_CALL_ALL && 0 == GetCurrTypeNum( call_type,arraycall_info) )
        #endif
        )  
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_BLANK;
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
    else
    {
        for ( i = 0; i < arraycall_info->record_num; i++ )
        {
            callinfo = &arraycall_info->call_bcdinfo[i];

            if( !IsAddCurrentLogToList(call_type,callinfo))
            {
                continue;
            }
        //Append item
            //Init Text Contents     Name/Number, SimName, Date, Time
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
            sim_name_str.wstr_ptr = sim_name_wstr;
            sim_name_str.wstr_len = 0;
#endif
            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_str));            
            datetime_str.wstr_ptr = datetime_wstr;
            SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));            
            time_str.wstr_ptr = wstr_time_str;

            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;


#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            GetSimNameContent(sim_name_wstr,callinfo);
            sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
            
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
            
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#else
            //Single don't need sim name
#endif

            //Add Contact name/or call number content
            if (GetNameOrNumberContent(name_number_wchar,callinfo,TRUE,FALSE))
            {	
    			name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            }  

            //Add date and time content
            if (0<callinfo->call_start_time[callinfo->call_times -1])
            {
                GetDateContent(&datetime_str, callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;

                GetTimeContent(&time_str,callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
            }
            if (CalllogPBData_IsSelecting())
                {
                   if (CalllogPBData_GetIsSelectedByIndex(i))
                   {
                    item_t.item_state  |= GUIITEM_STATE_SELECTED;
                   }
                   else
                   {
                   item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                   }
                   item_t.item_state &= ~GUIITEM_STATE_CONTENT_CHECK;

                }
                else
                {
                    item_t.item_state &= ~GUIITEM_STATE_SELECTED;
        		    item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
        			item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data.image_id = IMAGE_ISTYLE_MOREINFO_ICON;
                }
            item_t.user_data = (uint32)i;

            GUILIST_AppendItem( ctrl_id, &item_t );
        }
    }
}
#endif
#endif

/*****************************************************************************/
// 	Description : 将通话次数显示加到WSTRING的末尾
//	Global resource dependence : 
//  Author:Sam.hua
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_AddCallTimesStr(MMI_STRING_T *in_str_id,uint16 call_times,MMI_STRING_T* out_strid,uint16 max_len)
{
    MMI_STRING_T    calltimes_strid={0};
    char calltimes_str[MAX_CALL_TIME_LEN]={0};
    wchar calltime_wstr[MAX_CALL_TIME_LEN+1]={0};
    uint16 calltimestr_len=0;
    
    MMI_STRING_T    input_strid={0};
    wchar input_wstr[GUILIST_STRING_MAX_NUM+1]={0};

    if((in_str_id ==PNULL)||(out_strid ==PNULL))
    {
        return FALSE;
    }
    //Check call_times and len
    if ((in_str_id->wstr_ptr == PNULL)||(call_times>MMICL_MAX_CALL_TIMES)
        ||(in_str_id->wstr_len+MAX_CALL_TIME_LEN>max_len))
    {
        return FALSE;
    }
    
    sprintf((char *)calltimes_str, "(%d)", call_times);
    calltimestr_len = strlen((char *)calltimes_str);
    MMI_STRNTOWSTR(calltime_wstr, MAX_CALL_TIME_LEN, (uint8 *)calltimes_str, MAX_CALL_TIME_LEN, MAX_CALL_TIME_LEN);
    calltimes_strid.wstr_ptr = calltime_wstr;
    calltimes_strid.wstr_len = MMIAPICOM_Wstrlen(calltime_wstr);

    SCI_MEMSET(input_wstr,0,sizeof(input_wstr));
    MMI_WSTRNCPY(input_wstr,max_len,(const wchar*)in_str_id->wstr_ptr,in_str_id->wstr_len,in_str_id->wstr_len);
    input_strid.wstr_ptr = input_wstr;
    input_strid.wstr_len = MMIAPICOM_Wstrlen(input_wstr);
    
    MMIAPICOM_MergeTwoString(&input_strid,&calltimes_strid,out_strid,max_len);
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : 获取通话记录的联系人姓名或者号码显示
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: //isAddCalltimes   需要加入 Calltimes
          //isUseNumberType  需要转换为号码类别
/*****************************************************************************/
LOCAL BOOLEAN GetNameOrNumberContent(wchar *in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,
                                    BOOLEAN isAddCalltimes,BOOLEAN isUseNumberType)
{
    MMI_STRING_T namestrid = {0}; 
    wchar wchar_namestr[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+ 1] = {0}; 
    MMI_STRING_T temp_strid={0};

    if ((in_str == PNULL)||(callinfo==PNULL))
    {
        return FALSE;
    }

    temp_strid.wstr_ptr = in_str;
    
    SCI_MEMSET(&namestrid,0,sizeof(MMI_STRING_T));
    SCI_MEMSET(wchar_namestr,0,sizeof(wchar_namestr));
    namestrid.wstr_ptr = wchar_namestr;
    
    if ((!callinfo->is_num_present) ||(callinfo->is_emergency_call))
    {
        MMI_STRING_T res_strid = {0}; 
        if (callinfo->is_emergency_call)
        {
            MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &res_strid);
        }    
        else
        {
            MMI_GetLabelTextByLang(TXT_UNKNOW_NUM, &res_strid);
        }               
        res_strid.wstr_len = MIN(GUILIST_STRING_MAX_NUM, res_strid.wstr_len);
#ifdef CALL_TIMES_SUPPORT        
        if ((callinfo->call_times > 1) &&(isAddCalltimes))
        {
            MMICL_AddCallTimesStr(&res_strid,callinfo->call_times,&temp_strid,GUILIST_STRING_MAX_NUM);
        }
        else
#endif        
        {
            MMI_WSTRNCPY(in_str, GUILIST_STRING_MAX_NUM, res_strid.wstr_ptr, res_strid.wstr_len, res_strid.wstr_len);
        }
    }
    else
    {
        BOOLEAN find_result=FALSE;
        uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
        uint16  tele_num_len = 0;
#ifdef CALL_TIMES_SUPPORT
        uint16 temp_str_len = 0;
#endif        
#if defined(MMI_PDA_SUPPORT) ||defined(CALL_TIMES_SUPPORT)
        uint8   temp_str[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 ] = {0};
#endif
#ifdef MMI_PDA_SUPPORT   
        wchar   num_wstr[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2] ={0};
        MMI_STRING_T num_strid = {0};
#endif              
        
        SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));
    
        //get number
        tele_num_len = MMICL_GenDispNumber(callinfo->call_type,
            callinfo->number_type,
            callinfo->number_len,
            callinfo->number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);

        find_result = MMIAPICL_GetNameByClInfo(*callinfo, &namestrid, FALSE);

        //no name in pb
        if ( !find_result || 0 == namestrid.wstr_len )
        {
#ifdef CALL_TIMES_SUPPORT
            if((callinfo->call_times > 1)&&(isAddCalltimes))
            {
                sprintf((char *)temp_str, "%s(%d)", tele_num, callinfo->call_times);
                temp_str_len = strlen((char *)temp_str) ;
                MMI_STRNTOWSTR(in_str,max_len,temp_str,temp_str_len,temp_str_len);
            }
            else
#endif
            {
                MMI_STRNTOWSTR(in_str,max_len,tele_num,tele_num_len,tele_num_len);
            }
        }
        else//there is name
        {
#ifdef MMI_PDA_SUPPORT   
             //  Use type+number replaced Contact Name
            if (isUseNumberType)
            {
                MMI_STRING_T    type_strid = {0};
                wchar   type_wstr[GUILIST_STRING_MAX_NUM+1]={0};


                SCI_MEMSET(type_wstr,0,sizeof(type_wstr));
                type_strid.wstr_ptr = type_wstr;
                type_strid.wstr_len = 0;
                if (Cl_GetNumberTypeStrByNumber(type_wstr,callinfo))
                {
                    //Clear Name string
                    SCI_MEMSET(wchar_namestr,0,sizeof(wchar_namestr));
                    namestrid.wstr_len = 0;
                    SCI_MEMSET(num_wstr,0,sizeof(num_wstr));
                    num_strid.wstr_ptr = num_wstr;

                    //generate ' : '+number string id
                    sprintf((char *)temp_str, " : %s", tele_num);
                    MMI_STRNTOWSTR(num_wstr, PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 , 
                                    (uint8 *)temp_str, PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 , 
                                    PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+2 );
                    type_strid.wstr_len = MMIAPICOM_Wstrlen(type_wstr);
                    num_strid.wstr_len = MMIAPICOM_Wstrlen(num_wstr);
                    //merge it 
                    MMIAPICOM_MergeTwoString(&type_strid,&num_strid,&namestrid,max_len);
                }
            }
#endif                
#ifdef CALL_TIMES_SUPPORT
            if((callinfo->call_times > 1) &&(isAddCalltimes))
            {
                MMICL_AddCallTimesStr(&namestrid,callinfo->call_times,&temp_strid,max_len);
            }
            else
#endif            
            {
                namestrid.wstr_len= MMIAPICOM_Wstrlen(namestrid.wstr_ptr);
                MMI_WSTRNCPY(in_str,max_len,(const wchar*)namestrid.wstr_ptr,
                            namestrid.wstr_len,namestrid.wstr_len);
            }
        }
    }

    return TRUE;

}

/*****************************************************************************/
// 	Description : 获取Call type 的 ICON ID
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetCallTypeImageID(MMICL_CALL_BCDINFO_T *callinfo)
{
    uint32 offset = 0;
    if (callinfo ==PNULL)
        return PNULL;
        
    if (callinfo->is_emergency_call)
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
        {
            return IMAGE_ISTYLE_CL_LIST_SOS_ICON;
        }
        else
#endif        
        {
            return IMAGE_CL_SOS_CALL_ICON;
        }

     }

#ifdef VT_SUPPORT
    if (CC_CALL_VIDEO==callinfo->isvideocall)
    {
        offset = MMICL_IMAGE_INC;
    }
#endif

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
    {
        switch ( callinfo->call_type)
        {
            case MMICL_CALL_MISSED:
               return  (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_MISSED_LIST_ICON);
            case MMICL_CALL_RECEIVED:
               return  (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_RECEIVED_LIST_ICON);
            case MMICL_CALL_DIALED:
               return (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_DIALED_LIST_ICON);
#if defined(MMI_BLACKLIST_SUPPORT)
            case MMICL_CALL_REFUSED:
               return (MMI_IMAGE_ID_T)(ISTYLE_IMAGE_CL_REFUSEDLIST_ICON);
#endif
            default:
               //SCI_TRACE_LOW:"AppendLogListItem:the call_type %d is error!"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3123_112_2_18_2_11_33_57,(uint8*)"d",callinfo->call_type);
               break;
        }
       return PNULL;

    }
    
#endif
   
    switch ( callinfo->call_type)
    {
        case MMICL_CALL_MISSED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_MISSED_CALL_ICON+offset);
        case MMICL_CALL_RECEIVED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_RECEIVED_CALL_ICON+offset);
        case MMICL_CALL_DIALED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_DIALED_CALL_ICON+offset);
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_REFUSED_CALL_ICON+offset);
#endif
#if defined(BT_PBAP_SUPPORT)
        case MMICL_BT_CALL_MISSED:
            return IMAGE_CL_BT_MISSED_CALL_ICON;
        case MMICL_BT_CALL_RECEIVED:
            return IMAGE_CL_BT_RECEIVED_CALL_ICON;
        case MMICL_BT_CALL_DIALED:
            return IMAGE_CL_BT_DIALED_CALL_ICON;
#endif
        default:
           //SCI_TRACE_LOW:"AppendLogListItem:the call_type %d is error!"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3123_112_2_18_2_11_33_57,(uint8*)"d",callinfo->call_type);
           break;
    }
    return PNULL;
}

/*****************************************************************************/
// 	Description : 获取Call type 的 ICON ID
//	Global resource dependence : 
//  Author:tao.xue
//	Note:  
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPICL_GetCallTypeImageID(BOOLEAN is_emergency_call, BOOLEAN is_video_call, MMICL_CALL_TYPE_E call_type)
{
    uint32 offset = 0;
        
    if (is_emergency_call)
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
        {
            return IMAGE_ISTYLE_CL_LIST_SOS_ICON;
        }
        else
#endif        
        {
            return IMAGE_CL_SOS_CALL_ICON;
        }
    }

#ifdef VT_SUPPORT
    if (CC_CALL_VIDEO==is_video_call)
    {
        offset = MMICL_IMAGE_INC;
    }
#endif
   
    switch ( call_type)
    {
        case MMICL_CALL_MISSED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_MISSED_CALL_ICON+offset);
        case MMICL_CALL_RECEIVED:
           return  (MMI_IMAGE_ID_T)(IMAGE_CL_RECEIVED_CALL_ICON+offset);
        case MMICL_CALL_DIALED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_DIALED_CALL_ICON+offset);
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
           return (MMI_IMAGE_ID_T)(IMAGE_CL_REFUSED_CALL_ICON+offset);
#endif
        default:
           //SCI_TRACE_LOW:"MMIAPICL_GetCallTypeImageID:the call_type %d is error!"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3162_112_2_18_2_11_33_58,(uint8*)"d",call_type);
           break;
    }
    return PNULL;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
/*****************************************************************************/
// 	Description : 获取SIM_NAME WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSimNameContent(wchar* in_str,uint16 max_len,MN_DUAL_SYS_E dual_sys)
{
    MMISET_SIM_NAME_T sim_name={0};
    if (MN_DUAL_SYS_MAX <= dual_sys)
    {
        return FALSE;
    }

    //Max len should less than defined      .........
    //Add sim name content and set its color if needed
    sim_name = MMIAPISET_GetSimName(dual_sys);

    MMI_WSTRNCPY(in_str,max_len,(const wchar*)sim_name.wstr_arr,sim_name.wstr_len,sim_name.wstr_len);
    return TRUE;
}
#endif

/*****************************************************************************/
// 	Description : 获取日期显示的WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  今天/昨天用资源字串表示，其他的日期用 Mon/Day 表示   不加入年份
/*****************************************************************************/
LOCAL BOOLEAN GetDateContent(MMI_STRING_T* in_str,uint16 max_len,
                MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index,BOOLEAN isNeedYear)
{

    SCI_DATE_T sys_data = {0};
    MMI_TM_T tm = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;
    uint8      date_str[20] = {0};
    MMI_STRING_T    res_str={0};

    if ((callinfo == PNULL) ||(in_str == PNULL)||(call_time_index>=MMICL_MAX_CALL_TIMES))
    {
        return FALSE;
    }
    if (in_str->wstr_ptr == PNULL)
    {
        return FALSE;
    }
#ifdef CALL_TIMES_SUPPORT    
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time[call_time_index]);
#else
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time);
#endif
    TM_GetSysDate(&sys_data);

    cl_sec =MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,(uint32)(tm.tm_year));
    sys_sec =MMIAPICOM_Tm2Second(0,0,0,sys_data.mday,sys_data.mon,sys_data.year);

    if(cl_sec == sys_sec)
    {   
        MMI_GetLabelTextByLang(TXT_TODAY, &res_str);        //today
    }
    if (cl_sec < sys_sec)
    {
        if( (sys_sec - cl_sec) <= 60*60*24)
        {
            MMI_GetLabelTextByLang(TXT_YESTERDAY, &res_str);    //yesterday
        }
    }
    
    if (res_str.wstr_len >0)
    {
        MMI_WSTRNCPY(in_str->wstr_ptr,max_len,(const wchar*)res_str.wstr_ptr,
                        res_str.wstr_len,res_str.wstr_len);
    }
    else
    {
        MMIAPISET_FormatDateStrByDateEx(tm.tm_year, tm.tm_mon, tm.tm_mday,'/', date_str,20,isNeedYear);
        MMI_STRNTOWSTR(in_str->wstr_ptr,max_len,(const uint8*)date_str,(uint32)strlen((char*)date_str),(uint32)strlen((char*)date_str));
    }
    in_str->wstr_len = (uint16)MMIAPICOM_Wstrlen(in_str->wstr_ptr);
    return TRUE;
    

}
/*****************************************************************************/
// 	Description : 获取时间日期显示的WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  今天的用时间，其他的用日期
/*****************************************************************************/
LOCAL BOOLEAN GetTimeAndDateContent(MMI_STRING_T* in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index)
{
    SCI_DATE_T sys_data = {0};
    MMI_TM_T tm = {0};
    uint8      date_str[20] = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;

    if ((callinfo == PNULL) ||(in_str == PNULL)||(call_time_index>=MMICL_MAX_CALL_TIMES))
    {
        return FALSE;
    }
    if (in_str->wstr_ptr == PNULL)
    {
        return FALSE;
    }
#ifdef CALL_TIMES_SUPPORT    
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time[call_time_index]);
#else
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time);
#endif
    TM_GetSysDate(&sys_data);

    cl_sec =MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,(uint32)(tm.tm_year));
    sys_sec =MMIAPICOM_Tm2Second(0,0,0,sys_data.mday,sys_data.mon,sys_data.year);

//fixed by bingjie.chen

    if(cl_sec == sys_sec)       //Today ,return Time_str
    {   
        MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min, date_str,20);

        MMI_STRNTOWSTR(in_str->wstr_ptr,max_len,(const uint8*)date_str,
                    strlen((char*)date_str),strlen((char*)date_str));
    }
    else                        //use Date
    {
        MMIAPISET_FormatDateStrByDateEx(tm.tm_year, tm.tm_mon, tm.tm_mday,'/', date_str,20,FALSE);
        MMI_STRNTOWSTR(in_str->wstr_ptr,max_len,(const uint8*)date_str,(uint32)strlen((char*)date_str),(uint32)strlen((char*)date_str));
    }

    in_str->wstr_len = (uint16)MMIAPICOM_Wstrlen(in_str->wstr_ptr);

    return TRUE;

}


/*****************************************************************************/
// 	Description : 获取时间显示的WSTRING( HOUR:MIN)
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTimeContent(MMI_STRING_T* in_str,uint16 max_len,MMICL_CALL_BCDINFO_T *callinfo,uint16 call_time_index)
{
    MMI_TM_T tm = {0};
    uint8   time_str[10] = {0};

    if ((callinfo == PNULL) ||(in_str == PNULL)||(call_time_index>=MMICL_MAX_CALL_TIMES))
    {
        return FALSE;
    }
    if (in_str->wstr_ptr == PNULL)
    {
        return FALSE;
    }
#ifdef CALL_TIMES_SUPPORT    
    tm = MMIAPICOM_Second2Tm(callinfo->call_start_time[call_time_index]);
#else
    tm = MMIAPICOM_Second2Tm(callinfo->call_start_time);
#endif

    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min, time_str,10);

    MMI_STRNTOWSTR(in_str->wstr_ptr,max_len,(const uint8*)time_str,
                strlen((char*)time_str),strlen((char*)time_str));

    in_str->wstr_len = (uint16)MMIAPICOM_Wstrlen(in_str->wstr_ptr);
    return TRUE;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description :当前的记录是否需要加入到列表中
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 用于PDA下的PB中的通话记录列表
/*****************************************************************************/
LOCAL BOOLEAN IsAddCurrentLogToList(MMICL_CALL_TYPE_E call_type,MMICL_CALL_BCDINFO_T *callinfo)
{
    if (callinfo==PNULL)
    {
        return FALSE;
    }

    if(call_type != MMICL_CALL_ALL &&  callinfo->call_type != call_type)
    {
        return FALSE;
    }
    if (call_type == MMICL_CALL_ALL &&MMICL_CALL_MATCHED!=GetAllLogFillter() &&MMICL_CALL_PB_NUMBER_MATCHED!=GetAllLogFillter())
    {
        if((MMICL_CALL_ALL!=GetAllLogFillter()) &&  (callinfo->call_type != GetAllLogFillter()))
        {
            return FALSE;
        }
    }
    
    if (call_type == MMICL_CALL_ALL && MMICL_CALL_PB_NUMBER_MATCHED==GetAllLogFillter())
    {
        int j=0;
        BOOLEAN is_sameNumber = FALSE;
        MMIPB_BCD_NUMBER_T test={0};
        MMIPB_BCD_NUMBER_T *pb_number = PNULL;

        test.number_len = callinfo->number_len;
        for (j=0;j<test.number_len;j++)
            test.number[j] = callinfo->number[j];
        
        for (j=0;j<MMIPB_MAX_NV_PHONE_NUM;j++)
        {
            pb_number = GetAllLogFillterPBNumber(j);
            if (pb_number == PNULL || pb_number->number_len == 0)
            {
                continue;
            }
            //Compare Number,if Not equal,continue
            if(0 == MMIAPIPB_BcdSmartCmp(&test,pb_number))
            {    
                is_sameNumber = TRUE;
                break;
            }
        }

        if (!is_sameNumber)
        {
            return FALSE;
        }
        CalllogPBData_AddMatchedItemCount();
     } 

    return TRUE;
}
#endif 

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : load call log list into listbox for sms/mms
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItemForMessage(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             )
{
    uint8 i = 0;

    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN +MAX_CALL_TIME_LEN+ 1]  ={0};     
    
    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    BOOLEAN     isUseNumberType = FALSE;

    MMICL_CALL_BCDINFO_T    *callinfo=PNULL;
    
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_CHECK_ICON;

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;

    item_t.item_data_ptr = &item_data;


    //set softkey
    if ( s_select_return_info.need_select_return )
    {
        item_data.softkey_id[0] = TXT_COMMON_OK;
    }
    else
    {
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
    }
    
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
   
    //set list item
    if ( arraycall_info->record_num == 0
        || (call_type != MMICL_CALL_ALL && 0 == GetCurrTypeNum( call_type,arraycall_info) )
        )  
    {
        //return;
    }
    else
    {
        for ( i = 0; i < arraycall_info->record_num; i++ )
        {

            callinfo = &arraycall_info->call_bcdinfo[i];
            if(call_type != MMICL_CALL_ALL &&  callinfo->call_type != call_type)
            {
                continue;
            }
            if (call_type == MMICL_CALL_ALL &&MMICL_CALL_MATCHED!=GetAllLogFillter() &&MMICL_CALL_PB_NUMBER_MATCHED!=GetAllLogFillter())
            {
                if((MMICL_CALL_ALL!=GetAllLogFillter()) &&  (callinfo->call_type != GetAllLogFillter()))
                {
                    continue;
                }
            }


            
            //Init Text Contents     Name/Number, Date
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;

            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
            datetime_str.wstr_ptr = datetime_wstr;

            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[1].item_data.image_id = image_id;


            //Add Contact name/or call number content
            if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,TRUE,isUseNumberType))
            {	
				name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[0].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[0].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            }  
#ifdef CALL_TIMES_SUPPORT
            if(callinfo->call_times>0)
            {
                //Add date and time content
                if (0<callinfo->call_start_time[callinfo->call_times -1])
                {
                    GetTimeAndDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,callinfo->call_times -1);
                    item_data.item_content[2].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[2].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                    item_data.item_content[2].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
                }
            }            
#else
            if (0<callinfo->call_start_time)
            {
                GetTimeAndDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0);
                item_data.item_content[2].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[2].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[2].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
            }
#endif
            item_t.user_data = (uint32)i;
            GUILIST_AppendItem( ctrl_id, &item_t );
            
            if (MMICL_SelectedListIsValid(i))
            {
                GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : MMICC_CreateNewSoftKey
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMICC_CreateNewSoftKey(MMI_HANDLE_T win_handle, MMI_HANDLE_T form_id,
                            MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, 
                            MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    GUIFORM_DYNA_CHILD_T child_data = {0};

    child_data.is_softkey = TRUE;
    child_data.guid=SPRD_GUI_SOFTKEY_ID;
    child_data.child_handle=MMICALLLOG_SOFTKEY_CTRL_ID;
    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;
    child_data.init_data_ptr=&softkey_data;
    GUIFORM_CreatDynaChildCtrl(win_handle,form_id,&child_data);
    return TRUE;
}

#endif

/*****************************************************************************/
// 	Description : load call log list into listbox
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void AppendLogListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info
                             )
{
    uint8 i = 0;

    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN + 1+MAX_CALL_TIME_LEN]  ={0};     

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};

#endif

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    BOOLEAN     isUseNumberType = FALSE;

    MMICL_CALL_BCDINFO_T    *callinfo=PNULL;

#ifdef HOME_LOCATION_INQUIRE_SUPPORT
//    uint16  city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
    uint16  str_length = 0;
    uint8   origin_number_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    int16   origin_num_len  = 0;
    MMI_STRING_T location_str       ={0};
    wchar location_wstr[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
#endif

#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON;				
    }
    else
#endif		
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;				
    }
#else
#ifdef TOUCH_PANEL_SUPPORT            
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;				
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE
//fix by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;    
#else
    item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;
#endif
#else
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;    
#else
    item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT;
#endif
     
#endif
#endif
#endif
    
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;
    
    //set softkey
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;

    if ( s_select_return_info.need_select_return )
    {
        item_data.softkey_id[0] = TXT_COMMON_OK;
    }
    else
    {
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
        if (isMultiSelecting())
        {
            item_data.softkey_id[0] = TXT_COMMON_OK;
			item_data.softkey_id[1] = (MMI_TEXT_ID_T)STXT_MARK;
        }
        else
#endif            
        {
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
        }
    }
    
    //set list item
    if ( arraycall_info->record_num == 0
#ifdef MMI_PDA_SUPPORT
        || (call_type != MMICL_CALL_ALL && 0 == GetCurrTypeNum( call_type,arraycall_info) )
#endif
        )  
    {
#ifndef MMI_PDA_SUPPORT
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_BLANK;
        GUILIST_AppendItem( ctrl_id, &item_t );
#endif
    }
    else
    {
        if (call_type == MMICL_CALL_ALL && MMICL_CALL_PB_NUMBER_MATCHED==GetAllLogFillter())
        {
            isUseNumberType = TRUE;
        }
    
		
        for ( i = 0; i < arraycall_info->record_num; i++ )
        {

            callinfo = &(arraycall_info->call_bcdinfo[i]);
#ifdef MMI_PDA_SUPPORT
            //Check current log should be added to list or not
            if(!IsAddCurrentLogToList(call_type,callinfo))
            {   
                continue;
            }    
            
#endif
            //Init Text Contents     Name/Number, SimName, Date, Time
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
            sim_name_str.wstr_ptr = sim_name_wstr;
            sim_name_str.wstr_len = 0;
#endif
            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
            datetime_str.wstr_ptr = datetime_wstr;
            SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));            
            time_str.wstr_ptr = wstr_time_str;

            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (!callinfo->is_emergency_call)
            {
                MMIAPICL_GetSimNameContent(sim_name_wstr,GUILIST_STRING_MAX_NUM,callinfo->dual_sys);
            }
            sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
            
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
#ifdef MMI_CL_MINI_SUPPORT
#ifdef BT_DIALER_SUPPORT
			if (MMICL_BT_CALL_MISSED == call_type
                || MMICL_BT_CALL_RECEIVED == call_type
                || MMICL_BT_CALL_DIALED == call_type
                )
            {
            	item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	            item_data.item_content[2].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
	            item_data.item_content[2].item_data.text_buffer.wstr_len =0;
			}
			else
#endif
			{
	            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	            item_data.item_content[2].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
	            item_data.item_content[2].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
			}
#else 
#ifdef BT_DIALER_SUPPORT
            if (MMICL_BT_CALL_MISSED == call_type
                || MMICL_BT_CALL_RECEIVED == call_type
                || MMICL_BT_CALL_DIALED == call_type
                )
            {
            	item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
	            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =0;
            }
			else
#endif
			{
	            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
	            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
			}
#endif   
#else
            //Single don't need sim name
#endif

            //Add Contact name/or call number content
            if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,TRUE,isUseNumberType))
            {	
				name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
//fix by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[1].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
#else
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
#endif
            }  

#ifdef TOUCH_PANEL_SUPPORT
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
            SCI_MEMSET(&location_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&location_wstr,0,sizeof(location_wstr));       
            location_str.wstr_ptr = location_wstr;

            if (callinfo->is_num_present)//电话号码不存在
            {
                uint8 origin_number_str[CC_MAX_TELE_NUM_LEN + 2] = {0};
//                uint8 tel_number_str[CC_MAX_TELE_NUM_LEN + 2] = {0};

                MMICL_GenDispNumber(callinfo->call_type,
                                    callinfo->number_type,
                                    callinfo->number_len,
                                    callinfo->number, 
                                    origin_number_str,
                                    CC_MAX_TELE_NUM_LEN + 2);

                if(MMIAPICC_RemoveIpNumber(origin_number_str, strlen((char*)origin_number_str), origin_number_arr,CC_MAX_TELE_NUM_LEN))
                {
                    origin_num_len = strlen((char*)origin_number_arr);
                }
                else
                {
                    SCI_MEMCPY(origin_number_arr,origin_number_str,CC_MAX_TELE_NUM_LEN+2);
                    origin_num_len = strlen((char*)origin_number_arr);
                }
                
                str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_arr, origin_num_len, (uint16*)location_wstr);
            }
            else
            {
                str_length = 0;
            }
            if( str_length == 0 )
            {
                MMI_GetLabelTextByLang(TXT_UNKNOWN, &location_str);
            }   
            else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
            {
                MMI_GetLabelTextByLang(TXT_LOCAL, &location_str);
            }   
            else
            {
                location_str.wstr_ptr = location_wstr;
                //location_str.is_ucs2 = TRUE;
                location_str.wstr_len  = str_length;
            }
            item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = location_str.wstr_len;
            item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = location_str.wstr_ptr;


#endif
#endif
#ifdef CALL_TIMES_SUPPORT            
            if (callinfo->call_times>0)
            {
                //Add date and time content
                if (0<callinfo->call_start_time[callinfo->call_times -1])
                {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/				
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
#ifdef MMI_CL_MINI_SUPPORT
                GetTimeAndDateContent(&time_str, GUILIST_STRING_MAX_NUM, callinfo, callinfo->call_times - 1);
                item_data.item_content[4].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[4].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[4].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr; 
#else          
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr; 
#endif
                }

            }    
#else
                //Add date and time content
            if (0<callinfo->call_start_time)
            {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/				
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
#ifdef MMI_CL_MINI_SUPPORT
                GetTimeAndDateContent(&time_str, GUILIST_STRING_MAX_NUM, callinfo, 0);
                item_data.item_content[4].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[4].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[4].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr; 
#else                
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
            }
#endif
#endif
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
            if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
            {
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].font_color_id=MMITHEME_COLOR_BLUE;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].is_default =TRUE;
            }
#endif

            //Add selected content or icon content
            if (CalllogPBData_IsSelecting())
            {
                if (CalllogPBData_GetIsSelectedByIndex(i))
                {
                    item_t.item_state  |= GUIITEM_STATE_SELECTED;
                }
                else
                {
                    item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                }
                item_t.item_state &= ~GUIITEM_STATE_CONTENT_CHECK;
            }
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
            else if (isMultiSelecting())
            {
                if (isCurrentSelected(i))
                {
                    item_t.item_state  |= GUIITEM_STATE_SELECTED;
                }
                else
                {
                    item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                }
                item_t.item_state &= ~GUIITEM_STATE_CONTENT_CHECK;

            }
#endif
            else
            {
#ifdef TOUCH_PANEL_SUPPORT            
                item_t.item_state &= ~GUIITEM_STATE_SELECTED;
                item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
                if (MMITHEME_IsIstyle())
                {
                    item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data.image_id = IMAGE_ISTYLE_MOREINFO_ICON;
                }
                else
#endif                
                {
                    item_data.item_content[CL_LIST_ITEM_ICON_INDEX].item_data.image_id = IMAGE_CL_DIAL_ICON;
                }
#endif            
            }
#ifdef MMI_CL_MINI_SUPPORT

                if(s_select_info.is_select)
            
                {
                    if(s_select_info.log_selected_index[i])
                    {
                    item_data.item_content[5].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[5].item_data.image_id = IMAGE_MARKED_ICON;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
					item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_CANCEL_ICON;
#else
                    item_data.softkey_id[1] = (MMI_TEXT_ID_T)STXT_CANCEL;
#endif
                    } 
                    else
                    {
                    item_data.item_content[5].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[5].item_data.image_id = IMAGE_NULL;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
					item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_SELECTED_ICON;
#else
                    item_data.softkey_id[1] = (MMI_TEXT_ID_T)STXT_MARK;
#endif
                    }
                }
                else
                {
                    item_data.item_content[5].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[5].item_data.image_id = IMAGE_NULL;
                }
                      
#endif	
            item_t.user_data = (uint32)i;
            GUILIST_AppendItem( ctrl_id, &item_t );

        }
    }
}

/*****************************************************************************/
// 	Description : make a Mo call by record index
//	Global resource dependence : 
//  Author:Kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_MakeCallByRecordIndex(
                                        MMICL_CALL_TYPE_E	call_type,
                                        uint16				record_index
                                        )
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
	 
    #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
    #endif
    MMI_STRING_T*  name_ptr = PNULL;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMI_STRING_T  call_name = {0};
    wchar temp_str[MMICL_NAME_MAX_LEN +1] = {0};
#endif

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"MMIAPICL_MakeCallByRecordIndex call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3814_112_2_18_2_11_35_59,(uint8*)"");
        return;
    }
    
    MMICL_GetRecordInfo(call_type, arraycall_info);
    
    #ifdef MMI_PDA_SUPPORT
    if(call_type != MMICL_CALL_ALL) //list 无数据用此方法
    {  
        uint8 i = 0;
        uint8 count = 0;
        for(i = 0; i<arraycall_info->record_num ; i++)
        {
            if(arraycall_info->call_bcdinfo[i].call_type == call_type)
            {
                count++;
                if((record_index + 1) == count)   //在all中找到位置
                {
                     record_index = i;
                     break;
                }
            }
        }
    }
    #endif

    #ifdef MMI_PDA_SUPPORT
    if ((0 != GetCurrTypeNum( call_type,arraycall_info))&& arraycall_info->call_bcdinfo[record_index].is_num_present)//电话号码存在
    #else
    if ((arraycall_info->record_num != 0)&& arraycall_info->call_bcdinfo[record_index].is_num_present)//电话号码存在
    #endif
    {
        CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_GetCallSimType(arraycall_info->call_bcdinfo[record_index].dual_sys);
	
        //get number
        MMICL_GenDispNumber(arraycall_info->call_bcdinfo[record_index].call_type,
            arraycall_info->call_bcdinfo[record_index].number_type,
            arraycall_info->call_bcdinfo[record_index].number_len,
            arraycall_info->call_bcdinfo[record_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
        
        num_len = (uint8)strlen((char*)tele_num);
        
        //call
        if ( num_len > 0 )
        {
#if defined(CL_SAVE_NAME_SUPPORT)
            if (arraycall_info->call_bcdinfo[record_index].name.is_use_nv_name)
            {
                call_name.wstr_ptr = temp_str;
                call_name.wstr_len = arraycall_info->call_bcdinfo[record_index].name.wstr_len;
                MMI_WSTRNCPY(call_name.wstr_ptr,
                                            MMICL_NAME_MAX_LEN, 
                                            arraycall_info->call_bcdinfo[record_index].name.wstr, 
                                            MMICL_NAME_MAX_LEN, 
                                            call_name.wstr_len);
                name_ptr = &call_name;
            }
            else
            {
                name_ptr = PNULL;
            }
#endif

            #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
            cc_result = MMIAPICC_MakeCall(
                                            arraycall_info->call_bcdinfo[record_index].dual_sys,
                                            tele_num,
                                            num_len,
                                            PNULL,
                                            name_ptr,
                                            sim_type,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );
            if(CC_RESULT_SUCCESS == cc_result)
            {
               MMIAPIBT_SetCallSuccess();
            }
            else
            {
               MMIAPIBT_SetCallFail();
            }
            #else
            MMIAPICC_MakeCall(
                                            arraycall_info->call_bcdinfo[record_index].dual_sys,
                                            tele_num,
                                            num_len,
                                            PNULL,
                                            name_ptr,
                                            sim_type,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );

            #endif
        }
    }
    else
    {
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
        MMIAPIBT_SetCallFail(); //mdy by qing.yu@for cr238786
#endif
        //SCI_TRACE_LOW:"MMIAPICL_MakeCallByRecordIndex: the call number is not exited, arraycall_info->record_num is = %d,is num present = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_3917_112_2_18_2_11_35_60,(uint8*)"dd",arraycall_info->record_num,arraycall_info->call_bcdinfo[record_index].is_num_present);
    }
    
    SCI_FREE(arraycall_info);

}

#if defined(BT_DIALER_SUPPORT)
/*****************************************************************************/
// 	Description : is pbap allow sync
//	Global resource dependence: 
//  Author:allen.lee
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPbapAllowSync(MMICL_CALL_TYPE_E calltype)
{
    BOOLEAN is_pbap_allow_sync = FALSE;
    switch(calltype)
    {
        case MMICL_BT_CALL_MISSED:
            is_pbap_allow_sync = s_is_missed_allow_sync;
            break;
        case MMICL_BT_CALL_RECEIVED:
            is_pbap_allow_sync = s_is_received_allow_sync;
            break;
        case MMICL_BT_CALL_DIALED:
            is_pbap_allow_sync = s_is_dialed_allow_sync;
            break;
        default:
            break;
    }
    return is_pbap_allow_sync;
}

/*****************************************************************************/
// 	Description : set pbap allow sync
//	Global resource dependence: 
//  Author:allen.lee
//	Note: 
/*****************************************************************************/
LOCAL void SetPbapAllowSync(MMICL_CALL_TYPE_E calltype, BOOLEAN is_allow_sync)
{
    switch(calltype)
    {
        case MMICL_BT_CALL_MISSED:
            s_is_missed_allow_sync = is_allow_sync;
            break;
        case MMICL_BT_CALL_RECEIVED:
            s_is_received_allow_sync = is_allow_sync;
            break;
        case MMICL_BT_CALL_DIALED:
            s_is_dialed_allow_sync = is_allow_sync;
            break;
        default:
            break;
    }
}
#endif

/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
LOCAL void  AddTabsAndChildWindows(void)
{
    MMI_STRING_T    str_text = {0};
#if defined(MMI_BLACKLIST_SUPPORT)
    MMI_IMAGE_ID_T cl_pda_tab[4][2] = 
#else
    MMI_IMAGE_ID_T cl_pda_tab[3][2] = 
#endif
        {{IMAGE_CL_MISSED_SELECTED, IMAGE_CL_MISSED_RELEASED},
        {IMAGE_CL_DIALED_SELECTED, IMAGE_CL_DIALED_RELEASED},
        {IMAGE_CL_RECEIVED_SELECTED, IMAGE_CL_RECEIVED_RELEASED},
#if defined(MMI_BLACKLIST_SUPPORT)
        {IMAGE_CL_REFUSED_SELECTED, IMAGE_CL_REFUSED_RELEASED}
#endif
        };

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
#if defined(MMI_BLACKLIST_SUPPORT)
    MMI_IMAGE_ID_T istyle_cl_pda_tab[4][2] = 
#else
    MMI_IMAGE_ID_T istyle_cl_pda_tab[4][2] = 
#endif
        {{ISTYLE_IMAGE_CL_MISSED_SELECTED, ISTYLE_IMAGE_CL_MISSED_RELEASED},
        {ISTYLE_IMAGE_CL_DIALED_SELECTED, ISTYLE_IMAGE_CL_DIALED_RELEASED},
        {ISTYLE_IMAGE_CL_RECEIVED_SELECTED, ISTYLE_IMAGE_CL_RECEIVED_RELEASED},
#if defined(MMI_BLACKLIST_SUPPORT)
        {ISTYLE_IMAGE_CL_REFUSED_SELECTED, ISTYLE_IMAGE_CL_REFUSED_RELEASED}
#endif
    };
#endif

    MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        pda_tab_ptr = istyle_cl_pda_tab;
    }
    else
#endif
    {
        pda_tab_ptr = cl_pda_tab;
    }
    
    GUITAB_SetMaxItemNum(MMICL_LOG_TAB_CTRL_ID, 4);

    MMI_GetLabelTextByLang(TXT_CL_MISSED_CALLS_SHORT, &str_text);
    GUITAB_AppendSimpleItem(
        MMICL_LOG_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[0][0],
        pda_tab_ptr[0][1]);
    
#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
    GUITAB_AddChildWindow(
        MMICL_LOG_TAB_CTRL_ID,
        MMICL_BT_MISSED_CHILD_WIN_ID,
        0
        );
#else
    GUITAB_AddChildWindow(
        MMICL_LOG_TAB_CTRL_ID,
        MMICL_MISSED_CHILD_WIN_ID,
        0
        );
#endif

   MMI_GetLabelTextByLang(TXT_CL_DIALED_CALLS_SHORT, &str_text);
    GUITAB_AppendSimpleItem(
       MMICL_LOG_TAB_CTRL_ID,
       &str_text,
        pda_tab_ptr[1][0],
        pda_tab_ptr[1][1]);
   
#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_BT_DIALED_CHILD_WIN_ID, 
       1
       );
#else
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_DIALED_CHILD_WIN_ID, 
       1
       );
#endif
   
   MMI_GetLabelTextByLang(TXT_CL_RECEIVED_CALLS_SHORT, &str_text);
   GUITAB_AppendSimpleItem(
       MMICL_LOG_TAB_CTRL_ID,
       &str_text,
        pda_tab_ptr[2][0],
        pda_tab_ptr[2][1]);
   
#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_BT_RECEIVED_CHILD_WIN_ID, 
       2
       );
#else
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_RECEIVED_CHILD_WIN_ID, 
       2
       );
#endif
   
#if defined(MMI_BLACKLIST_SUPPORT)
   MMI_GetLabelTextByLang(TXT_CL_REFUSED_CALLS_SHORT, &str_text);
   GUITAB_AppendSimpleItem(
       MMICL_LOG_TAB_CTRL_ID,
       &str_text,
        pda_tab_ptr[3][0],
        pda_tab_ptr[3][1]);
   
   GUITAB_AddChildWindow(
       MMICL_LOG_TAB_CTRL_ID,
       MMICL_REFUSED_CHILD_WIN_ID, 
       3
       );
#endif

   switch (s_cur_sel_log_list_type)
    {
    case MMICL_CALL_MISSED:
#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
    case MMICL_BT_CALL_MISSED:
#endif
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 0);
        break;
        
    case MMICL_CALL_DIALED:
#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
    case MMICL_BT_CALL_DIALED:
#endif
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 1);
        break;
        
    case MMICL_CALL_RECEIVED:
#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
    case MMICL_BT_CALL_RECEIVED:
#endif
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 2);
        break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 3);
        break;
#endif
        
    default:
        GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 0);
        break;
    }
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
//	 Global resource dependence : 
// Author: Cheney.Wu
// 	Note: 
/*****************************************************************************/
LOCAL void  AddSeparateTabsAndChildWindows(void)
{
    MMI_STRING_T    str_text = {0};
#if defined(MMI_BLACKLIST_SUPPORT)
    MMI_IMAGE_ID_T cl_pda_tab[4][2] = 
#else
        MMI_IMAGE_ID_T cl_pda_tab[3][2] = 
#endif
    {{IMAGE_CL_MISSED_SELECTED, IMAGE_CL_MISSED_RELEASED},
    {IMAGE_CL_DIALED_SELECTED, IMAGE_CL_DIALED_RELEASED},
    {IMAGE_CL_RECEIVED_SELECTED, IMAGE_CL_RECEIVED_RELEASED},
#if defined(MMI_BLACKLIST_SUPPORT)
    {IMAGE_CL_REFUSED_SELECTED, IMAGE_CL_REFUSED_RELEASED}
#endif
    };

#ifdef MMI_PDA_SUPPORT
#if defined(MMI_BLACKLIST_SUPPORT)
    MMI_IMAGE_ID_T istyle_cl_pda_tab[4][2] = 
#else
        MMI_IMAGE_ID_T istyle_cl_pda_tab[3][2] = 
#endif
    {{ISTYLE_IMAGE_CL_MISSED_SELECTED, ISTYLE_IMAGE_CL_MISSED_RELEASED},
    {ISTYLE_IMAGE_CL_DIALED_SELECTED, ISTYLE_IMAGE_CL_DIALED_RELEASED},
    {ISTYLE_IMAGE_CL_RECEIVED_SELECTED, ISTYLE_IMAGE_CL_RECEIVED_RELEASED},
#if defined(MMI_BLACKLIST_SUPPORT)
    {ISTYLE_IMAGE_CL_REFUSED_SELECTED, ISTYLE_IMAGE_CL_REFUSED_RELEASED}
#endif
    };
#endif

    MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#ifdef MMI_PDA_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        pda_tab_ptr = istyle_cl_pda_tab;
    }
    else
#endif
    {
        pda_tab_ptr = cl_pda_tab;
    }

    GUITAB_SetMaxItemNum(MMICL_LOG_TAB_CTRL_ID, 2);


    switch (s_cur_sel_log_list_type)
    {
    case MMICL_CALL_MISSED:
        MMI_GetLabelTextByLang(TXT_CL_MISSED_CALLS_SHORT, &str_text);
        GUITAB_AppendSimpleItem(
            MMICL_LOG_TAB_CTRL_ID,
            &str_text,
            pda_tab_ptr[0][0],
            pda_tab_ptr[0][1]);

        GUITAB_AddChildWindow(
            MMICL_LOG_TAB_CTRL_ID,
            MMICL_MISSED_CHILD_WIN_ID,
            0
            );

        MMI_GetLabelTextByLang(TXT_CL_BT_MISSED_CALLS_SHORT, &str_text);
        GUITAB_AppendSimpleItem(
            MMICL_LOG_TAB_CTRL_ID,
            &str_text,
            pda_tab_ptr[0][0],
            pda_tab_ptr[0][1]);
        
        GUITAB_AddChildWindow(
            MMICL_LOG_TAB_CTRL_ID,
            MMICL_BT_MISSED_CHILD_WIN_ID,
            1
            );

        break;

    case MMICL_CALL_DIALED:
        MMI_GetLabelTextByLang(TXT_CL_DIALED_CALLS_SHORT, &str_text);
        GUITAB_AppendSimpleItem(
            MMICL_LOG_TAB_CTRL_ID,
            &str_text,
            pda_tab_ptr[1][0],
            pda_tab_ptr[1][1]);

        GUITAB_AddChildWindow(
            MMICL_LOG_TAB_CTRL_ID,
            MMICL_DIALED_CHILD_WIN_ID, 
            0
            );

        MMI_GetLabelTextByLang(TXT_CL_BT_DIALED_CALLS_SHORT, &str_text);
        GUITAB_AppendSimpleItem(
            MMICL_LOG_TAB_CTRL_ID,
            &str_text,
            pda_tab_ptr[1][0],
            pda_tab_ptr[1][1]);

        GUITAB_AddChildWindow(
            MMICL_LOG_TAB_CTRL_ID,
            MMICL_BT_DIALED_CHILD_WIN_ID, 
            1
            );
        break;

    case MMICL_CALL_RECEIVED:
        MMI_GetLabelTextByLang(TXT_CL_RECEIVED_CALLS_SHORT, &str_text);
        GUITAB_AppendSimpleItem(
            MMICL_LOG_TAB_CTRL_ID,
            &str_text,
            pda_tab_ptr[2][0],
            pda_tab_ptr[2][1]);

        GUITAB_AddChildWindow(
            MMICL_LOG_TAB_CTRL_ID,
            MMICL_RECEIVED_CHILD_WIN_ID, 
            0
            );

        MMI_GetLabelTextByLang(TXT_CL_BT_RECEIVED_CALLS_SHORT, &str_text);
        GUITAB_AppendSimpleItem(
            MMICL_LOG_TAB_CTRL_ID,
            &str_text,
            pda_tab_ptr[2][0],
            pda_tab_ptr[2][1]);

        GUITAB_AddChildWindow(
            MMICL_LOG_TAB_CTRL_ID,
            MMICL_BT_RECEIVED_CHILD_WIN_ID, 
            1
            );
        break;

#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        MMI_GetLabelTextByLang(TXT_CL_REFUSED_CALLS_SHORT, &str_text);
        GUITAB_AppendSimpleItem(
            MMICL_LOG_TAB_CTRL_ID,
            &str_text,
            pda_tab_ptr[3][0],
            pda_tab_ptr[3][1]);

        GUITAB_AddChildWindow(
            MMICL_LOG_TAB_CTRL_ID,
            MMICL_REFUSED_CHILD_WIN_ID, 
            0
            );
        break;
#endif

    default:
        break;
    }
    GUITAB_SetCurSel(MMICL_LOG_TAB_CTRL_ID, 0);
}

/*****************************************************************************/
// Description : start calllog pbap sync process
// Global resource dependence : 
// Author: Cheney.Wu
// Note: 
/*****************************************************************************/
LOCAL void StartPbapCalllogSync(MMI_WIN_ID_T win_id)
{
    uint32       dev_service = 0;
    BT_ADDRESS   dev_addr = {0};

	SCI_TRACE_LOW("[MMICL] StartPbapCalllogSync");
    if(IsBTCalllogWinId(win_id))
    {
        MMIAPIBT_GetHfuConnectedAddress(&dev_addr);
#ifndef WIN32
        if(BT_HfuGetConnStatus(&dev_addr))
#endif
        {
            dev_service = BT_GetPairedDeviceService(&dev_addr);

        if(FALSE == BT_GetState())
        {
            MMIAPICL_DeleteAllPbapSyncData();
            return;
        }

        if(FALSE == MMIAPIBT_IsConnectDevice())
        {
            MMIAPICL_DeleteAllPbapSyncData();
            return;
        }
        if(FALSE == IsPbapAllowSync(s_cur_sel_log_list_type))
        {
            return;
        }
        SetPbapAllowSync(s_cur_sel_log_list_type, FALSE);
#if !defined(WIN32)
        if(0 == (BT_SERVICE_PBAP_SERVER & dev_service))
        {
            MMIBT_OpenAlertTipWin(TXT_BT_PBAP_NOT_SUPPORT); 
            return;
        }
#endif
        MMIBT_SetPbapRequestType(BT_PBAP_TYPE_CL);
#if !defined(WIN32)
        if(MMIAPIBT_PbapIsConnected())
        {
            /************************************************************************/
            /* whether Static Data List is empty or not                             */
            /************************************************************************/
            MMICL_InitPBAPDataList();
            
            MMK_SendMsg(MMIAPICL_GetActivateBTWinId(), MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
        }
        else
#endif
        {
            if(!MMK_IsOpenWin(MMICL_PBAP_CONNECT_WAIT_WIN_ID))
            {
                MMK_SendMsg(MMIAPICL_GetActivateBTWinId(), MSG_BT_CALLLOG_SYNC_CNF, PNULL);
            }
        }
    }
}
}
#endif

/*****************************************************************************/
// 	Description : to handle tab parent window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListWindow(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 cur_select = 0;
    
	SCI_TRACE_LOW("[MMICL] HandleLogListWindow msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMICL_LOG_TAB_CTRL_ID);
#if defined(BT_DIALER_SUPPORT) && !defined(MMI_MULTI_SIM_SYS_NONE)
        AddSeparateTabsAndChildWindows();
#else
        AddTabsAndChildWindows();
#endif
        break;

    case MSG_NOTIFY_TAB_SWITCH:
#ifdef MMI_CL_MINI_SUPPORT
         ExitSelect();
#endif
        cur_select = GUITAB_GetCurSel(MMICL_LOG_TAB_CTRL_ID);

#ifndef BT_DIALER_SUPPORT
        switch (cur_select)
        {
        case 0:
            s_cur_sel_log_list_type = MMICL_CALL_MISSED;
            break;

        case 1:
            s_cur_sel_log_list_type = MMICL_CALL_DIALED;
            break;

        case 2:
            s_cur_sel_log_list_type = MMICL_CALL_RECEIVED;
            break;

#if defined(MMI_BLACKLIST_SUPPORT)
        case 3:
            s_cur_sel_log_list_type = MMICL_CALL_REFUSED;
            break;
#endif

        default:
            s_cur_sel_log_list_type = MMICL_CALL_MISSED;
            break;
        }
        MMK_PostMsg(MMICL_MAIN_MENU_WIN_ID, MMI_CL_MAINMENU_UPDATE_INDEX, &cur_select, sizeof(cur_select));
#else

#ifdef MMI_MULTI_SIM_SYS_NONE
        switch (cur_select)
        {
        case 0:
            s_cur_sel_log_list_type = MMICL_BT_CALL_MISSED;
            break;
            
        case 1:
            s_cur_sel_log_list_type = MMICL_BT_CALL_DIALED;
            break;
            
        case 2:
            s_cur_sel_log_list_type = MMICL_BT_CALL_RECEIVED;
            break;
            
#if defined(MMI_BLACKLIST_SUPPORT)
        case 3:
            s_cur_sel_log_list_type = MMICL_CALL_REFUSED;
            break;
#endif
            
        default:
            s_cur_sel_log_list_type = MMICL_CALL_MISSED;
            break;
        }
#else
        if(0 == cur_select)
        {
            switch (s_cur_sel_log_list_type)
            {
            case MMICL_CALL_MISSED:
            case MMICL_BT_CALL_MISSED:
                s_cur_sel_log_list_type = MMICL_CALL_MISSED;
                break;
                
            case MMICL_CALL_DIALED:
            case MMICL_BT_CALL_DIALED:
                s_cur_sel_log_list_type = MMICL_CALL_DIALED;
                break;
                
            case MMICL_CALL_RECEIVED:
            case MMICL_BT_CALL_RECEIVED:
                s_cur_sel_log_list_type = MMICL_CALL_RECEIVED;
                break;
                
#if defined(MMI_BLACKLIST_SUPPORT)
            case MMICL_CALL_REFUSED:
                s_cur_sel_log_list_type = MMICL_CALL_REFUSED;
                break;
#endif
            default:
                s_cur_sel_log_list_type = MMICL_CALL_MISSED;
                break;
            }
        }
        else
        {
            switch (s_cur_sel_log_list_type)
            {
            case MMICL_CALL_MISSED:
                s_cur_sel_log_list_type = MMICL_BT_CALL_MISSED;
                break;
                
            case MMICL_CALL_DIALED:
                s_cur_sel_log_list_type = MMICL_BT_CALL_DIALED;
                break;
                
            case MMICL_CALL_RECEIVED:
                s_cur_sel_log_list_type = MMICL_BT_CALL_RECEIVED;
                break;
            default:
                s_cur_sel_log_list_type = MMICL_BT_CALL_MISSED;
                break;
            }
        }
#endif
#endif

#if defined(BT_DIALER_SUPPORT) && defined(MMI_MULTI_SIM_SYS_NONE)
        if(TRUE == IsPbapAllowSync(s_cur_sel_log_list_type))
        {
            StartPbapCalllogSync(win_id);
        }
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : handle toolbar item sel 
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_HandleToolBarItemSel(CL_TOOLBAR_TYPE_E node_id)
{
    switch(node_id)
    {
        case     CL_TOOLBAR_DIAL_AUDIO:
            HandleRingBackByType(CC_CALL_NORMAL_CALL);
            break;

        #ifdef VT_SUPPORT
        case     CL_TOOLBAR_DIAL_VIDEO:        
            HandleRingBackByType(CC_CALL_VIDEO_CALL);            
            break;
        #endif
#ifdef MMI_IP_CALL_SUPPORT
        case     CL_TOOLBAR_DIAL_IPCALL:        
            HandleRingBackByType(CC_CALL_IP_CALL);            
            break;
#endif
        case     CL_TOOLBAR_DIAL_SMS:
            HandleAnswerSms();    
            break;
#ifdef CALL_TIMES_SUPPORT
        case     CL_TOOLBAR_DIAL_VIEW:
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
            break;
#endif       
        default:
            break;

    }           
}

/*****************************************************************************/
//  Description : Update EntryList Toolbar State 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMICL_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id)
{ 
#ifdef GUIF_TOOLBAR
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    GUITOOLBAR_INIT_DATA_T init_data = {0};
    GUITOOLBAR_ITEM_T cl_toolbar_item_a [] = 
    {
        {CL_TOOLBAR_DIAL_AUDIO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_CALL, IMAGE_TOOLBAR_CL_CALL_GRAY,0},/*lint !e785*/
        #ifdef  VT_SUPPORT
        {CL_TOOLBAR_DIAL_VIDEO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_VIDEOCALL, IMAGE_TOOLBAR_CL_VIDEOCALL,0},/*lint !e785*/
        #endif
#ifdef MMI_IP_CALL_SUPPORT
        {CL_TOOLBAR_DIAL_IPCALL, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_IPCALL, IMAGE_TOOLBAR_CL_IPCALL_GRAY,0},/*lint !e785*/
#endif
        {CL_TOOLBAR_DIAL_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_WRITESMS, IMAGE_TOOLBAR_CL_WRITESMS_GRAY,0},/*lint !e785*/
    };  

    switch(list_ctrl_id)
    {
    case MMICL_DIALED_LIST_CTRL_ID:
        call_type = MMICL_CALL_DIALED;
        break;
        
    case MMICL_RECEIVED_LIST_CTRL_ID:
        call_type = MMICL_CALL_RECEIVED;
        break;
    
    case MMICL_MISSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_MISSED;
        break;
    
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_REFUSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_REFUSED;
        break;
#endif
    
    case MMICL_ALL_LIST_CTRL_ID:
        call_type = MMICL_CALL_ALL;
        break;
        
    default:
        return;     
    }
           
    MMK_GetBothRect(win_id, &init_data.both_rect);
    
    GUITOOLBAR_CreateDynamic(win_id, ctrl_id, &init_data);
    GUITOOLBAR_SetAllItem(MMK_GetCtrlHandleByWin(win_id, ctrl_id), 
        cl_toolbar_item_a, 
        sizeof(cl_toolbar_item_a)/sizeof(GUITOOLBAR_ITEM_T));
#endif        
}
/*****************************************************************************/
// 	Description : update cl toolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_UpdateCLWinToolbar(
									MMI_WIN_ID_T      win_id,
									MMI_CTRL_ID_T       list_ctrl_id                                                                       
									)
{
    uint16  icon_width = 0;
    uint16  icon_height = 0;
    uint16  cur_index = 0;
    GUI_RECT_T cur_rect = {0};
    GUI_POINT_T pt_bar = {0};
    MMI_CTRL_ID_T       toolbar_ctrl_id = {0};
    //MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    MMI_HANDLE_T   toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, list_ctrl_id);//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
    MMI_HANDLE_T    list_handle;
    
    switch(list_ctrl_id)
    {
        case MMICL_DIALED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_DIALED_CTRL_ID;
            call_type = MMICL_CALL_DIALED;
            break;

        case MMICL_RECEIVED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_RECEIVED_CTRL_ID;
            call_type = MMICL_CALL_RECEIVED;
            break;
        
        case MMICL_MISSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_MISSED_CTRL_ID;
            call_type = MMICL_CALL_MISSED;
            break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_REFUSED_CTRL_ID;
            call_type = MMICL_CALL_REFUSED;
            break;
#endif
        
     case MMICL_ALL_LIST_CTRL_ID:
            toolbar_ctrl_id = MMICL_TOOLBAR_ALLLIST_CTRL_ID;
            call_type = MMICL_CALL_ALL;
            break;

#ifdef BT_DIALER_SUPPORT
     case MMICL_BT_DIALED_LIST_CTRL_ID:
         toolbar_ctrl_id =  MMICL_TOOLBAR_BT_DIALED_CTRL_ID;
         call_type = MMICL_BT_CALL_DIALED;
         break;

     case MMICL_BT_RECEIVED_LIST_CTRL_ID:
         toolbar_ctrl_id =  MMICL_TOOLBAR_BT_RECEIVED_CTRL_ID;
         call_type = MMICL_CALL_RECEIVED;
         break;

     case MMICL_BT_MISSED_LIST_CTRL_ID:
         toolbar_ctrl_id =  MMICL_TOOLBAR_BT_MISSED_CTRL_ID;
         call_type = MMICL_BT_CALL_MISSED;
         break;

     case MMICL_BT_ALL_LIST_CTRL_ID:
         toolbar_ctrl_id = MMICL_TOOLBAR_BT_ALLLIST_CTRL_ID;
         call_type = MMICL_BT_CALL_ALL;
         break;
#endif

        default:              
            return;     
    }
    toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, toolbar_ctrl_id);
    list_handle = MMK_GetCtrlHandleByWin(win_id, list_ctrl_id);

    GUITOOLBAR_SetVisible(toolbar_ctrl_handle,TRUE);

    cur_index = GUILIST_GetCurItemIndex(list_handle);	
    GUILIST_GetItemRect(list_handle, cur_index, &cur_rect);
    GUITOOLBAR_GetIconWidthHeight(toolbar_ctrl_handle, &icon_width, &icon_height);
    
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        pt_bar.x = cur_rect.right;
    }
    else
    {
        pt_bar.x = cur_rect.left;
    }
    
    if (0 != cur_index)
    {
        pt_bar.y = cur_rect.bottom - MMICL_TOOLBAR_PANEL_GAP + 1; //向上
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_handle, GUITOOLBAR_DIR_DOWN);
    }
    else
    {
        pt_bar.y = cur_rect.bottom + MMICL_TOOLBAR_PANEL_GAP - 1; //向下
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_handle, GUITOOLBAR_DIR_UP);
    }

    if (GUITOOLBAR_SetPanelBasePt(toolbar_ctrl_handle, &pt_bar, FALSE))
    {
		GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK );
        GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_EXPAND );
    }
    else        //如果相同的PTR,则会返回FALSE
    {
        if(GUITOOLBAR_GetShowType(toolbar_ctrl_handle)==GUITOOLBAR_EXPAND)
        {
            GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK );
        }
        else
        {
            GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_EXPAND );
        }
    }

    
}

#else
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : handle toolbar item sel 
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_HandleToolBarItemSel(CL_TOOLBAR_TYPE_E node_id)
{
    switch(node_id)
    {
        case     CL_TOOLBAR_DIAL_AUDIO:
            HandleRingBack(CC_CALL_SIM_MAX, FALSE);             //info_ptr can be null
            break;
#ifdef VT_SUPPORT
        case     CL_TOOLBAR_DIAL_VIDEO:        
            HandleRingBack(CC_CALL_SIM_MAX, TRUE);            
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case     CL_TOOLBAR_DIAL_IPCALL:        
            HandleIpCall();                
            break;
#endif
        case     CL_TOOLBAR_DIAL_SMS:
            HandleAnswerSms();    
            break;
        default:
            break;
    }           
}

/*****************************************************************************/
//  Description : Update EntryList Toolbar State 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMICL_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id)
{    
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    GUITOOLBAR_INIT_DATA_T init_data = {0};
    GUITOOLBAR_ITEM_T cl_toolbar_item_a [] = 
    {
        {CL_TOOLBAR_DIAL_AUDIO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_CALL, IMAGE_TOOLBAR_CL_CALL_GRAY,0},/*lint !e785*/
        #ifdef  VT_SUPPORT
        {CL_TOOLBAR_DIAL_VIDEO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_VIDEOCALL, IMAGE_TOOLBAR_CL_VIDEOCALL_GRAY,0},/*lint !e785*/
        #endif
        #ifdef MMI_IP_CALL_SUPPORT
        {CL_TOOLBAR_DIAL_IPCALL, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_IPCALL, IMAGE_TOOLBAR_CL_IPCALL_GRAY,0},/*lint !e785*/
        #endif
        {CL_TOOLBAR_DIAL_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_WRITESMS, IMAGE_TOOLBAR_CL_WRITESMS_GRAY,0},/*lint !e785*/
    };  

    switch(list_ctrl_id)
    {
    case MMICL_DIALED_LIST_CTRL_ID:
        call_type = MMICL_CALL_DIALED;
        break;
        
    case MMICL_RECEIVED_LIST_CTRL_ID:
        call_type = MMICL_CALL_RECEIVED;
        break;
    
    case MMICL_MISSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_MISSED;
        break;
    
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_REFUSED_LIST_CTRL_ID:
        call_type = MMICL_CALL_REFUSED;
        break;
#endif

    default:
        return;     
    }
           
    MMK_GetBothRect(list_ctrl_id, &init_data.both_rect);
    
    GUITOOLBAR_CreateDynamic(win_id, ctrl_id, &init_data);
    GUITOOLBAR_SetAllItem(ctrl_id, 
        cl_toolbar_item_a, 
        sizeof(cl_toolbar_item_a)/sizeof(GUITOOLBAR_ITEM_T));
    
}
/*****************************************************************************/
// 	Description : update cl toolbar
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMICL_UpdateCLWinToolbar(
									MMI_WIN_ID_T      win_id,
									MMI_CTRL_ID_T       list_ctrl_id                                                                       
									)
{
    uint16  icon_width = 0;
    uint16  icon_height = 0;
    uint16  cur_index = 0;
    GUI_RECT_T cur_rect = {0};
    GUI_POINT_T pt_bar = {0};
    MMI_CTRL_ID_T       toolbar_ctrl_id = {0};
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_TYPE_MAX;
    
    switch(list_ctrl_id)
    {
        case MMICL_DIALED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_DIALED_CTRL_ID;
            call_type = MMICL_CALL_DIALED;
            break;

        case MMICL_RECEIVED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_RECEIVED_CTRL_ID;
            call_type = MMICL_CALL_RECEIVED;
            break;
        
        case MMICL_MISSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_MISSED_CTRL_ID;
            call_type = MMICL_CALL_MISSED;
            break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_LIST_CTRL_ID:
            toolbar_ctrl_id =  MMICL_TOOLBAR_REFUSED_CTRL_ID;
            call_type = MMICL_CALL_REFUSED;
            break;
#endif
                
        default:              
            return;     
    }
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));    

    if(call_type  != MMICL_CALL_MATCHED)
    {
        MMICL_GetRecordInfo( call_type, arraycall_info);
    }
    
    if(( MMICL_CALL_MATCHED == call_type && (s_match_call_info_ptr != NULL && 0 == s_match_call_info_ptr->match_num))
    ||
    #ifdef MMI_PDA_SUPPORT
    (MMICL_CALL_MATCHED != call_type &&  0 == GetCurrTypeNum( call_type,arraycall_info))
    #else
    (MMICL_CALL_MATCHED != call_type &&  0 == arraycall_info->record_num)
    #endif    
    )
    {
        GUITOOLBAR_SetVisible(toolbar_ctrl_id,FALSE);
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
        return;
    }
    else 
    {
        GUITOOLBAR_SetVisible(toolbar_ctrl_id,TRUE);
    }
    SCI_FREE(arraycall_info);
    arraycall_info = PNULL;
    
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);	
    GUILIST_GetItemRect(list_ctrl_id, cur_index, &cur_rect);
    GUITOOLBAR_GetIconWidthHeight(toolbar_ctrl_id, &icon_width, &icon_height);
    
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        pt_bar.x = cur_rect.right;
    }
    else
    {
        pt_bar.x = cur_rect.left;
    }
    
    if (0 != cur_index)
    {
        pt_bar.y = cur_rect.bottom - MMICL_TOOLBAR_PANEL_GAP + 1; //向上
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_id, GUITOOLBAR_DIR_DOWN);
    }
    else
    {
        pt_bar.y = cur_rect.bottom + MMICL_TOOLBAR_PANEL_GAP - 1; //向下
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_id, GUITOOLBAR_DIR_UP);
    }
    GUITOOLBAR_SetPanelBasePt(toolbar_ctrl_id, &pt_bar, FALSE);
    
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        cur_rect.left += MMICL_TOOLBAR_ICON_GAP_RIGHT;
        cur_rect.right = cur_rect.left + icon_width - 1;
    }
    else
    {
        cur_rect.right -= MMICL_TOOLBAR_ICON_GAP_RIGHT;
        cur_rect.left = cur_rect.right - icon_width + 1;
    }
    
    cur_rect.top += MMICL_TOOLBAR_ICON_GAP_TOP;
    cur_rect.bottom = cur_rect.top + icon_height - 1;
    GUITOOLBAR_SetIconRect(toolbar_ctrl_id, &cur_rect, TRUE);	
}
#endif
#endif
#endif

#ifdef MMI_PDA_SUPPORT
LOCAL void DrawListItemIconData(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info,GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr)
{    
    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
    uint32 custom_photo_name_len = 0;
    uint32 photo_file_size = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    uint32 array_index = 0;
    if(arraycall_info == PNULL ||need_item_content_ptr == PNULL )
    {
        return;
    }
    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), need_item_content_ptr->item_index, &array_index);
    
    SCI_MEMSET(&pb_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));

    pb_num.number_len = arraycall_info->call_bcdinfo[array_index].number_len;
    pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[array_index].number_type,
      arraycall_info->call_bcdinfo[array_index].number_plan);
    MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
      arraycall_info->call_bcdinfo[array_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
      pb_num.number_len);

    if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
    {
       item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;     
       anim_path.full_path_wstr_ptr = custom_photo_name;
       anim_path.full_path_wstr_len = custom_photo_name_len;
       item_content.item_data.anim_path_ptr = &anim_path;
       GUILIST_SetItemContent(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), &item_content, need_item_content_ptr->item_index, need_item_content_ptr->item_content_index); 
    }          
}
#endif

/*****************************************************************************/
// 	Description : Get ctrl id for different win id
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetCtrlIDFromWinID(MMI_WIN_ID_T win_id)
{
    switch (win_id)
    {
        case MMICL_DIALED_CHILD_WIN_ID:
            return MMICL_DIALED_LIST_CTRL_ID;
        case MMICL_RECEIVED_CHILD_WIN_ID:
            return MMICL_RECEIVED_LIST_CTRL_ID;
        case MMICL_MISSED_CHILD_WIN_ID:
            return MMICL_MISSED_LIST_CTRL_ID;
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_CHILD_WIN_ID:
            return MMICL_REFUSED_LIST_CTRL_ID;
#endif

#ifdef BT_DIALER_SUPPORT
        case MMICL_BT_DIALED_CHILD_WIN_ID:
            return MMICL_BT_DIALED_LIST_CTRL_ID;
        case MMICL_BT_RECEIVED_CHILD_WIN_ID:
            return MMICL_BT_RECEIVED_LIST_CTRL_ID;
        case MMICL_BT_MISSED_CHILD_WIN_ID:
            return MMICL_BT_MISSED_LIST_CTRL_ID;
#endif

        default:
            break;
    }
    return MMICL_DIALED_LIST_CTRL_ID;
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : Get ctrl id for different win id
// 	Global resource dependence : 
// Author: cheney.wu
//	 Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsBTCalllogWinId(MMI_WIN_ID_T win_id)
{
    switch (win_id)
    {
#if defined(MMI_MULTI_SIM_SYS_NONE)
    case MMICL_LOG_LIST_WIN_ID:
    case MMICL_DIALED_CHILD_WIN_ID:
    case MMICL_RECEIVED_CHILD_WIN_ID:
    case MMICL_MISSED_CHILD_WIN_ID:
#endif
    case MMICL_BT_DIALED_CHILD_WIN_ID:
    case MMICL_BT_RECEIVED_CHILD_WIN_ID:
    case MMICL_BT_MISSED_CHILD_WIN_ID:
        return TRUE;
        
    default:
        break;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : Get win id for bt sync win id
// 	Global resource dependence : 
// Author:cheney.wu
// 	Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPICL_GetActivateBTWinId(void)
{
    MMI_WIN_ID_T win_id = 0;
    
    SCI_TRACE_LOW("[MMIAPICL_GetActivateBTWinId] s_cur_sel_log_list_type %d", s_cur_sel_log_list_type);
    switch (s_cur_sel_log_list_type)
    {
    case MMICL_BT_CALL_DIALED:
        win_id = MMICL_BT_DIALED_CHILD_WIN_ID;
        break;
        
    case MMICL_BT_CALL_MISSED:
        win_id = MMICL_BT_MISSED_CHILD_WIN_ID;
        break;
        
    case MMICL_BT_CALL_RECEIVED:
        win_id = MMICL_BT_RECEIVED_CHILD_WIN_ID;
        break;
        
    default:
        return 0;
    }
#if 0
    if (MMK_IsOpenWin(win_id))
    {
        SCI_TRACE_LOW("[MMIAPICL_GetActivateBTWinId] MMK_IsOpenWin win_id %d");
        return win_id;
    }
    
    SCI_TRACE_LOW("[MMIAPICL_GetActivateBTWinId] MMK_IsOpenWin fail");
    return 0;
#else
    return win_id;
#endif
}
#endif

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Get menu offset by calltype
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint16 GetMenuIDOffsetByCallType(MMICL_CALL_TYPE_E call_type)
{
    switch (call_type)
    {
        case MMICL_CALL_MISSED:
            return 0;
        case MMICL_CALL_RECEIVED:
            return (uint16)ID_CL_RECEIVED_CALLS-(uint16)ID_CL_MISSED_CALLS;
        case MMICL_CALL_DIALED:
            return (uint16)ID_CL_DIALED_CALLS-(uint16)ID_CL_MISSED_CALLS;
#if defined(BT_PBAP_SUPPORT)
        case MMICL_BT_CALL_MISSED:
            return 0;
        case MMICL_BT_CALL_RECEIVED:
            return (uint16)ID_CL_RECEIVED_CALLS-(uint16)ID_CL_MISSED_CALLS;
        case MMICL_BT_CALL_DIALED:
            return (uint16)ID_CL_DIALED_CALLS-(uint16)ID_CL_MISSED_CALLS;
#endif
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
            return (uint16)ID_CL_REFUSED_CALLS-(uint16)ID_CL_MISSED_CALLS;
#endif            
        default:
            return 0;
    }
}
#endif

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : CreatePbapTimer
//  Global resource dependence : 
//  Author: cheney.wu
//  Note: 创建超时定时器
/*****************************************************************************/
LOCAL void CreatePbapTimer(void)
{
    SCI_TRACE_LOW("[MMIAPICLBT] CreatePbapTimer enter");
    
    if(s_pbap_timeout_timer != 0) 
    {
        MMIAPICL_StopPbapTimer();
    }
    
#ifdef WIN32
    s_pbap_timeout_timer = MMK_CreateTimerCallback(1000, ProcessPbapTimerOver, 0, FALSE);
#else
    s_pbap_timeout_timer = MMK_CreateTimerCallback(BT_PBAP_TIMER_OUT, ProcessPbapTimerOver, 0, FALSE);
#endif
    
    SCI_TRACE_LOW("[MMIAPICLBT] CreatePbapTimer exit, timer_id = %d", s_pbap_timeout_timer);
}

/******************************************************************************/
// Description: StopPbapTimer
// Global resource dependence: 
// Author: cheney.wu
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIAPICL_StopPbapTimer(void)
{
    SCI_TRACE_LOW("[MMIAPICLBT] StopPbapTimer enter");
    
    if(s_pbap_timeout_timer != 0)    
    {   
        MMK_StopTimer(s_pbap_timeout_timer);   
        s_pbap_timeout_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIAPICLBT] StopPbapTimer exit, timer_id = %d", s_pbap_timeout_timer);
}

/******************************************************************************/
// Description: ProcessPbapTimerOver
// Global resource dependence: 
// Author: cheney.wu
// Note: 超时保护机制
/******************************************************************************/
LOCAL void ProcessPbapTimerOver(uint8 timer_id, uint32 param)
{
#ifdef WIN32
    MMIPB_CNF_MSG_T     cnf_msg = {0};
    cnf_msg.ret = MMIPB_ERROR_PENDING;
    MMIPB_SendSignalToMMITask(MMI_PB_PBAP_CALLLOG_READ_CNF, &cnf_msg);
#else
    SCI_TRACE_LOW("[MMIAPICLBT] ProcessPbapTimerOver enter");
    
    MMIAPICL_StopPbapTimer();
    MMK_CloseWin(MMICL_PBAP_CONNECT_WAIT_WIN_ID);
    MMIPUB_OpenAlertWarningWin(TXT_BT_TIMEOUT_ALERT);
    PbapClientTpDisconnect();
#endif
}

/*****************************************************************************/
//  Description : Open alert tip win
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
LOCAL void OpenPbapAlertTipWin(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T   alert_id = MMI_BT_ALERT_WIN_ID; 
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;
    
    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,text_id,TXT_NULL,image_id,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,0); 
}

/*****************************************************************************/
//  Description : Registers the Phonebook Access Client
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS PbapClientRegister(void)
{
#ifndef WIN32
#ifndef BT_DIALER_DIRECT_CONNECT
    BT_STATUS ret_val = BT_ERROR;
    
    ret_val = BT_PbapRegisterClient();
    SCI_TRACE_LOW("[PbapClientRegister] ret_val = 0x%x", ret_val);
    
    return ret_val;
#else
    return BT_SUCCESS;
#endif
#else
    return BT_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : Deregisters the PBAP Access Client
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMICL_PbapClientDeregister(void)
{
// #ifndef WIN32
// #ifndef BT_DIALER_DIRECT_CONNECT
//     BT_STATUS ret_val = BT_ERROR;
//     ret_val = BT_PbapDeregisterClient();
//     
//     SCI_TRACE_LOW("[MMICL_PbapClientDeregister] ret_val = 0x%x", ret_val);
//     if(BT_SUCCESS != ret_val)
//     {
//         MMIPUB_OpenAlertFailWin(TXT_ERROR);
//     }
// #endif
// #else
//     return;
// #endif
}

/*****************************************************************************/
//  Description : Create a transport connection to the specified device 
//                and issue an OBEX Connect Request
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS PbapConnect(const BT_ADDRESS *addr)
{
#ifndef WIN32
#ifndef BT_DIALER_DIRECT_CONNECT
    BT_STATUS ret_val = BT_ERROR;
    
    ret_val = BT_PbapConnect(addr);
    SCI_TRACE_LOW("[PbapConnect] ret_val = 0x%x", ret_val);
    
    return ret_val;
#else
    return BT_SUCCESS;
#endif
#else
    return BT_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
LOCAL void PbapDisconnect(void)
{
// #ifndef WIN32
// #ifndef BT_DIALER_DIRECT_CONNECT
//     BT_STATUS ret_val = BT_ERROR;
//     ret_val = BT_PbapDisconnect();
//     
//     SCI_TRACE_LOW("[PbapDisconnect] ret_val = 0x%x", ret_val);
//     if(BT_SUCCESS == ret_val)
//     {
//         MMICL_PbapClientDeregister();
//     }
//     else if((BT_PENDING != ret_val) && (BT_BUSY != ret_val))
//     {
//         MMIPUB_OpenAlertFailWin(TXT_ERROR);
//     }
// #endif
// #else
//     return;
// #endif
}

/*****************************************************************************/
//  Description : to initiate a transport disconnection. 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note: BT_PbapDisconnect will cause the transport to be brought down automatically, 
//  but if the transport must be brought down immediately, perhaps due an error condition, 
//  this routine may be used. 
/*****************************************************************************/
LOCAL void PbapClientTpDisconnect(void)
{
#ifndef WIN32
// #ifndef BT_DIALER_DIRECT_CONNECT
//     BT_STATUS ret_val = BT_ERROR;
//     ret_val = BT_PbapClientTpDisconnect();
//     
//     SCI_TRACE_LOW("[PbapClientTpDisconnect] ret_val = 0x%x", ret_val);
//     if(BT_SUCCESS == ret_val)
//     {
//         PbapDisconnect();
//     }
//     else if(BT_PENDING != ret_val)
//     {
//         MMIPUB_OpenAlertFailWin(TXT_ERROR);
//     }
// #endif
#else
    return;
#endif
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        break;
        
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
LOCAL void PbapAbortClient(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    MMI_STRING_T                wait_text = {0};
    ret_val = BT_PbapAbortClient();

    if(BT_SUCCESS == ret_val)
    {
        MMI_GetLabelTextByLang(TXT_ST_WAIT, &wait_text); 
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinWaitingMsg); 
        MMIPUB_SetWinSoftkey(MMIBT_WAIT_WIN_ID, TXT_NULL, TXT_NULL, TRUE);
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
// #ifndef BT_DIALER_DIRECT_CONNECT
//         MMIPB_PbapDisconnect();
// #endif
    }
    else if(BT_PBAP_TYPE_SIM != MMIBT_GetPbapRequestType())
    {
        MMIPUB_OpenAlertFailWin(TXT_BT_PULL_PHONEBOOK_ABORT_ERROR);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : Initiates the OBEX "Get" operation to retrieve a phonebook object 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS PbapPullCalllog(MMICL_CALL_TYPE_E log_list_type)
{
#ifndef WIN32
    BT_PBAP_VCARD_FILTER_T  pbap_filter = {0};
    BT_PBAP_VCARD_FORMAT_T  format = MMIAPICLBT_PBAP_VCARD_FORMAT_21;//BT_VCARD_FORMAT_30;//
    uint16                  max_list_count = MMICL_RECORD_MAX_NUM;
    uint16                  list_offset = 0;
    
    SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x00\x00\x00\x00\x00\x00\x00\x84", BT_PBAP_FILTER_SIZE*sizeof(uint8));
    
    switch (log_list_type)
    {
    case MMICL_BT_CALL_DIALED:
        SCI_TRACE_LOW("PbapPullCalllog with BT dialed call");
        return BT_PbapPullPhonebook(
            s_och_name,
            &pbap_filter,
            format,
            max_list_count,
            list_offset
            );
    case MMICL_BT_CALL_RECEIVED:
        SCI_TRACE_LOW("PbapPullCalllog with BT received call");
        return BT_PbapPullPhonebook(
            s_ich_name,
            &pbap_filter,
            format,
            max_list_count,
            list_offset
            );
    case MMICL_BT_CALL_MISSED:
        SCI_TRACE_LOW("PbapPullCalllog with BT missed call");
        return BT_PbapPullPhonebook(
            s_mch_name,
            &pbap_filter,
            format,
            max_list_count,
            list_offset
            );
    case MMICL_BT_CALL_ALL:
        SCI_TRACE_LOW("PbapPullCalllog with BT all call");
        return BT_PbapPullPhonebook(
            s_cch_name,
            &pbap_filter,
            format,
            max_list_count,
            list_offset
            );
    default:
        return BT_PENDING;
    }
    
#else
    return BT_PENDING;
#endif
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  cheney.wu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinConnectWaitingMsg(
                                                 MMI_WIN_ID_T         win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIAPICL_StopPbapTimer();
        PbapClientTpDisconnect();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  cheney.wu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinSyncWaitingMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        PbapAbortClient();
        // s_is_cancel_pull = TRUE;
        MMK_CloseWin(win_id);
        //MMIPUB_OpenAlertSuccessWin(STXT_CANCEL);
        break;
        
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}
#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
// Description : HandleBTCallogSyncCfn
// Global resource dependence : 
// Author: Cheney.Wu
// Note: 
/*****************************************************************************/
LOCAL void HandleBTCallogSyncCfn(MMI_WIN_ID_T win_id)
{
    BT_ADDRESS                  dev_addr = {0};
    BOOLEAN                     is_dev_connect = FALSE;
    BT_STATUS                   register_ret = BT_ERROR;
    BT_STATUS                   connect_ret = BT_ERROR;
    MMI_STRING_T                wait_text = {0};

	SCI_TRACE_LOW("[MMICL] HandleBTCallogSyncCfn");
    is_dev_connect = MMIBT_IsConnectDevice(&dev_addr);
    
    if(!is_dev_connect)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    
    /************************************************************************/
    /*Registers the Phonebook Access Client                                  */
    /************************************************************************/
    register_ret = PbapClientRegister();
    
    SCI_TRACE_LOW("[MMIAPICLBT] MSG_BT_CALLLOG_SYNC_CNF register_ret:%d", register_ret);
    
    if((BT_SUCCESS != register_ret) && ((BT_IN_USE != register_ret)))
    {
        OpenPbapAlertTipWin(TXT_BT_REGISTER_FAIL); 
        return;
    }
    
    /************************************************************************/
    /* whether Static Data List is empty or not                             */
    /************************************************************************/
    MMICL_InitPBAPDataList();
    
    /************************************************************************/
    /* Create a transport connection to the specified device and issue an  
    OBEX Connect Request */
    /************************************************************************/
    if(BT_IN_USE != register_ret)
    {
        if(!MMK_IsOpenWin(MMICL_PBAP_CONNECT_WAIT_WIN_ID))
        {
            MMI_GetLabelTextByLang(TXT_BT_CONNECT_WAITING, &wait_text); 
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMICL_PBAP_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinConnectWaitingMsg); 
            MMIPUB_SetWinSoftkey(MMICL_PBAP_CONNECT_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
        }
    }
    
    CreatePbapTimer();
    
    connect_ret = PbapConnect(&dev_addr);
    
    SCI_TRACE_LOW("[MMIAPICLBT] MSG_BT_CALLLOG_SYNC_CNF connect_ret:%d", connect_ret);
    
    if(BT_SUCCESS == connect_ret)
    {
        MMK_SendMsg(win_id, MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
    }
    else if(BT_PENDING != connect_ret)
    {
        OpenPbapAlertTipWin(TXT_BT_PBAP_CONNECT_FAIL);                    
    }
}

/*****************************************************************************/
// Description : HandleBTPbapConnectSuccess
// Global resource dependence : 
// Author: Cheney.Wu
// Note: 
/*****************************************************************************/
LOCAL void HandleBTPbapConnectSuccess(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T                wait_text = {0};
    BT_STATUS                   pull_ret = BT_ERROR;

    SCI_TRACE_LOW("[MMICL] HandleBTPbapConnectSuccess");

    /************************************************************************/
    /* Initiates the OBEX "Get" operation to retrieve a phonebook object from 
    the remote Phonebook Access Server. */
    /************************************************************************/
    pull_ret = PbapPullCalllog(s_cur_sel_log_list_type);
    //pull_ret = PbapPullCalllog(MMICL_BT_CALL_ALL);
    
    SCI_TRACE_LOW("[MMIAPICLBT] MSG_BT_PBAP_TP_CONNECT_SUCCESS pull_ret:%d", pull_ret);
    
    //Bug 944362 
    //lzk begin 2018.10.12
    {
        //receive PBAP CallLog Sync Signal. Must stop Pbap timer.
        SCI_TRACE_LOW("lzk log.HandleBTPbapConnectSuccess() call StopPdapTimer.");
        MMIAPICL_StopPbapTimer();
    }
    //lzk end 2018.10.12
    //Bug 944362 
    if(MMK_IsOpenWin(MMICL_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMICL_PBAP_CONNECT_WAIT_WIN_ID);
#ifndef WIN32
        MMIAPICL_StopPbapTimer();
#else
        MMICL_SimuPBAPDataToList(MMICL_GetCurCallType());
#endif
    }
    
    if (BT_PENDING != pull_ret)
    {
        if(BT_PBAP_TYPE_CL != MMIBT_GetPbapRequestType())
        {
        OpenPbapAlertTipWin(TXT_BT_PULL_PHONEBOOK_ERROR); 
        }
        return;
    }
    
    if(!MMK_IsOpenWin(MMICL_SYNC_WAIT_WIN_ID))
    {
        MMI_GetLabelTextByLang(TXT_BT_SYNC_WAITING, &wait_text); 
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMICL_SYNC_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinSyncWaitingMsg); 
        MMIPUB_SetWinSoftkey(MMICL_SYNC_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
    }
#ifdef WIN32
    MMIBT_SimuNotifyCLStartParse();
#endif
}
#endif

/*****************************************************************************/
// 	Description : to handle dialed call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallLogChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_DIALED_LIST_CTRL_ID;
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef  MMI_MULTI_GREEN_KEY_SINGLE
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifdef MMI_CL_TOOLBAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = GetLogToolBarCtrlId(win_id);
#endif
#endif
#endif
#ifdef MMI_CL_MINI_SUPPORT
    CC_CONTROL_ID_E list_id;
#endif

    list_ctrl_id = GetCtrlIDFromWinID(win_id);

	SCI_TRACE_LOW("[MMICL] HandleCallLogChildWindow msg_id = 0x%x", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
#ifdef MMI_CL_TOOLBAR_SUPPORT        
#ifndef MMI_PDA_SUPPORT        
#ifdef  TOUCH_PANEL_SUPPORT
            MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
#endif
            GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);

            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));

            GUILIST_SetItemHorMove(list_ctrl_id,TRUE);
            if (PNULL == arraycall_info)
            {
                arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            }

            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleCallLogChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4640_112_2_18_2_11_36_61,(uint8*)"");
                return recode;
            }

            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        
        break;
        
    case MSG_FULL_PAINT:
        MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);

		
        SetLogListItem();
#ifndef MMI_CL_MINI_SUPPORT		
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, GetMenuIDOffsetByCallType(s_cur_sel_log_list_type));
        }
#endif

#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, list_ctrl_id);
#endif        
#endif
        if(arraycall_info->record_num >0)
        {
            GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_NULL,STXT_RETURN,FALSE );
        }
        else
        {
#ifdef BT_PBAP_SUPPORT
            MMI_STRING_T			emtry_str_t={0};
            GUILIST_EMPTY_INFO_T	list_emty_info={0};
#endif
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL,STXT_RETURN, FALSE);
#ifdef BT_PBAP_SUPPORT
            if(IsBTCalllogWinId(win_id))
            {
                MMI_GetLabelTextByLang(TXT_DISCONNECTED, &emtry_str_t);
                list_emty_info.text_buffer = emtry_str_t;
                GUILIST_SetEmptyInfo(list_ctrl_id, &list_emty_info);
            }
#endif
        }    
#endif

        if (s_cur_sel_log_list_type ==MMICL_CALL_MISSED)
        {
            MMIAPICC_ClearMissedCallNum();
            MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
        }
#if defined(BT_DIALER_SUPPORT)
        if (MMICL_BT_CALL_MISSED == s_cur_sel_log_list_type
            || MMICL_BT_CALL_RECEIVED == s_cur_sel_log_list_type
            || MMICL_BT_CALL_DIALED == s_cur_sel_log_list_type
            )
        {
            if(arraycall_info->record_num == 0)
            {
                StartPbapCalllogSync(win_id);
            }
            else
            {
                if(FALSE == BT_GetState())
                {
                    MMIAPICL_DeleteAllPbapSyncData();
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    break;
                }
                else if(FALSE == MMIAPIBT_IsConnectDevice())
                {
                    MMIAPICL_DeleteAllPbapSyncData();
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    break;
                }
                
            }
        }
#endif
        break;
        
#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);

                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                DrawListItemIconData(arraycall_info,need_item_content_ptr);
            }
            break;
        }
#endif
        
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else

    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
         {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX); 
            
            if(s_is_allow_sim_call[dual_sys] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys]= FALSE;
         }
         break;
#endif

        //@samboo.shen 一键vt
    case MSG_APP_VIDEO_TEL:
        //SCI_TRACE_LOW:"kick the key VT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4743_112_2_18_2_11_36_62,(uint8*)"");
        if (GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;	
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
        if ((GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
            &&s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;  
#endif

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)    
        if (MMITHEME_IsIstyle())
        {
            if(PNULL == arraycall_info)
            {
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4927_112_2_18_2_11_37_63,(uint8*)"");
                return recode;
            }

            if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        else
#endif        
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            uint16 cur_sel = 0;

            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(item_content_ptr == PNULL)
            {
               break;
            }
            cur_sel = item_content_ptr->item_index;
            if(item_content_ptr->item_content_index == CL_LIST_ITEM_ICON_INDEX)    //Calltype ICON
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);
            }
        }
        break;
#endif

#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if (GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_TPMOVE_LEFT_SIDE:
        HandleAnswerSms();
        break;
    case MSG_NOTIFY_TPMOVE_RIGHT_SIDE:
        HandleRingBackByType(CC_CALL_NORMAL_CALL);
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT  
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if (GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0 )
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:        
    case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type);

#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,list_ctrl_id); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
             GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
             if (GUILIST_START_SLIDE == state)
             {
                GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
             }
             else if (GUILIST_END_SLIDE == state)
             {
                MMICL_UpdateCLWinToolbar(win_id,list_ctrl_id);
             }

        }
        break;
#endif        
#endif
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)    
        if (MMITHEME_IsIstyle())
        {
            if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)    
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);            
            }
        }
        else
#endif        
        
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
            MMI_NOTIFY_T notify = {0};
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif            
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleCallLogChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_4927_112_2_18_2_11_37_63,(uint8*)"");
                    return recode;
                }
   
                if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
           }
      }
      break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef MMI_CL_MINI_SUPPORT
 
    list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    if(s_select_info.is_select)
    {
        if(s_select_info.log_selected_index[GUILIST_GetCurItemIndex(list_id)])
            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), FALSE);
        else
            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), TRUE);

        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
    }
    else
    {
        if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
#ifdef MMI_LDN_SUPPORT
                if (!IsSimRecord(arraycall_info))
#endif
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
           
            }
        }
    }

#else
        if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
#ifdef MMI_LDN_SUPPORT
                if (!IsSimRecord(arraycall_info))
#endif
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
                
            }
        }
#endif
        break;

#ifdef BT_PBAP_SUPPORT
    case MSG_BT_CALLLOG_SYNC_CNF:
        HandleBTCallogSyncCfn(win_id);
        break;

    case MSG_BT_PBAP_TP_CONNECT_SUCCESS: 
        HandleBTPbapConnectSuccess(win_id);
        break;
#endif

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
#ifdef MMI_CL_MINI_SUPPORT
        ExitSelect();
#endif
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#if 0
/*****************************************************************************/
// 	Description : to handle dialed call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDialedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_DIALED_LIST_CTRL_ID;
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef  MMI_MULTI_GREEN_KEY_SINGLE
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_DIALED_CTRL_ID;
#endif
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        { 
#ifndef MMI_PDA_SUPPORT        
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);

        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));

        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleDialedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5030_112_2_18_2_11_37_64,(uint8*)"");
            return recode;
        }
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        s_cur_sel_log_list_type = MMICL_CALL_DIALED;
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_DIALED, arraycall_info);
#endif
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, (uint16)((uint32)ID_CL_DIALED_CALLS - (uint32)ID_CL_MISSED_CALLS));
            // GUIMENU_SetFirstAndSelectedItem(0, (uint16)((uint32)ID_CL_DIALED_CALLS - (uint32)ID_CL_MISSED_CALLS), MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_DIALED_LIST_CTRL_ID);
#endif
#endif
        break;
        
 #ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif
        
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else

    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
         {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX); 
            
            if(s_is_allow_sim_call[dual_sys] && GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys]= FALSE;
         }
         break;
#endif

        //@samboo.shen 一键vt
    case MSG_APP_VIDEO_TEL:
        //SCI_TRACE_LOW:"kick the key VT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5118_112_2_18_2_11_37_65,(uint8*)"");
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;	
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
        if ((GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
            &&s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;  
#endif

#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT  
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0 )
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:        
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_DIALED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
             GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
             if (GUILIST_START_SLIDE == state)
             {
                GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
             }
             else if (GUILIST_END_SLIDE == state)
             {
                MMICL_UpdateCLWinToolbar(win_id,MMICL_DIALED_LIST_CTRL_ID);
             }

        }
        break;
#endif
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            #ifndef MMI_PDA_SUPPORT
            #ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
            #endif
            #endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleDialedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5269_112_2_18_2_11_38_66,(uint8*)"");
                    return recode;
                }
   
                if(GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
           }
      }
      break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_DIALED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : to handle received call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReceivedChildWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_RECEIVED_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_RECEIVED_CTRL_ID;
#endif
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        #ifndef MMI_PDA_SUPPORT
        #ifdef TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
        #endif
        #endif
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleReceivedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5370_112_2_18_2_11_38_67,(uint8*)"");
            return recode;
        }
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_RECEIVED, arraycall_info);
#endif
        s_cur_sel_log_list_type = MMICL_CALL_RECEIVED;
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, (uint16)((uint32)ID_CL_RECEIVED_CALLS - (uint32)ID_CL_MISSED_CALLS));
            // GUIMENU_SetFirstAndSelectedItem(0, (uint16)((uint32)ID_CL_RECEIVED_CALLS - (uint32)ID_CL_MISSED_CALLS), MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_RECEIVED_LIST_CTRL_ID);
#endif
#endif
        break;

#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif

#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if ((GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)&&s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1] = FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            
            if(s_is_allow_sim_call[dual_sys] && (GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0))
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

        //@samboo.shen 一键vt
    case MSG_APP_VIDEO_TEL:  
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:    
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
        }
        break;
#endif
       
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif 

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:    
	case MSG_APP_MENU:

        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;

#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_RECEIVED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
            if (GUILIST_START_SLIDE == state)
            {
               GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
            }
            else if (GUILIST_END_SLIDE == state)
            {
               MMICL_UpdateCLWinToolbar(win_id,MMICL_RECEIVED_LIST_CTRL_ID);
            }
        }
       break;
#endif
#endif        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {   
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));       
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleReceivedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5605_112_2_18_2_11_38_68,(uint8*)"");
                    return recode;
                }
 
                if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);

                    }
                }
            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_RECEIVED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : to handle missed call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMissedChildWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_MISSED_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_MISSED_CTRL_ID;
#endif
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
        
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleMissedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5708_112_2_18_2_11_38_69,(uint8*)"");
            return recode;
        }
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_MISSED, arraycall_info);
#endif
        s_cur_sel_log_list_type = MMICL_CALL_MISSED;
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, 0);
            // GUIMENU_SetFirstAndSelectedItem(0, 0/*((uint32)ID_CL_MISSED_CALLS -ID_CL_MISSED_CALLS)*/, MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_MISSED_LIST_CTRL_ID);
#endif
#endif
        //关闭idle状态下提示位接电话的窗口以及状态栏图标
        MMIAPICC_ClearMissedCallNum();
        MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
        break;

#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif

#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && (GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1] = FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            
            if(s_is_allow_sim_call[dual_sys] && (GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif   
        
#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))        
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
        }
        break;
#endif
        
        //@samboo.shen 一键vt
    case MSG_APP_VIDEO_TEL:
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))   
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        }
        break;
        
    
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))   
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif 
    
    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if((GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0))   
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
	 }
        break;    
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_MISSED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
            if (GUILIST_START_SLIDE == state)
            {
               GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
            }
            else if (GUILIST_END_SLIDE == state)
            {
               MMICL_UpdateCLWinToolbar(win_id,MMICL_MISSED_LIST_CTRL_ID);
            }
        }
       break;
#endif
#endif        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleMissedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_5945_112_2_18_2_11_39_70,(uint8*)"");
                    return recode;
                }

                if(GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info); 
                    }
                }
            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
       
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_MISSED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRefusedChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_REFUSED_LIST_CTRL_ID;
    static BOOLEAN                  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif    
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T toolbars_ctrl_id = MMICL_TOOLBAR_REFUSED_CTRL_ID;
#endif
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
        MMICL_CreateToolbar(win_id,toolbars_ctrl_id,list_ctrl_id);
#endif
#endif
        
        GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"HandleRefusedChildWindow call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6049_112_2_18_2_11_39_71,(uint8*)"");
            return recode;
        }
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(MMICL_CALL_REFUSED, arraycall_info);
#endif
        s_cur_sel_log_list_type = MMICL_CALL_REFUSED;
        SetLogListItem();
        if (MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
        {
            GUIMENU_SetSelectedItem(MMICL_MAIN_MENU_CTRL_ID, (uint16)((uint32)ID_CL_REFUSED_CALLS - (uint32)ID_CL_MISSED_CALLS));
            // GUIMENU_SetFirstAndSelectedItem(0, (uint16)((uint32)ID_CL_REFUSED_CALLS - (uint32)ID_CL_MISSED_CALLS), MMICL_MAIN_MENU_CTRL_ID);
        }
#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
        MMICL_UpdateCLWinToolbar(win_id, MMICL_REFUSED_LIST_CTRL_ID);
#endif
#endif
        break;
     #ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
        #endif

#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:  
        if(s_is_allow_sim_call[MN_DUAL_SYS_1] && (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0))
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
            
            if(s_is_allow_sim_call[dual_sys] && (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0))            
            {
                HandleRingBack(sim_type, FALSE);
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:   
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)        
        {
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
        }
        break;
#endif
        
        //@samboo.shen 一键vt
    case MSG_APP_VIDEO_TEL:
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)    
        {
            HandleRingBack(CC_MAJOR_SIM_CALL,TRUE);
        }
        break;
        
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)  
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_REFUSE_LIST_MENU_WIN_TAB, PNULL);
            }
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        if (GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)  
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif        
#endif
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;
    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);    
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED);
#ifdef MMI_PDA_SUPPORT           
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:   
    case MSG_NOTIFY_LIST_MOVEDOWN:  
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:     
        {
            GUITOOLBAR_SetShowType(toolbars_ctrl_id, GUITOOLBAR_SHRINK);
            MMICL_UpdateCLWinToolbar(win_id,MMICL_REFUSED_LIST_CTRL_ID); 
        }
        break;
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
            if (GUILIST_START_SLIDE == state)
            {
               GUITOOLBAR_SetVisible(toolbars_ctrl_id, FALSE);
            }
            else if (GUILIST_END_SLIDE == state)
            {
               MMICL_UpdateCLWinToolbar(win_id,MMICL_REFUSED_LIST_CTRL_ID);
            }
        }
       break;
#endif
#endif        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            MMI_NOTIFY_T notify = {0};
            
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            
            if (toolbars_ctrl_id == notify.src_id)
            {
                CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(toolbars_ctrl_id, GUITOOLBAR_GetClickItemIndex(toolbars_ctrl_id));
                MMICL_HandleToolBarItemSel(node_id);
            }
            else
#endif
#endif
            {
                if(PNULL == arraycall_info)
                {
                    //SCI_TRACE_LOW:"HandleRefusedChildWindow MSG_CTL_PENOK arraycall_info is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6288_112_2_18_2_11_40_72,(uint8*)"");
                    return recode;
                }

                if(GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GetRecordNumByType(arraycall_info, MMICL_CALL_REFUSED) > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CL_ValidatePrivacyTabSwitch(BOOLEAN is_permitted, DPARAM param_ptr)
{
    if (is_permitted)
    {
        MMIPB_SetCLUnProtect(TRUE);
        //@@@ MMIAPIPB_SetCurAppType();
    }
    else
    {
        MMIAPIPB_SwitchToPreTab();
    }
}

/*****************************************************************************/
//  Description : 设置list滑动时的图片和字符串的回调函数
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetListSildeData(MMITHEME_LIST_CUSTDATA_T* data_ptr)
{
    if (PNULL == data_ptr)
    {
        return;
    }
/*
    data_ptr->data_type = GUILIST_TYPE_TEXT_ID;
    data_ptr->str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);            //item color
    data_ptr->str_style.font = MMI_LIST_DEFAULT_ITEM_FONT;
    data_ptr->str_style.align = ALIGN_HVMIDDLE;

    data_ptr->right_text_id = TXT_DIAL;
    data_ptr->left_text_id = TXT_SMS;
    data_ptr->left_icon_id = IMAGE_SMS_NOT_READ_SMS_ICON;
    data_ptr->right_icon_id = IMG_CC_ANSWER_ICON;*/
}

/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
//    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
    MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);
    uint8 call_log_num = 0;
    uint16      i=0;
    MMI_HANDLE_T      dropdownlist_ctr_handle;
    MMI_HANDLE_T      list_handle = 0;
    uint32  droplist_text_id[] =
            {
                TXT_CL_CALLS_ALL,
                TXT_CL_MISSED_CALLS,
                TXT_CL_RECEIVED_CALLS,
                TXT_CL_DIALED_CALLS,
#if defined(MMI_BLACKLIST_SUPPORT)
                TXT_CL_REFUSED_CALLS
#endif
            };
    uint32  droplist_type[]={
                MMICL_CALL_ALL,
                MMICL_CALL_MISSED,
                MMICL_CALL_RECEIVED,
                MMICL_CALL_DIALED,
#if defined(MMI_BLACKLIST_SUPPORT)
                MMICL_CALL_REFUSED
#endif
        };
    dropdownlist_ctr_handle =  MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID);
    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);

	SCI_TRACE_LOW("[MMICL] HandleAllChildWindow msg_id = 0x%x", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

 		

#ifdef MMI_PDA_SUPPORT 
        {
        MMI_HANDLE_T  main_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_FORM_CTRL_ID);
        MMI_HANDLE_T  droplist_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_FORM_CTRL_ID);
        GUIFORM_CHILD_HEIGHT_T              height_info = {0}; 
   
        if (!InitPbCalllogData(info_ptr))
        {
            MMK_CloseParentWin(MMK_GetParentWinHandle(win_id));
            break;
        }

        GUIFORM_IsSlide(main_form_handle,FALSE);
        GUIFORM_PermitChildBorder(main_form_handle, FALSE);
        GUILIST_PermitBorder(list_handle, FALSE);
       // GUILIST_SetUserBg(list_handle, FALSE);
        GUIFORM_PermitChildBg(main_form_handle,FALSE);

        height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
    	GUIFORM_SetChildHeight(main_form_handle, list_handle, &height_info);

        GUIFORM_PermitChildBorder(droplist_form_handle, TRUE);

        GUIDROPDOWNLIST_SetMaxItem(dropdownlist_ctr_handle,5);
        for(i=0; i<ARR_SIZE(droplist_text_id); i++) 
            {
                GUIDROPDOWNLIST_AppendItemById(dropdownlist_ctr_handle, droplist_text_id[i]);
            }
#ifdef MMI_CL_TOOLBAR_SUPPORT
        MMICL_CreateToolbar(win_id,MMICL_TOOLBAR_ALLLIST_CTRL_ID,list_ctrl_id);
#endif
        GUIDROPDOWNLIST_SetCurItemIndex(dropdownlist_ctr_handle,0);
        SetAllLogFillter(droplist_type[0]);
        GUIDROPDOWNLIST_SetButton(win_id, dropdownlist_ctr_handle, TRUE);

        }

        GUILIST_SetItemHorMove(list_handle, TRUE);
        // GUILIST_RegGetDataFunc(list_handle,SetListSildeData);

        CalllogPBData_SetListHandle( MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
#if defined MMI_ISTYLE_SUPPORT        
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
#endif
            //GUIAPICTRL_SetBothRect(list_ctrl_id,&list_both_rect);
            
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6502_112_2_18_2_11_40_73,(uint8*)"");
                return recode;
            }
            MMK_SetAtvCtrl(win_id, list_handle);
#ifdef MMI_PDA_SUPPORT
        }
#endif
        break;
        
    case MSG_FULL_PAINT:
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            break;
        }
        //关闭idle状态下提示位接电话的窗口以及状态栏图标
        MMIAPICC_ClearMissedCallNum();
#ifdef MMI_PDA_SUPPORT 
#if defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
#endif
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
#ifdef MMI_PDA_SUPPORT
        }
#endif

        break;

#ifdef MMI_PDA_SUPPORT
    /*add for istyle*/

     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif


#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
            }
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);

            if(s_is_allow_sim_call[dual_sys])
            {
                if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
                {
                    HandleRingBack(sim_type, FALSE);
                }
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
    case MSG_APP_VIDEO_TEL:        
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
#if 1//OPEN_VT_CHANGE
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
#else
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
#endif
        }
        break;
#endif
        

        
#ifdef MMI_PDA_SUPPORT    //to do open list menu

    case MSG_CTL_DROPDOWNLIST_SELECTED:
        {
        uint16 dropdownlist_index = 0;
        dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
        SetAllLogFillter(droplist_type[dropdownlist_index]);
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
        }
        break;
    case MSG_CTL_LIST_LONGOK:
		{
            MMI_NOTIFY_T notify = {0};
            // If not list long pen ok
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            if (notify.src_handle !=list_handle)
            {
                break;
            }
		}
        call_log_num = GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type);

        if( call_log_num > 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
#ifdef MMI_GPRS_SUPPORT
            else
            {
                MMK_CreateWin((uint32 *)MMICL_ALL_LIST_POP_MENU_WIN_TAB, (ADD_DATA)info_ptr);
            }
#endif
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        if (g_pbcalllog_data ==PNULL)
        {
            if(!InitPbCalllogData(info_ptr))
            {
                break;
            }
        }
        {
        uint16 dropdownlist_index = 0;
        dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
        SetAllLogFillter(droplist_type[dropdownlist_index]);
        }

        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
#if defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
#endif		
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_handle);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }

        GUIWIN_SetTitleIsVisible(win_id, FALSE, FALSE);
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;
#endif
    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;

    case MSG_CTL_TAB_SWITCH:
        //隐私检查
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //需要进行隐私检查
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        {
            uint8 record_num = GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type);

#ifdef MMI_PDA_SUPPORT
#ifdef MMI_CALLLOG_HIDE_MAINMENU
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            if (0 == record_num )
            {  
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENUKEY_CTRL_ID, MENU_CL_MEMU_KEY, ID_CL_DELETE_ALL, FALSE);
            }
#else
            if (0 != record_num)
            {       
                MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENUKEY_WIN_TAB, PNULL);
            }
#endif //MMI_CALLLOG_HIDE_MAINMENU
#else
            if (0 != record_num)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN_TAB, PNULL);   //rong.gu to do
                }
            }
#endif //MMI_PDA_SUPPORT
        }
        break;
        
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined MMI_ISTYLE_SUPPORT		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else
#endif			
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            uint16 cur_sel = 0;

            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(item_content_ptr == PNULL)
            {
               break;
            }
            cur_sel = item_content_ptr->item_index;
            if(item_content_ptr->item_content_index == CL_LIST_ITEM_TYPE_INDEX)    //Calltype ICON
            {
#ifdef MMI_CL_TOOLBAR_SUPPORT
                MMI_HANDLE_T toolbar_ctrl_handle;//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICL_TOOLBAR_ALLLIST_CTRL_ID);//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
                MMICL_UpdateCLWinToolbar(win_id,MMICL_ALL_LIST_CTRL_ID);
#endif                
            }
            else if(item_content_ptr->item_content_index == CL_LIST_ITEM_ICON_INDEX)
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);
            }
        }       

        break; 
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
#if defined MMI_ISTYLE_SUPPORT        
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
                
            {
#ifdef MMI_CL_TOOLBAR_SUPPORT
                MMI_NOTIFY_T notify = {0};
                MMI_HANDLE_T hToolbar =  MMK_GetCtrlHandleByWin(win_id,MMICL_TOOLBAR_ALLLIST_CTRL_ID); 

                SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
                
                if (MMICL_TOOLBAR_ALLLIST_CTRL_ID == notify.src_id)
                {
                    CL_TOOLBAR_TYPE_E node_id = CL_TOOLBAR_TYPE_INIT;
                    node_id = (CL_TOOLBAR_TYPE_E)GUITOOLBAR_GetItemUserDate(hToolbar, GUITOOLBAR_GetClickItemIndex(hToolbar));
                    MMICL_HandleToolBarItemSel(node_id);
                    break;
                }
                else
                {
                    GUITOOLBAR_SetVisible(hToolbar,FALSE);
                }
#endif                
            }

        
#endif
            {
                if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
                {
                    if (s_select_return_info.need_select_return)
                    {
                        if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                        {
                            MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                        }
                        MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                    }
                    else
                    {
                        CL_OpenCallTimeListOrDetailWin(arraycall_info);
                    }
                }
            }
            break;
#ifdef MMI_PDA_SUPPORT 
        }
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TPMOVE_LEFT_SIDE:
        HandleAnswerSms();
        break;
    case MSG_NOTIFY_TPMOVE_RIGHT_SIDE:
        HandleRingBackByType(CC_CALL_NORMAL_CALL);
        break;
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            if (s_select_return_info.need_select_return)
            {
                if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                {
                    MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                }
                MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
            }
            else
            {
                CL_OpenCallTimeListOrDetailWin(arraycall_info);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(info_ptr); 
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }

        FreePbCalllogData();
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
#endif
        break;
        
    default:
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleAllChildWindow(win_id, msg_id, param);
        }
#endif
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;

}

/*****************************************************************************/
// 	Description : to handle select contact from call log child window message
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectAllChildWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_WIN_ID_T info_ptr = 0;
    uint16      i=0;
    MMI_HANDLE_T      dropdownlist_ctr_handle;
    MMI_HANDLE_T      list_handle = 0;
    uint32  droplist_text_id[] ={
                TXT_CL_CALLS_ALL,
                TXT_CL_MISSED_CALLS,
                TXT_CL_RECEIVED_CALLS,
                TXT_CL_DIALED_CALLS,
#if defined(MMI_BLACKLIST_SUPPORT)
                TXT_CL_REFUSED_CALLS
#endif
            };
    uint32  droplist_type[]={
                MMICL_CALL_ALL,
                MMICL_CALL_MISSED,
                MMICL_CALL_RECEIVED,
                MMICL_CALL_DIALED,
#if defined(MMI_BLACKLIST_SUPPORT)
                MMICL_CALL_REFUSED
#endif
            };
	GUILIST_MARK_ITEM_T *list_param_ptr = PNULL;
    MMI_HANDLE_T main_form_handle       = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_FORM_CTRL_ID);
    uint16 max_selected_num = 0;

    info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);
    dropdownlist_ctr_handle =  MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_DROPLIST_CTRL_ID);
    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        else
        {
            SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        {
            MMI_HANDLE_T  droplist_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LOG_LIST_FORM_CTRL_ID);
            GUIFORM_CHILD_HEIGHT_T              height_info = {0}; 
       
            if (!InitPbCalllogData(info_ptr))
            {
                MMK_CloseParentWin(MMK_GetParentWinHandle(win_id));
                break;
            }
            GUIFORM_IsSlide(main_form_handle,FALSE);
            GUIFORM_PermitChildBorder(main_form_handle, FALSE);
            GUILIST_PermitBorder(list_handle, FALSE);
            GUIFORM_PermitChildBg(main_form_handle,FALSE);
            height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
        	GUIFORM_SetChildHeight(main_form_handle, list_handle, &height_info);
            GUIFORM_PermitChildBorder(droplist_form_handle, TRUE);
            GUIDROPDOWNLIST_SetMaxItem(dropdownlist_ctr_handle,5);
            for(i=0; i<ARR_SIZE(droplist_text_id); i++) 
                {
                    GUIDROPDOWNLIST_AppendItemById(dropdownlist_ctr_handle, droplist_text_id[i]);
                }
#ifdef MMI_CL_TOOLBAR_SUPPORT
            MMICL_CreateToolbar(win_id,MMICL_TOOLBAR_ALLLIST_CTRL_ID,list_ctrl_id);
#endif            
            GUIDROPDOWNLIST_SetCurItemIndex(dropdownlist_ctr_handle,0);
            SetAllLogFillter(droplist_type[0]);
            GUIDROPDOWNLIST_SetButton(win_id, dropdownlist_ctr_handle, TRUE);

        }
        CalllogPBData_SetListHandle( MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        SCI_MEMSET(&s_call_log_all_select_info, 0x00, sizeof(MMICL_ALL_SELECT_INFO_T));
        max_selected_num = MMISMS_DEST_ADDR_MAX_NUM - MMIAPISMS_GetDestCount();
        GUILIST_SetMaxSelectedItem(MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LIST_CTRL_ID), max_selected_num);
        if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())	
        {
			MMICC_CreateNewSoftKey(win_id, main_form_handle, STXT_ADD, TXT_NULL, STXT_CANCEL);
            GUISOFTKEY_SetButtonGray(win_id, MMICALLLOG_SOFTKEY_CTRL_ID,LEFT_BUTTON, TRUE);			
        }
#if defined MMI_ISTYLE_SUPPORT		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
        else   
#endif			
        {
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleSelectAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_6980_112_2_18_2_11_41_74,(uint8*)"");
                return recode;
            }
            MMK_SetAtvCtrl(win_id, list_handle);

        }
        break;
        
    case MSG_FULL_PAINT:
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            break;
        }
        //关闭idle状态下提示位接电话的窗口以及状态栏图标
        MMIAPICC_ClearMissedCallNum(); 
        MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
        s_cur_sel_log_list_type = MMICL_CALL_ALL;
        SetLogSelectListItem();
        break;
        
    //to do open list menu
    case MSG_CTL_DROPDOWNLIST_SELECTED:
        {
            uint16 dropdownlist_index = 0;
            dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
            SetAllLogFillter(droplist_type[dropdownlist_index]);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
            SCI_MEMSET(&s_call_log_all_select_info, 0x00, sizeof(MMICL_ALL_SELECT_INFO_T));
            GUILIST_SetAllSelected(MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LIST_CTRL_ID), FALSE);
        }
        break;

    case MSG_GET_FOCUS:
        {
            uint16 dropdownlist_index = 0;
            dropdownlist_index = GUIDROPDOWNLIST_GetCurItemIndex(dropdownlist_ctr_handle);
            SetAllLogFillter(droplist_type[dropdownlist_index]);
        }

        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
#endif		
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_handle);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }

        GUIWIN_SetTitleIsVisible(win_id, FALSE, FALSE);
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;

    case MSG_CTL_TAB_SWITCH:
        SCI_MEMSET(&s_call_log_all_select_info, 0x00, sizeof(MMICL_ALL_SELECT_INFO_T));
        GUILIST_SetAllSelected(MMK_GetCtrlHandleByWin(win_id, MMICL_ALL_LIST_CTRL_ID), FALSE);
        //隐私检查
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //需要进行隐私检查
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_SendMsg(
            MMK_GetPrevWinId(MMK_GetParentWinHandle(win_id)), 
            MMI_CL_MULTI_SELECT_LIST_RETURN, 
            PNULL
            );
        MMK_CloseWin(MMK_GetParentWinHandle(win_id));
        break;

	case MSG_CTL_LIST_MARK_ITEM:
        list_param_ptr = (GUILIST_MARK_ITEM_T*)param;
		if(list_param_ptr == PNULL)
		{
			break;
		}
        if(list_param_ptr->had_maked)
        {
            if (!s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid)
            {
                s_call_log_all_select_info.sel_count ++; 
                s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid = TRUE;
            }
        }
        else
        {
            if (s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid)
            {
                if(s_call_log_all_select_info.sel_count > 0) s_call_log_all_select_info.sel_count --; 
                s_call_log_all_select_info.sel_item[list_param_ptr->item_idx].is_valid = FALSE;
            }
        }
        if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())	
        {
            if(s_call_log_all_select_info.sel_count >0)
            {
                GUISOFTKEY_SetButtonGray(win_id, MMICALLLOG_SOFTKEY_CTRL_ID,LEFT_BUTTON, FALSE);
            }
            else
            {
                GUISOFTKEY_SetButtonGray(win_id, MMICALLLOG_SOFTKEY_CTRL_ID,LEFT_BUTTON, TRUE);
            }
        }    
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
        else
#endif			
        {					
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_SendMsg(
        			MMK_GetPrevWinId(MMK_GetParentWinHandle(win_id)),
        			MMI_CL_MULTI_SELECT_LIST_RETURN, 
        			PNULL
        			);
                MMK_CloseWin(MMK_GetParentWinHandle(win_id));
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            MMK_CloseWin(MMK_GetParentWinHandle(win_id));
        }
        else
#endif			
        {
            MMK_CloseParentWin(info_ptr); 
        }
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        FreePbCalllogData();
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
#endif		
        break;
        
    default:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandleSelectAllChildWindow(win_id, msg_id, param);
        }
#endif		
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
//	Description : the handle function of call list delete menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifdef MMI_CL_MINI_SUPPORT
        if(s_select_info.is_select)
        {
            DeleteRecord();
        }
        else
        {
            DeleteCurrentRecord();
        }
#else
        DeleteCurrentRecord(); 
#endif			
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        DeleteAllCallLog(s_cur_sel_log_list_type);
        
        //success!
        // close log list window
        //MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
        //close the window of call log list menu and detail win
        MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
        MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
        MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

#ifdef MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E  HandleLogListMenukeyWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    MMK_SetAtvCtrl(win_id, MMICL_LOG_LIST_MENUKEY_CTRL_ID);
    break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MMICL_LOG_LIST_MENUKEY_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
            case ID_CL_DELETE_ALL:
                           //询问是否删除!! 
                MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
                MMK_CloseWin(win_id);
                break;
#ifdef     MMI_CALLLOG_HIDE_MAINMENU
#ifdef ASP_SUPPORT            
            case ID_CL_ASP:
                {
                    MMIAPIASP_EnterAsp();
                    MMK_CloseWin(win_id);
                    break;
                }
#endif  
            case ID_CL_CALL_TIMERS:
                {            
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                    uint16 sim_card_ok = 0 ;
                    uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);
                    
                    if( 0 == sim_card_ok_num )
                    {
                        MMIAPIPHONE_AlertSimNorOKStatus();
                    }
                    else if ( 1 == sim_card_ok_num )
                    {
                        MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                    }            
                    else
                    {
                        MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                    }
#else
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
#endif
                MMK_CloseWin(win_id);
                }
                break;
                
            case ID_CL_GPRS_FLOWRATE:
#ifdef MMI_GPRS_SUPPORT
                MMIAPICL_OpenGprsFlowRateWin();
                MMK_CloseWin(win_id);
#endif
                break;
                
#endif

            default:
                break;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_Istyle_HandleLogListDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        DeleteAllCallLog(MMICL_CALL_ALL);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : insert dyna menu node by label
//	Global resource dependence : 
//  Author:rong.gu
//	Note:
/*****************************************************************************/
LOCAL void InsertContextMenuNode(
                        	     MMI_CTRL_ID_T      ctrl_id,        //控件id
                        	     MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                        	     uint16          	node_id,        //节点编号
                        	     uint16          	parent_node_id, //父节点编号
                                     uint16                index
                                 )
{
    MMI_STRING_T            string = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &string);
    node_item.item_text_ptr = &string;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}


#endif


#ifdef MMI_PDA_SUPPORT
LOCAL void  MMICL_SetLogListDetailMenuTitle(void)
 {
    GUIMENU_TITLE_INFO_T title_info = {0};
    MMI_STRING_T title_str = {0}; 
    MMI_STRING_T    string = {0};
    uint8 temp_str[PHONE_NUM_MAX_LEN] = {0};
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 disp_str[CL_SAVE_MAX_LEN] = {0};
    uint16 disp_num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;        

    wchar wchar_disp_str[CL_SAVE_MAX_LEN] = {0};

    MMI_GetLabelTextByLang(TXT_CALL, &title_str);
    title_info.is_static = TRUE;
    title_info.title_ptr = &title_str;
    title_info.sub_title_ptr = PNULL;
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    title_info.group_id = MENU_CL_DIAL_SUBMENU_OPT;
#else
    title_info.group_id = MENU_CL_LOG_LIST_OPT;
#endif
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);

    MMI_GetLabelTextByLang(TXT_PHONE_NUMBER, &string);

    MMIAPICOM_WstrToStr(string.wstr_ptr,temp_str);
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);

    disp_num_len= sprintf((char *)disp_str, "%s %s",temp_str, tele_num);
    MMI_STRNTOWSTR(wchar_disp_str, disp_num_len, (const uint8 *)disp_str, disp_num_len, disp_num_len);

    title_info.title_ptr->wstr_ptr = wchar_disp_str;
    title_info.title_ptr->wstr_len = disp_num_len;
    title_info.group_id = MENU_CL_ADD_TO_PB;
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);

    SCI_MEMSET(tele_num,0x00,sizeof(tele_num));
    SCI_MEMSET(wchar_disp_str,0x00,sizeof(wchar_disp_str));
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    MMI_STRNTOWSTR(wchar_disp_str, PHONE_NUM_MAX_LEN + 2, (const uint8 *)tele_num, PHONE_NUM_MAX_LEN + 2,  (uint16)strlen( (char *)tele_num ));
    title_info.title_ptr->wstr_ptr = wchar_disp_str;
    title_info.title_ptr->wstr_len = MMIAPICOM_Wstrlen(wchar_disp_str);
    title_info.group_id = MENU_CL_LOG_LIST_OPT;
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);

#if defined(MMI_BLACKLIST_SUPPORT)
    //Sam@Dec06 CR0144206 should set Refuse menu title here
    title_info.group_id = MENU_CL_REFUSE_OPT;	
    GUIMENU_SetMenuTitleEx(MMICL_LOG_LIST_MENU_CTRL_ID, &title_info);
#endif 
 }
#endif

/*****************************************************************************/
// 	Description : to handle the call log list menu window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;
    //MMI_CTRL_ID_T ctrl_list_id;
#ifdef MMI_CL_MINI_SUPPORT
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
#endif
#ifdef BT_DIALER_SUPPORT
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    uint8 record_num = 0;
#endif
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

#ifdef MMI_PDA_SUPPORT 
        MMICL_SetLogListDetailMenuTitle();
#endif            
        MMK_SetAtvCtrl(win_id, MMICL_LOG_LIST_MENU_CTRL_ID);
        {
            //cr99896
            if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
            {
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_VIDEOCALL,TRUE);
            }

            if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)&&MMIAPICC_IsVideoCall())	//@samboo vt通话时无法发起新的呼叫 20071127
            {
                #ifdef    MMI_PDA_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_VIDEOCALL,TRUE);
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_AUDIOCALL,TRUE);
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_DIAL_IPCALL,TRUE);                  
                #else
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
#ifdef VT_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_VIDEOCALL,TRUE);
#endif
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_AUDIOCALL,TRUE);
#ifdef MMI_IP_CALL_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_IPCALL,TRUE);
#endif
#endif
                #endif
            }

            if(MMIAPIUDISK_UdiskIsRun())
            {
                #ifdef  MMI_PDA_SUPPORT
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_SEND_MMS,TRUE);        
                #else
                //grayed mms menu
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_SEND_OPT,ID_CL_SEND_MMS,TRUE);                
                #endif
            }
#ifdef MMI_BLACKLIST_SUPPORT
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);
                
                if (MMIAPISET_IsCCBlacklist(&tele_num,MN_CALL_TYPE_NORMAL))
                {
                
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT2 , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);                    
                }
                else
                {
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT2 , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);                    
                }
            }    
#endif
#ifdef BT_DIALER_SUPPORT
			//LOCAL SIM CALL option is forbidden under bluetooth mode
			//REMOTE BT CALL option is forbidden if no device connected
			if(MMIAPIBT_IsBluetoothMode())
			{
				GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_SIMCALL,TRUE);
			}
            if(!MMIAPIBT_IsConnectDevice() || MMICC_IsExistActiveCall() || MMICC_IsExistHoldCall())
            {
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_DIAL_SUBMENU_OPT,ID_CL_DIAL_BTCALL,TRUE);
                GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_SYNC, TRUE);             // [EH]                  
            }
            if (MMICL_BT_CALL_MISSED == s_cur_sel_log_list_type
                || MMICL_BT_CALL_RECEIVED == s_cur_sel_log_list_type
                || MMICL_BT_CALL_DIALED == s_cur_sel_log_list_type
                )
            {
                arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
                if(PNULL == arraycall_info)
                {
                    SCI_TRACE_LOW("GetCurrentPhoneNum call info alloc failed",(uint8*)"");
                    return recode;
                }
                
                SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
                record_num = GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type);

                if (record_num == 0)
                {
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_DETAIL, FALSE);  
#if defined(MMI_MULTI_SIM_SYS_NONE) && defined(BT_DIALER_SUPPORT)
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_DIAL_BTCALL, FALSE);  
#elif defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT) || defined(BT_DIALER_SUPPORT)
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_DIAL, FALSE);  
#else
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_DIAL_AUDIOCALL, FALSE);  
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_SEND_SMS, FALSE);  
#else
#ifdef MMS_SUPPORT
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_SEND, FALSE);  
#else
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_SEND_SMS, FALSE);  
#endif
#endif
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_SAVE, FALSE);  
#if defined(MMI_BLACKLIST_SUPPORT)
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);  
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);  
#endif                  
#ifdef MMI_CL_MINI_SUPPORT
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_MARK, FALSE);  
#endif
                    
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_LIST_DELETE, FALSE);  
                    GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_DELETE_ALL, FALSE);  
                }
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_SYNC, TRUE);  
            }
            else
            {
                GUIMENU_SetItemVisible(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT , ID_CL_LOG_SYNC, FALSE);  
            }
#endif
        }
#ifdef MMI_CL_MINI_SUPPORT
        
        MMICL_LogListMenuGrayed(win_id, MMICL_LOG_LIST_MENU_CTRL_ID);

#endif
		break;
        
    case MSG_GET_FOCUS:
        if(MMIAPIUDISK_UdiskIsRun())
        {
            #ifdef  MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_SEND_MMS,TRUE);
            #else
                    //grayed mms menu
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_SEND_OPT,ID_CL_SEND_MMS,TRUE);
            #endif
        }
        else
        {
            #ifdef  MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_LOG_LIST_OPT,ID_CL_SEND_MMS,FALSE);
            #else
                  //grayed mms menu
            GUIMENU_SetItemGrayed(MMICL_LOG_LIST_MENU_CTRL_ID,MENU_CL_SEND_OPT,ID_CL_SEND_MMS,FALSE);
            #endif
        }
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        GUIMENU_GetId(MMICL_LOG_LIST_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
#ifdef BT_DIALER_SUPPORT
        case ID_CL_LOG_SYNC:
            {
                switch (s_cur_sel_log_list_type)
                {
                case MMICL_BT_CALL_MISSED:
                    DeleteAllCallLog(MMICL_BT_CALL_MISSED);
                    SetPbapAllowSync(MMICL_BT_CALL_MISSED, TRUE);
                    StartPbapCalllogSync(MMICL_BT_MISSED_CHILD_WIN_ID);
                    MMK_CloseWin(win_id);
                    break;
                    
                case MMICL_BT_CALL_RECEIVED:
                    DeleteAllCallLog(MMICL_BT_CALL_RECEIVED);
                    SetPbapAllowSync(MMICL_BT_CALL_RECEIVED, TRUE);
                    StartPbapCalllogSync(MMICL_BT_RECEIVED_CHILD_WIN_ID);
                    MMK_CloseWin(win_id);
                    break;
                    
                case MMICL_BT_CALL_DIALED:
                    DeleteAllCallLog(MMICL_BT_CALL_DIALED);
                    SetPbapAllowSync(MMICL_BT_CALL_DIALED, TRUE);
                    StartPbapCalllogSync(MMICL_BT_DIALED_CHILD_WIN_ID);
                    MMK_CloseWin(win_id);
                    break;
                    
                default:
                    break;
                }
            }
            break;
#endif
        case ID_CL_LOG_DETAIL:
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
            break;
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
        case ID_CL_LOG_LIST_DELETE_MULTI:
            MMK_CreateWin((uint32 *)MMICL_CHILD_SELECT_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);

            break;
#endif
        case ID_CL_ADD_TO_PB:
            HandleDistillPhoneNum();
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_LOG_LIST_DELETE:
            //询问是否删除!! 
#ifdef CALL_TIMES_SUPPORT            
            if(MMK_IsOpenWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
                /*||MMK_IsOpenWin(MMICL_CALL_TIMES_WIN_ID)*/)
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            }
            else
#endif    
            {
                //MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteQueryWinMsg);
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteQueryWinMsg, PNULL);
                MMK_CloseWin(win_id);
            }
            break;
            
 #ifdef VT_SUPPORT
       case ID_CL_DIAL_VIDEOCALL://@zhaohui add
            HandleRingBack(CC_CALL_SIM_MAX, TRUE);
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_CL_DIAL_AUDIOCALL:
            HandleRingBack(CC_CALL_SIM_MAX, FALSE);
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_IP_CALL_SUPPORT
        case ID_CL_DIAL_IPCALL:
            HandleIpCall();
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT            
        case ID_CL_ADD_TO_EXIST_CONTACT:
            HandleAddNumToName();
            MMK_CloseWin(win_id);
            break;
#endif            
        case ID_CL_SEND_SMS:
            HandleAnswerSms();
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_SEND_MMS:
            HandleAnswerMms();
            MMK_CloseWin(win_id);
            break;
            
#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_LOG_LIST_REFUSE_ADD:
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);
                AddNumToBlackList(tele_num);
                MMK_CloseWin(win_id);
            }
            break;
        case ID_CL_LOG_LIST_REFUSE_REMOVE:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    if (MMIAPISET_DeleteBlacklist(tele_num))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);                    
                    }    
                    MMK_CloseWin(win_id);
                }
            break;
#endif


        case ID_CL_DELETE_ALL:
            //询问是否删除!! 
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_CALL_TIMERS:
            {                
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint16 sim_card_ok = 0 ;
                uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);

                if( 0 == sim_card_ok_num )
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if ( 1 == sim_card_ok_num )
                {
                    MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)sim_card_ok);       
                }            
                else
                {
                MMICL_SelectSIMFunc(CallTimeSelectSimCallback,MMISET_APPEND_SIM_OK,PNULL);
                }               
#else
                MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, PNULL);
#endif
            }
            break;
            
        case ID_CL_GPRS_FLOWRATE:
#ifdef MMI_GPRS_SUPPORT
            MMIAPICL_OpenGprsFlowRateWin();
#endif
            break;
            
#ifdef AOC_SUPPORT
        case ID_CL_COSTS_TOTAL:
        case ID_CL_COSTS_MAX:
        case ID_CL_COSTS_RATE:
        case ID_CL_COSTS_RESET:
            MMICL_COSTS_HandleMainMenuMsg(menu_id);
            break;
#endif
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
        case ID_CL_MARK_LIST:

            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), TRUE);
            s_select_info.is_select = TRUE;
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_CANCEL_MARK_LIST:
            
            SetCurrentSelect(GUILIST_GetCurItemIndex(list_id), FALSE);

            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_MARKALL_LIST:
            
            s_select_info.is_select = TRUE;
            SetAllSelect(TRUE);
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_CANCLE_MARKALL_LIST:
            
            s_select_info.is_select = FALSE;
            SetAllSelect(FALSE);
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef BT_DIALER_SUPPORT
		case ID_CL_DIAL_SIMCALL:
			HandleRingBack(CC_CALL_SIM_MAX, FALSE);
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
			break;

		case ID_CL_DIAL_BTCALL:
			HandleBtCall();
            MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
			break;
#endif

        default:
            break;
        }
        break;

    #ifdef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_APP_OK:
    case MSG_CTL_OK:               
    #endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        //关闭记录的menu窗口和询问窗口
        GUIMENU_GetId(MMICL_LOG_LIST_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_SEND_MMS:
            MMIPUB_CloseQuerytWin(&query_win_id);
            HandleAnswerMms();
            break;
            
        case ID_CL_LOG_LIST_DELETE:
            //询问是否删除!! 
#ifdef CALL_TIMES_SUPPORT            
            if(MMK_IsOpenWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
                /*||MMK_IsOpenWin(MMICL_CALL_TIMES_WIN_ID)*/)
            {
                uint32 select_index = (uint32)MMK_GetWinAddDataPtr(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
                    
                MMIPUB_CloseQuerytWin(&query_win_id);
                /*DeleteOneCallTimesItem((uint16)select_index);*/
                MMICL_DeleteOneCallTimesItem(s_cur_sel_log_list_type,select_index);
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            }
#endif 
            break;
   
        default:
            break;
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add the num to blacklist   
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void AddNumToBlackList(
                        MMIPB_BCD_NUMBER_T  pb_bcd    //IN:
                        )
{
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
    MMI_STRING_T mmi_pb_name ={0};

    num_ton = pb_bcd.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == pb_bcd.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    if ((pb_bcd.number_len > 0) || is_plus_symbol)
    {
        mmi_pb_name.wstr_ptr = SCI_ALLOCA((PHONE_NUM_MAX_LEN + 1)*sizeof(wchar));
        if (PNULL != mmi_pb_name.wstr_ptr)
        {
            SCI_MEMSET(mmi_pb_name.wstr_ptr, 0x00, sizeof(wchar)*(PHONE_NUM_MAX_LEN + 1));
        }
        else
        {
            return;
        }
        MMIAPIPB_GetNameByNumber(&pb_bcd,
                                  &mmi_pb_name,
                                  PHONE_NUM_MAX_LEN,
                                  FALSE  //use default name,"no name"
                                  );
        if (MMIAPISET_AddBlacklist(mmi_pb_name, pb_bcd))
        {
#ifndef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif            
        }
        SCI_FREE(mmi_pb_name.wstr_ptr);
    }
    else
    {
        //提示窗口没有号码
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

/*****************************************************************************/
// 	Description : set detail of call log list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListDetail(void)
{
    uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    wchar   tele_num_wstr[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8   time[MMICL_DISPLAY_TIME_LEN + 1 ] = {0};
    wchar   time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    uint8   date[MMICL_DISPLAY_DATE_LEN + 1] = {0};
    wchar   date_wstr[MMICL_DISPLAY_DATE_LEN + 1] = {0};
    uint8   duration_time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar   duration_time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    uint16  sel_item_index = 0;
    MMI_TM_T    tm = {0};
    MMI_STRING_T    name_str = {0};
    MMI_STRING_T    num_str = {0};
    MMI_STRING_T    time_str = {0};
    MMI_STRING_T    date_str = {0};
    MMI_STRING_T    duration_time_str = {0};
    MMI_STRING_T    call_type_str = {0};
    MMIPB_BCD_NUMBER_T  bcd_number = {0};
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T    sim_num = {0};
    MMISET_SIM_NAME_T sim_name = {0};
#endif
    MMI_STRING_T    location_str = {0};
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    uint16  city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
    uint16  str_length = 0;
    uint8   origin_number_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    int16   origin_num_len  = 0;
#endif

    uint16 index = 0;
    MMI_CTRL_ID_T ctrl_id = MMICL_LOG_LIST_DETAIL_CTRL_ID;
    
    //initialize
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListDetail call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_7798_112_2_18_2_11_43_75,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(&tm,0,sizeof(tm));
    SCI_MEMSET(&name_str,0,sizeof(name_str));
    name_str.wstr_ptr = (wchar *)SCI_ALLOC_APP( ( MMIPB_MAX_STRING_LEN + 1 ) * sizeof(wchar) );
    SCI_MEMSET(&num_str,0,sizeof(num_str));
    SCI_MEMSET(&time_str,0,sizeof(time_str));
    SCI_MEMSET(&date_str,0,sizeof(date_str));
    SCI_MEMSET(&bcd_number,0,sizeof(bcd_number));

    GetCurrentRecordInfo(arraycall_info, &sel_item_index);

    //get name
    if (!arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//电话号码不存在
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_UNKNOW_NUM,&num_str);//空号
    }
    else
    {
        //get number
        MMICL_GenDispNumber(arraycall_info->call_bcdinfo[sel_item_index].call_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_len,
            arraycall_info->call_bcdinfo[sel_item_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
        num_str.wstr_len = (uint16)strlen((char*)tele_num);
        num_str.wstr_ptr = tele_num_wstr;//(uint8 *)tele_num;
        MMI_STRNTOWSTR( tele_num_wstr, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, num_str.wstr_len, num_str.wstr_len );

        MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_str, FALSE);
    }
    
    //get time and date
#ifdef CALL_TIMES_SUPPORT
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_start_time[0]);
#else
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_start_time);
#endif
    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,time,MMICL_DISPLAY_TIME_LEN + 1 );
    time_str.wstr_ptr = time_wstr;//(uint8 *)time;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);

    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    MMIAPISET_FormatDateStrByDateStyle(tm.tm_year,tm.tm_mon,tm.tm_mday,'/', date,MMICL_DISPLAY_DATE_LEN + 1);
    date_str.wstr_ptr = date_wstr;
    //date_str.is_ucs2 = FALSE;
    date_str.wstr_len = (uint16)strlen((char*)date);
    MMI_STRNTOWSTR( date_wstr, MMICL_DISPLAY_DATE_LEN, (uint8 *)date, date_str.wstr_len, date_str.wstr_len );
    
    //get duration time
#ifdef CALL_TIMES_SUPPORT
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[0]);
#else
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_duration_time);
#endif
    sprintf((char*)duration_time, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    duration_time_str.wstr_ptr  = duration_time_wstr;
    duration_time_str.wstr_len    = (uint16)strlen((char*)duration_time);
    //duration_time_str.is_ucs2   = FALSE;
    MMI_STRNTOWSTR( duration_time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)duration_time, duration_time_str.wstr_len, duration_time_str.wstr_len );

    //get location
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//电话号码不存在
    {
        uint8 origin_number_str[CC_MAX_TELE_NUM_LEN + 1] = {0};

        MMI_WSTRNTOSTR((uint8 *)origin_number_str, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len);

        if(MMIAPICC_RemoveIpNumber( origin_number_str, num_str.wstr_len, origin_number_arr,CC_MAX_TELE_NUM_LEN))
        {
            origin_num_len = strlen((char*)origin_number_arr);
        }
        else
        {
            MMI_WSTRNTOSTR((uint8 *)origin_number_arr, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len );
            //MMI_MEMCPY(origin_number_arr, CC_MAX_TELE_NUM_LEN + 2, num_str.str_ptr, num_str.length, num_str.length);
            origin_num_len = num_str.wstr_len;
        }
        
        str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_arr, origin_num_len, (uint16*)city_ucs2_str);
    }
    else
    {
        str_length = 0;
    }
    if( str_length == 0 )
    {
        MMI_GetLabelTextByLang(TXT_UNKNOWN, &location_str);
    }   
    else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
    {
        MMI_GetLabelTextByLang(TXT_LOCAL, &location_str);
    }   
    else
    {
        location_str.wstr_ptr = city_ucs2_str;
        //location_str.is_ucs2 = TRUE;
        location_str.wstr_len  = str_length;
    }
#endif
    
    //get call type
    if (arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
    {
        MMI_GetLabelTextByLang(TXT_CALL_VIDEO, &call_type_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CALL_AUDIO, &call_type_str);
    }   

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!    
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //get sim name
        sim_name = MMIAPISET_GetSimName(arraycall_info->call_bcdinfo[sel_item_index].dual_sys);
        
        sim_num.wstr_len  = sim_name.wstr_len;
        sim_num.wstr_ptr = sim_name.wstr_arr;
    }
#endif

    //display name in pb
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present && (0 != name_str.wstr_len))
    {
        //if the num is present and the name is being
        SetHeadAndContentInRText(TXT_NAME,ctrl_id,name_str,&index);
    }

    //display num
    SetHeadAndContentInRText(TXT_COMMON_NUMBER,ctrl_id,num_str,&index);

#ifdef CALL_TIMES_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time[0] > 0)
#else
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0)
#endif
    {
        //display date
        SetHeadAndContentInRText(TXT_DATE,ctrl_id,date_str,&index);

        //display time
        SetHeadAndContentInRText(TXT_TIME,ctrl_id,time_str,&index);
    }

#ifndef CMCC_UI_STYLE
    //display duration
    if ((MMICL_CALL_DIALED == arraycall_info->call_bcdinfo[sel_item_index].call_type ||
        MMICL_CALL_RECEIVED == arraycall_info->call_bcdinfo[sel_item_index].call_type)
        &&(!arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
#ifdef CALL_TIMES_SUPPORT
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time[0] > 0
#else
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0
#endif
        )//@zhaohui modify,视频通话不显示时长信息
    {
        SetHeadAndContentInRText(TXT_CL_CALL_TIME,ctrl_id,duration_time_str,&index);
    }
#endif

    //display location
    SetHeadAndContentInRText(TXT_GUISHUDI,ctrl_id,location_str,&index);

    //display type  voice call/video call	
    SetHeadAndContentInRText(TXT_COMMON_TYPE,ctrl_id,call_type_str,&index);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //display sim name
        SetHeadAndContentInRText(TXT_CL_DETAIL_SIM_HEAD,ctrl_id,sim_num,&index);
     }
#endif   

    //记录相关信息，为电话拨出提供必要信息
    s_call_log_detail_info_ptr->dual_sys = arraycall_info->call_bcdinfo[sel_item_index].dual_sys;
    SCI_MEMSET(s_call_log_detail_info_ptr->tele_num, 0, (PHONE_NUM_MAX_LEN + 2));
    SCI_MEMCPY(s_call_log_detail_info_ptr->tele_num, tele_num, (PHONE_NUM_MAX_LEN + 2));
#if defined(CL_SAVE_NAME_SUPPORT)
    if (arraycall_info->call_bcdinfo[sel_item_index].name.is_use_nv_name)
    {
        SCI_MEMCPY((void *)&s_call_log_detail_info_ptr->name,(void *)&arraycall_info->call_bcdinfo[sel_item_index].name,sizeof(MMICL_NAME_T));
    }
#endif
    //free memory
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
    
    if (PNULL != name_str.wstr_ptr)
    {
        SCI_FREE(name_str.wstr_ptr);
    }
}

/*****************************************************************************/
// 	Description : to handle the call log list detail window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLogListDetailWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T ctrl_id = MMICL_LOG_LIST_DETAIL_CTRL_ID;
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#endif
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        if (PNULL == s_call_log_detail_info_ptr)
        {
            s_call_log_detail_info_ptr = (MMICL_DETAIL_INFO_T *)SCI_ALLOCAZ(sizeof(MMICL_DETAIL_INFO_T));
            if(PNULL == s_call_log_detail_info_ptr)
            {
                //SCI_TRACE_LOW:"HandleLogListDetailWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8032_112_2_18_2_11_43_76,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
            SCI_MEMSET(s_call_log_detail_info_ptr, 0, sizeof(MMICL_DETAIL_INFO_T));
        }
        #ifdef MMI_PDA_SUPPORT
        GUILIST_SetMaxItem(ctrl_id, 100, FALSE);
        #endif
        SetLogListDetail();
#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/12
            #ifndef MMI_PDA_SUPPORT
	    GUIRICHTEXT_SetItemBorderSpace(ctrl_id, 0);
	    #endif
#endif 	 

        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)STXT_OPTION,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN, FALSE);

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK: //rong.gu to do
    case MSG_APP_OK:
    case MSG_APP_MENU:
        switch (s_cur_sel_log_list_type)
        {
        case MMICL_CALL_MISSED:
        case MMICL_CALL_RECEIVED:
        case MMICL_CALL_DIALED:
        case MMICL_CALL_MATCHED:
#ifdef MMI_PDA_SUPPORT
        case MMICL_CALL_ALL:
#endif
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
#endif
#ifdef BT_DIALER_SUPPORT
        case MMICL_BT_CALL_MISSED:
        case MMICL_BT_CALL_RECEIVED:
        case MMICL_BT_CALL_DIALED:
#endif
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN2_TAB, PNULL);
            break;
            

        default:
 //           SCI_TRACE_LOW("HandleLogListDetailWindow:s_cur_sel_log_list_type %d is error!",s_cur_sel_log_list_type);
            break;
        }
        break;
        
#ifdef MMI_MULTI_GREEN_KEY_SINGLE         
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        break;
#else        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);                
            HandleRingBack(sim_type, FALSE);
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
#if 1//OPEN_VT_CHANGE
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
#else
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
#endif
        break;        
#endif
        
        //@samboo.shen 一键vt
    case MSG_APP_VIDEO_TEL:
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        MMK_CloseWin(win_id);
        break; 
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_call_log_detail_info_ptr)
        {
            SCI_FREE(s_call_log_detail_info_ptr);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}
/*****************************************************************************/
// 	Description : Get Current Record Info for next operation
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void GetCurrentRecordInfo(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info_ptr , uint16* sel_item_index_ptr)
{
    uint16   sel_item_index =0;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    
    list_ctrl_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    //read logs of calls from NV
#ifdef MMI_PDA_SUPPORT
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info_ptr);
        
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[s_mmicl_match_item_index].array_index;
    }
    else
    {        
        sel_item_index = CL_GetArrayIndexofCurSelected();
    }
#else
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[s_mmicl_match_item_index].array_index;
        MMICL_GetRecordInfo(s_match_call_info_ptr->call_info[sel_item_index].call_type, arraycall_info_ptr);
        sel_item_index = s_match_call_info_ptr->call_info[sel_item_index].array_index;
    }
    else
    {
        MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info_ptr);
        sel_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    }
#endif

    *sel_item_index_ptr = sel_item_index;
}
/*****************************************************************************/
// 	Description : delete current recode item
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteCurrentRecord(void)
{
    uint8       record_num = 0;
    uint16      array_index = 0;
    if(PNULL != s_match_call_info_ptr && MMICL_CALL_MATCHED == s_cur_sel_log_list_type)
    {
        array_index = s_match_call_info_ptr->call_info[s_mmicl_match_item_index].array_index;
#if !defined(MMI_PDA_SUPPORT)
        s_cur_sel_log_list_type = s_match_call_info_ptr->call_info[array_index].call_type;
        array_index = s_match_call_info_ptr->call_info[array_index].array_index;
#endif
    }
    else
    {
        array_index = CL_GetArrayIndexofCurSelected();
    }

    //delete appointed record from nv
    record_num = MMICL_DeleteAppointedCallInfo(s_cur_sel_log_list_type,(uint8)array_index);
    
    //success!
    //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);

    if (PNULL != s_match_call_info_ptr && MMICL_CALL_MISSED == s_match_call_info_ptr->call_info[s_mmicl_match_item_index].call_type)
    {
#ifdef CALL_TIMES_SUPPORT    
        MMIAPICC_DecMissedCallNumber((uint32)(s_match_call_info_ptr->call_info[s_mmicl_match_item_index].call_times));
#else
        MMIAPICC_DecMissedCallNumber(1);
#endif
    }
    
//    if (0 == record_num) //没有记录
//    {
        // close log list window
//        MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
//    }
    
    //close the window of call log list menu and detail win
    MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
}

/*****************************************************************************/
// 	Description : ring back (回复电话)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBack(CC_CALL_SIM_TYPE_E call_sim_type, 
                                                BOOLEAN is_video_call
                                                )
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint32 call_type = CC_CALL_NORMAL_CALL;
    MMI_STRING_T *name_ptr = PNULL;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMI_STRING_T  call_name = {0};
    wchar temp_str[MMICL_NAME_MAX_LEN +1] = {0};
#endif

#if defined(CL_SAVE_NAME_SUPPORT)
    call_name.wstr_ptr = temp_str;
    if (GetDisplayPhoneName(&call_name))
    {
        name_ptr = &call_name;
    }
    else
    {
        name_ptr = PNULL;
    }
#endif
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if ( num_len > 0 )
    {
        if (is_video_call)
        {
            call_type = CC_CALL_VIDEO_CALL;
        }
        else
        {
            call_type = CC_CALL_NORMAL_CALL;
        }
        MMIAPICC_MakeCall(
                                        dual_sys,
                                        tele_num,
                                        num_len,
                                        PNULL,
                                        name_ptr,
                                        call_sim_type,
                                        call_type,
                                        PNULL
                                        );
    }
    else
    {
        //提示空号!
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
    
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : clean calllog wait win
// 	Global resource dependence : none
// Author: cheney.wu
// 	Note:
/*****************************************************************************/
PUBLIC void MMICL_ClosePBAPWaitWin(void)
{
    if(MMK_IsOpenWin(MMICL_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMICL_PBAP_CONNECT_WAIT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMICL_SYNC_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMICL_SYNC_WAIT_WIN_ID);
    }
}

/*****************************************************************************/
// 	Description : clean calllog wait win
// 	Global resource dependence : none
// Author: cheney.wu
// 	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_DeleteAllPbapSyncData(void)
{
    DeleteAllPbapCallLog();
//    DeleteAllCallLog(MMICL_BT_CALL_DIALED);
//    DeleteAllCallLog(MMICL_BT_CALL_RECEIVED);
//    DeleteAllCallLog(MMICL_BT_CALL_MISSED);
    SetPbapAllowSync(MMICL_BT_CALL_MISSED, TRUE);
    SetPbapAllowSync(MMICL_BT_CALL_RECEIVED, TRUE);
    SetPbapAllowSync(MMICL_BT_CALL_DIALED, TRUE);
}

/*****************************************************************************/
// 	Description : get display phone number of current log list 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void GetCurrentPhoneNum(
                              uint8*   tele_num, //out
                              uint8		max_tele_len
                              )
{
    if(max_tele_len > (PHONE_NUM_MAX_LEN + 2))
    {
        SCI_TRACE_LOW("GetCurrentPhoneNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)",(uint8*)"");
        return;
    }
    
    if(PNULL == tele_num)
    {
		SCI_TRACE_LOW("GetCurrentPhoneNum PNULL == tele_num ",(uint8*)"");
        return;
    }
	
    //如果通话记录详情窗口打开，号码从窗口信息中获得
    if ((MMK_IsFocusWin(MMICL_LOG_LIST_DETAIIL_WIN_ID) 
#ifdef CALL_TIMES_SUPPORT
        ||MMK_IsFocusWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
#endif
        )&& (PNULL != s_call_log_detail_info_ptr))
    {
        SCI_MEMCPY(tele_num, s_call_log_detail_info_ptr->tele_num, max_tele_len);
    }
    //否则从列表中获得
    else
    {
        uint16 array_index = 0;
        MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
		
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            SCI_TRACE_LOW("GetCurrentPhoneNum call info alloc failed",(uint8*)"");
            return;
        }
		
        SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        
        if(PNULL != s_match_call_info_ptr)
        {
            s_cur_sel_log_list_type = MMICL_CALL_MATCHED;
        }
        GetCurrentRecordInfo(arraycall_info, &array_index);
		
        if (arraycall_info->call_bcdinfo[array_index].is_num_present)//电话号码存在
        {
            //get number
            MMICL_GenDispNumber(arraycall_info->call_bcdinfo[array_index].call_type,
                arraycall_info->call_bcdinfo[array_index].number_type,
                arraycall_info->call_bcdinfo[array_index].number_len,
                arraycall_info->call_bcdinfo[array_index].number, 
                tele_num,
                max_tele_len);
        }
		
        //if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleBtCall(void)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;

    //get the phone number
    GetCurrentPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2);
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if ( num_len > 0 )
    {
       	MMIAPICC_MakeBtCall(tele_num, num_len);
    }
    else
    {
        //提示空号!
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : ring back (回复电话)
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleRingBackByType(uint32 call_type)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    BOOLEAN  is_video_call = FALSE;
 

    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    
    num_len = (uint8)strlen((char*)tele_num);

    if (call_type == CC_CALL_DEFAULT_CALL)
    {
        //Get Current log type, if video call,call video call back, if audio call,audio call back
 #ifdef TOUCH_PANEL_SUPPORT
       GetCallLogInfoDetail(&dual_sys,&is_video_call);
#endif
		call_type = CC_CALL_NORMAL_CALL;
    }
    //call
    if ( num_len > 0 )
    {
 #ifdef TOUCH_PANEL_SUPPORT
        if (is_video_call)
        {
            call_type = CC_CALL_VIDEO_CALL;
        }
#endif
        MMIAPICC_MakeCall(
                        dual_sys,
                        tele_num,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        call_type,
                        PNULL
                        );
    }
    else
    {
        //提示空号!
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CL_EMPTY_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    
}
//#endif
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
// 	Description : ip call
//	Global resource dependence : 
//  Author:lin.lin
//	Note: 
/*****************************************************************************/
LOCAL void HandleIpCall(void)
{
    uint8       tele_num[PHONE_NUM_MAX_LEN + 2] = {0},num_len = 0;
    MN_DUAL_SYS_E dual_sys_dummy = MN_DUAL_SYS_1;
    MMI_STRING_T *name_ptr = PNULL;
#if defined(CL_SAVE_NAME_SUPPORT)
    MMI_STRING_T  call_name = {0};
    wchar temp_str[MMICL_NAME_MAX_LEN +1] = {0};
#endif

#if defined(CL_SAVE_NAME_SUPPORT)
    call_name.wstr_ptr = temp_str;
    if (GetDisplayPhoneName(&call_name))
    {
        name_ptr = &call_name;
    }
    else
    {
        name_ptr = PNULL;
    }
#endif
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys_dummy);
        
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if (num_len > 0)
    {
        MMIAPICC_MakeCall(
                                        dual_sys_dummy,
                                        tele_num,
                                        num_len,
                                        PNULL,
                                        name_ptr,
                                        CC_CALL_SIM_MAX,
                                        CC_CALL_IP_CALL,
                                        PNULL
                                        );
    }
    else
    {
        //提示空号!
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

/*****************************************************************************/
// 	Description : get current log list control id
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
PUBLIC CC_CONTROL_ID_E MMICL_GetLogListCtrlId(MMICL_CALL_TYPE_E call_type)
{
    CC_CONTROL_ID_E		result = MMICL_DIALED_LIST_CTRL_ID;

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle()
        &&MMK_IsOpenWin(MMICL_ALL_CHILD_WIN_ID)) //istyle，the all_child win is open
    {
        return MMICL_ALL_LIST_CTRL_ID;
    }
#endif

    switch (call_type)
    {
    case MMICL_CALL_MISSED:
        result = MMICL_MISSED_LIST_CTRL_ID;
        break;
        
    case MMICL_CALL_RECEIVED:
        result = MMICL_RECEIVED_LIST_CTRL_ID;
        break;
        
    case MMICL_CALL_DIALED:
        result = MMICL_DIALED_LIST_CTRL_ID;
        break;

#if defined(BT_PBAP_SUPPORT)
    case MMICL_BT_CALL_MISSED:
        result = MMICL_BT_MISSED_LIST_CTRL_ID;
        break;
        
    case MMICL_BT_CALL_RECEIVED:
        result = MMICL_BT_RECEIVED_LIST_CTRL_ID;
        break;
        
    case MMICL_BT_CALL_DIALED:
        result = MMICL_BT_DIALED_LIST_CTRL_ID;
        break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        result = MMICL_REFUSED_LIST_CTRL_ID;
        break;
#endif

#ifdef MMI_PDA_SUPPORT
    case MMICL_CALL_ALL:
        result = CalllogPBData_GetListHandle();
        break;
#endif        
    default:
        result = MMICL_DIALED_LIST_CTRL_ID;
        break;
    }
    
    return(result);
}

/*****************************************************************************/
// 	Description : get current log list type
//	Global resource dependence : 
//  Author: allen.lee
//	Note: 
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMICL_GetLogListType(void)
{
    return s_cur_sel_log_list_type;
}

#ifndef MMI_PDA_SUPPORT
#ifdef  TOUCH_PANEL_SUPPORT
#ifdef MMI_CL_TOOLBAR_SUPPORT
/*****************************************************************************/
// 	Description : get current tool bar log list control id
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL CC_CONTROL_ID_E GetLogToolBarCtrlId(MMI_WIN_ID_T win_id)
{
    CC_CONTROL_ID_E result = MMICL_TOOLBAR_DIALED_CTRL_ID;

    switch (win_id)
    {
        case MMICL_DIALED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_DIALED_CTRL_ID;
        case MMICL_RECEIVED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_RECEIVED_CTRL_ID;
        case MMICL_MISSED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_MISSED_CTRL_ID;
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_REFUSED_CHILD_WIN_ID:
            return MMICL_TOOLBAR_REFUSED_CTRL_ID;
#endif

#ifdef BT_DIALER_SUPPORT
        case MMICL_BT_DIALED_CHILD_WIN_ID:
            return MMICL_BT_TOOLBAR_DIALED_CTRL_ID;
        case MMICL_BT_RECEIVED_CHILD_WIN_ID:
            return MMICL_BT_TOOLBAR_RECEIVED_CTRL_ID;
        case MMICL_BT_MISSED_CHILD_WIN_ID:
            return MMICL_BT_TOOLBAR_MISSED_CTRL_ID;
#endif

        default:
            break;
    }
    return MMICL_TOOLBAR_MISSED_CTRL_ID;
}
#endif
#endif
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
// 	Description : add phone number to name 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAddNumToName(void)
{
    MMIPB_BCD_NUMBER_T  tele_num = {0};
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
    BOOLEAN is_num_valid = TRUE;
    uint8   tele_string[PHONE_NUM_MAX_LEN + 2] = {0};
	uint8   tele_length = 0;
    
    //get the phone number(bcd)
    GetPartyPhoneNum(&tele_num);
    
    //+bug132483
    tele_length = MMIAPIPB_BCDNumberToString(&tele_num, PHONE_NUM_MAX_LEN + 2, tele_string);
    is_num_valid = MMIAPICOM_IsValidNumberString((char *)tele_string, tele_length);
	SCI_TRACE_LOW("yanyan.an is_num_valid:%d", is_num_valid);
	SCI_TRACE_BUF("yanyan.an tele_num.number:%d", tele_string, tele_length);
	if (FALSE == is_num_valid)
	{
		//提示无效号码
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return; 
	}
    //-bug132483
    
    num_ton = tele_num.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == tele_num.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    //call
    if ((tele_num.number_len > 0) || is_plus_symbol)
    {
        MMIAPIPB_AddToExistContact(&tele_num, MMIPB_ADD_TYPE_NUMBER, PNULL);
    }
    else
    {
        //提示窗口没有号码
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CL_EMPTY_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}
#endif
#if defined(CL_SAVE_NAME_SUPPORT)
/*****************************************************************************/
// 	Description : get display phone name of current log list 
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayPhoneName(
                              MMI_STRING_T*   call_name//out
                              )
{
    if(PNULL == call_name ||PNULL == call_name->wstr_ptr)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneName call_name OR call_name->wstr_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8509_112_2_18_2_11_44_77,(uint8*)"");
        return FALSE;
    }

    //如果通话记录详情窗口打开，号码从窗口信息中获得
    if (MMK_IsFocusWin(MMICL_LOG_LIST_DETAIIL_WIN_ID) && (PNULL != s_call_log_detail_info_ptr))
    {
        if (s_call_log_detail_info_ptr->name.is_use_nv_name)
        {
            call_name->wstr_len = s_call_log_detail_info_ptr->name.wstr_len;
            MMI_WSTRNCPY(call_name->wstr_ptr,
                                        MMICL_NAME_MAX_LEN, 
                                        s_call_log_detail_info_ptr->name.wstr, 
                                        MMICL_NAME_MAX_LEN, 
                                        call_name->wstr_len);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    //否则从列表中获得
    else
    {
        uint16 array_index = 0;
        MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8544_112_2_18_2_11_44_78,(uint8*)"");
            return FALSE;
        }
    
        SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

        if(PNULL != s_match_call_info_ptr)
        {
            s_cur_sel_log_list_type = MMICL_CALL_MATCHED;
        }
        GetCurrentRecordInfo(arraycall_info, &array_index);
        
        if (arraycall_info->call_bcdinfo[array_index].name.is_use_nv_name)
        {
            call_name->wstr_len = arraycall_info->call_bcdinfo[array_index].name.wstr_len;
            MMI_WSTRNCPY(call_name->wstr_ptr, 
                                        MMICL_NAME_MAX_LEN, 
                                        arraycall_info->call_bcdinfo[array_index].name.wstr, 
                                        MMICL_NAME_MAX_LEN, 
                                        call_name->wstr_len);
        
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
            return TRUE;
        }
        else
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
            return FALSE;
        }
    }
}
#endif

/*****************************************************************************/
// 	Description : get display phone number of current log list 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void GetDisplayPhoneNum(
                              uint8*   tele_num, //out
                              uint8		max_tele_len,
                              MN_DUAL_SYS_E *dual_sys_ptr
                              )
{
    if(max_tele_len > (PHONE_NUM_MAX_LEN + 2))
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8591_112_2_18_2_11_44_79,(uint8*)"");
        return;
    }
    
    if(PNULL == tele_num ||PNULL == dual_sys_ptr)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum PNULL == tele_num ||PNULL == dual_sys_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8597_112_2_18_2_11_44_80,(uint8*)"");
        return;
    }

    //如果通话记录详情窗口打开，号码从窗口信息中获得
    if ((MMK_IsFocusWin(MMICL_LOG_LIST_DETAIIL_WIN_ID) 
    #ifdef CALL_TIMES_SUPPORT
        ||MMK_IsFocusWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
    #endif
        )&& (PNULL != s_call_log_detail_info_ptr))
    {
        SCI_MEMCPY(tele_num, s_call_log_detail_info_ptr->tele_num, max_tele_len);
        *dual_sys_ptr = s_call_log_detail_info_ptr->dual_sys;
    }
    //否则从列表中获得
    else
    {
        uint16 array_index = 0;
        MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8620_112_2_18_2_11_44_81,(uint8*)"");
            return;
        }
    
        SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        
        if(PNULL != s_match_call_info_ptr)
        {
            s_cur_sel_log_list_type = MMICL_CALL_MATCHED;
        }
        GetCurrentRecordInfo(arraycall_info, &array_index);

        if (arraycall_info->call_bcdinfo[array_index].is_num_present)//电话号码存在
        {
            //get number
            MMICL_GenDispNumber(arraycall_info->call_bcdinfo[array_index].call_type,
                arraycall_info->call_bcdinfo[array_index].number_type,
                arraycall_info->call_bcdinfo[array_index].number_len,
                arraycall_info->call_bcdinfo[array_index].number, 
                tele_num,
                max_tele_len);
        }
    
        *dual_sys_ptr = arraycall_info->call_bcdinfo[array_index].dual_sys;
    
        //if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
    }
}

/*****************************************************************************/
// 	Description : distill phone number from call log 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleDistillPhoneNum(void)
{
    MMIPB_BCD_NUMBER_T  tele_num = {0};
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
	BOOLEAN is_num_valid = TRUE;
    uint8   tele_string[PHONE_NUM_MAX_LEN + 2] = {0};
	uint8   tele_length = 0;

    //get the phone number(bcd)
    GetPartyPhoneNum(&tele_num);

    //+bug132483
    tele_length = MMIAPIPB_BCDNumberToString(&tele_num, PHONE_NUM_MAX_LEN + 2, tele_string);
    is_num_valid = MMIAPICOM_IsValidNumberString((char *)tele_string, tele_length);
	SCI_TRACE_LOW("yanyan.an is_num_valid:%d", is_num_valid);
	SCI_TRACE_BUF("yanyan.an tele_num.number:%d", tele_string, tele_length);
	if (FALSE == is_num_valid)
	{
		//提示无效号码
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return; 
	}
    //-bug132483
    
    num_ton = tele_num.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == tele_num.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    if ((tele_num.number_len > 0) || is_plus_symbol)
    {
        MMIAPIPB_AddContactWin(&tele_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
        //MMIAPIPB_OpenAddMenu(&tele_num,PB_GROUP_ALL);
    }
    else
    {
        //提示窗口没有号码
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : add a number into forbidden number list
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleForbiddenNum(void)
{
    MMIPB_BCD_NUMBER_T  tele_num = {0};
    uint8  num_ton = 0;//@fen.xie MS00180421
    BOOLEAN is_plus_symbol = FALSE;
    MMI_STRING_T mmi_calllog_name ={0};
    MMISET_BLACKLIST_ERROR_TYPE_E add_blacklist_result = MMISET_BLACKLIST_ERROR_MAX;
    
    //get the phone number(bcd)
    GetPartyPhoneNum(&tele_num);

    num_ton = tele_num.npi_ton>> 4;//@fen.xie MS00180421
    if ((0 == tele_num.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))/*@fen.xie MS00180421: '+' */
    {
        is_plus_symbol = TRUE;
    }

    if ((tele_num.number_len > 0) || is_plus_symbol)
    {
        MMIAPIPB_GetNameByNumber(&tele_num,
                                  &mmi_calllog_name,
                                  MMICL_NAME_MAX_LEN,
                                  TRUE  //use default name,"no name"
                                  );
        add_blacklist_result = MMIAPISET_AddBlacklistFromExt(&mmi_calllog_name,&tele_num);      
        
        if(MMISET_BLACKLIST_ERROR_NONE == add_blacklist_result)
        {
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else if(MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM== add_blacklist_result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
        }
        else if(MMISET_BLACKLIST_ERROR_SAME_NUM == add_blacklist_result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_NUM_EXIST);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        }
        if(MMK_IsOpenWin( MMICL_LOG_LIST_MENU_WIN_ID))
        {
            MMK_CloseWin( MMICL_LOG_LIST_MENU_WIN_ID);
        }
    }
    else
    {
        //提示窗口没有号码
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

/*****************************************************************************/
// 	Description : get bcd phone number of current log list 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void GetPartyPhoneNum(
                            MMIPB_BCD_NUMBER_T*   bcd_number//out
                            )
{
    uint16                          array_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8757_112_2_18_2_11_45_82,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &array_index);
    
    if(PNULL == bcd_number)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum PNULL == bcd_number"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8767_112_2_18_2_11_45_83,(uint8*)"");
        return;
    }

    
    if (arraycall_info->call_bcdinfo[array_index].is_num_present)//电话号码存在
    {
        //get number
        bcd_number->number_len = arraycall_info->call_bcdinfo[array_index].number_len;
        bcd_number->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[array_index].number_type,
            arraycall_info->call_bcdinfo[array_index].number_plan);
        MMI_MEMCPY(bcd_number->number,MMIPB_BCD_NUMBER_MAX_LEN,
            arraycall_info->call_bcdinfo[array_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
            bcd_number->number_len);
    }
    
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
}

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerSms(void)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if ( num_len > 0 )
    {
        MMIAPISMS_ExitFromSMS();

        //调用sms接口函数
        MMIAPISMS_AnswerMessage(dual_sys ,PNULL,tele_num,num_len);
    }
    else
    {
        //提示窗口没有号码
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}

/*****************************************************************************/
// 	Description : answer sms back
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void HandleAnswerMms(void)
{
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 num_len                         = 0;
    MN_DUAL_SYS_E dual_sys                = MN_DUAL_SYS_1;
//     uint32 record_id                      = 0;
    MMI_STRING_T string_to = {0};
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    
    //get the phone number
    GetDisplayPhoneNum(tele_num, PHONE_NUM_MAX_LEN + 2, &dual_sys);
    num_len = (uint8)strlen((char*)tele_num);
    
    //call
    if ( num_len > 0 )
    {
        //调用mms接口函数
        string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
        MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)tele_num, num_len, num_len);
        string_to.wstr_len = num_len;
        MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_CL_SEND);
        MMIAPIMMS_AnswerMMS(dual_sys,PNULL,&string_to);
        SCI_FREE(string_to.wstr_ptr);        
    }
    else
    {
        //提示窗口没有号码
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}


/*****************************************************************************/
// 	Description : delete all call log with menu id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL void DeleteAllCallLog(
                            MMICL_CALL_TYPE_E  call_type//in
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint8 i = 0,count = 0;
    BOOLEAN result = FALSE;
   
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8877_112_2_18_2_11_45_84,(uint8*)"");
        return;
    }   
    if(call_type == MMICL_CALL_ALL)
    {
        if (MMICL_CALL_ALL==GetAllLogFillter())
        {
            SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

            MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
            
            //if (PNULL != missed_call)
            {
                SCI_FREE(arraycall_info);
                arraycall_info = PNULL;
            } 
#ifdef MMI_LDN_SUPPORT
            result = MMIAPICL_DeleteAllLdnInfo(MMICL_CALL_DIALED);
#endif
            return;
         }
    }
    arraycall_last_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_last_info)
    {
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8899_112_2_18_2_11_45_85,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(arraycall_last_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    //get the value of arraycall_info from nv
    return_value = MMICL_ReadAllNV(arraycall_info);
    for(i = 0; i <arraycall_info->record_num; i++)
    {
        if (call_type != MMICL_CALL_ALL)
        {
            if(call_type != arraycall_info->call_bcdinfo[i].call_type)
            {
                memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
            }
        }
        else
        {
            if(GetAllLogFillter()==MMICL_CALL_PB_NUMBER_MATCHED)
            {
                //Delete selected item
                if (CalllogPBData_GetIsSelectedByIndex(i))
                {
                    continue;
                }
				memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );				
            }
            else if( arraycall_info->call_bcdinfo[i].call_type!=GetAllLogFillter())
            {
                memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
            }        
            else
            {
                continue;
            }
        }
    }
    arraycall_last_info->record_num = count;
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_last_info);
    
    //if (PNULL != missed_call)
    {
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
        SCI_FREE(arraycall_last_info);
        arraycall_last_info = PNULL;
    }
   
    //SCI_TRACE_LOW:"DeleteAllCallLog: the call_type is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8950_112_2_18_2_11_45_86,(uint8*)"d",call_type);
}
#else
/*****************************************************************************/
// 	Description : delete all call log with menu id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteAllCallLog(
                            MMICL_CALL_TYPE_E  call_type//in
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
    uint8 i = 0,count = 0;
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
    uint16 cur_count=0;
#endif
   
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8969_112_2_18_2_11_45_87,(uint8*)"");
        return;
    }   
    if(call_type == MMICL_CALL_ALL) //All .memset it and write NV
    {
        SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
        SCI_FREE(arraycall_info);
        return;
    }

    //If no,copy and overwrite
    arraycall_last_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_last_info)
    {
        SCI_FREE(arraycall_info);
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8985_112_2_18_2_11_45_88,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(arraycall_last_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    //get the value of arraycall_info from nv
    MMICL_ReadNV(MMICL_CALL_ALL,arraycall_info);
    for(i = 0; i <arraycall_info->record_num; i++)
    {
        if (call_type != MMICL_CALL_ALL)
        {
            if(call_type != arraycall_info->call_bcdinfo[i].call_type)
            {
                memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
            }
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
            else
            {
                if (isMultiSelecting())
                {
                    if (!isCurrentSelected(cur_count))
                    {
                        memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
                    }
                    cur_count++;
                }
            }
#endif
        }
        
    }
    arraycall_last_info->record_num = count;
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_last_info);
    
    SCI_FREE(arraycall_info);
    SCI_FREE(arraycall_last_info);
   
    //SCI_TRACE_LOW:"DeleteAllCallLog: the call_type is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9012_112_2_18_2_11_45_89,(uint8*)"d",call_type);
#ifdef MMI_LDN_SUPPORT
    if(!MMIAPICL_DeleteAllLdnInfo(call_type))
#endif
    {
       // MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
    }
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : delete all call log with menu id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DeleteAllPbapCallLog(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
    uint8 i = 0,count = 0;
    MMICL_CALL_TYPE_E  call_type = MMICL_CALL_TYPE_MAX;
   
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8969_112_2_18_2_11_45_87,(uint8*)"");
        return;
    }   

    //If no,copy and overwrite
    arraycall_last_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_last_info)
    {
        SCI_FREE(arraycall_info);
        //SCI_TRACE_LOW:"DeleteAllCallLog call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_8985_112_2_18_2_11_45_88,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(arraycall_last_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    //get the value of arraycall_info from nv
    MMICL_ReadNV(MMICL_CALL_ALL,arraycall_info);
    for(i = 0; i <arraycall_info->record_num; i++)
    {
        call_type = arraycall_info->call_bcdinfo[i].call_type;
        if( (call_type != MMICL_BT_CALL_MISSED) &&
            (call_type != MMICL_BT_CALL_RECEIVED) &&
            (call_type != MMICL_BT_CALL_DIALED)
            )
        {
            memcpy(&arraycall_last_info->call_bcdinfo[count++], &arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
        }
    }
    arraycall_last_info->record_num = count;
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_last_info);
    
    SCI_FREE(arraycall_info);
    SCI_FREE(arraycall_last_info);

    MMICL_ResetPbapSyncCount();
   
    //SCI_TRACE_LOW:"DeleteAllCallLog: the call_type is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9012_112_2_18_2_11_45_89,(uint8*)"d",call_type);

}
#endif
#endif

/*****************************************************************************/
// 	Description : display the time of last call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimerWindow(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    GUI_RECT_T      rect = MMITHEME_GetClientRect();
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T ctrl_id = MMICL_CALL_TIMER_CTRL_ID;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
#ifdef MMI_CSP_SUPPORT    
    uint8 line = MMIAPISET_GetCallLineInfo(dual_sys);
#endif    
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetTitleTextId(win_id,(MMI_TEXT_ID_T)TXT_CL_CALL_TIMERS, FALSE);
        #ifdef MMI_PDA_SUPPORT
        GUILIST_SetMaxItem(ctrl_id, 100, FALSE);
        #endif
        //set softkey
        #ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_CL_RESET,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN,FALSE);
        #endif
        //GUIRICHTEXT_SetScroll(ctrl_id, TRUE);
        {
            MMI_STRING_T item_str = {0};
            uint16 display_index = 0;
            SCI_MEMSET(&item_str,0,sizeof(MMI_STRING_T));
            //set name 
#ifdef MMI_CSP_SUPPORT
            DisplayCallTimeItem(line, MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(line, MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#else //MMI_CSP_SUPPORT
            DisplayCallTimeItem(MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#endif //MMI_CSP_SUPPORT 
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef   MMI_PDA_SUPPORT      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
 case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
        break;
   #ifdef  MMI_PDA_SUPPORT   //not respond menu key
    case MSG_GET_FOCUS:
        {
            uint16 display_index = 0;
            GUILIST_RemoveAllItems(ctrl_id);
            //set name 
#ifdef MMI_CSP_SUPPORT
            DisplayCallTimeItem(line, MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(line, MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(line, MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#else //MMI_CSP_SUPPORT
            DisplayCallTimeItem(MMINV_CL_LASTCALL_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_TIMER_LAST_CALL);//????
            DisplayCallTimeItem(MMINV_CL_RECEIVEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_RECEIVED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_DIALEDCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DIALED_CALLS);//????
            DisplayCallTimeItem(MMINV_CL_ALLCALLS_TIME, dual_sys, ctrl_id, &display_index, TXT_CL_DEL_ALLLOGS);//????  
#endif //MMI_CSP_SUPPORT            
        }
        break;
   #endif
         
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
        #ifndef  MMI_PDA_SUPPORT   //not respond menu key
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        
        MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        break;
        #endif
    case MSG_PROMPTWIN_OK:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(&query_win_id);
        ResetCalllogTimer(dual_sys);
//        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&query_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : display call time in control
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            uint8 line, 
                            CC_NV_ITEM_E   nv_item, //@$discuss 6800????CC_NV_ITEM_E,????????MMI_NV_ITME_E
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            )
{
    uint8               time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar               time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    CC_CALL_TIME_COUNT_T call_time = {0};/*lint !e64*/
    MMI_STRING_T        time_str = {0};
//     MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    switch(nv_item)
    {
    case MMINV_CL_LASTCALL_TIME:
        MMIAPICL_GetLastCallTime(line, &call_time, dual_sys);
        break;

    case MMINV_CL_ALLCALLS_TIME:
        MMIAPICL_GetAllCallsTime(line, &call_time, dual_sys);
        break;

    case MMINV_CL_RECEIVEDCALLS_TIME:
        MMIAPICL_GetReceivedCallsTime(line, &call_time, dual_sys);
        break;

    case MMINV_CL_DIALEDCALLS_TIME:
        MMIAPICL_GetDialedCallsTime(line, &call_time, dual_sys);
        break;
        
    default:   
        //SCI_TRACE_LOW:"SetCallTimeLabel:nv_item %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9180_112_2_18_2_11_46_90,(uint8*)"d",nv_item);
    }

    if(0 == line)
    {
        call_time.line1_call_time = MIN(call_time.line1_call_time, MMICL_TIMER_MAX);
        //divert time type
        ConvertTimeToDisplay(call_time.line1_call_time, time);

    }
    else if(1 == line)
    {
        call_time.line2_call_time = MIN(call_time.line2_call_time, MMICL_TIMER_MAX);
        //divert time type
        ConvertTimeToDisplay(call_time.line2_call_time, time);

    }

    time_str.wstr_ptr = time_wstr;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);
    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    SetHeadAndContentInRText(head_text_id, ctrl_id, time_str, index_ptr);

}
#else //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : display call time in control
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void DisplayCallTimeItem(
                            CC_NV_ITEM_E   nv_item, //@$discuss 6800上用的是CC_NV_ITEM_E,但实际调用的还是MMI_NV_ITME_E
                            MN_DUAL_SYS_E dual_sys,
                            MMI_CTRL_ID_T ctrl_id,
                            uint16* index_ptr,
                            MMI_TEXT_ID_T head_text_id
                            )
{
    uint8               time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar               time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    CC_CALL_TIME_COUNT_T call_time = {0};/*lint !e64*/
    MMI_STRING_T        time_str = {0};
//     MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    switch(nv_item)
    {
    case MMINV_CL_LASTCALL_TIME:
        MMIAPICL_GetLastCallTime(&call_time, dual_sys);
        break;

    case MMINV_CL_ALLCALLS_TIME:
        MMIAPICL_GetAllCallsTime(&call_time, dual_sys);
        break;

    case MMINV_CL_RECEIVEDCALLS_TIME:
        MMIAPICL_GetReceivedCallsTime(&call_time, dual_sys);
        break;

    case MMINV_CL_DIALEDCALLS_TIME:
        MMIAPICL_GetDialedCallsTime(&call_time, dual_sys);
        break;
        
    default:   
        //SCI_TRACE_LOW:"SetCallTimeLabel:nv_item %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9180_112_2_18_2_11_46_90,(uint8*)"d",nv_item);
        break;
    }
    
    call_time.call_time = MIN(call_time.call_time, MMICL_TIMER_MAX);

    //divert time type
    ConvertTimeToDisplay(call_time.call_time, time);
    time_str.wstr_ptr = time_wstr;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);
    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    SetHeadAndContentInRText(head_text_id, ctrl_id, time_str, index_ptr);

}
#endif //MMI_CSP_SUPPORT

/*****************************************************************************/
// 	Description : 转换uint32的时间为时：分：秒的格式
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void ConvertTimeToDisplay(uint32  call_time,//in
                                uint8   *time//out
                                )
{
    uint8      sec     = 0;     // seconds after the minute - [0,59] 
    uint8      min     = 0;     // minutes after the hour - [0,59] 
    uint32     hour    = 0;    
    
    if(PNULL == time)
    {
        //SCI_TRACE_LOW:"ConvertTimeToDisplay  time is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9210_112_2_18_2_11_46_91,(uint8*)"");
        return;
    }
    
    sec       = (uint8)(call_time % 60);
    call_time = call_time/60;
    min       = (uint8)(call_time % 60);
    hour      = (uint32)(call_time / 60);
    
    sprintf((char*)time, "%02d:%02d:%02d",hour,min,sec);
}

/*****************************************************************************/
// 	Description : reset the timer
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void ResetCalllogTimer(MN_DUAL_SYS_E dual_sys)
{
//     CC_CALL_TIME_COUNT_T  time = {0};/*lint !e64*/

    MMIAPICL_SetDefaultCallTimeToNVBySim(dual_sys);
}

/*****************************************************************************/
// 	Description : open window CL main menu window
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_OpenMainMenuWindowExt(void)
{
    MMIMAINMENU_TYPE_E  mainmenu_style = MMIAPISET_GetMainmenuStyle();
    
    SCI_ASSERT(mainmenu_style <= MMIMAINMENU_TYPE_MAX); /*assert verified*/

    MMK_CloseMiddleWin(MMICL_START_WIN_ID, MMICL_END_WIN_ID);

#ifdef MMI_CALLLOG_HIDE_MAINMENU
    MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
#else
    MMK_CreateWin((uint32*)MMICL_ICON_MAIN_MENU_WIN_TAB, PNULL);
#endif
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : get selected cl item number
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSelectedItemNum(uint8* tele_num,  uint8 max_tele_len)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    if(max_tele_len > (PHONE_NUM_MAX_LEN + 2))
    {
        //SCI_TRACE_LOW:"MMIAPICL_GetSelectedItemNum max_tele_len > (PHONE_NUM_MAX_LEN + 2)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9317_112_2_18_2_11_46_93,(uint8*)"");
        return FALSE;
    }
    
    if(PNULL == tele_num)
    {
        //SCI_TRACE_LOW:"MMIAPICL_GetSelectedItemNum PNULL == tele_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9323_112_2_18_2_11_46_94,(uint8*)"");
        return FALSE;
    }
    
    GetDisplayPhoneNum(tele_num, max_tele_len, &dual_sys);
    MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
    //close the window of call log list menu and detail win
    MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : get current selected call type
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMICL_GetCurCallType(void)
{
    return s_cur_sel_log_list_type;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : to handle sim select window
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  CallTimeSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    if(isReturnOK)
        {
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMER_WIN_TAB, (ADD_DATA)dual_sys);
        }
        
    return recode;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : display one item of the cl detail
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //标题文字id
                               MMI_CTRL_ID_T ctrl_id,            //所在的控件容器
                               MMI_STRING_T content_str,         //显示内容
                               uint16   *index
                               )
{
    GUILIST_ITEM_T item_t = {0};    /*lint !e64*/ 
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/
    MMI_STRING_T string_prefix = {0};       //标题的内容
//     MMI_STRING_T title_string = {0};        //标题增加“：”
    //MMI_STRING_T colon_string = {0};
    //wchar colon_str[] = {':', 0};
    uint8 wstr_len = 0;
    wchar dst_string[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};

    if(PNULL == index)
    {
        //SCI_TRACE_LOW:"SetHeadAndContentInRText PNULL == index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9385_112_2_18_2_11_46_95,(uint8*)"");
        return;
    }
    

    if(0 == content_str.wstr_len)
    {
        return;
    }
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
     
    /*display head*/
    //get content of the head
    MMI_GetLabelTextByLang(head_text_id, &string_prefix);
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    
    item_data.item_content[0].item_data_type= GUIITEM_DATA_TEXT_BUFFER;    
    item_data.item_content[0].item_data.text_buffer.wstr_len = wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = dst_string;

    item_data.item_content[1].item_data_type= GUIITEM_DATA_TEXT_BUFFER;    
    item_data.item_content[1].item_data.text_buffer.wstr_len = content_str.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr =  content_str.wstr_ptr;

    GUILIST_AppendItem(ctrl_id, &item_t);
}
#else
/*****************************************************************************/
// 	Description : display one item of the cl detail
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //标题文字id
                               MMI_CTRL_ID_T ctrl_id,            //所在的控件容器
                               MMI_STRING_T content_str,         //显示内容
                               uint16   *index
                               )
{
    GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
    MMI_STRING_T string_prefix = {0};       //标题的内容
//     MMI_STRING_T title_string = {0};        //标题增加“：”
    MMI_STRING_T colon_string = {0};
    wchar colon_str[] = {':', 0};
    uint8 wstr_len = 0;
    wchar dst_string[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};

    if(PNULL == index)
    {
        //SCI_TRACE_LOW:"SetHeadAndContentInRText PNULL == index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9434_112_2_18_2_11_46_96,(uint8*)"");
        return;
    }

    if(0 == content_str.wstr_len)
    {
        return;
    }
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    /*display head*/
    //get content of the head
    MMI_GetLabelTextByLang(head_text_id, &string_prefix);
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.len = wstr_len;

    //add ":"
    colon_string.wstr_ptr = (wchar *)colon_str;
    colon_string.wstr_len = 1;
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string + item_data.text_data.buf.len, &colon_string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.len += wstr_len;
    item_data.text_data.buf.str_ptr = dst_string;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, index);

    /*display content*/
    item_data.text_data.buf.len = content_str.wstr_len;
    item_data.text_data.buf.str_ptr = content_str.wstr_ptr;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, index);
}
#endif 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_UpdateCalllogWin(BOOLEAN is_update)
{
    MMI_HANDLE_T    win_handle=0;
    
    win_handle = MMICL_GetUpdatedWinHandle();

    if (win_handle == MMK_GetFocusWinHandle() || win_handle ==MMK_GetFocusWinId())
    {
        MMK_PostMsg(win_handle, MSG_FULL_PAINT, 0,0);
    }
    
}

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenGprsFlowRateWin(void)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;

    //MS00214101:避免重入失败，故先关闭相关窗口
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMK_CloseWin(MMICL_COM_SELECT_SIM_WIN_ID);
#endif    
    MMK_CloseWin(MMICL_GPRS_FLOWRATE_WIN_ID);
    MMIPUB_CloseQuerytWin(&query_win_id);//MS00225189

    sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_sys, 1);

    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (1 == sim_num)
    {
        MMK_CreateWin((uint32*)MMICL_GPRS_FLOWRATE_WIN_TAB, (ADD_DATA)sim_sys);
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
    MMICL_SelectSIMFunc(SIMSelectForGprsCallback, MMISET_APPEND_SIM_OK,PNULL);
    }
#endif
}

/*****************************************************************************/
// 	Description : to close the gprs flow rate window
//	Global resource dependence : none
//  Author: fuyan.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_CloseGprsFlowRateWin(void)
{
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMK_CloseWin(MMICL_COM_SELECT_SIM_WIN_ID);
#endif    
    MMK_CloseWin(MMICL_GPRS_FLOWRATE_WIN_ID);
    MMIPUB_CloseQuerytWin(&query_win_id);//MS00225189
}

/*****************************************************************************/
// 	Description : display gprs flow rate
//	Global resource dependence : 
//  Author:li.li
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGprsFlowrateWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T        title_body = {0};
    MMI_STRING_T        title = {0};
    MMISET_SIM_NAME_T   sim_name = {0};	
    wchar               sim_select[MMISET_MAX_SIM_NAME_LEN+1] = {0};
    uint16              name_len = 0;
#endif
    MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_1;  
    MMI_WIN_ID_T    query_win_id = MMICL_QUERY_WIN_ID;

    //SCI_TRACE_LOW:"HandleGprsFlowrateWinMsg dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_9557_112_2_18_2_11_47_97,(uint8*)"d", dual_sys);
    dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
            
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_MULTI_SIM_SYS_SINGLE        
        sim_name = MMIAPISET_GetSimName(dual_sys);
        name_len = sim_name.wstr_len;
        MMI_WSTRNCPY(sim_select, name_len, sim_name.wstr_arr, name_len, name_len);

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_GPRS_FLOWRATE, &title_body);

        title.wstr_len = name_len + title_body.wstr_len;

        if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr, sim_name.wstr_len)
            && MMIAPICOM_IsASCIIString(title_body.wstr_ptr, title_body.wstr_len))//都是Ascii码，补上空格
        {
            title.wstr_len += 1;
        }
        
        title.wstr_ptr = (wchar*)(SCI_ALLOCA((title.wstr_len+1)*sizeof(wchar)));
        if (PNULL != title.wstr_ptr)
        {
            SCI_MEMSET(title.wstr_ptr, 0, ((title.wstr_len+1)*sizeof(wchar)));
            MMI_WSTRNCPY(title.wstr_ptr, name_len, sim_select, name_len, name_len);    

            if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr, sim_name.wstr_len)
                && MMIAPICOM_IsASCIIString(title_body.wstr_ptr, title_body.wstr_len))//都是Ascii码，补上空格
            {
                *(title.wstr_ptr + name_len) = 0x0020;
                name_len += 1;
            }

            MMI_WSTRNCPY((title.wstr_ptr + name_len), title_body.wstr_len, title_body.wstr_ptr, title_body.wstr_len, title_body.wstr_len);
            
            GUIWIN_SetTitleText(win_id,title.wstr_ptr,title.wstr_len,FALSE);	
            SCI_FREE(title.wstr_ptr);
        }
        else//alloc fail 
        {
            GUIWIN_SetTitleTextId(win_id,(MMI_TEXT_ID_T)TXT_CL_GPRS_FLOWRATE, FALSE);
        }
        
#else
        GUIWIN_SetTitleTextId(win_id,(MMI_TEXT_ID_T)TXT_CL_GPRS_FLOWRATE, FALSE);
#endif

         //set softkey
        #ifndef   MMI_PDA_SUPPORT      
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_CL_RESET,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN,FALSE);
        #else       
        GUILIST_SetMaxItem(MMICL_GPRS_FLOWRATE_CTRL_ID, 100, FALSE);
        #endif
        SetGprsFlowrateWindow(MMICL_GPRS_FLOWRATE_CTRL_ID,dual_sys);        
        MMK_SetAtvCtrl(win_id, MMICL_GPRS_FLOWRATE_CTRL_ID);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef   MMI_PDA_SUPPORT      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
 case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        
#ifndef MMI_PDA_SUPPORT        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:    
    case MSG_APP_OK:

        MMIPUB_OpenQueryWinByTextId(TXT_CL_RESET,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        break;
#endif       
    case MSG_PROMPTWIN_OK:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMIAPIPDP_ClearFlowrate(dual_sys);
//        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : handle message of sim list window 
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectForGprsCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    if (isReturnOK)
    {
            MMK_CreateWin((uint32*)MMICL_GPRS_FLOWRATE_WIN_TAB, (ADD_DATA)dual_sys);
    }

    return recode;

}
#endif

/*****************************************************************************/
// 	Description : the gprs flowrate size
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
BOOLEAN FlowrateFormSize(
                        MMI_STRING_T    *str_dst,        //out.
                        uint32            size
                        )
{
    uint8 display_str[MMICL_DSP_STR_LEN] = {0};
    uint16 display_str_len = 0;
    
    if (((float)size/MMICL_ONE_K_LEN) < 0.100000 && ((float)size/MMICL_ONE_K_LEN) > 0.000000)
    {
        sprintf((char *)display_str, "%.1fk", 0.1);
    }
    else
    {
        sprintf((char *)display_str, "%.1fK", (float)size/MMICL_ONE_K_LEN);
    }

    display_str_len = (uint16)strlen((char*)display_str);

    str_dst->wstr_len = display_str_len;

    str_dst->wstr_ptr = (wchar*)SCI_ALLOCA((str_dst->wstr_len + 1) * sizeof(wchar));
    SCI_MEMSET(str_dst->wstr_ptr, 0x00, ((str_dst->wstr_len + 1) * sizeof(wchar)));

    MMI_STRNTOWSTR(str_dst->wstr_ptr,
                                str_dst->wstr_len,
                                (uint8 *)display_str,
                                display_str_len,
                                display_str_len);
    
    return TRUE;
}


#ifdef MMI_PDA_SUPPORT
LOCAL void SetGprsFlowrateWindow(MMI_CTRL_ID_T ctrl_id,MN_DUAL_SYS_E  dual_sys)
{
    uint16   index_ptr = 0;
    MMIPDP_FLOWRATE_T   gprs_flowrate = {0};
    MMI_STRING_T       last_sent = {0};
    MMI_STRING_T       last_recv = {0};
    MMI_STRING_T       total_send = {0};
    MMI_STRING_T       total_recv = {0};
    MMIAPIPDP_GetFlowrate(&gprs_flowrate,dual_sys);
    FlowrateFormSize(&last_sent,gprs_flowrate.last_send);
    SetHeadAndContentInRText(TXT_BROWSER_LAST_SENT, ctrl_id, last_sent, &index_ptr);
    SCI_FREE(last_sent.wstr_ptr);

    FlowrateFormSize(&last_recv,gprs_flowrate.last_recv);
    SetHeadAndContentInRText(TXT_BROWSER_LAST_RECV, ctrl_id, last_recv, &index_ptr);
    SCI_FREE(last_recv.wstr_ptr);

    FlowrateFormSize(&total_send,gprs_flowrate.total_send);
    SetHeadAndContentInRText(TXT_BROWSER_TOTAL_SENT, ctrl_id, total_send, &index_ptr);
    SCI_FREE(total_send.wstr_ptr);

    FlowrateFormSize(&total_recv,gprs_flowrate.total_recv);
    SetHeadAndContentInRText(TXT_BROWSER_TOTAL_RECV, ctrl_id, total_recv, &index_ptr);
    SCI_FREE(total_recv.wstr_ptr);
}
#else
/*****************************************************************************/
//  Description : set the gprs flowrate window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGprsFlowrateWindow(MMI_CTRL_ID_T ctrl_id,MN_DUAL_SYS_E  dual_sys)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              index = 0;
    MMI_STRING_T        dst_temp1 = {0};
    MMIPDP_FLOWRATE_T   gprs_flowrate = {0};

    MMIAPIPDP_GetFlowrate(&gprs_flowrate,dual_sys);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));

   
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_LAST_SENT;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.last_send);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_LAST_RECV;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.last_recv);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);

    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_TOTAL_SENT;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.total_send);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_data.res.id = TXT_BROWSER_TOTAL_RECV;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(&dst_temp1, 0, sizeof(MMI_STRING_T));
    FlowrateFormSize(&dst_temp1,gprs_flowrate.total_recv);
    item_data.text_data.buf.len = MIN(dst_temp1.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dst_temp1.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(dst_temp1.wstr_ptr);
}
#endif
#endif

/*****************************************************************************/
//  Description : phonebook callback
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMICL_HandlePBEventFunc(MMIEVENT_PB_EVENT_E event)
{
    if((MMIEVENT_PB_ADD == event)
       || (MMIEVENT_PB_DELETE == event)
       || (MMIEVENT_PB_UPDATE == event))
    {
        if(MMK_IsOpenWin(MMICL_LOG_LIST_DETAIIL_WIN_ID)
#ifdef CALL_TIMES_SUPPORT
           || MMK_IsOpenWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID)
#endif
           )
        {
            MMICL_UpdateCalllogWin(TRUE);
        }
    }
}


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : Istyle_MMITHEME_GetWinClientBothRect
//	Global resource dependence : 
//    Author:chen yao for istyle 20110720
//	Note: 由于istyle风格的TAB移动到屏幕下方，客户区有所变动
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T Istyle_MMITHEME_GetWinClientBothRect(
                                                     MMI_HANDLE_T win_handle
                                                     )
{
	GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_handle);

	client_rect.v_rect.top -= MMI_ISTYLE_TITLE_HEIGHT;
	client_rect.h_rect.top -= MMI_ISTYLE_TITLE_HEIGHT; 

	return client_rect;
}

#if 0

/*****************************************************************************/
//  Description : istyle风格中，有button的"title"，其实是softkey
//  Global resource dependence : none
//  Author :chen yao for istyle 20110720
//  Note: 
/*****************************************************************************/
PUBLIC void Istyle_CreateDialWinTitle(MMI_WIN_ID_T      win_id,MMI_CTRL_ID_T ctrl_id)
{
	GUI_SOFTKEY_T *ctrl_ptr = 0;
	GUI_BOTH_RECT_T softkey_both_rect = Istyle_MMITHEME_GetWinClientBothRect(win_id);
	GUI_BG_T button_bg = {0};
	GUI_BG_T middle_button_bg = {0};
	//GUI_BG_T button_fg = {0};
	//wchar   add_text[] = L" ";
	//uint16 len = MMIAPICOM_Wstrlen(add_text);	
	GUI_FONT_ALL_T     font={CAF_FONT_14,MMI_WHITE_COLOR};
    MMI_HANDLE_T button_ctrl_handle[GUISOFTKEY_BUTTON_NUM] = {0};

	MMK_DestroyDynaCtrl(ctrl_id);
	
	//创建"title"
	GUISOFTKEY_CreateCtrlDyn(win_id, ctrl_id, TXT_CL_CALLS_ALL, TXT_CL_CALLS_ALL, TXT_CL_CALLS_ALL);

	ctrl_ptr = (GUI_SOFTKEY_T *)MMK_GetCtrlPtr(MMK_GetCtrlHandleByWin(win_id,ctrl_id));

	//设置softkey位置
	softkey_both_rect.v_rect.bottom = softkey_both_rect.v_rect.top + MMI_ISTYLE_TITLE_HEIGHT;
	softkey_both_rect.h_rect.bottom = softkey_both_rect.h_rect.top + MMI_ISTYLE_TITLE_HEIGHT;
	GUICTRL_SetBothRect((IGUICTRL_T * )ctrl_ptr,  &softkey_both_rect);

	//设置softkey背景并且draw
	ctrl_ptr->bg_type = GUI_SOFTKEY_BG_IMAGE_ONLY;
	ctrl_ptr->bg_image = IMAGE_ISTYLE_TITLE_BAR;
	ctrl_ptr->rect = softkey_both_rect.v_rect;
	//GUISOFTKEY_Update(win_id,ctrl_id);

	//中间的button其实是当TITLE用，不需要BG
	middle_button_bg.bg_type = GUI_BG_NONE;
	
	button_bg.bg_type = GUI_BG_IMG;
	button_bg.img_id = IMAGE_ISTYLE_TITLE_MENU;
	
	{
		//button layout info
		GUI_BOTH_RECT_T button0_both_rect_ptr ={{MMI_ISTYLE_TITLE_LEFT_BUTTON_X, 0, MMI_ISTYLE_TITLE_LEFT_BUTTON_X+MMI_ISTYLE_TITLE_LEFT_BUTTON_WIDTH-1, 0},
										{MMI_ISTYLE_TITLE_LEFT_BUTTON_X, 0, MMI_ISTYLE_TITLE_LEFT_BUTTON_X+MMI_ISTYLE_TITLE_LEFT_BUTTON_WIDTH-1, 0}};
		GUI_BOTH_RECT_T button1_both_rect_ptr ={{MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X, 0, MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X+MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH-1, 0},
										{MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X, 0, MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X+MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH-1, 0}};
		GUI_BOTH_RECT_T button2_both_rect_ptr ={{MMI_ISTYLE_TITLE_RIGHT_BUTTON_X, 0, MMI_ISTYLE_TITLE_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH-1, 0},
										{MMI_ISTYLE_TITLE_RIGHT_BUTTON_X, 0, MMI_ISTYLE_TITLE_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH-1, 0}};

		button0_both_rect_ptr.v_rect.top = softkey_both_rect.v_rect.top+((MMI_ISTYLE_TITLE_HEIGHT-MMI_ISTYLE_TITLE_BUTTON_HEIGHT)>>1);
		button0_both_rect_ptr.v_rect.bottom =button0_both_rect_ptr.v_rect.top+MMI_ISTYLE_TITLE_BUTTON_HEIGHT-1;
		button1_both_rect_ptr.v_rect.top = button0_both_rect_ptr.v_rect.top;
		button1_both_rect_ptr.v_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		button2_both_rect_ptr.v_rect.top = button0_both_rect_ptr.v_rect.top;
		button2_both_rect_ptr.v_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		
		button0_both_rect_ptr.h_rect.top = button0_both_rect_ptr.v_rect.top;
		button0_both_rect_ptr.h_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		button1_both_rect_ptr.h_rect.top = button0_both_rect_ptr.v_rect.top;
		button1_both_rect_ptr.h_rect.bottom =button0_both_rect_ptr.v_rect.bottom;
		button2_both_rect_ptr.h_rect.top = button0_both_rect_ptr.v_rect.top;
		button2_both_rect_ptr.h_rect.bottom =button0_both_rect_ptr.v_rect.bottom;

        button_ctrl_handle[0] = GUIBUTTONSOFTKEY_GetButtonHandle(win_id, 0);
        button_ctrl_handle[1] = GUIBUTTONSOFTKEY_GetButtonHandle(win_id, 1);
        button_ctrl_handle[2] = GUIBUTTONSOFTKEY_GetButtonHandle(win_id, 2);

		GUIBUTTON_SetFont(button_ctrl_handle[0], &font);
		GUIBUTTON_SetFont(button_ctrl_handle[1], &font);
		GUIBUTTON_SetFont(button_ctrl_handle[2], &font);
		
		//设置softkey的3个button
		if(MMICL_ALL_CHILD_WIN_ID == win_id)
		{
			GUI_BG_T all_button_bg = {GUI_BG_IMG, 0, 0, 0, FALSE};
			GUI_BG_T misscall_button_bg = {GUI_BG_IMG, 0, 0, 0, FALSE};

			GUI_BG_T misscall_button_fg = {GUI_BG_IMG, 0, 0, 0, FALSE};
			GUI_BG_T delete_button_fg = {GUI_BG_IMG, 0, 0, 0, FALSE};
			
			//全部通话
			button0_both_rect_ptr.v_rect.left = MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X;
			button0_both_rect_ptr.v_rect.right= MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			button0_both_rect_ptr.h_rect.left = MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X;
			button0_both_rect_ptr.h_rect.right= MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			all_button_bg.img_id = IMAGE_ISTYLE_ALLCALL_OFF;
			
			GUIBUTTON_SetTextId(button_ctrl_handle[0], TXT_CL_CALLS_ALL);
			GUIBUTTON_SetRect(button_ctrl_handle[0], &button0_both_rect_ptr.v_rect);
			GUICTRL_SetBothRect(MMK_GetCtrlPtr(button_ctrl_handle[0]), &button0_both_rect_ptr);
			GUIBUTTON_SetBg(button_ctrl_handle[0], &all_button_bg);
			GUIBUTTON_SetPressedBg(button_ctrl_handle[0], &all_button_bg);
			
			//未接来电
			button1_both_rect_ptr.v_rect.left = MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X;
			button1_both_rect_ptr.v_rect.right= MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			button1_both_rect_ptr.h_rect.left = MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X;
			button1_both_rect_ptr.h_rect.right= MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;
			misscall_button_bg.img_id = IMAGE_ISTYLE_MISSCALL_ON;
			misscall_button_fg.img_id = IMAGE_ISTYLE_MISSCALL_OFF;
			GUIBUTTON_SetTextId(button_ctrl_handle[1], TXT_CC_MISS_CALL);
			GUIBUTTON_SetRect(button_ctrl_handle[1], &button1_both_rect_ptr.v_rect);
			GUICTRL_SetBothRect(MMK_GetCtrlPtr(button_ctrl_handle[1]), &button1_both_rect_ptr);
			GUIBUTTON_SetBg(button_ctrl_handle[1], &misscall_button_bg);
			GUIBUTTON_SetPressedBg(button_ctrl_handle[1], &misscall_button_fg);
			
			//清除
			button2_both_rect_ptr.v_rect.left = MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X;
			button2_both_rect_ptr.v_rect.right= MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH-1;
			button2_both_rect_ptr.h_rect.left = MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X;
			button2_both_rect_ptr.h_rect.right= MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X+MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH-1;
			
			delete_button_fg.img_id = IMAGE_ISTYLE_TITLE_MENU_DOWN;
			GUIBUTTON_SetTextId(button_ctrl_handle[2], TXT_DELETE);
			GUIBUTTON_SetRect(button_ctrl_handle[2], &button2_both_rect_ptr.v_rect);
			GUICTRL_SetBothRect(MMK_GetCtrlPtr(button_ctrl_handle[2]), &button2_both_rect_ptr);
			GUIBUTTON_SetBg(button_ctrl_handle[2], &button_bg);
			GUIBUTTON_SetPressedBg(button_ctrl_handle[2], &delete_button_fg);
		}
	}
}

/*****************************************************************************/
// 	Description : set call log title button background
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_SetButtonBG(MMI_CTRL_ID_T ctrl_id, uint32 button_index)
{
    GUI_BG_T button_bg_left = {GUI_BG_IMG, 0, 0, 0, FALSE};
	GUI_BG_T button_bg_mid = {GUI_BG_IMG, 0, 0, 0,FALSE};
    //MMI_TEXT_ID_T text_id = 0;
    GUI_SOFTKEY_T *ctrl_ptr = (GUI_SOFTKEY_T *)MMK_GetCtrlPtr(ctrl_id);
    MMI_HANDLE_T button_ctrl_handle[GUISOFTKEY_BUTTON_NUM] = {0};
	GUI_BOTH_RECT_T softkey_both_rect = Istyle_MMITHEME_GetWinClientBothRect(MMICL_ALL_CHILD_WIN_ID);
	
    GUI_RECT_T button_rect = {0, 0, 0, 0};
	
	button_rect.top = softkey_both_rect.v_rect.top+((MMI_ISTYLE_TITLE_HEIGHT - MMI_ISTYLE_TITLE_BUTTON_HEIGHT)>>1);
	button_rect.bottom = button_rect.top + MMI_ISTYLE_TITLE_BUTTON_HEIGHT - 1;

    switch (button_index)
    {
    case 0:
        //全部通话
        button_rect.left = MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X;
        button_rect.right= MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;

		button_bg_left.img_id = IMAGE_ISTYLE_ALLCALL_OFF;
		button_bg_mid.img_id = IMAGE_ISTYLE_MISSCALL_ON;
        break;
        
    case 1:
        //未接来电
        button_rect.left = MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X;
        button_rect.right= MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X+MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH-1;

		button_bg_left.img_id = IMAGE_ISTYLE_ALLCALL_ON;
		button_bg_mid.img_id = IMAGE_ISTYLE_MISSCALL_OFF;
        break;
        
    case 2:
        //清除
        button_rect.left = MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X;
        button_rect.right= MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X + MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH-1;
        break;

    default:
        return;
    }
	/*更换button背景，显示选中或者未选*/
    if (PNULL != ctrl_ptr)
    {
        button_ctrl_handle[0] = GUIBUTTONSOFTKEY_GetButtonHandle(ctrl_ptr->win_handle, 0);
        button_ctrl_handle[1] = GUIBUTTONSOFTKEY_GetButtonHandle(ctrl_ptr->win_handle, 1);
        button_ctrl_handle[2] = GUIBUTTONSOFTKEY_GetButtonHandle(ctrl_ptr->win_handle, 2);
    }
    GUIBUTTON_SetBg(button_ctrl_handle[0], &button_bg_left);
	GUIBUTTON_SetBg(button_ctrl_handle[1], &button_bg_mid);
}
#endif
#if defined MMI_ISTYLE_SUPPORT
/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleAllChildWindow(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T      list_handle = 0;
    MMI_CTRL_ID_T list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMI_WIN_ID_T real_win_id = MMK_GetWinId(win_id); 
	MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);

    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);

    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {        
            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"Istyle_HandleAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10080_112_2_18_2_11_48_98,(uint8*)"");
                MMK_CloseParentWin(info_ptr);
                return recode;
            }

            //Istyle_CreateDialWinTitle(win_id, MMICALLLOG_SOFTKEY_CTRL_ID);

            GUILIST_SetTextFont(CalllogPBData_GetListHandle(), SONG_FONT_14, MMI_BLACK_COLOR);

            s_cur_sel_log_list_type = MMICL_CALL_ALL;

            MMK_SetAtvCtrl(win_id, CalllogPBData_GetListHandle());
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            //Istyle_MoveChildWinAboveTab(MMK_ConvertIdToHandle(win_id));
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
        }
        break;

	 /*add for istyle*/
    case MSG_CTL_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle())
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
            }
        }
#endif		
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (MMICL_ALL_SELECT_CHILD_WIN_ID == real_win_id)
        {
            break;
        }
        else
        {
            if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)    
            {
                HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
            }
		}
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_GET_FOCUS:
        if (MMICL_ALL_SELECT_CHILD_WIN_ID == real_win_id)
        {
            GUIWIN_SetTitleIsVisible(win_id, TRUE, FALSE);
    		//cancel and ok
    		//取消sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_CANCEL, FALSE);
    		//完成sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_FINISH, FALSE);
        }
        else
        {
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
        	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
            GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_LEFT, IMAGE_ISTYLE_TITLE_RETURN, IMAGE_ISTYLE_TITLE_RETURN_DOWN, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_LEFT, MSG_APP_CANCEL, 0);

            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
            
            GUIWIN_SetTitleIsVisible(win_id, TRUE, TRUE);
        }
        break;

    case MSG_CTL_TAB_SWITCH:
        //隐私检查
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //需要进行隐私检查
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
// 	Description : to handle select call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandleSelectAllChildWindow(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T      list_handle = 0;
    MMI_CTRL_ID_T list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
	MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);

    list_handle = MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id);

    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {        
            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"Istyle_HandleAllChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10080_112_2_18_2_11_48_98,(uint8*)"");
                MMK_CloseParentWin(info_ptr);
                return recode;
            }

            //Istyle_CreateDialWinTitle(win_id, MMICALLLOG_SOFTKEY_CTRL_ID);

            GUILIST_SetTextFont(CalllogPBData_GetListHandle(), SONG_FONT_14, MMI_BLACK_COLOR);

            s_cur_sel_log_list_type = MMICL_CALL_ALL;

            MMK_SetAtvCtrl(win_id, CalllogPBData_GetListHandle());
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            //Istyle_MoveChildWinAboveTab(MMK_ConvertIdToHandle(win_id));
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
        }
        break;

	 /*add for istyle*/
    case MSG_CTL_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle())
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
            }
        }
#endif		
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_GET_FOCUS:
            GUIWIN_SetTitleIsVisible(win_id, TRUE, FALSE);
    		//cancel and ok
    		//取消sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_CANCEL, FALSE);
    		//完成sk
    		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    		GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_FINISH, FALSE);
        break;

    case MSG_CTL_TAB_SWITCH:
        //隐私检查
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //需要进行隐私检查
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : to handle refused call log child window message
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  Istyle_HandlePBDetailCalllogChildWindow(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
//    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LIST_CTRL_ID;
    static BOOLEAN  s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
//    MMI_HANDLE_T    list_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_ALL_LIST_CTRL_ID);

    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;

	SCI_TRACE_LOW("[MMICL] Istyle_HandlePBDetailCalllogChildWindow msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"Istyle_HandlePBDetailCalllogChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10215_112_2_18_2_11_49_99,(uint8*)"");
                return recode;
            }

            //Istyle_CreateDialWinTitle(win_id, MMICALLLOG_SOFTKEY_CTRL_ID);

            GUILIST_SetTextFont(CalllogPBData_GetListHandle(), SONG_FONT_14, MMI_BLACK_COLOR);

            s_cur_sel_log_list_type = MMICL_CALL_ALL;

            MMK_SetAtvCtrl(win_id, CalllogPBData_GetListHandle());
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            //Istyle_MoveChildWinAboveTab(MMK_ConvertIdToHandle(win_id));
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            s_cur_sel_log_list_type = MMICL_CALL_ALL;
            SetLogListItem();
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
        }
        else
        {
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
    	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
        }
        GUIWIN_SetTitleIsVisible(win_id, TRUE, TRUE);
        
        break;

	 /*add for istyle*/
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle())
        {
            if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                if (s_select_return_info.need_select_return)
                {
                    if (MMK_IsOpenWin(s_select_return_info.return_win_id))
                    {
                        MMK_SendMsg(s_select_return_info.return_win_id, MSG_CL_SINGLE_SELECT_LIST_RETURN, PNULL);
                    }
                    MMK_CloseWin(MMICL_MAIN_MENU_WIN_ID);
                }
                else
                {
                    CL_OpenCallTimeListOrDetailWin(arraycall_info);
                }
            }
        }
#endif		
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
      
       if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)    
        {
            HandleRingBackByType(CC_CALL_DEFAULT_CALL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_GET_FOCUS:
		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
    	GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);

        break;

    case MSG_CTL_TAB_SWITCH:
        //隐私检查
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //需要进行隐私检查
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
 
     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}



#endif
#ifdef CALL_TIMES_SUPPORT
/*****************************************************************************/
// 	Description : load call times list into listbox
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void AppendCallTimeListItem(
                             MMI_CTRL_ID_T	            ctrl_id,
                             MMICL_CALL_TYPE_E              call_type,
                             MMICL_CALL_ARRAY_BCDINFO_T     *arraycall_info,
                             uint16     selected_call_log_index
                             )
{
    GUILIST_ITEM_T item_t                          = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data                       = {0};/*lint !e64*/

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN + MAX_CALL_TIME_LEN+1]  ={0};     

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};
#endif

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};
#ifndef MMI_CL_MINI_SUPPORT    
    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
#endif


#ifdef TOUCH_PANEL_SUPPORT
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT; //GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;
#else
    item_t.item_style = GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;
#endif
#else
#ifdef MMI_CL_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT;
#endif

#endif
#endif
    
    item_t.item_data_ptr = &item_data;
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT;

    //set softkey
    if ( s_select_return_info.need_select_return )
    {
        item_data.softkey_id[0] = TXT_COMMON_OK;
    }
    else
    {
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_DELETE;
#else
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
#endif
    }
    
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
    //set list item
    if ( arraycall_info->call_bcdinfo[selected_call_log_index].call_times < 1
    #ifdef MMI_PDA_SUPPORT
        || ((call_type != MMICL_CALL_ALL)&& (call_type != MMICL_CALL_MATCHED) && (0 == GetCurrTypeNum( call_type,arraycall_info)) )
    #endif
        )  
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_BLANK;
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
    else
    {
        uint16 i = 0;
        MMICL_CALL_BCDINFO_T     *callinfo=PNULL;

        for ( i = arraycall_info->call_bcdinfo[selected_call_log_index].call_times; i > 0 ; i -- )
        {
            callinfo = &arraycall_info->call_bcdinfo[selected_call_log_index];

            //Init Text Contents     Name/Number, SimName, Date, Time
            SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
            name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
            sim_name_str.wstr_ptr = sim_name_wstr;
            sim_name_str.wstr_len = 0;
#endif
            SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
            datetime_str.wstr_ptr = datetime_wstr;
            SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
            SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));            
            time_str.wstr_ptr = wstr_time_str;
//fix by bingjie.chen
#ifndef MMI_CL_MINI_SUPPORT
            //Add Image content
            image_id = GetCallTypeImageID(callinfo);
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
            if (!callinfo->is_emergency_call)
            {
                MMIAPICL_GetSimNameContent(sim_name_wstr,GUILIST_STRING_MAX_NUM,callinfo->dual_sys);
            }
            sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
            item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
#ifdef BT_DIALER_SUPPORT
            if (MMICL_BT_CALL_MISSED == call_type
                || MMICL_BT_CALL_RECEIVED == call_type
                || MMICL_BT_CALL_DIALED == call_type
                )
            {
                item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr = sim_name_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len = 0;
            }
            else
#endif
            {
                item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
            }
#else
            //Single card don't need sim name
#endif

            //Add Contact name/or call number content no Calltimes, use numertype
            if (callinfo->is_emergency_call)
            {
                uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};

                //get number
                MMICL_GenDispNumber(callinfo->call_type,
                    callinfo->number_type,
                    callinfo->number_len,
                    callinfo->number, 
                    tele_num,
                    PHONE_NUM_MAX_LEN + 2);
                name_number_str.wstr_len = (uint16)strlen((char*)tele_num);
                name_number_str.wstr_ptr = name_number_wchar;//(uint8 *)tele_num;
                MMI_STRNTOWSTR( name_number_wchar, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, name_number_str.wstr_len, name_number_str.wstr_len );            
                name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
            }
            else
            {
                if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,FALSE,TRUE))
                {
                    name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
                    item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
                    item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
                }
            }
#endif       
            if(callinfo->call_times>0)
            {
                //Add date and time content
                if (0<callinfo->call_start_time[callinfo->call_times -1])
                {
//add by bingjie.chen
#ifdef MMI_CL_MINI_SUPPORT
                 
                    GetDateContent(&datetime_str, GUILIST_STRING_MAX_NUM,callinfo,i-1,TRUE);
                    item_data.item_content[0].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[0].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                    item_data.item_content[0].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
                    
                    GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,i-1);
                    item_data.item_content[1].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[1].item_data.text_buffer.wstr_len = time_str.wstr_len;
                    item_data.item_content[1].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
#else
                    GetDateContent(&datetime_str, GUILIST_STRING_MAX_NUM,callinfo,i-1,FALSE);
                    item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                    item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
                    
                    GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,i-1);
                    item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                    item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
#endif
                }
            }
            //item_t.user_data = (uint32)j++;//remember item index of 
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
            if (MMITHEME_IsIstyle())        //IPHONE set time and to blue color
            {
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].font_color_id=MMITHEME_COLOR_BLUE;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].is_default =TRUE;
            }
#endif    
            
            GUILIST_AppendItem( ctrl_id, &item_t );
        }
    }
}

/*****************************************************************************/
// 	Description : to set call time list item
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListItem(MMI_CTRL_ID_T call_times_list_ctrl_id)
{
    uint16 sel_item_index = 0;
    uint16 current_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_TYPE_E call_type = s_cur_sel_log_list_type;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10476_112_2_18_2_11_50_100,(uint8*)"");
        return;
    }
    
    current_index = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        call_type = s_match_call_info_ptr->call_info[s_mmicl_match_item_index].call_type;
    }

    //load log list
    GUILIST_SetMaxItem(call_times_list_ctrl_id, MMICL_MAX_CALL_TIMES, FALSE );//max item MMICL_MAX_CALL_TIMES
    GUILIST_RemoveAllItems(call_times_list_ctrl_id);
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle() && MMICL_CALL_ALL == s_cur_sel_log_list_type)
    {
    //	Istyle_AppendLogListItem(call_times_list_ctrl_id, s_cur_sel_log_list_type, arraycall_info,sel_item_index);
    }
    //else
#endif    
    {
    	AppendCallTimeListItem(call_times_list_ctrl_id, call_type, arraycall_info,sel_item_index);
    }
#else
    AppendCallTimeListItem(call_times_list_ctrl_id, call_type, arraycall_info,sel_item_index);
#endif
	
    GUILIST_SetCurItemIndex(call_times_list_ctrl_id, current_index);//set at first index
    
    // (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
}

/*****************************************************************************/
// 	Description : set call time list title window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimeListTitle(MMI_WIN_ID_T win_id)
{
    uint16   sel_item_index =0;
    MMICL_CALL_ARRAY_BCDINFO_T * arraycall_info =PNULL;
    wchar title_string_ptr[PHONE_NUM_MAX_LEN +1] ={0};
    uint16 title_string_len =0;
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10526_112_2_18_2_11_50_101,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    /*号码为空*/
    if ( !arraycall_info->call_bcdinfo[sel_item_index].is_num_present )  //show number
    {
        GUIWIN_SetTitleTextId(win_id,TXT_UNKNOW_NUM, FALSE);
    }
    else if(arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        GUIWIN_SetTitleTextId(win_id,TXT_KL_PSW_EMERGENCY_CALL, FALSE);//紧急号码
    }
    else    //from PB first,then show num
    {
        uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
        MMI_STRING_T         name_str ={0};
        uint16  tele_num_len =0;
        BOOLEAN find_result =FALSE;
        wchar str_ptr[MMIPB_MAX_STRING_LEN + 1]     = {0};
        
        SCI_MEMSET(tele_num, 0x00, sizeof(tele_num));

        //get number
        tele_num_len = MMICL_GenDispNumber(arraycall_info->call_bcdinfo[sel_item_index].call_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_len,
            arraycall_info->call_bcdinfo[sel_item_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
 
        name_str.wstr_ptr = str_ptr;
        find_result = MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_str, FALSE);

        //no name in pb
        if ( !find_result || 0 == name_str.wstr_len )
        {
            MMI_STRNTOWSTR( title_string_ptr,
                    PHONE_NUM_MAX_LEN,
                    (uint8 *)tele_num,
                    PHONE_NUM_MAX_LEN,
                    tele_num_len);
            title_string_len = tele_num_len;
        }
        else//there is name
        {
            title_string_len = name_str.wstr_len;
            MMI_WSTRNCPY (title_string_ptr, 
                PHONE_NUM_MAX_LEN, 
                name_str.wstr_ptr, 
                PHONE_NUM_MAX_LEN, 
                title_string_len);
        }
        GUIWIN_SetTitleText(win_id,title_string_ptr,title_string_len,FALSE);
    }
    
    SCI_FREE(arraycall_info);
}


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//	Description : the handle function of call time list delete window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleCallTimeListDeleteSelectedQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
//        DeleteAllCallLog(s_cur_sel_log_list_type);
        DeleteCurrentRecord();
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);  //close 
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}
#endif
#ifdef TOUCH_PANEL_SUPPORT

//layout info,will move to mmiposition.h later
#ifdef MAINLCD_SIZE_320X480
#define CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT      62
#define CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 52
#define CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN 5
#define CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#define CALLTIME_LIST_BUTTON_FORM_HEIGHT     52
#define CALLTIME_LIST_BUTTON_HEIGHT          40   
#elif (defined MAINLCD_SIZE_240X320) || (defined MAINLCD_SIZE_240X400)
#define CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT     50
#define CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 43
#define CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN 3
#define CALLTIME_LIST_BUTTON_FORM_HEIGHT     45
#define CALLTIME_LIST_BUTTON_HEIGHT          40   
#define CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#else
#define CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT     50
#define CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 43
#define CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN 3
#define CALLTIME_LIST_BUTTON_FORM_HEIGHT     45
#define CALLTIME_LIST_BUTTON_HEIGHT          40   
#define CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#endif

#define CALLTIME_LIST_BUTTON_FORM_BG_COLOR  GUI_RGB2RGB565(234,234,234)

/*****************************************************************************/
// 	Description : init title bar of call time list window
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListWinTitleBar(MMI_WIN_ID_T win_id)
{
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    GUIFORM_CHILD_WIDTH_T               sbs_width = {0};
    GUIFORM_CHILD_HEIGHT_T              sbs_height = {0};
    GUI_BG_T                            bg_info = {0};
//    uint16                              image_width = 0;
//    uint16                              image_height = 0;
    MN_DUAL_SYS_E                       dual_sys = 0;
    MMICL_CALL_ARRAY_BCDINFO_T          *arraycall_info = PNULL;
    uint16                              sel_item_index = 0;
    uint16                              hor_space = 10;
    uint16                              ver_space = 0;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10678_112_2_18_2_11_50_102,(uint8*)"");
        return;
    }
//    if (g_pbcalllog_data == PNULL)
//    {
//        return;
//    }
//    if (g_pbcalllog_data->matched_contact==PNULL)
    {
	//        return;
    }
    GUIFORM_IsSlide(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBg(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE);
    GUILIST_PermitBorder(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE);
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_CL_DETAIL_TITLE_BG;
    GUIFORM_SetBg(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, &bg_info);
    
    sbs_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    sbs_height.add_data = CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT;
	GUIFORM_SetChildHeight(MMICL_CALL_TIMES_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, &sbs_height);
    GUIFORM_SetMargin(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN);
    GUIFORM_SetSpace(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID,&hor_space,&ver_space);

    sbs_height.add_data = CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT;
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID, &sbs_height);
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, &sbs_height);
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, &sbs_height);  

    //Set Width
    sbs_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    sbs_width.add_data = CALLTIME_LIST_CONTACT_INFO_BAR_ICON_HEIGHT; //Same as height
    GUIFORM_SetChildWidth(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID,MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID,&sbs_width);
    sbs_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
	sbs_width.add_data = (CALLTIME_LIST_CONTACT_INFO_BAR_LABEL_WID*100/MMI_MAINSCREEN_WIDTH);
    GUIFORM_SetChildWidth(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID,MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID,&sbs_width);

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    /*号码为空*/
    if ( !arraycall_info->call_bcdinfo[sel_item_index].is_num_present )  //show number
    {
        GUILABEL_SetTextById(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, TXT_UNKNOW_NUM, FALSE);
        //Set ICON
        ctrl_info.is_ctrl_id     = TRUE;
        ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
        ctrl_info.ctrl_ptr       = PNULL;
        data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;
        display_info.is_update   = FALSE;
        display_info.is_disp_one_frame = TRUE;
        display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        SCI_FREE(arraycall_info);
        return;

    }
    else if(arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        GUILABEL_SetTextById(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID, TXT_KL_PSW_EMERGENCY_CALL, FALSE);
        //Set ICON
        ctrl_info.is_ctrl_id     = TRUE;
        ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
        ctrl_info.ctrl_ptr       = PNULL;
        data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;
        display_info.is_update   = FALSE;
        display_info.is_disp_one_frame = TRUE;
        display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        SCI_FREE(arraycall_info);

        return;

    }

    //If number in contact.
    if (CheckCurrentNumberInContact())
    {
        MMIPB_BCD_NUMBER_T pb_num = {0};
        wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
        uint32 custom_photo_name_len = 0;
        uint32 photo_file_size = 0;
        MMI_STRING_T    name_strid = {0};
        wchar name_wstr[PHONE_NUM_MAX_LEN + 1]={0};
        GUIANIM_FILE_INFO_T                 file_info  = {0};
        BOOLEAN find_result = FALSE;

        
        SCI_MEMSET(&pb_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));
        pb_num.number_len = arraycall_info->call_bcdinfo[sel_item_index].number_len;
        pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[sel_item_index].number_type,
          arraycall_info->call_bcdinfo[sel_item_index].number_plan);
        MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
          arraycall_info->call_bcdinfo[sel_item_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
          pb_num.number_len);
        if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
        {
            ctrl_info.is_wallpaper_ctrl = FALSE;
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
    
            display_info.is_update = FALSE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            display_info.is_disp_one_frame = TRUE;
            display_info.is_crop_rect = TRUE;
            display_info.is_zoom = TRUE;
            display_info.is_syn_decode = TRUE;
            file_info.full_path_wstr_ptr = custom_photo_name;
            file_info.full_path_wstr_len = custom_photo_name_len;
            GUIANIM_SetParam(&ctrl_info,PNULL,&file_info,&display_info);
        }
        else
        {
//            uint8                               group_id = PB_GROUP_ALL;

            //Get SIM ICON
//            MMIPB_GetAllGroupIDs(g_pbcalllog_data->matched_contact->union_t.group, &group_id, 1);
//		    if(group_id >= MMIPB_MAX_GROUP_NUM)
//		    {
//			    group_id =PB_GROUP_ALL;
//		    }
            
            //Set ICON

            ctrl_info.is_ctrl_id     = TRUE;
            ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
            ctrl_info.ctrl_ptr       = PNULL;
            data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;//AAA_GetImageIcon(group_id);
            display_info.is_update   = FALSE;
            display_info.is_disp_one_frame = TRUE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        }      
        //Set Label
        name_strid.wstr_ptr = name_wstr;
        find_result = MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_strid, FALSE);
        GUILABEL_SetText(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID , &name_strid, FALSE);

        //Set Button
        GUIBUTTON_SetVisible(MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, FALSE,FALSE);
    }
    else
    {
        MMI_STRING_T    number_strid={0};
        wchar number_wstr[PHONE_NUM_MAX_LEN + 1]={0};
        uint8 number_str[PHONE_NUM_MAX_LEN + 1] ={0};
    
        //Set ICON
        ctrl_info.is_ctrl_id     = TRUE;
        ctrl_info.ctrl_id        = MMICL_CALL_TIMES_TITLE_ICON_CTRL_ID;
        ctrl_info.ctrl_ptr       = PNULL;
        data_info.img_id         = IAMGE_PB_DEFAULT_PHOTO;
        display_info.is_update   = FALSE;
        display_info.is_disp_one_frame = TRUE;
        display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    
        //Set label text 
        GetDisplayPhoneNum(number_str,PHONE_NUM_MAX_LEN+1,&dual_sys);
        MMI_STRNTOWSTR(number_wstr,PHONE_NUM_MAX_LEN+1,number_str,PHONE_NUM_MAX_LEN+1,PHONE_NUM_MAX_LEN+1);
        number_strid.wstr_ptr = number_wstr;
        number_strid.wstr_len = MMIAPICOM_Wstrlen(number_wstr);
        GUILABEL_SetText(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID , &number_strid, FALSE);

        GUIBUTTON_SetVisible(MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID, TRUE,FALSE);
    }

    SCI_FREE(arraycall_info);
    return;
}
#endif
/*****************************************************************************/
// 	Description : init controls of call time list window
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimeListWinCtrl(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_HEIGHT_T  height_info={0};
    uint16 ver_space = 0;
    uint16 hor_space = 0;
#ifdef TOUCH_PANEL_SUPPORT
    GUI_BORDER_T    btn_border= {1,MMI_GRAY_WHITE_COLOR,GUI_BORDER_NONE};
    GUI_BG_T        bg_info = {0};
#endif

    //Set Main Form
    GUIFORM_IsSlide(MMICL_CALL_TIMES_FORM_CTRL_ID,FALSE);
    GUIFORM_IsSlide(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,FALSE);
    GUIFORM_PermitChildBg(MMICL_CALL_TIMES_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBorder(MMICL_CALL_TIMES_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMICL_CALL_TIMES_FORM_CTRL_ID, 0);
    GUIFORM_SetSpace(MMICL_CALL_TIMES_FORM_CTRL_ID, &ver_space, &hor_space);
    
#ifdef MMI_PDA_SUPPORT
    //Set Title bar
    if (!MMITHEME_IsIstyle())
    {
    
        SetCallTimeListWinTitleBar(win_id);         //Android Style
        //GUILABEL_SetBackgroundColor(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID,MMI_RED_COLOR);
    }    
#if defined(MMI_ISTYLE_SUPPORT)	
    else
    {
        height_info.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height_info.add_data = 0;
        GUIFORM_SetChildHeight(MMICL_CALL_TIMES_FORM_CTRL_ID, MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, &height_info);
        GUIFORM_SetVisible(MMICL_CALL_TIMES_TITLE_FORM_CTRL_ID, FALSE,FALSE);   //Istyle Mode
        SetCallTimeListTitle(win_id);
    }
#endif	
#else    
#endif
#ifdef TOUCH_PANEL_SUPPORT
    //Set Btn Form BG
    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color = CALLTIME_LIST_BUTTON_FORM_BG_COLOR;

    GUIFORM_SetBg(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,&bg_info);
    
    GUIFORM_PermitChildBorder(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, 0);
    GUIFORM_SetSpace(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID, &ver_space, &hor_space);
    //Set Btn Border
    height_info.type = GUIFORM_CHILD_HEIGHT_FIXED;
    height_info.add_data = CALLTIME_LIST_BUTTON_FORM_HEIGHT;  //Gui requirements

    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_CALL_CTRL_ID, &btn_border,FALSE);
#ifdef MMI_IP_CALL_SUPPORT
    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID, &btn_border,FALSE);
#endif
    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_MSG_CTRL_ID, &btn_border,FALSE);
    GUIBUTTON_SetBorder(MMICL_CALL_TIMES_BTN_DEL_CTRL_ID, &btn_border,FALSE);

    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_CALL_CTRL_ID, &height_info);
#ifdef MMI_IP_CALL_SUPPORT
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID, &height_info);
#endif
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_MSG_CTRL_ID, &height_info);
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_BTN_FORM_CTRL_ID,MMICL_CALL_TIMES_BTN_DEL_CTRL_ID, &height_info);

#endif
    //Set list ctrl
    height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(MMICL_CALL_TIMES_FORM_CTRL_ID, MMICL_CALL_TIMES_MENU_CTRL_ID, &height_info);
    GUILIST_PermitBorder(MMICL_CALL_TIMES_MENU_CTRL_ID, FALSE);

    return;
}

/*****************************************************************************/
// 	Description : to handle call time list window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimeListWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   call_times_list_ctrl_id = MMICL_CALL_TIMES_MENU_CTRL_ID;
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info =PNULL;
    MMI_WIN_ID_T	query_win_id =MMICL_QUERY_WIN_ID;
    uint16 cur_selection=0;
#ifndef  MMI_MULTI_GREEN_KEY_SINGLE
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#endif
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
     
    switch(msg_id)
    {   
    case MSG_OPEN_WINDOW:
        {

            SetCallTimeListWinCtrl(win_id);
            GUILIST_SetMaxItem(call_times_list_ctrl_id, MMICL_MAX_CALL_TIMES, FALSE );
            /*title*/
//            SetCallTimeListTitle(win_id);
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandleDialedChildWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_10951_112_2_18_2_11_51_103,(uint8*)"");
                return recode;
            }
#ifndef MMI_PDA_SUPPORT
            SetCallTimeListTitle(win_id);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif

            MMK_SetAtvCtrl(win_id, call_times_list_ctrl_id);
        }
        break;

    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
#endif
        SetCallTimeListItem(call_times_list_ctrl_id);
        break;
    case MSG_GET_FOCUS:
        MMK_SetAtvCtrl(win_id, call_times_list_ctrl_id);
#ifdef MMI_PDA_SUPPORT
    if (!MMITHEME_IsIstyle())
    {
        SetCallTimeListWinTitleBar(win_id);         //Android Style
        //GUILABEL_SetBackgroundColor(MMICL_CALL_TIMES_TITLE_LABEL_CTRL_ID,MMI_RED_COLOR);
    }    
#else
        SetCallTimeListTitle(win_id);
#endif
        break;
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
    {
        //add anim into list item
        GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        if(PNULL != need_item_content_ptr)
        {
               DrawListItemIconData(arraycall_info,need_item_content_ptr);                
        }
        break;
    }
    case MSG_NOTIFY_TPMOVE_LEFT_SIDE:
        HandleAnswerSms();
        break;
    case MSG_NOTIFY_TPMOVE_RIGHT_SIDE:
        HandleRingBackByType(CC_CALL_NORMAL_CALL);
        break;
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_CTL_LIST_LONGOK:
#endif
    case MSG_APP_MENU:
#ifdef MMI_PDA_SUPPORT
        
#else
#ifdef MMI_CL_MINI_SUPPORT
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
#else
        cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
        MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_LIST_OPTION_WIN_TAB, (ADD_DATA)cur_selection);
#endif
#endif
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT
        
#else
//fixed by bingjie.chen
        cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
        MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_DETAIIL_WIN_TAB, (ADD_DATA)cur_selection);       
#endif        
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = 0;
            MMIPB_BCD_NUMBER_T bcd_number = {0};
            

            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
            GetPartyPhoneNum(&bcd_number);
            switch(ctrl_id)
            {
            case MMICL_CALL_TIMES_BTN_CALL_CTRL_ID:
                HandleRingBackByType(CC_CALL_NORMAL_CALL);
                break;
                
#ifdef MMI_IP_CALL_SUPPORT
            case MMICL_CALL_TIMES_BTN_IPCALL_CTRL_ID:
                HandleRingBackByType(CC_CALL_IP_CALL);
                break;
#endif

            case MMICL_CALL_TIMES_BTN_MSG_CTRL_ID:
                HandleAnswerSms();
                break;

            case MMICL_CALL_TIMES_BTN_DEL_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleCallTimeListDeleteSelectedQueryWinMsg);
                break;
#ifdef MMI_PDA_SUPPORT
            case MMICL_CALL_TIMES_TITLE_BTN_CTRL_ID:
                MMK_CreatePubListWin((uint32 *)MMICL_CALL_TIME_POP_LIST_WIN_TAB,PNULL);
                break;
#endif

#ifndef MMI_PDA_SUPPORT     //PDA时不需要进入DETAIL界面
            case MMICL_CALL_TIMES_MENU_CTRL_ID:
                MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_DETAIIL_WIN_TAB, (ADD_DATA)cur_selection);
                break;
#endif
            default:
                break;
            }       
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
         
                
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
     case MSG_CLOSE_WINDOW:
        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
        }
        break;
// add by bingjie.chen        

    case MSG_PROMPTWIN_OK: 
#ifdef CALL_TIMES_SUPPORT
         MMIPUB_CloseQuerytWin(&query_win_id);
         cur_selection = GUILIST_GetCurItemIndex(call_times_list_ctrl_id);
	  MMICL_DeleteOneCallTimesItem(s_cur_sel_log_list_type,cur_selection);
         MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
         //MMK_CloseWin(win_id);
         break;

    case MSG_PROMPTWIN_CANCEL:
         MMIPUB_CloseQuerytWin(&query_win_id);
         break;



//add by bingjie.chen
#ifdef  MMI_MULTI_GREEN_KEY_SINGLE
        case MSG_KEYDOWN_GREEN:
        case MSG_KEYDOWN_CALL2:
        case MSG_KEYDOWN_CALL3:
        case MSG_KEYDOWN_CALL4:
             s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
             break;
             
        case MSG_KEYUP_GREEN:
        case MSG_KEYUP_CALL2:
        case MSG_KEYUP_CALL3:
        case MSG_KEYUP_CALL4:
            if(s_is_allow_sim_call[MN_DUAL_SYS_1] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
            {
                HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
            }
            s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
            break;

#else
                
         case MSG_KEYDOWN_CALL1:
         case MSG_KEYDOWN_CALL2:
         case MSG_KEYDOWN_CALL3:
         case MSG_KEYDOWN_CALL4:
              {
                    dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
                    s_is_allow_sim_call[dual_sys] = TRUE;
              }
              break;
                 
         case MSG_KEYUP_CALL1:
         case MSG_KEYUP_CALL2:
         case MSG_KEYUP_CALL3:
         case MSG_KEYUP_CALL4:
             {
                 dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
                 sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX); 
             
                 if(s_is_allow_sim_call[dual_sys] && GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type) > 0)
                 {
                    HandleRingBack(sim_type, FALSE);
                 }
                    s_is_allow_sim_call[dual_sys]= FALSE;
             }
              break;
    
#endif
             
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : to handle call time list option window
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void  SetCallTimesListOptMenuTitle(uint16 call_times_index)
 {
    GUIMENU_TITLE_INFO_T title_info = {0};
    uint8 temp_str[PHONE_NUM_MAX_LEN] = {0};
    wchar temp_wstr_1[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar temp_wstr_2[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar temp_wstr_3[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_TM_T tm = {0};
    uint16   sel_item_index =0;
    MMICL_CALL_ARRAY_BCDINFO_T * arraycall_info =PNULL;
    uint16  times_index = 0;
    MMI_STRING_T title_string ={0};
    uint16 temp_str_len = 0;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetCallTimesListOptMenuTitle call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11100_112_2_18_2_11_51_104,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
    
    times_index = arraycall_info->call_bcdinfo[sel_item_index].call_times -call_times_index -1;
    tm = MMIAPICOM_Second2Tm( arraycall_info->call_bcdinfo[sel_item_index].call_start_time[times_index]);

    /* display date */
    SCI_MEMSET(temp_str,0x00,sizeof(temp_str));
    MMIAPISET_FormatDateStrByDateStyle(tm.tm_year, tm.tm_mon, tm.tm_mday,
        '/', temp_str,PHONE_NUM_MAX_LEN );
    SCI_MEMSET(temp_wstr_1,0x00,sizeof(wchar) * (GUILIST_STRING_MAX_NUM + 1));
    temp_str_len = SCI_STRLEN((char*)temp_str);
    MMI_STRNTOWSTR( (wchar*)temp_wstr_1,
        GUILIST_STRING_MAX_NUM,
        (uint8 *)temp_str,
        PHONE_NUM_MAX_LEN,
        temp_str_len
        );
    
    /*display time*/
    SCI_MEMSET(temp_str,0x00,sizeof(temp_str));
    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,temp_str,PHONE_NUM_MAX_LEN);
    SCI_MEMSET(temp_wstr_2,0x00,sizeof(wchar) * (GUILIST_STRING_MAX_NUM + 1));
    temp_str_len = SCI_STRLEN((char*)temp_str);
    MMI_STRNTOWSTR((wchar*)temp_wstr_2,
        GUILIST_STRING_MAX_NUM,
        (uint8 *)temp_str,
        PHONE_NUM_MAX_LEN,
        temp_str_len
        );
    SCI_MEMSET(temp_wstr_3,0x00,sizeof(wchar) * (GUILIST_STRING_MAX_NUM + 1));
    MMI_STRNTOWSTR((wchar*)temp_wstr_3,GUILIST_STRING_MAX_NUM,(uint8 *)"    ", 4,4);//date 和time之间增加4空格
    MMIAPICOM_Wstrncat((wchar*)temp_wstr_1,(const wchar*)temp_wstr_3,4);
    temp_str_len = MMIAPICOM_Wstrlen(temp_wstr_2);
    MMIAPICOM_Wstrncat((wchar*)temp_wstr_1,temp_wstr_2,temp_str_len);  
    
    title_string.wstr_ptr = temp_wstr_1;
    title_string.wstr_len = MMIAPICOM_Wstrlen((const wchar*)temp_wstr_1);
    
    title_info.is_static = TRUE;
    title_info.title_ptr = &title_string;
    title_info.sub_title_ptr = PNULL;
    title_info.group_id = MENU_CL_CALL_TIMES_LIST_OPT;
    GUIMENU_SetMenuTitleEx(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID, &title_info);

    SCI_FREE(arraycall_info);
}
#endif


#ifndef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : to handle the call times list option window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesListOptWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    MMI_CTRL_ID_T option_ctrl_id =MMICL_CALL_TIMES_LIST_OPT_CTRL_ID;
    uint32    select_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (!CheckCurrentNumberInContact())
        {
            GUIMENU_SetItemVisible(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID,MENU_CL_CALL_TIMES_LIST_OPT , ID_CL_CONTACT_VIEW, FALSE);
        }
#ifdef MMI_BLACKLIST_SUPPORT
        {
            MMIPB_BCD_NUMBER_T  tele_num = {0};
            //get the phone number(bcd)
            GetPartyPhoneNum(&tele_num);
            
            if (MMIAPISET_IsCCBlacklist(&tele_num,MN_CALL_TYPE_NORMAL))
            {
                GUIMENU_SetItemVisible(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID,MENU_CL_CALL_TIMES_LIST_OPT , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);
            }
            else
            {
                GUIMENU_SetItemVisible(MMICL_CALL_TIMES_LIST_OPT_CTRL_ID,MENU_CL_CALL_TIMES_LIST_OPT , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);
            }
        }    
#endif            
        
        SetCallTimesListOptMenuTitle((uint16)select_index);
        MMK_SetAtvCtrl(win_id, option_ctrl_id);
        break;
        
    case MSG_GET_FOCUS:
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        GUIMENU_GetId(option_ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_EDIT_BEFORE_DIAL:
            {
                uint8 tele_num[PHONE_NUM_MAX_LEN + 2];
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
                //Get dispaynum
                GetDisplayPhoneNum(tele_num,PHONE_NUM_MAX_LEN + 2 , &dual_sys);

                MMIAPICL_OpenEditBeforeDialWin((uint16)strlen((char*)tele_num),tele_num);
                MMK_CloseWin(win_id);
            }

            break;
        case ID_CL_CONTACT_VIEW:
            OpenNumberInContact();
            MMK_CloseWin(win_id);
            break;
        
        case ID_CL_LOG_LIST_DELETE:
            //询问是否删除!! 
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
            //MMK_CloseWin(win_id);
            break;
            
        case ID_CL_LOG_DETAIL:
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_ADD_TO_PB:
            HandleDistillPhoneNum();
            MMK_CloseWin(win_id);
            break;
            
 #ifdef VT_SUPPORT
       case ID_CL_DIAL_VIDEOCALL://@zhaohui add
#if 1//OPEN_VT_CHANGE
            HandleRingBack(CC_CALL_SIM_MAX, TRUE);
#else
            HandleRingBack(CC_CALL_SIM_MAX, TRUE, PNULL);
#endif
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_CL_DIAL_AUDIOCALL:
            HandleRingBack(CC_CALL_SIM_MAX, FALSE);
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
            
#ifdef MMI_IP_CALL_SUPPORT
        case ID_CL_DIAL_IPCALL:
            HandleIpCall();
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT            
        case ID_CL_ADD_TO_EXIST_CONTACT:
            HandleAddNumToName();
            MMK_CloseWin(win_id);
            break;
#endif            
        case ID_CL_SEND_SMS:
            HandleAnswerSms();
            MMK_CloseWin(win_id);
            break;
            
        case ID_CL_SEND_MMS:
            HandleAnswerMms();
            MMK_CloseWin(win_id);
            break;
            
#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_LOG_LIST_REFUSE_ADD:
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);
                AddNumToBlackList(tele_num);
                MMK_CloseWin(win_id);
            }
            break;
        case ID_CL_LOG_LIST_REFUSE_REMOVE:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    if (MMIAPISET_DeleteBlacklist(tele_num))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);                    
                    }    
                    
                    MMK_CloseWin(win_id);
                }
            break;
#endif

#ifdef AOC_SUPPORT
        case ID_CL_COSTS_TOTAL:
        case ID_CL_COSTS_MAX:
        case ID_CL_COSTS_RATE:
        case ID_CL_COSTS_RESET:
            MMICL_COSTS_HandleMainMenuMsg(menu_id);
            break;
#endif
#ifdef BT_DIALER_SUPPORT
		case ID_CL_DIAL_SIMCALL:
			HandleRingBack(CC_CALL_SIM_MAX, FALSE);
#ifdef CALL_TIMES_SUPPORT
            MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
#endif
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
			break;

		case ID_CL_DIAL_BTCALL:
			HandleBtCall();
            MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            MMK_CloseWin(win_id);
			break;
#endif
        default:
            break;
        }
        break;

    #ifdef  MMI_PDA_SUPPORT    //not respond menu key
    case MSG_APP_OK:
    case MSG_CTL_OK:               
    #endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        //关闭记录的menu窗口和询问窗口
        GUIMENU_GetId(option_ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_CL_LOG_LIST_DELETE:
            MMIPUB_CloseQuerytWin(&query_win_id);
//lzk mark
#ifdef BT_DIALER_SUPPORT	
            //DeleteOneCallTimesItem((uint16)select_index);
            MMICL_DeleteOneCallTimesItem(s_cur_sel_log_list_type,select_index);
#else
            DeleteOneCallTimesItem((uint16)select_index);
#endif
#if !defined(MMI_PDA_SUPPORT)
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            MMK_CloseWin(win_id);
            break;
        case ID_CL_SEND_MMS:
            MMIPUB_CloseQuerytWin(&query_win_id);
            HandleAnswerMms();
            break;
            
#if defined(MMI_BLACKLIST_SUPPORT)
        case ID_CL_LOG_LIST_REFUSE_ADD:
            {
                MMIPB_BCD_NUMBER_T  tele_num = {0};
                //get the phone number(bcd)
                GetPartyPhoneNum(&tele_num);
                AddNumToBlackList(tele_num);
                MMK_CloseWin(win_id);
            }
            break;
        case ID_CL_LOG_LIST_REFUSE_REMOVE:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    if (MMIAPISET_DeleteBlacklist(tele_num))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);                    
            }            
                    MMK_CloseWin(win_id);
                    
                }
            break;
#endif

        default:
            break;
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //关闭询问窗口
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void DeleteOneCallTimesItem(uint16 select_index)
{
    uint16   sel_item_index =0;
    MMICL_CALL_ARRAY_BCDINFO_T * arraycall_info =PNULL;
    uint16  real_call_times_index = 0;
    uint16  i =0;
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetCallTimesListOptMenuTitle call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11365_112_2_18_2_11_51_105,(uint8*)"");
        return;
    }

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    GetCurrentRecordInfo(arraycall_info, &sel_item_index);

    real_call_times_index  = arraycall_info->call_bcdinfo[sel_item_index].call_times - select_index -1;
    arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index] = 0;
    arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index] =0;
    arraycall_info->call_bcdinfo[sel_item_index].call_times--;

#ifdef MMI_LDN_SUPPORT
    //delete ldn
    //if (arraycall_info->call_bcdinfo[pos].is_sim_record)
    {
        MMICL_CALL_INFO_T call_info = {0};

        call_info.dual_sys = arraycall_info->call_bcdinfo[sel_item_index].dual_sys;
        call_info.call_num.is_num_present = arraycall_info->call_bcdinfo[sel_item_index].is_num_present;
        call_info.call_num.number.number_plan = arraycall_info->call_bcdinfo[sel_item_index].number_plan;
        call_info.call_num.number.number_type = arraycall_info->call_bcdinfo[sel_item_index].number_type;
        call_info.call_num.number.num_len = MIN(arraycall_info->call_bcdinfo[sel_item_index].number_len, MN_MAX_ADDR_BCD_LEN);
        SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[sel_item_index].number, call_info.call_num.number.num_len);

        if (arraycall_info->call_bcdinfo[sel_item_index].is_sim_record && 0 < arraycall_info->sim_record_num)
        {
            arraycall_info->sim_record_num--;
        }

        MMICL_DeleteCallInfo(s_cur_sel_log_list_type, call_info.dual_sys, &call_info);
    }
#endif

    //次数减到0，删除该联系人的记录,  返回
    if(0 == arraycall_info->call_bcdinfo[sel_item_index].call_times)
    {
        DeleteCurrentRecord();
        MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);
        return;
    }
    //后面所有的时间向前移一位
    for(i=0;i <select_index;i++)
    {
        arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index + i] =
            arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index+i+1];
        arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index + i]= 
            arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index+i+1];
    }
    //最后的时间置0
    arraycall_info->call_bcdinfo[sel_item_index].call_start_time[arraycall_info->call_bcdinfo[sel_item_index].call_times] =0;
    arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[arraycall_info->call_bcdinfo[sel_item_index].call_times]=0;

    //update NV 
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        MMICL_WriteNV(s_match_call_info_ptr->call_info[sel_item_index].call_type, arraycall_info);
    }
    else
    {
        MMICL_WriteNV(s_cur_sel_log_list_type, arraycall_info);
    }
    
    MMICL_UpdateEventInd();
    
    SCI_FREE(arraycall_info);
}
#endif

/*****************************************************************************/
// 	Description : set detail of call times list
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL void SetCallTimesListDetail(uint16 select_index)
{
    uint8   tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    wchar   tele_num_wstr[PHONE_NUM_MAX_LEN + 2] = {0};
    uint8   time[MMICL_DISPLAY_TIME_LEN + 1 ] = {0};
    wchar   time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    uint8   date[MMICL_DISPLAY_DATE_LEN + 1] = {0};
    wchar   date_wstr[MMICL_DISPLAY_DATE_LEN + 1] = {0};
#ifndef CMCC_UI_STYLE
    uint8   duration_time[MMICL_DISPLAY_TIME_LEN + 1] = {0};
    wchar   duration_time_wstr[MMICL_DISPLAY_TIME_LEN + 1] = {0};
#endif
    uint16  sel_item_index = 0;
    MMI_TM_T    tm = {0};
    MMI_STRING_T    name_str = {0};
    wchar str_ptr[MMIPB_MAX_STRING_LEN + 1]     = {0};
    MMI_STRING_T    num_str = {0};
    MMI_STRING_T    time_str = {0};
    MMI_STRING_T    date_str = {0};
    MMI_STRING_T    duration_time_str = {0};
    MMI_STRING_T    call_type_str = {0};
    MMIPB_BCD_NUMBER_T  bcd_number = {0};
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T    sim_num = {0};
    MMISET_SIM_NAME_T sim_name = {0};
#endif
    MMI_STRING_T    location_str = {0};
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    uint16  city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
    uint16  str_length = 0;
    uint8   origin_number_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    int16   origin_num_len  = 0;
#endif

    uint16 index = 0;
    uint16 real_call_times_index = 0;
    MMI_CTRL_ID_T ctrl_id = MMICL_CALL_TIMES_DETAIL_CTRL_ID;
    
    //initialize
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListDetail call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11482_112_2_18_2_11_52_106,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(&tm,0,sizeof(tm));
    SCI_MEMSET(&name_str,0,sizeof(name_str));
    SCI_MEMSET(&num_str,0,sizeof(num_str));
    SCI_MEMSET(&time_str,0,sizeof(time_str));
    SCI_MEMSET(&date_str,0,sizeof(date_str));
    SCI_MEMSET(&bcd_number,0,sizeof(bcd_number));
    
    GetCurrentRecordInfo(arraycall_info, &sel_item_index);
        
    //get name
    if (!arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//电话号码不存在
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_UNKNOW_NUM,&num_str);//空号
    }
    else
    {

        //get number
        MMICL_GenDispNumber(arraycall_info->call_bcdinfo[sel_item_index].call_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_type,
            arraycall_info->call_bcdinfo[sel_item_index].number_len,
            arraycall_info->call_bcdinfo[sel_item_index].number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);
        num_str.wstr_len = (uint16)strlen((char*)tele_num);
        num_str.wstr_ptr = tele_num_wstr;//(uint8 *)tele_num;
        MMI_STRNTOWSTR( tele_num_wstr, PHONE_NUM_MAX_LEN, (uint8 *)tele_num, num_str.wstr_len, num_str.wstr_len );

        name_str.wstr_ptr = str_ptr;
        MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[sel_item_index], &name_str, FALSE);
    }
    
    //get time and date
    real_call_times_index = arraycall_info->call_bcdinfo[sel_item_index].call_times - select_index -1;
    
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index]);
    MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,time,MMICL_DISPLAY_TIME_LEN + 1 );
    time_str.wstr_ptr = time_wstr;//(uint8 *)time;
    //time_str.is_ucs2 = FALSE;
    time_str.wstr_len = (uint16)strlen((char*)time);

    MMI_STRNTOWSTR( time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
    
    MMIAPISET_FormatDateStrByDateStyle(tm.tm_year,tm.tm_mon,tm.tm_mday,'/', date,MMICL_DISPLAY_DATE_LEN + 1);
    date_str.wstr_ptr = date_wstr;
    //date_str.is_ucs2 = FALSE;
    date_str.wstr_len = (uint16)strlen((char*)date);
    MMI_STRNTOWSTR( date_wstr, MMICL_DISPLAY_DATE_LEN, (uint8 *)date, date_str.wstr_len, date_str.wstr_len );
    
#ifndef CMCC_UI_STYLE
    //get duration time
    tm = MMIAPICOM_Second2Tm(arraycall_info->call_bcdinfo[sel_item_index].call_duration_time[real_call_times_index]);
    sprintf((char*)duration_time, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    duration_time_str.wstr_ptr  = duration_time_wstr;
    duration_time_str.wstr_len    = (uint16)strlen((char*)duration_time);
    //duration_time_str.is_ucs2   = FALSE;
    MMI_STRNTOWSTR( duration_time_wstr, MMICL_DISPLAY_TIME_LEN, (uint8 *)duration_time, duration_time_str.wstr_len, duration_time_str.wstr_len );
#endif

    //get location
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//电话号码不存在
    {
        uint8 origin_number_str[CC_MAX_TELE_NUM_LEN + 1] = {0};

        MMI_WSTRNTOSTR((uint8 *)origin_number_str, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len);

        if(MMIAPICC_RemoveIpNumber( origin_number_str, num_str.wstr_len, origin_number_arr,CC_MAX_TELE_NUM_LEN))
        {
            origin_num_len = strlen((char*)origin_number_arr);
        }
        else
        {
            MMI_WSTRNTOSTR((uint8 *)origin_number_arr, CC_MAX_TELE_NUM_LEN, num_str.wstr_ptr, num_str.wstr_len, num_str.wstr_len );
            //MMI_MEMCPY(origin_number_arr, CC_MAX_TELE_NUM_LEN + 2, num_str.str_ptr, num_str.length, num_str.length);
            origin_num_len = num_str.wstr_len;
        }
        
        str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_arr, origin_num_len, (uint16*)city_ucs2_str);
    }
    else
    {
        str_length = 0;
    }
    if( str_length == 0 )
    {
        MMI_GetLabelTextByLang(TXT_UNKNOWN, &location_str);
    }   
    else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
    {
        MMI_GetLabelTextByLang(TXT_LOCAL, &location_str);
    }   
    else
    {
        location_str.wstr_ptr = city_ucs2_str;
        //location_str.is_ucs2 = TRUE;
        location_str.wstr_len  = str_length;
    }
#endif
    
    //get call type
    if (arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
    {
        MMI_GetLabelTextByLang(TXT_CALL_VIDEO, &call_type_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CALL_AUDIO, &call_type_str);
    }   

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!    
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //get sim name
        sim_name = MMIAPISET_GetSimName(arraycall_info->call_bcdinfo[sel_item_index].dual_sys);
        
        sim_num.wstr_len  = sim_name.wstr_len;
        sim_num.wstr_ptr = sim_name.wstr_arr;
    }
#endif

    //display name in pb
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present && (0 != name_str.wstr_len))
    {
        //if the num is present and the name is being
        SetHeadAndContentInRText(TXT_NAME,ctrl_id,name_str,&index);
    }

    //display num
    SetHeadAndContentInRText(TXT_COMMON_NUMBER,ctrl_id,num_str,&index);

#ifdef CALL_TIMES_SUPPORT
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index] > 0)
#else
    if (arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0)
#endif
    {
        //display date
        SetHeadAndContentInRText(TXT_DATE,ctrl_id,date_str,&index);

        //display time
        SetHeadAndContentInRText(TXT_TIME,ctrl_id,time_str,&index);
    }

    //display duration
    if ((MMICL_CALL_DIALED == s_cur_sel_log_list_type || MMICL_CALL_RECEIVED == s_cur_sel_log_list_type)
        &&(!arraycall_info->call_bcdinfo[sel_item_index].isvideocall)
#ifdef CALL_TIMES_SUPPORT
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time[real_call_times_index] > 0
#else
        && arraycall_info->call_bcdinfo[sel_item_index].call_start_time > 0
#endif
        )//@zhaohui modify,视频通话不显示时长信息
    {
        SetHeadAndContentInRText(TXT_CL_CALL_TIME,ctrl_id,duration_time_str,&index);
    }

    //display location
    SetHeadAndContentInRText(TXT_GUISHUDI,ctrl_id,location_str,&index);

    //display type  voice call/video call	
    SetHeadAndContentInRText(TXT_COMMON_TYPE,ctrl_id,call_type_str,&index);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    //Emergency Number does not display sim name!!
    if(!arraycall_info->call_bcdinfo[sel_item_index].is_emergency_call)
    {
        //display sim name
        SetHeadAndContentInRText(TXT_CL_DETAIL_SIM_HEAD,ctrl_id,sim_num,&index);
     }
#endif   

    //记录相关信息，为电话拨出提供必要信息
    s_call_log_detail_info_ptr->dual_sys = arraycall_info->call_bcdinfo[sel_item_index].dual_sys;
    SCI_MEMSET(s_call_log_detail_info_ptr->tele_num, 0, (PHONE_NUM_MAX_LEN + 2));
    SCI_MEMCPY(s_call_log_detail_info_ptr->tele_num, tele_num, (PHONE_NUM_MAX_LEN + 2));
#if defined(CL_SAVE_NAME_SUPPORT)
    if (arraycall_info->call_bcdinfo[sel_item_index].name.is_use_nv_name)
    {
        SCI_MEMCPY((void *)&s_call_log_detail_info_ptr->name,(void *)&arraycall_info->call_bcdinfo[sel_item_index].name,sizeof(MMICL_NAME_T));
    }
#endif
    //free memory
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
}

/*****************************************************************************/
// 	Description : to handle the call times detail window message
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimesDetailWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T ctrl_id = MMICL_CALL_TIMES_DETAIL_CTRL_ID;
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#endif
    uint32 sel_time_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == s_call_log_detail_info_ptr)
        {
            s_call_log_detail_info_ptr = (MMICL_DETAIL_INFO_T *)SCI_ALLOCAZ(sizeof(MMICL_DETAIL_INFO_T));
            if(PNULL == s_call_log_detail_info_ptr)
            {
                //SCI_TRACE_LOW:"HandleLogListDetailWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11705_112_2_18_2_11_52_107,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
            SCI_MEMSET(s_call_log_detail_info_ptr, 0, sizeof(MMICL_DETAIL_INFO_T));
        }
        if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type)
        {
            GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)STXT_OPTION,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUILIST_SetMaxItem(ctrl_id, 100, FALSE);
#endif
//add by bingjie.chen
        GUIRICHTEXT_SetLineSpace(ctrl_id, 0);
        SetCallTimesListDetail((uint16)sel_time_index);
#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/12
#ifndef MMI_PDA_SUPPORT
        GUIRICHTEXT_SetItemBorderSpace(ctrl_id, 0);
#endif
#endif 	 
        break;
            
    case MSG_GET_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK: //rong.gu to do
    case MSG_APP_OK:
    case MSG_APP_MENU:
        switch (s_cur_sel_log_list_type)
        {
        case MMICL_CALL_MISSED:
        case MMICL_CALL_RECEIVED:
        case MMICL_CALL_DIALED:
        case MMICL_CALL_MATCHED:
#ifdef MMI_PDA_SUPPORT            
        case MMICL_CALL_ALL:
#endif            
#if defined(MMI_BLACKLIST_SUPPORT)
        case MMICL_CALL_REFUSED:
#endif
#ifdef BT_DIALER_SUPPORT
        case MMICL_BT_CALL_MISSED:
        case MMICL_BT_CALL_RECEIVED:
        case MMICL_BT_CALL_DIALED:
#endif
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_MENU_WIN2_TAB, PNULL);
            break;
            
        default:
    //           SCI_TRACE_LOW("HandleLogListDetailWindow:s_cur_sel_log_list_type %d is error!",s_cur_sel_log_list_type);
            break;
        }
        break;
        
#ifdef MMI_MULTI_GREEN_KEY_SINGLE         
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
        break;
#else        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);                
            HandleRingBack(sim_type, FALSE);
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
#if 1//OPEN_VT_CHANGE
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
#else
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
#endif
        break;        
#endif
        
        //@samboo.shen 一键vt
    case MSG_APP_VIDEO_TEL:
        HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
        MMK_CloseWin(win_id);
        break; 
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_call_log_detail_info_ptr)
        {
            SCI_FREE(s_call_log_detail_info_ptr);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

#endif

/*****************************************************************************/
// 	Description : get record num by type and get record content to arraycall_info
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: WARNING: "GetCurrTypeNum" just get record num by type, only for pda project
//                 "GetRecordNumByType" not only get record num, but also assign arraycall_info
/*****************************************************************************/
LOCAL uint8 GetRecordNumByType(MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info_ptr, MMICL_CALL_TYPE_E e_call_type)
{
    uint8 call_log_num = 0;
    if(PNULL == arraycall_info_ptr)
    {
        //SCI_TRACE_LOW:"mmicl GetRecordNumByType null ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11835_112_2_18_2_11_52_108,(uint8*)"");
        return call_log_num;
    }
    if (MMICL_CALL_ALL ==e_call_type)
    {
        e_call_type = GetAllLogFillter();
        if (e_call_type == MMICL_CALL_PB_NUMBER_MATCHED)
        {
             call_log_num= GetCalllogPBData_MatchedItemCount();
             return call_log_num;
        }
    }

    MMICL_GetRecordInfo(e_call_type, arraycall_info_ptr);
#ifdef MMI_PDA_SUPPORT            
    call_log_num = GetCurrTypeNum( e_call_type, arraycall_info_ptr);
#else             
    call_log_num = arraycall_info_ptr->record_num;
#endif

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
    if (MMICL_CALL_PB_NUMBER_MATCHED ==e_call_type)
    {
        call_log_num= GetCalllogPBData_MatchedItemCount();
    }
#endif
    return call_log_num;
}
/*****************************************************************************/
//  Description : 获得CL中某条纪录对应的姓名，get name by cl record, the name is from pb or the record itself
//  Global resource dependence : none
//  Author: tao.xue
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetNameByClInfo( MMICL_CALL_BCDINFO_T cl_nv_info, MMI_STRING_T* name_str, BOOLEAN is_use_default_name)
{
    BOOLEAN b_search_result = FALSE;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    int origin_num_len = 0;
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8 tele_number_no_ip[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint32 tele_len = 0;

    //SCI_TRACE_LOW:"Enter MMIAPICL_GetNameByClInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11878_112_2_18_2_11_52_109,(uint8*)"");

    if(PNULL == name_str || PNULL == name_str->wstr_ptr || 0 == cl_nv_info.number_len)
    {
        //SCI_TRACE_LOW:"MMIAPICL_GetNameByClInfo index:parameter error !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_11882_112_2_18_2_11_52_110,(uint8*)"");
        return FALSE;
    }
#if defined(CL_SAVE_NAME_SUPPORT)
    if (cl_nv_info.name.is_use_nv_name)
    {
        name_str->wstr_len  = cl_nv_info.name.wstr_len;
        if (name_str->wstr_len > 0)
        {
            MMI_WSTRNCPY(
                                        name_str->wstr_ptr,
                                        MMICL_NAME_MAX_LEN,
                                        cl_nv_info.name.wstr,
                                        MMICL_NAME_MAX_LEN,
                                        name_str->wstr_len
                                        );
            b_search_result = TRUE;
        }
        else
        {
            b_search_result = FALSE;
        }
    }
    else
#endif
    {
        //remove ip header
        tele_len = MMICL_GenDispNumber(cl_nv_info.call_type,
            cl_nv_info.number_type, 
            (uint8)cl_nv_info.number_len, 
            cl_nv_info.number, 
            tele_num, 
            CC_MAX_TELE_NUM_LEN + 2);

        pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(cl_nv_info.number_type, MN_NUM_PLAN_UNKNOW);
        pb_num.number_len = MIN(MN_MAX_ADDR_BCD_LEN, cl_nv_info.number_len);					
        MMI_MEMCPY(
                                pb_num.number,
                                sizeof(pb_num.number),
                                (void*)cl_nv_info.number,
                                sizeof(cl_nv_info.number),
                                pb_num.number_len
                                );

        b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, name_str, PHONE_NUM_MAX_LEN, is_use_default_name);/*lint !e506 */

        if(!b_search_result && MMIAPICC_RemoveIpNumber(tele_num, tele_len, tele_number_no_ip,CC_MAX_TELE_NUM_LEN))
        {
            SCI_MEMSET(&pb_num, 0x00, sizeof(pb_num));
        
            origin_num_len = strlen((char*)tele_number_no_ip);
            
            if(MMIAPICOM_GenPartyNumber(tele_number_no_ip, origin_num_len, &party_num))
            {
                pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_num.number_len = MIN(MN_MAX_ADDR_BCD_LEN, party_num.num_len);
                MMI_MEMCPY(
                                        pb_num.number,
                                        sizeof(pb_num.number),
                                        (void*)&party_num.bcd_num,
                                        sizeof(party_num.bcd_num),
                                        pb_num.number_len
                                        );       

                b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, name_str, PHONE_NUM_MAX_LEN, is_use_default_name);/*lint !e506 */
            }
            else
            {
                return FALSE;
            }
        }
    }

    return b_search_result;
}

/*****************************************************************************/
//  Description : Select SIM API of CL app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICL_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC  MMICLSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA        add_data
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMICL_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)append_type;
    caller_data.callback_func =MMICLSelectSIMWinCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
    caller_data.publist_id = MMICL_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin( &caller_data);
    

#endif
    return MMI_RESULT_TRUE;
}




#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error

/*****************************************************************************/
//	Description : the handle function of call list delete all menu window
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleLogListDeleteSelectedQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        DeleteAllCallLog(s_cur_sel_log_list_type);
        CalllogPBData_StopSelection();
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMICL_PB_DETAIL_SELECT_WIN_ID);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}


/*****************************************************************************/
// 	Description : to handle the contact call log list window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailCallLogListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_ALL_LOG_PB_DETAIL_LIST_CTRL_ID;
    static BOOLEAN s_is_allow_sim_call[MMI_DUAL_SYS_MAX] = {FALSE,};
    static MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_GREEN_KEY_SINGLE        
    CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
    MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);
    //uint8 call_log_num = 0;

    MMI_HANDLE_T parent_win_id = MMK_GetParentWinHandle(win_id);
    MMIPB_CONTACT_T * contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(parent_win_id);
    MMI_HANDLE_T    list_handle =MMK_GetCtrlHandleByWin(win_id,MMICL_ALL_LOG_PB_DETAIL_LIST_CTRL_ID);

    if(contact_ptr == PNULL)
    {
        return  recode;
    }

    SCI_TRACE_LOW("[MMICL] HandlePBDetailCallLogListWindow msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (CalllogPBData_GetMainWinID() == PNULL)
        {
            InitPbCalllogData(parent_win_id);
        }

        SetAllLogFillterPBNumber(contact_ptr);
        SetAllLogFillter(MMICL_CALL_PB_NUMBER_MATCHED);
        s_cur_sel_log_list_type = MMICL_CALL_ALL;
        SCI_MEMSET(s_is_allow_sim_call, 0x00, sizeof(s_is_allow_sim_call));
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));

        if(PNULL == arraycall_info)
            {
                //SCI_TRACE_LOW:"HandlePBDetailCallLogListWindow call info alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_12079_112_2_18_2_11_53_111,(uint8*)"");
                return recode;
            }
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

#if defined(MMI_ISTYLE_SUPPORT)        
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else     
#endif			
        {
            GUIAPICTRL_SetBothRect(list_handle,&list_both_rect);
            MMK_SetAtvCtrl(win_id, list_handle);
        }
        break;
   
    case MSG_LOSE_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
        break;
    case MSG_FULL_PAINT:
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            break;
        }
        //关闭idle状态下提示位接电话的窗口以及状态栏图标
        MMIAPICC_ClearMissedCallNum();
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
            MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
            SetLogListItem();
        }

        break;

#ifdef MMI_PDA_SUPPORT
    /*add for istyle*/
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else
#endif			
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            uint16 cur_sel = 0;

            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(item_content_ptr == PNULL)
            {
               break;
            }
            cur_sel = item_content_ptr->item_index;
            if(item_content_ptr->item_content_index == CL_LIST_ITEM_ICON_INDEX)
            {
                HandleRingBackByType(CC_CALL_DEFAULT_CALL);
            }
        }       

        break;

     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                //DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id);
                   DrawListItemIconData(arraycall_info,need_item_content_ptr);                
            }
            break;
        }
#endif


#ifdef MMI_MULTI_GREEN_KEY_SINGLE        
    case MSG_KEYDOWN_GREEN:
        s_is_allow_sim_call[MN_DUAL_SYS_1] = TRUE;
        break;
        
    case MSG_KEYUP_GREEN:
        if(s_is_allow_sim_call[MN_DUAL_SYS_1])
        {
            if(  GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
            {
                HandleRingBack(CC_MAJOR_SIM_CALL, FALSE);
            }
        }
        s_is_allow_sim_call[MN_DUAL_SYS_1]= FALSE;
        break;
#else
    case MSG_KEYDOWN_CALL1:
    case MSG_KEYDOWN_CALL2:
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_CALL4:
        {
            dual_sys =  MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            s_is_allow_sim_call[dual_sys] = TRUE;
        }
        break;
        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {            
            dual_sys = MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_1);
            sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);

            if(s_is_allow_sim_call[dual_sys])
            {
                if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
                {
                    HandleRingBack(sim_type, FALSE);
                }
            }
            s_is_allow_sim_call[dual_sys] = FALSE;
        }
        break;
#endif

#ifdef VT_SUPPORT        
    case MSG_KEYLONG_GREEN:
    case MSG_APP_VIDEO_TEL:        
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
#if 1//OPEN_VT_CHANGE
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE);
#else
            HandleRingBack(CC_MAJOR_SIM_CALL, TRUE, PNULL);
#endif
        }
        break;
#endif
        

        
#ifdef MMI_PDA_SUPPORT    //to do open list menu
    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteAllQueryWinMsg);
        }
        break;
#endif

    case MSG_GET_FOCUS:

        CalllogPBData_SetListHandle(MMK_GetCtrlHandleByWin(MMK_GetFocusWinHandle(),list_ctrl_id));
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
#endif		
        if (PNULL == arraycall_info)
        {
            arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        }
        if( GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)> 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }

        GUIWIN_SetTitleIsVisible(win_id, FALSE, FALSE);
		MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

        break;
#endif

    case MSG_CTL_TAB_SWITCH:
        //隐私检查
        if((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL)))
        {
            //需要进行隐私检查
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_CL, CL_ValidatePrivacyTabSwitch, PNULL, 0);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined(MMI_ISTYLE_SUPPORT)		
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
        else 
#endif			
        {
#ifdef CALL_TIMES_SUPPORT        
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, (ADD_DATA)win_id);
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:        
	case MSG_APP_MENU:
	    //check is no log or not
        if(GetRecordNumByType(arraycall_info, s_cur_sel_log_list_type)>0)
        {
#ifdef MMI_GPRS_SUPPORT
            MMK_CreateWin((uint32 *)MMICL_PB_DETAIL_LOG_LIST_MENU_WIN_TAB, (ADD_DATA)win_id);
#endif
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //Set Back 
        MMK_CloseParentWin(info_ptr);    
        break;
        
    case MSG_CLOSE_WINDOW:
        SetAllLogFillter(MMICL_CALL_ALL);

        if (PNULL != arraycall_info)
        {
            SCI_FREE(arraycall_info);
            arraycall_info = PNULL;
        }
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
#endif
        break;
        
    default:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)    
        //@modify by chenyao for istyle:move the window client above the tab_ctrl
        if (MMITHEME_IsIstyle()) 
        {
            return Istyle_HandlePBDetailCalllogChildWindow(win_id, msg_id, param);
        }
#endif
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;

}

/*****************************************************************************/
// 	Description : to handle the contact call log list menu window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailLogListMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32       group_id = 0, menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(  GetCalllogPBData_MatchedItemCount()== 0)
        {
            GUIMENU_SetItemGrayed(MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID,MENU_CL_PB_DETAIL_MEMU,
                    ID_CL_DELETE,TRUE);
        }

        MMK_SetAtvCtrl(win_id, MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID);
    break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        GUIMENU_GetId(MMICL_PB_DETAIL_CALLLOG_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
            case ID_CL_DELETE:
                //询问是否删除!! 
               if(  GetCalllogPBData_MatchedItemCount()== 0)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
                else
                {
                    //Enter new WinTAB
                    CalllogPBData_StartSelection();
                    MMK_CreateWin((uint32*)MMICL_PBDETAIL_SELECT_WIN_TAB,PNULL);
                }
                //Enter SelectWin
                MMK_CloseWin(win_id);
                break;
           default:
                break;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : to handle the call log list menu window message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllLogListPopMenuWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    uint32 group_id = 0, menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIPB_BCD_NUMBER_T  tele_num = {0};
            //get the phone number(bcd)
            GetPartyPhoneNum(&tele_num);
        if (CheckCurrentNumberInContact())
        {
            GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_LOG_LIST_SAVE, FALSE);
        }
        else
        {
            GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_CONTACT_VIEW, FALSE);
        }
#if defined(MMI_BLACKLIST_SUPPORT)            
            if (MMIAPISET_IsCCBlacklist(&tele_num,MN_CALL_TYPE_NORMAL))
            {
                GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_LOG_LIST_REFUSE_ADD, FALSE);
            }
            else
            {
                GUIMENU_SetItemVisible(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,MENU_ALL_LIST_POPUP_MEMU , ID_CL_LOG_LIST_REFUSE_REMOVE, FALSE);
            }
#endif			
        MMK_SetAtvCtrl(win_id,MMICL_ALL_LIST_POPUP_MENU_CTRL_ID );
        }    
    break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        GUIMENU_GetId(MMICL_ALL_LIST_POPUP_MENU_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
            case ID_CL_EDIT_BEFORE_DIAL:
                {
                    uint8 tele_num[PHONE_NUM_MAX_LEN + 2];
                    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
                    //Get dispaynum
                    GetDisplayPhoneNum(tele_num,PHONE_NUM_MAX_LEN + 2 , &dual_sys);

                    MMIAPICL_OpenEditBeforeDialWin((uint16)strlen((char*)tele_num),tele_num);
                    MMK_CloseWin(win_id);
                }
                break;
            case ID_CL_CONTACT_VIEW:
                OpenNumberInContact();
				MMK_CloseWin(win_id);
                break;
            case ID_CL_ADD_TO_PB:
                HandleDistillPhoneNum();
                MMK_CloseWin(win_id);
                break;
#if defined(MMI_BLACKLIST_SUPPORT)
            case ID_CL_LOG_LIST_REFUSE_ADD:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    AddNumToBlackList(tele_num);
                    MMK_CloseWin(win_id);
                    
                }
                 break;         
            case ID_CL_LOG_LIST_REFUSE_REMOVE:
                {
                    MMIPB_BCD_NUMBER_T  tele_num = {0};
                    //get the phone number(bcd)
                    GetPartyPhoneNum(&tele_num);
                    if (MMIAPISET_DeleteBlacklist(tele_num))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMPLETE);                    
                    }    
                    MMK_CloseWin(win_id);
                    
                }
                 break;                  
#endif                 
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
            case ID_CL_ADD_TO_EXIST_CONTACT:
                HandleAddNumToName();
                MMK_CloseWin(win_id);
                break;
#endif
            case ID_CL_LOG_LIST_DELETE:
                DeleteCurrentRecord();
                MMK_CloseWin(win_id);                
                break;
            
#ifdef VT_SUPPORT
           case ID_CL_DIAL_VIDEOCALL://@zhaohui add
#if 1//OPEN_VT_CHANGE
                HandleRingBack(CC_CALL_SIM_MAX, TRUE);
#else
                HandleRingBack(CC_CALL_SIM_MAX, TRUE, PNULL);
#endif
                MMK_CloseWin(win_id);
                break;
#endif

            case ID_CL_DIAL_AUDIOCALL:
                HandleRingBack(CC_CALL_SIM_MAX, FALSE);
                MMK_CloseWin(win_id);
                break;
                
#ifdef MMI_IP_CALL_SUPPORT
            case ID_CL_DIAL_IPCALL:
                HandleIpCall();
                MMK_CloseWin(win_id);
                break;
#endif

            case ID_CL_SEND_SMS:
                HandleAnswerSms();
                MMK_CloseWin(win_id);
                break;
                
            case ID_CL_SEND_MMS:
                HandleAnswerMms();
                MMK_CloseWin(win_id);
                break;
                
           default:
                break;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}


/*****************************************************************************/
// 	Description : SelectWin APIS
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void CalllogPBDataSelectWin_SetListHandle(MMI_HANDLE_T list_handle)
{
    CalllogPBData_SetListHandle(list_handle);
    return;
}
LOCAL uint16 CalllogPBDataSelectWin_GetMaxNum(void)
{
    return GetCalllogPBData_MatchedItemCount();
}
LOCAL void CalllogPBDataSelectWin_UpdateLogList(void)
{
	SetLogListItem();
	return;
}

LOCAL void CalllogPBDataSelectWin_SetSelectedbyIndex(uint16 index)
{
    if (CalllogPBData_GetIsSelectedByIndex(index))
    {
        CalllogPBData_SetSelectedByIndex(index, FALSE);
    }
    else
    {
        CalllogPBData_SetSelectedByIndex(index, TRUE);
    }
	return;
}

LOCAL void CalllogPBDataSelectWin_SetAllSelected(BOOLEAN isSelected,MMI_HANDLE_T list_handle)
{
    uint16 i =0;
    uint32 sel_item_index =0;
                
    if (isSelected == FALSE)
    {
        CalllogPBData_SetAllSelected(isSelected);
    }
    else
    {
        for (i = 0;i<GUILIST_GetTotalItemNum(list_handle);i++)
        {
            GUILIST_GetItemData(list_handle, i, &sel_item_index);    
            CalllogPBData_SetSelectedByIndex(sel_item_index,isSelected);
        }
    }
    return ;
}

LOCAL void CalllogPBDataSelectWin_ExitSelection(void)
{
    CalllogPBData_StopSelection();
    return;
}
/*****************************************************************************/

//
/*****************************************************************************/
// 	Description : to handle the contact call log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    GUIFORM_CHILD_WIDTH_T  child_width = {0};

    MMI_HANDLE_T list_handle =MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_LIST_CTRL_ID);
    MMI_HANDLE_T btn_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID);
    MMI_HANDLE_T titleform_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_FORM_CTRL_ID);
    MMI_HANDLE_T form_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_FORM_CTRL_ID);
    MMI_HANDLE_T label_handle = MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_LABEL_CTRL_ID);
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        CalllogPBDataSelectWin_SetListHandle(list_handle);
        
        GUILABEL_SetTextById(label_handle,TXT_PB_SELECT_ALL, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, TRUE);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = 40;
        GUIFORM_SetChildWidth(titleform_handle,btn_handle,&child_width);
        GUIFORM_PermitChildBorder(titleform_handle, FALSE);
        GUIBUTTON_PermitBorder(btn_handle, FALSE);

        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_handle,list_handle, &child_height);
        GUIFORM_IsSlide(form_handle, FALSE);
        GUIFORM_PermitChildBg(form_handle,FALSE);
        GUILIST_PermitBorder(list_handle,FALSE);
        
        GUILIST_SetMaxSelectedItem(list_handle,CalllogPBDataSelectWin_GetMaxNum());
        GUIBUTTON_SetSelect(MMK_GetCtrlHandleByWin(win_id,MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID), FALSE);                
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);

        MMK_SetAtvCtrl(win_id, list_handle);
        break;
    case MSG_FULL_PAINT:
//        CalllogPBDataSelectWin_SetAllSelected(FALSE,list_handle);
        CalllogPBDataSelectWin_UpdateLogList();
        break;
    case MSG_GET_FOCUS:
        MMIAPICL_SetUpdatedWinHandle(win_id,TRUE);
        break;
    case MSG_LOSE_FOCUS:
         MMIAPICL_SetUpdatedWinHandle(win_id,FALSE);
         break;
    case MSG_CTL_LIST_MARK_ITEM:
        {
            uint32 sel_item_index =0;
                
            GUILIST_GetItemData(list_handle, GUILIST_GetCurItemIndex(list_handle), &sel_item_index);
            CalllogPBDataSelectWin_SetSelectedbyIndex(sel_item_index);
            if(GUILIST_GetSelectedItemNum(list_handle) <  GUILIST_GetTotalItemNum(list_handle))           
            {
                if (GUIBUTTON_GetSelect(btn_handle))
                {
                    GUIBUTTON_SetSelect(btn_handle, FALSE);
					GUIBUTTON_Update(btn_handle);
                }
            }
            else
            {
                if (!GUIBUTTON_GetSelect(btn_handle))
                {
                    GUIBUTTON_SetSelect(btn_handle, TRUE);
					GUIBUTTON_Update(btn_handle);
                }
            }
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    if (GUILIST_GetSelectedItemNum(list_handle)<1)
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,CC_HandleLogListDeleteSelectedQueryWinMsg);                
                    }
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_CloseWin(win_id);   
                    break;
                case MMICL_PB_DETAIL_SELECT_TITLE_BUTTON_CTRL_ID:
                    CalllogPBDataSelectWin_SetAllSelected(GUIBUTTON_GetSelect(btn_handle),list_handle);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, 0,0);
                    break;
                default:
                    break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //Set Back 
        MMK_CloseWin(win_id);    
        break;
    case MSG_CLOSE_WINDOW:
        CalllogPBDataSelectWin_ExitSelection();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
// 	Description : to handle pub edit win to edit number before call
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBeforeDialWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    GUI_BG_T            edit_bg = {0};
    GUI_BORDER_T        edit_border = {0};
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set edit single
        GUIEDIT_SetStyle(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,GUIEDIT_STYLE_SINGLE);

        //set bg
        edit_bg.bg_type = GUI_BG_COLOR;
        edit_bg.color   = MMI_WHITE_COLOR;
        edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
        GUIEDIT_SetBg(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,&edit_bg);

        //set border
        edit_border.type  = GUI_BORDER_ROUNDED;
        edit_border.width = 2;
        edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        GUIEDIT_SetBorder(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,&edit_border);

        //set font
        GUIEDIT_SetFontColor(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,MMI_BLACK_COLOR);

        //set left softkey
        GUIEDIT_SetSoftkey(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        
        MMK_SetAtvCtrl(win_id,MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMI_STRING_T    edit_str = {0};
            uint8           temp_str[MMISMS_PBNUM_MAX_LEN + 1]  = {0};

            GUIEDIT_GetString(MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,&edit_str);
            if (0 != edit_str.wstr_len)
            {
                if (MMIAPICOM_IsValidNumberStringEx(&edit_str))
                {
                    //convert string
                    MMI_WSTRNTOSTR((uint8 *)temp_str,MMISMS_PBNUM_MAX_LEN,
                        edit_str.wstr_ptr,edit_str.wstr_len,
                        edit_str.wstr_len);
                    MMIAPICC_MakeCall(
                                     MN_DUAL_SYS_MAX,
                                            temp_str,
                                            strlen((char *)temp_str),
                                            PNULL,
                                            PNULL,
                                            CC_CALL_SIM_MAX,
                                            CC_CALL_NORMAL_CALL,
                                            PNULL
                                            );
                   MMK_CloseWin( win_id );      
                       
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_INVALID_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : to handle CallTime pop list win
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallTimePopListWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        recode  =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMICL_CALL_TIME_POP_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        GUILIST_ITEM_T  item = {0};
        GUILIST_ITEM_DATA_T item_date = {0};


        GUILIST_SetMaxItem(ctrl_id, 2,FALSE);
        item.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        item.item_data_ptr = &item_date;
        item_date.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_date.item_content[0].item_data.text_id= TXT_ADDTOPB_NEW;
        GUILIST_AppendItem(ctrl_id,&item );
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        item_date.item_content[0].item_data.text_id = TXT_ADDTOPB_EXIST;
        GUILIST_AppendItem(ctrl_id,&item );
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            if(GUILIST_GetCurItemIndex(ctrl_id) == 0)
            {
                HandleDistillPhoneNum();
            }
            else
            {
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
                HandleAddNumToName();
#endif
            }
        }
        MMK_CloseWin( win_id );      

        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}                                            
#endif

/*****************************************************************************/
//  Description :open call number window
//  Global resource dependence : 
//  Author:Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenEditBeforeDialWin(uint8   tel_len,
                                           uint8*  tel_num_ptr
                                           )
{       

    if (0 != tel_len)   //No Number
    {
        wchar mail_wstr[ MMISMS_PBNUM_MAX_LEN + 1 ] = {0};
        MMI_STRNTOWSTR( mail_wstr, MMISMS_PBNUM_MAX_LEN, tel_num_ptr, tel_len, tel_len );

        if (!MMK_IsOpenWin(MMICL_EDIT_BEFORE_DIAL_WIN_ID))
        {
            MMK_CreatePubEditWin((uint32*)MMICL_EDIT_BEFORE_DIAL_WIN_TAB,PNULL);
        }
        else
        {   MMK_CloseWin(MMICL_EDIT_BEFORE_DIAL_WIN_ID);
            MMK_CreatePubEditWin((uint32*)MMICL_EDIT_BEFORE_DIAL_WIN_TAB,PNULL);
        }
        // set the edit box init param
        GUIEDIT_SetString(
            MMICL_EDIT_BEFORE_DIAL_EDIT_CTRL_ID,
            mail_wstr,
            tel_len);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SMS_BOX_EMPTY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}



#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get display phone number of current log list 
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void GetCallLogInfoDetail(MN_DUAL_SYS_E *dual_sys_ptr,BOOLEAN *isVideoCall)
{

    uint32 array_index = 0;

    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetDisplayPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_12898_112_2_18_2_11_55_112,(uint8*)"");
        return;
    }

    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    
    array_index = CL_GetArrayIndexofCurSelected();

    *dual_sys_ptr = arraycall_info->call_bcdinfo[array_index].dual_sys;
    *isVideoCall  = arraycall_info->call_bcdinfo[array_index].isvideocall;

    SCI_FREE(arraycall_info);
    
    return;
}
#endif
#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : 本地搜索中通话记录匹配项的"打开"回调函数
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CL_OpenMatchedItem(uint32 user_data)
{    
    if (0 != s_match_call_info_ptr->match_num)
    {
        s_cur_sel_log_list_type = MMICL_CALL_MATCHED;
        s_mmicl_match_item_index = user_data;
#ifdef CALL_TIMES_SUPPORT
        if(s_match_call_info_ptr->call_info[s_mmicl_match_item_index].call_times>1)
        {
            MMK_CreateWin((uint32 *)MMICL_CALL_TIMES_WIN_TAB, PNULL);
        }
        else
        {
            MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
        }
#else
        MMK_CreateWin((uint32 *)MMICL_LOG_LIST_DETAIIL_WIN_TAB, PNULL);
#endif
    }          
}

/*****************************************************************************/
// 	Description : 本地搜索中通话记录匹配项的显示回调函数
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CL_SetMatchItemData(
                            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr,
                            uint32 user_data
                             )
{

    MMI_STRING_T name_number_str = {0};
    wchar name_number_wchar[PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN + 1]  ={0};     

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[GUILIST_STRING_MAX_NUM + 1]={0};
#endif

    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[GUILIST_STRING_MAX_NUM + 1] = {0};

    MMI_IMAGE_ID_T	image_id = IMAGE_NULL;
    
    MMICL_MATCH_TOTAL_INFO_T* info_ptr = PNULL;
    GUILIST_ITEM_DATA_T item_data = {0};
    uint32 cur_match_index = user_data;

    MMICL_CALL_BCDINFO_T *callinfo=PNULL;

    if (PNULL != s_match_call_info_ptr)
    {
        info_ptr = s_match_call_info_ptr;
    }
    else
    {
        return ;
    }

    if (0 == info_ptr->match_num)
    {        
        SCI_TRACE_LOW("warning! CL_SetMatchItemData 0 == info_ptr->match_num");
        return;
    }
    else
    {
        item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
        item_data.softkey_id[MIDDLE_BUTTON]    = TXT_VIEW;
        item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
    
        callinfo = &info_ptr->call_info[cur_match_index];
    //Append item
        //Init Text Contents     Name/Number, SimName, Date, Time
        SCI_MEMSET(&name_number_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&name_number_wchar,0,sizeof(name_number_wchar));            
        name_number_str.wstr_ptr = name_number_wchar;
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
        SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
        sim_name_str.wstr_ptr = sim_name_wstr;
        sim_name_str.wstr_len = 0;
#endif
        SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
        datetime_str.wstr_ptr = datetime_wstr;
        SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&wstr_time_str,0,sizeof(datetime_wstr));            
        time_str.wstr_ptr = wstr_time_str;

        //Add Image content
        image_id = GetCallTypeImageID(callinfo);
        item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[CL_LIST_ITEM_TYPE_INDEX].item_data.image_id = image_id;


#ifndef MMI_MULTI_SIM_SYS_SINGLE    
        if (!callinfo->is_emergency_call)
        {
            MMIAPICL_GetSimNameContent(sim_name_wstr,GUILIST_STRING_MAX_NUM,callinfo->dual_sys);
        }
        sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
        
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].font_color_id=MMIAPISET_GetSimColorID(callinfo->dual_sys);
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].is_default =TRUE;
#endif                 
        
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
        item_data.item_content[CL_LIST_ITEM_SIM_INDEX].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#else
        //Single don't need sim name
#endif

        //Add Contact name/or call number content
        if (GetNameOrNumberContent(name_number_wchar,PHONE_NUM_MAX_LEN+MAX_CALL_TIME_LEN,callinfo,TRUE,FALSE))
        {	
			name_number_str.wstr_len= MMIAPICOM_Wstrlen(name_number_str.wstr_ptr);
            item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_ptr =name_number_str.wstr_ptr;
            item_data.item_content[CL_LIST_ITEM_NUM_OR_NAME_INDEX].item_data.text_buffer.wstr_len =name_number_str.wstr_len;
        }  

#ifdef CALL_TIMES_SUPPORT            
        if (callinfo->call_times>0)
        {
            //Add date and time content
            if (0<callinfo->call_start_time[callinfo->call_times -1])
            {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/				
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,callinfo->call_times -1);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
                
            }
        }    
#else
            //Add date and time content
        if (0<callinfo->call_start_time)
        {
/*            
                GetDateContent(&datetime_str,GUILIST_STRING_MAX_NUM, callinfo,0,FALSE);
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_DATE_INDEX].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
*/				
                //GetTimeContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                GetTimeAndDateContent(&time_str,GUILIST_STRING_MAX_NUM,callinfo,0);
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_len = time_str.wstr_len;
                item_data.item_content[CL_LIST_ITEM_TIME_INDEX].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
        }
#endif            

    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
    }
}

/*****************************************************************************/
// 	Description : 本地搜索中通话记录搜索接口
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_LocalSearch(const MMI_STRING_T *key_word_str, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num)
{
    uint8 i = 0;
    uint8 type_index = 0;
    MMI_STRING_T name_str = {0};
    wchar tele_num_wstr[MMIPB_MAX_STRING_LEN + 1] = {0};
    uint16 tele_num_wstr_len = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    wchar wstr_ptr[MMIPB_MAX_STRING_LEN + 1] = {0};
    uint8 tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    MMIPB_BCD_NUMBER_T bcd_number = {0};
    uint16 tele_len = 0;
    MMICL_MATCH_TOTAL_INFO_T* info_ptr = PNULL;
#ifdef MMI_PDA_SUPPORT
    GUIITEM_STYLE_E item_style = GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2;				
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    GUIITEM_STYLE_E item_style = GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;     //GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT;   
#else
    GUIITEM_STYLE_E item_style = GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT;
#endif
#endif

    if (PNULL != s_match_call_info_ptr)
    {
        SCI_MEMSET((void*)s_match_call_info_ptr, 0x00, sizeof(MMICL_MATCH_TOTAL_INFO_T));
    }
    else
    {
        s_match_call_info_ptr = (MMICL_MATCH_TOTAL_INFO_T*)SCI_ALLOCAZ(sizeof(MMICL_MATCH_TOTAL_INFO_T));
    }
    
    if (PNULL != s_match_call_info_ptr)
    {
        info_ptr = s_match_call_info_ptr;
    }
    else
    {
        return ;
    }

    if (PNULL == match_data_ptr || PNULL == match_data_ptr->match_item_ptr)
    {
        return ;
    }
    
#ifndef MMI_PDA_SUPPORT
    for(type_index = MMICL_CALL_MISSED; type_index < MMICL_CALL_MATCHED; type_index++)
#endif
    {
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            SCI_FREE(s_match_call_info_ptr);
            return ;
        }        

        MMICL_GetRecordInfo((MMICL_CALL_TYPE_E)type_index, arraycall_info);

        for ( i = 0; i < arraycall_info->record_num; i++ )
        {
            tele_len = MMICL_GenDispNumber(arraycall_info->call_bcdinfo[i].call_type,
                                                                        arraycall_info->call_bcdinfo[i].number_type,
                                                                        arraycall_info->call_bcdinfo[i].number_len,
                                                                        arraycall_info->call_bcdinfo[i].number, 
                                                                        tele_num,
                                                                        PHONE_NUM_MAX_LEN + 2);

            if (0 == tele_len || !arraycall_info->call_bcdinfo[i].is_num_present)
            {
                break;
            }

            bcd_number.number_len = arraycall_info->call_bcdinfo[i].number_len;
            bcd_number.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[i].number_type,
                arraycall_info->call_bcdinfo[i].number_plan);
            MMI_MEMCPY(bcd_number.number,MMIPB_BCD_NUMBER_MAX_LEN,
                arraycall_info->call_bcdinfo[i].number,MMIPB_BCD_NUMBER_MAX_LEN,
                bcd_number.number_len);

            //search number
            SCI_MEMSET(tele_num_wstr, 0, sizeof(tele_num_wstr));
            tele_num_wstr_len = MIN(tele_len, MMIPB_MAX_STRING_LEN);

            MMI_STRNTOWSTR(tele_num_wstr,
                                        MMIPB_MAX_STRING_LEN,
                                        (uint8 *)tele_num,
                                        tele_len,
                                        tele_num_wstr_len);

            //search from pb
            SCI_MEMSET(wstr_ptr, 0, sizeof(wstr_ptr));

            name_str.wstr_ptr = wstr_ptr;
            MMIAPICL_GetNameByClInfo(arraycall_info->call_bcdinfo[i], &name_str, FALSE);

            if ((0 != name_str.wstr_len)
            && MMIAPICOM_WstrnstrExt(name_str.wstr_ptr, name_str.wstr_len, key_word_str->wstr_ptr, key_word_str->wstr_len, FALSE))
            {
                //添加到匹配列表
                SCI_MEMCPY(&info_ptr->call_info[info_ptr->match_num],&arraycall_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T));               
                info_ptr->call_info[info_ptr->match_num].array_index = i;
                info_ptr->match_num++;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].match_item_type = MMISEARCH_TYPE_CL;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].match_item_style = item_style;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].user_data = match_data_ptr->match_num;//用于保存该记录在通话记录中的索引值
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].open_pfunc = CL_OpenMatchedItem;
                match_data_ptr->match_item_ptr[match_data_ptr->match_num].set_data_pfunc = CL_SetMatchItemData;

                match_data_ptr->match_num++;
                
                if (match_data_ptr->match_num >= max_num)
                {
                    SCI_FREE(arraycall_info);
                    return;
                }
            }
        }

        SCI_FREE(arraycall_info);
    }
 
    return ;
}

/*****************************************************************************/
// 	Description : 本地搜索退出，通话记录模块释放模块内部资源
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_FreeLocalSearchResource(void)
{    
    if (PNULL != s_match_call_info_ptr)
    {
        SCI_FREE(s_match_call_info_ptr);
    }
}
#endif
#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
//  Description : 获得列表控件中被选中的item数目
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/

PUBLIC uint16 MMICL_GetSelectedItemCount(void)   //RETURN: 被选中的ITEM的总数
{
    return s_call_log_all_select_info.sel_count;
}

/*****************************************************************************/
//  Description : if the selected item is valid
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_SelectedListIsValid(uint16 index)
{
    return s_call_log_all_select_info.sel_item[index].is_valid;
}

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC void MMICL_GetSelectPhonenum(
                            MMIPB_BCD_NUMBER_T*   bcd_number ,//out
                            uint16 sel_item_index
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    uint32 array_index = 0;
	
	if (PNULL == bcd_number)
	{
	    return;
	}
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_13268_112_2_18_2_11_55_113,(uint8*)"");
        return;
    }

    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), sel_item_index, &array_index);
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[sel_item_index].array_index;
    }
    else
    {
        sel_item_index = array_index;
    }
    
    if (arraycall_info->call_bcdinfo[sel_item_index].is_num_present)//电话号码存在
    {
        //get number
        bcd_number->number_len = arraycall_info->call_bcdinfo[sel_item_index].number_len;
        bcd_number->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[sel_item_index].number_type,
        arraycall_info->call_bcdinfo[sel_item_index].number_plan);
        MMI_MEMCPY(bcd_number->number,MMIPB_BCD_NUMBER_MAX_LEN,
            arraycall_info->call_bcdinfo[sel_item_index].number,MMIPB_BCD_NUMBER_MAX_LEN,
            bcd_number->number_len);
    }
    
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
    }
}

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC void MMICL_GetSelectPhoneName(
                            MMI_STRING_T*   call_name, //out
                            uint16 sel_item_index,
                            uint16 max_name_len,
                            MMIPB_BCD_NUMBER_T    *bcd_number
                            )
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    uint32 array_index = 0;
	
	if (PNULL == call_name || PNULL == bcd_number)
	{
	    return;
	}
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetPartyPhoneNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_13324_112_2_18_2_11_55_114,(uint8*)"");
        return;
    }

    //read logs of calls from NV
    GUILIST_GetItemData(MMICL_GetLogListCtrlId(s_cur_sel_log_list_type), sel_item_index, &array_index);
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    if(MMICL_CALL_MATCHED == s_cur_sel_log_list_type && PNULL != s_match_call_info_ptr)
    {
        sel_item_index = s_match_call_info_ptr->call_info[sel_item_index].array_index;
    }
    else
    {
        sel_item_index = array_index;
    }

#if defined(CL_SAVE_NAME_SUPPORT)
    if (arraycall_info->call_bcdinfo[sel_item_index].name.is_use_nv_name)
    {
        call_name->wstr_len = arraycall_info->call_bcdinfo[sel_item_index].name.wstr_len;
        MMI_WSTRNCPY(call_name->wstr_ptr, 
                                    MMICL_NAME_MAX_LEN, 
                                    arraycall_info->call_bcdinfo[sel_item_index].name.wstr, 
                                    MMICL_NAME_MAX_LEN, 
                                    call_name->wstr_len);
    
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
    }
    else
#endif
    {
        MMIAPIPB_GetNameByNumber(bcd_number,
								 call_name,
								 max_name_len,
								 FALSE);
        //call_name->wstr_len = 0;
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
    }
}
#endif

/*****************************************************************************/
//  Description : Generate to the string number
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMICL_GenDispNumber(
                                        MMICL_CALL_TYPE_E call_type,
                                        MN_NUMBER_TYPE_E  number_type,
                                        uint8             party_len,
                                        uint8             *party_num,
                                        uint8             *tele_num,
                                        uint16            max_tele_len//!!!!!!!!!可显示的号码的最大长度 + 2(/0 字符串结束符号)
                                     )
{
    uint16 tele_len = 0;

    if (MMICL_CALL_MISSED == call_type
        || MMICL_CALL_RECEIVED == call_type
#if defined(BT_DIALER_SUPPORT)
        || MMICL_BT_CALL_MISSED == call_type /*lint !e774*/
        || MMICL_BT_CALL_RECEIVED == call_type /*lint !e774*/
#endif
#if defined(MMI_BLACKLIST_SUPPORT)
        || MMICL_CALL_REFUSED == call_type
#endif
        )
    {
        tele_len = MMIAPICOM_GenNetDispNumber(number_type,
                                                                    party_len,
                                                                    party_num, 
                                                                    tele_num,
                                                                    max_tele_len);
    }
    else
    {
        tele_len = MMIAPICOM_GenDispNumber(number_type,
                                                                    party_len,
                                                                    party_num, 
                                                                    tele_num,
                                                                    max_tele_len);
    }

    return tele_len;
}

#ifdef CALL_TIMES_SUPPORT
/*****************************************************************************/
// 	Description : to delete one call times list
//	Global resource dependence : 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_DeleteOneCallTimesItem(
                                        MMICL_CALL_TYPE_E    call_type,  //call 类型
                                        uint16 select_index    
                                        )
{
    uint8                       i                     = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info        = PNULL;
    uint16                      real_call_times_index = 0;
    uint16                      array_index           = 0;
    uint8                       pos                   = 0;

    if(call_type >= MMICL_CALL_MATCHED)
    {
        //SCI_TRACE_LOW:"MMICL_RecordCallInfo: the type of call is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_535_112_2_18_2_11_16_1,(uint8*)"");
        return;
    }

    //init arraycall_info
    arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_686_112_2_18_2_11_16_4,(uint8*)"");
        return;
    }
       
    array_index = CL_GetArrayIndexofCurSelected();
    
    //get the value of arraycall_info from nv
    MMICL_ReadNV(call_type, arraycall_info);

    //Convert Index to Array Index
    pos = arraycall_info->call_bcdinfo[array_index].array_index;

    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    //get the value of arraycall_info from nv
    MMICL_ReadNV(MMICL_CALL_ALL, arraycall_info);
    
#ifdef MMI_LDN_SUPPORT
    //delete ldn
    //if (arraycall_info->call_bcdinfo[pos].is_sim_record)
    {
        MMICL_CALL_INFO_T call_info = {0};
        
        call_info.dual_sys = arraycall_info->call_bcdinfo[pos].dual_sys;
        call_info.call_num.is_num_present = arraycall_info->call_bcdinfo[pos].is_num_present;
        call_info.call_num.number.number_plan = arraycall_info->call_bcdinfo[pos].number_plan;
        call_info.call_num.number.number_type = arraycall_info->call_bcdinfo[pos].number_type;
        call_info.call_num.number.num_len = MIN(arraycall_info->call_bcdinfo[pos].number_len, MN_MAX_ADDR_BCD_LEN);
        SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[pos].number, call_info.call_num.number.num_len);
        
        if (arraycall_info->call_bcdinfo[pos].is_sim_record && 0 < arraycall_info->sim_record_num)
        {
            arraycall_info->sim_record_num--;
        }
        
        MMICL_DeleteCallInfo(s_cur_sel_log_list_type, call_info.dual_sys, &call_info);
    }
#endif
    
    //删除指定位置的电话记录
    real_call_times_index  = arraycall_info->call_bcdinfo[pos].call_times - select_index - 1;
    arraycall_info->call_bcdinfo[pos].call_start_time[real_call_times_index] = 0;
    arraycall_info->call_bcdinfo[pos].call_duration_time[real_call_times_index] =0;
    arraycall_info->call_bcdinfo[pos].call_times--;
    
    //次数减到0，删除该联系人的记录,  返回
    if(0 == arraycall_info->call_bcdinfo[pos].call_times)
    {
        DeleteCurrentRecord();
        MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);
        SCI_FREE(arraycall_info);
        return;
    }

    //后面所有的时间向前移一位
    for(i=0; i < select_index; i++)
    {
        arraycall_info->call_bcdinfo[pos].call_start_time[real_call_times_index + i] =
            arraycall_info->call_bcdinfo[pos].call_start_time[real_call_times_index+i+1];
        arraycall_info->call_bcdinfo[pos].call_duration_time[real_call_times_index + i]= 
            arraycall_info->call_bcdinfo[pos].call_duration_time[real_call_times_index+i+1];
    }

    //最后的时间置0
    arraycall_info->call_bcdinfo[pos].call_start_time[arraycall_info->call_bcdinfo[pos].call_times]    = 0;
    arraycall_info->call_bcdinfo[pos].call_duration_time[arraycall_info->call_bcdinfo[pos].call_times] = 0;
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
       
    MMICL_UpdateEventInd();   
    
    
    SCI_FREE(arraycall_info);
}
#endif
#ifdef MMI_CL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : Set log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected)
{
    uint8 i = 0;
    uint8 mark_count = 0;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    if (index >= GUILIST_GetTotalItemNum(list_id))
    {
        return;
    }

    s_select_info.log_selected_index[index] = isSelected;

    //当取消标记时，如果当前控件中所有元素都取消了标记，需要把is_select的状态置为FALSE
    if (!isSelected)
    {
        for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
        {
            if(s_select_info.log_selected_index[i])
                
                mark_count++;
        }
        
        if (0 == mark_count)
        {
            s_select_info.is_select = FALSE;
        }
    }
}

/*****************************************************************************/
// 	Description : Set all log list item for mark or unmark
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetAllSelect(BOOLEAN isSelected)
{
    int i = 0;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
    {
        s_select_info.log_selected_index[i] = isSelected;
    }
}

/*****************************************************************************/
// 	Description : Exit mark for Set all log list item
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitSelect(void)
{
    s_select_info.is_select = FALSE;

    SCI_MEMSET(s_select_info.log_selected_index, 0, sizeof(s_select_info.log_selected_index));
}

/*****************************************************************************/
// 	Description : to delete marked call log record item
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void DeleteRecord(void)
{
    uint16                      i                       = 0;
    uint16                      j                       = 0;
    uint16                      pos                     = 0;
    uint16                      record_count            = 0;
    uint16                      record_count_copy       = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info          = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T* all_arraycall_info      = PNULL;
    MMICL_CALL_ARRAY_BCDINFO_T* all_arraycall_info_copy = PNULL;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    
    //init arraycall_info
    arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    all_arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    all_arraycall_info_copy = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info || PNULL == all_arraycall_info || PNULL == all_arraycall_info_copy)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_686_112_2_18_2_11_16_4,(uint8*)"");

        if (arraycall_info != PNULL)
        {
            SCI_FREE(arraycall_info);
        }

        if (all_arraycall_info != PNULL)
        {
            SCI_FREE(all_arraycall_info);
        }

        if (all_arraycall_info_copy != PNULL)
        {
            SCI_FREE(all_arraycall_info_copy);
        }
        return;
    }
    
    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(all_arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(all_arraycall_info_copy, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    //get the value of arraycall_info from nv
    MMICL_ReadNV(s_cur_sel_log_list_type, arraycall_info);
    MMICL_ReadNV(MMICL_CALL_ALL, all_arraycall_info);
    
    record_count_copy = all_arraycall_info->record_num;
    record_count = all_arraycall_info->record_num;

    for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
    {
        if(s_select_info.log_selected_index[i])

        {
            pos = arraycall_info->call_bcdinfo[i].array_index;
 
            //需要删除位置的电话记录置零
            SCI_MEMSET(&(all_arraycall_info->call_bcdinfo[pos]),0,sizeof(MMICL_CALL_BCDINFO_T));
#ifdef MMI_LDN_SUPPORT
            //delete ldn
            //if (arraycall_info->call_bcdinfo[pos].is_sim_record)
            {
                MMICL_CALL_INFO_T call_info = {0};
                
                call_info.dual_sys = arraycall_info->call_bcdinfo[pos].dual_sys;
                call_info.call_num.is_num_present = arraycall_info->call_bcdinfo[pos].is_num_present;
                call_info.call_num.number.number_plan = arraycall_info->call_bcdinfo[pos].number_plan;
                call_info.call_num.number.number_type = arraycall_info->call_bcdinfo[pos].number_type;
                call_info.call_num.number.num_len = MIN(arraycall_info->call_bcdinfo[pos].number_len, MN_MAX_ADDR_BCD_LEN);
                SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[pos].number, call_info.call_num.number.num_len);
                
                if (arraycall_info->call_bcdinfo[pos].is_sim_record && 0 < arraycall_info->sim_record_num)
                {
                    arraycall_info->sim_record_num--;
                }
                
                MMICL_DeleteCallInfo(s_cur_sel_log_list_type, call_info.dual_sys, &call_info);
            }
#endif
 
            record_count--;
        }
    }
    
    //删除标记的元素，即把置零的元素去掉
    for (i = 0, j = 0; j < record_count_copy; j++)
    {    
        if (all_arraycall_info->call_bcdinfo[j].is_num_present)
        {
            SCI_MEMCPY(&all_arraycall_info_copy->call_bcdinfo[i++], &all_arraycall_info->call_bcdinfo[j],sizeof(MMICL_CALL_BCDINFO_T));
        }
    }

    all_arraycall_info_copy->record_num = record_count;
    
    MMICL_WriteNV(MMICL_CALL_ALL, all_arraycall_info_copy);
    
    MMICL_UpdateEventInd();   

    s_select_info.is_select = FALSE;
    
    SCI_FREE(arraycall_info);
    SCI_FREE(all_arraycall_info);
    SCI_FREE(all_arraycall_info_copy);
}

/*****************************************************************************/
// 	Description : set menu grayed
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_LogListMenuGrayed(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);
    if (s_select_info.is_select)
    {
        if (GetTotalItemIsMark())
        {
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,FALSE);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,FALSE);
        }
        else
        {
            if (s_select_info.log_selected_index[GUILIST_GetCurItemIndex(list_id)])
            {
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,TRUE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,TRUE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,FALSE);
                GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,FALSE);
            }

        }
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARK_LIST,FALSE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCEL_MARK_LIST,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_MARKALL_LIST,FALSE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_CL_LIST_MARK_OPT,ID_CL_CANCLE_MARKALL_LIST,TRUE);
    }

}

/*****************************************************************************/
// 	Description : ALL item is marked
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetTotalItemIsMark(void)
{
    int i = 0;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(s_cur_sel_log_list_type);

    for(i = 0; i < GUILIST_GetTotalItemNum(list_id); i++)
    {
         if (!s_select_info.log_selected_index[i])
         {
             return FALSE;
         }
    }

    return TRUE;
}
#endif
#ifdef MMI_CALLLOG_SUPPORT_MULTI_DELETE
LOCAL MMI_RESULT_E HandleChildLogListDeleteSelectedQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
        //
        DeleteAllCallLog(s_cur_sel_log_list_type);
        MMK_CloseWin(win_id);
        MMK_CloseWin(MMICL_CHILD_SELECT_WIN_ID);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : start multiselect 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void BeginMultiSelecting(void)
{
    s_is_multiselecting = TRUE;
    SCI_MEMSET(s_log_selected_index,0,sizeof(s_log_selected_index));
    return;
}
/*****************************************************************************/
// 	Description : exit multiselect
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ExitMultiSelecting(void)
{
    s_is_multiselecting = FALSE;
    SCI_MEMSET(s_log_selected_index,0,sizeof(s_log_selected_index));
    return;
}
/*****************************************************************************/
// 	Description : to is multiselecting
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isMultiSelecting(void)
{
    return s_is_multiselecting ;
}

/*****************************************************************************/
// 	Description : check current index is selecting or not
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN isCurrentSelected(uint16 index)
{
    
    return s_log_selected_index[index];
}
/*****************************************************************************/
// 	Description : Set current index 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelect(uint16 index,BOOLEAN isSelected)
{
    s_log_selected_index[index]= isSelected;
    
    return;
}

/*****************************************************************************/
// 	Description : Set log list for multiselecting  
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void SetLogListItemForSelect(MMI_CTRL_ID_T ctrl_id)
{
    uint16 sel_item_index = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"SetLogListItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_WINTAB_2626_112_2_18_2_11_32_54,(uint8*)"");
        return;
    }
    
    //get the current selected item index
    //当电话记录列表已经打开时，重新刷新列表需要重新定位当前被选中的item!
    
    sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
    
    //read logs of calls from NV
    MMICL_GetRecordInfo(s_cur_sel_log_list_type, arraycall_info);
    
    //load log list
    GUILIST_SetMaxItem(ctrl_id, MMICL_RECORD_MAX_NUM, FALSE );//max item 20

    AppendLogListItem(ctrl_id, s_cur_sel_log_list_type, arraycall_info);
    
    GUILIST_SetCurItemIndex(ctrl_id, sel_item_index);
    
    SCI_FREE(arraycall_info);
}
/*****************************************************************************/
// 	Description : multiselecting  win handler
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildCallLogSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	list_ctrl_id = MMICL_CHILD_SELECT_LIST_CTRL_ID;
    MMI_WIN_ID_T query_win_id = MMICL_QUERY_WIN_ID;
    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        { 
            BeginMultiSelecting();
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id,MMICL_RECORD_MAX_NUM);
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        SetLogListItemForSelect(list_ctrl_id);
        break;

    case MSG_GET_FOCUS:
        break;
    case MSG_CTL_LIST_MARK_ITEM:
        {
            uint32 sel_item_index =0;
            BOOLEAN is_selected = FALSE;

            GUILIST_GetItemData(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), &sel_item_index);
            is_selected = isCurrentSelected(sel_item_index);
			is_selected = !is_selected;
            SetCurrentSelect(sel_item_index, is_selected);
            GUILIST_SetSelectedItem(list_ctrl_id,sel_item_index,is_selected);
            break;
        }

    case MSG_CTL_OK:
    case MSG_APP_OK:        
    case MSG_APP_MENU:
        //Delete selected record
        if (GUILIST_GetSelectedItemNum(list_ctrl_id)<1)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,HandleChildLogListDeleteSelectedQueryWinMsg);                
        }
        
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        ExitMultiSelecting();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL void SetItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index)
{
    THEMELIST_ITEM_STYLE_T* itemstyle_ptr = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT);
    if (s_itemstyle_ptr)
    {
        SCI_MEMCPY(s_itemstyle_ptr, itemstyle_ptr, sizeof(*s_itemstyle_ptr));
        s_itemstyle_ptr->content[1].rect.right = MMI_MAINSCREEN_WIDTH/4*3 - MMI_LIST_ICON_LEFT_SPACE;
		s_itemstyle_ptr->content[1].rect_focus.right = MMI_MAINSCREEN_WIDTH/4*3 - MMI_LIST_ICON_LEFT_SPACE;
        s_itemstyle_ptr->content[2].rect.left = MMI_MAINSCREEN_WIDTH/4*3 + MMI_LIST_ICON_LEFT_SPACE * 2;
		s_itemstyle_ptr->content[2].rect_focus.left = MMI_MAINSCREEN_WIDTH/4*3 + MMI_LIST_ICON_LEFT_SPACE * 2;
        GUILIST_SetItemStyleEx(ctrl_id, index, s_itemstyle_ptr);
    }
}
#endif

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : Is CL LDN Record
//	Global resource dependence: 
//  Author:bingjie.chen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSimRecord(MMICL_CALL_ARRAY_BCDINFO_T *callinfo)
{
    uint16 array_index =0;
    
    if (callinfo == PNULL)
    {
        return FALSE;
    }
    
    array_index = CL_GetArrayIndexofCurSelected();
    
    if (callinfo->call_bcdinfo[array_index].is_sim_record)
    {
        return TRUE;
    }
    
    return FALSE;
    
}
#endif
