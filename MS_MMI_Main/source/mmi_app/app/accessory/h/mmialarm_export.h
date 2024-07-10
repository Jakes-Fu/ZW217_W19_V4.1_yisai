/*****************************************************************************
** File Name:      mmialarm_export.h                                                
** Author:           taul.zhu   
** Date:            2004/09/04
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe DATA of alarm
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2004/09/04       taul zhu          Create                                 *
** 2006/08/10       liqing.peng      Modify -- code review
******************************************************************************/

#ifndef _MMI_ALARM_EXPORT_H_
#define _MMI_ALARM_EXPORT_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "window_parse.h"
#include "mmiacc_event.h"
#include "mmidisplay_data.h"
#include "mmialarm_service.h"
//#include "mmiset.h"
#include "mmiset_export.h"

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
#define RTC_DATE_STRING_LENGTH		  		10		// XXXX-XX-XX
#define RTC_TIME_STRING_LENGTH		  		8		// XX:XX:XX
#define MMIALM_TIME_STR_12HOURS_LEN	        MMISET_TIME_STR_12HOURS_LEN		//length of "%02d:%02d PM"
        
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


typedef struct EVENT_CURRENT_DATA_TAG
{
    uint16					id;			// current event ID
    MMIACC_SMART_EVENT_E	type;		// event type

    MMIACC_SMART_EVENT_T	event;		// event data buffer
}EVENT_CURRENT_DATA_T;

typedef BOOLEAN (*DRAWFUNC)(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect);
typedef struct ALARM_CLOCK_DATA
{
    MMI_IMAGE_ID_T			bg_img_id;			// current event ID
    DRAWFUNC	            DrawClockFunc;		// event type
}ALARM_CLOCK_DATA_T;

typedef uint8 MMIACC_RTC_DATE_T[RTC_DATE_STRING_LENGTH + 1];
typedef uint8 MMIACC_RTC_TIME_T[RTC_TIME_STRING_LENGTH + 1];


typedef BOOLEAN(*alarmClkSaveTimeFunc)(void);
/**--------------------------------------------------------------------------*
 **                         GLOBAL FUNCTIONS
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: modify one alarm event
//  Global resource dependence: none
//  Author: bin.wang
//  Note: 
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIALARM_ModifyOneAlarm( uint16 event_id, MMIACC_SMART_EVENT_FAST_TABLE_T* event_ptr );

/*****************************************************************************/
//  Discription: get one unused alarm event id
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIALARM_GetOneUnusedAlarm( uint16* event_id_ptr );

/*****************************************************************************/
//  Discription: turn off one alarm by event id
//  Global resource dependence: none 
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_CloseOneAlarm( uint16 event_id );

/*****************************************************************************/
//  Discription: delete one alarm by event id
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_DeleteOneAlarm( uint16 event_id );

/*****************************************************************************/
//  Discription: delete all alarm
//  Global resource dependence: none
//  Author: bin.wang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_DeleteAllAlarm();

/*****************************************************************************/
// 	Description : start alarm when interrupt of alarm is coming
//	Global resource dependence : 
//  Author:   taul.zhu
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_StartAlarm(
                                 MMIALMSVC_ID_T svc_id, 
                                 MMIALMSVC_EVENT_TYPE_E svc_type
                                 );

#ifdef FM_SUPPORT//CR132759
/*****************************************************************************/
//  Description : change float string to integer value
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:liyan.zhu	
//  Note: cr132759
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_ConvertFloatStrToInt(const MMI_STRING_T *str_ptr, int32 mul_scale);
/*****************************************************************************/
//  Description : 设置频率编辑窗口控件参数
//  Global resource dependence : 
//  Author:liyan.zhu	
//  Note: cr132759   
/*****************************************************************************/
PUBLIC void MMIAPIALM_SetFMEditCtrlParam(void);
/*****************************************************************************/
//  Description : 对外接口：打开ALARM_FM_EDITWIN_TAB
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:   cr132759 liyan.zhu modify local->public
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenFMEditWin(void* param_ptr);
#endif
/*****************************************************************************/
// 	Description : check to see if there is a delayed alarm and activate it
//	Global resource dependence : 
//  Author:        taul.zhu
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_CheckAndStartDelayedAlarm(void);
/*****************************************************************************/
// 	Description : 判断是否是一个有效频率值
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsValidFM(	const wchar *fre,     //in
                        	uint16       fre_len       //in, 双字节为单位
                       		);
