/******************************************************************************
 ** File Name:      audio.c                                                   *
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
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "os_api.h"
#include "layer1_audio.h"

#include "audio_drv.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define AUDIO_DUMMY_BEEP_FREQ   1000
#define AUDIO_DUMMY_BEEP_VOL    5

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to play special tone.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_PlaySpecialTone(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    TONE_ID_E   tone_id,            // Special tone enumeration value.
    BOOLEAN     b_uplink            // If uplink this tone.(SCI_TRUE: Uplink)
    )
{
    //SCI_TRACE_LOW:"LAYER1_PlaySpecialTone: tone_id = %d, b_uplink = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_45_112_2_18_1_4_5_20,(uint8*)"dd", tone_id, b_uplink);
    
    AUDDRV_Beep(AUDIO_DUMMY_BEEP_FREQ, AUDIO_DUMMY_BEEP_VOL);
    
    return SCI_SUCCESS;
}

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
    )
{
    //SCI_TRACE_LOW:"LAYER1_PlayGenericTone: freq1 = %d, freq2 = %d, freq3 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_63_112_2_18_1_4_5_21,(uint8*)"ddd", freq1, freq2, freq3);
    
    AUDDRV_Beep(freq1, AUDIO_DUMMY_BEEP_VOL);
    
    return SCI_SUCCESS;
}
    

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to stop playing tone.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_StopTone(  // If succeed, return SCI_SUCCESS,
                                // else return SCI_ERROR
    BOOLEAN b_pause     // SCI_TRUE: a pause    SCI_FALSE: stop
    )
{
    //SCI_TRACE_LOW:"LAYER1_StopTone: b_pause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_80_112_2_18_1_4_5_22,(uint8*)"d", b_pause);
    
    AUDDRV_Stop();
    
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to set volume(include voice and tone).
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SetVolume( // If succeed, return SCI_SUCCESS,
                                // else return SCI_ERROR
    uint16  mic_vol,    // Volume value of mic(0 - DSP_VOL_MIC_MAX)
    uint16  speaker_vol // Volume value of speaker(0 - DSP_VOL_SPEAKER_MAX)
    )
{
    //SCI_TRACE_LOW:"LAYER1_SetVolume: Mic = %d, Speaker = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_97_112_2_18_1_4_5_23,(uint8*)"dd", mic_vol, speaker_vol);
    
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to switch mute to ON or OFF.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_MuteSwitch(    // If succeed, return SCI_SUCCESS,
                                    // else return SCI_ERROR
    BOOLEAN b_mute                  // SCI_TRUE: Mute ON
    )
{
    //SCI_TRACE_LOW:"LAYER1_MuteSwitch: b_mute = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_111_112_2_18_1_4_5_24,(uint8*)"d", b_mute);
    
    return SCI_SUCCESS;    
}   

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
    )
{
	//SCI_TRACE_LOW:"LAYER1_OpenSoundDevice: Mic = %d, Speaker = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_126_112_2_18_1_4_5_25,(uint8*)"dd", b_mic, b_speaker);
	
    return SCI_SUCCESS;    
}
    
/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to switch sound device.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SwitchSoundDevice( // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN  b_aux_mic,     // SCI_TRUE: use Aux mic,       SCI_FALSE: use mic
    BOOLEAN  b_aux_speaker  // SCI_TRUE: use Aux speaker,   SCI_FALSE: use speaker
    )
{
    //SCI_TRACE_LOW:"LAYER1_SwitchSoundDevice: Mic = %d, Speaker = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_141_112_2_18_1_4_5_26,(uint8*)"dd", b_aux_mic, b_aux_speaker);

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function switchs layer1 mode to test mode.
//  Author:         fancier.fan
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SwitchTestMode(
	BOOLEAN is_test_mode
	)    
{
	//SCI_TRACE_LOW:"LAYER1_SwitchTestMode: is_test_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_153_112_2_18_1_4_5_27,(uint8*)"d", is_test_mode);

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function side tone play.
//  Author:         Fancier.fan
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_PlaySideTone(
    uint16 tone_gain    //
    )
{
    //SCI_TRACE_LOW:"LAYER1_PlaySideTone: tone_gain = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_165_112_2_18_1_4_5_28,(uint8*)"d", tone_gain);
    
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to send a signal to Layer1
//                  to switch echo cancel.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 LAYER1_SwitchEchoCancel( // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN  is_echo_cancel_on     // SCI_TRUE: turn on echo cancelation,       SCI_FALSE: turn off
    )
{
    //SCI_TRACE_LOW:"LAYER1_SwitchEchoCancel: is_echo_cancel_on = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DUMMY_LAYER1_AUDIO_179_112_2_18_1_4_5_29,(uint8*)"d", is_echo_cancel_on);
    
    return SCI_SUCCESS;    
}
    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
