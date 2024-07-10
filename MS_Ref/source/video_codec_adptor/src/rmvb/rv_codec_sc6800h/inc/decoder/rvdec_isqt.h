

#ifndef _RVDEC_ISQT_H_
#define _RVDEC_ISQT_H_

#include "rvdec_mode.h"

PUBLIC void RvDec_IQT4x4_Intra16x16(int16 *pBuf, uint32 uQP0, uint32 uQP);
PUBLIC void RvDec_ITrans_MC_blk4x4(uint8 *pPred, int16 *pcoeff, int32 width);
PUBLIC void RvDec_ITrans_MC_blk4x4_onlyDC(uint8 *pPred, int32 DCCoeff);
PUBLIC void arm_mc_itrans4x4(uint8 *pPred, int16 *pcoeff, int32 width);
PUBLIC void rvdec_ITrans_MC_blk4x4_d (uint8 * pPred, int16 * pcoeff, int width);
PUBLIC void rvdec_ITrans_MC_blk4x4_onlyDC_d (uint8 * pPred, int DCCoeff);

#if defined(ITRANS_ASSEMBLY)
	#define	ITRANS_MC4X4	arm_mc_itrans4x4
#else
	#define	ITRANS_MC4X4	RvDec_ITrans_MC_blk4x4
#endif

#endif //_RVDEC_ISQT_H_
