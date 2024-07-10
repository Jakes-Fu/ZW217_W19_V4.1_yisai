#ifndef _JPEG_INTERFACE_H_
#define _JPEG_INTERFACE_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define NEW_INTERFACE

#define LENGTH_TEXT_PIMAGEDESCRIPTION		((uint32) 0x00000100)
#define LENGTH_TEXT_PMAKE						((uint32) 0x00000100)
#define LENGTH_TEXT_PMODEL					((uint32) 0x00000100)
#define LENGTH_TEXT_PARTIST					((uint32) 0x00000100)
#define LENGTH_TEXT_PSOFTWAREVALUE		((uint32) 0x00000100)

#define LENGTH_TEXT_PUSERCOMMENT			((uint32) 0x00000100)
#define LENGTH_TEXT_PCOPYRIGHT				((uint32) 0x00000100)
#define LENGTH_TEXT_PDATETIME				((uint32) 0x00000050)
#define LENGTH_TEXT_PDATETIMEORIGINAL		((uint32) 0x00000050)
#define LENGTH_TEXT_PDATETIMEDIGITILIZED	((uint32) 0x00000050)

/*
 * Data Type struct 
 */

typedef enum
{
	JINF_SUCCESS = 0,
	JINF_STATUS_BUSY,
	JINF_INVALID_ARGUMENT,
	JINF_MEMORY_NOT_ENOUGH,
	JINF_MEMORY_MALLOC_FAILED,
	JINF_MEMORY_FREE_FAILED,
	JINF_PARSE_EXIF_ERROR,
	JINF_HEAD_INFO_ERROR,
	JINF_OUTPUT_BUFFER_OVERFLOW,
	JINF_FORMAT_UNSUPPORTED,	 
	JINF_HARDWARE_ERROR,
	JINF_READ_FILE_ERROR,
	JINF_WRITE_FILE_ERROR,
	JINF_PROGRESSIVE_JPEG_IN_FILE, 
	JINF_SCALING_FACTOR_UNSUPPORTED,
	JINF_SCALING_ERROR,
	JINF_FAILED = 0xff
}JINF_RET_E;

/*JPEG compression type*/
typedef enum
{
	JINF_JPEG_TYPE_BASELINE = 0,
	JINF_JPEG_TYPE_PROGRESSIVE,
	JINF_JPEG_TYPE_LOSSLESS,
	JINF_JPEG_TYPE_HIERARCHICAL,        
    JINF_JPEG_TYPE_SPREADTRUM,          //special JPEG type for Spreadtrum resource.
	JINF_JPEG_TYPE_MAX
}JINF_JPEG_TYPE_E;

/*data type*/
typedef enum
{
	JINF_DATA_TYPE_RGB = 0,
	JINF_DATA_TYPE_YUV,
	JINF_DATA_TYPE_MAX
}JINF_DATA_TYPE_E; 

/*YUV data format*/
typedef enum
{
	JINF_FORMAT_YUV444 = 0,
	JINF_FORMAT_YUV422,
    JINF_FORMAT_YUV420,
    JINF_FORMAT_YUV411,
    JINF_FORMAT_YUV400,
    JINF_FORMAT_YUV422R,
    JINF_FORMAT_YUV411R,
	JINF_FORMAT_YUV_MAX
}JINF_YUV_FORMAT_E; 

/*RGB data format*/
typedef enum
{
	JINF_FORMAT_RGB565 = JINF_FORMAT_YUV_MAX + 1,
	JINF_FORMAT_RGB666,
    JINF_FORMAT_RGB888,
    JINF_FORMAT_ARGB8888,
	JINF_FORMAT_RGB_MAX
}JINF_RGB_FORMAT_E; 

/*YUV data pattern*/
typedef enum
{
	JINF_YUV_PATTERN_SEQUENTIAL = 0,   		/*sequential order.*/
	JINF_YUV_PATTERN_CHN_Y_U_V,				/*channel 0: YYYY..., channel 1: UUUU..., channel 2: VVVV...*/
	JINF_YUV_PATTERN_CHN_Y_UV,			    /*channel 0: YYYY..., channel 1: UVUV...special for SC6600L HW output format*/
	JINF_YUV_PATTERN_MCU,					/*Spreadtrum MCU format*/
	JINF_YUV_PATTERN_MAX
}JINF_YUV_PATTERN_E;

/*data endian type*/
typedef enum
{
	JINF_DATA_BIG_ENDIAN = 0,
	JINF_DATA_LITTLE_ENDIAN,
	JINF_DATA_ENDIAN_MAX
}JINF_DATA_ENDIAN_E;   

/*JPEG compression qaulity*/
typedef enum
{
	JINF_QUALITY_LOW = 0,						/*poor*/
	JINF_QUALITY_MIDDLE_LOW,				    /*normal*/
	JINF_QUALITY_MIDDLE,						/*good*/
	JINF_QUALITY_MIDDLE_HIGH,				    /*excellent*/
	JINF_QUALITY_HIGH,							/*outstanding*/
	JINF_QUALITY_MAX
}JINF_QUALITY_E;

/*YUV channel buffer*/
typedef struct tag_jinf_yuv_chn
{
    uint8       *y_chn_ptr;
    uint8       *u_chn_ptr;
    uint8       *v_chn_ptr;
}JINF_YUV_CHN_T;  

