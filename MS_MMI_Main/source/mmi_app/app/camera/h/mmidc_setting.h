#ifndef _MMIDC_SETTING_H
#define _MMIDC_SETTING_H

#include "sci_types.h"

//#include "ffs.h"
#include "mmi_filemgr.h"
#include "guifont.h"
#include "mmidc_export.h"
#include "dal_dcamera.h"
#include "mmk_type.h"
#include "dal_lcd.h"
#include "guiimg.h"
#include "sfs.h"

#ifdef WIN32
//macro replace
//#define MMIMPEG4_AVI_ENABLE
#endif

//define ASSERT MACRO SWITCH
//#ifndef MMIDC_ASSERT_MODE
//#define MMIDC_ASSERT_MODE
//#endif

#define MMIDC_DEBUG_TRACE

#ifdef MMIDC_ASSERT_MODE
    #define MMIDC_ASSERT_WARNING    MMI_ASSERT_WARNING
#else
    #define MMIDC_ASSERT_WARNING    SCI_TRACE_LOW
#endif

#define MMIDC_RECORD_COUNT_TIME      490

#define MMIDC_DELAY_TIME             930
#define MAX_MMI_SENSOR_NUM           2
#define SENSOR_NUMBER_INVALID        0xFF

#define SIZE_STR_LENGTH  10


#define MMIDC_HVGA_PREVIEW_HEIGHT           480
#define MMIDC_WQVGA_PREVIEW_HEIGHT          400
#define MMIDC_QVGA_PREVIEW_HEIGHT           320
#define MMIDC_QCIF_PREVIEW_HEIGHT           220
#define MMIDC_QQVGA_PREVIEW_HEIGHT          160

#define DC_PHOTO_QUALITY_0				0
#define DC_PHOTO_QUALITY_1				1
#define DC_PHOTO_QUALITY_2				2
#define DC_PHOTO_QUALITY_3				3
#define DC_PHOTO_QUALITY_4				4
#define DC_PHOTO_QUALITY_5				5
#define DC_PHOTO_QUALITY_6				6
#define DC_PHOTO_QUALITY_7				7
#define DC_PHOTO_QUALITY_8				8
#define DC_PHOTO_QUALITY_9				9

#define DC_NO_BURST_PHOTO				1
#define DC_BURST_3_PHOTO				3
#define DC_BURST_6_PHOTO				6
#define DC_BURST_9_PHOTO				9


#define MAX_DC_BURST_PHOTO_NUMBER      DC_BURST_6_PHOTO 

#define NO_SAVE_PHOTO						(-1)
#define	INVALID_DATA						(0)

#define DC_FILE_NAME_ID_MAX				10000000

#define MMIDC_FULL_PATH_LENGTH          MMIFILE_FULL_PATH_MAX_LEN

typedef enum
{
    MMIDC_CMD_DISP_PREVIEW_FRAME = 0,//display every frame of preview
    MMIDC_CMD_IMAGE_PROCESS_DONE,//image process done
    MMIDC_CMD_CREATE_FILE_HANDLE,// create file handle
    MMIDC_CMD_GET_LAST_THUMBNAIL,// get last thumbnail
    MMIDC_CMD_START_DC,//start dc
    MMIDC_CMD_GET_FOCUS,//get focus
    MMIDC_CMD_SWITCH_VIDEO_MODE,//switch video mode
    MMIDC_CMD_PLAY_CAPTURE_RING_END,
	MMIDC_CMD_RENAME_FILE_AGAIN,
    MMIDC_CMD_MAX
}MMIDC_CMD_E;

typedef enum
{
    CAMERA_ENG_LCD_176X220 = 0,	
        CAMERA_ENG_LCD_240X320,
        CAMERA_ENG_LCD_240X400,
        CAMERA_ENG_LCD_SIZE_MODE_MAX
}CAMERA_ENG_LCD_SIZE_MODE_E;

typedef enum
{
    CAMERA_ENG_PREVIEW_HOR_MODE = 0,	
        CAMERA_ENG_PREVIEW_VER_MODE,
        CAMERA_ENG_PREVIEW_MODE_MAX
}CAMERA_ENG_PREVIEW_MODE_E;

typedef enum
{
    CAMERA_ENG_FULL_VISUAL_ANGLE = DCAMERA_DISP_FULLVIEW,	
        CAMERA_ENG_FULL_SCREEN = DCAMERA_DISP_FULLSCREEN,
        CAMERA_ENG_VISUAL_MODE_MAX
}CAMERA_ENG_VISUAL_MODE_E;

typedef enum
{	
        PHOTO_SIZE_128X160 = 0,
		PHOTO_SIZE_160X128,
		PHOTO_SIZE_176X220,
        PHOTO_SIZE_220X176,
        PHOTO_SIZE_240X320,
        PHOTO_SIZE_240X400,
        PHOTO_SIZE_320X240, 
        PHOTO_SIZE_320X480,
        PHOTO_SIZE_400X240,        
        PHOTO_SIZE_480X320,
        PHOTO_SIZE_480X640,        
        PHOTO_SIZE_640X480,
        PHOTO_SIZE_960X1280,
        PHOTO_SIZE_1280X960,
        PHOTO_SIZE_1600X1200,
        PHOTO_SIZE_2048X1536,
        PHOTO_SIZE_2592X1944,
        PHOTO_SIZE_3264X2448,
        PHOTO_SIZE_MAX
}MMIDC_PHOTO_SIZE_E;

typedef enum
{
    CAMERA_MODE_DC = 0,	
        CAMERA_MODE_DV,	
        CAMERA_MODE_MAX
}CAMERA_MODE_E;

typedef enum
{
    SCREEN_MODE_HORIIZONTAL = 0,
        SCREEN_MODE_VERTICAL,
        SCREEN_MODE_MAX
}SCREEN_MODE_E;

typedef enum
{
    FLICKER_50 = 0,
        FLICKER_60,
        FLICKER_MAX
}FLICKER_E;

typedef enum
{
    SELF_SHOOT_DISABLE = 0,
        SELF_SHOOT_3S,
        SELF_SHOOT_10S,
        SELF_SHOOT_MAX
}SELF_SHOOT_E;

