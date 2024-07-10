
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#if defined(RV8_SUPPORTED)
#include "rvdec_video_header.h"
#endif
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#if defined(RV8_SUPPORTED)

#if !defined(_ARM_MC_ASSEMBLY_)
void RvDec_MC_Luma8x8_H0V0_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{	
}

void RvDec_MC_Luma8x8_H0V1_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;

	pRef -= srcPitch;

	pPred += 7;
	for (i = 7; i >= 0; i--)
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
		p9  = *pRefTemp;					 	pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 8) >> 4];

		pPred--;
	}	
}

void RvDec_MC_Luma8x8_H0V2_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	uint8 * pRefTemp;
	int pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	
	pRef -= srcPitch;

	pPred += 7;
	for (i = 7; i >= 0; i--)
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
		p9  = *pRefTemp; 					 		pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(p7 + 2*p8) - p9 + 8) >> 4];
		
		pPred--;
	}	
}

void RvDec_MC_Luma8x8_H1V0_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;

	srcPitch -= 10;
	pRef -= 1;
	for (i = 8; i > 0; i--)
	{
		pn1 = *pRef++;
		p0  = *pRef++;
		p1  = *pRef++;
		p2  = *pRef++;	pPred[0] = pClip[(-pn1 + 6*(2*p0 + p1) - p2 + 8) >> 4];
		p3  = *pRef++;	pPred[1] = pClip[(-p0 + 6*(2*p1 + p2) - p3 + 8) >> 4];
		p4  = *pRef++;	pPred[2] = pClip[(-p1 + 6*(2*p2 + p3) - p4 + 8) >> 4];
		p5  = *pRef++;	pPred[3] = pClip[(-p2 + 6*(2*p3 + p4) - p5 + 8) >> 4];
		p6  = *pRef++;	pPred[4] = pClip[(-p3 + 6*(2*p4 + p5) - p6 + 8) >> 4];
		p7  = *pRef++;	pPred[5] = pClip[(-p4 + 6*(2*p5 + p6) - p7 + 8) >> 4];
		p8  = *pRef++;	pPred[6] = pClip[(-p5 + 6*(2*p6 + p7) - p8 + 8) >> 4];
		p9  = *pRef;	pPred[7] = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 8) >> 4];

		pRef += srcPitch;
		pPred += MB_SIZE;
	}	
}

void RvDec_MC_Luma8x8_H1V1_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	short * pTmpBfr = (short *)g_int_bfr;

//	rvdec_intp_luma8x8_h1_r8 (pRef-srcPitch, pTmpBfr, srcPitch);
	int i;
	int pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	
	pRef -= (srcPitch+1);
	srcPitch -= 10;
	for (i = 11; i > 0; i--)
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
		p9  = *pRef;	*pTmpBfr++ = -p6 + 6*(2*p7 + p8) - p9;
		
		pRef += srcPitch;
	}	
//	rvdec_intp_luma8x8_v1_r8 (pTmpBfr+BLOCK_SIZE, pPred);
{
	uint8 * pClip = g_rgiRvClipTab;
	int16 * pRefTemp;

	pTmpBfr = (short *)g_int_bfr;
	
	pPred += 7;
	for (i = 7; i >= 0; i--)
	{
		pRefTemp = pTmpBfr + i;
		
		pn1 = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p0  = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p1  = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p2  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[0] = pClip[(-pn1 + 6*(2*p0 + p1) - p2 + 128) >> 8];
		p3  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE] = pClip[(-p0 + 6*(2*p1 + p2) - p3 + 128) >> 8];
		p4  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X2] = pClip[(-p1 + 6*(2*p2 + p3) - p4 + 128) >> 8];
		p5  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X3] = pClip[(-p2 + 6*(2*p3 + p4) - p5 + 128) >> 8];
		p6  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X4] = pClip[(-p3 + 6*(2*p4 + p5) - p6 + 128) >> 8];
		p7  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X5] = pClip[(-p4 + 6*(2*p5 + p6) - p7 + 128) >> 8];
		p8  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X6] = pClip[(-p5 + 6*(2*p6 + p7) - p8 + 128) >> 8];
		p9  = *pRefTemp; 							pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 128) >> 8];
		
		pPred--;
	}	
}
}

