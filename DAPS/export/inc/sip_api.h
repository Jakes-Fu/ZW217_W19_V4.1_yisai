/****************************************************************************
** File Name:      sip_api.h                                                *
** Author:         bruce.chi                                                *
** Date:           2008.4                                                   *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the SIP api                *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2008.5        bruce.chi         Modify                                   *
****************************************************************************/
#ifndef __SIP_API_H__
#define  __SIP_API_H__

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "in_message.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//Provisional Response code: 1xx
#define SIP_RSP_CODE_TRYING                     100
#define SIP_RSP_CODE_RINGING                    180
#define SIP_RSP_CODE_QUEUED                     182
#define SIP_RSP_CODE_SESS_PROGRESS              183

//Successful Response code: 2xx
#define SIP_RSP_CODE_200OK                      200
//subscribe
#define SIP_RSP_CODE_202ACCEPTED                202

//Request Failure Response code: 4xx
#define SIP_RSP_CODE_
#define SIP_RSP_CODE_BAD_REQUEST                400
#define SIP_RSP_CODE_UNAUTHORIZED               401
#define SIP_RSP_CODE_PAYMENT_REQUIRED           402
#define SIP_RSP_CODE_FORBIDDEN                  403
#define SIP_RSP_CODE_NOT_FOUND                  404
#define SIP_RSP_CODE_METHOD_NOT_ALLOWED         405
#define SIP_RSP_CODE_NOT_ACCEPTABLE             406
#define SIP_RSP_CODE_PROXY_AUTHEN_REQUIRED      407
#define SIP_RSP_CODE_REQUEST_TIMEOUT            408
#define SIP_RSP_CODE_GONE                       410
#define SIP_RSP_CODE_REQUEST_ENTITY_TOO_LARGE   413
#define SIP_RSP_CODE_REQUEST_URI_TOO_LONG       414
#define SIP_RSP_CODE_UNSUPPORTED_MEDIA_TYPE     415
#define SIP_RSP_CODE_UNSUPPORTED_URI_SCHEME     416
#define SIP_RSP_CODE_BAD_EXTENSION              420
#define SIP_RSP_CODE_EXTENSION_REQUIRED         421
#define SIP_RSP_CODE_INTERVAL_TOO_BRIEF         423
#define SIP_RSP_CODE_TEMPORARILY_UNAVAILABLE    480
#define SIP_RSP_CODE_CALL_DOES_NOT_EXIST        481
#define SIP_RSP_CODE_LOOP_DETECTED              482
#define SIP_RSP_CODE_TOO_MANY_HOPS              483
#define SIP_RSP_CODE_ADDRESS_INCOMPLETE         484
#define SIP_RSP_CODE_AMBIGUOUS                  485
#define SIP_RSP_CODE_BUSY_HERE                  486
#define SIP_RSP_CODE_REQUEST_TERMINATED         487
#define SIP_RSP_CODE_NOT_ACCEPTABLE_HERE        488
#define SIP_RSP_CODE_REQUEST_PENDING            491
#define SIP_RSP_CODE_UNDECIPHERABLE             493
//subscribe
#define SIP_RSP_CODE_BAD_EVENT                  489

//sip header string attribute
#define SIP_MAX_DISPLAY_NAME_LEN                64      //max display name length
#define SIP_MAX_URI_LEN                         128     //max uri string length
#define SIP_MAX_URI_PARAM_STR_LEN               64      //max uri param length
#define SIP_MAX_OTHER_HEADER_NAME_STR_LEN       128     //max name string length in other header
#define SIP_MAX_OTHER_HEADER_VALUE_STR_LEN      128     //max value string length in other header
#define SIP_MAX_CONTACT_EXT_STR_LEN             256     //max contact extension string length
#define SIP_MAX_OPTION_STR_LEN                  128     //max option string length in require header
#define SIP_MAX_EVENT_HEADER_PARAM_STR_LEN      128     //max param string length in event header
#define SIP_MAX_OTHER_HEADER_NUM                8       //max other header number in one sip signaling
#define SIP_MAX_BODY_MEDIA_TYPE_STR_LEN         32      //max media type string length in sip content type header
#define SIP_MAX_DISPOSITION_TYPE_STR_LEN        32      //max disposition type string length
#define SIP_MAX_BODY_BUF_LEN                    1024    //max sip body buffer len
#define SIP_MAX_BODY_PART_NUM                   2       //max sip body part number of multi-part type 
#define SIP_MAX_IMPI_STR_LEN                    128     //max sip impi string length
#define SIP_MAX_PWD_STR_LEN                     64      //max sip password string length
#define SIP_MAX_IP_ADDR_STR_LEN                 15      //max sip ip address string length
#define SIP_MAX_HOST_NAME_LEN                   64      //max sip host name length

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef char *  SIP_STRING_PTR_T;           //string pointer.
typedef uint32  SIP_MODULE_ID_T;            //module id of app
typedef uint32  SIP_REGISTER_SESS_ID_T;     //sip register session id
typedef uint32  SIP_CALL_SESS_ID_T;         //call session id          
typedef uint32  SIP_PUB_SESS_ID_T;          //publish session id
typedef uint16  SIP_RSP_CODE_T;             //sip response code (the detailed code number please refer to the MACRO DEFINITION)
typedef uint32  SIP_SUBS_SESS_ID_T;         //subscribe session id
typedef uint16  SIP_PORT_T;                 //sip port type

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
	SIP_NO_ERROR,                       //ok, no error (2xx)
    SIP_FATAL_ERROR,                    //fatal error (3xx-6xx)

    SIP_INPUT_PARAM_ERROR,              //input param error
    SIP_GET_CONTEXTID_ERROR,            //get context id fail
    SIP_ENCODE_ERROR,                   //encode fail
    SIP_STATE_INVALID_ERROR,            //state invalid, not accept message from app
    SIP_AUTH_ERROR,                     //auth fail
    SIP_REDIRECTED_ERROR,               //redirected fail
    SIP_LOCAL_FAIL_ERROR,               //local fail in sip manager

    SIP_REMOTEACCEPTED_ERROR,           //remote accepted    
    SIP_TERMINATED_ERROR,               //callleg has terminated
    SIP_UPDATE_FAIL_ERROR,              //update fail
    SIP_NO_MANUAL_ERROR,                //app not allowed to do something, such as publish refresh
    SIP_DISCONNECTED_ERROR,             //callleg goes to disconnected state
    SIP_REMOTE_DISCONNECTED_ERROR,      //callleg goes to disconnected state
    SIP_LOCAL_DISCONNECTING_ERROR,      //send 500 response to INVITE request
    SIP_LOCAL_CANCELLING_ERROR,         //local send cancel
    SIP_CANCELLED_ERROR,                //callleg has been cancelled
    
    SIP_UNSUPPORTED_AUTH_PARAMS_ERROR,  //unsupported auth parameter in //401/407 response message
    SIP_TIMEOUT_ERROR,                  //transaction timeout, notify timeout, proceeding timeout 
    SIP_SEND_FAIL_ERROR,                //send fail
    SIP_RV_LOCAL_FAIL_ERROR,            //rv's local fail
    SIP_RV_NO_TMR_ERROR,                //no timer    
    SIP_RV_TRANS_ERROR,                 //transaction error, SUBSCRIBE need
    SIP_MISSING_SIP_ETAG_ERROR,         //publish 2xx response has no sip-etag header
    SIP_MISSING_EXPIRES_ERROR,          // publish 2xx response has no expires header

    SIP_OTHER_ERROR,                    //other error
    SIP_ERR_MAX                         //max number
}SIP_ERROR_CODE_E;      //sip error code


