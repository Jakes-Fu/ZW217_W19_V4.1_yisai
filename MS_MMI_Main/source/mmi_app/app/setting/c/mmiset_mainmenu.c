/*****************************************************************************
** File Name:      mmiset_mainmenur.c                                            *
** Author:                                                                   *
** Date:           03/06/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004       Jassmine.Meng       Create
******************************************************************************/

#define _MMISET_MAINMENU_C_
//#ifdef MMI_MULTI_MAINMENU_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmicc_export.h"
#include "mmiset_func.h"
#include "mmi_textfun.h"
#include "mmimms_export.h"
#include "mmisms_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiset_display.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmiset_text.h"
#include "mmiset_wintab.h"
#include "mmiset_set_ring.h"
#include "nvitem.h"
#include "mmipub.h"
//#include "mmiss_export.h"
#include "mmi_anim.h"
#include "mmiacc_event.h"

#include "mmk_tp.h"
#include "mmi_theme.h"  //@yongwei.he for multi-langugae macro definition
#include "mmi_modu_main.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_internal.h"
#include "mn_api.h"
#include "mmiset_image.h"//@zhaohui add for plmn icon
//#include "mmicountedtime_export.h"
#include "mmi_modu_main.h"
#include "mmidc_export.h"
//#include "mmi_nv.h"
#include "nv_item_id.h"
#include "mmiphone_export.h"
#include "mmibt_export.h"
#include "mmifm_export.h"
#include "mmi_custom_define.h"
#include "mmi_appmsg.h"
#include "mmiconnection_export.h"
#include "guires.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#include "mmiim.h"
#ifdef DCD_SUPPORT
#include "mmidcd_export.h"
#endif
#ifdef PIM_SUPPORT
#include "mmipim_export.h"
#endif
#include "mmipicview_export.h"
#include "mn_api.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#include "guidropdownlist.h"
//#include "mmiim_im_switching.h"
//#include "mmiim_nv.h"
#include "mmiim.h"
#include "mmimp3_export.h"
#include "mmifmm_export.h"
#include "guisetlist.h"
#include "guilabel.h"
#include "mmi_resource.h"
#include "guiiconlist.h"

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
//#include "mmiset_lock_effect.h"
#include "mmiset_lock_effect_internal.h"
#endif
#include "mmiset_internal.h"
#include "mmiset_mainmenu_export.h"
#ifdef PDA_UI_SUPPORT_U_IDLE
#include "mmigrid_uidle_se.h"
#endif
#ifdef	MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//#define MMISET_MAINMENU_MAX 3

// 主菜单显示的文字资源ID
LOCAL const MMI_TEXT_ID_T s_mainmenu_type_text_id[] = 
{   
#ifdef MATRIX_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_ICON,
#endif
#ifdef MENU_CRYSTALCUBE_SUPPORT
    TXT_SET_MAINMENU_CRYSTALCUBE,
#endif
#ifdef MENU_CYCLONE_SUPPORT
    TXT_SET_MAINMENU_CYCLONE,
#endif
#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
    TXT_SET_MAINMENU_GO,
#else
#ifdef SLIDE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SLIDE,
#endif
        
#ifdef TRAPEZOID_MAIN_MENU_SUPPORT
    TXT_ZOOM,
#endif
#ifdef SPHERE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SPHERE,
#endif
#ifdef CYLINDER_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_CYLINDER,
#endif
#ifdef BINARY_STAR_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_BINARY_STAR,
#endif
#ifdef SHUTTER_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SHUTTER,
#endif
#ifdef CHARIOT_MAIN_MENU_SUPPORT      
    TXT_SET_MAINMENU_CHARIOT,
#endif 
#endif
#ifdef MENU_SWEETS_SUPPORT
    TXT_SET_MAINMENU_SWEETS,
#endif
};
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
LOCAL const MMI_TEXT_ID_T s_mainmenu_select_type_text_id[] = 
{   
    TXT_SET_MAINMENU_SELECT_STATIC,//SELECT_EFFECT_STATIC
    TXT_SET_MAINMENU_SELECT_ROUNDLIGHT,//SELECT_EFFECT_ROUNDLIGHT
    TXT_SET_MAINMENU_SELECT_FIRE,//SELECT_EFFECT_FIRE
    TXT_SET_MAINMENU_SELECT_LIGHTING,//SELECT_EFFECT_LIGHTING
    TXT_SET_MAINMENU_SELECT_WATER,//SELECT_EFFECT_WATER
    TXT_SET_MAINMENU_SELECT_SNOW,//SELECT_EFFECT_SNOW
    TXT_SET_MAINMENU_SELECT_RANDOM,//SELECT_EFFECT_RANDOM
};
#endif
#if defined(PDA_UI_SUPPORT_MANIMENU_GO)

// GO主菜单显示的文字资源ID，必须和GO_MENU_STYLE_INDEX_E顺序一致
LOCAL const MMI_TEXT_ID_T s_go_menu_type_text_id[] = 
{   
    TXT_COMMON_CLASSIC,//默认打开
    TXT_SET_MAINMENU_SELECT_RANDOM,
#ifdef GO_FLASH_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SLIDE,//FLASH
#endif
#ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_BINARY_STAR,
#endif
#ifdef GO_SHUTTER_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SHUTTER,
#endif
#ifdef GO_SPHERE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SPHERE,
#endif
#ifdef GO_CYLINDER_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_CYLINDER,
#endif
#ifdef GO_SLOPE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SLOPE,
#endif
#ifdef GO_WHEEL_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_WHEEL,
#endif
#ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_OCEANWAVE,
#endif
#ifdef GO_PEGTOP_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_PEGTOP,
#endif
#ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_TURNTABLE,
#endif
#ifdef GO_BOXROT_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_BOXROT,
#endif
#ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_ICONBOXROT,
#endif
#ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_BOOKPAGE,
#endif
#ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_MIDROTATE,
#endif
#ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SMALLITEMS,
#endif
#ifdef GO_CONSTICK_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_CONSTICK,
#endif
#ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_SYMMETRY,
#endif
#ifdef GO_BENZITEM_MAIN_MENU_SUPPORT
    TXT_SET_MAINMENU_BENZITEM,
#endif
};
#endif

#if defined(PDA_UI_SUPPORT_U_IDLE)
LOCAL const MMI_TEXT_ID_T s_u_idle_text_id[] = 
{   
    //TXT_COMMON_CLASSIC,  //default open
    TXT_COMMON_CLOSE,
    TXT_SET_MAINMENU_SELECT_RANDOM,
    TXT_SET_MAINMENU_SLOPE,
    TXT_SET_MAINMENU_BOXROT,
    TXT_SET_MAINMENU_BOOKPAGE,
    TXT_SET_MAINMENU_MIDROTATE,
    TXT_SET_MAINMENU_SMALLITEMS,
};
#endif

#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
/*****************************************************************************/
//  Description : handle GOmenu type set win
//  Global resource dependence : 
//  Author:KYLE JIN
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGOMenuTypeSetWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM            param
                                                );
//GO主菜单效果选择窗口用列表实现
WINDOW_TABLE( MMISET_GOMENU_TYPE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleGOMenuTypeSetWinMsg ),    
    WIN_ID( MMISET_SET_GOMENU_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_MAINMENU_STYLE),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_GOMENU_TYPE_CTRL_ID),
    END_WIN
};

