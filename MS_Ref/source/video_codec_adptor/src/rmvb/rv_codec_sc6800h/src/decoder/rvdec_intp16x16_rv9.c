
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#if !defined(_ARM_MC_ASSEMBLY_)
#include "rvdec_video_header.h"
#endif
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#if !defined(_ARM_MC_ASSEMBLY_)
/*horizontal integer*/
void RvDec_MC_Luma16x16_H0V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int32 i;

	if (((uint32)pRef & 0x3) == 0)  //word align
	{
		for (i = 16; i > 0; i--)
		{
			((int *)pPred) [0] = ((int *)pRef) [0];
			((int *)pPred) [1] = ((int *)pRef) [1];
			((int *)pPred) [2] = ((int *)pRef) [2];
			((int *)pPred) [3] = ((int *)pRef) [3];

			pPred += MB_SIZE;
			pRef   += srcPitch;
		}
	}else
	{
		for (i = 16; i > 0; i--)
		{
			*pPred++ = pRef [0];	*pPred++ = pRef [1];
			*pPred++ = pRef [2];	*pPred++ = pRef [3];
			*pPred++ = pRef [4];	*pPred++ = pRef [5];
			*pPred++ = pRef [6];	*pPred++ = pRef [7];
			*pPred++ = pRef [8];	*pPred++ = pRef [9];
			*pPred++ = pRef [10];	*pPred++ = pRef [11];
			*pPred++ = pRef [12];	*pPred++ = pRef [13];
			*pPred++ = pRef [14];	*pPred++ = pRef [15];

			pRef   += srcPitch;
		}
	}

}

void RvDec_MC_Luma16x16_H0V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;

	pRef -= (srcPitch<<1);

	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pRefTemp = pRef + i;
		
		pn2 = *pRefTemp; pRefTemp += srcPitch; 
		pn1 = *pRefTemp; pRefTemp += srcPitch; 
		p0  = *pRefTemp; pRefTemp += srcPitch; 
		p1  = *pRefTemp; pRefTemp += srcPitch; 
		p2  = *pRefTemp; pRefTemp += srcPitch; 
		p3  = *pRefTemp; pRefTemp += srcPitch;	pPred[0] = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE] = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X2] = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X3] = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X4] = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X5] = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X6] = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X7] = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];
		p11 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X8] = pClip[(p6 - 5*(p7+p10) + 52*p8 + 20*p9 + p11 + 32) >> 6];
		p12 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X9] = pClip[(p7 - 5*(p8+p11) + 52*p9 + 20*p10 + p12 + 32) >> 6];
		p13 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X10] = pClip[(p8 - 5*(p9+p12) + 52*p10 + 20*p11 + p13 + 32) >> 6];
		p14 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X11] = pClip[(p9 - 5*(p10+p13) + 52*p11 + 20*p12 + p14 + 32) >> 6];
		p15 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X12] = pClip[(p10 - 5*(p11+p14) + 52*p12 + 20*p13 + p15 + 32) >> 6];
		p16 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X13] = pClip[(p11 - 5*(p12+p15) + 52*p13 + 20*p14 + p16 + 32) >> 6];
		p17 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X14] = pClip[(p12 - 5*(p13+p16) + 52*p14 + 20*p15 + p17 + 32) >> 6];
		p18 = *pRefTemp;						pPred[MB_SIZE_X15] = pClip[(p13 - 5*(p14+p17) + 52*p15 + 20*p16 + p18 + 32) >> 6];

		pPred--;
	}	
}

void RvDec_MC_Luma16x16_H0V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;

	pRef -= (srcPitch<<1);

	pPred += 15;
	for (i = 15; i >= 0; i--)
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
		p8  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X5] = pClip[(p3 - 5*(p4+p7) + 20*(p5+p6) + p8 + 16) >> 5];
		p9  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X6] = pClip[(p4 - 5*(p5+p8) + 20*(p6+p7) + p9 + 16) >> 5];
		p10 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X7] = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];
		p11 = *pRefTemp; pRefTemp += srcPitch;  pPred[MB_SIZE_X8] = pClip[(p6 - 5*(p7+p10) + 20*(p8+p9) + p11 + 16) >> 5];
		p12 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X9] = pClip[(p7 - 5*(p8+p11) + 20*(p9+p10) + p12 + 16) >> 5];
		p13 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X10] = pClip[(p8 - 5*(p9+p12) + 20*(p10+p11) + p13 + 16) >> 5];
		p14 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X11] = pClip[(p9 - 5*(p10+p13) + 20*(p11+p12) + p14 + 16) >> 5];
		p15 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X12] = pClip[(p10 - 5*(p11+p14) + 20*(p12+p13) + p15 + 16) >> 5];
		p16 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X13] = pClip[(p11 - 5*(p12+p15) + 20*(p13+p14) + p16 + 16) >> 5];
		p17 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X14] = pClip[(p12 - 5*(p13+p16) + 20*(p14+p15) + p17 + 16) >> 5];
		p18 = *pRefTemp;						pPred[MB_SIZE_X15] = pClip[(p13 - 5*(p14+p17) + 20*(p15+p16) + p18 + 16) >> 5];

		pPred--;
	}	
}

