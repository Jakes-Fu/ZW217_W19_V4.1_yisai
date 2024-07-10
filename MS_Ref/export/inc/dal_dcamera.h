/*******************************************************************************
 ** FileName:      dal_dcamera.h                                             *
 ** Author:        Zhemin.Lin&Benny.Zhou                                       *
 ** Date:          2005/8/04                                                   *
 ** CopyRight:     2005, Spreatrum, Incoporated, All right reserved            *
 ** Description:   define DCAM interface                                       *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2005/8/04      Zhemin.Lin&Benny.Zhou      Created                          *
 ** 2006/4/12      Liangwen.Zhen              Modify                           *
 ** 2007/3/12      Liangwen.Zhen              Modify interface                 *
 ** 2007/4/28      Frank.Yang                 Modify interface                 *
 *******************************************************************************/

#ifndef _DAL_DCAMERA_H_
#define _DAL_DCAMERA_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

#include "sci_types.h"
#include "dal_lcd.h"

/**---------------------------------------------------------------------------*
 **                         Macro definition                                  *
 **---------------------------------------------------------------------------*/
#define DCAMERA_SENSOR_NUMBER                   0x03
#define DCAMERA_BURST_SNAPSHOT_IMAGE_MAX        0x09
#define DCAMERA_TARGET_FRAME_MAX                0x08

#define DCAMERA_AUTO_FOCUS_STATUS_SUCCESS       0x00
#define DCAMERA_AUTO_FOCUS_STATUS_PROCESSING    0x01
#define DCAMERA_AUTO_FOCUS_STATUS_FAIL          0xFF

#define DCAMERA_OSD_RECT_NUM 0x08

typedef uint32 (*DCAMERA_CALLBACK)(void* param_ptr);
/**---------------------------------------------------------------------------*
 **                         Data Structure definition                         *
 **---------------------------------------------------------------------------*/
 // enum: return value
typedef enum
{
  DCAMERA_OP_SUCCESS = SCI_SUCCESS,
  DCAMERA_OP_ERROR,
  DCAMERA_OP_PARAM_ERR,
  DCAMERA_OP_NO_SENSOR_ERR,
  DCAMERA_OP_SENSOR_NOT_WORK_ERR,
  DCAMERA_OP_PREVIEW_ERR,
  DCAMERA_OP_IOCTL_ERR,
  DCAMERA_OP_SNAPSHOT_ERR,
  DCAMERA_OP_GET_SNAPSHOT_DATA_ERR,
  DCAMERA_OP_NO_ENOUGH_MEMORY,
  DCAMERA_OP_REVIEW_ERR,
  DCAMERA_OP_ISP_ERR,
  DCAMERA_OP_ENCODE_ERR,
  DCAMERA_OP_DECODE_ERR,
  DCAMERA_OP_NOT_OPEN,    
  DCAMERA_OP_FFS_READ_ERR,
  DCAMERA_OP_FFS_WRITE_ERR,
  DCAMERA_OP_ROTATION_ERR,
  DCAMERA_OP_MEMORY_ERR,
  
  DCAMERA_OP_MAX = 0xFF

}DCAMERA_RETURN_VALUE_E;

typedef enum
{
    DCAMERA_OP_NORMAL=0x01,
    DCAMERA_OP_DV,
    DCAMERA_OP_VT,
    DCAMERA_OP_ATV,
    DCAMERA_OP_ATV_FACTORY,
    DCAMERA_OP_SPECIAL,
    DCAMERA_OP_MINIDC,
    DCAMERA_OP_MINIDV,
    DCAMERA_OP_UPCC,
    DCAMERA_OP_VIDEOCALL,
    DCAMERA_OP_ERR
}DCAMERA_OP_MODE_E;

typedef enum
{
    DCAMERA_IMG_SENSOR_TYPE_4P3=0x00,
    DCAMERA_IMG_SENSOR_TYPE_3P4,
    DCAMERA_IMG_SENSOR_TYPE_EQ,
    DCAMERA_IMG_SENSOR_TYPE_MAX
}DCAMERA_IMG_SENSOR_TYPE_E;

typedef enum
{
    DCAMERA_CALLBACK_ATV_SCAN_CHN=0x01,
    DCAMERA_CALLBACK_ATV_SCAN_CHN_END,
    DCAMERA_CALLBACK_ATV_SCAN_CHN_STOP,
    DCAMERA_CALLBACK_ATV_RSSI,
    DCAMERA_CALLBACK_ATV_DECT_REGION,
    DCAMERA_CALLBACK_ATV_ERR,
    DCAMERA_CALLBACK_MAX
} DCAMERA_CALLBACK_CMD_E;

// enum: digital camera get information command
typedef enum
{
    DCAMERA_INFO_NONE = 0x00,
    DCAMERA_INFO_BRIGHTNESS,
    DCAMERA_INFO_CONTRAST,
    DCAMERA_INFO_SHARPNESS,
    DCAMERA_INFO_SATURATION,
    DCAMERA_INFO_ZOOM,
    DCAMERA_INFO_EFFECT,
    DCAMERA_INFO_PRVIEW_MODE,   
    DCAMERA_INFO_WB_MODE,   
    DCAMERA_INFO_BURST_PERMIT_NUM,
    DCAMERA_INFO_SENSOR_SIZE,
    DCAMERA_INFO_ISO,
    DCAMERA_INFO_EXPOSURE_COMPENSATION,
    DCAMERA_INFO_BANDING_FLICKER_COMPENSATION,
    DCAMERA_INFO_VIDEO_MODE,
    DCAMERA_INFO_ATV_CH_ALL_NUM,
    DCAMERA_INFO_ATV_RSSI,
    DCAMERA_INFO_FEATURE_SUPPORT,
    DCAMERA_INFO_IMG_SENSOR_NUM,
    DCAMERA_INFO_CUR_IMG_SENSOR,
    DCAMERA_INFO_MAX_SENSOR_SIZE,
    DCAMERA_INFO_PANORAMA_MAX_NUM,
    DCAMERA_INFO_SENSOR_TYPE,
    DCAMERA_INFO_FOCUS_MODE,
    DCAMERA_INFO_EXPOSURE_MODE,
    DCAMERA_INFO_THUMBNAIL_SIZE,
    DCAMERA_INFO_PREVIEW_ROT,
    DCAMERA_INFO_JPEG_SIZE,    
    DCAMERA_INFO_DISP_RECT,
    DCAMERA_INFO_UPCC_SIZE,
    DCAMERA_INFO_UPCC_ROTATION,
    DCAMERA_INFO_DC_PREVIEW_ROTATION,
    DCAMERA_INFO_DC_CAP_ROTATION,
    DCAMERA_INFO_MAX
}DCAMERA_INFO_CMD_E;