typedef enum
{
    SIP_ADDR_URI_TYPE,      //sip uri, start with "sip:"
    SIP_ADDR_TEL_TYPE,      //tel uri, start with "tel:"
    SIP_ADDR_URI_TYPE_MAX
}SIP_ADDR_URL_TYPE_E;       //sip address uri type

typedef enum
{
    SIP_SESSION_REFRESHER_NONE,
    SIP_SESSION_REFRESHER_UAC,
    SIP_SESSION_REFRESHER_UAS
}SIP_SESSION_REFRESH_TYPE_E;        //sip session expires refresh type

typedef enum
{
    SIP_BODY_MEDIATYPE_UNDEFINED,
    SIP_BODY_MEDIATYPE_TEXT,
    SIP_BODY_MEDIATYPE_IMAGE,
    SIP_BODY_MEDIATYPE_AUDIO,
    SIP_BODY_MEDIATYPE_VIDEO,
    SIP_BODY_MEDIATYPE_APPLICATION,
    SIP_BODY_MEDIATYPE_MULTIPART,
    SIP_BODY_MEDIATYPE_MESSAGE,
    SIP_BODY_MEDIATYPE_OTHER
}SIP_BODY_MEDIA_TYPE;               //sip body media type

typedef enum
{
    SIP_BODY_MEDIASUBTYPE_UNDEFINED,
    SIP_BODY_MEDIASUBTYPE_PLAIN,
    SIP_BODY_MEDIASUBTYPE_SDP,
    SIP_BODY_MEDIASUBTYPE_ISUP,
    SIP_BODY_MEDIASUBTYPE_QSIG,
    SIP_BODY_MEDIASUBTYPE_MIXED,
    SIP_BODY_MEDIASUBTYPE_ALTERNATIVE,
    SIP_BODY_MEDIASUBTYPE_DIGEST,
    SIP_BODY_MEDIASUBTYPE_RFC822,
	SIP_BODY_MEDIASUBTYPE_3GPP_IMS_XML,
    SIP_BODY_MEDIASUBTYPE_PIDF_XML,
	SIP_BODY_MEDIASUBTYPE_PARTIAL_PIDF_XML,
	SIP_BODY_MEDIASUBTYPE_WATCHERINFO_XML,
	SIP_BODY_MEDIASUBTYPE_RELATED,
    SIP_BODY_MEDIASUBTYPE_CSTA_XML,
	SIP_BODY_MEDIASUBTYPE_OTHER
}SIP_BODY_MEDIA_SUBTYPE_E;          //sip body media sub type

typedef enum
{
    SIP_BODY_DISPOSITIONTYPE_UNDEFINED,
    SIP_BODY_DISPOSITIONTYPE_RENDER,
    SIP_BODY_DISPOSITIONTYPE_SESSION,
    SIP_BODY_DISPOSITIONTYPE_ICON,
    SIP_BODY_DISPOSITIONTYPE_ALERT,
    SIP_BODY_DISPOSITIONTYPE_SIGNAL,    
    SIP_BODY_DISPOSITIONTYPE_OTHER
}SIP_BODY_DISPOSITION_TYPE_E;

