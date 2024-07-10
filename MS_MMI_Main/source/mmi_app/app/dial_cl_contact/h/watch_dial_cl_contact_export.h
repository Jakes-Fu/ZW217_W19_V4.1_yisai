/*****************************************************************************
** File Name:      watch_dial_cl_contact_internal.h                                                *
** Author:                                                                   *
** Date:           2021/01/29                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe dial cl contact api         *
                   for other module                                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2021/01/29     feiyue.ji        Create                                    *
******************************************************************************/

/*****************************************************************************/
//  Description : to init calllog
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void WatchCallLog_Init(void);

/*****************************************************************************/
//  Description : to enter dial cl contact window
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void Watch_Dial_Cl_Contact_Win_Enter(void);
/*****************************************************************************/
//  Description : CL update callback
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void CALLLog_UpdateCallback(MMIEVENT_INFO_T *event_info_ptr);


