/*****************************************************************************
** File Name:      guilcd.c                                                  *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Create
******************************************************************************/

#ifndef _GUILCD_C_
#define _GUILCD_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guilcd.h"
#include "guilcd_internal.h"
#include "guiblock.h"
#include "guiblock_internal.h"
#include "guiref_scale.h"
#include "mmi_theme.h"
#include "dal_display.h"
#include "ui_layer.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL LCD_ANGLE_E   s_init_logic_angle[MAX_LCD_ID] = {0};   //init logic angle
LOCAL LCD_ANGLE_E   s_logic_angle[MAX_LCD_ID] = {0};        //logic angle

//刷屏模式
LOCAL GUILCD_INVALIDATE_MODE_E s_guilcd_invalidate_mode = GUILCD_INVALIDATE_MODE_NORMAL;

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
// Description : init lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL void InitLcdLogicAngle(
                             LCD_ID_E   lcd_id
                             );

/*****************************************************************************/
// Description : set lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL void SetLogicAngle(
                         LCD_ID_E       lcd_id,
                         LCD_ANGLE_E    logic_angle
                         );

/*****************************************************************************/
// Description : Invalidate lcd rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN InvalidateLcdRect(
                                BOOLEAN     is_only_multiblend, //in:only multi blend
                                BOOLEAN     is_only_invalidate, //in:only invalidate,not handle multi layer
                                LCD_ID_E    lcd_id,             //in:
                                GUI_RECT_T  rect,               //in:
                                uint32      update_block_set    //in:for tv out
                                );

/*****************************************************************************/
// Description : check invalidate rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckInvalidateRect(
                                  GUI_RECT_T    *rect_ptr   //in/out
                                  );

/*****************************************************************************/
// Description : get block info for invalidate lcd
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL uint32 GetInvalidateBlockInfo(
                                    uint32                  max_block_num,      //in:max block number
                                    LCD_ID_E                lcd_id,             //in:
                                    DISPLAY_LAYER_INFO_T    *lcd_block_ptr,     //in/out:
                                    DISPLAY_CONVERT_INPUT_T *convert_input_ptr  //in
                                    );

/*****************************************************************************/
// Description : get rotate angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL LCD_ANGLE_E GetRotateAngle(
                                 LCD_ID_E       lcd_id,     //in:
                                 LCD_ANGLE_E    logic_angle //in:
                                 );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : init lcd
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_Init(
                           LCD_ID_E     lcd_id
                           )
{
    BOOLEAN     result = FALSE;
    
    switch (lcd_id)
    {
    case GUI_MAIN_LCD_ID:
    case GUI_SUB_LCD_ID:
        //init block
        if (GUIBLOCK_Init(lcd_id))
        {
            //init logic angle
            InitLcdLogicAngle(lcd_id);

            result = TRUE;
        }
        break;

    default:
        //SCI_TRACE_LOW:"GUILCD_Init:init lcd %d is fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUILCD_128_112_2_18_3_19_38_203,(uint8*)"d",lcd_id);
        break;
    }

    return (result);
}

/*****************************************************************************/
// Description : init lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL void InitLcdLogicAngle(
                             LCD_ID_E   lcd_id
                             )
{
    LCD_ANGLE_E     logic_angle = LCD_ANGLE_0;
    switch (lcd_id)
    {
    case GUI_MAIN_LCD_ID:
        //get main lcd logic angel
#ifdef MAINLCD_LOGIC_ANGLE_90
        logic_angle = LCD_ANGLE_90;
#elif defined MAINLCD_LOGIC_ANGLE_180
        logic_angle = LCD_ANGLE_180;
#elif defined MAINLCD_LOGIC_ANGLE_270
        logic_angle = LCD_ANGLE_270;
#endif
        break;

    case GUI_SUB_LCD_ID:
        //get sub lcd logic angel
#ifdef SUBLCD_LOGIC_ANGLE_90
        logic_angle = LCD_ANGLE_90;
#elif defined SUBLCD_LOGIC_ANGLE_180
        logic_angle = LCD_ANGLE_180;
#elif defined SUBLCD_LOGIC_ANGLE_270
        logic_angle = LCD_ANGLE_270;
#endif
        break;

    default:
        //SCI_TRACE_LOW:"InitLcdLogicAngle:init lcd %d is fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUILCD_169_112_2_18_3_19_38_204,(uint8*)"d",lcd_id);
        break;
    }

    //set logic angle
    s_init_logic_angle[lcd_id] = logic_angle;
    GUILCD_SetLogicAngle(lcd_id,logic_angle);
}

