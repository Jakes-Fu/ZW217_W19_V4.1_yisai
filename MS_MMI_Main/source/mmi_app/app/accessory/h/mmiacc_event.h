/*****************************************************************************
** File Name:      mmiacc_event.h                                                
** Author:           taul.zhu   
** Date:            2004/09/25
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe DATA structure of EFS-related used by smart event
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2004/09/25       taul zhu          Create                                   *
******************************************************************************/


#ifndef _MMIACC_EVENT_H_
#define _MMIACC_EVENT_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmifmm_export.h"
#include "mmialarm_service.h"


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

#ifdef MMI_ALARM_MINI_LOW_COST
#define MMIACC_ALARM_NUM     				3		    // 最大闹钟数,修改该宏，注意修改NV.
#define ALM_ALARM_NUM                       3           // used for alarm list menu,不能大于MMIACC_ALARM_NUM
#else 

#ifdef ALARM_SUPPORT_LOW_MEMORY//打开该宏可以节省NV
//最大闹钟数
#define MMIACC_ALARM_NUM     				5		    // 最大闹钟数,修改该宏，注意修改NV.
#define ALM_ALARM_NUM                       3           // used for alarm list menu,不能大于MMIACC_ALARM_NUM
#else  //ALARM_SUPPORT_LOW_MEMORY
#define MMIACC_ALARM_NUM     				10		    // 最大闹钟数,修改该宏，注意修改NV.
#ifdef MMI_PDA_SUPPORT
    #define ALM_ALARM_NUM                   3       // used for alarm list menu,不能大于MMIACC_ALARM_NUM
#else
    #define ALM_ALARM_NUM                   3       // used for alarm list menu,不能大于MMIACC_ALARM_NUM
#endif //MMI_PDA_SUPPORT
#endif //ALARM_SUPPORT_LOW_MEMORY
#endif //MMI_ALARM_MINI_LOW_COST

#define MMIACC_DELAY_ALARM_NUM	    MMIACC_ALARM_NUM    // 延时闹钟数,对应每个闹钟,设置一个延时闹钟
#define ALM_AUTOPOWER_NUM		    2       //自动开机+自动关机 alarm数量
#define ALM_WIDGET_ALARM_NUM        3       // WIDGET 支持显示的个数

//最大日程表数
#ifndef CALENDAR_SUPPORT
    #define MMIACC_SCH_NUM  					0		    
#else
#ifdef MMI_ALARM_MINI_LOW_COST
    #define MMIACC_SCH_NUM  					16		    // 极致裁剪版：最大日程表数
#else
#ifdef CALENDAR_SUPPORT_LOW_MEMORY
    #define MMIACC_SCH_NUM  					20		    // 修改该宏，注意修改NV.
#else
    #define MMIACC_SCH_NUM  					100		    // 最大日程表数,修改该宏，注意修改NV.
#endif
#endif
#endif  //CALENDAR_SUPPORT

#ifdef MMI_AZAN_SUPPORT
#define MMIACC_AZAN_NUM                     (6) //Azan alarm
#else
#define MMIACC_AZAN_NUM                     (0)
#endif

#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    #define    FM_TIMER_RECORD_NUM     (1)     //FM定时录音        
#else
    #define    FM_TIMER_RECORD_NUM     (0)
#endif

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    #define    IM_SOGOU_NUM     (1)     //FM定时录音        
#else
    #define    IM_SOGOU_NUM     (0)
#endif
                                    //  alarm               delay alarm              auto power on/off        schedule        azan alarm    
#define MMIACC_EVENT_MAX_NUM        (MMIACC_ALARM_NUM + MMIACC_DELAY_ALARM_NUM + ALM_AUTOPOWER_NUM+ MMIACC_SCH_NUM + MMIACC_AZAN_NUM + FM_TIMER_RECORD_NUM+IM_SOGOU_NUM)

#define MMIACC_EVENT_NV_SCH_PART_NUM		(MMIACC_SCH_NUM/4)//sch的信息分成4块NV

#ifdef MMI_ALARM_MINI_LOW_COST
#define MMIACC_EVENT_TEXT_MAX_LEN	    20		    // 日程表的最大长度
#define MMIALM_FREQ_MAX_LEN             10
#else
#define MMIACC_EVENT_TEXT_MAX_LEN	    40		    // 日程表的最大长度
#define MMIALM_FREQ_MAX_LEN             20
#endif
#define MMIALM_NAME_MAX_LEN	            20
#define MMIALM_DAY_NUM              		7		    // 天数	
#define MMIALM_DELAY_MAX_NUM                2           // 每个延时闹钟最大延时次数

