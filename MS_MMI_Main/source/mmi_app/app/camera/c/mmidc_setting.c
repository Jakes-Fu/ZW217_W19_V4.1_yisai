#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef  CAMERA_SUPPORT
#include "os_api.h"
//#include "sci_types.h"
#include "mn_type.h"
#include "dal_recorder.h"
#include "dal_dcamera.h"
#include "mmidc_setting_nv.h"
#include "mmifmm_export.h"
#include "mmidc_export.h"
#include "mmidc_camera_id.h"
#include "mmi_text.h"
#include "mmi_position.h"
#include "mmk_timer.h"

#include "mmisd_export.h"

//local module
#include "mmidc_gui.h"
#include "mmidc_main.h"
#include "mmidc_flow.h"
#include "mmidc_display.h"
#include "mmidc_save.h"
#include "mmidc_setting.h"
#include "mmiudisk_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "guilcd.h"
#include "mmidc_option.h"
#include "sensor_drv.h"
#ifdef MMIDC_F_WORK_MODE
#include "mmidc_workmode.h"
#endif

#ifdef MMIDC_F_U_CAMERA
#include "mmidc_imageprocess.h"   
#endif
#include "mmidc_srv.h"

#ifdef WIN32
//#define SENSOR_ROTATION_0
#endif

typedef struct 
{
    int8                             is_setting_param_ctrl;
    int8					         setting_number;
    uint8				             callback_timer;
    
    uint32                           dc_preview_width;
    uint32                           dc_preview_height;

    GUI_RECT_T                       left_rect;
    GUI_RECT_T                       right_rect;
    GUI_RECT_T                       middle_rect;
    GUI_RECT_T                       multi_photo_rect;
    
    MMIDC_OSD_SETTING_INFO_T         dc_setting_info;
}MMIDC_SETTING_DATA;

static MMIDC_SETTING_DATA*              s_setting_data_ptr = PNULL;
static CAMERA_ENG_LCD_SIZE_MODE_E       s_lcd_size_mode = 0; /*lint !e64*/
static BOOLEAN                          s_is_eng_turned_on = FALSE;

#define MMIDC_SETTING_DATA_PTR       (PNULL != s_setting_data_ptr ? s_setting_data_ptr : (MMIDC_SETTING_DATA*)MMIDC_AssertMacro())
LOCAL uint32 g_mmidc_lcd_id = MAIN_LCD_ID;

LOCAL MMIDC_INFO_T s_mmidc_info = {0}; 



typedef struct
{
    wchar str[SIZE_STR_LENGTH+1];
}MMIDC_SIZE_STR_T;


LOCAL MMIDC_SIZE_ATTRIBUTE_T   s_photo_size_val_array[]=
{
    {128, 160},//PHOTO_SIZE_128X160,
 	{160, 128},//PHOTO_SIZE_160X128,
	{176, 220},//PHOTO_SIZE_176X220,
    {220, 176},//PHOTO_SIZE_220X176,
    {240, 320},//PHOTO_SIZE_240X320,
    {240, 400},//PHOTO_SIZE_240X400,
    {320, 240},//PHOTO_SIZE_320X240,
    {320, 480},// PHOTO_SIZE_320X480,
    {400, 240},//PHOTO_SIZE_400X240,    
    {480, 320},//PHOTO_SIZE_480X320,
    {480, 640},//PHOTO_SIZE_480X640,    
    {640, 480},//PHOTO_SIZE_640X480,
    {960, 1280},//PHOTO_SIZE_960X1280,
    {1280, 960},//PHOTO_SIZE_1280X960,
    {1600, 1200},//PHOTO_SIZE_1600X1200,
    {2048, 1536},//PHOTO_SIZE_2048X1536,
    {2592, 1944},//PHOTO_SIZE_2592X1944,
    {3264, 2448} //PHOTO_SIZE_3264X2448,
};

LOCAL MMIDC_SIZE_ATTRIBUTE_T   s_video_size_val_array[]=
{
    {96, 128},//VIDEO_SIZE_96X128,
    {128, 160},//VIDEO_SIZE_128X160,
    {144, 176},//VIDEO_SIZE_144X176,
    {176, 240},//VIDEO_SIZE_176X240,
    {240, 320},//VIDEO_SIZE_240X320,
    {128, 96},//VIDEO_SIZE_128X96,  
    {160, 128},//VIDEO_SIZE_160X128,
    {176, 144},//VIDEO_SIZE_176X144,
    {240, 176},//VIDEO_SIZE_240X176,
    {320, 240},//VIDEO_SIZE_320X240,
#ifdef MMIDC_F_VIDEO_352X288    
    {352, 288}//VIDEO_SIZE_352X288,
#endif
};

LOCAL MMIDC_SIZE_STR_T s_photo_size_str_array[PHOTO_SIZE_MAX] = {0};
LOCAL MMIDC_SIZE_STR_T s_video_size_str_array[VIDEO_SIZE_MAX] = {0};

LOCAL MMIDC_FIXED_PARAMETER_T s_fixed_param = 
{
    {DCAMERA_IMG_SENSOR_TYPE_MAX,DCAMERA_IMG_SENSOR_TYPE_MAX},
    SENSOR_NUMBER_INVALID
};

#ifdef DC_TESTPATTERN_SUPPORT
    #define TEST_PATTERN_MODE TRUE
#else
    #define TEST_PATTERN_MODE FALSE
#endif

#if 0
DRECORDER_RETURN_E DRECORDER_Open (void) {return DRECORDER_SUCCESS;}
DRECORDER_RETURN_E DRECORDER_Close (void) {return DRECORDER_SUCCESS;}
//xxx
//DRECORDER_RETURN_E DRECORDER_StartRecord(DRECORDER_START_RECORD_T const * record_ptr) {return DRECORDER_SUCCESS;}
//DRECORDER_RETURN_E DRECORDER_StopRecord (void) {return DRECORDER_SUCCESS;}
//DRECORDER_RETURN_E DRECORDER_StartPreview(DRECORDER_PREVIEW_PARAM_T const * param_ptr) {return DRECORDER_SUCCESS;}
//DRECORDER_RETURN_E DRECORDER_StopPreview (void) {return DRECORDER_SUCCESS;}
DRECORDER_RETURN_E DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_CMD_E param_cmd, uint32 arg, DRECORDER_PARAM_CTRL_CALL_BACK callback) 
{
    callback(0, 0);
    return DRECORDER_SUCCESS;
}
DRECORDER_RETURN_E DRECORDER_Pause (void) {return DRECORDER_SUCCESS;}
DRECORDER_RETURN_E DRECORDER_Resume (void) {return DRECORDER_SUCCESS;}
DRECORDER_RETURN_E DRECORDER_GetInfo(DRECORDER_INFO_CMD_E	param_cmd, uint32*	info_ptr) {return DRECORDER_SUCCESS;}
#endif

/*****************************************************************************/
// 	Description : get lcd info for camera sensor
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void GetCameraLcdInfo(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_AssertMacro(void)    
{
    SCI_ASSERT(0); /*assert verified*/
    return 0; /*lint !e527 */
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_AllocSettingMemory(void)
{
    if(PNULL == s_setting_data_ptr)
    {
        s_setting_data_ptr = (MMIDC_SETTING_DATA*)SCI_ALLOC_APP(sizeof(MMIDC_SETTING_DATA));
        SCI_MEMSET(s_setting_data_ptr, 0, sizeof(MMIDC_SETTING_DATA));
        s_setting_data_ptr->is_setting_param_ctrl = 0;
        s_is_eng_turned_on = FALSE;
    }
    //SCI_TRACE_LOW:"[MMIDC]: MMIAPIDC_AllocSettingMemory s_setting_data_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_167_112_2_18_2_9_44_590,(uint8*)"d", s_setting_data_ptr);
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_FreeSettingMemory(void)
{
	//SCI_TRACE_LOW:"[MMIDC]: MMIAPIDC_FreeSettingMemory s_setting_data_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_176_112_2_18_2_9_44_591,(uint8*)"d", s_setting_data_ptr);

	MMIDC_FreeDCImageInfoMemory();
	
	/*change nv storage to alloc mem  type*/
	if(PNULL != s_setting_data_ptr->dc_setting_info.photo_file_path_ptr)
	{	
		SCI_FREE(s_setting_data_ptr->dc_setting_info.photo_file_path_ptr);
	}
	if(PNULL != s_setting_data_ptr->dc_setting_info.video_file_path_ptr)
	{	
		SCI_FREE(s_setting_data_ptr->dc_setting_info.video_file_path_ptr);
	}
    
    if(PNULL != s_setting_data_ptr)
    {        
        SCI_FREE(s_setting_data_ptr);
        s_setting_data_ptr = PNULL;
    }



}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveSettings(void)
{
    SETTING_SENSOR_ID_E sensor_id = MMIDC_GetSettingSensorID();
    int16 sensor_number = 0;
    BOOLEAN is_save = TRUE;

#if defined(MMIDC_F_WORK_MODE)    
    if (MMIDC_IsWorkMode())
    {
        is_save = FALSE;
    }
#endif    

   
    if (is_save)
    {
        sensor_number = MMIAPIDC_GetDCModuleSensorNumber();
        if (1 == sensor_number)
        {
            MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_size = MMIDC_GetPhotoSize();
            MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_size = MMIDC_GetVideoSize();
        }
        else
        {
            //save postpositive sensor setting to NV
            if (SETTING_SENSOR_REAR == sensor_id)
            {
                MMIDC_SaveSettingInfoBySwitchSensor(SETTING_SENSOR_REAR);
            }
        }

        MMINV_WRITE(MMINV_DC_SET_INFO, &(MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info));
    }        

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SaveSettings is_save=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_224_112_2_18_2_9_44_592,(uint8*)"d",is_save);
}

/*****************************************************************************/
// 	Description : get screen height
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int16 MMIDC_GetPreviewHeight(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_preview_height;
}

/*****************************************************************************/
// 	Description : get screen width
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int16 MMIDC_GetPreviewWidth(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_preview_width;
}

/*****************************************************************************/
// 	Description : set screen height
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPreviewHeight(int16 height)
{
    MMIDC_SETTING_DATA_PTR->dc_preview_height = height;
}

/*****************************************************************************/
// 	Description : set screen width 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPreviewWidth(int16 width)
{
    MMIDC_SETTING_DATA_PTR->dc_preview_width = width;
}

/*****************************************************************************/
// 	Description : init screen mode ,when init dc/dv param 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:    //因为SIZE，在退出DC后,会保存此参数
                 //需要根据SIZE，调整SCREEN MODE
/*****************************************************************************/
PUBLIC void MMIDC_InitScreenMode(void)
{   
    if (CAMERA_MODE_DC == MMIDC_GetCameraMode())
    {
       if(SCREEN_MODE_HORIIZONTAL == MMIDC_GetDefaultScreenMode()) /*lint !e64*/
        {
            if(MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd())
            {
#ifdef MMIDC_DESKTOP_PDA
                if(LCD_ANGLE_90 == MMIAPIDC_GetCameraSensorAngle())
                {
                    MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetHorLcd());
                }
                else
                {
                    MMIAPIDC_SetPhotoSize(PHOTO_SIZE_320X240);
                }
#else
                MMIDC_SetScreenMode(SCREEN_MODE_VERTICAL);
#endif
            }
        }
        else
        {
            //当默认竖屏时，若是竖屏不支持的尺寸，
            //修改为竖屏支持的尺寸，QVGA除外
            if ((PHOTO_SIZE_320X240 != MMIDC_GetPhotoSize()) 
                && MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetHorLcd()
               )
            {
               MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetVerLcd());
            }
        }
    }
}

/*****************************************************************************/
// 	Description : init photo size string array
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void MMIDC_InitSizeStringArray(MMIDC_SIZE_STR_T *str_ptr, //out
                                           MMIDC_SIZE_ATTRIBUTE_T *size_ptr, //in
                                           uint16 total_num
                                           )
{
    int32 i = 0;
    wchar * dst = PNULL;
    size_t dst_max_count = 0;
    char * src = PNULL;
    size_t src_max_count = 0;
    size_t count = 0;
    char   char_size[SIZE_STR_LENGTH+1] = {0};

    for (i = 0; i < total_num; i++)
    {
        SCI_MEMSET(&str_ptr[i], 0x00, sizeof(str_ptr[0]));
        SCI_MEMSET(char_size, 0x00, sizeof(char_size));

        sprintf(char_size, "%dx%d", size_ptr[i].size_width, size_ptr[i].size_height);
        
        dst = str_ptr[i].str;
        src = char_size;
        src_max_count = strlen(src);
        dst_max_count = src_max_count;
        count = src_max_count;
        MMI_STRNTOWSTR(dst, dst_max_count, (uint8 *)src, src_max_count, count);
    }
}

/*****************************************************************************/
//Description : init photo size and video size string array
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitSizeArray(void)
{
    int16 total_num = 0;

    //check photo size val array
    total_num = ARR_SIZE(s_photo_size_val_array);
    if (PHOTO_SIZE_MAX != total_num) /*lint !e774*/
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] InitSizeArray PHOTO_SIZE_MAX != total_num !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_342_112_2_18_2_9_45_593,(uint8*)"");
        return ;
    }

    //check video size val array
    total_num = ARR_SIZE(s_video_size_val_array);
    if (VIDEO_SIZE_MAX != total_num) /*lint !e774*/
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] InitSizeArray VIDEO_SIZE_MAX != total_num !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_350_112_2_18_2_9_45_594,(uint8*)"");
        return ;
    }


    MMIDC_InitSizeStringArray(s_photo_size_str_array, s_photo_size_val_array, PHOTO_SIZE_MAX);
    MMIDC_InitSizeStringArray(s_video_size_str_array, s_video_size_val_array, VIDEO_SIZE_MAX);
}

/*****************************************************************************/
// 	Description : save setting sensor info
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
LOCAL void SaveSettingSensorInfo(SETTING_SENSOR_ID_E  sensor_id, //in
                                      MMIDC_OSD_SETTING_INFO_T *dc_setting_info //in
                                      )
{
    if (SETTING_SENSOR_REAR == sensor_id)
    {
        //for postpositive sensor setting
        dc_setting_info->setting_nv_info.photo_size = MMIDC_GetPhotoSize();
        dc_setting_info->setting_nv_info.video_size = MMIDC_GetVideoSize();
    }
    else
    {
        //for prepositive sensor setting
        ;
    }
}

/*****************************************************************************/
// 	Description : restore setting sensor info
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
LOCAL void RestoreSettingSensorInfo(SETTING_SENSOR_ID_E  sensor_id, //in
                                          MMIDC_OSD_SETTING_INFO_T *dc_setting_info //in
                                          )
{
    //the sensor maybe only is sub sensor,
    //and restore photo size.
    dc_setting_info->photo_size = dc_setting_info->setting_nv_info.photo_size;
    dc_setting_info->video_size = dc_setting_info->setting_nv_info.video_size;
}

/*****************************************************************************/
//Description : check video size 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void CheckVideoSize(MMIDC_OSD_SETTING_INFO_T *dc_setting_info //in
                              )
{
    
    VIDEO_SIZE_E    video_size = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;
    VIDEO_FILE_TYPE_E  file_type = 0;
    VIDEO_SIZE_E max_video_size = MMIDC_GetCurMaxVideoSize();
    

    video_size = dc_setting_info->video_size;
    file_type = dc_setting_info->setting_nv_info.video_file_type;
    sensor_type = MMIAPIDC_GetDCModulSensorType();

    //SCI_TRACE_LOW:"[MMIDC] CheckVideoSize file_type = %d, video_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_410_112_2_18_2_9_45_595,(uint8*)"dd", file_type, video_size);
    if (DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
    {
        //DCAMERA_IMG_SENSOR_TYPE_4P3
        if (VIDEO_FILE_TYPE_3GP == file_type || VIDEO_FILE_TYPE_AVI == file_type )
        {
            if (video_size < VIDEO_SIZE_128X96
                || video_size > max_video_size
                )
            {
                video_size = max_video_size;
            }
        }
        else if(VIDEO_FILE_TYPE_MP4 == file_type)
        {
            if (VIDEO_SIZE_160X128 != video_size
                && VIDEO_SIZE_128X96 != video_size
                )
            {
                video_size = VIDEO_SIZE_160X128;
            }
        }
    }
    else
    {
        //DCAMERA_IMG_SENSOR_TYPE_3P4
        if (VIDEO_FILE_TYPE_3GP == file_type || VIDEO_FILE_TYPE_AVI == file_type )
        {
            if (video_size < VIDEO_SIZE_96X128
                || video_size > max_video_size
                )
            {    
                video_size = max_video_size;
            }
        }
        else if(VIDEO_FILE_TYPE_MP4 == file_type)
        {
            if (VIDEO_SIZE_128X160 != video_size
                && VIDEO_SIZE_96X128 != video_size
                )
            {    
                video_size = VIDEO_SIZE_128X160;
            }
        }
    }

    dc_setting_info->video_size = video_size;

    //SCI_TRACE_LOW:"[MMIDC] CheckVideoSize video_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_462_112_2_18_2_9_45_596,(uint8*)"d", video_size);
}

/*****************************************************************************/
//Description : init video size to default 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitVideoSizeToDefault(MMIDC_OSD_SETTING_INFO_T *dc_setting_info)
{
    
    VIDEO_SIZE_E    video_size = 0;    


    video_size = MMIDC_GetCurMaxVideoSize();

    dc_setting_info->video_size = video_size;

    //SCI_TRACE_LOW:"[MMIDC] InitVideoSizeToDefault video_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_479_112_2_18_2_9_45_597,(uint8*)"d", video_size);
}

/*****************************************************************************/
// 	Description : init NV setting default value
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
LOCAL void MMIDC_InitNVSettingDefaultValue(MMIDC_OSD_SETTING_INFO_T *dc_setting_info, BOOLEAN is_switch_sensor)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_InitNVSettingDefaultValue"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_488_112_2_18_2_9_45_598,(uint8*)"");
    SCI_TRACE_LOW("[MMIDC]: MMIDC_InitNVSettingDefaultValue is_switch_sensor = %d", is_switch_sensor);

    dc_setting_info->screen_mode = dc_setting_info->setting_nv_info.default_screen_mode;
    
    dc_setting_info->photo_zoom = 0;    
    dc_setting_info->frame	= FRAME_DISABLE;

    if(!is_switch_sensor)
    {
        dc_setting_info->photo_environment = ENVIRONMENT_AUTO;
        dc_setting_info->multi_shoot = MULTI_SHOOT_DISABLE;
        dc_setting_info->self_shoot = SELF_SHOOT_DISABLE;
        dc_setting_info->photo_effect = DC_EFFECT_NORMAL;
        dc_setting_info->photo_brightness = BRIGHTNESS_LEVEL_3;
        dc_setting_info->contrast = CONTRAST_3;
        dc_setting_info->exposal_compensate = EXPOSAL_COMPENSATE_ZERO;    
        dc_setting_info->photo_white_balance = WHITE_BALANCE_AUTO;
        dc_setting_info->shoot_mode = SHOOT_MODE_NORMAL;
    }

#if defined(MMIDC_F_U_CAMERA)
    if (!MMIDC_GetUCameraModuleFlag())
#endif
    {    
        dc_setting_info->video_zoom = 0;        

        if(!is_switch_sensor)
        {
            dc_setting_info->video_effect = DC_EFFECT_NORMAL;
            dc_setting_info->video_brightness = BRIGHTNESS_LEVEL_3;
            dc_setting_info->video_environment	= ENVIRONMENT_AUTO;
            dc_setting_info->video_contrast = CONTRAST_3;
        }
    }

	//SCI_TRACE_LOW:"[MMIDC] MMIDC_InitNVSettingDefaultValue ,screen_mode=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_518_112_2_18_2_9_45_599,(uint8*)"d",dc_setting_info->setting_nv_info.default_screen_mode);


} 

