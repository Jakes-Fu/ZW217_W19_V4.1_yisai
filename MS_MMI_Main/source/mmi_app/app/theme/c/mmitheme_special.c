/*****************************************************************************
** File Name:      mmitheme_special.c                                           *
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

#define _MMITHEME_SPECIAL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmidisplay_data.h"
#include "ui_special_effect.h"
#include "mmi_default.h"
#include "guilcd.h"
#include "mmiset_display.h"

//#ifdef MMI_SPECIAL_ENABLE
#if 1
#include "dal_lcd.h"
#include "lcm_drv.h"
#include "mmitheme_special.h"
#include "block_mem.h"
#include "mmiset_display.h"
#include "mmi_theme.h"
#ifdef UI_P3D_SUPPORT
#include "mmi3d_export.h"
//#include "chng_freq.h"
#include "graphics_draw.h"
#endif
#include "special_effect.h" 
#include "mmk_tp.h"
#include "mmi_appmsg.h"

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#ifdef UI_P3D_SUPPORT   
    #include "mmi_winse.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DIV8(color)     ((color&0xc718)>>3)
#define DIV4(color)     ((color&0xe79c)>>2)
#define DIV2(color)     ((color&0xf7de)>>1)

#define K1(color)       DIV8(color)
#define K2(color)       DIV4(color)
#define K3(color)       (DIV8(color)+DIV4(color))
#define K4(color)       DIV2(color)
#define K5(color)       (DIV8(color)+DIV2(color))
#define K6(color)       (DIV2(color)+DIV4(color))
#define K7(color)       (color-DIV8(color))

#define SCR_SIZE    ((MMI_MAINSCREEN_HEIGHT*MMI_MAINSCREEN_WIDTH)*sizeof(uint16))

#ifdef EFFECT_RIPPLE_SUPPORT  
#define MMI_RIPPLE_TP_MAX 100  //max number to save ripple tp point.
#define MMI_RIPPLE_OFFSET 2
#define MMI_RIPPLE_FADE_OUT_SPEED 3
#define MMI_RIPPLE_STONE_SIZE     12
#define MMI_RIPPLE_STONE_WEIGHT   128
#endif

#ifdef WIN32
#define GET_ANIM_SLEEP_TIME(type)     (10)
#else
#define GET_ANIM_SLEEP_TIME(type)     (type##_SLEEP_TIME)
#endif

typedef uint16 (*AlphaColorFunc)(
                uint16 color1,
                uint16 color2
                );

typedef enum
{
    ROTATE_NONE = 0,                       /* 未知*/
    ROTATE_CLOCKWISE,                   /*顺时针*/
    ROTATE_COUNTERCLOCKWISE,     /*逆时针*/
    ROTATE_180                              /* 旋转180*/
}ROTATE_DIR_E;

typedef enum
{
    MMI_POPUP_MENU_EFFECT_NONE,
    MMI_POPUP_MENU_EFFECT_SLIDE_IN,
    MMI_POPUP_MENU_EFFECT_SLIDE_OUT,
    MMI_POPUP_MENU_EFFECT_MAX
}MMI_POPUP_MENU_EFFETC_TYPE_E;

typedef enum
{
    MMI_POPUP_MENU_EFFECT_DIR_NONE,
    MMI_POPUP_MENU_EFFECT_DIR_UP,
    MMI_POPUP_MENU_EFFECT_DIR_DOWN,
    MMI_POPUP_MENU_EFFECT_DIR_LEFT,
    MMI_POPUP_MENU_EFFECT_DIR_RIGHT,
    MMI_POPUP_MENU_EFFECT_DIR_MAX    
}MMI_POPUP_MENU_EFFECT_DIR_E;

typedef struct
{
    MMI_POPUP_MENU_EFFETC_TYPE_E type;
    LCD_ID_E lcd_id;
    int16 x1;                   /* (x1,y1):起始位置；(x2,y2):结束位置*/
    int16 y1;                   
    int16 x2;
    int16 y2;
    uint16 *buff_old;       
    uint16 *buff_new;
    //uint16 *buff_lcd;
    uint16 buff_wid;
    uint16 buff_height;
    uint16 buf_mem_width;
    BOOLEAN is_full_scr;       /* TRUE: 全屏特效,坐标值无效;FALSE:使用坐标值*/
    uint16 dir;                 /* 0:完全根据坐标; 左右: y2无效; 上下: x2无效*/
}MMI_POPUP_MENU_EFFECT_PARAM_T;

typedef struct
{
    MMI_POPUP_MENU_EFFETC_TYPE_E type;
    LCD_ID_E lcd_id;
    GUI_RECT_T rect;
    MMI_POPUP_MENU_EFFECT_DIR_E dir;
    uint16 *buff_bg;
    uint16 *buff_move;
}MMI_POPUP_MENU_EFFECT_PARAM_SLIDE_T;

#ifdef EFFECT_RIPPLE_SUPPORT
typedef struct ripple_effect_tag
{
    SE_HANDLE               ripple_handle;
    SE_RIPPLE_INIT_PARAM_T  init_param;
    int                     ripple_point_num;
    SE_RIPPLE_POINT_PROP_T  ripple_pt[MMI_RIPPLE_TP_MAX];
    uint8                   ripple_move_timer;
    uint8                   ripple_long_press_timer;
}MMITHEME_RIPPLE_EFFECT_T;
#endif

#ifdef EFFECT_LCD_ROTATE_ASYNC_SUPPORT
#define SCALE_WAITING   0
#define SCALE_OK    1
#define SCALE_ERROR 2

LOCAL BOOLEAN s_scale_res = SCALE_WAITING;
LOCAL SCALE_IMAGE_OUT_T s_scale_out = {0};
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

//uint16 my_buf[240*320*2];
//uint16 my_buf[5];

#ifdef EFFECT_RIPPLE_SUPPORT
LOCAL MMITHEME_RIPPLE_EFFECT_T s_ripple_effect = {0};  //ripple effect data.
#endif

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

uint16 AlphaColor0(uint16 color1, uint16 color2);
uint16 AlphaColor1(uint16 color1, uint16 color2);
uint16 AlphaColor2(uint16 color1, uint16 color2);
uint16 AlphaColor3(uint16 color1, uint16 color2);
uint16 AlphaColor4(uint16 color1, uint16 color2);
uint16 AlphaColor5(uint16 color1, uint16 color2);
uint16 AlphaColor6(uint16 color1, uint16 color2);
uint16 AlphaColor7(uint16 color1, uint16 color2);
uint16 AlphaColor8(uint16 color1, uint16 color2);

AlphaColorFunc AlphaColor_table[] = 
{
    //AlphaColor0,
    AlphaColor1,
    //AlphaColor2,
    //AlphaColor3,
    //AlphaColor4,
    //AlphaColor5,
    //AlphaColor6,
    AlphaColor7,
    //AlphaColor8
};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef UI_P3D_SUPPORT
#ifdef EFFECT_ZOOM_SUPPORT
/*****************************************************************************/
//  Description:   play 3d zoom in,then zoom out animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomInOutAnim(LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:   play 3d zoom in animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomInNextAnim(LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:   play 3d zoom in previous frame animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomInPrevAnim(LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:   play 3d zoom out animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomOutAnim(LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:   play 3d zoom out animation with black background.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomOutAnimByBlackBg(LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:  play zoom animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomAnim(MMI_WIN_ANIM_PARAM_T* param); 

/*****************************************************************************/
//  Description:   play 3d zoom out previous frame animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomOutPrevAnim(LCD_ID_E lcd_id);
#endif

/*****************************************************************************/
//  Description:   invalidate a rect with special effect(3d rotate)
//    Global resource dependence:
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL void Play3DRotateAnim(
                             LCD_ID_E lcd_id,//ID of lcd to operate
                             uint32 iDirection
                             );

#ifdef EFFECT_WIN_SLIDE_SUPPORT
/*****************************************************************************/
//  Description:   play hor slide anim
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayHorSlideAnim(
                           LCD_ID_E   lcd_id,
                           uint16*    dst_ptr,
                           uint16*    src_ptr,
                           uint16     buf_width,
                           uint16     buf_height,
                           BOOLEAN    is_rtl //right to left
                           );   

/*****************************************************************************/
//  Description:  play ver slide anim
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayVerSlideAnim(
                            LCD_ID_E   lcd_id,
                            uint16*    dst_ptr,
                            uint16*    src_ptr,
                            uint16     buf_width,
                            uint16     buf_height,
                            BOOLEAN    is_btt //bottom to top
                            ); 

/*****************************************************************************/
//  Description:  play ver slide anim
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PlayHorSlideAnimWithOld(
                            LCD_ID_E   lcd_id,
                            uint16*    dst_ptr,
                            uint16*    new_ptr,
                            uint16*    old_ptr,
                            uint16     buf_width,
                            uint16     buf_height,
                            BOOLEAN    is_r2l   //right to left
                            );

/*****************************************************************************/
//  Description:  play ver slide anim with old screen following
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayVerSlideAnimWithOld(
                            LCD_ID_E   lcd_id,
                            uint16*    dst_ptr,
                            uint16*    new_ptr,
                            uint16*    old_ptr,
                            uint16     buf_width,
                            uint16     buf_height,
                            BOOLEAN    is_t2b //top to bottom
                            );

/*****************************************************************************/
//  Description:   play slide anim
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlaySlideAnim(
                         MMI_WIN_ANIM_PARAM_T* param
                         ); 
#endif

/*****************************************************************************/
//  Description:   Get Secial effect frame number.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL uint32 GetSpecialEffectFrameNum(MMI_WIN_ANIM_TYPE_E se_type);

#if 0 //def EFFECT_WIN_FADE_SUPPORT
/*****************************************************************************/
//  Description:  play ver slide anim with old screen following
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayFadeAnim(MMI_WIN_ANIM_PARAM_T* param);
#endif

#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
//  Description:   Get LCD rotate direction.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL ROTATE_DIR_E GetLCDRotateDir(LCD_ANGLE_E cur_angle, LCD_ANGLE_E old_angle);

/*****************************************************************************/
//  Description:   Play LCD switch effect rotate.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PlayLCDSwitchEffectRotate( 
                                MMI_WIN_ANIM_PARAM_T *param, 
                                LCD_ANGLE_E cur_angle, 
                                LCD_ANGLE_E old_angle
                                );

/*****************************************************************************/
//  Description:   Play LCD switch effect fade.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PlayLCDSwitchEffectFade( 
                                MMI_WIN_ANIM_PARAM_T *param, 
                                LCD_ANGLE_E cur_angle, 
                                LCD_ANGLE_E old_angle
                                );
#endif

/*****************************************************************************/
//  Description:   play win switch effect.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PopupMenuEffectSlide(
                                MMI_POPUP_MENU_EFFECT_PARAM_T *param
                                );
#endif


/*****************************************************************************/
//  Description : 计算两颜色值之间的过渡颜色，count为细分层数
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
/*
uint16 CalcFadeColor(
                     uint16 color1,
                     uint16 color2,
                     uint32 count
                     );
                     */

void CalcFullAlphaColor(
                        int level,
                        uint16 *buf1_ptr,
                        uint16 *buf2_ptr,
                        uint16 *out_buf_ptr,
                        uint32 buf_size
                        );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef UI_P3D_SUPPORT

/*****************************************************************************/
//  Description:   Get Secial effect frame number.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL uint32 GetSpecialEffectFrameNum(MMI_WIN_ANIM_TYPE_E se_type)
{
    uint32 frame_num = 0;
   
    switch (se_type)
    {
#ifdef EFFECT_ZOOM_SUPPORT    
    case MMI_WIN_ANIM_ZOOM_IN:
        frame_num = SE_ZOOM_IN_FRAME_NUM;
        break;

    case MMI_WIN_ANIM_ZOOM_OUT:
        frame_num = SE_ZOOM_OUT_FRAME_NUM;
        break;
              
    case MMI_WIN_ANIM_ZOOM_INOUT:
        frame_num = SE_ZOOM_INOUT_FRAME_NUM;
        break;
#endif

#ifdef EFFECT_WIN_SLIDE_SUPPORT
    case MMI_WIN_ANIM_HOR_SLIDE:
        frame_num = SE_HOR_SLIDE_FRAME_NUM;
        break;

    case MMI_WIN_ANIM_VER_SLIDE:
        frame_num = SE_VER_SLIDE_FRAME_NUM;
        break;

    case MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD:
        frame_num = SE_HOR_SLIDE_WITH_OLD_FRAME_NUM;
        break;
        
    case MMI_WIN_ANIM_VER_SLIDE_WITH_OLD:
        frame_num = SE_VER_SLIDE_WITH_OLD_FRAME_NUM;
        break;
#endif

#ifdef EFFECT_WIN_SLIDE_SUPPORT
    case MMI_WIN_ANIM_FADE:
        frame_num = SE_FADE_FRAME_NUM;
        break;
#endif

    case MMI_WIN_ANIM_POPMENU:
        frame_num = SE_POPMENU_FRAME_NUM;
        break;

#ifdef EFFECT_3D_ROTATE_SUPPORT		
    case MMI_WIN_ANIM_3D:
        frame_num = SE_GetEffectFrameAmount(SE_DESKTOP_ROTATE);
        break;
#endif

#ifdef EFFECT_LCD_ROTATE_SUPPORT		
    case MMI_WIN_ANIM_LCD_SWITCH_ROTATE:
        frame_num = SE_LCD_SWITCH_ROTATE_NUM;
        break;
#endif        

#ifdef EFFECT_RIPPLE_SUPPORT
    case MMI_WIN_ANIM_RIPPLE:
        frame_num = SE_RIPPLE_FRAME_NUM;
        break;
#endif
    default:
        frame_num = 8;
        SCI_TRACE_LOW("GetSpecialEffectFrameNum()  default frame num = 8");
        break;
    }

    return frame_num;
}

#ifdef EFFECT_RIPPLE_SUPPORT
/*****************************************************************************/
//  Description : Create ripple move timer.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void CreateRippleMoveTimer(MMI_WIN_ID_T win_id)
{
   if (0 == s_ripple_effect.ripple_move_timer)
   {
       s_ripple_effect.ripple_move_timer = MMK_CreateWinTimer(win_id, 50, TRUE);
   }
}

/*****************************************************************************/
//  Description : stop ripple move timer
//  Global resource dependence : 
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
LOCAL void StopRippleMoveTimer(void)
{
    if (0 != s_ripple_effect.ripple_move_timer)
    {
        MMK_StopTimer(s_ripple_effect.ripple_move_timer);
        s_ripple_effect.ripple_move_timer = 0;
    }
}

