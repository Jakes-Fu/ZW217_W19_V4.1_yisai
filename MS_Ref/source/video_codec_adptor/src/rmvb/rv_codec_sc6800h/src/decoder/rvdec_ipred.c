
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

#if !defined(_ARM_)
int32 s_MBLeftEdage [2][16] =
{
	{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
	{1, 0, 1, 0},
};

int32 s_MBTopEdge [2][16] = 
{
	{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0, 0},
};
#endif

#define GET_PIX_X(pTopPix, pLeftPix, beChroma, pitch, blkIdx, X)\
{\
	int blkWidth = 4;\
	if (beChroma)	blkWidth = 2;\
	if (blkIdx < blkWidth)	X = pTopPix[-1];\
	else	X = pLeftPix[-pitch];\
}	

//DC prediction
void intra_pred_4x4_DC_PRED(uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)
{
	int availNum = 0;
	int sum = 0;
	int dstPitch;
	int DC, fourDC;
	int tmp1, tmp2;

	if (!s_MBTopEdge[beChroma][blkIdx] || pMBCache->mbAvailB)
	{
		tmp1 = pTopPix[0];
		tmp2 = pTopPix[1];	sum += tmp1;
		tmp1 = pTopPix[2];	sum += tmp2;
		tmp2 = pTopPix[3];	sum += tmp1;
							sum += tmp2;
		availNum++;
	}

	if (!s_MBLeftEdage[beChroma][blkIdx] || pMBCache->mbAvailA)
	{
		tmp1 = pLeftPix[0];
		tmp2 = pLeftPix[pitch];	sum += tmp1;
		tmp1 = pLeftPix[pitch*2];	sum += tmp2;
		tmp2 = pLeftPix[pitch*3];	sum += tmp1;
								sum += tmp2;
		availNum++;
	}

	if (availNum == 2)
	{
		DC = (sum + 4) >> 3;
	}
	else if (availNum == 1)
	{
		DC = (sum + 2) >> 2;
	}
	else
	{
		DC = 128;
	}

	fourDC = DC * 0x01010101;

	dstPitch = 4;
	if (beChroma)
		dstPitch = 2;

	((uint32 *)pPred) [0] = fourDC;
	((uint32 *)pPred) [dstPitch] = fourDC;
	((uint32 *)pPred) [dstPitch*2] = fourDC;
	((uint32 *)pPred) [dstPitch*3] = fourDC;
//	g_IP4x4modeNum[0]++;
}
#else
;
#endif

//mode 1
void intra_pred_4x4_VERT_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)
{
	uint32 a;
	int dstPitch;

	a = ((uint32 *)pTopPix) [0];
	
	dstPitch = 4;
	if (beChroma)
		dstPitch = 2;

	((uint32 *)pPred)[0*dstPitch] = a;
	((uint32 *)pPred)[1*dstPitch] = a;
	((uint32 *)pPred)[2*dstPitch] = a;
	((uint32 *)pPred)[3*dstPitch] = a;
//	g_IP4x4modeNum[1]++;
}
#else
;
#endif

//mode 2
void intra_pred_4x4_HOR_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)
{
	int dstPitch;
	uint32 pix, fourPix;

	dstPitch = 16;
	if (beChroma)
		dstPitch = 8;
	
	pix = pLeftPix[0*pitch];	fourPix = pix*0x01010101;	((uint32 *)pPred)[0] = fourPix;	pPred += dstPitch;
	pix = pLeftPix[1*pitch];	fourPix = pix*0x01010101;	((uint32 *)pPred)[0] = fourPix;	pPred += dstPitch;
	pix = pLeftPix[2*pitch];	fourPix = pix*0x01010101;	((uint32 *)pPred)[0] = fourPix;	pPred += dstPitch;
	pix = pLeftPix[3*pitch];	fourPix = pix*0x01010101;	((uint32 *)pPred)[0] = fourPix;	
//	g_IP4x4modeNum[2]++;
}
#else
;
#endif	

