/*****************************************************************************
** File Name:      mmidc_srv.c                                               *
** Author:                                                                   *
** Date:           06/21/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera work mode                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2012       chunyou             Creat                                   *
******************************************************************************/

#define _MMIDC_WORKMODE_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_camera_trc.h"
#if defined(CAMERA_SUPPORT)

#ifdef WIN32 
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_default.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"

//this module
#include "mmidc_main.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmidc_export.h"
#include "mmidc_workmode.h"
#include "mmidc_option.h"
#if defined(MMIDC_F_U_CAMERA)
#include "mmidc_imageprocess.h"
#endif
#include "mmidc_srv.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL MMIDCSRV_INFO_T s_mmidcsrv_info = {0};
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
// 	Description : init mmidcsrv info
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCSRV_InitInfo(void)
{
    SCI_MEMSET(&s_mmidcsrv_info, 0 ,sizeof(s_mmidcsrv_info));
}

/*****************************************************************************/
// 	Description : get mmidcsrv info ptr
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL MMIDCSRV_INFO_T *GetMMIDCSRVInfoPtr(void)
{
    return &s_mmidcsrv_info;
}

/*****************************************************************************/
// 	Description : get mmidcsrv dc mem cfg
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_MEM_CFG_E MMIDCSRV_GetDCMemCfg(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    MMIDCSRV_MEM_CFG_E mem_cfg = 0;

    
    if (PNULL != info_ptr)
    {
        mem_cfg = info_ptr->work_param.dc.mem_cfg;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_GetDCMemCfg mem_cfg=%d",
                mem_cfg);
                
    return mem_cfg;
}

/*****************************************************************************/
// 	Description : get mmidcsrv dv mem cfg
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_MEM_CFG_E MMIDCSRV_GetDVMemCfg(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    MMIDCSRV_MEM_CFG_E mem_cfg = 0;

    
    if (PNULL != info_ptr)
    {
        mem_cfg = info_ptr->work_param.dv.mem_cfg;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_GetDVMemCfg mem_cfg=%d",
                mem_cfg);
    
    return mem_cfg;
}

/*****************************************************************************/
// 	Description : get mmidcsrv work mode
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_MODE_E MMIDCSRV_GetWorkMode(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    MMIDCSRV_MODE_E work_mode = 0;

    
    if (PNULL != info_ptr)
    {
        work_mode = info_ptr->work_param.work_mode;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_GetWorkMode work_mode=%d",
                work_mode);
    
    return work_mode;
}

/*****************************************************************************/
//  Description : get work video size width and height
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void GetMMIDCSRVWorkVideoSizeWidthAndHeight(MMIDC_WORK_VIDEO_SIZE_E video_size, //in
                                                            MMIDC_SIZE_ATTRIBUTE_T *size_ptr //out
                                                            )
{
    MMIDC_SIZE_ATTRIBUTE_T size_array[] =
    {
        {128, 96},//WORK_VIDEO_128_96,/*128x96 or 96x128*/
        {160, 128},//WORK_VIDEO_160_128,/*160x128 or 128x160*/
        {176, 144},//WORK_VIDEO_176_144,
        {320, 240}//WORK_VIDEO_320X240,/*320x240 or 240x320*/
    };
    int32 total_num = ARR_SIZE(size_array);

    MMIDC_SIZE_ATTRIBUTE_T size_3p4_array[] =
    {
        {96, 128},//WORK_VIDEO_128_96,/*128x96 or 96x128*/
        {128, 160},//WORK_VIDEO_160_128,/*160x128 or 128x160*/
        {144, 176},//WORK_VIDEO_176_144,
        {240, 320}//WORK_VIDEO_320X240,/*320x240 or 240x320*/
    };

    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  current_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);



    if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type
        && LCD_ANGLE_0 == current_angle)
    {
        if ((PNULL != size_ptr)
            && (video_size < WORK_VIDEO_SIZE_MAX)
            && (total_num <= WORK_VIDEO_SIZE_MAX) /*lint !e774*/
           )
        {
            SCI_MEMCPY(size_ptr, &size_3p4_array[video_size], sizeof(*size_ptr));
        }
    }
    else
    {    
        if ((PNULL != size_ptr)
            && (video_size < WORK_VIDEO_SIZE_MAX)
            && (total_num <= WORK_VIDEO_SIZE_MAX) /*lint !e774*/
           )
        {
            SCI_MEMCPY(size_ptr, &size_array[video_size], sizeof(*size_ptr));
        }
    }
}

