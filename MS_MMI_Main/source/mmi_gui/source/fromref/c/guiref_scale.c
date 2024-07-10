/*****************************************************************************
** File Name:      guiref_scale.c                                            *
** Author:                                                                   *
** Date:           02/2007                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe phone module                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2007       Jassmine           Create
******************************************************************************/
 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guiref_scale.h"
#include "guilcd.h"
#include "guiblock.h"
#ifndef _WIN32
//#include "ARM_ScaleDown.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIREF_BLOCK_16_DIVISOR     16  //must divide exactly by 16
#define GUIREF_BLOCK_2_DIVISOR      2   //must divide exactly by 2
#define GUIREF_BLOCK_8_DIVISOR      8   //must divide exactly by 8

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN   s_is_scale_down = FALSE;
LOCAL BOOLEAN   s_is_clear_osd = FALSE;
LOCAL BOOLEAN   s_is_used_memory1[GUI_BLOCK_MAX] = {0};

LOCAL uint32        s_tv_block_buffer_addr[GUI_BLOCK_MAX] = {0};
LOCAL BLOCKCFG_T    s_tv_block_cfg[GUI_BLOCK_MAX] = {0};

/*lint -e551*/ 
LOCAL uint32 s_row_pixels = 0;
LOCAL uint32 s_col_pixels = 0;
LOCAL GUI_COLOR_T *s_tvout_rect_memory = PNULL;
LOCAL GUI_COLOR_T *s_tvout_memory1[GUI_BLOCK_MAX] = {0};
LOCAL GUI_COLOR_T *s_tvout_memory2[GUI_BLOCK_MAX] = {0};
LOCAL BOOLEAN s_is_tvout_registered = FALSE;
/*lint -e551*/ 

#define TVOUT_IS_REGISTER           (s_is_tvout_registered)
#define GUIREF_TV_ROW_PIXELS        (s_row_pixels)
#define GUIREF_TV_COLUMN_PIXELS     (s_col_pixels)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : when tv out, get output data pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_COLOR_T* GetTvOutputDataPtr(
                                      BOOLEAN   is_copy,
                                      uint16    block_id
                                      );