//mode 3
void intra_pred_4x4_DIAG_DOWN_RIGHT_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
{
	int dstPitch;
	int A, B, C, D, I, J, K, L, X;

//	X = get_Pix_X (pTopPix, pLeftPix, beChroma, pitch, blkIdx);
	GET_PIX_X(pTopPix, pLeftPix, beChroma, pitch, blkIdx, X);
	I = pLeftPix [0];
	J = pLeftPix [pitch];
	K = pLeftPix [pitch*2];
	L = pLeftPix [pitch*3];

	dstPitch = 16;
	if (beChroma)
		dstPitch = 8;
	
	/*x coordinate greater than y*/
	pPred [dstPitch*3] = (L + 2*K + J + 2) >> 2;	//m
	pPred [dstPitch*2] = pPred[dstPitch*3+1] = (K + 2*J + I + 2) >> 2;		//i, n
	pPred [dstPitch*1] = pPred[dstPitch*2+1] = pPred[dstPitch*3+2] = (J + 2*I + X + 2) >> 2;	//e, j, o

	A = pTopPix[0];
	/*x coordinate equal y*/
	pPred [dstPitch*0] = pPred[dstPitch*1+1] = pPred[dstPitch*2+2] = pPred[dstPitch*3+3] = (I + 2*X + A + 2) >> 2;		//a, f, k, l

	/*x<y*/
	B = pTopPix[1];
	C = pTopPix[2];
	D = pTopPix[3];

	pPred[dstPitch*0 + 1] = pPred[dstPitch*1 + 2] = pPred[dstPitch*2 + 3] = (X + 2*A + B + 2) >> 2;		//b g l
	pPred[dstPitch*0 + 2] = pPred[dstPitch*1 + 3] = (A + 2*B + C + 2) >> 2;		//c h
	pPred[dstPitch*0 + 3] = (B + 2*C + D + 2) >> 2;		//d

//	g_IP4x4modeNum[3]++;
}

void get_topRight_pix (uint8 * pTrPix, MB_CACHE_T * pMBCache, uint8 * pTopPix, int beChroma, int blkIdx)
{
	int trAvl = 0;
	int leftBlkId;
	uint32 fourPix;

	leftBlkId = beChroma ? 1 : 3;

	if ((blkIdx & leftBlkId) < leftBlkId) 
	{
		trAvl = 1;
	}
	else if (blkIdx == leftBlkId)
	{
		trAvl = pMBCache->mbAvailC;
	} 

	if (trAvl)
	{
		fourPix = ((uint32 *)pTopPix) [1];
	}
	else
	{
		uint32 pD = pTopPix[3];
		fourPix = pD*0x01010101;
	}

	((uint32 *)pTrPix)[0] = fourPix;

}

void get_leftBelow_pix (uint8 * pLbpix, uint8 * pLeftPix, int beChroma, int blkIdx, int pitch)
{
	int lbAvil = 0;

	if (beChroma)
	{
		if (blkIdx == 0)
		{
			lbAvil = 1;
		}
	}
	else
	{
		if (((blkIdx & 3) == 0) && (blkIdx != 12))
		{
			lbAvil = 1;			
		}
	}

	if (lbAvil)
	{
		pLeftPix += pitch*4;
		pLbpix [0] = pLeftPix [pitch*0];
		pLbpix [1] = pLeftPix [pitch*1];
		pLbpix [2] = pLeftPix [pitch*2];
		pLbpix [3] = pLeftPix [pitch*3];
	}
	else
	{
		uint8 pL = pLeftPix[pitch*3];
		((uint32 *)pLbpix)[0] = pL * 0x01010101;
	}
}

