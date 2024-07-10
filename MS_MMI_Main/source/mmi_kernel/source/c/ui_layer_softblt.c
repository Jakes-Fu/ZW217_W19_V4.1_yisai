/****************************************************************************
** File Name:      ui_layer.c                                               *
** Author:                                                                 *
** Date:           07/08/2009                                              *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/08/2009     bin.ji        Create
** 
****************************************************************************/
#define UI_LAYER_C

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/

#ifdef UI_MULTILAYER_SUPPORT

#include "mmi_kernel_trc.h"
#include "ui_layer.h"
#include "guilcd.h"
#include "mmk_app.h"
#include "lcd_cfg.h"
#include "guicommon.h"
#include "mmi_theme.h"
#include "dal_display.h"

/**-----------------------------------------------------------------------------*
 **                         MACRO Declaration                                   *
 **----------------------------------------------------------------------------*/

#define MAX_ALPHA_VALUE     255 //全不透的值
#define DIV2(color)         ((color&0xf7de)>>1) //565颜色取半
#define SIMPLE_ALPHA_COLOR(color1, color2)      (DIV2(color1)+DIV2(color2)) //1:1的565颜色混合
#define R_COLOR(color)          ((color) & 0xf800)  //颜色分量：Red
#define G_COLOR(color)          ((color) & 0x07e0)  //颜色分量：Green
#define B_COLOR(color)          ((color) & 0x001f)  //颜色分量：Blue

//copy含透明色的8个像素
#define COPY_KEYCOLOR_8PIXEL(dst_buf_ptr, src_buf_ptr, color_key)   \
    if (color_key != *src_buf_ptr)              \
        *dst_buf_ptr = *src_buf_ptr;            \
    if (color_key != *(src_buf_ptr+1))          \
        *(dst_buf_ptr+1) = *(src_buf_ptr+1);    \
    if (color_key != *(src_buf_ptr+2))          \
        *(dst_buf_ptr+2) = *(src_buf_ptr+2);    \
    if (color_key != *(src_buf_ptr+3))          \
        *(dst_buf_ptr+3) = *(src_buf_ptr+3);    \
    if (color_key != *(src_buf_ptr+4))          \
        *(dst_buf_ptr+4) = *(src_buf_ptr+4);    \
    if (color_key != *(src_buf_ptr+5))          \
        *(dst_buf_ptr+5) = *(src_buf_ptr+5);    \
    if (color_key != *(src_buf_ptr+6))          \
        *(dst_buf_ptr+6) = *(src_buf_ptr+6);    \
    if (color_key != *(src_buf_ptr+7))          \
        *(dst_buf_ptr+7) = *(src_buf_ptr+7);    \



/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


//用来保存copy buffer的地址，rect等数据
typedef struct 
{
    uint8           *buf_ptr;           //layer的buffer指针
    GUI_RECT_T      rect;
}UILAYER_CLIP_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
 **                         Global Variables                                *
 **------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                    LOCAL FUNCTION DECLARE                                */
/*--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : copy含透明色的一行
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void CopyKeyColorLine(
                            GUI_COLOR_T *dst_buf_ptr,
                            GUI_COLOR_T *src_buf_ptr,
                            GUI_COLOR_T color_key,
                            uint32 line_length
                            );

/*****************************************************************************/
//     Description : 565的颜色混合
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL uint16 MyAlphaBlend(
                          uint16 color1,
                          uint16 color2,
                          uint8 alpha_value
                          );

/*****************************************************************************/
//     Description : 合并层到目标buf。
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void BltLayer(
                    uint8 *buf_ptr,
                    int16 offset_x,                 //目标buf相对于lcd左上角的x坐标
                    int16 offset_y,                 //目标buf相对于lcd左上角的y坐标
                    int16 width,                    //目标buf宽
                    int16 height,                   //目标buf高
                    LAYER_STRUCT_T *layer_arr[],
                    uint32 arr_size,
                    const GUI_RECT_T *rect_ptr
                    );

