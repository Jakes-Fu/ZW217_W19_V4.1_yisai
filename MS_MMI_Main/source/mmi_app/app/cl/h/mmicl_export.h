/*****************************************************************************
** File Name:      mmicl_export.h                                                   *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng       Create
******************************************************************************/

#ifndef _MMICL_H_
#define _MMICL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "mmicc_export.h"
#include "mmi_event_api.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMICL_NAME_MAX_LEN          20  //20*2*80 Bytes
#ifdef MMI_CL_NUM_MAX_SUPPORT
#define MMICL_MAX_CALL_TIMES       20
#else
#define MMICL_MAX_CALL_TIMES       10
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

//call log 的类型
typedef enum
{
    MMICL_CALL_MISSED = 0,
    MMICL_CALL_RECEIVED,
    MMICL_CALL_DIALED,
#if defined(MMI_BLACKLIST_SUPPORT)
    MMICL_CALL_REFUSED,
#endif
    MMICL_CALL_MATCHED,
    //#ifdef MMI_PDA_SUPPORT
    MMICL_CALL_ALL,
    //#endif
#ifdef BT_DIALER_SUPPORT
    MMICL_BT_CALL_MISSED,
    MMICL_BT_CALL_RECEIVED,
    MMICL_BT_CALL_DIALED,
    MMICL_BT_CALL_ALL,
#endif
    MMICL_CALL_PB_NUMBER_MATCHED,
	MMICL_CALL_TYPE_MAX
} MMICL_CALL_TYPE_E;

#ifdef BT_PBAP_SUPPORT
// BT PBAP parse result
typedef enum _MMICL_PBAP_PARSER_RESULT_E
{
    MMICL_PBAP_PARSER_FINISHED,
    MMICL_PBAP_PARSER_NO_SPACE,
    MMICL_PBAP_PARSER_CANCELED   
} MMICL_PBAP_PARSER_RESULT_E;
#endif

//电话号码的结构
typedef struct
{
    BOOLEAN             is_num_present;             //电话号码是否存在
    MN_CALLED_NUMBER_T  number;
} MMICL_CALL_NUMBER_T;

 //存储在NV中某次通话对应的名字，长度为MMICL_NAME_MAX_LEN，可小于CC_MAX_PBNAME_LEN
typedef struct _MMICL_NAME_T
{
//通话记录的nv中保存了且仅保存了姓名
//TRUE,由adn/fdn/sdn呼出的号码，显示该姓名
//FALSE,由pb/cl/dial等呼出的号码，显示由PB查找到的姓名、大头贴
    BOOLEAN is_use_nv_name;
    uint16   wstr_len;                   
    wchar   wstr[MMICL_NAME_MAX_LEN + 1];   
} MMICL_NAME_T;

//call的信息，包括电话号码和时间信息
typedef struct
{
#ifdef MMI_CSP_SUPPORT
    uint8 line;  //the call from line1 or line2
#endif
    BOOLEAN is_emergency_call; // 是否是紧急号码呼出
#ifdef BT_DIALER_SUPPORT
    BOOLEAN is_bt_dail;    //是否蓝牙拨号
#endif
#if defined(CL_SAVE_NAME_SUPPORT)
    MMICL_NAME_T name;
#endif
    MN_DUAL_SYS_E       dual_sys;
    MMICL_CALL_NUMBER_T call_num;   //call number
    CC_CALL_TIME_T      call_time;  //call时间
    CC_CALL_MODE_E		call_type;	//@xiongxiaoyan 20060406 add    
} MMICL_CALL_INFO_T;

//存储在NV中有关电话的信息
typedef struct
{
    MN_DUAL_SYS_E		dual_sys;
#ifdef MMI_LDN_SUPPORT
    BOOLEAN is_sim_record;
#endif
    BOOLEAN             is_num_present;                 //电话号码是否存在
    uint8               number_len;
    uint8               number[MMIPB_BCD_NUMBER_MAX_LEN];   //the dialling number
    MN_NUMBER_TYPE_E    number_type;
    MN_NUMBER_PLAN_E    number_plan;
#ifdef CALL_TIMES_SUPPORT
    uint32              call_start_time[MMICL_MAX_CALL_TIMES];                //call起始时间
    uint32              call_duration_time[MMICL_MAX_CALL_TIMES];             //@Lin.Lin, call持续时间, 2005-4-4
#else
    uint32              call_start_time;                //call起始时间
    uint32              call_duration_time;             //@Lin.Lin, call持续时间, 2005-4-4
#endif    
    CC_CALL_MODE_E		isvideocall;	//@zhaohui add,is VT call or normal call 
    BOOLEAN is_emergency_call; //是否是紧急号码呼出
#ifdef CALL_TIMES_SUPPORT
    uint16                   call_times;
#endif
#if defined(CL_SAVE_NAME_SUPPORT)
    MMICL_NAME_T    name;  //通话时显示的名字(total size in NV:44*80 Bytes)
#endif
     
    MMICL_CALL_TYPE_E   call_type;
    uint16              array_index;
#ifdef MMI_CSP_SUPPORT
    uint8 line;
#endif
#ifdef BT_DIALER_SUPPORT
    BOOLEAN is_bt_dail;  
#endif
#ifdef ADULT_WATCH_SUPPORT
    BOOLEAN is_missed_call_read;
#endif
} MMICL_CALL_BCDINFO_T;
#ifdef BT_PBAP_SUPPORT
typedef struct _MMICL_PBAP_PARSER_RESULT_T
{
    MMICL_PBAP_PARSER_RESULT_E ret;
    uint16 sync_count;
} MMICL_PBAP_PARSER_RESULT_T;

