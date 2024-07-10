
#ifndef _RVDEC_MV_H_
#define _RVDEC_MV_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "video_common.h"
#include "rvdec_basic.h"
#include "rvdec_mode.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

PUBLIC void RvDec_DecMV_P(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_ptr);
PUBLIC void RvDec_DecMV_B_RV8(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *pMV_f, MOTION_VECTOR_T *pMV_b);
PUBLIC void RvDec_DecMV_B_RV9(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *pMV_f, MOTION_VECTOR_T *pMV_b);
	
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_MV_H_