typedef enum
{
    DCAMERA_ATV_CHN_NORMAL_OP=0x00,
    DCAMERA_ATV_CHN_NOLY_SET,
    DCAMERA_ATV_CHN_NOLY_DECT,
    DCAMERA_ATV_CHN_MAX
}DCAMERA_ATV_CHN_OP_MODE_E;

typedef enum
{
    DCAMERA_IMG_SENSOR_MAIN,
    DCAMERA_IMG_SENSOR_SUB,
    DCAMERA_IMG_SENSOR_MAX
}DCAMERA_IMG_SENSOR_MODE;

typedef enum
{
    DCAMERA_FLASH_OFF = 0x00,   
    DCAMERA_FLASH_ON,
    DCAMERA_FLASH_AUTO,
    DCAMERA_FLASH_NONE,
        
    DCAMERA_FLASH_MAX   
}DCAMERA_FLASH_MODE_E;

// enum: digital camera parameter control command
typedef enum
{
    DCAMERA_PARAM_CTRL_NONE = 0x00, 
    DCAMERA_PARAM_CTRL_BRIGHTNESS,                  // Set brightness CMD
    DCAMERA_PARAM_CTRL_CONTRAST,                    // Set contrast CMD
    DCAMERA_PARAM_CTRL_SHARPNESS,                   // Set sharpness CMD
    DCAMERA_PARAM_CTRL_SATURATION,                  // Set saturation CMD
    DCAMERA_PARAM_CTRL_ZOOM,                        // Set zoom (window and size) CMD
    DCAMERA_PARAM_CTRL_EFFECT,                      // Set special effect CMD
    DCAMERA_PARAM_CTRL_PREVIEW_MODE,                // Set preview mode CMD
    DCAMERA_PARAM_CTRL_SET_TARGET_SIZE,             // Set target size CMD
    DCAMERA_PARAM_CTRL_CHECK_TARGET_SIZE,           // Check target size CMD
    DCAMERA_PARAM_CTRL_WB_MODE,                     // Set White balance CMD
    DCAMERA_PARAM_CTRL_AWB_CAL_ENABLE,              // Reserved
    DCAMERA_PARAM_CTRL_ISO,                         // Set ISO CMD
    DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION,       // Set Exposure compensation CMD
    DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION,        // 50/60HZ BANDING FLICKER COMPENSATION 
    DCAMERA_PARAM_CTRL_VIDEO_MODE,                  // DC/DC MODE Tim.zhu@20081013 for video mode
    DCAMERA_PARAM_CTRL_ATV_CHN,                     // For Analog TV use Select Channel
    DCAMERA_PARAM_CTRL_ATV_REGION,                  // For Analog TV use Select Region
    DCAMERA_PARAM_CTRL_ATV_VOLUME,                  // For Analog TV use Select Volume
    DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE,             // Select sensor : Main or Sub(Rear or Front)
    DCAMERA_PARAM_CTRL_BG_COLOR,                    // set osd background color
    DCAMERA_PARAM_CTRL_FLASH,                       // dc flash ctrl
    DCAMERA_PARAM_CTRL_FOCUS,                       // dc sensor focus ctrl
    DCAMERA_PARAM_CTRL_EXPOSURE,                    // dc esposure area ctrl
    DCAMERA_PARAM_CTRL_CAPTURE_MODE,                // Reserved
    DCAMERA_PARAM_CTRL_PANORAMA_DIR,                // Reserved
    DCAMERA_PARAM_CTRL_PANORAMA_NUM,                // Reserved
    DCAMERA_PARAM_CTRL_METER_MODE,
    DCAMERA_PARAM_CTRL_ROT_MODE,
    DCAMERA_PARAM_CTRL_FLASH_LIGHT,                 // flash light
    DCAMERA_PARAM_CTRL_TESTPATTERN_MODE,

    DCAMERA_PARAM_CTRL_MAX                          // Reserved
}DCAMERA_PARAM_CTRL_CMD_E;

/* callback for parameter control */
typedef uint32 (* DCAMERA_PARAM_CTRL_CALL_BACK)(void* param_ptr, uint32 param_size);

// enum: banding flicker elimination type //Tim.zhu@20080504 modify for cr114537 banding flicker elimination
typedef enum
{
    DCAMERA_FLICKER_50HZ = 0x00,
    DCAMERA_FLICKER_60HZ,   
    DCAMERA_FLICKER_MAX
}DCAMERA_PARAM_FLICKER_E;


// enum: ev level //Tim.zhu@20080421 modify for cr114537 iso set
typedef enum
{
    DCAMERA_EV_00 = 0x00,
    DCAMERA_EV_01,          
    DCAMERA_EV_02,                  
    DCAMERA_EV_03,              
    DCAMERA_EV_04,
    DCAMERA_EV_05,
    DCAMERA_EV_06,
    DCAMERA_EV_MAX
}DCAMERA_PARAM_EV_E;

typedef enum
{
    DCAMERA_METER_MODE_FULL = 0,
    DCAMERA_METER_MODE_CENTER,        
    DCAMERA_METER_MODE_SPOT,
    DCAMERA_METER_MODE_MATRIX,
    DCAMERA_METER_MODE_MAX = 0xFF
}DCAMERA_PARAM_METER_MODE_E;