/*rectangle*/
typedef struct tag_jinf_rect
{
	uint16 x;								
	uint16 y;								
	uint16 w;								
	uint16 h;								
}JINF_RECT_T;  

/*function of read JPEG stream data*/
typedef BOOLEAN (*JINF_READ_FILE_FUNC)(uint8 *buf_ptr, uint32 file_offset, uint32 bytes_to_read, uint32 *read_bytes_ptr);

/*function of read YUV data to separate*/
typedef uint32 (*JINF_READ_YUV_FUNC)(JINF_YUV_CHN_T *yuv_chn, uint32 line_offset, uint32 width, uint32 read_lines);

/*function of write JPEG stream data or RGB data.*/
typedef BOOLEAN (*JINF_WRITE_FILE_FUNC)(uint8 *buf_ptr, uint32 file_offset, uint32 bytes_to_write, uint32 *written_bytes_ptr);

/*function of write YUV data.*/
typedef uint32 (*JINF_WRITE_YUV_FUNC)(JINF_YUV_CHN_T *yuv_chn, uint32 line_offset, uint32 width, uint32 write_lines);

/*asynchronous function, which will be called after decoding/encoding done.*/
typedef void (*JINF_ASYNC_CALLBACK)(JINF_RET_E ret_value, void *out_param_ptr);

/*YUV data info*/
typedef	struct jinf_yuv_info_tag
{
	JINF_YUV_CHN_T		    yuv_chn;			
	JINF_YUV_FORMAT_E		sample_format;	
	JINF_YUV_PATTERN_E	    data_pattern;
    JINF_DATA_ENDIAN_E	    data_endian;
}JINF_YUV_INFO_T;

/*RGB data info*/
typedef	struct jinf_rgb_info_tag
{
	void				    *rgb_buf_ptr;			
	JINF_RGB_FORMAT_E	    rgb_format;	
    JINF_DATA_ENDIAN_E	    data_endian;
}JINF_RGB_INFO_T;

typedef enum
{
	JINF_DEC_TARGET_SIZE_RESIZABLE = 0,				//the target size maybe resized and the actual output size maybe not equal to the input target size
	JINF_DEC_TARGET_SIZE_UNRESIZABLE					//the target size can not be resized and the actual output size must eqaul to the input target size
}JINF_DEC_MODE_E;


/*input param of IMGJPEG_CreateMiniature function*/
typedef struct
{ 
	uint8				    *jpeg_buf_ptr;		    //jpeg stream buffer		
	uint32				    jpeg_buf_size;			//jpeg stream size
	JINF_RECT_T			    crop_rect;			    //crop rectangle
   	uint16				    target_width;           //target width
	uint16				    target_height;	        //target height
	JINF_QUALITY_E 		    quality;                //encode quality of miniature JPEG
	BOOLEAN			        is_ouput_yuv;           //output YUV data besides the miniature JPEG stream data if TRUE.
	uint8 				    *decode_buf_ptr;        //decode buffer, temp buffer for decoder/encoder/scaling..
	uint32 				    decode_buf_size;        //decode buffer size
	uint8                   *target_buf_ptr;        //target buffer, if it is NULL, get target buffer from decode_buf_ptr
	uint32                  target_buf_size;        //target buffer size.
	JINF_READ_FILE_FUNC	    read_file_func;         //if it is not NULL, call the read_file_func to get source JPEG stream data 
	JINF_WRITE_FILE_FUNC	write_file_func;        //if it is not NULL, call the write_file_func to return the miniature JPEG stream data
	JINF_WRITE_YUV_FUNC	    write_yuv_func;         //if it is not NULL and is_ouput_yuv is TRUE, call the write_yuv_func to return the YUV data
	JINF_ASYNC_CALLBACK 	async_callback;         //if it is not NULL, the IMGJPEG_CreateMiniature will return immediately after the encoding/decoding
                                                    //task being started and call this asynchronous function when all things done.
	uint32 target_buf_width;        //wre_app, width is 4 align
	uint32 target_buf_height;	    //wre_app,										
      
}JINF_MINI_IN_PARAM_T;

/*output param of IMGJPEG_CreateMiniature function*/
typedef struct
{
	uint16				    output_width;           //width of miniature JPEG.
	uint16				    output_height;          //height of miniature JPEG.
	uint8				    *jpeg_buf_ptr;          //miniature JPEG stream buffer
 	uint32 				    jpeg_buf_size;	        //miniature JPEG stream size.
	JINF_YUV_INFO_T		    yuv_info;				//YUV data info, only used when is_ouput_yuv is TRUE.
}JINF_MINI_OUT_PARAM_T;