/*****************************************************************************/
//  Description : init storage device
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void InitStorageDevice(MMIDC_OSD_SETTING_INFO_T *dc_setting_info_ptr //out
                                )
{
    //MMIFILE_DEVICE_E res = MMI_DEVICE_NUM;
    //BOOLEAN  udisk = FALSE;
    //BOOLEAN  sd = FALSE;
    MMIFILE_DEVICE_E photo_storage_dev = 0;
    MMIFILE_DEVICE_E video_storage_dev = 0;

    photo_storage_dev = dc_setting_info_ptr->setting_nv_info.photo_storage_dev;
    video_storage_dev = dc_setting_info_ptr->setting_nv_info.video_storage_dev;
    
    //added, @robert.wang, 09-12-3, cr159180
    // DC 存储位置为 手机时
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev(photo_storage_dev, MMI_DC_MIN_SPACE_SIZE, &photo_storage_dev))
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dc_setting_info_ptr->setting_nv_info.photo_storage_dev),
                                   MMIAPIFMM_GetDevicePathLen(dc_setting_info_ptr->setting_nv_info.photo_storage_dev)))
        {
            photo_storage_dev = dc_setting_info_ptr->setting_nv_info.photo_storage_dev;
        }
    }
    
    dc_setting_info_ptr->setting_nv_info.photo_storage_dev = photo_storage_dev;

//     MMIDC_CombinePath(dc_setting_info_ptr->setting_nv_info.photo_file_path,
//                                   MMIDC_FULL_PATH_LENGTH, photo_storage_dev,
//                                   MMIDC_GetPhotoDefaultPathName());

	dc_setting_info_ptr->photo_file_path_ptr = (wchar*)SCI_ALLOC_APPZ(sizeof(wchar) * MMIDC_FULL_PATH_LENGTH);
    if(PNULL != dc_setting_info_ptr->photo_file_path_ptr)
	{
		MMIDC_CombinePath(dc_setting_info_ptr->photo_file_path_ptr,\
									   MMIDC_FULL_PATH_LENGTH, photo_storage_dev,\
									   MMIDC_GetPhotoDefaultPathName());
	}

    //DV 存储位置为手机时
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev(video_storage_dev, MMI_DC_MIN_SPACE_SIZE, &video_storage_dev))
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dc_setting_info_ptr->setting_nv_info.video_storage_dev),
                                   MMIAPIFMM_GetDevicePathLen(dc_setting_info_ptr->setting_nv_info.video_storage_dev)))
        {
            video_storage_dev = dc_setting_info_ptr->setting_nv_info.video_storage_dev;
        }
    }

    #if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
    if(MMI_DEVICE_UDISK == video_storage_dev)
    {
        video_storage_dev = MMIAPIFMM_GetFirstValidSD();
    }
    #endif

    dc_setting_info_ptr->setting_nv_info.video_storage_dev = video_storage_dev;

    // MMIDC_CombinePath(dc_setting_info_ptr->setting_nv_info.video_file_path, MMIDC_FULL_PATH_LENGTH, video_storage_dev, MMIMULTIM_DIR_MOVIE);
    //end cr159180
	dc_setting_info_ptr->video_file_path_ptr = (wchar*)SCI_ALLOC_APPZ(sizeof(wchar) * MMIDC_FULL_PATH_LENGTH);
	if(PNULL != dc_setting_info_ptr->video_file_path_ptr)
	{
		MMIDC_CombinePath(dc_setting_info_ptr->video_file_path_ptr, MMIDC_FULL_PATH_LENGTH, video_storage_dev, MMIMULTIM_DIR_MOVIE);
	}
}

/*****************************************************************************/
//Description : get nv default value
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Setting_GetNVDefaultValue(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    CAMERA_MODE_E       temp = MMIDC_SETTING_DATA_PTR->dc_setting_info.camera_mode;
    BOOLEAN ret = TRUE;
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_Setting_GetNVDefaultValue"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_591_112_2_18_2_9_45_600,(uint8*)"");

    //use postpositive sensor
    MMIDC_SetSettingSensorID(SETTING_SENSOR_REAR);

    MMIDC_InitInfo();    
    
    if(s_is_eng_turned_on)
    {
        if(CAMERA_ENG_LCD_176X220 == s_lcd_size_mode)
        {
            if(MMIDC_SETTING_DATA_PTR->dc_preview_width >= 176 && MMIDC_SETTING_DATA_PTR->dc_preview_height >= 220)
            {
                if(CAMERA_ENG_FULL_VISUAL_ANGLE == MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.default_visual_mode)
                {
                    MMIDC_SETTING_DATA_PTR->dc_preview_width = 160;
                    MMIDC_SETTING_DATA_PTR->dc_preview_height = 212;
                }
                else
                {
                    MMIDC_SETTING_DATA_PTR->dc_preview_width = 176;
                    MMIDC_SETTING_DATA_PTR->dc_preview_height = 220;
                }
            }
        }
        else if(CAMERA_ENG_LCD_240X320 == s_lcd_size_mode)
        {
            if(MMIDC_SETTING_DATA_PTR->dc_preview_width >= 240 && MMIDC_SETTING_DATA_PTR->dc_preview_height >= 320)
            {
                MMIDC_SETTING_DATA_PTR->dc_preview_width = 240;
                MMIDC_SETTING_DATA_PTR->dc_preview_height = 320;
            }
        }
        else if(CAMERA_ENG_LCD_240X400 == s_lcd_size_mode)
        {
            if(MMIDC_SETTING_DATA_PTR->dc_preview_width >= 240 && MMIDC_SETTING_DATA_PTR->dc_preview_height >= 400)
            {
                MMIDC_SETTING_DATA_PTR->dc_preview_width = 240;
                MMIDC_SETTING_DATA_PTR->dc_preview_height = 400;
            }
        }
    }
    MMINV_READ(MMINV_DC_SET_INFO, &(MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info), return_value);
    MMIDC_InitSettingParamCtrl();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_Setting_InitDefaultValue ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_636_112_2_18_2_9_45_601,(uint8*)"d", return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIDC_Setting_SetFactoryValue();
        MMINV_READ(MMINV_DC_SET_INFO, &(MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info), return_value);
        MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_file_name_id = 0;
        MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_file_name_id = 0;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.camera_mode = temp;

#ifdef MMIDC_F_WORK_MODE
    if (MMIDC_IsWorkMode())
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.auto_save = AUTO_SAVE_OFF;
    }    
#endif    


	InitStorageDevice(&MMIDC_SETTING_DATA_PTR->dc_setting_info);


    //reset NV setting to default value
    MMIDC_InitNVSettingDefaultValue(&MMIDC_SETTING_DATA_PTR->dc_setting_info, FALSE);

    //restore postpositive sensor info to default setting
    RestoreSettingSensorInfo(SETTING_SENSOR_REAR, &MMIDC_SETTING_DATA_PTR->dc_setting_info);

    MMIDC_InitScreenMode();

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_Setting_GetNVDefaultValue ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_665_112_2_18_2_9_45_602,(uint8*)"d",ret);

    return ret;
} 

/*****************************************************************************/
// 	Description : init dc settings when open dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_Setting_InitDefaultValue(void)
{
    SHUTTER_VOICE_E voice = 0;
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_Setting_InitDefaultValue"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_678_112_2_18_2_9_45_603,(uint8*)"");


#ifndef CMCC_UI_STYLE
    if (MMIDC_IsSilentMode())
    {
        voice = SHUTTER_VOICE_DISABLE;
    }
    else
#endif    
    {
        voice = MMIDC_GetNVShutterVoice();
    }
    MMIDC_SetShutterVoice(voice);
   

    //init photo size and video size array for string and value
    InitSizeArray();

    MMIDCSRV_CheckVideoSize();
    
    //check video size
    CheckVideoSize(&MMIDC_SETTING_DATA_PTR->dc_setting_info);
} 

/*****************************************************************************/
// 	Description : reset setting value by factory
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Setting_SetFactoryValue(void)
{
    MMIDC_OSD_SETTING_INFO_T dc_all_setting_info = {0};
    MMIDC_OSD_SETTING_INFO_T check_store_dev_info = {0};
    
    MMIDC_SETTING_NV_INFO_T dc_setting_info ={0};/*lint !e64*/
    DCAMERA_ROTATION_MODE_E sensor_angle = 0;
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIAPIDC_Setting_SetFactoryValue"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_712_112_2_18_2_9_46_604,(uint8*)"");

    dc_setting_info.shutter_voice = SHUTTER_VOICE_1;	//快门音（无，快门音一，快

#if defined(MMIDC_DESKTOP_PDA)
    dc_setting_info.auto_save = AUTO_SAVE_ON;      //手动保存（开，关）
#else
    dc_setting_info.auto_save = AUTO_SAVE_OFF;      //手动保存（开，关）
#endif
    dc_setting_info.photo_display_setting_tip = PHOTO_DISPLAY_SETTING_TIP_ON;
    dc_setting_info.photo_quality = PHOTO_QUALITY_NORMAL;
    dc_setting_info.photo_flicker = FLICKER_50;

#if defined(MMI_CAMERA_F_PHOTO_DATE)
    dc_setting_info.photo_date = PHOTO_DATE_ON;
#else
    dc_setting_info.photo_date = PHOTO_DATE_OFF;
#endif
    
    dc_setting_info.photo_size = PHOTO_SIZE_MAX; 
    
#ifdef AVI_ENC_SUPPORT
#ifdef CMCC_TEST_FLAG
	dc_setting_info.video_file_type = VIDEO_FILE_TYPE_MP4;
#else
	dc_setting_info.video_file_type = VIDEO_FILE_TYPE_AVI;
#endif
#else
    dc_setting_info.video_file_type = VIDEO_FILE_TYPE_3GP;
#endif

    //inti default video size
    dc_all_setting_info.setting_nv_info = dc_setting_info;
    dc_setting_info.video_size = VIDEO_SIZE_MAX;

#ifdef CAMERA_SENSOR_ANGLE_90
    sensor_angle = DCAMERA_ROTATION_90;
#elif CAMERA_SENSOR_ANGLE_0
    sensor_angle = DCAMERA_ROTATION_0;
#else
    sensor_angle = DCAMERA_ROTATION_0;
#endif

    dc_setting_info.sensor_angle[SETTING_SENSOR_FRONT] = sensor_angle;
    dc_setting_info.sensor_angle[SETTING_SENSOR_REAR] = sensor_angle;
    
    
    dc_setting_info.video_display_setting_tip = VIDEO_DISPLAY_SETTING_TIP_ON;
    dc_setting_info.video_flicker = FLICKER_50;
    dc_setting_info.audio_setting = AUDIO_SETTING_ON;

#ifdef CAMERA_SCREEN_LANDSCAPE
    dc_setting_info.default_screen_mode = SCREEN_MODE_HORIIZONTAL;
#elif CAMERA_SCREEN_PORTRAIT
    dc_setting_info.default_screen_mode = SCREEN_MODE_VERTICAL;
#else
    dc_setting_info.default_screen_mode = SCREEN_MODE_VERTICAL;
#endif

    dc_setting_info.default_visual_mode = CAMERA_ENG_FULL_SCREEN;

    //check valid storage dev first
    check_store_dev_info.setting_nv_info.photo_storage_dev = MMI_DEVICE_SDCARD;
    check_store_dev_info.setting_nv_info.video_storage_dev = MMI_DEVICE_SDCARD;
    InitStorageDevice(&check_store_dev_info);
    dc_setting_info.photo_storage_dev       = check_store_dev_info.setting_nv_info.photo_storage_dev;
    dc_setting_info.video_storage_dev       = check_store_dev_info.setting_nv_info.video_storage_dev;
    dc_setting_info.pref_photo_storage_dev  = check_store_dev_info.setting_nv_info.photo_storage_dev;
    dc_setting_info.pref_video_storage_dev  = check_store_dev_info.setting_nv_info.video_storage_dev;
    
    dc_setting_info.dc_flash_lamp = DCAMERA_FLASH_OFF;
    dc_setting_info.dv_flash_lamp = DCAMERA_FLASH_OFF;
   
	MMINV_WRITE(MMINV_DC_SET_INFO, &dc_setting_info);

	//SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_Setting_SetFactoryValue sensor_angle 1 = %d,screen_mode=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_775_112_2_18_2_9_46_605,(uint8*)"dd",dc_setting_info.sensor_angle[0],dc_setting_info.default_screen_mode);
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetPhotoSavePath(void)
{
	// return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_file_path;
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_file_path_ptr;
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPIDC_GetPhotoSavePath(void)
{
    return MMIDC_GetPhotoSavePath();
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIDC_GetVideoSavePath(void)
{
	// return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_file_path;
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.video_file_path_ptr;	
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_StartCallbackTimer(void)
{
    if(0 == MMIDC_SETTING_DATA_PTR->callback_timer)
    {
        MMIDC_SETTING_DATA_PTR->callback_timer = MMK_CreateTimer(40000, TRUE);
    }
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseCallbackTimer(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_CloseCallbackTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_823_112_2_18_2_9_46_606,(uint8*)"");

    if(0 != MMIDC_SETTING_DATA_PTR->callback_timer)
    {
        MMK_StopTimer(MMIDC_SETTING_DATA_PTR->callback_timer);
        MMIDC_SETTING_DATA_PTR->callback_timer = 0;
    }
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_GetCallbackTimer(void)
{
    return MMIDC_SETTING_DATA_PTR->callback_timer;
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL uint32 ParamCtrlCallBack(void* param_ptr, uint32 param_size)
{
    //SCI_TRACE_LOW:"[MMIDC]: ParamCtrlCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_847_112_2_18_2_9_46_607,(uint8*)"");

    if (MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl > 0)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
    }
    
    MMIDC_CloseCallbackTimer();
    if(MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl > 0)
    {
        MMIDC_StartCallbackTimer();
    }
    return 0;
}

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetPhotoNameID(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_file_name_id;
}

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoNameID(uint32 id)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_file_name_id = id;
}

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetVideoNameID(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_file_name_id;
}

/*****************************************************************************/
// 	Description : get need shoot prompt
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoNameID(uint32 id)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_file_name_id = id;
}

/*****************************************************************************/
// 	Description : get need display photo preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC PHOTO_DISPLAY_SETTING_TIP_E MMIDC_GetNeedPhotoDisplaySettingTip(void)
{
	if(PHOTO_DISPLAY_SETTING_TIP_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_display_setting_tip)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_display_setting_tip = PHOTO_DISPLAY_SETTING_TIP_ON;
	}

	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_display_setting_tip;
}

/*****************************************************************************/
// 	Description : set need display photo preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNeedPhotoDisplaySettingTip(PHOTO_DISPLAY_SETTING_TIP_E tip)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_display_setting_tip = tip;
}

/*****************************************************************************/
// 	Description : get dc screen mode 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SCREEN_MODE_E MMIDC_GetScreenMode(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.screen_mode;
}

/*****************************************************************************/
// 	Description : set dc screen mode 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetScreenMode(SCREEN_MODE_E mode)
{
    if (mode >= SCREEN_MODE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetScreenMode mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_933_112_2_18_2_9_46_608,(uint8*)"");
        return;
    }
    
    if(mode != MMIDC_SETTING_DATA_PTR->dc_setting_info.screen_mode)
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.screen_mode = mode;
    }
}

/*****************************************************************************/
// 	Description : get photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetPhotoSize(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size;
}

/*****************************************************************************/
// 	Description : set photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetPhotoSize(MMIDC_PHOTO_SIZE_E size)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32 size_param = DCAMERA_PHOTO_SIZE_MAX;
    MMIDC_PHOTO_SIZE_E  cur_photo_size = 0;

    if (size >= PHOTO_SIZE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIAPIDC_SetPhotoSize size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_966_112_2_18_2_9_46_609,(uint8*)"d", size);
        return;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_SetPhotoSize size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_977_112_2_18_2_9_46_610,(uint8*)"d", size);


    //switch size, should update zoom to 0.
    cur_photo_size = MMIDC_GetPhotoSize();
    if((size != cur_photo_size)
       && (cur_photo_size < PHOTO_SIZE_MAX)
      )       
    {                                   
        MMIDC_PostCreateFileHandleMsg();
    
        MMIDC_UpdatePhotoZoomValue(0);     
    }                                      

#if defined(MMIDC_F_U_CAMERA)
    //u camera do not set photo size
    if (!MMIDC_IsUCameraEnable())
#endif    
    {
        if(MMIDC_IsDCOpen())
        {    
            size_param = MMIDC_ConvertPhotoSizeToDCameraSize(size);
            if (size_param < DCAMERA_PHOTO_SIZE_MAX)
            {
                MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
                MMIDC_StartCallbackTimer();
                ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_SET_TARGET_SIZE, size_param, ParamCtrlCallBack);
                //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_SetPhotoSize ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1002_112_2_18_2_9_46_611,(uint8*)"d", ret);

                if(ret != DCAMERA_OP_SUCCESS)
                {
                    MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
                    MMIDC_SETTING_DATA_PTR->setting_number --;

                    MMIDC_PostStateMsg();
                    MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
                    return;
                }
            }
            
        }
    }
    if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
    {
        if(size != MMIDC_GePhotoSizetHorLcd())
        {
            MMIDC_SetFrameIndex(FRAME_DISABLE, FALSE);
        }
        else if (!MMIDC_IsRealHorLcd() && LCD_ANGLE_0 == MMIAPIDC_GetCameraSensorAngle())
        {
            MMIDC_SetFrameIndex(FRAME_DISABLE, FALSE);
        }
    }
    else if(MMIDC_GetScreenMode() == SCREEN_MODE_VERTICAL)
    {
        if(size != MMIDC_GePhotoSizetVerLcd())
        {
            MMIDC_SetFrameIndex(FRAME_DISABLE, FALSE);
        }
    }

    MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size = size;
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get frame index
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC FRAME_E MMIDC_GetFrameIndex(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.frame;
}

