/*****************************************************************************
** File Name:      mmitheme_anim.c                                           *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_ANIM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmitheme_anim.h"
#include "mmi_image.h"
#include "mmi_modu_main.h"
#include "block_mem_def.h"
#include "block_mem.h"
#include "mmi_wallpaper_export.h"
//#include "mmicc_position.h"
#include "mmi_position.h"
#include "mmifmm_export.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_ANIM_ZOOM_MAX_WIDTH           640//1280
#define MMITHEME_ANIM_ZOOM_MAX_HEIGHT          480//960

#define MMITHEME_ANIM_RES_PERIOD                200     //200ms
#define MMITHEME_ANIM_PERIOD                    100     //100ms

#define MMITHEME_CREAT_MINIATURE_RESOLUTION     (640*480)   //图片分辨率>640*480才需要生成相应的缩略图

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN   s_mmitheme_anim_is_open_key_lock_win = FALSE;

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
//  Description : is open key lock window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsOpenKeyLockWin(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get anim theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetAnimTheme(
                           MMITHEME_ANIM_T  *theme_ptr //in/out:
                           )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if(PNULL == theme_ptr)
    {
        return;
    }
    
    //set zoom max width and height
    theme_ptr->zoom_max_width  = MMITHEME_ANIM_ZOOM_MAX_WIDTH;
    theme_ptr->zoom_max_height = MMITHEME_ANIM_ZOOM_MAX_HEIGHT;
    
    //set zoom ratio
    theme_ptr->zoom_in_ratio.num1  = 3;
    theme_ptr->zoom_in_ratio.num2  = 2;
    theme_ptr->zoom_out_ratio.num1 = 2;
    theme_ptr->zoom_out_ratio.num2 = 3;
    
    //set anim period
    theme_ptr->period     = MMITHEME_ANIM_PERIOD;
    theme_ptr->res_period = MMITHEME_ANIM_RES_PERIOD;
    
    //set other
    theme_ptr->min_resolution   = MMITHEME_CREAT_MINIATURE_RESOLUTION;
    theme_ptr->default_img_id   = IMAGE_COMMON_PIC_ICON;
    
    //set bg
    theme_ptr->default_bg.bg_type = GUI_BG_COLOR;
    theme_ptr->default_bg.color   = MMI_WINDOW_BACKGROUND_COLOR;
    theme_ptr->error_bg.bg_type = GUI_BG_IMG;
    theme_ptr->error_bg.img_id  = IMAGE_COMMON_PIC_ERROR;
    
    //set border
    theme_ptr->border.type  = GUI_BORDER_NONE;
    theme_ptr->border.width = DP2PX_VALUE(1);
    theme_ptr->border.color = MMI_DARK_GRAY_COLOR;
    
    //set frame
    theme_ptr->frame.type  = GUI_BORDER_SOLID;
    theme_ptr->frame.width = DP2PX_VALUE(2);
    theme_ptr->frame.color = MMI_DARK_GRAY_COLOR;
    
    //set lcd dev
    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;
    
    //set process func
    theme_ptr->IsFileExist             = MMIAPIFMM_IsFileExist;
    theme_ptr->GetDeviceFreeSpace      = MMIAPIFMM_GetDeviceFreeSpace;
    theme_ptr->DeleteFileSyn           = MMIAPIFMM_DeleteFileSyn;
    theme_ptr->GetLabelAnim            = MMI_GetLabelAnim;
    theme_ptr->GetLabelImg             = MMI_GetLabelImage;
    theme_ptr->IsDynaAllocWallpaperBuf = MMITHEME_IsDynaAllocWallpaperBuf;
#if defined MMI_WALLPAPER_SUPPORT
    theme_ptr->SaveWallpaperBitmap     = MMIAPIIDLE_SaveWallpaperBitmap;
#endif
    theme_ptr->IsOpenKeyLockWin        = IsOpenKeyLockWin;
    theme_ptr->SetFileAttr             = MMIAPIFMM_SetAttr;
}

/*****************************************************************************/
// 	Description : wallpaper buffer is dynamic alloc memory
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsDynaAllocWallpaperBuf(void)
{
    BOOLEAN     result = TRUE;
/*  
#ifdef MMI_DYNA_ALLOC_WALLPAPER_BUF
    result = TRUE;
#endif
*/
#ifdef CMCC_UI_STYLE
    result = FALSE;
#endif

    return (result);
}

/*****************************************************************************/
//  Description : is open key lock window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsOpenKeyLockWin(void)
{
    return (s_mmitheme_anim_is_open_key_lock_win);
}

/*****************************************************************************/
//  Description : is open key lock window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_SetKeyLockWin(
                                   BOOLEAN  is_open
                                   )
{
    s_mmitheme_anim_is_open_key_lock_win = is_open;
}
