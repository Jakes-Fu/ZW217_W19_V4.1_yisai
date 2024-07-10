/*****************************************************************************
** File Name:      guiimg.c                                                  *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2007        Jassmine              Creat
******************************************************************************/

#ifndef _GUIIMG_C_
#define _GUIIMG_C_

/*lint -e775*/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "mmk_app.h"
#include "guiimg.h"
#include "../../../../mmi_ctrl/source/anim/h/ctrlanim.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guigraph.h"
#include "mmi_theme.h"
#include "img_dec_interface.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIIMG_BMP_HEAD     0x4D42  /*!<"MB"*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : combine two bytes into one uint16
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void BmpGetU16(
                     GUIIMG_HEAD_BUF_T  *head_ptr,  //in/out:
                     uint16             *dst_u16_ptr//in/out:
                     );

/*****************************************************************************/
// 	Description : combine four bytes into one uint32
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void BmpGetU32(
                     GUIIMG_HEAD_BUF_T  *head_ptr,  //in/out:
                     uint32             *dst_u32_ptr//in/out:
                     );

/*****************************************************************************/
// 	Description : combine four bytes into one uint32
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL uint32 WbmpReadMultiBytes(
                                GUIIMG_HEAD_BUF_T   *head_ptr,  //in/out:
                                uint32              *dst_ptr    //in/out:
                                );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : decode jpg,bmp,wbmp,png gif etc. one frame image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_Decode(
                             GUIIMG_SRC_T       *src_ptr,       //in:image info
                             GUIIMG_DISPLAY_T   *display_ptr,   //in:
                             GUIIMG_DEC_OUT_T   *output_ptr     //in/out:
                             )
{
    BOOLEAN                 result = FALSE;
    uint32                  target_buf_size = 0;
    uint32                  img_handle = 0;
    GUIANIM_TARGET_T        dec_target = {0};
    GUIANIM_IMG_DATA_T      src_data = {0};
    GUIANIM_DEC_RESULT_T    dec_result = {0};
    GUI_RECT_T              rect = {0};
    GUIANIM_IMG_INFO_T      img_info = {0};

    if (PNULL == src_ptr || PNULL == display_ptr || PNULL == output_ptr
        || PNULL == output_ptr->decode_data_ptr
        || 0 >= output_ptr->decode_data_size)
    {
        return result;
    }

    //set src data
    src_data.src_buf_ptr   = src_ptr->src_buf_ptr;
    src_data.src_data_size = src_ptr->src_data_size;

    //set target data type and target buffer size
    if (display_ptr->is_handle_transparent)
    {
        dec_target.target_type = GUIIMG_DATE_TYPE_ARGB888;
        target_buf_size = display_ptr->dest_width * display_ptr->dest_height * 4;
    }
    else
    {
        dec_target.target_type = GUIIMG_DATE_TYPE_RGB565;
        target_buf_size = display_ptr->dest_width * display_ptr->dest_height * 2;
    }
    rect.left   = 0;
    rect.right  = display_ptr->dest_width  ;
    rect.top    = 0;
    rect.bottom = display_ptr->dest_height ;

    //creat handle
    if ((output_ptr->decode_data_size >= target_buf_size) &&
        (GUIANIM_RESULT_SUCC == GUIANIM_CreateImgHandlAndGetInfo(src_ptr->full_path_ptr,&src_data,&img_info,&rect,&img_handle)))
    {
        //set decode target
        dec_target.is_exist_bg	   = display_ptr->is_exist_bg;
        dec_target.target_width    = (uint16)(display_ptr->dest_width);
        dec_target.target_height   = (uint16)(display_ptr->dest_height);
        if (GUIANIM_TYPE_IMAGE_ICO == img_info.type)
        {
            dec_target.target_width    = (uint16)(img_info.img_width);
            dec_target.target_height   = (uint16)(img_info.img_height);
            dec_target.ico_frame_index = img_info.ico_frame_index;
        }
        dec_target.target_buf_ptr  = output_ptr->decode_data_ptr;
        dec_target.target_buf_size = output_ptr->decode_data_size;
        dec_target.alpha_buf_ptr   = src_ptr->alpha_buf_ptr;
        dec_target.alpha_buf_size  = src_ptr->alpha_buf_size;

        if (PNULL != display_ptr->crop_rect_ptr)
        {
            dec_target.crop_rect.left   = display_ptr->crop_rect_ptr->left;
            dec_target.crop_rect.top    = display_ptr->crop_rect_ptr->top;
            dec_target.crop_rect.right  = display_ptr->crop_rect_ptr->right;
            dec_target.crop_rect.bottom = display_ptr->crop_rect_ptr->bottom;
        }
        
        //decode image
        if (GUIANIM_RESULT_SUCC == GUIANIM_GetFrame(TRUE,img_handle,0,&dec_target,&dec_result))
        {
            output_ptr->is_handle_alpha = dec_result.is_handle_alpha;
            output_ptr->actual_width    = dec_result.img_width;
            output_ptr->actual_height   = dec_result.img_height;
            output_ptr->data_type       = dec_result.data_type;

            result = TRUE;
        }        
    }
    else
    {
        //SCI_TRACE_LOW:"GUIIMG_Decode: decode_data_size 0x%x < target_buf_size 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIMG_152_112_2_18_3_19_30_196,(uint8*)"dd",output_ptr->decode_data_size,target_buf_size);
    }
    if (0 != img_handle )
    {
        //destroy handle
        GUIANIM_DestroyHandle(FALSE,img_handle,0);
    }
    return (result);
}

