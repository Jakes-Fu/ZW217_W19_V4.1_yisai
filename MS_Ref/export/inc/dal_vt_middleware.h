/******************************************************************************
 ** File Name:      dal_vt_middleware.h 
 ** DATE:           12/18/2009
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.
 ** Description:    This file defines the marcro, struct, enums and controling
 **                 API supplied by VT middleware for VT application.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE                 DESCRIPTION 
 ** 12/18/2009           Create.
 ******************************************************************************/
 #ifndef _DAL_VT_MIDDLEWARE_H_
#define _DAL_VT_MIDDLEWARE_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies           			                  *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
// TODO : define macor below.


/**---------------------------------------------------------------------------*
 **                      Enum and Structure Declaration                       *
 **---------------------------------------------------------------------------*/
// return type of VT middleware.
typedef enum
{
    DVT_MW_RET_SUCCESS  = 0,
    DVT_MW_RET_FINISHED,
    DVT_MW_RET_NOT_SUPPORTED,
    DVT_MW_RET_MEMORY_ERR,
    DVT_MW_RET_HARDWARE_ERR,
    DVT_MW_RET_SENSOR_ERR,
    DVT_MW_RET_INVALID_PARAM,
    DVT_MW_RET_INVALID_STATE,
    DVT_MW_RET_INVALID_IMAGE,
    DVT_MW_RET_FAIL,
    DVT_MW_RET_DISK_FULL,   //--recorder disk is almost full.
    DVT_MW_RET_FILE_FULL,   //--recoreder file is too bigger.
    DVT_MW_RET_FILE_ERR,    //--file operation error.
    DVT_MW_RET_MAX
}DVT_MW_RET_E;

// rotation angle(clockwise).
typedef enum
{
    DVT_MW_ROTATE_0     = 0,
    DVT_MW_ROTATE_90,
    DVT_MW_ROTATE_180,
    DVT_MW_ROTATE_270,
    DVT_MW_ROTATE_360,
    //
    DVT_MW_ROTATE_H_MIRROR,
    DVT_MW_ROTATE_V_MIRROR,
    //
    DVT_MW_ROTATE_MAX
}DVT_MW_ROTATEANGLE_E;

// audio/video object controled,eg. local audio/video,remote audio/video.
typedef enum
{
    DVT_MW_LOCAL_AUDIO  = 1,
    DVT_MW_LOCAL_VIDEO  = (1<<1),
    DVT_MW_REMOTE_AUDIO = (1<<2),
    DVT_MW_REMOTE_VIDEO = (1<<3)
}DVT_MW_AVOBJECT_E;

// quality of video encoding.
typedef enum
{
    DVT_MW_VENC_Q_LOWEST    = 0,
    DVT_MW_VENC_Q_LOW,
    DVT_MW_VENC_Q_MIDDLE,
    DVT_MW_VENC_Q_HIGH,
    DVT_MW_VENC_Q_HIGHEST
}DVT_MW_VIDEO_ENC_Q_E;

// SENSOR ID.
typedef enum
{
    DVT_MW_SENSOR_NONE,
    DVT_MW_SENSOR_MAIN,
    DVT_MW_SENSOR_SUB,
    DVT_MW_SENSOR_MAX
}DVT_MW_SENSOR_ID_E;

typedef enum
{
    DVT_MW_BRIGHT_LOWEST,
    DVT_MW_BRIGHT_LEVEL1,
    DVT_MW_BRIGHT_LEVEL2,
    DVT_MW_BRIGHT_LEVEL3,
    DVT_MW_BRIGHT_LEVEL4,
    DVT_MW_BRIGHT_HIGHTEST
}DVT_MW_BRIGHTNESS_E;

typedef enum
{
    DVT_MW_CONTRAST_LOWEST,
    DVT_MW_CONTRAST_LEVEL1,
    DVT_MW_CONTRAST_LEVEL2,
    DVT_MW_CONTRAST_LEVEL3,
    DVT_MW_CONTRAST_LEVEL4,
    DVT_MW_CONTRAST_HIGHTEST
}DVT_MW_CONTRAST_E;

typedef enum
{
    DVT_MW_ZOOM_OUT4    = 4,
    DVT_MW_ZOOM_OUT3    = 8,
    DVT_MW_ZOOM_OUT2    = 12,
    DVT_MW_ZOOM_NONE    = 16,
    DVT_MW_ZOOM_IN2     = 20,
    DVT_MW_ZOOM_IN3     = 24,
    DVT_MW_ZOOM_IN4     = 28,
    DVT_MW_ZOOM_MAX     = 32
}DVT_MW_ZOOM_E;

typedef enum
{
    // sensor effect.
    DVT_MW_SENSOR_EFFECT_BRIGHT,
    DVT_MW_SENSOR_EFFECT_CONTRAST,
    DVT_MW_SENSOR_EFFECT_SHARPNESS,
    DVT_MW_SENSOR_EFFECT_SATURATION,
    DVT_MW_SENSOR_EFFECT_ZOOM,
    // other parameter.
    //set the angle from screen to sensor,refer to DVT_MW_ROTATEANGLE_E.
    DVT_MW_SENSOR_SCREEN2SENSOR_ANGLE,
    //
    DVT_MW_SENSOR_EFFECT_MAX
}DVT_MW_SENSOR_EFFECT_E;

typedef enum
{
    DVT_MW_DISP_EFFECT_BRIGHT,
    DVT_MW_DISP_EFFECT_CONTRAST,
    DVT_MW_DISP_EFFECT_MAX
}DVT_MW_DISP_EFFECT_E;

