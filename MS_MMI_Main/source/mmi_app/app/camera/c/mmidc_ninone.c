/*****************************************************************************
** File Name:      mmidc_ninone.c                                            *
** Author:                                                                   *
** Date:           01/26/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to n in one                             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012      robert.wang          Creat                                   *
******************************************************************************/

#define _MMIDC_NINONE_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_N_IN_ONE)
#include "mmi_app_camera_trc.h"

#ifdef WIN32 
#include "std_header.h"
#endif

#include "mmk_type.h"
#include "mmi_default.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"
#include "display_blend_alg.h"
#include "guiblock.h"
#include "guires.h"



//this module
#include "mmidc_option.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmidc_imageprocess.h"
#include "mmidc_ninone.h"
#include "mmidc_camera_id.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL MMIDC_N_IN_ONE_INFO_T *s_n_in_one_info_ptr = PNULL;


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
//  Description : get n in one info ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_N_IN_ONE_INFO_T *GetNInOneInfoPtr(void)
{
    return s_n_in_one_info_ptr;
}

/*****************************************************************************/
//  Description : get n in one osd info ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_N_IN_ONE_OSD_INFO_T *GetNInOneOSDInfoPtr(void)
{
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = PNULL;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if (PNULL != info_ptr)
    {
        osd_info_ptr = &info_ptr->osd_info;
    }

    return osd_info_ptr;
}

/*****************************************************************************/
//  Description : get n in one handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL IMGPROC_HANDLE GetNInOneHandle(void)
{
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();
    IMGPROC_HANDLE handle = 0;

    if (PNULL != info_ptr)
    {
        handle = info_ptr->handle;
    }

    //SCI_TRACE_LOW:"[MMIDC] GetNInOneHandle handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_106_112_2_18_2_9_25_461,(uint8*)"d",handle);

    return handle;                 
}

/*****************************************************************************/
//  Description : create n in one handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateNInOneHandle(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();
    IMGPROC_HANDLE handle = 0;
    IMG_PROC_CREATE_T handle_param = {0};

    if (PNULL != info_ptr)
    {
        handle = IMGPROC_CreateHandle(&handle_param);
        if (handle > 0)
        {
            info_ptr->handle = handle;
            ret = TRUE;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] CreateNInOneHandle ret=%d,handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_133_112_2_18_2_9_25_462,(uint8*)"dd",ret, handle);

    return ret;
}

/*****************************************************************************/
//  Description : destroy n in one handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyNInOneHandle(void)
{
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();
    IMGPROC_HANDLE handle = 0;
    uint32 ret = 0;

    if (PNULL != info_ptr)
    {
        handle = GetNInOneHandle();
        ret = IMGPROC_DestroyHandle(handle);

        info_ptr->handle = 0;
    }

    //SCI_TRACE_LOW:"[MMIDC] DestroyNInOneHandle ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_157_112_2_18_2_9_25_463,(uint8*)"d",ret);
}

/*****************************************************************************/
//  Description : create n in one osd info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateNInOneOSDInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = PNULL;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    DC_IMG_SIZE_T img_size = {0};
    uint32 buf_size = 0;
    uint8 *buf_ptr = PNULL;

    MMIDC_DATA_FORMAT_INFO_T format_info = {0};
    uint8 i = 0;
    uint8 total_num = MMIDC_N_IN_ONE_TOTAL_NUM;
    
    if (PNULL == info_ptr)
    {
        return ret;
    }

    MMIDC_GetDataFormatInfo(&format_info);
    osd_info_ptr = &info_ptr->osd_info;
    img_size.w = MMIDC_N_IN_ONE_IMG_WIDTH;
    img_size.h = MMIDC_N_IN_ONE_IMG_HEIGHT;

    
    osd_info_ptr->img_size = img_size;
    buf_size = img_size.w * img_size.h * format_info.data_unit;
    for (i = 0; i < total_num; i++)
    {
        buf_ptr = SCI_ALLOCA(buf_size);
        if (PNULL == buf_ptr)
        {
            break;
        }
        SCI_MEMSET(buf_ptr, 0x00, buf_size);

        osd_info_ptr->osd_data[i].buffer_ptr = buf_ptr;
        osd_info_ptr->osd_data[i].buffer_size = buf_size;
    }

    //check all to alloc successfully
    if (PNULL != osd_info_ptr->osd_data[total_num - 1].buffer_ptr)
    {
        ret = TRUE;

        info_ptr->total_num = total_num;
        info_ptr->hor_num = MMIDC_N_IN_ONE_LINE_NUM;
    }

    //SCI_TRACE_LOW:"[MMIDC] CreateNInOneOSDInfo img_size.w=%d, h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_214_112_2_18_2_9_25_464,(uint8*)"dd",img_size.w, img_size.h);
                  
    //SCI_TRACE_LOW:"[MMIDC] CreateNInOneOSDInfo ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_217_112_2_18_2_9_25_465,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
