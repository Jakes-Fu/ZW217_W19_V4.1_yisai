
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
void RvDec_MC_Luma16x16_H0V0_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{	
}

void RvDec_MC_Luma16x16_H0V1_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int32 pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

	pRef -= srcPitch;

	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pRefTemp = pRef + i;
		
		pn1 = *pRefTemp; pRefTemp += srcPitch; 
		p0  = *pRefTemp; pRefTemp += srcPitch; 
		p1  = *pRefTemp; pRefTemp += srcPitch; 
		p2  = *pRefTemp; pRefTemp += srcPitch; 	pPred[0] = pClip[(-pn1 + 6*(2*p0 + p1) - p2 + 8) >> 4];
		p3  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE] = pClip[(-p0 + 6*(2*p1 + p2) - p3 + 8) >> 4];
		p4  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X2] = pClip[(-p1 + 6*(2*p2 + p3) - p4 + 8) >> 4];
		p5  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X3] = pClip[(-p2 + 6*(2*p3 + p4) - p5 + 8) >> 4];
		p6  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X4] = pClip[(-p3 + 6*(2*p4 + p5) - p6 + 8) >> 4];
		p7  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X5] = pClip[(-p4 + 6*(2*p5 + p6) - p7 + 8) >> 4];
		p8  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X6] = pClip[(-p5 + 6*(2*p6 + p7) - p8 + 8) >> 4];
		p9  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 8) >> 4];
		p10 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X8] = pClip[(-p7 + 6*(2*p8 + p9) - p10 + 8) >> 4];
		p11 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X9] = pClip[(-p8 + 6*(2*p9 + p10) - p11 + 8) >> 4];
		p12 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X10] = pClip[(-p9 + 6*(2*p10 + p11) - p12 + 8) >> 4];
		p13 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X11] = pClip[(-p10 + 6*(2*p11 + p12) - p13 + 8) >> 4];
		p14 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X12] = pClip[(-p11 + 6*(2*p12 + p13) - p14 + 8) >> 4];
		p15 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X13] = pClip[(-p12 + 6*(2*p13 + p14) - p15 + 8) >> 4];
		p16 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X14] = pClip[(-p13 + 6*(2*p14 + p15) - p16 + 8) >> 4];
		p17 = *pRefTemp;					 	pPred[MB_SIZE_X15] = pClip[(-p14 + 6*(2*p15 + p16) - p17 + 8) >> 4];

		pPred--;
	}	
}

void RvDec_MC_Luma16x16_H0V2_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int32 pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;
	
	pRef -= srcPitch;

	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pRefTemp = pRef + i;
		
		pn1 = *pRefTemp; pRefTemp += srcPitch; 
		p0  = *pRefTemp; pRefTemp += srcPitch; 
		p1  = *pRefTemp; pRefTemp += srcPitch; 
		p2  = *pRefTemp; pRefTemp += srcPitch; 	pPred[0] = pClip[(-pn1 + 6*(p0 + 2*p1) - p2 + 8) >> 4];
		p3  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE] = pClip[(-p0 + 6*(p1 + 2*p2) - p3 + 8) >> 4];
		p4  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X2] = pClip[(-p1 + 6*(p2 + 2*p3) - p4 + 8) >> 4];
		p5  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X3] = pClip[(-p2 + 6*(p3 + 2*p4) - p5 + 8) >> 4];
		p6  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X4] = pClip[(-p3 + 6*(p4 + 2*p5) - p6 + 8) >> 4];
		p7  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X5] = pClip[(-p4 + 6*(p5 + 2*p6) - p7 + 8) >> 4];
		p8  = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X6] = pClip[(-p5 + 6*(p6 + 2*p7) - p8 + 8) >> 4];
		p9  = *pRefTemp; pRefTemp += srcPitch;	pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(p7 + 2*p8) - p9 + 8) >> 4];
		p10 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X8] = pClip[(-p7 + 6*(p8 + 2*p9) - p10 + 8) >> 4];
		p11 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X9] = pClip[(-p8 + 6*(p9 + 2*p10) - p11 + 8) >> 4];
		p12 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X10] = pClip[(-p9 + 6*(p10 + 2*p11) - p12 + 8) >> 4];
		p13 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X11] = pClip[(-p10 + 6*(p11 + 2*p12) - p13 + 8) >> 4];
		p14 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X12] = pClip[(-p11 + 6*(p12 + 2*p13) - p14 + 8) >> 4];
		p15 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X13] = pClip[(-p12 + 6*(p13 + 2*p14) - p15 + 8) >> 4];
		p16 = *pRefTemp; pRefTemp += srcPitch; 	pPred[MB_SIZE_X14] = pClip[(-p13 + 6*(p14 + 2*p15) - p16 + 8) >> 4];
		p17 = *pRefTemp;						pPred[MB_SIZE_X15] = pClip[(-p14 + 6*(p15 + 2*p16) - p17 + 8) >> 4];
		
		pPred--;
	}	
}

