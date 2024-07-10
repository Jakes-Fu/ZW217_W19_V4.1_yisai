
#include "ms_ref_rv_codec_trc.h"
#include "rvdec_video_header.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*//////////////////////////////////////////////////////// */
/* Mapping from luma QP to chroma QP */
const uint8 s_chroma_quant[32] = 
{
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,17,18,
	 19,20,20,21,22,22,23,23,24,24,25,25
};

const uint8 s_chroma_quant_DC[32] = 
{
	0,0,0,1,2,3,4,5,6,7, 8, 9,10,11,12,13,14,15,15,16,
	 17,18,18,19,20,20,21,21,22,22,23,23
};

PUBLIC void RvDec_InitPicture(RV_DECODER_T *rv_decoder_ptr)
{
	rv_decoder_ptr->uSilceNumber = 0;
	rv_decoder_ptr->uMbNumDec = 0;
	rv_decoder_ptr->iSkipModesLeft = 0;

	if (rv_decoder_ptr->need_downsize)
	{
		rv_decoder_ptr->pCurRecFrame->is_downsized = 1;

		if (INTRAPIC == rv_decoder_ptr->uPicCodeType)
		{
			uint8 **src, **dst, *tmp;

 			src = &(rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[0]);
 			dst = &((g_RvFrmYUVBfr+YUV_BUFFER_NUM)->imgYUV[0]);

			tmp = src[0];src[0] = dst[0];dst[0] = tmp;
			tmp = src[1];src[1] = dst[1];dst[1] = tmp;
			tmp = src[2];src[2] = dst[2];dst[2] = tmp;
		}
	}else
	{
		rv_decoder_ptr->pCurRecFrame->is_downsized = 0;
	}
	rv_decoder_ptr->pCurRecFrame->time = rv_decoder_ptr->uTR;
		
	//if B frame, compute iRatio0 and iRatio1
#if defined(BSLICE_SUPPORTED)
	if (TRUEBPIC == rv_decoder_ptr->uPicCodeType)
	{
		int trb, trd;
		
		trd = rv_decoder_ptr->pBckRefFrame->time - rv_decoder_ptr->pFrdRefFrame->time;
		if (trd < 0)
		{
			trd = trd + 256;	
		}
		
		trb = rv_decoder_ptr->pCurRecFrame->time  - rv_decoder_ptr->pFrdRefFrame->time;
		if (trb < 0)
		{
			trb = trb + 256;
		}
		
		rv_decoder_ptr->iRatio0 = (trb << 14) / trd;
		rv_decoder_ptr->iRatio1 = ((trd - trb) << 14) / trd;

		if (rv_decoder_ptr->iRatio0 == rv_decoder_ptr->iRatio1)
		{
			g_RvDec_Get_Weighted_Average = RvDec_Weighted_Average_r0EQr1;
		}else
		{
			g_RvDec_Get_Weighted_Average = RvDec_Weighted_Average_r0NEr1;
		}
	}
#endif

	{
		MB_CACHE_T *mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
		int32 qp = rv_decoder_ptr->uPQuant;
		
		mb_cache_ptr->luma_qp = g_BquantTable[qp];	
		mb_cache_ptr->chroma_dc_qp = g_BquantTable [s_chroma_quant_DC [qp]];
		mb_cache_ptr->chroma_ac_qp  = g_BquantTable [s_chroma_quant   [qp]];
	}
}

