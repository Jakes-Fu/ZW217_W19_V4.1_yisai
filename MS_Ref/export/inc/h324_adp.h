/**************************************************************************
 **	FileName:		h324_adp.h       								         *
 **	Author:			         											  *
 **	Date:			        											  *
 **  Copyright:		                                                	                                     *
 **  Description:	       one adaptor between ATC and H324					  *
 **************************************************************************

 **************************************************************************
 **							Edit	History								  *
 **------------------------------------------------------------------------
 **	DATE			NAME			DESCRPTION							  *
 ** 2011/08/25           Chaoyi.hu                  Created			
 **************************************************************************/

#ifndef H324_APD_H
#define  H324_APD_H

#ifdef __cplusplus
    extern "C"   {
#endif

/**************************************************************************/
/*  fuction H324_ADP_VideoCodecCtrl()   param */
typedef enum
{
    H324_ADP_VIDEO_CODEC_CTRL_OPEN = 1,
    H324_ADP_VIDEO_CODEC_CTRL_CLOSE,
    H324_ADP_VIDEO_CODEC_CTRL_CONFIG,
    H324_ADP_VIDEO_CODEC_CTRL_UNDEFINE
}H324_ADP_VIDEO_CODEC_CTRL_TYPE_E;


/*Video encode buffer state indication define*/
typedef enum
{
    H324_ADP_VIDEO_CODEC_FORMAT_ACCORD_NO = 0,
    H324_ADP_VIDEO_CODEC_FORMAT_ACCORD_YES,
    H324_ADP_VIDEO_CODEC_FORMAT_ACCORD_UNDEFINE
}H324_ADP_VIDEO_CODEC_FORMAT_ACCORD_E;


/**************************************************************************/
/*  msg H324_ADP_MSG_VID_REMOTE_AV_SEND_IND  param */
typedef struct
{
    BOOLEAN data_type;   // 0: audio, 1: video
    BOOLEAN switch_ind;         // 0: stop,  1: send
    BOOLEAN  indication;   // 0: dont send replace pic;  1: need send replace pic
}H324_ADP_AV_SEND_OR_STOP_IND_T; 


/**************************************************************************/
/* msg  H324_ADP_MSG_VID_REMOTE_RECORD_IND   param */
typedef enum
{
    H324_ADP_VIDEO_RECORD_START  = 0,
    H324_ADP_VIDEO_RECORD_STOP,
    H324_ADP_VIDEO_RECORD_UNDEFINE
}H324_ADP_VIDEO_RECORD_E;


/*Media Video format select*/
typedef enum
{
    H324_ADP_VIDEO_CODEC_H263 = 1,
    H324_ADP_VIDEO_CODEC_MPEG4,
    H324_ADP_VIDEO_CODEC_FORMAT_UNDEFINE
}H324_ADP_VIDEO_CODEC_FORMAT_E;


/**************************************************************************/
/*  callback function  H324_ADP_MODEM_CALLBACK_T()   param */
typedef enum
{
    H324_ADP_MSG_VID_NULL = 0,
    H324_ADP_MSG_VID_ENC_DATA_REQ,   
    H324_ADP_MSG_VID_CODEC_CTRL,    
    H324_ADP_MSG_VID_REMOTE_AV_SEND_IND,     
    H324_ADP_MSG_VID_REMOTE_RECORD_IND, 
    H324_ADP_MSG_VID_REMOTE_MP4_DCI_INFO,    
    H324_ADP_MSG_VID_END
}H324_ADP_MSG_VID_TYPE_E;


/**************************************************************************/
/* msg  H324_ADP_MSG_VID_ENC_DATA_REQ   param */
typedef enum
{
    H324_ADP_VIDEO_ENC_BUF_EMPTY = 0,
    H324_ADP_VIDEO_ENC_BUF_FULL,
    H324_ADP_VIDEO_ENC_BUF_UNDEFINE
}H324_ADP_VIDEO_ENC_DATA_REQ_E;


/**************************************************************************/
/*  msg H324_ADP_MSG_VID_REMOTE_MP4_DCI_INFO  param */
/*  fuction H324_ADP_LocalMp4DCI()   param */
typedef struct
{
    uint32 dci_length;
    uint8 *dci_ptr;
}H324_ADP_MP4_DCI_INFO_T;

/* msg  H324_ADP_MSG_VID_CODEC_CTRL param */
typedef struct
{
    H324_ADP_VIDEO_CODEC_CTRL_TYPE_E   ctrl_type;
    uint32  config_length;
    uint8   *config_ptr;
}H324_ADP_VIDEO_CTRL_T;

/* msg  H324_ADP_MSG_VID_CODEC_CTRL, type = config, config param define          *
  * video codec type.                                                                                                */
typedef enum
{
    H324_ADP_EXT_CODEC_V_H263    = 1, 
    H324_ADP_EXT_CODEC_V_MPEG4   = 2, 
    H324_ADP_EXT_CODEC_V_TYPE_MAX
}H324_ADP_EXT_CODEC_V_TYPE_E;

// video codec type.
typedef enum
{
    H324_ADP_EXT_CODEC_V_ENCODE   = 0, 
    H324_ADP_EXT_CODEC_V_DECODE    = 1, 
    H324_ADP_EXT_CODEC_V_OPER_MAX
}H324_ADP_EXT_CODEC_V_OPER_E;

typedef enum
{
    H324_ADP_EXT_CODEC_V_PROFILE_LEVEL_NONE  =0,
    // for mepg4.
    H324_ADP_EXT_CODEC_MPEG4_SIMPLE_LEVEL1   = 1,    // QCIF.64kbps.15fps.
    H324_ADP_EXT_CODEC_MPEG4_SIMPLE_LEVEL2   = 2,    // CIF.128kbps.15fps.
    H324_ADP_EXT_CODEC_MPEG4_SIMPLE_LEVEL3   = 3,    // CIF.384kbps.30fps.
    //
    H324_ADP_EXT_CODEC_MPEG4_MAIN_LEVEL2     = 10,    // CIF.2Mbps.60fps.
    H324_ADP_EXT_CODEC_MPEG4_MAIN_LEVEL3     = 11,    // 720x576.15Mbps.30fps.

    // for h.263.
    H324_ADP_EXT_CODEC_H263_BASELINE_LEVEL30 = 20   // CIF.384kbps.30fps.
}H324_ADP_EXT_CODEC_V_PROFILE_LEVEL_E;


typedef enum
{
    H324_ADP_EXT_CODEC_V_PP_NONE,
    H324_ADP_EXT_CODEC_V_PP_LEVEL1,
    H324_ADP_EXT_CODEC_V_PP_LEVEL2,
    H324_ADP_EXT_CODEC_V_PP_LEVEL3,
    H324_ADP_EXT_CODEC_V_PP_LEVEL4,
    H324_ADP_EXT_CODEC_V_PP_MAX
}H324_ADP_EXT_CODEC_V_POST_PROCESS_E;


typedef struct
{
    int32 width;
    int32 height;
}H324_ADP_EXT_CODEC_V_SIZE_T;


typedef struct
{
    int32 rate_control_en;
    int32 target_bitrate;
    int32 default_quant;    // [0,31]
}H324_ADP_EXT_CODEC_V_RATECONTROL_T;


typedef struct{
    int32 video_packet_en;
    int32 data_patrion_en;
    int32 rvlc_en;
}H324_ADP_EXT_CODEC_V_ENC_ERR_TOOL_T;

typedef struct
{
    int32 ac_prediction_en;
    int32 rounding_ctrl_en;
}H324_ADP_EXT_CODEC_V_ENC_COMPRESS_TOOL_T;

typedef struct {
    int32 intra_cycle_en;
    int32 intra_cycle;
}H324_ADP_EXT_CODEC_V_ENC_INTRA_CYCLE_T;

typedef struct
{
    H324_ADP_EXT_CODEC_V_OPER_E                codec_oper;
    H324_ADP_EXT_CODEC_V_TYPE_E                 codec_type;
    H324_ADP_EXT_CODEC_V_PROFILE_LEVEL_E         porfile_level;

    H324_ADP_EXT_CODEC_V_SIZE_T                  size;
    H324_ADP_EXT_CODEC_V_RATECONTROL_T           rate_ctrl;
    H324_ADP_EXT_CODEC_V_ENC_ERR_TOOL_T          error_tool;
    H324_ADP_EXT_CODEC_V_ENC_COMPRESS_TOOL_T     compress_tool;
    H324_ADP_EXT_CODEC_V_ENC_INTRA_CYCLE_T       intra_cycle;
}H324_ADP_EXT_CODEC_V_ENC_OPTION_T;


typedef struct
{
    H324_ADP_EXT_CODEC_V_OPER_E                codec_oper;
    H324_ADP_EXT_CODEC_V_TYPE_E         codec_type;
    H324_ADP_EXT_CODEC_V_SIZE_T          raw_size;
    H324_ADP_EXT_CODEC_V_POST_PROCESS_E  pp_level;
}H324_ADP_EXT_CODEC_V_DEC_OPTION_T;

#if 0
/* Sensor Selection */
typedef enum
{
    SENSOR_TYPE_FRONT = 1,
    SENSOR_TYPE_BACK

 }H324_ADP_SENSOR_TYPE_E;
#endif

/*H324M ---> APP MSG*/
typedef enum
{
    H324_ADP_MSG_NULL = 0,
    H324_ADP_MSG_IND_USER_INPUT_ALPHANUMERIC = 1,
    H324_ADP_MSG_STARTMEDIATX,
    H324_ADP_MSG_STARTMEDIARX,
    H324_ADP_MSG_STARTMEDIATXRX,
    H324_ADP_MSG_CLOSE_IND,
    H324_ADP_MSG_CLOSE_CNF,
    H324_ADP_MSG_MEDIA_INACTIVE_IND,
    H324_ADP_MSG_MEDIA_ACTIVE_IND,
    H324_ADP_MSG_ROUNDTRIP_CNF,
    H324_ADP_MSG_ABORTMEDIA,
    H324_ADP_MSG_VIDEO_RX_NO_SIGNAL_START,
    H324_ADP_MSG_VIDEO_RX_NO_SIGNAL_STOP,
    H324_ADP_MSG_END
}H324_ADP_MSG_TYPE_E;

/*CLOSE causes */
typedef enum
{
    H324_ADP_CLOSE_CAUSE_BYUSER = 1,
    H324_ADP_CLOSE_CAUSE_BYREMOTE,
    H324_ADP_CLOSE_CAUSE_BYPROTOCAL,
    H324_ADP_CLOSE_CAUSE_BYSW
 }H324_ADP_MSG_CLOSE_CAUSE_E;

/*Media Type */
typedef enum
{
    H324_ADP_MEDIA_AUDIO = 1,
    H324_ADP_MEDIA_VIDEO,
    H324_ADP_MEDIA_UNDEFINE
}H324_ADP_MEDIA_TYPE_E;

/*Media Direction*/
typedef enum
{
    H324_ADP_MEDIA_DIRECTION_TX =1,
    H324_ADP_MEDIA_DIRECTION_RX,  
    H324_ADP_MEDIA_DIRECTION_TXRX,
    H324_ADP_MEDIA_DIRECTION_UNDEFINE    
}H324_ADP_MEDIA_DIRECTION_E;

/*Media Video Frame Rate Type */
typedef enum
{
    H324_ADP_VIDEO_FRATE_LOWEST = 1,
    H324_ADP_VIDEO_FRATE_LOWER,
    H324_ADP_VIDEO_FRATE_LOW,
    H324_ADP_VIDEO_FRATE_MIDDLE,
    H324_ADP_VIDEO_FRATE_HIGH,
    H324_ADP_VIDEO_FRATE_HIGHER,
    H324_ADP_VIDEO_FRATE_HIGHEST    
}H324_ADP_VIDEO_FRATE_TYPE_E;

/*Media Video format select*/
typedef enum
{
    H324_ADP_VIDEO_FORMAT_ONLY_H263 = 1,
    H324_ADP_VIDEO_FORMAT_ONLY_MPEG4,
    H324_ADP_VIDEO_FORMAT_PREFER_H263,
    H324_ADP_VIDEO_FORMAT_PREFER_MPEG4,
    H324_ADP_VIDEO_FORMAT_UNDEFINE
}H324_ADP_VIDEO_FORMAT_E;

/*Media Video H263 size define*/
typedef enum
{
    H324_ADP_VIDEO_H263_QCIF = 1,
    H324_ADP_VIDEO_H263_SQCIF,
    H324_ADP_VIDEO_H263_SIZE_UNDEFINE
}H324_ADP_VIDEO_H263_SIZE_E;

/*Media Video Adapter Layer define*/
typedef struct
{
    BOOLEAN video_al_1;
    BOOLEAN video_al_2;
    BOOLEAN video_al_3;
}H324_ADP_VIDEO_ADAPTER_LAYER_T;


typedef struct
{
   uint16   uplink_time;
   uint16   downlink_time;
}H324_ADP_VIDEO_DELAY_TIME_T;

typedef void (*H324_ADP_MMI_CALLBACK_T)(H324_ADP_MSG_TYPE_E i_msg, int argc, void *argv[]);


/**************************************************************************/
/*  fuction H324_ADP_Modem_Open()   param */
typedef void (*H324_ADP_MODEM_CALLBACK_T)(H324_ADP_MSG_VID_TYPE_E i_msg, int argc, void *argv[]);

typedef void (*H324_ADP_MMI_CALLBACK_GET_REPLACE_DATA_T)(unsigned char **pSource, int *size);

/**************************************************************************/
// Description:
//     This function is used to create and initialize a Dilithium H324M app.
//
// Param
//    pf_callback    callback function, through it APP can catch the 
//                    interesting MSG.
//    b_mo  
//        0     mt, MPLH324M_ListenForCall should be following called.
//        1     mo, MPLH324M_MakeCall should be following called.
//
// Return
//     0       successful
//     others  failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_Open(int b_mo, H324_ADP_MMI_CALLBACK_T pf_callback);


/**************************************************************************/
// Description:
//     Close a Dilithium H324M App, realease all resource of this connection.
// Return
//     None
//
// Author:
//
// Note:
//
/**************************************************************************/
void H324_ADP_Close(void);


/**************************************************************************/
// Description:
//     This function is used to create and initialize a Dilithium H324M app.
//
// Param
//    pf_callback    callback function, through it APP can catch the 
//                    interesting MSG.
//    b_mo  
//        0     mt, MPLH324M_ListenForCall should be following called.
//        1     mo, MPLH324M_MakeCall should be following called.
//
// Return
//     0       successful
//     others  failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_Modem_Open( H324_ADP_MODEM_CALLBACK_T pf_callback);


/**************************************************************************/
// Description:
//     Close a Dilithium H324M App, realease all resource of this connection.
// Return
//     None
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_Modem_Close(void);



/**************************************************************************/
// Description:
//     Start to make a call. (MO case)
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//     before calling this function, be sure the circuit switch data link 
//     has been setup successfully 
//
/**************************************************************************/
int  H324_ADP_MakeCall(void);


/**************************************************************************/
// Description:
//     Listen for call. (MT case)
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//     before calling this function, be sure the circuit switch data link 
//     has been setup successfully 
//
/**************************************************************************/
int  H324_ADP_ListenForCall(void);


/**************************************************************************/
// Description:
//     Hang up 
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_HangUp(void);


/**************************************************************************/
// Description:
//     send the user input to peer by H245
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_SendUsrInput(char c_digit);


/**************************************************************************/
// Description:
//     Set video suspend
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_SetVideoSuspend( int b_suspend );



/**************************************************************************/
// Description:
//    operation type:  1: codec opened  successfully; 2: closed; 3: parameter configed
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_VideoCodecCtrl( H324_ADP_VIDEO_CODEC_CTRL_TYPE_E  type );



/**************************************************************************/
// Description:
//    operation type:  1: codec opened  successfully; 2: closed; 3: parameter configed
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_VideoCodecAbility( H324_ADP_VIDEO_CODEC_FORMAT_E decode, 
                                 H324_ADP_VIDEO_CODEC_FORMAT_E encode, 
                                 H324_ADP_VIDEO_CODEC_FORMAT_ACCORD_E  conformance);



/**************************************************************************/
// Description:
//     send the user input to peer by H245
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_SendUsrInputStr(char *digit_ptr);


/**************************************************************************/
// Description:
//     send the user input to peer by H245
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_SendOrStopAVData( H324_ADP_AV_SEND_OR_STOP_IND_T   oper );


/**************************************************************************/
// Description:
//     send the user input to peer by H245
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
/* 0: stop record; 1: start record*/
int  H324_ADP_VideoRecordInd( H324_ADP_VIDEO_RECORD_E ind  );   




/**************************************************************************/
// Description:
//     send the user input to peer by H245
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_LocalMp4DCI( H324_ADP_MP4_DCI_INFO_T header ); 


/**************************************************************************/
// Description:
//     Get the Time Delay by send out a round trip delay request message to remote.
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_RequestRoundTripDelay(void);


/**************************************************************************/
// Description:
//     Suspend/Resume the video output, from Local 2 Remote.
//     And, if suspend, also set the callback function used to get image data used to replace Local video data(Will be sent to Remote).
//
// Param
//    b_suspend  : 
//        0     Resume, Resume the output of video data from camera. 
//        1     Suspend, Suspend the output of video data from camera. Replaced data get from (func_get_data_L2R) function will be used.
//
//    func_get_data_L2R     
//        callback function, will be called by 324M, used to get video data that will be sent from Local 2 remote, replace data from Camera.
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_ConfigVideoL2R(
                              int b_suspend, 
   								            H324_ADP_MMI_CALLBACK_GET_REPLACE_DATA_T func_get_data_L2R
    								        );


/**************************************************************************/
// Description:
//     Suspend/Resume the audio output, from Local 2 Remote.
//     And, if suspend, also set the callback function used to get audio data used to replace Local audio data(Will be sent to Remote).
//
// Param
//    b_suspend  : 
//        0     Resume, Resume the output of audio data from Mic. 
//        1     Suspend, Suspend the output of audio data from Mic. Replaced data get from (func_get_data_L2R) function will be used.
//
//    func_get_data_L2R     
//        callback function, will be called by 324M, used to get audio data that will be sent from Local 2 remote, replace data from Mic.
//        If this param is set to NULL, no data will be output.
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//    1: Currently, func_get_data_L2R param do not support(Should be set to NULL).
//
/**************************************************************************/
int H324_ADP_ConfigAudioL2R(
    									        int b_suspend, 
    								        	H324_ADP_MMI_CALLBACK_GET_REPLACE_DATA_T func_get_data_L2R
    								       );


/**************************************************************************/
// Description:
//     Adjust source video frame rate.
//
// Param
//    fRate     FrameRate class that is demanded
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_VideoAdjust(H324_ADP_VIDEO_FRATE_TYPE_E fRate);


/**************************************************************************/
// Description:
//     Sends a command to the remote requesting adjustment of the incoming video quality as soon as possible.
//
// Param
//    fRate     FrameRate class that is demanded
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_VideoAdjustRx(H324_ADP_VIDEO_FRATE_TYPE_E fRate);

/**************************************************************************/
// Description:
//     Get the Time Delay by send out a round trip delay request message to remote.
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_RequestRoundTripDelay(void);


/**************************************************************************/
// Description:
//     Suspend/Resume the video output, from Local 2 Remote.
//     And, if suspend, also set the callback function used to get image data used to replace Local video data(Will be sent to Remote).
//
// Param
//    b_suspend  : 
//        0     Resume, Resume the output of video data from camera. 
//        1     Suspend, Suspend the output of video data from camera. Replaced data get from (func_get_data_L2R) function will be used.
//
//    func_get_data_L2R     
//        callback function, will be called by 324M, used to get video data that will be sent from Local 2 remote, replace data from Camera.
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_ConfigVideoL2R(
    									       int b_suspend, 
    									       H324_ADP_MMI_CALLBACK_GET_REPLACE_DATA_T func_get_data_L2R
  								         );


/**************************************************************************/
// Description:
//     Suspend/Resume the audio output, from Local 2 Remote.
//     And, if suspend, also set the callback function used to get audio data used to replace Local audio data(Will be sent to Remote).
//
// Param
//    b_suspend  : 
//        0     Resume, Resume the output of audio data from Mic. 
//        1     Suspend, Suspend the output of audio data from Mic. Replaced data get from (func_get_data_L2R) function will be used.
//
//    func_get_data_L2R     
//        callback function, will be called by 324M, used to get audio data that will be sent from Local 2 remote, replace data from Mic.
//        If this param is set to NULL, no data will be output.
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//    1: Currently, func_get_data_L2R param do not support(Should be set to NULL).
//
/**************************************************************************/
int H324_ADP_ConfigAudioL2R(
    									       int b_suspend, 
    									       H324_ADP_MMI_CALLBACK_GET_REPLACE_DATA_T func_get_data_L2R
   								         );


/**************************************************************************/
// Description:
//     Adjust source video frame rate.
//
// Param
//    fRate     FrameRate class that is demanded
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_VideoAdjust(H324_ADP_VIDEO_FRATE_TYPE_E fRate);


/**************************************************************************/
// Description:
//     Sends a command to the remote requesting adjustment of the incoming video quality as soon as possible.
//
// Param
//    fRate     FrameRate class that is demanded
//
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_VideoAdjustRx(H324_ADP_VIDEO_FRATE_TYPE_E fRate);



/**************************************************************************/
// Description:
//     turn on/off the recording switch of Input media stream.
//     b_onoff  if true, do recording; else not recording
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_RecordInMedia(int b_onoff);


/**************************************************************************/
// Description:
//     turn on/off the recording switch of output media stream
//     b_onoff  if true, do recording; else not recording
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_RecordOutMedia(int b_onoff);


/**************************************************************************/
// Description:
//     change the skin mode
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_SetSkinMode(int mode);


/**************************************************************************/
// Description:
//     Get the the prefered video format.
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
H324_ADP_VIDEO_FORMAT_E H324_ADP_GetVideoFormat(void);


/**************************************************************************/
// Description:
//     Set the the prefered video format.
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
void H324_ADP_SetVideoFormat(H324_ADP_VIDEO_FORMAT_E format);


/**************************************************************************/
// Description:
//     Get the the prefered size of video H263.
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
H324_ADP_VIDEO_H263_SIZE_E H324_ADP_GetVideoH263Size(void);


/**************************************************************************/
// Description:
//     Set the the prefered size of video H263.
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
void H324_ADP_SetVideoH263Size(H324_ADP_VIDEO_H263_SIZE_E h263_size);


/**************************************************************************/
// Description:
//     Enable or Disable the VT config file. When enable, the VT setting will be initialized with
//     the setting in config file.
//
// Param
//    b_onoff  
//        0     Disable
//        1     Enable
// Return
//
// Author:
//
// Note:
//     For the config file ,the default setting is disable.
//
/**************************************************************************/
void H324_ADP_EnableCfgFile(int b_onoff);


/**************************************************************************/
// Description:
//     Get video codec state, enable or disable.
//
// Param
//
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
BOOLEAN H324_ADP_isVideoEnable(void);



/**************************************************************************/
// Description:
//     Enable video codec before starting the videophone.
//
// Param
//    b_onoff  
//        0     turn off
//        1     turn on
//
// Return
//
// Author:
//
// Note:
//    If this param is not set, it's default value is 1, means turn on.
//
/**************************************************************************/
void H324_ADP_EnableVideo(int b_onoff);



/**************************************************************************/
// Description:
//     Get audio codec state, enable or disable.
//
// Param
//
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
BOOLEAN H324_ADP_isAudioEnable(void);



/**************************************************************************/
// Description:
//     Enable audio codec before starting the videophone.
//
// Param
//    b_onoff  
//        0     turn off
//        1     turn on
//
// Return
//
// Author:
//
// Note:
//    If this param is not set, it's default value is 1, means turn on.
//
/**************************************************************************/
void H324_ADP_EnableAudio(int b_onoff);



/**************************************************************************/
// Description:
//     Get Seq Number check Feature state, enable or disable.
//
// Param
//
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
BOOLEAN H324_ADP_isSeqCheckEnable(void);



/**************************************************************************/
// Description:
//     Enable Seq Number check Feature before starting the videophone.
//
// Param
//    b_onoff  
//        0     turn off
//        1     turn on
//
// Return
//
// Author:
//
// Note:
//    If this param is not set, it's default value is 1, means turn on.
//
/**************************************************************************/
void H324_ADP_EnableSeqCheck(int b_onoff);



/**************************************************************************/
// Description:
//     Get Mona Feature state, enable or disable.
//
// Param
//
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
BOOLEAN H324_ADP_isMonaEnable(void);



/**************************************************************************/
// Description:
//     Enable Mona Feature before starting the videophone.
//
// Param
//    b_onoff  
//        0     turn off
//        1     turn on
//
// Return
//
// Author:
//
// Note:
//    If this param is not set, it's default value is 1, means turn on.
//
/**************************************************************************/
void H324_ADP_EnableMona(int b_onoff);



/**************************************************************************/
// Description:
//     Get WNSRP Feature state, enable or disable.
//
// Param
//
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
BOOLEAN H324_ADP_isWNSRPEnable(void);



/**************************************************************************/
// Description:
//     Enable WNSRP Feature before starting the videophone.
//
// Param
//    b_onoff  
//        0     turn off
//        1     turn on
//
// Return
//
// Author:
//
// Note:
//    If this param is not set, it's default value is 1, means turn on.
//
/**************************************************************************/
void H324_ADP_EnableWNSRP(int b_onoff);



/**************************************************************************/
// Description:
//     Enable RequestMode Feature before starting the videophone.
//
// Param
//    b_onoff  
//        0     turn off
//        1     turn on
//
// Return
//
// Author:
//
// Note:
//    If this param is not set, it's default value is 1, means turn on.
//
/**************************************************************************/
void H324_ADP_EnableRequestMode(int b_onoff);



/**************************************************************************/
// Description:
//     Get RequestMode Feature state, enable or disable.
//
// Param
//
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
BOOLEAN H324_ADP_isRequestModeEnable(void);



/**************************************************************************/
// Description:
//     Set the the prefered video adapter layer.
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_SetVideoAdapterLayer(H324_ADP_VIDEO_ADAPTER_LAYER_T  *video_al_ptr);



/**************************************************************************/
// Description:
//     Get the the prefered video adapter layer.
// Return
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_GetVideoAdapterLayer(H324_ADP_VIDEO_ADAPTER_LAYER_T *video_al_ptr);



/**************************************************************************/
// Description:
//    set Audio delay time
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int H324_ADP_SetAudioDelayTime(BOOLEAN up_or_down, H324_ADP_VIDEO_DELAY_TIME_T delaytime );




/**************************************************************************/
// Description:
//    get Audio delay time
// Return
//     0        successful
//     others   failed
//
// Author:
//
// Note:
//
/**************************************************************************/
int  H324_ADP_GetAudioDelayTime(H324_ADP_VIDEO_DELAY_TIME_T *time_ptr);



#if 0
#ifdef VT_DUAL_SENSOR_SWITCH 
/**************************************************************************/
// Description:
//     switch sensor type when VT 
//
// Param
//    s_type  
//        1     SENSOR_TYPE_FRONT
//        2     SENSOR_TYPE_BACK
//
// Return
//
// Author:
//
// Note:
//    If this param is not set, it's default value is 1, means front sensor.
//
/**************************************************************************/
int H324_ADP_SensorSwitch(H324_ADP_SENSOR_TYPE_E s_type);
#endif
#endif

#ifdef  __cplusplus
    }
#endif


#endif
