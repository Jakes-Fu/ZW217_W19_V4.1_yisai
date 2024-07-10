/******************************************************************************
** File Name:      audio_config.h
** Author:         shujing.dong
** DATE:           11/11/2010
** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.
** Description:    This file defines the basic operation interfaces
**                 of apm_codec
******************************************************************************

******************************************************************************
**                        Edit History
**  -----------------------------------------------------------------------
** DATE           NAME             DESCRIPTION
** 11/11/2010     shujing.dong     Create.
******************************************************************************/
#ifndef _AUDIO_CONFIG_H
#define _AUDIO_CONFIG_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "audio_api.h"
#include "agc_exp.h"
/**---------------------------------------------------------------------------**
**                        Debugging Flag                                     **
**---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
#define AUDIO_CODEC_PROD_LOWPRIORITY   0
#define AUDIO_CODEC_PROD_HIGHPRIORITY  1
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/


extern HAUDIOCODEC hMIDICodec;
extern HAUDIOCODEC hAACCodec;
extern HAUDIOCODEC hAACLCCodec;
extern HAUDIOCODEC hAMRCodec;
extern HAUDIOCODEC hMP3Codec;
extern HAUDIOCODEC hMP3DSPCodec;
extern HAUDIOCODEC hWAVCodec;
extern HAUDIOCODEC hWMACodec;
extern HAUDIOCODEC hVoiceCodec;
extern HAUDIOCODEC hVoiceLBCodec;
extern HAUDIOCODEC hCustomCodec;
extern HAUDIOCODEC hDtmfCodec;
extern HAUDIOCODEC hGenericCodec;
extern HAUDIOCODEC hRecordAdpcmCodec;
extern HAUDIOCODEC hRecordCodec;
extern HAUDIOCODEC hDSPCodec;
extern HAUDIOCODEC hAMRDSPCodec;
extern HAUDIOCODEC hVtCodec;
extern HAUDIOCODEC hUplinkCodec;
extern HAUDIOCODEC hDownlinkCodec;
extern HAUDIOCODEC hNullCodec;
extern HAUDIOCODEC hArmAdCodec;
extern HAUDIOCODEC hAdOutputCodec;
extern HAUDIOCODEC hDspDataCodec;
#ifdef TA8LBR_SUPPORT
extern HAUDIOCODEC hRA8LBRCodec;
#endif

#ifdef DRA_SUPPORT
extern HAUDIOCODEC hDRACodec;
#endif

extern HAUDIODEV hAUDDEV;
extern HAUDIODEV hAUDDEVDUAL;
extern HAUDIODEV hLAYER1;
extern HAUDIODEV hLOOPBACKDEV;
extern HAUDIODEV hRECORDVB;
extern HAUDIODEV hAUDDEVRECORD;
extern HAUDIODEV hMP4RECORDVB;
extern HAUDIODEV hDSPDATADEVICE;
extern HAUDIODEV hPCMDev;
extern HAUDIODEV hL1Rec;
extern HAUDIODEV hA2DPDev;
extern HAUDIODEV hMixDev;
extern HAUDIODEV hBackgroundMusicDev;
extern HAUDIODEV hLINEINDev;
extern HAUDIODEV hLINEINADDev;
extern HAUDIODEV hFMDev;
extern HAUDIODEV hFMADDev;
extern HAUDIODEV hMTVVB;

extern HAUDIOEXP hTransSamExp;

extern HAUDIOEXP hEQExp;
extern HAUDIOEXP hAGCExp;
extern HAUDIOEXP hAUDPROCExp;
extern HAUDIOEXP hAUDENHAExp;
extern HAUDIOEXP hRateControlExp;
extern HAUDIOEXP hDigitalGainExp;
extern HAUDIOEXP hAUDARMRECExp;

extern HAUDIOCODEC hArmToneCodec;

#ifdef CAT_MVOICE_SUPPORT
extern HAUDIO hCatMvsample;
extern HAUDIOCODEC hCatMvSampleCodec;
extern HAUDIOEXP hCatMvMixExp;
#endif

#ifdef MIC_SAMPLE_SUPPORT
extern HAUDIO      hMicSample;
extern HAUDIOCODEC hMicSampleCodec;
PUBLIC AUDIO_RESULT_E AUD_EnableMicSample(BOOLEAN is_enable);

#endif

#ifdef MRAPP_SUPPORT
#if defined(SKY_CAT_SUPPORT) && !defined(WIN32) && !defined(CAT_MVOICE_SUPPORT)
extern HAUDIO hCatMvsample;
extern HAUDIOCODEC hCatMvSampleCodec;
extern HAUDIOEXP hCatMvMixExp;
#endif

#if defined(SKY_SKIRT_SUPPORT) && !defined(WIN32) && !defined(MIC_SAMPLE_SUPPORT)
extern HAUDIO      hMicSample;
extern HAUDIOCODEC hMicSampleCodec;
PUBLIC AUDIO_RESULT_E AUD_EnableMicSample(BOOLEAN is_enable);
#endif
#endif

typedef enum
{
    REC_BY_DSPVB = 0,
    REC_BY_ARMVB,
    REC_BY_PERIPHERAL,
    AUD_REC_DEV_TYPE_MAX
}AUD_REC_DEV_TYPE_E;

typedef struct
{
    uint16 UL_equalizer_coeff[33];
    uint16 DL_equalizer_coeff[33];


    uint16 UL_PGA_gain;
    uint16 DL_PGA_gain;

    uint16 UL_digital_gain;
    uint16 DL_digital_gain;

    uint16 UL_digital_scale;
    uint16 DL_digital_scale;

    uint16 midi_PGA_gain_base;
    uint16 digital_sidetone_gain;

    uint16 DL_IIR1_coeff[6];
    uint16 DL_IIR2_coeff[6];

    uint16 reserved[2];
} AUDIO_TEST_DATA_T;



#define AUD_PLAY_FOREVER            0xFFFFFFFF

// Volume of max value.
#define AUD_MAX_MIC_VOLUME          9
#define AUD_MAX_SPEAKER_VOLUME      9
#define AUD_MAX_SIDE_TONE_GAIN      10

//background music level
#define  AUD_BKMUSIC_LOWEST_LEVEL       0  //0:mute
#define  AUD_BKMUSIC_HIGHEST_LEVEL      5
#define  AUD_BKMUSIC_TOTAL_LEVEL        6
// Echo test callback function.
typedef void (*AUD_ECHOCALLBACK_PFUNC)(uint32);

// Audio type.
typedef enum
{
    AUD_TYPE_CUSTOM_TONE,
    AUD_TYPE_DTMF_TONE,
    AUD_TYPE_GENERIC_TONE,
    AUD_TYPE_RING,
    AUD_TYPE_VOICE,
    AUD_TYPE_RECORD,
    AUD_TYPE_MAX            // Reserved, user can't use it.
} AUD_TYPE_E;

typedef enum
{
    AUD_DTMF_Zero,          // 0
    AUD_DTMF_One,           // 1
    AUD_DTMF_Two,           // 2
    AUD_DTMF_Three,         // 3
    AUD_DTMF_Four,          // 4
    AUD_DTMF_Five,          // 5
    AUD_DTMF_Six,           // 6
    AUD_DTMF_Seven,         // 7
    AUD_DTMF_Eight,         // 8
    AUD_DTMF_Nine,          // 9
    AUD_DTMF_letterA,       // A
    AUD_DTMF_letterB,       // B
    AUD_DTMF_letterC,       // C
    AUD_DTMF_letterD,       // D
    AUD_DTMF_Pond,          // #
    AUD_DTMF_Star,          // *
    AUD_DTMF_MAX_ID         // Reserved, user can't use it.
} AUD_DTMF_TONE_ID_E;


// Generic tone ID.
typedef enum
{
    AUD_GENERIC_PRESS_KEY_TONE,
    AUD_GENERIC_WARNING_TONE,
    AUD_GENERIC_BATT_WARNING_TONE,
    AUD_GENERIC_DIAL_TONE,
    AUD_GENERIC_CONGESTION_TONE,
    AUD_GENERIC_SUBSCRIBER_BUSY_TONE,
    AUD_GENERIC_SPECIAL_DIAL_TONE,
    AUD_GENERIC_SV_CALL_WAITING_TONE,
    AUD_GENERIC_SV_RADIO_PATH_ACK_TONE,
    AUD_GENERIC_SV_RADIO_PATH_NOT_AVAIL_TONE,
    AUD_GENERIC_SV_ERROR_INFO_TONE,
    AUD_GENERIC_INFO_FREE_TONE,

    // User could add new at here.
    // ......

	AUD_GENERIC_TONE_ID_MAX     // Reserved, user can't use it.
} AUD_GENERIC_TONE_ID_E;

// Audio ring data type.
typedef enum
{
    AUD_MIDI_RING,          // File format:
                            //  General Midi file format

    AUD_SINGLE_TONE_RING,   // File format(in BigEndian):
                            //  uint16(type),       // AUD_SINGLE_TONE_RING
                            //  uint16(data_len),   // Number of bytes of ring data
                            //  uint16(freq),   uint16(duration millisecond),
                            //  uint16(freq),   uint16(duration millisecond),
                            //  ......
                            //  uint16(freq),   uint16(duration millisecond)

                            // Note: duration = 0xFFFF, means play for ever.

    AUD_DUAL_TONE_RING,     // File format(in BigEndian):
                            //  uint16(type),       // AUD_DUAL_TONE_RING
                            //  uint16(data_len),   // Number of bytes of ring data
                            //  uint16(freq1),  uint16(freq2),  uint16(duration millisecond),
                            //  uint16(freq1),  uint16(freq2),  uint16(duration millisecond),
                            //  ......
                            //  uint16(freq1),  uint16(freq2),  uint16(duration millisecond)

    AUD_TRIPLE_TONE_RING,   // File format(in BigEndian):
                            //  uint16(type),       // AUD_TRIPLE_TONE_RING
                            //  uint16(data_len),   // Number of bytes of ring data
                            //  uint16(freq1),  uint16(freq2),  uint16(freq3),  uint16(duration millisecond),
                            //  uint16(freq1),  uint16(freq2),  uint16(freq3),  uint16(duration millisecond),
                            //  ......
                            //  uint16(freq1),  uint16(freq2),  uint16(freq3),  uint16(duration millisecond)

    AUD_SMAF_RING,          // File format:
                            //  Defined by Yamaha.

    AUD_WAVE_RING,          // File format:
                            //  wave sound 16bit samples data ,sound sampling rate is 16k

    AUD_MP3_RING,          	// File format:
                            //  mp3

    AUD_AMR_RING,           // File format
                            // AMR_NB
    AUD_RING_TYPE_MAX       // Reserved, user can't use it.
} AUD_RING_TYPE_E;

// definition of type of background music volume  .
typedef enum
{
    AUD_BKMUSIC_BOTH_INVALID,/*both uplink and downlink paras are invalid*/
    AUD_BKMUSIC_UPLINK_VALID,        /*uplink para is valid*/
    AUD_BKMUSIC_DOWNLINK_VALID,  /*downlink para is valid*/
    AUD_BKMUSIC_BOTH_VALID,           /*both uplink and downlink paras are valid*/
    AUD_BKMUSIC_PARAM_TYPE_MAX   /* Reserved, user can't use it.*/
} AUD_BKMUSIC_PARAM_TYPE_E;