void RvDec_MC_Luma8x8_H1V2_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	short * pTmpBfr = (short *)g_int_bfr;

//	rvdec_intp_luma8x8_h1_r8 (pRef-srcPitch, pTmpBfr, srcPitch);
	int i;
	int pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	
	pRef -= (srcPitch+1);
	srcPitch -= 10;
	for (i = 11; i > 0; i--)
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
		p9  = *pRef;	*pTmpBfr++ = -p6 + 6*(2*p7 + p8) - p9;
		
		pRef += srcPitch;
	}	
//	rvdec_intp_luma8x8_v2_r8 (pTmpBfr+BLOCK_SIZE, pPred);
{
	uint8 * pClip = g_rgiRvClipTab;
	int16 * pRefTemp;

	pTmpBfr = (short *)g_int_bfr;
	
	pPred += 7;
	for (i = 7; i >= 0; i--)
	{
		pRefTemp = pTmpBfr + i;
		
		pn1 = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p0  = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p1  = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p2  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[0*MB_SIZE] = pClip[(-pn1 + 6*(p0 + 2*p1) - p2 + 128) >> 8];
		p3  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[1*MB_SIZE] = pClip[(-p0 + 6*(p1 + 2*p2) - p3 + 128) >> 8];
		p4  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[2*MB_SIZE] = pClip[(-p1 + 6*(p2 + 2*p3) - p4 + 128) >> 8];
		p5  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[3*MB_SIZE] = pClip[(-p2 + 6*(p3 + 2*p4) - p5 + 128) >> 8];
		p6  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[4*MB_SIZE] = pClip[(-p3 + 6*(p4 + 2*p5) - p6 + 128) >> 8];
		p7  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[5*MB_SIZE] = pClip[(-p4 + 6*(p5 + 2*p6) - p7 + 128) >> 8];
		p8  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[6*MB_SIZE] = pClip[(-p5 + 6*(p6 + 2*p7) - p8 + 128) >> 8];
		p9  = *pRefTemp; 							pPred[7*MB_SIZE] = pClip[(-p6 + 6*(p7 + 2*p8) - p9 + 128) >> 8];
		
		pPred--;
	}
}
}

void RvDec_MC_Luma8x8_H2V0_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	uint8 * pClip = g_rgiRvClipTab;
	int pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	
	srcPitch -= 10;
	pRef -= 1;
	for (i = 8; i > 0; i--)
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
		p9  = *pRef;	*pPred	 = pClip[(-p6 + 6*(p7 + 2*p8) - p9 + 8) >> 4];
		
		pRef += srcPitch;
		pPred += 9;
	}	
}

void RvDec_MC_Luma8x8_H2V1_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	short * pTmpBfr = (short *)g_int_bfr;

//	rvdec_intp_luma8x8_h2_r8 (pRef-srcPitch, pTmpBfr, srcPitch);
	int i;
	int pn1, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	
	pRef -= (srcPitch+1);
	srcPitch -= 10;
	for (i = 11; i > 0; i--)
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
		p9  = *pRef;	*pTmpBfr++ = -p6 + 6*(p7 + 2*p8) - p9;
		
		pRef += srcPitch;
	}