/*input param of IMGJPEG_Encode function*/
typedef struct
{
    JINF_YUV_INFO_T		    yuv_info;               //YUV data info
	uint16				    src_width;              //source image width
	uint16				    src_height;             //source image height
	JINF_QUALITY_E 		    quality;	            //encode qaulity
	uint8* 				    encode_buf_ptr;	        //encode buffer, include target buffer and temp buffer for encoder/scaling.
	uint32 				    encode_buf_size;	    //decode buffer size
    uint8                   *target_buf_ptr;        //target buffer, if it is NULL, get target buffer from decode_buf_ptr
    uint32                  target_buf_size;        //target buffer size.	
    JINF_READ_YUV_FUNC	    read_yuv_func;          //if it is not NULL, call read_yuv_func to get source YUV data.
	JINF_WRITE_FILE_FUNC	write_file_func;        //if it is not NULL, call write_file_func to return JPEG stream data.
	JINF_ASYNC_CALLBACK 	async_callback;         //if it is not NULL, the IMGJPEG_Encode will return immediately after the encoding
													//task being started and call this asynchronous function when all things done.
   	uint32					enc_slice_height;  		//slice height of encoding. Must be exactly divided by 8.
   													//Set 0 to use default value if do not care about it.   
    uint32                  reserved;
}JINF_ENC_IN_PARAM_T;

/*output param of IMGJPEG_Encode function*/
typedef struct
{
	uint16				    output_width;           //width of output JPEG.
	uint16				    output_height;          //height of output JPEG.
	uint8 				    *jpeg_buf_ptr;          //output JPEG stream buffer
 	uint32 				    jpeg_buf_size;			//output JPEG stream size	    	
}JINF_ENC_OUT_PARAM_T;


/*input param of IMGJPEG_Decode function*/
typedef struct jinf_dec_in_param_tag
{
   	uint8	 			    *jpeg_buf_ptr;		    //jpeg stream buffer	 
	uint32 				    jpeg_buf_size;	        //jpeg stream size
	JINF_RECT_T			    crop_rect;			    //crop rectangle
   	uint16 				    target_width;	        //expectant target width		 
	uint16 				    target_height;          //expectant target height
	uint8* 				    decode_buf_ptr;		    //decode buffer, include target buffer and temp buffer for decoder/scaling.
	uint32 				    decode_buf_size;	    //decode buffer size
	uint8                              *target_buf_ptr;        //target buffer, if it is NULL, get target buffer from decode_buf_ptr
	uint32                             target_buf_size;        //target buffer size.	
	JINF_DATA_TYPE_E		target_data_type;       //output data type, YUV or RGB.
	JINF_RGB_FORMAT_E		target_rgb_format;      //RGB data format, only used when the output data type is RGB. 
	JINF_DATA_ENDIAN_E		target_data_endian;		//target data endian
	JINF_READ_FILE_FUNC	    read_file_func;         //if it is not NULL, call read_yuv_func to get source JPEG stream data.
	union
	{
		JINF_WRITE_YUV_FUNC	    write_yuv;          //valid if output data type is YUV
		JINF_WRITE_FILE_FUNC	write_file;         //valid if output data type is RGB
	}write_func;                                    //if it is not NULL, call write_func to return output data.
	JINF_ASYNC_CALLBACK 	async_callback;         //if it is not NULL, the IMGJPEG_Decode will return immediately after the decoding
                                                    //task being started and call this asynchronous function when all things done.

	uint16		target_buf_width;     //wre_app, width is 4 align
	uint16		target_buf_height;   //wre_app,
	JINF_DEC_MODE_E img_dec_mode;   //Scaling mode , HW or SW
}JINF_DEC_IN_PARAM_T;

/*output param of IMGJPEG_Decode function*/
typedef struct jinf_dec_outparam_tag
{
   	uint16 				    output_width;		    //output image width 
	uint16 				    output_height;		    //output image height
	union 
	{
		JINF_YUV_INFO_T	    yuv;				    //valid if output data type is YUV
		JINF_RGB_INFO_T 	rgb;			        //valid if output data type is RGB
	}out_data_info;                                 //output data info
}JINF_DEC_OUT_PARAM_T;

typedef struct jinf_dec_res_in_prarm_tag
{
   	uint8	 			    *jpeg_buf_ptr;		    //jpeg stream buffer	 
	uint32 				    jpeg_buf_size;	        //jpeg stream size
	uint8 				    *decode_buf_ptr;		//decode buffer, include target buffer and temp buffer for decoder/scaling.
	uint32 				    decode_buf_size;	    //decode buffer size
    JINF_RGB_FORMAT_E	    format;
    JINF_RECT_T			    img_rect;
    JINF_RECT_T			    target_rect;
    uint32                  target_buf_width;
    void	 			    *target_buf_ptr;
}JINF_DEC_RES_IN_PARAM_T;

typedef struct jinf_dec_res_out_prarm_tag
{
   	uint16 				    output_width;		    //output image width 
	uint16 				    output_height;		    //output image height
	uint16					actual_width;			//actual width of output image
	uint32					actual_height;			//actual height of output image

	uint8					*output_buf_ptr;		//output rgb565 buffer
	uint32					output_buf_size;		//output buffer size, should be output_width * output_height * 2
}JINF_DEC_RES_OUT_PARAM_T;

typedef struct jinf_get_dec_buf_size_in_tag
{
	uint16 					image_width;
	uint16					image_height;
	JINF_RECT_T				src_crop_rect;
	JINF_JPEG_TYPE_E		jpeg_type;
	JINF_YUV_FORMAT_E		sample_format;
	BOOLEAN					is_jpeg_in_file;
	uint16					target_width;
	uint16					target_height;
	JINF_DATA_TYPE_E		target_data_type;
	BOOLEAN					is_output_jpeg;			
}JINF_GET_DEC_BUF_SIZE_IN_T;