// enum: preview effect type
typedef enum
{
    DCAMERA_EFFECT_NORMAL = 0x00,
    DCAMERA_EFFECT_BLACKWHITE,          // Black or White  // Grey
    DCAMERA_EFFECT_RED,                 // Red effect
    DCAMERA_EFFECT_GREEN,               // Green effect
    DCAMERA_EFFECT_BLUE,                // Blue effect
    DCAMERA_EFFECT_YELLOW,              // Yellow effect
    DCAMERA_EFFECT_NEGATIVE,            // Negative effect
    DCAMERA_EFFECT_CANVAS,              // Speia
#ifdef MMI_VER_STABLE
    DCAMERA_EFFECT_GRAY,
    DCAMERA_EFFECT_EMBOSS,
    DCAMERA_EFFECT_SKETCH,
#endif
    DCAMERA_EFFECT_RELIEVOS,            // 
    DCAMERA_EFFECT_AQUA,
    DCAMERA_EFFECT_MAX
}DCAMERA_PARAM_EFFECT_E;

// enum: preview enviroment type
typedef enum
{
    DCAMERA_ENVIRONMENT_NORMAL = 0x00,  // 
    DCAMERA_ENVIRONMENT_NIGHT,          // 
    DCAMERA_ENVIRONMENT_SUNNY,          // 
    DCAMERA_ENVIRONMENT_SPORTS,         // 
    DCAMERA_ENVIRONMENT_LANDSCAPE,      //
    DCAMERA_ENVIRONMENT_PORTRAIT,       // 
    DCAMERA_ENVIRONMENT_PORTRAIT_NIGHT, // 
    DCAMERA_ENVIRONMENT_BACKLIGHT,      //
    DCAMERA_ENVIRONMENT_MACRO,          // 
    DCAMERA_ENVIRONMENT_TEST_PATTERN,   //

    DCAMERA_ENVIRONMENT_MANUAL = 30,    // 
    DCAMERA_ENVIRONMENT_AUTO   = 31,    // 
    
    DCAMERA_ENVIRONMENT_MAX
}DCAMERA_PARAM_ENVIRONMENT_E;

// enum: preview while balance mode
typedef enum
{
    DCAMERA_WB_MODE_AUTO = 0x00,        //Auto
    DCAMERA_WB_MODE_INCANDESCENCE,      //Incandescent light
    DCAMERA_WB_MODE_U30,                //U30 light
    DCAMERA_WB_MODE_CWF,                //CWF light
    DCAMERA_WB_MODE_FLUORESCENT,        //Fluorescent light
    DCAMERA_WB_MODE_SUN,                //Sunny
    DCAMERA_WB_MODE_CLOUD,              //Cloudy
    DCAMERA_WB_MODE_MAX
}DCAMERA_PARAM_WB_MODE_E;

// enum: ISO mode
typedef enum
{
    DCAMERA_ISO_MODE_AUTO = 0x00,
    DCAMERA_ISO_MODE_LOW,
    DCAMERA_ISO_MODE_MID,
    DCAMERA_ISO_MODE_HIGH,
    DCAMERA_ISO_MODE_MAX
    
}DCAMERA_PARAM_ISO_MODE_E;

// enum: VIDEO mode
typedef enum
{
    DCAMERA_NORMAL_MODE = 0x00,
    DCAMERA_VIDEO_MODE,
    DCAMERA_UPCC_MODE,
    DCAMERA_MODE_MAX
}DCAMERA_PARAM_VIDEO_MODE_E;

typedef enum
{
#ifdef MMI_VER_MAIN
    DCAMERA_PHOTO_SIZE_80X80=0x00,
#endif
#ifdef MMI_VER_STABLE
    DCAMERA_PHOTO_SIZE_128X96=0x00,
#endif
	DCAMERA_PHOTO_SIZE_128X160,
	DCAMERA_PHOTO_SIZE_160X128,
    DCAMERA_PHOTO_SIZE_176X220,
    DCAMERA_PHOTO_SIZE_220X176,
    DCAMERA_PHOTO_SIZE_240X320,
    DCAMERA_PHOTO_SIZE_320X240,
    DCAMERA_PHOTO_SIZE_352X288,
    DCAMERA_PHOTO_SIZE_240X400,
    DCAMERA_PHOTO_SIZE_400X240,
    DCAMERA_PHOTO_SIZE_320X480,
    DCAMERA_PHOTO_SIZE_480X320,
    DCAMERA_PHOTO_SIZE_480X640,
    DCAMERA_PHOTO_SIZE_640X480,
    DCAMERA_PHOTO_SIZE_720X480,
    DCAMERA_PHOTO_SIZE_720X576,
    DCAMERA_PHOTO_SIZE_800X600,
    DCAMERA_PHOTO_SIZE_1024X768,
    DCAMERA_PHOTO_SIZE_960X1280,
    DCAMERA_PHOTO_SIZE_1280X960,
    DCAMERA_PHOTO_SIZE_1200X1600,//Add by baiyu for vertical
    DCAMERA_PHOTO_SIZE_1600X1200,
    DCAMERA_PHOTO_SIZE_2048X1536,
    DCAMERA_PHOTO_SIZE_2592X1944,
    DCAMERA_PHOTO_SIZE_3264X2448,
    DCAMERA_PHOTO_SIZE_MAX
}DCAMERA_PARAM_PHOTO_SIZE_E;

// enum: display disp mode
typedef enum
{
    DCAMERA_DISP_FULLVIEW=0x00, // 图像按照比例关系，不进行Trim显示到target_rect内部
    DCAMERA_DISP_FULLSCREEN,    // 按照比例，并进行Trim显示到target_rect，并充满这个Rect
    DCAMERA_DISP_CUSTOMIZE,     // 图像直接缩放到target_rect，并充满这个Rect?
    DCAMERA_DISP_MODE_MAX
}DCAMERA_DISP_MODE_E;

// enum: capture mode
typedef enum
{
    DCAMERA_CAPTURE_NORMAL  = 0x00,
    DCAMERA_CAPTURE_PANORAMA,
    DCAMERA_CAPTURE_FOURINONE,
    DCAMERA_CAPTURE_THREEINONE,
    DCAMERA_CAPTURE_MAX             
}DCAMERA_CAPTURE_MODE_E;

typedef enum 
{
    DC_PANORAMA_HORIZONTAL_LEFT = 0, 
    DC_PANORAMA_HORIZONTAL_RIGHT,
    DC_PANORAMA_DIRECTION_MAX
} DCAMERA_PANORAMA_DIR_E;