void RvDec_MC_Luma16x16_H1V0_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int32 pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

	srcPitch -= 18;
	pRef -= 1;
	for (i = 16; i > 0; i--)
	{
		pn1 = *pRef++;
		p0  = *pRef++;
		p1  = *pRef++;
		p2  = *pRef++;	*pPred++ = pClip[(-pn1 + 6*(2*p0 + p1) - p2 + 8) >> 4];
		p3  = *pRef++;	*pPred++ = pClip[(-p0 + 6*(2*p1 + p2) - p3 + 8) >> 4];
		p4  = *pRef++;	*pPred++ = pClip[(-p1 + 6*(2*p2 + p3) - p4 + 8) >> 4];
		p5  = *pRef++;	*pPred++ = pClip[(-p2 + 6*(2*p3 + p4) - p5 + 8) >> 4];
		p6  = *pRef++;	*pPred++ = pClip[(-p3 + 6*(2*p4 + p5) - p6 + 8) >> 4];
		p7  = *pRef++;	*pPred++ = pClip[(-p4 + 6*(2*p5 + p6) - p7 + 8) >> 4];
		p8  = *pRef++;	*pPred++ = pClip[(-p5 + 6*(2*p6 + p7) - p8 + 8) >> 4];
		p9  = *pRef++;	*pPred++ = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 8) >> 4];
		p10 = *pRef++;	*pPred++ = pClip[(-p7 + 6*(2*p8 + p9) - p10 + 8) >> 4];
		p11 = *pRef++;	*pPred++ = pClip[(-p8 + 6*(2*p9 + p10) - p11 + 8) >> 4];
		p12 = *pRef++;	*pPred++ = pClip[(-p9 + 6*(2*p10 + p11) - p12 + 8) >> 4];
		p13 = *pRef++;	*pPred++ = pClip[(-p10 + 6*(2*p11 + p12) - p13 + 8) >> 4];
		p14 = *pRef++;	*pPred++ = pClip[(-p11 + 6*(2*p12 + p13) - p14 + 8) >> 4];
		p15 = *pRef++;	*pPred++ = pClip[(-p12 + 6*(2*p13 + p14) - p15 + 8) >> 4];
		p16 = *pRef++;	*pPred++ = pClip[(-p13 + 6*(2*p14 + p15) - p16 + 8) >> 4];
		p17 = *pRef;	*pPred++ = pClip[(-p14 + 6*(2*p15 + p16) - p17 + 8) >> 4];

		pRef += srcPitch;
	}	
}

void RvDec_MC_Luma16x16_H1V1_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	short * pTmpBfr = (short *)g_int_bfr;

//	rvdec_intp_luma16x16_h1_r8 (pRef-srcPitch, pTmpBfr, srcPitch);
	int i;
	int32 pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

	pRef -= (srcPitch+1);
	srcPitch -= 18;
	for (i = 19; i > 0; i--)
	{
		pn1 = *pRef++;
		p0  = *pRef++;
		p1  = *pRef++;
		p2  = *pRef++;	*pTmpBfr++ = -pn1 + 6*(2*p0 + p1) - p2;
		p3  = *pRef++;	*pTmpBfr++ = -p0 + 6*(2*p1 + p2) - p3;
		p4  = *pRef++;	*pTmpBfr++ = -p1 + 6*(2*p2 + p3) - p4;
		p5  = *pRef++;	*pTmpBfr++ = -p2 + 6*(2*p3 + p4) - p5;
		p6  = *pRef++;	*pTmpBfr++ = -p3 + 6*(2*p4 + p5) - p6;
		p7  = *pRef++;	*pTmpBfr++ = -p4 + 6*(2*p5 + p6) - p7;
		p8  = *pRef++;	*pTmpBfr++ = -p5 + 6*(2*p6 + p7) - p8;
		p9  = *pRef++;	*pTmpBfr++ = -p6 + 6*(2*p7 + p8) - p9;
		p10 = *pRef++;	*pTmpBfr++ = -p7 + 6*(2*p8 + p9) - p10;
		p11 = *pRef++;	*pTmpBfr++ = -p8 + 6*(2*p9 + p10) - p11;
		p12 = *pRef++;	*pTmpBfr++ = -p9 + 6*(2*p10 + p11) - p12;
		p13 = *pRef++;	*pTmpBfr++ = -p10 + 6*(2*p11 + p12) - p13;
		p14 = *pRef++;	*pTmpBfr++ = -p11 + 6*(2*p12 + p13) - p14;
		p15 = *pRef++;	*pTmpBfr++ = -p12 + 6*(2*p13 + p14) - p15;
		p16 = *pRef++;	*pTmpBfr++ = -p13 + 6*(2*p14 + p15) - p16;
		p17 = *pRef;	*pTmpBfr++ = -p14 + 6*(2*p15 + p16) - p17;
		
		pRef += srcPitch;
	}	