/*****************************************************************************/
// 	Description : set frame index
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFrameIndex(FRAME_E frame, BOOLEAN is_preview)
{

    if ( frame >= FRAME_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetFrameIndex frame"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1086_112_2_18_2_9_46_613,(uint8*)"");
        return;
    }

    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetFrameIndex frame=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1098_112_2_18_2_9_46_614,(uint8*)"d",frame);
    
    MMIDC_SETTING_DATA_PTR->dc_setting_info.frame = frame;    
    if(MMIAPIDC_IsPreviewing())
    {
        if(frame != FRAME_DISABLE)
        {
            if(!is_preview)
            {
                if(MMIDC_GetScreenMode() == SCREEN_MODE_VERTICAL)
                {
                    MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetVerLcd());
                }
                else
                {
                   MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetHorLcd());
                }
               
                MMIDC_UpdatePhotoZoomValue(0);
                MMIDC_SetMultiShootEnum(MULTI_SHOOT_DISABLE);
            }

            //if screen is horizontal and sensor is 0, 
            //not support horizontal frame, change to vertical frame
            //need to restart preview
            if ((LCD_ANGLE_0 == MMIAPIDC_GetCameraSensorAngle())
                 && (SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode())
                 && (!MMIDC_IsRealHorLcd())
                )
            {
                //need to restart preview
                MMIDC_SetSwitchFrameIndex(frame);
                SetPhotoSizeState(MMIDC_GePhotoSizetVerLcd());
            }
        }
    }
    MMIDC_SETTING_DATA_PTR->setting_number --;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetFrameIndex is_preview=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1136_112_2_18_2_9_46_615,(uint8*)"d",is_preview);
}

/*****************************************************************************/
// 	Description : get shoot mode like normal or panorama
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SHOOT_MODE_E MMIDC_GetShootMode(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.shoot_mode;
}

/*****************************************************************************/
// 	Description : set shoot mode like normal or panorama
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetShootMode(SHOOT_MODE_E mode)
{
    if (mode >= SHOOT_MODE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetShootMode mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1156_112_2_18_2_9_46_616,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.shoot_mode = mode;
    MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetHorLcd());
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get multi shoot enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MULTI_SHOOT_E MMIDC_GetMultiShootEnum(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.multi_shoot;
}

/*****************************************************************************/
// 	Description : set multi shoot enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMultiShootEnum(MULTI_SHOOT_E shoot)
{
    MULTI_SHOOT_E      max_photo_number = 0;
    MMIDC_PHOTO_SIZE_E photo_size = 0;

    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3 || shoot == MMIDC_SETTING_DATA_PTR->dc_setting_info.multi_shoot)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetMultiShootEnum shoot=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1197_112_2_18_2_9_46_617,(uint8*)"d",shoot);
    
    MMIDC_SETTING_DATA_PTR->dc_setting_info.multi_shoot = shoot;
    MMIDC_SetFrameIndex(FRAME_DISABLE, FALSE);
    //MMIDC_UpdatePhotoZoomValue(0);

    MMIDC_SETTING_DATA_PTR->setting_number --;

    if (MULTI_SHOOT_DISABLE != shoot)
    {
        //if photo size is not permited burst
        //then set another photo size to be permited burst
        photo_size = MMIDC_GetPhotoSize();                                         
        max_photo_number = MMIDC_GetBurstNumberByPhotoSize(photo_size);           
        if (max_photo_number < shoot)
        {
            photo_size = MMIDC_GetMaxPhotoSizeToBurst(shoot);
            SetPhotoSizeState(photo_size);
        }

        MMIDC_PostCreateFileHandleMsg();
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetMultiShootEnum photo_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1219_112_2_18_2_9_47_618,(uint8*)"d",photo_size);
}

/*****************************************************************************/
// 	Description : get photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetPhotoStorageDevice(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_storage_dev;
}

/*****************************************************************************/
// 	Description : set photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoStorageDevice(MMIFILE_DEVICE_E dev)
{      
    if (dev >= MMI_DEVICE_NUM)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetPhotoStorageDevice dev"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1239_112_2_18_2_9_47_619,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_storage_dev = dev;
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetPrefPhotoStorageDevice(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.pref_photo_storage_dev;
}

/*****************************************************************************/
// 	Description : set photo storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPrefPhotoStorageDevice(MMIFILE_DEVICE_E dev)
{      
    if (dev >= MMI_DEVICE_NUM)
    {
        SCI_TRACE_LOW("[MMIDC]MMIDC_SetPrefPhotoStorageDevice dev=%d",dev);
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.pref_photo_storage_dev = dev;
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo quality
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC PHOTO_QUALITY_E MMIDC_GetPhotoQuality(void)
{
	if(PHOTO_QUALITY_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_quality) /*lint !e685*/
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_quality = PHOTO_QUALITY_BRIEF;
	}
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_quality;
}

/*****************************************************************************/
// 	Description : set photo quality
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoQuality(PHOTO_QUALITY_E quality)
{
    if (quality >= PHOTO_QUALITY_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetPhotoQuality quality"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1270_112_2_18_2_9_47_620,(uint8*)"");
        return ;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_quality = quality;
}

/*****************************************************************************/
// 	Description : is auto save
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC AUTO_SAVE_E MMIDC_GetIsAutoSave(void)
{
    if(AUTO_SAVE_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.auto_save)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.auto_save = AUTO_SAVE_ON;
	}
	
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.auto_save;
}

/*****************************************************************************/
// 	Description : set auto save
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetAutoSave(AUTO_SAVE_E promt)
{
    if (promt >= AUTO_SAVE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetAutoSave promt"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1293_112_2_18_2_9_47_621,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.auto_save = promt;
}

/*****************************************************************************/
// 	Description : get dc mode like photo or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_MODE_E MMIDC_GetCameraMode(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.camera_mode;
}

/*****************************************************************************/
// 	Description : get video file type
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_FILE_TYPE_E MMIDC_GetRecordFileType(void)
{
    //return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_file_type;
#if defined(PLATFORM_UWS6121E)
    return VIDEO_FILE_TYPE_3GP;
#else
    return VIDEO_FILE_TYPE_MP4;
#endif
}

/*****************************************************************************/
// 	Description : set video file type
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRecordFileType(VIDEO_FILE_TYPE_E type)
{
    if (type >= VIDEO_FILE_TYPE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetRecordFileType type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1326_112_2_18_2_9_47_622,(uint8*)"");
        return ;
    }

    //AVI和3GP有相同的size 组，MP4有单独的SIZE组
    //TYPE 被修改为其他类型
    if(MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_file_type != type)
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_file_type = type;

        InitVideoSizeToDefault(&MMIDC_SETTING_DATA_PTR->dc_setting_info);

#if defined(MMIDC_F_WORK_MODE)
        if (MMIDC_IsWorkMode())
        {
            MMIDC_SetCheckWorkParamFlag(TRUE);
        }
#endif  
        MMIDCSRV_CheckVideoSize();

#ifdef MMIDC_MINI_RESOURCE_SIZE
        MMIDC_SetSettingVideoSizeInfo();
#endif
		MMIDC_UpdateDVZoomValue(0);
    }
}

/*****************************************************************************/
// 	Description : get video size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetVideoSize(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.video_size;
}

/*****************************************************************************/
// 	Description : set video size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoSize(VIDEO_SIZE_E size)
{
    if (size >= VIDEO_SIZE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetVideoSize size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1364_112_2_18_2_9_47_623,(uint8*)"");
        return ;
    }

#if defined(MMIDC_F_DV_ZOOM)
    {
        BOOLEAN ret = FALSE;
        VIDEO_SIZE_E cur_video_size = 0;

        //switch size, should update zoom to 0.
        cur_video_size = MMIDC_GetVideoSize();
        if((size != cur_video_size)
           && (cur_video_size < VIDEO_SIZE_MAX)
          )       
        {                                      
            ret = MMIDC_UpdateDVZoomValue(0);
            if (!ret)
            {
                return;
            }
        } 
    }
#endif    
    
    MMIDC_SETTING_DATA_PTR->dc_setting_info.video_size = size;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetVideoSize size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1390_112_2_18_2_9_47_624,(uint8*)"d", size);
}

/*****************************************************************************/
// 	Description : get video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetVideoStorageDevice(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_storage_dev;
}

/*****************************************************************************/
// 	Description : set video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoStorageDevice(MMIFILE_DEVICE_E dev)
{
    if (dev >= MMI_DEVICE_NUM)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetVideoStorageDevice dev"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1410_112_2_18_2_9_47_625,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_storage_dev = dev;
}

/*****************************************************************************/
// 	Description : get video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIDC_GetPrefVideoStorageDevice(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.pref_video_storage_dev;
}

/*****************************************************************************/
// 	Description : set video storage device
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPrefVideoStorageDevice(MMIFILE_DEVICE_E dev)
{
    if (dev >= MMI_DEVICE_NUM)
    {
        SCI_TRACE_LOW("[MMIDC]MMIDC_SetPrefVideoStorageDevice dev=%d", dev);
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.pref_video_storage_dev = dev;
}

/*****************************************************************************/
// 	Description : get photo zoom value enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_UpdatePhotoZoomValue(uint32 zoom)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 4)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }	
    
    if(MMIDC_IsDCOpen())
    {
        uint32 zoom_max = 0;
       
        zoom_max = MMIDC_GetMaxZoomLevel();
        if(zoom_max > 1)
        {
            MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
            MMIDC_StartCallbackTimer();
            if(MMIDC_GetFrameIndex() != FRAME_DISABLE || zoom >= zoom_max)
            {
                zoom = 0;
            }
            ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_ZOOM, zoom, ParamCtrlCallBack);	
        }
        else
        {
            MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
            MMIDC_StartCallbackTimer();
            ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_ZOOM, 0, ParamCtrlCallBack);
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetDCZoomValue return: %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1460_112_2_18_2_9_47_627,(uint8*)"dd", ret, zoom);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
         MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
         MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_zoom = zoom;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo zoom value enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetPhotoZoomValue(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_zoom;
}

