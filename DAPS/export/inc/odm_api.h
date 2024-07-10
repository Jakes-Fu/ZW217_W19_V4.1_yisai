/*****************************************************************************
** File Name:      odm.h				                                     *
** Author:                                                                   *
** Date:           12/17/2007                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    OMA DOWNLOAD OTA v1.0: OMA Download Manager				 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/17/2007     shulin.chang     Create									 *	
******************************************************************************/

#ifndef _ODM_H_
#define _ODM_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"

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
// internet data encoding type
#define ODM_ENCODING_UTF8		0
#define ODM_ENCODING_GB2312		1
#define ODM_ENCODING_INVALID	255

// connection type
#define ODM_CONNECTION_TYPE_HTTP	0
#define ODM_CONNECTION_TYPE_WSP	1
#define ODM_CONNECTION_TYPE_MAX 2
#define ODM_CONNECTION_INVALID	255

// error no
#define ODM_SUCCESS 						 0
#define ERR_ODM_FAILED 						-1
#define ERR_ODM_OUT_OF_MEMORY				-2
#define ERR_ODM_CONN_NOT_SUPPORTTED			-3
#define ERR_ODM_CONN_ERROR					-4
#define ERR_ODM_CONN_DOWNLOAD_DD_ERROR		-5
#define ERR_ODM_CONN_DOWNLOAD_MO_ERROR		-6
#define ERR_ODM_XML_PARSE_FAILED			-7
#define ERR_ODM_INSTALL_MO_FAILED			-8
#define ERR_ODM_ENCODING_NOT_SUPPORT		-9	
#define ERR_ODM_INVALID_PARAM				-10		// Message param invalid
#define ERR_ODM_CANNOT_CANCEL_WHEN_DOWNLOAD	-11
#define ERR_ODM_CONN_REPORT_ERROR			-12
#define ERR_DD_TYPE_TOO_MORE				-13	// there is too more type field in download description
#define ERR_DD_OBJECT_URI_TOO_LONG			-14	// object_uri is too long than we can save it
#define ERR_DD_INSTALL_NOFIFY_URI_TOO_LONG	-15	// install_notify_uri is too long than we can save it		
#define ERR_INVALID_PARAMETER				-16	// function call's with invalid parameters
#define ERR_XML_PARSER_ERROR				-17	// XML parse error		
#define ERR_DD_INSTALL_PARAM_TOO_LONG		-18	// install param too long		
#define ERR_ODM_DOWNLOAD_SESSION_NOT_EXIST	-19	// session not exist		
#define ERR_ODM_READ_FILE_FAILED			-20	// read file failed
#define ERR_ODM_CONN_DOWNLOAD_ERROR			-21	// download file error
#define ERR_ODM_CANNOT_CANCEL_WHEN_POST		-22			
#define ERR_ODM_SESSION_NOT_EXIST			-23
#define ERR_ODM_HAS_SUBSCRIBED				-24
#define ERR_ODM_SUBSCRIBE_FAILED			-25
#define ERR_ODM_NOT_SUBSCRIBED				-26
#define ERR_ODM_DESUBSCRIBE_FAILED			-27
#define ERR_ODM_SERVER_RESPONSE_ERROR		-28	// report send to server and server response an error, it is not HTTP 1.1 200 OK
#define ERR_DD_MIME_TYPE_TOO_LONG           -29 // mime type too long
#define ERR_DD_SIZE_TOO_LONG                -30 // mime size too long
#define ERR_ODM_NO_MEMORY                   -31 // alloc memory fail
        
// OMA DOWNLOAD ERROR NO
#define ERR_ODM_STORAGE_INSUFFICENT 		-901
#define ERR_ODM_USER_CANCELED				-902
#define ERR_ODM_LOSS_OF_SERVICE				-903
#define ERR_ODM_ATTRIBUTE_MISMATCH			-905
#define ERR_ODM_INVALID_DD					-906
#define ERR_ODM_INVALID_DD_VERSION			-951
#define ERR_ODM_INVALID_DEVICE_ABORTED		-952
#define ERR_ODM_NON_ACCEPTABLE_CONTENT		-953
#define ERR_ODM_LOADER_ERROR				-954

