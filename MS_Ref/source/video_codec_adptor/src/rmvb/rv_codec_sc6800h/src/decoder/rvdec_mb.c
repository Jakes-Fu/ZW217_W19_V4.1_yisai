
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

/** * Select VLC set for decoding from current quantizer, modifier and frame type. */
__inline RV_VLC_SET_T *choose_vlc_set(int32 quant, int32 type)
{
    return type ? &g_inter_decode_tables[g_quant_to_vlc_set[1][quant]]
                : &g_intra_decode_tables[g_quant_to_vlc_set[0][quant]];
}

LOCAL uint8 RvDec_GetDQuant(BitStream *stream, uint32 uPrevQuant)
{
	int32 iDquant, iQp;
			
	if(RvDec_Read1Bits(stream))
	{
		iDquant = g_NewTAB_DQUANT_MQ_RV89[uPrevQuant][RvDec_Read1Bits(stream)];
		if (iDquant)
		{
			g_rv_decoder_ptr->error_flag = 1;
		//	PRINTF("error!, quantizer change in a frame is not allowed!\n"); //xwluo@20101223
		}
		iQp = (int32)uPrevQuant + iDquant;
	}else
	{
		iQp = RvDec_ReadBits(stream, 5);
	}

	iQp = IClip(0,31, iQp);

	return (uint8)iQp;
}

#if defined(RV8_SUPPORTED)
/* Decoder Intra prediction probabilities */
WORD_ALIGN const uint8 s_dec_iprob_rv8[162] = 
{
	0,0,					/* 1 bit */
	0,1, 1,0,				/* 3 bits */
	1,1, 0,2, 2,0, 0,3, 	/* 5 bits */
	3,0, 1,2, 2,1, 0,4,		/* 7 bits */
	4,0, 3,1, 1,3, 0,5,

	5,0, 2,2, 1,4, 4,1,		/* 9 bits */
	0,6, 3,2, 1,5, 2,3,
	5,1, 6,0, 0,7, 4,2,
	2,4, 3,3, 6,1, 1,6, 

	7,0, 0,8, 5,2, 4,3,		/* 11 bits */
	2,5, 3,4, 1,7, 4,4, 
	7,1, 8,0, 6,2, 3,5, 
	5,3, 2,6, 1,8, 2,7, 
	7,2, 8,1, 5,4, 4,5, 
	3,6, 6,3, 8,2, 4,6, 
	5,5, 6,4, 2,8, 7,3, 
	3,7, 6,5, 5,6, 7,4, 

	4,7, 8,3, 3,8, 7,5,		/* 13 bits */
	8,4, 5,7, 4,8, 6,6, 
	7,6, 5,8, 8,5, 6,7, 
	8,6, 7,7, 6,8, 8,7, 
	7,8, 8,8,
};

PUBLIC void RvDec_DecIPMode4x4_RV8(BitStream *stream, MB_CACHE_T *mb_cache_ptr, int8 *pIPCache)
{
	uint32 uRow;
	uint32 uInfo, uLen;
	int32 A, B;
	uint32 prob0, prob1;
	uint32 uTmpVal;
	
	/* Loop over 8 pairs of subblocks. One pair is decoded in each iteration. */
	for(uRow = 4; uRow > 0; uRow--)
	{
		uTmpVal = RvDec_GetVlcBits(stream, &uInfo, &uLen);
					
		//sub block 0
		B = 1 + pIPCache[-1];
		A = 1 + pIPCache[-RV_CTX_CACHE_WIDTH];

		prob0 = s_dec_iprob_rv8[uTmpVal << 1];
		prob1 = s_dec_iprob_rv8[(uTmpVal << 1) + 1];

		pIPCache[0] = g_dec_aic_prob_rv8[A*90+B*9+prob0];

		//sub block 1
		B = 1 + pIPCache[0];
		A = 1 + pIPCache[1-RV_CTX_CACHE_WIDTH];
		pIPCache[1] = g_dec_aic_prob_rv8[A*90+B*9+prob1];

		uTmpVal = RvDec_GetVlcBits(stream, &uInfo, &uLen);
				
		//sub block 2
		B = 1 + pIPCache[1];
		A = 1 + pIPCache[2-RV_CTX_CACHE_WIDTH];

		prob0 = s_dec_iprob_rv8[uTmpVal << 1];
		prob1 = s_dec_iprob_rv8[(uTmpVal << 1) + 1];

		pIPCache[2] = g_dec_aic_prob_rv8[A*90+B*9+prob0];

		//sub block 3
		B = 1 + pIPCache[2];
		A = 1 + pIPCache[3-RV_CTX_CACHE_WIDTH];
		pIPCache[3] = g_dec_aic_prob_rv8[A*90+B*9+prob1];
		pIPCache += RV_CTX_CACHE_WIDTH;			
	}
}

PUBLIC void RvDec_ReadMbtype_PB_RV8(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr)
{
	uint32 uLen, uInfo;
	uint8 uMode;
	uint8 uMbType;

	uMode = (uint8)RvDec_GetVlcBits(stream, &uInfo, &uLen);

	if(uMode > 5)
	{
		uMode -= 5;
		mb_mode_ptr->uQp = RvDec_GetDQuant(stream, rv_decoder_ptr->prevQuant);
	}

	if(INTERPIC == rv_decoder_ptr->uPicCodeType)
	{
		switch(uMode)
		{
		case 0:
			uMbType = MBTYPE_SKIPPED;
			break;
		case 1:
			uMbType = MBTYPE_INTER;
			break;
		case 2:
			uMbType = MBTYPE_INTER_4V;
			break;
		case 4:
			uMbType = MBTYPE_INTRA;
			break;
		case 5:
			uMbType = MBTYPE_INTRA_16x16;
			break;
		default:
			rv_decoder_ptr->error_flag = TRUE;
			break;
		}		
	}else  //B
	{
		switch(uMode)
		{
		case 0:
			uMbType = MBTYPE_SKIPPED;
			break;
		case 1:
			uMbType = MBTYPE_DIRECT;
			break;
		case 2:
			uMbType = MBTYPE_FORWARD;
			break;
		case 3:
			uMbType = MBTYPE_BACKWARD;
			break;
		case 4:
			uMbType = MBTYPE_INTRA;
			break;
		case 5:
			uMbType = MBTYPE_INTRA_16x16;
			break;
		default:
			rv_decoder_ptr->error_flag = TRUE;
			break;
		}		
	}
	
	mb_mode_ptr->uMbType = uMbType;
	mb_cache_ptr->bIsIntra = (uMbType < MBTYPE_INTER);
	mb_cache_ptr->intra16x16 = (MBTYPE_INTRA_16x16 == uMbType);
	mb_cache_ptr->inter16x16 = (MBTYPE_INTER_16x16 == uMbType);
	mb_cache_ptr->b16 = (mb_cache_ptr->intra16x16 || mb_cache_ptr->inter16x16);
}
#endif

#define AIC_TOP_BITS  8
#define AIC_TOP_SIZE 16

const uint8 s_aic_idx_remap[21] =
{
	0, 3, 8, 1, 4, 17, 18, 9, 2, 5, 19, 10, 13, 16, 12, 15, 6, 7, 11, 14, 20
};
PUBLIC void put_bottom2Buf (RV_DECODER_T *pDecoder, MB_CACHE_T *pMBCache, uint32 uMbX)
{
	int32 uv;
	uint8 * pFrame;
	int32 offset;
	int32 * pIntMB;
	uint8 * bottom_ptr;
	int32 * pIntFrame;	
	
	/*put Y*/
	pFrame = g_bottom_line_bfr[0] + uMbX * MB_SIZE + RV_Y_EXTEND_SIZE;
	top_left[0] = pFrame[MB_SIZE-1];//update top left pixel
	bottom_ptr = pMBCache->mbpix.pMBBfrY+(MB_SIZE-1)*MB_SIZE;
	pIntMB = (int32 *)bottom_ptr ;
	
	pIntFrame = (int32 *)pFrame;
	*pIntFrame++ = *pIntMB++;
	*pIntFrame++ = *pIntMB++;
	*pIntFrame++ = *pIntMB++;
	*pIntFrame++ = *pIntMB++;		
	
	
	offset = uMbX * MB_CHROMA_SIZE + RV_UV_EXTEND_SIZE;	
	bottom_ptr = pMBCache->mbpix.pMBBfrU + (MB_CHROMA_SIZE-1)*MB_CHROMA_SIZE;
	for (uv = 0; uv < 2; uv++)
	{
		pFrame = g_bottom_line_bfr[uv+1] + offset;
		top_left[1+uv] = pFrame[MB_CHROMA_SIZE-1];
				
		pIntMB = (int32 *)bottom_ptr;	
		pIntFrame = (int32 *)pFrame;

		*pIntFrame++ = *pIntMB++;
		*pIntFrame++ = *pIntMB++;
		
		bottom_ptr = pMBCache->mbpix.pMBBfrV + (MB_CHROMA_SIZE-1)*MB_CHROMA_SIZE;		
	}
}

