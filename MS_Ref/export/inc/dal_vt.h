/******************************************************************************
 ** File Name:      dal_vt.h                                              
 ** Author:         shunyi.wang                                              
 ** DATE:           8/21/2006                                                
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:    This file defines the marcro, struct and enums used for VT
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE           NAME             DESCRIPTION                               
 ** 8/21/2006     shujing.dong     Create.                                   
 ******************************************************************************/
#ifndef _DAL_VT_H
#define _DAL_VT_H

#include "sci_types.h"
#include "audio_api.h"

#define VT_ENC_NOTIFY_EVENT (1<<0)
#define VT_DEC_NOTIFY_EVENT (1<<1)

typedef enum
{
    AP_RET_SUCCESS,
    AP_RET_INVALID_PARAM,
    AP_RET_NOT_SUPPORTED,
    AP_RET_INVALID_STATE,
    AP_RET_MEMORY_OVERFLOW,
    AP_RET_HREF_ERROR,
    AP_RET_ENC_FIFO_FULL,
    AP_RET_SENSOR_INIT_FAIL,
    AP_RET_UNKNOWN_COMMAND,   /* Unknown Command from MVPU */
    AP_RET_FAIL,
    AP_RET_DMA_OCCUPIED,
    AP_RET_MVE_NO_EFFECT_IMAGE,
    AP_RET_MAX
} AP_RET_CODE_TYPE;

typedef enum
{
    AP_VT_ENUM_2D_FORMAT_YUV_422           = (0x08 << 4),
    AP_VT_ENUM_2D_FORMAT_YUV_420           = (0x0C << 4),
    AP_VT_ENUM_2D_FORMAT_RGB_8BPP          = (0x00 << 4),
    AP_VT_ENUM_2D_FORMAT_RGB_12BPP         = (0x01 << 4),
    AP_VT_ENUM_2D_FORMAT_RGB_16BPP         = (0x02 << 4),
    AP_VT_ENUM_2D_FORMAT_RGB_18BPP         = (0x03 << 4),
    AP_VT_ENUM_2D_FORMAT_RGB_24BPP         = (0x04 << 4),
    AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_LEFT    = (0x01 << 1),
    AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_RIGHT   = (0x00 << 1),
    AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_LEFT  = (0x01 << 0),
    AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_RIGHT = (0x00 << 0)
} AP_VT_ENUM_2D_FORMAT_TYPE;

typedef enum
{
//  profileAndLevel             code        typ.size    max bitrate(kbit/s)
    AP_VT_SIMPLE_LEVEL_1           = 1,    //  QCIF        64
    AP_VT_SIMPLE_LEVEL_2           = 2,    //  CIF         128
    AP_VT_SIMPLE_LEVEL_3           = 3,    //  CIF         384
    AP_VT_H263_LEVEL_3             = 10,   //  
    AP_VT_MAIN_LEVEL_2             = 50,   //  CIF         2000
    AP_VT_MAIN_LEVEL_3             = 51,   //  CCIR601     15000
    AP_VT_MAIN_LEVEL_4             = 52,   //  1920x1088   38400
    AP_VT_PROFILE_AND_LEVEL_MAX    = 0x7FFFFFFF
} AP_VT_VideoPROFILE_AND_LEVEL_TYPE;

typedef enum
{
    AP_VT_Video_POST_PROCESS_NONE            ,
    AP_VT_Video_POST_PROCESS_LVL_1           ,
    AP_VT_Video_POST_PROCESS_LVL_2           ,
    AP_VT_Video_POST_PROCESS_LVL_3           ,
    AP_VT_Video_POST_PROCESS_LVL_4           ,
    AP_VT_Video_POST_PROCESS_LVL_MAX         = AP_VT_Video_POST_PROCESS_LVL_4 ,
    AP_VT_Video_POST_PROCESS_TO_INT          = 0x7FFFFFFF
} AP_VT_Video_POST_PROCESS_TYPE;