#define MMIALM_FM_MAX_LEN	5
#define MMIALM_FREQ_RANGE_LOW    875
#define MMIALM_FREQ_RANGE_HIGH    1080
#define MMIALM_CHANNEL_NAME_LEN         30
//#define MMIALARM_DRING_NAME_MAX_LEN 	    50	        // 下载铃声文件名和路径名, 未使用，目前使用MMIFILE_FULL_PATH_MAX_LEN替代
// define MASK of frequency
#define     MASK_B0         0x01
#define     MASK_B1         0x02
#define     MASK_B2         0x04
#define     MASK_B3         0x08
#define     MASK_B4         0x10    
#define     MASK_B5         0x20
#define     MASK_B6         0x40
#define     MASK_B7         0x80

//[edit repeat list]
#define     MASK_WEEKDAY    0x3e
#define     MASK_WEEKEND    0x41
#define     MASK_EVERYDAY   0x7f
//[end]

//others
#define ALM_DECEMBER                        12
#define ALM_MODE_WEEK_NUM                   7
#define ALM_WEEK_DAY_NUM                    7
#define ALM_MODE_NUM                        3 // 三种重复方式

#define TYPE_AUTO_POWERON		            0
#define TYPE_AUTO_POWEROFF		            1

#define ALM_DEFAULT_ALARM_RINGTONG	        1       // default alarm ring tone   

typedef enum{
        SMART_EVENT_NULL = 0,   
        SMART_EVENT_ALARM,            //闹钟
        SMART_EVENT_AUTOPOWERON,      //自动开机
        SMART_EVENT_AUTOPOWEROFF,     //自动关机
        SMART_EVENT_SCH,            	 //日程表	
#ifdef MMI_AZAN_SUPPORT
        SMART_EVENT_AZAN,             //AZAN
        //SMART_EVENT_AZAN_ADD,         
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
        SMART_FM_TIMER_RECORD,        //FM定时录音
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
        SMART_EVENT_SOGOU_UPDATE,
#endif    
        SMART_EVENT_MAX				

}MMIACC_SMART_EVENT_E;

typedef uint32 MMIACC_MODE_TYPE_E;
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
#define ALM_MODE_ONCE		 	0   
#define ALM_MODE_EVERYDAY       1 
#define ALM_MODE_EVERYWEEK      2 
#define ALM_MODE_EVERY15DAYS    3 
#define ALM_MODE_EVERYMONTH     4 
#define ALM_MODE_EVERYYEAR      5 
#define ALM_MODE_NO				6
#define ALM_MODE_MAX_TYPE       ALM_MODE_NO+1-1 //原来的日程不要每15选项
#else
#define ALM_MODE_ONCE		 	0   
#define ALM_MODE_EVERYDAY       1 
#define ALM_MODE_EVERYWEEK      2 
#define ALM_MODE_EVERYMONTH     3 
#define ALM_MODE_EVERYYEAR      4 
#define ALM_MODE_NO				5
#define ALM_MODE_MAX_TYPE       6
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
// index of all event in EFS
typedef enum{
    EVENT_ALARM_0 =0,
    /*EVENT_ALARM_1,
    EVENT_ALARM_2,
    EVENT_ALARM_3,
    EVENT_ALARM_4,
    EVENT_ALARM_5,
    EVENT_ALARM_6,
    EVENT_ALARM_7,
    EVENT_ALARM_8,
    EVENT_ALARM_9,*/
    EVENT_ALARM_MAX=EVENT_ALARM_0+MMIACC_ALARM_NUM-1,

    EVENT_ALARM_DELAY_0, // 延时
    /*EVENT_ALARM_DELAY_1, // 延时
    EVENT_ALARM_DELAY_2, // 延时
	EVENT_ALARM_DELAY_3,
	EVENT_ALARM_DELAY_4,
	EVENT_ALARM_DELAY_5,
	EVENT_ALARM_DELAY_6,
	EVENT_ALARM_DELAY_7,
	EVENT_ALARM_DELAY_8,
	EVENT_ALARM_DELAY_9,*/
	EVENT_ALARM_DELAY_MAX=EVENT_ALARM_DELAY_0+MMIACC_DELAY_ALARM_NUM-1,
	
    EVENT_SCH_0,
    EVENT_SCH_MAX = EVENT_SCH_0 + MMIACC_SCH_NUM-1,
#ifdef MMI_AZAN_SUPPORT
	EVENT_AZAN_ALARM_1,        // Azan alarm
	EVENT_AZAN_ALARM_2,
	EVENT_AZAN_ALARM_3,
	EVENT_AZAN_ALARM_4,
	EVENT_AZAN_ALARM_5,
	EVENT_AZAN_ALARM_6,
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    EVENT_FM_TIMER_RECORD,      //FM定时录音    
#endif

    EVENT_AUTO_POWERON,
    EVENT_AUTO_POWEROFF,

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    EVENT_SOGOU_UPDATE,
#endif 
        
    EVENT_MAX_COUNT 
}MMIACC_EVENT_EFS_E;

