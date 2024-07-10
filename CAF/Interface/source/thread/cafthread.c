#include "caf_trc.h"
#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      cafthread.c                                             *
** Author:         songliang.liu                                             *
** Date:           20090311                                           *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2009        songliang.liu     Create                                 *
** 
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafmodule_gen.h"
#include "cafthread.h"
#include "mmk_app.h"
#include "sci_api.h"
#include "os_api.h"
#include "priority_app.h"
//#include "mmk_handle_internal.h"
//原型在上面的头文件里，暂时放这里，以后移动到caf 公共系统头文件里
extern BOOLEAN MMK_IsHandleValid(
                                 MMI_HANDLE_T handle
                                 );

//涉及的头文件太多
//#include "mmi_common.h"
//原型在上面的头文件里，暂时放这里，以后移动到caf 公共系统头文件里
extern void MMIAPICOM_OtherTaskToMMI( 
                                     MMI_HANDLE_T     handle,
                                     MMI_MESSAGE_ID_E msg_id,
                                     DPARAM           param_ptr,
                                     uint32           size_of_param
                                     );




/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/

typedef struct
{
    CAF_VTBL_T(ITHREAD_T)  *vtbl_ptr; // 函数列表
    
    CAF_GUID_T      guid;
    uint32          ref_num;   
    BLOCK_ID 		task_id;
	uint32			stack_size;
	void			(*thread_proc)(uint32 argc,  void* argv);
	uint32			argc;
	void*			argv;

}CAFTHREAD_T;


typedef struct
{
    SIGNAL_VARS
	int32 data;
} MMITHREAD_SIG_T;



LOCAL CAF_RESULT_E CAFTHREAD_ClassNew(CAF_GUID_T guid, void** object_pptr);
LOCAL CAF_RESULT_E CAFTHREAD_QueryInterface(ITHREAD_T *me_ptr, CAF_GUID_T guid, void ** object_pptr );
LOCAL uint32 CAFTHREAD_AddRef(ITHREAD_T *me_ptr);
LOCAL uint32 CAFTHREAD_Release(ITHREAD_T *me_ptr);
LOCAL void CAFTHREAD_SetParam ( ITHREAD_T *me_ptr , uint32 stack_size,void (*thread_proc)(uint32 argc,  void* argv) ,uint32 argc,  void* argv);
LOCAL void CAFTHREAD_Start ( ITHREAD_T *me_ptr );  
LOCAL void CAFTHREAD_PostSignalToThread ( ITHREAD_T *me_ptr , ITHREAD_T *des_thread_ptr , void * data_ptr ,uint16 signal_code ,uint16 signal_size);
LOCAL void CAFTHREAD_PostSignalToApplet (ITHREAD_T *me_ptr , CAF_HANDLE_T  applet_handle,void * data_ptr ,uint16 signal_code ,uint16 signal_size);
LOCAL void CAFTHREAD_GetSignal(ITHREAD_T *me_ptr ,CAF_THREAD_SIG_T *sig_ptr );


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// functions table

LOCAL const CAF_VTBL_T(ITHREAD_T) s_thread_vtbl =
{
    CAFTHREAD_QueryInterface,
    CAFTHREAD_AddRef,
    CAFTHREAD_Release,
    CAFTHREAD_SetParam,
    CAFTHREAD_Start,
    CAFTHREAD_PostSignalToThread,
    CAFTHREAD_PostSignalToApplet,
    CAFTHREAD_GetSignal,
  
};



LOCAL const CAF_STATIC_CLASS_T s_thread_class_list[] = 
{
    { SPRD_GUID_THREAD,      CAFTHREAD_ClassNew,  0}
};

const CAF_STATIC_CLASS_INFO_T g_thread_static_class_info = 
{ 
    (CAF_STATIC_CLASS_T*)s_thread_class_list, 
    sizeof(s_thread_class_list) / sizeof(s_thread_class_list[0])
};