PUBLIC void RvDec_DecIPMode4x4_RV9(BitStream *stream, MB_CACHE_T *mb_cache_ptr, int8 *pIPCache)
{
	int32 uRow;
	int32 pattern;
	const uint8 (*tbl)[2] = g_aic_top_vlc.table;
	int32 MBAvailB = mb_cache_ptr->mbAvailB;

	/*loop 4 row of one MB, one iteration decode four block's IP mode on the same row*/
	for (uRow = 0; uRow < 4; uRow++)
	{
		if ((!uRow) && (!MBAvailB))	/*four IP mode is coded in one codeword*/
		{
			uint32 idx;
	
		//	pattern = RvDec_HuffDecode(stream, g_aic_top_vlc.table, AIC_TOP_BITS, 1);
			idx = RvDec_ShowBits(stream, AIC_TOP_BITS);
			RvDec_FlushBits(stream, (uint32)(tbl[idx][1]));
			pattern = tbl[idx][0];

			pIPCache[0] = (int8)((pattern >> 2) & 2);
			pIPCache[1] = (int8)((pattern >> 1) & 2);
			pIPCache[2] = (int8)(pattern & 2);
			pIPCache[3] = (int8)((pattern << 1) & 2);	
		}else
		{
			int32 leftMode, trMode, tlMode;	
			int32 i, j;
			int32 v;

			for (i = 0; i < 4; i++)
			{
				/* Coefficients are read using VLC chosen by the prediction pattern
             * The first one (used for retrieving a pair of coefficients) is
             * constructed from the top, top right and left coefficients
             * The second one (used for retrieving only one coefficient) is
             * top + 10 * left.
             */
				leftMode = pIPCache [i-1];
				tlMode   = pIPCache [i-12];
				trMode   = pIPCache [i-11]; 

				if ((leftMode < 0) || (tlMode < 0) || (trMode < 0)
					|| ((leftMode != tlMode) && (tlMode != trMode) && (leftMode != trMode)) )
				{
					j = 20;				
				}else
				{
					int32 top = 20-1; 
					int32 bot = 0;
					int32 mid;
					const uint16 *aic_tbl_idx = g_aic_table_index;
					int32 flag = 20;					
					
					pattern = (leftMode<<8) + (tlMode<<4) + trMode;

					do
					{
						mid = (top+bot)>>1;///2;
						if (pattern == aic_tbl_idx[mid])
						{
							flag = mid;
							break;
						}else if (aic_tbl_idx[mid] > pattern)
						{
							top = mid - 1;
						}else
						{
							bot = mid + 1;
						}
					}while (top >= bot);

					j = s_aic_idx_remap[flag];
				}

				if ((j < 20) && (i < 3))	//pattern is found, decoding 2 coefficients
				{
					v = RvDec_HuffDecode(stream, g_aic_mode2_vlc[j].table/*, AIC_MODE2_BITS, 2*/);
					
					pIPCache [i+0] = (v*7282)>>16; //v/9;
					pIPCache [i+1] = v - pIPCache [i+0]*9;//v%9;
					i++;
				}else
				{
					uint32 idx = RvDec_ShowBits(stream, AIC_MODE1_BITS);
					uint32 flush_bits = 0;

					if ((tlMode != -1) && (leftMode != -1))
					{
					//	v = RvDec_HuffDecode(stream, g_aic_mode1_vlc[tlMode + leftMode*10].table, AIC_MODE1_BITS, 1);
						const uint8 (*tbl_tmp)[2] = g_aic_mode1_vlc[tlMode + leftMode*10].table;
						flush_bits = (uint32)(tbl_tmp[idx][1]);
						v = tbl_tmp[idx][0];
					}else
					{
						v = 0;
						switch(leftMode) 
						{
						case -1:	//code 0->1 1->0
							if (tlMode < 2)
							{
								v = (idx>>6) ^ 1;
								flush_bits = 1;
							}
							break;
						case 0:
						case 2:	//code 0->2, 1->0
							v = ((idx>>6) ^ 1 ) << 1;
							flush_bits = 1;
							break;
						default:
							break;
						}
					}
					RvDec_FlushBits(stream, flush_bits);
					pIPCache [i] = (int8)v;
				}							
			}		
		}
		pIPCache += RV_CTX_CACHE_WIDTH;    //point to next row
	}
}

PUBLIC void RvDec_ReadMbtype_PB_RV9(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr)
{
	uint32 val;
	int32 M, N, bestMode;
	int8 neighborMBHist[NUMBER_OF_MBTYPES];
	MB_MODE_T * pmbmd_top;
	
	//number of skipped MB
	if (0 == rv_decoder_ptr->iSkipModesLeft)
	{	
		uint32 uInfo, uLen;
		rv_decoder_ptr->iSkipModesLeft = RvDec_GetVlcBits(stream, &uInfo, &uLen);
	}else
	{
		rv_decoder_ptr->iSkipModesLeft--;
	}
	
	if (rv_decoder_ptr->iSkipModesLeft > 0)
	{
		mb_mode_ptr->uMbType = MBTYPE_SKIPPED;

#if 0	//replaced by #else 
		mb_cache_ptr->bIsIntra = 0;
		mb_cache_ptr->intra16x16 = 0;
		mb_cache_ptr->inter16x16 = 0;
		mb_cache_ptr->b16 = 0;
#else
	((int32 *)(&(mb_cache_ptr->bIsIntra))) [0]= 0;
#endif
		return;
	}
	
	/*get the statistic histogram*/
//	memset (neighborMBHist, 0, 12*sizeof(int8));
	((int32 *)neighborMBHist) [0]= 0;
	((int32 *)neighborMBHist) [1]= 0;
	((int32 *)neighborMBHist) [2]= 0;

	if (mb_cache_ptr->mbAvailA)
	{
		neighborMBHist [(mb_mode_ptr-1)->uMbType]++;
	}

	pmbmd_top = mb_mode_ptr - rv_decoder_ptr->uMbNumX;
	if (mb_cache_ptr->mbAvailB)
	{
		neighborMBHist [pmbmd_top->uMbType]++;
		
		if (mb_cache_ptr->mbAvailC)
		{
			neighborMBHist [(pmbmd_top+1)->uMbType]++;
		}
	}
	
	if (mb_cache_ptr->mbAvailD)
	{
		neighborMBHist [(pmbmd_top-1)->uMbType]++;
	}
	
	if (INTERPIC == rv_decoder_ptr->uPicCodeType)
	{
		neighborMBHist [MBTYPE_INTER] += neighborMBHist[MBTYPE_SKIPPED];
		neighborMBHist [MBTYPE_SKIPPED] = 0;		
	}else
	{
		neighborMBHist [MBTYPE_DIRECT] += neighborMBHist[MBTYPE_SKIPPED];
		neighborMBHist [MBTYPE_SKIPPED] = 0;
	}
	
	/*judge the most probable mb type*/
	bestMode = MBTYPE_SKIPPED;
	for (N = 0, M = 0; M < NUMBER_OF_MBTYPES; M++)
	{
		if (neighborMBHist[M] > N)
		{
			N = neighborMBHist[M];
			bestMode = M;
		}
	}

	/*vld according the most probable vld table*/
	{
		uint32 idx;	
		uint32 bits;
		const uint8 (*tbl)[2]; 
		uint32 val14;
		uint32 flush_bits;

		val14 = RvDec_ShowBits(stream, 14);
		if(INTERPIC == rv_decoder_ptr->uPicCodeType)
		{
			M = g_mbtype_p_tab[bestMode];
			tbl = g_ptype_vlc[M].table;
			bits = PTYPE_VLC_BITS;
			idx = val14 >> (14 - PTYPE_VLC_BITS);
		}else
		{
			M = g_mbtype_b_enc_tab[bestMode];
			tbl = g_btype_vlc[M].table;
			bits = BTYPE_VLC_BITS;
			idx = val14 >> (14 - BTYPE_VLC_BITS);
		}
		flush_bits = (uint32)(tbl[idx][1]);

	//	val = RvDec_HuffDecode(stream, g_ptype_vlc[M].table, PTYPE_VLC_BITS, 1);
		val = tbl[idx][0];

		if (val >= 0xff)
		{
		//	val = RvDec_HuffDecode(stream, g_ptype_vlc[M].table, PTYPE_VLC_BITS, 1);
			val14 <<= (18+bits);
			idx = val14 >> (32 - bits);
			flush_bits += (uint32)(tbl[idx][1]);
			val = tbl[idx][0];
		}
		RvDec_FlushBits(stream, flush_bits);
	}
	mb_mode_ptr->uMbType = val;

	mb_cache_ptr->bIsIntra = (val < MBTYPE_INTER);
	mb_cache_ptr->intra16x16 = (MBTYPE_INTRA_16x16 == val);
	mb_cache_ptr->inter16x16 = (MBTYPE_INTER_16x16 == val);
	mb_cache_ptr->b16 = (mb_cache_ptr->intra16x16 || mb_cache_ptr->inter16x16);
}

PUBLIC void RvDec_start_macroblock(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_ptr)
{
	int32 mb_x = rv_decoder_ptr->uMbPosX;
	int32 mb_y = rv_decoder_ptr->uMbPosY;
	int32 MBNumX = rv_decoder_ptr->uMbNumX;

	mb_mode_ptr->uQp = rv_decoder_ptr->uPQuant;
	mb_mode_ptr->uSilceNumber = rv_decoder_ptr->uSilceNumber;
	
	mb_cache_ptr->mbAvailA = (mb_x > 0) && (mb_mode_ptr->uSilceNumber == (mb_mode_ptr-1)->uSilceNumber);
	mb_cache_ptr->mbAvailB = (mb_y > 0) && (mb_mode_ptr->uSilceNumber ==  (mb_mode_ptr-MBNumX)->uSilceNumber);
//	mb_cache_ptr->mbAvailC = (mb_y > 0) && (mb_x < (MBNumX-1)) && (mb_mode_ptr->sliceNumber ==  (pmbmd_top+1)->sliceNumber);
	
	/*to comply with real's source code bug*/
	mb_cache_ptr->mbAvailC = mb_cache_ptr->mbAvailB && (mb_x < (MBNumX-1));
	mb_cache_ptr->mbAvailD = (mb_y > 0) && (mb_x > 0) && (mb_mode_ptr->uSilceNumber ==  (mb_mode_ptr-MBNumX-1)->uSilceNumber);

	if (mv_ptr != NULL)	//ONLY for P and B frame
	{
		((int32 *)mv_ptr) [0] = 0;
		((int32 *)mv_ptr) [1] = 0;
		((int32 *)mv_ptr) [2] = 0;
		((int32 *)mv_ptr) [3] = 0;
	}
}

 /**
 * number of ones in nibble minus one
 */
static const uint8 s_count_ones[16] = {
    0, 0, 0, 1, 0, 1, 1, 2, 0, 1, 1, 2, 1, 2, 2, 3
};

LOCAL uint32 RvDec_GetSuperCBP(RV_VLC_SET_T *vlc, BitStream *stream, int32 tbl_idx)
{
	int32 pattern, code;
	uint32 cbp = 0;
	int32 msk;

	code = RvDec_HuffDecode(stream, vlc->cbppattern[tbl_idx].table/*, 9, 2*/);
	pattern = code & 0xf;
	code >>= 4;

	if (pattern)
	{
		int32 ones = s_count_ones[pattern];
		const uint8 (*tbl)[2] = vlc->cbp[tbl_idx][ones].table;
		uint32 bits = vlc->cbp[tbl_idx][ones].bits;
		const int32 shifts[4] = {0, 2, 8, 10};
		const int32 *curshift = shifts;
		uint32 val32 = RvDec_ShowBits(stream, 32);
		uint32 flush_bits = 0;
		
		for (msk = 8; msk; msk >>= 1, curshift++)
		{
			if (pattern & msk)
			{
			//	cbp |= RvDec_HuffDecode(stream, vlc->cbp[tbl_idx][ones].table, vlc->cbp[tbl_idx][ones].bits, 1) << curshift[0];
				uint32 idx = val32 >> (32-bits);//RvDec_ShowBits(stream, bits);
				uint32 nbits = (uint32)(tbl[idx][1]);
				flush_bits += nbits;
				val32 <<= nbits;
				cbp |= (tbl[idx][0]<<curshift[0]);
			}
		}
		RvDec_FlushBits(stream, flush_bits);
	}
{
	const int32 cbp_msks[2] = {0x100000, 0x010000};
	const uint8 *module_tbl = modulo_three_table[code];
	int32 i, t;
	uint32 val4 = RvDec_ShowBits(stream, 4);
	uint32 flush_bits = 0;

	for (i = 0; i < 4; i++)
	{
		t = *module_tbl++;//modulo_three_table[code][i];
		if (t)
		{
			if (t == 1)
			{
				cbp |= cbp_msks[(val4>>3)&0x1] << i;
				val4 <<=1;
				flush_bits++;
			}else if (t == 2)
			{
				cbp |= 0x110000 << i;
			}
		}		
	}
	RvDec_FlushBits(stream, flush_bits);
}
	return cbp;
}

