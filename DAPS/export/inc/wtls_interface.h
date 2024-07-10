/*****************************************************************************
** File Name:      wtls_interface.h                                          *
** Author:         undiso.cheng                                              *
** Date:           2009.2                                                    *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    list functions                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.2     undiso.cheng         Create                                    *
******************************************************************************/

#ifndef _WTLS_INTERFACE_H_
#define _WTLS_INTERFACE_H_

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
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
//WTLS return result
typedef enum 
{
    WTLS_RESULT_OK = 0,            /*WTLS operate fail*/
    WTLS_RESULT_FAIL = 1,          /*WTLS operate success*/
    WTLS_RESULT_MAX
}WTLS_RESULT_E;


//the message sent to upper layer protocol
typedef enum
{
    WTLS_POSTMESSAGE_NULL              = 0,
    WTLS_POSTMESSAGE_CERT_NEED_CONFIRM = 1,    /*inform uplayer to confirm cert*/
    WTLS_POSTMESSAGE_HANDSHAKE_SUCC    = 200,  /*inform handshake succ*/
    WTLS_POSTMESSAGE_FAIL              = 201,  /*inform WTLS operate fail*/
    WTLS_POSTMESSAGE_CLOSE_BY_SERVER   = 202,  /*inform close reason--by server*/
    WTLS_POSTMESSAGE_CANCLED_BY_USER   = 203,  /*inform close reason--by our side*/
    WTLS_POSTMESSAGE_MAX               = 255
}WTLS_POSTMESSAGE_E;

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef uint32 WTLS_HANDLE;  // security instance
typedef uint32 WTLS_IP;

//call back function definition
typedef void (*WtlsPostMessage)(void*, uint32);  //inform message
typedef void (*WtlsInternalSend)(void*, uint8 *, uint32);//send wtls internal data

//call back structure
typedef struct
{
    WtlsPostMessage   postmessage_cb;
    WtlsInternalSend  internalsend_cb;
}WTLS_CALLBACK_T;

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
// 	Description : create the WTLS handler
//	Global resource dependence : 
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_HANDLE WTLS_Create(                      //the WTLS handler pointer
							   void *app_handle_ptr  //[IN]the upper layer module handler
							  );

/****************************************************************************/
// 	Description : destroy the WTLS handler
//	Global resource dependence : 
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_RESULT_E  WTLS_Destroy(                        //destroy success or not
								   WTLS_HANDLE wtls_handle //[IN]the security instance
								  );

/****************************************************************************/
// 	Description : to begin handshake
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_RESULT_E WTLS_HandShake(
									WTLS_HANDLE wtls_handle,  //[IN]the security instance
                                    WTLS_IP     authority_ip, //[IN]the server's ip address
                                    uint16      port          //[IN]the server's port
									);

/****************************************************************************/
// 	Description : transmit the data user received to security module
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: user will check both WTLS_RESULT_E, *dest_ptr and dest_len together to
//        decide action. if dest_len = 0, then app can ignore this result.
//        result will indicate interface error, wtls internal error will return
//        through postmessage callback.
//        if WTLS_RESULT_E == WTLS_RESULT_OK, and dest_len == 0, the data may 
//        be wtls alert or handshaking data
//        if WTLS_RESULT_E == WTLS_RESULT_FAIL, means interface encounter error
//        upperlayer should destroy this connection   
/****************************************************************************/
PUBLIC WTLS_RESULT_E WTLS_DecryptPasser(
										WTLS_HANDLE wtls_handle,  //[IN]the security instance's id
										uint8 *buf_ptr,           //[IN]data received
										uint32 len,               //[IN]data length
										char **dest_ptr,          //[OUT]the plaintext return to app
										uint32 *dest_len          //[OUT]the length of the plaintext
									   );

/****************************************************************************/
// 	Description : user can encrypt data before sending by this function
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	user will check both WTLS_RESULT_E,*dest_ptr and dest_len together
//          to decide action. if dest_len = 0 then error happens. 
//			WTLS_RESULT_E will indicate interface error, wtls internal error 
//          will return through postmessage callback.
//          if WTLS_RESULT_E == WTLS_RESULT_OK, and dest_len == 0, error happens 
//          in wtls internal, and error will inform through postmessage_cb.
//          if WTLS_RESULT_E == WTLS_RESULT_FAIL, means interface encounter error
//          upper layer should destroy this connection   
/****************************************************************************/
PUBLIC WTLS_RESULT_E WTLS_EncryptPasser(                           
										WTLS_HANDLE wtls_handle,   //[IN]the security instance
										uint8 *data_ptr,           //[IN]plain data to send
										uint32 len,                //[IN]plain data length
										char **dest_ptr,           //[OUT]the ciphertext 
										uint32 *dest_len           //[OUT]the length of the ciphertext
									   );

/****************************************************************************/
// 	Description : free the session list pointer 
//	Global resource dependence : s_wtlshsm_list
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_RESULT_E WTLS_Finalize(void);

/****************************************************************************/
// 	Description : register callbacks 
//	Global resource dependence : 
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_RESULT_E WTLS_RegCallback(
									  WTLS_HANDLE wtls_handle,      //[IN]security instance
									  const WTLS_CALLBACK_T *cb_ptr //[IN]callback functions
									 );

/****************************************************************************/
// 	Description : pass user's decision on cert to security module
//	Global resource dependence :
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void WTLS_UserCnfCert(
							 WTLS_HANDLE wtls_handle,  //[IN]the security handle
							 BOOLEAN is_continue       //[IN]TRUE ok, FALSE cancel
							);
							
/****************************************************************************/
// 	Description : init the session list pointer 
//	Global resource dependence : s_sessioninfo & s_wtlshsm_list
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_RESULT_E WTLS_Init(void); //initial variable success or fail

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif //#ifdef _NEWWTLS_INTERFACE_H
