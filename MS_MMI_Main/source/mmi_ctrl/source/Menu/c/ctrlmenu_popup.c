/*****************************************************************************
** File Name:      ctrlmenu_popup.c                                          *
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
#include "mmk_tp.h"
#include "ctrlmenu.h"
#include "ctrlmenu_export.h"
#include "guires.h"
#include "guibutton.h"
#include "guictrl_api.h"
#include "mmi_theme.h"
#include "mmitheme_prgbox.h"
#include "guiprgbox.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define CTRLMENU_CONTEXT_TOP_MARGIN          DP2PX_VALUE(0)          // 上下的
#define CTRLMENU_POPUP_ITEM_SCREENBAR_GAP    DP2PX_VALUE(2)    

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         LOCAL FUNCTION DECLARE                            *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get pop-up menu rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuAdjustMenuRect(
                                       CTRLMENU_OBJ_T     *menu_ctrl_ptr  //in
                                       );

/*****************************************************************************/
//  Description : reset Y offset
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int16 ResetPopMenuOffsetY(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuGetLineNumPage(
                                   CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : 获取item的行高
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL uint16 GetItemLineHeight(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr     //[in]
                                        );

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayBg(
                            CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                            GUI_RECT_T        bg_rect
                            );

#if defined(GUIPOPMENU_POP_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_POP_SUPPORT)
/*****************************************************************************/
//  Description : display pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenu(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : display pop-up menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenuBg(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              GUI_RECT_T        bg_rect
                              );

/*****************************************************************************/
//  Description : 获取context menu的区域
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void CalculatePopupMenuRect(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                  uint16            rect_top        //only for sub pop-up menu
                                  );

/*****************************************************************************/
//  Description : get menu width,include icon,string,tip and scroll bar etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuGetWidth(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : get pop-up menu left coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetPopMenuLeftCoordinate(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     );
#endif

/*****************************************************************************/
//  Description : display all menu item
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 不画边框
/*****************************************************************************/
LOCAL void PopmenuReDisplayMenu(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                BOOLEAN         is_draw_scroll,
                                BOOLEAN         is_draw_softkey
                                );

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
/*****************************************************************************/
//  Description : display context menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void DisplayContextMenu(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : 绘制title
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void DisplayContextMenuTitle(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : display context menu background
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void DisplayContextMenuBg(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                GUI_RECT_T      bg_rect
                                );

/*****************************************************************************/
//  Description : exist button or not
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsExistButton(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : 获取context menu的区域
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void CalculateContextMenuRect(
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : handle button1 msg
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleButton1Click(
                                      MMI_HANDLE_T button_handle
                                      );

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleButton2Click(
                                      MMI_HANDLE_T button_handle
                                      );
#endif

#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
/*****************************************************************************/
//  Description : 获取more menu的区域
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void CalculateMoreMenuRect(
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : display more menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoreMenu(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : display more menu border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoreMenuBorder(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : display more menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoreMenuBg(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             GUI_RECT_T         bg_rect
                             );
#endif

#if defined(GUIPOPMENU_POP_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_POP_SUPPORT)
/*****************************************************************************/
//  Description : display first level pop-up menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenuFirstLevelBg(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : display pop-up menu border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenuBorder(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  );
#endif

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
//   根据offset_y这个变量，计算first_index和end_index
/*****************************************************************************/
LOCAL void PopmenuResetTopItemAndEndItemIndex(
                                              CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                              );

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopmenuIsNeedScrollBar(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayAllItem(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : get selected bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 根据offset获取区域
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetSelectedBarRect(
                                           uint16           item_index,     //in
                                           CTRLMENU_OBJ_T   *menu_ctrl_ptr  //in
                                           );

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuGetScrollBarWidth(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : display one item include icon,text and select bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayOneItem(
                                 BOOLEAN        is_display_sel_bar,
                                 uint16         item_index,
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : display selected bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopMenuDisplaySelectedBar(
                                     uint16         item_index,     //in
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr  //in
                                     );

/*****************************************************************************/
//  Description : display one item include icon, text, no select bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayOneItemNoHighlight(
                                            uint16          item_index,
                                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                            );

/*****************************************************************************/
//  Description : display item icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayItemIcon(
                                  uint16            item_index,
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : get icon image id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T PopmenuGetItemIconId(
                                          uint16            item_index,
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                          );

/*****************************************************************************/
//  Description : get icon display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetItemIconDispRect(
                                            uint16          item_index,
                                            MMI_IMAGE_ID_T  icon_id,
                                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                            );

/*****************************************************************************/
//  Description : get icon buffer display rect
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note: 只适用于动态菜单，且icon为buffer的情况
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetItemIconBufferDispRect(
												  uint16							item_index,
												  CTRLMENU_OBJ_T					*menu_ctrl_ptr
                                            	);

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetIconRect(
                                    GUI_RECT_T  disp_rect, 
                                    GUI_RECT_T  inter_rect, 
                                    uint16      icon_width, 
                                    uint16      icon_height
                                    );

/*****************************************************************************/
//  Description : display item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayItemText(
                                  uint16            item_index,
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : get item text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetItemTextRect(
                                        uint16          item_index,
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        );

#if defined(GUIPOPMENU_POP_POP_SUPPORT)
/*****************************************************************************/
//  Description : display tip icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PommenuDisplayTipIcon(
                                 uint16         item_index,
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : get tip icon display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetItemTipIconDispRect(
                                        uint16          item_index,
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get tip icon image id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetItemTipIconId(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr, // [in]
                                      BOOLEAN           is_grayed       // [in]
                                      );
#endif

/*****************************************************************************/
//  Description : display separator
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplaySeparator(
                                   uint16           item_index,
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : get item separator display rect
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetSeparatorRect(
                                         uint16         item_index,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : display menu scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 该接口只有二级菜单和popmenu使用
/*****************************************************************************/
LOCAL void PopmenuDisplayScrollGroove(
                                      BOOLEAN           is_update,
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetScrollBarRect(
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : reset menu display item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetPopmenuDisplayItem(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                      int16             *offset_y_ptr
                                      );

/*****************************************************************************/
//  Description : update menu display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuUpdateMenuDisplay(
                                    BOOLEAN         is_support_3d,
                                    BOOLEAN         is_draw_all,
                                    uint16          prev_first_item_index,
                                    uint16          prev_item_index,
                                    GUI_RECT_T      *pre_cur_item_rect_ptr, // 为了3D翻转而是用的区域
                                    uint16          line_num,
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : calculator line index in one page
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuCalcLineIndexPage(
                                      uint16    item_index,
                                      uint16    first_item_index,
                                      uint16    item_total_num
                                      );

#ifdef UI_P3D_SUPPORT
/*****************************************************************************/
//  Description : save previous item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuSavePrevItem(
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
LOCAL void PopmenuSavePrevItemBg(
                                 GUI_RECT_T    item_rect
                                 );

/*****************************************************************************/
//  Description : save next item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuSaveNextItem(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               );
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResumePopmenu(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                            int16           move_space
                            );

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopmenuMoveDisplay(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                 GUI_POINT_T        *tp_point_ptr
                                 );

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

/*****************************************************************************/
//  Description : 根据一个item的索引号和区域，获得offset Y
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
//  此方法获取的偏移量，是当item_index为第一个可见项时的偏移
/*****************************************************************************/
LOCAL int16 PopmenuGetOffsetY(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr, 
                              uint16            item_index
                              );

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuModifyRect(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
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
PUBLIC void CTRLMENU_InitPop(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{   
    GUI_RECT_T menu_rect = {0};

    if (PNULL != menu_ctrl_ptr)
    {
        menu_ctrl_ptr->is_move_state = FALSE;
    
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

        // 设置起始位置
        menu_ctrl_ptr->offset_y = menu_rect.top;
    }
}

/*****************************************************************************/
//  Description : get pop-up menu rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuAdjustMenuRect(
                                       CTRLMENU_OBJ_T     *menu_ctrl_ptr  //in
                                       )
{
    GUI_RECT_T      menu_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	GUI_RECT_T      client_rect = {0};
	if (GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style || GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style)
	{
		client_rect = MMITHEME_GetClientRect( );
        base_ctrl_ptr->rect.left = client_rect.left;
        base_ctrl_ptr->rect.right = client_rect.right;
        base_ctrl_ptr->rect.top = client_rect.top;
        base_ctrl_ptr->rect.bottom = client_rect.bottom;
	}
#endif
    if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
    {
#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
        menu_rect.left   = (int16)(base_ctrl_ptr->rect.left + menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.right  = (int16)(base_ctrl_ptr->rect.right - menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.top    = (int16)(base_ctrl_ptr->rect.top + menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom);
#else
        menu_rect.left   = (int16)(base_ctrl_ptr->rect.left + menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.right  = (int16)(base_ctrl_ptr->rect.right - menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.top    = (int16)(base_ctrl_ptr->rect.top + menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - menu_ctrl_ptr->pop_theme.border_width);
#endif
    }
    else
    {
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        menu_rect = base_ctrl_ptr->rect;
#else
        menu_rect.left   = (int16)(base_ctrl_ptr->rect.left + menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.right  = (int16)(base_ctrl_ptr->rect.right - menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.top    = (int16)(base_ctrl_ptr->rect.top + menu_ctrl_ptr->pop_theme.border_width);
        menu_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - menu_ctrl_ptr->pop_theme.border_width);
#endif
    }

    return (menu_rect);
}

/*****************************************************************************/
//  Description : pop-up menu handle special msg
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_PopSpecial(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr, 
                                        MMI_MESSAGE_ID_E    msg_id
                                        )
{
    int32           offset_y = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_MENU:
        if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
        {
            CTRLMENU_NotifyMsg(base_ctrl_ptr->handle, MSG_APP_CANCEL);
        }
        break;

    case MSG_CTL_LOSE_FOCUS:
        menu_ctrl_ptr->is_item_pressed = FALSE;

        offset_y = ResetPopMenuOffsetY(menu_ctrl_ptr);

        if (0 != offset_y) // 判断是否需要恢复
        {
            menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);
            CTRLMENU_DrawPop(menu_ctrl_ptr);
        }
        break;

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    case MSG_CTL_LOSE_ACTIVE:
        // 因为button与menu同级，但刷新是由menu管理的，所以切焦点时，需要重绘
        if (IsExistButton(menu_ctrl_ptr))
        {
            MMK_PostMsg(menu_ctrl_ptr->win_handle, MSG_FULL_PAINT, 0, 0);
        }
        break;
#endif

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    CTRLMENU_HandleAnimMsg(menu_ctrl_ptr, msg_id);
#endif

    return result;
}

/*****************************************************************************/
//  Description : reset Y offset
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int16 ResetPopMenuOffsetY(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                )
{
    uint16 item_num = 0;
    uint16 line_page_num = 0;
    int16  offset_y = 0;
    int16  max_offset_y = 0;
    GUI_RECT_T menu_rect = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return offset_y;
    }
    
    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    if (menu_ctrl_ptr->offset_y >= menu_rect.top)
    {
        offset_y = (int16)(menu_rect.top - menu_ctrl_ptr->offset_y);
    }
    else
    {
        item_num = menu_ctrl_ptr->item_total_num;

        line_page_num = PopmenuGetLineNumPage(menu_ctrl_ptr);
        
        if (menu_ctrl_ptr->item_total_num > line_page_num) // 当超过一页的时候
        {
            // 计算所有item的高度
            max_offset_y = (int16)(item_num*GetItemLineHeight(menu_ctrl_ptr));
        
            // 计算最大偏移
            max_offset_y = (int16)(menu_rect.top - max_offset_y);

            // 判断偏移量是否小于一页的位置
            if(menu_ctrl_ptr->offset_y < max_offset_y + (menu_rect.bottom - menu_rect.top + 1))
            {
                offset_y = (int16)(max_offset_y + (menu_rect.bottom - menu_rect.top + 1) - menu_ctrl_ptr->offset_y);
            }
        }
        else // 条目尚未超过一页
        {
            offset_y = (int16)(menu_rect.top - menu_ctrl_ptr->offset_y);
        }
    }
    
    return offset_y;
}

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuGetLineNumPage(
                                   CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                   )
{
    uint16      line_num_page = 0;
    uint16      menu_height = 0;
    uint16      item_height = 0;
    GUI_RECT_T  menu_rect = {0};

    if (PNULL != menu_ctrl_ptr)
    {
        //get menu height
        menu_rect   = PopmenuAdjustMenuRect(menu_ctrl_ptr);
        menu_height = (uint16)(menu_rect.bottom - menu_rect.top + 1);

        item_height = menu_ctrl_ptr->pop_theme.item_height;
        line_num_page = (uint16)(menu_height/item_height);
    }

    return (line_num_page);
}

/*****************************************************************************/
//  Description : 获取item的行高
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL uint16 GetItemLineHeight(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr     //[in]
                                        )
{
    return (uint16)(menu_ctrl_ptr->pop_theme.item_height + menu_ctrl_ptr->pop_theme.item_space);
}

/*****************************************************************************/
//  Description : display menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DrawPop(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
    {
#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
        DisplayMoreMenu(menu_ctrl_ptr);
#else
        DisplayPopupMenu(menu_ctrl_ptr);
#endif
    }
    else
    {
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        DisplayContextMenu(menu_ctrl_ptr);
#else
        DisplayPopupMenu(menu_ctrl_ptr);
#endif
    }
}

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayBg(
                            CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                            GUI_RECT_T        bg_rect
                            )
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
    {
#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
        DisplayMoreMenuBg(menu_ctrl_ptr, bg_rect);
#else
        DisplayPopupMenuBg(menu_ctrl_ptr, bg_rect);
#endif
    }
    else
    {
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        DisplayContextMenuBg(menu_ctrl_ptr, bg_rect);
#else
        DisplayPopupMenuBg(menu_ctrl_ptr, bg_rect);
#endif
    }
}

#if defined(GUIPOPMENU_POP_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_POP_SUPPORT)
/*****************************************************************************/
//  Description : display pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenu(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            )
{
    GUI_RECT_T      menu_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
#ifdef UI_P3D_SUPPORT 
    GUI_RECT_T      rect = MMITHEME_GetFullScreenRect();
#endif
            
    if (0 == menu_ctrl_ptr->menu_level)
    {
        //display bg
        DisplayPopupMenuFirstLevelBg(menu_ctrl_ptr);
    }
    else
    {
#ifdef UI_P3D_SUPPORT        
        if (CTRLMENU_IsPopMenu(menu_ctrl_ptr) && MMITHEME_Is3DOpen() && menu_ctrl_ptr->is_first_disp)
        {
            UILAYER_InvalidateRect( LCD_ID_0, &rect );    
            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_WIN_SWITCH);    
        }
#endif

#ifdef GUIPOPMENU_POP_POP_SUPPORT
        {
            GUI_LCD_DEV_INFO *lcd_dev_info_ptr = &base_ctrl_ptr->lcd_dev_info;// MMIMENU_GetLcdDevInfo(menu_ctrl_ptr, PNULL);

            //display upper grayed bg
            LCD_FillArgbRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
                menu_ctrl_ptr->menu_stack[menu_ctrl_ptr->menu_level-1].rect,
                //MMITHEME_GetFullScreenRect(),
                menu_ctrl_ptr->pop_theme.mask_color,
                menu_ctrl_ptr->pop_theme.mask_alpha/2);
        }
#endif

        //get pop-up menu rect
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

        //display bg
        PopmenuDisplayBg(menu_ctrl_ptr, menu_rect);
    }

    
    //display border
    DisplayPopupMenuBorder(menu_ctrl_ptr);

    PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);

    //display item
    PopmenuDisplayAllItem(menu_ctrl_ptr);

    //display scroll groove
    PopmenuDisplayScrollGroove(FALSE,menu_ctrl_ptr);

    //sub pop rect > main pop rect,must update previous window,must update softkey
    CTRLMENU_SetSoftkey(menu_ctrl_ptr);
    
#ifdef UI_P3D_SUPPORT 
    if(menu_ctrl_ptr->is_first_disp && MMITHEME_Is3DOpen() &&  CTRLMENU_IsPopMenu(menu_ctrl_ptr))
    {
        if(1 == menu_ctrl_ptr->menu_level)
        {
            menu_rect = base_ctrl_ptr->rect;

            MMITHEME_PopupMenuEffect(&menu_rect, 1, TRUE);

            menu_ctrl_ptr->is_first_disp = FALSE;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : display pop-up menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenuBg(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                              GUI_RECT_T        bg_rect
                              )
{
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr = PNULL;

    if (PNULL != menu_ctrl_ptr &&
        GUI_IntersectRect(&bg_rect, bg_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr)))
    {
        lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

        GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            bg_rect,
            menu_ctrl_ptr->pop_theme.bg_color);
    }
}

/*****************************************************************************/
//  Description : display first level pop-up menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenuFirstLevelBg(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        )
{
    GUI_RECT_T       menu_rect = {0};
#ifdef GUIPOPMENU_POP_POP_SUPPORT
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr = &base_ctrl_ptr->lcd_dev_info;// MMIMENU_GetLcdDevInfo(menu_ctrl_ptr, PNULL);

    //display upper grayed bg
    LCD_FillArgbRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        MMITHEME_GetFullScreenRect(),
        menu_ctrl_ptr->pop_theme.mask_color,
        menu_ctrl_ptr->pop_theme.mask_alpha);
#endif

    //get pop-up menu rect
    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    //fill pop-up menu color
    PopmenuDisplayBg(menu_ctrl_ptr, menu_rect);
}

/*****************************************************************************/
//  Description : display pop-up menu border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayPopupMenuBorder(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  )
{
    GUI_RECT_T          border_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (0 == menu_ctrl_ptr->pop_theme.border_width)
    {
        return;
    }

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

    //set top border rect
    border_rect        = base_ctrl_ptr->rect;
    border_rect.bottom = (int16)(border_rect.top + menu_ctrl_ptr->pop_theme.border_width - 1);

    //display top border
    GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        border_rect,
        menu_ctrl_ptr->pop_theme.border_color);

    //set bottom border rect
    border_rect     = base_ctrl_ptr->rect;
    border_rect.top = (int16)(border_rect.bottom - menu_ctrl_ptr->pop_theme.border_width + 1);

    //display bottom border
    GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        border_rect,
        menu_ctrl_ptr->pop_theme.border_color);

    //set left border rect
    border_rect       = base_ctrl_ptr->rect;
    border_rect.right = (int16)(border_rect.left + menu_ctrl_ptr->pop_theme.border_width - 1);

    //display left border
    GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        border_rect,
        menu_ctrl_ptr->pop_theme.border_color);

    //set right border rect
    border_rect      = base_ctrl_ptr->rect;
    border_rect.left = (int16)(border_rect.right - menu_ctrl_ptr->pop_theme.border_width + 1);

    //display right border
    GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
        border_rect,
        menu_ctrl_ptr->pop_theme.border_color);
}

/*****************************************************************************/
//  Description : 获取context menu的区域
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void CalculatePopupMenuRect(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                  uint16            rect_top        //only for sub pop-up menu
                                  )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          item_count = 0;
    uint16          item_height = 0;
    uint16          item_max_num = 0;
    uint16          border_width = 0;
    uint16          menu_width = 0;
    uint16          max_menu_width = 0;
    int16           client_rect_right = 0;
    int16           client_rect_bottom = 0;
    GUI_RECT_T      menu_rect = {0};
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
    GUI_RECT_T      client_rect = {0};
#endif
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (GUIMENU_STYLE_POPUP_RECT != menu_ctrl_ptr->cur_style)
    {
        //get lcd width and height
        GUILCD_GetLogicWidthHeight(base_ctrl_ptr->lcd_dev_info.lcd_id,&lcd_width,&lcd_height);

        //set item height etc.
        item_height  = GetItemLineHeight(menu_ctrl_ptr);
        border_width = menu_ctrl_ptr->pop_theme.border_width;

        //get item max number
#ifdef MAINLCD_SIZE_128X64
        item_max_num = 3;
#else
        item_max_num = (uint16)(lcd_height*2/3/item_height);
#endif    
        //set item count
        if (item_max_num < menu_ctrl_ptr->item_total_num)
        {
            item_count = item_max_num;
        }
        else
        {
            item_count = menu_ctrl_ptr->item_total_num;
        }

        //get client rect right and bottom
        client_rect_right  = (int16)(lcd_width - 1);
        client_rect_bottom = (int16)(lcd_height - MMITHEME_GetWinSoftkeyHeight(menu_ctrl_ptr->win_handle) - 1);

        if (GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style)
        {
            if (MMK_IsWindowLandscape(menu_ctrl_ptr->win_handle))
            {
                max_menu_width = menu_ctrl_ptr->max_horz_width;
            }
            else
            {
                max_menu_width = menu_ctrl_ptr->max_vert_width;
            }

            // 先把top和bottom算出来，以保证PopmenuGetWidth的正确
            // PopmenuGetWidth计算滚动条时，需要top, bottom
            if ((GUIMENU_EP_BOTTOM_RIGHT == menu_ctrl_ptr->effective_point) || 
                (GUIMENU_EP_BOTTOM_LEFT == menu_ctrl_ptr->effective_point))
            {
                base_ctrl_ptr->rect.top = (int16)(base_ctrl_ptr->rect.bottom - item_height*item_count - border_width * 2 + 1);
            }
            else
            {
                base_ctrl_ptr->rect.bottom = (int16)(base_ctrl_ptr->rect.top + item_height*item_count + border_width * 2 - 1);
            }

            // get pop-up menu width
            // automatically calculate menu's width and height
            menu_width = PopmenuGetWidth(menu_ctrl_ptr);
            if (menu_width > max_menu_width)
            {
                menu_width = max_menu_width;
            }

            if ((GUIMENU_EP_BOTTOM_RIGHT == menu_ctrl_ptr->effective_point) || 
                (GUIMENU_EP_TOP_RIGHT == menu_ctrl_ptr->effective_point))
            {
                base_ctrl_ptr->rect.left = (int16)(base_ctrl_ptr->rect.right - menu_width - border_width * 2 + 1);
            }
            else
            {
                base_ctrl_ptr->rect.right  = (int16)(base_ctrl_ptr->rect.left + menu_width + border_width * 2 - 1);
            }

            // when out of right boundary, set right to right boundary
            if (base_ctrl_ptr->rect.right > client_rect_right)
            {
                base_ctrl_ptr->rect.left = (int16)(base_ctrl_ptr->rect.left - (base_ctrl_ptr->rect.right - client_rect_right));
                base_ctrl_ptr->rect.right = client_rect_right;
            }

            if (0 > base_ctrl_ptr->rect.left)
            {
                base_ctrl_ptr->rect.right = (int16)(base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left);
                base_ctrl_ptr->rect.left = 0;
            }

            // when out of bottom boundary, set bottom to bottom boundary
            if (base_ctrl_ptr->rect.bottom > client_rect_bottom)
            {
                base_ctrl_ptr->rect.top = (int16)(base_ctrl_ptr->rect.top - (base_ctrl_ptr->rect.bottom - client_rect_bottom));
                base_ctrl_ptr->rect.bottom = client_rect_bottom;
            }

            if (0 > base_ctrl_ptr->rect.top)
            {
                base_ctrl_ptr->rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top);
                base_ctrl_ptr->rect.top = 0;
            }
        }
        else
        {
            if (0 == menu_ctrl_ptr->menu_level)
            {
                //set pop-up menu rect
                base_ctrl_ptr->rect.left   = 0;
                base_ctrl_ptr->rect.right  = client_rect_right;
                base_ctrl_ptr->rect.bottom = client_rect_bottom;
                base_ctrl_ptr->rect.top    = (int16)(base_ctrl_ptr->rect.bottom - item_height*item_count - border_width*2 + 1);
            }
            else
            {
                //calculate menu rect
                menu_rect.left   = (int16)(client_rect_right / 2);
                menu_rect.right  = client_rect_right;
                menu_rect.top    = (int16)(rect_top - border_width);
                menu_rect.bottom = (int16)(rect_top + item_height*menu_ctrl_ptr->item_total_num + border_width - 1);
                if (menu_rect.bottom > client_rect_bottom)
                {
                    menu_rect.bottom = client_rect_bottom;
                    menu_rect.top = (int16)(menu_rect.bottom - item_height*item_count - border_width*2 + 1);
                }

                //set sub pop-up menu rect
                base_ctrl_ptr->rect = menu_rect;

                //adjust pop-up menu left position by menu rect
                base_ctrl_ptr->rect.left = GetPopMenuLeftCoordinate(menu_ctrl_ptr);

                GUI_CheckRectDirection(PNULL, &base_ctrl_ptr->rect);
            }
        }
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
        menu_ctrl_ptr->offset_y = menu_rect.top;

        // fix NEWMS00113290
        // 由于offset_y被改了，重新计算下第一条可见项的位置
        CTRLMENU_PopAdjustFirstAndDisplay(menu_ctrl_ptr, FALSE);
    }
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	if (GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style || GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style)
	{
		client_rect = MMITHEME_GetClientRect( );
        base_ctrl_ptr->rect.left = client_rect.left;
        base_ctrl_ptr->rect.right = client_rect.right;
        base_ctrl_ptr->rect.top = client_rect.top;
        base_ctrl_ptr->rect.bottom = client_rect.bottom;
	}
#endif
}

/*****************************************************************************/
//  Description : get menu width,include icon,string,tip and scroll bar etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuGetWidth(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{
    uint16          i = 0;
    uint16          menu_width = 0;
    uint16          icon_width = 0;
    uint16          icon_gap_width = 0;
    uint16          item_width = 0;
    uint16          max_item_width = 0;
    MMI_STRING_T    text_str = {0};
    MMI_IMAGE_ID_T  icon_id = 0;

    if (!CTRLMENU_IsPopMenu(menu_ctrl_ptr))
    {
        return menu_width;
    }

    //add border
    menu_width = (uint16)(menu_ctrl_ptr->pop_theme.border_width<<1);

    //add gap
    menu_width = (uint16)(menu_width + menu_ctrl_ptr->common_theme.gap_width);

    //add icon
    if ((GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style) ||
        (GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style))
    {
        //get icon image id
        icon_id = PopmenuGetItemIconId(0,menu_ctrl_ptr);

        //get icon width
        GUIRES_GetImgWidthHeight(&icon_width,
            PNULL,
            icon_id,
            menu_ctrl_ptr->win_handle);
        
        icon_gap_width = (uint16)(icon_width + menu_ctrl_ptr->common_theme.gap_width);
    }
    menu_width = (uint16)(menu_width + icon_gap_width);

    //get tip icon width
    GUIRES_GetImgWidthHeight(&icon_width,
        PNULL,
        menu_ctrl_ptr->pop_theme.left_img,
        menu_ctrl_ptr->win_handle);
    menu_width = (uint16)(menu_width + icon_width);

    //add scroll bar
    if (PopmenuIsNeedScrollBar(menu_ctrl_ptr))
    {
        menu_width = (uint16)(menu_width + PopmenuGetScrollBarWidth(menu_ctrl_ptr));
    }

    //add string
    for (i=0; i<menu_ctrl_ptr->item_total_num; i++)
    {
        CTRLMENU_GetItem(menu_ctrl_ptr, i, &text_str, PNULL, PNULL);

        //get item text len
        item_width = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
                            text_str.wstr_len,
                            menu_ctrl_ptr->pop_theme.item_font,
                            0);

        //get item string max width
        if (max_item_width < item_width)
        {
            max_item_width = item_width;
        }
    }
    menu_width = (uint16)(menu_width + max_item_width);
    
    return (menu_width);
}

/*****************************************************************************/
//  Description : get pop-up menu left coordinate
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 GetPopMenuLeftCoordinate(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     )
{
    uint16          lcd_width = 0;
    int16           client_rect_right = 0;
    int16           left_coordinate = 0;
    int16           sub_max_left = 0;
    int16           sub_min_left = 0;
    uint16          menu_width = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(base_ctrl_ptr->lcd_dev_info.lcd_id,&lcd_width,PNULL);

    //get client rect right
    client_rect_right = (int16)(lcd_width - 1);

    //get pop-up menu width
    menu_width = PopmenuGetWidth(menu_ctrl_ptr);

    //set pop-up menu left coordinate
    if (client_rect_right > menu_width)
    {
        left_coordinate = (int16)(client_rect_right - menu_width);
    }
    else
    {
        left_coordinate = 0;
    }

    //get sub pop-up max left
    sub_max_left = (int16)(lcd_width/2);
    sub_min_left = (int16)(lcd_width/6);
    
    //adjust left coordinate
    if (left_coordinate < sub_min_left)
    {
        left_coordinate = sub_min_left;
    }
    else if (left_coordinate > sub_max_left)
    {
        left_coordinate = sub_max_left;
    }

    return (left_coordinate);
}
#endif

/*****************************************************************************/
//  Description : adjust first item index and display before ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopAdjustFirstAndDisplay(
                                              CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                              BOOLEAN           is_need_update
                                              )
{
    BOOLEAN     is_adjust = FALSE;
    int16       cur_item_offset = 0;
    uint16      item_height = 0;
    GUI_RECT_T  menu_rect = {0};

    item_height = GetItemLineHeight(menu_ctrl_ptr);
    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    cur_item_offset = (int16)(item_height * menu_ctrl_ptr->cur_item_index);

    if (menu_ctrl_ptr->offset_y + cur_item_offset + item_height - 1 <= menu_rect.top)   // 当前item的底在显示范围之上
    {
        menu_ctrl_ptr->offset_y = (int16)(menu_rect.top - cur_item_offset);     // 将当前项设为第一个可见项
        is_adjust = TRUE;
    }
    else if (menu_ctrl_ptr->offset_y + cur_item_offset - 1 >= menu_rect.bottom) // 当前的顶在显示范围之下
    {
        menu_ctrl_ptr->offset_y = (int16)(menu_rect.bottom - cur_item_offset - item_height + 1); // 将当前项设为最后一个可见项
        is_adjust = TRUE;
    }

    if (is_adjust)
    {
        PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);

        if (is_need_update)
        {
            PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);
        }
    }
}

/*****************************************************************************/
//  Description : display all menu item
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 不画边框
/*****************************************************************************/
LOCAL void PopmenuReDisplayMenu(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                BOOLEAN         is_draw_scroll,
                                BOOLEAN         is_draw_softkey
                                )
{
    GUI_RECT_T      menu_rect = {0};

    //get pop-up menu rect
    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    //若无回写才需要擦背景
    if( GUI_IsZeroRect( menu_ctrl_ptr->writeback_rect ) )
    {
        //display bg
        PopmenuDisplayBg(menu_ctrl_ptr, menu_rect);
    }

    // reset top and end item index
    PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);

    //display item
    PopmenuDisplayAllItem(menu_ctrl_ptr);

    //display scroll groove
    if (is_draw_scroll)
    {
        PopmenuDisplayScrollGroove(TRUE,menu_ctrl_ptr);
    }

	//set softkey param
    if (is_draw_softkey)
    {
        CTRLMENU_SetSoftkey(menu_ctrl_ptr);        
    }
}

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
/*****************************************************************************/
//  Description : display context menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void DisplayContextMenu(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr
                              )
{
    BOOLEAN             is_common_lcd = FALSE;
    int32               i = 0;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, &is_common_lcd);

    // 画背景区
    if (!is_common_lcd)
    {
        // 清MENU内容
        UILAYER_Clear(lcd_dev_info_ptr);

#ifdef UI_MULTILAYER_SUPPORT
        // 画蒙板
        if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->mask_layer_handle))
        {
            LCD_FillArgbRect(
                (const GUI_LCD_DEV_INFO*)&menu_ctrl_ptr->mask_layer_handle,
                MMITHEME_GetFullScreenRect(),
                menu_ctrl_ptr->pop_theme.mask_color,
                menu_ctrl_ptr->pop_theme.mask_alpha);
        }
        else
