/******************************************************************************
 ** File Name:     lcd_ctrl_common.c                                          *
 ** Description:                                                              *
 **    This file contains display contrl implement on the lcd                 *
 ** Author:         Jianping.Wang                                             *
 ** DATE:           26/5/2010                                                 *
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

#ifdef  LCD_DATA_TYPE_RGB666
#define CAP_DATA_TYPE    LCD_RGB666
#define MEMORY_DATA_TYPE DATA_TYPE_ARGB666
#define DATA_BYTE_NUM    4
#else
#define CAP_DATA_TYPE    LCD_RGB565
#define MEMORY_DATA_TYPE DATA_TYPE_RGB565
#define DATA_BYTE_NUM    2
#endif

/**---------------------------------------------------------------------------*
 **                         local variable define                             *
 **---------------------------------------------------------------------------*/
LOCAL LCD_DISPLAY_CTRL_INFO_T s_lcd_ctrl_info = {0};
LOCAL uint32 s_save_data_buf = 0;

/*lint -save -e773  */
#define DECLARE_LCD_CTRL_INFO(s)  LCD_DISPLAY_CTRL_INFO_T *s=&s_lcd_ctrl_info
/*lint -restore */

LOCAL uint32 convert_angle(DISPLAY_ANGLE_E angle);
/**---------------------------------------------------------------------------*
 **                         local function define                             *
 **---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: get event
// Author: Jianping.Wang
/******************************************************************************/
LOCAL uint32 _get_ctrl_event(uint32 requested_flags, uint32 wait_option)
{
    DECLARE_LCD_CTRL_INFO(s);
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;
    if(PNULL!=s->lcd_ctrl_event)
    {
        status = SCI_GetEvent(s->lcd_ctrl_event,
                                requested_flags,
                                SCI_OR_CLEAR,
                                &actual_event,
                                SCI_WAIT_FOREVER);
    }

    return status;
}
/******************************************************************************/
// Description: clear event
// Author: Jianping.Wang
// input parameter:
/******************************************************************************/
LOCAL void _clear_ctrl_event(void)
{
    DECLARE_LCD_CTRL_INFO(s);

    if(PNULL!=s->lcd_ctrl_event)
    {
        SCI_SetEvent(s->lcd_ctrl_event, 0, SCI_AND);
    }
}
/******************************************************************************/
// Description: rotation by software
// Author: Jianping.Wang
/******************************************************************************/
 LOCAL void _software_rotation(LCD_ID_E lcd_id,uint16 *dst_ptr,uint16 *src_ptr,LCD_ANGLE_E angle)
 {
        DECLARE_LCD_CTRL_INFO(s);
        int32 i=0,j=0;
        uint16 *temp_ptr = PNULL;
        uint16 logic_width = s->logic_width[lcd_id];
        uint16 logic_height = s->logic_height[lcd_id];

        switch(angle)
        {
            case LCD_ANGLE_90:
                temp_ptr = dst_ptr + logic_height-1;
                for( i=0 ; i<logic_height ; i++)
                {
                    dst_ptr = temp_ptr-i;
                    for( j=0 ; j<logic_width ; j++)
                    {
                        *dst_ptr = *src_ptr++;
                        dst_ptr += logic_height;
                    }
                }
            break;
            case LCD_ANGLE_180:
                temp_ptr = dst_ptr + logic_width*logic_height-1;
                dst_ptr = temp_ptr;
                for( i=0 ; i<logic_height ; i++)
                {
                    for( j=0 ; j<logic_width ; j++)
                    {
                        *dst_ptr-- = *src_ptr++;
                    }
                }
            break;
            case LCD_ANGLE_270:
                temp_ptr = dst_ptr + (logic_width - 1)*logic_height;
                for( i=0 ; i<logic_height ; i++)
                {
                    dst_ptr = temp_ptr+i;
                    for( j=0 ; j<logic_width ; j++)
                    {
                        *dst_ptr = *src_ptr++;
                        dst_ptr -= logic_height;
                    }
                }
            break;
            default:
                //SCI_TRACE_LOW:"[_software_rotation ERROR],angle = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_134_112_2_18_1_45_0_33,(uint8*)"d",angle);
            break;
        }
 }
