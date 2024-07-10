/****************************************************************************
** File Name:      mmicl_internal.h                                        *
** Author:                                                                 *
** Date:           09/2006                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the mmi cc internal api   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 9/2006        bruce.chi         Create
****************************************************************************/
#ifndef  _MMICL_INTERNAL_H_
#define  _MMICL_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmicl_export.h"
#include "mmipb_export.h"
#include "mmi_custom_define.h"
#include "mmicc_id.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define PHONE_NUM_MAX_LEN        (MMIPB_BCD_NUMBER_MAX_LEN << 1)
#define MMICL_MAX_CALL_TIMES_LEN  8
#define MMICL_LDN_NUM_MAX  10
#ifdef MMI_LDN_SUPPORT
#define MMICL_RECORD_TOTAL_NUM  (MMICL_RECORD_MAX_NUM + (MMICL_LDN_NUM_MAX * MMI_DUAL_SYS_MAX))
#else
#define MMICL_RECORD_TOTAL_NUM  (MMICL_RECORD_MAX_NUM)
#endif

#define MMICL_RECORD_NUM_PER_NVITEM  (10)
#define MMICL_RECORD_NVITEM_COUNT    (MMICL_RECORD_TOTAL_NUM/MMICL_RECORD_NUM_PER_NVITEM)

#define MMICL_MAX_MATCH_ITEM    MMICL_RECORD_MAX_NUM


#ifdef CALL_TIMES_SUPPORT
#define MMICL_CALLTIME_SUPPORT_FLAG         1
#else
#define MMICL_CALLTIME_SUPPORT_FLAG         0
#endif

#ifdef MMI_LDN_SUPPORT
#define MMICL_LDN_SUPPORT_FLAG         1
#else
#define MMICL_LDN_SUPPORT_FLAG         0
#endif

#if defined(CL_SAVE_NAME_SUPPORT)
#define MMICL_SAVENAME_SUPPORT_FLAG         1
#else
#define MMICL_SAVENAME_SUPPORT_FLAG         0
#endif

#define MMICL_FUNC_SUPPORT_FLAG ((MMICL_CALLTIME_SUPPORT_FLAG<<2)|(MMICL_LDN_SUPPORT_FLAG<<1)|(MMICL_SAVENAME_SUPPORT_FLAG))
#define MMICL_NV_VER_NO         0x02

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

 //存储在NV中有关电话信息的数组
typedef struct
{
    MN_DUAL_SYS_E		dual_sys;
    uint8                   record_num;
#ifdef MMI_LDN_SUPPORT
    uint8                   sim_record_num;//may oci/ici/ldn
#endif
    MMICL_CALL_BCDINFO_T    call_bcdinfo[MMICL_RECORD_TOTAL_NUM];
} MMICL_CALL_ARRAY_BCDINFO_T;

typedef struct
{
    uint32          support_flag;
    uint32          nv_verno;
    MMICL_CALL_ARRAY_BCDINFO_T    call_array;
} MMICL_CALL_ARRAY_BCDINFO_BACKUP_T;



typedef struct
{
    MN_DUAL_SYS_E		dual_sys;
    uint8                   record_num;
#ifdef MMI_LDN_SUPPORT
    uint8                   sim_record_num;//may oci/ici/ldn
#endif
} MMICL_CALL_ARRAY_BCDINFO_HEADER_T;

typedef struct
{
    MMICL_CALL_BCDINFO_T    call_bcdinfo[MMICL_RECORD_NUM_PER_NVITEM];
} MMICL_CALL_ARRAY_BCDINFO_NVITEM_T;


typedef struct
{
    uint8       match_num;
    MMICL_CALL_BCDINFO_T    call_info[MMICL_MAX_MATCH_ITEM];
}MMICL_MATCH_TOTAL_INFO_T;

//只对已经存在电话记录有效
//增加的新纪录的原始位置是在当前阴影的item之前，之后，或者相等
typedef enum
{
    MMICL_NEW_RECORD_NONE_CUR_ITEM,
    MMICL_NEW_RECORD_BEFORE_CUR_ITEM,
    MMICL_NEW_RECORD_AFTER_CUR_ITEM,
    MMICL_NEW_RECORD_EQUAL_CUR_ITEM
} MMICL_NEW_RECORD_POS_TYPE_E;

