/******************************************************************************
 ** File Name:		jpeg_exif.h                                               *
 ** Author:			Frank.Yang		                                          *
 ** DATE:			03/25/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:	Refer to "Exchangeable image file formator digital		  *
 					still cameras: Exif Version 2.2" for more details.        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ******************************************************************************/


#ifndef _JPEG_EXIF_H_
#define _JPEG_EXIF_H_

#if 0
#include "sci_types.h"

#define TEXT_DATATIME_LEN 		((uint32) 0x000000014)
#define TEXT_IMGDESCRIPTION_LEN ((uint32) 0x000000020)
#define TEXT_MAKE_LEN 			((uint32) 0x000000020)
#define TEXT_MODEL_LEN 			((uint32) 0x000000020)
#define TEXT_SOFTWARE_LEN		((uint32) 0x000000020)
#define TEXT_ARTIST_LEN			((uint32) 0x000000020)



typedef enum
{
	EXIF_TYPE_BYTE		= 0x01,		   /*An 8-bit unsigned integer.*/
	EXIF_TYPE_ASCII		= 0x02,		   /*An 8-bit byte containing one 7-bit ASCII code.
									   		The final byte is terminated with NULL.*/
	EXIF_TYPE_SHORT		= 0x03,		   /*A 16-bit (2-byte) unsigned integer.*/
	EXIF_TYPE_LONG		= 0x04,		   /*A 32-bit (4-byte) unsigned integer.*/
	EXIF_TYPE_RATIONAL	= 0x05,		   /*Two LONGs. The first LONG is the numerator and the second LONG 
											expresses the denominator.*/
	EXIF_TYPE_SBYTE		= 0x06,		   
	EXIF_TYPE_UNDEFINED = 0x07,		   /*An 8-bit byte that can take any value depending on 
											the field definition.*/
	EXIF_TYPE_SSHORT	= 0x08,		   
	EXIF_TYPE_SLONG		= 0x09,		   /*A 32-bit (4-byte) signed integer (2's complement notation).*/
	EXIF_TYPE_SRATIONAL = 0x0a,		   /*Two SLONGs. The first SLONG is the numerator and the second SLONG
											is the denominator.*/
	EXIF_TYPE_MAX		
}EXIF_TYPE_E;


