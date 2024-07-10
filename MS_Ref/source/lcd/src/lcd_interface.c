/**-------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/ 
#include "ms_ref_lcd_trc.h"
#include "lcd_ctrl.h"
#include "lcd_task.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifdef WIN32
#define __align(x) 
//#include "display_cfg.c"


#ifdef DISPLAY_PIXELDEPTH_MAX_32         
#define DISPLAY_BYTE_NUM_IN_PIXEL  4
#else
#define DISPLAY_BYTE_NUM_IN_PIXEL  2
#endif 

#ifndef DISPLAY_LOW_MEMORY
#if defined MAIN_LCD_DISPLAY_LAYER_NUM_1
#define MAIN_LCD_DISPLAY_MEM_NUM        1
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
__align(32) LOCAL uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_display_mem)*/
#elif defined MAIN_LCD_DISPLAY_LAYER_NUM_2
#define MAIN_LCD_DISPLAY_MEM_NUM        2
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL*2)
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(552, s_display_mem)*/
#elif defined MAIN_LCD_DISPLAY_LAYER_NUM_3
#define MAIN_LCD_DISPLAY_MEM_NUM        3
#define MAIN_LCD_DISPLAY_MEM_SIZE    (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL*3)
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(552, s_display_mem)*/
#else
#ifdef PLATFORM_SC6600L
#define MAIN_LCD_DISPLAY_MEM_NUM        0
#define MAIN_LCD_DISPLAY_MEM_SIZE       2
#else
#define MAIN_LCD_DISPLAY_MEM_NUM        1
#define MAIN_LCD_DISPLAY_MEM_SIZE   (LCD_WIDTH*LCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
#endif//PLATFORM_SC6600L
LOCAL __align(32) uint8 s_display_mem[MAIN_LCD_DISPLAY_MEM_SIZE] = {0};
#endif