//mode 4
void intra_pred_4x4_DIAG_DOWN_LEFT_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
{
	int dstPitch;
	int A, B, C, D, I, J, K, L;
	int E, F, G, H, M, N, O, P;
	uint8 trPix[4], lbPix [4];
	
	get_topRight_pix (trPix, pMBCache, pTopPix, beChroma, blkIdx);
	get_leftBelow_pix (lbPix, pLeftPix, beChroma, blkIdx, pitch);

	A = pTopPix[0];
	B = pTopPix[1];
	C = pTopPix[2];

	I = pLeftPix [pitch*0];
	J = pLeftPix [pitch*1];
	K = pLeftPix [pitch*2];

	pPred [0] = (A + 2*B + C + I + 2*J + K + 4) >> 3;		//a

	D = pTopPix[3];
	L = pLeftPix[pitch*3];

	dstPitch = 16;
	if (beChroma)
		dstPitch = 8;
	pPred[1] = pPred[dstPitch] = (B + 2*C + D + J + 2*K + L + 4) >> 3;	//b, e

	E = trPix[0];	M = lbPix[0];
	pPred[2] = pPred[dstPitch+1] = pPred[dstPitch*2] = (C + 2*D + E + K + 2*L + M + 4) >> 3;	//c, f, i

	F = trPix[1];	N = lbPix[1];
	pPred[3] = pPred[dstPitch+2] = pPred[dstPitch*2+1] = pPred[dstPitch*3] = (D + 2*E + F + L + 2*M + N + 4) >> 3;		//d, g, j, m

	G = trPix[2];	O = lbPix[2];
	pPred[dstPitch+3] = pPred[dstPitch*2+2] = pPred[dstPitch*3+1] = (E + 2*F + G + M + 2*N + O + 4) >> 3;		//h, k, n

	H = trPix[3];	P = lbPix[3];
	pPred[dstPitch*2+3] = pPred[dstPitch*3+2] = (F + 2*G + H + N + 2*O + P + 4) >> 3;				//l, o

	pPred[dstPitch*3+3] = (G + H + O + P + 2) >> 2;			//p

	
//	g_IP4x4modeNum[4]++;
}

//mode 5
void intra_pred_4x4_VERT_RIGHT_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
{
	int dstPitch;
	int A, B, C, D, I, J, K, X;

//	X = get_Pix_X (pTopPix, pLeftPix, beChroma, pitch, blkIdx);
	GET_PIX_X(pTopPix, pLeftPix, beChroma, pitch, blkIdx, X);
	A = pTopPix[0];
	B = pTopPix[1];
	C = pTopPix[2];
	D = pTopPix[3];

	dstPitch = 16;
	if (beChroma)
		dstPitch = 8;
	
	//can simplify the addressing
	pPred[0] = pPred[dstPitch*2+1] = (X + A + 1) >> 1;		//a, j
	pPred[1] = pPred[dstPitch*2+2] = (A + B + 1) >> 1;		//b, k
	pPred[2] = pPred[dstPitch*2+3] = (B + C + 1) >> 1;		//c, l
	pPred[3] = (C + D + 1) >> 1;							//d

	I = pLeftPix[0];
	pPred[dstPitch] = pPred[dstPitch*3+1] = (I + 2*X + A + 2) >> 2;		//e, n
	pPred[dstPitch+1] = pPred[dstPitch*3+2] = (X + 2*A + B + 2) >> 2;	//f, o
	pPred[dstPitch+2] = pPred[dstPitch*3+3] = (A + 2*B + C + 2) >> 2;	//g, p
	pPred[dstPitch+3] = (B + 2*C + D + 2) >> 2;							//h

	I = pLeftPix[0];
	J = pLeftPix[pitch];
	K = pLeftPix[pitch*2];
	pPred[dstPitch*2] = (X + 2*I + J + 2) >> 2;			//i
	pPred[dstPitch*3] = (I + 2*J + K + 2) >> 2;			//m

	
//	g_IP4x4modeNum[5]++;
}