/*****************************************************************************/
// 	Description : get current ring type of  two alarm: 0 and 1
//	Global resource dependence : 
//  Author:      taul.zhu
//	Note:  
/*****************************************************************************/
PUBLIC uint16 MMIAPIALM_GetRingId(
                        uint16 alarm_id
                        );

/*****************************************************************************/
// 	Description : intial Alarm module  
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_InitModule(void);

/*****************************************************************************/
// 	Description : return alarm expired win id  
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIALM_GetAlarmPromptWinId(void);

/*****************************************************************************/
// 	Description : open alarm main window
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenMainWin(void);

/*****************************************************************************/
// 	Description : open auto power on/off main window
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUTOPOWER_OpenMainWin(void);

/*****************************************************************************/
// 	Description : get first alarm ring, for setting use
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_GetFirstAlarmRing(
                              BOOLEAN       *is_fixed,
                              uint16        *ring_id,
                              uint16        *file_name_len,
                              wchar         *file_name_ptr
                              );

/*****************************************************************************/
// 	Description : if alarm alive win is opened
//	Global resource dependence : 
//  Author:        zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsAliveWinOpened(void);

/*****************************************************************************/
// 	Description : if alarm alive win is opened
//	Global resource dependence : 
//  Author:        zhaohui
//	Note: 
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPIALM_IsAliveWinOpened(void);//@zhaohui,counted time

/*****************************************************************************/
//  Description : Set alarm ring by file full path name.
//  Global resource dependence : 
//  Author: xingdong.li
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_SetAlarmRing(
                                     uint16 alarm_id,
                                     const wchar *full_path_ptr,
                                     uint16 full_path_len        //len in WORD
                                     );

/*****************************************************************************/
// 	Description : get the integrity event info by event_id
//	Global resource dependence : 
//  Author:        zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIALM_FormInteEventInfo(
                            uint16 event_id,//in
                            MMIACC_SMART_EVENT_T* integrity_event_ptr//out
                            );//形成完整的事件信息

/*****************************************************************************/
// Description : get ring info
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_GetRingInfo(uint16 event_id, MMIACC_SMART_EVENT_T *event_info_ptr);

/*****************************************************************************/
// 	Description : Save the integrity event info 
//	Global resource dependence : 
//  Author:        zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_SaveInteEventInfo(
                            uint16 event_id,//in
                            MMIACC_SMART_EVENT_T* integrity_event_ptr//in
                            );

/*****************************************************************************/
// 	Description : MMIAPIALM_CompareTwoEvent
//         compare two event, find smaller with date and time
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note: 比较的两个event都要包括完整的date和time  
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_CompareTwoEvent(
                    const MMIACC_SMART_EVENT_FAST_TABLE_T    *first_event_ptr,  
                    const MMIACC_SMART_EVENT_FAST_TABLE_T    *second_event_ptr 
                    );

/*****************************************************************************/
// 	Description : open alarm expired win according to the alarm type
//	Global resource dependence : 
//  Author:      lin.lin
//	Note:            
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenAlarmExpiredWin(void);

/*****************************************************************************/
// 	Description : get total times of the arrived alarm and the arrived calender 
//	Global resource dependence : 
//  Author:      jianhui.luo
//	Note:            
/*****************************************************************************/
PUBLIC uint32 MMIAPIALM_GetTotalTimesOfArrivedEvent(void);

/*****************************************************************************/
// 	Description : get event timer string
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALARM_GetEventTimeString(
                              const MMIACC_SMART_EVENT_FAST_TABLE_T* event_ptr,
                              BOOLEAN                     is_valid,
                              wchar*                      str_ptr,
                              uint16                      str_len
                              );
PUBLIC void  MMIAPIALARM_OpenEditAlarmWinbyEventid(uint16 event_id);

