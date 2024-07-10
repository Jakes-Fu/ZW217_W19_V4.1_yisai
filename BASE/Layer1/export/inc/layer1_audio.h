/******************************************************************************
 ** File Name:      layer1_audio.h                                            *
 ** Author:         Richard Yang                                              *
 ** DATE:           17/01/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 17/01/2002     Richard.Yang     Create.                                   *
 ** 29/04/2003     Xueliang.Wang    Modify.                                   *
 ******************************************************************************/

#ifndef _LAYER1_AUDIO_H
#define _LAYER1_AUDIO_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_audio.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define DSP_VOL_SPEAKER_MAX     9

// @Xiangxin cr6574 03-12-25
#define MAX_SIDE_TONE_GAIN		4

#define MAX_AMR_FRAME_SIZE     32
/**---------------------------------------------------------------------------*
 **                         Macro definition                                  *
 **---------------------------------------------------------------------------*/
 typedef void (*MAG_CALLBACK)();
 
//jingyin call back function for cr122236 
typedef void (*LAYER1_AUDIO_SILENCEPROCESS_PFUNC)(void);
//added by tom for cr118941
typedef void (*LAYER1_AUDIO_CALLBACK)(uint16 param0,uint16 param1, uint16 param2,uint16 param3);
//format of record file

#define NUM_OF_EQ_TYPES 4 //cr27477

#define NUM_OF_SPEAKER_MODES 3 //cr27477  refers to handset, handfree, earphone


#define LAYER1_AUDIO_CMD 0x35

typedef enum
{
    AUDIO_SUB_CMD_MP3_INIT = 0,
	AUDIO_SUB_CMD_MP3_DECODE,
	AUDIO_SUB_CMD_MP3_BRAKE,
#ifndef GSM_CUSTOMER_AFP_SUPPORT	
    AUDIO_SUB_CMD_AAC_INIT,
	AUDIO_SUB_CMD_AAC_DECODE,
	AUDIO_SUB_CMD_AAC_BRAKE,
#else
    AUDIO_SUB_CMD_TONE_INIT,
	AUDIO_SUB_CMD_TONE_DECODE,
	AUDIO_SUB_CMD_TONE_BRAKE,
#endif	
    AUDIO_SUB_CMD_AMR_NB_INIT,
	AUDIO_SUB_CMD_AMR_NB_DECODE,
	AUDIO_SUB_CMD_AMR_NB_BRAKE,
	AUDIO_SUB_CMD_END
}Audio_Sub_Cmd_E;

typedef struct {
    uint16 Command_Queue_ID;
    uint16 parameter0;
    uint16 parameter1;
    uint16 parameter2;
    uint16 parameter3;
}Audio_CmdQ_T;
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//interface for ref
typedef void (*GET_DATA_CALLBACK)(  
  BOOLEAN  report_type,
  uint32   data_len  
  );
  
//mp3 param
typedef struct
{	
	uint32 phase;       //synth paras
	uint32 ns;
	uint32 channels;
	uint32 mute;	
	
	int32* sbSamplePtr;//transfer data to DSP
	uint32 sbSampleLen;
	 
}L1_A2DP_SYNTH_PARAM_T;

typedef struct 
{
	L1_A2DP_SYNTH_PARAM_T synthParam;
	
	int16* pcmLeftPtr; //store pcm data from DSP 
	int16* pcmRightPtr;

}L1_MP3_SYNTH_PARAS_T;

//sbc param
typedef struct
{	
	 uint8  channel_mode;
	 uint8  alloc_method;                      
	 uint16 sample_freq;
	 uint8  nrof_blocks;
	 uint8  nrof_subbands;  
	 uint8  bitpool;
	 uint16 framesize;
	 
}L1_A2DP_SBC_PARAM_T;	


typedef struct
{
	
	L1_A2DP_SBC_PARAM_T  sbcConfig;
	 
	BOOLEAN is_need_config;
	uint8*  sbcPtr;     //store sbc data from DSP
	 
}L1_MP3_SBC_PARAS_T;

