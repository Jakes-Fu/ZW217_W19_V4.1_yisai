/*****************************************************************************
** File Name:      mmidc_smileautophoto.c                                            *
** Author:                                                                   *
** Date:           02/06/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to n in one                             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012      chunyou          Creat                                   *
******************************************************************************/

#define _MMIDC_SMILEAUTOPHOTO_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_SMILE_AUTO_PHOTO)
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
#include "mmidisplay_data.h"

//this module
#include "mmidc_camera_id.h"
#include "mmidc_option.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmidc_imageprocess.h"
#include "mmidc_smileautophoto.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL MMIDC_SMILE_AUTO_PHOTO_INFO_T *s_smile_auto_photo_info_ptr = PNULL;


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
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get smile auto photo info ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL MMIDC_SMILE_AUTO_PHOTO_INFO_T *GetSmileAutoPhotoInfoPtr(void)
{
    return s_smile_auto_photo_info_ptr;
}

/*****************************************************************************/
//  Description : get smile auto photo handle
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL IMGPROC_HANDLE GetSmileAutoPhotoHandle(void)
{
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    IMGPROC_HANDLE handle = 0;

    if (PNULL != info_ptr)
    {
        handle = info_ptr->handle;
    }

    //SCI_TRACE_LOW:"[MMIDC] GetSmileAutoPhotoHandle handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_86_112_2_18_2_9_54_736,(uint8*)"d",handle);

    return handle;                 
}

/*****************************************************************************/
//  Description : create smile auto photo handle
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateSmileAutoPhotoHandle(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    IMGPROC_HANDLE handle = 0;
    IMG_PROC_CREATE_T param = {0};

    if (PNULL != info_ptr)
    {
        if(0 == info_ptr->handle)
        {
            handle = IMGPROC_CreateHandle(&param);
            if (handle > 0)
            {
                info_ptr->handle = handle;
                ret = TRUE;
            }
        }
        else
        {
            ret = TRUE;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] CreateSmileAutoPhotoHandle ret=%d,handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_113_112_2_18_2_9_54_737,(uint8*)"dd",ret, handle);

    return ret;
}

/*****************************************************************************/
//  Description : destroy smile auto photo handle
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroySmileAutoPhotoHandle(void)
{
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    IMGPROC_HANDLE handle = 0;
    uint32 ret = 0;

    if (PNULL != info_ptr)
    {
        handle = GetSmileAutoPhotoHandle();
        if(handle > 0)
        {
            {
                uint32 count = 0;
                while(UCAMERA_STATUS_PROCESS == MMIDC_GetUCameraStatus())
                {
                    SCI_Sleep(150);
                    count++;
                    if(count > 30)
                    {
                        break;
                    }
                }
                SCI_TRACE_LOW("[MMIDC] DestroySmileAutoPhotoHandle count=%d", count);
            }
            
            ret = IMGPROC_DestroyHandle(handle);
        }

        info_ptr->handle = 0;
    }
    MMIDC_ClearSmileAutoPhotoOSDLayer();
    //SCI_TRACE_LOW:"[MMIDC] DestroySmileAutoPhotoHandle ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_137_112_2_18_2_9_54_738,(uint8*)"d",ret);
}

/*****************************************************************************/
//Description : get smile auto photo osd layer device info
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetSmileAutoPhotoOSDLayerDevInfo(void)
{
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
	if (PNULL != info_ptr)
	{
        if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&info_ptr->osd_dev_info))
        {
	        dev_info = info_ptr->osd_dev_info;
        }
    }

    return dev_info;
}

/*****************************************************************************/
//Description : clear smile auto photo osd layer
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearSmileAutoPhotoOSDLayer(void)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    
    dev_info = GetSmileAutoPhotoOSDLayerDevInfo();
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ClearSmileAutoPhotoOSDLayer block_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_171_112_2_18_2_9_54_739,(uint8*)"d",dev_info.block_id);

    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&dev_info))
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_ClearSmileAutoPhotoOSDLayer layer clear"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_175_112_2_18_2_9_54_740,(uint8*)"");

        UILAYER_Clear(&dev_info);
    }
}
/*****************************************************************************/
//Description : destroy smile auto photoosd layer
//Global resource dependence : none
//Author:chunyou
//Note:
/*****************************************************************************/
LOCAL void DestroySmileAutoPhotoOSDLayer(void)
{
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    GUI_LCD_DEV_INFO dev_info = {0};

    if (PNULL != info_ptr)
    {
        dev_info = GetSmileAutoPhotoOSDLayerDevInfo();
        if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&dev_info))
        {
            UILAYER_RELEASELAYER(&dev_info);    /*lint !e506 !e774*/
            
            dev_info.block_id = 0;
            info_ptr->osd_dev_info = dev_info;        
        }
    }
    //UILAYER_EnableOsdLayer(FALSE);
    //UILAYER_RestoreMainLayer(); 
}