typedef struct jinf_get_dec_buf_size_out_tag
{
	uint32					temp_buf_size;
}JINF_GET_DEC_BUF_SIZE_OUT_T;


/**---------------------------------------------------------------------------*
 **                         Exif related structure		                      *
 **---------------------------------------------------------------------------*/
typedef enum
{
    EXIF_BYTE            = 1,
    EXIF_ASCII           = 2,    
    EXIF_SHORT           = 3,
    EXIF_LONG            = 4,
    EXIF_RATIONAL        = 5,
    EXIF_UNDEFINED       = 7,
    EXIF_SLONG           = 9,
    EXIF_SRATIONAL       = 10
}EXIF_TYPE_E;

typedef uint8	EXIF_BYTE_T;
typedef uint8	EXIF_ASCII_T;
typedef uint16	EXIF_SHORT_T;
typedef uint32	EXIF_LONG_T;
typedef uint8	EXIF_UNDEFINED_T;
typedef int32	EXIF_SLONG_T;

typedef struct 
{
	uint32		numerator;
	uint32		denominator;
}EXIF_RATIONAL_T;

typedef struct
{
	int32		numerator;
	int32		denominator;
}EXIF_SRATIONAL_T;

typedef struct
{   
    EXIF_TYPE_E	type;       //type of the content
	void 		*ptr;       //content buffer
	uint32		count;		//count of content by type
    uint32      size;       //size of content buffer by byte
}EXIF_CUSTOM_T;


#define MAX_ASCII_STR_SIZE     256
#define EXIF_VALID              1

//Tags relating to image data structure. Mandatory
typedef struct exif_pri_basic_tag
{
    EXIF_SHORT_T        YCbCrPositioning;
    EXIF_RATIONAL_T     XResolution;
    EXIF_RATIONAL_T     YResolution;
    EXIF_SHORT_T        ResolutionUnit;
}EXIF_PRI_BASIC_T;

//Tags relating to image data structure. Optional
typedef struct exif_pri_data_struct_valid_tag
{
    uint32              Orientation     :1;
    uint32              Reserved        :1;
}EXIF_PRI_DATA_STRUCT_VALID_T;

typedef struct exif_pri_data_struct_tag
{                  
    EXIF_PRI_DATA_STRUCT_VALID_T    valid;
    
    EXIF_SHORT_T                    Orientation;
    EXIF_SHORT_T                    Reserved;                   //reserved. ignore    
}EXIF_PRI_DATA_STRUCT_T;

//Tags relating to image data characteristics. Optional
typedef struct exif_data_char_valid_tag
{
    uint32        TransferFunction              :1;
    uint32        WhitePoint                    :1;
    uint32        PrimaryChromaticities         :1;
    uint32        YCbCrCoefficients             :1;
    uint32        ReferenceBlackWhite           :1;
}EXIF_PRI_DATA_CHAR_VALID_T;

typedef struct exif_data_char_tag
{
    EXIF_PRI_DATA_CHAR_VALID_T      valid;

    EXIF_SHORT_T        TransferFunction[768];
    EXIF_RATIONAL_T     WhitePoint[2];
    EXIF_RATIONAL_T     PrimaryChromaticities[6];
    EXIF_RATIONAL_T     YCbCrCoefficients[3];
    EXIF_RATIONAL_T     ReferenceBlackWhite[6];
}EXIF_PRI_DATA_CHAR_T;

//Tag relating to image descriptions. Optional
typedef struct exif_pri_desc_valid_tag
{
    uint32        DateTime              :1;
    uint32        ImageDescription      :1;
    uint32        Make                  :1;
    uint32        Model                 :1;
    uint32        Software              :1;
    uint32        Artist                :1;
    uint32        Copyright             :1;
}EXIF_PRI_DESC_VALID_T;

typedef struct exif_pri_desc_tag
{
    EXIF_PRI_DESC_VALID_T   valid;
    
    EXIF_ASCII_T        DateTime[20];
    EXIF_ASCII_T        ImageDescription[MAX_ASCII_STR_SIZE];
    EXIF_ASCII_T        Make[MAX_ASCII_STR_SIZE];
    EXIF_ASCII_T        Model[MAX_ASCII_STR_SIZE];
    EXIF_ASCII_T        Software[MAX_ASCII_STR_SIZE];
    EXIF_ASCII_T        Artist[MAX_ASCII_STR_SIZE];
    EXIF_ASCII_T        Copyright[MAX_ASCII_STR_SIZE];
}EXIF_PRI_DESC_T;

typedef struct exif_primary_tag
{    
    EXIF_PRI_BASIC_T                basic;                         //Mandatory
    EXIF_PRI_DATA_STRUCT_T          *data_struct_ptr;              //Optional. Set NULL to ignore it
    EXIF_PRI_DATA_CHAR_T            *data_char_ptr;                //Optional. Set NULL to ignore it
    EXIF_PRI_DESC_T                 *img_desc_ptr;                 //Optional. Set NULL to ignore it
    uint32                          reserved;                      //reserved
}EXIF_PRIMARY_INFO_T;

//Tags relating to image data characteristics and image configuration. Mandatory
typedef struct exif_spec_basic_tag
{       
    EXIF_SHORT_T        ColorSpace;
    EXIF_UNDEFINED_T    ComponentsConfiguration[4];   
    EXIF_LONG_T         PixelXDimension;
    EXIF_LONG_T         PixelYDimension;
}EXIF_SPEC_BASIC_T;