/*****************************************************************************/
// 	Description : 获取闹钟名称
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIAPIALM_GetAlarmName(uint16 index,MMI_STRING_T* str_ptr);

#ifdef MMI_AZAN_SUPPORT
/*****************************************************************************/
// Description : get event table
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T * MMIAPIALM_GetEventFastTab(void);

/*****************************************************************************/
// Description : find latest event
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_FindLatestEventToSet(void);
#endif

/*****************************************************************************/
// Description : MMIAPIALM_CheckandStartExpiredEvent
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_CheckandStartExpiredEvent(void);
/*****************************************************************************/
// Description : MMIAPIALM_IsAlarmActive
// Global resource dependence : 
// Author:  renyi.hu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsAlarmActive(void);

/*****************************************************************************/
// 	Description : SetNextAlarmTimeByCurrent
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:  根据event_ptr是否开启、类型(闹钟、自动开关机、日程)、
//         模式(一次、每天、每周等)、时间日期，
//         计算event_ptr下一次发生的时间
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_SetNextAlarmTimeByCurrent(
										MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr         //in & out
										);
//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC uint16 MMIAPIALARM_GetFreModeString(MMIACC_MODE_TYPE_E	fre_mode,uint8 fre,wchar* buff,uint16 buff_max_len);
//#endif

/*****************************************************************************/
//  Discription: MMIAPIALARM_GetALARMTimerID
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:获得ALARM TIMER的id，给日程widget使用
//
/*****************************************************************************/
PUBLIC uint8 MMIAPIALARM_GetALARMTimerID(void);

/*****************************************************************************/
//  Discription: MMIALARM_TreatVibrate
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:处理振动器，给日程widget使用
//
/*****************************************************************************/
//PUBLIC void MMIALARM_TreatVibrate(void);

/*****************************************************************************/
//  Discription: MMIAPIALARM_DealWithCancelOprate
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:处理关闭日程起闹的时间，同HandleEventAliveWinMsg中的MSG_APP_CANCEL要做的事情
//
/*****************************************************************************/
PUBLIC void MMIAPIALARM_DealWithCancelOprate(void);

/*****************************************************************************/
//  Discription: MMIAPIALARM_HandleEventAliveMSG
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:HandleEventAliveWinMsg的封装，供外部使用
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E   MMIAPIALARM_HandleEventAliveMSG(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param 
                                                     );


/*****************************************************************************/
//  Description : create clock window.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALARM_OpenAlmClockWin(SCI_TIME_T time, alarmClkSaveTimeFunc savetimefunc);


/*****************************************************************************/
// 	Description : 比较两个日期时间
//	Global resource dependence : 
//  Author:  xiaoqing.lu
//	Note:   
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_CompareDateAndTime(
                                SCI_DATE_T      first_date, 
                                SCI_TIME_T      first_time, 
                                SCI_DATE_T      second_date, 
                                SCI_TIME_T      second_time 
                                );

/*****************************************************************************/
// 	Description : 从wday后一天向后找到第一个匹配的，返回offset
//	Global resource dependence : 
//  Author:        xiaoqing.lu
//	Note:   
/*****************************************************************************/
PUBLIC uint8 MMIAPIALM_RoundWeek(
                              uint8 wday,
                              uint8 fre,
                              uint32 type
                              );

/*****************************************************************************/
// 	Description : increase current date by one
//	Global resource dependence : 
//  Author:        xiaoqinglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_IncreaseDateByOne(
                                     SCI_DATE_T  *pdate_ptr
                                     );
#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_DisplayAlmClock(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect, int32 angel);
/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALARM_DrawTime(SCI_TIME_T sys_time, GUI_POINT_T point);
/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALARM_DrawDate(SCI_DATE_T sys_date, GUI_POINT_T point);
#endif
/*****************************************************************************/
//  Description : Request alarm play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_ReqPlaySrv(void);
/*****************************************************************************/
//  Description : Get alarm play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAPIALARM_GetPlayHandle(void);
/*****************************************************************************/
//  Description : Free alarm play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIALARM_FreePlaySrv(void);

