
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_video_header.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define CHROMA_DIRECT_MC_SIMPFIED

void RvDec_InitMCFun(BOOLEAN bReal8)
{
#if defined(RV8_SUPPORTED)
	if(bReal8)
	{
		RvDec_InitMCFun_RV8();
	}else
#endif	
	{
		RvDec_InitMCFun_RV9();
	}	
}

//direct copy ref mb from reference frame to reconstruction frame
#if !defined(_ARM_MC_ASSEMBLY_)
PUBLIC void copyRefToRecFrm (RV_DECODER_T *rv_decoder_ptr, uint8 *ppxlcRecMBYUV[3])
{
	int32 i, uv;
	int32 *pIntSrc, *pIntDst;
	int32 width = rv_decoder_ptr->uFrameExtendWidth >> 2;
	uint8 **refFrame = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV;
	uint8 **recFrame = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV;
	uint32 offset;
	
	/*put Y*/
	offset = ppxlcRecMBYUV[0] - recFrame[0]; //byte
	pIntSrc = (int32 *)(refFrame[0] + offset);
	pIntDst = (int32 *)ppxlcRecMBYUV[0];
	for (i = MB_SIZE; i > 0; i--)
	{
		pIntDst [0] = pIntSrc[0];
		pIntDst [1] = pIntSrc[1];
		pIntDst [2] = pIntSrc[2];
		pIntDst [3] = pIntSrc[3];

		pIntSrc += width;
		pIntDst += width;
	}

	/*put U*/
	width >>= 1;
	offset = ppxlcRecMBYUV[1] - recFrame[1]; //byte
	for (uv = 1; uv < 3; uv++)
	{
		pIntSrc = (int32 *)(refFrame[uv] + offset);
		pIntDst = (int32 *)ppxlcRecMBYUV[uv];

		for (i = 0; i < BLOCK_SIZE; i++)
		{
			pIntDst [0]  = pIntSrc[0];
			pIntDst [1]  = pIntSrc[1];
			
			pIntSrc += width;
			pIntDst += width;
		}
	}
}
//#else
//;
#endif

