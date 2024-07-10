/******************************************************************************
 ** File Name:      layer_blend.c                                             *
 ** Description:                                                              *
 **    This file contains functions of handle layer                           *
 ** Author:         Jianping.Wang                                             *
 ** DATE:           11/20/2009                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE                  NAME             DESCRIPTION                        *
 ** 11/20/2009    Jianping.Wang        Create.
 ******************************************************************************/
#include "ms_ref_lcd_trc.h"
#include "lcd_ctrl.h"
#include "lcd_osd_rgb888_blender.h"
#include "display_blend_alg.h"
#include "lcdc_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


#ifdef WIN32
#define SOFTWARE_BLEND_LAYER            1
#else
#define SOFTWARE_BLEND_LAYER            0
#endif

#ifdef PLATFORM_UWS6121E
#define SOFTWARE_BLEND_LAYER            1
#endif

#define LAYER_DST_INDEX                 LAYER_SUPPORT_NUM
#define LAYER_ERROR_DATA_TYPE           0xff
#define MAX_ALPHA_VALUE                 255
#define LAYER_HANDLE_TRACE              SCI_TRACE_LOW
#define R_COLOR(color)                  ((color) & 0xf800)
#define G_COLOR(color)                  ((color) & 0x07e0)
#define B_COLOR(color)                  ((color) & 0x001f)

#define U32_R_COLOR(color)              (((color) & 0xf80000)>>8)
#define U32_G_COLOR(color)              (((color) & 0xfc00)>>5)
#define U32_B_COLOR(color)              (((color) & 0xf8)>>3)

#define LAYER_TYPE_OSD_WITH_PIXEL_ALPHA 1
#define MULTILAYER_PRINTF_DEBUG_INFO    0

#define DATA_ENDIAN_BIG                 0
#define DATA_ENDIAN_LITTLE              1

#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))

#define Blend_GET_R_FROM_RGB565_VALUE(_rgb565_value) (((_rgb565_value) >> 8) & 0xf8)
#define Blend_GET_G_FROM_RGB565_VALUE(_rgb565_value) (((_rgb565_value) >> 3) & 0xfc)
#define Blend_GET_B_FROM_RGB565_VALUE(_rgb565_value) (((_rgb565_value) << 3)  & 0xf8)


#define BLEND_RGB565TORGB888(value_565,value_888) do{ \
       value_888 = (( value_565&0xf800) | ( ( value_565 & 0x800 ) ? 0x700 : 0 ))<<8 \
                   |((value_565&0x7e0 ) | ( ( value_565&0x20 ) ? 0x18 : 0 ))<<5 \
                   |((( value_565&0x1f )<<3) | ( ( value_565 & 0x1 ) ? 7 : 0 )); \
}while(0)

typedef struct _layer_context_tag
{
    DISPLAY_BLEND_LAYER_T      *layer_cfg_ptr;
    DISPLAY_RECT_T              disp_rect;//it's in the lcd plane
}LAYER_CONTEXT_T;
LOCAL ERR_DISPLAY_E blend_argb888(  LCD_ID_E lcd_id,
                                    DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                    LAYER_CONTEXT_T *layer_arr,
                                    uint32 layer_num,
                                    DISPLAY_RECT_T *rect_ptr);
__inline uint32 covert_to_byte_num(DISPLAY_DATA_TYPE_E data_format);
/*lint -save -e737 */
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:  check parameter
//  Global resource dependence:
//  Author: Allen.mao
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _check_clip_rect (DISPLAY_RECT_T *clip_rect, 
                                            int16 width,
                                            int16 height)
{
    if (clip_rect->left < 0)
    {
        clip_rect->left = 0;
    }
    else if (clip_rect->left > width - 1)
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    if (clip_rect->top < 0) { 
        clip_rect->top = 0;
    }
    else if (clip_rect->top > height - 1)
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    if (clip_rect->right == 0 || clip_rect->right > width - 1) 
    {
        clip_rect->right = width - 1;
    }
    else if (clip_rect->right < 0)
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    if (clip_rect->bottom == 0 || clip_rect->bottom > height - 1) 
    {
        clip_rect->bottom = height - 1;
    }
    else if (clip_rect->bottom < 0)
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    return ERR_DISPLAY_NONE;
}

/*****************************************************************************/
//  Description:  check parameter
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _check_layer_parameter(DISPLAY_BLEND_LAYER_T *layer_arr[],  uint32 array_size, BOOLEAN is_soft_handle)
{
    uint32 i = 0;
    ERR_DISPLAY_E         ret = ERR_DISPLAY_NONE;
    DISPLAY_BLEND_LAYER_T *layer_ptr = PNULL;

    if(is_soft_handle)
    {
        for(i=0 ; i<array_size ; i++)
        {
            layer_ptr = layer_arr[i];
            if(layer_ptr != PNULL)
            {               
                if((layer_ptr->layer_info.layer_data.type == DATA_TYPE_ARGB565)  \
                   || (layer_ptr->layer_info.layer_data.type == DATA_TYPE_ARGB666))
                {
                    ret =ERR_DISPLAY_PARAMETER_WRONG;
                    break;
                }
            }
            else
            {
                ret = ERR_DISPLAY_PARAMETER_WRONG;
            }
        }
   }
   else
   {
        for(i=0 ; i<array_size ; i++)
        {
            layer_ptr = layer_arr[i];
            if(layer_ptr != PNULL)
            {
                if(layer_ptr->layer_info.layer_data.type >= DATA_TYPE_MAX)
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    break;
                }
            }
            else
            {
                ret = ERR_DISPLAY_PARAMETER_WRONG;
            }
        }
    }
    return ret;
}
/*****************************************************************************/
//  Description:  get the layer's display rect on the lcd plane
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _get_layer_display_rect(LAYER_CONTEXT_T *layer_context_ptr, DISPLAY_RECT_T *rect_ptr)
{
    int16 left   = 0;
    int16 top    = 0;
    int16 right  = 0;
    int16 bottom = 0;
    int16 lcd_left  = rect_ptr->left;
    int16 lcd_top   = rect_ptr->top;
    int16 lcd_right  = rect_ptr->right;
    int16 lcd_bottom = rect_ptr->bottom;
    DISPLAY_BLEND_LAYER_T *layer_ptr = layer_context_ptr->layer_cfg_ptr;
    DISPLAY_RECT_T   *clip_rect = &layer_ptr->layer_info.clip_rect;
    uint32 bytes_num = 0;

    if(layer_ptr != PNULL)
    {
        //the lcd plane
        left   = layer_ptr->layer_info.pos.x + clip_rect->left;
        top    = layer_ptr->layer_info.pos.y + clip_rect->top;      
        right  = left + clip_rect->right  - clip_rect->left;
        bottom = top  + clip_rect->bottom - clip_rect->top;

        left = left > lcd_left ? left : lcd_left;
        top = top > lcd_top ? top : lcd_top;
        right = right > lcd_right ? lcd_right : right;
        bottom = bottom > lcd_bottom ? lcd_bottom : bottom;

        if((left>=right)||(top>=bottom))
            return ERR_DISPLAY_PARAMETER_WRONG;
        {
            //base pointer must be aligned by words 
            bytes_num = covert_to_byte_num(layer_ptr->layer_info.layer_data.type);
            if (bytes_num == 1)
            {
                left &= ~3;
            }
            else if(bytes_num == 2)
            {
                left &= ~1;
            }
        }
        layer_context_ptr->disp_rect.left   = left;
        layer_context_ptr->disp_rect.top    = top;
        layer_context_ptr->disp_rect.right  = right;
        layer_context_ptr->disp_rect.bottom = bottom;
       
    }

    return ERR_DISPLAY_NONE;
}

/*****************************************************************************/
//  Description:  convert data type 
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 _data_type_convert(DISPLAY_DATA_TYPE_E data_type)
{
    uint32 ret = 0;
     //0:RGB888; 1:RGB666; 2:RGB565(RGB555 if OSD);3:RGB555;4:GREY,5:YUV422;6:YUV420;7:YUV400
    switch(data_type)
    {
           case DATA_TYPE_ARGB888:
           case DATA_TYPE_PMARGB888:
                ret = 0; // LCD_RESOLUTION_RGB888
                break;
           case DATA_TYPE_ARGB666:
           case DATA_TYPE_PMARGB666:
                ret = 1; // LCD_RESOLUTION_RGB666
                break;

           case DATA_TYPE_ARGB565:
           case DATA_TYPE_RGB565: 
           case DATA_TYPE_PMARGB565:
                ret = 2; //LCD_RESOLUTION_RGB565
                break;
           case DATA_TYPE_ARGB555:
                ret = 3; //LCD_RESOLUTION_RGB555
                break;
           case DATA_TYPE_GREY:
                ret = 4; 
                break;
           case DATA_TYPE_YUV422:
                ret = 5;
                break;
           case DATA_TYPE_YUV420:
                ret = 6;
                break;
           case DATA_TYPE_YUV400:
                ret = 7;
                break;
           default:
                ret = LAYER_ERROR_DATA_TYPE;
                break;
    }

    return ret;
}