typedef enum
{
    SIP_SUBSTATE_UNDEFINED,
    SIP_SUBSTATE_ACTIVE,        //active
    SIP_SUBSTATE_PENDING,       //pending
    SIP_SUBSTATE_TERMINATED,    //terminated
    SIP_SUBSTATE_MAX            //max number
} SIP_SUBSTATE_E;               // subscription state

typedef enum
{
    SIP_SUBS_RETRYAFTER_INVALID,        //valid when substate is "pending" or "active"
    SIP_SUBS_RETRYAFTER_NOT_PRESENT,    //retry-after not present, you can resend subscribe request at any time
    SIP_SUBS_RETRYAFTER_ZERO,           //you can resend subscribe request immediately
    SIP_SUBS_RETRYAFTER_NO_NEED,        //no need to resend subscribe request
    SIP_SUBS_RETRYAFTER_PRESENT,        //wait retryafter_seconds time, you can resend subscribe request
    SIP_SUBS_RETRYAFTER_MAX             //max number
} SIP_SUBS_RETRYAFTER_E;                // subs retry-after type

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
/**--------------------------------------------*
**             BASIC STRUCTURE                 *
**---------------------------------------------*/
typedef struct 
{
    SIP_ADDR_URL_TYPE_E uri_type;                                       //uri type    
    char                display_name[SIP_MAX_DISPLAY_NAME_LEN + 1];     //display name
    char                uri[SIP_MAX_URI_LEN + 1];                       //sip uri, format is user@host:port
    char                uri_param_str[SIP_MAX_URI_PARAM_STR_LEN + 1];   //uri parameter
}SIP_NAME_ADDR_T;                                                       //sip name-address type
                                                                        //Example : display_name<sip:uri;uri_para_str>
                                                                        //Example : display_name_str:   bruce.chi            
                                                                        //          uri:                impu@cmcc.net:5060 
                                                                        //          uri_param_str:      app-event

typedef struct 
{
    char                ip_addr[SIP_MAX_IP_ADDR_STR_LEN + 1];           //ip address string   
    SIP_PORT_T          port;                                           //port
}SIP_IP_ADDR_T;

typedef struct 
{
    char        name_str[SIP_MAX_OTHER_HEADER_NAME_STR_LEN + 1];        //name pointer
    char        value_str[SIP_MAX_OTHER_HEADER_VALUE_STR_LEN + 1];      //value pointer
}SIP_OTHER_HEADER_T;                                                    //sip other header structure, format is "name=value"
                                                                        //Example:name_str=value_str
                                                                        //Example:Event=app-settings

typedef struct 
{
    SIP_NAME_ADDR_T request_uri;    //host
}SIP_REQUEST_URI_T;                 //Request-URI in Request Line.

typedef struct 
{
    SIP_NAME_ADDR_T redirected_uri;    //redirected uri
}SIP_REDIRECTED_URI_T;                 //Redirected-URI in contact header of 3xx response.

typedef struct 
{
    uint32          delta_seconds;  //seconds  
}SIP_EXPIRES_HEADER_T;              //Expires Header
                                    //Example: Expires:1800
                                    //Example:         1800                                

typedef struct 
{
    SIP_NAME_ADDR_T name_addr;      //name-addr
}SIP_FROM_HEADER_T;                 //From Header
                                    //Note: From header tag will be filled by sip protocol stack
typedef struct 
{
    SIP_NAME_ADDR_T name_addr;      //name-addr
}SIP_TO_HEADER_T;                   //To header
                                    //Note: To header tag will be filled by sip protocol stack

typedef struct 
{
    SIP_NAME_ADDR_T name_addr;                                      //name-addr
    char            extention_str[SIP_MAX_CONTACT_EXT_STR_LEN + 1]; //contact extention pointer
}SIP_CONTACT_HEADER_T;                                              //Contact header
                                                                    //Example: name_addr;extention_str
                                                                    //Example:           g.app.feature
typedef struct 
{
    int32                       delta_seconds;      //seconds
    SIP_SESSION_REFRESH_TYPE_E  sess_refresh_type;  //refresh type
}SIP_SESSION_EXPIRES_HEADER_T;                      //Session-Expires header

typedef struct 
{
    char                        package_str[SIP_MAX_EVENT_HEADER_PARAM_STR_LEN + 1];  //package information
    char			template_str[SIP_MAX_EVENT_HEADER_PARAM_STR_LEN + 1]; //template information
}SIP_EVENT_HEADER_T;                                                                //Event header

typedef struct 
{
    uint32                  other_header_num;                           //other header number
    SIP_OTHER_HEADER_T      other_header[SIP_MAX_OTHER_HEADER_NUM];     //other header array pointer
}SIP_ALL_OTHER_HEADER_T;                                                //all other headers in sip message structure.

typedef struct 
{
    SIP_BODY_MEDIA_TYPE         media_type;                                             //media type.
    char                        media_type_str[SIP_MAX_BODY_MEDIA_TYPE_STR_LEN + 1];    //media type header string . Note: Only take effect when media_type is SIP_BODY_MEDIATYPE_OTHER
    SIP_BODY_MEDIA_SUBTYPE_E    media_subtype;                                          //media sub type
    char                        sub_type_str[SIP_MAX_BODY_MEDIA_TYPE_STR_LEN + 1];      //media sub type header string. Note: Only take effect when media_subtype is SIP_MEDIASUBTYPE_OTHER
    SIP_BODY_DISPOSITION_TYPE_E disposition_type;                                       //disposition type
    char                        disp_type_str[SIP_MAX_DISPOSITION_TYPE_STR_LEN + 1];    //disposition type string. Note: Only take effect when disposition_type is SIP_BODY_DISPOSITIONTYPE_OTHER
    uint32                      body_len;                                               //body length in bytes
    char                        body_buf[SIP_MAX_BODY_BUF_LEN + 1];                     //body buffer pointer      
}SIP_BODY_PART_T;                                                                       //sip body part structure

