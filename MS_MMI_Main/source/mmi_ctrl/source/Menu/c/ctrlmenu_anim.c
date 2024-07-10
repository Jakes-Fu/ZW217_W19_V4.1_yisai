/*****************************************************************************
** File Name:      ctrlmenu_anim.c                                           *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmitheme_menu.h"
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
#include "ctrlmenu.h"
#include "ctrlmenu_export.h"
#include "mmi_theme.h"
#include "mmitheme_layer_special.h"
#include "guictrl_api.h"
//#include "mmi_nv.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMIMENU_MOVE_FRAME_NUM      4
#define MMIMENU_MOVE_STEP           10

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePopupLayer(
                               CTRLMENU_OBJ_T       *menu_ctrl_ptr,     // in
                               GUI_LCD_DEV_INFO     *lcd_dev_info_ptr,  // out
                               BOOLEAN              is_mask_layer
                               );

/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAnimLayer(
                              int16                 width,              // in
                              int16                 height,             // in
                              UILAYER_MEM_FORMAT_T  mem_format,         // in
                              GUI_LCD_DEV_INFO      *lcd_dev_info_ptr   // out
                              );

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseAnimLayer(
                            GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                            );

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendAnimLayer(
                           GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                           );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle msg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_HandleAnimMsg(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                                   MMI_MESSAGE_ID_E msg_id
                                   )
{
    uint16              i = 0;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    CAF_HANDLE_T        button_handle = 0;
    UILAYER_INFO_T      layer_info = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    switch (msg_id)
    {
    case MSG_CTL_GET_FOCUS:
    case MSG_CTL_OPEN:
#ifdef MMI_MENUMASKLAYER_SUPPORT
        if (GUIMENU_STYLE_POPUP != menu_ctrl_ptr->cur_style
            && GUIMENU_STYLE_OPTIONS != menu_ctrl_ptr->cur_style)
        {
            if (CreatePopupLayer(menu_ctrl_ptr, &menu_ctrl_ptr->mask_layer_handle, TRUE))
            {
                AppendAnimLayer(&menu_ctrl_ptr->mask_layer_handle);
            }
        }
#endif

        if (CreatePopupLayer(menu_ctrl_ptr, &menu_ctrl_ptr->popup_layer_handle, FALSE))
        {
            AppendAnimLayer(&menu_ctrl_ptr->popup_layer_handle);
        }

        // 需要重设下button的图层
        for (i = 0; i < CTRLMENU_BUTTON_NUM; i++)
        {
            if (0 != menu_ctrl_ptr->button_handle[i])
            {
                button_handle = menu_ctrl_ptr->button_handle[i];
                lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);
                GUIAPICTRL_SetLcdDevInfo(button_handle, lcd_dev_info_ptr);
            }
        }
        break;

    case MSG_CTL_LOSE_FOCUS:
#ifdef UI_MULTILAYER_SUPPORT
        if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->popup_layer_handle))
        {
            ReleaseAnimLayer(&menu_ctrl_ptr->popup_layer_handle);
        }

#ifdef MMI_MENUMASKLAYER_SUPPORT
        if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->mask_layer_handle))
        {
            ReleaseAnimLayer(&menu_ctrl_ptr->mask_layer_handle);
        }
#endif
#endif
        break;

    case MSG_CTL_CLOSE:
        //get lcd width and height
        GUILCD_GetLogicWidthHeight(
                base_ctrl_ptr->lcd_dev_info.lcd_id,
                &lcd_width, &lcd_height);

        UILAYER_GetLayerInfo(
                &menu_ctrl_ptr->popup_layer_handle,
                &layer_info);

        // 判断是否需要动画，在横竖屏切换时，不需要
        // 这里menu_ctrl_ptr->popup_layer_handle是肯定有的，且是屏幕大小
        if (lcd_width == layer_info.layer_width
            && lcd_height == layer_info.layer_height)
        {
            // 关闭动画
            if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style
                || GUIMENU_STYLE_OPTIONS == menu_ctrl_ptr->cur_style)
            {
                CTRLMENU_MoveDown(menu_ctrl_ptr, TRUE);
            }
            else
            {
                CTRLMENU_Zoom(menu_ctrl_ptr, TRUE);
            }

#ifdef UI_MULTILAYER_SUPPORT
            if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->popup_layer_handle))
            {
                ReleaseAnimLayer(&menu_ctrl_ptr->popup_layer_handle);
            }

#ifdef MMI_MENUMASKLAYER_SUPPORT
            if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->mask_layer_handle))
            {
                ReleaseAnimLayer(&menu_ctrl_ptr->mask_layer_handle);
            }
#endif
#endif

            MMITHEME_UpdateRect();
        }
        else
        {
#ifdef UI_MULTILAYER_SUPPORT
            if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->popup_layer_handle))
            {
                ReleaseAnimLayer(&menu_ctrl_ptr->popup_layer_handle);
            }

#ifdef MMI_MENUMASKLAYER_SUPPORT
            if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->mask_layer_handle))
            {
                ReleaseAnimLayer(&menu_ctrl_ptr->mask_layer_handle);
            }
#endif
#endif
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : reset layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ResetAnimLayer(
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                    )
{
    // 加上焦点判断，不然在失焦状态下转屏，会重新创建并添加到层系统中，显示错乱
    if (MMK_IsFocusWin(menu_ctrl_ptr->win_handle))
    {
        CTRLMENU_HandleAnimMsg(menu_ctrl_ptr, MSG_CTL_LOSE_FOCUS);
        CTRLMENU_HandleAnimMsg(menu_ctrl_ptr, MSG_CTL_GET_FOCUS);
    }
}

/*****************************************************************************/
//  Description : move down
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_MoveDown(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              BOOLEAN           is_down
                              )
{
#ifdef UI_MULTILAYER_SUPPORT
	int32 i = 0;

    if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->popup_layer_handle))
    {
        if (is_down)    // 向下动画
        {
            for (i = 0; i < MMIMENU_MOVE_FRAME_NUM; i++)
            {
                UILAYER_SetLayerPosition(
                        &menu_ctrl_ptr->popup_layer_handle,
                        0, 
                        (int16)(i * MMIMENU_MOVE_STEP));

                GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
            }

            // 清掉里面的内容，并恢复到初始位置
            UILAYER_Clear(&menu_ctrl_ptr->popup_layer_handle);
            UILAYER_SetLayerPosition(&menu_ctrl_ptr->popup_layer_handle, 0, 0);
        }
        else            // 向上动画
        {
            for (i = 0; i < MMIMENU_MOVE_FRAME_NUM; i++)
            {
                UILAYER_SetLayerPosition(
                        &menu_ctrl_ptr->popup_layer_handle,
                        0, 
                        (int16)((MMIMENU_MOVE_FRAME_NUM - i - 1) * MMIMENU_MOVE_STEP));

                GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
            }
        }
    }
