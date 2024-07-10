/******************************************************************************
 ** File Name:      midi_adp.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           17/11/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    MIDI Codec adapter layer.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 17/11/2005     Benjamin.Wang     Create.                                  *
 ** 04/15/2008     Binggo.Zhou         Modify for Audio Service 2.0.     *
 ******************************************************************************/

#ifndef _MIDI_ADP_H
#define _MIDI_ADP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif
#include "audio_api.h"
#include "apm_codec.h"
/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
#define MIDI_CODEC_EXT_OPE_PARA_COUNT 2
#define MIDI_OPT_EXT_OPE_PARA_COUNT 2	
#define MIDI_EQ_PARAS_NUM 5
#define MIDI_LOW_CUT_FILTER_PARAS_NUM 3
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct MIDI_FileInfo_struct
{
    uint32 uiMidiFormat;
    uint32 uiTrackNum;
} MIDI_FILEINFO_T;


//cherry add for midi optimization ---  begin
typedef enum
{
	 MIDI_MUSIC_VIBRATION_OFF=0,
	 MIDI_MUSIC_ONLY ,
	 MIDI_VIBRATION_ONLY ,
	 MIDI_MUSIC_VIBRATION_ON,
	 MIDI_WORKING_MODE_MAX
}MIDI_WORKING_MODE_E;

typedef enum
{
	 MIDI_PLAY_MODE_MONO=1,
	 MIDI_PLAY_MODE_STEREO ,
	 MIDI_PLAY_MODE_MAX
}MIDI_OPT_PLAY_MODE_E; 

typedef void (*MIDI_SETVIBRATOR_CALLBACK_PFUNC)(
	BOOLEAN VibrationOn,
	uint8 Note, 
	uint8 Velocity);

typedef enum//  3  para  types
{   
    MIDI_OPT_PARA_NULL = 0,
    MIDI_OPT_PARA_WORKING_MODE ,
    MIDI_OPT_PARA_PLAY_MODE,    
    MIDI_OPT_PARA_VIBRATOR_CALLBACK,
    MIDI_OPT_PARA_MAX
}MIDI_OPT_PARA_TYPE_E;

union Midi_opt_para_union
{
    MIDI_WORKING_MODE_E workMode;               
    MIDI_OPT_PLAY_MODE_E playMode;
    MIDI_SETVIBRATOR_CALLBACK_PFUNC vibratorCallback;
};

 typedef struct 
{
    MIDI_OPT_PARA_TYPE_E eParaType;			  
	union Midi_opt_para_union unAudProcPara; 

}MIDI_OPT_PARA_T;

typedef enum 
{
    SOUNDBANK_DEFAULT,
    SOUNDBANK_VERSION1, // sf2
    SOUNDBANK_VERSION2 
}SOUNDBANK_TYPE_E;
   

typedef struct midi_soundbank_type
{
    uint32 size;  //in kbyte unit
    SOUNDBANK_TYPE_E type;
}MIDI_SOUNDBANK_T;


typedef union MIDI_Codec_Para_Setting_union
{
    uint16 usPolyNum;
    uint16 usSoundBankBits;
    uint16 usTms;
    int16  sStartVolumn;
    int16  sEndVolumn;
    int16  sTargetCdb;
    BOOLEAN bLpfFilterEnable;
// 6600L1 does not support following parameters
    uint16 uiLpfCutCentFreq;//range:1024~16384,if it equals to 0,then use the default value
    struct {
        BOOLEAN bLcfFilterEnable;
        int32 B0, B1, A1;
    } tLcfParams;
    struct {
        BOOLEAN bEQEnable;
        int32 B0, B1, B2, A1, A2;
    } tEQParams;
    struct {
	MIDI_WORKING_MODE_E wmode;
	MIDI_SETVIBRATOR_CALLBACK_PFUNC pSetVibrateCallbackFunc;
    } tWorkingModeParams;
    MIDI_SOUNDBANK_T soundbank_type;
    BOOLEAN is_stereo;
}MIDI_CODEC_PARA_SETTING_U;


typedef struct 
{  
    HAUDIO    hAudioHandle; 
    MIDI_CODEC_PARA_SETTING_U uPara;
}MIDI_CODEC_EXT_OPE_PARA_T;	

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This Function is used to initialize the audio server. 
//  Author:         Benjamin.Wang
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC MIDI_RegCodecPlugger(HCOLLISION hCollision, AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo);

/*!<Ext Ope Symbol.*/
/* SET_POLYNUM
     SET_SBBITS
     SET_STARTVOLUMN
     SET_ENDVOLUMN
     SET_GCTVol
     SET_COMPRESS
     
     SET_LPFFILTER
     SET_LPFCUTCENTFREQ
     SET_EQFILTER
     SET_LCFFILTER
     SET_WORKINGMODE
*/
/* Parameter [IN]: argc:Para Counter
                            argv:MIDI_CODEC_EXT_OPE_PARA_T Struct Pointer
*/
//Ext Ope Setting of MIDI take effect befroe playing 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _MIDI_ADP_H

// End of midi_adp.h