//downsize
#if 1
PUBLIC void RvDec_downsize_frame_i (STORABLE_PICTURE_T *pCurrentPic,RV_DECODER_T *rv_decoder_ptr)
#if  !defined(_ARM_DOWNSIZE_)
{
	DEC_FRM_BFR *Frm_d = g_RvFrmYUVBfr+YUV_BUFFER_NUM;
	DEC_FRM_BFR *pDecFrame = pCurrentPic->pDecFrame;
	int32 dst_width_mb = rv_decoder_ptr->uMbNumX;
	int32 dst_height = rv_decoder_ptr->uFrameHeight_d;
	int32 src_offset = rv_decoder_ptr->uStartInFrameY;
	int32 dst_offset = rv_decoder_ptr->uStartInFrameY_d;
	int32 src_ext_width = rv_decoder_ptr->uFrameExtendWidth;
	uint8 *src, *dst, *src1, *dst1;
	int32 i, j;

	src = Frm_d->imgYUV[0] + src_offset;
	dst = pDecFrame->imgYUV[0] + dst_offset;

#ifdef WIN32
        SCI_TraceLow("RvDec_downsize_frame_i");
#else
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,RVDEC_FRAME_109_112_2_10_17_27_7_1,(uint8*)"");
#endif
	for (i = dst_height; i > 0 ; i--)
	{
		for (j = dst_width_mb; j > 0 ; j--)
		{
			dst[0] = src[0]; dst[1] = src[2];	dst[2] = src[4];	dst[3] = src[6];
			dst[4] = src[8];dst[5] = src[10];	dst[6] = src[12];	dst[7] = src[14];			

			src += 16;	dst += 8;	
		}
		dst += (RV_Y_EXTEND_SIZE);
		src += (src_ext_width + (RV_Y_EXTEND_SIZE <<1));
	}

	src_offset = rv_decoder_ptr->uStartInFrameUV;
	dst_offset = rv_decoder_ptr->uStartInFrameUV_d;

	//u, v
	src = Frm_d->imgYUV[1] + src_offset;
	src1 = Frm_d->imgYUV[2] + src_offset;
	dst = pDecFrame->imgYUV[1] + dst_offset;
	dst1 = pDecFrame->imgYUV[2] + dst_offset;
		
	for (i = dst_height/2; i >0 ; i--)
	{
		for (j = dst_width_mb; j >0 ; j--)
		{
			dst[0] = src[0];	dst[1] = src[2];	dst[2] = src[4];	dst[3] = src[6];	src += 8; 	dst += 4;	
			dst1[0] = src1[0];	dst1[1] = src1[2];	dst1[2] = src1[4];	dst1[3] = src1[6];	src1 += 8; 	dst1 += 4;	
		}
		dst += (RV_UV_EXTEND_SIZE);	src += ((src_ext_width + (RV_Y_EXTEND_SIZE <<1))/2);
		dst1 += (RV_UV_EXTEND_SIZE);	src1 += ((src_ext_width + (RV_Y_EXTEND_SIZE <<1))/2);
	}
	
	return;
}
#else
;
#endif
#endif

PUBLIC MMDecRet RvDec_DecoderFrameI(RV_DECODER_T *rv_decoder_ptr)
{
	uint32 uMbX, uMbY;
	MB_MODE_T *mb_mode_ptr = rv_decoder_ptr->mb_mode_ptr;
	MB_CACHE_T *mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	BitStream *stream = rv_decoder_ptr->stream;
	uint8 *ppxlcRecGobYUV[3], *ppxlcRecMBYUV[3];
	int32 RecGobYOffset;

	RecGobYOffset = rv_decoder_ptr->uFrameExtendWidth << 4;
	ppxlcRecGobYUV[0] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[0] + rv_decoder_ptr->uStartInFrameY;
	ppxlcRecGobYUV[1] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[1] + rv_decoder_ptr->uStartInFrameUV;
	ppxlcRecGobYUV[2] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[2] + rv_decoder_ptr->uStartInFrameUV;

	for(uMbY = 0; uMbY < rv_decoder_ptr->uMbNumY; uMbY++)
	{			
		rv_decoder_ptr->uMbPosY = uMbY;
					
		ppxlcRecMBYUV[0] = ppxlcRecGobYUV[0];
		ppxlcRecMBYUV[1] = ppxlcRecGobYUV[1];
		ppxlcRecMBYUV[2] = ppxlcRecGobYUV[2];

		for(uMbX = 0; uMbX < rv_decoder_ptr->uMbNumX; uMbX++)
		{
			rv_decoder_ptr->uMbPosX = uMbX;
#if _DEBUG_
			if((uMbX == 18) && (uMbY  == 1) && (g_nFrame_rv == 0))
			{
				foo();
			}
#endif

			if (g_RvDec_Get_Slice_StartCode(rv_decoder_ptr, stream))
			{
				RvDec_GetSliceHeader(rv_decoder_ptr, stream, 0);
				rv_decoder_ptr->uSilceNumber++;
				rv_decoder_ptr->iSkipModesLeft = 0;
			}	
			
			RvDec_start_macroblock(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, NULL/*rv_decoder_ptr->pMVFrameP+rv_decoder_ptr->uMbNumDec*4*/);
			RvDec_read_macroblock_I(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, stream);		
			RvDec_recon_intraMB(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, ppxlcRecMBYUV);
			PUT_MB2FRAME(rv_decoder_ptr, mb_cache_ptr, ppxlcRecMBYUV);

			rv_decoder_ptr->uMbNumDec++;
			mb_mode_ptr++;

			ppxlcRecMBYUV[0] += MB_SIZE;
			ppxlcRecMBYUV[1] += BLOCK_SIZE;
			ppxlcRecMBYUV[2] += BLOCK_SIZE;	
		}

		ppxlcRecGobYUV[0] += RecGobYOffset;
		ppxlcRecGobYUV[1] += (RecGobYOffset>>2);
		ppxlcRecGobYUV[2] += (RecGobYOffset>>2);
	}

	if (rv_decoder_ptr->need_downsize)
	{
		uint8 **src, **dst, *tmp;

 		src = &(rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[0]);
 		dst = &((g_RvFrmYUVBfr+YUV_BUFFER_NUM)->imgYUV[0]);

		tmp = src[0];src[0] = dst[0];dst[0] = tmp;
		tmp = src[1];src[1] = dst[1];dst[1] = tmp;
		tmp = src[2];src[2] = dst[2];dst[2] = tmp;

		RvDec_downsize_frame_i(rv_decoder_ptr->pCurRecFrame,rv_decoder_ptr);
	}
	return MMDEC_OK;
}

