/*****************************************************************************
** File Name:      ctrlmenu_sec.c                                             *
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

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_msg.h"
#include "mmk_tp.h"
#include "ctrlmenu.h"
#include "ctrlmenu_export.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmitheme_list.h"
#include "mmitheme_coverbar.h"
#include "guiprgbox.h"
#include "guictrl_api.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define CTRLMENU_ITEM_SCREENBAR_GAP    DP2PX_VALUE(2)
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**---------------------------------------------------------------------------*/
#ifdef MEDIA_DEMO_SUPPORT
extern uint8 get_video_state(void);/*lint !e526*/
extern uint8 get_audio_state(void);/*lint !e526*/
#endif

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         LOCAL FUNCTION DECLARE                            *
 *---------------------------------------------------------------------------*/
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 调用该函数之前，先调用ReleaseSecondMenuPageLayer
/*****************************************************************************/
LOCAL void CreateSecondMenuPageLayer(
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr  //in
                                     );

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseSecondMenuPageLayer(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                      );

/*****************************************************************************/
//  Description : 合并层，由于最多只有两个Page合并到主层上，因此只需要两个层的参数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetBltSecondMenuPageLayer(
                                     UILAYER_HANDLE_T   layer1,
                                     UILAYER_HANDLE_T   layer2
                                     );

/*****************************************************************************/
//  Description : 合并层，由于最多只有两个Page合并到主层上，因此只需要两个层的参数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetBltSecondMenuNotClear(
                                    UILAYER_HANDLE_T    layer1,
                                    UILAYER_HANDLE_T    layer2
                                    );
#endif

/*****************************************************************************/
//  Description : display selected bar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightItem(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : Clear Icon Rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ClearSecMenuItemRect(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                uint16          item_index
                                );

/*****************************************************************************/
//  Description : GetSecMenuItemDispLcdDevInfo
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetSecMenuItemDispLcdDevInfo(
                                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                                    );

/*****************************************************************************/
//  Description : GetSecMenuItemRect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemRect(
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                    uint16          item_index
                                    );

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedScrollBar(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLineNumPage(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetScrollBarWidth(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMenuBg(
                         GUI_RECT_T         bg_rect,
                         CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                         GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
                         );

/*****************************************************************************/
//  Description : display selected bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightItemBg(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr, //in
                                  uint16            item_index,     //in
                                  GUI_LCD_DEV_INFO  lcd_dev_info
                                  );

/*****************************************************************************/
//  Description : display one item include icon, text, no select bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplaySecMenuItem(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              uint16            item_index,
                              BOOLEAN           is_draw_focus,
                              GUI_LCD_DEV_INFO  lcd_dev_info
                              );

/*****************************************************************************/
//  Description : display item icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 DisplaySecMenuItemIcon(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    uint16              item_index,
                                    GUI_RECT_T          disp_rect,
                                    GUI_RECT_T          clip_rect,
                                    GUI_LCD_DEV_INFO    lcd_dev_info
                                    );

/*****************************************************************************/
//  Description : get icon image id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetItemIconId(
                                   uint16           item_index,
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : get icon image buffer
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:该函数仅适用于动态menu，并且menu的图片icon类型为buffer的情况
/*****************************************************************************/
LOCAL CTRLMENU_SELECT_ICON_DATA_T GetItemIconBuffer(
													uint16							item_index,
													CTRLMENU_OBJ_T					*menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : get icon display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemIconRect(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                        GUI_RECT_T      item_rect,
                                        MMI_IMAGE_ID_T  icon_id,
                                        uint16          *width_ptr,
                                        uint16          *height_ptr
                                        );

/*****************************************************************************/
//  Description : get icon buffer display rect
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemBufferRect(
										  CTRLMENU_OBJ_T  *menu_ctrl_ptr,
										  GUI_RECT_T      item_rect,
										  uint16          item_index
                                        );

/*****************************************************************************/
//  Description : display item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySecMenuItemText(
                                  CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                                  uint16           item_index,
                                  GUI_RECT_T       item_rect,
                                  GUI_RECT_T       clip_rect,
                                  uint16           icon_width,
                                  BOOLEAN          is_draw_focus_text,
                                  GUI_LCD_DEV_INFO lcd_dev_info
                                  );

/*****************************************************************************/
//  Description : get item text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemTextRect(
                                        CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                        GUI_RECT_T     item_rect,
                                        int16          icon_gap_width
                                        );

/*****************************************************************************/
//  Description : display one item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayOneItem(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                          GUI_RECT_T        item_rect,
                          uint16            index,
                          BOOLEAN           is_draw_focus,
                          GUI_LCD_DEV_INFO  lcd_dev_info
                          );

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawSecMenuItemSplitLine(
                                    CTRLMENU_OBJ_T          *menu_ctrl_ptr,
                                    uint16                  item_index,
                                    GUI_RECT_T              item_rect,
                                    const GUI_LCD_DEV_INFO  *lcd_dev_info_ptr
                                    );

/*****************************************************************************/
//  Description : reset Y offset
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int16 ResetSecMenuOffsetY(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateSecSlideInfo(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                              );

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateSecItemRect(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr  //in
                                   );

/*****************************************************************************/
//  Description : set second menu title param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetSecondTitle(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : reset
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetSecMenuStartIndex(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : display one item include icon,text and select bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySecMenuAllItem(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                 BOOLEAN            is_draw_focus,
                                 GUI_LCD_DEV_INFO   lcd_dev_info
                                 );

/*****************************************************************************/
//  Description : display menu scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollGroove(
                               BOOLEAN          is_update,
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollBarRect(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ReDrawSecondMenu(
                            CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                            BOOLEAN           is_draw_focus,
                            BOOLEAN           is_draw_sk,
                            BOOLEAN           is_draw_groove
                            );

/*****************************************************************************/
//  Description : update menu display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdateSecMenuDisplay(
                                BOOLEAN            is_support_3d,
                                BOOLEAN            is_move_item,
                                BOOLEAN            is_handle_tp,
                                uint16             prev_first_item_index,
                                GUI_RECT_T         *pre_first_item_rect_ptr,
                                uint16             prev_cur_item_index,
                                GUI_RECT_T         *pre_cur_item_rect_ptr,
                                uint16             line_num,
                                CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : calculator line index in one page
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint16 CalcLineIndexPage(
                               uint16   item_index,
                               uint16   first_item_index,
                               uint16   item_total_num
                               );

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
/*****************************************************************************/
//  Description : save previous item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SavePrevItem(
                        uint16          prev_item_index,
                        uint16          cur_item_index,
                        GUI_RECT_T      item_rect,
                        MMI_HANDLE_T    win_handle
                        );

/*****************************************************************************/
//  Description : save previous item bg for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SavePrevItemBg(
                          GUI_RECT_T    item_rect
                          );

/*****************************************************************************/
//  Description : save next item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SaveNextItem(
                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                        );
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResumeSecondMenuEx(
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                 int16          move_space
                                 );

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResumeSecondMenu(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                               int16            move_space
                               );

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveSecondMenu(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr,
                             GUI_POINT_T    *tp_point_ptr
                             );

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 拷贝不需要更新的buffer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DoDoubleMemWriteBack(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                int32           offset_y
                                );
#endif
#endif

/*****************************************************************************/
//  Description : 根据一个item的索引号和区域，获得offset Y
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetSecMenuOffsetY(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr, 
                              uint16            item_index, 
                              int16             item_top
                              );

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_InitSec(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (PNULL != menu_ctrl_ptr)
    {
        menu_ctrl_ptr->is_move_state = FALSE;

#ifdef UI_MULTILAYER_SUPPORT
        // 创建层之前，保证层不存在
        ReleaseSecondMenuPageLayer(menu_ctrl_ptr);

        // 创建层
        CreateSecondMenuPageLayer(menu_ctrl_ptr);
#endif

        // 设置起始位置
        menu_ctrl_ptr->offset_y = base_ctrl_ptr->rect.top;

#ifdef GUISECMENU_PDASTYLE_SUPPORT
        CTRLMENU_SetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_DRAW_SPLIT_LINE, TRUE);
#endif
    }
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 调用该函数之前，先调用ReleaseSecondMenuPageLayer
/*****************************************************************************/
LOCAL void CreateSecondMenuPageLayer(
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr  //in
                                     )
{   
    GUI_RECT_T          page_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    UILAYER_CREATE_T    create_info = {0};

    //get selected bar display rect
    page_rect = base_ctrl_ptr->rect;
    
    menu_ctrl_ptr->item_layer.block_id = UILAYER_NULL_HANDLE;
    
    if (menu_ctrl_ptr->common_theme.is_used_slide)
    {
        create_info.lcd_id       = menu_ctrl_ptr->common_theme.lcd_dev.lcd_id;
        create_info.owner_handle = base_ctrl_ptr->handle;
        create_info.offset_x     = page_rect.left;
        create_info.offset_y     = page_rect.top;
        create_info.width        = (int16)(page_rect.right + 1 - page_rect.left);
        create_info.height       = (int16)(page_rect.bottom + 1 - page_rect.top);
        create_info.format       = UILAYER_MEM_DOUBLE_COPY;
        
        // 创建层
        UILAYER_CreateLayer(&create_info,&menu_ctrl_ptr->item_layer);

        // 设置color key
        UILAYER_SetLayerColorKey(&menu_ctrl_ptr->item_layer, TRUE, UILAYER_TRANSPARENT_COLOR);
        
        // 使用color清除层
        UILAYER_SetLayerPosition(&menu_ctrl_ptr->item_layer, page_rect.left, page_rect.top);
    }
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseSecondMenuPageLayer(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                      )
{
    // 释放层
    if ((UILAYER_IsMultiLayerEnable()) && 
        (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&menu_ctrl_ptr->item_layer)) && 
        (menu_ctrl_ptr->common_theme.is_used_slide))
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->item_layer);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : 合并层，由于最多只有两个Page合并到主层上，因此只需要两个层的参数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetBltSecondMenuPageLayer(
                                     UILAYER_HANDLE_T   layer1,
                                     UILAYER_HANDLE_T   layer2
                                     )
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
   
    lcd_dev_info.block_id = MMITHEME_GetBgLayer();
    UILAYER_RemoveBltLayer(&lcd_dev_info);

    lcd_dev_info.block_id = layer1;
    if (0 != layer1 && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 

        UILAYER_SetLayerColorKey(&lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&lcd_dev_info);
    }

    lcd_dev_info.block_id = layer2;
    if (0 != layer2 && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 

        UILAYER_SetLayerColorKey(&lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&lcd_dev_info);
    }

    return;
}

/*****************************************************************************/
//  Description : 合并层，由于最多只有两个Page合并到主层上，因此只需要两个层的参数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetBltSecondMenuNotClear(
                                    UILAYER_HANDLE_T    layer1,
                                    UILAYER_HANDLE_T    layer2
                                    )
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    
    lcd_dev_info.block_id = layer1;

    if (0 != layer1 && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }

    if (0 != layer2 && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}
#endif