//	rvdec_intp_luma16x16_v1_r8 (pTmpBfr+MB_SIZE, pPred);
{
	uint8 * pClip = g_rgiRvClipTab;
	int16 * pRefTemp;

	pTmpBfr = (short *)g_int_bfr;
	
	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pRefTemp = pTmpBfr + i;
		
		pn1 = *pRefTemp; pRefTemp += MB_SIZE; 
		p0  = *pRefTemp; pRefTemp += MB_SIZE; 
		p1  = *pRefTemp; pRefTemp += MB_SIZE; 
		p2  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[0] = pClip[(-pn1 + 6*(2*p0 + p1) - p2 + 128) >> 8];
		p3  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE] = pClip[(-p0 + 6*(2*p1 + p2) - p3 + 128) >> 8];
		p4  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X2] = pClip[(-p1 + 6*(2*p2 + p3) - p4 + 128) >> 8];
		p5  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X3] = pClip[(-p2 + 6*(2*p3 + p4) - p5 + 128) >> 8];
		p6  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X4] = pClip[(-p3 + 6*(2*p4 + p5) - p6 + 128) >> 8];
		p7  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X5] = pClip[(-p4 + 6*(2*p5 + p6) - p7 + 128) >> 8];
		p8  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X6] = pClip[(-p5 + 6*(2*p6 + p7) - p8 + 128) >> 8];
		p9  = *pRefTemp; pRefTemp += MB_SIZE;	pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 128) >> 8];
		p10 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X8] = pClip[(-p7 + 6*(2*p8 + p9) - p10 + 128) >> 8];
		p11 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X9] = pClip[(-p8 + 6*(2*p9 + p10) - p11 + 128) >> 8];
		p12 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X10] = pClip[(-p9 + 6*(2*p10 + p11) - p12 + 128) >> 8];
		p13 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X11] = pClip[(-p10 + 6*(2*p11 + p12) - p13 + 128) >> 8];
		p14 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X12] = pClip[(-p11 + 6*(2*p12 + p13) - p14 + 128) >> 8];
		p15 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X13] = pClip[(-p12 + 6*(2*p13 + p14) - p15 + 128) >> 8];
		p16 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X14] = pClip[(-p13 + 6*(2*p14 + p15) - p16 + 128) >> 8];
		p17 = *pRefTemp;						pPred[MB_SIZE_X15] = pClip[(-p14 + 6*(2*p15 + p16) - p17 + 128) >> 8];
		
		pPred--;
	}	
}
}

void RvDec_MC_Luma16x16_H1V2_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	short * pTmpBfr = (short *)g_int_bfr;

	//	rvdec_intp_luma16x16_h1_r8 (pRef-srcPitch, pTmpBfr, srcPitch);
	int i;
	int32 pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

	pRef -= (srcPitch+1);
	srcPitch -= 18;
	for (i = 19; i > 0; i--)
	{
		pn1 = *pRef++;
		p0  = *pRef++;
		p1  = *pRef++;
		p2  = *pRef++;	*pTmpBfr++ = -pn1 + 6*(2*p0 + p1) - p2;
		p3  = *pRef++;	*pTmpBfr++ = -p0 + 6*(2*p1 + p2) - p3;
		p4  = *pRef++;	*pTmpBfr++ = -p1 + 6*(2*p2 + p3) - p4;
		p5  = *pRef++;	*pTmpBfr++ = -p2 + 6*(2*p3 + p4) - p5;
		p6  = *pRef++;	*pTmpBfr++ = -p3 + 6*(2*p4 + p5) - p6;
		p7  = *pRef++;	*pTmpBfr++ = -p4 + 6*(2*p5 + p6) - p7;
		p8  = *pRef++;	*pTmpBfr++ = -p5 + 6*(2*p6 + p7) - p8;
		p9  = *pRef++;	*pTmpBfr++ = -p6 + 6*(2*p7 + p8) - p9;
		p10 = *pRef++;	*pTmpBfr++ = -p7 + 6*(2*p8 + p9) - p10;
		p11 = *pRef++;	*pTmpBfr++ = -p8 + 6*(2*p9 + p10) - p11;
		p12 = *pRef++;	*pTmpBfr++ = -p9 + 6*(2*p10 + p11) - p12;
		p13 = *pRef++;	*pTmpBfr++ = -p10 + 6*(2*p11 + p12) - p13;
		p14 = *pRef++;	*pTmpBfr++ = -p11 + 6*(2*p12 + p13) - p14;
		p15 = *pRef++;	*pTmpBfr++ = -p12 + 6*(2*p13 + p14) - p15;
		p16 = *pRef++;	*pTmpBfr++ = -p13 + 6*(2*p14 + p15) - p16;
		p17 = *pRef;	*pTmpBfr++ = -p14 + 6*(2*p15 + p16) - p17;
		
		pRef += srcPitch;
	}	