#if defined SUB_LCD_DISPLAY_LAYER_NUM_1
#define SUB_LCD_DISPLAY_MEM_NUM        1
#define SUB_LCD_DISPLAY_MEM_SIZE    (SUBLCD_WIDTH*SUBLCD_HEIGHT*DISPLAY_BYTE_NUM_IN_PIXEL)
__align(32) LOCAL uint8 s_sub_lcd_display_mem[SUB_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*/
#else
#define SUB_LCD_DISPLAY_MEM_NUM        0
#define SUB_LCD_DISPLAY_MEM_SIZE    2
__align(32) LOCAL uint8 s_sub_lcd_display_mem[SUB_LCD_DISPLAY_MEM_SIZE] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*/
    #endif

#else

    #define MAIN_LCD_DISPLAY_MEM_NUM        0
    #define MAIN_LCD_DISPLAY_MEM_SIZE       0
    
    LOCAL __align(32) uint8 s_display_mem[2] = {0};

    #define SUB_LCD_DISPLAY_MEM_NUM        0
    #define SUB_LCD_DISPLAY_MEM_SIZE    0
    
    LOCAL __align(32) uint8 s_sub_lcd_display_mem[2] = {0};/*lint -esym(551, s_sub_lcd_display_mem)*/
#endif
/*****************************************************************************/
//  Description:  get display number of layer on specified lcd
//    Global resource dependence:
//  Author: Jianping.wang
//    Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetLayerNum(LCD_ID_E lcd_id)
{
    uint32 layer_num = 0;

    if(lcd_id == LCD_ID_0)
    {
        layer_num = MAIN_LCD_DISPLAY_MEM_NUM;
    }
    else
    {
        layer_num = SUB_LCD_DISPLAY_MEM_NUM;
    }
    
    return layer_num;
}
/*****************************************************************************/
//  Description:  get display memory  address of layer on specified lcd
//    Global resource dependence:
//  Author: Jianping.wang
//    Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetMemAddr(LCD_ID_E lcd_id)
{
    uint32 mem_addr = 0;   
    uint32 layer_num = 0;

    if(lcd_id == LCD_ID_0)
    {
        layer_num = MAIN_LCD_DISPLAY_MEM_NUM;
        if(layer_num == 0)/*lint !e774*/
        {
            mem_addr = 0;
        }
        else
        {
            mem_addr = (uint32)&s_display_mem[0];
        }
    }
    else
    {
        layer_num = SUB_LCD_DISPLAY_MEM_NUM;
        if(layer_num == 0)/*lint !e774*/
        {
            mem_addr = 0;
        }
        else
        {
            mem_addr = (uint32)&s_sub_lcd_display_mem[0];
        }
    }

    return mem_addr;
}
/*****************************************************************************/
//  Description:  get display memory size of layer on specified lcd
//    Global resource dependence:
//  Author: Jianping.wang
//    Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetMemSize(LCD_ID_E lcd_id,uint32 layer_id)
{
    uint32 mem_size = 0;

    if(lcd_id == LCD_ID_0)
    {
        mem_size = MAIN_LCD_DISPLAY_MEM_SIZE;
    }
    else
    {
        mem_size = SUB_LCD_DISPLAY_MEM_SIZE;
    }
    
    return mem_size;
}


#endif




#if 0//def WIN32      
/*****************************************************************************/
//  Description:  invalidate lcd for GUI
//    Global resource dependence:
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_BlendAndInvalidateRect(
                                                    LCD_ID_E    lcd_id, 
                                                    DISPLAY_BLENDLAYER_PARM_T *blend,
                                                    DISPLAY_INVALIDATE_PARM_T *invalidate
                                                    )
{   
    if(PNULL != blend) 
    {
        blend->dst_layer_ptr->layer_info.is_enable = 1;
        DISPLAY_BlendLayer(lcd_id, blend->dst_layer_ptr, blend->layer_arr,
                           blend->layer_num, blend->rect_ptr);
       
    }
    if (PNULL != invalidate)
    {
        DISPLAY_InvalidateRect(lcd_id,invalidate->layer_arr,
                           invalidate->layer_num, invalidate->param_ptr);
              
    }            
    return ERR_DISPLAY_NONE;
}

PUBLIC  ERR_LCD_E  LCDAPI_GetInfo (
                                    LCD_ID_E   lcd_id,           //id of lcd to operate
                                    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
                                   )
{
    ERR_LCD_E ret;
    ret = LCD_GetInfo(lcd_id,lcd_info_ptr);
    return ret;
}
#else

/**---------------------------------------------------------------------------*
 **                         local functions                                                                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: clip rect
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 DISPLAY_ClipRect(DISPLAY_RECT_T *dst, DISPLAY_RECT_T*src)
{
    if(dst->left > src->right || dst->right < src->left ||
        dst->top > src->bottom || dst->bottom < src->top)
            return 0;
    /* They must overlap */
    if(dst->left < src->left)
            dst->left = src->left;
    if(dst->top < src->top)
            dst->top = src->top;
    if(dst->right > src->right)
            dst->right = src->right;
    if(dst->bottom > src->bottom)
            dst->bottom = src->bottom;
    return 1;
}

