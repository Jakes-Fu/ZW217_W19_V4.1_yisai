/*****************************************************************************
** File Name:      mmialarm_internal.h                                                
** Author:         taul.zhu   
** Date:           2012/02/07
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe internal interface of alarm
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** Date:           2012/02/07         Create                                 *
******************************************************************************/

#ifndef _MMI_ALARM_INTERNAL_H_
#define _MMI_ALARM_INTERNAL_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
//#include "window_parse.h"
#include "mmiacc_event.h"
//#include "mmidisplay_data.h"
//#include "mmialarm_service.h"
//#include "mmiset.h"

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

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    ALM_WEEK_DAY_IS_DARK,
    ALM_WEEK_DAY_IS_UNSEL,
    ALM_WEEK_DAY_IS_SEL,
}ALARM_WEEK_DAY_MODE_E;

typedef enum
{
    ALM_CLOCK_TYPE_1 = 0,
    ALM_CLOCK_TYPE_2,
    ALM_CLOCK_TYPE_MAX,
}ALARM_CLOCK_TYPE_E;

/**--------------------------------------------------------------------------*
 **                         GLOBAL FUNCTIONS
 **--------------------------------------------------------------------------*/
/***********************************************************************************
 Name:			MMIALM_IsAlarmExpired
 Description:	if an alarm/event is waiting for occuring
 Author:		
 Return:		 True if  an alarm is waiting for prompt
***********************************************************************************/
PUBLIC BOOLEAN MMIALM_IsAlarmExpired(void);
/*****************************************************************************/
// 	Description : Register alarm menu group
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIALM_RegMenuGroup(void);
/*****************************************************************************/
// 	Description : register alarm module nv len and max item
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIALM_RegAlarmNv(void);

//*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIALARM_RegWinIdNameArr(void);

/*****************************************************************************/
// Description : get ring info
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_GetRingInfo(uint16 event_id, MMIACC_SMART_EVENT_T *event_info_ptr);
/*****************************************************************************/
//  Description : get clock time.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIALARM_GetTime(uint8* hour, uint8* min);
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : 获取日程时间
//	Global resource dependence : 
//  Author:  
//	Note:   is_get_remind_time //true:实际提醒时间,false对应的设置时间
/*****************************************************************************/
PUBLIC MMI_TM_T MMIALM_GetSchTime(
                                            uint16 year,
                                            uint8 mon,
                                            uint8 day,
                                            uint8 hour,
                                            uint8 minute,
                                            MMISCH_REMIND_TYPE_E remind_type,
                                            BOOLEAN is_get_remind_time
                                            );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  


