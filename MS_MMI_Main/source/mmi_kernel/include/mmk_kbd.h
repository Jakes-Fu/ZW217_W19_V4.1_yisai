/****************************************************************************
** File Name:      mmk_kdb.h                                               *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message id of key     *
**                  and define the function of key                         *                                                *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create								   *
**																		   *
****************************************************************************/
/*!
 *  \addtogroup mmk_kbd_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmk_kbd.h
 *  \author	Louis.wei
 *  \date	Mar,2003
 *  \brief 	This file is used to describe the message id of key
 */

#ifndef  _MMK_KBD_H_    
#define _MMK_KBD_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_signal.h"
#include "mmi_osbridge.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMI_MAX_WAITING_KEYS    3					/*!< max waiting key msg in mmi queue*/
#define MMI_MAX_MULTI_KEYS      3		            /*!< max key msg on same time*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    BOOLEAN         is_slide;                               //if slide,only for tp up
    GUI_POINT_T     pre_tp_point;
    GUI_POINT_T     cur_tp_point;                           /*!< current tp point*/
    uint32          multi_key_num;                          /*!< other key num*/
    uint32          multi_key_msg[MMI_MAX_MULTI_KEYS];      /*!< other key message array*/
    MMI_TP_STATUS_E multi_tp_status;                        /*!< other tp status*/
    GUI_POINT_T     multi_tp_point;                         /*!< other tp point*/
    MMI_GESTURE_TYPE_E gesture_type;
    MMI_TP_FLING_DIRECTION_E tp_fling_direction;
}MMI_MULTI_KEY_TP_MSG_PARAM_T;

//key queue info
typedef struct
{
    GUI_POINT_T              down_tp_point;             /*!<down tp point*/
    GUI_POINT_T              up_tp_point;               /*!<up tp point*/
    MMI_TP_FLING_DIRECTION_E tp_fling_direction;
}MMI_TP_MSG_PARAM_T;

//key queue info
typedef struct
{
    uint32          waiting_keys_num;                       //from 0 to 3
    uint32          waiting_keycode[MMI_MAX_WAITING_KEYS];  //keycode array
    uint32          keycode_num;                            //keycode array number,from 0 to 2
} MMK_KEY_QUEUE_INFO;

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/
/******************************************************************************/
/*! \brief check the key single because of the system queue full,
 *			call by KeypadCallBackFunc
 *  \param[in]		single_code		key type, KPD_DOWN or KPD_UP
 *  \param[in]		key_code		key code
 *  \author  Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>: 	key signal valid
 * 		- <i><c>FALSE</c></i>:	key signal invalid
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_CheckKeySingle(
                                  uint16 single_code, //[IN] key type, KPD_DOWN or KPD_UP
                                  uint32 key_code     //[IN] key code
                                  );

/******************************************************************************/
/*! \brief			Init mmk keypad globle
 *  \author			bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_InitKBD(void);

/******************************************************************************/
/*! \brief			handle key long press timer
 *  \param[in]		timer_id		key long press timer id
 *  \author  bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_HandleKeyLongPressTimer(uint8 timer_id);

/******************************************************************************/
/*! \brief			handle key long press timer
 *  \param[in]		timer_id		key long press timer id
 *  \author  bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_HandleKeyRepeatPressTimer( void );

/******************************************************************************/
/*! \brief			stop repeat timer
 *  \param[in]		timer_id		key long press timer id
 *  \author  bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_StopKeyRepeatPressTimer( void );
/******************************************************************************/
/*! \brief get multi keypad message
 *  \param[in]		multi_key_msg	multi key message array
 *  \author  bruce.chi
 *  \return 
 *		- <i><c>message number</c></i>
 */
/******************************************************************************/
PUBLIC int32 MMK_GetMultiKeyMsg(MMI_MESSAGE_ID_E multi_key_msg[MMI_MAX_MULTI_KEYS], uint8 cur_key_code);

/******************************************************************************/
/*! \brief			quick key, key is nearly full
 *  \author			bin.ji
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsQuickKey(void);

/*****************************************************************************/
// 	Description : is key msg
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsKeyMsg(
                            MMI_MESSAGE_ID_E    msg_id
                            );

#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
/*****************************************************************************/
// 	Description : is shift key press
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsShiftKeyPress( void );
#endif

/*****************************************************************************/
// 	Description : dispatch the key message after translate the signal of RTOS to
//                MMI Message
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_DispatchMSGKbd(
                        MmiKeyPress* signal_ptr // the pointer of message which need dispatch
                        );

BOOLEAN MMK_DefaultMSGKbd(
    uint32 keystatus,
    uint32 keycode
);
BOOLEAN MMK_DefaultMSGKbdLater(
    uint32 keystatus,
    uint32 keycode
);

/*****************************************************************************/
// 	Description : window需要同时关注单击和双击事件时调用
//	Global resource dependence :
//  Author: Cheng.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMK_NeedKbdClkNotifyOnce(void);

/*******************************************************************/
//  Interface:		MMK_IsNeedKbClkNotify
//  Description : 	MMK_IsNeedKbClkNotify
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_IsNeedKbClkNotify(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_KBD_H_  */
/*! @} */