/*****************************************************************************/
//  Description: copy src layer memory and info to the dst layer
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 DISPLAY_CopyToAuxMemory(
                            DISPLAY_LAYER_INFO_T  *dst,
                            DISPLAY_BLENDLAYER_INFO_T *info,
                            DISPLAY_RECT_T        *rect,
                            DISPLAY_LAYER_INFO_T  *src
                            )
{
    uint32 i,size,width,height;
    uint32 offset_src; 
    uint32 start_x,start_y; 
    uint32 pitch_src,pitch_dst;
    DISPLAY_RECT_T dst_rect;
    uint32 ret = LCD_SUCCESS;
    
    dst_rect.left = src->pos.x;
    dst_rect.top  = src->pos.y;
    dst_rect.right  = src->pos.x + src->width - 1;
    dst_rect.bottom = src->pos.y + src->height - 1;
    if (PNULL != info->rect_ptr)
    {
        DISPLAY_RECT_T *blend_rect = info->rect_ptr ;    
        if (DISPLAY_ClipRect(&dst_rect,blend_rect) == 0)
        { 
            return ret;
        }
    } 
    if (DISPLAY_ClipRect(&dst_rect,rect) == 0)
    { 
        return ret;
    }        
    *dst = *src;   // copy the entire property to dst first
    
    width  = dst_rect.right  - dst_rect.left + 1;
    if (width > info->lcd_width) 
    {
        width = info->lcd_width;    
    }
    height = dst_rect.bottom - dst_rect.top + 1;
    if (height > info->lcd_height)
    {
        height = info->lcd_height;
    }
    start_x = dst_rect.left - src->pos.x;    // src
    start_y = dst_rect.top - src->pos.y; 
    pitch_src = src->width;    
    offset_src = start_y * pitch_src + start_x;

    switch(src->layer_data.type)
    {
    case DATA_TYPE_ARGB888:
    case DATA_TYPE_ARGB666:
        {
            uint32 * dst_base = (uint32 *)info->aux_mem_addr;
            uint32 * src_base = (uint32 *)src->buf_addr  + offset_src;
            
            pitch_dst = width; 
            if (pitch_src == pitch_dst)
            { 
                size = width * height;
                SCI_MEMCPY(dst_base,(void*)src_base,size * 4); /*lint !e516 -e516*/
            }
            else
            {  
               for ( i = 0; i < height ; i++)
               {
                   SCI_MEMCPY(dst_base + i * pitch_dst,
                        (void*)(src_base + i * pitch_src),width * 4);
               }    
            }
            dst->buf_addr = (uint32)info->aux_mem_addr;            
        } 

        break;
    case DATA_TYPE_ARGB565:
        {
            uint16 *dst_base = (uint16 *)info->aux_mem_addr;
            uint16 *src_base = (uint16 *)src->buf_addr + offset_src;           
            uint8  *alpha_start = (uint8*)((uint16 *)info->aux_mem_addr + info->lcd_width * info->lcd_height);
            pitch_dst = ((width + 1) >> 1)<<1;   // align
            if (pitch_src == pitch_dst)
            { 
                size = pitch_dst * height;
                SCI_MEMCPY(dst_base,(void*)src_base,size * 2);
                dst->buf_addr = (uint32)info->aux_mem_addr;    
                if (PNULL != src->alpha_buf_addr)
                {
                    SCI_MEMCPY(alpha_start ,
                             (void*)((uint8 *)src->alpha_buf_addr + offset_src),size);
                    dst->alpha_buf_addr = (uint32)alpha_start;
                }
            }
            else
            {          
                for (i =0; i < height ; i++)
                {
                   SCI_MEMCPY(dst_base + i* pitch_dst,(void*)(src_base + i* pitch_src) ,width * 2);
                }
                dst->buf_addr = (uint32)info->aux_mem_addr;
                if (PNULL != src->alpha_buf_addr)
                {
                   uint8  *alpha_src_base = (uint8 *)src->alpha_buf_addr + offset_src;
                    for (i =0; i < height ; i++)
                    {
                       SCI_MEMCPY(alpha_start + i * pitch_dst,
                                    (void*)(alpha_src_base + i * pitch_src),width );   
                    }
                    dst->alpha_buf_addr = (uint32)alpha_start;
                }
            }
        } 
        break;

    case DATA_TYPE_RGB565:
        {
            uint16 *dst_base = (uint16 *)info->aux_mem_addr;
            uint16 *src_base = (uint16 *)src->buf_addr + offset_src;               
            pitch_dst = ((width + 1) >> 1)<<1;   // align
            if (pitch_src == pitch_dst)
            { 
                size = pitch_dst * height;
                SCI_MEMCPY(dst_base,(void*)src_base,size * 2);
            }
            else
            {
                for ( i =0; i < height ; i++)
                {
                    SCI_MEMCPY(dst_base + i * pitch_dst,(void*)(src_base + i * pitch_src),width * 2);
                }    
            }
            dst->buf_addr = (uint32)info->aux_mem_addr;
        } 
        break;
 
    default:
       pitch_dst = 0;
       ret = LCD_INVALID_ARGUMENT;
       //DISPLAY_TRACE:"DISPLAY_CopyToAuxMemory: invalid data type"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_INTERFACE_352_112_2_18_1_45_13_99,(uint8*)"");
       break;
    }
    if (LCD_SUCCESS == ret)
    {
        dst->pos.x = dst_rect.left;
        dst->pos.y = dst_rect.top;
        dst->width = pitch_dst;
        dst->height = height;   
    }

    return ret;
} 


