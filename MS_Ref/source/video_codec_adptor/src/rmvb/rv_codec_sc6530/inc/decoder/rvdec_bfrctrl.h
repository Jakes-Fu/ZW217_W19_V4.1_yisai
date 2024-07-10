

#ifndef _RVDEC_BFR_CTRL_H_
#define _RVDEC_BFR_CTRL_H_

#include "rvdec_mode.h"

PUBLIC MMDecRet RvDec_InitYUVBfr(RV_DECODER_T *rv_decoder_ptr);
PUBLIC BOOLEAN RvDec_GetCurRecFrameBfr(RV_DECODER_T *rv_decoder_ptr);
PUBLIC MMDecRet RV_DecReleaseDispBfr(uint8 *pBfrAddr);
PUBLIC void RvDec_SetRefFlag();
PUBLIC void RvDec_SetDspFlag();

#endif //_RVDEC_BFR_CTRL_H_
