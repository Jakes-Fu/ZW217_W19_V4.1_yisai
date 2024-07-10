
#ifndef _RVDEC_FRAME_H_
#define _RVDEC_FRAME_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "video_common.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

PUBLIC void RvDec_InitPicture(RV_DECODER_T *rv_decoder_ptr);
PUBLIC MMDecRet RvDec_DecoderFrameI(RV_DECODER_T *rv_decoder_ptr);
PUBLIC MMDecRet RvDec_DecoderFrameP(RV_DECODER_T *rv_decoder_ptr);
PUBLIC MMDecRet RvDec_DecoderFrameB(RV_DECODER_T *rv_decoder_ptr);
PUBLIC void RvDec_exit_picture(RV_DECODER_T *rv_decoder_ptr);

PUBLIC MMDecRet RvDec_DecoderFrameP_d(RV_DECODER_T *rv_decoder_ptr);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_FRAME_H_
