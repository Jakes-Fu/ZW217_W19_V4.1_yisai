
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

LOCAL __inline void RvDec_GetMVD(BitStream *stream, int32 pred_mv[2], int32 *mv_xy)
{
	uint32 j, val;
	uint32 uLen;
	int32 mv_x, mv_y, mvd;
	int32 *mv = &mv_x;

	for (j = 2; j > 0; j--)
	{
		//here is rvdec_getvlcbits()
		val = RvDec_ShowBits(stream, 32);
#if _DEBUG_
		if(!val)
		{
		//	PRINTF("wrong bit stream!\n");
		}
#endif
		if (val & 0xaa800000)
		{
			val >>= (32-8);
			uLen = g_interleaved_golomb_vlc[val][1];
			val = g_interleaved_golomb_vlc[val][0]+1;	
		}else
		{
			uint32 ubits, i = 0;
			uint32 uInfo = 0; 
			uint32 uLenAndInfo;
			uint32 uThisChunksLen;

			uLen = 0;
			
			do
			{
				ubits = (val >> (24-i*8)) & 0xff;
				uLenAndInfo = gs_VLCDecodeTable[ubits];
				uThisChunksLen = uLenAndInfo >> 4;

				uInfo = (uInfo << (uThisChunksLen >> 1)) | (uLenAndInfo & 0xf);
				uLen += uThisChunksLen;

				i++;
			}while(!(uLen & 1));
			
			val = (1 << (uLen >>1)) + uInfo;
		}
		
		RvDec_FlushBits(stream, uLen);

		mvd = val >> 1;

		if ((val&1))
		{
			mvd = -mvd;
		}

		*mv = mvd + (*pred_mv++);
		mv = &mv_y;
	}

#if defined(RV_BIG_ENDIAN)
	*mv_xy = (mv_x<<16) | (mv_y&0xffff);
#else
	*mv_xy = (mv_y<<16) | (mv_x&0xffff);
#endif
}

#define PIX_SWAP(a, b) {int32 temp = a; a = b; b = temp;}
#define PIX_SORT(a, b) {if (a > b) PIX_SWAP(a, b);}

__inline int32 RvDec_GetMedianofThree(int32 a0, int32 a1, int32 a2)
{
	PIX_SORT(a0,a1);
	PIX_SORT(a1,a2);
	PIX_SORT(a0,a1);

	return a1;
}

/****************************************************************
input: 
	mv_ptr: point to current block's MV in MV cache
	width: current block's width for choosing rigth-above block's mv
*****************************************************************/
LOCAL void RvDec_MotionPred_P(MB_CACHE_T *mb_cache_ptr, int16 *pMVCache, int32 *mv_ptr, int32 mbtype, int32 blkId)
{
	int32 blkwidth;
	int16 *sa, *sl, *sr;  //point to above, left, top-right candidator
	int32 leftAvail = mb_cache_ptr->mbAvailA;
	int32 topAvail = mb_cache_ptr->mbAvailB;
	int32 trAvail = mb_cache_ptr->mbAvailC;

	switch(mbtype)
	{
	case MBTYPE_INTER_16x16:
	case MBTYPE_INTER:
//	case MBTYPE_FORWARD:
//	case MBTYPE_BACKWARD:
		blkwidth = 2;
		break;

	case MBTYPE_INTER_16x8V:
		topAvail |= blkId;
		trAvail  &= (int32)(!blkId);
		blkwidth = 2;			
		break;
	case MBTYPE_INTER_8x16V:
		leftAvail |= blkId ;
		trAvail = (blkId == 0) ? topAvail : trAvail;
		blkwidth = 1;
		break;
	case MBTYPE_INTER_4V:
		leftAvail |= (blkId&1);
		topAvail  |= (blkId>>1) ;
		if (blkId == 3)
		{
			trAvail = 0;
		}else if (blkId == 2)
		{
			trAvail = 1;
		}else if (blkId == 0)
		{
			trAvail = topAvail;
		}
		blkwidth = 1;
			
		break;

	default:
        	blkwidth = 0;
	//	PRINTF ("wrong MB type!\n");
		break;
	}

	sl = pMVCache-2;
	sa = pMVCache - 4*2;
	sr = sa + blkwidth*2;

	if (!leftAvail)
	{
		((int32 *)sl)[0] = 0;
	}

	if (!topAvail)
	{
		sa = sr = sl;
	}else if (!trAvail)
	{	
		sr = sa - 2;

#if defined(RV8_SUPPORTED)
		if (g_rv_decoder_ptr->bIsRV8 && !leftAvail)
		{
			sr = sl;
		}
#endif			
	}

	mv_ptr[0] = RvDec_GetMedianofThree(sl[0], sa[0], sr[0]);
	mv_ptr[1] = RvDec_GetMedianofThree(sl[1], sa[1], sr[1]);
}