typedef enum
{
    AP_VT_PIXEL_YUV422     = AP_VT_ENUM_2D_FORMAT_YUV_422,

    AP_VT_PIXEL_YUV420     = AP_VT_ENUM_2D_FORMAT_YUV_420,

    AP_VT_PIXEL_RGB332     = AP_VT_ENUM_2D_FORMAT_RGB_8BPP,

    AP_VT_PIXEL_RGB444_0   = /* xRGB */
        AP_VT_ENUM_2D_FORMAT_RGB_12BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_RIGHT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_RIGHT,

    AP_VT_PIXEL_RGB444_1   = /* RGxB */
        AP_VT_ENUM_2D_FORMAT_RGB_12BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_RIGHT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_LEFT,

    AP_VT_PIXEL_RGB444_2   = /* RxGB */
        AP_VT_ENUM_2D_FORMAT_RGB_12BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_LEFT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_RIGHT,

    AP_VT_PIXEL_RGB444_3   = /* RGBx */
        AP_VT_ENUM_2D_FORMAT_RGB_12BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_LEFT |AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_LEFT,

    AP_VT_PIXEL_RGB565     = AP_VT_ENUM_2D_FORMAT_RGB_16BPP,

    AP_VT_PIXEL_RGB666_0   = /* xRGB */
        AP_VT_ENUM_2D_FORMAT_RGB_18BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_RIGHT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_RIGHT,

    AP_VT_PIXEL_RGB666_1   = /* RGxB */
        AP_VT_ENUM_2D_FORMAT_RGB_18BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_RIGHT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_LEFT,

    AP_VT_PIXEL_RGB666_2   = /* RxGB */
        AP_VT_ENUM_2D_FORMAT_RGB_18BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_LEFT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_RIGHT,

    AP_VT_PIXEL_RGB666_3   = /* RGBx */
        AP_VT_ENUM_2D_FORMAT_RGB_18BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_LEFT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_LEFT,

    AP_VT_PIXEL_RGB888_0   = /* xRGB */
        AP_VT_ENUM_2D_FORMAT_RGB_24BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_RIGHT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_RIGHT,

    AP_VT_PIXEL_RGB888_1   = /* RGxB */
        AP_VT_ENUM_2D_FORMAT_RGB_24BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_RIGHT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_LEFT,

    AP_VT_PIXEL_RGB888_2   = /* RxGB */
        AP_VT_ENUM_2D_FORMAT_RGB_24BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_LEFT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_RIGHT,

    AP_VT_PIXEL_RGB888_3   = /* RGBx */
        AP_VT_ENUM_2D_FORMAT_RGB_24BPP | AP_VT_ENUM_2D_FORMAT_BUS_ALIGN_LEFT | AP_VT_ENUM_2D_FORMAT_PIXEL_ALIGN_LEFT,

    AP_VT_PIXEL_MAX        = /* Max to 32bit */
        0x7FFFFFFF
} AP_VT_PIXEL_TYPE;

typedef struct
{
    int32 width;
    int32 height;
} AP_VT_VideoSIZE_STRUCT;

typedef struct
{
    int32 sx;                        /* start x  */
    int32 sy;                        /* start y  */
    int32 dx;                        /* width    */
    int32 dy;                        /* height   */
} AP_VT_VideoRECT_STRUCT;

typedef enum
{
    AP_VT_ROTATE_0,
    AP_VT_ROTATE_90,
    AP_VT_ROTATE_180,
    AP_VT_ROTATE_270,
    AP_VT_ROTATE_MAX   = 0x7FFFFFFF    /*  uskim jpeg rot  */
} AP_VT_ROTATE_TYPE;

typedef struct{
    uint16	   	left;        	   	/*!<left up point coordinate x */
    uint16	   	top;           	   	/*!<left up point coordinate y */
    uint16	   	right;         	   	/*!<right down point coordinate x */
    uint16	   	bottom;       	   	/*!<right down point coordinate y */
}AP_VT_RECT_T;

typedef enum{
	AP_VT_DISPMODE_FULLME,					/*只显示自已的图像			*/
	AP_VT_DISPMODE_FULLREMOTE,				/*只显示对方的图像			*/
	AP_VT_DISPMODE_DOUBLE,					/*同时显示双方的图像			*/
	AP_VT_DISPMODE_MAX					
}AP_VT_DISP_MODE_E;

typedef enum{
	AP_VT_VIDEOMODE_ENC_ONLY,				/*只显示自已的图像			*/
	AP_VT_VIDEOMODE_DEC_ONLY,				/*只显示对方的图像			*/
	AP_VT_VIDEOMODE_ENC_DEC,				/*同时显示双方的图像,己方在前	*/
	AP_VT_VIDEOMODE_DEC_ENC,				/*同时显示双方的图像,对方在前	*/
	AP_VT_VIDEOMODE_MAX					
}AP_VT_VIDEO_MODE_E;

