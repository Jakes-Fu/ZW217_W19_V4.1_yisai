#if defined(DBK_SUPPORTED)
#include "rvdec_video_header.h"
#endif

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#if defined(DBK_SUPPORTED)

const uint8 g_strength [32] = {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5};

void horEdge4_filter (uint8 * pPixY, int pitch, int s)
{
	int i;
	int d1, d2;
	int A, B, C, D;
	uint8 * pClip = g_rgiRvClipTab;
	
	for (i = 0; i < 4; i++)
	{
		A = pPixY[-2*pitch];
		B = pPixY[-1*pitch];
		C = pPixY[0];
		D = pPixY[1*pitch];
		
		d1 = (A - D - ((B-C)<<2)) >> 3;
		d2 = IClip (-s, s, d1);
		pPixY[-1*pitch] = pClip [B+d2];
		pPixY[0*pitch]  = pClip [C-d2];
		
		pPixY += 1;
	}
}

void vertEdge4_filter (uint8 * pPixY, int pitch, int s)
{
	int i;
	int d1, d2;
	int A, B, C, D;
	uint8 * pClip = g_rgiRvClipTab;

	for (i = 0; i < 4; i++)
	{
		A = pPixY[-2];
		B = pPixY[-1];
		C = pPixY[0];
		D = pPixY[1];

		d1 = (A - D - ((B-C)<<2)) >> 3;;
		d2 = IClip (-s, s, d1);
		pPixY[-1] = pClip [B+d2];
		pPixY[0]  = pClip [C-d2];

		pPixY += pitch;
	}
}

void rvdec_mb_vertEdge_filter_r8 (RV_DECODER_T * pDecoder, MB_MODE_T * mb_mode_ptr, uint8 ** pMBYUV)
{
	int s;
	int i, j;
	int blkidx;
	int beOnLeftEdge = 0;
	int beOnTopEdge = 0;
	int QP = mb_mode_ptr->uQp;
	int mbNumX = pDecoder->uMbNumX;
	MB_MODE_T * pTopMB, * pLeftMB, * pTLMB;
	uint8 * pPixY, * pPixU, * pPixV;
	int frameWidthY = pDecoder->uFrameExtendWidth;
	int frameWidthC = frameWidthY / 2;	

	if (pDecoder->uMbPosX == 0)
		beOnLeftEdge = 1;

	if (pDecoder->uMbPosY == 0)
		beOnTopEdge = 1;
	
	pTopMB = mb_mode_ptr - mbNumX;
	pLeftMB = mb_mode_ptr - 1;
	pTLMB = pTopMB - 1;


	s = g_strength [mb_mode_ptr->uQp];

	/********************************************************************************************
							filter Y plane of one MB
	*********************************************************************************************/
	/*first vertical edge of one MB*/
	if (!beOnLeftEdge)
	{
		pPixY = pMBYUV[0];

		for (i = 0; i < 4; i++)
		{	
			if ( (mb_mode_ptr->uCbpcoef & (1<<i*4)) || (pLeftMB->uCbpcoef & (1<<(i*4 + 3))) )
			{	
				vertEdge4_filter (pPixY, frameWidthY, s);				
			}
			
			pPixY += frameWidthY*4;
		}

	}

	for (i = 1; i < 4; i++)
	{
		pPixY = pMBYUV[0]+ i*4;

		/*the other three block4x4 edge*/
		for (j = 0; j < 4; j++)
		{
			blkidx = j*4 + i;
			if ( mb_mode_ptr->uCbpcoef & ((1<<(blkidx-1)) | (1<<blkidx)) )
			{
				vertEdge4_filter (pPixY, frameWidthY, s);
			}

			pPixY += frameWidthY * 4;
		}
	}


	/********************************************************************************************
	filter UV plane of one MB
	*********************************************************************************************/
	if (pDecoder->uPicCodeType == INTRAPIC)
	{
		/*first column of chroma*/
		if (!beOnLeftEdge)
		{	
			pPixU = pMBYUV[1];
			pPixV = pMBYUV[2];
			
			for (i = 0; i < 2; i++)
			{			
				/*U*/
				if ((mb_mode_ptr->uCbpcoef & (1<<(16+i*2))) || (pLeftMB->uCbpcoef & (1<<(16+i*2+1))))
				{
					vertEdge4_filter (pPixU, frameWidthC, s);
				}
				
				/*V*/
				if ((mb_mode_ptr->uCbpcoef & (1<<(20+i*2))) || (pLeftMB->uCbpcoef & (1<<(20+i*2+1))))
				{
					vertEdge4_filter (pPixV, frameWidthC, s);
				}
				
				pPixU += frameWidthC*4;
				pPixV += frameWidthC*4;			
			}
		}
		
		/*second column of chroma*/
		pPixU = pMBYUV[1] + 4;
		pPixV = pMBYUV[2] + 4;
		
		for (i = 0; i < 2; i++)
		{
			/*U*/
			if ( (mb_mode_ptr->uCbpcoef & (1<<(16+i*2))) || (mb_mode_ptr->uCbpcoef & (1<<(17+i*2))) )
			{
				vertEdge4_filter (pPixU, frameWidthC, s);
			}
			
			/*V*/
			if ( (mb_mode_ptr->uCbpcoef & (1<<(20+i*2))) || (mb_mode_ptr->uCbpcoef & (1<<(21+i*2))) )
			{
				vertEdge4_filter (pPixV, frameWidthC, s);
			}	
			
			pPixU += frameWidthC*4;
			pPixV += frameWidthC*4;	
		}
		
	}
}

