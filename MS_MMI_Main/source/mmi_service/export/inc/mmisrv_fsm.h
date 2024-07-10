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

//״̬�����յ���Ϣ
typedef enum
{
	FSMSRV_INIT_REQ,
	FSMSRV_ENTRY_REQ,       
	FSMSRV_EXIT_REQ,
    FSMSRV_USER_REQ
}FSMSRV_MSG_E; 

//���Ʋ����Ϣ�ṹ
typedef struct 
{
    FSMSRV_HANDLE       fsm_handle;     //״̬��
    uint32		        msg_id;         //��Ϣid
    uint32              msg_size;       //��Ϣ��С
    void                *msg_body;      //��Ϣ��
}FSMSRV_MSG_T;

typedef BOOLEAN    (*FSM_STATE_FUNC)(FSMSRV_HANDLE fsm_handle, FSMSRV_MSG_T *msg_ptr); 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSMSRV_HANDLE FSMSRV_Construct(FSM_OBJECT		fsm_object,
									  uint32			fsm_object_size,
									  FSM_STATE_FUNC	fsm_state_init
									  );

/*****************************************************************************/
//  Description : ����״̬��
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void FSMSRV_Destruct(FSMSRV_HANDLE *fsm_handle_ptr);

/*****************************************************************************/
//  Description : �ַ�������Ϣ msg_body_ptr����Ƕ��ָ�룬���������л���
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN FSMSRV_Dispatch(FSMSRV_HANDLE fsm_handle,  uint32 msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : ת��״̬
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void FSMSRV_StateTrans(FSMSRV_HANDLE fsm_handle, FSM_STATE_FUNC	fsm_func);

/*****************************************************************************/
//  Description : ��ȡ��ǰ״̬����
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC FSM_STATE_FUNC FSMSRV_GetState(FSMSRV_HANDLE fsm_handle);

/*****************************************************************************/
//  Description : ��ȡ�������ݽṹ
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
