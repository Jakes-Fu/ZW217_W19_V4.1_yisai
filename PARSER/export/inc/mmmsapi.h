/*****************************************************************************
** File Name:		mmmsapi.h                                                *
** Author:			kelly.li                                                 *
** Date:			07/01/2009                                               *
** Copyright:		2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:		This file is used to describe the interface of MMS lib   *
*****************************************************************************/

#ifndef _MMSAPI_H_
#define _MMSAPI_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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
//MMS lib config
#define MMS_MAX_FILE_NAME_LEN                       SFS_MAX_PATH	//max file name string length
#define MMS_FILE_NAME_LEN			                SFS_MAX_PATH     //mms file name len
#define MMS_MAX_URL_LENGTH				            256     //max url len


//macro define of message priority value 
#define	MMS_PRIORITY_VALUE_LOW		                128     //priority low
#define	MMS_PRIORITY_VALUE_NORMAL	                129     //priority normal
#define	MMS_PRIORITY_VALUE_HIGH		                130     //priority high

//macro define of send visibility value 
#define	MMS_SENDER_VISIBILITY_HIDE			        128     //visibility hide
#define	MMS_SENDER_VISIBILITY_SHOW			        129     //visibility show

//macro define of delivery report head value    
#define	MMS_DELIVERY_REPORT_YES                     1       
#define	MMS_DELIVERY_REPORT_NO                      0    
  
//macro define of read report head value  
#define	MMS_READ_REPORT_YES                         1         
#define	MMS_READ_REPORT_NO                          0

//macro define of report allowed head value 
#define MMS_REPORT_ALLOWED_YES                      128
#define MMS_REPORT_ALLOWED_NO                       129

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef void*	MMS_DOCUMENT_HANDLE;		                                //MMS document handle
typedef void (*MMS_RESULT_INFO_FUNC)(int32, void *,int32);                  //mms call back function
typedef void* (*MMS_ALLOC_FUNC)(uint32 size,char *file_ptr, uint32 line);   //memory alloc callback
typedef void (*MMS_FREE_FUNC)(void *ptr);                                   //memory free callback
    

typedef enum
{
	MMS_PUSH_TYPE_DELIVERY_IND,      //delivery reporting, the recipient has received the message
	MMS_PUSH_TYPE_NOTIFICATION_IND,  //there is new mms
	MMS_PUSH_TYPE_READORIG_IND,      //the recipient has read the mms
	MMS_PUSH_TYPE_UNKNOW
}MMS_PUSH_TYPE_E;

typedef enum
{
	MMS_STATUS_SEND_RETURN,		//finish sending
	MMS_STATUS_RECV_RETURN,		//finish receiving
	MMS_STATUS_SEND_RETRYING,   //mms lib is sending mms again
	MMS_STATUS_RECV_RETRYING,   //mms lib is receiving mms again
	MMS_STATUS_NORMAL,			//normal status, mms lib is sending or receiving mms  
	MMS_STATUS_SENDED_BYTES,    //the bytes of sended mms
	MMS_STATUS_RECVED_BYTES,    //the bytes of received mms
    MMS_STATUS_MAX
}MMS_STATUS_E; 

