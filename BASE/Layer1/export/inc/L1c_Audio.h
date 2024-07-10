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

#ifndef _L1C_AUDIO_H
#define _L1C_AUDIO_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_audio.h"
//#include "gsm_gprs.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures     					       	  * 
 **---------------------------------------------------------------------------*/
typedef enum 
{
    LAYER1_SPECIALTONE_CMD = 0x1,
    LAYER1_GENERICTONE_CMD,
    LAYER1_STOPTONE_CMD,
    LAYER1_SWITCHDEV_CMD,
    LAYER1_SETPGAGAIN_CMD,
    LAYER1_ENABLEAUDIO_CMD,
    LAYER1_VOICELOOPBACK_CMD,
    LAYER1_ENABLECODEC_CMD
} LAYER1_CMD_TYPE;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define DSP_VOL_MIC_MAX         12
#define DSP_VOL_SPEAKER_MAX_TD     11

// @Xiangxin cr6574 03-12-25
#define MAX_SIDE_TONE_GAIN		4

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to play special tone.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_PlaySpecialTone(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    AUD_DTMF_TONE_ID_E   tone_id,            // Special tone enumeration value.
    BOOLEAN              b_uplink            // If uplink this tone.(SCI_TRUE: Uplink)
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to play a tone, which is combined by freq1, freq2 and freq3.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_PlayGenericTone(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    uint16  freq1,  // Freq to be played.
    uint16  freq2,  // Freq to be played.
    uint16  freq3   // Freq to be played.
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to stop playing tone.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_StopTone(	// If succeed, return SCI_SUCCESS,
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
    //uint16  mic_vol,    // Volume value of mic(0 - DSP_VOL_MIC_MAX)
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
    BOOLEAN b_mute,                  // SCI_TRUE: Mute ON
    uint16  mute_type
    );

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to open sound device.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_OpenSoundDevice(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN  b_mic,     // SCI_TRUE: Open,   SCI_FALSE: Close
    BOOLEAN  b_speaker  // SCI_TRUE: Open,   SCI_FALSE: Close
    );
    
	/*****************************************************************************/
	//	Description:	This function is used to send a signal to Layer1
	//					to switch sound device.
	//	Author: 		Xueliang.Wang
	//	Note:			
	/*****************************************************************************/
	PUBLIC uint32 LAYER1_SwitchSoundDevice( // If succeed, return SCI_SUCCESS,
											// else return SCI_ERROR
		BOOLEAN  b_aux_mic, 	// SCI_TRUE: use Aux mic,		SCI_FALSE: use mic
		BOOLEAN  b_aux_speaker,  // SCI_TRUE: use Aux speaker,	 SCI_FALSE: use speaker
		AUD_DEV_MODE_E	 mode
		);


/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to switch echo cancel.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SwitchEchoCancel( // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN  is_echo_cancel_on     // SCI_TRUE: turn on echo cancelation,       SCI_FALSE: turn off
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
//  Description:    This function side tone play.
//  Author:         Fancier.fan
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_PlaySideTone(
    uint16 tone_gain    //
    );
    
/*****************************************************************************/
//  Description:    This function set digital audio gain.
//  Author:         Leo.Feng
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetDigitalAudioGain(
    int8 digital_mic_gain , 
    int8 digital_speaker_gain 
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

PUBLIC uint32 LAYER1_DownlinkMuteEnable (
		BOOLEAN mute_enable,
		uint16  mute_type
 );

/*****************************************************************************/
//	Description:	This function is used to send a signal to Layer1
//					to set audio engineering paramters.
//	Author: 		Jenny.xu
//	Note:			
/*****************************************************************************/
PUBLIC uint32 LAYER1_GetAudioEngineeringParameter( // If succeed, return SCI_SUCCESS,
												   // else return SCI_ERROR
    AUD_DEV_MODE_E  		parameter_type,
    AUDIO_TEST_DATA_T       *parameter_data
	);

/*****************************************************************************/
//	Description:	This function is used to send a signal to Layer1
//					to set audio engineering paramters.
//	Author: 		Jenny.xu
//	Note:			
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetAudioEngineeringParameter( // If succeed, return SCI_SUCCESS,
												   // else return SCI_ERROR
    AUD_DEV_MODE_E  		parameter_type,
    AUDIO_TEST_DATA_T       parameter_data
	);
	
	PUBLIC uint32 LAYER1_EnableEchoTest (
			BOOLEAN is_echo_test_enable,
			uint16	freq,
			AUD_ECHOCALLBACK_PFUNC echo_callback
		);

#if 0
#ifndef NXP_LVVE_SUPPORT
/*****************************************************************************/
//	Description:	This function start/stop voice recognition
//	Author: 		Rossi.Luo
//	Note:			
/*****************************************************************************/
PUBLIC uint32 LAYER1_EnableVoiceLoopback(
	BOOLEAN is_enable,			   //0: disable  1:enable
	uint16	delay_time			   // millisecones
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
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }  
#endif

#endif  // _LAYER1_AUDIO_H
