#include "ms_ref_diplay_trc.h"
#include "img.h"
#include "bmp.h"
#include "img_port.h"

#ifdef WIN32
void BMP_CreateDispCtl(
		uint16 width, 
		uint16 height, 
		uint16 bitperpixel,
		uint32 r_mask,
		uint32 g_mask,
		uint32 b_mask,
		IMG_DISPCTL_T *dispctl
		)
{
	uint32 len = width*height*bitperpixel/8;
	dispctl->dispmem = IMG_AllocMem(len);
	dispctl->r_bitmask = r_mask;
	dispctl->g_bitmask = g_mask;
	dispctl->b_bitmask = b_mask;
	dispctl->bits_per_pixel = bitperpixel;
	dispctl->height = height;
	dispctl->width = width;
}

void BMP_SaveDispCtl(
	char *name, 
	IMG_DISPCTL_T *dispctl)
{
	IMG_FILE			*fp;
	BITMAPINFOHEADER 	infoheader;
	BITMAPFILEHEADER 	fileheader;
	uint32 				size, image_size, offset, info_size;

	/* Try opening the file; use "wb" mode to write this *binary* file. */
	if ((fp = IMG_OpenFile(name, IMG_OPENMODE_CREATE)) == NULL)
	{
		IMG_SET_LASTERROR(IMG_EID_FILEWRITE);	
		return IMG_FAILED;
	}

	image_size = dispctl->width * dispctl->height * dispctl->bits_per_pixel / 8;
	offset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 12;
	info_size = sizeof(BITMAPINFOHEADER) + 12;
	size = image_size + offset;

	/* Write the file header, bitmap information, and bitmap pixel data... */
	fileheader.bfType      = BF_TYPE; /* Non-portable... sigh */
	fileheader.bfSize      = size;
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits   = offset;

	BMPHEAD_ENDIAN_EXCHANGE(&fileheader);
	
	if (IMG_WriteFile((void*)(&fileheader), sizeof(BITMAPFILEHEADER), fp) < sizeof(BITMAPFILEHEADER))
	{
		/* Couldn't write the file header - return... */
		IMG_CloseFile(fp);
		return IMG_FAILED;
	}

	infoheader.biBitCount = dispctl->bits_per_pixel;
	infoheader.biClrImportant = 0;
	infoheader.biClrUsed = 0;
	infoheader.biCompression = BI_BITFIELDS;
	infoheader.biHeight = dispctl->height;
	infoheader.biPlanes = 1;
	infoheader.biSize = info_size;
	infoheader.biSizeImage = image_size;
	infoheader.biWidth = dispctl->width;
	infoheader.biXPelsPerMeter = 0;
	infoheader.biYPelsPerMeter = 0;

	BMPINFO_ENDIAN_EXCHANGE(&infoheader);

	if (IMG_WriteFile((void*)(&infoheader), sizeof(BITMAPINFOHEADER), fp) < sizeof(BITMAPINFOHEADER))
	{
		/* Couldn't write the bitmap header - return... */
		IMG_CloseFile(fp);
		return IMG_FAILED;
	}

	IMG_WriteFile(&(dispctl->r_bitmask),4,fp);
	IMG_WriteFile(&(dispctl->g_bitmask),4,fp);
	IMG_WriteFile(&(dispctl->b_bitmask),4,fp);

	if (IMG_WriteFile(dispctl->dispmem, image_size, fp) < image_size)
	{
		/* Couldn't write the bitmap - return... */
		IMG_CloseFile(fp);
		return IMG_FAILED;
	}

	/* OK, everything went fine - return... */
	IMG_CloseFile(fp);
	return IMG_SUCCESS;	
}

void BMP_DecodeTest(char *srcfile, char *destfile)
{
	uint8 	*bmpfile_stream,
			*code_stream;
	uint32 	filelen;
	
	if (IMG_Load(srcfile, &code_stream))
	{
		//IMG_PRINT:"load image failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_108_112_2_18_1_27_18_181,(uint8*)"");
		return;
	}

	if (IMG_ExtractBmp(code_stream, &bmpfile_stream, &filelen))
	{
		IMG_FreeMem(code_stream);
		//IMG_PRINT:"compress failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_115_112_2_18_1_27_18_182,(uint8*)"");
		return;
	}

	if (IMG_Save(destfile, bmpfile_stream))
	{
		//IMG_PRINT:"save failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_121_112_2_18_1_27_18_183,(uint8*)"");
	}

	IMG_FreeMem(bmpfile_stream);
	IMG_FreeMem(code_stream);

}

