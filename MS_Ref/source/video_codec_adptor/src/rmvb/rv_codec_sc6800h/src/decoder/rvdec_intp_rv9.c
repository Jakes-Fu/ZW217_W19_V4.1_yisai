
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

#if !defined(_ARM_MC_ASSEMBLY_)
/*horizontal integer*/
void RvDec_MC_Luma8x8_H0V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int32 i;

	if (((uint32)pRef & 0x3) == 0)  //word align
	{
		for (i = 8; i > 0; i--)
		{
			((int *)pPred) [0] = ((int *)pRef) [0];
			((int *)pPred) [1] = ((int *)pRef) [1];

			pPred += MB_SIZE;
			pRef   += srcPitch;
		}
	}else
	{
		for (i = 8; i > 0; i--)
		{
			pPred [0] = pRef [0];
			pPred [1] = pRef [1];
			pPred [2] = pRef [2];
			pPred [3] = pRef [3];
			pPred [4] = pRef [4];
			pPred [5] = pRef [5];
			pPred [6] = pRef [6];
			pPred [7] = pRef [7];

			pPred += MB_SIZE;
			pRef   += srcPitch;
		}
	}
}

void RvDec_MC_Luma8x8_H0V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

	pRef -= (srcPitch<<1);

	pPred += 7;
	for (i = 7; i >= 0; i--)
	{
		pRefTemp = pRef + i;
		
		pn2 = *pRefTemp; pRefTemp += srcPitch; 
		pn1 = *pRefTemp; pRefTemp += srcPitch; 
		p0  = *pRefTemp; pRefTemp += srcPitch; 
		p1  = *pRefTemp; pRefTemp += srcPitch; 
		p2  = *pRefTemp; pRefTemp += srcPitch; 
		p3  = *pRefTemp; pRefTemp += srcPitch;pPred[0] = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRefTemp; pRefTemp += srcPitch;pPred[MB_SIZE] = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRefTemp; pRefTemp += srcPitch;pPred[MB_SIZE_X2] = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRefTemp; pRefTemp += srcPitch;pPred[MB_SIZE_X3] = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRefTemp; pRefTemp += srcPitch;pPred[MB_SIZE_X4] = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRefTemp; pRefTemp += srcPitch;pPred[MB_SIZE_X5] = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRefTemp; pRefTemp += srcPitch;pPred[MB_SIZE_X6] = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRefTemp; 				 	  pPred[MB_SIZE_X7] = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];

		pPred--;
	}	
}

void RvDec_MC_Luma8x8_H0V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

	pRef -= (srcPitch<<1);

	pPred += 7;
	for (i = 7; i >= 0; i--)
	{
		pRefTemp = pRef + i;
		
		pn2 = *pRefTemp; pRefTemp += srcPitch; 
		pn1 = *pRefTemp; pRefTemp += srcPitch; 
		p0  = *pRefTemp; pRefTemp += srcPitch; 
		p1  = *pRefTemp; pRefTemp += srcPitch; 
		p2  = *pRefTemp; pRefTemp += srcPitch; 
		p3  = *pRefTemp; pRefTemp += srcPitch;  pPred[0] = pClip[(pn2 - 5*(pn1+p2) + 20*(p0+p1) + p3 + 16) >> 5];
		p4  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE] = pClip[(pn1 - 5*(p0+p3) + 20*(p1+p2) + p4 + 16) >> 5];
		p5  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X2] = pClip[(p0 - 5*(p1+p4) + 20*(p2+p3) + p5 + 16) >> 5];
		p6  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X3] = pClip[(p1 - 5*(p2+p5) + 20*(p3+p4) + p6 + 16) >> 5];
		p7  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X4] = pClip[(p2 - 5*(p3+p6) + 20*(p4+p5) + p7 + 16) >> 5];
		p8  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X5] = pClip[(p3 - 5*(p4+p7)+ 20*(p5+p6) + p8 + 16) >> 5];
		p9  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X6] = pClip[(p4 - 5*(p5+p8) + 20*(p6+p7) + p9 + 16) >> 5];
		p10 = *pRefTemp; 						pPred[MB_SIZE_X7] = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];

		pPred--;
	}	
}

