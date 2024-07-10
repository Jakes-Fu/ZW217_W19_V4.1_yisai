/*****************************************************************************
** File Name:      mmischedule_export.h                                                
** Author:         liqing.peng   
** Date:           2006/07/04
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        * 
** Description:    This file is used to implement schedule function          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/06/04     liqing.peng      Create                                    *
******************************************************************************/


/*---------------------------------------------------------------------------*/
/*                          INCLUDE FILES                                    */
/*---------------------------------------------------------------------------*/
#include "mmiacc_event.h"
#include "mmicom_time.h"
#ifdef MMI_VCALENDAR_SUPPORT
#include "mmifilearray_export.h"
#endif

#ifndef _MMISCHEDULE_H_
#define _MMISCHEDULE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE DEFINITION                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    CMP_EQUAL =0,
    CMP_FIRST_SMALL,
    CMP_SECOND_SMALL
}ALM_CMP_E;

typedef struct  
{
    MMI_WIN_ID_T		    win_id;
	uint32					index;
    MMIACC_SMART_EVENT_T event;
}MMISCH_DETAIL_WIN_PARAM_T;

typedef struct  
{
    uint16                  year;
    uint8                   month;
    uint8                   day;
    MMI_WIN_ID_T            win_id;
    uint16                  index;
    MMIACC_SMART_EVENT_T    event;
}MMISCH_LIST_WIN_PARAM_T;

typedef enum
{
	MMISCH_NEW_NAME,
	MMISCH_NEW_DATE,
	MMISCH_NEW_TIME,
	MMISCH_NEW_RING,
	MMISCH_NEW_FREQ,
	MMISCH_NEW_FREQ_WEEK,
	MMISCH_NEW_DEADLINE,
	MMISCH_NEW_MAX,
}MMISCH_NEW_ITEM_T;

typedef enum
{
	MMISCH_MODE_ONCE,
	MMISCH_MODE_EVERYDAY,
	MMISCH_MODE_EVERYWEEK,
	MMISCH_MODE_EVERYMONTH,
	MMISCH_MODE_EVERYYEAR,	
	MMISCH_MODE_AWAKE_NO,
	MMISCH_MODE_MAX_TYPE    
}MMISCH_MODE_TYPE_E;

#ifdef MMI_VCALENDAR_SUPPORT
// ��¼����ĸ�����λ����Ϣ
typedef struct _MMISCH_OPERATE_RECORDS_INFO_T
{
    uint16	    total_count;    // ��Ҫ���������
    uint16      done_count;     // �Ѿ�����ĸ���
	uint16      failed_count;   //ʧ�ܵĸ���
    //uint16	    event_id;       // Ҫ���Ƶļ�¼ID     
	//uint32      ctrl_list_id;
	uint16      event_id[MMIACC_SCH_NUM];
} MMISCH_OPERATE_RECORDS_INFO_T;
#endif

#ifdef CALENDAR_SUPPORT
/*---------------------------------------------------------------------------*/
/*                          EXTERNAL VARIABLE DECLARE                        */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init a schedule record
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISCH_InitNewSch(
                       uint16                   year,
                       uint8                    month,
                       uint8                    day,
                       MMIACC_SMART_EVENT_T     *event_ptr
                       );

/*****************************************************************************/
// 	Description : check whether the date has schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsOneDateHasSche(
                                uint16      year,
                                uint8       month,
                                uint8       day
                                );

/*****************************************************************************/
// 	Description : get one date's schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_GetOneDateSche(
                            uint16              year,
                            uint8               month,
                            uint8               day,
                            uint16              *sche_num_ptr,
                            MMI_IMAGE_ID_T      *sche_icon_1_ptr,
                            MMI_IMAGE_ID_T      *sche_icon_2_ptr
                            );


/*****************************************************************************/
// 	Description : open schedule view list win
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenViewListWin(ADD_DATA win_param);

/*****************************************************************************/
// 	Description : open schedule view list all win
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenViewListAllWin(ADD_DATA win_param);

/*****************************************************************************/
// 	Description : open schedule view detail win
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenEditWin(ADD_DATA win_param);

/*****************************************************************************/
// 	Description : To get all schedule number
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_GetAllSchNumber(void);

