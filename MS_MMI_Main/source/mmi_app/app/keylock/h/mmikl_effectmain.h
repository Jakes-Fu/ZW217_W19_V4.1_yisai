/*****************************************************************************
** File Name:      mmikl_effectmain.h                                        *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe effect keylock main function*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma	       Create
******************************************************************************/

#ifndef _MMIKL_EFFECTMAIN_H_
#define _MMIKL_EFFECTMAIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_font.h"
#include "guifont.h"
//#include "mmiset_lock_effect.h"
#include "mmiset_lock_effect_export.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
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

#define MMI_KL_PATH_LEN_MAX     128
#define MMI_KL_STR_LEN_MAX        20

#define MMI_KL_COLOR_ALPHA_MAX              255
#define MMI_KL_256_ALIGN_PADDING             256

/* select one member from rect and pos */
#define MMK_KL_ELEMENT_BASE \
    MMI_KL_ELEMENT_E element;   \
    GUI_RECT_T rect;   \
    GUI_POINT_T pos;

#define START_TIMER(timer_id, time)  \
{   \
    if(timer_id != 0)   \
    {   \
        MMK_StopTimer(timer_id);    \
    }   \
    timer_id = MMK_CreateWinTimer(MMIKL_CLOCK_DISP_WIN_ID, time, FALSE);    \
}

#define STOP_TIMER(timer_id)    \
{   \
    if(timer_id != 0)   \
    {   \
        MMK_StopTimer(timer_id);    \
        timer_id = 0;   \
    }   \
}

#define RESET_TIMER_KL(timer_id)   \
{   \
    if(timer_id != 0)   \
    {   \
        timer_id = 0;   \
    }   \
}

#define MMI_KL_CREATE_LAYER(create_info, win_handle, layer_width, layer_height, layer, result) \
{   \
    create_info.lcd_id = GUI_MAIN_LCD_ID;   \
    create_info.owner_handle = win_handle;  \
    create_info.offset_x = 0;   \
    create_info.offset_y = 0;   \
    create_info.width = layer_width;  \
    create_info.height = layer_height;    \
    create_info.is_bg_layer = FALSE;    \
    create_info.is_static_layer = FALSE;    \
    result = UILAYER_CreateLayer(&create_info, &layer);     \
}
typedef enum
{
    PROC_CALL = 0,
    PROC_AUDIO,
    PROC_CAM,
    PROC_SMS,
    PROC_CALC,
    PROC_MAINMENU,
    PROC_MAX
}KL_PROC_FLAG_E;
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : MMIKL_CreatEffectWallpaperCtrl
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_CreatEffectWallpaperCtrl(MMISET_KEY_LOCK_EFFECT_E lock_type);
/******************************************************************************/
//  Description : handle kl display window msg
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleEffeckKLWinMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        uint16          msg_id, 
                                        DPARAM          param
                                        );

/*****************************************************************************/
// 	Description :  Init process flag.
//	Global resource dependence :
//   Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIKL_InitProcFlag(void);
/*****************************************************************************/
// 	Description :  Open the process by the flag.
//	Global resource dependence :
//   Author: Yintang.Ren
//	Note: Only one process would be opened.
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_OpenEffectProc(MMI_WIN_ID_T win_id);


/*****************************************************************************/
// 	Description :  Display the missed call num and missed SMS num.
//	Global resource dependence : 
//  Author: 
//  Date:
/*****************************************************************************/
PUBLIC void MMIKL_DisplayMissedEvent(
                                     MMI_WIN_ID_T win_id, 
                                     GUI_RECT_T *rect, 
                                     const GUI_LCD_DEV_INFO * dev_info_ptr
                                     );
/*****************************************************************************/
// 	Description :  Display date and day
//	Global resource dependence :
//   Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIKL_DisplayDateAndDay(
                                    GUI_LCD_DEV_INFO *dev, 
                                    GUI_RECT_T rect, 
                                    GUI_FONT_T font, 
                                    GUI_ALIGN_E align, 
                                    GUI_COLOR_T color, 
                                    GUIFONT_EFFECT_T effect 
                                    );
/*****************************************************************************/
// 	Description :  General  digital clock display.
//	Global resource dependence :
//   Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIKL_DisplayDigitalClock(
                                      GUI_LCD_DEV_INFO *dev, 
                                      GUI_RECT_T rect, 
                                      MMI_IMAGE_ID_T start_img_id
                                      );

/*****************************************************************************/
// 	Description :  Set the flag that the corresponding process will be opened later.
//	Global resource dependence :
//   Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIKL_SetProcOpenFlag(KL_PROC_FLAG_E flag);

/*****************************************************************************/
// 	Description :  Display image which is pack the GUI API for special using.
//	Global resource dependence :
//   Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIKL_DispImg(
                          GUI_RECT_T * display_rect, 
                          MMI_WIN_ID_T win_id, 
                          MMI_IMAGE_ID_T img_id, 
                          const GUI_LCD_DEV_INFO * dev_info_ptr, 
                          BOOLEAN is_align_mid
                          );
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMILIVEWALLPAPER_GetPreviewFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMILIVEWALLPAPER_GetKLFgInfo(LW_FG_INFO *lw_fg_info_ptr);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
