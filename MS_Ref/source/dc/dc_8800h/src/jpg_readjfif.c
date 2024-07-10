#include "JPG_Codec.h"


/* Skip over an unknown or uninteresting variable-length marker */
void SkipVariable(void)
{
	uint16 length;

	length = JPEG_GETW();

//	JPEG_TRACE("JPEG: Skipping length %d\n", length);

	for (length -= 2; length > 0; length--)
	{
		JPEG_GETC();
	}
}

/* Find the next JPEG marker */
/* Note that the output might not be a valid marker code, */
/* but it will never be 0 or FF */
int GetNextMarker(void)
{
	int c, nbytes;

	nbytes = 0;
	do
	{
		/* skip any non-FF bytes */
		do
		{			
			nbytes++;
			c = JPEG_GETC();
		} while (c != 0xFF);

		do 
		{			
			/* skip any duplicate FFs */
			nbytes++;
			c = JPEG_GETC();
		} while (c == 0xFF);
		
	} while (c == 0); /* repeat if it was a stuffed FF/00 */

	if (nbytes != 2)
	{
//		JPEG_PRINTF("JPEG: Skipped %d bytes before marker 0x%02x\n",
//			nbytes-2, c);
	}

	return c;
}

void GetSOI(void)
{
	
}

void GetDRI(void)
{
	uint16	length;

	length = JPEG_GETW();

	if (length != 4)
	{
//		JPEG_ERROR(JPEG_EID_JFIFLEN, "length = %d\n", length);
		g_JpegCodec.error_id = JPEG_EID_JFIFLEN;
		return;
	}
	
	g_JpegCodec.restart_interval = JPEG_GETW();
	
	g_JpegCodec.restart_to_go	= g_JpegCodec.restart_interval;
	g_JpegCodec.next_restart_num = 0;
	
}

void GetSOS(void)
{
	uint16	length;
	uint8	i, c, cc, n;
	uint8	yuv_id;

	length = JPEG_GETW();

	n = JPEG_GETC();  /* Number of components */
	if (n != 3)
	{
//		JPEG_ERROR(JPEG_EID_COMPNUM, "component = %d\n", n);
		g_JpegCodec.error_id = JPEG_EID_COMPNUM;
		return;
	}
	
	length -= 3;

	if (length != ( n* 2 + 3))
	{
//		JPEG_ERROR(JPEG_EID_JFIFLEN, "length = %d\n", length);
		g_JpegCodec.error_id = JPEG_EID_JFIFLEN;
		return;
	}

	/*CAUTION: current we only support YUV format, and the scan sequence is Y..U..V*/
	for (i = 0, yuv_id = 1; i < n; i++, yuv_id++)
	{
		cc = JPEG_GETC();	/*get component id*/
		c = JPEG_GETC();	/*get dc/ac table*/
		length -= 2;

		if(cc != yuv_id)
		{
//			JPEG_ERROR(JPEG_EID_COLORSPACE, "sorry, this scan sequence not support\n");
			g_JpegCodec.error_id = JPEG_EID_COLORSPACE;
			return ;
		}

		/*set huffman table*/
		g_JpegCodec.tbl_map[i].dc_huff_tbl_id = (c>>4)&0x0F;
		g_JpegCodec.tbl_map[i].ac_huff_tbl_id = c&0x0F;
	}

	/*discarding the remain bytes*/
	while (length > 0)
	{
		JPEG_GETC();
		length--;
	}
}