/******************************************************************************/
// Description: hardware rotation
// Author: Jianping.Wang
/******************************************************************************/
LOCAL ERR_DISPLAY_E _rotation_by_hw(LCD_ID_E lcd_id,
                                         uint16 *dst_ptr,
                                         uint16 *src_ptr,
                                         DISPLAY_LAYER_INFO_T *layer_ptr,
                                         DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
#ifndef WIN32

#if (defined(PLATFORM_SC8800H)  && defined(CHIP_ENDIAN_LITTLE))
    _software_rotation(lcd_id,dst_ptr,src_ptr,(LCD_ANGLE_E)param_ptr->rot_angle);
#else
    DISPLAY_LAYER_INFO_T *blk_info_ptr = layer_ptr;
    ROTATION_PARAM_T rotation_param = {0};

    switch(blk_info_ptr->layer_data.type)
    {
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_PMARGB888:
            rotation_param.data_format = ROTATION_RGB888;
            break;
        case DATA_TYPE_ARGB666:
        case DATA_TYPE_PMARGB666:
            rotation_param.data_format = ROTATION_RGB666;
            break;
        case DATA_TYPE_ARGB565:
        case DATA_TYPE_ARGB555:
        case DATA_TYPE_RGB565:
        case DATA_TYPE_PMARGB565:
            rotation_param.data_format = ROTATION_RGB565;
            break;
        case DATA_TYPE_GREY:
            rotation_param.data_format = ROTATION_YUV400;
            break;
        case DATA_TYPE_YUV422:
            rotation_param.data_format = ROTATION_YUV422;
            rotation_param.src_addr.uv_addr = layer_ptr->uv_buf_addr;
            rotation_param.dst_addr.uv_addr = (uint32)dst_ptr+layer_ptr->width*layer_ptr->height;
            layer_ptr->buf_addr = (uint32)dst_ptr;
            layer_ptr->uv_buf_addr = rotation_param.dst_addr.uv_addr;
            break;
        case DATA_TYPE_YUV420:
            rotation_param.data_format = ROTATION_YUV420;
            rotation_param.src_addr.uv_addr = layer_ptr->uv_buf_addr;
            rotation_param.dst_addr.uv_addr = (uint32)dst_ptr+layer_ptr->width*layer_ptr->height;
            layer_ptr->buf_addr = (uint32)dst_ptr;
            layer_ptr->uv_buf_addr = rotation_param.dst_addr.uv_addr;
            break;
        case DATA_TYPE_YUV400:
            rotation_param.data_format = ROTATION_YUV400;
            layer_ptr->buf_addr = (uint32)dst_ptr;
            break;
        default:
            rotation_param.data_format = ROTATION_MAX;
            break;
    }
    if(rotation_param.data_format == ROTATION_MAX)
    {
        //SCI_TRACE_LOW:"_rotation_by_hw:[ERROR]don't support format %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_198_112_2_18_1_45_1_34,(uint8*)"d",blk_info_ptr->layer_data.type);
        return ERR_DISPLAY_FUNC_NOT_SUPPORT;
    }
    rotation_param.src_addr.y_addr = (uint32)src_ptr;
    rotation_param.dst_addr.y_addr = (uint32)dst_ptr;
    if((param_ptr->rot_angle==DISPLAY_ANGLE_90) || (param_ptr->rot_angle==DISPLAY_ANGLE_270))
    {
        rotation_param.img_size.w = blk_info_ptr->height;
        rotation_param.img_size.h = blk_info_ptr->width;
    }
    else
    {
        rotation_param.img_size.w = blk_info_ptr->width ;
        rotation_param.img_size.h = blk_info_ptr->height;
    }
    rotation_param.rotation_mode = (ROTATION_MODE_E)convert_angle(param_ptr->rot_angle);


    if(SCI_SUCCESS != Rotation_AppStart(&rotation_param))
    {
        //SCI_TRACE_LOW:"LCDGUI_CTRL:Rotation_AppStart failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_218_112_2_18_1_45_1_35,(uint8*)"");
    }
#endif

#endif
    return ret;
}
/******************************************************************************/
// Description: coordinate convert from phsical plane to logic plane
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC ERR_DISPLAY_E _convert_coordinate (LCD_ID_E  lcd_id,
                                     DISPLAY_CONVERT_INPUT_T *out_ptr,
                                     DISPLAY_CONVERT_OUTPUT_T *input_ptr
                                     )
{
    DECLARE_LCD_CTRL_INFO(s);
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    DISPLAY_ANGLE_E rot_angle    = DISPLAY_ANGLE_0;
    uint16 logic_width = 0;
    uint16 logic_height = 0;
    int16 left = 0,top = 0,right=0,bottom=0;
    LCD_INFO_T lcd_info = {0};

    //DISPLAY_TRACE:"DISPLAY_ConvertCoordinate:rotation_angle=%d,left=%d,top=%d,right=%d,bottom=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_243_112_2_18_1_45_1_36,(uint8*)"ddddd", input_ptr->rotation_angle,input_ptr->physical_coordinate.left,input_ptr->physical_coordinate.top, input_ptr->physical_coordinate.right, input_ptr->physical_coordinate.bottom);


    LCD_GetInfo(lcd_id, &lcd_info);

    logic_width = s->logic_width[lcd_id];
    logic_height = s->logic_height[lcd_id];

    left      = input_ptr->physical_coordinate.left;
    top       = input_ptr->physical_coordinate.top;
    right     = input_ptr->physical_coordinate.right;
    bottom    = input_ptr->physical_coordinate.bottom;
    rot_angle = input_ptr->rotation_angle;

    if(((left>=right) || (top>=bottom)) &&(bottom!=0)&&(right!=0))
    {
        //SCI_TRACE_LOW:"_convert_coordinate:[ERROR] physical rect is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_259_112_2_18_1_45_1_37,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    switch(rot_angle)
    {
        case DISPLAY_ANGLE_0:
        break;
        case DISPLAY_ANGLE_90:
        #ifdef LCD_INSTALL_ANGLE_90
        out_ptr->logic_coordinate.left   = left;
        out_ptr->logic_coordinate.top    = top;
        out_ptr->logic_coordinate.right  = right;
        out_ptr->logic_coordinate.bottom = bottom;
        #else
        if((right==0) &&(bottom==0))//pixel convert
        {
            out_ptr->logic_coordinate.left = top;
            out_ptr->logic_coordinate.top  = logic_height-1-left;
        }
        else
        {
            out_ptr->logic_coordinate.left   = top;
            out_ptr->logic_coordinate.top    = logic_height-1-right;
            out_ptr->logic_coordinate.right  = bottom;
            out_ptr->logic_coordinate.bottom = logic_height-1-left;
        }
        #endif
        break;
        case DISPLAY_ANGLE_180:
        if((right==0) &&(bottom==0))//pixel convert
        {
            out_ptr->logic_coordinate.left = logic_width-1-left;
            out_ptr->logic_coordinate.top  = logic_height-1-top;
        }
        else
        {
            out_ptr->logic_coordinate.left   = logic_width-1-right;
            out_ptr->logic_coordinate.top    = logic_height-1-bottom;
            out_ptr->logic_coordinate.right  = logic_width-1-left;
            out_ptr->logic_coordinate.bottom = logic_height-1-top;
        }
        break;
        case DISPLAY_ANGLE_270:
        if((right==0) &&(bottom==0))//pixel convert
        {
            out_ptr->logic_coordinate.left = logic_width-1-top;
            out_ptr->logic_coordinate.top  = left;
        }
        else
        {
            out_ptr->logic_coordinate.left   = logic_width-1-bottom;
            out_ptr->logic_coordinate.top    = left;
            out_ptr->logic_coordinate.right  = logic_width-1-top;
            out_ptr->logic_coordinate.bottom = right;
        }
        break;
        default:
        ret = ERR_DISPLAY_PARAMETER_WRONG;
        break;
    }
    if(s->lcd_direction[lcd_id] == LCD_PLACED_VERTICAL)
    {
        out_ptr->logic_angle = input_ptr->rotation_angle;
    }
    else
    {
        switch(input_ptr->rotation_angle)
        {
            case DISPLAY_ANGLE_0:
                out_ptr->logic_angle = DISPLAY_ANGLE_90;
                break;
            case DISPLAY_ANGLE_90:
                out_ptr->logic_angle = DISPLAY_ANGLE_0;
                break;
            case DISPLAY_ANGLE_180:
                out_ptr->logic_angle = DISPLAY_ANGLE_270;
                break;
            case DISPLAY_ANGLE_270:
                out_ptr->logic_angle = DISPLAY_ANGLE_180;
                break;
            default:
                //SCI_TRACE_LOW:"[LCDCTRL:]_convert_coordinate,rotaion_angle ERROR!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_334_112_2_18_1_45_1_38,(uint8*)"");
                break;
        }
    }

    //DISPLAY_TRACE:"DISPLAY_ConvertCoordinate:out:angle=%d,left=%d,top=%d,right=%d,bottom=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_341_112_2_18_1_45_1_39,(uint8*)"ddddd", out_ptr->logic_angle,out_ptr->logic_coordinate.left,out_ptr->logic_coordinate.top, out_ptr->logic_coordinate.right,out_ptr->logic_coordinate.bottom);

   return ret;

}
/******************************************************************************/
// Description: rotation with crop by software
// Author: Jianping.Wang
/******************************************************************************/
LOCAL ERR_DISPLAY_E _software_rotation_halfword(LCD_ID_E lcd_id,
                                                       LCD_BUF_INFO_T *dst_ptr,
                                                       uint16 *src_ptr,
                                                       DISPLAY_LAYER_INFO_T *layer_ptr,
                                                       DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    int32 i=0,j=0;
    uint16 *temp_ptr = PNULL;
    DISPLAY_CONVERT_INPUT_T out_put = {0};
    DISPLAY_CONVERT_OUTPUT_T input = {0};
    uint16 *dst_offset_ptr = PNULL;
    uint16 *src_offset_ptr = PNULL;
    int16 left   = 0;
    int16 top    = 0;
    int16 right  = 0;
    int16 bottom = 0;
    uint16 rect_width  = 0;
    uint16 rect_height = 0;
    uint16 dst_mem_width = dst_ptr->width;

    input.physical_coordinate = param_ptr->rect;
    input.rotation_angle = param_ptr->rot_angle;
    ret = _convert_coordinate(lcd_id,&out_put,&input);

    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_375_112_2_18_1_45_1_40,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    left   = out_put.logic_coordinate.left;
    top    = out_put.logic_coordinate.top;
    right  = out_put.logic_coordinate.right;
    bottom = out_put.logic_coordinate.bottom;

    input.physical_coordinate.left   = layer_ptr->pos.x;
    input.physical_coordinate.top    = layer_ptr->pos.y;
    input.physical_coordinate.right  = layer_ptr->pos.x+layer_ptr->width-1;
    input.physical_coordinate.bottom = layer_ptr->pos.y+layer_ptr->height-1;

    ret = _convert_coordinate(lcd_id,&out_put,&input);
    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_391_112_2_18_1_45_1_41,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    dst_offset_ptr = (uint16*)dst_ptr->addr;
    src_offset_ptr = src_ptr;

    if((param_ptr->rot_angle == DISPLAY_ANGLE_90) ||(param_ptr->rot_angle ==DISPLAY_ANGLE_270))
    {
        src_offset_ptr += ((top-out_put.logic_coordinate.top)*layer_ptr->height+(left-out_put.logic_coordinate.left));
    }
    else
    {
        src_offset_ptr += ((top-out_put.logic_coordinate.top)*layer_ptr->width+(left-out_put.logic_coordinate.left));
    }

    left   = param_ptr->rect.left;
    top    = param_ptr->rect.top;
    right  = param_ptr->rect.right;
    bottom = param_ptr->rect.bottom;
    rect_width  = right-left+1;
    rect_height = bottom-top+1;

    switch(param_ptr->rot_angle)
    {
        case DISPLAY_ANGLE_90:
            temp_ptr = dst_offset_ptr + top*dst_mem_width+right;
            for( i=0 ; i<rect_width ; i++)
            {
                dst_offset_ptr = temp_ptr-i;
                for( j=0 ; j<rect_height ; j++)
                {
                    *dst_offset_ptr = *src_offset_ptr++;
                     dst_offset_ptr += dst_mem_width;
                }
                src_offset_ptr += layer_ptr->height-rect_height;
            }
        break;
        case DISPLAY_ANGLE_180:
            temp_ptr = dst_offset_ptr + (bottom*dst_mem_width+right);
            dst_offset_ptr = temp_ptr;
            for( i=0 ; i<rect_height ; i++)
            {
                dst_offset_ptr = temp_ptr-i*dst_mem_width;
                for( j=0 ; j<rect_width ; j++)
                {
                    *dst_offset_ptr-- = *src_offset_ptr++;
                }
                src_offset_ptr += layer_ptr->width-rect_width;
            }
        break;
        case DISPLAY_ANGLE_270:
            temp_ptr = dst_offset_ptr + (bottom*dst_mem_width+left);
            for( i=0 ; i<rect_width; i++)
            {
                dst_offset_ptr = temp_ptr+i;
                for( j=0 ; j<rect_height; j++)
                {
                    *dst_offset_ptr = *src_offset_ptr++;
                    dst_offset_ptr -= dst_mem_width;
                }
                src_offset_ptr += layer_ptr->height-rect_height;
            }
        break;
        default:
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]param is error! %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_456_112_2_18_1_45_1_42,(uint8*)"d",param_ptr->rot_angle);
        break;
    }
    return ret;
 }
/******************************************************************************/
// Description: rotation with crop by software
// Author: Jianping.Wang
/******************************************************************************/
LOCAL ERR_DISPLAY_E _software_rotation_word(LCD_ID_E lcd_id,
                                                   LCD_BUF_INFO_T *dst_ptr,
                                                   uint16 *src_ptr,
                                                   DISPLAY_LAYER_INFO_T *layer_ptr,
                                                   DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    int32 i=0,j=0;
    uint32 *temp_ptr = PNULL;
    DISPLAY_CONVERT_INPUT_T out_put = {0};
    DISPLAY_CONVERT_OUTPUT_T input = {0};
    uint32 *dst_offset_ptr = PNULL;
    uint32 *src_offset_ptr = PNULL;
    int16 left   = 0;
    int16 top    = 0;
    int16 right  = 0;
    int16 bottom = 0;
    uint16 rect_width  = 0;
    uint16 rect_height = 0;
    uint16 dst_mem_width = dst_ptr->width;

    input.physical_coordinate = param_ptr->rect;
    input.rotation_angle = param_ptr->rot_angle;
    ret = _convert_coordinate(lcd_id,&out_put,&input);

    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_490_112_2_18_1_45_1_43,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    left   = out_put.logic_coordinate.left;
    top    = out_put.logic_coordinate.top;
    right  = out_put.logic_coordinate.right;
    bottom = out_put.logic_coordinate.bottom;

    input.physical_coordinate.left   = layer_ptr->pos.x;
    input.physical_coordinate.top    = layer_ptr->pos.y;
    input.physical_coordinate.right  = layer_ptr->pos.x+layer_ptr->width-1;
    input.physical_coordinate.bottom = layer_ptr->pos.y+layer_ptr->height-1;

    ret = _convert_coordinate(lcd_id,&out_put,&input);
    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_506_112_2_18_1_45_1_44,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    dst_offset_ptr = (uint32*)dst_ptr->addr;
    src_offset_ptr = (uint32*)src_ptr;

    if((param_ptr->rot_angle == DISPLAY_ANGLE_90) ||(param_ptr->rot_angle ==DISPLAY_ANGLE_270))
    {
        src_offset_ptr += ((top-out_put.logic_coordinate.top)*layer_ptr->height+(left-out_put.logic_coordinate.left));
    }
    else
    {
        src_offset_ptr += ((top-out_put.logic_coordinate.top)*layer_ptr->width+(left-out_put.logic_coordinate.left));
    }

    left   = param_ptr->rect.left;
    top    = param_ptr->rect.top;
    right  = param_ptr->rect.right;
    bottom = param_ptr->rect.bottom;
    rect_width  = right-left+1;
    rect_height = bottom-top+1;

    switch(param_ptr->rot_angle)
    {
        case DISPLAY_ANGLE_90:
            temp_ptr = dst_offset_ptr + top*dst_mem_width+right;
            for( i=0 ; i<rect_width ; i++)
            {
                dst_offset_ptr = temp_ptr-i;
                for( j=0 ; j<rect_height ; j++)
                {
                    *dst_offset_ptr = *src_offset_ptr++;
                     dst_offset_ptr += dst_mem_width;
                }
                src_offset_ptr += layer_ptr->height-rect_height;
            }
        break;
        case DISPLAY_ANGLE_180:
            temp_ptr = dst_offset_ptr + (bottom*dst_mem_width+right);
            dst_offset_ptr = temp_ptr;
            for( i=0 ; i<rect_height ; i++)
            {
                dst_offset_ptr = temp_ptr-i*dst_mem_width;
                for( j=0 ; j<rect_width ; j++)
                {
                    *dst_offset_ptr-- = *src_offset_ptr++;
                }
                src_offset_ptr += layer_ptr->width-rect_width;
            }
        break;
        case DISPLAY_ANGLE_270:
            temp_ptr = dst_offset_ptr + (bottom*dst_mem_width+left);
            for( i=0 ; i<rect_width; i++)
            {
                dst_offset_ptr = temp_ptr+i;
                for( j=0 ; j<rect_height; j++)
                {
                    *dst_offset_ptr = *src_offset_ptr++;
                    dst_offset_ptr -= dst_mem_width;
                }
                src_offset_ptr += layer_ptr->height-rect_height;
            }
        break;
        default:
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            //SCI_TRACE_LOW:"_software_rotation_word:[ERROR]param is error! %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_571_112_2_18_1_45_1_45,(uint8*)"d",param_ptr->rot_angle);
        break;
    }
    return ret;
 }
