/*****************************************************************************
** File Name:      mmi_livewallpaper_preview_wintab.c                                    *
** Author:          arvin.wu                                                *
** Date:           01/08/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  08/2011      Arvin.wu            Create                                  *
******************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_id.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "mmiset_wintab.h"
#include "mmiset_internal.h"
#include "guibutton.h"
#include "mmilivewallpaper_export.h"
#include "mmiset_id.h"
#include "mmiset_display.h"
#include "mmi_image.h"

#ifndef _MMI_LIVEWALLPAPER_PREVIEW_WINTAB_C_
#define _MMI_LIVEWALLPAPER_PREVIEW_WINTAB_C_
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_LIVEWALLPAPER_PREVIEW_BUTTON_BOTTOM_MARGIN (20)
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : SetLiveWallpaperOKButtonCallback
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetLiveWallpaperOKButtonCallback(void);

/*****************************************************************************/
//  Description : SetLiveWallpaperOptionButtonCallback
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetLiveWallpaperOptionButtonCallback(void);
/*****************************************************************************/
//  Description : CreateSetWallPaperButton
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL void CreateSetWallPaperButton(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : CreateSetAndOptionmenuButton
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL void CreateSetAndOptionmenuButton(MMI_WIN_ID_T        win_id);
/*****************************************************************************/
//  Description : HandleWidgetAddMenuWinMsg
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLivewallpaperPreviewWinMsg
(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL GUI_LCD_DEV_INFO s_button_layer_info = {0,UILAYER_NULL_HANDLE};

LOCAL  BOOLEAN  s_is_need_handle = FALSE;
LOCAL  BOOLEAN  s_is_need_resume = TRUE;
//pb member option menu
WINDOW_TABLE(MMILIVEWALLPAPER_PREVIEW_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleLivewallpaperPreviewWinMsg),
    WIN_HIDE_STATUS,
    WIN_ID(MMILIVEWALLPAPER_PREVIEW_WIN_ID),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT),
    END_WIN
};
/*****************************************************************************/
//  Description : SetLiveWallpaperOKButtonCallback
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetLiveWallpaperOKButtonCallback(void)
{
    s_is_need_resume = FALSE;
    s_is_need_handle = TRUE;
    //MMK_ReturnIdleWin();
    //为了保持和静态壁纸统一，修改成回到设置界面
    MMK_CloseWin(MMILIVEWALLPAPER_PREVIEW_WIN_ID);
    MMK_CloseWin(MMISET_SET_DYNAMIC_WALLPAPER_WIN_ID);
    MMK_CloseWin(MMISET_SET_WALLPAPER_WIN_ID);

}

/*****************************************************************************/
//  Description : SetLiveWallpaperCancelButtonCallback
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetLiveWallpaperCancelButtonCallback()
{
    s_is_need_handle = FALSE;
    s_is_need_resume = TRUE;
    MMK_CloseWin(MMILIVEWALLPAPER_PREVIEW_WIN_ID);
}

/*****************************************************************************/
//  Description : SetStaticWallpaperOKButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetStaticWallpaperOKButtonCallback(void)
{
    MMISET_SetFixedWallpaper(MMISET_SET_STATIC_WALLPAPER_CTRL_ID);
    MMK_ReturnIdleWin();
}

/*****************************************************************************/
//  Description : SetStaticWallpaperCancelButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetStaticWallpaperCancelButtonCallback(void)
{
    MMK_CloseWin(MMK_GetFocusWinHandle());
}

/*****************************************************************************/
//  Description : SetLiveWallpaperOptionButtonCallback
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetLiveWallpaperOptionButtonCallback(void)
{
    s_is_need_resume = TRUE;
    s_is_need_handle = TRUE;
    MMIAPILIVEWALLPAPER_CreateSettingWindow();
}

