/*****************************************************************************
** File Name:      mmivp_dummy.h                                              *
** Author:         juan.zhang                                                *
** Date:           1/19/2009                                                 *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    streaming api head file                                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/19/2009     juan.zhang          Create                                  *
******************************************************************************/

#ifndef _MMIVP_DUMMY_H_
#define _MMIVP_DUMMY_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "dal_player.h"
#include "mn_type.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/
typedef uint32  STRM_HANDLE;
typedef uint16  STRM_PORT_T;
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define STRM_MAX_MIDEA_TYPE_LEN 10
#define STRM_TIME_RANGE_NO_LIMITED 0xffffffff
#define STRM_PRO_LEVEL_ID_LEN    3
#define STRM_SPROP_SETS_MAX_NUM  20

#define _STRM_SIGNAL_VARS \
    		_SIGNAL_VARS \
			STRM_HANDLE strm_handle;			//Streamingœ˚œ¢Õ∑

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
typedef enum
{
    STRM_RESULT_SUCCESS,
    STRM_RESULT_TIMEOUT,
    STRM_RESULT_INVALID_STATE,
    STRM_RESULT_UNSUPPORTED,                                
    STRM_RESULT_OTHER_ERROR,
    STRM_RESULT_BAD_REQUEST                         = 400,
    STRM_RESULT_UNAUTHORIZED                        = 401,
    STRM_RESULT_PAYMENT_REQUIRED                    = 402,
    STRM_RESULT_FORBIDDEN                           = 403,
    STRM_RESULT_NOT_FOUND                           = 404,
    STRM_RESULT_METHOD_NOT_ALLOWED                  = 405,
    STRM_RESULT_NOT_ACCEPTABLE                      = 406,
    STRM_RESULT_PROXY_AUTHENTICATION                = 407,
    STRM_RESULT_REQUEST_TIMEOUT                     = 408,
    STRM_RESULT_GONE                                = 410,
    STRM_RESULT_LENGTH_REQUIRED                     = 411,
    STRM_RESULT_PRECONDITION_FAILED                 = 412,
    STRM_RESULT_REQUEST_ENTITY_TOO_LARGE            = 413,
    STRM_RESULT_REQUEST_URI_TOO_LONG                = 414,
    STRM_RESULT_UNSUPPORTED_MEDIA_TYPE              = 415,
    STRM_RESULT_INVALID_PARAMETER                   = 451,
    STRM_RESULT_ILLEGAL_CONFERENCE_IDENTIFIER       = 452,
    STRM_RESULT_NOT_ENOUGH_BANDWITH                 = 453,
    STRM_RESULT_SESSION_NOT_FOUND                   = 454,
    STRM_RESULT_METHOD_NOT_INVALID_IN_THIS_STATE    = 455,
    STRM_RESULT_HEADER_FIELD_NOT_VALID              = 456,
    STRM_RESULT_INVALID_RANGE                       = 457,
    STRM_RESULT_PARAMETER_IS_READ_ONLY              = 458,
    STRM_RESULT_AGGREGATE_NOT_ALLOWED               = 459,
    STRM_RESULT_ONLY_AGGREGATE_ALLOWED              = 460,
    STRM_RESULT_UNSUPPORTED_TRANSPORT               = 461,
    STRM_RESULT_DESTINATION_UNREACHABLE             = 462,
    STRM_RESULT_INTERNAL_SERVER_ERROR               = 500,
    STRM_RESULT_NOT_IMPLEMENTED                     = 501,
    STRM_RESULT_BAD_GATEWAY                         = 502,
    STRM_RESULT_SERVER_UNAVAILABLE                  = 503,
    STRM_RESULT_GATEWAY_TIMEOUT                     = 504,
    STRM_RESULT_VERSION_NOT_SUPPORTED               = 505,
    STRM_RESULT_OPTION_NOT_SUPPORT                  = 551,
    STRM_RESULT_UNKNOWN                             = 600,
    STRM_RESULT_MAX
}STRM_RESULT_E;

typedef enum
{
    STRM_CHARSET_UTF8,
    STRM_CHARSET_ASCII,
    STRM_CHARSET_UNICODE,
    STRM_CHARSET_MAX
}STRM_CHARSET_E;

typedef enum
{
    STRM_SDP_ENC_UNSUPPORTED,
    STRM_SDP_ENC_AMR,
    STRM_SDP_ENC_H263,
    STRM_SDP_ENC_H263_PLUS,
    STRM_SDP_ENC_H264
}STRM_SDP_ENC_E;

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