typedef enum
{
    MULTI_SHOOT_DISABLE = 0,
        MULTI_SHOOT_3,
        MULTI_SHOOT_6,
        MULTI_SHOOT_9,
        MULTI_SHOOT_MAX
}MULTI_SHOOT_E;

typedef enum
{
    DC_EFFECT_NORMAL = 0,				// 普通
    DC_EFFECT_BLACKWHITE,				// 黑白		        
    DC_EFFECT_BLUE,                     // 蓝色
    DC_EFFECT_GREEN,						// 绿色	
    DC_EFFECT_YELLOW,					      // 黄色
    DC_EFFECT_RED,                      // 红色
    DC_EFFECT_CANVAS,                   // 怀旧
    DC_EFFECT_NEGATIVE,				// 底片
    DC_EFFECT_RELIEVOS,     				//浮雕
    DC_EFFECT_MAX
}DC_EFFECT_E;


typedef enum
{
    SHOOT_SCENE_AUTO = 0,
        SHOOT_SCENE_HIGHT_LIGHT,
        SHOOT_SCENE_LOW_LIGHT,
        SHOOT_SCENE_PEOPLE,
        //SHOOT_SCENE_LOW_LIGHT_PEOPLE,
        SHOOT_SCENE_NIGHTS,
        SHOOT_SCENE_SAND_BEACH,
        SHOOT_SCENE_SPORTS,
        SHOOT_SCENE_FILE,
        SHOOT_SCENE_MAX
}SHOOT_SCENE_E;

typedef enum
{
    FRAME_DISABLE = 0,
        FRAME_1,
        FRAME_2,
        FRAME_3,
        FRAME_4,
        FRAME_MAX,
        FRAME_5,
        FRAME_6,
        FRAME_7,
        FRAME_8,
        FRAME_9,
        FRAME_10,
        FRAME_11,
        FRAME_12,
        FRAME_13,
        FRAME_14,
        FRAME_15
}FRAME_E;

typedef enum
{
    WHITE_BALANCE_AUTO = DCAMERA_WB_MODE_AUTO,
        WHITE_BALANCE_INCANDESCENCE = DCAMERA_WB_MODE_INCANDESCENCE, 
        WHITE_BALANCE_FLUORESCENT = DCAMERA_WB_MODE_FLUORESCENT,
        WHITE_BALANCE_SUN = DCAMERA_WB_MODE_SUN,
        WHITE_BALANCE_CLOUDY = DCAMERA_WB_MODE_CLOUD,
        WHITE_BALANCE_MAX
}WHITE_BALANCE_E;

typedef enum
{
    PHOTO_QUALITY_BRIEF = 0 ,				// 草稿
    PHOTO_QUALITY_NORMAL,				// 普通
    PHOTO_QUALITY_ADVANCED,			   // 精细
    PHOTO_QUALITY_MAX
}PHOTO_QUALITY_E;

typedef enum
{
    SHUTTER_VOICE_1 = 0,
        SHUTTER_VOICE_DISABLE,				// 快门声音关
        SHUTTER_VOICE_MAX,
        SHUTTER_VOICE_2,
        SHUTTER_VOICE_3
}SHUTTER_VOICE_E;

typedef enum
{
    SHOOT_MODE_NORMAL = 0,		
        SHOOT_MODE_PANORAMA_LEFT_RIGHT,  //全景拍摄
        SHOOT_MODE_PANORAMA_RIGHT_LEFT,
        SHOOT_MODE_MAX
}SHOOT_MODE_E;


typedef enum
{
    AUTO_SAVE_ON = 0,
        AUTO_SAVE_OFF,
        AUTO_SAVE_MAX
}AUTO_SAVE_E;

typedef enum
{
    PHOTO_DISPLAY_SETTING_TIP_ON = 0,
        PHOTO_DISPLAY_SETTING_TIP_OFF,
        PHOTO_DISPLAY_SETTING_TIP_MAX
}PHOTO_DISPLAY_SETTING_TIP_E;

typedef enum
{
    VIDEO_DISPLAY_SETTING_TIP_ON = 0,
        VIDEO_DISPLAY_SETTING_TIP_OFF,
        VIDEO_DISPLAY_SETTING_TIP_MAX
}VIDEO_DISPLAY_SETTING_TIP_E;

typedef enum
{
    EXPOSAL_COMPENSATE_MINUS_3 = 0,
        EXPOSAL_COMPENSATE_MINUS_2,
        EXPOSAL_COMPENSATE_MINUS_1,
        EXPOSAL_COMPENSATE_ZERO,
        EXPOSAL_COMPENSATE_PLUS_1,
        EXPOSAL_COMPENSATE_PLUS_2,
        EXPOSAL_COMPENSATE_PLUS_3,
        EXPOSAL_COMPENSATE_MAX
}EXPOSAL_COMPENSATE_E;

typedef enum
{
    BRIGHTNESS_LEVEL_0 = 0,
        BRIGHTNESS_LEVEL_1,
        BRIGHTNESS_LEVEL_2,
        BRIGHTNESS_LEVEL_3,
        BRIGHTNESS_LEVEL_4,
        BRIGHTNESS_LEVEL_5,
        BRIGHTNESS_LEVEL_6,
        BRIGHTNESS_LEVEL_MAX
} BRIGHTNESS_E;

typedef enum
{
    CONTRAST_0 = 0,
        CONTRAST_1,
        CONTRAST_2,
        CONTRAST_3,
        CONTRAST_4,
        CONTRAST_5,
        CONTRAST_6,
        CONTRAST_MAX
}CONTRAST_E;

typedef enum
{
    ISO_AUTO = 0,	
        ISO_LOW,				
        ISO_MID,
        ISO_HIGH,
        ISO_MAX
}ISO_E;

typedef enum
{
    VIDEO_SIZE_96X128 = 0,
    VIDEO_SIZE_128X160,
    VIDEO_SIZE_144X176,
    VIDEO_SIZE_176X240,
    VIDEO_SIZE_240X320,
    VIDEO_SIZE_128X96,
    VIDEO_SIZE_160X128,
    VIDEO_SIZE_176X144,
    VIDEO_SIZE_240X176,
    VIDEO_SIZE_320X240,
#ifdef MMIDC_F_VIDEO_352X288    
    VIDEO_SIZE_352X288,
#endif
    VIDEO_SIZE_MAX
}VIDEO_SIZE_E;