/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _SoftLayerBlend2PMARGB888(
                                         DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                         LAYER_CONTEXT_T *layer_array, 
                                         DISPLAY_RECT_T *rect_ptr,
                                         uint32 layer_num,
                                         uint32 *dst_u32_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint32 *dst_buf = PNULL;    
    uint32 i = 0;
    uint16 refresh_width = 0;
    uint16 refresh_height = 0;   
    uint16 *src_buf = PNULL; 
    uint32 *u32_src_buf = PNULL; 
    DISPLAY_LAYER_INFO_T *layer_cfg_ptr = PNULL;
    int16 left = 0,top =0;
    int16 right = 0,bottom = 0;
    DISPLAY_BLEND_LAYER_T *layer_ptr = PNULL;    
    uint32 src_width = 0;    
    uint16 srcjumpcount = 0,dstjumpcount = 0;  
    
   
    for( i=0 ; i<layer_num ; i++)
    {
        layer_ptr = layer_array[i].layer_cfg_ptr;
        if (PNULL == layer_ptr)
        {
            continue;
                
        }
        layer_cfg_ptr = &(layer_ptr->layer_info);
        left = layer_array[i].disp_rect.left;
        top  = layer_array[i].disp_rect.top;
        right  = layer_array[i].disp_rect.right;
        bottom = layer_array[i].disp_rect.bottom;
        if ((left > right) || (top > bottom))
        {
             continue;
        }

        dst_buf = (uint32*)dst_layer_ptr->layer_info.buf_addr;
        dst_buf += left-dst_layer_ptr->layer_info.pos.x+ (top-dst_layer_ptr->layer_info.pos.y)*dst_layer_ptr->layer_info.width;  


        refresh_width  = right - left + 1;
        refresh_height = bottom - top + 1;          
        src_width = layer_cfg_ptr->width;
        src_buf = (uint16*)layer_ptr->layer_info.buf_addr + (left- layer_cfg_ptr->pos.x) + (top-layer_cfg_ptr->pos.y)*src_width;
        u32_src_buf = (uint32*)layer_ptr->layer_info.buf_addr + (left- layer_cfg_ptr->pos.x) + (top-layer_cfg_ptr->pos.y)*src_width;
        srcjumpcount = src_width-refresh_width;
        dstjumpcount = dst_layer_ptr->layer_info.width-refresh_width;       
        if (DATA_TYPE_ARGB888 == layer_cfg_ptr->layer_data.type)
        {
            DISPLAY_Blend_ARGB888ToPMARGB888(layer_cfg_ptr->layer_data.colorkey_en,
                                             layer_cfg_ptr->layer_data.colorkey, 
                                             dst_buf, 
                                             u32_src_buf, 
                                             srcjumpcount, 
                                             dstjumpcount, 
                                             refresh_width, 
                                             refresh_height,
                                             layer_cfg_ptr->layer_data.alpha
                                             );
        }
        else if (DATA_TYPE_RGB565 == layer_cfg_ptr->layer_data.type)
        {

            DISPLAY_Blend_RGB565ToPMARGB888(layer_cfg_ptr->layer_data.colorkey_en,
                                            layer_cfg_ptr->layer_data.colorkey,  
                                            dst_buf, 
                                            src_buf, 
                                            srcjumpcount, 
                                            dstjumpcount, 
                                            refresh_width, 
                                            refresh_height, 
                                            layer_cfg_ptr->layer_data.alpha
                                           );
        }
        else if (DATA_TYPE_PMARGB888 == layer_cfg_ptr->layer_data.type)
        {
            if(layer_cfg_ptr->layer_data.alpha_sel ==DISPLAY_BLOCK_ALPHA)
            {
                //SCI_TRACE_LOW:"[_SoftLayerBlend ERROR 0],dont supoort!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_337_112_2_18_1_44_52_0,(uint8*)"");
                return ERR_DISPLAY_FUNC_NOT_SUPPORT;
            }

             //blend pmargb888 to pmargb888 image
            DISPLAY_Blend_PMARGB888ToPMARGB888(layer_cfg_ptr->layer_data.colorkey_en,
                                               layer_cfg_ptr->layer_data.colorkey,  
                                               dst_buf, 
                                               u32_src_buf, 
                                               srcjumpcount, 
                                               dstjumpcount, 
                                               refresh_width, 
                                               refresh_height,
                                               layer_cfg_ptr->layer_data.alpha
                                              );
         }
         else
         {            
            //SCI_TRACE_LOW:"[_SoftLayerBlend ERROR 2],dont supoort!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_356_112_2_18_1_44_52_1,(uint8*)"");
            return ERR_DISPLAY_FUNC_NOT_SUPPORT;
         }
       
    }

    return ret;
}


