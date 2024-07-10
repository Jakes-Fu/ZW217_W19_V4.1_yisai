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

const char clip_table[3][32] =
{
	/*   0         5        10        15        20        25        30 */
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,3,3,3,3,4,5,5},
	{0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,3,3,3,4,4,5,5,5,7,8,9},
};

const int32 alpha_tab[32] = 
{
	128,128,128,128,128,128,128,128,128,128,
		122, 96, 75, 59, 47, 37, 29, 23, 18, 15,
		13, 11,  10, 9,  8,  7,  6,  5,  4, 3,
		2,  1,
};

const uint8 beta_tab[32] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
		3, 4, 4, 4, 6, 6, 6, 7, 8, 8,
		9, 9,10,10,11,11,12,13,14,15,
		16,17,
};

const int16 ditherL[4*4] = 
{ 
	64, 80, 32, 96,
		48, 80, 64, 48,
		80, 64, 80, 48,
		96, 32, 80, 64
};

const int16 ditherR[4*4] = 
{ 
	64, 48, 96, 32,
		80, 48, 48, 64,
		64, 64, 80, 48,
		32, 96, 48, 64
};

/* 4x4 coded block CBP flags/masks */
const uint32 CBP4x4Mask[24] =
{
	0x000001, 0x000002, 0x000004, 0x000008,
	0x000010, 0x000020, 0x000040, 0x000080,
	0x000100, 0x000200, 0x000400, 0x000800,
	0x001000, 0x002000, 0x004000, 0x008000,
	0x010000, 0x020000, 0x040000, 0x080000,
	0x100000, 0x200000, 0x400000, 0x800000,
};
#define	uTopBlocksMask (CBP4x4Mask[0]|CBP4x4Mask[1]|CBP4x4Mask[2]|CBP4x4Mask[3])
#define uLeftBlocksMask  (CBP4x4Mask[0]|CBP4x4Mask[4]|CBP4x4Mask[8]|CBP4x4Mask[12])
#define uBottomBlocksMask (CBP4x4Mask[12]|CBP4x4Mask[13]|CBP4x4Mask[14]|CBP4x4Mask[15])
#define	uRightBlocksMask (CBP4x4Mask[3]|CBP4x4Mask[7]|CBP4x4Mask[11]|CBP4x4Mask[15])

#define	uTopBlocksMaskChroma (CBP4x4Mask[0]|CBP4x4Mask[1])
#define	uLeftBlocksMaskChroma (CBP4x4Mask[0]|CBP4x4Mask[2])
#define	uBottomBlocksMaskChroma (CBP4x4Mask[2]|CBP4x4Mask[3])
#define	uRightBlocksMaskChroma (CBP4x4Mask[1]|CBP4x4Mask[3])

void rvdec_deblock_mb_r8 (RV_DECODER_T * pDecoder, MB_MODE_T * mb_mode_ptr, uint8 **pMBYUV);

BOOLEAN deblockRefDiff (uint32 mbType, uint32 adjMbType) 
{	
	switch(mbType) 
	{
	case MBTYPE_FORWARD: /* 5 */
		if(adjMbType != MBTYPE_FORWARD)
			return TRUE;
		break;
		
	case MBTYPE_BACKWARD: /* 6 */
		if(adjMbType != MBTYPE_BACKWARD)
			return TRUE;
		break;
		
	case MBTYPE_SKIPPED:		/* 7				 */
	case MBTYPE_DIRECT:		    /* 8		 */
	case MBTYPE_BIDIR:
		if(adjMbType == MBTYPE_FORWARD || adjMbType == MBTYPE_BACKWARD) 
			return TRUE;
		break;
		
	default:
		break;
	}
	
	return FALSE;
}

uint8 getMBStrength(uint32 mbType) 
{
	if(mbType == MBTYPE_INTRA || mbType == MBTYPE_INTRA_16x16 || mbType == MBTYPE_INTER_16x16) 
	{
		return 2;
	}
	else 
	{		
		return 1;
	}
}