/*****************************************************************************/
//  Description : Create ripple move timer.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void CreateRippleLongPressTimer(MMI_WIN_ID_T win_id)
{
   if (0 == s_ripple_effect.ripple_long_press_timer)
   {
       s_ripple_effect.ripple_long_press_timer = MMK_CreateWinTimer(win_id, 30, FALSE);
   }
}

/*****************************************************************************/
//  Description : stop ripple move timer
//  Global resource dependence : 
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
LOCAL void StopRippleLongPressTimer(void)
{
    if (0 != s_ripple_effect.ripple_long_press_timer)
    {
        MMK_StopTimer(s_ripple_effect.ripple_long_press_timer);
        s_ripple_effect.ripple_long_press_timer = 0;
    }
}

/*****************************************************************************/
//  Description : Create ripple effect handle.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void CreateSlideRippleEffect()
{
    //uint16 *buf_ptr   = PNULL;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;

    if (0 != s_ripple_effect.ripple_handle)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);	
    s_ripple_effect.init_param.width          = lcd_width;
    s_ripple_effect.init_param.height         = lcd_height;
    s_ripple_effect.init_param.ripple_offset  = MMI_RIPPLE_OFFSET;
    s_ripple_effect.init_param.fade_out_speed = 4;
    s_ripple_effect.init_param.stone_size     = 20;//6;
    s_ripple_effect.init_param.stone_weight   = 128;

    s_ripple_effect.init_param.target_data_format = IMGREF_FORMAT_RGB565; 
    //buf_ptr = GUILCD_GetMainLcdBufPtr();

    s_ripple_effect.init_param.src_pixels    = (uint16*)SCI_ALLOCA(lcd_width*lcd_height*2); //GUILCD_GetMainLcdBufPtr();        

    if (PNULL == s_ripple_effect.init_param.src_pixels)
    {
        return;
    }
    
    s_ripple_effect.init_param.target_pixels = (uint16*)SCI_ALLOCA(lcd_width*lcd_height*2);             // 目标缓冲区的指针

    if (PNULL == s_ripple_effect.init_param.target_pixels)
    {
        SCI_FREE(s_ripple_effect.init_param.src_pixels);
        s_ripple_effect.init_param.src_pixels = PNULL;     
        return;
    }
    
    //SCI_MEMCPY(s_ripple_effect.init_param.src_pixels, buf_ptr, lcd_width*lcd_height*2);
    
    s_ripple_effect.init_param.src_data_format = s_ripple_effect.init_param.target_data_format;// 背景缓冲区的指针

    s_ripple_effect.ripple_handle = SE_Create(SE_ID_RIPPLE,&s_ripple_effect.init_param);  
    
    if (0 == s_ripple_effect.ripple_handle)                             // 检查返回值
    {
        SCI_FREE(s_ripple_effect.init_param.src_pixels);
        s_ripple_effect.init_param.src_pixels = PNULL;      

        SCI_FREE(s_ripple_effect.init_param.target_pixels);
        s_ripple_effect.init_param.target_pixels = PNULL;     
        return;	   
    }    
    
    s_ripple_effect.ripple_point_num = 0;
}

/*****************************************************************************/
//  Description : Create ripple effect handle.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void CreateRippleEffect(MMI_WIN_ID_T win_id, BOOLEAN IsDefaultDev)
{
    //uint16 *buf_ptr   = PNULL;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;

    if (0 != s_ripple_effect.ripple_handle)
    {
        return;
    }

    if (!MMK_IsFocusWin(win_id))
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);	
    s_ripple_effect.init_param.width          = lcd_width;
    s_ripple_effect.init_param.height         = lcd_height;
    s_ripple_effect.init_param.ripple_offset  = MMI_RIPPLE_OFFSET;
    s_ripple_effect.init_param.fade_out_speed = MMI_RIPPLE_FADE_OUT_SPEED;
    s_ripple_effect.init_param.stone_size     = MMI_RIPPLE_STONE_SIZE;
    s_ripple_effect.init_param.stone_weight   = MMI_RIPPLE_STONE_WEIGHT;

    s_ripple_effect.init_param.target_data_format = IMGREF_FORMAT_RGB565; 
    //buf_ptr = GUILCD_GetMainLcdBufPtr();

    s_ripple_effect.init_param.target_pixels = (uint16*)SCI_ALLOCA(lcd_width*lcd_height*2);             // 目标缓冲区的指针

    if (PNULL == s_ripple_effect.init_param.target_pixels)
    {
        return;
    }

    if (IsDefaultDev)
    {
        s_ripple_effect.init_param.src_pixels = UILAYER_GetLayerBufferPtr(MMITHEME_GetDefaultLcdDev());
    }
    else
    {
        s_ripple_effect.init_param.src_pixels    = (uint16*)SCI_ALLOCA(lcd_width*lcd_height*2); //GUILCD_GetMainLcdBufPtr();        
        
        if (PNULL == s_ripple_effect.init_param.src_pixels)
        {
            SCI_FREE(s_ripple_effect.init_param.target_pixels);
            s_ripple_effect.init_param.target_pixels = PNULL;        
            return;
        }               
    }
    
    //SCI_MEMCPY(s_ripple_effect.init_param.src_pixels, buf_ptr, lcd_width*lcd_height*2);
    
    s_ripple_effect.init_param.src_data_format = s_ripple_effect.init_param.target_data_format;// 背景缓冲区的指针

    s_ripple_effect.ripple_handle = SE_Create(SE_ID_RIPPLE,&s_ripple_effect.init_param);  
    
    if (0 == s_ripple_effect.ripple_handle)                             // 检查返回值
    {
        SCI_FREE(s_ripple_effect.init_param.src_pixels);
        s_ripple_effect.init_param.src_pixels = PNULL;      

        SCI_FREE(s_ripple_effect.init_param.target_pixels);
        s_ripple_effect.init_param.target_pixels = PNULL;  
        return;	   
    }    
   
    UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
    s_ripple_effect.ripple_point_num = 0;

    if (0 != win_id)
    {
        //CreateRippleMoveTimer(win_id); 
    }
}

/*****************************************************************************/
//  Description : render ripple effect handle.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void RenderRippleEffect(BOOLEAN isUpdate)
{
    SE_RESULT_E ret = SE_RESULT_SUCCESS;
    SE_RIPPLE_RENDER_INPUT_PARAM_T input={0};
    SE_RIPPLE_RENDER_OUTPUT_PARAM_T output={0};
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    GUI_RECT_T rect = {0};
    uint16 *buf_ptr = PNULL;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);	

    rect.left = 0;
    rect.top  = 0;
    rect.right  = lcd_width - 1;
    rect.bottom = lcd_height - 1;

    //s_ripple_point_num = 1;
    input.skip_frame_num = 0;
    input.stone_type   = 0;
    input.point_num    = s_ripple_effect.ripple_point_num ;    
    input.points = s_ripple_effect.ripple_pt;

    //buf_ptr = GUILCD_GetMainLcdBufPtr();   
    //SCI_MEMCPY(s_ripple_effect.init_param.src_pixels, buf_ptr, lcd_width*lcd_height*2);

    ret = SE_Render(s_ripple_effect.ripple_handle, &input, &output);
    
    s_ripple_effect.ripple_point_num = 0;

    if (SE_RESULT_SUCCESS != ret)
    {
        return;
    }

    if (1 == output.is_calm)
    {
        StopRippleMoveTimer();
    }

    if (isUpdate)
    {
        buf_ptr = GUILCD_GetMainLcdBufPtr();
        SCI_MEMCPY(buf_ptr, s_ripple_effect.init_param.target_pixels, lcd_width*lcd_height*2);
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID, rect, 0);
    }
    
    if (s_ripple_effect.ripple_point_num != 0)  
    {
        s_ripple_effect.ripple_point_num = 0;
    }
}

/*****************************************************************************/
//  Description : Destroy ripple effect handle.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DestroyRippleEffect(BOOLEAN IsDefaultDev)
{
    if (0 == s_ripple_effect.ripple_handle)
    {
        return;
    }

    UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
    StopRippleMoveTimer();
    StopRippleLongPressTimer();
    
    SE_Destory(s_ripple_effect.ripple_handle);
    s_ripple_effect.ripple_handle = 0;
    s_ripple_effect.ripple_pt[0].mode = 0;

    if (PNULL != s_ripple_effect.init_param.target_pixels)
    {
        SCI_FREE(s_ripple_effect.init_param.target_pixels);
        s_ripple_effect.init_param.target_pixels = PNULL;
    }

    if (!IsDefaultDev && (PNULL != s_ripple_effect.init_param.src_pixels))
    {
        SCI_FREE(s_ripple_effect.init_param.src_pixels);
        s_ripple_effect.init_param.src_pixels = PNULL;
    }
}

/*****************************************************************************/
//  Description : save tp point each timer time.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void SaveRippleTPPoint(GUI_POINT_T pt)
{
    if (s_ripple_effect.ripple_point_num < MMI_RIPPLE_TP_MAX) 
    {
		s_ripple_effect.ripple_pt[s_ripple_effect.ripple_point_num].mode = SE_RIPPLE_MODE_MOVE;
		s_ripple_effect.ripple_pt[s_ripple_effect.ripple_point_num].stone.x = pt.x;
		s_ripple_effect.ripple_pt[s_ripple_effect.ripple_point_num].stone.y = pt.y;
		s_ripple_effect.ripple_point_num++;
	}
}

/*****************************************************************************/
//  Description : handle ripple effect.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_HandleRippleEffect(MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param,
                                        BOOLEAN           isRippleMove)
{
    GUI_POINT_T point = {0};
    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);	
    
    MMK_GetMultiTPMsg(&tp_status, &point);    

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateRippleEffect(win_id, TRUE);
        break;

    case MSG_GET_FOCUS:
        CreateRippleEffect(win_id, TRUE);
        break;

    case MSG_LOSE_FOCUS:
        DestroyRippleEffect(TRUE);
        break;

    case MSG_FULL_PAINT:      
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
		CreateRippleMoveTimer(win_id);
        CreateRippleLongPressTimer(win_id);    
        SaveRippleTPPoint(point);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        StopRippleLongPressTimer();
        
        if (isRippleMove)
        {
            SaveRippleTPPoint(point);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        StopRippleLongPressTimer();    
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_LCD_SWITCH:
        DestroyRippleEffect(TRUE); 
        CreateRippleEffect(win_id, TRUE);
        break;

    case MSG_BACKLIGHT_TURN_OFF:
        StopRippleMoveTimer();
        break;

    case MSG_BACKLIGHT_TURN_ON:
        //StopRippleMoveTimer();    
        //CreateRippleMoveTimer(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        DestroyRippleEffect(TRUE); 
        break;

    case MSG_TIMER:   
        if (s_ripple_effect.ripple_move_timer == *(uint8*)param)
        {   
#ifndef WIN32
            CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif         
            if (!isRippleMove)
            {
                RenderRippleEffect(FALSE);
                RenderRippleEffect(FALSE);             
            }
            
            RenderRippleEffect(TRUE);
            
#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif               
        }
        else
        if (s_ripple_effect.ripple_long_press_timer == *(uint8*)param)
        {
#ifndef WIN32
            CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif 		
            RenderRippleEffect(TRUE);
            CreateRippleLongPressTimer(win_id);			
#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif  			
        }
        break;
    default:
        break;
    }
}  

/*****************************************************************************/
//  Description: fade in/out with ripple effect between two frames.
//  Global resource dependence:
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PlayRippleFadeAnim(LCD_ID_E lcd_id)
{
    uint32 i              = 0;
    uint32 j              = 0;
    uint8  n              = 0;
    uint32 x              = 0;
    uint32 y              = 0;
    uint32 result         = 0;
    
    uint16 *buf_ptr 	  = PNULL;
    uint16 *old_buf_ptr   = PNULL;
    uint16 *new_buf_ptr   = PNULL;
    
    uint16 *tmp_new_ptr   = PNULL;
    uint16 *tmp_old_ptr   = PNULL;
    uint16 *alpha_buf_ptr = PNULL;
    uint16 *tmp_buf_ptr   = PNULL;    
    
    GUI_RECT_T rect       = MMITHEME_GetFullScreenRect();
    uint32  frame_num     = GetSpecialEffectFrameNum(MMI_WIN_ANIM_RIPPLE);
	
    uint8 alpha_step      = 255/frame_num;
    uint8 alpha           = 0;

    uint16 lcd_width      = rect.right  - rect.left + 1;
    uint16 lcd_height     = rect.bottom - rect.top + 1;
    GUI_POINT_T rpoint    = {0};        

    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
    MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        return;        				
    }
    
#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    CreateSlideRippleEffect();
    
    if ((0 == s_ripple_effect.ripple_handle) || (PNULL == s_ripple_effect.init_param.src_pixels))
    {
        return;
    }
    
    //CreateRippleEffect(FALSE);
    
    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect );

    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY( new_buf_ptr, buf_ptr, SCR_SIZE );
    
    for (j = 0; j < 7; j++)
    {
       rpoint.x = (j+1)*lcd_width/8;
       rpoint.y = (j+1)*lcd_height/8;
       SaveRippleTPPoint(rpoint);
    }

    for (j = 0; j < 7; j++)
    {
       rpoint.x = (j+1)*lcd_width/8;
       rpoint.y = (7-j)*lcd_height/8;
       SaveRippleTPPoint(rpoint);
    }    

    rpoint.x = lcd_width/8;
    rpoint.y = lcd_height/2;
    SaveRippleTPPoint(rpoint);    

    rpoint.x = lcd_width/2;
    rpoint.y = lcd_height/8;
    SaveRippleTPPoint(rpoint);

    rpoint.x = lcd_width*7/8;
    rpoint.y = lcd_height/2;
    SaveRippleTPPoint(rpoint);

    rpoint.x = lcd_width/2;
    rpoint.y = lcd_height* 7/8;
    SaveRippleTPPoint(rpoint);

    tmp_buf_ptr = s_ripple_effect.init_param.src_pixels;
    s_ripple_effect.init_param.src_pixels = old_buf_ptr;

    for (j = 0; j < 32; j++)
    {
        RenderRippleEffect(FALSE);  
    }

    s_ripple_effect.init_param.src_pixels = tmp_buf_ptr;
    
    for (i = 1; i < frame_num; i++)
    {          
        alpha += alpha_step;
        alpha_buf_ptr = s_ripple_effect.init_param.src_pixels;
        tmp_new_ptr = new_buf_ptr;
        tmp_old_ptr = old_buf_ptr;
        
        for(j = 0; j < SCR_SIZE >> 1; j++)
        {
            x = *tmp_new_ptr++;
            y = *tmp_old_ptr++;
            n = (alpha + 1) >> 3;
            
            x = ( x | (x << 16)) & 0x7E0F81F;
            y = ( y | (y << 16)) & 0x7E0F81F;
            result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
            *alpha_buf_ptr++ = (uint16)((result & 0xFFFF) | (result >> 16));
        }
 
        RenderRippleEffect(FALSE); 
        RenderRippleEffect(FALSE);         
        RenderRippleEffect(TRUE);        
        SCI_SLEEP(10);        
    }

    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    GUILCD_InvalidateRectOnly(lcd_id, rect, 0 );

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif   

    MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
    DestroyRippleEffect(FALSE);
}
#endif /*ripple support */