#endif

#if defined(PDA_UI_SUPPORT_U_IDLE)
/*****************************************************************************/
//  Description : handle uidle type set win
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUIdleTypeSetWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM            param
                                                );
//GO主菜单效果选择窗口用列表实现
WINDOW_TABLE( MMISET_UIDLE_TYPE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleUIdleTypeSetWinMsg ),    
    WIN_ID( MMISET_SET_UIDLE_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_UIDLE_STYLE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_UIDLE_TYPE_CTRL_ID),
    END_WIN
};
#endif

/*****************************************************************************/
//  Description : to handle the main menu type set window
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuTypeSetWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM            param
                                                );
                                                
/*****************************************************************************/
//  Description : to handle the main menu preview set window
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuTypePreviewWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM            param
                                                    );                                                   
/*************************************************************************************/
//  Description : listbox里添加主菜单显示格式
//  Global resource dependence : none
//  Author:
//  Note:  
/**************************************************************************************/
LOCAL void MMISETMainMenuTypeAppendItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      );
               
#ifdef PDA_UI_SUPPORT_U_IDLE                     
/*****************************************************************************/
//  Description : get uidle type item index
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetUIdleStyleItemIndex(UIDLE_STYLE_E uidle_style);
#endif 
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
// 	Description : to handle set main menu select style win
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMainMenuSelectStyleWindow(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                );
#endif                                     

#ifdef MMI_SETTING_CUTDOWN_MAINMENU
//主菜单显示窗口用列表实现
WINDOW_TABLE( MMISET_MAINMENU_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    //WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    WIN_FUNC( (uint32)HandleMainMenuTypeSetWinMsg ),    
    WIN_ID( MMISET_SET_MAINMENU_TYPE_WIN_ID ),
    WIN_TITLE(TXT_SET_MAINMENU_STYLE),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_MAINMENU_TYPE_CTRL_ID),
    END_WIN
};

#else
// main menu type set win table
WINDOW_TABLE( MMISET_MAINMENU_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    //WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    WIN_FUNC( (uint32)HandleMainMenuTypeSetWinMsg ),    
    WIN_ID( MMISET_SET_MAINMENU_TYPE_WIN_ID ),
    WIN_TITLE(TXT_NULL),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_ICONLIST_CTRL(MMISET_MAINMENU_TYPE_CTRL_ID),
    END_WIN
};



// main menu type set win table
WINDOW_TABLE( MMISET_MAINMENU_TYPE_PREVIEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    //WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    WIN_FUNC( (uint32)HandleMainMenuTypePreviewWinMsg ),    
    WIN_ID( MMISET_SET_MAINMENU_PREVIEW_TYPE_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, TXT_NULL),
    END_WIN
};    
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
//the window for set main menu select style
WINDOW_TABLE( MMISET_MAINMENU_SELECT_STYLE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetMainMenuSelectStyleWindow ),    
    WIN_ID( MMISET_MAINMENU_SELECT_STYLE_WIN_ID ),
    WIN_TITLE( TXT_SET_MAINMENU_SELECT_STYLE ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_MAINMENU_SELECT_STYLE_CTRL_ID),
    //WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

LOCAL GUI_POINT_T s_set_mainmenu_start_point = {0};
LOCAL GUI_POINT_T s_set_mainmenu_pre_point = {0};

LOCAL uint8 s_set_mainmenu_redraw_timer = 0;
/*****************************************************************************/
//  Description : judge is mainmenu redraw time
//  Global resource dependence : 
//  Author: MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsMainMenuRedrawTimer(uint8 timer)
{
   return(s_set_mainmenu_redraw_timer == timer);
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void StopSetingRedrawTimer(MMI_WIN_ID_T win_id)
{
    if (0 != s_set_mainmenu_redraw_timer)
    {
        MMK_StopTimer(s_set_mainmenu_redraw_timer);
        s_set_mainmenu_redraw_timer = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void StartSetingRedrawTimer(MMI_WIN_ID_T win_id)
{
    if (0 == s_set_mainmenu_redraw_timer)
    {
        s_set_mainmenu_redraw_timer= MMK_CreateWinTimer(win_id, 100, FALSE);
    }
}



// 主菜单风格数图片ID组
LOCAL MMI_IMAGE_ID_T s_mmiset_mainmenu_style[MMISET_PDA_MAINMENU_MAX][2] = 
{
#ifndef MMI_SETTING_CUTDOWN_MAINMENU
#ifdef MATRIX_MAIN_MENU_SUPPORT
    // 宫格类型
    {IMAGE_SET_ICON_MAINMENU_ICON,  IMAGE_SET_IMG_MAINMENU_ICON},
#endif
    // 左右滑动菜单
#ifdef SLIDE_MAIN_MENU_SUPPORT
    {IMAGE_SET_ICON_MAINMENU_SLIDE_PAGE, IMAGE_SET_IMG_MAINMENU_SLIDE_PAGE},
#endif

    // 水晶立方
#ifdef MENU_CRYSTALCUBE_SUPPORT    
    {IMAGE_SET_ICON_MAINMENU_CRYSTAL_CUBE, IMAGE_SET_IMG_MAINMENU_CRYSTAL_CUBE},
#endif
    // 龙卷风
#ifdef MENU_CYCLONE_SUPPORT
    {IMAGE_SET_ICON_MAINMENU_CYCLONE, IMAGE_SET_IMG_MAINMENU_CYCLONE},
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    {IMAGE_SET_ICON_MAINMENU_GO,  IMAGE_SET_IMG_MAINMENU_GO},
#else
    // 梯形滑动，即左右滑动时有梯形变换
#ifdef TRAPEZOID_MAIN_MENU_SUPPORT
    {IMAGE_SET_ICON_MAINMENU_TRAPEZOID, IMAGE_SET_IMG_MAINMENU_TRAPEZOID},
#endif
    // 球体
#ifdef SPHERE_MAIN_MENU_SUPPORT
    {IMAGE_SET_ICON_MAINMENU_SPHERE, IMAGE_SET_IMG_MAINMENU_SPHERE},
#endif
    // 圆柱体
#ifdef CYLINDER_MAIN_MENU_SUPPORT
    {IMAGE_SET_ICON_MAINMENU_CYLINDER, IMAGE_SET_IMG_MAINMENU_CYLINDER},
#endif
    // 双子星
#ifdef BINARY_STAR_MAIN_MENU_SUPPORT
    {IMAGE_SET_ICON_MAINMENU_BINARY_STAR, IMAGE_SET_IMG_MAINMENU_BINARY_STAR},
#endif
    // 百叶窗
#ifdef SHUTTER_MAIN_MENU_SUPPORT
    {IMAGE_SET_ICON_MAINMENU_SHUTTER, IMAGE_SET_IMG_MAINMENU_SHUTTER},
#endif
    //滚咕噜咕噜滚
#ifdef CHARIOT_MAIN_MENU_SUPPORT    
    {IMAGE_SET_ICON_MAINMENU_CHARIOT, IMAGE_SET_IMG_MAINMENU_CHARIOT},
#endif
#endif
#ifdef MENU_SWEETS_SUPPORT
    //糖果
    {IMAGE_SET_ICON_MAINMENU_SWEET, IMAGE_SET_IMG_MAINMENU_SWEET},
#endif
#else
    {0}
#endif
};


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void SetMainmenuPrePoint(int16 x, int16 y)
{
     s_set_mainmenu_pre_point.x = x;
     s_set_mainmenu_pre_point.y = y;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void SetMainmenuStartPoint(int16 x, int16 y)
{
     s_set_mainmenu_start_point.x = x;
     s_set_mainmenu_start_point.y = y;
}
/*****************************************************************************/
//  Description : get main menu type item index
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMainmenuStyleItemIndex(
                                       MMIMAINMENU_TYPE_E mainmenu_style
                                       )
{
    uint16 item_index = 0;

    switch(mainmenu_style)
    {
#ifdef MATRIX_MAIN_MENU_SUPPORT    
    case MMIMAINMENU_MATRIX_E: // 宫格
        item_index = MMISET_PDA_MAINMENU_ICON;
        break;
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    case MMIMAINMENU_GO_SLIDE_E:
        item_index = MMISET_PDA_MAINMENU_GO;
        break;
#endif
#ifdef MENU_SWEETS_SUPPORT
    case MMIMAINMENU_SWEETS_E:
        item_index = MMISET_PDA_MAINMENU_SWEETS;
        break;
#endif

    default:
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_521_112_2_18_2_51_21_235,(uint8*)"d", mainmenu_style);
        break;
    }

    return item_index;
}

/*****************************************************************************/
//  Description : get main menu item index
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMIMAINMENU_TYPE_E GetMainmenuStyleByIndex(
                                              uint16 item_index
                                              )
{
    MMIMAINMENU_TYPE_E mainmenu_style = MMIMAINMENU_MATRIX_E;

    switch(item_index)
    {
#ifdef MATRIX_MAIN_MENU_SUPPORT    
    case MMISET_PDA_MAINMENU_ICON: // 宫格
        mainmenu_style = MMIMAINMENU_MATRIX_E;
        break;
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    case MMISET_PDA_MAINMENU_GO:
        mainmenu_style = MMIMAINMENU_GO_SLIDE_E;
        break; 
#endif
#ifdef MENU_SWEETS_SUPPORT
    case MMISET_PDA_MAINMENU_SWEETS:
        mainmenu_style = MMIMAINMENU_SWEETS_E;
        break;
#endif
    default:
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_597_112_2_18_2_51_21_236,(uint8*)"d", item_index);
        break;
    }

    return mainmenu_style;
}

/*****************************************************************************/
//  Description : handle main menu preview set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MainmenuStylePreviewDispImage(
                                         uint16 item_index, 
                                         int16 offset_x,
                                         GUI_RECT_T client_rect,
                                         MMI_WIN_ID_T win_id
                                         )
{
    uint16 image_width = 0;
    uint16 image_height = 0;
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T image_rect = {0};
    MMI_IMAGE_ID_T     image_id        = 0;
    GUI_LCD_DEV_INFO        lcd_dev_info    = {0};
    
    if (win_id == MMISET_SET_MAINMENU_PREVIEW_TYPE_WIN_ID)
    {
        image_id = s_mmiset_mainmenu_style[item_index][1];
    }
    
    GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, win_id);
    
    if (image_width > 0)
    {
        image_rect = GUI_CreateRect(0, 0, image_width - 1, image_height - 1);
        
        // 先做偏移
        client_rect.left += offset_x;
        client_rect.right += offset_x;
        
        // 计算显示区域
        disp_rect = GUI_GetCenterRect(client_rect, image_width, image_height);
        
        // 显示图片
        GUIRES_DisplayImg(PNULL, &disp_rect, &image_rect, win_id, image_id, &lcd_dev_info);
    }
    
    return;
}


