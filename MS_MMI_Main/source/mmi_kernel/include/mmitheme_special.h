/*****************************************************************************
** File Name:      mmitheme_special.h                                           *
** Author:         bin.ji                                                          *
** Date:           04/29/2008                                                *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    窗口特效                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/29/2008      bin.ji              Create
******************************************************************************/

#ifndef _MMITHEME_SPECIAL_H_
#define _MMITHEME_SPECIAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
//#include "guicommon.h"
//#include "guiblock.h"
//#include "guifont.h"
//#include "guistring.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
// 各种3D动画执行的帧数
#define SE_ZOOM_IN_FRAME_NUM             10
#define SE_ZOOM_OUT_FRAME_NUM            10
#define SE_ZOOM_INOUT_FRAME_NUM          10  
#define SE_HOR_SLIDE_FRAME_NUM           16
#define SE_VER_SLIDE_FRAME_NUM           16
#define SE_HOR_SLIDE_WITH_OLD_FRAME_NUM  16     
#define SE_VER_SLIDE_WITH_OLD_FRAME_NUM  10
#define SE_FADE_FRAME_NUM                8
#define SE_POPMENU_FRAME_NUM            6
#define SE_LCD_SWITCH_ROTATE_NUM            8 
#define SE_RIPPLE_FRAME_NUM             8

// 各种动画执行的time
#if defined MAINLCD_SIZE_240X400
#define     MMI_WIN_ANIM_NONE_SLEEP_TIME                    (0)
#define     MMI_WIN_ANIM_ZOOM_IN_SLEEP_TIME                 (0)
#define     MMI_WIN_ANIM_ZOOM_OUT_SLEEP_TIME                (0)
#define     MMI_WIN_ANIM_ZOOM_INOUT_SLEEP_TIME              (0)
#define     MMI_WIN_ANIM_HOR_SLIDE_SLEEP_TIME               (0)
#define     MMI_WIN_ANIM_VER_SLIDE_SLEEP_TIME               (0)
#define     MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD_SLEEP_TIME      (0)
#define     MMI_WIN_ANIM_VER_SLIDE_WITH_OLD_SLEEP_TIME      (0)
#define     MMI_WIN_ANIM_FADE_SLEEP_TIME                    (0)
#define     MMI_WIN_ANIM_3D_SLEEP_TIME                      (15)
#define     MMI_WIN_ANIM_ROTATE_SLEEP_TIME                  (10)
#define     MMI_WIN_ANIM_POPMENU_SLEEP_TIME                 (5)
#define     MMI_WIN_ANIM_3DLIST_SLEEP_TIME                  (15)
#elif defined MAINLCD_SIZE_320X480
#define     MMI_WIN_ANIM_NONE_SLEEP_TIME                    (0)
#define     MMI_WIN_ANIM_ZOOM_IN_SLEEP_TIME                 (0)
#define     MMI_WIN_ANIM_ZOOM_OUT_SLEEP_TIME                (0)
#define     MMI_WIN_ANIM_ZOOM_INOUT_SLEEP_TIME              (0)
#define     MMI_WIN_ANIM_HOR_SLIDE_SLEEP_TIME               (0)
#define     MMI_WIN_ANIM_VER_SLIDE_SLEEP_TIME               (0)
#define     MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD_SLEEP_TIME      (0)
#define     MMI_WIN_ANIM_VER_SLIDE_WITH_OLD_SLEEP_TIME      (0)
#define     MMI_WIN_ANIM_FADE_SLEEP_TIME                    (0)
#define     MMI_WIN_ANIM_3D_SLEEP_TIME                      (10)
#define     MMI_WIN_ANIM_ROTATE_SLEEP_TIME                  (10)
#define     MMI_WIN_ANIM_POPMENU_SLEEP_TIME                 (5)
#define     MMI_WIN_ANIM_3DLIST_SLEEP_TIME                  (10)
#else
#define     MMI_WIN_ANIM_NONE_SLEEP_TIME                    (0)
#define     MMI_WIN_ANIM_ZOOM_IN_SLEEP_TIME                 (0)
#define     MMI_WIN_ANIM_ZOOM_OUT_SLEEP_TIME                (0)
#define     MMI_WIN_ANIM_ZOOM_INOUT_SLEEP_TIME              (0)
#define     MMI_WIN_ANIM_HOR_SLIDE_SLEEP_TIME               (0)
#define     MMI_WIN_ANIM_VER_SLIDE_SLEEP_TIME               (0)
#define     MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD_SLEEP_TIME      (0)
#define     MMI_WIN_ANIM_VER_SLIDE_WITH_OLD_SLEEP_TIME      (0)
#define     MMI_WIN_ANIM_FADE_SLEEP_TIME                    (0)
#define     MMI_WIN_ANIM_3D_SLEEP_TIME                      (20)
#define     MMI_WIN_ANIM_ROTATE_SLEEP_TIME                  (10)
#define     MMI_WIN_ANIM_POPMENU_SLEEP_TIME                 (5)
#define     MMI_WIN_ANIM_3DLIST_SLEEP_TIME                  (20)
#endif
#ifdef UI_P3D_SUPPORT   
#define WINSE_DEF(winse_type, winse_setting_type)  winse_type,
#endif
// 动画的类型
typedef enum
{
    MMI_WIN_ANIM_NONE,
#ifdef EFFECT_ZOOM_SUPPORT		
    MMI_WIN_ANIM_ZOOM_IN,      
    MMI_WIN_ANIM_ZOOM_OUT,
    MMI_WIN_ANIM_ZOOM_INOUT,
#endif

#ifdef EFFECT_WIN_SLIDE_SUPPORT
    MMI_WIN_ANIM_HOR_SLIDE,
    MMI_WIN_ANIM_VER_SLIDE,
    MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD,
    MMI_WIN_ANIM_VER_SLIDE_WITH_OLD, 
#endif

#ifdef EFFECT_WIN_SLIDE_SUPPORT
    MMI_WIN_ANIM_FADE,
#endif    
#ifdef EFFECT_3D_ROTATE_SUPPORT		    
    MMI_WIN_ANIM_3D, 
#endif
#ifdef UI_P3D_SUPPORT   
    MMI_WIN_ANIM_SE_TYPE_START,
    #include "mmi_winse.def"//使用一个def
    MMI_WIN_ANIM_SE_TYPE_END,
#endif
    MMI_WIN_ANIM_ROTATE,
    MMI_WIN_ANIM_AUTO,    
    MMI_WIN_ANIM_POPMENU,
    
#ifdef EFFECT_LCD_ROTATE_SUPPORT
    MMI_WIN_ANIM_LCD_SWITCH_ROTATE,
#endif    
#ifdef EFFECT_RIPPLE_SUPPORT
    MMI_WIN_ANIM_RIPPLE,
#endif    
    MMI_WIN_ANIM_MAX
} MMI_WIN_ANIM_TYPE_E;
#ifdef UI_P3D_SUPPORT   
#undef WINSE_DEF
#endif
// 窗口执行状态
typedef enum
{
    MMI_WIN_DISP_STATE_NONE,
    MMI_WIN_DISP_STATE_PAINT,
    MMI_WIN_DISP_STATE_CLOSE,        
    MMI_WIN_DISP_STATE_LOSE_FOCUS,        

    MMI_WIN_DISP_STATE_MAX
} MMI_WIN_DISP_STATE_E;