LOCAL void RvDec_GetIntraMBCtx (RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, BitStream *stream)
{
	int8 *pIPModeTop;
	int8 *pIPCache = mb_cache_ptr->IPMode4x4_cache;

	rv_decoder_ptr->cur_vlcs = choose_vlc_set(rv_decoder_ptr->uEntropyQP, 0);

	/*filling left block's IP mode into context*/
	if (mb_cache_ptr->mbAvailA)
	{	
		int32 tmp1, tmp2;	//to remove arm instruction interlock

		tmp1 = pIPCache [RV_CTX_CACHE_WIDTH*1 + 7];   //the context width is 12
		tmp2 = pIPCache [RV_CTX_CACHE_WIDTH*2 + 7];
		pIPCache [RV_CTX_CACHE_WIDTH*1 + 3] = tmp1;
		pIPCache [RV_CTX_CACHE_WIDTH*2 + 3] = tmp2;
		tmp1 = pIPCache [RV_CTX_CACHE_WIDTH*3 + 7];
		tmp2 = pIPCache [RV_CTX_CACHE_WIDTH*4 + 7];
		pIPCache [RV_CTX_CACHE_WIDTH*3 + 3] = tmp1;
		pIPCache [RV_CTX_CACHE_WIDTH*4 + 3] = tmp2;
	}else
	{
		pIPCache [RV_CTX_CACHE_WIDTH*1 + 3] = -1;
		pIPCache [RV_CTX_CACHE_WIDTH*2 + 3] = -1;
		pIPCache [RV_CTX_CACHE_WIDTH*3 + 3] = -1;
		pIPCache [RV_CTX_CACHE_WIDTH*4 + 3] = -1;
	}
	
	/*filling top block's IP mode into context*/
	pIPModeTop = rv_decoder_ptr->pIPredModeTop + rv_decoder_ptr->uMbPosX*4;
	if (mb_cache_ptr->mbAvailB)
	{
		((int32 *)pIPCache) [1] = ((int32 *)pIPModeTop) [0];
	}else 
	{
		((int32 *)pIPCache) [1] = (int32)0xffffffff;
	}

//	RvDec_GetIPredMode(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, stream);
	pIPCache += (RV_CTX_CACHE_WIDTH + 4); // point to current MB position

	if(mb_cache_ptr->b16)	//intra16x16
	{
		int32 uTmpVal;

		uTmpVal = RvDec_ReadBits(stream, 2);
		uTmpVal = uTmpVal * 0x01010101;

		((int32 *)pIPCache)[0] = uTmpVal;	//filling 16 the sub-block's prediction mode
		((int32 *)pIPCache)[3] = uTmpVal;
		((int32 *)pIPCache)[6] = uTmpVal;
		((int32 *)pIPCache)[9] = uTmpVal;

		mb_cache_ptr->i16mode = (uint8)uTmpVal;
	}else
	{
		rv_decoder_ptr->DecIPredMode4x4(stream, mb_cache_ptr, pIPCache);
	}

	/*store the bottom block's IP mode*/
	pIPCache += (RV_CTX_CACHE_WIDTH*3);
	((int32 *)pIPModeTop) [0] = ((int32 *)pIPCache) [0];
	
	mb_cache_ptr->cbp =  RvDec_GetSuperCBP(rv_decoder_ptr->cur_vlcs, stream, mb_cache_ptr->intra16x16);
#if defined(DBK_SUPPORTED)
	mb_mode_ptr->uCbpcoef = 0xffffff;
#endif
}

#if defined(RV8_SUPPORTED)
/* Mapping for QP applied to luma DC coeffients */
const uint8 s_luma_quant_DC_RV8[32] = 
{
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
	 18,19,20,21,22,22,22,23,23,23,24,24,25,25
};
#endif

/* Mapping for QP applied to luma DC coeffients */
const uint8 s_luma_intra_quant_DC[32] = 
{
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
	 17,18,18,18,19,19,19,20,20,20,22,22,22,22
};

const uint8 s_luma_inter_quant_DC[32] = 
{  
	0, 1, 2, 3, 4, 5, 6, 7,		/* 0-7 */
   8, 9,10,11,12,13,14,15,		/* 8-15 */
  16,17,18,19,20,20,21,21,		/* 16-23 */
  22,23,23,23,24,24,24,24		/* 24-31 */
};

LOCAL void RvDec_GetIntraMBCoeff(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr)
{
	int16 *pBlk;
	int32 iBlkIdx;
	int32 QDC, QP;
	int32 svlc_mbtype;
	int32 cbpmask = 1;
	int32 cbp = mb_cache_ptr->cbp;

	/********************************DC coefficient VLD and IQ and ITransform*******************/
	if(mb_cache_ptr->b16)	//intra16x16
    {
		int16 *pDC = mb_cache_ptr->Luma_DCCoeff;
		uint8 qp = mb_mode_ptr->uQp;

		RvDec_Dec4x4Block(rv_decoder_ptr, pDC, 3, 0, 16, 16); //if DC, IQ is not done in vld

	#if defined(RV8_SUPPORTED)
		if (rv_decoder_ptr->bIsRV8)
		{
			QDC = QP = (INTRAPIC == rv_decoder_ptr->uPicCodeType)? s_luma_quant_DC_RV8[qp] : qp;
		}else
	#endif	
		{
			QP = qp;
			QDC = s_luma_intra_quant_DC[qp];
		}

		RvDec_IQT4x4_Intra16x16(pDC, QDC, QP);

		svlc_mbtype = 2;
    }else
	{
		svlc_mbtype = 1;
	}

	/******************************luma coefficient VLD and IQuant****************************/
	if (cbp & 0xffff)
	{
		QP = QDC = mb_cache_ptr->luma_qp;//g_BquantTable[qp];	
		pBlk = mb_cache_ptr->LumaCoeff;
		for (iBlkIdx = 16; iBlkIdx > 0; iBlkIdx--)
	    {	
			if (cbp & cbpmask)
			{
				RvDec_Dec4x4Block(rv_decoder_ptr, pBlk, svlc_mbtype, FALSE, QDC, QP);	
			}

			cbpmask <<= 1;
			pBlk += 16;
		}
	}else
	{
		cbpmask = 0x10000;
	}

	if (mb_cache_ptr->b16)
	{
		int16 *pDC = mb_cache_ptr->Luma_DCCoeff;
		pBlk = mb_cache_ptr->LumaCoeff;
		for (iBlkIdx = 16; iBlkIdx > 0; iBlkIdx--)
		{
			pBlk [0] = *pDC++;
			pBlk += 16;
		}
	}

	/******************************chroma coefficient VLD and IQuant****************************/
	if (cbp & 0xff0000)
	{	
		QDC = mb_cache_ptr->chroma_dc_qp; //g_BquantTable [s_chroma_quant_DC [qp]];
		QP  = mb_cache_ptr->chroma_ac_qp; //g_BquantTable [s_chroma_quant    [qp]];
		pBlk = mb_cache_ptr->ChromaCoeff[0];
				
		for (iBlkIdx = 8; iBlkIdx > 0; iBlkIdx--)
		{			
			if (cbp & cbpmask)
			{
				RvDec_Dec4x4Block(rv_decoder_ptr, pBlk, 0/*!mb_cache_ptr->bIsIntra*/, 1, QDC, QP);
			}

			cbpmask <<= 1;
			pBlk += 16;
		}
	}
}

LOCAL void RvDec_GetInterMBCoeff(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr)
{
	int16 *pBlk;
	int32 iBlkIdx;
	int32 QDC, QP;
	int32 svlc_mbtype;
	int32 cbpmask = 1;
	int32 cbp = mb_cache_ptr->cbp;

	/********************************DC coefficient VLD and IQ and ITransform*******************/
	if(mb_cache_ptr->b16)
    {
		int16 *pDC = mb_cache_ptr->Luma_DCCoeff;
		uint8 qp = mb_mode_ptr->uQp;

		RvDec_Dec4x4Block(rv_decoder_ptr, pDC, 3, 0, 16, 16); //if DC, IQ is not done in vld

	#if defined(RV8_SUPPORTED)
		if (rv_decoder_ptr->bIsRV8)
		{
			QDC = QP = (INTRAPIC == rv_decoder_ptr->uPicCodeType)? s_luma_quant_DC_RV8[qp] : qp;
		}else
	#endif	
		{
			QP = qp;
			QDC = s_luma_inter_quant_DC[qp];
		}

		RvDec_IQT4x4_Intra16x16(pDC, QDC, QP);

		svlc_mbtype = 2;
    }else
	{
		if (!cbp)
		{
			return;
		}

		svlc_mbtype = 0;
	}

	/******************************luma coefficient VLD and IQuant****************************/
	if (cbp & 0xffff)
	{
		QP = QDC = mb_cache_ptr->luma_qp;//g_BquantTable[qp];	
		pBlk = mb_cache_ptr->LumaCoeff;
		for (iBlkIdx = 16; iBlkIdx > 0; iBlkIdx--)
	    {	
			if (cbp & cbpmask)
			{
				RvDec_Dec4x4Block(rv_decoder_ptr, pBlk, svlc_mbtype, FALSE, QDC, QP);	
			}

			cbpmask <<= 1;
			pBlk += 16;
		}
	}else
	{
		cbpmask = 0x10000;
	}

	if (mb_cache_ptr->b16)
	{
		int16 *pDC = mb_cache_ptr->Luma_DCCoeff;
		pBlk = mb_cache_ptr->LumaCoeff;
		for (iBlkIdx = 16; iBlkIdx > 0; iBlkIdx--)
		{
			pBlk [0] = *pDC++;
			pBlk += 16;
		}
	}

	/******************************chroma coefficient VLD and IQuant****************************/
	if (cbp & 0xff0000)
	{	
		QDC = mb_cache_ptr->chroma_dc_qp; //g_BquantTable [s_chroma_quant_DC [qp]];
		QP  = mb_cache_ptr->chroma_ac_qp; //g_BquantTable [s_chroma_quant    [qp]];
		pBlk = mb_cache_ptr->ChromaCoeff[0];
		
		if (mb_cache_ptr->b16) //inter16x16
		{
			rv_decoder_ptr->cur_vlcs =  choose_vlc_set(rv_decoder_ptr->uEntropyQP, 1);
		}
		
		for (iBlkIdx = 8; iBlkIdx > 0; iBlkIdx--)
		{			
			if (cbp & cbpmask)
			{
				RvDec_Dec4x4Block(rv_decoder_ptr, pBlk, 1/*!mb_cache_ptr->bIsIntra*/, 1, QDC, QP);
			}

			cbpmask <<= 1;
			pBlk += 16;
		}
	}
}