/*****************************************************************************/
// Description : get lcd info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_GetInfo(
                              LCD_ID_E      lcd_id,
                              LCD_INFO_T    *lcd_info_ptr
                              )
{
    BOOLEAN     result = FALSE;

    if ((PNULL != lcd_info_ptr) &&
        (ERR_LCD_NONE == LCD_GetInfo(lcd_id,lcd_info_ptr)))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : get main lcd buffer
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T* GUILCD_GetMainLcdBufPtr(void)
{
    LCD_INFO_T      lcd_info = {0};
    GUI_COLOR_T     *buf_ptr = PNULL;

    //get lcd info
    //if (GUILCD_GetInfo(GUI_MAIN_LCD_ID,&lcd_info))
    if ( ERR_LCD_NONE == LCDAPI_GetInfo( GUI_MAIN_LCD_ID, &lcd_info ) )
    {
        buf_ptr = (GUI_COLOR_T *)lcd_info.lcdbuff_ptr;
    }

    return (buf_ptr);
}

/*****************************************************************************/
// Description : check lcd id,is tv
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC LCD_ID_E GUILCD_CheckLcdId(
                                  LCD_ID_E  lcd_id
                                  )
{
    if ((GUI_MAIN_LCD_ID == lcd_id) && (MMITHEME_IsTVOpen()))
    {
        lcd_id = GUI_TV_ID;
    }

    return (lcd_id);
}

/*****************************************************************************/
// Description : get lcd init angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC LCD_ANGLE_E GUILCD_GetInitAngle(
                                       LCD_ID_E     lcd_id
                                       )
{
    return (s_init_logic_angle[lcd_id]);
}

/*****************************************************************************/
// Description : get lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC LCD_ANGLE_E  GUILCD_GetLogicAngle(
                                         LCD_ID_E   lcd_id
                                         )
{
    return (s_logic_angle[lcd_id]);
}

/*****************************************************************************/
// Description : set lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void GUILCD_SetLogicAngle(
                                 LCD_ID_E       lcd_id,
                                 LCD_ANGLE_E    logic_angle
                                 )
{
    //set logic angle
    SetLogicAngle(lcd_id,logic_angle);
}

/*****************************************************************************/
// Description : set lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL void SetLogicAngle(
                         LCD_ID_E       lcd_id,
                         LCD_ANGLE_E    logic_angle
                         )
{
    GUI_LCD_DEV_INFO    lcd_dev = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    
    if ((s_logic_angle[lcd_id] != logic_angle) &&
        (LCD_ANGLE_0 <= logic_angle) &&
        (LCD_ANGLE_270 >= logic_angle))/*lint !e685*/
    {
        s_logic_angle[lcd_id] = logic_angle;

        //modify block
        GUIBLOCK_Rotation(lcd_id);

        //modify layer
        lcd_dev.lcd_id = lcd_id;
#ifdef UI_MULTILAYER_SUPPORT 
        UILAYER_Rotation(&lcd_dev);
#endif        
        //switch resource
        MMITHEME_SwitchStaticThemeRes();
    }
}

/*****************************************************************************/
// Description : Invalidate lcd rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_InvalidateRect(
                                     LCD_ID_E       lcd_id,             //in:
                                     GUI_RECT_T     rect,               //in:
                                     uint32         update_block_set    //in:for tv out
                                     )
{
    BOOLEAN     result = FALSE;

    result = InvalidateLcdRect(FALSE, FALSE,lcd_id,rect,update_block_set);

    return (result);
}

/*****************************************************************************/
// Description : multiblend lcd rect
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_MultiBlendRect(
                                     LCD_ID_E       lcd_id,             //in:
                                     GUI_RECT_T     rect,               //in:
                                     uint32         update_block_set    //in:for tv out
                                     )
{
    BOOLEAN     result = FALSE;

    result = InvalidateLcdRect(TRUE, FALSE,lcd_id,rect,update_block_set);

    return (result);
}


/*****************************************************************************/
// Description : set invalidate mode
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_SetInvalidateMode(
                                        GUILCD_INVALIDATE_MODE_E mode
                                        )
{
    SCI_TRACE_LOW("guilcd.c GUILCD_SetInvalidateMode() mode = %d, s_guilcd_invalidate_mode = %d", mode, s_guilcd_invalidate_mode);
    s_guilcd_invalidate_mode = mode;

    return TRUE;
}

