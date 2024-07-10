/****************************************************************************
** File Name:      poc_api.h                                                *
** Author:         bruce.chi                                                *
** Date:           2008.3.5                                                 *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the poc api                *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2008.3        bruce.chi         Create                                   *
**                                                                          *
****************************************************************************/

#ifndef POC_API_H
#define POC_API_H

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "in_message.h"
#include "Qhsm.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define POC_MAX_URI_STR_LEN         256         //max string length of sip uri or tel uri. (in byte)
#define POC_MAX_NAME_STR_LEN        32          //max string length of name. (in byte)
#define POC_MAX_PWD_STR_LEN         32          //max string length of password. (in byte)
#define POC_MAX_APN_STR_LEN         32          //max string length of apn. (in byte)
#define POC_MAX_UA_STR_LEN          64          //max string length of user agent. (in byte)
#define POC_MAX_IP_ADDR_STR_LEN     15          //max string length of ip addr. (in byte)
#define POC_MAX_MSISDN_STR_LEN      32          //max string length of msisdn. (in byte)
#define POC_MAX_IPA_STR_LEN	        64          //max string length of instant personal alert. (in byte)
#define POC_INVALID_SESSION_ID      0xFFFFFFFF  //invalid session id
#define POC_INVALID_HANDLE          0xFFFFFFFF  //invalid call leg handle
#define POC_MAX_APPID_STR_LEN       32          //max string length of application id. (in byte)
#define POC_MAX_APPNAME_STR_LEN     32          //max string length of application name. (in byte)
#define POC_MAX_PROVIDER_ID_STR_LEN 32          //max string length of provider id. (in byte)
#define POC_MAX_TO_NAPID_STR_LEN    32          //max string length of to napid name. (in byte)
#define POC_MAX_TO_APPREF_STR_LEN   32          //max string length of to app ref name. (in byte)

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    POC_MANUAL_ANSWER_MODE, //manual answer
    POC_AUTO_ANSWER_MODE,   //auto answer
    POC_ANSWER_MODE_MAX
}POC_ANSWER_MODE_E;

typedef enum
{
    POC_REGISTRATION_ACT,   //Registration
    POC_DEREGISTRATION_ACT, //De-registration
    POC_REGISTRATION_ACT_MAX
}POC_REGISTRATION_ACT_E;

typedef enum
{
    POC_SESSION_ONE_TO_ONE,         //1 to 1
    POC_SESSION_ADHOC_GROUP,        //ad-hoc group
    POC_SESSION_CHAT_GROUP,         //chat group
    POC_SESSION_PREARRANGED_GROUP,  //prearranged group
    POC_SESSION_TYP_MAX
}POC_SESSION_TYP_E;

typedef enum
{
    POC_STR_FORMAT_ASCII,   //ascii format
    POC_STR_FORMAT_UTF8,    //utf8 format
    POC_STR_FORMAT_UCS2,    //ucs2 format
    POC_STR_FORMAT_MAX    
}POC_STR_FORMAT_E;

typedef enum
{
    POC_PLAYER_HANDFREE,    //speech player hand free mode (default)
    POC_PLAYER_HANDHOLD,    //speech player hand hold mode
    POC_SPEECH_PLAYER_MODE_MAX
}POC_SPEECH_PLAYER_MODE_E;

typedef enum
{
    POC_SESSION_PRIORITY_PRIMARY,   //session priority: primary.
    POC_SESSION_PRIORITY_SECONDARY, //session priority: secondary
    POC_SESSION_PRIORITY_MAX
}POC_SESSION_PRIORITY_E;

typedef enum
{
    POC_NOTIFY_STATUS_CONNECTED,    //"connected"
    POC_NOTIFY_STATUS_DISCONNECTED, //"disconnected"
    POC_NOTIFY_STATUS_ONHOLD,       //"on-hold"
    POC_NOTIFY_STATUS_ALERTING,     //"alerting"
    POC_NOTIFY_STATUS_MAX
}POC_NOTIFY_STATUS_TYPE_E;          //poc notify status type

