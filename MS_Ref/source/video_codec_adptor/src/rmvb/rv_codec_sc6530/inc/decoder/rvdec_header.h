
#ifndef _RVDEC_HEADER_H_
#define _RVDEC_HEADER_H_

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

PUBLIC MMDecRet RvDec_GetSliceHeader(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, uint32 isPicHeader);
PUBLIC BOOLEAN RvDec_GetSSC_RV8(RV_DECODER_T *rv_decoder_ptr, BitStream *stream);
PUBLIC BOOLEAN RvDec_GetSSC_RV9(RV_DECODER_T *rv_decoder_ptr, BitStream *stream);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_HEADER_H_
