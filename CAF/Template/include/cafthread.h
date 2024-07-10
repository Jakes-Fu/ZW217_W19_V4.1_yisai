/****************************************************************************
** File Name:      cafTHREAD.h                                            *
** Author:         songliang¡£liu                                             *
** Date:           03/11/2009                                              *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        songliang.liu       Create
** 
****************************************************************************/

#ifndef  _CAFTHREAD_H_    
#define  _CAFTHREAD_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "caf.h"
#include "cafguid_def.h"




/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

typedef struct ITHREAD_T        ITHREAD_T;


/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/


typedef struct
{
	uint16 SignalCode; 
    uint16 SignalSize; 
	int32 data;
} CAF_THREAD_SIG_T;




#define SIG_ITHREAD_USER_START 0x8000
#define MSG_ITHREAD_USER_START (MSG_KERNEL_START + 1200)




// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(ITHREAD_T)
{
    INHERIT_IUNKNOWN(ITHREAD_T);
    void (*SetParam ) ( ITHREAD_T *me_ptr ,  uint32 stack_size,	void (*thread_proc)(uint32 argc,  void* argv), uint32 argc,  void* argv );
  	void (*Start) (ITHREAD_T *me_ptr );  
  	void (*PostSignalToThread) ( ITHREAD_T *me_ptr , ITHREAD_T *des_thread_ptr , void * data_ptr, uint16 signal_code ,uint16 signal_size );
	void (*PostSignalToApplet) (ITHREAD_T *me_ptr , CAF_HANDLE_T  applet_handle,void * data_ptr , uint16 signal_code ,uint16 signal_size );
	void (*GetSignal) (ITHREAD_T *me_ptr ,CAF_THREAD_SIG_T *sig_ptr);
};


// Define pointer to poit the virtual table of the control base class
struct ITHREAD_T
{
    CAF_VTBL_T(ITHREAD_T)  *vtbl_ptr;
};





static __inline CAF_RESULT_E ITHREAD_QueryInterface( ITHREAD_T *me_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    return CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->QueryInterface( me_ptr, guid, object_pptr );
}

static __inline uint32 ITHREAD_AddRef( ITHREAD_T *me_ptr ) 
{
    return CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->AddRef( me_ptr );
}

static __inline uint32 ITHREAD_Release( ITHREAD_T *me_ptr ) 
{
    return CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->Release( me_ptr );
}


static __inline void ITHREAD_SetParam ( ITHREAD_T *me_ptr , uint32 stack_size,	void (*thread_proc)(uint32 argc,  void* argv),uint32 argc,  void* argv )
{
 	CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->SetParam( me_ptr,stack_size, thread_proc ,argc, argv);
}
 
static __inline 	void ITHREAD_Start ( ITHREAD_T *me_ptr )
{
 	CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->Start( me_ptr );
}

static __inline void ITHREAD_PostSignalToThread ( ITHREAD_T *me_ptr , ITHREAD_T *des_thread_ptr , void * data_ptr , uint16 signal_code ,uint16 signal_size)
{
 		CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->PostSignalToThread( me_ptr,des_thread_ptr ,data_ptr,signal_code,signal_size);
}

static __inline void ITHREAD_PostSignalToApplet (ITHREAD_T *me_ptr , CAF_HANDLE_T  applet_handle,void * data_ptr  ,uint16 signal_code ,uint16 signal_size)
{
 	CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->PostSignalToApplet( me_ptr,applet_handle ,data_ptr,signal_code,signal_size);
}

static __inline void  ITHREAD_GetSignal(ITHREAD_T *me_ptr ,CAF_THREAD_SIG_T *sig_ptr)
{
	CAF_GET_VTBLPTR( me_ptr, ITHREAD_T )->GetSignal( me_ptr ,sig_ptr );
}




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFITHREAD_H_