typedef enum
{
    POC_OK = 0,                                     //No error
    POC_INTERNAL_ERROR,                             //internal error
    POC_REJECT_APP_REQUEST,                         //reject app request, because PoC PS can't handle this quest now.
    POC_ALREADY_STARTED,                            //PoC service already started
    POC_SERVER_TIME_OUT,                            //time out, server has no response.
    POC_INVALID_PARAM,                              //invalid input parameters
    POC_ALREADY_REGISTRATION,                       //Already registration
    POC_AUTH_ERROR,                                 //Authentication error
    POC_REREGISTRATION_FAIL,                        //re-registration fail
    POC_PUBLISH_SETTING_ERROR,                      //publish setting error
    POC_CALLE_TEMP_UNAVAILABLE,                     //SIP 480 RESPONSE
    POC_CALLEE_TIME_OUT,                            //callee time out. (SIP 408 RESPONSE)
    POC_ERR_CODE_EXCEED_MAX_PARTICIPANTS,           //Exceed max participants in one session
    POC_ANOTHER_HAS_FLOOR,                          //another PoC User has permission to send a Talk Burst
    POC_SERVER_ERROR,                               //PoC server internal error
    POC_ERROR_ONLY_ONE_PARTICIPANT_IN_GROUP,        //only one participant in the PoC Session
    POC_RETRY_AFTER_TIMER_NOT_EXPIRED,              //Retry-after has not expired after the floor has been revoked
    POC_LISTEN_ONLY,                                //the requesting party only has listen only privilege.
    POC_ERR_CODE_EXCEED_MAX_TALKBRUST_DUARATION,    //Exceed max talk burst duration.
    POC_NO_PERMISSION_TO_SEND_TALK_BURST,           //do not have permission to send a talk burst.
    POC_TALK_BURST_PREEMPTED,                       //PoC Client's permission to send a talk burst is being pre-empted.
    POC_ERROR_CANNOT_SWITCH_SESSION_BLOCK,          //Can't switch session block.
    POC_ERROR_CANNOT_SWITCH_SESSION_ALREADY_EXIST,  //Can't switch session already exist.
    POC_ERROR_CALLEE_BLOCK,                         //Callee block
    POC_ERROR_CALLEE_LOCK_IN_SESSION,               //callee locked in one session
    POC_CALLER_CANCEL_SESSION,                      //caller cancel the session
    POC_NOT_STARTED,                                //have not started poc service
    POC_NOT_CONFIGED,                               //have not configed poc ps
    POC_NOT_REGISTERED,                             //have not registration
    POC_REQUEST_REJECTED,                           //request rejected
    POC_CALLER_LEAVE_CALL,                          //caller leave the call
    POC_ALL_OTHER_PARTICIPANTS_LEAVE,               //all other participants have left the call
    POC_ERR_CODE_MAX
}POC_ERR_CODE_E;                                    //poc error code

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef char    POC_URI_T[POC_MAX_URI_STR_LEN + 1];          //URI type. Null-Terminated.
typedef uint16  POC_Port_T;                                  //port
typedef char    POC_IP_ADDR_T[POC_MAX_IP_ADDR_STR_LEN + 1];  //ip addr string. Null-terminated.
typedef uint32  POC_Session_ID_T;                            //Session ID

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
typedef struct  
{
    char                str_arr[POC_MAX_IPA_STR_LEN ];  //string array
    uint32              str_len;                        //string length
    POC_STR_FORMAT_E    str_format;                     //string code format, (note: str_format must be POC_STR_FORMAT_UCS2)
}POC_IPA_STR_T;

typedef struct  
{
    char                str_arr[POC_MAX_NAME_STR_LEN];  //string array
    uint32              str_len;                        //string length
    POC_STR_FORMAT_E    str_format;                     //string code format, (note: str_format must be POC_STR_FORMAT_UCS2)
}POC_NickName_STR_T;

typedef struct 
{
    POC_URI_T           impi;		                        //SIP URI of IP Multimedia Public Identity
    POC_URI_T           impu;		                        //SIP URI of IP Multimedia Private Identity
    POC_NickName_STR_T  nickname;                           //nick name
    char                password[POC_MAX_PWD_STR_LEN + 1];  //password
    char                msisdn[POC_MAX_MSISDN_STR_LEN + 1]; //msisdn string
    BOOLEAN             need_privacy;                       //whether need privacy
}POC_UserParam_T;