typedef struct 
{
    SIP_BODY_MEDIA_TYPE         media_type;                                             //media type
    char                        media_type_str[SIP_MAX_BODY_MEDIA_TYPE_STR_LEN + 1];    //media type header string . Note: Only take effect when media_type is SIP_BODY_MEDIATYPE_OTHER
    SIP_BODY_MEDIA_SUBTYPE_E    media_subtype;                                          //media sub type
    char                        sub_type_str[SIP_MAX_BODY_MEDIA_TYPE_STR_LEN + 1];      //media sub type header string. Note: Only take effect when media_subtype is RVSIP_MEDIASUBTYPE_OTHER
    SIP_BODY_DISPOSITION_TYPE_E disposition_type;                                       //disposition type
    char                        disp_type_str[SIP_MAX_DISPOSITION_TYPE_STR_LEN + 1];    //disposition type string. Note: Only take effect when disposition_type is SIP_BODY_DISPOSITIONTYPE_OTHER
    uint32                      part_num;                                               //multi-part number. Note: "1" means single part (not multi-part). "0" means this whole struct does not take effect
    SIP_BODY_PART_T             part[SIP_MAX_BODY_PART_NUM];                            //body part pointer
}SIP_BODY_T;                                                                            //sip body structure. Note, this structure include Content-Type header structure

                                                                                        //Example 1, single part
                                                                                        //Content-Type:applicaton/sdp
                                                                                        //Body-String:c=IN IP4 192.168.1.1
                                                                                        //
                                                                                        //(SIP_BODY_T::media_type) = SIP_BODY_MEDIATYPE_APPLICATION
                                                                                        //(SIP_BODY_T::media_subtype) = RVSIP_MEDIASUBTYPE_SDP
                                                                                        //(SIP_BODY_T::part_num) = 1
                                                                                        //(SIP_BODY_T::part[0]) = &SIP_BODY_PART_T
                                                                                        //(SIP_BODY_PART_T::buf_len) and (SIP_BODY_PART_T::body_buf) is filled with the sdp buffer. 
                                                                                        //other param in SIP_BODY_PART_T is ignored

                                                                                        //Example 2, multi-part
                                                                                        //Content-Type:mutipart/mixed;boundary=unique-boundary-1
                                                                                        //Content-Type1:application/sdp
                                                                                        //Body-String1:c=IN IP4 192.168.1.1
                                                                                        //Content-Type2:application/plain
                                                                                        //Body-String2:"Hello world."
                                                                                        //
                                                                                        //(SIP_BODY_T::media_type) = SIP_BODY_MEDIATYPE_MULTIPART
                                                                                        //(SIP_BODY_T::media_subtype) = RVSIP_MEDIASUBTYPE_MIXED
                                                                                        //(SIP_BODY_T::part_num) = 2
                                                                                        //(SIP_BODY_T::part[0]) = &SIP_BODY_PART_T1
                                                                                        //(SIP_BODY_T::part[1]) = &SIP_BODY_PART_T2
                                                                                        //(SIP_BODY_PART_T1::media_type) = SIP_BODY_MEDIATYPE_APPLICATION
                                                                                        //(SIP_BODY_PART_T1::media_subtype) = RVSIP_MEDIASUBTYPE_SDP
                                                                                        //(SIP_BODY_PART_T1::buf_len) and (SIP_BODY_PART_T::body_buf) is filled with the sdp buffer. 
                                                                                        //(SIP_BODY_PART_T2::media_type) = SIP_BODY_MEDIATYPE_APPLICATION
                                                                                        //(SIP_BODY_PART_T2::media_subtype) = RVSIP_MEDIASUBTYPE_PLAIN
                                                                                        //(SIP_BODY_PART_T2::buf_len) and (SIP_BODY_PART_T::body_buf) is filled with the plain string. 


/*
Note: The following headers are treated as extention header
Accept-Contact
Asserted-Identity
Answer-Mode
Answer-State
Alerting-Mode
P-Preferred-Identity
P-Asserted-Identity
User-Agent
Privacy
Priv-Answer-Mode
Server
Supported
*/


/**--------------------------------------------*
**                REGISTER                     *
**---------------------------------------------*/

typedef struct 
{
    char    impi_str[SIP_MAX_IMPI_STR_LEN + 1];     //impi string
    char    password_str[SIP_MAX_PWD_STR_LEN + 1];  //password string
}SIP_USER_AUTH_PARAM_T;                             //sip auth param. (Note: This is not Contact header)

typedef struct 
{
    SIP_IP_ADDR_T       ip_info;                                  //proxy ip info
    char                host_name[SIP_MAX_HOST_NAME_LEN + 1];     //outbound proxy host name
}SIP_PROXY_INFO_T;