void RvDec_MC_Luma16x16_H0V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	
	pRef -= (srcPitch<<1);

	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pRefTemp = pRef + i;
		
		pn2 = *pRefTemp; pRefTemp += srcPitch; 
		pn1 = *pRefTemp; pRefTemp += srcPitch; 
		p0  = *pRefTemp; pRefTemp += srcPitch; 
		p1  = *pRefTemp; pRefTemp += srcPitch; 
		p2  = *pRefTemp; pRefTemp += srcPitch; 
		p3  = *pRefTemp; pRefTemp += srcPitch;  pPred[0] = pClip[(pn2 - 5*(pn1+p2) + 20*p0 + 52*p1 + p3 + 32) >> 6];
		p4  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE] = pClip[(pn1 - 5*(p0+p3) + 20*p1 + 52*p2 + p4 + 32) >> 6];
		p5  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X2] = pClip[(p0 - 5*(p1+p4) + 20*p2 + 52*p3 + p5 + 32) >> 6];
		p6  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X3] = pClip[(p1 - 5*(p2+p5) + 20*p3 + 52*p4 + p6 + 32) >> 6];
		p7  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X4] = pClip[(p2 - 5*(p3+p6) + 20*p4 + 52*p5 + p7 + 32) >> 6];
		p8  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X5] = pClip[(p3 - 5*(p4+p7) + 20*p5 + 52*p6 + p8 + 32) >> 6];
		p9  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X6] = pClip[(p4 - 5*(p5+p8) + 20*p6 + 52*p7 + p9 + 32) >> 6];
		p10 = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X7] = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];
		p11 = *pRefTemp; pRefTemp += srcPitch;  pPred[MB_SIZE_X8] = pClip[(p6 - 5*(p7+p10) + 20*p8 + 52*p9 + p11 + 32) >> 6];
		p12 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X9] = pClip[(p7 - 5*(p8+p11) + 20*p9 + 52*p10 + p12 + 32) >> 6];
		p13 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X10] = pClip[(p8 - 5*(p9+p12) + 20*p10 + 52*p11 + p13 + 32) >> 6];
		p14 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X11] = pClip[(p9 - 5*(p10+p13) + 20*p11 + 52*p12 + p14 + 32) >> 6];
		p15 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X12] = pClip[(p10 - 5*(p11+p14) + 20*p12 + 52*p13 + p15 + 32) >> 6];
		p16 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X13] = pClip[(p11 - 5*(p12+p15) + 20*p13 + 52*p14 + p16 + 32) >> 6];
		p17 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X14] = pClip[(p12 - 5*(p13+p16) + 20*p14 + 52*p15 + p17 + 32) >> 6];
		p18 = *pRefTemp; 						pPred[MB_SIZE_X15] = pClip[(p13 - 5*(p14+p17) + 20*p15 + 52*p16 + p18 + 32) >> 6];
		
		pPred--;
	}	
}

void RvDec_MC_Luma16x16_H1V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h1_r9 (pRef, pPred, srcPitch, 16);	
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	
	pRef -= 2;
	srcPitch -= 20;
	for (i = 16; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++;	*pPred++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++;	*pPred++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++;	*pPred++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++;	*pPred++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++;	*pPred++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++;	*pPred++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef++;	*pPred++ = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		p11 = *pRef++;	*pPred++ = pClip[(p6 - 5*(p7+p10) + 52*p8 + 20*p9 + p11 + 32) >> 6];	
		p12 = *pRef++;	*pPred++ = pClip[(p7 - 5*(p8+p11) + 52*p9 + 20*p10 + p12 + 32) >> 6];	
		p13 = *pRef++;	*pPred++ = pClip[(p8 - 5*(p9+p12) + 52*p10 + 20*p11 + p13 + 32) >> 6];	
		p14 = *pRef++;	*pPred++ = pClip[(p9 - 5*(p10+p13) + 52*p11 + 20*p12 + p14 + 32) >> 6];	
		p15 = *pRef++;	*pPred++ = pClip[(p10 - 5*(p11+p14) + 52*p12 + 20*p13 + p15 + 32) >> 6];	
		p16 = *pRef++;	*pPred++ = pClip[(p11 - 5*(p12+p15) + 52*p13 + 20*p14 + p16 + 32) >> 6];	
		p17 = *pRef++;	*pPred++ = pClip[(p12 - 5*(p13+p16) + 52*p14 + 20*p15 + p17 + 32) >> 6];	
		p18 = *pRef;	*pPred++ = pClip[(p13 - 5*(p14+p17) + 52*p15 + 20*p16 + p18 + 32) >> 6];	
		
		pRef += srcPitch;
	}
}

