#include "JPG_Codec.h"
#include "os_api.h"

#define COEFF_INDEX(i)  (zigzag_order[i])

int32 num = 0; int32 ac_num = 0;
int32 total_bit = 0; int32 ac_total_bit = 0;

void FixHuffTbl(HUFF_TBL_T *htbl)
{
	uint16 p, i, l, lastp, si;
	uint8 v_huffsize[257];
	uint16 v_huffcode[257];
	uint16 code;
	int lookbits, ctr;

	int16  symbol_num = 0;

	/* Figure 7.3.5.4.2.1: make table of Huffman code length for each symbol */
	/* Note that this is in code-length order. */
	SCI_MEMSET(v_huffsize, 0, 257);
	SCI_MEMSET(v_huffcode, 0xFF, 257*2);

	p = 0;
	for (l = 1; l <= 16; l++) 
	{
		for (i = 1; i <= htbl->bits[l]; i++)
		{
			v_huffsize[p] = (uint8)l;
			p++;
		}
	}
	v_huffsize[p] = 0;
	lastp = p;
	
	symbol_num = p;

	/* Figure 7.3.5.4.2.2: generate the codes themselves */
	/* Note that this is in code-length order. */

	code = 0;
	si = v_huffsize[0];
	p = 0;
	while (v_huffsize[p]) 
	{
		while (v_huffsize[p] == si) 
		{
			v_huffcode[p] = code;
			p++;
			code++;
		}
		code <<= 1;
		si++;
	}

	/* Figure 7.3.5.4.2.3: generate encoding tables */
	/* These are code and size indexed by symbol value */

	for (p = 0; p < lastp; p++) 
	{
		htbl->ehufco[htbl->huffval[p]] = v_huffcode[p];
		htbl->ehufsi[htbl->huffval[p]] = v_huffsize[p];
	}


	/* Figure 13.4.2.3.1: generate decoding tables */

	p = 0;
	for (l = 1; l <= 16; l++) 
	{
		if (htbl->bits[l]) 
		{
			htbl->offset[l] = (int32)p - (int32)v_huffcode[p]; /* offset */
			p += htbl->bits[l];
			htbl->maxcode[l] = v_huffcode[p-1];	/* maximum code of length l */
		} 
		else
		{
			htbl->maxcode[l] = -1;
		}
		
	}

	SCI_MEMSET(htbl->look_nbits, 0, sizeof(htbl->look_nbits));
	
	p = 0;
	for (l = 1; l <= HUFF_FIRST_READ; l++) 
	{
		for (i = 1; i <= (int) htbl->bits[l]; i++, p++) 
		{
			/* l = current code's length, p = its index in v_huffcode[] & huffval[]. */
			/* Generate left-justified code followed by all possible bit sequences */
			lookbits = v_huffcode[p] << (HUFF_FIRST_READ-l);
			for (ctr = 1 << (HUFF_FIRST_READ-l); ctr > 0; ctr--) 
			{
				htbl->look_nbits[lookbits] = l;
				htbl->look_sym[lookbits] = htbl->huffval[p];
				lookbits++;
			}
		}
	}
}

void InitHuffTbl(void)
{	
	FixHuffTbl(&g_JpegCodec.dc_huff_tbl[LUM_ID]);

	FixHuffTbl(&g_JpegCodec.ac_huff_tbl[LUM_ID]);

	FixHuffTbl(&g_JpegCodec.dc_huff_tbl[CHR_ID]);
	
	FixHuffTbl(&g_JpegCodec.ac_huff_tbl[CHR_ID]);

}

void InitHuffTblWithDefaultValue(void)
{
	g_JpegCodec.dc_huff_tbl[LUM_ID].bits = &lum_dc_bits_default[0];
	g_JpegCodec.dc_huff_tbl[LUM_ID].huffval = &lum_dc_huffvalue_default[0];
	g_JpegCodec.ac_huff_tbl[LUM_ID].bits = &lum_ac_bits_default[0];
	g_JpegCodec.ac_huff_tbl[LUM_ID].huffval = &lum_ac_huffvalue_default[0];

	g_JpegCodec.dc_huff_tbl[CHR_ID].bits = &chr_dc_bits_default[0];
	g_JpegCodec.dc_huff_tbl[CHR_ID].huffval = &chr_dc_huffvalue_default[0];
	g_JpegCodec.ac_huff_tbl[CHR_ID].bits = &chr_ac_bits_default[0];
	g_JpegCodec.ac_huff_tbl[CHR_ID].huffval = &chr_ac_huffvalue_default[0];

	FixHuffTbl(&g_JpegCodec.dc_huff_tbl[LUM_ID]);

	FixHuffTbl(&g_JpegCodec.ac_huff_tbl[LUM_ID]);

	FixHuffTbl(&g_JpegCodec.dc_huff_tbl[CHR_ID]);

	FixHuffTbl(&g_JpegCodec.ac_huff_tbl[CHR_ID]);

	g_JpegCodec.tbl_map[Y_ID].dc_huff_tbl_id = LUM_ID;
	g_JpegCodec.tbl_map[Y_ID].ac_huff_tbl_id = LUM_ID;
	g_JpegCodec.tbl_map[U_ID].dc_huff_tbl_id = CHR_ID;
	g_JpegCodec.tbl_map[U_ID].ac_huff_tbl_id = CHR_ID;
	g_JpegCodec.tbl_map[V_ID].dc_huff_tbl_id = CHR_ID;
	g_JpegCodec.tbl_map[V_ID].ac_huff_tbl_id = CHR_ID;
}


