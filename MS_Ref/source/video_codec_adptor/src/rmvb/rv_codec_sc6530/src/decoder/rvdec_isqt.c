
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

const uint8 s_dec_single_scan[16] = 
{
	0,1,4,8,5,2,3,6,9,12,13,10,7,11,14,15
};

//for intra16x16 or inter16x16
LOCAL void RvDec_ITrans4x4_Intra16x16(int16 *pIn)
{
	int16 *pin;
	int32 a0, a1, a2, a3;
	int32 b0, b1, b2, b3;
	int32 i;
	int32 tmp[16], *ptmp;

	/* horizontal */
	pin = pIn;
	ptmp = tmp;
	for (i = 4; i > 0; i--)
	{
		/* For Telenor complaint CPK, downshifting by 20 is done later during */
		/* reconstruction.  For APK and non-Telenor-complaint CPK,  */
		/* downshifting by 10 was done earlier during dequantization. */
		a0 = *pin++;
		a1 = *pin++;
		a2 = *pin++;
		a3 = *pin++;

		b0 = (a0 + a2) * 13;
		b1 = (a0 - a2) * 13;
		b2 = a1 * 7  - a3 * 17;
		b3 = a1 * 17 + a3 * 7;

		ptmp[0] = (b0 + b3);
		ptmp[3] = (b0 - b3);
		ptmp[1] = (b1 + b2);
		ptmp[2] = (b1 - b2);
		
	//	pin += 4;
		ptmp += 4;
	}

	/* vertical */
	pin = pIn;
	ptmp = tmp;
	for (i = 4; i > 0; i--)
	{
		a0 = *ptmp;	ptmp +=4;
		a1 = *ptmp;	ptmp +=4;
		a2 = *ptmp;	ptmp +=4;
		a3 = *ptmp;	
		
		b0 = (a0 + a2) * 13;
		b1 = (a0 - a2) * 13;
		b2 = a1 * 7  - a3 * 17;
		b3 = a1 * 17 + a3 * 7;

		pin[0]  = (int16)(((b0 + b3) * 48) >> 15);
		pin[12] = (int16)(((b0 - b3) * 48) >> 15);
		pin[4]  = (int16)(((b1 + b2) * 48) >> 15);
		pin[8]  = (int16)(((b1 - b2) * 48) >> 15);
		
		pin ++;
		ptmp -= (12-1);
	}
}

PUBLIC void RvDec_IQT4x4_Intra16x16(int16 *pBuf, uint32 uQP0, uint32 uQP)
{
	uint32 i;
	int32 qp;
	int32 coef;
	int32 idx;
	int32 bNZCoefPresent = FALSE;
	const int32 *quant_tbl = g_BquantTable;
	const uint8 *zigzag = s_dec_single_scan;

	qp = quant_tbl[uQP0];
	for(i = 0; i < 3; i++)
	{
		idx = zigzag[i];
		coef = pBuf[idx];
		if(coef)
		{
			pBuf[idx] = (coef * qp + 8) >> 4;
			bNZCoefPresent = TRUE;
		}
	}

	qp = quant_tbl[uQP];
	for (i = 3; i < 16; i++)
	{
		idx = zigzag[i];
		coef = pBuf[idx];
		if (coef)
		{
			pBuf[idx] = (coef * qp + 8) >> 4;
			bNZCoefPresent = TRUE;
		}
	}

	if(bNZCoefPresent)
	{
		RvDec_ITrans4x4_Intra16x16(pBuf);
	}
}

#if !defined(ITRANS_ASSEMBLY)
PUBLIC void RvDec_ITrans_MC_blk4x4(uint8 *pPred, int16 *pcoeff, int32 width)
{
#if 1
	int32 i;
	int16 m0, m1, m2, m3;
	int32 n0, n1, n2, n3;	
	int16 * src = pcoeff;
	uint8 * pClip = g_rgiRvClipTab;
	
	/* horizontal */
	for (i = 4; i > 0; i--)
	{
		/* For Telenor complaint CPK, downshifting by 20 is done later during */
		/* reconstruction.  For APK and non-Telenor-complaint CPK,  */
		/* downshifting by 10 was done earlier during dequantization. */
		m0 = src[0];
		m1 = src[1];
		m2 = src[2];
		m3 = src[3];
		
		n0 = (m0 + m2) * 13;
		n1 = (m0 - m2) * 13;
		n2 = m1 * 7  - m3 * 17;
		n3 = m1 * 17 + m3 * 7;
		
		*src++ = (int16)(n0 + n3);
		*src++ = (int16)(n1 + n2);
		*src++ = (int16)(n1 - n2);
		*src++ = (int16)(n0 - n3);
	}
	
	/* vertical */
	src = pcoeff;
	for (i = 4; i > 0; i--)
	{
		m0 = src[0];
		m1 = src[4];
		m2 = src[8];
		m3 = src[12];
		n0 = (m0 + m2) * 13;
		n1 = (m0 - m2) * 13;
		n2 = m1 * 7  - m3 * 17;
		n3 = m1 * 17 + m3 * 7;

		m0 = pPred [0];			pPred [width*0] = pClip[((n0 + n3 + 0x200) >> 10) + m0];
		m1 = pPred [width];		pPred [width*1] = pClip[((n1 + n2 + 0x200) >> 10) + m1];
		m2 = pPred [width*2];	pPred [width*2] = pClip[((n1 - n2 + 0x200) >> 10) + m2];	
		m3 = pPred [width*3];	pPred [width*3] = pClip[((n0 - n3 + 0x200) >> 10) + m3];
		
		src++;
		pPred++;
	}
#endif	
}
#endif


