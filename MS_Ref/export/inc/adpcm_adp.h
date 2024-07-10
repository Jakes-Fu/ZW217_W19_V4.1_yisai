/**************************************************************************
 ** File Name:      adpcm_adp.h                                           *
 ** Author:         Michael chen                                          *
 ** Date:           12/07/2005                                            *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.     *
 ** Description:    adaptation layer for ADPCM playback.                  *
***************************************************************************
***************************************************************************
 **                        Edit History                                   *
 ** ----------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                           *
 ** 12/07/2005     Michael.Chen     Create.                               *
***************************************************************************/
#ifndef _ADPCM_ADP_H_
#define _ADPCM_ADP_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Register IMA ADPCM decoder to the audio server.
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC ADPCM_RegCodecPlugger(HCOLLISION hCollision, uint32 uiPriority);

 /*****************************************************************************/
//  Description:    Create a dynamic thread for playing a wav file contained 
//                    IMA ADPCM audio data.
//  Author:         Michael.Chen
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_Play(
HAUDIO hAudioHandle, 
uint32 uiOffset, 
AUDIO_CALLBACK_PFUNC pCallbackFunc
);

/*****************************************************************************/
//  Description: stop playing ADPCM and delete the dynamic ADPCM thread.    
//  Author:      Michael.Chen
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_Stop(HAUDIO hAudioHandle);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_Pause(HAUDIO hAudioHandle);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_Resume(HAUDIO hAudioHandle);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_Seek(HAUDIO hAudioHandle, uint32 uiOffset);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_SetSpeed(HAUDIO hAudioHandle, uint32 uiSpeedX);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_SetDigitalGain(
HAUDIO hAudioHandle,
uint32 uiDigitalGain);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 ADPCM_GetDigitalGain(HAUDIO hAudioHandle);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_GetFileInfo(
HAUDIO hAudioHandle, 
AUDIO_FILEINFO_T* ptFileInfo            // OUT parameter
);

/*****************************************************************************/
// Description:    This function is used to precheck the ADPCM file format.
// Author:         Michael.Chen
// Note:           if the wav file contained ADPCM data is valid, return AUDIO_NO_ERROR
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_VerifyFormat(
HAUDIO hAudioHandle,
AUDIO_FILEINFO_T* ptFileInfo        // out param
);

/*****************************************************************************
// Description:    check whether the wav file(ADPCM) is valid or not.
// Author:         Michael.Chen
// Note:           if valid, return AUDIO_NO_ERROR.
******************************************************************************/
PUBLIC AUDIO_RESULT_E Check_ADPCM_Wav_File_Format(
uint8 *data_buf,
AUDIO_FILEINFO_T* ptFileInfo        // out param
);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_GetPlayingInfo(HAUDIO hAudioHandle, AUDIO_PLAYINFO_T* ptPlayInfo);

/*****************************************************************************/
//  Description:     
//  Author:         Michael.Chen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 ADPCM_GetSampleRate(HAUDIO hAudioHandle);

/*****************************************************************************/
//  Description:     
//  Author:         Benjamin.Wang
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E ADPCM_SetSampleRate(HAUDIO hAudioHandle, uint32 uiSamplerate);

/*********************************************************************************
/	FUNCTION NAME: 		Write_ADPCMDecOutputBuf
/	DESCRIPTION:    	Write ADPCM Decoder output data to vb ring buffer
/	Author:             Michael.Chen
/	NOTES:			
**********************************************************************************/
PUBLIC void Write_ADPCMDecOutputBuf(
uint16* data_buf_ptr,
uint16* data_buf_ptr_r,
uint32 count,
uint32 track_num
);
              
#ifdef __cplusplus
    }
#endif

#endif /* _ADPCM_ADP_H_ */

/*~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~*/