#endif
}

/*****************************************************************************/
//  Description : zoom anim
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Zoom(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                          BOOLEAN           is_zoom_out
                          )
{
#ifdef MMI_MENUMASKLAYER_SUPPORT
    BOOLEAN                 result = FALSE;
#endif
#ifdef EFFECT_ZOOM_SUPPORT
    MMITHEME_ZOOM_DATA_T    zoom_data = {0};
    GUI_RECT_T              zoom_rect = {0};
#endif

    // 没有层，不用做动画
#ifdef UI_MULTILAYER_SUPPORT
    if (!CTRLMENU_IsLayerValid(&menu_ctrl_ptr->popup_layer_handle))
#endif
    {
        return;
    }

#ifdef MMI_MENUMASKLAYER_SUPPORT
    // 如果没创建蒙层，创建之
#ifdef UI_MULTILAYER_SUPPORT
    if (!CTRLMENU_IsLayerValid(&menu_ctrl_ptr->mask_layer_handle))
#endif
    {
        result = CreatePopupLayer(menu_ctrl_ptr, &menu_ctrl_ptr->mask_layer_handle, TRUE);
        if (!result)
        {
            return;
        }
    }
#endif

#ifdef EFFECT_ZOOM_SUPPORT
    zoom_data.content_lcd_dev = menu_ctrl_ptr->popup_layer_handle;
    zoom_data.mask_lcd_dev = menu_ctrl_ptr->mask_layer_handle;
    zoom_data.mask_alpha = menu_ctrl_ptr->pop_theme.mask_alpha;
    zoom_data.mask_color = menu_ctrl_ptr->pop_theme.mask_color;


    zoom_rect = GUI_AddupRect( &menu_ctrl_ptr->title_rect, &menu_ctrl_ptr->bg_rect );
    zoom_rect = GUI_AddupRect( &menu_ctrl_ptr->button_rect, &zoom_rect );
	zoom_data.rect_ptr = &zoom_rect;

    MMITHEME_PlayZoomAnim(&zoom_data, is_zoom_out);
#else
    GUI_INVALID_PARAM(is_zoom_out);
#endif

    // 重添MENU层
    AppendAnimLayer(&menu_ctrl_ptr->mask_layer_handle);

    // MMITHEME_PlayZoomAnim不会remove此层，但会remove掉mask_layer，所以这里先remove后append
    UILAYER_RemoveBltLayer(&menu_ctrl_ptr->popup_layer_handle);
    AppendAnimLayer(&menu_ctrl_ptr->popup_layer_handle);
}

