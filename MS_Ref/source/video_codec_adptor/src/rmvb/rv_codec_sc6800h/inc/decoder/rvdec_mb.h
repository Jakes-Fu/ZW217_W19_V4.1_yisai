
#ifndef  _RVDEC_MB_H_
#define  _RVDEC_MB_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_basic.h"
#include "rvdec_mode.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

PUBLIC void RvDec_DecIPMode4x4_RV8(BitStream *stream, MB_CACHE_T *mb_cache_ptr, int8 *pIPCache);
PUBLIC void RvDec_DecIPMode4x4_RV9(BitStream *stream, MB_CACHE_T *mb_cache_ptr, int8 *pIPCache);
PUBLIC void RvDec_ReadMbtype_PB_RV8(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr);
PUBLIC void RvDec_ReadMbtype_PB_RV9(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr);
PUBLIC void RvDec_start_macroblock(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_ptr);
PUBLIC void RvDec_read_macroblock_I(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, BitStream *stream);
PUBLIC void RvDec_read_macroblock_P(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_ptr, uint32 *pmvd);
PUBLIC void RvDec_read_macroblock_B(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint32 *mvd_ptr);
PUBLIC void RvDec_recon_intraMB(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3]);
PUBLIC void RvDec_recon_interMB_P(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr);
PUBLIC void RvDec_recon_interMB_B(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr);
PUBLIC void RvDec_WriteRecMB2Frame(RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3]);
PUBLIC void arm_put_mb2Frame(RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3]);

PUBLIC void RvDec_WriteRecMB2Frame_d(RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3]);
PUBLIC void put_bottom2Buf (RV_DECODER_T *pDecoder, MB_CACHE_T *pMBCache, uint32 uMbX);
PUBLIC void RvDec_recon_intraMB_d (RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3]);
PUBLIC void RvDec_recon_interMB_P_d(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr);
PUBLIC void copyLeftCol_d (RV_DECODER_T *rv_decoder_ptr, uint8 *ppxlcRecMBYUV[3]);

#if !defined(PUT_MB_ASSEMLY)
	#define PUT_MB2FRAME	RvDec_WriteRecMB2Frame
#else
        PUBLIC void arm_put_mb2Frame_rv(RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3]);

	#define PUT_MB2FRAME	arm_put_mb2Frame_rv
#endif

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_MB_H_