//  Description : destroy n in one osd info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyNInOneOSDInfo(void)
{
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = GetNInOneOSDInfoPtr();

    uint8 *buf_ptr = PNULL;
    int16 i = 0;

    
    if (PNULL != osd_info_ptr)
    {
        for (i = 0; i < MMIDC_N_IN_ONE_TOTAL_NUM; i++)
        {
            buf_ptr = osd_info_ptr->osd_data[i].buffer_ptr;
            if (PNULL != buf_ptr)
            {
                SCI_FREE(buf_ptr);
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] DestroyNInOneOSDInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_246_112_2_18_2_9_26_466,(uint8*)"");
}

/*****************************************************************************/
//  Description : get n in one photo info ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_N_IN_ONE_PHOTO_INFO_T* GetNInOnePhotoInfoPtr(void)
{
    MMIDC_N_IN_ONE_PHOTO_INFO_T *photo_info_ptr = PNULL;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    
    if (PNULL != info_ptr)
    {
        photo_info_ptr = &info_ptr->photo_info;
    }

    return photo_info_ptr;
}

/*****************************************************************************/
//  Description : create n in one photo info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateNInOnePhotoInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_N_IN_ONE_PHOTO_INFO_T *photo_info_ptr = PNULL;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    DC_IMG_SIZE_T img_size = {0};
    uint32 buf_size = 0;
    uint8 *buf_ptr = PNULL;

    MMIDC_DATA_FORMAT_INFO_T format_info = {0};
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);
    
    if (PNULL == info_ptr)
    {
        return ret;
    }

    MMIDC_GetDataFormatInfo(&format_info);

    photo_info_ptr = &info_ptr->photo_info;
    if ( (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type) 
          && (LCD_ANGLE_0 == logic_angle)
        )
    {
        img_size.w = 480;
        img_size.h = 640;
    }
    else
    {
        img_size.w = MMIDC_N_IN_ONE_PHOTO_WIDTH;
        img_size.h = MMIDC_N_IN_ONE_PHOTO_HEIGHT;
    }
    
    photo_info_ptr->img_size = img_size;
    buf_size = MMIDC_GetDataSizeForJPG(img_size.w, img_size.h);

    buf_ptr = SCI_ALLOCA(buf_size);
    if (PNULL != buf_ptr)
    {
        SCI_MEMSET(buf_ptr, 0x00, buf_size);

        photo_info_ptr->photo_data.buffer_ptr = buf_ptr;
        photo_info_ptr->photo_data.buffer_size = buf_size;

        ret = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIDC] CreateNInOnePhotoInfo buf_ptr=0x%X, buf_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_310_112_2_18_2_9_26_467,(uint8*)"dd",buf_ptr, buf_size);
                  
    //SCI_TRACE_LOW:"[MMIDC] CreateNInOnePhotoInfo ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_313_112_2_18_2_9_26_468,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
//  Description : destroy n in one photo info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyNInOnePhotoInfo(void)
{
    MMIDC_N_IN_ONE_PHOTO_INFO_T *photo_info_ptr = PNULL;

    uint8 *buf_ptr = PNULL;
    
    photo_info_ptr = GetNInOnePhotoInfoPtr();

    if (PNULL != photo_info_ptr)
    {
        buf_ptr = photo_info_ptr->photo_data.buffer_ptr;
        if (PNULL != buf_ptr)
        {
            SCI_FREE(buf_ptr);
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] DestroyNInOnePhotoInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_339_112_2_18_2_9_26_469,(uint8*)"");
}

