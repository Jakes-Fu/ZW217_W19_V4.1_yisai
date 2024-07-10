/*****************************************************************************
** File Name:      mmidc_effect360.c                                         *
** Author:                                                                   *
** Date:           02/02/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to effect 360                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012      robert.wang          Creat                                   *
******************************************************************************/

#define _MMIDC_EFFECT360_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_EFFECT_360)
#include "mmi_app_camera_trc.h"

#ifdef WIN32 
#include "std_header.h"
#endif

#include "mmk_type.h"
#include "mmi_default.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"
#include "guiblock.h"

//other module
#include "guiiconlist.h"

//this module
#include "mmidc_camera_id.h"
#include "mmidc_option.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmidc_imageprocess.h"
#include "mmidc_effect360.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/

LOCAL MMIDC_EFFECT_360_INFO_T *s_effect_360_info_ptr = PNULL;
LOCAL MMIDC_EFFECT_360_COMMON_INFO_T *s_effect_360_common_info_ptr = PNULL;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get effect 360 info ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_EFFECT_360_INFO_T *GetEffect360InfoPtr(void)
{
    return s_effect_360_info_ptr;
}

/*****************************************************************************/
//  Description : get effect 360 common info ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL MMIDC_EFFECT_360_COMMON_INFO_T *GetEffect360CommonInfoPtr(void)
{
    return s_effect_360_common_info_ptr;
}

/*****************************************************************************/
//  Description : get cur effect 360 common type
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL MMIDC_IMAGE_PROCESS_TYPE_E GetCurEffect360CommonType(void)
{
    MMIDC_IMAGE_PROCESS_TYPE_E type = 0;
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();

    if(PNULL != info_ptr)
    {
        type = info_ptr->cur_common_type;
        //SCI_TRACE_LOW:"[MMIDC] GetCurEffect360CommonType type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_95_112_2_18_2_8_26_15,(uint8*)"d",info_ptr->cur_common_type);
    }

    return type;
}

/*****************************************************************************/
//  Description : is effect 360 common type
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEffect360CommonType(void)
{
    BOOLEAN resut = FALSE;
    MMIDC_EFFECT_360_INFO_T *info_ptr = GetEffect360InfoPtr();

    if(PNULL != info_ptr)
    {
        switch(info_ptr->cur_type)
        {
        case IMG_PROC_HDR:
        case IMG_PROC_FISHEYE:
        case IMG_PROC_LOMO:
        case IMG_PROC_BLACK_WHITE_DRAWING:
        case IMG_PROC_XRAY:
        case IMG_PROC_INFRARED:
        case IMG_PROC_EXPOSAL:
        case IMG_PROC_NEGATIVE:
            resut = TRUE;
            break;
        default:
            break;
        }
    }
    //SCI_TRACE_LOW:"[MMIDC] IsEffect360CommonType resut=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_129_112_2_18_2_8_26_16,(uint8*)"d",resut);

    return resut;
}

/*****************************************************************************/
//  Description : get effect 360 common handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL IMGPROC_HANDLE GetEffect360CommonHandle(void)
{
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();
    IMGPROC_HANDLE handle = 0;

    if (PNULL != info_ptr)
    {
        switch(GetCurEffect360CommonType())
        {
        case IMG_PROC_HDR:
            handle = info_ptr->hdr_info.handle;
            break;
        case IMG_PROC_FISHEYE:
            handle = info_ptr->fisheye_info.handle;
            break;
        case IMG_PROC_LOMO:
            handle = info_ptr->lomo_info.handle;
            break;
        case IMG_PROC_BLACK_WHITE_DRAWING:
            handle = info_ptr->black_white_drawing_info.handle;
            break;
        case IMG_PROC_XRAY:
            handle = info_ptr->x_ray_info.handle;
            break;
        case IMG_PROC_INFRARED:
            handle = info_ptr->infrared_info.handle;
            break;
        case IMG_PROC_EXPOSAL:
            handle = info_ptr->exposal_info.handle;
            break;
        case IMG_PROC_NEGATIVE:
            handle = info_ptr->negative_info.handle;
            break;            
        default:
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] GetEffect360CommonHandle handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_177_112_2_18_2_8_26_17,(uint8*)"d",handle);

    return handle;                 
}

