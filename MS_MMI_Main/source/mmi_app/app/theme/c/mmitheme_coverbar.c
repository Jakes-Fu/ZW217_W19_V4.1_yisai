/*****************************************************************************
** File Name:      mmitheme_coverbar.c                                           *
** Author:                                                                   *
** Date:           06/24/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/24/2011      bin.ji              Creat
******************************************************************************/

#define _MMITHEME_COVERBAR_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "guitext.h"
#include "guiblock.h"
#include "mmitheme_coverbar.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#include "ui_layer.h"
#include "guires.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//coverbar高度
#if defined (MAINLCD_SIZE_240X320)||defined (MAINLCD_SIZE_240X400)
#define MMITHEME_COVERBAR_HEIGHT            8
#else 
#define MMITHEME_COVERBAR_HEIGHT            20
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

//coverbar类型: top, bottom各一个
typedef enum
{
    MMITHEME_COVERBAR_TOP,
    MMITHEME_COVERBAR_BOTTOM,

    MMITHEME_COVERBAR_MAX
}MMITHEME_COVERBAR_E;

//创建图层方式，影响多层合成的性能,暂时屏蔽,后续可以让list\menu自己实现蒙版效果。改成提供显示蒙板效果接口
#if 0

//coverbar内容
typedef struct 
{
    GUI_LCD_DEV_INFO lcd_dev_info;
}MMITHEME_COVERBAR_ITEM_T;

//coverbar内容
typedef struct 
{
    BOOLEAN is_user_rect_enable;
    GUI_RECT_T user_rect;
    MMITHEME_COVERBAR_ITEM_T item_info[MMITHEME_COVERBAR_MAX];
}MMITHEME_COVERBAR_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

//全局蒙板，包括top, bottom各一个
LOCAL MMITHEME_COVERBAR_T s_coverbar_info = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : change coverbar rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_ChangeCoverBarRect(
                                        GUI_RECT_T user_rect
                                        )
{
    s_coverbar_info.is_user_rect_enable = TRUE;
    s_coverbar_info.user_rect = user_rect;
}

/*****************************************************************************/
//  Description : restore coverbar rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void RestoreCoverBarRect(void)
{
    s_coverbar_info.is_user_rect_enable = FALSE;
}

