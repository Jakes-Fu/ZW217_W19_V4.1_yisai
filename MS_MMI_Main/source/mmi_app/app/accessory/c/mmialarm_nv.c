/*************************************************************************
 ** File Name:      mmialarm_nv.c                                        *
 ** Author:         liqing.peng                                          *
 ** Date:           2006/09/12                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/12     liqing.peng      Create.                              *
*************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmialarm_nv.h"
#include "mmi_modu_main.h"
#include "mmialarm_export.h"

/*the length of alarm nv*/
const uint16 alarm_nv_len[] =
{
    //sizeof(BOOLEAN), // MMINV_ACC_ALARM_FLAG

    sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T)*MMIACC_EVENT_MAX_NUM,//快表,包含了所有事件的简略信息    

    //alarm NV begin
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 1
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 2
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 3
#ifndef MMI_ALARM_MINI_LOW_COST    //与MMIACC_ALARM_NUM数量有关
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 4
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 5
#ifndef ALARM_SUPPORT_LOW_MEMORY	
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 6
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 7
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 8
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 9
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),  //alarm 10
#endif
#endif //MMI_ALARM_MINI_LOW_COST

    sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 1
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 2
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 3
#ifndef MMI_ALARM_MINI_LOW_COST    //与MMIACC_ALARM_NUM数量有关
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 4
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 5
#ifndef ALARM_SUPPORT_LOW_MEMORY	
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 6
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 7
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 8
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 9
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //delay alarm 10
#endif
#endif  //MMI_ALARM_MINI_LOW_COST

    sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //EVENT_AUTOPOWER_ON
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),   //EVENT_AUTOPOWER_OFF
    
#ifdef MMI_AZAN_SUPPORT
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
	sizeof(MMIACC_SMART_EVENT_CONTENT_T),
#endif  
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
#endif
#ifdef CALENDAR_SUPPORT 
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
#ifndef MMI_ALARM_MINI_LOW_COST    //与MMIACC_SCH_NUM数量有关
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
#ifndef CALENDAR_SUPPORT_LOW_MEMORY
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
    sizeof(MMIACC_SMART_EVENT_CONTENT_T),
#endif //CALENDAR_SUPPORT_LOW_MEMORY
#endif //MMI_ALARM_MINI_LOW_COST
#endif //CALENDAR_SUPPORT
//alarm NV end
};

/*****************************************************************************/
// 	Description : register alarm module nv len and max item
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIALM_RegAlarmNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_ALARM, alarm_nv_len, (1+MMIACC_EVENT_MAX_NUM)/* sizeof(alarm_nv_len) / sizeof(uint16)*/);
}
