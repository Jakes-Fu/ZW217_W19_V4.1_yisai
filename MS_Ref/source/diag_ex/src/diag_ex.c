/******************************************************************************
 ** File Name:      diag_ex.c                                           *
 ** Author:         gloria.he                                              *
 ** DATE:           2015.07.06                                                *
 ** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2015.07.09    gloria.he         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "cmddef.h"
#include "diag_ex.h"
#include "ref_outport.h"
#include "diag.h"



/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
//#define AUTOTEST_DEBUG

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

typedef enum  _DIAG_OPERATION_E
{
    XFILE_OPER = 0,        
    SIMLOCK_OPER = 1,  
    WNS_OPER = 2,      
    WNS_KEY_OPER = 3,  
    DEVICE_OPER = 4, 
    BSC_OPER = 5,
    WNS_SELFTEST_OPER =6, 
    RESET_OPER = 7,
    RESET_DOWNLOAD = 8,
    DIAG_MAX_F
}DIAG_OPERATION_E;


/*******************************************************************************
 **     static variable
 *******************************************************************************/
LOCAL DIAG_EX_CALLBACK s_diag_ex_callback[DIAG_MAX_F] = {NULL};


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 #if defined MS_SIMLOCK_SUPPORT
extern uint32 HandleSimlockReq(uint8       **dest_ptr,
                           uint16       *dest_len_ptr,
                           const uint8  *src_ptr,
                           uint16        src_len);
 #endif
#if defined SECURITY_DATA_SUPPORT
 extern uint32 HandleSecurityDataWriteReq(uint8       **dest_ptr,
                           uint16       *dest_len_ptr,
                           const uint8  *src_ptr,
                           uint16        src_len);
 #endif                          
 
#if defined MS_DEVICE_INFO_SUPPORT
extern uint32	HandleDeviceInfoReq(uint8       **dest_ptr,
                           uint16       *dest_len_ptr,
                           const uint8  *src_ptr,
                           uint16        src_len);
#endif

#if defined MS_WNS_SUPPORT
extern void HandleWNS_Certificate_Req(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);      // Size of the source buffer in uint8.

extern void HandleWNS_Private_Key_Req(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)  ;       // Size of the source buffer in uint8.
    
extern void HandleWNS_Selftest_Req(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len);         // Size of the source buffer in uint8.
#endif



/*****************************************************************************/
//  Description:    To Handle Diag command request for reset into download mode. 
//  Global resource dependence:
//  Author:         maolong.mei
//  Note:
/*****************************************************************************/
LOCAL void HandleResetDownLoad(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
    MSG_HEAD_T  *head_ptr = src_ptr;
    MSG_HEAD_T  *respond_ptr = NULL;

    if (head_ptr->subtype == RESET_DOWNLOAD) {
        respond_ptr =(uint8 *)SCI_ALLOC_APP(sizeof(MSG_HEAD_T));
        SCI_ASSERT(respond_ptr != SCI_NULL);
        SCI_MEMCPY(respond_ptr, head_ptr, sizeof(MSG_HEAD_T));
        respond_ptr->subtype = 0;  //O: SUCCESS 1:FAULT.
        SIO_SendPacket(respond_ptr, sizeof(MSG_HEAD_T));
        OS_TickDelay(500);
        SCI_FREE(respond_ptr);
        osiReset(OSI_RESET_FORCE_DOWNLOAD);
        SCI_PASSERT(0, ("RESET FOR DOWNLOAD FAIL!\n"));
    }
}

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

PUBLIC void Register_Ex_CmdRoutine (void)
{
    SCI_TRACE_LOW ("diag: Register Cmd Routine");
	
	//you can register your callback here
    DIAG_InitExCallback();
	
#if defined MS_SIMLOCK_SUPPORT
    DIAG_RegisterExCallback( SIMLOCK_OPER, HandleSimlockReq);
#endif
#if defined SECURITY_DATA_SUPPORT
    DIAG_RegisterExCallback( BSC_OPER, HandleSecurityDataWriteReq);
#endif
#if defined MS_DEVICE_INFO_SUPPORT
  DIAG_RegisterExCallback( DEVICE_OPER, HandleDeviceInfoReq); 
#endif
#if defined MS_WNS_SUPPORT
	DIAG_RegisterExCallback( WNS_OPER, HandleWNS_Certificate_Req);
    DIAG_RegisterExCallback( WNS_KEY_OPER, HandleWNS_Private_Key_Req);
	DIAG_RegisterExCallback(WNS_SELFTEST_OPER,HandleWNS_Selftest_Req);
#endif



    DIAG_RegisterExCallback( RESET_DOWNLOAD, HandleResetDownLoad);

    DIAG_RegisterCmdRoutine ( DIAG_CARETOOL_F,  HandleDiagEx);
}

/*****************************************************************************/
//  Description:    this function is used to init the diag's callback func array.
//                                  
//	Global resource dependence:               
//  Author:        
//  Note:           
/*****************************************************************************/

PUBLIC void DIAG_InitExCallback( void )
{
    int  type = 0;
    
    for( type = 0; type < DIAG_MAX_F; type++ )
    {
        s_diag_ex_callback[ type ] = NULL;
    }
}
/*****************************************************************************/
//  Description:    this function is used to register the diag's callback func.
//                                  
//	Global resource dependence:               
//  Author:         
//  Note:           
/*****************************************************************************/

PUBLIC void DIAG_RegisterExCallback( uint32  type,  DIAG_EX_CALLBACK  routine )
{    
    if( type < DIAG_MAX_F )
        s_diag_ex_callback[ type ] = routine;
}

/*****************************************************************************/
//  Description:    this function is used to unregister the diag's callback func.
//                                  
//	Global resource dependence:                              
//  Author:         
//  Note:           
/*****************************************************************************/

PUBLIC void DIAG_UnRegisterExCallback( uint32  type )
{
    if( type < DIAG_MAX_F )
        s_diag_ex_callback[ type ] = NULL;
}


/*****************************************************************************/
//  Description:    this function is used to handle the diag's callback func.
//                                  
//	Global resource dependence:                              
//  Author:        
//  Note:           
/*****************************************************************************/

LOCAL uint32 HandleDiagEx( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    uint8        cmd_subtype;
    MSG_HEAD_T  *msg_head;

    msg_head    = (MSG_HEAD_T *) src_ptr;
    cmd_subtype = msg_head->subtype;

    if( cmd_subtype < DIAG_MAX_F )
    {
        if( NULL != s_diag_ex_callback[cmd_subtype] )
        {
            if( s_diag_ex_callback[cmd_subtype]( dest_ptr, dest_len_ptr, src_ptr, src_len ) )// has been handled.
            {
                return  1;
            }
        }
    }
    return 0;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
