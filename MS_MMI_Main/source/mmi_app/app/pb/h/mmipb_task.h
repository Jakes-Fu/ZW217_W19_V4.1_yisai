/*****************************************************************************
** File Name:      mmipb_task.h                                            *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file  define task of phonebook.
**                 这个文件PB task中的消息接受，处理和管理                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_TASK_H_
#define _MMIPB_TASK_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "mmk_type.h"
#include "mmipb_common.h"
#include "mmipb_interface.h"
#include "mmi_custom_define.h"
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

 /**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//发送给pb task的消息
typedef enum
{
    MMIPB_SIG_NONE,

    MMIPB_SIG_SORT, //排序请求
    MMIPB_SIG_READ, //读请求
	MMIPB_SIG_WRITE,//写请求
	MMIPB_SIG_DELETE,//删除请求
#ifdef  SNS_SUPPORT
	MMIPB_SIG_SNS_UPDATE,
    MMIPB_SIG_SNS_READ_ACTIVE,
#endif

#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    MMIPB_SIG_SEARCH,
#endif
#ifdef BT_PBAP_SUPPORT
    MMIPB_SIG_PBAP_CONTACT_READ, //读取联系人同步信息请求
    MMIPB_SIG_PBAP_CALLLOG_READ, //read request for calllog
#endif
    MMIPB_SIG_MAX
}MMIPB_SIG_TYPE_E;
//
typedef struct  
{
    SIGNAL_VARS
    MMIPB_HANDLE_T      handle;
    MMIPB_HANDLE_NODE_T handle_node; 
    MMIPB_ERROR_E ret;
}MMIPB_SIGNAL_T;

typedef struct  
{
    SIGNAL_VARS
    uint32    handle;
    MMIPB_HANDLE_NODE_T handle_node; 
    MMIPB_ERROR_E ret;
}MMIPB_CNF_MSG_T;

typedef struct _MMIPB_SIGN_T
{
    uint16   wstr_len;                    
	wchar   wstr[MMIPB_MAX_CUSTOM_TEXT_LEN + 1];   
} MMIPB_SIGN_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create pb task
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateTask(void);

/*****************************************************************************/
//  Description : send signal to pb task
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_SendSignalToPBTask( uint16 sig_id, uint32 handle);

/*****************************************************************************/
//  Description : send signal to mmi task
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_SendSignalToMMITask(uint16 sig_id, MMIPB_CNF_MSG_T *msg_ptr);

#ifdef   __cplusplus
    }
#endif

#endif


