#ifndef _MMIDC_API_H
#define _MMIDC_API_H

#include "sci_types.h"
#include "mmk_type.h"
#include "dal_dcamera.h"
#include "dal_recorder.h"

#include "mmisrv.h"
#include "mmifilearray_export.h"

#ifdef NANDBOOT_SUPPORT 
#define MMIDC_MAX_PHOTO_SIZE            (4 * 1024 * 1024)
#else
#define MMIDC_MAX_PHOTO_SIZE            (970 * 1024)
#endif


/*
@brief:彩信/大头贴/微博的拍照和录像接口

1.使用CALLBACK方式实现数据传递， 
  每次只返回最后一个文件的信息。 
  目前只提供文件方式，暂不提供BUFFER方式 

2.拍照和录像使用同一个接口 
3.进入拍照或录像功能后，不能互相切换 
4.录像暂不支持设置时间， 
  因为与拍摄场景有关， 
  时间太短，显示在界面上体验不好。 

5.接口:
  (1) 头文件:mmidc_export.h
  (2)主要函数:MMIAPIDC_StartWork, MMIDC_WORK_CALLBACK
  (3)对应的宏MMIDC_F_WORK_MODE
  
sample code:

#ifdef MMIDC_F_WORK_MODE
LOCAL int32 WorkCallBack( MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr) 
{ 
    int32 ret = 0; 
    
    if (PNULL == data_info_ptr) 
    { 
        return ret; 
    } 

    //step 1. check return value
    if (MMIDC_WORK_SUCCESS == error_ret)  
    { 

        //step 2. check file name len 
        if (data_info_ptr->file_name_len > 0)
        {

            if (MMIFMM_FILE_TYPE_PICTURE == data_info_ptr->file_type)  
            { 
                ;//插入彩信  
            } 
            else if(MMIFMM_FILE_TYPE_MOVIE == data_info_ptr->file_type) 
            { 
                ;//插入录像  
            } 
        }
    }

    return ret; 
} 

1.capture
LOCAL void OpenDC(void) 
{ 
    MMIDC_WORK_PARAM_T work_param = {0}; 
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR; 

    work_param.work_mode = MMIDC_WORK_DC; 
    work_param.param.dc.max_photo_size = WORK_PHOTO_SIZE_640_480; 
    work_param.work_callback = WorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param); 
    if (MMIDC_WORK_SUCCESS == error_ret) 
    { 
        ;
    } 
    else 
    { 
        ;
    } 
} 

2.record
LOCAL void OpenDV(void) 
{ 
    MMIDC_WORK_PARAM_T work_param = {0}; 
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR; 

    work_param.work_mode = MMIDC_WORK_DV; 
    work_param.param.dv.max_file_size = 20 * 1024;
    work_param.param.dv.max_video_size = WORK_VIDEO_SIZE_160_128; 
    work_param.work_callback = WorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param); 
    if (MMIDC_WORK_SUCCESS == error_ret) 
    { 
        ;
    } 
    else 
    { 
        ;
    } 
} 
#endif

*/

/*
@brief: 不同类型摄像头，其尺寸会不同，由相机模块适配
故WORK_PHOTO_SIZE_640_480 对应 640x480 或 480x640
*/
typedef enum
{
    WORK_PHOTO_SIZE_SCREEN,/*屏幕尺寸，例如(240,320), (240,400),(400,240),(320,480),(176,220)*/
    WORK_PHOTO_SIZE_320_240,/*320x240 or 240x320*/
    WORK_PHOTO_SIZE_640_480,/*640x480 or 480x640*/
    WORK_PHOTO_SIZE_DEFAULT_SIZE = WORK_PHOTO_SIZE_640_480, //default size for not care size
    WORK_PHOTO_SIZE_1280_960,
    WORK_PHOTO_SIZE_1600_1200,
    WORK_PHOTO_SIZE_2048_1536,
    WORK_PHOTO_SIZE_2592_1944,
    WORK_PHOTO_SIZE_3264_2448,/*3264x2448 or 2448x3264*/
    WORK_PHOTO_SIZE_MAX
}MMIDC_WORK_PHOTO_SIZE_E;

/*
@brief: 不同类型摄像头，其尺寸会不同，由相机模块适配
故WORK_VIDEO_160_128 对应 160x128 或 128x160
*/
typedef enum
{
    WORK_VIDEO_SIZE_MIN, //min size
    WORK_VIDEO_SIZE_128_96 = WORK_VIDEO_SIZE_MIN,/*128x96 or 96x128*/
    WORK_VIDEO_SIZE_160_128,/*160x128 or 128x160*/
    WORK_VIDEO_SIZE_DEFAULT_SIZE = WORK_VIDEO_SIZE_160_128, //default size for not care size
    WORK_VIDEO_SIZE_176_144,
    WORK_VIDEO_SIZE_320X240,/*320x240 or 240x320*/
    WORK_VIDEO_SIZE_MAX
}MMIDC_WORK_VIDEO_SIZE_E;