//	rvdec_intp_luma16x16_v2_r8 (pTmpBfr+MB_SIZE, pPred);
{
	uint8 * pClip = g_rgiRvClipTab;
	int16 * pRefTemp;

	pTmpBfr = (short *)g_int_bfr;
	
	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pRefTemp = pTmpBfr + i;
		
		pn1 = *pRefTemp; pRefTemp += MB_SIZE; 
		p0  = *pRefTemp; pRefTemp += MB_SIZE; 
		p1  = *pRefTemp; pRefTemp += MB_SIZE; 
		p2  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[0*MB_SIZE] = pClip[(-pn1 + 6*(p0 + 2*p1) - p2 + 128) >> 8];
		p3  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[1*MB_SIZE] = pClip[(-p0 + 6*(p1 + 2*p2) - p3 + 128) >> 8];
		p4  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[2*MB_SIZE] = pClip[(-p1 + 6*(p2 + 2*p3) - p4 + 128) >> 8];
		p5  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[3*MB_SIZE] = pClip[(-p2 + 6*(p3 + 2*p4) - p5 + 128) >> 8];
		p6  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[4*MB_SIZE] = pClip[(-p3 + 6*(p4 + 2*p5) - p6 + 128) >> 8];
		p7  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[5*MB_SIZE] = pClip[(-p4 + 6*(p5 + 2*p6) - p7 + 128) >> 8];
		p8  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[6*MB_SIZE] = pClip[(-p5 + 6*(p6 + 2*p7) - p8 + 128) >> 8];
		p9  = *pRefTemp; pRefTemp += MB_SIZE;	pPred[7*MB_SIZE] = pClip[(-p6 + 6*(p7 + 2*p8) - p9 + 128) >> 8];
		p10 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[8*MB_SIZE] = pClip[(-p7 + 6*(p8 + 2*p9) - p10 + 128) >> 8];
		p11 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[9*MB_SIZE] = pClip[(-p8 + 6*(p9 + 2*p10) - p11 + 128) >> 8];
		p12 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[10*MB_SIZE] = pClip[(-p9 + 6*(p10 + 2*p11) - p12 + 128) >> 8];
		p13 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[11*MB_SIZE] = pClip[(-p10 + 6*(p11 + 2*p12) - p13 + 128) >> 8];
		p14 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[12*MB_SIZE] = pClip[(-p11 + 6*(p12 + 2*p13) - p14 + 128) >> 8];
		p15 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[13*MB_SIZE] = pClip[(-p12 + 6*(p13 + 2*p14) - p15 + 128) >> 8];
		p16 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[14*MB_SIZE] = pClip[(-p13 + 6*(p14 + 2*p15) - p16 + 128) >> 8];
		p17 = *pRefTemp; 						pPred[15*MB_SIZE] = pClip[(-p14 + 6*(p15 + 2*p16) - p17 + 128) >> 8];
		
		pPred--;
	}
}
}

void RvDec_MC_Luma16x16_H2V0_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int32 pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;
	
	pRef -= 1;
	srcPitch -= 18;
	for (i = 16; i > 0; i--)
	{
		pn1 = *pRef++;
		p0  = *pRef++;
		p1  = *pRef++;
		p2  = *pRef++;	*pPred++ = pClip[(-pn1 + 6*(p0 + 2*p1) - p2 + 8) >> 4];
		p3  = *pRef++;	*pPred++ = pClip[(-p0 + 6*(p1 + 2*p2) - p3 + 8) >> 4];
		p4  = *pRef++;	*pPred++ = pClip[(-p1 + 6*(p2 + 2*p3) - p4 + 8) >> 4];
		p5  = *pRef++;	*pPred++ = pClip[(-p2 + 6*(p3 + 2*p4) - p5 + 8) >> 4];
		p6  = *pRef++;	*pPred++ = pClip[(-p3 + 6*(p4 + 2*p5) - p6 + 8) >> 4];
		p7  = *pRef++;	*pPred++ = pClip[(-p4 + 6*(p5 + 2*p6) - p7 + 8) >> 4];
		p8  = *pRef++;	*pPred++ = pClip[(-p5 + 6*(p6 + 2*p7) - p8 + 8) >> 4];
		p9  = *pRef++;	*pPred++ = pClip[(-p6 + 6*(p7 + 2*p8) - p9 + 8) >> 4];
		p10 = *pRef++;	*pPred++ = pClip[(-p7 + 6*(p8 + 2*p9) - p10 + 8) >> 4];
		p11 = *pRef++;	*pPred++ = pClip[(-p8 + 6*(p9 + 2*p10) - p11 + 8) >> 4];
		p12 = *pRef++;	*pPred++ = pClip[(-p9 + 6*(p10 + 2*p11) - p12 + 8) >> 4];
		p13 = *pRef++;	*pPred++ = pClip[(-p10 + 6*(p11 + 2*p12) - p13 + 8) >> 4];
		p14 = *pRef++;	*pPred++ = pClip[(-p11 + 6*(p12 + 2*p13) - p14 + 8) >> 4];
		p15 = *pRef++;	*pPred++ = pClip[(-p12 + 6*(p13 + 2*p14) - p15 + 8) >> 4];
		p16 = *pRef++;	*pPred++ = pClip[(-p13 + 6*(p14 + 2*p15) - p16 + 8) >> 4];
		p17 = *pRef;	*pPred++ = pClip[(-p14 + 6*(p15 + 2*p16) - p17 + 8) >> 4];
		
		pRef += srcPitch;
	}	
}