/*****************************************************************************/
// 	Description : set capture shoot scene param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetShootScene(SHOOT_SCENE_E scene)
{
    if (scene >= SHOOT_SCENE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetShootScene scene"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1492_112_2_18_2_9_47_628,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.scene = scene;
    {
        switch(scene)
        {
        case SHOOT_SCENE_AUTO:
            break;
            
        case SHOOT_SCENE_HIGHT_LIGHT:
            break;
            
        case SHOOT_SCENE_LOW_LIGHT:
            break;
            
        case SHOOT_SCENE_PEOPLE:
            break;
            
            //case SHOOT_SCENE_LOW_LIGHT_PEOPLE:
            //break;
            
        case SHOOT_SCENE_NIGHTS:
            break;
            
        case SHOOT_SCENE_SAND_BEACH:
            break;
            
        case SHOOT_SCENE_SPORTS:
            break;
            
        case SHOOT_SCENE_FILE:
            break;
        default:
            //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetShootScene 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1533_112_2_18_2_9_47_629,(uint8*)"");
            break;
        }
    }
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

#ifdef MMIDC_F_FLASH
/*****************************************************************************/
// 	Description : get dc or dv flash lamp
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_FLASH_MODE_E MMIDC_GetFlashLamp(void)
{
    DCAMERA_FLASH_MODE_E cur_flash = DCAMERA_FLASH_OFF;
    
    if(CAMERA_MODE_DC == MMIDC_GetCameraMode())
    {
		if(DCAMERA_FLASH_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dc_flash_lamp)
		{
			MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dc_flash_lamp = DCAMERA_FLASH_OFF;
		}
		
		cur_flash = MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dc_flash_lamp;
    }
    else if(CAMERA_MODE_DV == MMIDC_GetCameraMode())
    {
		if(DCAMERA_FLASH_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dv_flash_lamp)
		{
			MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dv_flash_lamp = DCAMERA_FLASH_OFF;
		}
		
		cur_flash = MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dv_flash_lamp;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetFlashLamp cur_flash = %d", cur_flash);
    return cur_flash;
}

/*****************************************************************************/
// 	Description : set capture flash lamp param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetFlashLamp(DCAMERA_FLASH_MODE_E flash)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    
    if (flash >= DCAMERA_FLASH_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetFlashLamp flash"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1548_112_2_18_2_9_47_630,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }

    if(MMIDC_IsDCOpen() 
        || MMIDC_IsDVOpen())
    {    
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLASH, flash, ParamCtrlCallBack);
        SCI_TRACE_LOW("[MMIDC] MMIDC_SetFlashLamp ret = %d", ret);

        if(ret != DCAMERA_OP_SUCCESS)
        {
            MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
            MMIDC_SETTING_DATA_PTR->setting_number --;

            MMIDC_PostStateMsg();
            MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
            return;
        }
        
    }
    if(DCAMERA_FLASH_NONE != flash)
    {
        if(CAMERA_MODE_DC == MMIDC_GetCameraMode())
        {
            MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dc_flash_lamp = flash;
        }
        else if(CAMERA_MODE_DV == MMIDC_GetCameraMode())
        {
            MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.dv_flash_lamp = flash;
        }
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetFlashLamp flash = %d", flash);
}
#endif

/*****************************************************************************/
// 	Description : set photo exposal compensate enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetExposalCompensate(EXPOSAL_COMPENSATE_E exposal_compensate)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;

    if (exposal_compensate >= EXPOSAL_COMPENSATE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetExposalCompensate exposal_compensate"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1572_112_2_18_2_9_47_631,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetExposalCompensate setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1588_112_2_18_2_9_47_632,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, exposal_compensate, ParamCtrlCallBack);	
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetExposalCompensate compentsate return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1597_112_2_18_2_9_47_633,(uint8*)"dd", ret, exposal_compensate);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.exposal_compensate = exposal_compensate;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo exposal compensate enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC EXPOSAL_COMPENSATE_E MMIDC_GetExposalCompensate(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.exposal_compensate;
}

/*****************************************************************************/
// 	Description : get self shoot time enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSelfShootDelayTime(SELF_SHOOT_E time)
{
    if (time >= SELF_SHOOT_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetSelfShootDelayTime time"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1629_112_2_18_2_9_47_634,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.self_shoot = time;
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : set self shoot time enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SELF_SHOOT_E MMIDC_GetSelfShootDelayTime(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.self_shoot;
}

/*****************************************************************************/
// 	Description : set self shoot time enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_GetSelfShootDelaySeconds(void)
{
    uint8 sec = 0;
    if(SELF_SHOOT_3S == MMIDC_SETTING_DATA_PTR->dc_setting_info.self_shoot)
    {
        sec = 3;
    }
    else if(SELF_SHOOT_10S == MMIDC_SETTING_DATA_PTR->dc_setting_info.self_shoot)
    {
        sec = 10;
    }
    return sec;
}
/*****************************************************************************/
// 	Description : get dc param effect
//	Global resource dependence : none
//  Author: kunliu
//	Note:
/*****************************************************************************/
LOCAL uint32 GetDCParmEffect(DC_EFFECT_E effect)
{    
    DCAMERA_PARAM_EFFECT_E type = DCAMERA_EFFECT_NORMAL;
    
    switch(effect)
    {
        case DC_EFFECT_NORMAL:
        {
            type = DCAMERA_EFFECT_NORMAL;
        }
        break;
        
        case DC_EFFECT_BLACKWHITE:
        {
            type = DCAMERA_EFFECT_BLACKWHITE;
        }
        break;
        
        case DC_EFFECT_BLUE:
        {
            type = DCAMERA_EFFECT_BLUE;
        }
        break;
        
        case DC_EFFECT_GREEN:
        {
            type = DCAMERA_EFFECT_GREEN;
        }
        break;
        
        case DC_EFFECT_YELLOW:
        {
            type = DCAMERA_EFFECT_YELLOW;
        }
        break;
        
        case DC_EFFECT_RED:
        {
            type = DCAMERA_EFFECT_RED;
        }
        break;
        
        case DC_EFFECT_CANVAS:
        {
            type = DCAMERA_EFFECT_CANVAS;
        }
        break;
        
        case DC_EFFECT_NEGATIVE:
        {
            type = DCAMERA_EFFECT_NEGATIVE;
        }
        break;
        
        case DC_EFFECT_RELIEVOS:
        {
            type = DCAMERA_EFFECT_RELIEVOS;
        }
        break;
       
        default:
        break;
        
    }
    
    return (uint32)type;
}

/*****************************************************************************/
// 	Description : set photo effect enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoEffect(DC_EFFECT_E effect)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32  arg = 0;

    if (effect >= DC_EFFECT_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetPhotoEffect effect"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1752_112_2_18_2_9_48_635,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen())
    {
         if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetPhotoEffect setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1768_112_2_18_2_9_48_636,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        arg = GetDCParmEffect(effect);
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EFFECT, arg, ParamCtrlCallBack);		
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetPhotoEffect return: %d, effect = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1778_112_2_18_2_9_48_637,(uint8*)"dd", ret, effect);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_effect = effect;        
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo effect enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_EFFECT_E MMIDC_GetPhotoEffect(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_effect;
}

/*****************************************************************************/
// 	Description : get video effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_EFFECT_E MMIDC_GetVideoEffect(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.video_effect;
}

/*****************************************************************************/
// 	Description : set video effect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoEffect(DC_EFFECT_E effect)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    uint32 arg = 0;
    
    if (effect >= DC_EFFECT_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetVideoEffect effect"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1822_112_2_18_2_9_48_638,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDVOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetVideoEffect setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1838_112_2_18_2_9_48_639,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();        
        arg = GetDCParmEffect(effect);
        ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_EFFECT, arg, ParamCtrlCallBack);
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetVideoEffect return: %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1848_112_2_18_2_9_48_640,(uint8*)"dd", ret, effect);
    
    if (DRECORDER_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.video_effect = effect;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : set shutter voice enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetShutterVoice(SHUTTER_VOICE_E voice)
{
    if (voice >= SHUTTER_VOICE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetShutterVoice voice"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1871_112_2_18_2_9_48_641,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.shutter_voice = voice;
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get shutter voice enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SHUTTER_VOICE_E MMIDC_GetShutterVoice(void)
{
	if(SHUTTER_VOICE_3 < MMIDC_SETTING_DATA_PTR->dc_setting_info.shutter_voice)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.shutter_voice = SHUTTER_VOICE_1;
	}

	return MMIDC_SETTING_DATA_PTR->dc_setting_info.shutter_voice;
}

/*****************************************************************************/
// 	Description : set preview EnviroNment
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoEnvironment(ENVIRONMENT_E environment)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    DCAMERA_PARAM_ENVIRONMENT_E  env_param = 0; 

    if (environment >= ENVIRONMENT_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetPhotoEnvironment environment"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1905_112_2_18_2_9_48_642,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetPhotoEnvironment setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1921_112_2_18_2_9_48_643,(uint8*)"");
            return;
        }
        
        switch(environment)
        {
        case ENVIRONMENT_NIGHT:
            env_param = DCAMERA_ENVIRONMENT_NIGHT;
            break;
        case ENVIRONMENT_AUTO:
            env_param = DCAMERA_ENVIRONMENT_NORMAL;
            break;
        default:
            env_param = DCAMERA_ENVIRONMENT_NORMAL;
            break;
        }
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_PREVIEW_MODE, env_param, ParamCtrlCallBack);
        if(TEST_PATTERN_MODE)
        {
            ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_TESTPATTERN_MODE, DCAMERA_ENVIRONMENT_TEST_PATTERN, ParamCtrlCallBack);
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: SetphotoEnvironment return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1942_112_2_18_2_9_48_644,(uint8*)"dd", ret, environment);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_environment = environment;   
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo environment enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC ENVIRONMENT_E MMIDC_GetPhotoEnvironment(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_environment;
}

/*****************************************************************************/
// 	Description : set video environment enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoEnvironment(ENVIRONMENT_E environment)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    DRECORDER_PARAM_ENVIROMENT_E  env_param = 0;

    if (environment >= ENVIRONMENT_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetVideoEnvironment environment"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1977_112_2_18_2_9_48_645,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDVOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetVideoEnvironment setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_1993_112_2_18_2_9_48_646,(uint8*)"");
            return;
        }
        
        switch(environment)
        {
        case ENVIRONMENT_NIGHT:
            env_param = DRECORDER_ENVIROMENT_NIGHT;
            break;
        case ENVIRONMENT_AUTO:
            env_param = DRECORDER_ENVIROMENT_NORMAL;
            break;
        default:
            env_param = DRECORDER_ENVIROMENT_NORMAL;
            break;
        }

        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_PRVIEW_MODE, env_param, ParamCtrlCallBack);
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: SetvideoEnvironment return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2015_112_2_18_2_9_48_647,(uint8*)"dd", ret, environment);

    if (DRECORDER_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.video_environment = environment;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get video environment enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC ENVIRONMENT_E MMIDC_GetVideoEnvironment(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.video_environment;
}

/*****************************************************************************/
// 	Description : set photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetContrast(CONTRAST_E contrast)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;

    if (contrast >= CONTRAST_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetContrast contrast"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2049_112_2_18_2_9_48_648,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetContrast setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2065_112_2_18_2_9_48_649,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, contrast, ParamCtrlCallBack);	
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: SetphotoContrast return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2074_112_2_18_2_9_48_650,(uint8*)"dd", ret, contrast);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.contrast = contrast;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : set Video contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoContrast(CONTRAST_E contrast)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

    if (contrast >= CONTRAST_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetVideoContrast contrast"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2099_112_2_18_2_9_48_651,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDVOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetVideoContrast setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2115_112_2_18_2_9_48_652,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_CONTRAST, contrast, ParamCtrlCallBack);	
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetVideoContrast return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2124_112_2_18_2_9_48_653,(uint8*)"dd", ret, contrast);

    if (DRECORDER_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.video_contrast = contrast;   
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get Video contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CONTRAST_E MMIDC_GetVideoContrast(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.video_contrast;
}

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CONTRAST_E MMIDC_GetContrast(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.contrast;
}

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC FLICKER_E MMIDC_GetPhotoFlicker(void)
{
	if(FLICKER_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_flicker)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_flicker = FLICKER_50;
	}

    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_flicker;
}

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoFlicker(FLICKER_E cor)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;

    if (cor >= FLICKER_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetPhotoFlicker cor"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2176_112_2_18_2_9_48_654,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetPhotoFlicker setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2192_112_2_18_2_9_48_655,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, cor, ParamCtrlCallBack);	
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetPhotoFlicker return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2200_112_2_18_2_9_48_656,(uint8*)"dd", ret, cor);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_flicker = cor;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC FLICKER_E MMIDC_GetVideoFlicker(void)
{
	if(FLICKER_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_flicker)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_flicker = FLICKER_50;
	}

	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_flicker;
}

/*****************************************************************************/
// 	Description : get photo contrast enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoFlicker(FLICKER_E cor)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

    if (cor >= FLICKER_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetVideoFlicker cor"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2234_112_2_18_2_9_49_657,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDVOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetVideoFlicker setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2250_112_2_18_2_9_49_658,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_FLICKER_COMPENSATION, cor, ParamCtrlCallBack);	
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetVideoFlicker return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2258_112_2_18_2_9_49_659,(uint8*)"dd", ret, cor);
    
    if(DRECORDER_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_flicker = cor;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : set self shoot time enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC ISO_E MMIDC_GetISO(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.iso;
}

/*****************************************************************************/
// 	Description : set self shoot time enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetISO(ISO_E iso)
{
    if (iso >= ISO_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetISO iso"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2290_112_2_18_2_9_49_660,(uint8*)"");
        return ;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.iso = iso;
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : set preview white balance param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoWhiteBalance(WHITE_BALANCE_E balance)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetPhotoWhiteBalance setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2324_112_2_18_2_9_49_661,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE, balance, ParamCtrlCallBack);	
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: SetphotoWhiteBalance return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2333_112_2_18_2_9_49_662,(uint8*)"dd", ret, balance);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_white_balance = balance;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : set preview white balance param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC WHITE_BALANCE_E MMIDC_GetPhotoWhiteBalance(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_white_balance;
}

/*****************************************************************************/
// 	Description : set dc mode like photo or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCameraMode(CAMERA_MODE_E mode)
{
    if (mode >= CAMERA_MODE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetCameraMode mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2365_112_2_18_2_9_49_663,(uint8*)"");
        return ;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.camera_mode = mode;
}

/*****************************************************************************/
// 	Description : set preview zoom param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoZoomValue(uint32 zoom)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.video_zoom = zoom;
    //SCI_TRACE_LOW:"[MMIDC]: SetVideoZoomValue return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2386_112_2_18_2_9_49_664,(uint8*)"dd", ret, zoom);
   
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : set preview white balance
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoWhiteBalance(WHITE_BALANCE_E balance)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
  
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    MMIDC_SETTING_DATA_PTR->dc_setting_info.video_white_balance = balance;
    //SCI_TRACE_LOW:"[MMIDC]: SetvideoWhiteBalance return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2406_112_2_18_2_9_49_665,(uint8*)"d", ret, balance);
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get photo brightness
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BRIGHTNESS_E MMIDC_GetPhotoBrightness(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_brightness;
}

/*****************************************************************************/
// 	Description : set photo brightness
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoBrightness(BRIGHTNESS_E photo_brightness)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;

    if (photo_brightness >= BRIGHTNESS_LEVEL_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetPhotoBrightness photo_brightness"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2429_112_2_18_2_9_49_666,(uint8*)"");
        return ;
    }

    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetPhotoBrightness setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2445_112_2_18_2_9_49_667,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, photo_brightness, ParamCtrlCallBack);	
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetPhotoBrightness return %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2454_112_2_18_2_9_49_668,(uint8*)"dd", ret, photo_brightness);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_brightness = photo_brightness;   
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get video brightness enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BRIGHTNESS_E MMIDC_GetVideoBrightness(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.video_brightness;
}

/*****************************************************************************/
// 	Description : set video brightness enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoBrightness(BRIGHTNESS_E video_brightness)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

    if (video_brightness >= BRIGHTNESS_LEVEL_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetVideoBrightness video_brightness"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2488_112_2_18_2_9_49_669,(uint8*)"");
        return ;
    }
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDVOpen())
    {
        if (MMIDC_IsSettingParamCtrl())
        {
            MMIDC_SETTING_DATA_PTR->setting_number --;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetVideoBrightness setting count > 0 !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2503_112_2_18_2_9_49_670,(uint8*)"");
            return;
        }
        
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_BRIGHTNESS, video_brightness, ParamCtrlCallBack);
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_SetvideoBrightness return: %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2512_112_2_18_2_9_49_671,(uint8*)"dd", ret, video_brightness);
    
    if (DRECORDER_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
        MMIDC_SETTING_DATA_PTR->dc_setting_info.video_brightness = video_brightness;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;
}

/*****************************************************************************/
// 	Description : get need display video preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC VIDEO_DISPLAY_SETTING_TIP_E MMIDC_GetNeedVideoDisplaySettingTip(void)
{
	if(VIDEO_DISPLAY_SETTING_TIP_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_display_setting_tip)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_display_setting_tip = VIDEO_DISPLAY_SETTING_TIP_ON;
	}
	
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_display_setting_tip;
}

/*****************************************************************************/
// 	Description : set need display video preview setting tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNeedVideoDisplaySettingTip(VIDEO_DISPLAY_SETTING_TIP_E tip)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.video_display_setting_tip = tip;
}

/*****************************************************************************/
// 	Description : get record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC AUDIO_SETTING_E MMIDC_GetVideoAudioSetting(void)
{
	if(AUDIO_SETTING_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.audio_setting)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.audio_setting = AUDIO_SETTING_ON;
	}
	
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.audio_setting;
}

/*****************************************************************************/
// 	Description : set record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetVideoAudioSetting(AUDIO_SETTING_E setting)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.audio_setting = setting;
}

/*****************************************************************************/
// 	Description : set record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//           在ZOOM_MAX >1 ，< 最大变焦值，非显示像框 ,
//           非连拍模式下，才允许变焦
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IncreaseZoomValue(void)
{
#ifdef MMIDC_F_DC_ZOOM
    uint32 zoom_max = MMIDC_GetMaxZoomLevel();


#if defined(MMIDC_F_U_CAMERA)
    if (MMIDC_GetUCameraModuleFlag()
        && (CAMERA_MODE_DC == MMIDC_GetCameraMode())
        )
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_IncreaseZoomValue U CAMERA NO SUPPORT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2579_112_2_18_2_9_49_672,(uint8*)"");
        return FALSE;
    }
#endif


    if( (zoom_max > 1) 
        && (MMIDC_GetPhotoZoomValue() < zoom_max - 1 )
        && (MMIDC_GetFrameIndex() == FRAME_DISABLE)
        //&& (MULTI_SHOOT_DISABLE == MMIDC_GetMultiShootEnum() ) 
       )
    {
        MMIDC_UpdatePhotoZoomValue(MMIDC_GetPhotoZoomValue() + 1);
        return TRUE;
    }
#endif
    return FALSE;
}

/*****************************************************************************/
// 	Description : set record video whether audio is will be recorded
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DecreaseZoomValue(void)
{
#ifdef MMIDC_F_DC_ZOOM
    uint32 zoom_max = MMIDC_GetMaxZoomLevel();


#if defined(MMIDC_F_U_CAMERA)
    if (MMIDC_GetUCameraModuleFlag()
        && (CAMERA_MODE_DC == MMIDC_GetCameraMode())
        )
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_IncreaseZoomValue U CAMERA NO SUPPORT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2610_112_2_18_2_9_49_673,(uint8*)"");
        return FALSE;
    }
#endif

    
    if (MMIDC_GetPhotoZoomValue() > 0 
        && zoom_max > 1 
        && MMIDC_GetFrameIndex() == FRAME_DISABLE
       )
    {
        MMIDC_UpdatePhotoZoomValue(MMIDC_GetPhotoZoomValue() - 1);
        return TRUE;
    }
