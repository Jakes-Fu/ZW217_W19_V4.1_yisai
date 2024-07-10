/*****************************************************************************
** File Name:      guiblock.c                                                *
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

#ifndef _GUIBLOCK_C_
#define _GUIBLOCK_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guiblock.h"
#include "guilcd.h"
#include "guilcd_internal.h"
#include "guiref_scale.h"
#include "mmi_theme.h"
#include "mmitheme_block.h"
#include "dal_lcd.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//all block info
LOCAL GUIBLOCK_INFO_T  s_guiblock_info[GUI_LCD_MAX][GUI_BLOCK_MAX] = {0};

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
// Description : init block info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN InitBlockInfo(
                            LCD_ID_E          lcd_id,     //in:
                            GUIBLOCK_ID_E     block_id,   //in:
                            GUI_COLOR_T       *buffer_ptr //in:
                            );

/*****************************************************************************/
// Description : is need register
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedRegister(
                             GUI_LCD_DEV_INFO   *lcd_dev_ptr
                             );

/*****************************************************************************/
// Description : adjust block rect,left and width must divide 8 pixel(16byte)
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL void AdjustBlockRect(
                           GUI_RECT_T   *block_rect_ptr
                           );

/*****************************************************************************/
// Description : adjust block buffer with block rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustBlockBuffer(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr    //in:
                                );

/*****************************************************************************/
// Description : lcd id is valid
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLcdValid(
                         LCD_ID_E   lcd_id
                         );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : init block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Init(
                             LCD_ID_E  lcd_id
                             )
{
    BOOLEAN             result = FALSE;
    LCD_INFO_T 	        lcd_info = {0};
    GUIBLOCK_INFO_T     *block_info_ptr = s_guiblock_info[lcd_id];

    //init global variable
    SCI_MEMSET(block_info_ptr,0,(GUI_BLOCK_MAX*sizeof(GUIBLOCK_INFO_T)));

    if (GUILCD_GetInfo(lcd_id,&lcd_info))
    {
        //disable block 0
        if (GUI_MAIN_LCD_ID == lcd_id)
        {
            DISPLAY_DisableLayer(lcd_id,GUI_BLOCK_MAIN);
        }

        //init block info
        result = InitBlockInfo(lcd_id,GUI_BLOCK_MAIN,(GUI_COLOR_T *)lcd_info.lcdbuff_ptr);
    }

    return (result);
}