/*****************************************************************************/
//  Description:  multi layer blend using software
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _SoftLayerBlend(
                                         DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                         LAYER_CONTEXT_T *layer_array, 
                                         DISPLAY_RECT_T *rect_ptr,
                                         uint32 layer_num,
                                         uint32 *dst_u32_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint32 *dst_buf = PNULL;    
    uint32 i = 0,k = 1;
    uint16 refresh_width = 0;
    uint16 refresh_height = 0;   
    uint16 *src_buf = PNULL; 
    uint32 *u32_src_buf = PNULL; 
    DISPLAY_LAYER_INFO_T *layer_cfg_ptr = PNULL;
    int16 left = 0,top =0;
    int16 right = 0,bottom = 0;
    DISPLAY_BLEND_LAYER_T *layer_ptr = PNULL;    
    uint32 src_width = 0;    
    uint16 srcjumpcount = 0,dstjumpcount = 0;  
    
    layer_num--;        
    for( i=0 ; i<layer_num ; i++)
    {        
        layer_ptr = layer_array[k].layer_cfg_ptr;
        if (PNULL == layer_ptr)
        {
            k++;
            continue;
        }
        layer_cfg_ptr = &(layer_ptr->layer_info);
        left = layer_array[k].disp_rect.left;
        top  = layer_array[k].disp_rect.top;
        right  = layer_array[k].disp_rect.right;
        bottom = layer_array[k].disp_rect.bottom;
        if ((left > right) || (top > bottom))
        {
            k++;
            continue;
        }
        if(dst_u32_ptr)
        {
            dst_buf = dst_u32_ptr;
            dst_buf += left-dst_layer_ptr->layer_info.pos.x+ (top-dst_layer_ptr->layer_info.pos.y)*dst_layer_ptr->layer_info.width;  
        }
        else
        {
            dst_buf = (uint32*)dst_layer_ptr->layer_info.buf_addr;
           // dst_buf += left-dst_layer_ptr->layer_info.pos.x+ (top-dst_layer_ptr->layer_info.pos.y)*dst_layer_ptr->layer_info.width;  
        }
        refresh_width  = right - left + 1;
        refresh_height = bottom - top + 1;          
        src_width = layer_cfg_ptr->width;
        src_buf = (uint16*)layer_ptr->layer_info.buf_addr + (left- layer_cfg_ptr->pos.x) + (top-layer_cfg_ptr->pos.y)*src_width;
        u32_src_buf = (uint32*)layer_ptr->layer_info.buf_addr + (left- layer_cfg_ptr->pos.x) + (top-layer_cfg_ptr->pos.y)*src_width;
        srcjumpcount = src_width-refresh_width;
        dstjumpcount = dst_layer_ptr->layer_info.width-refresh_width;       
        if (DATA_TYPE_ARGB888 == layer_cfg_ptr->layer_data.type)
        {
            if(layer_cfg_ptr->layer_data.alpha_sel ==DISPLAY_BLOCK_ALPHA)
            {
                //SCI_TRACE_LOW:"[_SoftLayerBlend ERROR 0],dont supoort!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_431_112_2_18_1_44_52_2,(uint8*)"");
                return ERR_DISPLAY_FUNC_NOT_SUPPORT;
            }
            DISPLAY_Blend_ARGB888ToARGB888(layer_cfg_ptr->layer_data.colorkey_en,
                                 layer_cfg_ptr->layer_data.colorkey,            
                                 dst_buf, 
                                 u32_src_buf, 
                                 srcjumpcount, 
                                 dstjumpcount, 
                                 refresh_width, 
                                 refresh_height,
                                 layer_cfg_ptr->layer_data.alpha);
        }
        else if (DATA_TYPE_RGB565 == layer_cfg_ptr->layer_data.type)
        {
            if(layer_cfg_ptr->layer_data.alpha_sel ==DISPLAY_PIXEL_ALPHA)
            {
                //SCI_TRACE_LOW:"[_SoftLayerBlend ERROR 1],dont supoort!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_448_112_2_18_1_44_52_3,(uint8*)"");
                return ERR_DISPLAY_FUNC_NOT_SUPPORT;
            }
            DISPLAY_Blend_RGB565ToARGB888(layer_cfg_ptr->layer_data.colorkey_en,
                                 layer_cfg_ptr->layer_data.colorkey, 
                                 dst_buf, 
                                 src_buf,
                                 srcjumpcount, 
                                 dstjumpcount, 
                                 refresh_width, 
                                 refresh_height,
                                 layer_cfg_ptr->layer_data.alpha);
        }
        else if (DATA_TYPE_PMARGB888 == layer_cfg_ptr->layer_data.type)
        {
            DISPLAY_Blend_PMARGB888ToARGB888(layer_cfg_ptr->layer_data.colorkey_en,
                                 layer_cfg_ptr->layer_data.colorkey, 
                                 dst_buf, \
                                 (uint32 *)u32_src_buf, \
                                 srcjumpcount, \
                                 dstjumpcount, \
                                 refresh_width, \
                                 refresh_height,\
                                 layer_cfg_ptr->layer_data.alpha
                                 );
        }
        else
        {
            //SCI_TRACE_LOW:"[_SoftLayerBlend ERROR 2],dont supoort!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_476_112_2_18_1_44_52_4,(uint8*)"");
            return ERR_DISPLAY_FUNC_NOT_SUPPORT;
        }       
        k++;
    }

    return ret;
}
/*****************************************************************************/
//  Description:  single layer blend used for layer convert or transfer
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _SoftLayerConvert(
                                            LCD_ID_E lcd_id,
                                            DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                            LAYER_CONTEXT_T *layer_array, 
                                            DISPLAY_RECT_T *rect_ptr,
                                            uint32 layer_num)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint32 i = 0;        
    DISPLAY_BLEND_LAYER_T dst_layer = {0};
    DISPLAY_LAYER_INFO_T *layer_cfg_ptr = PNULL;    
    uint16 *temp_ptr = (uint16*)layer_array->layer_cfg_ptr->layer_info.buf_addr;
    uint16 *src_buf = PNULL;
    uint16 *dst_buf = PNULL;
    uint32 *dst_u32_buf = PNULL;
    uint16 src_width = 0;
    uint16 left = rect_ptr->left,right = rect_ptr->right;
    uint16 top = rect_ptr->top, bottom = rect_ptr->bottom;
    uint16 refresh_width = right - left + 1;
    uint16 refresh_height = bottom - top + 1;
    uint16 lcd_buf_width = 0;
    LAYER_CONTEXT_T layer_context = {0};
    uint32 length =0;

    dst_layer.layer_info.buf_addr = dst_layer_ptr->layer_info.buf_addr;
    lcd_buf_width = dst_layer_ptr->layer_info.width;
    SCI_PASSERT((dst_layer.layer_info.buf_addr != PNULL),("dst is NULL!"));/*assert verified*/
    dst_layer.layer_info.width  = dst_layer_ptr->layer_info.width;
    dst_layer.layer_info.height = dst_layer_ptr->layer_info.height;
    dst_layer.layer_info.pos.x  = dst_layer_ptr->layer_info.pos.x;
    dst_layer.layer_info.pos.y  = dst_layer_ptr->layer_info.pos.y;
    dst_layer.layer_info.clip_rect = dst_layer_ptr->layer_info.clip_rect;
    layer_context.layer_cfg_ptr = &dst_layer;
    ret = _get_layer_display_rect(&layer_context,rect_ptr);
    if(ret != ERR_DISPLAY_NONE)
    {
        return ret;
    }
    
    layer_cfg_ptr = &layer_array->layer_cfg_ptr->layer_info;
    src_width = layer_cfg_ptr->width;       
    
    if ((DATA_TYPE_ARGB565 == layer_cfg_ptr->layer_data.type) \
         || (DATA_TYPE_RGB565 == layer_cfg_ptr->layer_data.type))
    {
        if((DATA_TYPE_RGB565 == dst_layer_ptr->layer_info.layer_data.type) \
            || (DATA_TYPE_ARGB565 == dst_layer_ptr->layer_info.layer_data.type))
        {
            dst_buf = (uint16*)dst_layer.layer_info.buf_addr+ \
                      (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
                      + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*lcd_buf_width;
            //copy
            src_buf = temp_ptr + (left - layer_cfg_ptr->pos.x) \
                     + ((top - layer_cfg_ptr->pos.y)*src_width);
            length = refresh_width<<1;    
            
            for( i=0 ; i<refresh_height ; i++)
            {
                SCI_MEMCPY((uint8*)dst_buf, (uint8*)src_buf,length); /*lint !e718 !e746 -e718 -e746*/
                dst_buf += lcd_buf_width;
                src_buf += src_width;
            }                
        }
        else if(DATA_TYPE_ARGB888 == dst_layer_ptr->layer_info.layer_data.type)
        {
            uint16 value_565 = 0;
            uint32 j = 0;
            
            dst_u32_buf = (uint32*)dst_layer.layer_info.buf_addr+ \
                      (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
                      + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*lcd_buf_width;
            src_buf = temp_ptr + (left - layer_cfg_ptr->pos.x) \
                     + ((top - layer_cfg_ptr->pos.y)*src_width);           
                
            for( i=0 ; i<refresh_height ; i++)
            {
                for( j=0 ; j<refresh_width ; j++)
                {
                    value_565 = src_buf[j];
                    BLEND_RGB565TORGB888(value_565, dst_u32_buf[j]);
                }
            }  
            dst_u32_buf += lcd_buf_width;
            src_buf     += src_width;
        }
        else//RGB666
        {
            //SCI_TRACE_LOW:"[_SoftLayerConvert ERROR],don't support RGB666!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_575_112_2_18_1_44_53_5,(uint8*)"");
            return ERR_DISPLAY_FUNC_NOT_SUPPORT;
        }
    }
    else if (DATA_TYPE_ARGB888 == layer_cfg_ptr->layer_data.type)
    {
        uint32 *src_u32_buf = PNULL;       
        uint32 value_888 = 0;
        uint32 j = 0;
        //copy
        src_u32_buf = (uint32*)temp_ptr + (left - layer_cfg_ptr->pos.x) \
                    + ((top - layer_cfg_ptr->pos.y)*src_width);
                
        if((DATA_TYPE_RGB565 == dst_layer_ptr->layer_info.layer_data.type) \
            || (DATA_TYPE_ARGB565 == dst_layer_ptr->layer_info.layer_data.type))
        {
            dst_buf = (uint16*)dst_layer.layer_info.buf_addr+ \
                      (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
                      + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*lcd_buf_width;

            for( i=0 ; i<refresh_height ; i++)
            {
                for( j=0 ; j<refresh_width ; j++)
                {
                    value_888 = src_u32_buf[j];
                    dst_buf[j] = RGB8882RGB565(value_888);
                }
                dst_buf     += lcd_buf_width;
                src_u32_buf += src_width;
            }  
            
        }
        else if(DATA_TYPE_ARGB888 == dst_layer_ptr->layer_info.layer_data.type)
        {
            //SCI_TRACE_LOW:"[_SoftLayerConvert ERROR],don't support RGB888!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_609_112_2_18_1_44_53_6,(uint8*)"");
            return ERR_DISPLAY_FUNC_NOT_SUPPORT;
        }
        else
        {
            //SCI_TRACE_LOW:"[_SoftLayerConvert ERROR],don't support output RGB666!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_614_112_2_18_1_44_53_7,(uint8*)"");
            return ERR_DISPLAY_FUNC_NOT_SUPPORT;
        }
    }
    else//RGB666
    {
        //SCI_TRACE_LOW:"[_SoftLayerConvert ERROR],don't support RGB666 convertion!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_620_112_2_18_1_44_53_8,(uint8*)"");
        return ERR_DISPLAY_FUNC_NOT_SUPPORT;
    }

    return ERR_DISPLAY_NONE;
}
LOCAL ERR_DISPLAY_E _SoftLayerConvertPMARGB888(
                                            LCD_ID_E lcd_id,
                                            DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                            LAYER_CONTEXT_T *layer_array, 
                                            DISPLAY_RECT_T *rect_ptr,
                                            uint32 layer_num)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    DISPLAY_BLEND_LAYER_T dst_layer = {0};
    DISPLAY_LAYER_INFO_T *layer_cfg_ptr = PNULL;    
    uint16 *temp_ptr = (uint16*)layer_array->layer_cfg_ptr->layer_info.buf_addr;
    uint32 *dst_buf32 = PNULL;
    uint16 *src_basebuf = PNULL;
    uint32 *dst_basebuf32 = PNULL;
    uint16 src_width = 0;
    uint16 left = rect_ptr->left,right = rect_ptr->right;
    uint16 top = rect_ptr->top, bottom = rect_ptr->bottom;
    uint16 refresh_width = right - left + 1;
    uint16 refresh_height = bottom - top + 1;
    uint16 target_width = 0;
    LAYER_CONTEXT_T layer_context = {0};
    uint32 length =0;
    uint32 colorkey = dst_layer.layer_info.layer_data.colorkey;
    uint32 colorkey_en = dst_layer.layer_info.layer_data.colorkey_en;
    dst_layer.layer_info.buf_addr = dst_layer_ptr->layer_info.buf_addr;
    target_width = dst_layer_ptr->layer_info.width;
    SCI_PASSERT((dst_layer.layer_info.buf_addr != PNULL),("dst is NULL!"));/*assert verified*/
    dst_layer.layer_info.width  = dst_layer_ptr->layer_info.width;
    dst_layer.layer_info.height = dst_layer_ptr->layer_info.height;
    dst_layer.layer_info.pos.x  = dst_layer_ptr->layer_info.pos.x;
    dst_layer.layer_info.pos.y  = dst_layer_ptr->layer_info.pos.y;
    layer_context.layer_cfg_ptr = &dst_layer;
    ret = _get_layer_display_rect(&layer_context,rect_ptr);
    if(ret != ERR_DISPLAY_NONE)
    {
        return ret;
    }
    layer_cfg_ptr = &layer_array->layer_cfg_ptr->layer_info;
    src_width = layer_cfg_ptr->width;       
    if (DATA_TYPE_RGB565 == layer_cfg_ptr->layer_data.type)
    {
            dst_basebuf32 = (uint32*)dst_layer.layer_info.buf_addr+ \
                      (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
                      + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*target_width;
            src_basebuf = temp_ptr + (left - layer_cfg_ptr->pos.x) \
                     + ((top - layer_cfg_ptr->pos.y)*src_width);
            length = refresh_width<<1;    
            DISPLAY_RGB565ToPMARGB888(dst_basebuf32, src_basebuf, target_width, src_width, refresh_width, refresh_height, colorkey,colorkey_en);
    } 
    else if (DATA_TYPE_ARGB888 == layer_cfg_ptr->layer_data.type)
    {
        uint32 *src_u32_buf = PNULL;       
        src_u32_buf = (uint32*)temp_ptr + (left - layer_cfg_ptr->pos.x) \
                    + ((top - layer_cfg_ptr->pos.y)*src_width);
        dst_buf32 = (uint32*)dst_layer.layer_info.buf_addr+ \
                      (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
                      + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*target_width;
          DISPLAY_ARGB888ToPMARGB888(dst_basebuf32, (uint32 *)src_basebuf, target_width, src_width, refresh_width, refresh_height, colorkey,colorkey_en);
    }
    else//RGB666
    {
        //SCI_TRACE_LOW:"[_SoftLayerConvert ERROR],don't support RGB666 convertion!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_687_112_2_18_1_44_53_9,(uint8*)"");
        return ERR_DISPLAY_FUNC_NOT_SUPPORT;
    }
    return ERR_DISPLAY_NONE;
}
/*****************************************************************************/
//  Description:  multi layer blend invalidate lcd using software blend
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _SoftInvalidateRectUsingLayer(LCD_ID_E lcd_id,DISPLAY_BLEND_LAYER_T *dst_layer_ptr,LAYER_CONTEXT_T *layer_array, DISPLAY_RECT_T *rect_ptr,uint32 layer_num)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint32 i = 0;       
    DISPLAY_BLEND_LAYER_T dst_layer = {0};
    DISPLAY_LAYER_INFO_T *layer_cfg_ptr = PNULL;   
    uint16 *temp_ptr = (uint16*)layer_array->layer_cfg_ptr->layer_info.buf_addr;
    uint16 *src_buf = PNULL;
    uint16 *dst_buf = PNULL;
    uint32 *dst_u32_buf = PNULL;
    uint16 src_width = 0;
    uint16 left = rect_ptr->left,right = rect_ptr->right;
    uint16 top = rect_ptr->top, bottom = rect_ptr->bottom;
    uint16 refresh_width = right - left + 1;
    uint16 refresh_height = bottom - top + 1;
    uint16 lcd_buf_width = 0;
    LAYER_CONTEXT_T layer_context = {0};
    LAYER_CONTEXT_T temp_layer_context = {0};
    DISPLAY_BLEND_LAYER_T temp_layer = {0};
   
    uint32 *dst_u32_buf_copy = PNULL;
    
    if(dst_layer_ptr == PNULL)
    {       
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    
    dst_layer.layer_info.buf_addr = dst_layer_ptr->layer_info.buf_addr;
    lcd_buf_width = dst_layer_ptr->layer_info.width;
    SCI_PASSERT((dst_layer.layer_info.buf_addr != PNULL),("dst is NULL!"));/*assert verified*/
    dst_layer.layer_info.width  = dst_layer_ptr->layer_info.width;
    dst_layer.layer_info.height = dst_layer_ptr->layer_info.height;
    dst_layer.layer_info.pos.x  = dst_layer_ptr->layer_info.pos.x;
    dst_layer.layer_info.pos.y  = dst_layer_ptr->layer_info.pos.y;
    dst_layer.layer_info.layer_data.type = dst_layer_ptr->layer_info.layer_data.type;
    dst_layer.layer_info.clip_rect = dst_layer_ptr->layer_info.clip_rect;
    layer_context.layer_cfg_ptr = &dst_layer;
    ret = _get_layer_display_rect(&layer_context,rect_ptr);
    if(ret != ERR_DISPLAY_NONE)
    {
        return ret;
    }

    if((1 == layer_num) && ((uint32)dst_layer.layer_info.buf_addr!= (uint32)temp_ptr))
    {
        ret = _SoftLayerConvert(lcd_id,dst_layer_ptr,layer_array, rect_ptr,layer_num);
    }
    else
    {
        if((DATA_TYPE_ARGB888 != dst_layer_ptr->layer_info.layer_data.type) \
           && (DATA_TYPE_ARGB666 != dst_layer_ptr->layer_info.layer_data.type))
        {             
            lcd_buf_width = (right-left+1);
            dst_u32_buf = SCI_ALLOCA(lcd_buf_width*(bottom-top+1)*4);  
            if(!dst_u32_buf)
            {
                //SCI_TRACE_LOW:"---_SoftInvalidateRectUsingLayer:malloc fial!---"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_751_112_2_18_1_44_53_10,(uint8*)"");
                return ERR_DISPLAY_OPERATE_FAIL;
            }
            dst_u32_buf_copy = dst_u32_buf;
            temp_layer_context.disp_rect.left   = left;
            temp_layer_context.disp_rect.top    = top;
            temp_layer_context.disp_rect.right  = right;
            temp_layer_context.disp_rect.bottom = bottom;
            temp_layer_context.layer_cfg_ptr = &temp_layer;
            temp_layer.layer_info.buf_addr = (uint32)dst_u32_buf_copy;
            temp_layer.layer_info.is_enable = TRUE;
            temp_layer.layer_info.layer_id = 0;
            temp_layer.layer_info.pos.x = left;
            temp_layer.layer_info.pos.y = top;
            temp_layer.layer_info.width = (right-left+1);
            temp_layer.layer_info.height = (bottom-top+1);
            temp_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
            temp_layer.layer_info.layer_data.type = DATA_TYPE_ARGB888;
        }
        else
        {
            lcd_buf_width = dst_layer.layer_info.width;
            dst_u32_buf_copy = 0;
            dst_u32_buf = (uint32*)dst_layer.layer_info.buf_addr+ \
               (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
               + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*lcd_buf_width;
            temp_layer_context.disp_rect.left   = left;
            temp_layer_context.disp_rect.top    = top;
            temp_layer_context.disp_rect.right  = right;
            temp_layer_context.disp_rect.bottom = bottom;
            temp_layer_context.layer_cfg_ptr = &temp_layer;
            temp_layer.layer_info.buf_addr = (uint32)dst_u32_buf;
            temp_layer.layer_info.is_enable = TRUE;
            temp_layer.layer_info.layer_id = 0;
            temp_layer.layer_info.pos.x = left;
            temp_layer.layer_info.pos.y = top;
            temp_layer.layer_info.width = dst_layer.layer_info.width;
            temp_layer.layer_info.height = (bottom-top+1);
            temp_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
            temp_layer.layer_info.layer_data.type = dst_layer.layer_info.layer_data.type;             
        }
        if(((uint32)dst_layer.layer_info.buf_addr!= (uint32)temp_ptr) \
           ||(((uint32)dst_layer.layer_info.buf_addr== (uint32)temp_ptr))&&(DATA_TYPE_ARGB888 != dst_layer.layer_info.layer_data.type))
        {        
            uint32 length =0;                            
            
            layer_cfg_ptr = &layer_array->layer_cfg_ptr->layer_info;            
            src_width = layer_cfg_ptr->width;  
            if (DATA_TYPE_ARGB888 == layer_cfg_ptr->layer_data.type)
            {
                uint32 *src_u32_buf = PNULL;               
                
                src_u32_buf = (uint32*)temp_ptr;
                src_u32_buf += (left - layer_cfg_ptr->pos.x) + ((top - layer_cfg_ptr->pos.y)*src_width);
                length = refresh_width*sizeof(uint32);      
                for( i=0 ; i<refresh_height ; i++)
                {
                    SCI_MEMCPY((void*)dst_u32_buf,(void*)src_u32_buf,length);
                    dst_u32_buf += lcd_buf_width;
                    src_u32_buf += src_width;                                
                }
            }
            else if ((DATA_TYPE_ARGB565 == layer_cfg_ptr->layer_data.type) \
                      ||(DATA_TYPE_RGB565 == layer_cfg_ptr->layer_data.type))
            {
                uint32 j = 0;
                uint16 value_565 = 0;
                uint32 value_888 = 0;                                          
                
                src_buf = temp_ptr + (left - layer_cfg_ptr->pos.x) \
                             + ((top - layer_cfg_ptr->pos.y)*src_width);
    
                for( i=0 ; i<refresh_height ; i++)
                {
                   for( j=0 ; j<refresh_width ; j++)
                   {    
                        value_565 = src_buf[j];
                        BLEND_RGB565TORGB888(value_565, value_888);                          
                        dst_u32_buf[j] = value_888;                        
                   }
                   src_buf += src_width;
                   dst_u32_buf += lcd_buf_width;
                }
            }
        }     
        
        ret = _SoftLayerBlend(&temp_layer,layer_array, rect_ptr,layer_num,dst_u32_buf_copy);

        if(ERR_DISPLAY_NONE != ret)
        {
            return ret;
        }
       
        if(dst_u32_buf_copy)
        {           
            uint32 j = 0;
            uint32 value = 0;
            
            lcd_buf_width = dst_layer_ptr->layer_info.width;
            dst_buf = (uint16*)dst_layer.layer_info.buf_addr+ \
               (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
               + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*lcd_buf_width;
            dst_u32_buf = dst_u32_buf_copy;
            for( i=0 ; i<refresh_height ; i++)
            {                
                for( j=0 ; j<refresh_width ; j++)
                {                                   
                    value = dst_u32_buf[j];
                    dst_buf[j] = RGB8882RGB565(value);                 
                }
                dst_u32_buf += refresh_width;
                dst_buf += lcd_buf_width;
            }
        }
        
    }
    if(dst_u32_buf_copy)
    {
        SCI_FREE(dst_u32_buf_copy);
    } 
    return ret;
    
}
LOCAL ERR_DISPLAY_E _SoftInvalidateRectUsingLayer_Ext(LCD_ID_E lcd_id,DISPLAY_BLEND_LAYER_T *dst_layer_ptr,LAYER_CONTEXT_T *layer_array, DISPLAY_RECT_T *rect_ptr,uint32 layer_num)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    DISPLAY_BLEND_LAYER_T dst_layer = {0};
    uint32 *dst_u32_buf = PNULL;
    uint16 left = rect_ptr->left,right = rect_ptr->right;
    uint16 top = rect_ptr->top, bottom = rect_ptr->bottom;
    uint16 lcd_buf_width = 0;
    LAYER_CONTEXT_T layer_context = {0};
    LAYER_CONTEXT_T temp_layer_context = {0};
    DISPLAY_BLEND_LAYER_T temp_layer = {0};
    uint32 *dst_u32_buf_copy = PNULL;
    if(dst_layer_ptr == PNULL)
    {       
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    dst_layer.layer_info.buf_addr = dst_layer_ptr->layer_info.buf_addr;
    lcd_buf_width = dst_layer_ptr->layer_info.width;
    if(dst_layer.layer_info.buf_addr == PNULL)
    {       
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    dst_layer.layer_info.width  = dst_layer_ptr->layer_info.width;
    dst_layer.layer_info.height = dst_layer_ptr->layer_info.height;
    dst_layer.layer_info.pos.x  = dst_layer_ptr->layer_info.pos.x;
    dst_layer.layer_info.pos.y  = dst_layer_ptr->layer_info.pos.y;
    dst_layer.layer_info.layer_data.type = dst_layer_ptr->layer_info.layer_data.type;
    layer_context.layer_cfg_ptr = &dst_layer;
    ret = _get_layer_display_rect(&layer_context,rect_ptr);
    if(ret != ERR_DISPLAY_NONE)
    {
        return ret;
    }
    if((1 == layer_num) && ((uint32)dst_layer.layer_info.buf_addr!= (uint32)0))
    {
        ret = _SoftLayerConvertPMARGB888(lcd_id,dst_layer_ptr,layer_array, rect_ptr,layer_num);
    }
    else
    {
        {
            lcd_buf_width = dst_layer.layer_info.width;
            dst_u32_buf_copy = 0;
            dst_u32_buf = (uint32*)dst_layer.layer_info.buf_addr+ \
               (layer_context.disp_rect.left-layer_context.layer_cfg_ptr->layer_info.pos.x) \
               + (layer_context.disp_rect.top-layer_context.layer_cfg_ptr->layer_info.pos.y)*lcd_buf_width;
            temp_layer_context.disp_rect.left   = left;
            temp_layer_context.disp_rect.top    = top;
            temp_layer_context.disp_rect.right  = right;
            temp_layer_context.disp_rect.bottom = bottom;
            temp_layer_context.layer_cfg_ptr = &temp_layer;
            temp_layer.layer_info.buf_addr = (uint32)dst_u32_buf;
            temp_layer.layer_info.is_enable = TRUE;
            temp_layer.layer_info.layer_id = 0;
            temp_layer.layer_info.pos.x = left;
            temp_layer.layer_info.pos.y = top;
            temp_layer.layer_info.width = dst_layer.layer_info.width;
            temp_layer.layer_info.height = (bottom-top+1);
            temp_layer.layer_info.layer_data.alpha_sel = DISPLAY_BLOCK_ALPHA;
            temp_layer.layer_info.layer_data.type = dst_layer.layer_info.layer_data.type;             
        }
        ret = _SoftLayerBlend2PMARGB888(&temp_layer,layer_array, rect_ptr,layer_num,dst_u32_buf_copy);
        if(ERR_DISPLAY_NONE != ret)
        {
            return ret;
        }
    }
    return ret;
}
/*****************************************************************************/
//  Description:  computer byte number of pixel
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
__inline uint32 covert_to_byte_num(DISPLAY_DATA_TYPE_E data_format)
{
    uint32 bytes_num = 0;
    switch(data_format)
    {
        case DATA_TYPE_YUV422:  
        case DATA_TYPE_YUV420:
        case DATA_TYPE_YUV400:
        case DATA_TYPE_GREY:
            bytes_num = 1;
                break;
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_ARGB666:
        case DATA_TYPE_PMARGB888:
        case DATA_TYPE_PMARGB666:
            bytes_num = 4;
            break;
        case DATA_TYPE_ARGB565:            
        case DATA_TYPE_RGB565:
        case DATA_TYPE_ARGB555:
        case DATA_TYPE_PMARGB565:
            bytes_num = 2;
            break;          
            default:
            //LAYER_HANDLE_TRACE:"covert_to_byte_num:result is 0,data_format is %d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_970_112_2_18_1_44_53_11,(uint8*)"d",data_format);
                break;
    }
    return bytes_num;
}
/*****************************************************************************/
//  Description:  adjust layer configuration
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
__inline void _adjust_layer_config(LAYER_CONTEXT_T *layer_context_ptr,uint32 layer_num)
{
    int32 i = 0;    
    int32 offset_x = 0;
    int32 offset_y = 0;
    uint32 bytes_num = 0;
    uint32 buffer_width;
    uint32 addr_offset = 0;
    DISPLAY_BLEND_LAYER_T *layer_cfg_ptr = PNULL;
    DISPLAY_RECT_T     *disp_rect_ptr = PNULL;

    layer_cfg_ptr = layer_context_ptr[i].layer_cfg_ptr;
    disp_rect_ptr = &layer_context_ptr[i].disp_rect;
    bytes_num = covert_to_byte_num(layer_cfg_ptr->layer_info.layer_data.type);
    offset_x = layer_cfg_ptr->layer_info.pos.x;
    offset_y = layer_cfg_ptr->layer_info.pos.y;
    buffer_width = layer_cfg_ptr->layer_info.width;

    addr_offset = ((disp_rect_ptr->left-offset_x)+(disp_rect_ptr->top-offset_y)*buffer_width)*bytes_num;
    layer_cfg_ptr->layer_info.buf_addr+= addr_offset;
    
    //computer address
    switch(layer_cfg_ptr->layer_info.layer_data.type)
    {
        case DATA_TYPE_YUV422:                  
            layer_cfg_ptr->layer_info.uv_buf_addr += addr_offset;
            break;
        case DATA_TYPE_YUV420:              
            addr_offset = ((disp_rect_ptr->left-offset_x)+((disp_rect_ptr->top-offset_y)>>1)*buffer_width)*bytes_num;
            layer_cfg_ptr->layer_info.uv_buf_addr += addr_offset;
            break;
        case DATA_TYPE_YUV400:          
            layer_cfg_ptr->layer_info.uv_buf_addr = 0;
            break;
        case DATA_TYPE_ARGB565:
            if(layer_cfg_ptr->layer_info.layer_data.alpha_sel == DISPLAY_PIXEL_ALPHA)
            {
                layer_cfg_ptr->layer_info.alpha_buf_addr += (addr_offset>>1);
            }
            break;
        default:
            break;
    }       
}
/*****************************************************************************/
//  Description:  Configure layer on hw platform
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL int32 _ConfigLayer(LCD_ID_E lcd_id,LAYER_CONTEXT_T *layer_array,DISPLAY_RECT_T *rect_ptr,uint32 layer_num)
{
#if !SOFTWARE_BLEND_LAYER
    uint32 i = 0;
    BLOCKCFG_T blk_cfg = {0};
    DISPLAY_BLEND_LAYER_T *layer_ptr = PNULL;
    uint32 blk_addr  = 0;       
    uint8 data_type = LAYER_ERROR_DATA_TYPE;
    LAYER_CONTEXT_T *temp_context_ptr = PNULL;  
    
    for( i=0 ; i<layer_num ; i++)
    {
        temp_context_ptr = &layer_array[i];
        layer_ptr = temp_context_ptr->layer_cfg_ptr;            
        if(layer_ptr != PNULL)
        {           
            data_type = _data_type_convert(layer_ptr->layer_info.layer_data.type);  
            blk_addr = layer_ptr->layer_info.buf_addr;      
            #if 0
            blk_cfg.start_x = layer_ptr->layer_info.pos.x; 
            blk_cfg.start_y = layer_ptr->layer_info.pos.y;
            blk_cfg.end_x  = layer_ptr->layer_info.width + layer_ptr->layer_info.pos.x-1;
            blk_cfg.end_y  = layer_ptr->layer_info.height + layer_ptr->layer_info.pos.y-1;
            #else
            blk_cfg.start_x = temp_context_ptr->disp_rect.left;
            blk_cfg.start_y = temp_context_ptr->disp_rect.top;
            blk_cfg.end_x  = temp_context_ptr->disp_rect.right;
            blk_cfg.end_y  = temp_context_ptr->disp_rect.bottom;
            #endif
            if((blk_cfg.start_x>=blk_cfg.end_x) ||(blk_cfg.start_y>=blk_cfg.end_y))
            {
                //SCI_TRACE_LOW:"[_ConfigLayer ERROR],layer position!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1058_112_2_18_1_44_54_12,(uint8*)"");
                return SCI_ERROR;
            }
            
            blk_cfg.colorkey_en = layer_ptr->layer_info.layer_data.colorkey_en;           
            blk_cfg.colorkey    = layer_ptr->layer_info.layer_data.colorkey;
            blk_cfg.width       = layer_ptr->layer_info.width;  
            
            blk_cfg.resolution = data_type;         
            blk_cfg.alpha       = layer_ptr->layer_info.layer_data.alpha;
            if(DATA_TYPE_PMARGB888 == layer_ptr->layer_info.layer_data.type
                ||DATA_TYPE_PMARGB666 == layer_ptr->layer_info.layer_data.type
                ||DATA_TYPE_PMARGB565 == layer_ptr->layer_info.layer_data.type)
            {
                blk_cfg.alpha_sel = DISPLAY_PIXEL_ALPHA;
                blk_cfg.blend_mode= LCDC_OSD_BLEND_SPECIAL; 
            }
            else
            {
                blk_cfg.alpha_sel = layer_ptr->layer_info.layer_data.alpha_sel;
                blk_cfg.blend_mode= LCDC_OSD_BLEND_NORMAL; 
            }
            blk_cfg.grey_rgb      = layer_ptr->layer_info.layer_data.grey_value;
            LCD_ConfigBlock(lcd_id,i,blk_cfg);            
            LCD_SetBlockBuffer(lcd_id, i, (uint32*)blk_addr);
            
            if(i==0)
            {
                if((layer_ptr->layer_info.layer_data.type == DATA_TYPE_YUV422) || (layer_ptr->layer_info.layer_data.type == DATA_TYPE_YUV420))
                {
                    LCD_SetUVBuffer(lcd_id,i,(uint32*)layer_ptr->layer_info.uv_buf_addr);
                }
            }
            
            if(i==LAYER_TYPE_OSD_WITH_PIXEL_ALPHA)
            {
                if((layer_ptr->layer_info.layer_data.type == DATA_TYPE_ARGB565) && (layer_ptr->layer_info.layer_data.alpha_sel==DISPLAY_PIXEL_ALPHA))
                {
                    LCD_SetPixelAlphaBuffer(lcd_id,i,(uint32*)layer_ptr->layer_info.alpha_buf_addr);                    
                }
            }       
            LCD_EnableBlock(lcd_id,i);
        }
   }
#endif       
   return SCI_SUCCESS;
}
/*****************************************************************************/
//  Description:  convert data type when cap mode
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL LCD_DATA_FORMAT_E _cap_data_type_convert(DISPLAY_DATA_TYPE_E type)
{
    LCD_DATA_FORMAT_E ret = LCD_RGB565;
    switch(type)
    {
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_PMARGB888:
            ret = LCD_RGB888;
            break;
        case DATA_TYPE_ARGB666:
        case DATA_TYPE_PMARGB666:
            ret = LCD_RGB666;
            break;
        case DATA_TYPE_ARGB565:
        case DATA_TYPE_RGB565:
        case DATA_TYPE_PMARGB565:
            ret= LCD_RGB565;
            break;  
        default:
            //LAYER_HANDLE_TRACE:"_cap_data_type_convert:default rgb565,type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1128_112_2_18_1_44_54_13,(uint8*)"d",type);
            ret = LCD_RGB565;
            break;
    }
    return ret;
}
/*****************************************************************************/
//  Description:  invalidate on hardware
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void _invalidateRect(LCD_ID_E lcd_id,DISPLAY_BLEND_LAYER_T *dst_layer_ptr,DISPLAY_RECT_T *rect_ptr)
{
#if !SOFTWARE_BLEND_LAYER   
    LCD_CAP_PARAM_T cap_param = {0};
    uint16 rect_w = 0;
    uint16 rect_h = 0;
    
    if(dst_layer_ptr != PNULL)  
    {       
        cap_param.cap_en = TRUE;
        cap_param.dst_base_addr = dst_layer_ptr->layer_info.buf_addr;
        rect_w = rect_ptr->right-rect_ptr->left+1;
        rect_h = rect_ptr->bottom - rect_ptr->top + 1;
        if(rect_w > dst_layer_ptr->layer_info.width)
        {
            rect_w = dst_layer_ptr->layer_info.width;
        }            
        if(rect_h > dst_layer_ptr->layer_info.height)
        {
            rect_h = dst_layer_ptr->layer_info.height;
        }
        cap_param.cap_rect.x = rect_ptr->left;
        cap_param.cap_rect.y = rect_ptr->top;
        cap_param.cap_rect.w = rect_w;
        cap_param.cap_rect.h = rect_h;
        cap_param.dst_size.w = dst_layer_ptr->layer_info.width;
        cap_param.dst_size.h = dst_layer_ptr->layer_info.height;
        cap_param.dst_trim_rect.x = 0;
        cap_param.dst_trim_rect.y = 0;
        cap_param.dst_trim_rect.w = rect_w;
        cap_param.dst_trim_rect.h = rect_h;  
        cap_param.format = (LCD_DATA_FORMAT_E)_cap_data_type_convert(dst_layer_ptr->layer_info.layer_data.type);  
        cap_param.rotation = dst_layer_ptr->layer_info.layer_data.rotation;
        cap_param.rb_switch = dst_layer_ptr->layer_info.layer_data.rb_switch;
        if(DATA_TYPE_PMARGB888 == dst_layer_ptr->layer_info.layer_data.type
           || DATA_TYPE_PMARGB666 == dst_layer_ptr->layer_info.layer_data.type
           || DATA_TYPE_PMARGB565 == dst_layer_ptr->layer_info.layer_data.type)
        {
            cap_param.cap_mode = LCDC_CAP_ONLY_OSD;
        }
        else
        {
            cap_param.cap_mode = LCDC_CAP_NORMAL;
        }
        LCD_ConfigCapture(lcd_id,&cap_param);
    }
    
    LCD_InvalidateRect(lcd_id,rect_ptr->left,rect_ptr->top,rect_ptr->right,rect_ptr->bottom);
    if(dst_layer_ptr != PNULL)  
    {
        cap_param.cap_en = FALSE;
        LCD_ConfigCapture(lcd_id,&cap_param);
    }
#endif    

}
/*****************************************************************************/
//  Description:  display all LCDC layers
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
__inline void _disable_layer(LCD_ID_E lcd_id)
{
    uint32 i = 0;
    for( i=0 ; i<HW_BLOCK_NUM ; i++)
    {
        LCD_DisableBlock(lcd_id, i);
    }
}
/*****************************************************************************/
//  Description:  multiplay blend on hardware
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _HW_InvalidateRectUsingLayer(LCD_ID_E lcd_id, 
                                                    DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                                    LAYER_CONTEXT_T *layer_array, 
                                                    DISPLAY_RECT_T *rect_ptr, 
                                                    uint32 layer_num)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;   
    uint32 i = 0;
    LAYER_CONTEXT_T temp_layer_array[HW_BLOCK_NUM] = {0};   
    uint8 *mem_addr = PNULL,*mem_addr_copy = PNULL;
    uint16 left = rect_ptr->left,right = rect_ptr->right;
    uint16 top = rect_ptr->top,bottom = rect_ptr->bottom;
    uint16 width  = right -left + 1;
    uint16 height =  bottom - top + 1;
    DISPLAY_BLEND_LAYER_T dst_temp_layer0={0};
    BOOLEAN layer_blend_fashion = SCI_FALSE;
    uint32 byte_num_pixel = 4;
    DISPLAY_DATA_TYPE_E mem_data_type = DATA_TYPE_RGB565;

    for( i=0 ; i<HW_BLOCK_NUM ; i++)
    {
        LCD_DisableBlock(lcd_id, i);
    }
    //SCI_TRACE_LOW:"_HW_InvalidateRectUsingLayer:layer_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1233_112_2_18_1_44_54_14,(uint8*)"d",layer_num);

    for( i=0 ; i<layer_num ; i++)
    {
        if(DATA_TYPE_ARGB565 == layer_array[i].layer_cfg_ptr->layer_info.layer_data.type)
        {
            layer_blend_fashion = SCI_TRUE;
            break;
        }
    }
   
    if((layer_blend_fashion && (layer_num>2))||(!layer_blend_fashion && (layer_num>HW_BLOCK_NUM)))
    {
        if(dst_layer_ptr)
        {
            byte_num_pixel = 4;
            mem_data_type = DATA_TYPE_ARGB888;
        }
        mem_addr_copy = (uint8*)SCI_ALLOC_APP(width*height*byte_num_pixel+8);
        SCI_PASSERT((mem_addr_copy!=PNULL),("Multiple layer malloc memory is fail!"));/*assert verified*/
        mem_addr = (((uint32)mem_addr_copy & 7) == 0) ? mem_addr_copy : (mem_addr_copy+(8-((uint32)mem_addr_copy&7)));        
        dst_temp_layer0.layer_info.buf_addr = (uint32)mem_addr;
        dst_temp_layer0.layer_info.layer_id = 0;
        dst_temp_layer0.layer_info.layer_data.type = mem_data_type;
        dst_temp_layer0.layer_info.pos.x = left;
        dst_temp_layer0.layer_info.pos.y = top;
        dst_temp_layer0.layer_info.width     = width;
        dst_temp_layer0.layer_info.height    = height; 
    } 
        
    temp_layer_array[0] = layer_array[0];

    if(layer_num>2)
    {
        if(!layer_blend_fashion)
        {
            if(layer_num <= HW_BLOCK_NUM)
            {   
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,layer_array,rect_ptr,layer_num))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                _invalidateRect(lcd_id,dst_layer_ptr,rect_ptr);
            }
            else if(layer_num<=(LAYER_SUPPORT_NUM-1))
            {
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,layer_array,rect_ptr,HW_BLOCK_NUM))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                _invalidateRect(lcd_id,&dst_temp_layer0,rect_ptr);
                _disable_layer(lcd_id);
                temp_layer_array[0].layer_cfg_ptr->layer_info.layer_data.type = DATA_TYPE_ARGB888;
                temp_layer_array[0].layer_cfg_ptr = &dst_temp_layer0;            
                temp_layer_array[1] = layer_array[HW_BLOCK_NUM];
#if (4 == LCDC_LAYER_NUM)         
                temp_layer_array[2] = layer_array[HW_BLOCK_NUM+1];
                temp_layer_array[3] = layer_array[HW_BLOCK_NUM+2];
#endif            
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,temp_layer_array,rect_ptr,(layer_num-HW_BLOCK_NUM+1)))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                _invalidateRect(lcd_id,dst_layer_ptr,rect_ptr);
            }
            else
            {
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,layer_array,rect_ptr,HW_BLOCK_NUM))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                _invalidateRect(lcd_id,&dst_temp_layer0,rect_ptr);
#if (4 == LCDC_LAYER_NUM)         
                temp_layer_array[0].layer_cfg_ptr = &dst_temp_layer0;
                temp_layer_array[1] = layer_array[HW_BLOCK_NUM];
                temp_layer_array[2] = layer_array[HW_BLOCK_NUM+1];
                temp_layer_array[3] = layer_array[HW_BLOCK_NUM+2];
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,temp_layer_array,rect_ptr,HW_BLOCK_NUM))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                _invalidateRect(lcd_id,&dst_temp_layer0,rect_ptr);
                temp_layer_array[0].layer_cfg_ptr = &dst_temp_layer0;
                temp_layer_array[1] = layer_array[7];
                _disable_layer(lcd_id);
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,temp_layer_array,rect_ptr,2))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                _invalidateRect(lcd_id,dst_layer_ptr,rect_ptr);
#else
                temp_layer_array[0].layer_cfg_ptr = &dst_temp_layer0;
                temp_layer_array[1] = layer_array[HW_BLOCK_NUM];
                temp_layer_array[2] = layer_array[HW_BLOCK_NUM+1];
                _disable_layer(lcd_id);
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,temp_layer_array,rect_ptr,3))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                _invalidateRect(lcd_id,dst_layer_ptr,rect_ptr);                
#endif                
            }
        }
        else
        {
            for( i=1 ; i<layer_num ; i++)
            {
                temp_layer_array[1] = layer_array[i];
                if(SCI_SUCCESS != _ConfigLayer(lcd_id,temp_layer_array,rect_ptr,2))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                    goto end_handle;
                }
                if((i+1)==layer_num)
                {
                    _invalidateRect(lcd_id,dst_layer_ptr,rect_ptr);
                    break;
                }
                else
                {
                    _invalidateRect(lcd_id,&dst_temp_layer0,rect_ptr);
                    temp_layer_array[0].layer_cfg_ptr = &dst_temp_layer0;
                }
            }
         } 
    }
    else
    {
        temp_layer_array[0] = layer_array[0];
        temp_layer_array[1] = layer_array[1];
        if(SCI_SUCCESS != _ConfigLayer(lcd_id,temp_layer_array,rect_ptr,2))
        {
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            goto end_handle;
        }
        _invalidateRect(lcd_id,dst_layer_ptr,rect_ptr);
    }