PUBLIC void RvDec_read_macroblock_I(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, BitStream *stream)
{
	if(RvDec_Read1Bits(stream))
	{
		mb_mode_ptr->uMbType = MBTYPE_INTRA_16x16;
#if 0	
		mb_cache_ptr->bIsIntra = TRUE;
		mb_cache_ptr->intra16x16 = TRUE;
		mb_cache_ptr->inter16x16 = FALSE;
		mb_cache_ptr->b16 = TRUE;
#else
	#if defined(RV_BIG_ENDIAN)
		((int32 *)(&(mb_cache_ptr->bIsIntra))) [0]= ((1<<24) | (1<<16) | 1);
	#else
		((int32 *)(&(mb_cache_ptr->bIsIntra))) [0]= ((1<<24) | (1<<8) | 1);
	#endif
#endif
	}else
	{
		mb_mode_ptr->uMbType = MBTYPE_INTRA;
#if 0
		mb_cache_ptr->bIsIntra = TRUE;
		mb_cache_ptr->intra16x16 = FALSE;
		mb_cache_ptr->inter16x16 = FALSE;
		mb_cache_ptr->b16 = FALSE;
#else
	#if defined(RV_BIG_ENDIAN)
		((int32 *)(&(mb_cache_ptr->bIsIntra))) [0]= (1<<24);
	#else
		((int32 *)(&(mb_cache_ptr->bIsIntra))) [0]= 1;
	#endif
#endif

#if defined(RV8_SUPPORTED)	
		if(!rv_decoder_ptr->bIsRV8)
#endif			
		{
#if _DEBUG_
                        uint32 tmp = 
#endif                        
			RvDec_Read1Bits(stream);
#if _DEBUG_
			if (!tmp)
			{
			//	PRINTF("quantizer change in a frame is not allowed!\n");
			}
#endif			
		}
	}
	
	RvDec_GetIntraMBCtx(rv_decoder_ptr, mb_cache_ptr, stream);	
	RvDec_GetIntraMBCoeff(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);	
}

LOCAL void RvDec_FillInterMBCtx(RV_DECODER_T *rv_decoder_ptr, MOTION_VECTOR_T *pMV_f, MOTION_VECTOR_T *pMV_b)
{
	MB_CACHE_T *mb_cache_ptr = rv_decoder_ptr->mb_cache_ptr;
	int32 MBNumX_X4 = rv_decoder_ptr->uMbNumX<<2;
	int8 *pIPCache = mb_cache_ptr->IPMode4x4_cache;
	int8 *pIPModeTop = rv_decoder_ptr->pIPredModeTop + rv_decoder_ptr->uMbPosX*4;
	int16 *pMVCache_f = mb_cache_ptr->mv_cache;
	
	/*set intra prediction mode to zero*/
	((int32 *)pIPCache) [4]  = 0;
	((int32 *)pIPCache) [7]  = 0;
	((int32 *)pIPCache) [10] = 0;
	((int32 *)pIPCache) [13] = 0;
	((int32 *)pIPModeTop)  [0] = 0;
	
	/*fill forward context*/
	if (mb_cache_ptr->mbAvailA)
	{
		((int32 *)pMVCache_f ) [4] = ((int32 *)(pMV_f-4)) [1];
		((int32 *)pMVCache_f ) [8] = ((int32 *)(pMV_f-4)) [3];
	}
		
	if (mb_cache_ptr->mbAvailB)
	{
		((int32 *)pMVCache_f ) [1] = ((int32 *)(pMV_f-MBNumX_X4)) [2];
		((int32 *)pMVCache_f ) [2] = ((int32 *)(pMV_f-MBNumX_X4)) [3];
		
		if (mb_cache_ptr->mbAvailC)
		{
			((int32 *)pMVCache_f ) [3] = ((int32 *)(pMV_f-MBNumX_X4 + 4)) [2];
		}
		
	//	if (mb_cache_ptr->mbAvailD)  //modified to comply with RV reference code
		if (mb_cache_ptr->mbAvailA /*&& mb_cache_ptr->mbAvailB*/)
		{
			((int32 *)pMVCache_f ) [0] = ((int32 *)(pMV_f-MBNumX_X4 - 4)) [3];
		}		
	}
	
	/*fill backward context*/
#if defined(BSLICE_SUPPORTED)
	if (pMV_b != NULL)	//if (TRUEBPIC == rv_decoder_ptr->uPicCodeType) //((mvdir&2)==2): fill backward context
	{
		int16 *pMVCache_b = mb_cache_ptr->bck_mv_cache;

		if (mb_cache_ptr->mbAvailA)
		{			
			((int32 *)pMVCache_b ) [4] = ((int32 *)(pMV_b-4)) [1];
			((int32 *)pMVCache_b ) [8] = ((int32 *)(pMV_b-4)) [3];	
		}
		
		if (mb_cache_ptr->mbAvailB)
		{
			((int32 *)pMVCache_b ) [1] = ((int32 *)(pMV_b-MBNumX_X4)) [2];
			((int32 *)pMVCache_b ) [2] = ((int32 *)(pMV_b-MBNumX_X4)) [3];
			
			if (mb_cache_ptr->mbAvailC)
			{
				((int32 *)pMVCache_b ) [3] = ((int32 *)(pMV_b-MBNumX_X4+4)) [2];
			}
		}

		if (mb_cache_ptr->mbAvailD)
		{
			((int32 *)pMVCache_b ) [0] = ((int32 *)(pMV_b-MBNumX_X4-4)) [3];
		}		
	}
#endif
}

PUBLIC void RvDec_read_macroblock_P(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, MOTION_VECTOR_T *mv_ptr, uint32 *mvd_ptr)
{
	BitStream *stream = rv_decoder_ptr->stream;
	
	g_RvDec_ReadMbtype_PB (rv_decoder_ptr, stream, mb_mode_ptr, mb_cache_ptr);
		
	if (!mb_cache_ptr->bIsIntra)
	{
		RvDec_FillInterMBCtx(rv_decoder_ptr, mv_ptr, NULL);

		if(MBTYPE_SKIPPED != mb_mode_ptr->uMbType)
		{
			RvDec_DecMV_P(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr, mv_ptr);	
			rv_decoder_ptr->cur_vlcs =  choose_vlc_set(rv_decoder_ptr->uEntropyQP, (int32)(!(mb_cache_ptr->inter16x16)));
			mb_cache_ptr->cbp =  RvDec_GetSuperCBP(rv_decoder_ptr->cur_vlcs, stream, mb_cache_ptr->inter16x16);		
#if defined(DBK_SUPPORTED)
			mb_mode_ptr->uCbpcoef = mb_cache_ptr->cbp;	
			if (MBTYPE_INTER_16x16 == mb_mode_ptr->uMbType)
			{
				mb_mode_ptr->uCbpcoef |= 0xffff;
			}
#endif			
		}else
		{	
			((int32 *)mb_cache_ptr->mv_cache) [5] = 0;
			((int32 *)mb_cache_ptr->mv_cache) [6] = 0;
			((int32 *)mb_cache_ptr->mv_cache) [9] = 0;
			((int32 *)mb_cache_ptr->mv_cache) [10] = 0;

#if defined(DBK_SUPPORTED)			
			mb_mode_ptr->uCbpcoef = 0;
#endif
			return;	//bypass deblock for fast decoding		
		}

		RvDec_GetInterMBCoeff(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);	
	}else
	{
		RvDec_GetIntraMBCtx(rv_decoder_ptr, mb_cache_ptr, stream);
		RvDec_GetIntraMBCoeff(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);	
	}

#if defined(DBK_SUPPORTED)
  	*mvd_ptr = g_RvDec_Set_Dbk_coeff (rv_decoder_ptr, mb_mode_ptr, mv_ptr, mb_mode_ptr->uMbType);			
#endif
}

#if defined(BSLICE_SUPPORTED)
PUBLIC void RvDec_read_macroblock_B(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint32 *mvd_ptr)
{
	BitStream *stream = rv_decoder_ptr->stream;

	g_RvDec_ReadMbtype_PB (rv_decoder_ptr, stream, mb_mode_ptr, mb_cache_ptr);
	
	if (!mb_cache_ptr->bIsIntra )
	{
		MOTION_VECTOR_T *pMV_f = rv_decoder_ptr->pMVFrameB_f + rv_decoder_ptr->uMbNumDec*4;
		MOTION_VECTOR_T *pMV_b = rv_decoder_ptr->pMVFrameB_b + rv_decoder_ptr->uMbNumDec*4;
				
		RvDec_FillInterMBCtx(rv_decoder_ptr, pMV_f, pMV_b);

		if (MBTYPE_SKIPPED != mb_mode_ptr->uMbType)
		{
			if(MBTYPE_DIRECT != mb_mode_ptr->uMbType)
			{
				//forward/backward/bi-prediction mode
				g_DecMV_B(rv_decoder_ptr, mb_mode_ptr, pMV_f, pMV_b);					
			}else
			{	
				//direct mode
			#if 0 //no useful code	
				mb_cache_ptr->intpDir = 3;
			#endif	
				mb_mode_ptr->mvdir = 0;
			}

			rv_decoder_ptr->cur_vlcs =  choose_vlc_set(rv_decoder_ptr->uEntropyQP,(int32)( !(mb_cache_ptr->inter16x16)));
			mb_cache_ptr->cbp = RvDec_GetSuperCBP(rv_decoder_ptr->cur_vlcs, stream, 0);
		#if defined(DBK_SUPPORTED)
			mb_mode_ptr->uCbpcoef = mb_cache_ptr->cbp;
		#endif
		}else
		{
			//skip mode
		#if 0 //no useful code
			mb_mode_ptr->uCbpcoef = 0;
			mb_cache_ptr->intpDir = 3;
		#endif
			mb_mode_ptr->mvdir = 0;
			mb_cache_ptr->cbp = 0;	
			//return; masked only for printf the debug infor for comparison
			return;	//bypass deblock for fast decoding
		}

		RvDec_GetInterMBCoeff(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);	
	}else
	{
		mb_mode_ptr->mvdir = 0;	//it will affect g_DecMV_B() function.
		RvDec_GetIntraMBCtx(rv_decoder_ptr, mb_cache_ptr, stream);
		RvDec_GetIntraMBCoeff(rv_decoder_ptr, mb_mode_ptr, mb_cache_ptr);	
	}

#if defined(DBK_SUPPORTED)
{
	int32 mbtype;
	MOTION_VECTOR_T *mv_ptr;

	if ((MBTYPE_DIRECT == mb_mode_ptr->uMbType) || (MBTYPE_SKIPPED == mb_mode_ptr->uMbType))
	{
		mbtype = rv_decoder_ptr->mb_mode_ptr[rv_decoder_ptr->uMbNumDec].uMbType;
		mv_ptr = rv_decoder_ptr->pMVFrameP+rv_decoder_ptr->uMbNumDec*4;
	}else
	{
		mbtype = mb_mode_ptr->uMbType;
		mv_ptr = pMV_f;
	}

	*mvd_ptr = g_RvDec_Set_Dbk_coeff (rv_decoder_ptr, mb_mode_ptr, mv_ptr, mbtype);
}
#endif
}
#endif