#endif
    return FALSE;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetLeftSoftkey(void)
{
    return MMIDC_SETTING_DATA_PTR->left_rect;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetLeftSoftkey(GUI_RECT_T rect)
{
    MMIDC_SETTING_DATA_PTR->left_rect = rect;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetRightSoftkey(void)
{
    return MMIDC_SETTING_DATA_PTR->right_rect;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetRightSoftkey(GUI_RECT_T rect)
{
    MMIDC_SETTING_DATA_PTR->right_rect = rect;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetMiddleSoftkey(void)
{
    return MMIDC_SETTING_DATA_PTR->middle_rect;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMiddleSoftkey(GUI_RECT_T rect)
{
    MMIDC_SETTING_DATA_PTR->middle_rect = rect;
}

/*****************************************************************************/
// 	Description : get multi photo file name rect 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//   Param [Return]: GUI_RECT_T  struct  value
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetMultiPhotoRect(void)
{
    return MMIDC_SETTING_DATA_PTR->multi_photo_rect;
}

/*****************************************************************************/
// 	Description : set multi photo file name rect 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//   Param [Input]: rect   GUI_RECT_T
/*****************************************************************************/
PUBLIC void MMIDC_SetMultiPhotoRect(GUI_RECT_T rect)
{
    MMIDC_SETTING_DATA_PTR->multi_photo_rect = rect;
}

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsSettingParamCtrl(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_IsSettingParamCtrl %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_2707_112_2_18_2_9_49_674,(uint8*)"d", MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl);
    if(MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl > 0)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitSettingParamCtrl(void)
{
    MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl = 0;
}

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_Assert(BOOLEAN res, void* file, uint32 line)
{
    if (!res)
    {
        SCI_ASSERT(0); /*assert verified*/
    }
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetLCDSizeMode(CAMERA_ENG_LCD_SIZE_MODE_E mode)
{
    mode = CAMERA_ENG_LCD_240X320;//for test
    s_lcd_size_mode = mode;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_TurnOnEngTestMode(void)
{
    s_is_eng_turned_on = TRUE;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsEngTestModeTurnedOn(void)
{
    return s_is_eng_turned_on;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_LCD_SIZE_MODE_E MMIAPIDC_GetLCDSizeMode(void)
{
    return s_lcd_size_mode;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetPreviewMode(CAMERA_ENG_PREVIEW_MODE_E mode)
{
    MMIDC_SETTING_NV_INFO_T data = {0}; /*lint !e64*/
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_DC_SET_INFO, &data, return_value);    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIDC_Setting_SetFactoryValue();

        //added, @robert.wang, 09-11-4, cr159393   
        MMINV_READ(MMINV_DC_SET_INFO, &data, return_value); 
        //end cr159393


        data.photo_file_name_id = 0;
        data.video_file_name_id = 0;
    }
    data.default_screen_mode = (SCREEN_MODE_E)mode;
    MMINV_WRITE(MMINV_DC_SET_INFO, &data);
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_PREVIEW_MODE_E MMIAPIDC_GetPreviewMode(void)
{
    MMIDC_SETTING_NV_INFO_T data = {0};/*lint !e64*/
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_DC_SET_INFO, &data, return_value);    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIDC_Setting_SetFactoryValue();

        //added, @robert.wang, 09-11-4, cr159393   
        MMINV_READ(MMINV_DC_SET_INFO, &data, return_value); 
        if (MN_RETURN_SUCCESS != return_value)
        {
            data.default_screen_mode = (SCREEN_MODE_E)CAMERA_ENG_PREVIEW_VER_MODE;
        }
        //end cr159393

        data.photo_file_name_id = 0;
        data.video_file_name_id = 0;
    }
    return (CAMERA_ENG_PREVIEW_MODE_E)data.default_screen_mode;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetVisualMode(CAMERA_ENG_VISUAL_MODE_E mode)
{
    MMIDC_SETTING_NV_INFO_T data = {0};/*lint !e64*/
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_DC_SET_INFO, &data, return_value);    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIDC_Setting_SetFactoryValue();

        //added, @robert.wang, 09-11-4, cr159393   
        MMINV_READ(MMINV_DC_SET_INFO, &data, return_value); 
        //end cr159393

        data.photo_file_name_id = 0;
        data.video_file_name_id = 0;
    }
    data.default_visual_mode = mode;
    MMINV_WRITE(MMINV_DC_SET_INFO, &data);
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_VISUAL_MODE_E MMIAPIDC_GetVisualMode(void)
{
    MMIDC_SETTING_NV_INFO_T data = {0};/*lint !e64*/
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_DC_SET_INFO, &data, return_value);    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIDC_Setting_SetFactoryValue();

        //added, @robert.wang, 09-11-4, cr159393   
        MMINV_READ(MMINV_DC_SET_INFO, &data, return_value); 
        if (MN_RETURN_SUCCESS != return_value)
        {
            data.default_visual_mode = CAMERA_ENG_FULL_SCREEN;
        }
        //end cr159393

        data.photo_file_name_id = 0;
        data.video_file_name_id = 0;
    }
    return data.default_visual_mode;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC SCREEN_MODE_E MMIDC_GetDefaultScreenMode(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.default_screen_mode;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_VISUAL_MODE_E MMIDC_GetDefaultVisualMode(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.default_visual_mode;
}

/*****************************************************************************/
// 	Description : get photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GePhotoSizetHorLcd(void)
{
    MMIDC_PHOTO_SIZE_E size = PHOTO_SIZE_MAX;
    int32 lcd_size = MMIDC_GetLCDSize();

    switch(lcd_size)
    {
    case MMIDC_HVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_480X320; 
        break;
        
    case MMIDC_WQVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_400X240; 
        break;
        
    case MMIDC_QVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_320X240;
        break;
        
    case MMIDC_QCIF_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_220X176;
        break;
    
	case MMIDC_QQVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_160X128;
        break;    

    default:
        size = PHOTO_SIZE_640X480;
        break;
    }
    
    return size;
}

/*****************************************************************************/
// 	Description : get photo size enum
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GePhotoSizetVerLcd(void)
{
    MMIDC_PHOTO_SIZE_E size = PHOTO_SIZE_MAX;
    int32 lcd_size = MMIDC_GetLCDSize();

    switch(lcd_size)
    {
    case MMIDC_HVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_320X480;
        break;
        
    case MMIDC_WQVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_240X400; 
        break;
        
    case MMIDC_QVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_240X320;
        break;
        
   case MMIDC_QCIF_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_176X220;
        break;

   case MMIDC_QQVGA_PREVIEW_HEIGHT:
        size = PHOTO_SIZE_128X160;
        break;
		
   default:
        size = PHOTO_SIZE_640X480;
        break;
    }
    return size;
}

/*****************************************************************************/
// 	Description : set used lcd for camera
//	Global resource dependence : none
//  Author: jesse.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetLcdID(uint32 lcd_id)
{
#ifdef SUBLCD_SIZE 
	if(g_mmidc_lcd_id != lcd_id)
	{
		g_mmidc_lcd_id = lcd_id;

		GetCameraLcdInfo();
		
		MMIDC_ResetLogicalScreen();
	    
		if(SUB_LCD_ID == g_mmidc_lcd_id)
		{
			if(MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_zoom > 0)
			{
				MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_zoom = 0;
			}
			if(MMIDC_SETTING_DATA_PTR->dc_setting_info.frame > FRAME_DISABLE)
			{
				MMIDC_SETTING_DATA_PTR->dc_setting_info.frame = FRAME_DISABLE;
				MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size = PHOTO_SIZE_640X480;
			}
			if(MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size >= PHOTO_SIZE_640X480)
			{
				MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size = PHOTO_SIZE_640X480;
			}
			else //if(MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size < PHOTO_SIZE_320X240)
			{
				MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size = PHOTO_SIZE_320X240;
			}
			
			MMIDC_SETTING_DATA_PTR->dc_setting_info.multi_shoot = MULTI_SHOOT_DISABLE;
		}
	}
#endif
}

/*****************************************************************************/
// 	Description : get used lcd for camera
//	Global resource dependence : none
//  Author: jesse.yu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetLcdID(void)
{
#ifdef SUBLCD_SIZE
	return g_mmidc_lcd_id;
#else
	return (MAIN_LCD_ID);
#endif
}

/*****************************************************************************/
// 	Description : get used lcd for camera
//	Global resource dependence : none
//  Author: jesse.yu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDC_GetLcdID(void)
{
    return MMIDC_GetLcdID();
}

/*****************************************************************************/
// 	Description : get lcd info for camera sensor
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void GetCameraLcdInfo(void)
{
    LCD_ID_E   lcd_id = GUI_MAIN_LCD_ID;
    LCD_INFO_T lcd_info = {0};
    DCAMERA_ROTATION_MODE_E angle = 0;

    lcd_id = MMIDC_GetLcdID();
    GUILCD_GetInfo((LCD_ID_E)lcd_id, &lcd_info);

    angle = MMIAPIDC_GetCameraSensorAngle();
    switch(angle)
    {
    case DCAMERA_ROTATION_90:
    case DCAMERA_ROTATION_270:
        if (lcd_info.lcd_height >= lcd_info.lcd_width)
        {
            //sensor 90, lcd 0
            s_mmidc_info.physical_lcd_width = lcd_info.lcd_height;
            s_mmidc_info.physical_lcd_height = lcd_info.lcd_width;
        }
        else
        {
            //sensor 90, lcd 90
            s_mmidc_info.physical_lcd_width = lcd_info.lcd_width;
            s_mmidc_info.physical_lcd_height = lcd_info.lcd_height;
        }
        break;      
    default:
        if (lcd_info.lcd_height >= lcd_info.lcd_width)
        {
            //sensor 0, lcd 0
            s_mmidc_info.physical_lcd_width = lcd_info.lcd_width;
            s_mmidc_info.physical_lcd_height = lcd_info.lcd_height;
        }
        else
        {
            //sensor 0, lcd 90
            s_mmidc_info.physical_lcd_width = lcd_info.lcd_height;
            s_mmidc_info.physical_lcd_height = lcd_info.lcd_width;
        }
        break;
    }
    
}
/*****************************************************************************/
// 	Description : init mmi dc information
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitInfo(void)
{
    LCD_ID_E   lcd_id = GUI_MAIN_LCD_ID;
    LCD_INFO_T lcd_info = {0};

    SCI_MEMSET(&s_mmidc_info, 0x00, sizeof(s_mmidc_info));

    //get platform angle 
    s_mmidc_info.platform_angle = GUILCD_GetInitAngle(GUI_MAIN_LCD_ID);

    GetCameraLcdInfo();

    GUILCD_GetInfo((LCD_ID_E)lcd_id, &lcd_info);
    
    if (lcd_info.lcd_height >= lcd_info.lcd_width)
    {
        s_mmidc_info.lcd_size = lcd_info.lcd_height;
    }
    else
    {
        s_mmidc_info.is_real_hor_lcd = TRUE;
        s_mmidc_info.lcd_size = lcd_info.lcd_width;
    }

    MMIDC_ResetLogicalScreen();

    s_mmidc_info.switch_frame_index = FRAME_DISABLE;
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_InitInfo platform_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3109_112_2_18_2_9_50_675,(uint8*)"d", s_mmidc_info.platform_angle);
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_InitInfo lcd_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3110_112_2_18_2_9_50_676,(uint8*)"d", s_mmidc_info.lcd_size);
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_InitInfo physical width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3112_112_2_18_2_9_50_677,(uint8*)"dd",s_mmidc_info.physical_lcd_width, s_mmidc_info.physical_lcd_height);
}

/*****************************************************************************/
//Description : physical lcd is real hor or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsRealHorLcd(void)
{
    return s_mmidc_info.is_real_hor_lcd;
}

/*****************************************************************************/
// 	Description : get platform angle
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMIDC_GetPlatformAngle(void)
{
    return s_mmidc_info.platform_angle;
}

/*****************************************************************************/
// 	Description : get max value of platform lcd screen size
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetLCDSize(void)
{
    return s_mmidc_info.lcd_size;
}

/*****************************************************************************/
// 	Description : get current screen layout style
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
//  Param:
//       Input:NONE
//       Output:MMIDC_LAYOUT_E 
/*****************************************************************************/
PUBLIC MMIDC_LAYOUT_E  MMIDC_GetLayoutStyle(void)
{
    MMIDC_LAYOUT_E   layout_style = MMIDC_LAYOUT_TRADITION;

    if (LCD_ANGLE_90 == MMIDC_GetPlatformAngle())
    {
        if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode())
        {
            layout_style = MMIDC_LAYOUT_TRADITION;
        }
        else
        {
            layout_style = MMIDC_LAYOUT_RIGHTWARD;
        }
    }
    else
    {
        if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode())
        {
            layout_style = MMIDC_LAYOUT_RIGHTWARD;
        }
        else
        {
            layout_style = MMIDC_LAYOUT_TRADITION;
        }
    }

    return layout_style;
}

/*****************************************************************************/
// 	Description : set current lcd id screen width and height 
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetLogicalScreen(void)
{
    LCD_ID_E lcd_id = GUI_MAIN_LCD_ID;
    uint16   logic_width = 0;
    uint16   logic_height = 0;

    lcd_id = MMIDC_GetLcdID();
    
    GUILCD_GetLogicWidthHeight((LCD_ID_E)lcd_id, &logic_width, &logic_height);

    //SCI_TRACE_LOW:"[MMIDC] lcd_id = %d, width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3194_112_2_18_2_9_50_678,(uint8*)"ddd",lcd_id, logic_width, logic_height);
    MMIDC_SetPreviewWidth(logic_width);
    MMIDC_SetPreviewHeight(logic_height);
}

/*****************************************************************************/
// 	Description : get max width of softkey(left, middle, right)
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetMaxWidthOfSoftkey(void)
{
    GUI_RECT_T  rect1 = {0};
    GUI_RECT_T  rect2 = {0};
    int32       width1 = 0;
    int32       width2 = 0;

    rect1 = MMIDC_GetLeftSoftkey();
    width1 = rect1.right - rect1.left + 1;

    rect2 = MMIDC_GetMiddleSoftkey();
    width2 = rect2.right - rect1.left + 1;

    width2 = MAX(width1, width2);

    rect1 = MMIDC_GetRightSoftkey();
    width1 = rect1.right - rect1.left + 1;

    width2 = MAX(width1, width2);

    return width2;
}

/*****************************************************************************/
// 	Description : get state for screen display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_SHOW_STATE_E MMIDC_GetShowState(void)
{
    return s_mmidc_info.show_state;
}

/*****************************************************************************/
// 	Description : set state for screen display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetShowState(MMIDC_SHOW_STATE_E show_state)
{
    s_mmidc_info.show_state = show_state;
}

/*****************************************************************************/
// 	Description : set frame rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFrameRect(GUI_RECT_T  frame_rect)
{
    s_mmidc_info.frame_rect = frame_rect;
}

/*****************************************************************************/
// 	Description : get frame rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDC_GetFrameRect(void)
{
    return (s_mmidc_info.frame_rect);
}

/*****************************************************************************/
// 	Description : get max zoom  rect
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMaxZoomLevel(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32 zoom_max = 0;    
    MMIDC_PHOTO_SIZE_E photo_size = MMIDC_GetPhotoSize();
    uint8 size_type = 0;
    uint8 display_mode = MMIDCSRV_GetDCDisplayMode();//MMIDC_GetPhotoDisplayMode();
    uint8 rotation_mode = MMIDC_GetPhotoRotationMode();
    uint8 lcd_id = GUI_MAIN_LCD_ID;


    if (photo_size < PHOTO_SIZE_MAX)
    {
        lcd_id = MMIDC_GetLcdID();

        size_type = MMIDC_ConvertPhotoSizeToDCameraSize(photo_size);
        zoom_max = size_type;
        zoom_max |= (display_mode<<0x08);
        zoom_max |= (rotation_mode<<0x10);
        zoom_max |= (lcd_id<<0x18);

        //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMaxZoomLevel zoom_max = 0x%04lX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3287_112_2_18_2_9_51_679,(uint8*)"d", zoom_max);
        ret = DCAMERA_GetInfo((DCAMERA_INFO_CMD_E)DCAMERA_INFO_ZOOM, &zoom_max);
        if (DCAMERA_OP_SUCCESS != ret)
        {
            zoom_max = 0;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMaxZoomLevel FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3292_112_2_18_2_9_51_680,(uint8*)"");
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMaxZoomLevel out zoom_max = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3296_112_2_18_2_9_51_681,(uint8*)"d", zoom_max);
    return zoom_max;
}

/*****************************************************************************/
//  Description : get preview angle from dcam
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIDC_GetPreviewAngleFromDCam(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32 preview_angle = 0;

    if (MMIDC_IsDCOpen() || MMIDC_IsDVOpen())
    {
        ret = DCAMERA_GetInfo((DCAMERA_INFO_CMD_E)DCAMERA_INFO_DC_PREVIEW_ROTATION, &preview_angle);
        if (DCAMERA_OP_SUCCESS != ret)
        {
            preview_angle = 0;
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] MMIDC_GetPreviewAngleFromDCam preview_angle=%d",\
                  preview_angle);
                  
    return (DCAMERA_ROTATION_MODE_E)preview_angle;
}

/*****************************************************************************/
//  Description : get Cap angle from dcam
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIDC_GetCapAngleFromDCam(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32 cap_angle = 0;

    if (MMIDC_IsDCOpen() || MMIDC_IsDVOpen())
    {
        ret = DCAMERA_GetInfo((DCAMERA_INFO_CMD_E)DCAMERA_INFO_DC_CAP_ROTATION, &cap_angle);
        if (DCAMERA_OP_SUCCESS != ret)
        {
            cap_angle = 0;
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] MMIDC_GetCapAngleFromDCam cap_angle=%d",\
                  cap_angle);
                  
    return (DCAMERA_ROTATION_MODE_E)cap_angle;
}

/*****************************************************************************/
// 	Description : get physical lcd width and height
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetPhysicalLCDWidthAndHeight(uint16 *lcd_width,//out
                                                            uint16 *lcd_height, //out
                                                            DCAMERA_ROTATION_MODE_E *angle   //out
                                                            )
{
    LCD_ID_E    lcd_id = GUI_MAIN_LCD_ID;
    GUI_RECT_T  logic_rect= {0};
    LCD_ANGLE_E logic_angle = 0;
    uint16      logic_width = 0;
    uint16      logic_height = 0;

    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0};
    GUILCD_COORDINATE_PARAM_T physical_coordinate = {0};
    DCAMERA_ROTATION_MODE_E  dcam_angle = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    BOOLEAN is_real_hor_lcd = MMIDC_IsRealHorLcd();
    LCD_ANGLE_E  platform_angle = MMIDC_GetPlatformAngle();

    lcd_id = MMIDC_GetLcdID();

    logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)lcd_id);
    GUILCD_GetLogicWidthHeight(lcd_id, &logic_width, &logic_height);

    logic_rect.right = logic_width - 1;
    logic_rect.bottom = logic_height - 1;


    logic_coordinate.rect = logic_rect;
    logic_coordinate.angle = logic_angle;
    logic_coordinate.lcd_width = logic_width;
    logic_coordinate.lcd_height = logic_height;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetPhysicalLCDWidthAndHeight logic_angle =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3334_112_2_18_2_9_51_682,(uint8*)"d",logic_coordinate.angle);
    //SCI_TRACE_LOW:"[MMIDC] width = %d,height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3336_112_2_18_2_9_51_683,(uint8*)"dd",logic_coordinate.lcd_width, logic_coordinate.lcd_height);
    //SCI_TRACE_LOW:"[MMIDC] rect_right = %d,rect_bottom = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3338_112_2_18_2_9_51_684,(uint8*)"dd",logic_coordinate.rect.right, logic_coordinate.rect.bottom);

    GUILCD_ConvertLogicToPhysicalCoordinate(lcd_id,\
                        &logic_coordinate, 
                        &physical_coordinate
                        );

    //SCI_TRACE_LOW:"[MMIDC] out rect_right = %d,rect_bottom = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3346_112_2_18_2_9_51_685,(uint8*)"dd",physical_coordinate.rect.right, physical_coordinate.rect.bottom);
    //SCI_TRACE_LOW:"[MMIDC] out angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3347_112_2_18_2_9_51_686,(uint8*)"d", physical_coordinate.angle);


    dcam_angle = MMIAPIDC_GetCameraSensorAngle();

    switch(dcam_angle)
    {
    case DCAMERA_ROTATION_0:
        if ((LCD_ANGLE_90 == platform_angle)
             && (!is_real_hor_lcd)
             && (DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
            )
        {
#if defined MAINLCD_LOGIC_ANGLE_90 && (defined MAINLCD_DEV_SIZE_128X160 || defined MAINLCD_DEV_SIZE_176X220 || defined MAINLCD_DEV_SIZE_240X320)
			//假横屏
#if defined LCD_ROT_MODE_LCD180
			dcam_angle = DCAMERA_ROTATION_0;
#else
			dcam_angle = DCAMERA_ROTATION_180;
#endif
#else
			dcam_angle = DCAMERA_ROTATION_90;
#endif
        }
        break;
    case DCAMERA_ROTATION_90:
        if ((LCD_ANGLE_90 == platform_angle)
             && (!is_real_hor_lcd)
             && (DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
            )
        {
            dcam_angle = DCAMERA_ROTATION_0;
        }
        break;
    default:
        break;
    }

    if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
    {
        dcam_angle = MMIDC_GetPreviewAngleFromDCam();
    }
    
    *lcd_width = physical_coordinate.rect.right - physical_coordinate.rect.left + 1;
    *lcd_height = physical_coordinate.rect.bottom - physical_coordinate.rect.top + 1;
    *angle = dcam_angle;
    
    //SCI_TRACE_LOW:"[MMIDC] *angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3356_112_2_18_2_9_51_687,(uint8*)"d", dcam_angle);

}

/*****************************************************************************/
// 	Description : get current camera sensor angle
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIAPIDC_GetCameraSensorAngle(void)
{
    SETTING_SENSOR_ID_E id = 0;
    DCAMERA_ROTATION_MODE_E angle = 0;

    id = MMIDC_GetSettingSensorID();

    angle = MMIAPIDC_GetDCModuleSensorAngle(id);

    return angle;
}

/*****************************************************************************/
// 	Description : set current camera sensor angle
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetSensorAngle(DCAMERA_ROTATION_MODE_E angle)
{
    SETTING_SENSOR_ID_E id = 0;

    id = MMIDC_GetSettingSensorID();

    MMIAPIDC_SetDCModuleSensorAngle(angle, id);
}

/*****************************************************************************/
//Description : get camera sensor angle by id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIAPIDC_GetDCModuleSensorAngle(SETTING_SENSOR_ID_E id
                                                                            )
{
    MMIDC_SETTING_NV_INFO_T data = {0};/*lint !e64*/
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    DCAMERA_ROTATION_MODE_E angle = 0;

    if (id < SETTING_SENSOR_ID_MAX)
    {
        MMINV_READ(MMINV_DC_SET_INFO, &data, return_value);    
        if (MN_RETURN_SUCCESS != return_value)
        {
            MMIAPIDC_Setting_SetFactoryValue();

            MMINV_READ(MMINV_DC_SET_INFO, &data, return_value); 
            if (MN_RETURN_SUCCESS != return_value)
            {
                data.sensor_angle[id] = 0;
            }

        }
        
        angle = data.sensor_angle[id];
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_GetDCModuleSensorAngle angle=%d,id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3419_112_2_18_2_9_51_688,(uint8*)"dd",angle, id);
    return angle;
}

/*****************************************************************************/
//Description : set camera sensor angle by id
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetDCModuleSensorAngle(DCAMERA_ROTATION_MODE_E angle,
                                                         SETTING_SENSOR_ID_E id
                                                        )
{
    MMIDC_SETTING_NV_INFO_T data = {0};/*lint !e64*/
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_SetDCModuleSensorAngle angle=%d,id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3435_112_2_18_2_9_51_689,(uint8*)"dd",angle, id);

    if (id >= SETTING_SENSOR_ID_MAX)
    {
        return;
    }
    
    MMINV_READ(MMINV_DC_SET_INFO, &data, return_value);    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPIDC_Setting_SetFactoryValue();

        MMINV_READ(MMINV_DC_SET_INFO, &data, return_value); 
    }
    data.sensor_angle[id] = angle;
    MMINV_WRITE(MMINV_DC_SET_INFO, &data);
}
/*****************************************************************************/
// 	Description : environment is or not silent mode
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsSilentMode(void)
{
    uint8	 envset_mode = 0;
    BOOLEAN  is_silent = FALSE;
    
    //envset(ring,vibration,silence)
    envset_mode = MMIAPIENVSET_GetActiveModeId();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsSilentMode mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3463_112_2_18_2_9_51_690,(uint8*)"d", envset_mode);
    switch(envset_mode)
    {
    case MMIENVSET_SILENT_MODE:     // 安静环境（静音）
    case MMIENVSET_MEETING_MODE:    // 会议环境（振动）
        is_silent = TRUE;
        break;
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    case MMIENVSET_EARPHONE_MODE:
    case MMIENVSET_BLUETOOTH_MODE:
        if(MMIAPIENVSET_GetMuteState())
        {
            is_silent = TRUE;
        }
        break;
#endif        
    default:
        break;
    }

    return is_silent;
}