/******************************************************************************************
* DESCRIPTION:
*   create new thread object function
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   guid: [IN]   object id
*   object_pptr [out] handle ptr of return object
*
* RETURN:
*   caf result enum 
*   
* NOTE:
******************************************************************************************/
LOCAL CAF_RESULT_E CAFTHREAD_ClassNew(CAF_GUID_T guid, void** object_pptr)
{
	CAFTHREAD_T	 *thread_ptr = PNULL;

	if( SPRD_GUID_THREAD != guid )
		return CAF_RESULT_FAILED;//不是thread id
		
	thread_ptr = SCI_ALLOC_APP(sizeof(CAFTHREAD_T));

	SCI_MEMSET(thread_ptr, 0, sizeof(CAFTHREAD_T));
	thread_ptr->vtbl_ptr = (CAF_VTBL_T(ITHREAD_T)*)&s_thread_vtbl;


	thread_ptr->guid 	 = guid;
	thread_ptr->ref_num	 = 1;


	*object_pptr = thread_ptr;


	return CAF_RESULT_SUCCEEDED;

		
}


/******************************************************************************************
* DESCRIPTION:
*   query thread object interface 
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:		[IN]   thread object prt
*   guid:		[IN]   object id
*   object_pptr [out]  handle ptr of return object
*
* RETURN:
*   caf result enum 
*   
* NOTE:
******************************************************************************************/
LOCAL CAF_RESULT_E CAFTHREAD_QueryInterface(ITHREAD_T *me_ptr, CAF_GUID_T guid, void ** object_pptr )
{
		
	CAFTHREAD_T* thread_ptr = (CAFTHREAD_T*)me_ptr;
	
	if(NULL == thread_ptr)
		return CAF_RESULT_FAILED;
	
	if ( IUNKNOWN_ID == guid ||  thread_ptr->guid == guid )
	{
		*object_pptr = me_ptr;
		
		IUNKNOWN_AddRef( *object_pptr );
	}
	else
		return CAF_RESULT_FAILED; 
	
	return CAF_RESULT_SUCCEEDED;

		
}



/******************************************************************************************
* DESCRIPTION:
*   add thread ref num
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:		[IN]   thread object prt
*
* RETURN:
*   ref num 
*   
* NOTE:
******************************************************************************************/
LOCAL uint32 CAFTHREAD_AddRef(ITHREAD_T *me_ptr)
{
		
	if(NULL == me_ptr)
		return CAF_RESULT_FAILED;

    return ++(((CAFTHREAD_T *)me_ptr)->ref_num);		
		
		
}





/******************************************************************************************
* DESCRIPTION:
*   release thread ref num
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:		[IN]   thread object prt
*
* RETURN:
*   ref num 
*   
* NOTE:
******************************************************************************************/
LOCAL uint32 CAFTHREAD_Release(ITHREAD_T *me_ptr)
{


	CAFTHREAD_T * thread_ptr = (CAFTHREAD_T *)me_ptr;
	
	if(NULL == me_ptr)
		return CAF_RESULT_FAILED;
    
    if ( --thread_ptr->ref_num )
    {
        return( thread_ptr->ref_num );
    }
    else
    {

		SCI_TerminateThread(thread_ptr->task_id);
		SCI_DeleteThread(thread_ptr->task_id);

        SCI_FREE( thread_ptr );   

        return CAF_RESULT_SUCCEEDED;
    }



}


/******************************************************************************************
* DESCRIPTION:
*   set thread object param
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:		[IN]   thread object prt
*   stack_size  [IN]   stack size
*   thread_proc [IN]   thread function
*   argc        [IN]   thread function param's count
*   argv        [IN]   thread function param's ptr
*   
*
* RETURN:
*   null
*   
* NOTE:
******************************************************************************************/
LOCAL void CAFTHREAD_SetParam ( ITHREAD_T *me_ptr ,  uint32 stack_size,	void (*thread_proc)(uint32 argc,  void* argv),uint32 argc,  void* argv )
{
	
	CAFTHREAD_T * thread_ptr = (CAFTHREAD_T *)me_ptr;

	thread_ptr->thread_proc = thread_proc;
	thread_ptr->stack_size = stack_size;
	thread_ptr->argc = argc;
	thread_ptr->argv = argv;

}



/******************************************************************************************
* DESCRIPTION:
*   start thread's running
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:		[IN]   thread object prt
*   
*
* RETURN:
*   null
*   
* NOTE:
******************************************************************************************/


#define  USER_THREAD_QUEUE_NUM   		8
#define  USER_THREAD_TASK_PRIORITY 		PRI_USER_THREAD_TASK_PRIORITY