const uint8 s_MBLumaAddrMap[16] = 
{
	16 *0+0, 	16 *0+4, 	16 *0+8,	16 *0+12,
	16 *4+0, 	16 *4+4,   	16 *4+8, 	16 *4+12, 
	16 *8+0, 	16 *8+4, 	16 *8+8,	16 *8+12, 
	16 *12+0, 	16 *12+4,  	16 *12+8, 	16 *12 + 12, 
};
const uint8 s_MBLumaAddrMap_d[16] = 
{
	8 *0+0, 		8 *0+2, 		8 *0+4,		8 *0+6,
	8 *2+0, 		8 *2+2,   		8 *2+4, 	8 *2+6, 
	8 *4+0, 		8 *4+2, 		8 *4+4,		8 *4+6, 
	8 *6+0, 		8 *6+2,  		8 *6+4, 	8 *6+6, 
};
const uint8 s_MBChromaAddrMap[4] = 
{
	8 *0+0, 		8 *0+4, 		
	8 *4+0, 		8 *4+4,   	
};

const uint8 s_MBChromaAddrMap_d[4] = 
{
	4 *0+0, 		4 *0+2, 		
	4 *2+0, 		4 *2+2,   	
};
/*block order map from decoder order to context cache order*/
const uint8 s_blockOrderMap [16] = {
	1*12+4, 	1*12+5, 	1*12+6,	1*12+7,  //first one block8x8
	2*12+4, 	2*12+5, 	2*12+6, 	2*12+7,
	3*12+4, 	3*12+5, 	3*12+6, 	3*12+7,
	4*12+4, 	4*12+5, 	4*12+6, 	4*12+7
};

const uint8 chroma_block_address[4] = {0,2,8,10};
PUBLIC void RvDec_recon_intraMB (RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3])
{
	const uint8 *pMbLumaAddr = s_MBLumaAddrMap;
	const uint8 *pMbChromaAddr = s_MBChromaAddrMap;
	uint32 frameWidth = rv_decoder_ptr->uFrameExtendWidth;
	int32 cbp = mb_cache_ptr->cbp;
	int32 cbpmask = 1;
	uint8 *pPred; 
	int16 *coeff;
	uint8 *pLeftPix, *pTopPix;
	uint32 uv;
	uint32 iBlkIdxInMB;

	if (!mb_cache_ptr->intra16x16)
	{
	    int32 blkStrOdr;
		int32 intraPredMode;
		int8 *pI4PredMode = mb_cache_ptr->IPMode4x4_cache;
	    const uint8 *pBlkOrderMap = s_blockOrderMap;
		Intra4x4Pred_func *intraPred4x4 = g_RvIPred4x4;
		int32 pitch;
		
		coeff = mb_cache_ptr->LumaCoeff;
	    for(iBlkIdxInMB = 0; iBlkIdxInMB < 16; iBlkIdxInMB++)
	    {
			pPred = mb_cache_ptr->mbpix.pMBBfrY + pMbLumaAddr[iBlkIdxInMB];

			blkStrOdr = pBlkOrderMap[iBlkIdxInMB];
			intraPredMode = pI4PredMode[blkStrOdr];

			if (iBlkIdxInMB < 4)
			{
				pTopPix = ppxlcRecMBYUV[0] + iBlkIdxInMB*4 - frameWidth;
			}else
			{
				pTopPix = pPred - MB_SIZE;
			}

			if ((iBlkIdxInMB & 3) == 0)
			{
				pLeftPix = ppxlcRecMBYUV[0] + (iBlkIdxInMB >> 2) * frameWidth * 4 - 1;
				pitch = frameWidth;			
			}else
			{
				pLeftPix = pPred - 1;
				pitch = MB_SIZE;
			}
			
			intraPred4x4[intraPredMode](pPred, pTopPix, pLeftPix, 0, pitch, iBlkIdxInMB, mb_cache_ptr);
			
			if (cbp & cbpmask)
			{
				ITRANS_MC4X4(pPred, coeff, MB_SIZE);
			}

			cbpmask <<= 1;
			coeff += 16;
	    }	

		frameWidth = frameWidth >> 1;

		for(uv = 0; uv < 2; uv++)
		{
			coeff = mb_cache_ptr->ChromaCoeff[uv];

			for (iBlkIdxInMB = 0; iBlkIdxInMB < 4; iBlkIdxInMB++)
			{
		       	 pPred = mb_cache_ptr->mbpix.pMBBfrU + uv * 64 + pMbChromaAddr[iBlkIdxInMB];

		       	 blkStrOdr = pBlkOrderMap[chroma_block_address[iBlkIdxInMB]];
				intraPredMode = pI4PredMode[blkStrOdr];
				
				if (iBlkIdxInMB < 2)
				{
					pTopPix = ((uv == 0) ? ppxlcRecMBYUV[1] : ppxlcRecMBYUV[2]) + iBlkIdxInMB*4 - frameWidth;
				}else
				{
					pTopPix = pPred - BLOCK_SIZE;
				}

				if ((iBlkIdxInMB & 1) == 0)
				{
					pLeftPix = ((uv == 0) ? ppxlcRecMBYUV[1] : ppxlcRecMBYUV[2]) + (iBlkIdxInMB>> 1)*4*frameWidth - 1;
					pitch = frameWidth;
				}else
				{
					pLeftPix = pPred - 1;
					pitch = BLOCK_SIZE;
				}

				intraPred4x4[intraPredMode](pPred, pTopPix, pLeftPix, 1, pitch, iBlkIdxInMB, mb_cache_ptr);

				if (cbp & cbpmask)
				{
					ITRANS_MC4X4 (pPred, coeff, MB_CHROMA_SIZE);
				}
				
				cbpmask <<= 1;
				coeff += 16;
			}
		}
	}else
	{
		uint8 *pred;

		/*Y*/
		pTopPix = ppxlcRecMBYUV[0] - frameWidth;
		pLeftPix = ppxlcRecMBYUV[0] - 1;
		pPred = mb_cache_ptr->mbpix.pMBBfrY;
		g_intraPredLuma16x16[mb_cache_ptr->i16mode](pTopPix, pLeftPix, pPred, frameWidth, mb_cache_ptr);

		pPred = mb_cache_ptr->mbpix.pMBBfrY;
		coeff = mb_cache_ptr->LumaCoeff;
		for (iBlkIdxInMB = 0; iBlkIdxInMB < 16; iBlkIdxInMB++)
		{
			pred = pPred + pMbLumaAddr[iBlkIdxInMB];
			
			if (cbp & cbpmask)
			{	
				ITRANS_MC4X4 (pred, coeff, MB_SIZE);
			}else //if (mb_cache_ptr->b16)
			{
				RvDec_ITrans_MC_blk4x4_onlyDC (pred, *coeff);
			}

			cbpmask <<= 1;
			coeff += 16;
		}

		/*U and V*/
		frameWidth = frameWidth >> 1;
		for(uv = 0; uv < 2; uv++)
		{
			pTopPix = ppxlcRecMBYUV[1+uv] - frameWidth;
			pLeftPix = ppxlcRecMBYUV[1+uv] - 1;
			pPred = mb_cache_ptr->mbpix.pMBBfrU + uv * 64;
			g_intraPredChroma8x8[mb_cache_ptr->i16mode] (pTopPix, pLeftPix, pPred, frameWidth, mb_cache_ptr);

			if (cbp & (0xf<<(16+uv*4)))
			{
				coeff = mb_cache_ptr->ChromaCoeff[uv];	

				for (iBlkIdxInMB = 0; iBlkIdxInMB < 4; iBlkIdxInMB++)	
				{
					if (cbp & cbpmask)
					{
						pred = pPred + pMbChromaAddr[iBlkIdxInMB];
						ITRANS_MC4X4 (pred, coeff, BLOCK_SIZE);
					}

					cbpmask <<= 1;
					coeff += 16;
				}
			}else
			{
				cbpmask <<= 4;
			}
		}
	}	
}

PUBLIC void copyLeftPredPixel(MB_CACHE_T *pMBCache)
#if !defined(ITRANS_ASSEMBLY)
{
	int32 i, uv;
	uint8 * pSrc, *pDst;

	/*copy left border pixel*/
	pSrc = pMBCache->mbpix.pMBBfrY + 15;
	pDst = pMBCache->leftPredPix_Y;
	for (i = 0; i < 4; i++)
	{
		*pDst++ = pSrc[0];
		*pDst++ = pSrc[MB_SIZE];
		*pDst++ = pSrc[MB_SIZE * 2];
		*pDst++ = pSrc[MB_SIZE * 3];
		
		pSrc += MB_SIZE * 4;
	}

	for (uv = 0; uv < 2; uv++)
	{
		pSrc = pMBCache->mbpix.pMBBfrU + uv*64 + MB_CHROMA_SIZE - 1;
		pDst = pMBCache->leftPredPix_UV [uv];	
		for (i = 0; i < 2; i++)
		{
			*pDst++ = pSrc[MB_CHROMA_SIZE * 0];	
			*pDst++ = pSrc[MB_CHROMA_SIZE * 1];
			*pDst++ = pSrc[MB_CHROMA_SIZE * 2];
			*pDst++ = pSrc[MB_CHROMA_SIZE * 3];		
			pSrc += MB_CHROMA_SIZE * 4; 
		}
	}
}
#else
;
#endif