/*****************************************************************************/
//  Description : second menu handle special msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_SecSpecial(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr, 
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    BOOLEAN         *is_gray_ptr = (BOOLEAN *)param;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    if (PNULL != menu_ctrl_ptr)
    {
	    if ((PNULL != is_gray_ptr) && 
            (*is_gray_ptr)) // 这一步主要处理灰化菜单的高亮问题
        {
            if ((!menu_ctrl_ptr->common_theme.is_hl_disappear) || 
                (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                DisplayHighlightItem(menu_ctrl_ptr);
            }
		    else
		    {
                GUI_RECT_T          item_rect = {0};
			    GUI_LCD_DEV_INFO    lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
			    
			    ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
			    item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
			    DisplayOneItem(menu_ctrl_ptr, item_rect, menu_ctrl_ptr->cur_item_index, FALSE, lcd_dev_info);
		    }
        }
        

        switch (msg_id)
        {
        case MSG_CTL_LOSE_FOCUS:
#ifdef UI_MULTILAYER_SUPPORT
            //若前一帧为direct draw，需要重画
            if ( UILAYER_IsPerDirectDraw() )
            {
                CTRLMENU_DrawSec(menu_ctrl_ptr);
            }
#endif
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display selected bar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightItem(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                )
{
    BOOLEAN             is_true = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    
    ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
    
    lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
    
    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
    {
        is_true = TRUE;
        DisplayHighlightItemBg(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, lcd_dev_info);
    }

    DisplaySecMenuItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, is_true, lcd_dev_info);
}

/*****************************************************************************/
//  Description : Clear Icon Rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ClearSecMenuItemRect(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                uint16          item_index
                                )
{
    GUI_RECT_T          item_rect  = {0};
    GUI_RECT_T          clip_rect  = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (PNULL != menu_ctrl_ptr)
    {
        //get item index and rect
        lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);

        item_rect = GetSecMenuItemRect(menu_ctrl_ptr, item_index);

        if (GUI_IntersectRect(&clip_rect, item_rect, base_ctrl_ptr->rect))
        {
            //display bg
    #ifdef UI_MULTILAYER_SUPPORT
            if (UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer))
            {
                // clear lcd rect
                MMITHEME_ClearRect(&lcd_dev_info, &clip_rect);
            }
            else
    #endif
            {
                DisplayMenuBg(clip_rect,menu_ctrl_ptr,PNULL);
            }
        }
    }
}

/*****************************************************************************/
//  Description : GetSecMenuItemDispLcdDevInfo
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetSecMenuItemDispLcdDevInfo(
                                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                                    )
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (PNULL != menu_ctrl_ptr)
    {
#ifdef UI_MULTILAYER_SUPPORT    
        if ((UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer)) && 
            (menu_ctrl_ptr->common_theme.is_used_slide))
        {
            lcd_dev_info.block_id = menu_ctrl_ptr->item_layer.block_id;
        }
        else
#endif
        {
            lcd_dev_info = menu_ctrl_ptr->common_theme.lcd_dev;
        }
    }

    return (lcd_dev_info);
}

/*****************************************************************************/
//  Description : GetSecMenuItemRect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemRect(
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                    uint16          item_index
                                    )
{
    GUI_RECT_T      item_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    // 参数检查
    if ((PNULL != menu_ctrl_ptr) &&
        (item_index < menu_ctrl_ptr->item_total_num) &&
        (PNULL != menu_ctrl_ptr->item_pos_info_ptr))
    {
        item_rect = menu_ctrl_ptr->item_pos_info_ptr[item_index].item_rect;

        if (IsNeedScrollBar(menu_ctrl_ptr))
        {
            item_rect.right = (int16)(item_rect.right- GetScrollBarWidth(menu_ctrl_ptr));
        }

        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &item_rect);
    }

    return (item_rect);
}

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedScrollBar(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr
                              )
{
    BOOLEAN     result = FALSE;
    uint16      line_num_page = 0;

    //get line number in one page
    line_num_page = GetLineNumPage(menu_ctrl_ptr);
    if (line_num_page < menu_ctrl_ptr->item_total_num)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLineNumPage(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            )
{
    uint16          line_num_page = 0;
    uint16          item_height = 0;
    GUI_RECT_T      menu_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (PNULL != menu_ctrl_ptr)
    {
        menu_rect = base_ctrl_ptr->rect;

        item_height = menu_ctrl_ptr->sec_theme.item_height;

        line_num_page = (uint16)((menu_rect.bottom - menu_rect.top + 1 - (menu_ctrl_ptr->sec_theme.expand_item_height - item_height))/item_height);     /*lint !e778*/
    }

    return (line_num_page);
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetScrollBarWidth(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               )
{
    uint16      width = 0;

    width = (uint16)(MMITHEME_GetScrollBarWidthByState(menu_ctrl_ptr->common_theme.is_used_slide) + CTRLMENU_ITEM_SCREENBAR_GAP);

    return (width);
}

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMenuBg(
                         GUI_RECT_T         bg_rect,
                         CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                         GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
                         )
{
    GUI_BG_T            common_bg = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    MMI_IMAGE_ID_T      bg_img_id = menu_ctrl_ptr->common_theme.bg_image;
    GUI_LCD_DEV_INFO    *dst_lcd_dev_info_ptr = lcd_dev_info_ptr;

    if (PNULL == dst_lcd_dev_info_ptr)
    {
        dst_lcd_dev_info_ptr = &base_ctrl_ptr->lcd_dev_info;
    }

    if (GUIMENU_STYLE_SECOND == menu_ctrl_ptr->cur_style)
    {
#if 0
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
        common_bg = MMITHEME_GetCommonLightBg();
#else
        common_bg = MMITHEME_GetCommonBg();
#endif
#endif
        common_bg = MMITHEME_GetSecMenuBg(); //for watch,second menu background type is color and color is black
        bg_img_id = common_bg.img_id;
    }

    if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
    {
        GUI_FillRect((const GUI_LCD_DEV_INFO*)dst_lcd_dev_info_ptr,
            bg_rect,
            menu_ctrl_ptr->pop_theme.bg_color);
    }
    else
    {
#ifdef BG_ON_BOTTOM_SUPPORT        
#ifdef UI_MULTILAYER_SUPPORT
        if (UILAYER_IsMultiLayerEnable())
        {
            if (MMITHEME_GetBgImgId() == bg_img_id)
            {
                //通常背景
                MMITHEME_DisplayBg(&bg_rect, menu_ctrl_ptr->win_handle, dst_lcd_dev_info_ptr);
            }
            else if (0 != bg_img_id)
            {
                GUIRES_DisplayImg(PNULL,
                    &bg_rect,
                    &bg_rect,
                    menu_ctrl_ptr->win_handle,
                    bg_img_id,
                    (const GUI_LCD_DEV_INFO*)dst_lcd_dev_info_ptr);
            }
            else
            {
                GUI_FillRect(dst_lcd_dev_info_ptr,
                    bg_rect,
                    menu_ctrl_ptr->common_theme.bg_color);
            }            
        }
        else
#endif
#endif
        {
            if (0 != bg_img_id)
            {
                GUIRES_DisplayImg(PNULL,
                    &bg_rect,
                    &bg_rect,
                    menu_ctrl_ptr->win_handle,
                    bg_img_id,
                    (const GUI_LCD_DEV_INFO*)dst_lcd_dev_info_ptr);
            }
            else
            {
                GUI_FillRect(dst_lcd_dev_info_ptr,
                    bg_rect,
                    menu_ctrl_ptr->common_theme.bg_color);
            }
        }
    }
}

/*****************************************************************************/
//  Description : display selected bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightItemBg(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr, //in
                                  uint16            item_index,     //in
                                  GUI_LCD_DEV_INFO  lcd_dev_info
                                  )
{
    uint16          bar_width = 0;
    GUI_RECT_T      select_rect = {0};
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      icon_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    select_rect = menu_ctrl_ptr->item_pos_info_ptr[item_index].item_rect;

    //get image display rect
    icon_rect        = select_rect;
    icon_rect.left   = 0;
    icon_rect.top    = 0;
    icon_rect.bottom = (int16)(select_rect.bottom - select_rect.top);
    
    if (GUI_IntersectRect(&item_rect, select_rect, base_ctrl_ptr->rect))
    {
        if (IsNeedScrollBar(menu_ctrl_ptr))
        {
            bar_width = GetScrollBarWidth(menu_ctrl_ptr);

            icon_rect.right = (int16)(icon_rect.right - bar_width);
            item_rect.right = (int16)(item_rect.right - bar_width);
        }
        
        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &item_rect);

#ifdef  MONOCHROME_LCD_SUPPORT  
		//begin 221441
		item_rect.bottom--;
		//end 221441
        LCD_FillRect(&lcd_dev_info, item_rect, MMI_BLACK_COLOR);
        return;
#endif

#ifdef UI_MULTILAYER_SUPPORT
        if (UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer))
        {
            //display selected bar
            GUIRES_DisplayImg(PNULL,
                &item_rect,
                &icon_rect,
                menu_ctrl_ptr->win_handle,
                menu_ctrl_ptr->common_theme.selected_img,
                (const GUI_LCD_DEV_INFO*)&lcd_dev_info);
        }
        else
#endif
        {
            if (menu_ctrl_ptr->is_transparent)
            {
                DisplayMenuBg(item_rect,menu_ctrl_ptr,PNULL);
            }

            //display selected bar
            GUIRES_DisplayImg(PNULL,
                &item_rect,
                &icon_rect,
                menu_ctrl_ptr->win_handle,
                menu_ctrl_ptr->common_theme.selected_img,
                (const GUI_LCD_DEV_INFO*)&lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : display one item include icon, text, no select bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplaySecMenuItem(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              uint16            item_index,
                              BOOLEAN           is_draw_focus,
                              GUI_LCD_DEV_INFO  lcd_dev_info
                              )
{
    uint16          icon_width = 0;
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    
    item_rect = menu_ctrl_ptr->item_pos_info_ptr[item_index].item_rect;

    if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect))
    {
        //display item icon
        icon_width = DisplaySecMenuItemIcon(menu_ctrl_ptr,item_index,item_rect, inter_rect, lcd_dev_info);
        
        //display item text
        DisplaySecMenuItemText(menu_ctrl_ptr, item_index, item_rect, inter_rect, icon_width, is_draw_focus, lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : display item icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 DisplaySecMenuItemIcon(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    uint16              item_index,
                                    GUI_RECT_T          disp_rect,
                                    GUI_RECT_T          clip_rect,
                                    GUI_LCD_DEV_INFO    lcd_dev_info
                                    )
{
    GUI_RECT_T      			icon_rect = {0};
    GUI_RECT_T      			icon_disp_rect = {0};
    GUI_RECT_T      			inter_rect = {0};
    MMI_IMAGE_ID_T  			icon_id = 0;
    uint16          			icon_width = 0;
    uint16          			icon_height = 0;
	CTRLMENU_NODE_T 			*item_node_ptr = PNULL;
	CTRLMENU_SELECT_ICON_DATA_T item_icon_data = {0};

    if (menu_ctrl_ptr->is_static)
    {	
		//get icon image id
		icon_id = GetItemIconId(item_index,menu_ctrl_ptr);
    } 
    else
    {
		//get current item node pointer
		item_node_ptr = CTRLMENU_GetNodeByVisibleIndex(item_index, menu_ctrl_ptr->cur_parent_node_ptr);
		if (DYNA_ITEM_ICON_ID == item_node_ptr->select_icon_type)
		{
			//get icon image id
			icon_id = GetItemIconId(item_index,menu_ctrl_ptr);
		} 
		else
		{
			// get the buffer of image in dynamic menu item 
			item_icon_data = GetItemIconBuffer(item_index, menu_ctrl_ptr);
		}
    }

	if (0 != icon_id)
	{    
		//get icon display rect
		icon_disp_rect = GetSecMenuItemIconRect(menu_ctrl_ptr, disp_rect, icon_id, &icon_width, &icon_height);

		//set display point
		if (GUI_IntersectRect(&inter_rect, icon_disp_rect, clip_rect))
		{
			icon_rect.right = (int16)(icon_width - 1);

			if (icon_disp_rect.top < inter_rect.top)
			{
				icon_rect.top    = (int16)(inter_rect.top - icon_disp_rect.top);
				icon_rect.bottom = (int16)(icon_height - 1);
			}
			else if (icon_disp_rect.bottom > inter_rect.bottom)
			{
				icon_rect.top    = 0;
				icon_rect.bottom = (int16)(icon_height - (icon_disp_rect.bottom - inter_rect.bottom));
			}
			else
			{
				icon_rect.top    = 0;
				icon_rect.bottom = (int16)(icon_height - 1);
			}
        
			//display icon
			GUIRES_DisplayImg(PNULL,
				&inter_rect,
				&icon_rect,
				menu_ctrl_ptr->win_handle,
				icon_id,
				(const GUI_LCD_DEV_INFO*)&lcd_dev_info);
		}
	}
	else if (PNULL != item_node_ptr && DYNA_ITEM_ICON_BUFFER == item_node_ptr->select_icon_type)
	{
		//get icon display rect
		icon_disp_rect = GetSecMenuItemBufferRect(menu_ctrl_ptr, disp_rect, item_index);
		// 区域有交集的时候
		if (GUI_IntersectRect(&inter_rect, icon_disp_rect, clip_rect))
		{
			GUI_POINT_T					img_point = {0};
			GUIIMG_BITMAP_T				img_map_info = {0};
			GUIIMG_DISPLAYBMP_PARAM_T	display_param = {0};
			GUI_LCD_DEV_INFO			*lcd_dev_info_ptr = PNULL;
			
			lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);
			icon_width  = (uint16)(icon_disp_rect.right - icon_disp_rect.left + 1);
			icon_height = (uint16)(icon_disp_rect.bottom - icon_disp_rect.top + 1);
			
			img_point.x = inter_rect.left;
			img_point.y = inter_rect.top;
			
			img_map_info.bit_ptr    = item_icon_data.buffer.data;
			img_map_info.img_width  = item_icon_data.buffer.width;
			img_map_info.img_height = item_icon_data.buffer.height;
			img_map_info.date_type  = GUIIMG_DATE_TYPE_ARGB888;
			
			display_param.is_horizontal = FALSE;
			display_param.rect_ptr = &inter_rect;
			display_param.point_ptr = &img_point;
			display_param.bitmap_ptr = &img_map_info;
			display_param.lcd_dev_ptr = lcd_dev_info_ptr;
			display_param.is_transparent_mode = TRUE;
			display_param.transparent_color = 0x00;
			GUIIMG_DisplayBmpEx(&display_param);
		}
	}

    return icon_width;
}

/*****************************************************************************/
//  Description : get icon image id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetItemIconId(
                                   uint16           item_index,
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    MMI_IMAGE_ID_T          icon_id = 0;
    GUIMENU_POP_SEL_INFO_T  item_info = {0};

    if (PNULL != menu_ctrl_ptr)
    {
        //set item info
        CTRLMENU_GetPopSelInfo(menu_ctrl_ptr, item_index, &item_info);

        if (GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style)
        {
            if (CTRLMENU_GetPopItemStatus(&item_info))
            {
                icon_id = menu_ctrl_ptr->pop_theme.radio_sel_img;
            }
            else
            {
                icon_id = menu_ctrl_ptr->pop_theme.radio_unsel_img;
            }
        }
        else if (GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style)
        {
            if (CTRLMENU_GetPopItemStatus(&item_info))
            {
                icon_id = menu_ctrl_ptr->pop_theme.check_sel_img;
            }
            else
            {
                icon_id = menu_ctrl_ptr->pop_theme.check_unsel_img;
            }
        }
        else
        {
            CTRLMENU_GetItem(menu_ctrl_ptr, item_index, PNULL, &icon_id, PNULL);
        }

        if (0 == icon_id && menu_ctrl_ptr->sec_theme.is_draw_default_icon)
        {
            icon_id = MMITHEME_GetSecItemIcon(item_index);
        }
    }

    return (icon_id);
}

/*****************************************************************************/
//  Description : get icon image buffer
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:该函数仅适用于动态menu，并且menu的图片icon类型为buffer的情况
/*****************************************************************************/
LOCAL CTRLMENU_SELECT_ICON_DATA_T GetItemIconBuffer(
                                   uint16							item_index,
                                   CTRLMENU_OBJ_T					*menu_ctrl_ptr
                                   )
{
	CTRLMENU_SELECT_ICON_DATA_T item_icon_data = {0};

    if (PNULL != menu_ctrl_ptr && !menu_ctrl_ptr->is_static)
    {
		CTRLMENU_NODE_T* node_item_ptr = PNULL;
		node_item_ptr = CTRLMENU_GetNodeByVisibleIndex(item_index, menu_ctrl_ptr->cur_parent_node_ptr);
		if ( DYNA_ITEM_ICON_BUFFER == node_item_ptr->select_icon_type)
		{		
			item_icon_data.buffer.data = node_item_ptr->select_icon_data.buffer.data;
			item_icon_data.buffer.width = node_item_ptr->select_icon_data.buffer.width;
			item_icon_data.buffer.height = node_item_ptr->select_icon_data.buffer.height;
		}
    }	

    return item_icon_data;
}

/*****************************************************************************/
//  Description : get icon display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemIconRect(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                        GUI_RECT_T      item_rect,
                                        MMI_IMAGE_ID_T  icon_id,
                                        uint16          *width_ptr,
                                        uint16          *height_ptr
                                        )
{
    GUI_RECT_T      icon_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (PNULL != menu_ctrl_ptr)
    {
        //get icon width
        GUIRES_GetImgWidthHeight(width_ptr,
            height_ptr,
            icon_id,
            menu_ctrl_ptr->win_handle
            );

        //set icon rect
        icon_rect.left   = (int16)(item_rect.left + menu_ctrl_ptr->sec_theme.gap_width);
        icon_rect.right  = (int16)(icon_rect.left + *width_ptr - 1);
        icon_rect.top    = (int16)((item_rect.top + item_rect.bottom + 1 - *height_ptr)/2);
	    icon_rect.bottom = (int16)(icon_rect.top + *height_ptr - 1);

        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &icon_rect);
    }

    return (icon_rect);
}