#ifdef NXP_LVVE_SUPPORT
typedef	void (*RECORD_CALLBACK2) (uint16 *);
#endif
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to initialize global and static
//					varibles in Layer1_audio.c file.
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC void LAYER1_AudioInit(void);

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to play special tone.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
#if defined(MS_OAPI_SUPPORT) && !defined(CHIP_VER_6610)
PUBLIC uint32 LAYER1_PlaySpecialTone(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    AUD_DTMF_TONE_ID_E   tone_id,            // Special tone enumeration value.
    BOOLEAN     		 b_uplink            // If uplink this tone.(SCI_TRUE: Uplink)
    );

#else
PUBLIC uint32 LAYER1_PlaySpecialTone(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    AUD_DTMF_TONE_ID_E   tone_id,            // Special tone enumeration value.
    BOOLEAN     		 b_uplink,            // If uplink this tone.(SCI_TRUE: Uplink)
    uint16                     tone_volume
    );
#endif

PUBLIC uint32 LAYER1_PlaySpecialTone_Ex(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    AUD_DTMF_TONE_ID_E   tone_id,            // Special tone enumeration value.
    BOOLEAN     		 b_uplink,            // If uplink this tone.(SCI_TRUE: Uplink)
    uint16                     tone_volume
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to play a tone, which is combined by freq1, freq2 and freq3.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
#if defined(MS_OAPI_SUPPORT) && !defined(CHIP_VER_6610)
PUBLIC uint32 LAYER1_PlayGenericTone(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    uint16  freq1,  // Freq to be played.
    uint16  freq2,  // Freq to be played.
    uint16  freq3   // Freq to be played.
    );
#else
PUBLIC uint32 LAYER1_PlayGenericTone(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    uint16  freq1,  // Freq to be played.
    uint16  freq2,  // Freq to be played.
    uint16  freq3,   // Freq to be played.
    uint16 tone_volume
    );
#endif

PUBLIC uint32 LAYER1_PlayGenericTone_Ex(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    uint16  freq1,  // Freq to be played.
    uint16  freq2,  // Freq to be played.
    uint16  freq3,   // Freq to be played.
    uint16 tone_volume
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to stop playing tone.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_StopTone(  // If succeed, return SCI_SUCCESS,
                                // else return SCI_ERROR
    void
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to set volume(include voice and tone).
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetVolume( // If succeed, return SCI_SUCCESS,
                                // else return SCI_ERROR
    uint16  speaker_vol // Volume value of speaker(0 - DSP_VOL_SPEAKER_MAX)
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to switch mute to ON or OFF.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_MuteSwitch(    // If succeed, return SCI_SUCCESS,
                                    // else return SCI_ERROR
    BOOLEAN b_mute,                 // SCI_TRUE: Mute ON
    uint16  mute_type               // add for bluetooth mode
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to open sound device.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetCodecEnable(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN  is_enable         // SCI_TRUE: Open,   SCI_FALSE: Close
    );

/*****************************************************************************/
//  Description:    before set audio enable , ref need to inform dsp .
//  Author:         refer to chipdrv and ref()
//	Note:           
/*****************************************************************************/    
PUBLIC uint32 LAYER1_PreSetAudioEnable(   // If succeed, return SCI_SUCCESS,
               BOOLEAN  is_enable      // else return SCI_ERROR       
               );
    
    
/*****************************************************************************/
//  Description:    This function is used to send a CMD to DSP
//  Author:         Y.J.KE
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetAudioEnable(   // If succeed, return SCI_SUCCESS,
               BOOLEAN  is_enable      // else return SCI_ERROR       
               );
    
/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to switch sound device.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SwitchSoundDevice( // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN  b_aux_mic,     // SCI_TRUE: use Aux mic,       SCI_FALSE: use mic
    BOOLEAN  b_aux_speaker,  // SCI_TRUE: use Aux speaker,   SCI_FALSE: use speaker
    AUDIO_DEVICE_MODE_TYPE_E   mode
    );

/*****************************************************************************/
//  Description:    This function switchs layer1 mode to test mode.
//  Author:         fancier.fan
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SwitchTestMode(
	BOOLEAN is_test_mode
	);

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to set audio engineering paramters.
//  Author:         Jenny.xu
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetAudioEngineeringParameter( // If succeed, return SCI_SUCCESS,
                                                   // else return SCI_ERROR
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,     
    AUDIO_TEST_DATA_T   param_data				 
    );

#if 0
#ifndef NXP_LVVE_SUPPORT
/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to set voice loop back paramters.
//  Author:         Chirs.jin
//	Note:           
/*****************************************************************************/    
PUBLIC uint32 LAYER1_EnableVoiceLoopback(
    BOOLEAN is_enable,             //0: disable  1:enable
    uint16  delay_time             // millisecones
    );
#else
/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to set voice loop back paramters.
//  Author:         Chirs.jin
//	Note:           
/*****************************************************************************/    
PUBLIC uint32 LAYER1_EnableVoiceLoopback(
    BOOLEAN is_enable,             //0: disable  1:enable
    uint16  usLoopbackType,      // 
    uint16  usVoiceFormat,
    uint16  usReserved            // millisecones
    );
#endif
#endif
/*****************************************************************************/
//  Description:    This function start/stop echo test
//  Author:         Chris.jin
//	Note:           
/*****************************************************************************/
    
PUBLIC uint32 LAYER1_EnableEchoTest (
    	BOOLEAN is_echo_test_enable,
    	uint16	freq,
    	AUD_ECHOCALLBACK_PFUNC echo_callback
	);
/*****************************************************************************/
//  Description:    This function start/stop echo test
//  Author:         Chris.jin
//	Note:           
/*****************************************************************************/
	
PUBLIC uint32 LAYER1_DownlinkMuteEnable (
    	BOOLEAN mute_enable,
    	uint16  mute_type
 );

/*****************************************************************************/
//  Description:    This function response echo test result
//  Author:         chris.jin
//	Note:           
/*****************************************************************************/
	
PUBLIC void LAYER1_SendEchoResult(
    uint32  echo_result
	);

/**********************************************************************************/
//  Description:    This function register a callback function for jingyin issue
//  Author:         tom.fu
//	Note:           
/**********************************************************************************/
PUBLIC void LAYER1_RegSilenceFunc(LAYER1_AUDIO_SILENCEPROCESS_PFUNC silence_callback);

/**********************************************************************************/
//  Description:    This function carry out the jingyin callback function
//  Author:         tom.fu
//	Note:           
/**********************************************************************************/
PUBLIC void LAYER1_ProcessAudioSilence(void);
						
/*****************************************************************************
    Description:    This function stop the record
    Author:         yujun.ke
	Note:           
*****************************************************************************/						
extern void LAYER1_StopRecord(void);

/*****************************************************************************/
//  Description:    This function start the record flow
//  Author:         yujun.ke
//	Note:           AUD_REC_FORMAT_E
/*****************************************************************************/
extern void LAYER1_StartRecord(  AUD_REC_FORMAT_E voice_format,
							    RECORD_CALLBACK callback_fun,
				                BOOLEAN is_from_mic,
				                BOOLEAN is_from_net
					   		 );

 /*****************************************************************************/
//  Description:    This function config the amr parameters
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/

extern void LAYER1_CfgAmrParam( AMR_MODE amr_mode, uint8 channels, uint8 sample_rate );

 /*****************************************************************************/
//  Description:    This function transmit the recorded data to uplayer buffer
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void LAYER1_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length );


 /*****************************************************************************/
//  Description:    This function start the play flow
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void LAYER1_StartPlay( AUD_REC_FORMAT_E voice_format,
					           PLAY_CALLBACK callback_fun,
				               BOOLEAN is_to_speaker,
				               BOOLEAN is_to_net
	                          );
extern void LAYER1_StartPlayEx( AUD_REC_FORMAT_E voice_format,
		               PLAY_CALLBACK callback_fun,
		               BOOLEAN is_to_speaker,
		               BOOLEAN is_to_net
                     );

#ifdef MS_OAPI_SUPPORT
extern void set_audio_oapi_support(BOOLEAN flag);
extern BOOLEAN get_audio_oapi_support(void);
#endif
 /*****************************************************************************/
//  Description:    This function transmit data to downlayer for playing
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void LAYER1_WriteVoiceData (uint16 * source_buf_ptr,uint16 data_length);

 /*****************************************************************************/
//  Description:    This function stop the paly process
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void LAYER1_StopPlay( void );

extern void LAYER1_StopPlayEx( void ); 

/*****************************************************************************
    Description:    This function will re-transmite audio parameters to dsp 
    				to change the EQ parameters
    Author:         yujun.ke
	Note: cr27477          
*****************************************************************************/
void LAYER1_SetEqParameter(AUD_EQ_TYPE_E eq_type);


/*****************************************************************************
    Description:    this function sets the mute type of the system, 
    Author:         yujun.ke
	Note:           mute_type indicatea the speaker output type
*****************************************************************************/

void LAYER1_SetDlnkMute(BOOLEAN is_enable,uint16 dlnk_mute_type);

/*****************************************************************************
    Description:    this function sets the mute type of the system, 
    Author:         yujun.ke
	Note:           mute_type indicatea the microphone input type
*****************************************************************************/

void LAYER1_SetUlnkMute(BOOLEAN is_enable,uint16 ulnk_mute_type);

/*****************************************************************************
    Description:    this function sets the volume of the system, 
    Author:         yujun.ke
	Note:           volume_level indicate the db value
*****************************************************************************/

void LAYER1_SetDevVolume(uint16 volume_level);
/*****************************************************************************
    Description:    this function sets the extra volume of the system, 
    Author:         janson.liu
	Note:          
*****************************************************************************/
extern void LAYER1_SetDevExtraVolume(   BOOLEAN flag, uint16  usValue );

/*****************************************************************************
    Description:    This function transmite the audio parameters of certain 
                    device mode to Dsp 
    Author:         yujun.ke
	Note:           
*****************************************************************************/

void LAYER1_SetDevModeParam(
	    uint16  *param_buffer_ptr,//buffer where stores the parameter of one mode
	    uint32 length           //byte uint
	    );

//begin cr79575 yang wuding 2007.5.23
/*******************************************************************/
//	Function name:   L1API_GetAudioHandsetParamP
// 	Discription: 	 Return audio_handset pointer saved in audio_param 
//  Author: 		 Jimmy.Jia
//	Note:           
/*******************************************************************/
extern AUDIO_TEST_DATA_T * L1API_GetAudioHandsetParamP(void);

/*******************************************************************/
//	Function name:   L1API_GetAudioHandfreeParamP
// 	Discription: 	 Return audio_handfree pointer saved in audio_param 
//  Author: 		 Jimmy.Jia
//	Note:           
/*******************************************************************/
extern AUDIO_TEST_DATA_T * L1API_GetAudioHandfreeParamP(void);

/*******************************************************************/
//	Function name:   L1API_GetAudioEarphoneParamP
// 	Discription: 	 Return audio_earphone pointer saved in audio_param 
//  Author: 		 Jimmy.Jia
//	Note:           
/*******************************************************************/
extern AUDIO_TEST_DATA_T * L1API_GetAudioEarphoneParamP(void);
 //end cr79575 yang wuding 2007.5.23
 
/*----------------------------------------------------------------
	FUNCATION NAME:	LAYER1_Mp3SynthFrame
	Description:    Ref require layer1 to do mp3 and sbc by this function				 
	Notes: 	
	Author:         @
-----------------------------------------------------------------*/
//the redefinition of the interface 
extern PUBLIC BOOLEAN LAYER1_Mp3SynthFrame (
    L1_MP3_SYNTH_PARAS_T * synthParasPtr, 
    L1_MP3_SBC_PARAS_T * sbcParasPtr,
    BOOLEAN report_data_type, 
    GET_DATA_CALLBACK callback
);



/*----------------------------------------------------------------
	FUNCATION NAME:	Layer1_Cmd_Audio
	Description:    send signal to layer1 				 
	Notes: 	
	Author:         @tom.fu for cr118941
-----------------------------------------------------------------*/
extern void Layer1_Cmd_Audio(Audio_CmdQ_T *cmd_msg);

/*----------------------------------------------------------------
	FUNCATION NAME:	Layer1_Audio_Register
	Description:    register callback for audio operation	 
	Notes: 	
	Author:         @tom.fu for cr118941
-----------------------------------------------------------------*/
extern void Layer1_Audio_Register(
	LAYER1_AUDIO_CALLBACK callback_fun
);

/*----------------------------------------------------------------
	FUNCATION NAME:	Layer1_is_insearchingprogress
	Description:    judge whether the dsp is search arfcn	 
	Notes: 	
	Author:         @tom.fu for cr118941
-----------------------------------------------------------------*/
extern BOOLEAN Layer1_AudioIsInsearchingprogress(void);

/*****************************************************************************
    Description:    this function used to start magic voice function 
    Author:        
	Note:           
*****************************************************************************/
void LAYER1_StartMagicVoiceFunc(MAG_CALLBACK       callback_fun,
                                uint16              type,     //used to indicate which direction is valid.
                                uint16	            uplink_value,
                                uint16              downlink_value);
                                
/*****************************************************************************
    Description:    this function used to stop magic voice function 
    Author:         
	Note:           
*****************************************************************************/
void LAYER1_StopMagicVoiceFunc(void) ;

/*****************************************************************************
    Description:    this function used to start magic voice function 
    Author:        
	Note:           
*****************************************************************************/
void LAYER1_SetMagicVoiceParam( uint16  type,
                                uint16  uplink_value, 
                                uint16  downlink_value) ;

/*****************************************************************************/
//  Description:    This function stop the dsp log record process
//  Author:         William Qian
//	Note:
/*****************************************************************************/
extern BOOLEAN LAYER1_StopDspLogRecord(
                void
                );
                
/*****************************************************************************/
//  Description:    This function start the dsp log record flow
//  Author:         William Qian
//	Note:           
/*****************************************************************************/
extern BOOLEAN LAYER1_StartDspLogRecord(
                void
			   	);

 /*****************************************************************************/
//  Description:    This function transmit the iq data to uplayer buffer
//  Author:         William Qian
//	Note:
/*****************************************************************************/
void LAYER1_ReadIQData(
                uint16  data_length
                );

/*****************************************************************************/
//  Description:    LAYER1_StartRecord_Downlink  API function
//  Author:        janson.liu
//	Note:  call in the pld used to register the callback function
/*****************************************************************************/
void LAYER1_StartRecord_Downlink(RECORD_CALLBACK2 callback);

/*****************************************************************************/
//  Description:    LAYER1_StopRecord_Downlink  API function
//  Author:         janson.liu
//	Note:  call in the pld used to unregister the callback function
/*****************************************************************************/
void LAYER1_StopRecord_Downlink(void);

/*****************************************************************************/
//  Description:    LAYER1_StartRecord_Ad  API function
//  Author:         janson.liu
//	Note:  call in the pld used to register the callback function
/*****************************************************************************/
void LAYER1_StartRecord_Ad(RECORD_CALLBACK2 callback);

/*****************************************************************************/
//  Description:    LAYER1_StopRecord_Ad  API function
//  Author:         janson.liu
//	Note:  call in the pld used to unregister the callback function
/*****************************************************************************/
void LAYER1_StopRecord_Ad(void);

/*****************************************************************************/
//  Description:    LAYER1_SendVoiceDataToDA  API function
//  Author:         janson.liu
//	Note:  call in the pld used to send the data for dsp to spreak
/*****************************************************************************/

void LAYER1_SendVoiceDataToDA(uint16 *data_ptr);

/*****************************************************************************/
//  Description:    LAYER1_SendVoiceDataToNet  API function
//  Author:         janson.liu
//	Note:  call in the pld used to send  the data for dsp to transmit
/*****************************************************************************/

void LAYER1_SendVoiceDataToNet(uint16 *data_ptr);

/*****************************************************************************/
//  Description:    LAYER1_GetPCMUPData
//  Author:         janson.liu
//	Note:   called in the dsp int, used to fetch the uplink data
/*****************************************************************************/

void LAYER1_GetPCMUPData(void);

/*****************************************************************************/
//  Description:    LAYER1_GetPCMDNData
//  Author:         janson.liu
//	Note:   called in the dsp int, used to fetch the downlink data
/*****************************************************************************/

void LAYER1_GetPCMDNData(void);
                

PUBLIC void LAYER1_SetToneAPIWorkMode( BOOLEAN is_clean_API);

PUBLIC BOOLEAN LAYER1_IsToneAPIClean( void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }  
#endif

#endif  // _LAYER1_AUDIO_H