/*****************************************************************************/
//  Description : open n in one
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenNInOne(void)
{
    BOOLEAN ret = TRUE;
    uint32 buf_size = 0;
    MMIDC_IMAGE_PROCESS_TYPE_E type = IMG_PROC_NINONE;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);
    
    do
    {
        /*Jessica: wait for integration
        IMG_PROC_OPEN_T img_proc_open;
        IMG_PROC_CREATE_T img_proc_create;
        uint32  img_proc_handle;
        
        if(0 != IMGPROC_GetCapabilty
        {
            return FALSE;
        }
        */
        
        //alloc memory
        buf_size = sizeof(*s_n_in_one_info_ptr);
        if (PNULL == s_n_in_one_info_ptr)
        {
            s_n_in_one_info_ptr = SCI_ALLOCA(buf_size);
            if (PNULL == s_n_in_one_info_ptr)
            {
                break;
            }
        }
        if (PNULL != s_n_in_one_info_ptr)
        {
            SCI_MEMSET(s_n_in_one_info_ptr, 0, buf_size);
        }

        //create handle
        ret = CreateNInOneHandle();
        if (!ret)
        {
            break;
        }

        ret = CreateNInOneOSDInfo();
        if (!ret)
        {
            break;
        }
        
        ret = CreateNInOnePhotoInfo();
        if (!ret)
        {
            break;
        }

        
        s_n_in_one_info_ptr->is_enable = TRUE;
        ret = TRUE;        
    }while(0);

    if (ret)
    {
        MMIDC_SetImageProcessType(type);

        if ( (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type) 
            && (LCD_ANGLE_0 == logic_angle)
            )
        {
            MMIAPIDC_SetPhotoSize(PHOTO_SIZE_240X320);
        }
        else
        {
            MMIAPIDC_SetPhotoSize(PHOTO_SIZE_320X240);
        }

        #if 0
        {
            CAMERA_ENG_VISUAL_MODE_E visual_mode = 0;

            visual_mode = MMIAPIDC_GetVisualMode();
            MMIDC_PushVisualMode(visual_mode);
            MMIAPIDC_SetVisualMode(CAMERA_ENG_FULL_VISUAL_ANGLE);
        }
        #endif
    }
    else
    {
        MMIDC_CloseNInOne();
    }


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_OpenNInOne ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_426_112_2_18_2_9_26_470,(uint8*)"d", ret);
    
    return ret;    
}

/*****************************************************************************/
//  Description : close n in one
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseNInOne(void)
{
#if 0
    if (MMIDC_IsNInOneEnable())
    {
        CAMERA_ENG_VISUAL_MODE_E visual_mode = 0;

        visual_mode = MMIDC_PopVisualMode();
        MMIAPIDC_SetVisualMode(visual_mode);
    }
#endif    
    
    DestroyNInOneHandle();

    DestroyNInOneOSDInfo();
    DestroyNInOnePhotoInfo();
    
    if (PNULL != s_n_in_one_info_ptr)
    {
        SCI_FREE(s_n_in_one_info_ptr);
    }


}
/*****************************************************************************/
//Description : check if n in one function opened
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsNInOneEnable(void)
{
    BOOLEAN is_enable = FALSE;    
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if (PNULL != info_ptr)
    {
        is_enable = info_ptr->is_enable;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsNInOneEnable is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_474_112_2_18_2_9_26_471,(uint8*)"d",is_enable);
    
    return is_enable;
}

/*****************************************************************************/
//Description : get n in one image size
//Global resource dependence : 
//Author: jessica.lu
//Note:
/*****************************************************************************/
LOCAL DC_IMG_SIZE_T GetNInOneImgSize(void)
{
    DC_IMG_SIZE_T size = {0};    
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if (PNULL != info_ptr)
    {
        size  = info_ptr->osd_info.img_size;
    }

    return size;
}