/*do jpeg baseline, huffman*/
void GetSOF0(void)
{
	uint16	length;
	uint8	ci, component_num, yuv_id;
	uint8	c;

	length = JPEG_GETW();				/*get length*/
	c = JPEG_GETC();		/*get sample precision*/
	if (c !=8 )
	{
// 		JPEG_ERROR(JPEG_EID_SAMPLEPRCISION, "pricision = %d\n", c);
		g_JpegCodec.error_id = JPEG_EID_COMPNUM;
		return;
	}

	g_JpegCodec.height = JPEG_GETW();
	g_JpegCodec.width = JPEG_GETW();
	if ((g_JpegCodec.height < 8)||(g_JpegCodec.width < 8))
	{
 //		JPEG_ERROR(JPEG_EID_IMGSIZE, "width =%d, height = %d\n", g_JpegCodec.height, g_JpegCodec.width);
		g_JpegCodec.error_id = JPEG_EID_IMGSIZE;
		return;
	}

	component_num = JPEG_GETC(); /*get the component number*/
	if (component_num != 3)
	{
// 		JPEG_ERROR(JPEG_EID_COMPNUM, "component num = %d\n", component_num);
		g_JpegCodec.error_id = JPEG_EID_COMPNUM;

		return;
	}

	/*check length*/
	if (length != (component_num * 3 + 8))
	{
// 		JPEG_ERROR(JPEG_EID_JFIFLEN, "length = %d\n", length);
		g_JpegCodec.error_id = JPEG_EID_JFIFLEN;
		return;
	}

//	JPEG_PRINTF("JPEG: GetSOF0, width=%d, height=%d\n",
//		g_JpegCodec.width, g_JpegCodec.height);


	/*Caution: current we only support the YUV format, sequence is Y..U..V*/
	for (ci=0; ci<component_num; ci++)
	{
		yuv_id = JPEG_GETC();
		if ((yuv_id<1)||(yuv_id>3))
		{
// 			JPEG_ERROR(JPEG_EID_COLORSPACE, "component id = %d\n", c);
			g_JpegCodec.error_id = JPEG_EID_COLORSPACE;
			return;
		}
		yuv_id--;

		c = JPEG_GETC();	/*get sample ratio*/
		g_JpegCodec.ratio[yuv_id].h_ratio = (c>>4)&0x0F;
		g_JpegCodec.ratio[yuv_id].v_ratio = (c)&0x0F;
		c = JPEG_GETC();	/*get quant table*/
		if (c > CHR_ID)
		{
// 			JPEG_ERROR(JPEG_EID_MISSQUANT, "quant id = %d\n", c);
			g_JpegCodec.error_id = JPEG_EID_MISSQUANT;
			return;
		}
		g_JpegCodec.tbl_map[yuv_id].quant_tbl_id = c;
	}

}

void GetHuffTbl(void)
{
	uint16 		length;
	uint16		i, index, count;
	HUFF_TBL_T	*htblptr;
	uint8		*bits, *huffval;

	length = JPEG_GETW();
	length -= 2;

	while (length > 0)
	{
		index = JPEG_GETC();
		if ((index&0x0F) > 1)
		{
// 			JPEG_ERROR(JPEG_EID_MISSHUFF, "huff table index = %d\n", index);
			g_JpegCodec.error_id = JPEG_EID_MISSHUFF;
			return;
		}
		
		if (index & 0x10)	
		{		
			index -= 0x10;	/* AC table definition */
			htblptr = &(g_JpegCodec.ac_huff_tbl[index]);
			bits = &(ac_bits[index][0]);
			huffval = &(ac_huffvalue[index][0]);
		}
		else
		{				/* DC table definition */
			htblptr = &(g_JpegCodec.dc_huff_tbl[index]);
			bits = &dc_bits[index][0];
			huffval = &dc_huffvalue[index][0];
		}
		
		/*read bits*/
		bits[0] = 0;
		count = 0;
		for (i = 1; i <= 16; i++)
		{
			bits[i] = JPEG_GETC();
			count += bits[i];
		}
		if (count > 256)
		{
// 			JPEG_ERROR(JPEG_EID_MISSHUFF, "huff value table len = %d\n", count);
			g_JpegCodec.error_id = JPEG_EID_MISSHUFF;
			return;
		}

		for (i = 0; i < count; i++)
		{
			huffval[i] = JPEG_GETC();
		}

		length -= 1 + 16 + count;

		htblptr->bits = bits;
		htblptr->huffval = huffval;
	}
}

void GetQuantTbl(void)
{
	uint16	length;
	uint8	n, i, prec;
	uint8	*quant_ptr;

	length = JPEG_GETW();
	length -= 2;

	while (length > 0)
	{
		n = JPEG_GETC();
		prec = n>>4;
		n &= 0x0F;
		if (prec)
		{
// 			JPEG_ERROR(JPEG_EID_MISSQUANT, "quant table precision = %d\n", prec);
			g_JpegCodec.error_id = JPEG_EID_MISSQUANT;

			return;
		}

		if (n > 1)
		{
// 			JPEG_ERROR(JPEG_EID_MISSQUANT, "quant table id = %d\n", n);
			g_JpegCodec.error_id = JPEG_EID_MISSQUANT;

			return;
		}

		quant_ptr = &quant_tbl[n][0];

		for (i = 0; i < 64; i++)
		{
			quant_ptr[i] = JPEG_GETC();
		}

		g_JpegCodec.quant_tbl[n] = quant_ptr;

		length -= 64 + 1;
	}

}