PUBLIC void RvDec_DecMV_P(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_ptr)
{
	int32 i;
	int32 mv[2];
	int32 mv_xy;
	BitStream * stream = rv_decoder_ptr->stream;
	int16 *pMVMB = mb_cache_ptr->mv_cache + (4+1)*2;   //point to current MB's mv in mv cache;
	int32 mbtype = mb_mode_ptr->uMbType;
	int32 blkoffset;
	
	switch(mbtype) 
	{
	case MBTYPE_INTER_16x16:	
	case MBTYPE_INTER:
		RvDec_MotionPred_P(mb_cache_ptr, pMVMB, mv, mbtype, 0);
		RvDec_GetMVD(stream, mv, &mv_xy);
		
		((int32 *)pMVMB) [0] = mv_xy;
		((int32 *)pMVMB) [1] = mv_xy;
		((int32 *)pMVMB) [4] = mv_xy;
		((int32 *)pMVMB) [5] = mv_xy;

		mb_cache_ptr->bUse4Mv = FALSE;
		break;

	case MBTYPE_INTER_16x8V:
		/*first MV*/
		RvDec_MotionPred_P(mb_cache_ptr, pMVMB, mv, mbtype, 0);
		RvDec_GetMVD(stream, mv, &mv_xy);

		((int32 *)pMVMB) [0] = mv_xy;
		((int32 *)pMVMB) [1] = mv_xy;

		/*second MV*/
		RvDec_MotionPred_P(mb_cache_ptr, pMVMB+4*2, mv, mbtype, 1);
		RvDec_GetMVD(stream, mv, &mv_xy);
		
		((int32 *)pMVMB) [4] = mv_xy;
		((int32 *)pMVMB) [5] = mv_xy;
		
		mb_cache_ptr->bUse4Mv = TRUE;
		break;

	case MBTYPE_INTER_8x16V:
		/*first MV*/
		RvDec_MotionPred_P(mb_cache_ptr, pMVMB, mv, mbtype, 0);
		RvDec_GetMVD(stream, mv, &mv_xy);
		
		((int32 *)pMVMB) [0] = mv_xy;
		((int32 *)pMVMB) [4] = mv_xy;
		
		/*second MV*/
		RvDec_MotionPred_P(mb_cache_ptr, pMVMB+2, mv, mbtype, 1);
		RvDec_GetMVD(stream, mv, &mv_xy);
		
		((int32 *)pMVMB) [1] = mv_xy;
		((int32 *)pMVMB) [5] = mv_xy;

		mb_cache_ptr->bUse4Mv = TRUE;
		break;

	case MBTYPE_INTER_4V:
		for (i = 0; i < 4; i++)
		{
			blkoffset = (i>>1)*4 + (i&1);
			RvDec_MotionPred_P(mb_cache_ptr, pMVMB+blkoffset*2, mv, mbtype, i);
			RvDec_GetMVD(stream, mv, &mv_xy);
			
			((int32 *)pMVMB) [blkoffset] = mv_xy;
		}

		mb_cache_ptr->bUse4Mv = TRUE;
		break;

	default:
	//	PRINTF ("wrong MB type!\n");
	        break;
	}

	/*store MV cache into */
	((int32 *)mv_ptr)[0] = ((int32 *)pMVMB) [0];
	((int32 *)mv_ptr)[1] = ((int32 *)pMVMB) [1];
	((int32 *)mv_ptr)[2] = ((int32 *)pMVMB) [4];
	((int32 *)mv_ptr)[3] = ((int32 *)pMVMB) [5];	
}