/*****************************************************************************/
//  Description : rect1 is large or equal rect2
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Rect1IsLargeEqualRect2(
                                     GUI_RECT_T    *rect1_ptr, //in/out:交叉区域
                                     GUI_RECT_T    rect2
                                     );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : when tv out, handle scaling down function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void GUIREF_TvScalingDown(
                          uint32        update_block_set,
                          GUI_RECT_T    *update_rect_ptr    //PNULL,update all lcd
                          )
{
#ifndef _WIN32
#ifdef MMI_TVSCALINGDOWN_ENABLE
    BOOLEAN                 is_equal = FALSE;
    BOOLEAN                 update_rect_is_large = FALSE;
    uint8                   rgb_format = 0; // 0: 565,  1: 666,  2:888,  3: 555
    uint16                  input_block_width = 0;
    uint16                  input_block_heigth = 0;
    uint16                  output_block_width = 0;
    uint16                  output_block_heigth = 0;
    uint16                  scale_start_x = 0;
    uint16                  scale_end_x = 0;
    uint16                  scale_width = 0;
    int16                   j = 0,k = 0;
    uint32                  i = 0;
    BLOCKCFG_T              *block_cfg_ptr = PNULL;
    BLOCKCFG_T              scale_block_cfg = {0};
    GUI_RECT_T              block_rect = {0};
    GUI_COLOR_T             *input_data_ptr = PNULL;
    GUI_COLOR_T             *output_data_ptr = PNULL;
    GUI_LCD_DEV_INFO        lcd_dev = {GUI_TV_ID,GUI_BLOCK_MAIN};
    SCALING_COEFF_LIST_T    scaling_param = {0};

    if (!TVOUT_IS_REGISTER)
        return;

    if (GUIREF_GetIsScaleDown())
    {
        //get update block id
        for (i=0; i<GUI_BLOCK_MAX; i++)
        {
            if (0x01 == ((update_block_set >> i) & 0x01)) //update
            {
                //get input data buffer
                input_data_ptr = (GUI_COLOR_T *)GUIREF_GetBlockBufferBeforeScale(GUI_TV_ID,(GUIBLOCK_ID_E)i);
                //SCI_TRACE_LOW("GUIREF_TvScalingDown:input_data_ptr = 0x%x",input_data_ptr);

                SCI_ASSERT(PNULL != input_data_ptr); /*assert verified*/

                //get block config
                block_cfg_ptr = GUIREF_GetBlockCfgBeforeScale(GUI_TV_ID,(GUIBLOCK_ID_E)i);
                SCI_ASSERT(block_cfg_ptr != PNULL); /*assert verified*/

                //check update rect is large than all block rect
                block_rect.left = block_cfg_ptr->start_x;
                block_rect.top = block_cfg_ptr->start_y;
                block_rect.right = block_cfg_ptr->end_x;
                block_rect.bottom = block_cfg_ptr->end_y;
                //SCI_TRACE_LOW("GUIREF_TvScalingDown:block rect left = %d,top = %d,right = %d,bottom = %d",block_rect.left,block_rect.top,block_rect.right,block_rect.bottom);
                //SCI_TRACE_LOW("GUIREF_TvScalingDown:update1 rect left = %d,top = %d,right = %d,bottom = %d",update_rect_ptr->left,update_rect_ptr->top,update_rect_ptr->right,update_rect_ptr->bottom);
                update_rect_is_large = Rect1IsLargeEqualRect2(update_rect_ptr,block_rect);
                //SCI_TRACE_LOW("GUIREF_TvScalingDown:update2 rect left = %d,top = %d,right = %d,bottom = %d",update_rect_ptr->left,update_rect_ptr->top,update_rect_ptr->right,update_rect_ptr->bottom);

                //block width is equal end x - start x
                if (block_cfg_ptr->width == (block_cfg_ptr->end_x - block_cfg_ptr->start_x + 1))
                {
                    is_equal = TRUE;
                }
                else
                {
                    is_equal = FALSE;
                }

                //set input block width,must divide exactly by 16
                input_block_width = block_cfg_ptr->width;
                if (0 != input_block_width % GUIREF_BLOCK_16_DIVISOR)
                {
                    input_block_width = input_block_width + GUIREF_BLOCK_16_DIVISOR - input_block_width % GUIREF_BLOCK_16_DIVISOR;
                }

                //set input block height,must divide exactly by 2
                input_block_heigth = block_cfg_ptr->end_y - block_cfg_ptr->start_y + 1;

                //set rgb format
                if (0 == block_cfg_ptr->type)
                {
                    rgb_format = 3;
                }

                //set output block width,must divide exactly by 16
                output_block_width = input_block_width * 137/160;
                if (0 != output_block_width % GUIREF_BLOCK_16_DIVISOR)
                {
                    output_block_width = output_block_width + GUIREF_BLOCK_16_DIVISOR - output_block_width % GUIREF_BLOCK_16_DIVISOR;
                }

                //set scale width
                scale_width = output_block_width;

                //set output block height,must divide exactly by 2
                if (update_rect_is_large)
                {
                    output_block_heigth = input_block_heigth * 137/160;
                }
                else
                {
                    output_block_heigth = (update_rect_ptr->bottom-update_rect_ptr->top+1) * 137/160;
                }

                //set scaling down param
                if (update_rect_is_large)
                {
                    scaling_param.start_pos_x    = 0;
                    scaling_param.start_pos_y    = 0;
                    scaling_param.before_scale_w = input_block_width;
                    scaling_param.before_scale_h = input_block_heigth;
                    scaling_param.after_scale_w  = output_block_width;
                }
                else
                {
                    scaling_param.start_pos_x    = update_rect_ptr->left-block_rect.left;
                    scaling_param.start_pos_y    = update_rect_ptr->top-block_rect.top;
                    scaling_param.before_scale_w = update_rect_ptr->right-update_rect_ptr->left+1;
                    //set input block width,must divide exactly by 16
                    if (0 != scaling_param.before_scale_w  % GUIREF_BLOCK_16_DIVISOR)
                    {
                        scaling_param.before_scale_w  = scaling_param.before_scale_w  + GUIREF_BLOCK_16_DIVISOR - scaling_param.before_scale_w  % GUIREF_BLOCK_16_DIVISOR;
                    }
                    SCI_ASSERT(0 < scaling_param.before_scale_w); /*assert verified*/

                    scaling_param.before_scale_h = update_rect_ptr->bottom-update_rect_ptr->top+1;
                    SCI_ASSERT(0 < scaling_param.before_scale_h); /*assert verified*/

                    scaling_param.after_scale_w  = scaling_param.before_scale_w*137/160;
                    if (0 != scaling_param.after_scale_w % GUIREF_BLOCK_16_DIVISOR)
                    {
                        scaling_param.after_scale_w = scaling_param.after_scale_w + GUIREF_BLOCK_16_DIVISOR - scaling_param.after_scale_w % GUIREF_BLOCK_16_DIVISOR;
                    }
                    SCI_ASSERT(0 < scaling_param.after_scale_w); /*assert verified*/
                }
                scaling_param.rgb_format     = rgb_format;
                scaling_param.input_orig_w   = input_block_width;
                scaling_param.input_orig_h   = input_block_heigth;
                scaling_param.after_scale_h  = output_block_heigth;

                //SCI_TRACE_LOW("GUIREF_TvScalingDown:startx = %d,starty = %d,before_scale_w = %d,before_scale_h= %d,after_scale_w=%d,after_scale_h=%d,input_orig_w=%d,input_orig_h=%d",scaling_param.start_pos_x,scaling_param.start_pos_y,scaling_param.before_scale_w,scaling_param.before_scale_h,scaling_param.after_scale_w,scaling_param.after_scale_h,scaling_param.input_orig_w,scaling_param.input_orig_h);
                //SCI_TRACE_LOW("GUIREF_TvScalingDown:update_rect_is_large = %d",update_rect_is_large);

                //scaling down
                if (update_rect_is_large)
                {
                    //set output data buffer
                    output_data_ptr = GetTvOutputDataPtr(FALSE,i);
                    SCI_ASSERT(PNULL != output_data_ptr); /*assert verified*/

//                    ScalingDown((uint16 *)input_data_ptr,(uint16 *)output_data_ptr,scaling_param,1);
                }
                else
                {
                    uint16  after_scaling_start_x = scaling_param.start_pos_x*137/160;
                    uint16  after_scaling_start_y = scaling_param.start_pos_y*137/160;

                    //set output data buffer
                    //SCI_TRACE_LOW:"GUIREF_TvScalingDown:s_is_clear_osd = %d,i = %d!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIREF_SCALE_239_112_2_18_3_18_41_166,(uint8*)"dd",s_is_clear_osd,i);
                    if ((GUI_BLOCK_OSD == i) && (s_is_clear_osd))
                    {
                    	//SCI_TRACE_LOW("GUIREF_TvScalingDown:not copy!");
                        output_data_ptr = GetTvOutputDataPtr(FALSE,i);

                        //set osd has update
                        s_is_clear_osd = FALSE;
                    }
                    else
                    {
               			//SCI_TRACE_LOW("GUIREF_TvScalingDown:copy!");
                        output_data_ptr = GetTvOutputDataPtr(TRUE,i);
                    }
                    SCI_ASSERT(PNULL != output_data_ptr); /*assert verified*/

//                    ScalingDown((uint16 *)input_data_ptr,(uint16 *)s_tvout_rect_memory,scaling_param,1);

                    //SCI_TRACE_LOW("GUIREF_TvScalingDown:scale_width = %d",scale_width);
                    
                    //move scaling down data to out buffer(TV)
                    for(j=0; j<scaling_param.after_scale_h; j++)
                    {
                        for (k=0; k<scaling_param.after_scale_w; k++)
                        {
                            *((output_data_ptr+after_scaling_start_x+after_scaling_start_y*scale_width)+(j*scale_width)+k) = *(s_tvout_rect_memory+(j*scaling_param.after_scale_w)+k);
                        }
                    }
                }

                //set block buffer
                lcd_dev.block_id = (GUIBLOCK_ID_E)i;
                GUIBLOCK_SetBuffer(&lcd_dev,output_data_ptr);

                //set scale block config
                MMI_MEMCPY(&scale_block_cfg,sizeof(BLOCKCFG_T),
                    block_cfg_ptr,sizeof(BLOCKCFG_T),
                    sizeof(BLOCKCFG_T));

                //SCI_TRACE_LOW:"GUIREF_TvScalingDown1:i = %d,scale_start_x = %d,scale_end_x = %d,start_y = %d,end_y = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIREF_SCALE_278_112_2_18_3_18_41_167,(uint8*)"ddddd",i,block_cfg_ptr->start_x,block_cfg_ptr->end_x,block_cfg_ptr->start_y,block_cfg_ptr->end_y);

                //set start x,must divide exactly by 8
                scale_start_x = block_cfg_ptr->start_x*137/160;
                if (0 != scale_start_x % GUIREF_BLOCK_8_DIVISOR)
                {
                    scale_start_x = scale_start_x + GUIREF_BLOCK_8_DIVISOR - scale_start_x % GUIREF_BLOCK_8_DIVISOR;
                }
                scale_block_cfg.start_x = scale_start_x;
                scale_block_cfg.start_y = block_cfg_ptr->start_y*137/160;

                //set end x
                scale_end_x = block_cfg_ptr->end_x*137/160;
                if (is_equal)
                {
                    if (0 != (scale_end_x - scale_start_x + 1) % GUIREF_BLOCK_16_DIVISOR)
                    {
                        scale_end_x = scale_end_x + GUIREF_BLOCK_16_DIVISOR - (scale_end_x - scale_start_x + 1) % GUIREF_BLOCK_16_DIVISOR;
                    }
                }
                else
                {                    
                    if (0 != (scale_end_x - scale_start_x + 1) % GUIREF_BLOCK_8_DIVISOR)
                    {
                        scale_end_x = scale_end_x + GUIREF_BLOCK_8_DIVISOR - (scale_end_x - scale_start_x + 1) % GUIREF_BLOCK_8_DIVISOR;
                    }
                }
                scale_block_cfg.end_x = scale_end_x;
                scale_block_cfg.end_y = block_cfg_ptr->end_y*137/160;
                
                scale_block_cfg.width = scale_width;

                //SCI_TRACE_LOW("GUIREF_TvScalingDown2:scale_start_x = %d,scale_end_x = %d,start_y = %d,end_y = %d",scale_start_x,scale_end_x,scale_block_cfg.start_y,scale_block_cfg.end_y);

                //set block config
				if(GUI_BLOCK_OSD == i)
				{
					LCDC_ConfigBlock(GUI_TV_ID,(GUIBLOCK_ID_E)i,scale_block_cfg);
				}
				else
				LCD_ConfigBlock(GUI_TV_ID,(GUIBLOCK_ID_E)i,scale_block_cfg);
            }
        }
    }
#endif  //MMI_TVSCALINGDOWN_ENABLE    
#endif
}