/*****************************************************************************/
//  Description : get icon buffer display rect
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemBufferRect(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                        GUI_RECT_T      item_rect,
                                        uint16          item_index
                                        )
{
    GUI_RECT_T      icon_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
	
    if (PNULL != menu_ctrl_ptr && !menu_ctrl_ptr->is_static)
    {
		//get current item node pointer
		uint16  icon_width = 0;
		uint16  icon_height = 0;
		CTRLMENU_NODE_T *item_node_ptr = PNULL;
		item_node_ptr =	CTRLMENU_GetNodeByVisibleIndex(item_index, menu_ctrl_ptr->cur_parent_node_ptr);
		//get icon width & height
		icon_width = item_node_ptr->select_icon_data.buffer.width;
		icon_height = item_node_ptr->select_icon_data.buffer.height;
        //set icon rect
        icon_rect.left   = (int16)(item_rect.left + menu_ctrl_ptr->sec_theme.gap_width);
        icon_rect.right  = (int16)(icon_rect.left + icon_width - 1);
        icon_rect.top    = (int16)((item_rect.top + item_rect.bottom + 1 - icon_height)/2);
		icon_rect.bottom = (int16)(icon_rect.top + icon_height - 1);
		
        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &icon_rect);
    }
	
    return (icon_rect);
}

/*****************************************************************************/
//  Description : display item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySecMenuItemText(
                                  CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                                  uint16           item_index,
                                  GUI_RECT_T       item_rect,
                                  GUI_RECT_T       clip_rect,
                                  uint16           icon_width,
                                  BOOLEAN          is_draw_focus_text,
                                  GUI_LCD_DEV_INFO lcd_dev_info
                                  )
{
    BOOLEAN         is_display = FALSE;
    BOOLEAN         is_scroll_text = FALSE;
    BOOLEAN         is_grayed = FALSE;
    uint16          text_pixel_len = 0;
    uint16          icon_gap_width = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_ALIGN_E     text_align = ALIGN_LVMIDDLE;
    GUI_COLOR_T     text_color = 0;
    MMI_STRING_T    text_str = {0};
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    // 如果有图标，加上图标的间隙
    if (0 != icon_width)
    {
        icon_gap_width = (uint16)(icon_width + menu_ctrl_ptr->sec_theme.gap_width);
    }

    //get text display rect
    text_rect = GetSecMenuItemTextRect(menu_ctrl_ptr, item_rect, icon_gap_width);
    
    //get text and color
    CTRLMENU_GetItem(menu_ctrl_ptr, item_index, &text_str, PNULL, &is_grayed);

    if (is_grayed)
    {
        text_color = MMITHEME_GetGrayColor();
    }
    else
    {
#ifdef  MONOCHROME_LCD_SUPPORT  //zss modify begin				
        if (item_index == menu_ctrl_ptr->cur_item_index)
        {
        	text_color = 0xffff;
        }
        else
        {
            text_color = 0;
        }
#else
         text_color = CTRLMENU_GetItemFontColor(menu_ctrl_ptr,item_index);
#endif //zss modify end 
    }

    //display text
    if (item_index == menu_ctrl_ptr->cur_item_index
        && (!menu_ctrl_ptr->common_theme.is_hl_disappear || is_draw_focus_text))
    {
        //get item text len
        text_pixel_len = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
                            text_str.wstr_len,
                            menu_ctrl_ptr->sec_theme.cur_item_font,
                            0);

        //is scroll item text
        if (text_pixel_len > (text_rect.right - text_rect.left + 1))
        {
            is_scroll_text = TRUE;
        }

        if (is_scroll_text)
        {
            //start text scroll timer
            if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
            {
               CTRLMENU_StartItemTextTimer(menu_ctrl_ptr);
            }
            else
            {
                // 拖动时，从头开始显示
                menu_ctrl_ptr->item_str_index = 0;
            }

            if (MMITHEME_GetIsScrollAsPixel())
            {
                str_style.align = text_align;
                str_style.font = menu_ctrl_ptr->sec_theme.cur_item_font;
                str_style.font_color = text_color;
                
                GUISTR_DrawTextToLCDByOffset( 
                    (const GUI_LCD_DEV_INFO*)&lcd_dev_info,
                    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
                    (const GUI_RECT_T      *)&clip_rect,
                    menu_ctrl_ptr->item_str_index,
                    0,
                    (const MMI_STRING_T    *)&text_str,
                    &str_style,
                    str_state
                    );

                is_display = TRUE;
            }
            else
            {
                text_str.wstr_ptr = text_str.wstr_ptr + menu_ctrl_ptr->item_str_index;
                text_str.wstr_len = (uint16)(text_str.wstr_len - menu_ctrl_ptr->item_str_index);
            }
        }
    }
    else
    {
        str_state |= GUISTR_STATE_ELLIPSIS;
    }
    
    if (!is_display)
    {
        str_style.align = text_align;

        if (menu_ctrl_ptr->common_theme.is_hl_disappear)
        {
            str_style.font = menu_ctrl_ptr->sec_theme.item_font;
        }
        else
        {
            str_style.font = (item_index == menu_ctrl_ptr->cur_item_index) 
                ? menu_ctrl_ptr->sec_theme.cur_item_font : menu_ctrl_ptr->sec_theme.item_font;
        }
        
        str_style.font_color = text_color;
        //display text
        GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info,
            (const GUI_RECT_T*)&text_rect, 
            (const GUI_RECT_T*)&clip_rect, 
            (const MMI_STRING_T*)&text_str, 
            &str_style, 
            str_state,
            GUISTR_TEXT_DIR_AUTO);
    }
}

/*****************************************************************************/
//  Description : get item text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSecMenuItemTextRect(
                                        CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                        GUI_RECT_T     item_rect,
                                        int16          icon_gap_width
                                        )
{
    uint16          prg_width = 0;
    GUI_RECT_T      text_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return text_rect;
    }

    //get progress width
    if (IsNeedScrollBar(menu_ctrl_ptr))
    {
        prg_width = GetScrollBarWidth(menu_ctrl_ptr);
    }

    //set icon rect
    text_rect.left   = (int16)(item_rect.left + menu_ctrl_ptr->sec_theme.gap_width + icon_gap_width);
    text_rect.right  = (int16)(item_rect.right - prg_width);
    text_rect.top    = item_rect.top;
    text_rect.bottom = item_rect.bottom;
    
    GUI_CheckRectDirection(&base_ctrl_ptr->rect, &text_rect);

    return text_rect;
}