#endif
        {
            LCD_FillArgbRect(
                MMITHEME_GetDefaultLcdDev(),
                MMITHEME_GetFullScreenRect(),
                menu_ctrl_ptr->pop_theme.mask_color,
                menu_ctrl_ptr->pop_theme.mask_alpha);
        }
    }
    else
    {
        // 半透明蒙板
        LCD_FillArgbRect(
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            MMITHEME_GetFullScreenRect(),
            menu_ctrl_ptr->pop_theme.mask_color,
            menu_ctrl_ptr->pop_theme.mask_alpha);
    }

    // title区
    DisplayContextMenuTitle(menu_ctrl_ptr);

    // item区
    PopmenuDisplayBg(menu_ctrl_ptr, menu_ctrl_ptr->bg_rect);

    // button区
    IMG_EnableTransparentColor(TRUE);

    if (GUIMENU_BUTTON_STYLE_NONE != menu_ctrl_ptr->cur_button_style)
    {
        GUIRES_DisplayImg(
                PNULL, &menu_ctrl_ptr->button_rect, 
                PNULL, menu_ctrl_ptr->win_handle, 
                menu_ctrl_ptr->pop_theme.layout.bottom_bg_id,
                lcd_dev_info_ptr);
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL, &menu_ctrl_ptr->button_rect, 
                PNULL, menu_ctrl_ptr->win_handle, 
                menu_ctrl_ptr->pop_theme.layout.bottom_bg_without_button_id,
                lcd_dev_info_ptr);
    }

    IMG_EnableTransparentColor(FALSE);

    for (i = 0; i < CTRLMENU_BUTTON_NUM; i++)
    {
        if (0 != menu_ctrl_ptr->button_handle[i])
        {
            GUIBUTTON_Update(menu_ctrl_ptr->button_handle[i]);
        }
    }

    PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);

    //display item
    PopmenuDisplayAllItem(menu_ctrl_ptr);

    //display scroll groove
    PopmenuDisplayScrollGroove(FALSE,menu_ctrl_ptr);

    if (menu_ctrl_ptr->is_first_disp && !is_common_lcd)
    {
        menu_ctrl_ptr->is_first_disp = FALSE;
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        CTRLMENU_Zoom(menu_ctrl_ptr, FALSE);
#endif
    }
}