/*
@brief: 返回值
*/
typedef enum
{ 
    MMIDC_WORK_SUCCESS,//成功 
    MMIDC_WORK_ERROR,//发生错误 
    MMIDC_WORK_ERROR_CANCEL_BY_USER,//用户取消
    MMIDC_WORK_ERROR_MAX
}MMIDC_WORK_ERROR_E;

/*
@brief: work mode
*/
typedef enum
{
    MMIDC_WORK_DC,//拍照 
    MMIDC_WORK_DV,//录像 
    MMIDC_WORK_MAX
}MMIDC_WORK_E;//工作模式

/*
@brief: work data to return
*/
typedef struct
{
    MMIDC_WORK_E work_mode;//工作模式  
    int32 file_name_len;//文件路径长度
    wchar *file_name;//文件名称（包括路径）
}MMIDC_WORK_DATA_INFO_T;

/*
@brief:work mode allback function
*/
typedef int32 (*MMIDC_WORK_CALLBACK)(MMIDC_WORK_ERROR_E error_ret,/*in*/
                                     MMIDC_WORK_DATA_INFO_T *data_info_ptr /*in*/
                                     );
/*
@brief: dc param
最大拍照尺寸，实际尺寸可能小于等于此值,不能小于屏幕尺寸，若小于屏幕尺寸，DC模块自动更正，不再返回错误。
最大值表示支持任何尺寸，0表示支持屏幕尺寸，DC模块负责适配
*/
typedef struct
{ 
    MMIDC_WORK_PHOTO_SIZE_E max_photo_size;
}MMIDC_WORK_DC_PARAM_T;

/*
@brief: dv param
*/
typedef struct
{ 
    MMIDC_WORK_VIDEO_SIZE_E max_video_size;//最大录像尺寸，实际尺寸可能小于等于此值  
    uint32 max_file_size;//(byte)最大文件尺寸，实际尺寸可能小于等于此值 
}MMIDC_WORK_DV_PARAM_T;

/*
@brief: work param
*/
typedef struct
{
    MMIDC_WORK_E work_mode;//工作模式，拍照或录像 
    union
    {
        MMIDC_WORK_DC_PARAM_T dc;
        MMIDC_WORK_DV_PARAM_T dv;
    }param;
    MMIDC_WORK_CALLBACK work_callback;//传数据给调用者 
}MMIDC_WORK_PARAM_T;


/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIDC_GetFileIDString       MMIAPIDC_GetFileIDString
#define MMIDC_CombinePathName       MMIAPIDC_CombinePathName
#define MMIDC_AllocSettingMemory    MMIAPIDC_AllocSettingMemory
#define MMIDC_FreeSettingMemory     MMIAPIDC_FreeSettingMemory
#define MMIDC_InitOSDMenuIcon       MMIAPIDC_InitOSDMenuIcon
#define MMIDC_FreeGUI               MMIAPIDC_FreeGUI
#define MMIDC_GetSavePhotoIndex     MMIAPIDC_GetSavePhotoIndex
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
#define MMIDC_GetDVSavePathState    MMIAPIDC_GetDVSavePathState
#define MMIDC_SetDVSavePathState    MMIAPIDC_SetDVSavePathState
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT



/*
@brief:普通相机对外接口
/*
@brief: work mode
*/
typedef enum
{
    MMIDCSRV_MODE_DC,//拍照 
    MMIDCSRV_MODE_DV,//录像 
    MMIDCSRV_MODE_MAX
}MMIDCSRV_MODE_E;//工作模式

/*
@brief:内存配置
*/
typedef enum
{
    MMIDCSRV_MEM_DC_NORMAL,
    MMIDCSRV_MEM_DC_MINI, //low memory, size is related
    MMIDCSRV_MEM_DC_UCAMERA,
    MMIDCSRV_MEM_DV_NORMAL,
    MMIDCSRV_MEM_DV_MINI,
    MMIDCSRV_MEM_MAX
}MMIDCSRV_MEM_CFG_E;

typedef struct
{ 
    MMIDCSRV_MEM_CFG_E mem_cfg;
    MMIDC_WORK_PHOTO_SIZE_E max_photo_size;
    DCAMERA_DISP_MODE_E disp_mode;
}MMIDCSRV_DC_PARAM_T;

typedef struct
{ 
    MMIDCSRV_MEM_CFG_E mem_cfg;
    MMIDC_WORK_VIDEO_SIZE_E max_video_size;
    DRECORDER_DISP_MODE_E disp_mode;
}MMIDCSRV_DV_PARAM_T;

typedef struct
{
    MMIDCSRV_MODE_E work_mode;//工作模式，拍照或录像 

    MMIDCSRV_DC_PARAM_T dc;
    MMIDCSRV_DV_PARAM_T dv;
}MMIDCSRV_WORK_PARAM_T;