typedef enum{
	AP_VT_CIRCLE_ME_HMIRROR,				/*竖直方向的变换				*/				
	AP_VT_CIRCLE_ME_VMIRROR,				/*横向方向的变换				*/
	AP_VT_CIRCLE_ME_MAX	
}AP_VT_CIRCLE_ME_E;

typedef enum
{
	AP_VT_ENC_LEVEL_0, 
	AP_VT_ENC_LEVEL_1, 
	AP_VT_ENC_LEVEL_2, 
	AP_VT_ENC_LEVEL_3, 
	AP_VT_ENC_LEVEL_MAX	
}AP_VT_ENC_LEVEL_E;

typedef enum
{
	AP_VT_ENC_ZOOM_LEVEL0=0, 
	AP_VT_ENC_ZOOM_LEVEL1=4, 
	AP_VT_ENC_ZOOM_LEVEL2=8, 
	AP_VT_ENC_ZOOM_LEVEL3=12, 
	AP_VT_ENC_ZOOM_LEVEL4=16, 
	AP_VT_ENC_ZOOM_LEVEL5=20, 
	AP_VT_ENC_ZOOM_LEVEL6=24, 	
	AP_VT_ENC_ZOOM_MAX=30 
}AP_VT_ENC_ZOOM_LEVEL_E;

typedef enum
{
    AP_VT_SENSOR_PROPERTY_BRIGHTNESS,
    AP_VT_SENSOR_PROPERTY_COLOR,
    AP_VT_SENSOR_PROPERTY_MAX
}  AP_VT_SENSOR_PROPERTY_E;

typedef struct AP_VT_OSTDATA_TAG
{
    AP_VT_RECT_T OSD_Rect;
    uint16* Image; 
    uint16 BackColor;
    uint16 u16pad;
}AP_VT_OSTDATA_T;

typedef struct AP_VT_DISPMODE_TAG
{
    AP_VT_DISP_MODE_E dispmode;
    AP_VT_RECT_T me_rect; 
    AP_VT_RECT_T remote_rect;
}AP_VT_DISPMODE_T;

typedef struct AP_TV_PREVIEWDATA_TAG
{
    AP_VT_RECT_T Preview_Rect;
    AP_VT_RECT_T OSD_Rect;
    uint16* OSD_Image;
    uint16 BackColor;
    uint16  u16pad;
}AP_VT_PREVIEWDATA_T;

typedef struct
{
    int32 RateCtrlEnable;            // 0 : disable  1: enable
    int32 targetBitRate;             // 400 ~  (bit/s)
    int32 DefaultQuantParameter;     // 1 ~ 32, default QP value if the Rate Control is disabled
} AP_VT_VideoRATE_CONTROL;

typedef struct{
    int32 VideoPacketsEnable;   // 0: disable 1: enable  : Video packets are not available with SVH or H.263 streams.
    int32 DataPartitionEnable;   //  0: disable 1: enable  : Enable the use of the Data Partitioning.
    int32 RvlcEnable;                 //   Valid values: 0 . disabled, 1 . enabled
} AP_VT_VideoEncParam_ErrorTools;

typedef struct /* MP4API_EncParam_CompressionParameters */
{
    int32  AcPredictionEnable; //Valid values: 0 . disabled, 1 . enabled   /* Enable AC Prediction */
    int32 RoundingCtrl;  //Valid values: 0 . disabled, 1 . enabled  /* Rounding Control for the Interpolation */
}AP_VT_VideoEncParam_CompressionParameters;

typedef struct {
    int32 IntraCycleEnable;
    int32 IntraCount;
} AP_VT_VedioIntra_cycle;

typedef struct
{
    AP_VT_VideoSIZE_STRUCT size;
    AP_VT_VideoRECT_STRUCT dispRect;    // x : divisible by 8, y : divisible by 2
    AP_VT_VideoRATE_CONTROL rateCtrl;
    AP_VT_VideoEncParam_ErrorTools errorTool;
    AP_VT_VideoEncParam_CompressionParameters compressPara;
    // T3_MP4API_EncParam_CyclicIntraRefresh intraRefresh;
    AP_VT_VedioIntra_cycle intraCycle;
    int32 zoomLevel;         // 0 ~ ??? : use t3_GetMaxZoomLevel()
    int32 useCallback;       // use callback function if TRUE
    int32 foregroundEn;      // MV_TRUE if foreground, else background
    int32 h263En;            // MV_TRUE : H.263, MV_FALSE : MP4

    AP_VT_VideoPROFILE_AND_LEVEL_TYPE profileAndLevel;   // 8 bit value
                                // ignored when h263En = TRUE
                                // CAUTION! : the encoder is not checking for
                                // profile&level limitations
    int32 hMirror;
    int32 vMirror;
    AP_VT_ROTATE_TYPE rotate; //061215 MTS
   AP_VT_ROTATE_TYPE scalerRot; //061215 MTS
} AP_VT_VideoENC_OPTION_TYPE;