/*****************************************************************************/
//  Description : 绘制title
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void DisplayContextMenuTitle(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    uint16              down_arrow_width = 0;
    uint16              down_arrow_height = 0;
    uint16              title_margin = menu_ctrl_ptr->pop_theme.layout.title_start_margin;
    GUI_RECT_T          down_arrow_rect = {0};
    GUI_RECT_T          title_rect = {0};
    MMI_STRING_T        title_text = {0};
    MMI_STRING_T        sub_title_text = {0};
    MMI_IMAGE_ID_T      title_icon_id = 0;
    GUISTR_STYLE_T      str_style = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR | GUISTR_STATE_ELLIPSIS;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

    // 1. title背景
    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(
            PNULL, &menu_ctrl_ptr->title_rect, 
            PNULL, menu_ctrl_ptr->win_handle, 
            menu_ctrl_ptr->pop_theme.layout.top_bg_id, lcd_dev_info_ptr);

    IMG_EnableTransparentColor(FALSE);

    // 获取title的内容
    CTRLMENU_GetTitleText(menu_ctrl_ptr, &title_text, &sub_title_text, &title_icon_id);

    // 2. 绘制箭头
    if (0 == title_icon_id || MMITHEME_GetInvalidImageID() == title_icon_id)
    {
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
		title_icon_id = menu_ctrl_ptr->pop_theme.down_arrow_img;
#else
		title_icon_id = MMITHEME_GetInvalidImageID();
#endif
    }

    GUIRES_GetImgWidthHeight(
            &down_arrow_width, &down_arrow_height,
            title_icon_id, menu_ctrl_ptr->win_handle);

    // 计算时要减去页边距
    down_arrow_rect        = menu_ctrl_ptr->title_rect;
    down_arrow_rect.top    = (int16)(down_arrow_rect.top + menu_ctrl_ptr->pop_theme.layout.title_v_top_margin);
    down_arrow_rect.bottom = (int16)(down_arrow_rect.bottom - menu_ctrl_ptr->pop_theme.layout.title_v_bottom_margin);
    down_arrow_rect.left   = (int16)(down_arrow_rect.left + title_margin);
    down_arrow_rect.right  = (int16)(down_arrow_rect.left + down_arrow_width - 1);

    down_arrow_rect = GUI_GetCenterRectExt(down_arrow_rect, down_arrow_width, down_arrow_height);

    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(
            PNULL, &down_arrow_rect, 
            PNULL, menu_ctrl_ptr->win_handle, 
            title_icon_id, lcd_dev_info_ptr);

    IMG_EnableTransparentColor(FALSE);

    // 3. 画title文字
    if (PNULL != sub_title_text.wstr_ptr && 0 != sub_title_text.wstr_len)
    {
        // title
        title_rect        = menu_ctrl_ptr->title_rect;
        title_rect.left   = (int16)(down_arrow_rect.right + menu_ctrl_ptr->pop_theme.layout.title_image_text_space + 1);
        title_rect.right  = (int16)(title_rect.right - title_margin);
        title_rect.top    = (int16)(title_rect.top + menu_ctrl_ptr->pop_theme.layout.title_v_top_margin);
        title_rect.bottom = (int16)((title_rect.top  + title_rect.bottom) / 2);

        str_style.align = ALIGN_BOTTOM;
        str_style.font = menu_ctrl_ptr->pop_theme.item_font;
        str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_CONTEXTMENU_TITLE_FONT);

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            (const GUI_RECT_T*)&title_rect,
            (const GUI_RECT_T*)&title_rect,
            (const MMI_STRING_T*)&title_text,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);

        // sub title
        title_rect.top    = (int16)(title_rect.bottom + 1);
        title_rect.bottom = (int16)(menu_ctrl_ptr->title_rect.bottom - menu_ctrl_ptr->pop_theme.layout.title_v_bottom_margin);

        str_style.align = ALIGN_LVMIDDLE;
        str_style.font = menu_ctrl_ptr->pop_theme.context_title_font;

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            (const GUI_RECT_T*)&title_rect,
            (const GUI_RECT_T*)&title_rect,
            (const MMI_STRING_T*)&sub_title_text,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
    }
    else
    {
        title_rect        = menu_ctrl_ptr->title_rect;
        title_rect.left   = (int16)(down_arrow_rect.right + menu_ctrl_ptr->pop_theme.layout.title_image_text_space + 1);
        title_rect.right  = (int16)(title_rect.right - title_margin);
        title_rect.top    = (int16)(title_rect.top + menu_ctrl_ptr->pop_theme.layout.title_v_top_margin);
        title_rect.bottom = (int16)(title_rect.bottom - menu_ctrl_ptr->pop_theme.layout.title_v_bottom_margin);

        str_style.align = ALIGN_LVMIDDLE;
        str_style.font = menu_ctrl_ptr->pop_theme.item_font;
        str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_CONTEXTMENU_TITLE_FONT);

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            (const GUI_RECT_T*)&title_rect,
            (const GUI_RECT_T*)&title_rect,
            (const MMI_STRING_T*)&title_text,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
    }
}

/*****************************************************************************/
//  Description : display context menu background
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void DisplayContextMenuBg(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                GUI_RECT_T      bg_rect
                                )
{
    BOOLEAN             is_common_lcd = FALSE;
    GUI_RECT_T          bg_img_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    if (PNULL != menu_ctrl_ptr)
    {
        lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr,&is_common_lcd);

        if (GUI_IntersectRect(&bg_rect, bg_rect, menu_ctrl_ptr->bg_rect))
        {
        	// 如果只是画内容区域，用单色填充来优化
        	if (GUI_RectIsCovered(base_ctrl_ptr->rect, bg_rect)
				&& MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_info.slide_state)
    		{
    			GUI_FillRect(lcd_dev_info_ptr, bg_rect, menu_ctrl_ptr->pop_theme.layout.ctrl_bg_color);
    		}
			else
			{
	            bg_img_rect        = bg_rect;
	            bg_img_rect.left   = (int16)(bg_img_rect.left - menu_ctrl_ptr->bg_rect.left);
	            bg_img_rect.right  = (int16)(bg_img_rect.right - menu_ctrl_ptr->bg_rect.left);
	            bg_img_rect.top    = (int16)(bg_img_rect.top - menu_ctrl_ptr->bg_rect.top);
	            bg_img_rect.bottom = (int16)(bg_img_rect.bottom - menu_ctrl_ptr->bg_rect.top);

	            IMG_EnableTransparentColor(TRUE);

	            GUIRES_DisplayImg(
	                    PNULL, &bg_rect, 
	                    &bg_img_rect, menu_ctrl_ptr->win_handle, 
	                    menu_ctrl_ptr->pop_theme.layout.client_bg_id,
	                    lcd_dev_info_ptr);

	            IMG_EnableTransparentColor(FALSE);
			}
        }
    }
}

/*****************************************************************************/
//  Description : exist button or not
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsExistButton(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                            )
{
    BOOLEAN is_exist = FALSE;
    int16   i = 0;

    for (i = 0; i < CTRLMENU_BUTTON_NUM; i++)
    {
        if (0 != menu_ctrl_ptr->button_handle[i])
        {
            is_exist = TRUE;
            break;
        }
    }

    return is_exist;
}