typedef enum
{
    AUD_EQ_NORMAL = 0x0,
    AUD_EQ_BASS,
    AUD_EQ_ALT,
    AUD_EQ_SPECIAL
}AUD_EQ_TYPE_E;


typedef struct AUD_SINGLE_TONE_tag
{
    uint16  freq;
    uint16  duration;           // Number of milliseconds
} AUD_SINGLE_TONE_T;

typedef struct AUD_RING_DATA_INFO_tag
{
    AUD_RING_TYPE_E type;
    uint8           name[32];
    uint32          data_len;
	const uint8	   *data_ptr;
} AUD_RING_DATA_INFO_T;

enum aud_stereo_mode
{
    AUD_STEREO 			= 0,	// dual channel
    AUD_NON_STEREO		= 1		// single channel
};

typedef enum
{
        AUD_REC_PCM= 0x0,
        AUD_REC_FS,
        AUD_REC_EFS,
        AUD_REC_AMR,
        AUD_REC_ADPCM,
        AUD_REC_DSP_DATA_MIN, //=5
        AUD_REC_DSP_DATA_1 = AUD_REC_DSP_DATA_MIN,//=5
        AUD_REC_DSP_DATA_2, //=6
        AUD_REC_DSP_DATA_3, //=7
        AUD_REC_DSP_DATA_4, //=8
        AUD_REC_DSP_DATA_5, //=9
        AUD_REC_DSP_DATA_6, //=10
        AUD_REC_DSP_DATA_7, //=11
        AUD_REC_DSP_DATA_8, //=12
        AUD_REC_DSP_DATA_9, //=13
        AUD_REC_DSP_DATA_10, //=14
        AUD_REC_DSP_DATA_11, //=15
        AUD_REC_DSP_DATA_12, //=16
        AUD_REC_DSP_DATA_13, //=17
        AUD_REC_DSP_DATA_14, //=18
        AUD_REC_DSP_DATA_15, //=19
        AUD_REC_DSP_DATA_MAX = AUD_REC_DSP_DATA_15,
        AUD_REC_DSP_LOG,    //20
		AUD_REC_MP3, // fm record as mp3
		AUD_REC_AMR_SW,  //22
		AUD_REC_AMR_WB_SW, //23
        AUD_REC_FORMAT_MAX
}AUD_REC_FORMAT_E;