typedef enum 
{
	EXIF_TAG_INTEROPERABILITY_INDEX			= 0x0001,
	EXIF_TAG_INTEROPERABILITY_VERSION		= 0x0002,
	EXIF_TAG_IMAGE_WIDTH 					= 0x0100,
	EXIF_TAG_IMAGE_LENGTH 					= 0x0101,
	EXIF_TAG_BITS_PER_SAMPLE 				= 0x0102,
	EXIF_TAG_COMPRESSION 					= 0x0103,
	EXIF_TAG_PHOTOMETRIC_INTERPRETATION 	= 0x0106,
	EXIF_TAG_FILL_ORDER 					= 0x010a,
	EXIF_TAG_DOCUMENT_NAME 					= 0x010d,
	EXIF_TAG_IMAGE_DESCRIPTION 				= 0x010e,
	EXIF_TAG_MAKE 							= 0x010f,
	EXIF_TAG_MODEL 							= 0x0110,
	EXIF_TAG_STRIP_OFFSETS 					= 0x0111,
	EXIF_TAG_ORIENTATION 					= 0x0112,
	EXIF_TAG_SAMPLES_PER_PIXEL 				= 0x0115,
	EXIF_TAG_ROWS_PER_STRIP 				= 0x0116,
	EXIF_TAG_STRIP_BYTE_COUNTS				= 0x0117,
	EXIF_TAG_X_RESOLUTION 					= 0x011a,
	EXIF_TAG_Y_RESOLUTION 					= 0x011b,
	EXIF_TAG_PLANAR_CONFIGURATION 			= 0x011c,
	EXIF_TAG_RESOLUTION_UNIT 				= 0x0128,
	EXIF_TAG_TRANSFER_FUNCTION 				= 0x012d,
	EXIF_TAG_SOFTWARE 						= 0x0131,
	EXIF_TAG_DATE_TIME						= 0x0132,
	EXIF_TAG_ARTIST							= 0x013b,
	EXIF_TAG_WHITE_POINT					= 0x013e,
	EXIF_TAG_PRIMARY_CHROMATICITIES			= 0x013f,
	EXIF_TAG_TRANSFER_RANGE					= 0x0156,
	EXIF_TAG_JPEG_PROC						= 0x0200,
	EXIF_TAG_JPEG_INTERCHANGE_FORMAT		= 0x0201,
	EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH	= 0x0202,
	EXIF_TAG_YCBCR_COEFFICIENTS				= 0x0211,
	EXIF_TAG_YCBCR_SUB_SAMPLING				= 0x0212,
	EXIF_TAG_YCBCR_POSITIONING				= 0x0213,
	EXIF_TAG_REFERENCE_BLACK_WHITE			= 0x0214,
	EXIF_TAG_RELATED_IMAGE_FILE_FORMAT		= 0x1000,
	EXIF_TAG_RELATED_IMAGE_WIDTH			= 0x1001,
	EXIF_TAG_RELATED_IMAGE_LENGTH			= 0x1002,
	EXIF_TAG_CFA_REPEAT_PATTERN_DIM			= 0x828d,
	EXIF_TAG_CFA_PATTERN					= 0x828e,
	EXIF_TAG_BATTERY_LEVEL					= 0x828f,
	EXIF_TAG_COPYRIGHT						= 0x8298,
	EXIF_TAG_EXPOSURE_TIME					= 0x829a,
	EXIF_TAG_FNUMBER						= 0x829d,
	EXIF_TAG_IPTC_NAA						= 0x83bb,
	EXIF_TAG_EXIF_IFD_POINTER				= 0x8769,
	EXIF_TAG_INTER_COLOR_PROFILE			= 0x8773,
	EXIF_TAG_EXPOSURE_PROGRAM				= 0x8822,
	EXIF_TAG_SPECTRAL_SENSITIVITY			= 0x8824,
	EXIF_TAG_GPS_INFO_IFD_POINTER			= 0x8825,
	EXIF_TAG_ISO_SPEED_RATINGS				= 0x8827,
	EXIF_TAG_OECF							= 0x8828,
	EXIF_TAG_EXIF_VERSION					= 0x9000,
	EXIF_TAG_DATE_TIME_ORIGINAL				= 0x9003,
	EXIF_TAG_DATE_TIME_DIGITIZED			= 0x9004,
	EXIF_TAG_COMPONENTS_CONFIGURATION		= 0x9101,
	EXIF_TAG_COMPRESSED_BITS_PER_PIXEL		= 0x9102,
	EXIF_TAG_SHUTTER_SPEED_VALUE			= 0x9201,
	EXIF_TAG_APERTURE_VALUE					= 0x9202,
	EXIF_TAG_BRIGHTNESS_VALUE				= 0x9203,
	EXIF_TAG_EXPOSURE_BIAS_VALUE			= 0x9204,
	EXIF_TAG_MAX_APERTURE_VALUE				= 0x9205,
	EXIF_TAG_SUBJECT_DISTANCE				= 0x9206,
	EXIF_TAG_METERING_MODE					= 0x9207,
	EXIF_TAG_LIGHT_SOURCE					= 0x9208,
	EXIF_TAG_FLASH							= 0x9209,
	EXIF_TAG_FOCAL_LENGTH					= 0x920a,
	EXIF_TAG_SUBJECT_AREA					= 0x9214,
	EXIF_TAG_MAKER_NOTE						= 0x927c,
	EXIF_TAG_USER_COMMENT					= 0x9286,
	EXIF_TAG_SUBSEC_TIME					= 0x9290,
	EXIF_TAG_SUB_SEC_TIME_ORIGINAL			= 0x9291,
	EXIF_TAG_SUB_SEC_TIME_DIGITIZED			= 0x9292,
	EXIF_TAG_FLASH_PIX_VERSION				= 0xa000,
	EXIF_TAG_COLOR_SPACE					= 0xa001,
	EXIF_TAG_PIXEL_X_DIMENSION				= 0xa002,
	EXIF_TAG_PIXEL_Y_DIMENSION				= 0xa003,
	EXIF_TAG_RELATED_SOUND_FILE				= 0xa004,
	EXIF_TAG_INTEROPERABILITY_IFD_POINTER	= 0xa005,
	EXIF_TAG_FLASH_ENERGY					= 0xa20b,
	EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE		= 0xa20c,
	EXIF_TAG_FOCAL_PLANE_X_RESOLUTION		= 0xa20e,
	EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION		= 0xa20f,
	EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT	= 0xa210,
	EXIF_TAG_SUBJECT_LOCATION				= 0xa214,
	EXIF_TAG_EXPOSURE_INDEX					= 0xa215,
	EXIF_TAG_SENSING_METHOD					= 0xa217,
	EXIF_TAG_FILE_SOURCE					= 0xa300,
	EXIF_TAG_SCENE_TYPE						= 0xa301,
	EXIF_TAG_NEW_CFA_PATTERN				= 0xa302,
	EXIF_TAG_CUSTOM_RENDERED				= 0xa401,
	EXIF_TAG_EXPOSURE_MODE					= 0xa402,
	EXIF_TAG_WHITE_BALANCE					= 0xa403,
	EXIF_TAG_DIGITAL_ZOOM_RATIO				= 0xa404,
	EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM		= 0xa405,
	EXIF_TAG_SCENE_CAPTURE_TYPE				= 0xa406,
	EXIF_TAG_GAIN_CONTROL					= 0xa407,
	EXIF_TAG_CONTRAST						= 0xa408,
	EXIF_TAG_SATURATION						= 0xa409,
	EXIF_TAG_SHARPNESS						= 0xa40a,
	EXIF_TAG_DEVICE_SETTING_DESCRIPTION		= 0xa40b,
	EXIF_TAG_SUBJECT_DISTANCE_RANGE			= 0xa40c,
	EXIF_TAG_IMAGE_UNIQUE_ID				= 0xa420
}EXIF_TAG_E;