typedef struct 
{
    SIP_REGISTER_SESS_ID_T  reg_session_id;                             //register session id
    SIP_REQUEST_URI_T       registrar;                                  //Request-URI
    SIP_EXPIRES_HEADER_T    expires_header;                             //Expires header
    SIP_FROM_HEADER_T       from_header;                                //From header
    SIP_TO_HEADER_T         to_header;                                  //To header
    SIP_CONTACT_HEADER_T    contact_header;                             //Contact header
    SIP_USER_AUTH_PARAM_T   auth_param;                                 //auth param
    SIP_PROXY_INFO_T        outbound_proxy_info;                        //outbound proxy    
    SIP_ALL_OTHER_HEADER_T  all_other_header;                           //all other header
}SIP_REGISTER_PARAM_T;                    

typedef struct
{
    _SIGNAL_VARS
    SIP_MODULE_ID_T         module_id;          //application module id
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id
    SIP_ERROR_CODE_E        error_code;         //error code
}SIP_REG_SESS_INIT_CNF_SIG_T;                //sip register session init confirm signal

typedef struct
{
    _SIGNAL_VARS
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id
    SIP_ERROR_CODE_E        error_code;         //error code
    SIP_RSP_CODE_T          rsp_code;          //register response code
}SIP_REG_REGISTER_CNF_SIG_T;                    //sip register confirm signal

typedef struct
{
    _SIGNAL_VARS
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id
    SIP_REDIRECTED_URI_T    redirected_uri;     //redirected uri
}SIP_REG_REDIRECTED_IND_SIG_T;       //sip register redirected indication signal

typedef struct
{
    _SIGNAL_VARS
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id
    SIP_ERROR_CODE_E        error_code;         //error code
    SIP_RSP_CODE_T          rsp_code;          //register response code
}SIP_REG_DEREGISTER_CNF_SIG_T;                  //sip de-register confirm signal

typedef struct
{
    _SIGNAL_VARS
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id    
}SIP_REG_REF_START_IND_SIG_T;           //refresh start indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id    
    SIP_ERROR_CODE_E        error_code;     //error code
    SIP_RSP_CODE_T          rsp_code;       //response code, such as 200, 400, etc. 
}SIP_REG_REF_END_IND_SIG_T;           //refresh end indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id
    SIP_ERROR_CODE_E        error_code;		    //error code
}SIP_REG_TERMINATE_CNF_SIG_T;                   //sip register session terminate confirm signal

typedef struct 
{
    _SIGNAL_VARS
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id
    SIP_ERROR_CODE_E        error_code;         //error code
}SIP_REG_ERROR_IND_SIG_T;   //register error ind. (Note: this is used like the return value of function call)

/**--------------------------------------------*
**                CALL                         *
**---------------------------------------------*/
typedef struct 
{
    SIP_CALL_SESS_ID_T              call_session_id;        //call session id
    SIP_REGISTER_SESS_ID_T          reg_session_id;         //the associated register session id 
    SIP_REQUEST_URI_T               req_uri;                //Request-Uri
    SIP_FROM_HEADER_T               from_header;            //From header
    SIP_TO_HEADER_T                 to_header;              //To header
    SIP_BODY_T                      body;                   //sip body (including Content-Type header)
    SIP_SESSION_EXPIRES_HEADER_T    sess_expires_header;    //Session-Expires header
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
}SIP_CALL_INVITE_REQ_PARAM_T;            //invite cmd param

typedef struct 
{
    SIP_CALL_SESS_ID_T              call_sess_id;           //call session id
    SIP_RSP_CODE_T                  response_code;          //call response code
    BOOLEAN							is_reliably;            //1xx reliable or not
    SIP_FROM_HEADER_T               from_header;            //From header
    SIP_TO_HEADER_T                 to_header;              //To header
    SIP_SESSION_EXPIRES_HEADER_T    sess_expires_header;    //Session-Expires header
    SIP_BODY_T                      body;                   //sip body
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
}SIP_CALL_INVITE_RSP_PARAM_T;        //call invite response params

typedef struct
{
    SIP_CALL_SESS_ID_T              call_sess_id;           //call session id
    SIP_FROM_HEADER_T               from_header;            //From header
    SIP_TO_HEADER_T                 to_header;              //To header
    SIP_SESSION_EXPIRES_HEADER_T    sess_expires_header;    //Session expires header
    SIP_CONTACT_HEADER_T            contact_header;         //Contact header
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
    SIP_BODY_T                      body;                   //sip body
    BOOLEAN                         is_send_ack;            //TRUE, app need to send ACK; FALSE, sip stack send ACK, app do nothing
    SIP_RSP_CODE_T              	response_code;          //final response code, 4xx-6xx
    SIP_ERROR_CODE_E    			error_code;             //error code
}SIP_CALL_INVITE_CNF_PARAM_T;                //call invite confirm params

typedef struct 
{
    SIP_CALL_SESS_ID_T              call_session_id;        //call session id
    SIP_BODY_T                      body;                   //sip body (including Content-Type header)
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
}SIP_CALL_UPDATE_REQ_PARAM_T;     //update request param

typedef struct 
{
    SIP_CALL_SESS_ID_T              call_session_id;        //call session id
    SIP_RSP_CODE_T                  response_code;          //call response code
    SIP_BODY_T                      body;                   //sip body
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
}SIP_CALL_UPDATE_RSP_PARAM_T;

typedef struct 
{
    SIP_CALL_SESS_ID_T              call_session_id;        //call session id
    SIP_ERROR_CODE_E                error_code;             //error code
    SIP_RSP_CODE_T                  response_code;          //call response code
    SIP_BODY_T                      body;                   //sip body (including Content-Type header)
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
}SIP_CALL_UPDATE_CNF_PARAM_T;