/*****************************************************************************/
// 	Description : open dc main window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenPhotoWin(void);

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
// 	Description : open preview window
//	Global resource dependence : none
//  Author: feiyue.ji
//	Note:
/*****************************************************************************/
PUBLIC void WATCH_DC_OpenPhotoWin(void);
#endif
/*****************************************************************************/
// 	Description : open preview window
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenVideoWin(void);

/*****************************************************************************/
// 	Description : close dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Exit(void);

/*****************************************************************************/
// 	Description : interrupt camera
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Interrupt(void);

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_IsPreviewing(void);

/*****************************************************************************/
// 	Description : is video recording
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_IsRecording(void);

/*****************************************************************************/
// 	Description : set factory value
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Setting_SetFactoryValue(void);

/*****************************************************************************/
// 	Description : is permit delete current picture or video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_IsPermitDeleteFile(const wchar* file_name);

/*****************************************************************************/
// 	Description : get camera is open
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_IsOpened(void);

/*****************************************************************************/
// 	Description : open preview window
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenForChipTest(void);

/*****************************************************************************/
// 	Description : init camera module
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_InitModule(void);

/*****************************************************************************/
// 	Description : set change folder state
//	Global resource dependence : none
//  Author: robert.wang
//   Date: 2009-9-8
//	Note:  查看图片管理中浏览默认路径下的图片
//     应允许浏览其他文件夹的图片，以此来区别
//    相关联的函数 mmipicview_wintab.c   
//    HandleLoadAllPicWinMsg() ,HandlePiclistOptWinMsg()
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetChangeFolderState(BOOLEAN is_changed);    

/*****************************************************************************/
// 	Description : get change folder state
//	Global resource dependence : none
//  Author: robert.wang
//   Date: 2009-9-8
//	Note:  查看图片管理中浏览默认路径下的图片
//     应允许浏览其他文件夹的图片，以此来区别
//    相关联的函数 mmipicview_wintab.c   
//    HandleLoadAllPicWinMsg() ,HandlePiclistOptWinMsg()
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_GetChangeFolderState(void) ;

/*****************************************************************************/
//  Description : Close staroge Menu
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_CloseStorageMenu(void);

/*****************************************************************************/
//  Description : get dc modul sensor type by id
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_IMG_SENSOR_TYPE_E  MMIAPIDC_GetDCModulSensorTypeById(int32 id);

/*****************************************************************************/
//  Description : get dc modul sensor type
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_IMG_SENSOR_TYPE_E  MMIAPIDC_GetDCModulSensorType(void);


/*****************************************************************************/
//  Description : start dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_WORK_ERROR_E MMIAPIDC_StartWork(MMIDC_WORK_PARAM_T *work_param_ptr /*in*/
                                                    );

/*****************************************************************************/
// 	Description : create capture photo name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIDC_GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type);

/*****************************************************************************/
//  Description : combine path according to root, dir, name
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIDC_CombinePathName(wchar* path_name_ptr, uint16 path_name_len, const wchar* file_path,
                                    const wchar* file_name);

/*****************************************************************************/
// 	Description : malloc dc setting memory
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_AllocSettingMemory(void);

/*****************************************************************************/
// 	Description : free dc setting memory
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_FreeSettingMemory(void);

/*****************************************************************************/
// 	Description : Register DC menu group
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIDC_InitOSDMenuIcon(void);

/*****************************************************************************/
// 	Description : free dc gui resource
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_FreeGUI(void);

/*****************************************************************************/
// 	Description : get used lcd for camera
//	Global resource dependence : none
//  Author: jesse.yu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDC_GetLcdID(void);

/*****************************************************************************/
// 	Description : get current save photo index
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDC_GetSavePhotoIndex(
                                      uint32        pic_num,    //in
                                      FILEARRAY     pic_array   //in
                                      );

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPIDC_GetPhotoSavePath(void);


#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
/*****************************************************************************/
// 	Description : get dv is in save path state or not
//	Global resource dependence : none
//  Author: dream.chen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_GetDVSavePathState( void);

/*****************************************************************************/
// 	Description : set dv is in save path state or not
//	Global resource dependence : none
//  Author: dream.chen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetDVSavePathState(BOOLEAN is_in_dv);

#endif

#if defined(MMIDC_F_U_CAMERA)
/*****************************************************************************/
//  Description : open u camera window
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenUCameraPhotoWin(void);
#endif

/*****************************************************************************/
// 	Description : mmidcsrv open dc main window
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCSRV_OpenMainWin(MMIDCSRV_WORK_PARAM_T *work_param_ptr /*in*/);

/*****************************************************************************/
//  Description : init sensor
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_InitSensor(BOOLEAN is_force_init,
                                         DCAMERA_CAM_PARAM_T *sensor_param_ptr /*out*/
                                         );
                                         
/*****************************************************************************/
// 	Description : init camera module parameter
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Init(void);


#ifdef DC_WATCH_UI_SUPPORT
/*****************************************************************************/
//  Description : watch dc factory reset
//  Parameter: [In]  None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_DC_FactoryReset(void);
#endif
#endif