typedef struct 
{
	char            home_network_domain[POC_MAX_URI_STR_LEN + 1];   //Home network domain
	char            apn[POC_MAX_APN_STR_LEN + 1];                   //Access Point 
	POC_IP_ADDR_T   pcscf_ip;                                       //ip addr of p-cscf
	POC_Port_T      pcscf_port;                                     //p-cscf's receiving port
	BOOLEAN	        privacy;                                        //need privacy
	char            user_agent[POC_MAX_UA_STR_LEN + 1];             //user agent string
    //uint32	    max_adhoc_member;                               //Maximum number of Participants allowed for an Ad-hoc PoC Group PoC Session. This comes from DM.
}POC_NetParam_T;

typedef struct 
{
    BOOLEAN             incoming_session_barring;       //TRUE: incoming session barring, FALSE: allowing
    POC_ANSWER_MODE_E   answer_mode;                    //answer mode
    BOOLEAN             incoming_personal_alert_barring;//TRUE: incoming personal alert barring, FALSE: allowing
    BOOLEAN             simultaneous_sessions_support;  //TRUE: simultaneous session support, FALSE: not support
}POC_SettingParam_T;

typedef struct 
{
    uint32 t_speech_pack;       //Time of Speech Pack (in ms)
    uint32 t_speech_dejitter;   //Time of Speech Dejitter (in ms)
    uint32 t_max_rtt;           //Timer of max Round Trip Timer (in ms)
}POC_QoEParam_T;

typedef struct 
{
    char        application_id[POC_MAX_APPID_STR_LEN + 1];      //APPID (Application ID): The application characteristics name for this application, to be used by DM Client to uniquely identify the application.
    char        application_name[POC_MAX_APPNAME_STR_LEN  + 1]; //NAME: Application name. To be displayed in equipment, it's specific for each service provider
    char        provider_id[POC_MAX_PROVIDER_ID_STR_LEN + 1];   //PROVIDER-ID: provides an identifier for the application service access point described by an APPLICATION characteristic.
    char        to_napid[POC_MAX_TO_NAPID_STR_LEN + 1];         //TO-NAPID: This parameter allows an application to refer to a network access point with a matching NAPID parameter. It is only possible to refer to network access points defined within the same provisioning document.
    char        to_appref[POC_MAX_TO_APPREF_STR_LEN + 1];       //TO-APPREF: The TO-APPREF parameter links the APPLICATION characteristics to another secondary APPLICATION characteristic with a matching APPREF parameter.
    BOOLEAN     is_server_pre_estab_session_support;            //Pre-established Session support: A flag to inform whether the PoC Server supports the Pre-established Session functionality
    uint32      simul_session_num;                              //Support for Simultaneous PoC Sessions: Maximum number of Simultaneous PoC Sessions supported for this PoC Client. The value 0 is used to indicate that Simultaneous PoC Sessions are not supported.
    uint32      max_adhoc_group_size;                           //Max Ad-hoc Group size: Maximum number of Participants allowed for an Ad-hoc PoC Group PoC Session.
    POC_URI_T   conference_facotry_uri;                         //Conference-Factory-URI: A SIP URI used for setting up an Ad-hoc PoC Group or 1-1 PoC Session.
    POC_URI_T   explorder_uri;                                  //Exploder-URI: A SIP URI used for sending SIP MESSAGE e.g. Group Advertisement to an URI-list.
    POC_URI_T   conference_uri_template;                        //Conference-URI Template: A template used by the XDM Client to propose a Conference URI when creating a PoC Group document.
    uint32      timer_value_talk_burst_release;                 //T10 (Talk Burst Release) timer: This parameter indicates the value of the T10 (Talk Burst Release) timer in milliseconds
    uint32      timer_value_talk_burst_request;                 //T11 (Talk Burst Request) timer: This parameter indicates the value of the T11 (Talk Burst Request)timer in milliseconds
    uint32      timer_value_end_of_rtp_media;                   //T13 (End of RTP Media) timer: This parameter indicates the value of the T13 (End of RTP Media) timer in milliseconds
    BOOLEAN     is_server_presence_support;                     //PoC Server Presence support: This parameter indicates the Presence publish capability of the PoC Server, the PoC status on behalf of a PoC Client.
}POC_DMParam_T;