#ifdef EFFECT_ZOOM_SUPPORT
/*****************************************************************************/
//  Description:   play 3d zoom in,then zoom out animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomInOutAnim(LCD_ID_E lcd_id)
{
    uint32 i = 0;
    // uint32 j = 0;
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 iIndex = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 lcd_width  = rect.right - rect.left + 1;
    uint32 lcd_height = rect.bottom - rect.top + 1;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    uint16  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_ZOOM_INOUT);
    uint16  in_frame_num  = frame_num;
    uint16  out_frame_num = frame_num;
  	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_1050_112_2_18_3_3_15_22,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect);

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    //SCI_MEMCPY(buf_ptr, old_buf_ptr, SCR_SIZE);

    //old buf zoom in
    for (iIndex = in_frame_num; iIndex > 1; iIndex--)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};

        scale_in.src_format           = IMGREF_FORMAT_RGB565;
        scale_in.src_chn.chn0.ptr     = old_buf_ptr;
        scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 2 ;
        scale_in.src_size.w           = lcd_width;
        scale_in.src_size.h           = lcd_height;        
        scale_in.src_trim_rect.x      = 0;
        scale_in.src_trim_rect.y      = 0;
        scale_in.src_trim_rect.w      = lcd_width;
        scale_in.src_trim_rect.h      = lcd_height;
        scale_in.target_buf.size      = lcd_width * (iIndex - 1)/in_frame_num * lcd_height * (iIndex - 1)/in_frame_num * 2 + 255;          
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);  

        if (PNULL == scale_in.target_buf.ptr)
        {
            break;
        }

        SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
        
        scale_in.target_format        = IMGREF_FORMAT_RGB565;
        scale_in.target_size.w        = lcd_width * (iIndex - 1)/in_frame_num;
        scale_in.target_size.h        = lcd_height * (iIndex - 1)/in_frame_num;

        SCI_MEMSET(buf_ptr, 0x00, SCR_SIZE);

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32 offset_x = (lcd_width  - scale_out.output_size.w)/2;
            uint32 offset_y = (lcd_height - scale_out.output_size.h)/2;
            uint16 *bk_ptr   = (uint16*)buf_ptr;
            uint16 *for_ptr  = (uint16*)scale_out.output_chn.chn0.ptr;

            bk_ptr +=  offset_y * lcd_width + offset_x;
            
            for (i = 0; i < scale_out.output_size.h; i++)
            {
                SCI_MEMCPY( bk_ptr, for_ptr, scale_out.output_size.w * sizeof(uint16) );

                bk_ptr  += lcd_width;
                for_ptr += scale_out.output_size.w;
            }
        }

        SCI_FREE(scale_in.target_buf.ptr);

        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_ZOOM_INOUT));
    }

    //next buffer zoom out.
    for (iIndex = 0; iIndex < out_frame_num; iIndex++)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};

        scale_in.src_format           = IMGREF_FORMAT_RGB565;
        scale_in.src_chn.chn0.ptr     = new_buf_ptr;
        scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 2 ;
        scale_in.src_size.w           = lcd_width;
        scale_in.src_size.h           = lcd_height;        
        scale_in.src_trim_rect.x      = 0;
        scale_in.src_trim_rect.y      = 0;
        scale_in.src_trim_rect.w      = lcd_width;
        scale_in.src_trim_rect.h      = lcd_height;
        scale_in.target_buf.size      = lcd_width * (iIndex + 1)/out_frame_num * lcd_height * (iIndex + 1)/out_frame_num * 2 + 255;          
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);  

        if (PNULL == scale_in.target_buf.ptr)
        {
            //restore buf_ptr
            SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
            GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif  
            return;
        }
        
        SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
        
        scale_in.target_format        = IMGREF_FORMAT_RGB565;
        scale_in.target_size.w        = lcd_width * (iIndex + 1)/out_frame_num;
        scale_in.target_size.h        = lcd_height * (iIndex + 1)/out_frame_num;

        SCI_MEMSET(buf_ptr, 0x00, SCR_SIZE); 

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32 offset_x = (lcd_width  - scale_out.output_size.w)/2;
            uint32 offset_y = (lcd_height - scale_out.output_size.h)/2;
            uint16 *bk_ptr   = (uint16*)buf_ptr;
            uint16 *for_ptr  = (uint16*)scale_out.output_chn.chn0.ptr;

            bk_ptr +=  offset_y * lcd_width + offset_x;
            
            for (i = 0; i < scale_out.output_size.h; i++)
            {
                SCI_MEMCPY( bk_ptr, for_ptr, scale_out.output_size.w * sizeof(uint16) );

                bk_ptr  += lcd_width;
                for_ptr += scale_out.output_size.w;
            }
        }

        SCI_FREE(scale_in.target_buf.ptr);

        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_ZOOM_INOUT));
    }
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif       
}

/*****************************************************************************/
//  Description:  play 3d zoom in animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomInNextAnim(LCD_ID_E lcd_id)
{
    uint32 i = 0;
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 iIndex = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 lcd_width  = rect.right - rect.left + 1;
    uint32 lcd_height = rect.bottom - rect.top + 1;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    uint16  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_ZOOM_IN);
  	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_1222_112_2_18_3_3_15_23,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect);

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    SCI_MEMCPY(buf_ptr, old_buf_ptr, SCR_SIZE);
    
    //anim
    for (iIndex = 3; iIndex < frame_num; iIndex++)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};

        scale_in.src_format           = IMGREF_FORMAT_RGB565;
        scale_in.src_chn.chn0.ptr     = new_buf_ptr;
        scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 2 ;
        scale_in.src_size.w           = lcd_width;
        scale_in.src_size.h           = lcd_height;        
        scale_in.src_trim_rect.x      = (lcd_width - lcd_width * (iIndex + 1)/frame_num)/2;
        scale_in.src_trim_rect.y      = (lcd_height - lcd_height * (iIndex + 1)/frame_num)/2;
        scale_in.src_trim_rect.w      = lcd_width * (iIndex + 1)/frame_num;
        scale_in.src_trim_rect.h      = lcd_height * (iIndex + 1)/frame_num;
        scale_in.target_buf.size      = lcd_width * lcd_height * 2 + 255;          
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);  

        if (PNULL == scale_in.target_buf.ptr)
        {
            //restore buf_ptr
            SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
            GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif  
            return;
        }
        
        SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
        
        scale_in.target_format        = IMGREF_FORMAT_RGB565;
        scale_in.target_size.w        = lcd_width;
        scale_in.target_size.h        = lcd_height;

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32 offset_x = (lcd_width  - scale_out.output_size.w)/2;
            uint32 offset_y = (lcd_height - scale_out.output_size.h)/2;
            uint16 *bk_ptr   = (uint16*)buf_ptr;
            uint16 *for_ptr  = (uint16*)scale_out.output_chn.chn0.ptr;

            bk_ptr +=  offset_y * lcd_width + offset_x;
            
            for (i = 0; i < scale_out.output_size.h; i++)
            {

                SCI_MEMCPY( bk_ptr, for_ptr, scale_out.output_size.w * sizeof(uint16) );

                bk_ptr  += lcd_width;
                for_ptr += scale_out.output_size.w;
            }
        }

        SCI_FREE(scale_in.target_buf.ptr);
     
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

        // SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_ZOOM_IN));

    }
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif       
}

/*****************************************************************************/
//  Description:   play 3d zoom out animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomOutAnim(LCD_ID_E lcd_id)
{
    uint32 i = 0;
    // uint32 j = 0;
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 iIndex = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 lcd_width  = rect.right - rect.left + 1;
    uint32 lcd_height = rect.bottom - rect.top + 1;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    uint16  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_ZOOM_OUT);
  	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_1339_112_2_18_3_3_16_24,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect);

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    SCI_MEMCPY(buf_ptr, old_buf_ptr, SCR_SIZE);
    
    //anim
    for (iIndex = 0; iIndex < frame_num; iIndex++)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};

        scale_in.src_format           = IMGREF_FORMAT_RGB565;
        scale_in.src_chn.chn0.ptr     = new_buf_ptr;
        scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 2 ;
        scale_in.src_size.w           = lcd_width;
        scale_in.src_size.h           = lcd_height;        
        scale_in.src_trim_rect.x      = 0;
        scale_in.src_trim_rect.y      = 0;
        scale_in.src_trim_rect.w      = lcd_width;
        scale_in.src_trim_rect.h      = lcd_height;
        scale_in.target_buf.size      = lcd_width * (iIndex + 1)/frame_num * lcd_height* (iIndex + 1)/frame_num * 2 + 255;          
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);  

        if (PNULL == scale_in.target_buf.ptr)
        {
            //restore buf_ptr
            SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
            GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif         
            return;
        }
        
        SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
        
        scale_in.target_format        = IMGREF_FORMAT_RGB565;
        scale_in.target_size.w        = lcd_width * (iIndex + 1)/frame_num;
        scale_in.target_size.h        = lcd_height* (iIndex + 1)/frame_num;

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32 offset_x = (lcd_width  - scale_out.output_size.w)/2;
            uint32 offset_y = (lcd_height - scale_out.output_size.h)/2;
            uint16 *bk_ptr   = (uint16*)buf_ptr;
            uint16 *for_ptr  = (uint16*)scale_out.output_chn.chn0.ptr;

            bk_ptr +=  offset_y * lcd_width + offset_x;
            
            for (i = 0; i < scale_out.output_size.h; i++)
            {
                SCI_MEMCPY( bk_ptr, for_ptr, scale_out.output_size.w * sizeof(uint16) );

                bk_ptr  += lcd_width;
                for_ptr += scale_out.output_size.w;
            }
        }

        SCI_FREE(scale_in.target_buf.ptr);
     
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_ZOOM_OUT));
    }
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif       
}

/*****************************************************************************/
//  Description:   play 3d zoom out animation with black background.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomOutAnimByBlackBg(LCD_ID_E lcd_id)
{
    uint32 i = 0;
    // uint32 j = 0;
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 iIndex = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 lcd_width  = rect.right - rect.left + 1;
    uint32 lcd_height = rect.bottom - rect.top + 1;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    uint16  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_ZOOM_OUT);
  	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_1454_112_2_18_3_3_16_25,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect);

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    SCI_MEMSET(buf_ptr, 0, SCR_SIZE);
    
    //anim
    for (iIndex = 0; iIndex < frame_num; iIndex++)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};

        scale_in.src_format           = IMGREF_FORMAT_RGB565;
        scale_in.src_chn.chn0.ptr     = new_buf_ptr;
        scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 2 ;
        scale_in.src_size.w           = lcd_width;
        scale_in.src_size.h           = lcd_height;        
        scale_in.src_trim_rect.x      = 0;
        scale_in.src_trim_rect.y      = 0;
        scale_in.src_trim_rect.w      = lcd_width;
        scale_in.src_trim_rect.h      = lcd_height;
        scale_in.target_buf.size      = lcd_width * (iIndex + 1)/frame_num * lcd_height* (iIndex + 1)/frame_num * 2 + 255;          
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);  

        if (PNULL == scale_in.target_buf.ptr)
        {
            //restore buf_ptr
            SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
            GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif          
            return;
        }
        
        SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
        
        scale_in.target_format        = IMGREF_FORMAT_RGB565;
        scale_in.target_size.w        = lcd_width * (iIndex + 1)/frame_num;
        scale_in.target_size.h        = lcd_height* (iIndex + 1)/frame_num;

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32 offset_x = (lcd_width  - scale_out.output_size.w)/2;
            uint32 offset_y = (lcd_height - scale_out.output_size.h)/2;
            uint16 *bk_ptr   = (uint16*)buf_ptr;
            uint16 *for_ptr  = (uint16*)scale_out.output_chn.chn0.ptr;

            bk_ptr +=  offset_y * lcd_width + offset_x;
            
            for (i = 0; i < scale_out.output_size.h; i++)
            {
                SCI_MEMCPY( bk_ptr, for_ptr, scale_out.output_size.w * sizeof(uint16) );

                bk_ptr  += lcd_width;
                for_ptr += scale_out.output_size.w;
            }
        }

        SCI_FREE(scale_in.target_buf.ptr);
     
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_ZOOM_OUT));
    }
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif       
}

/*****************************************************************************/
//  Description:   play 3d zoom in previous frame animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomInPrevAnim(LCD_ID_E lcd_id)
{
    uint32 i = 0;
    // uint32 j = 0;
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 iIndex = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 lcd_width  = rect.right - rect.left + 1;
    uint32 lcd_height = rect.bottom - rect.top + 1;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    uint16  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_ZOOM_IN);
  	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_1569_112_2_18_3_3_16_26,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect);

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    //SCI_MEMCPY(buf_ptr, old_buf_ptr, SCR_SIZE);
    
    //anim
    for (iIndex = frame_num; iIndex > 1; iIndex--)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};

        scale_in.src_format           = IMGREF_FORMAT_RGB565;
        scale_in.src_chn.chn0.ptr     = old_buf_ptr;
        scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 2 ;
        scale_in.src_size.w           = lcd_width;
        scale_in.src_size.h           = lcd_height;        
        scale_in.src_trim_rect.x      = 0;
        scale_in.src_trim_rect.y      = 0;
        scale_in.src_trim_rect.w      = lcd_width;
        scale_in.src_trim_rect.h      = lcd_height;
        scale_in.target_buf.size      = lcd_width * (iIndex - 1)/frame_num * lcd_height* (iIndex - 1)/frame_num * 2 + 255;          
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);  

        if (PNULL == scale_in.target_buf.ptr)
        {
            //restore buf_ptr
            SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
            GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif          
            return;
        }
        
        SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
        
        scale_in.target_format        = IMGREF_FORMAT_RGB565;
        scale_in.target_size.w        = lcd_width * (iIndex - 1)/frame_num;
        scale_in.target_size.h        = lcd_height* (iIndex - 1)/frame_num;

        SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32 offset_x = (lcd_width  - scale_out.output_size.w)/2;
            uint32 offset_y = (lcd_height - scale_out.output_size.h)/2;
            uint16 *bk_ptr   = (uint16*)buf_ptr;
            uint16 *for_ptr  = (uint16*)scale_out.output_chn.chn0.ptr;

            bk_ptr +=  offset_y * lcd_width + offset_x;
            
            for (i = 0; i < scale_out.output_size.h; i++)
            {
                SCI_MEMCPY( bk_ptr, for_ptr, scale_out.output_size.w * sizeof(uint16) );

                bk_ptr  += lcd_width;
                for_ptr += scale_out.output_size.w;
            }
        }

        SCI_FREE(scale_in.target_buf.ptr);
     
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_ZOOM_IN));
    }
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif       
}