// Download User Agent Stage
#define ODM_DUA_STAGE_INIT				1
#define ODM_DUA_STAGE_DOWNLOAD_DD		2
#define ODM_DUA_STAGE_PARSE_DD			3
#define ODM_DUA_STAGE_CAPABILITY_CHECK	4
#define ODM_DUA_STAGE_DOWNLOAD_MO		5
#define ODM_DUA_STAGE_INSTALL_MO		6
#define ODM_DUA_STAGE_REPORT			7
#define ODM_DUA_STAGE_COMPLETED			8

// unicode char define
typedef uint16 ODM_WCHAR;

// max number of "type" in download description which we support
#define ODM_DD_TYPE_MAX_NUM 2

// max subscriber which we support
#define ODM_SUBSCRIBE_COUNT 5
		
// invalid subscribe identify
#define ODM_INVALID_SUBSCRIBE SCI_INVALID_BLOCK_ID

// OMA download description type definition, detail is in OMA download OTA spec v1.0
#define ODM_DD_SCHEMA_SHORT_STRING 40
#define ODM_DD_SCHEMA_LONG_STRING 160
#define ODM_DD_SCHEMA_VERY_LONG_STRING 256
#define ODM_DD_SCHEMA_URI 512

/*
 It is used for define identical signal
 we will move all signal to a header file later
 */
#define TASK_OMA_DOWNLOAD_MANAGER 39	

// Signal code list of OMA download manager
// ODM: OMA DOWNLOAD MANAGER

#define ODM_USER_NAME_BUFFER_SIZE  16
#define ODM_PASSWORD_BUFFER_SIZE   16
#define ODM_FILE_FULL_PATH_MAX_LEN   SFS_MAX_PATH

/*---------------------------------------------------------------------------*/
/*                          Enum			                                 */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          Struct				                             */
/*---------------------------------------------------------------------------*/
// download description basic content 
// add 1 for string's null terminated
typedef struct
{	
	ODM_WCHAR name[ODM_DD_SCHEMA_SHORT_STRING+1];	// Media object name					
	char type[ODM_DD_TYPE_MAX_NUM][ODM_DD_SCHEMA_SHORT_STRING+1];	// Media object mime type	
	uint32 size;	// media object size	
	char object_uri[ ODM_DD_SCHEMA_URI+1];			// media object uri	
	char object_install_uri[ ODM_DD_SCHEMA_URI+1];	// report uri	
	char dd_version[ODM_DD_SCHEMA_SHORT_STRING+1];	// dd version		
	ODM_WCHAR description[ODM_DD_SCHEMA_LONG_STRING+1];	// media object description		
	ODM_WCHAR vendor[ODM_DD_SCHEMA_SHORT_STRING+1];		// media object vendor	
	ODM_WCHAR install_param[ ODM_DD_SCHEMA_VERY_LONG_STRING+1];	// media object install param
	char next_url[ ODM_DD_SCHEMA_URI+1];	// next url after download completed	
	char info_uri[ ODM_DD_SCHEMA_URI+1];	// info about media object
	char icon_uri[ ODM_DD_SCHEMA_URI+1];	// icon about media object
} ODM_DOWNLOAD_DESCRIPTION_T;



/*---------------------------------------------------------------------------*/
/*                          Message				                             */
/*---------------------------------------------------------------------------*/
typedef struct _ODM_PROXY
{
    BOOLEAN is_use_proxy;   //if use proxy
    char*   user_name_ptr;  //the proxy user name
    char*   password_ptr;   //the proxy password
    char*   proxy_host_ptr; //the proxy ip address, example: 10.0.0.172
    uint32  proxy_port;     //the http proxy port, example: 80
}ODM_PROXY_T;

typedef struct {
	uint8           is_dd_url;		    // content is url or download description content	
	uint8           content_encoding;	// utf8 or other
	uint8           connection_type;	// HTTP or WSP
	uint8           padding;
	uint32          length;			    // content's length
	uint8 *         content;			// content
	uint32          magic;			    // use magic in cnf to identify different request
    ODM_PROXY_T     proxy_info;         // http/wsp proxy info
	uint32	        net_id;			    // the net id for socket binding
    ODM_WCHAR	    dd_file_name[ODM_FILE_FULL_PATH_MAX_LEN + 1];   //if need omadl to download dd first,APP should assign location for saving dd
} ODM_START_DOWNLOAD_PARAM_T;