/*****************************************************************************/
//Description : get n in one start point
//Global resource dependence : 
//Author: jessica.lu
//Note:
/*****************************************************************************/
LOCAL GUI_POINT_T GetNInOneStartPoint(void)
{
    GUI_POINT_T point = {0};
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if (PNULL == info_ptr)
    {
        return point;
    }

    if(0 == info_ptr->start_point.x && 0 == info_ptr->start_point.y)
    {
        if(SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode())
        {
            point.x = MMIDC_N_IN_ONE_H_X;
            point.y = MMIDC_N_IN_ONE_H_Y;
        }
        else
        {
            point.x = MMIDC_N_IN_ONE_V_X;
            point.y = MMIDC_N_IN_ONE_V_Y;
        }
    }
    else
    {
        point.x = info_ptr->start_point.x;
        point.y = info_ptr->start_point.y;
    }
    
    return point;
}

/*****************************************************************************/
//Description : get n in one image rect
//Global resource dependence : 
//Author: jessica.lu
//Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetNInOneImgRect(uint8 img_index)
{
    GUI_RECT_T rect = {0}; 
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    DC_IMG_SIZE_T img_size = GetNInOneImgSize();
    uint8 line = 0;
    uint8 col = 0;
    uint8 total_num = 0;
    uint8 hor_num = 0;
    uint8 num_per_line = 0;
    GUI_POINT_T start_point = GetNInOneStartPoint();

    if (PNULL == info_ptr)
    {
        return rect;
    }
    
    total_num = info_ptr->total_num;
    hor_num = info_ptr->hor_num;
    
    if (img_index >= total_num)
    {
        return rect;
    }

    num_per_line = hor_num;
    
    line = img_index / num_per_line;
    col = img_index % hor_num;

    rect.top = start_point.y + (line + 1) * MMIDC_N_IN_ONE_BORDER_WIDTH + (line)* img_size.h; /*lint !e737*/
    rect.bottom = rect.top + img_size.h - 1; /*lint !e737*/
    rect.left = start_point.x + (col + 1) * MMIDC_N_IN_ONE_BORDER_WIDTH + (col)* img_size.w; /*lint !e737*/
    rect.right = rect.left + img_size.w - 1; /*lint !e737*/

    return rect;
}

/*****************************************************************************/
//Description : display n in one border
//Global resource dependence : 
//Author: jessica.lu
//Note:
/*****************************************************************************/
LOCAL void DisplayNInOneBorder(void)
{
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    uint8 line_num = 0;
    uint8 col_num = 0;
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    uint8 i = 0;
    uint8 j = 0;
    uint16 x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    GUI_POINT_T start_point = GetNInOneStartPoint();
    DC_IMG_SIZE_T img_size = GetNInOneImgSize();


    if (PNULL == info_ptr)
    {
        return ;
    }
    
    col_num = info_ptr->hor_num;
    if (col_num != 0)
    {
        line_num = (info_ptr->total_num + col_num - 1) / col_num;
    }

    //draw horizontal line
    x1 = start_point.x;
    y1 = start_point.y;
    x2 = x1 + (col_num + 1) * MMIDC_N_IN_ONE_BORDER_WIDTH + col_num * img_size.w - 1;
    for(i = 0; i <= line_num; i++)
    {
        for(j = 0; j < MMIDC_N_IN_ONE_BORDER_WIDTH; j++)
        {
            LCD_DrawHLine(&dev_info, x1, y1, x2, MMIDC_N_IN_ONE_BORDER_COLOR);
            y1++;
        }
        y1 += img_size.h;
    }

    //draw vertical line
    x1 = start_point.x;
    y1 = start_point.y;
    y2 = start_point.y + (line_num + 1) * MMIDC_N_IN_ONE_BORDER_WIDTH + line_num * img_size.h - 1; /*lint !e737*/
    for(i = 0; i <= col_num; i++)
    {
        for(j = 0; j < MMIDC_N_IN_ONE_BORDER_WIDTH; j++)
        {
            LCD_DrawVLine(&dev_info, x1, y1, y2, MMIDC_N_IN_ONE_BORDER_COLOR);
            x1++;
        }
        x1 += img_size.w;
    }
    
}