typedef enum
{
        ENVIRONMENT_NIGHT = 0,// 夜间
        ENVIRONMENT_AUTO,// AUTO
        ENVIRONMENT_MAX
}ENVIRONMENT_E;

typedef enum
{
    VIDEO_FILE_TYPE_MP4 = 0,
        VIDEO_FILE_TYPE_AVI,
        VIDEO_FILE_TYPE_3GP,
        VIDEO_FILE_TYPE_MJPEG,
        VIDEO_FILE_TYPE_MAX
} VIDEO_FILE_TYPE_E;

typedef enum
{
    AUDIO_SETTING_ON = 0,
        AUDIO_SETTING_OFF,
        AUDIO_SETTING_MAX
} AUDIO_SETTING_E;

typedef enum
{
    SETTING_SENSOR_REAR = 0,
    SETTING_SENSOR_FRONT,
    SETTING_SENSOR_ID_MAX
}SETTING_SENSOR_ID_E;

typedef enum
{
    PHOTO_DATE_ON = 0,
    PHOTO_DATE_OFF,
    PHOTO_DATE_MAX
}PHOTO_DATE_E;

typedef struct
{
    uint8 data_unit; // RGB565:2, ARGB888:4;
    GUIIMG_DATE_TYPE_E data_format;//rgb565, rgb888,jpg
}MMIDC_DATA_FORMAT_INFO_T;

typedef struct
{
    FRAME_E last_frame_index;
    MMIDC_PHOTO_SIZE_E photo_size;
    CAMERA_ENG_VISUAL_MODE_E visual_mode;
}SETTING_BACKUP_INFO_T;

typedef struct
{
    BOOLEAN is_enable;
    MMIFILE_DEVICE_E    file_dev;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN+1];
    uint16  file_name_len;
    uint32  file_size;
    SFS_HANDLE file_handle;
}MMIDC_FILE_INFO_T;

typedef struct
{
    BOOLEAN is_enable;
    MMIFILE_DEVICE_E    file_dev;
	wchar*   file_name_ptr;
    uint16  file_name_len;
    uint32  file_size;
    SFS_HANDLE file_handle;
}MMIDC_FILE_INFO_PTR_T;

typedef enum
{
    MMIDC_BRUSH_START,
    MMIDC_BRUSH_BRUSHING,
    MMIDC_BRUSH_MAX
}MMIDC_BRUSH_STATUS_E; //for mmi brush lcd

typedef enum
{
    MMIDC_GET_FOCUS,
    MMIDC_LOSE_FOCUS,
    MMIDC_FOCUS_MAX
}MMIDC_FOCUS_STATUS_E;

#ifdef MMIDC_MINI_RESOURCE_SIZE
typedef struct
{
    int16 total_num;
    MMIDC_PHOTO_SIZE_E photo_size_array[PHOTO_SIZE_MAX];
}SETTING_PHOTO_SIZE_INFO_T;

typedef struct
{
    int16 total_num;
    VIDEO_SIZE_E video_size_array[VIDEO_SIZE_MAX];
}SETTING_VIDEO_SIZE_INFO_T;
#endif

typedef struct
{
    DCAMERA_FLASH_MODE_E            dc_flash_lamp;         //闪光灯（自动，开启，关闭）
    DCAMERA_FLASH_MODE_E            dv_flash_lamp;         //闪光灯（自动，开启，关闭）
    FLICKER_E               photo_flicker;
    PHOTO_QUALITY_E         photo_quality;      //图片质量（精细，普通，草稿）
    MMIDC_PHOTO_SIZE_E      photo_size;         //图片分辨率(3MP, 2MP, 1MP, VGA,     
	SHUTTER_VOICE_E         shutter_voice;      //快门音（无，快门音一，快门音二，快门音三）    
	AUTO_SAVE_E             auto_save;          //手动保存（开，关）    
	PHOTO_DISPLAY_SETTING_TIP_E  photo_display_setting_tip;    
	MMIFILE_DEVICE_E          pref_photo_storage_dev;  //优先存储位置  （存储卡，手机）    
	MMIFILE_DEVICE_E          photo_storage_dev;  //存储位置  （存储卡，手机）    
	PHOTO_DATE_E              photo_date; //photo date enable or no
    
    AUDIO_SETTING_E         audio_setting;
    FLICKER_E               video_flicker;
    VIDEO_SIZE_E            video_size;         //图像分辨率(352X288, 176X144)
    VIDEO_FILE_TYPE_E       video_file_type;            //文件格式(mp4, 3gp)   
	VIDEO_DISPLAY_SETTING_TIP_E video_display_setting_tip;  
	MMIFILE_DEVICE_E          pref_video_storage_dev;  //优先存储位置  （存储卡，手机）    
	MMIFILE_DEVICE_E           video_storage_dev;  //存储位置  （存储卡，手机）


    //不同平台可选功能设置 
    uint32 photo_file_name_id;
    uint32 video_file_name_id;
    SCREEN_MODE_E default_screen_mode;
    CAMERA_ENG_VISUAL_MODE_E default_visual_mode;
    DCAMERA_ROTATION_MODE_E sensor_angle[SETTING_SENSOR_ID_MAX]; //camera sensor angle

//     wchar photo_file_path[MMIDC_FULL_PATH_LENGTH + 1];
//     wchar video_file_path[MMIDC_FULL_PATH_LENGTH + 1];
}MMIDC_SETTING_NV_INFO_T;

