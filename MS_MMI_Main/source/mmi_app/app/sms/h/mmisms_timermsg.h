/*****************************************************************************
** File Name:      mmisms_timermsg.h                                      
** Author:         hongbing.ju
** Date:           201104/26
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to implement timer message function 
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/04/26     hongbing.ju      Create                                    *
******************************************************************************/
#ifdef MMI_TIMERMSG_SUPPORT

/*---------------------------------------------------------------------------*/
/*                          INCLUDE FILES                                    */
/*---------------------------------------------------------------------------*/


#ifndef _MMISMS_TIMERMSG_H_
#define _MMISMS_TIMERMSG_H_

#include "mmiacc_event.h"
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

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE DEFINITION                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMISMS_TIMERMSG_CMP_EQUAL =0,
    MMISMS_TIMERMSG_CMP_FIRST_SMALL,
    MMISMS_TIMERMSG_CMP_SECOND_SMALL
}MMISMS_TIMERMSG_ALM_CMP_E;

typedef struct  
{
    uint16                  year;
    uint8                   month;
    uint8                   day;
    MMI_WIN_ID_T            win_id;
    uint16                  index;
    MMIACC_SMART_EVENT_T    event;
}MMISMS_TIMERMSG_LIST_WIN_PARAM_T;

typedef enum
{
	MMISMS_TIMERMSG_NEW_TIME,
	MMISMS_TIMERMSG_NEW_DATE,
	MMISMS_TIMERMSG_NEW_FREQ,
	MMISMS_TIMERMSG_NEW_FREQ_WEEK,
	MMISMS_TIMERMSG_NEW_DEADLINE,
	MMISMS_TIMERMSG_NEW_MAX,
}MMISMS_TIMERMSG_NEW_ITEM_T;

typedef enum
{
	MMISMS_TIMERMSG_MODE_ONCE,
	MMISMS_TIMERMSG_MODE_EVERYDAY,
	MMISMS_TIMERMSG_MODE_EVERYWEEK,
	MMISMS_TIMERMSG_MODE_EVERYMONTH,
	MMISMS_TIMERMSG_MODE_EVERYYEAR,	
	MMISMS_TIMERMSG_MODE_MAX_TYPE    
}MMISMS_TIMERMSG_MODE_TYPE_E;

typedef struct MMISMS_TIMERMSG_NODE_T
{
    MMIACC_SMART_EVENT_FAST_TABLE_T event_fast_info;
    MMISMS_BOX_TYPE_E box_type;
} MMISMS_TIMERMSG_NODE, *MMISMS_TIMERMSG_NODE_ID;

typedef int32 (* MMISMS_BIN_COMPARE_FUNC)(uint32 base_index, void *compare_base_data, void *list);

typedef struct _MMISMS_BIN_SEARCH_INFO_T
{
    uint32 start_pos;
    uint32 end_pos;
    void  *compare_base_data;
    BOOLEAN b_is_backward;
}MMISMS_BIN_SEARCH_INFO_T;

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL VARIABLE DECLARE                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************
// 	Description : init of this module
// 	Global resource dependence : none
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_Init(void);
#if 1
/*****************************************************************************/
// 	Description : create a window according to timer msg index
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: if index == MMISMS_TIMERMSG_MAX_ITEM create a timer msg interface with current time
//        otherwise   create a timer msg interface with timer msg info from NV
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterTimerMsgMainWin(void);
#else
/*****************************************************************************/
// 	Description : Create Timer Msg Windows
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_CreateTimerMsgWin(void);
#endif
/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_GetCurMsgNode(void);

/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_GetCurEvent(void);

/*****************************************************************************
// 	Description : Set current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_SetCurEvent(MMIACC_SMART_EVENT_FAST_TABLE_T* event_fast_info_ptr);

/*****************************************************************************
// 	Description : get current smart event instance
// 	Global resource dependence : current global timer msg index
// 	Author: jixin.xu
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_Win_GetCurEvent(void);

/*****************************************************************************
// 	Description : get smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMISMS_TIMERMSG_GetEventByOrder(MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************
// 	Description : set box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_TYPE_E box_type);

/*****************************************************************************
// 	Description : get box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMISMS_BOX_TYPE_E MMISMS_TIMERMSG_GetCurrBoxType(void);

/*****************************************************************************
// 	Description : get a free nv storage for saving smart event
// 	Global resource dependence : none
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_DeleteTimerMsg(MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_SetIsTimerMode(BOOLEAN is_timer_mode);
                  
/*****************************************************************************/
// 	Description : get timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsTimerMode(void);

/*****************************************************************************/
// 	Description : set timer msg on/off
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_SetIsTimerMsg(BOOLEAN is_timer_msg);

/*****************************************************************************/
// 	Description : get timer msg status
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMISMS_GetIsTimerMsg(void);

/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_SetIsTimerMsgReplace(BOOLEAN is_timer_msg_replace);
 
/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsTimerMsgReplace(void);

/*****************************************************************************/
// 	Description : check sending timer msg 
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note: if timer msg is ahead of current time, save the msg to send box
//        otherwise send the message directly   
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_CheckTimerSend(BOOLEAN is_send);

/*****************************************************************************/
// Description : Content Notify Callback
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_ContentNotifyCallback(MMIALMSVC_ID_T svc_id, MMIALMSVC_EVENT_TYPE_E svc_type);

/*****************************************************************************/
// 	Description : proc for timer msg after sending or saving
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_PostProc(MMISMS_STATE_T *order_info_ptr);

/*****************************************************************************
// 	Description : Get Timer Msg By Index
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMINV_SMS_TIMERMSG_T* MMISMS_TIMERMSG_GetTimerMsgByIndex(uint16 timer_index);

/*****************************************************************************/
//     Description : 检查Timer Msg是否有效
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMISMS_TIMERMSG_CheckTimerMsgValid(void);

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_TIMERMSG_CheckTimerMSG(BOOLEAN is_direct);

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif // c++

#endif  // _MMISMS_TIMERMSG_H_
#endif
