/***************************************************************************
*rvdec_intp_r9.c*
*only provide interpolation function for block 8x8, 
if block 16x16, it will be seperated into four block8x8 *
***************************************************************************/
#ifdef WIN32
#include <memory.h>
#include "rvdec_global.h"
#endif
#include "rvdec_basic.h"
//#include "rvdec_mode.h"
//#include "rvdec_global.h"
#include "rvdec_mc.h"


// uint8 g_int_bfr [16*16];//the memory pitch is 16

/*six coeff*/
#if 0
int8 g_interpCoeff [8][6] = 
{
	{0,   0,  0,  0,   0, 0}, //reserved
	{1,  -5, 116, 20,  -5, 1}, //r1
	{2, -10, 104, 40, -10, 2}, //q1
	{3, -15,  92, 60, -15, 3}, //r2
	{4, -20,  80, 80, -20, 4}, //h
	{3, -15,  60, 92, -15, 3}, //r3
	{2, -10,  40,104, -10, 2}, //q2
	{1,  -5,  20,116,  -5, 1}, //r4
};

int g_shift_bits_tab[8] = {0, 7, 7, 7, 7, 7, 7, 7};

#else
/*
//too sharpness
int8 g_interpCoeff [8][6] = 
{
	{0,  0,  0,     0,    0, 0}, //reserved
	{1, -5, 116,   20,   -5, 1}, //r1     128
	{1, -5,  52,   20,   -5, 1}, //q1		64 
	{1, -5,  23,   17,   -5, 1}, //r2		32
	{1, -5,  20,   20,   -5, 1}, //h		32
	{1, -5,  17,   23,  -15, 3}, //r3		32
	{1, -5,  20,   52,   -5, 1}, //q2		64
	{1, -5,  20,  116,   -5, 1}, //r4		128
};
int g_shift_bits_tab[8] = {0, 7, 6, 5, 5, 5, 6, 7};
*/
/*
int8 g_interpCoeff [8][6] = 
{
	{0,   0,   0,  0,   0, 0}, //reserved
	{1,  -5, 116, 20,  -5, 1}, //r1     128
	{1,  -5,  52, 20,  -5, 1}, //q1		64 
	{3, -15,  92, 60, -15, 3}, //r2		128
	{1,  -5,  20, 20,  -5, 1}, //h		32
	{3, -15,  60, 92, -15, 3}, //r3		128
	{1,  -5,  20, 52,  -5, 1}, //q2		64
	{1,  -5,  20,116,  -5, 1}, //r4		128
};
int g_shift_bits_tab[8] = {0, 7, 6, 7, 5, 7, 6, 7};
*/

/*
//smooth
int8 g_interpCoeff [8][6] = 
{
	{0,   0,   0,  0,  0, 0}, //reserved
	{1,  -5, 116, 20, -5, 1}, //r1     128
	{1, -5,   52, 20, -5, 1}, //q1		64 
	{1, -5,   42, 30, -5, 1}, //r2		64
	{1, -5,   20, 20, -5, 1}, //h		32
	{1, -5,   30, 42, -5, 1}, //r3		64
	{1, -5,   20, 52, -5, 1}, //q2		64
	{1, -5,   20,116, -5, 1}, //r4		128
};
int g_shift_bits_tab[8] = {0, 7, 6, 6, 5, 6, 6, 7};
*/

uint32 g_intp_coeff[8][4] = 
{
	//coff2, coff3, rnd, shift_bits
	{  0,  0,  0, 0},
	{116, 20, 64, 7},
	{52,  20, 32, 6},
	{42,  30, 32, 6},
	{20,  20, 16, 5},
	{30,  42, 32, 6},
	{20,  52, 32, 6},
	{20, 116, 64, 7}
};


