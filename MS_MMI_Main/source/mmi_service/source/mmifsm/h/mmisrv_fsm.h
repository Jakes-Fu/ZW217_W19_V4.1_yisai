/*****************************************************************************
** File Name:      mmisrv_fsm.h                                              *
** Author:                                                                   *
** Date:           2012/7/3													 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe fsm						 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/7/3       aoke.hu			Create
******************************************************************************/

#ifndef _MMISRV_FSM_H_
#define _MMISRV_FSM_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
typedef uint32	FSMSRV_HANDLE;
typedef void*	FSM_OBJECT;

//状态机接收的消息
typedef enum
{
	FSMSRV_INIT_REQ,
	FSMSRV_ENTRY_REQ,       
	FSMSRV_EXIT_REQ,
    FSMSRV_USER_REQ
}FSMSRV_MSG_E; 

//控制层的消息结构
typedef struct 
{
    FSMSRV_HANDLE       fsm_handle;     //状态机
    uint32		        msg_id;         //消息id
    uint32              msg_size;       //消息大小
    void                *msg_body;      //消息体
}FSMSRV_MSG_T;

typedef BOOLEAN    (*FSM_STATE_FUNC)(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr); 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSMSRV_HANDLE FSMSRV_Construct(FSM_OBJECT		fsm_object,
									  uint32			fsm_object_size,
									  FSM_STATE_FUNC	fsm_state_init
									  );

/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void FSMSRV_Destruct(FSMSRV_HANDLE *fsm_handle_ptr);

/*****************************************************************************/
//  Description : 分发请求消息 msg_body_ptr中如嵌套指针，请先做序列化。
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN FSMSRV_Dispatch(FSMSRV_HANDLE fsm_handle,  uint32 msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void FSMSRV_StateTrans(FSMSRV_HANDLE fsm_handle, FSM_STATE_FUNC	fsm_func);

/*****************************************************************************/
//  Description : 获取当前状态函数
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSM_STATE_FUNC FSMSRV_GetState(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : 获取对象数据结构
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSM_OBJECT FSMSRV_GetObject(FSMSRV_HANDLE fsm_handle);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
