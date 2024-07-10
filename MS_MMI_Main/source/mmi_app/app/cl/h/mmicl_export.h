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

//call log ������
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

//�绰����Ľṹ
typedef struct
{
    BOOLEAN             is_num_present;             //�绰�����Ƿ����
    MN_CALLED_NUMBER_T  number;
} MMICL_CALL_NUMBER_T;

 //�洢��NV��ĳ��ͨ����Ӧ�����֣�����ΪMMICL_NAME_MAX_LEN����С��CC_MAX_PBNAME_LEN
typedef struct _MMICL_NAME_T
{
//ͨ����¼��nv�б������ҽ�����������
//TRUE,��adn/fdn/sdn�����ĺ��룬��ʾ������
//FALSE,��pb/cl/dial�Ⱥ����ĺ��룬��ʾ��PB���ҵ�����������ͷ��
    BOOLEAN is_use_nv_name;
    uint16   wstr_len;                   
    wchar   wstr[MMICL_NAME_MAX_LEN + 1];   
} MMICL_NAME_T;

//call����Ϣ�������绰�����ʱ����Ϣ
typedef struct
{
#ifdef MMI_CSP_SUPPORT
    uint8 line;  //the call from line1 or line2
#endif
    BOOLEAN is_emergency_call; // �Ƿ��ǽ����������
#ifdef BT_DIALER_SUPPORT
    BOOLEAN is_bt_dail;    //�Ƿ���������
#endif
#if defined(CL_SAVE_NAME_SUPPORT)
    MMICL_NAME_T name;
#endif
    MN_DUAL_SYS_E       dual_sys;
    MMICL_CALL_NUMBER_T call_num;   //call number
    CC_CALL_TIME_T      call_time;  //callʱ��
    CC_CALL_MODE_E		call_type;	//@xiongxiaoyan 20060406 add    
} MMICL_CALL_INFO_T;