typedef void(*MMICL_PBAP_PARSER_CALLBACK_FUNC)(MMICL_PBAP_PARSER_RESULT_T *result);
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : initialize call log
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMIAPICL_Init(void);

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
                          );

/*****************************************************************************/
//  Description :open call number window
//  Global resource dependence : 
//  Author:Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenEditBeforeDialWin(uint8   tel_len,
                                        uint8*  tel_num_ptr
                                        );

/*****************************************************************************/
// 	Description : if enter into call log main menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_OpenMainMenuWindow(void);

/*****************************************************************************/
// 	Description : make a Mo call by record index
//	Global resource dependence : 
//  Author:Kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_MakeCallByRecordIndex(
									   MMICL_CALL_TYPE_E	call_type,
									   uint16				record_index
									   );

/*****************************************************************************/
// 	Description : to handle enter missed,received and dialed calls menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenCallsListWindow(
                               MMICL_CALL_TYPE_E call_type
                               );

/*****************************************************************************/
// 	Description : get selected cl item number
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSelectedItemNum(uint8* tele_num,  uint8 max_tele_len);									

/*****************************************************************************/
// 	Description : MMIAPICL_OpenCLWindow
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenCLWindow(void);

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : MMIAPICL_OpenCLWindow
//	Global resource dependence : 
//  Author:cheney.wu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenBTCLWindow(void);

/*****************************************************************************/
// 	Description : Get win id for bt sync win id
//	Global resource dependence : 
//  Author:cheney.wu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPICL_GetActivateBTWinId(void);

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :Check if the PBAP data is synced
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_IsPbapDataSynced(void);

/*******************************************************************************/
//  Description : reset pbap sync data count
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMICL_ResetPbapSyncCount(void);

/*******************************************************************************/
//  Description :解析vcard buffer data  
//  Parameter: data_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			  data_len[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMICL_NotifyPBStartParser(MMICL_PBAP_PARSER_CALLBACK_FUNC callback);

/*****************************************************************************/
//  Description : Deregisters the PBAP Access Client
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMICL_PbapClientDeregister(void);

/*****************************************************************************/
// 	Description : insert pbap data
//	Global resource dependence : none
//  Author: cheney.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_InsertPBAPDataToList(void* buff, uint16 buff_len);

/*****************************************************************************/
// 	Description : callog handler for MMI_PB_PBAP_CALLLOG_READ_CNF
// 	Global resource dependence : none
// Author: cheney.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_HandlePBAPReadCnf(DPARAM  param);

/*****************************************************************************/
// 	Description : parse pbap data
//	Global resource dependence : none
//  Author: cheney.wu
//	Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMICL_ParsePbapData(void);

/*****************************************************************************/
// 	Description : clean calllog wait win
// 	Global resource dependence : none
// Author: cheney.wu
// 	Note:
/*****************************************************************************/
PUBLIC void MMICL_ClosePBAPWaitWin(void);

/*****************************************************************************/
// 	Description : indication to other module that cl has updated
// 	Global resource dependence : none
// Author: cheney.wu
// 	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_DeleteAllPbapSyncData(void);

/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMICL_FreeAllPbapData(void);

/******************************************************************************/
// Description: StopPbapTimer
// Global resource dependence: 
// Author: cheney.wu
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIAPICL_StopPbapTimer(void);

#ifdef WIN32
/*****************************************************************************/
//     Description : Simulate PBAP calllog data
//     Global resource dependence : 
//     Author:Cheney Wu
//     Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_SimuPBAPDataToList(MMICL_CALL_TYPE_E call_type);
#endif
#endif
#endif

/*****************************************************************************/
// 	Description : export GetCallDurationTime
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICL_GetCallDurationTime( uint32 start_time, uint32 stop_time);

/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 找到的号码的个数
/*****************************************************************************/
PUBLIC uint32 MMIAPICL_SearchList(MMI_STRING_T *search_str);

