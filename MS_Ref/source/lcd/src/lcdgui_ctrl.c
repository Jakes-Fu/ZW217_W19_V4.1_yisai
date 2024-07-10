/******************************************************************************
 ** File Name:     lcdgui_ctrl.c                                              *
 ** Description:                                                              *
 **    This file contains display contrl on the lcd                           *
 ** Author:         Jianping.Wang                                             *
 ** DATE:          26/5/2010                                                  *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 26/5/2009      Jianping.Wang    Create.                                   *
 ******************************************************************************/

 /**--------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "ms_ref_lcd_trc.h"
#include "lcd_ctrl.h"
#include "lcdc_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
#define LCDC_ALGINED_1        0x00
#define LCDC_ALGINED_2        0x01
#define LCDC_ALGINED_4        0x03
#define LCDC_ALGINED_8        0x07
#define LCDC_ALGINED_16       0x0f

/**---------------------------------------------------------------------------*
 **                         public function define                            *
 **---------------------------------------------------------------------------*/
/******************************************************************************/
//  Description:  convert from logic coordinate to physical coordinate
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/******************************************************************************/
ERR_DISPLAY_E DISPLAY_ConvertCoordinate (
                                              LCD_ID_E  lcd_id, 
                                              DISPLAY_CONVERT_INPUT_T *input_ptr,
                                              DISPLAY_CONVERT_OUTPUT_T *out_ptr
                                            )
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL; 
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;    
    DISPLAY_ANGLE_E logic_angle = DISPLAY_ANGLE_0;
    uint16 logic_width = 0;
    uint16 logic_height = 0;
    int16 left = 0,top = 0,right=0,bottom=0;
   
    if((input_ptr==PNULL) || (out_ptr == PNULL))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    s = _get_ctrl_info();

    ret = _check_convert_param(lcd_id,input_ptr);
    if(ret != ERR_DISPLAY_NONE)
    {
        return ret;
    }    

    out_ptr->physical_coordinate.left = 0;              
    out_ptr->physical_coordinate.top = 0;                
    out_ptr->physical_coordinate.right = 0;
    out_ptr->physical_coordinate.bottom = 0;
    
    logic_angle = input_ptr->logic_angle;
    logic_width = input_ptr->logic_lcd_width;
    logic_height = input_ptr->logic_lcd_height;   
    s->logic_width[lcd_id] = logic_width;
    s->logic_height[lcd_id] = logic_height;
    
    left = input_ptr->logic_coordinate.left;
    top = input_ptr->logic_coordinate.top;
    right = input_ptr->logic_coordinate.right;
    bottom = input_ptr->logic_coordinate.bottom;
    
    if(s->lcd_direction[lcd_id] == LCD_PLACED_VERTICAL)
    {
        switch(logic_angle)
        {
            case DISPLAY_ANGLE_0:
            out_ptr->rotation_angle = DISPLAY_ANGLE_0;            
            out_ptr->physical_coordinate.left = left;
            out_ptr->physical_coordinate.top = top;
            out_ptr->physical_coordinate.right = right;
            out_ptr->physical_coordinate.bottom = bottom;
            break;
            case DISPLAY_ANGLE_90:
            out_ptr->rotation_angle = DISPLAY_ANGLE_90;
            if((right==0) &&(bottom==0))//pixel convert
            {          
                out_ptr->physical_coordinate.left = logic_height-1-top;
                out_ptr->physical_coordinate.top = left;
            }
            else
            {                
                out_ptr->physical_coordinate.left = logic_height-1-bottom;              
                out_ptr->physical_coordinate.top = left;                
                out_ptr->physical_coordinate.right = logic_height-1-top;
                out_ptr->physical_coordinate.bottom = right;
            }
            break;
            case DISPLAY_ANGLE_180:
            out_ptr->rotation_angle = DISPLAY_ANGLE_180;
            if((right==0) &&(bottom==0))//pixel convert
            {          
                out_ptr->physical_coordinate.left = logic_width-1-left;
                out_ptr->physical_coordinate.top = logic_height-1-top;
            }
            else
            {                
                out_ptr->physical_coordinate.left = logic_width-1-right;
                out_ptr->physical_coordinate.top = logic_height-1-bottom;
                out_ptr->physical_coordinate.right = logic_width-1-left;
                out_ptr->physical_coordinate.bottom = logic_height-1-top;
            }
            break;
            case DISPLAY_ANGLE_270:
            out_ptr->rotation_angle = DISPLAY_ANGLE_270;
             if((right==0) &&(bottom==0))//pixel convert
            {          
                out_ptr->physical_coordinate.left = top;
                out_ptr->physical_coordinate.top = logic_width-1-left;
            }
            else
            {                
                out_ptr->physical_coordinate.left = top;
                out_ptr->physical_coordinate.top = logic_width-1-right;
                out_ptr->physical_coordinate.right = bottom;
                out_ptr->physical_coordinate.bottom = logic_width-1-left;
            }
            break;
            default:
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            break;
        }
    }
    else
    {
        //////////////////////////////////////////////////////////////////////////
        // fixed by feng.xiao for NEWMS00173374
        // -fix: angle 0 and 180 are reversed when horizontal case
        switch(logic_angle)
        {
            case DISPLAY_ANGLE_0:
                out_ptr->rotation_angle = DISPLAY_ANGLE_270;
                if((right==0) &&(bottom==0))//pixel convert
                {          
                    out_ptr->physical_coordinate.left = top;
                    out_ptr->physical_coordinate.top = logic_width-1-left;
                }
                else
                {                
                    out_ptr->physical_coordinate.left = top;
                    out_ptr->physical_coordinate.top = logic_width-1-right;
                    out_ptr->physical_coordinate.right = bottom;
                    out_ptr->physical_coordinate.bottom = logic_width-1-left;
                }
//             out_ptr->rotation_angle = DISPLAY_ANGLE_90;
//             if((right==0) &&(bottom==0))//pixel convert
//             {          
//                 out_ptr->physical_coordinate.left = logic_height-1-top;
//                 out_ptr->physical_coordinate.top = left;
//             }
//             else
//             {                
//                 out_ptr->physical_coordinate.left = logic_height-1-bottom;
//                 out_ptr->physical_coordinate.top = left;
//                 out_ptr->physical_coordinate.right = logic_height-1-top;
//                 out_ptr->physical_coordinate.bottom = right;
//             }
            break;
            case DISPLAY_ANGLE_90:
            out_ptr->rotation_angle = DISPLAY_ANGLE_0;
            out_ptr->physical_coordinate.left = left;
            out_ptr->physical_coordinate.top = top;
            out_ptr->physical_coordinate.right = right;
            out_ptr->physical_coordinate.bottom = bottom;
            break;
            case DISPLAY_ANGLE_180:
                out_ptr->rotation_angle = DISPLAY_ANGLE_90;
                if((right==0) &&(bottom==0))//pixel convert
                {          
                    out_ptr->physical_coordinate.left = logic_height-1-top;
                    out_ptr->physical_coordinate.top = left;
                }
                else
                {                
                    out_ptr->physical_coordinate.left = logic_height-1-bottom;
                    out_ptr->physical_coordinate.top = left;
                    out_ptr->physical_coordinate.right = logic_height-1-top;
                    out_ptr->physical_coordinate.bottom = right;
                }
//             out_ptr->rotation_angle = DISPLAY_ANGLE_270;
//             if((right==0) &&(bottom==0))//pixel convert
//             {          
//                 out_ptr->physical_coordinate.left = top;
//                 out_ptr->physical_coordinate.top = logic_width-1-left;
//             }
//             else
//             {                
//                 out_ptr->physical_coordinate.left = top;
//                 out_ptr->physical_coordinate.top = logic_width-1-right;
//                 out_ptr->physical_coordinate.right = bottom;
//                 out_ptr->physical_coordinate.bottom = logic_width-1-left;
//             }
            break;
            case DISPLAY_ANGLE_270:
            out_ptr->rotation_angle = DISPLAY_ANGLE_180;
            if((right==0) &&(bottom==0))//pixel convert
            {          
                out_ptr->physical_coordinate.left = logic_width-1-left;
                out_ptr->physical_coordinate.top = logic_height-1-top;
            }
            else
            {                
                out_ptr->physical_coordinate.left = logic_width-1-right;
                out_ptr->physical_coordinate.top = logic_height-1-bottom;
                out_ptr->physical_coordinate.right = logic_width-1-left;
                out_ptr->physical_coordinate.bottom = logic_height-1-top;
            }
            break;
            default:
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            break;
        }
    }