//	rvdec_intp_luma8x8_v1_r8 (pTmpBfr+BLOCK_SIZE, pPred);
{
	uint8 * pClip = g_rgiRvClipTab;
	int16 * pRefTemp;

	pTmpBfr = (short *)g_int_bfr;
	
	pPred += 7;
	for (i = 7; i >= 0; i--)
	{
		pRefTemp = pTmpBfr + i;
		
		pn1 = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p0  = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p1  = *pRefTemp; pRefTemp += BLOCK_SIZE; 
		p2  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[0] = pClip[(-pn1 + 6*(2*p0 + p1) - p2 + 128) >> 8];
		p3  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE] = pClip[(-p0 + 6*(2*p1 + p2) - p3 + 128) >> 8];
		p4  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X2] = pClip[(-p1 + 6*(2*p2 + p3) - p4 + 128) >> 8];
		p5  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X3] = pClip[(-p2 + 6*(2*p3 + p4) - p5 + 128) >> 8];
		p6  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X4] = pClip[(-p3 + 6*(2*p4 + p5) - p6 + 128) >> 8];
		p7  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X5] = pClip[(-p4 + 6*(2*p5 + p6) - p7 + 128) >> 8];
		p8  = *pRefTemp; pRefTemp += BLOCK_SIZE; 	pPred[MB_SIZE_X6] = pClip[(-p5 + 6*(2*p6 + p7) - p8 + 128) >> 8];
		p9  = *pRefTemp; 							pPred[MB_SIZE_X7] = pClip[(-p6 + 6*(2*p7 + p8) - p9 + 128) >> 8];
		
		pPred--;
	}	
}
}

void RvDec_MC_Luma8x8_H2V2_R8 (uint8 * pRef, uint8 * pPred, uint32 srcPitch)
{
	int i;
	int p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	short * pTmpBfr = (short *)g_int_bfr;
	uint8 * pClip = g_rgiRvClipTab;

	srcPitch -= 9;
	for (i = 10; i > 0; i--)
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
		p9 = *pRef;		*pTmpBfr++ = (6*p7 + p8) + (8*p8 + p9);

		pRef += srcPitch;
	//	pTmpBfr += BLOCK_SIZE;
	}

	pPred += 7;
	for (i = 7; i >= 0; i--)
	{
		pTmpBfr = (short *)g_int_bfr+i;
		
		p0 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;
		p1 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;
		p2 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;	pPred[0] = pClip[(6*p0 + 9*p1 + p2 + 128) >> 8];
		p3 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;	pPred[MB_SIZE] = pClip[(6*p1 + 9*p2 + p3 + 128) >> 8];
		p4 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;	pPred[MB_SIZE_X2] = pClip[(6*p2 + 9*p3 + p4 + 128) >> 8];
		p5 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;	pPred[MB_SIZE_X3] = pClip[(6*p3 + 9*p4 + p5 + 128) >> 8];
		p6 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;	pPred[MB_SIZE_X4] = pClip[(6*p4 + 9*p5 + p6 + 128) >> 8];
		p7 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;	pPred[MB_SIZE_X5] = pClip[(6*p5 + 9*p6 + p7 + 128) >> 8];
		p8 = *pTmpBfr;	pTmpBfr += BLOCK_SIZE;	pPred[MB_SIZE_X6] = pClip[(6*p6 + 9*p7 + p8 + 128) >> 8];
		p9 = *pTmpBfr;							pPred[MB_SIZE_X7] = pClip[(6*p7 + 9*p8 + p9 + 128) >> 8];

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
void RvDec_MC_Chroma4x4_H0V0_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
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
		uint32 fourPxl;
		int p0, p1, p2, p3;
		for (dstRow = 4; dstRow > 0; dstRow--)
		{
			p0 = pRef [0];
			p1 = pRef [1];
			p2 = pRef [2];
			p3 = pRef [3];
#if defined(RV_BIG_ENDIAN)
			fourPxl = (p0<<24) | (p1<<16) | (p2<<8) | p3;
#else
			fourPxl = (p3<<24) | (p2<<16) | (p1<<8) | p0;			
#endif
			((uint32 *)pPred4x4)[0] = fourPxl;

			pPred4x4 += MB_CHROMA_SIZE;
			pRef += uFrmWidth;
		}		
	}
}	/* C_MCCopyChroma_H00V00 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	1/3 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (5,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H1V0_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4;

	uFrmWidth -= 4;
	
    for (j = 4; j > 0; j--)
	{
		p0 = *pRef++;
		p1 = *pRef++;	*pPred4x4++ = (uint8) ((5*p0 + 3*p1 + 4)>>3);
		p2 = *pRef++;	*pPred4x4++ = (uint8) ((5*p1 + 3*p2 + 4)>>3);
		p3 = *pRef++;	*pPred4x4++ = (uint8) ((5*p2 + 3*p3 + 4)>>3);
		p4 = *pRef;		*pPred4x4   = (uint8) ((5*p3 + 3*p4 + 4)>>3);

		pRef += uFrmWidth;
		pPred4x4 += 5;
	}
}	/* C_MCCopyChroma_H01V00 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	2/3 pel horizontal displacement */
/*	0 vertical displacement */
/*	Use horizontal filter (3,5) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H2V0_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int p0, p1, p2, p3, p4;
	
	uFrmWidth -= 4;
	
    for (j = 4; j > 0; j--)
	{
		p0 = *pRef++;
		p1 = *pRef++;	*pPred4x4++ = (uint8) ((3*p0 + 5*p1 + 4)>>3);
		p2 = *pRef++;	*pPred4x4++ = (uint8) ((3*p1 + 5*p2 + 4)>>3);
		p3 = *pRef++;	*pPred4x4++ = (uint8) ((3*p2 + 5*p3 + 4)>>3);
		p4 = *pRef;		*pPred4x4   = (uint8) ((3*p3 + 5*p4 + 4)>>3);
		
		pRef += uFrmWidth;
		pPred4x4 += 5;
	}
}	/* C_MCCopyChroma_H02V00 */