void  C_EdgeFilter4H(
			uint8 *pPels,			/* first of 4 */
			uint32 uPitch,
			uint32 uMBPos,			/* offset in position from MB zero pixel for this edge (0,4,8,or 12) */
			int32 Cr,				/* clip for right */
			int32 Cl,				/* clip for left */
			int32 alpha,
			int32 betaX,
			int32 beta2,
			BOOLEAN bChroma,
			BOOLEAN bStrong		/* true when strong filter enabled */
			)
{
	
	int32 d1, d2, n;
    uint8 Ar, Al, C, c, s, t1, t2;	
    uint8 *	pL1, *pL2, *pL3, *pL4, *pR2, *pR3, *pR4;    	
	int32 delta;
	uint8 beta = betaX*4;
    uint32 u4Pitch = uPitch <<2;
    uint32 u3Pitch = u4Pitch - uPitch;
	int32 deltaL[4], deltaL2[4], deltaR[4], deltaR2[4];
	BOOLEAN b3SmoothLeft, b3SmoothRight;
	int32 R1, R2, R3, R4, L1, L2, L3, L4, d, z;	
	uint8 * r = pPels;
	uint8 * pClip = g_rgiRvClipTab;

	pR2 = r + uPitch;
	pR3 = pR2 + uPitch;
	pR4 = pR3 + uPitch;
	pL1 = r - uPitch;
	pL2 = pL1 - uPitch;
	pL3 = pL2 - uPitch;
	pL4 = pL3 - uPitch;            
	
	C = (uint8)(Cr+Cl);
	Al = Ar = 3;
	deltaL[0] = pL2[0] - pL1[0];
	deltaL[1] = pL2[1] - pL1[1];
	deltaL[2] = pL2[2] - pL1[2];
	deltaL[3] = pL2[3] - pL1[3];
	deltaR[0] = pR2[0] - r[0];
	deltaR[1] = pR2[1] - r[1];
	deltaR[2] = pR2[2] - r[2];
	deltaR[3] = pR2[3] - r[3];
	
	delta = deltaL[0]+deltaL[1]+deltaL[2]+deltaL[3];
	if (abs(delta) >= beta) 
		Al = 1;

	delta = deltaR[0]+deltaR[1]+deltaR[2]+deltaR[3];
	if (abs(delta) >= beta) 
		Ar = 1;

	if((Al+Ar) > 2)
	{
		if (bStrong)
		{
			b3SmoothLeft = (Al == 3);
			b3SmoothRight = (Ar == 3);
		}else
		{
			/* not strong, force select weak below */
			b3SmoothLeft = FALSE;
			b3SmoothRight = FALSE;
		}
		
		deltaL2[0] = pL2[0] - pL3[0];
		deltaL2[1] = pL2[1] - pL3[1];
		deltaL2[2] = pL2[2] - pL3[2];
		deltaL2[3] = pL2[3] - pL3[3];
		deltaR2[0] = pR2[0] - pR3[0];
		deltaR2[1] = pR2[1] - pR3[1];
		deltaR2[2] = pR2[2] - pR3[2];
		deltaR2[3] = pR2[3] - pR3[3];

		if (b3SmoothLeft)
		{
			delta = deltaL2[0]+deltaL2[1]+deltaL2[2]+deltaL2[3];
			if (abs(delta) >= beta2)
				b3SmoothLeft = FALSE;
		}

		if (b3SmoothRight)
		{
			delta = deltaR2[0]+deltaR2[1]+deltaR2[2]+deltaR2[3];
			if (abs(delta) >= beta2) 
				b3SmoothRight = FALSE;
		}
		
		c = (C + Al + Ar)>>1;
		if(b3SmoothLeft && b3SmoothRight) //strong
		{
			for(z=0; z<4; z++) 
			{
				L1 = (int32) pL1[z];
				R1 = (int32) r[z];
				delta = (R1 -L1);

				if(delta == 0) 
					continue;

				n = (abs(delta)*alpha)>>7;
				if(n > 1) 
					continue;

				L2 = (int32) pL2[z];
				R2 = (int32) pR2[z];
				L3 = (int32) pL3[z];
				R3 = (int32) pR3[z];
				L4 = (int32) pL4[z];
				R4 = (int32) pR4[z];
#ifdef DITHER
				d1 = ditherL[uMBPos+z];
				d2 = ditherR[uMBPos+z];
#else
				d1 = d2 = 64;
#endif
				t1 = (uint8)((25*L3 + 26*L2 + 26*L1 + 26*R1 + 25*R2 + d1)>>7);
				t2 = (uint8)((25*L2 + 26*L1 + 26*R1 + 26*R2 + 25*R3 + d2)>>7);
				if(n) 
				{
					pL1 [z] = (uint8)IClip (L1-c, L1+c, t1);
					r   [z] = (uint8)IClip (R1-c, R1+c, t2);
				} 
				else 
				{
					pL1[z] = t1;
					r  [z] = t2;
				}

				t1 = (uint8)((25*L4 + 26*L3 + 26*L2 + 26*pL1[z] + 25*R1 + d1)>>7);
				t2 = (uint8)((25*L1 + 26*r[z] + 26*R2 + 26*R3 + 25*R4 + d2)>>7);
				if(n) 
				{
					pL2 [z] = (uint8)IClip (L2-c, L2+c, t1);
					pR2 [z] = (uint8)IClip (R2-c, R2+c, t2);
				} 
				else 
				{
					pL2[z] = t1;
					pR2[z] = t2;
				}
				
				if(!bChroma)
				{
					pL3[z] = (uint8)((26*L4 + 51*L3 + 26*pL2[z] + 25*pL1[z] + 64)>>7);
					pR3[z] = (uint8)((25*r[z] + 26*pR2[z] + 51*R3 + 26*R4  + 64)>>7);
				}
			}
		} 
		else 
		{
			if(Al!=1 && Ar!=1) //normal
			{
				for(z=0;z<4;z++) 
				{
					L1 = (int32) pL1[z];
					R1 = (int32) r[z];
					
					delta = (R1 -L1);
					if(delta == 0) 
						continue;

					if(((abs(delta)*alpha)>>7) > 2)
						continue;
					
					s = c;
					d = ((int32) pL2[z] - (int32) pR2[z] + ((delta) << 2) + 4) >> 3;
					d = IClip (-s, s, d);
					
					pL1[z] = (uint8)pClip[pL1[z] + d];
					r[z]   = (uint8)pClip[r[z] - d];


					if(abs(deltaL2[z]) <=  betaX) 
					{
						d2 = (deltaL[z] + deltaL2[z] - d) >> 1;
						pL2[z] = pClip[pL2[z]-IClip(-Cl, Cl, d2)];
					}

					if(abs(deltaR2[z]) <= betaX) 
					{
						d2 = (deltaR[z] + deltaR2[z] + d) >> 1;
						pR2[z] = pClip[pR2[z] - IClip(-Cr, Cr, d2)];
					}
				} /* z */
			}
			else   //weak
			{ 
				for(z=0; z<4; z++) 
				{
					L1 = (int32) pL1[z];
					R1 = (int32) r[z];

					delta = (R1 -L1);
					if(delta == 0) 
						continue;

					if(((abs(delta)*alpha)>>7)>3) 
						continue;
					
					d = (delta + 1) >> 1;
					s = c >> 1;					
					d = IClip (-s, s, d);
					pL1[z] = (uint8)pClip [pL1[z]+d];
					r[z] = (uint8)pClip [r[z]-d];

					if (abs(deltaL2[z]) <= betaX && Al!=1) 
					{
						d2 = (deltaL[z] + deltaL2[z] - d) >> 1;
						pL2[z] = (uint8)pClip[pL2[z] - IClip(-Cl/2, Cl/2, d2)];
					}

					if (abs(deltaR2[z]) <= betaX && Ar!=1) 
					{
						d2 = (deltaR[z] + deltaR2[z] + d) >> 1;
						pR2[z] = (uint8)pClip[pR2[z] - IClip(-Cr/2, Cr/2, d2)];
					}								
				} /* z */
			}		
		} /* if */
	} /* >2 */
}