/*****************************************************************************/
// 	Description : convert dcamera size to mmi photo size
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int16 GetDCameraSizeToPhotoSizeArray(MMIDC_SIZE_T* size_ptr, int16 size_len)
{
    MMIDC_SIZE_T size_array[]=
    {
        {DCAMERA_PHOTO_SIZE_128X160, PHOTO_SIZE_128X160},
 		{DCAMERA_PHOTO_SIZE_160X128, PHOTO_SIZE_160X128},
		{DCAMERA_PHOTO_SIZE_176X220, PHOTO_SIZE_176X220},
        {DCAMERA_PHOTO_SIZE_220X176,  PHOTO_SIZE_220X176},
        {DCAMERA_PHOTO_SIZE_240X320,  PHOTO_SIZE_240X320},
        {DCAMERA_PHOTO_SIZE_320X240,  PHOTO_SIZE_320X240},
        {DCAMERA_PHOTO_SIZE_240X400,  PHOTO_SIZE_240X400},
        {DCAMERA_PHOTO_SIZE_400X240,  PHOTO_SIZE_400X240},
        {DCAMERA_PHOTO_SIZE_320X480,  PHOTO_SIZE_320X480},
        {DCAMERA_PHOTO_SIZE_480X320,  PHOTO_SIZE_480X320},
        {DCAMERA_PHOTO_SIZE_480X640,  PHOTO_SIZE_480X640},
        {DCAMERA_PHOTO_SIZE_640X480,  PHOTO_SIZE_640X480},
        {DCAMERA_PHOTO_SIZE_1280X960, PHOTO_SIZE_1280X960},
        {DCAMERA_PHOTO_SIZE_1600X1200,PHOTO_SIZE_1600X1200},
        {DCAMERA_PHOTO_SIZE_2048X1536,PHOTO_SIZE_2048X1536},
        {DCAMERA_PHOTO_SIZE_2592X1944,PHOTO_SIZE_2592X1944},
        {DCAMERA_PHOTO_SIZE_3264X2448,PHOTO_SIZE_3264X2448}        
    };

    MMIDC_SIZE_T size_array_3p4[]=
    {
        {DCAMERA_PHOTO_SIZE_128X160, PHOTO_SIZE_128X160},
		{DCAMERA_PHOTO_SIZE_176X220, PHOTO_SIZE_176X220},
        {DCAMERA_PHOTO_SIZE_240X320,  PHOTO_SIZE_240X320},
        {DCAMERA_PHOTO_SIZE_240X400,  PHOTO_SIZE_240X400},
        {DCAMERA_PHOTO_SIZE_320X480,  PHOTO_SIZE_320X480},
        {DCAMERA_PHOTO_SIZE_480X640,  PHOTO_SIZE_480X640},
        {DCAMERA_PHOTO_SIZE_960X1280,  PHOTO_SIZE_960X1280}
    };
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;
    LCD_ANGLE_E  current_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);
    int16 total_num = 0;

    if(PNULL == size_ptr)
    {
        return 0;
    }
    sensor_type = MMIAPIDC_GetDCModulSensorType();

    if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type
        && LCD_ANGLE_0 == current_angle)
    {
        total_num = ARR_SIZE(size_array_3p4);
        MMI_MEMCPY(size_ptr, (uint32)size_len, size_array_3p4, sizeof(size_array_3p4), sizeof(size_array_3p4));/*lint !e571 !e737*/
    }
    else
    {
        total_num = ARR_SIZE(size_array);
        MMI_MEMCPY(size_ptr, (uint32)size_len, size_array, sizeof(size_array), sizeof(size_array));/*lint !e571*/
    }

    SCI_TRACE_LOW("[MMIDC] GetDCameraSizeToPhotoSizeArray sensor_type = %d,current_angle = %d, total_num = %d",
                    sensor_type, current_angle, total_num);

    return total_num;
}
/*****************************************************************************/
// 	Description : convert dcamera size to mmi photo size
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_ConvertDCameraSizeToPhotoSize(DCAMERA_PARAM_PHOTO_SIZE_E dcamera_size)
{
    MMIDC_SIZE_T size_array[PHOTO_SIZE_MAX] = {0};
    MMIDC_PHOTO_SIZE_E photo_size = PHOTO_SIZE_MAX;
    int16  total_item = 0;
    int16  i = 0;

    total_item = GetDCameraSizeToPhotoSizeArray(size_array, sizeof(size_array));
    for (i = 0; i < total_item; i++)
    {
        if (dcamera_size == size_array[i].src_size)
        {
            photo_size = (MMIDC_PHOTO_SIZE_E)size_array[i].dst_size;
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ConvertDCameraSizeToPhotoSize photo_size = %d,dcamera_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3529_112_2_18_2_9_51_692,(uint8*)"dd",photo_size, dcamera_size);

    return photo_size;
}

/*****************************************************************************/
// 	Description : convert dcamera size to mmi photo size
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_PARAM_PHOTO_SIZE_E MMIDC_ConvertPhotoSizeToDCameraSize(MMIDC_PHOTO_SIZE_E photo_size)
{
    MMIDC_SIZE_T size_array[]=
    {
        {PHOTO_SIZE_128X160,  DCAMERA_PHOTO_SIZE_128X160},
        {PHOTO_SIZE_160X128,  DCAMERA_PHOTO_SIZE_160X128},
        {PHOTO_SIZE_176X220,  DCAMERA_PHOTO_SIZE_176X220},
        {PHOTO_SIZE_220X176,  DCAMERA_PHOTO_SIZE_220X176}, 
        {PHOTO_SIZE_240X320,  DCAMERA_PHOTO_SIZE_240X320},
        {PHOTO_SIZE_240X400,  DCAMERA_PHOTO_SIZE_240X400},  
        {PHOTO_SIZE_400X240,  DCAMERA_PHOTO_SIZE_400X240},  
        {PHOTO_SIZE_320X480,  DCAMERA_PHOTO_SIZE_320X480},  
        {PHOTO_SIZE_480X320,  DCAMERA_PHOTO_SIZE_480X320}, 
        {PHOTO_SIZE_480X640,   DCAMERA_PHOTO_SIZE_480X640},  
        {PHOTO_SIZE_320X240,   DCAMERA_PHOTO_SIZE_320X240},  
        {PHOTO_SIZE_640X480,   DCAMERA_PHOTO_SIZE_640X480},  
        {PHOTO_SIZE_960X1280,  DCAMERA_PHOTO_SIZE_960X1280},
        {PHOTO_SIZE_1280X960,  DCAMERA_PHOTO_SIZE_1280X960}, 
        {PHOTO_SIZE_1600X1200, DCAMERA_PHOTO_SIZE_1600X1200},
        {PHOTO_SIZE_2048X1536, DCAMERA_PHOTO_SIZE_2048X1536},
        {PHOTO_SIZE_2592X1944, DCAMERA_PHOTO_SIZE_2592X1944},
        {PHOTO_SIZE_3264X2448, DCAMERA_PHOTO_SIZE_3264X2448}
        
    };
    DCAMERA_PARAM_PHOTO_SIZE_E dcamera_size = DCAMERA_PHOTO_SIZE_MAX;
    int16  total_item = ARR_SIZE(size_array);
    int16  i = 0;

    if (PHOTO_SIZE_MAX != total_item) /*lint !e774*/
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_ConvertPhotoSizeToDCameraSize total_item !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3565_112_2_18_2_9_51_693,(uint8*)"");
        return dcamera_size;
    }

    for (i = 0; i < total_item; i++)
    {
        if (photo_size == size_array[i].src_size)
        {
            dcamera_size = (DCAMERA_PARAM_PHOTO_SIZE_E)size_array[i].dst_size;
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ConvertPhotoSizeToDCameraSize photo_size = %d,dcamera_size= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3579_112_2_18_2_9_51_694,(uint8*)"dd",photo_size, dcamera_size);
                       
    return dcamera_size;
}

/*****************************************************************************/
// 	Description : get support size array
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
//  Param:
//      return: max photo size  
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetSupportSizeArray(MMIDC_PHOTO_SIZE_E *size_array  //out,maybe PNULL
                                                            )
{
    uint32  size_bit = 0;
    int32   i = 0;
    uint8   bit_val = 0;
    
    MMIDC_PHOTO_SIZE_E max_photo_size = 0;
    MMIDC_PHOTO_SIZE_E photo_size = 0;

    if (MMIDC_IsDCOpen())
    {
        DCAMERA_GetInfo(DCAMERA_INFO_SENSOR_SIZE, &size_bit);
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetSupportSizeArray size_bit = 0x%08lX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3603_112_2_18_2_9_51_695,(uint8*)"d", size_bit);

        for (i = 0; i < DCAMERA_PHOTO_SIZE_MAX; i++)
        {
            bit_val = size_bit >>i;
            //SCI_TRACE_LOW("i = %d,bit_val = 0x%02lX", i, bit_val);
            
            //check bit 
            if (bit_val & 0x01)
            {
                photo_size = MMIDC_ConvertDCameraSizeToPhotoSize((DCAMERA_PARAM_PHOTO_SIZE_E)i);
                if (photo_size < PHOTO_SIZE_MAX)
                {
                    if (PNULL != size_array)
                    {
                        size_array[photo_size] = photo_size;
                    }
                    max_photo_size = MAX(max_photo_size, photo_size);
                }
            }
        }
    }
    return max_photo_size;
}

/*****************************************************************************/
//  Description : get max photo size
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetCurMaxPhotoSize(void)
{
    MMIDC_PHOTO_SIZE_E max_photo_size = 0;
    MMIDC_PHOTO_SIZE_E srv_photo_size = 0;
    
    max_photo_size = MMIDC_GetSupportSizeArray(PNULL);
    srv_photo_size = MMIDCSRV_GetMaxPhotoSize();
    max_photo_size = MIN(srv_photo_size, max_photo_size);

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetCurMaxPhotoSize max_photo_size= %d", max_photo_size);

    return max_photo_size;
}

/*****************************************************************************/
// 	Description : set switch frame index
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSwitchFrameIndex(FRAME_E index)
{
    s_mmidc_info.switch_frame_index = index;
}

/*****************************************************************************/
// 	Description : get switch frame index
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC FRAME_E MMIDC_GetSwitchFrameIndex(void)
{
    return (s_mmidc_info.switch_frame_index);
}

/*****************************************************************************/
// 	Description : set setting sensor id
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSettingSensorID(SETTING_SENSOR_ID_E sensor_id)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.sensor_id = sensor_id;
}

/*****************************************************************************/
// 	Description : get setting sensor id
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC SETTING_SENSOR_ID_E MMIDC_GetSettingSensorID(void)
{
    return (MMIDC_SETTING_DATA_PTR->dc_setting_info.sensor_id);
}

/*****************************************************************************/
// 	Description : init switch sensor parameter
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitSwitchSensorParameter(void)
{
    MMIDC_InitNVSettingDefaultValue(&MMIDC_SETTING_DATA_PTR->dc_setting_info, TRUE);

#if defined(MMIDC_F_U_CAMERA)
    if (!MMIDC_GetUCameraModuleFlag())
#endif    
    {
        //init default photo size and video size
        MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size = PHOTO_SIZE_MAX;
    }

    
    InitVideoSizeToDefault(&MMIDC_SETTING_DATA_PTR->dc_setting_info);

#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        MMIDC_SetCheckWorkParamFlag(TRUE);
    }
#endif  
}

/*****************************************************************************/
// 	Description : set dcamera sensor id by command
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetDCameraSensorID(SETTING_SENSOR_ID_E sensor_id)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32      sensor_param = 0;
    int32 sensor_number = 0;

    if (sensor_id >= SETTING_SENSOR_ID_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetDCameraSensorID sensor_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3696_112_2_18_2_9_51_696,(uint8*)"d", sensor_id);
        return;
    }

    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
    
    if(MMIDC_IsDCOpen() || MMIDC_IsDVOpen())
    {
        sensor_number = MMIDC_GetDCameraSensorNumber();

        if (MAX_MMI_SENSOR_NUM <= sensor_number)
        {
            switch(sensor_id)
            {
            case SETTING_SENSOR_FRONT:
                sensor_param = SENSOR_SUB;
                break;
            case SETTING_SENSOR_REAR:
                sensor_param = SENSOR_MAIN;
                break;
            default:
                sensor_param = SENSOR_MAIN;
                break;
            }
            MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
            MMIDC_StartCallbackTimer();
            ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, sensor_param, ParamCtrlCallBack);
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetDCameraSensorID ret = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3729_112_2_18_2_9_51_697,(uint8*)"d", ret);
            if(ret != DCAMERA_OP_SUCCESS)
            {
                MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
                MMIDC_SETTING_DATA_PTR->setting_number --;
                MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
                return;
            }
        }
    }

    MMIDC_SetSettingSensorID(sensor_id);
    MMIDC_SETTING_DATA_PTR->setting_number --;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetDCameraSensorID sensor_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3743_112_2_18_2_9_52_698,(uint8*)"d", sensor_id);
}

/*****************************************************************************/
// 	Description : get dcamera sensor number
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetDCameraSensorNumber(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32 sensor_number = 0;   

    
    if(MMIDC_IsDCOpen() || MMIDC_IsDVOpen())
    {
        ret = DCAMERA_GetInfo((DCAMERA_INFO_CMD_E)DCAMERA_INFO_IMG_SENSOR_NUM, &sensor_number);
        if (DCAMERA_OP_SUCCESS != ret)
        {
            sensor_number = 0;
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDCameraSensorNumber FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3762_112_2_18_2_9_52_699,(uint8*)"");
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDCameraSensorNumber out sensor_number = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3766_112_2_18_2_9_52_700,(uint8*)"d", sensor_number);
    return (int32)sensor_number;
}

/*****************************************************************************/
// 	Description : save setting info by switch sensor
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveSettingInfoBySwitchSensor(SETTING_SENSOR_ID_E  sensor_id)
{
    SaveSettingSensorInfo(sensor_id, &MMIDC_SETTING_DATA_PTR->dc_setting_info);
}

/*****************************************************************************/
// 	Description : get burst number by photo size 
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC MULTI_SHOOT_E MMIDC_GetBurstNumberByPhotoSize(MMIDC_PHOTO_SIZE_E photo_size)
{
    //DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32  max_photo_number = 0;

    //DCAMERA_PARAM_PHOTO_SIZE_E dcam_photo_size = 0;
    //DCAMERA_BURST_INFO_U  param = {0};


#if 1    
    do
    {
        if (photo_size >= PHOTO_SIZE_2048X1536)
        {
            break;
        }
        
        max_photo_number = MULTI_SHOOT_6;
    }while(0);
#endif

#if 0
    if (MMIDC_IsDCOpen())
    {
        dcam_photo_size = (uint32)MMIDC_ConvertPhotoSizeToDCameraSize(photo_size); 

        param.get_info_input.size_type = dcam_photo_size;
        param.get_info_input.cap_rot = 0;
        param.get_info_input.review_rot = MMIDC_GetReviewPhotoAngle();
        
        ret = DCAMERA_GetInfo(DCAMERA_INFO_BURST_PERMIT_NUM, &param);
        if (DCAMERA_OP_SUCCESS == ret)
        {
            max_photo_number = param.img_burst_cnt;
        }
    }
#endif

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetBurstNumberByPhotoSize num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3805_112_2_18_2_9_52_701,(uint8*)"d",max_photo_number);

    return (MULTI_SHOOT_E)max_photo_number;
}

/*****************************************************************************/
// 	Description : get max photo size to burst
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetMaxPhotoSizeToBurst(MULTI_SHOOT_E shoot_number)
{
    MMIDC_PHOTO_SIZE_E photo_size = 0;
    MULTI_SHOOT_E      max_photo_number  = MULTI_SHOOT_DISABLE;

    photo_size = PHOTO_SIZE_MAX - 1;
    while(photo_size >= 0) /*lint !e568*/
    {
        max_photo_number = MMIDC_GetBurstNumberByPhotoSize(photo_size);
        if (max_photo_number >= shoot_number)
        {
            break;
        }
        photo_size--;
    }
    
    if (photo_size < 0) /*lint !e568*/
    {
        photo_size = 0;
    }
    return photo_size;
}

/*****************************************************************************/
// 	Description : init param ctrl 
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_InitParamCtrl(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_InitParamCtrl"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3843_112_2_18_2_9_52_702,(uint8*)"");
    
    MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl = 0;
    MMIDC_CloseCallbackTimer();

    return 0;
}

/*****************************************************************************/
//Description : get photo size or video size string
//Global resource dependence : none
//  Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetSizeString(uint16 size_index, //in
                                       MMI_STRING_T  *str_ptr //out
                                       )
{
    MMIDC_SIZE_STR_T *size_str_ptr = PNULL;
    CAMERA_MODE_E   cur_mode = 0;

    cur_mode = MMIDC_GetCameraMode();
    if (CAMERA_MODE_DC == cur_mode)
    {
        if (size_index >= PHOTO_SIZE_MAX)
        {
            return;
        }
        size_str_ptr = &s_photo_size_str_array[size_index];
    }
    else
    {
        if (size_index >= VIDEO_SIZE_MAX)
        {
            return;
        }
        size_str_ptr = &s_video_size_str_array[size_index];
    }
    
    str_ptr->wstr_ptr = size_str_ptr->str; 
    str_ptr->wstr_len = MMIAPICOM_Wstrlen(size_str_ptr->str);
}

/*****************************************************************************/
//  Description : get photo size width and height
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetPhotoSizeWidthAndHeight(MMIDC_PHOTO_SIZE_E photo_size, //in
                                                          MMIDC_SIZE_ATTRIBUTE_T *size_ptr //out
                                                         )
{
    if (PNULL != size_ptr && photo_size < PHOTO_SIZE_MAX)
    {
        SCI_MEMCPY(size_ptr, &s_photo_size_val_array[photo_size], sizeof(*size_ptr));
    }
}