end_handle:    
    if(mem_addr_copy != PNULL)
    {
        SCI_FREE(mem_addr_copy);
        mem_addr_copy = PNULL;
    }    

    for( i=0 ; i<HW_BLOCK_NUM ; i++)
    {
        LCD_DisableBlock(lcd_id, i);
    }

    return ret;
            
}
/*****************************************************************************/
//  Description:  multi layer blend interface for the APP
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _SoftwareHandle(  LCD_ID_E lcd_id,
                                        DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                        DISPLAY_BLEND_LAYER_T *layer_arr[],
                                        uint32 array_size,
                                        DISPLAY_RECT_T *rect_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint32 layer_num = 0;
    uint32 i = 0,k = 0;
    LAYER_CONTEXT_T layer_array[LAYER_SUPPORT_NUM+1]={0};
    uint32 err_num = 0;    
    DISPLAY_BLEND_LAYER_T save_layer_arr[LAYER_SUPPORT_NUM]={0};
    DISPLAY_BLEND_LAYER_T dst_layer = {0};
    DISPLAY_LAYER_INFO_T *layer_info = NULL;
    DISPLAY_RECT_T *clip_rect = NULL;

    if(LAYER_SUPPORT_NUM<array_size)
    {
        //LAYER_HANDLE_TRACE:"--DISPLAY_BlendLayer:--array_size=%d--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1414_112_2_18_1_44_54_15,(uint8*)"d",array_size);
        return ERR_DISPLAY_PARAMETER_WRONG;
    }    

    if(dst_layer_ptr != 0)
    {             
        dst_layer = *dst_layer_ptr;
        layer_info = &dst_layer.layer_info;
        clip_rect  = &layer_info->clip_rect;
        if (_check_clip_rect(clip_rect,layer_info->width, layer_info->height)
                != ERR_DISPLAY_NONE)
        {
             return ERR_DISPLAY_PARAMETER_WRONG;
        }   
    }

    ret = _check_layer_parameter(layer_arr,array_size,SCI_TRUE);
    if(ERR_DISPLAY_NONE != ret)
        return ret;

    for( i=0 ; i<array_size ; i++)
    {
        save_layer_arr[i] = *layer_arr[i];
    }

    for( i=0 ; i<array_size ; i++)
    {
        layer_array[i].layer_cfg_ptr = &save_layer_arr[i];
    }

    for( i=0 ; i<array_size ; i++)
    {
         if(layer_array[i].layer_cfg_ptr != PNULL)
         {
            if(layer_array[i].layer_cfg_ptr->layer_info.is_enable)
            {
                layer_info = &layer_array[i].layer_cfg_ptr->layer_info;
                clip_rect = &layer_info->clip_rect;
                if (_check_clip_rect(clip_rect,layer_info->width, layer_info->height)
                        != ERR_DISPLAY_NONE)
                {
                    continue;
                }
                layer_num++;
                layer_array[k++].layer_cfg_ptr = layer_array[i].layer_cfg_ptr;
            }
         }
    }

    //compute the layer's display rectangle
    for( i=0 ; i<layer_num ; i++)
    {
        if(ERR_DISPLAY_NONE != _get_layer_display_rect(&layer_array[i],rect_ptr))
        {          
            layer_array[i].layer_cfg_ptr = PNULL;
            err_num++;
        }
    }
    if(err_num != 0)
    {
        if(err_num >= layer_num)
        {
            return ERR_DISPLAY_PARAMETER_WRONG;
        }
        else
        {
            k=0;
            layer_num -= err_num;
            for( i=0 ; i<LAYER_SUPPORT_NUM ; i++)
            {
                if(layer_array[i].layer_cfg_ptr!=0)
                {
                    layer_array[k] = layer_array[i];
                    k++;
                }
            }
        }
    }

    if(DATA_TYPE_ARGB888 == dst_layer.layer_info.layer_data.type)
    {
#ifdef OSD_LAYER_BLEND
        ret = blend_argb888( lcd_id,dst_layer_ptr,&layer_array[0],layer_num,rect_ptr);
           //SCI_TRACE_LOW:"robert _SoftwareHandle  OSD_LAYER_BLEND"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1483_112_2_18_1_44_55_16,(uint8*)"");
#else
        //SCI_TRACE_LOW:"robert _SoftwareHandle not OSD_LAYER_BLEND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1485_112_2_18_1_44_55_17,(uint8*)"");
        ret = ERR_DISPLAY_FUNC_NOT_SUPPORT;
#endif
    }
    else if(DATA_TYPE_PMARGB888 == dst_layer.layer_info.layer_data.type)//dst is DATA_TYPE_PMARGB888
    {
        ret = _SoftInvalidateRectUsingLayer_Ext(lcd_id,&dst_layer,&layer_array[0],rect_ptr,layer_num); 
    }
    else
    {
    ret = _SoftInvalidateRectUsingLayer(lcd_id,&dst_layer,&layer_array[0],rect_ptr,layer_num);  
    }
    return ret;    
}
/*****************************************************************************/
//  Description:  multi layer blend interface for the APP
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_DISPLAY_E _HardwareHandle(  LCD_ID_E lcd_id,
                                        DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                        DISPLAY_BLEND_LAYER_T *layer_arr[],
                                        uint32 array_size,
                                        DISPLAY_RECT_T *rect_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
#if !SOFTWARE_BLEND_LAYER   
    uint32 layer_num = 0;
    uint32 i = 0,k = 0;
    LAYER_CONTEXT_T layer_array[LAYER_SUPPORT_NUM+1]={0};
    uint32 err_num = 0;
    DISPLAY_BLEND_LAYER_T save_layer_arr[LAYER_SUPPORT_NUM]={0};   
    DISPLAY_BLEND_LAYER_T dst_layer = {0};
    BLOCK_T blk_info[HW_BLOCK_NUM] = {0};  
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;  
    DISPLAY_OPERATION_MODE_E op_mode = UI_OPERATION;
    DISPLAY_LAYER_INFO_T *layer_info = NULL;
    DISPLAY_RECT_T *clip_rect = NULL;
    
    if(LAYER_SUPPORT_NUM<array_size)
    {
        //LAYER_HANDLE_TRACE:"--DISPLAY_BlendLayer :--array_size=%d--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1526_112_2_18_1_44_55_19,(uint8*)"d",array_size);
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    
    s = _get_ctrl_info();
    
    if(dst_layer_ptr != 0)
    {        
        dst_layer = *dst_layer_ptr;
        layer_info = &dst_layer.layer_info;
        clip_rect  = &layer_info->clip_rect;
        if (_check_clip_rect(clip_rect,layer_info->width, layer_info->height)
                != ERR_DISPLAY_NONE)
        {
             return ERR_DISPLAY_PARAMETER_WRONG;
        }
    }
   
    ret = _check_layer_parameter(layer_arr,array_size,SCI_FALSE);
    if(ERR_DISPLAY_NONE != ret)
    {       
        return ret;
    }

    for( i=0 ; i<array_size ; i++)
    {
        save_layer_arr[i] = *layer_arr[i];
    }

    for( i=0 ; i<array_size ; i++)
    {
        layer_array[i].layer_cfg_ptr = &save_layer_arr[i];
    }

    for( i=0 ; i<array_size ; i++)
    {
         if(layer_array[i].layer_cfg_ptr != PNULL)
         {
            if(layer_array[i].layer_cfg_ptr->layer_info.is_enable)
            {
                layer_info = &layer_array[i].layer_cfg_ptr->layer_info;
                clip_rect = &layer_info->clip_rect;
                if (_check_clip_rect(clip_rect,layer_info->width, layer_info->height)
                        != ERR_DISPLAY_NONE)
                {
                    continue;
                }
                layer_num++;
                layer_array[k++].layer_cfg_ptr = layer_array[i].layer_cfg_ptr;
            }
         }
    }
    if(dst_layer_ptr!=NULL)
    {
        layer_array[LAYER_DST_INDEX].layer_cfg_ptr = &dst_layer;
    }

    //compute the layer's display rectangle
    for( i=0 ; i<layer_num ; i++)
    {
        if(ERR_DISPLAY_NONE != _get_layer_display_rect(&layer_array[i],rect_ptr))
        {          
            layer_array[i].layer_cfg_ptr = PNULL;
            err_num++;
        }
    }
    //SCI_TRACE_LOW:"_HardwareHandle,err_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1578_112_2_18_1_44_55_20,(uint8*)"d",err_num);
    if(err_num != 0)
    {
        if(err_num >= layer_num)
        {
            return ERR_DISPLAY_PARAMETER_WRONG;
        }
        else
        {
            k=0;
            layer_num -= err_num;
            for( i=0 ; i<LAYER_SUPPORT_NUM ; i++)
            {
                if(layer_array[i].layer_cfg_ptr!=0)
                {
                    layer_array[k] = layer_array[i];
                    k++;
                }
            }
        }
    }

    if(ERR_DISPLAY_NONE != _get_layer_display_rect(&layer_array[LAYER_DST_INDEX],rect_ptr))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    } 
    if(dst_layer_ptr != PNULL)
    {
        _adjust_layer_config(&layer_array[LAYER_DST_INDEX],1);
    }
   
    for(i=0;i<layer_num;i++)
    {
        _adjust_layer_config(&layer_array[i],1);
    }
    
    LCD_GetLcdCtrlSemaphore(lcd_id);
    LCD_SetIsUseCtrlSm(FALSE);
    op_mode = LCD_GetOpMode(lcd_id);
    if(UI_OPERATION != op_mode)
    {
        LCD_GetBlockInfo(lcd_id,&blk_info[0]);
    }
    //LAYER_HANDLE_TRACE:"_HW_InvalidateRectUsingLayer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1621_112_2_18_1_44_55_21,(uint8*)"");
    ret = _HW_InvalidateRectUsingLayer(lcd_id,&dst_layer,&layer_array[0],rect_ptr,layer_num); 
    if(UI_OPERATION != op_mode)
    {
        for(i=0;i<HW_BLOCK_NUM;i++)
        {
            if(blk_info[i].is_enable)
            {
                blk_info[i].cfg.set_endian_flag = 1;
                LCD_ConfigBlock(lcd_id, i, blk_info[i].cfg);
                LCD_SetBlockBuffer(lcd_id, i,(uint32*)blk_info[i].mem_ptr);
                LCD_EnableBlock(lcd_id, i);
            }
        }
      }
    LCD_SetIsUseCtrlSm(TRUE);
    LCD_PutLcdCtrlSemaphore(lcd_id);
#endif    
       
    return ret;        
}

