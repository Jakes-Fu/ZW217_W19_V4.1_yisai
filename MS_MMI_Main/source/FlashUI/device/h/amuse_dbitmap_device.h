/*
*  amuse_dbitmap_device.h -- SWF bitmap decode by device
*  Copyright (C) 2011 Microrapid, Inc. All Rights Reserved.
*  Authors: Microrapid AMUSE GROUP
*  This file is part of Microrapid(TM).
*  Date:
*  $Id$
*/

#ifndef AMUSE_DBITMAP_DEVICE_H
#define AMUSE_DBITMAP_DEVICE_H


struct bitmapInfo{
	int pixelFormat; /*given by amuse,FMT_ABGR = 1,Packed ABGR 8:8:8:8, 32bpp, ABGRABGR...; FMT_RGB565 = 4, Packed RGB 5:6:5, 16bpp //bpp==2 use*/
	unsigned char* fName; /*given by amuse*/
	unsigned char* srcBuf;/*given by amuse*/
	int srcBufSize;/*given by amuse */

	int width;/*set by device */
	int height;/*set by device*/
	int hasalpha;/*set by device*/
	unsigned char* rgbBuf;/*set by device,decode image file,malloc and free by device; decode jpeg data,malloc and free by amuse.*/
	int rgbBufSize;/*decode image file,set by device; decode jpeg data,set by amuse.*/

	void *extdata;/*for decoder use*/
};
typedef struct bitmapInfo bitmapInfo;

/*
* This function decode the loading image file by device.
* Need to open the configure parameter 'use_hardware_decode_image'
* in amuse_config_device.c first to make this function available.
*
* Parameters: 
* binfo: the input and output info need by decoder
*
* Returns: success: 0; failure: -1
*/
int amuse_decode_image_file_device(bitmapInfo * binfo);

/*
* This function get the jpeg dimension from jpeg data
*
* Parameters: 
* binfo: the input and output info need by decoder
*
* Returns: success: 0; failure: -1
*/
int amuse_get_jpeg_dimension_device(bitmapInfo * binfo);

/*
* This function decode the JPGE data by device.
* Need to open the configure parameter 'use_hardware_decode_jpeg' 
* in amuse_config_device.c first to make this function available.
*
* Parameters: 
* binfo: the input and output info need by decoder
*
* Returns: success: 0; failure: -1
*/
int amuse_decode_jpeg_data_device(bitmapInfo * binfo);


#endif