/*****************************************************************************/
//  Description : create effect 360 common handle
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateEffect360CommonHandle(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_EFFECT_360_INFO_T *effect360_info_ptr = GetEffect360InfoPtr();
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();
    IMGPROC_HANDLE handle = 0;
    IMG_PROC_CREATE_T param = {0};

    if (PNULL != info_ptr && PNULL != effect360_info_ptr)
    {
        handle = IMGPROC_CreateHandle(&param);
        if (handle > 0)
        {
            switch(effect360_info_ptr->cur_type)
            {
            case IMG_PROC_HDR:
                info_ptr->hdr_info.handle = handle;
                break;
            case IMG_PROC_FISHEYE:
                info_ptr->fisheye_info.handle = handle;
                break;
            case IMG_PROC_LOMO:
                info_ptr->lomo_info.handle = handle;
                break;
            case IMG_PROC_BLACK_WHITE_DRAWING:
                info_ptr->black_white_drawing_info.handle = handle;
                break;
            case IMG_PROC_XRAY:
                info_ptr->x_ray_info.handle = handle;
                break;
            case IMG_PROC_INFRARED:
                info_ptr->infrared_info.handle = handle;
                break;                
            case IMG_PROC_EXPOSAL:
                info_ptr->exposal_info.handle = handle;
                break;
            case IMG_PROC_NEGATIVE:
                info_ptr->negative_info.handle = handle;
                break;                
            default:
                break;
            }
            ret = TRUE;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIDC] CreateEffect360CommonHandle ret=%d,handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_233_112_2_18_2_8_26_18,(uint8*)"dd",ret, handle);

    return ret;
}

/*****************************************************************************/
//  Description : destroy effect 360 common handle
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL void DestroyEffect360CommonHandle(void)
{
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();
    IMGPROC_HANDLE handle = 0;
    uint32 ret = 0;

    if (PNULL != info_ptr)
    {
        handle = GetEffect360CommonHandle();
        ret = IMGPROC_DestroyHandle(handle);

        switch(GetCurEffect360CommonType())
        {
        case IMG_PROC_HDR:
            info_ptr->hdr_info.handle = 0;
            break;
        case IMG_PROC_FISHEYE:
            info_ptr->fisheye_info.handle = 0;
            break;
        case IMG_PROC_LOMO:
            info_ptr->lomo_info.handle = 0;
            break;
        case IMG_PROC_BLACK_WHITE_DRAWING:
            info_ptr->black_white_drawing_info.handle = 0;
            break;
        case IMG_PROC_XRAY:
            info_ptr->x_ray_info.handle = 0;
            break;
        case IMG_PROC_INFRARED:
            info_ptr->infrared_info.handle = 0;
            break;
        case IMG_PROC_EXPOSAL:
            info_ptr->exposal_info.handle = 0;
            break;
        case IMG_PROC_NEGATIVE:
            info_ptr->negative_info.handle = 0;
            break;            
        default:
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] DestroyEffect360CommonHandle ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_285_112_2_18_2_8_26_19,(uint8*)"d",ret);
}

/*****************************************************************************/
//  Description : create photo info
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateCommonPhotoInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();

    DC_IMG_SIZE_T img_size = {0};
    uint32 buf_size = 0;
    uint8 *buf_ptr = PNULL;


    MMIDC_SIZE_ATTRIBUTE_T size_value = {0};
    MMIDC_PHOTO_SIZE_E photo_size = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);

    if(PNULL == info_ptr)
    {
        return ret;
    }

    if ( (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type) 
            && (LCD_ANGLE_0 == logic_angle)
        )
    {
        photo_size = PHOTO_SIZE_480X640;
    }
    else
    {
        photo_size = MMIDC_U_CAMERA_DEFAULT_PHOTO_SIZE;
    }

    MMIDC_GetPhotoSizeWidthAndHeight(photo_size, &size_value);
    img_size.w = size_value.size_width;
    img_size.h = size_value.size_height;

    
    info_ptr->photo_info.img_size = img_size;
    buf_size = MMIDC_GetDataSizeForJPG(img_size.w, img_size.h);

    buf_ptr = SCI_ALLOCA(buf_size);
    if (PNULL != buf_ptr)
    {
        SCI_MEMSET(buf_ptr, 0x00, buf_size);

        info_ptr->photo_info.photo_data.buffer_ptr = buf_ptr;
        info_ptr->photo_info.photo_data.buffer_size = buf_size;

        ret = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIDC] CreateCommonPhotoInfo buf_ptr=0x%X, buf_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_332_112_2_18_2_8_26_20,(uint8*)"dd",buf_ptr, buf_size);
                  
    //SCI_TRACE_LOW:"[MMIDC] CreateCommonPhotoInfo ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_335_112_2_18_2_8_26_21,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