/*****************************************************************************/
//Description : display n in one preview image
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayNInOnePreviewImage(GUI_LCD_DEV_INFO dev_info, GUI_RECT_T rect)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    uint16 width = 0;
    uint16 height = 0;
    uint16 temp = 0;
    GUI_RECT_T img_rect = rect;
    GUI_POINT_T     dis_point = {0};
    
    
    GUIRES_GetImgWidthHeight(&width, &height, IMG_NINONE_PREVIEW, win_id);

    temp = img_rect.bottom - img_rect.top + 1;
    if(temp > height)
    {
        img_rect.top += (temp - height) / 2;
        img_rect.bottom = img_rect.top + height - 1;
    }

    temp = img_rect.right - img_rect.left + 1;
    if(temp > width)
    {
        img_rect.left += (temp - width) / 2;
        img_rect.right = img_rect.left + width - 1;
    }

    dis_point.x = img_rect.left;
    dis_point.y = img_rect.top;
    GUIRES_DisplayImg(&dis_point, PNULL, PNULL, win_id, IMG_NINONE_PREVIEW, &dev_info);       
}


/*****************************************************************************/
//Description : display n in one image
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DisplayNInOne(void)
{
    BOOLEAN ret = FALSE;
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    GUI_RECT_T rect = {0};
    uint8 i = 0;
    uint8 total_num = 0;
    GUIIMG_BITMAP_T  bitmap = {0};
    DC_IMG_SIZE_T img_size = GetNInOneImgSize();
    GUI_POINT_T     dis_point = {0};
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = GetNInOneOSDInfoPtr();
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if ((PNULL != osd_info_ptr)
        && (PNULL != info_ptr)
       )
    {
        DisplayNInOneBorder();

        total_num = info_ptr->total_num;
        for(i = 0;i < total_num;i++)
        {
            rect = GetNInOneImgRect(i);
            if ( osd_info_ptr->osd_data[i].is_enable
               && (PNULL != osd_info_ptr->osd_data[i].buffer_ptr)
               )
            {
                bitmap.bit_ptr = osd_info_ptr->osd_data[i].buffer_ptr;
                bitmap.img_width = img_size.w;
                bitmap.img_height = img_size.h;


                dis_point.x = rect.left;
                dis_point.y = rect.top;
                GUIIMG_DisplayBmp(FALSE,
                    &rect,
                    &dis_point,
                    &bitmap,
                    &dev_info);
            }
            else
            {
                LCD_FillRect(&dev_info, rect, MMIDC_N_IN_ONE_IMG_COLOR);
                DisplayNInOnePreviewImage(dev_info, rect);
            }
           
        }

        ret = TRUE;
    }
    
    return ret;
}
/*****************************************************************************/
//Description : get n in one start point
//Global resource dependence : 
//Author: jinju.ma
//Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMIDC_GetNInOneRectStartPoint(void)
{
	GUI_POINT_T point = {0};
	point = GetNInOneStartPoint();	
	
	return point;
}
/*****************************************************************************/
//Description: reset osd data info
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetOSDDataInfo(void)
{
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = GetNInOneOSDInfoPtr();
    uint8 i = 0;
    //uint8 cur_index = 0;
    uint8 total_num = 0;

    if ( (PNULL != info_ptr)
        && (PNULL != osd_info_ptr)
        )
    {
        osd_info_ptr->cur_index = 0;
        
        total_num = info_ptr->total_num;
        for(i = 0; i < total_num; i++)
        {
            osd_info_ptr->osd_data[i].is_enable = FALSE;
        }
    }
}