void RvDec_MC_Luma16x16_H2V1_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	short * pTmpBfr = (short *)g_int_bfr;

//	rvdec_intp_luma16x16_h2_r8 (pRef-srcPitch, pTmpBfr, srcPitch);
	int i;
	int32 pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;
	
	pRef -= (srcPitch+1);
	srcPitch -= 18;
	for (i = 19; i > 0; i--)
	{
		pn1 = *pRef++;
		p0  = *pRef++;
		p1  = *pRef++;
		p2  = *pRef++;	*pTmpBfr++ = -pn1 + 6*(p0 + 2*p1) - p2;
		p3  = *pRef++;	*pTmpBfr++ = -p0 + 6*(p1 + 2*p2) - p3;
		p4  = *pRef++;	*pTmpBfr++ = -p1 + 6*(p2 + 2*p3) - p4;
		p5  = *pRef++;	*pTmpBfr++ = -p2 + 6*(p3 + 2*p4) - p5;
		p6  = *pRef++;	*pTmpBfr++ = -p3 + 6*(p4 + 2*p5) - p6;
		p7  = *pRef++;	*pTmpBfr++ = -p4 + 6*(p5 + 2*p6) - p7;
		p8  = *pRef++;	*pTmpBfr++ = -p5 + 6*(p6 + 2*p7) - p8;
		p9  = *pRef++;	*pTmpBfr++ = -p6 + 6*(p7 + 2*p8) - p9;
		p10 = *pRef++;	*pTmpBfr++ = -p7 + 6*(p8 + 2*p9) - p10;
		p11 = *pRef++;	*pTmpBfr++ = -p8 + 6*(p9 + 2*p10) - p11;
		p12 = *pRef++;	*pTmpBfr++ = -p9 + 6*(p10 + 2*p11) - p12;
		p13 = *pRef++;	*pTmpBfr++ = -p10 + 6*(p11 + 2*p12) - p13;
		p14 = *pRef++;	*pTmpBfr++ = -p11 + 6*(p12 + 2*p13) - p14;
		p15 = *pRef++;	*pTmpBfr++ = -p12 + 6*(p13 + 2*p14) - p15;
		p16 = *pRef++;	*pTmpBfr++ = -p13 + 6*(p14 + 2*p15) - p16;
		p17 = *pRef;	*pTmpBfr++ = -p14 + 6*(p15 + 2*p16) - p17;
		
		pRef += srcPitch;
	}

//	rvdec_intp_luma16x16_v1_r8 (pTmpBfr+MB_SIZE, pPred);
{
	uint8 * pClip = g_rgiRvClipTab;
	int16 * pRefTemp;

	pTmpBfr = (short *)g_int_bfr;
	
	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pRefTemp = pTmpBfr + i;
		
		pn1 = *pRefTemp; pRefTemp += MB_SIZE; 
		p0  = *pRefTemp; pRefTemp += MB_SIZE; 
		p1  = *pRefTemp; pRefTemp += MB_SIZE; 
		p2  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[0] = pClip[(-pn1 + 6*(2*p0 + p1) - p2 + 128) >> 8];
		p3  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE] = pClip[(-p0 + 6*(2*p1 + p2) - p3 + 128) >> 8];
		p4  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X2] = pClip[(-p1 + 6*(2*p2 + p3) - p4 + 128) >> 8];
		p5  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X3] = pClip[(-p2 + 6*(2*p3 + p4) - p5 + 128) >> 8];
		p6  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X4] = pClip[(-p3 + 6*(2*p4 + p5) - p6 + 128) >> 8];
		p7  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X5] = pClip[(-p4 + 6*(2*p5 + p6) - p7 + 128) >> 8];
		p8  = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X6] = pClip[(-p5 + 6*(2*p6 + p7) - p8 + 128) >> 8];
		p9  = *pRefTemp; pRefTemp += MB_SIZE;	pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 128) >> 8];
		p10 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X8] = pClip[(-p7 + 6*(2*p8 + p9) - p10 + 128) >> 8];
		p11 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X9] = pClip[(-p8 + 6*(2*p9 + p10) - p11 + 128) >> 8];
		p12 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X10] = pClip[(-p9 + 6*(2*p10 + p11) - p12 + 128) >> 8];
		p13 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X11] = pClip[(-p10 + 6*(2*p11 + p12) - p13 + 128) >> 8];
		p14 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X12] = pClip[(-p11 + 6*(2*p12 + p13) - p14 + 128) >> 8];
		p15 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X13] = pClip[(-p12 + 6*(2*p13 + p14) - p15 + 128) >> 8];
		p16 = *pRefTemp; pRefTemp += MB_SIZE; 	pPred[MB_SIZE_X14] = pClip[(-p13 + 6*(2*p14 + p15) - p16 + 128) >> 8];
		p17 = *pRefTemp;						pPred[MB_SIZE_X15] = pClip[(-p14 + 6*(2*p15 + p16) - p17 + 128) >> 8];
		
		pPred--;
	}	
}
}