//  Description : destroy photo info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyCommonPhotoInfo(void)
{
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();

    uint8 *buf_ptr = PNULL;
    

    if (PNULL != info_ptr)
    {
        if (PNULL != info_ptr->photo_info.photo_data.buffer_ptr)
        {
            buf_ptr = info_ptr->photo_info.photo_data.buffer_ptr;
            if (PNULL != buf_ptr)
            {
                SCI_FREE(buf_ptr);
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] DestroyCommonPhotoInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_363_112_2_18_2_8_26_22,(uint8*)"");
}

/*****************************************************************************/
//  Description : open effect 360
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenEffect360(void)
{
    BOOLEAN ret = FALSE;
    uint32 buf_size = 0;
    MMIDC_IMAGE_PROCESS_TYPE_E type = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);
    
    do
    {
        //alloc memory
        buf_size = sizeof(*s_effect_360_info_ptr);
        if (PNULL == s_effect_360_info_ptr)
        {
            s_effect_360_info_ptr = SCI_ALLOCA(buf_size);
            if (PNULL == s_effect_360_info_ptr)
            {
                break;
            }
        }
        if (PNULL != s_effect_360_info_ptr)
        {
            SCI_MEMSET(s_effect_360_info_ptr, 0, buf_size);
        }
        
        type = MMIDC_GetImageProcessType();
        s_effect_360_info_ptr->cur_type = type;

        if(IsEffect360CommonType())
        {
            //alloc memory
            buf_size = sizeof(*s_effect_360_common_info_ptr);
            if (PNULL == s_effect_360_common_info_ptr)
            {
                s_effect_360_common_info_ptr = SCI_ALLOCA(buf_size);
                if (PNULL == s_effect_360_common_info_ptr)
                {
                    break;
                }
            }
            if (PNULL != s_effect_360_common_info_ptr)
            {
                SCI_MEMSET(s_effect_360_common_info_ptr, 0, buf_size);
            }
            ret = CreateEffect360CommonHandle();
            if (!ret)
            {
                break;
            }

            ret = CreateCommonPhotoInfo();
            if (!ret)
            {
                break;
            }
            s_effect_360_common_info_ptr->cur_common_type = s_effect_360_info_ptr->cur_type;
        }       
        
        s_effect_360_info_ptr->is_enable = TRUE;
        ret = TRUE;        
    }while(0);

    if (ret)
    {
        MMIDC_SetImageProcessType(type);
        
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

        MMIDC_SetPhotoDataFlag(TRUE);
    }
    else
    {
        MMIDC_CloseEffect360();
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_OpenEffect360 ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_442_112_2_18_2_8_26_23,(uint8*)"d", ret);
    
    return ret;    
}

/*****************************************************************************/
//  Description : close effect 360
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseEffect360(void)
{
    if(IsEffect360CommonType())
    {
        DestroyCommonPhotoInfo();
        DestroyEffect360CommonHandle();
        
        if (PNULL != s_effect_360_common_info_ptr)
        {
            SCI_FREE(s_effect_360_common_info_ptr);
        }
    }
  
    if (PNULL != s_effect_360_info_ptr)
    {
        SCI_FREE(s_effect_360_info_ptr);
    }
}

/*****************************************************************************/
//Description : is effect 360 enable 
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsEffect360Enable(void)
{
    BOOLEAN is_enable = FALSE;    
    MMIDC_EFFECT_360_INFO_T *info_ptr = GetEffect360InfoPtr();

    if (PNULL != info_ptr)
    {
        is_enable = info_ptr->is_enable;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsEffect360Enable is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_485_112_2_18_2_8_27_24,(uint8*)"d",is_enable);
    
    return is_enable;
}

/*****************************************************************************/
//Description: process photo image callback
//Global resource dependence:
//Author:chunyou
//Note:
/*****************************************************************************/
LOCAL uint32 Effect360CommonProcessCallBack(void *param_ptr)
{
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();
    uint32 ret = 1;
    
    if ((PNULL == param_ptr)
        || (PNULL == info_ptr)
       )
    {
        return ret;
    }

    MMIDC_UCameraASyncCallbackSendMsgToApp((IMG_PROC_DATA_T *)param_ptr);

    ret = 0;
    //SCI_TRACE_LOW:"[MMIDC] Effect360CommonProcessCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_509_112_2_18_2_8_27_25,(uint8*)"");

    return ret;
}

