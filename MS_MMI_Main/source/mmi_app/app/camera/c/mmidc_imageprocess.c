/*****************************************************************************
** File Name:      mmidc_imageprocess.c                                      *
** Author:                                                                   *
** Date:           01/26/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to u camera                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012      robert.wang          Creat                                   *
******************************************************************************/

#define _MMIDC_IMAGEPROCESS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_U_CAMERA)
#include "mmi_app_camera_trc.h"

#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_default.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"

#include "mmifmm_export.h"

//this module
#include "mmidc_camera_id.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#ifdef MMIDC_F_N_IN_ONE
#include "mmidc_ninone.h"
#endif
#ifdef MMIDC_F_SMILE_AUTO_PHOTO
#include "mmidc_smileautophoto.h"
#endif

#ifdef MMIDC_F_EFFECT_360
#include "mmidc_effect360.h"
#endif

#include "mmidc_imageprocess.h"
#include "mmidc_export.h"
#include "mmidc_main.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL MMIDC_SETTING_U_CAMERA_INFO_T *s_u_camera_info_ptr = PNULL;
LOCAL BOOLEAN s_u_camera_module_flag = FALSE;

LOCAL MMIDC_U_CAMERA_FUNCTION_ITEM_T s_u_camera_function_table[] =
{
#ifdef MMIDC_F_N_IN_ONE
    {IMG_PROC_NINONE, MMIDC_OpenNInOne, MMIDC_CloseNInOne},
#endif

#ifdef MMIDC_F_SMILE_AUTO_PHOTO
    {IMG_PROC_FACE_DECT, MMIDC_OpenSmileAutoPhoto, MMIDC_CloseSmileAutoPhoto},
#endif

#ifdef MMIDC_F_LOMO
    {IMG_PROC_LOMO, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif

#ifdef MMIDC_F_FISH_EYE
    {IMG_PROC_FISHEYE, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif

#ifdef MMIDC_F_HDR
    {IMG_PROC_HDR, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif

#ifdef MMIDC_F_PENCIL_SKETCH
    {IMG_PROC_BLACK_WHITE_DRAWING, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif

#ifdef MMIDC_F_X_RAY
    {IMG_PROC_XRAY, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif 

#ifdef MMIDC_F_INFRARED
    {IMG_PROC_INFRARED, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif

#ifdef MMIDC_F_EXPOSAL
    {IMG_PROC_EXPOSAL, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif 

#ifdef MMIDC_F_NEGATIVE
    {IMG_PROC_NEGATIVE, MMIDC_OpenEffect360, MMIDC_CloseEffect360},
#endif
};

LOCAL MMIDC_IMAGE_PROCESS_TYPE_E s_u_camera_img_proc_type = 0;
LOCAL SCI_SEMAPHORE_PTR        s_u_camera_status_semaphore_ptr = PNULL;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : get u camera info ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_SETTING_U_CAMERA_INFO_T *GetUCameraInfoPtr(void);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get u camera info ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_SETTING_U_CAMERA_INFO_T *GetUCameraInfoPtr(void)
{
    return s_u_camera_info_ptr;
}

/*****************************************************************************/
//  Description : open image process
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImageProcessDirectory(wchar **path_ptr,//out
                                               uint32 *path_len //out
                                              )
{
    BOOLEAN ret = FALSE;
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    wchar temp_dir_name[] = L"temp";
    wchar *temp_dir_ptr = PNULL;
    uint32 temp_dir_len = 0;
    wchar *save_path_ptr = MMIDC_GetPhotoSavePath();

    MMIFILE_ERROR_E file_err = 0;

    if ((PNULL != info_ptr)
        &&(PNULL != save_path_ptr)
       )
    {
        temp_dir_ptr = info_ptr->temp_dir;
        temp_dir_len = MMIFILE_FULL_PATH_MAX_LEN;
        temp_dir_len = MMIAPIDC_CombinePathName(temp_dir_ptr, temp_dir_len, \
                             save_path_ptr, temp_dir_name);

        info_ptr->temp_dir_len = temp_dir_len;
            
        *path_ptr = temp_dir_ptr;
        *path_len = temp_dir_len;
            
        file_err = MMIAPIFMM_CreateDirectory(temp_dir_ptr);
        if ((SFS_ERROR_NONE == file_err)
            || (SFS_ERROR_HAS_EXIST == file_err)
           )
        {
            ret = TRUE;
        }
    }

    //SCI_TRACE_LOW:"[MMDIC] GetImageProcessDirectory ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_172_112_2_18_2_9_12_320,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
//  Description : open image process
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenImageProc(void)
{
    BOOLEAN ret = FALSE;  
    uint32 img_proc_ret = 0;
    IMG_PROC_OPEN_T img_proc_open = {0};

    //ret = GetImageProcessDirectory(&img_proc_open.temp_directory,
    GetImageProcessDirectory(&img_proc_open.temp_directory,
                            &img_proc_open.temp_directory_len
                            );

    //if (ret)
    {
        img_proc_ret = IMGPROC_Open(&img_proc_open);
        if(IMG_PROC_SUCCESS == img_proc_ret)
        {
            ret = TRUE;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] OpenImageProc ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_201_112_2_18_2_9_13_321,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
//  Description : open u camera
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_U_CAMERA_ERR_E MMIDC_OpenUCamera(void)
{
    BOOLEAN ret = FALSE;
    uint32 buf_size = 0;
    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_UDISK;
    MMIDC_U_CAMERA_ERR_E err = UCAMERA_ERR_MAX;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);
    
    do
    {
        fmm_dev = MMIAPIFMM_GetFirstValidDevice();
        if (fmm_dev >= MMI_DEVICE_NUM)
        {
            err = UCAMERA_ERR_NO_SD_CARD;
            break;
        }
        buf_size = sizeof(*s_u_camera_info_ptr);
        if (PNULL == s_u_camera_info_ptr)
        {
            s_u_camera_info_ptr = SCI_ALLOCA(buf_size);
            if (PNULL == s_u_camera_info_ptr)
            {
                break;
            }
        }
        if (PNULL != s_u_camera_info_ptr)
        {
            SCI_MEMSET(s_u_camera_info_ptr, 0, buf_size);
        }

        ret = MMIDC_OpenImageProc();
        if (!ret)
        {
            break;
        }
        
        s_u_camera_info_ptr->is_enable = TRUE;
        ret = TRUE;
    }while(0);

    
    if(ret)
    {
        MMIDC_PushPhotoSize(MMIDC_GetPhotoSize());
    
        //init param
        MMIDC_SetUCameraStatus(UCAMERA_STATUS_START);
        MMIDC_SetPhotoDataFlag(TRUE);

        if ( (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type) 
            && (LCD_ANGLE_0 == logic_angle)
            )
        {
            MMIAPIDC_SetPhotoSize(PHOTO_SIZE_480X640);
        }
        else
        {
            MMIAPIDC_SetPhotoSize(MMIDC_U_CAMERA_DEFAULT_PHOTO_SIZE);
        }
    }
    else
    {
        MMIDC_CloseUCamera();
    }

    if(ret)
    {
        err = UCAMERA_ERR_NONE;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_OpenImgProc ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_270_112_2_18_2_9_13_322,(uint8*)"d", ret);
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_OpenImgProc err=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_271_112_2_18_2_9_13_323,(uint8*)"d", err);
    
    return err;    
}

/*****************************************************************************/
//  Description : wait for img proc done
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_WaitForImgProcDone(void)
{
    uint32 count = 0;

    
    while(UCAMERA_STATUS_PROCESS == MMIDC_GetUCameraStatus())
    {
        SCI_Sleep(100);
        count++;
        if(count > 50)
        {
            break;
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] MMIDC_WaitForImgProcDone count=%d", count);
}

/*****************************************************************************/
//  Description : close img proc
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseImageProc(void)
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_CloseImageProc");
    
	
    if (MMIDC_IsUCameraEnable())
    {
        MMIDC_WaitForImgProcDone();

        IMGPROC_Close(PNULL);
    }
}

/*****************************************************************************/
//  Description : close u camera
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseUCamera(void)
{

    MMIDC_PHOTO_SIZE_E photo_size = 0;


    MMIDC_StopPreviewForSwitchSize();

    MMIDC_PushUCameraStatusSemaphore();
    MMIDC_PopUCameraStatusSemaphore();
        
    if (MMIDC_IsUCameraEnable())
    {
        MMIDC_WaitForImgProcDone();
    }

    //u camera enable to pop photo size
    if (MMIDC_IsUCameraEnable())
    {
        photo_size = MMIDC_PopPhotoSize();

        //MMIDC_StopPreviewForSwitchSize();
        MMIDC_SetPhotoSizeParameter(photo_size);
        //MMIDC_StartPreviewForSwitchSize();
    }

#if defined(MMIDC_F_N_IN_ONE)
    if (MMIDC_IsNInOneEnable())
    {
        MMIDC_CloseNInOne();
    }
#endif

#ifdef MMIDC_F_SMILE_AUTO_PHOTO
    if (MMIDC_IsSmileAutoPhotoEnable())
    {
        MMIDC_CloseSmileAutoPhoto();
    }
#endif

#ifdef MMIDC_F_EFFECT_360
    if (MMIDC_IsEffect360Enable())
    {
        MMIDC_CloseEffect360();
    }
#endif

    
    MMIDC_CloseImageProc();
    
    if (PNULL != s_u_camera_info_ptr)
    {
        MMIFILE_DeleteDirectory(s_u_camera_info_ptr->temp_dir);
        
        SCI_FREE(s_u_camera_info_ptr);
    }


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CloseUCamera"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_328_112_2_18_2_9_13_324,(uint8*)"");
}

/*****************************************************************************/
//  Description : u camera is enable or not
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsUCameraEnable(void)
{
    BOOLEAN is_enable = FALSE;
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    if (PNULL != info_ptr)
    {
        is_enable = info_ptr->is_enable;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsUCameraEnable is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_346_112_2_18_2_9_13_325,(uint8*)"d",is_enable);
                 
    return is_enable;
}    

/*****************************************************************************/
//  Description : trace param to debug
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void trace_param(IMG_PROC_PROCESS_T *param_ptr)
{
#ifdef MMIDC_DEBUG_TRACE
    IMG_PROC_DATA_T_PTR src_img_ptr = PNULL;
    IMG_PROC_DST_T *proc_img_ptr = PNULL;
    IMG_PROC_DST_T *before_0_ptr = PNULL;


    //SCI_TRACE_LOW:"trace_param cmd=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_363_112_2_18_2_9_13_326,(uint8*)"d",param_ptr->cmd);

    src_img_ptr = param_ptr->src_img_ptr;
    //SCI_TRACE_LOW:"trace_param src =0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_366_112_2_18_2_9_13_327,(uint8*)"d",src_img_ptr);
    //SCI_TRACE_LOW:"trace_param src format=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_367_112_2_18_2_9_13_328,(uint8*)"d",src_img_ptr->format);
    //SCI_TRACE_LOW:"trace_param src format=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_369_112_2_18_2_9_13_329,(uint8*)"d",src_img_ptr->size.w, src_img_ptr->size.h);

    //SCI_TRACE_LOW:"trace_param src chn0=0x%X,chn0_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_372_112_2_18_2_9_13_330,(uint8*)"dd",src_img_ptr->addr.chn0, src_img_ptr->addr.chn0_len);
    //SCI_TRACE_LOW:"trace_param src chn1=0x%X,chn1_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_374_112_2_18_2_9_13_331,(uint8*)"dd",src_img_ptr->addr.chn1, src_img_ptr->addr.chn1_len);

    //SCI_TRACE_LOW:"trace_param proc_mode=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_376_112_2_18_2_9_13_332,(uint8*)"d",param_ptr->proc_mode);

    //proc img
    proc_img_ptr = &param_ptr->proc_img;
    //SCI_TRACE_LOW:"trace_param proc_img w=%d, h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_381_112_2_18_2_9_13_333,(uint8*)"dd",proc_img_ptr->rect.w, proc_img_ptr->rect.h);
          
    //SCI_TRACE_LOW:"trace_param proc_img format=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_384_112_2_18_2_9_13_334,(uint8*)"d",proc_img_ptr->data.format);

    //SCI_TRACE_LOW:"trace_param proc_img ctrl_callback=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_387_112_2_18_2_9_13_335,(uint8*)"d",proc_img_ptr->ctrl_callback);

    //SCI_TRACE_LOW:"trace_param proc_img size w=%d h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_390_112_2_18_2_9_13_336,(uint8*)"dd",proc_img_ptr->data.size.w, proc_img_ptr->data.size.h);

    //SCI_TRACE_LOW:"trace_param proc_img chn0 =0x%X chn0_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_393_112_2_18_2_9_13_337,(uint8*)"dd",proc_img_ptr->data.addr.chn0, proc_img_ptr->data.addr.chn0_len);
              
    //SCI_TRACE_LOW:"trace_param proc_img chn1 =0x%X chn1_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_396_112_2_18_2_9_13_338,(uint8*)"dd",proc_img_ptr->data.addr.chn1, proc_img_ptr->data.addr.chn1_len);

    //before 0
    before_0_ptr = &param_ptr->view_before[0];
    //SCI_TRACE_LOW:"trace_param before 0 w=%d, h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_401_112_2_18_2_9_13_339,(uint8*)"dd",before_0_ptr->rect.w, before_0_ptr->rect.h);
          
    //SCI_TRACE_LOW:"trace_param before 0 format=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_404_112_2_18_2_9_13_340,(uint8*)"d",before_0_ptr->data.format);

    //SCI_TRACE_LOW:"trace_param before 0 ctrl_callback=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_407_112_2_18_2_9_13_341,(uint8*)"d",before_0_ptr->ctrl_callback);

    //SCI_TRACE_LOW:"trace_param before 0 size w=%d h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_410_112_2_18_2_9_13_342,(uint8*)"dd",before_0_ptr->data.size.w, before_0_ptr->data.size.h);

    //SCI_TRACE_LOW:"trace_param before 0 chn0 =0x%X chn0_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_413_112_2_18_2_9_13_343,(uint8*)"dd",before_0_ptr->data.addr.chn0, before_0_ptr->data.addr.chn0_len);
              
    //SCI_TRACE_LOW:"trace_param before 0 chn1 =0x%X chn1_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_416_112_2_18_2_9_13_344,(uint8*)"dd",before_0_ptr->data.addr.chn1, before_0_ptr->data.addr.chn1_len);
    
#endif
}

/*****************************************************************************/
//  Description : get img pro source data, and set src info, dst info data size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL int32 GetImgProcSrcData(MMIDC_IMAGE_PROCESS_PARAM_T *mmi_param_ptr //in and out
                                    )
{
    int32 ret = -1;
    
    IMG_PROC_DATA_T *src_data_ptr = PNULL;
    DC_IMG_SIZE_T src_data_size = {0};
    
    
    uint16 img_width = 0;
    uint16 img_height = 0;
    
    MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr = PNULL;
    MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr = PNULL;

    if (PNULL == mmi_param_ptr)
    {
        return ret;
    }

    src_data_ptr = &mmi_param_ptr->src_data;
    src_info_ptr = mmi_param_ptr->src_info_ptr;
    dst_info_ptr = mmi_param_ptr->dst_info_ptr;
    
    if ((PNULL == src_info_ptr)
        || (PNULL == dst_info_ptr)
        || (PNULL == src_data_ptr)
       )/*lint !e774*/
    {
        return ret; 
    }

  
    //get source data
    src_data_size = src_info_ptr->data_size;
    src_data_ptr->format = MMIDC_ConvertDCamDataTypeToImgProDataFormat(src_info_ptr->data_type);

    //src chn0
    src_data_ptr->addr.chn0 = src_info_ptr->data_addr.chn1_ptr;
    if (PNULL != src_data_ptr->addr.chn0)
    {
        src_data_ptr->addr.chn0_len = MMIDC_GetDataSizeByImgProcFormat(src_data_size.w,
                                                                 src_data_size.h, 
                                                                 1, 
                                                                 src_data_ptr->format
                                                                 );
        src_info_ptr->data_addr.chn1_size = src_data_ptr->addr.chn0_len;
        dst_info_ptr->data_addr.chn1_size = src_data_ptr->addr.chn0_len;
    }
    
    //src chn1
    src_data_ptr->addr.chn1 = src_info_ptr->data_addr.chn2_ptr;
    if (PNULL != src_data_ptr->addr.chn1)
    {
        src_data_ptr->addr.chn1_len = MMIDC_GetDataSizeByImgProcFormat(src_data_size.w,
                                                                 src_data_size.h, 
                                                                 2, 
                                                                 src_data_ptr->format
                                                                 );
        src_info_ptr->data_addr.chn2_size = src_data_ptr->addr.chn1_len;
        dst_info_ptr->data_addr.chn2_size = src_data_ptr->addr.chn1_len;
       
    }

    //src chn2
    src_data_ptr->addr.chn2 = src_info_ptr->data_addr.chn3_ptr;
    if (PNULL != src_data_ptr->addr.chn2)
    {
        src_data_ptr->addr.chn2_len = MMIDC_GetDataSizeByImgProcFormat(src_data_size.w,
                                                                 src_data_size.h, 
                                                                 3, 
                                                                 src_data_ptr->format
                                                                 );
        src_info_ptr->data_addr.chn3_size = src_data_ptr->addr.chn2_len;
        dst_info_ptr->data_addr.chn3_size = src_data_ptr->addr.chn2_len;       
    }
    
    src_data_ptr->name_ptr = src_info_ptr->file_name_ptr;

    if (PNULL != src_data_ptr->name_ptr)
    {
        ;//get image width and height by file
    }
    else
    {
        img_width = src_data_size.w;
        img_height = src_data_size.h;
    }
    
    src_data_ptr->size.w = img_width;
    src_data_ptr->size.h = img_height;


    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess src w=%d,h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_519_112_2_18_2_9_13_345,(uint8*)"dd",src_data_ptr->size.w , src_data_ptr->size.h);

#ifdef MMIDC_DEBUG_TRACE
    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess src chn1_ptr=0x%X, chn1_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_523_112_2_18_2_9_13_346,(uint8*)"dd",src_data_ptr->addr.chn0, src_data_ptr->addr.chn0_len);
                 
    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess src chn2_ptr=0x%X,chn2_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_526_112_2_18_2_9_13_347,(uint8*)"dd",src_data_ptr->addr.chn1, src_data_ptr->addr.chn1_len);
                 
    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess src chn3_ptr=0x%X,chn3_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_529_112_2_18_2_9_13_348,(uint8*)"dd",src_data_ptr->addr.chn2, src_data_ptr->addr.chn2_len);

#endif

    ret = 0;

    return ret;
}

/*****************************************************************************/
//  Description : process image by command
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL int32 DoImageProcess(MMIDC_IMAGE_PROCESS_PARAM_T *mmi_param_ptr //in and out
                                )
{
    int32 img_proc_ret = -1;
    
    IMG_PROC_PROCESS_T *proc_param_ptr = PNULL;
    IMG_PROC_PROCESS_RTN_T rtn_param = {0};

    
    IMGPROC_HANDLE handle = 0;
    
   
    MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr = PNULL;
    MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr = PNULL;


    uint32 t1 = 0;
    uint32 t2 = 0;


    if (PNULL == mmi_param_ptr)
    {
        return img_proc_ret;
    }

    src_info_ptr = mmi_param_ptr->src_info_ptr;
    dst_info_ptr = mmi_param_ptr->dst_info_ptr;
    proc_param_ptr = &mmi_param_ptr->img_proc_param;

    if ((PNULL == src_info_ptr)
        || (PNULL == dst_info_ptr)
        || (PNULL == proc_param_ptr)
       )/*lint !e774*/
    {
        return img_proc_ret; 
    }


    
    proc_param_ptr->src_img_num = 1;
    proc_param_ptr->src_img_ptr =(IMG_PROC_DATA_T_PTR)&mmi_param_ptr->src_data;

    handle = mmi_param_ptr->handle;

    trace_param(proc_param_ptr);

    t1 = SCI_GetTickCount();
    
    MMIDC_SetUCameraStatus(UCAMERA_STATUS_PROCESS);
    
    img_proc_ret = IMGPROC_DoProcess(handle, proc_param_ptr, &rtn_param);



    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess img_proc_ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_597_112_2_18_2_9_13_349,(uint8*)"d",img_proc_ret);

    t2 = SCI_GetTickCount();
    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess t2=%d, t2-t1=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_601_112_2_18_2_9_13_350,(uint8*)"dd",t2, t2 - t1);

#ifdef MMIDC_DEBUG_TRACE
    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess rtn chn0=0x%X,chn0_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_605_112_2_18_2_9_13_351,(uint8*)"dd",rtn_param.data_rtn.proc_img.addr.chn0, rtn_param.data_rtn.proc_img.addr.chn0_len);
                 
    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess rtn chn1=0x%X,chn1_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_608_112_2_18_2_9_13_352,(uint8*)"dd",rtn_param.data_rtn.proc_img.addr.chn1, rtn_param.data_rtn.proc_img.addr.chn1_len);


    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess dst chn1=0x%X,chn1_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_612_112_2_18_2_9_13_353,(uint8*)"dd",dst_info_ptr->data_addr.chn1_ptr, dst_info_ptr->data_addr.chn1_size);
                 
    //SCI_TRACE_LOW:"[MMIDC] DoImageProcess dst chn2=0x%X,chn2_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_615_112_2_18_2_9_13_354,(uint8*)"dd",dst_info_ptr->data_addr.chn2_ptr, dst_info_ptr->data_addr.chn2_size);