/*****************************************************************************/
//  Description : get video size width and height
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetVideoSizeWidthAndHeight(VIDEO_SIZE_E video_size, //in
                                                          MMIDC_SIZE_ATTRIBUTE_T *size_ptr //out
                                                         )
{
    if (PNULL != size_ptr && video_size < VIDEO_SIZE_MAX)
    {
        SCI_MEMCPY(size_ptr, &s_video_size_val_array[video_size], sizeof(*size_ptr));
    }
}    

/*****************************************************************************/
//  Description : get dc modul sensor type by id
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_IMG_SENSOR_TYPE_E  MMIAPIDC_GetDCModulSensorTypeById(int32 id)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    DCAMERA_IMG_SENSOR_TYPE_E  sensor_type = 0;
    uint32 sensor_type_param = 0;
    uint32 sensor_param = 0;
        
    if (id >= SETTING_SENSOR_ID_MAX)
    {
        return sensor_type;
    }
    
    sensor_type = s_fixed_param.sensor_type[id]; 
    
    //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_GetDCModulSensorType sensor_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3924_112_2_18_2_9_52_703,(uint8*)"d", sensor_type);
    
    if (DCAMERA_IMG_SENSOR_TYPE_MAX == sensor_type)
    {
        if (MMIDC_IsDCOpen() || MMIDC_IsDVOpen())
        {
            MMIDC_SetDCameraSensorID(id);
            ret = DCAMERA_GetInfo(DCAMERA_INFO_SENSOR_TYPE, &sensor_type_param);        
        }
        else
        {
             ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
             if (DCAMERA_OP_SUCCESS == ret)
             {
                 switch(id)
                {
                case SETTING_SENSOR_FRONT:
                    sensor_param = SENSOR_SUB;
                    break;
                case SETTING_SENSOR_REAR:
                    sensor_param = SENSOR_MAIN;
                    break;
                default:
                    sensor_param = SENSOR_MAIN;
                    break;
                }
                ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, sensor_param, PNULL);

                ret = DCAMERA_GetInfo(DCAMERA_INFO_SENSOR_TYPE, &sensor_type_param);
                DCAMERA_Close();
             }
         }

         if (DCAMERA_OP_SUCCESS == ret)
         {
            sensor_type = sensor_type_param;
            s_fixed_param.sensor_type[id] = sensor_type;
         }
    }

    
     //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_GetDCModulSensorType sensor_type 2=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3950_112_2_18_2_9_52_704,(uint8*)"d", sensor_type);
     
     return sensor_type;
}

/*****************************************************************************/
//  Description : get dc modul sensor type
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_IMG_SENSOR_TYPE_E  MMIAPIDC_GetDCModulSensorType(void)
{
    SETTING_SENSOR_ID_E id = 0;
    
    if (PNULL != s_setting_data_ptr)
    {
        id = MMIDC_GetSettingSensorID();
    }

    return MMIAPIDC_GetDCModulSensorTypeById(id);
}

/*****************************************************************************/
//  Description : set backup frame index
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetBackupFrameIndex(FRAME_E frame_index)
{
    if (frame_index >= FRAME_MAX)
    {
        return;
    }
    
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_backup_info.last_frame_index = frame_index;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetBackupFrameIndex frame_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_3968_112_2_18_2_9_52_705,(uint8*)"d", frame_index);
}

/*****************************************************************************/
//  Description : get backup frame index
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC FRAME_E MMIDC_GetBackupFrameIndex(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_backup_info.last_frame_index;
}

/*****************************************************************************/
//  Description : Backup Running Parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int16  MMIDC_BackupRunningParameter(DO_ACTION_E action
                                                        )
{
    int16 ret = 0;
    FRAME_E frame_index = 0;

    frame_index = MMIDC_GetFrameIndex();
    MMIDC_SetBackupFrameIndex(frame_index);
    
    return ret;
}

/*****************************************************************************/
//  Description : Restore Running Parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int16  MMIDC_RestoreRunningParameter(DO_ACTION_E action
                                                        )
{
    int16 ret = 0;
    FRAME_E frame_index = 0;

    frame_index = MMIDC_GetBackupFrameIndex();
    MMIDC_SetFrameIndex(frame_index, FALSE);
    
    return ret;
}

/*****************************************************************************/
//Description : set NV shutter voice enum
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNVShutterVoice(SHUTTER_VOICE_E voice)
{
    if (voice >= SHUTTER_VOICE_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_SetNVShutterVoice voice"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4020_112_2_18_2_9_52_706,(uint8*)"");
        return ;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetNVShutterVoice voice=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4024_112_2_18_2_9_52_707,(uint8*)"d", voice);
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 3)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return;
    }
   
	MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.shutter_voice = voice;    
	MMIDC_SETTING_DATA_PTR->setting_number --;
    MMIDC_SetShutterVoice(voice);
}

/*****************************************************************************/
//Description : get NV shutter voice enum
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC SHUTTER_VOICE_E MMIDC_GetNVShutterVoice(void)
{    
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.shutter_voice;
}

/*****************************************************************************/
//Description : get NV photo date
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC PHOTO_DATE_E MMIDC_GetNVPhotoDate(void)
{
    if(PHOTO_DATE_MAX < MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_date)
	{
		MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_date = PHOTO_DATE_ON;
	}
	
	return MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_date;
}

/*****************************************************************************/
//Description : set NV photo date
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNVPhotoDate(PHOTO_DATE_E photo_date)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_nv_info.photo_date = photo_date;
}

/*****************************************************************************/
//Description : get thumbnail photo size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetThumbnailPhotoSize(void)
{
    uint32  size_param = 0;
    MMIDC_PHOTO_SIZE_E photo_size = PHOTO_SIZE_320X240;
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  current_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);

    if (MMIDC_IsDCOpen())
    {
        ret = DCAMERA_GetInfo(DCAMERA_INFO_THUMBNAIL_SIZE, &size_param);

        if (DCAMERA_OP_SUCCESS == ret)
        {
            photo_size = MMIDC_ConvertDCameraSizeToPhotoSize((DCAMERA_PARAM_PHOTO_SIZE_E)size_param);
            if (PHOTO_SIZE_MAX <= photo_size)
            {
                if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type
                    && LCD_ANGLE_0 == current_angle
                    )
                {
                    photo_size = PHOTO_SIZE_240X320;
                }
                else
                {
                    photo_size = PHOTO_SIZE_320X240;
                }
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetThumbnailPhotoSize photo_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4090_112_2_18_2_9_52_708,(uint8*)"d",photo_size);
    
    return photo_size;
}

/*****************************************************************************/
//  Description : get dc module sensor number
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32  MMIAPIDC_GetDCModuleSensorNumber(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32  sensor_number = 0;
    
    sensor_number = s_fixed_param.sensor_number; 
    
    //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_GetDCModuleSensorNumber sensor_number=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4107_112_2_18_2_9_52_709,(uint8*)"d",sensor_number);
    
    if (SENSOR_NUMBER_INVALID == sensor_number)
    {
        if (MMIDC_IsDCOpen() || MMIDC_IsDVOpen())
        {
            sensor_number = MMIDC_GetDCameraSensorNumber();        
        }
        else
        {
             ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
             if (DCAMERA_OP_SUCCESS == ret)
             {
                ret = DCAMERA_GetInfo((DCAMERA_INFO_CMD_E)DCAMERA_INFO_IMG_SENSOR_NUM, &sensor_number);
             }
             DCAMERA_Close();
         }

         if (DCAMERA_OP_SUCCESS == ret)
         {
            s_fixed_param.sensor_number = sensor_number;
         }
    }

    
     //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_GetDCModuleSensorNumber sensor_number 2=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4133_112_2_18_2_9_52_710,(uint8*)"d",sensor_number);
     
     return sensor_number;
}

#if defined(MMIDC_F_SOFTWARE_ANGLE)
/*****************************************************************************/
//Description : get software rotation angle
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMIDC_GetSoftwareRotationAngle(void)
{
    LCD_ANGLE_E angle = 0;

    angle = s_mmidc_info.software_rot_angle;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetSoftwareRotationAngle angle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4150_112_2_18_2_9_52_711,(uint8*)"d",angle);
    
    return angle;
}

/*****************************************************************************/
//Description : set software rotation angle
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetSoftwareRotationAngle(LCD_ANGLE_E angle)
{
    s_mmidc_info.software_rot_angle = angle;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetSoftwareRotationAngle angle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4164_112_2_18_2_9_52_712,(uint8*)"d",angle);
}

/*****************************************************************************/
//Description : get dc module software rotation angle
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetDCModuleSoftwareRotationAngle(void)
{
    BOOLEAN ret = FALSE;
    DCAMERA_RETURN_VALUE_E dc_ret = 0;
    DRECORDER_RETURN_E dv_ret = 0;
    uint32 param = 0;    

    if (MMIDC_IsDCOpen())
    {
        dc_ret = DCAMERA_GetInfo(DCAMERA_INFO_PREVIEW_ROT, &param);
        if (DCAMERA_OP_SUCCESS == dc_ret)
        {
            ret = TRUE;
        }
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle dc_ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4186_112_2_18_2_9_52_713,(uint8*)"d",dc_ret);
    }
    else if (MMIDC_IsDVOpen())
    {
        dv_ret = DRECORDER_GetInfo(DRECORDER_INFO_PREVIEW_ROT, &param);
        if (DRECORDER_SUCCESS == dv_ret)
        {
            ret = TRUE;
        }
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle dv_ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4196_112_2_18_2_9_52_714,(uint8*)"d",dv_ret);
    }

    if (ret)
    {
        MMIDC_SetSoftwareRotationAngle((LCD_ANGLE_E)param);
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDCModuleSoftwareRotationAngle ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4205_112_2_18_2_9_52_715,(uint8*)"d",ret);

    return ret;
}
#endif

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//  Description : get dv max zoom  rect
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDVMaxZoomLevel(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    uint32 zoom_max = 0;    

    if (MMIDC_IsDVOpen())
    {
        ret = DRECORDER_GetInfo(DRECORDER_INFO_ZOOM, &zoom_max);
        if (DRECORDER_SUCCESS != ret)
        {
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDVMaxZoomLevel FAIL ret=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4226_112_2_18_2_9_52_716,(uint8*)"d", ret);
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDVMaxZoomLevel out zoom_max = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4230_112_2_18_2_9_52_717,(uint8*)"d", zoom_max);
    
    return zoom_max;
}

/*****************************************************************************/
//  Description : set dv zoom value enum
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_UpdateDVZoomValue(uint32 zoom)
{
    BOOLEAN result = FALSE;
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    uint32 zoom_param = 0;
    
    MMIDC_SETTING_DATA_PTR->setting_number ++;
    if(MMIDC_SETTING_DATA_PTR->setting_number >= 4)
    {
        MMIDC_SETTING_DATA_PTR->setting_number --;
        return result;
    }   

    zoom_param = zoom;
    if(MMIDC_IsDVOpen())
    {
        uint32 zoom_max = 0;
            
        zoom_max = MMIDC_GetDVMaxZoomLevel();
        if(zoom_max <= 1)
        {
            zoom_param = 0;
        }
        if(zoom >= zoom_max)
        {
            zoom_param = 0;
        }

        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl ++;
        MMIDC_StartCallbackTimer();
        ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_ZOOM, zoom_param, ParamCtrlCallBack);
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_UpdateDVZoomValue ret=%d, zoom_param=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4272_112_2_18_2_9_53_718,(uint8*)"dd", ret, zoom_param);
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_UpdateDVZoomValue zoom=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4273_112_2_18_2_9_53_719,(uint8*)"d", zoom);

    
    if(DRECORDER_SUCCESS != ret)
    {
        MMIDC_SETTING_DATA_PTR->is_setting_param_ctrl --;
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
    else
    {
         MMIDC_SETTING_DATA_PTR->dc_setting_info.video_zoom = zoom_param;
         result = TRUE;
    }
    
    MMIDC_SETTING_DATA_PTR->setting_number --;

    return result;
}

/*****************************************************************************/
//  Description : get dv zoom value enum
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDVZoomValue(void)
{
    return MMIDC_SETTING_DATA_PTR->dc_setting_info.video_zoom;
}

/*****************************************************************************/
//  Description : increase dv zoom value
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IncreaseDVZoomValue(void)
{
    BOOLEAN ret = FALSE;
    uint32 zoom_max = MMIDC_GetDVMaxZoomLevel();
    uint32 cur_zoom = MMIDC_GetDVZoomValue();

    if( (zoom_max > 1) 
        && (cur_zoom < zoom_max - 1 )
       )
    {
        ret = MMIDC_UpdateDVZoomValue(cur_zoom + 1);
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IncreaseDVZoomValue ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4318_112_2_18_2_9_53_720,(uint8*)"d", ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : derease dv zoom value
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DecreaseDVZoomValue(void)
{
    BOOLEAN ret = FALSE;
    uint32 zoom_max = MMIDC_GetDVMaxZoomLevel();
    uint32 cur_zoom = MMIDC_GetDVZoomValue();
    
    if (zoom_max > 1 
        &&  cur_zoom > 0
       )
    {
        ret = MMIDC_UpdateDVZoomValue(cur_zoom - 1);
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DecreaseDVZoomValue ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4340_112_2_18_2_9_53_721,(uint8*)"d", ret);
    
    return ret;
}
#endif //#if defined(MMIDC_F_DV_ZOOM)

/*****************************************************************************/
//  Description : get screen photo size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetScreenPhotoSize(void)
{
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    MMIDC_PHOTO_SIZE_E screen_size = 0;

    if(SCREEN_MODE_VERTICAL == screen_mode)
    {
        screen_size = MMIDC_GePhotoSizetVerLcd();
    }
    else
    {
        screen_size = MMIDC_GePhotoSizetHorLcd();
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetScreenPhotoSize screen_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4365_112_2_18_2_9_53_722,(uint8*)"d",screen_size);
                
    return screen_size;
}

/*****************************************************************************/
//  Description : get max video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetCurMaxVideoSize(void)
{   
    VIDEO_SIZE_E    max_video_size = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;
    VIDEO_FILE_TYPE_E  file_type = 0;

    VIDEO_SIZE_E srv_video_size = 0;
    LCD_ANGLE_E  platform_angle = MMIDC_GetPlatformAngle();
    
    file_type = MMIDC_GetRecordFileType();
    sensor_type = MMIAPIDC_GetDCModulSensorType();

    if ((DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
        || ( DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type
            && LCD_ANGLE_90 == platform_angle
           )
       )
    {
        //DCAMERA_IMG_SENSOR_TYPE_4P3
        if (VIDEO_FILE_TYPE_3GP == file_type 
            || VIDEO_FILE_TYPE_AVI == file_type 
            )
        {
#if defined MMIDC_MINI_DISPLAY_STYLE  
			max_video_size = VIDEO_SIZE_320X240;
#else
            if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
            {
                max_video_size = VIDEO_SIZE_320X240;
            }
            else
            {
    			#if defined(MMIDC_F_VIDEO_352X288)
                max_video_size = VIDEO_SIZE_352X288;
                #else
                max_video_size = VIDEO_SIZE_320X240;
                #endif
            }
#endif		
		}
        else if(VIDEO_FILE_TYPE_MP4 == file_type)
        {
            max_video_size = VIDEO_SIZE_160X128;
        }
    }
    else
    {
        //DCAMERA_IMG_SENSOR_TYPE_3P4
        if (VIDEO_FILE_TYPE_3GP == file_type 
            || VIDEO_FILE_TYPE_AVI == file_type 
            )
        {
            max_video_size = VIDEO_SIZE_240X320;
        }
        else if(VIDEO_FILE_TYPE_MP4 == file_type)
        {
            max_video_size = VIDEO_SIZE_128X160;
        }
    }

    srv_video_size = MMIDCSRV_GetMaxVideoSizee();

    max_video_size = MIN(srv_video_size, max_video_size);


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetCurMaxVideoSize max_video_size= %d, file_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4414_112_2_18_2_9_53_723,(uint8*)"dd",max_video_size, file_type);
    
    return max_video_size;
}

/*****************************************************************************/
//  Description : get min video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetMinVideoSize(void)
{   
    VIDEO_SIZE_E    min_video_size = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;
    
    
    sensor_type = MMIAPIDC_GetDCModulSensorType();

    if (DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
    {
        min_video_size = VIDEO_SIZE_128X96;
    }
    else
    {
       min_video_size = VIDEO_SIZE_96X128;
    }


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMiniVideoSize min_video_size= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4442_112_2_18_2_9_53_724,(uint8*)"d",min_video_size);
    
    return min_video_size;
}

/*****************************************************************************/
//Description : get support video size array
//lobal resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetSupportVideoSizeArray(int16 max_item, //in
                                                        VIDEO_SIZE_E *size_list_ptr //out
                                                        )
{
    VIDEO_FILE_TYPE_E  file_type = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;    

    
    VIDEO_SIZE_E  *video_size_ptr = PNULL;
    LCD_ANGLE_E  platform_angle = MMIDC_GetPlatformAngle();
    
    VIDEO_SIZE_E  avi_array[]=
    {
#if !defined(CMCC_UI_STYLE) //CMCC入库不能小于QCIF
        VIDEO_SIZE_128X96,
        VIDEO_SIZE_160X128,
#endif
        VIDEO_SIZE_176X144,
        VIDEO_SIZE_320X240,

#ifdef MMIDC_F_VIDEO_352X288
        VIDEO_SIZE_352X288
#endif
    };
    
    VIDEO_SIZE_E  mp4_array[]=
    {
        VIDEO_SIZE_128X96,
        VIDEO_SIZE_160X128
    };
    VIDEO_SIZE_E  avi_3p4_array[]=
    {
        VIDEO_SIZE_96X128,
        VIDEO_SIZE_128X160,
        VIDEO_SIZE_144X176,
        VIDEO_SIZE_240X320
    };
    
    VIDEO_SIZE_E  mp4_3p4_array[]=
    {
        VIDEO_SIZE_96X128,
        VIDEO_SIZE_128X160
    };

    uint16  copy_count = 0;
    uint32 copy_bytes = 0;
    int16 total_item = 0;


    file_type = MMIDC_GetRecordFileType();
    sensor_type = MMIAPIDC_GetDCModulSensorType();

    if ((DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
        || (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type
            && (LCD_ANGLE_90 == platform_angle)
           )
       )
    {
        if(file_type == VIDEO_FILE_TYPE_3GP || file_type == VIDEO_FILE_TYPE_AVI)
        {
            video_size_ptr = avi_array;
            total_item = ARR_SIZE(avi_array);
        }
        else if(file_type == VIDEO_FILE_TYPE_MP4)
        {
            video_size_ptr = mp4_array;
            total_item = ARR_SIZE(mp4_array);
        }
    }
    else if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
    {
        if(file_type == VIDEO_FILE_TYPE_3GP || file_type == VIDEO_FILE_TYPE_AVI)
        {
            video_size_ptr = avi_3p4_array;
            total_item = ARR_SIZE(avi_3p4_array);
        }
        else if(file_type == VIDEO_FILE_TYPE_MP4)
        {
            video_size_ptr = mp4_3p4_array;
            total_item = ARR_SIZE(mp4_3p4_array);
        }
    }

    if ((PNULL !=video_size_ptr)
        && (PNULL != size_list_ptr)
       )
    {
        copy_count = MIN(total_item, max_item);
        copy_bytes = copy_count * sizeof(size_list_ptr[0]);
        SCI_MEMCPY(size_list_ptr, video_size_ptr, copy_bytes);
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetSupportVideoSizeArray copy_count=%d, total_item=%d, video_size_ptr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4533_112_2_18_2_9_53_725,(uint8*)"ddd",copy_count, total_item, (uint32)video_size_ptr);
    
    return copy_count;
}

/*****************************************************************************/
//  Description : get max burst photo number
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetMaxBurstPhotoNumber(void)
{
    return MAX_DC_BURST_PHOTO_NUMBER;
}
/*****************************************************************************/
//  Description : get data format info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetDataFormatInfo(MMIDC_DATA_FORMAT_INFO_T *info_ptr//out
                                             )
{
    GUIIMG_DATE_TYPE_E data_format = GUIIMG_DATE_TYPE_RGB565;
    uint8 data_unit = 2;
#ifdef UI_MULTILAYER_SUPPORT    
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
#endif
    
    if (PNULL != info_ptr)
    {
#ifdef UI_MULTILAYER_SUPPORT        
		if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&dev_info))
        {
            data_format = GUIIMG_DATE_TYPE_ARGB888;
            data_unit = 4;
        }
#endif        
        info_ptr->data_format = data_format;
        info_ptr->data_unit = data_unit;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDataFormatInfo data_format=%d, data_unit=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4570_112_2_18_2_9_53_726,(uint8*)"dd",data_format, data_unit);
}

