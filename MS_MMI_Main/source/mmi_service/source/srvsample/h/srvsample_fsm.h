/*****************************************************************************
** File Name:      srvsample_fsm.h                                           *
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

#ifndef _SRVSAMPLE_FSM_H_
#define _SRVSAMPLE_FSM_H_

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
typedef unsigned long int	uint32;
#define PUBLIC 
#define LOCAL static
#define PNULL 0
    
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 

typedef uint32 SAMPLE_HANDLE;

//回调函数的参数结构
typedef struct
{
    uint32                  msg_id; //消息ID
    uint32                  handle; //句柄（init时返回给UI的handle）    
    void*                   para;   //消息所带的参数
} SAMPLE_CALLBACK_T;

typedef void (*SAMPLE_CALLBACK)(SAMPLE_CALLBACK_T *msg_ptr);

//状态机接收的消息
typedef enum
{
	SAMPLE_ENTRY_REQ,       
	SAMPLE_INIT_REQ,
	SAMPLE_CLOSE_REQ, 
	SAMPLE_RELEASE_REQ,	
	SAMPLE_EXIT_REQ,
    SAMPLE_REQ_MSG_MAX
}SAMPLE_FSM_MSG_E; 

//控制层的消息结构
typedef struct 
{
    SAMPLE_HANDLE        fsm_handle;     //状态机
    SAMPLE_FSM_MSG_E     msg_id;         //消息id
    uint32               msg_size;       //消息大小
    void                 *msg_body;      //消息体
}SAMPLE_FSM_MSG_T;

//状态
typedef enum
{
	SAMPLE_STATE_IDLE,
	SAMPLE_STATE_READY,
	SAMPLE_STATE_MAX
}SAMPLE_STATE_E;

//状态机信息结构
typedef struct  
{    
    SAMPLE_STATE_E              state;                      //状态机状态   
	uint32						sample_param;
	SAMPLE_CALLBACK				samplecallback;
}SAMPLE_FSM_T;

//状态机列表 用于扩展多状态机，但是目前支持单个状态机
typedef struct SAMPLE_FSM_LIST_tag
{
    SAMPLE_FSM_T                *fsm_info_ptr;       //状态机信息
    struct SAMPLE_FSM_LIST_tag   *next_fsm_ptr;     //下一个状态机
}SAMPLE_FSM_LIST_T;

typedef BOOLEAN    (*FSM_DISPATCH)(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr); 


/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC  SAMPLE_HANDLE  SrvSample_FSMCreate(SAMPLE_CALLBACK callback);

/*****************************************************************************/
//  Description : 封装接口示例
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SrvSample_FSMRelease(SAMPLE_HANDLE   fsm_handle);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