PUBLIC MMDecRet RvDec_DecoderFrameP(RV_DECODER_T *rv_decoder_ptr)
{
	uint32 uMbX, uMbY;
	uint32 *mvd_ptr = rv_decoder_ptr->mvd_ptr;
	MB_MODE_T *mb_mode_ptr = rv_decoder_ptr->mb_mode_ptr;
	MB_CACHE_T *mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	MOTION_VECTOR_T *mv_ptr = rv_decoder_ptr->pMVFrameP;
	BitStream *stream = rv_decoder_ptr->stream;
	uint8 *ppxlcRecGobYUV[3], *ppxlcRecMBYUV[3];
	int32 RecGobYOffset;

	RecGobYOffset = rv_decoder_ptr->uFrameExtendWidth << 4;
	ppxlcRecGobYUV[0] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[0] + rv_decoder_ptr->uStartInFrameY;
	ppxlcRecGobYUV[1] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[1] + rv_decoder_ptr->uStartInFrameUV;
	ppxlcRecGobYUV[2] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[2] + rv_decoder_ptr->uStartInFrameUV;

	for(uMbY = 0; uMbY < rv_decoder_ptr->uMbNumY; uMbY++)
	{			
		rv_decoder_ptr->uMbPosY = uMbY;
					
		ppxlcRecMBYUV[0] = ppxlcRecGobYUV[0];
		ppxlcRecMBYUV[1] = ppxlcRecGobYUV[1];
		ppxlcRecMBYUV[2] = ppxlcRecGobYUV[2];

		for(uMbX = 0; uMbX < rv_decoder_ptr->uMbNumX; uMbX++)
		{ 
			rv_decoder_ptr->uMbPosX = uMbX;
#if _DEBUG_
			if((uMbX == 14) && (uMbY  == 15) && (g_nFrame_rv == 0x2))
			{
				foo();
				//exit(-1);
			}
			
			if((rv_decoder_ptr->uMbNumDec  == 0) && (g_nFrame_rv == 13))
			{
				foo();
				//exit(-1);
			}	
#endif

			if (g_RvDec_Get_Slice_StartCode(rv_decoder_ptr, stream))
			{
				RvDec_GetSliceHeader(rv_decoder_ptr, stream, 0);
				rv_decoder_ptr->uSilceNumber++;
				rv_decoder_ptr->iSkipModesLeft = 0;
			}

			RvDec_start_macroblock(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, mv_ptr);
			RvDec_read_macroblock_P(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, mv_ptr, mvd_ptr);

			if(MBTYPE_SKIPPED != mb_mode_ptr->uMbType)
			{
				if(!mb_cache_ptr->bIsIntra)
				{
					RvDec_recon_interMB_P(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);
				}else
				{
			 		RvDec_recon_intraMB (rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, ppxlcRecMBYUV);
				}
				PUT_MB2FRAME(rv_decoder_ptr, mb_cache_ptr, ppxlcRecMBYUV);	
			}else
			{
				COPY_REFMB2FRAME (rv_decoder_ptr, ppxlcRecMBYUV);		
			}
			
			mv_ptr += 4;
			mvd_ptr++;
			mb_mode_ptr++;
			rv_decoder_ptr->uMbNumDec++;

			ppxlcRecMBYUV[0] += MB_SIZE;
			ppxlcRecMBYUV[1] += BLOCK_SIZE;
			ppxlcRecMBYUV[2] += BLOCK_SIZE;			
		}
		
		ppxlcRecGobYUV[0] += RecGobYOffset;
		ppxlcRecGobYUV[1] += (RecGobYOffset>>2);
		ppxlcRecGobYUV[2] += (RecGobYOffset>>2);
	}

	return MMDEC_OK;
}