#if 0
    //SCI_TRACE_LOW:"DISPLAY_ConvertCoordinate:out:angle=%d,left=%d,top=%d,right=%d,bottom=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDGUI_CTRL_206_112_2_18_1_45_43_182,(uint8*)"ddddd", out_ptr->rotation_angle,out_ptr->physical_coordinate.left,out_ptr->physical_coordinate.top, out_ptr->physical_coordinate.right,out_ptr->physical_coordinate.bottom);
#endif
   return ret;                             
    
}

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
#ifdef WIN32      
/*****************************************************************************/
//  Description:  invalidate lcd for GUI
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_InvalidateRect(
                                                    LCD_ID_E    lcd_id, 
                                                    DISPLAY_LAYER_INFO_T *layer_arr[],
                                                    uint32 layer_num,
                                                    DISPLAY_INVALIDATE_POS_T *param_ptr
                                                    )
{   
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL; 
    uint32 i = 0,k = 0;  
    DISPLAY_LAYER_INFO_T display_layer[DISPLAY_UPDATE_LAYER_NUM]= {0};        
    DISPLAY_LAYER_INFO_T *layer_tmp_arr[DISPLAY_UPDATE_LAYER_NUM] = {0};
    LCD_INFO_T lcd_info = {0};
    DISPLAY_RECT_T *clip_rect = NULL;

    s = _get_ctrl_info();
    
    LCD_GetInfo(lcd_id,&lcd_info);
    for( i=0 ; i<layer_num ; i++)
    {
        if(layer_arr[i]->is_enable)
        {
            if(k > DISPLAY_UPDATE_LAYER_NUM)
            {
                return ERR_DISPLAY_FUNC_NOT_SUPPORT;
            }
            display_layer[k] = *layer_arr[i];
            layer_tmp_arr[k] = &display_layer[i];

            clip_rect = &display_layer[k].clip_rect;
            if (_check_clip_rect(clip_rect, display_layer[k].width,
                        display_layer[k].height) != ERR_DISPLAY_NONE)
            {
                continue;  // invalidate Rect Clip
            }

            if((param_ptr->rot_angle == DISPLAY_ANGLE_90)||(param_ptr->rot_angle == DISPLAY_ANGLE_270))
            {         
                if(display_layer[k].height > lcd_info.lcd_height)
                {
                    display_layer[k].height = lcd_info.lcd_height;
                }
                else
                {
                    if(display_layer[k].width > lcd_info.lcd_width)
                    {
                        display_layer[k].width = lcd_info.lcd_width;
                    }
                }
            }
            k++; 
        }               
    }  

    layer_num = k;    
    if(lcd_id>(SUPPORT_LCD_NUM-1))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }        
        
    if(param_ptr->rot_angle != LCD_ANGLE_0) 
    {  
        for( i=0 ; i<layer_num ; i++)
        {
            if(s->menu_buffer_info[lcd_id][i].addr==0)
            {
                s->menu_buffer_info[lcd_id][i].size = s->logic_width[lcd_id]*s->logic_height[lcd_id]*4;
                s->menu_buffer_info[lcd_id][i].addr = (uint32)SCI_ALLOCA(s->menu_buffer_info[lcd_id][i].size);                                
                s->menu_buffer_info[lcd_id][i].width  = lcd_info.lcd_width;
                s->menu_buffer_info[lcd_id][i].height = lcd_info.lcd_height;
            }
        }
    }
    
    if(param_ptr->rot_angle != DISPLAY_ANGLE_0)
    {
        for(i=0 ; i<layer_num ; i++)
        {
            if(s->menu_buffer_info[lcd_id][i].addr==NULL)
            {
                return ERR_DISPLAY_FUNC_NOT_SUPPORT;
            }        
        }
    }
    LCD_GetLcdCtrlSemaphore(lcd_id);    
    _invalidate_on_simulator(lcd_id,&layer_tmp_arr[0],layer_num,param_ptr);
    LCD_PutLcdCtrlSemaphore(lcd_id);

    return ERR_DISPLAY_NONE;
}
#else
/*****************************************************************************/
//  Description:  invalidate lcd for GUI
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_InvalidateRect(
                                                    LCD_ID_E    lcd_id,
                                                    DISPLAY_LAYER_INFO_T *layer_arr[],
                                                    uint32 layer_num,
                                                    DISPLAY_INVALIDATE_POS_T *param_ptr
                                                    )
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;
    uint32 i = 0,k = 0;
    LCD_INFO_T lcd_info = {0};
    DISPLAY_LAYER_INFO_T display_layer[DISPLAY_UPDATE_LAYER_NUM] = {0};
    DISPLAY_LAYER_INFO_T *layer_tmp_arr[DISPLAY_UPDATE_LAYER_NUM]= {0};
    DISPLAY_RECT_T *clip_rect = NULL;

    //DISPLAY_TRACE:"DISPLAY_InvalidateRect:rect:%d,%d,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDGUI_CTRL_315_112_2_18_1_45_43_183,(uint8*)"dddd",param_ptr->rect.left,param_ptr->rect.top, param_ptr->rect.right,param_ptr->rect.bottom);

    if(lcd_id>(SUPPORT_LCD_NUM-1))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    s = _get_ctrl_info();

    LCD_GetInfo(lcd_id, &lcd_info);

    for( i=0 ; i<layer_num ; i++)
    {
        if(layer_arr[i]->is_enable)
        {
            if(k >= DISPLAY_UPDATE_LAYER_NUM)
            {
                return ERR_DISPLAY_FUNC_NOT_SUPPORT;
            }
            display_layer[k] = *layer_arr[i];
            layer_tmp_arr[k] = &display_layer[k];

            clip_rect = &display_layer[k].clip_rect;
            if (_check_clip_rect(clip_rect, display_layer[k].width,
                        display_layer[k].height) != ERR_DISPLAY_NONE)
            {
                continue;  // invalidate Rect Clip
            }
            if((param_ptr->rot_angle == DISPLAY_ANGLE_90)||(param_ptr->rot_angle == DISPLAY_ANGLE_270))
            {
                if(display_layer[k].height > lcd_info.lcd_height)
                {
                    display_layer[k].height = lcd_info.lcd_height;
                }
                else
                {
                    if(display_layer[k].width > lcd_info.lcd_width)
                    {
                        display_layer[k].width = lcd_info.lcd_width;
                    }
                }
            }
            k++;
        }
    }
    layer_num = k;

    _invalidate_on_lcd(lcd_id,&layer_tmp_arr[0],layer_num,param_ptr);

    return ERR_DISPLAY_NONE;
}

