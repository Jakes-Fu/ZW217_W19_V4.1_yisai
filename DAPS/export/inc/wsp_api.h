/*****************************************************************************
** File Name:      wsp_api.h                                                 *
** Author:         kelly.li                                                  *
** Date:           04/24/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the WSP interface  struct   *
*****************************************************************************/

#ifndef _WSP_API_H_
#define _WSP_API_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "http_api.h"

#ifdef WTLS_ENABLE
#include "wtls_interface.h"
#endif
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
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*ATTENTION: all the pointers can be free after using the relative interface,*/
/*except the buffer_ptr in HTTP_POST_BUFFER_T. if the is_copied_by_http in   */
/*HTTP_POST_BUFFER_T is set TRUE, the pointer of buffer_ptr can be free after*/
/*using the relative interface, if the is_copied_by_http in                  */
/*HTTP_POST_BUFFER_T is set FALSE, the pointer of buffer_ptr can not be free */
/*until the app has received the HTTP_SIG_POST_CNF signal                    */
/*---------------------------------------------------------------------------*/

typedef struct _WSP_PROXY
{
    char*   user_name_ptr;  //the proxy user name		
    char*   password_ptr;	//the proxy password	
    char*   proxy_host_ptr;	//the proxy ip address, example: 10.0.0.172
    uint16  wsp_port;		//the http proxy port, example: 9201	
   // uint16  wtls_port;			
}WSP_PROXY_T;

typedef struct _WSP_POST_PARAM
{
    HTTP_URI_T                      uri;                //the resource uri
    HTTP_ACCEPT_HEADER_T            accept;             //accept header value
    HTTP_ACCEPT_CHARSET_HEADER_T    accept_charset;     //accept charset header value
    HTTP_ACCEPT_LANGUAGE_HEADER_T   accept_language;    //accept language header value  
    HTTP_USER_AGENT_HEADER_T        user_agent;         //user agent header value
    HTTP_UA_PROFILE_HEADER_T        ua_profile;         //ua profile header value
    HTTP_REFERER_HEADER_T           referer;            //referer header value
    HTTP_EXTERN_HEADER_T            extern_header;      //extern header
    HTTP_AUTH_PARAM_T               auth;               //auth param      
    HTTP_POST_BODY_T                post_body;          //post body: file or buffer
    HTTP_ENCODING_TYPE_E            accept_encoding;    //accept encoding header value
    BOOLEAN                         need_net_prog_ind;  //if the app need net progress indication, set this flag TRUE
}WSP_POST_PARAM_T;


typedef struct _WSP_GET_PARAM
{
    HTTP_URI_T                      uri;                //the resource uri
    HTTP_ACCEPT_HEADER_T            accept;             //accept header value
    HTTP_ACCEPT_CHARSET_HEADER_T    accept_charset;     //accept charset header value
    HTTP_ACCEPT_LANGUAGE_HEADER_T   accept_language;    //accept language header value
    HTTP_USER_AGENT_HEADER_T        user_agent;         //user agent header value
    HTTP_UA_PROFILE_HEADER_T        ua_profile;         //ua profile header value
    HTTP_REFERER_HEADER_T           referer;            //referer header value
    HTTP_EXTERN_HEADER_T            extern_header;      //extern header
    HTTP_AUTH_PARAM_T               auth;               //auth param       
    HTTP_ENCODING_TYPE_E            accept_encoding;    //accept encoding header value
    BOOLEAN                         need_net_prog_ind;  //if the app need net progress indication, set this flag TRUE
}WSP_GET_PARAM_T;

typedef struct _WSP_INIT_PARAM
{
    WSP_PROXY_T     proxy;                                      //the proxy param
	uint32			net_id;										//the net id for socket binding
    BOOLEAN         is_cache_enable;                            //if enable cache, if this flag is set TRUE, the cache_path must be set
    BOOLEAN         is_cookie_enable;                           //if enable cookie, if this flag is set TRUE, the cookie_file_name must be set
    HTTP_UNICODE_T  cache_path[HTTP_MAX_FILE_NAME_LEN+1];       //cache path info
    HTTP_UNICODE_T  cookie_file_name[HTTP_MAX_FILE_NAME_LEN+1]; //cookie path info
}WSP_INIT_PARAM_T;