typedef struct
{
    AP_VT_VideoRECT_STRUCT dispRect;            // sx must be divisible by 8
                                                // sy must be divisible by 2

    AP_VT_Video_POST_PROCESS_TYPE postProcess;  // strength of the postprocessing filtering
                                                // Postprocessing disabled when set to zero
    int32 useCallback;                          // use callback function if TRUE

    AP_VT_VideoSIZE_STRUCT rawSize;             // size of the decoded raw data(yuv, rgb, ...)
    AP_VT_PIXEL_TYPE rawFormat;                 // format of the decoded raw data
    int32 *pRawAddr;                            // addressof the decoded raw data
    AP_VT_ROTATE_TYPE rotate;                   //061215 MTS
} AP_VT_VideoDEC_OPTION_TYPE;

typedef struct AP_VT_VIDEOENCHOST_TAG
{
    BOOLEAN IntraFrame;                         // force intra frame encode
    uint32 uiTimestamp;                         // timestamp, not used now
    uint8 *pucPacket;                           // data packet to be encoded
    int32 *piPacketLength;                      // packet length
    BOOLEAN* pzFrameEncoded  ;      
}AP_VT_VIDEOENCHOST_T;

typedef struct AP_VT_VIDEOENCHEADER_TAG
{
  	uint32 *pData;                              // video header data to be encoded
	uint32 *pLength ;     
}AP_VT_VIDEOENCHEADER_T;

typedef struct AP_VT_VIDEODECHOST_TAG
{
  	uint8 *pucPacket;                           // data to bedecoded 
	int32 *piPacketLength;                      // data length
	BOOLEAN *pzFrameDecoded;                    // none Iframe decoded indicator
	BOOLEAN *pzIFrameDecoded;     
	uint32 *perrMB;
}AP_VT_VIDEODECHOST_T;

typedef struct AP_VT_VIDEOENCMUTE_OPTION_TAG{
    AP_VT_VideoSIZE_STRUCT img_size;
    void *img_addr;
    uint32 mute_en;
    AP_VT_PIXEL_TYPE format;
}AP_VT_VIDEOENCMUTE_OPTION_T;

/*for 324 lib and streaming lib*/
/*****************************************************************************/
// 	Description : Initialize VT encoder instance and resource
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoEncInitHost(void);


/*****************************************************************************/
// 	Description : Set encoder configuration
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
BOOLEAN AP_VT_VideoEncConfHost(                  // return: success or fail
    AP_VT_VideoENC_OPTION_TYPE *pConf                         // video encoder configure option
    );

/*****************************************************************************/
// 	Description : Encode Video Header
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoEncHeaderHost( // return: success or error code
	uint32 *pData,                                                          // video header data to be encoded
	uint32 *pLength                                                       // video header data length
	);

/*****************************************************************************/
// 	Description : Encode one frame
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoEncHostDil(       // return: success or error code
	BOOLEAN IntraFrame,                                            // force intra frame encode
	uint32 uiTimestamp,                                             // timestamp, not used now
	uint8 *pucPacket,                                                 // data packet to be encoded
	int32 *piPacketLength,                                       // packet length
	BOOLEAN* pzFrameEncoded                                 // frame encoded indicator
	);

/*****************************************************************************/
// 	Description : Encode one frame
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
uint8 AP_VT_VideoEncStopHost(                         // return: success, not used now
	uint8 *pData, 
	int32 *pLength
	);

/*****************************************************************************/
// 	Description : Release encode instance and resource
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
PUBLIC AP_RET_CODE_TYPE AP_VT_VideoEncFreeHost(void);   

/*****************************************************************************/
// 	Description : Enable or disable video encode mute
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoEncMuteHostDil(
	BOOLEAN Mute_Enable                                             // enable mute
	);

/*****************************************************************************/
// 	Description : Initialize decoder instance and resource
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoDecInitHost(void);