void Vlc_Quant_Block(int16 *block, HUFF_TBL_T *dc_tbl, HUFF_TBL_T *ac_tbl, const uint16 *quant)
{
	int32 temp, temp1;
	int16 k, r, i;
	int16 nbits;
	int8  leadzero = 0;
	/* Encode the DC coefficient difference per section 7.3.5.1 */

	/* Find the number of bits needed for the magnitude of the coefficient */
	{
		temp = block[0];
		temp1 = temp;
		/* If positive, emit nbits low order bits; */
		/* if negative, emit nbits low order bits of value-1 */
		if (temp1 < 0)
		{
			temp1--;
			temp = -temp;
		}
#ifndef _ARM_
		JPEG_GETVALSIZE(temp);
#else
		__asm
		{
			clz leadzero, temp;
		}
		nbits = 32 - leadzero;
#endif
		
		/* Emit the Huffman-coded symbol for the number of bits */
		JPEG_PUTBITS(dc_tbl->ehufco[nbits], dc_tbl->ehufsi[nbits]);

		JPEG_PUTBITS((uint16)temp1, nbits);		
	}

	/* Encode the AC coefficients per section 7.3.5.2 */

	r = 0;			/* r = run length of zeros */

	for (k = 1; k < 64; k++)
	{
		temp = (block[COEFF_INDEX(k)] * quant[(k)] + (1<<14))>>15;
		
		if (temp == 0)
		{
			r++;
		}
		else
		{
			temp1 = temp;
			
			/* if run length > 15, must emit special run-length-16 codes (0xF0) */
			while (r > 15) 
			{
				JPEG_PUTBITS(ac_tbl->ehufco[0xF0], ac_tbl->ehufsi[0xF0]);
				r -= 16;
			}

			/* Find the number of bits needed for the magnitude of the coefficient */

			/* If positive, emit nbits low order bits; */
			/* if negative, emit nbits low order bits of value-1 */
			if (temp1 < 0)
			{
				temp1--;
				temp = -temp;
			}
			

#ifndef _ARM_
			JPEG_GETVALSIZE(temp);
#else
			__asm
			{
				clz leadzero, temp;
			}
			nbits = 32 - leadzero;
#endif
			/* Emit Huffman symbol for run length / number of bits */
			i = (r << 4) + nbits;
			JPEG_PUTBITS(ac_tbl->ehufco[i], ac_tbl->ehufsi[i]);/*lint !e662 !e661*/

			JPEG_PUTBITS((uint16) temp1, nbits);
			
			r = 0;
		}
	}

	/* If the last coef(s) were zero, emit an end-of-block code */
	if (r > 0)
	{
		JPEG_PUTBITS(ac_tbl->ehufco[0], ac_tbl->ehufsi[0]);
	}
	
}


#define huff_EXTEND(x, s)	((x) < (1 << ((s)-1)) ? \
				 (x) + (-1 << (s)) + 1 : \
				 (x))

uint8 huff_DECODE (HUFF_TBL_T * htbl, int min_bits)
{
	register uint16 l = min_bits;
	register int32 code;

  CHECK_BIT_BUFFER(l);
  code = JPEG_GETBITS(l);
  while (code > htbl->maxcode[l])
  {
	  CHECK_BIT_BUFFER(1);
	  code = ((code << 1) | JPEG_GETBITS(1));
	  l++;
  }

  if (l >16)
  {
	  return 0;
  }
  
  return htbl->huffval[(int32)(code + htbl->offset[l]) ];
}

BOOLEAN Vld_DQuant_Block(
	int16 		*block,
	HUFF_TBL_T 	*dc_tbl,
	HUFF_TBL_T 	*ac_tbl,
	const uint16 *quant
	)
{
	int32 s, k, r, n;
	/* zero out the coefficient block */
	SCI_MEMSET((void *) block, 0, 64*sizeof(int16));

	/* Section 13.4.2.1: decode the DC coefficient difference */
	HUFF_DECODE(s, dc_tbl, label1);

	//For Test: caculate the mean of the code length

	//End Test
	CHECK_BIT_BUFFER(s)
	r = JPEG_GETBITS((uint8)s);
	block[0] = (int16)huff_EXTEND(r, (uint8)s);

	/* Section 13.4.2.2: decode the AC coefficients */
	for (k = 1; k < 64; k++)
	{
		
		HUFF_DECODE(r, ac_tbl, label2);

		s = r & 15;
		n = r >> 4;
		if (s)
		{
			k = k + n;
			CHECK_BIT_BUFFER(s)
			r = JPEG_GETBITS((uint8)s);
			if (k<64)
			{
				block[COEFF_INDEX(k)] = quant[(k)] * ((int16)huff_EXTEND(r, s));
			}
			//For Test: caculate the mean of the code length
		}
		else
		{
			if (n != 15)
			{
				break;
			}
			k += 15;
		}
	}
	return TRUE;
}


