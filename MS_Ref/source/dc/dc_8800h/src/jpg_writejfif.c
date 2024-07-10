
#include "Jpg_codec.h"

/* Emit a marker code */
void PutMarker(JPEG_MARKER_E mark)
{
  JPEG_PUTC(0xFF);
  JPEG_PUTC((uint8)mark);
}

void PutAPP0(void)
{
	/*
	* Length of APP0 block	(2 bytes)
	* Block ID			(4 bytes - ASCII "JFIF")
	* Zero byte			(1 byte to terminate the ID string)
	* Version Major, Minor	(2 bytes - 0x01, 0x01)
	* Units			(1 byte - 0x00 = none, 0x01 = inch, 0x02 = cm)
	* Xdpu			(2 bytes - dots per unit horizontal)
	* Ydpu			(2 bytes - dots per unit vertical)
	* Thumbnail X size		(1 byte)
	* Thumbnail Y size		(1 byte)
	*/

	PutMarker(M_APP0);				/*marker*/
	JPEG_PUTW(2+4+1+2+1+2+2+1+1);	/*lenght*/
	JPEG_PUTC('J');					/*identifier*/
	JPEG_PUTC('F');
	JPEG_PUTC('I');
	JPEG_PUTC('F');
	JPEG_PUTC(0);
	JPEG_PUTC(1);					/*major version*/
	JPEG_PUTC(1);					/*minor version*/
	JPEG_PUTC(0);					/*pixel size info, no unit*/
	JPEG_PUTW(1);					/*x_density = 1*/
	JPEG_PUTW(1);					/*y_density = 1*/
	JPEG_PUTC(0);					/*no thumbnail image*/
	JPEG_PUTC(0);
}

void PutSOF0(void)
{
	/*
	* Length of SOF head(2 byte)
	* Sample precision(1 byte)
	* Image Height(2 byte)
	* Image Width (2 byte)
	* Component number(1 byte)
	* Component id(1 byte)
	* Sample ratio (1 byte)
	* Q table id( 1 byte)
	*/
	uint8 i;

	PutMarker(M_SOF0);				/*put SOF0 marker*/
	JPEG_PUTW(3*3+2+5+1);			/*length*/
	JPEG_PUTC(8);					/*sample precision*/
	JPEG_PUTW(g_JpegCodec.height);	/*height*/
	JPEG_PUTW(g_JpegCodec.width);	/*width*/
	JPEG_PUTC(3);					/*component number*/

	for (i=0; i<3; i++)
	{
		JPEG_PUTC((uint8)(i+1));				/*put component id, from 1 to 3*/
		JPEG_PUTC((uint8)((g_JpegCodec.ratio[i].h_ratio<<4)|(g_JpegCodec.ratio[i].v_ratio)));
		JPEG_PUTC(g_JpegCodec.tbl_map[i].quant_tbl_id);
	}
}

void PutSOS(void)
{
	/*
	* Length of scan head(2 byte)
	* Component number in scane(1 byte)
	* Csj scan component selector (1 byte)
	* Tdj DC entroy coding table ,Taj AC entropy coding table, (1 byte)
	* Start of spectral predictor selection (1 byte)
	* End of spectral predictor selection  (1 byte)
	* 0         (1 byte)
	*/

	uint8 i;

	PutMarker(M_SOS);			/*put marker*/
	JPEG_PUTW(2*3+2+1+3);		/*scan head length*/
	JPEG_PUTC(3);				/*component num*/

	for(i=0; i<3; i++)
	{
		JPEG_PUTC((uint8)(i+1));			/*put component selector for YUV*/
		JPEG_PUTC((uint8)((g_JpegCodec.tbl_map[i].dc_huff_tbl_id<<4)|g_JpegCodec.tbl_map[i].ac_huff_tbl_id));
	}

	JPEG_PUTC(0);				/*Spectral selection start */
	JPEG_PUTC(63);				/*Spectral selection end */
	JPEG_PUTC(0);				/*Successive approximation */
}


void PutQuantTbl(void)
{
	uint8 i, k;
	const uint8	*quant;
	
	/*we have two table, one for luminance, the other for chroma*/
	for (i=0; i < 2; i++)
	{
		/*
		* Length of table(2 byte)
		* Table header (1 byte)
		* Table content (64 byte)
		*/
		PutMarker(M_DQT);			/*put marker*/
		JPEG_PUTW(64 + 1 + 2);		/*length*/
		JPEG_PUTC(i);				/*table id, precision = 8*/
		quant = g_JpegCodec.quant_tbl[i];
//		JPEG_ASSERT(quant != 0);
		for (k=0; k<64; k++)
		{
			JPEG_PUTC(quant[k]);
		}
	}
}

void PutOneHuffTbl(uint8 index, const uint8 *bits, const uint8 *huffval)
{
	uint16 count, i, length;

//	JPEG_ASSERT(bits != 0);
//	JPEG_ASSERT(huffval != 0);
	
	/*get the symbol count*/
	for (i=1, count=0; i<=16; i++)
	{
		count += bits[i];
	}
//	JPEG_ASSERT(count <= 256 );
	
	/*get the data length*/
	length = 2 + 1 + 16 + count;

	/*
	*  length of table(2 byte)
	*  table id(1 byte)
	*  bits table(16 byte)  
	*  huff value table
	*/
	PutMarker(M_DHT);			/*put marker*/
	JPEG_PUTW(length);			/*length*/
	JPEG_PUTC(index);			/*table id*/

	for(i=1; i<=16 ; i++)		/*put bits*/
	{
		JPEG_PUTC(bits[i]);
	}

	for(i=0; i<count; i++)
	{
		JPEG_PUTC(huffval[i]);
	}

}

void PutHuffTbl(void)
{
	/*put lum dc tbl*/
	PutOneHuffTbl(
		0x00,
		g_JpegCodec.dc_huff_tbl[LUM_ID].bits,
		g_JpegCodec.dc_huff_tbl[LUM_ID].huffval
		);

	/*put lum ac tbl*/
	PutOneHuffTbl(
		0x10,
		g_JpegCodec.ac_huff_tbl[LUM_ID].bits,
		g_JpegCodec.ac_huff_tbl[LUM_ID].huffval
		);

	/*put chr dc tbl*/
	PutOneHuffTbl(
		0x01,
		g_JpegCodec.dc_huff_tbl[CHR_ID].bits,
		g_JpegCodec.dc_huff_tbl[CHR_ID].huffval
		);

	/*put chr ac tbl*/
	PutOneHuffTbl(
		0x11,
		g_JpegCodec.ac_huff_tbl[CHR_ID].bits,
		g_JpegCodec.ac_huff_tbl[CHR_ID].huffval
		);

}

void WriteDRI(void)
{
	if (g_JpegCodec.restart_interval)
	{
		PutMarker(M_DRI);			/*put marker*/
		JPEG_PUTW(4);			/*length*/
		JPEG_PUTW((uint16) g_JpegCodec.restart_interval);			/*interval*/	
	}
}


uint32 WriteJpegHead(void)
{
	/*put SOI*/
	PutMarker(M_SOI);


	/*put APP)*/
	PutAPP0();

	/*put quant tbl*/
	PutQuantTbl();

	/*put SOF0*/
	PutSOF0();

	/*put huffman tbl*/
	PutHuffTbl();
	
	/*put DRI*/
	WriteDRI();

	/*put SOS*/
	PutSOS();

	return JPEG_SUCCESS;
}

void WriteJpegTail(void)
{
	JPEG_FLUSH(0xFFFFFFFF);
	PutMarker(M_EOI);
}
