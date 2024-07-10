/****************************************************************************
** File Name:      mmk_msg_internal.h                                     *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                  message                                                *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
** 
****************************************************************************/
#ifndef  _MMK_MSG_INTERNAL_H_    
#define _MMK_MSG_INTERNAL_H_   

/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_kbd.h"
#include "tb_dal.h"



/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/*Define the struct of message queue*/
typedef struct MsgQueue_tag
{
    MMI_MESSAGE_PTR_T   msg;            // the struct pointer of message
    uint8               len;            // the length of message queue
    uint8               readpos;        // the position of read message 
    uint8               writepos;       // the position of write message
    BOOLEAN             is_queue_full;
} MMK_MSG_QUEUE_T;

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/



/*****************************************************************************/
// 	Description : dispatch the window message to window or the parent window of control
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
BOOLEAN  MMK_DispatchToHandle(
                              MMI_HANDLE_T        handle,
                              MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                              DPARAM              param_ptr   //the param of the message
                              );



/*****************************************************************************/
// 	Description : dispatch the window message to focus window
//	Global resource dependence : ;
//  Author:Louis wei
//	Note:
/*****************************************************************************/
BOOLEAN  MMK_DispMsgToFocusWin(
                                MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                                DPARAM              param_ptr   //the param of the message
                                );

/*****************************************************************************/
// 	Description : get the current queue msg
//	Global resource dependence : ;
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_MESSAGE_PTR_T MMK_GetCurQueueMsg(
                                            void
                                            );

/*****************************************************************************/
// 	Description : delete the message by the window or control handleptr
//                ��Close���ڻ��߿ؼ�ʱ��Ҫ�������������������Ϣ���У�
//                �Ա��⵱���ڻ�ؼ����ͷź���Ȼ�յ���Ϣ�����⡣
//                ʵ���Ͻ��������⻹������һ��˼·�������ڲ����еĹ���Ȩ
//                �������ڣ�����������ʱ����ϢҲͬʱ���١�����Ŀǰ����Ϣ��
//                ����ʽ������ͬ����Щ���ѣ����������������Ϊ��ͬ���õ��ġ�              
//	Global resource dependence :
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DeleteMSGByHandle(
                                  MMI_HANDLE_T handle
                                  );

/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_MSG_H_  */