typedef struct
{
	uint32 start_time;          //the accurate start time in ms
	uint32 end_before_time;     //the time in ms that the stream will be end before, 
                                //this can be set to STRM_TIME_RANGE_NO_LIMITED, which means
                                //unlimited end time, and in this case, the end time is depend
                                //on the actual end time of the media
}STRM_TIME_RANGE_T;

typedef struct
{
    uint8	           payload_type;           //audio payload type
    char               *audio_type_name_ptr;   //audio payload type name got from SDP
    STRM_TIME_RANGE_T  time_range;             //a=range:npt       
    uint32	           sample_rate;            //the clock rate got from SDP
    uint8  	           channel;                //the audio channel number
    uint32	           config_len;             //the audio config length, not used in this version
    uint8	           *config_ptr;            //the audio config buffer got from SDP which is always used in AAC, not used in this version
    char	           *url_ptr;               //stream url

}STRM_AUDIO_PARAM_T;

typedef struct
{
    uint8	           payload_type;         //video payload type
    char  	           *video_type_name_ptr; //video payload type name
    STRM_TIME_RANGE_T  time_range;           //a=range:npt  
    uint32	           sample_rate;          //the clock rate
    uint32             frame_width;	         //video frame width in pixel
    uint32             frame_height;         //video frame height in pixel
    uint32             frame_rate;           //video framerate numerator
    uint32             frame_rate_base;      //video framerate denominator
    uint32	           config_len;           //the video config length, not used in this version
    uint8	           *config_ptr;          //the video config buffer which is always used in mpeg4, not used in this version
    char	           *url_ptr;             //stream url
}STRM_VIDEO_PARAM_T;

typedef struct 
{
    uint8  octet_align;     //default 0
    uint8  crc;             //default 0
    uint8  robust_sorting;  //default 0
    uint32 interleaving;    //indicates the maximum number of the frame_blocks allowed in an interleaving group, 
                            //default 0 (which means no interleaving) 
}STRM_SDP_AMR_FMTP_PARAM_T;

//typedef struct 
//{
//    uint32  sprop_sets_len;
//    uint8   *sprop_sets_ptr;
//}STRM_SDP_H264_SPROP_SETS_T;
typedef struct 
{
    uint8                           profile_level_id[STRM_PRO_LEVEL_ID_LEN]; //3 bytes
    uint32                          sprop_sets_len; //sprop_parameter length
    uint8                           *sprop_sets_ptr; //sprop_parameters, parameters are separated by 0x00,0x00,0x00,0x01 as in ISO-IEC-14496-10
    //uint32  packetization_mode; //default 0 (which means single NAL mode), 1: none-interleaved, 2: interleaved
}STRM_SDP_H264_FMTP_PARAM_T;

typedef struct 
{
    STRM_SDP_ENC_E  enc_type;
    union
    {
        STRM_SDP_AMR_FMTP_PARAM_T   amr_param;
        STRM_SDP_H264_FMTP_PARAM_T  h264_param;
    }fmtp_param;
}STRM_SDP_FMTP_INFO_T;

typedef struct
{
    STRM_TIME_RANGE_T		    time_range;         //Ranges are half-open intervals, including the lower point, but
                                                    //excluding the upper point. In other words, a range of a-b starts
                                                    //exactly at time a, but stops just before b.
    STRM_AUDIO_PARAM_T 		    audio_param;        //stream audio parameter
    STRM_VIDEO_PARAM_T 		    video_param;        //stream video parameter
    STRM_SDP_FMTP_INFO_T        audio_fmtp_info;
    STRM_SDP_FMTP_INFO_T        video_fmtp_info;
    char                        *media_name_ptr;    //stream media name
    char                		*session_info_ptr;  //stream session information like author, copyright
}STRM_SDP_INFO_EXTERNAL_T;

typedef struct
{
    char				*host_ptr;      //proxy host address
    STRM_PORT_T	        port;           //proxy port
    char				*username_ptr;  //proxy user name
    char				*password_ptr;  //proxy password
}STRM_PROXY_INFO_T;

typedef struct
{
    char				*username_ptr;  //username
    char				*password_ptr;  //password
}STRM_AUTH_INFO_T;

typedef struct
{
    BLOCK_ID            app_thread_id;
    STRM_PROXY_INFO_T	proxy;      //proxy address
    STRM_AUTH_INFO_T	auth_info;  //authentication infomation
    char 				*url_ptr;   //destination URL
    uint8               *sdp_buf_ptr;//if using http to get the SDP
    uint32              sdp_len;
    char                *user_agent_ptr;
    char                *uaprof_ptr;
    uint32              net_id;
    uint16              rtp_lowest_port;
    uint16              rtp_highest_port;
}STRM_INIT_PARAM_T;

