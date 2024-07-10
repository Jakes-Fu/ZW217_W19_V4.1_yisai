/******************************************************************************
 ** File Name:      BMP.h                                                     *
 ** Author:         Zhemin.Lin                                                *
 ** DATE:           2004/05/09                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define BMP operation interface                            *
 ** Note:           None                                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/05/09     Zhemin.Lin       Create.                                   *
 ******************************************************************************/
 
#ifndef BMP_H
#define BMP_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h"

/*
 *  definition related to the BMP file structure
 */
#define BF_TYPE         0x4D42      /* "MB" */
#define MV_TYPE			0x4D56		/* "MV" */
#define CK_TYPE         0x4B43      /* "CK" */

#define PALETTE_SIZE    256         /* color palette size*/
#define MAX_FRAME_NUM   99          /* max frame num */

#define BI_RGB_S          0           /* No compression - straight BGR data */
#define BI_RLE8_S         1           /* 8-bit run-length compression */
#define BI_RLE4_S         2           /* 4-bit run-length compression */
#define BI_BITFIELDS_S    3           /* RGB bitmap with RGB masks */
#define BI_PALETTECOMP_S  4          /* palette compression */
#define BI_MY_RLE       0x1000      /* our run-length*/

#ifdef WIN32
    #pragma pack(1)
    typedef struct                /**** BMP file header structure ****/
    {
        uint16  bfType;           /* Magic number for file */
        uint32  bfSize;           /* Size of file */
//        uint16  bfReserved1;      /* Reserved */
//        uint16  bfReserved2;      /* ... */
		uint32 same_color888;
        uint32  bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER_T;


    typedef struct                /**** BMP file info structure ****/
    {
        uint32  biSize;           /* Size of info header */
        uint32  biWidth;          /* Width of image */
        uint32  biHeight;         /* Height of image */
        uint16  biPlanes;         /* Number of color planes */
        uint16  biBitCount;       /* Number of bits per pixel */
        uint32  biCompression;    /* Type of compression to use */
        uint32  biSizeImage;      /* Size of image data */
        uint32  biXPelsPerMeter;  /* X pixels per meter */
        uint32  biYPelsPerMeter;  /* Y pixels per meter */
        uint32  biClrUsed;        /* Number of colors used */
        uint32  biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER_T;

    typedef struct               /**** Colormap entry structure ****/
    {
        uint8  rgbBlue;          /* Blue value */
        uint8  rgbGreen;         /* Green value */
        uint8  rgbRed;           /* Red value */
        uint8  rgbReserved;      /* Reserved */
    } RGBQUAD_T;

    typedef struct                       /**** Bitmap information structure ****/
    {
        BITMAPINFOHEADER_T bmiHeader;      /* Image header */
        RGBQUAD_T          bmiColors[PALETTE_SIZE]; /* Image colormap */
    } BITMAPINFO_T;
    
	#define IMG_CODE_SIZE_MASK 0x7FFFFFFF
    typedef struct     /*****  Frame infomation structure  ****/
    {
        uint32 code_size;       /*code size */ //Zhemin.Lin, the MSB indicate if the code stream isn't commpressed
        									   //if bit[31] == 0, it's run length encoding, else not encoding
        uint32 code_offset;     /*code start address*/
    }FRAMEINFO;
    typedef  struct                             /*****  Movie infomation structure  ****/
    {
        uint32 frame_num;                       /* frame number */
        FRAMEINFO frame_infos[MAX_FRAME_NUM];   /* frame infomations*/
    }MOVIEINFO;

    
    #pragma pack()
#else

    typedef __packed struct       /**** BMP file header structure ****/
    {
        uint16  bfType;           /* Magic number for file */
        uint32  bfSize;           /* Size of file */
//        uint16  bfReserved1;      /* Reserved */
//        uint16  bfReserved2;      /* ... */
		uint32 same_color888;
        uint32  bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER_T;


    typedef __packed struct       /**** BMP file info structure ****/
    {
        uint32  biSize;           /* Size of info header */
        uint32  biWidth;          /* Width of image */
        uint32  biHeight;         /* Height of image */
        uint16  biPlanes;         /* Number of color planes */
        uint16  biBitCount;       /* Number of bits per pixel */
        uint32  biCompression;    /* Type of compression to use */
        uint32  biSizeImage;      /* Size of image data */
        uint32  biXPelsPerMeter;  /* X pixels per meter */
        uint32  biYPelsPerMeter;  /* Y pixels per meter */
        uint32  biClrUsed;        /* Number of colors used */
        uint32  biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER_T;

    typedef __packed struct              /**** Colormap entry structure ****/
    {
        uint8  rgbBlue;          /* Blue value */
        uint8  rgbGreen;         /* Green value */
        uint8  rgbRed;           /* Red value */
        uint8  rgbReserved;      /* Reserved */
    } RGBQUAD_T;

    typedef __packed struct                         /**** Bitmap information structure ****/
    {
        BITMAPINFOHEADER_T bmiHeader;                 /* Image header */
        RGBQUAD_T          bmiColors[PALETTE_SIZE];   /* Image colormap */
    } BITMAPINFO_T;

	#define IMG_CODE_SIZE_MASK 0x7FFFFFFF
    typedef __packed struct     /*****  Frame infomation structure  ****/
    {
        uint32 code_size;       /*code size */ //Zhemin.Lin, the MSB indicate if the code stream isn't commpressed
        									   //if bit[31] == 0, it's run length encoding, else not encoding
        uint32 code_offset;     /*code start address*/
    }FRAMEINFO;
    typedef __packed struct                     /*****  Movie infomation structure  ****/
    {
        uint32 frame_num;                       /* frame number */
        FRAMEINFO frame_infos[MAX_FRAME_NUM];   /* frame infomations*/
    }MOVIEINFO;
#endif //WIN32

typedef struct
{
    uint8     *moviestream;
    IMG_RECT_T       *framerect;

}BMP_MOV_HANDLER_T;


/*
 *  definition related to the BMP run length codec
 */

#define MIN_RUN_LEN     3

#define BMPHEAD_ENDIAN_EXCHANGE(head) \
	(head)->bfType = UINT16_LE((head)->bfType); \
	(head)->bfSize = UINT32_LE((head)->bfSize); \
	(head)->same_color888 = UINT32_LE((head)->same_color888); \
	(head)->bfOffBits = UINT32_LE((head)->bfOffBits);

	
#define BMPINFO_ENDIAN_EXCHANGE(info) \
	(info)->biSize = UINT32_LE((info)->biSize); \
	(info)->biWidth = UINT32_LE((info)->biWidth); \
	(info)->biHeight = UINT32_LE((info)->biHeight); \
	(info)->biPlanes = UINT16_LE((info)->biPlanes); \
	(info)->biBitCount = UINT16_LE((info)->biBitCount); \
	(info)->biCompression = UINT32_LE((info)->biCompression); \
	(info)->biSizeImage = UINT32_LE((info)->biSizeImage); \
	(info)->biXPelsPerMeter = UINT32_LE((info)->biXPelsPerMeter); \
	(info)->biYPelsPerMeter = UINT32_LE((info)->biYPelsPerMeter); \
	(info)->biClrUsed = UINT32_LE((info)->biClrUsed); \
	(info)->biClrImportant = UINT32_LE((info)->biClrImportant);

#define MOVIEINFO_ENDIAN_EXCHANGE(pmovieinfo, framenum) \
	{\
		uint32 i;\
		pmovieinfo->frame_num = UINT32_LE((pmovieinfo)->frame_num);\
		for(i=0; i<(framenum); i++)\
		{\
			(pmovieinfo)->frame_infos[i].code_offset = UINT32_LE((pmovieinfo)->frame_infos[i].code_offset);\
			(pmovieinfo)->frame_infos[i].code_size = UINT32_LE((pmovieinfo)->frame_infos[i].code_size);\
		}\
	}

/*****************************************************************************/
//  Description :
//      load img stream from file system.
//  Author:
//      Zhemin.Lin
//  Input:
//		filename		file name
//		*bmpfile_stream	stream of the image
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 

uint32 IMG_Load(
	const char *filename,
	uint8 **bmpfile_stream
	);

/*****************************************************************************/
//  Description :
//      save img stream to file system.
//  Author:
//      Zhemin.Lin
//  Input:
//		filename		file name
//		*bmpfile_stream	stream of the image
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 

uint32 IMG_Save(
	const char *filename,
	uint8 *bmpfile_stream
	);

/*****************************************************************************/
//  Description :
//      check frames' validity of the specified movie.
//		all the frames are save as a bmp file.
//		if the framenum>9, bmp files are named by 01.bmp, 02.bmp..
//		else named by 1.bmp, 2.bmp ..
//  Author:
//      Zhemin.Lin
//  Input:
//		dirname		directory name, where the frames are stored
//		framenum	frame number, >1
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 

uint32 MOVIE_FramesIsInvalid(
	const char *dirname,
	uint32 framenum
	);

/*****************************************************************************/
//  Description :
//      get the file name of the specified frame of a movie
//		if the framenum>9, bmp files are named by 01.bmp, 02.bmp..
//		else named by 1.bmp, 2.bmp ..
//  Author:
//      Zhemin.Lin
//  Input:
//		filename	input: dirname, output: filename
//		framenum	frame number, >1
//		frameid		frame ID, 0...,framenum-1
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 

void MOVIE_GetFileName(
	char 		*filename,
	uint32		framenum,
	uint32		frameid
	);

/*****************************************************************************/
//  Description :
//      load a frame from file system
//  Author:
//      Zhemin.Lin
//  Input:
//		dirname			directory name
//		framenum		frame number, >1
//		frameid			frame ID, 0...,framenum-1
//		*bmpfile_stream	img stream
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 

uint32 MOVIE_LoadFrame(
	const char *dirname,
	uint32 framenum,
	uint32 frameid,
	uint8 **bmpfile_stream
	);

/*****************************************************************************/
//  Description :
//      do run length coding
//  Author:
//      Zhemin.Lin
//  Input:
//		infoheader	infomation header of a bmp
//		prawdata	point to raw data of the bmp
//		pcodedata	point to code
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 

uint32 BMP_RunLengthEncode(
	BITMAPINFOHEADER_T	*infoheader,
	const uint8				*prawdata,
	uint8				*pcodedata
	);

/*****************************************************************************/
//  Description :
//      do run length decoding
//  Author:
//      Zhemin.Lin
//  Input:
//		infoheader	infomation header of a bmp
//		prawdata	point to raw data of the bmp
//		pcodedata	point to code
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 

uint32 BMP_RunLengthDecode(
	BITMAPINFOHEADER_T	*infoheader,
	uint8				*prawdata,
	const uint8			*pcodedata
	);

void BMP_ColorBitConvert(
						 uint32 color32,		/* [in] 888 bit*/
						 uint16 *color16_p,		/* [out] 565 bit*/
						 uint8  *color8_p		/* [out] 332 bit*/
						 );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif //BMP_H 