//MMS库回调函数mmsproc参数定义,向上层反映库的状态
typedef enum
{
	MMS_NETLINKING,                         //pdp activing
	MMS_NETLINK_SUCCESS,                    //pdp active success
	MMS_NETLINK_FAILED,                     //pdp active fail
	MMS_NETLINK_DATA_ROAMING_DISABLE,       //data roaming disabled(added by feng.xiao)
 	MMS_NETLINK_FDN_ONLY,                     //pdp FDN not include"*99#"...

    MMS_SOCKET_CONNECTING,                  //connecting socket for tcp, connecting proxy for wsp
	MMS_CONNECT_SUCCESS,                    //connect success
	MMS_RECEIVING,                          //mms receiving
	MMS_SENDING,                            //mms sending
	MMS_RECV_FAILED,                        //mms recv fail
	MMS_RECV_FAILED_BY_NOT_FOUND,           //mms recv fail by not found
	MMS_RECV_SUCCESS,                       //mms recv success
	MMS_OPR_CANCEL,                         //user request cancel
    //other failed reasons without connection
    MMS_RECV_FAILED_NO_SPACE,               //no space
    MMS_RECV_FAILED_INVALIDPERIOD,          //invalid period
    MMS_RECV_FAILED_INVALID_URL,            //invalid url
    MMS_RECV_FAILED_NEED_AUTH,              //receive mms need auth,may download other simcard's push message
    MMS_RECV_FAILED_ERROR_DATA,             //receive mms fail because error network data:such as not mms data
    MMS_FAILED_OTHER_VALUE,                 
	// macro define of Response status value 
	MMS_RESPONSE_STATUS_VALUE_OK					=128,
	MMS_RESPONSE_STATUS_VALUE_E_UNSPECIFIED			=129,
	MMS_RESPONSE_STATUS_VALUE_E_SERVICE_DENIED		=130,
	MMS_RESPONSE_STATUS_VALUE_E_FORMAT_CORRUPT		=131,
	MMS_RESPONSE_STATUS_VALUE_E_ADDRESS_UNRESOLVED	=132,
	MMS_RESPONSE_STATUS_VALUE_E_MESSAGE_NOT_FOUND	=133,
	MMS_RESPONSE_STATUS_VALUE_E_NETWORK_PROBLEM		=134,
	MMS_RESPONSE_STATUS_VALUE_E_NOT_ACCEPTED		=135,
	MMS_RESPONSE_STATUS_VALUE_E_UNSUPORTED_MESSAGE	=136,
	MMS_RESPONSE_STATUS_VALUE_E_UNKNOW_ERROR        =999
}MMS_STATUS_VALUE_E;

//macro define of status value 
typedef enum
{
    MMS_TYPE_TEXT = 0x01,
    MMS_TYPE_WBMP =	0x02,
    MMS_TYPE_GIF  =	0x03,
    MMS_TYPE_JPEG = 0x04,
    MMS_TYPE_WAV  = 0x05,
    MMS_TYPE_AMR  = 0x06,    
    MMS_TYPE_MIDI = 0x07,
    MMS_TYPE_MP3  = 0x08,
    MMS_TYPE_IMY  = 0x09,
    MMS_TYPE_MMF  = 0x0A,
    MMS_TYPE_BMP1 = 0x0B,
    MMS_TYPE_MP4  = 0x0C,
    MMS_TYPE_PNG  = 0x0D,
    MMS_TYPE_3GP  = 0x0E,   
    MMS_TYPE_WMA  = 0x0F,   
    MMS_TYPE_AAC  = 0x10, 
    MMS_TYPE_M4A  = 0x11, 
    MMS_TYPE_AVI  = 0x12,
    MMS_TYPE_VCF  = 0x13,
//MMS_TYPE_IMAGE_UNKNOWN  0x13
//MMS_TYPE_AUDIO_UNKNOWN  0x14
//MMS_TYPE_VIDEO_UNKNOWN  0x15
//MMS_TYPE_UNKNOWN        0x16
    MMS_TYPE_MAX
}MMS_CONTENT_TYPE_E; 

//macro define of status value 
typedef enum
{
	MMS_STATUS_VALUE_EXPIRED        = 128,	//the message has expired	
	MMS_STATUS_VALUE_RETRIVED       = 129,	//the message is retrived
	MMS_STATUS_VALUE_REJECTED       = 130,	//the message is rejected	
	MMS_STATUS_VALUE_DEFERRED       = 131,  //the message is deferred
	MMS_STATUS_VALUE_UNRECOGNISED   = 132   //the message is unrecognised
}MMS_MESSAGE_STATUS_VALUE_E; 