PUBLIC MMDecRet RvDec_DecoderFrameP_d(RV_DECODER_T *rv_decoder_ptr)
{
	uint32 uMbX, uMbY;
	uint32 *mvd_ptr = rv_decoder_ptr->mvd_ptr;
	MB_MODE_T *mb_mode_ptr = rv_decoder_ptr->mb_mode_ptr;
	MB_CACHE_T *mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	MOTION_VECTOR_T *mv_ptr = rv_decoder_ptr->pMVFrameP;
	BitStream *stream = rv_decoder_ptr->stream;
	uint8 *ppxlcRecGobYUV[3], *ppxlcRecMBYUV[3];
	int32 RecGobYOffset;
	
	RecGobYOffset = rv_decoder_ptr->uFrameExtendWidth << 2;
	ppxlcRecGobYUV[0] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[0] + rv_decoder_ptr->uStartInFrameY_d;
	ppxlcRecGobYUV[1] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[1] + rv_decoder_ptr->uStartInFrameUV_d;
	ppxlcRecGobYUV[2] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[2] + rv_decoder_ptr->uStartInFrameUV_d;

	for(uMbY = 0; uMbY < rv_decoder_ptr->uMbNumY; uMbY++)
	{			
		rv_decoder_ptr->uMbPosY = uMbY;
					
		ppxlcRecMBYUV[0] = ppxlcRecGobYUV[0];
		ppxlcRecMBYUV[1] = ppxlcRecGobYUV[1];
		ppxlcRecMBYUV[2] = ppxlcRecGobYUV[2];

		for(uMbX = 0; uMbX < rv_decoder_ptr->uMbNumX; uMbX++)
		{ 
			rv_decoder_ptr->uMbPosX = uMbX;
#if _DEBUG_
			if((uMbX == 12) && (uMbY  == 1) && (g_nFrame_rv == 1))
			{
				foo();
				//exit(-1);
			}
			
			if((rv_decoder_ptr->uMbNumDec  == 0) && (g_nFrame_rv == 13))
			{
				foo();
				//exit(-1);
			}	
#endif

			if (g_RvDec_Get_Slice_StartCode(rv_decoder_ptr, stream))
			{
				RvDec_GetSliceHeader(rv_decoder_ptr, stream, 0);
				rv_decoder_ptr->uSilceNumber++;
				rv_decoder_ptr->iSkipModesLeft = 0;
			}

			RvDec_start_macroblock(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, mv_ptr);
			RvDec_read_macroblock_P(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, mv_ptr, mvd_ptr);

			if(MBTYPE_SKIPPED != mb_mode_ptr->uMbType)
			{
				if(!mb_cache_ptr->bIsIntra)
				{
					RvDec_recon_interMB_P_d(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);
				}else
				{
			 		RvDec_recon_intraMB_d (rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, ppxlcRecMBYUV);
				}
				RvDec_WriteRecMB2Frame_d(rv_decoder_ptr, mb_cache_ptr, ppxlcRecMBYUV);	
			}else
			{
				COPY_REFMB2FRAME_D (rv_decoder_ptr, ppxlcRecMBYUV);
			}
				
			put_bottom2Buf(rv_decoder_ptr, mb_cache_ptr, uMbX);	
			
			mv_ptr += 4;
			mvd_ptr++;
			mb_mode_ptr++;
			rv_decoder_ptr->uMbNumDec++;
			
			ppxlcRecMBYUV[0] += (MB_SIZE/2);
			ppxlcRecMBYUV[1] += (BLOCK_SIZE/2);
			ppxlcRecMBYUV[2] += (BLOCK_SIZE/2);			
		}

		ppxlcRecGobYUV[0] += RecGobYOffset;
		ppxlcRecGobYUV[1] += (RecGobYOffset>>2);
		ppxlcRecGobYUV[2] += (RecGobYOffset>>2);
	}

	return MMDEC_OK;
}