// ODM_START_DOWNLOAD_REQ
typedef struct {
	_SIGNAL_VARS			        // header part
    ODM_START_DOWNLOAD_PARAM_T      start_param;
} ODM_START_DOWNLOAD_REQ_MSG_T;

// ODM_START_DOWNLOAD_CNF
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	int32 result;		// ODM_SUCCESS: start success, otherwise failed
	// use magic in cnf to identify different request, the value same as request
	uint32 magic;		
} ODM_START_DOWNLOAD_CNF_MSG_T;


// ODM_CANCEL_DOWNLOAD_REQ
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
} ODM_CANCEL_DOWNLOAD_REQ_MSG_T;

// ODM_CANCEL_DOWNLOAD_CNF
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	int32 result;		// ODM_SUCCESS: start success, ERR_ODM_FAILED: failed
} ODM_CANCEL_DOWNLOAD_CNF_MSG_T;


// ODM_DOWNLOAD_CONFIRMATION_IND
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	ODM_DOWNLOAD_DESCRIPTION_T basic_dd_content;
} ODM_DOWNLOAD_CONFIRMATION_IND_MSG_T;


// ODM_DOWNLOAD_CONFIRMATION_RSP
typedef struct {
	uint32 session_id;
	BOOLEAN is_user_allowed;	// TRUE: download, else canceled
    uint8 padding;
	uint16 padding2;
    ODM_WCHAR file_name[ODM_FILE_FULL_PATH_MAX_LEN+1];	// APP assigned file name 
    uint16    file_name_len;            //the unicode length		
} ODM_DOWNLOAD_CONFIRMATION_PARAM_T;

typedef struct {
	_SIGNAL_VARS				            // header part
    ODM_DOWNLOAD_CONFIRMATION_PARAM_T       rsp_param; 
} ODM_DOWNLOAD_CONFIRMATION_RSP_MSG_T;

// ODM_DOWNLOAD_AUTH_IND
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	uint32 auth_type;	// auth type 
} ODM_DOWNLOAD_AUTH_IND_MSG_T;

// ODM_DOWNLOAD_HEADER_IND
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	char*  header_ptr;	// http header  
} ODM_DOWNLOAD_HEADER_IND_MSG_T;

// ODM_DOWNLOAD_AUTH_RSP
typedef struct {
	uint32 session_id;
	uint32 auth_type;	// auth type
	// user input user name, if NULL indicate user canceled
	char user_name [ODM_USER_NAME_BUFFER_SIZE];
	// password
	char password [ODM_PASSWORD_BUFFER_SIZE];
} ODM_DOWNLOAD_AUTH_RSP_PARAM_T;

typedef struct {
	_SIGNAL_VARS		                // header part
    ODM_DOWNLOAD_AUTH_RSP_PARAM_T       auth_rsp_param;
} ODM_DOWNLOAD_AUTH_RSP_MSG_T;

// ODM_DOWNLOAD_PROGRESS_IND
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	uint32 stage;		    // download stage
    uint32 content_len;     //the total length of content
    uint32 current_len;     //current length
} ODM_DOWNLOAD_PROGRESS_IND_MSG_T;

typedef ODM_DOWNLOAD_PROGRESS_IND_MSG_T ODM_MANAGER_PROGRESS_IND_MSG_T;


// ODM_DOWNLOAD_SUCCESS_IND
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
} ODM_DOWNLOAD_SUCCESS_IND_MSG_T;

typedef ODM_DOWNLOAD_SUCCESS_IND_MSG_T ODM_MANAGER_SUCCESS_IND_MSG_T;
	

// ODM_DOWNLOAD_ERROR_IND
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	int32 error_no;		// error no
} ODM_DOWNLOAD_ERROR_IND_MSG_T;

typedef ODM_DOWNLOAD_ERROR_IND_MSG_T ODM_MANAGER_ERROR_IND_MSG_T;

// ODM_DOWNLOAD_GET_SESSION_LIST_REQ
typedef struct {
	_SIGNAL_VARS		// header part
} ODM_DOWNLOAD_GET_SESSION_LIST_REQ_MSG_T;


// ODM_DOWNLOAD_GET_SESSION_LIST_CNF
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 count;
	uint32 session_id_arry[1];	
} ODM_DOWNLOAD_GET_SESSION_LIST_CNF_MSG_T;