//�洢��NV���йص绰����Ϣ
typedef struct
{
    MN_DUAL_SYS_E		dual_sys;
#ifdef MMI_LDN_SUPPORT
    BOOLEAN is_sim_record;
#endif
    BOOLEAN             is_num_present;                 //�绰�����Ƿ����
    uint8               number_len;
    uint8               number[MMIPB_BCD_NUMBER_MAX_LEN];   //the dialling number
    MN_NUMBER_TYPE_E    number_type;
    MN_NUMBER_PLAN_E    number_plan;
#ifdef CALL_TIMES_SUPPORT
    uint32              call_start_time[MMICL_MAX_CALL_TIMES];                //call��ʼʱ��
    uint32              call_duration_time[MMICL_MAX_CALL_TIMES];             //@Lin.Lin, call����ʱ��, 2005-4-4
#else
    uint32              call_start_time;                //call��ʼʱ��
    uint32              call_duration_time;             //@Lin.Lin, call����ʱ��, 2005-4-4
#endif    
    CC_CALL_MODE_E		isvideocall;	//@zhaohui add,is VT call or normal call 
    BOOLEAN is_emergency_call; //�Ƿ��ǽ����������
#ifdef CALL_TIMES_SUPPORT
    uint16                   call_times;
#endif
#if defined(CL_SAVE_NAME_SUPPORT)
    MMICL_NAME_T    name;  //ͨ��ʱ��ʾ������(total size in NV:44*80 Bytes)
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
                          MMICL_CALL_TYPE_E     call_type,  //call ����
                          MMICL_CALL_INFO_T     call_info   //call ��Ϣ
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
//  Description :����vcard buffer data  
//  Parameter: data_ptr[in]: ���ڱ���UTF-8�����vCard��Ŀ���ݣ��������vCard2.1�淶�Ĺ涨��
//			  data_len[in]: ����vCard��¼�Ĵ�С
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
// Note: ֹͣ��ʱ��ʱ��
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
//  Description : ����������ַ������Һ����а����ô��ļ�¼
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: �ҵ��ĺ���ĸ���
/*****************************************************************************/
PUBLIC uint32 MMIAPICL_SearchList(MMI_STRING_T *search_str);

/*****************************************************************************/
//  Description : �ⲿ�����˳������ⲿ�����˳��ǣ�����ӿ�һ��Ҫ����
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIAPICL_SearchExit(void);

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼������
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
//  Description : �������ص��ĵ�index����0��ʼ������¼�ĺ���
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchEntryCalledNumber(MN_CALLED_NUMBER_T *called_num, //OUT
									  uint32 index//IN
									  );
									  
/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼�ĺ���
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
//  Description : �������ص��ĵ�index����0��ʼ������¼�Ŀ�
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICL_GetSearchEntrySim(uint32 index);

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼��ͨ������
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MMICL_CALL_TYPE_E MMIAPICL_GetSearchEntryCallType(uint32 index);

/*****************************************************************************/
//  Description : �������ص���ͨ������
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSearchCallType(CC_CALL_MODE_E *call_mode_ptr, BOOLEAN *is_emergency_call_ptr, uint32 index);

/*****************************************************************************/
// 	Description : ���õ�ǰ��Ҫ���µ�WINHANDLE����ͨ����¼�����仯�ǣ������WIN��FullPaint
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
// 	Description : ������ϵ�˵�ͨ����¼�Ӵ���
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
// 	Description : ����������ͨ����¼�����ӿ�
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICL_LocalSearch(const MMI_STRING_T *key_word_str, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num);
#endif
/*****************************************************************************/
// 	Description : ���������˳���ͨ����¼ģ���ͷ�ģ���ڲ���Դ
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
// 	Description : ��ȡSIM_NAME WSTRING
//	Global resource dependence : 
//  Author:Sam.hua
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetSimNameContent(wchar* in_str,uint16 max_len,MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// 	Description : ��ȡCall type �� ICON ID
//	Global resource dependence : 
//  Author:tao.xue
//	Note:  
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPICL_GetCallTypeImageID(BOOLEAN is_emergency_call, BOOLEAN is_video_call, MMICL_CALL_TYPE_E call_type);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : ����б�ؼ��б�ѡ�е�item��Ŀ
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: ��SMS, MMS������ģ����á�
/*****************************************************************************/

PUBLIC uint16 MMIAPICL_GetSelectedItemCount(void);   //RETURN: ��ѡ�е�ITEM������

/*****************************************************************************/
//  Description :if the selected item is valid
//  Global resource dependence : none
//  Author: jixin.xu
//  Note: ��SMS, MMS������ģ����á�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SelectedListIsValid(uint16 index);

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item 
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: ��SMS, MMS������ģ����á�
/*****************************************************************************/
PUBLIC void MMIAPICL_GetSelectPhonenum(
                            MMIPB_BCD_NUMBER_T *bcd_number ,//out
                            uint16 sel_item_index
                            );

/*****************************************************************************/
// 	Description : get bcd phone number of selected loglist item
//	Global resource dependence : 
//  Author:jixin.xu
//	Note: ��SMS, MMS������ģ����á�
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
//  Description : �����ʾ���õĵ绰����
//  Global resource dependence : none
//  [IN]:cl_nv_info,nv�洢�ĵ���calllog��Ϣ
//  [OUT]:tele_num,����ʾ�ĵ绰���룬ʹ�õ�Ascii
//  [IN]:����ʾ�ĵ绰�������󳤶�
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICL_GenDispNumber(const MMICL_CALL_BCDINFO_T* cl_nv_info,
                                               uint8     *tele_num,
                                               uint16    max_tele_len);//!!!!!!!!!����ʾ�ĺ������󳤶� + 2(/0 �ַ�����������))   //RETURN: ��ѡ�е�ITEM������

/*****************************************************************************/
//  Description : ���BCD����
//  Global resource dependence : none
//  [IN]:cl_nv_info,nv�洢�ĵ���calllog��Ϣ
//  [OUT]:tele_num,����ʾ�ĵ绰���룬ʹ�õ�Ascii
//  [IN]:����ʾ�ĵ绰�������󳤶�
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_GetBCDNumber(const MMICL_CALL_BCDINFO_T* cl_nv_info,
                                                       MMIPB_BCD_NUMBER_T* bcd_num);

/*****************************************************************************/
//  Description : ��ȡʱ��������ʾ��WSTRING
//	Global resource dependence :
//  [IN/OUT]:time_data_str,ʱ��������ʾ�ִ�
//  [IN]:max_len,������󳤶�
//  [IN]:callinfo��nv�洢�ĵ���calllog��Ϣ
//  [IN]:call_time_index������ڼ�������
//  [IN]:symbol��������ʾ�ļ����
//  [IN]:is_display_year���Ƿ���ʾ��
//  [RETURN]:�����ִ�����
//  Author:feiyue.ji
//	Note:  �������ʱ�䣬������������
/*****************************************************************************/
PUBLIC uint16 MMIAPICL_GenDispTimeAndDate(const MMICL_CALL_BCDINFO_T *callinfo,
                                                          wchar* time_data_str,
                                                          uint16 str_max_len,
                                                          uint16 call_time_index,
                                                          char symbol, //in
                                                          BOOLEAN is_display_year);
#endif