#if defined(BSLICE_SUPPORTED)
PUBLIC MMDecRet RvDec_DecoderFrameB(RV_DECODER_T *rv_decoder_ptr)
{
	uint32 uMbX, uMbY;
	uint32 *mvd_ptr = rv_decoder_ptr->mvd_ptr;
	MB_MODE_T *mb_mode_ptr = rv_decoder_ptr->mb_mode_b_ptr;
	MB_CACHE_T *mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	MOTION_VECTOR_T *fmv_ptr = rv_decoder_ptr->pMVFrameB_f;
	BitStream *stream = rv_decoder_ptr->stream;
	uint8 *ppxlcRecGobYUV[3], *ppxlcRecMBYUV[3];
	int32 RecGobYOffset;

	RecGobYOffset = rv_decoder_ptr->uFrameExtendWidth << 4;
	ppxlcRecGobYUV[0] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[0] + rv_decoder_ptr->uStartInFrameY;
	ppxlcRecGobYUV[1] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[1] + rv_decoder_ptr->uStartInFrameUV;
	ppxlcRecGobYUV[2] = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV[2] + rv_decoder_ptr->uStartInFrameUV;

	for(uMbY = 0; uMbY < rv_decoder_ptr->uMbNumY; uMbY++)
	{			
		rv_decoder_ptr->uMbPosY = uMbY;
					
		ppxlcRecMBYUV[0] = ppxlcRecGobYUV[0];
		ppxlcRecMBYUV[1] = ppxlcRecGobYUV[1];
		ppxlcRecMBYUV[2] = ppxlcRecGobYUV[2];

		for(uMbX = 0; uMbX < rv_decoder_ptr->uMbNumX; uMbX++)
		{ 	
			rv_decoder_ptr->uMbPosX = uMbX;
#if _DEBUG_
			if((uMbX == 12) && (uMbY  == 0) && (g_nFrame_rv == 2))
			{
				foo();
				//exit(-1);
			}
			
			if((rv_decoder_ptr->uMbNumDec == 0x02) && (g_nFrame_rv == 3))
			{
				foo();
			//	exit(-1);
			}
#endif

			if (g_RvDec_Get_Slice_StartCode(rv_decoder_ptr, stream))
			{
				RvDec_GetSliceHeader(rv_decoder_ptr, stream, 0);
				rv_decoder_ptr->uSilceNumber++;
				rv_decoder_ptr->iSkipModesLeft = 0;
			}

			RvDec_start_macroblock(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, fmv_ptr);
		 	RvDec_read_macroblock_B(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, mvd_ptr);

			if(mb_cache_ptr->bIsIntra)
			{
				RvDec_recon_intraMB (rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, ppxlcRecMBYUV);
			}else
			{
		 		RvDec_recon_interMB_B(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);
			}
			PUT_MB2FRAME(rv_decoder_ptr, mb_cache_ptr, ppxlcRecMBYUV);

			fmv_ptr += 4;
			mvd_ptr++;
			mb_mode_ptr++;
			rv_decoder_ptr->uMbNumDec++;

			ppxlcRecMBYUV[0] += MB_SIZE;
			ppxlcRecMBYUV[1] += BLOCK_SIZE;
			ppxlcRecMBYUV[2] += BLOCK_SIZE;	
		}
		
		ppxlcRecGobYUV[0] += RecGobYOffset;
		ppxlcRecGobYUV[1] += (RecGobYOffset>>2);
		ppxlcRecGobYUV[2] += (RecGobYOffset>>2);
	}

	return MMDEC_OK;
}
#endif