typedef enum
{
        MR475 = 0,/*4.75kbps*/
        MR515,
        MR59,
        MR67,
        MR74,
        MR795,
        MR102,
        MR122,
        MRDTX,
        N_MODES     /* number of (SPC) modes */
}AMR_MODE; //amr encode mode

// w217 only support AUDIO_OUT_SPK
typedef enum {
    AUDIO_OUT_DEFAULT = 0,
    AUDIO_OUT_EAR = 0x1,
    AUDIO_OUT_SPK = 0x2,
    AUDIO_OUT_HP = 0x4,
    AUDIO_OUT_MAX
}AUDIO_OUT_DEV_TYPE_E;

// w217 only support AUDIO_IN_MAINMIC
typedef enum {
    AUDIO_IN_DEFAULT =0,
    AUDIO_IN_MAINMIC = 0x1,
    AUDIO_IN_AUXMIC = 0x2,
    AUDIO_IN_HPMIC = 0x4,
    AUDIO_IN_MAX
}AUDIO_IN_DEV_TYPE_E;


//record call back function
typedef void (*RECORD_CALLBACK)(uint16 data_length);

typedef	void (*RECORD_CALLBACK2) (uint16 *);

typedef	void (*RECORD_CALLBACK3) (uint16 *, uint16 *, uint16 *);

