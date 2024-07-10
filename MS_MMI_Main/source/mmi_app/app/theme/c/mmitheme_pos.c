/*****************************************************************************
** File Name:      mmitheme_pos.c                                            *
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

#define _MMITHEME_POS_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilcd.h"
#include "guisoftkey.h"
#include "mmi_position.h"
#include "mmitheme_pos.h"
#include "mmidisplay_data.h"
#include "mmitheme_tab.h"
#include "mmitheme_softkey.h"
#include "mmi_theme.h"
#include "mmi_id.h"
#include "guimenu.h"
#if defined(MENU_CRYSTALCUBE_SUPPORT) || defined (MENU_CYCLONE_SUPPORT) 
//#include "mmiset.h"
#include "mmiset_export.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
/*****************************************************************************/
//  Description : is full screen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsFullScreenRect( 
                                         GUI_RECT_T     rect
                                         )
{
    BOOLEAN     result = FALSE;
    GUI_BOTH_RECT_T  screen_rect = {0};

    //get full screen rect
    screen_rect = MMITHEME_GetFullScreenBothRect();

    result = (BOOLEAN)(GUI_RectIsCovered( rect, screen_rect.v_rect ) || GUI_RectIsCovered( rect, screen_rect.h_rect ));

    return (result);
}

/*****************************************************************************/
//  Description : get full screen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetFullScreenRect(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_RECT_T      rect = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //set rect
    rect.right  = (int16)(lcd_width - 1);
    rect.bottom = (int16)(lcd_height - 1);

    return (rect);
}

/*****************************************************************************/
//  Description : get full screen rect by win handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetFullScreenRectEx(
                                               MMI_HANDLE_T win_handle
                                               )
{
    GUI_RECT_T          full_rect = {0};
    GUI_BOTH_RECT_T     both_rect = {0};

    both_rect = MMITHEME_GetFullScreenBothRect();

    if (MMK_IsWindowLandscape(win_handle))
    {
        full_rect = both_rect.h_rect;
    }
    else
    {
        full_rect = both_rect.v_rect;
    }

    return (full_rect);
}

/*****************************************************************************/
//  Description : get full screen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetFullScreenBothRect(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_BOTH_RECT_T full_rect = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //screen wide mode
    if ( lcd_width > lcd_height )
    {
        full_rect.h_rect.right  = (int16)(lcd_width - 1);
        full_rect.h_rect.bottom = (int16)(lcd_height - 1);

        full_rect.v_rect.right  = (int16)(lcd_height - 1);
        full_rect.v_rect.bottom = (int16)(lcd_width - 1);
    }
    else
    {
        full_rect.v_rect.right  = (int16)(lcd_width - 1);
        full_rect.v_rect.bottom = (int16)(lcd_height - 1);

        full_rect.h_rect.right  = (int16)(lcd_height - 1);
        full_rect.h_rect.bottom = (int16)(lcd_width - 1);
    }

    return (full_rect);
}

/*****************************************************************************/
//  Description : get client rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetClientRect(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_RECT_T      client_rect = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //set client rect
    client_rect.top    = MMI_TITLE_HEIGHT;
    client_rect.right  = (int16)(lcd_width - 1);
    client_rect.bottom = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1);

    return (client_rect);
}

/*****************************************************************************/
//  Description : get client rect by win handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetClientRectEx(
                                           MMI_HANDLE_T win_handle
                                           )
{
    GUI_RECT_T          client_rect = {0};
    GUI_BOTH_RECT_T     both_rect = {0};

    both_rect = MMITHEME_GetWinClientBothRect(win_handle);

    if (MMK_IsWindowLandscape(win_handle))
    {
        client_rect = both_rect.h_rect;
    }
    else
    {
        client_rect = both_rect.v_rect;
    }

    return (client_rect);
}

/*****************************************************************************/
//  Description : get win default client rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
// 判断是否有状态栏等，请用MMK_IsWinIncludeStatusbar,MMK_IsWinIncludeSoftkey,MMK_IsWinIncludeTitle
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetWinClientBothRect(
                                                     MMI_HANDLE_T win_handle
                                                     )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_BOTH_RECT_T client_rect = {0};