void RvDec_MC_Luma16x16_H2V2_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	int32 p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;
	short * pTmpBfr = (short *)g_int_bfr;
	uint8 * pClip = g_rgiRvClipTab;

	srcPitch -= 17;
	for (i = 18; i > 0; i--)
	{
		p0 = *pRef++;
		p1 = *pRef++;
		p2 = *pRef++;	*pTmpBfr++ = (6*p0 + p1) + (8*p1 + p2);
		p3 = *pRef++;	*pTmpBfr++ = (6*p1 + p2) + (8*p2 + p3);
		p4 = *pRef++;	*pTmpBfr++ = (6*p2 + p3) + (8*p3 + p4);
		p5 = *pRef++;	*pTmpBfr++ = (6*p3 + p4) + (8*p4 + p5);
		p6 = *pRef++;	*pTmpBfr++ = (6*p4 + p5) + (8*p5 + p6);
		p7 = *pRef++;	*pTmpBfr++ = (6*p5 + p6) + (8*p6 + p7);
		p8 = *pRef++;	*pTmpBfr++ = (6*p6 + p7) + (8*p7 + p8);
		p9 = *pRef++;	*pTmpBfr++ = (6*p7 + p8) + (8*p8 + p9);
		p10 = *pRef++;	*pTmpBfr++ = (6*p8 + p9) + (8*p9 + p10);
		p11 = *pRef++;	*pTmpBfr++ = (6*p9 + p10) + (8*p10 + p11);
		p12 = *pRef++;	*pTmpBfr++ = (6*p10 + p11) + (8*p11 + p12);
		p13 = *pRef++;	*pTmpBfr++ = (6*p11 + p12) + (8*p12 + p13);
		p14 = *pRef++;	*pTmpBfr++ = (6*p12 + p13) + (8*p13 + p14);
		p15 = *pRef++;	*pTmpBfr++ = (6*p13 + p14) + (8*p14 + p15);
		p16 = *pRef++;	*pTmpBfr++ = (6*p14 + p15) + (8*p15 + p16);
		p17 = *pRef;	*pTmpBfr++ = (6*p15 + p16) + (8*p16 + p17);

		pRef += srcPitch;
	//	pTmpBfr += BLOCK_SIZE;
	}

	pPred += 15;
	for (i = 15; i >= 0; i--)
	{
		pTmpBfr = (short *)g_int_bfr+i;
		
		p0 = *pTmpBfr;	pTmpBfr += MB_SIZE;
		p1 = *pTmpBfr;	pTmpBfr += MB_SIZE;
		p2 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[0] = pClip[(6*p0 + 9*p1 + p2 + 128) >> 8];
		p3 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE] = pClip[(6*p1 + 9*p2 + p3 + 128) >> 8];
		p4 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X2] = pClip[(6*p2 + 9*p3 + p4 + 128) >> 8];
		p5 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X3] = pClip[(6*p3 + 9*p4 + p5 + 128) >> 8];
		p6 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X4] = pClip[(6*p4 + 9*p5 + p6 + 128) >> 8];
		p7 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X5] = pClip[(6*p5 + 9*p6 + p7 + 128) >> 8];
		p8 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X6] = pClip[(6*p6 + 9*p7 + p8 + 128) >> 8];
		p9 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X7] = pClip[(6*p7 + 9*p8 + p9 + 128) >> 8];
		p10 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X8] = pClip[(6*p8 + 9*p9 + p10 + 128) >> 8];
		p11 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X9] = pClip[(6*p9 + 9*p10 + p11 + 128) >> 8];
		p12 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X10] = pClip[(6*p10 + 9*p11 + p12 + 128) >> 8];
		p13 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X11] = pClip[(6*p11 + 9*p12 + p13 + 128) >> 8];
		p14 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X12] = pClip[(6*p12 + 9*p13 + p14 + 128) >> 8];
		p15 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X13] = pClip[(6*p13 + 9*p14 + p15 + 128) >> 8];
		p16 = *pTmpBfr;	pTmpBfr += MB_SIZE;	pPred[MB_SIZE_X14] = pClip[(6*p14 + 9*p15 + p16 + 128) >> 8];
		p17 = *pTmpBfr;						pPred[MB_SIZE_X15] = pClip[(6*p15 + 9*p16 + p17 + 128) >> 8];

		pPred--;
	}
}

/* chroma functions */
/* Block size is 4x4 for all. */

