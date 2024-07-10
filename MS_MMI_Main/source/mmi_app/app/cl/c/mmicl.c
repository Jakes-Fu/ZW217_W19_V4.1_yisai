/*****************************************************************************
** File Name:      mmicl.c                                                   *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng       Create
******************************************************************************/

#define _MMICL_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cl_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiphone_export.h"
#include "guilistbox.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmicc_id.h"
#include "mmicc_nv.h"
#include "mmicl_internal.h"
//#include "mmicc_text.h"
#include "mmitheme_edit.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#if defined(IKEYBACKUP_SUPPORT)
#include "mmiikeybackup_api.h"
#endif

#include "nvitem.h"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_dial_cl_contact_export.h"
#endif

#ifdef BT_PBAP_SUPPORT
#include "mmivcard_export.h"
#include "mmipb_task.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define CALENDER_MONTH_MAX 12
#define CALENDER_DATE_MAX 31
#define CALENDER_HOUR_MAX 23
#define CALENDER_MIN_MAX 59
#define CALENDER_SCECOND_MAX 59

#define CALENDER_MONTH_MIN 1
#define CALENDER_DATE_MIN 1
#define CALENDER_HOUR_MIN 0
#define CALENDER_MIN_MIN 0
#define CALENDER_SCECOND_MIN 0

#define CALL_TIMES_MAX 65535

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL CC_CALL_TIME_CONTEXT_T  g_call_time_context = {0};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//已拨电话中搜索匹配的号码项
typedef struct
{
    int32 index; //已拨电话的对应条目
    MN_DUAL_SYS_E dual_sys;
    MN_CALLED_NUMBER_T number;
    CC_CALL_MODE_E call_mode;	//@zhaohui add,is VT call or normal call 
    BOOLEAN is_emergency_call; //是否是紧急号码呼出
#if defined(CL_SAVE_NAME_SUPPORT)
    MMICL_NAME_T name;  //call log中存储的通话姓名
#endif
    MMICL_CALL_TYPE_E   call_type;
#ifdef CALL_TIMES_SUPPORT
    uint16                   call_times;
#endif
}NUM_SEARCH_ITEM;

#ifdef BT_PBAP_SUPPORT
typedef struct PbapDataList{
    void    *data;     //节点数据
    uint16  len;       //数据长度
    struct PbapDataList *next; //指向下个节点
}PbapDataList;

#define PBAP_DATA_LIST_LEN              sizeof(PbapDataList)
#define VCARD_BEGIN_TAGS                "BEGIN:VCARD"   //域开始标签
#define VCARD_END_TAGS                  "END:VCARD"     //域结束标签

#endif

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct
{
NUM_SEARCH_ITEM s_mmicl_search_list[MMICL_RECORD_TOTAL_NUM];
}NUM_SEARCH_ITEM_CONTENT_T;

//记录号码匹配搜索信息
LOCAL NUM_SEARCH_ITEM_CONTENT_T *s_mmicl_search_list_ptr = PNULL;

#ifdef BT_PBAP_SUPPORT
LOCAL PbapDataList                      *s_list_head = PNULL;
LOCAL char                              *s_vcard_buff_ptr = PNULL;      //记录不完整的vcard数据
LOCAL MMICL_CALL_ARRAY_BCDINFO_T        *s_arraycall_info_ptr = PNULL;  //calllog data
LOCAL uint32                            s_sync_calllog_count = 0xFFFF;       //total add callog count
LOCAL uint32                            s_node_number = 1;
LOCAL MMICL_PBAP_PARSER_CALLBACK_FUNC   parser_callback = PNULL;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : add a new record of call in the head of list 
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void AddNewCallInfo(
                          MN_DUAL_SYS_E			dual_sys,  // Indicate the selected system
                          MMICL_CALL_TYPE_E             call_type,    //call 类型
                          MMICL_CALL_INFO_T             call_info     //call 信息
                          );

/*****************************************************************************/
// 	Description : add a new detail record of call in the head of list 
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL uint8 AddNewDetailCallInfo(
                                 MN_DUAL_SYS_E			dual_sys,  // Indicate the selected system
                                 MMICL_CALL_ARRAY_BCDINFO_T  *arraycall_info_ptr,
                                 MMICL_CALL_INFO_T           call_info  ,   //call 信息
                                 MMICL_CALL_TYPE_E   call_type
                                 );
#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : record the time of call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordCallTime(
                          uint8 line, //call line
                          MMICL_CALL_TYPE_E     call_type,    //call ??
                          CC_CALL_TIME_T        call_time     //call ??
                          );

/*****************************************************************************/
// 	Description : record the time of the last call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordLastCallTime(
                              uint8 line, //call line
                              CC_CALL_TIME_T   call_time//call ??
                              );

/*****************************************************************************/
// 	Description : record the time of all calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllCallsTime(
                              uint8 line, //call line
                              CC_CALL_TIME_T   call_time//call ??
                              );

/*****************************************************************************/
// 	Description : record the time of all received calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllReceivedCallsTime(
                                      uint8 line, //call line
                                      CC_CALL_TIME_T   call_time//call ??
                                      );

/*****************************************************************************/
// 	Description : record the time of all dialed calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllDialedCallsTime(
                                    uint8 line, //call line
                                    CC_CALL_TIME_T   call_time//call ??
                                    );
#else //MMI_CSP_SUPPORT

/*****************************************************************************/
// 	Description : record the time of call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordCallTime(
                          MMICL_CALL_TYPE_E     call_type,    //call 类型
                          CC_CALL_TIME_T        call_time     //call 时间
                          );

/*****************************************************************************/
// 	Description : record the time of the last call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordLastCallTime(
                              CC_CALL_TIME_T   call_time//call 时间
                              );

/*****************************************************************************/
// 	Description : record the time of all calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllCallsTime(
                              CC_CALL_TIME_T   call_time//call 时间
                              );

/*****************************************************************************/
// 	Description : record the time of all received calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllReceivedCallsTime(
                                      CC_CALL_TIME_T   call_time//call 时间
                                      );

/*****************************************************************************/
// 	Description : record the time of all dialed calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllDialedCallsTime(
                                    CC_CALL_TIME_T   call_time//call 时间
                                    );
#endif //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : write call info nv
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL uint32 GetCallDurationTime( uint32 start_time, uint32 stop_time);

#if defined(IKEYBACKUP_SUPPORT)
/*****************************************************************************/
//  Description : 1key backup call back function for backup
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL void calllog_backup_callback(
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T *param_out_ptr
);

/*****************************************************************************/
//  Description : 1key backup call back function for restore
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL void calllog_restore_callback(
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T *param_out_ptr
);

/*****************************************************************************/
//  Description : 1key backup call back function for get total num
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL void cc_get_total_callback(
    MMIIKB_GET_TOTAL_OUT_T *param_out_ptr
);

/*****************************************************************************/
//  Description : restore backup info 
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL BOOLEAN RestoreCallLog(MMICL_CALL_ARRAY_BCDINFO_T * backup_call_info_ptr);
#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
// 	Description : free all pbap data
//	Global resource dependence : none
//  Author: cheney.wu
//	Note:
/*****************************************************************************/
LOCAL void FinialPbapParse(void);

/*****************************************************************************/
// 	Description : free all pbap data
//	Global resource dependence : none
//  Author: cheney.wu
//	Note:
/*****************************************************************************/
LOCAL void FreeAllPbapData(void);

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL PbapDataList* InitList(void);
/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int InsertList(PbapDataList *L, void* buff,uint16 buff_len);

/*******************************************************************************/
//  Description:清除线性表L中的所有元素，即释放单链表L中所有的结点，使之成为一个空表
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL void ClearList(PbapDataList *pHead);
/*******************************************************************************/
//  Description:获取单链表中第i个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int GetElem(PbapDataList *L, int i, void** buff, uint16 *len);
/*******************************************************************************/
//  Description:判断单链表是否为空
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int IsListEmpty(PbapDataList *L);

/*******************************************************************************/
//  Description:判断链表节点个数
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int ListNodeNum(PbapDataList *L);
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set dafault call times
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_SetDefaultCallTime(void)
{
    SCI_MEMSET(g_call_time_context.last_call_time, 0, sizeof(g_call_time_context.last_call_time));
    SCI_MEMSET(g_call_time_context.all_calls_time, 0, sizeof(g_call_time_context.all_calls_time));
    SCI_MEMSET(g_call_time_context.received_calls_time, 0, sizeof(g_call_time_context.received_calls_time));
    SCI_MEMSET(g_call_time_context.dialed_calls_time, 0, sizeof(g_call_time_context.dialed_calls_time));
}

/*****************************************************************************/
// 	Description : set dafault call times by sim
//	Global resource dependence : none
//    Author: tao.xue
//	Note:
/*****************************************************************************/
void MMIAPICL_SetDefaultCallTimeToNVBySim(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        SCI_MEMSET((CC_CALL_TIME_COUNT_T*)&g_call_time_context.last_call_time[dual_sys], 0, sizeof(CC_CALL_TIME_COUNT_T));
        SCI_MEMSET((CC_CALL_TIME_COUNT_T*)&g_call_time_context.all_calls_time[dual_sys], 0, sizeof(CC_CALL_TIME_COUNT_T));
        SCI_MEMSET((CC_CALL_TIME_COUNT_T*)&g_call_time_context.received_calls_time[dual_sys], 0, sizeof(CC_CALL_TIME_COUNT_T));
        SCI_MEMSET((CC_CALL_TIME_COUNT_T*)&g_call_time_context.dialed_calls_time[dual_sys], 0, sizeof(CC_CALL_TIME_COUNT_T));
        MMINV_WRITE(MMINV_CL_LASTCALL_TIME, g_call_time_context.last_call_time);
        MMINV_WRITE(MMINV_CL_ALLCALLS_TIME, g_call_time_context.all_calls_time);
        MMINV_WRITE(MMINV_CL_RECEIVEDCALLS_TIME, g_call_time_context.received_calls_time);
        MMINV_WRITE(MMINV_CL_DIALEDCALLS_TIME, g_call_time_context.dialed_calls_time);
    }
}

/*****************************************************************************/
// 	Description : set dafault call times
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_SetDefaultCallTimeToNV(void)
{
    SCI_MEMSET(g_call_time_context.last_call_time, 0, sizeof(g_call_time_context.last_call_time));
    SCI_MEMSET(g_call_time_context.all_calls_time, 0, sizeof(g_call_time_context.all_calls_time));
    SCI_MEMSET(g_call_time_context.received_calls_time, 0, sizeof(g_call_time_context.received_calls_time));
    SCI_MEMSET(g_call_time_context.dialed_calls_time, 0, sizeof(g_call_time_context.dialed_calls_time));
    MMINV_WRITE(MMINV_CL_LASTCALL_TIME, g_call_time_context.last_call_time);
    MMINV_WRITE(MMINV_CL_ALLCALLS_TIME, g_call_time_context.all_calls_time);
    MMINV_WRITE(MMINV_CL_RECEIVEDCALLS_TIME, g_call_time_context.received_calls_time);
    MMINV_WRITE(MMINV_CL_DIALEDCALLS_TIME, g_call_time_context.dialed_calls_time);
}

/*****************************************************************************/
// 	Description : init the calltime context
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_InitCallTimeContext(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    if( !g_call_time_context.last_call_time_valid)
    {
        MMINV_READ(MMINV_CL_LASTCALL_TIME, g_call_time_context.last_call_time, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(g_call_time_context.last_call_time, 0, sizeof(g_call_time_context.last_call_time));
            MMINV_WRITE(MMINV_CL_LASTCALL_TIME, g_call_time_context.last_call_time);
        }
        g_call_time_context.last_call_time_valid = TRUE;
    }

    if( !g_call_time_context.all_calls_time_valid)
    {
        MMINV_READ(MMINV_CL_ALLCALLS_TIME, g_call_time_context.all_calls_time, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(g_call_time_context.all_calls_time, 0, sizeof(g_call_time_context.all_calls_time));
            MMINV_WRITE(MMINV_CL_ALLCALLS_TIME, g_call_time_context.all_calls_time);
        }
        g_call_time_context.all_calls_time_valid = TRUE;
    }

    if( !g_call_time_context.received_calls_time_valid)
    {
        MMINV_READ(MMINV_CL_RECEIVEDCALLS_TIME, g_call_time_context.received_calls_time, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(g_call_time_context.received_calls_time, 0, sizeof(g_call_time_context.received_calls_time));
            MMINV_WRITE(MMINV_CL_RECEIVEDCALLS_TIME, g_call_time_context.received_calls_time);
        }
        g_call_time_context.received_calls_time_valid = TRUE;
    }

    if( !g_call_time_context.dialed_calls_time_valid)
    {
        MMINV_READ(MMINV_CL_DIALEDCALLS_TIME, g_call_time_context.dialed_calls_time, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(g_call_time_context.dialed_calls_time, 0, sizeof(g_call_time_context.dialed_calls_time));
            MMINV_WRITE(MMINV_CL_DIALEDCALLS_TIME, g_call_time_context.dialed_calls_time);
        }
        g_call_time_context.dialed_calls_time_valid = TRUE;
    }
}
#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : store the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreLastCallTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_StoreLastCallTime err line: %d", line);
    }
    if( !g_call_time_context.last_call_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    if(0 == line)
    {
        g_call_time_context.last_call_time[p_cc_time->dual_sys].line1_call_time = p_cc_time->line1_call_time;
    }
    else if(1 == line)
    {
        g_call_time_context.last_call_time[p_cc_time->dual_sys].line2_call_time = p_cc_time->line2_call_time;
    }
    
    g_call_time_context.last_call_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_LASTCALL_TIME, g_call_time_context.last_call_time);
}

/*****************************************************************************/
// 	Description : get the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetLastCallTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_GetLastCallTime err line: %d", line);
    }

    if( !g_call_time_context.last_call_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    if(0 == line)
    {
        p_cc_time->line1_call_time = g_call_time_context.last_call_time[dual_sys].line1_call_time;
    }
    else if(1 == line)
    {
        p_cc_time->line2_call_time = g_call_time_context.last_call_time[dual_sys].line2_call_time;
    }
    
    p_cc_time->dual_sys = dual_sys;
}


/*****************************************************************************/
// 	Description : store all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreAllCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_StoreAllCallsTime err line: %d", line);
    }

    if( !g_call_time_context.all_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    
    if(0 == line)
    {
        g_call_time_context.all_calls_time[p_cc_time->dual_sys].line1_call_time = p_cc_time->line1_call_time;
    }
    else if(1 == line)
    {
        g_call_time_context.all_calls_time[p_cc_time->dual_sys].line2_call_time = p_cc_time->line2_call_time;
    }

    
    g_call_time_context.all_calls_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_ALLCALLS_TIME, g_call_time_context.all_calls_time);
}

/*****************************************************************************/
// 	Description : get all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetAllCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_GetAllCallsTime err line: %d", line);
    }

    if( !g_call_time_context.all_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    if(0 == line)
    {
        p_cc_time->line1_call_time = g_call_time_context.all_calls_time[dual_sys].line1_call_time;
    }
    else if(1 == line)
    {
        p_cc_time->line2_call_time = g_call_time_context.all_calls_time[dual_sys].line2_call_time;
    }
    p_cc_time->dual_sys = dual_sys;
}

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreReceivedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_StoreReceivedCallsTime err line: %d", line);
    }

    if( !g_call_time_context.received_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    if(0 == line)
    {
        g_call_time_context.received_calls_time[p_cc_time->dual_sys].line1_call_time = p_cc_time->line1_call_time;
    }
    else if(1 == line)
    {
        g_call_time_context.received_calls_time[p_cc_time->dual_sys].line2_call_time = p_cc_time->line2_call_time;
    }
    
    g_call_time_context.received_calls_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_RECEIVEDCALLS_TIME, g_call_time_context.received_calls_time);
}

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetReceivedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_GetReceivedCallsTime err line: %d", line);
    }

    if( !g_call_time_context.received_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    if(0 == line)
    {
        p_cc_time->line1_call_time = g_call_time_context.received_calls_time[dual_sys].line1_call_time;
    }
    else if(1 == line)
    {
        p_cc_time->line2_call_time = g_call_time_context.received_calls_time[dual_sys].line2_call_time;
    }
    
    p_cc_time->dual_sys = dual_sys;
}

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreDialedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_StoreDialedCallsTime err line: %d", line);
    }

    if( !g_call_time_context.dialed_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    if(0 == line)
    {
        g_call_time_context.dialed_calls_time[p_cc_time->dual_sys].line1_call_time = p_cc_time->line1_call_time;
    }
    else if(1 == line)
    {
        g_call_time_context.dialed_calls_time[p_cc_time->dual_sys].line2_call_time = p_cc_time->line2_call_time;
    }
    g_call_time_context.dialed_calls_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_DIALEDCALLS_TIME, g_call_time_context.dialed_calls_time);
}

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetDialedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if(line > 1)
    {
        SCI_TRACE_LOW("MMIAPICL_GetDialedCallsTime err line: %d", line);
    }

    if( !g_call_time_context.dialed_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    if(0 == line)
    {
        p_cc_time->line1_call_time = g_call_time_context.dialed_calls_time[dual_sys].line1_call_time;
    }
    else if(1 == line)
    {
        p_cc_time->line2_call_time = g_call_time_context.dialed_calls_time[dual_sys].line2_call_time;
    }
    p_cc_time->dual_sys = dual_sys;
}
#else //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : store the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreLastCallTime(CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if( !g_call_time_context.last_call_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    g_call_time_context.last_call_time[p_cc_time->dual_sys].call_time = p_cc_time->call_time;
    g_call_time_context.last_call_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_LASTCALL_TIME, g_call_time_context.last_call_time);
}

/*****************************************************************************/
// 	Description : get the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetLastCallTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if( !g_call_time_context.last_call_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    p_cc_time->call_time = g_call_time_context.last_call_time[dual_sys].call_time;
    p_cc_time->dual_sys = dual_sys;
}


/*****************************************************************************/
// 	Description : store all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreAllCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if( !g_call_time_context.all_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    g_call_time_context.all_calls_time[p_cc_time->dual_sys].call_time = p_cc_time->call_time;
    g_call_time_context.all_calls_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_ALLCALLS_TIME, g_call_time_context.all_calls_time);
}

/*****************************************************************************/
// 	Description : get all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetAllCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if( !g_call_time_context.all_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    p_cc_time->call_time = g_call_time_context.all_calls_time[dual_sys].call_time;
    p_cc_time->dual_sys = dual_sys;
}

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreReceivedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if( !g_call_time_context.received_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    g_call_time_context.received_calls_time[p_cc_time->dual_sys].call_time = p_cc_time->call_time;
    g_call_time_context.received_calls_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_RECEIVEDCALLS_TIME, g_call_time_context.received_calls_time);
}

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetReceivedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if( !g_call_time_context.received_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    p_cc_time->call_time = g_call_time_context.received_calls_time[dual_sys].call_time;
    p_cc_time->dual_sys = dual_sys;
}

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreDialedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time)
{
    if( !g_call_time_context.dialed_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    g_call_time_context.dialed_calls_time[p_cc_time->dual_sys].call_time = p_cc_time->call_time;
    g_call_time_context.dialed_calls_time[p_cc_time->dual_sys].dual_sys = p_cc_time->dual_sys;
    MMINV_WRITE(MMINV_CL_DIALEDCALLS_TIME, g_call_time_context.dialed_calls_time);
}

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetDialedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys)
{
    if( !g_call_time_context.dialed_calls_time_valid)
    {
        MMIAPICL_InitCallTimeContext();
    }
    p_cc_time->call_time = g_call_time_context.dialed_calls_time[dual_sys].call_time;
    p_cc_time->dual_sys = dual_sys;
}
#endif //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : initialize call log
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMIAPICL_Init(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T *call_info = PNULL;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
        
    call_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == call_info)
    {
        //SCI_TRACE_LOW:"MMIAPICL_Init call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_424_112_2_18_2_11_16_0,(uint8*)"");
        return;
    }
    
    //SCI_MEMSET(call_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    if (MMIAPIPHONE_IsSameSim(MN_DUAL_SYS_1))//同一张sim卡