#if defined(MAINLCD_SIZE_128X64) //@ alex added in 2010.11.29
    if ( MMK_IsChildWin( win_handle ) )
    {
        uint16 status_h = 0;

        if (MMK_IsWinIncludeStatusbar(win_handle))
        {
            if(MMK_IsFocusWin(MAIN_GetIdleWinId()))
            {
                status_h = MMI_STATUSBAR_HEIGHT;
            }
            else
            {
                status_h = 0;
            }
        }

        client_rect.v_rect.top = MMI_TAB_HEIGHT + status_h;;
        client_rect.h_rect.top = MMI_TAB_HEIGHT_H + status_h;
    }
    else
    {
        //title
        if (MMK_IsWinIncludeTitle(win_handle))
        {
            client_rect.v_rect.top = MMI_TITLE_THEME_HEIGHT;
            client_rect.h_rect.top = MMI_TITLE_THEME_HEIGHT_H;
        }
        
        //is include status bar
        if (MMK_IsWinIncludeStatusbar(win_handle))
        {
            client_rect.v_rect.top = (int16)(client_rect.v_rect.top + MMI_STATUSBAR_HEIGHT);
            client_rect.h_rect.top = (int16)(client_rect.h_rect.top + MMI_STATUSBAR_HEIGHT);
        }
    }
#else    
    if ( MMK_IsChildWin( win_handle ) ) // 目前平台默认只有tab控件才有子窗口
    {
        uint16 status_h = 0;

        if (MMK_IsWinIncludeStatusbar(win_handle)) // TAB控件子窗口不会有状态栏，因此这里判断没有实际意义
        {
            status_h = MMI_STATUSBAR_HEIGHT;
        }
        
        if (MMK_IsParentWinIncStb(win_handle)) // 需要判断父窗口是否有状态栏控件
        {
            status_h = MMI_STATUSBAR_HEIGHT;
        }

        
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        //Iphone 风格TAB在下
        if (MMITHEME_IsIstyle())
        {
            // 这里修复拨号面板处，没有title的情况，不需要扣除title的区域
            // TODO: 这里是不和谐之处，以后需要注意改掉的
            if (MMK_GetWinDisplayStyleState(win_handle, WS_NO_DEFAULT_STYLE)
                && !MMK_IsWinIncludeTitle(win_handle))
            {
                client_rect.v_rect.top += (int16)(status_h);
                client_rect.h_rect.top += (int16)(status_h);
            }
            else
            {
                client_rect.v_rect.top += (int16)(MMI_TITLE_THEME_HEIGHT + status_h);
                client_rect.h_rect.top += (int16)(MMI_TITLE_THEME_HEIGHT_H + status_h);
            }
        }
        else
#endif
        {
            client_rect.v_rect.top = (int16)(MMI_TAB_HEIGHT + status_h);
            client_rect.h_rect.top = (int16)(MMI_TAB_HEIGHT_H + status_h);
        }
    }
    else
    {
        //title
        if (MMK_IsWinIncludeTitle(win_handle))
        {
            client_rect.v_rect.top = MMI_TITLE_THEME_HEIGHT;
            client_rect.h_rect.top = MMI_TITLE_THEME_HEIGHT_H;
        }
        
        //is include status bar
        if (MMK_IsWinIncludeStatusbar(win_handle))
        {
            client_rect.v_rect.top = (int16)(client_rect.v_rect.top + MMI_STATUSBAR_HEIGHT);
            client_rect.h_rect.top = (int16)(client_rect.h_rect.top + MMI_STATUSBAR_HEIGHT);
        }
    }