//macro define of from message type value
typedef enum
{
    MMS_MESSAGE_TYPE_SEND_REQ           = 128,  
    MMS_MESSAGE_TYPE_SEND_CONF			= 129,
    MMS_MESSAGE_TYPE_NOTIFICATION_IND	= 130,
    MMS_MESSAGE_TYPE_NOTIFYRESP_IND		= 131,
    MMS_MESSAGE_TYPE_RETRIVE_CONF		= 132,
    MMS_MESSAGE_TYPE_ACKNOWLEDGE_IND	= 133,
    MMS_MESSAGE_TYPE_DELIVERY_IND		= 134,
    MMS_MESSAGE_TYPE_READREC_IND		= 135,
    MMS_MESSAGE_TYPE_READORIG_IND		= 136,
    MMS_MESSAGE_TYPE_MAX
}MMS_MESSAGE_TYPE_E; 


//macro define of from mms head filed  type value
typedef enum
{
    MMS_BCC					= 0x01,
    MMS_CC					= 0x02,
    MMS_CONTENT_LOCATION	= 0x03,
    MMS_CONTENT_TYPE		= 0x04,
    MMS_DATE				= 0x05,
    MMS_DELIVERY_REPORT		= 0x06,
    MMS_DELIVERY_TIME		= 0x07,
    MMS_EXPIRY				= 0x08,
    MMS_FROM				= 0x09,
    MMS_MESSAGE_CLASS		= 0x0a,
    MMS_MESSAGE_ID			= 0x0b,
    MMS_MESSAGE_TYPE		= 0x0c,
    MMS_MMS_VERSION			= 0x0d,
    MMS_MESSAGE_SIZE		= 0x0e,
    MMS_PRIORITY			= 0x0f,
    MMS_READ_REPORT			= 0x10,
    MMS_REPORT_ALLOWED		= 0x11,
    MMS_RESPONSE_STATUS		= 0x12,
    MMS_RESPONSE_TEXT		= 0x13,
    MMS_SENDER_VISIBILITY	= 0x14,
    MMS_STATUS				= 0x15,
    MMS_SUBJECT				= 0x16,
    MMS_TO					= 0x17,
    MMS_TRANSACTION_ID		= 0x18,
    MMS_READ_STATUS         = 0x1b,
    MMS_HEAD_FIELD_MAX
}MMS_HEAD_FIELD_TYPE_E; 

//macro define of from message class type value
typedef enum
{
    MMS_MESSAGE_CLASS_TYPE_PERSONAL		    = 128,//class type personal
    MMS_MESSAGE_CLASS_TYPE_ADVERTISEMENT	= 129,//class type advertisement
    MMS_MESSAGE_CLASS_TYPE_INFORMATIONAL	= 130,//class type informational
    MMS_MESSAGE_CLASS_TYPE_AUTO				= 131,//class type auto
    MMS_MESSAGE_CLASS_TYPE_MAX
}MMS_MESSAGE_CLASS_TYPE_E; 

typedef enum 
{
	MMS_ORDER_TEXT_FIRST,  //first display text then image
	MMS_ORDER_IMAGE_FIRST, //first display image then text
    MMS_ORDER_INVALID
}MMS_TEXT_IMAGE_ORDER_E; 

//begin 11.03.24 forcr 234015
typedef enum 
{
    MMS_TEXT_CHAR_MIN,
    MMS_TEXT_CHAR_ANSI,
	MMS_TEXT_CHARE_BG5, 
    MMS_TEXT_CHAR_UCS2_LE,
    MMS_TEXT_CHAR_UCS2_BE,
    MMS_TEXT_CHAR_UTF8,
    MMS_TEXT_CHAR_WINDOW1251,
    MMS_TEXT_CHAR_CP855,
    MMS_TEXT_CHAR_CP866,
    MMS_TEXT_CHAR_KOI8R,
    MMS_TEXT_CHAR_MACCYRILLIC,
    MMS_TEXT_CHAR_MAX
}MMS_TEXT_ENCODE_TYPE_E; 
//end

typedef struct _MMS_CONTENT
{
	struct _MMS_CONTENT	*prev;			//prev content info
	struct _MMS_CONTENT	*next;			//next content info
	int32		         x;				/*位置x*///可以考虑不要
	int32			   	 y;				/*位置y*/////可以考虑不要
	int32			 	width;			//display width
	int32				height;			//display height
	unsigned int		type;			//content type
	unsigned int		time_start;		//start play time(ms)
	unsigned int		time_end;		//finish play time(ms)
	unsigned int		color;			//define for text content 
	char				filename[MMS_FILE_NAME_LEN]; //content file name
	char				forwardlock;	//1:drm forwardlocked content; 0 free content 
	int32               contentsize;       
	unsigned char		size;			//size, define for text content
}MMS_CONTENT_T,*P_MMS_CONTENT_T;