/*****************************************************************************/
//  Description : 获取context menu的区域
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void CalculateContextMenuRect(
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                    )
{
    uint16          i = 0;
    uint16          title_height = 0;
    uint16          button_height = 0;
    uint16          button_img_height = 0;
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          actual_menu_height = 0;
    uint16          max_menu_height = 0;
    uint16          button_num = 0;
    uint16          softkey_height = 0;
    uint16          h_add_win_margin = 0;
    int16           start_y = 0;
    GUI_RECT_T      menu_rect = {0};
    GUI_FONT_ALL_T  font_all = {0};
    IGUICTRL_T*     button_ctrl = PNULL;
    CAF_HANDLE_T    button_handle = 0;
    MMI_TEXT_ID_T   button_text[CTRLMENU_BUTTON_NUM] = {0};
    GUI_RECT_T      button_rect[CTRLMENU_BUTTON_NUM] = {0};
	GUI_BG_T        img_info          = {0};
	GUI_BG_T        img_fg_press_info = {0};
	GUI_BG_T        img_fg_gray_info  = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;
    GUIBUTTON_CALLBACK_FUNC_EXT button_callback[CTRLMENU_BUTTON_NUM] = 
                            {
                                HandleButton1Click,
                                HandleButton2Click
                            };

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(base_ctrl_ptr->lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    if (lcd_width > lcd_height)
    {
        h_add_win_margin = menu_ctrl_ptr->pop_theme.layout.win_h_add_margin;
    }

    // 最大的menu客户区高度
    max_menu_height = (uint16)(lcd_height - softkey_height - MMITHEME_GetStatusBarHeight());
    max_menu_height = (uint16)(max_menu_height - CTRLMENU_CONTEXT_TOP_MARGIN*2);

    // 计算menu需要的高度
    actual_menu_height = (uint16)(menu_ctrl_ptr->item_total_num * GetItemLineHeight(menu_ctrl_ptr));

    // title区域的高度
    GUIRES_GetImgWidthHeight(
            PNULL, &title_height,
            menu_ctrl_ptr->pop_theme.layout.top_bg_id,
            menu_ctrl_ptr->win_handle);

    actual_menu_height = (uint16)(actual_menu_height + title_height);

    // button区域的高度
    if (GUIMENU_BUTTON_STYLE_NONE != menu_ctrl_ptr->cur_button_style)
    {
        GUIRES_GetImgWidthHeight(
                PNULL, &button_height,
                menu_ctrl_ptr->pop_theme.layout.bottom_bg_id,
                menu_ctrl_ptr->win_handle);
    }
    else
    {
        GUIRES_GetImgWidthHeight(
                PNULL, &button_height,
                menu_ctrl_ptr->pop_theme.layout.bottom_bg_without_button_id,
                menu_ctrl_ptr->win_handle);
    }

    actual_menu_height = (uint16)(actual_menu_height + button_height);

    // 实际的menu高度
    if (actual_menu_height > max_menu_height)
    {
        actual_menu_height = max_menu_height;
    }

    // 起始位置
    start_y = (int16)((max_menu_height - actual_menu_height) / 2);
    start_y = (int16)(start_y + MMITHEME_GetStatusBarHeight() + CTRLMENU_CONTEXT_TOP_MARGIN);   /*lint !e737*/

    // title的区域
    // TITLE的图片是整修屏幕宽，所以这里不扣除MARGIN的区域
    menu_ctrl_ptr->title_rect.left   = h_add_win_margin;
    menu_ctrl_ptr->title_rect.right  = (int16)(lcd_width - 1 - h_add_win_margin);
    menu_ctrl_ptr->title_rect.top    = start_y;
    menu_ctrl_ptr->title_rect.bottom = (int16)(menu_ctrl_ptr->title_rect.top + title_height - 1);

    // button的区域
    // BUTTON的图片是整修屏幕宽，所以这里不扣除MARGIN的区域
    menu_ctrl_ptr->button_rect.left   = h_add_win_margin;
    menu_ctrl_ptr->button_rect.right  = (int16)(lcd_width - 1 - h_add_win_margin);
    menu_ctrl_ptr->button_rect.bottom = (int16)(start_y + actual_menu_height - 1);
    menu_ctrl_ptr->button_rect.top    = (int16)(menu_ctrl_ptr->button_rect.bottom - button_height + 1);

    // 创建button
    if (GUIMENU_BUTTON_STYLE_OK_CANCEL == menu_ctrl_ptr->cur_button_style)
    {
        GUIRES_GetImgWidthHeight(
                PNULL, &button_img_height,
                menu_ctrl_ptr->pop_theme.layout.button_img_id,
                menu_ctrl_ptr->win_handle);

        // 计算时要减去页边距
        button_rect[0] = menu_ctrl_ptr->button_rect;
        button_rect[0].bottom = (int16)(button_rect[0].bottom - menu_ctrl_ptr->pop_theme.layout.button_bottom_margin);

        button_rect[0].left   = (int16)(button_rect[0].left + (menu_ctrl_ptr->pop_theme.layout.button_margin));
        button_rect[0].right  = (int16)((menu_ctrl_ptr->button_rect.left + menu_ctrl_ptr->button_rect.right - menu_ctrl_ptr->pop_theme.layout.button_space) / 2);
        button_rect[0].top    = (int16)((button_rect[0].top + button_rect[0].bottom + 1 - button_img_height) / 2);
        button_rect[0].bottom = (int16)(button_rect[0].top + button_img_height);

        button_text[0] = menu_ctrl_ptr->common_theme.text_ok_id;

        button_rect[1]        = menu_ctrl_ptr->button_rect;
        button_rect[1].left   = (int16)((menu_ctrl_ptr->button_rect.left + menu_ctrl_ptr->button_rect.right + menu_ctrl_ptr->pop_theme.layout.button_space) / 2);
        button_rect[1].right  = (int16)(button_rect[1].right - menu_ctrl_ptr->pop_theme.layout.button_margin);
        button_rect[1].top    = button_rect[0].top;
        button_rect[1].bottom = button_rect[0].bottom;

        button_text[1] = menu_ctrl_ptr->common_theme.text_cancel_id;

        button_num = 2;
    }
    else if ((GUIMENU_BUTTON_STYLE_OK == menu_ctrl_ptr->cur_button_style) || 
             (GUIMENU_BUTTON_STYLE_CANCEL == menu_ctrl_ptr->cur_button_style))
    {
        GUIRES_GetImgWidthHeight(
                PNULL, &button_img_height,
                menu_ctrl_ptr->pop_theme.layout.button_img_id,
                menu_ctrl_ptr->win_handle);

        // 计算时要减去页边距
        button_rect[0] = menu_ctrl_ptr->button_rect;
        button_rect[0].bottom = (int16)(button_rect[0].bottom - menu_ctrl_ptr->pop_theme.layout.button_bottom_margin);

        button_rect[0].left   = (int16)(button_rect[0].left + menu_ctrl_ptr->pop_theme.layout.button_margin);
        button_rect[0].right  = (int16)(button_rect[0].right - menu_ctrl_ptr->pop_theme.layout.button_margin);
        button_rect[0].top    = (int16)((button_rect[0].top + button_rect[0].bottom + 1 - button_img_height) / 2);
        button_rect[0].bottom = (int16)(button_rect[0].top + button_img_height);

        if (GUIMENU_BUTTON_STYLE_OK == menu_ctrl_ptr->cur_button_style)
        {
            button_text[0] = menu_ctrl_ptr->common_theme.text_ok_id;
        }
        else
        {
            button_text[0] = menu_ctrl_ptr->common_theme.text_cancel_id;
        }

        button_num = 1;
    }

    SCI_ASSERT(button_num <= CTRLMENU_BUTTON_NUM);   /*assert verified*/

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

    for (i = 0; i < CTRLMENU_BUTTON_NUM; i++)
    {
        if (i < button_num)
        {
            if (0 != menu_ctrl_ptr->button_handle[i])
            {
                GUIBUTTON_SetRect(menu_ctrl_ptr->button_handle[i], &button_rect[i]);
            }
            else
            {
                MMI_CONTROL_CREATE_T    ctrl_creat = {0};
                GUIBUTTON_INIT_DATA_T   init_data = {0};

                //init data
                init_data.both_rect.v_rect = button_rect[i];
                init_data.both_rect.h_rect = init_data.both_rect.v_rect;
                init_data.bg.bg_type = GUI_BG_IMG;
                init_data.bg.img_id = menu_ctrl_ptr->pop_theme.layout.button_img_id;

                ctrl_creat.guid               = SPRD_GUI_BUTTON_ID;
                ctrl_creat.parent_win_handle  = menu_ctrl_ptr->win_handle;
    			ctrl_creat.parent_ctrl_handle = base_ctrl_ptr->handle;
                ctrl_creat.init_data_ptr      = &init_data;

                //creat button control
                button_ctrl = MMK_CreateControl(&ctrl_creat);
                if (PNULL == button_ctrl)
                {
                    SCI_TRACE_LOW("== CalculateContextMenuRect == create button failed!");
                    continue;
                }

                button_handle = GUICTRL_GetCtrlHandle(button_ctrl);
                menu_ctrl_ptr->button_handle[i] = button_handle;

                GUIBUTTON_SetRunSheen(button_handle, FALSE);

                GUIBUTTON_SetForbidPaint(button_handle, TRUE);

                font_all = menu_ctrl_ptr->pop_theme.layout.button_font;
                GUIBUTTON_SetFont(button_handle, &font_all);

                init_data.bg.img_id = menu_ctrl_ptr->pop_theme.layout.button_img_hl_id;
                GUIBUTTON_SetPressedBg( button_handle, &init_data.bg );

                GUIAPICTRL_SetState(button_handle, GUICTRL_STATE_KEYMODE_ACTIVE, TRUE);

                GUIBUTTON_SetCallBackFuncExt(button_handle, button_callback[i]);
            }

            if (0 != menu_ctrl_ptr->pop_theme.layout.button_ok_normal_img)
            {
    			img_info.bg_type          = GUI_BG_IMG;
                img_fg_press_info.bg_type = GUI_BG_IMG;
    			img_fg_gray_info.bg_type  = GUI_BG_IMG;

                if (menu_ctrl_ptr->common_theme.text_ok_id == button_text[i])
                {
    				img_info.img_id          = menu_ctrl_ptr->pop_theme.layout.button_ok_normal_img;
                    img_fg_press_info.img_id = menu_ctrl_ptr->pop_theme.layout.button_ok_press_img;
    				img_fg_gray_info.img_id  = menu_ctrl_ptr->pop_theme.layout.button_ok_gray_img;
                }
                else
                {
    				img_info.img_id          = menu_ctrl_ptr->pop_theme.layout.button_cancel_normal_img;
                    img_fg_press_info.img_id = menu_ctrl_ptr->pop_theme.layout.button_cancel_press_img;
    				img_fg_gray_info.img_id  = menu_ctrl_ptr->pop_theme.layout.button_cancel_gray_img;
                }

    			GUIBUTTON_SetGrayedFg(button_handle, FALSE, &img_fg_gray_info, FALSE);
    			GUIBUTTON_SetFg(button_handle, &img_info);
    			GUIBUTTON_SetPressedFg(button_handle, &img_fg_press_info);

                if (2 == button_num)
                {
                    GUIBUTTON_SetStyle(button_handle,GUIBUTTON_STYLE_LIRTL);
                }
                else
                {
                    GUIBUTTON_SetStyle(button_handle,GUIBUTTON_STYLE_LIRTM);
                }
            }

            GUIAPICTRL_SetLcdDevInfo(button_handle, lcd_dev_info_ptr);

            GUIBUTTON_SetTextId(button_handle, button_text[i]);
        }
        else
        {
            if (0 != menu_ctrl_ptr->button_handle[i])
            {
                MMK_DestroyControl(menu_ctrl_ptr->button_handle[i]);
                menu_ctrl_ptr->button_handle[i] = 0;
            }
        }
    }

    // menu item的区域
    menu_ctrl_ptr->bg_rect.left   = menu_ctrl_ptr->title_rect.left;
    menu_ctrl_ptr->bg_rect.right  = menu_ctrl_ptr->title_rect.right;
    menu_ctrl_ptr->bg_rect.top    = (int16)(menu_ctrl_ptr->title_rect.bottom + 1);
    menu_ctrl_ptr->bg_rect.bottom = (int16)(menu_ctrl_ptr->button_rect.top - 1);
    if (menu_ctrl_ptr->bg_rect.top > menu_ctrl_ptr->bg_rect.bottom)
    {
        menu_ctrl_ptr->bg_rect.top = menu_ctrl_ptr->bg_rect.bottom;
    }

    // 计算时要减去页边距
    base_ctrl_ptr->rect = menu_ctrl_ptr->bg_rect;
    base_ctrl_ptr->rect.left  = (int16)(base_ctrl_ptr->rect.left + menu_ctrl_ptr->pop_theme.layout.client_h_margin);
    base_ctrl_ptr->rect.right = (int16)(base_ctrl_ptr->rect.right - menu_ctrl_ptr->pop_theme.layout.client_h_margin);

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
    menu_ctrl_ptr->offset_y = menu_rect.top;
}

/*****************************************************************************/
//  Description : handle button1 msg
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleButton1Click(
                                      MMI_HANDLE_T button_handle
                                      )
{
    MMI_HANDLE_T    menu_handle = MMK_GetParentCtrlHandle(button_handle);
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    if (0 != menu_handle
        && PNULL != (menu_ctrl_ptr = (CTRLMENU_OBJ_T  *)MMK_GetCtrlPtr(menu_handle)))
    {
        if (0 != menu_ctrl_ptr->button_handle[0])
        {
            base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

            switch (menu_ctrl_ptr->cur_button_style)
            {
            case GUIMENU_BUTTON_STYLE_OK:
            case GUIMENU_BUTTON_STYLE_OK_CANCEL:
                CTRLMENU_NotifyMsg(base_ctrl_ptr->handle, MSG_TP_PRESS_UP);
                break;


            case GUIMENU_BUTTON_STYLE_CANCEL:
            default:
                CTRLMENU_NotifyMsg(base_ctrl_ptr->handle, MSG_APP_CANCEL);
                break;
            }
        }
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleButton2Click(
                                      MMI_HANDLE_T button_handle
                                      )
{
    MMI_HANDLE_T    menu_handle = MMK_GetParentCtrlHandle(button_handle);
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    if (0 != menu_handle
        && PNULL != (menu_ctrl_ptr = (CTRLMENU_OBJ_T *)MMK_GetCtrlPtr(menu_handle)))
    {
        if (GUIMENU_BUTTON_STYLE_OK_CANCEL == menu_ctrl_ptr->cur_button_style
            && 0 != menu_ctrl_ptr->button_handle[1])
        {
            base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

            CTRLMENU_NotifyMsg(base_ctrl_ptr->handle, MSG_APP_CANCEL);
        }
    }


    return MMI_RESULT_TRUE;
}
#endif

#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
/*****************************************************************************/
//  Description : 获取more menu的区域
//  Global resource dependence : 
//  Author: hua.fang
//  Note:  
/*****************************************************************************/
LOCAL void CalculateMoreMenuRect(
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr
                                 )
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          item_height = 0;
    uint16          border_width = 0;
    uint16          max_menu_height = 0;
    uint16          actual_menu_height = 0;
    uint16          softkey_height = 0;
    GUI_RECT_T      menu_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(0 == menu_ctrl_ptr->menu_level);     /*assert verified*/
    if (0 != menu_ctrl_ptr->menu_level)
    {
        return;
    }

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(base_ctrl_ptr->lcd_dev_info.lcd_id,&lcd_width,&lcd_height);

    // max menu height
    max_menu_height = (uint16)(lcd_height - softkey_height);

    if (MMK_IsIncludeStatusBar(menu_ctrl_ptr->win_handle))
    {
        max_menu_height = (uint16)(max_menu_height - MMITHEME_GetStatusBarHeight());
    }

    // actual menu height
    item_height  = GetItemLineHeight(menu_ctrl_ptr);
    border_width = menu_ctrl_ptr->pop_theme.border_width;

    // 边线只有上面一边
    actual_menu_height = (uint16)(menu_ctrl_ptr->item_total_num * item_height + border_width);

    // 当超过最大高度时，用最大的
    if (actual_menu_height > max_menu_height)
    {
        actual_menu_height = max_menu_height;
    }

    //set pop-up menu rect
    base_ctrl_ptr->rect.left   = (int16)((lcd_width - MMITHEME_MOREMENU_WIDTH) / 2);
    base_ctrl_ptr->rect.right  = (int16)(base_ctrl_ptr->rect.left + MMITHEME_MOREMENU_WIDTH - 1);
    base_ctrl_ptr->rect.bottom = (int16)(lcd_height - softkey_height - 1);
    base_ctrl_ptr->rect.top    = (int16)(base_ctrl_ptr->rect.bottom - actual_menu_height + 1);

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
    menu_ctrl_ptr->offset_y = menu_rect.top;
}

/*****************************************************************************/
//  Description : display more menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoreMenu(
                           CTRLMENU_OBJ_T   *menu_ctrl_ptr
                           )
{
    BOOLEAN             is_common_lcd = FALSE;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;
    GUI_RECT_T          menu_rect = {0};

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, &is_common_lcd);

    // 画背景区
    if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
    {
        if (!is_common_lcd)
        {
            // 清MENU内容
            UILAYER_Clear(lcd_dev_info_ptr);
        }
        
        SCI_ASSERT(0 == menu_ctrl_ptr->menu_level);     /*assert verified*/

        //get pop-up menu rect
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

        //display bg
        PopmenuDisplayBg(menu_ctrl_ptr, menu_rect);

        //display border
        DisplayMoreMenuBorder(menu_ctrl_ptr);
    }

    PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);

    //display item
    PopmenuDisplayAllItem(menu_ctrl_ptr);

    //display scroll groove
    PopmenuDisplayScrollGroove(FALSE,menu_ctrl_ptr);

    if (menu_ctrl_ptr->is_first_disp && !is_common_lcd)
    {
        menu_ctrl_ptr->is_first_disp = FALSE;
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        CTRLMENU_MoveDown(menu_ctrl_ptr, FALSE);
#endif
    }
}