/*****************************************************************************/
//  Description : get image width and height by image data buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetImgInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *img_info_ptr   //in/out:
                                 )
{
    BOOLEAN             result = FALSE;
    IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != img_info_ptr); /*assert verified*/
    if (PNULL == img_info_ptr)
    {
        return FALSE;
    }

    //set decode src info
    dec_src.src_ptr       = data_ptr;  /*lint !e413*/
    dec_src.src_file_size = data_size; /*lint !e413*/

    //get image info
    get_result = IMG_DEC_GetInfo(&dec_src,&dec_info);
    //SCI_TRACE_LOW:"GUIIMG_GetImgInfo:get info result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIMG_186_112_2_18_3_19_30_197,(uint8*)"d",get_result);
    if (IMG_DEC_RET_SUCCESS == get_result)
    {
        //set image info
        switch (dec_info.img_type)
        {
        case IMG_DEC_TYPE_BMP:
            img_info_ptr->image_width  = dec_info.img_detail_info.bmp_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.bmp_info.img_height;
            break;

        case IMG_DEC_TYPE_WBMP:
            img_info_ptr->image_width  = dec_info.img_detail_info.wbmp_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.wbmp_info.img_height;
            break;

        case IMG_DEC_TYPE_JPEG:
            img_info_ptr->image_width  = dec_info.img_detail_info.jpeg_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.jpeg_info.img_heigth;
            break;

        case IMG_DEC_TYPE_PNG:
            img_info_ptr->image_width  = dec_info.img_detail_info.png_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.png_info.img_height;
            break;

        case IMG_DEC_TYPE_GIF:
            img_info_ptr->image_width  = dec_info.img_detail_info.gif_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.gif_info.img_height;
            break;

		case IMG_DEC_TYPE_ICO: // 暂时不支持，直接返回0
            img_info_ptr->image_width  = 0;
            img_info_ptr->image_height = 0;
			SCI_TRACE_LOW("GUIIMG_GetImgInfo: img_type = IMG_DEC_TYPE_ICO!");
			break;

        case IMG_DEC_TYPE_UNKNOWN:
			SCI_TRACE_LOW("GUIIMG_GetImgInfo: img_type = IMG_DEC_TYPE_UNKNOWN!");
            break;
        
        default:
			SCI_TRACE_LOW("GUIIMG_GetImgInfo: img_type %d is error!",dec_info.img_type);
            // SCI_PASSERT(FALSE,("GUIIMG_GetImgInfo:img_type %d is error!",dec_info.img_type)); /*assert verified*/
            break;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get bmp image info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetBmpInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *bmp_info_ptr   //in/out:
                                 )
{
    BOOLEAN             result = FALSE;
    uint16              dst_u16 = 0;
    uint32              dst_u32 = 0;
    GUIIMG_HEAD_BUF_T   head_buf = {0};

    if (0 < data_size && PNULL != data_ptr && PNULL != bmp_info_ptr)
    {
        //set head buffer
        head_buf.head_ptr = data_ptr;
        head_buf.len      = data_size;
        head_buf.offset   = 0;

        //get Type
        BmpGetU16(&head_buf,&dst_u16);
        if (GUIIMG_BMP_HEAD == dst_u16)
        {
            //size
            BmpGetU32(&head_buf,&dst_u32);

            //reserved1 and reserved2
            BmpGetU16(&head_buf,&dst_u16);
	        BmpGetU16(&head_buf,&dst_u16);

            //offbits
	        BmpGetU32(&head_buf,&dst_u32);

            //biSize
            BmpGetU32(&head_buf,&dst_u32);

            //width
	        BmpGetU32(&head_buf,&dst_u32);
            bmp_info_ptr->image_width = (uint16)(dst_u32);

            //height
	        BmpGetU32(&head_buf,&dst_u32);
            bmp_info_ptr->image_height = (uint16)(dst_u32);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : combine two bytes into one uint16
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void BmpGetU16(
                     GUIIMG_HEAD_BUF_T  *head_ptr,  //in/out:
                     uint16             *dst_u16_ptr//in/out:
                     )
{
    uint8   *src_buf_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != head_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != dst_u16_ptr);   /*assert verified*/
    if ((PNULL == head_ptr)||(PNULL == dst_u16_ptr))
    {
        return;
    }

    //set buffer
    src_buf_ptr = head_ptr->head_ptr + head_ptr->offset;

    //set uint16
    *dst_u16_ptr = (uint16)((src_buf_ptr[1]<<8) | src_buf_ptr[0]);

    head_ptr->offset = (uint16)(head_ptr->offset + 2);
}

/*****************************************************************************/
// 	Description : combine four bytes into one uint32
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void BmpGetU32(
                     GUIIMG_HEAD_BUF_T  *head_ptr,  //in/out:
                     uint32             *dst_u32_ptr//in/out:
                     )
{
    uint8   *src_buf_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != head_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != dst_u32_ptr);   /*assert verified*/
    if ((PNULL == head_ptr)||(PNULL == dst_u32_ptr))
    {
        return;
    }

    //set buffer
    src_buf_ptr = head_ptr->head_ptr + head_ptr->offset;

    //set uint32
    *dst_u32_ptr = (uint32)((src_buf_ptr[3]<<24) | (src_buf_ptr[2]<<16) | (src_buf_ptr[1]<<8) | src_buf_ptr[0]);

    head_ptr->offset = (uint16)(head_ptr->offset + 4);
}

/*****************************************************************************/
// 	Description : get wbmp image info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetWbmpInfo(
                                  uint8         *data_ptr,      //in:
                                  uint32        data_size,      //in:
                                  GUIIMG_INFO_T *wbmp_info_ptr  //in/out:
                                  )
{
    BOOLEAN             result = FALSE;
    uint32              width = 0;
    uint32              height = 0;
    GUIIMG_HEAD_BUF_T   head_buf = {0};

    if (0 < data_size && PNULL != data_ptr && PNULL != wbmp_info_ptr)
    {
        //set head buffer
        head_buf.head_ptr = data_ptr;
        head_buf.len      = data_size;
        head_buf.offset   = 2;

        //width
        if (4 >= WbmpReadMultiBytes(&head_buf,&width))
        {
            //height
            if (4 >= WbmpReadMultiBytes(&head_buf,&height))
            {
                if (0 < width * height)
                {
                    //set width and height
                    wbmp_info_ptr->image_width = (uint16)(width);
                    wbmp_info_ptr->image_height = (uint16)(height);

                    result = TRUE;
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : combine four bytes into one uint32
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL uint32 WbmpReadMultiBytes(
                                GUIIMG_HEAD_BUF_T   *head_ptr,  //in/out:
                                uint32              *dst_ptr    //in/out:
                                )
{
    uint32  read_bytes_num = 0;
    uint32  start_offset = 0;

    //set start offset
    start_offset = head_ptr->offset;

    while (0 != (*(head_ptr->head_ptr+head_ptr->offset) & 0x80))
	{
		*dst_ptr = ((*dst_ptr)<<7) + (*(head_ptr->head_ptr+head_ptr->offset) & 0x7F);
		head_ptr->offset += 1;
	}
	*dst_ptr = ((*dst_ptr)<<7) + (*(head_ptr->head_ptr+head_ptr->offset) & 0x7F);
	head_ptr->offset += 1;
    
    //set read bytes number
    read_bytes_num = head_ptr->offset - start_offset;

    return (read_bytes_num);
}

/*****************************************************************************/
// 	Description : get png image info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetPngInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *png_info_ptr   //in/out:
                                 )
{
    BOOLEAN     result = FALSE;
    uint32      data_offset = 0;

    if (PNULL == data_ptr || PNULL == png_info_ptr)
    {
        return result;
    }

    //judge len
    if (data_size >= (8+8+13))
    {
        //judge png head
        if((0x89 ==data_ptr[0]) && (0x50 ==data_ptr[1]) && (0x4e ==data_ptr[2]) && (0x47 ==data_ptr[3]) &&
            (0x0d ==data_ptr[4]) && (0x0a ==data_ptr[5]) && (0x1a ==data_ptr[6]) && (0x0a ==data_ptr[7]))
        {
            data_offset = 8 + 8;

            //get png image width
            png_info_ptr->image_width = (uint16)((data_ptr[data_offset] << 24) + (data_ptr[data_offset+1] << 16) +
                (data_ptr[data_offset+2] << 8) + data_ptr[data_offset+3]);

            data_offset += 4;

            //get png image height
            png_info_ptr->image_height = (uint16)((data_ptr[data_offset] << 24) + (data_ptr[data_offset+1] << 16) +
                (data_ptr[data_offset+2] << 8) + data_ptr[data_offset+3]);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get gif image info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetGifInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *gif_info_ptr   //in/out:
                                 )
{
    BOOLEAN     result = FALSE;
    uint32      data_offset = 0;

    if (PNULL == data_ptr || PNULL == gif_info_ptr)
    {
        return result;
    }

    //judge len
    if (data_size >= (6+4))//gif87a/gif89a
    {
        //judge gif head
        if (0x47 == data_ptr[0] && 0x49 == data_ptr[1] && 0x46 == data_ptr[2])//gif
        {
            data_offset = 6;

            //get gif image width
            gif_info_ptr->image_width = (uint16)((data_ptr[data_offset+1] << 8) + data_ptr[data_offset]);

            data_offset += 2;

            //get gif image height
            gif_info_ptr->image_height = (uint16)((data_ptr[data_offset+1] << 8) + data_ptr[data_offset]);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display bmp bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_DisplayBmp(
                                 BOOLEAN                is_horizontal,  //in:is horizontal display
                                 GUI_RECT_T             *rect_ptr,      //in:display rect
                                 GUI_POINT_T            *point_ptr,     //in:display start point
                                 GUIIMG_BITMAP_T        *bitmap_ptr,    //in:
                                 GUI_LCD_DEV_INFO const *lcd_dev_ptr    //in:
                                 )
{
    BOOLEAN                     result = FALSE;
    GUIIMG_DISPLAYBMP_PARAM_T   display_param = {0};

    display_param.is_horizontal = is_horizontal;
    display_param.rect_ptr = rect_ptr;
    display_param.point_ptr = point_ptr;
    display_param.bitmap_ptr = bitmap_ptr;
    display_param.lcd_dev_ptr = lcd_dev_ptr;
    display_param.is_transparent_mode = FALSE;
    display_param.transparent_color = 0;
    result = GUIIMG_DisplayBmpEx(&display_param);

    return (result);
}

/*****************************************************************************/
//  Description : display bmp bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_DisplayBmpEx(
                                   GUIIMG_DISPLAYBMP_PARAM_T *param_ptr
                                   )
{
    BOOLEAN         result = TRUE;
    uint16          logic_lcd_width = 0;
    uint16          logic_lcd_height = 0;
    uint16          mem_width = 0;
    uint16          show_width = 0;
    uint16          show_height = 0;
    uint16          img_width = 0;
    uint16          img_height = 0;
    uint16          img_x = 0;
    uint16          img_y = 0;
    uint16          *img_buf_ptr = PNULL;
    GUI_RECT_T      show_rect = {0};
    GUI_RECT_T      img_rect = {0};
    GUI_COLOR_T     *lcd_buf_ptr = PNULL;
    GUI_COLOR_T     *temp_lcd_buf_ptr = PNULL;
    GUI_RECT_T      display_rect = {0};
    GUI_POINT_T     display_point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};
    BOOLEAN                is_horizontal = FALSE;  //in:is horizontal display
    GUI_RECT_T             *rect_ptr = PNULL;      //in:display rect
    GUI_POINT_T            *point_ptr = PNULL;     //in:display start point
    GUIIMG_BITMAP_T        *bitmap_ptr = PNULL;    //in:
    GUI_LCD_DEV_INFO const *lcd_dev_ptr = PNULL;    //in:
    BOOLEAN         is_transparent_mode = FALSE;
    GUI_COLOR_T     transparent_color = 0;

    if (PNULL == param_ptr)
    {
        return FALSE;
    }
    is_horizontal = param_ptr->is_horizontal;
    rect_ptr = param_ptr->rect_ptr;
    point_ptr = param_ptr->point_ptr;
    bitmap_ptr = param_ptr->bitmap_ptr;
    lcd_dev_ptr = param_ptr->lcd_dev_ptr;
    is_transparent_mode = param_ptr->is_transparent_mode;
    transparent_color = param_ptr->transparent_color;    
    //相对于lcd的坐标转换为相对于图层的坐标
    point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)lcd_dev_ptr, point_ptr, &display_point);
    rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)lcd_dev_ptr, rect_ptr, &display_rect);
    
    if (!LCD_PrepareDrawParam(lcd_dev_ptr, &draw_dc))
    {
        return FALSE;
    }

    logic_lcd_width  = (uint16)draw_dc.width;
    logic_lcd_height = (uint16)draw_dc.height;
    mem_width = (uint16)draw_dc.mem_width;
    lcd_buf_ptr = (GUI_COLOR_T *)draw_dc.lcd_buffer_ptr;

    //set show rect
    show_rect.right  = (uint16)(logic_lcd_width - 1);
    show_rect.bottom = (uint16)(logic_lcd_height - 1);
    
    //modify show rect
    if ((PNULL != rect_ptr) && 
        (!GUI_IntersectRect(&show_rect,show_rect,*rect_ptr)))
    {
        //SCI_TRACE_LOW:"GUIIMG_DisplayBmp:display rect is not on lcd!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIMG_592_112_2_18_3_19_31_198,(uint8*)"");
        result = FALSE;
    }

    if (result)
    {
        if (is_horizontal)
        {
            img_width  = bitmap_ptr->img_height;
            img_height = bitmap_ptr->img_width;
        }
        else
        {
            img_width  = bitmap_ptr->img_width;
            img_height = bitmap_ptr->img_height;
        }

        //set image rect
        img_rect.left   = point_ptr->x;
        img_rect.top    = point_ptr->y;
        img_rect.right  = (int16)(img_rect.left + img_width - 1);
        img_rect.bottom = (int16)(img_rect.top + img_height - 1);

        //modify show rect
        if (!GUI_IntersectRect(&show_rect,show_rect,img_rect))
        {
            //SCI_TRACE_LOW:"GUIIMG_DisplayBmp:image rect is not cross with show rect!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIMG_618_112_2_18_3_19_31_199,(uint8*)"");
            result = FALSE;
        }

        if (result)
        {
            if (PNULL == lcd_buf_ptr)
            {
                return FALSE;
            }

            //set temp lcd buffer
            temp_lcd_buf_ptr = lcd_buf_ptr + show_rect.top*mem_width + show_rect.left;

            //set show image width and height
            show_width  = (uint16)(show_rect.right - show_rect.left + 1);
            show_height = (uint16)(show_rect.bottom - show_rect.top + 1);

            img_x = (uint16)(show_rect.left - img_rect.left);
            img_y = (uint16)(show_rect.top - img_rect.top);

            switch (bitmap_ptr->date_type)
            {
            case GUIIMG_DATE_TYPE_RGB565:
                if (is_horizontal)
                {
                    uint16  *dst_temp_ptr = PNULL;
                    uint16  *dst_img_ptr = PNULL;
                    uint32  i = 0;
                    uint32  j = 0;
                
                    dst_temp_ptr = (uint16 *)(temp_lcd_buf_ptr + show_width - 1);

                    //set image buffer
                    dst_img_ptr = bitmap_ptr->bit_ptr;
                    dst_img_ptr = dst_img_ptr + (img_width-img_x-show_width)*img_height + img_y;

                    for (i=0; i<show_width; i++)
                    {
                        temp_lcd_buf_ptr = dst_temp_ptr - i;

                        img_buf_ptr = dst_img_ptr;
                        for (j=0; j<show_height; j++)
                        {
                            *temp_lcd_buf_ptr = *img_buf_ptr++;
                            temp_lcd_buf_ptr += mem_width;
                        }
                        dst_img_ptr += img_height;
                    }
                }
                else
                {
                    //set image buffer
                    img_buf_ptr = bitmap_ptr->bit_ptr;
                    img_buf_ptr = img_buf_ptr + img_y*img_width + img_x;
#ifdef UI_MULTILAYER_SUPPORT
                    switch (UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
                    {
                    case DATA_TYPE_ARGB888:
                        {
                            uint32 *u32_temp_lcd_buf_ptr = PNULL;
                            uint32 i = 0;
                            uint32 j = 0;
                            uint16 temp_data = 0;

                            u32_temp_lcd_buf_ptr = (uint32*)lcd_buf_ptr;
                            u32_temp_lcd_buf_ptr += show_rect.top*mem_width + show_rect.left;
                            for (i = 0; i < show_height; i++)
                            {
                                for (j = 0; j < show_width; j++)
                                {
                                    temp_data = *img_buf_ptr;
        //                            temp_data = ((temp_data<<8)&0xff00) | ((temp_data>>8)&0xff);
                                    if (!is_transparent_mode || (transparent_color != temp_data))
                                    {
									    *u32_temp_lcd_buf_ptr = ConvertRgb565ToArgb888(temp_data, 0xff);
                                    }
                                    u32_temp_lcd_buf_ptr++;
                                    img_buf_ptr++;
                                }
                                u32_temp_lcd_buf_ptr += (mem_width-show_width);
                                img_buf_ptr += (img_width-show_width);
                            }
                        }
                        break;

                    case DATA_TYPE_RGB565:
                    default:
#endif
                        while (show_height)
                        {
                            SCI_MEMCPY(temp_lcd_buf_ptr,img_buf_ptr,(show_width*2));
                            temp_lcd_buf_ptr += mem_width;
                            img_buf_ptr += img_width;
                            show_height--;
                        }
#ifdef UI_MULTILAYER_SUPPORT
                        break;
                    }
#endif
                }
                break;

            case GUIIMG_DATE_TYPE_ARGB888:
#ifdef UI_MULTILAYER_SUPPORT
                switch (UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
                {
                case DATA_TYPE_ARGB888:
                    {
                        uint32  *src_buf_ptr = PNULL;
                        uint32  *u32_temp_lcd_buf_ptr = PNULL;
                        uint32  *temp_src_ptr = PNULL;
                        uint32  *temp_dst_ptr = PNULL;
                        uint16  temp_width = 0;

                        src_buf_ptr = bitmap_ptr->bit_ptr;
                        src_buf_ptr = src_buf_ptr + img_y*img_width + img_x;

                        u32_temp_lcd_buf_ptr = (uint32*)lcd_buf_ptr;
                        u32_temp_lcd_buf_ptr += show_rect.top*mem_width + show_rect.left;

                        while (show_height)
        	            {
                            temp_src_ptr = src_buf_ptr;
                            temp_dst_ptr = u32_temp_lcd_buf_ptr;

                            temp_width = show_width;
                            while (temp_width)
                            {
                                *temp_dst_ptr = GUI_CompoundARGB888(*temp_dst_ptr, *temp_src_ptr++);

                                temp_dst_ptr++;
                                temp_width--;
                            }

                            src_buf_ptr += img_width;
                            u32_temp_lcd_buf_ptr += mem_width;

                            show_height--;
                        }
                    }
                    break;

                case DATA_TYPE_RGB565:
                default:
#endif
                    {
                        uint8       alpha = 0;
                        uint8       h_r = 0,l_r = 0;
                        uint8       h_g = 0,l_g = 0;
                        uint8       h_b = 0,l_b = 0;
                        uint8       r = 0,g = 0,b = 0; 
                        uint16      temp_width = 0;
                        uint16      dst_value = 0;
                        uint16      *temp_dst_ptr = PNULL;
                        uint32      src_value = 0;
                        uint32      *src_buf_ptr = PNULL;
                        uint32      *temp_src_ptr = PNULL;
                        
                        //set image buffer
                        src_buf_ptr = bitmap_ptr->bit_ptr;
                        src_buf_ptr = src_buf_ptr + img_y*img_width + img_x;

                        while (show_height)
        	            {
                            temp_src_ptr = src_buf_ptr;
                            temp_dst_ptr = temp_lcd_buf_ptr;

                            temp_width = show_width;
                            while (temp_width)
                            {
                                src_value = *temp_src_ptr++;

                                alpha = (uint8)(src_value>>24);
                                if (0 == alpha)
                                {
                                    temp_dst_ptr++;
                                }
                                else if ((255 == alpha) || (254 == alpha))
                                {
                                    *temp_dst_ptr++ = (uint16)RGB8882RGB565(src_value);
                                }
                                else
                                {
                                    h_r = (uint8)((src_value&0x00ff0000)>>16);
                                    h_g = (uint8)((src_value&0x0000ff00)>>8);
                                    h_b = (uint8)(src_value&0x000000ff);

                                    dst_value = *temp_dst_ptr;
                                    l_r = (uint8)((dst_value&0xf800)>>8);
                                    l_g = (uint8)((dst_value&0x07e0)>>3);
                                    l_b = (uint8)((dst_value&0x001f)<<3);
                        
                                    r = (uint8)(((l_r<<8)+alpha*(h_r-l_r)+128)>>8);
                                    g = (uint8)(((l_g<<8)+alpha*(h_g-l_g)+128)>>8);
                                    b = (uint8)(((l_b<<8)+alpha*(h_b-l_b)+128)>>8);
                                    dst_value = (uint16)(((r&0xf8)<<8) | ((g&0xfc)<<3) | (b>>3));

                                    *temp_dst_ptr++ = dst_value;
                                }

                                temp_width--;
                            }

                            src_buf_ptr += img_width;
                            temp_lcd_buf_ptr += mem_width;
                        
                            show_height--;
                        }
                    }
#ifdef UI_MULTILAYER_SUPPORT
                    break;
                }
#endif
                
                break;

            default:
                break;
            }

            //store rect
            MMITHEME_StoreUpdateRect(lcd_dev_ptr,show_rect);
        }
    }

    return (result);
}

#ifdef IM_ENGINE_SOGOU
PUBLIC BOOLEAN GUIIMG_DisplayBmpDst(GUIIMG_DISPLAYBMP_PARAM_T *param_ptr, 
                                    void *dc_data_ptr)
{
    BOOLEAN         result = TRUE;
    uint16          logic_lcd_width = 0;
    uint16          logic_lcd_height = 0;
    uint16          mem_width = 0;
    uint16          show_width = 0;
    uint16          show_height = 0;
    uint16          img_width = 0;
    uint16          img_height = 0;
    uint16          img_x = 0;
    uint16          img_y = 0;
    uint16          *img_buf_ptr = PNULL;
    GUI_RECT_T      show_rect = {0};
    GUI_RECT_T      img_rect = {0};
    GUI_COLOR_T     *lcd_buf_ptr = PNULL;
    GUI_COLOR_T     *temp_lcd_buf_ptr = PNULL;
    GUI_RECT_T      display_rect = {0};
    GUI_POINT_T     display_point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};
    BOOLEAN                is_horizontal = FALSE;  //in:is horizontal display
    GUI_RECT_T             *rect_ptr = PNULL;      //in:display rect
    GUI_POINT_T            *point_ptr = PNULL;     //in:display start point
    GUIIMG_BITMAP_T        *bitmap_ptr = PNULL;    //in:
    GUI_LCD_DEV_INFO const *lcd_dev_ptr = PNULL;    //in:
    BOOLEAN         is_transparent_mode = FALSE;
    GUI_COLOR_T     transparent_color = 0;
    if (PNULL == param_ptr)
    {
        return FALSE;
    }
    is_horizontal = param_ptr->is_horizontal;
    rect_ptr = param_ptr->rect_ptr;
    point_ptr = param_ptr->point_ptr;
    bitmap_ptr = param_ptr->bitmap_ptr;
    lcd_dev_ptr = param_ptr->lcd_dev_ptr;
    is_transparent_mode = param_ptr->is_transparent_mode;
    transparent_color = param_ptr->transparent_color;    
    point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)lcd_dev_ptr, point_ptr, &display_point);
    rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)lcd_dev_ptr, rect_ptr, &display_rect);
    if (dc_data_ptr == NULL)
    {
        if (!LCD_PrepareDrawParam(lcd_dev_ptr, &draw_dc))
        {
            return FALSE;
        }
    }
    else
    {
        SCI_MEMCPY(&draw_dc, dc_data_ptr, sizeof(draw_dc));
    }
    logic_lcd_width  = (uint16)draw_dc.width;
    logic_lcd_height = (uint16)draw_dc.height;
    mem_width = (uint16)draw_dc.mem_width;
    lcd_buf_ptr = (GUI_COLOR_T *)draw_dc.lcd_buffer_ptr;

    //set show rect
    show_rect.right  = (uint16)(logic_lcd_width - 1);
    show_rect.bottom = (uint16)(logic_lcd_height - 1);
    
    //modify show rect
    if ((PNULL != rect_ptr) && 
        (!GUI_IntersectRect(&show_rect,show_rect,*rect_ptr)))
    {
        //SCI_TRACE_LOW:"GUIIMG_DisplayBmp:display rect is not on lcd!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIMG_906_112_2_18_3_19_32_200,(uint8*)"");
        result = FALSE;
    }

    if (result)
    {
        if (is_horizontal)
        {
            img_width  = bitmap_ptr->img_height;
            img_height = bitmap_ptr->img_width;
        }
        else
        {
            img_width  = bitmap_ptr->img_width;
            img_height = bitmap_ptr->img_height;
        }

        //set image rect
        img_rect.left   = point_ptr->x;
        img_rect.top    = point_ptr->y;
        img_rect.right  = (int16)(img_rect.left + img_width - 1);
        img_rect.bottom = (int16)(img_rect.top + img_height - 1);

        //modify show rect
        if (!GUI_IntersectRect(&show_rect,show_rect,img_rect))
        {
            //SCI_TRACE_LOW:"GUIIMG_DisplayBmp:image rect is not cross with show rect!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIMG_932_112_2_18_3_19_32_201,(uint8*)"");
            result = FALSE;
        }

        if (result)
        {
            if (PNULL == lcd_buf_ptr)
            {
                return FALSE;
            }

            //set temp lcd buffer
            temp_lcd_buf_ptr = lcd_buf_ptr + show_rect.top*mem_width + show_rect.left;

            //set show image width and height
            show_width  = (uint16)(show_rect.right - show_rect.left + 1);
            show_height = (uint16)(show_rect.bottom - show_rect.top + 1);

            img_x = (uint16)(show_rect.left - img_rect.left);
            img_y = (uint16)(show_rect.top - img_rect.top);

            switch (bitmap_ptr->date_type)
            {
            case GUIIMG_DATE_TYPE_RGB565:
                if (is_horizontal)
                {
                    uint16  *dst_temp_ptr = PNULL;
                    uint16  *dst_img_ptr = PNULL;
                    uint32  i = 0;
                    uint32  j = 0;
                
                    dst_temp_ptr = (uint16 *)(temp_lcd_buf_ptr + show_width - 1);

                    //set image buffer
                    dst_img_ptr = bitmap_ptr->bit_ptr;
                    dst_img_ptr = dst_img_ptr + (img_width-img_x-show_width)*img_height + img_y;

                    for (i=0; i<show_width; i++)
                    {
                        temp_lcd_buf_ptr = dst_temp_ptr - i;

                        img_buf_ptr = dst_img_ptr;
                        for (j=0; j<show_height; j++)
                        {
                            *temp_lcd_buf_ptr = *img_buf_ptr++;
                            temp_lcd_buf_ptr += mem_width;
                        }
                        dst_img_ptr += img_height;
                    }
                }
                else
                {
                    //set image buffer
                    img_buf_ptr = bitmap_ptr->bit_ptr;
                    img_buf_ptr = img_buf_ptr + img_y*img_width + img_x;

                    switch (draw_dc.data_type)
                    {
                    case DATA_TYPE_ARGB888:
                        {
                            uint32 *u32_temp_lcd_buf_ptr = PNULL;
                            uint32 i = 0;
                            uint32 j = 0;
                            uint16 temp_data = 0;

                            u32_temp_lcd_buf_ptr = (uint32*)lcd_buf_ptr;
                            u32_temp_lcd_buf_ptr += show_rect.top*mem_width + show_rect.left;
                            for (i = 0; i < show_height; i++)
                            {
                                for (j = 0; j < show_width; j++)
                                {
                                    temp_data = *img_buf_ptr;
        //                            temp_data = ((temp_data<<8)&0xff00) | ((temp_data>>8)&0xff);
                                    if (!is_transparent_mode || (transparent_color != temp_data))
                                    {
									    *u32_temp_lcd_buf_ptr = ConvertRgb565ToArgb888(temp_data, 0xff);
                                    }
                                    u32_temp_lcd_buf_ptr++;
                                    img_buf_ptr++;
                                }
                                u32_temp_lcd_buf_ptr += (mem_width-show_width);
                                img_buf_ptr += (img_width-show_width);
                            }
                        }
                        break;

                    case DATA_TYPE_RGB565:
                    default:
                        while (show_height)
                        {
                            SCI_MEMCPY(temp_lcd_buf_ptr,img_buf_ptr,(show_width*2));
                            temp_lcd_buf_ptr += mem_width;
                            img_buf_ptr += img_width;
                            show_height--;
                        }
                        break;
                    }
                }
                break;

            case GUIIMG_DATE_TYPE_ARGB888:
#ifdef UI_MULTILAYER_SUPPORT
                switch (UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)lcd_dev_ptr))
                {
                case DATA_TYPE_ARGB888:
                    {
                        uint32  *src_buf_ptr = PNULL;
                        uint32  *u32_temp_lcd_buf_ptr = PNULL;
                        uint32  *temp_src_ptr = PNULL;
                        uint32  *temp_dst_ptr = PNULL;
                        uint16  temp_width = 0;

                        src_buf_ptr = bitmap_ptr->bit_ptr;
                        src_buf_ptr = src_buf_ptr + img_y*img_width + img_x;

                        u32_temp_lcd_buf_ptr = (uint32*)lcd_buf_ptr;
                        u32_temp_lcd_buf_ptr += show_rect.top*mem_width + show_rect.left;

                        while (show_height)
        	            {
                            temp_src_ptr = src_buf_ptr;
                            temp_dst_ptr = u32_temp_lcd_buf_ptr;

                            temp_width = show_width;
                            while (temp_width)
                            {
                                *temp_dst_ptr = GUI_CompoundARGB888(*temp_dst_ptr, *temp_src_ptr++);

                                temp_dst_ptr++;
                                temp_width--;
                            }

                            src_buf_ptr += img_width;
                            u32_temp_lcd_buf_ptr += mem_width;

                            show_height--;
                        }
                    }
                    break;

                case DATA_TYPE_RGB565:
                default:
#endif
                    {
                        uint8       alpha = 0;
                        uint8       h_r = 0,l_r = 0;
                        uint8       h_g = 0,l_g = 0;
                        uint8       h_b = 0,l_b = 0;
                        uint8       r = 0,g = 0,b = 0; 
                        uint16      temp_width = 0;
                        uint16      dst_value = 0;
                        uint16      *temp_dst_ptr = PNULL;
                        uint32      src_value = 0;
                        uint32      *src_buf_ptr = PNULL;
                        uint32      *temp_src_ptr = PNULL;
                        
                        //set image buffer
                        src_buf_ptr = bitmap_ptr->bit_ptr;
                        src_buf_ptr = src_buf_ptr + img_y*img_width + img_x;

                        while (show_height)
        	            {
                            temp_src_ptr = src_buf_ptr;
                            temp_dst_ptr = temp_lcd_buf_ptr;

                            temp_width = show_width;
                            while (temp_width)
                            {
                                src_value = *temp_src_ptr++;

                                alpha = (uint8)(src_value>>24);
                                if (0 == alpha)
                                {
                                    temp_dst_ptr++;
                                }
                                else if ((255 == alpha) || (254 == alpha))
                                {
                                    *temp_dst_ptr++ = (uint16)RGB8882RGB565(src_value);
                                }
                                else
                                {
                                    h_r = (uint8)((src_value&0x00ff0000)>>16);
                                    h_g = (uint8)((src_value&0x0000ff00)>>8);
                                    h_b = (uint8)(src_value&0x000000ff);

                                    dst_value = *temp_dst_ptr;
                                    l_r = (uint8)((dst_value&0xf800)>>8);
                                    l_g = (uint8)((dst_value&0x07e0)>>3);
                                    l_b = (uint8)((dst_value&0x001f)<<3);
                        
                                    r = (uint8)(((l_r<<8)+alpha*(h_r-l_r)+128)>>8);
                                    g = (uint8)(((l_g<<8)+alpha*(h_g-l_g)+128)>>8);
                                    b = (uint8)(((l_b<<8)+alpha*(h_b-l_b)+128)>>8);
                                    dst_value = (uint16)(((r&0xf8)<<8) | ((g&0xfc)<<3) | (b>>3));

                                    *temp_dst_ptr++ = dst_value;
                                }

                                temp_width--;
                            }

                            src_buf_ptr += img_width;
                            temp_lcd_buf_ptr += mem_width;
                        
                            show_height--;
                        }
                    }
#ifdef UI_MULTILAYER_SUPPORT
                    break;
                }
#endif
                
                break;

            default:
                break;
            }

            //store rect
            MMITHEME_StoreUpdateRect(lcd_dev_ptr,show_rect);
        }
    }

    return (result);
}
#endif
/*lint +e775*/

#endif