void RvDec_MC_Luma16x16_H1V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h1_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 52*p8 + 20*p9 + p11 + 32) >> 6];	
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 52*p9 + 20*p10 + p12 + 32) >> 6];	
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 52*p10 + 20*p11 + p13 + 32) >> 6];	
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 52*p11 + 20*p12 + p14 + 32) >> 6];	
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 52*p12 + 20*p13 + p15 + 32) >> 6];	
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 52*p13 + 20*p14 + p16 + 32) >> 6];	
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 52*p14 + 20*p15 + p17 + 32) >> 6];	
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 52*p15 + 20*p16 + p18 + 32) >> 6];			
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V1_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma16x16_H1V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h1_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 52*p8 + 20*p9 + p11 + 32) >> 6];	
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 52*p9 + 20*p10 + p12 + 32) >> 6];	
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 52*p10 + 20*p11 + p13 + 32) >> 6];	
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 52*p11 + 20*p12 + p14 + 32) >> 6];	
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 52*p12 + 20*p13 + p15 + 32) >> 6];	
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 52*p13 + 20*p14 + p16 + 32) >> 6];	
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 52*p14 + 20*p15 + p17 + 32) >> 6];	
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 52*p15 + 20*p16 + p18 + 32) >> 6];
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V2_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma16x16_H1V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h1_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0; i--)
	{
		pn2 = *pRef++;
		pn1 = *pRef++;
		p0  = *pRef++; 
		p1  = *pRef++;
		p2  = *pRef++;
		p3  = *pRef++;	*pPred_tmp++ = pClip[(pn2 - 5*(pn1+p2) + 52*p0 + 20*p1 + p3 + 32) >> 6];
		p4  = *pRef++;	*pPred_tmp++ = pClip[(pn1 - 5*(p0+p3) + 52*p1 + 20*p2 + p4 + 32) >> 6];
		p5  = *pRef++;	*pPred_tmp++ = pClip[(p0 - 5*(p1+p4) + 52*p2 + 20*p3 + p5 + 32) >> 6];
		p6  = *pRef++;	*pPred_tmp++ = pClip[(p1 - 5*(p2+p5) + 52*p3 + 20*p4 + p6 + 32) >> 6];
		p7  = *pRef++;	*pPred_tmp++ = pClip[(p2 - 5*(p3+p6) + 52*p4 + 20*p5 + p7 + 32) >> 6];
		p8  = *pRef++;	*pPred_tmp++ = pClip[(p3 - 5*(p4+p7) + 52*p5 + 20*p6 + p8 + 32) >> 6];
		p9  = *pRef++;	*pPred_tmp++ = pClip[(p4 - 5*(p5+p8) + 52*p6 + 20*p7 + p9 + 32) >> 6];
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 52*p7 + 20*p8 + p10 + 32) >> 6];	
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 52*p8 + 20*p9 + p11 + 32) >> 6];	
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 52*p9 + 20*p10 + p12 + 32) >> 6];	
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 52*p10 + 20*p11 + p13 + 32) >> 6];	
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 52*p11 + 20*p12 + p14 + 32) >> 6];	
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 52*p12 + 20*p13 + p15 + 32) >> 6];	
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 52*p13 + 20*p14 + p16 + 32) >> 6];	
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 52*p14 + 20*p15 + p17 + 32) >> 6];	
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 52*p15 + 20*p16 + p18 + 32) >> 6];
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V3_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);		
}