/*****************************************************************************/
//  Description : display more menu border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoreMenuBorder(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 )
{
    GUI_RECT_T          border_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (0 == menu_ctrl_ptr->pop_theme.border_width)
    {
        return;
    }

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

    border_rect = base_ctrl_ptr->rect;

    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(
            PNULL, &border_rect, 
            PNULL, menu_ctrl_ptr->win_handle, 
            menu_ctrl_ptr->pop_theme.border_img,
            lcd_dev_info_ptr);

    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : display more menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoreMenuBg(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                             GUI_RECT_T         bg_rect
                             )
{
    BOOLEAN             is_common_lcd = FALSE;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    if (PNULL != menu_ctrl_ptr)
    {
        lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, &is_common_lcd);

        if (GUI_IntersectRect(&bg_rect, bg_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr)))
        {
            if (is_common_lcd)
            {
                GUI_FillRect(
                    (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
                    bg_rect, menu_ctrl_ptr->pop_theme.bg_color);
            }
            else
            {
                LCD_FillArgbRect(
                    (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
                    bg_rect,
                    menu_ctrl_ptr->pop_theme.bg_color,
                    menu_ctrl_ptr->pop_theme.bg_alpha);
            }
        }
    }
}
#endif

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
//   根据offset_y这个变量，计算first_index和end_index
/*****************************************************************************/
LOCAL void PopmenuResetTopItemAndEndItemIndex(
                                              CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                              )
{
    uint16 i = 0;
    uint16 item_num = 0;
    uint16 item_height = 0;
    uint16 total_item_num = 0;
    uint32 total_item_height = 0;
    int32  offset_y = 0;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T inter_rect = {0};
    GUI_RECT_T menu_rect = {0};
    BOOLEAN is_start = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;
    
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    total_item_num = menu_ctrl_ptr->item_total_num;
	menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
	base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (0 == total_item_num)
    {
        menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->end_item_index = 0;
        menu_ctrl_ptr->offset_y = menu_rect.top;
        return;
    }

    offset_y = menu_ctrl_ptr->offset_y;

    item_rect.left  = menu_rect.left;
    item_rect.right = menu_rect.right;
    
    if (PopmenuIsNeedScrollBar( menu_ctrl_ptr ) )
    {
        item_rect.right = (int16)( item_rect.right - (MMITHEME_GetScrollBarWidthByState(menu_ctrl_ptr->common_theme.is_used_slide) + CTRLMENU_POPUP_ITEM_SCREENBAR_GAP) );
        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &item_rect);
    }

    item_height = GetItemLineHeight(menu_ctrl_ptr);

    for (i = 0; i < total_item_num; i++)
    {
        total_item_height += item_height;

        if (offset_y + (int32)total_item_height - (int32)item_height > menu_rect.bottom /*lint !e737*/
            || offset_y + (int32)total_item_height - 1 < menu_rect.top) /*lint !e737*/
        {
            continue;
        }

        item_rect.top = (int16)(offset_y + (int32)total_item_height - (int32)item_height);/*lint !e737*/

        item_rect.bottom = (int16)(item_rect.top + item_height - 1);

        if (GUI_IntersectRect(&inter_rect, item_rect, menu_rect))
        {
            item_num++;

            if (!is_start)
            {
                menu_ctrl_ptr->first_item_index = i;
            }
            
            is_start = TRUE;
        }
    }

    if (item_num > 0)
    {
        menu_ctrl_ptr->end_item_index = (uint16)(menu_ctrl_ptr->first_item_index + item_num - 1);
    }
    else
    {
        menu_ctrl_ptr->end_item_index = 0;
    }
}

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopmenuIsNeedScrollBar(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     )
{
    BOOLEAN     result = FALSE;
    uint16      line_num_page = 0;

    //get line number in one page
    line_num_page = PopmenuGetLineNumPage(menu_ctrl_ptr);
    if (line_num_page < menu_ctrl_ptr->item_total_num)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayAllItem(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                 )
{
    uint16          i = 0;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    for (i = menu_ctrl_ptr->first_item_index; i <= menu_ctrl_ptr->end_item_index; i++)
    {
        BOOLEAN    is_need_draw = TRUE;
        GUI_RECT_T cur_rect     = {0};
        GUI_RECT_T item_rect    = PopmenuGetSelectedBarRect( i , menu_ctrl_ptr );
        
        //如果做了内存回写加速
        if(!GUI_IsZeroRect( menu_ctrl_ptr->writeback_rect ) )
        {
            // 区域有交集的时候
            GUI_IntersectRect(&item_rect, item_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr));
            
            //if the the item in the writeback rect 
            if( GUI_IntersectRect( &cur_rect, item_rect, menu_ctrl_ptr->writeback_rect ) )
            {
                //判断边界item是否是完整的，如果不是完整的需要将拷贝的区域重置
                if( !GUI_RectIsCovered( menu_ctrl_ptr->writeback_rect, item_rect ) )
                {
                    PopmenuDisplayBg( menu_ctrl_ptr, cur_rect );
                }
                else
                {
                    is_need_draw = FALSE;
                }
            }
        }
        
        if ( is_need_draw )
        {
            //display one item
            PopmenuDisplayOneItem(TRUE,i,menu_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : get selected bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 根据offset获取区域
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetSelectedBarRect(
                                           uint16           item_index,     //in
                                           CTRLMENU_OBJ_T   *menu_ctrl_ptr  //in
                                           )
{
    uint16          item_height = 0;
    GUI_RECT_T      menu_rect = {0};
    GUI_RECT_T      select_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (PNULL != menu_ctrl_ptr)
    {
        //get pop-up menu rect
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
        item_height = menu_ctrl_ptr->pop_theme.item_height;
        
        //set select rect
#ifdef  MONOCHROME_LCD_SUPPORT  //zss modify begin
        if(GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style || GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style )
        {//@ alex modified in 2010.12.3
            select_rect.left   = menu_rect.left;
        }
        else
        {
            select_rect.left   = menu_rect.left + menu_ctrl_ptr->pop_theme.item_height;
        }
#else    
        select_rect.left   = menu_rect.left;
#endif //zss modify end
        
        select_rect.right  = menu_rect.right;
        select_rect.top    = (int16)(menu_ctrl_ptr->offset_y + item_height*item_index);
        select_rect.bottom = (int16)(select_rect.top + item_height - 1);
        
        if (PopmenuIsNeedScrollBar(menu_ctrl_ptr))
        {
            select_rect.right = (int16)(select_rect.right - PopmenuGetScrollBarWidth(menu_ctrl_ptr));
        }
        
        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &select_rect);
    }

    return (select_rect);
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuGetScrollBarWidth(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    if (PNULL != menu_ctrl_ptr)
    {
        return (uint16)(MMITHEME_GetScrollBarWidthByState(menu_ctrl_ptr->common_theme.is_used_slide) + 2);
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : display one item include icon,text and select bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayOneItem(
                                 BOOLEAN        is_display_sel_bar,
                                 uint16         item_index,
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr
                                 )
{
    if ((item_index == menu_ctrl_ptr->cur_item_index) 
        && (is_display_sel_bar)
        && MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state
        )
    {
#ifdef MMI_PDA_SUPPORT
        BOOLEAN is_gray = FALSE;

        CTRLMENU_GetItem(menu_ctrl_ptr, item_index, PNULL, PNULL, &is_gray);

        if ((menu_ctrl_ptr->is_item_pressed && !is_gray && menu_ctrl_ptr->is_tp_pressed)
            || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode() && menu_ctrl_ptr->is_active))
#endif
        {
            //display selected bar
            PopMenuDisplaySelectedBar(item_index,menu_ctrl_ptr);
        }
    }

    PopmenuDisplayOneItemNoHighlight(item_index, menu_ctrl_ptr);    
}

/*****************************************************************************/
//  Description : display selected bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopMenuDisplaySelectedBar(
                                     uint16         item_index,     //in
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr  //in
                                     )
{
    GUI_RECT_T      select_rect = {0};
    GUI_RECT_T      icon_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    GUI_RECT_T      menu_rect = {0};
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    if ( PNULL == menu_ctrl_ptr )
    {
        return;
    }
    
    //如果做了内存回写加速不需要画高亮
    if( !GUI_IsZeroRect( menu_ctrl_ptr->writeback_rect ) )
    {
        return;
    }

    // menu rect
    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    //get selected bar display rect
    select_rect = PopmenuGetSelectedBarRect(item_index,menu_ctrl_ptr);
    
    // 获取交集区域
    if (GUI_IntersectRect(&inter_rect, menu_rect, select_rect))
    {
        lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

#ifdef MONOCHROME_LCD_SUPPORT 
        LCD_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr, inter_rect, MMI_BLACK_COLOR);
#else
        //get image display rect
        icon_rect        = select_rect;
        icon_rect.left   = 0;
        icon_rect.top    = 0;
        icon_rect.bottom = (int16)(select_rect.bottom - select_rect.top);
        
        if (menu_ctrl_ptr->is_transparent)
        {
            PopmenuDisplayBg(menu_ctrl_ptr, inter_rect);
        }

        IMG_EnableTransparentColor(TRUE);
        
        //display selected bar
        GUIRES_DisplayImg(PNULL,
            &inter_rect,
            &icon_rect,
            menu_ctrl_ptr->win_handle,
            menu_ctrl_ptr->common_theme.selected_img,
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr);

        IMG_EnableTransparentColor(FALSE);
#endif
    }
}

/*****************************************************************************/
//  Description : display one item include icon, text, no select bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayOneItemNoHighlight(
                                            uint16          item_index,
                                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                            )
{
    if (PNULL != menu_ctrl_ptr)
    {
        //display item icon
        PopmenuDisplayItemIcon(item_index,menu_ctrl_ptr);

        //display item text
        PopmenuDisplayItemText(item_index,menu_ctrl_ptr);

#if defined(GUIPOPMENU_POP_POP_SUPPORT)
        //display tip icon
        PommenuDisplayTipIcon(item_index,menu_ctrl_ptr);
#endif

        //display separator
        if (CTRLMENU_GetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_DRAW_SPLIT_LINE))
        {
            PopmenuDisplaySeparator(item_index,menu_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : display item icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayItemIcon(
                                  uint16            item_index,
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  )
{
    GUI_RECT_T					icon_disp_rect = {0};
    GUI_RECT_T					inter_rect = {0};
    GUI_RECT_T					icon_rect = {0};
    MMI_IMAGE_ID_T				icon_id = 0;
    uint16						icon_width  = 0; 
    uint16						icon_height = 0;
    GUI_LCD_DEV_INFO			*lcd_dev_info_ptr = PNULL;
	CTRLMENU_NODE_T				*item_dyn_node_ptr = PNULL;
	CTRLMENU_SELECT_ICON_DATA_T item_dyn_node_icon = {0};

    //get icon image
	if (menu_ctrl_ptr->is_static)
	{
		icon_id = PopmenuGetItemIconId(item_index,menu_ctrl_ptr);
	}
	else
	{
		item_dyn_node_ptr = CTRLMENU_GetNodeByVisibleIndex(item_index, menu_ctrl_ptr->cur_parent_node_ptr);
		if ( DYNA_ITEM_ICON_ID == item_dyn_node_ptr->select_icon_type )
		{
			icon_id = PopmenuGetItemIconId(item_index,menu_ctrl_ptr);
		}
		else if( DYNA_ITEM_ICON_BUFFER == item_dyn_node_ptr->select_icon_type )
		{
			CTRLMENU_GetItemForIconBuffer(menu_ctrl_ptr, item_index, PNULL, &item_dyn_node_icon, PNULL);
		}
	}

	if (0 != icon_id)
	{    
		//get icon display rect
		icon_disp_rect = PopmenuGetItemIconDispRect(item_index,icon_id,menu_ctrl_ptr);

		// 区域有交集的时候
		if (GUI_IntersectRect(&inter_rect, icon_disp_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr)))
		{
			lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

			icon_width  = (uint16)(icon_disp_rect.right - icon_disp_rect.left + 1);
			icon_height = (uint16)(icon_disp_rect.bottom - icon_disp_rect.top + 1);

			icon_rect = PopmenuGetIconRect(icon_disp_rect, inter_rect, icon_width, icon_height);
    
			//set display point        
			IMG_EnableTransparentColor(TRUE);
    
			//display icon
			GUIRES_DisplayImg(PNULL,
				&inter_rect,
				&icon_rect,
				menu_ctrl_ptr->win_handle,
				icon_id,
				(const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr);
    
			IMG_EnableTransparentColor(FALSE);
		}
	}
	else if (PNULL != item_dyn_node_ptr && DYNA_ITEM_ICON_BUFFER == item_dyn_node_ptr->select_icon_type)
	{
		//get icon display rect
		icon_disp_rect = PopmenuGetItemIconBufferDispRect(item_index, menu_ctrl_ptr);
		// 区域有交集的时候
		if (GUI_IntersectRect(&inter_rect, icon_disp_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr)))
		{
			GUI_POINT_T img_point = {0};
			GUIIMG_BITMAP_T img_map_info = {0};
			GUIIMG_DISPLAYBMP_PARAM_T display_param = {0};
			
			lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);
			icon_width  = (uint16)(icon_disp_rect.right - icon_disp_rect.left + 1);
			icon_height = (uint16)(icon_disp_rect.bottom - icon_disp_rect.top + 1);
			
			img_point.x = inter_rect.left;
			img_point.y = inter_rect.top;
			
			img_map_info.bit_ptr    = item_dyn_node_icon.buffer.data;
			img_map_info.img_width  = item_dyn_node_icon.buffer.width;
			img_map_info.img_height = item_dyn_node_icon.buffer.height;
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

}

/*****************************************************************************/
//  Description : get icon image id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T PopmenuGetItemIconId(
                                          uint16            item_index,
                                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                          )
{
    MMI_IMAGE_ID_T          icon_id = 0;
    GUIMENU_POP_SEL_INFO_T  item_info = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return icon_id;
    }

    //set item info
    CTRLMENU_GetPopSelInfo(menu_ctrl_ptr, item_index, &item_info);

    if (GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style)
    {
        if(CTRLMENU_GetPopItemStatus(&item_info))
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
        if(CTRLMENU_GetPopItemStatus(&item_info))
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

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
	// MORE不用显示图标
	if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
	{
		icon_id = 0;
	}
#endif

    return (icon_id);
}

/*****************************************************************************/
//  Description : get icon display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetItemIconDispRect(
                                            uint16          item_index,
                                            MMI_IMAGE_ID_T  icon_id,
                                            CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                            )
{
    uint16          icon_width = 0;
    uint16          icon_height = 0;
    uint16          item_height = 0;
    GUI_RECT_T      menu_rect = {0};
    GUI_RECT_T      icon_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    
    if (PNULL != menu_ctrl_ptr)
    {
        //get pop-up menu rect
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

        item_height = menu_ctrl_ptr->pop_theme.item_height;
        
        //get icon width
        GUIRES_GetImgWidthHeight(&icon_width,
            &icon_height,
            icon_id,
            menu_ctrl_ptr->win_handle);

        //set icon rect
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        if ((GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style)
            || (GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style))
        {
            icon_rect.right  = (int16)(menu_rect.right - menu_ctrl_ptr->common_theme.gap_width);
            icon_rect.left   = (int16)(icon_rect.right - icon_width + 1);
        }
        else
#endif
        {
            icon_rect.left   = (int16)(menu_rect.left + menu_ctrl_ptr->common_theme.gap_width);
            icon_rect.right  = (int16)(icon_rect.left + icon_width - 1);
        }

        icon_rect.top    = (int16)(menu_ctrl_ptr->offset_y + item_height*item_index + (item_height - icon_height)/2);
        icon_rect.bottom = (int16)(icon_rect.top + icon_height - 1);
        
        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &icon_rect);
    }

    return (icon_rect);
}

/*****************************************************************************/
//  Description : get icon buffer display rect
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note: 只适用于动态菜单，且icon为buffer的情况
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetItemIconBufferDispRect(
                                            uint16							item_index,
                                            CTRLMENU_OBJ_T					*menu_ctrl_ptr
                                            )
{
    uint16          icon_width = 0;
    uint16          icon_height = 0;
    uint16          item_height = 0;
    GUI_RECT_T      menu_rect = {0};
    GUI_RECT_T      icon_rect = {0};
	CTRLMENU_NODE_T *item_node_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    
    if (PNULL != menu_ctrl_ptr && !menu_ctrl_ptr->is_static)
    {
        //get pop-up menu rect
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
		
        item_height = menu_ctrl_ptr->pop_theme.item_height;
        item_node_ptr = CTRLMENU_GetNodeByVisibleIndex(item_index, menu_ctrl_ptr->cur_parent_node_ptr);
        //get icon width
		icon_height = item_node_ptr->select_icon_data.buffer.height;
		icon_width = item_node_ptr->select_icon_data.buffer.width;
		
        //set icon rect
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        if ((GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style)
            || (GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style))
        {
            icon_rect.right  = (int16)(menu_rect.right - menu_ctrl_ptr->common_theme.gap_width);
            icon_rect.left   = (int16)(icon_rect.right - icon_width + 1);
        }
        else
#endif
        {
            icon_rect.left   = (int16)(menu_rect.left + menu_ctrl_ptr->common_theme.gap_width);
            icon_rect.right  = (int16)(icon_rect.left + icon_width - 1);
        }
		
        icon_rect.top    = (int16)(menu_ctrl_ptr->offset_y + item_height*item_index + (item_height - icon_height)/2);
        icon_rect.bottom = (int16)(icon_rect.top + icon_height - 1);
        
        GUI_CheckRectDirection(&base_ctrl_ptr->rect, &icon_rect);
    }
	
    return (icon_rect);
}

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetIconRect(
                                    GUI_RECT_T  disp_rect, 
                                    GUI_RECT_T  inter_rect, 
                                    uint16      icon_width, 
                                    uint16      icon_height
                                    )
{
    GUI_RECT_T icon_rect = {0};

    icon_width  = (uint16)(disp_rect.right - disp_rect.left + 1);
    icon_height = (uint16)(disp_rect.bottom - disp_rect.top + 1);
    
    icon_rect.right  = (int16)(disp_rect.right - disp_rect.left);
    icon_rect.bottom = (int16)(disp_rect.bottom - disp_rect.top);
    
    if (disp_rect.top < inter_rect.top)
    {
        icon_rect.top    = (int16)(inter_rect.top - disp_rect.top);
        icon_rect.bottom = (int16)(icon_height - 1);
    }
    else if (disp_rect.bottom > inter_rect.bottom)
    {
        icon_rect.top    = 0;
        icon_rect.bottom = (int16)(icon_height - (disp_rect.bottom - inter_rect.bottom));
    }
    else
    {
        icon_rect.top    = 0;
        icon_rect.bottom = (int16)(icon_height - 1);
    }

    return icon_rect;
}

/*****************************************************************************/
//  Description : display item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplayItemText(
                                  uint16            item_index,
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                  )
{
    BOOLEAN         is_display = FALSE;
    BOOLEAN         is_grayed = FALSE;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      inter_rect = {0};
    GUI_ALIGN_E     text_align = ALIGN_LVMIDDLE;
    GUI_COLOR_T     text_color = 0;
    MMI_STRING_T    text_str = {0};
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

    //get text display rect
    text_rect = PopmenuGetItemTextRect(item_index,menu_ctrl_ptr);

    // 判断是否有交集
    if (!GUI_IntersectRect(&inter_rect, text_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr)))
    {
        return;
    }

    //get text and color
    CTRLMENU_GetItem(menu_ctrl_ptr, item_index, &text_str, PNULL, &is_grayed);
    if (is_grayed)
    {
        text_color = MMITHEME_GetGrayColor();
    }
    else
    {
#ifdef MONOCHROME_LCD_SUPPORT 
        if(item_index == menu_ctrl_ptr->cur_item_index)
        {
            text_color = 0xffff;
        }
        else
        {
            text_color = CTRLMENU_GetFontColor(menu_ctrl_ptr);
        }
#else
        text_color = CTRLMENU_GetFontColor(menu_ctrl_ptr);
#endif
    }

#if !defined(MMI_PDA_SUPPORT)
    //display text
    if (item_index == menu_ctrl_ptr->cur_item_index)
    {
        BOOLEAN         is_scroll_text = FALSE;
        uint16          text_pixel_len = 0;

        //get item text len
        str_style.font = menu_ctrl_ptr->pop_theme.cur_item_font;

        text_pixel_len = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
                            text_str.wstr_len,
                            str_style.font,
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
                str_style.font_color = text_color;
                GUISTR_DrawTextToLCDByOffset( 
                    (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
                    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
                    &inter_rect,
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
#endif
    {
        str_state |= GUISTR_STATE_ELLIPSIS;
    }
    
    if (!is_display)        /*lint !e774*/
    {
        str_style.align = text_align;

        str_style.font = (item_index == menu_ctrl_ptr->cur_item_index) 
            ? menu_ctrl_ptr->pop_theme.cur_item_font : menu_ctrl_ptr->pop_theme.item_font;
        
        str_style.font_color = text_color;
        //display text
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr,
            (const GUI_RECT_T*)&text_rect, 
            (const GUI_RECT_T*)&inter_rect, 
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
LOCAL GUI_RECT_T PopmenuGetItemTextRect(
                                        uint16          item_index,
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        )
{
    uint16          			item_height = 0;
    uint16          			icon_gap_width = 0;
    uint16          			tip_icon_width = 0;
    uint16          			prg_width = 0;
    GUI_RECT_T      			menu_rect = {0};
    GUI_RECT_T      			text_rect = {0};
    GUI_RECT_T      			icon_rect = {0};
    MMI_IMAGE_ID_T  			icon_id = 0;
	CTRLMENU_NODE_T 			*item_node_ptr = PNULL;
	CTRLMENU_SELECT_ICON_DATA_T icon_buffer = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return text_rect;
    }

    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        //get pop-up menu rect
        menu_rect   = PopmenuAdjustMenuRect(menu_ctrl_ptr);
        item_height = menu_ctrl_ptr->pop_theme.item_height;

        //get tip icon width
        GUIRES_GetImgWidthHeight(&tip_icon_width,
            PNULL,
            menu_ctrl_ptr->pop_theme.left_img,
            menu_ctrl_ptr->win_handle);

        //get icon image id
		if (menu_ctrl_ptr->is_static)
		{
			icon_id = PopmenuGetItemIconId(item_index,menu_ctrl_ptr);
		}
		else
		{
			item_node_ptr = CTRLMENU_GetNodeByVisibleIndex(item_index, menu_ctrl_ptr->cur_parent_node_ptr);
			if ( DYNA_ITEM_ICON_ID == item_node_ptr->select_icon_type)
			{        
				icon_id = PopmenuGetItemIconId(item_index,menu_ctrl_ptr);
			}
			else
			{
				CTRLMENU_GetItemForIconBuffer(menu_ctrl_ptr, item_index, PNULL, &icon_buffer, PNULL);
			}
		}

        if (0 != icon_id)
        {
            //get icon display rect
            icon_rect = PopmenuGetItemIconDispRect(item_index,icon_id,menu_ctrl_ptr);
            icon_gap_width = (uint16)(icon_rect.right - icon_rect.left + 1 + menu_ctrl_ptr->common_theme.gap_width);
        }
        else if( PNULL != item_node_ptr && DYNA_ITEM_ICON_BUFFER == item_node_ptr->select_icon_type)
        {
            //get icon display rect
            icon_rect = PopmenuGetItemIconBufferDispRect(item_index,menu_ctrl_ptr);
            icon_gap_width = (uint16)(icon_rect.right - icon_rect.left + 1 + menu_ctrl_ptr->common_theme.gap_width);
        }
		else
		{
			icon_gap_width = 0;
		}
        break;

    default:
        SCI_PASSERT(FALSE,("GetItemTextRect:menu style %d is error!",menu_ctrl_ptr->cur_style)); /*assert verified*/
        break;
    }

    //set icon rect
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    if ((GUIMENU_STYLE_POPUP_RADIO == menu_ctrl_ptr->cur_style)
        || (GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style))
    {
        text_rect.left   = (int16)(menu_rect.left + menu_ctrl_ptr->common_theme.gap_width);
        text_rect.right  = (int16)(menu_rect.right - icon_gap_width);
    }
    else
#endif
    {
        //get progress width
        if (PopmenuIsNeedScrollBar(menu_ctrl_ptr))
        {
            prg_width = PopmenuGetScrollBarWidth(menu_ctrl_ptr);
        }

        text_rect.left   = (int16)(menu_rect.left + menu_ctrl_ptr->common_theme.gap_width + icon_gap_width);
        text_rect.right  = (int16)(menu_rect.right - prg_width - tip_icon_width);
    }

    text_rect.top    = (int16)(menu_ctrl_ptr->offset_y + item_height*item_index);
    text_rect.bottom = (int16)(text_rect.top + item_height - 1);

    GUI_CheckRectDirection(&base_ctrl_ptr->rect, &text_rect);

    return (text_rect);
}

#if defined(GUIPOPMENU_POP_POP_SUPPORT)
/*****************************************************************************/
//  Description : display tip icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PommenuDisplayTipIcon(
                                 uint16         item_index,
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr
                                 )
{
    GUI_RECT_T          icon_disp_rect = {0};
    GUI_RECT_T          inter_rect = {0};
    GUI_RECT_T          icon_rect = {0};
    MMI_IMAGE_ID_T      icon_id = 0;
    BOOLEAN             is_grayed = FALSE;
    uint16              icon_width = 0;
    uint16              icon_height = 0;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    if (CTRLMENU_IsExistChildMenu(menu_ctrl_ptr, item_index, &is_grayed))
    {
        //get tip icon display rect
        icon_disp_rect = GetItemTipIconDispRect(item_index,menu_ctrl_ptr);
        
        // 区域有交集的时候
        if (GUI_IntersectRect(&inter_rect, icon_disp_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr)))
        {           
            lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

            //get tip icon image id
            icon_id = GetItemTipIconId(menu_ctrl_ptr, is_grayed);

            icon_width  = (uint16)(icon_disp_rect.right - icon_disp_rect.left + 1);
            icon_height = (uint16)(icon_disp_rect.bottom - icon_disp_rect.top + 1);

            icon_rect = PopmenuGetIconRect(icon_disp_rect, inter_rect, icon_width, icon_height);

            //display icon
            IMG_EnableTransparentColor(TRUE);

            GUIRES_DisplayImg(PNULL,
                &inter_rect,
                &icon_rect,
                menu_ctrl_ptr->win_handle,
                icon_id,
                (const GUI_LCD_DEV_INFO*)lcd_dev_info_ptr);
            
            IMG_EnableTransparentColor(FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : get tip icon display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetItemTipIconDispRect(
                                        uint16          item_index,
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        )
{
    uint16          img_width = 0;
    uint16          img_height = 0;
    uint16          prg_width = 0;
    uint16          item_height = 0;
    GUI_RECT_T      menu_rect = {0};
    GUI_RECT_T      icon_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return icon_rect;
    }
	item_height = menu_ctrl_ptr->pop_theme.item_height;
	base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    //get pop-up menu rect
    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    //get icon width and height
    GUIRES_GetImgWidthHeight(&img_width,
        &img_height,
        menu_ctrl_ptr->pop_theme.left_img,
        menu_ctrl_ptr->win_handle);

    //get progress width
    if (PopmenuIsNeedScrollBar(menu_ctrl_ptr))
    {
        prg_width = PopmenuGetScrollBarWidth(menu_ctrl_ptr);
    }

    //set icon rect
    icon_rect.right = (int16)(menu_rect.right - prg_width + 1);
    icon_rect.left  = (int16)(icon_rect.right - img_width + 1);

    icon_rect.top    = (int16)(menu_ctrl_ptr->offset_y + item_height*item_index + (item_height-img_height)/2);
	icon_rect.bottom = (int16)(icon_rect.top + item_height - 1);

    GUI_CheckRectDirection(&base_ctrl_ptr->rect, &icon_rect);

    return (icon_rect);
}

/*****************************************************************************/
//  Description : get tip icon image id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetItemTipIconId(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr, // [in]
                                      BOOLEAN           is_grayed       // [in]
                                      )
{
    MMI_IMAGE_ID_T  icon_id = 0;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return icon_id;
    }

    if (is_grayed)
    {
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            icon_id = menu_ctrl_ptr->pop_theme.left_gray_img;
        }
        else
        {
            icon_id = menu_ctrl_ptr->pop_theme.right_gray_img;
        }
    }
    else
    {
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            icon_id = menu_ctrl_ptr->pop_theme.left_img;
        }
        else
        {
            icon_id = menu_ctrl_ptr->pop_theme.right_img;
        }
    }

    return (icon_id);
}
#endif