void rvdec_mb_horEdge_filter_r8 (RV_DECODER_T *pDecoder, MB_MODE_T *mb_mode_ptr, uint8 **pMBYUV)
{
	int s;
	int i, j;
	int blkidx;
	int beOnLeftEdge = 0;
	int beOnTopEdge = 0;
	int QP = mb_mode_ptr->uQp;
	int mbNumX = pDecoder->uMbNumX;
	MB_MODE_T * pTopMB, * pLeftMB, * pTLMB;
	uint8 * pPixY, * pPixU, * pPixV;
	int frameWidthY = pDecoder->uFrameExtendWidth;
	int frameWidthC = frameWidthY / 2;	
	
	if (pDecoder->uMbPosX == 0)
		beOnLeftEdge = 1;
	
	if (pDecoder->uMbPosY == 0)
		beOnTopEdge = 1;
	
	pTopMB = mb_mode_ptr - mbNumX;
	pLeftMB = mb_mode_ptr - 1;
	pTLMB = pTopMB - 1;

	
	s = g_strength [mb_mode_ptr->uQp];

	/********************************************************************************************
							filter Y plane of one MB
	*********************************************************************************************/
	/*first row*/
	if (!beOnTopEdge)
	{	
		pPixY = pMBYUV[0] - 4;

		/*first edge4*/
		if (!beOnLeftEdge)
		{
			if ( (pTLMB->uCbpcoef & (1<<15)) || (pLeftMB->uCbpcoef & (1<<3)) )
			{
				horEdge4_filter (pPixY, frameWidthY, s);
			}
		}

		pPixY += 4;

		/*the other three edge4*/
		for (i = 0; i < 3; i++)
		{
			if ( (pTopMB->uCbpcoef & (1<<(12+i))) | (mb_mode_ptr->uCbpcoef & (1<<i)) )
			{
				horEdge4_filter (pPixY, frameWidthY, s);
			}

			pPixY += 4;
		}
	}

	/*the other 3 row*/
	for (i = 1; i < 4; i++)
	{
		pPixY = pMBYUV[0] + frameWidthY*i*4 - 4;

		/*the first edge4*/
		if (!beOnLeftEdge)
		{			
			if ( (pLeftMB->uCbpcoef & (1<<(i*4-1))) | (pLeftMB->uCbpcoef & (1<<(i*4+3))) )
			{
				horEdge4_filter (pPixY, frameWidthY, s);
			}
		}

		pPixY += 4;

		/*the other three edge4*/
		for (j = 0; j < 3; j++)
		{
			blkidx = i*4 + j;

			if ( (mb_mode_ptr->uCbpcoef & (1<<blkidx)) | (mb_mode_ptr->uCbpcoef & (1<<(blkidx-4))) )
			{
				horEdge4_filter (pPixY, frameWidthY, s);
			}
			
			pPixY += 4;
		}
	}


	/********************************************************************************************
	filter UV plane of one MB
	*********************************************************************************************/
	/*first row*/
	if (pDecoder->uPicCodeType == INTRAPIC)
	{
		if (!beOnTopEdge)
		{
			pPixU = pMBYUV [1] - 4;
			pPixV = pMBYUV [2] - 4;
			
			if (!beOnLeftEdge)
			{
				if ( (pTLMB->uCbpcoef&(1<<19)) | (pLeftMB->uCbpcoef & (1<<17)) )
				{
					horEdge4_filter (pPixU, frameWidthC, s);
				}
				
				if ( (pTLMB->uCbpcoef&(1<<23)) | (pLeftMB->uCbpcoef & (1<<21)) )
				{
					horEdge4_filter (pPixV, frameWidthC, s);
				}
			}
			
			pPixU += 4;
			pPixV += 4;
			
			if ((pTopMB->uCbpcoef & (1<<18)) | (mb_mode_ptr->uCbpcoef & (1<<16)))
			{
				horEdge4_filter (pPixU, frameWidthC, s);
			}
			
			if ((pTopMB->uCbpcoef & (1<<22)) | (mb_mode_ptr->uCbpcoef & (1<<20)))
			{
				horEdge4_filter (pPixV, frameWidthC, s);
			}
		}
		
		/*second row*/
		pPixU = pMBYUV [1] + frameWidthC*4 - 4;
		pPixV = pMBYUV [2] + frameWidthC*4 - 4;
		
		if (!beOnLeftEdge)
		{
			if ( (pLeftMB->uCbpcoef & (1<<17)) | (pLeftMB->uCbpcoef & (1<<19)))
			{
				horEdge4_filter (pPixU, frameWidthC, s);
			}
			
			if ( (pLeftMB->uCbpcoef & (1<<21)) | (pLeftMB->uCbpcoef & (1<<23)))
			{
				horEdge4_filter (pPixV, frameWidthC, s);
			}
		}
		
		pPixU += 4;
		pPixV += 4;
		if ( (mb_mode_ptr->uCbpcoef & (1<<16)) | (mb_mode_ptr->uCbpcoef & (1<<18)) )
		{
			horEdge4_filter (pPixU, frameWidthC, s);
		}
		
		if ( (mb_mode_ptr->uCbpcoef & (1<<20)) | (mb_mode_ptr->uCbpcoef & (1<<22)) )
		{
			horEdge4_filter (pPixV, frameWidthC, s);
		}
	}
	
	/*if last MB in one MB row, filtering the last horizontal edge 4*/
	if (pDecoder->uMbPosX == pDecoder->uMbNumX-1)
	{
		/*first row*/
		pPixY = pMBYUV[0] + 12;
		if (!beOnTopEdge)
		{
			if ( (pTopMB->uCbpcoef & (1<<15)) | (mb_mode_ptr->uCbpcoef & (1<<3)) )
			{
				horEdge4_filter (pPixY, frameWidthY, s);
			}
		}	
		
		pPixY += frameWidthY*4;

		for (i = 1; i < 4; i++)
		{
			blkidx = i*4 + 3;
			if ( (mb_mode_ptr->uCbpcoef & (1<<blkidx)) | (mb_mode_ptr->uCbpcoef & (1<<(blkidx - 4))) )
			{
				horEdge4_filter (pPixY, frameWidthY, s);
			}

			pPixY += frameWidthY*4;
		}
		
		/*first row of UV*/
		if (pDecoder->uPicCodeType == INTRAPIC)
		{
			pPixU = pMBYUV[1] + 4;
			pPixV = pMBYUV[2] + 4;
			if (!beOnTopEdge)
			{
				if ((pTopMB->uCbpcoef & (1<<19)) | (mb_mode_ptr->uCbpcoef & (1<<17)))
				{
					horEdge4_filter (pPixU, frameWidthC, s);
				}
				
				if ((pTopMB->uCbpcoef & (1<<23)) | (mb_mode_ptr->uCbpcoef & (1<<21)))
				{
					horEdge4_filter (pPixV, frameWidthC, s);
				}
			}
			
			/*second row of UV*/
			pPixU += frameWidthC*4;
			pPixV += frameWidthC*4;
			
			if ( (mb_mode_ptr->uCbpcoef & (1<<17)) | (mb_mode_ptr->uCbpcoef & (1<<19)) )
			{
				horEdge4_filter (pPixU, frameWidthC, s);
			}
			
			if ( (mb_mode_ptr->uCbpcoef & (1<<21)) | (mb_mode_ptr->uCbpcoef & (1<<23)) )
			{
				horEdge4_filter (pPixV, frameWidthC, s);
			}
		}
	}
}

