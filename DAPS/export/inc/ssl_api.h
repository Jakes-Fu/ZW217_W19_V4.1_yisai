/*****************************************************************************
** File Name:      newsslinterface.h                                                *
** Author:         undiso.cheng                                                *
** Date:           2009.2                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    list functions                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.2     undiso.cheng         Create                                 *
******************************************************************************/

#ifndef _SSL_INTERFACE_H_
#define _SSL_INTERFACE_H_

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

//modify by qinhe for emulator compile error
#ifdef WIN32
#ifndef __FUNCTION__
#define VAL(x) #x
#define STR(x) VAL(x)
//#define __FUNCTION__ ((strrchr(__FILE__, '\\') == NULL) ? (__FILE__) : (strrchr(__FILE__, '\\') + 1))
#define __FUNCTION__  "Line"STR(__LINE__)
#endif
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SSL_ASYNC 1                  /* use async interface */
#define SSL_SYNC 0                   /*use sync interface*/
#define SSL_ALGORITHM_MAX_LENGTH 10  /*length of the algorithm's name*/
//modify for coverity, because USE_SHA512 is open
#define SSL_SHA1_HASH_LENGTH 64      /*length of sha1 hash result*/
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
//SSL return result
typedef enum 
{
    SSL_FAIL=-1,            /*ssl operate fail*/
    SSL_OK = 0,             /*ssl operate success*/
    NEWSSL_CERT_UNVERIFY=1 /*ssl encounter cert problem*/
}SSL_RESULT_E;

//ssl supported protocol type
typedef enum 
{
	SSL_PROTOCOLSSL,         /* support ssl3.0 */
	SSL_PROTOCOLTLS,         /* support tls1.0 */
	SSL_PROTOCOLTLS_1_1,  /* support tls1.1 */
	SSL_PROTOCOLTLS_1_2,  /* support tls1.2 */
	SSL_PROTOCOLBOTH          /* support tls and ssl3.0 */
} SSL_PROTOVER_E;

typedef enum 
{
   SSL_HASH_MD5 = 1,        /*md5 algorithm*/
   SSL_HASH_SHA1 = 2,       /*sha1 algorithm*/
   SSL_HASH_NULL
} SSL_HASHALGO_E;

//the message from upper layer protocol
typedef enum 
{
    SSL_RECV_MESSAGE_NULL=0,
	SSL_RECV_MESSAGE_SEND_SUCC,     /*up layer has send encrypted data succ*/
    SSL_RECV_MESSAGE_SEND_FAIL,     /*up layer has send encrypted data fail*/ 
	SSL_RECV_MESSAGE_RECV_SUCC,     /*up layer has received decrypted data succ*/
	SSL_RECV_MESSAGE_RECV_FAIL,     /*up layer has received decrypted data fail*/   
	SSL_RECV_MESSAGE_MAX =255   
} SSL_RECV_MESSAGE_E;

//the message sent to upper layer protocol
typedef enum
{
    SSL_SEND_MESSAGE_NULL =0,
    SSL_SEND_MESSAGE_SEND_END=1,            /*inform up layer the data has been end*/
    SSL_SEND_MESSAGE_HANDSHAKE_SUCC = 200,  /*inform handshake succ*/
    SSL_SEND_MESSAGE_FAIL =201,             /*inform ssl operate fail*/
    SSL_SEND_MESSAGE_CLOSE_BY_SERVER =202,  /*inform close reason--by server*/
    SSL_SEND_MESSAGE_CANCLED_BY_USER = 203, /*inform close reason--by our side*/
    SSL_SEND_MESSAGE_SET_ALPN_H1_TYPE = 204,
    SSL_SEND_MESSAGE_SET_ALPN_H2_TYPE = 205,
    SSL_SEND_MESSAGE_MAX = 255
}SSL_SEND_MESSAGE_E;
/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef uint32 SSL_HANDLE;                          // security instance
//call back function definition
typedef void (*SecurityPostMessage)(void*, uint32);                 //inform message
typedef void (*SecurityDecryptDataOutput)(void*, uint8*, uint32);   //return decrypt data to up layer
typedef int32 (*SecurityEncryptDataOutput)(void*, uint8 *, uint32); //send encrypted data out
typedef void (*SecurityShowCertInfo)(void*);                        //cert untrusted inform