//Tags relating to version, read only.
typedef struct exif_spec_version_tag
{
    EXIF_UNDEFINED_T    ExifVersion[4];
    EXIF_UNDEFINED_T    FlashpixVersion[4]; 
}EXIF_SPEC_VERSION_T;

//Tags relating to image configuration. Optional
typedef struct exif_spec_img_config_valid_tag
{
    uint32        CompressedBitsPerPixel         :1;
}EXIF_SPEC_IMG_CONFIG_VALID_T;

typedef struct exif_spec_img_config_tag
{
    EXIF_SPEC_IMG_CONFIG_VALID_T    valid;
    EXIF_RATIONAL_T                 CompressedBitsPerPixel;    
}EXIF_SPEC_IMG_CONFIG_T;

//Tags relating to user information. Optional
typedef struct exif_spec_user_valid_tag
{
    uint32        MakerNote                     :1;
    uint32        UserComment                   :1;
}EXIF_SPEC_USER_VALID_T;

typedef struct exif_spec_user_tag
{
    EXIF_SPEC_USER_VALID_T  	valid;  

    EXIF_CUSTOM_T            	MakerNote;
    EXIF_CUSTOM_T            	UserComment;  
}EXIF_SPEC_USER_T;

//Tags relating to related file information. Optional
typedef struct exif_spec_related_file_valid_tag
{
    uint32        RelatedSoundFile              :1;
}EXIF_SPEC_RELATED_FILE_VALID_T;

typedef struct exif_spec_related_file_tag
{   
    EXIF_SPEC_RELATED_FILE_VALID_T		valid;    
    EXIF_ASCII_T						RelatedSoundFile[13];
}EXIF_SPEC_RELATED_FILE_T;

//Tags relating to date and time. Optional
typedef struct exif_spec_date_time_valid_tag
{
    uint32          DateTimeOriginal            :1;
    uint32          DateTimeDigitized           :1;
    uint32          SubSecTime                  :1;
    uint32          SubSecTimeOriginal          :1;
    uint32          SubSecTimeDigitized         :1;
}EXIF_SPEC_DATE_TIME_VALID_T;

typedef struct exif_spec_date_time_tag
{
    EXIF_SPEC_DATE_TIME_VALID_T     valid;
    
    EXIF_ASCII_T        DateTimeOriginal[20];
    EXIF_ASCII_T        DateTimeDigitized[20];
    EXIF_ASCII_T        SubSecTime[MAX_ASCII_STR_SIZE];
    EXIF_ASCII_T        SubSecTimeOriginal[MAX_ASCII_STR_SIZE];
    EXIF_ASCII_T        SubSecTimeDigitized[MAX_ASCII_STR_SIZE];
}EXIF_SPEC_DATE_TIME_T;

//Tags relating to picture-taking conditions. Optional
typedef struct exif_spec_pic_taking_cond_valid_0_tag
{
    uint64              ExposureTime            :1;
    uint64              FNumber                 :1;
    uint64              ExposureProgram         :1;
    uint64              SpectralSensitivity     :1;
    uint64              ISOSpeedRatings         :1;

    uint64				OECF                    :1;	
    uint64		        ShutterSpeedValue       :1;
	uint64			    ApertureValue           :1;
	uint64		        BrightnessValue         :1;
	uint64		        ExposureBiasValue       :1;     

	uint64			    MaxApertureValue        :1;
	uint64			    SubjectDistance         :1;
	uint64			    MeteringMode            :1;
	uint64			    LightSource             :1;	
	uint64			    Flash                   :1;

	uint64			    FocalLength             :1;
	uint64			    SubjectArea             :1;			
	uint64			    FlashEnergy             :1;
	uint64				SpatialFrequencyResponse:1;
	uint64			    FocalPlaneXResolution   :1;

	uint64			    FocalPlaneYResolution   :1;
	uint64			    FocalPlaneResolutionUnit:1;
	uint64			    SubjectLocation         :1;
	uint64			    ExposureIndex           :1;
	uint64			    SensingMethod           :1;

	uint64		        FileSource              :1;
	uint64		        SceneType               :1;
	uint64				CFAPattern              :1;
	uint64			    CustomRendered          :1;
	uint64			    ExposureMode            :1;
	uint64			    WhiteBalance                :1;
	uint64			    DigitalZoomRatio            :1;
	uint64			    FocalLengthIn35mmFilm       :1;
	uint64			    SceneCaptureType            :1;	
	uint64			    GainControl                 :1;

	uint64			    Contrast                    :1;
	uint64			    Saturation                  :1;
	uint64			    Sharpness                   :1;
	uint64			    DeviceSettingDescription    :1;
	uint64			    SubjectDistanceRange        :1;
}EXIF_SPEC_PIC_TAKING_COND_VALID_T;