typedef struct 
{
    //photo settings
    CAMERA_MODE_E           camera_mode;        //相机模式（phot、video）
    uint32                  photo_zoom;         //变焦（0 ~ 7）
    ENVIRONMENT_E           photo_environment;
    SELF_SHOOT_E            self_shoot;         //延时自拍（关闭，3s, 10s）
    MULTI_SHOOT_E           multi_shoot;        //多张连拍（关闭，3，6，9）
    DC_EFFECT_E             photo_effect;               //图像效果（普通，黑白，底片，深褐色，曝光）
    SHOOT_SCENE_E           scene;              //拍摄场景（自动，强光风景，微光风景，人像，微光人像
    //夜间，沙滩/雪地，运动，文件）
    EXPOSAL_COMPENSATE_E    exposal_compensate; //曝光补偿(-4 ~ 4)
    BRIGHTNESS_E            photo_brightness;
    CONTRAST_E              contrast;           //对比度(-2 ~ 2)
    ISO_E                   iso;
    FRAME_E                 frame;              //相框（无，相框1 ~ 相框15）
    WHITE_BALANCE_E         photo_white_balance;//白平衡（自动，白炽灯，荧光灯，晴天，阴天）
    SCREEN_MODE_E           screen_mode;        //屏幕模式（横屏，竖屏）    
	SHUTTER_VOICE_E         shutter_voice;      //快门音（无，快门音一，快门音二，快门音三）   
	MMIDC_PHOTO_SIZE_E      photo_size;         //图片分辨率(3MP, 2MP, 1MP, VGA, 
    SHOOT_MODE_E            shoot_mode;         //拍摄模式（普通，全境拍照）

    MMIFILE_DEVICE_E        photo_storage_dev;  //存储位置  （存储卡，手机）
    //video settings
    DC_EFFECT_E             video_effect;
    uint32                  video_zoom;         //变焦（0 ~ 7）
    BRIGHTNESS_E            video_brightness;
    CONTRAST_E              video_contrast;
    ENVIRONMENT_E           video_environment;      //环境(AUTO, 室内, 夜间)
    WHITE_BALANCE_E         video_white_balance;    //白平衡（自动，白炽灯，荧光灯，晴天，阴天）
    VIDEO_SIZE_E            video_size;         //图像分辨率(352X288, 176X144)
    
    SETTING_SENSOR_ID_E       sensor_id;   //front sensor or rear sensor
    
    SETTING_BACKUP_INFO_T    setting_backup_info;
    MMIDC_SETTING_NV_INFO_T setting_nv_info;
    MMIFILE_DEVICE_E        video_storage_dev;  //存储位置  （存储卡，手机）

    DCAMERA_DISPALY_PARAM_T src_dcam_display;
    DCAMERA_DISPALY_PARAM_T dst_dcam_display;

    MMIDC_BRUSH_STATUS_E lcd_brush_status;
    MMIDC_FOCUS_STATUS_E focus_status;

    SCI_MUTEX_PTR file_handle_mutex_ptr;
    SCI_MUTEX_PTR img_proc_mutex_ptr;

	wchar* photo_file_path_ptr;
	wchar* video_file_path_ptr;

#ifdef MMIDC_MINI_RESOURCE_SIZE
    SETTING_PHOTO_SIZE_INFO_T photo_size_info;
    SETTING_VIDEO_SIZE_INFO_T video_size_info;
#endif
}MMIDC_OSD_SETTING_INFO_T;

typedef enum
{
    MMIDC_LAYOUT_TRADITION = 0, //body strings are underside at LCD
    MMIDC_LAYOUT_RIGHTWARD,     //body strings are rightward at LCD
    MMIDC_LAYOUT_LEFTWARD,      //body strings are leftward at LCD
    MMIDC_LAYOUT_MAX
}MMIDC_LAYOUT_E;

typedef enum
{
    MMIDC_SHOW_NONE = 0,
    MMIDC_SHOW_SETTING,
    MMIDC_SHOW_ICON,
    MMIDC_SHOW_MENU,
    MMIDC_SHOW_ADJUST,
    MMIDC_SHOW_ALLMENU,
    MMIDC_SHOW_MAX
}MMIDC_SHOW_STATE_E;

typedef struct
{
    LCD_ANGLE_E         platform_angle;
    uint16              lcd_size;        //max value of lcd screen size
    uint16              physical_lcd_width;  //physical lcd width
    uint16              physical_lcd_height; //physical lcd height
    MMIDC_SHOW_STATE_E  show_state;
    GUI_RECT_T          frame_rect;
    FRAME_E             switch_frame_index; //for size 
    BOOLEAN             is_real_hor_lcd; //
    LCD_ANGLE_E         software_rot_angle; //software rotation angle as physical sensor hardware
}MMIDC_INFO_T;

typedef struct
{
    int32 src_size;
    int32 dst_size;
}MMIDC_SIZE_T;

typedef enum
{
    MMIDC_CHIPTEST_STEP_START,
    MMIDC_CHIPTEST_STEP_SNAPSHOT,
    MMIDC_CHIPTEST_STEP_RECORD,
    MMIDC_CHIPTEST_STEP_PLAY,
    MMIDC_CHIPTEST_STEP_END,
    MMIDC_STEP_MAX
}MMIDC_CHIPTEST_STEP_E;

typedef struct
{
    uint16 size_width;
    uint16 size_height;
}MMIDC_SIZE_ATTRIBUTE_T;

typedef enum
{
    ACTION_SWITCH_CAMERA_MODE,
    ACTION_MAX
}DO_ACTION_E;

typedef struct
{
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type[MAX_MMI_SENSOR_NUM];
    int32 sensor_number;
}MMIDC_FIXED_PARAMETER_T;

typedef struct
{
    MMIDC_CMD_E cmd;
    uint32 start_preview_tick;
}MMIDC_CMD_PARAM_T;


/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIDC_SetPhotoSize      MMIAPIDC_SetPhotoSize

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveSettings(void);

/*****************************************************************************/
// 	Description : init dc settings when open dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_Setting_InitDefaultValue(void);

/*****************************************************************************/
// 	Description : get dc screen mode 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SCREEN_MODE_E MMIDC_GetScreenMode(void);

/*****************************************************************************/
// 	Description : set dc screen mode 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetScreenMode(SCREEN_MODE_E mode);

/*****************************************************************************/
// 	Description : get dc mode like photo or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_MODE_E MMIDC_GetCameraMode(void);

/*****************************************************************************/
// 	Description : set dc mode like photo or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCameraMode(CAMERA_MODE_E mode);

/*****************************************************************************/
// 	Description : get frame index
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC FRAME_E MMIDC_GetFrameIndex(void);