#endif
    {
        //初始化未接电话数目
        MMIAPICC_LoadMissedCallNumFromNv();

#ifdef PDA_UI_DROPDOWN_WIN
        if(0 != MMIAPICC_GetMissedCallNumber())
        {
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_MISS_CALL);
        }
#endif
        return_value =MMICL_ReadAllNV(call_info);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(call_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
            MMICL_WriteNV(MMICL_CALL_ALL, call_info);
        }


        //初始化最近通话时间        
        //初始化所有通话时间        
        //初始化已接通话时间        
        //初始化已拨通话时间
        MMIAPICL_InitCallTimeContext();

    }
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        //初始化未接电话数目
        MMIAPICC_ClearMissedCallNum();
        //初始化未接电话


        SCI_MEMSET(call_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        MMICL_WriteNV(MMICL_CALL_ALL, call_info);
        //初始化最近通话时间        
        //初始化所有通话时间        
        //初始化已接通话时间        
        //初始化已拨通话时间
        MMIAPICL_SetDefaultCallTimeToNV();
    }
#endif

    MMK_PBRegisterEvent(MMICL_HandlePBEventFunc);
#ifdef ADULT_WATCH_SUPPORT
#ifdef DROPDOWN_NOTIFY_SHORTCUT
    WatchCallLog_Init();
#endif
#endif
    //if (PNULL != call_info)
    {
        SCI_FREE(call_info);
    }
}

/*****************************************************************************/
// 	Description : record the information of call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMIAPICL_RecordCallInfo(
                             MN_DUAL_SYS_E  dual_sys,          //SIM id
                             MMICL_CALL_TYPE_E     call_type,  //call 类型
                             MMICL_CALL_INFO_T     call_info   //call 信息
                             )
{
    //cr138231:网络回'a' 'b' 'c' 'd' 'e'等非sim卡支持字段，在存入call log的时候，会记录成空号
    if ((MMICL_CALL_MISSED == call_type)
        ||(MMICL_CALL_RECEIVED == call_type)
#if defined(MMI_BLACKLIST_SUPPORT)
        ||(MMICL_CALL_REFUSED == call_type)
#endif
        )
    {
        uint8   tele_num[CC_MAX_TELE_NUM_LEN + 2]= {0};
        uint8	tel_num_len = 0;
        if( call_info.call_num.is_num_present )
        {        
            tel_num_len =  MMIAPICOM_GenNetDispNumber( call_info.call_num.number.number_type,
                                        (uint8)call_info.call_num.number.num_len,
                                        (uint8*)call_info.call_num.number.party_num,
                                        tele_num,
                                        (CC_MAX_TELE_NUM_LEN + 2));

            //if (!MMIAPICOM_IsValidNumberString((char *)tele_num, tel_num_len))
            if (0 == tel_num_len)
            {
                call_info.call_num.is_num_present = FALSE;
            }
        }
    }
    call_info.dual_sys = dual_sys;
    
    MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
    MMK_CloseWin(MMICL_CALL_TIMES_DETAIIL_WIN_ID);
    MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID);  

    if (call_type<MMICL_CALL_MATCHED)
    {
        AddNewCallInfo(dual_sys, call_type,call_info);
        if ((call_type == MMICL_CALL_RECEIVED)||(call_type == MMICL_CALL_DIALED))
        {   
            call_info.call_time.dual_sys = dual_sys;
#ifdef MMI_CSP_SUPPORT
            RecordCallTime(call_info.line, call_type,call_info.call_time);
#else //MMI_CSP_SUPPORT
            RecordCallTime(call_type,call_info.call_time);
#endif //MMI_CSP_SUPPORT
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMICL_RecordCallInfo: the type of call is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_535_112_2_18_2_11_16_1,(uint8*)"");
    }

    MMICL_UpdateEventInd();
}
/*****************************************************************************/
// 	Description : indication to other module that cl has updated
//	Global resource dependence : none
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_UpdateEventInd(void)
{
#ifdef SEARCH_SUPPORT
    MMIAPISEARCH_AppUpdateInd(MMISEARCH_TYPE_CL);
#endif
}

/*****************************************************************************/
// 	Description : delete the appointed record of call 
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
uint8 MMICL_DeleteAppointedCallInfo( MMICL_CALL_TYPE_E    call_type,  //call 类型
                                    uint8                pos 
                                    )
{
     uint8                       i = 0,record_num = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
    uint32 index = 0;
    //判断指定的位置是否出错
    if (pos >= MMICL_RECORD_TOTAL_NUM) //cr35264 jian.ma uint8 类型的数恒为非负数。
    {
        SCI_TRACE_LOW("MMICL_DeleteAppointedCallInfo: the appointed position is %d",pos);
        return record_num;
    }
    
    //init arraycall_info
    arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_576_112_2_18_2_11_16_2,(uint8*)"");
        return 0;
    }
    
    //SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    //get the value of arraycall_info from nv
    return_value = MMICL_ReadAllNV(arraycall_info);
    
    if((arraycall_info->record_num == 0) || (arraycall_info->record_num > MMICL_RECORD_TOTAL_NUM))
    {
        return 0;
    }

    index = pos; //pos is array index now

#ifdef MMI_LDN_SUPPORT
    //delete ldn
    if (pos < MMICL_RECORD_TOTAL_NUM)
    {
     
        MMICL_CALL_INFO_T call_info = {0}; 

        call_info.dual_sys = arraycall_info->call_bcdinfo[pos].dual_sys;
        call_info.call_num.is_num_present = arraycall_info->call_bcdinfo[pos].is_num_present;
        call_info.call_num.number.number_plan = arraycall_info->call_bcdinfo[pos].number_plan;
        call_info.call_num.number.number_type = arraycall_info->call_bcdinfo[pos].number_type;
        call_info.call_num.number.num_len = MIN(arraycall_info->call_bcdinfo[pos].number_len, MN_MAX_ADDR_BCD_LEN);
#ifdef MMI_CSP_SUPPORT
        call_info.line = arraycall_info->call_bcdinfo[pos].line;
#endif   
        SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[pos].number, call_info.call_num.number.num_len);

        if (arraycall_info->call_bcdinfo[pos].is_sim_record
            && 0 < arraycall_info->sim_record_num)
        {
            arraycall_info->sim_record_num--;
        }

        MMICL_DeleteCallInfo(arraycall_info->call_bcdinfo[pos].call_type, call_info.dual_sys, &call_info);
    }
#endif

    //删除指定位置的电话记录
    SCI_MEMSET(&(arraycall_info->call_bcdinfo[index]),0,sizeof(MMICL_CALL_BCDINFO_T));
    
    if (arraycall_info->record_num > MMICL_RECORD_TOTAL_NUM)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo error record_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_620_112_2_18_2_11_16_3,(uint8*)"d", arraycall_info->record_num);
        return 0;
    }
    
    //后面的电话记录前移
    for (i=index; i<(arraycall_info->record_num - 1) && (i + 1 < MMICL_RECORD_TOTAL_NUM); i++)
    {
        arraycall_info->call_bcdinfo[i] = arraycall_info->call_bcdinfo[i+1];
    }
    
    //最后一条电话记录赋零
    SCI_MEMSET(&arraycall_info->call_bcdinfo[arraycall_info->record_num - 1],0,sizeof(MMICL_CALL_BCDINFO_T));
    
    if (0 < arraycall_info->record_num)
    {
        arraycall_info->record_num--;
    }
    
    MMICL_WriteNV(call_type, arraycall_info);
    
    record_num = arraycall_info->record_num;
    
    if (MMK_IsOpenWin(MMICL_LOG_LIST_WIN_ID))
    {
        if (MMICL_GetCurCallType() == call_type)
        {
            //delete appointed record from listbox
            GUILIST_RemoveItem(
                MMICL_GetLogListCtrlId(call_type),
                pos);
        }
    }

    MMICL_UpdateEventInd();
    
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
        arraycall_info =PNULL;
    }
    
    return record_num;

}
#else
uint8 MMICL_DeleteAppointedCallInfo(//剩下的记录的个数
                                    MMICL_CALL_TYPE_E    call_type,  //call 类型
                                    uint8                pos         //in
                                    )
{
    uint8                       i = 0,record_num = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
    
    //判断指定的位置是否出错
    if (pos >= MMICL_RECORD_TOTAL_NUM) //cr35264 jian.ma uint8 类型的数恒为非负数。
    {
        SCI_TRACE_LOW("MMICL_DeleteAppointedCallInfo: the appointed position is %d",pos);
        return record_num;
    }
    
    //init arraycall_info
    arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_686_112_2_18_2_11_16_4,(uint8*)"");
        return 0;
    }
    
    //SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    //get the value of arraycall_info from nv
    return_value = MMICL_ReadNV(call_type, arraycall_info);

    if((arraycall_info->record_num == 0) || (arraycall_info->record_num > MMICL_RECORD_TOTAL_NUM))
    {
        return 0;
    }
    //Convert Index to Array Index
    pos = arraycall_info->call_bcdinfo[pos].array_index;

    SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    //get the value of arraycall_info from nv
    return_value = MMICL_ReadNV(MMICL_CALL_ALL, arraycall_info);

    if((arraycall_info->record_num == 0) || (arraycall_info->record_num > MMICL_RECORD_TOTAL_NUM))
    {
        return 0;
    }
    
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
#ifdef MMI_CSP_SUPPORT
        call_info.line = arraycall_info->call_bcdinfo[pos].line;
#endif    
        SCI_MEMCPY(&call_info.call_num.number.party_num, arraycall_info->call_bcdinfo[pos].number, call_info.call_num.number.num_len);

        if (arraycall_info->call_bcdinfo[pos].is_sim_record && 0 < arraycall_info->sim_record_num)
        {
            arraycall_info->sim_record_num--;
        }

        MMICL_DeleteCallInfo(call_type, call_info.dual_sys, &call_info);
    }
#endif
    
    //删除指定位置的电话记录
    SCI_MEMSET(&(arraycall_info->call_bcdinfo[pos]),0,sizeof(MMICL_CALL_BCDINFO_T));
    
    if(arraycall_info->record_num > MMICL_RECORD_TOTAL_NUM)
    {
        //SCI_TRACE_LOW:"MMICL_DeleteAppointedCallInfo error record_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_738_112_2_18_2_11_17_5,(uint8*)"d", arraycall_info->record_num);
        return 0;
    }
    
    //后面的电话记录前移
    for (i=pos; i<(arraycall_info->record_num - 1) && (i + 1 < MMICL_RECORD_TOTAL_NUM); i++)
    {
        arraycall_info->call_bcdinfo[i] = arraycall_info->call_bcdinfo[i+1];
    }
    
    //最后一条电话记录赋零
    SCI_MEMSET(&arraycall_info->call_bcdinfo[arraycall_info->record_num - 1],0,sizeof(MMICL_CALL_BCDINFO_T));
    
    if (0 < arraycall_info->record_num)
    {
        arraycall_info->record_num--;
    }
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
    
    record_num = arraycall_info->record_num;
    
    MMICL_UpdateEventInd();   
    
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
    
    return record_num;
}
#endif
/*****************************************************************************/
// 	Description : add a new record of call in the head of list 
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void AddNewCallInfo(
                          MN_DUAL_SYS_E			dual_sys,  // Indicate the selected system
                          MMICL_CALL_TYPE_E             call_type,    //call 类型
                          MMICL_CALL_INFO_T             call_info     //call 信息
                          )
{
    MMICL_CALL_ARRAY_BCDINFO_T*   arraycall_info = PNULL;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint8   insert_index = 0; //@Lin.Lin, 2005-5-30
#ifndef MMI_PDA_SUPPORT    //pda 无focus概念
    uint16 cur_item_index = 0;
    CC_CONTROL_ID_E list_id = MMICL_GetLogListCtrlId(call_type);
#endif
    //init arraycall_info
    arraycall_info = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"AddNewCallInfo call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_793_112_2_18_2_11_17_6,(uint8*)"");
        return;
    }
    
    //SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    //get the value of arraycall_info from nv

#ifdef MMI_PDA_SUPPORT
    return_value = MMICL_ReadAllNV(arraycall_info);
#else
    return_value = MMICL_ReadNV(MMICL_CALL_ALL, arraycall_info);            //Just use all type here
#endif

    //add new detail record of call in the head of list 
    insert_index = AddNewDetailCallInfo(dual_sys, arraycall_info,call_info,call_type);

 #ifdef MMI_LDN_SUPPORT
    if (MMICL_CALL_DIALED == call_type
        //|| MMICL_CALL_RECEIVED == call_type   //ldn don't include received call.
        )
    {
        MMICL_UpdateCallInfo(call_type, dual_sys, &call_info);
    }