/*
Y extent 16 pixel
UV extent 8 pixel
*/
PUBLIC void RvDec_ExtendFrame(RV_DECODER_T *rv_decoder_ptr)
{
	int32 i;
	int32 height, width, offset, extendWidth, copyWidth;
	uint8 *pSrc1, *pSrc2, *pDst1, *pDst2;
	uint8 **Frame = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV;

#if !defined(ASM_HOR_EXTENSION)
	height      = rv_decoder_ptr->uFrameHeight;
	width       = rv_decoder_ptr->uFrameWidth;
	extendWidth = rv_decoder_ptr->uFrameExtendWidth;
	offset      = rv_decoder_ptr->uStartInFrameY;

	pSrc1 = Frame[0] + offset;
	pDst1 = pSrc1 - RV_Y_EXTEND_SIZE;
	pSrc2 = pSrc1 + width - 1;
	pDst2 = pSrc2 + 1;
	copyWidth = RV_Y_EXTEND_SIZE;	

	/*horizontal repeat Y*/
	for(i = 0; i < height; i++)
	{
		int32 intValue = ((int32)(*pSrc1) << 24) | ((int32)(*pSrc1) << 16) | ((int32)(*pSrc1) << 8) | (int32)(*pSrc1);
		int32 * pIntDst = (int32 *)pDst1;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;
		pIntDst[2] = intValue;
		pIntDst[3] = intValue;

		intValue = ((int32)(*pSrc2) << 24) | ((int32)(*pSrc2) << 16) | ((int32)(*pSrc2) << 8) | (int32)(*pSrc2);
		pIntDst = (int32 *)pDst2;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;
		pIntDst[2] = intValue;
		pIntDst[3] = intValue;

		pSrc1 += extendWidth;
		pDst1 += extendWidth;
		pSrc2 += extendWidth;
		pDst2 += extendWidth;
	}

	/*horizontal repeat U*/
	extendWidth = extendWidth / 2;
	offset      = rv_decoder_ptr->uStartInFrameUV;
	pSrc1       = Frame [1] + offset;
	pDst1       = pSrc1 - RV_UV_EXTEND_SIZE;
	pSrc2 = pSrc1 + width / 2 - 1;
	pDst2 = pSrc2 + 1;
	copyWidth   = RV_UV_EXTEND_SIZE;
	height = height / 2;
	
	for(i = 0; i < height; i++)
	{
		int32 intValue = ((int32)(*pSrc1) << 24) | ((int32)(*pSrc1) << 16) | ((int32)(*pSrc1) << 8) | (int32)(*pSrc1);
		int32 *pIntDst = (int32 *)pDst1;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;
	
		intValue = ((int32)(*pSrc2) << 24) | ((int32)(*pSrc2) << 16) | ((int32)(*pSrc2) << 8) | (int32)(*pSrc2);
		pIntDst = (int32 *)pDst2;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;

		pSrc1 += extendWidth;
		pDst1 += extendWidth;
		pSrc2 += extendWidth;
		pDst2 += extendWidth;
	}

	/*horizontal repeat V*/
	pSrc1 = Frame [2] + offset;
	pDst1 = pSrc1 - RV_UV_EXTEND_SIZE;
	pSrc2 = pSrc1 + width / 2 - 1;
	pDst2 = pSrc2 + 1;
	for (i = 0; i < height; i++)
	{
		int32 intValue = ((int32)(*pSrc1) << 24) | ((int32)(*pSrc1) << 16) | ((int32)(*pSrc1) << 8) | (int32)(*pSrc1);
		int32 * pIntDst = (int32 *)pDst1;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;	
		
		intValue = ((int32)(*pSrc2) << 24) | ((int32)(*pSrc2) << 16) | ((int32)(*pSrc2) << 8) | (int32)(*pSrc2);
		pIntDst = (int32 *)pDst2;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;
		
		pSrc1 += extendWidth;
		pDst1 += extendWidth;
		pSrc2 += extendWidth;
		pDst2 += extendWidth;	
	}

#else
	asm_horExtendYUV(Frame, rv_decoder_ptr->uStartInFrameY, rv_decoder_ptr->uStartInFrameUV, 
		rv_decoder_ptr->uFrameExtendWidth, rv_decoder_ptr->uFrameHeight);
#endif

	/*copy first row and last row*/
	/*vertical repeat Y*/
	height = rv_decoder_ptr->uFrameHeight;
	extendWidth  = rv_decoder_ptr->uFrameExtendWidth;
	offset = extendWidth * RV_Y_EXTEND_SIZE;
	pSrc1  = Frame[0] + offset;
	pDst1  = Frame[0];
	pSrc2  = pSrc1 + extendWidth * (height - 1);
	pDst2  = pSrc2 + extendWidth;

	for(i = 0; i < RV_Y_EXTEND_SIZE; i++)
	{
		memcpy(pDst1, pSrc1, extendWidth);
		memcpy(pDst2, pSrc2, extendWidth);
		pDst1 += extendWidth;
		pDst2 += extendWidth;
	}

    /*vertical repeat U*/
    height = height / 2;
	extendWidth  = extendWidth / 2;
	offset = offset / 4;
	pSrc1  = Frame[1] + offset;
	pDst1  = Frame[1];
	pSrc2  = pSrc1 + extendWidth * (height - 1);
	pDst2  = pSrc2 + extendWidth;

	for(i = 0; i < RV_UV_EXTEND_SIZE; i++)
	{
		memcpy (pDst1, pSrc1, extendWidth);
		memcpy (pDst2, pSrc2, extendWidth);
		pDst1 += extendWidth;
		pDst2 += extendWidth;
	}

	/*vertical repeat U*/
	pSrc1  = Frame[2] + offset;
	pDst1  = Frame[2];
	pSrc2  = pSrc1 + extendWidth * (height - 1);
	pDst2  = pSrc2 + extendWidth;
	
	for(i = 0; i < RV_UV_EXTEND_SIZE; i++)
	{
		memcpy (pDst1, pSrc1, extendWidth);
		memcpy (pDst2, pSrc2, extendWidth);
		pDst1 += extendWidth;
		pDst2 += extendWidth;
	}	
}