/*****************************************************************************/
//  Description : display one item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayOneItem(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                          GUI_RECT_T        item_rect,
                          uint16            index,
                          BOOLEAN           is_draw_focus,
                          GUI_LCD_DEV_INFO  lcd_dev_info
                          )
{
    BOOLEAN is_true = FALSE;
    
    if (CTRLMENU_GetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_DRAW_SPLIT_LINE))
    {
        DrawSecMenuItemSplitLine(menu_ctrl_ptr, index, item_rect, &lcd_dev_info);
    }
    
    if (is_draw_focus 
        && index == menu_ctrl_ptr->cur_item_index 
        && MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
    {
        is_true = TRUE;
        DisplayHighlightItemBg(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, lcd_dev_info);
    }
    else
    {
        is_true = FALSE;
    }
    
    DisplaySecMenuItem(menu_ctrl_ptr, index, is_true, lcd_dev_info);
}

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawSecMenuItemSplitLine(
                                    CTRLMENU_OBJ_T          *menu_ctrl_ptr,
                                    uint16                  item_index,
                                    GUI_RECT_T              item_rect,
                                    const GUI_LCD_DEV_INFO  *lcd_dev_info_ptr
                                    )
{
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (PNULL != menu_ctrl_ptr && 0 < menu_ctrl_ptr->item_total_num && !GUI_IsInvalidRect(item_rect))
    {
        if (0 == item_index) // 上线
        {
            line_rect = item_rect;
            line_rect.bottom = item_rect.top;

            if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->rect,line_rect))
            {
                //draw line
                GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
                    cross_rect,
                    menu_ctrl_ptr->common_theme.split_line_color);
            }
        }
        
        // 下线
        line_rect = item_rect;
        line_rect.top = item_rect.bottom;
        
        if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->rect,line_rect))
        {
            //draw line
            GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
                cross_rect,
                menu_ctrl_ptr->common_theme.split_line_color);
        }
    }
}

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DrawSec(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{
    BOOLEAN             is_draw_focus = FALSE;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + ResetSecMenuOffsetY(menu_ctrl_ptr));

    CreateSecSlideInfo(menu_ctrl_ptr);

    CalculateSecItemRect(menu_ctrl_ptr);

    lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);

    // 如果item layer有效，则需要进行层的合并
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable() 
        && UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer)
        && menu_ctrl_ptr->common_theme.is_used_slide)
    {
        SetBltSecondMenuPageLayer(lcd_dev_info.block_id, UILAYER_NULL_HANDLE);
    }
#endif

    //display bg
    DisplayMenuBg(base_ctrl_ptr->rect,menu_ctrl_ptr, PNULL);

    //set title param
    SetSecondTitle(menu_ctrl_ptr);

    // reset start index
    ResetSecMenuStartIndex(menu_ctrl_ptr);

    //display item
    if (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        is_draw_focus = TRUE;
    }
    else
    {
        is_draw_focus = FALSE;
    }
    DisplaySecMenuAllItem(menu_ctrl_ptr, is_draw_focus, lcd_dev_info);

    //display scroll groove
    DisplayScrollGroove(TRUE,menu_ctrl_ptr);

    //set softkey param
    CTRLMENU_SetSoftkey(menu_ctrl_ptr);        
}

/*****************************************************************************/
//  Description : reset Y offset
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int16 ResetSecMenuOffsetY(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                )
{
    uint16          item_num = 0;
    uint16          line_page_num = 0;
    uint16          item_height = 0;
    uint16          item_space = 0;
    int16           offset_y = 0;
    int16           max_offset_y = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    
    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return offset_y;
    }
    
    if (menu_ctrl_ptr->offset_y >= base_ctrl_ptr->rect.top)
    {
        offset_y = (int16)(base_ctrl_ptr->rect.top - menu_ctrl_ptr->offset_y);
    }
    else
    {
        item_num = menu_ctrl_ptr->item_total_num;

        item_height = menu_ctrl_ptr->sec_theme.item_height;
        item_space = menu_ctrl_ptr->sec_theme.item_space;

        line_page_num = GetLineNumPage(menu_ctrl_ptr);
        
        if (menu_ctrl_ptr->item_total_num > line_page_num) // 当超过一页的时候
        {
            // 计算所有item的高度
            max_offset_y = (int16)(item_num*(item_height + item_space) + (menu_ctrl_ptr->sec_theme.expand_item_height - item_height));
        
            // 计算最大偏移
            max_offset_y = (int16)(base_ctrl_ptr->rect.top - max_offset_y);

            // 判断偏移量是否小于一页的位置
            if (menu_ctrl_ptr->offset_y < max_offset_y + (base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1))
            {
                offset_y = (int16)(max_offset_y + (base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1) - menu_ctrl_ptr->offset_y);
            }
        }
        else // 条目尚未超过一页
        {
            offset_y = (int16)(base_ctrl_ptr->rect.top - menu_ctrl_ptr->offset_y);
        }
    }
    
    return offset_y;
}

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateSecSlideInfo(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                              )
{
    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (PNULL != menu_ctrl_ptr->item_pos_info_ptr)
    {
        SCI_FREE(menu_ctrl_ptr->item_pos_info_ptr);
    }
    // 申请内存
    if (PNULL == menu_ctrl_ptr->item_pos_info_ptr && 0 != menu_ctrl_ptr->item_total_num)
    {
        menu_ctrl_ptr->item_pos_info_ptr = SCI_ALLOC_APP(menu_ctrl_ptr->item_total_num*sizeof(CTRLMENU_ITEM_POS_INFO_T));
        SCI_MEMSET(menu_ctrl_ptr->item_pos_info_ptr, 0, menu_ctrl_ptr->item_total_num*sizeof(CTRLMENU_ITEM_POS_INFO_T));
    }
}

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateSecItemRect(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr  //in
                                   )
{
    uint16          i = 0;
    uint16          item_num = 0;
    uint16          item_height = 0;
    uint16          item_space = 0;
    int16           ofsset_y = 0;
    BOOLEAN         result = TRUE;
    GUI_RECT_T      item_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    
    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }
    
    if(PNULL == menu_ctrl_ptr->item_pos_info_ptr)
    {
        return FALSE;
    }

    item_num = menu_ctrl_ptr->item_total_num;

    item_height = menu_ctrl_ptr->sec_theme.item_height;
    item_space = menu_ctrl_ptr->sec_theme.item_space;

    ofsset_y = menu_ctrl_ptr->offset_y;

    item_rect.left  = base_ctrl_ptr->rect.left;
    item_rect.right = base_ctrl_ptr->rect.right;

    for (i = 0; i < item_num; i++)
    {
        menu_ctrl_ptr->item_pos_info_ptr[i].item_index = i;

        if (i > menu_ctrl_ptr->cur_item_index)
        {
#ifdef GUIF_LIST_PDASTYLE
            item_rect.top = (int16)(ofsset_y + i*(item_height + item_space));
#else
			item_rect.top = (int16)(ofsset_y + i*(item_height + item_space) + (menu_ctrl_ptr->sec_theme.expand_item_height - item_height));
#endif
        }
        else
        {
            item_rect.top = (int16)(ofsset_y + i*(item_height + item_space));
        }
 
        if (i == menu_ctrl_ptr->cur_item_index)
        {
#ifdef GUIF_LIST_PDASTYLE
            item_rect.bottom = (int16)(item_rect.top + (item_height + item_space) - 1);
#else
            item_rect.bottom = (int16)(item_rect.top + (item_height + item_space + (menu_ctrl_ptr->sec_theme.expand_item_height - item_height)) - 1);
#endif			
		}
        else
        {
            item_rect.bottom = (int16)(item_rect.top + (item_height + item_space) - 1);
        }

        menu_ctrl_ptr->item_pos_info_ptr[i].item_rect = item_rect;
    }

    return result;
}

/*****************************************************************************/
//  Description : set second menu title param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetSecondTitle(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          )
{
    GUI_COLOR_T     title_color = 0;
    MMI_STRING_T    title_str = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

	// 如果title由应用控制，这里直接返回
	if (CTRLMENU_GetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_CUSTOMER_TITLE))
	{
		return;
	}

    //set bg
    GUIWIN_SetTitleBackground(menu_ctrl_ptr->win_handle,menu_ctrl_ptr->sec_theme.title_icon_id,0);
    
    //get title text
    CTRLMENU_GetTitleText(menu_ctrl_ptr, &title_str, PNULL, PNULL);

    //set title font color
    title_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_TITLE);
    GUIWIN_SetTitleFontColor(menu_ctrl_ptr->win_handle,title_color);

    //set title text
	GUIWIN_SetTitleText(menu_ctrl_ptr->win_handle,
        title_str.wstr_ptr,
        title_str.wstr_len,
        TRUE);
}

/*****************************************************************************/
//  Description : reset
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetSecMenuStartIndex(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  )
{
    uint16          i = 0;
    uint16          item_num = 0;
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    
    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    item_num = menu_ctrl_ptr->item_total_num;
    
    for (i = 0; i < item_num; i++)
    {
        item_rect = menu_ctrl_ptr->item_pos_info_ptr[i].item_rect;
        
        if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect))
        {
            menu_ctrl_ptr->first_item_index = i;
            
            break;
        }
    }
}

/*****************************************************************************/
//  Description : display one item include icon,text and select bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySecMenuAllItem(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                 BOOLEAN            is_draw_focus,
                                 GUI_LCD_DEV_INFO   lcd_dev_info
                                 )
{
    uint16          i = 0;
    uint16          item_num = 0;
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    item_num = menu_ctrl_ptr->item_total_num;

    for (i = menu_ctrl_ptr->first_item_index; i < item_num; i++)
    {
        item_rect = menu_ctrl_ptr->item_pos_info_ptr[i].item_rect;

        if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect))
        {
            BOOLEAN     is_need_draw = TRUE;
            GUI_RECT_T  cur_rect = {0};

            //如果做了内存回写加速
            if(! GUI_IsZeroRect( menu_ctrl_ptr->writeback_rect ) )
            {
                //if the the item in the writeback rect 
                if( GUI_IntersectRect( &cur_rect, inter_rect, menu_ctrl_ptr->writeback_rect ) )
                {
                    //判断边界item是否是完整的，如果不是完整的需要将拷贝的区域重置
                    if( !GUI_RectIsCovered( menu_ctrl_ptr->writeback_rect, inter_rect ) )
                    {
#ifdef UI_MULTILAYER_SUPPORT
                        UILAYER_ClearRect( &lcd_dev_info,cur_rect );
#endif
                    }
                    else
                    {
                        is_need_draw = FALSE;
                    }
                }
            }

            if ( is_need_draw )
            {
                DisplayOneItem( menu_ctrl_ptr, item_rect, i, is_draw_focus, lcd_dev_info );
            }

        }
    }  
}