typedef struct _MMS_SLIDE
{
	struct _MMS_SLIDE	*prev;				            //prev slide
	struct _MMS_SLIDE	*next;				            //next slide
	unsigned int		times;			                //play time(ms)
	unsigned int		color;			                //background color
	char				audiofile[MMS_FILE_NAME_LEN];   //audio file name
	uint8				audio_type;				        //background audio type
	char				audio_forwardlock;	            //1:drm forwardlocked content; 0 free content 
	char				text_flag;                      //if contains text
	char				image_flag;                     //if contains image
	char				video_flag;                     //if contains video
	P_MMS_CONTENT_T		content_root;	                //slide content struct
	P_MMS_CONTENT_T		content_cur;	                //current slide content
	int32               audiosize;                      //audio size
	MMS_TEXT_IMAGE_ORDER_E				textimage_order; //the order of text and image
}MMS_SLIDE_T,*P_MMS_SLIDE_T;

typedef struct _MMS_ACCESSORY_PARAM
{
    char				            filename[MMS_FILE_NAME_LEN];            //content file name
    char                            content_type_string[MMS_FILE_NAME_LEN]; //content type string info
    uint32		                    type;			                        //content type int value
    uint32                          contentsize;                            //content size
    struct _MMS_ACCESSORY_PARAM*    next;                                   //net accessory info
}MMS_ACCESSORY_PARAM_T;

typedef struct _MMS_EDIT_BODY
{
	int32		            width;			//default screen width
	int32			        height;			//default screen height
	int32		            ftype;			//file type
	P_MMS_SLIDE_T		    slide_root;		//sile info
	P_MMS_SLIDE_T		    slide_cur;		//current slide info
    MMS_ACCESSORY_PARAM_T*  accessory_ptr;  //the accessory info in body
	int32                   totalsize;      //mms total size
}MMS_EDIT_BODY_T,*P_MMS_EDIT_BODY_T;


typedef struct _MMS_SEND_SETTING
{
    uint32  max_times;         //if >1, need mms lib to retry, ==1 need not retry 
	char    *send_vfilename;   //virtual file name for to send mms content
	char    *conf_vfilename;   //virtual file name for mms response
}MMS_SEND_SETTING_T;

typedef struct _MMS_RECV_SETTING
{
    uint32  max_times;              //if >1, need mms lib to retry, ==1 need not retry 
	char*   get_url;                //get url info
	int32   immediate_recv;         //if receive immediately
	int32   recv_msgsize;           // rcv msg max size
    BOOLEAN is_permit_send_report;  //if permit proxy send delivery report to MMS originator
    uint16	recv_file_name[MMS_MAX_FILE_NAME_LEN + 1]; //sys file name for received mms
}MMS_RECV_SETTING_T;

typedef struct _MMS_CONFIG_SETTING
{
	char*           wap_gateway;   //gateway info, CMCC:10.0.0.172
	char*           mms_centery;   //MMScenter，CMCC:"Http://mmsc.monternet.com/"
    char*           user_name_ptr; //the proxy user name
    char*           password_ptr;  //the proxy password
    char*           user_agent_ptr;//user agent info 
    char*           ua_profile_ptr; //user agent profile info
	uint16          wap_port;      //port number, wsp: 9201
    BOOLEAN         is_http;       //if net stack is http, FALSE:WSP
    uint32          net_id;        //the net id for socket binding
    uint32          app_module_id;
}MMS_CONFIG_SETTING_T;

typedef struct _MMS_NOTIFICATION_IND_MSG
{
	int32   msgsize;                //message size
	int32   expiry_time;            //expiry time
	int32   de_date;				//delivery report date
	int32   de_status;              //delivery report status
	int32   message_class_value;	//message class value
	char *  de_msgid;               //delivery report message id
	char *  de_to;                  //delivery report receiver
    char *  mms_url;                //the URL of mms
    char *  phone_number;           //phone number of originate
    char *  subject;                //MMS subject
	char *  tid;                    //transaction ID
}MMS_NOTIFICATION_IND_MSG_T;