#if defined(BSLICE_SUPPORTED)
#define RvDec_StoreMV(mv_xy, pMVCache, mv_ptr)\
{\
	int32 * pIntmv;\
\
	pIntmv = (int32 *)pMVCache;\
	pIntmv [5]  = mv_xy;\
	pIntmv [6]  = mv_xy;\
	pIntmv [9]  = mv_xy;\
	pIntmv [10] = mv_xy;\
\
	pIntmv = (int32 *)mv_ptr;\
	pIntmv[0] = mv_xy;\
	pIntmv[1] = mv_xy;\
	pIntmv[2] = mv_xy;\
	pIntmv[3] = mv_xy;\
}

LOCAL void RvDec_MotionPred_B_r9(MB_CACHE_T *mb_cache_ptr, MB_MODE_T *mb_mode_ptr, MB_MODE_T *pmbmd_t, int32 mv_ptr[2], int32 mvdir)
{
	int32 lAvail = 0;
	int32 availNum = 0;
	int16 mvcand [3*2] = {0};
	int32 pmv0, pmv1, pmv2;
	int16 *pMVCache = ((mvdir == 1) ? mb_cache_ptr->mv_cache : mb_cache_ptr->bck_mv_cache) + 5*2; 

	/*left mv candidator*/
	if (mb_cache_ptr->mbAvailA && ((mb_mode_ptr-1)->mvdir & mvdir))
	{
		((int32 *)mvcand)[availNum] = ((int32 *)pMVCache)[-1];
		availNum++;
		lAvail = 1;
	}

	/*top mv candidator*/
	if (mb_cache_ptr->mbAvailB)
	{
		if (mvdir & pmbmd_t->mvdir)
		{
			((int32 *)mvcand)[availNum] = ((int32 *)pMVCache)[-4];
			availNum++;
		}
	}else
	{
		if (lAvail)
		{
			((int32 *)mvcand)[availNum] = ((int32 *)pMVCache)[-1];
			availNum++;
		}
	}

	/*tor right mv candidator*/
	if (mb_cache_ptr->mbAvailC)
	{
		if (mvdir & (pmbmd_t+1)->mvdir)
		{
			((int32 *)mvcand)[availNum] = ((int32 *)pMVCache)[-2];
			availNum++;
		}
	}else
	{
		if (mb_cache_ptr->mbAvailD)
		{
			if (mvdir & (pmbmd_t-1)->mvdir)
			{
				((int32 *)mvcand)[availNum] = ((int32 *)pMVCache)[-5];
				availNum++;
			}
		}else
		{
			if (lAvail)
			{
				((int32 *)mvcand)[availNum] = ((int32 *)pMVCache)[-1];
				availNum++;
			}
		}
	}

	switch(availNum)
	{
	case 3:
		pmv0 = mvcand[0];
		pmv1 = mvcand[2];
		pmv2 = mvcand[4];
		mv_ptr[0] = RvDec_GetMedianofThree(pmv0, pmv1, pmv2);

		pmv0 = mvcand[1];
		pmv1 = mvcand[3];
		pmv2 = mvcand[5];
		mv_ptr[1] = RvDec_GetMedianofThree(pmv0, pmv1, pmv2);
		break;

	case 2:
		mv_ptr[0] = (mvcand[0] + mvcand[2])/2;
		mv_ptr[1] = (mvcand[1] + mvcand[3])/2;
		break;

	case 1:
		mv_ptr[0] = mvcand[0];
		mv_ptr[1] = mvcand[1];
		break;

	case 0:
		mv_ptr[0] = 0;
		mv_ptr[1] = 0;
		break;

	default:
		break;
	}
}