/*****************************************************************************/
//  Description : convert work video size to dv video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL VIDEO_SIZE_E ConvertMMIDCSRVWorkVideoSizeToDCVideoSize(MMIDC_WORK_VIDEO_SIZE_E work_video_size)
{
    VIDEO_SIZE_E size_array[VIDEO_SIZE_MAX] = {0};
    int32 total_item = 0; 
    int32 i = 0;

    
    MMIDC_SIZE_ATTRIBUTE_T video_size_val = {0};
    MMIDC_SIZE_ATTRIBUTE_T work_size_val = {0};
    VIDEO_SIZE_E video_size = VIDEO_SIZE_MAX;
    VIDEO_SIZE_E support_size = 0;
    MMIDC_WORK_VIDEO_SIZE_E work_size = 0;

    
    do
    {
        work_size = work_video_size;
        //check work screen size
        if (work_size <= WORK_VIDEO_SIZE_MIN)
        {
            video_size = MMIDC_GetMinVideoSize();
            break;
        }

        //check work max size
        if (WORK_VIDEO_SIZE_MAX < work_size)
        {
            work_size = WORK_VIDEO_SIZE_MAX;
        }

        total_item = MMIDC_GetSupportVideoSizeArray(ARR_SIZE(size_array),size_array);

        //check other work size
        GetMMIDCSRVWorkVideoSizeWidthAndHeight(work_size, &work_size_val);
        for (i = total_item - 1; i >= 0; i--)
        {
            support_size = size_array[i];
            if (support_size < VIDEO_SIZE_MAX)
            {
                //find work photo size val to equal dc photo size val
                //if true, get dc photo size by work photo size
                MMIDC_GetVideoSizeWidthAndHeight(support_size, &video_size_val);
                if ((work_size_val.size_width == video_size_val.size_width)
                    && (work_size_val.size_height == video_size_val.size_height)
                   )
                {
                    video_size = support_size;
                    break;
                }

                if ((work_size_val.size_height == video_size_val.size_width)
                    && (work_size_val.size_width == video_size_val.size_height)
                   )
                {
                    video_size = support_size;
                    break;
                }
            }
        }

    }while(0);
    

    SCI_TRACE_LOW("[MMIDC] ConvertMMIDCSRVVideoSizeToDCVideoSize video_size=%d",\
                  video_size);
    
    return video_size;
}

/*****************************************************************************/
//  Description : check video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDCSRV_CheckVideoSize(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();

    MMIDC_WORK_VIDEO_SIZE_E work_video_size = 0;
    VIDEO_SIZE_E video_size = 0;


    if (PNULL != info_ptr)
    {
        work_video_size = info_ptr->work_param.dv.max_video_size;        
        video_size = ConvertMMIDCSRVWorkVideoSizeToDCVideoSize(work_video_size);

        info_ptr->dv_info.max_video_size = video_size;
    }
    
    SCI_TRACE_LOW("[MMIDC] MMIDCSRV_CheckVideoSize video_size=%d",\
                 video_size);
    
    return video_size;
}