typedef	void (*RECORD_CALLBACK4) (uint16 *, uint16 *, uint16 *, uint16 flag); //flag: 1:wb, 0: nb

//play call back function
typedef BOOLEAN (*PLAY_CALLBACK)(void);

// Ring callback function prototype.
typedef void (*AUD_RING_CALLBACK_PFUNC)(void);

typedef	void (*RECORD_GETPCMDATA) (uint16 *, uint32, uint8/*1: uplink, 2: downlink*/);

/**
 * read data callback function prototype.
 * @puiLength : unit: byte, [IN: size of pDest/OUT: size of data in pDest].
 */
typedef void (*READDATA_CALLBACK_PFUNC)(
	uint16 *pDest,
	uint32 *puiLength,
	void *param);

/**
 * write data callback function prototype.
 * @uiLength : unit: byte, [IN: size of data in pSrc].
 */
typedef void (*WRITEDATA_CALLBACK_PFUNC)(
	uint16 *pSrc,
	uint32 uiLength,
	void *param);

/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/

/*****************************************************************************/
//  Description:    This function init audio interface.
//  Author:         Xueliang.Wang
//  Note:           This function should be called after initializing RTOS.
/*****************************************************************************/
PUBLIC void AUD_Init (void);

/*****************************************************************************/
//  Description:    This function is used to set engineering audio parameters.
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUD_SetEngineeringParameter(  	// If succeed, return AUDIO_NO_ERROR,
                                            	// else return error value
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,
    AUDIO_TEST_DATA_T  parameter_data
    );

/*****************************************************************************/
//  Description:    This function is used to get engineering audio parameters.
//  Author:         Sunsome.Ju
//	Note:
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUD_GetEngineeringParameter(  	// If succeed, return AUDIO_NO_ERROR,
                                            	// else return error value
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,
    AUDIO_TEST_DATA_T *parameter_data
    );

/*****************************************************************************/
//  Description:    This function switchs layer1 mode to test mode.
//  Author:         fancier.fan
//  Note:
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUD_SwitchTestMode ( // If succeed, return AUDIO_NO_ERROR,
    // else return error value
    BOOLEAN is_test_mode
);

/*****************************************************************************/
//  Description:    This function is used to enable/disable volice loopback test.
//           is_enable:            1:enable   0:disable
//		usLoopbackType:
//		 		 		 0: AD->DA loop,
//		 		 		 1: AD->ul_process->dl_process->DA loop,
//		 		 		 2: AD->ul_process->encoder->decoder->dl_process->DA loop
//		usVoiceFormat:
//		 		               1: EFS vocoder
//		 		               2: HR Vocoder
//		 		               3: AMR Vocoder
//		delay_time:      unit: ms
//			                      range: 0-1000
//  Author:         shujing.dong
//  Note:
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUD_EnableVoiceLoopback (
    BOOLEAN is_enable,      // 0: disable  1:enable
    uint16 usLoopbackType,
    uint16 usVoiceFormat,
    uint16  delay_time      // millisecones
);

/*****************************************************************************/
// Description: United operation for POP noise restrain.
// Author: Benjamin
// Note:
/*****************************************************************************/
PUBLIC void AUD_DSPUnitedOperation(void);