/*****************************************************************************/
//Description: process photo image callback
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
LOCAL int32 ProcessPhotoImageCallback(IMG_PROC_DATA_T *param_ptr)
{
    int32 ret = -1;

   // uint8 i = 0;
    uint8 cur_index = 0;
    uint8 total_num = 0;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = GetNInOneOSDInfoPtr();



    //if param_ptr is null, img proc is error,
    //so send fail msg
    if ((PNULL == param_ptr)
        || (PNULL == info_ptr)
        || (PNULL == osd_info_ptr)
        )
    {
        MMIDC_SetUCameraStatus(UCAMERA_STATUS_START);
    
        ResetOSDDataInfo();
        MMIDC_SendCaptureFailMsg(ret, PNULL, 0);
        return ret;
    }
    

    cur_index = osd_info_ptr->cur_index;
    total_num = info_ptr->total_num;

    if (cur_index == total_num)
    {
        ResetOSDDataInfo();
        
        ret = MMIDC_UCameraASyncCallbackSendMsgToApp(param_ptr);
    }
    else
    {
        MMIDC_SetUCameraStatus(UCAMERA_STATUS_START);
    }

    //SCI_TRACE_LOW:"[MMIDC] ProcessPhotoImageCallback cur_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_786_112_2_18_2_9_27_472,(uint8*)"d",cur_index);

    ret = 0;

    return ret;
}

/*****************************************************************************/
//Description: view osd image callback
//Global resource dependence:
//Author:robert.wang
//Note:
/*****************************************************************************/
LOCAL int32 ViewOsdImageCallback(IMG_PROC_PROCESS_RTN_T *img_proc_result_ptr, //in
                                        MMIDC_IMAGE_PROCESS_INFO_T *src_ptr,//in 
                                        MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_ptr //out
                                        )
{
    int32 ret = -1;
    IMG_PROC_DATA_T *proc_data_ptr = PNULL;

    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = GetNInOneOSDInfoPtr();
    MMIDC_BUFFER_DATA_T *osd_data_ptr = PNULL;
    uint8 cur_index = 0;
    uint8 total_num = 0;
    int16 i = 0;

    if ((PNULL == img_proc_result_ptr)
        || (PNULL == osd_info_ptr)
        || (PNULL == info_ptr)
       )
    {
        return ret;
    }

    //proc data
    proc_data_ptr = &img_proc_result_ptr->data_rtn.view_before[i++];

    total_num = info_ptr->total_num;
    cur_index = osd_info_ptr->cur_index;
    osd_data_ptr = &osd_info_ptr->osd_data[cur_index];

    if ((PNULL != proc_data_ptr)
        && (PNULL != osd_data_ptr)
        && (PNULL != osd_data_ptr->buffer_ptr)
        ) /*lint !e774*/
    {

        osd_data_ptr->is_enable = TRUE;


        //SCI_TRACE_LOW:"[MMIDC] ViewOsdImageCallback buffer_ptr=0x%X,buffer_size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_837_112_2_18_2_9_27_473,(uint8*)"dd",osd_data_ptr->buffer_ptr, osd_data_ptr->buffer_size);

        //SCI_TRACE_LOW:"[MMIDC] ViewOsdImageCallback W=%d, h=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_840_112_2_18_2_9_27_474,(uint8*)"dd",proc_data_ptr->size.w, proc_data_ptr->size.h);

      }


    osd_info_ptr->cur_index++;

    //last capture not show 
    if (osd_info_ptr->cur_index < total_num)
    {    
        MMIDC_DisplayNInOne();
        MMIDC_CompleteDisplayOSD();
    }
    
#if 1
    ProcessPhotoImageCallback(&img_proc_result_ptr->data_rtn.proc_img);

    ret = MMIDC_UCameraSyncCallback(img_proc_result_ptr,
                              src_ptr,
                              dst_ptr
                              );
#endif                              

    return ret;
}


