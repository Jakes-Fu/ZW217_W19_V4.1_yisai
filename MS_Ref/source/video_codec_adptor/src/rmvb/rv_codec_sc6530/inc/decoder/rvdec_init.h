
#ifndef _RVDEC_INIT_H_
#define _RVDEC_INIT_H_

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

PUBLIC void RvDec_InitDecoderPara(RV_DECODER_T *rv_decoder_ptr);
PUBLIC void RvDec_SetClipTab(void);
PUBLIC MMDecRet RvDec_InitSessionDecode(RV_DECODER_T *rv_decoder_ptr);
PUBLIC void RvDec_init_ROM_TABLE();

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_INIT_H_