void BMP_EncodeTest(char *srcfile, char *destfile)
{
	uint8 	*bmpfile_stream,
			*code_stream;
	uint32 	codelen;
	
	if (IMG_Load(srcfile, &bmpfile_stream))
	{
		//IMG_PRINT:"load image failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_137_112_2_18_1_27_18_184,(uint8*)"");
		return;
	}

	if (IMG_CompressBmp(bmpfile_stream, &code_stream, &codelen))
	{
		IMG_FreeMem(bmpfile_stream);
		//IMG_PRINT:"compress failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_144_112_2_18_1_27_18_185,(uint8*)"");
		return;
	}

	if (IMG_Save(destfile, code_stream))
	{
		//IMG_PRINT:"save failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_150_112_2_18_1_27_18_186,(uint8*)"");
	}

	IMG_FreeMem(bmpfile_stream);
	IMG_FreeMem(code_stream);

}

void MOVIE_DecodeTest(char *srcfile, char *destdir)
{
	uint8 	*code_stream;
	uint32 	framenum;
	
	if (IMG_Load(srcfile, &code_stream))
	{
		//IMG_PRINT:"load image failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_165_112_2_18_1_27_18_187,(uint8*)"");
		return;
	}

	if (IMG_ExtractMovie(code_stream, destdir, &framenum))
	{
		IMG_FreeMem(code_stream);
		//IMG_PRINT:"compress failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_172_112_2_18_1_27_18_188,(uint8*)"");
		return;
	}

	IMG_FreeMem(code_stream);

}

void MOVIE_EncodeTest(char *dirname, uint32 framenum, char *destfile)
{
	uint8 	*code_stream;
	uint32 	codelen;
	
	if (IMG_CompressMovie(dirname, framenum, &code_stream, &codelen))
	{
		IMG_FreeMem(code_stream);
		//IMG_PRINT:"compress failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_188_112_2_18_1_27_18_189,(uint8*)"");
		return;
	}

	if (IMG_Save(destfile, code_stream))
	{
		//IMG_PRINT:"save failed\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_194_112_2_18_1_27_18_190,(uint8*)"");
	}

	IMG_FreeMem(code_stream);

}

void DISPBMP_Test(char *filename, IMG_DISPCTL_T *dispctl, uint32 scale)
{
	IMG_RECT_T bmprect;
	IMG_RECT_T disprect;
	IMG_INFO_T img_info;
	
	uint8 *bmpstream;
	uint32 width;
	uint32 height;

	if (IMG_Load(filename, &bmpstream))
	{
		//IMG_PRINT:"\r\n load failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_213_112_2_18_1_27_18_191,(uint8*)"");
		return;
	}

	if (IMG_GetImageInfo(bmpstream, &img_info))
	{
		IMG_FreeMem(bmpstream);
		//IMG_PRINT:"\r\n get info failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_220_112_2_18_1_27_18_192,(uint8*)"");
		return;
	}

	height =img_info.height / scale;
	width = img_info.width / scale;
	if (scale >1)
	{
		bmprect.top = 5;
		bmprect.bottom = bmprect.top + height -1;
		bmprect.left = 5;
		bmprect.right = bmprect.left + width -1;
	}
	else
	{
		bmprect.top = 0;
		bmprect.bottom = bmprect.top + height -1;
		bmprect.left = 0;
		bmprect.right = bmprect.left + width -1;
	}

	disprect.top = 10;
	disprect.bottom = disprect.top + height - 1;
	disprect.left = 10;
	disprect.right = disprect.left + width -  1;
	
	if (IMG_DisplayBmp(bmpstream, &bmprect, dispctl, &disprect))
	{
		IMG_FreeMem(bmpstream);
		//IMG_PRINT:"\r\n disp failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_249_112_2_18_1_27_18_193,(uint8*)"");
		return;
	}
}