typedef struct 
{
    MMS_ALLOC_FUNC   alloc_cb_ptr;  //memory alloc function
    MMS_FREE_FUNC    free_cb_ptr;   //memory free function
}MMS_INIT_PARAM;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init the mms lib
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMS_Init(const MMS_INIT_PARAM *init_param_ptr);

/*****************************************************************************/
//  Description : mmsClose
//  Global resource dependence : 
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMS_Close(void);

/*****************************************************************************/
//  Description :new mms
//  Global resource dependence : 
//  Author: kelly.li
//  Note: MMS_DOCUMENT_HANDLE: success    0:failed
/*****************************************************************************/
PUBLIC MMS_DOCUMENT_HANDLE MMS_CreateNewMms(void);

/*****************************************************************************/
//  Description :delete the mms
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  pMmsDoc		a mms document pointer
//  Return: 1: success    0:failed	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_DeleteMms(MMS_DOCUMENT_HANDLE mms_handle);

/*****************************************************************************/
//  Description :set the default value of mms head 
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  pMmsDoc		a mms document pointer
//  Param:  mmstype		mms document type
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_SetHeadByType(
                               MMS_DOCUMENT_HANDLE mms_handle, //[IN]mms document handle
                               uint8               mms_type    //[IN]mms type
                               );

/*****************************************************************************/
//  Description :set the mms file
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  pMmsDoc		a mms document pointer
//  Param:  pMmsFile	mms document filename to be appointmented
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_SetMmsFile(
                            MMS_DOCUMENT_HANDLE mms_handle,     //[IN]mms document handle
                            const char*         file_name_ptr   //[IN]mms document file name
                            );

/*****************************************************************************/
//  Description :set the mms file
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  pMmsDoc		a mms document pointer
//  Param:  pMmsFile	a buffer to store file name of mms document
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_GetMmsFile(
                            MMS_DOCUMENT_HANDLE mms_handle,     //[IN]mms document handle
                            char *              name_buf_ptr,   //[OUT]mms document file name
                            uint32              buf_len         //[IN]file name buffer size
                            );

/*****************************************************************************/
//  Description :set the mms file
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  pMmsDoc		a mms document pointer
//  Param:  pMmsFile	mms head filed to be set value
//  Param:  ivalue		int32 or int64 value of head field, 
//					only one of ivalue and svalue is valid.
//  Param:  pValue		string value of mms head field,
//					only one of ivalue and svalue is valid.
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_SetHeadField(
                              MMS_DOCUMENT_HANDLE   mms_handle,     //[IN]mms document handle
                              int32                 mms_head,       //[IN]mms head type
                              long                  head_value,     //[IN]mms head value
                              const char*           head_value_ptr  //[IN]mms head value ptr
                              );

/*****************************************************************************/
//  Description :get the field of mms head  
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  mms_handle		a mms document pointer
//  Param:  mms_head		mms head iled to be set value
//  Param:  head_int_ptr	int32 or int64 value pointer to get head field, only one of ivalue and svalue is valid.
//  Param:  head_str_ptr	string buffer to get mms head field,only one of ivalue and svalue is valid.  
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_GetHeadField(
                              MMS_DOCUMENT_HANDLE   mms_handle,     //[IN]mms document handle
                              int32                 mms_head,       //[IN]mms head type
                              int32*                head_int_ptr,   //[OUT]mms head int value ptr
                              char*                 head_str_ptr    //[OUT]mms head str value ptr
                              );    

/*****************************************************************************/
//  Description :get the field of mms edit body 
//  Global resource dependence : 
//  Author:kelly.li
//  Param:  mms_handle:		    a mms document pointer
//  Param:  edit_body_ptr:		mms edit document ptr  
//  Return: 1: success       0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_GetEditBody(
                             MMS_DOCUMENT_HANDLE mms_handle,    //[IN]mms document handle
                             P_MMS_EDIT_BODY_T   edit_body_ptr  //[IN]edit body ptr
                             );