// enum: display rotation mode
typedef enum
{
    DCAMERA_ROTATION_0  = 0x00,         // Normal
    DCAMERA_ROTATION_90,                // Clockwise rotation 90 degree 
    DCAMERA_ROTATION_180,               // Clockwise rotation 180 degree 
    DCAMERA_ROTATION_270,               // Clockwise rotation 270 degree 
    DCAMERA_ROTATION_MIRROR,            // Mirror
    DCAMERA_ROTATION_FLIP,              // horizontal mirror;
    DCAMERA_ROTATION_MIRROR_FLIP,
    DCAMERA_ROTATION_MAX

}DCAMERA_ROTATION_MODE_E;

typedef enum
{
    DC_CONTINUE_SHOOT_DISABLE = 0,
    DC_CONTINUE_SHOOT_3,
    DC_CONTINUE_SHOOT_6,
    DC_CONTINUE_SHOOT_9,
    DC_CONTINUE_SHOOT_MAX
}DC_CONTINUE_SHOOT_E;

// enum: data type
typedef enum
{
    DCAMERA_DATA_TYPE_JPEG = 0,
    DCAMERA_DATA_TYPE_YUV422,
    DCAMERA_DATA_TYPE_YUV420,
    DCAMERA_DATA_TYPE_RGB565,  //BMP to RGB to avoid conflict
    DCAMERA_DATA_TYPE_RAW,
    DCAMERA_DATA_TYPE_MAX
}DCAMERA_DATA_TYPE_E;

typedef enum
{
    DCAMERA_YUV_YUYV = 0,      
    DCAMERA_YUV_YVYU,        
    DCAMERA_YUV_UYVY,           
    DCAMERA_YUV_VYUY,     
    DCAMERA_YUV_TWO_PLANE,        
    DCAMERA_YUV_THREE_PLANE,

    DCAMERA_YUV_MAX
}DCAMERA_YUV_PATTERN_E;

typedef enum
{
    DCAMERA_FOCUS_NONE=0x00,
    DCAMERA_FOCUS_TRIG,
    DCAMERA_FOCUS_ZONE,
    DCAMERA_FOCUS_MAX
}DCAMERA_FOCUS_CMD_E;

typedef enum
{
    DCAMERA_EXPOSURE_NONE=0x00,
    DCAMERA_EXPOSURE_AUTO,
    DCAMERA_EXPOSURE_ZONE,
    DCAMERA_EXPOSURE_MAX
}DCAMERA_EXPOSURE_CMD_E;

typedef enum
{
    DCAMERA_INVALIDATEMODE_ISP=0x00,
    DCAMERA_INVALIDATEMODE_EXTERN,
    DCAMERA_INVALIDATEMODE_NONE
  
}DCAMERA_INALIDATEMODE_E;

typedef union 
{
    DCAMERA_YUV_PATTERN_E   yuv_pattern;
}DCAMERA_DATA_PATTERN_T;


typedef enum
{
    DCAMERA_IMAGE_ROTATION_TYPE_JPEG = 0x00,    
    DCAMERA_IMAGE_ROTATION_TYPE_YUV420,
    DCAMERA_IMAGE_ROTATION_TYPE_RGB565,
        
    DCAMERA_IMAGE_ROTATION_TYPE_MAX 
}DCAMERA_IMAGE_ROTATION_TYPE_E;

typedef enum
{
    DC_UPCC_SIZE_160X128,
    DC_UPCC_SIZE_320X240,
    DC_UPCC_SIZE_240X320,
    DC_UPCC_SIZE_MAX
}DC_UPCC_SIZE_E;

// Rectangle Structure
typedef struct
{
    uint16 x;                                       // x offset of rectangle
    uint16 y;                                       // y offset of rectangle
    uint16 w;                                       // width of rectangle
    uint16 h;                                       // height of rectangle
}DCAMERA_RECT_T, *DCAMERA_RECT_T_PTR;

// Rectangle Structure
typedef struct
{
    uint16 w;                                       // width of rectangle
    uint16 h;                                       // height of rectangle
}DCAMERA_SIZE_T, *DCAMERA_SIZE_T_PTR;

typedef union
{
    DCAMERA_PARAM_PHOTO_SIZE_E      size_type;
    DCAMERA_ROTATION_MODE_E         cap_rot;
    DCAMERA_ROTATION_MODE_E         review_rot;
}DCAMERA_BURST_INPUT_T, *DCAMERA_BURST_INPUT_T_PTR;

//dcamera size ,angle, and burst image information
typedef struct
{
    DCAMERA_BURST_INPUT_T      get_info_input;
    uint32                                  img_burst_cnt;
}DCAMERA_BURST_INFO_U, *DCAMERA_BURST_INFO_U_PTR;

typedef struct
 {
    uint32 y_addr;
    uint32 uv_addr;
    uint32 v_addr;
 }DCAMERA_DATA_ADDR_T, *DCAMERA_DATA_ADDR_T_PTR;

typedef struct
 {
    uint32 y_len;
    uint32 uv_len;
    uint32 v_len;
 }DCAMERA_DATA_LEN_T, *DCAMERA_DATA_LEN_T_PTR;

// background color Structure
typedef struct
{
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 reserve;
}DCAMERA_DISP_BG_COLOR_T, *DCAMERA_DISP_BG_COLOR_T_PTR;

typedef struct
{
    uint32 num;
    DCAMERA_RECT_T osd_rect[DCAMERA_OSD_RECT_NUM]; // physic point
}DCAMERA_OSD_RECT_T, *DCAMERA_OSD_RECT_T_PTR;

typedef struct
{
    uint32 param;
    DCAMERA_RECT_T lcd_point;
    DCAMERA_CALLBACK set_osd_rect;
}DCAMERA_PARAM_CTRL_T, *DCAMERA_PARAM_CTRL_T_PTR;

typedef struct
{
    DCAMERA_SIZE_T src_size;
    DCAMERA_DATA_ADDR_T src;
    DCAMERA_RECT_T src_buf_rect;
}DCAMERA_YUV_SRC_T, *DCAMERA_YUV_SRC_T_PTR;