/*****************************************************************************/
//  Description : create button layer.
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateButtonLayer(MMI_WIN_ID_T win_id)
{
    UILAYER_CREATE_T   create_info = {0};
    UILAYER_RESULT_E  result = UILAYER_RESULT_SUCCESS;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    /*创建BUTTON使用的层*/
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&lcd_width,&lcd_height);
    create_info.width           = lcd_width;
    create_info.height          = lcd_height;

    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;   

    result = UILAYER_CreateLayer(&create_info, &s_button_layer_info);
    if(UILAYER_RESULT_SUCCESS == result)
    {
        UILAYER_APPEND_BLT_T  append_blt = {0};
        UILAYER_Clear(&s_button_layer_info);
        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_button_layer_info;
        UILAYER_AppendBltLayer(&append_blt);         
        return TRUE;
    }
    else
    {
        s_button_layer_info.block_id = 0;
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : CreateButton
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void CreateButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id, GUI_BOTH_RECT_T both_rect)
{
    GUIBUTTON_INIT_DATA_T   init_data = {0};
    MMI_HANDLE_T handle = 0;
    GUI_FONT_ALL_T font_all = {0};
    GUI_BG_T btn_bg = {0};
    
    init_data.both_rect = both_rect;
    init_data.bg.bg_type = GUI_BG_IMG;
//#ifdef MMIPUBWIN_PDASTYLE_SUPPORT                
    init_data.bg.img_id = IMAGE_COMMON_PICVIEW_BUTTON;
//#else
//    init_data.bg.img_id = IMAGE_SOFTKEY_BG;
//#endif

    handle = GUIBUTTON_CreateDynamic(MMK_GetFocusWinId(), ctrl_id, &init_data);
    GUIBUTTON_SetTextId(handle, text_id);

    btn_bg.bg_type = GUI_BG_IMG;
#ifdef GUIF_SOFTKEY_PDASTYLE                    
    btn_bg.img_id = IMAGE_PUBWIN_BUTTON_HL_PDA;    
#else
    btn_bg.img_id = IMAGE_SOFTKEY_BG_DOWN;    
#endif
    GUIBUTTON_SetPressedBg(handle, &btn_bg);
    GUIBUTTON_SetRunSheen(handle, FALSE);

    font_all.color = MMI_BLACK_COLOR;
    font_all.font = MMI_DEFAULT_NORMAL_FONT;
    GUIBUTTON_SetFont(handle, &font_all);

}
/*****************************************************************************/
//  Description : CreateSetWallPaperButton
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL void CreateSetWallPaperButton(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    //MMI_CTRL_ID_T  ctrl_id=MMI_LIVEWALLPAPER_PREVIEW_SET_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T ctrl_cancel_id = MMI_LIVEWALLPAPER_PREVIEW_CANCEL_BUTTON_CTRL_ID;
    uint16    button_icon_width = 0;
    uint16    button_icon_height = 0;
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    GUI_RECT_T  rc = {0};
    GUI_BOTH_RECT_T  both_rect = {0};

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT            
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_PUBWIN_BUTTON_PDA, win_id);
#else
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_SOFTKEY_BG, win_id);
#endif
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);
    button_icon_width = layer_width/3 - 4;

    rc.left = ((layer_width/2) - button_icon_width)/2;
    rc.right = rc.left + button_icon_width;
    rc.bottom = layer_height - MMI_LIVEWALLPAPER_PREVIEW_BUTTON_BOTTOM_MARGIN;
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
    both_rect.h_rect = rc;
    CreateButton(win_id, ctrl_id, TXT_COMMON_OK, both_rect);

    rc.left = ((layer_width/2) - button_icon_width)/2 + (layer_width/2);
    rc.right = rc.left + button_icon_width;
    rc.bottom = layer_height - MMI_LIVEWALLPAPER_PREVIEW_BUTTON_BOTTOM_MARGIN;
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
    both_rect.h_rect = rc;
    CreateButton(win_id, ctrl_cancel_id, STXT_CANCEL, both_rect);
    if(MMISET_SET_STATIC_WALLPAPER_BUTTON_CTRL_ID == ctrl_id)
    {
        GUIBUTTON_SetCallBackFunc(ctrl_id, (GUIBUTTON_CALLBACK_FUNC)SetStaticWallpaperOKButtonCallback);
        GUIBUTTON_SetCallBackFunc(ctrl_cancel_id, (GUIBUTTON_CALLBACK_FUNC)SetStaticWallpaperCancelButtonCallback);
    }
    else
    {
        GUIBUTTON_SetCallBackFunc(ctrl_id, (GUIBUTTON_CALLBACK_FUNC)SetLiveWallpaperOKButtonCallback);
        GUIBUTTON_SetCallBackFunc(ctrl_cancel_id, (GUIBUTTON_CALLBACK_FUNC)SetLiveWallpaperCancelButtonCallback);
    }

    if(CreateButtonLayer(win_id))
    {
        MMK_SetCtrlLcdDevInfo(ctrl_id, &s_button_layer_info);
        MMK_SetCtrlLcdDevInfo(ctrl_cancel_id, &s_button_layer_info);
    }
}
/*****************************************************************************/
//  Description : CreateSetAndOptionmenuButton
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void CreateSetAndOptionmenuButton(MMI_WIN_ID_T        win_id)
{
    MMI_CTRL_ID_T  ctrl_set_id=MMI_LIVEWALLPAPER_PREVIEW_SET_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T  ctrl_option_id=MMI_LIVEWALLPAPER_PREVIEW_OPTION_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T  ctrl_cancel_id=MMI_LIVEWALLPAPER_PREVIEW_CANCEL_BUTTON_CTRL_ID;    
    uint16    button_icon_width = 0;
    uint16    button_icon_height = 0;
    UILAYER_CREATE_T   create_info = {0};
    uint16 layer_width = 0;
    uint16 layer_height = 0;
    GUI_RECT_T  rc = {0};
    GUI_BOTH_RECT_T  both_rect = {0};

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT    
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_PUBWIN_BUTTON_PDA, win_id);
#else
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_SOFTKEY_BG, win_id);
#endif

    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);
    button_icon_width = layer_width/3 - 4;

    rc.left = 0;
    rc.right = rc.left + button_icon_width;
    rc.bottom = layer_height - MMI_LIVEWALLPAPER_PREVIEW_BUTTON_BOTTOM_MARGIN;
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
    both_rect.h_rect = rc;

    CreateButton(win_id, ctrl_set_id, TXT_COMMON_OK, both_rect);
    GUIBUTTON_SetCallBackFunc(ctrl_set_id, (GUIBUTTON_CALLBACK_FUNC)SetLiveWallpaperOKButtonCallback);


    rc.left += (button_icon_width + 4);
    rc.right = rc.left + button_icon_width;
    both_rect.v_rect = rc;
    both_rect.h_rect = rc;

    CreateButton(win_id, ctrl_option_id, STXT_OPTION, both_rect);
    GUIBUTTON_SetCallBackFunc(ctrl_option_id, (GUIBUTTON_CALLBACK_FUNC)SetLiveWallpaperOptionButtonCallback);

    rc.left += (button_icon_width + 4);
    rc.right = rc.left + button_icon_width;
    both_rect.v_rect = rc;
    both_rect.h_rect = rc;

    CreateButton(win_id, ctrl_cancel_id, STXT_CANCEL, both_rect);
    GUIBUTTON_SetCallBackFunc(ctrl_cancel_id, (GUIBUTTON_CALLBACK_FUNC)SetLiveWallpaperCancelButtonCallback);

    if(CreateButtonLayer(win_id))
    {
        MMK_SetCtrlLcdDevInfo(ctrl_set_id,&s_button_layer_info);
        MMK_SetCtrlLcdDevInfo(ctrl_option_id,&s_button_layer_info);
        MMK_SetCtrlLcdDevInfo(ctrl_cancel_id,&s_button_layer_info);        
    }
}
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : HandleWidgetAddMenuWinMsg
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLivewallpaperPreviewWinMsg
(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result        = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id       = MMI_LIVEWALLPAPER_PREVIEW_SET_BUTTON_CTRL_ID;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        s_is_need_resume = TRUE;
        s_is_need_handle = TRUE;
        if(MMIAPILIVEWALLPAPER_IsNeedOptionMenu())
        {
            CreateSetAndOptionmenuButton(win_id);
        }
        else
        {
            CreateSetWallPaperButton(win_id,ctrl_id);
        }      
        break;
    case MSG_FULL_PAINT:
        //MMITHEME_ResumeScreen(win_id);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        SetLiveWallpaperOKButtonCallback();
        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if(MMIAPILIVEWALLPAPER_IsNeedOptionMenu())
        {
            SetLiveWallpaperOptionButtonCallback();
        }
        else
        {
            SetLiveWallpaperOKButtonCallback();
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_GET_FOCUS:
        s_is_need_resume = TRUE;
        s_is_need_handle = TRUE;
        /*if (UILAYER_IsLayerActive(&s_button_layer_info))
        {   
            UILAYER_APPEND_BLT_T  append_blt = {0};

            append_blt.layer_level = UILAYER_LEVEL_NORMAL;
            append_blt.lcd_dev_info = s_button_layer_info;
            MMK_SetCtrlLcdDevInfo(MMI_LIVEWALLPAPER_PREVIEW_SET_BUTTON_CTRL_ID,&s_button_layer_info);
            if(MMIAPILIVEWALLPAPER_IsNeedOptionMenu())
            {
                MMK_SetCtrlLcdDevInfo(MMI_LIVEWALLPAPER_PREVIEW_SET_BUTTON_CTRL_ID,&s_button_layer_info);
            }
            UILAYER_AppendBltLayer(&append_blt); 
        }*/
        MMIAPILIVEWALLPAPER_AppendSetWallPaperButton();
        break;
    case MSG_LOSE_FOCUS:
        //MMITHEME_SaveScreen(win_id);
        break;

	case MSG_NOTIFY_UPDATE:
        if(PNULL != param)
        {
            GUI_RECT_T			*display_rect_ptr = PNULL;
            display_rect_ptr = (GUI_RECT_T*)param;
            
        	if (UILAYER_IsLayerActive(&s_button_layer_info))
        	{
        		UILAYER_ClearRect(&s_button_layer_info, *display_rect_ptr);
        	}
        }
        break;
        
    case MSG_APP_RED:
        s_is_need_handle = FALSE;
        result = MMI_RESULT_FALSE;
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        s_is_need_handle = FALSE;
        MMK_CloseWin(win_id);
        break;
   case MSG_CLOSE_WINDOW:
        if(!s_is_need_handle || s_is_need_resume)
        {
            s_is_need_handle = FALSE; 
            MMIAPILIVEWALLPAPER_Stop();
            MMIAPISET_ResumePDAPreLivewallpaper();  //PDA and non-PDA could use same resume function due to same menu entrance .
        }
        if(UILAYER_IsLayerActive(&s_button_layer_info))
        {
            UILAYER_RELEASELAYER(&s_button_layer_info);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&s_button_layer_info);
        }
        //MMITHEME_ReleaseScreenSaver();
        s_button_layer_info.block_id = UILAYER_NULL_HANDLE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    if(s_is_need_handle)
    {
        MMIAPILIVEWALLPAPER_HandleEvent(win_id,msg_id,param);
    }
    return (result);
}
/*****************************************************************************/
//  Description : MMILIVEWALLPAPER_GetPreviewFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMILIVEWALLPAPER_GetPreviewFgInfo(LW_FG_INFO *lw_fg_info_ptr)
{
    
    if(lw_fg_info_ptr)
    {
        lw_fg_info_ptr->disp_dev_info.lcd_id = 0;
        lw_fg_info_ptr->disp_dev_info.block_id = 0;
        
        lw_fg_info_ptr->lw_fg_pos_info.rc = MMITHEME_GetFullScreenRect();//获得前景层的区域信息
        lw_fg_info_ptr->lw_fg_pos_info.x = 0;//由于预览界面没有前景层，所以前景层始终不变
        lw_fg_info_ptr->lw_fg_pos_info.y = 0;//由于预览界面没有前景层，所以前景层始终不变
    }
}

/*****************************************************************************/
//  Description : MMIAPILIVEWALLPAPER_CreatePreviewWin
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_CreatePreviewWin(void)
{
    MMK_CreateWin((uint32*)MMILIVEWALLPAPER_PREVIEW_WIN_TAB,PNULL);
}
/*****************************************************************************/
//  Description : MMIAPILIVEWALLPAPER_CreateSetWallPaperButton(供外部使用) 
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_CreateSetWallPaperButton(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    CreateSetWallPaperButton(win_id,ctrl_id);
}
/*****************************************************************************/
//  Description : MMIAPILIVEWALLPAPER_AppendSetWallPaperButton(供外部使用) 
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_AppendSetWallPaperButton(void)
{
    if (UILAYER_IsLayerActive(&s_button_layer_info))
    {   
        UILAYER_APPEND_BLT_T  append_blt = {0};

        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_button_layer_info;
        UILAYER_AppendBltLayer(&append_blt); 
    }
}
#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