void GetAPP0(void)
{
	#define JFIF_LEN 14
	
	uint16	length;
	uint8	b[JFIF_LEN];
	uint16	buffp;

	length = JPEG_GETW();
	length -= 2;

	/* See if a JFIF APP0 marker is present */
	if (length >= JFIF_LEN)
	{
		for (buffp = 0; buffp < JFIF_LEN; buffp++)
		{
			b[buffp] = JPEG_GETC();
		}
		length -= JFIF_LEN;

		if (b[0]=='J' && b[1]=='F' && b[2]=='I' && b[3]=='F' && b[4]==0)
		{
			/* Found JFIF APP0 marker: check version */
			/* Major version must be 1 */
			if (b[5] != 1)
			{
// 				JPEG_ERROR(JPEG_EID_JFIFVERSION, "version type 0x%2x,0x%2x",b[5], b[6]);
				g_JpegCodec.error_id = JPEG_EID_JFIFVERSION;
				return;
			}
			/* Minor version should be 0 or 1, but try to process anyway if newer */
			if (b[6] > 1)
			{
				g_JpegCodec.error_id = JPEG_EID_JFIFVERSION;

// 				JPEG_TRACE("version type 0x%2x,0x%2x",b[5], b[6]);
			}

// 			JPEG_TRACE("JPEG: GetAPP0, image density unit: %d\n", b[7] );
			g_JpegCodec.xdensity = (b[8] << 8) + b[9];
// 			JPEG_TRACE("JPEG: GetAPP0, image x_density: %d\n", temp );
			g_JpegCodec.ydensity = (b[10] << 8) + b[11];
// 			JPEG_TRACE("JPEG: GetAPP0, image x_density: %d\n", temp );
		}

		while (length-- > 0)		/* skip any remaining data */
		{
			JPEG_GETC();		
		}
	}
	else
	{
// 		JPEG_ERROR(JPEG_EID_NOTJFIF," type : 0x%2x 0x%2x 0x%2x 0x%2x",
// 			b[0], b[1], b[2], b[3]);
		g_JpegCodec.error_id = JPEG_EID_NOTJFIF;

	}
}

/* Scan and process JPEG markers that can appear in any order */
/* Return when an SOI, EOI, SOFn, or SOS is found */
JPEG_MARKER_E ProcessTables ()
{
  int c;

	while (TRUE)
	{
		c = GetNextMarker();

		switch (c) 
		{
			case M_SOF0:
				GetSOF0();
				break;
				
			case M_SOI:
			case M_EOI:
			case M_SOS:
				return c;

			case M_DHT:
				GetHuffTbl();
				break;

			case M_DQT:
				GetQuantTbl();
				break;

			case M_APP0:
				GetAPP0();
				break;

			case M_DRI:
				GetDRI();
				break;

			case M_SOF1:
			case M_SOF2:
			case M_SOF3:
			case M_SOF5:
			case M_SOF6:
			case M_SOF7:
			case M_JPG:
			case M_SOF9:
			case M_SOF10:
			case M_SOF11:
			case M_SOF13:
			case M_SOF14:
			case M_SOF15:
			case M_DAC:
			case M_RST0:
			case M_RST1:
			case M_RST2:
			case M_RST3:
			case M_RST4:
			case M_RST5:
			case M_RST6:
			case M_RST7:
			case M_TEM:
	// 			JPEG_ERROR(JPEG_EID_MARKNOTSUPPORT,"Unexpected marker 0x%02x\n", c);
				return M_ERROR;

			default:	/* must be DNL, DHP, EXP, APPn, JPGn, COM, or RESn */
				SkipVariable();
				break;
		}
		if (g_JpegCodec.error_id)
		{
			return JPEG_FAILED;
		}

	}
}


uint32 ReadJpegHead(void)
{
	JPEG_MARKER_E c;

	/* Expect an SOI marker first */
	if (GetNextMarker() == M_SOI)
	{
		GetSOI();
	}
	else
	{
// 		JPEG_ERROR(JPEG_EID_FILETYPE, "File does not start with JPEG SOI marker");
		g_JpegCodec.error_id = JPEG_EID_FILETYPE;
		return JPEG_FAILED;
	}

	/* Process markers until SOF */
	c = ProcessTables();

	switch (c)
	{
		case M_ERROR:
			return JPEG_FAILED;
			
		case M_SOS:
			GetSOS();
			break;
			
		case M_EOI:
// 			JPEG_ERROR(JPEG_EID_ENCOUNTEOI, "Encount EOI marker\n");
			return JPEG_FAILED;
			
		default:
// 			JPEG_ERROR(JPEG_EID_MARKNOTSUPPORT, "Unsupported SOF marker type 0x%02x\n", c);
			return JPEG_FAILED;
	}

	if (g_JpegCodec.error_id)
	{
		return JPEG_FAILED;
	}

	return JPEG_SUCCESS;
}