/*****************************************************************************/
// 	Description : set frame index
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFrameIndex(FRAME_E frame, BOOLEAN is_preview);

/*****************************************************************************/
// 	Description : get shoot mode like normal or panorama
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SHOOT_MODE_E MMIDC_GetShootMode(void);

/*****************************************************************************/
// 	Description : set shoot mode like normal or panorama
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetShootMode(SHOOT_MODE_E mode);

/*****************************************************************************/
// 	Description : get multi shoot enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MULTI_SHOOT_E MMIDC_GetMultiShootEnum(void);

/*****************************************************************************/
// 	Description : set multi shoot enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMultiShootEnum(MULTI_SHOOT_E shoot);

/*****************************************************************************/
// 	Description : get photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetPhotoStorageDevice(void);

/*****************************************************************************/
// 	Description : set photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoStorageDevice(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
// 	Description : get photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetPrefPhotoStorageDevice(void);

/*****************************************************************************/
// 	Description : set photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPrefPhotoStorageDevice(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
// 	Description : get photo brightness
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BRIGHTNESS_E MMIDC_GetPhotoBrightness(void);

/*****************************************************************************/
// 	Description : set photo brightness
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoBrightness(BRIGHTNESS_E photo_brightness);

/*****************************************************************************/
// 	Description : get photo quality
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC PHOTO_QUALITY_E MMIDC_GetPhotoQuality(void);

/*****************************************************************************/
// 	Description : set photo quality
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoQuality(PHOTO_QUALITY_E quality);

/*****************************************************************************/
// 	Description : is auto save
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC AUTO_SAVE_E MMIDC_GetIsAutoSave(void);

/*****************************************************************************/
// 	Description : set  auto save
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetAutoSave(AUTO_SAVE_E promt);

/*****************************************************************************/
// 	Description : get photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetPhotoSize(void);

/*****************************************************************************/
// 	Description : set photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetPhotoSize(MMIDC_PHOTO_SIZE_E size);

/*****************************************************************************/
// 	Description : get photo environment enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC ENVIRONMENT_E MMIDC_GetPhotoEnvironment(void);

/*****************************************************************************/
// 	Description : set photo environment enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoEnvironment(ENVIRONMENT_E environment);

/*****************************************************************************/
// 	Description : set photo exposal compensate enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetExposalCompensate(EXPOSAL_COMPENSATE_E exposal_compensate);

/*****************************************************************************/
// 	Description : get photo exposal compensate enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC EXPOSAL_COMPENSATE_E MMIDC_GetExposalCompensate(void);

/*****************************************************************************/
// 	Description : set shutter voice enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetShutterVoice(SHUTTER_VOICE_E voice);

/*****************************************************************************/
// 	Description : get shutter voice enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SHUTTER_VOICE_E MMIDC_GetShutterVoice(void);

/*****************************************************************************/
// 	Description : get self shoot time enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SELF_SHOOT_E MMIDC_GetSelfShootDelayTime(void);

/*****************************************************************************/
// 	Description : set self shoot time enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSelfShootDelayTime(SELF_SHOOT_E time);

/*****************************************************************************/
// 	Description : get photo effect enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_EFFECT_E MMIDC_GetPhotoEffect(void);

/*****************************************************************************/
// 	Description : set photo effect enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoEffect(DC_EFFECT_E effect);

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CONTRAST_E MMIDC_GetContrast(void);

/*****************************************************************************/
// 	Description : set photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetContrast(CONTRAST_E contrast);

/*****************************************************************************/
// 	Description : get photo zoom value enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetPhotoZoomValue(void);

/*****************************************************************************/
// 	Description : set photo zoom value enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_UpdatePhotoZoomValue(uint32 zoom);

/*****************************************************************************/
// 	Description : get need display photo preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC PHOTO_DISPLAY_SETTING_TIP_E MMIDC_GetNeedPhotoDisplaySettingTip(void);

/*****************************************************************************/
// 	Description : set need display photo preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNeedPhotoDisplaySettingTip(PHOTO_DISPLAY_SETTING_TIP_E tip);

/*****************************************************************************/
// 	Description : get video file type
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_FILE_TYPE_E MMIDC_GetRecordFileType(void);

/*****************************************************************************/
// 	Description : set video file type
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_E type);

/*****************************************************************************/
// 	Description : get video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetVideoStorageDevice(void);

/*****************************************************************************/
// 	Description : set video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoStorageDevice(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
// 	Description : get video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetPrefVideoStorageDevice(void);

/*****************************************************************************/
// 	Description : set video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPrefVideoStorageDevice(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
// 	Description : get video effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_EFFECT_E MMIDC_GetVideoEffect(void);

/*****************************************************************************/
// 	Description : set video effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoEffect(DC_EFFECT_E effect);

/*****************************************************************************/
// 	Description : set video environment enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoEnvironment(ENVIRONMENT_E environment);

/*****************************************************************************/
// 	Description : get video environment enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC ENVIRONMENT_E MMIDC_GetVideoEnvironment(void);

/*****************************************************************************/
// 	Description : get video quality enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC PHOTO_QUALITY_E MMIDC_GetVideoQuality(void);

/*****************************************************************************/
// 	Description : set video quality enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoQuality(PHOTO_QUALITY_E quality);

/*****************************************************************************/
// 	Description : set video size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoSize(VIDEO_SIZE_E size);

/*****************************************************************************/
// 	Description : get video size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetVideoSize(void);

/*****************************************************************************/
// 	Description : get video brightness enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BRIGHTNESS_E MMIDC_GetVideoBrightness(void);

/*****************************************************************************/
// 	Description : set video brightness enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoBrightness(BRIGHTNESS_E video_brightness);

/*****************************************************************************/
// 	Description : get need display video preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_DISPLAY_SETTING_TIP_E MMIDC_GetNeedVideoDisplaySettingTip(void);

/*****************************************************************************/
// 	Description : set need display video preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNeedVideoDisplaySettingTip(VIDEO_DISPLAY_SETTING_TIP_E tip);

/*****************************************************************************/
// 	Description : get record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC AUDIO_SETTING_E MMIDC_GetVideoAudioSetting(void);