#endif


    if (IMG_PROC_SUCCESS == img_proc_ret)
    {
        if (PNULL != mmi_param_ptr->sync_callback)
        {
            mmi_param_ptr->sync_callback(&rtn_param, src_info_ptr, dst_info_ptr);
        }
    }
    else
    {
        MMIDC_SetUCameraStatus(UCAMERA_STATUS_START);
    }

    return img_proc_ret;
}

/*****************************************************************************/
//  Description : process image by command
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ProcessImage(MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr, //in
                                        MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr //out
                                        )
{
    int32 img_proc_ret = -1;    
    
    MMIDC_IMAGE_PROCESS_TYPE_E cmd = 0;    
    MMIDC_IMAGE_PROCESS_PARAM_T img_process_param = {0};
    
    BOOLEAN is_skip_image_process = FALSE;


    if ((PNULL == src_info_ptr)
        || (PNULL == dst_info_ptr)
       )
    {
        return img_proc_ret;
    }

    //check status   
    if (MMIDC_IsUCameraDoing())
    {
        return img_proc_ret;
    }

    img_process_param.src_info_ptr = src_info_ptr;
    img_process_param.dst_info_ptr = dst_info_ptr;


    //get source data, and set src info, dst info data size
    img_proc_ret = GetImgProcSrcData(&img_process_param);
    if (0 != img_proc_ret)
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_ProcessImage get src data img_proc_ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_673_112_2_18_2_9_14_355,(uint8*)"d",img_proc_ret);
                      
        return img_proc_ret;
    }



    cmd = MMIDC_GetImageProcessType();
    img_process_param.img_proc_param.cmd = cmd;
    
    switch(cmd)
    {
#ifdef MMIDC_F_N_IN_ONE    
    case IMG_PROC_NINONE:
        MMIDC_GetNInOneImageProcessParam(&img_process_param);
        break;
#endif        
#ifdef MMIDC_F_SMILE_AUTO_PHOTO        
    case IMG_PROC_FACE_DECT:
        MMIDC_GetSmileAutoPhotoImageProcessParam(&img_process_param);
        break;
#endif
#ifdef MMIDC_F_EFFECT_360
    case IMG_PROC_HDR:
    case IMG_PROC_FISHEYE:
    case IMG_PROC_LOMO:
    case IMG_PROC_BLACK_WHITE_DRAWING:
    case IMG_PROC_XRAY:
    case IMG_PROC_INFRARED:
    case IMG_PROC_EXPOSAL:
    case IMG_PROC_NEGATIVE:
        MMIDC_GetEffect360ImageProcessParam(&img_process_param);
        break;
#endif
    default:
        break;
    }

    if (PNULL != img_process_param.dst_info_ptr)
    {
        is_skip_image_process = img_process_param.dst_info_ptr->is_skip_image_process;
    }
    
    if (is_skip_image_process)
    {
        img_proc_ret = 0;
    }
    else
    {
        img_proc_ret = DoImageProcess(&img_process_param);
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ProcessImage is_skip_image_process=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_726_112_2_18_2_9_14_356,(uint8*)"d",is_skip_image_process);
                  
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ProcessImage img_proc_ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_729_112_2_18_2_9_14_357,(uint8*)"d",img_proc_ret);
    
    //SCI_Sleep(200);
    //SCI_ASSERT(0);
    
    return img_proc_ret;
}