void RvDec_MC_Luma8x8_H0V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	
	pRef -= (srcPitch<<1);

	pPred += 7;
	for (i = 7; i >= 0; i--)
	{
		pRefTemp = pRef + i;
		
		pn2 = *pRefTemp; pRefTemp += srcPitch; 
		pn1 = *pRefTemp; pRefTemp += srcPitch; 
		p0  = *pRefTemp; pRefTemp += srcPitch; 
		p1  = *pRefTemp; pRefTemp += srcPitch; 
		p2  = *pRefTemp; pRefTemp += srcPitch; 
		p3  = *pRefTemp; pRefTemp += srcPitch; pPred[0] = pClip[(pn2 - 5*(pn1+p2) + 20*p0 + 52*p1 + p3 + 32) >> 6];
		p4  = *pRefTemp; pRefTemp += srcPitch; pPred[MB_SIZE] = pClip[(pn1 - 5*(p0+p3) + 20*p1 + 52*p2 + p4 + 32) >> 6];
		p5  = *pRefTemp; pRefTemp += srcPitch; pPred[MB_SIZE_X2] = pClip[(p0 - 5*(p1+p4) + 20*p2 + 52*p3 + p5 + 32) >> 6];
		p6  = *pRefTemp; pRefTemp += srcPitch; pPred[MB_SIZE_X3] = pClip[(p1 - 5*(p2+p5) + 20*p3 + 52*p4 + p6 + 32) >> 6];
		p7  = *pRefTemp; pRefTemp += srcPitch; pPred[MB_SIZE_X4] = pClip[(p2 - 5*(p3+p6) + 20*p4 + 52*p5 + p7 + 32) >> 6];
		p8  = *pRefTemp; pRefTemp += srcPitch; pPred[MB_SIZE_X5] = pClip[(p3 - 5*(p4+p7) + 20*p5 + 52*p6 + p8 + 32) >> 6];
		p9  = *pRefTemp; pRefTemp += srcPitch; pPred[MB_SIZE_X6] = pClip[(p4 - 5*(p5+p8) + 20*p6 + 52*p7 + p9 + 32) >> 6];
		p10 = *pRefTemp; 					   pPred[MB_SIZE_X7] = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];
		
		pPred--;
	}	
}

void RvDec_MC_Luma8x8_H1V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h1_r9 (pRef, pPred, srcPitch, 8);	
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	
	srcPitch -= 12; 
	pRef -= 2;
	for (i = 8; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++; *pPred++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++; *pPred++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++; *pPred++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++; *pPred++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++; *pPred++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++; *pPred++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++; *pPred++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef;   *pPred   = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		
		pRef += srcPitch;
		pPred += 9;
	}
}

void RvDec_MC_Luma8x8_H1V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h1_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++; *pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++; *pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++; *pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++; *pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++; *pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++; *pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++; *pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef;   *pPred_tmp   = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}

	RvDec_MC_Luma8x8_H0V1_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma8x8_H1V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h1_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++; *pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++; *pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++; *pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++; *pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++; *pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++; *pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++; *pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef;   *pPred_tmp   = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}
	RvDec_MC_Luma8x8_H0V2_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma8x8_H1V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h1_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++; *pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++; *pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++; *pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++; *pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++; *pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++; *pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++; *pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef;   *pPred_tmp = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}
	RvDec_MC_Luma8x8_H0V3_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);		
}

void RvDec_MC_Luma8x8_H2V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h2_r9 (pRef, pPred, srcPitch, 8);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	
	pRef -= 2;
	srcPitch -= 12;
	for (i = 8; i > 0 ; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred++ = pClip[(pn2 - 5*(pn1+p2) + 20*(p0+p1) + p3 + 16) >> 5];
		p4  = *pRef++;	*pPred++ = pClip[(pn1 - 5*(p0+p3) + 20*(p1+p2) + p4 + 16) >> 5];
		p5  = *pRef++;	*pPred++ = pClip[(p0 - 5*(p1+p4) + 20*(p2+p3) + p5 + 16) >> 5];
		p6  = *pRef++;	*pPred++ = pClip[(p1 - 5*(p2+p5) + 20*(p3+p4) + p6 + 16) >> 5];
		p7  = *pRef++;	*pPred++ = pClip[(p2 - 5*(p3+p6) + 20*(p4+p5) + p7 + 16) >> 5];
		p8  = *pRef++;	*pPred++ = pClip[(p3 - 5*(p4+p7) + 20*(p5+p6) + p8 + 16) >> 5];
		p9  = *pRef++;	*pPred++ = pClip[(p4 - 5*(p5+p8) + 20*(p6+p7) + p9 + 16) >> 5];
		p10 = *pRef;	*pPred   = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		
		pRef += srcPitch;
		pPred += 9;
	}	
}