// ODM_DOWNLOAD_GET_SESSION_INFO_REQ
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
} ODM_DOWNLOAD_GET_SESSION_INFO_REQ_MSG_T;


// ODM_DOWNLOAD_GET_SESSION_INFO_CNF
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 session_id;
	int32 result;
	uint32 stage;
    uint32 content_len;        //the total length of content
    uint32 current_len;        //current length
	char dd_url[ ODM_DD_SCHEMA_URI ];
	BOOLEAN is_dd_content_set;
	uint8 padding1;
	uint16 padding2;
	ODM_DOWNLOAD_DESCRIPTION_T basic_dd_content;
} ODM_DOWNLOAD_GET_SESSION_INFO_CNF_MSG_T;


// ODM_SUBSCRIBE_INDICATION_REQ
typedef struct {
	_SIGNAL_VARS		// header part
} ODM_SUBSCRIBE_INDICATION_REQ_MSG_T;


// ODM_SUBSCRIBE_INDICATION_CNF
typedef struct {
	_SIGNAL_VARS		// header part	
	int32 result;		// error no
} ODM_SUBSCRIBE_INDICATION_CNF_MSG_T;


// ODM_DESUBSCRIBE_INDICATION_REQ
typedef struct {
	_SIGNAL_VARS		// header part
} ODM_DESUBSCRIBE_INDICATION_REQ_MSG_T;


// ODM_DESUBSCRIBE_INDICATION_CNF
typedef struct {
	_SIGNAL_VARS		// header part
	int32 result;
} ODM_DESUBSCRIBE_INDICATION_CNF_MSG_T;


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : start odm task
//    Global resource dependence : 
//    Author: shulin.chang
//	  Retrun: task id
//    Note: 
/*****************************************************************************/
PUBLIC BLOCK_ID ODM_Start( uint32 argc, void *argv );

/*****************************************************************************/
//    Description : get odm task id
//    Global resource dependence : 
//    Author: shulin.chang
//	  Retrun: task id
//    Note: 
/*****************************************************************************/
PUBLIC BLOCK_ID ODM_GetTaskId( void );

/*****************************************************************************/
//    Description : start download
//    Global resource dependence : 
//    Author: kelly.li
//	  Retrun: action result
//    Note: 
/*****************************************************************************/
PUBLIC int32 ODM_StartDownload(const ODM_START_DOWNLOAD_PARAM_T * download_param_ptr);

/*****************************************************************************/
//    Description : cancel download
//    Global resource dependence : 
//    Author: kelly.li
//	  Retrun: action result
//    Note: 
/*****************************************************************************/
PUBLIC int32 ODM_CancelDownload(uint32 session_id);

/*****************************************************************************/
//    Description : app response if confirm to download
//    Global resource dependence : 
//    Author: kelly.li
//	  Retrun: action result
//    Note: 
/*****************************************************************************/
PUBLIC int32 ODM_DownloadConfirmationResponse(const ODM_DOWNLOAD_CONFIRMATION_PARAM_T *confirm_param_ptr);

/*****************************************************************************/
//    Description : app response auth param
//    Global resource dependence : 
//    Author: kelly.li
//	  Retrun: action result
//    Note: 
/*****************************************************************************/
PUBLIC int32 ODM_AuthResponse(const ODM_DOWNLOAD_AUTH_RSP_PARAM_T *auth_param_ptr);

/*****************************************************************************/
//    Description : app get session info
//    Global resource dependence : 
//    Author: kelly.li
//	  Retrun: action result
//    Note: 
/*****************************************************************************/
PUBLIC int32 ODM_GetSessionInfo(uint32 session_id);

/*****************************************************************************/
//    Description : app subscriber indication request
//    Global resource dependence : 
//    Author: kelly.li
//	  Retrun: action result
//    Note: 
/*****************************************************************************/
PUBLIC int32 ODM_SubscribeIndication(void);

/*****************************************************************************/
//    Description : app desubscriber indication 
//    Global resource dependence : 
//    Author: kelly.li
//	  Retrun: action result
//    Note: 
/*****************************************************************************/
PUBLIC int32 ODM_DesubscribeIndication(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_ODM_H_