typedef struct
{
	uint32	numerator;
	uint32	denominator;
}RATIONAL;

typedef struct
{
	int32	numerator;
	int32	denominator;
}SRATIONAL;

/*InterOperability IFD*/
typedef struct
{
	/* public informaton */
	uint8		InterOperabilityIndex[4];
	uint8		ExifR98Version[4];

	/* private informaton */
	uint16		EntryCount;
	uint16		padding;
	uint32		NextOffset;		  	
}IFD_IOB_T;


/*Exif IFD*/
typedef struct
{
	/* public informaton */
	IFD_IOB_T	IOB;	  		/*InterOperability IFD.*/	   
	RATIONAL	ExposureTime;   /*Exposure time, given in seconds (sec)*/
	RATIONAL	FNumber;		/*The F number*/

	uint16		ExposureProgram;
	/*Default = 0 
	0 = Not defined 
	1 = Manual 
	2 = Normal program 
	3 = Aperture priority 
	4 = Shutter priority 
	5  =  Creative program (biased toward depth of field) 
	6  =  Action program (biased toward fast shutter speed) 
	7  =  Portrait mode (for closeup photos with the background out of focus)   
	8  =  Landscape mode (for landscape photos with the background in focus) 
	Other = reserved */
	
	uint16		ISOSpeedRatings;	/*Indicates the ISO Speed and ISO Latitude of the camera 
										or input device as specified in ISO 12232*/
	uint8		ExifVersion[4]; 
	uint8		DataTimeOriginal[TEXT_DATATIME_LEN];   /*YYYY:MM:DD HH:MM:SS*/
	uint8		DataTimeDigitized[TEXT_DATATIME_LEN];  /*YYYY:MM:DD HH:MM:SS*/

	uint8		ComponentsConfiguration[4];
	/*Default  =	4 5 6 0 (if RGB uncompressed) 
				    1 2 3 0 (other cases) 
	0  =  does not exist 
	1 = Y 
	2 = Cb 
	3 = Cr 
	4 = R 
	5 = G 
	6 = B 
	Other = reserved */

	RATIONAL	CompressedBitsPerPixel; 
	SRATIONAL	ExposureBiasValue;	/*The exposure bias. The unit is the APEX value. 
										Ordinarily it is given in the range of –99.99 to 99.99. */
	RATIONAL	MaxApetureValue;	/*The smallest F number of the lens. The unit is the APEX value. 
										Ordinarily it is given in the range of 00.00 to 99.99,
										but it is not limited to this range. */
	uint16		MeteringMode;
	/*Default = 0 
	0 = unknown 
	1 = Average 
	2 = CenterWeighte
	3 = Spot 
	4 = MultiSpot 
	5 = Pattern 
	6 = Partial 
	Other = reserved 
	255 = other*/

	uint16		LightSource;
	/*Default = 0 
	1	= Daylight 
	2	= Fluorescent 
	3	=  Tungsten (incandescent light) 
	4	= Flash 
	9	= Fine weather 
	10	= Cloudy weather 
	11	= Shade 
	12  =  Daylight fluorescent (D 5700 – 7100
	13  =  Day white fluorescent (N 4600 – 540
	14  =  Cool white fluorescent (W 3900 – 45
	15  =  White fluorescent (WW 3200 – 3700
	17  =  Standard light A 
	18  =  Standard light B 
	19  =  Standard light C 
	20	= D55 
	21	= D65 
	22	= D75 
	23	= D50 
	24 	=  ISO studio tungsten 
	255	=  other light source 
	Other = reserved */

	uint16		Flash;
	/*
	bit[0]:
	0b = Flash did not fire. 
	1b = Flash fired. 
	bot[2~1]:
	0b = Flash did not fire. 
	1b = Flash fired. 
	bit[4~3]:
	00b  =  unknown 
	01b  =  Compulsory flash firing 
	10b  =  Compulsory flash suppression 
	11b  =  Auto mode 
	bit[5]:
	0b  =  Flash function present 
	1b  =  No flash function  
	bit[6]:
	0b  =    No red-eye reduction mode or unknown 
	1b  =  Red-eye reduction supported

	Resulting Flash tag values.   
	0000.H  =	Flash did not fire. 
	0001.H  =	Flash fired. 
	0005.H  =	Strobe return light not detected. 
	0007.H  =	Strobe return light detected. 
	0009.H  =	Flash fired, compulsory flash mode 
	000D.H  =	Flash fired, compulsory flash mode, return light not detected 
	000F.H  =   Flash fired, compulsory flash mode, return light detected 
	0010.H  =	Flash did not fire, compulsory flash mode 
	0018.H  =	Flash did not fire, auto mode 
	0019.H  =   Flash fired, auto mode 
	001D.H  =   Flash fired, auto mode, return light not detected 
	001F.H  =	Flash fired, auto mode, return light detected 
	0020.H  =   No flash function  
	0041.H  =	Flash fired, red-eye reduction mode 
	0045.H  =	Flash fired, red-eye reduction mode, return light not detected 
	0047.H  =	Flash fired, red-eye reduction mode, return light detected 
	0049.H  =	Flash fired, compulsory flash mode, red-eye reduction mode 
	004D.H  =	Flash fired, compulsory flash mode, red-eye reduction mode, return light not detected 
	004F.H  =	Flash fired, compulsory flash mode, red-eye reduction mode, return light detected 
	0059.H  =	Flash fired, auto mode, red-eye reduction mode 
	005D.H  =	Flash fired, auto mode, return light not detected, red-eye reduction mode 
	005F.H  =	Flash fired, auto mode, return light detected, red-eye reduction mode 
	Other	=	reserved*/

	uint16		ColorSpace;		/* 1= sRGB, 0xFFFF = Uncalibrated, Other = reserved*/
	RATIONAL	FocalLength;    /*The actual focal length of the lens, in mm.
									Conversion is not made to the focal length of a 35 mm film camera. */

	uint8		FlashPixVersion[4];	  /*Default = "0100",0100 = Flashpix Format Version 1.0, other = reserved*/
	uint16		PixelXDimension;
	uint16		PixelYDimension;
	uint8		FileSource;	/* Default=3, 3=DSC, Other=reserved. */
	uint8		SceneType;	/* Default=1, 1=A directly photographed image, Other=reserved */

	uint16		CustomRendered;	   
	/*This tag indicates the use of special processing on image data, 
	such as rendering geared to output. 
	Default  =  0 
	0  =  Normal process 
	1  =  Custom process 
	Other  =  reserved*/

	RATIONAL	DigitalZoomRatio; /*This tag indicates the digital zoom ratio when the image was shot. 
									If the numerator of the recorded value is 0, 
									this indicates that digital zoom was not used. */
	RATIONAL	GainControl;
 	/*This tag indicates the degree of overall image gain adjustment. 
	Default  =  none 
	0  =  None 
	1  =  Low gain up 
	2  =  High gain up 
	3  =  Low gain down 
	4  =  High gain down 
	Other  =  reserved*/

	uint16		ExposureMode;
	/*This tag indicates the exposure mode set when the image was shot. 
	Default  =  none 
	0  =  Auto exposure 
	1  =  Manual exposure 
	2  =  Auto bracket 
	Other  =  reserved*/
 
	uint16		WhiteBalance;
	/*This tag indicates the white balance mode set when the image was shot. 
	Default  =  none 
	0  =  Auto white balance 
	1  =  Manual white balance 
	Other  =  reserved*/

	uint16		SceneCaptureType;
	/*This tag indicates the type of scene that was shot. 
	Default  =  0 
	0  =  Standard 
	1  =  Landscape 
	2  =  Portrait 
	3  =  Night scene 
	Other  =  reserved*/

	uint16		Contrast;
	/*This tag indicates the direction of contrast processing applied by the camera when the image was shot. 
	Default  =  0 
	0  =  Normal 
	1  =  Soft 
	2  =  Hard 
	Other  =  reserved*/

	uint16		Saturation;
	/*This tag indicates the direction of saturation processing applied by the camera when the image was shot. 
	Default  =  0 
	0  =  Normal 
	1  =  Low saturation 
	2  =  High saturation 
	Other  =  reserved*/

	uint16		Sharpness;
 	/*This tag indicates the direction of sharpness processing applied by the camera when the image was shot. 
	Default  =  0 
	0  =  Normal 
	1  =  Soft 
	2  =  Hard 
	Other  =  reserved*/

	/* private informaton */
	uint16		EntryCount;
	uint32		InterOperabilityIFDPointer;
	uint32		NextOffset;	   
}IFD_EXIF_T;