#endif //MAINLCD_SIZE_128X64

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //screen wide mode
    if ( lcd_width > lcd_height )
    {
        //set client rect
        client_rect.h_rect.right  = (int16)(lcd_width - 1);
        client_rect.h_rect.bottom = (int16)(lcd_height - 1);

        client_rect.v_rect.right  = (int16)(lcd_height - 1);
        client_rect.v_rect.bottom = (int16)(lcd_width - 1);
    }
    else
    {
        //set client rect
        client_rect.v_rect.right  = (int16)(lcd_width - 1);
        client_rect.v_rect.bottom = (int16)(lcd_height - 1);

        client_rect.h_rect.right  = (int16)(lcd_height - 1);
        client_rect.h_rect.bottom = (int16)(lcd_width - 1);
    }
    
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    //Iphone 风格TAB在下
    if (MMK_IsChildWin(win_handle)
        && MMITHEME_IsIstyle())
    {
        client_rect.v_rect.bottom -= MMI_ISTYLE_TAB_HEIGHT;
        client_rect.h_rect.bottom -= MMI_ISTYLE_TAB_HEIGHT_H;
    }
#endif

#ifdef GUIF_SOFTKEY_PDASTYLE    //pda大多数场景不显示softkey
    if (MMK_GetWinDisplayStyleState(win_handle, WS_HAS_BUTTON_SOFTKEY))
#endif
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle()
            && !MMK_GetWinDisplayStyleState(win_handle, WS_DISABLE_SOFTKEY_TO_TITLE)
            && !MMK_IsWinIncludeSoftkey(win_handle))
        {
            //no softkey
        }
        else
#endif
        {
            if (MMK_IsWinIncludeSoftkey(win_handle))
            {
                client_rect.v_rect.bottom = (int16)(client_rect.v_rect.bottom - MMITHEME_GetWinSoftkeyHeight(win_handle));/*lint !e737*/
                client_rect.h_rect.bottom = (int16)(client_rect.h_rect.bottom - MMITHEME_GetWinSoftkeyHeight(win_handle));/*lint !e737*/
            }
        }
    }

    return (client_rect);
}

/*****************************************************************************/
//  Description : get win default title rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetWinTitleBothRect(
                                                    MMI_HANDLE_T    win_handle
                                                    )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_BOTH_RECT_T title_rect = {0};

    //is include status bar
    if (MMK_IsWinIncludeStatusbar(win_handle))
    {
        title_rect.v_rect.top = title_rect.h_rect.top = MMI_STATUSBAR_HEIGHT;
    }

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    //screen wide mode
    if ( lcd_width > lcd_height )
    {
        title_rect.h_rect.right  = (int16)(lcd_width - 1);
        title_rect.h_rect.bottom = (int16)(title_rect.h_rect.top + MMI_TITLE_THEME_HEIGHT_H - 1);
        
        title_rect.v_rect.right  = (int16)(lcd_height - 1);
        title_rect.v_rect.bottom = (int16)(title_rect.v_rect.top + MMI_TITLE_THEME_HEIGHT - 1); 
    }
    else
    {
        title_rect.v_rect.right  = (int16)(lcd_width - 1);
        title_rect.v_rect.bottom = (int16)(title_rect.v_rect.top + MMI_TITLE_THEME_HEIGHT - 1);
        
        title_rect.h_rect.right  = (int16)(lcd_height - 1);
        title_rect.h_rect.bottom = (int16)(title_rect.h_rect.top + MMI_TITLE_THEME_HEIGHT_H - 1);
    }
#ifdef MAINLCD_SIZE_128X64
    if(title_rect.v_rect.bottom < title_rect.v_rect.top) //zss add begin
    {
        title_rect.v_rect.bottom = title_rect.v_rect.top;
    }   
    
    if(title_rect.h_rect.bottom < title_rect.h_rect.top) //zss add begin
    {
        title_rect.h_rect.bottom = title_rect.h_rect.top;
    } 
#endif
    return (title_rect);
}

/*****************************************************************************/
//  Description : get win default title rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetWinTitleHeight(
                                         void
                                         )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    // GUI_BOTH_RECT_T title_rect = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //screen wide mode
    if ( lcd_width > lcd_height )
    {
        return MMI_TITLE_THEME_HEIGHT_H;
    }
    else
    {
        return MMI_TITLE_THEME_HEIGHT;
    }
}

