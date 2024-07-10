/*****************************************************************************
** File Name:      mmitheme_anim.h                                           *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007      Jassmine              Creat
******************************************************************************/

#ifndef _MMITHEME_ANIM_H_
#define _MMITHEME_ANIM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "guilcd.h"
#include "guiimg.h"
#include "guictrl.h"
#include "mmitheme_form.h"
#include "sfs.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_ANIM_MIN_PERIOD                50          //50ms,图片浏览,幻灯片浏览时,时间间隔为50ms

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//the anim theme
typedef struct
{
    uint16              zoom_max_width;     //src max width for zoom
    uint16              zoom_max_height;    //src max height for zoom

    MMITHEME_RATIO_T    zoom_in_ratio;      //zoom in的倍数,放大后图片宽度=当前图片宽*zoom_in_ratio (must > 1)
    MMITHEME_RATIO_T    zoom_out_ratio;     //zoom out的倍数,缩小后图片宽度=当前图片宽*zoom_out_ratio (must < 1)

    uint32              period;             //anim period
    uint32              res_period;         //resource anim period
    uint32              min_resolution;     //creat miniature min resolution
    
    MMI_IMAGE_ID_T      default_img_id;     //default display image icon id
    GUI_BG_T            default_bg;         //default bg
    GUI_BG_T            error_bg;           //error bg,may image or color
    
    GUI_BORDER_T        border;             //border
    GUI_BORDER_T        frame;              //anim frame
    
    GUI_LCD_DEV_INFO    lcd_dev;            //lcd dev

    BOOLEAN             (*IsFileExist)(const uint16*,uint16);  //file is exist
    BOOLEAN             (*GetDeviceFreeSpace)(const uint16*,uint16,uint32*,uint32*);//get device free space

    BOOLEAN             (*DeleteFileSyn)(uint16*,uint16);   //delete file syn
                
    const uint8*        (*GetLabelAnim)(MMI_ANIM_ID_T,MMI_WIN_ID_T,uint32*);    //get anim
    const uint8*        (*GetLabelImg)(MMI_IMAGE_ID_T,MMI_WIN_ID_T,uint32*);    //get image

    BOOLEAN             (*IsDynaAllocWallpaperBuf)(void);           //is dyna alloc wallpaper buffer
    BOOLEAN             (*SaveWallpaperBitmap)(GUIIMG_BITMAP_T*);   //save wallpaper bitmap

    BOOLEAN             (*IsOpenKeyLockWin)(void);  //is open key lock window

    BOOLEAN             (*SetFileAttr)(const wchar*,uint16,BOOLEAN,BOOLEAN,BOOLEAN,BOOLEAN);    //set file attribute
} MMITHEME_ANIM_T;

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
                           );

/*****************************************************************************/
// 	Description : wallpaper buffer is dynamic alloc memory
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsDynaAllocWallpaperBuf(void);

/*****************************************************************************/
//  Description : is open key lock window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_SetKeyLockWin(
                                   BOOLEAN  is_open
                                   );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

