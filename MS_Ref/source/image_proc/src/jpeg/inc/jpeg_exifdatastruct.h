/******************************************************************************
 ** File Name:	jpeg_exifdatastruct.h                           	  							*
 ** Author:		jianping.wang	                                          							*
 ** DATE:		03/17/2008                                              			 				*
 ** Copyright: 	2008 Spreatrum, Incoporated. All Rights Reserved.         				*
 ** Description:                                                              								*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       								*
 ** -------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                            						*
 ******************************************************************************/
#ifndef JPEG_EXIF_DATA_STRUCT_H_
#define JPEG_EXIF_DATA_STRUCT_H_

//#include "jpeg_exifTypedef.h"
#include "jpeg_exif.h"
#include "jpeg_interface.h"

//#define JPEG_EXIF_INFO_DETAIL

typedef struct _JPEG_DCDRV_DEF_PARSEREXIFPARAM
{
	JINF_EXIF_INFO_T exif_param;
	uint8		Get_Text_ImageDescriptionValue;
	uint8		Get_Text_MakeValue;
	uint8		Get_Text_ModelValue;
	uint8		Get_OrientationValue;
	uint8		Get_XResolutionValue;
	uint8		Get_YResolutionValue;
	uint8		Get_ResolutionUnitValue;
	uint8		Get_Text_DateTimeValue;
	uint8		Get_YCbCrPositionValue;
	uint8		Get_ExifIFDPointerValue;
	uint8		Get_ExposureTimeValue;
	uint8		Get_FNumberValue;
	uint8		Get_ExposureProgramValue;
	uint8		Get_Text_DateTimeOriginalValue;
	uint8		Get_Text_DateTimeDigitilizedValue;
	uint8		Get_CompressedBitsPerPixelValue;
	uint8		Get_ExposureBiasValue;
	uint8		Get_MaxApertureValue;
	uint8		Get_FocalLengthValue;
	uint8		Get_ExifVersionValue;
	uint8		Get_ISOSpeedRatingsValue;
	uint8		Get_ComponentsConfigurationValue;
	uint8		Get_MeteringModeValue;
	uint8		Get_LightSourceValue;
	uint8		Get_FlashValue;
	uint8		Get_FlashPixVersiontValue;
	uint8		Get_ColorSpaceValue;
	uint8		Get_PixelXDimensionValue;
	uint8		Get_PixelYDimensionValue;
	uint8		Get_InteroperabilityIFDPointerValue;
	uint8		Get_FileSourceValue;
	uint8		Get_SceneTypeValue;
	uint8		Get_CustomRenderedValue;
	uint8		Get_ExposureModeValue;
	uint8		Get_WhiteBalanceValue;
	uint8		Get_SceneCaptureTypeValue;
	uint8		Get_CompressionValue;
	uint8		Get_JPEGInterChangeFormatValue;
	uint8		Get_JPEGInterChangeFormatLengthValue;
	uint8		Get_InteroperabilityIndexValue;
	uint8		Get_InteroperabilityVersionValue;
	uint8		Get_GPSIFDOffsetValue;
	uint8		Get_Text_ArtistValue;
	uint8		Get_Text_UserCommentValue;
	uint8		Get_Text_CopyrightValue;
	uint8		Get_Text_SoftwareValue;	
} JPEG_DCDRV_DEF_PARSEREXIFPARAM;


typedef struct _JPEG_DCDRV_DEF_PARSEREXIFINFO
{

	uint8*	MainPtr;
	uint32	MainLen;
	uint8	MainFormat;
	uint16	MainWidth;
	uint16	MainHeight;
	
//	uint8 MainYQT[64];		// Y Q table
//	uint8 MainUVQT[64];	// UV Q talbe
	uint8  MainrstMCU_flag;
	uint16 MainrstMCU; 
	uint32 Mainheader_len;
		
//	uint8 MainYDCHcount[16];
//	uint8 MainYACHcount[16];
//	uint8 MainCDCHcount[16];
//	uint8 MainCACHcount[16];
	
//	uint8 MainYDCTable[0x100];	// Y DC talbe
//	uint8 MainYACTable[0x100];	// Y AC talbe
//	uint8 MainCDCTable[0x100];	// C DC talbe
//	uint8 MainCACTable[0x100];	// C AC talbe

	uint8 MainYQ_flag;
	uint8 MainUVQ_flag;
	uint8 MainYDC_flag;
	uint8 MainYAC_flag;
	uint8 MainCDC_flag;
	uint8 MainCAC_flag;

	uint8*	ThumbPtr;
	uint32	ThumbLen;
	uint8	ThumbFormat;
	uint16	ThumbWidth;
	uint16	ThumbHeight;
	

	//uint8 ThumbYQT[64];		// Y Q table
	//uint8 ThumbUVQT[64];	// UV Q talbe
	uint8  ThumbrstMCU_flag;
	uint16 ThumbrstMCU; 
	uint32 Thumbheader_len;
		
//	uint8 ThumbYDCHcount[16];
//	uint8 ThumbYACHcount[16];
//	uint8 ThumbCDCHcount[16];
//	uint8 ThumbCACHcount[16];
	
//	uint8 ThumbYDCTable[0x100];	// Y DC talbe
//	uint8 ThumbYACTable[0x100];	// Y AC talbe
//	uint8 ThumbCDCTable[0x100];	// C DC talbe
//	uint8 ThumbCACTable[0x100];	// C AC talbe

	uint8 ThumbYQ_flag;
	uint8 ThumbUVQ_flag;
	uint8 ThumbYDC_flag;
	uint8 ThumbYAC_flag;
	uint8 ThumbCDC_flag;
	uint8 ThumbCAC_flag;

	uint8* ImgBufferPtr;
	uint32	ImgBufferSize;	
#ifdef JPEG_EXIF_INFO_DETAIL
	JPEG_DCDRV_DEF_PARSEREXIFPARAM ExifParam;
#endif	

       uint8 *pThumbnail;
       uint32 ThumbLength;       
       uint8 *pMainImage;
       uint32 MainLength;	
} JPEG_DCDRV_DEF_PARSEREXIFINFO;



#endif