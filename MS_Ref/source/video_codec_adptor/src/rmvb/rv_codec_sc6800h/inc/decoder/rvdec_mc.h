
#ifndef _RVDEC_MC_H_
#define _RVDEC_MC_H_

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

#define MV_DIV_4(mv, mv_int, mv_mod4) \
{ \
	(mv_int) += ((mv) >> 2);\
	(mv_mod4) = ((mv) & 0x3);\
}

#if defined(RV8_SUPPORTED)
#define MV_DIV_3(mv, mv_int, mv_mod3) \
{ \
	if(mv)\
	{\
		if ((mv) < 0) \
			(mv_int) += (((mv)*21845)>>16);	\
		else \
			(mv_int) += (((mv)*21846)>>16);	\
		(mv_mod3) = ((mv)-(mv_int)*3);\
	}else\
	{\
		mv_mod3 = 0;\
	}\
}

#define GET_MC_POS(mv, mv_int, mv_mod)\
{\
	if (rv_decoder_ptr->bIsRV8)\
	{\
		MV_DIV_3(mv, mv_int, mv_mod)\
	}else\
		MV_DIV_4(mv, mv_int, mv_mod)\
}
#else
#define GET_MC_POS(mv, mv_int, mv_mod)\
{\
	MV_DIV_4(mv, mv_int, mv_mod)\
}
#endif

#if !defined(_ARM_MC_ASSEMBLY_)
	#define DECLARE_MC_FUN(size,x,y) \
		void RvDec_MC_Luma##size##_H##x##V##y##_R8(uint8 * pRef, uint8 * pPred, uint32 srcPitch);\
		void RvDec_MC_Luma##size##_H##x##V##y##_R9(uint8 * pRef, uint8 * pPred, uint32 srcPitch);
#else
	#define DECLARE_MC_FUN(size,x,y) \
		void arm_mc_luma##size##_dx##x##dy##y##_rv8(uint8 * pRef, uint8 * pPred, uint32 srcPitch);\
		void arm_mc_luma##size##_dx##x##dy##y##_rv9(uint8 * pRef, uint8 * pPred, uint32 srcPitch);
#endif	

	DECLARE_MC_FUN(16x16,0,0)	
	DECLARE_MC_FUN(16x16,0,1)	DECLARE_MC_FUN(16x16,0,2)	DECLARE_MC_FUN(16x16,0,3)
	DECLARE_MC_FUN(16x16,1,0)	DECLARE_MC_FUN(16x16,1,1)	DECLARE_MC_FUN(16x16,1,2)	DECLARE_MC_FUN(16x16,1,3)
	DECLARE_MC_FUN(16x16,2,0)	DECLARE_MC_FUN(16x16,2,1)	DECLARE_MC_FUN(16x16,2,2)	DECLARE_MC_FUN(16x16,2,3)
	DECLARE_MC_FUN(16x16,3,0)	DECLARE_MC_FUN(16x16,3,1)	DECLARE_MC_FUN(16x16,3,2)	DECLARE_MC_FUN(16x16,3,3)

	DECLARE_MC_FUN(8x8,0,0)	DECLARE_MC_FUN(8x8,0,1)	DECLARE_MC_FUN(8x8,0,2)	DECLARE_MC_FUN(8x8,0,3)
	DECLARE_MC_FUN(8x8,1,0)	DECLARE_MC_FUN(8x8,1,1)	DECLARE_MC_FUN(8x8,1,2)	DECLARE_MC_FUN(8x8,1,3)
	DECLARE_MC_FUN(8x8,2,0)	DECLARE_MC_FUN(8x8,2,1)	DECLARE_MC_FUN(8x8,2,2)	DECLARE_MC_FUN(8x8,2,3)
	DECLARE_MC_FUN(8x8,3,0)	DECLARE_MC_FUN(8x8,3,1)	DECLARE_MC_FUN(8x8,3,2)	DECLARE_MC_FUN(8x8,3,3)
#undef DECLARE_MC_FUN
	
#if !defined(_ARM_MC_ASSEMBLY_)
	#define DECLARE_MC_FUN(size,x,y) \
		void RvDec_MC_Chroma##size##_H##x##V##y##_R8(uint8 * pRef, uint8 * pPred, uint32 srcPitch);\
		void RvDec_MC_Chroma##size##_H##x##V##y##_R9(uint8 * pRef, uint8 * pPred, uint32 srcPitch);
#else
	#define DECLARE_MC_FUN(size,x,y) \
		void arm_mc_chroma##size##_dx##x##dy##y##_rv8(uint8 * pRef, uint8 * pPred, uint32 srcPitch);\
		void arm_mc_chroma##size##_dx##x##dy##y##_rv9(uint8 * pRef, uint8 * pPred, uint32 srcPitch);