void rvdec_deblock_mb_r8 (RV_DECODER_T * pDecoder, MB_MODE_T * mb_mode_ptr, uint8 ** pMBYUV)
{
	/*vertical edge deblocking*/
	rvdec_mb_vertEdge_filter_r8 (pDecoder, mb_mode_ptr, pMBYUV);

	/*horizontal edge deblocking*/
	rvdec_mb_horEdge_filter_r8 (pDecoder, mb_mode_ptr, pMBYUV);
}

/*****************************************************************
input:
	mv_ptr:
	    1. if skipped or direct MB type in B frame, point to backward co-located MB's motion vector difference
		2. other, point to current MB's motion vector
	mbtype: 
		if I and p frame, mbtype is current MB type, 
		if B frame, and the MB type is skipped or direct, mbtype is the co-located MB's type in backward reference frame
	return:
		return the whether deblocking flag determined by MV, and this flag is stored into cbpcoeff	
******************************************************************/
int32 set_deblock_coeff_r8 (RV_DECODER_T *pDecoder, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *mv_ptr, int32 mbtype)
{
	int32 i, b;
	int32 dx, dy;
	int32 index;
	const int32 *pLeftBlockList;
	const int32 *pAboveBlockList;
	MOTION_VECTOR_T *mv, *adj_mv;
	int32 mbNumX = pDecoder->uMbNumX;
	MB_CACHE_T * mb_cache_ptr = pDecoder->mb_cache_ptr;

	if (mbtype == MBTYPE_INTER_4V)
	{
		if (mb_cache_ptr->mbAvailA)
			pLeftBlockList = Left4VList;
		else
			pLeftBlockList = Left4VOnEdgeList;
		
		if (mb_cache_ptr->mbAvailB)
			pAboveBlockList = Above4VList;
		else
			pAboveBlockList = Above4VOnEdgeList;
	}
	else
	{
		if (mb_cache_ptr->mbAvailA)
			pLeftBlockList = Left1VList;
		else
			pLeftBlockList = Left1VOnEdgeList;
		
		if (mb_cache_ptr->mbAvailB)
			pAboveBlockList = Above1VList;
		else
			pAboveBlockList = Above1VOnEdgeList;
	}

	/*left*/	
	i = 0;
	b = pLeftBlockList[i];
	while (b >= 0)
	{
		index = (b>>3)*2 + ((b&3)>>1);   //b is the 4x4 block ID in raster scan order, and index is block 8x8 ID
		mv = mv_ptr + index;
		adj_mv = mv - ((index&1) ? 1 : 3);
		
		dx = mv->x - adj_mv->x;
		dy = mv->y - adj_mv->y;
		if (dx > 3 || dx < -3 || dy > 3 || dy < -3)
		{   
			mb_mode_ptr->uCbpcoef |= 1<<b;
			if (b & 3)
			{
				mb_mode_ptr->uCbpcoef |= 1<<(b-1);
			}
			else
			{
				(mb_mode_ptr-1)->uCbpcoef |= 1<<(b+3);
			}
		}
		
		i++;
		b = pLeftBlockList[i];
	}
	
	/*above*/
	i = 0; 
	b = pAboveBlockList [i];
	while (b >= 0)
	{
		index = (b>>3)*2 + ((b&3)>>1);
		mv = mv_ptr + index;
		adj_mv = mv - ((index < 2) ? (pDecoder->uMbNumX*4 - 2) : 2);
		
		dx = mv->x - adj_mv->x;
		dy = mv->y - adj_mv->y;
		if (dx > 3 || dx < -3 || dy > 3 || dy < -3)
		{   
			mb_mode_ptr->uCbpcoef |= 1<<(b);
			if (b > 3)
			{
				mb_mode_ptr->uCbpcoef |= 1<<(b-4);
			}
			else
			{
				(mb_mode_ptr-mbNumX)->uCbpcoef |= 1<<(b+12);
			}
		}
		
		i++;
		b = pAboveBlockList [i];
	}	

	return 0;
}
#endif //#if defined(DBK_SUPPORTED)

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 