//mode 6
void intra_pred_4x4_VERT_LEFT_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
{
	int dstPitch;
	int A, B, C, D, J, K, L;
	int E, F, G, M;
	uint8 trPix[4], lbPix [4];
	
	get_topRight_pix (trPix, pMBCache, pTopPix, beChroma, blkIdx);
	get_leftBelow_pix (lbPix, pLeftPix, beChroma, blkIdx, pitch);
	
	/*y = 0 or 2*/
	A = pTopPix[0];
	B = pTopPix[1];
	C = pTopPix[2];
	D = pTopPix[3];
	J = pLeftPix[pitch];
	K = pLeftPix[pitch*2];
	L = pLeftPix[pitch*3];

	dstPitch = 16;
	if (beChroma)
		dstPitch = 8;

	pPred[0] = (2*A + 2*B + J + 2*K + L + 4) >> 3;						//a
	pPred[1] = pPred[dstPitch*2] = (B + C + 1) >> 1;					//b, i
	pPred[2] = pPred[dstPitch*2+1] = (C + D + 1) >> 1;					//c, j

	E = trPix[0];	F = trPix[1];
	pPred[3] = pPred[dstPitch*2+2] = (D + E + 1) >> 1;					//d, k
	pPred[dstPitch*2+3] = (E + F + 1) >> 1;								//l

	/*y = 1 or 3*/
	M = lbPix[0];
	pPred[dstPitch] = (A + 2*B + C + K + 2*L + M + 4) >> 3;				//e
	pPred[dstPitch*1+1] = pPred[dstPitch*3+0] = (B + 2*C + D + 2) >> 2;	//f, m
	pPred[dstPitch*1+2] = pPred[dstPitch*3+1] = (C + 2*D + E + 2) >> 2;	//g, n
	pPred[dstPitch*1+3] = pPred[dstPitch*3+2] = (D + 2*E + F + 2) >> 2;	//h, o

	G = trPix[2];
	pPred[dstPitch*3+3] = (E + 2*F + G + 2) >> 2;						//p

	
//	g_IP4x4modeNum[6]++;
}

//mode 7
void intra_pred_4x4_HOR_UP_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
{
	int dstPitch;
	int B, C, D, I, J, K, L;
	int E, F, G, H, M, N, O;
	uint8 trPix[4], lbPix [4];

	get_topRight_pix (trPix, pMBCache, pTopPix, beChroma, blkIdx);
	get_leftBelow_pix (lbPix, pLeftPix, beChroma, blkIdx, pitch);
	
	B = pTopPix[1];
	C = pTopPix[2];
	D = pTopPix[3];
	I = pLeftPix[0];
	J = pLeftPix[pitch];
	K = pLeftPix[pitch*2];
	L = pLeftPix[pitch*3];

	pPred[0] = (B + 2*C + D + 2*I + 2*J + 4) >> 3;								//a

	E = trPix[0];
	pPred[1] = (C + 2*D + E + I + 2*J + K + 4) >> 3;							//b

	F = trPix[1];
	dstPitch = 16;
	if (beChroma)
		dstPitch = 8;
	pPred [2] = pPred[dstPitch] = (D + 2*E + F + 2*J + 2* K + 4) >> 3;			//c, e

	G = trPix[2];
	pPred [3] = pPred[dstPitch+1] = (E + 2*F + G + J + 2*K + L + 4) >> 3;		//d, f

	G = trPix[2];	H = trPix[3];
    pPred [dstPitch+2] = pPred[dstPitch*2] = (F + 2*G + H + 2*K + 2*L + 4) >> 3;		//g, i
	pPred [dstPitch+3] = pPred[dstPitch*2+1] = (G + 3*H + K + 3*L + 4) >> 3;		//h, j

	M = lbPix[0];	N = lbPix[1];
	pPred [dstPitch*2+3] = pPred[dstPitch*3+1] = (L + 2*M + N + 2) >> 2;		//l, n
	pPred[dstPitch*3] = pPred[dstPitch*2+2] = (G + H + L + M + 2) >> 2;			//m, k
	pPred [dstPitch*3+2] = (M + N + 1) >> 1;			//o

	O = lbPix[2];
	pPred [dstPitch*3+3] = (M + 2*N + O + 2) >> 2;		//p

	
//	g_IP4x4modeNum[7]++;
}