/*****************************************************************************/
//  Description : get tab rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetWinTabBothRect(
                                                  MMI_HANDLE_T   win_handle
                                                  )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_BOTH_RECT_T  tab_rect = {0};

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    //Iphone 风格TAB在下
    if (MMITHEME_IsIstyle())
    {
        //get lcd width and height
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

        //screen wide mode
        if ( lcd_width > lcd_height )
        {
            tab_rect.h_rect.right  = (int16)(lcd_width - 1);
            tab_rect.h_rect.bottom = (int16)(lcd_height - 1);
            tab_rect.h_rect.top = (int16)(tab_rect.h_rect.bottom - MMI_ISTYLE_TAB_HEIGHT_H + 1);
            
            tab_rect.v_rect.right  = (int16)(lcd_height - 1);
            tab_rect.v_rect.bottom = (int16)(lcd_width - 1);
            tab_rect.v_rect.top = (int16)(tab_rect.v_rect.bottom - MMI_ISTYLE_TAB_HEIGHT + 1);
        }
        else
        {
            tab_rect.v_rect.right  = (int16)(lcd_width - 1);
            tab_rect.v_rect.bottom = (int16)(lcd_height - 1);
            tab_rect.v_rect.top = (int16)(tab_rect.v_rect.bottom - MMI_ISTYLE_TAB_HEIGHT + 1);
            
            tab_rect.h_rect.right  = (int16)(lcd_height - 1);
            tab_rect.h_rect.bottom = (int16)(lcd_width - 1);
            tab_rect.h_rect.top = (int16)(tab_rect.h_rect.bottom - MMI_ISTYLE_TAB_HEIGHT_H + 1);
        }
    }
    else
#endif
    {
        //title
        if (MMK_IsWinIncludeTitle(win_handle))
        {
            tab_rect.v_rect.top = MMI_TITLE_THEME_HEIGHT;
            tab_rect.h_rect.top = MMI_TITLE_THEME_HEIGHT_H;
        }

        //is include status bar
        if (MMK_IsWinIncludeStatusbar(win_handle))
        {
            tab_rect.v_rect.top = (int16)(tab_rect.v_rect.top + MMI_STATUSBAR_HEIGHT);
            tab_rect.h_rect.top = (int16)(tab_rect.h_rect.top + MMI_STATUSBAR_HEIGHT);
        }

        //get lcd width and height
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

        //screen wide mode
        if ( lcd_width > lcd_height )
        {
            tab_rect.h_rect.right  = (int16)(lcd_width - 1);
            tab_rect.h_rect.bottom = (int16)(tab_rect.h_rect.top + MMI_TAB_HEIGHT_H - 1);
            
            tab_rect.v_rect.right  = (int16)(lcd_height - 1);
            tab_rect.v_rect.bottom = (int16)(tab_rect.v_rect.top + MMI_TAB_HEIGHT - 1);
        }
        else
        {
            tab_rect.v_rect.right  = (int16)(lcd_width - 1);
            tab_rect.v_rect.bottom = (int16)(tab_rect.v_rect.top + MMI_TAB_HEIGHT - 1);
            
            tab_rect.h_rect.right  = (int16)(lcd_height - 1);
            tab_rect.h_rect.bottom = (int16)(tab_rect.h_rect.top + MMI_TAB_HEIGHT_H - 1);
        }
    }
    
    return (tab_rect);
}

/*****************************************************************************/
//  Description : get status bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMITHEME_GetStatusBarBothRect(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_BOTH_RECT_T status_rect = {0};
    
    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //screen wide mode
    if ( lcd_width > lcd_height )
    {
        //set status bar rect
        status_rect.h_rect.right  = (int16)(lcd_width - 1);
        status_rect.h_rect.bottom = (int16)(MMI_STATUSBAR_HEIGHT - 1);
        
        status_rect.v_rect.right  = (int16)(lcd_height - 1);
        status_rect.v_rect.bottom = status_rect.h_rect.bottom;
    }
    else
    {
        //set status bar rect
        status_rect.v_rect.right  = (int16)(lcd_width - 1);
        status_rect.v_rect.bottom = (int16)(MMI_STATUSBAR_HEIGHT - 1);
        status_rect.h_rect.right  = (int16)(lcd_height - 1);
        status_rect.h_rect.bottom = status_rect.v_rect.bottom;
    }

    return (status_rect);
}