/*
Y extent 8 pixel
UV extent 4 pixel
*/
void RvDec_ExtendFrame_d(RV_DECODER_T *rv_decoder_ptr)
{
	int32 i;
	int32 height, width, offset, extendWidth, copyWidth;
	uint8 *pSrc1, *pSrc2, *pDst1, *pDst2;
	uint8 **Frame = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV;

#if !defined(ASM_HOR_EXTENSION)
	height      = rv_decoder_ptr->uFrameHeight_d;
	width       = rv_decoder_ptr->uFrameWidth_d;
	extendWidth = rv_decoder_ptr->uFrameExtendWidth_d;
	offset      = rv_decoder_ptr->uStartInFrameY_d;

	pSrc1 = Frame[0] + offset;
	pDst1 = pSrc1 - (RV_Y_EXTEND_SIZE/2);
	pSrc2 = pSrc1 + width - 1;
	pDst2 = pSrc2 + 1;
	copyWidth = (RV_Y_EXTEND_SIZE/2);	

	/*horizontal repeat Y*/
	for(i = 0; i < height; i++)
	{
		int32 intValue = ((int32)(*pSrc1) << 24) | ((int32)(*pSrc1) << 16) | ((int32)(*pSrc1) << 8) | (int32)(*pSrc1);
		int32 * pIntDst = (int32 *)pDst1;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;
//		pIntDst[2] = intValue;
//		pIntDst[3] = intValue;

		intValue = ((int32)(*pSrc2) << 24) | ((int32)(*pSrc2) << 16) | ((int32)(*pSrc2) << 8) | (int32)(*pSrc2);
		pIntDst = (int32 *)pDst2;
		pIntDst[0] = intValue;
		pIntDst[1] = intValue;
//		pIntDst[2] = intValue;
//		pIntDst[3] = intValue;

		pSrc1 += extendWidth;
		pDst1 += extendWidth;
		pSrc2 += extendWidth;
		pDst2 += extendWidth;
	}

	/*horizontal repeat U*/
	extendWidth = extendWidth / 2;
	offset      = rv_decoder_ptr->uStartInFrameUV_d;
	pSrc1       = Frame [1] + offset;
	pDst1       = pSrc1 - (RV_UV_EXTEND_SIZE/2);
	pSrc2 = pSrc1 + width / 2 - 1;
	pDst2 = pSrc2 + 1;
	copyWidth   = (RV_UV_EXTEND_SIZE/2);
	height = height / 2;
	
	for(i = 0; i < height; i++)
	{
		int32 intValue = ((int32)(*pSrc1) << 24) | ((int32)(*pSrc1) << 16) | ((int32)(*pSrc1) << 8) | (int32)(*pSrc1);
		int32 *pIntDst = (int32 *)pDst1;
		pIntDst[0] = intValue;
// 		pIntDst[1] = intValue;
	
		intValue = ((int32)(*pSrc2) << 24) | ((int32)(*pSrc2) << 16) | ((int32)(*pSrc2) << 8) | (int32)(*pSrc2);
		pIntDst = (int32 *)pDst2;
		pIntDst[0] = intValue;
// 		pIntDst[1] = intValue;

		pSrc1 += extendWidth;
		pDst1 += extendWidth;
		pSrc2 += extendWidth;
		pDst2 += extendWidth;
	}

	/*horizontal repeat V*/

	offset      = rv_decoder_ptr->uStartInFrameUV_d;
	pSrc1       = Frame [2] + offset;
	pDst1       = pSrc1 - (RV_UV_EXTEND_SIZE/2);
	pSrc2 = pSrc1 + width / 2 - 1;
	pDst2 = pSrc2 + 1;
	copyWidth   = (RV_UV_EXTEND_SIZE/2);
	
	for (i = 0; i < height; i++)
	{
		int32 intValue = ((int32)(*pSrc1) << 24) | ((int32)(*pSrc1) << 16) | ((int32)(*pSrc1) << 8) | (int32)(*pSrc1);
		int32 * pIntDst = (int32 *)pDst1;
		pIntDst[0] = intValue;
// 		pIntDst[1] = intValue;	
		
		intValue = ((int32)(*pSrc2) << 24) | ((int32)(*pSrc2) << 16) | ((int32)(*pSrc2) << 8) | (int32)(*pSrc2);
		pIntDst = (int32 *)pDst2;
		pIntDst[0] = intValue;
// 		pIntDst[1] = intValue;
		
		pSrc1 += extendWidth;
		pDst1 += extendWidth;
		pSrc2 += extendWidth;
		pDst2 += extendWidth;	
	}

#else
	asm_horExtendYUV_d(Frame, rv_decoder_ptr->uStartInFrameY_d, rv_decoder_ptr->uStartInFrameUV_d, 
		rv_decoder_ptr->uFrameExtendWidth_d, rv_decoder_ptr->uFrameHeight_d);
#endif

	/*copy first row and last row*/
	/*vertical repeat Y*/
	height = rv_decoder_ptr->uFrameHeight_d;
	extendWidth  = rv_decoder_ptr->uFrameExtendWidth_d;
	offset = extendWidth * (RV_Y_EXTEND_SIZE/2);
	pSrc1  = Frame[0] + offset;
	pDst1  = Frame[0];
	pSrc2  = pSrc1 + extendWidth * (height - 1);
	pDst2  = pSrc2 + extendWidth;

	for(i = 0; i < (RV_Y_EXTEND_SIZE/2); i++)
	{
		memcpy(pDst1, pSrc1, extendWidth);
		memcpy(pDst2, pSrc2, extendWidth);
		pDst1 += extendWidth;
		pDst2 += extendWidth;
	}

    /*vertical repeat U*/
    height = height / 2;
	extendWidth  = extendWidth / 2;
	offset = offset / 4;
	pSrc1  = Frame[1] + offset;
	pDst1  = Frame[1];
	pSrc2  = pSrc1 + extendWidth * (height - 1);
	pDst2  = pSrc2 + extendWidth;

	for(i = 0; i < (RV_UV_EXTEND_SIZE/2); i++)
	{
		memcpy (pDst1, pSrc1, extendWidth);
		memcpy (pDst2, pSrc2, extendWidth);
		pDst1 += extendWidth;
		pDst2 += extendWidth;
	}

	/*vertical repeat U*/
	pSrc1  = Frame[2] + offset;
	pDst1  = Frame[2];
	pSrc2  = pSrc1 + extendWidth * (height - 1);
	pDst2  = pSrc2 + extendWidth;
	
	for(i = 0; i < (RV_UV_EXTEND_SIZE/2); i++)
	{
		memcpy (pDst1, pSrc1, extendWidth);
		memcpy (pDst2, pSrc2, extendWidth);
		pDst1 += extendWidth;
		pDst2 += extendWidth;
	}	
}

