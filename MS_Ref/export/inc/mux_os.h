/******************************************************************************
 ** File Name:      mux_osl.h                                      *
 ** Author:         yayan.xu                                              *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:   Related to OS, in order to portability
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                       NAME             DESCRIPTION                               *
 ** 07/10/2008     yayan.xu                 Create.                                   *
 ******************************************************************************/
#ifndef _MUX_OS_H
#define _MUX_OS_H
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "string.h"   /* strxxx() from native library */
#ifdef   __cplusplus
extern   "C"
{
#endif


    /*------------------------------------------------------------------------------
    Defination :Related to OS, in order to portability
    ------------------------------------------------------------------------------*/
    /*memory*/
#define MUX_ALLOC(_SIZE)                                                  SCI_ALLOC(_SIZE)
#define MUX_FREE(_MEM_PTR)                                             SCI_FREE(_MEM_PTR)

#define MUX_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE)     SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE) 
#define MUX_MEMSET(_DEST_PTR, _VALUE, _SIZE)           SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE) 

    /*communtion mechanism*/
#define MUX_CREATE_SIGNAL(_SIG_PTR, _SIG_CODE, _SIG_SIZE, _SENDER) \
         SCI_CREATE_SIGNAL(_SIG_PTR, _SIG_CODE, _SIG_SIZE, _SENDER)
#define MUX_FREE_SIGNAL(_SIG_PTR)                             SCI_FREE_SIGNAL(_SIG_PTR) 

#define MUX_SEND_SIGNAL(_SIG_PTR, _RECEIVER)         SCI_SEND_SIGNAL(_SIG_PTR, _RECEIVER)
#define MUX_GET_SIGNAL( _SELF_ID)                               SCI_GetSignal(_SELF_ID)

    /*thread/task*/
#define MUX_CREATE_THREAD(_THREAD_NAME, _QUEUE_NAME, _ENTRY, _ARGC, _ARGV, \
            _STACK_SIZE, _QUEUE_NUM, _PRIO, _PREEMPT, _AUTO_START) \
            SCI_CREATE_THREAD(_THREAD_NAME, _QUEUE_NAME, _ENTRY, _ARGC, _ARGV, \
            _STACK_SIZE, _QUEUE_NUM, _PRIO, _PREEMPT, _AUTO_START)             

#define MUX_IdentifyThread    SCI_IdentifyThread   

#define MUX_PRIORITY_ABOVE_NORMAL                         SCI_PRIORITY_ABOVE_NORMAL
#define MUX_PREEMPT                                                         SCI_PREEMPT
#define MUX_AUTO_START                                                  SCI_AUTO_START
#define MUX_INVALID_BLOCK_ID   SCI_INVALID_BLOCK_ID
#define MUX_WAIT_FOREVER  SCI_WAIT_FOREVER

    /*timer*/
    typedef SCI_TIMER_PTR           MUX_TIMER_PTR;
#define MUX_DEACTIVE_TIMER(timer_ptr)  SCI_DeactiveTimer(timer_ptr)
#define MUX_ACTIVE_TIMER(timer_ptr) SCI_ActiveTimer(timer_ptr)
#define MUX_CHANGE_TIMER(timer_ptr,timer_fun,timer_expire)  SCI_ChangeTimer(timer_ptr,timer_fun,timer_expire)

#define MUX_CREATE_TIMER(timer_name,timer_fun,input,timer_expire,auto_activate)  \
      SCI_CreateTimer(timer_name,timer_fun,input,timer_expire,auto_activate)

#define MUX_DELETE_TIMER(timer_ptr)  SCI_DeleteTimer(timer_ptr)

#define MUX_NO_ACTIVATE SCI_NO_ACTIVATE

    /*mutex*/
    typedef SCI_MUTEX_PTR           MUX_MUTEX_PTR;
#define MUX_DELETE_MUTEX(mutex_ptr)  SCI_DeleteMutex(mutex_ptr)
#define MUX_CREATE_MUTEX(name_ptr, priority_inherti)  SCI_CreateMutex(name_ptr, priority_inherti)
#define MUX_GET_MUTEX(mutex_ptr, wait_option)  SCI_GetMutex(mutex_ptr, wait_option)
#define MUX_PUT_MUTEX(mutex_ptr)  SCI_PutMutex(mutex_ptr)
#define MUX_INHERIT SCI_INHERIT


    /*event*/
    typedef  SCI_EVENT_GROUP_PTR    MUX_EVENT_GROUP_PTR;
#define MUX_CREATE_EVENT(name_ptr) SCI_CreateEvent(name_ptr)
#define MUX_DELETE_EVENT(group_ptr) SCI_DeleteEvent(group_ptr)
#define MUX_SET_EVENT(group_ptr, flags_to_set, set_option)  SCI_SetEvent(group_ptr, flags_to_set, set_option)
#define MUX_GET_EVENT(group_ptr,requested_flags,get_option,actual_flags_ptr,wait_option) \
     SCI_GetEvent(group_ptr,requested_flags,get_option,actual_flags_ptr,wait_option)

#define MUX_OR SCI_OR
#define MUX_OR_CLEAR SCI_OR_CLEAR
#define MUX_AND_CLEAR  SCI_AND_CLEAR

    /*IRQ*/
#define MUX_DisableIRQ SCI_DisableIRQ
#define MUX_RestoreIRQ SCI_RestoreIRQ


    /*string operation*/
#define MUX_STRCPY  strcpy
#define MUX_SPRINTF sprintf
#define MUX_STRLEN  strlen

#define MUX_SLEEP(time)   SCI_Sleep(time)
#ifdef   __cplusplus
}
#endif
#endif/*_MUX_OS_H*/