//call back structure
typedef struct
{
    SecurityPostMessage postmessage_cb;
    SecurityDecryptDataOutput decryptout_cb;
    SecurityEncryptDataOutput encryptout_cb;
    SecurityShowCertInfo      showcert_cb;
}SSL_CALLBACK_T;

//issuer and subject's distinguished name attribute
typedef struct 
{
    char *country_ptr;
    char *state_ptr;
    char *locality_ptr;
    char *organization_ptr ;
    char *orgunit_ptr ;
    char *commoname_ptr;
    char *hash_ptr;
} SSL_DNATTRIBUTE_T;
//cert's info
typedef struct SSL_CERTINFO
{
    uint32  version;
    char   *serialnumber_ptr;
    SSL_DNATTRIBUTE_T issuer;
    SSL_DNATTRIBUTE_T subject;
    char *not_before_ptr ;
    char *not_after_ptr ;
    uint8 sig_algorithm[SSL_ALGORITHM_MAX_LENGTH];
    uint8 sig_hash[SSL_SHA1_HASH_LENGTH];
    char *unique_userid_ptr;  
    char *unique_subjectid_ptr ;
    struct SSL_CERTINFO *next;
} SSL_CERTINFO_T;

//Hello extension 
typedef enum
{
    SSL_HELLOEXT_NULL = 0, //default client
    SSL_HELLOEXT_ALPN_H2 = 1,
    SSL_HELLOEXT_MAX = 0xFF,
}SSL_HELLOEXT_MODE_E;

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
// 	Description : create the SSL handler
//  Param    :app_handle: the upper layer module handler
//            socket_fd: socket_id created by the upper layer
//            is_sync: flag, identifying the u
//	Global resource dependence : g_sessionlist_ptr
//  Return : the ssl handler pointer
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC SSL_HANDLE SSL_Create(void *app_handle_ptr,//[IN] 
                             long socket_fd,      //[IN]
                             BOOLEAN is_sync      //[IN]
                             );
/****************************************************************************/
// 	Description : destroy the SSL handler
//  Param    :ssl_ptr: the security instance
//            is_sync: flag, identifying the interface
//	Global resource dependence : g_sslhsmlist_ptr
//  Return : destroy success or not
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC SSL_RESULT_E  SSL_Close(SSL_HANDLE ssl_handle,  //[IN]
                                     BOOLEAN is_sync//[IN]
                                     );
#ifdef USE_CLIENT_SIDE_SSL
/****************************************************************************/
// 	Description : get all certification info
//  Param        :cert_pptr       [in][out]
//	Global resource dependence : 
//  Author: wenmei.hua
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN SSL_GetAllCertInfo(SSL_CERTINFO_T **cert_pptr);
/****************************************************************************/
// 	Description : free all memory allocated by SSL_GetAllCertInfo()
//  Param        :cert_pptr       [in][out]
//	Global resource dependence : 
//  Author: wenmei.hua
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN SSL_DestroyAllCertInfo(SSL_CERTINFO_T **cert_pptr);
#endif
/****************************************************************************/
// 	Description : to set the security instance protocol version
//  Param    :ssl_ptr: the security instance
//            security protocol :the protocol it choose.
//                              at present it only support SSL_PROTOCOLSSL 
//            is_sync: flag, identifying the interface
//	Global resource dependence : none
//  Return : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void SSL_ProtocolChoose(SSL_HANDLE ssl_handle,              //[IN]
                               SSL_PROTOVER_E security_protocol,//[IN]
                               BOOLEAN is_sync                  //[IN]
                               );

/****************************************************************************/
// 	Description : to set ExtensionInfo, just for ALPN now.
//  Param    :ssl_ptr: the security instance
//            mode:SSL_HELLOEXT_MODE_E.
//           
//	Global resource dependence : none
//  Return : none
//	Note:	
/****************************************************************************/
PUBLIC void SSL_SetExtensionMode(SSL_HANDLE ssl_handle, SSL_HELLOEXT_MODE_E mode);
/****************************************************************************/
// 	Description : to begin handshake
//  Param    :ssl_ptr: the security instance
//            authorityname_ptr :the server's authority name
//            port : the server's port
//            is_sync: flag, identifying the interface
//	Global resource dependence : none
//  Return : handshake result will return through callback
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_HandShake(SSL_HANDLE ssl_handle,        //[IN]
                                     char *authorityname_ptr,//[IN]
                                     uint16 port,            //[IN] 
                                     BOOLEAN is_sync         //[IN]
                                     );
