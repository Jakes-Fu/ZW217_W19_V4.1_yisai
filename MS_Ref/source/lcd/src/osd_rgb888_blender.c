#include "ms_ref_lcd_trc.h"
#include "lcd_osd_rgb888_blender.h"


/*lint -save -e737 */

//#define ARGB888(r,g,b)  (0xff|(r<<16)|(g<<8)|(b));
#define RGB565TOARGB888(rgb565) ( ((rgb565 <<3)&0xff)| ((rgb565 <<5)&0xfc00)|((rgb565 <<8)&0xf80000)|0xff000000);


LOCAL uint32 _blendrgb888_rgb888_line(uint32 *dst_ptr, uint32 * src_ptr, uint32 count);
LOCAL uint32 _blendrgb888_565_line(uint32 *dst_ptr, uint16 *src_ptr, uint16 colorkey, uint32 count);

const uint16 div_tab[] =
{
    #include "div_table.c"
};


//common crop rectangle
//
LOCAL uint32 _blend_common_rect(DISPLAY_RECT_T * img_rect_ptr, DISPLAY_RECT_T * crop_rect_ptr)
{

    if(img_rect_ptr->left >= crop_rect_ptr->right)
    {
        return SCI_ERROR;
    }

    if(img_rect_ptr->right <= crop_rect_ptr->left)
    {
        return SCI_ERROR;
    }

    if(img_rect_ptr->top >= crop_rect_ptr->bottom)
    {
        return SCI_ERROR;
    }

    if(img_rect_ptr->bottom <= crop_rect_ptr->top)
    {
        return SCI_ERROR;
    }

    
    if(img_rect_ptr->left > crop_rect_ptr->left)
    {
        crop_rect_ptr->left = img_rect_ptr->left;
    }
    
    if(img_rect_ptr->right < crop_rect_ptr->right)
    {
        crop_rect_ptr->right = img_rect_ptr->right;
    }

    if(img_rect_ptr->top > crop_rect_ptr->top)
    {
        crop_rect_ptr->top = img_rect_ptr->top;
    }

    if(img_rect_ptr->bottom < crop_rect_ptr->bottom)
    {
        crop_rect_ptr->bottom = img_rect_ptr->bottom;
    }
    
    return SCI_SUCCESS;
}