#if !defined(ITRANS_ASSEMBLY)
PUBLIC void rvdec_ITrans_MC_blk4x4_d (uint8 * pPred, int16 * pcoeff, int width)
{
	int   i;
	int16 *pin;
	//uint8 p0, p1, p2, p3;
	int16 a0, a1, a2, a3;
	int32 b0, b1, b2, b3;
	uint8 * pClip = g_rgiRvClipTab;
	
	/* horizontal */
	pin = pcoeff;
	for (i = 0; i <= 3; i ++)
	{
		/* For Telenor complaint CPK, downshifting by 20 is done later during */
		/* reconstruction.  For APK and non-Telenor-complaint CPK,  */
		/* downshifting by 10 was done earlier during dequantization. */
		a0 = pin[0];
		a1 = pin[1];
		a2 = pin[2];
		a3 = pin[3];
		
		b0 = (a0 + a2) * 13;
		b1 = (a0 - a2) * 13;
		b2 = a1 * 7  - a3 * 17;
		b3 = a1 * 17 + a3 * 7;
		
		pin[0] = (int16)(b0 + b3);
		pin[3] = (int16)(b0 - b3);
		pin[1] = (int16)(b1 + b2);
		pin[2] = (int16)(b1 - b2);
		
		pin += 4;
	}
	
	/* vertical */
	pin = pcoeff + 1;
	for (i = 0; i <= 1; i ++)
	{
		a0 = pin[0];
		a1 = pin[4];
		a2 = pin[8];
		a3 = pin[12];
		b0 = (a0 + a2) * 13;
		b1 = (a0 - a2) * 13;
		b2 = a1 * 7  - a3 * 17;
		b3 = a1 * 17 + a3 * 7;

		a0 = pPred[0]; pPred[0] = pClip[(((b1 + b2 + 0x200) >> 10) + pPred[0])];
		a1 = pPred[width]; pPred[width] = pClip[(((b0 - b3 + 0x200) >> 10) + pPred[width])];
		
		pPred++;
		pin += 2;
	}
}
//#else
//;
#endif

#if !defined(ITRANS_ASSEMBLY)
PUBLIC void RvDec_ITrans_MC_blk4x4_onlyDC(uint8 *pPred, int32 DCCoeff)
{
	int i;
	uint8 p0, p1, p2, p3;
	uint8 * pClip = g_rgiRvClipTab;
	int v = ((DCCoeff * 13 * 13) + 0x200) >> 10;

	for (i = 4; i > 0; i--)
	{
		p0 = pPred[0];
		p1 = pPred[1];
		p2 = pPred[2];
		p3 = pPred[3];

		pPred[0] = pClip[p0 + v];
		pPred[1] = pClip[p1 + v];
		pPred[2] = pClip[p2 + v];
		pPred[3] = pClip[p3 + v];

		pPred += MB_SIZE;		
	}
}
//#else
//;
#endif

#if !defined(ITRANS_ASSEMBLY)
PUBLIC void rvdec_ITrans_MC_blk4x4_onlyDC_d (uint8 * pPred, int DCCoeff)
{

	uint8 * pClip = g_rgiRvClipTab;
	int v = ((DCCoeff * 13 * 13) + 0x200) >> 10;

	pPred[0] =       pClip[v + pPred[0]];
	pPred[1] =       pClip[v + pPred[1]];
	pPred[MB_SIZE/2] =   pClip[v + pPred[MB_SIZE/2]];
	pPred[MB_SIZE/2+1] = pClip[v + pPred[MB_SIZE/2+1]];

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
