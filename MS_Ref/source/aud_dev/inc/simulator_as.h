#ifndef WIN32_DEV_H
#define WIN32_DEV_H

#include "audio_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


PUBLIC HAUDIODEV WIN32_DEV_RegAudioDevice(void);

typedef int32 (*AUDIO_OUT_OPEN)(uint32 samplerate);
typedef int32 (*AUDIO_OUT_PAUSE)(void);
typedef int32 (*AUDIO_OUT_RESUME)(uint32 samplerate);
typedef int32 (*AUDIO_OUT_STOP)(void);

void AudioOut_Set_Open(AUDIO_OUT_OPEN audio_out_open);
void AudioOut_Set_Pause(AUDIO_OUT_PAUSE audio_out_pause);
void AudioOut_Set_Resume(AUDIO_OUT_RESUME audio_out_resume);
void AudioOut_Set_Stop(AUDIO_OUT_STOP audio_out_stop);

int32 AudioOut_GetPcmData(void* dest_buffer,uint32 read_len);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of win32_dev.h
#endif