/*****************************************************************************/
//  Description : get work photo size width and height
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void GetMMIDCSRVWorkPhotoSizeWidthAndHeight(MMIDC_WORK_PHOTO_SIZE_E photo_size, //in
                                                                MMIDC_SIZE_ATTRIBUTE_T *size_ptr //out
                                                                )
{
    MMIDC_SIZE_ATTRIBUTE_T size_array[] =
    {
        {0, 0},//WORK_PHOTO_SIZE_SCREEN,
        {320, 240},//WORK_PHOTO_SIZE_320_240,/*320x240 or 240x320*/
        {640, 480},//WORK_PHOTO_SIZE_640_480,/*640x480 or 480x640*/
        {1280, 960},//WORK_PHOTO_SIZE_1280_960,
        {1600, 1200},//WORK_PHOTO_SIZE_1600_1200,
        {2048, 1536},//WORK_PHOTO_SIZE_2048_1536,
        {2592, 1944},//WORK_PHOTO_SIZE_2592_1944,
        {3264, 2448}//WORK_PHOTO_SIZE_3264_2448,/*3264x2448 or 2448x3264*/
    };
    
    MMIDC_SIZE_ATTRIBUTE_T size_3pr_array[] =
    {
        {0, 0},//WORK_PHOTO_SIZE_SCREEN,
        {240, 320},//WORK_PHOTO_SIZE_320_240,/*320x240 or 240x320*/
        {480, 640},//WORK_PHOTO_SIZE_640_480,/*640x480 or 480x640*/
        {960, 1280},//WORK_PHOTO_SIZE_1280_960,
        {1200, 1600},//WORK_PHOTO_SIZE_1600_1200,
        {1536, 2048},//WORK_PHOTO_SIZE_2048_1536,
        {1944, 2592},//WORK_PHOTO_SIZE_2592_1944,
        {2448, 3264}//WORK_PHOTO_SIZE_3264_2448,/*3264x2448 or 2448x3264*/
    };
    
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  current_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);

    if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type
        && LCD_ANGLE_0 == current_angle)
    {
        if (PNULL != size_ptr && photo_size < WORK_PHOTO_SIZE_MAX)
        {
            SCI_MEMCPY(size_ptr, &size_3pr_array[photo_size], sizeof(*size_ptr));
        }
    }
    else
    {
        if (PNULL != size_ptr && photo_size < WORK_PHOTO_SIZE_MAX)
        {
            SCI_MEMCPY(size_ptr, &size_array[photo_size], sizeof(*size_ptr));
        }
    }
}

/*****************************************************************************/
//  Description : convert work photo size to dc photo size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_PHOTO_SIZE_E ConvertMMIDCSRVWorkPhotoSizeToDCPhotoSize(MMIDC_WORK_PHOTO_SIZE_E work_photo_size)
{
    MMIDC_PHOTO_SIZE_E size_array[PHOTO_SIZE_MAX] = {0};
    int16 total_item = 0; 
    int16 i = 0;

    MMIDC_PHOTO_SIZE_E max_photo_size = 0;
    
    MMIDC_SIZE_ATTRIBUTE_T photo_size_val = {0};
    MMIDC_SIZE_ATTRIBUTE_T work_size_val = {0};
    MMIDC_PHOTO_SIZE_E photo_size = PHOTO_SIZE_MAX;
    MMIDC_PHOTO_SIZE_E support_size = 0;


    do
    {
        //check work screen size
        if (work_photo_size <= WORK_PHOTO_SIZE_SCREEN)
        {
            photo_size = MMIDC_GetScreenPhotoSize();
            break;
        }

        total_item = ARR_SIZE(size_array);
        //init size array to PHOTO_SIZE_MAX
        //PHOTO_SIZE_MAX is not support size
        for (i = 0; i < total_item; i++)
        {
            size_array[i] = PHOTO_SIZE_MAX;
        }
        
        max_photo_size = MMIDC_GetSupportSizeArray(size_array);

        //check work max size
        if (WORK_PHOTO_SIZE_MAX <= work_photo_size)
        {
            photo_size = max_photo_size;
            break;
        }

        //check other work size
        GetMMIDCSRVWorkPhotoSizeWidthAndHeight(work_photo_size, &work_size_val);
        for (i = total_item - 1; i >= 0; i--)
        {
            support_size = size_array[i];
            if (support_size < PHOTO_SIZE_MAX)
            {
                //find work photo size val to equal dc photo size val
                //if true, get dc photo size by work photo size
                MMIDC_GetPhotoSizeWidthAndHeight(support_size, &photo_size_val);
                if ((work_size_val.size_width == photo_size_val.size_width)
                    && (work_size_val.size_height == photo_size_val.size_height)
                   )
                {
                    photo_size = support_size;
                    break;
                }

                if ((work_size_val.size_height == photo_size_val.size_width)
                    && (work_size_val.size_width == photo_size_val.size_height)
                   )
                {
                    photo_size = support_size;
                    break;
                }
            }
        }

    }while(0);
    
    //if do not find dc photo size
    //then get screen photo size
    if (PHOTO_SIZE_MAX == photo_size)
    {
        photo_size = MMIDC_GetScreenPhotoSize();
    }

    //SCI_TRACE_LOW:"[MMIDC] ConvertWorkPhotoSizeToDCPhotoSize photo_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_350_112_2_18_2_10_3_785,(uint8*)"d",photo_size);
    
    return photo_size;
}