#ifdef OSD_LAYER_BLEND
LOCAL ERR_DISPLAY_E blend_argb888(  LCD_ID_E lcd_id,
                                    DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                    LAYER_CONTEXT_T       *layer_arr,
                                    uint32 layer_num,
                                    DISPLAY_RECT_T *rect_ptr)
{

        IMG_LAYER_T dst_layer ={0};
        IMG_LAYER_T *layer_ptr = PNULL;
        DISPLAY_BLEND_LAYER_T *src_ptr = PNULL;

        ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
        
        uint32 i;

        layer_ptr = SCI_ALLOC_APP(sizeof(IMG_LAYER_T)*layer_num);

        if(NULL == layer_ptr)
        {
            return ERR_DISPLAY_OPERATE_FAIL;
        }
              
        /*lint -e{613}*/
        {
            dst_layer.alpha = dst_layer_ptr->layer_info.layer_data.alpha;
            dst_layer.alpha_sel = dst_layer_ptr->layer_info.layer_data.alpha_sel;
            dst_layer.buf_addr = dst_layer_ptr->layer_info.buf_addr;
            dst_layer.colorkey = dst_layer_ptr->layer_info.layer_data.colorkey;

            dst_layer.colorkey_en= dst_layer_ptr->layer_info.layer_data.colorkey_en;
            dst_layer.height= dst_layer_ptr->layer_info.height;
            dst_layer.pos = dst_layer_ptr->layer_info.pos;
            dst_layer.type = dst_layer_ptr->layer_info.layer_data.type;
            dst_layer.width = dst_layer_ptr->layer_info.width;
        }

        for(i=0; i<layer_num; i++)
        {
            src_ptr = layer_arr[i].layer_cfg_ptr;
            layer_ptr[i].alpha = src_ptr->layer_info.layer_data.alpha;
            layer_ptr[i].alpha_sel = src_ptr->layer_info.layer_data.alpha_sel;
            layer_ptr[i].buf_addr = src_ptr->layer_info.buf_addr;
            layer_ptr[i].colorkey = src_ptr->layer_info.layer_data.colorkey;

            layer_ptr[i].colorkey_en= src_ptr->layer_info.layer_data.colorkey_en;
            layer_ptr[i].height= src_ptr->layer_info.height;
            layer_ptr[i].pos = src_ptr->layer_info.pos;
            layer_ptr[i].type = src_ptr->layer_info.layer_data.type;
            layer_ptr[i].width = src_ptr->layer_info.width;
            layer_ptr[i].clip_rect  = src_ptr->layer_info.clip_rect;
        }

            OSD_Blend(&dst_layer,layer_ptr , layer_num,  rect_ptr);

        SCI_FREE(layer_ptr);

        return ret;
            

}

    
#endif