/*0th IFD*/
typedef struct
{
	/* public informaton */
	IFD_EXIF_T	exif;  			//Exif IFD
	uint8		ImageDescription[TEXT_IMGDESCRIPTION_LEN]; /*A character string giving the title of the image.*/
	uint8		Make[TEXT_MAKE_LEN]; /*The manufacturer of the recording equipment,Default = none*/
	uint8		Model[TEXT_MODEL_LEN];	/*The model name or model number of the equipment,Default = none*/
	uint8		Software[TEXT_SOFTWARE_LEN];
	uint8		Artist[TEXT_ARTIST_LEN];

	uint16		Orientation;
	/*	Default = 1 
	1  =  The 0th row is at the visual top of the image, and the 0th column is the visual left-hand side. 
	2  =  The 0th row is at the visual top of the image, and the 0th column is the visual right-hand side. 
	3  =  The 0th row is at the visual bottom of the image, and the 0th column is the visual right-hand side. 
	4  =  The 0th row is at the visual bottom of the image, and the 0th column is the visual left-hand side. 
	5  =  The 0th row is the visual left-hand side of the image, and the 0th column is the visual top. 
	6  =  The 0th row is the visual right-hand side of the image, and the 0th column is the visual top. 
	7  =  The 0th row is the visual right-hand side of the image, and the 0th column is the visual bottom. 
	8  =  The 0th row is the visual left-hand side of the image, and the 0th column is the visual bottom. 
	Other = reserved*/

	uint16		ResolutionUnit;	/*Default = 2,	2 = inches,	3 = centimeters,Other = reserved.*/
	RATIONAL	XResolution;	/*The number of pixels per ResolutionUnit in the ImageWidth direction,Default = 72*/
	RATIONAL	YResolution;    /*The number of pixels per ResolutionUnit in the ImageLength direction,Default = 72*/
	uint8		DateTime[TEXT_DATATIME_LEN];
	uint16		YCbCrSubSampling[2];	/*[2, 1]  =  YCbCr4:2:2,	[2, 2]  =  YCbCr4:2:0,	Other = reserved*/
	uint16		YCbCrPositioning;		/*Default = 1,	1 = centered,	2 = co-sited, Other = reserved*/

	/* private informaton */
	uint16		EntryCount;
	uint16		padding;
	uint32		ExifIFDPointer;
	uint32		NextOffset;	
}EXIF_MAIN_T;


