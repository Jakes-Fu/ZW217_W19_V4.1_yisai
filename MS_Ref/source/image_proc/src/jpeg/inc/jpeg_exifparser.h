/******************************************************************************
 ** File Name:	jpeg_exifparser.h                                  	  							*
 ** Author:		jianping.wang		                                          						*
 ** DATE:		03/17/2008                                              			 				*
 ** Copyright: 	2008 Spreatrum, Incoporated. All Rights Reserved.         				*
 ** Description:                                                              								*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       								*
 ** -------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                            						*
 ******************************************************************************/
#ifndef _JPEG_EXIF_PARSER_H_
#define _JPEG_EXIF_PARSER_H_

#include "jpeg_exifDataStruct.h"

typedef uint16 _STATUS;
#define _STATUS_SUCCESS 0

#define JPEG_EXIF_DQT_LEN_1		0x84
#define JPEG_EXIF_DQT_LEN_2		0xc5
#define JPEG_EXIF_DQT_LEN_3		0x43

#define JPEG_EXIF_DHT_YDC		0x00
#define JPEG_EXIF_DHT_YAC		0x10
#define JPEG_EXIF_DHT_CDC		0x01
#define JPEG_EXIF_DHT_CAC		0x11

#define JPEG_EXIF_SOS_LEN		0x0c

#define EXIF_LITTLE_ENDIAN_MARKER		0x49
#define EXIF_BIG_ENDIAN_MARKER			0x4D


enum JPEG_markerID{
	JPEG_SOF_MARKER = 0xc0,
	JPEG_SOF2_MARKER = 0xc2,
	JPEG_DHT_MARKER = 0xc4,
	
	JPEG_SOI_MARKER = 0xd8,
	JPEG_EOI_MARKER = 0xd9,
	JPEG_SOS_MARKER = 0xda,
	JPEG_DQT_MARKER = 0xdb,
	JPEG_DRI_MARKER = 0xdd,
		
	JPEG_APP0_MARKER = 0xe0,
	JPEG_APP1_MARKER = 0xe1,
	JPEG_APP2_MARKER = 0xe2,
	JPEG_APP3_MARKER = 0xe3,
	JPEG_APP4_MARKER = 0xe4,
	JPEG_APP5_MARKER = 0xe5,
	JPEG_APP6_MARKER = 0xe6,
	JPEG_APP7_MARKER = 0xe7,
	JPEG_APP8_MARKER = 0xe8,
	JPEG_APP9_MARKER = 0xe9,
	JPEG_APP10_MARKER = 0xea,
	JPEG_APP11_MARKER = 0xeb,
	JPEG_APP12_MARKER = 0xec,
	JPEG_APP13_MARKER = 0xed,
	JPEG_APP14_MARKER = 0xee,
	JPEG_APP15_MARKER = 0xef,
	
	JPEG_COM_MARKER = 0xfe,
	JPEG_PREFIX_MARKER = 0xff
};

enum JPEG_errid
{
	JPEG_LIBEXIF_MISC_ERR0001=0x01,
	JPEG_LIBEXIF_SOI_ERR0001,
	JPEG_LIBEXIF_DQT_ERR0002,
	JPEG_LIBEXIF_DQT_ERR0003,
	JPEG_LIBEXIF_DHT_ERR0006,
	JPEG_LIBEXIF_DHT_ERR0002,
	JPEG_LIBEXIF_DHT_ERR0001,
	JPEG_LIBEXIF_SOF_ERR0003,
	JPEG_LIBEXIF_SOF_ERR0001,
	JPEG_LIBEXIF_SOS_ERR0001,
	JPEG_LIBEXIF_SOS_ERR0002,
	JPEG_LIBEXIF_NO_THUMBNAIL
};




extern int JPEG_JP_ExifDecodeInit(
	JPEG_DCDRV_DEF_PARSEREXIFINFO * pExifPaser
	);

extern int JPEG_API_ExifPaser(
	JPEG_DCDRV_DEF_PARSEREXIFINFO* pExifPaser
	);



extern uint32 JPEG_PaserExifInfo(	uint8 * pBuf,   //pBuf: Image buffer address
							uint32 imgSize,  //imgSize: Image Size
							JINF_EXIF_INFO_T *exif_info_ptr,
							JINF_THUMBNAIL_INFO_T   *thumbnail_info_ptr
						 );



enum JPEG_ExifParamID{

JPEG_IMAGEDESCRIPTION = 0x10e,
JPEG_MAKE = 0x10f,
JPEG_MODEL = 0x110,
JPEG_ORIENTATION = 0x112,
JPEG_XRESOLUTION = 0x11A,
JPEG_YRESOLUTION = 0x11B,
JPEG_RESOLUTIONUNIT = 0x128,
JPEG_DATETIME = 0x132,
JPEG_YCBCRPOSITION = 0x213,
JPEG_EXIFIFDPOINTER = 0x8769,
JPEG_EXPOSURETIME = 0x829a,
JPEG_FNUMBER = 0x829D,
JPEG_EXPOSUREPROGRAM = 0x8822,
JPEG_DATETIMEORIGINAL = 0x9003,
JPEG_DATETIMEDIGITILIZED = 0x9004,
JPEG_COMPRESSEDBITSPERPIXEL = 0x9102,
JPEG_EXPOSUREBIAS = 0x9204,
JPEG_MAXAPERTURE = 0x9205,
JPEG_FOCALLENGTH = 0x920A,
JPEG_EXIFVERSION = 0x9000,
JPEG_ISOSPEEDRATINGS = 0x8827,
JPEG_COMPONENTSCONFIGURATION = 0x9101,
JPEG_METERINGMODE = 0x9207,
JPEG_LIGHTSOURCE = 0x9208,
JPEG_FLASH = 0x9209,
JPEG_FLASHPIXVERSIONT = 0xa000,
JPEG_COLORSPACE = 0xa001,
JPEG_PIXELXDIMENSION = 0xa002,
JPEG_PIXELYDIMENSION = 0xa003,
JPEG_INTEROPERABILITYIFDPOINTER = 0xa005,
JPEG_FILESOURCE = 0xa300,
JPEG_SCENETYPE = 0xa301,
JPEG_CUSTOMRENDERED = 0xa401,
JPEG_EXPOSUREMODE = 0xa402,
JPEG_WHITEBALANCE = 0xa403,
JPEG_SCENECAPTURETYPE = 0xa406,
JPEG_INTEROPERABILITYINDEX = 0x0001,
JPEG_INTEROPERABILITYVERSION = 0x0002,
JPEG_COMPRESSION = 0x103,
JPEG_JPEGINTERCHANGEFORMAT = 0x0201,
JPEG_JPEGINTERCHANGEFORMATLENGTH = 0x0202,
JPEG_GPSIFDOFFSET = 0x8825,
JPEG_ARTIST = 0x13b,
JPEG_USERCOMMENT = 0x9286,
JPEG_COPYRIGHT = 0x8298,
JPEG_SOFTWARE = 0x131
};


#endif