#endif//WIN32
/*****************************************************************************/
//  Description:  disable display layer for invalidate LCD function
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_DisableLayer(LCD_ID_E lcd_id, uint32 layer_id)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint32 layer_id_max = 0;

    layer_id_max = (uint32)(LCDC_LAYER_NUM -1);
    
    if(layer_id > layer_id_max)
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    if(ERR_LCD_NONE != LCD_DisableBlock(lcd_id, layer_id))
    {
        ret = ERR_DISPLAY_OPERATE_FAIL;
    }

    return ret;        
}
/*****************************************************************************/
//  Description:  enter or exit sleep
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_EnterSleep(LCD_ID_E lcd_id, BOOLEAN is_sleep)
{
    ERR_LCD_E ret = ERR_LCD_NONE;
    
    ret = LCD_EnterSleep(lcd_id, is_sleep);

    return (ERR_DISPLAY_E)ret;
}
/*****************************************************************************/
//  Description: if permit to update UI to LCD
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:is_permit - TRUE,permit to update UI to LCD
//                   FALSE,don't permit to update UI to LCD
/*****************************************************************************/
ERR_DISPLAY_E DISPLAY_IsPermit(LCD_ID_E lcd_id,BOOLEAN is_permit)
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;    
    
    if(lcd_id>(SUPPORT_LCD_NUM-1))
    {
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    
    s = _get_ctrl_info();

    
    LCD_GetLcdCtrlSemaphore(lcd_id);
    if(is_permit)
    {
        s->limit_type [lcd_id] = PERMIT_GUI_INVALIDATE;
    }
    else
    {
        s->limit_type [lcd_id] = NO_PERMIT_GUI_INVALIDATE;
    }

    LCD_PutLcdCtrlSemaphore(lcd_id);

    return ERR_DISPLAY_NONE;
    
}