/*****************************************************************************/
//  Description : check max photo size
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckMaxPhotoSize(MMIDC_PHOTO_SIZE_E photo_size_in, 
                                                                    MMIDC_PHOTO_SIZE_E* photo_size_out
                                                                    )
{
    BOOLEAN ret = FALSE;
    int16  total_size_number = 0;
    int32  lcd_size = 0;
    SCREEN_MODE_E screen_mode = 0;  
    MMIDC_PHOTO_SIZE_E size_array[PHOTO_SIZE_MAX] = {0};
    int16 total_item = 0;
    int16 i = 0;
    
    if(PNULL == photo_size_out)
    {
        return ret;
    }
    
    total_item = ARR_SIZE(size_array);
    //init size array to PHOTO_SIZE_MAX
    //PHOTO_SIZE_MAX is not support size
    for (i = 0; i < total_item; i++)
    {
        size_array[i] = PHOTO_SIZE_MAX;
    }
    lcd_size = MMIDC_GetLCDSize();
    screen_mode = MMIDC_GetScreenMode();  
    total_size_number = GetPhonePhotoSizeList(ARR_SIZE(size_array), lcd_size, screen_mode, size_array);

    if(total_size_number > 0)
    {
        for (i = 0; i < total_size_number; i++)
        {
            if(size_array[i] == photo_size_in)
            {
                ret = TRUE;
                break;
            }
        }

        if(!ret)
        {
            *photo_size_out = size_array[total_size_number - 1];
        }
    }   

    SCI_TRACE_LOW("[MMIDC] CheckMaxPhotoSize max_photo_size=%d", *photo_size_out);
    return ret;
}

/*****************************************************************************/
//  Description : check photo size
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDCSRV_CheckPhotoSize(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();

    MMIDC_WORK_PHOTO_SIZE_E work_photo_size = 0;
    MMIDC_PHOTO_SIZE_E photo_size = 0;


    if (PNULL != info_ptr)
    {
        work_photo_size = info_ptr->work_param.dc.max_photo_size;        
        photo_size = ConvertMMIDCSRVWorkPhotoSizeToDCPhotoSize(work_photo_size);
        info_ptr->dc_info.max_photo_size = photo_size;
        if(!CheckMaxPhotoSize(photo_size, &photo_size))
        {
            info_ptr->dc_info.max_photo_size = photo_size;
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] MMIDCSRV_CheckPhotoSize photo_size=%d",\
                 photo_size);
    
    return photo_size;
}

/*****************************************************************************/
//Description : set dc work parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDCSRV_WORK_PARAM_T MMIDCSRV_GetDCWorkParam(MMIDCSRV_MEM_CFG_E mem_cfg)
{
    MMIDCSRV_WORK_PARAM_T work_param = {0};

    
    switch(mem_cfg)
    {
    case MMIDCSRV_MEM_DC_UCAMERA:
        work_param.work_mode = MMIDCSRV_MODE_DC;
    
        work_param.dc.mem_cfg = MMIDCSRV_MEM_DC_UCAMERA;
        work_param.dc.max_photo_size = WORK_PHOTO_SIZE_MAX;
        work_param.dc.disp_mode = DCAMERA_DISP_MODE_MAX;    

        work_param.dv.mem_cfg = MMIDCSRV_MEM_DV_NORMAL;
        work_param.dv.max_video_size = WORK_VIDEO_SIZE_MAX;
        work_param.dv.disp_mode = DRECORDER_DISP_MODE_MAX;

        break;
    default:
        work_param.work_mode = MMIDCSRV_MODE_DC;
    
        work_param.dc.mem_cfg = MMIDCSRV_MEM_DC_NORMAL;
        work_param.dc.max_photo_size = WORK_PHOTO_SIZE_MAX;
        work_param.dc.disp_mode = DCAMERA_DISP_MODE_MAX;
        
        work_param.dv.mem_cfg = MMIDCSRV_MEM_DV_NORMAL;
        work_param.dv.max_video_size = WORK_VIDEO_SIZE_MAX;
        work_param.dv.disp_mode = DRECORDER_DISP_MODE_MAX;
        break;
    }


    SCI_TRACE_LOW("[MMIDC] MMIDCSRV_GetDCWorkParam mem_cfg=%d",\
                 mem_cfg);

    return work_param;                 
}

/*****************************************************************************/
//Description : set dc work parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDCSRV_SetWorkParam(MMIDCSRV_WORK_PARAM_T *work_param_ptr)
{
    s_mmidcsrv_info.work_param = *work_param_ptr;
}

/*****************************************************************************/
//  Description : get mmidcsrv max video size
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDCSRV_GetMaxVideoSizee(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    VIDEO_SIZE_E video_size = 0;

    
    if (PNULL != info_ptr)
    {
        video_size = info_ptr->dv_info.max_video_size;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_GetMaxVideoSizee video_size=%d",
                video_size);
    
    return video_size;
}

/*****************************************************************************/
//  Description : get photo size
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDCSRV_GetMaxPhotoSize(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    MMIDC_PHOTO_SIZE_E photo_size = 0;

    
    if (PNULL != info_ptr)
    {
        photo_size = info_ptr->dc_info.max_photo_size;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_GetMaxPhotoSize photo_size=%d",
                photo_size);
    
    return photo_size;
}

/*****************************************************************************/
//  Description : check mmidcsrv dc display mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISP_MODE_E MMIDCSRV_CheckDCDisplayMode(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    DCAMERA_DISP_MODE_E disp_mode = 0;

    
    if (PNULL != info_ptr)
    {
        disp_mode = info_ptr->work_param.dc.disp_mode;
        if (DCAMERA_DISP_MODE_MAX <= disp_mode)
        {
            disp_mode = MMIDC_GetPhotoDisplayMode();
        }
#if defined(MMIDC_F_WORK_MODE)
        if (MMIDC_IsWorkMode())
        {
            if((MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd())
                || (MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetHorLcd()))
            {

                if((PHOTO_SIZE_320X240 == MMIDC_GetPhotoSize()) 
                  && (LCD_ANGLE_0 == MMIAPIDC_GetCameraSensorAngle()))
                {
                    //nothing
                }
                else
                {
                    disp_mode = DCAMERA_DISP_FULLSCREEN;
                }
            }
        }
#endif
        info_ptr->dc_info.disp_mode = disp_mode;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_CheckDCDisplayMode disp_mode=%d",
                disp_mode);
    
    return disp_mode;
}