//mode 0: copy;
//mode 1: blend
LOCAL uint32 _blend_layer(IMG_LAYER_T * dst_layer_ptr, IMG_LAYER_T * layer_arr,DISPLAY_RECT_T * rect_ptr, uint32 mode)
{
    DISPLAY_RECT_T dst_rect = {0};
    DISPLAY_RECT_T layer_rect = {0};
    DISPLAY_RECT_T crop_rect = {0};

    uint32 i=0;
    uint32 j=0;

    uint32 crop_width = 0;
    uint32 crop_height = 0;

    //SCI_TRACE_LOW:"OSD_Blend, _blend_layer, enter: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_81_112_2_18_1_45_45_190,(uint8*)"d", SCI_GetTickCount());

    dst_rect.left = dst_layer_ptr->pos.x;
    dst_rect.top = dst_layer_ptr->pos.y;
    dst_rect.right = dst_layer_ptr->pos.x+dst_layer_ptr->width-1;
    dst_rect.bottom = dst_layer_ptr->pos.y+dst_layer_ptr->height-1;

    layer_rect.left   = layer_arr->pos.x + layer_arr->clip_rect.left;
    layer_rect.top    = layer_arr->pos.y + layer_arr->clip_rect.top;
    layer_rect.right  = layer_arr->pos.x + layer_arr->clip_rect.right 
                                         - layer_arr->clip_rect.left;
    layer_rect.bottom = layer_arr->pos.y + layer_arr->clip_rect.bottom 
                                         - layer_arr->clip_rect.top;

    crop_rect = *rect_ptr;

    //process crop rect

    if(SCI_SUCCESS != _blend_common_rect(&dst_rect, &crop_rect))
    {
        //SCI_TRACE_LOW:"OSD_Blend: Invalid rectangle happeps"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_99_112_2_18_1_45_45_191,(uint8*)"");
        return SCI_ERROR;
    }
    if(SCI_SUCCESS != _blend_common_rect(&layer_rect, &crop_rect))
    {
        //SCI_TRACE_LOW:"OSD_Blend: Invalid rectangle happeps"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_104_112_2_18_1_45_45_192,(uint8*)"");
        return SCI_ERROR;
    }

    crop_width = crop_rect.right - crop_rect.left +1;
    crop_height = crop_rect.bottom - crop_rect.top +1;


    if(DATA_TYPE_ARGB888 == layer_arr->type)
    {
        uint32 dst_width = dst_layer_ptr->width;
        uint32 src_width = layer_arr->width;
        
        uint32 *dst_ptr =(uint32 *)dst_layer_ptr->buf_addr+(crop_rect.top - dst_layer_ptr->pos.y)*dst_width+(crop_rect.left - dst_layer_ptr->pos.x);
        uint32 *src_ptr =(uint32 *)layer_arr->buf_addr+(crop_rect.top - layer_arr->pos.y)*src_width+(crop_rect.left - layer_arr->pos.x);


        //SCI_TRACE_LOW:"OSD_Blend, _blend_layer ARGB888 width: %d, height:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_121_112_2_18_1_45_45_193,(uint8*)"dd", crop_width, crop_height);
        for(i = 0; i<crop_height; i++ )
        {   
            if(0 == mode)
            {
                SCI_MEMCPY(dst_ptr, src_ptr, crop_width *4);
            }
            else
            {
                _blendrgb888_rgb888_line(dst_ptr,src_ptr, crop_width);
            }
            dst_ptr = dst_ptr + dst_width;
            src_ptr = src_ptr + src_width;
        }
    }

    if(DATA_TYPE_RGB565== layer_arr->type)
    {
        uint32 dst_width = dst_layer_ptr->width;
        uint32 src_width = layer_arr->width;
        
        uint32 *dst_ptr =(uint32 *)dst_layer_ptr->buf_addr+(crop_rect.top - dst_layer_ptr->pos.y)*dst_width+(crop_rect.left - dst_layer_ptr->pos.x);
        uint16 *src_ptr =(uint16 *)layer_arr->buf_addr+(crop_rect.top - layer_arr->pos.y)*src_width+(crop_rect.left - layer_arr->pos.x);

        uint16 rgb565_value = 0;
        uint16 colorkey = layer_arr->colorkey;

        //SCI_TRACE_LOW:"OSD_Blend, _blend_layer ARGB565 width: %d, height:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_148_112_2_18_1_45_45_194,(uint8*)"dd", crop_width, crop_height);
        //uint32 r, g, b;
        for(i = 0; i<crop_height; i++ )
        {
            if(0 == mode)
            {
                if(layer_arr->colorkey_en)
                {
                    for(j = 0; j<crop_width; j++ )
                    {
                        rgb565_value = *(src_ptr+j);
                        if(colorkey == rgb565_value)
                        {
                            *(dst_ptr+j) = 0x0;
                        }
                        else
                        {   
                            *(dst_ptr+j) = RGB565TOARGB888(rgb565_value)
                        }
                    }
                }
                else
                {
                    for(j = 0; j<crop_width; j++ )
                    {
                        rgb565_value = *(src_ptr+j);
                        *(dst_ptr+j) = RGB565TOARGB888(rgb565_value)

                    }
                    
                }
            }
            else
            {
                if(layer_arr->colorkey_en)
                {
                    _blendrgb888_565_line(dst_ptr, src_ptr, colorkey, crop_width);
                    
                }
                else
                {
                    for(j = 0; j<crop_width; j++ )
                    {
                        rgb565_value = *(src_ptr+j);
                        *(dst_ptr+j) = RGB565TOARGB888(rgb565_value)

                    }
                }
            }
            
            dst_ptr = dst_ptr + dst_width;
            src_ptr = src_ptr + src_width;
            
        }
    }   

    //SCI_TRACE_LOW:"OSD_Blend, _blend_layer, leave: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_204_112_2_18_1_45_45_195,(uint8*)"d", SCI_GetTickCount());

    return SCI_SUCCESS;

}

    
LOCAL uint32 _blend_frame(IMG_LAYER_T *dst_layer_ptr,
                                        IMG_LAYER_T *layer_arr,
                                        uint32 layer_num,
                                    DISPLAY_RECT_T *rect_ptr)
{
    uint32 i=0;
    //uint32 * dst_base_ptr = (uint32 *)dst_layer_ptr->buf_addr;
    IMG_LAYER_T *dst_ptr = PNULL;
    IMG_LAYER_T *src_ptr = PNULL;

    //DISPLAY_RECT_T common_rect = {0};
    

    dst_ptr = dst_layer_ptr;


    if(SCI_SUCCESS != _blend_layer(dst_layer_ptr, &layer_arr[0], rect_ptr, 0))  //only copy
    {
        //SCI_TRACE_LOW:"_blend_frame() error i = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_229_112_2_18_1_45_45_196,(uint8*)"");
        //SCI_TRACE_LOW:"_blend_frame() error x = %d, y = %d, width = %d, height = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_230_112_2_18_1_45_45_197,(uint8*)"dddd", layer_arr[0].pos.x, layer_arr[0].pos.y, layer_arr[0].width, layer_arr[0].height);
        //SCI_TRACE_LOW:"_blend_frame() error rect(%d,%d,%d,%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_231_112_2_18_1_45_45_198,(uint8*)"dddd", rect_ptr->left, rect_ptr->top, rect_ptr->right, rect_ptr->bottom);
//      return SCI_ERROR;
    }
    for(i=1; i< layer_num; i++)
    {
        dst_ptr = dst_layer_ptr;
        src_ptr = &layer_arr[i];    

        if(SCI_SUCCESS != _blend_layer(dst_ptr, src_ptr, rect_ptr, 1)) //blend
        {
        //SCI_TRACE_LOW:"_blend_frame() error i = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_241_112_2_18_1_45_45_199,(uint8*)"d", i);
        //SCI_TRACE_LOW:"_blend_frame() error x = %d, y = %d, width = %d, height = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_242_112_2_18_1_45_45_200,(uint8*)"dddd", src_ptr->pos.x, src_ptr->pos.y, src_ptr->width, src_ptr->height);
        //SCI_TRACE_LOW:"_blend_frame() error rect(%d,%d,%d,%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_243_112_2_18_1_45_45_201,(uint8*)"dddd", rect_ptr->left, rect_ptr->top, rect_ptr->right, rect_ptr->bottom);
//          return SCI_ERROR;
        }
        
    }
    return SCI_SUCCESS;
    
}