#endif
    
    MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
    
    //be care,this op must locate after MMICL_WriteNV
    if (MMK_IsOpenWin(MMICL_LOG_LIST_WIN_ID))
    {
        if (MMICL_GetCurCallType() == call_type)
        {
            MMICL_SetLogListItem();


#ifndef MMI_PDA_SUPPORT    //pda 无focus概念
            //下移该窗口光标，保证对应电话为之前的电话
            cur_item_index = GUILIST_GetCurItemIndex(list_id);
#ifdef CALL_TIMES_SUPPORT  
            if (MMICL_RECORD_TOTAL_NUM > cur_item_index)
            {
                if(MMICL_CALL_DIALED == call_type)
                {
                    GUILIST_SetCurItemIndex(list_id, 0);
                }
                else
                {
                    if(1 == arraycall_info->call_bcdinfo[0].call_times)
                    {
                        GUILIST_SetCurItemIndex(list_id, cur_item_index+1);
                    }
                    else
                    {
                        GUILIST_SetCurItemIndex(list_id, 0);
                    }
                }
            }
#else
            if (MMICL_RECORD_TOTAL_NUM > cur_item_index)
            {
                GUILIST_SetCurItemIndex(list_id, cur_item_index+1);
            }
#endif
            #endif
        }
    }

    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
    

    
}

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : add a new detail record of call in the head of list 
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL uint8 AddNewDetailCallInfo(
                                 MN_DUAL_SYS_E			dual_sys,  // Indicate the selected system
                                 MMICL_CALL_ARRAY_BCDINFO_T  *arraycall_info_ptr,
                                 MMICL_CALL_INFO_T           call_info,     //call 信息
                                 MMICL_CALL_TYPE_E   call_type
                                 )
{
    uint8   i = 0;
    uint8   insert_index = 0;    
    uint16 record_num = 0;
    uint8 j = 0, same_num_pos = 0;
#ifdef CALL_TIMES_SUPPORT
    uint16 call_times = 0;
    BOOLEAN is_same_num = FALSE;
    MMICL_CALL_BCDINFO_T    *call_bcdinfo = PNULL;
    BOOLEAN is_sim_record = FALSE;

#ifdef BT_DIALER_SUPPORT
    BOOLEAN is_bt_call_type = FALSE;
#endif

#ifdef BT_PBAP_SUPPORT
    if(call_type == MMICL_BT_CALL_ALL 
        || call_type == MMICL_BT_CALL_DIALED
        || call_type == MMICL_BT_CALL_MISSED
        || call_type == MMICL_BT_CALL_RECEIVED)
    {
        is_bt_call_type = TRUE;
    }
#endif

    for(j=0; j<arraycall_info_ptr->record_num; j++)
    {
        is_same_num = 
            (arraycall_info_ptr->call_bcdinfo[j].number_len == call_info.call_num.number.num_len) 
            && (0 == memcmp((char *)arraycall_info_ptr->call_bcdinfo[j].number, (char *)call_info.call_num.number.party_num, 
                    call_info.call_num.number.num_len)) 
#if defined(CL_SAVE_NAME_SUPPORT)
            && ((arraycall_info_ptr->call_bcdinfo[j].name.is_use_nv_name == FALSE && call_info.name.is_use_nv_name == FALSE)
            ||(((arraycall_info_ptr->call_bcdinfo[j].name.wstr_len == call_info.name.wstr_len)||(arraycall_info_ptr->call_bcdinfo[j].name.wstr_len == MMICL_NAME_MAX_LEN && MMICL_NAME_MAX_LEN < call_info.name.wstr_len))
                && (0 == memcmp((char *)arraycall_info_ptr->call_bcdinfo[j].name.wstr, (char *)call_info.name.wstr, 
                MIN(call_info.name.wstr_len, MMICL_NAME_MAX_LEN)*sizeof(wchar)))
                &&(arraycall_info_ptr->call_bcdinfo[j].name.is_use_nv_name == TRUE && call_info.name.is_use_nv_name == TRUE)))   
#endif
           ;

#ifdef BT_PBAP_SUPPORT
        if(TRUE == is_bt_call_type)
        {
            if((is_same_num
                ||((0 == arraycall_info_ptr->call_bcdinfo[j].number_len) && (0 == call_info.call_num.number.num_len))) // num not present
                && arraycall_info_ptr->call_bcdinfo[j].dual_sys == call_info.dual_sys
#ifdef MMI_CSP_SUPPORT
                && arraycall_info_ptr->call_bcdinfo[j].line == call_info.line
#endif            
                && arraycall_info_ptr->call_bcdinfo[j].isvideocall == CC_CALL_NORMAL
                &&(arraycall_info_ptr->call_bcdinfo[j].call_type == call_type)
                )
            {
                same_num_pos = j;
                call_times = arraycall_info_ptr->call_bcdinfo[j].call_times;
                break;
            }
        }
        else
#endif
        {
            if((is_same_num
                ||((0 == arraycall_info_ptr->call_bcdinfo[j].number_len) && (0 == call_info.call_num.number.num_len))) // num not present
                && arraycall_info_ptr->call_bcdinfo[j].dual_sys == call_info.dual_sys
#ifdef MMI_CSP_SUPPORT
                && arraycall_info_ptr->call_bcdinfo[j].line == call_info.line
#endif            
                && arraycall_info_ptr->call_bcdinfo[j].isvideocall == call_info.call_type
                &&(arraycall_info_ptr->call_bcdinfo[j].call_type == call_type)
            )
            {
                same_num_pos = j;
                call_times = arraycall_info_ptr->call_bcdinfo[j].call_times;
                break;
            }
        }
    }

    if(MMICL_MAX_CALL_TIMES >= call_times)
    {
        call_times++;
    }
    else
    {
        call_times = MMICL_MAX_CALL_TIMES;
    }
    
    if (arraycall_info_ptr->record_num >= arraycall_info_ptr->sim_record_num)
    {
        record_num = arraycall_info_ptr->record_num - arraycall_info_ptr->sim_record_num;//计算数目
    }
    //bug id: 1301782, 1307447, 1311747, 1314442, 1335543
    if(((0 != same_num_pos) || (call_times > 1)) /* && call_info.call_num.is_num_present */)
    {
        //如果是LDN号码，需要NV记录,LDN只记录一次
        if (arraycall_info_ptr->call_bcdinfo[same_num_pos].is_sim_record && 2 == call_times)
        {
            arraycall_info_ptr->call_bcdinfo[same_num_pos].is_sim_record = FALSE;

            if (0 < arraycall_info_ptr->sim_record_num)
            {
                arraycall_info_ptr->sim_record_num--;
            }

            SCI_MEMSET(&arraycall_info_ptr->call_bcdinfo[same_num_pos],0x00,sizeof(MMICL_CALL_BCDINFO_T));

            call_times = 1;

            is_sim_record = TRUE;
        }
        else
        {
            call_bcdinfo = (MMICL_CALL_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_BCDINFO_T));

            if(PNULL == call_bcdinfo)
            {
                //SCI_TRACE_LOW:"call_bcdinfo alloc fail!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_959_112_2_18_2_11_17_7,(uint8*)"");

                return 0;
            }

            /*记下最新的相同的那条记录*/
            SCI_MEMCPY(call_bcdinfo,&arraycall_info_ptr->call_bcdinfo[same_num_pos],sizeof(MMICL_CALL_BCDINFO_T));
        }
    
        if (MMICL_RECORD_MAX_NUM > record_num)//未存储满
        {
            for (i=same_num_pos; i>0; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
        else
        {
            //delete ldn
            if (arraycall_info_ptr->call_bcdinfo[same_num_pos].is_sim_record)//如果是LDN号码。删除最后一条NV记录
            {
                //删除LDN
                //MMICL_DeleteCallInfo(call_type, call_info.dual_sys, &call_info);

                //删除LDN记录
                for (i=same_num_pos; i>0; i--)
                {
                    arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
                }
            }

            for (i=(MMICL_RECORD_MAX_NUM-1); i>insert_index; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
    }
    else
    {        
        if (MMICL_RECORD_MAX_NUM > record_num)//未存储满
        {
            for (i=(MMICL_RECORD_TOTAL_NUM-1); i>insert_index; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
        else
        {
            //delete ldn    //存储满删除最后一条
            MMICL_DeleteCallInfo(call_type, call_info.dual_sys, &call_info);

            for (i=(MMICL_RECORD_TOTAL_NUM-1); i>insert_index; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
    }
#else    
    for(j=0; j<arraycall_info_ptr->record_num; j++)
    {
        if((arraycall_info_ptr->call_bcdinfo[j].number_len == call_info.call_num.number.num_len) && 
            (0 == memcmp((char *)arraycall_info_ptr->call_bcdinfo[j].number, (char *)call_info.call_num.number.party_num, 
            call_info.call_num.number.num_len)) && arraycall_info_ptr->call_bcdinfo[j].dual_sys == call_info.dual_sys
#ifdef MMI_CSP_SUPPORT
            && arraycall_info_ptr->call_bcdinfo[j].line == call_info.line
#endif 
            && arraycall_info_ptr->call_bcdinfo[j].isvideocall == call_info.call_type)
        {
            same_num_pos = j;
            break;
        }
    }

    //所有的电话记录后移，最后一条自动舍弃
    if (arraycall_info_ptr->record_num >= arraycall_info_ptr->sim_record_num)
    {
        record_num = arraycall_info_ptr->record_num - arraycall_info_ptr->sim_record_num;//计算号码
    }
    
    //如果是LDN号码，需要删除NV记录
    if(arraycall_info_ptr->call_bcdinfo[same_num_pos].is_sim_record)
    {
        if (0 < arraycall_info_ptr->sim_record_num)
        {
            arraycall_info_ptr->sim_record_num--;
        }

        if (0 < arraycall_info_ptr->record_num)
        {
            arraycall_info_ptr->record_num--;
        }

        if (MMICL_RECORD_MAX_NUM > record_num)//未存储满
        {
            for (i=same_num_pos; i>0; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
        else
        {
            //delete ldn
            if (arraycall_info_ptr->call_bcdinfo[same_num_pos].is_sim_record)//如果是LDN号码。删除最后一条NV记录
            {
                //删除LDN
                //MMICL_DeleteCallInfo(call_type, call_info.dual_sys, &call_info);

                //删除LDN记录
                for (i=same_num_pos; i>0; i--)
                {
                    arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
                }
            }

            for (i=(MMICL_RECORD_MAX_NUM-1); i>insert_index; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
    }
    else
    {
        if (MMICL_RECORD_MAX_NUM != record_num)//未存储满
        {
            for (i=(MMICL_RECORD_TOTAL_NUM-1); i>insert_index; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
        else
        {
            //delete ldn    //存储满删除最后一条
            MMICL_DeleteCallInfo(call_type, call_info.dual_sys, &call_info);

            for (i=(MMICL_RECORD_MAX_NUM-1); i>insert_index; i--)
            {
                arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
            }
        }
    }
#endif
    
    //第一条电话记录置零
    SCI_MEMSET(&arraycall_info_ptr->call_bcdinfo[insert_index],0,sizeof(MMICL_CALL_BCDINFO_T));
    
#ifdef CALL_TIMES_SUPPORT     
    if(((0 != same_num_pos) || (call_times > 1)) /*&& call_info.call_num.is_num_present*/ &&(PNULL !=call_bcdinfo))
    {
        /*把原来的时间保存下来*/
        SCI_MEMCPY(&arraycall_info_ptr->call_bcdinfo[insert_index],call_bcdinfo,sizeof(MMICL_CALL_BCDINFO_T));
    }
    if(PNULL != call_bcdinfo)
    {
        SCI_FREE(call_bcdinfo);
    }
    //记录电话记录的个数
    if ((MMICL_RECORD_TOTAL_NUM > arraycall_info_ptr->record_num)  &&  (1 == call_times) && !is_sim_record)
    {
        arraycall_info_ptr->record_num++;
    }
#else
    //记录电话记录的个数
    if (MMICL_RECORD_TOTAL_NUM > arraycall_info_ptr->record_num)
    {
        arraycall_info_ptr->record_num++;
    }
#endif
    //添加新的电话记录在第一条
    arraycall_info_ptr->call_bcdinfo[insert_index].is_num_present = call_info.call_num.is_num_present;

#if defined(CL_SAVE_NAME_SUPPORT)
    SCI_MEMCPY((void *)&arraycall_info_ptr->call_bcdinfo[insert_index].name,(void *)&call_info.name,sizeof(MMICL_NAME_T));
#endif

    //电话号码赋值
    if (call_info.call_num.is_num_present)
    {
//lzk mark
#ifdef BT_DIALER_SUPPORT
        if(call_info.call_num.number.num_len > MMIPB_BCD_NUMBER_MAX_LEN)
        {
            SCI_TRACE_LOW("AddNewDetailCallInfo: unknown num_len  = %d", call_info.call_num.number.num_len);
            SCI_TRACE_BUF("AddNewDetailCallInfo: unknown number  =",\
                arraycall_info_ptr->call_bcdinfo[insert_index].number,
                call_info.call_num.number.num_len
                );
            SCI_MEMSET(arraycall_info_ptr->call_bcdinfo[insert_index].number,0,sizeof(arraycall_info_ptr->call_bcdinfo[insert_index].number));
            arraycall_info_ptr->call_bcdinfo[insert_index].number_plan = MN_NUM_PLAN_ISDN_TELE;
            arraycall_info_ptr->call_bcdinfo[insert_index].number_type = MN_NUM_TYPE_UNKNOW;
            arraycall_info_ptr->call_bcdinfo[insert_index].number_len = 0;
        }
        else
#endif
        {
            //debug时需要着重注意number plan and type的赋值!!!
            arraycall_info_ptr->call_bcdinfo[insert_index].number_plan = MN_NUM_PLAN_ISDN_TELE;
            arraycall_info_ptr->call_bcdinfo[insert_index].number_type = call_info.call_num.number.number_type;
            arraycall_info_ptr->call_bcdinfo[insert_index].number_len = (uint8)call_info.call_num.number.num_len;
            MMI_MEMCPY((void*)arraycall_info_ptr->call_bcdinfo[insert_index].number,MMIPB_BCD_NUMBER_MAX_LEN,   /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
                (void*)call_info.call_num.number.party_num,MN_MAX_ADDR_BCD_LEN,                                 /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
                call_info.call_num.number.num_len);                                                             /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
        }
    }
    else
    {
        SCI_MEMSET(arraycall_info_ptr->call_bcdinfo[insert_index].number,0,sizeof(arraycall_info_ptr->call_bcdinfo[insert_index].number));
        arraycall_info_ptr->call_bcdinfo[insert_index].number_type = call_info.call_num.number.number_type;//MN_NUM_TYPE_UNKNOW;
        arraycall_info_ptr->call_bcdinfo[insert_index].number_len = 0;
    }
    
#ifdef CALL_TIMES_SUPPORT
    if(MMICL_MAX_CALL_TIMES < call_times)
    {
        int8  times=0;
        /*超过最大次数，所有时间向前移动一位，最前面自动舍弃*/
        for(times=0; times < (MMICL_MAX_CALL_TIMES-1); times++)
        {
            arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[times] = arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[times+1];            
            arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[times] = arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[times+1];
        }
        call_times = MMICL_MAX_CALL_TIMES;
    }
    arraycall_info_ptr->call_bcdinfo[insert_index].call_times = call_times;

#ifdef ADULT_WATCH_SUPPORT
    if(is_same_num && (MMICL_CALL_MISSED == call_info.call_type))
    {
        arraycall_info_ptr->call_bcdinfo[insert_index].is_missed_call_read = FALSE;
    }
#endif

    /*没有达到最大次数，通话时间直接存在最后面一条*/
#ifdef BT_PBAP_SUPPORT
    if(is_bt_call_type && (1 < call_times))
    {
        if(arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[0] > call_info.call_time.start_time)
        {
            // adjust the time sequence as the pbap put the time stamp from new to olad
            for(i = (call_times-1); i >0 ; i--)
            {
                arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[i] = arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[i-1];
                arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[i] = arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[i-1];
            }
            arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[0] = call_info.call_time.start_time;
            arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[0] = GetCallDurationTime(call_info.call_time.start_time, 
                call_info.call_time.stop_time);
        }
        else
        {
            arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[call_times-1] = call_info.call_time.start_time;

            arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[call_times-1] = GetCallDurationTime(call_info.call_time.start_time, 
                                                                                                call_info.call_time.stop_time);
        }
    }
    else
#endif
    {
        arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[call_times-1] = call_info.call_time.start_time;

        arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[call_times-1] = GetCallDurationTime(call_info.call_time.start_time, 
                                                                                            call_info.call_time.stop_time);
    }

#else
    arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time = call_info.call_time.start_time;
    
    arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time = GetCallDurationTime(call_info.call_time.start_time, 
                                                                                            call_info.call_time.stop_time);
#endif

    //@zhaohui add,记录当前是可视电话还是普通电话
#ifdef BT_PBAP_SUPPORT
    if(is_bt_call_type)
    {
        arraycall_info_ptr->call_bcdinfo[insert_index].isvideocall = CC_CALL_NORMAL;
    }
    else
#endif
    {
        arraycall_info_ptr->call_bcdinfo[insert_index].isvideocall = call_info.call_type;
    }
    //记录当前电话使用的是哪张SIM卡
    arraycall_info_ptr->call_bcdinfo[insert_index].dual_sys = call_info.dual_sys;
    //记录当前电话是否是紧急号码呼出
    arraycall_info_ptr->call_bcdinfo[insert_index].is_emergency_call = call_info.is_emergency_call;
#ifdef BT_DIALER_SUPPORT
    arraycall_info_ptr->call_bcdinfo[insert_index].is_bt_dail = call_info.is_bt_dail;
#endif
    arraycall_info_ptr->call_bcdinfo[insert_index].call_type = call_type;
#ifdef MMI_CSP_SUPPORT
    arraycall_info_ptr->call_bcdinfo[insert_index].line = call_info.line;
#endif
    return insert_index; //@Lin.Lin, 2005-5-30
}
#else
/*****************************************************************************/
// 	Description : add a new detail record of call in the head of list 
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL uint8 AddNewDetailCallInfo(
                                 MN_DUAL_SYS_E			dual_sys,  // Indicate the selected system
                                 MMICL_CALL_ARRAY_BCDINFO_T  *arraycall_info_ptr,
                                 MMICL_CALL_INFO_T           call_info,     //call 信息
                                 MMICL_CALL_TYPE_E   call_type
                                 )
{
    uint8   i = 0;
    uint8   insert_index = 0;    
    
#ifdef CALL_TIMES_SUPPORT    
    uint8 j = 0, same_num_pos = 0;
    uint16 call_times = 0;
    BOOLEAN is_same_num = FALSE;
    MMICL_CALL_BCDINFO_T    *call_bcdinfo = PNULL;
#ifdef BT_DIALER_SUPPORT
    BOOLEAN is_bt_call_type = FALSE;
#endif

    for(j=0; j<arraycall_info_ptr->record_num; j++)
    {
        is_same_num = 
            (arraycall_info_ptr->call_bcdinfo[j].number_len == call_info.call_num.number.num_len) 
            && (0 == memcmp((char *)arraycall_info_ptr->call_bcdinfo[j].number, (char *)call_info.call_num.number.party_num, 
                    call_info.call_num.number.num_len)) 
#if defined(CL_SAVE_NAME_SUPPORT)
            && ((arraycall_info_ptr->call_bcdinfo[j].name.is_use_nv_name == FALSE && call_info.name.is_use_nv_name == FALSE)
            ||(((arraycall_info_ptr->call_bcdinfo[j].name.wstr_len == call_info.name.wstr_len)||(arraycall_info_ptr->call_bcdinfo[j].name.wstr_len == MMICL_NAME_MAX_LEN && MMICL_NAME_MAX_LEN < call_info.name.wstr_len))
                && (0 == memcmp((char *)arraycall_info_ptr->call_bcdinfo[j].name.wstr, (char *)call_info.name.wstr, 
                MIN(call_info.name.wstr_len, MMICL_NAME_MAX_LEN)*sizeof(wchar)))
                &&(arraycall_info_ptr->call_bcdinfo[j].name.is_use_nv_name == TRUE && call_info.name.is_use_nv_name == TRUE)))   
#endif
           ;

        if((is_same_num
            ||((0 == arraycall_info_ptr->call_bcdinfo[j].number_len) && (0 == call_info.call_num.number.num_len))) // num not present
            && arraycall_info_ptr->call_bcdinfo[j].dual_sys == call_info.dual_sys
#ifdef MMI_CSP_SUPPORT
            && arraycall_info_ptr->call_bcdinfo[j].line == call_info.line
#endif
#ifdef MMI_PDA_SUPPORT
            && arraycall_info_ptr->call_bcdinfo[j].isvideocall== call_info.call_type
#endif
			&&(arraycall_info_ptr->call_bcdinfo[j].call_type == call_type)
            )
        {
            same_num_pos = j;
            call_times = arraycall_info_ptr->call_bcdinfo[j].call_times;
            break;
        }

    }

#ifdef BT_PBAP_SUPPORT
    if(call_type == MMICL_BT_CALL_ALL 
        || call_type == MMICL_BT_CALL_DIALED
        || call_type == MMICL_BT_CALL_MISSED
        || call_type == MMICL_BT_CALL_RECEIVED)
    {
        is_bt_call_type = TRUE;
    }
#endif

    if(MMICL_MAX_CALL_TIMES >= call_times)//CALL_TIMES_MAX 等于最大也要加以，后面需要处理
    {
        call_times++;
    }
    else
    {
        call_times = MMICL_MAX_CALL_TIMES;
    }
    
    if(((0 != same_num_pos) || (call_times > 1))/* && call_info.call_num.is_num_present*/)//空号码也要同号保存
    {
        call_bcdinfo = (MMICL_CALL_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_BCDINFO_T));
        if(PNULL == call_bcdinfo)
        {
            //SCI_TRACE_LOW:"call_bcdinfo alloc fail!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1249_112_2_18_2_11_18_8,(uint8*)"");
            return 0;
        }
        /*记下最新的相同的那条记录*/
        SCI_MEMCPY(call_bcdinfo,&arraycall_info_ptr->call_bcdinfo[same_num_pos],sizeof(MMICL_CALL_BCDINFO_T));
        
        /*将找到的那条记录前面的所有记录后移一位，将最新的号码移到前面*/
        for (i=same_num_pos; i>0; i--)
        {
            arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
        }
    }
    else
    {
        for (i=(MMICL_RECORD_TOTAL_NUM-1); i>insert_index; i--)
        {
            arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
        }
    }
#else    
    //所有的电话记录后移，最后一条自动舍弃
    for (i=(MMICL_RECORD_TOTAL_NUM-1); i>insert_index; i--)
    {
        arraycall_info_ptr->call_bcdinfo[i] = arraycall_info_ptr->call_bcdinfo[i-1];
    }
#endif
    
    //第一条电话记录置零
    SCI_MEMSET(&arraycall_info_ptr->call_bcdinfo[insert_index],0,sizeof(MMICL_CALL_BCDINFO_T));
    
#ifdef CALL_TIMES_SUPPORT     
    if(((0 != same_num_pos) || (call_times > 1)) /*&& call_info.call_num.is_num_present*/ &&(PNULL !=call_bcdinfo))
    {
        /*把原来的时间保存下来*/
        SCI_MEMCPY(&arraycall_info_ptr->call_bcdinfo[insert_index],call_bcdinfo,sizeof(MMICL_CALL_BCDINFO_T));
    }
    if(PNULL != call_bcdinfo)
    {
        SCI_FREE(call_bcdinfo);
    }
    //记录电话记录的个数
    if ((MMICL_RECORD_TOTAL_NUM > arraycall_info_ptr->record_num)  &&  (1 == call_times))
    {
        arraycall_info_ptr->record_num++;
    }
#else
    //记录电话记录的个数
    if (MMICL_RECORD_TOTAL_NUM > arraycall_info_ptr->record_num)
    {
        arraycall_info_ptr->record_num++;
    }
#endif
    //添加新的电话记录在第一条
    arraycall_info_ptr->call_bcdinfo[insert_index].is_num_present = call_info.call_num.is_num_present;

#if defined(CL_SAVE_NAME_SUPPORT)
    SCI_MEMCPY((void *)&arraycall_info_ptr->call_bcdinfo[insert_index].name,(void *)&call_info.name,sizeof(MMICL_NAME_T));
#endif

    //电话号码赋值
    if (TRUE == call_info.call_num.is_num_present)
    {
        //debug时需要着重注意number plan and type的赋值!!!
        arraycall_info_ptr->call_bcdinfo[insert_index].number_plan = MN_NUM_PLAN_ISDN_TELE;
        arraycall_info_ptr->call_bcdinfo[insert_index].number_type = call_info.call_num.number.number_type;
        arraycall_info_ptr->call_bcdinfo[insert_index].number_len = (uint8)call_info.call_num.number.num_len;
        MMI_MEMCPY((void*)arraycall_info_ptr->call_bcdinfo[insert_index].number,MMIPB_BCD_NUMBER_MAX_LEN,   /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
            (void*)call_info.call_num.number.party_num,MN_MAX_ADDR_BCD_LEN,                                 /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
            call_info.call_num.number.num_len);                                                             /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
    }
    else
    {
        SCI_MEMSET(arraycall_info_ptr->call_bcdinfo[insert_index].number,0,sizeof(arraycall_info_ptr->call_bcdinfo[insert_index].number));
        arraycall_info_ptr->call_bcdinfo[insert_index].number_type = call_info.call_num.number.number_type;//MN_NUM_TYPE_UNKNOW;
        arraycall_info_ptr->call_bcdinfo[insert_index].number_len = 0;
    }
    
#ifdef CALL_TIMES_SUPPORT
    if(MMICL_MAX_CALL_TIMES < call_times)
    {
        int8  times=0;
        /*超过最大次数，所有时间向前移动一位，最前面自动舍弃*/
        for(times=0; times < (MMICL_MAX_CALL_TIMES-1); times++)
        {
            arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[times] = arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[times+1];            
            arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[times] = arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[times+1];
        }
        call_times = MMICL_MAX_CALL_TIMES;
    }
    arraycall_info_ptr->call_bcdinfo[insert_index].call_times = call_times;
  
    /*没有达到最大次数，通话时间直接存在最后面一条*/
    arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time[call_times-1] = call_info.call_time.start_time;

    arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time[call_times-1] = GetCallDurationTime(call_info.call_time.start_time, 
                                                                                        call_info.call_time.stop_time);
#ifdef ADULT_WATCH_SUPPORT
    if(is_same_num && (MMICL_CALL_MISSED == call_info.call_type))
    {
        arraycall_info_ptr->call_bcdinfo[insert_index].is_missed_call_read = FALSE;
    }
#endif
#else
    arraycall_info_ptr->call_bcdinfo[insert_index].call_start_time = call_info.call_time.start_time;
    
    arraycall_info_ptr->call_bcdinfo[insert_index].call_duration_time = GetCallDurationTime(call_info.call_time.start_time, 
                                                                                            call_info.call_time.stop_time);
#endif

    //@zhaohui add,记录当前是可视电话还是普通电话
#ifdef BT_PBAP_SUPPORT
    if(is_bt_call_type)
    {
        arraycall_info_ptr->call_bcdinfo[insert_index].isvideocall = CC_CALL_NORMAL;
    }
    else
#endif
    {
        arraycall_info_ptr->call_bcdinfo[insert_index].isvideocall = call_info.call_type;
    }
    //记录当前电话使用的是哪张SIM卡
    arraycall_info_ptr->call_bcdinfo[insert_index].dual_sys = call_info.dual_sys;
    //记录当前电话是否是紧急号码呼出
    arraycall_info_ptr->call_bcdinfo[insert_index].is_emergency_call = call_info.is_emergency_call;
#ifdef BT_DIALER_SUPPORT
    arraycall_info_ptr->call_bcdinfo[insert_index].is_bt_dail = call_info.is_bt_dail;
#endif
    arraycall_info_ptr->call_bcdinfo[insert_index].call_type = call_type;
#ifdef MMI_CSP_SUPPORT
    arraycall_info_ptr->call_bcdinfo[insert_index].line = call_info.line;
#endif
    return insert_index; //@Lin.Lin, 2005-5-30
}
#endif

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : record the time of call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordCallTime(
                          uint8 line,
                          MMICL_CALL_TYPE_E     call_type,    //call ??
                          CC_CALL_TIME_T        call_time     //call ??
                          )
{
    //record the time of last call
    RecordLastCallTime(line, call_time);
    
    //record the time of all calls
    RecordAllCallsTime(line, call_time);
    
    //record the time of received or dialed call
    switch(call_type)
    {
    case MMICL_CALL_MISSED:
        //SCI_TRACE_LOW:"RecordCallTime:the time of missed call is zero!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1382_112_2_18_2_11_18_9,(uint8*)"");
        break;
        
    case MMICL_CALL_RECEIVED:
        RecordAllReceivedCallsTime(line, call_time);
        break;
        
    case MMICL_CALL_DIALED:
        RecordAllDialedCallsTime(line, call_time);
        break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        //SCI_TRACE_LOW:"RecordCallTime:the time of refused call is zero!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1395_112_2_18_2_11_18_10,(uint8*)"");
        break;
#endif
        
    default:
        SCI_TRACE_LOW("RecordCallTime:the call_type is error!");
        break;
    }
}

/*****************************************************************************/
// 	Description : record the time of the last call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordLastCallTime(
                              uint8 line, //call line
                              CC_CALL_TIME_T   call_time//call ??
                              )
{
    uint32               temp_time = 0;
    CC_CALL_TIME_COUNT_T time_count = {0};/*lint !e64*/
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time-call_time.start_time;
    
    if (temp_time != 0)//?????????
    {
        if(0 == line)
        {
            time_count.line1_call_time = temp_time;
        }
        else if(1 == line)
        {
            time_count.line2_call_time = temp_time;
        }
        
        time_count.dual_sys = call_time.dual_sys;
        MMIAPICL_StoreLastCallTime(line,&time_count);
    }
}

/*****************************************************************************/
// 	Description : record the time of all calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllCallsTime(
                              uint8 line, //call line
                              CC_CALL_TIME_T   call_time//call ??
                              )
{
    uint32              temp_time=0;
    CC_CALL_TIME_COUNT_T time_count = {0};/*lint !e64*/
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time - call_time.start_time;
    if (temp_time != 0)//?????????
    {
        MMIAPICL_GetAllCallsTime(line, &time_count, call_time.dual_sys);
        if(0 == line)
        {
            time_count.line1_call_time = time_count.line1_call_time + temp_time;
        }
        else if(1 == line)
        {
            time_count.line2_call_time = time_count.line2_call_time + temp_time;
        }
        
        time_count.dual_sys = call_time.dual_sys;

        MMIAPICL_StoreAllCallsTime(line, &time_count);
    }
}

/*****************************************************************************/
// 	Description : record the time of all received calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllReceivedCallsTime(
                                      uint8 line, //call line
                                      CC_CALL_TIME_T   call_time//call ??
                                      )
{
    uint32              temp_time=0;
    CC_CALL_TIME_COUNT_T receivedcalls_time = {0};/*lint !e64*/
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time - call_time.start_time;
    if (temp_time != 0)//?????????
    {
        MMIAPICL_GetReceivedCallsTime(line, &receivedcalls_time, call_time.dual_sys);
        if(0 == line)
        {
            receivedcalls_time.line1_call_time = receivedcalls_time.line1_call_time + temp_time;
        }
        else if(1 == line)
        {
            receivedcalls_time.line2_call_time = receivedcalls_time.line2_call_time + temp_time;
        }
        receivedcalls_time.dual_sys = call_time.dual_sys;

        MMIAPICL_StoreReceivedCallsTime(line, &receivedcalls_time);
    }
}

/*****************************************************************************/
// 	Description : record the time of all dialed calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllDialedCallsTime(
                                    uint8 line, //call line
                                    CC_CALL_TIME_T   call_time//call ??
                                    )
{
    CC_CALL_TIME_COUNT_T    dialedcalls_time = {0};/*lint !e64*/
    uint32                  temp_time = 0;
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time - call_time.start_time;
    if (temp_time != 0)//?????????
    {
        MMIAPICL_GetDialedCallsTime(line, &dialedcalls_time, call_time.dual_sys);
        if(0 == line)
        {
            dialedcalls_time.line1_call_time = dialedcalls_time.line1_call_time + temp_time;
        }
        else if(1 == line)
        {
            dialedcalls_time.line2_call_time = dialedcalls_time.line2_call_time + temp_time;
        }
        
        dialedcalls_time.dual_sys = call_time.dual_sys;

        MMIAPICL_StoreDialedCallsTime(line, &dialedcalls_time);        
    }
}

#else //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : record the time of call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordCallTime(
                          MMICL_CALL_TYPE_E     call_type,    //call 类型
                          CC_CALL_TIME_T        call_time     //call 时间
                          )
{
    //record the time of last call
    RecordLastCallTime(call_time);
    
    //record the time of all calls
    RecordAllCallsTime(call_time);
    
    //record the time of received or dialed call
    switch(call_type)
    {
    case MMICL_CALL_MISSED:
        //SCI_TRACE_LOW:"RecordCallTime:the time of missed call is zero!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1382_112_2_18_2_11_18_9,(uint8*)"");
        break;
        
    case MMICL_CALL_RECEIVED:
        RecordAllReceivedCallsTime(call_time);
        break;
        
    case MMICL_CALL_DIALED:
        RecordAllDialedCallsTime(call_time);
        break;
        
#if defined(MMI_BLACKLIST_SUPPORT)
    case MMICL_CALL_REFUSED:
        //SCI_TRACE_LOW:"RecordCallTime:the time of refused call is zero!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1395_112_2_18_2_11_18_10,(uint8*)"");
        break;
#endif
        
    default:
        SCI_TRACE_LOW("RecordCallTime:the call_type is error!");
        break;
    }
}

/*****************************************************************************/
// 	Description : record the time of the last call
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordLastCallTime(
                              CC_CALL_TIME_T   call_time//call 时间
                              )
{
    uint32               temp_time = 0;
    CC_CALL_TIME_COUNT_T time_count = {0};/*lint !e64*/
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time-call_time.start_time;
    
    if (temp_time != 0)//上次通话时间不为零
    {
        time_count.call_time = temp_time;
        time_count.dual_sys = call_time.dual_sys;
        MMIAPICL_StoreLastCallTime(&time_count);
    }
}

/*****************************************************************************/
// 	Description : record the time of all calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllCallsTime(
                              CC_CALL_TIME_T   call_time//call 时间
                              )
{
    uint32              temp_time=0;
    CC_CALL_TIME_COUNT_T time_count = {0};/*lint !e64*/
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time - call_time.start_time;
    if (temp_time != 0)//上次通话时间不为零
    {
        MMIAPICL_GetAllCallsTime(&time_count, call_time.dual_sys);
        
        time_count.call_time = time_count.call_time + temp_time;
        time_count.dual_sys = call_time.dual_sys;

        MMIAPICL_StoreAllCallsTime(&time_count);
    }
}

/*****************************************************************************/
// 	Description : record the time of all received calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllReceivedCallsTime(
                                      CC_CALL_TIME_T   call_time//call 时间
                                      )
{
    uint32              temp_time=0;
    CC_CALL_TIME_COUNT_T receivedcalls_time = {0};/*lint !e64*/
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time - call_time.start_time;
    if (temp_time != 0)//上次通话时间不为零
    {
        MMIAPICL_GetReceivedCallsTime(&receivedcalls_time, call_time.dual_sys);
        
        receivedcalls_time.call_time = receivedcalls_time.call_time + temp_time;
        receivedcalls_time.dual_sys = call_time.dual_sys;

        MMIAPICL_StoreReceivedCallsTime(&receivedcalls_time);
    }
}

/*****************************************************************************/
// 	Description : record the time of all dialed calls
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void RecordAllDialedCallsTime(
                                    CC_CALL_TIME_T   call_time//call 时间
                                    )
{
    CC_CALL_TIME_COUNT_T    dialedcalls_time = {0};/*lint !e64*/
    uint32                  temp_time = 0;
    
    temp_time = GetCallDurationTime(call_time.start_time, call_time.stop_time);//call_time.stop_time - call_time.start_time;
    if (temp_time != 0)//上次通话时间不为零
    {
        MMIAPICL_GetDialedCallsTime(&dialedcalls_time, call_time.dual_sys);
        
        dialedcalls_time.call_time = dialedcalls_time.call_time + temp_time;
        dialedcalls_time.dual_sys = call_time.dual_sys;

        MMIAPICL_StoreDialedCallsTime(&dialedcalls_time);        
    }
}
#endif //MMI_CSP_SUPPORT
LOCAL BOOLEAN CopyCallinfoToNvHeader(MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr,
            MMICL_CALL_ARRAY_BCDINFO_HEADER_T *header)
{
    if ((call_info_ptr ==PNULL) ||(header ==PNULL))
    {
        return FALSE;
    }
    header->dual_sys = call_info_ptr->dual_sys;
    header->record_num = call_info_ptr->record_num;
#ifdef MMI_LDN_SUPPORT
    header->sim_record_num = call_info_ptr->sim_record_num;
#endif    
    return TRUE;
}

LOCAL BOOLEAN CopyNvHeaderToCallInfo(MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr,
            MMICL_CALL_ARRAY_BCDINFO_HEADER_T *header)
{
    if ((call_info_ptr ==PNULL) ||(header ==PNULL))
    {
        return FALSE;
    }
    call_info_ptr->dual_sys  = header->dual_sys;
    call_info_ptr->record_num = header->record_num;
#ifdef MMI_LDN_SUPPORT
    call_info_ptr->sim_record_num = header->sim_record_num;
#endif    
    return TRUE;
}



/*****************************************************************************/
// 	Description : read call info nv
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MMICL_ReadAllNV(MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr)
{
    MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;    
    MMICL_CALL_ARRAY_BCDINFO_HEADER_T  nv_header={0};
    MMICL_CALL_ARRAY_BCDINFO_NVITEM_T  callinfo_nvitem={0};
    uint16  i = 0;
    uint16  nvitem_id = 0;
    //SCI_ASSERT(call_info_ptr !=  PNULL);/* assert to do*/
    if (PNULL == call_info_ptr)
    {
        //SCI_TRACE_LOW:"MMICL_ReadAllNV call_info_ptr ==  PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1541_112_2_18_2_11_18_11,(uint8*)"");
        return return_value;
    }

    //Read Header
    MMINV_READ(MMINV_CL_CALL_ALL_HEADER, &nv_header, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&nv_header,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_HEADER_T));
        MMINV_WRITE(MMINV_CL_CALL_ALL_HEADER,&nv_header);
    }

    CopyNvHeaderToCallInfo(call_info_ptr,&nv_header);   

    //Read body
    for (i=0;i<MMICL_RECORD_NVITEM_COUNT;i++)
    {
        nvitem_id = MMI_GetTrueNVItemId(MMINV_CL_CALL_ALL_CONTENT_BEGIN+i);
        SCI_MEMSET(&callinfo_nvitem,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_NVITEM_T));        
        return_value =(MN_RETURN_RESULT_E)EFS_NvitemRead(nvitem_id,
                        sizeof(MMICL_CALL_ARRAY_BCDINFO_NVITEM_T),(uint8 *)(&callinfo_nvitem));
        SCI_MEMCPY(&call_info_ptr->call_bcdinfo[i*MMICL_RECORD_NUM_PER_NVITEM],&callinfo_nvitem,
                    sizeof(MMICL_CALL_ARRAY_BCDINFO_NVITEM_T));
                    
        if (MN_RETURN_SUCCESS != return_value)
        {
            break;
        }
    }


    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(call_info_ptr,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));     //Clear call info
        //Write body
        SCI_MEMSET(&callinfo_nvitem,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_NVITEM_T));        
        for (i=0;i<MMICL_RECORD_NVITEM_COUNT;i++)
        {
            nvitem_id = MMI_GetTrueNVItemId(MMINV_CL_CALL_ALL_CONTENT_BEGIN+i);
            EFS_NvitemWrite(nvitem_id,sizeof(MMICL_CALL_ARRAY_BCDINFO_NVITEM_T),(uint8 *)&callinfo_nvitem,FALSE);
        }
    }
    
    return return_value;
}