void RvDec_MC_Luma8x8_H2V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h2_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0 ; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 20*(p0+p1) + p3 + 16) >> 5];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 20*(p1+p2) + p4 + 16) >> 5];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 20*(p2+p3) + p5 + 16) >> 5];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 20*(p3+p4) + p6 + 16) >> 5];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 20*(p4+p5) + p7 + 16) >> 5];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 20*(p5+p6) + p8 + 16) >> 5];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 20*(p6+p7) + p9 + 16) >> 5];
		p10 = *pRef;	*pPred_tmp   = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}	
	RvDec_MC_Luma8x8_H0V1_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);
}

void RvDec_MC_Luma8x8_H2V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h2_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0 ; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 20*(p0+p1) + p3 + 16) >> 5];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 20*(p1+p2) + p4 + 16) >> 5];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 20*(p2+p3) + p5 + 16) >> 5];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 20*(p3+p4) + p6 + 16) >> 5];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 20*(p4+p5) + p7 + 16) >> 5];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 20*(p5+p6) + p8 + 16) >> 5];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 20*(p6+p7) + p9 + 16) >> 5];
		p10 = *pRef;	*pPred_tmp   = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}	
	RvDec_MC_Luma8x8_H0V2_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma8x8_H2V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h2_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0 ; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 20*(p0+p1) + p3 + 16) >> 5];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 20*(p1+p2) + p4 + 16) >> 5];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 20*(p2+p3) + p5 + 16) >> 5];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 20*(p3+p4) + p6 + 16) >> 5];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 20*(p4+p5) + p7 + 16) >> 5];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 20*(p5+p6) + p8 + 16) >> 5];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 20*(p6+p7) + p9 + 16) >> 5];
		p10 = *pRef;	*pPred_tmp   = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}	
	RvDec_MC_Luma8x8_H0V3_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma8x8_H3V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h3_r9 (pRef, pPred, srcPitch, 8);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	
	pRef -= 2;
	srcPitch -= 12;
	for (i = 8; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred++ = pClip[(pn2 - 5*(pn1+p2) + 20*p0 + 52*p1 + p3 + 32) >> 6];
		p4  = *pRef++;	*pPred++ = pClip[(pn1 - 5*(p0+p3) + 20*p1 + 52*p2 + p4 + 32) >> 6];
		p5  = *pRef++;	*pPred++ = pClip[(p0 - 5*(p1+p4) + 20*p2 + 52*p3 + p5 + 32) >> 6];
		p6  = *pRef++;	*pPred++ = pClip[(p1 - 5*(p2+p5) + 20*p3 + 52*p4 + p6 + 32) >> 6];
		p7  = *pRef++;	*pPred++ = pClip[(p2 - 5*(p3+p6) + 20*p4 + 52*p5 + p7 + 32) >> 6];
		p8  = *pRef++;	*pPred++ = pClip[(p3 - 5*(p4+p7) + 20*p5 + 52*p6 + p8 + 32) >> 6];
		p9  = *pRef++;	*pPred++ = pClip[(p4 - 5*(p5+p8) + 20*p6 + 52*p7 + p9 + 32) >> 6];
		p10 = *pRef;	*pPred   = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];		
		
		pRef += srcPitch;
		pPred += 9;
	}
}

void RvDec_MC_Luma8x8_H3V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h3_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 20*p0 + 52*p1 + p3 + 32) >> 6];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 20*p1 + 52*p2 + p4 + 32) >> 6];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 20*p2 + 52*p3 + p5 + 32) >> 6];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 20*p3 + 52*p4 + p6 + 32) >> 6];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 20*p4 + 52*p5 + p7 + 32) >> 6];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 20*p5 + 52*p6 + p8 + 32) >> 6];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 20*p6 + 52*p7 + p9 + 32) >> 6];
		p10 = *pRef;	*pPred_tmp   = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];		
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}
	RvDec_MC_Luma8x8_H0V1_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma8x8_H3V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma8x8_h3_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 13);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 12;
	for (i = 13; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 20*p0 + 52*p1 + p3 + 32) >> 6];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 20*p1 + 52*p2 + p4 + 32) >> 6];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 20*p2 + 52*p3 + p5 + 32) >> 6];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 20*p3 + 52*p4 + p6 + 32) >> 6];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 20*p4 + 52*p5 + p7 + 32) >> 6];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 20*p5 + 52*p6 + p8 + 32) >> 6];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 20*p6 + 52*p7 + p9 + 32) >> 6];
		p10 = *pRef;	*pPred_tmp   = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];		
		
		pRef += srcPitch;
		pPred_tmp += 9;
	}
	RvDec_MC_Luma8x8_H0V2_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);
}