/*****************************************************************************/
//Description : create smile auto photo osd layer
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateSmileAutoPhotoOSDLayer(void)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_bg_layer = TRUE;
    
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    int16   lcd_width = 0;
    int16   lcd_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO dev_info = {0};
    UILAYER_RESULT_E ret = 0;
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL == info_ptr)
    {
        return result;
    }
    dev_info = GetSmileAutoPhotoOSDLayerDevInfo();
    
    lcd_width = MMIDC_GetPreviewWidth();
    lcd_height = MMIDC_GetPreviewHeight();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreateSmileAutoPhotoOSDLayer width=%d, height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_231_112_2_18_2_9_54_741,(uint8*)"dd",lcd_width, lcd_height);

     //creat layer
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = MMIDC_MAIN_WIN_ID;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = lcd_width;
    create_info.height = lcd_height;
#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
    is_bg_layer = FALSE;
#endif
    create_info.is_bg_layer = is_bg_layer;
    create_info.is_static_layer = TRUE; 
    ret = UILAYER_CreateLayer(
        &create_info,
        &dev_info
        );

    if (UILAYER_RESULT_SUCCESS == ret)
    {
        result = TRUE;

        //UILAYER_RemoveMainLayer();

        //UILAYER_EnableOsdLayer(TRUE);
        
        info_ptr->osd_dev_info = dev_info;
        
        append_layer.layer_level = UILAYER_LEVEL_LOW;
        append_layer.lcd_dev_info = dev_info;
        UILAYER_AppendBltLayer(&append_layer);
        UILAYER_SetLayerColorKey(&dev_info, TRUE, MMIDC_GetTransparentColor());
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreateSmileAutoPhotoOSDLayer result=%d,dev_info.block_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_267_112_2_18_2_9_54_742,(uint8*)"dd",result, dev_info.block_id);
             
   return result;
}

/*****************************************************************************/
//  Description : open smile auto photo
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenSmileAutoPhoto(void)
{
    BOOLEAN ret = TRUE;
    uint32 buf_size = 0;
    MMIDC_IMAGE_PROCESS_TYPE_E type = IMG_PROC_FACE_DECT;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);
    
    do
    {
        //alloc memory
        buf_size = sizeof(*s_smile_auto_photo_info_ptr);
        if (PNULL == s_smile_auto_photo_info_ptr)
        {
            s_smile_auto_photo_info_ptr = SCI_ALLOCA(buf_size);
            if (PNULL == s_smile_auto_photo_info_ptr)
            {
                break;
            }
        }
        if (PNULL != s_smile_auto_photo_info_ptr)
        {
            SCI_MEMSET(s_smile_auto_photo_info_ptr, 0, buf_size);
        }

        //create handle
        ret = MMIDC_CreateSmileAutoPhotoHandle();
        if (!ret)
        {
            break;
        }
        //create osd layer
        ret = CreateSmileAutoPhotoOSDLayer();
        if (!ret)
        {
            break;
        }
        
        s_smile_auto_photo_info_ptr->is_enable = TRUE;
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
            MMIDC_SetPhotoSize(MMIDC_U_CAMERA_DEFAULT_PHOTO_SIZE);
        }

        MMIDC_SetPhotoDataFlag(TRUE);        
    }
    else
    {
        MMIDC_CloseSmileAutoPhoto();
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_OpenSmileAutoPhoto ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_329_112_2_18_2_9_54_743,(uint8*)"d", ret);
    
    return ret;    
}