/*****************************************************************************/
//  Description : display menu scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollGroove(
                               BOOLEAN          is_update,
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               )
{
    uint16                  first_item_index = 0;
    uint32                  prg_total_count = 0;
    uint32                  prg_count_in_page = 0;
    uint32                  prg_cur_index = 0;
    uint32                  prg_first_index = 0;
    GUI_RECT_T              prg_rect = {0};
    GUI_RECT_T              item_rect = {0};
    GUI_RECT_T              inter_rect = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    MMI_CONTROL_CREATE_T    ctrl_creat = {0};
    GUIPRGBOX_INIT_DATA_T   init_data = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info = {0};

    //is need scroll bar
    if (IsNeedScrollBar(menu_ctrl_ptr))
    {
        //get progress bar rect
        prg_rect = GetScrollBarRect(menu_ctrl_ptr);

        //init
        if (PNULL == menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            //init data
            init_data.lcd_id = GUI_MAIN_LCD_ID;
            init_data.both_rect.h_rect = init_data.both_rect.v_rect = prg_rect;
            init_data.style = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
            
            ctrl_creat.guid               = SPRD_GUI_PRGBOX_ID;
            ctrl_creat.parent_win_handle  = menu_ctrl_ptr->win_handle;
			ctrl_creat.parent_ctrl_handle = base_ctrl_ptr->handle;
            ctrl_creat.init_data_ptr      = &init_data;
            
            //creat progress bar control
            menu_ctrl_ptr->prgbox_ctrl_ptr = MMK_CreateControl(&ctrl_creat);

            //set progress bar update
            is_update = TRUE;
        }

        if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
#if !defined(GUIF_PRGBOX_AUTOHIDE)
            lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
            GUIAPICTRL_SetLcdDevInfo(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr), &lcd_dev_info);
#endif
            
#ifdef UI_MULTILAYER_SUPPORT
//            if ( !UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer) )
#endif
            {
                IGUICTRL_GetLcdDevInfo(menu_ctrl_ptr->prgbox_ctrl_ptr, &lcd_dev_info);

                DisplayMenuBg(prg_rect,menu_ctrl_ptr, &lcd_dev_info);
            }
            
            GUIPRGBOX_ResizeRectByPtr(menu_ctrl_ptr->prgbox_ctrl_ptr, prg_rect);

            GUIPRGBOX_SetNeedBg(menu_ctrl_ptr->prgbox_ctrl_ptr, FALSE);

            if (CTRLMENU_GetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_PRG_PIXEL_SCROLL))
            {
                prg_total_count = (uint32)menu_ctrl_ptr->item_total_num * menu_ctrl_ptr->sec_theme.item_height;
                prg_count_in_page = prg_rect.bottom - prg_rect.top + 1;

                prg_cur_index = menu_ctrl_ptr->cur_item_index * menu_ctrl_ptr->sec_theme.item_height;
                if (menu_ctrl_ptr->offset_y > prg_rect.top)
                {
                    prg_first_index = 0;
                }
                else
                {
                    prg_first_index = prg_rect.top - menu_ctrl_ptr->offset_y;
                }
            }
            else
            {
                item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);

                if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect)
                    && !GUI_RectIsCovered(base_ctrl_ptr->rect, item_rect))
                {
                    if (base_ctrl_ptr->rect.bottom < item_rect.bottom)
                    {
                        first_item_index = menu_ctrl_ptr->first_item_index;
                    }
                    else
                    {
                        first_item_index = (uint16)(menu_ctrl_ptr->first_item_index + 1);
                        
                        if (first_item_index >= menu_ctrl_ptr->item_total_num)
                        {
                            first_item_index = (uint16)(menu_ctrl_ptr->item_total_num - 1);
                        }
                    }
                }
                else
                {
                    first_item_index = menu_ctrl_ptr->first_item_index;
                }

                prg_total_count = (uint32)menu_ctrl_ptr->item_total_num;
                prg_count_in_page = (uint32)GetLineNumPage(menu_ctrl_ptr);

                prg_first_index = (uint32)first_item_index;
                prg_cur_index = (uint32)menu_ctrl_ptr->cur_item_index;
            }

            //set progress param
            GUIPRGBOX_SetParamByPtr(
                menu_ctrl_ptr->prgbox_ctrl_ptr,
                prg_total_count,
                prg_count_in_page);

            //set current position
            GUIPRGBOX_SetPosByPtr(is_update,
                prg_cur_index,
    			prg_first_index,
                menu_ctrl_ptr->prgbox_ctrl_ptr);
        }
    }
    else
    {
        if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            MMK_DestroyControl(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
            menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollBarRect(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  )
{
    GUI_RECT_T      menu_rect = {0};
    GUI_RECT_T      prg_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (PNULL != menu_ctrl_ptr)
    {
        menu_rect = base_ctrl_ptr->rect;
        
        //set prg rect
        prg_rect      = menu_rect;
        prg_rect.left = (int16)(menu_rect.right - MMITHEME_GetScrollBarWidthByState(menu_ctrl_ptr->common_theme.is_used_slide));
        prg_rect.right = (int16)(prg_rect.left + MMITHEME_GetScrollBarWidthByState(menu_ctrl_ptr->common_theme.is_used_slide) - 1);

        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &prg_rect);
    }

    return (prg_rect);
}

/*****************************************************************************/
//  Description : handle second menu ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecHandleOk(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    MMI_MESSAGE_ID_E    msg_id
                                    )
{
    BOOLEAN     result = FALSE;
    
    switch(msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
        if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
        {
            result = TRUE;
            DisplayHighlightItem(menu_ctrl_ptr);
        }
        break;
        
    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 销毁菜单，释放内存
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_DestroySec(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                )
{
    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

#ifdef UI_MULTILAYER_SUPPORT
    // 释放层
    ReleaseSecondMenuPageLayer(menu_ctrl_ptr);
#endif

    SCI_FREE(menu_ctrl_ptr->item_pos_info_ptr);
    menu_ctrl_ptr->item_pos_info_ptr = PNULL;
}

/*****************************************************************************/
//  Description : adjust first item index and display before ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecAdjustFirstAndDisplay(
                                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                              BOOLEAN           is_need_update
                                              )
{
    BOOLEAN             is_draw_focus = FALSE;
    uint16              line_num_page = 0;
    uint16              last_item_index = 0;
    GUI_RECT_T          menu_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    //get line number in one page
    line_num_page = GetLineNumPage(menu_ctrl_ptr);

    //get current page last item index
    last_item_index = (uint16)(menu_ctrl_ptr->first_item_index + line_num_page - 1);

    //current item is not in current page
    if ((menu_ctrl_ptr->first_item_index > menu_ctrl_ptr->cur_item_index) ||
        (last_item_index < menu_ctrl_ptr->cur_item_index))
    {
        //stop item text timer
        CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

        //set first item index
        if (menu_ctrl_ptr->cur_item_index + 1 + line_num_page <= menu_ctrl_ptr->item_total_num)
        {
            menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->cur_item_index;
        }
        else
        {
            menu_ctrl_ptr->first_item_index = (uint16)(menu_ctrl_ptr->item_total_num - line_num_page);
        }

	    menu_ctrl_ptr->offset_y = GetSecMenuOffsetY(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index, base_ctrl_ptr->rect.top);

        if (is_need_update) 
        {
            menu_rect = base_ctrl_ptr->rect;

            //display bg
            DisplayMenuBg(menu_rect,menu_ctrl_ptr,PNULL);
            
            lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
            
#ifdef UI_MULTILAYER_SUPPORT
            if (UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer))
            {
                SetBltSecondMenuPageLayer(lcd_dev_info.block_id, UILAYER_NULL_HANDLE);
                
                UILAYER_BltLowerLayerWhenTransparent(&lcd_dev_info, &base_ctrl_ptr->rect);
            }
#endif
            
            // reset start index
            ResetSecMenuStartIndex(menu_ctrl_ptr);
            //display item

            is_draw_focus = (BOOLEAN)(menu_ctrl_ptr->common_theme.is_hl_disappear?FALSE:TRUE);
            DisplaySecMenuAllItem(menu_ctrl_ptr, is_draw_focus, lcd_dev_info);

            //display scroll groove
            DisplayScrollGroove(TRUE,menu_ctrl_ptr);

    		//set softkey param
    		CTRLMENU_SetSoftkey(menu_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : handle second menu up/down key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_SecHandleUpDown(
                                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                             MMI_MESSAGE_ID_E   msg_id
                                             )
{
    uint16      prev_item_index = 0;
    uint16      prev_first_item_index = 0;
    uint16      line_num = 0;
    int16       offset_y = 0;
    GUI_RECT_T  pre_first_item_rect = {0};
    GUI_RECT_T  pre_cur_item_rect = {0};
    BOOLEAN     is_move_item = FALSE;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
#ifdef MEDIA_DEMO_SUPPORT 
    if(!(((get_audio_state() == 6) ||(get_audio_state() == 7)) && ((get_video_state()==2) || (get_video_state()==3) )))
    {
        return result;
    }
    else if(get_audio_state() == 6 || get_video_state()==2)
    {
        if (MSG_APP_UP == msg_id)
        {
            MMK_PostMsg(menu_ctrl_ptr->win_handle,MSG_MEDIA_DEMO_APP_UP,0,0);
        }
        else if (MSG_APP_DOWN == msg_id)
        {
            MMK_PostMsg(menu_ctrl_ptr->win_handle,MSG_MEDIA_DEMO_APP_DOWN,0,0);
        }
    }
#endif
    
    if (CTRLMENU_SecResetDisplayItem(menu_ctrl_ptr, &offset_y, FALSE)) // 当offset y改变了，需要重新显示，然后直接返回
    {
        if (offset_y != 0)
        {
            menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);

            // 显示            
            ReDrawSecondMenu(menu_ctrl_ptr, (BOOLEAN)((MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())?TRUE:FALSE), TRUE, TRUE);
        }

        return result;
    }

    if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        DisplayHighlightItem(menu_ctrl_ptr);

        return result;
    }
    
    if (1 < menu_ctrl_ptr->item_total_num)
    {
        //set previous item index
        prev_item_index = menu_ctrl_ptr->cur_item_index;
        prev_first_item_index = menu_ctrl_ptr->first_item_index;
        pre_first_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
        pre_cur_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        //get line number
        line_num = menu_ctrl_ptr->item_total_num;

        //set first and current item index
        if (CTRLMENU_SetCurAndFirstItemIndex(menu_ctrl_ptr,line_num,msg_id))
        {
            //stop item text timer
            CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

            // 重新设置offset Y
            if (CTRLMENU_SecResetDisplayItem(menu_ctrl_ptr, &offset_y, FALSE))
            {
                menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);

                CalculateSecItemRect(menu_ctrl_ptr);

                ResetSecMenuStartIndex(menu_ctrl_ptr);

                is_move_item = TRUE;
            }

            //update menu display
            UpdateSecMenuDisplay(TRUE, 
                is_move_item, 
                FALSE,
                prev_first_item_index, 
                &pre_first_item_rect, 
                prev_item_index, 
                &pre_cur_item_rect, 
                line_num, 
                menu_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : reset list display item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecResetDisplayItem(
                                            CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                            int16             *offset_y_ptr,
                                            BOOLEAN           is_true
                                            )
{
    int16           offset_y = 0;
    BOOLEAN         reslut = TRUE;
    GUI_RECT_T      cur_item_rect = {0};
    GUI_RECT_T      inter_item_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != offset_y_ptr);/*assert verified*/
    if (
        (PNULL == menu_ctrl_ptr)||
        (PNULL == offset_y_ptr)
        )
    {
        return FALSE;
    }

    cur_item_rect = menu_ctrl_ptr->item_pos_info_ptr[menu_ctrl_ptr->cur_item_index].item_rect;

    if (GUI_IntersectRect(&inter_item_rect, cur_item_rect, base_ctrl_ptr->rect)) // 有交集
    {
        // 不被完全覆盖
        if (!GUI_RectIsCovered(base_ctrl_ptr->rect, cur_item_rect))
        {
            if (inter_item_rect.top > cur_item_rect.top) // item的上半部被覆盖
            {
                // 被覆盖后，不管down key 还是 up key都只回复显示cur，而不改变cur index
                offset_y = (int16)(inter_item_rect.top - cur_item_rect.top); // 需要向下移动的位移
            }
            else if (inter_item_rect.bottom < cur_item_rect.bottom) // item的下半部分被覆盖
            {
                offset_y = (int16)(inter_item_rect.bottom - cur_item_rect.bottom); // 需要向上移动的位移
            }
            else
            {
                reslut = FALSE;
            }
        }
        else
        {
            reslut = FALSE;
        }
    }
    else // 完全没有交集
    {
        if (cur_item_rect.top < base_ctrl_ptr->rect.top) // 在菜单区域之上
        {
            offset_y = (int16)(base_ctrl_ptr->rect.top - cur_item_rect.top); // 向下移动的位移
        }
        else if (cur_item_rect.bottom > base_ctrl_ptr->rect.bottom)
        {
            offset_y = (int16)(base_ctrl_ptr->rect.bottom - cur_item_rect.bottom);
        }
        else
        {
            reslut = FALSE;
        }
    }

    *offset_y_ptr = offset_y;

    if (is_true && 0 != offset_y)
    {
        menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);
        // 重新计算区域
        CalculateSecItemRect(menu_ctrl_ptr);
    }

    return reslut;
}

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ReDrawSecondMenu(
                            CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                            BOOLEAN           is_draw_focus,
                            BOOLEAN           is_draw_sk,
                            BOOLEAN           is_draw_groove
                            )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    // 变频
#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif    

    // 重新计算区域
    CalculateSecItemRect(menu_ctrl_ptr);

    // 重新设置start index
    ResetSecMenuStartIndex(menu_ctrl_ptr);

    lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
    if (menu_ctrl_ptr->common_theme.is_used_slide
        && UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer))
    {
        //若无回写才需要擦图层
        if( GUI_IsZeroRect( menu_ctrl_ptr->writeback_rect ) )
        {
            UILAYER_Clear(&lcd_dev_info);
        }
    }
    else
#endif
    {
        DisplayMenuBg(base_ctrl_ptr->rect,menu_ctrl_ptr, PNULL);
    }

    //display item
    DisplaySecMenuAllItem(menu_ctrl_ptr, is_draw_focus, lcd_dev_info);

    if (is_draw_groove)
    {
        DisplayScrollGroove(TRUE,menu_ctrl_ptr);
    }

    if (is_draw_sk)
    {
        CTRLMENU_SetSoftkey(menu_ctrl_ptr);
    }

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif
}

/*****************************************************************************/
//  Description : update menu display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdateSecMenuDisplay(
                                BOOLEAN            is_support_3d,
                                BOOLEAN            is_move_item,
                                BOOLEAN            is_handle_tp,
                                uint16             prev_first_item_index,
                                GUI_RECT_T         *pre_first_item_rect_ptr,
                                uint16             prev_cur_item_index,
                                GUI_RECT_T         *pre_cur_item_rect_ptr,
                                uint16             line_num,
                                CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                )
{
    BOOLEAN             is_draw_focus = FALSE;
    uint16              line_index = 0;
    GUI_RECT_T          bg_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if ((PNULL == pre_first_item_rect_ptr)||
        (PNULL == pre_cur_item_rect_ptr))
    {
        return;
    }

    lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
    
    is_draw_focus = is_handle_tp;// 按键时必须显示focus

    menu_ctrl_ptr->item_str_index = 0;
    //从头到末尾或从末尾到头
    if ((menu_ctrl_ptr->item_total_num > line_num)
        && ((0 == prev_cur_item_index) && ((menu_ctrl_ptr->item_total_num - 1) == menu_ctrl_ptr->cur_item_index)
        || (((menu_ctrl_ptr->item_total_num - 1) == prev_cur_item_index) && (0 == menu_ctrl_ptr->cur_item_index))))
    {
        ReDrawSecondMenu(menu_ctrl_ptr, is_draw_focus, TRUE, TRUE);
    }
    else
    {
        //get previous item line index
        line_index = CalcLineIndexPage(
            prev_cur_item_index, 
            prev_first_item_index, 
            menu_ctrl_ptr->item_total_num
            );

        //previous is in current page
        if (line_index < line_num)
        {
            //get previous item selected bar rect
            bg_rect = *pre_cur_item_rect_ptr;

            if (GUI_RectIsCovered(base_ctrl_ptr->rect, bg_rect))
            {
                if (is_support_3d)
                {
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
                    SavePrevItem(prev_cur_item_index,
                        menu_ctrl_ptr->cur_item_index,
                        bg_rect,
                        menu_ctrl_ptr->win_handle);
#endif
                }
            }

            //翻页
            if (is_move_item || prev_first_item_index != menu_ctrl_ptr->first_item_index)
            {
                ReDrawSecondMenu(menu_ctrl_ptr, is_draw_focus, TRUE, TRUE);
            }
            else
            {
                ReDrawSecondMenu(menu_ctrl_ptr, is_draw_focus, TRUE, TRUE); // 这里可以修改为只画两个item，暂时不改
            }

            if (GUI_RectIsCovered(base_ctrl_ptr->rect, bg_rect))
            {
                if (is_support_3d)
                {
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
                    //save previous item bg
                    SavePrevItemBg(bg_rect);
                    //save next item
                    SaveNextItem(menu_ctrl_ptr);
#endif
                }
            }
        }

        if (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
        {
            is_draw_focus = FALSE;

            ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

            if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state)
            {
                DisplayHighlightItemBg(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, lcd_dev_info);
                is_draw_focus = TRUE;
            }
            DisplaySecMenuItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, is_draw_focus, lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : calculator line index in one page
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint16 CalcLineIndexPage(
                               uint16   item_index,
                               uint16   first_item_index,
                               uint16   item_total_num
                               )
{
    uint16      line_index = 0;
    
    //set current item line index
    if (item_index >= first_item_index)
    {
        line_index = (uint16)(item_index - first_item_index);
    }
    else
    {
        line_index = (uint16)(item_total_num - first_item_index + item_index);
    }

    return (line_index);
}

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
/*****************************************************************************/
//  Description : save previous item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SavePrevItem(
                        uint16          prev_item_index,
                        uint16          cur_item_index,
                        GUI_RECT_T      item_rect,
                        MMI_HANDLE_T    win_handle
                        )
{
    uint32          buf_size = 0;

    if (MMITHEME_IsNeed3DList())
    {
        //set buffer size
        buf_size = (item_rect.right-item_rect.left + 1) * (item_rect.bottom-item_rect.top + 1) * (int16)sizeof(uint16);

        //alloc
        MMITHEME_Alloc3DListBuf(buf_size, win_handle);

        //save previous item
        MMITHEME_Save3DListBuf(GUILIST_PREV_BUF,item_rect);

        //set icon move direction
        MMITHEME_Set3DListDirection((BOOLEAN)(cur_item_index > prev_item_index));
    }
}

/*****************************************************************************/
//  Description : save previous item bg for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SavePrevItemBg(
                          GUI_RECT_T    item_rect
                          )
{
    if (MMITHEME_IsNeed3DList())
    {
        MMITHEME_Save3DListBuf(GUILIST_PREV_BG_BUF,item_rect);
    }
}

/*****************************************************************************/
//  Description : save next item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SaveNextItem(
                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                        )
{
    GUI_RECT_T      item_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (MMITHEME_IsNeed3DList())
    {
        lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
        
        ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        DisplayHighlightItemBg(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, lcd_dev_info);

        DisplaySecMenuItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, FALSE, lcd_dev_info);
        
        item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
        
        //save icon
        MMITHEME_Save3DListBuf(GUILIST_NEXT_BUF,item_rect);
        
        //reset item bg
        if (lcd_dev_info.block_id == 0)
        {
            DisplayMenuBg(item_rect,menu_ctrl_ptr,PNULL);
        }
        
        //reset item display
        ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
        DisplaySecMenuItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, FALSE, lcd_dev_info);
        
        MMITHEME_Start3DListAnim();
    }
}
#endif

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecHandleNum(
                                     uint16         num_index,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     )
{
    BOOLEAN     result = FALSE;
    uint16      prev_item_index = 0;
    uint16      prev_first_item_index = 0;
    int16       offset_y = 0;
    BOOLEAN     is_move_item = FALSE;
    GUI_RECT_T  pre_first_item_rect = {0};
    GUI_RECT_T  pre_cur_item_rect = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

#ifdef MEDIA_DEMO_SUPPORT 
    if(!(((get_audio_state() == 6) ||(get_audio_state() == 7)) && ((get_video_state()==2) || (get_video_state()==3) )))
    {
        return TRUE;
    }
#endif

    //set current item index
    if (num_index < menu_ctrl_ptr->item_total_num)
	{
        //stop item text timer
        CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

        if (num_index != menu_ctrl_ptr->cur_item_index)
        {
            pre_first_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
            pre_cur_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

            menu_ctrl_ptr->cur_item_index = num_index;
            // 重新设置offset Y
            if (CTRLMENU_SecResetDisplayItem(menu_ctrl_ptr, &offset_y, FALSE))
            {
                menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);
                
                CalculateSecItemRect(menu_ctrl_ptr);
                
                ResetSecMenuStartIndex(menu_ctrl_ptr);
                
                is_move_item = TRUE;
            }

            //update menu display
            UpdateSecMenuDisplay(TRUE, 
                is_move_item, 
                FALSE,
                prev_first_item_index, 
                &pre_first_item_rect, 
                prev_item_index, 
                &pre_cur_item_rect, 
                menu_ctrl_ptr->item_total_num, 
                menu_ctrl_ptr);
        }

        result = TRUE;
	}

    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle menu touch panel press down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecHandleTpDown(
                                     GUI_POINT_T        *tp_point_ptr,
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     )
{
    uint16          i = 0;
    uint16          item_num = 0;
    uint16          item_index = 0;
    uint16          prev_item_index = 0;
    uint16          line_num = 0;
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    GUI_RECT_T      pre_first_item_rect = {0};
    GUI_RECT_T      pre_cur_item_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if ((PNULL == tp_point_ptr)||(PNULL == menu_ctrl_ptr))
    {
        return;
    }

    menu_ctrl_ptr->is_tp_pressed = TRUE; // 该变量在此表示触笔按下

    //get item number in one page
    item_num = menu_ctrl_ptr->item_total_num;

    // 记录起始点
    menu_ctrl_ptr->slide_info.start_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->slide_info.start_point.y = tp_point_ptr->y;
    menu_ctrl_ptr->slide_info.prev_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->slide_info.prev_point.y = tp_point_ptr->y;

    SCI_TRACE_LOW("CTRLMENU_SecHandleTpDown tp point y=%d, offset_y=%d",tp_point_ptr->y, menu_ctrl_ptr->offset_y);

    // 记录这个时候的滑动状态
    menu_ctrl_ptr->slide_info.tpdown_slide_state = menu_ctrl_ptr->slide_info.slide_state;

    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y ); // 增加速度的点

    if ( MMK_TP_SLIDE_FLING == menu_ctrl_ptr->slide_info.slide_state ) // fling的时候直接返回
    {
        return;
    }

    //if press item
    for (i = 0; i < item_num; i++)
    {
        //get item rect
        item_rect = menu_ctrl_ptr->item_pos_info_ptr[i].item_rect;

        if (GUI_PointIsInRect(*tp_point_ptr,item_rect))
        {
            //get item index
            item_index = i;
            
            if (item_index != menu_ctrl_ptr->cur_item_index)
            {
                CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

                //set previous item index
                prev_item_index = menu_ctrl_ptr->cur_item_index;
                pre_first_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
                pre_cur_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
                
                item_rect = GetSecMenuItemRect(menu_ctrl_ptr, item_index);
                
                if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect))
                {
                    if (item_rect.top < inter_rect.top)
                    {
                        menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.top - item_rect.top);
                        CalculateSecItemRect(menu_ctrl_ptr);
                    }
                    else if (item_rect.bottom > inter_rect.bottom)
                    {
                        menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.bottom - item_rect.bottom);
                        CalculateSecItemRect(menu_ctrl_ptr);
                    }
                    else
                    {
                        SCI_TRACE_LOW("current item is in menu!");
                    }
                }

                //set current item index
                menu_ctrl_ptr->cur_item_index = item_index;

                //get line number
                line_num = menu_ctrl_ptr->item_total_num;
                
                //update menu display
                UpdateSecMenuDisplay(FALSE,
                    FALSE,
                    TRUE,
                    menu_ctrl_ptr->first_item_index,
                    &pre_first_item_rect,
                    prev_item_index,
                    &pre_cur_item_rect,
                    line_num,
                    menu_ctrl_ptr);
            }
			else
			{
				GUI_LCD_DEV_INFO lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
				
				ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
				item_rect = menu_ctrl_ptr->item_pos_info_ptr[i].item_rect;
				DisplayOneItem(menu_ctrl_ptr, item_rect, menu_ctrl_ptr->cur_item_index, TRUE, lcd_dev_info);
			}
            
            break;
        }
    }

    if (menu_ctrl_ptr->common_theme.is_used_slide)
    {
        CTRLMENU_StartRedrawTimer(menu_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : handle menu touch panel press up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SecHandleTpUp(
                                      GUI_POINT_T       *tp_point_ptr,
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    BOOLEAN             result = FALSE;	
    uint16              i = 0;
    uint16              item_num = 0;
    uint16              item_index = 0;
    GUI_RECT_T          item_rect = {0};
    GUI_POINT_T         tpup_point  =  {0};
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
	GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    
    if ((PNULL == tp_point_ptr)||(PNULL == menu_ctrl_ptr))
    {
        return FALSE;
    }

	lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);

    SCI_TRACE_LOW("CTRLMENU_SecHandleTpUp tp point y = %d",tp_point_ptr->y);

    //get selected bar display rect
    menu_ctrl_ptr->is_tp_pressed = FALSE; // 该变量在此表示触笔弹起

    CTRLMENU_StopRedrawTimer(menu_ctrl_ptr);

    if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_info.slide_state)
    {
        // add
        MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );

        // compute
        MMK_ComputeCurrentVelocity( PNULL, &menu_ctrl_ptr->slide_info.fling_velocity, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY );
        
        // 最小速度
        if (FLING_MIN_VELOCITY <= abs((int32)menu_ctrl_ptr->slide_info.fling_velocity))
        {
            CTRLMENU_StartRedrawTimer(menu_ctrl_ptr);
            menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_FLING;
        }
        else
        {
            MMK_ResetVelocityItem();
            menu_ctrl_ptr->slide_info.fling_velocity = 0;

            GUILCD_GetLogicWidthHeight(base_ctrl_ptr->lcd_dev_info.lcd_id, &layer_width, &layer_height);
            
            tpup_point.x = tpup_point.x;
            tpup_point.y = (int16)MIN(tp_point_ptr->y, (int16)layer_height);
            
            // 恢复
            ResumeSecondMenuEx(menu_ctrl_ptr, ResetSecMenuOffsetY(menu_ctrl_ptr));

            menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
            
			// 处理高亮
			lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
			
			ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
			item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
			
			if (menu_ctrl_ptr->common_theme.is_hl_disappear)
			{
				DisplayOneItem(menu_ctrl_ptr, item_rect, menu_ctrl_ptr->cur_item_index, FALSE, lcd_dev_info);
			}
			else
			{
				DisplayOneItem(menu_ctrl_ptr, item_rect, menu_ctrl_ptr->cur_item_index, TRUE, lcd_dev_info);
			}
        }
    }
    else if (GUI_PointIsInRect(*tp_point_ptr,base_ctrl_ptr->rect))
    {
        //get item number in one page
        item_num = menu_ctrl_ptr->item_total_num;

        //if press item
        for (i = 0; i < item_num; i++)
        {
            //get item rect
            item_rect = menu_ctrl_ptr->item_pos_info_ptr[i].item_rect;

            if (GUI_PointIsInRect(*tp_point_ptr,item_rect))
            {
                //get item index
                item_index = i;
            
                if (item_index == menu_ctrl_ptr->cur_item_index)
                {
                    result = TRUE;
                }
                break;
            }
        }

        if (menu_ctrl_ptr->common_theme.is_hl_disappear)
        {
			lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);
			
			ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
			item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
			DisplayOneItem(menu_ctrl_ptr, item_rect, menu_ctrl_ptr->cur_item_index, FALSE, lcd_dev_info);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResumeSecondMenuEx(
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                 int16          move_space
                                 )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
    //恢复双buffer copy
    UILAYER_SwitchDoubleMem( &lcd_dev_info, FALSE );
#endif

    
    if ( 0 == move_space )
    {
#ifdef UI_MULTILAYER_SUPPORT
        if ( !UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer) )
#endif
        {
            DisplayMenuBg(base_ctrl_ptr->rect,menu_ctrl_ptr, PNULL);
        }
#ifdef UI_MULTILAYER_SUPPORT
        else
        {
            UILAYER_Clear(&lcd_dev_info);
        }
#endif
        //display item
        DisplaySecMenuAllItem(menu_ctrl_ptr, TRUE, lcd_dev_info);
        
        //display scroll groove
        DisplayScrollGroove(TRUE,menu_ctrl_ptr);

        return TRUE;
    }
    else
    {
        return ResumeSecondMenu( menu_ctrl_ptr, move_space );
    }
}

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResumeSecondMenu(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                               int16            move_space
                               )
{
    BOOLEAN             result      = TRUE;
    uint16              i           = 0;
    uint16              frame_num   = MMK_RECOVER_SLIDE_TOTAL_FRAME; // 移动频率
    int16               offset_y    = 0;
    int32               offset      = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (0 == move_space)
    {
        return FALSE;
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif       

    offset_y = menu_ctrl_ptr->offset_y;

    lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);

    for (i = MMK_RECOVER_SLIDE_START_FRAME; i <= frame_num; i++)
    {
        offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, move_space, frame_num, i);
        
        menu_ctrl_ptr->offset_y = (int16)(offset_y + offset);

        // 重新计算区域
        CalculateSecItemRect(menu_ctrl_ptr);
        
        // 重新设置start index
        ResetSecMenuStartIndex(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
        if ( !UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer) )
#endif
        {
            DisplayMenuBg(base_ctrl_ptr->rect,menu_ctrl_ptr, PNULL);
        }
#ifdef UI_MULTILAYER_SUPPORT
        else
        {
            UILAYER_Clear(&lcd_dev_info);
        }
#endif

        //display item
        DisplaySecMenuAllItem(menu_ctrl_ptr, TRUE, lcd_dev_info);
        
        //display scroll groove
        DisplayScrollGroove(TRUE,menu_ctrl_ptr);

        if (i != frame_num)
        {
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, base_ctrl_ptr->rect,0);
        }