/*****************************************************************************/
//     Description : 合并图层
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void BltClip(
                    UILAYER_CLIP_T *src_clip_ptr,   //源clip
                    UILAYER_CLIP_T *dst_clip_ptr,   //目标clip
                    GUI_RECT_T     *blt_rect_ptr,   //blt的rect
                    BOOLEAN        is_colorkey_enable,  //透明色是否使用
                    GUI_COLOR_T    color_key,        //透明色
                    uint8          alpha_value      //alpha值
                    );

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  multi layer blend interface for the APP
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E SOFT_InvalidateRectUsingLayer(LCD_ID_E lcd_id,
				LAYER_STRUCT_T *dst_layer_handle,
    				LAYER_STRUCT_T *layer0_ptr,
    				LAYER_STRUCT_T *layer1_ptr,
    				LAYER_STRUCT_T *layer2_ptr,
    				LAYER_STRUCT_T *layer3_ptr,
    				LCD_RECT_T *rect_ptr)
{
    LAYER_STRUCT_T *layer_stuct_arr[UILAYER_TOTAL_BLT_COUNT] = {0};
    uint32 i = 0;
    GUI_RECT_T rect = {0};
    
    layer_stuct_arr[0] = layer0_ptr;
    layer_stuct_arr[1] = layer1_ptr;
    layer_stuct_arr[2] = layer2_ptr;
    layer_stuct_arr[3] = layer3_ptr;

    for (i = 0; i < UILAYER_TOTAL_BLT_COUNT; i++)
    {
        if (PNULL != layer_stuct_arr[i])
        {
            //8对齐
            SCI_ASSERT(0 == (layer_stuct_arr[i]->layer_cfg.offset_x%8));/*assert verified*/
            SCI_ASSERT(0 == (layer_stuct_arr[i]->layer_cfg.width%8));/*assert verified*/
        }        
    }

    rect.left = rect_ptr->left;
    rect.top = rect_ptr->top;
    rect.right = rect_ptr->right;
    rect.bottom = rect_ptr->bottom;

    if (PNULL != dst_layer_handle)
    {
        GUI_COLOR_T dst_buf_ptr = PNULL;

        //8对齐
        SCI_ASSERT(0 == (dst_layer_handle->layer_cfg.offset_x%8));/*assert verified*/
        SCI_ASSERT(0 == (dst_layer_handle->layer_cfg.width%8)); /*assert verified*/
        //刷到数据buf
        BltLayer(
            dst_layer_handle->layer_buf_ptr, 
            dst_layer_handle->layer_cfg.offset_x, 
            dst_layer_handle->layer_cfg.offset_y,
            dst_layer_handle->layer_cfg.width,
            dst_layer_handle->layer_cfg.height,
            layer_stuct_arr, 
            UILAYER_TOTAL_BLT_COUNT, 
            &rect
            );
    }
    else
    {
        //刷到lcd
        GUI_COLOR_T *lcd_buf_ptr = PNULL;
        uint16 lcd_width = 0;
        uint16 lcd_height = 0;
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

        lcd_dev_info.lcd_id = lcd_id;
        GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id,&lcd_width,&lcd_height);
        lcd_buf_ptr = GUILCD_GetMainLcdBufPtr();
        BltLayer((uint8*)lcd_buf_ptr, 0, 0, lcd_width, lcd_height, layer_stuct_arr, UILAYER_TOTAL_BLT_COUNT, &rect);
        GUILCD_InvalidateRectOnly(lcd_id, *rect_ptr,0);
    }
    return ERR_LCD_NONE;
}

