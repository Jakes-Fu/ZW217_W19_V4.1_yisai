/*****************************************************************************
** File Name:      mmiclipbrd_magnify.c                                      *
** Author:         hua.fang                                                  *
** Date:           09/01/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011        hua.fang          Create                                   *
******************************************************************************/

#include "mmi_app_clipbrd_trc.h"
#include "mmiclipbrd_internal.h"

#ifdef CLIPBRD_MAGNIFY_SUPPORT
#ifdef GUIF_CLIPBOARD
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiclipbrd_image.h"
#include "graphics_draw.h"
#include "guires.h"
#include "guictrl_api.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MAGNIFY_MULTIPLE_A      12
#define MAGNIFY_MULTIPLE_B      10

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
    BOOLEAN             is_init;
    uint16              width;
    uint16              height;
    GUI_LCD_DEV_INFO    display_layer;
    GUI_LCD_DEV_INFO    bg_layer;
    GUI_LCD_DEV_INFO    mask_layer;
} MMICLIPBRD_MAGNIFY_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/
LOCAL MMICLIPBRD_MAGNIFY_T s_clipbrd_magnify = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 创建层
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateMagnifyLayer(
                                 int16 width,
                                 int16 height,
                                 GUI_LCD_DEV_INFO *lcd_dev_ptr
                                 );

/*****************************************************************************/
//  Description : 释放层
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseMagnifyLayer(
                               GUI_LCD_DEV_INFO *lcd_dev_ptr
                               );

/*****************************************************************************/
//  Description : 层是否有效
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMagnifyLayerValid(
                                  GUI_LCD_DEV_INFO *lcd_dev_ptr
                                  );

/*****************************************************************************/
//  Description : 放大显示内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MagnifyContent(
                             GUI_LCD_DEV_INFO *dst_lcd_dev_ptr,
                             GUI_LCD_DEV_INFO *src_lcd_dev_ptr
                             );

/*****************************************************************************/
//  Description : 去掉放大镜外面的数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BlendManginfy(
                            GUI_LCD_DEV_INFO *display_lcd_dev_ptr,
                            GUI_LCD_DEV_INFO *bg_lcd_dev_ptr,
                            GUI_LCD_DEV_INFO *mask_lcd_dev_ptr
                            );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_StartMagnify(
                                     int16   x,
                                     int16   y,
                                     MMI_HANDLE_T ctrl_handle
                                     )
{
    BOOLEAN         result = FALSE;
    GUI_POINT_T     pt = {0};
    MMI_HANDLE_T    win_handle = 0;

    if (0 == ctrl_handle || s_clipbrd_magnify.is_init)
    {
        return;
    }

    win_handle = MMK_GetWinHandleByCtrl(ctrl_handle);

    result = GUIRES_GetImgWidthHeight(
            &s_clipbrd_magnify.width, &s_clipbrd_magnify.height,
            IMAGE_CLIPBRD_MAGNIFY_BG,
            win_handle);
    if (!result)
    {
        return;
    }

    if (!IsMagnifyLayerValid(&s_clipbrd_magnify.display_layer))
    {
        result = CreateMagnifyLayer(
                s_clipbrd_magnify.width,
                s_clipbrd_magnify.height,
                &s_clipbrd_magnify.display_layer);
        if (!result)
        {
            return;
        }
    }

    if (!IsMagnifyLayerValid(&s_clipbrd_magnify.bg_layer))
    {
        result = CreateMagnifyLayer(
                s_clipbrd_magnify.width,
                s_clipbrd_magnify.height,
                &s_clipbrd_magnify.bg_layer);
        if (!result)
        {
            return;
        }

        pt.x = 0;
        pt.y = 0;

        GUIRES_DisplayImg(
                &pt, PNULL, PNULL,
                win_handle,
                IMAGE_CLIPBRD_MAGNIFY_BG,
                &s_clipbrd_magnify.bg_layer);
    }

    if (!IsMagnifyLayerValid(&s_clipbrd_magnify.mask_layer))
    {
        result = CreateMagnifyLayer(
                s_clipbrd_magnify.width,
                s_clipbrd_magnify.height,
                &s_clipbrd_magnify.mask_layer);
        if (!result)
        {
            return;
        }

        pt.x = 0;
        pt.y = 0;

        GUIRES_DisplayImg(
                &pt, PNULL, PNULL,
                win_handle,
                IMAGE_CLIPBRD_MAGNIFY_MASK,
                &s_clipbrd_magnify.mask_layer);
    }

    s_clipbrd_magnify.is_init = TRUE;
}