void RvDec_MC_Luma8x8_H3V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	int32 t0, t1;
	int32 b0, b1;
	uint8 *pRef_b = pRef + srcPitch;

	srcPitch -= 8;
	for (i = 8; i > 0; i--)
	{
		t0 = *pRef++;t1 = *pRef++;b0 = *pRef_b++;b1 = *pRef_b++;*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1; b0 = b1; t1 = *pRef++; b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1; b0 = b1; t1 = *pRef++; b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1; b0 = b1; t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1; b0 = b1; t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1; b0 = b1; t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1; b0 = b1; t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1; b0 = b1; t1 = *pRef;	b1 = *pRef_b;	*pPred   = (t0 + t1 + b0 + b1 + 2) >> 2;
		pPred += 9;

		pRef   += srcPitch;
		pRef_b += srcPitch;
	}
}

/*////////////////////////////////////////////////////////////////////////////// */
/* C_MCCopyChroma_H00V00 */
/* */
/*	 0 horizontal displacement */
/*	 0 vertical displacement */
/*	 No interpolation required, simple block copy. */
/* */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H0V0_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
    uint32 dstRow;

    /* Do not perform a sequence of U32 copies, since this function */
    /* is used in contexts where pSrc or pDst is not 4-byte aligned. */

	if (((uint32)pRef&0x3) == 0)
	{
		for (dstRow = 4; dstRow > 0; dstRow--)
		{
			((uint32*)pPred4x4)[0] = ((uint32*)pRef)[0];
			pPred4x4 += MB_CHROMA_SIZE;
			pRef += uFrmWidth;
		}
	}else
	{
		for (dstRow = 4; dstRow > 0; dstRow--)
		{
			pPred4x4 [0] = pRef [0];
			pPred4x4 [1] = pRef [1];
			pPred4x4 [2] = pRef [2];
			pPred4x4 [3] = pRef [3];

			pPred4x4 += MB_CHROMA_SIZE;
			pRef += uFrmWidth;
		}		
	}
}	/* C_MCCopyChroma_H00V00 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	1/4 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (3,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H1V0_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int32 p0, p1, p2, p3, p4;

	uFrmWidth -= 4;
	
	for (j = 4; j > 0; j--)
	{
		p0 = *pRef++;
		p1 = *pRef++;	pPred4x4[0] = (uint8)((3*p0 + p1 + 1)>>2);
		p2 = *pRef++;	pPred4x4[1] = (uint8)((3*p1 + p2 + 1)>>2);
		p3 = *pRef++;	pPred4x4[2] = (uint8)((3*p2 + p3 + 1)>>2);
		p4 = *pRef;		pPred4x4[3] = (uint8)((3*p3 + p4 + 1)>>2);
			
		pRef += uFrmWidth;
		pPred4x4 += MB_CHROMA_SIZE;
	}
}	/* C_MCCopyChroma4_H01V00 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	2/4 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H2V0_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4;
	
	uFrmWidth -= 4;

	for (j = 4; j > 0; j--)
	{
		p0 = *pRef++;
		p1 = *pRef++;	pPred4x4[0] = (uint8)((p0 + p1 + 1)>>1);
		p2 = *pRef++;	pPred4x4[1] = (uint8)((p1 + p2 + 1)>>1);
		p3 = *pRef++;	pPred4x4[2] = (uint8)((p2 + p3 + 1)>>1);
		p4 = *pRef;		pPred4x4[3] = (uint8)((p3 + p4 + 1)>>1);

		pRef += uFrmWidth;
		pPred4x4 += MB_CHROMA_SIZE;
	}
}	/* C_MCCopyChroma4_H02V00 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	3/4 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (1,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H3V0_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4;
	
	uFrmWidth -= 4;

	for (j = 4; j > 0; j--)
	{
		p0 = *pRef++;
		p1 = *pRef++;	*pPred4x4++ = (uint8)((p0 + 3*p1 + 1)>>2);
		p2 = *pRef++;	*pPred4x4++ = (uint8)((p1 + 3*p2 + 1)>>2);
		p3 = *pRef++;	*pPred4x4++ = (uint8)((p2 + 3*p3 + 1)>>2);
		p4 = *pRef;		*pPred4x4	= (uint8)((p3 + 3*p4 + 1)>>2);

		pRef += uFrmWidth;
		pPred4x4 += 5;
	}
}	/* C_MCCopyChroma4_H03V00 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	0 pel horizontal displacement */
/*	1/4 vertical displacement */
/*	Use vertical filter (3,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H0V1_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3;
	int pb0, pb1, pb2, pb3;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 3;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef;

	for (j = 4; j > 0; j--)
	{	
		pb0 = *pRef_b++; *pPred4x4++ = (uint8) ((3*pt0 + pb0 + 2)>>2);	pt0 = pb0;
		pb1 = *pRef_b++; *pPred4x4++ = (uint8) ((3*pt1 + pb1 + 2)>>2);  pt1 = pb1;
		pb2 = *pRef_b++; *pPred4x4++ = (uint8) ((3*pt2 + pb2 + 2)>>2);	pt2 = pb2;
		pb3 = *pRef_b;	 *pPred4x4   = (uint8) ((3*pt3 + pb3 + 2)>>2);	pt3 = pb3;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H00V01 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	0 pel horizontal displacement */