PUBLIC void RvDec_exit_picture(RV_DECODER_T *rv_decoder_ptr)
{	
	/*reorder reference frame list*/
	if (rv_decoder_ptr->uPicCodeType != TRUEBPIC)
	{
		STORABLE_PICTURE_T * pic;

		pic = rv_decoder_ptr->pCurRecFrame;
		pic->pDecFrame->bRef = TRUE;
		if (pic->is_downsized)
		{
			RvDec_ExtendFrame_d(rv_decoder_ptr);
		} else
		{
			RvDec_ExtendFrame(rv_decoder_ptr);
		}
	
		/*the buffer for forward reference frame will no longe be a reference frame*/		
		if (rv_decoder_ptr->pFrdRefFrame->pDecFrame != NULL)
		{
			rv_decoder_ptr->pFrdRefFrame->pDecFrame->bRef = 0;
			rv_decoder_ptr->pFrdRefFrame->pDecFrame = NULL;
		}

		pic = rv_decoder_ptr->pFrdRefFrame;
		rv_decoder_ptr->pFrdRefFrame = rv_decoder_ptr->pBckRefFrame;
		rv_decoder_ptr->pBckRefFrame = rv_decoder_ptr->pCurRecFrame;	
		rv_decoder_ptr->pCurRecFrame = pic;	
	}	
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