/*****************************************************************************/
// 	Description : To get today's schedule number
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_GetTodaySchNumber(SCI_DATE_T *date);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISCH_GetAllUnreadSchNum(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_GetFirstUnreadSch(MMIACC_SMART_EVENT_T * event_ptr);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISCH_GetAllUnreadAlarmNum(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_GetFirstUnreadAlarm(MMIACC_SMART_EVENT_T * event_ptr);

/*****************************************************************************/
// 	Description : get one date's schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_GetInvalidSchByDate(SCI_DATE_T *date);

/*****************************************************************************/
// 	Description : to open today's schedule event for idle use
//	Global resource dependence : 
//  Author:  zhaohui
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISCH_OpenEventToday(uint16* event_id_ptr);

/*****************************************************************************/
// 	Description : if today's event is all past
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsAllPastEventToday(uint16* event_id_ptr);

/*****************************************************************************/
// 	Description : get if start_date <= cur_date <= iend_date
//	Global resource dependence : 
//  Author:  zhaohui
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsInDateRange(     
				SCI_DATE_T   start_date, 
                          SCI_DATE_T   end_date,
                          SCI_DATE_T   cur_date
                         );
/*****************************************************************************/
// 	Description : �����Ƿ����ճ�
//	Global resource dependence : 
//  Author:applezhang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsEventToday(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISCH_AddUnreadEvent(uint16 event_id);
#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISCH_DeleteUnreadEvent(uint16 event_id);
#endif
/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC uint32 MMIAPISCH_GetTotalTimesOfUnreadEvent(void);

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenUnreadEventWin(void);

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC MMISET_DATE_T* MMIAPISCH_GetAllListCurData(void);

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC void MMIAPISCH_SetAllListCurData(uint8 date_day, uint8  date_month,uint16  date_year);

/*****************************************************************************/
// 	Description : open child schedule view list win
//	Global resource dependence : none
//  Author: maryxiao
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenChildViewAllListWin(MMI_WIN_ID_T win_id , ADD_DATA win_param);

#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : ����VCalendar�ļ�,�����浽PB��
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISCH_ReadVCalendarData(uint8  *data_ptr, uint32 data_size);

/*****************************************************************************/
// Description : ����VCalendar�ļ�,�����浽PB��
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISCH_ReadVcalendarFile(uint16  *name_ptr);

/*****************************************************************************/
//  Description : notify vcalendar saved
//  Global resource dependence : g_ready_times
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_VCalendarSavedNotify(BOOLEAN is_ok);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_SaveVcalendar(const wchar *path_name_ptr);

/*****************************************************************************/
//  Description : notify vcalendar saved
//  Global resource dependence : g_ready_times
//  Author: 
//RETURN: void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_VCalendarSavedNotify(BOOLEAN is_ok);

/*****************************************************************************/
// 	Description : load next vcard
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
//PUBLIC void  MMISCH_LoadNextVcalendar(void);

/*****************************************************************************/
// Description : ����VCard�ļ�,�����浽PB��
// Global resource dependence :
// Author:Jesse.Yu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISCH_OpenVCalendarFile(uint16  *name_ptr);

/*****************************************************************************/
// Description : �ж�vcalendar���뵼��
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISCH_InterruptVCalendar(void);

/*****************************************************************************/
// Description : MMIAPISCH_CreateFileArray
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC FILEARRAY  MMIAPISCH_CreateFileArray(void);

/*****************************************************************************/
//  Description : MMIAPISCH_LoadVcalendar
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_LoadVcalendar(FILEARRAY   file_array);

/*****************************************************************************/
//  Description : notify vcalendar saved
//  Global resource dependence : g_ready_times
//  Author: 
//RETURN: void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_DelSchNotify(void);

#endif
/*****************************************************************************/
//  Description : MMIAPIACCSCH_AppendAllUserListBoxItem ���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
//PUBLIC void  MMIAPIACCSCH_AppendAllUserListBoxItem(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_widget_call);
/*****************************************************************************/
//  Description : MMIAPIACCSCH_IsHaveEvent ���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIACCSCH_IsHaveEvent(void);
/*****************************************************************************/
//  Description : MMIAPIACCSCH_AppendAllUserListBoxItemData ���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
/*PUBLIC void MMIAPIACCSCH_AppendAllUserListBoxItemData(
                                                    MMI_CTRL_ID_T    ctrl_id ,  
                                                    uint16 list_index
                                                    );*/
/*****************************************************************************/
//  Description : MMIAPIACCSCH_GetEventInfoByIndex ���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: ����list��ǰ��index���event����Ϣ
/*****************************************************************************/
//PUBLIC void MMIAPIACCSCH_GetEventInfoByIndex(uint16 cur_index,MMIACC_SMART_EVENT_T *event_ptr);
/*****************************************************************************/
//  Description : MMIAPIACCSCH_GetEventInfoByIndex ���ⲿʹ��
//  Global resource dependence : 
//  Author:juan.wu
//  Note: ����list��ǰ��index���event����Ϣ
/*****************************************************************************/
//PUBLIC uint16 MMIAPIACCSCH_GetEventEntryIdByIndex(uint16 cur_index);
#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : һ����ԭע�ắ��
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_ikeybackup_register(void);
#endif

/*****************************************************************************/
// 	Description : open event list
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_OpenEventList(
                            BOOLEAN         is_all,
                            uint16          year,
                            uint8           month,
                            uint8           day
                            );

/*****************************************************************************/
// 	Description : CompareDateTime
//         compare a time to another , find smaller(earlier)
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_CompareTime(
                         SCI_TIME_T   first, 
                         SCI_TIME_T   second
                         );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: ����list��ǰ��index���event����Ϣ
/*****************************************************************************/
PUBLIC MMIACC_SCH_LIST_T* MMIAPISCH_GetUserList(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenDetailWin(ADD_DATA detail_win_param );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32  MMIAPISCH_GetNextActiveSchEventId(void);

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				      *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMISCH_InitNewSch  MMIAPISCH_InitNewSch
#define MMISCH_IsOneDateHasSche  MMIAPISCH_IsOneDateHasSche
#define MMISCH_GetOneDateSche  MMIAPISCH_GetOneDateSche
#define MMISCH_OpenViewListWin  MMIAPISCH_OpenViewListWin
#define MMISCH_OpenViewListAllWin  MMIAPISCH_OpenViewListAllWin
#define MMISCH_GetAllSchNumber  MMIAPISCH_GetAllSchNumber
#define MMISCH_GetTodaySchNumber  MMIAPISCH_GetTodaySchNumber
#define MMISCH_GetInvalidSchByDate  MMIAPISCH_GetInvalidSchByDate
#define MMISCH_OpenEventToday  MMIAPISCH_OpenEventToday
#define MMISCH_IsAllPastEventToday  MMIAPISCH_IsAllPastEventToday
#define MMISCH_AddUnreadEvent  MMIAPISCH_AddUnreadEvent
#ifdef CALENDAR_SUPPORT
#define MMISCH_DeleteUnreadEvent  MMIAPISCH_DeleteUnreadEvent
#endif
#define MMISCH_GetTotalTimesOfUnreadEvent  MMIAPISCH_GetTotalTimesOfUnreadEvent
#define MMISCH_OpenUnreadEventWin  MMIAPISCH_OpenUnreadEventWin
#define MMISCH_GetAllListCurData  MMIAPISCH_GetAllListCurData
#define MMISCH_SetAllListCurData  MMIAPISCH_SetAllListCurData
#define MMISCH_OpenChildViewAllListWin  MMIAPISCH_OpenChildViewAllListWin
#ifdef MMI_VCALENDAR_SUPPORT
#define MMISCH_ReadVCalendarData  MMIAPISCH_ReadVCalendarData
#define MMISCH_ReadVcalendarFile  MMIAPISCH_ReadVcalendarFile
#define MMISCH_VCalendarSavedNotify  MMIAPISCH_OpenVCalendarFile
#define MMISCH_InterruptVCalendar  MMIAPISCH_InterruptVCalendar
#define MMISCH_CreateFileArray  MMIAPISCH_CreateFileArray
#define MMISCH_LoadVcalendar  MMIAPISCH_LoadVcalendar
#define MMISCH_DelSchNotify  MMIAPISCH_DelSchNotify
#endif
//#define MMIACCSCH_AppendAllUserListBoxItem  MMIAPIACCSCH_AppendAllUserListBoxItem
//#define MMIACCSCH_GetEventInfoByIndex  MMIAPIACCSCH_GetEventInfoByIndex
//#define MMIACCSCH_GetEventEntryIdByIndex  MMIAPIACCSCH_GetEventEntryIdByIndex
#ifdef IKEYBACKUP_SUPPORT
#define MMISCH_ikeybackup_register  MMIAPISCH_ikeybackup_register	
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif // c++
#endif
#endif  // _MMISCHEDULE_H_