/*****************************************************************************/
//  Description : when tv out, get block buffer by block id before scaling down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
uint32 GUIREF_GetBlockBufferBeforeScale(
                                        GUI_LCD_ID_E    lcd_id,  //in
                                        GUIBLOCK_ID_E   block_id //in
                                        )
{
    uint32  block_buffer_addr = 0;

    //SCI_TRACE_LOW:"GUIREF_GetBlockBufferBeforeScale:lcd_id = %d,block_id = %d,s_tv_block_buffer_addr[block_id] = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIREF_SCALE_337_112_2_18_3_18_41_168,(uint8*)"ddd",lcd_id,block_id,s_tv_block_buffer_addr[block_id]);
    if (GUI_TV_ID == lcd_id)
    {
        block_buffer_addr = s_tv_block_buffer_addr[block_id];
    }

    return (block_buffer_addr);
}

/*****************************************************************************/
//  Description : when tv out, set block buffer by block id before scaling down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIREF_SetBlockBufferBeforeScale(
                                                LCD_ID_E        lcd_id,             //in
                                                GUIBLOCK_ID_E   block_id,           //in
                                                uint32          block_buffer_addr   //in
                                                )
{
    BOOLEAN     result = FALSE;

    if (block_id >= GUI_BLOCK_MAX)
    {
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"GUIREF_SetBlockBufferBeforeScale:lcd_id = %d,block_id = %d,block_buffer_addr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIREF_SCALE_363_112_2_18_3_18_42_169,(uint8*)"ddd",lcd_id,block_id,block_buffer_addr);
    if (GUI_TV_ID == lcd_id)
    {
        s_tv_block_buffer_addr[block_id] = block_buffer_addr;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : when tv out, get output data pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_COLOR_T* GetTvOutputDataPtr(
                                      BOOLEAN   is_copy,
                                      uint16    block_id
                                      )
{
    GUI_COLOR_T     *output_data_ptr = PNULL;
    

    if (!TVOUT_IS_REGISTER)
        return PNULL;

    
#ifndef _WIN32
    //memory1 if is used
    if (s_is_used_memory1[block_id])
    {
        output_data_ptr = (GUI_COLOR_T *)&s_tvout_memory2[block_id];
        s_is_used_memory1[block_id] = FALSE;

        //SCI_TRACE_LOW:"GetTvOutputDataPtr1:s_is_used_memory1[block_id] = %d,is_copy = %d,color_key = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIREF_SCALE_397_112_2_18_3_18_42_170,(uint8*)"ddd",s_is_used_memory1[block_id],is_copy,output_data_ptr[0]);

        if (is_copy)
        {
            uint32  copy_len = sizeof(GUI_COLOR_T)*(GUIREF_TV_ROW_PIXELS*GUIREF_TV_COLUMN_PIXELS);

            SCI_MEMCPY((void *)s_tvout_memory2[block_id],
                (void *)s_tvout_memory1[block_id],
                copy_len);
        }
    }
    else
    {
        output_data_ptr = (GUI_COLOR_T *)&s_tvout_memory1[block_id];
        s_is_used_memory1[block_id] = TRUE;

        //SCI_TRACE_LOW:"GetTvOutputDataPtr2:s_is_used_memory1[block_id] = %d,is_copy = %d,color_key = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIREF_SCALE_413_112_2_18_3_18_42_171,(uint8*)"ddd",s_is_used_memory1[block_id],is_copy,output_data_ptr[0]);
        if (is_copy)
        {
            uint32  copy_len = sizeof(GUI_COLOR_T)*(GUIREF_TV_ROW_PIXELS*GUIREF_TV_COLUMN_PIXELS);

            SCI_MEMCPY((void *)s_tvout_memory1[block_id],
                (void *)s_tvout_memory2[block_id],
                copy_len);
        }
    }
#endif

    return (output_data_ptr);
}

/*****************************************************************************/
//  Description : when tv out, clear osd output buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note需要对osd buffer填入透明色
/*****************************************************************************/
void GUIREF_ClearTvOsdOutputData(
                                 GUI_COLOR_T   color_key
                                 )
{
#ifndef _WIN32
    uint16      block_id = GUI_BLOCK_OSD;
    uint32      i = 0;
  
    if (!TVOUT_IS_REGISTER)
    {
        return;
    }
 
    if (s_is_used_memory1[block_id])
    {
        for (i=0; i<(GUIREF_TV_ROW_PIXELS*GUIREF_TV_COLUMN_PIXELS); i++)
        {
            s_tvout_memory2[block_id][i] = color_key;
        }
    }
    else
    {
        for (i=0; i<(GUIREF_TV_ROW_PIXELS*GUIREF_TV_COLUMN_PIXELS); i++)
        {
            s_tvout_memory1[block_id][i] = color_key;
        }
    }
    s_is_clear_osd = TRUE;
#endif
}

/*****************************************************************************/
//  Description : when tv out, get block config by block id before scaling down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
BLOCKCFG_T* GUIREF_GetBlockCfgBeforeScale(
                                          GUI_LCD_ID_E      lcd_id,  //in
                                          GUIBLOCK_ID_E     block_id //in
                                          )
{
    BLOCKCFG_T  *block_cfg_ptr = PNULL;

    if (GUI_TV_ID == lcd_id)
    {
        block_cfg_ptr = (BLOCKCFG_T*)&s_tv_block_cfg[block_id];
    }

    return (block_cfg_ptr);
}

/*****************************************************************************/
//  Description : when tv out, set block config by block id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void GUIREF_SetBlockCfgBeforeScale(
                                   GUI_LCD_ID_E      lcd_id,
                                   GUIBLOCK_ID_E     block_id,
                                   BLOCKCFG_T        *block_cfg_ptr
                                   )
{
    if (GUI_TV_ID == lcd_id)
    {
        MMI_MEMCPY(&s_tv_block_cfg[block_id],sizeof(BLOCKCFG_T),
            block_cfg_ptr,sizeof(BLOCKCFG_T),
            sizeof(BLOCKCFG_T));
    }
    //SCI_TRACE_LOW:"GUIREF_SetBlockCfgBeforeScale:lcd_id = %d,block_id = %d,start_x = %d,end_x = %d,start_y = %d,end_y = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIREF_SCALE_498_112_2_18_3_18_42_172,(uint8*)"dddddd",lcd_id,block_id,block_cfg_ptr->start_x,block_cfg_ptr->end_x,block_cfg_ptr->start_y,block_cfg_ptr->end_y);
}

/*****************************************************************************/
//  Description : get block set with block id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
uint32 GUIREF_GetUpdateBlockSet(
                                GUIBLOCK_ID_E   block_id
                                )
{
    uint32  block_set = 0;

    switch (block_id)
    {
    case GUI_BLOCK_MAIN:
        block_set = GUIREF_SCALE_BLOCK0;
        break;

    case GUI_BLOCK_1:
        block_set = GUIREF_SCALE_BLOCK1;
        break;

    case GUI_BLOCK_2:
        block_set = GUIREF_SCALE_BLOCK2;
        break;

    default:
        SCI_PASSERT(FALSE,("GUIREF_GetUpdateBlockSet:block_id %d is error!",block_id)); /*assert verified*/
        break;
    }
    //SCI_TRACE_LOW("GUIREF_GetUpdateBlockSet:block_set = %d",block_set);
    return (block_set);
}

/*****************************************************************************/
//  Description : get if is scaling down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
BOOLEAN GUIREF_GetIsScaleDown(void)
{
    return (s_is_scale_down);
}

/*****************************************************************************/
//  Description : Set if is scaling down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void GUIREF_SetIsScaleDown(
                           BOOLEAN  is_used
                           )
{
    s_is_scale_down = is_used;
}

/*****************************************************************************/
//  Description : rect1 is large or equal rect2
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Rect1IsLargeEqualRect2(
                                     GUI_RECT_T    *rect1_ptr, //in/out:交叉区域
                                     GUI_RECT_T    rect2
                                     )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL != rect1_ptr)
    {
        if ((rect1_ptr->left <= rect2.left) &&
            (rect1_ptr->top <= rect2.top) &&
            (rect1_ptr->right >= rect2.right) && 
            (rect1_ptr->bottom >= rect2.bottom))
        {
            result = TRUE;
        }
        else
        {
            rect1_ptr->left = MAX(rect1_ptr->left,rect2.left);
            rect1_ptr->top = MAX(rect1_ptr->top,rect2.top);
            rect1_ptr->right = MIN(rect1_ptr->right,rect2.right);
            rect1_ptr->bottom = MIN(rect1_ptr->bottom,rect2.bottom);
        }
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get tv out buffer for dc
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32* GUIREF_GetTvOutBuffer(void)
{
	if(s_is_scale_down)
	{
		SCI_PASSERT(FALSE,("GUIREF_GetTvOutBuffer: s_is_scale_down %d is error!",s_is_scale_down)); /*assert verified*/
	}
#ifndef WIN32
	return (uint32*)s_tvout_memory1;
#else
	return PNULL;
#endif
}

/*****************************************************************************/
//  Description : 提供APP注册TVOUT功能（配置row_pixels，col_pixels，memory等参数）
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void GUIREF_RegisterTvOutForApp(
                                       uint32 row_pixels,
                                       uint32 col_pixels,                                       
                                       GUI_COLOR_T *tvout_rect_memory,
                                       GUI_COLOR_T **tvout_memory1,
                                       GUI_COLOR_T **tvout_memory2
                                       )
{
    uint32 i = 0;

    s_row_pixels = row_pixels;
    s_col_pixels = col_pixels;
    
    s_tvout_rect_memory = tvout_rect_memory;
    for (i = 0; i < GUI_BLOCK_MAX; i++)
    {
        s_tvout_memory1[i] = tvout_memory1[i];
        s_tvout_memory2[i] = tvout_memory2[i];
    }
    
    s_is_tvout_registered = TRUE;
}

/*****************************************************************************/
//  Description : 提供APP关闭TVOUT功能
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void GUIREF_UnregisterTvOutForApp(void)
{
    s_is_tvout_registered = FALSE;
}