/*
//too smooth
int8 g_interpCoeff [8][6] = 
{
	{0,  0,  0,  0,  0, 0}, //reserved
	{1, -5, 61, 11, -5, 1}, //r1		64
	{1, -5, 52, 20, -5, 1}, //q1		64 
	{1, -5, 42, 30, -5, 1}, //r2		64
	{1, -5, 36, 36, -5, 1}, //h			64
	{1, -5, 30, 42, -5, 1}, //r3		64
	{1, -5, 20, 52, -5, 1}, //q2		64
	{1, -5, 11, 61, -5, 1}, //r4		64
};
int g_shift_bits_tab[8] = {0, 6, 6, 6, 6, 6, 6, 6};
*/
#endif

/********************************************************************************
//				interpolation function for luma block 4x4
********************************************************************************/
#if !defined(_ARM_MC_ASSEMBLY_)
void MC_luma4x4_x0y0_d (uint8 * pRefBlk, uint8 * pPred, int32 width)
{
	int32 i;
	uint8 * pPredBlk;
	uint8 p0, p1, p2, p3;
	
	pPredBlk = pPred;    
	
	for (i = 4; i > 0; i--)
	{
		p0 = pRefBlk [0];
		p1 = pRefBlk [1];
		p2 = pRefBlk [2];
		p3 = pRefBlk [3];
		
		pPredBlk [0] = p0;
		pPredBlk [1] = p1;
		pPredBlk [2] = p2;
		pPredBlk [3] = p3;
		
		pPredBlk += MB_SIZE/2;
		pRefBlk += width;
	}
}

void MC_luma4x4_yfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int dx, int line)
{
	int32 i;
	uint8 * pPredBlk;
	uint8 * pClipTab = g_rgiRvClipTab;
	int32 p0, p1, p2, p3, p4, p5, p6, p7, p8;
	uint32 *pCoeff = g_intp_coeff [dx];
	int8 c0, c1, c2, c3, c4, c5;

	int shift_bits;
	int rnd;

	c0 = 1;	
	c1 = -5;
	c2 = pCoeff[0];
	c3 = pCoeff[1];	
	c4 = -5;
	c5 = 1;

	rnd = pCoeff[2];
	shift_bits = pCoeff[3];

	pPredBlk = pPred;   

	for (i = line; i > 0; i--)
	{
		p0 = pRefBlk [-2];
		p1 = pRefBlk [-1];
		p2 = pRefBlk [0];
		p3 = pRefBlk [1];
		p4 = pRefBlk [2];
		p5 = pRefBlk [3];
		
		pPred[0] = pClipTab [(c0*p0 + c1*p1 + c2*p2 + c3*p3 + c4*p4 + c5*p5 + rnd) >>shift_bits];
		
		p6 = pRefBlk [4];
		pPred[1] = pClipTab [(c0*p1 + c1*p2 + c2*p3 + c3*p4 + c4*p5 + c5*p6 + rnd) >>shift_bits];

		p7 = pRefBlk [5];
		pPred[2] = pClipTab [(c0*p2 + c1*p3 + c2*p4 + c3*p5 + c4*p6 + c5*p7 + rnd) >>shift_bits];

		p8 = pRefBlk [6];
		pPred[3] = pClipTab [(c0*p3 + c1*p4 + c2*p5 + c3*p6 + c4*p7 + c5*p8 + rnd) >>shift_bits];
		
		pPred += MB_SIZE/2;
		pRefBlk += width;
	}
}