/*****************************************************************************/
//  Description:   play 3d zoom out previous frame animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomOutPrevAnim(LCD_ID_E lcd_id)
{
    uint32 i = 0;
    // uint32 j = 0;
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 iIndex = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 lcd_width  = rect.right - rect.left + 1;
    uint32 lcd_height = rect.bottom - rect.top + 1;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    uint16  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_ZOOM_OUT);
  	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"PlayZoomInOutAnim old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_1686_112_2_18_3_3_16_27,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect);

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    //SCI_MEMCPY(buf_ptr, old_buf_ptr, SCR_SIZE);
    
    //anim
    for (iIndex = frame_num; iIndex > 1; iIndex--)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};

        scale_in.src_format           = IMGREF_FORMAT_RGB565;
        scale_in.src_chn.chn0.ptr     = old_buf_ptr;
        scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 2 ;
        scale_in.src_size.w           = lcd_width;
        scale_in.src_size.h           = lcd_height; 

        scale_in.src_trim_rect.x      = (lcd_width - lcd_width * (iIndex - 1)/frame_num)/2;
        scale_in.src_trim_rect.y      = (lcd_height - lcd_height * (iIndex - 1)/frame_num)/2;
        scale_in.src_trim_rect.w      = lcd_width * (iIndex - 1)/frame_num;
        scale_in.src_trim_rect.h      = lcd_height * (iIndex - 1)/frame_num;
       
        scale_in.target_buf.size      = lcd_width * lcd_height * 2 + 255;          
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size); 

        if (PNULL == scale_in.target_buf.ptr)
        {
            //restore buf_ptr
            SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
            GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif          
            return;
        }
        
        SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);
        
        scale_in.target_format        = IMGREF_FORMAT_RGB565;
        scale_in.target_size.w        = lcd_width;
        scale_in.target_size.h        = lcd_height;

        SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32 offset_x = (lcd_width  - scale_out.output_size.w)/2;
            uint32 offset_y = (lcd_height - scale_out.output_size.h)/2;
            uint16 *bk_ptr   = (uint16*)buf_ptr;
            uint16 *for_ptr  = (uint16*)scale_out.output_chn.chn0.ptr;

            bk_ptr +=  offset_y * lcd_width + offset_x;
            
            for (i = 0; i < scale_out.output_size.h; i++)
            {
                SCI_MEMCPY( bk_ptr, for_ptr, scale_out.output_size.w * sizeof(uint16) );


                bk_ptr  += lcd_width;
                for_ptr += scale_out.output_size.w;
            }
        }

        SCI_FREE(scale_in.target_buf.ptr);
     
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifdef WIN32
        SCI_Sleep(30);
#else
        SCI_Sleep(10);
#endif
    }
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);   
    GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif       
}

/*****************************************************************************/
//  Description:  play zoom in out.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomInOut(MMI_WIN_ANIM_PARAM_T* param)
{
    if (param->is_back)
    {
        PlayZoomOutPrevAnim(param->lcd_id);
    }
    else
    {
        PlayZoomInOutAnim(param->lcd_id);
    }
}

/*****************************************************************************/
//  Description:  play zoom in.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomIn(MMI_WIN_ANIM_PARAM_T* param)
{
    if (param->is_back)
    {
        PlayZoomInPrevAnim(param->lcd_id);
    }
    else
    {
        PlayZoomInNextAnim(param->lcd_id);
    } 
}

/*****************************************************************************/
//  Description:  play zoom out.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomOut(MMI_WIN_ANIM_PARAM_T* param)
{
    if (param->is_back)
    {
        PlayZoomOutAnimByBlackBg(param->lcd_id);
    }
    else
    {
        PlayZoomOutAnim(param->lcd_id);
    }
}

/*****************************************************************************/
//  Description:  play zoom animation.
//    Global resource dependence:
//  Author: hongbo.lan
//    Note:
/*****************************************************************************/
LOCAL void PlayZoomAnim(MMI_WIN_ANIM_PARAM_T* param)
{
    switch( param->type)
    {
    case MMI_WIN_ANIM_ZOOM_IN:
        if (param->is_back)
        {
            PlayZoomOut(param);
        }
        else
        {
            PlayZoomIn(param);
        }
        break;

    case MMI_WIN_ANIM_ZOOM_OUT:
        if (param->is_back)
        {
            PlayZoomIn(param);
        }
        else
        {
            PlayZoomOut(param);
        }
        break;
    
    case MMI_WIN_ANIM_ZOOM_INOUT: 
        PlayZoomInOut(param);
        break;
        
    default:
        break;
    }
}
#endif /* zoom support */

#ifdef EFFECT_3D_ROTATE_SUPPORT
/*****************************************************************************/
//  Description:   invalidate a rect with special effect(3d rotate)
//    Global resource dependence:
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL void Play3DRotateAnim(
                             LCD_ID_E lcd_id,//ID of lcd to operate
                             uint32 iDirection
                             )
{
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 iIndex = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"MMITHEME_InvalidateRect_3DRotate old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_1897_112_2_18_3_3_17_28,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect(lcd_id, &rect);

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    //anim
    for (iIndex = 0; iIndex < GetSpecialEffectFrameNum(MMI_WIN_ANIM_3D); iIndex++)
    {
        SE_INPUT_PARAM_T    input_param = {0};
        SE_OUTPUT_PARAM_T   output_param = {0};

        SCI_MEMSET(buf_ptr, 0x00, SCR_SIZE);

        output_param.output_buf_ptr = buf_ptr;
        output_param.output_rect.left   = rect.left;
        output_param.output_rect.top    = rect.top;
        output_param.output_rect.right  = rect.right;
        output_param.output_rect.bottom = rect.bottom;

        input_param.direction = iDirection;
        input_param.frame_index = iIndex;
        input_param.key_color = 0x00;
        input_param.next_ptr = new_buf_ptr;
        input_param.next_rect = output_param.output_rect;
        input_param.menu_rect.left   = rect.left;
        input_param.menu_rect.top    = rect.top;
        input_param.menu_rect.right  = rect.right;
        input_param.menu_rect.bottom = rect.bottom;

        input_param.selected_ptr = old_buf_ptr;
        input_param.selected_rect = output_param.output_rect;
        
        SE_DesktopRotate(&input_param, &output_param);

        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);

        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_ROTATE));
    }
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,rect,0);    
    //SCI_MEMCPY(s_old_buf_arr, s_new_buf_arr, SCR_SIZE);
    //FreeTrans3DBuf();

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif       
}
#endif

#ifdef EFFECT_WIN_SLIDE_SUPPORT
/*****************************************************************************/
//  Description:   play hor slide anim
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayHorSlideAnim(
                           LCD_ID_E   lcd_id,
                           uint16*    dst_ptr,
                           uint16*    src_ptr,
                           uint16     buf_width,
                           uint16     buf_height,
                           BOOLEAN    is_rtl //right to left
                           )
{
    uint32   i = 0;
    uint32   j = 0;
    uint32  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_HOR_SLIDE);
    uint16  dst_start_pos = 0;
    uint16  src_start_pos = 0;
    uint16  copy_width = 0;
    uint16* temp_dst_ptr = PNULL;
    uint16* temp_src_ptr = PNULL;
    GUI_RECT_T rect = {0};

    rect.right = buf_width - 1;
    rect.bottom = buf_height - 1;

    if(rect.right <= 0 || rect.bottom <= 0)
    {
        return;
    }
    if(frame_num <= 1)
    {
        return;
    }
    //anim
    for ( i = 1; i <= frame_num - 1; i++ )
    {
        copy_width = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, buf_width, frame_num, i);
        
        if ( is_rtl )
        {
            dst_start_pos = buf_width - copy_width;
        }
        else
        {
            src_start_pos = buf_width - copy_width;
        }

        temp_dst_ptr = dst_ptr + dst_start_pos;
        temp_src_ptr = src_ptr + src_start_pos;

        for( j = 0; j < buf_height; j++ )
        {
            //n += buf_width;
            SCI_MEMCPY( temp_dst_ptr, temp_src_ptr, copy_width * sizeof(uint16));

            temp_dst_ptr += buf_width;
            temp_src_ptr += buf_width;
        }
        
        GUILCD_InvalidateRectOnly( lcd_id, rect, 0 );
        
        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_HOR_SLIDE));
    }
}

/*****************************************************************************/
//  Description:  play ver slide anim
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayVerSlideAnim(
                            LCD_ID_E   lcd_id,
                            uint16*    dst_ptr,
                            uint16*    src_ptr,
                            uint16     buf_width,
                            uint16     buf_height,
                            BOOLEAN    is_btt //bottom to top
                            )
{
    uint32   i = 0;
    uint32   j = 0;
    uint32  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_VER_SLIDE);
    uint16  dst_start_pos = 0;
    uint16  src_start_pos = 0;
    uint16  copy_height   = 0;
    uint16* temp_dst_ptr  = PNULL;
    uint16* temp_src_ptr  = PNULL;
    GUI_RECT_T rect = {0};

    rect.right = buf_width - 1;
    rect.bottom = buf_height - 1;
    if(rect.right <= 0 || rect.bottom <= 0)
    {
        return;
    }

    if(frame_num <= 1)
    {
        return;
    }    
    //anim
    for ( i = 0; i < frame_num - 1; i++ )
    {
        copy_height = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, buf_height, frame_num, i);
        if ( is_btt )
        {
            dst_start_pos = buf_height - copy_height;
        }
        else
        {
            src_start_pos = buf_height - copy_height;
        }

        temp_dst_ptr = dst_ptr + dst_start_pos * buf_width;
        temp_src_ptr = src_ptr + src_start_pos * buf_width;

        for( j = 0; j < copy_height; j++ )
        {
            //n += buf_width;
            SCI_MEMCPY( temp_dst_ptr, temp_src_ptr, buf_width * sizeof(uint16));

            temp_dst_ptr += buf_width;
            temp_src_ptr += buf_width;
        }
        
        GUILCD_InvalidateRectOnly( lcd_id, rect, 0 );
        
        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_VER_SLIDE));
    }
}

/*****************************************************************************/
//  Description:  play hor slide anim
//  Global resource dependence:
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void PlayHorSlideAnimWithOld(
                                   LCD_ID_E   lcd_id,
                                   uint16*    dst_ptr,
                                   uint16*    new_ptr,
                                   uint16*    old_ptr,
                                   uint16     buf_width,
                                   uint16     buf_height,
                                   BOOLEAN    is_r2l   //right to left
                                   )
{
    uint32      frame_num       = GetSpecialEffectFrameNum(MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD);
    uint32      i               = 0; 
    uint32      j               = 0;
    int32       step            = 0; // buf_width/frame_num;
    uint16      *temp_dst_ptr   = PNULL;
    uint16      *temp_ptr1      = PNULL;
    uint16      *temp_ptr2      = PNULL;
    uint16      wid1            = 0;
    uint16      wid2            = 0;
    GUI_RECT_T  rect            = {0};

    rect.right = buf_width - 1;
    rect.bottom = buf_height - 1;

    if(rect.right <= 0 || rect.bottom <= 0)
    {
        return;
    }
    
    if(frame_num <= 1)
    {
        return;
    }

    for ( i = 1; i <= frame_num; i++ )
    {    	
        step = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_5, buf_width, frame_num, i);

        if(is_r2l)
        {
            wid2 = step;
            wid1 = buf_width - step;
            
            temp_ptr1 = old_ptr + step;
            temp_ptr2 = new_ptr;
        }
        else
        {
            wid1 = step;
            wid2 = buf_width - wid1;
            temp_ptr1 = new_ptr + wid2;
            temp_ptr2 = old_ptr;
            
        }
        temp_dst_ptr = dst_ptr;

        for(j = 0; j < buf_height; j++)
        {
            SCI_MEMCPY(temp_dst_ptr, temp_ptr1, wid1<<1);
            SCI_MEMCPY(temp_dst_ptr + wid1, temp_ptr2, wid2<<1);

            temp_ptr1 += buf_width;
            temp_ptr2 += buf_width;
            temp_dst_ptr += buf_width;
        }

        GUILCD_InvalidateRectOnly( lcd_id, rect, 0 );    
        
        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD));
    }
}

/*****************************************************************************/
//  Description:  play ver slide anim with old screen following
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayVerSlideAnimWithOld(
                            LCD_ID_E   lcd_id,
                            uint16*    dst_ptr,
                            uint16*    new_ptr,
                            uint16*    old_ptr,
                            uint16     buf_width,
                            uint16     buf_height,
                            BOOLEAN    is_t2b //top to bottom
                            )
{
    uint32   i = 0;
    uint32  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_VER_SLIDE_WITH_OLD);
    uint16  height1 = 0, height2 = 0;
    uint16* temp_ptr1  = PNULL;
    uint16* temp_ptr2  = PNULL;
    GUI_RECT_T rect = {0};

    rect.right = buf_width - 1;
    rect.bottom = buf_height - 1;
    if(rect.right <= 0 || rect.bottom <= 0)
    {
        return;
    }

    //anim
    for ( i = 1; i <= frame_num - 1; i++ )
    {
        if(is_t2b)
        {
            height1 = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_5, buf_height, frame_num, i);
            height2 = buf_height - height1;
            temp_ptr1 = new_ptr + (height2 * buf_width);
            temp_ptr2 = old_ptr;
        }
        else
        {
            height2 = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_5, buf_height, frame_num, i);
            height1 = buf_height - height2;
            temp_ptr1 = old_ptr + (height2 * buf_width);
            temp_ptr2 = new_ptr;
        }
        SCI_MEMCPY(dst_ptr, temp_ptr1, height1 * (buf_width << 1));        
        SCI_MEMCPY(dst_ptr + height1 * buf_width, temp_ptr2, height2 * (buf_width << 1));

        GUILCD_InvalidateRectOnly( lcd_id, rect, 0 );
        
        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_VER_SLIDE_WITH_OLD));
    }
}