/*****************************************************************************/
//  Description : 移动放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_MoveMagnify(
                                    int16   x,
                                    int16   y,
                                    MMI_HANDLE_T ctrl_handle
                                    )
{
    //GUI_POINT_T         pt = {0};
    GUI_RECT_T          rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    MMI_HANDLE_T        win_handle = 0;
    UILAYER_APPEND_BLT_T    layer_append = {0};

    if (s_clipbrd_magnify.is_init
        && GUIAPICTRL_GetLcdDevInfo(ctrl_handle, &lcd_dev_info))
    {
        // 删除显示层，并重新添加
        UILAYER_RemoveBltLayer(&s_clipbrd_magnify.display_layer);

        layer_append.lcd_dev_info = s_clipbrd_magnify.display_layer;
        layer_append.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&layer_append);

        // 清除原有数据
        UILAYER_Clear(&s_clipbrd_magnify.display_layer);

        // 从目标层上拷需要放大的数据
        rect.left = x - s_clipbrd_magnify.width / 2;
        rect.top = y - s_clipbrd_magnify.height / 2;
        rect.right = rect.left + s_clipbrd_magnify.width - 1;
        rect.bottom = rect.top + s_clipbrd_magnify.height - 1;

        UILAYER_SetLayerPosition(&s_clipbrd_magnify.display_layer, rect.left, rect.top);

        UILAYER_BltLayerToLayer(
            &s_clipbrd_magnify.display_layer,
            &lcd_dev_info,
            &rect,
            FALSE);

        // 进行放大
        MagnifyContent(
                &s_clipbrd_magnify.display_layer,
                &s_clipbrd_magnify.display_layer);

        // 画上放大镜
        win_handle = MMK_GetWinHandleByCtrl(ctrl_handle);

        GUIRES_DisplayImg(
                PNULL, &rect, PNULL,
                win_handle,
                IMAGE_CLIPBRD_MAGNIFY_BG,
                &s_clipbrd_magnify.display_layer);

        // 去掉放大镜外面的无用数据
        BlendManginfy(
                &s_clipbrd_magnify.display_layer,
                &s_clipbrd_magnify.bg_layer,
                &s_clipbrd_magnify.mask_layer);

        // 重置到目标显示位置
        UILAYER_SetLayerPosition(
                &s_clipbrd_magnify.display_layer,
                rect.left, y - s_clipbrd_magnify.height);
    }
}

/*****************************************************************************/
//  Description : 结束放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_StopMagnify()
{
    if (s_clipbrd_magnify.is_init)
    {
        s_clipbrd_magnify.is_init = FALSE;

        ReleaseMagnifyLayer(&s_clipbrd_magnify.display_layer);
        ReleaseMagnifyLayer(&s_clipbrd_magnify.bg_layer);
        ReleaseMagnifyLayer(&s_clipbrd_magnify.mask_layer);
    }
}

/*****************************************************************************/
//  Description : 创建层
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateMagnifyLayer(
                                 int16 width,
                                 int16 height,
                                 GUI_LCD_DEV_INFO *lcd_dev_ptr
                                 )
{
    BOOLEAN             result = FALSE;
    UILAYER_RESULT_E    layer_result = UILAYER_RESULT_SUCCESS;
    UILAYER_CREATE_T    create_info = {0};

    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = 0;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = width;;
    create_info.height = height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = TRUE;             // 这里用static，是因为弹出剪贴板窗口后抢了焦点，层会被释放
    create_info.format = UILAYER_MEM_FORMAT_DOUBLE;

    layer_result = UILAYER_CreateLayer(&create_info, lcd_dev_ptr);
    if (UILAYER_RESULT_SUCCESS == layer_result)
    {
        result = TRUE;;
    }

    return result;
}