typedef struct 
{
    SIP_CALL_SESS_ID_T      call_session_id;    //call session id
    SIP_REQUEST_URI_T       request_uri;        //Request uri
    SIP_ALL_OTHER_HEADER_T  all_other_header;   //all other header
}SIP_CALL_DISCONNECT_REQ_PARAM_T;               //call disconnect req param

typedef struct
{
    _SIGNAL_VARS
    SIP_MODULE_ID_T     module_id;          //application module id
    SIP_CALL_SESS_ID_T  call_sess_id;       //call session id
    SIP_ERROR_CODE_E    error_code;         //error code
}SIP_CALL_SESS_INIT_CNF_SIG_T;              //call session 

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T      call_sess_id;       //call session id
    SIP_REDIRECTED_URI_T    redirected_uri;     //redirected uri
}SIP_CALL_REDIRECTED_IND_SIG_T;             //redirected ind signal

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T          call_sess_id;       //call session id
    SIP_RSP_CODE_T              response_code;      //provisional response code, 1xx
    SIP_FROM_HEADER_T           from_header;        //From header
    SIP_TO_HEADER_T             to_header;          //To header
    SIP_CONTACT_HEADER_T        contact_header;     //Contact header
    SIP_ALL_OTHER_HEADER_T      all_other_header;   //all other header
    SIP_BODY_T                  body;               //sip body
}SIP_CALL_PROCEEDING_IND_SIG_T;     //call proceeding ind. (In another word, 1xx ind)

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T  call_sess_id;       //call session id
}SIP_CALL_PROCEEDING_TIMEOUT_IND_SIG_T;     //call proceeding time out indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T              call_sess_id;           //call session id     
    SIP_FROM_HEADER_T               from_header;            //From header
    SIP_TO_HEADER_T                 to_header;              //To header
    SIP_REQUEST_URI_T               request_uri;            //Request uri
    SIP_CONTACT_HEADER_T            contact_header;         //Contact header
    SIP_SESSION_EXPIRES_HEADER_T    sess_expires_header;    //Session-Expires header
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
    SIP_BODY_T                      body;                   //sip body
}SIP_CALL_INVITE_IND_SIG_T;      //call invite indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_INVITE_CNF_PARAM_T     invite_cnf_param;       //invite confirm params
}SIP_CALL_INVITE_CNF_SIG_T;                //call invite confirm

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T      call_sess_id;       //call session id
    SIP_ERROR_CODE_E        error_code;         //error code, indicate success ACK or fail ACK    
}SIP_CALL_ACK_IND_SIG_T;       //call ACK indicate (remote ack)

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T  call_sess_id;       //call session id
}SIP_CALL_CANCEL_IND_SIG_T;              //call cancel ind

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T  call_sess_id;       //call session id
    SIP_ERROR_CODE_E    error_code;         //error code
    SIP_RSP_CODE_T      rsp_code;           //response code for INVITE/CANCEL
}SIP_CALL_CANCEL_CNF_SIG_T;                 //call cancel cnf

typedef struct 
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T              call_session_id;        //call session id
    SIP_BODY_T                      body;                   //sip body (including Content-Type header)
    SIP_ALL_OTHER_HEADER_T          all_other_header;       //all other header
}SIP_CALL_UPDATE_IND_SIG_T;

typedef struct 
{
    _SIGNAL_VARS
    SIP_CALL_UPDATE_CNF_PARAM_T     update_cnf_param;       //update confirm params
}SIP_CALL_UPDATE_CNF_SIG_T;

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T  call_sess_id;     //call session id
    SIP_ERROR_CODE_E    error_code;             //error code
}SIP_CALL_DISCONNECT_IND_SIG_T;                         //disconnect ind

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T  call_sess_id;           //call session id
    SIP_ERROR_CODE_E    error_code;             //error code
    SIP_RSP_CODE_T      response_code;          //call response code
}SIP_CALL_DISCONNECT_CNF_SIG_T;                 //disconnect confirm

typedef struct
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T  call_sess_id;       //call session id
    SIP_ERROR_CODE_E    error_code;         //error code
}SIP_CALL_TERMINATE_CNF_SIG_T;              //call session terminate confirm

typedef struct 
{
    _SIGNAL_VARS
    SIP_CALL_SESS_ID_T  call_sess_id;       //call session id
    SIP_ERROR_CODE_E    error_code;         //error code
}SIP_CALL_ERROR_IND_SIG_T;     //call error ind. (Note: this is used like the return value of function call)


/**--------------------------------------------*
**                PUBLISH                      *
**---------------------------------------------*/
typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id; //publish session id
    SIP_MODULE_ID_T     module_id;      //application module id
    SIP_ERROR_CODE_E    error_code;     //error code
}SIP_PUB_SESS_INIT_CNF_SIG_T;