/*****************************************************************************/
//  Description:   play slide anim
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlaySlideAnim(
                         MMI_WIN_ANIM_PARAM_T* param
                         )
{
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint16 buf_height   = 0;
    // uint32 i            = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
	
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        return;        				
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect( param->lcd_id, &rect );

    GUILCD_GetLogicWidthHeight( param->lcd_id, &buf_width, &buf_height );
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY( new_buf_ptr, buf_ptr, SCR_SIZE );
    SCI_MEMCPY( buf_ptr, old_buf_ptr, SCR_SIZE );

    switch( param->type )
    {
    case MMI_WIN_ANIM_HOR_SLIDE:
        PlayHorSlideAnim( param->lcd_id, buf_ptr, new_buf_ptr, buf_width, buf_height, (BOOLEAN)!param->is_back );
    	break;
    	
    case MMI_WIN_ANIM_VER_SLIDE:
        PlayVerSlideAnim( param->lcd_id, buf_ptr, new_buf_ptr, buf_width, buf_height, (BOOLEAN)!param->is_back );
    	break;
    	
    case MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD:
        PlayHorSlideAnimWithOld(param->lcd_id, buf_ptr, new_buf_ptr, old_buf_ptr, buf_width, buf_height, (BOOLEAN)!param->is_back);
        break;
        
    case MMI_WIN_ANIM_VER_SLIDE_WITH_OLD:
        PlayVerSlideAnimWithOld(param->lcd_id, buf_ptr, new_buf_ptr, old_buf_ptr, buf_width, buf_height, (BOOLEAN)!param->is_back);
        break;
        
    default:
        break;
    }
    
    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif   
}

#endif

/*****************************************************************************/
//  Description:  play hor slide anim, old screen slide too.
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
                                             )
{
#ifdef EFFECT_WIN_SLIDE_SUPPORT
    PlayHorSlideAnimWithOld(lcd_id, dst_ptr, new_ptr, old_ptr, buf_width, buf_height, is_r2l);
#endif
}

#if 0//def  EFFECT_WIN_FADE_SUPPORT
/*****************************************************************************/
//  Description:  play ver slide anim with old screen following
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void PlayFadeAnim(MMI_WIN_ANIM_PARAM_T* param)
{
    uint8  n = 0;
    uint32 result = 0;
    uint32 x = 0;
    uint32 y = 0;

    uint16 *buf_ptr 	= PNULL;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    
    uint16 *tmp_new_ptr = PNULL;
    uint16 *tmp_old_ptr = PNULL;
    uint16 *alpha_buf_ptr = PNULL;
    
    uint32 i = 0,j = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_FADE);
	
    uint8 alpha_step = 255/frame_num;
    uint8 alpha = 0;
    
    
    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
    MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        return;        				
    }

    if(frame_num <= 1)
    {
        return;
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect( param->lcd_id, &rect );

    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY( new_buf_ptr, buf_ptr, SCR_SIZE );

    for (i = 1; i < frame_num; i++)
    {            
        alpha += alpha_step;
        alpha_buf_ptr = buf_ptr;
        tmp_new_ptr = new_buf_ptr;
        tmp_old_ptr = old_buf_ptr;
        n = (alpha + 1) >> 3;

        for(j = 0; j < SCR_SIZE >> 1; j++)
        {
            x = *tmp_new_ptr++;
            y = *tmp_old_ptr++;
            
            x = ( x | (x << 16)) & 0x7E0F81F;
            y = ( y | (y << 16)) & 0x7E0F81F;
            result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
            *alpha_buf_ptr++ = (uint16)((result & 0xFFFF) | (result >> 16));
        }
        
        GUILCD_InvalidateRectOnly( param->lcd_id, rect, 0 );

        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_FADE));
    }

    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif   

}
#endif

/*****************************************************************************/
//  Description:   invalidate a rect with special effect(3d rotate)
//    Global resource dependence:
//  Author: james.zhang
//    Note: 如果动画最后一帧播放慢，则肯定是合层导致，可以拷贝oldbuffer后GUILCD_InvalidateRectOnly,
//          然后呼叫MMITHEME_InitUpdate使mmimain主循环最后不刷新
/*****************************************************************************/
PUBLIC void MMITHEME_PlayWinAnim(
                                 MMI_WIN_ANIM_PARAM_T* param
                                 )
{
    if ( PNULL == param || MMK_IsQuickKey() )
    {
        return;
    }

    // 判断视频墙纸是否打开？
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    if ( MMIAPISETVWP_IsOpen() && MMIAPISETVWP_IsVideoLayerEnable() )
    {
        return;
    }
#endif

    switch( param->type )
    {
#ifdef UI_P3D_SUPPORT    

#ifdef EFFECT_WIN_SLIDE_SUPPORT
    case MMI_WIN_ANIM_HOR_SLIDE:
    case MMI_WIN_ANIM_VER_SLIDE:
    case MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD:
    case MMI_WIN_ANIM_VER_SLIDE_WITH_OLD:
        PlaySlideAnim( param );
    	break;
#endif

#if 0//def EFFECT_WIN_FADE_SUPPORT
    case MMI_WIN_ANIM_FADE:
        PlayFadeAnim(param);
        break;
#endif

#ifdef EFFECT_3D_ROTATE_SUPPORT		
    case MMI_WIN_ANIM_3D:
        Play3DRotateAnim(param->lcd_id, param->is_back );
        break;
#endif

#ifdef EFFECT_ZOOM_SUPPORT        
    case MMI_WIN_ANIM_ZOOM_IN:
    case MMI_WIN_ANIM_ZOOM_OUT:
    case MMI_WIN_ANIM_ZOOM_INOUT: 
        PlayZoomAnim(param);
        break;
#endif		
#endif

    default:
#ifdef UI_P3D_SUPPORT   
    {
        if(MMI_WIN_ANIM_SE_TYPE_START < param->type && MMI_WIN_ANIM_SE_TYPE_END > param->type)
        {
            MMIWINSE_Play(param);
        }
    }
#endif
        break;
    }
}

#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
//  Description:   Get LCD rotate direction.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL ROTATE_DIR_E GetLCDRotateDir(LCD_ANGLE_E cur_angle, LCD_ANGLE_E old_angle)
{
#define TRAN_ANGLE(val, angle)  \
    switch(angle)   \
    {   \
    case LCD_ANGLE_0:   \
        val = 0;    \
        break;      \
    case LCD_ANGLE_90:  \
        val = 1;    \
        break;      \
    case LCD_ANGLE_180: \
        val = 2;    \
        break;      \
    case LCD_ANGLE_270: \
        val = 3;    \
        break;      \
    default:        \
        val = 0;    \
    }
        
    ROTATE_DIR_E dir = ROTATE_NONE;
    int32 start_dir = 0;
    int32 end_dir  = 0;
    
    TRAN_ANGLE(start_dir, old_angle)
    TRAN_ANGLE(end_dir, cur_angle)

    switch(end_dir - start_dir)
    {
    case 0:
        dir = ROTATE_NONE;
        break;
    case 1:
    case -3:
        dir = ROTATE_CLOCKWISE;
        break;
    case -2:
    case 2:
        dir = ROTATE_180;
        break;
    case -1:
    case 3:
        dir = ROTATE_COUNTERCLOCKWISE;
        break;
    default:
        dir = ROTATE_NONE;
        break;
    }
    return dir;
}

#ifdef EFFECT_LCD_ROTATE_ASYNC_SUPPORT

/*****************************************************************************/
//  Description:   Set scale result.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void SetScaleRes(BOOLEAN is_over)
{
    s_scale_res = is_over;
}

/*****************************************************************************/
//  Description:   Get scale result.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL uint32 GetScaleRes(void)
{
    return s_scale_res;
}

/*****************************************************************************/
//  Description:   scaling callback func.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void ScaleCallBack(uint32 ret_value, SCALE_IMAGE_OUT_T *out_param_ptr, uint32 async_param)
{
    if(SCI_SUCCESS == ret_value)
    {
        SetScaleRes(SCALE_OK);
        s_scale_out = *out_param_ptr;
    }
    else
    {
        SetScaleRes(SCALE_ERROR);
    }
}

/*****************************************************************************/
//  Description:   Get scale out param.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL SCALE_IMAGE_OUT_T *GetScaleOutParam(void)
{
    return &s_scale_out;
}