typedef enum
{
	ALM_FIX_RING,				//固定铃声
	ALM_DOWNLOAD_RING,			//下载铃声
#ifdef FM_SUPPORT
	ALM_FM_RING,				//fm铃声
#endif 
    ALM_RECORD_RING,
	ALM_RING_TYPE_MAX	
}ALM_RING_TYPE_E;	


#ifdef CMCC_UI_STYLE
typedef enum
{
	MMISCH_REMIND_ONTIME, //准时
	MMISCH_REMIND_FIVE_MIN, //提前5分钟
	MMISCH_REMIND_FIFTEEN_MIN, //提前15分钟
	MMISCH_REMIND_ONE_HOUR,	 //提前1小时
	MMISCH_REMIND_ONE_DAY, //提前一天
	MMISCH_REMIND_ONE_WEEK, //提前一周
	MMISCH_REMIND_MAX_TYPE    
}MMISCH_REMIND_TYPE_E;
#endif


//定义日程表，闹钟的数据结构快表,记录了事件的基本数据,共20byte, saved in NV
typedef struct MMIACC_SMART_EVENT_FAST_TABLE_TAG
{
    BOOLEAN             is_valid;           // whether record is used
    BOOLEAN 			is_on;		        // True: Smart event ON ,False: Smart event OFF
    MMIALMSVC_ID_T      service_id;
    
    MMIACC_SMART_EVENT_E	type;		    // the type of smart event
    uint8               delay_num;          // 闹钟自动延时的次数,如果是手动延时,该值始终为0

//start time,用于周期性事件的开始时间
    uint16              start_year;
    uint8               start_mon; 
    uint8               start_day;	

//cur event time	
    uint8				hour;
    uint8				minute;
    uint16				year;
    uint8				mon;
    uint8				day;
#ifdef CMCC_UI_STYLE
//stop event time	
    uint8				stop_hour;
    uint8				stop_minute;
    uint16				stop_year;
    uint8				stop_mon;
    uint8				stop_day;
#endif

//deadline
    uint16              end_year;
    uint8               end_mon; 
    uint8               end_day;

//type and frequency
    MMIACC_MODE_TYPE_E	fre_mode; 
    uint8				fre;		        // frequence of happening, from MON (bit6) to SUN (bit0)  
#ifdef CMCC_UI_STYLE
    MMISCH_REMIND_TYPE_E  remind_type; //提醒时间类型
#endif
} MMIACC_SMART_EVENT_FAST_TABLE_T;

//定义日程表，闹钟事件内容的数据结构, saved in NV
typedef struct MMIACC_SMART_EVENT_CONTENT_TAG//124个byte
{
    //text
    uint16				w_length;			// of content
    wchar				w_content[MMIACC_EVENT_TEXT_MAX_LEN+1];

    //content text
    uint16				w_text_length;	        // of text
    wchar				w_text[MMIACC_EVENT_TEXT_MAX_LEN+1];

    //icon
    uint32				image_id;	        //日历提示图标的ID。无效图标ID为0xFFFF

    //ring
    ALM_RING_TYPE_E		ring_type;		    //固定铃声还是下载铃声
    uint16				ring;		        // ring  mode : ring ID or vibration(ring==0)  

    uint16              ring_dur;//响铃持续时长
    BOOLEAN             is_vibrate;//响铃时是否震动
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    BOOLEAN             is_add;
#endif
} MMIACC_SMART_EVENT_CONTENT_T;

//余下内容存于fs中,下载铃声信息
typedef struct MMIACC_SMART_EVENT_RING_TAG
{
    uint16					dring_name_len;     //下载铃声名的长度
    wchar					dring_name_arr[MMIFILE_FULL_PATH_MAX_LEN+1]; //铃声名
}MMIACC_SMART_EVENT_RING_T;
	
//定义日程表，闹钟的数据结构, saved in NV
typedef struct MMIACC_SMART_EVENT_TAG
{
    MMIACC_SMART_EVENT_FAST_TABLE_T     event_fast_info;
    MMIACC_SMART_EVENT_CONTENT_T        event_content_info;
    MMIACC_SMART_EVENT_RING_T           event_ring_info;
} MMIACC_SMART_EVENT_T;