//mode 8
void intra_pred_4x4_HOR_DOWN_PRED (uint8 *pPred,  uint8 * pTopPix, uint8 * pLeftPix , int beChroma, int pitch, int blkIdx, MB_CACHE_T *pMBCache)
{
	int dstPitch;
	int A, B, C, I, J, K, L, X;

	dstPitch = 16;
	if (beChroma)
		dstPitch = 8;

//	X = get_Pix_X (pTopPix, pLeftPix, beChroma, pitch, blkIdx);
	GET_PIX_X(pTopPix, pLeftPix, beChroma, pitch, blkIdx, X);
	A = pTopPix[0];
	B = pTopPix[1];
	C = pTopPix[2];
	I = pLeftPix[0];
	J = pLeftPix[pitch];

	pPred[0] = pPred[dstPitch+2] = (X + I + 1) >> 1;		//a, g
	pPred[1] = pPred[dstPitch+3] = (I + 2*X + A + 2) >> 2;	//b, h
	pPred[2] = (X + 2*A + B + 2) >> 2;						//c
	pPred[3] = (A + 2*B + C + 2) >> 2;						//d

	pPred[dstPitch] = pPred[dstPitch*2+2] = (I + J + 1) >> 1;	//e, k
	pPred[dstPitch+1] = pPred[dstPitch*2+3] = (X + 2*I + J + 2) >> 2;	//f, l

	K = pLeftPix[pitch*2];
	pPred[dstPitch*2] = pPred[dstPitch*3+2] = (J + K + 1) >> 1;		//i, o
	pPred[dstPitch*2+1] = pPred[dstPitch*3+3] = (I + 2*J + K + 2) >> 2; //j, p

	L = pLeftPix[pitch*3];
	pPred[dstPitch*3] = (K + L + 1) >> 1;	//m
	pPred[dstPitch*3+1] = (J + 2*K + L + 2) >> 2;	//n
	
//	g_IP4x4modeNum[8]++;
}

/*the predictor is stored in frame buffer, pitch is frame width, and result of prediction is stored in MB buffer*/
void intra_pred_Luma16x16_DC_PRED(uint8 *pTopPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)
{
	int32 i;
	int32 sum = 0;
	int32 DC, fourDC;
	int32 availNum = 0;
	uint32 *pPredInt;

	if (pMBCache->mbAvailA)
	{		
		for (i = 4; i > 0; i--)
		{
			sum += pLeftPix[0];  pLeftPix += pitch;
			sum += pLeftPix[0];  pLeftPix += pitch;
			sum += pLeftPix[0];  pLeftPix += pitch;
			sum += pLeftPix[0];  pLeftPix += pitch;	
		}

		availNum++;
	}

	if (pMBCache->mbAvailB)
	{
		for (i = 4; i > 0; i--)
		{
			sum += *pTopPix++;
			sum += *pTopPix++;
			sum += *pTopPix++;
			sum += *pTopPix++;
		}

		availNum++;		
	}

	if (availNum == 2)
	{
		DC = (sum + 16)>>5;
	}
	else if (availNum == 1)
	{
		DC = (sum + 8)>>4;
	}
	else
	{
		DC = 128;
	}

	fourDC = DC * 0x01010101;
	pPredInt = (uint32 *)pPred;
	for (i = 16; i > 0; i--)
	{
		*pPredInt++ = fourDC;
		*pPredInt++ = fourDC;
		*pPredInt++ = fourDC;
		*pPredInt++ = fourDC;	
	}
}
#else
;
#endif

void intra_pred_Luma16x16_VERT_PRED(uint8 *pTopPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)	
{
	int32 i;
	uint32 a, b, c, d;
	uint32 *pPredInt = (uint32 *)pPred;	
	
	a = ((uint32 *)pTopPix)[0];
	b = ((uint32 *)pTopPix)[1];
	c = ((uint32 *)pTopPix)[2];
	d = ((uint32 *)pTopPix)[3];

	for(i = 16; i > 0; i--)
	{
		*pPredInt++ = a;
		*pPredInt++ = b;
		*pPredInt++ = c;
		*pPredInt++ = d;
	}
}
#else
;
#endif