/*****************************************************************************/
//  Description : set dcam source display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetDCamSrcDisplay(DCAMERA_DISPALY_PARAM_T src_disp)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.src_dcam_display = src_disp;
}

/*****************************************************************************/
//  Description : get dcam source display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISPALY_PARAM_T MMIDC_GetDCamSrcDisplay(void)
{
    DCAMERA_DISPALY_PARAM_T src_disp = {0};
    
    src_disp = MMIDC_SETTING_DATA_PTR->dc_setting_info.src_dcam_display;

    return src_disp;
}

/*****************************************************************************/
//  Description : set dcam destination display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetDCamDstDisplay(DCAMERA_DISPALY_PARAM_T dst_disp)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.dst_dcam_display = dst_disp;
}

/*****************************************************************************/
//  Description : get dcam destination display parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISPALY_PARAM_T MMIDC_GetDCamDstDisplay(void)
{
    DCAMERA_DISPALY_PARAM_T dst_disp = {0};
    
    dst_disp = MMIDC_SETTING_DATA_PTR->dc_setting_info.dst_dcam_display;

    return dst_disp;
}

/*****************************************************************************/
//  Description : set mmi brush status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMMIBrushStatus(MMIDC_BRUSH_STATUS_E status)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.lcd_brush_status = status;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetMMIBrushStatus status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4626_112_2_18_2_9_53_727,(uint8*)"d",status);
}

/*****************************************************************************/
//  Description : get mmi brush status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_BRUSH_STATUS_E MMIDC_GetMMIBrushStatus(void)
{
    MMIDC_BRUSH_STATUS_E status = 0;
    
    status = MMIDC_SETTING_DATA_PTR->dc_setting_info.lcd_brush_status;


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMMIBrushStatus status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4641_112_2_18_2_9_53_728,(uint8*)"d",status);

    return status;
}

/*****************************************************************************/
//  Description : mmi is brushing or not
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsMMIBrushing(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_BRUSH_STATUS_E status = 0;

    status = MMIDC_GetMMIBrushStatus();
    if (MMIDC_BRUSH_START != status)
    {
        ret = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsMMIBrushing ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4662_112_2_18_2_9_53_729,(uint8*)"d",ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : set mmi focus status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetMMIFocusStatus(MMIDC_FOCUS_STATUS_E status)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.focus_status = status;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetMMIFocusStatus status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4676_112_2_18_2_9_53_730,(uint8*)"d",status);
}

/*****************************************************************************/
//  Description : get mmi focus status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_FOCUS_STATUS_E MMIDC_GetMMIFocusStatus(void)
{
    MMIDC_FOCUS_STATUS_E status = 0;
    
    status = MMIDC_SETTING_DATA_PTR->dc_setting_info.focus_status;


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMMIFocusStatus status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4691_112_2_18_2_9_53_731,(uint8*)"d",status);

    return status;
}

/*****************************************************************************/
//  Description : push to backup photo size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushPhotoSize(MMIDC_PHOTO_SIZE_E photo_size)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_backup_info.photo_size = photo_size;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_PushPhotoSize photo_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4705_112_2_18_2_9_53_732,(uint8*)"d",photo_size);
}

/*****************************************************************************/
//  Description : pop to get backuped photo size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_PopPhotoSize(void)
{
    MMIDC_PHOTO_SIZE_E photo_size = 0;
    
    photo_size = MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_backup_info.photo_size;



    SCI_TRACE_LOW("[MMIDC] MMIDC_PopPhotoSize photo_size=%d",
                  photo_size);


    return photo_size;
}

/*****************************************************************************/
//  Description : push to backup visual mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushVisualMode(CAMERA_ENG_VISUAL_MODE_E visual_mode)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_backup_info.visual_mode = visual_mode;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_PushVisualMode visual_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SETTING_4734_112_2_18_2_9_53_734,(uint8*)"d",visual_mode);
}

/*****************************************************************************/
//  Description : pop to get backuped visual mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC CAMERA_ENG_VISUAL_MODE_E MMIDC_PopVisualMode(void)
{
    CAMERA_ENG_VISUAL_MODE_E visual_mode = 0;
    
    visual_mode = MMIDC_SETTING_DATA_PTR->dc_setting_info.setting_backup_info.visual_mode;



    SCI_TRACE_LOW("[MMIDC] MMIDC_PopVisualMode visual_mode=%d",
                  visual_mode);


    return visual_mode;
}

/*****************************************************************************/
//  Description : init dc module setting info to default value
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitSettingInfoToDefaultValue(void)
{
    MMIDC_InitNVSettingDefaultValue(&MMIDC_SETTING_DATA_PTR->dc_setting_info, FALSE);
}

/*****************************************************************************/
// 	Description : get jpeg data size by photo size 
//	Global resource dependence : none
//  Author: robert.wang 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetJPEGSizeByPhotoSize(MMIDC_PHOTO_SIZE_E photo_size)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32  jpeg_size = 0;

    DCAMERA_PARAM_PHOTO_SIZE_E dcam_photo_size = 0;
    uint32  param = 0;

    
    if (MMIDC_IsDCOpen())
    {
        dcam_photo_size = (uint32)MMIDC_ConvertPhotoSizeToDCameraSize(photo_size); 

        if (dcam_photo_size < DCAMERA_PHOTO_SIZE_MAX)
        {
            param =   dcam_photo_size;
            ret = DCAMERA_GetInfo(DCAMERA_INFO_JPEG_SIZE, &param);
            
            SCI_TRACE_LOW("[MMIDC] MMIDC_GetJPEGSizeByPhotoSize ret=%d,param = %d",\
                    ret, param);
        }
        
        if (DCAMERA_OP_SUCCESS == ret)
        {
            jpeg_size = param;
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] MMIDC_GetJPEGSizeByPhotoSize jpeg_size = %d",\
                jpeg_size);

    return jpeg_size;
}

/*****************************************************************************/
//  Description : get file handle mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC SCI_MUTEX_PTR MMIDC_GetFileHandleMutexPtr(void)
{
    SCI_MUTEX_PTR mutex_ptr = PNULL;

    
    mutex_ptr = MMIDC_SETTING_DATA_PTR->dc_setting_info.file_handle_mutex_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetFileHandleMutexPtr mutex_ptr=%d",
                 mutex_ptr);

    return mutex_ptr;                 
}

/*****************************************************************************/
//  Description : get file handle mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFileHandleMutexPtr(SCI_MUTEX_PTR mutex_ptr)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.file_handle_mutex_ptr = mutex_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetFileHandleMutexPtr mutex_ptr=%d",
                 mutex_ptr);             
}

/*****************************************************************************/
//  Description : create file handle mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateFileHandleMutexPtr(void)
{
    SCI_MUTEX_PTR mutex_ptr = PNULL;


    mutex_ptr = SCI_CreateMutex("MMIDCFileHandleMutex",SCI_NO_INHERIT);
    
    MMIDC_SetFileHandleMutexPtr(mutex_ptr);

    SCI_TRACE_LOW("[MMIDC] MMIDC_CreateFileHandleMutexPtr mutex_ptr=%d",
                 mutex_ptr);             
}

/*****************************************************************************/
//  Description : delete file handle mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteFileHandleMutexPtr(void)
{
    SCI_MUTEX_PTR mutex_ptr = PNULL;


    mutex_ptr = MMIDC_GetFileHandleMutexPtr();
    if (PNULL != mutex_ptr)
    {
        MMIDC_PushFileHandleMutex();
        MMIDC_PopFileHandleMutex();
       
       SCI_DeleteMutex(mutex_ptr);
       MMIDC_SetFileHandleMutexPtr(PNULL);
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_DeleteFileHandleMutexPtr mutex_ptr=%d",
	             mutex_ptr);             
}

/*****************************************************************************/
//  Description : push file handle mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushFileHandleMutex(void)
{
    SCI_MUTEX_PTR mutex_ptr = MMIDC_GetFileHandleMutexPtr();


    SCI_TRACE_LOW("MMIDC_PushFileHandleMutex mutex_ptr=%d",\
                 mutex_ptr);

    if (PNULL != mutex_ptr)
    {
        SCI_GetMutex(mutex_ptr, SCI_WAIT_FOREVER);
    }
}

/*****************************************************************************/
//  Description : pop file handle mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopFileHandleMutex(void)
{
    SCI_MUTEX_PTR mutex_ptr = MMIDC_GetFileHandleMutexPtr();


    SCI_TRACE_LOW("MMIDC_PopFileHandleMutex mutex_ptr=%d",\
                 mutex_ptr);

    if (PNULL != mutex_ptr)
    {
        SCI_PutMutex(mutex_ptr);
    }
}

/*****************************************************************************/
//  Description : get img proc mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC SCI_MUTEX_PTR MMIDC_GetImgProcMutexPtr(void)
{
    SCI_MUTEX_PTR mutex_ptr = PNULL;

    
    mutex_ptr = MMIDC_SETTING_DATA_PTR->dc_setting_info.img_proc_mutex_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetImgProcMutexPtr mutex_ptr=%d",
                 mutex_ptr);

    return mutex_ptr;                 
}

/*****************************************************************************/
//  Description : get img proc mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImgProcMutexPtr(SCI_MUTEX_PTR mutex_ptr)
{
    MMIDC_SETTING_DATA_PTR->dc_setting_info.img_proc_mutex_ptr = mutex_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetImgProcMutexPtr mutex_ptr=%d",
                 mutex_ptr);             
}

/*****************************************************************************/
//  Description : create img proc mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateImgProcMutexPtr(void)
{
    SCI_MUTEX_PTR mutex_ptr = PNULL;


    mutex_ptr = MMIDC_GetImgProcMutexPtr();
    if (PNULL == mutex_ptr)
    {
        mutex_ptr = SCI_CreateMutex("MMIDCIMGPROCMutex",SCI_NO_INHERIT);
    }
    
    MMIDC_SetImgProcMutexPtr(mutex_ptr);

    SCI_TRACE_LOW("[MMIDC] MMIDC_CreateImgProcMutexPtr mutex_ptr=%d",
                 mutex_ptr);             
}

/*****************************************************************************/
//  Description : delete img proc mutex ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteImgProcMutexPtr(void)
{
    SCI_MUTEX_PTR mutex_ptr = PNULL;


    mutex_ptr = MMIDC_GetImgProcMutexPtr();
    if (PNULL != mutex_ptr)
    {
        MMIDC_PushImgProcMutex();
        MMIDC_PopImgProcMutex();
       
       SCI_DeleteMutex(mutex_ptr);
       MMIDC_SetImgProcMutexPtr(PNULL);
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_DeleteImgProcMutexPtr mutex_ptr=%d",
                 mutex_ptr);             
}

/*****************************************************************************/
//  Description : push img proc mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushImgProcMutex(void)
{
    SCI_MUTEX_PTR mutex_ptr = MMIDC_GetImgProcMutexPtr();


    SCI_TRACE_LOW("[MMIDC] MMIDC_PushImgProcMutex mutex_ptr=%d",\
                 mutex_ptr);

    if (PNULL != mutex_ptr)
    {
        SCI_GetMutex(mutex_ptr, SCI_WAIT_FOREVER);
    }
}

/*****************************************************************************/
//  Description : pop Img Proc mutex
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopImgProcMutex(void)
{
    SCI_MUTEX_PTR mutex_ptr = MMIDC_GetImgProcMutexPtr();


    SCI_TRACE_LOW("[MMIDC] MMIDC_PopImgProcMutex mutex_ptr=%d",\
                 mutex_ptr);

    if (PNULL != mutex_ptr)
    {
        SCI_PutMutex(mutex_ptr);
    }
}

/*****************************************************************************/
//Description : set photo size parameter 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoSizeParameter(MMIDC_PHOTO_SIZE_E size)
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_SetParameterPhotoSize size = %d",\
              size);

    MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size = size;
}

#ifdef MMIDC_MINI_RESOURCE_SIZE
/*****************************************************************************/
// 	Description : get setting photo size info
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
PUBLIC SETTING_PHOTO_SIZE_INFO_T * MMIDC_GetSettingPhotoSizeInfo(void)
{
    return &MMIDC_SETTING_DATA_PTR->dc_setting_info.photo_size_info;
}

/*****************************************************************************/
//Description :  get setting photo size info
//Global resource dependence : none
//Author: chunyou
//Note: size list from high size to low size
// Return: total supported number
/*****************************************************************************/
PUBLIC int16 MMIDC_SetSettingPhotoSizeInfo(void)
{
    int16  total_size_number = 0;
    int32  lcd_size = 0;
    SCREEN_MODE_E screen_mode = 0;
    SETTING_PHOTO_SIZE_INFO_T *size_ptr = MMIDC_GetSettingPhotoSizeInfo();
    int16 i = 0;

    if(PNULL == size_ptr)
    {
       return total_size_number;
    }
    //init size array to PHOTO_SIZE_MAX
    //PHOTO_SIZE_MAX is not support size
    for (i = 0; i < PHOTO_SIZE_MAX; i++)
    {
        size_ptr->photo_size_array[i] = PHOTO_SIZE_MAX;
    }
    lcd_size = MMIDC_GetLCDSize();
    screen_mode = MMIDC_GetScreenMode();
    total_size_number = GetPhonePhotoSizeList(PHOTO_SIZE_MAX, lcd_size, screen_mode, size_ptr->photo_size_array);
    size_ptr->total_num = total_size_number;
    
    return total_size_number;
}

/*****************************************************************************/
// 	Description : get setting video size info
//	Global resource dependence : none
//  Author: chunyou
//	Note:
/*****************************************************************************/
PUBLIC SETTING_VIDEO_SIZE_INFO_T * MMIDC_GetSettingVideoSizeInfo(void)
{
    return &MMIDC_SETTING_DATA_PTR->dc_setting_info.video_size_info;
}

/*****************************************************************************/
//Description :  get setting video size info
//Global resource dependence : none
//Author: chunyou
//Note: size list from high size to low size
// Return: total supported number
/*****************************************************************************/
PUBLIC int16 MMIDC_SetSettingVideoSizeInfo(void)
{
    int16  total_size_number = 0;
    SETTING_VIDEO_SIZE_INFO_T *size_ptr = MMIDC_GetSettingVideoSizeInfo();
    int16 i = 0;

    if(PNULL == size_ptr)
    {
       return total_size_number;
    }

    //init size array to VIDEO_SIZE_MAX
    //VIDEO_SIZE_MAX is not support size
    for (i = 0; i < VIDEO_SIZE_MAX; i++)
    {
        size_ptr->video_size_array[i] = VIDEO_SIZE_MAX;
    }
    total_size_number = MMIDC_GetPhoneVideoSizeList(VIDEO_SIZE_MAX, size_ptr->video_size_array);
    size_ptr->total_num = total_size_number;
    
    return total_size_number;
}
#endif // MMIDC_MINI_RESOURCE_SIZE

/*****************************************************************************/
//Description :  get sensor actual resolution snapshot size
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC DCAMERA_PARAM_PHOTO_SIZE_E MMIDC_GetActualResolutionSize(void)
{
    DCAMERA_PARAM_PHOTO_SIZE_E dcam_size = 0;
    
    dcam_size = DCAMERA_GetActualSnapshotMaxResolution();

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetActualResolutionSize dcam_size=%d",\
                  dcam_size);
    
    return dcam_size;
}


/*****************************************************************************/
//Description :  check 2M sensor state
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Check2MSensorState(MMIDCSRV_MEM_CFG_E mem_cfg)
{
    BOOLEAN ret = FALSE;

    if ((DCAMERA_PHOTO_SIZE_MAX != MMIDC_GetActualResolutionSize())
         && (MMIDCSRV_MEM_DC_NORMAL == mem_cfg)
        )
    {
        ret = TRUE;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_Check2MSensorState ret=%d",
                ret);

    return ret;
}

/*****************************************************************************/
//Description :  is 2M sensor state
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Is2MSensorState(void)
{
    BOOLEAN ret = FALSE;


    ret = MMIDC_Check2MSensorState(MMIDCSRV_GetDCMemCfg());

    SCI_TRACE_LOW("[MMIDC] MMIDC_Is2MSensorState ret=%d",
                ret);

    return ret;
}

/*****************************************************************************/
//Description :  init sensor param
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
PUBLIC void MMIDC_InitSensorParam(void)
{
    DCAMERA_CAM_PARAM_T sensor_param = {0};

    
    MMIAPIDC_InitSensor(FALSE, &sensor_param);
}

#endif  //#ifdef  CAMERA_SUPPORT

