/*****************************************************************************
** File Name:      mmitheme_block.c                                          *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_BLOCK_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "std_header.h"
#include "guiblock.h"
#include "guilcd.h"
#include "mmi_theme.h"
#include "mmitheme_block.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//mmi需要分配的block buffer number,main/osd block buffer由ref分配
#define MMIBLOCK_ALLOC_BUFFER_NUM   (GUI_BLOCK_MAX-2)

//#ifdef NANDBOOT_SUPPORT
//#define MMIBLOCK_BUFFER_SIZE    ((MMI_MAINSCREEN_WIDTH)*(((MMI_MAINSCREEN_HEIGHT)+(MMIBLOCK_MIN_UNIT-1))/MMIBLOCK_MIN_UNIT)*MMIBLOCK_MIN_UNIT)
//#else
#define MMIBLOCK_BUFFER_SIZE    1
//#endif

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//mmi block buffer info
typedef struct
{
    GUI_COLOR_T     block_buffer[MMIBLOCK_BUFFER_SIZE]; //block buffer
    BOOLEAN         is_used;
    uint8           reserved[7];    // the buffer address of block must be the multiple of 8.
} MMITHEME_BLOCK_INFO_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//block buffer
#ifdef _WIN32
LOCAL MMITHEME_BLOCK_INFO_T     s_mmiblock_info[MMIBLOCK_ALLOC_BUFFER_NUM] = {0};
#else
//#ifdef LCD_BLOCK_SUPPORT
LOCAL MMITHEME_BLOCK_INFO_T s_mmiblock_info[MMIBLOCK_ALLOC_BUFFER_NUM] __attribute__((aligned(32))) = {0};
//#endif
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get block buffer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetBlockBuf(
                                    GUI_LCD_DEV_INFO    *lcd_dev_ptr,   //in:
                                    GUI_COLOR_T         **block_buf_pptr//in/out:
                                    )
{
    BOOLEAN     result = FALSE;

    if ((GUI_MAIN_LCD_ID == lcd_dev_ptr->lcd_id) || 
        (GUI_TV_ID == lcd_dev_ptr->lcd_id))
    {
        switch (lcd_dev_ptr->block_id)
        {
        case GUI_BLOCK_1:
//#ifdef LCD_BLOCK_SUPPORT
            //block if is used
			if (s_mmiblock_info[lcd_dev_ptr->block_id-1].is_used)
			{
				//SCI_TRACE_LOW:"MMITHEME_GetBlockBuf: block %d is used!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_BLOCK_86_112_2_18_3_1_54_8,(uint8*)"d",lcd_dev_ptr->block_id);
			}
			else
			{
				*block_buf_pptr = s_mmiblock_info[lcd_dev_ptr->block_id-1].block_buffer;
				s_mmiblock_info[lcd_dev_ptr->block_id-1].is_used = TRUE;
				result = TRUE;
			}
//#endif
            break;

        case GUI_BLOCK_OSD:
            *block_buf_pptr = GUILCD_GetMainLcdBufPtr();
            result = TRUE;
            break;

        default:
            //SCI_TRACE_LOW:"MMITHEME_GetBlockBuf: block_id %d no block buffer"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_BLOCK_103_112_2_18_3_1_54_9,(uint8*)"d",lcd_dev_ptr->block_id);
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : reset block buffer is not used
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_ResetBlockBuf(
                                   GUI_LCD_DEV_INFO *lcd_dev_ptr//in:
                                   )
{
    if (((GUI_MAIN_LCD_ID == lcd_dev_ptr->lcd_id) || (GUI_TV_ID == lcd_dev_ptr->lcd_id)) &&
        (GUI_BLOCK_1 == lcd_dev_ptr->block_id))
    {
//#ifdef LCD_BLOCK_SUPPORT
        if (s_mmiblock_info[lcd_dev_ptr->block_id-1].is_used)
        {
            s_mmiblock_info[lcd_dev_ptr->block_id-1].is_used = FALSE;
        }
//#endif
    }
}
