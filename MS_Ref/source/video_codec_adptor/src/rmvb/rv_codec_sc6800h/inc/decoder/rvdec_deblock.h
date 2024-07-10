
#ifndef _RVDEC_DEBLOCK_H_
#define _RVDEC_DEBLOCK_H_

#include "rvdec_basic.h"
#include "rvdec_mode.h"



void rvdec_deblocking_frame (RV_DECODER_T *pDecoder);
void rvdec_deblocking_frame_r8 (RV_DECODER_T *pDecoder);
int32 set_deblock_coeff_r8 (RV_DECODER_T *pDecoder, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *mv_ptr, int32 mbtype);
int32 set_deblock_coeff_r9 (RV_DECODER_T *pDecoder, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *mv_ptr, int32 mbtype);

#endif //_RVDEC_DEBLOCK_H_