typedef struct 
{
    SIP_PUB_SESS_ID_T       pub_session_id;     //publish session id
    SIP_REGISTER_SESS_ID_T  reg_session_id;     //register session id    
    SIP_FROM_HEADER_T       from_header;        //From header
    SIP_TO_HEADER_T         to_header;          //To header
    SIP_REQUEST_URI_T       publisher;          //Request-URI
    SIP_EXPIRES_HEADER_T    expires_header;     //Expires header
    SIP_EVENT_HEADER_T      event_header;       //Event header (Note: this is necessary)
    SIP_BODY_T              body;               //sip body
    SIP_ALL_OTHER_HEADER_T  all_other_header;   //all other header
}SIP_PUBLISH_PARAM_T;                           //publish cmd param

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id;         //publish session id
    SIP_ERROR_CODE_E    error_code;             //error code    
    SIP_RSP_CODE_T      rsp_code;               //response code, such as 200, 400, etc. 
}SIP_PUB_PUBLISH_CNF_SIG_T;                 //publish cmd confirm

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T       pub_session_id; //publish session id
    SIP_REDIRECTED_URI_T    redirected_uri; //redirected uri
}SIP_PUB_REDIRECTED_IND_SIG_T;          //publish redirected ind

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id; //publish session id
    SIP_ERROR_CODE_E    error_code;     //error code
    SIP_RSP_CODE_T      rsp_code;       //response code, such as 200, 400, etc. 
}SIP_PUB_REMOVE_CNF_SIG_T;              //publish remove confirm

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id; //publish session id    
}SIP_PUB_REF_START_IND_SIG_T;           //refresh start indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id; //publish session id    
    SIP_ERROR_CODE_E    error_code;     //error code
    SIP_RSP_CODE_T      rsp_code;       //response code, such as 200, 400, etc. 
}SIP_PUB_REF_END_IND_SIG_T;           //refresh end indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id; //publish session id    
    SIP_ERROR_CODE_E    error_code;     //error code
    SIP_RSP_CODE_T      rsp_code;       //response code, such as 200, 400, etc. 
}SIP_PUB_REFRESH_CNF_SIG_T;           //refresh confirm

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id; //publish session id
    SIP_ERROR_CODE_E    error_code;     //error code
}SIP_PUB_TERMINATE_CNF_SIG_T;           //pub terminate confirm

typedef struct
{
    _SIGNAL_VARS
    SIP_PUB_SESS_ID_T   pub_session_id; //publish session id
    SIP_ERROR_CODE_E    error_code;     //error code
}SIP_PUB_ERROR_IND_SIG_T;           //pub error indicate

/**--------------------------------------------*
**                SUBSCRIBE                    *
**---------------------------------------------*/
typedef struct
{
	SIP_SUBSTATE_E          substate;           //subscription state
    SIP_SUBS_RETRYAFTER_E   retryafter_type;    //retry after type
	uint32                  retryafter_seconds; //seconds. Valid when retryafter_type is "SIP_SUBS_RETRYAFTER_PRESENT".                                     
} SIP_SUBSCRIPTIONSTATE_HEADER_T;   // subscription-state header

typedef struct
{
    _SIGNAL_VARS
    SIP_MODULE_ID_T      module_id;         //application module id
    SIP_SUBS_SESS_ID_T   subs_sess_id;      //subs session id
    SIP_ERROR_CODE_E     error_code;        //error code
}SIP_SUBS_SESS_INIT_CNF_SIG_T;              //subs session

typedef struct 
{
    SIP_SUBS_SESS_ID_T		subs_sess_id;           // subs session id
    SIP_REGISTER_SESS_ID_T  reg_session_id;         //the associated register session id
    SIP_REQUEST_URI_T		notifier;               //Request-URI
	SIP_EXPIRES_HEADER_T	expires_header;         //Expires header
	SIP_EVENT_HEADER_T		event_header;           //Event header
    SIP_FROM_HEADER_T		from_header;            //From header
    SIP_TO_HEADER_T			to_header;              //To header      
	SIP_ALL_OTHER_HEADER_T  all_other_header;       //all other header
	SIP_BODY_T				body;                   //sip body (including Content-Type header)
} SIP_SUBS_SUBSPARAM_T;

typedef struct
{
    _SIGNAL_VARS
	SIP_SUBS_SESS_ID_T		subs_sess_id;   // subs session id
	SIP_ERROR_CODE_E        err_code;       //error code
    SIP_RSP_CODE_T          rsp_code;       //response code, such as 200, 400, etc.    
} SIP_SUBS_SUBSCRIBE_CNF_SIG_T;             //sip subscribe confirm signal

typedef struct
{
    _SIGNAL_VARS
	SIP_SUBS_SESS_ID_T		subs_sess_id;   // subs session id
    SIP_REDIRECTED_URI_T    redirected_uri; //redirected uri
} SIP_SUBS_REDIRECTED_IND_SIG_T;            //sip subscribe redirected indication signal

typedef struct
{
    _SIGNAL_VARS
	SIP_SUBS_SESS_ID_T				subs_sess_id;               // subs session id
    SIP_SUBSCRIPTIONSTATE_HEADER_T  subscription_state_header;  //subscription-state
    SIP_BODY_T                      body;                       //sip body
} SIP_SUBS_NOTIFY_IND_SIG_T;                                    //sip subs notify ind

typedef struct
{
    _SIGNAL_VARS
	SIP_SUBS_SESS_ID_T		subs_sess_id;   //subs session id
	SIP_ERROR_CODE_E        err_code;       //error code
    SIP_RSP_CODE_T          rsp_code;       //response code, such as 200, 481, etc.
} SIP_SUBS_UNSUBSCRIBE_CNF_SIG_T;           //unsubscribe cnf signal