/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MoveSetingPreview(
                                GUI_POINT_T tp_point,
                                uint16 item_index,
                                uint16 max_item_num,
                                MMI_WIN_ID_T  win_id
                                )
{
    int16   lcd_pos = 0;
    uint16  layer_width = 0;
    uint16  layer_height = 0;
    BOOLEAN result = TRUE;
    BOOLEAN retcode = FALSE;
    #ifdef TOUCH_PANEL_SUPPORT
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    #endif
    uint16 tmp_item_index = 0;
    GUI_RECT_T              client_rect     = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info    = {0};
    
    if (0 == max_item_num)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MoveSetingPreview FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_671_112_2_18_2_51_21_237,(uint8*)"");
        return FALSE;
    }

    StopSetingRedrawTimer(win_id);
    StartSetingRedrawTimer(win_id);
    
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);
    
#ifdef TOUCH_PANEL_SUPPORT
    MMK_GetLogicTPMsg(&state, &tp_point);
#endif

    if (tp_point.x > s_set_mainmenu_start_point.x) // 向右移动
    {
        if (0 == item_index)
        {
            tmp_item_index = max_item_num - 1;
        }
        else
        {
            tmp_item_index = item_index - 1;
        }
        tmp_item_index = tmp_item_index%max_item_num;
        
        lcd_pos = -layer_width;
    }
    else if (tp_point.x < s_set_mainmenu_start_point.x)
    {
        tmp_item_index = item_index + 1;
        tmp_item_index = tmp_item_index%max_item_num;
        
        lcd_pos = layer_width;
    }
    else
    {
        result = FALSE;
    }
    
    if (result && IS_TP_MOVE_X(s_set_mainmenu_pre_point.x, tp_point.x))
    {
        client_rect = MMITHEME_GetClientRectEx(win_id);
        
        MMITHEME_DisplayCommonBg(&client_rect, win_id, &lcd_dev_info);
        
        MainmenuStylePreviewDispImage(item_index, tp_point.x - s_set_mainmenu_start_point.x, client_rect, win_id);
        
        client_rect.left += lcd_pos;
        client_rect.right += lcd_pos;
        
        MainmenuStylePreviewDispImage(tmp_item_index, tp_point.x - s_set_mainmenu_start_point.x, client_rect, win_id);

        retcode = TRUE;
    }

    return retcode;
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 ResumeSetingPreview(
                                 GUI_POINT_T  tp_point,
                                 uint16       item_index,
                                 uint16       max_item_num,
                                 MMI_WIN_ID_T win_id
                                 )
{
    int16   lcd_pos = 0;
    int16   offset_x = 0;
    int16   offset = 0;
    int16   move_space = 0;
    uint16  i = 0;
    uint16  layer_width = 0;
    uint16  layer_height = 0;
    uint16  tmp_item_index = 0;
    BOOLEAN is_changed = FALSE;
    GUI_RECT_T      client_rect = {0};
    GUI_RECT_T      next_client_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (0 == max_item_num)
    {
        //SCI_TRACE_LOW:"mmisetting_assert ResumeSetingPreview 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_753_112_2_18_2_51_21_238,(uint8*)"");
        return 0; 
    }

    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);

    offset_x = tp_point.x - s_set_mainmenu_start_point.x;
    
    if (tp_point.x > s_set_mainmenu_start_point.x) // 向右移动
    {
        if (0 == item_index)
        {
            tmp_item_index = max_item_num - 1;
        }
        else
        {
            tmp_item_index = item_index - 1;
        }
        tmp_item_index = tmp_item_index%max_item_num;
        
        if (tp_point.x - s_set_mainmenu_start_point.x >= layer_width/4)
        {
            move_space = layer_width - (tp_point.x - s_set_mainmenu_start_point.x); // 向右滑动的位移
            is_changed = TRUE;
        }
        else
        {
            move_space = -(tp_point.x - s_set_mainmenu_start_point.x); // 向左恢复的位移
        }
        
        lcd_pos = -layer_width;
    }
    else if (tp_point.x < s_set_mainmenu_start_point.x) // 向左移动
    {
        tmp_item_index = item_index + 1;
        tmp_item_index = tmp_item_index%max_item_num;
        
        if (s_set_mainmenu_start_point.x - tp_point.x >= layer_width/4)
        {
            is_changed = TRUE;
            move_space = -(layer_width + (tp_point.x - s_set_mainmenu_start_point.x)); // 向左滑动的位移
        }
        else
        {
            move_space = -(tp_point.x - s_set_mainmenu_start_point.x); // 向右恢复的位移
        }
        
        lcd_pos = layer_width;
    }
    else
    {
        is_changed = FALSE;
    }
    
    next_client_rect = client_rect = MMITHEME_GetClientRectEx(win_id);
    
    for (i = 1; i <= 10; i++)
    {
        offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, 10, i);
        
        if (0 != offset)
        {
            MMITHEME_DisplayCommonBg(&client_rect, win_id, &lcd_dev_info);
            
            MainmenuStylePreviewDispImage(item_index, offset + offset_x, client_rect, win_id);
            
            next_client_rect.left = client_rect.left + lcd_pos;
            next_client_rect.right = client_rect.right + lcd_pos;
            
            MainmenuStylePreviewDispImage(tmp_item_index, offset + offset_x, next_client_rect, win_id);
            
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, client_rect,0);
        }
        
        // SCI_Sleep(100);
    }
    
    if (is_changed)
    {
        item_index = tmp_item_index;
    }

    return item_index;
}