/*****************************************************************************/
//  Description : display separator
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PopmenuDisplaySeparator(
                                   uint16           item_index,
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    GUI_RECT_T  separator_rect = {0};
    GUI_RECT_T  inter_rect = {0};
    GUI_RECT_T  menu_rect = {0};
    GUI_LCD_DEV_INFO *lcd_dev_info_ptr = PNULL;

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    separator_rect = PopmenuGetSeparatorRect(item_index, menu_ctrl_ptr);

    if (GUI_IntersectRect(&inter_rect, separator_rect, menu_rect))
    {
        lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

        LCD_FillRect(
                lcd_dev_info_ptr, inter_rect,
                menu_ctrl_ptr->common_theme.split_line_color);
    }
}

/*****************************************************************************/
//  Description : get item separator display rect
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PopmenuGetSeparatorRect(
                                         uint16         item_index,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         )
{
    GUI_RECT_T  separator_rect = {0};
    GUI_RECT_T  menu_rect = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return separator_rect;
    }

    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

        //get pop-up menu rect
        separator_rect.left   = (int16)(menu_rect.left + 1);
        separator_rect.right  = (int16)(menu_rect.right - 1);
        separator_rect.top    = (int16)(menu_ctrl_ptr->offset_y + menu_ctrl_ptr->pop_theme.item_height*(1+item_index) - 1);
        separator_rect.bottom = separator_rect.top;
        break;

    default:
        SCI_PASSERT(FALSE,("PopmenuGetSeparatorRect:menu style %d is error!",menu_ctrl_ptr->cur_style)); /*assert verified*/
        break;
    }

    return separator_rect;
}

/*****************************************************************************/
//  Description : display menu scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 该接口只有二级菜单和popmenu使用
/*****************************************************************************/
LOCAL void PopmenuDisplayScrollGroove(
                                      BOOLEAN           is_update,
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    uint32                  prg_total_count = 0;
    uint32                  prg_count_in_page = 0;
    uint32                  prg_cur_index = 0;
    uint32                  prg_first_index = 0;
    GUI_RECT_T              prg_rect = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    MMI_CONTROL_CREATE_T    ctrl_creat = {0};
    GUIPRGBOX_INIT_DATA_T   init_data = {0};

    //is need scroll bar
    if (PopmenuIsNeedScrollBar(menu_ctrl_ptr))
    {
        //get progress bar rect
        prg_rect = PopmenuGetScrollBarRect(menu_ctrl_ptr);

        //display bg
        PopmenuDisplayBg(menu_ctrl_ptr, prg_rect);

        //init
        if (PNULL == menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            //init data
            init_data.lcd_id = GUI_MAIN_LCD_ID;
            init_data.both_rect.h_rect = init_data.both_rect.v_rect   = prg_rect;
            init_data.style  = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
#ifdef GUIF_PRGBOX_AUTOHIDE
            init_data.is_forbid_paint = TRUE;
#endif
            ctrl_creat.guid               = SPRD_GUI_PRGBOX_ID;
            ctrl_creat.parent_win_handle  = menu_ctrl_ptr->win_handle;
			ctrl_creat.parent_ctrl_handle = base_ctrl_ptr->handle;
            ctrl_creat.init_data_ptr      = &init_data;
            
            //creat progress bar control
            menu_ctrl_ptr->prgbox_ctrl_ptr = MMK_CreateControl(&ctrl_creat);

            is_update = TRUE;
        }

        GUIPRGBOX_ResizeRectByPtr(menu_ctrl_ptr->prgbox_ctrl_ptr, prg_rect);

        if (CTRLMENU_GetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_PRG_PIXEL_SCROLL))
        {
            prg_total_count = menu_ctrl_ptr->item_total_num * GetItemLineHeight(menu_ctrl_ptr);
            prg_count_in_page = prg_rect.bottom - prg_rect.top + 1;

            prg_cur_index = menu_ctrl_ptr->cur_item_index * GetItemLineHeight(menu_ctrl_ptr);
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
            prg_total_count = (uint32)menu_ctrl_ptr->item_total_num;
            prg_count_in_page = (uint32)PopmenuGetLineNumPage(menu_ctrl_ptr);
            prg_first_index = (uint32)menu_ctrl_ptr->first_item_index;
            prg_cur_index = (uint32)menu_ctrl_ptr->cur_item_index;
        }
        
        //set progress param
        GUIPRGBOX_SetParamByPtr(
            menu_ctrl_ptr->prgbox_ctrl_ptr,
            prg_total_count,
            prg_count_in_page);

        //set current position
        GUIPRGBOX_SetPosByPtr(
            is_update,
            prg_cur_index,
			prg_first_index,
            menu_ctrl_ptr->prgbox_ctrl_ptr);
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
LOCAL GUI_RECT_T PopmenuGetScrollBarRect(
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         )
{
    GUI_RECT_T      menu_rect = {0};
    GUI_RECT_T      prg_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
    {
        //get pop-up menu rect
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
        //set prg rect
        prg_rect       = menu_rect;
        prg_rect.left  = (int16)(menu_rect.right - MMITHEME_GetScrollBarWidthByState(menu_ctrl_ptr->common_theme.is_used_slide));
        prg_rect.right = (int16)(prg_rect.left + MMITHEME_GetScrollBarWidthByState(menu_ctrl_ptr->common_theme.is_used_slide) - 1);
    }

    GUI_CheckRectDirection(&base_ctrl_ptr->rect, &prg_rect);

    return (prg_rect);
}

/*****************************************************************************/
//  Description : calculate pop-up menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_CalcPopupRect(
                                   uint16           rect_top,       //only for sub pop-up menu
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style)
    {
#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
        CalculateMoreMenuRect(menu_ctrl_ptr);
#else
        CalculatePopupMenuRect(menu_ctrl_ptr, rect_top);
#endif
    }
    else
    {
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
        CalculateContextMenuRect(menu_ctrl_ptr);
#else
        CalculatePopupMenuRect(menu_ctrl_ptr, rect_top);
#endif
    }
}

#if defined(GUIPOPMENU_POP_POP_SUPPORT)
/*****************************************************************************/
//  Description : update previous pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:  当前是pop-up menu,lose focus后get focus需要刷新之前所有的pop-up menu
//         当前是pop-up menu,cancel后需要根据Rect判断是否刷新之前的pop-up menu
/*****************************************************************************/
PUBLIC void CTRLMENU_UpdatePrevPop(
                                   uint16           start_level,
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    int16                   offset_y = 0;
    uint16                  i = 0;
    uint16                  menu_level = 0;
    uint16                  first_item_index = 0;
    uint16                  cur_item_index = 0;
    GUI_RECT_T              menu_rect = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    CTRLMENU_NODE_T         *cur_parent_node_ptr = PNULL;
    MMI_MENU_GROUP_ID_T     cur_group_id = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if ((CTRLMENU_IsPopMenu(menu_ctrl_ptr)) &&
        (start_level < menu_ctrl_ptr->menu_level))
    {
        //save current menu info
        menu_level          = menu_ctrl_ptr->menu_level;
        cur_group_id        = menu_ctrl_ptr->cur_group_id;
        cur_item_index      = menu_ctrl_ptr->cur_item_index;
        first_item_index    = menu_ctrl_ptr->first_item_index;
        menu_rect           = base_ctrl_ptr->rect;
        offset_y            = menu_ctrl_ptr->offset_y;
        cur_parent_node_ptr = menu_ctrl_ptr->cur_parent_node_ptr;

        //need update previous all pop-up menu
        for (i=start_level; i<menu_level; i++)
        {
            //set menu info
            CTRLMENU_SetInfo(i,
                menu_ctrl_ptr->menu_stack[i].first_item_index,
                menu_ctrl_ptr->menu_stack[i].cur_item_index,
                menu_ctrl_ptr->menu_stack[i].rect,
                menu_ctrl_ptr->menu_stack[i].offset_y,
                menu_ctrl_ptr->menu_stack[i].group_id,
                menu_ctrl_ptr->menu_stack[i].parent_node_ptr,
                menu_ctrl_ptr);

            menu_ctrl_ptr->is_first_disp = FALSE;

            //display previous menu,may not be a pop menu
			//CTRLMENU_DrawPop(menu_ctrl_ptr);
			CTRLMENU_Draw(menu_ctrl_ptr);

            CTRLMENU_ResetInfo(menu_ctrl_ptr);
        }

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
}
#endif

/*****************************************************************************/
//  Description : handle OK key
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
//    when in PDA project:
//    |-----------------------------------------------------------------|
//    |      event     |    Popup(More item)      |   Popup_Auto        |
//    |-----------------------------------------------------------------|
//    |OK(menu key)    |    close                 |   none              |
//    |-----------------------------------------------------------------|
//    |Web             |    has active: OK        |   the same as Popup |
//    |                |    not active: none      |                     |
//    |-----------------------------------------------------------------|
//    |back            |    back to Options       |   close             |
//    |-----------------------------------------------------------------|
//    |tp              |    in client: OK         |   in client: OK     |
//    |                |    not: close            |   not : none        |
//    |-----------------------------------------------------------------|
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleOk(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    MMI_MESSAGE_ID_E    msg_id
                                    )
{
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    BOOLEAN is_handled = TRUE;
#else
    BOOLEAN is_handled = FALSE;
#endif

#ifdef MMI_PDA_SUPPORT
    if (MSG_APP_OK == msg_id || MSG_APP_WEB == msg_id)
    {
        // 模式切换时，改变焦点
        if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
            && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
        {
            CTRLMENU_DrawPop(menu_ctrl_ptr);
            return TRUE;
        }
        else
        {
            is_handled = FALSE;
        }
    }
#endif

    if (MSG_APP_WEB == msg_id
        && GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style)
    {
        BOOLEAN                 is_checked = FALSE;
        GUIMENU_POP_SEL_INFO_T  item_info = {0};

        CTRLMENU_GetPopSelInfo(
                menu_ctrl_ptr,
                menu_ctrl_ptr->cur_item_index,
                &item_info);

        is_checked = CTRLMENU_GetPopItemStatus(&item_info);

        is_checked = (BOOLEAN)(is_checked ? FALSE : TRUE);

        CTRLMENU_SetPopItemStatus(is_checked, &item_info);

        PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);

        is_handled = TRUE;
    }
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    else if (MSG_TP_PRESS_UP == msg_id)
    {
        is_handled = FALSE;
    }
#endif

    return is_handled;
}

/*****************************************************************************/
//  Description : handle cancel key
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
//    when in PDA project:
//    |-----------------------------------------------------------------|
//    |      event     |    Popup(More item)      |   Popup_Auto        |
//    |-----------------------------------------------------------------|
//    |OK(menu key)    |    close                 |   none              |
//    |-----------------------------------------------------------------|
//    |Web             |    has active: OK        |   the same as Popup |
//    |                |    not active: none      |                     |
//    |-----------------------------------------------------------------|
//    |back            |    back to Options       |   close             |
//    |-----------------------------------------------------------------|
//    |tp              |    in client: OK         |   in client: OK     |
//    |                |    not: close            |   not : none        |
//    |-----------------------------------------------------------------|
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleCancel(
                                        CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                        MMI_MESSAGE_ID_E    msg_id
                                        )
{
    BOOLEAN is_handled = FALSE;

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_POPUP:
        switch (msg_id)
        {
        case MSG_APP_CANCEL:
            is_handled = CTRLMENU_SwitchOptions(menu_ctrl_ptr, TRUE);
            break;

        default:
            is_handled = FALSE;
            break;
        }
        break;

    default:
        break;
    }
#endif

    return is_handled;
}

/*****************************************************************************/
//  Description : destroy popmenu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_DestroyPop(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                )
{
    if (PNULL != menu_ctrl_ptr->options_item_info_ptr)
    {
        SCI_FREE(menu_ctrl_ptr->options_item_info_ptr);
    }

    if (PNULL != menu_ctrl_ptr->item_len_array_ptr)
    {
        SCI_FREE(menu_ctrl_ptr->item_len_array_ptr);
    }

    if (PNULL != menu_ctrl_ptr->num_in_line_ptr)
    {
        SCI_FREE(menu_ctrl_ptr->num_in_line_ptr);
    }
}