/*****************************************************************************/
//  Description :encode the mms 
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  mms_handle		a mms document pointer
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_EncodeMms(
                           MMS_DOCUMENT_HANDLE mms_handle //[IN]mms document handle
                           );

/*****************************************************************************/
//  Description :decode the mms
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  mms_handle		a mms document pointer
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_DecodeMms(
                           MMS_DOCUMENT_HANDLE mms_handle //[IN]mms document handle
                           );

/*****************************************************************************/
//  Description :get mms header from sys file, and save in vf 
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  file_name_ptr: sys file name
//  Param:  file_name_len: sys file name len
//  Param:  vf_name_ptr: the vf name to save mms header
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_GetHeaderFromSysFile(
                              uint16*       file_name_ptr,  //[IN]sys file name
                              uint32        file_name_len,  //[IN]sys file name len
                              char*         vf_name_ptr     //[IN]the vf name to save mms header
                               );

/*****************************************************************************/
//  Description :just decode the mms head
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  mms_handle		a mms document pointer
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_DecodeMmsHead(
                               MMS_DOCUMENT_HANDLE mms_handle //[IN]mms document handle
                               );

/*****************************************************************************/
//  Description :init the send and receive configure,such as port,mmsc etc.
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_SendRecvInit(
                              const MMS_CONFIG_SETTING_T *config_setting_ptr //[IN] init setting ptr
                              );

/*****************************************************************************/
//  Description :free the virtual file and buffer for mms send/rcv
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_SendRecvClose(void);

/*****************************************************************************/
//  Description :mms msg dispatch fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 1:mms lib handle the message  0:mms lib does not handle the message, does not match
/*****************************************************************************/
PUBLIC BOOLEAN MMS_HandleNetMsg(
                                const xSignalHeaderRec* sig_ptr //[IN]sig message ptr
                                );

/*****************************************************************************/
//  Description :send mms
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  pSendSetting	the point of send setting
//  Param:  proc			callback of send mms
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_Send(
                      const MMS_SEND_SETTING_T *send_setting_ptr,//[IN]mms send setting ptr
                      MMS_RESULT_INFO_FUNC      proc             //[IN]action callback function
                      );

/*****************************************************************************/
//  Description :receive the mms and save in sys file
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  pRecvSetting	the setting value when receive mms
//  Param:  proc			the callback of receive mms
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_Receive(
                         const MMS_RECV_SETTING_T *recv_setting_ptr,  //[IN]mms recv setting ptr
                         MMS_RESULT_INFO_FUNC       proc              //[IN]action callback function
                         );

/*****************************************************************************/
//  Description :send or receive cancel
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  none
//  Return: 1: success    0:failed 	
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_CancelSendRecv(void);

/*****************************************************************************/
//  Description :sms process the push message
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  user_data_ptr		sms user data    
//  Param:  data_len		    the length of sms user data	
//  Param:  is_exist_user_head	if sms userdata has user data head. 
//  Param:  proc                the callback of sms 
//  Return: 1: success    0:failed 
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_ParsePushMsg(
                              uint8*                user_data_ptr,      //[IN]push user data ptr
                              int32                 data_len,           //[IN]push user data len
                              BOOLEAN               is_exist_user_head, //[IN]if user data contains user head
                              MMS_RESULT_INFO_FUNC  proc                //[IN]call back function info
                              );

/*****************************************************************************/
//  Description :convert the format from P_MMS_EDIT_BODY_T to MMS_DOCUMENT_HANDLE
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  edit_body_ptr	a document to be integrated
//  Return: no NULL:success; NULL:failed	
//  Note: 
/*****************************************************************************/
PUBLIC MMS_DOCUMENT_HANDLE MMS_IntegrateMmsBody(
                                                P_MMS_EDIT_BODY_T edit_body_ptr //[IN]mms edit body ptr
                                                );

