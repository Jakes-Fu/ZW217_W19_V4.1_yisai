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

//�ص������Ĳ����ṹ
typedef struct
{
    uint32                  msg_id; //��ϢID
    uint32                  handle; //�����initʱ���ظ�UI��handle��    
    void*                   para;   //��Ϣ�����Ĳ���
} SAMPLE_CALLBACK_T;

typedef void (*SAMPLE_CALLBACK)(SAMPLE_CALLBACK_T *msg_ptr);

//״̬�����յ���Ϣ
typedef enum
{
	SAMPLE_ENTRY_REQ,       
	SAMPLE_INIT_REQ,
	SAMPLE_CLOSE_REQ, 
	SAMPLE_RELEASE_REQ,	
	SAMPLE_EXIT_REQ,
    SAMPLE_REQ_MSG_MAX
}SAMPLE_FSM_MSG_E; 

//���Ʋ����Ϣ�ṹ
typedef struct 
{
    SAMPLE_HANDLE        fsm_handle;     //״̬��
    SAMPLE_FSM_MSG_E     msg_id;         //��Ϣid
    uint32               msg_size;       //��Ϣ��С
    void                 *msg_body;      //��Ϣ��
}SAMPLE_FSM_MSG_T;

//״̬
typedef enum
{
	SAMPLE_STATE_IDLE,
	SAMPLE_STATE_READY,
	SAMPLE_STATE_MAX
}SAMPLE_STATE_E;

//״̬����Ϣ�ṹ
typedef struct  
{    
    SAMPLE_STATE_E              state;                      //״̬��״̬   
	uint32						sample_param;
	SAMPLE_CALLBACK				samplecallback;
}SAMPLE_FSM_T;

//״̬���б� ������չ��״̬��������Ŀǰ֧�ֵ���״̬��
typedef struct SAMPLE_FSM_LIST_tag
{
    SAMPLE_FSM_T                *fsm_info_ptr;       //״̬����Ϣ
    struct SAMPLE_FSM_LIST_tag   *next_fsm_ptr;     //��һ��״̬��
}SAMPLE_FSM_LIST_T;

typedef BOOLEAN    (*FSM_DISPATCH)(SAMPLE_FSM_T *fsm_ptr, SAMPLE_FSM_MSG_T *msg_ptr); 


/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC  SAMPLE_HANDLE  SrvSample_FSMCreate(SAMPLE_CALLBACK callback);

/*****************************************************************************/
//  Description : ��װ�ӿ�ʾ��
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