void intra_pred_Luma16x16_HORZ_PRED(uint8 *pTopPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)	
{
	int32 i;
	uint32 fourPix;
	uint32 *pPredInt = (uint32 *)pPred;

	for(i = 16; i > 0; i--)
	{
		fourPix = pLeftPix[0] * 0x01010101; 

		*pPredInt++ = fourPix;
		*pPredInt++ = fourPix;
		*pPredInt++ = fourPix;
		*pPredInt++ = fourPix;

		pLeftPix += pitch;
	}
}
#else
;
#endif

void intra_pred_Luma16x16_PLANAR_PRED(uint8 *pTopPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)	
{
	int i,j;
	int32 a, b, c;
	int32 iH = 0, iV = 0;
	uint8 *tp, * lpt, * lpb;
	int cFactor, bFactor, sum;
	uint8 *pClip = g_rgiRvClipTab;

	tp = pTopPix;
	a = (tp[15]+pLeftPix[15*pitch])<<4;

	lpb = pLeftPix + 8*pitch;
	lpt = lpb - 2*pitch;
	for(i = 1; i < 8; i++)
	{
		iH += i * (tp[7+i] - tp[7-i]);	
		iV += i * (lpb[0] - lpt[0]);

		lpb += pitch;
		lpt -= pitch;
	}
	iH += 8 * (tp[15] - tp[-1]);	
	iV += 8 * (lpb[0] - tp[-1]);

	b = (iH + (iH>>2))>>4;
	c = (iV + (iV>>2))>>4;
	
	cFactor = -7*c;	
	bFactor = -7*b;
	for (j = 16; j > 0; j--)
	{
		sum = a + cFactor + 16 + bFactor;
		
		for (i = 16; i > 0; i-=4)
		{
			*pPred++ = pClip[sum >> 5];	sum += b;
			*pPred++ = pClip[sum >> 5];	sum += b;
			*pPred++ = pClip[sum >> 5];	sum += b;
			*pPred++ = pClip[sum >> 5];	sum += b;
		}

		cFactor += c;
	}
}
#else
;
#endif

void intra_pred_CHROMA8x8_DC_PRED(uint8 *pTopPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *pMBCache)
{
	int32 i;
	int32 sum = 0;
	int32 availNum = 0;
	int32 fourDC, DC;
	uint32 *pPredInt;
	
	if (pMBCache->mbAvailA)
	{		
		for (i = 2; i > 0; i--)
		{
		#if defined(_ARM_)
			__asm
			{
				ldrb r6, [pLeftPix], pitch
				ldrb r7, [pLeftPix], pitch
				add	sum, sum, r6
				ldrb r6, [pLeftPix], pitch
				add sum, sum, r7
				ldrb r7, [pLeftPix], pitch
				add	sum, sum, r6
				add	sum, sum, r7
			}
		#else
			sum += pLeftPix[0];  pLeftPix += pitch;
			sum += pLeftPix[0];  pLeftPix += pitch;
			sum += pLeftPix[0];  pLeftPix += pitch;
			sum += pLeftPix[0];  pLeftPix += pitch;
		#endif
		}
		
		availNum++;
	}
	
	if (pMBCache->mbAvailB)
	{
		for (i = 2; i > 0; i--)
		{
		#if defined(_ARM_)
			__asm
			{
				ldrb	r2, [pTopPix], #1
				ldrb	r3, [pTopPix], #1
				add	sum, sum, r2
				ldrb	r2, [pTopPix], #1
				add	sum, sum, r3
				ldrb	r3, [pTopPix], #1
				add	sum, sum, r2
				add	sum, sum, r3				
			}
		#else
			sum += *pTopPix++;
			sum += *pTopPix++;
			sum += *pTopPix++;
			sum += *pTopPix++;
		#endif	
		}
		
		availNum++;		
	}
	
	if (availNum == 2)
	{
		DC = (sum + 8)>>4;
	}
	else if (availNum == 1)
	{
		DC = (sum + 4)>>3;
	}
	else
	{
		DC = 128;
	}
	
	fourDC = DC * 0x01010101;
	pPredInt = (uint32 *)pPred;
	for (i = 4; i > 0; i--)
	{
	#if defined(_ARM_)
		__asm
		{
			str fourDC, [pPred], #4
			str fourDC, [pPred], #4
			str fourDC, [pPred], #4
			str fourDC, [pPred], #4			
		}
	#else
		*pPredInt++ = fourDC;
		*pPredInt++ = fourDC;
		*pPredInt++ = fourDC;
		*pPredInt++ = fourDC;
	#endif	
	}
}