/*****************************************************************************/
// 	Description : set record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoAudioSetting(AUDIO_SETTING_E setting);

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetPhotoNameID(void);

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoNameID(uint32 id);

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetVideoNameID(void);

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoNameID(uint32 id);

/*****************************************************************************/
// 	Description : get iso value
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC ISO_E MMIDC_GetISO(void);

/*****************************************************************************/
// 	Description : set iso value
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetISO(ISO_E iso);

/*****************************************************************************/
// 	Description : get flicker value
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC FLICKER_E MMIDC_GetPhotoFlicker(void);

/*****************************************************************************/
// 	Description : set flicker value
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoFlicker(FLICKER_E cor);

/*****************************************************************************/
// 	Description : get seconds of delay shot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_GetSelfShootDelaySeconds(void);

/*****************************************************************************/
// 	Description : set preview white balance param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_E balance);

/*****************************************************************************/
// 	Description : get preview white balance param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC WHITE_BALANCE_E MMIDC_GetPhotoWhiteBalance(void);

/*****************************************************************************/
// 	Description : handle the message of dc window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_HandleLoseFocus(void);

/*****************************************************************************/
// 	Description : handle the message of dc window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_HandleGetFocus(void);

/*****************************************************************************/
// 	Description : set record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IncreaseZoomValue(void);

/*****************************************************************************/
// 	Description : set record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DecreaseZoomValue(void);

/*****************************************************************************/
// 	Description : get Video contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CONTRAST_E MMIDC_GetVideoContrast(void);