typedef struct
{
    DCAMERA_PARAM_METER_MODE_E        meter_mode;   
    DCAMERA_RECT_T                    meter_rect;
}DCAMERA_PARAM_METER_T;

typedef struct
{
    uint32 cmd;
    uint32 atv_chn_is_valid;
    uint32 atv_chn_id;
    uint32 atv_rssi;
    uint32 rtn;
} DC_ATV_CALLBACK_T, *DC_ATV_CALLBACK_T_PTR;

typedef uint32 (*DCAMERA_ATV_CALLBACK)(DC_ATV_CALLBACK_T_PTR param_ptr);

typedef struct
{
    DCAMERA_DATA_TYPE_E format;
    DCAMERA_DATA_ADDR_T data_addr;
    DCAMERA_DATA_LEN_T data_len;
    DCAMERA_SIZE_T size;
}DCAMERA_CALLBACK_DATA_T, *DCAMERA_CALLBACK_DATA_T_PTR;

typedef struct
{
    BOOLEAN eb;
    BOOLEAN reserve2;
    BOOLEAN reserve1;
    BOOLEAN reserve0;
    DCAMERA_DATA_TYPE_E format;
    DCAMERA_RECT_T trim_size;
    DCAMERA_SIZE_T target_size;
    DCAMERA_CALLBACK callback;
}DCAMERA_GET_DATA_T, *DCAMERA_GET_DATA_T_PTR;

// Preview Structure
typedef struct
{
    LCD_ID_E                        lcd_id;             // lcd id define in dal_lcd_sc6800.h
    uint32                          lcd_block_id;       // lcd block id: 0~5
    DCAMERA_RECT_T                  disp_rect;          // display size on lcd
    DCAMERA_RECT_T                  target_rect;        // target display rect as display mode is customize 
    DCAMERA_ROTATION_MODE_E         rotation_mode;      // 0: normal; 1: 90; 2: 180; 3:270;
    DCAMERA_DISP_MODE_E             disp_mode;
    DCAMERA_CAPTURE_MODE_E          capture_mode;
    DCAMERA_PANORAMA_DIR_E          panorama_dir;
    DCAMERA_ATV_CALLBACK            dc_atv_callback;
}DCAMERA_PREVIEW_PARAM_T;

typedef struct
{
    uint32      select_personal;                        // 1: select personal snapshot; 0: ingore below param
    
    uint16*     image_buffer;       
    uint16      image_width;    
    uint16      image_height;   
    
    uint16*     photo_buffer;
    uint16      photo_offset_x;
    uint16      photo_offset_y;
    uint16      photo_width;
    uint16      photo_height;

}DCAMERA_PERSONAL_SNAPSHOT_INFO_T;

// Digital camera call back function define
typedef uint32 (* DCAMERA_CALL_BACK)(DCAMERA_RETURN_VALUE_E ret_value, void* param_ptr, uint32 param_size);
typedef uint32 (* DCAMERA_COMBINE_BACK)(DCAMERA_RETURN_VALUE_E ret_value, void* param_ptr, DCAMERA_RECT_T* param_rect);

typedef struct
{
    BOOLEAN eb;
    BOOLEAN reserve2;
    BOOLEAN reserve1;
    BOOLEAN reserve0;
    DCAMERA_DATA_ADDR_T osd_addr;       // y_addr argb888
    DCAMERA_RECT_T overlay_rect;
    DCAMERA_CALLBACK date_update;       // update date
    uint32 color_key;                   // reserve
    uint32 osd_offset;                  // reserve

    DCAMERA_DATA_ADDR_T thumb_osd_addr; // y_addr argb888
    DCAMERA_RECT_T thumb_overlay_rect;
    DCAMERA_CALLBACK thumb_date_update; // update date
    uint32 thumb_color_key;             // reserve
    uint32 thumb_osd_offset;            // reserve
    
}DCAMERA_CAPTURE_DATE_T, *DCAMERA_CAPTURE_DATE_T_PTR;

typedef struct _osd_pos_t
{
    DCAMERA_RECT_T          osd_pos_t;
    struct _osd_pos_t       *next;
}
DCAMERA_OSD_POS_T;

typedef DCAMERA_OSD_POS_T *(*DCAMERA_QUICK_VIEW_CALL_BACK)(void);

// Snapshot Structure
typedef struct
{
    DCAMERA_DATA_TYPE_E     data_type;              // data type of snapshot
    uint8*                  data_buf;               // use to save jpeg data
    uint32                  buf_len;                // lenght of buffer
    uint16                  *file_name;             // file name in Unicode (relative direction)
    uint16                  enc_quality_level;      // quality level of encode jpeg: 0~8
    uint16                  flash_enable;           // 1: enable flash; 0: disable flash   
    DCAMERA_ROTATION_MODE_E rotation_mode;
    DCAMERA_CALL_BACK       capture_start_callback; // call back function when start capture
    DCAMERA_CALL_BACK       snapshot_callback;      // call back function for snapshot param1: data buffer address; param2: data size
    DCAMERA_QUICK_VIEW_CALL_BACK     quickview_callback;
    DCAMERA_PERSONAL_SNAPSHOT_INFO_T personal_snapshot;
    DCAMERA_COMBINE_BACK             combine_callback;
    DCAMERA_CAPTURE_DATE_T  capture_date;
    DCAMERA_GET_DATA_T get_data;
}DCAMERA_SNAPSHOT_PARAM_T;

typedef struct
{   
    uint8*      return_data_addr;               // return data address
    uint32      return_data_len;                // return data lenght
}DCAMERA_SNAPSHOT_RETURN_PARAM_T;