/*****************************************************************************/
// 	Description : Set decoder configuration 
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
BOOLEAN AP_VT_VideoDecConfHost(                        // return: config success or fail
    AP_VT_VideoDEC_OPTION_TYPE *pConf                              // video decoder configure option
    );

/*****************************************************************************/
// 	Description : Set decoder configuration 
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoDecHostDil(
	uint8 *pucPacket,                                                        // data to bedecoded 
	int32 *piPacketLength,                                              // data length
	BOOLEAN *pzFrameDecoded,                                       // none Iframe decoded indicator
	BOOLEAN *pzIFrameDecoded,                                      // intra frame decoded indicator
	uint32 *perrMB
	);

/*****************************************************************************/
// 	Description : Set decoder configuration 
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoDecHostDilSimp(
	uint8 *pucPacket,                                                        // data to bedecoded 
	int32 *piPacketLength,                                              // data length
	BOOLEAN *pzFrameDecoded,                                       // none Iframe decoded indicator
	BOOLEAN *pzIFrameDecoded,                                      // intra frame decoded indicator
	uint32 *perrMB
	);

/*****************************************************************************/
// 	Description : Release decode instance and resource 
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoDecFreeHost(void);
	
/*****************************************************************************/
// 	Description : Release decode instance and resource 
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
AP_RET_CODE_TYPE AP_VT_VideoEncHostMute(AP_VT_VIDEOENCMUTE_OPTION_T* pMuteOption);

/*****************************************************************************/
// 	Description : Register vt audio device to audioservice
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
PUBLIC HAUDIODEV AP_VT_RegAudioDevice(void);

/*****************************************************************************/
// 	Description : Register vt audio encode codec  to audioservice
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
PUBLIC HAUDIOCODEC AP_VT_RegEncoderPugger(     //return codec handle
                HCOLLISION hCollision                                    //collision handle
        );

/*****************************************************************************/
// 	Description : Register vt audio decode codec  to audioservice
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/
PUBLIC HAUDIOCODEC AP_VT_RegDeCoderPugger(     //return codec handle
                HCOLLISION hCollision                                    //collision handle
        );
        
/*****************************************************************************/
// 	Description : Power on/off the specified sensor
//	Global resource dependence : none
//  Author: Shunyi Wang
//	Note:
/*****************************************************************************/        
PUBLIC AP_RET_CODE_TYPE AP_VT_SensorCtrl(int SensorID, BOOLEAN ctrl);    

PUBLIC void AP_VT_Open( void );
PUBLIC void AP_VT_Close( void );
        