typedef struct
{
    _SIGNAL_VARS
    SIP_SUBS_SESS_ID_T      subs_sess_id;   //subs session id  
}SIP_SUBS_REF_START_IND_SIG_T;           //refresh start indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_SUBS_SESS_ID_T  subs_sess_id;   //subs session id
    SIP_ERROR_CODE_E    error_code;     //error code
    SIP_RSP_CODE_T      rsp_code;       //response code, such as 200, 481, etc.
}SIP_SUBS_REF_END_IND_SIG_T;           //refresh end indicate

typedef struct
{
    _SIGNAL_VARS
    SIP_SUBS_SESS_ID_T   subs_sess_id;      //subs session id
    SIP_ERROR_CODE_E     error_code;        //error code
}SIP_SUBS_TERMINATE_CNF_SIG_T;              //subs session terminate confirm

typedef struct
{
    _SIGNAL_VARS
    SIP_SUBS_SESS_ID_T   subs_sess_id; //subs session id
    SIP_ERROR_CODE_E     error_code;      //error code
}SIP_SUBS_ERROR_IND_SIG_T;           //subs error indicate

/**-------------------------------------------------------------------------*
**                         PUBLIC FUNCTION DECLARE                          *
**--------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : init register session request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_RegisterSessInitReq(
                        BLOCK_ID            task_id, 
                        SIP_MODULE_ID_T     module_id
                      );

/****************************************************************************/
// 	Description : register request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_RegisterReq(
                        const SIP_REGISTER_PARAM_T *reg_param_ptr                                          
                          );

/****************************************************************************/
// 	Description : de-register cmd request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_DeRegisterReq(
                         SIP_REGISTER_SESS_ID_T reg_session_id
                         );

/****************************************************************************/
// 	Description : terminate register session request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_RegisterSessTerminateReq(
                        SIP_REGISTER_SESS_ID_T  reg_session_id
                        );

/****************************************************************************/
// 	Description : call session init request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallSessInitReq(
                           BLOCK_ID         task_id, 
                           SIP_MODULE_ID_T  module_id
                           );

/****************************************************************************/
// 	Description : invite cmd request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallInviteReq(
                            const SIP_CALL_INVITE_REQ_PARAM_T *invite_param_ptr                                             
                            );

/****************************************************************************/
// 	Description : call disconnect request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallDisconnectReq(
                            const SIP_CALL_DISCONNECT_REQ_PARAM_T *disconnect_param_ptr
                             );

/****************************************************************************/
// 	Description : call cancel request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallCancelReq(
                          SIP_CALL_SESS_ID_T    call_session_id
                          );

/****************************************************************************/
// 	Description : call cancel request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallAckReq(
                          SIP_CALL_SESS_ID_T    call_session_id
                          );

/****************************************************************************/
// 	Description : response to call offering
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallOfferingRsp(
                            const SIP_CALL_INVITE_RSP_PARAM_T *offering_rsp_param
                           );

/****************************************************************************/
// 	Description : call session terminate  request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallSessTerminateReq(
                             SIP_CALL_SESS_ID_T call_session_id
                             );

/****************************************************************************/
// 	Description : update cmd request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallUpdateReq(
                            const SIP_CALL_UPDATE_REQ_PARAM_T *update_req_param_ptr                                             
                            );

/****************************************************************************/
// 	Description : update cmd request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_CallUpdateRsp(
                            const SIP_CALL_UPDATE_RSP_PARAM_T *update_rsp_param_ptr                                             
                            );

/****************************************************************************/
// 	Description : publish session init request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_PublishSessInitReq(
                              BLOCK_ID          task_id, 
                              SIP_MODULE_ID_T   module_id
                              );

/****************************************************************************/
// 	Description : publish request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_PublishReq(
                            const SIP_PUBLISH_PARAM_T *pub_param_ptr
                          );

/****************************************************************************/
// 	Description : publish remove cmd request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_PublishRemoveReq(
                             SIP_PUB_SESS_ID_T  pub_session_id
                             );

/****************************************************************************/
// 	Description : publish refresh cmd request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_PublishRefreshReq(
                              SIP_PUB_SESS_ID_T pub_session_id
                              );

/****************************************************************************/
// 	Description : re-publish cmd request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_RePublishReq(
                            SIP_PUB_SESS_ID_T   pub_session_id
                            );

/****************************************************************************/
// 	Description : publish session terminate request
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_PubTerminateReq(
                            SIP_PUB_SESS_ID_T   pub_session_id
                            );

/****************************************************************************/
// 	Description : init subs session request
//	Global resource dependence : 
//  Author: hui.li
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_SubsSessInitReq(
                        BLOCK_ID            task_id, 
                        SIP_MODULE_ID_T     module_id
                      );

/****************************************************************************/
// 	Description : subscribe request
//	Global resource dependence : 
//  Author: hui.li
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_SubscribeReq(
                        const SIP_SUBS_SUBSPARAM_T *subs_param_ptr                                          
                          );

/****************************************************************************/
// 	Description : un-subscribe request
//	Global resource dependence : 
//  Author: hui.li
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_UnsubscribeReq(
                         SIP_SUBS_SESS_ID_T subs_session_id
                         );

/****************************************************************************/
// 	Description : terminate subs session request
//	Global resource dependence : 
//  Author: hui.li
//	Note:	
/****************************************************************************/
PUBLIC SIP_ERROR_CODE_E SIP_SubsSessTerminateReq(
                        SIP_SUBS_SESS_ID_T  subs_session_id
                        );

#endif /*end of __SIP_API_H__*/