/*////////////////////////////////////////////////////////////////////////////// */
/*	 0 horizontal displacement */
/*	 0 vertical displacement */
/*	 No interpolation required, simple block copy. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H0V0_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
}	/* C_MCCopyChroma_H00V00 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	1/3 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (5,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H1V0_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4, p5, p6, p7, p8;

	uFrmWidth -= 8;
	
    for (j = 8; j > 0; j--)
	{
		p0 = *pRef++;	
		p1 = *pRef++;*pPred++ = (uint8) ((5*p0 + 3*p1 + 4)>>3);
		p2 = *pRef++;*pPred++ = (uint8) ((5*p1 + 3*p2 + 4)>>3);
		p3 = *pRef++;*pPred++ = (uint8) ((5*p2 + 3*p3 + 4)>>3);
		p4 = *pRef++;*pPred++ = (uint8) ((5*p3 + 3*p4 + 4)>>3);
		p5 = *pRef++;*pPred++ = (uint8) ((5*p4 + 3*p5 + 4)>>3);
		p6 = *pRef++;*pPred++ = (uint8) ((5*p5 + 3*p6 + 4)>>3);
		p7 = *pRef++;*pPred++ = (uint8) ((5*p6 + 3*p7 + 4)>>3);
		p8 = *pRef;  *pPred++ = (uint8) ((5*p7 + 3*p8 + 4)>>3);

       	pRef += uFrmWidth;
	}
}	/* C_MCCopyChroma_H01V00 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	2/3 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (3,5) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H2V0_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4, p5, p6, p7, p8;
	
	uFrmWidth -= 8;

    for (j = 8; j > 0; j--)
	{
		p0 = *pRef++;	
		p1 = *pRef++;*pPred++ = (uint8) ((3*p0 + 5*p1 + 4)>>3);
		p2 = *pRef++;*pPred++ = (uint8) ((3*p1 + 5*p2 + 4)>>3);
		p3 = *pRef++;*pPred++ = (uint8) ((3*p2 + 5*p3 + 4)>>3);
		p4 = *pRef++;*pPred++ = (uint8) ((3*p3 + 5*p4 + 4)>>3);
		p5 = *pRef++;*pPred++ = (uint8) ((3*p4 + 5*p5 + 4)>>3);
		p6 = *pRef++;*pPred++ = (uint8) ((3*p5 + 5*p6 + 4)>>3);
		p7 = *pRef++;*pPred++ = (uint8) ((3*p6 + 5*p7 + 4)>>3);
		p8 = *pRef;   *pPred++ = (uint8) ((3*p7 + 5*p8 + 4)>>3);

        pRef += uFrmWidth;
	}
}	/* C_MCCopyChroma_H02V00 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	0 pel horizontal displacement */
/*	1/3 vertical displacement */
/*	Use vertical filter (5,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H0V1_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	int32 pb;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 7;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef;

    for (j = 8; j > 0; j--)
	{
		pb = *pRef_b++;*pPred++ = (uint8) ((5*pt0 + 3*pb + 4)>>3);pt0 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((5*pt1 + 3*pb + 4)>>3);pt1 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((5*pt2 + 3*pb + 4)>>3);pt2 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((5*pt3 + 3*pb + 4)>>3);pt3 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((5*pt4 + 3*pb + 4)>>3);pt4 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((5*pt5 + 3*pb + 4)>>3);pt5 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((5*pt6 + 3*pb + 4)>>3);pt6 = pb;
		pb = *pRef_b;  *pPred++ = (uint8) ((5*pt7 + 3*pb + 4)>>3);pt7 = pb;

		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H00V01 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	0 pel horizontal displacement */
/*	2/3 vertical displacement */
/*	Use vertical filter (3,5) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H0V2_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
{
	uint32 j;
	int32 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	int32 pb;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 7;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;
	pt4 = *pRef++;	pt5 = *pRef++;
	pt6 = *pRef++;	pt7 = *pRef;

    for (j = 8; j > 0; j--)
	{
		pb = *pRef_b++;*pPred++ = (uint8) ((3*pt0 + 5*pb + 4)>>3);pt0 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((3*pt1 + 5*pb + 4)>>3);pt1 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((3*pt2 + 5*pb + 4)>>3);pt2 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((3*pt3 + 5*pb + 4)>>3);pt3 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((3*pt4 + 5*pb + 4)>>3);pt4 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((3*pt5 + 5*pb + 4)>>3);pt5 = pb;
		pb = *pRef_b++;*pPred++ = (uint8) ((3*pt6 + 5*pb + 4)>>3);pt6 = pb;
		pb = *pRef_b;  *pPred++ = (uint8) ((3*pt7 + 5*pb + 4)>>3);pt7 = pb;

		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H00V02 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	1/3 pel horizontal displacement */
