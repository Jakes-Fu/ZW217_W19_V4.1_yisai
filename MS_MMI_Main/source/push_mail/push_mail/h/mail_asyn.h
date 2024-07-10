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
    comm_uint32 fileHandle, //文件句柄，数据类型与模块内保持一致
    EStateMachine fsm, 
    long instance, 
    int sig,
    EcsAsyncMode mode, //该参数暂时不用
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
//在mail_AsyncOperationStop中释放mail_AsyncOperationStart中申请的空间
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
// 	Description : 读写完成异步消息的分发
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
void mail_AsyncSendMsg(eAsyncOperationHandle oper,
                                                mail_retval param);

#endif 