#ifdef WIN32
        SCI_Sleep(10);
#endif
    }
    
#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif
    
    return result;
}

/*****************************************************************************/
//  Description : handle menu tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecHandleTpMove(
                                     GUI_POINT_T    *tp_point_ptr,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     )
{
    if ((PNULL == tp_point_ptr)||
        (PNULL == menu_ctrl_ptr))
    {
        return;
    }

    // 当move消息来时，停止timer
    if (IS_TP_MOVE_Y(menu_ctrl_ptr->slide_info.start_point.y, tp_point_ptr->y))
    {
        if (MMK_TP_SLIDE_FLING != menu_ctrl_ptr->slide_info.tpdown_slide_state
            || MMK_IsWayChanged(menu_ctrl_ptr->slide_info.fling_velocity, &menu_ctrl_ptr->slide_info.start_point, tp_point_ptr, FALSE))
        {
            menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_TP_SCROLL;
        }
    }

    if (menu_ctrl_ptr->common_theme.is_used_slide)
    {
        MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );
    }
    else
    {
        uint16          i = 0;
        uint16          item_num = 0;
        uint16          item_index = 0;
        uint16          prev_item_index = 0;
        uint16          line_num = 0;
        GUI_RECT_T      item_rect = {0};
        GUI_RECT_T      inter_rect = {0};
        GUI_RECT_T      pre_first_item_rect = {0};
        GUI_RECT_T      pre_cur_item_rect = {0};
        CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

        menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;

        item_num = menu_ctrl_ptr->item_total_num;

        //if press item
        for (i = 0; i < item_num; i++)
        {
            //get item rect
            item_rect = menu_ctrl_ptr->item_pos_info_ptr[i].item_rect;
            
            if (GUI_PointIsInRect(*tp_point_ptr,item_rect))
            {
                //get item index
                item_index = i;
                
                if (item_index != menu_ctrl_ptr->cur_item_index)
                {
                    //set previous item index
                    prev_item_index = menu_ctrl_ptr->cur_item_index;
                    pre_first_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
                    pre_cur_item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
                    
                    item_rect = GetSecMenuItemRect(menu_ctrl_ptr, item_index);
                    
                    if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect))
                    {
                        if (item_rect.top < inter_rect.top)
                        {
                            menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.top - item_rect.top);
                            CalculateSecItemRect(menu_ctrl_ptr);
                        }
                        else if (item_rect.bottom > inter_rect.bottom)
                        {
                            menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.bottom - item_rect.bottom);
                            CalculateSecItemRect(menu_ctrl_ptr);
                        }
                        else
                        {
                            SCI_TRACE_LOW("current item is in menu!");
                        }
                    }
                    
                    //set current item index
                    menu_ctrl_ptr->cur_item_index = item_index;
                    
                    //get line number
                    line_num = menu_ctrl_ptr->item_total_num;
                    
                    //update menu display
                    UpdateSecMenuDisplay(FALSE,
                        FALSE,
                        TRUE,
                        menu_ctrl_ptr->first_item_index,
                        &pre_first_item_rect,
                        prev_item_index,
                        &pre_cur_item_rect,
                        line_num,
                        menu_ctrl_ptr);
                    
                    //stop item text timer
                    CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);
                }
                break;
            }
        }
        
    }
}