typedef struct
{
    MN_DUAL_SYS_E   dual_sys;	//标示当前的使用SIM卡ID
#ifdef MMI_CSP_SUPPORT
    uint32 line1_call_time; //line1 call time
    uint32 line2_call_time; //line2 call time
#else //MMI_CSP_SUPPORT
    uint32          call_time;	//????
#endif //MMI_CSP_SUPPORT    
}CC_CALL_TIME_COUNT_T;		    //通话持续时间

typedef struct
{
    BOOLEAN last_call_time_valid;
    BOOLEAN all_calls_time_valid;
    BOOLEAN received_calls_time_valid;
    BOOLEAN dialed_calls_time_valid;
    CC_CALL_TIME_COUNT_T  last_call_time[MMI_DUAL_SYS_MAX];
    CC_CALL_TIME_COUNT_T  all_calls_time[MMI_DUAL_SYS_MAX];
    CC_CALL_TIME_COUNT_T  received_calls_time[MMI_DUAL_SYS_MAX];
    CC_CALL_TIME_COUNT_T  dialed_calls_time[MMI_DUAL_SYS_MAX];
}CC_CALL_TIME_CONTEXT_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : delete the appointed record of call 
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
uint8 MMICL_DeleteAppointedCallInfo(//剩下的记录的个数
                                    MMICL_CALL_TYPE_E    call_type,  //call 类型
                                    uint8                pos         //in
                                    );

/*****************************************************************************/
// 	Description : to handle enter missed,received and dialed calls menu
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
void MMICL_HandleEnterCallsMenu(
                                MMICL_CALL_TYPE_E call_type
                                );

/*****************************************************************************/
// 	Description : get selected cl item number
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
void  MMICL_COSTS_HandleMainMenuMsg(
                                    MMI_MENU_ID_T menu_id
                                    );

/*****************************************************************************/
// 	Description : get current selected call typ
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMICL_GetCurCallType(void);

/*****************************************************************************/
// 	Description : read call info nv
//	Global resource dependence : none
//  Author: rong.gu
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MMICL_ReadAllNV(MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr);

/*****************************************************************************/
//  Description : read last missed call detail
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_ReadLastMissedCall(MMICL_CALL_BCDINFO_T *   call_bcdinfo_ptr);

/*****************************************************************************/
// 	Description : read call info nv
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MMICL_ReadNV(MMICL_CALL_TYPE_E call_type, MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr);

/*****************************************************************************/
// 	Description : write call info nv
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
void MMICL_WriteNV(MMICL_CALL_TYPE_E call_type, MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr);

/*****************************************************************************/
// 	Description : to set call log list item
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMICL_SetLogListItem(void);

/*****************************************************************************/
// 	Description : get current log list control id
//	Global resource dependence : 
//  Author: guanyong.zhang
//	Note: 
/*****************************************************************************/
PUBLIC CC_CONTROL_ID_E MMICL_GetLogListCtrlId(MMICL_CALL_TYPE_E call_type);

/*****************************************************************************/
// 	Description : set dafault call times
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_SetDefaultCallTime(void);