// 记录某种类型的统计事件
typedef struct MMIACC_SCH_LIST_TAG
{
    uint16					total;	// total valid list item 
    uint16                  entry_id[MMIACC_SCH_NUM+MMIACC_ALARM_NUM];
}MMIACC_SCH_LIST_T;


/**--------------------------------------------------------------------------*
 **                         GLOBAL VARIABLE
 **--------------------------------------------------------------------------*/
//extern MMI_TEXT_ID_T s_mode_week_list_arr[];
extern const uint8 s_week_mask[];
extern const MMI_TEXT_ID_T s_alm_week_text_id[];

/**--------------------------------------------------------------------------*
 **                         GLOBAL FUNCTIONS
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get one event 
//	Global resource dependence : 
//  Author:      liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC const MMIACC_SMART_EVENT_FAST_TABLE_T *MMIALM_GeEvent (
                                            uint16 event_id
                                            );


/*****************************************************************************/
// 	Description : modify one event 
//	Global resource dependence : 
//  Author:      liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_ModifyEvent(
						    uint16 				    event_id, 
						    MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr
						    );

/*****************************************************************************/
//  Description : modify one event content name
//  Global resource dependence : 
//  Author:     xiaohua.liu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_ModifyEventName(
                                  uint16                    event_id, //in
                                  MMIACC_SMART_EVENT_CONTENT_T    *event_content_ptr  //in,out
                                  );
/*****************************************************************************/
// 	Description : delete one event 
//	Global resource dependence : 
//  Author:    liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelOneEvent(
                           uint16 event_id
                           );


/*****************************************************************************/
// 	Description : add one event 
//	Global resource dependence : 
//  Author:    liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_AddOneEvent(
                           MMIACC_SMART_EVENT_T *event_ptr,
                           uint16               *index_ptr);


/*****************************************************************************/
// 	Description : delete a lot of event
//	Global resource dependence : 
//  Author:    liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelEvents(
                         uint16 start_event,
                         uint16 end_event
                         );

/*****************************************************************************/
// 	Description : delete first valid events
//	Global resource dependence : 
//  Author:        
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelFirstValidEvent(void);

/*****************************************************************************/
// 	Description : find if exist valid event
//	Global resource dependence : 
//  Author:        
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_IsExistValidEvent(void);

/*****************************************************************************/
// 	Description : delete current day of one schedule
//	Global resource dependence : 
//  Author: xiangjie
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMICALENDAR_DelCurDayFirstValidSch(void);

/*****************************************************************************/
// 	Description : check exist valid sch in current day
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMIALM_IsExistValidEventInCurDay(void);

/*****************************************************************************/
// 	Description : Open Delete Waiting window
//	Global resource dependence : 
//  Author:        
//	Note:   
/*****************************************************************************/
PUBLIC void MMIALM_OpenDeleteWaittingWin(MMI_MESSAGE_ID_E pos_msg_id);

/*****************************************************************************/
// 	Description : delete event cnf
//	Global resource dependence : 
//  Author:        
//	Note:   
/*****************************************************************************/
PUBLIC void MMIALM_DeleteCnf(void);

/*****************************************************************************/
// 	Description :interrupt alarm delete 
//	Global resource dependence : 
//  Author:      
//	Note:            
/*****************************************************************************/
PUBLIC void MMIALM_SetAlmInterruptFlag(BOOLEAN  flag);

/*****************************************************************************/
// 	Description :interrupt alarm delete 
//	Global resource dependence : 
//  Author:      
//	Note:            
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_GetAlmInterruptFlag(void);

/*****************************************************************************/
// 	Description : update events when date and time setting changes
//	Global resource dependence : 
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIALM_UpdateEvents(void);


/*****************************************************************************/
// 	Description : whether there's active smart event
//	Global resource dependence : 
//  Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_HasActiveEvent(void);

/*****************************************************************************/
// 	Description : init alarm module
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_Init(void);

/*****************************************************************************/
// 	Description : get valid event match with current date and time
//	Global resource dependence : 
//  RETURN :  if event found, return TRUE
//  Author:   taul.zhu
//	Note: 当MMI收到闹铃消息时，获得和当时日期时间相匹配的事件，然后更新事件(删除或者
//      计算下一次发生的时间，修改事件) 
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_GetArrivedEvent(
                              MMIACC_SMART_EVENT_T *event_ptr,
                              uint16               *event_id
                              );

/*****************************************************************************/
// 	Description : restore factory setting of alarm
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_RestoreFactorySetting(void);

/*******************************************************************/
//  Interface:		MMIALM_TimeUpdateCallback
//  Description : 	MMIALM_TimeUpdateCallback
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIALM_TimeUpdateCallback(uint32 old_time, uint32 new_time);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif  