//top and bottom both argb888
LOCAL uint32 _blendrgb888_rgb888_line(uint32 *dst_ptr, uint32 * src_ptr, uint32 count)
{
    //uint32 *l_src_ptr = ( uint32 *)src_ptr;
    uint32 *l_dst_argb_ptr  =(uint32 *)dst_ptr;
    uint32 i=0;
    uint32 colortable_argb_value;
    uint32 l_dst_argb_value;
    uint32 layer1_point_value;
    uint32 layer1_point_alpha;
    uint32 layer2_point_value;
    uint32 layer2_point_alpha;
    uint32 compound_argb=0;
        
    for(i=0; i<count; i++)
    {
        colortable_argb_value=*src_ptr++; 
        layer2_point_alpha=(colortable_argb_value>>24)&0xff;

            if(0 == layer2_point_alpha)
        {
                    l_dst_argb_ptr++;
            continue;       
            }
        else if(255 == layer2_point_alpha)
        {
            *l_dst_argb_ptr++=colortable_argb_value;
            continue;                           
        }
            
        l_dst_argb_value= *l_dst_argb_ptr;
        layer1_point_alpha=(l_dst_argb_value>>24)&0xff;
        if(0 == layer1_point_alpha)
        {               
            *l_dst_argb_ptr++=colortable_argb_value;                
        }
        else if(255 == layer1_point_alpha)
        {
            compound_argb=0; 
               //blue
                layer1_point_value=l_dst_argb_value&0xff;
                layer2_point_value=colortable_argb_value&0xff;
        
            layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;
                
            compound_argb|=layer1_point_value&0xff;

                   //green
                layer1_point_value=(l_dst_argb_value>>8)&0xff;
                    layer2_point_value=(colortable_argb_value>>8)&0xff;
        
            layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;

            compound_argb|=(layer1_point_value&0xff)<<8;    
                
            //red
                layer1_point_value=(l_dst_argb_value>>16)&0xff;
                    layer2_point_value=(colortable_argb_value>>16)&0xff;
        
            layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;
                
            compound_argb|=(layer1_point_value&0xff)<<16;   
                
                //alpha
            compound_argb|=layer1_point_alpha<<24;  
                                    
            *l_dst_argb_ptr++=compound_argb;
        }
        else
        {               
            uint32 compound_alpha = 0;
            uint32 alpha_div_factor = 0;
            compound_argb=0; 
                
                //caculate alpha
            compound_alpha = (255-layer1_point_alpha) * (255 - layer2_point_alpha) ;
            compound_alpha = (compound_alpha+128) >> 8;
                //get normal alpha & value
            compound_alpha= 255 - compound_alpha;   

            compound_argb|=compound_alpha<<24;
            
            alpha_div_factor = div_tab[compound_alpha];//(1 << 16) / compound_alpha;     

            compound_alpha = compound_alpha >> 1;
                        
            layer1_point_alpha =     layer1_point_alpha * (255-layer2_point_alpha);
            
            //blue
                layer1_point_value=l_dst_argb_value&0xff;
                layer2_point_value=colortable_argb_value&0xff;
                      
            layer1_point_value = layer1_point_value * layer1_point_alpha  + ((layer2_point_value * layer2_point_alpha)<<8);
            layer1_point_value = (layer1_point_value+32768) >> 16;
            layer1_point_value = (((layer1_point_value<<8)+(compound_alpha)) * alpha_div_factor) >> 16;

            compound_argb|=layer1_point_value&0xff;
                
                //green
            layer1_point_value=(l_dst_argb_value>>8)&0xff;
                layer2_point_value=(colortable_argb_value>>8)&0xff;
                      
            layer1_point_value = layer1_point_value * layer1_point_alpha  + ((layer2_point_value * layer2_point_alpha)<<8);
            layer1_point_value = (layer1_point_value+32768) >> 16;
            layer1_point_value = (((layer1_point_value<<8)+(compound_alpha)) * alpha_div_factor) >> 16;
                
            compound_argb|=(layer1_point_value&0xff)<<8;    
                
                //red
            layer1_point_value=(l_dst_argb_value>>16)&0xff;
                layer2_point_value=(colortable_argb_value>>16)&0xff;
                
            layer1_point_value = layer1_point_value * layer1_point_alpha + ((layer2_point_value * layer2_point_alpha)<<8);
            layer1_point_value = (layer1_point_value+32768) >> 16;
            layer1_point_value = (((layer1_point_value<<8)+(compound_alpha)) * alpha_div_factor) >> 16;
                
            compound_argb|=(layer1_point_value&0xff)<<16;   

            *l_dst_argb_ptr++=compound_argb;

            
        }

    }

    return SCI_SUCCESS;
    
}