/*****************************************************************************/
//  Description : get menu current item top value
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLMENU_GetCurItemTop(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     )
{
    GUI_RECT_T  selected_rect = {0};

    //get selected bar rect
    selected_rect = PopmenuGetSelectedBarRect(menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);

    return ((uint16)selected_rect.top);
}

/*****************************************************************************/
//  Description : handle pop menu up/down key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_PopHandleUpDown(
                                             CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                             MMI_MESSAGE_ID_E   msg_id
                                             )
{
    uint16          prev_item_index = 0;
    uint16          prev_first_item_index = 0;
    uint16          line_num = 0;
    int16           offset_y = 0;
    BOOLEAN         is_draw_all = FALSE;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

#ifdef MMI_PDA_SUPPORT
    // 模式切换时，改变焦点
    if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
        && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        CTRLMENU_DrawPop(menu_ctrl_ptr);
        return result;
    }
#endif

    if (ResetPopmenuDisplayItem(menu_ctrl_ptr, &offset_y)) // 当offset y改变了，需要重新显示，然后直接返回
    {
        if (offset_y != 0)
        {
            menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);

            PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
            
            // 显示         
            PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);
        }

        return result;
    }

    if (1 < menu_ctrl_ptr->item_total_num)
    {
        GUI_RECT_T  pre_cur_item_rect = {0};
        
        //set previous item index
        prev_item_index = menu_ctrl_ptr->cur_item_index;
        prev_first_item_index = menu_ctrl_ptr->first_item_index;

        pre_cur_item_rect = PopmenuGetSelectedBarRect(prev_item_index, menu_ctrl_ptr);
        
        //get line number
        line_num = PopmenuGetLineNumPage(menu_ctrl_ptr);

        //set first and current item index
        if (CTRLMENU_SetCurAndFirstItemIndex(menu_ctrl_ptr, line_num, msg_id))
        {
            //stop item text timer
            CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

            // 重新设置offset Y
            if (ResetPopmenuDisplayItem(menu_ctrl_ptr, &offset_y))
            {
                menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);

                is_draw_all = TRUE;

                PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
            }

            //update menu display
            PopmenuUpdateMenuDisplay(TRUE,
                is_draw_all,
                prev_first_item_index,
                prev_item_index,
                &pre_cur_item_rect,
                menu_ctrl_ptr->item_total_num,
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
//  Description : reset menu display item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetPopmenuDisplayItem(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr,
                                      int16             *offset_y_ptr
                                      )
{
    int16       offset_y = 0;
    BOOLEAN     reslut = TRUE;
    GUI_RECT_T  cur_item_rect = {0};
    GUI_RECT_T  inter_item_rect = {0};
    GUI_RECT_T  menu_rect = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != offset_y_ptr); /*assert verified*/
    if ((PNULL == menu_ctrl_ptr) || (PNULL == offset_y_ptr))
    {
        return FALSE;
    }

    cur_item_rect = PopmenuGetSelectedBarRect(menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr);

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    if (GUI_IntersectRect(&inter_item_rect, cur_item_rect, menu_rect)) // 有交集
    {
        // 不被完全覆盖
        if (!GUI_RectIsCovered(menu_rect, cur_item_rect))
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
        if (cur_item_rect.top < menu_rect.top) // 在菜单区域之上
        {
            offset_y = (int16)(menu_rect.top - cur_item_rect.top); // 向下移动的位移
        }
        else if (cur_item_rect.bottom > menu_rect.bottom)
        {
            offset_y = (int16)(menu_rect.bottom - cur_item_rect.bottom);
        }
        else
        {
            reslut = FALSE;
        }
    }

    *offset_y_ptr = offset_y;

    return reslut;
}

/*****************************************************************************/
//  Description : update menu display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuUpdateMenuDisplay(
                                    BOOLEAN         is_support_3d,
                                    BOOLEAN         is_draw_all,
                                    uint16          prev_first_item_index,
                                    uint16          prev_item_index,
                                    GUI_RECT_T      *pre_cur_item_rect_ptr, // 为了3D翻转而是用的区域
                                    uint16          line_num,
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                    )
{
    uint16      line_index = 0;
    GUI_RECT_T  bg_rect = {0};
    GUI_RECT_T  menu_rect = {0};
#ifdef MMI_PDA_SUPPORT
    BOOLEAN is_gray = FALSE;
#endif

    //SCI_ASSERT(PNULL != pre_cur_item_rect_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    menu_ctrl_ptr->item_str_index = 0;

    //从头到末尾或从末尾到头
    if ((menu_ctrl_ptr->item_total_num > line_num)
        && ((0 == prev_item_index) && ((menu_ctrl_ptr->item_total_num - 1) == menu_ctrl_ptr->cur_item_index)
        || (((menu_ctrl_ptr->item_total_num - 1) == prev_item_index) && (0 == menu_ctrl_ptr->cur_item_index))))
    {
        PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);     
    }
    else
    {
        menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

        //get previous item line index
        line_index = PopmenuCalcLineIndexPage(
            prev_item_index, 
            prev_first_item_index, 
            menu_ctrl_ptr->item_total_num
            );

        //previous is in current page
        if (line_index < line_num)
        {
            //get previous item selected bar rect
            bg_rect = *pre_cur_item_rect_ptr;

            if (GUI_RectIsCovered(menu_rect, bg_rect) && is_support_3d)
            {
#ifdef UI_P3D_SUPPORT
                PopmenuSavePrevItem(prev_item_index,
                    menu_ctrl_ptr->cur_item_index,
                    bg_rect,
                    menu_ctrl_ptr->win_handle);
#endif
            }

            //翻页
            if (is_draw_all
                || prev_first_item_index != menu_ctrl_ptr->first_item_index)
            {
                PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);
            }
            else
            {
                //reset previous item bg
                PopmenuDisplayBg(menu_ctrl_ptr, bg_rect);

                //reset previous item display
                PopmenuDisplayOneItem(TRUE,prev_item_index,menu_ctrl_ptr);

                //display scroll groove
                PopmenuDisplayScrollGroove(TRUE,menu_ctrl_ptr);
	            
	            //set softkey param
	            CTRLMENU_SetSoftkey(menu_ctrl_ptr);
            }

            if (GUI_RectIsCovered(menu_rect, bg_rect) && is_support_3d)
            {
#ifdef UI_P3D_SUPPORT
                //save previous item bg
                PopmenuSavePrevItemBg(bg_rect);

                //save next item
                PopmenuSaveNextItem(menu_ctrl_ptr);
#endif
            }
        }

#ifdef MMI_PDA_SUPPORT
        // 由于DISABLE时不画焦点，所以这里也不用画了
        CTRLMENU_GetItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, PNULL, PNULL, &is_gray);
        if (!is_gray)
#endif
        {
            //display active item
            PopmenuDisplayOneItem(TRUE,menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : calculator line index in one page
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint16 PopmenuCalcLineIndexPage(
                                      uint16    item_index,
                                      uint16    first_item_index,
                                      uint16    item_total_num
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

#ifdef UI_P3D_SUPPORT
/*****************************************************************************/
//  Description : save previous item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuSavePrevItem(
                               uint16          prev_item_index,
                               uint16          cur_item_index,
                               GUI_RECT_T      item_rect,
                               MMI_HANDLE_T    win_handle
                               )
{
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
    uint32      buf_size = 0;
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
#endif

}

/*****************************************************************************/
//  Description : save previous item bg for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuSavePrevItemBg(
                                 GUI_RECT_T    item_rect
                                 )
{
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT

    if (MMITHEME_IsNeed3DList())
    {
        MMITHEME_Save3DListBuf(GUILIST_PREV_BG_BUF,item_rect);
    }
#endif
}

/*****************************************************************************/
//  Description : save next item for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuSaveNextItem(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               )
{
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
    GUI_RECT_T  item_rect = {0};
    if (MMITHEME_IsNeed3DList())
    {
        //display active item
        PopmenuDisplayOneItem(TRUE,menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);
        
        //get current item rect
        item_rect = PopmenuGetSelectedBarRect(menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);
        
        //save icon
        MMITHEME_Save3DListBuf(GUILIST_NEXT_BUF,item_rect);
        
        //reset item bg
        PopmenuDisplayBg(menu_ctrl_ptr,item_rect);
        
        //reset item display
        PopmenuDisplayOneItem(FALSE,menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);

        MMITHEME_Start3DListAnim();
    }
#endif
}
#endif

/*****************************************************************************/
//  Description : handle pop menu left key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleLeft(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_grayed = FALSE;

#ifdef MMI_PDA_SUPPORT
    // 模式切换时，改变焦点
    // 虽然这里不应该显示焦点，但是由于按左右键后会切到KEY模式，再按上下键后，
    // 焦点会有问题，所以这里就显示一下
    if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
        && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        CTRLMENU_DrawPop(menu_ctrl_ptr);
        return FALSE;
    }