typedef struct
{
    uint32 start_time;  //the start time for resume from in ms, 0xffffffff is used for resume from the time when paused
}STRM_RESUME_PARAM_T;

typedef struct
{
    uint32 seek_to_time;    //the new start time for SEEK in ms
}STRM_SEEK_PARAM_T;

typedef struct
{
    _STRM_SIGNAL_VARS
    STRM_RESULT_E 		        result;     //init result
    STRM_SDP_INFO_EXTERNAL_T    sdp_info;   //the parsed SDP information got either by RTSP DESCRIBE or HTTP GET method
}STRM_INIT_CNF_T;

typedef struct
{
    _STRM_SIGNAL_VARS
    STRM_RESULT_E result;       //play result
}STRM_PLAY_CNF_T;

typedef struct
{
    _STRM_SIGNAL_VARS
    STRM_RESULT_E result;       //pause result
}STRM_PAUSE_CNF_T;

typedef struct
{
    _STRM_SIGNAL_VARS
    STRM_RESULT_E result;       //resume result
}STRM_RESUME_CNF_T;

typedef struct
{
    _STRM_SIGNAL_VARS
    STRM_RESULT_E result;       //seek result
}STRM_SEEK_CNF_T;

typedef struct
{
    _STRM_SIGNAL_VARS 
    STRM_RESULT_E result;       //stop result
}STRM_STOP_CNF_T;

typedef struct
{
    _STRM_SIGNAL_VARS 
    STRM_RESULT_E result;       //close result
}STRM_CLOSE_CNF_T;

typedef struct
{
    _STRM_SIGNAL_VARS 
}STRM_GOODBYE_IND_T;       //GOODBYE IND is send when receiving RTP session "GOODBYE", this indicate that the media data has been all send out

typedef struct
{
    _STRM_SIGNAL_VARS 
}STRM_SERVER_CLOSE_IND_T;       //This indication is received when RTSP server is disconnected

typedef struct
{
    _STRM_SIGNAL_VARS
}STRM_BUFFERING_IND_T;          //To indicate the APP that the streaming is start to enter "Buffering" state

typedef struct
{
    _STRM_SIGNAL_VARS
}STRM_BUFFER_END_IND_T;         //To indicate the APP that the streaming is exit the "Buffering" state, and will play the stream

typedef struct
{
    _STRM_SIGNAL_VARS 
}STRM_AUTH_IND_T;               //To indicate the APP that the server needs to authenticate the client

typedef struct
{
    _STRM_SIGNAL_VARS 
}STRM_DATA_TIMEOUT_IND_T;       //To indicate the APP that no sufficient data received while receiving

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
//Description : init the streaming controller
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
STRM_HANDLE STRM_Init (
						STRM_INIT_PARAM_T	*init_param_ptr //in: init parameter
							);

/****************************************************************************/
//Description : streaming PLAY request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Play (
						STRM_HANDLE     strm_handler,   //in: streaming handler
						DPLAYER_HANDLE	player_handler  //in: player handler
							);

/****************************************************************************/
//Description : streaming PAUSE request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Pause (
						STRM_HANDLE strm_handler    //in: streaming handler
							);

/****************************************************************************/
//Description : streaming SEEK request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Seek (
						STRM_HANDLE         strm_handler,       //in: streaming handler
						STRM_SEEK_PARAM_T  *seek_param_ptr      //in: seek parameters
							);

/****************************************************************************/
//Description : streaming RESUME request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Resume(
						STRM_HANDLE 	    strm_handler,    //in: streaming handler
						STRM_RESUME_PARAM_T	*param_ptr      //in: resume parameters
							);

/****************************************************************************/
//Description : streaming STOP request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Stop (
						STRM_HANDLE strm_handler    //in: streaming handler
							);

/****************************************************************************/
//Description : streaming CLOSE request
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
BOOLEAN STRM_Close (
						STRM_HANDLE strm_handler    //in: streaming handler
							);
/****************************************************************************/
//Description : streaming describe auth rsp
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN STRM_DescribeAuthRsp(
						STRM_HANDLE 			strm_handler,
						const STRM_AUTH_INFO_T	*auth_info_ptr
							);
/****************************************************************************/
//Description : streaming setup auth rsp
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN STRM_SetupAuthRsp(
						STRM_HANDLE 			strm_handler,
						const STRM_AUTH_INFO_T	*auth_info_ptr
							);

/****************************************************************************/
//Description : streaming play auth rsp
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN STRM_PlayAuthRsp(
						STRM_HANDLE 			strm_handle,
						const STRM_AUTH_INFO_T	*auth_info_ptr
							);

    
    
#ifdef   __cplusplus
    }
#endif
    
    
    
#endif 
    