/*mainly for mmi*/
/*------------------------------------------------------------------------------
    Function name   : vt init
    Return          : 
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
void AP_VT_Init( void );

/*------------------------------------------------------------------------------
    Function name   : MV_ByPassLock()
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_ByPassLock(void);

/*------------------------------------------------------------------------------
    Function name   : AP_ByPassLock()
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_ByPassUnLock(void);

/*------------------------------------------------------------------------------
    Function name   : VT_InitOsd
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_InitOsd(void);

/*------------------------------------------------------------------------------
    Function name   : VT_SetOsd
    Return          : MV_TRUE if success
    Argument        : VT OSD rect, Image is add of the buff
    Comments        : OSD SIZE == LCD SIZE, main lcd only
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN  AP_VT_ClearOsd(AP_VT_RECT_T OSD_Rect);

/*------------------------------------------------------------------------------
    Function name   : VT_SetOsd
    Return          : MV_TRUE if success
    Argument        : VT OSD rect, Image is add of the buff
    Comments        : OSD SIZE == LCD SIZE, main lcd only
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetOsd(
               AP_VT_RECT_T OSD_Rect, \
               uint16* Image,   \
               uint16 BackColor \
                );

/*------------------------------------------------------------------------------
    Function name   : VT_Osd
    Return          : 
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_OsdAble(BOOLEAN  enable);

/*------------------------------------------------------------------------------
    Function name   : VT_CloseOsd
    Return          : 
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC void AP_VT_CloseOsd(void);


/*------------------------------------------------------------------------------
    Function name   : VT_SetEncConf
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC	BOOLEAN AP_VT_SetEncConf(AP_VT_VideoENC_OPTION_TYPE * vt_EncConf);

/*------------------------------------------------------------------------------
    Function name   : VT_GetEncConf
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC	BOOLEAN AP_VT_GetEncConf(AP_VT_VideoENC_OPTION_TYPE * vt_EncConf);

/*------------------------------------------------------------------------------
    Function name   : VT_SetDecConf
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC	BOOLEAN AP_VT_SetDecConf(AP_VT_VideoDEC_OPTION_TYPE * vt_DecConf);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_GetDecConf
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC	BOOLEAN AP_VT_GetDecConf(AP_VT_VideoDEC_OPTION_TYPE * vt_DecConf);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_SetDispMode
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : dispmode and rect
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetDispMode(AP_VT_DISP_MODE_E dispmode, AP_VT_RECT_T me_rect, AP_VT_RECT_T remote_rect);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_SetDispMode
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : dispmode and rect
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetVideoMode(AP_VT_VIDEO_MODE_E VideoMode);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_CircleMe
    Return          : MV_TRUE if success
    Argument        : 
    Comments        :mirror type
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_CircleMe(AP_VT_CIRCLE_ME_E Mirror_type);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_RotateEnc
    Return          :  
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_RotateEnc(AP_VT_ROTATE_TYPE Rotate_type);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_RotateDec
    Return          :
    Argument        : 
    Comments        :
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_RotateDec(AP_VT_ROTATE_TYPE Rotate_type);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_SetLocalPos
    Return          : MV_TRUE if success
    Argument        : 
    Comments        :mirror type
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetEncPos(AP_VT_VideoRECT_STRUCT* video_rect);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_SetLRemotePos
    Return          : MV_TRUE if success
    Argument        : 
    Comments        :mirror type
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetLDecPos(AP_VT_VideoRECT_STRUCT* video_rect);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_ZoomMe
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : zoom level
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_ZoomMe(BOOLEAN zoom_type);

/*------------------------------------------------------------------------------
    Function name   : VT_SetEncodeRate
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : OSD SIZE == LCD SIZE, main lcd only
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetEncodeRate(AP_VT_ENC_LEVEL_E encLevel);

/*------------------------------------------------------------------------------
    Function name   : VT_SetEncQuality
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : OSD SIZE == LCD SIZE, main lcd only
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetEncodeQuality(AP_VT_VideoPROFILE_AND_LEVEL_TYPE encrate);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_SetBypass
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetBypass(BOOLEAN is_bypass);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_PreviewStart
    Return          : MV_TRUE if success
    Argument        : Not used 
    Comments        : Indicates MMI starting preview
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_PreviewStart(AP_VT_RECT_T Preview_Rect,
                    AP_VT_RECT_T OSD_Rect, 
                    uint16* OSD_Image, 
                    uint16 BackColor);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_PreviewStop
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : Indicates MMI stop preview
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_PreviewStop(void);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_PreviewSetOsd
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : OSD_Rect:preview rect,OSD_Image:image of the osd,BackColor全透色
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN  AP_VT_PreviewSetOsd(AP_VT_RECT_T OSD_Rect, uint16* OSD_Image, uint16 BackColor);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_PreviewClearOsd
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : OSD_Rect: clear rect
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN  AP_VT_PreviewClearOsd(AP_VT_RECT_T OSD_Rect);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_PreviewOsdAble
    Return          : 
    Argument        : 
    Comments        : 
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_PreviewOsdAble(BOOLEAN  enable);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_SetBright
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : mode:true为增加false为减少
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetBright(BOOLEAN mode);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_SetContrast
    Return          : MV_TRUE if success
    Argument        : 
    Comments        : mode:true为增加false为减少
------------------------------------------------------------------------------*/
PUBLIC BOOLEAN AP_VT_SetContrast(BOOLEAN mode);

/*------------------------------------------------------------------------------
    Function name   : AP_VT_Preview
    Return          : AP_RET_SUCCESS if success
    Argument        : 
    Comments        : For MMI to continue VT encoding preview. to avoid conflict with 324 VT
                             encoding
------------------------------------------------------------------------------*/
PUBLIC AP_RET_CODE_TYPE AP_VT_Preview(// return: success or error code
	BOOLEAN IntraFrame, /*lint !e728*/ // force intra frame encode
	uint32 uiTimestamp, // timestamp, not used now
	uint8 *pucPacket,   // data packet to be encoded
	int32 *piPacketLength, // packet length
	BOOLEAN* pzFrameEncoded /*lint !e728*/ // frame encoded indicator    
	);
        
void AP_LcdSensorInit(void);

        
#endif /*DAL_VT_H*/