/******************************************************************************/
// Description: rotation with crop by software
// Author: Jianping.Wang
/******************************************************************************/
LOCAL ERR_DISPLAY_E _software_rotation_byte(LCD_ID_E lcd_id,
                                                  LCD_BUF_INFO_T *dst_ptr,
                                                  uint16 *src_ptr,
                                                  DISPLAY_LAYER_INFO_T *layer_ptr,
                                                  DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    int32 i=0,j=0;
    uint8 *temp_ptr = PNULL;
    DISPLAY_CONVERT_INPUT_T out_put = {0};
    DISPLAY_CONVERT_OUTPUT_T input = {0};
    uint8 *dst_offset_ptr = PNULL;
    uint8 *src_offset_ptr = PNULL;
    int16 left   = 0;
    int16 top    = 0;
    int16 right  = 0;
    int16 bottom = 0;
    uint16 rect_width  = 0;
    uint16 rect_height = 0;
    uint16 dst_mem_width = dst_ptr->width;

    input.physical_coordinate = param_ptr->rect;
    input.rotation_angle = param_ptr->rot_angle;
    ret = _convert_coordinate(lcd_id,&out_put,&input);

    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_605_112_2_18_1_45_1_46,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    left   = out_put.logic_coordinate.left;
    top    = out_put.logic_coordinate.top;
    right  = out_put.logic_coordinate.right;
    bottom = out_put.logic_coordinate.bottom;

    input.physical_coordinate.left   = layer_ptr->pos.x;
    input.physical_coordinate.top    = layer_ptr->pos.y;
    input.physical_coordinate.right  = layer_ptr->pos.x+layer_ptr->width-1;
    input.physical_coordinate.bottom = layer_ptr->pos.y+layer_ptr->height-1;

    ret = _convert_coordinate(lcd_id,&out_put,&input);
    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_621_112_2_18_1_45_1_47,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    dst_offset_ptr = (uint8*)dst_ptr->addr;
    src_offset_ptr = (uint8*)src_ptr;

    if((param_ptr->rot_angle == DISPLAY_ANGLE_90) ||(param_ptr->rot_angle ==DISPLAY_ANGLE_270))
    {
        src_offset_ptr += ((top-out_put.logic_coordinate.top)*layer_ptr->height+(left-out_put.logic_coordinate.left));
    }
    else
    {
        src_offset_ptr += ((top-out_put.logic_coordinate.top)*layer_ptr->width+(left-out_put.logic_coordinate.left));
    }

    left   = param_ptr->rect.left;
    top    = param_ptr->rect.top;
    right  = param_ptr->rect.right;
    bottom = param_ptr->rect.bottom;
    rect_width  = right-left+1;
    rect_height = bottom-top+1;

    switch(param_ptr->rot_angle)
    {
        case DISPLAY_ANGLE_90:
            temp_ptr = dst_offset_ptr + top*dst_mem_width+right;
            for( i=0 ; i<rect_width ; i++)
            {
                dst_offset_ptr = temp_ptr-i;
                for( j=0 ; j<rect_height ; j++)
                {
                    *dst_offset_ptr = *src_offset_ptr++;
                     dst_offset_ptr += dst_mem_width;
                }
                src_offset_ptr += layer_ptr->height-rect_height;
            }
        break;
        case DISPLAY_ANGLE_180:
            temp_ptr = dst_offset_ptr + (bottom*dst_mem_width+right);
            dst_offset_ptr = temp_ptr;
            for( i=0 ; i<rect_height ; i++)
            {
                dst_offset_ptr = temp_ptr-i*dst_mem_width;
                for( j=0 ; j<rect_width ; j++)
                {
                    *dst_offset_ptr-- = *src_offset_ptr++;
                }
                src_offset_ptr += layer_ptr->width-rect_width;
            }
        break;
        case DISPLAY_ANGLE_270:
            temp_ptr = dst_offset_ptr + (bottom*dst_mem_width+left);
            for( i=0 ; i<rect_width; i++)
            {
                dst_offset_ptr = temp_ptr+i;
                for( j=0 ; j<rect_height; j++)
                {
                    *dst_offset_ptr = *src_offset_ptr++;
                    dst_offset_ptr -= dst_mem_width;
                }
                src_offset_ptr += layer_ptr->height-rect_height;
            }
        break;
        default:
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            //SCI_TRACE_LOW:"_software_rotation_byte:[ERROR]param is error! %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_686_112_2_18_1_45_2_48,(uint8*)"d",param_ptr->rot_angle);
        break;
    }
    return ret;
 }