/*****************************************************************************/
//  Description:     Get AGC Support Flag corresponding music type base on NV configuration
//  Author:            binggo
/*****************************************************************************/
PUBLIC BOOLEAN AUD_GetAGC_SupportFlag (AUDIO_AGC_INPUG_GAIN_TYPE_E eType);

/*****************************************************************************/
//  Description:    This function is used to set Audio PA.
//  Author:         Johnson.sun
//  Note:   provide for external interface, only reserved.
/*****************************************************************************/
PUBLIC void AUD_EnableAmplifier (BOOLEAN bEnable);

/*****************************************************************************/
//  Description:    This function is used to set Audio Codec Output Data Type.
/*****************************************************************************/
PUBLIC void AUD_SetOutputDataType (AUDIO_OUTPUT_DATA_TYPE_E eDataType);

/*****************************************************************************/
//  Description:    This function is used to get Audio Codec Output Data Type.
/*****************************************************************************/
PUBLIC AUDIO_OUTPUT_DATA_TYPE_E AUD_GetOutputDataType (void);

/*****************************************************************************/
//  Description:    This function is used to get record dev type.
/*****************************************************************************/
PUBLIC AUD_REC_DEV_TYPE_E AUD_GetRecordDevType (HAUDIODEV hDevice);

PUBLIC void AUD_Cfg_PcmTrans_Param (uint16 dest_samplerate);

PUBLIC uint16 AUD_Get_PcmTrans_Samplerate (void);
/*****************************************************************************/
//  Description:    This function config the amr parameters
//  Author:         yujun.ke
//  Note:
/*****************************************************************************/
void AUD_CfgAmrParam (AMR_MODE amr_mode, uint8 channels, uint8 sample_rate);

#ifdef BACKGROUND_MUSIC_ENABLE
/*****************************************************************************/
//  Description:    This function sets volume of background music.
//  Author:         Cherry.Liu
//  Note:
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUD_SetBkMusicVolume (
    AUD_BKMUSIC_PARAM_TYPE_E  eParamType,
    uint16  uiUplinkLevel,
    uint16  uiDownlinkLevel
);
#endif

PUBLIC void AUD_SetDgGain (
    int32 dac_gain
);

/*****************************************************************************/
//  Description:    interface for mmi to set eq mode.
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
PUBLIC BOOLEAN AUD_SetEqMode (
    int32 eq_mode
);

/*****************************************************************************/
//  Description:    interface for mmi to get current eq mode.
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
PUBLIC int32 AUD_GetEqMode (void);

/*****************************************************************************/
//  Description: Audio interface to call sbc encoder for dsp codec.
//  Author: TH
//  Note: NONE.
//****************************************************************************/
PUBLIC void AUD_SBC_Encode_For_DSPCodec (
    uint16 source_samplerate,
    uint16 frame_address,
    uint16 frame_size,
    void *callback
);

/*****************************************************************************/
//  Description: Audio interface to get  the busy or free status of the a2dp device  .
//  Author: TH
//  Note: NONE.
//****************************************************************************/
PUBLIC int32 AUD_A2dp_Is_Busy(void );

/*****************************************************************************/
//  Description: check if some exp need to work or not when some dev is working.
//               for example, hAUDPROCExp need not to work when output dev is bt.
//               need to work, return true; need not to work, return false.
//  Author: shujing.dong
//  Note: NONE.
//****************************************************************************/
PUBLIC BOOLEAN AUD_ExpWorked_ForSomeDev(HAUDIOEXP hExp, HAUDIODEV hDev);

PUBLIC AUDIO_RESULT_E AUD_EnableVoiceProcessConfig ( // If succeed, return ERR_AUD_NONE,
    // else return error values
    BOOLEAN is_enable,                   // SCI_TRUE: Enable,   SCI_FALSE: Disable
    BOOLEAN is_loopback,
    void * para_reserve
);

 /******************************************************************************
 * FUNCTION:		BT_GetHeadsetNrec
 *
 * RETURNS: get NREC flag of a headset
 *
 * NOTE: return 1: bt headset has NREC feature
 ******************************************************************************/
PUBLIC uint8 AUD_BTGetHeadsetNrec(void);

PUBLIC void VoiceProcess_GetDataFromUplink(uint16 *puiSrcData);
PUBLIC void VoiceProcess_SendDataToDownlink(uint16 *puiSrcData);
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/
#ifdef __cplusplus
}
#endif

#endif //end of _AUDIO_CONFIG_H

//end of file