/*****************************************************************************/
//  Description : handle menu scroll bar touch panel press msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecHandleTpScroll(
                                       uint16           first_item_index,
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       )
{
    BOOLEAN         is_draw_focus = FALSE;
    BOOLEAN         is_redraw = FALSE;
    uint16          line_num_page = 0;
    int16           offset_y = 0;
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    item_rect = GetSecMenuItemRect(menu_ctrl_ptr, first_item_index);

    if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect) 
        && !GUI_RectIsCovered(base_ctrl_ptr->rect, item_rect))
    {
        is_redraw = TRUE;
    }

    if ((first_item_index < menu_ctrl_ptr->item_total_num) 
        && (first_item_index != menu_ctrl_ptr->first_item_index || is_redraw))
    {
        //stop item text timer
        CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

        //set first item index
        if (first_item_index <= menu_ctrl_ptr->item_total_num - line_num_page)
		{
			menu_ctrl_ptr->first_item_index = first_item_index;
		}
		else
		{
			menu_ctrl_ptr->first_item_index = (uint16)(menu_ctrl_ptr->item_total_num - line_num_page);
		}
        
        offset_y = base_ctrl_ptr->rect.top;

        menu_ctrl_ptr->offset_y = GetSecMenuOffsetY(menu_ctrl_ptr, first_item_index, offset_y);

        is_draw_focus = (BOOLEAN)(menu_ctrl_ptr->common_theme.is_hl_disappear?FALSE:TRUE);
        ReDrawSecondMenu(menu_ctrl_ptr, is_draw_focus, TRUE, TRUE);
    }
}