/*****************************************************************************/
//  Description : create cover bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CreateCoverBar(
                                    MMI_WIN_ID_T win_id
                                    )
{
    UILAYER_CREATE_T  create_info = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T coverbar_rect[MMITHEME_COVERBAR_MAX] = {0};
    uint32 i = 0;
    GUI_BOTH_RECT_T both_rect = {0};
    GUI_RECT_T client_rect = {0};
    MMI_IMAGE_ID_T  cover_bg_id[MMITHEME_COVERBAR_MAX] = {IMAGE_COMMON_COVER_BAR_TOP, IMAGE_COMMON_COVER_BAR_BOTTOM};

    //calc coverbar_rect
    if (s_coverbar_info.is_user_rect_enable)
    {
        client_rect = s_coverbar_info.user_rect;
    }
    else
    {
        both_rect = MMITHEME_GetWinClientBothRect(win_id);
        if (MMITHEME_IsMainScreenLandscape())
        {
            client_rect = both_rect.h_rect;
        }
        else
        {
            client_rect = both_rect.v_rect;
        }   
    }
    coverbar_rect[0] = coverbar_rect[1] = client_rect;
    coverbar_rect[0].bottom = coverbar_rect[0].top + MMITHEME_COVERBAR_HEIGHT -1;
    coverbar_rect[1].top = coverbar_rect[1].bottom - MMITHEME_COVERBAR_HEIGHT + 1;
    for (i = 0; i < MMITHEME_COVERBAR_MAX; i++)
    {
        if (UILAYER_HANDLE_MAIN != UILAYER_GetHandleType(&(s_coverbar_info.item_info[i].lcd_dev_info)))
        {
            UILAYER_RELEASELAYER(&(s_coverbar_info.item_info[i].lcd_dev_info));   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&(s_coverbar_info.item_info[i].lcd_dev_info));
//            s_coverbar_info.item_info[i].lcd_dev_info.lcd_id = 0;
//            s_coverbar_info.item_info[i].lcd_dev_info.block_id = 0;            
        }
        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = coverbar_rect[i].left;
        create_info.offset_y = coverbar_rect[i].top;
        create_info.width = (coverbar_rect[i].right+1-coverbar_rect[i].left);
        create_info.height = (coverbar_rect[i].bottom+1-coverbar_rect[i].top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        create_info.format = UILAYER_MEM_IMMUTABLE;
        if (UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(
            &create_info,
            &lcd_dev_info))
        {
            s_coverbar_info.item_info[i].lcd_dev_info = lcd_dev_info;

            append_layer.lcd_dev_info = lcd_dev_info;
            append_layer.layer_level = UILAYER_LEVEL_HIGH;
            UILAYER_AppendBltLayer(&append_layer);

            GUIRES_DisplayImg(PNULL,
                &coverbar_rect[i],
                PNULL,
                win_id,
                cover_bg_id[i],
                (const GUI_LCD_DEV_INFO*)&lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : destroy cover bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DestroyCoverBar(void)
{
    uint32 i = 0;

    RestoreCoverBarRect();
    for (i = 0; i < MMITHEME_COVERBAR_MAX; i++)
    {
        if (UILAYER_HANDLE_MAIN != UILAYER_GetHandleType(&(s_coverbar_info.item_info[i].lcd_dev_info)))
        {
            UILAYER_RELEASELAYER(&(s_coverbar_info.item_info[i].lcd_dev_info));   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&(s_coverbar_info.item_info[i].lcd_dev_info));
//            s_coverbar_info.item_info[i].lcd_dev_info.lcd_id = 0;
//            s_coverbar_info.item_info[i].lcd_dev_info.block_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : 根据窗口属性判断是否要coverbar效果
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsNeedCoverBar(
                                       MMI_HANDLE_T win_handle
                                       )
{
    if (MMK_GetWinDisplayStyleState(win_handle, WS_HAS_COVER_BAR)
        && !MMK_GetWinDisplayStyleState(win_handle, WS_HAS_TRANSPARENT))
    {
        return TRUE;
    }
    return FALSE;
}

#endif

/*****************************************************************************/
//  Description : 在client_rect区域上下部显示蒙板效果
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayCoverBar(
                                     MMI_WIN_ID_T win_id,
                                     const GUI_LCD_DEV_INFO *dev_info_ptr,
                                     GUI_RECT_T client_rect
                                     )
{
#ifdef MMI_PDA_SUPPORT
    MMI_IMAGE_ID_T  cover_bg_id[MMITHEME_COVERBAR_MAX] = {IMAGE_COMMON_COVER_BAR_TOP, IMAGE_COMMON_COVER_BAR_BOTTOM};
    GUI_RECT_T coverbar_rect[MMITHEME_COVERBAR_MAX] = {0};
    uint32 i = 0;
//    uint32 cur_tick  = 0;
    uint16 top_width = 0;
    uint16 top_height = 0;
    uint16 bottom_width = 0;
    uint16 bottom_height = 0;

    MMITHEME_GetCoverBarWidthHeight( win_id, &top_width, &top_height, &bottom_width, &bottom_height );

//     GUIRES_GetImgWidthHeight(&top_width, &top_height, IMAGE_COMMON_COVER_BAR_TOP, win_id);
//     GUIRES_GetImgWidthHeight(&bottom_width, &bottom_height, IMAGE_COMMON_COVER_BAR_BOTTOM, win_id);
//    cur_tick = SCI_GetTickCount();
    //calc coverbar_rect
    coverbar_rect[0] = coverbar_rect[1] = client_rect;
    coverbar_rect[0].bottom = coverbar_rect[0].top + top_height -1;
    coverbar_rect[1].top = coverbar_rect[1].bottom - bottom_height + 1;
    for (i = 0; i < MMITHEME_COVERBAR_MAX; i++)
    {
        GUIRES_DisplayImg(PNULL,
            &coverbar_rect[i],
            PNULL,
            win_id,
            cover_bg_id[i],
            dev_info_ptr);
    }
//    SCI_TRACE_LOW("MMITHEME_DisplayCoverBar() tick = %d", SCI_GetTickCount()-cur_tick);
#endif
}

/*****************************************************************************/
//  Description : get image width height
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetCoverBarWidthHeight(
                                            MMI_WIN_ID_T win_id,
                                            uint16 *top_width_ptr, 
                                            uint16 *top_height_ptr,
                                            uint16 *bottom_width_ptr, 
                                            uint16 *bottom_height_ptr
                                            )
{
    if ( PNULL == top_width_ptr
        || PNULL == top_height_ptr
        || PNULL == bottom_width_ptr
        || PNULL == bottom_height_ptr )
    {
        return;
    }
    
    *top_width_ptr = *top_height_ptr = *bottom_width_ptr = *bottom_height_ptr = 0;
#ifdef MMI_PDA_SUPPORT    
    GUIRES_GetImgWidthHeight(top_width_ptr, top_height_ptr, IMAGE_COMMON_COVER_BAR_TOP, win_id);
    GUIRES_GetImgWidthHeight(bottom_width_ptr, bottom_height_ptr, IMAGE_COMMON_COVER_BAR_BOTTOM, win_id);
#endif
}