void MC_luma4x4_xfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int dy)
{
	int32 i;
	uint8 * pPredBlk;
	uint8 * pClipTab = g_rgiRvClipTab;
	int32 p0, p1, p2, p3, p4, p5, p6, p7, p8;
	uint32 *pCoeff = g_intp_coeff [dy];
	int8 c0, c1, c2, c3, c4, c5;
	int shift_bits;
	int rnd;

	c0 = 1;	
	c1 = -5;
	c2 = pCoeff[0];
	c3 = pCoeff[1];	
	c4 = -5;
	c5 = 1;

	rnd = pCoeff[2];
	shift_bits = pCoeff[3];

	pPredBlk = pPred;   
	
	for (i = 0; i < 4; i++)
	{
		p0 = pRefBlk [-2*width];
		p1 = pRefBlk [-1*width];
		p2 = pRefBlk [0];
		p3 = pRefBlk [1*width];
		p4 = pRefBlk [2*width];
		p5 = pRefBlk [3*width];

		pPredBlk[0] = pClipTab [(c0*p0 + c1*p1 + c2*p2 + c3*p3 + c4*p4 + c5*p5 + rnd) >>shift_bits];
		
		p6 = pRefBlk [4*width];
		pPredBlk[MB_SIZE/2] = pClipTab [(c0*p1 + c1*p2 + c2*p3 + c3*p4 + c4*p5 + c5*p6 + rnd) >>shift_bits];

		p7 = pRefBlk [5*width];
		pPredBlk[2*(MB_SIZE/2)] = pClipTab [(c0*p2 + c1*p3 + c2*p4 + c3*p5 + c4*p6 + c5*p7 + rnd) >>shift_bits];

		p8 = pRefBlk [6*width];
		pPredBlk[3*(MB_SIZE/2)] = pClipTab [(c0*p3 + c1*p4 + c2*p5 + c3*p6 + c4*p7 + c5*p8 + rnd) >>shift_bits];

		pPredBlk += 1;
		pRefBlk += 1;
	}
}
#endif

#if !defined(_ARM_MC_ASSEMBLY_)
#define MC_LUMA4X4_X0Y0_D	MC_luma4x4_x0y0_d
#define MC_LUMA4X4_XFULL_D MC_luma4x4_xfull_d
#define MC_LUMA4X4_YFULL_D MC_luma4x4_yfull_d
#else
#define MC_LUMA4X4_X0Y0_D arm_mc_chroma4x4_dx0dy0_rv9
#define MC_LUMA4X4_XFULL_D arm_mc_luma4x4_xfull_d
#define MC_LUMA4X4_YFULL_D arm_mc_luma4x4_yfull_d
#endif

void MC_luma4x4_xmyn (uint8 * pRefFrame, uint8 * pPred, int32 xpos, int32 ypos, int width)
{
	int32	dx = xpos & 0x7;
	int32	dy = ypos & 0x7;	
	uint8	blk_tmp[9*8];
	uint8 * pRefBlk;
	uint32	pitch_width;

	pRefBlk = pRefFrame + (ypos >> 3) * width + (xpos >> 3);

	if ((dx == 0) && (dy == 0))
	{
		MC_LUMA4X4_X0Y0_D (pRefBlk, pPred, width);
	}else if (dx == 0)
	{
		MC_LUMA4X4_XFULL_D (pRefBlk, pPred, width, dy);
	}else if (dy == 0)
	{
		MC_LUMA4X4_YFULL_D (pRefBlk, pPred, width, dx, 4);
	}else
	{
		pRefBlk -= (2*width);
		MC_LUMA4X4_YFULL_D    (pRefBlk, blk_tmp, width, dx, 9);

		pitch_width = 8;
		pRefBlk = blk_tmp + pitch_width*2;
		MC_LUMA4X4_XFULL_D    (pRefBlk, pPred, pitch_width, dy);
	}	
}

/********************************************************************************
//				interpolation function for luma block 8x8
********************************************************************************/
#if !defined(_ARM_MC_ASSEMBLY_)
void MC_luma8x8_x0y0_d (uint8 * pRefBlk, uint8 * pPred, int32 width)
{
	int32 i;
	uint8 * pPredBlk;
	uint8 p0, p1, p2, p3;
	
	pPredBlk = pPred;    
	
	for (i = 8; i > 0; i--)
	{
		p0 = pRefBlk [0];
		p1 = pRefBlk [1];
		p2 = pRefBlk [2];
		p3 = pRefBlk [3];
		
		pPredBlk [0] = p0;
		pPredBlk [1] = p1;
		pPredBlk [2] = p2;
		pPredBlk [3] = p3;

		/*right 4 pixels*/
		p0 = pRefBlk [4];
		p1 = pRefBlk [5];
		p2 = pRefBlk [6];
		p3 = pRefBlk [7];
		
		pPredBlk [4] = p0;
		pPredBlk [5] = p1;
		pPredBlk [6] = p2;
		pPredBlk [7] = p3;
		
		pPredBlk += MB_SIZE/2;
		pRefBlk += width;
	}
}