/*****************************************************************************/
//  Description : handle redraw timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_SecRedrawTimer(
                                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;	
    GUI_POINT_T     point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;    

    MMK_GetLogicTPMsg(&state, &point);

    CTRLMENU_StopRedrawTimer(menu_ctrl_ptr);
    
    if (MMK_IsFocusWin(menu_ctrl_ptr->win_handle)
        && menu_ctrl_ptr->common_theme.is_used_slide)
    {
        CTRLMENU_StartRedrawTimer(menu_ctrl_ptr);
                    
        if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_info.slide_state)
        {
            if (!menu_ctrl_ptr->is_tp_pressed 
                || IS_TP_MOVE_Y(menu_ctrl_ptr->slide_info.prev_point.y, point.y))
            {
                MoveSecondMenu(menu_ctrl_ptr, &point);
                menu_ctrl_ptr->slide_info.prev_point.x = point.x;
                menu_ctrl_ptr->slide_info.prev_point.y = point.y;
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveSecondMenu(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr,
                             GUI_POINT_T    *tp_point_ptr
                             )
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_draw_sk = FALSE;
    int32           offset_y = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if ((PNULL == menu_ctrl_ptr)||
        (PNULL == tp_point_ptr))
    {
        return FALSE;
    }

    if (0 == menu_ctrl_ptr->item_total_num)
    {
        return result;
    }

    offset_y = menu_ctrl_ptr->offset_y;

    if ( MMK_TP_SLIDE_FLING == menu_ctrl_ptr->slide_info.slide_state )
    { 
        menu_ctrl_ptr->offset_y = (int16)MMK_GetFlingOffset(menu_ctrl_ptr->offset_y,menu_ctrl_ptr->slide_info.fling_velocity, (float)FLING_TIME,&menu_ctrl_ptr->slide_info.fling_velocity);

        if (FLING_RUN_MIN_VELOCITY > abs((int32)menu_ctrl_ptr->slide_info.fling_velocity))
        {
            CTRLMENU_StopRedrawTimer(menu_ctrl_ptr);

            is_draw_sk = TRUE;
            
            if (0 != ResetSecMenuOffsetY(menu_ctrl_ptr)) // 判断是否需要恢复
            {
                result = FALSE;
            }
            else
            {
                menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
                result = TRUE;
            }
        }
        else if (0 >= menu_ctrl_ptr->slide_info.fling_velocity) // 向上
        {
            int16 end_offset_y = 0;
            uint16 item_height = 0;
            uint16 item_space  = 0;
            
            // 记录
            item_height = menu_ctrl_ptr->sec_theme.item_height;
            item_space = menu_ctrl_ptr->sec_theme.item_space;
            
            end_offset_y = (int16)(menu_ctrl_ptr->item_total_num*(item_height + item_space) + (menu_ctrl_ptr->sec_theme.expand_item_height - item_height));

            // 计算最大偏移
            end_offset_y = (int16)(base_ctrl_ptr->rect.top - end_offset_y);

            if (menu_ctrl_ptr->offset_y < end_offset_y + (base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top)/2) // 1/2处恢复
            {
                CTRLMENU_StopRedrawTimer(menu_ctrl_ptr);

                menu_ctrl_ptr->slide_info.fling_velocity = 0;

                result = FALSE;
            }
            else
            {
                result = TRUE;
            }
        }
        else // 向下
        {
            int16  top_offset_y = 0;
            
            // 第一条显示在第一行的偏移量
            top_offset_y = base_ctrl_ptr->rect.top;
            
            if (menu_ctrl_ptr->offset_y > top_offset_y + (base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top)/2) // 1/2处恢复
            {
                CTRLMENU_StopRedrawTimer(menu_ctrl_ptr);

                menu_ctrl_ptr->slide_info.fling_velocity = 0;

                result = FALSE;
            }
            else
            {
                result = TRUE;
            }
        }
    }
    else if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_info.slide_state)
    {
        menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + (tp_point_ptr->y - menu_ctrl_ptr->slide_info.prev_point.y));

        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    if (result)
    {
        SCI_MEMSET(&menu_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));

#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetDirectDraw( TRUE );
        UILAYER_SetMainLayerImmutable( TRUE );
        
        //设置双buffer切换，不使用copy
        DoDoubleMemWriteBack( menu_ctrl_ptr, offset_y );
#endif

        ReDrawSecondMenu(menu_ctrl_ptr, TRUE, is_draw_sk, TRUE);

        SCI_MEMSET(&menu_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));
    }
    else
    {
        ResumeSecondMenuEx(menu_ctrl_ptr, ResetSecMenuOffsetY(menu_ctrl_ptr));
        
        menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;

        if (!menu_ctrl_ptr->common_theme.is_hl_disappear)
        {
            DisplayHighlightItem( menu_ctrl_ptr);
        }
    }
    
    return result;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 拷贝不需要更新的buffer
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DoDoubleMemWriteBack(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                int32           offset_y
                                )
{
    GUI_RECT_T  src_rc = {0};
    GUI_RECT_T  dst_rc = {0};
    int32       offset = 0;
    uint16      top_width = 0;
    uint16      top_height = 0;
    uint16      bottom_width = 0;
    uint16      bottom_height = 0;
    UILAYER_MEM_FORMAT_T old_format = 0;

    if( PNULL == menu_ctrl_ptr )
    {
        return;
    }

    if ( !( menu_ctrl_ptr->common_theme.is_used_slide
        && UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer ) ) )
    {
        return;
    }

    offset = (int32)(menu_ctrl_ptr->offset_y - offset_y);

    //根据old format判断是否第一帧
    old_format = UILAYER_GetLayerMemFormat( &menu_ctrl_ptr->item_layer );
    
    //切换双buffer
    if ( !UILAYER_SwitchDoubleMem( &menu_ctrl_ptr->item_layer, TRUE ) )
    {
        return;
    }

    //第一帧需要重画
    if ( !( old_format & UILAYER_MEM_FORMAT_IMMUTABLE ) )
    {
        return;
    }
    
    if ( 0 == offset )
    {
        return;
    }

    src_rc = UILAYER_GetLayerRect( &menu_ctrl_ptr->item_layer );
    
    //去掉converbar区域，避免残留
    MMITHEME_GetCoverBarWidthHeight( menu_ctrl_ptr->win_handle, &top_width, &top_height, &bottom_width, &bottom_height );

    src_rc.top    = (int16)(src_rc.top + top_height);
    src_rc.bottom = (int16)(src_rc.bottom - bottom_height);
    dst_rc = src_rc;

    dst_rc.top    = (int16)(dst_rc.top + offset);
    dst_rc.bottom = (int16)(dst_rc.bottom + offset);

    UILAYER_Clear( &menu_ctrl_ptr->item_layer );
    
    if( UILAYER_DoubleMemWriteBack( &menu_ctrl_ptr->item_layer, &dst_rc, PNULL, &src_rc ) )
    {
        //如果回写bufer成功则将相应的区域值上
        menu_ctrl_ptr->writeback_rect = dst_rc;
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : 根据一个item的索引号和区域，获得offset Y
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetSecMenuOffsetY(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr, 
                              uint16            item_index, 
                              int16             item_top
                              )
{
    int16 offset_y = 0;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return offset_y;
    }

    offset_y = (int16)(item_top - item_index*(menu_ctrl_ptr->sec_theme.item_height + menu_ctrl_ptr->sec_theme.item_space));

    return offset_y;
}

/*****************************************************************************/
//  Description : display scroll item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SecScrollItemStr(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    uint16          text_rect_width = 0;
    uint16          text_pixel_len = 0;
    uint16          icon_width = 0;
    uint16          icon_gap_width = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      clip_rect = {0};
    MMI_STRING_T    text_str = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    item_rect = menu_ctrl_ptr->item_pos_info_ptr[menu_ctrl_ptr->cur_item_index].item_rect;

    GUIRES_GetImgWidthHeight(&icon_width,
            PNULL,
            GetItemIconId(menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr),
            menu_ctrl_ptr->win_handle);

    // 如果有图标，加上图标的间隙
    if (0 != icon_width)
    {
        icon_gap_width = (uint16)(icon_width + menu_ctrl_ptr->sec_theme.gap_width);
    }

    //get text display rect
    text_rect = GetSecMenuItemTextRect(menu_ctrl_ptr, item_rect, icon_gap_width);
    
    text_rect_width = (uint16)(text_rect.right - text_rect.left + 1);

    //get text
    CTRLMENU_GetItem(menu_ctrl_ptr,
        menu_ctrl_ptr->cur_item_index,
        &text_str, PNULL, PNULL);

    //set text string index
    if (MMITHEME_GetIsScrollAsPixel())
    {
        //get item text len
        text_pixel_len = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
                            text_str.wstr_len,
                            menu_ctrl_ptr->sec_theme.cur_item_font,
                            0);

        if ((text_pixel_len - 1 - menu_ctrl_ptr->item_str_index) > text_rect_width)
        {
            menu_ctrl_ptr->item_str_index = (uint16)(menu_ctrl_ptr->item_str_index + menu_ctrl_ptr->sec_theme.pixel_scroll_step);
        }
        else
        {
            menu_ctrl_ptr->item_str_index = 0;
        }
    }
    else
    {
        text_str.wstr_ptr = text_str.wstr_ptr + menu_ctrl_ptr->item_str_index;
        text_str.wstr_len = (uint16)(text_str.wstr_len - menu_ctrl_ptr->item_str_index);

        //get item text len
        text_pixel_len = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
                            text_str.wstr_len,
                            menu_ctrl_ptr->sec_theme.cur_item_font,
                            0);

        //text has display
        if (text_pixel_len > text_rect_width)
        {
            menu_ctrl_ptr->item_str_index++;
        }
        else
        {
            menu_ctrl_ptr->item_str_index = 0;
        }
    }


    //display one item
    if (GUI_IntersectRect(&clip_rect, item_rect, base_ctrl_ptr->rect))
    {

        ClearSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        lcd_dev_info = GetSecMenuItemDispLcdDevInfo(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
        if (UILAYER_IsLayerActive(&menu_ctrl_ptr->item_layer))
        {
            SetBltSecondMenuNotClear(lcd_dev_info.block_id, 0);
        }
#endif

        // 画高亮
        DisplayHighlightItemBg(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, lcd_dev_info);
        
        // 画item
        DisplaySecMenuItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, TRUE, lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : update previous second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_SecModifyRect(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    int16                   offset_y = 0;
    uint16                  i = 0;
    uint16                  menu_level = 0;
    uint16                  first_item_index = 0;
    uint16                  cur_item_index = 0;
    uint16                  line_num_page = 0;
    GUI_RECT_T              menu_rect = {0};
    GUI_RECT_T              item_rect = {0};
    GUI_RECT_T              inter_rect = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    CTRLMENU_NODE_T         *cur_parent_node_ptr = PNULL;
    MMI_MENU_GROUP_ID_T     cur_group_id = 0;

    //save current menu info
    menu_level          = menu_ctrl_ptr->menu_level;
    cur_group_id        = menu_ctrl_ptr->cur_group_id;
    cur_item_index      = menu_ctrl_ptr->cur_item_index;
    first_item_index    = menu_ctrl_ptr->first_item_index;
    menu_rect           = base_ctrl_ptr->rect;
    cur_parent_node_ptr = menu_ctrl_ptr->cur_parent_node_ptr;
    offset_y            = menu_ctrl_ptr->offset_y;

    for (i=0; i<=menu_level; i++)
    {
        if (i < menu_level)
        {
            //set menu info
            CTRLMENU_SetInfo(i,
                menu_ctrl_ptr->menu_stack[i].first_item_index,
                menu_ctrl_ptr->menu_stack[i].cur_item_index,
                menu_rect,
                menu_ctrl_ptr->menu_stack[i].offset_y,
                menu_ctrl_ptr->menu_stack[i].group_id,
                menu_ctrl_ptr->menu_stack[i].parent_node_ptr,
                menu_ctrl_ptr);
        }
        else
        {
            //reset current menu info
            CTRLMENU_SetInfo(menu_level,
                first_item_index,
                cur_item_index,
                menu_rect,
                offset_y,
                cur_group_id,
                cur_parent_node_ptr,
                menu_ctrl_ptr);
        }


        //get line number in one page
        line_num_page = GetLineNumPage(menu_ctrl_ptr);
    
        //current page line number less than one page line number,for delete
        if (menu_ctrl_ptr->item_total_num < (menu_ctrl_ptr->first_item_index +line_num_page))
        {
            if (menu_ctrl_ptr->item_total_num > line_num_page)
            {
                menu_ctrl_ptr->first_item_index = (uint16)(menu_ctrl_ptr->item_total_num - line_num_page);
            }
            else
            {
                menu_ctrl_ptr->first_item_index = 0;
            }
        }
    
        //current item is not in the current page
        if (menu_ctrl_ptr->cur_item_index < menu_ctrl_ptr->first_item_index)
        {
            menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->cur_item_index;
        }
        else if (menu_ctrl_ptr->cur_item_index >= (menu_ctrl_ptr->first_item_index + line_num_page))
        {
            menu_ctrl_ptr->first_item_index = (uint16)(menu_ctrl_ptr->cur_item_index - line_num_page + 1);
        }

#ifdef UI_MULTILAYER_SUPPORT
        ReleaseSecondMenuPageLayer(menu_ctrl_ptr);
    
        CreateSecondMenuPageLayer(menu_ctrl_ptr);
#endif
    
        menu_ctrl_ptr->offset_y = GetSecMenuOffsetY(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index, base_ctrl_ptr->rect.top);
    
        // 重新计算区域
        CreateSecSlideInfo(menu_ctrl_ptr);
        if (CalculateSecItemRect(menu_ctrl_ptr))
        {
            item_rect = GetSecMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
        
            if (GUI_IntersectRect(&inter_rect, item_rect, base_ctrl_ptr->rect))
            {
                if (item_rect.top < inter_rect.top)
                {
                    menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.top - item_rect.top);
                }
                else if (item_rect.bottom > inter_rect.bottom)
                {
                    menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.bottom - item_rect.bottom);
                }
                else
                {
                    SCI_TRACE_LOW("current item is in menu!");
                }
            }
        }

        if (i < menu_level)
        {
            //modify rect and first item index
            menu_ctrl_ptr->menu_stack[i].rect             = base_ctrl_ptr->rect;
            menu_ctrl_ptr->menu_stack[i].first_item_index = menu_ctrl_ptr->first_item_index;
            menu_ctrl_ptr->menu_stack[i].offset_y         = menu_ctrl_ptr->offset_y;
        }
    }
}