typedef enum
{
    DVT_MW_FILE_ACCESS_DISK,
    DVT_MW_FILE_ACCESS_MEM,
    DVT_MW_FILE_ACCESS_NET,
    DVT_MW_FILE_ACCESS_MAX
}DVT_MW_FILE_ACCESS_E;

typedef enum
{
    DVT_MW_FILE_FMT_3GP,
    DVT_MW_FILE_FMT_MP4,
    DVT_MW_FILE_FMT_JPG,
    DVT_MW_FILE_FMT_AVI,
    DVT_MW_FILE_FMT_MAX
}DVT_MW_FILE_FMT_E;

typedef enum
{
    DVT_MW_REP_MOD_FULL,
    DVT_MW_REP_MOD_MIX,
    DVT_MW_REP_MOD_MAX
}DVT_MW_REPLACE_MODE_E;

typedef struct
{
    int32   sx; // start pos at x direction.
    int32   sy; // start pos at y direction.
    int32   dx; // distance at x direction.
    int32   dy; // distance at y direction.
}DVT_MW_LCD_RECT_T;

typedef struct
{
    int     lcd_id; 
    int     lcd_blockid;
    int     lcd_block_layer; 
    int     lcd_block_transparency; // 0: not transparent,overlay on the bottom,1: half transparent.

    DVT_MW_LCD_RECT_T       lcd_rect;           // the rect in LCD.
    DVT_MW_ROTATEANGLE_E    rotation;           // the image angle in rect.
    DVT_MW_ROTATEANGLE_E    rect_angle_in_lcd;  // the rect angle in LCD.
}DVT_MW_DISP_RECT_T;

typedef struct
{
    DVT_MW_SENSOR_ID_E      sensor_id;
    DVT_MW_ROTATEANGLE_E    screen2sensor_angle;
    //uint32  width;
    //uint32  height;
    //uint32  framerate;
}DVT_MW_SENSOR_PARAM_T;

typedef union
{
    struct
    {
        uint16   *p_file_fullname;
        uint32  name_len;
    }file_disk_fullname;

    struct
    {
        DVT_MW_FILE_FMT_E   file_fmt;
        uint8               *p_addr;
        uint32              data_len;
    }file_mem_addr;

}DVT_MW_FILE_PATH_U;

typedef struct
{
    DVT_MW_FILE_ACCESS_E    access_type;
    DVT_MW_FILE_PATH_U      file_path;
    int                     audio_permit;   // play audio or not.
    int                     video_permit;   // play video or not.
}DVT_MW_MOVIEINFO_T;

typedef struct
{
    DVT_MW_FILE_ACCESS_E    access_type;
    DVT_MW_FILE_PATH_U      file_path;
    uint32                  record_object;
}DVT_MW_RECORDINFO_T;

typedef struct
{
    DVT_MW_FILE_ACCESS_E    access_type;
    DVT_MW_FILE_PATH_U      file_path;

    uint32                  replace_object;
    DVT_MW_REPLACE_MODE_E   rep_mode_a;
    DVT_MW_REPLACE_MODE_E   rep_mode_v;
}DVT_MW_REPLACEINFO_T;


typedef void (*DVT_MW_ERROR_CALLBACK)(DVT_MW_RET_E err_type);

/**---------------------------------------------------------------------------*
 **                       API Functions Declaration                           *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : open the VT middleware.
//	input       : pf_err_notify, callback function.when error happend,middleware
//                will callback to notify what's wrong with middleware.
//  output      : DVT_MW_RET_E.
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_Open(DVT_MW_ERROR_CALLBACK pf_err_notify);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_Close(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_MovieRingStart(DVT_MW_MOVIEINFO_T movie_info, 
                                                    DVT_MW_DISP_RECT_T *p_disp_rect);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_MovieRingStop(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_SelectSensor(DVT_MW_SENSOR_ID_E sensor_id);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_SelectSensor_Ex(DVT_MW_SENSOR_PARAM_T *p_sensor_para);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_GetSensorParam(DVT_MW_SENSOR_EFFECT_E cmd,
                                                        void *p_param);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_SetSensorParam(DVT_MW_SENSOR_EFFECT_E cmd,
                                                        void *p_param);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_GetRemoteDispEffect(DVT_MW_DISP_EFFECT_E cmd, 
                                                            void *p_param);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_SetRemoteDispEffect(DVT_MW_DISP_EFFECT_E cmd,
                                                            void *p_param);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_SetVideoEncQuality(DVT_MW_VIDEO_ENC_Q_E v_enc_quality);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_PreviewStart(DVT_MW_DISP_RECT_T *p_disp_rect);

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_PreviewStop(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_SetDisplayArea(DVT_MW_DISP_RECT_T *p_disp_remote,
                                                    DVT_MW_DISP_RECT_T *p_disp_local);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_DialStart(void);

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_DialStop(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_RecordStart(DVT_MW_RECORDINFO_T *p_record_info);

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_RecordStop(void);



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_ReplaceStart(DVT_MW_REPLACEINFO_T *p_replace_info);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_ReplaceStop(uint32 replace_object);

#if 0
/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_ReplaceLocalStart(DVT_MW_REPLACEINFO_T *p_replace_info);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_ReplaceLocalStop(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_ReplaceRemoteStart(DVT_MW_REPLACEINFO_T *p_replace_info) ;

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_ReplaceRemoteStop(void);
#endif

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_EnableMute(BOOLEAN is_mute_en, 
                                                DVT_MW_AVOBJECT_E mute_obj);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_InitOSD(void);

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_MW_RET_E DVT_MW_ReleaseOSD(void);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*_DAL_VT_MIDDLEWARE_H_*/