// Burst-Snapshot Structure
typedef struct
{   
    DCAMERA_DATA_TYPE_E data_type;              // data type of snapshot
    uint8  *data_buf;                           // use to save jpeg data
    uint32 buf_len;                             // lenght of buffer
    uint16 *file_name;                          // file name in Unicode (relative direction)   
    uint16 enc_quality_level;                   // quality level of encode jpeg: 0~9
    uint16 flash_enable;                        // 1: enable flash; 0: disable flash       
    uint16 image_count;                         // count of images
    uint16 interval_of_frame;                   // Unit: ms
    DCAMERA_ROTATION_MODE_E rotation_mode;
    DCAMERA_CAPTURE_DATE_T capture_date;
    DCAMERA_CALL_BACK   burst_capture_start_callback;
    DCAMERA_CALL_BACK burst_snapshot_callback;// call back function for burst snapshot  data buffer address; param2: data size
    DCAMERA_QUICK_VIEW_CALL_BACK quickview_callback;
    DCAMERA_GET_DATA_T get_data;
}DCAMERA_BURST_SNAPSHOT_PARAM_T;

typedef struct
{
    uint8 *         return_data_addr[DCAMERA_BURST_SNAPSHOT_IMAGE_MAX];     // JPEG return address
    uint32          return_data_len[DCAMERA_BURST_SNAPSHOT_IMAGE_MAX];      // JPEG return lenght
    
}DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T;

// Review Structure
typedef struct
{
    LCD_ID_E                    lcd_id;             // lcd id define in dal_lcd_sc6800.h
    uint32                      lcd_block_id;       // lcd block id: 0~5
    DCAMERA_RECT_T              disp_rect;          // display size on lcd
    uint8*                      jpeg_buffer;        // use to save jpeg data    
    uint32                      jpeg_len;           // lenght of buffer
    uint16*                     file_name;          // file name in Unicode (relative direction)    
    DCAMERA_RECT_T              target_rect;        // rectangle of jpeg that will show on lcd  
    DCAMERA_ROTATION_MODE_E     rotation_mode;      // 0: normal; 1: 90; 2: 180; 3:270;
    DCAMERA_DISP_MODE_E         disp_mode;          // 0: normal(no distortion); 1:proportion distortion; 2: trimming distortion    
    DCAMERA_CALL_BACK           review_callback;    // call back function for review JPEG, don't care param1 and param2 
}DCAMERA_REVIEW_PARAM_T;


// Auto Focus Structure
typedef uint32 (*DCAMERA_AF_CALLBACK_FUNC)(uint32);     // input value(0x00): successful;(0x01):processing, (0xff) focus fail
typedef struct
{
    uint32                      enable_af;              // 0: disable auto focus, ingore below param; 1: enable auto focus
    uint32                      af_single;              // 1: single time to focus ; 0: continues to focus
    DCAMERA_RECT_T              af_rect;                // auto focus rectangle
    DCAMERA_AF_CALLBACK_FUNC    af_callback_func;       
}DCAMERA_AF_INFO_T;


// Record Structure
typedef struct
{           
    DCAMERA_DATA_TYPE_E             out_put_data_type;  // data type of record output
    uint32                          enc_quality_level;  // jpeg encode quality
    uint8*                          data_buf_ptr;       // pointer of data buffer
    uint32                          data_buf_len;       // lenght  of data buffer
    DCAMERA_RECT_T                  target_size;        // image rectangle      
    DCAMERA_CALL_BACK               record_callback;    // param_ptr: data address; param_value: data size
    DCAMERA_ROTATION_MODE_E         rotation_mode;      // 0: normal; 1: 90; 2: 180; 3:270;
}DCAMERA_RECORD_PARAM_T;

typedef struct
{
    uint32 chn1_addr;    
    uint32 chn2_addr;    
    uint32 chn3_addr;    
}DCAMERA_ADDR_T;

typedef struct
{
    uint32 y_chn_addr;  // Y data channel address 
    uint32 u_chn_addr;  // U data channel address
    uint32 v_chn_addr;  // V data channel address
}DCAMERA_YUV_CHN_ADDR_T;

typedef struct
{
    DCAMERA_SIZE_T                  target_size;
    DCAMERA_ROTATION_MODE_E         rotation_mode;      // 0: normal; 1: 90; 2: 180; 3:270;
    DCAMERA_DATA_TYPE_E             data_type;  
    DCAMERA_DATA_PATTERN_T          data_pattern;   
} DCAMERA_IMAGE_FRAME_PARAM_T;


typedef struct
{
    uint32                  yaddr;
    uint32                  uaddr;
    uint32                  vaddr;    
    uint16                  width;
    uint16                  height;
    DCAMERA_DATA_TYPE_E     type;
    DCAMERA_YUV_PATTERN_E   pattern;
} DCAMERA_DISPALY_PARAM_T;


typedef struct
{
    DCAMERA_IMAGE_FRAME_PARAM_T     frame_info_t[DCAMERA_TARGET_FRAME_MAX];
    DCAMERA_ADDR_T                  dcamera_addr[DCAMERA_TARGET_FRAME_MAX]; 
    uint32                          frame_count; 
}DCAMERA_IMAGE_PARAM_T;

typedef uint32 (* DCAMEMRA_IMAGE_NOTICE_CALLBACK)(uint32 ret_value, DCAMERA_IMAGE_PARAM_T *p_image_frame, void* p_param);

typedef uint32 (* DCAMEMRA_DISPLAY_CALLBACK)(uint32 ret_value, DCAMERA_DISPALY_PARAM_T *p_disp_src, DCAMERA_DISPALY_PARAM_T *p_disp_dst,void* p_param);

typedef struct
{
    LCD_ID_E                        lcd_id;             // lcd id 
    uint32                          lcd_block_id;       // lcd block id: 0~5
    DCAMERA_RECT_T                  disp_rect;          // display size on lcd
    DCAMERA_RECT_T                  target_rect;        // preview image size   
    DCAMERA_ROTATION_MODE_E         rotation_mode;      // 0: normal; 1: 90; 2: 180; 3:270;
    DCAMERA_DISP_MODE_E             disp_mode;
    
    DCAMERA_IMAGE_FRAME_PARAM_T     image_frame_info[DCAMERA_TARGET_FRAME_MAX];     
    uint32                          image_frame_count;
    DCAMEMRA_IMAGE_NOTICE_CALLBACK  image_notice_cb;
    void                            *app_data_ptr;
    uint32                          app_data_len;
    DCAMEMRA_DISPLAY_CALLBACK       disp_cb;
    DCAMERA_INALIDATEMODE_E         invalidatemode;
    
}DCAMERA_PREVIEW_EX_PARAM_T;


