#ifndef __TRACK_MIXER_H
#define __TRACK_MIXER_H

#include "audio_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define SHR(a, shift) ((a) >> (shift))
#define ADD32(a, b) ((int32)(a) + (int32)(b))
#define MULT16_16(a, b) (((int32)(int16)(a)) * ((int32)(int16)(b)))
#define MULT16_16_P14(a, b) (SHR(ADD32(8192, MULT16_16((a), (b))), 14))

PUBLIC void _Track_Mixer(ADM_TRACK_T *track_ptr,
                  int32 *mix_buf_left,
                  int32 *mix_buf_right,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned samples_per_block,
                  unsigned num_of_blocks,
                  int downmix,
                  int is_mute,
                  int dg_level);

PUBLIC void _Read_SingleTrack(ADM_TRACK_T *track_ptr,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned sample_count,
                  BOOLEAN is_mute);
PUBLIC BOOLEAN _Track_Mixer_End(ADM_TRACK_T *track_ptr);

PUBLIC uint32 _GetDataCount_SingleTrack(ADM_TRACK_T *track_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of track_mixer.h

#endif //__TRACK_MIXER_H