//MMS edit api begin
/*****************************************************************************/
//  Description :delete content of a MMS
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  slide_ptr	  the slide to be deleted with content
//  Param:  edit_body_ptr the edit document to be deleted with content
//  Return: void
//  Note: 
/*****************************************************************************/
PUBLIC void MMS_DelContOfSlide(
                               P_MMS_SLIDE_T        slide_ptr,      //[IN]mms slide ptr
                               P_MMS_EDIT_BODY_T    edit_body_ptr   //[IN]mms edit body ptr
                               );

/*****************************************************************************/
//  Description :new a MMS edit document
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  void
//  Return: pDocument  the edit document new
//  Note: 
/*****************************************************************************/
PUBLIC P_MMS_EDIT_BODY_T MMS_CreateNewEditDoc(void);

/*****************************************************************************/
//  Description :delete slide
//  Global resource dependence : 
//  Author: kelly.li
//  Param:  edit_body_ptr: the edit document to new
//  Return: void
//  Note: 
/*****************************************************************************/
PUBLIC void MMS_DelSlideOfEdoc(
                               P_MMS_EDIT_BODY_T    edit_body_ptr   //[IN]mms edit body ptr
                               );

/*****************************************************************************/
//  Description :delete edit document of a MMS
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMS_DeleteEditDoc(P_MMS_EDIT_BODY_T pDocument);

/*****************************************************************************/
//  Description :mms create new slide for edit
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC P_MMS_SLIDE_T MMS_CreateNewSlide(P_MMS_EDIT_BODY_T pDocument);

/*****************************************************************************/
//  Description :add mms content to slide
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC P_MMS_CONTENT_T MMS_AddConetnt(
                                     char*              pFilename,      //[IN]the filename of content
                                     uint32             type,           //[IN]the type of content
                                     P_MMS_SLIDE_T      pSlide,         //[IN]the slide to be added with content
                                     P_MMS_EDIT_BODY_T  pDocument       //the edit doc to be added with content
                                     );

/*****************************************************************************/
//  Description :add mms new audio to slide
//  Global resource dependence : 
//  Author:kelly.li
// @param   pFilename the filename of audio
// @param   type	  the type of audio
// @param   pSlide	  the slide to be added with audio
// @param   pDocument the edit document to be added with audio
// @return  0		  success
//			-1		  fail
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_AddNewAudio(
                             uint8 *            pFilename, //[IN]
                             uint32             type,      //[IN]
                             P_MMS_SLIDE_T      pSlide,    //[IN]
                             P_MMS_EDIT_BODY_T  pDocument  //[IN]
                             );

/*****************************************************************************/
//  Description :Get slide counts of a edit document
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
//  Return:count the counts of slides  of a edit document  
/*****************************************************************************/
PUBLIC uint16 MMS_GetSlideCount(
                            P_MMS_EDIT_BODY_T pDocument     //[IN]pDocument the edit document to be gotten
                            );

/*****************************************************************************/
//  Description :delete audio of a MMS
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMS_DeleteAudio(
                         P_MMS_SLIDE_T      pSlide,     //[IN]the slide to be deleted with audio
                         P_MMS_EDIT_BODY_T  pDocument   //[IN]the edit document to be deleted with audio
                         );

/*****************************************************************************/
//  Description :get slide index
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMS_GetSlideIndex(
                            P_MMS_EDIT_BODY_T pDocument //[IN] pDocument the edocument to be gotten
                            );

/*****************************************************************************/
//  Description :set text image order of a MMS
//  Global resource dependence : 
//  param: pDocument :the edit-document to be set
//  param: order_type: order details
//  Author:kelly.li
//  Return: 1: success    0:failed 
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMS_SetSlideTextImageOrder(
                            P_MMS_EDIT_BODY_T       pDocument,  //[IN] pDocument that to de setted
                            MMS_TEXT_IMAGE_ORDER_E  order_type  //[IN] order type
                            );

/*****************************************************************************/
//  Description :get text image order of a MMS
//  Global resource dependence : 
//  param: pDocument :the edit-document to be get
//  Author:kelly.li
//  Return: MMS_TEXT_IMAGE_ORDER_E: order details
//  Note: 
/*****************************************************************************/
PUBLIC MMS_TEXT_IMAGE_ORDER_E  MMS_GetSlideTextImageOrder(
                                                        P_MMS_EDIT_BODY_T       pDocument  //[IN] pDocument that to de setted
                                                        );