/*****************************************************************************/
//  Description : read last missed call detail
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_ReadLastMissedCall(MMICL_CALL_BCDINFO_T *   call_bcdinfo_ptr)
{
    MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
    BOOLEAN result = FALSE;
    MMICL_CALL_ARRAY_BCDINFO_T  *arraycall_info = PNULL;
#ifdef MMI_PDA_SUPPORT
    uint32 i = 0;
    BOOLEAN is_find = FALSE;
#endif    
    
    if (PNULL == call_bcdinfo_ptr)
    {
        return result;
    }
    arraycall_info = SCI_ALLOCA(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"MMICL_ReadLastMissedCall() call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1609_112_2_18_2_11_18_12,(uint8*)"");
        return result;
    }

    SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

#ifdef MMI_PDA_SUPPORT
    return_value = MMICL_ReadAllNV(arraycall_info);
#else
    return_value = MMICL_ReadNV(MMICL_CALL_MISSED, arraycall_info);
#endif

    if(MN_RETURN_SUCCESS == return_value)
    {
#ifdef MMI_PDA_SUPPORT
        for (i = 0; i < arraycall_info->record_num; i++)
        {
            if (MMICL_CALL_MISSED == arraycall_info->call_bcdinfo[i].call_type)
            {
                is_find = TRUE;
                break;
            }
        }
        if (is_find)
        {
            *call_bcdinfo_ptr = arraycall_info->call_bcdinfo[i];
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMICL_ReadLastMissedCall() not find missed call! record_num = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1639_112_2_18_2_11_18_13,(uint8*)"d", arraycall_info->record_num);
        }
#else
        *call_bcdinfo_ptr = arraycall_info->call_bcdinfo[0];
        result = TRUE;
#endif
    }
    else
    {
        result = FALSE;
    }
    SCI_FREE(arraycall_info);

    return result;
}