/*****************************************************************************/
//  Description:   Get rotate buffer.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetRotateBuff(uint16 **buf1, uint16 **buf2)
{
    uint8 *alloc_buf_ptr = PNULL;
    alloc_buf_ptr = SCI_ALLOCA((SCR_SIZE + 256) * 2);
    if(alloc_buf_ptr != PNULL)
    {
        *buf1 = (uint16 *)alloc_buf_ptr;
        *buf2 = (uint16 *)(alloc_buf_ptr + SCR_SIZE + 256);
        SCI_MEMSET(alloc_buf_ptr, 0, (SCR_SIZE + 256) * 2);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description:   Get time duation, this function is just for testing the rotate speed.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL uint32 GetTime(uint32 *tick)
{
    uint32 tick_count = 0;
    tick_count = *tick;
    *tick = SCI_GetTickCount();
    return *tick - tick_count;
}

/*****************************************************************************/
//  Description:   Play LCD switch effect rotate.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PlayLCDSwitchEffectRotate( 
                                MMI_WIN_ANIM_PARAM_T *param, 
                                LCD_ANGLE_E cur_angle, 
                                LCD_ANGLE_E old_angle
                                )
{
#define GET_SCALE_OUT_PARAM(ptr, i)    \
    if(((i)%2) != 0)  \
    {   \
        ptr = &scale_out;   \
    }   \
    else    \
    {   \
        ptr = &scale_out2;  \
    }

#define GET_SCALE_IN_BUF(buf_ptr, i)    \
    if(((i) % 2) != 0)    \
    {   \
        buf_ptr = alloc_buf_ptr1;   \
    }   \
    else    \
    {   \
        buf_ptr = alloc_buf_ptr2;   \
    }
        
    uint16 *lcd_buf_ptr 	= PNULL;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    
    uint16 *tmp_old_ptr = PNULL;
    uint16 *tmp_lcd_buf_ptr = PNULL;
    uint16 *alloc_buf_ptr1 = PNULL;
    uint16 *alloc_buf_ptr2 = PNULL;

    uint32 i = 0,j = 0;
    uint32 frm_index = 0;
    uint32 scale_index = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_LCD_SWITCH_ROTATE);
	
    uint16 new_width = 0;
    uint16 new_height = 0;
    uint16 old_width = 0;
    uint16 old_height = 0;

    ROTATE_DIR_E rotate_dir = ROTATE_NONE;
    ROTATE_ARBITRARY_IN_T rotate_in_ptr = {0};
    ROTATE_ARBITRARY_OUT_T rotate_out_ptr ={0};

    SCALE_IMAGE_IN_T    scale_in  = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};
    SCALE_IMAGE_OUT_T   scale_out2 = {0};
    SCALE_IMAGE_OUT_T   *scale_out_ptr = PNULL;

    uint32 effect_scale_step = 0;
    BOOLEAN is_effect_scale_w = FALSE;
    uint32 scale_res  = SCI_SUCCESS;
    uint32 tick = SCI_GetTickCount();
    uint32 tick2  = tick;
    rotate_dir = GetLCDRotateDir(cur_angle, old_angle);
    if(rotate_dir != ROTATE_CLOCKWISE && rotate_dir != ROTATE_COUNTERCLOCKWISE && rotate_dir != ROTATE_180)
    {
        return;
    }

    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }

    MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);
    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        return;        				
    }

    if(GetRotateBuff(&alloc_buf_ptr1, &alloc_buf_ptr2))
    {
        if(PNULL == alloc_buf_ptr1 || PNULL == alloc_buf_ptr2)
        {
            return;
        }
    }
    else
    {
        return;
    }
    
    GUILCD_GetLogicWidthHeight( param->lcd_id, &new_width, &new_height );

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    /* Get new screen data */
    UILAYER_InvalidateRect( param->lcd_id, &rect );
    lcd_buf_ptr = GUILCD_GetMainLcdBufPtr();
    SCI_MEMCPY( new_buf_ptr, lcd_buf_ptr, SCR_SIZE );
    
    tmp_lcd_buf_ptr = lcd_buf_ptr;    
    switch(rotate_dir)
    {
    case ROTATE_CLOCKWISE:
        old_width = new_height;
        old_height = new_width;
        for(i = 0; i < new_height; i++)
        {
            tmp_old_ptr = old_buf_ptr + old_width - 1 - i;
            for(j = 0; j < new_width; j++)
            {
                *tmp_lcd_buf_ptr++ = *tmp_old_ptr;
                tmp_old_ptr += old_width;
            }
        }
        SCI_MEMCPY( old_buf_ptr, lcd_buf_ptr, SCR_SIZE );
        break;
    case ROTATE_COUNTERCLOCKWISE:
        old_width = new_height;
        old_height = new_width;                
        for(i = 0; i < new_height; i++)
        {
            tmp_old_ptr = old_buf_ptr + (old_height- 1) * old_width + i;
            for(j = 0; j < new_width; j++)
            {
                *tmp_lcd_buf_ptr++ = *tmp_old_ptr;
                tmp_old_ptr -= old_width;
            }
        }
        SCI_MEMCPY( old_buf_ptr, lcd_buf_ptr, SCR_SIZE );
        break;
    case ROTATE_180:
        old_height = new_height;
        old_width = new_width;                
        tmp_old_ptr = old_buf_ptr + old_width*old_height - 1;
        for(j = 0; j < SCR_SIZE >> 1; j++)
        {
            *tmp_lcd_buf_ptr++ = *tmp_old_ptr;
            tmp_old_ptr--;
        }
        SCI_MEMCPY( old_buf_ptr, lcd_buf_ptr, SCR_SIZE );
        break;
    default:
        break;
    }

    rotate_in_ptr.src_size.w = new_width;
    rotate_in_ptr.src_size.h = new_height;
    rotate_in_ptr.src_format = IMGREF_FORMAT_RGB565;
    
    rotate_in_ptr.target_size.w = new_width;
    rotate_in_ptr.target_size.h = new_height;
    rotate_in_ptr.target_format = IMGREF_FORMAT_RGB565;
    rotate_in_ptr.target_chn.chn0.ptr = lcd_buf_ptr;
    rotate_in_ptr.target_chn.chn0.size = SCR_SIZE;
    rotate_in_ptr.target_center.x = rotate_in_ptr.target_size.w/2;
    rotate_in_ptr.target_center.y = rotate_in_ptr.target_size.h/2;
    rotate_in_ptr.speed_up = TRUE;

    scale_in.src_chn.chn0.ptr     = old_buf_ptr;
    scale_in.src_chn.chn0.size    = SCR_SIZE;
    scale_in.src_format           = IMGREF_FORMAT_RGB565;
    scale_in.src_size.w           = new_width;
    scale_in.src_size.h           = new_height;
    
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = new_width;
    scale_in.src_trim_rect.h      = new_height;
    
    scale_in.scale_mode           = SCALE_MODE_DISTORT;
    
    scale_in.target_buf.size      = SCR_SIZE;          
    scale_in.target_format        = IMGREF_FORMAT_RGB565;
    
    if(rotate_dir != ROTATE_180)
    {
        effect_scale_step = (uint32)(abs(new_height - new_width))/(frame_num/2);
        if(new_width > new_height)
        {
            is_effect_scale_w = TRUE;
        }
        else
        {
            is_effect_scale_w = FALSE;
        }
    }

    frm_index = 1;
    GET_SCALE_OUT_PARAM(scale_out_ptr, frm_index);      /*lint !e774*/
    GET_SCALE_IN_BUF(scale_in.target_buf.ptr, frm_index);   /*lint !e774*/

    //SCI_TRACE_LOW:"[ROTATE]Prepare finished, time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2805_112_2_18_3_3_19_29,(uint8*)"d", GetTime(&tick));

    /* scale buff before rotate */
    if(is_effect_scale_w)
    {
        scale_in.target_size.w = new_width - effect_scale_step * frm_index;
        scale_in.target_size.h = new_height;
        scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
    }
    else
    {
        scale_in.target_size.w = new_width;
        scale_in.target_size.h = new_height - effect_scale_step * frm_index;                
        scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
    }
    //SCI_TRACE_LOW:"[ROTATE]First sync scale,time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2820_112_2_18_3_3_19_30,(uint8*)"d", GetTime(&tick));

    /* Scale Async */
    scale_in.async_callback = ScaleCallBack;    
    for (frm_index = 1; frm_index < frame_num / 2; frm_index++)
    {
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, Before scal, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2826_112_2_18_3_3_19_31,(uint8*)"d", GetTime(&tick));
        
        scale_index = frm_index + 1;
        SCI_MEMSET( lcd_buf_ptr, 0, SCR_SIZE );
        
        GET_SCALE_OUT_PARAM(scale_out_ptr, scale_index);
        GET_SCALE_IN_BUF(scale_in.target_buf.ptr, scale_index);
        SetScaleRes(SCALE_WAITING);
        if(scale_index < frame_num / 2 )
        {
            if(is_effect_scale_w)
            {
                scale_in.target_size.w = new_width - effect_scale_step * scale_index;
                scale_in.target_size.h = new_height;
                scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
            }
            else
            {
                scale_in.target_size.w = new_width;
                scale_in.target_size.h = new_height - effect_scale_step * scale_index;                
                scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
            }            
        }
        else
        {
            scale_in.src_chn.chn0.ptr = new_buf_ptr;
            if(is_effect_scale_w)
            {
                scale_in.target_size.w = new_height + effect_scale_step * (scale_index - frame_num/2);
                scale_in.target_size.h = new_height;
                scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
            }
            else
            {
                scale_in.target_size.w = new_width;
                scale_in.target_size.h = new_width + effect_scale_step * (scale_index - frame_num/2);                
                scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
            }
        }
        
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, Async scale, time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2866_112_2_18_3_3_19_32,(uint8*)"d", GetTime(&tick));
        
        if(1 == frm_index)
        {
            scale_out_ptr = &scale_out;
        }
        else
        {
            scale_out_ptr = GetScaleOutParam();
        }
        rotate_in_ptr.src_chn.chn0 = scale_out_ptr->output_chn.chn0;
        rotate_in_ptr.src_size.w = scale_out_ptr->output_size.w;
        rotate_in_ptr.src_size.h = scale_out_ptr->output_size.h;

        rotate_in_ptr.src_center.x = scale_out_ptr->output_size.w/2;
        rotate_in_ptr.src_center.y = scale_out_ptr->output_size.h/2;
        /* buff scale end */
    
        switch(rotate_dir)
        {
        case ROTATE_CLOCKWISE:
            rotate_in_ptr.angle = (90 * frm_index) / frame_num;
            break;
        case ROTATE_COUNTERCLOCKWISE:
            rotate_in_ptr.angle = 360 - (90 * frm_index) / frame_num;
            break;
        case ROTATE_180:
            rotate_in_ptr.angle = (180 * frm_index) / frame_num;
            break;
        default:
            break;
        }
        
        if(SCI_SUCCESS != GRAPH_RotateArbitrary(&rotate_in_ptr, &rotate_out_ptr))
        {
            break;
        }
        
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, rotate, time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2904_112_2_18_3_3_19_33,(uint8*)"d", GetTime(&tick));

        GUILCD_InvalidateRectOnly( param->lcd_id, rect, 0 );
        
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, refresh LCD, time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2908_112_2_18_3_3_19_34,(uint8*)"d", GetTime(&tick));

        if(scale_res == SCI_SUCCESS)
        {
            while(GetScaleRes() == SCALE_WAITING)
            {
                SCI_SLEEP(1);
            }    
        }
    }
    
    scale_in.src_chn.chn0.ptr = new_buf_ptr;
    for (frm_index = frame_num / 2; frm_index < frame_num; frm_index++)
    {
        scale_index = frm_index + 1;
    
        SCI_MEMSET( lcd_buf_ptr, 0, SCR_SIZE );

        SetScaleRes(SCALE_WAITING);
        
        if(scale_index < frame_num)
        {
            /* scale buff before rotate */
            GET_SCALE_OUT_PARAM(scale_out_ptr, scale_index);
            GET_SCALE_IN_BUF(scale_in.target_buf.ptr, scale_index);
            
            if(is_effect_scale_w)
            {
                scale_in.target_size.w = new_height + effect_scale_step * (scale_index - frame_num/2);
                scale_in.target_size.h = new_height;
                scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
            }
            else
            {
                scale_in.target_size.w = new_width;
                scale_in.target_size.h = new_width + effect_scale_step * (scale_index - frame_num/2);                
                scale_res = GRAPH_ScaleImage(&scale_in, scale_out_ptr);
            }
            
        }
        //SCI_TRACE_LOW:"[ROTATE]Be-Half, scale, time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2948_112_2_18_3_3_19_35,(uint8*)"d", GetTime(&tick));

        scale_out_ptr = GetScaleOutParam();        
        rotate_in_ptr.src_chn.chn0 = scale_out_ptr->output_chn.chn0;

        rotate_in_ptr.src_size.w = scale_out_ptr->output_size.w;
        rotate_in_ptr.src_size.h = scale_out_ptr->output_size.h;

        rotate_in_ptr.src_center.x = scale_out_ptr->output_size.w/2;
        rotate_in_ptr.src_center.y = scale_out_ptr->output_size.h/2;
        /* buff scale end */
        
        switch(rotate_dir)
        {
        case ROTATE_CLOCKWISE:
            rotate_in_ptr.angle =  360 - (90 * (frame_num - frm_index)) / frame_num;
            break;
        case ROTATE_COUNTERCLOCKWISE:
            rotate_in_ptr.angle = (90 * (frame_num - frm_index)) / frame_num;
            break;
        case ROTATE_180:
            rotate_in_ptr.angle =  360 - (180 * (frame_num - frm_index)) / frame_num;
            break;
        default:
            break;
        }
        
        if(SCI_SUCCESS != GRAPH_RotateArbitrary(&rotate_in_ptr, &rotate_out_ptr))
        {
            break;
        }
        //SCI_TRACE_LOW:"[ROTATE]Be-Half, rotate, time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2979_112_2_18_3_3_19_36,(uint8*)"d", GetTime(&tick));

        GUILCD_InvalidateRectOnly( param->lcd_id, rect, 0 );
        //SCI_TRACE_LOW:"[ROTATE]Be-Half, refresh LCD, time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2982_112_2_18_3_3_19_37,(uint8*)"d", GetTime(&tick));

        if(scale_index < frame_num)
        {
            if(scale_res == SCI_SUCCESS)
            {
                while(GetScaleRes() == SCALE_WAITING)
                {
                    SCI_SLEEP(1);
                }    
            }
        }
    }

    /* restore LCD buff */
    SCI_MEMCPY(lcd_buf_ptr, new_buf_ptr, SCR_SIZE);
    //SCI_TRACE_LOW:"[ROTATE]Restore lcd buf, time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_2998_112_2_18_3_3_19_38,(uint8*)"d", GetTime(&tick));

    SCI_FREE(alloc_buf_ptr1);
#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif

    //SCI_TRACE_LOW:"[ROTATE]whole time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3005_112_2_18_3_3_19_39,(uint8*)"d", SCI_GetTickCount() - tick2);

}

#else
/*****************************************************************************/
//  Description:   Play LCD switch effect rotate.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PlayLCDSwitchEffectRotate( 
                                MMI_WIN_ANIM_PARAM_T *param, 
                                LCD_ANGLE_E cur_angle, 
                                LCD_ANGLE_E old_angle
                                )
{
    uint16 *lcd_buf_ptr 	= PNULL;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    
    uint16 *tmp_old_ptr = PNULL;
    uint16 *tmp_lcd_buf_ptr = PNULL;
    uint16 *alloc_buf_ptr = PNULL;

    uint32 i = 0,j = 0;
    uint32 frm_index = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_LCD_SWITCH_ROTATE);
	
    uint16 new_width = 0;
    uint16 new_height = 0;
    uint16 old_width = 0;
    uint16 old_height = 0;

    ROTATE_DIR_E rotate_dir = ROTATE_NONE;
    ROTATE_ARBITRARY_IN_T rotate_in_ptr = {0};
    ROTATE_ARBITRARY_OUT_T rotate_out_ptr ={0};

    SCALE_IMAGE_IN_T    scale_in  = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};

    uint32 effect_scale_step = 0;
    BOOLEAN is_effect_scale_w = FALSE;
    uint32 scale_res  = SCI_SUCCESS;
    
    rotate_dir = GetLCDRotateDir(cur_angle, old_angle);
    if(rotate_dir != ROTATE_CLOCKWISE && rotate_dir != ROTATE_COUNTERCLOCKWISE && rotate_dir != ROTATE_180)
    {
        return;
    }

    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }

    MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);
    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        return;        				
    }

    alloc_buf_ptr = SCI_ALLOCA(SCR_SIZE + 256);  
    if(PNULL == alloc_buf_ptr)
    {
        return;
    }
    SCI_MEMSET(alloc_buf_ptr, 0, SCR_SIZE + 256);
    
    GUILCD_GetLogicWidthHeight( param->lcd_id, &new_width, &new_height );

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    /* Get new screen data */
    UILAYER_InvalidateRect( param->lcd_id, &rect );
    lcd_buf_ptr = GUILCD_GetMainLcdBufPtr();
    SCI_MEMCPY( new_buf_ptr, lcd_buf_ptr, SCR_SIZE );
    
    tmp_lcd_buf_ptr = lcd_buf_ptr;    
    switch(rotate_dir)
    {
    case ROTATE_CLOCKWISE:
        old_width = new_height;
        old_height = new_width;
        for(i = 0; i < new_height; i++)
        {
            tmp_old_ptr = old_buf_ptr + old_width - 1 - i;
            for(j = 0; j < new_width; j++)
            {
                *tmp_lcd_buf_ptr++ = *tmp_old_ptr;
                tmp_old_ptr += old_width;
            }
        }
        SCI_MEMCPY( old_buf_ptr, lcd_buf_ptr, SCR_SIZE );
        break;
    case ROTATE_COUNTERCLOCKWISE:
        old_width = new_height;
        old_height = new_width;                
        for(i = 0; i < new_height; i++)
        {
            tmp_old_ptr = old_buf_ptr + (old_height- 1) * old_width + i;
            for(j = 0; j < new_width; j++)
            {
                *tmp_lcd_buf_ptr++ = *tmp_old_ptr;
                tmp_old_ptr -= old_width;
            }
        }
        SCI_MEMCPY( old_buf_ptr, lcd_buf_ptr, SCR_SIZE );
        break;
    case ROTATE_180:
        old_height = new_height;
        old_width = new_width;                
        tmp_old_ptr = old_buf_ptr + old_width*old_height - 1;
        for(j = 0; j < SCR_SIZE >> 1; j++)
        {
            *tmp_lcd_buf_ptr++ = *tmp_old_ptr;
            tmp_old_ptr--;
        }
        SCI_MEMCPY( old_buf_ptr, lcd_buf_ptr, SCR_SIZE );
        break;
    default:
        break;
    }

    rotate_in_ptr.src_size.w = new_width;
    rotate_in_ptr.src_size.h = new_height;
    rotate_in_ptr.src_format = IMGREF_FORMAT_RGB565;
    
    rotate_in_ptr.target_size.w = new_width;
    rotate_in_ptr.target_size.h = new_height;
    rotate_in_ptr.target_format = IMGREF_FORMAT_RGB565;
    rotate_in_ptr.target_chn.chn0.ptr = lcd_buf_ptr;
    rotate_in_ptr.target_chn.chn0.size = SCR_SIZE;
    rotate_in_ptr.target_center.x = rotate_in_ptr.target_size.w/2;
    rotate_in_ptr.target_center.y = rotate_in_ptr.target_size.h/2;
    rotate_in_ptr.speed_up = TRUE;
    
    scale_in.src_chn.chn0.ptr     = old_buf_ptr;
    scale_in.src_chn.chn0.size    = SCR_SIZE;
    scale_in.src_format           = IMGREF_FORMAT_RGB565;
    scale_in.src_size.w           = new_width;
    scale_in.src_size.h           = new_height;
    
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = new_width;
    scale_in.src_trim_rect.h      = new_height;
    
    scale_in.scale_mode           = SCALE_MODE_DISTORT;
    
    scale_in.target_buf.size      = SCR_SIZE;          
    scale_in.target_buf.ptr       = alloc_buf_ptr; 
    scale_in.target_format        = IMGREF_FORMAT_RGB565;

    if(rotate_dir != ROTATE_180)
    {
        effect_scale_step = (uint32)(abs(new_height - new_width))/(frame_num/2);
        if(new_width > new_height)
        {
            is_effect_scale_w = TRUE;
        }
        else
        {
            is_effect_scale_w = FALSE;
        }
    }

    for (frm_index = 1; frm_index < frame_num / 2; frm_index++)
    {
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, Before scal, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3175_112_2_18_3_3_19_40,(uint8*)"d", SCI_GetTickCount());
        
        SCI_MEMSET( lcd_buf_ptr, 0, SCR_SIZE );
        
        /* scale buff before rotate */
        if(is_effect_scale_w)
        {
            scale_in.target_size.w = new_width - effect_scale_step * frm_index;
            scale_in.target_size.h = new_height;
            scale_res = GRAPH_ScaleImage(&scale_in,&scale_out);
        }
        else
        {
            scale_in.target_size.w = new_width;
            scale_in.target_size.h = new_height - effect_scale_step * frm_index;                
            scale_res = GRAPH_ScaleImage(&scale_in,&scale_out);
        }
        
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, After scal2, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3193_112_2_18_3_3_19_41,(uint8*)"d", SCI_GetTickCount());
        if (SCI_SUCCESS == scale_res)
        {
            rotate_in_ptr.src_chn.chn0 = scale_out.output_chn.chn0;
            rotate_in_ptr.src_size.w = scale_out.output_size.w;
            rotate_in_ptr.src_size.h = scale_out.output_size.h;

            rotate_in_ptr.src_center.x = scale_out.output_size.w/2;
            rotate_in_ptr.src_center.y = scale_out.output_size.h/2;
        }
        else
        {
            break;
        }
        /* buff scale end */
    
        switch(rotate_dir)
        {
        case ROTATE_CLOCKWISE:
            rotate_in_ptr.angle = (90 * frm_index) / frame_num;
            break;
        case ROTATE_COUNTERCLOCKWISE:
            rotate_in_ptr.angle = 360 - (90 * frm_index) / frame_num;
            break;
        case ROTATE_180:
            rotate_in_ptr.angle = (180 * frm_index) / frame_num;
            break;
        default:
            break;
        }
        if(SCI_SUCCESS != GRAPH_RotateArbitrary(&rotate_in_ptr, &rotate_out_ptr))
        {
            break;
        }
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, After rotate, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3227_112_2_18_3_3_19_42,(uint8*)"d", SCI_GetTickCount());

        GUILCD_InvalidateRectOnly( param->lcd_id, rect, 0 );
        //SCI_TRACE_LOW:"[ROTATE]Pre-Half, After refresh lcd, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3230_112_2_18_3_3_19_43,(uint8*)"d", SCI_GetTickCount());
    }
    
    scale_in.src_chn.chn0.ptr = new_buf_ptr;
    for (frm_index = frame_num / 2; frm_index < frame_num; frm_index++)
    {
        //SCI_TRACE_LOW:"[ROTATE]Be-Half, Before scale, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3236_112_2_18_3_3_19_44,(uint8*)"d", SCI_GetTickCount());
    
        SCI_MEMSET( lcd_buf_ptr, 0, SCR_SIZE );

        /* scale buff before rotate */
        if(is_effect_scale_w)
        {
            scale_in.target_size.w = new_height + effect_scale_step * (frm_index - frame_num/2);
            scale_in.target_size.h = new_height;
            scale_res = GRAPH_ScaleImage(&scale_in,&scale_out);
        }
        else
        {
            scale_in.target_size.w = new_width;
            scale_in.target_size.h = new_width + effect_scale_step * (frm_index - frame_num/2);                
            scale_res = GRAPH_ScaleImage(&scale_in,&scale_out);
        }
        
        //SCI_TRACE_LOW:"[ROTATE]Be-Half, After scale2, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3254_112_2_18_3_3_19_45,(uint8*)"d", SCI_GetTickCount());
        if (SCI_SUCCESS == scale_res)
        {
            rotate_in_ptr.src_chn.chn0 = scale_out.output_chn.chn0;
            rotate_in_ptr.src_size.w = scale_out.output_size.w;
            rotate_in_ptr.src_size.h = scale_out.output_size.h;

            rotate_in_ptr.src_center.x = scale_out.output_size.w/2;
            rotate_in_ptr.src_center.y = scale_out.output_size.h/2;
        }
        else
        {
            break;
        }
        /* buff scale end */
        
        switch(rotate_dir)
        {
        case ROTATE_CLOCKWISE:
            rotate_in_ptr.angle =  360 - (90 * (frame_num - frm_index)) / frame_num;
            break;
        case ROTATE_COUNTERCLOCKWISE:
            rotate_in_ptr.angle = (90 * (frame_num - frm_index)) / frame_num;
            break;
        case ROTATE_180:
            rotate_in_ptr.angle =  360 - (180 * (frame_num - frm_index)) / frame_num;
            break;
        default:
            break;
        }
        if(SCI_SUCCESS != GRAPH_RotateArbitrary(&rotate_in_ptr, &rotate_out_ptr))
        {
            break;
        }
        //SCI_TRACE_LOW:"[ROTATE]Be-Half, After rotate, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3288_112_2_18_3_3_19_46,(uint8*)"d", SCI_GetTickCount());

        GUILCD_InvalidateRectOnly( param->lcd_id, rect, 0 );
        //SCI_TRACE_LOW:"[ROTATE]Be-Half, After refresh lcd, tick=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3291_112_2_18_3_3_19_47,(uint8*)"d", SCI_GetTickCount());
    }

    /* restore LCD buff */
    SCI_MEMCPY(lcd_buf_ptr, new_buf_ptr, SCR_SIZE);

    SCI_FREE(alloc_buf_ptr);