void DISPMOVIE_Test(char *filename, IMG_DISPCTL_T *dispctl, uint32 frameid, uint32 scale)
{
	IMG_RECT_T bmprect;
	IMG_RECT_T disprect;
	IMG_INFO_T img_info;
	
	uint8 *bmpstream;
	uint32 width;
	uint32 height;

	if (IMG_Load(filename, &bmpstream))
	{
		//IMG_PRINT:"\r\n load failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_266_112_2_18_1_27_18_194,(uint8*)"");
		return;
	}

	if (IMG_GetImageInfo(bmpstream, &img_info))
	{
		IMG_FreeMem(bmpstream);
		//IMG_PRINT:"\r\n get info failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_273_112_2_18_1_27_18_195,(uint8*)"");
		return;
	}

	height =img_info.height / scale;
	width = img_info.width / scale;
	if (scale >1)
	{
		bmprect.top = 5;
		bmprect.bottom = bmprect.top + height -1;
		bmprect.left = 5;
		bmprect.right = bmprect.left + width -1;
	}
	else
	{
		bmprect.top = 0;
		bmprect.bottom = bmprect.top + height -1;
		bmprect.left = 0;
		bmprect.right = bmprect.left + width -1;
	}

	disprect.top = 10;
	disprect.bottom = disprect.top + height - 1;
	disprect.left = 10;
	disprect.right = disprect.left + width -  1;
	
	if (IMG_DisplayMovie(bmpstream, frameid, &bmprect, dispctl, &disprect))
	{
		IMG_FreeMem(bmpstream);
		//IMG_PRINT:"\r\n disp failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_302_112_2_18_1_27_18_196,(uint8*)"");
		return;
	}
}

void main(void)
{
	IMG_DISPCTL_T dispctl;

	BMP_CreateDispCtl(256, 256, 
		16, 
		0x00000F00, 0x000000F0, 0x0000000F, 
		&dispctl
		);

	DISPBMP_Test("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\01.code" ,&dispctl, 1);
	BMP_SaveDispCtl("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\scale2.bmp", &dispctl);

	DISPBMP_Test("D:\\spiht_bitmap\\bmp\\gif_awaiting\\01.bmp" ,&dispctl, 1);
	BMP_SaveDispCtl("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\scale1.bmp", &dispctl);

	DISPMOVIE_Test("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\movie.code" ,&dispctl, 0, 2);
	BMP_SaveDispCtl("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\scale2.bmp", &dispctl);

	DISPMOVIE_Test("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\movie.code" ,&dispctl, 1, 1);
	BMP_SaveDispCtl("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\scale1.bmp", &dispctl);

	IMG_FreeMem(dispctl.dispmem);
	
	BMP_EncodeTest("D:\\spiht_bitmap\\bmp\\gif_awaiting\\01.bmp",
		"D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\01.code");

	BMP_DecodeTest("D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\01.code",
		"D:\\spiht_bitmap\\bmp\\gif_awaitingnew\\01.bmp");

	MOVIE_EncodeTest("D:\\spiht_bitmap\\bmp\\gif_calling\\", 4,
		"D:\\spiht_bitmap\\bmp\\gif_callingnew\\movie.code");

	MOVIE_DecodeTest("D:\\spiht_bitmap\\bmp\\gif_callingnew\\movie.code",
		"D:\\spiht_bitmap\\bmp\\gif_callingnew\\frames\\");

}

#else

extern uint16  g_clcd_buffer[160][128];

void BMP_CreateDispCtl(IMG_DISPCTL_T *dispctl)
{
	dispctl->bits_per_pixel = 16;
	dispctl->r_bitmask = 0x0000F800;
	dispctl->g_bitmask = 0x000007E0;
	dispctl->b_bitmask = 0x0000001F;
	dispctl->dispmem = (uint8 *)g_clcd_buffer;
	dispctl->height = 160;
	dispctl->width = 128;
}

void IMG_Test(void)
{
	IMG_DISPCTL_T dispctl;
	IMG_INFO_T img_info;
	IMG_FILE *file;
	uint8 *stream;
	IMG_RECT_T bmprect;
	IMG_RECT_T disprect;
	uint32 i;

	BMP_CreateDispCtl(&dispctl);
	
	file = IMG_OpenFile("aaa", IMG_OPENMODE_READONLY);

	stream = (uint8 *)file;

	if (IMG_GetImageInfo(stream, &img_info))
	{
		//IMG_PRINT:"\r\n get info failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,IMG_TEST_378_112_2_18_1_27_19_197,(uint8*)"");
		return;
	}
	
	
	bmprect.left = 0;
	bmprect.top = 0;
	bmprect.bottom = 128-1;
	bmprect.right = 128-1;

	disprect.left = 0;
	disprect.top = 16;
	disprect.bottom = 144-1;
	disprect.right = 128-1;

	if (img_info.framenum <2)
	{
		if (IMG_DisplayBmp(stream, &bmprect, &dispctl, &disprect))
		{
			BREAK_POINT("call dispbmp failed");
		}
		LCD_Invalidate();
	}
	else
	{
		for (i=0; i<img_info.framenum; i++)
		{
			if (IMG_DisplayMovie(stream, i, &bmprect, &dispctl, &disprect))
			{
				BREAK_POINT("call dispmovie failed");
			}
			LCD_Invalidate();
		}

	}
}
	


#endif