#ifdef ATEST_SUPPORT
/*****************************************************************************/
// Description : set alarm time
// Global resource dependence : 
// Author:lin.chen
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ATESTALM_SetAlarmTime(SCI_TIME_T  time_val);

/*****************************************************************************/
//  Description : get the Next valid Alarm Time
//  Global resource dependence : 
//  Author:  lin.chen
//  Note:   
/*****************************************************************************/
PUBLIC void ATESTALM_GetLatestAlarmTime(SCI_ALARM_T  *s_alm);
#endif	//ATEST_SUPPORT

/*****************************************************************************/
//  Discription: turn ON one alarm by event id
//  Global resource dependence: none
//  Author: qi.liu1
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIALARM_OpenOneAlarm( uint16 event_id );

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
// Description : open alarm dialog
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenAlarmDialogWin(void);
#endif

PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T *GetDelayEvent(uint16 event_id);
/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIALM_StartAlarm				                    MMIAPIALM_StartAlarm
#define MMIALM_CheckAndStartDelayedAlarm				MMIAPIALM_CheckAndStartDelayedAlarm
#define MMALM_IsValidFM				                    MMIAPIALM_IsValidFM
#define MMIALM_GetRingId				                    MMIAPIALM_GetRingId
#define MMIALM_InitModule				                    MMIAPIALM_InitModule
#define MMIALM_GetAlarmPromptWinId				        MMIAPIALM_GetAlarmPromptWinId
#define MMIALM_OpenMainWin				                MMIAPIALM_OpenMainWin
#define MMIAUTOPOWER_OpenMainWin				            MMIAPIAUTOPOWER_OpenMainWin
#define MMIALM_GetFirstAlarmRing				            MMIAPIALM_GetFirstAlarmRing
#define MMIALM_FormInteEventInfo				            MMIAPIALM_FormInteEventInfo
#define MMIALM_GetRingInfo				                MMIAPIALM_GetRingInfo
#define MMIALM_SaveInteEventInfo				            MMIAPIALM_SaveInteEventInfo
#define MMIALM_CompareTwoEvent				            MMIAPIALM_CompareTwoEvent
#define MMIALM_OpenAlarmExpiredWin				        MMIAPIALM_OpenAlarmExpiredWin
#define MMIALM_GetTotalTimesOfArrivedEvent				MMIAPIALM_GetTotalTimesOfArrivedEvent
#define MMIALARM_GetEventTimeString				        MMIAPIALARM_GetEventTimeString
#define MMIALARM_OpenEditAlarmWinbyEventid				MMIAPIALARM_OpenEditAlarmWinbyEventid
#define MMIALM_GetAlarmName				                MMIAPIALM_GetAlarmName
#ifdef MMI_AZAN_SUPPORT
#define MMIALM_GetEventFastTab				            MMIAPIALM_GetEventFastTab
#define MMIALM_FindLatestEventToSet				        MMIAPIALM_FindLatestEventToSet
#endif
#define MMIALM_CheckandStartExpiredEvent				MMIAPIALM_CheckandStartExpiredEvent
#define MMIALM_IsAlarmActive				                MMIAPIALM_IsAlarmActive
#define MMIALM_SetNextAlarmTimeByCurrent				MMIAPIALM_SetNextAlarmTimeByCurrent
#ifdef MMI_PDA_SUPPORT
#define MMIALARM_GetFreModeString				        MMIAPIALARM_GetFreModeString
#endif
#define MMIALARM_GetALARMTimerID				            MMIAPIALARM_GetALARMTimerID
#define MMIALARM_DealWithCancelOprate				    MMIAPIALARM_DealWithCancelOprate
#define MMIALARM_DealWithCancelOprate				    MMIAPIALARM_DealWithCancelOprate
#define MMIALARM_HandleEventAliveMSG				        MMIAPIALARM_HandleEventAliveMSG
#define MMIALARM_OpenAlmClockWin				            MMIAPIALARM_OpenAlmClockWin
#define MMIALM_CompareDateAndTime				        MMIAPIALM_CompareDateAndTime
#define MMIALM_RoundWeek				                    MMIAPIALM_RoundWeek
#define MMIALM_IncreaseDateByOne				            MMIAPIALM_IncreaseDateByOne

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  