/*****************************************************************************/
// Description : init block info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN InitBlockInfo(
                            LCD_ID_E          lcd_id,     //in:
                            GUIBLOCK_ID_E     block_id,   //in:
                            GUI_COLOR_T       *buffer_ptr //in:
                            )
{
    BOOLEAN             result = FALSE;
    uint16              logic_lcd_width = 0;
    uint16              logic_lcd_height = 0;
    GUIBLOCK_INFO_T     *block_info_ptr = PNULL;

    //check lcd id
    lcd_id = GUILCD_CheckLcdId(lcd_id);

    //for scaling down
    if ((GUI_TV_ID == lcd_id) && (GUIREF_GetIsScaleDown()))
    {
        result = GUIREF_SetBlockBufferBeforeScale(lcd_id,block_id,(uint32)buffer_ptr);
    }
    else
    {
        if (GUILCD_GetLogicWidthHeight(lcd_id,&logic_lcd_width,&logic_lcd_height))
        {
            //set block info
            block_info_ptr = &s_guiblock_info[lcd_id][block_id];

            //set main block info
            block_info_ptr->is_enable    = TRUE;
            block_info_ptr->init_buf_ptr = buffer_ptr;
            block_info_ptr->offset = 0;

            //set config
            block_info_ptr->config.start_x    = 0;
            block_info_ptr->config.start_y    = 0;
            block_info_ptr->config.end_x      = (uint16)(logic_lcd_width - 1);
            block_info_ptr->config.end_y      = (uint16)(logic_lcd_height - 1);
            block_info_ptr->config.colorkey   = 0;
            block_info_ptr->config.width      = logic_lcd_width;
            block_info_ptr->config.resolution = GUIBLOCK_RESOLUTION_565;
            block_info_ptr->config.priority   = GUIBLOCK_PRIORITY_3;
            block_info_ptr->config.alpha      = 0xFF;
            block_info_ptr->config.alpha_sel  = 1;

            if (GUI_BLOCK_OSD == block_id)
            {
                block_info_ptr->config.type = GUIBLOCK_TYPE_OSD;
            }
            else
            {
                block_info_ptr->config.type = GUIBLOCK_TYPE_COMMON;
            }

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : register block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Register(
                                 GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                 )
{
    BOOLEAN             result = FALSE;
    GUI_COLOR_T         *block_buf_ptr = PNULL;
    GUI_LCD_DEV_INFO    lcd_dev = {0};

    if (GUI_SUB_LCD_ID == lcd_dev_ptr->lcd_id)
    {
        return result;
    }

    //set lcd device
    if (GUI_TV_ID == lcd_dev_ptr->lcd_id)
    {
        lcd_dev.lcd_id = GUI_MAIN_LCD_ID;
    }
    else
    {
        lcd_dev.lcd_id = lcd_dev_ptr->lcd_id;
    }
    lcd_dev.block_id = lcd_dev_ptr->block_id;

    if (IsNeedRegister(lcd_dev_ptr))
    {
        //get block buffer
        if (MMITHEME_GetBlockBuf(&lcd_dev,&block_buf_ptr))
        {
            //set block info
            result = InitBlockInfo(lcd_dev_ptr->lcd_id,lcd_dev_ptr->block_id,block_buf_ptr);
            if (result)
            {
                //disable block
                result = GUIBLOCK_Disable(lcd_dev_ptr);
            }
            else
            {
                //reset block buffer
                MMITHEME_ResetBlockBuf(&lcd_dev);
            }
        }
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : destroy block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Destroy(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                )
{
    BOOLEAN             result = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev = {0};
    GUIBLOCK_INFO_T     *block_info_ptr = PNULL;

    //set lcd device
    if (GUI_TV_ID == lcd_dev_ptr->lcd_id)
    {
        lcd_dev.lcd_id = GUI_MAIN_LCD_ID;
    }
    else
    {
        lcd_dev.lcd_id = lcd_dev_ptr->lcd_id;
    }
    lcd_dev.block_id = lcd_dev_ptr->block_id;

    if (IsNeedRegister(lcd_dev_ptr))
    {
        //disable block
        result = GUIBLOCK_Disable(lcd_dev_ptr);

        //set variable
        if (result)
        {
            //reset block buffer
            MMITHEME_ResetBlockBuf(&lcd_dev);

            //set block info
            block_info_ptr = &s_guiblock_info[lcd_dev_ptr->lcd_id][lcd_dev_ptr->block_id];
            block_info_ptr->init_buf_ptr = PNULL;
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : is need register
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedRegister(
                             GUI_LCD_DEV_INFO   *lcd_dev_ptr
                             )
{
    BOOLEAN     result = TRUE;

    if (((GUI_MAIN_LCD_ID == lcd_dev_ptr->lcd_id) || (GUI_SUB_LCD_ID == lcd_dev_ptr->lcd_id)) &&
         (GUI_BLOCK_MAIN == lcd_dev_ptr->block_id))
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set block buffer
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetBuffer(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr,   //in:
                                  GUI_COLOR_T       *buffer_ptr     //in:
                                  )
{
    BOOLEAN             result = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev = {0};

    //check lcd id
    lcd_dev.lcd_id   = GUILCD_CheckLcdId(lcd_dev_ptr->lcd_id);
    lcd_dev.block_id = lcd_dev_ptr->block_id;

    //for scaling down
    if ((GUI_TV_ID == lcd_dev.lcd_id) && (GUIREF_GetIsScaleDown()))
    {
        result = GUIREF_SetBlockBufferBeforeScale(lcd_dev.lcd_id,lcd_dev.block_id,(uint32)buffer_ptr);
    }
    else
    {
        //set block buffer
        s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].init_buf_ptr = buffer_ptr;
        
        //set rect buffer
        AdjustBlockBuffer(lcd_dev_ptr);
    }

    return (result);
}

/*****************************************************************************/
// Description : is enable block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_IsEnable(
                                 GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                 )
{
    BOOLEAN             result = TRUE;
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    result = s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].is_enable;

    return (result);
}

/*****************************************************************************/
// Description : enable block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Enable(
                               GUI_LCD_DEV_INFO     *lcd_dev_ptr
                               )
{
    BOOLEAN             result = TRUE;
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].is_enable = TRUE;

    return (result);
}

/*****************************************************************************/
// Description : disable block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_Disable(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                )
{
    BOOLEAN             result = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    if (s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].is_enable)
    {
        //disable block
	    if (ERR_LCD_NONE == DISPLAY_DisableLayer(lcd_dev.lcd_id,GUIBLOCK_ConvertBlockId(lcd_dev.lcd_id,lcd_dev.block_id)))
        {
            s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].is_enable = FALSE;

#ifdef _WIN32
            s_guiblock_info[GUI_MAIN_LCD_ID][GUI_BLOCK_MAIN].is_enable = TRUE;
#endif
            result = TRUE;
        }
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : convert mmi block id to ref block id
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC uint32 GUIBLOCK_ConvertBlockId(
                                      LCD_ID_E          lcd_id, //in
                                      GUIBLOCK_ID_E     block_id//in
                                      )
{
    uint32      convert_block_id = 0;

    if (GUI_MAIN_LCD_ID == lcd_id)
    {
        //mmi don't use block 0,from block 1
        convert_block_id = (uint32)(block_id + 2);


    }
    else
    {
        convert_block_id = block_id;
    }

    return (convert_block_id);
}

/*****************************************************************************/
// Description : set block rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetRect(
                                GUI_RECT_T          block_rect, //in:
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr//in:
                                )
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          tv_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;
    GUIBLOCK_INFO_T     *block_info_ptr = PNULL;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    //adjust rect
    AdjustBlockRect(&block_rect);

    //adjust block buffer
    if (AdjustBlockBuffer(&lcd_dev))
    {
        //set block info
        block_info_ptr = &s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id];

        //set block rect
        block_info_ptr->rect = block_rect;

        if (GUI_TV_ID == lcd_dev.lcd_id)
        {
            MMITHEME_GetTVOutShowRect(&tv_rect);
            block_info_ptr->config.start_x = (uint16)(block_info_ptr->rect.left + tv_rect.left);
            block_info_ptr->config.start_y = (uint16)(block_info_ptr->rect.top + tv_rect.top);
            block_info_ptr->config.end_x   = (uint16)(block_info_ptr->rect.right + tv_rect.left);
            block_info_ptr->config.end_y   = (uint16)(block_info_ptr->rect.bottom + tv_rect.top);
        }
        else
        {
            block_info_ptr->config.start_x = block_info_ptr->rect.left;
            block_info_ptr->config.start_y = block_info_ptr->rect.top;
            block_info_ptr->config.end_x   = block_info_ptr->rect.right;
            block_info_ptr->config.end_y   = block_info_ptr->rect.bottom;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : reset block rect 0
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_ClearRect(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                                  )
{
    BOOLEAN             result = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    //lcd is valid
    if (IsLcdValid(lcd_dev_ptr->lcd_id))
    {
        //reset rect
        s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].rect.left   = 0;
        s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].rect.top    = 0;
        s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].rect.right  = 0;
        s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].rect.bottom = 0;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : adjust block rect,left and width must divide 8 pixel(16byte)
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL void AdjustBlockRect(
                           GUI_RECT_T   *block_rect_ptr
                           )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != block_rect_ptr);    /*assert verified*/
    if (PNULL != block_rect_ptr)
    {
        //adjust rectm,left and width必须能够被8个象素点整除
        block_rect_ptr->left  = (int16)(block_rect_ptr->left - block_rect_ptr->left%MMIBLOCK_MIN_UNIT);
        block_rect_ptr->right = (int16)(((block_rect_ptr->right/MMIBLOCK_MIN_UNIT)+1)*MMIBLOCK_MIN_UNIT - 1);
    }    
}