LOCAL	void CAFTHREAD_Start ( ITHREAD_T *me_ptr )  
{
	

	CAFTHREAD_T * thread_ptr = (CAFTHREAD_T *)me_ptr;


	BLOCK_ID task_id;
	
    task_id = SCI_CreateThread( 
                            "T_P_USER_THREAD", 
                            "Q_P_USER_THREAD", 
                            thread_ptr->thread_proc, 
                            thread_ptr->argc, 
                            thread_ptr->argv, 
                            thread_ptr->stack_size, 
                            USER_THREAD_QUEUE_NUM, 
                            USER_THREAD_TASK_PRIORITY, 
			                SCI_PREEMPT, 
                            SCI_AUTO_START);

	thread_ptr->task_id = task_id;

	
	
}




/******************************************************************************************
* DESCRIPTION:
*   post signal to thread 
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:			[IN]   thread object prt
*   des_thread_ptr  [IN]   des thread object prt
*   data_ptr        [IN]   data prt of signal 
*   signal_code     [IN]   signal code 
*   signal_size     [IN]   signal size
*   
* RETURN:
*   null
*   
* NOTE:
******************************************************************************************/
LOCAL	void CAFTHREAD_PostSignalToThread ( ITHREAD_T *me_ptr , ITHREAD_T *des_thread_ptr , void * data_ptr ,uint16 signal_code ,uint16 signal_size )
{

	CAFTHREAD_T * my_sor_thread_ptr = (CAFTHREAD_T *)me_ptr;

	MMITHREAD_SIG_T *sig_ptr = PNULL;
    
    sig_ptr = (MMITHREAD_SIG_T *)SCI_ALLOC_APP(sizeof(MMITHREAD_SIG_T));
        
    sig_ptr->SignalCode = (uint16)signal_code;
    sig_ptr->SignalSize = sizeof( MMITHREAD_SIG_T );
    sig_ptr->Sender = my_sor_thread_ptr->task_id;
    sig_ptr->data = (int32)data_ptr;

  	//SCI_TRACE_LOW:"CAFTHREAD_PostSignalToThread , signal_code is %x"
  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTHREAD_384_112_2_17_23_1_12_27,(uint8*)"d",signal_code);
    SCI_ASSERT(SCI_SUCCESS ==
        SCI_SendSignal(
        (xSignalHeader)sig_ptr, 
        my_sor_thread_ptr->task_id) );    


}



/******************************************************************************************
* DESCRIPTION:
*   post signal to Applet 
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:			[IN]   thread object prt
*   applet_handle   [IN]   des applet object prt
*   data_ptr        [IN]   data prt of signal 
*   signal_code     [IN]   signal code 
*   signal_size     [IN]   signal size
*   
* RETURN:
*   null
*   
* NOTE:
******************************************************************************************/
LOCAL	void CAFTHREAD_PostSignalToApplet (ITHREAD_T *me_ptr , CAF_HANDLE_T  applet_handle,void * data_ptr ,uint16 signal_code ,uint16 signal_size )
{
	if(!MMK_IsHandleValid(applet_handle))
    {
		//SCI_TRACE_LOW:"CAFTHREAD_PostSignalToApplet :invalid applet_handle ,signal not post out! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTHREAD_417_112_2_17_23_1_13_28,(uint8*)"");
        return ;
    }
    
	MMIAPICOM_OtherTaskToMMI(applet_handle, signal_code, data_ptr, signal_size);
}


/******************************************************************************************
* DESCRIPTION:
*   get  signal 
*
* AUTHOR:
*   songliang.liu
*
* PARAMETERS:
*   me_ptr:			[IN]   thread object prt
*   sig_ptr			[out]  save signal comment
*   
* RETURN:
*   null
*   
* NOTE:
******************************************************************************************/
LOCAL void	 CAFTHREAD_GetSignal(ITHREAD_T *me_ptr , CAF_THREAD_SIG_T *sig_ptr )
{
	MMITHREAD_SIG_T *psig_ptr = NULL;

	
	CAFTHREAD_T * my_sor_thread_ptr = (CAFTHREAD_T *)me_ptr;
	/*lint -e64*/
	SCI_RECEIVE_SIGNAL(psig_ptr , my_sor_thread_ptr->task_id );
	
	sig_ptr->SignalCode = psig_ptr->SignalCode;
	sig_ptr->SignalSize = psig_ptr->SignalSize;
	sig_ptr->data = psig_ptr->data;

	SCI_FREE_SIGNAL(psig_ptr);

}
#endif