typedef struct
{
    POC_URI_T                   conference_uri;     //uri
    POC_NickName_STR_T          nickname;           //nick name
    POC_NOTIFY_STATUS_TYPE_E    notify_status;      //notify status
}POC_NOTIFY_T;

/**-------------------------------------------------------------------------*
**                         MESSAGE STRUCTURE DEFINITION                     *
**--------------------------------------------------------------------------*/
typedef struct 
{
    _SIGNAL_VARS
    uint32  app_task_id;        //app task id
}POC_START_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E err_code;    //POC_OK, 
                                //POC_INTERNAL_ERROR, 
                                //POC_ALREADY_STARTED
}POC_START_CNF_T;


typedef struct 
{
    _SIGNAL_VARS
    POC_UserParam_T     user_param;         //user param
    POC_NetParam_T      net_param;          //net param
    POC_SettingParam_T  setting_param;      //setting param
    POC_QoEParam_T      qoe_param;          //qoe param
    POC_DMParam_T       dm_param;           //dm param
    BOOLEAN             is_param_used[5];   //whether these 5 structures are in use. (Sometimes user may only config PoC with one of the 5 structures)
}POC_CONFIG_REQ_T;


typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E err_code;    //POC_OK, 
                                //POC_INTERNAL_ERROR, 
                                //POC_INVALID_PARAM,
                                //POC_NOT_STARTED,
                                //POC_NOT_CONFIGED,
                                //POC_NOT_REGISTERED
}POC_CONFIG_CNF_T;


typedef struct 
{
    _SIGNAL_VARS
}POC_REGISTRATION_REQ_T;


typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E err_code;    //POC_OK, 
                                //POC_INTERNAL_ERROR, 
                                //POC_INVALID_PARAM,
                                //POC_SERVER_TIME_OUT
                                //POC_ALREADY_REGISTRATION,
                                //POC_AUTH_ERROR,
                                //POC_REQUEST_REJECTED
}POC_REGISTRATION_CNF_T;


typedef struct 
{
    _SIGNAL_VARS
}POC_DEREGISTRATION_REQ_T;


typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E err_code;//POC_OK
}POC_DEREGISTRATION_CNF_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E err_code;//POC_REREGISTRATION_FAIL
}POC_REGISTRATION_STATE_IND_T;

typedef struct 
{
    _SIGNAL_VARS
}POC_PUBLISH_SETTING_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E err_code;    //POC_OK, 
                                //POC_INTERNAL_ERROR, 
                                //POC_INVALID_PARAM,
                                //POC_SERVER_TIME_OUT
}POC_PUBLISH_SETTING_CNF_T;

typedef struct  
{
    _SIGNAL_VARS
    uint32          app_param;  //app param, which will be returned in POC_MO_CALL_INIT_CNF response.
}POC_MOCALL_INIT_REQ_T;