/*////////////////////////////////////////////////////////////////////////////// */
/*	0 pel horizontal displacement */
/*	1/3 vertical displacement */
/*	Use vertical filter (5,3) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H0V1_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3;
	int pb;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 3;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef;

    for (j = 4; j > 0; j--)
	{
		pb = *pRef_b++;	*pPred4x4++ = (uint8) ((5*pt0 + 3*pb + 4)>>3);pt0 = pb;
		pb = *pRef_b++;	*pPred4x4++ = (uint8) ((5*pt1 + 3*pb + 4)>>3);pt1 = pb;
		pb = *pRef_b++;	*pPred4x4++ = (uint8) ((5*pt2 + 3*pb + 4)>>3);pt2 = pb;
		pb = *pRef_b;	*pPred4x4   = (uint8) ((5*pt3 + 3*pb + 4)>>3);pt3 = pb;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H00V01 */

/*////////////////////////////////////////////////////////////////////////////// */
/*	0 pel horizontal displacement */
/*	2/3 vertical displacement */
/*	Use vertical filter (3,5) */
/*	Dst pitch is DEST_PITCH. */
/*////////////////////////////////////////////////////////////////////////////// */
void RvDec_MC_Chroma4x4_H0V2_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3;
	int pb;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

	uFrmWidth -= 3;
	
	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef;

    for (j = 4; j > 0; j--)
	{
		pb = *pRef_b++;	*pPred4x4++ = (uint8) ((3*pt0 + 5*pb + 4)>>3);pt0 = pb;
		pb = *pRef_b++;	*pPred4x4++ = (uint8) ((3*pt1 + 5*pb + 4)>>3);pt1 = pb;
		pb = *pRef_b++;	*pPred4x4++ = (uint8) ((3*pt2 + 5*pb + 4)>>3);pt2 = pb;
		pb = *pRef_b;	*pPred4x4   = (uint8) ((3*pt3 + 5*pb + 4)>>3);pt3 = pb;

		pPred4x4 += 5;
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
void RvDec_MC_Chroma4x4_H1V1_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
{
	uint32 j;
	int pt0, pt1, pt2, pt3, pt4;
	int pb0, pb1, pb2, pb3, pb4;
	uint8 *pRef_b = pRef + uFrmWidth; //bottom

  	uFrmWidth -= 4;

	pt0 = *pRef++;	pt1 = *pRef++;
	pt2 = *pRef++;	pt3 = *pRef++;	pt4 = *pRef;

  	for (j = 4; j >0; j--)
	{
		pb0 = *pRef_b++;
		pb1 = *pRef_b++; *pPred4x4++ = (uint8) ((25*pt0 + 15*(pt1 + pb0) + 9*pb1 + 32)>>6);pt0 = pb0;
		pb2 = *pRef_b++; *pPred4x4++ = (uint8) ((25*pt1 + 15*(pt2 + pb1) + 9*pb2 + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++; *pPred4x4++ = (uint8) ((25*pt2 + 15*(pt3 + pb2) + 9*pb3 + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b;	 *pPred4x4   = (uint8) ((25*pt3 + 15*(pt4 + pb3) + 9*pb4 + 32)>>6);pt3 = pb3;
		pt4 = pb4;
		
		pPred4x4 += 5;
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
void RvDec_MC_Chroma4x4_H2V1_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
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
		pb1 = *pRef_b++; *pPred4x4++ = (uint8) ((15*(pt0 + pb1) + 9*pb0 + 25*pt1 + 32)>>6);pt0 = pb0;
		pb2 = *pRef_b++; *pPred4x4++ = (uint8) ((15*(pt1 + pb2) + 9*pb1 + 25*pt2 + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++; *pPred4x4++ = (uint8) ((15*(pt2 + pb3) + 9*pb2 + 25*pt3 + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b;	 *pPred4x4   = (uint8) ((15*(pt3 + pb4) + 9*pb3 + 25*pt4 + 32)>>6);pt3 = pb3;
		pt4 = pb4;
		
		pPred4x4 += 5;
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
void RvDec_MC_Chroma4x4_H1V2_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
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
		pb1 = *pRef_b++; *pPred4x4++ = (uint8) ((9*pt1 + 25*pb0 + 15*(pt0 + pb1) + 32)>>6);pt0 = pb0;
		pb2 = *pRef_b++; *pPred4x4++ = (uint8) ((9*pt2 + 25*pb1 + 15*(pt1 + pb2) + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++; *pPred4x4++ = (uint8) ((9*pt3 + 25*pb2 + 15*(pt2 + pb3) + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b;	 *pPred4x4	 = (uint8) ((9*pt4 + 25*pb3 + 15*(pt3 + pb4) + 32)>>6);pt3 = pb3;
		pt4 = pb4;

		pPred4x4 += 5;
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
void RvDec_MC_Chroma4x4_H2V2_R8(uint8 *pRef, uint8 *pPred4x4, uint32 uFrmWidth)
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
		pb1 = *pRef_b++; *pPred4x4++ = (uint8) ((9*pt0 + 15*(pt1 + pb0) + 25*pb1 + 32)>>6);pt0 = pb0;	
		pb2 = *pRef_b++; *pPred4x4++ = (uint8) ((9*pt1 + 15*(pt2 + pb1) + 25*pb2 + 32)>>6);pt1 = pb1;
		pb3 = *pRef_b++; *pPred4x4++ = (uint8) ((9*pt2 + 15*(pt3 + pb2) + 25*pb3 + 32)>>6);pt2 = pb2;
		pb4 = *pRef_b;	 *pPred4x4	 = (uint8) ((9*pt3 + 15*(pt4 + pb3) + 25*pb4 + 32)>>6);pt3 = pb3;
		pt4 = pb4;

		pPred4x4 += 5;
		pRef_b += uFrmWidth;
	}
}	/* C_MCCopyChroma_H02V02 */
#endif //#if !defined(_ARM_MC_ASSEMBLY_)

void RvDec_InitMCFun_RV8(void)
{
mc_func *mc_fun_ptr;
	
#if !defined(_ARM_MC_ASSEMBLY_)
	#define INIT_MC_FUN(size, x, y)	mc_fun_ptr[(x<<2)|y] = RvDec_MC_Luma##size##_H##x##V##y##_R8
#else
	#define INIT_MC_FUN(size, x, y)	mc_fun_ptr[(x<<2)|y] = arm_mc_luma##size##_dx##x##dy##y##_rv8
#endif
	mc_fun_ptr = g_RvDec_Luma16x16_MC;
	INIT_MC_FUN(16x16,1,0);	INIT_MC_FUN(16x16,2,0);	
	INIT_MC_FUN(16x16,0,1);	INIT_MC_FUN(16x16,1,1);	INIT_MC_FUN(16x16,2,1);	
	INIT_MC_FUN(16x16,0,2);	INIT_MC_FUN(16x16,1,2);	INIT_MC_FUN(16x16,2,2);	
	
	mc_fun_ptr = g_RvDec_Luma8x8_MC;
	INIT_MC_FUN(8x8,1,0);	INIT_MC_FUN(8x8,2,0);
	INIT_MC_FUN(8x8,0,1);	INIT_MC_FUN(8x8,1,1);	INIT_MC_FUN(8x8,2,1);
	INIT_MC_FUN(8x8,0,2);	INIT_MC_FUN(8x8,1,2);	INIT_MC_FUN(8x8,2,2);
	#undef INIT_MC_FUN

#if !defined(_ARM_MC_ASSEMBLY_)
	#define INIT_MC_FUN(size, x, y)	mc_fun_ptr[(x<<2)|y] = RvDec_MC_Chroma##size##_H##x##V##y##_R8
#else
	#define INIT_MC_FUN(size, x, y)	mc_fun_ptr[(x<<2)|y] = arm_mc_chroma##size##_dx##x##dy##y##_rv8
#endif
	mc_fun_ptr = g_RvDec_Chroma8x8_MC;
	INIT_MC_FUN(8x8,1,0);	INIT_MC_FUN(8x8,2,0);	
	INIT_MC_FUN(8x8,0,1);	INIT_MC_FUN(8x8,1,1);	INIT_MC_FUN(8x8,2,1);
	INIT_MC_FUN(8x8,0,2);	INIT_MC_FUN(8x8,1,2);	INIT_MC_FUN(8x8,2,2);

	mc_fun_ptr = g_RvDec_Chroma4x4_MC;
	INIT_MC_FUN(4x4,1,0);	INIT_MC_FUN(4x4,2,0);	
	INIT_MC_FUN(4x4,0,1);	INIT_MC_FUN(4x4,1,1);	INIT_MC_FUN(4x4,2,1);;
	INIT_MC_FUN(4x4,0,2);	INIT_MC_FUN(4x4,1,2);	INIT_MC_FUN(4x4,2,2);
	#undef INIT_MC_FUN	
	
#if !defined(_ARM_MC_ASSEMBLY_)
	g_RvDec_Luma16x16_MC[(0<<2)|0] = RvDec_MC_Luma16x16_H0V0_R9;
	g_RvDec_Luma8x8_MC[(0<<2)|0] = RvDec_MC_Luma8x8_H0V0_R9;
	g_RvDec_Chroma8x8_MC[(0<<2)|0] = RvDec_MC_Chroma8x8_H0V0_R9;
	g_RvDec_Chroma4x4_MC[(0<<2)|0] = RvDec_MC_Chroma4x4_H0V0_R9;
#else
	g_RvDec_Luma16x16_MC[(0<<2)|0] = arm_mc_luma16x16_dx0dy0_rv9;
	g_RvDec_Luma8x8_MC[(0<<2)|0] = arm_mc_luma8x8_dx0dy0_rv9;
	g_RvDec_Chroma8x8_MC[(0<<2)|0] = arm_mc_chroma8x8_dx0dy0_rv9;
	g_RvDec_Chroma4x4_MC[(0<<2)|0] = arm_mc_chroma4x4_dx0dy0_rv9;
#endif	

}
#endif //rv8_supported
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