/*	1/3 vertical displacement */
/*	Use horizontal filter (5,3) */
/*	Use vertical filter (5,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H1V1_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
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

	for (j = 8; j >0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++;*pPred++ = (uint8) ((16*pt0 + 15*(pt1 + pb0) + 9*(pt0 + pb1) + 32)>>6);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8) ((16*pt1 + 15*(pt2 + pb1) + 9*(pt1 + pb2) + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8) ((16*pt2 + 15*(pt3 + pb2) + 9*(pt2 + pb3) + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8) ((16*pt3 + 15*(pt4 + pb3) + 9*(pt3 + pb4) + 32)>>6);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8) ((16*pt4 + 15*(pt5 + pb4) + 9*(pt4 + pb5) + 32)>>6);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8) ((16*pt5 + 15*(pt6 + pb5) + 9*(pt5 + pb6) + 32)>>6);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8) ((16*pt6 + 15*(pt7 + pb6) + 9*(pt6 + pb7) + 32)>>6);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8) ((16*pt7 + 15*(pt8 + pb7) + 9*(pt7 + pb8) + 32)>>6);pt7 = pb7;
		pt8 = pb8;	
		
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H01V01 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	2/3 pel horizontal displacement */
/*	1/3 vertical displacement */
/*	Use horizontal filter (3,5) */
/*	Use vertical filter (5,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H2V1_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
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
		pb1 = *pRef_b++;*pPred++ = (uint8) ((15*(pt0 + pb1) + 9*(pb0 + pt1) + 16*pt1 + 32)>>6);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8) ((15*(pt1 + pb2) + 9*(pb1 + pt2) + 16*pt2 + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8) ((15*(pt2 + pb3) + 9*(pb2 + pt3) + 16*pt3 + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8) ((15*(pt3 + pb4) + 9*(pb3 + pt4) + 16*pt4 + 32)>>6);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8) ((15*(pt4 + pb5) + 9*(pb4 + pt5) + 16*pt5 + 32)>>6);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8) ((15*(pt5 + pb6) + 9*(pb5 + pt6) + 16*pt6 + 32)>>6);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8) ((15*(pt6 + pb7) + 9*(pb6 + pt7) + 16*pt7 + 32)>>6);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8) ((15*(pt7 + pb8) + 9*(pb7 + pt8) + 16*pt8 + 32)>>6);pt7 = pb7;
		pt8 = pb8;	
		
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H02V01 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	1/3 pel horizontal displacement */
/*	2/3 vertical displacement */
/*	Use horizontal filter (5,3) */
/*	Use vertical filter (3,5) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H1V2_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
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
		pb1 = *pRef_b++;*pPred++ = (uint8) ((9*(pt1 + pb0) + 16*pb0 + 15*(pt0 + pb1) + 32)>>6);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8) ((9*(pt2 + pb1) + 16*pb1 + 15*(pt1 + pb2) + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8) ((9*(pt3 + pb2) + 16*pb2 + 15*(pt2 + pb3) + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8) ((9*(pt4 + pb3) + 16*pb3 + 15*(pt3 + pb4) + 32)>>6);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8) ((9*(pt5 + pb4) + 16*pb4 + 15*(pt4 + pb5) + 32)>>6);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8) ((9*(pt6 + pb5) + 16*pb5 + 15*(pt5 + pb6) + 32)>>6);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8) ((9*(pt7 + pb6) + 16*pb6 + 15*(pt6 + pb7) + 32)>>6);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8) ((9*(pt8 + pb7) + 16*pb7 + 15*(pt7 + pb8) + 32)>>6);pt7 = pb7;
		pt8 = pb8;	

		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H01V02 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	Motion compensated 4x4 chroma block copy. */
/*	2/3 pel horizontal displacement */
/*	2/3 vertical displacement */
/*	Use horizontal filter (3,5) */
/*	Use vertical filter (3,5) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma8x8_H2V2_R8(uint8 *pRef, uint8 *pPred, uint32 uFrmWidth)
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
		pb1 = *pRef_b++;*pPred++ = (uint8) ((9*(pt0 + pb1) + 15*(pt1 + pb0) + 16*pb1 + 32)>>6);pt0 = pb0;
		pb2 = *pRef_b++;*pPred++ = (uint8) ((9*(pt1 + pb2) + 15*(pt2 + pb1) + 16*pb2 + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++;*pPred++ = (uint8) ((9*(pt2 + pb3) + 15*(pt3 + pb2) + 16*pb3 + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b++;*pPred++ = (uint8) ((9*(pt3 + pb4) + 15*(pt4 + pb3) + 16*pb4 + 32)>>6);pt3 = pb3;	
		pb5 = *pRef_b++;*pPred++ = (uint8) ((9*(pt4 + pb5) + 15*(pt5 + pb4) + 16*pb5 + 32)>>6);pt4 = pb4;
		pb6 = *pRef_b++;*pPred++ = (uint8) ((9*(pt5 + pb6) + 15*(pt6 + pb5) + 16*pb6 + 32)>>6);pt5 = pb5;
		pb7 = *pRef_b++;*pPred++ = (uint8) ((9*(pt6 + pb7) + 15*(pt7 + pb6) + 16*pb7 + 32)>>6);pt6 = pb6;
		pb8 = *pRef_b;	*pPred++ = (uint8) ((9*(pt7 + pb8) + 15*(pt8 + pb7) + 16*pb8 + 32)>>6);pt7 = pb7;
		pt8 = pb8;	

		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H02V02 */
#endif //#if !defined(_ARM_MC_ASSEMBLY_)
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