void RvDec_MC_Luma16x16_H2V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h2_r9 (pRef, pPred, srcPitch, 16);	
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	
	pRef -= 2;
	srcPitch -= 20;
	for (i = 16; i > 0 ; i--)
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
		p10 = *pRef++;	*pPred++ = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		p11 = *pRef++;	*pPred++ = pClip[(p6 - 5*(p7+p10) + 20*(p8+p9) + p11 + 16) >> 5];		
		p12 = *pRef++;	*pPred++ = pClip[(p7 - 5*(p8+p11) + 20*(p9+p10) + p12 + 16) >> 5];		
		p13 = *pRef++;	*pPred++ = pClip[(p8 - 5*(p9+p12) + 20*(p10+p11) + p13 + 16) >> 5];		
		p14 = *pRef++;	*pPred++ = pClip[(p9 - 5*(p10+p13) + 20*(p11+p12) + p14 + 16) >> 5];		
		p15 = *pRef++;	*pPred++ = pClip[(p10 - 5*(p11+p14) + 20*(p12+p13) + p15 + 16) >> 5];		
		p16 = *pRef++;	*pPred++ = pClip[(p11 - 5*(p12+p15) + 20*(p13+p14) + p16 + 16) >> 5];		
		p17 = *pRef++;	*pPred++ = pClip[(p12 - 5*(p13+p16) + 20*(p14+p15) + p17 + 16) >> 5];		
		p18 = *pRef;	*pPred++ = pClip[(p13 - 5*(p14+p17) + 20*(p15+p16) + p18 + 16) >> 5];		
		
		pRef += srcPitch;
	}
}

void RvDec_MC_Luma16x16_H2V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h2_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0 ; i--)
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
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 20*(p8+p9) + p11 + 16) >> 5];		
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 20*(p9+p10) + p12 + 16) >> 5];		
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 20*(p10+p11) + p13 + 16) >> 5];		
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 20*(p11+p12) + p14 + 16) >> 5];		
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 20*(p12+p13) + p15 + 16) >> 5];		
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 20*(p13+p14) + p16 + 16) >> 5];		
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 20*(p14+p15) + p17 + 16) >> 5];		
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 20*(p15+p16) + p18 + 16) >> 5];		
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V1_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);
}

void RvDec_MC_Luma16x16_H2V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h2_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0 ; i--)
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
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 20*(p8+p9) + p11 + 16) >> 5];		
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 20*(p9+p10) + p12 + 16) >> 5];		
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 20*(p10+p11) + p13 + 16) >> 5];		
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 20*(p11+p12) + p14 + 16) >> 5];		
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 20*(p12+p13) + p15 + 16) >> 5];		
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 20*(p13+p14) + p16 + 16) >> 5];		
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 20*(p14+p15) + p17 + 16) >> 5];		
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 20*(p15+p16) + p18 + 16) >> 5];		
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V2_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma16x16_H2V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h2_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0 ; i--)
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
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 20*(p7+p8) + p10 + 16) >> 5];		
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 20*(p8+p9) + p11 + 16) >> 5];		
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 20*(p9+p10) + p12 + 16) >> 5];		
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 20*(p10+p11) + p13 + 16) >> 5];		
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 20*(p11+p12) + p14 + 16) >> 5];		
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 20*(p12+p13) + p15 + 16) >> 5];		
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 20*(p13+p14) + p16 + 16) >> 5];		
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 20*(p14+p15) + p17 + 16) >> 5];		
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 20*(p15+p16) + p18 + 16) >> 5];		
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V3_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma16x16_H3V0_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	
	pRef -= 2;
	srcPitch -= 20;
	for (i = 16; i > 0; i--)
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
		p10 = *pRef++;	*pPred++ = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];	
		p11 = *pRef++;	*pPred++ = pClip[(p6 - 5*(p7+p10) + 20*p8 + 52*p9 + p11 + 32) >> 6];	
		p12 = *pRef++;	*pPred++ = pClip[(p7 - 5*(p8+p11) + 20*p9 + 52*p10 + p12 + 32) >> 6];	
		p13 = *pRef++;	*pPred++ = pClip[(p8 - 5*(p9+p12) + 20*p10 + 52*p11 + p13 + 32) >> 6];	
		p14 = *pRef++;	*pPred++ = pClip[(p9 - 5*(p10+p13) + 20*p11 + 52*p12 + p14 + 32) >> 6];	
		p15 = *pRef++;	*pPred++ = pClip[(p10 - 5*(p11+p14) + 20*p12 + 52*p13 + p15 + 32) >> 6];	
		p16 = *pRef++;	*pPred++ = pClip[(p11 - 5*(p12+p15) + 20*p13 + 52*p14 + p16 + 32) >> 6];	
		p17 = *pRef++;	*pPred++ = pClip[(p12 - 5*(p13+p16) + 20*p14 + 52*p15 + p17 + 32) >> 6];	
		p18 = *pRef;	*pPred++ = pClip[(p13 - 5*(p14+p17) + 20*p15 + 52*p16 + p18 + 32) >> 6];	
		
		pRef += srcPitch;
	}
}