/****************************************************************************/
// 	Description : transmit the data user received to security module
//  Param    :ssl_ptr: the security instance
//            buf_ptr :data received
//            len : data length
//	Global resource dependence : none
//  Return : the decrypted data will return through callback
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void SSL_DecryptPasser(SSL_HANDLE ssl_handle, //[IN]
                              uint8 *buf_ptr,     //[IN]
                              uint32 len          //[IN]
                              );
/****************************************************************************/
// 	Description : user can encrypt data before sending by this function
//  Param    :ssl_ptr: the security instance
//            buf_ptr :plain data to send
//            len : plain data length
//	Global resource dependence : none
//  Return : the encrypted data will be sent out through callback
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void SSL_EncryptPasser(SSL_HANDLE ssl_handle, //[IN]
                              uint8 *data_ptr,    //[IN]
                              uint32 len          //[IN]
                              );
/****************************************************************************/
// 	Description : user send message to tell ssl free buffer
//  Param    :ssl_ptr: the security instance
//            message : the message to tell ssl
//            len : length to free 
//	Global resource dependence : none
//  Return : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void SSL_AsyncMessageProc(SSL_HANDLE ssl_handle,     //[IN] 
                                 SSL_RECV_MESSAGE_E message, //[IN]
                                 uint32 len);            //[IN]
/****************************************************************************/
// 	Description : init the session list pointer 
//  Param        :
//	Global resource dependence : s_sessionlist_ptr & g_serverhashlist_ptr
//  Return : initial variable success or fail
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_Init(void);
/****************************************************************************/
// 	Description : free the session list pointer 
//  Param        :
//	Global resource dependence : g_sessionlist_ptr
//  Return : free pointer success or fail
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_Destroy(void);
/****************************************************************************/
// 	Description : register callbacks to let the user receive info
//  Param        :ssl_ptr        security instance
//                cb             callback functions
//	Global resource dependence : none
//  Return : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void SSL_AsyncRegCallback(SSL_HANDLE ssl_handle,     //[IN]
                                 const SSL_CALLBACK_T *cb_ptr);//[IN]
/****************************************************************************/
// 	Description : file encryption
//  Param    :ssl_ptr: the security instance
//            file_handle : file handle
//	Global resource dependence : none
//  Return :encrypted file will be sent through callback
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void SSL_EncryptPasserFile(SSL_HANDLE ssl_handle,//[IN]
                                  uint16 *file_name);//[IN]
/****************************************************************************/
// 	Description : pass user's decision on cert to security module
//  Param    :ssl_ptr: the security instance
//            file_handle : file handle
//	Global resource dependence : none
//  Return : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC void SSL_UserCnfCert(SSL_HANDLE ssl_handle,  //[IN]
                            BOOLEAN is_continue);//[IN]
/****************************************************************************/
// 	Description :Parse the certificate infomation 
//  Param    :ssl_ptr: machine instance
//            cert_ptr: the wanted cert info
//	Global resource dependence : 
//  Return : none
//  Author: undiso.cheng
//	Note:after get cert info through this api,user should call SSL_FreeCertInfo
//       to free cert buffer
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_GetCurrentCertInfo(SSL_HANDLE ssl_handle,          //[IN]
                                              SSL_CERTINFO_T **cert_ptr //[IN]
                                              );
/****************************************************************************/
// 	Description :Free Certificate information struct resource 
//  Param    :cert_ptr: the cert info to free      
//	Global resource dependence : 
//  Return : none
//  Author: undiso.cheng
//	Note:	cert should set to NULL after call this interface
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_FreeCurrentCertInfo(SSL_CERTINFO_T *cert_info_ptr);//[IN]
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif //#ifdef _NEWSSL_INTERFACE_H