void intra_pred_CHROMA8x8_VERT_PRED(uint8 *pTopPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)
{
	uint32 i;
	uint32 a, b;
	uint32 * pPredInt = (uint32 *)pPred;	

	a = ((uint32 *)pTopPix) [0];
	b = ((uint32 *)pTopPix) [1];
	for(i = 2; i > 0; i--)
	{		
		*pPredInt++ = a;	*pPredInt++ = b;	//row0
		*pPredInt++ = a;	*pPredInt++ = b;	//row1
		*pPredInt++ = a;	*pPredInt++ = b;	//row2
		*pPredInt++ = a;	*pPredInt++ = b;	//row3
	}
}
#else
;
#endif

void intra_pred_CHROMA8x8_HORZ_PRED(uint8 *pTopLeftPix, uint8 *pLeftPix, uint8 *pPred, int32 pitch, MB_CACHE_T *pMBCache)
#if !defined(_ARM_)
{
	uint32 i;
	uint32 fourPix;
	uint32 *pPredInt = (uint32 *)pPred;

	for(i = 4; i > 0; i--)
	{
		fourPix = pLeftPix[0] *0x01010101; *pPredInt++ = fourPix;	*pPredInt++ = fourPix;	pLeftPix += pitch;	//row 0
		fourPix = pLeftPix[0] *0x01010101; *pPredInt++ = fourPix;	*pPredInt++ = fourPix;	pLeftPix += pitch;	//row 1
	}
}
#else
;
#endif

PUBLIC void RvDec_InitIPredFun(void)
{
	Intra4x4Pred_func *intraPred4x4 = g_RvIPred4x4;
	Intra16x16Pred_func *intraPredLuma16x16 = g_intraPredLuma16x16;
	Intra16x16Pred_func *intraPredChroma8x8 = g_intraPredChroma8x8;
	
	intraPred4x4[0] = intra_pred_4x4_DC_PRED;
	intraPred4x4[1] = intra_pred_4x4_VERT_PRED;
	intraPred4x4[2] = intra_pred_4x4_HOR_PRED;
	intraPred4x4[3] = intra_pred_4x4_DIAG_DOWN_RIGHT_PRED;
	intraPred4x4[4] = intra_pred_4x4_DIAG_DOWN_LEFT_PRED;
	intraPred4x4[5] = intra_pred_4x4_VERT_RIGHT_PRED;
	intraPred4x4[6] = intra_pred_4x4_VERT_LEFT_PRED;
	intraPred4x4[7] = intra_pred_4x4_HOR_UP_PRED;
	intraPred4x4[8] = intra_pred_4x4_HOR_DOWN_PRED;

	intraPredLuma16x16[0] = intra_pred_Luma16x16_DC_PRED;
	intraPredLuma16x16[1] = intra_pred_Luma16x16_VERT_PRED;
	intraPredLuma16x16[2] = intra_pred_Luma16x16_HORZ_PRED;
	intraPredLuma16x16[3] = intra_pred_Luma16x16_PLANAR_PRED;
	
	intraPredChroma8x8[0] = intra_pred_CHROMA8x8_DC_PRED;
	intraPredChroma8x8[1] = intra_pred_CHROMA8x8_VERT_PRED;
	intraPredChroma8x8[2] = intra_pred_CHROMA8x8_HORZ_PRED;
	intraPredChroma8x8[3] = intra_pred_CHROMA8x8_DC_PRED;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