void RvDec_MC_Luma16x16_H3V1_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h3_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0; i--)
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
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];	
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 20*p8 + 52*p9 + p11 + 32) >> 6];	
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 20*p9 + 52*p10 + p12 + 32) >> 6];	
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 20*p10 + 52*p11 + p13 + 32) >> 6];	
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 20*p11 + 52*p12 + p14 + 32) >> 6];	
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 20*p12 + 52*p13 + p15 + 32) >> 6];	
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 20*p13 + 52*p14 + p16 + 32) >> 6];	
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 20*p14 + 52*p15 + p17 + 32) >> 6];	
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 20*p15 + 52*p16 + p18 + 32) >> 6];		
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V1_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);	
}

void RvDec_MC_Luma16x16_H3V2_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
//	rvdec_intp_luma16x16_h3_r9 (pRef-srcPitch*2, g_int_bfr, srcPitch, 21);
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn2, pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12, p13, p14, p15, p16, p17, p18;
	uint8 *pPred_tmp = g_int_bfr;
	
	pRef -= (srcPitch*2+2);
	srcPitch -= 20;
	for (i = 21; i > 0; i--)
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
		p10 = *pRef++;	*pPred_tmp++ = pClip[(p5 - 5*(p6+p9) + 20*p7 + 52*p8 + p10 + 32) >> 6];	
		p11 = *pRef++;	*pPred_tmp++ = pClip[(p6 - 5*(p7+p10) + 20*p8 + 52*p9 + p11 + 32) >> 6];	
		p12 = *pRef++;	*pPred_tmp++ = pClip[(p7 - 5*(p8+p11) + 20*p9 + 52*p10 + p12 + 32) >> 6];	
		p13 = *pRef++;	*pPred_tmp++ = pClip[(p8 - 5*(p9+p12) + 20*p10 + 52*p11 + p13 + 32) >> 6];	
		p14 = *pRef++;	*pPred_tmp++ = pClip[(p9 - 5*(p10+p13) + 20*p11 + 52*p12 + p14 + 32) >> 6];	
		p15 = *pRef++;	*pPred_tmp++ = pClip[(p10 - 5*(p11+p14) + 20*p12 + 52*p13 + p15 + 32) >> 6];	
		p16 = *pRef++;	*pPred_tmp++ = pClip[(p11 - 5*(p12+p15) + 20*p13 + 52*p14 + p16 + 32) >> 6];	
		p17 = *pRef++;	*pPred_tmp++ = pClip[(p12 - 5*(p13+p16) + 20*p14 + 52*p15 + p17 + 32) >> 6];	
		p18 = *pRef;	*pPred_tmp++ = pClip[(p13 - 5*(p14+p17) + 20*p15 + 52*p16 + p18 + 32) >> 6];		
		
		pRef += srcPitch;
	}
	RvDec_MC_Luma16x16_H0V2_R9 (g_int_bfr+MB_SIZE*2, pPred, MB_SIZE);
}