#endif	

	DECLARE_MC_FUN(8x8,0,0)	DECLARE_MC_FUN(8x8,0,1)	DECLARE_MC_FUN(8x8,0,2)	DECLARE_MC_FUN(8x8,0,3)
	DECLARE_MC_FUN(8x8,1,0)	DECLARE_MC_FUN(8x8,1,1)	DECLARE_MC_FUN(8x8,1,2)	DECLARE_MC_FUN(8x8,1,3)
	DECLARE_MC_FUN(8x8,2,0)	DECLARE_MC_FUN(8x8,2,1)	DECLARE_MC_FUN(8x8,2,2)	DECLARE_MC_FUN(8x8,2,3)
	DECLARE_MC_FUN(8x8,3,0)	DECLARE_MC_FUN(8x8,3,1)	DECLARE_MC_FUN(8x8,3,2)	DECLARE_MC_FUN(8x8,3,3)

	DECLARE_MC_FUN(4x4,0,0)	DECLARE_MC_FUN(4x4,0,1)	DECLARE_MC_FUN(4x4,0,2)	DECLARE_MC_FUN(4x4,0,3)
	DECLARE_MC_FUN(4x4,1,0)	DECLARE_MC_FUN(4x4,1,1)	DECLARE_MC_FUN(4x4,1,2)	DECLARE_MC_FUN(4x4,1,3)
	DECLARE_MC_FUN(4x4,2,0)	DECLARE_MC_FUN(4x4,2,1)	DECLARE_MC_FUN(4x4,2,2)	DECLARE_MC_FUN(4x4,2,3)
	DECLARE_MC_FUN(4x4,3,0)	DECLARE_MC_FUN(4x4,3,1)	DECLARE_MC_FUN(4x4,3,2)	DECLARE_MC_FUN(4x4,3,3)
#undef DECLARE_MC_FUN

//for downsize
#if defined(_ARM_MC_ASSEMBLY_)
//void arm_mc_chroma4x4_dx0dy0_rv9 (uint8 * pRefBlk, uint8 * pPred, int32 width);
void arm_mc_luma4x4_xfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dy);
void arm_mc_luma4x4_yfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dx, int32 line);
void arm_mc_luma8x8_xfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dy);
void arm_mc_luma8x8_yfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dx, int32 line);

void arm_mc_chroma2x2_xfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dy);
void arm_mc_chroma2x2_yfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dx);
void arm_mc_chroma2x2_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dx, int32 dy);

void arm_mc_chroma4x4_dx0dy0_d (uint8 * pRefBlk, uint8 * pPred, int32 width);
void arm_mc_chroma4x4_xfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dy);
void arm_mc_chroma4x4_yfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dx);
void arm_mc_chroma4x4_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int32 dx, int32 dy);
#endif

PUBLIC void RvDec_InitMCFun_RV8(void);
PUBLIC void RvDec_InitMCFun_RV9(void);
PUBLIC void RvDec_InitMCFun(BOOLEAN bReal8);

PUBLIC void RvDec_MC_direct_MB(RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *pMbCache);
PUBLIC void RvDec_MC_oneDirection_MB(RV_DECODER_T *rv_decoder_ptr, uint8 **pRefFrame, MB_PIXEL_T *pMBPix, int16 *pMVCache);	
PUBLIC void RvDec_Weighted_Average_r0EQr1(RV_DECODER_T *rv_decoder_ptr, MB_PIXEL_T *pmbpix, MB_PIXEL_T *pmbpix_b);
PUBLIC void RvDec_Weighted_Average_r0NEr1(RV_DECODER_T *rv_decoder_ptr, MB_PIXEL_T *pmbpix, MB_PIXEL_T *pmbpix_b);
PUBLIC void copyRefToRecFrm (RV_DECODER_T *rv_decoder_ptr, uint8 *ppxlcRecMBYUV[3]);
PUBLIC void copyRefToRecFrm_d (RV_DECODER_T *rv_decoder_ptr, uint8 *ppxlcRecMBYUV[3]);

PUBLIC void MC_luma4x4_xmyn (uint8 * pRefFrame, uint8 * pPred, int32 xpos, int32 ypos, int width);
PUBLIC void MC_luma8x8_xmyn (uint8 * pRefFrame, uint8 * pPred, int32 xpos, int32 ypos, int width);
PUBLIC void PC_MC_chroma2x2_rv (uint8 * pRefFrame, uint8 * pPredUV, int32 xpos, int32 ypos, int width);
PUBLIC void PC_MC_chroma4x4_rv (uint8 * pRefFrame, uint8 * pPredUV, int32 xpos, int32 ypos, int width);

#define COPY_REFMB2FRAME	copyRefToRecFrm
#define COPY_REFMB2FRAME_D	copyRefToRecFrm_d

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_MC_H_