PUBLIC void Down_Sample_Mb(MB_CACHE_T *mb_cache_ptr)
#if !defined(_ARM_DOWNSIZE_)
{
	uint8 *src, *dst, *src1, *dst1;
	int32	i;

	//y
	src = mb_cache_ptr->mbpix.pMBBfrY + (MB_SIZE + 1);
	dst = mb_cache_ptr->mbpix_d.pMBBfrY;

	for (i = MB_SIZE_D; i >0 ; i--)
	{
		dst[0] = src[0];	dst[1] = src[2];	dst[2] = src[4];		dst[3] = src[6];
		dst[4] = src[8];	dst[5] = src[10];	dst[6] = src[12];		dst[7] = src[14];			

		src += (MB_SIZE<<1);		dst += MB_SIZE_D;	
	}

	//u
	src = mb_cache_ptr->mbpix.pMBBfrU + (MB_CHROMA_SIZE + 1);
	dst = mb_cache_ptr->mbpix_d.pMBBfrU;
	//v
	src1 = mb_cache_ptr->mbpix.pMBBfrV + (MB_CHROMA_SIZE + 1);
	dst1 = mb_cache_ptr->mbpix_d.pMBBfrV;

	for (i = MB_CHROMA_SIZE_D; i >0 ; i--)
	{
		dst[0] = src[0];	dst[1] = src[2];	dst[2] = src[4];	dst[3] = src[6];	src += (MB_CHROMA_SIZE<<1);	dst += MB_CHROMA_SIZE_D;	
		dst1[0] = src1[0];	dst1[1] = src1[2];	dst1[2] = src1[4];	dst1[3] = src1[6];	src1 += (MB_CHROMA_SIZE<<1);	dst1 += MB_CHROMA_SIZE_D;	
	}
}
#else
;
#endif

PUBLIC void RvDec_recon_intraMB_d (RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3])
{
	const uint8 *pMbLumaAddr = s_MBLumaAddrMap;
	const uint8 *pMbChromaAddr = s_MBChromaAddrMap;
	uint32 frameWidth = rv_decoder_ptr->uFrameExtendWidth_d;
	int32 cbp = mb_cache_ptr->cbp;
	int32 cbpmask = 1;
	uint8 *pPred; 
	int16 *coeff;
	uint8 *pLeftPix, *pTopPix;
	uint32 uv;
	uint32 iBlkIdxInMB;
	uint8 ** bottom_line_buf = g_bottom_line_bfr;
	uint8 * pUpscale_topleft;
	int32 MbPos_x = rv_decoder_ptr->uMbPosX;

	//Y
	pUpscale_topleft = bottom_line_buf[0] + RV_Y_EXTEND_SIZE + MbPos_x * MB_SIZE/* - 1*/;
	//load current mb's bordery data
	memcpy(luma_top, pUpscale_topleft, 16+4);
	luma_top[-1] = top_left[0];

	//U
	pUpscale_topleft = bottom_line_buf[1] + RV_UV_EXTEND_SIZE + MbPos_x * MB_CHROMA_SIZE/* - 1*/;	
	memcpy (chroma_top[0], pUpscale_topleft, 8+4);
	chroma_top[0][-1] =  top_left[1+0];

	//V
	pUpscale_topleft = bottom_line_buf[2] + RV_UV_EXTEND_SIZE + MbPos_x * MB_CHROMA_SIZE/* - 1*/;	
	memcpy (chroma_top[1], pUpscale_topleft, 8+4);
	chroma_top[1][-1] =  top_left[1+1];

	if (MbPos_x > 0)
	{
		copyLeftPredPixel (mb_cache_ptr);
	}

	if (!mb_cache_ptr->intra16x16)
	{
	    int32 blkStrOdr;
		int32 intraPredMode;
		int8 *pI4PredMode = mb_cache_ptr->IPMode4x4_cache;
	    const uint8 *pBlkOrderMap = s_blockOrderMap;
		// Intra4x4Pred_func *intraPred4x4 = g_RvIPred4x4_d;
		Intra4x4Pred_func *intraPred4x4 = g_RvIPred4x4;
		int32 pitch;
		
		coeff = mb_cache_ptr->LumaCoeff;
	    for(iBlkIdxInMB = 0; iBlkIdxInMB < 16; iBlkIdxInMB++)
	    {
			pPred = mb_cache_ptr->mbpix.pMBBfrY + pMbLumaAddr[iBlkIdxInMB];

			blkStrOdr = pBlkOrderMap[iBlkIdxInMB];
			intraPredMode = pI4PredMode[blkStrOdr];

			if (iBlkIdxInMB < 4)
			{
				pTopPix = luma_top + iBlkIdxInMB*4;
			}
			else
			{
				pTopPix = pPred - MB_SIZE;
			}	
			
			if ((iBlkIdxInMB & 3) == 0)
			{
				pLeftPix = mb_cache_ptr->leftPredPix_Y + iBlkIdxInMB ;
				pitch = 1;			
			}else
			{
				pLeftPix = pPred - 1;
				pitch = MB_SIZE;
			}
			
			//intraPred4x4[intraPredMode](pPred, pTopPix, mb_cache_ptr->leftPredPix_Y, 0,pitch, iBlkIdxInMB,mb_cache_ptr);
			intraPred4x4[intraPredMode](pPred, pTopPix, pLeftPix, 0,pitch, iBlkIdxInMB,mb_cache_ptr);

			if (cbp & cbpmask)
			{
				ITRANS_MC4X4(pPred, coeff, MB_SIZE);
			}

			cbpmask <<= 1;
			coeff += 16;
	    }	

		frameWidth = frameWidth >> 1;

		for(uv = 0; uv < 2; uv++)
		{
			coeff = mb_cache_ptr->ChromaCoeff[uv];

			for (iBlkIdxInMB = 0; iBlkIdxInMB < 4; iBlkIdxInMB++)
			{
		       	 pPred = mb_cache_ptr->mbpix.pMBBfrU + uv * 64 + pMbChromaAddr[iBlkIdxInMB];

		       	 blkStrOdr = pBlkOrderMap[chroma_block_address[iBlkIdxInMB]];
				intraPredMode = pI4PredMode[blkStrOdr];
				
				if (iBlkIdxInMB < 2)
				{
					pTopPix = chroma_top[uv] + iBlkIdxInMB*4;
				}
				else
				{
					pTopPix = pPred - BLOCK_SIZE;
				}
				if ((iBlkIdxInMB & 1) == 0)
				{
					pLeftPix = mb_cache_ptr->leftPredPix_UV[uv] + (iBlkIdxInMB<<1);
					pitch = 1;			
				}else
				{
					pLeftPix = pPred - 1;
					pitch = BLOCK_SIZE;
				}

				//intraPred4x4[intraPredMode](pPred, pTopPix, mb_cache_ptr->leftPredPix_UV[uv], 1, pitch, iBlkIdxInMB,mb_cache_ptr);
				intraPred4x4[intraPredMode](pPred, pTopPix, pLeftPix, 1, pitch, iBlkIdxInMB,mb_cache_ptr);

				
				if (cbp & cbpmask)
				{
					ITRANS_MC4X4 (pPred, coeff, MB_CHROMA_SIZE);
				}
				
				cbpmask <<= 1;
				coeff += 16;
			}
		}
	}else
	{
		uint8 *pred;

		/*Y*/
		pPred = mb_cache_ptr->mbpix.pMBBfrY;
		// g_intraPredLuma16x16_d[mb_cache_ptr->i16mode](luma_top, mb_cache_ptr->leftPredPix_Y, pPred, frameWidth,mb_cache_ptr);
		g_intraPredLuma16x16[mb_cache_ptr->i16mode](luma_top, mb_cache_ptr->leftPredPix_Y, pPred, 1,mb_cache_ptr);
		pPred = mb_cache_ptr->mbpix.pMBBfrY;
		coeff = mb_cache_ptr->LumaCoeff;
		for (iBlkIdxInMB = 0; iBlkIdxInMB < 16; iBlkIdxInMB++)
		{
			pred = pPred + pMbLumaAddr[iBlkIdxInMB];
			
			if (cbp & cbpmask)
			{	
				ITRANS_MC4X4 (pred, coeff, MB_SIZE);
			}else //if (mb_cache_ptr->b16)
			{
				RvDec_ITrans_MC_blk4x4_onlyDC (pred, *coeff);
			}

			cbpmask <<= 1;
			coeff += 16;
		}

		/*U and V*/
		frameWidth = frameWidth >> 1;
		for(uv = 0; uv < 2; uv++)
		{
			pPred = mb_cache_ptr->mbpix.pMBBfrU + uv * 64;
			//g_intraPredChroma8x8_d[mb_cache_ptr->i16mode] (chroma_top[uv], mb_cache_ptr->leftPredPix_UV[uv], pPred, frameWidth,mb_cache_ptr);
			g_intraPredChroma8x8[mb_cache_ptr->i16mode] (chroma_top[uv], mb_cache_ptr->leftPredPix_UV[uv], pPred, 1,mb_cache_ptr);
			if (cbp & (0xf<<(16+uv*4)))
			{
				coeff = mb_cache_ptr->ChromaCoeff[uv];	

				for (iBlkIdxInMB = 0; iBlkIdxInMB < 4; iBlkIdxInMB++)	
				{
					if (cbp & cbpmask)
					{
						pred = pPred + pMbChromaAddr[iBlkIdxInMB];
						ITRANS_MC4X4 (pred, coeff, BLOCK_SIZE);
					}

					cbpmask <<= 1;
					coeff += 16;
				}
			}else
			{
				cbpmask <<= 4;
			}
		}
	}	

	//downsizing output mb form 16x16 to 8x8
	Down_Sample_Mb(mb_cache_ptr);

}

/****************************************************
description:
	inverse transform the dct coefficient,
	and add the residula to the prediction
****************************************************/
LOCAL void RvDec_AddPredAndResidual_Mb(MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint32 cbp)
{
	int16 *coeff;
	uint8 *pred;
	uint32 cbpmask = 1;
	int32 iBlkIdxInMB;
	uint8 *pPred = mb_cache_ptr->mbpix.pMBBfrY;
	const uint8 *pMbLumaAddr = s_MBLumaAddrMap;

	coeff = mb_cache_ptr->LumaCoeff;
	for (iBlkIdxInMB = 0; iBlkIdxInMB < 16; iBlkIdxInMB++)
	{
		pred = pPred + pMbLumaAddr[iBlkIdxInMB];
		
		if (cbp & cbpmask)
		{
			ITRANS_MC4X4 (pred, coeff, MB_SIZE);
		}else if (mb_cache_ptr->b16)
		{
			RvDec_ITrans_MC_blk4x4_onlyDC (pred, coeff[0]);
		}

		cbpmask <<= 1;
		coeff += 16;
	}	
	
	{
		int32 uv;
		const uint8 *pMbChromaAddr = s_MBChromaAddrMap;

		for(uv = 0; uv < 2; uv++)
		{	
			if (cbp & (0xf<<(16+uv*4)))
			{
				pPred = mb_cache_ptr->mbpix.pMBBfrU + uv * 64;	
				coeff = mb_cache_ptr->ChromaCoeff[uv];	

				for (iBlkIdxInMB = 0; iBlkIdxInMB < 4; iBlkIdxInMB++)	
				{
					if (cbp & cbpmask)
					{
						pred = pPred + pMbChromaAddr[iBlkIdxInMB];
						ITRANS_MC4X4 (pred, coeff, BLOCK_SIZE);
					}

					cbpmask <<= 1;
					coeff += 16;
				}
			}else
			{
				cbpmask <<= 4;
			}
		}
	}
}