/******************************************************************************/
// Description: rotation with crop by software
// Author: Jianping.Wang
/******************************************************************************/
LOCAL ERR_DISPLAY_E _software_rotation_with_crop(LCD_ID_E lcd_id,
                                                LCD_BUF_INFO_T *dst_ptr,
                                                uint16 *src_ptr,
                                                DISPLAY_LAYER_INFO_T *layer_ptr,
                                                DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    DISPLAY_DATA_TYPE_E layer_data_type = DATA_TYPE_MAX;
    DISPLAY_LAYER_INFO_T layer_info = {0};

    layer_data_type = layer_ptr->layer_data.type;

    switch(layer_data_type)
    {
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_ARGB666:
        case DATA_TYPE_PMARGB888:
        case DATA_TYPE_PMARGB666:
            ret = _software_rotation_word(lcd_id,dst_ptr,src_ptr,layer_ptr,param_ptr);
            break;
        case DATA_TYPE_ARGB555:
        case DATA_TYPE_ARGB565:
        case DATA_TYPE_RGB565:
        case DATA_TYPE_PMARGB565:
            ret = _software_rotation_halfword(lcd_id,dst_ptr,src_ptr,layer_ptr,param_ptr);
            break;
        case DATA_TYPE_GREY:
            ret = _software_rotation_byte(lcd_id,dst_ptr,src_ptr,&layer_info,param_ptr);
            break;
        default:
            ret = ERR_DISPLAY_FUNC_NOT_SUPPORT;
            //SCI_TRACE_LOW:"_software_rotation_with_crop:[ERROR]don't support format %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_724_112_2_18_1_45_2_49,(uint8*)"d",layer_data_type);
            break;
    }
    return ret;
}
/******************************************************************************/
// Description: rotation with crop by software
// Author: Jianping.Wang
/******************************************************************************/
LOCAL ERR_DISPLAY_E _software_rotation_with_yuv(LCD_ID_E lcd_id,
                                                LCD_BUF_INFO_T *dst_ptr,
                                                uint16 *src_ptr,
                                                DISPLAY_LAYER_INFO_T *layer_ptr,
                                                DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    DECLARE_LCD_CTRL_INFO(s);
    DISPLAY_CONVERT_INPUT_T out_put = {0};
    DISPLAY_CONVERT_OUTPUT_T input = {0};
    int16 rect_left   = 0;
    int16 rect_top    = 0;
    int16 rect_right  = 0;
    int16 rect_bottom = 0;
    uint8 *temp_ptr = PNULL;
    uint8 *src_temp_ptr = PNULL;

    //convert coordinate from physical plane to logical plane
    input.physical_coordinate = param_ptr->rect;
    input.rotation_angle = param_ptr->rot_angle;
    ret = _convert_coordinate(lcd_id,&out_put,&input);

    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"_software_rotation_halfword:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_755_112_2_18_1_45_2_50,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }
    rect_left   = out_put.logic_coordinate.left;
    rect_top    = out_put.logic_coordinate.top;
    rect_right  = out_put.logic_coordinate.right;
    rect_bottom = out_put.logic_coordinate.bottom;

    input.physical_coordinate.left   = layer_ptr->pos.x;
    input.physical_coordinate.top    = layer_ptr->pos.y;
    input.physical_coordinate.right  = layer_ptr->pos.x+layer_ptr->width-1;
    input.physical_coordinate.bottom = layer_ptr->pos.y+layer_ptr->height-1;

    ret = _convert_coordinate(lcd_id,&out_put,&input);
    if(ret != ERR_DISPLAY_NONE)
    {
        //SCI_TRACE_LOW:"[LCDCTRL:]_software_rotation_with_yuv,coordinate convert error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_771_112_2_18_1_45_2_51,(uint8*)"");
        return ERR_DISPLAY_PARAMETER_WRONG;
    }

    if(!((rect_left == out_put.logic_coordinate.left)
        &&(rect_top == out_put.logic_coordinate.top)
        &&(rect_right == out_put.logic_coordinate.right)
        &&(rect_bottom == out_put.logic_coordinate.bottom)))
    {
        int16 left   = 0;
        int16 top    = 0;
        int16 right  = 0;
        int16 bottom = 0;
        uint8 *y_ptr = PNULL;
        uint8 *uv_ptr = PNULL;
        uint32 i = 0;
        uint32 width = 0;
        uint32 height = 0;
        uint8 *src_copy_ptr = PNULL;
        uint16 layer_width = 0;

        temp_ptr = (uint8*)SCI_ALLOCA((rect_right-rect_left+1)*(rect_bottom-rect_top+1)*2+8);
        if(!temp_ptr)
        {
            //SCI_TRACE_LOW:"[LCDCTRL:]_software_rotation_with_yuv,alloc fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_795_112_2_18_1_45_2_52,(uint8*)"");
            return ERR_DISPLAY_FUNC_NOT_SUPPORT;
        }
        //SCI_TRACE_LOW:"[LCDCTRL:]_software_rotation_with_yuv,malloc addr=0x%x,size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_799_112_2_18_1_45_2_53,(uint8*)"dd",temp_ptr,((rect_right-rect_left+1)*(rect_bottom-rect_top+1)*2+8));
        left   = out_put.logic_coordinate.left;
        top    = out_put.logic_coordinate.top;
        right  = out_put.logic_coordinate.right;
        bottom = out_put.logic_coordinate.bottom;

        if(rect_left <= left)
        {
            rect_left = left;
        }
        if(rect_top <= top)
        {
            rect_top = top;
        }
        if(rect_right >= right)
        {
            rect_right = right;
        }
        if(rect_bottom >= bottom)
        {
            rect_bottom = bottom;
        }
        if((DISPLAY_ANGLE_90 == param_ptr->rot_angle)
            || (DISPLAY_ANGLE_270 == param_ptr->rot_angle))
        {
            layer_width = layer_ptr->height;
        }
        else
        {
            layer_width = layer_ptr->width;
        }
        width = rect_right-rect_left+1;
        height = rect_bottom-rect_top+1;
        src_temp_ptr = temp_ptr + 4-((uint32)temp_ptr&0x3);
        src_copy_ptr = src_temp_ptr;
        y_ptr = (uint8*)layer_ptr->buf_addr+(rect_left-left)+(rect_top-top)*layer_width;
        for( i=0 ; i<height ; i++)
        {
            SCI_MEMCPY(src_copy_ptr,y_ptr,width);
            y_ptr += layer_width;
            src_copy_ptr += width;
        }

        if(DATA_TYPE_YUV422 == layer_ptr->layer_data.type)
        {
            uv_ptr = (uint8*)layer_ptr->uv_buf_addr+(rect_left-left)+(rect_top-top)*layer_width;
            for( i=0 ; i<height ; i++)
            {
                SCI_MEMCPY(src_copy_ptr,uv_ptr,width);
                uv_ptr += layer_width;
                src_copy_ptr += width;
            }
            layer_ptr->uv_buf_addr = (uint32)src_temp_ptr + width*height;
        }
        if(DATA_TYPE_YUV420 == layer_ptr->layer_data.type)
        {
            uv_ptr = (uint8*)layer_ptr->uv_buf_addr+(rect_left-left)+(rect_top-top)*layer_width/2;
            for( i=0 ; i<(height/2) ; i++)
            {
                SCI_MEMCPY(src_copy_ptr,uv_ptr,width);
                uv_ptr += layer_width;
                src_copy_ptr += width;
            }
            layer_ptr->uv_buf_addr = (uint32)src_temp_ptr + width*height;
        }
        layer_ptr->buf_addr = (uint32)src_temp_ptr;

        //convert coordinate from logical plane to physical plane
        out_put.logic_coordinate.left = rect_left;
        out_put.logic_coordinate.top  = rect_top;
        out_put.logic_coordinate.right  = rect_right;
        out_put.logic_coordinate.bottom = rect_bottom;
        out_put.logic_lcd_width = s->logic_width[lcd_id];
        out_put.logic_lcd_height = s->logic_height[lcd_id];
        if(ERR_DISPLAY_NONE != DISPLAY_ConvertCoordinate(lcd_id,&out_put,&input))
        {
            //SCI_TRACE_LOW:"[LCDCTRL:],_software_rotation_with_yuv,convert ERROR!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_875_112_2_18_1_45_2_54,(uint8*)"");
            return ERR_DISPLAY_PARAMETER_WRONG;
        }
        layer_ptr->width = input.physical_coordinate.right-input.physical_coordinate.left+1;
        layer_ptr->height = input.physical_coordinate.bottom-input.physical_coordinate.top+1;
        layer_ptr->pos.x = input.physical_coordinate.left;
        layer_ptr->pos.y = input.physical_coordinate.top;

    }

    ret = _rotation_by_hw(lcd_id,(uint16*)dst_ptr->addr,(uint16*)layer_ptr->buf_addr,layer_ptr,param_ptr);
    if(temp_ptr)
    {
        SCI_FREE(temp_ptr);
        temp_ptr = PNULL;
    }

    return ret;
}
/******************************************************************************/
// Description: display rotation
// Author: Jianping.Wang
/******************************************************************************/
LOCAL ERR_DISPLAY_E _display_software_rotation(LCD_ID_E lcd_id,
                                                     LCD_BUF_INFO_T *dst_ptr,
                                                     uint16 *src_ptr,
                                                     DISPLAY_LAYER_INFO_T *layer_ptr,
                                                     DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    DECLARE_LCD_CTRL_INFO(s);
    LCD_INFO_T lcd_info = {0};
    uint32 rect_w = param_ptr->rect.right-param_ptr->rect.left+1;
    uint32 rect_h = param_ptr->rect.bottom-param_ptr->rect.top+1;

    LCD_GetInfo(lcd_id, &lcd_info);
    #ifdef LCD_INSTALL_ANGLE_90
    ret = _rotation_by_hw(lcd_id,(uint16*)dst_ptr->addr,src_ptr,layer_ptr,param_ptr);
    #else
    if(((rect_w*rect_h)>=((lcd_info.lcd_width*lcd_info.lcd_height)>>2)) \
        || (s->full_copy_flag[lcd_id]==1))
    {
        //SCI_TRACE_LOW:"_display_software_rotation:full copy"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_913_112_2_18_1_45_2_55,(uint8*)"");
        ret = _rotation_by_hw(lcd_id,(uint16*)dst_ptr->addr,src_ptr,layer_ptr,param_ptr);
    }
    else
    {
        if((DATA_TYPE_YUV420 == layer_ptr->layer_data.type)
            ||(DATA_TYPE_YUV422 == layer_ptr->layer_data.type)
            ||(DATA_TYPE_YUV400 == layer_ptr->layer_data.type))
        {
            ret = _software_rotation_with_yuv(lcd_id,dst_ptr,src_ptr,layer_ptr,param_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"_display_software_rotation:crop copy"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_926_112_2_18_1_45_2_56,(uint8*)"");
            ret = _software_rotation_with_crop(lcd_id,dst_ptr,src_ptr,layer_ptr,param_ptr);
        }
    }
    #endif
    return ret;
}
/******************************************************************************/
// Description: convert angle
// Author: Jianping.Wang
/******************************************************************************/
LOCAL uint32 convert_angle(DISPLAY_ANGLE_E angle)
{
    uint32 ret = ROTATION_90;

    switch(angle)
    {
        case DISPLAY_ANGLE_90:
            ret = ROTATION_90;
            break;
        case DISPLAY_ANGLE_180:
            ret = ROTATION_180;
            break;
        case DISPLAY_ANGLE_270:
            ret = ROTATION_270;
            break;
        default:
            //SCI_TRACE_LOW:"convert_angle: default!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_951_112_2_18_1_45_2_57,(uint8*)"");
            break;
    }

    return ret;
}
LOCAL LCD_DIRECT_E convert_angle_display2lcd_dir(DISPLAY_ANGLE_E angle)
{
    LCD_DIRECT_E ret = LCD_DIRECT_NORMAL;

    switch(angle)
    {
        case DISPLAY_ANGLE_0:
            ret = LCD_DIRECT_NORMAL;
            break;

        case DISPLAY_ANGLE_90:
            ret = LCD_DIRECT_ROT_90;
            break;

        case DISPLAY_ANGLE_180:
            ret = LCD_DIRECT_ROT_180;
            break;

        case DISPLAY_ANGLE_270:
            ret = LCD_DIRECT_ROT_270;
            break;

        default:
        //SCI_TRACE_LOW:"convert_angle: default!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_980_112_2_18_1_45_2_58,(uint8*)"");
        break;
    }

    return ret;
}
#if 0
/******************************************************************************/
// Description: invalidate on nor platform
// Author: Jianping.Wang
/******************************************************************************/
LOCAL void _invalidate_on_nor(LCD_ID_E  lcd_id,
                                   DISPLAY_LAYER_INFO_T *layer_arr[],
                                   uint32 array_size,
                                   DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    DECLARE_LCD_CTRL_INFO(s);
    uint16 logic_width = s->logic_width[lcd_id];
    uint16 logic_height = s->logic_height[lcd_id];
    uint32 dst_ptr = NULL;
    uint32 blk_id = 0;
    BLOCKCFG_T cfg = {0};
    DISPLAY_LAYER_INFO_T *layer_ptr = PNULL;

    layer_ptr = layer_arr[0];
    blk_id = layer_ptr->layer_id;

    _convert_cfg(layer_ptr,&cfg);
    LCD_ConfigBlock(lcd_id, blk_id,cfg);

    if(s->menu_buffer_dc[lcd_id][0].addr != 0)
    {
         //SCI_TRACE_LOW:"LCDGUI_Ctrl:_invalidate_on_nor:dc mode"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1009_112_2_18_1_45_2_59,(uint8*)"");
         SCI_MEMCPY((void*)s->menu_buffer_dc[lcd_id][0].addr,(void*)layer_ptr->buf_addr,(logic_width*logic_height*2));
         dst_ptr = s->menu_buffer_dc[lcd_id][0].addr;
    }
    else
    {
        dst_ptr = layer_ptr->buf_addr;
    }
    LCD_SetBlockBuffer(lcd_id, blk_id, (uint32*)dst_ptr);
    if(param_ptr->rot_angle != LCD_ANGLE_0)
    {
        LCD_SetRotateMode(LCD_ROT_MODE_DMA);
    }

    LCD_SetDirection(lcd_id,(LCD_DIRECT_E)param_ptr->rot_angle);
    LCD_EnableBlock(lcd_id, blk_id);

    if(s->limit_type[lcd_id] != NO_PERMIT_GUI_INVALIDATE) //dc mode
    {
         LCD_InvalidateRect(lcd_id,param_ptr->rect.left,param_ptr->rect.top,\
                                     param_ptr->rect.right,param_ptr->rect.bottom);
    }
}
#endif
/******************************************************************************/
// Description: invalidate on nand platform
// Author: Jianping.Wang
/******************************************************************************/
LOCAL int32 _blend_and_rotate( LCD_ID_E lcd_id,
                                   DISPLAY_LAYER_INFO_T *layer_arr[],
                                   uint32 layer_num,
                                   DISPLAY_INVALIDATE_POS_T *param_ptr)
{
#ifndef WIN32
    DECLARE_LCD_CTRL_INFO(s);
    uint32 blk_id;
    uint16 *src_ptr = PNULL;
    BLOCKCFG_T cfg={0};
    LCD_BRUSH_MODE_E brush_mode = LCD_BRUSH_MODE_SYNC;
    uint32 i=0;
    DISPLAY_LAYER_INFO_T *blk_info_ptr = 0;
    DISPLAY_LAYER_INFO_T display_blk = {0};
    LCD_BUF_INFO_T *dst_buf_info_ptr = PNULL;
    uint32 mem_addr = 0;
    uint32 *mem_addr_ptr = PNULL;
    uint32 temp_buf_addr = 0;
    uint16 rect_w = 0;
    uint16 rect_h = 0;
    BLOCK_T blk_info[HW_BLOCK_NUM] = {0};
    DISPLAY_CONVERT_INPUT_T out_put = {0};
    DISPLAY_CONVERT_OUTPUT_T input = {0};
    LCD_MEMORY_INFO_T mem_info = {0};
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;

    LCD_SetIsUseCtrlSm(SCI_FALSE);
    _clear_ctrl_event();

    //DISPLAY_TRACE:"_blend_and_rotate:param_ptr->rot_angle=%d,layer_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1060_112_2_18_1_45_2_60,(uint8*)"dd",param_ptr->rot_angle,layer_num);

    LCD_GetBlockInfo(lcd_id,&blk_info[0]);
    LCD_DisableAllBlock(lcd_id);

    switch(param_ptr->rot_angle)
    {
        case DISPLAY_ANGLE_90:
        case DISPLAY_ANGLE_270:
            rect_h = param_ptr->rect.right-param_ptr->rect.left+1;
            rect_w = param_ptr->rect.bottom-param_ptr->rect.top+1;
            display_blk.width  = rect_h;
            display_blk.height = rect_w;
            break;
        case DISPLAY_ANGLE_180:
            rect_w = param_ptr->rect.right-param_ptr->rect.left+1;
            rect_h= param_ptr->rect.bottom-param_ptr->rect.top+1;
            display_blk.width  = rect_w;
            display_blk.height = rect_h;
            break;
        default:
            //SCI_TRACE_LOW:"[_blend_and_rotate ERROR]:angle is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1081_112_2_18_1_45_2_61,(uint8*)"");
            break;
    }
    if((0==rect_w) || (0==rect_h))
    {
        return -1;
    }
    mem_addr_ptr = (uint32*)SCI_ALLOCA(rect_w*rect_h*DATA_BYTE_NUM+8);
    mem_addr = (uint32)mem_addr_ptr;
    if(0 == mem_addr)
    {
        return -1;
    }
    temp_buf_addr = (uint16*)(((uint32)mem_addr & 7) == 0) ? mem_addr : (mem_addr+(8-((uint32)mem_addr&7)));

    for( i=0 ; i<layer_num ; i++ )
    {
        blk_info_ptr = layer_arr[i];
        SCI_PASSERT((blk_info_ptr!=PNULL),("layer is NULL layer_id = %d,array_size=%d",i,layer_num));/*assert verified*/

        blk_id = blk_info_ptr->layer_id;

        //convert from physical plane to logic plane
        input.rotation_angle = param_ptr->rot_angle;
        input.physical_coordinate.left   = blk_info_ptr->pos.x;
        input.physical_coordinate.top    = blk_info_ptr->pos.y;
        input.physical_coordinate.right  = blk_info_ptr->pos.x + blk_info_ptr->width - 1;
        input.physical_coordinate.bottom = blk_info_ptr->pos.y + blk_info_ptr->height - 1;

		ret = _convert_coordinate(lcd_id,&out_put,&input);
        if(ret != ERR_DISPLAY_NONE)
        {
            SCI_TRACE_LOW("_software_rotation_halfword:[ERROR]");
            return ERR_DISPLAY_PARAMETER_WRONG;
        }

        blk_info_ptr->pos.x  = out_put.logic_coordinate.left;
        blk_info_ptr->pos.y  = out_put.logic_coordinate.top;
        blk_info_ptr->width  = out_put.logic_coordinate.right-out_put.logic_coordinate.left+1;
        blk_info_ptr->height = out_put.logic_coordinate.bottom-out_put.logic_coordinate.top+1;
        //
        _convert_cfg(blk_info_ptr,&cfg);
        LCD_ConfigBlock(lcd_id, blk_id,cfg);

        LCD_SetBlockBuffer(lcd_id, blk_id, (uint32*)blk_info_ptr->buf_addr);
        if(((DATA_TYPE_YUV422 == blk_info_ptr->layer_data.type)
            ||(DATA_TYPE_YUV420 == blk_info_ptr->layer_data.type))&&(0==blk_id))
        {
            LCD_SetUVBuffer(lcd_id,blk_id, (uint32*)blk_info_ptr->uv_buf_addr);
        }
        LCD_EnableBlock(lcd_id, blk_id);
    }

    //convert from physical plane to logic plane
    input.rotation_angle = param_ptr->rot_angle;
    input.physical_coordinate.left   = param_ptr->rect.left;
    input.physical_coordinate.top    = param_ptr->rect.top;
    input.physical_coordinate.right  = param_ptr->rect.right;
    input.physical_coordinate.bottom = param_ptr->rect.bottom;
    ret = _convert_coordinate(lcd_id,&out_put,&input);
    if(ret != ERR_DISPLAY_NONE)
    {
	   SCI_TRACE_LOW("_software_rotation_halfword:[ERROR]");
	   return ERR_DISPLAY_PARAMETER_WRONG;
    }

    mem_info.buf_addr = temp_buf_addr;
    mem_info.cap_rect.left   = out_put.logic_coordinate.left;
    mem_info.cap_rect.top    = out_put.logic_coordinate.top;
    mem_info.cap_rect.right  = out_put.logic_coordinate.right;
    mem_info.cap_rect.bottom = out_put.logic_coordinate.bottom;
    mem_info.data_type = CAP_DATA_TYPE;
    mem_info.dst_size.w = rect_w;
    mem_info.dst_size.h = rect_h;
    mem_info.dst_trim_rect.left = 0;
    mem_info.dst_trim_rect.top  = 0;
    mem_info.dst_trim_rect.right  = rect_w - 1;
    mem_info.dst_trim_rect.bottom = rect_h - 1;

    LCD_InvalidateRectToMemory(lcd_id,&mem_info);
    LCD_DisableAllBlock(lcd_id);
    //rotate handle
    dst_buf_info_ptr = &s->menu_buffer_info[lcd_id][0];
    src_ptr = (uint16*)temp_buf_addr;
    SCI_PASSERT((dst_buf_info_ptr!=PNULL),("s->menu_buffer_info[lcd_id][0] is NULL!"));/*assert verified*/
    SCI_PASSERT((dst_buf_info_ptr->size>=rect_w*rect_h*2),("rotate dst buf is small,left=%d,top=%d,right=%d,bottom=%d", param_ptr->rect.left,param_ptr->rect.top,param_ptr->rect.right,param_ptr->rect.bottom));/*assert verified*/

    display_blk.buf_addr = temp_buf_addr;
    display_blk.is_enable = SCI_TRUE;
    display_blk.layer_id  = layer_arr[0]->layer_id;
    display_blk.pos.x = param_ptr->rect.left;
    display_blk.pos.y = param_ptr->rect.top;
    display_blk.layer_data.alpha = 255;
    display_blk.layer_data.alpha_sel   = DISPLAY_BLOCK_ALPHA;
    display_blk.layer_data.colorkey_en = SCI_FALSE;
    display_blk.layer_data.colorkey = 0;
    display_blk.layer_data.type = MEMORY_DATA_TYPE;
    display_blk.layer_data.grey_value = 0;

    if(ERR_DISPLAY_NONE != _software_rotation_with_crop(lcd_id,dst_buf_info_ptr,src_ptr,&display_blk,param_ptr))
    {
        SCI_FREE(mem_addr_ptr);
        mem_addr_ptr = PNULL;
        mem_addr = 0;

        LCD_SetIsUseCtrlSm(SCI_TRUE);
        LCD_PutLcdCtrlSemaphore(lcd_id);
        //SCI_TRACE_LOW:"_display_software_rotation:[ERROR]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1176_112_2_18_1_45_3_62,(uint8*)"");
        return -1;
    }

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

    blk_id = display_blk.layer_id;
    display_blk.pos.x = 0;
    display_blk.pos.y = 0;
    display_blk.width  = dst_buf_info_ptr->width;
    display_blk.height = dst_buf_info_ptr->height;
    _convert_cfg(&display_blk,&cfg);
    LCD_ConfigBlock(lcd_id, blk_id,cfg);

    LCD_SetBlockBuffer(lcd_id, blk_id, (uint32*)dst_buf_info_ptr->addr);
    LCD_EnableBlock(lcd_id, blk_id);
    brush_mode = LCD_BRUSH_MODE_ASYNC;

    SCI_FREE(mem_addr_ptr);
    mem_addr_ptr = PNULL;
    mem_addr = 0;

    LCD_SetIsUseCtrlSm(SCI_TRUE);
    return 0;
#endif
}