typedef struct exif_spec_pic_taking_cond_tag
{
    EXIF_SPEC_PIC_TAKING_COND_VALID_T     valid;        
           
	EXIF_RATIONAL_T			ExposureTime;
	EXIF_RATIONAL_T			FNumber;
	EXIF_SHORT_T			ExposureProgram;	
	EXIF_ASCII_T			SpectralSensitivity[MAX_ASCII_STR_SIZE];	
	EXIF_CUSTOM_T				ISOSpeedRatings;	//Modified

	EXIF_CUSTOM_T				OECF;	
	EXIF_SRATIONAL_T		ShutterSpeedValue;
	EXIF_RATIONAL_T			ApertureValue;
	EXIF_SRATIONAL_T		BrightnessValue;
	EXIF_SRATIONAL_T		ExposureBiasValue;

	EXIF_RATIONAL_T			MaxApertureValue;
	EXIF_RATIONAL_T			SubjectDistance;
	EXIF_SHORT_T			MeteringMode;
	EXIF_SHORT_T			LightSource;	
	EXIF_SHORT_T			Flash;

	EXIF_RATIONAL_T			FocalLength;
	EXIF_CUSTOM_T			    SubjectArea;			// 2 or 3 or 4
	EXIF_RATIONAL_T			FlashEnergy;
	EXIF_CUSTOM_T				SpatialFrequencyResponse;
	EXIF_RATIONAL_T			FocalPlaneXResolution;

	EXIF_RATIONAL_T			FocalPlaneYResolution;
	EXIF_SHORT_T			FocalPlaneResolutionUnit;
	EXIF_SHORT_T			SubjectLocation[2];
	EXIF_RATIONAL_T			ExposureIndex;
	EXIF_SHORT_T			SensingMethod;

	EXIF_UNDEFINED_T		FileSource;
	EXIF_UNDEFINED_T		SceneType;
	EXIF_CUSTOM_T				CFAPattern;
	EXIF_SHORT_T			CustomRendered;
	EXIF_SHORT_T			ExposureMode;

	EXIF_SHORT_T			WhiteBalance;
	EXIF_RATIONAL_T			DigitalZoomRatio;
	EXIF_SHORT_T			FocalLengthIn35mmFilm;
	EXIF_SHORT_T			SceneCaptureType;	
	EXIF_SHORT_T			GainControl;
    
	EXIF_SHORT_T			Contrast;
	EXIF_SHORT_T			Saturation;
	EXIF_SHORT_T			Sharpness;
	EXIF_CUSTOM_T				DeviceSettingDescription;
	EXIF_SHORT_T			SubjectDistanceRange;
}EXIF_SPEC_PIC_TAKING_COND_T;


//Other tags. Optional
typedef struct exif_spec_other_valid_t
{
    uint32			ImageUniqueID               :1;
}EXIF_SPEC_OTHER_VALID_T;

typedef struct exif_spec_other_tag
{
    EXIF_SPEC_OTHER_VALID_T valid;
    EXIF_ASCII_T            ImageUniqueID[33];
}EXIF_SPEC_OTHER_T;

//structure of EXIF-specific information
typedef struct exif_specific_info_tag
{
    EXIF_SPEC_BASIC_T               basic;                      //Mandatory
    EXIF_SPEC_VERSION_T				*version_ptr;				//Optional. Read only. Set NULL to ignore it
    EXIF_SPEC_IMG_CONFIG_T          *img_config_ptr;            //Optional. Set NULL to ignore it
    EXIF_SPEC_USER_T                *user_ptr;                  //Optional. Set NULL to ignore it
    EXIF_SPEC_RELATED_FILE_T        *related_file_ptr;          //Optional. Set NULL to ignore it
    EXIF_SPEC_DATE_TIME_T           *date_time_ptr;             //Optional. Set NULL to ignore it
    EXIF_SPEC_PIC_TAKING_COND_T     *pic_taking_cond_ptr;       //Optional. Set NULL to ignore it
    EXIF_SPEC_OTHER_T               *other_ptr;                 //Optional. Set NULL to ignore it
}EXIF_SPECIFIC_INFO_T;

//structure of GPS information
typedef struct exif_gps_valid_tag
{
	uint32			        GPSVersionID            :1;							//read only
	uint32	            	GPSLatitudeRef          :1;							
	uint32		            GPSLatitude             :1;
	uint32		            GPSLongitudeRef         :1;
	uint32		            GPSLongitude            :1;
	uint32			        GPSAltitudeRef          :1;
	uint32		            GPSAltitude             :1;
	uint32		            GPSTimeStamp            :1;
	uint32		            GPSSatellites           :1;	
	uint32		            GPSStatus               :1;								
	uint32		            GPSMeasureMode          :1;
	uint32		            GPSDOP                  :1;										
	uint32		            GPSSpeedRef             :1;								
	uint32		            GPSSpeed                :1;									
	uint32		            GPSTrackRef             :1;								
	uint32		            GPSTrack                :1;										
	uint32		            GPSImgDirectionRef      :1;
	uint32		            GPSImgDirection         :1;
	uint32		            GPSMapDatum             :1;
	uint32		            GPSDestLatitudeRef      :1;						
	uint32		            GPSDestLatitude         :1;							
	uint32		            GPSDestLongitudeRef     :1;						
	uint32		            GPSDestLongitude        :1;						
	uint32		            GPSDestBearingRef       :1;						
	uint32		            GPSDestBearing          :1;								
	uint32		            GPSDestDistanceRef      :1;						
	uint32		            GPSDestDistance         :1;							
	uint32	                GPSProcessingMethod     :1;	
	uint32			        GPSAreaInformation      :1;							
	uint32		            GPSDateStamp            :1;							
	uint32		            GPSDifferential         :1;        
}EXIF_GPS_VALID_T;