/*	2/4 vertical displacement */
/*	Use vertical filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H0V2_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3;
	int pb0, pb1, pb2, pb3;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 3;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((pt0 + pb0)>>1);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((pt1 + pb1)>>1);pt1 = pb1;
		pb3 = *pRef_b;		*pPred4x4++ = (uint8)((pt2 + pb2)>>1);pt2 = pb2;
							*pPred4x4   = (uint8)((pt3 + pb3)>>1);pt3 = pb3;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H00V02 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	0 pel horizontal displacement */
/*	2/4 vertical displacement */
/*	Use vertical filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H0V3_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3;
	int pb0, pb1, pb2, pb3;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 3;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((pt0 + 3*pb0 + 2)>>2);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((pt1 + 3*pb1 + 2)>>2);pt1 = pb1;
		pb3 = *pRef_b;		*pPred4x4++ = (uint8)((pt2 + 3*pb2 + 2)>>2);pt2 = pb2;
							*pPred4x4   = (uint8)((pt3 + 3*pb3 + 2)>>2);pt3 = pb3;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H00V03 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated chroma 4x4 block copy. */
/*	1/4 pel horizontal displacement */
/*	1/4 vertical displacement */
/*	Use horizontal filter (3,1) */
/*	Use vertical filter (3,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H1V1_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pt0 + 3*(pt1 + pb0) + pb1 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pt1 + 3*(pt2 + pb1) + pb2 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pt2 + 3*(pt3 + pb2) + pb3 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((9*pt3 + 3*(pt4 + pb3) + pb4 + 7)>>4);pt3 = pb3;	
		pt4 = pb4;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H01V01 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	2/4 pel horizontal displacement */
/*	1/4 vertical displacement */
/*	Use horizontal filter (2,2) */
/*	Use vertical filter (3,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H2V1_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt0 + pt1) + pb0 + pb1 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt1 + pt2) + pb1 + pb2 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt2 + pt3) + pb2 + pb3 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((3*(pt3 + pt4) + pb3 + pb4 + 4)>>3);pt3 = pb3;
		pt4 = pb4;
		
		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H02V01 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	3/4 pel horizontal displacement */
/*	1/4 vertical displacement */
/*	Use horizontal filter (1,3) */
/*	Use vertical filter (3,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H3V1_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pt1 + 3*(pt0 + pb1) + pb0 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pt2 + 3*(pt1 + pb2) + pb1 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pt3 + 3*(pt2 + pb3) + pb2 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((9*pt4 + 3*(pt3 + pb4) + pb3 + 7)>>4);pt3 = pb3;
		pt4 = pb4;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H03V01 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	1/4 pel horizontal displacement */