LOCAL int32 _osd_copy(
                        LCD_ID_E lcd_id,
                        uint32 layer_num,
                        DISPLAY_LAYER_INFO_T *blk_info_ptr,
                        DISPLAY_INVALIDATE_POS_T *param_ptr
                      )
{
    DECLARE_LCD_CTRL_INFO(s);
    uint16 y_offset = 0;
    uint16 line_num = 0;
    uint32 dst_mem_size = 0;
    uint32 blk_size = 0;
    int32  ret = 0;

    if(s->is_copy_osd[lcd_id])
    {
        if(DATA_TYPE_RGB565 == blk_info_ptr->layer_data.type)
        {
            uint16 *dst_copy_ptr = PNULL;
            uint16 *src_ptr = PNULL;
            src_ptr = (uint16*)blk_info_ptr->buf_addr;

            dst_copy_ptr = (uint16*)s->menu_buffer_info[lcd_id][layer_num].addr;
            if(s->full_copy_flag[lcd_id])
            {
                line_num = blk_info_ptr->height;
                s->full_copy_flag[lcd_id] = 0;
            }
            else
            {
                if(blk_info_ptr->pos.y<=param_ptr->rect.top)
                {
                    y_offset = param_ptr->rect.top-blk_info_ptr->pos.y;
                }
                else
                {
                    y_offset = 0;
                }

                dst_copy_ptr += y_offset*blk_info_ptr->width;
                src_ptr += y_offset*blk_info_ptr->width;
                if((blk_info_ptr->pos.y+blk_info_ptr->height-1)>=param_ptr->rect.bottom)
                {
                    if(y_offset>0)
                    {
                        line_num = param_ptr->rect.bottom-param_ptr->rect.top+1;
                    }
                    else
                    {
                        line_num = param_ptr->rect.bottom-blk_info_ptr->pos.y+1;
                    }
                }
                else
                {
                    if(y_offset>0)
                    {
                        line_num = blk_info_ptr->pos.y+blk_info_ptr->height - y_offset;
                    }
                    else
                    {
                        line_num = blk_info_ptr->height;
                    }
                }
            }
            dst_mem_size = s->menu_buffer_info[lcd_id][layer_num].size;
            blk_size = (line_num*blk_info_ptr->width)<<1;
            if(dst_mem_size>=blk_size)
            {
                SCI_MEMCPY((void*)dst_copy_ptr,(void*)src_ptr,blk_size);
            }
            else
            {
                //SCI_TRACE_LOW:"dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1282_112_2_18_1_45_3_63,(uint8*)"ddd",layer_num,dst_mem_size,blk_size);
                ret = 1;
            }
        }
        else if(DATA_TYPE_ARGB888 == blk_info_ptr->layer_data.type)
        {

            uint32 *dst_copy_ptr = PNULL;
            uint32 *src_ptr = PNULL;

            src_ptr = (uint32*)blk_info_ptr->buf_addr;

            dst_copy_ptr = (uint32*)s->menu_buffer_info[lcd_id][layer_num].addr;
            if(s->full_copy_flag[lcd_id])
            {
                line_num = blk_info_ptr->height;
                s->full_copy_flag[lcd_id] = 0;
            }
            else
            {
                if(blk_info_ptr->pos.y<=param_ptr->rect.top)
                {
                    y_offset = param_ptr->rect.top-blk_info_ptr->pos.y;
                }
                else
                {
                    y_offset = 0;
                }

                dst_copy_ptr += y_offset*blk_info_ptr->width;
                src_ptr += y_offset*blk_info_ptr->width;
                if((blk_info_ptr->pos.y+blk_info_ptr->height-1)>=param_ptr->rect.bottom)
                {
                    if(y_offset>0)
                    {
                        line_num = param_ptr->rect.bottom-param_ptr->rect.top+1;
                    }
                    else
                    {
                        line_num = param_ptr->rect.bottom-blk_info_ptr->pos.y+1;
                    }
                }
                else
                {
                    if(y_offset>0)
                    {
                        line_num = blk_info_ptr->pos.y+blk_info_ptr->height - y_offset;
                    }
                    else
                    {
                        line_num = blk_info_ptr->height;
                    }
                }
            }
            dst_mem_size = s->menu_buffer_info[lcd_id][layer_num].size;
            blk_size = (line_num*blk_info_ptr->width)<<2;
            if(dst_mem_size>=blk_size)
            {
                SCI_MEMCPY((void*)dst_copy_ptr,(void*)src_ptr,blk_size);
            }
            else
            {
                //SCI_TRACE_LOW:"dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1344_112_2_18_1_45_3_64,(uint8*)"ddd",layer_num,dst_mem_size,blk_size);
                ret = 1;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[lcd_ctrl_common.c]:OSD copy don't support!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1350_112_2_18_1_45_3_65,(uint8*)"");
            ret = 1;
        }

    }
    return ret;
}
#ifdef LCD_INSTALL_ANGLE_90
uint32 driver_rotation=0;
extern void GC9A01_Init_driver_rotation(void);
extern void GC9A01_Init_soft_rotation(void);
#endif
#ifndef  WIN32
PUBLIC void _invalidate_on_lcd( LCD_ID_E    lcd_id,
                                     DISPLAY_LAYER_INFO_T *layer_arr[],
                                     uint32 layer_num,
                                     DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    DECLARE_LCD_CTRL_INFO(s);
    uint32 blk_id;
    uint16 *src_ptr = PNULL;
    uint16 *dst_ptr = PNULL;
    BLOCKCFG_T cfg={0};
    uint16 logic_width = s->logic_width[lcd_id];
    uint16 logic_height = s->logic_height[lcd_id];
    LCD_BRUSH_MODE_E brush_mode = LCD_BRUSH_MODE_SYNC;
    uint32 i=0;
    uint32 dst_mem_size = 0;
    DISPLAY_LAYER_INFO_T *blk_info_ptr = 0;
    uint32 logic_size = (logic_width*logic_height)<<1;
    uint32 blk_size = 0;
    BOOLEAN is_handle_flag = FALSE;
    LCD_BUF_INFO_T *dst_buf_info_ptr = PNULL;
    LCD_INFO_T lcd_info = {0};
    DISPLAY_RECT_T temp_rect = {0};
    LCD_GetLcdCtrlSemaphore(lcd_id);
    _clear_ctrl_event();
    LCD_GetInfo(lcd_id, &lcd_info);

    brush_mode = LCD_GetBrushMode( lcd_id);
    #ifdef LCD_INSTALL_ANGLE_90
    param_ptr->rot_angle = LCD_ANGLE_90;
    param_ptr->rect.left = 0;
    param_ptr->rect.right = 239;
    param_ptr->rect.top = 0;
    param_ptr->rect.bottom = 239;
     if(driver_rotation==1)
     {
          GC9A01_Init_soft_rotation();
     }
    #endif
    //DISPLAY_TRACE:"param_ptr->rot_angle=%d,size=%d,layer_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1383_112_2_18_1_45_3_66,(uint8*)"ddd",param_ptr->rot_angle,logic_size,layer_num);

    if((layer_num>1) && (param_ptr->rot_angle != LCD_ANGLE_0))
    {
        if( 0 != _blend_and_rotate(lcd_id,&layer_arr[0],layer_num,param_ptr))
        {
            LCD_PutLcdCtrlSemaphore(lcd_id);
            //SCI_TRACE_LOW:"[_invalidate_on_nand:ERROR],_blend_and_rotate error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1437_112_2_18_1_45_3_71,(uint8*)"");
            return;
        }
    }
    else
    {
        for( i=0 ; i<layer_num ; i++ )
        {
            blk_info_ptr = layer_arr[i];
            SCI_PASSERT((blk_info_ptr!=PNULL),("layer is NULL layer_id = %d,array_size=%d",i,layer_num));/*assert verified*/

            if(param_ptr->rot_angle != LCD_ANGLE_0 && 0 != s->menu_buffer_info[lcd_id][i].addr)
            {
                src_ptr = (uint16*)blk_info_ptr->buf_addr;

                dst_ptr = (uint16*)s->menu_buffer_info[lcd_id][i].addr;
                dst_mem_size = s->menu_buffer_info[lcd_id][i].size;
                dst_buf_info_ptr = &s->menu_buffer_info[lcd_id][i];

                SCI_PASSERT((dst_ptr!=PNULL),("lcd_id = %d,i=%d",lcd_id,i));   /*assert verified*/
                SCI_PASSERT((dst_mem_size>=logic_size),("dst memory is small!"));/*assert verified*/
                //DISPLAY_TRACE:"dst_ptr=0x%x,src_ptr=0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1458_112_2_18_1_45_3_72,(uint8*)"dd",(uint32)dst_ptr,(uint32)src_ptr);
                if(ERR_DISPLAY_NONE != _display_software_rotation(lcd_id,dst_buf_info_ptr,src_ptr,blk_info_ptr,param_ptr))
                {
                    LCD_PutLcdCtrlSemaphore(lcd_id);
                    //SCI_TRACE_LOW:"_display_software_rotation:[ERROR]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1463_112_2_18_1_45_3_73,(uint8*)"");
                    return;
                }
                //brush_mode = LCD_BRUSH_MODE_ASYNC;
                blk_info_ptr->buf_addr = (uint32)dst_ptr;
                #ifdef LCD_INSTALL_ANGLE_90
                temp_rect.left = param_ptr->rect.left;
                temp_rect.top =  param_ptr->rect.top;
                temp_rect.right = param_ptr->rect.right;
                temp_rect.bottom = param_ptr->rect.bottom;
                param_ptr->rect.left =lcd_info.lcd_width-1-temp_rect.bottom;
                param_ptr->rect.top = temp_rect.left;
                param_ptr->rect.right =lcd_info.lcd_height-1-temp_rect.top;
                param_ptr->rect.bottom = temp_rect.right;
                #endif

            }
            else//not rotation
            {
                dst_ptr = (uint16*)s->menu_buffer_info[lcd_id][i].addr;

                if(s->op_mode[lcd_id] == OSD_OPERATION || s->op_mode[lcd_id] == OSD_OPERATION_SPECIAL)
                {
                    if(dst_ptr != 0)
                    {
                        if(0 != _osd_copy(lcd_id,i,blk_info_ptr,param_ptr))
                        {
                            //SCI_TRACE_LOW:"dst_mem samll,i=%d,dst_mem_size=%d,blk_size=%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1481_112_2_18_1_45_3_74,(uint8*)"ddd",i,dst_mem_size,blk_size);
                            LCD_PutLcdCtrlSemaphore(lcd_id);
                            return;
                        }
                        blk_info_ptr->buf_addr = (uint32)dst_ptr;
                        //brush_mode = LCD_BRUSH_MODE_ASYNC;
                    }
                    else
                    {
                        brush_mode = LCD_BRUSH_MODE_SYNC;
                        is_handle_flag = TRUE;
                    }
                }
                else
                {
                    brush_mode = LCD_BRUSH_MODE_SYNC;
                }
            }

            blk_id = blk_info_ptr->layer_id;
            #ifdef LCD_INSTALL_ANGLE_90
            driver_rotation = 0;
            #endif
            _convert_cfg(blk_info_ptr,&cfg);

            if(0 == s->menu_buffer_info[lcd_id][i].addr)
                cfg.rotation = param_ptr->rot_angle;

            if (ERR_LCD_NONE != LCD_ConfigBlock(lcd_id, blk_id,cfg))
            {
                LCD_PutLcdCtrlSemaphore(lcd_id);
                return;
            }

            LCD_SetBlockBuffer(lcd_id, blk_id, (uint32*)blk_info_ptr->buf_addr);

            if((i==0)&&
                ((DATA_TYPE_YUV422 == blk_info_ptr->layer_data.type)
                ||(DATA_TYPE_YUV420 == blk_info_ptr->layer_data.type)))
            {
                LCD_SetUVBuffer(lcd_id, blk_id, (uint32*)blk_info_ptr->uv_buf_addr);
            }

            LCD_EnableBlock(lcd_id, blk_id);
        }
    }
    LCD_SetBrushMode(lcd_id, brush_mode);

    if(s->limit_type [lcd_id] == PERMIT_GUI_INVALIDATE)
    {
        SCI_TRACE_LOW("[_invalidate_on_lcd:] limit_type =  PERMIT_GUI_INVALIDATE");
        LCD_InvalidateRect(lcd_id,
                           param_ptr->rect.left,
                           param_ptr->rect.top,
                           param_ptr->rect.right,
                           param_ptr->rect.bottom);
    }
   else
   {
       SCI_TRACE_LOW("[_invalidate_on_lcd:] limit_type !=  PERMIT_GUI_INVALIDATE");
       LCD_PutLcdCtrlSemaphore(lcd_id);
       if(is_handle_flag)
       {
           s->is_use_ctrl_event = TRUE;
           _get_ctrl_event(LCD_CTRL_EVENT,0);
           s->is_use_ctrl_event = FALSE;
       }
   }

   if(s->op_mode[lcd_id] == UI_OPERATION)
   {
        SCI_TRACE_LOW("[_invalidate_on_lcd:]  op_mode =  UI_OPERATION");

       LCD_DisableAllBlock(lcd_id);
   }
}

#endif

/**---------------------------------------------------------------------------*
 **                         public function define                            *
 **---------------------------------------------------------------------------*/

#ifdef WIN32
/******************************************************************************/
// Description: invalidate on simulator
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void _invalidate_on_simulator( LCD_ID_E  lcd_id,
                                            DISPLAY_LAYER_INFO_T *layer_arr[],
                                            uint32 array_size,
                                            DISPLAY_INVALIDATE_POS_T *param_ptr)
{
    DECLARE_LCD_CTRL_INFO(s);
    uint32 blk_id;
    uint16 *src_ptr = PNULL;
    uint16 *dst_ptr = PNULL;
    uint16 *temp_ptr = PNULL;
    BLOCKCFG_T cfg={0};
    uint16 logic_width = s->logic_width[lcd_id];
    uint16 logic_height = s->logic_height[lcd_id];
    LCD_BRUSH_MODE_E brush_mode = LCD_BRUSH_MODE_SYNC;
    int32 i=0 , j = 0;
    uint32 dst_mem_size = 0;
    DISPLAY_LAYER_INFO_T *blk_info_ptr = PNULL;
    uint32 size = (logic_width*logic_height)<<1;
    LCD_BUF_INFO_T *dst_buf_info_ptr = PNULL;

    //SCI_TRACE_LOW:"param_ptr->rot_angle=%d,size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1616_112_2_18_1_45_3_75,(uint8*)"dd",param_ptr->rot_angle,size);
    for( i=0 ; i<array_size ; i++ )
    {
        blk_info_ptr = layer_arr[i];
        blk_id = blk_info_ptr->layer_id;
        _convert_cfg(blk_info_ptr,&cfg);
        LCD_ConfigBlock(lcd_id, blk_id,cfg);
        if(param_ptr->rot_angle != LCD_ANGLE_0)
        {
            src_ptr = (uint16*)blk_info_ptr->buf_addr;

            dst_ptr = (uint16*)s->menu_buffer_info[lcd_id][i].addr;
            dst_mem_size = s->menu_buffer_info[lcd_id][i].size;
            dst_buf_info_ptr = &s->menu_buffer_info[lcd_id][i];

            SCI_PASSERT((dst_ptr!=PNULL),("lcd_id = %d,lcd_id"));   /*assert verified*/
            SCI_PASSERT((dst_mem_size>=size),("dst memory is small!"));/*assert verified*/
            //SCI_TRACE_LOW:"dst_ptr=0x%x,src_ptr=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1630_112_2_18_1_45_3_76,(uint8*)"dd",(uint32)dst_ptr,(uint32)src_ptr);
            //_software_rotation(lcd_id,dst_ptr,src_ptr,param_ptr->rot_angle);
            _software_rotation_with_crop(lcd_id,dst_buf_info_ptr,src_ptr,blk_info_ptr,param_ptr);
         }
         else//not rotation
         {
            dst_ptr = (uint16*)blk_info_ptr->buf_addr;
         }
        LCD_SetBlockBuffer(lcd_id, blk_id, (uint32)dst_ptr);
        LCD_EnableBlock(lcd_id, blk_id);

        if(blk_info_ptr == PNULL)
        {
            //SCI_TRACE_LOW:"_invalidate_on_nand: break,layer_num=%d,i=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1647_112_2_18_1_45_4_78,(uint8*)"dd",array_size,i);
            break;
        }
    }
    LCD_SetBrushMode(lcd_id, brush_mode);
    if(s->limit_type [lcd_id] == PERMIT_GUI_INVALIDATE)
    {
        LCD_InvalidateRect(lcd_id, \
                           param_ptr->rect.left, \
                           param_ptr->rect.top, \
                           param_ptr->rect.right, \
                           param_ptr->rect.bottom);
        if(brush_mode == LCD_BRUSH_MODE_SYNC)
        {
            LCD_PutLcdCtrlSemaphore(lcd_id);
        }
   }
}
#endif//WIN32
/******************************************************************************/
// Description: check parameter
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC ERR_DISPLAY_E _check_convert_param(LCD_ID_E lcd_id,
                                             DISPLAY_CONVERT_INPUT_T *input_ptr)
{
    DECLARE_LCD_CTRL_INFO(s);
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    uint16 logic_width = 0;
    uint16 logic_height = 0;
    LCD_INFO_T lcd_info = {0};
    DISPLAY_ANGLE_E logic_angle = DISPLAY_ANGLE_0;
    LCD_PLACED_FASHION_E lcd_placed_fashion = s->lcd_direction[lcd_id];

    ret = (ERR_DISPLAY_E)LCD_GetInfo(lcd_id,&lcd_info);
    if(ret != ERR_DISPLAY_NONE)
    {
        return ret;
    }

    logic_angle = input_ptr->logic_angle;
    logic_width = input_ptr->logic_lcd_width;
    logic_height = input_ptr->logic_lcd_height;

    if(lcd_placed_fashion == LCD_PLACED_VERTICAL)
    {
        switch(logic_angle)
        {
            case DISPLAY_ANGLE_0:
            case DISPLAY_ANGLE_180:
                if((logic_width>lcd_info.lcd_width) || (logic_height>lcd_info.lcd_height))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                }
            break;
            case DISPLAY_ANGLE_90:
            case DISPLAY_ANGLE_270:
                if((logic_width>lcd_info.lcd_height) || (logic_height>lcd_info.lcd_width))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                }
            break;
            default:
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            break;
        }
   }
   else
   {
        switch(logic_angle)
        {
             case DISPLAY_ANGLE_90:
             case DISPLAY_ANGLE_270:
                if((logic_width>lcd_info.lcd_width) || (logic_height>lcd_info.lcd_height))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                }
            break;
            case DISPLAY_ANGLE_0:
            case DISPLAY_ANGLE_180:
                if((logic_width>lcd_info.lcd_height) || (logic_height>lcd_info.lcd_width))
                {
                    ret = ERR_DISPLAY_PARAMETER_WRONG;
                }
            break;
            default:
            ret = ERR_DISPLAY_PARAMETER_WRONG;
            break;
        }
    }

    return ret;
}
/*lint -save -e737 */
/******************************************************************************/
// Description: convert configuration
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void _convert_cfg(DISPLAY_LAYER_INFO_T *blk_ptr, BLOCKCFG_T *cfg_ptr)
{
    uint8 data_resolution = 0;
    uint32 addr = blk_ptr->buf_addr;
    uint32 byte_num = 0;
    DISPLAY_RECT_T *clip_rect = &blk_ptr->clip_rect;

    switch(blk_ptr->layer_data.type)
    {
        case DATA_TYPE_ARGB888:
        case DATA_TYPE_PMARGB888:
            data_resolution = 0;
            byte_num = 4;
            break;
        case DATA_TYPE_ARGB666:
        case DATA_TYPE_PMARGB666:
            data_resolution = 1;
            byte_num = 4;
            break;
        case DATA_TYPE_ARGB565:
        case DATA_TYPE_RGB565:
        case DATA_TYPE_PMARGB565:
            data_resolution = 2;
            byte_num = 2;
            break;
        case DATA_TYPE_ARGB555:
            data_resolution = 3;
            byte_num = 2;
            break;
        case DATA_TYPE_GREY:
            data_resolution = 4;
            byte_num = 1;
            break;
        case DATA_TYPE_YUV422:
            data_resolution = 5;
            byte_num = 1;
            break;
        case DATA_TYPE_YUV420:
            data_resolution = 6;
            byte_num = 1;
            break;
        case DATA_TYPE_YUV400:
            data_resolution = 7;
            byte_num = 1;
            break;
        default:
            //SCI_TRACE_LOW:"[_convert_cfg ERROR],display data type is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1786_112_2_18_1_45_4_79,(uint8*)"");
            SCI_MEMSET(blk_ptr,0,sizeof(DISPLAY_LAYER_INFO_T)); /*lint !e516 -e516*/
            break;
    }
    if (byte_num == 1) // base pointer must be aligned by words
    {
        clip_rect->left &= ~3;
    }
    else if (byte_num == 2)
    {
        clip_rect->left &= ~1;
    }

    if(DATA_TYPE_YUV422 == blk_ptr->layer_data.type)
    {

        if(blk_ptr->pos.x<0)
        {
            cfg_ptr->start_x = 0;
            addr += (-blk_ptr->pos.x + clip_rect->left); /*lint -e737*/
            blk_ptr->uv_buf_addr += (-blk_ptr->pos.x + clip_rect->left); /*lint -e737*/
        }
        else
        {
            cfg_ptr->start_x = blk_ptr->pos.x + clip_rect->left;
            addr += clip_rect->left; /*lint -e737*/
            blk_ptr->uv_buf_addr += clip_rect->left; /*lint -e737*/
        }
        if(blk_ptr->pos.y<0)
        {
            cfg_ptr->start_y = 0;
            addr += byte_num * blk_ptr->width *(-blk_ptr->pos.y + clip_rect->top);    /*lint -e737*/
            blk_ptr->uv_buf_addr += (-blk_ptr->pos.y + clip_rect->top) * blk_ptr->width;/*lint -e737*/
        }
        else
        {
            cfg_ptr->start_y = blk_ptr->pos.y + clip_rect->top;
            addr += byte_num * blk_ptr->width * clip_rect->top;    /*lint -e737*/
            blk_ptr->uv_buf_addr += clip_rect->top * blk_ptr->width;/*lint -e737*/
        }
    }
    else if(DATA_TYPE_YUV420 == blk_ptr->layer_data.type)
    {
        if(blk_ptr->pos.x<0)
        {
            cfg_ptr->start_x = 0;
            addr += (-blk_ptr->pos.x + clip_rect->left); /*lint -e737*/
            blk_ptr->uv_buf_addr += (-blk_ptr->pos.x + clip_rect->left); /*lint -e737*/
        }
        else
        {
            cfg_ptr->start_x = blk_ptr->pos.x + clip_rect->left;
            addr += clip_rect->left; /*lint -e737*/
            blk_ptr->uv_buf_addr += clip_rect->left; /*lint -e737*/
        }
        if(blk_ptr->pos.y<0)
        {
            cfg_ptr->start_y = 0;
            addr += byte_num * blk_ptr->width * (-blk_ptr->pos.y + clip_rect->top);    /*lint -e737*/
            blk_ptr->uv_buf_addr += (-blk_ptr->pos.y + clip_rect->top)*blk_ptr->width/2;/*lint -e737*/
        }
        else
        {
            cfg_ptr->start_y = blk_ptr->pos.y + clip_rect->top;
            addr += byte_num * blk_ptr->width * clip_rect->top;    /*lint -e737*/
            blk_ptr->uv_buf_addr += clip_rect->top * blk_ptr->width/2;/*lint -e737*/
        }
    }
    else
    {
        if(blk_ptr->pos.x<0)
        {
            cfg_ptr->start_x = 0;
            addr += byte_num * (-blk_ptr->pos.x + clip_rect->left);/*lint -e737*/
        }
        else
        {
            cfg_ptr->start_x = blk_ptr->pos.x + clip_rect->left;
            addr += byte_num * clip_rect->left;/*lint -e737*/
        }
        if(blk_ptr->pos.y<0)
        {
            cfg_ptr->start_y = 0;
            addr += byte_num * (-blk_ptr->pos.y + clip_rect->top) * blk_ptr->width; /*lint -e737*/
        }
        else
        {
            cfg_ptr->start_y = blk_ptr->pos.y + clip_rect->top;
            addr += byte_num * clip_rect->top * blk_ptr->width; /*lint -e737*/
        }
    }
    cfg_ptr->end_x   = blk_ptr->pos.x + clip_rect->right - clip_rect->left;
    cfg_ptr->end_y   = blk_ptr->pos.y + clip_rect->bottom - clip_rect->top;
    cfg_ptr->width   = blk_ptr->width;

    cfg_ptr->resolution = data_resolution;
    cfg_ptr->type     = 1;
    cfg_ptr->priority = blk_ptr->layer_id;
    cfg_ptr->colorkey_en = blk_ptr->layer_data.colorkey_en;
    cfg_ptr->colorkey    = (uint16)blk_ptr->layer_data.colorkey;
    cfg_ptr->alpha     = (uint8)blk_ptr->layer_data.alpha;
    cfg_ptr->grey_rgb = blk_ptr->layer_data.grey_value;
    blk_ptr->buf_addr = addr;

    if(DATA_TYPE_PMARGB888 == blk_ptr->layer_data.type
        ||DATA_TYPE_PMARGB666 == blk_ptr->layer_data.type
        ||DATA_TYPE_PMARGB565 == blk_ptr->layer_data.type )
    {
        cfg_ptr->blend_mode = LCDC_OSD_BLEND_SPECIAL;
        cfg_ptr->alpha_sel = DISPLAY_PIXEL_ALPHA;
    }
    else
    {
        cfg_ptr->blend_mode = LCDC_OSD_BLEND_NORMAL;
        cfg_ptr->alpha_sel = (uint8)blk_ptr->layer_data.alpha_sel;
    }
}
/******************************************************************************/
// Description: get byte number for data type
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC uint32 _get_byte_num(DISPLAY_DATA_TYPE_E data_format)
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
            bytes_num = 4;
            break;
        case DATA_TYPE_RGB565:
        case DATA_TYPE_ARGB555:
            bytes_num = 2;
            break;
            default:
            //SCI_TRACE_LOW:"covert_to_byte_num:result is 0,data_format is %d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1906_112_2_18_1_45_4_80,(uint8*)"d",data_format);
                break;
    }
    return bytes_num;
}
/******************************************************************************/
// Description: data format convert when captrue mode
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC LCD_DATA_FORMAT_E _cap_data_format_convert(DISPLAY_DATA_TYPE_E type)
{
    LCD_DATA_FORMAT_E ret = LCD_RGB565;
    switch(type)
    {
        case DATA_TYPE_ARGB888:
            ret = LCD_RGB888;
            break;
        case DATA_TYPE_ARGB666:
            ret = LCD_RGB666;
            break;
        case DATA_TYPE_RGB565:
            ret= LCD_RGB565;
            break;
        default:
            //SCI_TRACE_LOW:"_cap_data_type_convert:default rgb565,type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1928_112_2_18_1_45_4_81,(uint8*)"d",type);
            ret = LCD_RGB565;
            break;
    }
    return ret;
}
/******************************************************************************/
// Description: invalidate on nand platform
// Author: Jianping.Wang
/******************************************************************************/
#if 0
PUBLIC void _invalidate_on_lcd( LCD_ID_E    lcd_id,
                                     DISPLAY_LAYER_INFO_T *layer_arr[],
                                     uint32 layer_num,
                                     DISPLAY_INVALIDATE_POS_T *param_ptr)
{
#if defined(NANDBOOT_SUPPORT) || defined(PLATFORM_SC6800H)
    //SCI_TRACE_LOW:"_invalidate_on_nand:%d,%d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1943_112_2_18_1_45_4_82,(uint8*)"ddd",lcd_id,layer_num,layer_arr[0]->buf_addr);
    _invalidate_on_nand(lcd_id,&layer_arr[0], layer_num,param_ptr);
#else
    //SCI_TRACE_LOW:"_invalidate_on_nor:%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_CTRL_COMMON_1946_112_2_18_1_45_4_83,(uint8*)"dd",lcd_id,layer_num);
    _invalidate_on_nor(lcd_id,&layer_arr[0], layer_num,param_ptr);
    LCD_PutLcdCtrlSemaphore(lcd_id);
#endif//end of NANDBOOT_SUPPORT
}
#endif
/******************************************************************************/
// Description:
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void _initial_lcd_memory(LCD_ID_E lcd_id)
{
#ifndef WIN32
    DECLARE_LCD_CTRL_INFO(s);
    uint32 layer_num = 0;
    uint32 layer_mem_addr = 0;
    uint32 layer_mem_size = 0;
    uint32 i = 0;
    LCD_INFO_T lcd_info = {0};

    LCD_GetInfo(lcd_id, &lcd_info);

    layer_num      = DISPLAYCFG_GetLayerNum(lcd_id);
    layer_mem_addr = DISPLAYCFG_GetMemAddr(lcd_id);

    if(LCD_ID_0 == lcd_id)
    {
        s->layer_num[0] = layer_num;
    }
    else
    {
        s->layer_num[1] = layer_num;
    }

    if((0 == layer_num) || (0 == layer_mem_addr))
    {
        return;
    }

    do
    {
        layer_mem_addr += layer_mem_size;
        layer_mem_size = DISPLAYCFG_GetMemSize(lcd_id,i);
        if(lcd_id == LCD_ID_0)
        {
            s->menu_buffer_info[0][i].addr = (uint32)layer_mem_addr;
            s->menu_buffer_info[0][i].size = layer_mem_size;
            s->menu_buffer_info[0][i].width  = lcd_info.lcd_width;
            s->menu_buffer_info[0][i].height = lcd_info.lcd_height;
        }
        else
        {
            s->menu_buffer_info[1][i].addr = (uint32)layer_mem_addr;
            s->menu_buffer_info[1][i].size = layer_mem_size;
            s->menu_buffer_info[1][i].width  = lcd_info.lcd_width;
            s->menu_buffer_info[1][i].height = lcd_info.lcd_height;
        }
        i++;
    }while(i<layer_num);

#endif

}
/******************************************************************************/
// Description: set event
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void _set_ctrl_event(uint32 set_flags)
{
    DECLARE_LCD_CTRL_INFO(s);

    if(PNULL!=s->lcd_ctrl_event)
    {
        SCI_SetEvent(s->lcd_ctrl_event, set_flags, SCI_OR);
    }
}
/******************************************************************************/
// Description: get information of display control
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC LCD_DISPLAY_CTRL_INFO_T *_get_ctrl_info(void)
{
#ifdef WIN32
    if (!s_lcd_ctrl_info.is_init)
    {
        _display_init();
    }
#endif
    return &s_lcd_ctrl_info;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void _display_init(void)
{
    DECLARE_LCD_CTRL_INFO(s);
    LCD_INFO_T lcd_info = {0};
    uint32 i = 0;
    uint32 lcd_num = 0;

    if(s->is_init)
    {
        return;
    }
#ifndef WIN32
    lcd_num = LCD_GetUsedNum();
#else
    lcd_num = 2;
#endif
    SCI_PASSERT(((lcd_num!=0)&&(lcd_num<=2)),("LCD num is less than 3!,lcd_num = %d", lcd_num));/*assert verified*/
    SCI_MEMSET(s,0,sizeof(LCD_DISPLAY_CTRL_INFO_T));

    s->lcd_ctrl_sm_ptr[0] = SCI_CreateSemaphore("LCD DISPLAY CTRL SEMAPHORE", 1);
    SCI_PASSERT((s->lcd_ctrl_sm_ptr[0] !=PNULL),("create lcd ctrl semaphore fail!"));/*assert verified*/

    if (NULL == s->lcd_ctrl_event)
    {
        s->lcd_ctrl_event = SCI_CreateEvent("lcd control event");
    }

    for( i=0 ; i<lcd_num ; i++)
    {
         LCD_GetInfo(i,&lcd_info);
         if(lcd_info.lcd_width>lcd_info.lcd_height)
         {
             s->lcd_direction[i] = LCD_PLACED_HORIZONTAL;
         }
         else
         {
             s->lcd_direction[i] = LCD_PLACED_VERTICAL;
         }
         s->limit_type[i] = PERMIT_GUI_INVALIDATE;
         s->physical_width[i] = lcd_info.lcd_width;
         s->physical_height[i] = lcd_info.lcd_height;
         s->is_used = SCI_FALSE;
         s->full_copy_flag[i] = 0;

#ifndef WIN32
         _initial_lcd_memory(i == 0 ? LCD_ID_0 : LCD_ID_1);
#endif

         s->op_mode[i]=UI_OPERATION;
         s->is_copy_osd[i] = SCI_TRUE;
         s->previous_mode[i] = DISPLAY_OP_MAX;
    }
    s->is_use_ctrl_event = FALSE;

#ifndef WIN32
    LCD_RegCallback(LCD_PutLcdCtrlSemaphore);
#endif
    s->is_init = TRUE;
}

/*lint -restore */
/**----------------------------------------------------------------------------*
 **                         Compiler Flag                                      *
 **----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