/*****************************************************************************/
//  Description : set image process type
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetImageProcessType(MMIDC_IMAGE_PROCESS_TYPE_E type)
{
    s_u_camera_img_proc_type = type;

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetImageProcessType type=%d",type);
}

/*****************************************************************************/
//  Description : get image process type
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_IMAGE_PROCESS_TYPE_E MMIDC_GetImageProcessType(void)
{
    MMIDC_IMAGE_PROCESS_TYPE_E type = 0;

    type = s_u_camera_img_proc_type;

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetImageProcessType type=%d",type);

    return type;
}

/*****************************************************************************/
//  Description : convert dcam data type to img proc data format
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC IMG_PROC_DATA_FORMAT_E MMIDC_ConvertDCamDataTypeToImgProDataFormat(DCAMERA_DATA_TYPE_E data_type
                                                                                           )
{
    IMG_PROC_DATA_FORMAT_E data_format = 0;


    switch(data_type)
    {
    case DCAMERA_DATA_TYPE_YUV422:
        data_format = IMG_PROC_YUV422;
        break;
    case DCAMERA_DATA_TYPE_YUV420:
        data_format = IMG_PROC_YUV420;
        break;        
    case DCAMERA_DATA_TYPE_JPEG:
        data_format = IMG_PROC_JPG;
        break;
    default:
        data_format = IMG_PROC_RGB565;
        break;
    }

    return data_format;
}

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
                                                           )
{
    uint32 data_size = 0;
    
    switch(data_format)
    {
    case IMG_PROC_YUV400:
    case IMG_PROC_YUV422:
        data_size = width * height;
        break;

    case IMG_PROC_YUV420:
        switch(which_chn)
        {
        case 2:
            data_size =(width * height)>>0x01;
            break;
        case 1:
        default:
            data_size = width * height;
            break;
        }
        break;


    case IMG_PROC_YUV420_3FRAME:
        switch(which_chn)
        {
        case 1:
            data_size = width * height;
            break;
        case 2:
        case 3:
        default:
            data_size =(width * height)>>0x02;
            data_size =(width * height)>>0x02;
        }
        break;
    case IMG_PROC_YUV422_3FRAME:
        switch(which_chn)
        {
        case 1:
            data_size = width * height;
            break;
        case 2:
        case 3:
        default:
            data_size =(width * height)>>0x01;
            data_size =(width * height)>>0x01;
        }
        break;
    case IMG_PROC_RGB888:
    case IMG_PROC_ARGB888:
        data_size =(width * height)<<0x02;
        break;
    case IMG_PROC_RGB565:
    default:
        data_size =(width * height)<<0x01;
        break;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDataSizeByImgProcFormat data_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_873_112_2_18_2_9_14_360,(uint8*)"d",data_size);
    
    return data_size;
}