void  C_EdgeFilter4V(
			uint8 *pPels,			/* first of 4 */
			uint32 uPitch,
			uint32 uMBPos,			/* offset in position from MB zero pixel for this edge (0,4,8,or 12) */
			int32 Cr,				/* clip for right */
			int32 Cl,				/* clip for left */
			int32 alpha,
			int32 betaX,
			int32 beta2,
			BOOLEAN bChroma,
			BOOLEAN bStrong		/* true when strong filter enabled */
			)
{
	int32 d1, d2, n;
    uint8 Ar, Al, C, c, s, t1, t2;	
	int32 delta;
	uint8 beta = betaX*4;
    uint32 u4Pitch = uPitch <<2;
    uint32 u3Pitch = u4Pitch - uPitch;
	int32 deltaL[4], deltaL2[4], deltaR[4], deltaR2[4];
	BOOLEAN b3SmoothLeft, b3SmoothRight;
	uint8 *pClip = g_rgiRvClipTab;
	int32 R1, R2, R3, R4, L1, L2, L3, L4, d, z;
	uint8 *rt = pPels;

	C = (uint8)(Cr+Cl);
	Al = Ar = 3;
	deltaL[0] = rt[-2] - rt[-1];
	deltaL[1] = rt[uPitch-2] - rt[uPitch-1];
	deltaL[2] = rt[(uPitch<<1)-2] - rt[(uPitch<<1)-1];
	deltaL[3] = rt[u3Pitch-2] -rt[u3Pitch-1];
	deltaR[0] = rt[1] - rt[0];
	deltaR[1] = rt[uPitch+1] - rt[uPitch];
	deltaR[2] = rt[(uPitch<<1)+1] - rt[(uPitch<<1)];
	deltaR[3] = rt[u3Pitch+1] - rt[u3Pitch];

	delta = deltaL[0]+deltaL[1]+deltaL[2]+deltaL[3];
	if (abs(delta) >= beta) 
		Al =1;

	delta = deltaR[0]+deltaR[1]+deltaR[2]+deltaR[3];
	if (abs(delta) >= beta) 
		Ar =1;

	if((Al+Ar) > 2)
	{
		if (bStrong)
		{
			b3SmoothLeft = (Al == 3);
			b3SmoothRight = (Ar == 3);
		}else
		{
			/* not strong, force select weak below */
			b3SmoothLeft = FALSE;
			b3SmoothRight = FALSE;
		}
		
		deltaL2[0] = rt[-2] - rt[-3];
		deltaL2[1] = rt[uPitch-2] - rt[uPitch-3];
		deltaL2[2] = rt[(uPitch<<1)-2] - rt[(uPitch<<1)-3];
		deltaL2[3] = rt[u3Pitch-2] - rt[u3Pitch-3];
		deltaR2[0] = rt[1] - rt[2];
		deltaR2[1] = rt[uPitch+1] - rt[uPitch+2];
		deltaR2[2] = rt[(uPitch<<1)+1] - rt[(uPitch<<1)+2];
		deltaR2[3] = rt[u3Pitch+1] - rt[u3Pitch+2];

		if (b3SmoothLeft)
		{
			delta = deltaL2[0]+deltaL2[1]+deltaL2[2]+deltaL2[3];
			if (abs(delta) >= beta2) 
				b3SmoothLeft = FALSE;
		}
		
		if (b3SmoothRight)
		{
			delta = deltaR2[0]+deltaR2[1]+deltaR2[2]+deltaR2[3];
			if (abs(delta) >= beta2) 
				b3SmoothRight = FALSE;
		}

		c = (C + Al + Ar) >> 1;
		if(b3SmoothLeft && b3SmoothRight) 
		{
			for(z=0; z<4; z++, rt+=uPitch)
			{		
				L1 = (int32) rt[-1];
				R1 = (int32) rt[0];								
				delta = (R1 -L1);

				if(delta == 0) 
					continue;

				n=(abs(delta)*alpha)>>7;
				if(n>1) 
					continue;

				L2 = (int32) rt[-2];
				R2 = (int32) rt[1];
				L3 = (int32) rt[-3];
				R3 = (int32) rt[2];
				L4 = (int32) rt[-4];
				R4 = (int32) rt[3];
#ifdef DITHER
				d1 = ditherL[uMBPos+z];
				d2 = ditherR[uMBPos+z];
#else
				d1=d2=64;
#endif
				t1 = (uint8)((25*L3 + 26*L2 + 26*L1 + 26*R1 + 25*R2 + d1)>>7);
				t2 = (uint8)((25*L2 + 26*L1 + 26*R1 + 26*R2 + 25*R3 + d2)>>7); 
				if(n) 
				{
					rt[-1] = (uint8) IClip (L1-c, L1+c, t1);
					rt[0] = (uint8) IClip (R1-c, R1+c, t2);
				} 
				else 
				{
					rt[-1] = t1;
					rt[0] = t2;
				}

				t1 = (uint8)((25*L4 + 26*L3 + 26*L2 + 26*rt[-1] + 25*R1 + d1)>>7);
				t2 = (uint8)((25*L1 + 26*rt[0] + 26*R2 + 26*R3 + 25*R4 + d2)>>7);
				if(n) 
				{
					rt[-2] = (uint8) IClip (L2-c, L2+c, t1);
					rt[1] = (uint8) IClip (R2-c, R2+c, t2);
				} 
				else 
				{
					rt[-2] = t1;
					rt[1] = t2;
				}

				if(!bChroma)
				{
					rt[-3] = (uint8)((26*L4 + 51*L3 + 26*rt[-2] + 25*rt[-1] + 64)>>7);
					rt[2] = (uint8)((25*rt[0] + 26*rt[1] + 51*R3 + 26*R4    + 64)>>7);
				}
			} /*z */
		}else
		{
			if(Al!=1 && Ar!=1) 
			{ 
				for(z=0; z<4; z++, rt+=uPitch)
				{		
					L1 = (int32) rt[-1];
					R1 = (int32) rt[0];								
					delta = (R1 -L1);

					if(delta == 0) 
						continue;

					if(((abs(delta)*alpha)>>7)>2) 
						continue;

					d = ((int32) rt[-2] - (int32) rt[1] + ((delta) << 2) + 4) >> 3;
					d = IClip (-c, c, d);
					rt[-1] = (uint8)pClip[rt[-1] + d];
					rt[0] = (uint8)pClip[rt[0] - d];

					if(abs(deltaL2[z])<= betaX) 
					{ 
						d2 = (deltaL[z] + deltaL2[z] - d) >> 1;
						d2 = IClip (-Cl, Cl, d2);
						rt[-2] = (uint8)pClip[rt[-2]-d2];
					}

					if(abs(deltaR2[z])<= betaX) 
					{ 
						d2 = (deltaR[z] + deltaR2[z] + d) >> 1;
						d2 = IClip (-Cr, Cr, d2);
						rt[1] = (uint8)pClip[rt[1]-d2];
					}
				}
			}else 
			{						
				for(z=0; z<4; z++, rt+=uPitch)
				{		
					L1 = (int32) rt[-1];
					R1 = (int32) rt[0];								
					delta = (R1 -L1);

					if(delta == 0) 
						continue;

					if(((abs(delta)*alpha)>>7)>3)
						continue;

					s = c>>1;
					d = (delta + 1) >> 1;
					d = IClip(-s, s, d);
					rt[-1] = (uint8)pClip[rt[-1]+d];
					rt[0] = (uint8)pClip[rt[0] - d];
					
					if(abs(deltaL2[z]) <= betaX && Al!=1) 
					{ 
						d2 = (deltaL[z] + deltaL2[z] - d) >> 1;
						d2 = IClip(-Cl/2, Cl/2, d2);
						rt[-2] = (uint8)pClip[rt[-2]-d2];
					}

					if(abs(deltaR2[z])<= betaX && Ar!=1) 
					{ 
						d2 = (deltaR[z] + deltaR2[z] + d) >> 1;
						d2 = IClip (-Cr/2, Cr/2, d2);
						rt[1] = (uint8)pClip[rt[1]-d2];
					}
					
				} /* z */
			}
		} /* if */
	} /* > 2 */

#ifdef DBK_TRACE
	printf_filterEdgeV_pixel (pPels, uPitch);
#endif
}