typedef struct exif_gps_tag
{
    EXIF_GPS_VALID_T                valid;

	EXIF_BYTE_T			            GPSVersionID[4];							//read only
	EXIF_ASCII_T	            	GPSLatitudeRef[2];							
	EXIF_RATIONAL_T		            GPSLatitude[3];
	EXIF_ASCII_T		            GPSLongitudeRef[2];
	EXIF_RATIONAL_T		            GPSLongitude[3];
	EXIF_BYTE_T			            GPSAltitudeRef;
	EXIF_RATIONAL_T		            GPSAltitude;
	EXIF_RATIONAL_T		            GPSTimeStamp[3];
	EXIF_ASCII_T		            GPSSatellites[MAX_ASCII_STR_SIZE];	
	EXIF_ASCII_T		            GPSStatus[2];								
	EXIF_ASCII_T		            GPSMeasureMode[2];
	EXIF_RATIONAL_T		            GPSDOP;										
	EXIF_ASCII_T		            GPSSpeedRef[2];								
	EXIF_RATIONAL_T		            GPSSpeed;									
	EXIF_ASCII_T		            GPSTrackRef[2];								
	EXIF_RATIONAL_T		            GPSTrack;										
	EXIF_ASCII_T		            GPSImgDirectionRef[2];
	EXIF_RATIONAL_T		            GPSImgDirection;
	EXIF_ASCII_T		            GPSMapDatum[MAX_ASCII_STR_SIZE];
	EXIF_ASCII_T		            GPSDestLatitudeRef[2];						
	EXIF_RATIONAL_T		            GPSDestLatitude[3];							
	EXIF_ASCII_T		            GPSDestLongitudeRef[2];						
	EXIF_RATIONAL_T		            GPSDestLongitude[3];						
	EXIF_ASCII_T		            GPSDestBearingRef[2];						
	EXIF_RATIONAL_T		            GPSDestBearing;								
	EXIF_ASCII_T		            GPSDestDistanceRef[2];						
	EXIF_RATIONAL_T		            GPSDestDistance;							
    EXIF_CUSTOM_T                      GPSProcessingMethod;
	EXIF_CUSTOM_T			            GPSAreaInformation;							
	EXIF_ASCII_T		            GPSDateStamp[11];							
	EXIF_SHORT_T		            GPSDifferential;								    
}EXIF_GPS_INFO_T;

typedef struct exif_interoperability_valid_tag
{
    uint32          InteroperabilityIndex            :1;
}EXIF_INTEROPERABILITY_VALID_T;

typedef struct exif_interoperability_info_t
{
	EXIF_INTEROPERABILITY_VALID_T 			valid;
	
	EXIF_CUSTOM_T							InteroperabilityIndex;
}EXIF_INTEROPERABILITY_INFO_T;

//structure of EXIF information
typedef struct exif_info_tag
{
    EXIF_PRIMARY_INFO_T             primary;            //Mandatory
    EXIF_SPECIFIC_INFO_T            *spec_ptr;          //Optional. Set NULL to ignore it
    EXIF_GPS_INFO_T                 *gps_ptr;           //Optional. Set NULL to ignore it
    EXIF_INTEROPERABILITY_INFO_T	*inter_ptr;			//Optional. Set NULL to ignore it
}JINF_EXIF_INFO_T;

/**---------------------------------------------------------------------------*
 **                         JPEG info related structure		                  *
 **---------------------------------------------------------------------------*/
/*JPEG thumbnail info*/
typedef struct jinf_thumbnail_info_tag
{
	BOOLEAN	    is_thumb_existed;           //is thumbnail is exist ?
	uint16		thumb_width;                //width of thumbnail
	uint16		thumb_height;               //height of thumbnail
	uint32		thumb_offset;               //offset of thumbnial data from the start of JPEG stream.
   	uint32		thumb_size;                 //thumbnial data size
}JINF_THUMBNAIL_INFO_T;

/*JPEG image info*/
typedef struct jinf_info_tag
{
   	uint16 				    image_width;    //width of JPEG image		
	uint16 				    image_height;   //height of JPEG image
   	uint16				    x_density;	    //density in x direction
    uint16				    y_density;	    //density in y direction
	JINF_JPEG_TYPE_E		jpeg_type;	    //JPEG compression type
	JINF_YUV_FORMAT_E		sample_format;	//sample format of YUV data input to compressor
	JINF_THUMBNAIL_INFO_T	thumbnail_info; //thumbnial info
	JINF_EXIF_INFO_T		*exif_info_ptr; //EXIF info, if it is NULL, EXIT info will not returned
	BOOLEAN			        is_supported;	//is the JPEG type supported.
}JINF_INFO_T;

/*input parameter of IMGJPEG_GetInfo */
typedef struct jinf_get_info_in_tag
{
	uint8					*jpeg_buf_ptr;			//jpeg stream buffer, must be valid if read_file_func is NULL
	uint32					jpeg_buf_size;			//jpeg stream size
	JINF_READ_FILE_FUNC		read_file_func;			//if it is not NULL, call the read_file_func to get source JPEG stream data 
	uint8					*read_buf_ptr;			//temp buffer to read jpeg stream from file, if read_file_func is valid, the read_buf_ptr
													//must be valid
	uint32					read_buf_size;			//16K bytes is a recommended size. Actually anysize is ok, buf the speed will be lower if the buffer is too small 
}JINF_GET_INFO_IN_T;


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               
 **---------------------------------------------------------------------------*/