/*****************************************************************************/
//  Description : 外部搜索退出，当外部搜索退出是，这个接口一定要调用
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIAPICL_SearchExit(void);

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
										   );

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的号码
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchEntryCalledNumber(MN_CALLED_NUMBER_T *called_num, //OUT
									  uint32 index//IN
									  );
									  
/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的号码
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchEntryNumber(MMI_STRING_T *number_str, //OUT
									  //uint8 *num_type, //OUT
									  //uint16 max_number_len,//IN  
									  uint32 index//IN
									  );

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的卡
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICL_GetSearchEntrySim(uint32 index);

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的通话类型
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMIAPICL_GetSearchEntryCallType(uint32 index);

/*****************************************************************************/
//  Description : 返回搜素到的通话类型
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchCallType(CC_CALL_MODE_E *call_mode_ptr, BOOLEAN *is_emergency_call_ptr, uint32 index);

/*****************************************************************************/
// 	Description : 设置当前需要更新的WINHANDLE，当通话记录发生变化是，会向此WIN发FullPaint
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SetUpdatedWinHandle(MMI_HANDLE_T  updated_win_handle,BOOLEAN isNeedUpdate);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:rong.gu
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_CreateAllLogChildWin(MMI_WIN_ID_T	parent_win_id);

/*****************************************************************************/
// 	Description : create calllog wintab for sms/mms to select receiver
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreateAllLogSelectChildWin(MMI_WIN_ID_T parent_win_id);

/*****************************************************************************/
// 	Description : 创建联系人的通话记录子窗口
//	Global resource dependence : none
//  Author:sam.hua
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_CreatePBDetailLogChildWin(MMI_WIN_ID_T	parent_win_id);
#endif
#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_OpenGprsFlowRateWin(void);

/*****************************************************************************/
// 	Description : to close the gprs flow rate window
//	Global resource dependence : none
//  Author: fuyan.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_CloseGprsFlowRateWin(void);
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : 本地搜索中通话记录搜索接口
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_LocalSearch(const MMI_STRING_T *key_word_str, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num);
#endif
/*****************************************************************************/
// 	Description : 本地搜索退出，通话记录模块释放模块内部资源
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_FreeLocalSearchResource(void);
#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : Initial Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_InitCallInfo(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Initial Call Info Record NV
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_InitCallInfoRecord(void);

/*****************************************************************************/
// 	Description : Reset Call Info Record NV
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_ResetCallInfoRecord(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Set Is Call Info Ready
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SetIsCallInfoReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ready);

/*****************************************************************************/
// 	Description : Delete All LDN Call Info
//	Global resource dependence : none
//  Author:dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_DeleteAllLdnInfo(MMICL_CALL_TYPE_E call_type);

/*****************************************************************************/
// 	Description : Is current sim delete all
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetLdnIsDeleteAll(void);

/*****************************************************************************/
// 	Description : set Is current sim deleting all ldn
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SetLdnIsDeleteAll( BOOLEAN is_delete_all);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPICL_HandleLdnReadyInd(void* param);

#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
/*****************************************************************************/
// 	Description : 获取SIM_NAME WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSimNameContent(wchar* in_str,uint16 max_len,MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// 	Description : 获取Call type 的 ICON ID
//	Global resource dependence : 
//  Author:tao.xue
//	Note:  
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPICL_GetCallTypeImageID(BOOLEAN is_emergency_call, BOOLEAN is_video_call, MMICL_CALL_TYPE_E call_type);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获得列表控件中被选中的item数目
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/

PUBLIC uint16 MMIAPICL_GetSelectedItemCount(void);   //RETURN: 被选中的ITEM的总数

/*****************************************************************************/
//  Description :if the selected item is valid
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SelectedListIsValid(uint16 index);

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item 
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC void MMIAPICL_GetSelectPhonenum(
                            MMIPB_BCD_NUMBER_T *bcd_number ,//out
                            uint16 sel_item_index
                            );

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
                            );
#endif
/*****************************************************************************/
// 	Description : Clear call log
//	Global resource dependence : 
//  Author:Jiaoyou.wu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_ClearCalllogData(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*****************************************************************************/
//  Description: Open cl list window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCL_ListWin_Enter(void);

/*****************************************************************************/
//  Description : Close cl list window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCL_ListWin_Exit(void);

/*****************************************************************************/
//  Description : open no call log tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCL_EmptyList_NoteWin(void);
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
                                               uint16    max_tele_len);//!!!!!!!!!可显示的号码的最大长度 + 2(/0 字符串结束符号))   //RETURN: 被选中的ITEM的总数

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
                                                       MMIPB_BCD_NUMBER_T* bcd_num);

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
                                                          BOOLEAN is_display_year);
#endif