#ifdef MMI_SETTING_CUTDOWN_MAINMENU
/*****************************************************************************/
//  Description : handle mainmenu type set win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuTypeSetWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM            param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_MAINMENU_TYPE_CTRL_ID;
    //GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    //GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    
    //SCI_TRACE_LOW:"HandleMainMenuTypeSetWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_854_112_2_18_2_51_21_239,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISETMainMenuTypeAppendItem(ctrl_id); 
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16 sel_index = 0;                    
            sel_index = GetMainmenuStyleItemIndex(MMIAPISET_GetMainmenuStyle());
            GUILIST_SetSelectedItem(ctrl_id, sel_index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,sel_index);
        }
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        {
#ifdef MATRIX_MAIN_MENU_SUPPORT        
            MMIMAINMENU_TYPE_E     mainmenu_style  =   MMIMAINMENU_MATRIX_E;
#else
            MMIMAINMENU_TYPE_E     mainmenu_style  =   MMIMAINMENU_TYPE_MAX;
#endif
            uint16         cur_select_id   =   0;            
            cur_select_id = GUILIST_GetCurItemIndex( ctrl_id );            
            mainmenu_style = GetMainmenuStyleByIndex(cur_select_id); // 获得类型
			#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
            if(mainmenu_style == MMIMAINMENU_GO_SLIDE_E)
			{
            	MMK_CreateWin((uint32 *)MMISET_GOMENU_TYPE_WIN_TAB,PNULL);
			}
			else
			#endif
			{
                if (mainmenu_style != MMIAPISET_GetMainmenuStyle())
                {
                    //set main menu style
                    MMIAPISET_SetMainmenuStyle(mainmenu_style);
                    MMK_ReturnIdleWin();
	            }
                else
                {
                    MMK_CloseWin(win_id);
                }
            }
        }
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}
#else
/*****************************************************************************/
//  Description : handle mainmenu type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuTypeSetWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM            param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16       item_index = 0;
    GUIANIM_DATA_INFO_T             data_info = {0};
    MMI_CTRL_ID_T ctrl_id = MMISET_MAINMENU_TYPE_CTRL_ID;

    //SCI_TRACE_LOW:"HandleMainMenuTypeSetWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_927_112_2_18_2_51_22_240,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetTitleTextId(win_id, TXT_SET_MAINMENU_STYLE, FALSE);
        GUIICONLIST_SetTotalIcon(ctrl_id,MMISET_PDA_MAINMENU_MAX);
        GUIICONLIST_SetMaxSelectedItem(ctrl_id, 1);

        GUIICONLIST_SetIconWidthHeight(ctrl_id, MMISET_SCREEN_LOCK_ICON_WIDTH, MMISET_SCREEN_LOCK_ICON_HEIGHT);
        GUIICONLIST_SetLoadType(ctrl_id,GUIICONLIST_LOAD_ALL);

        item_index = GetMainmenuStyleItemIndex(MMIAPISET_GetMainmenuStyle());
        GUIICONLIST_SetSelectedItem(ctrl_id, item_index, TRUE);
        GUIICONLIST_SetCurIconIndex(item_index,ctrl_id);
        GUIICONLIST_SetDispCustomerSoftkey(ctrl_id, TRUE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_ICONLIST_APPEND_ICON:
        if (PNULL != param)
        {
            //get pic index and item index
            item_index = *((uint16 *)param);
            
            if (item_index < MMISET_PDA_MAINMENU_MAX)
            {
                data_info.img_id = s_mmiset_mainmenu_style[item_index][0];
                
                //append icon
                GUIICONLIST_AppendIcon(item_index, ctrl_id, &data_info, NULL);
            }
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16 sel_index = 0;

            GUIICONLIST_GetSelectedItemIndex(ctrl_id, &sel_index, 1);
            GUIICONLIST_SetSelectedItem(ctrl_id, sel_index, FALSE);

            item_index = GetMainmenuStyleItemIndex(MMIAPISET_GetMainmenuStyle());
            GUIICONLIST_SetSelectedItem(ctrl_id, item_index, TRUE);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        {
            uint16 cur_item_index = 0;
#ifdef MATRIX_MAIN_MENU_SUPPORT             
            MMIMAINMENU_TYPE_E     mainmenu_style  =   MMIMAINMENU_MATRIX_E;
#else
            MMIMAINMENU_TYPE_E     mainmenu_style  =   MMIMAINMENU_TYPE_MAX;
#endif
			#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
			uint16 cur_select_mainmenu_style = 0;
			#endif
            mainmenu_style = MMIAPISET_GetMainmenuStyle();

            cur_item_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
			
			#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
			cur_select_mainmenu_style = GetMainmenuStyleByIndex(cur_item_index);

            if(cur_select_mainmenu_style == MMIMAINMENU_GO_SLIDE_E)
			{
            	MMK_CreateWin((uint32 *)MMISET_GOMENU_TYPE_WIN_TAB,PNULL);
			}
			else
			#endif
			{
            	MMK_CreateWin((uint32 *)MMISET_MAINMENU_TYPE_PREVIEW_WIN_TAB,(ADD_DATA)cur_item_index);
			}
        }
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}
#endif

/*************************************************************************************/
//  Description : listbox里添加主菜单显示格式
//  Global resource dependence : none
//  Author:
//  Note:  
/**************************************************************************************/
LOCAL void MMISETMainMenuTypeAppendItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      )
{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    uint8                       mode_id             = 0;
    uint32                      max_size            = ARR_SIZE(s_mainmenu_type_text_id);
    
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, max_size, FALSE );
    GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

    //顺序添加五个列表项
    for( mode_id = 0; mode_id < max_size; mode_id++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = s_mainmenu_type_text_id[mode_id];            
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
}