/*****************************************************************************/
// 	Description : read call info nv
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MMICL_ReadNV(MMICL_CALL_TYPE_E call_type, MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr)
{
    MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_last_info = PNULL;
    uint16 i = 0,count = 0;
    
    if (PNULL == call_info_ptr)
    {
        return MN_RETURN_FAILURE;
    }
	
    if (call_type == MMICL_CALL_ALL)
    {
        return_value = MMICL_ReadAllNV(call_info_ptr);
    	if (MN_RETURN_SUCCESS != return_value)
    	{
            SCI_MEMSET(call_info_ptr,0x0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    		MMICL_WriteNV(MMICL_CALL_ALL, call_info_ptr);
    	}
    	return return_value;
    }

    arraycall_last_info = SCI_ALLOC_APPZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(call_info_ptr,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    //SCI_MEMSET(arraycall_last_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    
    return_value = MMICL_ReadAllNV(arraycall_last_info);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(arraycall_last_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
		MMICL_WriteNV(MMICL_CALL_ALL, arraycall_last_info);
    }

    //Copy 
    for(i = 0; i <arraycall_last_info->record_num; i++)
    {
        if(call_type == arraycall_last_info->call_bcdinfo[i].call_type)
        {
            memcpy(&call_info_ptr->call_bcdinfo[count],&arraycall_last_info->call_bcdinfo[i],sizeof(MMICL_CALL_BCDINFO_T) );
            call_info_ptr->call_bcdinfo[count].array_index = i; //Realy index in NV
            count ++;
        }
    }    
    call_info_ptr->record_num = count;

    SCI_Free(arraycall_last_info);

    return return_value;
}

/*****************************************************************************/
// 	Description : write call info nv
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
void MMICL_WriteNV(MMICL_CALL_TYPE_E call_type, MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr)
{  
    MMICL_CALL_ARRAY_BCDINFO_HEADER_T  nv_header={0};
    MMICL_CALL_ARRAY_BCDINFO_NVITEM_T  callinfo_nvitem={0};
    uint16  i = 0;
    uint16  nvitem_id = 0;
    if (PNULL == call_info_ptr)
    {
        return;
    }
    //Write header
    CopyCallinfoToNvHeader(call_info_ptr,&nv_header);

    MMINV_WRITE(MMINV_CL_CALL_ALL_HEADER,&nv_header);

    //Write Body
    for (i=0;i<MMICL_RECORD_NVITEM_COUNT;i++)
    {
        nvitem_id = MMI_GetTrueNVItemId(MMINV_CL_CALL_ALL_CONTENT_BEGIN+i);

        SCI_MEMCPY(&callinfo_nvitem,&call_info_ptr->call_bcdinfo[i*MMICL_RECORD_NUM_PER_NVITEM],sizeof(MMICL_CALL_ARRAY_BCDINFO_NVITEM_T));
        EFS_NvitemWrite(nvitem_id,sizeof(MMICL_CALL_ARRAY_BCDINFO_NVITEM_T),(uint8 *)(&callinfo_nvitem),FALSE);
    }    

    

    MMICL_UpdateCalllogWin(TRUE);
}

/*****************************************************************************/
// 	Description : calcute call duration time
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL uint32 GetCallDurationTime( uint32 start_time, uint32 stop_time)
{
    uint32 dura_time = 0;
    if(start_time <= stop_time)
    {
        dura_time = stop_time - start_time;
    }
    else
    {
        uint16 sys_min_year = 0;
        uint16 sys_max_year = 0;
        uint32 sys_min_time = 0;    
        uint32 sys_max_time = 0;
        MMITHEME_GetEditDateRange(&sys_min_year, &sys_max_year, 1); //第三个参数GUIEDIT_DATE_YEAR_RANGE_SET
        sys_min_time = MMIAPICOM_Tm2Second(CALENDER_SCECOND_MIN, 
                                                                                CALENDER_MIN_MIN, 
                                                                                CALENDER_HOUR_MIN, 
                                                                                CALENDER_DATE_MIN, 
                                                                                CALENDER_MONTH_MIN, 
                                                                                (uint32)sys_min_year
                                                                                );
        sys_max_time = MMIAPICOM_Tm2Second(CALENDER_SCECOND_MAX, 
                                                                                CALENDER_MIN_MAX, 
                                                                                CALENDER_HOUR_MAX, 
                                                                                CALENDER_DATE_MAX, 
                                                                                CALENDER_MONTH_MAX, 
                                                                                (uint32)sys_max_year
                                                                                );
        dura_time = stop_time - sys_min_time + sys_max_time - start_time;
    }
    return dura_time;
}

/*****************************************************************************/
// 	Description : export GetCallDurationTime
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICL_GetCallDurationTime( uint32 start_time, uint32 stop_time)
{
    return GetCallDurationTime(start_time, stop_time);
} 

#if defined(MMI_PDA_SUPPORT) 
/*****************************************************************************/
// 	Description : Check if it is a repeat record(通话记录中同一张sim卡并且是同一个号码，只保留一条记录，不考虑通话类型和姓名)
//	Global resource dependence : none
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsExistInMatchList(uint32 search_num, MMICL_CALL_BCDINFO_T calll_bcdinfo)
{
    uint32 index = 0;

    for (index = 0; index < search_num; index ++)
    {
        if ((s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len == calll_bcdinfo.number_len)
            && (0 == MMIAPICOM_CompareTwoUint8String(s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.party_num,s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len, calll_bcdinfo.number, s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len))
#if defined(CL_SAVE_NAME_SUPPORT)
                &&(s_mmicl_search_list_ptr->s_mmicl_search_list[index].name.is_use_nv_name == calll_bcdinfo.name.is_use_nv_name)
                &&(s_mmicl_search_list_ptr->s_mmicl_search_list[index].dual_sys == calll_bcdinfo.dual_sys)
                && (0 == MMIAPICOM_CompareTwoWstr(s_mmicl_search_list_ptr->s_mmicl_search_list[index].name.wstr, s_mmicl_search_list_ptr->s_mmicl_search_list[index].name.wstr_len, 
                                                                            calll_bcdinfo.name.wstr, calll_bcdinfo.name.wstr_len))
#endif
                &&(s_mmicl_search_list_ptr->s_mmicl_search_list[index].dual_sys == calll_bcdinfo.dual_sys)
            )
        {
            return TRUE;
        }
    }

    return FALSE;
}
#endif

/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 找到的号码的个数
/*****************************************************************************/
PUBLIC uint32 MMIAPICL_SearchList(MMI_STRING_T *search_str)
{
    uint32 search_num = 0;
    MMIPB_BCD_NUMBER_T search_bcd_num = {0};
    MMIPB_BCD_NUMBER_T cl_bcd_num = {0};
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info = PNULL;
    MN_RETURN_RESULT_E return_value = MN_RETURN_SUCCESS;    
    
    //SCI_TRACE_LOW:"MMIAPICL_SearchList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1832_112_2_18_2_11_19_14,(uint8*)"");
    
    if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL))
    {
        return 0;
    }
     
    if((PNULL == search_str) || (PNULL ==  search_str->wstr_ptr)
        || (search_str->wstr_len > MMIPB_BCD_NUMBER_MAX_LEN << 1)  //search string name too long
        )
    {
        return 0;
    }

    if(MMIAPICOM_ConvertWStr2Num(search_str->wstr_ptr, search_str->wstr_len, &search_bcd_num)
        || (0 == search_str->wstr_len)
    )
    {
        uint16 index = 0;
        arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == arraycall_info)
        {
            //SCI_TRACE_LOW:"arraycall_info alloc memory failed!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1854_112_2_18_2_11_19_15,(uint8*)"");
            return 0;
        }

        if (s_mmicl_search_list_ptr ==PNULL)
        {
            s_mmicl_search_list_ptr = (NUM_SEARCH_ITEM_CONTENT_T*)SCI_ALLOCAZ(sizeof(NUM_SEARCH_ITEM_CONTENT_T));
        }    
        if(PNULL == s_mmicl_search_list_ptr)
        {
            //SCI_TRACE_LOW:"s_mmicl_search_list_ptr alloc memory failed!!";
            SCI_FREE(arraycall_info);
            return 0;
        }        
        SCI_MEMSET(s_mmicl_search_list_ptr->s_mmicl_search_list, 0, sizeof(s_mmicl_search_list_ptr->s_mmicl_search_list));
        //SCI_MEMSET(arraycall_info,0x00,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        //get the value of arraycall_info from nv
#ifdef MMI_PDA_SUPPORT
        return_value = MMICL_ReadAllNV(arraycall_info);
#else
        return_value = MMICL_ReadNV(MMICL_CALL_DIALED, arraycall_info);
#endif
        for(index =0; index < arraycall_info->record_num; index++)
        {
            if(arraycall_info->call_bcdinfo[index].is_num_present)
            {
                cl_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(arraycall_info->call_bcdinfo[index].number_type, MN_NUM_PLAN_UNKNOW);        
                cl_bcd_num.number_len = arraycall_info->call_bcdinfo[index].number_len;
                cl_bcd_num.number_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN,cl_bcd_num.number_len);
                MMI_MEMCPY(
                	cl_bcd_num.number, 
                	MMIPB_BCD_NUMBER_MAX_LEN, 
                	arraycall_info->call_bcdinfo[index].number, 
                	cl_bcd_num.number_len, 
                	cl_bcd_num.number_len);            
                if(MMIAPICOM_bcdsstr(&cl_bcd_num, &search_bcd_num)
                    || (0 == search_str->wstr_len)
                )
                {
#if defined(MMI_PDA_SUPPORT) 
                    if (IsExistInMatchList(search_num, arraycall_info->call_bcdinfo[index]))
                    {
                        continue;
                    }
#endif
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].index = index;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].dual_sys = arraycall_info->call_bcdinfo[index].dual_sys;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.number_type = arraycall_info->call_bcdinfo[index].number_type;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.number_plan = arraycall_info->call_bcdinfo[index].number_plan;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.num_len = arraycall_info->call_bcdinfo[index].number_len;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].call_mode = arraycall_info->call_bcdinfo[index].isvideocall;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].is_emergency_call = arraycall_info->call_bcdinfo[index].is_emergency_call;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].call_type = arraycall_info->call_bcdinfo[index].call_type;
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.num_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN,s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.num_len);
                    
                    MMI_MEMCPY(
                    	s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.party_num, 
                    	MMIPB_BCD_NUMBER_MAX_LEN, 
                    	arraycall_info->call_bcdinfo[index].number, 
                    	s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.num_len, 
                    	s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].number.num_len);
#if defined(CL_SAVE_NAME_SUPPORT)
                        MMI_MEMCPY(
                                                &s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].name, 
                                                sizeof(MMICL_NAME_T), 
                                                &arraycall_info->call_bcdinfo[index].name, 
                                                sizeof(MMICL_NAME_T),
                                                sizeof(MMICL_NAME_T));  
#endif
#ifdef CALL_TIMES_SUPPORT
                    s_mmicl_search_list_ptr->s_mmicl_search_list[search_num].call_times = arraycall_info->call_bcdinfo[index].call_times;
#endif
                    search_num++;
                }
            }
        }
        SCI_FREE(arraycall_info);
    }
    //SCI_TRACE_LOW:"MMIAPICL_SearchList result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1917_112_2_18_2_11_19_16,(uint8*)"d", search_num);

    return search_num;
}

/*****************************************************************************/
//  Description : 外部搜索退出，当外部搜索退出是，这个接口一定要调用
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIAPICL_SearchExit(void)
{
    //SCI_TRACE_LOW:"MMIAPICL_SearchExit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1929_112_2_18_2_11_19_17,(uint8*)"");
    SCI_FREE(s_mmicl_search_list_ptr);

}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的姓名
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchEntryName(MMI_STRING_T *name_str, //OUT
										   //uint16 max_name_len, //IN
										   uint32 index,//IN
										   BOOLEAN is_display_calltimes
										   )
{
    BOOLEAN b_search_result = FALSE;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    int origin_num_len = 0;
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8 tele_number_no_ip[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint32 tele_len = 0;
    MMI_STRING_T temp_name_str = {0};
    wchar name_wchar[MMIPB_MAX_STRING_LEN] = {0};

    temp_name_str.wstr_len = 0;
    temp_name_str.wstr_ptr = name_wchar;
    //SCI_TRACE_LOW:"MMIAPICL_GetSearchEntryName index:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1951_112_2_18_2_11_19_18,(uint8*)"d", index);

    if (s_mmicl_search_list_ptr == PNULL)
    {
        return FALSE;
    }

    if(PNULL == name_str || PNULL == name_str->wstr_ptr || 0 == s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len)
    {
        //SCI_TRACE_LOW:"MMIAPICL_GetSearchEntryName index:parameter error !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_1955_112_2_18_2_11_19_19,(uint8*)"");
        return FALSE;
    }

#if defined(CL_SAVE_NAME_SUPPORT)
    if (s_mmicl_search_list_ptr->s_mmicl_search_list[index].name.is_use_nv_name)
    {
        temp_name_str.wstr_len  = s_mmicl_search_list_ptr->s_mmicl_search_list[index].name.wstr_len;
        if (temp_name_str.wstr_len != 0)
        {
            MMI_WSTRNCPY(
                                        temp_name_str.wstr_ptr,
                                        MMICL_NAME_MAX_LEN,
                                        s_mmicl_search_list_ptr->s_mmicl_search_list[index].name.wstr,
                                        MMICL_NAME_MAX_LEN,
                                        temp_name_str.wstr_len
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
        tele_len = MMICL_GenDispNumber(s_mmicl_search_list_ptr->s_mmicl_search_list[index].call_type,
            s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.number_type, 
            (uint8)s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len, 
            s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.party_num, 
            tele_num, 
            CC_MAX_TELE_NUM_LEN + 2);

        pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.number_type, MN_NUM_PLAN_UNKNOW);
        pb_num.number_len = MIN(MN_MAX_ADDR_BCD_LEN, s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len);					
        MMI_MEMCPY(
                                pb_num.number,
                                sizeof(pb_num.number),
                                (void*)s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.party_num,
                                sizeof(s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.party_num),
                                pb_num.number_len
                                );

        b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, &temp_name_str, CC_MAX_PBNAME_LEN, FALSE);/*lint !e506 */

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

                b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, &temp_name_str, CC_MAX_PBNAME_LEN, FALSE);/*lint !e506 */
            }
            else
            {
                return FALSE;
            }
        }

        if (b_search_result && 0 < temp_name_str.wstr_len)
        {
            b_search_result = TRUE;
        }
        else
        {
            b_search_result = FALSE;
        }
    }

    if (is_display_calltimes)
    {
        //add call times
#ifdef CALL_TIMES_SUPPORT
        if (1 < s_mmicl_search_list_ptr->s_mmicl_search_list[index].call_times)
        {
            MMICL_AddCallTimesStr(&temp_name_str,s_mmicl_search_list_ptr->s_mmicl_search_list[index].call_times,name_str,GUILIST_STRING_MAX_NUM);
        }
        else
#endif
        {
            name_str->wstr_len = temp_name_str.wstr_len;
            SCI_MEMCPY(name_str->wstr_ptr, temp_name_str.wstr_ptr, MMIPB_MAX_STRING_LEN);
        }
    }
    else
    {
        name_str->wstr_len = temp_name_str.wstr_len;
        SCI_MEMCPY(name_str->wstr_ptr, temp_name_str.wstr_ptr, MMIPB_MAX_STRING_LEN);
    }

    return b_search_result;
}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的号码
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchEntryCalledNumber(MN_CALLED_NUMBER_T *called_num,//OUT
									  uint32 index//IN
									  )
{
    if (s_mmicl_search_list_ptr == PNULL)
    {
        return FALSE;
    }

    if(PNULL == called_num || 0 == s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len)
    {
    	//SCI_TRACE_LOW:"MMIAPICL_GetSearchEntryNumber parameter error !"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_2040_112_2_18_2_11_19_20,(uint8*)"");
    	return FALSE;
    }

    MMI_MEMCPY((void*)called_num,sizeof (MN_CALLED_NUMBER_T),   /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
        (void*)&s_mmicl_search_list_ptr->s_mmicl_search_list[index].number,sizeof (MN_CALLED_NUMBER_T),                                 /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
        sizeof (MN_CALLED_NUMBER_T));
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的号码
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchEntryNumber(MMI_STRING_T *number_str, //OUT
									  //uint16 max_number_len,//IN  
									  uint32 index//IN
									  )
{
    uint8 tel_num[PHONE_NUM_MAX_LEN + 2] = {0};    
    uint8 tel_num_len = 0;

    if (s_mmicl_search_list_ptr == PNULL)
    {
        return FALSE;
    }


    if(PNULL == number_str || PNULL == number_str->wstr_ptr || 0 == s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len)
    {
    	//SCI_TRACE_LOW:"MMIAPICL_GetSearchEntryNumber parameter error !"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_2066_112_2_18_2_11_20_21,(uint8*)"");
    	return FALSE;
    }

    tel_num_len = MMIAPICOM_GenNetDispNumber(
        s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.number_type,//s_call_context.call_info[s_call_context.current_call].address.number_type, 
        (uint8)s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.num_len,//(uint8)s_call_context.call_info[s_call_context.current_call].address.num_len, 
        s_mmicl_search_list_ptr->s_mmicl_search_list[index].number.party_num,//s_call_context.call_info[s_call_context.current_call].address.party_num,
        tel_num,
        (uint8)sizeof(tel_num)
        );

    //get info for display call number
    if (tel_num_len >0)
    {
        number_str->wstr_len = tel_num_len;
        //prompt_str_array[2].wstr_ptr = tel_num;
        MMI_STRNTOWSTR(number_str->wstr_ptr,
            number_str->wstr_len,
            (uint8 *)tel_num,
            tel_num_len,
            tel_num_len);
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的卡
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICL_GetSearchEntrySim(uint32 index)
{
    if (s_mmicl_search_list_ptr == PNULL)
    {
        return MN_DUAL_SYS_1;
    }

    return s_mmicl_search_list_ptr->s_mmicl_search_list[index].dual_sys;
}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的通话类型
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMIAPICL_GetSearchEntryCallType(uint32 index)
{
    if (s_mmicl_search_list_ptr == PNULL)
    {
        return MMICL_CALL_MISSED;
    }

    return s_mmicl_search_list_ptr->s_mmicl_search_list[index].call_type;
}

/*****************************************************************************/
//  Description : 返回搜素到的通话类型
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchCallType(CC_CALL_MODE_E *call_mode_ptr, BOOLEAN *is_emergency_call_ptr, uint32 index)
{
    if (s_mmicl_search_list_ptr == PNULL)
    {
        return FALSE;
    }

    if (PNULL != call_mode_ptr)
    {
        *call_mode_ptr = s_mmicl_search_list_ptr->s_mmicl_search_list[index].call_mode;
    }

    if (PNULL != is_emergency_call_ptr)
    {
        *is_emergency_call_ptr = s_mmicl_search_list_ptr->s_mmicl_search_list[index].is_emergency_call;
    }

    return TRUE;
}
#ifdef MMI_PDA_SUPPORT 
/*****************************************************************************/
// 	Description : to get number of all records
//	Global resource dependence : 
//  Author:rong.gu
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICL_GetAllRecordsNum(void)
{
    uint8 record_num = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetAllRecordsNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_2149_112_2_18_2_11_20_22,(uint8*)"");
        return 0;
    }
    
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);
    record_num = arraycall_info->record_num;
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
        arraycall_info = PNULL;
    }
    
    return record_num;
}
#else
/*****************************************************************************/
// 	Description : to get number of all records
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICL_GetAllRecordsNum(void)
{
    uint8 record_num = 0;
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    
    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info)
    {
        //SCI_TRACE_LOW:"GetAllRecordsNum call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_2177_112_2_18_2_11_20_23,(uint8*)"");
        return 0;
    }
    
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_MISSED, arraycall_info);
    record_num += arraycall_info->record_num;
    
    
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_RECEIVED, arraycall_info);
    record_num += arraycall_info->record_num;
    
    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_CALL_DIALED, arraycall_info);
    record_num += arraycall_info->record_num;
    
    //read logs of calls from NV
#if defined(MMI_BLACKLIST_SUPPORT)    
    MMICL_GetRecordInfo(MMICL_CALL_REFUSED, arraycall_info);
    record_num += arraycall_info->record_num;
#endif
#ifdef BT_DIALER_SUPPORT
    //read logs of calls from NV    
    MMICL_GetRecordInfo(MMICL_BT_CALL_MISSED, arraycall_info);
    record_num += arraycall_info->record_num;

    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_BT_CALL_RECEIVED, arraycall_info);
    record_num += arraycall_info->record_num;

    //read logs of calls from NV
    MMICL_GetRecordInfo(MMICL_BT_CALL_DIALED, arraycall_info);
    record_num += arraycall_info->record_num;
#endif
    //if (PNULL != arraycall_info)
    {
        SCI_FREE(arraycall_info);
    }
    
    return (record_num);
}
#endif

/*****************************************************************************/
// 	Description : get record info
//	Global resource dependence : 
//  Author:bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_GetRecordInfo(MMICL_CALL_TYPE_E e_call_type, MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr)
{
    if (call_info_ptr == PNULL)
    {
        return FALSE;
    }

    SCI_MEMSET(call_info_ptr, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    //read logs of calls from NV
#ifdef MMI_PDA_SUPPORT
    MMICL_ReadAllNV(call_info_ptr);
#else
    MMICL_ReadNV(e_call_type, call_info_ptr);
#endif
    
    return TRUE;
}


#if defined(IKEYBACKUP_SUPPORT)
/*****************************************************************************/
//  Description : 1key backup call back function for backup
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL void calllog_backup_callback(
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    //void *buf_ptr = PNULL;
    
    if (PNULL == param_in_ptr
        || PNULL == param_out_ptr)
    {
        //log pls.
        return;
    }
    
    if (MMIIKB_TYPE_CALLLOG != param_in_ptr->type
        || MMIIKB_OPERATE_TYPE_BUF != param_in_ptr->op_type)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
    }
    else
    {
        MMICL_CALL_ARRAY_BCDINFO_BACKUP_T   *back_info_ptr=PNULL;
        uint16 size = 0;
        uint16 num = 0;
        size = sizeof(MMICL_CALL_ARRAY_BCDINFO_BACKUP_T);
        back_info_ptr = (MMICL_CALL_ARRAY_BCDINFO_BACKUP_T*)SCI_ALLOC_APP(size);
        if(PNULL == back_info_ptr)
        {
            error_code = MMIIKB_STATUS_NO_MEMORY;
        }
        else
        {
            MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
            return_value = MMICL_ReadAllNV(&back_info_ptr->call_array);
            num = back_info_ptr->call_array.record_num;
            back_info_ptr->nv_verno = MMICL_NV_VER_NO;
            back_info_ptr->support_flag = MMICL_FUNC_SUPPORT_FLAG;

            if(MN_RETURN_SUCCESS != return_value)
            {
                error_code = MMIIKB_STATUS_FAILED;
            }
            else
            {
                param_out_ptr->type = param_in_ptr->type;
                param_out_ptr->op_type = param_in_ptr->op_type;
                param_out_ptr->buf_ptr = back_info_ptr;
                param_out_ptr->begin = param_in_ptr->begin;
                param_out_ptr->len = size;
                param_out_ptr->number = num;
                param_out_ptr->is_need_free = TRUE;
                param_out_ptr->is_finished = TRUE;
                error_code = MMIIKB_STATUS_SUCCESS;                
            }
        }
    }
    
    param_out_ptr->error_code = error_code;

    return;   
}

/*****************************************************************************/
//  Description : 1key backup call back function for restore
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL void calllog_restore_callback(
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T *param_out_ptr
)
{
    /*本人测试时，就是把需要还原的内容写文件出来给用户看*/
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;

    if (PNULL == param_in_ptr
        || PNULL == param_out_ptr
        || PNULL == param_in_ptr->buf_ptr
        || 0 == param_in_ptr->number)
    {
        //log pls.
        //SCI_TRACE_LOW:"mmicl calllog_restore_callback: error param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_2320_112_2_18_2_11_20_24,(uint8*)"");
        return;
    }
    
    if (MMIIKB_TYPE_CALLLOG != param_in_ptr->type
        || MMIIKB_OPERATE_TYPE_BUF != param_in_ptr->op_type)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
    }
    else
    {
        MMICL_CALL_ARRAY_BCDINFO_BACKUP_T* backup_call_info_ptr = PNULL;
        param_out_ptr->type = param_in_ptr->type;
        param_out_ptr->op_type = param_in_ptr->op_type;

		backup_call_info_ptr = param_in_ptr->buf_ptr;
        //Check MMIIKB_STATUS_INVALID_FORMAT
        if (backup_call_info_ptr->nv_verno != MMICL_NV_VER_NO)
        {
            param_out_ptr->error_code = MMIIKB_STATUS_INVALID_PARAM;
            return;
        }

        if( backup_call_info_ptr->support_flag != MMICL_FUNC_SUPPORT_FLAG)
        {            
            param_out_ptr->error_code = MMIIKB_STATUS_INVALID_PARAM;
            return;

        }
        
        
        if (RestoreCallLog(&backup_call_info_ptr->call_array))
        {
            param_out_ptr->suc_cnt = backup_call_info_ptr->call_array.record_num;
            param_out_ptr->err_cnt = 0;
            error_code = MMIIKB_STATUS_SUCCESS;
        }
        else
        {
            error_code = MMIIKB_STATUS_FAILED;
        }
    }
    
    param_out_ptr->error_code = error_code;

    return;
}