/****************************************************************************/
/* Purpose:	free resources allocated by JPEG Codec					 			*/
/* Author:	frank.yang														*/
/* Input:      	none															*/
/* Output:	none															*/
/* Return:	operation results													*/
/* Note:    																	*/
/****************************************************************************/
PUBLIC  JINF_RET_E IMGJPEG_FreeRes(void);

/****************************************************************************/
/* Purpose:	get informaton,such as exif,basic image info...				 	*/
/* Author:	frank.yang														*/
/* Input:      																*/
/*			in_param_ptr	--points to in parameter						*/
/* Output:	info_ptr        --jpeg basic information						*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC  JINF_RET_E IMGJPEG_GetInfo(JINF_GET_INFO_IN_T *in_param_ptr,
								 		JINF_INFO_T *info_ptr);


/****************************************************************************/
/* Purpose:	create a miniature for fast view, that is, decode jpeg with big size 			*/
/* then encode into jpeg with small size and small resolution.							*/		
/* Author:	frank.yang														*/
/* Input:      																	*/
/*			in_param	--points to in parameter									*/	
/*			out_param   --points to out parameter									*/
/* Output:	none															*/
/* Return:	operation results													*/
/* Note:    																	*/
/****************************************************************************/
PUBLIC  JINF_RET_E IMGJPEG_CreateMiniature(
									JINF_MINI_IN_PARAM_T *in_param_ptr,
									JINF_MINI_OUT_PARAM_T *out_param_ptr);



/****************************************************************************/
/* Purpose:	Encode YUV image data to a JPEG file					 			*/
/* Author:	frank.yang														*/
/* Input:      																	*/
/*			in_param	--points to in parameter									*/	
/*			out_param   --points to out parameter									*/
/* Output:	none															*/
/* Return:	operation results													*/
/* Note:    																	*/
/****************************************************************************/
PUBLIC  JINF_RET_E IMGJPEG_Encode(
									JINF_ENC_IN_PARAM_T *in_param_ptr,
									JINF_ENC_OUT_PARAM_T *out_param_ptr);


/****************************************************************************/
/* Purpose:	Decode JPEG file to YUV or RGB image data				 			*/
/* then encode into jpeg with small size and small resolution.							*/		
/* Author:	frank.yang														*/
/* Input:      																	*/
/*			in_param	--points to in parameter									*/	
/*			out_param   --points to out parameter									*/
/* Output:	none															*/
/* Return:	operation results													*/
/* Note:    																	*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_Decode(
									JINF_DEC_IN_PARAM_T  *in_param_ptr, 
									JINF_DEC_OUT_PARAM_T *out_param_ptr);

/****************************************************************************/
/* Purpose:	Decode RES JPEG file to RGB565 image data				 			*/
/* then encode into jpeg with small size and small resolution.							*/		
/* Author:	shan.he														*/
/* Input:      																	*/
/*			in_param_ptr	--points to in parameter									*/	
/*			out_param_ptr   --points to out parameter									*/
/* Output:	none															*/
/* Return:	operation results													*/
/* Note:    																	*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_Decode_Res(
									JINF_DEC_RES_IN_PARAM_T  *in_param_ptr, 
									JINF_DEC_RES_OUT_PARAM_T *out_param_ptr);

/****************************************************************************/
/* Purpose:	enable miniature, if the JPG_MINIATURE_SUPPORT is defined, this function   */
/*			can be called to disable miniature which means the IMGJPEG_CreateMiniature*/  
/*			always return error.											*/
/* Author:	shan.he															*/
/* Input:      																*/
/*			enable			--TRUE: enable the miniature					*/
/*							  FALSE: disable the miniature					*/	
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_EnableMiniature(BOOLEAN enable);

/****************************************************************************/
/* Purpose:	create task and other objects used by decoder and encoder which are           */
/*			not be freed. Should be called before the system startup	      */
/* Author:	shan.he															*/
/* Input:   none															*/	
/* Output:	none															*/
/* Return:	none. Any error occurred in the function will cause an ASSERT	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC void IMGJPEG_Init(void);

/****************************************************************************/
/* Purpose:	get decode buffer size											*/
/* Author:	shan.he															*/
/* Input:      																*/
/*			in_param_ptr			--pointer of input parameters			*/
/*			out_param_ptr			--pointer of output parameters			*/	
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_GetDecodeBufSize(JINF_GET_DEC_BUF_SIZE_IN_T *in_param_ptr,
										   	JINF_GET_DEC_BUF_SIZE_OUT_T *out_param_ptr);

/****************************************************************************/
/* Purpose:	set jpeg task priority if task exist											*/
/* Author:	shan.he															*/
/* Input:      																*/
/*			new_priority			--new priority							*/
/*			old_priority_ptr		--pointer to receive the old priority	*/	
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_SetTaskPriority(uint32 new_priority, uint32 *old_priority_ptr);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // _JPEG_INTERFACE_H_