/*****************************************************************************/
//  Description : is photo data
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsPhotoData(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    if (PNULL != info_ptr)
    {
        ret = info_ptr->is_photo_data;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsPhotoData ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_893_112_2_18_2_9_14_361,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Description : set photo data flag
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoDataFlag(BOOLEAN is_enable)
{
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    if (PNULL != info_ptr)
    {
        info_ptr->is_photo_data = is_enable;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetPhotoDataFlag is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_911_112_2_18_2_9_14_362,(uint8*)"d",is_enable);
}

/*****************************************************************************/
//  Description : get u camera status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_U_CAMERA_STATUS_E MMIDC_GetUCameraStatus(void)
{
    MMIDC_U_CAMERA_STATUS_E status = 0;
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    if (PNULL != info_ptr)
    {
        status = info_ptr->status;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetUCameraStatus status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_929_112_2_18_2_9_14_363,(uint8*)"d",status);
    return status;
}

/*****************************************************************************/
//  Description : set u camera status
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetUCameraStatus(MMIDC_U_CAMERA_STATUS_E status)
{
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    if (PNULL != info_ptr)
    {
        MMIDC_PushUCameraStatusSemaphore();
        info_ptr->status = status;
        MMIDC_PopUCameraStatusSemaphore();
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetUCameraStatus status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_947_112_2_18_2_9_14_364,(uint8*)"d",status);
}

/*****************************************************************************/
//  Description : is u camera doing
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsUCameraDoing(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_U_CAMERA_STATUS_E status = 0;
    

    status = MMIDC_GetUCameraStatus();
    if (UCAMERA_STATUS_START != status)
    {
        ret = TRUE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsUCameraDoing ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_967_112_2_18_2_9_14_365,(uint8*)"d",ret);
                 
    return ret;
}

/*****************************************************************************/
//Description: u camera sync callback
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_UCameraSyncCallback(IMG_PROC_PROCESS_RTN_T *img_proc_result_ptr, //in
                                                  MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr, //in
                                                  MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr //out
                                                  )
{
    int32 ret = -1;
    //IMG_PROC_DATA_T *proc_data_ptr = PNULL;

    if ((PNULL == img_proc_result_ptr)
        || (PNULL == src_info_ptr)
        || (PNULL == dst_info_ptr)
        )
    {
        return ret;
    }

    //proc data
    //proc_data_ptr = &img_proc_result_ptr->data_rtn.proc_img;


    MMIDC_SetUCameraStatus(UCAMERA_STATUS_START);

    ret = 0;

    return ret;
}


/*****************************************************************************/
//Description: u camera async callback to save photo file
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_UCameraASyncCallbackToSavePhotoFile(void)
{
    int32 ret = -1;
    BOOLEAN is_jpg = FALSE;
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();
    IMG_PROC_DATA_T *photo_data_ptr = PNULL;

    if (PNULL != info_ptr)
    {
        photo_data_ptr = &info_ptr->photo_img_proc_data;

        if (PNULL != photo_data_ptr) /*lint !e774*/
        {
            if (IMG_PROC_JPG == photo_data_ptr->format)
            {
                is_jpg = TRUE;
                MMIDC_SetPhotoDataFlag(TRUE);

                MMIDC_SetUCameraPhotoData(TRUE,
                                          photo_data_ptr->addr.chn0,
                                          photo_data_ptr->addr.chn0_len
                                          );

                ret = MMIDC_SaveDataToFile(0, 
                                    photo_data_ptr->addr.chn0,
                                    photo_data_ptr->addr.chn0_len
                                    );

            }

            //SCI_TRACE_LOW:"[MMIDC] MMIDC_UCameraASyncCallbackToSavePhotoFile format=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1037_112_2_18_2_9_14_366,(uint8*)"d",photo_data_ptr->format);

            //SCI_TRACE_LOW:"[MMIDC] MMIDC_UCameraASyncCallbackToSavePhotoFile chn0=0x%X,chn0_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1040_112_2_18_2_9_14_367,(uint8*)"dd",photo_data_ptr->addr.chn0, photo_data_ptr->addr.chn0_len);
        }
    }

    MMIDC_SetUCameraStatus(UCAMERA_STATUS_START);

    //capture fail to send msg
    if (0 != ret && !is_jpg)
    {
        MMIDC_SendCaptureFailMsg(ret, PNULL, 0);
    }


    return ret;
}

/*****************************************************************************/
//Description: u camera async callback send msg to app
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_UCameraASyncCallbackSendMsgToApp(IMG_PROC_DATA_T *photo_data_ptr //in
                                                                      )
{
    int32 ret = -1;
    MMIDC_CMD_PARAM_T cmd_param = {0};
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    if ((PNULL != photo_data_ptr)
        && (PNULL != info_ptr)
       )
    {
        info_ptr->photo_img_proc_data = *photo_data_ptr;

        //SCI_TRACE_LOW:"[MMIDC] MMIDC_UCameraASyncCallbackSendMsgToApp format=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1074_112_2_18_2_9_14_368,(uint8*)"d",photo_data_ptr->format);

        //SCI_TRACE_LOW:"[MMIDC] MMIDC_UCameraASyncCallbackSendMsgToApp chn0=0x%X,chn0_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1077_112_2_18_2_9_14_369,(uint8*)"dd",photo_data_ptr->addr.chn0, photo_data_ptr->addr.chn0_len);


        ret = 0;
        cmd_param.cmd = MMIDC_CMD_IMAGE_PROCESS_DONE;
        //MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_MMI_CMD, &cmd, sizeof(cmd));
        MMIDC_ProcessMsgCmd(cmd_param);
    }
    

    return ret;
}


/*****************************************************************************/
//  Description : set u camera enable flag
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetUCameraModuleFlag(void)
{
    return s_u_camera_module_flag;
}

/*****************************************************************************/
//  Description : set u camera enable flag
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetUCameraModuleFlag(BOOLEAN enable)
{
    s_u_camera_module_flag = enable;
}

/*****************************************************************************/
//  Description : get data size for jpeg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDataSizeForJPG(uint32 width,uint32 height)
{
    uint32 data_size = 0;
    uint8 plane = 2;

    data_size = width * height * plane / 2;
//    data_size = width * height / 3;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDataSizeForJPG width=%d, height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1120_112_2_18_2_9_14_370,(uint8*)"dd",width, height);
                 
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetDataSizeForJPG data_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1123_112_2_18_2_9_14_371,(uint8*)"d",data_size);

    return data_size;    
}

/*****************************************************************************/
//  Description : open u camera feature
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenUCameraFeature(MMIDC_IMAGE_PROCESS_TYPE_E type)
{
    BOOLEAN ret = FALSE;
    int16 i = 0;
    int16 total = 0;

    MMIDC_U_CAMERA_FUNCTION_ITEM_T func_item = {0};
    

    total = ARR_SIZE(s_u_camera_function_table);

    for (i = 0; i < total; i++)
    {
        func_item = s_u_camera_function_table[i];
        if (type == func_item.type)
        {
            if (PNULL != func_item.open_func_ptr)
            {
                ret = func_item.open_func_ptr();
            }
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_OpenUCameraCurTypeFunction type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1157_112_2_18_2_9_14_372,(uint8*)"d",type);
                 
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_OpenUCameraCurTypeFunction ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1160_112_2_18_2_9_14_373,(uint8*)"d",ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : destroy u camera feature
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyUCameraFeature(MMIDC_IMAGE_PROCESS_TYPE_E type)
{
    int16 i = 0;
    int16 total = 0;

    MMIDC_U_CAMERA_FUNCTION_ITEM_T func_item = {0};
    

    total = ARR_SIZE(s_u_camera_function_table);

    for (i = 0; i < total; i++)
    {
        func_item = s_u_camera_function_table[i];
        if (type == func_item.type)
        {
            if (PNULL != func_item.destroy_func_ptr)
            {
                func_item.destroy_func_ptr();
            }
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DestroyUCameraCurTypeFunction type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1193_112_2_18_2_9_15_374,(uint8*)"d",type);
}

/*****************************************************************************/
//  Description : set u camera function photo data
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetUCameraPhotoData(BOOLEAN is_enable,//in
                                                 uint8 *buf_ptr, //in
                                                 uint32 buf_size //in
                                                 )
{
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();

    if (PNULL != info_ptr)//not check buf_ptr, buf_size, permit pnull
    {
        info_ptr->photo_data.buffer_ptr = buf_ptr;
        info_ptr->photo_data.buffer_size = buf_size;
        info_ptr->photo_data.is_enable = is_enable;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetUCameraPhotoData buf_ptr=0x%X,buf_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1219_112_2_18_2_9_15_375,(uint8*)"dd",buf_ptr, buf_size);
                  
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetUCameraPhotoData is_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1222_112_2_18_2_9_15_376,(uint8*)"d",is_enable);
}

/*****************************************************************************/
//  Description : get u camera function photo data
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetUCameraPhotoData(uint8 **buf_ptr_ptr, //out
                                                    uint32 *buf_size_ptr //out
                                                    )
{
    MMIDC_SETTING_U_CAMERA_INFO_T *info_ptr = GetUCameraInfoPtr();
    BOOLEAN is_enable = FALSE;

    if ((PNULL != buf_ptr_ptr)
        && (PNULL != buf_size_ptr)
        && (PNULL != info_ptr)
       )
    {
        is_enable = info_ptr->photo_data.is_enable;
        if (is_enable)
        {
            *buf_ptr_ptr = info_ptr->photo_data.buffer_ptr;
            *buf_size_ptr = info_ptr->photo_data.buffer_size;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetUCameraPhotoData is_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_IMAGEPROCESS_1250_112_2_18_2_9_15_377,(uint8*)"d",is_enable);

    return is_enable;                  
}

/*****************************************************************************/
//  Description : get u camera status semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC SCI_SEMAPHORE_PTR MMIDC_GetUCameraStatusSemaphorePtr(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = PNULL;

    
    semaphore_ptr = s_u_camera_status_semaphore_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetUCameraStatusSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);

    return semaphore_ptr;                 
}

/*****************************************************************************/
//  Description : set u camera status semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetUCameraStatusSemaphorePtr(SCI_SEMAPHORE_PTR semaphore_ptr)
{
    s_u_camera_status_semaphore_ptr = semaphore_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetUCameraStatusSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);             
}

/*****************************************************************************/
//  Description : create u camera status semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateUCameraStatusSemaphorePtr(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = s_u_camera_status_semaphore_ptr;

    if (PNULL == semaphore_ptr)
    {
        semaphore_ptr = SCI_CreateSemaphore("ucamera status semaphore", 1);
    }
    
    MMIDC_SetUCameraStatusSemaphorePtr(semaphore_ptr);

    SCI_TRACE_LOW("[MMIDC] MMIDC_CreateUCameraStatusSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);
}

/*****************************************************************************/
//  Description : delete u camera status semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteUCameraStatusSemaphorePtr(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = PNULL;


    semaphore_ptr = MMIDC_GetUCameraStatusSemaphorePtr();
    if (PNULL != semaphore_ptr)
    {
        MMIDC_PushUCameraStatusSemaphore();
        MMIDC_PopUCameraStatusSemaphore();
       
       SCI_DeleteSemaphore(semaphore_ptr);
       MMIDC_SetUCameraStatusSemaphorePtr(PNULL);
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_DeleteUCameraStatusSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);             
}

/*****************************************************************************/
//  Description : push u camera status semaphore
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushUCameraStatusSemaphore(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = MMIDC_GetUCameraStatusSemaphorePtr();


    SCI_TRACE_LOW("[MMIDC] MMIDC_PushUCameraStatusSemaphore semaphore_ptr=%d",\
                 semaphore_ptr);

    if (PNULL != semaphore_ptr)
    {
        SCI_GetSemaphore(semaphore_ptr, SCI_WAIT_FOREVER);
    }
}

/*****************************************************************************/
//  Description : pop u camera status semaphore
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopUCameraStatusSemaphore(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = MMIDC_GetUCameraStatusSemaphorePtr();


    SCI_TRACE_LOW("[MMIDC] MMIDC_PopUCameraStatusSemaphore semaphore_ptr=%d",\
                 semaphore_ptr);

    if (PNULL != semaphore_ptr)
    {
        SCI_PutSemaphore(semaphore_ptr);
    }
}
#endif //#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_U_CAMERA)