/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePopupLayer(
                               CTRLMENU_OBJ_T       *menu_ctrl_ptr,     // in
                               GUI_LCD_DEV_INFO     *lcd_dev_info_ptr,  // out
                               BOOLEAN              is_mask_layer
                               )
{
    BOOLEAN         result = FALSE;
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(base_ctrl_ptr->lcd_dev_info.lcd_id,&lcd_width,&lcd_height);

    result = CreateAnimLayer(lcd_width,
                lcd_height,
                (UILAYER_MEM_FORMAT_T)(is_mask_layer ? UILAYER_MEM_IMMUTABLE : UILAYER_MEM_FPS_ACCELERATE),
                lcd_dev_info_ptr);

    return result;
}

/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAnimLayer(
                              int16                 width,              // in
                              int16                 height,             // in
                              UILAYER_MEM_FORMAT_T  mem_format,         // in
                              GUI_LCD_DEV_INFO      *lcd_dev_info_ptr   // out
                              )
{
    BOOLEAN             result = FALSE;
    UILAYER_RESULT_E    layer_result = UILAYER_RESULT_SUCCESS;
    UILAYER_CREATE_T    create_info = {0};

    if (PNULL != lcd_dev_info_ptr
        && (0 == lcd_dev_info_ptr->block_id || UILAYER_NULL_HANDLE == lcd_dev_info_ptr->block_id)
        && UILAYER_IsMultiLayerEnable())
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = 0;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE;
        create_info.format = mem_format;

        layer_result = UILAYER_CreateLayer(&create_info, lcd_dev_info_ptr);

        if (UILAYER_RESULT_SUCCESS == layer_result)
        {
            if (UILAYER_IsLayerActive(lcd_dev_info_ptr))
            {
                UILAYER_SetLayerColorKey(lcd_dev_info_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
                UILAYER_Clear(lcd_dev_info_ptr);

                result = TRUE;
            }
            else
            {
                // 没内存也要释放掉，不做动画
                ReleaseAnimLayer(lcd_dev_info_ptr);
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseAnimLayer(
                            GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                            )
{
    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        UILAYER_RELEASELAYER(lcd_dev_info_ptr);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendAnimLayer(
                           GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                           )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        append_layer.lcd_dev_info = *lcd_dev_info_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;

        UILAYER_AppendBltLayer(&append_layer);
    }
}
#endif