/*****************************************************************************/
//  Description : get mmidcsrv dc display mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISP_MODE_E MMIDCSRV_GetDCDisplayMode(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    DCAMERA_DISP_MODE_E disp_mode = 0;

    
    if (PNULL != info_ptr)
    {
        disp_mode = info_ptr->dc_info.disp_mode;
        
    #if defined(MMIDC_F_U_CAMERA)
        if (MMIDC_GetUCameraModuleFlag())
        {
            disp_mode = DCAMERA_DISP_FULLVIEW;
        }
    #endif
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_GetDCDisplayMode disp_mode=%d",
                disp_mode);
    
    return disp_mode;
}

/*****************************************************************************/
//  Description : check mmidcsrv dv display mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DRECORDER_DISP_MODE_E MMIDCSRV_CheckDVDisplayMode(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    DRECORDER_DISP_MODE_E disp_mode = 0;

    
    if (PNULL != info_ptr)
    {
        disp_mode = info_ptr->work_param.dv.disp_mode;
        if (DCAMERA_DISP_MODE_MAX <= disp_mode)
        {
            disp_mode = (DRECORDER_DISP_MODE_E)MMIDC_GetDefaultVisualMode();
        }
        info_ptr->dv_info.disp_mode = disp_mode;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_CheckDVDisplayMode disp_mode=%d",
                disp_mode);
    
    return disp_mode;
}

/*****************************************************************************/
//  Description : get mmidcsrv dv display mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DRECORDER_DISP_MODE_E MMIDCSRV_GetDVDisplayMode(void)
{
    MMIDCSRV_INFO_T *info_ptr = GetMMIDCSRVInfoPtr();
    DRECORDER_DISP_MODE_E disp_mode = 0;

    
    if (PNULL != info_ptr)
    {
        disp_mode = info_ptr->dv_info.disp_mode;
    }

    SCI_TRACE_LOW("[MMIDCSRV] MMIDCSRV_GetDVDisplayMode disp_mode=%d",
                disp_mode);
    
    return disp_mode;
}

/*****************************************************************************/
//  Description : check mmidcsrv param
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCSRV_CheckParam(void)
{
    MMIDCSRV_CheckDCDisplayMode();
    MMIDCSRV_CheckDVDisplayMode();
}

#endif //#ifdef CAMERA_SUPPORT