void MC_luma8x8_yfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int dx, int line)
{
	int32 i;
	uint8 * pPredBlk;
	int8 c0, c1, c2, c3, c4, c5;
	uint32 *pCoeff = g_intp_coeff [dx];
	uint8 * pClipTab = g_rgiRvClipTab;
	int32 p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12;

	int shift_bits;
	int rnd;

	c0 = 1;	
	c1 = -5;
	c2 = pCoeff[0];
	c3 = pCoeff[1];	
	c4 = -5;
	c5 = 1;

	rnd = pCoeff[2];
	shift_bits = pCoeff[3];

	pPredBlk = pPred;   

	for (i = line; i > 0; i--)
	{
		p0 = pRefBlk [-2];
		p1 = pRefBlk [-1];
		p2 = pRefBlk [0];
		p3 = pRefBlk [1];
		p4 = pRefBlk [2];
		p5 = pRefBlk [3];
		
		pPred[0] = pClipTab [(c0*p0 + c1*p1 + c2*p2 + c3*p3 + c4*p4 + c5*p5 + rnd) >>shift_bits];
		
		p6 = pRefBlk [4];
		pPred[1] = pClipTab [(c0*p1 + c1*p2 + c2*p3 + c3*p4 + c4*p5 + c5*p6 + rnd) >>shift_bits];

		p7 = pRefBlk [5];
		pPred[2] = pClipTab [(c0*p2 + c1*p3 + c2*p4 + c3*p5 + c4*p6 + c5*p7 + rnd) >>shift_bits];

		p8 = pRefBlk [6];
		pPred[3] = pClipTab [(c0*p3 + c1*p4 + c2*p5 + c3*p6 + c4*p7 + c5*p8 + rnd) >>shift_bits];

		/*right four pixels*/
		p9 = pRefBlk [7];
		pPred[4] = pClipTab [(c0*p4 + c1*p5 + c2*p6 + c3*p7 + c4*p8 + c5*p9 + rnd) >>shift_bits];
		
		p10 = pRefBlk [8];
		pPred[5] = pClipTab [(c0*p5 + c1*p6 + c2*p7 + c3*p8 + c4*p9 + c5*p10 + rnd) >>shift_bits];

		p11 = pRefBlk [9];
		pPred[6] = pClipTab [(c0*p6 + c1*p7 + c2*p8 + c3*p9 + c4*p10 + c5*p11 + rnd) >>shift_bits];

		p12 = pRefBlk [10];
		pPred[7] = pClipTab [(c0*p7 + c1*p8 + c2*p9 + c3*p10 + c4*p11 + c5*p12 + rnd) >>shift_bits];
		
		pPred += MB_SIZE/2;
		pRefBlk += width;
	}
}