/*****************************************************************************/
//  Description : 1key backup call back function for get total num
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL void cc_get_total_callback(
    MMIIKB_GET_TOTAL_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_SUCCESS;
    uint16 total_num = MMICL_GetAllRecordsNum();
    
    param_out_ptr->total = total_num;
    param_out_ptr->total_type = MMIIKB_TOTAL_TYPE_NUMBER;
    
    param_out_ptr->error_code = error_code;

    return;
}

/*****************************************************************************/
//  Description : 1key backup call back function for register
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_BackupRegister(void)
{
    BOOLEAN is_ret = FALSE;
    MMIIKB_TYPE_PROCESS_PARAM_T ikb_param = {0};
    
    ikb_param.type = MMIIKB_TYPE_CALLLOG;
    ikb_param.op_type = MMIIKB_OPERATE_TYPE_BUF;
    ikb_param.backup_call_back = calllog_backup_callback;
    ikb_param.is_backup_asyn = FALSE;
    ikb_param.restore_call_back = calllog_restore_callback;
    ikb_param.is_restore_asyn = FALSE;
    ikb_param.get_total_call_back = cc_get_total_callback;
    is_ret = MMIAPIIKB_RegTypeFunc(&ikb_param);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : restore backup info 
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL BOOLEAN RestoreCallLog(MMICL_CALL_ARRAY_BCDINFO_T * backup_call_info_ptr)
{
    BOOLEAN result = TRUE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info_ptr = PNULL;
    MMICL_CALL_BCDINFO_T call_info = {0};
    uint16 i = 0;
    uint16 j = 0;
    BOOLEAN is_same_num = FALSE;
    BOOLEAN is_new_num = FALSE;
    arraycall_info_ptr = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    if(PNULL == arraycall_info_ptr)
    {
        return FALSE;
    }
    
    return_value = MMICL_ReadAllNV(arraycall_info_ptr);

    if(MN_RETURN_SUCCESS != return_value || 0 == arraycall_info_ptr->record_num)
    {
        MMICL_WriteNV(MMICL_CALL_ALL, backup_call_info_ptr);
    }
    else
    {
        for(i=0; i<backup_call_info_ptr->record_num; i++)
        {
            call_info = backup_call_info_ptr->call_bcdinfo[i];
            is_new_num = FALSE;
            for(j=0; j<arraycall_info_ptr->record_num; j++)
            {
                is_same_num = 
                    (arraycall_info_ptr->call_bcdinfo[j].number_len == call_info.number_len) 
                    && (0 == memcmp((char *)arraycall_info_ptr->call_bcdinfo[j].number, (char *)call_info.number, 
                            call_info.number_len)) 
#if defined(CL_SAVE_NAME_SUPPORT)
                    && ((arraycall_info_ptr->call_bcdinfo[j].name.is_use_nv_name == FALSE && call_info.name.is_use_nv_name == FALSE)
                    ||(((arraycall_info_ptr->call_bcdinfo[j].name.wstr_len == call_info.name.wstr_len)||(arraycall_info_ptr->call_bcdinfo[j].name.wstr_len == MMICL_NAME_MAX_LEN && MMICL_NAME_MAX_LEN < call_info.name.wstr_len))
                        && (0 == memcmp((char *)arraycall_info_ptr->call_bcdinfo[j].name.wstr, (char *)call_info.name.wstr, 
                        MIN(call_info.name.wstr_len, MMICL_NAME_MAX_LEN)*sizeof(wchar)))
                        &&(arraycall_info_ptr->call_bcdinfo[j].name.is_use_nv_name == TRUE && call_info.name.is_use_nv_name == TRUE)))   
#endif
                   &&(arraycall_info_ptr->call_bcdinfo[j].call_type == call_info.call_type)
                 ;
                //same num need to compare time
                if((is_same_num
                    ||((0 == arraycall_info_ptr->call_bcdinfo[j].number_len) && (0 == call_info.number_len))) // num not present
                    && arraycall_info_ptr->call_bcdinfo[j].dual_sys == call_info.dual_sys
                    && arraycall_info_ptr->call_bcdinfo[j].isvideocall == call_info.isvideocall    
#ifdef MMI_CSP_SUPPORT
                    && arraycall_info_ptr->call_bcdinfo[j].line == call_info.line
#endif                       
                    )
                {
                    BOOLEAN is_same_time = FALSE;
                    
#ifdef CALL_TIMES_SUPPORT
                    uint16 k = 0;
                    uint16 l = 0;
                    for(k=0; k<call_info.call_times; k++)
                    {
                        is_same_time = FALSE;
                        
                        //no space to save call time
                        if(arraycall_info_ptr->call_bcdinfo[j].call_times >= MMICL_MAX_CALL_TIMES)
                        {
                            break;
                        }

                        //compare time
                        for(l=0; l<arraycall_info_ptr->call_bcdinfo[j].call_times; l++)
                        {
                            if(arraycall_info_ptr->call_bcdinfo[j].call_start_time[l] == call_info.call_start_time[k])
                            {
                                is_same_time = TRUE;
                                break;
                            }
                        }

                        //different time to save
                        if(!is_same_time)
                        {
                            arraycall_info_ptr->call_bcdinfo[j].call_start_time[arraycall_info_ptr->call_bcdinfo[j].call_times]
                                = call_info.call_start_time[k];
                            arraycall_info_ptr->call_bcdinfo[j].call_duration_time[arraycall_info_ptr->call_bcdinfo[j].call_times]
                                = call_info.call_duration_time[k];
                            
                            arraycall_info_ptr->call_bcdinfo[j].call_times++;
                        }
                    }
#else
                    is_same_time = arraycall_info_ptr->call_bcdinfo[j].call_start_time == call_info.call_start_time;
                    
                    if(!is_same_time)
                    {
                        MMICL_CALL_INFO_T             my_call_info={0};
                        
                        my_call_info.call_num.is_num_present =call_info.is_num_present;
                        my_call_info.call_num.number.number_type = call_info.number_type;
                        my_call_info.call_num.number.number_plan =call_info.number_plan;
                        my_call_info.call_num.number.num_len = call_info.number_len;
                        SCI_MEMCPY(my_call_info.call_num.number.party_num,call_info.number,sizeof(call_info.number));
                        my_call_info.call_time.start_time= call_info.call_start_time;
                        my_call_info.call_time.stop_time= call_info.call_start_time+call_info.call_duration_time;
                        my_call_info.call_time.dual_sys= call_info.dual_sys;
                        my_call_info.call_type =call_info.call_type;
                        my_call_info.dual_sys = call_info.dual_sys;
                        my_call_info.is_emergency_call = call_info.is_emergency_call;
#if defined(CL_SAVE_NAME_SUPPORT)
                        my_call_info.name = call_info.name;
#endif            
#ifdef MMI_CSP_SUPPORT
                        my_call_info.line = call_info.line;
#endif                        
                        AddNewCallInfo(call_info.dual_sys,call_info.call_type,my_call_info);
                    }
#endif
                    //this call log is new name
                    is_new_num = TRUE;
                }
            }
            if(!is_new_num) //non same name save to call_info
            {
                if(arraycall_info_ptr->record_num < MMICL_RECORD_MAX_NUM)
                {
                    arraycall_info_ptr->call_bcdinfo[arraycall_info_ptr->record_num] = call_info;
                    arraycall_info_ptr->record_num++;
                }
            }
        }
        MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info_ptr);
    }
    SCI_FREE(arraycall_info_ptr);
    
    return result;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获得列表控件中被选中的item数目
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/

PUBLIC uint16 MMIAPICL_GetSelectedItemCount(void)   //RETURN: 被选中的ITEM的总数
{
    return MMICL_GetSelectedItemCount();
}

/*****************************************************************************/
//  Description :if the selected item is valid
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SelectedListIsValid(uint16 index) 
{
    return MMICL_SelectedListIsValid(index);
}

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item 
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC void MMIAPICL_GetSelectPhonenum(
                            MMIPB_BCD_NUMBER_T*   bcd_number ,//out
                            uint16 sel_item_index
                            )
{
    MMICL_GetSelectPhonenum(
                            bcd_number ,//out
                            sel_item_index
                            );
}

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC void MMIAPICL_GetSelectPhoneName(
                            MMI_STRING_T*   call_name, //out
                            uint16 sel_item_index,
                            uint16 max_name_len,
                            MMIPB_BCD_NUMBER_T    *bcd_number
                            )
{
    MMICL_GetSelectPhoneName(
                                call_name, //out
                                sel_item_index,
                                max_name_len,
                                bcd_number
                             );
}
#endif

/*****************************************************************************/
// 	Description : Clear call log
//	Global resource dependence : 
//  Author:Jiaoyou.wu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_ClearCalllogData(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info = PNULL;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if(PNULL != arraycall_info)
    {
        MMICL_WriteNV(MMICL_CALL_ALL, arraycall_info);
        SCI_FREE(arraycall_info);
    }
}