#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
}
#endif

/*****************************************************************************/
//  Description:   Play LCD switch effect fade.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PlayLCDSwitchEffectFade( 
                                MMI_WIN_ANIM_PARAM_T *param, 
                                LCD_ANGLE_E cur_angle, 
                                LCD_ANGLE_E old_angle
                                )
{
#define DO_ALPHA    \
    {   \
        x = *tmp_new_ptr++; \
        y = *tmp_old_ptr;       \
        n = (alpha + 1) >> 3;   \
        x = ( x | (x << 16)) & 0x7E0F81F;   \
        y = ( y | (y << 16)) & 0x7E0F81F;   \
        result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;    \
        *alpha_buf_ptr++ = (uint16)((result & 0xFFFF) | (result >> 16));    \
    }

    uint8  n = 0;
    uint32 result = 0;
    uint32 x = 0;
    uint32 y = 0;

    uint16 *buf_ptr 	= PNULL;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    
    uint16 *tmp_new_ptr = PNULL;
    uint16 *tmp_old_ptr = PNULL;
    uint16 *alpha_buf_ptr = PNULL;
    
    uint32 i = 0,j = 0;
    uint32 frm_index = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32  frame_num = GetSpecialEffectFrameNum(MMI_WIN_ANIM_LCD_SWITCH_ROTATE);//GetSpecialEffectFrameNum(MMI_WIN_ANIM_FADE);
	
    uint8 alpha_step = 0;//255/frame_num;
    uint8 alpha = 0;


    uint16 new_width = 0;
    uint16 new_height = 0;
    uint16 old_width = 0;
    uint16 old_height = 0;

    GUILCD_GetLogicWidthHeight( param->lcd_id, &new_width, &new_height );
    old_width = new_height;
    old_height = new_width;

    if (!MMITHEME_IsUpdateFullScreen())
    {
        return;
    }
    	
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        return;        				
    }

    if(0 == frame_num)
    {
        frame_num = 8;
    }

    alpha_step = 255/frame_num;

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

    //执行3d特效之前，将lcd buf内容准备好
    UILAYER_InvalidateRect( param->lcd_id, &rect );

    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    //write s_new_buf_arr
    SCI_MEMCPY( new_buf_ptr, buf_ptr, SCR_SIZE );
    for (frm_index = 1; frm_index < frame_num; frm_index++)
    {
        alpha += alpha_step;
        alpha_buf_ptr = buf_ptr;
        tmp_new_ptr = new_buf_ptr;
        switch(GetLCDRotateDir(cur_angle, old_angle))
        {
        case ROTATE_CLOCKWISE:        /* 逆时针90度*/
            for(i = 0; i < new_height; i++)
            {
                tmp_old_ptr = old_buf_ptr + old_width - 1 - i;
                for(j = 0; j < new_width; j++)
                {
                    DO_ALPHA
                    tmp_old_ptr += old_width;
                }
            }
            break;
        case ROTATE_COUNTERCLOCKWISE:        /* 顺时针90 */
            for(i = 0; i < new_height; i++)
            {
                tmp_old_ptr = old_buf_ptr + (old_height- 1) * old_width + i;
                for(j = 0; j < new_width; j++)
                {
                    DO_ALPHA
                    tmp_old_ptr -= old_width;
                }
            }
            break;
        case ROTATE_180:        /* 180 */
            tmp_old_ptr = old_buf_ptr + old_width*old_height - 1;
            for(j = 0; j < SCR_SIZE >> 1; j++)
            {
                DO_ALPHA
                tmp_old_ptr--;
            }
            break;
        default:
            break;
        }
    //SCI_MEMCPY( buf_ptr, old_buf_ptr, SCR_SIZE );

        GUILCD_InvalidateRectOnly( param->lcd_id, rect, 0 );
        
        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_FADE));
        
    }

    //restore buf_ptr
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif   
    
}
/*****************************************************************************/
//  Description:   Play LCD rotate animate.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PlayWinRotate( 
                                   LCD_ANGLE_E cur_angle, 
                                   LCD_ANGLE_E old_angle
                                   )
{
    MMI_WIN_ANIM_PARAM_T param = {0};

#ifdef MMI_RES_ORIENT_BOTH // 表示支持横竖屏切换
    if(MMI_IsPermit3D((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_DESKTOP))
    {
        param.lcd_id = GUI_MAIN_LCD_ID;
        param.is_back = 0;
        if (MMISET_G_SENSOR_SWITCH_DEFAULT == MMIAPISET_GetGsensorSwitchStyle())
        {
            PlayLCDSwitchEffectRotate( &param, cur_angle, old_angle);
        }
        if (MMISET_G_SENSOR_SWITCH_FADE == MMIAPISET_GetGsensorSwitchStyle())
        {
            PlayLCDSwitchEffectFade( &param, cur_angle, old_angle);
        }
        else
        {
            //SCI_TRACE_LOW:"MMITHEME_PlayWinRotate Error type!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3462_112_2_18_3_3_20_48,(uint8*)"");
        }
    }
#endif

    MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_LCD_ROTATE);
} 
#endif

/*****************************************************************************/
//  Description:   Play effect slide.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void EffectPlaySlide(MMI_POPUP_MENU_EFFECT_PARAM_T *param)
{
    int32 i = 0;
    // int32 j = 0;
    int32 idx = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint16 lcd_width  = rect.right - rect.left + 1;
    uint16 lcd_height = rect.bottom - rect.top + 1;

    int16  frame_num = (int16)GetSpecialEffectFrameNum(MMI_WIN_ANIM_POPMENU);
    uint16 *temp_dst_ptr  = PNULL;
    uint16 *temp_src_ptr  = PNULL;
    uint16 *temp_lcd_buff = PNULL;
    
    uint16 buf_width = param->buff_wid;
    uint16 buf_height = param->buff_height;
    int16 step_x = 0;
    int16 step_y = 0;

    int16  dst_start_y = 0;
    int16  src_start_y = 0;
    int16  copy_height = 0;
    
    int16  dst_start_x = 0;
    int16  src_start_x = 0;
    uint16 copy_width = 0;
    int16 move_x = param->x1;
    int16 move_y = param->y1;
    
    if(PNULL == param)
    {
        return;
    }
    
    if(PNULL == param->buff_new)
    {
        return;
    }

    if(frame_num <= 1)
    {
        return;
    }
    if(buf_width > lcd_width || buf_height > lcd_height)
    {
        return;
    }
    step_y = (int16)(param->y2 - param->y1)/frame_num;
    step_x = (param->x2 - param->x1)/frame_num;
    
    for ( idx = 1; idx < frame_num; idx++ )
    {
        move_x += step_x;
        move_y += step_y;

        if(move_x < 0)
        {
            if(move_x + buf_width <= 0)
            {
                break;
            }
            dst_start_x = 0;
            src_start_x = -move_x;
            copy_width = buf_width - src_start_x;
        }
        else
        {
            if(move_x >= lcd_width)
            {
                break;
            }
            dst_start_x = move_x;
            src_start_x = 0;
            if(dst_start_x + buf_width> lcd_width)
            {
                copy_width = lcd_width - dst_start_x;
            }
            else
            {
                copy_width = buf_width;
            }
        }
        
        if(move_y < 0)
        {
            if(move_y + buf_height <= 0)
            {
                break;
            }
            dst_start_y = 0;
            src_start_y = -move_y;
            copy_height = buf_height - src_start_y;
        }
        else
        {
            if(move_y >= lcd_height)
            {
                break;
            }
            src_start_y = 0;
            dst_start_y = move_y;
            if(dst_start_y + buf_height > lcd_height)
            {
                copy_height = lcd_height - dst_start_y;
            }
            else
            {
                copy_height = buf_height;
            }
        }

        temp_lcd_buff = GUILCD_GetMainLcdBufPtr();
        SCI_MEMCPY( temp_lcd_buff, param->buff_old, SCR_SIZE );
        
        temp_dst_ptr = temp_lcd_buff  + dst_start_y * lcd_width + dst_start_x;
        temp_src_ptr = param->buff_new + src_start_y * param->buf_mem_width + src_start_x;

        for( i = 0; i < copy_height; i++ )
        {
            SCI_MEMCPY( temp_dst_ptr, temp_src_ptr, copy_width * sizeof(uint16));

            temp_dst_ptr += lcd_width;
            temp_src_ptr += param->buf_mem_width;
        }            
        
        GUILCD_InvalidateRectOnly( param->lcd_id, rect, 0 );
        SCI_SLEEP(GET_ANIM_SLEEP_TIME(MMI_WIN_ANIM_POPMENU));
    }
}