/*****************************************************************************/
//  Description : 释放层
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseMagnifyLayer(
                               GUI_LCD_DEV_INFO *lcd_dev_ptr
                               )
{
    if (PNULL != lcd_dev_ptr)
    {
        UILAYER_RELEASELAYER(lcd_dev_ptr);
//        lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : 层是否有效
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMagnifyLayerValid(
                                  GUI_LCD_DEV_INFO *lcd_dev_ptr
                                  )
{
    if (PNULL != lcd_dev_ptr
        && 0 != lcd_dev_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_ptr->block_id)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 放大显示内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MagnifyContent(
                             GUI_LCD_DEV_INFO *dst_lcd_dev_ptr,
                             GUI_LCD_DEV_INFO *src_lcd_dev_ptr
                             )
{
    BOOLEAN             result = FALSE;
    int16               dst_x = 0;
    int16               dst_y = 0;
    UILAYER_INFO_T      src_layer_info = {0};
    UILAYER_INFO_T      dst_layer_info = {0};
    SCALE_IMAGE_IN_T    scale_in  = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};
    UILAYER_COPYBUF_T           copy_buf = {0};

    if (PNULL == dst_lcd_dev_ptr || PNULL == src_lcd_dev_ptr)
    {
        goto _Exit_;
    }

    UILAYER_GetLayerInfo(src_lcd_dev_ptr, &src_layer_info);

    scale_in.src_format           = IMGREF_FORMAT_ARGB888;
//    scale_in.src_chn.chn0.ptr     = src_layer_info.layer_buf_ptr;
    scale_in.src_chn.chn0.ptr     = UILAYER_GetLayerBufferPtr(src_lcd_dev_ptr);
    if (PNULL == scale_in.src_chn.chn0.ptr)
    {
        goto _Exit_;
    }
    scale_in.src_chn.chn0.size    = src_layer_info.mem_width * src_layer_info.layer_height * 4;
    scale_in.src_size.w           = src_layer_info.mem_width;
    scale_in.src_size.h           = src_layer_info.layer_height;
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = src_layer_info.mem_width;
    scale_in.src_trim_rect.h      = src_layer_info.layer_height;

    scale_in.target_format        = IMGREF_FORMAT_ARGB888;
    scale_in.target_size.w        = scale_in.src_trim_rect.w * MAGNIFY_MULTIPLE_A/MAGNIFY_MULTIPLE_B;
    scale_in.target_size.h        = scale_in.src_trim_rect.h * MAGNIFY_MULTIPLE_A/MAGNIFY_MULTIPLE_B;
    scale_in.target_buf.size      = scale_in.target_size.w * scale_in.target_size.h * 4 + 255 + 31;
    scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);

    if (PNULL == scale_in.target_buf.ptr)
    {
        goto _Exit_;
    }

    if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
    {
        UILAYER_GetLayerPosition(dst_lcd_dev_ptr, &dst_x, &dst_y);

        UILAYER_GetLayerInfo(dst_lcd_dev_ptr, &dst_layer_info);

        copy_param.rect.left = dst_x + (dst_layer_info.layer_width - scale_out.output_size.w)/2;
        copy_param.rect.top = dst_y + (dst_layer_info.layer_height - scale_out.output_size.h)/2;

        copy_param.rect.right = copy_param.rect.left + scale_out.output_size.w - 1;
        copy_param.rect.bottom = copy_param.rect.top + scale_out.output_size.h - 1;

        copy_buf.buf_ptr = (uint8*)scale_out.output_chn.chn0.ptr;
        copy_buf.data_type = DATA_TYPE_ARGB888;
        copy_buf.width = scale_out.output_size.w;
        copy_buf.height = scale_out.output_size.h;

        UILAYER_CopyBufferToLayer(dst_lcd_dev_ptr, &copy_param, &copy_buf);

        result = TRUE;
    }

_Exit_:
    if (PNULL != scale_in.target_buf.ptr)
    {
        SCI_FREE(scale_in.target_buf.ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : 去掉放大镜外面的数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BlendManginfy(
                            GUI_LCD_DEV_INFO *display_lcd_dev_ptr,
                            GUI_LCD_DEV_INFO *bg_lcd_dev_ptr,
                            GUI_LCD_DEV_INFO *mask_lcd_dev_ptr
                            )
{
//    BOOLEAN             result = FALSE;
    UILAYER_INFO_T      display_layer_info = {0};
    UILAYER_INFO_T      bg_layer_info = {0};
    UILAYER_INFO_T      mask_layer_info = {0};
    uint32              *display_buf_ptr = PNULL;
    uint32              *bg_buf_ptr = PNULL;
    uint32              *mask_buf_ptr = PNULL;
    uint16              i = 0;
    uint16              j = 0;

    if (PNULL == display_lcd_dev_ptr
        || PNULL == bg_lcd_dev_ptr
        || PNULL == mask_lcd_dev_ptr)
    {
        return FALSE;
    }

    UILAYER_GetLayerInfo(display_lcd_dev_ptr, &display_layer_info);
    UILAYER_GetLayerInfo(bg_lcd_dev_ptr, &bg_layer_info);
    UILAYER_GetLayerInfo(mask_lcd_dev_ptr, &mask_layer_info);

//    if (PNULL == display_layer_info.layer_buf_ptr
//        || PNULL == bg_layer_info.layer_buf_ptr
//        || PNULL == mask_layer_info.layer_buf_ptr)
//    {
//        SCI_TRACE_LOW("== BlendManginfy == buf is null, 0x%x, 0x%x, 0x%x",
//                display_layer_info.layer_buf_ptr,
//                bg_layer_info.layer_buf_ptr,
//                mask_layer_info.layer_buf_ptr);
//
//        return FALSE;
//    }
//    display_buf_ptr = (uint32*)display_layer_info.layer_buf_ptr;
//    bg_buf_ptr = (uint32*)bg_layer_info.layer_buf_ptr;
//    mask_buf_ptr = (uint32*)mask_layer_info.layer_buf_ptr;

    display_buf_ptr = (uint32*)UILAYER_GetLayerBufferPtr(display_lcd_dev_ptr);
    bg_buf_ptr = (uint32*)UILAYER_GetLayerBufferPtr(bg_lcd_dev_ptr);
    mask_buf_ptr = (uint32*)UILAYER_GetLayerBufferPtr(mask_lcd_dev_ptr);
    if (PNULL == display_buf_ptr
        || PNULL == bg_buf_ptr
        || PNULL == mask_buf_ptr)
    {
        //SCI_TRACE_LOW:"== BlendManginfy == buf is null, 0x%x, 0x%x, 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICLIPBRD_MAGNIFY_478_112_2_18_2_11_59_0,(uint8*)"ddd",display_buf_ptr,bg_buf_ptr,mask_buf_ptr);

        return FALSE;
    }
    
    for (i = 0; i < display_layer_info.layer_height; i++)
    {
        for (j = 0; j < display_layer_info.layer_width; j++)
        {
            if (0xff000000 == *(mask_buf_ptr + j))
            {
                *(display_buf_ptr + j) = *(bg_buf_ptr + j);
            }
        }

        display_buf_ptr += display_layer_info.mem_width;
        bg_buf_ptr += bg_layer_info.mem_width;
        mask_buf_ptr += mask_layer_info.mem_width;
    }

    return TRUE;
}
#else

/*****************************************************************************/
//  Description : 创建放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_StartMagnify(
                                     int16   x,
                                     int16   y,
                                     MMI_HANDLE_T ctrl_handle
                                     )
{
}

/*****************************************************************************/
//  Description : 移动放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_MoveMagnify(
                                    int16   x,
                                    int16   y,
                                    MMI_HANDLE_T ctrl_handle
                                    )
{
}

/*****************************************************************************/
//  Description : 结束放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_StopMagnify()
{
}

#endif
#endif