// 窗口类型
typedef enum
{
    MMI_WIN_TYPE_NONE,      // 无类型，使用默认类型
    MMI_WIN_TYPE_LIST,      // 全屏list窗口
    MMI_WIN_TYPE_MAINMENU,  // 主菜单窗口
    MMI_WIN_TYPE_SECMENU,   // 二级列表菜单窗口
    MMI_WIN_TYPE_POPMENU,   // popmenu窗口
    MMI_WIN_TYPE_PUBWIN,    // pubwin窗口
    MMI_WIN_TYPE_OTHER,     // 其他窗口

    MMI_WIN_TYPE_MAX
} MMI_WIN_TYPE_E;

// 窗口动画数据
typedef struct  
{
    MMI_WIN_ANIM_TYPE_E type;
    LCD_ID_E lcd_id;
    BOOLEAN  is_back;

}MMI_WIN_ANIM_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

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

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef UI_P3D_SUPPORT
#ifdef EFFECT_RIPPLE_SUPPORT
/*****************************************************************************/
//  Description: fade in/out with ripple effect between two frames.
//  Global resource dependence:
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PlayRippleFadeAnim(LCD_ID_E lcd_id);
#endif
                                   
/*****************************************************************************/
//  Description:   invalidate a rect with special effect(Fade In淡入)
//    Global resource dependence:
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InvalidateRect_FadeIn(
                                   LCD_ID_E lcd_id,//ID of lcd to operate
                                   uint16 left,     //left of the rectangle
                                   uint16 top,     //top of the rectangle
                                   uint16 right,     //right of the rectangle
                                   uint16 bottom    //bottom of the rectangle.
                                   );



/*****************************************************************************/
//  Description:   invalidate a rect with special effect(3d rotate)
//    Global resource dependence:
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PlayWinAnim(
                                 MMI_WIN_ANIM_PARAM_T* param
                                 );    

/*****************************************************************************/
// Description: 获得当前的动画类型
// Global resource dependence:
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ANIM_TYPE_E MMITHEME_Get3DScreenAnimType(
                                                        MMI_HANDLE_T win_handle,
                                                        MMI_WIN_DISP_STATE_E win_state
                                                        );


/*****************************************************************************/
// Description: 判断3D效果是否开启
// Global resource dependence:
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_Is3DOpen(void);

#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
//  Description:   Play LCD rotate animate.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PlayWinRotate(
                                   LCD_ANGLE_E cur_angle,
                                   LCD_ANGLE_E old_angle
                                   );
#endif

/*****************************************************************************/
//  Description:   Play Popup menu effect.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PopupMenuEffect(
                                     GUI_RECT_T *rect_ptr,
                                     uint32 level,
                                     BOOLEAN is_entry
                                     );

/*****************************************************************************/
//  Description:  play ver slide anim
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PlayHorSlideAnimWithOld(
                                             LCD_ID_E   lcd_id,
                                             uint16*    dst_ptr,
                                             uint16*    new_ptr,
                                             uint16*    old_ptr,
                                             uint16     buf_width,
                                             uint16     buf_height,
                                             BOOLEAN    is_r2l   //right to left
                                             );

#ifdef EFFECT_RIPPLE_SUPPORT
/*****************************************************************************/
//  Description : handle ripple effect.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_HandleRippleEffect(MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param,
                                        BOOLEAN          isRippleMove);                                             
#endif

#endif
#endif
