#ifndef _VECTOR_FONT_COMPRESS_H
#define _VECTOR_FONT_COMPRESS_H

typedef signed char			int8;
typedef signed short			int16;
//typedef signed long int			int32;
typedef unsigned char			uint8;
typedef unsigned short			uint16;
typedef unsigned long int		uint32;

/*---------------------------------------------------------*/
//Description: vector font compress interface
//	Global resource dependence: 
//  Author: Jialian.bai
//	Note:
//	input:
//			font_src_ptr -  input font data address
//			font_width - input font width
//          font_height - input font height
//	output:
//          font_des_ptr - output font data address
//          output_size - output font data size after compress
//	return: 
//			1 - compress success
//          0 - compress fail
/*----------------------------------------------------------*/

bool Font_Rle_Compress(unsigned char *font_src_ptr, unsigned char *font_des_ptr,int font_width, int font_height, int *output_size);

/*---------------------------------------------------------*/
//Description: vector font decompress interface
//	Global resource dependence: 
//  Author: Jialian.bai
//	Note:
//	input:
//			rle_font_ptr -  input compressed font data address
//			font_size -     input compressed font data size
//	output:
//          unrle_font_ptr - output decomressed font data address
//	return: 
/*----------------------------------------------------------*/

void Font_Rle_Decompress(uint8 *rle_font_ptr, uint8 *unrle_font_ptr, uint32 font_size,int font_width, int font_height);


void Font_Half_Sample(unsigned char *font_src_ptr, unsigned char *font_des_ptr, int font_width, int font_height,bool idxflag);

#endif