void MC_luma8x8_xfull_d (uint8 * pRefBlk, uint8 * pPred, int32 width, int dy)
{
	int32 i;
	uint8 * pPredBlk;
	uint8 * pClipTab = g_rgiRvClipTab;
	int32 p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12;
	uint32 *pCoeff = g_intp_coeff [dy];
	int8 c0, c1, c2, c3, c4, c5;
	int shift_bits;
	int rnd;

	c0 = 1;	
	c1 = -5;
	c2 = pCoeff[0];
	c3 = pCoeff[1];	
	c4 = -5;
	c5 = 1;

	rnd = pCoeff[2];
	shift_bits = pCoeff[3];

	pPredBlk = pPred;   
	
	for (i = 0; i < 8; i++)
	{
		p0 = pRefBlk [-2*width];
		p1 = pRefBlk [-1*width];
		p2 = pRefBlk [0];
		p3 = pRefBlk [1*width];
		p4 = pRefBlk [2*width];
		p5 = pRefBlk [3*width];

		pPredBlk[0] = pClipTab [(c0*p0 + c1*p1 + c2*p2 + c3*p3 + c4*p4 + c5*p5 + rnd) >>shift_bits];
		
		p6 = pRefBlk [4*width];
		pPredBlk[MB_SIZE/2] = pClipTab [(c0*p1 + c1*p2 + c2*p3 + c3*p4 + c4*p5 + c5*p6 + rnd) >>shift_bits];

		p7 = pRefBlk [5*width];
		pPredBlk[2*(MB_SIZE/2)] = pClipTab [(c0*p2 + c1*p3 + c2*p4 + c3*p5 + c4*p6 + c5*p7 + rnd) >>shift_bits];

		p8 = pRefBlk [6*width];
		pPredBlk[3*(MB_SIZE/2)] = pClipTab [(c0*p3 + c1*p4 + c2*p5 + c3*p6 + c4*p7 + c5*p8 + rnd) >>shift_bits];

		/*bottom 4 pixels*/
		p9 = pRefBlk [7*width];

		pPredBlk[4*(MB_SIZE/2)] = pClipTab [(c0*p4 + c1*p5 + c2*p6 + c3*p7 + c4*p8 + c5*p9 + rnd) >>shift_bits];
		
		p10 = pRefBlk [8*width];
		pPredBlk[5*(MB_SIZE/2)] = pClipTab [(c0*p5 + c1*p6 + c2*p7 + c3*p8 + c4*p9 + c5*p10 + rnd) >>shift_bits];

		p11 = pRefBlk [9*width];
		pPredBlk[6*(MB_SIZE/2)] = pClipTab [(c0*p6 + c1*p7 + c2*p8 + c3*p9 + c4*p10 + c5*p11 + rnd) >>shift_bits];

		p12 = pRefBlk [10*width];
		pPredBlk[7*(MB_SIZE/2)] = pClipTab [(c0*p7 + c1*p8 + c2*p9 + c3*p10 + c4*p11 + c5*p12 + rnd) >>shift_bits];

		pPredBlk += 1;
		pRefBlk += 1;
	}
}
#endif

#if !defined(_ARM_MC_ASSEMBLY_)
#define MC_LUMA8X8_X0Y0_D	MC_luma8x8_x0y0_d
#define MC_LUMA8X8_XFULL_D MC_luma8x8_xfull_d
#define MC_LUMA8X8_YFULL_D MC_luma8x8_yfull_d
#else
#define MC_LUMA8X8_X0Y0_D arm_mc_chroma8x8_dx0dy0_rv9
#define MC_LUMA8X8_XFULL_D arm_mc_luma8x8_xfull_d
#define MC_LUMA8X8_YFULL_D arm_mc_luma8x8_yfull_d
#endif

void MC_luma8x8_xmyn (uint8 * pRefFrame, uint8 * pPred, int32 xpos, int32 ypos, int width)
{
	int32	dx = xpos & 0x7;
	int32	dy = ypos & 0x7;	
	uint8	blk_tmp[13*8];
	uint8 * pRefBlk;
	uint32	pitch_width;

	pRefBlk = pRefFrame + (ypos >> 3) * width + (xpos >> 3);

	if ((dx == 0) && (dy == 0))
	{
		MC_LUMA8X8_X0Y0_D (pRefBlk, pPred, width);
	}else if (dx == 0)
	{
		MC_LUMA8X8_XFULL_D (pRefBlk, pPred, width, dy);
	}else if (dy == 0)
	{
		MC_LUMA8X8_YFULL_D (pRefBlk, pPred, width, dx, 8);
	}else
	{
		pRefBlk -= (2*width);
		MC_LUMA8X8_YFULL_D    (pRefBlk, blk_tmp, width, dx, 13);

		pitch_width = 8;
		pRefBlk = blk_tmp + pitch_width*2;
		MC_LUMA8X8_XFULL_D     (pRefBlk, pPred, pitch_width, dy);
	}	
}

/********************************************************************************
//				interpolation function for chroma block 2x2
********************************************************************************/
#if !defined(_ARM_MC_ASSEMBLY_)
void MC_chroma2x2_xfull_d (uint8 * pRefBlk, uint8 * pPred, int width, int32 dy)
{
	uint8 p00, p01;
	uint8 p10, p11;
	uint8 p20, p21;

	int32 dy0, dy1;

	dy0			= dy;
	dy1			= 8 - dy;

	p00			= pRefBlk[0];
	p10			= pRefBlk[width];
	p20			= pRefBlk[width*2];

	pPred[0]	= (p00*dy1 + p10*dy0 + 4) / 8;	
	pPred[4]	= (p10*dy1 + p20*dy0 + 4) / 8;

	p01			= pRefBlk[1];
	p11			= pRefBlk[width + 1];
	p21			= pRefBlk[width*2 + 1];

	pPred[1]	= (p01*dy1 + p11*dy0 + 4) / 8;
	pPred[5]	= (p11*dy1 + p21*dy0 + 4) / 8;
}

void MC_chroma2x2_yfull_d (uint8 * pRefBlk, uint8 * pPred, int width, int dx)
{
	uint8 p00, p01, p02;
	uint8 p10, p11, p12;

	int32 dx0, dx1;

	dx0			= dx;
	dx1			= 8 - dx;

	p00			= pRefBlk[0];
	p01			= pRefBlk[1];
	p02			= pRefBlk[2];

	p10			= pRefBlk[width];
	p11			= pRefBlk[width + 1];
	p12			= pRefBlk[width + 2];

	pPred[0]	= (p00*dx1 + p01*dx0 + 4) / 8;
	pPred[1]	= (p01*dx1 + p02*dx0 + 4) / 8;
	
	pPred[4]	= (p10*dx1 + p11*dx0 + 4) / 8;
	pPred[5]	= (p11*dx1 + p12*dx0 + 4) / 8;
}


void MC_chroma2x2_d (uint8 * pRefBlk, uint8 * pPred, int width, int dx, int dy)
{
	uint8 p00, p01, p02;
	uint8 p10, p11, p12;
	uint8 p20, p21, p22;

	int32 d00, d01;
	int32 d10, d11;

	d00			= (8-dx)*(8-dy);
	d01			= dx*(8-dy);
	d10			= (8-dx)*dy;
	d11			= dx*dy;

	p00			= pRefBlk[0];
	p01			= pRefBlk[1];
	p02			= pRefBlk[2];

	p10			= pRefBlk[width];
	p11			= pRefBlk[width + 1];
	p12			= pRefBlk[width + 2];

	pPred[0]	= (p00*d00 + p01*d01 + p10*d10 + p11*d11 + 32) / 64;
	pPred[1]	= (p01*d00 + p02*d01 + p11*d10 + p12*d11 + 32) / 64;

	p20			= pRefBlk[width*2];
	p21			= pRefBlk[width*2 + 1];
	p22			= pRefBlk[width*2 + 2];
	pPred[4]	= (p10*d00 + p11*d01 + p20*d10 + p21*d11 + 32) / 64;
	pPred[5]	= (p11*d00 + p12*d01 + p21*d10 + p22*d11 + 32) / 64;
}
#endif

#if !defined(_ARM_MC_ASSEMBLY_)
#define MC_CHROMA2X2_XFULL_D MC_chroma2x2_xfull_d
#define MC_CHROMA2X2_YFULL_D MC_chroma2x2_yfull_d
#define MC_CHROMA2X2_D	MC_chroma2x2_d
#else
#define MC_CHROMA2X2_XFULL_D arm_mc_chroma2x2_xfull_d
#define MC_CHROMA2X2_YFULL_D arm_mc_chroma2x2_yfull_d
#define MC_CHROMA2X2_D	arm_mc_chroma2x2_d
#endif

void PC_MC_chroma2x2_rv (uint8 * pRefFrame, uint8 * pPredUV, int32 xpos, int32 ypos, int width)
{	
	int32 dx, dy;
	int32 offset;
	uint8 * pRefBlk;

	dx     = xpos & 0x7;
	dy	   = ypos & 0x7;
		
	xpos   = xpos >> 3;
	ypos   = ypos >> 3;
	offset = ypos * width + xpos;

	pRefBlk = pRefFrame + offset;

	if (dx == 0)
	{
		if (dy == 0)
		{
			//MC_chroma2x2_x0y0_d (pRefBlk, pPredUV, width);
			pPredUV[0]   = pRefBlk[0];
			pPredUV[1]   = pRefBlk[1];
			pPredUV[4+0] = pRefBlk[width];
			pPredUV[4+1] = pRefBlk[width+1];
		}else
		{
			MC_CHROMA2X2_XFULL_D (pRefBlk, pPredUV, width, dy);
		}
	}else
	{
		if (dy == 0)
		{
			MC_CHROMA2X2_YFULL_D (pRefBlk, pPredUV, width, dx);
		}else
		{
			MC_CHROMA2X2_D (pRefBlk, pPredUV, width, dx, dy);
		}
	}	 
}



/********************************************************************************
//				interpolation function for chroma block 2x2
********************************************************************************/
#if !defined(_ARM_MC_ASSEMBLY_)
void MC_chroma4x4_x0y0_d (uint8 * pRefBlk, uint8 * pPred, int width)
{
	if ((((uint32)pRefBlk) & 0x3) == 0)
	{
		((uint32 *)pPred)[0] = ((uint32 *)pRefBlk)[0];

		pRefBlk += width;
		((uint32 *)pPred)[1] = ((uint32 *)pRefBlk)[0];

		pRefBlk += width;
		((uint32 *)pPred)[2] = ((uint32 *)pRefBlk)[0];

		pRefBlk += width;
		((uint32 *)pPred)[3] = ((uint32 *)pRefBlk)[0];
	}
	else
	{		
		*pPred++ = pRefBlk[0];
		*pPred++ = pRefBlk[1];
		*pPred++ = pRefBlk[2];
		*pPred++ = pRefBlk[3];
		
		pRefBlk += width;
		*pPred++ = pRefBlk[0];
		*pPred++ = pRefBlk[1];
		*pPred++ = pRefBlk[2];
		*pPred++ = pRefBlk[3];
		
		pRefBlk += width;
		*pPred++ = pRefBlk[0];
		*pPred++ = pRefBlk[1];
		*pPred++ = pRefBlk[2];
		*pPred++ = pRefBlk[3];
		
		pRefBlk += width;
		*pPred++ = pRefBlk[0];
		*pPred++ = pRefBlk[1];
		*pPred++ = pRefBlk[2];
		*pPred++ = pRefBlk[3];
	}
}


void MC_chroma4x4_xfull_d (uint8 * pRefBlk, uint8 * pPred, int width, int32 dy)
{
	int		i;
	uint8	p0, p1, p2, p3, p4;
	int32	dy0, dy1;

	dy0			= dy;
	dy1			= 8 - dy;

	for (i = 4; i > 0; i--)
	{
		p0			= pRefBlk[0];
		p1			= pRefBlk[width];
		p2			= pRefBlk[width*2];
		p3			= pRefBlk[width*3];
		p4			= pRefBlk[width*4];
		
		pPred[0]	= (p0*dy1 + p1*dy0 + 4) / 8;	
		pPred[4]	= (p1*dy1 + p2*dy0 + 4) / 8;
		pPred[8]	= (p2*dy1 + p3*dy0 + 4) / 8;
		pPred[12]	= (p3*dy1 + p4*dy0 + 4) / 8;

		pPred++;
		pRefBlk++;
	}
}


void MC_chroma4x4_yfull_d (uint8 * pRefBlk, uint8 * pPred, int width, int32 dx)
{
	int		i;
	uint8	p0, p1, p2, p3, p4;
	int32	dx0, dx1;

	dx0		= dx;
	dx1		= 8 - dx;

	for (i = 4; i > 0; i--)
	{
		p0		= pRefBlk[0];
		p1		= pRefBlk[1];
		p2		= pRefBlk[2];
		p3		= pRefBlk[3];
		p4		= pRefBlk[4];
		
		*pPred++	= (p0*dx1 + p1*dx0 + 4) / 8;	
		*pPred++	= (p1*dx1 + p2*dx0 + 4) / 8;
		*pPred++	= (p2*dx1 + p3*dx0 + 4) / 8;
		*pPred++	= (p3*dx1 + p4*dx0 + 4) / 8;

		pRefBlk += width;
	}
}

void MC_chroma4x4_d (uint8 * pRefBlk, uint8 * pPred, int width, int32 dx, int dy)
{
	int		i;
	int32	d00, d01;
	int32	d10, d11;
	uint8	p00, p01, p02, p03, p04;
	uint8	p10, p11, p12, p13, p14;

	d00		= (8-dx)*(8-dy);
	d01		= dx*(8-dy);
	d10		= (8-dx)*dy;
	d11		= dx*dy;

	p00 = pRefBlk[0];
	p01 = pRefBlk[1];
	p02 = pRefBlk[2];
	p03 = pRefBlk[3];
	p04 = pRefBlk[4];
	pRefBlk += width;

	for (i = 4; i > 0; i--)
	{
		p10 = pRefBlk[0];
		p11 = pRefBlk[1];
		p12 = pRefBlk[2];
		p13 = pRefBlk[3];
		p14 = pRefBlk[4];

		*pPred++	= (p00*d00 + p01*d01 + p10*d10 + p11*d11 + 32) / 64;
		*pPred++	= (p01*d00 + p02*d01 + p11*d10 + p12*d11 + 32) / 64;
		*pPred++	= (p02*d00 + p03*d01 + p12*d10 + p13*d11 + 32) / 64;
		*pPred++	= (p03*d00 + p04*d01 + p13*d10 + p14*d11 + 32) / 64;

		p00 = p10;
		p01 = p11;
		p02 = p12;
		p03 = p13;
		p04 = p14;		
		pRefBlk += width;
	}
}
#endif

#if !defined(_ARM_MC_ASSEMBLY_)
#define MC_CHROMA4X4_X0Y0_D	MC_chroma4x4_x0y0_d
#define MC_CHROMA4X4_XFULL_D MC_chroma4x4_xfull_d
#define MC_CHROMA4X4_YFULL_D MC_chroma4x4_yfull_d
#define MC_CHROMA4X4_D	MC_chroma4x4_d
#else
#define MC_CHROMA4X4_X0Y0_D	arm_mc_chroma4x4_dx0dy0_d
#define MC_CHROMA4X4_XFULL_D arm_mc_chroma4x4_xfull_d
#define MC_CHROMA4X4_YFULL_D arm_mc_chroma4x4_yfull_d
#define MC_CHROMA4X4_D	arm_mc_chroma4x4_d
#endif

PUBLIC void PC_MC_chroma4x4_rv(uint8 * pRefFrame, uint8 * pPredUV, int32 xpos, int32 ypos, int width)
{	
	int32 dx, dy;
	int32 offset;
	uint8 * pRefBlk;

	dx     = xpos & 0x7;
	dy	   = ypos & 0x7;
		
	xpos   = xpos >> 3;
	ypos   = ypos >> 3;
	offset = ypos * width + xpos;

	pRefBlk = pRefFrame + offset;

	if (dx == 0)
	{
		if (dy == 0)
		{
			MC_CHROMA4X4_X0Y0_D (pRefBlk, pPredUV, width);
		}else
		{
			MC_CHROMA4X4_XFULL_D (pRefBlk, pPredUV, width, dy);
		}
	}
	else
	{
		if (dy == 0)
		{
			MC_CHROMA4X4_YFULL_D (pRefBlk, pPredUV, width, dx);
		}else
		{
			MC_CHROMA4X4_D (pRefBlk, pPredUV, width, dx, dy);
		}
	}	 
}