#endif

    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        if (CTRLMENU_IsExistChildMenu(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &is_grayed))
        {
            //enter the next menu
            if (!is_grayed)
            {
                result = TRUE;
            }
        }
    }
    else
    {
        if (0 < menu_ctrl_ptr->menu_level)
        {
            //return the parent menu
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle pop menu right key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleRight(
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_grayed = FALSE;

#ifdef MMI_PDA_SUPPORT
    // 模式切换时，改变焦点
    // 虽然这里不应该显示焦点，但是由于按左右键后会切到KEY模式，再按上下键后，
    // 焦点会有问题，所以这里就显示一下
    if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
        && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
    {
        CTRLMENU_DrawPop(menu_ctrl_ptr);
        return FALSE;
    }
#endif

    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        if (0 < menu_ctrl_ptr->menu_level)
        {
            //return the parent menu
            result = TRUE;
        }
    }
    else
    {
        if (CTRLMENU_IsExistChildMenu(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &is_grayed))
        {
            //enter the next menu
            if (!is_grayed)
            {
                result = TRUE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_PopHandleNum(
                                     uint16         num_index,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     )
{
    BOOLEAN     result = FALSE;
    uint16      prev_item_index = 0;
    uint16      prev_first_item_index = 0;
    int16       offset_y = 0;
    BOOLEAN     is_draw_all = FALSE;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    //set current item index
    if (num_index < menu_ctrl_ptr->item_total_num)
	{
        //stop item text timer
        CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

        if (num_index != menu_ctrl_ptr->cur_item_index)
        {
            GUI_RECT_T  pre_cur_item_rect = {0};
            
            //set previous item index
            prev_item_index = menu_ctrl_ptr->cur_item_index;
            prev_first_item_index = menu_ctrl_ptr->first_item_index;
            
            pre_cur_item_rect = PopmenuGetSelectedBarRect(prev_item_index, menu_ctrl_ptr);

            //set current item index
            menu_ctrl_ptr->cur_item_index = num_index;
            
            // 重新设置offset Y
            if (ResetPopmenuDisplayItem(menu_ctrl_ptr, &offset_y))
            {
                menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + offset_y);
                is_draw_all = TRUE;
                PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
            }

            //update menu display
            PopmenuUpdateMenuDisplay(FALSE,
                is_draw_all,
                prev_first_item_index,
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
PUBLIC void CTRLMENU_PopHandleTpDown(
                                     GUI_POINT_T    *tp_point_ptr,
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     )
{
    uint16      i = 0;
    uint16      item_index = 0;
    uint16      prev_item_index = 0;
    GUI_RECT_T  item_rect = {0};
    GUI_RECT_T  inter_rect = {0};
    GUI_RECT_T  menu_rect = {0};
    BOOLEAN     is_draw_all = FALSE;

    if ((PNULL == menu_ctrl_ptr) || (PNULL == tp_point_ptr))
    {
        return;
    }

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    menu_ctrl_ptr->is_tp_pressed = TRUE; // 该变量在此表示触笔按下

    // 记录起始点
    menu_ctrl_ptr->slide_info.start_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->slide_info.start_point.y = tp_point_ptr->y;
    menu_ctrl_ptr->slide_info.prev_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->slide_info.prev_point.y = tp_point_ptr->y;

    if (!GUI_PointIsInRect(*tp_point_ptr,menu_rect))
    {
        return;
    }

    // 记录这个时候的滑动状态
    menu_ctrl_ptr->slide_info.tpdown_slide_state = menu_ctrl_ptr->slide_info.slide_state;

    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y ); // 增加速度的点

    if ( MMK_TP_SLIDE_FLING == menu_ctrl_ptr->slide_info.slide_state ) // fling的时候直接返回
    {
        return;
    }

    menu_ctrl_ptr->is_item_pressed = FALSE;

    //if press item
    for (i = 0; i < menu_ctrl_ptr->item_total_num; i++)
    {
        //get item rect
        item_rect = PopmenuGetSelectedBarRect(i,menu_ctrl_ptr);

        if (GUI_IntersectRect(&inter_rect, menu_rect, item_rect)
            && GUI_PointIsInRect(*tp_point_ptr,item_rect))
        {
            //get item index
            item_index = (uint16)(i + menu_ctrl_ptr->first_item_index);

#if !defined(MMI_PDA_SUPPORT)   // 由于PDA项目抬起时不画高亮，这里每次都画一下
            if (i != menu_ctrl_ptr->cur_item_index)
#endif
            {
                GUI_RECT_T  pre_cur_item_rect = {0};

                //stop item text timer
                CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

                //set previous item index
                prev_item_index = menu_ctrl_ptr->cur_item_index;

                pre_cur_item_rect = PopmenuGetSelectedBarRect(prev_item_index, menu_ctrl_ptr);
                                
                if (item_rect.top < inter_rect.top)
                {
                    menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.top - item_rect.top);
                    PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
                    is_draw_all = TRUE;
                }
                else if (item_rect.bottom > inter_rect.bottom)
                {
                    menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.bottom - item_rect.bottom);
                    PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
                    is_draw_all = TRUE;
                }
                else
                {
                    SCI_TRACE_LOW("current item is in menu!");
                }

                //set current item index
                menu_ctrl_ptr->cur_item_index = i;
                menu_ctrl_ptr->is_item_pressed = TRUE;

                //update menu display
                PopmenuUpdateMenuDisplay(FALSE,
                    is_draw_all,
                    menu_ctrl_ptr->first_item_index,
                    prev_item_index,
                    &pre_cur_item_rect,
                    menu_ctrl_ptr->item_total_num,
                    menu_ctrl_ptr);
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
PUBLIC BOOLEAN CTRLMENU_PopHandleTpUp(
                                      GUI_POINT_T       *tp_point_ptr,
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    BOOLEAN         result = FALSE;
    uint16          i = 0;
    GUI_RECT_T      item_rect = {0};
    GUI_RECT_T      menu_rect = {0};
    GUI_POINT_T     tpup_point  =  {0};
    uint16          layer_width = 0;
    uint16          layer_height = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if ((PNULL == menu_ctrl_ptr) || (PNULL == tp_point_ptr))
    {
        return FALSE;
    }

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
    menu_ctrl_ptr->is_item_pressed = FALSE;

    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state
        && !GUI_PointIsInRect(menu_ctrl_ptr->slide_info.start_point, menu_rect))
    {
        return result;
    }

    //get selected bar display rect
    menu_ctrl_ptr->is_tp_pressed = FALSE; // 该变量在此表示触笔弹起

    CTRLMENU_StopRedrawTimer(menu_ctrl_ptr);

    if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_info.slide_state
        && menu_ctrl_ptr->common_theme.is_used_slide)
    {
        // add
        MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );

        // compute
        if (menu_ctrl_ptr->slide_info.tpdown_slide_state == MMK_TP_SLIDE_FLING
            && abs((int)menu_ctrl_ptr->slide_info.fling_velocity) > FLING_MIN_VELOCITY)
        {
            MMK_ComputeCurrentVelocity( PNULL, &menu_ctrl_ptr->slide_info.fling_velocity, FLING_MAX_SUM_VELOCITY, FLING_MAX_SUM_VELOCITY );
        }
        else
        {
            MMK_ComputeCurrentVelocity( PNULL, &menu_ctrl_ptr->slide_info.fling_velocity, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY );
        }

        SCI_TRACE_LOW("PopmenuHandleTpUp %f",menu_ctrl_ptr->slide_info.fling_velocity);
        
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
            ResumePopmenu(menu_ctrl_ptr, ResetPopMenuOffsetY(menu_ctrl_ptr));
        }
    }
    else if (GUI_PointIsInRect(*tp_point_ptr,base_ctrl_ptr->rect))
    {
        //if press item
        for (i = 0; i < menu_ctrl_ptr->item_total_num; i++)
        {
            //get item rect
            item_rect = PopmenuGetSelectedBarRect(i,menu_ctrl_ptr);

            if (GUI_IntersectRect(&item_rect, PopmenuAdjustMenuRect(menu_ctrl_ptr), item_rect) 
                && GUI_PointIsInRect(*tp_point_ptr,item_rect))
            {
                if (i == menu_ctrl_ptr->cur_item_index)
                {
                    if (GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style)
                    {
                        BOOLEAN     is_checked = FALSE;
                        GUIMENU_POP_SEL_INFO_T  item_info = {0};

                        CTRLMENU_GetPopSelInfo(menu_ctrl_ptr, i, &item_info);

                        is_checked = CTRLMENU_GetPopItemStatus(&item_info);

                        is_checked = (BOOLEAN)(is_checked ? FALSE : TRUE);

                        CTRLMENU_SetPopItemStatus(is_checked, &item_info);

                        PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);
                    }
                    else
                    {
                        result = TRUE;
                    }
                }
                else
                {
                    //get item rect
                    item_rect = PopmenuGetSelectedBarRect(menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);
                    //reset item bg
                    PopmenuDisplayBg(menu_ctrl_ptr,item_rect);
                    PopmenuDisplayOneItem(TRUE, menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr);
                }
                break;
            }
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
LOCAL BOOLEAN ResumePopmenu(
                            CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                            int16           move_space
                            )
{
    BOOLEAN             result      = FALSE;
    uint16              i           = 0;
    uint16              frame_num   = MMK_RECOVER_SLIDE_TOTAL_FRAME; // 移动频率
    int16               offset_y    = 0;
    int32               offset      = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (0 != move_space)
    {
        offset_y = menu_ctrl_ptr->offset_y;
        
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif       

        lcd_dev_info.block_id = menu_ctrl_ptr->item_layer.block_id;

        for (i = MMK_RECOVER_SLIDE_START_FRAME; i <= frame_num; i++)
        {
            offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, move_space, frame_num, i);
            
            menu_ctrl_ptr->offset_y = (int16)(offset_y + offset);
                         
            //display item
            PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);
            
            if (i != frame_num)
            {
                GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, base_ctrl_ptr->rect,0);
            }
        }
        
#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif

        result = TRUE;
    }

    menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;

#if !defined(MMI_PDA_SUPPORT)   // PDA风格不需要画最后的高亮
    PopmenuDisplayOneItem(TRUE, menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr);
#endif

    return result;
}

/*****************************************************************************/
//  Description : handle menu touch panel press down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopHandleTpMove(
                                     GUI_POINT_T        *tp_point_ptr,
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                     )
{
    GUI_RECT_T  menu_rect = {0};

    if ((PNULL == menu_ctrl_ptr) || (PNULL == tp_point_ptr))
    {
        return;
    }

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_info.slide_state
        && !GUI_PointIsInRect(menu_ctrl_ptr->slide_info.start_point, menu_rect))
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
        uint16      i = 0;
        uint16      item_index = 0;
        uint16      prev_item_index = 0;
        GUI_RECT_T  item_rect = {0};      
        GUI_RECT_T  inter_rect = {0};
        BOOLEAN     is_draw_all = FALSE;

        menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;

        //if press item
        for (i = 0; i < menu_ctrl_ptr->item_total_num; i++)
        {
            //get item rect
            item_rect = PopmenuGetSelectedBarRect(i,menu_ctrl_ptr);
            
            if (GUI_IntersectRect(&inter_rect, menu_rect, item_rect)
                && GUI_PointIsInRect(*tp_point_ptr,item_rect))
            {
                //get item index
                item_index = (uint16)(i + menu_ctrl_ptr->first_item_index);
                
                if (i != menu_ctrl_ptr->cur_item_index)
                {
                    GUI_RECT_T  pre_cur_item_rect = {0};
                    
                    //stop item text timer
                    CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);
                    
                    //set previous item index
                    prev_item_index = menu_ctrl_ptr->cur_item_index;
                    
                    pre_cur_item_rect = PopmenuGetSelectedBarRect(prev_item_index, menu_ctrl_ptr);
                    
                    if (item_rect.top < inter_rect.top)
                    {
                        menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.top - item_rect.top);
                        PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
                        is_draw_all = TRUE;
                    }
                    else if (item_rect.bottom > inter_rect.bottom)
                    {
                        menu_ctrl_ptr->offset_y = (int16)(menu_ctrl_ptr->offset_y + inter_rect.bottom - item_rect.bottom);
                        PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
                        is_draw_all = TRUE;
                    }
                    else
                    {
                        SCI_TRACE_LOW("current item is in menu!");
                    }
                    
                    //set current item index
                    menu_ctrl_ptr->cur_item_index = i;
                    
                    //update menu display
                    PopmenuUpdateMenuDisplay(FALSE,
                        is_draw_all,
                        menu_ctrl_ptr->first_item_index,
                        prev_item_index,
                        &pre_cur_item_rect,
                        menu_ctrl_ptr->item_total_num,
                        menu_ctrl_ptr);
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
PUBLIC void CTRLMENU_PopHandleTpScroll(
                                       uint16           first_item_index,
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       )
{
    uint16      line_num_page = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (CTRLMENU_GetStatePtr(menu_ctrl_ptr, CTRLMENU_STATE_PRG_PIXEL_SCROLL))
    {
        if ((first_item_index < CTRLPROGRESSBAR_GetTotalItemNum(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr))) &&
            (first_item_index != CTRLPROGRESSBAR_GetFirstItemIndex(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr))))
        {
            GUI_RECT_T menu_rect = {0};

            //stop item text timer
            CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);

            menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);
            menu_ctrl_ptr->offset_y = (int16)(menu_rect.top - first_item_index);

            PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);
        }
    } 
    else
    {
        if ((first_item_index < menu_ctrl_ptr->item_total_num) &&
            (first_item_index != menu_ctrl_ptr->first_item_index))
        {
            //stop item text timer
            CTRLMENU_StopItemTextTimer(menu_ctrl_ptr);
            
            //get line number in one page
            line_num_page = PopmenuGetLineNumPage(menu_ctrl_ptr);
            
            //set first item index
            if (first_item_index <= menu_ctrl_ptr->item_total_num - line_num_page)
            {
                menu_ctrl_ptr->first_item_index = first_item_index;
            }
            else
            {
                menu_ctrl_ptr->first_item_index = (uint16)(menu_ctrl_ptr->item_total_num - line_num_page);
            }
            
            menu_ctrl_ptr->offset_y = PopmenuGetOffsetY(menu_ctrl_ptr, first_item_index);
            
            PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, TRUE);
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_PopRedrawTimer(
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
            if (!menu_ctrl_ptr->is_tp_pressed || 
                IS_TP_MOVE_Y(menu_ctrl_ptr->slide_info.prev_point.y, point.y))
            {
                UILAYER_SetDirectDraw( TRUE );
                
                PopmenuMoveDisplay(menu_ctrl_ptr, &point);
                menu_ctrl_ptr->slide_info.prev_point.x = point.x;
                menu_ctrl_ptr->slide_info.prev_point.y = point.y;

                VTLBASE_SetSlideState((CTRLBASE_OBJ_T*)menu_ctrl_ptr,TRUE);
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
LOCAL BOOLEAN PopmenuMoveDisplay(
                                 CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                 GUI_POINT_T        *tp_point_ptr
                                 )
{
    BOOLEAN         result = FALSE;
    int32           offset_y = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if ((PNULL == tp_point_ptr) || (PNULL == menu_ctrl_ptr))
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
        menu_ctrl_ptr->offset_y = (int16)MMK_GetFlingOffset( menu_ctrl_ptr->offset_y, menu_ctrl_ptr->slide_info.fling_velocity, (float)FLING_TIME, &menu_ctrl_ptr->slide_info.fling_velocity );
        if (FLING_RUN_MIN_VELOCITY > abs((int32)menu_ctrl_ptr->slide_info.fling_velocity))
        {
            CTRLMENU_StopRedrawTimer(menu_ctrl_ptr);

            if (0 != ResetPopMenuOffsetY(menu_ctrl_ptr)) // 判断是否需要恢复
            {
                result = FALSE;
            }
            else
            {
                result = TRUE;
            }
        }
        else if (0 >= menu_ctrl_ptr->slide_info.fling_velocity) // 向上
        {
            int16 end_offset_y = 0;
            
            // 记录
            end_offset_y = (int16)(menu_ctrl_ptr->item_total_num*GetItemLineHeight(menu_ctrl_ptr));

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

        DoDoubleMemWriteBack( menu_ctrl_ptr, offset_y );

        PopmenuReDisplayMenu(menu_ctrl_ptr, TRUE, FALSE);

        SCI_MEMSET(&menu_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));
    }
    else
    {
        ResumePopmenu(menu_ctrl_ptr, ResetPopMenuOffsetY(menu_ctrl_ptr));
    }
    
    return result;
}

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
    GUI_RECT_T  clip_rc = {0};
    int32       offset = 0;
    GUI_LCD_DEV_INFO    *lcd_dev_info_ptr = PNULL;

    if( PNULL == menu_ctrl_ptr )
    {
        return;
    }

    //滑动第一帧需要重画
    if (!VTLBASE_GetSlideState((CTRLBASE_OBJ_T*)menu_ctrl_ptr))
    {
        return;
    }

    lcd_dev_info_ptr = CTRLMENU_GetPopLcdDev(menu_ctrl_ptr, PNULL);

    if ( !( menu_ctrl_ptr->common_theme.is_used_slide
        && UILAYER_IsLayerActive( lcd_dev_info_ptr ) ) )
    {
        return;
    }

    if ( 0 == (offset = menu_ctrl_ptr->offset_y - offset_y ) )
    {
        return;
    }

    //是否双缓冲
    if ( !( UILAYER_GetLayerMemFormat( lcd_dev_info_ptr ) & UILAYER_MEM_FORMAT_DOUBLE ) )
    {
        return;
    }

    //get pop-up menu rect
    dst_rc = src_rc = clip_rc = PopmenuAdjustMenuRect(menu_ctrl_ptr);
    
    dst_rc.top    = (int16)(dst_rc.top + offset);
    dst_rc.bottom = (int16)(dst_rc.bottom + offset);

    //display bg
    PopmenuDisplayBg(menu_ctrl_ptr, src_rc);

    GUI_IntersectRect( &clip_rc, clip_rc, dst_rc );
    
    if( UILAYER_DoubleMemWriteBack( lcd_dev_info_ptr, &dst_rc, &clip_rc, &src_rc ) )
    {
        //如果回写bufer成功则将相应的区域值上
        menu_ctrl_ptr->writeback_rect = clip_rc;
    }
}
#endif

/*****************************************************************************/
//  Description : 根据一个item的索引号和区域，获得offset Y
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
//  此方法获取的偏移量，是当item_index为第一个可见项时的偏移
/*****************************************************************************/
LOCAL int16 PopmenuGetOffsetY(
                              CTRLMENU_OBJ_T    *menu_ctrl_ptr, 
                              uint16            item_index
                              )
{
    int16       offset_y = 0;
    GUI_RECT_T  menu_rect = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return offset_y;
    }

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    offset_y = (int16)(menu_rect.top - item_index*GetItemLineHeight(menu_ctrl_ptr));

    return offset_y;
}

/*****************************************************************************/
//  Description : display scroll item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_PopScrollItemStr(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    uint16          text_rect_width = 0;
    uint16          text_pixel_len = 0;
    GUI_RECT_T      text_rect = {0};
    MMI_STRING_T    text_str = {0};
    GUI_FONT_T      font_type = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    //get text display rect
    text_rect = PopmenuGetItemTextRect(menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);
    text_rect_width = (uint16)(text_rect.right - text_rect.left + 1);

    //get text
    CTRLMENU_GetItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index,&text_str, PNULL, PNULL);

    //get item text len
    font_type = menu_ctrl_ptr->pop_theme.cur_item_font;
    
    //set text string index
    if (MMITHEME_GetIsScrollAsPixel())
    {
        //get item text len
        text_pixel_len = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
                            text_str.wstr_len,
                            font_type,
                            0);

        if ((text_pixel_len - 1 - menu_ctrl_ptr->item_str_index) > text_rect_width)
        {
            menu_ctrl_ptr->item_str_index = (uint16)(menu_ctrl_ptr->item_str_index + menu_ctrl_ptr->pop_theme.pixel_scroll_step);
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
                            font_type,
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
    PopmenuDisplayOneItem(TRUE,menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr);
}

/*****************************************************************************/
//  Description : update previous pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:  当前是pop-up menu,旋转后需要刷新之前所有的pop-up menu
/*****************************************************************************/
PUBLIC void CTRLMENU_PopModifyRect(
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    int16                   offset_y = 0;
    uint16                  i = 0;
    uint16                  menu_level = 0;
    uint16                  first_item_index = 0;
    uint16                  cur_item_index = 0;
    uint16                  cur_item_top = 0;
    GUI_RECT_T              menu_rect = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    CTRLMENU_NODE_T         *cur_parent_node_ptr = PNULL;
    MMI_MENU_GROUP_ID_T     cur_group_id = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
    {
        if (0 < menu_ctrl_ptr->menu_level)
        {
            //save current menu info
            menu_level          = menu_ctrl_ptr->menu_level;
            cur_group_id        = menu_ctrl_ptr->cur_group_id;
            cur_item_index      = menu_ctrl_ptr->cur_item_index;
            first_item_index    = menu_ctrl_ptr->first_item_index;
            menu_rect           = base_ctrl_ptr->rect;
            offset_y            = menu_ctrl_ptr->offset_y;
            cur_parent_node_ptr = menu_ctrl_ptr->cur_parent_node_ptr;

            //need update previous all pop-up menu
            for (i=0; i<=menu_level; i++)
            {
                if (i < menu_level)
                {
                    //set menu info
                    CTRLMENU_SetInfo(i,
                        menu_ctrl_ptr->menu_stack[i].first_item_index,
                        menu_ctrl_ptr->menu_stack[i].cur_item_index,
                        menu_ctrl_ptr->menu_stack[i].rect,
                        menu_ctrl_ptr->menu_stack[i].offset_y,
                        menu_ctrl_ptr->menu_stack[i].group_id,
                        menu_ctrl_ptr->menu_stack[i].parent_node_ptr,
                        menu_ctrl_ptr);
                }

                if (0 < i)
                {
                    cur_item_top = CTRLMENU_GetCurItemTop(menu_ctrl_ptr);
                }

                if (i == menu_level)
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

                //calculate pop-up menu rect
                CTRLMENU_CalculateRect(cur_item_top,menu_ctrl_ptr);

                PopmenuModifyRect(menu_ctrl_ptr);

                if (i < menu_level)
                {
                    //modify rect and first item index
                    menu_ctrl_ptr->menu_stack[i].rect             = base_ctrl_ptr->rect;
                    menu_ctrl_ptr->menu_stack[i].first_item_index = menu_ctrl_ptr->first_item_index;
                    menu_ctrl_ptr->menu_stack[i].offset_y         = menu_ctrl_ptr->offset_y;
                }
            }
        }
        else
        {
            //calculate pop-up menu rect
            CTRLMENU_CalculateRect(cur_item_top,menu_ctrl_ptr);

            PopmenuModifyRect(menu_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void PopmenuModifyRect(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{
    uint16      line_num_page = 0;
    GUI_RECT_T  item_rect = {0};
    GUI_RECT_T  inter_rect = {0};
    GUI_RECT_T  menu_rect = {0};

    //get line number in one page
    line_num_page = PopmenuGetLineNumPage(menu_ctrl_ptr);
    
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

    menu_ctrl_ptr->offset_y = PopmenuGetOffsetY(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
    
    // 重新计算区域
    PopmenuResetTopItemAndEndItemIndex(menu_ctrl_ptr);
    
    item_rect = PopmenuGetSelectedBarRect(menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr);

    menu_rect = PopmenuAdjustMenuRect(menu_ctrl_ptr);

    if (GUI_IntersectRect(&inter_rect, item_rect, menu_rect))
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

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    CTRLMENU_ResetAnimLayer(menu_ctrl_ptr);
#endif
}

#ifdef ATEST_SUPPORT
/*****************************************************************************/
//  Description : get the item rect of pop Menu for atest
//  Global resource dependence : none
//  Author: linchen
//  Note:
/*****************************************************************************/
PUBLIC void Atest_GetPopmenuItemTextRect(
                                         uint16         item_index,
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                         GUI_RECT_T     *item_rect
                                         )
{
	if(PNULL != menu_ctrl_ptr)
	{
		*item_rect = PopmenuGetItemTextRect(item_index,menu_ctrl_ptr);
	}
}
#endif