LOCAL void RvDec_AddPredAndResidual_Mb_d(MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr, uint32 cbp)
{
	int16 *coeff;
	uint8 *pred;
	uint32 cbpmask = 1;
	int32 iBlkIdxInMB;
	uint8 *pPred = mb_cache_ptr->mbpix_d.pMBBfrY;
	const uint8 *pMbLumaAddr = s_MBLumaAddrMap_d;

	coeff = mb_cache_ptr->LumaCoeff;
	for (iBlkIdxInMB = 0; iBlkIdxInMB < 16; iBlkIdxInMB++)
	{
		pred = pPred + pMbLumaAddr[iBlkIdxInMB];
		
		if (cbp & cbpmask)
		{
			rvdec_ITrans_MC_blk4x4_d (pred, coeff, MB_SIZE/2);
		}else if (mb_cache_ptr->b16)
		{
			rvdec_ITrans_MC_blk4x4_onlyDC_d (pred, coeff[0]);
		}

		cbpmask <<= 1;
		coeff += 16;
	}	
	
	{
		int32 uv;
		const uint8 *pMbChromaAddr = s_MBChromaAddrMap_d;

		for(uv = 0; uv < 2; uv++)
		{	
			if (cbp & (0xf<<(16+uv*4)))
			{
				pPred = (uv==0) ? mb_cache_ptr->mbpix_d.pMBBfrU : mb_cache_ptr->mbpix_d.pMBBfrV;	
				coeff = mb_cache_ptr->ChromaCoeff[uv];	

				for (iBlkIdxInMB = 0; iBlkIdxInMB < 4; iBlkIdxInMB++)	
				{
					if (cbp & cbpmask)
					{
						pred = pPred + pMbChromaAddr[iBlkIdxInMB];
						rvdec_ITrans_MC_blk4x4_d (pred, coeff, BLOCK_SIZE/2);
					}

					cbpmask <<= 1;
					coeff += 16;
				}
			}else
			{
				cbpmask <<= 4;
			}
		}
	}
}

PUBLIC void RvDec_recon_interMB_P(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr)
{
	uint8 *pRef;
	int32 dx, dy;
	int32 mv_x, mv_y;
    int32 iRefPosX, iRefPosY;
	uint8 *pPred = mb_cache_ptr->mbpix.pMBBfrY; 
	int16 *pMVMB = mb_cache_ptr->mv_cache + (4+1)*2;   //point to current MB's mv in mv cache;
	uint8 **Frame = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV;
	int32 uExdFrameWidth =  rv_decoder_ptr->uFrameExtendWidth;
	int32 iMbPosX = (rv_decoder_ptr->uMbPosX * MB_SIZE + RV_Y_EXTEND_SIZE);
	int32 iMbPosY = (rv_decoder_ptr->uMbPosY * MB_SIZE + RV_Y_EXTEND_SIZE);
	uint32 uOffset;

	if(!mb_cache_ptr->bUse4Mv) ///1 mv, same as RvDec_MC_oneDirection_MB()
	{
		mc_func *mc_ptr = g_RvDec_Luma16x16_MC;

		iRefPosX = iMbPosX;
		mv_x = (*pMVMB);
		GET_MC_POS(mv_x, iRefPosX, dx);

		iRefPosY = iMbPosY;
		mv_y = (*(pMVMB + 1));	
		GET_MC_POS(mv_y, iRefPosY, dy);

		pRef = Frame[0] + iRefPosY * uExdFrameWidth + iRefPosX;
		mc_ptr[(dx<<2)|dy](pRef, pPred, uExdFrameWidth);

		//chroma
		mc_ptr = g_RvDec_Chroma8x8_MC;
		uExdFrameWidth >>= 1;
		iRefPosX = (iMbPosX>>1);
		mv_x /= 2;
		GET_MC_POS(mv_x, iRefPosX, dx);

		iRefPosY = (iMbPosY>>1);
		mv_y /= 2;
		GET_MC_POS(mv_y, iRefPosY, dy);
		
		uOffset = iRefPosY * uExdFrameWidth + iRefPosX;
		pRef = Frame[1] + uOffset;
		pPred = mb_cache_ptr->mbpix.pMBBfrU;
		mc_ptr[(dx<<2)|dy](pRef, pPred, uExdFrameWidth);
		
		pRef = Frame[2] + uOffset;
		pPred = mb_cache_ptr->mbpix.pMBBfrV;
		mc_ptr[(dx<<2)|dy](pRef, pPred, uExdFrameWidth);
	}else ///4mv
	{
		int32 uBlkIdx;
		uint8 *pPred_blk;  //for 4mv
		int16 *mv_ptr;
		uint8 *pPredU = mb_cache_ptr->mbpix.pMBBfrU; 
		uint8 *pPredV = mb_cache_ptr->mbpix.pMBBfrV; 
		mc_func *luma8x8_mc = g_RvDec_Luma8x8_MC;
		mc_func *chroma4x4_mc = g_RvDec_Chroma4x4_MC;
	
		for(uBlkIdx = 0; uBlkIdx < 4; uBlkIdx++)
		{
			int32 xoffset, yoffset;

			xoffset = (uBlkIdx & 1) * 8;
			yoffset = (uBlkIdx >> 1) * 8;
			
			iRefPosX = (iMbPosX + xoffset);
			mv_ptr = pMVMB + yoffset + (xoffset>>2);//((uBlkIdx>>1)*4 + (uBlkIdx&1)) * 2;
			mv_x = *mv_ptr;
			GET_MC_POS(mv_x, iRefPosX, dx);
			
			iRefPosY = (iMbPosY + yoffset);
			mv_y = *(mv_ptr+1);
			GET_MC_POS(mv_y, iRefPosY, dy);
			
			pRef = Frame[0] + iRefPosY * uExdFrameWidth + iRefPosX;	
			pPred_blk = pPred + yoffset * MB_SIZE + xoffset;
			luma8x8_mc[(dx<<2)|dy](pRef, pPred_blk, uExdFrameWidth);

		//chroma
			mv_x /= 2;
			iRefPosX = ((iMbPosX + xoffset)>>1);
			GET_MC_POS(mv_x, iRefPosX, dx);

			mv_y /= 2;
			iRefPosY = ((iMbPosY + yoffset)>>1);
			GET_MC_POS(mv_y, iRefPosY, dy);
			
			uOffset = iRefPosY * (uExdFrameWidth>>1) + iRefPosX;

			//u
			pRef = Frame[1] + uOffset;
			pPred_blk = pPredU + yoffset * 4 + (xoffset>>1);
			chroma4x4_mc[(dx<<2)|dy](pRef, pPred_blk, (uExdFrameWidth>>1));

			//V
			pRef = Frame[2] + uOffset;
			pPred_blk = pPredV + yoffset * 4 + (xoffset>>1);
			chroma4x4_mc[(dx<<2)|dy](pRef, pPred_blk, (uExdFrameWidth>>1));
		}
	}
	
	if (mb_cache_ptr->cbp || mb_cache_ptr->inter16x16)
	{
		RvDec_AddPredAndResidual_Mb(mb_mode_ptr, mb_cache_ptr, mb_cache_ptr->cbp);
	}
}