/************************************************************************
description: deblock one MB
input:
	pMBYUV: MB pixel start address for Y U V
	mvd: mv differential factor which will affect edge filtering, 
	     and computed in decoding process
************************************************************************/
void rvdec_deblock_mb_r9 (RV_DECODER_T * pDecoder, MB_MODE_T * mb_mode_ptr, uint8 ** pMBYUV, uint32 *mvd_ptr)
{
	uint8 * pV, *pH;
	uint8 *pVc[2], *pHc[2];
	int uPitch;
	uint32 uTQPa, uTQPl;

	/*for current MB*/
	uint32 cbp;		/* luma */
	uint32 cbpc[2];	/* chroma, U=0,V=1 */
	uint32 mbtype;
	uint32 mvd;
	uint32 s;				/* strength (index to clip_table) */
	uint32 mbclip;			/* clip_table[s][QP] */
	uint32 mbclip0;
	uint32 mbclip2;		/* for B frame ref diff case */
	uint32 bhfilter;		/* bit-packed boolean, filter H edge (1=yes) */
	uint32 bhfilterc[2];	/* bit-packed boolean, filter H edge (1=yes) */
	uint32 bvfilter;		/* bit-packed boolean, filter V edge (1=yes) */
	uint32 bvfilterc[2];	/* bit-packed boolean, filter V edge (1=yes) */
	uint32 hfilter;		/* bit-packed H edge filter (0=weak,1=strong) */
	uint32 hfilterchroma[2];	/* bit-packed H edge filter (0=weak,1=strong) */
	uint32 vfilter;		/* bit-packed V edge filter (0=weak,1=strong) */
	uint32 vfilterchroma[2];	/* bit-packed V edge filter (0=weak,1=strong) */
	
	/*for adjacent MB*/
	uint32 cbp_left;
	uint32 cbpc_left[2];
	uint32 cbp_above;
	uint32 cbpc_above[2];
	uint32 cbp_below;
	uint32 cbpc_below[2];
	uint32 mvd_below;
	uint32 mvd_above;
	uint32 mvd_left;
	uint32 mbtype_left;
	uint32 mbtype_above;
	uint32 mbtype_below;
	uint32 s_left;
	uint32 s_above;
	uint32 s_below;
	uint32 mbclip_left;
	uint32 mbclip_above;
	uint32 mbclip_below;
	uint32 bnzs_left;
	uint32 bnzs_above;

	uint32 bnzs;			/* bit-packed block boolean, use MB strength (1=yes, 0=use 0 strength) */
	BOOLEAN bOnTopEdge;
	BOOLEAN bOnLeftEdge;
	BOOLEAN bOnBottomEdge;
	BOOLEAN bAboveRefDiff = FALSE;
	BOOLEAN bLeftRefDiff = FALSE;
	BOOLEAN bBelowRefDiff = FALSE;

	/* for adjacent blocks, formed by combining this MB and adjacent MB bits */
	uint32 cbph_adj;
	uint32 cbpv_adj;

	uint8 beta2;	
	uint8 beta2chroma;

	uint32 uQP;

	uint32 uBlock;
	uint32 uBlockRow;
	/* edge bit masks for current block */
	uint32 uEdgeBit;		/* for left and upper edges */
	uint32 uLowEdgeBit;	/* for lower edge */
	uint32 uBlockClip;		/* clip level current block */
	uint32 uBlockClipBelow;
	uint32 uBlockClipAbove;
	uint32 uBlockClipLeft;
	uint32 uBlockClipHVal;	/* H clip level current block */
	uint32 uBlockClipVVal;	/* V clip level current block */
	uint32 uCIx;
	
	mbtype = mb_mode_ptr->uMbType;

	/*get mb postion information, and get it's neighbor MB's mb type and other information*/	
	if (pDecoder->uMbPosX > 0)//left boundary
	{
		bOnLeftEdge = FALSE;
		mbtype_left = (mb_mode_ptr-1)->uMbType;
		cbp_left = (mb_mode_ptr-1)->uCbpcoef;
		mvd_left = *(mvd_ptr-1);
	}else
	{
		bOnLeftEdge = TRUE;
		mbtype_left = mbtype;
		cbp_left = 0;
		mvd_left = 0;
	}

	if (pDecoder->uMbPosY > 0) //top boundary
	{
		bOnTopEdge = FALSE;
		mbtype_above = (mb_mode_ptr-pDecoder->uMbNumX)->uMbType;
		cbp_above = (mb_mode_ptr-pDecoder->uMbNumX)->uCbpcoef;
		mvd_above = *(mvd_ptr-pDecoder->uMbNumX);
	}else
	{
		bOnTopEdge = TRUE;
		mbtype_above = mbtype;
		cbp_above = 0;
		mvd_above = 0;
	}

	if (pDecoder->uMbPosY < pDecoder->uMbNumY-1)	//bottom boundary
	{
		bOnBottomEdge = FALSE;
		mbtype_below = (mb_mode_ptr + pDecoder->uMbNumX)->uMbType;
		cbp_below = (mb_mode_ptr + pDecoder->uMbNumX)->uCbpcoef;
		mvd_below = *(mvd_ptr + pDecoder->uMbNumX);
	}else
	{
		bOnBottomEdge = TRUE;
		mbtype_below = mbtype;
		cbp_below = 0;
		mvd_below = 0;
	}

	/*get MB strength*/
	s = getMBStrength(mbtype);
	s_left = getMBStrength(mbtype_left);
	s_above = getMBStrength(mbtype_above);
	s_below = getMBStrength(mbtype_below);
	
	/*compute the edge filtering flag for one MB*/
	cbp = mb_mode_ptr->uCbpcoef;
	cbpc[0] = (cbp >> 16) & 0xf;				//U
	cbpc[1] = (cbp >> 20) & 0xf;				//V
	cbpc_left[0] = (cbp_left >> 16) & 0xf;		
	cbpc_left[1] = (cbp_left >> 20) & 0xf;
	cbpc_above[0] = (cbp_above >> 16) & 0xf;
	cbpc_above[1] = (cbp_above >> 20) & 0xf;
	cbpc_below[0] = (cbp_below >> 16) & 0xf;
	cbpc_below[1] = (cbp_below >> 20) & 0xf;

	cbp &= 0xffff;						/* clear chroma bits */
	cbp_above &= uBottomBlocksMask;		/* only want bottom edge blocks */
	cbpc_above[0] &= uBottomBlocksMaskChroma;
	cbpc_above[1] &= uBottomBlocksMaskChroma;
	mvd_above &= uBottomBlocksMask;		/* only want bottom edge blocks */
	cbp_left &= uRightBlocksMask;		/* only want right edge blocks */
	cbpc_left[0] &= uRightBlocksMaskChroma;
	cbpc_left[1] &= uRightBlocksMaskChroma;
	mvd_left &= uRightBlocksMask;		/* only want right edge blocks */
	cbp_below &= uTopBlocksMask;		/* only want top edge blocks */
	cbpc_below[0] &= uTopBlocksMaskChroma;
	cbpc_below[1] &= uTopBlocksMaskChroma;
	mvd_below &= uTopBlocksMask;

	/*  adj V: 14 13 12 15L 10 9 8 11L 6 5 4 7L 2 1 0 3L */
	cbpv_adj = (cbp<<1) & (~uLeftBlocksMask);
	cbpv_adj |= (cbp_left>>3);

	/* OR in the cbp and mvd bits for the top edge blocks (0-3) from the MB below, */
	/* to bits 16-19 */
	//cbp: 3B 2B 1B 0B 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
	//mvd: same orgonization with cbp
	mvd = (pDecoder->uPicCodeType == INTRAPIC) ? 0 : *mvd_ptr;
	cbp |= (cbp_below << 16);
	mvd |= (mvd_below << 16);

	//cbph_adj: 3B 2B 1B 0B 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 15A 14A 13A 12A
	cbph_adj = (cbp<<4) & (~uTopBlocksMask);
	cbph_adj |= (cbp_above>>12);

	/* filter an edge if cbp for the block or the adjacent block is set, or */
	/* if mvd for the block is set */
	bhfilter = cbp | cbph_adj | mvd;
	bvfilter = cbp | cbpv_adj | mvd;

	/* Repeat for chroma, U */
	/*  adj V: 2 3L 0 1L */
	cbpv_adj = (cbpc[0]<<1) & (~uLeftBlocksMaskChroma);
	cbpv_adj |= (cbpc_left[0]>>1);
	/* OR in the cbp bits for the top edge blocks (0-1) from the MB below, */
	/* to bits 4-5 */
	cbpc[0] |= (cbpc_below[0] << 4);
	/*  adj H: 1 0 3A 2A */
	cbph_adj = (cbpc[0]<<2) & (~uTopBlocksMaskChroma);
	cbph_adj |= (cbpc_above[0]>>2);
	/* filter an edge if cbp for the block or the adjacent block is set */
	bhfilterc[0] = cbpc[0] | cbph_adj;
	bvfilterc[0] = cbpc[0] | cbpv_adj;
	
	/* Repeat for chroma, V */
	cbpv_adj = (cbpc[1]<<1) & (~uLeftBlocksMaskChroma);
	cbpv_adj |= (cbpc_left[1]>>1);
	cbpc[1] |= (cbpc_below[1] << 4);
	cbph_adj = (cbpc[1]<<2) & (~uTopBlocksMaskChroma);
	cbph_adj |= (cbpc_above[1]>>2);
	bhfilterc[1] = cbpc[1] | cbph_adj;
	bvfilterc[1] = cbpc[1] | cbpv_adj;


	/* but don't filter picture edges */
	if (bOnLeftEdge)
	{
		bvfilter &= (~uLeftBlocksMask);
		bvfilterc[0] &= (~uLeftBlocksMaskChroma);
		bvfilterc[1] &= (~uLeftBlocksMaskChroma);
	}
	if (bOnTopEdge)
	{
		bhfilter &= (~uTopBlocksMask);
		bhfilterc[0] &= (~uTopBlocksMaskChroma);
		bhfilterc[1] &= (~uTopBlocksMaskChroma);
	}
	if (bOnBottomEdge)
	{
		bhfilter &= (~(uBottomBlocksMask<<4));
		bhfilterc[0] &= (~(uBottomBlocksMaskChroma<<2));
		bhfilterc[1] &= (~(uBottomBlocksMaskChroma<<2));
	}

	bnzs = cbp | mvd;
	bnzs_left = cbp_left | mvd_left;
	bnzs_above = cbp_above | mvd_above;

	/* select filter (0=weak or 1=strong) for each edge, H and V */
	/* init to weak for all edges */
	hfilter = 0;
	hfilterchroma[0] = hfilterchroma[1] = 0;
	vfilter = 0;
	vfilterchroma[0] = vfilterchroma[1] = 0;

	/* change to strong on MB edges if this or adj MB strength is 2 */
	if ((s == 2) || (s_above == 2))
	{
		hfilter |= uTopBlocksMask;
		hfilterchroma[0] |= uTopBlocksMaskChroma;
		hfilterchroma[1] |= uTopBlocksMaskChroma;
	}
	if ((s == 2) || (s_left == 2))
	{
		vfilter |= uLeftBlocksMask;
		vfilterchroma[0] |= uLeftBlocksMaskChroma;
		vfilterchroma[1] |= uLeftBlocksMaskChroma;
	}
	if ((s == 2) || (s_below == 2))
	{
		/* force no filter of MB bottom edges, because only the weak H */
		/* filter is used on the bottom edge of the MB. The edge will */
		/* be filtered using the strong filter when the MB below is */
		/* filtered. */
		bhfilter &= ~(uBottomBlocksMask<<4);
		bhfilterc[0] &= ~(uBottomBlocksMaskChroma<<2);
		bhfilterc[1] &= ~(uBottomBlocksMaskChroma<<2);
	}

	if (pDecoder->uPicCodeType == TRUEBPIC)
	{
		/*if use different referennce frame*/
		if (deblockRefDiff(mbtype, mbtype_left))
		{
			bLeftRefDiff = TRUE;
			vfilter |= uLeftBlocksMask;
			vfilterchroma[0] |= uLeftBlocksMaskChroma;
			vfilterchroma[1] |= uLeftBlocksMaskChroma;

			bvfilter |= uLeftBlocksMask;
			bvfilterc[0] |= uLeftBlocksMaskChroma;
			bvfilterc[1] |= uLeftBlocksMaskChroma;
		}
		else
		{
			bLeftRefDiff = FALSE;
		}

		if (deblockRefDiff(mbtype, mbtype_above))
		{
			bAboveRefDiff = TRUE;
			
			hfilter |= uTopBlocksMask;
			hfilterchroma[0] |= uTopBlocksMaskChroma;
			hfilterchroma[1] |= uTopBlocksMaskChroma;

			bhfilter |= uTopBlocksMask;
			bhfilterc[0] |= uTopBlocksMaskChroma;
			bhfilterc[1] |= uTopBlocksMaskChroma;
		}
		else
		{
			bAboveRefDiff = FALSE;
		}

		if (deblockRefDiff(mbtype, mbtype_below))
		{
			bBelowRefDiff = TRUE;

			bhfilter &= ~(uBottomBlocksMask<<4);
			bhfilterc[0] &= ~(uBottomBlocksMaskChroma<<2);
			bhfilterc[1] &= ~(uBottomBlocksMaskChroma<<2);			
		}
		else
		{
			bBelowRefDiff = FALSE;			
		}

	}

	uQP = mb_mode_ptr->uQp;	
	mbclip0 = clip_table[0][uQP];
	mbclip = clip_table[s][uQP];
	mbclip_left = clip_table[s_left][uQP];
	mbclip_above = clip_table[s_above][uQP];
	mbclip_below = clip_table[s_below][uQP];	

	if ((pDecoder->uFrameWidth*pDecoder->uFrameHeight) <= (176*144))
		beta2 = beta_tab[uQP]*4;
	else
		beta2 = beta_tab[uQP]*3;
	beta2chroma = beta_tab[uQP]*3;
	
	/*filtering Y*/
	uPitch = pDecoder->uFrameExtendWidth;

	mbclip0 = clip_table[0][uQP];
	mbclip2 = clip_table[2][uQP];	/* for B frame ref diff case */
	
	if ((bhfilter | bvfilter) != 0)
	{
		/*block 0*/
		pV = pMBYUV[0];
		pH = pMBYUV[0] + uPitch*4;    //point to the bottom line of first block
		
		uBlock = 0;
		uEdgeBit = CBP4x4Mask[uBlock];
		uLowEdgeBit = CBP4x4Mask[uBlock+4];
		
		//check for no filter at all this block
		if ( (((bhfilter | bvfilter) & uEdgeBit) != 0) ||
			((bhfilter & uLowEdgeBit) != 0))
		{
			uBlockClip = (bnzs & uEdgeBit) == 0 ? mbclip0 : mbclip;				
			uTQPa = uQP;
			uTQPl = uQP;
			
			uBlockClipHVal = bAboveRefDiff ? mbclip2 : uBlockClip;
			uBlockClipVVal = bLeftRefDiff ? mbclip2 : uBlockClip;
			
			if ((bvfilter & uEdgeBit) != 0)
				uBlockClipLeft = (bnzs_left & CBP4x4Mask[uBlock+3]) == 0 ? mbclip0 : mbclip_left;
			
			if (((bhfilter & uLowEdgeBit) != 0) && ((hfilter & uLowEdgeBit) == 0))
			{
				/* weak filter H */
				uBlockClipBelow = (bnzs & uLowEdgeBit) == 0 ? mbclip0 : mbclip;	

				C_EdgeFilter4H (pH, uPitch, uBlock*4, uBlockClipBelow, uBlockClip,
					alpha_tab[uQP], beta_tab[uQP], beta2,
					FALSE,	/* not chroma */
					FALSE	/* not strong */
					);				
			}
			
			if (((bvfilter & uEdgeBit) != 0) && ((vfilter & uEdgeBit) == 0))
			{
				/* weak filter V */
				C_EdgeFilter4V(pV, uPitch, uBlock*4, uBlockClipVVal, uBlockClipLeft,
					alpha_tab[uQP], beta_tab[uTQPl], beta2,
					FALSE,	/* not chroma */
					FALSE	/* not strong */
					);
			}
			
			if (((bhfilter & uEdgeBit) != 0) && ((hfilter & uEdgeBit) != 0))
			{
				/* strong filter H */
				uBlockClipAbove = (bnzs_above & CBP4x4Mask[uBlock+12]) == 0 ? mbclip0 : mbclip_above;
							
				C_EdgeFilter4H(pH - uPitch*4, uPitch, uBlock*4, uBlockClipHVal,
					uBlockClipAbove, alpha_tab[uQP], beta_tab[uQP], beta2, 
					FALSE,	/* not chroma */
					TRUE	/* strong */
					);

			}
			
			if (((bvfilter & uEdgeBit) != 0) && ((vfilter & uEdgeBit) != 0))
			{
				/* strong filter V */
				C_EdgeFilter4V(pV, uPitch, 0, uBlockClipVVal,
					uBlockClipLeft, alpha_tab[uQP], beta_tab[uQP], beta2, 
					FALSE,	/* not chroma */
					TRUE	/* strong */
					);
			}
			
		}
		
		pH += 4;
		pV += 4;
		
		/*block 1,2,3*/
		/* 2. blocks 1 - 3, edges 1,2,3 (no strong vertical) */
		for (uBlock = 1; uBlock < 4; uBlock++)
		{
			uEdgeBit = CBP4x4Mask[uBlock];
			uLowEdgeBit = CBP4x4Mask[uBlock+4];
			
			/* check for no filter at all this block */
			if ( (((bhfilter | bvfilter) & uEdgeBit) != 0) ||
				((bhfilter & uLowEdgeBit) != 0))
			{
				uBlockClip = (bnzs & uEdgeBit) == 0 ? mbclip0 : mbclip;					
				
				uBlockClipHVal = bAboveRefDiff ? mbclip2 : uBlockClip;
				
				if (((bhfilter & uLowEdgeBit) != 0) && ((hfilter & uLowEdgeBit) == 0))
				{
					/* weak filter H */
					uBlockClipBelow = (bnzs & uLowEdgeBit) == 0 ? mbclip0 : mbclip;
					
					C_EdgeFilter4H(pH, uPitch, uBlock*4, uBlockClipBelow, uBlockClip,
						alpha_tab[uQP], beta_tab[uQP], beta2,
						FALSE,	/* not chroma */
						FALSE	/* not strong */
						);
				}
				if ((bvfilter & uEdgeBit) != 0)
				{
					/* weak filter V */
					uBlockClipLeft = (bnzs & CBP4x4Mask[uBlock-1]) == 0 ? mbclip0 : mbclip;		
										
					C_EdgeFilter4V(pV, uPitch, uBlock*4, uBlockClip, uBlockClipLeft,
						alpha_tab[uQP], beta_tab[uQP], beta2,
						FALSE,	/* not chroma */
						FALSE	/* not strong */
						);
				}
				
				if (((bhfilter & uEdgeBit) != 0) && ((hfilter & uEdgeBit) != 0))
				{
					/* strong filter H */
					uBlockClipAbove = (bnzs_above & CBP4x4Mask[uBlock+12]) == 0 ? mbclip0 : mbclip_above;					
					
					C_EdgeFilter4H(pH - uPitch*4, uPitch, uBlock*4, uBlockClipHVal,
						uBlockClipAbove, alpha_tab[uQP], beta_tab[uQP], beta2, 
						FALSE,	/* not chroma */
						TRUE	/* strong */
						);	
				}				
			}	/* filter this block */
			
			pH += 4;
			pV += 4;
			
		}	/* blocks 1-3 */
	
		pH += (uPitch*4 - 16);
		pV += (uPitch*4 - 16);
		for (uBlockRow=1; uBlockRow<4; uBlockRow++)
		{
			/* 3. blocks 4,8,12 edges 1,2,4 (no strong horizontal) */
			uBlock = uBlockRow*4;
			uEdgeBit = CBP4x4Mask[uBlock];
			uLowEdgeBit = CBP4x4Mask[uBlock+4];
			
			/* check for no filter at all this block */
			if ( ((bvfilter & uEdgeBit) != 0) ||
				((bhfilter & uLowEdgeBit) != 0))
			{
				uBlockClip = (bnzs & uEdgeBit) == 0 ? mbclip0 : mbclip;
				uTQPl = uQP;
				
				uBlockClipVVal = bLeftRefDiff ? mbclip2 : uBlockClip;
				
				if ((bvfilter & uEdgeBit) != 0)
					uBlockClipLeft = (bnzs_left & CBP4x4Mask[uBlock+3]) == 0 ? mbclip0 : mbclip_left;
				
				if ((bhfilter & uLowEdgeBit) != 0)
				{
					/*weak H*/
					uBlockClipBelow = (bnzs & uLowEdgeBit) == 0 ? mbclip0 : mbclip;
										
					C_EdgeFilter4H(pH, uPitch, uBlock, uBlockClipBelow, uBlockClip,
						alpha_tab[uQP], beta_tab[uQP], beta2,
						FALSE,	/* not chroma */
						FALSE	/* not strong */
						);
				}  
				
				if (((bvfilter & uEdgeBit) != 0) && ((vfilter & uEdgeBit) == 0))
				{
					/* weak filter V */					
					C_EdgeFilter4V(pV, uPitch, uBlock, uBlockClipVVal, uBlockClipLeft,
						alpha_tab[uQP], beta_tab[uTQPl], beta2,
						FALSE,	/* not chroma */
						FALSE	/* not strong */
						);
				}
				
				if (((bvfilter & uEdgeBit) != 0) && ((vfilter & uEdgeBit) != 0))
				{
					/* strong filter V */					
					C_EdgeFilter4V(pV, uPitch, uBlock, uBlockClipVVal,
						uBlockClipLeft, alpha_tab[uQP], beta_tab[uQP], beta2,
						FALSE,	/* not chroma */
						TRUE	/* strong */
						);
				}
			}	/* filter edges, block 4 */
			pH += 4;
			pV += 4;
			
			/* 4. blocks 5-7, 9-11, 13-15, edges 1,2 (no strong-filtered) */
			for (uBlock = uBlockRow*4+1; uBlock < uBlockRow*4+4; uBlock++)
			{
				uEdgeBit = CBP4x4Mask[uBlock];
				uLowEdgeBit = CBP4x4Mask[uBlock+4];
				
				/* check for no filter at all this block */
				if ( ((bvfilter & uEdgeBit) != 0) ||
					((bhfilter & uLowEdgeBit) != 0))
				{
					uBlockClip = (bnzs & uEdgeBit) == 0 ? mbclip0 : mbclip;
					
					if ((bhfilter & uLowEdgeBit) != 0)
					{
						/* weak filter H */
						uTQPa = uQP;
						if (uBlock < 12)
							uBlockClipBelow = (bnzs & uLowEdgeBit) == 0 ? mbclip0 : mbclip;
						else
							
							uBlockClipBelow = (bnzs & uLowEdgeBit) == 0 ? mbclip0 : mbclip_below;					
						
						C_EdgeFilter4H(pH, uPitch, uBlock, uBlockClipBelow, uBlockClip,
							alpha_tab[uQP], beta_tab[uTQPa], beta2,
							FALSE,	/* not chroma */
							FALSE	/* not strong */
							);
					}
					if ((bvfilter & uEdgeBit) != 0)
					{
						/* weak filter V */
						uBlockClipLeft = (bnzs & CBP4x4Mask[uBlock-1]) == 0 ? mbclip0 : mbclip;	
												
						C_EdgeFilter4V(pV, uPitch, uBlock, uBlockClip, uBlockClipLeft,
							alpha_tab[uQP], beta_tab[uQP], beta2,
							FALSE,	/* not chroma */
							FALSE	/* not strong */
							);
					}
				}	/* filter edges, block 4 */
				pH += 4;
				pV += 4;
			}	/* 4. blocks 5-7, 9-11, 13-15 */
			pH += (uPitch*4 - 16);
			pV += (uPitch*4 - 16);
		}	/* for uBlockRow */
	}	/* bhfilter | bvfilter, luma */	
	
	
	/*filtering UV*/
	uPitch = uPitch >> 1;
	
	/* if bhfilter and bvfilter are zero, nothing to do this MB for chroma */
	if ((bhfilterc[0] | bvfilterc[0] | bhfilterc[1] | bvfilterc[1]) != 0)
	{
		pVc[0] = pMBYUV[1];				/* top of block 0 left V edge */
		pHc[0] = pMBYUV[1] + uPitch*4;	/* left pixel, block 0 lower H edge */
		pVc[1] = pMBYUV[2];				/* top of block 0 left V edge */
		pHc[1] = pMBYUV[2] + uPitch*4;	/* left pixel, block 0 lower H edge */
		
		/* 5. chroma blocks 0 edges 1-4 */
		uBlock = 0;
		uEdgeBit = CBP4x4Mask[uBlock];
		uLowEdgeBit = CBP4x4Mask[uBlock+2];
		
		/* first U then V */
		for (uCIx=0; uCIx<2; uCIx++)
		{
			/* check for no filter at all this block */
			if ( (((bhfilterc[uCIx] | bvfilterc[uCIx]) & uEdgeBit) != 0) ||
				((bhfilterc[uCIx] & uLowEdgeBit) != 0))
			{
				uBlockClip = (cbpc[uCIx] & uEdgeBit) == 0 ? mbclip0 : mbclip;
				uTQPl = uQP;  

				uBlockClipHVal = bAboveRefDiff ? mbclip2 : uBlockClip;
				uBlockClipVVal = bLeftRefDiff ? mbclip2 : uBlockClip;

				if ((bvfilterc[uCIx] & uEdgeBit) != 0)
					uBlockClipLeft = (cbpc_left[uCIx] & CBP4x4Mask[1]) == 0 ? mbclip0 : mbclip_left;
				
				if (((bhfilterc[uCIx] & uLowEdgeBit) != 0) && ((hfilterchroma[uCIx] & uLowEdgeBit) == 0))
				{
					/* weak filter H */
					uBlockClipBelow = (cbpc[uCIx] & uLowEdgeBit) == 0 ? mbclip0 : mbclip;
					
					C_EdgeFilter4H(pHc[uCIx], uPitch, 0, uBlockClipBelow, uBlockClip,
						alpha_tab[uQP], beta_tab[uQP], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
				if (((bvfilterc[uCIx] & uEdgeBit) != 0) && ((vfilterchroma[uCIx] & uEdgeBit) == 0))
				{
					/* weak filter V */
					C_EdgeFilter4V(pVc[uCIx], uPitch, 0, uBlockClipVVal, uBlockClipLeft,
						alpha_tab[uQP], beta_tab[uTQPl], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
				
				if (((bhfilterc[uCIx] & uEdgeBit) != 0) && ((hfilterchroma[uCIx] & uEdgeBit) != 0))
				{
					/* strong filter H */
					uBlockClipAbove = (cbpc_above[uCIx] & CBP4x4Mask[2]) == 0 ? mbclip0 : mbclip_above;
					
					C_EdgeFilter4H(pHc[uCIx] - uPitch*4, uPitch, 0, uBlockClipHVal,
						uBlockClipAbove, alpha_tab[uQP], beta_tab[uQP], beta2chroma, 
						TRUE,   /* chroma */
						TRUE	/* strong */
						);
				}
				if (((bvfilterc[uCIx] & uEdgeBit) != 0) && ((vfilterchroma[uCIx] & uEdgeBit) != 0))
				{
					/* strong filter V */
					C_EdgeFilter4V(pVc[uCIx], uPitch, 0, uBlockClipVVal,
						uBlockClipLeft, alpha_tab[uQP], beta_tab[uQP], beta2chroma, 
						TRUE,	/* chroma */
						TRUE	/* strong */
						);
				}
			}	/* filter edges, chroma block 0 */
			
			pHc[uCIx] += 4;
			pVc[uCIx] += 4;
		}	/* block 0 for uCIx */
		
		/* 6. chroma blocks 1 edges 1,2,3 (no strong vertical) */
		uBlock = 1;
		uEdgeBit = CBP4x4Mask[uBlock];
		uLowEdgeBit = CBP4x4Mask[uBlock+2];
		
		/* first U then V */
		for (uCIx=0; uCIx<2; uCIx++)
		{
			/* check for no filter at all this block */
			if ( (((bhfilterc[uCIx] | bvfilterc[uCIx]) & uEdgeBit) != 0) ||
				((bhfilterc[uCIx] & uLowEdgeBit) != 0))
			{
				uBlockClip = (cbpc[uCIx] & uEdgeBit) == 0 ? mbclip0 : mbclip;					

				uBlockClipHVal = bAboveRefDiff ? mbclip2 : uBlockClip;
				
				if (((bhfilterc[uCIx] & uLowEdgeBit) != 0) &&
					((hfilterchroma[uCIx] & uLowEdgeBit) == 0))
				{
					/* weak filter H */
					uBlockClipBelow = (cbpc[uCIx] & uLowEdgeBit) == 0 ? mbclip0 : mbclip;
										
					C_EdgeFilter4H(pHc[uCIx], uPitch, 8, uBlockClipBelow, uBlockClip,
						alpha_tab[uQP], beta_tab[uQP], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
				if ((bvfilterc[uCIx] & uEdgeBit) != 0)
				{
					/* weak filter V */
					uBlockClipLeft = (cbpc[uCIx] & CBP4x4Mask[uBlock-1]) == 0 ? mbclip0 : mbclip;	
				
					C_EdgeFilter4V(pVc[uCIx], uPitch, 0, uBlockClip, uBlockClipLeft,
						alpha_tab[uQP], beta_tab[uQP], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
				
				if (((bhfilterc[uCIx] & uEdgeBit) != 0) && ((hfilterchroma[uCIx] & uEdgeBit) != 0))
				{
					/* strong filter H */
					uBlockClipAbove = (cbpc_above[uCIx] & CBP4x4Mask[3]) == 0 ? mbclip0 : mbclip_above;
					
					C_EdgeFilter4H(pHc[uCIx] - uPitch*4, uPitch, 8, uBlockClipHVal,
						uBlockClipAbove, alpha_tab[uQP], beta_tab[uQP], beta2chroma, 
						TRUE,	/* chroma */
						TRUE	/* strong */
						);
				}
				
			}	/* filter this block */
			
			pHc[uCIx] += (uPitch*4 - 4);
			pVc[uCIx] += (uPitch*4 - 4);
		}	/* block 1 for uCIx */
		
		/* 7. chroma blocks 2, edges 1,2,4 (no strong horizontal) */
		uBlock = 2;
		uEdgeBit = CBP4x4Mask[uBlock];
		uLowEdgeBit = CBP4x4Mask[uBlock+2];
		
		/* first U then V */
		for (uCIx=0; uCIx<2; uCIx++)
		{
			/* check for no filter at all this block */
			if ( ((bvfilterc[uCIx] & uEdgeBit) != 0) ||
				((bhfilterc[uCIx] & uLowEdgeBit) != 0))
			{
				uBlockClip = (cbpc[uCIx] & uEdgeBit) == 0 ? mbclip0 : mbclip;
				uTQPl = uQP;

				uBlockClipVVal = bLeftRefDiff ? mbclip2 : uBlockClip;

				if ((bvfilterc[uCIx] & uEdgeBit) != 0)
					uBlockClipLeft = (cbpc_left[uCIx] & CBP4x4Mask[3]) == 0 ? mbclip0 : mbclip_left;
				
				if ((bhfilterc[uCIx] & uLowEdgeBit) != 0)
				{
					/* weak filter H */
					uTQPa = uQP;

					uBlockClipBelow = (cbpc[uCIx] & uLowEdgeBit) == 0 ? mbclip0 : mbclip;				
					
					C_EdgeFilter4H(pHc[uCIx], uPitch, 0, uBlockClipBelow, uBlockClip,
						alpha_tab[uQP], beta_tab[uTQPa], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
				if (((bvfilterc[uCIx] & uEdgeBit) != 0) &&
					((vfilterchroma[uCIx] & uEdgeBit) == 0))
				{
					/* weak filter V */

					C_EdgeFilter4V(pVc[uCIx], uPitch, 8, uBlockClipVVal, uBlockClipLeft,
						alpha_tab[uQP], beta_tab[uTQPl], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
				if (((bvfilterc[uCIx] & uEdgeBit) != 0) &&
					((vfilterchroma[uCIx] & uEdgeBit) != 0))
				{
					/* strong filter V */
					C_EdgeFilter4V(pVc[uCIx], uPitch, 8, uBlockClipVVal,
						uBlockClipLeft, alpha_tab[uQP], beta_tab[uQP], beta2chroma, 
						TRUE,	/* chroma */
						TRUE	/* strong */
						);

				}
			}	/* filter edges, chroma block 2 */
			
			pHc[uCIx] += 4;
			pVc[uCIx] += 4;
		}	/* block 2 for uCIx */
		
		/* 8. chroma blocks 3, edges 1,2 (no strong-filtered) */
		uBlock = 3;
		uEdgeBit = CBP4x4Mask[uBlock];
		uLowEdgeBit = CBP4x4Mask[uBlock+2];
		
		/* first U then V */
		for (uCIx=0; uCIx<2; uCIx++)
		{
			/* check for no filter at all this block */
			if ( ((bvfilterc[uCIx] & uEdgeBit) != 0) ||
				((bhfilterc[uCIx] & uLowEdgeBit) != 0))
			{
				uBlockClip = (cbpc[uCIx] & uEdgeBit) == 0 ? mbclip0 : mbclip;
				
				if ((bhfilterc[uCIx] & uLowEdgeBit) != 0)
				{
					/* weak filter H */
					uTQPa = uQP;

					uBlockClipBelow = (cbpc[uCIx] & uLowEdgeBit) == 0 ? mbclip0 : mbclip_below;				
					
					C_EdgeFilter4H(pHc[uCIx], uPitch, 8, uBlockClipBelow, uBlockClip,
						alpha_tab[uQP], beta_tab[uTQPa], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
				if ((bvfilterc[uCIx] & uEdgeBit) != 0)
				{
					/* weak filter V */
					uBlockClipLeft = (cbpc[uCIx] & CBP4x4Mask[uBlock-1]) == 0 ? mbclip0 : mbclip;
					
					C_EdgeFilter4V(pVc[uCIx], uPitch, 8, uBlockClip, uBlockClipLeft,
						alpha_tab[uQP], beta_tab[uQP], beta2chroma,
						TRUE,	/* chroma */
						FALSE	/* not strong */
						);
				}
			}	/* filter edges, block 4 */
		}	/* block 3 for uCIx */
		
	}	/* chroma blocks */		
}

/***********************************************************************
rv9 deblock
description: deblocking one frame after the frame has been decoded 

************************************************************************/
void rvdec_deblocking_frame (RV_DECODER_T * pDecoder)
{
	int mb_x, mb_y;	
	uint8 *pGobPix[3];
	uint8 *pMBYUV[3];
	uint32 *mvd_ptr = pDecoder->mvd_ptr;
	MB_MODE_T * mb_mode_ptr;
	
	if ((pDecoder->uPQuant <= 10) && pDecoder->bIsRV8)
		return;

	if (pDecoder->uPicCodeType == TRUEBPIC)
		mb_mode_ptr = pDecoder->mb_mode_b_ptr;
	else
		mb_mode_ptr = pDecoder->mb_mode_ptr;
		
	pGobPix [0] = pDecoder->pCurRecFrame->pDecFrame->imgYUV[0] + pDecoder->uStartInFrameY;
	pGobPix [1] = pDecoder->pCurRecFrame->pDecFrame->imgYUV[1] + pDecoder->uStartInFrameUV;
	pGobPix [2] = pDecoder->pCurRecFrame->pDecFrame->imgYUV[2] + pDecoder->uStartInFrameUV;

	for (mb_y = 0; mb_y < pDecoder->uMbNumY; mb_y++)
	{
		pDecoder->uMbPosY = mb_y;
		pMBYUV [0] = pGobPix [0];
		pMBYUV [1] = pGobPix [1];
		pMBYUV [2] = pGobPix [2];

		for (mb_x = 0; mb_x < pDecoder->uMbNumX; mb_x++)
		{
		//	if ((mb_y == 11) && (mb_x == 4) && (g_nFrame_rv == 3))
		//		PRINTF ("");

			pDecoder->uMbPosX = mb_x;
			
			if (!pDecoder->bIsRV8)
				rvdec_deblock_mb_r9 (pDecoder, mb_mode_ptr, pMBYUV, mvd_ptr);
			else
				rvdec_deblock_mb_r8 (pDecoder, mb_mode_ptr, pMBYUV);

			mb_mode_ptr++;
			mvd_ptr++;
			pMBYUV [0] += MB_SIZE;
			pMBYUV [1] += BLOCK_SIZE;
			pMBYUV [2] += BLOCK_SIZE;			
		}

		pGobPix [0] += pDecoder->uFrameExtendWidth * MB_SIZE;
		pGobPix [1] += (pDecoder->uFrameExtendWidth>>1) * BLOCK_SIZE;
		pGobPix [2] += (pDecoder->uFrameExtendWidth>>1) * BLOCK_SIZE;
	}
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
		return the whether deblocking flag determined by MV	
******************************************************************/
int32 set_deblock_coeff_r9 (RV_DECODER_T *pDecoder, MB_MODE_T *mb_mode_ptr, MOTION_VECTOR_T *mv_ptr, int32 mbtype)
{
	int32 i, b;
	int32 dx, dy;
	int32 index;
	int32 mvdelta = 0;
	MOTION_VECTOR_T *mv, *adj_mv;
	const int32 *pLeftBlockList;
	const int32 *pAboveBlockList;
	MB_CACHE_T *mb_cache_ptr = pDecoder->mb_cache_ptr;
	
	if ((mbtype == MBTYPE_INTER_4V) || (mbtype == MBTYPE_INTER_16x8V) || (mbtype == MBTYPE_INTER_8x16V))
	{
		/*if not 16x16, update mb and 8x8 block edge's filtering flag*/
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
		/*if be 16x16 type, only update the MB edge's flitering flag*/
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
		index = (b>>3)*2 + ((b&3)>>1);
		mv = mv_ptr + index;
		adj_mv = mv - ((index&1) ? 1 : 3);
		
		dx = mv->x - adj_mv->x;
		dy = mv->y - adj_mv->y;
		if (dx > 3 || dx < -3 || dy > 3 || dy < -3)
		{   
			mvdelta |= (1<<b);
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
			mvdelta |= (1<<b);
		}
		
		i++;
		b = pAboveBlockList [i];
	}

	return mvdelta;
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