/*****************************************************************************/
// 	Description : set Video contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoContrast(CONTRAST_E contrast);

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC FLICKER_E MMIDC_GetVideoFlicker(void);

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoFlicker(FLICKER_E cor);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetLeftSoftkey(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetLeftSoftkey(GUI_RECT_T rect);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetRightSoftkey(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRightSoftkey(GUI_RECT_T rect);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetMiddleSoftkey(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMiddleSoftkey(GUI_RECT_T rect);

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsSettingParamCtrl(void);

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitSettingParamCtrl(void);

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_Assert(BOOLEAN res, void* file, uint32 line);

/*****************************************************************************/
// 	Description : dc engineer mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseStoreWin(void);

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_StartCallbackTimer(void);

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseCallbackTimer(void);

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_GetCallbackTimer(void);

/*****************************************************************************/
// 	Description : get screen height
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int16 MMIDC_GetPreviewHeight(void);

/*****************************************************************************/
// 	Description : get screen width
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int16 MMIDC_GetPreviewWidth(void);

/*****************************************************************************/
// 	Description : set screen height
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPreviewHeight(int16 height);

/*****************************************************************************/
// 	Description : set screen width 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPreviewWidth(int16 width);

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetPhotoSavePath(void);

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetVideoSavePath(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetLCDSizeMode(CAMERA_ENG_LCD_SIZE_MODE_E mode);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_LCD_SIZE_MODE_E MMIAPIDC_GetLCDSizeMode(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetPreviewMode(CAMERA_ENG_PREVIEW_MODE_E mode);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_PREVIEW_MODE_E MMIAPIDC_GetPreviewMode(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetVisualMode(CAMERA_ENG_VISUAL_MODE_E mode);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_VISUAL_MODE_E MMIAPIDC_GetVisualMode(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_TurnOnEngTestMode(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsEngTestModeTurnedOn(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_AssertMacro(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SCREEN_MODE_E MMIDC_GetDefaultScreenMode(void);

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_VISUAL_MODE_E MMIDC_GetDefaultVisualMode(void);

/*****************************************************************************/
// 	Description : get photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GePhotoSizetHorLcd(void);

/*****************************************************************************/
// 	Description : get photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GePhotoSizetVerLcd(void);

/*****************************************************************************/
// 	Description : init screen mode ,when init dc/dv param 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:    //因为SIZE，在退出DC后,会保存此参数
                 //需要根据SIZE，调整SCREEN MODE
/*****************************************************************************/
PUBLIC void MMIDC_InitScreenMode(void);

/*****************************************************************************/
// 	Description : get multi photo file name rect 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//   Param [Return]: GUI_RECT_T  struct  value
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetMultiPhotoRect(void);

/*****************************************************************************/
// 	Description : set multi photo file name rect 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//   Param [Input]: rect   GUI_RECT_T
/*****************************************************************************/
PUBLIC void MMIDC_SetMultiPhotoRect(GUI_RECT_T rect);

/*****************************************************************************/
// 	Description : set used lcd for camera
//	Global resource dependence : none
//  Author: jesse.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetLcdID(uint32 lcd_id);

/*****************************************************************************/
// 	Description : get used lcd for camera
//	Global resource dependence : none
//  Author: jesse.yu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetLcdID(void);

/*****************************************************************************/
// 	Description : init mmi dc information
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitInfo(void);

/*****************************************************************************/
// 	Description : get platform angle
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMIDC_GetPlatformAngle(void);

/*****************************************************************************/
// 	Description : get current screen layout style
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
//  Param:
//       Input:NONE
//       Output:MMIDC_LAYOUT_E 
/*****************************************************************************/
PUBLIC MMIDC_LAYOUT_E  MMIDC_GetLayoutStyle(void);

/*****************************************************************************/
// 	Description : set current lcd id screen width and height 
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetLogicalScreen(void);

/*****************************************************************************/
// 	Description : get max width of softkey(left, middle, right)
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetMaxWidthOfSoftkey(void);

/*****************************************************************************/
// 	Description : get max value of platform lcd screen size
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetLCDSize(void);

/*****************************************************************************/
// 	Description : get state for screen display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_SHOW_STATE_E MMIDC_GetShowState(void);

/*****************************************************************************/
// 	Description : set state for screen display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetShowState(MMIDC_SHOW_STATE_E show_state);

/*****************************************************************************/
// 	Description : set frame rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFrameRect(GUI_RECT_T  frame_rect);

/*****************************************************************************/
// 	Description : get frame rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetFrameRect(void);

/*****************************************************************************/
// 	Description : get count down seconds
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetCountDownSeconds(void);

/*****************************************************************************/
// 	Description : get max zoom  rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMaxZoomLevel(void);

/*****************************************************************************/
// 	Description : get physical lcd width and height
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetPhysicalLCDWidthAndHeight(uint16 *lcd_width,//out
                                                            uint16 *lcd_height, //out
                                                            DCAMERA_ROTATION_MODE_E *angle   //out
                                                            );
                                               
/*****************************************************************************/
// 	Description : get camera sensor angle
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIAPIDC_GetCameraSensorAngle(void);

/*****************************************************************************/
// 	Description : set camera sensor angle
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetSensorAngle(DCAMERA_ROTATION_MODE_E angle);

/*****************************************************************************/
// 	Description : environment is or not silent mode
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsSilentMode(void);

/*****************************************************************************/
// 	Description : get support size array
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
//  Param:
//      return: max photo size  
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetSupportSizeArray(MMIDC_PHOTO_SIZE_E *size_array  //out,maybe PNULL
                                                            );

/*****************************************************************************/
//  Description : get max photo size
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetCurMaxPhotoSize(void);

/*****************************************************************************/
// 	Description : convert dcamera size to mmi photo size
//	Global resource dependence : none
//  Author: robert.wang
//	Note:   
/*****************************************************************************/
PUBLIC DCAMERA_PARAM_PHOTO_SIZE_E MMIDC_ConvertPhotoSizeToDCameraSize(MMIDC_PHOTO_SIZE_E photo_size);

/*****************************************************************************/
// 	Description : convert dcamera size to mmi photo size
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_ConvertDCameraSizeToPhotoSize(DCAMERA_PARAM_PHOTO_SIZE_E dcamera_size);

/*****************************************************************************/
// 	Description : set switch frame index
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSwitchFrameIndex(FRAME_E index);

/*****************************************************************************/
// 	Description : get switch frame index
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC FRAME_E MMIDC_GetSwitchFrameIndex(void);

/*****************************************************************************/
// 	Description : set setting sensor id
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSettingSensorID(SETTING_SENSOR_ID_E sensor_id);

/*****************************************************************************/
// 	Description : get setting sensor id
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC SETTING_SENSOR_ID_E MMIDC_GetSettingSensorID(void);

/*****************************************************************************/
// 	Description : init switch sensor parameter
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitSwitchSensorParameter(void);

/*****************************************************************************/
// 	Description : get dcamera sensor number
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetDCameraSensorNumber(void);

/*****************************************************************************/
// 	Description : set dcamera sensor id by command
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetDCameraSensorID(SETTING_SENSOR_ID_E sensor_id);

/*****************************************************************************/
// 	Description : save setting info by switch sensor
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveSettingInfoBySwitchSensor(SETTING_SENSOR_ID_E  sensor_id);

/*****************************************************************************/
// 	Description : get burst number by photo size 
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC MULTI_SHOOT_E MMIDC_GetBurstNumberByPhotoSize(MMIDC_PHOTO_SIZE_E photo_size);

/*****************************************************************************/
// 	Description : get max photo size to burst
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetMaxPhotoSizeToBurst(MULTI_SHOOT_E shoot_number);

/*****************************************************************************/
// 	Description : init param ctrl 
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_InitParamCtrl(void);

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RegSettingNv(void);

/*****************************************************************************/
//  Description : get photo size width and height
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetPhotoSizeWidthAndHeight(MMIDC_PHOTO_SIZE_E photo_size, //in
                                                          MMIDC_SIZE_ATTRIBUTE_T *size_ptr //out
                                                         );
                                                         
/*****************************************************************************/
//  Description : get video size width and height
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetVideoSizeWidthAndHeight(VIDEO_SIZE_E video_size, //in
                                                          MMIDC_SIZE_ATTRIBUTE_T *size_ptr //out
                                                         );
                                                         
/*****************************************************************************/
//Description : get photo size or video size string
//Global resource dependence : none
//  Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetSizeString(uint16 size_index, //in
                                       MMI_STRING_T  *str_ptr //out
                                       );      
                                       
/*****************************************************************************/
//  Description : Backup Running Parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int16  MMIDC_BackupRunningParameter(DO_ACTION_E action
                                                        );

/*****************************************************************************/
//  Description : Restore Running Parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int16  MMIDC_RestoreRunningParameter(DO_ACTION_E action
                                                        );
                                                        
/*****************************************************************************/
//Description : set NV shutter voice enum
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNVShutterVoice(SHUTTER_VOICE_E voice);

/*****************************************************************************/
//Description : get NV shutter voice enum
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC SHUTTER_VOICE_E MMIDC_GetNVShutterVoice(void);

/*****************************************************************************/
//Description : get NV photo date
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC PHOTO_DATE_E MMIDC_GetNVPhotoDate(void);

/*****************************************************************************/
//Description : set NV photo date
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNVPhotoDate(PHOTO_DATE_E photo_date);

/*****************************************************************************/
//Description : get thumbnail photo size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetThumbnailPhotoSize(void);

/*****************************************************************************/
//Description : get camera sensor angle by id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIAPIDC_GetDCModuleSensorAngle(SETTING_SENSOR_ID_E id
                                                                            );
                                                                            
/*****************************************************************************/
//Description : set camera sensor angle by id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetDCModuleSensorAngle(DCAMERA_ROTATION_MODE_E angle,
                                                         SETTING_SENSOR_ID_E id
                                                        );
                                                        
/*****************************************************************************/
//  Description : get dc module sensor number
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32  MMIAPIDC_GetDCModuleSensorNumber(void);

/*****************************************************************************/
//Description : get nv default value
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Setting_GetNVDefaultValue(void);

/*****************************************************************************/
//Description : physical lcd is real hor or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsRealHorLcd(void);

/*****************************************************************************/
//Description : get software rotation angle
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMIDC_GetSoftwareRotationAngle(void);

/*****************************************************************************/
//Description : set software rotation angle
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSoftwareRotationAngle(LCD_ANGLE_E angle);

/*****************************************************************************/
//Description : get dc module software rotation angle
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetDCModuleSoftwareRotationAngle(void);

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//  Description : get dv max zoom  rect
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDVMaxZoomLevel(void);

/*****************************************************************************/
//  Description : set dv zoom value enum
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_UpdateDVZoomValue(uint32 zoom);

/*****************************************************************************/
//  Description : get dv zoom value enum
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDVZoomValue(void);

/*****************************************************************************/
//  Description : increase dv zoom value
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IncreaseDVZoomValue(void);

/*****************************************************************************/
//  Description : derease dv zoom value
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DecreaseDVZoomValue(void);
#endif //#if defined(MMIDC_F_DV_ZOOM)

/*****************************************************************************/
//  Description : get screen photo size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetScreenPhotoSize(void);

/*****************************************************************************/
//  Description : get min video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetMinVideoSize(void);

/*****************************************************************************/
//  Description : get max video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetCurMaxVideoSize(void);

/*****************************************************************************/
//Description : get support video size array
//lobal resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetSupportVideoSizeArray(int16 max_item, //in
                                                        VIDEO_SIZE_E *size_list_ptr //out
                                                        );
														
/*****************************************************************************/
//  Description : get max burst photo number
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetMaxBurstPhotoNumber(void);														
														
/*****************************************************************************/
//  Description : get data format info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetDataFormatInfo(MMIDC_DATA_FORMAT_INFO_T *info_ptr//out
                                             );
                                             
/*****************************************************************************/
//  Description : set dcam source display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetDCamSrcDisplay(DCAMERA_DISPALY_PARAM_T src_disp);

/*****************************************************************************/
//  Description : get dcam source display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISPALY_PARAM_T MMIDC_GetDCamSrcDisplay(void);

/*****************************************************************************/
//  Description : set dcam destination display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetDCamDstDisplay(DCAMERA_DISPALY_PARAM_T dst_disp);

/*****************************************************************************/
//  Description : get dcam destination display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISPALY_PARAM_T MMIDC_GetDCamDstDisplay(void);

/*****************************************************************************/
//  Description : set mmi brush status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMMIBrushStatus(MMIDC_BRUSH_STATUS_E status);

/*****************************************************************************/
//  Description : get mmi brush status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_BRUSH_STATUS_E MMIDC_GetMMIBrushStatus(void);

/*****************************************************************************/
//  Description : mmi is brushing or not
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsMMIBrushing(void);

/*****************************************************************************/
//  Description : push to backup photo size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushPhotoSize(MMIDC_PHOTO_SIZE_E photo_size);

/*****************************************************************************/
//  Description : pop to get backuped photo size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_PopPhotoSize(void);

/*****************************************************************************/
//  Description : push to backup visual mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushVisualMode(CAMERA_ENG_VISUAL_MODE_E visual_mode);

/*****************************************************************************/
//  Description : pop to get backuped visual mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_VISUAL_MODE_E MMIDC_PopVisualMode(void);

/*****************************************************************************/
//  Description : init dc module setting info to default value
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitSettingInfoToDefaultValue(void);

/*****************************************************************************/
//  Description : set mmi focus status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMMIFocusStatus(MMIDC_FOCUS_STATUS_E status);

/*****************************************************************************/
//  Description : get mmi focus status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_FOCUS_STATUS_E MMIDC_GetMMIFocusStatus(void);

/*****************************************************************************/
// 	Description : get jpeg data size by photo size 
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetJPEGSizeByPhotoSize(MMIDC_PHOTO_SIZE_E photo_size);

/*****************************************************************************/
//  Description : create file handle mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateFileHandleMutexPtr(void);

/*****************************************************************************/
//  Description : delete file handle mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteFileHandleMutexPtr(void);

/*****************************************************************************/
//  Description : push file handle mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushFileHandleMutex(void);

/*****************************************************************************/
//  Description : pop file handle mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopFileHandleMutex(void);

/*****************************************************************************/
//  Description : create img proc mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateImgProcMutexPtr(void);

/*****************************************************************************/
//  Description : delete img proc mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteImgProcMutexPtr(void);

/*****************************************************************************/
//  Description : push img proc mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushImgProcMutex(void);

/*****************************************************************************/
//  Description : pop Img Proc mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopImgProcMutex(void);

/*****************************************************************************/
//Description : set photo size parameter 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoSizeParameter(MMIDC_PHOTO_SIZE_E size);

#ifdef MMIDC_MINI_RESOURCE_SIZE
/*****************************************************************************/
// 	Description : get setting photo size info
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
PUBLIC SETTING_PHOTO_SIZE_INFO_T * MMIDC_GetSettingPhotoSizeInfo(void);

/*****************************************************************************/
//Description :  get setting photo size info
//Global resource dependence : none
//Author: chunyou
//Note: size list from high size to low size
// Return: total supported number
/*****************************************************************************/
PUBLIC int16 MMIDC_SetSettingPhotoSizeInfo(void);

/*****************************************************************************/
// 	Description : get setting video size info
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
PUBLIC SETTING_VIDEO_SIZE_INFO_T * MMIDC_GetSettingVideoSizeInfo(void);

/*****************************************************************************/
//Description :  get setting video size info
//Global resource dependence : none
//Author: chunyou
//Note: size list from high size to low size
// Return: total supported number
/*****************************************************************************/
PUBLIC int16 MMIDC_SetSettingVideoSizeInfo(void);
#endif //MMIDC_MINI_RESOURCE_SIZE

/*****************************************************************************/
//Description :  get sensor actual resolution snapshot size
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC DCAMERA_PARAM_PHOTO_SIZE_E MMIDC_GetActualResolutionSize(void);

/*****************************************************************************/
//Description :  init sensor param
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC void MMIDC_InitSensorParam(void);

/*****************************************************************************/
//Description :  is 2M sensor state
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Is2MSensorState(void);

/*****************************************************************************/
//Description :  check 2M sensor state
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Check2MSensorState(MMIDCSRV_MEM_CFG_E mem_cfg);

#ifdef MMIDC_F_FLASH
/*****************************************************************************/
// 	Description : get dc or dv flash lamp
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_FLASH_MODE_E MMIDC_GetFlashLamp(void);

/*****************************************************************************/
// 	Description : set capture flash lamp param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetFlashLamp(DCAMERA_FLASH_MODE_E flash);
#endif

/*****************************************************************************/
//  Description : get Cap angle from dcam
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIDC_GetCapAngleFromDCam(void);
#endif