/*****************************************************************************/
//Description : get record info of CL
//Global resource dependence : 
//Author:
//Note: 
/*****************************************************************************/                
PUBLIC uint16 MMIAPICL_GetRecordInfo(void)
{
    MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;

    arraycall_info = (MMICL_CALL_ARRAY_BCDINFO_T*)SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if (PNULL != arraycall_info)
    {
        uint16 record_num = 0;
    
        MMICL_GetRecordInfo(MMICL_CALL_ALL, arraycall_info);

        record_num = arraycall_info->record_num;

        SCI_FREE(arraycall_info);

        return record_num;
    }

    return 0;
}

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :Check if the PBAP data is synced
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_IsPbapDataSynced(void)
{
    if(0 == s_sync_calllog_count)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*******************************************************************************/
//  Description : reset pbap sync data count
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMICL_ResetPbapSyncCount(void)
{
    s_sync_calllog_count = 0;
}

/*******************************************************************************/
//  Description :解析vcard buffer data  
//  Parameter: data_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			  data_len[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMICL_NotifyPBStartParser(MMICL_PBAP_PARSER_CALLBACK_FUNC callback)
{
    parser_callback = callback;
    
    if(IsListEmpty(s_list_head))
    {
        MMICL_PBAP_PARSER_RESULT_T result = {0};
        result.ret = MMICL_PBAP_PARSER_FINISHED;
        result.sync_count = s_sync_calllog_count;
        
        parser_callback(&result);
        
        FreeAllPbapData();
    }
    else
    {
        SCI_TRACE_LOW("[MMICL] MMICL_NotifyPBStartParser count = %d",ListNodeNum(s_list_head));
        MMIPB_SendSignalToPBTask(MMIPB_SIG_PBAP_CALLLOG_READ, 0);
    }
}

/*******************************************************************************/
//  Description :解析vcard buffer data  
//  Parameter: data_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			   data_len[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E PBAPVcardToCallLog(
									 uint8 *data_ptr,//[IN]
                                     uint32 data_len,//[IN]
                                     MMICL_CALL_INFO_T *calllog_ptr //[OUT]
                                      )
{
#ifdef MMI_VCARD_SUPPORT
    MMIPB_ERROR_E result = MMIPB_ERROR_SUCCESS;
    VCARD_ERROR_E vcard_ret = VCARD_NO_ERROR;
    
    if(PNULL == data_ptr||0 == data_len||PNULL == calllog_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(calllog_ptr, 0, sizeof(MMICL_CALL_INFO_T));
    //开始解析
    vcard_ret = MMIAPIVCARD_VcardDataToCalllogEx(data_ptr, data_len, calllog_ptr);
    if(VCARD_NO_ERROR != vcard_ret)
    {
        if(VCARD_FORMAT_ERROR == vcard_ret)
        {
            result = MMIPB_ERROR_INVALID_FORMAT;
        }
        else if(VCARD_EMPTY_FILE == vcard_ret)
        {
            result = MMIPB_ERROR_NO_ENTRY;
        }
        else if(VCARD_SPACE_FULL == vcard_ret)
        {
            result = MMIPB_ERROR_NO_SPACE;
        }
        else 
        {
            result = MMIPB_ERROR_ERROR;
        }
    }
    return result;
#else
    return MMIPB_ERROR_ERROR;
#endif    
}

/*******************************************************************************/
//  Description :parse buffer data ,then save to calllog
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMICL_AddDataToCalllog(const char*  buff) 
{
    char*   p_searched_ptr = PNULL;
    uint32  offset_len = 0;
    char*   search_ptr = (char*)buff;
    uint32	extract_len = 0;
    char*	extract_buff_ptr = PNULL;
    MMICL_CALL_INFO_T calllog = {0};
    MMIPB_ERROR_E   ret = MMIPB_ERROR_ERROR;
    char*	search_endTag_ptr = PNULL;
    char*	merge_buff_ptr = PNULL; 
    uint32  merge_buff_len = 0;
    char*   rn_property = "\r\n"; 
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog TIME start this section %d", SCI_GetTickCount());
    
    if(PNULL == buff)
    {
        SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog buff is null");
        return MMIPB_ERROR_ERROR;
    }
    
    if (PNULL != s_vcard_buff_ptr)
    {
        merge_buff_len = strlen((char*)s_vcard_buff_ptr) + strlen((char*)buff);
    }
    else
    {
        merge_buff_len = strlen((char*)buff);
    }
    
    //分配拼接后 所需buffer
    merge_buff_ptr = SCI_ALLOC_APP(merge_buff_len + 1);
    
    if (PNULL == merge_buff_ptr)
    {
        SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog merge_buff_ptr Alloc fail!");
        
        if(PNULL != s_vcard_buff_ptr)
        {
            SCI_FREE(s_vcard_buff_ptr);
        }
        
        return MMIPB_ERROR_ERROR;
    }
    
    SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog merge_buff_len:%d", merge_buff_len);
    
    SCI_MEMSET(merge_buff_ptr, 0x00, merge_buff_len + 1);
    
    //上批缓存中有内容, 需要先将 上一批缓存中的data 与 下一批buffer中的data 拼接起来
    if (PNULL != s_vcard_buff_ptr)
    {
        SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog s_vcard_buff_ptr has data");
        
        strncpy(merge_buff_ptr, s_vcard_buff_ptr, strlen((char*)s_vcard_buff_ptr));
        
        //释放上一批缓存buffer
        SCI_FREE(s_vcard_buff_ptr);	
    }
    
    strncat(merge_buff_ptr, buff, strlen((char*)buff));

    //init arraycall_info
    if(PNULL == s_arraycall_info_ptr)
    {
        SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog allocate arraycall_info");
        s_arraycall_info_ptr = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
        if(PNULL == s_arraycall_info_ptr)
        {
            SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog arraycall_info is null");
            return MMIPB_ERROR_ERROR;
        }
#ifdef MMI_PDA_SUPPORT
        return_value = MMICL_ReadAllNV(s_arraycall_info_ptr);
#else
        return_value = MMICL_ReadNV(MMICL_CALL_ALL, s_arraycall_info_ptr);            //Just use all type here
#endif
    }
    
    MMI_CheckAllocatedMemInfo();
    do
    {
        //偏移长度
        p_searched_ptr = &merge_buff_ptr[offset_len];
        
        //判断是否只剩下"\r\n"
        if (0 == strcmp(rn_property, p_searched_ptr))
        {
            ret = MMIPB_ERROR_SUCCESS;
            break;
        }
        
        /************************************************************************/
        //buffer 提取规则如下:
        //BEGIN:VCARD与END:VCARD必须配对出现
        //先查找BEGIN:VCARD，再查找END:VCARD 
        //如果BEGIN:VCARD与END:VCARD都存在,说明数据完整，可以直接对其进行解析
        //否则说明数据不完整，需要和下一批buffer的数据一起解析
        /************************************************************************/        
        {
            SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog current sync record: %d", s_sync_calllog_count);
            //找BEGIN:VCARD 
            search_ptr = strstr(p_searched_ptr, VCARD_BEGIN_TAGS);
            SCI_TRACE_LOW("MMICL_AddDataToCalllog start find BEGIN:VCARD = %ld",search_ptr);
            
            if (PNULL != search_ptr)
            {        
                //找END:VCARD 
                search_endTag_ptr = strstr(search_ptr, VCARD_END_TAGS);
                SCI_TRACE_LOW("MMICL_AddDataToCalllog start find END:VCARD = %ld",search_endTag_ptr);
                
                if (PNULL != search_endTag_ptr)
                {
                    //begin and end 两个都找到的情况下
                    //此处使用p_searched_ptr来代替search_ptr来计算偏移量
                    extract_len = (strlen((char*)p_searched_ptr) - strlen((char*)search_endTag_ptr) + strlen((char*)VCARD_END_TAGS));	
                    
                    extract_buff_ptr = SCI_ALLOC_APP(extract_len + 1);
                    
                    if (PNULL == extract_buff_ptr)
                    {
                        SCI_TRACE_LOW("MMICL_AddDataToCalllog extract_buff_ptr alloc fail");
                        ret = MMIPB_ERROR_ERROR;
                        break;
                    }
                    
                    SCI_MEMSET(extract_buff_ptr, 0x00, (extract_len + 1));
                    
                    strncpy(extract_buff_ptr, p_searched_ptr, extract_len);

                    //解析一个记录的内容
                    ret = PBAPVcardToCallLog((uint8 *)extract_buff_ptr, (uint32)extract_len, &calllog);
                    
                    SCI_FREE(extract_buff_ptr);
                    
                    if(MMIPB_ERROR_SUCCESS != ret)
                    {
                        ret = MMIPB_ERROR_ERROR;
                        break;
                    }
                    
                    calllog.is_bt_dail = TRUE;

                    ret = MMICL_AddCalllogForPbap(&calllog, s_arraycall_info_ptr);
                    
                    if(MMIPB_ERROR_SUCCESS != ret)
                    {
                        break;
                    }
                    //解析保存成功后，index加一，用于下一个记录的解析
                    s_sync_calllog_count++;
                    
                    offset_len += extract_len;           
                    
                    continue;
                }
            }
            
            //数据不完整，需要先缓存起来 ps:处理每条buffer最后不完整的记录
            extract_len = strlen((char*)p_searched_ptr);
            
            if (PNULL != s_vcard_buff_ptr)
            {
                SCI_FREE(s_vcard_buff_ptr);
            }
            
            s_vcard_buff_ptr = SCI_ALLOC_APP(extract_len + 1);
            
            if (PNULL == s_vcard_buff_ptr)
            {
                ret = MMIPB_ERROR_ERROR;
                break;
            }
            
            SCI_MEMSET(s_vcard_buff_ptr, 0x00, extract_len + 1);
            
            strncpy(s_vcard_buff_ptr, p_searched_ptr, extract_len);
            
            ret = MMIPB_ERROR_SUCCESS;
            break;
            //缓存数据结束
        }
    }
    while(offset_len < strlen((char*)merge_buff_ptr));
    
    MMI_CheckAllocatedMemInfo();
    
    if (PNULL != merge_buff_ptr) /*lint !e774*/
    {
        SCI_FREE(merge_buff_ptr);
    }
    
    SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog TIME finish this section %d", SCI_GetTickCount());

    SCI_TRACE_LOW("[MMICL] MMICL_AddDataToCalllog s_sync_calllog_count = %d,ret = %d", s_sync_calllog_count,ret); 
    return ret;  		
}

/*******************************************************************************/
//  Description :check if the calllog record is already exist
//  Parameter: call_info[in]: calllog record, arraycall_info_ptr[in]: database
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
BOOLEAN IsSameRecord(MMICL_CALL_INFO_T *call_info, MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info_ptr)
{
    uint8 i=0, j = 0;
    MMICL_CALL_BCDINFO_T  *call_bcdinfo = PNULL;
    BOOLEAN is_same_num = FALSE;
    
    for(j=0; j<arraycall_info_ptr->record_num; j++)
    {
        call_bcdinfo = &(arraycall_info_ptr->call_bcdinfo[j]);
        is_same_num = 
            (call_bcdinfo->number_len == call_info->call_num.number.num_len) 
            && (0 == memcmp((char *)call_bcdinfo->number, (char *)call_info->call_num.number.party_num, 
            call_info->call_num.number.num_len)) 
#if defined(CL_SAVE_NAME_SUPPORT)
            && ((call_bcdinfo->name.is_use_nv_name == FALSE && call_info->name.is_use_nv_name == FALSE)
            ||(((call_bcdinfo->name.wstr_len == call_info.name.wstr_len)||(call_bcdinfo->name.wstr_len == MMICL_NAME_MAX_LEN && MMICL_NAME_MAX_LEN < call_info.name.wstr_len))
            && (0 == memcmp((char *)call_bcdinfo->name.wstr, (char *)call_info->name.wstr, 
            MIN(call_info->name.wstr_len, MMICL_NAME_MAX_LEN)*sizeof(wchar)))
            &&(call_bcdinfo->name.is_use_nv_name == TRUE && call_info->name.is_use_nv_name == TRUE)))   
#endif
            &&(call_bcdinfo->call_type == call_info->call_type)
            ;
        
        if((is_same_num
            ||((0 == call_bcdinfo->number_len) && (0 == call_info->call_num.number.num_len))) // num not present
            && call_bcdinfo->dual_sys == call_info->dual_sys
#ifdef MMI_CSP_SUPPORT
            && call_bcdinfo->line == call_info->line
#endif
            
#ifdef MMI_PDA_SUPPORT
            && call_bcdinfo->isvideocall== call_info->call_type
#endif
            && call_bcdinfo->call_type == call_info->call_type
            )
        {
#ifdef CALL_TIMES_SUPPORT
            for(i=0; i< MMICL_MAX_CALL_TIMES; i++)
            {
                if(0 == call_bcdinfo->call_start_time[i] && 0 != call_info->call_time.start_time)
                {
                    return FALSE;
                }
                if(call_bcdinfo->call_start_time[i] == call_info->call_time.start_time)
                {
                    return TRUE;
                }
            }
#else
            if(call_bcdinfo->call_start_time == call_info->call_time.start_time)
#endif
            return FALSE;
        }
        
    }
    return FALSE;
}

/*******************************************************************************/
//  Description : Calllog sync, add record to callog
//  Parameter: calllog_ptr[in]: calllog data pointer
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMICL_AddCalllogForPbap(MMICL_CALL_INFO_T *calllog_ptr, MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info_ptr)
{
    MMICL_CALL_INFO_T   call_info = {0};
    
    if (PNULL == calllog_ptr)
    {
        SCI_TRACE_LOW("[MMICL] MMICL_AddCalllogForPbap invalid param !");
        return MMIPB_ERROR_ERROR;
    }

    if (PNULL == arraycall_info_ptr)
    {
        SCI_TRACE_LOW("[MMICL] MMICL_AddCalllogForPbap invalid param !");
        return MMIPB_ERROR_ERROR;
    }

    call_info = *(MMICL_CALL_INFO_T *)calllog_ptr;

    if(IsSameRecord(calllog_ptr, arraycall_info_ptr))
    {
        return MMIPB_ERROR_SUCCESS;
    }
    else
    {
        AddNewDetailCallInfo(0, arraycall_info_ptr, call_info, (MMICL_CALL_TYPE_E)call_info.call_type);
    }
    
    return MMIPB_ERROR_SUCCESS;
}

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_InitPBAPDataList(void)
{				
    if(PNULL == s_list_head)
    {
        s_list_head = InitList();
    }
    else if(!IsListEmpty(s_list_head))
    {   
        ClearList(s_list_head);
    }
    
    if(PNULL == s_list_head)
    {
        return FALSE;
    }
    s_sync_calllog_count = 0;
    s_node_number = 1;
    return TRUE;
}

#if 0
PUBLIC void DEBUG_BackupData(void* buff,uint32 buff_len, char* token);
#endif

/*******************************************************************************/
//  Description : Insert pbap data into list
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_InsertPBAPDataToList(void* buff,uint16 buff_len)
{
#if 0
    #define TRACE_MAP_BUF_COUNT_BYTE 120
    uint16 len = 0;
    uint16 i;
    uint16 count = 0;
    
    for( i = 0; i < (buff_len/TRACE_MAP_BUF_COUNT_BYTE) +1;  i++ )
    {
        SCI_TRACE_LOW("[MMICL], MMICL_InsertPBAPDataToList, i = %d", i);
        len = buff_len - i*TRACE_MAP_BUF_COUNT_BYTE;
        count = i*TRACE_MAP_BUF_COUNT_BYTE;
        
        if( len >= TRACE_MAP_BUF_COUNT_BYTE)
        {
            SCI_TRACE_BUF("[MMICL] MMICL_InsertPBAPDataToList: buff is :", ((char*)buff+count), TRACE_MAP_BUF_COUNT_BYTE);
        }
        else
        {
            SCI_TRACE_BUF("[MMICL] MMICL_InsertPBAPDataToList: buff is :", ((char*)buff+count), len);
        }
    }

    DEBUG_BackupData(buff, (uint32)buff_len, "CALLLOG");

#endif

    if(0 == InsertList(s_list_head, buff,buff_len))
    {
        SCI_TRACE_LOW("MMICL_InsertPBAPDataToList failed");
        
        return FALSE;
    }
    
    return TRUE;
}

/*******************************************************************************/
//  Description : Parse PBAP data structure to calllog
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMICL_ParsePbapData(void)
{   
    char *data_ptr = PNULL;
    uint16 len = 0;
    
    GetElem(s_list_head, s_node_number, &data_ptr, &len);
    s_node_number++;
    
    return MMICL_AddDataToCalllog(data_ptr);
}

/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL void FinialPbapParse(void)
{
    if(PNULL != s_arraycall_info_ptr)
    {
        MMICL_WriteNV(MMICL_CALL_ALL, s_arraycall_info_ptr);
    }
    SCI_FREE(s_arraycall_info_ptr);
    s_arraycall_info_ptr = PNULL;

    FreeAllPbapData();
}

/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL void FreeAllPbapData(void)
{
    ClearList(s_list_head);
//    s_sync_calllog_count = 0;
    s_node_number = 1;
    SCI_FREE(s_vcard_buff_ptr);
    s_vcard_buff_ptr = PNULL;
}

/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMICL_FreeAllPbapData(void)
{
    FreeAllPbapData();
}

/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL PbapDataList* InitList(void) 
{   
    PbapDataList *h;  /* h 保存表头节点的指针*/
    
    h=(PbapDataList*)SCI_ALLOC_APPZ(PBAP_DATA_LIST_LEN);
    
    if(PNULL == h)  /*分配空间并检测*/
        return 0;
    
    h->data = 0;    /*把表头节点的数据域置空*/
    h->next = PNULL; /*把表头结点的链域置空*/
    h->len = 0;
    return h;
}

/*******************************************************************************/
//  Description : 向单链表的末尾添加一个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int InsertList(PbapDataList *L, void* buff,uint16 buff_len)
{
    PbapDataList *pInsert;
    PbapDataList *pHead;
    PbapDataList *pTmp;  //定义一个临时链表用来存放第一个节点 
    
    pHead = L;
    pTmp = pHead;
    
    
    pInsert = (PbapDataList*)SCI_ALLOC_APPZ(PBAP_DATA_LIST_LEN);//申请一个新节点 
    
    if(PNULL == pInsert)  /*分配空间并检测*/
    {
        return 0;
    } 
    
    pInsert->data = SCI_ALLOC_APPZ(buff_len+1);
    
    if(PNULL == pInsert->data)
    {
        SCI_FREE(pInsert);
        return 0;
    }
    
    SCI_MEMCPY(pInsert->data,buff,buff_len);
    pInsert->len = buff_len;
    pInsert->next = PNULL;
    
    while(pHead->next != PNULL)
    {
        pHead = pHead->next;
    }
    
    pHead->next = pInsert; //将链表末尾节点的下一结点指向新添加的节点 
    L = pTmp;
    return 1;
}

/*******************************************************************************/
//  Description:清除线性表L中的所有元素，即释放单链表L中所有的结点，使之成为一个空表
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL void ClearList(PbapDataList *pHead) 
{ 
    PbapDataList *pNext;            //定义一个与pHead相邻节点 
    PbapDataList *pHead_backup = pHead;
    
    if(pHead == PNULL) 
    { 
        return; 
    } 
    pHead = pHead->next;
    
    while(pHead!= PNULL) 
    { 
        pNext = pHead->next;//保存下一结点的指针 
        SCI_FREE(pHead->data);
        SCI_FREE(pHead); 
        pHead = pNext;      //表头下移 
    } 
    
    pHead_backup->next = PNULL;
}

/*******************************************************************************/
//  Description:获取单链表中第i个元素
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int GetElem(PbapDataList *L, int i, void** buff, uint16 *len)
{
    int j = 1;
    PbapDataList *p = L->next;
    
    while(p && (j < i))
    {
        p = p->next;
        ++j;
    }
    
    if(PNULL == p || (j > i))
    {
        return 0;
    }
    else
    {
        *buff = p->data;
        *len = p->len;
        return 1;
    }
}

/*******************************************************************************/
//  Description:判断单链表是否为空
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int IsListEmpty(PbapDataList *L)
{
    return(PNULL == (L->next));
}

/*******************************************************************************/
//  Description:判断链表节点个数
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL int ListNodeNum(PbapDataList *L)
{
    PbapDataList *p = L;
    int i = 0;
    while(PNULL != p->next)
    {
        i++;
        p = p->next;
    }
    return i;
}

/*****************************************************************************/
// 	Description : indication to other module that cl has updated
//	Global resource dependence : none
//  Author: cheney.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_HandlePBAPReadCnf(DPARAM  param)
{
    MMIPB_CNF_MSG_T *cnf_ptr = PNULL;
    char *data_ptr = PNULL;
    uint16 len = 0;
    MMICL_PBAP_PARSER_RESULT_T result = {0};/*lint !e578*/
    
    SCI_TRACE_LOW("MMICL_HandlePBAPReadCnf MMI_PB_PBAP_CALLLOG_READ_CNF");
    cnf_ptr = (MMIPB_CNF_MSG_T *)param;
    
    //canceled
    if(MMIAPIBT_IsPbapCancel())
    { 
        SCI_TRACE_LOW("MMICL: MMICL_HandlePBAPReadCnf canceled, SendFinishToBT");
        result.ret = MMICL_PBAP_PARSER_CANCELED;
        result.sync_count = s_sync_calllog_count;
        if(parser_callback)
        {
            parser_callback(&result);
        }
        FinialPbapParse();
    }
    //no space
    else if((PNULL != cnf_ptr)&&(MMIPB_ERROR_NO_SPACE == cnf_ptr->ret))
    {
        SCI_TRACE_LOW("MMICL: MMICL_HandlePBAPReadCnf no space, SendFinishToBT");
        result.ret = MMICL_PBAP_PARSER_NO_SPACE;
        result.sync_count = s_sync_calllog_count;
        if(parser_callback)
        {
            parser_callback(&result);
        }
        FinialPbapParse();
    }
    //no elem anymore
    else if(0 == GetElem(s_list_head, s_node_number, &data_ptr, &len))
    {
        SCI_TRACE_LOW("MMICL: MMICL_HandlePBAPReadCnf Finished, SendFinishToBT");
        result.ret = MMICL_PBAP_PARSER_FINISHED;
        result.sync_count = s_sync_calllog_count;

        if(parser_callback)
        {
            parser_callback(&result);
        }
        FinialPbapParse();
        if (MMK_IsOpenWin(MMICL_LOG_LIST_WIN_ID))
        {
            MMK_SendMsg(MMICL_LOG_LIST_WIN_ID, MSG_NOTIFY_LIST_CHECK_CONTENT, NULL);
        }
    }
    //send buffer to pb_task
    else
    {
        MMIPB_SendSignalToPBTask(MMIPB_SIG_PBAP_CALLLOG_READ, 0);
    }
}

#ifdef WIN32
/*****************************************************************************/
//     Description : Simulate PBAP calllog data
//     Global resource dependence : 
//     Author:Cheney Wu
//     Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_SimuPBAPDataToList(MMICL_CALL_TYPE_E call_type)
{
    
    uint8 simu_missed_calllog_array1[] =  {
        0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41,
            0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53,
            0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A,
            0x30, 0x39, 0x31, 0x38, 0x32, 0x31, 0x32, 0x38, 0x32, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45,
            0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x35, 0x54, 0x31, 0x35, 0x31, 0x33, 0x32, 0x33, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41,
            0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E,
            0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43,
            0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43,
            0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x31, 0x38, 0x32, 0x31, 0x32, 0x38, 0x32, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45,
            0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x35, 0x54, 0x31, 0x32, 0x32, 0x32, 0x35, 0x36, 0x0D, 0x0A, 0x45, 0x4E, 0x44,
            0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31,
            0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D,
            0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54,
            0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D,
            0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x35, 0x54, 0x31, 0x31, 0x32, 0x37, 0x35, 0x37, 0x0D,
            0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E,
            0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B,
            0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9,
            0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43,
            0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x34, 0x54, 0x31, 0x35, 0x35,
            0x39, 0x35, 0x33, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52,
            0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A,
            0x9F, 0xE8, 0xB7, 0xAF, 0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8,
            0xB7, 0xAF, 0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x32, 0x32, 0x37, 0x39, 0x32, 0x36, 0x38, 0x37, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52,
            0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x31,
            0x54, 0x31, 0x37, 0x34, 0x39, 0x33, 0x37, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7,
            0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8,
            0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x39, 0x31, 0x36, 0x37, 0x39, 0x30, 0x31, 0x0D, 0x0A,
            0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34,
            0x30, 0x35, 0x31, 0x38, 0x54, 0x32, 0x32, 0x32, 0x31, 0x31, 0x32, 0x0D
    };
    
    uint8 simu_missed_calllog_array2[] =  {
        0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E,
            0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B,
            0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9,
            0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x39, 0x31, 0x36, 0x37, 0x39, 0x30, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43,
            0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x38, 0x54, 0x30, 0x31, 0x31,
            0x37, 0x35, 0x31, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52,
            0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x33, 0x34, 0x33,
            0x39, 0x31, 0x39, 0x33, 0x32, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53,
            0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x30, 0x39, 0x35, 0x37, 0x34, 0x37, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42,
            0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A,
            0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x33, 0x34, 0x33, 0x39, 0x31, 0x39, 0x33, 0x32, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C,
            0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x30, 0x39, 0x35, 0x35, 0x35,
            0x33, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49,
            0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x33, 0x34, 0x33, 0x39, 0x31,
            0x39, 0x33, 0x32, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44,
            0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x30, 0x39, 0x35, 0x35, 0x30, 0x39, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47,
            0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A,
            0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x37, 0x30, 0x32, 0x34, 0x35, 0x32, 0x35, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D,
            0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x35, 0x54, 0x31, 0x33, 0x34, 0x38, 0x33, 0x30, 0x0D,
            0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E,
            0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B,
            0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9,
            0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43,
            0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x32, 0x30, 0x33,
            0x30, 0x31, 0x34, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52,
            0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C,
            0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6,
            0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52,
            0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33,
            0x54, 0x32, 0x30, 0x30, 0x38, 0x35, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31  
    };
    
    uint8 simu_missed_calllog_array3[] =  {
        0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D,
            0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54,
            0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D,
            0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x4D, 0x49, 0x53, 0x53, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x31, 0x54, 0x31, 0x31, 0x35, 0x37, 0x34, 0x37, 0x0D,
            0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A 
    };

    
    uint8 simu_dialed_calllog_array1[] =  {
        0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41,
            0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53,
            0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A,
            0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45,
            0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x35, 0x54, 0x31, 0x36, 0x32, 0x30, 0x32, 0x34, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41,
            0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E,
            0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43,
            0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43,
            0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x31, 0x38, 0x32, 0x31, 0x32, 0x38, 0x32, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45,
            0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x35, 0x54, 0x31, 0x32, 0x33, 0x37, 0x34, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44,
            0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31,
            0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF, 0xE5, 0xAE, 0xB6, 0x0D,
            0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF, 0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x54,
            0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x32, 0x32, 0x37, 0x39, 0x32, 0x36, 0x38, 0x37, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D,
            0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x31, 0x54, 0x31, 0x38, 0x30, 0x30, 0x33, 0x37, 0x0D,
            0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E,
            0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF,
            0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF, 0xE5, 0xAE,
            0xB6, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x32, 0x32, 0x37, 0x39, 0x32, 0x36, 0x38, 0x37, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43,
            0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x31, 0x54, 0x31, 0x37, 0x35,
            0x39, 0x34, 0x37, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52,
            0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66,
            0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x30, 0x35, 0x33, 0x35, 0x33, 0x33, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52,
            0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x30,
            0x54, 0x31, 0x32, 0x31, 0x32, 0x34, 0x30, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x48, 0x65, 0x6E, 0x72, 0x79, 0x20, 0x43, 0x68, 0x65, 0x6E, 0x0D, 0x0A, 0x4E, 0x3A,
            0x2A, 0x48, 0x65, 0x6E, 0x72, 0x79, 0x20, 0x43, 0x68, 0x65, 0x6E, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x30, 0x30, 0x36, 0x30, 0x31, 0x36, 0x32,
            0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30,
            0x31, 0x34, 0x30, 0x35, 0x31, 0x39, 0x54, 0x30, 0x39, 0x32, 0x35, 0x35, 0x37, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A,
            0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55,
            0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D 
    };
    
    uint8 simu_dialed_calllog_array2[] =  {
        0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54,
            0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x39, 0x31, 0x36, 0x37, 0x39, 0x30, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D,
            0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x38, 0x54, 0x31, 0x30, 0x33, 0x34, 0x32, 0x38, 0x0D,
            0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E,
            0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B,
            0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9,
            0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x39, 0x31, 0x36, 0x37, 0x39, 0x30, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43,
            0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x31, 0x39, 0x34,
            0x34, 0x31, 0x34, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52,
            0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88,
            0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6,
            0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x39, 0x31, 0x36, 0x37, 0x39, 0x30, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52,
            0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37,
            0x54, 0x31, 0x39, 0x34, 0x33, 0x34, 0x30, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7,
            0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8,
            0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x39, 0x31, 0x36, 0x37, 0x39, 0x30, 0x31, 0x0D, 0x0A,
            0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34,
            0x30, 0x35, 0x31, 0x37, 0x54, 0x31, 0x39, 0x33, 0x39, 0x30, 0x34, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43,
            0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46,
            0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38,
            0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37,
            0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A,
            0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x31, 0x31, 0x30, 0x35, 0x34, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49,
            0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54,
            0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55,
            0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38,
            0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41,
            0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x31, 0x30, 0x34, 0x39, 0x35, 0x31, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A,
            0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41,
            0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6 
    };
    
    uint8 simu_dialed_calllog_array3[] =  {
        0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B,
            0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43,
            0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x31,
            0x30, 0x34, 0x39, 0x31, 0x32, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56,
            0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9,
            0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C,
            0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D,
            0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35,
            0x31, 0x37, 0x54, 0x31, 0x30, 0x34, 0x38, 0x32, 0x31, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52,
            0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38,
            0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5,
            0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43,
            0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x31,
            0x30, 0x31, 0x31, 0x30, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56,
            0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x37,
            0x30, 0x32, 0x34, 0x35, 0x32, 0x35, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49,
            0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x36, 0x54, 0x31, 0x30, 0x32, 0x35, 0x33, 0x36, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48,
            0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52,
            0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C,
            0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D,
            0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x36, 0x54, 0x31, 0x30, 0x30, 0x36, 0x33, 0x31, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43,
            0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46,
            0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B,
            0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B,
            0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54,
            0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x35, 0x54, 0x31, 0x36, 0x32, 0x30, 0x30, 0x35, 0x0D, 0x0A, 0x45, 0x4E,
            0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E,
            0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x32, 0x32, 0x35, 0x32, 0x38, 0x37, 0x37, 0x37, 0x36, 0x0D, 0x0A,
            0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34,
            0x30, 0x35, 0x31, 0x35, 0x54, 0x31, 0x36, 0x31, 0x35, 0x33  
    };
    
    uint8 simu_dialed_calllog_array4[] =  {
        0x32, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49,
            0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x31, 0x32, 0x33, 0x0D, 0x0A, 0x58, 0x2D,
            0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35,
            0x31, 0x35, 0x54, 0x31, 0x36, 0x31, 0x30, 0x34, 0x36, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52,
            0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30,
            0x3A, 0x30, 0x39, 0x37, 0x30, 0x32, 0x34, 0x35, 0x32, 0x35, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D,
            0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x35, 0x54, 0x31, 0x36, 0x30, 0x39, 0x35, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43,
            0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46,
            0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x31, 0x32, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C,
            0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x35, 0x54, 0x31, 0x36, 0x30, 0x36, 0x35, 0x34,
            0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F,
            0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61,
            0x6E, 0x67, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x30, 0x35, 0x33, 0x35, 0x33, 0x33, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D,
            0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x34, 0x54, 0x32, 0x30,
            0x31, 0x37, 0x33, 0x39, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45,
            0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66,
            0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x30, 0x35, 0x33, 0x35, 0x33, 0x33, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49,
            0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31,
            0x34, 0x54, 0x32, 0x30, 0x31, 0x36, 0x30, 0x33, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44,
            0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A,
            0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE,
            0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D,
            0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31,
            0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x31, 0x30, 0x33, 0x32, 0x31, 0x39, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56,
            0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54,
            0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D,
            0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33,
            0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44,
            0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x31, 0x30, 0x32, 0x36, 0x35, 0x39, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47,
            0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A,
            0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x31, 0x30, 0x35, 0x38, 0x36, 0x30, 0x38, 0x36 
    };
    
    
    uint8 simu_dialed_calllog_array5[] =  {
        0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A, 0x32, 0x30,
            0x31, 0x34, 0x30, 0x35, 0x31, 0x32, 0x54, 0x31, 0x32, 0x30, 0x33, 0x32, 0x37, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A,
            0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4B, 0x61, 0x79, 0x20, 0x43, 0x68, 0x65, 0x6E,
            0x0D, 0x0A, 0x4E, 0x3A, 0x2A, 0x4B, 0x61, 0x79, 0x20, 0x43, 0x68, 0x65, 0x6E, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x31, 0x30, 0x31, 0x35, 0x30, 0x37,
            0x33, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x44, 0x49, 0x41, 0x4C, 0x45, 0x44, 0x3A,
            0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x30, 0x54, 0x32, 0x30, 0x34, 0x33, 0x30, 0x32, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A 
    };
    
    uint8 simu_recived_calllog_array1[] =  {
        0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41,
            0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53,
            0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x9B, 0x85, 0xE5, 0x90, 0x9B, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A,
            0x30, 0x39, 0x31, 0x38, 0x32, 0x31, 0x32, 0x38, 0x32, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45,
            0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x35, 0x54, 0x31, 0x35, 0x30, 0x33, 0x34, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56,
            0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A,
            0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E,
            0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C,
            0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41,
            0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x35, 0x54, 0x31, 0x31, 0x33, 0x31, 0x30, 0x30, 0x0D,
            0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E,
            0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF,
            0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF, 0xE5, 0xAE,
            0xB6, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x32, 0x32, 0x37, 0x39, 0x32, 0x36, 0x38, 0x37, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43,
            0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x34, 0x54, 0x31,
            0x30, 0x31, 0x33, 0x31, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56,
            0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9,
            0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C,
            0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D,
            0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34,
            0x30, 0x35, 0x32, 0x34, 0x54, 0x31, 0x30, 0x30, 0x36, 0x31, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43,
            0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46,
            0x2D, 0x38, 0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF, 0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38,
            0x3A, 0xE6, 0x88, 0x90, 0xE5, 0x8A, 0x9F, 0xE8, 0xB7, 0xAF, 0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x32, 0x32, 0x37, 0x39, 0x32, 0x36, 0x38,
            0x37, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45,
            0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x33, 0x54, 0x32, 0x31, 0x32, 0x39, 0x34, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45,
            0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53,
            0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54,
            0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39,
            0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52,
            0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x33, 0x54, 0x31 
    };
    
    uint8 simu_recived_calllog_array2[] =  {
        0x39, 0x30, 0x38, 0x32, 0x30, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56,
            0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x32, 0x38,
            0x32, 0x32, 0x36, 0x32, 0x36, 0x36, 0x39, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45,
            0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x32, 0x54, 0x31, 0x36, 0x30, 0x34, 0x35, 0x30, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52,
            0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A,
            0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x32, 0x38, 0x32, 0x32, 0x36, 0x32, 0x36, 0x36, 0x39, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43,
            0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x32, 0x32,
            0x54, 0x31, 0x36, 0x30, 0x33, 0x34, 0x32, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5,
            0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9,
            0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A,
            0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30,
            0x31, 0x34, 0x30, 0x35, 0x31, 0x38, 0x54, 0x31, 0x37, 0x34, 0x39, 0x34, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A,
            0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55,
            0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A,
            0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE5, 0xAE, 0xB6, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x34, 0x32, 0x35, 0x32, 0x33, 0x33, 0x33, 0x37, 0x36, 0x0D, 0x0A,
            0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30,
            0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x32, 0x30, 0x30, 0x38, 0x30, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A,
            0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55,
            0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46,
            0x2D, 0x38, 0x3A, 0xE7, 0x88, 0xB8, 0xE7, 0x88, 0xB8, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x39, 0x31, 0x36,
            0x37, 0x39, 0x30, 0x31, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49,
            0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x31, 0x39, 0x34, 0x35, 0x32, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A,
            0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41,
            0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53,
            0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A,
            0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45,
            0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x37, 0x54, 0x31, 0x30, 0x35, 0x30, 0x34, 0x31, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56,
            0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A,
            0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE9, 0x99, 0xB3, 0xE5, 0xB0, 0x8F, 0xE5, 0xA8, 0x9F, 0xE8, 0x8C, 0xB6, 0xE8, 0x91, 0x89,
            0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A  
    };
    
    uint8 simu_recived_calllog_array3[] =  {
        0xE9, 0x99, 0xB3, 0xE5, 0xB0, 0x8F, 0xE5, 0xA8, 0x9F, 0xE8, 0x8C, 0xB6, 0xE8, 0x91, 0x89, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x3A, 0x30, 0x39, 0x33, 0x37, 0x38, 0x37, 0x35, 0x31, 0x32, 0x37,
            0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A,
            0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x36, 0x54, 0x31, 0x36, 0x31, 0x35, 0x30, 0x39, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49,
            0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54,
            0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x32, 0x32, 0x36, 0x35, 0x39, 0x36, 0x31, 0x37, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44,
            0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x36, 0x54, 0x31, 0x33, 0x33, 0x37, 0x32, 0x37,
            0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F,
            0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x32, 0x32, 0x36, 0x35, 0x39, 0x36, 0x31,
            0x34, 0x37, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45,
            0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x36, 0x54, 0x31, 0x33, 0x33, 0x36, 0x34, 0x36, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45,
            0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D,
            0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x37, 0x30, 0x32, 0x34, 0x35, 0x32, 0x35, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C,
            0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x36, 0x54, 0x31, 0x30, 0x30, 0x34,
            0x31, 0x30, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53,
            0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F,
            0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89,
            0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D,
            0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31,
            0x34, 0x54, 0x31, 0x38, 0x32, 0x39, 0x31, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44,
            0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A,
            0x30, 0x32, 0x37, 0x37, 0x32, 0x31, 0x35, 0x36, 0x32, 0x35, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45,
            0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x34, 0x54, 0x31, 0x37, 0x31, 0x34, 0x33, 0x36, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56,
            0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A,
            0x46, 0x4E, 0x3A, 0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x38, 0x32, 0x34, 0x32, 0x39, 0x34, 0x30, 0x38, 0x0D, 0x0A, 0x58, 0x2D, 0x49,
            0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30,
            0x35, 0x31, 0x33, 0x54, 0x31, 0x35, 0x31, 0x34, 0x33, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41,
            0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A,
            0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x30, 0x35, 0x33, 0x35, 0x33, 0x33,
            0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44,
            0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x31, 0x30, 0x34, 0x31, 0x33, 0x33, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47,
            0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A 
    };
    
    uint8 simu_recived_calllog_array4[] =  {
        0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67,
            0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x30, 0x35, 0x33, 0x35, 0x33, 0x33, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41,
            0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x31, 0x30,
            0x34, 0x31, 0x30, 0x31, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45,
            0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66,
            0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x30, 0x35, 0x33, 0x35, 0x33, 0x33, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49,
            0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30,
            0x35, 0x31, 0x33, 0x54, 0x31, 0x30, 0x32, 0x36, 0x33, 0x39, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41,
            0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A,
            0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x35, 0x38, 0x36, 0x35, 0x35, 0x39, 0x37,
            0x37, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44,
            0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x31, 0x30, 0x32, 0x36, 0x30, 0x33, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47,
            0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20,
            0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x4E, 0x3A, 0x2A, 0x4A, 0x65, 0x66, 0x66, 0x20, 0x46, 0x61, 0x6E, 0x67, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32,
            0x30, 0x35, 0x33, 0x35, 0x33, 0x33, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45,
            0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x33, 0x54, 0x31, 0x30, 0x31, 0x31, 0x32, 0x32, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52,
            0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A,
            0x0D, 0x0A, 0x4E, 0x3A, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x58, 0x2D, 0x30, 0x3A, 0x30, 0x39, 0x31, 0x30, 0x35, 0x38, 0x36, 0x30, 0x38, 0x36, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43,
            0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x32,
            0x54, 0x31, 0x32, 0x30, 0x36, 0x30, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5,
            0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9,
            0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A,
            0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30,
            0x31, 0x34, 0x30, 0x35, 0x31, 0x31, 0x54, 0x31, 0x31, 0x35, 0x36, 0x35, 0x37, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A,
            0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55,
            0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46,
            0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31,
            0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49,
            0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x31, 0x54, 0x31, 0x31, 0x34, 0x39, 0x33, 0x35, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A,
            0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x56, 0x45, 0x52, 0x53, 0x49 
    };
    
    uint8 simu_recived_calllog_array5[] =  {
        0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6,
            0x89, 0x8B, 0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x4E, 0x3B, 0x43, 0x48, 0x41, 0x52, 0x53, 0x45, 0x54, 0x3D, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x3A, 0xE5, 0xAE, 0xB9, 0xE7, 0x9C, 0x9F, 0xE6, 0x89, 0x8B,
            0xE6, 0xA9, 0x9F, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x32, 0x38, 0x39, 0x31, 0x33, 0x37, 0x36, 0x33, 0x0D, 0x0A, 0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43,
            0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30, 0x31, 0x34, 0x30, 0x35, 0x31, 0x31,
            0x54, 0x31, 0x31, 0x33, 0x31, 0x32, 0x32, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D,
            0x0A, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x3A, 0x32, 0x2E, 0x31, 0x0D, 0x0A, 0x46, 0x4E, 0x3A, 0x54, 0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x0D, 0x0A, 0x4E, 0x3A, 0x54,
            0x57, 0x4E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x0D, 0x0A, 0x54, 0x45, 0x4C, 0x3B, 0x43, 0x45, 0x4C, 0x4C, 0x3A, 0x30, 0x39, 0x33, 0x35, 0x31, 0x32, 0x30, 0x31, 0x38, 0x38, 0x0D, 0x0A,
            0x58, 0x2D, 0x49, 0x52, 0x4D, 0x43, 0x2D, 0x43, 0x41, 0x4C, 0x4C, 0x2D, 0x44, 0x41, 0x54, 0x45, 0x54, 0x49, 0x4D, 0x45, 0x3B, 0x52, 0x45, 0x43, 0x45, 0x49, 0x56, 0x45, 0x44, 0x3A, 0x32, 0x30,
            0x31, 0x34, 0x30, 0x35, 0x30, 0x39, 0x54, 0x32, 0x30, 0x34, 0x34, 0x32, 0x38, 0x0D, 0x0A, 0x45, 0x4E, 0x44, 0x3A, 0x56, 0x43, 0x41, 0x52, 0x44, 0x0D, 0x0A
    };

    switch(call_type)
    {
    case MMICL_BT_CALL_MISSED:
        MMICL_InsertPBAPDataToList(simu_missed_calllog_array1, strlen(simu_missed_calllog_array1));
        MMICL_InsertPBAPDataToList(simu_missed_calllog_array2, strlen(simu_missed_calllog_array2));
        MMICL_InsertPBAPDataToList(simu_missed_calllog_array3, strlen(simu_missed_calllog_array3));
        break;
    
    case MMICL_BT_CALL_DIALED:
        MMICL_InsertPBAPDataToList(simu_dialed_calllog_array1, strlen(simu_dialed_calllog_array1));
        MMICL_InsertPBAPDataToList(simu_dialed_calllog_array2, strlen(simu_dialed_calllog_array2));
        MMICL_InsertPBAPDataToList(simu_dialed_calllog_array3, strlen(simu_dialed_calllog_array3));
        MMICL_InsertPBAPDataToList(simu_dialed_calllog_array4, strlen(simu_dialed_calllog_array4));
        MMICL_InsertPBAPDataToList(simu_dialed_calllog_array5, strlen(simu_dialed_calllog_array5));
        break;

    case MMICL_BT_CALL_RECEIVED:
        MMICL_InsertPBAPDataToList(simu_recived_calllog_array1, strlen(simu_recived_calllog_array1));
        MMICL_InsertPBAPDataToList(simu_recived_calllog_array2, strlen(simu_recived_calllog_array2));
        MMICL_InsertPBAPDataToList(simu_recived_calllog_array3, strlen(simu_recived_calllog_array3));
        MMICL_InsertPBAPDataToList(simu_recived_calllog_array4, strlen(simu_recived_calllog_array4));
        MMICL_InsertPBAPDataToList(simu_recived_calllog_array5, strlen(simu_recived_calllog_array5));
        break;

    default:
        MMIAPIPB_InsertPBAPDataToList(simu_recived_calllog_array1, strlen(simu_recived_calllog_array1));
        MMIAPIPB_InsertPBAPDataToList(simu_recived_calllog_array2, strlen(simu_recived_calllog_array2));
        MMIAPIPB_InsertPBAPDataToList(simu_recived_calllog_array3, strlen(simu_recived_calllog_array3));
        MMIAPIPB_InsertPBAPDataToList(simu_recived_calllog_array4, strlen(simu_recived_calllog_array4));
        MMIAPIPB_InsertPBAPDataToList(simu_recived_calllog_array5, strlen(simu_recived_calllog_array5));
        break;
    }

    return TRUE;
}
#endif
#endif

/*****************************************************************************/
//  Description : 获得显示可用的电话号码
//  Global resource dependence : none
//  [IN]:cl_nv_info,nv存储的单条calllog信息
//  [OUT]:tele_num,可显示的电话号码，使用的Ascii
//  [IN]:可显示的电话号码的最大长度
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICL_GenDispNumber(const MMICL_CALL_BCDINFO_T* cl_nv_info,
                                               uint8     *tele_num,
                                               uint16    max_tele_len)//!!!!!!!!!可显示的号码的最大长度 + 2(/0 字符串结束符号))   //RETURN: 被选中的ITEM的总数
{
    uint16 tele_len = 0;
    BOOLEAN is_net_num = FALSE;

    if((PNULL == cl_nv_info)
        ||(PNULL == tele_num)
        || (0 == max_tele_len))
    {
        SCI_TraceLow("param is error!");
        return tele_len;
    }

    //判断电话号码的来源，如果来源于网络，则需要使用解析网络BCD号码的API MMIAPICOM_GenNetDispNumber，否则使用MMIAPICOM_GenDispNumber
    if (MMICL_CALL_MISSED == cl_nv_info->call_type
        || MMICL_CALL_RECEIVED == cl_nv_info->call_type
        )
    {
        is_net_num = TRUE;
    }

    if(is_net_num)
    {
        tele_len = MMIAPICOM_GenNetDispNumber(cl_nv_info->number_type,
                                              cl_nv_info->number_len,
                                              cl_nv_info->number,
                                              tele_num,
                                              max_tele_len);
    }
    else
    {
        tele_len = MMIAPICOM_GenDispNumber(cl_nv_info->number_type,
                                           cl_nv_info->number_len,
                                           cl_nv_info->number,
                                           tele_num,
                                           max_tele_len);
    }

    return tele_len;
}
/*****************************************************************************/
//  Description : 获得BCD号码
//  Global resource dependence : none
//  [IN]:cl_nv_info,nv存储的单条calllog信息
//  [OUT]:tele_num,可显示的电话号码，使用的Ascii
//  [IN]:可显示的电话号码的最大长度
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_GetBCDNumber(const MMICL_CALL_BCDINFO_T* cl_nv_info,
                                             MMIPB_BCD_NUMBER_T* bcd_num)
{
    if((PNULL == cl_nv_info) || (PNULL == bcd_num))
    {
        SCI_TraceLow("param is error!");
        return;
    }
    bcd_num->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(cl_nv_info->number_type, MN_NUM_PLAN_UNKNOW);
    bcd_num->number_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN, cl_nv_info->number_len);
    MMI_MEMCPY(bcd_num->number,
               sizeof(bcd_num->number),
               (void*)cl_nv_info->number,
               sizeof(cl_nv_info->number),
               bcd_num->number_len
                            );
}
/*****************************************************************************/
//  Description : 获取时间日期显示的WSTRING
//	Global resource dependence :
//  [IN/OUT]:time_data_str,时间日期显示字串
//  [IN]:max_len,日期最大长度
//  [IN]:callinfo，nv存储的单条calllog信息
//  [IN]:call_time_index，代表第几次来电
//  [IN]:symbol，日期显示的间隔符
//  [IN]:is_display_year，是否显示年
//  [RETURN]:返回字串长度
//  Author:feiyue.ji
//	Note:  今天的用时间，其他的用日期
/*****************************************************************************/
PUBLIC uint16 MMIAPICL_GenDispTimeAndDate(const MMICL_CALL_BCDINFO_T *callinfo,
                                                          wchar* time_data_str,
                                                          uint16 str_max_len,
                                                          uint16 call_time_index,
                                                          char symbol, //in
                                                          BOOLEAN is_display_year)
{
    SCI_DATE_T sys_data = {0};
    MMI_TM_T tm = {0};
    uint8      date_str[20] = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;
    uint16     str_len = 0;

    if ((callinfo == PNULL)
        ||(time_data_str == PNULL)
        ||(call_time_index>=MMICL_MAX_CALL_TIMES)
        ||(0 == str_max_len))
    {
        return str_len;
    }
#ifdef CALL_TIMES_SUPPORT
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time[call_time_index]);
#else
    tm = MMIAPICOM_Second2Tm( callinfo->call_start_time);
#endif
    TM_GetSysDate(&sys_data);

    cl_sec =MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,(uint32)(tm.tm_year));
    sys_sec =MMIAPICOM_Tm2Second(0,0,0,sys_data.mday,sys_data.mon,sys_data.year);

    //如果是当天,则只显示时间；否则显示年月日，显示格式2021/01/01
    if(cl_sec == sys_sec)//当天时间
    {
        MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min, date_str,20);

        MMI_STRNTOWSTR(time_data_str,str_max_len,(const uint8*)date_str,
                    strlen((char*)date_str),strlen((char*)date_str));
    }
    else //use Date
    {
        MMIAPISET_FormatDateStrByDateEx(tm.tm_year, tm.tm_mon, tm.tm_mday,symbol, date_str,20,is_display_year);
        MMI_STRNTOWSTR(time_data_str,str_max_len,(const uint8*)date_str,(uint32)strlen((char*)date_str),(uint32)strlen((char*)date_str));
    }

    str_len = (uint16)MMIAPICOM_Wstrlen(time_data_str);
    return str_len;
}