typedef struct  
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id;     //session id
    uint32              app_param;      //app param, which comes from POC_MOCALL_INIT_REQ
    POC_ERR_CODE_E      err_code;       //POC_OK,
                                        //POC_NOT_STARTED,
                                        //POC_NOT_CONFIGED,
                                        //POC_NOT_REGISTERED,
                                        //POC_INTERNAL_ERROR
}POC_MOCALL_INIT_CNF_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T        session_id;         //session id
    uint32                  callee_count;       //callee count
    POC_URI_T               *callee_uri_ptr;    //callee uri array
    POC_SESSION_TYP_E       session_type;       //session type
    POC_SESSION_PRIORITY_E  session_priority;   //session priority
    BOOLEAN                 need_MAO;           //whether need MAO
}POC_MOCALL_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id;    //session id
    //POC_URI_T         uri;        //processing callee uri, [bruce]应该去掉这个参数.参与者的状态是通过SUBSCRIBE信令获得的.
}POC_MOCALL_PROCESSING_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id.
}POC_MOCALL_RINGING_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E      err_code;   //POC_OK
    POC_Session_ID_T    session_id; //session id. Note: cannot access call uri in this signal. It is taken in NOTIFY param.
}POC_MOCALL_ESTABLISH_CNF_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id;     //session id
    POC_ERR_CODE_E      err_code;       //POC_INTERNAL_ERROR, 
                                        //POC_ERR_CODE_EXCEED_MAX_PARTICIPANTS,
                                        //POC_SERVER_TIME_OUT,
                                        //POC_CALLE_TEMP_UNAVAILABLE,
                                        //POC_CALLEE_TIME_OUT,
                                        //POC_ERROR_CALLEE_BLOCK,
                                        //POC_ERROR_CALLEE_LOCK_IN_SESSION,
                                        //POC_REQUEST_REJECTED
}POC_MOCALL_ESTABLISH_FAIL_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_ERR_CODE_E      err_code;   //POC_INTERNAL_ERROR, 
                                    //POC_SERVER_TIME_OUT,
                                    //POC_CALLER_CANCEL_SESSION,
                                    //POC_CALLER_LEAVE_CALL,
                                    //POC_ALL_OTHER_PARTICIPANTS_LEAVE
}POC_CALL_DISCONNECT_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T session_id;    //session id
}POC_MOCALL_CANCEL_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_ERR_CODE_E      err_code;   //POC_OK,
                                    //POC_INVALID_PARAM
}POC_MOCALL_CANCEL_CNF_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id;    //session id
}POC_LEAVE_CALL_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_ERR_CODE_E      err_code;   //POC_OK,
                                    //POC_SERVER_TIME_OUT
}POC_LEAVE_CALL_CNF_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_ANSWER_MODE_E   answer_mode;    //answer mode
    BOOLEAN             is_MAO;         //is MAO
    BOOLEAN             privacy;        //is privacy( if privacy is TRUE, call uri, and nickname is empty)
    POC_URI_T           caller_uri;     //MO caller uri
    POC_NickName_STR_T  nick_name;      //nick name string
    POC_Session_ID_T    session_id;     //session id
    POC_SESSION_TYP_E   session_type;   //session type
}POC_MTCALL_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id;     //session id
}POC_ANSWER_MTCALL_RSP_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id;     //session id
}POC_REJECT_MTCALL_RSP_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_ERR_CODE_E      err_code;   //POC_OK
}POC_MTCALL_ESTABLISH_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_ERR_CODE_E      err_code;   //POC_CALLEE_TIME_OUT
                                    //POC_INTERNAL_ERROR
}POC_MTCALL_ESTABLISH_FAIL_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_URI_T           uri;        //leaving participant uri
    POC_NickName_STR_T  nickname;   //nick name
}POC_LEAVE_CALL_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_URI_T           from_uri;   //from uri
    POC_NickName_STR_T  nickname;   //nick name
    POC_IPA_STR_T       ipa_str;    //Instant Personal Alert string
}POC_IPA_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id;         //session id
    uint32              parti_num;          //participants number
    POC_NOTIFY_T        *parti_param_ptr;   //participants param
}POC_CONFERENCE_NOTIFY_IND_T;               //conference notify indication

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T session_id;    //session id
}POC_FLOOR_REQUEST_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T session_id;    //session id
}POC_FLOOR_RELEASE_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T session_id;    //session id
}POC_FLOOR_GRANTED_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_ERR_CODE_E      err_code;   //POC_ANOTHER_HAS_FLOOR,
                                    //POC_SERVER_ERROR,
                                    //POC_ERROR_ONLY_ONE_PARTICIPANT_IN_GROUP,
                                    //POC_RETRY_AFTER_TIMER_NOT_EXPIRED,
                                    //POC_LISTEN_ONLY
}POC_FLOOR_DENY_IND_T;


typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
}POC_FLOOR_IDLE_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_URI_T           uri;        //floor taken uri
    POC_NickName_STR_T  nick_name;  //floor taken nickname
}POC_FLOOR_TAKEN_IND_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_Session_ID_T    session_id; //session id
    POC_ERR_CODE_E      err_code;   //POC_ERROR_ONLY_ONE_PARTICIPANT_IN_GROUP,
                                    //POC_ERR_CODE_EXCEED_MAX_TALKBRUST_DUARATION,
                                    //POC_NO_PERMISSION_TO_SEND_TALK_BURST,
                                    //POC_TALK_BURST_PREEMPTED
}POC_FLOOR_REVOKE_IND_T;


