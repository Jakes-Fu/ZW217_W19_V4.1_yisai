/******************************************************************************
 ** File Name:      mail_asyn.h                                              *
 ** Author:         lixu                                                   *
 ** DATE:           2010/05/01                                               *
 ** Copyright:               *
 ** Description:                                                              *
 ******************************************************************************/
#ifndef _EASYNC_H
#define _EASYNC_H

#include "mail_sig.h"
#include "comm_platform.h"
#include "mail_ret.h"




typedef void* eAsyncOperationHandle;


typedef enum
{
    ASYNC_MODE_READ,
    ASYNC_MODE_WRITE,
    ASYNC_MODE_APPEND,
    ASYNC_MODE_READ_WRITE,
    ASYNC_MODE_DISPATCH
} EcsAsyncMode;


/*****************************************************************************/
//Description : 
//Global resource dependence : none
//Author: 
//Note:
/*****************************************************************************/
eAsyncOperationHandle mail_AsyncOperationStart(const char *fileName,
    comm_uint32 fileHandle, //�ļ����������������ģ���ڱ���һ��
    EStateMachine fsm, 
    long instance, 
    int sig,
    EcsAsyncMode mode, //�ò�����ʱ����
    mail_retval *result);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
comm_boolean mail_AsyncWriteHead(eAsyncOperationHandle oper,
                    long startPos,
                    void *buffer,
                    long bufSize);

/*****************************************************************************/
//Description : 
//��mail_AsyncOperationStop���ͷ�mail_AsyncOperationStart������Ŀռ�
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
void mail_AsyncOperationStop(eAsyncOperationHandle *oper);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
comm_boolean mail_AsyncRead(eAsyncOperationHandle oper,
                   long startPos,
                   void *buffer,
                   long bufSize);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
comm_boolean mail_AsyncWrite(eAsyncOperationHandle oper,
                    long startPos,
                    void *buffer,
                    long bufSize);
/*****************************************************************************/
//Description : 
//Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
int mail_AsyncSetSize(eAsyncOperationHandle oper,
                     comm_uint32 newSize);
/*****************************************************************************/
// 	Description : ��д����첽��Ϣ�ķַ�
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
void mail_AsyncSendMsg(eAsyncOperationHandle oper,
                                                mail_retval param);

#endif 