/*****************************************************************************/
// 	Description : set dafault call times by sim
//	Global resource dependence : none
//    Author: tao.xue
//	Note:
/*****************************************************************************/
void MMIAPICL_SetDefaultCallTimeToNVBySim(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set dafault call times
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_SetDefaultCallTimeToNV(void);

/*****************************************************************************/
// 	Description : init the calltime context
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_InitCallTimeContext(void);

#ifdef  MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : store the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreLastCallTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetLastCallTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : store all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreAllCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetAllCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreReceivedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetReceivedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreDialedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetDialedCallsTime(uint8 line, CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);
#else //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : store the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreLastCallTime(CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get the last call time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetLastCallTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : store all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreAllCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get all calls time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetAllCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreReceivedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetReceivedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : store received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_StoreDialedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time);

/*****************************************************************************/
// 	Description : get received callsl time
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
void MMIAPICL_GetDialedCallsTime(CC_CALL_TIME_COUNT_T* p_cc_time, MN_DUAL_SYS_E dual_sys);
#endif //MMI_CSP_SUPPORT

/*****************************************************************************/
//  Description : 获得CL中某条纪录对应的姓名，get name by cl record, the name is from pb or the record itself
//  Global resource dependence : none
//  Author: tao.xue
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetNameByClInfo( MMICL_CALL_BCDINFO_T cl_nv_info, MMI_STRING_T* name_str, BOOLEAN is_use_default_name);

/*****************************************************************************/
// 	Description : to get number of all records
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICL_GetAllRecordsNum(void);

/*****************************************************************************/
// 	Description : get record info
//	Global resource dependence : 
//  Author:bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_GetRecordInfo(MMICL_CALL_TYPE_E e_call_type, MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr);

/*****************************************************************************/
// 	Description : Read Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
PUBLIC BOOLEAN MMICL_ReadCallInfo(PHONEBOOK_STORAGE_E storage, MN_DUAL_SYS_E dual_sys, PHONEBOOK_ENTRY_T *entry_ptr, CALL_INFO_ENTRY_T *call_info_entry_ptr);
#else
PUBLIC BOOLEAN MMICL_ReadCallInfo(PHONEBOOK_STORAGE_E storage, MN_DUAL_SYS_E dual_sys, PHONEBOOK_ENTRY_T *entry_ptr);
#endif

/*****************************************************************************/
// 	Description : Update Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_UpdateCallInfo(MMICL_CALL_TYPE_E call_type, MN_DUAL_SYS_E dual_sys, MMICL_CALL_INFO_T *call_info_ptr);

/*****************************************************************************/
// 	Description : Delete Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_DeleteCallInfo(MMICL_CALL_TYPE_E call_type, MN_DUAL_SYS_E dual_sys, MMICL_CALL_INFO_T *call_info_ptr);

/*****************************************************************************/
// 	Description : Delete All Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_DeleteAllCallnfo(MMICL_CALL_TYPE_E call_type, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Is Call Info Ready
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_IsCallInfoReady(void);

/*****************************************************************************/
// 	Description : open window CL main menu window
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_OpenMainMenuWindowExt(void);

/*****************************************************************************/
//  Description : phonebook callback
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMICL_HandlePBEventFunc(MMIEVENT_PB_EVENT_E event);

#if defined(IKEYBACKUP_SUPPORT)
/*****************************************************************************/
//  Description : 1key backup call back function for register
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_BackupRegister(void);
#endif

/*****************************************************************************/
//  Description : Select SIM API of CL app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICL_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC  MMICLSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E    append_type,
                                        ADD_DATA                    add_data
                                        );

/*****************************************************************************/
// 	Description : 给已经设定的需要更新的窗口发FULL_PAINT消息，通知其更新数据
//	Global resource dependence : none
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_UpdateCalllogWin(BOOLEAN is_update);

/*****************************************************************************/
// 	Description : indication to other module that cl has updated
//	Global resource dependence : none
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_UpdateEventInd(void);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获得列表控件中被选中的item数目
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/

PUBLIC uint16 MMICL_GetSelectedItemCount(void);   //RETURN: 被选中的ITEM的总数

/*****************************************************************************/
//  Description : if the selected item is valid
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/

PUBLIC BOOLEAN MMICL_SelectedListIsValid(uint16 index);

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item 
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: 供SMS, MMS等其它模块调用。
/*****************************************************************************/
PUBLIC void MMICL_GetSelectPhonenum(
                            MMIPB_BCD_NUMBER_T*   bcd_number ,//out
                            uint16 sel_item_index
                            );

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
                            );
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
                                     );
/*****************************************************************************/
// 	Description : 将通话次数显示加到WSTRING的末尾
//	Global resource dependence : 
//  Author:Sam.hua
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_AddCallTimesStr(MMI_STRING_T *in_str_id,uint16 call_times,MMI_STRING_T* out_strid,uint16 max_len);

#endif


#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_InitPBAPDataList(void);

/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMICL_AddDataToCalllog(const char*  buff);

/*******************************************************************************/
//  Description :联系人同步，添加至通讯录
//  Parameter: contact_ptr[in]: 记录相关信息
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMICL_AddCalllogForPbap(MMICL_CALL_INFO_T *calllog_ptr, 
                                             MMICL_CALL_ARRAY_BCDINFO_T *arraycall_info_ptr
                                             );
#endif