/*	2/4 vertical displacement */
/*	Use horizontal filter (3,1) */
/*	Use vertical filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H1V2_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt0 + pb0) + pt1 + pb1 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt1 + pb1) + pt2 + pb2 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt2 + pb2) + pt3 + pb3 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((3*(pt3 + pb3) + pt4 + pb4 + 4)>>3);pt3 = pb3;
		pt4 = pb4;
	
		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H01V02 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	2/3 pel horizontal displacement */
/*	2/3 vertical displacement */
/*	Use horizontal filter (1,1) */
/*	Use vertical filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H2V2_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;
	
	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((pt0 + pb0 + pt1 + pb1 + 1)>>2);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((pt1 + pb1 + pt2 + pb2 + 1)>>2);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((pt2 + pb2 + pt3 + pb3 + 1)>>2);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((pt3 + pb3 + pt4 + pb4 + 1)>>2);pt3 = pb3;
		pt4 = pb4;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H02V02 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	3/4 pel horizontal displacement */
/*	2/4 vertical displacement */
/*	Use horizontal filter (1,3) */
/*	Use vertical filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H3V2_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt1 + pb1) + pt0 + pb0 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt2 + pb2) + pt1 + pb1 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pt3 + pb3) + pt2 + pb2 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((3*(pt4 + pb4) + pt3 + pb3 + 4)>>3);pt3 = pb3;
		pt4 = pb4;
	
		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H01V02 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	1/4 pel horizontal displacement */
/*	3/4 vertical displacement */
/*	Use horizontal filter (3,1) */
/*	Use vertical filter (1,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H1V3_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pb0 + 3*(pb1 + pt0) + pt1 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pb1 + 3*(pb2 + pt1) + pt2 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pb2 + 3*(pb3 + pt2) + pt3 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((9*pb3 + 3*(pb4 + pt3) + pt4 + 7)>>4);pt3 = pb3;
		pt4 = pb4;
	
		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H01V02 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	2/3 pel horizontal displacement */
/*	2/3 vertical displacement */
/*	Use horizontal filter (3,5) */
/*	Use vertical filter (3,5) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H2V3_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pb0 + pb1) + pt0 + pt1 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pb1 + pb2) + pt1 + pt2 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((3*(pb2 + pb3) + pt2 + pt3 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((3*(pb3 + pb4) + pt3 + pt4 + 4)>>3);pt3 = pb3;
		pt4 = pb4;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H02V03 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	3/4 pel horizontal displacement */
/*	2/4 vertical displacement */
/*	Use horizontal filter (1,3) */
/*	Use vertical filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H3V3_R9(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

	for (j = 4; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pb1 + 3*(pb0 + pt1) + pt0 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pb2 + 3*(pb1 + pt2) + pt1 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred4x4++ = (uint8)((9*pb3 + 3*(pb2 + pt3) + pt2 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b;		*pPred4x4   = (uint8)((9*pb4 + 3*(pb3 + pt4) + pt3 + 7)>>4);pt3 = pb3;
		pt4 = pb4;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H03V03 */
#endif //#if !defined(_ARM_MC_ASSEMBLY_)