/*****************************************************************************/
//Description : get n in one image process parameter
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL int32 GetNInOneImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                                 )
{
    int32 ret = -1;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();
    MMIDC_N_IN_ONE_OSD_INFO_T *osd_info_ptr = GetNInOneOSDInfoPtr();   
    
    DC_IMG_SIZE_T osd_img_size = {0};
    MMIDC_N_IN_ONE_PHOTO_INFO_T *photo_info_ptr = GetNInOnePhotoInfoPtr();

    IMG_PROC_PROCESS_T *img_proc_param_ptr = PNULL;
    IMG_PROC_NINONE_T *n_in_one_param_ptr = PNULL;

    
    uint8 i = 0;
    uint8 cur_index = 0;
    uint8 total_num = 0;


    MMIDC_PHOTO_SIZE_E photo_size = 0;
    MMIDC_SIZE_ATTRIBUTE_T photo_size_value = {0};

    
    if ((PNULL == info_ptr)
        || (PNULL == img_param_ptr)
        || (PNULL == photo_info_ptr)
        || (PNULL == osd_info_ptr)
       )
    {
        return ret;
    }


    img_proc_param_ptr = &img_param_ptr->img_proc_param;
    n_in_one_param_ptr = &info_ptr->n_in_one_param;

    if ((PNULL == img_proc_param_ptr)
        || (PNULL == n_in_one_param_ptr)
        ) /*lint !e774*/
    {
        return ret;
    }
    
    cur_index = osd_info_ptr->cur_index;       
    total_num = info_ptr->total_num;
    
    //osd info
    osd_img_size = osd_info_ptr->img_size;

    
    photo_size = MMIDC_GetPhotoSize();
    MMIDC_GetPhotoSizeWidthAndHeight(photo_size, &photo_size_value);


    //start image proc param to set
    img_param_ptr->handle = GetNInOneHandle();

    //n in one alg param
    n_in_one_param_ptr->all_img_num = info_ptr->total_num;
    n_in_one_param_ptr->h_img_num = info_ptr->hor_num;

    img_proc_param_ptr->proc_param_ptr = (void*)n_in_one_param_ptr;


    //osd info
    img_proc_param_ptr->view_before[i].rect.w = photo_size_value.size_width;
    img_proc_param_ptr->view_before[i].rect.h = photo_size_value.size_height;
    img_proc_param_ptr->view_before[i].data.format = IMG_PROC_RGB565;//IMG_PROC_YUV422;
    img_proc_param_ptr->view_before[i].data.addr.chn0 = osd_info_ptr->osd_data[cur_index].buffer_ptr;
    img_proc_param_ptr->view_before[i].data.addr.chn0_len = osd_info_ptr->osd_data[cur_index].buffer_size;

    img_proc_param_ptr->view_before[i].data.size.w = osd_img_size.w;
    img_proc_param_ptr->view_before[i].data.size.h = osd_img_size.h;
    //img_proc_param_ptr->view_before[i].ctrl_callback = ViewOsdImageCallback;
    i++;


    //photo info
    img_proc_param_ptr->proc_img.rect.w = photo_info_ptr->img_size.w;
    img_proc_param_ptr->proc_img.rect.h = photo_info_ptr->img_size.h;
    img_proc_param_ptr->proc_img.data.format = IMG_PROC_JPG;
    img_proc_param_ptr->proc_img.data.addr.chn0 = photo_info_ptr->photo_data.buffer_ptr;
    img_proc_param_ptr->proc_img.data.addr.chn0_len = photo_info_ptr->photo_data.buffer_size;
    img_proc_param_ptr->proc_img.data.size.w = photo_info_ptr->img_size.w;
    img_proc_param_ptr->proc_img.data.size.h = photo_info_ptr->img_size.h;
    //g_proc_param_ptr->proc_img.ctrl_callback = ProcessPhotoImageCallback;


    if (0 == cur_index)
    {
        img_proc_param_ptr->proc_mode = IMG_PROC_FIRST;
        img_proc_param_ptr->proc_mode |= IMG_PROC_PROCESS_EB;
        img_proc_param_ptr->proc_mode |= IMG_PROC_VIDW_BEFORE0_EB;
        
        MMIDC_SetPhotoDataFlag(FALSE);
    }
    else if (cur_index < (total_num - 1))
    {
        img_proc_param_ptr->proc_mode = IMG_PROC_UPDATE_SRC;
        img_proc_param_ptr->proc_mode |= IMG_PROC_PROCESS_EB;
        img_proc_param_ptr->proc_mode |= IMG_PROC_VIDW_BEFORE0_EB;
    }
    else//4     
    {
        img_proc_param_ptr->proc_mode = IMG_PROC_PROCESS_EB;
        img_proc_param_ptr->proc_mode |= IMG_PROC_END;
    }
    
    img_param_ptr->sync_callback = ViewOsdImageCallback; /*lint !e64*/
 
    ret = 0;
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetNInOneImageProcessParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_982_112_2_18_2_9_27_475,(uint8*)"");

    return ret;
}


