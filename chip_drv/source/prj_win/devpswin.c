
#ifdef WIN32 
#include <stdio.h>
#include "sci_types.h"

typedef int int32_t;
typedef short int16_t;
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned long int   uint32_t;

uint32 REG_ACCESS_ADDRESS(uint32 addr)
{  
   return (addr);
}
BOOLEAN  GPIO_GetValue (uint32  gpio_id)
{
	return FALSE;
}
void  DSP_AACDecode(  uint16_t    *audio_stream_ptr,    // the input stream
					  int16_t      stream_size ,        // the input stream length
					  uint16_t    *out_pcm_data_ptr,    // output data 
					  uint16_t    *pcm_size_ptr,        // output data length
					  uint16_t    *result_ptr)          // return value
{

}
void  DSP_AACDecInit(uint16_t  *buffer_ptr,         // the input stream
					 uint32_t  *sample_rate_ptr,    // the audio sample rate
					 int16_t   *channel_count_ptr,  // channel count
					 int16_t   *flag_ptr)           // return the status
{

}

uint16 mp3_dsp_decode(void* hdec, uint8 *in_buf, uint8 *out_buf, uint16 pcm_interlace_flag)
{

}

void* mp3_dsp_open()
{

}

void mp3_dsp_close(void* hdec)
{

}


#if 0
int32 TextTibetanProc(void* buf, BOOLEAN all)
{

}

int32 TextKhmerProc(void* buf, BOOLEAN all)
{

}
#endif

#endif