/*****************************************************************************/
//Description: effect 360 common sync save photo file callback
//Global resource dependence:
//Author:chunyou
//Note:
/*****************************************************************************/
LOCAL int32 Effect360CommonSyncSavePhotoFileCallBack(IMG_PROC_PROCESS_RTN_T *img_proc_result_ptr, //in
                                                  MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr, //in
                                                  MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr //out
                                                  )
{
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();
    IMG_PROC_DATA_T *proc_data_ptr = PNULL;    
    int32 ret = -1;
    
    if ((PNULL == img_proc_result_ptr)
        || (PNULL == info_ptr)
       )
    {
        return ret;
    }
    
    proc_data_ptr = &img_proc_result_ptr->data_rtn.proc_img;
    MMIDC_UCameraASyncCallbackSendMsgToApp((IMG_PROC_DATA_T *)proc_data_ptr);

    ret = 0;
    //SCI_TRACE_LOW:"[MMIDC] Effect360CommonProcessCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_538_112_2_18_2_8_27_26,(uint8*)"");

    return ret;
}

/*****************************************************************************/
//Description : get hdr image process parameter
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL int32 GetEffect360CommonImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                                            )
{
    MMIDC_EFFECT_360_COMMON_INFO_T *info_ptr = GetEffect360CommonInfoPtr();
    int32 ret = -1;
    IMG_PROC_HDR_T* hdr_param_ptr = PNULL;
    IMG_PROC_FISHEYE_T *fisheye_param_ptr = PNULL;
    IMG_PROC_LOMO_PARAM_T *lomo_param_ptr = PNULL;
    IMG_PROC_PENCILSKETCH_T *pencil_sketch_param_ptr = PNULL;
    
    IMG_PROC_PROCESS_T* proc_param_ptr = PNULL; 
    uint32 cur_index = 0;

    if ((PNULL == info_ptr)
    || (PNULL == img_param_ptr)
    )
    {
        return ret;
    }
    proc_param_ptr = &img_param_ptr->img_proc_param;

    img_param_ptr->handle = GetEffect360CommonHandle();    

    
    switch(GetCurEffect360CommonType())
    {
    case IMG_PROC_HDR:
        hdr_param_ptr = &info_ptr->hdr_info.hdr_param;
        hdr_param_ptr->level = 1;
        
        proc_param_ptr->cmd = IMG_PROC_HDR;
        proc_param_ptr->proc_param_ptr = hdr_param_ptr;
        cur_index = info_ptr->hdr_info.cur_index++;
        break;
    case IMG_PROC_FISHEYE:
        fisheye_param_ptr = &info_ptr->fisheye_info.fisheye_param;
        fisheye_param_ptr->distort_level = 100;
        fisheye_param_ptr->back_color = MMIDC_FISH_EYE_BG_COLOR;
        
        proc_param_ptr->cmd = IMG_PROC_FISHEYE;
        proc_param_ptr->proc_param_ptr = fisheye_param_ptr;
        cur_index = info_ptr->fisheye_info.cur_index++;
        break;
    case IMG_PROC_LOMO:
        lomo_param_ptr = &info_ptr->lomo_info.lomo_param;
        lomo_param_ptr->width = img_param_ptr->src_info_ptr->data_size.w;
        lomo_param_ptr->height = img_param_ptr->src_info_ptr->data_size.h;        
        lomo_param_ptr->mode = IMG_PROC_LOMO_MODE_FULL;
        
        proc_param_ptr->cmd = IMG_PROC_LOMO;
        proc_param_ptr->proc_param_ptr = lomo_param_ptr;
        cur_index = info_ptr->lomo_info.cur_index++;
        break;
    case IMG_PROC_BLACK_WHITE_DRAWING:
        pencil_sketch_param_ptr = &info_ptr->black_white_drawing_info.black_white_drawing_param;

        proc_param_ptr->cmd = IMG_PROC_BLACK_WHITE_DRAWING;
        proc_param_ptr->proc_param_ptr = pencil_sketch_param_ptr;
        cur_index = info_ptr->black_white_drawing_info.cur_index++;
        break;
        
    case IMG_PROC_XRAY:
        proc_param_ptr->cmd = IMG_PROC_XRAY;
        cur_index = info_ptr->x_ray_info.cur_index++;
        break;
        
    case IMG_PROC_INFRARED:
        proc_param_ptr->cmd = IMG_PROC_INFRARED;
        cur_index = info_ptr->infrared_info.cur_index++;
        break;
    case IMG_PROC_EXPOSAL:
        proc_param_ptr->cmd = IMG_PROC_EXPOSAL;
        cur_index = info_ptr->exposal_info.cur_index++;
        break;
        
    case IMG_PROC_NEGATIVE:
        proc_param_ptr->cmd = IMG_PROC_NEGATIVE;
        cur_index = info_ptr->negative_info.cur_index++;
        break;        
    default:
        break;
    }
    
    if(UCAMERA_DATA_ROUTE_PREVIEW == img_param_ptr->src_info_ptr->data_from)
    {
        proc_param_ptr->proc_img.rect.w = img_param_ptr->src_info_ptr->data_size.w;
        proc_param_ptr->proc_img.rect.h = img_param_ptr->src_info_ptr->data_size.h;
        proc_param_ptr->proc_img.data.format= IMG_PROC_YUV422;
        proc_param_ptr->proc_img.data.addr.chn0= img_param_ptr->dst_info_ptr->data_addr.chn1_ptr;//info_ptr->osd_info.osd_yuv.chn1_ptr;
        proc_param_ptr->proc_img.data.addr.chn0_len= img_param_ptr->dst_info_ptr->data_addr.chn1_size;//info_ptr->osd_info.osd_yuv.chn1_size;
        proc_param_ptr->proc_img.data.addr.chn1= img_param_ptr->dst_info_ptr->data_addr.chn2_ptr;//info_ptr->osd_info.osd_yuv.chn2_ptr;
        proc_param_ptr->proc_img.data.addr.chn1_len= img_param_ptr->dst_info_ptr->data_addr.chn2_size;//info_ptr->osd_info.osd_yuv.chn2_size;
        proc_param_ptr->proc_img.data.size.w = img_param_ptr->src_info_ptr->data_size.w;
        proc_param_ptr->proc_img.data.size.h = img_param_ptr->src_info_ptr->data_size.h;

        //proc_param_ptr->proc_img.ctrl_callback=HdrProcessCallBack;¡¡// callback fun ptr

        if(cur_index == 0)
        {
            proc_param_ptr->proc_mode = IMG_PROC_FIRST;
            proc_param_ptr->proc_mode |= IMG_PROC_PROCESS_EB;
        }
        else
        {
            proc_param_ptr->proc_mode = IMG_PROC_UPDATE_SRC;
            proc_param_ptr->proc_mode |= IMG_PROC_UPDATE_DST;
            proc_param_ptr->proc_mode |= IMG_PROC_PROCESS_EB;
            
        }        

        img_param_ptr->sync_callback = MMIDC_UCameraSyncCallback;
    }
    else if(UCAMERA_DATA_ROUTE_CAPTURE == img_param_ptr->src_info_ptr->data_from)
    {
        proc_param_ptr->proc_img.rect.w = img_param_ptr->src_info_ptr->data_size.w;
        proc_param_ptr->proc_img.rect.h = img_param_ptr->src_info_ptr->data_size.h;
        proc_param_ptr->proc_img.data.format = IMG_PROC_JPG;
        proc_param_ptr->proc_img.data.addr.chn0 = info_ptr->photo_info.photo_data.buffer_ptr;
        proc_param_ptr->proc_img.data.addr.chn0_len = info_ptr->photo_info.photo_data.buffer_size;
        proc_param_ptr->proc_img.data.size.w = img_param_ptr->src_info_ptr->data_size.w;
        proc_param_ptr->proc_img.data.size.h = img_param_ptr->src_info_ptr->data_size.h;

        //proc_param_ptr->proc_img.ctrl_callback = Effect360CommonProcessCallBack; // callback fun ptr

        proc_param_ptr->proc_mode = IMG_PROC_UPDATE_SRC;
        proc_param_ptr->proc_mode |= IMG_PROC_UPDATE_DST;
        proc_param_ptr->proc_mode |= IMG_PROC_PROCESS_EB;
        proc_param_ptr->proc_mode |= IMG_PROC_END;
        //proc_param_ptr->proc_mode |= IMG_PROC_FIRST;
        

        img_param_ptr->sync_callback = Effect360CommonSyncSavePhotoFileCallBack;

        //SCI_TRACE_LOW:"[MMIDC] GetEffect360CommonImageProcessParam capture"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_680_112_2_18_2_8_27_27,(uint8*)"");
    }    
    
    ret = 0;
    //SCI_TRACE_LOW:"[MMIDC] GetEffect360CommonImageProcessParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_684_112_2_18_2_8_27_28,(uint8*)"");
    
    return ret;
    
}

/*****************************************************************************/
//Description : get effect 360 image process parameter
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetEffect360ImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                                               )
{
    int32 ret = 0;
    MMIDC_EFFECT_360_INFO_T *info_ptr = GetEffect360InfoPtr();
    
    if ((PNULL == info_ptr)
        || (PNULL == img_param_ptr)
       )
    {
        return ret;
    }

    if(IsEffect360CommonType())
    {
        GetEffect360CommonImageProcessParam(img_param_ptr);
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetEffect360ImageProcessParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EFFECT360_712_112_2_18_2_8_27_29,(uint8*)"");

    return ret;
}

#endif //#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_EFFECT_360)