PUBLIC void Up_Scaler_ppred(MB_CACHE_T * mb_cache_ptr)
#if !defined(_ARM_MC_ASSEMBLY_)
{
	uint8 *pred, *pred_d;
	int32 uv;
	int32 t0, t1, t2, t3, t4, t5, t6, t7;

//luma	
	// up scale bottom line
	pred = mb_cache_ptr->mbpix.pMBBfrY + (MB_SIZE-1)*MB_SIZE;
	pred_d = mb_cache_ptr->mbpix_d.pMBBfrY + (MB_SIZE_D-1)*MB_SIZE_D;
	
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
	t7 = *pred_d; pred_d -= MB_SIZE_D;
	*pred++ = t5;
	*pred++ = ((-1)*t4 + 9*t5 + 9*t6 + (-1)*t7 + 8) >> 4;

	//12, 13
	*pred++ = t6;
	*pred++ = ((-1)*t5 + 9*t6 + 9*t7 + (-1)*t7 + 8) >> 4;

	//14, (pix15 in left line)
	*pred++ = t7;

	// up scale left line
	t6 = *pred_d;pred_d -= MB_SIZE_D;
	
	//15, 14
	*pred = ((-1)*t6 + 9*t7 + 9*t7 + (-1)*t7 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t7;	pred -= MB_SIZE;

	//13, 12
	t5 = *pred_d;pred_d -= MB_SIZE_D;
	*pred = ((-1)*t5 + 9*t6 + 9*t7 + (-1)*t7 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t6;	pred -= MB_SIZE;
	
	//11, 10
	t4 = *pred_d;pred_d -= MB_SIZE_D;
	*pred = ((-1)*t4 + 9*t5 + 9*t6 + (-1)*t7 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t5;	pred -= MB_SIZE;

	//9, 8
	t3 = *pred_d;pred_d -= MB_SIZE_D;
	*pred = ((-1)*t3 + 9*t4 + 9*t5 + (-1)*t6 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t4;	pred -= MB_SIZE;

	//7, 6
	t2 = *pred_d;pred_d -= MB_SIZE_D;
	*pred = ((-1)*t2 + 9*t3 + 9*t4 + (-1)*t5 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t3;	pred -= MB_SIZE;

	//5, 4
	t1 = *pred_d;pred_d -= MB_SIZE_D;
	*pred = ((-1)*t1 + 9*t2 + 9*t3 + (-1)*t4 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t2;	pred -= MB_SIZE;

	//3, 2
	t0 = *pred_d;
	*pred = ((-1)*t0 + 9*t1 + 9*t2 + (-1)*t3 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t1;	pred -= MB_SIZE;
	
	//1, 0
	*pred = ((-1)*t0 + 9*t0 + 9*t1 + (-1)*t2 + 8) >> 4;	pred -= MB_SIZE;
	*pred = t0;


//chroma
	for (uv = 0; uv < 2; uv++)
	{
	// up scale bottom line
		pred = mb_cache_ptr->mbpix.pMBBfrU + uv*64 + (MB_CHROMA_SIZE-1)*MB_CHROMA_SIZE;
		pred_d = mb_cache_ptr->mbpix_d.pMBBfrU + uv*16 + (MB_CHROMA_SIZE_D-1)*MB_CHROMA_SIZE_D;

		//0, 1
		t0 = *pred_d++;	t1 = *pred_d++; t2 = *pred_d++;
		*pred++ = t0;
		*pred++ = ((-1)*t0 + 9*t0 + 9*t1 + (-1)*t2 + 8) >> 4;

		//2, 3
		t3 = *pred_d; pred_d -= MB_CHROMA_SIZE_D;
		*pred++ = t1;
		*pred++ = ((-1)*t0 + 9*t1 + 9*t2 + (-1)*t3 + 8) >> 4;

		//4, 5
		*pred++ = t2;
		*pred++ = ((-1)*t1 + 9*t2 + 9*t3 + (-1)*t3 + 8) >> 4;

		//6, (pix7 in left line)
		*pred++ = t3;

	// up scale left line

		//7, 6
		t2 = *pred_d;pred_d -= MB_CHROMA_SIZE_D;
		*pred = ((-1)*t2 + 9*t3 + 9*t3 + (-1)*t3 + 8) >> 4;	pred -= MB_CHROMA_SIZE;
		*pred = t3;	pred -= MB_CHROMA_SIZE;

		//5, 4
		t1 = *pred_d;pred_d -= MB_CHROMA_SIZE_D;
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
#else
;
#endif

PUBLIC void RvDec_recon_interMB_P_d(RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr)
{
    int32 iRefPosX, iRefPosY;
	int32 uMcFactor = rv_decoder_ptr->uMcFactor_d;
	uint8 *pPred = mb_cache_ptr->mbpix_d.pMBBfrY; 
	int16 *pMVMB = mb_cache_ptr->mv_cache + (4+1)*2;   //point to current MB's mv in mv cache;
	uint8 **Frame = rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV;
	uint32 uExdFrameWidth =  rv_decoder_ptr->uFrameExtendWidth_d;
	int32 iMbPosX = (rv_decoder_ptr->uMbPosX * (MB_SIZE/2) + (RV_Y_EXTEND_SIZE/2)) * uMcFactor;
	int32 iMbPosY = (rv_decoder_ptr->uMbPosY * (MB_SIZE/2) + (RV_Y_EXTEND_SIZE/2)) * uMcFactor;
// 	uint32 uOffset;
	int32 uBlkIdx;
	uint8 *pPred_blk;  //for 4mv
	//int16 *mv_ptr;
	uint8 *pPredU = mb_cache_ptr->mbpix_d.pMBBfrU; 
	uint8 *pPredV = mb_cache_ptr->mbpix_d.pMBBfrV; 
	short * pmv;
	int32 xoffset, yoffset;
	int32 iMbPosX_uv, iMbPosY_uv;

	iMbPosX_uv = iMbPosX >>1;
	iMbPosY_uv = iMbPosY >>1;

	if(!mb_cache_ptr->bUse4Mv) ///1 mv, same as RvDec_MC_oneDirection_MB()
	{
		// luma
		iRefPosX = iMbPosX + (*pMVMB);
		iRefPosY = iMbPosY + (*(pMVMB + 1));

		MC_luma8x8_xmyn (Frame[0], pPred, iRefPosX, iRefPosY, uExdFrameWidth);
		
		//chroma
		
		pmv = pMVMB;
		iRefPosX = iMbPosX_uv + *pmv/2;
		iRefPosY = iMbPosY_uv + *(pmv+1)/2;
		PC_MC_chroma4x4_rv (Frame[1], pPredU, iRefPosX, iRefPosY, uExdFrameWidth/2);
		PC_MC_chroma4x4_rv (Frame[2], pPredV, iRefPosX, iRefPosY, uExdFrameWidth/2);


	}else ///4mv
	{
		for(uBlkIdx = 0; uBlkIdx < 4; uBlkIdx++)
		{
			//luma
			xoffset = (uBlkIdx & 1) * 8/2;
			yoffset = (uBlkIdx >> 1) * 8/2;
		
			pmv = pMVMB + ((uBlkIdx>>1)*4 + (uBlkIdx&1)) * 2;
			iRefPosX = iMbPosX + xoffset*uMcFactor + *pmv;
			iRefPosY = iMbPosY + yoffset*uMcFactor + *(pmv+1);
		
			pPred_blk = pPred + yoffset * MB_SIZE/2 + xoffset;
		
			MC_luma4x4_xmyn (Frame[0], pPred_blk, iRefPosX, iRefPosY, uExdFrameWidth);

			//chroma
			xoffset >>=1;
			yoffset >>=1;

			iRefPosX = iMbPosX_uv + xoffset*uMcFactor + *pmv/2;
			iRefPosY = iMbPosY_uv + yoffset*uMcFactor + *(pmv+1)/2;
		
			//u
			pPred_blk = pPredU + yoffset * MB_CHROMA_SIZE/2 + xoffset;

			PC_MC_chroma2x2_rv (Frame[1], pPred_blk, iRefPosX, iRefPosY, uExdFrameWidth/2);

			//V
			pPred_blk = pPredV + yoffset * MB_CHROMA_SIZE/2 + xoffset;
	
			PC_MC_chroma2x2_rv (Frame[2], pPred_blk, iRefPosX, iRefPosY, uExdFrameWidth/2);

		}
	}
	
	if (mb_cache_ptr->cbp || mb_cache_ptr->inter16x16)
	{
		RvDec_AddPredAndResidual_Mb_d(mb_mode_ptr, mb_cache_ptr, mb_cache_ptr->cbp);
	}

	Up_Scaler_ppred(mb_cache_ptr);
}

#if defined(BSLICE_SUPPORTED)
//if mv is integer, the reference pixel need not to be copy into MB buffer.
//note: when optimization, the reconstructed pixel should be writen to reference frame buffer directly, other than MB buffer
PUBLIC void RvDec_recon_interMB_B (RV_DECODER_T *rv_decoder_ptr, MB_MODE_T *mb_mode_ptr, MB_CACHE_T *mb_cache_ptr)
{
	if ((MBTYPE_SKIPPED == mb_mode_ptr->uMbType) || (MBTYPE_DIRECT == mb_mode_ptr->uMbType))
	{
		RvDec_MC_direct_MB (rv_decoder_ptr, mb_cache_ptr);
	}else
	{	
		int32 intpDir = mb_cache_ptr->intpDir;
	
		if (intpDir & 1)
		{
			RvDec_MC_oneDirection_MB (rv_decoder_ptr, rv_decoder_ptr->pFrdRefFrame->pDecFrame->imgYUV, &mb_cache_ptr->mbpix, mb_cache_ptr->mv_cache);
		}
		
		if (intpDir & 2)
		{	
			MB_PIXEL_T * pMBPix = (intpDir&1) ? &mb_cache_ptr->mbpix_bck : &mb_cache_ptr->mbpix;
			RvDec_MC_oneDirection_MB (rv_decoder_ptr, rv_decoder_ptr->pBckRefFrame->pDecFrame->imgYUV, pMBPix, mb_cache_ptr->bck_mv_cache);
		}
		
		if (intpDir == 3)
		{
			g_RvDec_Get_Weighted_Average (rv_decoder_ptr, &mb_cache_ptr->mbpix, &mb_cache_ptr->mbpix_bck);
		}
	}
	
	if (mb_cache_ptr->cbp)
	{
		RvDec_AddPredAndResidual_Mb(mb_mode_ptr, mb_cache_ptr, mb_cache_ptr->cbp);
	}
}
#endif

#if !defined(PUT_MB_ASSEMLY)
PUBLIC void RvDec_WriteRecMB2Frame(RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3])
{
	int32 i;
	int32 *pIntSrc, *pIntDst;
	int32 *pIntSrc1, *pIntDst1;
	int32 width = rv_decoder_ptr->uFrameExtendWidth >> 2;

	/*put Y*/
	pIntSrc = (int32 *)mb_cache_ptr->mbpix.pMBBfrY;
	pIntDst = (int32 *)ppxlcRecMBYUV[0];
	for (i =0; i < MB_SIZE; i++)
	{
		pIntDst [0] = *pIntSrc++;
		pIntDst [1] = *pIntSrc++;
		pIntDst [2] = *pIntSrc++;
		pIntDst [3] = *pIntSrc++;

		pIntDst += width;
	}

	/*put U*/
	width = width / 2;
	pIntSrc = (int32 *)mb_cache_ptr->mbpix.pMBBfrU;
	pIntDst = (int32 *)ppxlcRecMBYUV[1];

	pIntSrc1 = (int32 *)mb_cache_ptr->mbpix.pMBBfrV;
	pIntDst1 = (int32 *)ppxlcRecMBYUV[2];
	
	for (i = 0; i < BLOCK_SIZE; i++)
	{
		pIntDst [0]  = *pIntSrc++;
		pIntDst [1]  = *pIntSrc++;

		pIntDst1 [0] = *pIntSrc1++;
		pIntDst1 [1] = *pIntSrc1++;
		
		pIntDst  += width;
		pIntDst1 += width;
	}
}
#endif

#if  !defined(PUT_MB_ASSEMLY)
PUBLIC void RvDec_WriteRecMB2Frame_d(RV_DECODER_T *rv_decoder_ptr, MB_CACHE_T *mb_cache_ptr, uint8 *ppxlcRecMBYUV[3])
{
	int32 i;
	int32 *pIntSrc, *pIntDst;
	int32 *pIntSrc1, *pIntDst1;
	int32 width = rv_decoder_ptr->uFrameExtendWidth_d >> 2;

	/*put Y*/
	pIntSrc = (int32 *)mb_cache_ptr->mbpix_d.pMBBfrY;
	pIntDst = (int32 *)ppxlcRecMBYUV[0];
	for (i =0; i < (MB_SIZE/2); i++)
	{
		pIntDst [0] = *pIntSrc++;
		pIntDst [1] = *pIntSrc++;
//		pIntDst [2] = *pIntSrc++;
//		pIntDst [3] = *pIntSrc++;

		pIntDst += width;
	}

	/*put U*/
	width = width / 2;
	pIntSrc = (int32 *)mb_cache_ptr->mbpix_d.pMBBfrU;
	pIntDst = (int32 *)ppxlcRecMBYUV[1];

	pIntSrc1 = (int32 *)mb_cache_ptr->mbpix_d.pMBBfrV;
	pIntDst1 = (int32 *)ppxlcRecMBYUV[2];
	
	for (i = 0; i < (BLOCK_SIZE/2); i++)
	{
		pIntDst [0]  = *pIntSrc++;
	// 	pIntDst [1]  = *pIntSrc++;

		pIntDst1 [0] = *pIntSrc1++;
	// 	pIntDst1 [1] = *pIntSrc1++;
		
		pIntDst  += width;
		pIntDst1 += width;
	}
}
//#else
//;
#endif
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
