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
@brief:����/��ͷ��/΢�������պ�¼��ӿ�

1.ʹ��CALLBACK��ʽʵ�����ݴ��ݣ� 
  ÿ��ֻ�������һ���ļ�����Ϣ�� 
  Ŀǰֻ�ṩ�ļ���ʽ���ݲ��ṩBUFFER��ʽ 

2.���պ�¼��ʹ��ͬһ���ӿ� 
3.�������ջ�¼���ܺ󣬲��ܻ����л� 
4.¼���ݲ�֧������ʱ�䣬 
  ��Ϊ�����㳡���йأ� 
  ʱ��̫�̣���ʾ�ڽ��������鲻�á� 

5.�ӿ�:
  (1) ͷ�ļ�:mmidc_export.h
  (2)��Ҫ����:MMIAPIDC_StartWork, MMIDC_WORK_CALLBACK
  (3)��Ӧ�ĺ�MMIDC_F_WORK_MODE
  
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
                ;//�������  
            } 
            else if(MMIFMM_FILE_TYPE_MOVIE == data_info_ptr->file_type) 
            { 
                ;//����¼��  
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
@brief: ��ͬ��������ͷ����ߴ�᲻ͬ�������ģ������
��WORK_PHOTO_SIZE_640_480 ��Ӧ 640x480 �� 480x640
*/
typedef enum
{
    WORK_PHOTO_SIZE_SCREEN,/*��Ļ�ߴ磬����(240,320), (240,400),(400,240),(320,480),(176,220)*/
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
@brief: ��ͬ��������ͷ����ߴ�᲻ͬ�������ģ������
��WORK_VIDEO_160_128 ��Ӧ 160x128 �� 128x160
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
@brief: ����ֵ
*/
typedef enum
{ 
    MMIDC_WORK_SUCCESS,//�ɹ� 
    MMIDC_WORK_ERROR,//�������� 
    MMIDC_WORK_ERROR_CANCEL_BY_USER,//�û�ȡ��
    MMIDC_WORK_ERROR_MAX
}MMIDC_WORK_ERROR_E;

/*
@brief: work mode
*/
typedef enum
{
    MMIDC_WORK_DC,//���� 
    MMIDC_WORK_DV,//¼�� 
    MMIDC_WORK_MAX
}MMIDC_WORK_E;//����ģʽ

/*
@brief: work data to return
*/
typedef struct
{
    MMIDC_WORK_E work_mode;//����ģʽ  
    int32 file_name_len;//�ļ�·������
    wchar *file_name;//�ļ����ƣ�����·����
}MMIDC_WORK_DATA_INFO_T;

/*
@brief:work mode allback function
*/
typedef int32 (*MMIDC_WORK_CALLBACK)(MMIDC_WORK_ERROR_E error_ret,/*in*/
                                     MMIDC_WORK_DATA_INFO_T *data_info_ptr /*in*/
                                     );
/*
@brief: dc param
������ճߴ磬ʵ�ʳߴ����С�ڵ��ڴ�ֵ,����С����Ļ�ߴ磬��С����Ļ�ߴ磬DCģ���Զ����������ٷ��ش���
���ֵ��ʾ֧���κγߴ磬0��ʾ֧����Ļ�ߴ磬DCģ�鸺������
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
    MMIDC_WORK_VIDEO_SIZE_E max_video_size;//���¼��ߴ磬ʵ�ʳߴ����С�ڵ��ڴ�ֵ  
    uint32 max_file_size;//(byte)����ļ��ߴ磬ʵ�ʳߴ����С�ڵ��ڴ�ֵ 
}MMIDC_WORK_DV_PARAM_T;

/*
@brief: work param
*/
typedef struct
{
    MMIDC_WORK_E work_mode;//����ģʽ�����ջ�¼�� 
    union
    {
        MMIDC_WORK_DC_PARAM_T dc;
        MMIDC_WORK_DV_PARAM_T dv;
    }param;
    MMIDC_WORK_CALLBACK work_callback;//�����ݸ������� 
}MMIDC_WORK_PARAM_T;


/*****************************************************************************/
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
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
@brief:��ͨ�������ӿ�
/*
@brief: work mode
*/
typedef enum
{
    MMIDCSRV_MODE_DC,//���� 
    MMIDCSRV_MODE_DV,//¼�� 
    MMIDCSRV_MODE_MAX
}MMIDCSRV_MODE_E;//����ģʽ

/*
@brief:�ڴ�����
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
    MMIDCSRV_MODE_E work_mode;//����ģʽ�����ջ�¼�� 

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
//	Note:  �鿴ͼƬ���������Ĭ��·���µ�ͼƬ
//     Ӧ������������ļ��е�ͼƬ���Դ�������
//    ������ĺ��� mmipicview_wintab.c   
//    HandleLoadAllPicWinMsg() ,HandlePiclistOptWinMsg()
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetChangeFolderState(BOOLEAN is_changed);    

/*****************************************************************************/
// 	Description : get change folder state
//	Global resource dependence : none
//  Author: robert.wang
//   Date: 2009-9-8
//	Note:  �鿴ͼƬ���������Ĭ��·���µ�ͼƬ
//     Ӧ������������ļ��е�ͼƬ���Դ�������
//    ������ĺ��� mmipicview_wintab.c   
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