void RvDec_MC_Luma16x16_H3V3_R9 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	int32 t0, t1;
	int32 b0, b1;
	uint8 *pRef_b = pRef + srcPitch;

	srcPitch -= 16;
	for (i = 16; i > 0; i--)
	{
		t0 = *pRef++;	t1 = *pRef++;	b0 = *pRef_b++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++;	*pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef++;	b1 = *pRef_b++; *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;
		t0 = t1;b0 = b1;t1 = *pRef;	    b1 = *pRef_b;   *pPred++ = (t0 + t1 + b0 + b1 + 2) >> 2;

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
void RvDec_MC_Chroma8x8_H0V0_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
    uint32 dstRow;

    /* Do not perform a sequence of U32 copies, since this function */
    /* is used in contexts where pSrc or pDst is not 4-byte aligned. */

	if (((uint32)pRef&0x3) == 0)
	{
		for (dstRow = 8; dstRow > 0; dstRow--)
		{
			((uint32*)pPred)[0] = ((uint32*)pRef)[0];
			((uint32*)pPred)[1] = ((uint32*)pRef)[1];
			pPred += MB_CHROMA_SIZE;
			pRef += uFrmWidth;
		}
	}else
	{
		for (dstRow = 8; dstRow > 0; dstRow--)
		{
			*pPred++ = pRef [0];
			*pPred++ = pRef [1];
			*pPred++ = pRef [2];
			*pPred++ = pRef [3];
			*pPred++ = pRef [4];
			*pPred++ = pRef [5];
			*pPred++ = pRef [6];
			*pPred++ = pRef [7];

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
void RvDec_MC_Chroma8x8_H1V0_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 p0, p1, p2, p3, p4, p5, p6, p7, p8;

	uFrmWidth -= 8;
	
	for (j = 8; j > 0; j--)
	{
		p0 = *pRef++;	
		p1 = *pRef++;	*pPred++ = (uint8)((3*p0 + p1 + 1)>>2);
		p2 = *pRef++;	*pPred++ = (uint8)((3*p1 + p2 + 1)>>2);
		p3 = *pRef++;	*pPred++ = (uint8)((3*p2 + p3 + 1)>>2);
		p4 = *pRef++;	*pPred++ = (uint8)((3*p3 + p4 + 1)>>2);
		p5 = *pRef++;	*pPred++ = (uint8)((3*p4 + p5 + 1)>>2);
		p6 = *pRef++;	*pPred++ = (uint8)((3*p5 + p6 + 1)>>2);
		p7 = *pRef++;	*pPred++ = (uint8)((3*p6 + p7 + 1)>>2);
		p8 = *pRef;		*pPred++ = (uint8)((3*p7 + p8 + 1)>>2);	

		pRef += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H01V00 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	2/4 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (1,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H2V0_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4, p5, p6, p7, p8;

	uFrmWidth -= 8;

	for (j = 8; j > 0; j--)
	{
		p0 = *pRef++;
		p1 = *pRef++;	*pPred++ = (uint8)((p0 + p1 + 1)>>1);
		p2 = *pRef++;	*pPred++ = (uint8)((p1 + p2 + 1)>>1);
		p3 = *pRef++;	*pPred++ = (uint8)((p2 + p3 + 1)>>1);
		p4 = *pRef++;	*pPred++ = (uint8)((p3 + p4 + 1)>>1);
		p5 = *pRef++;	*pPred++ = (uint8)((p4 + p5 + 1)>>1);
		p6 = *pRef++;	*pPred++ = (uint8)((p5 + p6 + 1)>>1);
		p7 = *pRef++;	*pPred++ = (uint8)((p6 + p7 + 1)>>1);
		p8 = *pRef;		*pPred++ = (uint8)((p7 + p8 + 1)>>1);

		pRef += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H02V00 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	3/4 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (1,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H3V0_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4, p5, p6, p7, p8;

	uFrmWidth -= 8;

	for (j = 8; j > 0; j--)
	{
		p0 = *pRef++;
		p1 = *pRef++;	*pPred++ = (uint8)((p0 + 3*p1 + 1)>>2);
		p2 = *pRef++;	*pPred++ = (uint8)((p1 + 3*p2 + 1)>>2);
		p3 = *pRef++;	*pPred++ = (uint8)((p2 + 3*p3 + 1)>>2);
		p4 = *pRef++;	*pPred++ = (uint8)((p3 + 3*p4 + 1)>>2);	
		p5 = *pRef++;	*pPred++ = (uint8)((p4 + 3*p5 + 1)>>2);
		p6 = *pRef++;	*pPred++ = (uint8)((p5 + 3*p6 + 1)>>2);
		p7 = *pRef++;	*pPred++ = (uint8)((p6 + 3*p7 + 1)>>2);
		p8 = *pRef;		*pPred++ = (uint8)((p7 + 3*p8 + 1)>>2);		
		
		pRef += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H03V00 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	0 pel horizontal displacement */
/*	1/4 vertical displacement */
/*	Use vertical filter (3,1) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H0V1_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 7;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef;

	for (j = 8; j > 0; j--)
	{	
		pb0 = *pRef_b++;*pPred++ = (uint8) ((3*pt0 + pb0 + 2)>>2);	pt0 = pb0;
		pb1 = *pRef_b++;*pPred++ = (uint8) ((3*pt1 + pb1 + 2)>>2);	pt1 = pb1;
		pb2 = *pRef_b++;*pPred++ = (uint8) ((3*pt2 + pb2 + 2)>>2);	pt2 = pb2;
		pb3 = *pRef_b++;*pPred++ = (uint8) ((3*pt3 + pb3 + 2)>>2);	pt3 = pb3;
		pb4 = *pRef_b++;*pPred++ = (uint8) ((3*pt4 + pb4 + 2)>>2);	pt4 = pb4;
		pb5 = *pRef_b++;*pPred++ = (uint8) ((3*pt5 + pb5 + 2)>>2);	pt5 = pb5;
		pb6 = *pRef_b++;*pPred++ = (uint8) ((3*pt6 + pb6 + 2)>>2);	pt6 = pb6;
		pb7 = *pRef_b;	*pPred++ = (uint8) ((3*pt7 + pb7 + 2)>>2);	pt7 = pb7;
							
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
void RvDec_MC_Chroma8x8_H0V2_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 7;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef;

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;	
		pb1 = *pRef_b++;*pPred++ = (uint8)((pt0 + pb0)>>1);	pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((pt1 + pb1)>>1);	pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((pt2 + pb2)>>1);	pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((pt3 + pb3)>>1);	pt3 = pb3;
		pb5 = *pRef_b++;*pPred++ = (uint8)((pt4 + pb4)>>1);	pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((pt5 + pb5)>>1);	pt5 = pb5;
		pb7 = *pRef_b;	*pPred++ = (uint8)((pt6 + pb6)>>1);	pt6 = pb6;
						*pPred++ = (uint8)((pt7 + pb7)>>1);	pt7 = pb7;

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
void RvDec_MC_Chroma8x8_H0V3_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 7;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef;

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;	
		pb1 = *pRef_b++;*pPred++ = (uint8)((pt0 + 3*pb0 + 2)>>2);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((pt1 + 3*pb1 + 2)>>2);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((pt2 + 3*pb2 + 2)>>2);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((pt3 + 3*pb3 + 2)>>2);pt3 = pb3;
		pb5 = *pRef_b++;*pPred++ = (uint8)((pt4 + 3*pb4 + 2)>>2);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((pt5 + 3*pb5 + 2)>>2);pt5 = pb5;
		pb7 = *pRef_b;	*pPred++ = (uint8)((pt6 + 3*pb6 + 2)>>2);pt6 = pb6;
						*pPred++ = (uint8)((pt7 + 3*pb7 + 2)>>2);pt7 = pb7;
		
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
void RvDec_MC_Chroma8x8_H1V1_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((9*pt0 + 3*(pt1 + pb0) + pb1 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((9*pt1 + 3*(pt2 + pb1) + pb2 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((9*pt2 + 3*(pt3 + pb2) + pb3 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((9*pt3 + 3*(pt4 + pb3) + pb4 + 7)>>4);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8)((9*pt4 + 3*(pt5 + pb4) + pb5 + 7)>>4);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((9*pt5 + 3*(pt6 + pb5) + pb6 + 7)>>4);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((9*pt6 + 3*(pt7 + pb6) + pb7 + 7)>>4);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((9*pt7 + 3*(pt8 + pb7) + pb8 + 7)>>4);pt7 = pb7;			
		pt8 = pb8;	

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
void RvDec_MC_Chroma8x8_H2V1_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((3*(pt0 + pt1) + pb0 + pb1 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((3*(pt1 + pt2) + pb1 + pb2 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((3*(pt2 + pt3) + pb2 + pb3 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((3*(pt3 + pt4) + pb3 + pb4 + 4)>>3);pt3 = pb3;
		pb5 = *pRef_b++;*pPred++ = (uint8)((3*(pt4 + pt5) + pb4 + pb5 + 4)>>3);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((3*(pt5 + pt6) + pb5 + pb6 + 4)>>3);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((3*(pt6 + pt7) + pb6 + pb7 + 4)>>3);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((3*(pt7 + pt8) + pb7 + pb8 + 4)>>3);pt7 = pb7;
		pt8 = pb8;	
		
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
void RvDec_MC_Chroma8x8_H3V1_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((9*pt1 + 3*(pt0 + pb1) + pb0 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((9*pt2 + 3*(pt1 + pb2) + pb1 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((9*pt3 + 3*(pt2 + pb3) + pb2 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((9*pt4 + 3*(pt3 + pb4) + pb3 + 7)>>4);pt3 = pb3;
		pb5 = *pRef_b++;*pPred++ = (uint8)((9*pt5 + 3*(pt4 + pb5) + pb4 + 7)>>4);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((9*pt6 + 3*(pt5 + pb6) + pb5 + 7)>>4);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((9*pt7 + 3*(pt6 + pb7) + pb6 + 7)>>4);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((9*pt8 + 3*(pt7 + pb8) + pb7 + 7)>>4);pt7 = pb7;
		pt8 = pb8;	

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
void RvDec_MC_Chroma8x8_H1V2_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;	*pPred++ = (uint8)((3*(pt0 + pb0) + pt1 + pb1 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;	*pPred++ = (uint8)((3*(pt1 + pb1) + pt2 + pb2 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;	*pPred++ = (uint8)((3*(pt2 + pb2) + pt3 + pb3 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b++;	*pPred++ = (uint8)((3*(pt3 + pb3) + pt4 + pb4 + 4)>>3);pt3 = pb3;
		pb5 = *pRef_b++;	*pPred++ = (uint8)((3*(pt4 + pb4) + pt5 + pb5 + 4)>>3);pt4 = pb4;
		pb6 = *pRef_b++;	*pPred++ = (uint8)((3*(pt5 + pb5) + pt6 + pb6 + 4)>>3);pt5 = pb5;
		pb7 = *pRef_b++;	*pPred++ = (uint8)((3*(pt6 + pb6) + pt7 + pb7 + 4)>>3);pt6 = pb6;
		pb8 = *pRef_b;		*pPred++ = (uint8)((3*(pt7 + pb7) + pt8 + pb8 + 4)>>3);pt7 = pb7;
		pt8 = pb8;	

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
void RvDec_MC_Chroma8x8_H2V2_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((pt0 + pb0 + pt1 + pb1 + 1)>>2);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((pt1 + pb1 + pt2 + pb2 + 1)>>2);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((pt2 + pb2 + pt3 + pb3 + 1)>>2);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((pt3 + pb3 + pt4 + pb4 + 1)>>2);pt3 = pb3;
		pb5 = *pRef_b++;*pPred++ = (uint8)((pt4 + pb4 + pt5 + pb5 + 1)>>2);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((pt5 + pb5 + pt6 + pb6 + 1)>>2);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((pt6 + pb6 + pt7 + pb7 + 1)>>2);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((pt7 + pb7 + pt8 + pb8 + 1)>>2);pt7 = pb7;
		pt8 = pb8;	

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
void RvDec_MC_Chroma8x8_H3V2_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((3*(pt1 + pb1) + pt0 + pb0 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((3*(pt2 + pb2) + pt1 + pb1 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((3*(pt3 + pb3) + pt2 + pb2 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((3*(pt4 + pb4) + pt3 + pb3 + 4)>>3);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8)((3*(pt5 + pb5) + pt4 + pb4 + 4)>>3);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((3*(pt6 + pb6) + pt5 + pb5 + 4)>>3);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((3*(pt7 + pb7) + pt6 + pb6 + 4)>>3);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((3*(pt8 + pb8) + pt7 + pb7 + 4)>>3);pt7 = pb7;		
		pt8 = pb8;	
	
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
void RvDec_MC_Chroma8x8_H1V3_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((9*pb0 + 3*(pb1 + pt0) + pt1 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((9*pb1 + 3*(pb2 + pt1) + pt2 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((9*pb2 + 3*(pb3 + pt2) + pt3 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((9*pb3 + 3*(pb4 + pt3) + pt4 + 7)>>4);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8)((9*pb4 + 3*(pb5 + pt4) + pt5 + 7)>>4);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((9*pb5 + 3*(pb6 + pt5) + pt6 + 7)>>4);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((9*pb6 + 3*(pb7 + pt6) + pt7 + 7)>>4);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((9*pb7 + 3*(pb8 + pt7) + pt8 + 7)>>4);pt7 = pb7;
		pt8 = pb8;	
	
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
void RvDec_MC_Chroma8x8_H2V3_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((3*(pb0 + pb1) + pt0 + pt1 + 4)>>3);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((3*(pb1 + pb2) + pt1 + pt2 + 4)>>3);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((3*(pb2 + pb3) + pt2 + pt3 + 4)>>3);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((3*(pb3 + pb4) + pt3 + pt4 + 4)>>3);pt3 = pb3;
		pb5 = *pRef_b++;*pPred++ = (uint8)((3*(pb4 + pb5) + pt4 + pt5 + 4)>>3);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((3*(pb5 + pb6) + pt5 + pt6 + 4)>>3);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((3*(pb6 + pb7) + pt6 + pt7 + 4)>>3);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((3*(pb7 + pb8) + pt7 + pt8 + 4)>>3);pt7 = pb7;
		pt8 = pb8;	

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
void RvDec_MC_Chroma8x8_H3V3_R9(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	int32 pb0, pb1, pb2, pb3, pb4, pb5, pb6, pb7, pb8;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 8;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef++;
	pt8 = *pRef;	

	for (j = 8; j > 0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8)((9*pb1 + 3*(pb0 + pt1) + pt0 + 7)>>4);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8)((9*pb2 + 3*(pb1 + pt2) + pt1 + 7)>>4);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8)((9*pb3 + 3*(pb2 + pt3) + pt2 + 7)>>4);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8)((9*pb4 + 3*(pb3 + pt4) + pt3 + 7)>>4);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8)((9*pb5 + 3*(pb4 + pt5) + pt4 + 7)>>4);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8)((9*pb6 + 3*(pb5 + pt6) + pt5 + 7)>>4);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8)((9*pb7 + 3*(pb6 + pt7) + pt6 + 7)>>4);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8)((9*pb8 + 3*(pb7 + pt8) + pt7 + 7)>>4);pt7 = pb7;			
		pt8 = pb8;	

		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma4_H03V03 */
#endif //#if !defined(_ARM_MC_ASSEMBLY_)

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
