 /****************************************************************************
** File Name:      mmisrvaud_util.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             22/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to declare some util functions about audio service.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 22/04/2011       Yintang.ren         Create
** 
****************************************************************************/

#ifndef _MMISRVAUD_UTIL_H_
#define _MMISRVAUD_UTIL_H_


typedef enum
{
    SRVAUD_LINK_ACTIVE,
    SRVAUD_LINK_READY,
    SRVAUD_LINK_STANDING,
    SRVAUD_LINK_SUSPEND
}SRVAUD_LINK_E;

typedef void (*MMISRVAUD_LINK_TRAVEL_FUNC)(uint32 data, uint32 param);
typedef BOOLEAN (*MMISRVAUD_LINK_TRAVEL_EX_FUNC)(uint32 data, uint32 param, uint32 *res);
typedef void (*MMISRVAUD_TIMER_CB_FUNC)(uint8  timer_id, uint32 param);


typedef void    *MMISRVAUD_MUTEX_PTR;
    
/*****************************************************************************/
//  Description:  Init audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkCreate(void);

/*****************************************************************************/
//  Description:  Push service entity to an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkPush(SRVAUD_LINK_E link_type, uint32 data);

/*****************************************************************************/
//  Description:  Pop a service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkPop(SRVAUD_LINK_E link_type);

/*****************************************************************************/
//  Description:  Append a service entity to an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkAppend(SRVAUD_LINK_E link_type, uint32 data);

/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkDeAppend(SRVAUD_LINK_E link_type);

/*****************************************************************************/
//  Description:  Get the top node of one link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_E link_type);

/*****************************************************************************/
//  Description:  Get the next node of one link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: Pay attention!!! Please call MMISRVAUD_LinkGetHeadNode() first, 
//          otherwise it will occur unexpected error!!!
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetNextNode(void);

/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetNum(SRVAUD_LINK_E link_type);

/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_E link_type, uint32 data);

/*****************************************************************************/
//  Description:  Get the link head node by link type.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkSuspend(void);

/*****************************************************************************/
//  Description:  Destroy all the links.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_LinkDestroy(void);


/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetNode(SRVAUD_LINK_E link_type, uint32 data);

/*****************************************************************************/
//  Description:  resume the appointed suspended link 
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkResume(uint32 data);


/*****************************************************************************/
//  Description:  Travel link and do somethine one by one.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkTravel(SRVAUD_LINK_E link_type, MMISRVAUD_LINK_TRAVEL_FUNC func, uint32 param);

/*****************************************************************************/
//  Description:  Travel link and do somethine one by one.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkTravelEx(SRVAUD_LINK_E link_type, MMISRVAUD_LINK_TRAVEL_EX_FUNC func, uint32 param, uint32 *res);

/*****************************************************************************/
//  Description:  push the current active link to stack.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_StackPush(void);

/*****************************************************************************/
//  Description:  pop a link from stack.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_StackPop(void);

/*****************************************************************************/
//  Description:  Get the link from stack by data.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_StackGetLink(uint32 data);

/*****************************************************************************/
//  Description:  Get the data from stack and delete it from stack.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_StackGetAndDelete(uint32 data);

/*****************************************************************************/
//  Description:  Get the stack num.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMISRVAUD_StackGetNum(void);

/*****************************************************************************/
//  Description:  Make a judgement if headset is plug in.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsHeadSetPlugIn(void);

/*****************************************************************************/
//  Description:  Make a judgement if BT headset is active.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsBTHeadSetActive(void);

/*****************************************************************************/
//  Description:  Create and start a timer with callback function.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMISRVAUD_CreateTimer(
                    uint32 time_out,
                    MMISRVAUD_TIMER_CB_FUNC func,
                    uint32 param,
                    BOOLEAN is_period
                    );


/*****************************************************************************/
//  Description:  Close timer.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_StopTimer(
                    uint8 timer_id
                    );

/*****************************************************************************/
//  Description:  If the BT headset is a2dp type.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_BtIsA2dp(void);

/*****************************************************************************/
//  Description:  Open BT device.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_BtOpen(MMISRVAUD_TYPE_U *type_ptr, uint32 sample_rate);

/*****************************************************************************/
//  Description:  Close BT device.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_BtClose(void);

/*****************************************************************************/
//  Description:  Send a BT msg to system's focus win.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_SendBtMsg(uint32 msg_data);

/*****************************************************************************/
//  Description : Set video's sound is play or stop.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_SetVideoSound(BOOLEAN is_sound_on);

/*****************************************************************************/
//  Description : Create a mutex.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_MUTEX_PTR MMISRVAUD_CreateMutex(void);

/*****************************************************************************/
//  Description : Get the appointed mutex.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_GetMutext(MMISRVAUD_MUTEX_PTR mutex_ptr);

/*****************************************************************************/
//  Description : Free the appointed mutex.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PutMutext(MMISRVAUD_MUTEX_PTR mutex_ptr);

#endif