/*****************************************************************************/
//  Description : close smile auto photo
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseSmileAutoPhoto(void)
{
    MMIDC_DestroySmileAutoPhotoHandle();
    DestroySmileAutoPhotoOSDLayer();
    
    if (PNULL != s_smile_auto_photo_info_ptr)
    {
        SCI_FREE(s_smile_auto_photo_info_ptr);
    }
}
/*****************************************************************************/
//Description : check if smile auto photo function opened
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsSmileAutoPhotoEnable(void)
{
    BOOLEAN is_enable = FALSE;    
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();

    if (PNULL != info_ptr)
    {
        is_enable = info_ptr->is_enable;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsSmileAutoPhotoEnable is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_363_112_2_18_2_9_55_744,(uint8*)"d",is_enable);
    
    return is_enable;
}

/*****************************************************************************/
//Description : change smile auto photo function enable or disable
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ChangeSmileAutoPhotoEnable(void)
{
    if(MMIDC_IsSmileAutoPhotoEnable())
    {
        MMIDC_CloseSmileAutoPhoto();
    }
    else
    {
        MMIDC_OpenSmileAutoPhoto();
    }
}

/*****************************************************************************/
//Description: convert smile rect to lcd rect
//Global resource dependence:
//Author:chunyou
//Note:
/*****************************************************************************/
LOCAL void ConvertSmileRectToLcdRect(GUI_RECT_T *dst_rect, GUI_RECT_T *src_rect)
{
    int16 lcd_height = MMIDC_GetPreviewHeight();
    //int16 lcd_width = MMIDC_GetPreviewWidth();
    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    DCAMERA_ROTATION_MODE_E camera_angle = MMIAPIDC_GetCameraSensorAngle();
    
    if ((PNULL == dst_rect)
        || (PNULL == src_rect)
       )
    {
        return;
    }    

    if(SCREEN_MODE_HORIIZONTAL == screen_mode)
    {
        switch(camera_angle)
        {
        case DCAMERA_ROTATION_0:
            dst_rect->left = src_rect->top;
            dst_rect->top = lcd_height - src_rect->right;
            dst_rect->right = src_rect->bottom;
            dst_rect->bottom = lcd_height - src_rect->left;
            break;
#if 0
        case DCAMERA_ROTATION_180:
            dst_rect->left = lcd_width - src_rect->bottom;
            dst_rect->top = src_rect->left;
            dst_rect->right = lcd_width - src_rect->top;
            dst_rect->bottom = src_rect->right;
            break;

        case DCAMERA_ROTATION_270:
            dst_rect->left = lcd_width - src_rect->right;
            dst_rect->top = lcd_height - src_rect->bottom;
            dst_rect->right = lcd_width - src_rect->left;
            dst_rect->bottom = lcd_height - src_rect->top;
            break;
            
        case DCAMERA_ROTATION_90:
#endif
        default:
            dst_rect->left = src_rect->left;
            dst_rect->top = src_rect->top;
            dst_rect->right = src_rect->right;
            dst_rect->bottom = src_rect->bottom;            
            break;
        } 
        
        
    }
    else
    {
        switch(camera_angle)
        {
#if 0
        case DCAMERA_ROTATION_90:
            dst_rect->left = lcd_width - src_rect->bottom;
            dst_rect->top = src_rect->left;
            dst_rect->right = lcd_width - src_rect->top;
            dst_rect->bottom = src_rect->right;
            break;

        case DCAMERA_ROTATION_180:
            dst_rect->left = lcd_width - src_rect->right;
            dst_rect->top = lcd_height - src_rect->bottom;
            dst_rect->right = lcd_width - src_rect->left;
            dst_rect->bottom = lcd_height - src_rect->top;
            break;

        case DCAMERA_ROTATION_270:
            dst_rect->left = src_rect->top;
            dst_rect->top = lcd_height - src_rect->right;
            dst_rect->right = src_rect->bottom;
            dst_rect->bottom = lcd_height - src_rect->left;
            break;
#endif            
        case DCAMERA_ROTATION_0:
        default:
            dst_rect->left = src_rect->left;
            dst_rect->top = src_rect->top;
            dst_rect->right = src_rect->right;
            dst_rect->bottom = src_rect->bottom;
            break;
        }        
    }

    //SCI_TRACE_LOW:"[MMIDC] ConvertVerRectToHorRect lcd_height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_475_112_2_18_2_9_55_745,(uint8*)"d", lcd_height);
}