/*****************************************************************************/
//  Description : main menu type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateMainmenuStyleWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_MAINMENU_TYPE_WIN_TAB,PNULL);
}

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : uidle effect set win
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateUIdleStyleWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_UIDLE_TYPE_WIN_TAB,PNULL);
}
#endif

/*****************************************************************************/
//  Description : handle main menu preview set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuTypePreviewWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param
                                                    )
{
    //uint32                  image_id        = 0;
    GUI_RECT_T              client_rect     = {0};
    MMI_RESULT_E            recode          = MMI_RESULT_TRUE;
    //MMI_CTRL_ID_T           ctrl_id         = MMISET_MAINMENU_TYPE_CTRL_ID;
    GUI_LCD_DEV_INFO        lcd_dev_info    = {0};
    MMIMAINMENU_TYPE_E      mainmenu_style  = MMIMAINMENU_TYPE_MAX;
    //uint16                  tmp_item_index  = 0;
    static uint16           item_index      = 0;
    GUI_POINT_T             tp_point        = {0};
    MMI_TEXT_ID_T number_text_index[MMISET_PDA_MAINMENU_MAX] = 
    {
#ifdef MATRIX_MAIN_MENU_SUPPORT    
        TXT_SET_MAINMENU_ICON,          // 立方体
#endif        
            
#ifdef MENU_CRYSTALCUBE_SUPPORT        
            TXT_SET_MAINMENU_CRYSTALCUBE,   // 魔方
#endif
            
#ifdef MENU_CYCLONE_SUPPORT
            TXT_SET_MAINMENU_CYCLONE,       // 龙卷风
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
            TXT_SET_MAINMENU_GO,
#else
#ifdef SLIDE_MAIN_MENU_SUPPORT
            TXT_SET_MAINMENU_SLIDE,         // 闪现
#endif
#ifdef TRAPEZOID_MAIN_MENU_SUPPORT
            TXT_ZOOM,     // 缩放
#endif
            
#ifdef SPHERE_MAIN_MENU_SUPPORT
            TXT_SET_MAINMENU_SPHERE,        // 球形叠化
#endif
            
#ifdef CYLINDER_MAIN_MENU_SUPPORT
            TXT_SET_MAINMENU_CYLINDER,      // 矩形叠化
#endif
            
#ifdef BINARY_STAR_MAIN_MENU_SUPPORT
            TXT_SET_MAINMENU_BINARY_STAR,   // 散花  
#endif
            
#ifdef SHUTTER_MAIN_MENU_SUPPORT
            TXT_SET_MAINMENU_SHUTTER,       // 若隐若现
#endif
            
#ifdef CHARIOT_MAIN_MENU_SUPPORT    
            TXT_SET_MAINMENU_CHARIOT,        // 滚咕噜咕噜滚
#endif	
#endif
#ifdef MENU_SWEETS_SUPPORT
        TXT_SET_MAINMENU_SWEETS,       // 糖果
#endif
    };    
    
    //SCI_TRACE_LOW:"HandleMainMenuTypePreviewWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_1124_112_2_18_2_51_22_241,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        item_index = 0;
        if(NULL != (uint32)MMK_GetWinAddDataPtr(win_id))
        {
            item_index = (uint32)MMK_GetWinAddDataPtr(win_id);
        }
        GUIWIN_SetTitleTextId(win_id, number_text_index[item_index], FALSE);
        break;

    case MSG_FULL_PAINT:
        {
            //uint16 image_width = 0;
            //uint16 image_height = 0;
            //GUI_RECT_T disp_rect = {0};
            //GUI_RECT_T image_rect = {0};yongsheng.wang 修改pclint

            client_rect = MMITHEME_GetClientRectEx(win_id);

            MMITHEME_DisplayCommonBg(&client_rect, win_id, &lcd_dev_info);

            MainmenuStylePreviewDispImage(item_index, 0, client_rect, win_id);
        }
        break;

    case MSG_TIMER:
        if (IsMainMenuRedrawTimer( *(uint8*)param))
        {
            //int16   lcd_pos = 0;
            //BOOLEAN result = TRUE;
            MMI_TP_STATUS_E state = MMI_TP_NONE;

            StopSetingRedrawTimer(win_id);
            StartSetingRedrawTimer(win_id);

#ifdef TOUCH_PANEL_SUPPORT
            MMK_GetLogicTPMsg(&state, &tp_point);
#endif

            if (MoveSetingPreview(tp_point, item_index, MMISET_PDA_MAINMENU_MAX, win_id))
            {
                s_set_mainmenu_pre_point.x = tp_point.x;
                s_set_mainmenu_pre_point.y = tp_point.y;
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_LOSE_FOCUS:
        StopSetingRedrawTimer(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        s_set_mainmenu_start_point.x = MMK_GET_TP_X(param);
        s_set_mainmenu_start_point.y = MMK_GET_TP_Y(param);
        s_set_mainmenu_pre_point.x = MMK_GET_TP_X(param);
        s_set_mainmenu_pre_point.y = MMK_GET_TP_Y(param);
        StartSetingRedrawTimer(win_id);
        break;

    case MSG_TP_PRESS_MOVE:
        break;

    case MSG_TP_PRESS_UP:
        {
            StopSetingRedrawTimer(win_id);

            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);

            item_index = ResumeSetingPreview(tp_point, item_index, MMISET_PDA_MAINMENU_MAX, win_id);
            GUIWIN_SetTitleTextId(win_id, number_text_index[item_index], TRUE);
        }
        break;
#endif

    case MSG_APP_MENU:
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        StopSetingRedrawTimer(win_id);
        mainmenu_style = GetMainmenuStyleByIndex(item_index); // 获得类型

        if (mainmenu_style != MMIAPISET_GetMainmenuStyle())
        {
            //set main menu style
            MMIAPISET_SetMainmenuStyle(mainmenu_style);
            MMK_ReturnIdleWin();
        }
        else
        {
            MMK_CloseWin(win_id); // 关闭窗口
        }
        break;

    case MSG_CLOSE_WINDOW:
        item_index = 0;
        s_set_mainmenu_pre_point.x = 0;
        s_set_mainmenu_start_point.x = 0;
        break;
        
    case MSG_APP_CANCEL:
        StopSetingRedrawTimer(win_id);
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
/*****************************************************************************/
//  Description : get main menu type item index
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetGOmenuStyleItemIndex(
                                       GO_MENU_STYLE_E go_menu_style
                                       )
{
    uint16 cur_selection = 0;

    switch(go_menu_style)
	{
		//#ifdef GO_CLASSIC_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_CLASSIC:
	        cur_selection = GO_MENU_STYLE_INDEX_CLASSIC;
	        break;
		//#endif
		case GO_MENU_STYLE_RANDOM:
            cur_selection = GO_MENU_STYLE_INDEX_RANDOM;
            break;
		#ifdef GO_FLASH_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_FLASH:
	        cur_selection = GO_MENU_STYLE_INDEX_FLASH;
	        break;
		#endif
		#ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_BINARY_STAR:
	        cur_selection = GO_MENU_STYLE_INDEX_BINARY_STAR;
	        break;
		#endif
		#ifdef GO_SHUTTER_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_SHUTTER:
	        cur_selection = GO_MENU_STYLE_INDEX_SHUTTER;
	        break;
		#endif   
		#ifdef GO_SPHERE_MAIN_MENU_SUPPORT
	     case GO_MENU_STYLE_SPHERE:
	        cur_selection = GO_MENU_STYLE_INDEX_SPHERE;
	        break;
		#endif
		#ifdef GO_CYLINDER_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_CYLINDER:
	        cur_selection = GO_MENU_STYLE_INDEX_CYLINDER;
	        break;
		#endif

		#ifdef GO_SLOPE_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_SLOPE:
	        cur_selection = GO_MENU_STYLE_INDEX_SLOPE;
	        break;
		#endif
		#ifdef GO_WHEEL_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_WHEEL:
	        cur_selection = GO_MENU_STYLE_INDEX_WHEEL;
	        break;
		#endif
		#ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_OCEANWAVE:
	        cur_selection = GO_MENU_STYLE_INDEX_OCEANWAVE;
	        break;
		#endif
		#ifdef GO_PEGTOP_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_PEGTOP:
	        cur_selection = GO_MENU_STYLE_INDEX_PEGTOP;
	        break;
		#endif
		#ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_TURNTABLE:
	        cur_selection = GO_MENU_STYLE_INDEX_TURNTABLE;
	        break;
		#endif
		#ifdef GO_BOXROT_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_BOXROT:
	        cur_selection = GO_MENU_STYLE_INDEX_BOXROT;
	        break;
		#endif
		#ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_ICONBOXROT:
	        cur_selection = GO_MENU_STYLE_INDEX_ICONBOXROT;
	        break;
		#endif
		#ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_BOOKPAGE:
	        cur_selection = GO_MENU_STYLE_INDEX_BOOKPAGE;
	        break;
		#endif
		#ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_MIDROTATE:
	        cur_selection = GO_MENU_STYLE_INDEX_MIDROTATE;
	        break;
		#endif
		#ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_SMALLITEMS:
	        cur_selection = GO_MENU_STYLE_INDEX_SMALLITEMS;
	        break;
		#endif
		#ifdef GO_CONSTICK_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_CONSTICK:
	        cur_selection = GO_MENU_STYLE_INDEX_CONSTICK;
	        break;
		#endif
		#ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_SYMMETRY:
	        cur_selection = GO_MENU_STYLE_INDEX_SYMMETRY;
	        break;
		#endif
		#ifdef GO_BENZITEM_MAIN_MENU_SUPPORT
	    case GO_MENU_STYLE_BENZITEM:
	        cur_selection = GO_MENU_STYLE_INDEX_BENZITEM;
	        break;
		#endif

	    default:
	        //SCI_TRACE_LOW:"GetMainmenuStyleItemIndex error type = %d"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_1357_112_2_18_2_51_23_242,(uint8*)"d", go_menu_style);
	        break;
	    }

    return cur_selection;
}

/*****************************************************************************/
//  Description : get GO menu item index
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL GO_MENU_STYLE_E GetGOmenuStyleByIndex(
                                              uint16 item_index
                                              )
{
    GO_MENU_STYLE_E go_menu_style = GO_MENU_STYLE_CLASSIC;

    switch(item_index)
    {
        //#ifdef GO_CLASSIC_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_CLASSIC:
            go_menu_style = GO_MENU_STYLE_CLASSIC;
            break;
        //#endif 
        case GO_MENU_STYLE_INDEX_RANDOM:
            go_menu_style = GO_MENU_STYLE_RANDOM;
            break;
        #ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT
        case GO_MENU_STYLE_INDEX_BINARY_STAR:
            go_menu_style = GO_MENU_STYLE_BINARY_STAR;
            break;
        #endif
        #ifdef GO_SHUTTER_MAIN_MENU_SUPPORT
        case GO_MENU_STYLE_INDEX_SHUTTER:
            go_menu_style = GO_MENU_STYLE_SHUTTER;
            break;
        #endif
        #ifdef GO_SPHERE_MAIN_MENU_SUPPORT
        case GO_MENU_STYLE_INDEX_SPHERE:
            go_menu_style = GO_MENU_STYLE_SPHERE;
            break;
        #endif
        #ifdef GO_CYLINDER_MAIN_MENU_SUPPORT
        case GO_MENU_STYLE_INDEX_CYLINDER:
            go_menu_style = GO_MENU_STYLE_CYLINDER;
            break;
        #endif
        #ifdef GO_SLOPE_MAIN_MENU_SUPPORT
        case GO_MENU_STYLE_INDEX_SLOPE:
            go_menu_style = GO_MENU_STYLE_SLOPE;
            break;
        #endif
        #ifdef GO_WHEEL_MAIN_MENU_SUPPORT
        case GO_MENU_STYLE_INDEX_WHEEL:
            go_menu_style = GO_MENU_STYLE_WHEEL;
            break;
        #endif
        #ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_OCEANWAVE:
            go_menu_style = GO_MENU_STYLE_OCEANWAVE;
            break;
        #endif    
        #ifdef GO_PEGTOP_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_PEGTOP:
            go_menu_style = GO_MENU_STYLE_PEGTOP;
            break;
        #endif    
        #ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_TURNTABLE:
            go_menu_style = GO_MENU_STYLE_TURNTABLE;
            break;
        #endif    
        #ifdef GO_BOXROT_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_BOXROT:
            go_menu_style = GO_MENU_STYLE_BOXROT;
            break;
        #endif 
        #ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_ICONBOXROT:
            go_menu_style = GO_MENU_STYLE_ICONBOXROT;
            break;
        #endif 
        #ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_BOOKPAGE:
            go_menu_style = GO_MENU_STYLE_BOOKPAGE;
            break;
        #endif 
        #ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_MIDROTATE:
            go_menu_style = GO_MENU_STYLE_MIDROTATE;
            break;
        #endif 
        #ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_SMALLITEMS:
            go_menu_style = GO_MENU_STYLE_SMALLITEMS;
            break;
        #endif 
        #ifdef GO_FLASH_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_FLASH:
            go_menu_style = GO_MENU_STYLE_FLASH;
            break;
        #endif 
        #ifdef GO_CONSTICK_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_CONSTICK:
            go_menu_style = GO_MENU_STYLE_CONSTICK;
            break;
        #endif 
        #ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_SYMMETRY:
            go_menu_style = GO_MENU_STYLE_SYMMETRY;
            break;
        #endif 
        #ifdef GO_BENZITEM_MAIN_MENU_SUPPORT  
        case GO_MENU_STYLE_INDEX_BENZITEM:
            go_menu_style = GO_MENU_STYLE_BENZITEM;
            break;
        #endif 

    default:
        //SCI_TRACE_LOW:"GetMainmenuStyleByIndex error item_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_1476_112_2_18_2_51_23_243,(uint8*)"d", item_index);
        break;
    }

    return go_menu_style;
}

/*************************************************************************************/
//  Description : listbox里添加主菜单显示格式
//  Global resource dependence : none
//  Author:
//  Note:  
/**************************************************************************************/
LOCAL void MMISETGOMenuTypeAppendItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      )
{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    uint8                       mode_id             = 0;
    uint32                      max_size            = ARR_SIZE(s_go_menu_type_text_id);
    
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, max_size, FALSE );
    GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

    //顺序添加五个列表项
    for( mode_id = 0; mode_id < max_size; mode_id++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = s_go_menu_type_text_id[mode_id];            
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
}

/*****************************************************************************/
//  Description : handle GOmenu type set win
//  Global resource dependence : 
//  Author:KYLE JIN
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGOMenuTypeSetWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM            param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_GOMENU_TYPE_CTRL_ID;
    //GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    //GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
	
    //SCI_TRACE_LOW:"HandleGOMenuTypeSetWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_1530_112_2_18_2_51_23_244,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISETGOMenuTypeAppendItem(ctrl_id); 
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16 sel_index = 0;                    
            sel_index = GetGOmenuStyleItemIndex(MMIAPISET_GetGOmenuStyle());
            GUILIST_SetSelectedItem(ctrl_id, sel_index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,sel_index);
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_MIDSK:
    case MSG_APP_OK:
        {
            GO_MENU_STYLE_E     go_menu_style  =   GO_MENU_STYLE_CLASSIC;
            uint16         cur_select_id   =   0;            
            cur_select_id = GUILIST_GetCurItemIndex( ctrl_id );            

			go_menu_style = GetGOmenuStyleByIndex(cur_select_id); // 获得类型
            if (go_menu_style != MMIAPISET_GetGOmenuStyle())
            {
                //set main menu style
                MMISET_SetGOmenuStyle(go_menu_style);
				
		        if (MMIMAINMENU_GO_SLIDE_E != MMIAPISET_GetMainmenuStyle())
		        {
		            //set main menu style
		            MMIAPISET_SetMainmenuStyle(MMIMAINMENU_GO_SLIDE_E);
		        }
				
            }
	        else
	        {
		        if (MMIMAINMENU_GO_SLIDE_E != MMIAPISET_GetMainmenuStyle())
		        {
		            //set main menu style
		            MMIAPISET_SetMainmenuStyle(MMIMAINMENU_GO_SLIDE_E);
		        }
	        }

            MMK_ReturnIdleWin();
				
        }
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        if(MMIMAINMENU_GO_SLIDE_E != MMIAPISET_GetMainmenuStyle())
        {
            //set main menu style
            MMIAPISET_SetMainmenuStyle(MMIMAINMENU_GO_SLIDE_E);
			MMK_ReturnIdleWin();
        }
        else
        {
            MMK_CloseWin(win_id);
        }
        break;
        
	case MSG_NOTIFY_LIST_SELECT_QUERY:
		{
			GUILIST_QRERY_SELECT_ITEM_T * mark_item_ptr = (GUILIST_QRERY_SELECT_ITEM_T *)param;
            
			mark_item_ptr->is_check_select = TRUE;
		}
		break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}
#endif

/*****************************************************************************/
//  Description : MMISET_GetMenuStyleTxt
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T  MMISET_GetMenuStyleTxt(void)
{
    uint16 menu_index = 0;    
    
    menu_index = GetMainmenuStyleItemIndex(MMIAPISET_GetMainmenuStyle());
    return s_mainmenu_type_text_id[menu_index];
}
#if defined(PDA_UI_SUPPORT_U_IDLE)

/*****************************************************************************/
//  Description : MMISET_GetUIdleStyleTxt
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMISET_GetUIdleStyleTxt(void)
{
    uint16 menu_index = 0;    
    
    menu_index = GetUIdleStyleItemIndex(MMIAPISET_GetUIdleStyle());/*lint !e64*/
    return s_u_idle_text_id[menu_index];
}

/*****************************************************************************/
//  Description : get uidle type item index
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetUIdleStyleItemIndex(UIDLE_STYLE_E uidle_style)
{
    uint16 cur_selection = 0;

    switch(uidle_style)
	{
	    case UIDLE_STYLE_CLASSIC:
	        cur_selection = UIDLE_STYLE_INDEX_CLASSIC;
	        break;

		case UIDLE_STYLE_RANDOM:
            cur_selection = UIDLE_STYLE_INDEX_RANDOM;
            break;

		case UIDLE_STYLE_SLOPE:
            cur_selection = UIDLE_STYLE_INDEX_SLOPE;
            break;

		case UIDLE_STYLE_BOXROT:
            cur_selection = UIDLE_STYLE_INDEX_BOXROT;
            break;

		case UIDLE_STYLE_BOOKPAGE:
            cur_selection = UIDLE_STYLE_INDEX_BOOKPAGE;
            break;            

		case UIDLE_STYLE_MIDROTATE:
            cur_selection = UIDLE_STYLE_INDEX_MIDROTATE;
            break;  

		case UIDLE_STYLE_SMALLITEMS:
            cur_selection = UIDLE_STYLE_INDEX_SMALLITEMS;
            break; 
            
	    default:
	        //SCI_TRACE_LOW:"GetMainmenuStyleItemIndex error type = %d"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_1357_112_2_18_2_51_23_242,(uint8*)"d", uidle_style);
	        break;
	    }

    return cur_selection;
}

/*****************************************************************************/
//  Description : get uidle item index
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL UIDLE_STYLE_E GetUIdleStyleByIndex(uint16 item_index)
{
    UIDLE_STYLE_E uidle_style = UIDLE_STYLE_CLASSIC;

    switch(item_index)
    {
	    case UIDLE_STYLE_INDEX_CLASSIC:
	        uidle_style = UIDLE_STYLE_CLASSIC;
	        break;

		case UIDLE_STYLE_INDEX_RANDOM:
            uidle_style = UIDLE_STYLE_RANDOM;
            break;

		case UIDLE_STYLE_INDEX_SLOPE:
            uidle_style = UIDLE_STYLE_SLOPE;
            break;

		case UIDLE_STYLE_INDEX_BOXROT:
            uidle_style = UIDLE_STYLE_BOXROT;
            break;

		case UIDLE_STYLE_INDEX_BOOKPAGE:
            uidle_style = UIDLE_STYLE_BOOKPAGE;
            break;            

		case UIDLE_STYLE_INDEX_MIDROTATE:
            uidle_style = UIDLE_STYLE_MIDROTATE;
            break;  

		case UIDLE_STYLE_INDEX_SMALLITEMS:
            uidle_style = UIDLE_STYLE_SMALLITEMS;
            break; 

        default:
            //SCI_TRACE_LOW:"GetMainmenuStyleByIndex error item_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_1476_112_2_18_2_51_23_243,(uint8*)"d", item_index);
            break;
    }

    return uidle_style;
}

/*************************************************************************************/
//  Description : append items to list.
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note:  
/**************************************************************************************/
LOCAL void MMISETUIdleTypeAppendItem(MMI_CTRL_ID_T ctrl_id)
{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    uint8                       mode_id             = 0;
    uint32                      max_size            = ARR_SIZE(s_u_idle_text_id);
    
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, max_size, FALSE );
    GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

    //顺序添加五个列表项
    for( mode_id = 0; mode_id < max_size; mode_id++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = s_u_idle_text_id[mode_id];            
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
}

#ifdef	MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
// 	Description : handle uidle set query window
//	Global resource dependence : none
//  Author: hongbo.lan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUIdleSetQueryWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              )
{
    MMI_RESULT_E   result       = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *info_ptr     = (MMIPUB_INFO_T*)MMK_GetWinAddDataEx(win_id);
    UIDLE_STYLE_E  uidle_style  = info_ptr->user_data;

    switch(msg_id)
    {
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //set main menu style
        MMISET_SetUIdleStyle(uidle_style);								
        MMIAPISET_ResetDefaultWallpaper();        
        MMK_ReturnIdleWin();
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}
#endif

/*****************************************************************************/
//  Description : handle UIdle type set win
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUIdleTypeSetWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM            param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_UIDLE_TYPE_CTRL_ID;
    //GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    //GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
	
    //SCI_TRACE_LOW:"HandleGOMenuTypeSetWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MAINMENU_1530_112_2_18_2_51_23_244,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISETUIdleTypeAppendItem(ctrl_id); 
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16 sel_index = 0;                    
            sel_index = GetUIdleStyleItemIndex((UIDLE_STYLE_E)MMIAPISET_GetUIdleStyle());
            GUILIST_SetSelectedItem(ctrl_id, sel_index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,sel_index);
        }
        break;

    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        {
            UIDLE_STYLE_E     uidle_style  =   UIDLE_STYLE_CLASSIC;
            uint16         cur_select_id   =   0;            
            cur_select_id = GUILIST_GetCurItemIndex( ctrl_id );            

			uidle_style = GetUIdleStyleByIndex(cur_select_id); // 获得类型

#ifdef	MMI_VIDEOWALLPAPER_SUPPORT
            if ((UIDLE_STYLE_CLASSIC != uidle_style) && MMIAPISETVWP_IsOpen())
            {
                MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),
                                              TXT_SET_QUERY_CLOSE_VIDEO_WALLPAPER, 
                                              IMAGE_PUBWIN_QUERY,
                                              PNULL, 
                                              HandleUIdleSetQueryWinMsg,
                                              (uint32)uidle_style);

                break;                              
                                              
            }