typedef struct 
{
    _SIGNAL_VARS
    POC_SPEECH_PLAYER_MODE_E    dev_mode;   //device mode
    uint32                      vol;        //volume. The degree of the volume is defined by audio service in our platform.
}POC_SPEECH_PLAYER_SET_PARAM_REQ_T;

typedef struct 
{
    _SIGNAL_VARS
    POC_ERR_CODE_E err_code;    //POC_OK,
                                //POC_INTERNAL_ERROR,
                                //POC_INVALID_PARAM
}POC_SPEECH_PLAYER_SET_PARAM_CNF_T;


/**-------------------------------------------------------------------------*
**                         PUBLIC FUNCTION DECLARE                          *
**--------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : Async function.Create PoC dynamic task , and start PoC PS.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_StartPoC(uint32 app_task_id);

/****************************************************************************/
// 	Description : Async function. Config PoC params, and PoC PS will save these params in ram.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_ConfigPoC(
			const POC_UserParam_T		*user_param_ptr,    //[IN], user param ptr. When equal to NULL, it means that this param is in no use.
			const POC_NetParam_T		*net_param_ptr,     //[IN], net param ptr. When equal to NULL, it means that this param is in no use.
			const POC_SettingParam_T	*setting_param_ptr, //[IN], setting param ptr, When equal to NULL, it means that this param is in no use.
			const POC_QoEParam_T		*qoe_param_ptr,     //[IN], qoe param ptr, When equal to NULL, it means that this param is in no use.
			const POC_DMParam_T		    *dm_param_ptr       //[IN], dm param ptr, When equal to NULL, it means that this param is in no use.
            );


/****************************************************************************/
// 	Description : Async function. Registration
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_Registration(void);

/****************************************************************************/
// 	Description : Async function. De-registration.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_DeRegistration(void);

/****************************************************************************/
// 	Description : Async function. Publish setting to PoC server.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_PublishSetting(void);

/****************************************************************************/
// 	Description : Async function. Init one mo call.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_InitMoCall(
			uint32          app_param   //[IN], app param, which will be returned in POC_MO_CALL_INIT_CNF response.
            );

/****************************************************************************/
// 	Description : Async function.Make an out-going call.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_MakeMoCall(
            POC_Session_ID_T        session_id,     //[IN], session id
			uint32 			        callee_count,   //[IN], callee count
			const POC_URI_T 	    *callee_uri_ptr,//[IN], callee uri array
			POC_SESSION_TYP_E       session_type,   //[IN], session type
			BOOLEAN		            need_MAO,       //[IN], whether need MAO
            POC_SESSION_PRIORITY_E  priority        //[IN], priority
            );

/****************************************************************************/
// 	Description : Async function. Cancel a mo call.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_CancelMoCall(
			POC_Session_ID_T session_id //[IN], session id
            );
	
/****************************************************************************/
// 	Description : Async function. Leave from a call.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_LeaveCall(
			POC_Session_ID_T session_id //[IN], session id
            );

/****************************************************************************/
// 	Description : Async function. Answer mt call.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_AnswerMtCall(
			POC_Session_ID_T session_id //[IN], session id
            );

/****************************************************************************/
// 	Description : Async function. Reject mt call.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_RejectMtCall(
			POC_Session_ID_T session_id //[IN], session id
            );

/****************************************************************************/
// 	Description : Async function. Request floor.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_RequestFloor(
			POC_Session_ID_T session_id //[IN], session id
            );

/****************************************************************************/
// 	Description : Async function. Release floor.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_ReleaseFloor(
			POC_Session_ID_T session_id //[IN], session id
            );

/****************************************************************************/
// 	Description : Async function. Control speech player.
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN POC_ControlSpeechPlayer(
			POC_SPEECH_PLAYER_MODE_E    dev_mode,   //[IN], speech device mode
			uint32                      vol         //[IN], volume
            );

#endif