/*****************************************************************************/
//  Description:  multi layer blend interface for the APP
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/

// LOCAL uint32 *argb88_buf = 0;
// LOCAL uint32 lcd_convert_flag = 1;

PUBLIC ERR_DISPLAY_E DISPLAY_BlendLayer(  LCD_ID_E lcd_id,
                                                 DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                                 DISPLAY_BLEND_LAYER_T *layer_arr[],
                                                 uint32 layer_num,
                                                 DISPLAY_RECT_T *rect_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint32 i = 0;    
    BOOLEAN handle_flag = SCI_TRUE;
    uint16 rect_w = rect_ptr->right-rect_ptr->left+1;
    uint16 rect_h = rect_ptr->bottom-rect_ptr->top+1;
    int16 left = rect_ptr->left;
    int16 top = rect_ptr->top;
    int16 right = rect_ptr->right;
    int16 bottom = rect_ptr->bottom; 
//    DISPLAY_BLEND_LAYER_T *layer0_ptr = PNULL;

      if((left<0)||(top<0)||(right<=0)||(bottom<=0)\
        || (right<=left)||(bottom<=top)||(0==dst_layer_ptr))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    {        
        if(rect_w>dst_layer_ptr->layer_info.width)
        {
            rect_ptr->right = dst_layer_ptr->layer_info.width+rect_ptr->left-1;
        }
        if(rect_h>dst_layer_ptr->layer_info.height)
        {
            rect_ptr->bottom = dst_layer_ptr->layer_info.height+rect_ptr->top-1;
        }
    }

#if SOFTWARE_BLEND_LAYER
    ret = _SoftwareHandle(lcd_id,dst_layer_ptr,layer_arr,layer_num,rect_ptr);
#else
    handle_flag = SCI_FALSE;

    if((rect_ptr->left & 0x1) || ((((rect_ptr->right - rect_ptr->left + 1) & 0x3) | 0x2) == 3))
    {
        handle_flag = SCI_TRUE;
    }
    else
    {
        for( i=0 ; i<layer_num ; i++)
        {
            if((layer_arr[i]->layer_info.layer_data.type != DATA_TYPE_ARGB888)
                &&(layer_arr[i]->layer_info.layer_data.type != DATA_TYPE_ARGB666)
                &&(layer_arr[i]->layer_info.layer_data.type != DATA_TYPE_PMARGB888))
            {
                if((layer_arr[i]->layer_info.pos.x & 0x1) || (layer_arr[i]->layer_info.width % 2 != 0))
                {
                   handle_flag = SCI_TRUE;
                   break;
                }   
            }
        }
        if(handle_flag == SCI_FALSE)
        {
            if((dst_layer_ptr->layer_info.pos.x & 0x1) 
				|| (dst_layer_ptr->layer_info.width % 2 != 0)
				|| (DATA_TYPE_RGB565 != dst_layer_ptr->layer_info.layer_data.type))
            {
                handle_flag = SCI_TRUE;
            }
        }
    }

    if(handle_flag == SCI_TRUE)
    {
        ret = _SoftwareHandle(lcd_id,dst_layer_ptr,layer_arr,layer_num,rect_ptr);
    }
    else
    {
        ret = _HardwareHandle(lcd_id,dst_layer_ptr,layer_arr,layer_num,rect_ptr);
    }
   
    //SCI_TRACE_LOW:"DISPLAY_BlendLayer: Out"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LAYER_BLEND_1800_112_2_18_1_44_55_22,(uint8*)"");
#endif

    return ret;
    
}
/*lint -restore*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

/* end of lcd.c*/