#if defined(RV8_SUPPORTED)
PUBLIC void RvDec_DecMV_B_RV8(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *pMV_f, MOTION_VECTOR_T *pMV_b)
{
	int32 mv[2];
	int32 mv_xy;
	int32 mvdir = mb_mode_ptr->mvdir;
	BitStream * stream = rv_decoder_ptr->stream;
	MB_CACHE_T * mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	MB_MODE_T * pmbmd_t = mb_mode_ptr - rv_decoder_ptr->uMbNumX;
	int16 * pMVCache = mb_cache_ptr->mv_cache + (4+1)*2;

	switch(mb_mode_ptr->uMbType)
	{
	case MBTYPE_FORWARD:
		RvDec_MotionPred_P (mb_cache_ptr, pMVCache, mv, mb_mode_ptr->uMbType, 0);
		RvDec_GetMVD(stream, mv, &mv_xy);
		RvDec_StoreMV(mv_xy, mb_cache_ptr->mv_cache, pMV_f);
		
		mb_mode_ptr->mvdir = 1;
		mb_cache_ptr->intpDir = 1;
		break;

	case MBTYPE_BACKWARD:
		RvDec_MotionPred_P (mb_cache_ptr, pMVCache, mv, mb_mode_ptr->uMbType, 0);
		RvDec_GetMVD(stream, mv, &mv_xy);
		RvDec_StoreMV(mv_xy, mb_cache_ptr->bck_mv_cache, pMV_b);

		((int *)pMV_f)[0] = mv_xy; //for computing mvdelta
		((int *)pMV_f)[1] = mv_xy;
		((int *)pMV_f)[2] = mv_xy;
		((int *)pMV_f)[3] = mv_xy;

		mb_mode_ptr->mvdir = 2;
		mb_cache_ptr->intpDir = 2;
		break;

	case MBTYPE_BIDIR:
		/*get forward mv*/
		RvDec_MotionPred_B_r9(mb_cache_ptr, mb_mode_ptr, pmbmd_t, mv, 1);
		RvDec_GetMVD(stream, mv, &mv_xy);	
		RvDec_StoreMV(mv_xy, mb_cache_ptr->mv_cache, pMV_f);

		/*get backward mv*/
		RvDec_MotionPred_B_r9(mb_cache_ptr, mb_mode_ptr, pmbmd_t, mv, (int32)2);
		RvDec_GetMVD(stream, mv, &mv_xy);		
		RvDec_StoreMV(mv_xy, mb_cache_ptr->bck_mv_cache, pMV_b);

		mb_mode_ptr->mvdir = 3;
		mb_cache_ptr->intpDir = 3;
		break;
		
	default:
		break;
	}
}
#endif

PUBLIC void RvDec_DecMV_B_RV9(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *pMV_f, MOTION_VECTOR_T *pMV_b)
{
	int32 mv[2];
	int32 mv_xy;
	MB_CACHE_T * mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	MB_MODE_T * pmbmd_t = mb_mode_ptr - rv_decoder_ptr->uMbNumX;
	BitStream * stream = rv_decoder_ptr->stream;

	switch(mb_mode_ptr->uMbType)
	{
	case MBTYPE_FORWARD:
		RvDec_MotionPred_B_r9(mb_cache_ptr, mb_mode_ptr, pmbmd_t, mv, 1);
		RvDec_GetMVD(stream, mv, &mv_xy);
		RvDec_StoreMV(mv_xy, mb_cache_ptr->mv_cache, pMV_f);
		
		mb_mode_ptr->mvdir = 1;
		mb_cache_ptr->intpDir = 1;
		break;

	case MBTYPE_BACKWARD:
		RvDec_MotionPred_B_r9(mb_cache_ptr,	mb_mode_ptr, pmbmd_t, mv, 2);
		RvDec_GetMVD(stream, mv, &mv_xy);
		RvDec_StoreMV(mv_xy, mb_cache_ptr->bck_mv_cache, pMV_b);

		((int *)pMV_f)[0] = mv_xy; //for computing mvdelta
		((int *)pMV_f)[1] = mv_xy;
		((int *)pMV_f)[2] = mv_xy;
		((int *)pMV_f)[3] = mv_xy;

		mb_mode_ptr->mvdir = 2;
		mb_cache_ptr->intpDir = 2;
		break;

	case MBTYPE_BIDIR:
		/*get forward mv*/
		RvDec_MotionPred_B_r9(mb_cache_ptr, mb_mode_ptr, pmbmd_t, mv, 1);
		RvDec_GetMVD(stream, mv, &mv_xy);
		RvDec_StoreMV(mv_xy, mb_cache_ptr->mv_cache, pMV_f);

		/*get backward mv*/
		RvDec_MotionPred_B_r9(mb_cache_ptr, mb_mode_ptr, pmbmd_t, mv, 2);
		RvDec_GetMVD(stream, mv, &mv_xy);
		RvDec_StoreMV(mv_xy, mb_cache_ptr->bck_mv_cache, pMV_b);

		mb_mode_ptr->mvdir = 3;
		mb_cache_ptr->intpDir = 3;
		break;
		
	default:
		break;
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