/*1th IFD*/
typedef struct
{
	/* public informaton */
	uint16		Compression;				/*0--Default,1--uncompressed,6--JPEG compression,other--reserved*/
	uint16		padding;
	uint8		Make[TEXT_MAKE_LEN];
	uint8		Model[TEXT_MODEL_LEN];

	uint16		Orientation;
	/*	Default = 1 
	1  =  The 0th row is at the visual top of the image, and the 0th column is the visual left-hand side. 
	2  =  The 0th row is at the visual top of the image, and the 0th column is the visual right-hand side. 
	3  =  The 0th row is at the visual bottom of the image, and the 0th column is the visual right-hand side. 
	4  =  The 0th row is at the visual bottom of the image, and the 0th column is the visual left-hand side. 
	5  =  The 0th row is the visual left-hand side of the image, and the 0th column is the visual top. 
	6  =  The 0th row is the visual right-hand side of the image, and the 0th column is the visual top. 
	7  =  The 0th row is the visual right-hand side of the image, and the 0th column is the visual bottom. 
	8  =  The 0th row is the visual left-hand side of the image, and the 0th column is the visual bottom. 
	Other = reserved*/

	uint16		ResolutionUnit;	/*Default = 2,	2 = inches,	3 = centimeters,	Other = reserved*/
	RATIONAL	XResolution;	/*The number of pixels per ResolutionUnit in the ImageWidth direction,Default = 72*/
	RATIONAL	YResolution;    /*The number of pixels per ResolutionUnit in the ImageLength direction,Default = 72*/
	uint8		DateTime[TEXT_DATATIME_LEN];	/*YYYY:MM:DD HH:MM:SS*/
	uint32		JPEGInterchangeFormat;	/*The offset to the start byte (SOI) of JPEG compressed thumbnail data.*/
 	uint32		JPEGInterchangeFormatLength;

	/* private informaton */
	uint16		EntryCount;
	uint16		padding_01;
	uint32		NextOffset;	

}EXIF_THUMBNAIL_T;