/****************************************************************************************/
// Description: get lcdc algin parameter
// Global resource dependence:
// Author: Eddy.wei
/*****************************************************************************************/
PUBLIC BOOLEAN LCDC_GetAlginParam (DISPLAY_DATA_TYPE_E data_format, 
                                   uint32 *width_algin, 
                                   uint32 *height_algin)
{
    uint32 l_width_algin=0x00;
    uint32 l_height_algin=0x00;

    switch (data_format)
    {
        case DATA_TYPE_YUV422:
        case DATA_TYPE_YUV400:
        case DATA_TYPE_GREY:            
             l_width_algin=LCDC_ALGINED_4;
             l_height_algin=LCDC_ALGINED_1;
             break;      
             
        case DATA_TYPE_YUV420:            
             l_width_algin=LCDC_ALGINED_4;
             l_height_algin=LCDC_ALGINED_2;
             break;   
             
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_ARGB666:  
        case DATA_TYPE_PMARGB888:  
        case DATA_TYPE_PMARGB666:  
             l_width_algin=LCDC_ALGINED_1;
             l_height_algin=LCDC_ALGINED_1;
             break;    
             
        case DATA_TYPE_RGB565:
        case DATA_TYPE_ARGB565:
        case DATA_TYPE_ARGB555:  
        case DATA_TYPE_PMARGB565:  
             l_width_algin=LCDC_ALGINED_2;
             l_height_algin=LCDC_ALGINED_1;
             break;         
             
        default:
             SCI_TRACE_LOW("LCDC_GetAlginParam ERROR],LCDC:img data format err format:%d",data_format);
             break;
    }

    *width_algin=l_width_algin;
    *height_algin=l_height_algin;

    return SCI_TRUE;
}

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