//top argb565
//bot argb888
//target argb888
LOCAL uint32 _blendrgb888_565_line(uint32 *dst_ptr, uint16 *src_ptr, uint16 colorkey, uint32 count)
{
    //uint32 *l_src_ptr = ( uint32 *)src_ptr;
    uint32 *l_dst_argb_ptr  =(uint32 *)dst_ptr;
    uint32 i=0;
    uint16 rgb565_value = 0;

    //uint32 r, g, b;
        
    for(i=0; i<count; i++)
    {
        rgb565_value=*src_ptr++; 
           if(colorkey == rgb565_value)
        {
            l_dst_argb_ptr++;
        }
        else
        {
            *l_dst_argb_ptr++= RGB565TOARGB888(rgb565_value)
        }
    
    }

    return SCI_SUCCESS;
    
}


LOCAL ERR_DISPLAY_E _check_param(IMG_LAYER_T *dst_layer_ptr,
                                        IMG_LAYER_T *layer_arr,
                                        uint32 layer_num,
                                    DISPLAY_RECT_T *rect_ptr)

{
    
    


    return ERR_DISPLAY_NONE;
        
}

//default layer_num == 2;
PUBLIC ERR_DISPLAY_E OSD_Blend(IMG_LAYER_T *dst_layer_ptr,
                                        IMG_LAYER_T *layer_arr,
                                        uint32 layer_num,
                                    DISPLAY_RECT_T *rect_ptr)
{

    //SCI_TRACE_LOW:"OSD_Blend, num:%d, enter: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_433_112_2_18_1_45_46_202,(uint8*)"dd", layer_num, SCI_GetTickCount());
    if(ERR_DISPLAY_NONE !=_check_param(dst_layer_ptr, layer_arr, layer_num, rect_ptr ))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }


    if(SCI_SUCCESS != _blend_frame(dst_layer_ptr, layer_arr,layer_num, rect_ptr))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    //SCI_TRACE_LOW:"OSD_Blend, leave: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,OSD_RGB888_BLENDER_445_112_2_18_1_45_46_203,(uint8*)"d", SCI_GetTickCount());

    return ERR_DISPLAY_NONE;

    
}

/*lint -restore */