//direct copy ref mb from reference frame to reconstruction frame
#if !defined(_ARM_MC_ASSEMBLY_)
PUBLIC void copyRefToRecFrm_d (RV_DECODER_T *rv_decoder_ptr, uint8 *ppxlcRecMBYUV[3])
{
	int32 i, uv;
	uint8 *pSrc, *pDst;
	int32 width = rv_decoder_ptr->uFrameExtendWidth_d;
	uint8 **refFrame = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV;
	uint8 **recFrame = rv_decoder_ptr->pCurRecFrame->pDecFrame->imgYUV;
	uint32 offset;
	MB_CACHE_T * mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	uint8 *pred, *pred_d;
	int32 t0, t1, t2, t3, t4, t5, t6, t7;

	
/*put Y*/
	offset = ppxlcRecMBYUV[0] - recFrame[0]; //byte
	pSrc = refFrame[0] + offset;
	pDst = ppxlcRecMBYUV[0];
	for (i = MB_SIZE_D; i > 0; i--)
	{
		((int32 *)pDst) [0] = ((int32 *)pSrc)[0];
		((int32 *)pDst) [1] = ((int32 *)pSrc)[1];

		pSrc += width;
		pDst += width;
	}

	// up scale bottom line
	pred = mb_cache_ptr->mbpix.pMBBfrY + (MB_SIZE-1)*MB_SIZE;
	pred_d = pSrc - width;

	//0, 1
	t0 = *pred_d++;	t1 = *pred_d++; t2 = *pred_d++;
	*pred++ = t0;
	*pred++ = ((-1)*t0 + 9*t0 + 9*t1 + (-1)*t2 + 8) >> 4;

	//2, 3
	t3 = *pred_d++;
	*pred++ = t1;
	*pred++ = ((-1)*t0 + 9*t1 + 9*t2 + (-1)*t3 + 8) >> 4;

	//4, 5
	t4 = *pred_d++;
	*pred++ = t2;
	*pred++ = ((-1)*t1 + 9*t2 + 9*t3 + (-1)*t4 + 8) >> 4;

	//6, 7
	t5 = *pred_d++;
	*pred++ = t3;
	*pred++ = ((-1)*t2 + 9*t3 + 9*t4 + (-1)*t5 + 8) >> 4;

	//8, 9
	t6 = *pred_d++;
	*pred++ = t4;
	*pred++ = ((-1)*t3 + 9*t4 + 9*t5 + (-1)*t6 + 8) >> 4;

	//10, 11
	t7 = *pred_d; pred_d -= width;
	*pred++ = t5;
	*pred++ = ((-1)*t4 + 9*t5 + 9*t6 + (-1)*t7 + 8) >> 4;

	//12, 13
	*pred++ = t6;
	*pred++ = ((-1)*t5 + 9*t6 + 9*t7 + (-1)*t7 + 8) >> 4;

	//14, (pix15 in left line)
	*pred++ = t7;

	// up scale left line
	t6 = *pred_d;pred_d -= width;
	
	//15, 14
	*pred = ((-1)*t6 + 9*t7 + 9*t7 + (-1)*t7 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t7;	pred -= MB_SIZE;

	//13, 12
	t5 = *pred_d;pred_d -= width;
	*pred = ((-1)*t5 + 9*t6 + 9*t7 + (-1)*t7 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t6;	pred -= MB_SIZE;
	
	//11, 10
	t4 = *pred_d;pred_d -= width;
	*pred = ((-1)*t4 + 9*t5 + 9*t6 + (-1)*t7 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t5;	pred -= MB_SIZE;

	//9, 8
	t3 = *pred_d;pred_d -= width;
	*pred = ((-1)*t3 + 9*t4 + 9*t5 + (-1)*t6 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t4;	pred -= MB_SIZE;

	//7, 6
	t2 = *pred_d;pred_d -= width;
	*pred = ((-1)*t2 + 9*t3 + 9*t4 + (-1)*t5 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t3;	pred -= MB_SIZE;

	//5, 4
	t1 = *pred_d;pred_d -= width;
	*pred = ((-1)*t1 + 9*t2 + 9*t3 + (-1)*t4 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t2;	pred -= MB_SIZE;

	//3, 2
	t0 = *pred_d;
	*pred = ((-1)*t0 + 9*t1 + 9*t2 + (-1)*t3 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t1;	pred -= MB_SIZE;
	
	//1, 0
	*pred = ((-1)*t0 + 9*t0 + 9*t1 + (-1)*t2 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t0;


/*put U*/
	width >>= 1;
	offset = ppxlcRecMBYUV[1] - recFrame[1]; //byte
	for (uv = 1; uv < 3; uv++)
	{
		pSrc = (refFrame[uv] + offset);
		pDst = ppxlcRecMBYUV[uv];

		for (i = 0; i < MB_CHROMA_SIZE_D; i++)
		{
			((int32 *)pDst) [0]  = ((int32 *)pSrc)[0];
			
			pSrc += width;
			pDst += width;
		}

	//chroma
		// up scale bottom line
		pred = mb_cache_ptr->mbpix.pMBBfrU + (uv-1)*64 + (MB_CHROMA_SIZE-1)*MB_CHROMA_SIZE;
		pred_d = pSrc - width;

		//0, 1
		t0 = *pred_d++;	t1 = *pred_d++; t2 = *pred_d++;
		*pred++ = t0;
		*pred++ = ((-1)*t0 + 9*t0 + 9*t1 + (-1)*t2 + 8) >> 4;

		//2, 3
		t3 = *pred_d; pred_d -= width;
		*pred++ = t1;
		*pred++ = ((-1)*t0 + 9*t1 + 9*t2 + (-1)*t3 + 8) >> 4;

		//4, 5
		*pred++ = t2;
		*pred++ = ((-1)*t1 + 9*t2 + 9*t3 + (-1)*t3 + 8) >> 4;

		//6, (pix7 in left line)
		*pred++ = t3;

	// up scale left line

		//7, 6
		t2 = *pred_d;pred_d -= width;
		*pred = ((-1)*t2 + 9*t3 + 9*t3 + (-1)*t3 + 8) >> 4;	pred -= MB_CHROMA_SIZE;
		*pred = t3;	pred -= MB_CHROMA_SIZE;

		//5, 4
		t1 = *pred_d;pred_d -= width;
		*pred = ((-1)*t1 + 9*t2 + 9*t3 + (-1)*t3 + 8) >> 4;	pred -= MB_CHROMA_SIZE;
		*pred = t2;	pred -= MB_CHROMA_SIZE;

		//3, 2
		t0 = *pred_d;
		*pred = ((-1)*t0 + 9*t1 + 9*t2 + (-1)*t3 + 8) >> 4;	pred -= MB_CHROMA_SIZE;
		*pred = t1;	pred -= MB_CHROMA_SIZE;
		
		//1, 0
		*pred = ((-1)*t0 + 9*t0 + 9*t1 + (-1)*t2 + 8) >> 4;	pred -= MB_CHROMA_SIZE;
		*pred = t0;
	}
}
//#else
//;
#endif

#if defined(BSLICE_SUPPORTED)

#define clip(min, max, val) \
{\
	if((val) < (min))	(val) = (min);\
	else if ((val) > (max)) (val) = (max);\
}

#define clip1(min, max, mv, val) \
{\
	if((mv) < (min))	(mv) = (min);\
	else if ((mv) > (max)) (mv) = (max);\
	val = mv;\
}

PUBLIC void RvDec_MC_oneDirection_MB (RV_DECODER_T *rv_decoder_ptr, uint8 **pRefFrame, MB_PIXEL_T *pMBPix, int16 *pMVCache)
{
	int32 dx, dy;
	int32 mv_x, mv_y;
	uint8 *pRef;
	int32 refOffset;
    int32 iRefPosXInt, iRefPosYInt;
	int32 uExdFrameWidth =  rv_decoder_ptr->uFrameExtendWidth;
	int32 iMbPosX = (rv_decoder_ptr->uMbPosX * MB_SIZE + RV_Y_EXTEND_SIZE);
	int32 iMbPosY = (rv_decoder_ptr->uMbPosY * MB_SIZE + RV_Y_EXTEND_SIZE);
	int16 *pMVMB = pMVCache + 5*2;

	/*interpolation for Y*/
	iRefPosXInt = iMbPosX;
	mv_x = pMVMB[0];
	GET_MC_POS(mv_x, iRefPosXInt, dx);
	
	iRefPosYInt = iMbPosY;
	mv_y = pMVMB[1];
	GET_MC_POS(mv_y, iRefPosYInt, dy);
				
	pRef = pRefFrame[0] + iRefPosYInt * uExdFrameWidth + iRefPosXInt;
	g_RvDec_Luma16x16_MC[(dx<<2)|dy](pRef, pMBPix->pMBBfrY, uExdFrameWidth);

	/*interpolation for UV*/
	uExdFrameWidth >>= 1;
	
	iRefPosXInt = (iMbPosX>>1);
	mv_x /=  2;
	GET_MC_POS(mv_x, iRefPosXInt, dx);
	
	iRefPosYInt = (iMbPosY>>1);
	mv_y /= 2;
	GET_MC_POS(mv_y, iRefPosYInt, dy);
			
	refOffset = iRefPosYInt * uExdFrameWidth + iRefPosXInt;
	g_RvDec_Chroma8x8_MC[(dx<<2)|dy](pRefFrame[1] + refOffset, pMBPix->pMBBfrU, uExdFrameWidth);
	g_RvDec_Chroma8x8_MC[(dx<<2)|dy](pRefFrame[2] + refOffset, pMBPix->pMBBfrV, uExdFrameWidth);
}

//copy ref mb from reference frame to mb cache buffer
void copy_ref_MB (uint8 **pRefFrame, MB_PIXEL_T *pMBPix, int32 pitch, int32 iMbPosX, int32 iMbPosY)
#if !defined(_ARM_MC_ASSEMBLY_)
{
	int32 i;
	uint8 *pRef;
	uint32 *pRefInt, *pPredInt;

	pRef = pRefFrame[0] + iMbPosY*pitch + iMbPosX;
	pPredInt = (uint32 *)pMBPix->pMBBfrY;	
	for (i = 16; i > 0; i--)
	{
		pRefInt = (uint32 *)pRef;

		*pPredInt++ = pRefInt [0];
		*pPredInt++ = pRefInt [1];
		*pPredInt++ = pRefInt [2];
		*pPredInt++ = pRefInt [3];

		pRef += pitch;
	}

	iMbPosX >>= 1;
	iMbPosY >>= 1;
	pitch   >>= 1;

	/*copy U*/
	pRef = pRefFrame[1] + iMbPosY*pitch + iMbPosX;
	pPredInt = (uint32 *)pMBPix->pMBBfrU;	
	for (i = 8; i > 0; i--)
	{
		pRefInt = (uint32 *)pRef;
		
		*pPredInt++ = pRefInt [0];
		*pPredInt++ = pRefInt [1];
		
		pRef += pitch;
	}	


	/*copy V*/
	pRef = pRefFrame[2] + iMbPosY*pitch + iMbPosX;
	pPredInt = (uint32 *)pMBPix->pMBBfrV;
	for (i = 8; i > 0; i--)
	{
		pRefInt = (uint32 *)pRef;
		
		*pPredInt++ = pRefInt [0];
		*pPredInt++ = pRefInt [1];
		
		pRef += pitch;
	}
}
#else
;
#endif



#if defined(CHROMA_DIRECT_MC_SIMPFIED)
LOCAL void MC_direct_oneMV_C (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_f_ptr, MOTION_VECTOR_T *mv_b_ptr)
{
	int32 dx, dy;
	int32 mv_x, mv_y;
	uint8 *pRef, *pPred;
	int32 refPosXInt, refPosYInt;
	int32 mbposx = rv_decoder_ptr->uMbPosX*8 + RV_UV_EXTEND_SIZE;
	int32 mbposy = rv_decoder_ptr->uMbPosY*8 + RV_UV_EXTEND_SIZE;
	int32 extWidth = rv_decoder_ptr->uFrameExtendWidth / 2;
	int32 refOffset;
	mc_func *chroma8x8_mc = g_RvDec_Chroma8x8_MC;

//fwd
	refPosXInt = mbposx;
	mv_x = mv_f_ptr->x / 2;
	GET_MC_POS(mv_x, refPosXInt, dx);
	
	refPosYInt = mbposy;
	mv_y = mv_f_ptr->y / 2;
	GET_MC_POS(mv_y, refPosYInt, dy);

	refOffset = refPosYInt * extWidth + refPosXInt;
	pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[1] + refOffset;
	pPred = mb_cache_ptr->mbpix.pMBBfrU;
	chroma8x8_mc[(dx<<2)|dy](pRef, pPred, extWidth);

	pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[2] + refOffset;
	pPred = mb_cache_ptr->mbpix.pMBBfrV;
	chroma8x8_mc[(dx<<2)|dy](pRef, pPred, extWidth);
	
//bwd
	refPosXInt = mbposx;
	mv_x = mv_b_ptr->x /2;
	GET_MC_POS(mv_x, refPosXInt, dx);

	refPosYInt = mbposy;
	mv_y = mv_b_ptr->y / 2;
	GET_MC_POS(mv_y, refPosYInt, dy);

	refOffset = refPosYInt * extWidth + refPosXInt;
	pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[1] + refOffset;
	pPred = mb_cache_ptr->mbpix_bck.pMBBfrU;
	chroma8x8_mc[(dx<<2)|dy](pRef, pPred, extWidth);
	
	pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[2] + refOffset;
	pPred = mb_cache_ptr->mbpix_bck.pMBBfrV;
	chroma8x8_mc[(dx<<2)|dy](pRef, pPred, extWidth);
}
#else
LOCAL void MC_direct_oneMV_C (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, int32 refMVX, int32 refMVY)
{
	int32 max, min;
	MOTION_VECTOR_T mv_f, mv_b;
	int32 dx_f, dy_f;
	int32 dx_b, dy_b;
	int32 mv;
	uint8 *pRef, *pPred;
	int32 refPosXInt_f, refPosYInt_f;
	int32 refPosXInt_b, refPosYInt_b;
	int32 mbposx = rv_decoder_ptr->uMbPosX*8;
	int32 mbposy = rv_decoder_ptr->uMbPosY*8;
	int32 iRatio0 = rv_decoder_ptr->iRatio0;
	int32 uMcFactor = rv_decoder_ptr->uMcFactor;
	int32 extWidth = rv_decoder_ptr->uFrameExtendWidth / 2;
	
	refPosXInt_f = mbposx + RV_UV_EXTEND_SIZE;
	refPosXInt_b = refPosXInt_f;	
	if (refMVX)
	{
		mv_f.x = mv = (int16)((iRatio0*refMVX + (1<<13)) >> 14);	
		GET_MC_POS(mv, refPosXInt_f, dx_f);	
		
		max = rv_decoder_ptr->xmax_chroma - mbposx * uMcFactor;//(framewidth - mbposx - 1) * uMcFactor - 1;
		min = (-7 - mbposx) * uMcFactor + 1;
		mv -= refMVX;
		clip1 (min, max, mv, mv_b.x);	
		GET_MC_POS(mv_b.x, refPosXInt_b, dx_b);		
	}else
	{
		dx_f = 0;
		dx_b = 0;
	}

	refPosYInt_f = mbposy + RV_UV_EXTEND_SIZE;
	refPosYInt_b = refPosYInt_f;	
	if (refMVY)
	{
		mv_f.y = mv = (int16)((iRatio0*refMVY + (1<<13)) >> 14);
		GET_MC_POS(mv, refPosYInt_f, dy_f);

		max = rv_decoder_ptr->ymax_chroma - mbposy * uMcFactor;//(frameHeigth - mbposy - 1) * uMcFactor - 1;
		min = (-7 - mbposy) * uMcFactor + 1;	
		mv -= refMVY;
		clip1 (min, max, mv, mv_b.y);	
		GET_MC_POS(mv, refPosYInt_b, dy_b);				
	}else
	{
		dy_f = 0;
		dy_b = 0;
	}

	pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[1] + refPosYInt_f * extWidth + refPosXInt_f;
	pPred = mb_cache_ptr->mbpix.pMBBfrU;
	g_RvDec_Chroma8x8_MC[(dx_f<<2)|dy_f](pRef, pPred, extWidth);

	pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[1] + refPosYInt_b * extWidth + refPosXInt_b;
	pPred = mb_cache_ptr->mbpix_bck.pMBBfrU;
	g_RvDec_Chroma8x8_MC[(dx_b<<2)|dy_b](pRef, pPred, extWidth);

	pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[2] + refPosYInt_f * extWidth + refPosXInt_f;
	pPred = mb_cache_ptr->mbpix.pMBBfrV;
	g_RvDec_Chroma8x8_MC[(dx_f<<2)|dy_f](pRef, pPred, extWidth);
	
	pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[2] + refPosYInt_b * extWidth + refPosXInt_b;
	pPred = mb_cache_ptr->mbpix_bck.pMBBfrV;
	g_RvDec_Chroma8x8_MC[(dx_b<<2)|dy_b](pRef, pPred, extWidth);
}
#endif

void MC_direct_refOneMV (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *pMV_ref)
{
	int32 refMVX, refMVY;
	MOTION_VECTOR_T mv_f, mv_b;
	int32 max, min;
	int32 dx_f, dy_f;
	int32 dx_b, dy_b;
	int32 mv;
	int32 uExtWidth;
	uint8 *pRef, *pPred;
	int32 refPosXInt_f, refPosYInt_f;
	int32 refPosXInt_b, refPosYInt_b;
	int32 uMcFactor = rv_decoder_ptr->uMcFactor;
	int32 mbposx = rv_decoder_ptr->uMbPosX * MB_SIZE;
	int32 mbposy = rv_decoder_ptr->uMbPosY * MB_SIZE;
	
	refMVX = pMV_ref->x;
	refMVY = pMV_ref->y;

	refPosXInt_f = (mbposx + RV_Y_EXTEND_SIZE);
	refPosXInt_b = refPosXInt_f;	
	if (refMVX)
	{
		mv_f.x = mv = (int16)((rv_decoder_ptr->iRatio0*refMVX + (1<<13)) >> 14);	
		GET_MC_POS(mv, refPosXInt_f, dx_f);
		
		max = rv_decoder_ptr->xmax_luma - mbposx * uMcFactor;//(frameWidth - 3 - mbposx) * uMcFactor - 1;
		min = (-14 - mbposx) * uMcFactor + 1;
		mv -=  refMVX;
		clip1 (min, max, mv, mv_b.x);		
		GET_MC_POS(mv, refPosXInt_b, dx_b);
	}else
	{	
		dx_f = 0;
		dx_b = 0;
#if defined(CHROMA_DIRECT_MC_SIMPFIED)  //for chroma mc
		mv_f.x = 0;   
		mv_b.x = 0;
#endif		
	}

	refPosYInt_f = (mbposy + RV_Y_EXTEND_SIZE);	
	refPosYInt_b = refPosYInt_f;	
	if (refMVY)
	{		
		mv_f.y = mv = (int16)((rv_decoder_ptr->iRatio0*refMVY + (1<<13)) >> 14);		
		GET_MC_POS(mv, refPosYInt_f, dy_f);
		
		max = rv_decoder_ptr->ymax_luma - mbposy * uMcFactor;//(frameHeight - 6 - mbposy) * uMcFactor - 1;
		min = (-11 - mbposy) * uMcFactor + 1;
		mv -= refMVY;	
		clip1 (min, max, mv, mv_b.y);
		GET_MC_POS(mv, refPosYInt_b, dy_b);
	}else
	{
		dy_f = 0;
		dy_b = 0;
#if defined(CHROMA_DIRECT_MC_SIMPFIED)  //for chroma mc
		mv_f.y = 0;   
		mv_b.y = 0;
#endif		
	}

	uExtWidth = rv_decoder_ptr->uFrameExtendWidth;
		
	pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[0] + refPosYInt_f*uExtWidth + refPosXInt_f;
	pPred = mb_cache_ptr->mbpix.pMBBfrY;
	g_RvDec_Luma16x16_MC[(dx_f<<2)|dy_f](pRef, pPred, uExtWidth);

	pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[0] + refPosYInt_b*uExtWidth + refPosXInt_b;
	pPred = mb_cache_ptr->mbpix_bck.pMBBfrY;
	g_RvDec_Luma16x16_MC[(dx_b<<2)|dy_b](pRef, pPred, uExtWidth);
	
	/*compute motion vector for UV*/
#if defined(CHROMA_DIRECT_MC_SIMPFIED)
	MC_direct_oneMV_C (rv_decoder_ptr, mb_cache_ptr, &mv_f, &mv_b);
#else
	MC_direct_oneMV_C (rv_decoder_ptr, mb_cache_ptr, refMVX/2, refMVY/2);
#endif
}

#if defined(CHROMA_DIRECT_MC_SIMPFIED)
void MC_direct_fourMV_C (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_f_ptr, MOTION_VECTOR_T *mv_b_ptr)
{
	int32 i;
	int32 blkOffset, refOffset;
	int32 dx, dy;
	int32 pos_x, pos_y;
	int32 mv_x, mv_y;
	int32 refPosXInt, refPosYInt;
	int32 mbposx = rv_decoder_ptr->uMbPosX*8 + RV_UV_EXTEND_SIZE;
	int32 mbposy = rv_decoder_ptr->uMbPosY*8 + RV_UV_EXTEND_SIZE;
	int32 extWidth = rv_decoder_ptr->uFrameExtendWidth / 2;
	uint8 *pRef, *pPred;
	mc_func *chroma4x4_mc = g_RvDec_Chroma4x4_MC;

	for (i = 0; i < 4; i++)
	{
		blkOffset = (i>>1)*32 + (i&1)*4;
		pos_x = mbposx + (i&1)*4;
		pos_y = mbposy + (i>>1)*4;

	//fwd
		refPosXInt = pos_x;
		mv_x = mv_f_ptr->x / 2;
		GET_MC_POS(mv_x, refPosXInt, dx);

		refPosYInt = pos_y;
		mv_y = mv_f_ptr->y / 2;
		GET_MC_POS(mv_y, refPosYInt, dy);
		mv_f_ptr++;
		
		refOffset = refPosYInt * extWidth + refPosXInt;
		
		pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[1] + refOffset;
		pPred = mb_cache_ptr->mbpix.pMBBfrU + blkOffset;
		chroma4x4_mc[(dx<<2)|dy](pRef, pPred, extWidth);

		pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[2] + refOffset;
		pPred = mb_cache_ptr->mbpix.pMBBfrV + blkOffset;
		chroma4x4_mc[(dx<<2)|dy](pRef, pPred, extWidth);

	//bwd
		refPosXInt = pos_x;
		mv_x = mv_b_ptr->x /2;
		GET_MC_POS(mv_x, refPosXInt, dx);

		refPosYInt = pos_y;
		mv_y = mv_b_ptr->y / 2;
		GET_MC_POS(mv_y, refPosYInt, dy);
		mv_b_ptr++;

		refOffset = refPosYInt * extWidth + refPosXInt;
		pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[1] + refOffset;
		pPred = mb_cache_ptr->mbpix_bck.pMBBfrU + blkOffset;
		chroma4x4_mc[(dx<<2)|dy](pRef, pPred, extWidth);
		
		pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[2] + refOffset;
		pPred = mb_cache_ptr->mbpix_bck.pMBBfrV + blkOffset;
		chroma4x4_mc[(dx<<2)|dy](pRef, pPred, extWidth);		
	}
}
#else
void MC_direct_fourMV_C (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *pMV_ref)
{
	int32 i;
	int32 max, min;
	int32 blkOffset;
	int32 mv_f, mv_b;
	int32 dx_f, dy_f;
	int32 dx_b, dy_b;
	int32 refMV, pos;
	int32 refPosXInt_f, refPosYInt_f;
	int32 refPosXInt_b, refPosYInt_b;
	int32 mbposx = rv_decoder_ptr->uMbPosX*8;
	int32 mbposy = rv_decoder_ptr->uMbPosY*8;
	int32 iRatio0 = rv_decoder_ptr->iRatio0;
	int32 uMcFactor = rv_decoder_ptr->uMcFactor;
	int32 extWidth = rv_decoder_ptr->uFrameExtendWidth / 2;
	uint8 *pRef, *pPred;
	mc_func *chroma4x4_mc = g_RvDec_Chroma4x4_MC;

	for (i = 0; i < 4; i++)
	{
		pos = mbposx + (i&1)*4;
		refPosXInt_f = pos + RV_UV_EXTEND_SIZE;
		refPosXInt_b = refPosXInt_f;
		
		refMV = pMV_ref->x/2;
		if (refMV)
		{
			mv_f = (short)((iRatio0*refMV + (1<<13)) >> 14);	
			mv_b = mv_f - refMV;
			
			max = rv_decoder_ptr->xmax4v_chroma - pos * uMcFactor; //(framewidth - posx - 3) * uMcFactor - 1;
			min = (-7 - pos) * uMcFactor + 1;
			clip (min, max, mv_b);	
			
			GET_MC_POS(mv_f, refPosXInt_f, dx_f);
			GET_MC_POS(mv_b, refPosXInt_b, dx_b);	
		}else
		{
			dx_f = 0;
			dx_b = 0;
		}
		
		pos = mbposy + (i>>1)*4;
		refPosYInt_f = (pos + RV_UV_EXTEND_SIZE);	
		refPosYInt_b = refPosYInt_f;
		
		refMV = pMV_ref->y/2;
		if (refMV)
		{
			mv_f = (short)((iRatio0*refMV + (1<<13)) >> 14);
			mv_b = mv_f - refMV;	
			max = rv_decoder_ptr->ymax4v_chroma - pos*uMcFactor; //(frameHeigth - posy - 3) * uMcFactor - 1;
			min = (-7 - pos) * uMcFactor + 1;
			clip (min, max, mv_b);	

			GET_MC_POS(mv_f, refPosYInt_f, dy_f);
			GET_MC_POS(mv_b, refPosYInt_b, dy_b);	
		}else
		{
			dy_f = 0;
			dy_b = 0;
		}

		blkOffset = (i>>1)*32 + (i&1)*4;

		pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[1] + refPosYInt_f * extWidth + refPosXInt_f;
		pPred = mb_cache_ptr->mbpix.pMBBfrU + blkOffset;
		chroma4x4_mc[(dx_f<<2)|dy_f](pRef, pPred, extWidth);
		
		pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[1] + refPosYInt_b * extWidth + refPosXInt_b;
		pPred = mb_cache_ptr->mbpix_bck.pMBBfrU + blkOffset;
		chroma4x4_mc[(dx_b<<2)|dy_b](pRef, pPred, extWidth);
		
		pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[2] + refPosYInt_f * extWidth + refPosXInt_f;
		pPred = mb_cache_ptr->mbpix.pMBBfrV + blkOffset;
		chroma4x4_mc[(dx_f<<2)|dy_f](pRef, pPred, extWidth);
		
		pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[2] + refPosYInt_b * extWidth + refPosXInt_b;
		pPred = mb_cache_ptr->mbpix_bck.pMBBfrV + blkOffset;
		chroma4x4_mc[(dx_b<<2)|dy_b](pRef, pPred, extWidth);	
		
		pMV_ref++;
	}
}
#endif

void MC_direct_refFourMV (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *pMV_ref)
{
	int32 i;
	int32 max, min;
	int32 blkOffset;
	int32 dx_f, dy_f;
	int32 dx_b, dy_b;
	int32 refPosXInt_f, refPosYInt_f;
	int32 refPosXInt_b, refPosYInt_b;
	int32 refMV, pos;
	int32 iRatio0;
	int32 mv_f, mv_b;
	int32 uMcFactor = rv_decoder_ptr->uMcFactor;
	int32 mbposx = rv_decoder_ptr->uMbPosX * MB_SIZE;
	int32 mbposy = rv_decoder_ptr->uMbPosY * MB_SIZE;
	int32 extFrmWidth = rv_decoder_ptr->uFrameExtendWidth;
	uint8 * pRef, * pPred;
	mc_func *luma8x8_mc = g_RvDec_Luma8x8_MC;
	MOTION_VECTOR_T pMV_ref_b[4];
	MOTION_VECTOR_T *mv_ref_f_ptr, *mv_ref_b_ptr;
	
	iRatio0 = rv_decoder_ptr->iRatio0;
	mv_ref_f_ptr = pMV_ref;
	mv_ref_b_ptr = pMV_ref_b;

	for (i = 0; i < 4; i++)
	{
		pos = mbposx + (i&1) * 8;
		refPosXInt_f = (pos + RV_Y_EXTEND_SIZE);
		refPosXInt_b = refPosXInt_f;			
		
		refMV = mv_ref_f_ptr->x;
		if (refMV)
		{
			mv_f = (short)((iRatio0*refMV + (1<<13)) >> 14);	
			mv_b = mv_f - refMV;
			max = rv_decoder_ptr->xmax4v_luma - pos * uMcFactor;//(frameWidth + 5 - posx) * uMcFactor - 1;
			min = (-14 - pos) * uMcFactor + 1;
			clip (min, max, mv_b);

			GET_MC_POS(mv_f, refPosXInt_f, dx_f);
			GET_MC_POS(mv_b, refPosXInt_b, dx_b);	
#if defined(CHROMA_DIRECT_MC_SIMPFIED)  //for chroma mc
			mv_ref_b_ptr->x = mv_b;
#endif
		}else
		{
			dx_f = 0;
			dx_b = 0;

#if defined(CHROMA_DIRECT_MC_SIMPFIED)  //for chroma mc
			mv_ref_b_ptr->x = 0;
#endif
		}
		
		pos = mbposy + (i>>1) * 8;
		refPosYInt_f = pos + RV_Y_EXTEND_SIZE;
		refPosYInt_b = refPosYInt_f;
		
		refMV = mv_ref_f_ptr->y;	
		if (refMV)
		{	
			mv_f = (int16)((iRatio0*refMV + (1<<13)) >> 14);			
			mv_b = mv_f - refMV;
			max =  rv_decoder_ptr->ymax4v_luma - pos * uMcFactor;//(frameHeight + 2 - posy) * uMcFactor - 1;
			min = (-11 - pos) * uMcFactor + 1;
			clip (min, max, mv_b);	

			GET_MC_POS(mv_f, refPosYInt_f, dy_f);
			GET_MC_POS(mv_b, refPosYInt_b, dy_b);	

#if defined(CHROMA_DIRECT_MC_SIMPFIED)  //for chroma mc
			mv_ref_b_ptr->y = mv_b;
#endif
		}else
		{
			dy_f = 0;
			dy_b = 0;

#if defined(CHROMA_DIRECT_MC_SIMPFIED)  //for chroma mc
			mv_ref_b_ptr->y = 0;
#endif
		}
		mv_ref_f_ptr++;		
		mv_ref_b_ptr++;

		blkOffset =  (i>>1)*128 + (i&1)*8;
		pRef = rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV[0] + refPosYInt_f*extFrmWidth + refPosXInt_f;
		pPred = mb_cache_ptr->mbpix.pMBBfrY + blkOffset;
		luma8x8_mc[(dx_f<<2)|dy_f](pRef, pPred, extFrmWidth);
		
		pRef = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV[0] + refPosYInt_b*extFrmWidth + refPosXInt_b;
		pPred = mb_cache_ptr->mbpix_bck.pMBBfrY + blkOffset;
		luma8x8_mc[(dx_b<<2)|dy_b](pRef, pPred, extFrmWidth);
	}

	/*compute mv for UV*/
#if defined(CHROMA_DIRECT_MC_SIMPFIED)
	MC_direct_fourMV_C (rv_decoder_ptr, mb_cache_ptr, pMV_ref, pMV_ref_b);
#else	
	MC_direct_fourMV_C (rv_decoder_ptr, mb_cache_ptr, pMV_ref);
#endif
}

/******************************************************************
input:
	pmbpix: forwared reference MB, i.e previous reference MB
	pmbpix_b: backward reference MB, i.e, future reference MB
*******************************************************************/
#if !defined(_ARM_MC_ASSEMBLY_)
PUBLIC void RvDec_Weighted_Average_r0EQr1(RV_DECODER_T *rv_decoder_ptr, MB_PIXEL_T *pmbpix, MB_PIXEL_T *pmbpix_b)
{

#if 0
	int32 i, uv;
	uint8 * pPixF, *pPixP;
	
	/*Y*/
	pPixP = pmbpix->pMBBfrY;
	pPixF = pmbpix_b->pMBBfrY;
	for (i = 16; i > 0; i--)
	{
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;			
	}
	
	/*UV*/
	for (uv = 0; uv < 2; uv++)
	{
		pPixP = (uv == 0) ? pmbpix->pMBBfrU : pmbpix->pMBBfrV;
		pPixF = (uv == 0) ? pmbpix_b->pMBBfrU : pmbpix_b->pMBBfrV;
		for (i = 8; i > 0; i--)
		{
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
			*pPixP++ = (*pPixF++ + *pPixP + 1) >> 1;
		}
	}
#else
	int32 i;
	uint32 * pPixF, *pPixP;
	uint32 msk1 = 0xfefefefe;
	uint32 msk2 = 0x01010101;
	uint32 int_f, int_b;
	uint32 int0, int1;
	
	pPixP = (uint32 *)(pmbpix->pMBBfrY);
	pPixF = (uint32 *)(pmbpix_b->pMBBfrY);

	for (i = 96; i > 0; i--)
	{
		int_f = *pPixP;
		int_b = *pPixF++;

		int0 = int_f & msk1;
		int0 >>= 1;

		int1 = int_b & msk1;
		int0 += (int1>>1);

		int_f &= msk2;
		int_b &= msk2;
		int_f += int_b;
		int_f += msk2;
		int_f &= msk1;
		int0 += (int_f>>1);
		*pPixP++ = int0;
	}
#endif
}

PUBLIC void RvDec_Weighted_Average_r0NEr1(RV_DECODER_T *rv_decoder_ptr, MB_PIXEL_T *pmbpix, MB_PIXEL_T *pmbpix_b)
{
#if 0
	int32 i, uv;
	int32 v1, v2, w;
	uint8 * pPixF, *pPixP;
	int32 iRatio0 = rv_decoder_ptr->iRatio0;
	int32 iRatio1 = rv_decoder_ptr->iRatio1;
	
	/*Y*/
	pPixP = pmbpix->pMBBfrY;
	pPixF = pmbpix_b->pMBBfrY;
	for (i = 0; i < 256; i++)
	{
		v1 = pPixF[i] << 7;
		v2 = pPixP[i] << 7;
		
		w = (v1*iRatio0 >> 16) + (v2* iRatio1 >> 16);

		{
			int32 w1;
			w1 = (pPixF[i]*iRatio0 >> 9) + (pPixP[i]*iRatio1 >> 9);

			if (w1 != w)
			{
				foo();
			}

//			w1 = pPixF[i]*(iRatio0 >> 9) + pPixP[i]*(iRatio1 >> 9);
//			if (w1 != w)
//			{
//				foo();
//			}
		}
		
		pPixP [i] = (uint8)((w + 0x10) >> 5);
	}
	
	/*UV*/
	for (uv = 0; uv < 2; uv++)
	{
		pPixP = (uv == 0) ? pmbpix->pMBBfrU : pmbpix->pMBBfrV;
		pPixF = (uv == 0) ? pmbpix_b->pMBBfrU : pmbpix_b->pMBBfrV;
		
		for (i = 0; i < 64; i++)
		{
			v1 = pPixF[i] << 7;
			v2 = pPixP[i] << 7;
			
			w = (v1*iRatio0 >> 16) + (v2* iRatio1 >> 16);
			
			pPixP [i] = (uint8)((w + 0x10) >> 5);
		}
	}
#else
	int32 i;
	int32 f, b, w;
	uint8 *pPixF, *pPixP;
	int32 iRatio0 = rv_decoder_ptr->iRatio0;
	int32 iRatio1 = rv_decoder_ptr->iRatio1;

	pPixP = (pmbpix->pMBBfrY);
	pPixF = (pmbpix_b->pMBBfrY);

	for (i = 384; i > 0; i -= 4)
	{
		//0
		f = *pPixF++;	b = *pPixP;
		w = (f*iRatio0) + (b*iRatio1);
		*pPixP++ = (uint8)(w >> 14);

		///1
		f = *pPixF++;	b = *pPixP;
		w = (f*iRatio0) + (b*iRatio1);
		*pPixP++ = (uint8)(w >> 14);

		///2
		f = *pPixF++;	b = *pPixP;
		w = (f*iRatio0) + (b*iRatio1);
		*pPixP++ = (uint8)(w >> 14);

		///3
		f = *pPixF++;	b = *pPixP;
		w = (f*iRatio0) + (b*iRatio1);
		*pPixP++ = (uint8)(w >> 14);
	}
#endif
}
#endif

PUBLIC void RvDec_MC_direct_MB (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr)
{
	int32 refMBMode = (rv_decoder_ptr->mb_mode_ptr + rv_decoder_ptr->uMbNumDec)->uMbType;
	MOTION_VECTOR_T *pMV_ref = rv_decoder_ptr->pMVFrameP + rv_decoder_ptr->uMbNumDec*4;

#if 1 //for fast mc in case that ref_mv is zero mv, same as intra refmb
{
	if (!((int32 *)pMV_ref)[0] && !((int32 *)pMV_ref)[1] && !((int32 *)pMV_ref)[2] && !((int32 *)pMV_ref)[3])
	{
		refMBMode = 0; //intra
	}
}
#endif

	if ((refMBMode == MBTYPE_SKIPPED) || (refMBMode < 2)/*intra*/)
	{
		uint32 uExdFrameWidth =  rv_decoder_ptr->uFrameExtendWidth;
		int32 iMbPosX = rv_decoder_ptr->uMbPosX * MB_SIZE + RV_Y_EXTEND_SIZE;
		int32 iMbPosY = rv_decoder_ptr->uMbPosY * MB_SIZE + RV_Y_EXTEND_SIZE;
		
		copy_ref_MB (rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV, &mb_cache_ptr->mbpix, uExdFrameWidth, iMbPosX, iMbPosY);

		if (refMBMode != MBTYPE_SKIPPED)	//ref mb is intra mb
		{
			copy_ref_MB (rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV, &mb_cache_ptr->mbpix_bck, uExdFrameWidth, iMbPosX, iMbPosY);
			g_RvDec_Get_Weighted_Average (rv_decoder_ptr, &mb_cache_ptr->mbpix, &mb_cache_ptr->mbpix_bck);
		}
	}else
	{
		switch(refMBMode)
		{
		case MBTYPE_INTER:
		case MBTYPE_INTER_16x16:
			MC_direct_refOneMV (rv_decoder_ptr, mb_cache_ptr, pMV_ref);
			break;
		case MBTYPE_INTER_16x8V:			
		case MBTYPE_INTER_8x16V:
		case MBTYPE_INTER_4V:
			MC_direct_refFourMV (rv_decoder_ptr, mb_cache_ptr, pMV_ref);
			break;
		default:
		//	PRINTF ("wrong reference mb type!\n");
		        break;
		}
		
		g_RvDec_Get_Weighted_Average (rv_decoder_ptr, &mb_cache_ptr->mbpix, &mb_cache_ptr->mbpix_bck);
	}
}

#endif
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
