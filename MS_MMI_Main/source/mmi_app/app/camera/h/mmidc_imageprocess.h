/*****************************************************************************
** File Name:      mmidc_imageprocess.h                                      *
** Author:                                                                   *
** Date:           01/26/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to u camera                           *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_IMAGEPROCESS_H_
#define _MMIDC_IMAGEPROCESS_H_ 

#if defined(MMIDC_F_U_CAMERA)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmidc_ucamera_internal.h"
#include "dal_dcamera.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/ 

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

typedef enum
{
    UCAMERA_STATUS_START,
    UCAMERA_STATUS_PROCESS,
    UCAMERA_STATUS_MAX
}MMIDC_U_CAMERA_STATUS_E;


typedef BOOLEAN (*UCAMERA_OPEN_FUNC)(void);
typedef void (*UCAMERA_DESTROY_FUNC)(void);
typedef struct
{
    MMIDC_IMAGE_PROCESS_TYPE_E type;
    UCAMERA_OPEN_FUNC open_func_ptr;
    UCAMERA_DESTROY_FUNC destroy_func_ptr;
}MMIDC_U_CAMERA_FUNCTION_ITEM_T;

typedef struct
{
    BOOLEAN is_enable;
    MMIDC_IMAGE_PROCESS_TYPE_E type;//
    wchar temp_dir[MMIFILE_FULL_PATH_MAX_LEN + 1];//temp directory to img proc
    uint32 temp_dir_len;
    BOOLEAN is_photo_data;//save photo image to file or not
    MMIDC_U_CAMERA_STATUS_E status;
    IMG_PROC_DATA_T photo_img_proc_data;//from image proc task
    MMIDC_BUFFER_DATA_T photo_data;//for review jpeg
}MMIDC_SETTING_U_CAMERA_INFO_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open u camera
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_U_CAMERA_ERR_E MMIDC_OpenUCamera(void);

/*****************************************************************************/
//  Description : close u camera
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseUCamera(void);

/*****************************************************************************/
//  Description : u camera is enable or not
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsUCameraEnable(void);

/*****************************************************************************/
//  Description : process image by command
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ProcessImage(MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr, //in
                                        MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr //out
                                        );

/*****************************************************************************/
//  Description : set image process type
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageProcessType(MMIDC_IMAGE_PROCESS_TYPE_E type);

/*****************************************************************************/
//  Description : get image process type
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_IMAGE_PROCESS_TYPE_E MMIDC_GetImageProcessType(void);
                                                          
/*****************************************************************************/
//  Description : convert dcam data type to img proc data format
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC IMG_PROC_DATA_FORMAT_E MMIDC_ConvertDCamDataTypeToImgProDataFormat(DCAMERA_DATA_TYPE_E data_type
                                                                                             );
                                                                          
/*****************************************************************************/
//  Description : get data size by img proc format
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDataSizeByImgProcFormat(uint32 width,
                                                           uint32 height,
                                                           uint8 which_chn,//from 1
                                                           IMG_PROC_DATA_FORMAT_E data_format
                                                           );

/*****************************************************************************/
//  Description : is photo data
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsPhotoData(void);

/*****************************************************************************/
//  Description : set photo data flag
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoDataFlag(BOOLEAN is_enable);

/*****************************************************************************/
//  Description : get u camera status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_U_CAMERA_STATUS_E MMIDC_GetUCameraStatus(void);

/*****************************************************************************/
//  Description : set u camera status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetUCameraStatus(MMIDC_U_CAMERA_STATUS_E status);

/*****************************************************************************/
//  Description : get u camera enable flag
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetUCameraModuleFlag(void);

/*****************************************************************************/
//  Description : set u camera enable flag
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetUCameraModuleFlag(BOOLEAN enable);

/*****************************************************************************/
//  Description : is u camera doing
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsUCameraDoing(void);

/*****************************************************************************/
//Description: u camera async callback to save photo file
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_UCameraASyncCallbackToSavePhotoFile(void);
                                                                       
/*****************************************************************************/
//Description: u camera sync callback
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_UCameraSyncCallback(IMG_PROC_PROCESS_RTN_T *img_proc_result_ptr, //in
                                                  MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr, //in
                                                  MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr //out
                                                  );                                                                       

/*****************************************************************************/
//  Description : get data size for jpeg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDataSizeForJPG(uint32 width,uint32 height);

/*****************************************************************************/
//  Description : open u camera feature
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenUCameraFeature(MMIDC_IMAGE_PROCESS_TYPE_E type);

/*****************************************************************************/
//  Description : destroy u camera feature
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyUCameraFeature(MMIDC_IMAGE_PROCESS_TYPE_E type);

/*****************************************************************************/
//Description: u camera async callback send msg to app
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_UCameraASyncCallbackSendMsgToApp(IMG_PROC_DATA_T *photo_data_ptr //in
                                                                      );
                                                                      
/*****************************************************************************/
//  Description : set u camera function photo data
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetUCameraPhotoData(BOOLEAN is_enable,//in
                                                 uint8 *buf_ptr, //in
                                                 uint32 buf_size //in
                                                 );  
                                                 
/*****************************************************************************/
//  Description : get u camera function photo data
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetUCameraPhotoData(uint8 **buf_ptr_ptr, //out
                                                    uint32 *buf_size_ptr //out
                                                    );   

/*****************************************************************************/
//  Description : open image process
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenImageProc(void);

/*****************************************************************************/
//  Description : close img proc
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseImageProc(void);

/*****************************************************************************/
//  Description : create u camera status semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateUCameraStatusSemaphorePtr(void);

/*****************************************************************************/
//  Description : delete u camera status semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteUCameraStatusSemaphorePtr(void);

/*****************************************************************************/
//  Description : push u camera status semaphore
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushUCameraStatusSemaphore(void);

/*****************************************************************************/
//  Description : pop u camera status semaphore
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopUCameraStatusSemaphore(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#if defined(MMIDC_F_U_CAMERA)

#endif //_MMIDC_IMAGEPROCESS_H_