//message struct define begin
typedef struct _WSP_INIT_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_APP_MODULE_ID_T    module_id;      //module id of app
    HTTP_APP_PARAM_T    	app_param;      //app param
    WSP_INIT_PARAM_T     	init_param;     //init parameter that app set
}WSP_INIT_REQ_SIG_T;

typedef struct _WSP_GET_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T   context_id;         //context id that wsp assigned for app
    WSP_GET_PARAM_T     get_param;          //get parameter that app set
    HTTP_APP_INSTANCE_T	app_instance;       //the app instance
}WSP_GET_REQ_SIG_T;

typedef struct _WSP_POST_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T    context_id;        //context id that wsp assigned for app
    WSP_POST_PARAM_T     post_param;        //post parameter that app set
    HTTP_APP_INSTANCE_T  app_instance;      //the app instance
}WSP_POST_REQ_SIG_T;
//message struct define end

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init WSP session request
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_InitRequest(
			                    HTTP_APP_MODULE_ID_T    module_id,      //[IN]
			                    HTTP_APP_PARAM_T        app_param,      //[IN]
			                    const WSP_INIT_PARAM_T* init_param_ptr  //[IN]
			                    );

/*****************************************************************************/
//  Description : get WSP data request
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_GetRequest(
                            HTTP_CONTEXT_ID_T       context_id,     //[IN]
                            const WSP_GET_PARAM_T*  get_param_ptr,  //[IN]
                            HTTP_APP_INSTANCE_T     app_instance    //[IN]
                            );

/*****************************************************************************/
//  Description : post WSP data request
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_PostRequest(
                                HTTP_CONTEXT_ID_T          context_id,      //[IN]
                                const WSP_POST_PARAM_T*    post_param_ptr,  //[IN]
                                HTTP_APP_INSTANCE_T        app_instance     //[IN]
                                );

/*****************************************************************************/
//  Description : create and send auth signal
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_AuthResponse(
                            HTTP_CONTEXT_ID_T         context_id,       //[IN]
                            const HTTP_AUTH_PARAM_T*  auth_param_ptr,   //[IN]
                            HTTP_APP_INSTANCE_T       app_instance,     //[IN]
                            HTTP_REQUEST_ID_T         request_id        //[IN]
                            );

/*****************************************************************************/
//  Description : close WSP request
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_CloseRequest(
                                     HTTP_CONTEXT_ID_T context_id   //[IN]
                                     );

/*****************************************************************************/
//  Description : cancel WSP request
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_CancelRequest(
                                HTTP_CONTEXT_ID_T   context_id,     //[IN]
                                HTTP_APP_INSTANCE_T app_instance,   //[IN]
                                HTTP_REQUEST_ID_T   request_id      //[IN]
                                );

/*****************************************************************************/
//  Description : clear wsp cookie
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
//PUBLIC HTTP_ERROR_E WSP_CookieClear(
//                                   const HTTP_UNICODE_T* cookie_file_name_ptr //[IN]
//                                   );

/*****************************************************************************/
//  Description : clear wsp cookie
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_CookieClear(
                                   HTTP_CONTEXT_ID_T context_id, //[IN]
                                   const HTTP_UNICODE_T* cookie_file_name_ptr
                                   );

/*****************************************************************************/
//  Description : clear wsp cache
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_CacheClear(
                                   const HTTP_UNICODE_T* cache_file_name_ptr //[IN]
                                   );

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : create and send wtls cert signal
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E WSP_WtlsCertResponse(
                                        HTTP_CONTEXT_ID_T         context_id,   //[IN]
                                        HTTP_APP_INSTANCE_T       app_instance, //[IN]
                                        HTTP_REQUEST_ID_T         request_id,   //[IN]
                                        BOOLEAN                   is_trust_cert //[IN]
                                        );
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif