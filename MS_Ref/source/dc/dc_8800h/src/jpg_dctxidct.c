#include "JPG_DCTxIDCT.h"
///////////////////////////////////////////////////////////////////////////////

void Initialize_Clip()
{
	short i;
	pClip_table = iclip+512;
	for (i= -512; i<512; i++)
		pClip_table[i] = (i<0) ? 0 : ((i>255) ? 255 : i);
}

#ifndef _VSP_

/************************************************************************/
/* DCT Transform                                                        */
/************************************************************************/
//cos(i/16*pi)*2^13, 13-bit
static const int im_c1z = 0x1f62;
static const int im_c2z = 0x1d90;
static const int im_c3z = 0x1a9b;
static const int im_c4z = 0x16a0;
static const int im_c5z = 0x11c7;
static const int im_c6z = 0x0c3e;
static const int im_c7z = 0x063e;

//cos(i/16*pi)*2^15, 15-bit
static const int im_c1x = 0x7d8a;
static const int im_c2x = 0x7642;
static const int im_c3x = 0x6a6e;
static const int im_c4x = 0x5a82;
static const int im_c5x = 0x471d;
static const int im_c6x = 0x30fc;
static const int im_c7x = 0x18f9; 


static __inline void mpeg4Enc_fdct8x8_C_row(uint8 * block, int16 * rgiDst)
{
    int32 i;
    uint8 * sIn;
   	int32 t0, t1, t2, t3, t4, t5, t6, t7;
   	int32 s0, s1, s2, s3;

	//Row transform
	sIn = block;

    for (i = 0; i < 8; i++)	
    {
    	//Step 1
    	t0 = sIn[0] + sIn[7]; 
    	t1 = sIn[1] + sIn[6]; 
    	t2 = sIn[2] + sIn[5]; 
    	t3 = sIn[3] + sIn[4];
    	
    	t4 = sIn[0] - sIn[7]; 
    	t5 = sIn[1] - sIn[6];	
    	t6 = sIn[2] - sIn[5]; 
    	t7 = sIn[3] - sIn[4];

		//Step 2: 0123
    	s0 = t0 + t3;
    	s1 = t1 + t2;
    	s2 = t0 - t3;
    	s3 = t1 - t2;

		//14+16+1 = 31; 31 - 15 = 16
    	rgiDst[4] = (int16) ( ( ( s0 - s1) * im_c4x + (1<<11) )>>12 );
	    rgiDst[0] = (int16) ( ( ( s0 + s1) * im_c4x + (1<<11) )>>12 );
    	rgiDst[6] = (int16) ( ( s2 * im_c6x - s3 * im_c2x + (1<<11) )>>12 );
	    rgiDst[2] = (int16) ( ( s2 * im_c2x + s3 * im_c6x + (1<<11) )>>12 );

		//Step 3: 4567
    	s0 = t4<<3;
	    s1 = t7<<3;
    	s2 = ((t5 - t6) * im_c4x + (1<<11) )>>12;
    	s3 = ((t5 + t6) * im_c4x + (1<<11) )>>12;
    
    	t6 = s0 - s3;
    	t4 = s0 + s3;
    	t7 = s1 - s2;
    	t5 = s1 + s2;

    	rgiDst[7] = (int16) ( ( t4 * im_c7x - t5 * im_c1x + (1<<14) )>>15 ); //15 + 15 + 1 = 31; 31 -15 = 16
    	rgiDst[1] = (int16) ( ( t5 * im_c7x + t4 * im_c1x + (1<<14) )>>15 );
    	rgiDst[3] = (int16) ( ( t6 * im_c3x - t7 * im_c5x + (1<<14) )>>15 );
    	rgiDst[5] = (int16) ( ( t7 * im_c3x + t6 * im_c5x + (1<<14) )>>15 );

		sIn += 8;
		rgiDst += 8;
	}
}

//Col transform
static __inline void mpeg4Enc_fdct8x8_int_col(int16 * block)
{
    int32 i;
    int16 *sIn;
   	int32 t0, t1, t2, t3, t4, t5, t6, t7;
   	int32 s0, s1, s2, s3;

	sIn = block;
	
    for (i = 0; i < 8; i++)	
    {
		//Step 1
	    t0 = sIn[8*0] + sIn[8*7];//16+1 = 17-bit
    	t1 = sIn[8*1] + sIn[8*6];
	    t2 = sIn[8*2] + sIn[8*5];
    	t3 = sIn[8*3] + sIn[8*4];
	    t4 = sIn[8*0] - sIn[8*7];
    	t5 = sIn[8*1] - sIn[8*6];
	    t6 = sIn[8*2] - sIn[8*5];
    	t7 = sIn[8*3] - sIn[8*4];

		//Step 2: 0123
	    s0 = t0 + t3; //17+1 = 18-bit
    	s1 = t1 + t2;
	    s2 = t0 - t3;    
    	s3 = t1 - t2;

    	sIn[8*4] = (int16) ( ( s0 * im_c4z - s1 * im_c4z + (1<<17) )>>18 ); //18+13+1 = 32-bit; 32-18=14-bit
	    sIn[8*0] = (int16) ( ( s0 * im_c4z + s1 * im_c4z + (1<<17) )>>18 );
    	sIn[8*6] = (int16) ( ( s2 * im_c6z - s3 * im_c2z + (1<<17) )>>18 );
	    sIn[8*2] = (int16) ( ( s3 * im_c6z + s2 * im_c2z + (1<<17) )>>18 );

		//Step 3: 4567
	    s0 = t4; //17 + 2 = 19bit 
    	s1 = t7;
    	s2 = ((t5 - t6) * im_c4x + (1<<14) )>>15; //17 + 1 + 10.5 - 9 = 19.5bit
	    s3 = ((t5 + t6) * im_c4x + (1<<14) )>>15;

	    t4 = s0 + s3;
    	t5 = s1 + s2;

	    t6 = s0 - s3;//20.5bit
    	t7 = s1 - s2;
    	
    	sIn[8*7] = (int16) ( ( t4 * im_c7z - t5 * im_c1z + (1<<17) )>>18 ); //20.5 + 11 + 1 =32.5bit;??? 32.5-18 = 14.5bit
	    sIn[8*1] = (int16) ( ( t5 * im_c7z + t4 * im_c1z + (1<<17) )>>18 );
    	sIn[8*3] = (int16) ( ( t6 * im_c3z - t7 * im_c5z + (1<<17) )>>18 );
	    sIn[8*5] = (int16) ( ( t7 * im_c3z + t6 * im_c5z + (1<<17) )>>18 );
    
		sIn ++;
	}
}

void JPEG_SoftDCT(uint8 * block, int16 *rgiDst)
{
	
	mpeg4Enc_fdct8x8_C_row(block, rgiDst);

	mpeg4Enc_fdct8x8_int_col(rgiDst);
}

/************************************************************************/
/* IDCT Transform                                                       */
/************************************************************************/

////////////////////////////////////////////////////////////////////////
void JPEG_SoftIDCT(int16 * block, uint8 * pixelData)
{
	int i;
	int16 *blk;
	int16 * pDstTmp;
#ifdef _VSP_
	uint8 * blkDst;
#else
	int16 * blkDst;
#endif
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;
	
	for (i=0; i<8; i++)
	{
		blk = block + 8*i;
		pDstTmp =blk;
		
		x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */
		x1 = blk[4]<<11;
		x2 = blk[6];  x3 = blk[2];
		x4 = blk[1];  x5 = blk[7];
		x6 = blk[5];  x7 = blk[3];
		
		/* first stage */
		x8 = W7*(x4+x5);
		x4 = x8 + (W1-W7)*x4;
		x5 = x8 - (W1+W7)*x5;
		x8 = W3*(x6+x7);
		x6 = x8 - (W3-W5)*x6;
		x7 = x8 - (W3+W5)*x7;
		
		/* second stage */
		x8 = x0 + x1;
		x0 -= x1;
		x1 = W6*(x3+x2);
		x2 = x1 - (W2+W6)*x2;
		x3 = x1 + (W2-W6)*x3;
		x1 = x4 + x6;
		x4 -= x6;
		x6 = x5 + x7;
		x5 -= x7;
		
		/* third stage */
		x7 = x8 + x3;
		x8 -= x3;
		x3 = x0 + x2;
		x0 -= x2;
		x2 = (181*(x4+x5)+128)>>8;
		x4 = (181*(x4-x5)+128)>>8;
		
		/* fourth stage */
		pDstTmp[0] = (x7+x1)>>8;
		pDstTmp[1] = (x3+x2)>>8;
		pDstTmp[2] = (x0+x4)>>8;
		pDstTmp[3] = (x8+x6)>>8;
		pDstTmp[4] = (x8-x6)>>8;
		pDstTmp[5] = (x0-x4)>>8;
		pDstTmp[6] = (x3-x2)>>8;
		pDstTmp[7] = (x7-x1)>>8;
	}
	
	for (i=0; i<8; i++)
	{
		blk = block + i;
#ifdef _VSP_
		blkDst = pixelData + i;
#else
		blkDst = block + i;
#endif
		x0 = (blk[8*0]<<8) + 8192;
		x1 = (blk[8*4]<<8);
		x2 = blk[8*6];  x3 = blk[8*2];
		x4 = blk[8*1];  x5 = blk[8*7];
		x6 = blk[8*5];  x7 = blk[8*3];
		
		/* first stage */
		x8 = W7*(x4+x5) + 4;
		x4 = (x8+(W1-W7)*x4)>>3;
		x5 = (x8-(W1+W7)*x5)>>3;
		x8 = W3*(x6+x7) + 4;
		x6 = (x8-(W3-W5)*x6)>>3;
		x7 = (x8-(W3+W5)*x7)>>3;
		
		/* second stage */
		x8 = x0 + x1;
		x0 -= x1;
		x1 = W6*(x3+x2) + 4;
		x2 = (x1-(W2+W6)*x2)>>3;
		x3 = (x1+(W2-W6)*x3)>>3;
		x1 = x4 + x6;
		x4 -= x6;
		x6 = x5 + x7;
		x5 -= x7;
		
		/* third stage */
		x7 = x8 + x3;
		x8 -= x3;
		x3 = x0 + x2;
		x0 -= x2;
		x2 = (181*(x4+x5)+128)>>8;
		x4 = (181*(x4-x5)+128)>>8;
		
		/* fourth stage */
#ifdef _VSP_
		blkDst[8*0] = pClip_table[(x7+x1)>>14];
		blkDst[8*1] = pClip_table[(x3+x2)>>14];
		blkDst[8*2] = pClip_table[(x0+x4)>>14];
		blkDst[8*3] = pClip_table[(x8+x6)>>14];
		blkDst[8*4] = pClip_table[(x8-x6)>>14];
		blkDst[8*5] = pClip_table[(x0-x4)>>14];
		blkDst[8*6] = pClip_table[(x3-x2)>>14];
		blkDst[8*7] = pClip_table[(x7-x1)>>14];
#else
		blkDst[8*0] = (x7+x1)>>14;
		blkDst[8*1] = (x3+x2)>>14;
		blkDst[8*2] = (x0+x4)>>14;
		blkDst[8*3] = (x8+x6)>>14;
		blkDst[8*4] = (x8-x6)>>14;
		blkDst[8*5] = (x0-x4)>>14;
		blkDst[8*6] = (x3-x2)>>14;
		blkDst[8*7] = (x7-x1)>>14;
#endif
	}
} 

#endif