typedef struct
{
	EXIF_MAIN_T			exif_main;
	EXIF_THUMBNAIL_T	exif_thumbnail;
}JPEG_EXIF_T;


extern JPEG_EXIF_T g_JpegExif;
  
/*
typedef struct
{
	//user information
	uint8		MakerNote[TEXT_MAKE_LEN];
	uint8		UserComment[TEXT_COMMENT_LEN];

	//file information
	uint8		RelatedSoundFile[13];

	//Date&Time
	uint8		SubSecTime[TEXT_DATATIME_LEN];
	uint8		SubSecTimeOriginal[TEXT_DATATIME_LEN];
	uint8		SubSecTimeDigitized[TEXT_DATATIME_LEN];

	//picture-taking conditions
	uint8		SpectralSensitivity[4];
	SRATIONAL	ShutterSpeedValue;
	RATIONAL	ApetureValue;
	SRATIONAL	BrightnessValue;
	RATIONAL	SubjectDistance;
	uint16		SubjectArea[4];
	RATIONAL	FlashEnergy;
	uint8		SpatialFrequencyResponse[4];
	RATIONAL	FocalPlaneXResolution;
	RATIONAL	FocalPlaneYResolution;
	uint16		FocalPlaneResolutionUnit;
	uint16		SubjectLocation[2];
	uint16		SensingMethod;
	RATIONAL	ExposureIndex;
	uint8		CFAPattern[MAX_EXIF_COUNT_NUM];
	uint8		DeviceSettingdescription[MAX_EXIF_COUNT_NUM+1];
	uint16		SubjectDistanceRange;

	//other
	uint8		ImageUniqueID[33+1];
};
*/
#endif

#endif