typedef struct
{
    DCAMERA_IMAGE_FRAME_PARAM_T     target_info[DCAMERA_TARGET_FRAME_MAX];
    uint32                          target_frame_count;
    BOOLEAN                         b_exif_needed;
    uint8                           snapshot_num;
    uint8                           res2;    
    uint8                           res3;    
    void*                           app_data_ptr;
    uint32                          app_data_len;
    uint32                          interval_of_frame;
    uint16                          enc_quality_level;
    uint16                          flash_enable;   
    DCAMEMRA_IMAGE_NOTICE_CALLBACK  image_notice_cb;    
    DCAMERA_QUICK_VIEW_CALL_BACK    quickview_callback;
    DCAMERA_CALL_BACK               capture_start_callback; // call back function when start capture
}DCAMERA_SNAPSHOT_EX_PARAM_T;

typedef struct
{   
    DCAMERA_IMAGE_PARAM_T           return_frame_info;
}DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T;
typedef struct _dc_ext_get_data_tag
{
    DCAMERA_DATA_TYPE_E data_format;
    DCAMERA_SIZE_T img_size;
    uint32 buf_addr;
    uint32 buf_len;    
    DCAMERA_RECT_T display_rect;
    DCAMERA_RECT_T target_rect;
    DCAMERA_ROTATION_MODE_E display_rotation;
    BOOLEAN is_display; 
    BOOLEAN reserve1;
    BOOLEAN reserve2;
    BOOLEAN reserve3;
    LCD_ID_E   lcd_id;
}DCAMERA_EXT_GET_DATA_T, *DCAMERA_EXT_GET_DATA_T_PTR;

typedef struct _dc_ext_display_tag
{
    DCAMERA_DATA_TYPE_E src_format;
    DCAMERA_SIZE_T src_img_size;
    uint32 src_jpg_addr;
    uint32 src_jpg_buf_len; 
    uint32 jpg_decoder_addr;
    uint32 src_decoder_buf_len;    
    DCAMERA_YUV_CHN_ADDR_T src_yuv_addr;
    uint32 src_yuv_buf_len;       
    DCAMERA_RECT_T display_rect;
    DCAMERA_RECT_T target_rect;
    uint32 display_addr;
    uint32 display_buf_len; 
    uint32 rotation_addr;
    uint32 rotation_buf_len; 
    DCAMERA_ROTATION_MODE_E display_rotation;
    LCD_ID_E   lcd_id;
}DCAMERA_EXT_DISPLAY_T, *DCAMERA_EXT_DISPLAY_T_PTR;

typedef struct
{
    uint32                  sensor_nv_sign;                         //sensor nv significant code value
    uint8                   sensor_index[DCAMERA_SENSOR_NUMBER];    //sensor id value
    uint8                   reserved;
}DCAMERA_CAM_NV_INFO;

typedef struct
{
    DCAMERA_CAM_NV_INFO     cam_val_nv;                             //NV information
    BOOLEAN                 is_need_save;                           //if need to save to NV
    uint8                   reserved1;
    uint8                   reserved2;
    uint8                   reserved3;
}DCAMERA_CAM_PARAM_T, *DCAMERA_CAM_PARAM_T_PTR;

/**---------------------------------------------------------------------------*
 **                         Function definition                                  *
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description:analog tv start auto search channel
// Author:     Tim.zhu
// Input:      fun: call back function
// Output:     none
// Return:     SCI_TRUE    open success
//             SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
PUBLIC uint32 DCAMERA_ATVAutoScanChn(DCAMERA_PREVIEW_PARAM_T* param_ptr);

/******************************************************************************/
// Description:analog tv stop auto search channel
// Author:     Tim.zhu
// Input:      fun: call back function
// Output:     none
// Return:     SCI_TRUE    open success
//             SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
PUBLIC uint32 DCAMERA_ATVStopScanChn(uint32 (*fun)());

 /******************************************************************************/
// Description: Third part open sensor
// Author:     Tim.zhu
// Input:       none
// Output:     none
// Return:     SCI_TRUE    open success
//                 SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
PUBLIC uint32 DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MODE img_sensor_mode, DCAMERA_PARAM_VIDEO_MODE_E work_mode);
 
 /******************************************************************************/
// Description: Third part open sensor
// Author:     Tim.zhu
// Input:       none
// Output:     none
// Return:     SCI_TRUE    open success
//                 SCI_FALSE   open fail
// Note:       
/******************************************************************************/  
PUBLIC uint32 DCAMERA_ExtCloseSensor(void);
 
 /******************************************************************************/
// Description: Third part get data
// Author:     Tim.zhu
// Input:       DCAMERA_EXT_GET_DATA_T_PTR : the pointer of get data param
// Output:     none
// Return:     SCI_TRUE    open success
//                 SCI_FALSE   open fail
// Note:       
/******************************************************************************/  
PUBLIC uint32 DCAMERA_ExtGetData(DCAMERA_EXT_GET_DATA_T_PTR get_param , DCAMERA_YUV_CHN_ADDR_T* rtn_img_addr, DCAMERA_SIZE_T* rtn_size);
 
 /******************************************************************************/
// Description: Third part display data
// Author:     Tim.zhu
// Input:       DCAMERA_EXT_DISPLAY_T_PTR : the pointer of display param
// Output:     none
// Return:     SCI_TRUE    open success
//                 SCI_FALSE   open fail
// Note:       
/******************************************************************************/  
PUBLIC uint32 DCAMERA_ExtDisplay(DCAMERA_EXT_DISPLAY_T_PTR display_param); 

/******************************************************************************/
// Description: Get the hight of scale match
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     uint16: match height
// Note:       
/******************************************************************************/  
PUBLIC uint16 DCAMERA_GetScaleMatchHeight(uint16 input_height, uint16 output_height);
#if 0
/******************************************************************************/
// Description: Get the info of register img sensor
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     SENSOR_REGISTER_INFO_T_PTR:the info pointer of register img sensor
// Note:       
/******************************************************************************/  
PUBLIC SENSOR_REGISTER_INFO_T_PTR DC_GetImgSensorRegisterInfo(void);
#endif
/******************************************************************************/
// Description: Get the id if currect img sensor
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     uint32:the id of img sensor
// Note:       
/******************************************************************************/  
PUBLIC uint32 DC_GetCurImgSensorId(void);

