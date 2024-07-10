/*
*  amuse_jpeg_hw_decoder.h
*  Hardware Jpeg decoder interfaces for Amuse.
*
*  Copyright (C) 2011 Microrapid, Inc. All Rights Reserved.
*  Authors: Brad.
* 
*  This file is part of Amuse(TM).
*  Date: 2006-11-20
*  $Id$
*/

#ifndef AMUSE_JPEG_HW_DECODER_H
#define AMUSE_JPEG_HW_DECODER_H

#if MTK
#define JPEG_COLOR_FORMAT 2 // 16-bit, rgb 565		
#else
#define JPEG_COLOR_FORMAT 3 // 24-bit, B,G,R 	 8,8,8
#endif


/*
  * The interface of getting Jpeg dimension info (width & height) by hardware.
  * This interface should be implemented by every platform with hardware Jpeg Codec.
  * Brad, 2006-11-14
  */
int swf_jpeg_get_dimension_HW(unsigned char *jpeg_buf,unsigned int buf_size, int *width, int *height);

/*
  * The interface of decoding Jpeg data by hardware.
  * This interface should be implemented by every platform with hardware Jpeg Codec.
  * Brad, 2006-11-14
  */
int swf_jpeg_draw_to_buffer_HW(int width, int height,
                                         unsigned char *jpeg_src, unsigned int jpeg_src_size, 
                                         unsigned char *dest_buf, int dest_buf_size);

#endif // AMUSE_JPEG_HW_DECODER_H