LOCAL uint32 DISPLAY_CheckBlendParam(
                                    LCD_ID_E    lcd_id, 
                                    DISPLAY_TASK_CONTEXT_T *context,
                                    DISPLAY_BLENDLAYER_PARM_T *blend,
                                    LCD_INFO_T *lcd_info
                                    )
{
    uint32 layer_id = 0;
    uint32 ret = LCD_SUCCESS;    
    DISPLAY_BLEND_LAYER_T *layer = PNULL;
    DISPLAY_LAYER_INFO_T  *src_layer = PNULL;    

    for(layer_id = 0;layer_id < blend->layer_num; layer_id++)
    {
        layer = blend->layer_arr[layer_id];
        src_layer = &layer->layer_info;
        if (!src_layer->layer_data.immutable )
        {
            return LCD_INVALID_ARGUMENT;
        }
    }

    context->blend.layer_num = blend->layer_num;
    context->blend.rect = *blend->rect_ptr ; 
    context->blend.dst_layer = *blend->dst_layer_ptr;

    for(layer_id = 0;layer_id < blend->layer_num; layer_id++)
    {
        context->blend.layer_arr[layer_id] = *blend->layer_arr[layer_id];
    }

    return ret;    
}

/*****************************************************************************/
//  Description: when only invalidate, check param, and copy bufer to aux memory if need
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL uint32 DISPLAY_CheckInvalidateParam(
                                        LCD_ID_E    lcd_id, 
                                        DISPLAY_TASK_CONTEXT_T *context,
                                        DISPLAY_INVALIDATE_PARM_T *invalidate,
                                        LCD_INFO_T *lcd_info)
{
    uint32 ret = LCD_SUCCESS;
    uint32 layer_id = 0;
    DISPLAY_LAYER_INFO_T  *layer = PNULL;    

    if (DISPLAY_ANGLE_0 != invalidate->param_ptr->rot_angle
        && invalidate->layer_num > 1 )
    {
        return LCD_MEMORY_NOT_ENOUGH;    
    }
    
    for(layer_id = 0;layer_id < invalidate->layer_num; layer_id++)
    {
        layer = invalidate->layer_arr[layer_id];
        if (!layer->layer_data.immutable )
        {
            return LCD_INVALID_ARGUMENT;
        }
    }
    
    context->invalidate.layer_num = invalidate->layer_num;
    context->invalidate.param = *invalidate->param_ptr; 

    for(layer_id = 0;layer_id < invalidate->layer_num; layer_id++)
    {
        context->invalidate.layer_arr[layer_id] = *invalidate->layer_arr[layer_id];
    }
    return ret;
}

/*****************************************************************************/
//  Description: when both blend and invalidate, check param, and copy bufer to 
//             aux memory if need
//  Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL uint32 DISPLAY_CheckBlendInvalidateParam(
                                            LCD_ID_E    lcd_id,
                                            DISPLAY_TASK_CONTEXT_T *context,                                            
                                            DISPLAY_BLENDLAYER_PARM_T *blend,
                                            DISPLAY_INVALIDATE_PARM_T *invalidate,
                                            LCD_INFO_T *lcd_info
                                        )
{
    uint32 ret = LCD_SUCCESS;
    uint32 layer_id = 0;
   
    ret = DISPLAY_CheckBlendParam(lcd_id, context, blend, lcd_info);
    if ( LCD_SUCCESS == ret)    // invalidate
    {   
        context->invalidate.layer_num = invalidate->layer_num;
        context->invalidate.param = *invalidate->param_ptr;    
        //for(layer_id = 0;layer_id < invalidate->layer_num; layer_id++) // only num == 1
        //{
        context->invalidate.layer_arr[layer_id] = *invalidate->layer_arr[layer_id];
        //}
    }
    return ret;    
}

