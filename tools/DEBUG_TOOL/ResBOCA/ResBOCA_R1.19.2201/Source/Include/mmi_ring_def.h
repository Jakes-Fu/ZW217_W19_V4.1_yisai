#ifndef _MMI_RING_DEF_H__
#define _MMI_RING_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

// 定义铃声的类型
typedef enum _MMIAUDIO_TYPE_E
{
    MMIAUDIO_MIDI,			// File format: [hongliang]".mid",".midi"
                            //  General Midi file format  
                                                        
    MMIAUDIO_GENERIC_TONE,  // File format(in BigEndian):
                                            
    MMIAUDIO_CUSTOM_TONE,   // File format(in BigEndian):
                            
    MMIAUDIO_DTMF_TONE,		// File format(in BigEndian):

    MMIAUDIO_SMAF,          // File format: 
                            //  Defined by Yamaha.
                            
    MMIAUDIO_WAVE,          // File format: [hongliang]".wav"
                            //  wave sound 16bit samples data ,sound sampling rate is 16k                            
                            
    MMIAUDIO_MP3,           // File format:[hongliang]".mp3"
                            //  mp3
    MMIAUDIO_WMA,			//[hongliang]".wma"
    
    MMIAUDIO_AMR,           // File format".amr"

    MMIAUDIO_AAC,           //".aac"
    
    MMIAUDIO_M4A,			//".m4a" 也是aac的一种

    MMIAUDIO_TYPE_MAX       // Reserved, user can't use it.

} MMIAUDIO_TYPE_E;

typedef struct _AUD_RING_DATA_INFO_T
{
    MMIAUDIO_TYPE_E type;
    uint32          data_len;  
	uint8	      * data_ptr;
} MMIAUD_RING_DATA_INFO_T, * PAUD_RING_DATA_INFO_T;

#ifdef __cplusplus
}
#endif

#endif // _MMI_RING_DEF_H__