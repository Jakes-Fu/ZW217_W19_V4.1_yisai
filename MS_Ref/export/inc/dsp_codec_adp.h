/******************************************************************************
** File Name:      dsp_codec.h                                              
** Author:         shujing.dong                                              
** DATE:           02/10/2006                                                
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of dsp_codec                     
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 02/10/2006     Shujing.Dong    Create.                                   *
** 04/30/2008      Binggo.Zhou     Modify For Audio Service 2.0                                 *
******************************************************************************/
#ifndef _DSP_CODEC_H
#define _DSP_CODEC_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
#include "asm.h"
#include "audio_config.h"
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
#define DSP_CODEC_EXT_OPE_PLAY_PARA_COUNT 5	
#define DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT 2
#define DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT 3
	
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef struct 
{  
    HAUDIO    hAudioHandle; 
    AUD_REC_FORMAT_E voice_format;
    PLAY_CALLBACK callback_fun;
    BOOLEAN is_to_speaker;
    BOOLEAN is_to_net;
}DSP_CODEC_EXT_OPE_PLAY_PARA_T;	

typedef struct
{
    uint16 * source_buf_ptr;
    uint16 data_length;
}DSP_CODEC_EXT_OPE_VOICE_DATA_T;   

typedef struct
{
    HAUDIO  hAudioHandle;   	
	AUD_REC_FORMAT_E recorder_format;
	RECORD_CALLBACK callback_fun;
}DSP_CODEC_EXT_OPE_START_RECORD_T;

typedef void (*DSP_CODEC_UPLOAD_DATA_CALLBACK)(int16 * p_buf ,uint32 length);

typedef struct 
{
    HAUDIO hAudioHandle;
    uint32 uiSamplesOnce;
    uint32 CachFrameCount;
    AUD_REC_FORMAT_E format;
    DSP_CODEC_UPLOAD_DATA_CALLBACK pUpLoadDataCallbackFunc;

}DSP_CODEC_EXT_OPE_UPLOAD_PARAM_T;

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/
	
/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//  Description:    This Function is used to initialize the codec. 
//  Author:         Shujing.Dong
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC DSP_CODEC_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _VOICE_ADP_H

//end of file