/*****************************************************************************/
//Description: display smile auto photo osd rect
//Global resource dependence:
//Author:chunyou
//Note:
/*****************************************************************************/
LOCAL uint32 DisplaySmileAutoPhotoOsdRect(void* param_ptr /*in*/)
{
    uint32 ret = 0;
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    GUI_RECT_T osd_rect = {0};
    GUI_LCD_DEV_INFO dev_info = GetSmileAutoPhotoOSDLayerDevInfo();
    uint8 i=0x00;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;        
    GUI_RECT_T smile_rect = {0};
    IMG_PROC_RTN_T_PTR rtn_pram_ptr = (IMG_PROC_RTN_T_PTR)param_ptr;
    IMG_PROC_FACE_DECT_RTN_T* smile_pram_ptr = PNULL;

    if ((PNULL == param_ptr)
        || (PNULL == info_ptr)
        || (PNULL == rtn_pram_ptr)
       )
    {
        return ret;
    }

    smile_pram_ptr = (IMG_PROC_FACE_DECT_RTN_T*)rtn_pram_ptr->alg_ptr;

    if (PNULL == smile_pram_ptr)
    {
        return ret;
    }
    
    //首先清空smile auto photo osd layer,
    MMIDC_ClearSmileAutoPhotoOSDLayer();
       
    //SCI_TRACE_LOW:"[MMIDC] DisplaySmileAutoPhotoOsdRect pram 0 num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_513_112_2_18_2_9_55_746,(uint8*)"d",smile_pram_ptr->num);

    //当人脸识别检测到人脸数为0时，后者mmi关闭人脸识别功能，直接退出
    if(0x00 == smile_pram_ptr->num) 
    {
        //SCI_TRACE_LOW:"[MMIDC] DisplaySmileAutoPhotoOsdRect dect_pram_ptr->num == 0,return "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_518_112_2_18_2_9_55_747,(uint8*)"");

        return SCI_SUCCESS;
    }

    DCAMERA_GetInfo(DCAMERA_INFO_DISP_RECT, (uint32*)&info_ptr->dc_disp_rect);

    //SCI_TRACE_LOW:"[MMIDC] DisplaySmileAutoPhotoOsdRect dc_disp_rect x = %d,y = %d,w = %d,h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_527_112_2_18_2_9_55_748,(uint8*)"dddd",info_ptr->dc_disp_rect.x,info_ptr->dc_disp_rect.y,info_ptr->dc_disp_rect.w,info_ptr->dc_disp_rect.h);
    
    for(i=0x00; i<smile_pram_ptr->num; i++)
    {
        //SCI_TRACE_LOW:"[MMIDC] DisplaySmileAutoPhotoOsdRect pram i=%d,x = %d,y = %d,w = %d,h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_533_112_2_18_2_9_55_749,(uint8*)"ddddd",i,smile_pram_ptr->rect[i].x,smile_pram_ptr->rect[i].y,smile_pram_ptr->rect[i].w,smile_pram_ptr->rect[i].h);
        SCI_TRACE_LOW("[MMIDC] DisplaySmileAutoPhotoOsdRect smile_score =%d", smile_pram_ptr->smile_score[i] );
        
        smile_rect.left = info_ptr->dc_disp_rect.x + smile_pram_ptr->rect[i].x;
        smile_rect.top = info_ptr->dc_disp_rect.y + smile_pram_ptr->rect[i].y;
        smile_rect.right = smile_rect.left + smile_pram_ptr->rect[i].w - 1;
        smile_rect.bottom = smile_rect.top + smile_pram_ptr->rect[i].h - 1;

        //将driver传递上来的rect,转换成MMI 的rect
        ConvertSmileRectToLcdRect(&osd_rect, &smile_rect);        
        
        GUILCD_GetLogicWidthHeight((LCD_ID_E)dev_info.lcd_id,&lcd_width,&lcd_height);

        //adjust rect
        osd_rect.left   = (uint16)MAX(0,osd_rect.left);
        osd_rect.top    = (uint16)MAX(0,osd_rect.top);
        osd_rect.right  = (uint16)MIN((lcd_width-1),osd_rect.right);
        osd_rect.bottom = (uint16)MIN((lcd_height-1),osd_rect.bottom);
        
        //SCI_TRACE_LOW:"[MMIDC] DisplaySmileAutoPhotoOsdRect osd rect  left = %d,top = %d,right = %d,bottom = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_553_112_2_18_2_9_55_750,(uint8*)"dddd",osd_rect.left,osd_rect.top,osd_rect.right,osd_rect.bottom);

        //如果是无效的rect,则不画人脸识别的矩形，直接返回
        if (osd_rect.right < osd_rect.left || osd_rect.bottom < osd_rect.top)
        {
            //SCI_TRACE_LOW:"[MMIDC] DisplaySmileAutoPhotoOsdRect SCI_SUCCESS! draw Rect "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_558_112_2_18_2_9_55_751,(uint8*)"");
            //do nothing
        }
        else
        {            
            //有效，则根据人脸识别提供的rect, 画矩形
            if(smile_pram_ptr->smile_score[i] >= MMIDC_SMILE_AUTO_PHOTO_SMILE_SCORE)
            {
                LCD_DrawRect(&dev_info,osd_rect,MMI_YELLOW_COLOR);
            }
            else
            {
                LCD_DrawRect(&dev_info,osd_rect,MMI_GREEN_COLOR);
            }
            
            //SCI_TRACE_LOW:"[MMIDC] DisplaySmileAutoPhotoOsdRect  Draw Rect "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_573_112_2_18_2_9_55_752,(uint8*)"");
        }

    }
    
    MMIDC_CompleteDisplayOSD();
    return SCI_SUCCESS;
}
/*****************************************************************************/
//Description: process photo image callback
//Global resource dependence:
//Author:chunyou
//Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSmileAutoPhotoAllSmile(void *param_ptr)
{
    BOOLEAN result = TRUE;
    uint8 i = 0x00;
    IMG_PROC_RTN_T_PTR rtn_pram_ptr = (IMG_PROC_RTN_T_PTR)param_ptr;
    IMG_PROC_FACE_DECT_RTN_T* smile_pram_ptr = PNULL;

    if (PNULL != param_ptr && PNULL != rtn_pram_ptr)
    {
        smile_pram_ptr = (IMG_PROC_FACE_DECT_RTN_T*)rtn_pram_ptr->alg_ptr;

        if(PNULL == smile_pram_ptr)
        {
            return FALSE;
        }
        if(smile_pram_ptr->num == 0)
        {
            result = FALSE;
        }
        else
        {
            for(i=0x00; i<smile_pram_ptr->num; i++)
            {
                if(smile_pram_ptr->smile_score[i] < MMIDC_SMILE_AUTO_PHOTO_SMILE_SCORE)
                {
                    result = FALSE;
                    break;
                }
            }
        }
    }
    else
    {
        result = FALSE;
    }

    //SCI_TRACE_LOW:"[MMIDC] IsSmileAutoPhotoAllSmile result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_621_112_2_18_2_9_55_753,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//Description: process photo image callback
//Global resource dependence:
//Author:chunyou
//Note:
/*****************************************************************************/
LOCAL uint32 SmileAutoPhotoImageProcessCallBack(void *param_ptr)
{
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    uint32 ret = 1;
    
    
    if ((PNULL == param_ptr)
        || (PNULL == info_ptr)
       )
    {
        return ret;
    }    

    SCI_TRACE_LOW("[MMIDC] SmileAutoPhotoImageProcessCallBack tick = %d ", SCI_GetTickCount());
    
    info_ptr->cur_index++;
    if (0 == info_ptr->cur_index)
    {
        info_ptr->cur_index++;
    }
    if(MMIDC_GET_FOCUS == MMIDC_GetMMIFocusStatus())
    {
        DisplaySmileAutoPhotoOsdRect(param_ptr);
        if(IsSmileAutoPhotoAllSmile(param_ptr))
        {
            MMIDC_ClearSmileAutoPhotoOSDLayer();
            MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_APP_WEB, (DPARAM)PNULL, (uint32)0);
        }   
    }
    MMIDC_SetUCameraStatus(UCAMERA_STATUS_START);
    
    ret = 0;
    //SCI_TRACE_LOW:"[MMIDC] SmileAutoPhotoImageProcessCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_657_112_2_18_2_9_55_754,(uint8*)"");

    return ret;
}