/*****************************************************************************/
// Description : adjust block buffer with block rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustBlockBuffer(
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr    //in:
                                )
{
    BOOLEAN         result = FALSE;
    uint16          logic_lcd_width = 0;
    GUI_RECT_T      block_rect = s_guiblock_info[lcd_dev_ptr->lcd_id][lcd_dev_ptr->block_id].rect;
    GUI_COLOR_T     *block_buffer_ptr = PNULL;

    //lcd is valid
    if (IsLcdValid(lcd_dev_ptr->lcd_id))
    {
        if (GUILCD_GetLogicWidthHeight(lcd_dev_ptr->lcd_id,&logic_lcd_width,PNULL))
        {
            //get buffer
            block_buffer_ptr = s_guiblock_info[lcd_dev_ptr->lcd_id][lcd_dev_ptr->block_id].init_buf_ptr;

            //set buffer with block rect
            //block_buffer_ptr = block_buffer_ptr + block_rect.left + block_rect.top*logic_lcd_width;
            //s_guiblock_info[lcd_dev_ptr->lcd_id][lcd_dev_ptr->block_id].rect_buf_ptr = block_buffer_ptr;
            s_guiblock_info[lcd_dev_ptr->lcd_id][lcd_dev_ptr->block_id].offset = block_rect.left + block_rect.top*logic_lcd_width;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : lcd id is valid
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLcdValid(
                         LCD_ID_E   lcd_id
                         )
{
    BOOLEAN     result = FALSE;

    if ((GUI_MAIN_LCD_ID == lcd_id) ||
        (GUI_TV_ID == lcd_id))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
// Description : set block priority
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetPriority(
                                    GUI_LCD_DEV_INFO        *lcd_dev_ptr,   //in:
                                    GUIBLOCK_PRIORITY_E     block_priority  //in:
                                    )
{
    BOOLEAN             result = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    //lcd is valid
    if (IsLcdValid(lcd_dev.lcd_id))
    {
        //set block priority
        s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id].config.priority = (uint8)block_priority;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set block type,common blcok or osd block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetType(
                                uint32              alpha,      //in:
                                GUI_COLOR_T         color_key,  //in:
                                GUIBLOCK_TYPE_E     block_type, //in:
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr//in:
                                )
{
    BOOLEAN             result = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;
    GUIBLOCK_INFO_T     *block_info_ptr = PNULL;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    //lcd is valid
    if (IsLcdValid(lcd_dev.lcd_id))
    {
        //set block info
        block_info_ptr = &s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id];

        //set block config
        block_info_ptr->config.resolution  = GUIBLOCK_RESOLUTION_565;
        block_info_ptr->config.alpha       = (uint8)alpha;
        block_info_ptr->config.alpha_sel   = 1;
        block_info_ptr->config.colorkey    = color_key;
        block_info_ptr->config.colorkey_en = 1;
        block_info_ptr->config.type        = (uint8)block_type;
        
        result = TRUE;
    }

    return (result);
}

PUBLIC BOOLEAN GUIBLOCK_ResetType(
                                GUIBLOCK_TYPE_E     block_type, //in:
                                GUI_LCD_DEV_INFO    *lcd_dev_ptr//in:
                                )
{
    BOOLEAN             result = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev = *lcd_dev_ptr;
    GUIBLOCK_INFO_T     *block_info_ptr = PNULL;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    //lcd is valid
    if (IsLcdValid(lcd_dev.lcd_id))
    {
        //set block info
        block_info_ptr = &s_guiblock_info[lcd_dev.lcd_id][lcd_dev.block_id];

        //set block config
        block_info_ptr->config.resolution  = GUIBLOCK_RESOLUTION_565;
        block_info_ptr->config.alpha       = 0xFF;
        block_info_ptr->config.alpha_sel   = 1;
        block_info_ptr->config.colorkey    = 0;
        block_info_ptr->config.colorkey_en = 0;
        block_info_ptr->config.type        = (uint8)block_type;
        
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : reset main block type
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void GUIBLOCK_ResetMainType(void)
{
    GUIBLOCK_INFO_T     *block_info_ptr = PNULL;

    //set block info
    block_info_ptr = &s_guiblock_info[GUI_MAIN_LCD_ID][GUI_BLOCK_MAIN];

    //set block config
    block_info_ptr->config.resolution  = GUIBLOCK_RESOLUTION_565;
    block_info_ptr->config.alpha       = 0xFF;
    block_info_ptr->config.alpha_sel   = 1;
    block_info_ptr->config.colorkey    = 0;
    block_info_ptr->config.colorkey_en = 0;
    block_info_ptr->config.type        = GUIBLOCK_TYPE_COMMON;
}

/*****************************************************************************/
// Description : clear block with color key
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_ClearByColor(
                                     GUI_COLOR_T        color,
                                     GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                     )
{
    BOOLEAN                 result = FALSE;
    uint16                  logic_lcd_width = 0;
    uint16                  logic_lcd_height = 0;
    uint32                  pixel_number = 0;
    register uint32         color_32bit = 0;
    register uint32         *dest_buf_ptr = PNULL;
    register GUI_COLOR_T    *src_buf_ptr = PNULL;
    GUI_LCD_DEV_INFO        lcd_dev = *lcd_dev_ptr;

    //check lcd id
    lcd_dev.lcd_id = GUILCD_CheckLcdId(lcd_dev.lcd_id);

    //lcd is valid
    if (IsLcdValid(lcd_dev.lcd_id))
    {
        if (GUILCD_GetLogicWidthHeight(lcd_dev.lcd_id,&logic_lcd_width,&logic_lcd_height))
        {
            //get clear pixel number
            pixel_number = logic_lcd_height*((logic_lcd_width+(MMIBLOCK_MIN_UNIT-1))/MMIBLOCK_MIN_UNIT)*MMIBLOCK_MIN_UNIT;

            //get block buffer
            src_buf_ptr = GUIBLOCK_GetBuf(&lcd_dev);
            if (PNULL != src_buf_ptr)
            {
                //set dest block buffer
                dest_buf_ptr         = (uint32 *)(src_buf_ptr + pixel_number) - 1;
    	        dest_buf_ptr = (char *)dest_buf_ptr - (uint32)((uint32)dest_buf_ptr%sizeof(uint32));/*lint !e63*/

                //clear block buffer
    	        color_32bit = (color << 16 ) | color;
    	        do 
                {
                    *(dest_buf_ptr--) = color_32bit;
                } while(dest_buf_ptr >= (uint32 *)src_buf_ptr);

                if (GUI_TV_ID == lcd_dev.lcd_id)
                {
                    GUIREF_ClearTvOsdOutputData(color);
                }

                result = TRUE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : disable block
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBLOCK_SetHalfTransparent(
                                           uint32               alpha,
                                           GUI_RECT_T           display_rect,
                                           GUI_COLOR_T          color_key,
                                           GUI_LCD_DEV_INFO     *lcd_dev_ptr,
                                           GUIBLOCK_TYPE_E      block_type,
                                           GUIBLOCK_PRIORITY_E  display_priority
                                           )
{
    BOOLEAN     result = FALSE;
    
    //register block
    if (GUIBLOCK_Register(lcd_dev_ptr))
    {
        //clear rect
        GUIBLOCK_ClearRect(lcd_dev_ptr);

        //set block priority
        GUIBLOCK_SetPriority(lcd_dev_ptr,display_priority);

        //set block type
        GUIBLOCK_SetType(alpha,color_key,block_type,lcd_dev_ptr);

        //clear block by color
        GUIBLOCK_ClearByColor(color_key,lcd_dev_ptr);
        
        //set block rect
        GUIBLOCK_SetRect(display_rect,lcd_dev_ptr);

        //enable block
        if (GUIBLOCK_Enable(lcd_dev_ptr))
        {
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"GUIBLOCK_SetHalfTransparent: enable block fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIBLOCK_762_112_2_18_3_15_8_52,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUIBLOCK_SetHalfTransparent: register block fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIBLOCK_767_112_2_18_3_15_8_53,(uint8*)"");
    }

    return (result);
}

/*****************************************************************************/
// Description : get block info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC GUIBLOCK_INFO_T* GUIBLOCK_GetInfo(
                                         LCD_ID_E   lcd_id
                                         )
{
    GUIBLOCK_INFO_T* block_info_ptr = PNULL;

    if ((GUI_MAIN_LCD_ID == lcd_id) || (GUI_SUB_LCD_ID == lcd_id))
    {
        if ( GUI_MAIN_LCD_ID == lcd_id )
        {
            s_guiblock_info[0][0].init_buf_ptr = GUILCD_GetMainLcdBufPtr();
        }
        
        block_info_ptr = s_guiblock_info[lcd_id];
    }
    else
    {
        //SCI_TRACE_LOW:"== GUIBLOCK_GetInfo == lcd id %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIBLOCK_794_112_2_18_3_15_8_54,(uint8*)"d", lcd_id);
    }

    return block_info_ptr;
}

/*****************************************************************************/
// Description : get block info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T* GUIBLOCK_GetBuf(
                                    GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                    )
{
    GUI_COLOR_T     *block_buf_ptr = PNULL;
	
    if (PNULL != lcd_dev_ptr)
	{
		if ((GUI_MAIN_LCD_ID == lcd_dev_ptr->lcd_id) && 
			(GUI_BLOCK_MAIN == lcd_dev_ptr->block_id))
		{
			s_guiblock_info[0][0].init_buf_ptr = GUILCD_GetMainLcdBufPtr();
		}
		
#ifdef UI_MULTILAYER_SUPPORT
		switch (UILAYER_GetHandleType(lcd_dev_ptr))
		{
		case UILAYER_HANDLE_BLOCK:       //block handle
			block_buf_ptr = s_guiblock_info[lcd_dev_ptr->lcd_id][lcd_dev_ptr->block_id].init_buf_ptr;
			break;
        
		case UILAYER_HANDLE_INVALID:     //无效handle
		case UILAYER_HANDLE_MAIN:        //主层handle
		case UILAYER_HANDLE_MULTI:        //multi handle
		default:
			block_buf_ptr = (GUI_COLOR_T*)UILAYER_GetLayerBufferPtr(lcd_dev_ptr);
			if (PNULL == block_buf_ptr)
			{
				//SCI_TRACE_LOW:"== GUIBLOCK_GetBuf == block_buf_ptr is PNULL"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIBLOCK_831_112_2_18_3_15_8_55,(uint8*)"");
			}
			break;
		}
#else
		block_buf_ptr = s_guiblock_info[lcd_dev_ptr->lcd_id][lcd_dev_ptr->block_id].init_buf_ptr;
#endif
	}

    return (block_buf_ptr);
}

/*****************************************************************************/
// Description : modify block width and rect info for rotation
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void GUIBLOCK_Rotation(
                              LCD_ID_E  lcd_id
                              )
{
    uint16              logic_lcd_width = 0;
    uint16              logic_lcd_height = 0;
    GUIBLOCK_INFO_T     *block_info_ptr = PNULL;

    //check lcd id
    lcd_id = GUILCD_CheckLcdId(lcd_id);

    //for scaling down
    if ((GUI_TV_ID == lcd_id) && (GUIREF_GetIsScaleDown()))
    {
        //do nothing
    }
    else
    {
        if (GUILCD_GetLogicWidthHeight(lcd_id,&logic_lcd_width,&logic_lcd_height))
        {
            //set block info
            block_info_ptr = &s_guiblock_info[lcd_id][GUI_BLOCK_MAIN];

            //modify width and rect
            block_info_ptr->config.end_x = (uint16)(logic_lcd_width - 1);
            block_info_ptr->config.end_y = (uint16)(logic_lcd_height - 1);
            block_info_ptr->config.width = logic_lcd_width;
        }
    }
}

#endif