/*****************************************************************************/
//  Description:   play win switch effect.
//    Global resource dependence:
//  Author: Yintang.Ren
//    Note:
/*****************************************************************************/
LOCAL void PopupMenuEffectSlide(
                                MMI_POPUP_MENU_EFFECT_PARAM_T *param
                                )
{
    MMI_POPUP_MENU_EFFECT_PARAM_T effect_param = *param;
    // uint16 *buff_new = PNULL;
    // uint16 *buff_old = PNULL;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint16 lcd_width  = rect.right - rect.left + 1;
    uint16 lcd_height = rect.bottom - rect.top + 1;

    MMI_POPUP_MENU_EFFECT_PARAM_SLIDE_T *my_param = (MMI_POPUP_MENU_EFFECT_PARAM_SLIDE_T *)param;
    effect_param.type = my_param->type;
    effect_param.lcd_id = my_param->lcd_id;
    effect_param.dir = my_param->dir;
    
    effect_param.buff_new = my_param->buff_move + my_param->rect.top* lcd_width + my_param->rect.left;
    effect_param.buff_old = my_param->buff_bg;
    
    effect_param.buff_wid = my_param->rect.right - my_param->rect.left + 1;
    effect_param.buff_height = my_param->rect.bottom - my_param->rect.top + 1;
    effect_param.buf_mem_width = lcd_width;
    effect_param.is_full_scr = FALSE;
    if(param->type == MMI_POPUP_MENU_EFFECT_SLIDE_IN)
    {
        switch(my_param->dir)
        {
        case MMI_POPUP_MENU_EFFECT_DIR_UP:
            effect_param.x1 = my_param->rect.left;
            effect_param.x2 = my_param->rect.left;
            effect_param.y1 = lcd_height;
            effect_param.y2 = my_param->rect.top;
            break;
        case MMI_POPUP_MENU_EFFECT_DIR_DOWN:
            effect_param.x1 = my_param->rect.left;
            effect_param.x2 = my_param->rect.left;
            effect_param.y1 = my_param->rect.top - my_param->rect.bottom;
            effect_param.y2 = my_param->rect.top;
            break;
        case MMI_POPUP_MENU_EFFECT_DIR_LEFT:
            effect_param.x1 = lcd_width;
            effect_param.x2 = my_param->rect.left;
            effect_param.y1 = my_param->rect.top;
            effect_param.y2 = my_param->rect.top;
            break;
        case MMI_POPUP_MENU_EFFECT_DIR_RIGHT:
            effect_param.x1 = my_param->rect.left - my_param->rect.right;
            effect_param.x2 = my_param->rect.left;
            effect_param.y1 = my_param->rect.top;
            effect_param.y2 = my_param->rect.top;
            break;
        default:
            return;
        }
    }
    else
    {
        switch(my_param->dir)
        {
        case MMI_POPUP_MENU_EFFECT_DIR_UP:
            effect_param.x1 = my_param->rect.left;
            effect_param.x2 = my_param->rect.left;
            effect_param.y1 = my_param->rect.top;
            effect_param.y2 = my_param->rect.top - my_param->rect.bottom;
            break;
        case MMI_POPUP_MENU_EFFECT_DIR_DOWN:
            effect_param.x1 = my_param->rect.left;
            effect_param.x2 = my_param->rect.left;
            effect_param.y1 = my_param->rect.top;
            effect_param.y2 = lcd_height;
            break;
        case MMI_POPUP_MENU_EFFECT_DIR_LEFT:
            effect_param.x1 = my_param->rect.left;
            effect_param.x2 = my_param->rect.left - my_param->rect.right;
            effect_param.y1 = my_param->rect.top;
            effect_param.y2 = my_param->rect.top;
            break;
        case MMI_POPUP_MENU_EFFECT_DIR_RIGHT:
            effect_param.x1 = my_param->rect.left;
            effect_param.x2 = lcd_width;
            effect_param.y1 = my_param->rect.top;
            effect_param.y2 = my_param->rect.top;
            break;
        default:
            return;
        }
    }
    EffectPlaySlide(&effect_param);
}


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
                                     )
{
    GUI_RECT_T lcd_rect;
    MMI_POPUP_MENU_EFFECT_PARAM_SLIDE_T effect_param = {0};
    uint16 *buff_new = PNULL;
    uint16 *buff_old=PNULL;

    if ( !MMK_IsQuickKey() 
        && PNULL != rect_ptr
        && MMITHEME_Is3DOpen()
        && MMI_IsPermit3D((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_DESKTOP))
    {
        lcd_rect = MMITHEME_GetFullScreenRect();
        MMI_GetTrans3DBuf(&buff_old, &buff_new);
        if (PNULL == buff_old || PNULL == buff_new)
        {
            //SCI_TRACE_LOW:"MMITHEME_PopupMenuEffect: buff_old = %d, buff_new = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3724_112_2_18_3_3_20_49,(uint8*)"dd", buff_old, buff_new);
            MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_WIN_SWITCH);
            return;
        }
        if(is_entry)
        {
            if(level == 0)
            {
                effect_param.dir = MMI_POPUP_MENU_EFFECT_DIR_UP;
            }
            else
            {
                if(MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
                {
                    effect_param.dir = MMI_POPUP_MENU_EFFECT_DIR_RIGHT;
                }
                else
                {
                    effect_param.dir = MMI_POPUP_MENU_EFFECT_DIR_LEFT;
                }
            }
            effect_param.type = MMI_POPUP_MENU_EFFECT_SLIDE_IN;
            UILAYER_InvalidateRect( LCD_ID_0,  &lcd_rect);    
            SCI_MEMCPY(buff_new, GUILCD_GetMainLcdBufPtr(), SCR_SIZE );
        }
        else
        {
            if(level == 0)
            {
                effect_param.dir = MMI_POPUP_MENU_EFFECT_DIR_DOWN;
            }
            else
            {
                if(MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
                {
                    effect_param.dir = MMI_POPUP_MENU_EFFECT_DIR_LEFT;
                }
                else
                {
                    effect_param.dir = MMI_POPUP_MENU_EFFECT_DIR_RIGHT;
                }
            }
            effect_param.type = MMI_POPUP_MENU_EFFECT_SLIDE_OUT;
        }
        
        effect_param.rect= *rect_ptr;
        effect_param.lcd_id = LCD_ID_0;
        
        effect_param.buff_bg = buff_old;
        effect_param.buff_move = buff_new;
        PopupMenuEffectSlide((MMI_POPUP_MENU_EFFECT_PARAM_T *)&effect_param);
    }

    MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_WIN_SWITCH);
}

/*****************************************************************************/
// Description: 判断3D效果是否开启
// Global resource dependence:
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_Is3DOpen(void)
{
    BOOLEAN is_open = FALSE;

    if (MMI_WIN_ANIM_NONE != MMISET_Get3DStyleScreen())
    {
        is_open = TRUE;
    }
    return (is_open);
}

#if 0
#define MMI_WIN_ANIM_TYPE_NUM 10
LOCAL MMI_WIN_ANIM_TYPE_E s_rand_3D_type[] = {MMI_WIN_ANIM_ZOOM_IN, MMI_WIN_ANIM_ZOOM_OUT, MMI_WIN_ANIM_ZOOM_INOUT, 
                            MMI_WIN_ANIM_HOR_SLIDE, 
                            MMI_WIN_ANIM_VER_SLIDE, 
                            MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD, 
                            MMI_WIN_ANIM_VER_SLIDE_WITH_OLD, 
                            MMI_WIN_ANIM_FADE, MMI_WIN_ANIM_3D, MMI_WIN_ANIM_ROTATE
                            };
#endif
/*****************************************************************************/
// Description: 获得当前的动画类型
// Global resource dependence:
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ANIM_TYPE_E MMITHEME_Get3DScreenAnimType(
                                                        MMI_HANDLE_T win_handle,
                                                        MMI_WIN_DISP_STATE_E win_state
                                                        )
{
    MMI_WIN_ANIM_TYPE_E anim_type = MMI_WIN_ANIM_NONE;
    static uint16 type_index = 0;
    // static MMI_WIN_ANIM_TYPE_E s_auto_type = MMI_WIN_ANIM_NONE;
    MMISET_3D_STYLE_SCREEN_E screen_style = MMISET_3D_STYLE_SCREEN_NONE;
    
    screen_style = (MMISET_3D_STYLE_SCREEN_E)(MMISET_Get3DStyleScreen());
    
    switch(screen_style)
    {
    case MMISET_3D_STYLE_SCREEN_NONE:// 关闭窗口特效
        anim_type = MMI_WIN_ANIM_NONE;
        break;

    case MMISET_3D_STYLE_SCREEN_DEFAULT:// 依据应用场景特点设定 
#ifdef EFFECT_ZOOM_SUPPORT
        if (MMIDEFAULT_IsMainMenuWin(win_handle))
        {
            anim_type = MMI_WIN_ANIM_ZOOM_IN;
        }
        else
#endif
#ifdef EFFECT_WIN_SLIDE_SUPPORT
        {
            anim_type = MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD;
        }
#else
        {
            anim_type = MMI_WIN_ANIM_NONE;
        }
#endif
        break;

    case MMISET_3D_STYLE_SCREEN_AUTO:// 各3D效果顺序显示
        type_index = (type_index + 1)%(MMI_WIN_ANIM_AUTO - 1);// MMI_WIN_ANIM_TYPE_NUM;
        anim_type = MMI_WIN_ANIM_NONE + type_index;//   s_rand_3D_type[type_index];
        break;
        
#ifdef EFFECT_WIN_SLIDE_SUPPORT
    case MMISET_3D_STYLE_SCREEN_SLIDEMOVE:// 窗口水平切换-覆盖原窗口
        anim_type = MMI_WIN_ANIM_HOR_SLIDE;
        break;
        
    case MMISET_3D_STYLE_SCREEN_SLIDE:// 窗口水平切换-原窗口跟随
        anim_type = MMI_WIN_ANIM_HOR_SLIDE_WITH_OLD;
        break;
#endif        
#ifdef EFFECT_ZOOM_SUPPORT
    case MMISET_3D_STYLE_SCREEN_ZOOM:// 窗口进入由小变大和退出由大变小
        anim_type = MMI_WIN_ANIM_ZOOM_OUT;
        break;
#endif        

#if 0//def EFFECT_WIN_SLIDE_SUPPORT
    case MMISET_3D_STYLE_SCREEN_FADE:// 窗口淡化
        anim_type = MMI_WIN_ANIM_FADE;
        break;
#endif

#ifdef EFFECT_3D_ROTATE_SUPPORT				
    case MMISET_3D_STYLE_SCREEN_3DROLL:// 窗口3D翻转
        anim_type = MMI_WIN_ANIM_3D;
        break;
#endif		
        
    default:
#ifdef UI_P3D_SUPPORT   
        anim_type = MMIWINSE_GetAnimTypeBySetingStyle(screen_style);
#else
        anim_type = MMI_WIN_ANIM_NONE;
#endif
        break;
    }
    
    return anim_type;
}

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
                                   )
{
	uint16 *buf_ptr 	= 0;
	uint16 buf_width 	= 0;
    uint32 i = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 table_size = 0;
    uint16 *old_buf_ptr = PNULL;
    uint16 *new_buf_ptr = PNULL;
    
	MMI_GetTrans3DBuf(&old_buf_ptr, &new_buf_ptr);

    if (PNULL == old_buf_ptr || PNULL == new_buf_ptr)
    {
        //内存未分配
        //SCI_TRACE_LOW:"MMITHEME_InvalidateRect_FadeIn old_buf_ptr = %d, new_buf_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_SPECIAL_3915_112_2_18_3_3_21_50,(uint8*)"dd", old_buf_ptr, new_buf_ptr);
        return;
    }

    GUILCD_GetLogicWidthHeight(lcd_id,&buf_width,PNULL);
    buf_ptr = GUILCD_GetMainLcdBufPtr();
    
    SCI_MEMCPY(new_buf_ptr, buf_ptr, SCR_SIZE);
    table_size = sizeof(AlphaColor_table)/sizeof(AlphaColorFunc);
    for (i = 0; i < table_size; i++)
    {
        CalcFullAlphaColor(
            i, 
            old_buf_ptr, 
            new_buf_ptr, 
            buf_ptr, 
            SCR_SIZE
            );
        GUILCD_InvalidateRect(
            GUI_MAIN_LCD_ID, 
            rect,
            0); 
#ifdef WIN32
        SCI_Sleep(300);
#endif
    }
    //SCI_Sleep(1000);
    SCI_MEMCPY(buf_ptr, new_buf_ptr, SCR_SIZE);
    //SCI_MEMCPY(s_old_buf_arr, s_new_buf_arr, SCR_SIZE);
}
#endif

void CalcFullAlphaColor(
                        int level,
                        uint16 *buf1_ptr,
                        uint16 *buf2_ptr,
                        uint16 *out_buf_ptr,
                        uint32 buf_size
                        )
{
    AlphaColorFunc func_ptr;

    func_ptr = AlphaColor_table[level];
#if 1
    while (buf_size > 0)
    {
        *out_buf_ptr = func_ptr(*buf1_ptr, *buf2_ptr);
        *(out_buf_ptr+1) = func_ptr(*(buf1_ptr+1), *(buf2_ptr+1));
        *(out_buf_ptr+2) = func_ptr(*(buf1_ptr+2), *(buf2_ptr+2));
        *(out_buf_ptr+3) = func_ptr(*(buf1_ptr+3), *(buf2_ptr+3));
        *(out_buf_ptr+4) = func_ptr(*(buf1_ptr+4), *(buf2_ptr+4));
        *(out_buf_ptr+5) = func_ptr(*(buf1_ptr+5), *(buf2_ptr+5));
        *(out_buf_ptr+6) = func_ptr(*(buf1_ptr+6), *(buf2_ptr+6));
        *(out_buf_ptr+7) = func_ptr(*(buf1_ptr+7), *(buf2_ptr+7));
        buf1_ptr+=8;
        buf2_ptr+=8;
        out_buf_ptr+=8;
        buf_size-=16;
    }
#else
    while (buf_size > 0)
    {
        *out_buf_ptr = func_ptr(*buf1_ptr, *buf2_ptr);
        buf1_ptr++;
        buf2_ptr++;
        out_buf_ptr++;
        buf_size-=2;
    }
#endif
}

uint16 AlphaColor0(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = color1;
    
    return color;
}

uint16 AlphaColor1(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = (color1 == color2) ? color1 : K7(color1)+K1(color2);
    return color;
}

uint16 AlphaColor2(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = (color1 == color2) ? color1 : K6(color1)+K2(color2);
    return color;
}

uint16 AlphaColor3(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = (color1 == color2) ? color1 : K5(color1)+K3(color2);
    return color;
}

uint16 AlphaColor4(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = (color1 == color2) ? color1 : K4(color1)+K4(color2);
    return color;
}

uint16 AlphaColor5(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = (color1 == color2) ? color1 : K3(color1)+K5(color2);
    return color;
}

uint16 AlphaColor6(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = (color1 == color2) ? color1 : K2(color1)+K6(color2);
    return color;
}

uint16 AlphaColor7(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = (color1 == color2) ? color1 : K1(color1)+K7(color2);

    return color;
}

uint16 AlphaColor8(
                uint16 color1,
                uint16 color2
                )
{
    uint16 color = 0;

    color = color2;

    return color;
}

#endif


/*Edit by script, ignore 3 case. Thu Apr 26 19:01:08 2012*/ //IGNORE9527