void RvDec_InitMCFun_RV9(void)
{
	mc_func *mc_fun_ptr;
	
#if !defined(_ARM_MC_ASSEMBLY_)
	#define INIT_MC_FUN(size, x, y)	mc_fun_ptr[(x<<2)|y] = RvDec_MC_Luma##size##_H##x##V##y##_R9
#else
	#define INIT_MC_FUN(size, x, y)	do{mc_fun_ptr[(x<<2)|y] = arm_mc_luma##size##_dx##x##dy##y##_rv9;}while(0)
#endif
	mc_fun_ptr = g_RvDec_Luma16x16_MC;
	INIT_MC_FUN(16x16,0,0);	INIT_MC_FUN(16x16,1,0);	INIT_MC_FUN(16x16,2,0);	INIT_MC_FUN(16x16,3,0);
	INIT_MC_FUN(16x16,0,1);	INIT_MC_FUN(16x16,1,1);	INIT_MC_FUN(16x16,2,1);	INIT_MC_FUN(16x16,3,1);
	INIT_MC_FUN(16x16,0,2);	INIT_MC_FUN(16x16,1,2);	INIT_MC_FUN(16x16,2,2);	INIT_MC_FUN(16x16,3,2);
	INIT_MC_FUN(16x16,0,3);	INIT_MC_FUN(16x16,1,3);	INIT_MC_FUN(16x16,2,3);	INIT_MC_FUN(16x16,3,3);

	mc_fun_ptr = g_RvDec_Luma8x8_MC;
	INIT_MC_FUN(8x8,0,0);	INIT_MC_FUN(8x8,1,0);	INIT_MC_FUN(8x8,2,0);	INIT_MC_FUN(8x8,3,0);
	INIT_MC_FUN(8x8,0,1);	INIT_MC_FUN(8x8,1,1);	INIT_MC_FUN(8x8,2,1);	INIT_MC_FUN(8x8,3,1);
	INIT_MC_FUN(8x8,0,2);	INIT_MC_FUN(8x8,1,2);	INIT_MC_FUN(8x8,2,2);	INIT_MC_FUN(8x8,3,2);
	INIT_MC_FUN(8x8,0,3);	INIT_MC_FUN(8x8,1,3);	INIT_MC_FUN(8x8,2,3);	INIT_MC_FUN(8x8,3,3);
	#undef INIT_MC_FUN

#if !defined(_ARM_MC_ASSEMBLY_)
	#define INIT_MC_FUN(size, x, y)	mc_fun_ptr[(x<<2)|y] = RvDec_MC_Chroma##size##_H##x##V##y##_R9
#else
	#define INIT_MC_FUN(size, x, y)	do{mc_fun_ptr[(x<<2)|y] = arm_mc_chroma##size##_dx##x##dy##y##_rv9;}while(0)
#endif
	mc_fun_ptr = g_RvDec_Chroma8x8_MC;
	INIT_MC_FUN(8x8,0,0);	INIT_MC_FUN(8x8,1,0);	INIT_MC_FUN(8x8,2,0);	INIT_MC_FUN(8x8,3,0);
	INIT_MC_FUN(8x8,0,1);	INIT_MC_FUN(8x8,1,1);	INIT_MC_FUN(8x8,2,1);	INIT_MC_FUN(8x8,3,1);
	INIT_MC_FUN(8x8,0,2);	INIT_MC_FUN(8x8,1,2);	INIT_MC_FUN(8x8,2,2);	INIT_MC_FUN(8x8,3,2);
	INIT_MC_FUN(8x8,0,3);	INIT_MC_FUN(8x8,1,3);	INIT_MC_FUN(8x8,2,3);	//INIT_MC_FUN(8x8,3,3);
	#if !defined(_ARM_MC_ASSEMBLY_)
		mc_fun_ptr[(3<<2)|3] = RvDec_MC_Chroma8x8_H2V2_R9;//(uint8 * pRef, uint8 * pPred, uint32 uFrmWidth)##x##V##y##_R9
	#else
		mc_fun_ptr[(3<<2)|3] = arm_mc_chroma8x8_dx2dy2_rv9;
	#endif

	mc_fun_ptr = g_RvDec_Chroma4x4_MC;
	INIT_MC_FUN(4x4,0,0);	INIT_MC_FUN(4x4,1,0);	INIT_MC_FUN(4x4,2,0);	INIT_MC_FUN(4x4,3,0);
	INIT_MC_FUN(4x4,0,1);	INIT_MC_FUN(4x4,1,1);	INIT_MC_FUN(4x4,2,1);	INIT_MC_FUN(4x4,3,1);
	INIT_MC_FUN(4x4,0,2);	INIT_MC_FUN(4x4,1,2);	INIT_MC_FUN(4x4,2,2);	INIT_MC_FUN(4x4,3,2);
	INIT_MC_FUN(4x4,0,3);	INIT_MC_FUN(4x4,1,3);	INIT_MC_FUN(4x4,2,3);	//INIT_MC_FUN(4x4,3,3);
	#if !defined(_ARM_MC_ASSEMBLY_)
		mc_fun_ptr[(3<<2)|3] = RvDec_MC_Chroma4x4_H2V2_R9;//(uint8 * pRef, uint8 * pPred, uint32 uFrmWidth)##x##V##y##_R9
	#else
		mc_fun_ptr[(3<<2)|3] = arm_mc_chroma4x4_dx2dy2_rv9;
	#endif
	#undef INIT_MC_FUN	
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