/*****************************************************************************/
//     Description : 合并层到目标buf。
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void BltLayer(
                    uint8 *buf_ptr,
                    int16 offset_x,                 //目标buf相对于lcd左上角的x坐标
                    int16 offset_y,                 //目标buf相对于lcd左上角的y坐标
                    int16 width,                    //目标buf宽
                    int16 height,                   //目标buf高
                    LAYER_STRUCT_T *layer_arr[],
                    uint32 arr_size,
                    const GUI_RECT_T *rect_ptr
                    )
{
    uint32 i = 0;
    uint32 j = 0;
    LAYER_STRUCT_T *layer_ptr = PNULL;    //layer buffer指针
    UILAYER_CLIP_T src_clip = {0};   //源clip
    UILAYER_CLIP_T dst_clip = {0};   //目标clip
    GUI_RECT_T blt1_rect = {0}; //第一次目标buf跟rect重叠blt部分  
    GUI_RECT_T blt_rect = {0};   //最终重叠blt的rect

    SCI_ASSERT(PNULL != buf_ptr);/*assert verified*/
    SCI_ASSERT(PNULL != layer_arr);/*assert verified*/
    SCI_ASSERT(PNULL != rect_ptr);/*assert verified*/
    SCI_ASSERT(arr_size <= UILAYER_TOTAL_BLT_COUNT);/*assert verified*/

    dst_clip.buf_ptr = buf_ptr;
    //计算目标buf的区域
    dst_clip.rect.left = offset_x;
    dst_clip.rect.top = offset_y;
    dst_clip.rect.right = offset_x+width-1;
    dst_clip.rect.bottom = offset_y+height-1;
    if (GUI_IntersectRect(&blt1_rect, dst_clip.rect, *rect_ptr))
    {        
        for (i = 0; i < arr_size; i++)
        {
            layer_ptr = layer_arr[i];
            if (PNULL != layer_ptr)
            {
                src_clip.buf_ptr = layer_ptr->layer_buf_ptr;
                //计算源buf的区域     
                src_clip.rect.left = layer_ptr->layer_cfg.offset_x;
                src_clip.rect.top = layer_ptr->layer_cfg.offset_y;
                src_clip.rect.right = layer_ptr->layer_cfg.offset_x+layer_ptr->layer_cfg.width-1;
                src_clip.rect.bottom = layer_ptr->layer_cfg.offset_y+layer_ptr->layer_cfg.height-1;
                if (GUI_IntersectRect(&blt_rect, blt1_rect, src_clip.rect))
                {
                    //软件实现层的合并：将layer刷到main_lcd_buf（或者dst_layer buf）中去
                    BltClip(
                        &src_clip, 
                        &dst_clip, 
                        &blt_rect, 
                        layer_ptr->layer_cfg.is_color_key_enable,
                        layer_ptr->layer_cfg.color_key,
                        layer_ptr->layer_cfg.alpha_value
                        );
                }
                else
                {
                    //该层和指定区域没有重叠的地方
                    //SCI_TRACE_LOW:"dst_rect & layer_rect not intersect! dst_rect = (%d, %d, %d, %d)"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_SOFTBLT_266_112_2_18_3_23_1_101,(uint8*)"dddd", dst_clip.rect.left, dst_clip.rect.top, dst_clip.rect.right, dst_clip.rect.bottom);
                    //SCI_TRACE_LOW:"dst_rect & layer_rect not intersect! layer_rect = (%d, %d, %d, %d)"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_SOFTBLT_267_112_2_18_3_23_1_102,(uint8*)"dddd", src_clip.rect.left, src_clip.rect.top, src_clip.rect.right, src_clip.rect.bottom);
                }
            }
        }
    }
    else
    {
        //该层和指定区域没有重叠的地方
        //SCI_TRACE_LOW:"dst_rect & rect_ptr not intersect! dst_rect = (%d, %d, %d, %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_SOFTBLT_275_112_2_18_3_23_2_103,(uint8*)"dddd", dst_clip.rect.left, dst_clip.rect.top, dst_clip.rect.right, dst_clip.rect.bottom);
        //SCI_TRACE_LOW:"dst_rect & rect_ptr not intersect! rect_ptr = (%d, %d, %d, %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UI_LAYER_SOFTBLT_276_112_2_18_3_23_2_104,(uint8*)"dddd", rect_ptr->left, rect_ptr->top, rect_ptr->right, rect_ptr->bottom);
    }
}