/*****************************************************************************/
// Description : Invalidate lcd rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN InvalidateLcdRect(
                                BOOLEAN     is_only_multiblend, //in:only multi blend
                                BOOLEAN     is_only_invalidate, //in:only invalidate,not handle multi layer
                                LCD_ID_E    lcd_id,             //in:
                                GUI_RECT_T  rect,               //in:
                                uint32      update_block_set    //in:for tv out
                                )
{
    BOOLEAN                     result = FALSE;
    uint32                      i = 0;
    uint32                      array_size = 0;
    GUI_RECT_T                  tv_rect = {0};
    GUI_RECT_T                  update_rect = {0};
    GUI_RECT_T                  lcd_rect = {0};
    DISPLAY_LAYER_INFO_T        block_info_array[GUI_BLOCK_MAX] = {0};
    DISPLAY_CONVERT_INPUT_T     convert_input = {0};
    DISPLAY_CONVERT_OUTPUT_T    convert_output = {0};
    DISPLAY_LAYER_INFO_T        *layer_arr[GUI_BLOCK_MAX]={0};
    DISPLAY_INVALIDATE_POS_T    pos_param = {0};
    UILAYER_BLEND_T             ui_layer_blend = {0};
    DISPLAY_BLENDLAYER_PARM_T   blend_param = {0};
    DISPLAY_INVALIDATE_PARM_T   invalidate_param = {0};
    DISPLAY_BLENDLAYER_PARM_T*  blend_param_ptr = NULL;
    DISPLAY_BLEND_LAYER_T*      src_layer[UILAYER_TOTAL_BLT_COUNT] = {0};

    //check invalidate rect
    if (CheckInvalidateRect(&rect))
    {
        //set logic lcd width and height
        GUILCD_GetLogicWidthHeight(lcd_id,
            &convert_input.logic_lcd_width,
            &convert_input.logic_lcd_height);

        lcd_rect.right = convert_input.logic_lcd_width-1;
        lcd_rect.bottom = convert_input.logic_lcd_height-1;

        //adjust rect
        rect.left   = (uint16)MAX(0,rect.left);
        rect.top    = (uint16)MAX(0,rect.top);
        rect.right  = (uint16)MIN((convert_input.logic_lcd_width-1),rect.right);
        rect.bottom = (uint16)MIN((convert_input.logic_lcd_height-1),rect.bottom);

        if (rect.right < rect.left || rect.bottom < rect.top)
        {
            return result;
        }

        if ((MMITHEME_IsTVOpen()) &&
            ((GUI_MAIN_LCD_ID == lcd_id) || (GUI_TV_ID == lcd_id)))
        {
            //tv out scaling down function
            MMITHEME_GetTVOutShowRect(&tv_rect);
        
            update_rect.left   = (int16)(rect.left + tv_rect.left);
            update_rect.top    = (int16)(rect.top + tv_rect.top);
            update_rect.right  = (int16)(rect.right + tv_rect.left);
            update_rect.bottom = (int16)(rect.bottom + tv_rect.top);
            GUIREF_TvScalingDown(update_block_set,&update_rect);
        }

        //set logic rect
        convert_input.logic_coordinate.left   = rect.left;
        convert_input.logic_coordinate.top    = rect.top;
        convert_input.logic_coordinate.right  = rect.right;
        convert_input.logic_coordinate.bottom = rect.bottom;

        //set logic angle
        convert_input.logic_angle = (DISPLAY_ANGLE_E)s_logic_angle[lcd_id];

        //convert logic rect and angle to physical rect and angle
        DISPLAY_ConvertCoordinate(lcd_id,
            &convert_input,
            &convert_output);

        //set rect and angle
        pos_param.rect      = convert_output.physical_coordinate;
        pos_param.rot_angle = convert_output.rotation_angle;

        //get block info
        array_size = GetInvalidateBlockInfo(GUI_BLOCK_MAX,
            lcd_id,
            block_info_array,
            &convert_input);

        //set block 
        for(i=0; i<array_size; i++)
        {
            if(block_info_array[i].is_enable)
            {
                layer_arr[i] = &block_info_array[i];
            }
        }

        invalidate_param.layer_arr = layer_arr;
        invalidate_param.layer_num = array_size;
        invalidate_param.param_ptr = &pos_param;

#ifdef UI_MULTILAYER_SUPPORT
        if (!is_only_invalidate)
        {
            //multi layer
            if (UILAYER_GetBlendInfo(lcd_id, &rect, &ui_layer_blend))
            {
                blend_param.dst_layer_ptr = &ui_layer_blend.dst_layer;
                blend_param.layer_arr = src_layer;
                blend_param.layer_num = ui_layer_blend.layer_num;
                blend_param.rect_ptr = &(ui_layer_blend.rect);
                blend_param.callback = ui_layer_blend.callback;
                blend_param.param = ui_layer_blend.param;

                //底层需要的是指针数组，在此适配一下
                for ( i = 0; i < ui_layer_blend.layer_num; i++ )
                {
                    src_layer[i] = &ui_layer_blend.src_layer[i];
                }

                blend_param_ptr = &blend_param;
            }
            else
            {
                //SCI_TRACE_LOW:"InvalidateLcdRect.c UILAYER_GetBlendInfo failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUILCD_452_112_2_18_3_19_38_205,(uint8*)"");
//                UILAYER_InvalidateRect(lcd_id,&rect);
                blend_param_ptr = PNULL;
            }
        }
        else
        {
            UILAYER_GetInvalidateParam( lcd_id, &rect, &invalidate_param );
        }

        if (UILAYER_IsOsdLayerEnable())
        {
            if ((PNULL != blend_param_ptr) && (PNULL != blend_param_ptr->dst_layer_ptr) 
                && (PNULL != blend_param_ptr->dst_layer_ptr->layer_info.buf_addr) && (PNULL != invalidate_param.param_ptr))
            {
                //软件合成
                DISPLAY_BlendLayer(lcd_id, blend_param_ptr->dst_layer_ptr, blend_param_ptr->layer_arr, blend_param_ptr->layer_num, blend_param_ptr->rect_ptr);
                invalidate_param.layer_arr[0]->buf_addr = blend_param_ptr->dst_layer_ptr->layer_info.buf_addr;
                invalidate_param.layer_arr[0]->layer_data = blend_param_ptr->dst_layer_ptr->layer_info.layer_data;
                invalidate_param.layer_arr[0]->layer_id = GUIBLOCK_ConvertBlockId(lcd_id,0);
                DISPLAY_BlendAndInvalidateRect( lcd_id, PNULL, &invalidate_param );
            }
        }
        else
        {
            //小层合成大层
//            UILAYER_PreMergeLayer(&ui_layer_blend, PNULL);
            if (PNULL != blend_param_ptr)
            {
                if (blend_param_ptr->layer_num > UILAYER_BLEND_MAX_NUM)
                {
                    //超过4层分多次合成
//                UILAYER_SetDirectDraw( FALSE );
                    blend_param_ptr->dst_layer_ptr->layer_info.is_enable = TRUE;
                    UILAYER_MultiBlendLayer(lcd_id, blend_param_ptr);
                    //invalidate
                    if (!is_only_multiblend)
                    {
                        DISPLAY_BlendAndInvalidateRect( lcd_id, PNULL, &invalidate_param );
                    }
                }
                else
                {
                    //invalidate
                    if (is_only_multiblend)
                    {
                        DISPLAY_BlendLayer(lcd_id, blend_param_ptr->dst_layer_ptr, blend_param_ptr->layer_arr, blend_param_ptr->layer_num, blend_param_ptr->rect_ptr);
                    }
                    else
                    {
                        if (GUILCD_INVALIDATE_MODE_OSD == s_guilcd_invalidate_mode)
                        {    
                            SCI_TRACE_LOW("guilcd.c InvalidateLcdRect() GUILCD_INVALIDATE_MODE_OSD == s_guilcd_invalidate_mode");

                            if ((PNULL != blend_param_ptr->layer_arr[0]) && (PNULL != invalidate_param.layer_arr[0]))
                            {
                                SCI_TRACE_LOW("guilcd.c InvalidateLcdRect() layer_num = %d", invalidate_param.layer_num);
                                SCI_TRACE_LOW("layer_info.buf_addr = 0x%x", blend_param_ptr->layer_arr[0]->layer_info.buf_addr);
                                SCI_TRACE_LOW("buf_addr = 0x%x", invalidate_param.layer_arr[0]->buf_addr);
                                invalidate_param.layer_num = 1;
                                invalidate_param.layer_arr[0]->buf_addr = blend_param_ptr->layer_arr[0]->layer_info.buf_addr;
                                DISPLAY_BlendAndInvalidateRect( lcd_id, PNULL, &invalidate_param );
                            }
                            else
                            {
                                SCI_TRACE_LOW("blend_param_ptr->layer_arr[0] = 0x%x, invalidate_param.layer_arr[0] = 0x%x", blend_param_ptr->layer_arr[0], invalidate_param.layer_arr[0]);
                            }
                        }
                        else
                        {
                            DISPLAY_BlendAndInvalidateRect( lcd_id, blend_param_ptr, &invalidate_param );
#ifdef WIN32
                            //模拟器显示多图层图像
                            MSDev_DISPLAY_BlendLayer(lcd_id, blend_param_ptr->layer_arr, blend_param_ptr->layer_num, &invalidate_param.param_ptr->rect);
#endif
                        }
                    }                    
                }
            }
            else
            {
                DISPLAY_BlendAndInvalidateRect( lcd_id, PNULL, &invalidate_param );
            }
        }
//        DISPLAY_BlendAndInvalidateRect( lcd_id, blend_param_ptr, &invalidate_param );
#else
		DISPLAY_BlendAndInvalidateRect( lcd_id, PNULL, &invalidate_param );
#endif
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetDirectDraw( FALSE );
        UILAYER_SetMainLayerImmutable( FALSE );
#endif        
        //update lcd old api
        //DISPLAY_InvalidateRect(lcd_id,&layer_arr[0],array_size,&pos_param);

#if 0//james test for LCDAPI_GetInfo, force to destroy main lcd buffer
        {
            LCD_INFO_T lcd_info = {0};
            char* start;
            uint32 size;
            
            start = (char*)lcd_info.lcdbuff_ptr + rect.top * lcd_info.lcd_width * sizeof(GUI_COLOR_T);
            size = (rect.bottom - rect.top) * lcd_info.lcd_width * sizeof(GUI_COLOR_T);
            
            LCDAPI_GetInfo( lcd_id, &lcd_info );
            SCI_MEMSET( start, 0, size );
            
            SCI_Sleep(1);
            
            LCDAPI_GetInfo( lcd_id, &lcd_info );
            SCI_MEMSET( start, 0xFF, size );
        }
#endif
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
// Description : Invalidate lcd all rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_Invalidate(
                                 LCD_ID_E   lcd_id,             //in:
                                 uint32     update_block_set    //in:for tv out
                                 )
{
    BOOLEAN     result = FALSE;
    uint16      logic_lcd_width = 0;
    uint16      logic_lcd_height = 0;
    GUI_RECT_T  rect = {0};

    //get logic lcd width and height
    GUILCD_GetLogicWidthHeight(lcd_id,
        &logic_lcd_width,
        &logic_lcd_height);

    //set rect
    rect.right  = (int16)(logic_lcd_width - 1);
    rect.bottom = (int16)(logic_lcd_height - 1);

    //invalidate all lcd
    result = InvalidateLcdRect(FALSE, FALSE,lcd_id,rect,update_block_set);

    return (result);
}

/*****************************************************************************/
// Description : Invalidate lcd rect,not handle multi layer
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_InvalidateRectOnly(
                                         LCD_ID_E       lcd_id,             //in:
                                         GUI_RECT_T     rect,               //in:
                                         uint32         update_block_set    //in:for tv out
                                         )
{
    BOOLEAN     result = FALSE;

    //invalidate all lcd
    result = InvalidateLcdRect(FALSE, TRUE,lcd_id,rect,update_block_set);

    //UILAYER_SetDirectDraw( FALSE );

    return (result);
}

/*****************************************************************************/
// Description : check invalidate rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckInvalidateRect(
                                  GUI_RECT_T    *rect_ptr   //in/out
                                  )
{
    BOOLEAN     result = FALSE;

    /** 2位对齐 **/
    rect_ptr->left  = (int16)(rect_ptr->left - (rect_ptr->left % 2));
    rect_ptr->right = (int16)(((rect_ptr->right+2) / 2) * 2 - 1);

    if (rect_ptr->top != rect_ptr->bottom)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : get logic lcd width and height
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_GetLogicWidthHeight(
                                          LCD_ID_E      lcd_id,             //in:
                                          uint16        *logic_width_ptr,   //in/out:may PNULL
                                          uint16        *logic_height_ptr   //in/out:may PNULL
                                          )
{
    BOOLEAN     result = FALSE;

    result = GUILCD_GetWidthHeightByAngle(lcd_id,
                s_logic_angle[lcd_id],
                logic_width_ptr,
                logic_height_ptr);

    return (result);
}

/*****************************************************************************/
// Description : get logic lcd width and height by angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_GetWidthHeightByAngle(
                                            LCD_ID_E    lcd_id,             //in:
                                            LCD_ANGLE_E lcd_angle,          //in:
                                            uint16      *logic_width_ptr,   //in/out:may PNULL
                                            uint16      *logic_height_ptr   //in/out:may PNULL
                                            )
{
    BOOLEAN     result = FALSE;
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;
    uint16      tem_width = 0;
    LCD_INFO_T  lcd_info = {0};

    //get lcd info
    if (GUILCD_GetInfo(lcd_id,&lcd_info))
    {
        if (GUI_MAIN_LCD_ID == lcd_id)
        {
            if (lcd_info.lcd_width < lcd_info.lcd_height)
            {
                lcd_width  = lcd_info.lcd_width;
                lcd_height = lcd_info.lcd_height;
            }
            else
            {
                lcd_width  = lcd_info.lcd_height;
                lcd_height = lcd_info.lcd_width;
            }                        
            
            //get width/height by logic angle
            switch (lcd_angle)
            {
            case LCD_ANGLE_0:
            case LCD_ANGLE_180:
                result = TRUE;
                //do nothing
                break;
                
            case LCD_ANGLE_90:
            case LCD_ANGLE_270:
                tem_width  = lcd_width;
                lcd_width  = lcd_height;
                lcd_height = tem_width;
                result = TRUE;
                break;
                
            default:
                result = FALSE;
                //SCI_TRACE_LOW:"GUILCD_GetLogicWidthHeight: logic angle %d is error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUILCD_676_112_2_18_3_19_39_206,(uint8*)"d",lcd_angle);
                break;
            }
            
            //set width
            if (PNULL != logic_width_ptr)
            {
                *logic_width_ptr = lcd_width;
            }
            
            //set height
            if (PNULL != logic_height_ptr)
            {
                *logic_height_ptr = lcd_height;
            }
        }
        else
        {
            if (PNULL != logic_width_ptr)
            {
                *logic_width_ptr = lcd_info.lcd_width;
            }
            if (PNULL != logic_height_ptr)
            {
                *logic_height_ptr = lcd_info.lcd_height;
            }
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : convert
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL __inline DISPLAY_DATA_TYPE_E ConvertResolutionToDataType(
                                                               uint32 resolution
                                                               )
{
    DISPLAY_DATA_TYPE_E type = DATA_TYPE_RGB565;
    switch( resolution )
    {
    case LCD_RESOLUTION_RGB888:
        type = DATA_TYPE_ARGB888;
    	break;
    case LCD_RESOLUTION_RGB666:
        type = DATA_TYPE_ARGB666;
    	break;
    case LCD_RESOLUTION_RGB565:
        type = DATA_TYPE_RGB565;
        break;
    case LCD_RESOLUTION_RGB555:
        type = DATA_TYPE_ARGB555;
        break;
    case LCD_RESOLUTION_GREY:
        type = DATA_TYPE_GREY;
    	break;
    case LCD_RESOLUTION_YUV422:
        type = DATA_TYPE_YUV422;
    	break;
    case LCD_RESOLUTION_YUV420:
        type = DATA_TYPE_YUV420;
        break;
    case LCD_RESOLUTION_YUV400:
        type = DATA_TYPE_YUV400;
        break;
    default:
        break;
    }

    return type;
}


/*****************************************************************************/
// Description : get block info for invalidate lcd
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL uint32 GetInvalidateBlockInfo(
                                    uint32                  max_block_num,      //in:max block number
                                    LCD_ID_E                lcd_id,             //in:
                                    DISPLAY_LAYER_INFO_T    *lcd_block_ptr,     //in/out:
                                    DISPLAY_CONVERT_INPUT_T *convert_input_ptr  //in
                                    )
{
    uint32                      i = 0;
    uint32                      block_num = 0;
    GUIBLOCK_INFO_T             *block_info_ptr = PNULL;
    DISPLAY_CONVERT_OUTPUT_T    convert_output = {0};

    SCI_ASSERT(GUI_BLOCK_MAX == max_block_num); /*assert verified*/

    block_info_ptr = GUIBLOCK_GetInfo(lcd_id);
    if (PNULL == block_info_ptr)
    {
        //SCI_TRACE_LOW:"== GetInvalidateBlockInfo == can get lcd id %d info"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUILCD_760_112_2_18_3_19_39_207,(uint8*)"d", lcd_id);

        return block_num;
    }

    for (i=0; i<GUI_BLOCK_MAX; i++)
    {
        if (block_info_ptr[i].is_enable)
        {
            //set block info
            lcd_block_ptr[block_num].layer_id   = GUIBLOCK_ConvertBlockId(lcd_id,i);
            lcd_block_ptr[block_num].is_enable  = TRUE;
            lcd_block_ptr[block_num].buf_addr   = (uint32)(block_info_ptr[i].init_buf_ptr + block_info_ptr[i].offset);

            //set block data
            lcd_block_ptr[block_num].layer_data.type        = ConvertResolutionToDataType( block_info_ptr[i].config.resolution );
            lcd_block_ptr[block_num].layer_data.alpha_sel   = (DISPLAY_ALPHA_TYPE_E)block_info_ptr[i].config.alpha_sel;
            lcd_block_ptr[block_num].layer_data.alpha       = (uint32)block_info_ptr[i].config.alpha;
            lcd_block_ptr[block_num].layer_data.colorkey_en = block_info_ptr[i].config.colorkey_en;
            lcd_block_ptr[block_num].layer_data.colorkey    = block_info_ptr[i].config.colorkey;
            lcd_block_ptr[block_num].layer_data.grey_value  = block_info_ptr[i].config.grey_rgb;
            
            //set logic rect
            convert_input_ptr->logic_coordinate.left   = block_info_ptr[i].config.start_x;
            convert_input_ptr->logic_coordinate.top    = block_info_ptr[i].config.start_y;
            convert_input_ptr->logic_coordinate.right  = block_info_ptr[i].config.end_x;
            convert_input_ptr->logic_coordinate.bottom = block_info_ptr[i].config.end_y;

            //convert logic rect to physical rect and angle
            DISPLAY_ConvertCoordinate(lcd_id,
                convert_input_ptr,
                &convert_output);

            //set physical rect
            lcd_block_ptr[block_num].pos.x  = convert_output.physical_coordinate.left;
            lcd_block_ptr[block_num].pos.y  = convert_output.physical_coordinate.top;
            lcd_block_ptr[block_num].width  = (uint16)(convert_output.physical_coordinate.right - convert_output.physical_coordinate.left + 1);
            lcd_block_ptr[block_num].height = (uint16)(convert_output.physical_coordinate.bottom - convert_output.physical_coordinate.top + 1);

            block_num++;
        }
    }

    return (block_num);
}

/*****************************************************************************/
// Description : is landscape
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_IsLandscape(
                                  LCD_ID_E  lcd_id
                                  )
{
    BOOLEAN         result = FALSE;
    LCD_ANGLE_E     logic_angle = LCD_ANGLE_0;

    //get logic angle
    logic_angle = GUILCD_GetLogicAngle(lcd_id);
    if ((LCD_ANGLE_90 == logic_angle) ||
        (LCD_ANGLE_270 == logic_angle))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : convert logic rect and angle to physical rect and angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_ConvertRectAndAngle(
                                          LCD_ID_E      lcd_id,             //in:
                                          GUI_RECT_T    *logic_rect_ptr,    //in:
                                          LCD_ANGLE_E   *logic_angle_ptr,   //in:
                                          GUI_RECT_T    *physical_rect_ptr, //in/out:may PNULL
                                          LCD_ANGLE_E   *physical_angle_ptr //in/out:may PNULL
                                          )
{
    BOOLEAN                     result = FALSE;
    DISPLAY_CONVERT_INPUT_T     convert_input = {0};
    DISPLAY_CONVERT_OUTPUT_T    convert_output = {0};

    if (PNULL == logic_rect_ptr || PNULL == logic_angle_ptr)
    {
        return result;
    }

    //set logic angle
    convert_input.logic_angle = *logic_angle_ptr; /*lint !e64*/

    //set logic rect
    convert_input.logic_coordinate.left   = (uint16)MAX(0,logic_rect_ptr->left);
    convert_input.logic_coordinate.top    = (uint16)MAX(0,logic_rect_ptr->top);
    convert_input.logic_coordinate.right  = (uint16)MAX(0,logic_rect_ptr->right);
    convert_input.logic_coordinate.bottom = (uint16)MAX(0,logic_rect_ptr->bottom);

    //set logic width and height
    GUILCD_GetLogicWidthHeight(lcd_id,
        &convert_input.logic_lcd_width,
        &convert_input.logic_lcd_height);

    //convert
    DISPLAY_ConvertCoordinate(lcd_id,
            &convert_input,
            &convert_output);

    if (PNULL != physical_rect_ptr)
    {
        physical_rect_ptr->left   = convert_output.physical_coordinate.left;
        physical_rect_ptr->top    = convert_output.physical_coordinate.top;
        physical_rect_ptr->right  = convert_output.physical_coordinate.right;
        physical_rect_ptr->bottom = convert_output.physical_coordinate.bottom;
    }

    if (PNULL != physical_angle_ptr)
    {
        *physical_angle_ptr = convert_output.rotation_angle; /*lint !e64*/
    }

    return (result);
}

/*****************************************************************************/
// Description : convert key by lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void GUILCD_ConvertKeyCode(
                                  uint32        *key_code_ptr,  //in/out:
                                  LCD_ID_E      lcd_id,         //in:
                                  LCD_ANGLE_E   logic_angle     //in:current angle
                                  )
{
    LCD_ANGLE_E     rotate_angle = LCD_ANGLE_0;

    //get rotate angle
    rotate_angle = GetRotateAngle(lcd_id,logic_angle);


    switch (rotate_angle)
    {
    case LCD_ANGLE_90:
        switch (*key_code_ptr)
        {
        case KEY_UP:
            *key_code_ptr = KEY_LEFT;
            break;

        case KEY_DOWN:
            *key_code_ptr = KEY_RIGHT;
            break;

        case KEY_LEFT:
            *key_code_ptr = KEY_DOWN;
            break;

        case KEY_RIGHT:
            *key_code_ptr = KEY_UP;
            break;

        default:
            break;
        }
        break;

    case LCD_ANGLE_180:
        switch (*key_code_ptr)
        {
        case KEY_UP:
            *key_code_ptr = KEY_DOWN;
            break;

        case KEY_DOWN:
            *key_code_ptr = KEY_UP;
            break;

        case KEY_LEFT:
            *key_code_ptr = KEY_RIGHT;
            break;

        case KEY_RIGHT:
            *key_code_ptr = KEY_LEFT;
            break;

        default:
            break;
        }
        break;

    case LCD_ANGLE_270:
        switch (*key_code_ptr)
        {
        case KEY_UP:
            *key_code_ptr = KEY_RIGHT;
            break;

        case KEY_DOWN:
            *key_code_ptr = KEY_LEFT;
            break;

        case KEY_LEFT:
            *key_code_ptr = KEY_UP;
            break;

        case KEY_RIGHT:
            *key_code_ptr = KEY_DOWN;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// Description : get rotate angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC LCD_ANGLE_E GUILCD_GetRotateAngle(
                                         LCD_ID_E   lcd_id  //in:
                                         )
{
    LCD_ANGLE_E     logic_angle = LCD_ANGLE_0;
    LCD_ANGLE_E     rotate_angle = LCD_ANGLE_0;

    //get lcd logic angle
    logic_angle = GUILCD_GetLogicAngle(lcd_id);

    //get rotate angle
    rotate_angle = GetRotateAngle(lcd_id,logic_angle);

    return (rotate_angle);
}

/*****************************************************************************/
// Description : get rotate angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL LCD_ANGLE_E GetRotateAngle(
                                 LCD_ID_E       lcd_id,     //in:
                                 LCD_ANGLE_E    logic_angle //in:
                                 )
{
    LCD_ANGLE_E     init_logic_angle = LCD_ANGLE_0;
    LCD_ANGLE_E     rotate_angle = LCD_ANGLE_0;

    //get lcd init logic angle
    init_logic_angle = s_init_logic_angle[lcd_id];

    //get rotate angle
    if (logic_angle > init_logic_angle)
    {
        rotate_angle = (LCD_ANGLE_E)((uint32)logic_angle - (uint32)init_logic_angle);
    }
    else if (logic_angle < init_logic_angle)
    {
        rotate_angle = (LCD_ANGLE_E)(4 + (uint32)logic_angle - (uint32)init_logic_angle);
    }

    return (rotate_angle);
}

/*****************************************************************************/
// Description : convert tp point by lcd logic angle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void GUILCD_ConvertTpPoint(
                                  uint32    *x_ptr, //in/out:
                                  uint32    *y_ptr, //in/out:
                                  LCD_ID_E  lcd_id  //in:
                                  )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint32  		old_x = 0;
    uint32  		old_y = 0;
    LCD_ANGLE_E     rotate_angle = LCD_ANGLE_0;

    if (PNULL == x_ptr || PNULL == y_ptr)
    {
        return;
    }

    if (!MMITHEME_IsFocusTpCoordinateWin())
    {
        //get rotate angle
        rotate_angle = GUILCD_GetRotateAngle(lcd_id);

        //get lcd width and height
        GUILCD_GetLogicWidthHeight(lcd_id,&lcd_width,&lcd_height);

        switch (rotate_angle)
        {
        case LCD_ANGLE_90:
            old_x  = *x_ptr;
            *x_ptr = *y_ptr;
            *y_ptr = (uint32)(lcd_height - old_x);
            break;

        case LCD_ANGLE_180:
            *x_ptr = (uint32)(lcd_width - *x_ptr);
            *y_ptr = (uint32)(lcd_height - *y_ptr);
            break;

        case LCD_ANGLE_270:
            old_y  = *y_ptr;
            *y_ptr = *x_ptr;
            *x_ptr = (uint32)(lcd_width - old_y);
            break;

        default:
            break;
        }
    }
}

#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
/*****************************************************************************/
// Description : convert tp point by lcd logic angle
// Global resource dependence : 
// Author: zhenpeng.yao
// Note:we can get the rotate angle from LCD_GetWearType() according to the angle ,change TP point
/*****************************************************************************/
PUBLIC void GUILCD_ConvertTpPointEx(
                                  uint32    *x_ptr, //in/out:
                                  uint32    *y_ptr, //in/out:
                                  LCD_ID_E  lcd_id  //in:
                                  )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint32          old_x = 0;
    uint32          old_y = 0;
    LCD_ANGLE_E     rotate_angle = LCD_ANGLE_0;

    if (PNULL == x_ptr || PNULL == y_ptr)
    {
        return;
    }

    if (!MMITHEME_IsFocusTpCoordinateWin())
    {
        //get rotate angle
        rotate_angle = LCD_GetWearType();

        //get lcd width and height
        GUILCD_GetLogicWidthHeight(lcd_id,&lcd_width,&lcd_height);

        switch (rotate_angle)
        {
        case LCD_ANGLE_90:
            old_x  = *x_ptr;
            *x_ptr = *y_ptr;
            *y_ptr = (uint32)(lcd_height - old_x);
            break;

        case LCD_ANGLE_180:
            *x_ptr = (uint32)(lcd_width - *x_ptr);
            *y_ptr = (uint32)(lcd_height - *y_ptr);
            break;

        case LCD_ANGLE_270:
            old_y  = *y_ptr;
            *y_ptr = *x_ptr;
            *x_ptr = (uint32)(lcd_width - old_y);
            break;

        default:
            break;
        }
    }
    // if there is something wrong with TP point,we can open the SCI_TRACE_LOW to see the log.we can not open it in normal mode.
    //SCI_TRACE_LOW("GUILCD_ConvertTpPointEx:rotate_angle=%d,*x_ptr=%d,*y_ptr=%d",rotate_angle,*x_ptr,*y_ptr)
}
#endif

/*****************************************************************************/
// Description : convert logic coordinate to  physical coordinate
// Global resource dependence : 
// Author: robert.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_ConvertLogicToPhysicalCoordinate(
                                          LCD_ID_E      lcd_id,             //in:
                                          GUILCD_COORDINATE_PARAM_T  *logic_coordinate_ptr, //in:
                                          GUILCD_COORDINATE_PARAM_T  *physical_coordinate_ptr //out:
                                          )
{
    BOOLEAN                     result = FALSE;
    GUI_RECT_T                  logic_rect = {0};
    LCD_ANGLE_E                 logic_angle = 0;
    uint16                      logic_width = 0;
    uint16                      logic_height = 0;
    GUI_RECT_T                  physical_rect = {0};
    LCD_ANGLE_E                 physical_angle = 0;
    uint16                      physical_width = 0;
    uint16                      physical_height = 0;
    
    DISPLAY_CONVERT_INPUT_T     convert_input = {0};
    DISPLAY_CONVERT_OUTPUT_T    convert_output = {0};
    
    ERR_DISPLAY_E ret = ERR_DISPLAY_NONE;
    LCD_INFO_T lcd_info = {0};

    if (PNULL == logic_coordinate_ptr)
    {
        return result;
    }

    //init param
    logic_rect = logic_coordinate_ptr->rect;
    logic_angle = logic_coordinate_ptr->angle;
    logic_width = logic_coordinate_ptr->lcd_width;
    logic_height = logic_coordinate_ptr->lcd_height;
    
    //set logic angle
    convert_input.logic_angle = logic_angle; /*lint !e64*/

    //set logic rect
    convert_input.logic_coordinate.left   = (uint16)MAX(0,logic_rect.left);
    convert_input.logic_coordinate.top    = (uint16)MAX(0,logic_rect.top);
    convert_input.logic_coordinate.right  = (uint16)MAX(0,logic_rect.right);
    convert_input.logic_coordinate.bottom = (uint16)MAX(0,logic_rect.bottom);

    //set logic width and height
    convert_input.logic_lcd_width = logic_width;
    convert_input.logic_lcd_height = logic_height;

    //convert
    if (SCI_SUCCESS  == DISPLAY_ConvertCoordinate(lcd_id,&convert_input,&convert_output))
    {
        result = TRUE;
    }

    if (PNULL != physical_coordinate_ptr)
    {
        ret = (ERR_DISPLAY_E)LCD_GetInfo(lcd_id,&lcd_info);
        if(ret != ERR_DISPLAY_NONE)
        {
            return FALSE;
        }
        
        physical_rect.left   = convert_output.physical_coordinate.left;
        physical_rect.top    = convert_output.physical_coordinate.top;
        physical_rect.right  = convert_output.physical_coordinate.right;
        physical_rect.bottom = convert_output.physical_coordinate.bottom;

        
        physical_width = lcd_info.lcd_width;
        physical_height = lcd_info.lcd_height;

        physical_angle = convert_output.rotation_angle; /*lint !e64*/

        physical_coordinate_ptr->rect = physical_rect;
        physical_coordinate_ptr->angle = physical_angle;
        physical_coordinate_ptr->lcd_width = physical_width;
        physical_coordinate_ptr->lcd_height = physical_height;
        
    }

    return (result);
}

#endif