/**------------------------------------------------------------------------------------*
 **                         public functions                                                                           *
 **------------------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: bend or invalidate lcd for GUI
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_BlendAndInvalidateRect(
                                                    LCD_ID_E    lcd_id, 
                                                    DISPLAY_BLENDLAYER_PARM_T *blend,
                                                    DISPLAY_INVALIDATE_PARM_T *invalidate
                                                    )
{   
    LCD_INFO_T lcd_info;
    uint32 mode = LCD_UPDATE_MODE_SYNC;  
    uint32 result  = LCD_SUCCESS;
    DISPLAY_TASK_CONTEXT_T *context = LCD_GetTaskContext();
    LCD_GetInfo (lcd_id, &lcd_info);

    /*if ( lcd_id != MAIN_LCD_ID
        && PNULL != invalidate )
    {
        return DISPLAY_InvalidateRect( lcd_id, invalidate->layer_arr, invalidate->layer_num, invalidate->param_ptr );
    }*/

    LCD_GetAPISemaphore();
    if (LCD_UPDATE_MODE_ASYNC == context->mode) 
    {
        LCD_WaitAndClearDoneStatus(); // if last time the do async,must wait,
    }
    //if (lcd_id == MAIN_LCD_ID) 
    {
        context->lcd_id = lcd_id;
        if (PNULL != blend && PNULL == invalidate)
        {            
            result = DISPLAY_CheckBlendParam(lcd_id, context, blend, &lcd_info);
            if(result == LCD_SUCCESS)
            {
                 mode = LCD_UPDATE_MODE_ASYNC ;                
            }
            else 
            {
               //DISPLAY_TRACE:"lcd_task blend: LCD_UPDATE_MODE_SYNC "
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_INTERFACE_503_112_2_18_1_45_13_100,(uint8*)"");
            }    
        }        
        else if (PNULL == blend && PNULL != invalidate)
        {
            result = DISPLAY_CheckInvalidateParam(lcd_id, context, invalidate, &lcd_info);
            if(result == LCD_SUCCESS)
            {
                 mode = LCD_UPDATE_MODE_ASYNC ;                
            }
            else 
            {
               //DISPLAY_TRACE:"lcd_task invalidate: LCD_UPDATE_MODE_SYNC "
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_INTERFACE_515_112_2_18_1_45_13_101,(uint8*)"");
            }
        }
        else if (PNULL != blend && PNULL != invalidate)
        {            
            if(!LCD_IsOSDMode(lcd_id))
            {
                if ( 1 == invalidate->layer_num && 
                       blend->dst_layer_ptr->layer_info.buf_addr == invalidate->layer_arr[0]->buf_addr )
                {
                    result = DISPLAY_CheckBlendInvalidateParam(lcd_id, context, blend, invalidate, &lcd_info);
                    if(result == LCD_SUCCESS)
                    {
                         mode = LCD_UPDATE_MODE_ASYNC;                
                    }
                    else 
                    {
                       //DISPLAY_TRACE:"lcd_task blend: LCD_UPDATE_MODE_SYNC "
                       SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_INTERFACE_532_112_2_18_1_45_13_102,(uint8*)"");
                    }
                }    
            }
        }  
    }
    context->mode = mode;
    if (mode == LCD_UPDATE_MODE_SYNC) 
    {
        context->p_blend = blend;
        context->p_invalidate = invalidate;
    }
    
    if ( PNULL != blend )
    {
        context->blend.callback = blend->callback;
        context->blend.param = blend->param;
    }

    LCD_PutTaskSemaphore(lcd_id);     //  notice the lcd task to run
    if (mode == LCD_UPDATE_MODE_SYNC) 
    {
        LCD_WaitAndClearDoneStatus(); 
    }
    
    LCD_PutAPISemaphore();
    
    return ERR_DISPLAY_NONE;
}

/*****************************************************************************/
//  Description: get the LCD info
//  Global resource dependence:
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC  ERR_LCD_E  LCDAPI_GetInfo (
                                    LCD_ID_E   lcd_id,          //id of lcd to operate
                                    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
                                   )
{
    ERR_LCD_E ret;
    LCD_GetAPISemaphore();
    LCD_WaitBlendDoneStatus();
    ret = LCD_GetInfo(lcd_id,lcd_info_ptr);
    LCD_PutAPISemaphore();    
    return ret;
}


#endif//WIN32

#ifdef   __cplusplus
    }
#endif