/*****************************************************************************/
//     Description : 合并图层
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void BltClip(
                    UILAYER_CLIP_T *src_clip_ptr,   //源clip
                    UILAYER_CLIP_T *dst_clip_ptr,   //目标clip
                    GUI_RECT_T     *blt_rect_ptr,   //blt的rect
                    BOOLEAN        is_colorkey_enable,  //透明色是否使用
                    GUI_COLOR_T    color_key,        //透明色
                    uint8          alpha_value      //alpha值
                    )
{
    GUI_COLOR_T *src_buf_ptr = PNULL;
    GUI_COLOR_T *dst_buf_ptr = PNULL;
    int32 src_width = 0;
    int32 src_height = 0;
    int32 dst_width = 0;
    int32 dst_height = 0;
    int32 blt_rect_width = 0;
    int32 blt_rect_height = 0;
    int32 src_line_offset = 0;
    int32 dst_line_offset = 0;    
    int32 wcount = 0;
    int32 hcount = 0;
    int32 i = 0;

    blt_rect_width = (blt_rect_ptr->right+1-blt_rect_ptr->left);
    blt_rect_height = (blt_rect_ptr->bottom+1-blt_rect_ptr->top);
    src_width = (src_clip_ptr->rect.right+1-src_clip_ptr->rect.left);
    src_height = (src_clip_ptr->rect.bottom +1-src_clip_ptr->rect.top);
    dst_width = (dst_clip_ptr->rect.right+1-dst_clip_ptr->rect.left);
    dst_height = (dst_clip_ptr->rect.bottom+1-dst_clip_ptr->rect.top);
    src_line_offset = (src_width - blt_rect_width);
    dst_line_offset = (dst_width - blt_rect_width);

    src_buf_ptr = (GUI_COLOR_T*)src_clip_ptr->buf_ptr;
    dst_buf_ptr = (GUI_COLOR_T*)dst_clip_ptr->buf_ptr;
    src_buf_ptr += (blt_rect_ptr->top - src_clip_ptr->rect.top) * src_width 
        + (blt_rect_ptr->left - src_clip_ptr->rect.left);
    dst_buf_ptr += (blt_rect_ptr->top - dst_clip_ptr->rect.top) * dst_width
        + (blt_rect_ptr->left - dst_clip_ptr->rect.left);
    
    if (is_colorkey_enable || (MAX_ALPHA_VALUE > alpha_value))
    {
        //使用透明色或者alpha混合，需要逐点计算
#if 1   //用优化
        if (MAX_ALPHA_VALUE > alpha_value)
        {
            //alpha混合
            hcount = blt_rect_height;
            while (hcount)
            {
                wcount = blt_rect_width;
                while (wcount)
                {
                    if (color_key != *src_buf_ptr)
                    {
                        *dst_buf_ptr = MyAlphaBlend(*src_buf_ptr, *dst_buf_ptr, alpha_value);
                    }
                    src_buf_ptr++;
                    dst_buf_ptr++;
                    wcount--;
                }
                src_buf_ptr += src_line_offset;
                dst_buf_ptr += dst_line_offset;
                hcount--;
            }
        }
        else
        {
            //透明色
            hcount = blt_rect_height;
            while (hcount)
            {
                CopyKeyColorLine(dst_buf_ptr, src_buf_ptr, color_key, blt_rect_width);
                src_buf_ptr += (src_line_offset+blt_rect_width);
                dst_buf_ptr += (dst_line_offset+blt_rect_width);
                hcount--;
            }            
        }
#else   //未用优化
        //使用透明色或者alpha混合，需要逐点计算
        hcount = blt_rect_height;
        while (hcount)
        {
            wcount = blt_rect_width;
            while (wcount)
            {
                if (color_key != *src_buf_ptr)
                {
                    if (MAX_ALPHA_VALUE > alpha_value)
                    {
                        //alpha混合
                        *dst_buf_ptr = MyAlphaBlend(*src_buf_ptr, *dst_buf_ptr, alpha_value);
                    }
                    else
                    {
                        *dst_buf_ptr = *src_buf_ptr;
                    }
                }
                src_buf_ptr++;
                dst_buf_ptr++;
                wcount--;
            }
            src_buf_ptr += src_line_offset;
            dst_buf_ptr += dst_line_offset;
            hcount--;
        }
#endif
    }
    else
    {
        //整行计算
        for (i = 0; i < blt_rect_height; i++)
        {                        
            SCI_MEMCPY(
                dst_buf_ptr,
                src_buf_ptr,
                blt_rect_width * (int32)sizeof(GUI_COLOR_T)
                );
            src_buf_ptr += src_width;
            dst_buf_ptr += dst_width;
        }        
    }
}

/*****************************************************************************/
//     Description : 565的颜色混合
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL uint16 MyAlphaBlend(
                          uint16 color1,
                          uint16 color2,
                          uint8 alpha_value
                          )
{
    uint16 color = 0;
    uint16 r_color = 0;
    uint16 g_color = 0;
    uint16 b_color = 0;

    r_color = (R_COLOR(color1) * alpha_value + R_COLOR(color2) * (256 - alpha_value)) >> 8;
    g_color = (G_COLOR(color1) * alpha_value + G_COLOR(color2) * (256 - alpha_value)) >> 8;
    b_color = (B_COLOR(color1) * alpha_value + B_COLOR(color2) * (256 - alpha_value)) >> 8;

    color = (R_COLOR(r_color) | G_COLOR(g_color) | B_COLOR(b_color));

    return color;
}

/*****************************************************************************/
//     Description : copy含透明色的一行
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void CopyKeyColorLine(
                            GUI_COLOR_T *dst_buf_ptr,
                            GUI_COLOR_T *src_buf_ptr,
                            GUI_COLOR_T color_key,
                            uint32 line_length
                            )
{
    int32 hcount = line_length>>3;
    int32 wcount = line_length&0x07;

    while (hcount)
    {
        COPY_KEYCOLOR_8PIXEL(dst_buf_ptr, src_buf_ptr, color_key);
        src_buf_ptr += 8;
        dst_buf_ptr += 8;
        hcount--;
    }
    while (wcount)
    {
        if (color_key != *src_buf_ptr)
        {
            *dst_buf_ptr = *src_buf_ptr;
        }
        src_buf_ptr++;
        dst_buf_ptr++;
        wcount--;
    }    
}

#endif  //UI_MULTILAYER_SUPPORT