/*****************************************************************************/
//  Description : get softkey rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetSoftkeyRect(
                                          uint16    softkey_index
                                          )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          softkey_width = 0;
    GUI_RECT_T      softkey_rect = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //get softkey width
    softkey_width = (uint16)(lcd_width / GUISOFTKEY_BUTTON_NUM);

    switch (softkey_index)
    {
    case LEFT_BUTTON:
        softkey_rect.left   = 0;
        softkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
        softkey_rect.right  = (int16)(softkey_width - 1);
        softkey_rect.bottom = (int16)(lcd_height - 1);
        break;

    case MIDDLE_BUTTON:
        softkey_rect.left   = (int16)(softkey_width);
        softkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
        softkey_rect.right  = (int16)(softkey_width*2 - 1);
        softkey_rect.bottom = (int16)(lcd_height - 1);
        break;

    case RIGHT_BUTTON:
        softkey_rect.left   = (int16)(softkey_width*2);
        softkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
        softkey_rect.right  = (int16)(lcd_width - 1);
        softkey_rect.bottom = (int16)(lcd_height -1);
        break;

    default:
        //SCI_TRACE_LOW:"MMITHEME_GetSoftkeyRect: softkey_index %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_POS_580_112_2_18_3_3_9_20,(uint8*)"d",softkey_index);
        break;
    }

    return (softkey_rect);
}

/*****************************************************************************/
//  Description : get the right pixel of the main screen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetMainScreenRightPixel(void)
{
    uint16      lcd_width = 0;
    uint16      right_pixel = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,PNULL);

    //set right
    right_pixel = (uint16)(lcd_width - 1);

    return (right_pixel);
}

/*****************************************************************************/
//  Description : get the bottom pixel of the main screen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetMainScreenBottomPixel(void)
{
    uint16      lcd_height = 0;
    uint16      bottom_pixel = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,PNULL,&lcd_height);

    //set bottom
    bottom_pixel = (uint16)(lcd_height - 1);

    return (bottom_pixel);
}

/*****************************************************************************/
//  Description : get the message box rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetMsgBoxRect(void)
{
    GUI_RECT_T      rect = {0};

    rect.left   = MMI_PROMPT_WIN_LEFT;
    rect.top    = MMI_PROMPT_WIN_TOP;
    rect.right  = MMI_PROMPT_WIN_RIGHT;
    rect.bottom = MMI_PROMPT_WIN_BOTTOM;

    return (rect);
}

/*****************************************************************************/
//  Description : main lcd is landscape
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 主屏是否横屏
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMainScreenLandscape(void)
{
    BOOLEAN     result = FALSE;

    result = GUILCD_IsLandscape(GUI_MAIN_LCD_ID);

    return (result);
}

/*****************************************************************************/
//  Description : get client rect width
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClientRectWidth(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          client_width = 0;

    //get lcd width and height
    
    if (GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height))
    {
        client_width = lcd_width;
    }
    
    return (client_width);
}

/*****************************************************************************/
//  Description : get client rect height
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClientRectHeight(void)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          client_height = 0;

    //get lcd width and height
    
    if (GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height))
    {
        client_height = (uint16)(lcd_height - MMI_TITLE_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
    }
    
    return (client_height);
}

/*****************************************************************************/
//  Description : get client rect height by win handle
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetClientRectHeightEx(MMI_HANDLE_T   win_handle)
{
    uint16              height = {0};
    GUI_BOTH_RECT_T     both_rect = {0};
    
    both_rect = MMITHEME_GetWinClientBothRect(win_handle);
    
    if (MMK_IsWindowLandscape(win_handle))
    {
        height = both_rect.h_rect.bottom - both_rect.h_rect.top +1;
    }
    else
    {
        height = both_rect.v_rect.bottom - both_rect.v_rect.top +1;
    }
    
    return (height);
}