/*****************************************************************************/
//Description : get n in one image process parameter
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetNInOneImageProcessParam(MMIDC_IMAGE_PROCESS_PARAM_T *img_param_ptr /*out*/
                                                            )
{
    int32 ret = -1;

    BOOLEAN is_skip_image_process = FALSE;
    MMIDC_IMAGE_PROCESS_INFO_T *src_info_ptr = PNULL;
    MMIDC_IMAGE_PROCESS_OUT_INFO_T *dst_info_ptr = PNULL;


    if (PNULL == img_param_ptr)
    {
        return ret;
    }
    
    src_info_ptr = img_param_ptr->src_info_ptr;
    dst_info_ptr = img_param_ptr->dst_info_ptr;
    
    if ((PNULL == src_info_ptr)
        || (PNULL == dst_info_ptr)
        )
    {
        return ret;
    }

    
     //若是预览数据，直接跳过image process
    if (UCAMERA_DATA_ROUTE_PREVIEW == src_info_ptr->data_from)
    {
        ret = 0;
        is_skip_image_process = TRUE;
    }
    else
    {
        //获得image process 参数
        is_skip_image_process = FALSE;

        ret = GetNInOneImageProcessParam(img_param_ptr);
    }

    dst_info_ptr->is_skip_image_process = is_skip_image_process;


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetNInOneImageProcessParam ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_1036_112_2_18_2_9_27_476,(uint8*)"d",ret);
                  
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetNInOneImageProcessParam is_skip_image_process=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_NINONE_1039_112_2_18_2_9_27_477,(uint8*)"d",is_skip_image_process);
                  
    return ret;
}


/*****************************************************************************/
//Description : get n in one image number
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint8 MMIDC_GetNInOneImageNumber(void)
{
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if(PNULL == info_ptr)
    {
        return 0;
    }
    
    return info_ptr->osd_info.cur_index;
}

/*****************************************************************************/
//Description : set n in one start point
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNInOneStartPoint(GUI_POINT_T point)
{
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if(PNULL != info_ptr)
    {
        info_ptr->start_point.x = point.x;
        info_ptr->start_point.y = point.y;
    }
}

/*****************************************************************************/
//Description : get n in one display width and height
//Global resource dependence : 
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetNInOneDisplayWidthAndHeight(int16 *width, int16 *height)
{
    int16 col_num = (int16)((MMIDC_N_IN_ONE_TOTAL_NUM + MMIDC_N_IN_ONE_LINE_NUM - 1) / MMIDC_N_IN_ONE_LINE_NUM);

    if(PNULL != width)
    {
        *width = MMIDC_N_IN_ONE_IMG_WIDTH * MMIDC_N_IN_ONE_LINE_NUM
                    + (MMIDC_N_IN_ONE_LINE_NUM + 1) * MMIDC_N_IN_ONE_BORDER_WIDTH;
    }
    if(PNULL != height)
    {
        *height = MMIDC_N_IN_ONE_IMG_HEIGHT * col_num + (col_num + 1) * MMIDC_N_IN_ONE_BORDER_WIDTH;
    }
}

/*****************************************************************************/
//Description : is n in one start display frame
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsNInOneStartDisplayFrame(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if(PNULL != info_ptr)
    {
        ret = info_ptr->is_start_display_frame;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_IsNInOneDisplayFrame ret=%d", ret);
    return ret;
}

/*****************************************************************************/
//Description : set n in one start display frame
//Global resource dependence : 
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetNInOneStartDisplayFrame(BOOLEAN flag)
{
    MMIDC_N_IN_ONE_INFO_T *info_ptr = GetNInOneInfoPtr();

    if(PNULL != info_ptr)
    {
        info_ptr->is_start_display_frame = flag;
    }
}

#endif //#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_N_IN_ONE)