/*****************************************************************************/
//  Description :Get all content counts of a edit document
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMS_GetAllContentCount(
                             P_MMS_EDIT_BODY_T pDocument    //[IN]pDocument the edit document to be gotten
                             );

/*****************************************************************************/
// 	Description: get the seconds from appointed time to 1970年1月1日零点
//	Global resource dependence:
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMSAdapter_Tm2SecondFrom1970( 
                      uint32 tm_sec,    //[IN]
                      uint32 tm_min,    //[IN]
                      uint32 tm_hour,   //[IN]
                      uint32 tm_mday,   //[IN]
                      uint32 tm_mon,    //[IN]
                      uint32 tm_year    //[IN]
                      );

/*****************************************************************************/
// 	Description : change gb to wstr
//	Global resource dependence : none
//  Author: kelly.li
//  Return: the number of UCS2 code that have been converted.
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMSAdapter_GBToWstr(					
                                   uint16 *wstr_ptr, 	//destination: string pointer to UCS2 string.
                                   const uint8 *gb_ptr, //source: string pointer to GB string
                                   uint32 len			//the length to convert from GB to UCS2.
                                   );

/*****************************************************************************/
// 	Description : other code to wstr
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMSAdapter_OtherCodeToWstr(//ucs2b len
                                  uint16 *wstr_ptr,//out
                                  uint32 wstr_len,//in
                                  MMS_TEXT_ENCODE_TYPE_E code_type,//in
                                  const uint8 *code_ptr,//in
                                  uint32 code_len//in
                                  );

/*****************************************************************************/
// 	Description : judge the type of text file
//	Global resource dependence : none
//  Author: MINGHU.MAO
//  Return: judge the type of text by content
//	Note: 
/*****************************************************************************/
PUBLIC MMS_TEXT_ENCODE_TYPE_E MMSAdapter_GetTextCharType(					
                                                         uint8 *code_ptr, //source: content
                                                         uint32 code_len			//the length to judge
                                                         );

/*****************************************************************************/
//  Description :mms create new slide for edit
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC P_MMS_SLIDE_T MMS_CreateNewSlideR(P_MMS_EDIT_BODY_T pDocument);

/*****************************************************************************/
//  Description :add attachment file to edit body
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMS_AddAttachment(
                             char*              pFilename,      //[IN]the filename of content
                             P_MMS_EDIT_BODY_T  pDocument       //the edit doc to be added with attachment
                             );

/*****************************************************************************/
//  Description :delete attachment from edit body
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMS_DeleteAttachment(
                             char*              pFilename,      //[IN]the filename of content
                             P_MMS_EDIT_BODY_T  pDocument       //[IN]the edit doc
                             );

/*****************************************************************************/
//  Description :get attachment number of the edit body
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMS_GetAttachmentNumber(
                             P_MMS_EDIT_BODY_T  pDocument,          //[IN]the edit doc
                             uint32             *attachment_num_ptr //[OUT]the total number of attachment
                             );

/*****************************************************************************/
//  Description :Get appointed attachment file size
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMS_GetAttachmentSize(
                             char               *pFilename,     //[IN]the filename of content
                             P_MMS_EDIT_BODY_T  pDocument,      //[IN]the edit doc
                             uint32             *file_size_ptr  //[OUT]appointed attachment file size
                             );

/*****************************************************************************/
//  Description :replace old attachment file with new attachment file
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMS_ReplaceAttachment(
                             char               *old_filename_ptr,     //[IN]the filename of old attachment
                             char               *new_filename_ptr,     //[IN]the filename of new attachment
                             P_MMS_EDIT_BODY_T  pDocument              //[IN]the edit doc
                             );


/*****************************************************************************/
//  Description :Set mms slide time
//  Global resource dependence : 
//  Author:sally.he
//  Note: 
/*****************************************************************************/
PUBLIC void MMS_SetSlideTimes(uint32 slide_times);
//MMS edit api end
#endif