/*****************************************************************************/
//Description : get smile auto photo image process parameter
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetSmileAutoPhotoImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                                            )
{
    MMIDC_SMILE_AUTO_PHOTO_INFO_T *info_ptr = GetSmileAutoPhotoInfoPtr();
    int32 ret = -1;
    IMG_PROC_PROCESS_T* proc_param_ptr={0x00}; 
    MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr = PNULL;
    
    if ((PNULL == info_ptr)
        || (PNULL == img_param_ptr)
    )
    {
        return ret;
    }    

    src_info_ptr = img_param_ptr->src_info_ptr;
    if(PNULL == src_info_ptr)
    {
        return ret;
    }
    
    if(UCAMERA_DATA_ROUTE_PREVIEW == src_info_ptr->data_from)
    {
        proc_param_ptr = &img_param_ptr->img_proc_param; 

        img_param_ptr->handle = GetSmileAutoPhotoHandle();
        proc_param_ptr->cmd=IMG_PROC_FACE_DECT;

        proc_param_ptr->proc_img.rect.w = img_param_ptr->src_info_ptr->data_size.w;
        proc_param_ptr->proc_img.rect.h = img_param_ptr->src_info_ptr->data_size.h;
        proc_param_ptr->proc_img.data.format = IMG_PROC_FORMAT_MAX; //not need post process data
        proc_param_ptr->proc_img.ctrl_callback = SmileAutoPhotoImageProcessCallBack;    // callback fun ptr

        proc_param_ptr->proc_img.data.addr.chn0 = img_param_ptr->dst_info_ptr->data_addr.chn1_ptr;
        proc_param_ptr->proc_img.data.addr.chn0_len = img_param_ptr->dst_info_ptr->data_addr.chn1_size;
        proc_param_ptr->proc_img.data.addr.chn1 = img_param_ptr->dst_info_ptr->data_addr.chn2_ptr;
        proc_param_ptr->proc_img.data.addr.chn1_len = img_param_ptr->dst_info_ptr->data_addr.chn2_size;
        proc_param_ptr->proc_img.data.size.w = img_param_ptr->src_info_ptr->data_size.w;
        proc_param_ptr->proc_img.data.size.h = img_param_ptr->src_info_ptr->data_size.h;

        SCI_MEMCPY(img_param_ptr->dst_info_ptr->data_addr.chn1_ptr,\
                           img_param_ptr->src_info_ptr->data_addr.chn1_ptr,\
                           img_param_ptr->src_info_ptr->data_addr.chn1_size);
        
        SCI_MEMCPY(img_param_ptr->dst_info_ptr->data_addr.chn2_ptr,\
                           img_param_ptr->src_info_ptr->data_addr.chn2_ptr,\
                           img_param_ptr->src_info_ptr->data_addr.chn2_size);
        

        if(info_ptr->cur_index == 0)
        {
            proc_param_ptr->proc_mode=IMG_PROC_FIRST;
        }
        else
        {
            proc_param_ptr->proc_mode=IMG_PROC_UPDATE_SRC;
        }    
        proc_param_ptr->proc_mode|=IMG_PROC_PROCESS_EB;

        ret = 0;
    }
    else if (UCAMERA_DATA_ROUTE_CAPTURE == src_info_ptr->data_from)
    {
        MMIDC_ClearSmileAutoPhotoOSDLayer();
        img_param_ptr->dst_info_ptr->is_skip_image_process = TRUE;
        
        ret = MMIDC_SaveDataToFile(0, 
                                    src_info_ptr->data_addr.chn1_ptr,
                                    src_info_ptr->data_addr.chn1_size
                                    );
    }
   
#ifdef WIN32
{
    IMG_PROC_RTN_T rtn_pram = {0};
    IMG_PROC_FACE_DECT_RTN_T smile_pram = {0};    

    smile_pram.num = 1;
    smile_pram.smile_score[0] = 60;
    smile_pram.rect[0].x = 20;
    smile_pram.rect[0].y = 40;
    smile_pram.rect[0].w = 50;
    smile_pram.rect[0].h = 100;

    rtn_pram.alg_ptr = &smile_pram;

    DisplaySmileAutoPhotoOsdRect(&rtn_pram);
}
#endif   
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetSmileAutoPhotoImageProcessParam ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_SMILEAUTOPHOTO_757_112_2_18_2_9_55_755,(uint8*)"d",ret);

    return ret;
}
#endif //#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_SMILE_AUTO_PHOTO)