/******************************************************************************/
// Description: Set the id if currect img sensor
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     SCI_SUCCESS: success
// Note:       
/******************************************************************************/  
PUBLIC uint32 DC_SetCurImgSensorId(uint32 sensor_id);
 
/******************************************************************************/
// Description: To check if digical camera is open
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     SCI_TRUE    has opened
//             SCI_FALSE   has not opened
// Note:       
/******************************************************************************/ 
PUBLIC BOOLEAN DCAMERA_IsOpen(void);

/******************************************************************************/
// Description: Open digital Camera function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       before start preview, call this function to open digital camera
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Open(DCAMERA_OP_MODE_E op_mode);

/******************************************************************************/
// Description: Close digital Camera function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Close(void);

/******************************************************************************/
// Description: Start to preview
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartPreview(DCAMERA_PREVIEW_PARAM_T const* param_ptr);

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartPreviewEX(DCAMERA_PREVIEW_EX_PARAM_T const* param_ex_ptr);
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DisplayOneFrame(DCAMERA_DISPALY_PARAM_T *param_ptr);

/******************************************************************************/
// Description: Stop preview
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopPreview(void);

/******************************************************************************/
// Description: snapshot single image
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoSnapshot
    (
        DCAMERA_SNAPSHOT_PARAM_T const  *   param_ptr,
        DCAMERA_SNAPSHOT_RETURN_PARAM_T *   return_param_ptr
    );

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoSnapshotEX
(
    DCAMERA_SNAPSHOT_EX_PARAM_T const  *    param_ptr,
    DCAMERA_SNAPSHOT_EX_RETURN_PARAM_T *    return_param_ptr
);

/******************************************************************************/
// Description: snapshot multi images
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoBurstSnapshot
    (
        DCAMERA_BURST_SNAPSHOT_PARAM_T const  * param_ptr,
        DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T *     return_param_ptr
    );

/******************************************************************************/
// Description: Digital camera parameter control
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ParamCtrl
    (
        DCAMERA_PARAM_CTRL_CMD_E            param_cmd,
        uint32                              arg,
        DCAMERA_PARAM_CTRL_CALL_BACK        callback
    );

/******************************************************************************/
// Description: Get some DC information
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_GetInfo
    (
        DCAMERA_INFO_CMD_E          param_cmd,
        uint32*                     info_ptr
    );
/******************************************************************************/
// Description: Set auto focus information
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetAutoFocus(DCAMERA_AF_INFO_T const * af_info_ptr);

/******************************************************************************/
// Description: Review JPEG
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ReviewJpeg(DCAMERA_REVIEW_PARAM_T const * param_ptr);


/******************************************************************************/
// Description: Start record
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartRecord ( DCAMERA_RECORD_PARAM_T * dc_param_ptr );

/******************************************************************************/
// Description: Stop record
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopRecord ( void );


/******************************************************************************/
// Description: Start video phone
// Author:     Baggio.he
// Input:      preview_param_ptr, pointer to data structure DCAMERA_PREVIEW_PARAM_T
//             capture_param_ptr, pointer to data structure DCAMERA_RECORD_PARAM_T
// Output:     none
// Return:     0           successful
//             others      error code
// Note:       Be called just after DCAMERA_Open
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartVideoPhone(DCAMERA_PREVIEW_PARAM_T const *preview_param_ptr,
                                                      DCAMERA_RECORD_PARAM_T * capture_param_ptr);

/******************************************************************************/
// Description: Stop video phone
// Author:     Baggio.he
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopVideoPhone( void );


/******************************************************************************/
// Description:   set the combine(panorama) max num
// Author:        Tim.zhu
// Input:         uint32 num: max capture num
// Output:        none
// Return:        0           successful
//                others      failed
// Note:          if return successful
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetCombineNum(uint32 num);

/******************************************************************************/
// Description:   clean the combine(panorama fourinone) parameter
// Author:        Tim.zhu
// Input:         none
// Output:        none
// Return:        0           successful
//                others      failed
// Note:          if return successful, the LCD will be on bridge mode until capture is done
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_CleanCombineParam(void);

/******************************************************************************/
// Description:   ext set the dc memory size and addr
// Author:        Tim.zhu
// Input:         mem_size: memory size
//                mem_ptr: memory addr
// Output:        none
// Return:        0           successful
//                others      failed
// Note:          if return successful, the LCD will be on bridge mode until capture is done
/******************************************************************************/
PUBLIC uint32 DCAMERA_SetExtMem(uint32 mem_size,void* mem_ptr);

/******************************************************************************/
// Description:   get snapshot size without interpolation
// Author:        Eddy.wei
// Input:         none
// Output:        none
// Return:        others                      successful
//                DCAMERA_PHOTO_SIZE_MAX      failed
// Note:            
/******************************************************************************/
PUBLIC DCAMERA_PARAM_PHOTO_SIZE_E DCAMERA_GetActualSnapshotMaxResolution(void);

/******************************************************************************/
// Description:   DCAMERA intial operating
// Author:        XZ
// Input:         BOOLEAN is_eng_mode / DCAMERA_CAM_PARAM_T *p_Param
// Output:        DCAMERA_CAM_PARAM_T *p_Param
// Return:        
//                DCAMERA_RETURN_VALUE_E      rtn
// Note:            
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Init(BOOLEAN if_force_calib, DCAMERA_CAM_PARAM_T *p_Param);

/******************************************************************************/
// Description:   DCAMERA is support 2M snapshot
// Author:        Eddy.wei
// Input:         none
// Output:        none *p_Param
// Return:        
//                SCI_TRUE   support
//                SCI_FALSE  not support 
// Note:            
/******************************************************************************/
PUBLIC uint32 DCAMERA_IsSupport2MSnapshot(void);

#ifdef __cplusplus
    }
#endif


#endif  /*#ifndef _DAL_DCAMERA_H_*/