#endif
			
            if (uidle_style != (UIDLE_STYLE_E)MMIAPISET_GetUIdleStyle())
            {
                //set main menu style
                MMISET_SetUIdleStyle(uidle_style);								
            }

            MMK_ReturnIdleWin();
				
        }
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : 获取选中BY INDEX
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIMENU_SELECT_EFFECT_STYLE_E MMIAPISET_GetMainmenuSelectStyleByIndex(
                                              uint16 item_index
                                              )
{
    MMIMENU_SELECT_EFFECT_STYLE_E mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;

    switch (item_index)
    {
        case SELECT_EFFECT_INDEX_STATIC:
            mainmenu_select_style = SELECT_EFFECT_STATIC;
            break; 
        case SELECT_EFFECT_INDEX_ROUNDLIGHT:
            mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;
            break; 
        case SELECT_EFFECT_INDEX_FIRE:
            mainmenu_select_style = SELECT_EFFECT_FIRE;
            break; 
        case SELECT_EFFECT_INDEX_LIGHTING:
            mainmenu_select_style = SELECT_EFFECT_LIGHTING;
            break; 
        case SELECT_EFFECT_INDEX_WATER:
            mainmenu_select_style = SELECT_EFFECT_WATER;
            break; 
        case SELECT_EFFECT_INDEX_SNOW:
            mainmenu_select_style = SELECT_EFFECT_SNOW;
            break; 
        case SELECT_EFFECT_INDEX_RANDOM:
            mainmenu_select_style = SELECT_EFFECT_RANDOM;
            break; 
        default:
            //mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;
            //SCI_PASSERT(FALSE,("MMIAPISET_GetMainmenuSelectStyleByIndex:item_index %d is error!",item_index));/*assert verified*/
            break; 
            
    }

    return mainmenu_select_style;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMainmenuSelectStyleItemIndex(
                                       MMIMENU_SELECT_EFFECT_STYLE_E mainmenu_select_style
                                       )
{
    uint16 item_index = 0;

    switch(mainmenu_select_style)
    {
    case SELECT_EFFECT_STATIC: // 
        item_index = SELECT_EFFECT_INDEX_STATIC;
        break;
    case SELECT_EFFECT_ROUNDLIGHT: // 
        item_index = SELECT_EFFECT_INDEX_ROUNDLIGHT;
        break;
    case SELECT_EFFECT_FIRE: //
        item_index = SELECT_EFFECT_INDEX_FIRE;
        break;
    case SELECT_EFFECT_LIGHTING: //
        item_index = SELECT_EFFECT_INDEX_LIGHTING;
        break;
    case SELECT_EFFECT_WATER: //
        item_index = SELECT_EFFECT_INDEX_WATER;
        break;
    case SELECT_EFFECT_SNOW: //
        item_index = SELECT_EFFECT_INDEX_SNOW;
        break;
    case SELECT_EFFECT_RANDOM: //
        item_index = SELECT_EFFECT_INDEX_RANDOM;
        break;
    default:
        SCI_TRACE_LOW("mainmenu_select_style ERROR!");
        break;
    }

    return item_index;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T  MMISET_GetMenuSelectStyleTxt(void)
{
    uint16 menu_select_index = 0;    
    
    menu_select_index = GetMainmenuSelectStyleItemIndex(MMIAPISET_GetMainmenuSelectStyle());
    return s_mainmenu_select_type_text_id[menu_select_index];
}
/*****************************************************************************/
//  Description : main menu slect type set win  (highlight)
//  Global resource dependence : 
//  Author:kyle
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateMainmenuSelectStyleWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_MAINMENU_SELECT_STYLE_WIN_TAB,PNULL);
}
/*************************************************************************************/
//  Description : listbox里添加主菜单选中显示格式
//  Global resource dependence : none
//  Author:
//  Note:  
/**************************************************************************************/
LOCAL void MMISETMainMenuSelectTypeAppendItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      )
{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    uint8                       mode_id             = 0;
    uint32                      max_size            = ARR_SIZE(s_mainmenu_select_type_text_id);
    
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, max_size, FALSE );
    GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

    //顺序添加五个列表项
    for( mode_id = 0; mode_id < max_size; mode_id++ )
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = s_mainmenu_select_type_text_id[mode_id];            
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
}

/*****************************************************************************/
// 	Description : to handle set main menu select style win
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMainMenuSelectStyleWindow(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISET_SET_MAINMENU_SELECT_STYLE_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISETMainMenuSelectTypeAppendItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        {
            uint16 sel_index = 0;                    
            sel_index = GetMainmenuSelectStyleItemIndex(MMIAPISET_GetMainmenuSelectStyle());
            GUILIST_SetSelectedItem(ctrl_id, sel_index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,sel_index);
        }
        break;
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16         cur_select_id   =   0;   
            MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;

            cur_select_id = GUILIST_GetCurItemIndex( ctrl_id ); 
            mainmenu_select_style = MMIAPISET_GetMainmenuSelectStyleByIndex(cur_select_id);
            //set main menu style
            if(mainmenu_select_style != MMIAPISET_GetMainmenuSelectStyle())
            {
               MMISET_SetMainmenuSelectStyle(mainmenu_select_style);
            }
        }
        MMK_CloseWin( win_id );  
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif
//#endif


/*Edit by script, ignore 3 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527


/*Edit by script, ignore 5 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
