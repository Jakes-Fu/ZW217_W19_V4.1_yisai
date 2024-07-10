/*****************************************************************************
** File Name:      mmi_mainmenu_matrix.c                                     *
** Author:                                                                   *
** Date:           10/16/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu             Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MATRIX_MAIN_MENU_SUPPORT

#include "ctrlmainmenu_export.h"
#include "mmi_mainmenu_data.h"
#include "mmi_mainmenu_synchronize.h"
#include "mmitheme_mainmenu.h"
#include "mmitheme_mainmenu.h"
#include "mmi_gui_trc.h"
#include "mmi_id.h"

#include "mmi_app_menu_trc.h"
#include "guimenu.h"
#include "guifont.h"
#include "guires.h"
#include "mmi_modu_main.h"
#include "mmi_theme.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "guistring.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "mmi_mainmenu_export.h"
#include "ui_layer.h"
#include "mmi_common.h"
#include "block_mem.h"
#include "mmk_tp.h"
#include "mmitheme_special.h"
#include "guibutton.h"
#include "mmk_app.h"
#include "mmk_type.h"

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif

#ifdef ROLL_MAINMENU_SUPPORT
#include "special_effect.h"
#include "guistatusbar.h"
#endif

#include "mmi_menutable.h"
#if !defined MMI_PDA_SUPPORT
#include "guictrl_api.h"
#endif

#include "mmk_app.h"
#include "guictrl_api.h"
#include "u_launcher_export.h"
#include "graphics_3d_rotate.h"

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
#include "mmi3d_export.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_INVALID_ITEM_INDEX 0xFFFF
#ifdef ROLL_MAINMENU_SUPPORT
#define ROLL_DRAW_NONE   0
#define ROLL_DRAW_FIRST  1
#define ROLL_DRAW_SECOND 2
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint16 line_index;
    int32  line_offset_y;
} MMI_ICONMENU_POS_T;

typedef struct
{
    MMI_HANDLE_T                win_handle;                             //window handle
    MMI_HANDLE_T                ctrl_handle;

    GUIMENU_ICONMENU_DATA_T     icon_data;

    GUIMENU_STATE_T             menu_state;
    BOOLEAN                     is_static;                              //is static menu
    BOOLEAN                     is_focus_title;                         //title is focus for option page
    BOOLEAN                     is_title_style;

    uint16                      cur_page_index;                         //default 0,from 0 to max-1
    uint16                      cur_item_index;                         //current selected menu index. in option page style, it is also item index
    uint16                      first_item_index;                       //item index at the top
    int16                       offset_y;                               //offset y
    uint16                      pre_cur_item_index;                     //previous current index
    uint16                      item_total_num;                         //current total visible item number. in option page style, it is page count.

    BOOLEAN                     is_need_prgbox;                         //is need prgbox or not
    BOOLEAN                     is_revise_rect;                         //is need revise rect or not，当增加滚动条的时候，需要重新调整区域
    IGUICTRL_T                  *prgbox_ctrl_ptr;                       //progress box control pointer

    MMITHEME_MAINMENU_COMMON_T  main_menu_theme;                        //main menu theme

    MMIMAINEMENU_ITEM_LIST_T    menu_list_info;                         //menu list information

    MMK_TP_SLIDE_E              slide_state;
    BOOLEAN                     is_move_state;                          //item是否在抖动
    BOOLEAN                     is_moving;                              //item是否被拖动
    BOOLEAN                     is_item_pressed;                        //tp down时，是否在item上
    BOOLEAN                     is_tp_pressed;                          //is tp down?

    float                       fling_velocity;                         //滑动速度
    GUIMENU_ENTER_ANIM_TYPE_E   enter_anim_type;                        // 进入时的动画类型

    // mainmenu button
    MMI_HANDLE_T                button_handle[GUIMENU_BUTTON_NUM];      // button控件
    GUI_RECT_T                  writeback_rect;//双buffer回写

    GUIMENU_SOFTKEY_STATE_T     softkey_state;                          //softkey的状态，目前只有主菜单使用

    GUI_LCD_DEV_INFO            highlight_layer_handle;
    GUI_LCD_DEV_INFO            icon_move_layer_handle;
    uint32                      start_item_index;
    uint16                      tmp_cur_item_index;                     //移动的时候使用
    GUI_RECT_T                  move_rect;
    CONTINUE_MOVE_TYPE_E        continue_move_type;

    GUI_POINT_T                 slide_start_point;                      //Slide时候使用
    GUI_POINT_T                 slide_pre_point;                        //Slide时候使用

    MMK_TP_SLIDE_E              tpdown_slide_state;                     //slide state when tp down

    uint16                      tmp_width;                              //移动的时候使用
    uint16                      tmp_height;                             //移动的时候使用

    uint16                      tmp_first_item_index;                   //移动的时候使用

    uint8                       slide_timer_id;                         // 滑动时，slide和fling状态下，画图的timer
    uint16                      pre_msg_loop_page_index;                // 每次消息循环后，focus page index
    uint16                      pre_msg_loop_item_index;                // 每次消息循环后，focus项index
    uint16                      pre_msg_loop_menu_level;                // 每次消息循环后，focus项的level
    uint16                      menu_level;                             //menu level
    
} MMIMAINMENU_MATRIX_DATA_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO s_item_layer_handle = {0, UILAYER_NULL_HANDLE};
#endif

LOCAL uint16 s_total_row_num = 0; // 总行
#ifdef TOUCH_PANEL_SUPPORT
LOCAL int32  s_total_line_height = 0; // 总行
#endif

LOCAL MMI_ICONMENU_POS_T *s_icon_line_pos_ptr = PNULL;

LOCAL uint16 s_start_line_index = MMI_INVALID_ITEM_INDEX;
LOCAL uint16 s_end_line_index = MMI_INVALID_ITEM_INDEX;

#ifdef ROLL_MAINMENU_SUPPORT
#define ROLL_DRAW_NONE   0
#define ROLL_DRAW_FIRST  1
#define ROLL_DRAW_SECOND 2

#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO s_rollSrc_layer_handle = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO s_homebutton_layer_handle = {0, UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO s_activeTitle_layer_handle = {0, UILAYER_NULL_HANDLE};
#endif
LOCAL SE_HANDLE s_roll_effect_handle = NULL;
LOCAL int16 s_init_menuoffset = 0;
LOCAL int16 s_init_effectOffset = 0;
LOCAL int16 s_effect_offset = 0;
LOCAL uint8 s_is_all = ROLL_DRAW_NONE;
#elif defined MMI_PDA_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO s_homebutton_layer_handle = {0, UILAYER_NULL_HANDLE};
#endif
#endif
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef MMI_RES_LOW_COST
LOCAL const MMI_TEXT_ID_T s_default_softkey_text_id[] = {TXT_COMMON_OK, TXT_NULL, STXT_RETURN};
LOCAL const MMI_TEXT_ID_T s_default_softkey2_text_id[] = {TXT_NULL, TXT_NULL, STXT_FINISH};
LOCAL const MMI_TEXT_ID_T s_default_softkey3_text_id[] = {TXT_COMMON_OK, TXT_NULL, STXT_RETURN};
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Matrix Get Info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuGetInfo(
                             MMI_HANDLE_T             win_handle,
                             MMIMAINMENU_MATRIX_DATA_T *icon_menu_info_ptr
                             );

LOCAL void *MatrixMenuInit(
    MMI_HANDLE_T ctrl_handle
);

LOCAL BOOLEAN MatrixMenuTerm(
    void *pm_data_ptr
);

LOCAL MMI_RESULT_E MatrixMenuHandle(
    void *pm_data_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : redraw move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuRedrawMoveLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,  //in
    uint16 index,
    GUI_RECT_T update_rect
);

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateMoveLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : release
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseMoveIconLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
);
#endif
#endif

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create highlight layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateHighlightLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseHighlightLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
);
#endif

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawHighlightItem(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDraw(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 重新加载主菜单
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReloadItem(MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr);

#endif
/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawItemIcon(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description : handle icon menu direction key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuDirectionKey(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
);

/*****************************************************************************/
//  Description : reset icon display
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuResetIconDisplay(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : get next item index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 MatrixMenuGetNextItemIndex(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
);

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuNumKey(
    uint16           num_index,
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuTpDown(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle icon touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuTpUp(
    GUI_POINT_T       *tp_point_ptr,
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuTpMove(
    GUI_POINT_T *point,
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : display active icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplayActiveItemTitle(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
);


#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : alloc 3D buffer for icon main menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuAllocMenu3DBuffer(
                              MMI_HANDLE_T      win_handle,
                              MMI_IMAGE_ID_T    item_img_id
                              );

/*****************************************************************************/
//  Description : save previous menu icon for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL SE_RECT_T MatrixMenuSavePrevIcon(
                              MMI_HANDLE_T      win_handle,
                              MMI_IMAGE_ID_T    item_image_hl_id,
                              GUI_RECT_T        icon_rect,
                              MMI_MESSAGE_ID_E  msg_id
                              );

/*****************************************************************************/
//  Description : save next menu icon for 3D
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSaveNextMenuIcon(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuInitMenuDetail(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : 销毁宫格菜单，释放内存
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDestory(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : 初始化链表
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void MatrixResetItemListInfo(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 MatrixMenuGetTotalLineNum(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuIsNeedScrollBar(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetScrollBarRect(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : display menu scroll bar
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawScrollBar(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr,
    BOOLEAN         is_update
);

/*****************************************************************************/
//  Description : search in list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *MatrixMenuSearchMenuItemByIndex(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : get item info in list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *MatrixMenuGetMenuItemInfoByIndex(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : get item
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuGetItemInfo(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *item_ptr
);


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 创建移动的buffer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuMoveInit(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : 确认移动
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuConfirmMoveMenuItem(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
);
#endif

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuOk(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuResetMoveItemInfo(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : 根据一个item的索引号，获得该行的第一个item索引号
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 如果索引号大于总数，则返回无效的数值-1
/*****************************************************************************/
LOCAL int16 MatrixMenuGetLineStartItemIndex(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    uint16           line_index
);

/*****************************************************************************/
//  Description : 读取nv中存储的宫格菜单数据
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixReadIconMenuItemList(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 *item_num_ptr
);

#ifndef MMI_RES_LOW_COST
/*****************************************************************************/
//  Description : get softkey text id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuGetSoftkeyText(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    GUI_BUTTON_T      *button_id_ptr, //out
    MMI_TEXT_ID_T     *softkey_id_ptr //in
);
#endif

/*****************************************************************************/
//  Description : set menu softkey param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetSoftkey(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMatrixMenuTimer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,     //事件类型
    DPARAM           param          //附带的处理参数
);
#endif

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuCancelKey(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
);

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuModifyRect(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetBltMoveLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : Clear Icon Rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuClearItemRect(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr,
    uint16 item_index
);
#endif

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateItemLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseItemLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetBltIconMenuDispLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    UILAYER_HANDLE_T layer1,
    UILAYER_HANDLE_T layer2
);

/*****************************************************************************/
//  Description : blt layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetBltHeightLightLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplayIconSpecialMenu(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuBltItemLayerToMenuLayer(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMatrixMenuSpecialMsg(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetIconMenuMoveLayerRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);
#endif
#endif


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuSlide(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    GUI_POINT_T        *tp_point_ptr
);

/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 MatrixMenuGetMenuItemIndexExt(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
);
#endif

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetItemRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
);

/*****************************************************************************/
//  Description : get rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetTotalItemRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
);

/*****************************************************************************/
//  Description : reset list display item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuResetDisplayItem(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    int16  *offset_y_ptr
);

/*****************************************************************************/
//  Description : reset list display item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuAdjuestOffsetY(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    int16  *offset_y_ptr,
    uint16 line_index
);

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuAdjuestRect(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : reset Y offset
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int16 MatrixMenuResetOffsetY(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateButton(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
);
/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuTransmitTpMsg(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *tp_point_ptr
);

/*****************************************************************************/
//  Description : get rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetIconDispRect(
    GUI_RECT_T disp_rect,
    GUI_RECT_T cross_rect,
    uint16 width,
    uint16 height
);
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : MatrixMenuCreateRollSrcLayer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateRollSrcLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
);

/****************************************************************************/
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseRollLayer(void);
#endif

/*****************************************************************************/
// Description : MatrixMenuCreateRollEffect
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuCreateRollEffect(void);

/*****************************************************************************/
// Description : MatrixMenuDestroyRollEffect
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void MatrixMenuDestroyRollEffect(SE_HANDLE handle);

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
// Description : MatrixMenuRenderRollEffect
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuRenderRollEffect(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : MatrixMenuBltItemLayerToAppointLayer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuBltItemLayerToAppointLayer(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO   *dev_info_ptr
);

/*****************************************************************************/
//  Description : MatrixMenuBltItemLayerToAppointLayer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawRollSrcLayer(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
);

/****************************************************************************/
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseStatusBarAndHomeButtonLayer(void);

/*****************************************************************************/
//  Description : Create Status Bar Layer
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetStatusBarAndHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuAppendStatusBarAndHomeButtonLayer(void);

/*****************************************************************************/
//  Description : Create Status Bar Layer
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : Create Status Bar Layer
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateActiveItemTitleLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : display active icon
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplayActiveItemTitleEx(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
);
#endif
#elif defined MMI_PDA_SUPPORT //NEWMS00161615
#ifdef UI_MULTILAYER_SUPPORT
LOCAL void MatrixMenuCreateHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);
LOCAL void MatrixMenuAppendHomeButtonLayer(void);
LOCAL void MatrixMenuSetHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);
LOCAL void MatrixMenuReleaseHomeButtonLayer(void);
#endif
#endif


/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetLineRectButNotMargin(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,  //in
    uint16 line_index
);

/*****************************************************************************/
//  Description : 获取高亮区域
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetHighLightRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuClearBackGround(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
);
#endif

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 拷贝不需要更新的buffer
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDoDoubleMemWriteBack(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    int32             offset_y
);
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : display softkey bg
//  Global resource dependence :
//  Author:nan ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplaysoftkeybg(MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
                                      GUI_LCD_DEV_INFO  *lcd_dev_info_ptr,
                                      BOOLEAN is_left
                                     );

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawHandleButton(
    GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr
);
#endif

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawBg(
    GUI_RECT_T        bg_rect,
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO  *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL void MatrixMenuRunItem(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MatrixMenuNotifyParentMsg(
    CAF_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id
);

/*****************************************************************************/
//  Description : Matrix Menu Draw Active Icon
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawActiveIcon(
                                    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : Matrix Menu Get Not Title Style Info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuGetTitleStyleInfo(
                                             MMI_HANDLE_T             win_handle,
                                             MMIMAINMENU_MATRIX_DATA_T *icon_menu_info_ptr
                                             );
/*****************************************************************************/
//  Description : Matrix Menu Get Not Title Style Info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuGetNotTitleStyleInfo(
                                             MMI_HANDLE_T             win_handle,
                                             MMIMAINMENU_MATRIX_DATA_T *icon_menu_info_ptr
                                             );



#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawItem(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO *dst_dev_ptr,
    uint16                 item_index,
    BOOLEAN                is_compound
);
#endif


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Matrix Get Info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuGetInfo(
                             MMI_HANDLE_T             win_handle,
                             MMIMAINMENU_MATRIX_DATA_T *icon_menu_info_ptr
                             )
{
    if (PNULL == icon_menu_info_ptr)
    {
        return;
    }

    if (MMIMAINMENU_MATRIX_TITLE_E == MMIAPISET_GetMainmenuStyle())
    {
        MatrixMenuGetTitleStyleInfo(win_handle, icon_menu_info_ptr);
    } 
    else
    {
        MatrixMenuGetNotTitleStyleInfo(win_handle, icon_menu_info_ptr);
    }

    return;
}

/*****************************************************************************/
//  Description : Matrix Menu Get Title Style Info
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuGetTitleStyleInfo(
                                          MMI_HANDLE_T            win_handle,
                                          MMIMAINMENU_MATRIX_DATA_T *icon_menu_info_ptr
                                          )
{
    BOOLEAN result = FALSE;

	GUI_RECT_T menu_rect   = {0};    // menu的区域大小

    GUI_RECT_T title_text_display_rect   = MMITHEME_TITLE_ICONMENU_TITLE_TEXT_RECT;
	GUI_RECT_T h_title_text_display_rect = MMITHEME_H_TITLE_ICONMENU_TITLE_TEXT_RECT;
    
    uint16 line_num    = MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM;
    uint16 row_num     = MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM;
    uint16 item_height = MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_V;
	uint16 item_width  = 0;
    
    uint16 top_margin        = MMITHEME_TITLE_ICONMENU_TOP_MARGIN_V;       // menu区域的上方间距,默认值，用户可调
    uint16 left_margin       = MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_V;      // menu区域的左方间距,默认值，用户可调   
    uint16 right_margin      = MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_V;     // menu区域的右方间距,默认值，用户可调   
	uint16 margin_bottom     = MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_V;    // menu区域的下方间距,默认值，用户可调   

	// 横屏状态下
    if (MMITHEME_IsMainScreenLandscape())
	{
        title_text_display_rect = h_title_text_display_rect;

		line_num          = MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H; 
        row_num           = MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H;
        top_margin        = MMITHEME_TITLE_ICONMENU_TOP_MARGIN_H;       
		margin_bottom     = MMITHEME_TITLE_ICONMENU_BOTTOM_MARGIN_H;
		left_margin       = MMITHEME_TITLE_ICONMENU_LEFT_MARGIN_H;      
        right_margin      = MMITHEME_TITLE_ICONMENU_RIGHT_MARGIN_H;
		item_height       = MMITHEME_TITLE_ICONMENU_ITEM_HEIGHT_H;  //因为需要考虑显示半行的需求，故icon menu的item height用户由设定
	}

    if (PNULL != icon_menu_info_ptr)
    {
		// 设置menu info
		menu_rect   = MMITHEME_GetClientRectEx(win_handle); //获取menu区域
		menu_rect.top = title_text_display_rect.bottom;

		item_width  = (menu_rect.right - menu_rect.left+ 1 - left_margin - right_margin) / row_num; //均分menu
		
		icon_menu_info_ptr->icon_data.item_height   = item_height; 
		icon_menu_info_ptr->icon_data.item_width    = item_width;
		icon_menu_info_ptr->icon_data.page_row_num  = line_num;
		icon_menu_info_ptr->icon_data.page_col_num  = row_num;
		icon_menu_info_ptr->icon_data.page_num      = 1;              //icon menu仅一页
		
		icon_menu_info_ptr->icon_data.bottom_margin    = margin_bottom;
		icon_menu_info_ptr->icon_data.top_margin       = top_margin;
		icon_menu_info_ptr->icon_data.left_margin      = left_margin;
		icon_menu_info_ptr->icon_data.right_margin     = right_margin;
		icon_menu_info_ptr->icon_data.title_text_rect  = title_text_display_rect;
		
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Matrix Menu Get Not Title Style Info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuGetNotTitleStyleInfo(
                                             MMI_HANDLE_T             win_handle,
                                             MMIMAINMENU_MATRIX_DATA_T *icon_menu_info_ptr
                                             )
{
    BOOLEAN result = FALSE;

    GUI_RECT_T menu_rect   = {0};    // menu的区域大小

    uint16 line_num          = MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM; // 行数
    uint16 row_num           = MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM;  // 列数
    uint16 item_height       = MMITHEME_ICONMENU_ITEM_HEIGHT_V;
    uint16 item_width        = 0;

    uint16 top_margin        = MMITHEME_ICONMENU_TOP_MARGIN_V;       // menu区域的上方间距,默认值，用户可调
    uint16 left_margin       = MMITHEME_ICONMENU_LEFT_MARGIN_V;      // menu区域的左方间距,默认值，用户可调
    uint16 right_margin      = MMITHEME_ICONMENU_RIGHT_MARGIN_V;     // menu区域的右方间距,默认值，用户可调
    uint16 margin_bottom     = MMITHEME_ICONMENU_BOTTOM_MARGIN_V;    // menu区域的下方间距,默认值，用户可调
    uint16 icon_text_margin  = MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V; // menu item的icon和text间距,默认值，用户可调
    uint16 icon_top_space    = MMITHEME_ICONMENU_ICON_TOP_SPACE_V;   // icon与menu item区域顶部之间的的距离

    // 横屏状态下
    if (MMITHEME_IsMainScreenLandscape())
    {
        line_num          = MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM_H;//MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM;
        row_num           = MMITHEME_MENUMAIN_ICON_PAGE_ROW_NUM_H;//MMITHEME_MENUMAIN_ICON_PAGE_LINE_NUM;
        top_margin        = MMITHEME_ICONMENU_TOP_MARGIN_H;
        margin_bottom     = MMITHEME_ICONMENU_BOTTOM_MARGIN_H;
        icon_text_margin  = MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H;
        left_margin       = MMITHEME_ICONMENU_LEFT_MARGIN_H;
        right_margin      = MMITHEME_ICONMENU_RIGHT_MARGIN_H;
        item_height       = MMITHEME_ICONMENU_ITEM_HEIGHT_H;  //因为需要考虑显示半行的需求，故icon menu的item height用户由设定
        icon_top_space    = MMITHEME_ICONMENU_ICON_TOP_SPACE_H;   // icon与menu item区域顶部之间的的距离
    }

    if (PNULL != icon_menu_info_ptr)
    {
        // 设置menu info
        menu_rect   = MMITHEME_GetClientRectEx(win_handle); //获取menu区域
        item_width  = (menu_rect.right - menu_rect.left + 1 - left_margin - right_margin) / row_num; //均分menu

        icon_menu_info_ptr->icon_data.item_height   = item_height;
        icon_menu_info_ptr->icon_data.item_width    = item_width;
        icon_menu_info_ptr->icon_data.page_row_num  = line_num;
        icon_menu_info_ptr->icon_data.page_col_num  = row_num;

        icon_menu_info_ptr->icon_data.bottom_margin    = margin_bottom;
        icon_menu_info_ptr->icon_data.top_margin       = top_margin;
        icon_menu_info_ptr->icon_data.left_margin      = left_margin;
        icon_menu_info_ptr->icon_data.right_margin     = right_margin;
        icon_menu_info_ptr->icon_data.icon_text_margin = icon_text_margin;
        icon_menu_info_ptr->icon_data.icon_top_space   = icon_top_space;

        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuStopSlideTimer(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->slide_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->slide_timer_id);
        menu_ctrl_ptr->slide_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuStartSlideTimer(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->slide_timer_id)
    {
        menu_ctrl_ptr->slide_timer_id = MMK_CreateWinTimer(
                                            menu_ctrl_ptr->ctrl_handle,
                                            MMITHEME_MENU_REDRAW_TIME,
                                            FALSE);
    }
}

/*****************************************************************************/
//  Description : Matrix Init
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL void *MatrixMenuInit(
    MMI_HANDLE_T ctrl_handle
)
{
    MMI_HANDLE_T              win_handle = 0;
    MMIMAINMENU_MATRIX_DATA_T *matrix_data = PNULL;

    matrix_data = SCI_ALLOC_APP(sizeof(MMIMAINMENU_MATRIX_DATA_T));

    if (PNULL == matrix_data)
    {
        SCI_TRACE_LOW("MatrixMenuInit Faild");
        return PNULL;
    }

    SCI_MEMSET(matrix_data, 0, sizeof(MMIMAINMENU_MATRIX_DATA_T));

    matrix_data->ctrl_handle = ctrl_handle;
    matrix_data->win_handle = win_handle = MMK_GetWinHandleByCtrl(ctrl_handle);

    // 是否移动状态，目前主菜单才有效
    matrix_data->is_move_state = FALSE;

    // 默认支持滚动条
    matrix_data->is_need_prgbox = TRUE;

    // 增加滚动条时，是否需要动态调整区域
    matrix_data->is_revise_rect = FALSE;

    if (MMIMAINMENU_MATRIX_E == MMIAPISET_GetMainmenuStyle())
    {
        matrix_data->is_title_style = FALSE;
    } 
    else
    {
        matrix_data->is_title_style = TRUE;
        //清除idle上缓存的menu item layer
        MMITHEME_DestroyMenuItemCache();
    }

    matrix_data->main_menu_theme.is_used_slide = TRUE;

    MatrixMenuInitMenuDetail(matrix_data);

    matrix_data->cur_item_index = matrix_data->main_menu_theme.default_item_index;

    return (void *)matrix_data;
}

/*****************************************************************************/
//  Description : Matrix Term
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuTerm(
    void *pm_data_ptr
)
{
    MMIMAINMENU_MATRIX_DATA_T * menu_ctrl_ptr =(MMIMAINMENU_MATRIX_DATA_T *)pm_data_ptr;

    if (PNULL == pm_data_ptr)
    {
        SCI_TRACE_LOW("MatrixMenuTerm Faild");
        return FALSE;
    }

    MatrixMenuDestory(menu_ctrl_ptr);
    SCI_FREE(pm_data_ptr);
    pm_data_ptr = PNULL;

    return TRUE;
}


/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MatrixMenuNotifyParentMsg(
    CAF_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id
)
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    MMI_MESSAGE_ID_E    notify_msg = MSG_NOTIFY_START;

    switch (msg_id)
    {
        case MSG_APP_WEB:
            notify_msg = MSG_NOTIFY_MIDSK;
            break;

        case MSG_APP_OK:
            notify_msg = MSG_NOTIFY_OK;
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_TP_PRESS_UP:
            notify_msg = MSG_NOTIFY_PENOK;
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_APP_CANCEL:
            notify_msg = MSG_NOTIFY_CANCEL;
            break;

        default:
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUIMENU_2297_112_2_18_3_20_22_238, (uint8 *)"d", msg_id);
            break;
    }

    if (MSG_NOTIFY_START != notify_msg)
    {
        //notify parent control
        result = GUICTRL_PostNotify(ctrl_handle, notify_msg);
    }

    return (result);
}
/*****************************************************************************/
//  Description : MatrixMenuHandleOkKey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuHandleOkKey(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
)
{
    BOOLEAN                 is_handled = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    is_handled = HandleMatrixMenuOk(menu_ctrl_ptr, msg_id);

    if (!menu_ctrl_ptr->is_move_state && !is_handled)
    {
        BOOLEAN is_grayed = FALSE;
        MMIMAINMENU_ITEM_INFO_T item = {0};

        MMIMAINMENU_GetItem(&menu_ctrl_ptr->menu_list_info, menu_ctrl_ptr->cur_item_index, &item);
        is_grayed = MMIMAINMENU_IsItemDisable(item.menu_id);

        if (!is_grayed)
        {
            MatrixMenuRunItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
        }
        else
        {
            HandleMatrixMenuSpecialMsg(menu_ctrl_ptr, msg_id, (DPARAM)&is_grayed);
        }    
    }

    return (is_handled);
}

/*****************************************************************************/
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_MatrixRegister(CTRLMAINMENU_PM_APIS_T *apis)
{
    if (PNULL == apis)
    {
        return FALSE;
    }

    apis->Initialize = MatrixMenuInit;
    apis->Destory = MatrixMenuTerm;
    apis->Handle = MatrixMenuHandle;

    return TRUE;
}

/*****************************************************************************/
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_MatrixSetIsTitleStyle(MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
                                                 BOOLEAN is_title_style)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    menu_ctrl_ptr->is_title_style = is_title_style;

    return TRUE;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 当前的Item是否在页中显示,因为当触笔移动滚动条的时候,
//                  当前Item可能会不在页面中显示
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN MatrixMenuIsItemInPage(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    uint16           item_index
)
{
    uint16 page_item_num = 0;
    BOOLEAN result = FALSE;

    if (PNULL != menu_ctrl_ptr)
    {
        page_item_num = menu_ctrl_ptr->icon_data.page_col_num * menu_ctrl_ptr->icon_data.page_row_num;

        if (item_index >= menu_ctrl_ptr->start_item_index
            && (page_item_num > (item_index - menu_ctrl_ptr->start_item_index)))
        {
            result = TRUE;
        }
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description : get line index of item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL __inline uint16 MatrixMenuGetItemLineIndex(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr,
    uint16 item_index
)
{
    uint16 line_index = 0;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return line_index;
    }

    if (0 != menu_ctrl_ptr->icon_data.page_col_num)
    {
        line_index = item_index / menu_ctrl_ptr->icon_data.page_col_num;
    }

    return (line_index);
}

#if defined TOUCH_PANEL_SUPPORT ||  defined ROLL_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 获取当前页能够显示的数目
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note
/*****************************************************************************/
LOCAL __inline uint16 MatrixMenuGetIconMenuPageItemNum(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    uint16 page_item_num = 0;

    if (PNULL != menu_ctrl_ptr)
    {
        page_item_num = menu_ctrl_ptr->icon_data.page_col_num * menu_ctrl_ptr->icon_data.page_row_num;
        page_item_num = MIN(page_item_num, menu_ctrl_ptr->menu_list_info.item_count);
    }

    return page_item_num;
}
#endif

/*****************************************************************************/
//  Description : 根据一个item的索引号，获得该行的第一个item索引号
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 如果索引号大于总数，则返回无效的数值-1
/*****************************************************************************/
LOCAL int16 MatrixMenuGetLineStartItemIndex(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    uint16           line_index
)
{
    int16 item_index = -1;

    if (PNULL != menu_ctrl_ptr && line_index < MatrixMenuGetTotalLineNum(menu_ctrl_ptr))
    {
        item_index = line_index * menu_ctrl_ptr->icon_data.page_col_num;
    }

    return item_index;
}


/*****************************************************************************/
//  Description : 销毁宫格菜单，释放内存
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDestory(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    MatrixResetItemListInfo(menu_ctrl_ptr);

#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuReleaseMoveIconLayer(menu_ctrl_ptr);
#endif
#endif

#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuReleaseItemLayer(menu_ctrl_ptr);

    MatrixMenuReleaseHighlightLayer(menu_ctrl_ptr);
#endif

    if (PNULL != s_icon_line_pos_ptr)
    {
        SCI_FREE(s_icon_line_pos_ptr);
    }

#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMENU_DyFreeData();
#endif

#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuReleaseRollLayer();
#endif
    MatrixMenuDestroyRollEffect(s_roll_effect_handle);
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuReleaseStatusBarAndHomeButtonLayer();
#endif
#elif defined MMI_PDA_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuReleaseHomeButtonLayer();
#endif
#endif

    return;
}

/*****************************************************************************/
//  Description : 初始化链表
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void MatrixResetItemListInfo(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (PNULL != menu_ctrl_ptr->menu_list_info.list_head_ptr)
    {
        MMILINK_DestroyLink(menu_ctrl_ptr->menu_list_info.list_head_ptr, MMIMAINMENU_FreeItemNode);
    }

    menu_ctrl_ptr->menu_list_info.list_head_ptr = PNULL;
    menu_ctrl_ptr->menu_list_info.item_count = 0;

    return;
}

/*****************************************************************************/
//  Description : search in list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *MatrixMenuSearchMenuItemByIndex(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index
)
{
    BOOLEAN is_find = FALSE;
    uint32  i = 0;
    MMI_LINK_NODE_T *tmp_ptr = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr = PNULL;

    if (PNULL != menu_ctrl_ptr)
    {
        tmp_head_ptr = tmp_ptr = menu_ctrl_ptr->menu_list_info.list_head_ptr;

        if (PNULL != tmp_ptr)
        {
            if (0 == item_index)
            {
                tmp_ptr = tmp_head_ptr;
                is_find = TRUE;
            }
            else
            {
                for (i = 0; i < menu_ctrl_ptr->menu_list_info.item_count; i++)
                {
                    // 当回到起始点的时候，肯定是没找到
                    if (i == item_index && tmp_ptr != tmp_head_ptr)
                    {
                        is_find = TRUE;
                        break;
                    }

                    // 指向下一个指针
                    tmp_ptr = tmp_ptr->next_ptr;
                }
            }
        }
    }

    if (!is_find)
    {
        // 如果没找到，初始化为PNULL
        tmp_ptr = PNULL;
    }

    return tmp_ptr;
}

/*****************************************************************************/
//  Description : get item info in list
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *MatrixMenuGetMenuItemInfoByIndex(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index
)
{
    MMI_LINK_NODE_T *item_node_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;

    item_node_ptr = MatrixMenuSearchMenuItemByIndex(menu_ctrl_ptr, item_index);

    if (PNULL != item_node_ptr)
    {
        item_info_ptr = (MMIMAINMENU_ITEM_INFO_T *)(item_node_ptr->data);
    }

    return item_info_ptr;
}

/*****************************************************************************/
//  Description : get item
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuGetItemInfo(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *item_ptr
)
{
    MMIMAINMENU_ITEM_INFO_T item = {0};

    MMIMAINMENU_GetItem(&menu_ctrl_ptr->menu_list_info, item_index, &item);

    MMIMAINMENU_GetItemData(item.menu_id, item.group_id, item_ptr);

    return;
}

/*****************************************************************************/
//  Description : get item
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_MatrixGetItem(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *menuitem_info_ptr
)
{
    MatrixMenuGetItemInfo(menu_ctrl_ptr, item_index, menuitem_info_ptr);

    return;
}

/*****************************************************************************/
//  Description : 读取nv中存储的宫格菜单数据
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixReadIconMenuItemList(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 *item_num_ptr
)
{
    uint32 i = 0;
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;
    MMI_LINK_NODE_T          *node_ptr = PNULL;

    item_list_ptr = MMIMAINMENU_GetItemList();

    if (PNULL == item_list_ptr)
    {
        return FALSE;
    }

    menu_ctrl_ptr->menu_list_info.item_count = 0;
    node_ptr = item_list_ptr->list_head_ptr;

    for (i = 0; i < item_list_ptr->item_count; i++)
    {
        if (PNULL == node_ptr)
        {
            break;
        }
        
#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
        if (ID_GOMENU_CHANGE_STYLE == ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id )
        {
            SCI_TRACE_LOW("MatrixReadIconMenuItemList ID_GOMENU_CHANGE_STYLE");
        }
        else
#endif
#ifdef QBTHEME_SUPPORT
        if(ID_MAINMENU_QBTHEMEAPP == ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id
            || ID_MAINMENU_QBTHEMEMORE == ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id)
        {
            SCI_TRACE_LOW("MatrixReadIconMenuItemList ID_MAINMENU_QBTHEMEAPP");
        }
        else
#endif
        {
            
            item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));
            
            SCI_MEMSET(item_info_ptr, 0, sizeof(MMIMAINMENU_ITEM_INFO_T));
            
            item_info_ptr->group_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->group_id;
            item_info_ptr->menu_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id;
            
            MMIMAINMENU_AddItemNode(&menu_ctrl_ptr->menu_list_info.list_head_ptr, (uint32)item_info_ptr);
            
            menu_ctrl_ptr->menu_list_info.item_count++;
        }
        
        node_ptr = node_ptr->next_ptr;
    }

    if (PNULL != item_num_ptr)
    {
        *item_num_ptr = menu_ctrl_ptr->menu_list_info.item_count;
    }

    return TRUE;
}


#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateItemLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    GUI_RECT_T          page_rect = {0};
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    MatrixMenuReleaseItemLayer(menu_ctrl_ptr);

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

    page_rect.right = layer_width - 1;
    page_rect.bottom = layer_height - 1;


    s_item_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_item_layer_handle.block_id = UILAYER_NULL_HANDLE;

    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = page_rect.left;
    create_info.offset_y = page_rect.top;
    create_info.width = (page_rect.right + 1 - page_rect.left);
    create_info.height = (page_rect.bottom + 1 - page_rect.top);
    create_info.is_bg_layer = FALSE;
    create_info.format = UILAYER_MEM_FPS_ACCELERATE;
    create_info.is_static_layer = FALSE;

    UILAYER_CreateLayer(&create_info,  &s_item_layer_handle);
    // 设置color key
    UILAYER_SetLayerColorKey(&s_item_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(&s_item_layer_handle);

    return;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseItemLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
#ifdef UI_MULTILAYER_SUPPORT

    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&s_item_layer_handle);   /*lint !e506 !e774*/
    }

#endif
}

/*****************************************************************************/
//  Description : remove all menu layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuRemoveAllLayer(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
#ifdef UI_MULTILAYER_SUPPORT
    uint32 i = 0;
    GUI_LCD_DEV_INFO lcd_dev_info[3] = {0};

    lcd_dev_info[0] = s_item_layer_handle;
    lcd_dev_info[1] = menu_ctrl_ptr->icon_move_layer_handle;
    lcd_dev_info[2] = menu_ctrl_ptr->highlight_layer_handle;

    for (i = 0; i < ARR_SIZE(lcd_dev_info); i++)
    {
        if (0 != lcd_dev_info[i].block_id)
        {
            UILAYER_RemoveBltLayer(&lcd_dev_info[i]);
        }
    }

#endif
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetBltIconMenuDispLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    UILAYER_HANDLE_T layer1,
    UILAYER_HANDLE_T layer2
)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    MatrixMenuRemoveAllLayer(menu_ctrl_ptr);

    lcd_dev_info.block_id = layer1;

    if (0 != layer1 && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    lcd_dev_info.block_id = layer2;

    if (0 != layer2 && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

#endif
}
#endif


/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateIconAndTextPosMem(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (PNULL != s_icon_line_pos_ptr)
    {
        SCI_FREE(s_icon_line_pos_ptr);
    }

    // 申请内存
    if (PNULL == s_icon_line_pos_ptr && 0 != s_total_row_num)
    {
        s_icon_line_pos_ptr = SCI_ALLOC_APP(s_total_row_num * sizeof(MMI_ICONMENU_POS_T));
        SCI_MEMSET(s_icon_line_pos_ptr, 0, s_total_row_num * sizeof(MMI_ICONMENU_POS_T));
    }

    return;
}

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuAdjuestRect(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    GUI_RECT_T menu_rect = {0};

    //xiyuan edit
    menu_rect = MMITHEME_GetClientRectEx(menu_ctrl_ptr->win_handle);

    if (menu_ctrl_ptr->is_title_style)
    {
        menu_rect.top = menu_ctrl_ptr->icon_data.title_text_rect.bottom;
    }

    menu_rect.left     += menu_ctrl_ptr->icon_data.left_margin;
    menu_rect.right    -= menu_ctrl_ptr->icon_data.right_margin;
    menu_rect.top      += menu_ctrl_ptr->icon_data.top_margin;
    menu_rect.bottom   -= menu_ctrl_ptr->icon_data.bottom_margin;
    //end edit

    return menu_rect;
}

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCalIconAndTextPos(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    uint16     row          = 0;
    uint16     line_height  = 0;
    GUI_RECT_T menu_rect    = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    if (0 == s_total_row_num)
    {
        return;
    }

    line_height  = menu_ctrl_ptr->icon_data.item_height;  //行高

    for (row = 0; row < s_total_row_num; row++)
    {
        s_icon_line_pos_ptr[row].line_index    = row;
        s_icon_line_pos_ptr[row].line_offset_y = row * line_height + menu_rect.top;
    }

#ifdef TOUCH_PANEL_SUPPORT
    // 计算总高度
    s_total_line_height = s_total_row_num * line_height;
#endif

    return;
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuGetTopAndBottomLineIndex(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN    is_first    = FALSE;
    uint16     row         = 0;
    uint16     line_height = 0;
    GUI_RECT_T line_rect   = {0};
    GUI_RECT_T cross_rect  = {0};
    GUI_RECT_T disp_rect   = {0};

    s_start_line_index = 0;
    s_end_line_index = 0;

    disp_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);
    line_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);
    line_height = menu_ctrl_ptr->icon_data.item_height;

    for (row = 0; row < s_total_row_num; row++)
    {
        line_rect.top = s_icon_line_pos_ptr[row].line_offset_y + menu_ctrl_ptr->offset_y;
        line_rect.bottom = line_rect.top + line_height - 1;

        if (GUI_IntersectRect(&cross_rect, line_rect, disp_rect)
            && !GUI_IsInvalidRect(cross_rect))
        {
            if (!is_first)
            {
                s_start_line_index = row; // 起始行
                s_end_line_index = row;
                is_first = TRUE;
            }
            else
            {
                s_end_line_index = row; // 结束行
            }
        }
    }

    menu_ctrl_ptr->start_item_index = s_start_line_index * menu_ctrl_ptr->icon_data.page_col_num;
    menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->start_item_index;

    return;
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note: xiyuan edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T HandleMatrixMenuGetIconRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
)
{
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T icon_rect = {0};

    if (!menu_ctrl_ptr->is_title_style)
    {
        // 获取total区域
        item_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

        // 获取中心区域，定高和宽
        item_rect = GUI_GetCenterRectExt(item_rect, MMITHEME_MENU_ITEM_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);

        // 获取中心区域，定最终的宽
        icon_rect = GUI_GetCenterRectExt(item_rect, MENUICON_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);

        icon_rect.right = icon_rect.left + MENUICON_WIDTH - 1;
        icon_rect.bottom = icon_rect.top + MENUICON_HEIGHT - 1;
    }
    else if (menu_ctrl_ptr->is_title_style)//不带title
    {
        GUI_RECT_T temp_rect = {0};

        temp_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

        icon_rect = GUI_GetCenterRect(temp_rect, MENUICON_WIDTH, MENUICON_HEIGHT);
    }
    else
    {
        icon_rect.right = MENUICON_WIDTH - 1;
        icon_rect.bottom = MENUICON_HEIGHT - 1;
    }

    return icon_rect;
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note: xiyuan edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetTextRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
)
{
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T text_rect = {0};

    if (!menu_ctrl_ptr->is_title_style)
    {
        // 获取total区域
        item_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

        // 获取中心区域，定高和宽
        item_rect = GUI_GetCenterRectExt(item_rect, MMITHEME_MENU_ITEM_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);

        // 获取中心区域，定最终的宽
        text_rect = GUI_GetCenterRectExt(item_rect, MENUTEXT_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);

        text_rect.right = text_rect.left + MENUTEXT_WIDTH - 1;
        text_rect.top = text_rect.bottom - MENUTEXT_HEIGHT + 1;
    }
    else
    {
        text_rect = menu_ctrl_ptr->icon_data.title_text_rect;
    }

    return text_rect;
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note: xiyuan edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetItemRect(
                                       MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
                                       int16 item_index
                                       )
{
    return HandleMatrixMenuGetIconRect(menu_ctrl_ptr, item_index);
}

#ifdef ATEST_SUPPORT
PUBLIC GUI_RECT_T Atest_MatrixMenuGetItemRect(void *menu_ctrl_ptr, int16 item_index)
{
  return MatrixMenuGetItemRect((MMIMAINMENU_MATRIX_DATA_T *)menu_ctrl_ptr, item_index);
}
#endif
/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note: xiyuan edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetTextRectExt(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
)
{
    return MatrixMenuGetTextRect(menu_ctrl_ptr, item_index);
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetTotalItemRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
)
{
    int16 i = 0;
    int16 line_index = 0;
    int16 start_item_index = 0;
    int16 line_height = 0;
    GUI_RECT_T line_rect = {0};
    GUI_RECT_T item_rect = {0};

    // 行号
    line_index = MatrixMenuGetItemLineIndex(menu_ctrl_ptr, item_index);

    // 本行开始的item索引
    start_item_index = MatrixMenuGetLineStartItemIndex(menu_ctrl_ptr, line_index);

    // 初始化区域
    line_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    // 行高
    line_height = menu_ctrl_ptr->icon_data.item_height;

    // 行区域的top和bottom
    line_rect.top = s_icon_line_pos_ptr[line_index].line_offset_y + menu_ctrl_ptr->offset_y;
    line_rect.bottom = line_rect.top + line_height - 1;

    // 列数
    i = item_index - start_item_index;

    // 计算item的坐标信息
    item_rect.left = menu_ctrl_ptr->icon_data.left_margin + i * menu_ctrl_ptr->icon_data.item_width;
    item_rect.top = line_rect.top;

    item_rect.right = item_rect.left + menu_ctrl_ptr->icon_data.item_width - 1;
    item_rect.bottom = line_rect.bottom;

    return item_rect;
}

/*****************************************************************************/
//  Description : 获取高亮区域
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetHighLightRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index
)
{

#ifdef MMI_PDA_SUPPORT
    GUI_RECT_T item_rect = {0};

    item_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

    return item_rect;
#else
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T icon_rect = {0};

    // 获取total区域
    item_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

    // 获取中心区域，定高和宽
    item_rect = GUI_GetCenterRect(item_rect, MMITHEME_MENU_ITEM_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);

    // 获取中心区域，定最终的宽
    item_rect = GUI_GetCenterRect(item_rect, MENUICON_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);

    item_rect.right = item_rect.left + MENUICON_WIDTH - 1;
    item_rect.bottom = item_rect.top + MENUICON_HEIGHT - 1;

    // 获得icon的中心区域
    icon_rect = GUI_GetCenterRectExt(item_rect, menu_ctrl_ptr->main_menu_theme.focus_item_w, menu_ctrl_ptr->main_menu_theme.focus_item_h);

    return icon_rect;
#endif
}

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuInitMenuDetail(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    uint16 item_num = 0;

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_2506_112_2_18_2_33_31_118, (uint8 *)"");

    menu_ctrl_ptr->is_move_state = FALSE;
    
    if (MMIMAINMENU_MATRIX_TITLE_E == GUIMAINMENU_GetStyle(menu_ctrl_ptr->ctrl_handle))
    {
        menu_ctrl_ptr->is_title_style = TRUE;
    }

    menu_ctrl_ptr->enter_anim_type = GUIMENU_ENTER_ANIM_SLIDE_ICON;
    menu_ctrl_ptr->cur_item_index = menu_ctrl_ptr->main_menu_theme.default_item_index;
    MMITHEME_GetIconMenuInfo( menu_ctrl_ptr->win_handle, &menu_ctrl_ptr->icon_data, menu_ctrl_ptr->is_title_style);

    MatrixMenuGetInfo(menu_ctrl_ptr->win_handle, menu_ctrl_ptr);

    MatrixResetItemListInfo(menu_ctrl_ptr);

    MatrixReadIconMenuItemList(menu_ctrl_ptr, &item_num);

    menu_ctrl_ptr->item_total_num = item_num;

#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuCreateItemLayer(menu_ctrl_ptr);
#endif

    // 获得总行数
    s_total_row_num = MatrixMenuGetTotalLineNum(menu_ctrl_ptr);

    // 申请位置信息内存
    MatrixMenuCreateIconAndTextPosMem(menu_ctrl_ptr);
    MatrixMenuCalIconAndTextPos(menu_ctrl_ptr);

    MMITHEME_GetMainMenuTheme(&menu_ctrl_ptr->main_menu_theme);
    MatrixMenuCreateButton(menu_ctrl_ptr);
#ifdef UI_MULTILAYER_SUPPORT
    GUIWIN_SetStbState(menu_ctrl_ptr->win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
#endif
    GUIWIN_SetStbBgIsDesplay(menu_ctrl_ptr->win_handle, TRUE);
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuCreateRollSrcLayer(menu_ctrl_ptr);
#endif
    MatrixMenuCreateRollEffect();

#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuCreateHomeButtonLayer(menu_ctrl_ptr);
    MatrixMenuCreateActiveItemTitleLayer(menu_ctrl_ptr);
    MatrixMenuSetStatusBarAndHomeButtonLayer(menu_ctrl_ptr);
#endif
#elif defined MMI_PDA_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuCreateHomeButtonLayer(menu_ctrl_ptr);
    MatrixMenuSetHomeButtonLayer(menu_ctrl_ptr);
#endif
#endif

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_2589_112_2_18_2_33_31_119, (uint8 *)"");

    return;
}

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 MatrixMenuGetTotalLineNum(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr
)
{
    uint16      total_line_num = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return total_line_num;
    }

    if (0 != menu_ctrl_ptr->icon_data.page_col_num)
    {
        total_line_num = menu_ctrl_ptr->menu_list_info.item_count / menu_ctrl_ptr->icon_data.page_col_num;

        if (0 != menu_ctrl_ptr->menu_list_info.item_count % menu_ctrl_ptr->icon_data.page_col_num)
        {
            total_line_num++;
        }
    }

    return (total_line_num);
}

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuIsNeedScrollBar(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;
#ifndef MMI_PDA_SUPPORT
    uint16      total_line_num = 0;

    //get line number in one page
    total_line_num = MatrixMenuGetTotalLineNum(menu_ctrl_ptr);

    if (menu_ctrl_ptr->is_need_prgbox &&
        total_line_num > menu_ctrl_ptr->icon_data.page_row_num)
    {
        result = TRUE;
    }
    else
    {
        menu_ctrl_ptr->is_revise_rect = FALSE;
    }

#endif

    return (result);
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetScrollBarRect(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr
)
{
    GUI_RECT_T  menu_rect = {0};
    GUI_RECT_T  prg_rect = {0};

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    //set prg rect
    prg_rect = menu_rect;

    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        prg_rect.right = (int16)(prg_rect.left + MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH - 1);
    }
    else
    {
        prg_rect.left = (int16)(prg_rect.right - MMITHEME_MENUMAIN_SCROLL_BAR_WIDTH + 1);
    }

    return (prg_rect);
}

/*****************************************************************************/
//  Description : display menu scroll bar
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawScrollBar(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr,
    BOOLEAN         is_update
)
{
    //uint16                  line_height = 0;
    uint32                  prg_total_count = 0;
    uint32                  prg_count_in_page = 0;
    uint32                  prg_cur_index = 0;
    uint32                  prg_first_index = 0;
    GUI_RECT_T              prg_rect = {0};
    //GUI_RECT_T              menu_rect = {0};
    MMI_CONTROL_CREATE_T    ctrl_creat = {0};
    GUIPRGBOX_INIT_DATA_T   init_data = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info = {0};

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    //is need scroll bar
    if (MatrixMenuIsNeedScrollBar(menu_ctrl_ptr))
    {
        //get progress bar rect
        prg_rect = MatrixMenuGetScrollBarRect(menu_ctrl_ptr);

        //init
        if (PNULL == menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            //init data
            init_data.lcd_id = lcd_dev_info.lcd_id;
            init_data.both_rect.h_rect = init_data.both_rect.v_rect   = prg_rect;
            init_data.style  = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;

            ctrl_creat.guid               = SPRD_GUI_PRGBOX_ID;
            ctrl_creat.parent_win_handle  = menu_ctrl_ptr->win_handle;
            ctrl_creat.parent_ctrl_handle = menu_ctrl_ptr->ctrl_handle;
            ctrl_creat.init_data_ptr      = &init_data;

            //creat progress bar control
            menu_ctrl_ptr->prgbox_ctrl_ptr = MMK_CreateControl(&ctrl_creat);

            //set progress bar update
            is_update = TRUE;
        }

        if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
        {
            MatrixMenuDrawBg(prg_rect, menu_ctrl_ptr, PNULL);

            GUIPRGBOX_ResizeRectByPtr(menu_ctrl_ptr->prgbox_ctrl_ptr, prg_rect);
            GUIPRGBOX_SetFobidThumbByPtr(menu_ctrl_ptr->prgbox_ctrl_ptr);

            prg_total_count = (uint32)MatrixMenuGetTotalLineNum(menu_ctrl_ptr);
            prg_count_in_page = (uint32)menu_ctrl_ptr->icon_data.page_row_num;

            prg_first_index = (uint32)MatrixMenuGetItemLineIndex(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
            prg_cur_index = (uint32)MatrixMenuGetItemLineIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

            //set progress param
            GUIPRGBOX_SetParamByPtr(
                menu_ctrl_ptr->prgbox_ctrl_ptr,
                prg_total_count,
                prg_count_in_page);

            //set current positon
            GUIPRGBOX_SetPosByPtr(
                is_update,
                prg_cur_index,
                prg_first_index,
                menu_ctrl_ptr->prgbox_ctrl_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"== MatrixMenuDrawScrollBar == prgbox ctrl is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_2804_112_2_18_2_33_31_120, (uint8 *)"");
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

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuResetMoveItemInfo(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->is_move_state) // 记录当前和起始位置
    {
        result = TRUE;
        menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index;
        menu_ctrl_ptr->tmp_first_item_index = menu_ctrl_ptr->first_item_index;
    }

    return result;
}

/*****************************************************************************/
//  Description : 创建移动的buffer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuMoveInit(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;
    GUI_RECT_T icon_rect = {0};

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->is_move_state) // 记录当前和起始位置
    {
        result = TRUE;

        MatrixMenuResetMoveItemInfo(menu_ctrl_ptr);

        MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        MMK_SendMsg(menu_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL);

#ifdef UI_MULTILAYER_SUPPORT

        if (UILAYER_IsMultiLayerEnable())
        {
            MatrixMenuCreateMoveLayer(menu_ctrl_ptr);

            if (menu_ctrl_ptr->is_item_pressed)
            {
                menu_ctrl_ptr->is_moving = TRUE;

                if (UILAYER_IsMultiLayerEnable())
                {
                    icon_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

                    MatrixMenuCreateMoveLayer(menu_ctrl_ptr);
                    MatrixMenuSetBltMoveLayer(menu_ctrl_ptr);
                    MatrixMenuRedrawMoveLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index,
                                              icon_rect);
                }

                // 记录移动的信息
                MatrixMenuResetMoveItemInfo(menu_ctrl_ptr);
            }

        }
        else
#endif
        {
            menu_ctrl_ptr->is_move_state = FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 确认移动
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuConfirmMoveMenuItem(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
)
{
    BOOLEAN is_handled = FALSE;

    if (menu_ctrl_ptr->is_move_state)
    {
#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
        // 释放层
        MatrixMenuReleaseMoveIconLayer(menu_ctrl_ptr);
#endif
#endif
        // 设置移动状态
        menu_ctrl_ptr->is_move_state = FALSE;
        menu_ctrl_ptr->is_moving = FALSE;

        MatrixMenuDraw(menu_ctrl_ptr);

        MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);

        is_handled = TRUE;
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_2962_112_2_18_2_33_32_121, (uint8 *)"");
    }

    return is_handled;
}
#endif

#ifndef MMI_RES_LOW_COST
/*****************************************************************************/
//  Description : get softkey text id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuGetSoftkeyText(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    GUI_BUTTON_T      *button_id_ptr, //out
    MMI_TEXT_ID_T     *softkey_id_ptr //in
)
{
    BOOLEAN         is_null_softkey = TRUE;
    uint16          i = 0;
    MMI_TEXT_ID_T   *softkey_text_id_arr = PNULL;

    if (
        (PNULL == softkey_id_ptr) ||
        (PNULL == button_id_ptr)
    )
    {
        return;
    }

    //judge all softkey is null
    for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
    {
        if (!MMI_IsTextNull(softkey_id_ptr[i]))
        {
            is_null_softkey = FALSE;
            break;
        }
    }

    //set default softkey text
    if (menu_ctrl_ptr->is_move_state)
    {
        softkey_text_id_arr = (MMI_TEXT_ID_T *)s_default_softkey2_text_id;
        menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_RIGHTSK_NEED;
    }
    else if (is_null_softkey)
    {
#ifndef TOUCHPANEL_TYPE_NONE
        softkey_text_id_arr = (MMI_TEXT_ID_T *)s_default_softkey_text_id;
        menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_MIDSK_NEED | GUIMENU_RIGHTSK_NEED;
#else
        softkey_text_id_arr = (MMI_TEXT_ID_T *)s_default_softkey3_text_id;
        menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_RIGHTSK_NEED;
#endif
    }
    else
    {
        menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_MIDSK_NEED | GUIMENU_RIGHTSK_NEED;
        softkey_text_id_arr = softkey_id_ptr;
    }

    for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
    {
        button_id_ptr[i].content.data_u.text_id = softkey_text_id_arr[i];
    }
}

/*****************************************************************************/
//  Description : set menu softkey param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetSoftkey(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    GUI_COLOR_T     font_color = 0;
    GUI_BUTTON_T    button_arr[GUISOFTKEY_BUTTON_NUM] = {0};
    MMI_TEXT_ID_T   *softkey_id_ptr = PNULL;
    MMI_IMAGE_ID_T  bg_id = IMAGE_SOFTKEY_BG;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    //set default softkey text
    if (menu_ctrl_ptr->is_move_state)
    {
        softkey_id_ptr = (MMI_TEXT_ID_T *)s_default_softkey2_text_id;
        menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_RIGHTSK_NEED;
    }
    else
    {
#ifndef TOUCHPANEL_TYPE_NONE
        softkey_id_ptr = (MMI_TEXT_ID_T *)s_default_softkey_text_id;
        menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_MIDSK_NEED | GUIMENU_RIGHTSK_NEED;
#else
        softkey_id_ptr = (MMI_TEXT_ID_T *)s_default_softkey3_text_id;
        menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_RIGHTSK_NEED;
#endif
    }

    MatrixMenuGetSoftkeyText(menu_ctrl_ptr, button_arr, softkey_id_ptr);

    font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);

    GUIWIN_SetSoftkeyStyle(menu_ctrl_ptr->win_handle,
                           font_color,
                           GUI_SOFTKEY_BG_IMAGE_ONLY,
                           0,
                           bg_id);

    GUIWIN_SetSoftkeyTextId(menu_ctrl_ptr->win_handle,
                            button_arr[0].content.data_u.text_id,
                            button_arr[1].content.data_u.text_id,
                            button_arr[2].content.data_u.text_id,
                            TRUE);
}

#else
/*****************************************************************************/
//  Description : set menu softkey param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetSoftkey(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    GUI_COLOR_T     font_color = 0;
    MMI_IMAGE_ID_T  bg_id = IMAGE_SOFTKEY_BG;

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    menu_ctrl_ptr->softkey_state = GUIMENU_LEFTSK_NEED | GUIMENU_MIDSK_NEED | GUIMENU_RIGHTSK_NEED;

    font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);

    GUIWIN_SetSoftkeyStyle(menu_ctrl_ptr->win_handle,
                           font_color,
                           GUI_SOFTKEY_BG_IMAGE_ONLY,
                           0,
                           bg_id);

    GUIWIN_SetSoftkeyTextId(menu_ctrl_ptr->win_handle,
                            TXT_COMMON_OK,
                            TXT_NULL,
                            STXT_RETURN,
                            TRUE);
}
#endif

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuModifyRect(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    int16 offset_y = 0;
    
    //destroy progress
    if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
    {
        MMK_DestroyControl(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
        menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
    }
    
    //set icon menu current index
    MMITHEME_GetIconMenuInfo(menu_ctrl_ptr->win_handle, &menu_ctrl_ptr->icon_data, menu_ctrl_ptr->is_title_style);

#ifdef UI_MULTILAYER_SUPPORT
    //释放高亮层
    MatrixMenuReleaseHighlightLayer(menu_ctrl_ptr);
    MatrixMenuCreateHighlightLayer(menu_ctrl_ptr);
    //
    MatrixMenuReleaseItemLayer(menu_ctrl_ptr);
    MatrixMenuCreateItemLayer(menu_ctrl_ptr);
#endif

    // 获得总行数
    s_total_row_num = MatrixMenuGetTotalLineNum(menu_ctrl_ptr);

    // 申请位置信息内存
    MatrixMenuCreateIconAndTextPosMem(menu_ctrl_ptr);
    MatrixMenuCalIconAndTextPos(menu_ctrl_ptr);

    MatrixMenuAdjuestOffsetY(menu_ctrl_ptr, &offset_y, 0);

    menu_ctrl_ptr->offset_y += offset_y;

    MatrixMenuCreateButton(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
    GUIWIN_SetStbState(menu_ctrl_ptr->win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
#endif
    GUIWIN_SetStbBgIsDesplay(menu_ctrl_ptr->win_handle, TRUE);
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuReleaseRollLayer();
    MatrixMenuCreateRollSrcLayer(menu_ctrl_ptr);
    MatrixMenuReleaseStatusBarAndHomeButtonLayer();
    MatrixMenuCreateHomeButtonLayer(menu_ctrl_ptr);
    MatrixMenuCreateActiveItemTitleLayer(menu_ctrl_ptr);
    MatrixMenuSetStatusBarAndHomeButtonLayer(menu_ctrl_ptr);
#endif
#elif defined MMI_PDA_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuReleaseHomeButtonLayer();
    MatrixMenuCreateHomeButtonLayer(menu_ctrl_ptr);
    MatrixMenuSetHomeButtonLayer(menu_ctrl_ptr);
#endif
#endif

    return;
}

#if defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 创建button
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateButton(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    uint16              icon_width = 0;
    uint16              icon_height = 0;
    GUI_RECT_T          disp_rect = {0};

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->main_menu_theme.is_has_button)
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_COMMON_HOME_ICON, menu_ctrl_ptr->win_handle);

        disp_rect = MMITHEME_GetIconMenuButtonRect();

        disp_rect = GUI_GetCenterRectExt(disp_rect, icon_width, icon_height);

        if (PNULL == menu_ctrl_ptr->icon_data.data_ptr)
        {
            GUIBUTTON_INIT_DATA_T  init_data = {0};
            MMI_CONTROL_CREATE_T    ctrl_create = {0};

            //init
            init_data.bg.bg_type = GUI_BG_IMG;
            init_data.bg.img_id  = IMAGE_COMMON_HOME_ICON;

            ctrl_create.ctrl_id            = 0;
            ctrl_create.guid               = SPRD_GUI_BUTTON_ID;
            ctrl_create.init_data_ptr      = &init_data;
            ctrl_create.parent_ctrl_handle = menu_ctrl_ptr->ctrl_handle;
            ctrl_create.parent_win_handle  = menu_ctrl_ptr->win_handle;

            //creat control
            menu_ctrl_ptr->icon_data.data_ptr = (void *)MMK_CreateControl(&ctrl_create);
        }

        IGUICTRL_SetRect(((IGUICTRL_T *)(menu_ctrl_ptr->icon_data.data_ptr)), &disp_rect);
    }

    return;
}
#else
/*****************************************************************************/
//  Description : handle button1 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MatrixMenuHandleButton1Click(MMI_HANDLE_T button_handle)
{
    GUICTRL_PostNotify(button_handle, MSG_NOTIFY_PENOK);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MatrixMenuHandleButton2Click(MMI_HANDLE_T button_handle)
{
    GUICTRL_PostNotify(button_handle, MSG_NOTIFY_CANCEL);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : display softkey bg
//  Global resource dependence :
//  Author:nan ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplaysoftkeybg(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO  *lcd_dev_info_ptr,
    BOOLEAN is_left
)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_RECT_T      softkey_rect = {0};
    GUI_LCD_DEV_INFO    *dst_lcd_dev_info_ptr = lcd_dev_info_ptr;
    GUI_LCD_DEV_INFO    *menu_lcd_dev_info = {0};

    if (PNULL == dst_lcd_dev_info_ptr)
    {
        GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr, &menu_lcd_dev_info);
        dst_lcd_dev_info_ptr = &menu_lcd_dev_info;
    }

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if (is_left)
    {
        softkey_rect.left   = 0;
        softkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
        softkey_rect.right  = (int16)(lcd_width / 2 - 1);
        softkey_rect.bottom = (int16)(lcd_height - 1);
    }
    else
    {
        softkey_rect.left   = (int16)(lcd_width / 2);
        softkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
        softkey_rect.right  = (int16)(lcd_width - 1);
        softkey_rect.bottom = (int16)(lcd_height - 1);
    }
#if !defined MMI_MAINMENU_MINI_SUPPORT
    GUIRES_DisplayImg(PNULL,
                      &softkey_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      IMAGE_NULL,//IMAGE_MAINMENU_SOFTKEY_BG,
                      (const GUI_LCD_DEV_INFO *)dst_lcd_dev_info_ptr);
#else
    GUIRES_DisplayImg(PNULL,
                      &softkey_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      IMAGE_NULL,
                      (const GUI_LCD_DEV_INFO *)dst_lcd_dev_info_ptr);
#endif
}

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawHandleButton(
    GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr
)
{
    if (PNULL != owner_draw_ptr)
    {
        MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByGuid(MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle), SPRD_GUI_MAINMENU_ID);
        MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr = (MMIMAINMENU_MATRIX_DATA_T *)CTRLMAINMENU_GetPM(ctrl_handle);

        if (PNULL != menu_ctrl_ptr && !menu_ctrl_ptr->is_title_style)
        {
            GUI_RECT_T        display_rect = owner_draw_ptr->display_rect;
            BOOLEAN           is_left = 0;
            uint16            width  = 0;
            uint16            height = 0;
            GUI_LCD_DEV_INFO  lcd_dev_info = {0};

            GUIAPICTRL_GetLcdDevInfo(ctrl_handle, &lcd_dev_info);

            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &width, &height);

            if (0 == owner_draw_ptr->display_rect.left)
            {
                display_rect.right = width / 2 - 1;
                is_left = TRUE;
            }
            else if ((width - 1) == owner_draw_ptr->display_rect.right)
            {
                display_rect.left = width / 2;
                is_left = FALSE;
            }

#ifdef UI_MULTILAYER_SUPPORT
            UILAYER_ClearRect(&lcd_dev_info, display_rect);
#endif
            MatrixMenuDrawBg(display_rect, menu_ctrl_ptr, PNULL);
            MatrixMenuDisplaysoftkeybg(menu_ctrl_ptr, &lcd_dev_info, is_left);
            GUIBUTTON_UpdateText(owner_draw_ptr->ctrl_handle);
        }
    }
}

/*****************************************************************************/
//  Description : 创建button
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateButton(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    uint16              i = 0;
    GUI_RECT_T          button_rect[GUIMENU_BUTTON_NUM] = {0};
    GUIBUTTON_INIT_DATA_T  init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    IGUICTRL_T          *button_ptr = PNULL;
    MMI_TEXT_ID_T       button_text[GUIMENU_BUTTON_NUM] = {0};
    GUIBUTTON_CALLBACK_FUNC_EXT button_func[GUIMENU_BUTTON_NUM] = {PNULL};
    GUIBUTTON_OWNER_DRAW_FUNC button_draw_func[GUIMENU_BUTTON_NUM] = {PNULL};
    GUI_FONT_ALL_T     font_info = {0};
    GUI_LCD_DEV_INFO   button_lcd = {0};

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->main_menu_theme.is_has_button)
    {
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
        font_info.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);
#else
        font_info.color = ~MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);
#endif
        font_info.font = MMITHEME_GetSoftKeyTextFont();

        button_rect[0] = MMITHEME_GetSoftkeyRect(LEFT_BUTTON);
        button_rect[1] = MMITHEME_GetSoftkeyRect(RIGHT_BUTTON);
        button_text[0] = TXT_COMMON_OK;
        button_text[1] = STXT_RETURN;
        button_func[0] = MatrixMenuHandleButton1Click;
        button_func[1] = MatrixMenuHandleButton2Click;
        button_draw_func[0] = MatrixMenuDrawHandleButton;
        button_draw_func[1] = MatrixMenuDrawHandleButton;

        for (i = 0; i < GUIMENU_BUTTON_NUM; i++)
        {
            if (0 != menu_ctrl_ptr->button_handle[i])
            {
                MMK_DestroyControl(menu_ctrl_ptr->button_handle[i]);

                menu_ctrl_ptr->button_handle[i] = 0;
            }

            //init
            init_data.bg.bg_type = GUI_BG_IMG;

            ctrl_create.ctrl_id            = 0;
            ctrl_create.guid               = SPRD_GUI_BUTTON_ID;
            ctrl_create.init_data_ptr      = &init_data;
            ctrl_create.parent_ctrl_handle = menu_ctrl_ptr->ctrl_handle;
            ctrl_create.parent_win_handle  = menu_ctrl_ptr->win_handle;

            //creat control
            button_ptr = MMK_CreateControl(&ctrl_create);

            menu_ctrl_ptr->button_handle[i] = GUICTRL_GetCtrlHandle(button_ptr);

            GUIBUTTON_SetRunSheen(menu_ctrl_ptr->button_handle[i] , TRUE);

            if (i == 0)
            {
                GUIBUTTON_SetTextAlign(menu_ctrl_ptr->button_handle[i], ALIGN_LVMIDDLE);
            }
            else if (1 == i)
            {
                GUIBUTTON_SetTextAlign(menu_ctrl_ptr->button_handle[i], ALIGN_RVMIDDLE);
            }
            else
            {
                GUIBUTTON_SetTextAlign(menu_ctrl_ptr->button_handle[i], ALIGN_HVMIDDLE);
            }

            GUIBUTTON_SetFont(menu_ctrl_ptr->button_handle[i], &font_info);

            GUIBUTTON_SetTextId(menu_ctrl_ptr->button_handle[i], button_text[i]);

#ifdef UI_MULTILAYER_SUPPORT

            if (UILAYER_IsLayerActive(&s_item_layer_handle))
            {
                button_lcd = s_item_layer_handle;
            }
            else
#endif
            {
                GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &button_lcd);
            }

            GUIAPICTRL_SetLcdDevInfo(menu_ctrl_ptr->button_handle[i], &button_lcd);

            GUIBUTTON_SetCallBackFuncExt(menu_ctrl_ptr->button_handle[i], button_func[i]);
            GUIBUTTON_SetOwnerDraw(menu_ctrl_ptr->button_handle[i], button_draw_func[i]);

            GUIAPICTRL_SetRect(menu_ctrl_ptr->button_handle[i], &button_rect[i]);
        }
    }

    return;
}

#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuClearBackGround(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
#ifdef UI_MULTILAYER_SUPPORT

    if (!UILAYER_IsLayerActive(&s_item_layer_handle))
#endif
    {
        GUI_RECT_T menu_rect = {0};

        GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &menu_rect);

        MatrixMenuDrawBg(menu_rect, menu_ctrl_ptr, PNULL);
    }
}
#endif

/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDraw(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO                lcd_dev_info      = {0};
    //MMISET_WALLPAPER_SET_T          wallpaper_info    = {0};
    GUI_RECT_T                      rect              = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_state)
    {
        return;
    }

#ifdef DYNAMIC_MAINMENU_SUPPORT

    // 组织状态下，不需要reload
    if (!menu_ctrl_ptr->is_move_state && MMIMENU_DyCheckIsSyn())//check信息有变化
    {
        MatrixMenuReloadItem(menu_ctrl_ptr);
    }

#endif

    menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;
    MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);

    GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &rect);
    rect.top = 0;
    MatrixMenuDrawBg(rect, menu_ctrl_ptr, PNULL);

    //display title
    if (MMIMAINMENU_MATRIX_TITLE_E == MMIAPISET_GetMainmenuStyle())
    {
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuDisplayActiveItemTitleEx(menu_ctrl_ptr);
#endif
#else
    MatrixMenuDisplayActiveItemTitle(menu_ctrl_ptr);
#endif
    }

    //display scroll
#ifndef ROLL_MAINMENU_SUPPORT
    MatrixMenuDrawScrollBar(menu_ctrl_ptr, TRUE);
#endif

    //set softkey param
    MatrixMenuSetSoftkey(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT

    //Create highlight
    if (UILAYER_IsMultiLayerEnable())
    {
        MatrixMenuCreateHighlightLayer(menu_ctrl_ptr);
    }

#endif

    //Create roll layer
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT

    if (s_rollSrc_layer_handle.block_id == UILAYER_NULL_HANDLE)
    {
        MatrixMenuCreateRollSrcLayer(menu_ctrl_ptr);
    }

#endif
#endif

#ifdef UI_MULTILAYER_SUPPORT

    //display icon item
    if (UILAYER_IsLayerActive(&s_item_layer_handle))
    {
        if (menu_ctrl_ptr->is_move_state || (menu_ctrl_ptr->main_menu_theme.is_hl_disappear  && MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetTiggerMode()))
        {
            MatrixMenuSetBltIconMenuDispLayer(menu_ctrl_ptr, s_item_layer_handle.block_id, 0);
        }
        else
        {
            MatrixMenuSetBltIconMenuDispLayer(menu_ctrl_ptr, menu_ctrl_ptr->highlight_layer_handle.block_id, s_item_layer_handle.block_id);
        }

        //Append StatusBar & HomeBotton
#ifdef ROLL_MAINMENU_SUPPORT
        MatrixMenuAppendStatusBarAndHomeButtonLayer();
#elif defined MMI_PDA_SUPPORT
        MatrixMenuAppendHomeButtonLayer();
#endif
        lcd_dev_info.block_id = s_item_layer_handle.block_id;
    }

    if (UILAYER_IsLayerActive(&s_item_layer_handle)) // cp buffer
    {
#ifdef ROLL_MAINMENU_SUPPORT
        MatrixMenuRenderRollEffect(menu_ctrl_ptr);
#else
        MatrixMenuBltItemLayerToMenuLayer(menu_ctrl_ptr);
#endif
    }
    else // 直接画
#endif
    {
        MatrixMenuDrawItemIcon(menu_ctrl_ptr, &lcd_dev_info);
#ifdef ROLL_MAINMENU_SUPPORT // 此处强行刷新状态栏
        IGUICTRL_HandleEvent((IGUICTRL_T *)MMK_GetCtrlPtrByWin(menu_ctrl_ptr->win_handle, MMITHEME_GetStatusBarCtrlId()), MSG_CTL_PAINT, PNULL);  // 这里之所以update状态栏，是因为在ROLL_MAINMENU_SUPPORT打开的时候，菜单失去焦点时刷新覆盖了状态栏
#endif
    }

    if (!menu_ctrl_ptr->is_move_state && (!menu_ctrl_ptr->main_menu_theme.is_hl_disappear  || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        MatrixMenuDrawHighlightItem(menu_ctrl_ptr);
    }

    return;
}

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 重新加载主菜单
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReloadItem(MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr)
{
    if (PNULL != menu_ctrl_ptr)
    {
        MatrixMenuDestory(menu_ctrl_ptr);  // 释放
        MatrixMenuInitMenuDetail(menu_ctrl_ptr);
    }

    return;
}
#endif

/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplayIconSpecialMenu(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
#ifdef UI_MULTILAYER_SUPPORT
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
#endif
    GUI_RECT_T       rect = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &rect);

    menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;

#ifdef UI_MULTILAYER_SUPPORT

    if (UILAYER_IsLayerActive(&s_item_layer_handle))
    {
        lcd_dev_info.block_id = s_item_layer_handle.block_id;
    }

#endif

#ifdef TOUCH_PANEL_SUPPORT

    if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_state)
    {
        return;
    }

#endif
    // 变频
#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_BT_A2DP);
#endif

    // 设置合并的层，这里只需要把第一个page的层合并即可
#ifdef UI_MULTILAYER_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT

    if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)
    {
        GUI_RECT_T tmp_rect = {0};

        //BOOLEAN    is_true = FALSE;

        tmp_rect = menu_ctrl_ptr->move_rect;

        MatrixMenuSetBltIconMenuDispLayer(menu_ctrl_ptr, s_item_layer_handle.block_id, menu_ctrl_ptr->icon_move_layer_handle.block_id);

        // 清空
        MMITHEME_ClearRect(&lcd_dev_info, &rect);


        MatrixMenuBltItemLayerToMenuLayer(menu_ctrl_ptr);

        tmp_rect = MatrixMenuGetIconMenuMoveLayerRect(menu_ctrl_ptr);

        UILAYER_SetLayerPosition(&menu_ctrl_ptr->icon_move_layer_handle, tmp_rect.left, tmp_rect.top);
    }
    else
#endif
    {
        //BOOLEAN is_true = FALSE;

        MatrixMenuSetBltIconMenuDispLayer(menu_ctrl_ptr, s_item_layer_handle.block_id, 0);

        // 清空
        MMITHEME_ClearRect(&lcd_dev_info, &rect);

        MatrixMenuBltItemLayerToMenuLayer(menu_ctrl_ptr);
    }

#endif

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif

    return;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawItem(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO *dst_dev_ptr,
    uint16                 item_index,
    BOOLEAN                is_compound
)
{
    GUI_LCD_DEV_INFO item_layer = {0};
    CTRLMAINMENU_ITEM_T menuitem_info = {0};
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T clip_rect = {0};
    GUIMENU_DISPLAY_ITEM_PARAM_T param = {0};

    if (PNULL == menu_ctrl_ptr || PNULL == dst_dev_ptr)
    {
        return;
    }

    disp_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);
    disp_rect = GUI_GetCenterRect(disp_rect, MMITHEME_MENU_ITEM_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);
    clip_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    if (!GUI_IntersectRect(&clip_rect, disp_rect, clip_rect))
    {
        return;
    }

    MatrixMenuGetItemInfo(menu_ctrl_ptr, item_index, &menuitem_info);

    if (DATA_TYPE_RGB565 == UILAYER_GetLayerColorType(dst_dev_ptr))
    {
        is_compound = TRUE;
    }

    if (is_compound)
    {
        item_layer = MMITHEME_DrawMainmenuItem(menu_ctrl_ptr->win_handle, &menuitem_info, PNULL);

        UILAYER_SetLayerPosition(&item_layer, disp_rect.left, disp_rect.top);
        UILAYER_BltLayerToLayer(dst_dev_ptr, &item_layer, &clip_rect, is_compound);
    }
    else
    {
        param.format |= GUIMENU_DISPLAY_ITEM_FORMAT_TO_LAYER;
        param.dst_layer = *dst_dev_ptr;
        param.dst_rect  = disp_rect;
        param.clip_rect_ptr = &clip_rect;

        item_layer = MMITHEME_DrawMainmenuItem(menu_ctrl_ptr->win_handle, &menuitem_info, &param);
    }
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuBltItemLayerToMenuLayer(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
)
{
    int16               row = 0; // 行
    int16               line_index = 0; // 列
    int16               start_item_index = 0; // 列
    int16               item_index = 0;
    GUI_RECT_T          disp_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          menu_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    IMG_EnableTransparentColor(TRUE);

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    if (UILAYER_IsLayerActive(&s_item_layer_handle))
    {
        lcd_dev_info = s_item_layer_handle;
    }
    else
    {
        CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);;
    }

    //若无回写才需要擦图层
    if (GUI_IsZeroRect(menu_ctrl_ptr->writeback_rect))
    {
        GUI_RECT_T rect = {0};
        GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &rect);
        MMITHEME_ClearRect(&lcd_dev_info, &rect);
    }

    MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);

    for (line_index = s_start_line_index; line_index <= s_end_line_index; line_index++)
    {
        start_item_index = MatrixMenuGetLineStartItemIndex(menu_ctrl_ptr, line_index);

        for (row = 0; row < menu_ctrl_ptr->icon_data.page_col_num; row++)
        {
            item_index = start_item_index + row;

            if (item_index == menu_ctrl_ptr->tmp_cur_item_index && menu_ctrl_ptr->is_moving)
            {
                continue;
            }

            if (item_index < menu_ctrl_ptr->item_total_num)
            {
                // 计算显示区域
                disp_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

                if ((int32)menu_ctrl_ptr->menu_list_info.item_count > (int32)item_index && GUI_IntersectRect(&cross_rect, disp_rect, menu_rect))
                {
                    BOOLEAN is_need_draw = TRUE;
                    GUI_RECT_T cur_rect = {0};

                    //如果做了内存回写加速
                    if (! GUI_IsZeroRect(menu_ctrl_ptr->writeback_rect))
                    {
                        //if the the item in the writeback rect
                        if (GUI_IntersectRect(&cur_rect, cross_rect, menu_ctrl_ptr->writeback_rect))
                        {
                            //判断边界item是否是完整的，如果不是完整的需要将拷贝的区域重置
                            if (!GUI_RectIsCovered(menu_ctrl_ptr->writeback_rect, cross_rect))
                            {
                                UILAYER_ClearRect(&lcd_dev_info, cur_rect);
                            }
                            else
                            {
                                is_need_draw = FALSE;
                            }
                        }
                    }

                    if (is_need_draw)
                    {
                        MatrixMenuDrawItem(menu_ctrl_ptr, &lcd_dev_info, item_index, FALSE);
                    }
                }
            }
        }

        MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);
    }

    IMG_EnableTransparentColor(FALSE);
}
#endif

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawItemIcon(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
)
{
    int16               row = 0;
    int16               start_item_index = 0;
    int16               line_index = 0;
    uint16              item_index = 0;
    GUI_RECT_T          disp_rect = {0};
    GUI_RECT_T          icon_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          menu_rect = {0};
    CTRLMAINMENU_ITEM_T menuitem_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    IMG_EnableTransparentColor(TRUE);

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    for (line_index = s_start_line_index; line_index <= s_end_line_index; line_index++)
    {
        start_item_index = MatrixMenuGetLineStartItemIndex(menu_ctrl_ptr, line_index);

        for (row = 0; row < menu_ctrl_ptr->icon_data.page_col_num; row++)
        {
            item_index = start_item_index + row;

            if (item_index == menu_ctrl_ptr->tmp_cur_item_index
                && menu_ctrl_ptr->is_moving)
            {
                continue;
            }

            if (item_index < menu_ctrl_ptr->item_total_num)
            {
                // 计算显示区域
                disp_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);
#ifdef UI_MULTILAYER_SUPPORT

                if (!UILAYER_IsMultiLayerEnable())
                {
                    //display active icon
                    if (!menu_ctrl_ptr->is_move_state && item_index == menu_ctrl_ptr->cur_item_index)
                    {
                        if (!menu_ctrl_ptr->is_moving) // 正在移动的时候，不需要画高亮
                        {
                            disp_rect = GUI_GetCenterRectExt(disp_rect, MENUICON_WIDTH, MENUICON_HEIGHT);

                            MatrixMenuGetItemInfo(menu_ctrl_ptr, item_index, &menuitem_info);

                            if (!GUI_IntersectRect(&cross_rect, disp_rect, menu_rect))
                            {
                                break;
                            }

                            icon_rect = MatrixMenuGetIconDispRect(disp_rect, cross_rect, MENUICON_WIDTH, MENUICON_HEIGHT);

                            GUIRES_DisplayImg(PNULL,
                                              &cross_rect,
                                              &icon_rect,
                                              menu_ctrl_ptr->win_handle,
                                              menuitem_info.active_icon.data.icon_id,
                                              (const GUI_LCD_DEV_INFO *)lcd_dev_info_ptr);

                        }
                    }
                }

                MatrixMenuDrawItem(menu_ctrl_ptr, lcd_dev_info_ptr, item_index, FALSE);
#else
                disp_rect = GUI_GetCenterRectExt(disp_rect,
                                                 MENUICON_WIDTH,
                                                 MENUICON_HEIGHT);

                if (!GUI_IntersectRect(&cross_rect, disp_rect, menu_rect))
                {
                    break;
                }

                icon_rect = MatrixMenuGetIconDispRect(disp_rect, cross_rect, MENUICON_WIDTH, MENUICON_HEIGHT);
                MatrixMenuGetItemInfo(menu_ctrl_ptr, item_index, &menuitem_info);

                MMIMAINMENU_DrawItemIcon(menu_ctrl_ptr->win_handle, &menuitem_info, lcd_dev_info_ptr, &icon_rect, &disp_rect);
#endif
            }
        }
    }

    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuCancelKey(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
)
{
    BOOLEAN is_handled = FALSE;

    if (menu_ctrl_ptr->is_move_state)
    {
        MatrixMenuInitMenuDetail(menu_ctrl_ptr);

#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
        // 释放层
        MatrixMenuReleaseMoveIconLayer(menu_ctrl_ptr);
#endif
#endif
        // 设置移动状态
        menu_ctrl_ptr->is_move_state = FALSE;
        menu_ctrl_ptr->is_moving = FALSE;

        is_handled = TRUE;
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_4715_112_2_18_2_33_35_124, (uint8 *)"");
    }

    if (!is_handled)
    {
        //notify parent control or parent window
        MatrixMenuNotifyParentMsg(menu_ctrl_ptr->ctrl_handle, MSG_APP_CANCEL);
    }

    return is_handled;
}


#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetIconMenuMoveLayerRect(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    GUI_RECT_T tmp_rect = {0};
    GUI_POINT_T point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;

#ifdef TOUCH_PANEL_SUPPORT
    MMK_GetLogicTPMsg(&state, &point);
#endif
    tmp_rect.left = MAX(point.x - menu_ctrl_ptr->tmp_width, 0);
    tmp_rect.top = MAX(point.y - menu_ctrl_ptr->tmp_height, 0);
    tmp_rect.right = tmp_rect.left + MENUICON_WIDTH - 1;
    tmp_rect.bottom = tmp_rect.top + MENUICON_HEIGHT - 1;

    return tmp_rect;
}
#endif
#endif

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuOk(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
)
{
    BOOLEAN is_handled = FALSE;
#ifdef TOUCH_PANEL_SUPPORT
    //GUIMENU_ITEM_BUF_T item_buf = {0};
    MMI_LINK_NODE_T *pre_cur_item_node_ptr = PNULL;
    MMI_LINK_NODE_T *cur_item_node_ptr = PNULL;

    if (menu_ctrl_ptr->is_moving && menu_ctrl_ptr->is_move_state) // 正在移動狀態
    {
        is_handled = TRUE;

        pre_cur_item_node_ptr = MatrixMenuSearchMenuItemByIndex(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index);
        cur_item_node_ptr = MatrixMenuSearchMenuItemByIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        if (menu_ctrl_ptr->cur_item_index != menu_ctrl_ptr->tmp_cur_item_index) // 相等的时候无变化
        {
            menu_ctrl_ptr->menu_list_info.list_head_ptr = MMILINK_RemoveNode(pre_cur_item_node_ptr, menu_ctrl_ptr->menu_list_info.list_head_ptr);

            if (menu_ctrl_ptr->cur_item_index > menu_ctrl_ptr->tmp_cur_item_index)
            {
                MMILINK_AddNodeAfterBaseNode(cur_item_node_ptr, pre_cur_item_node_ptr);
            }
            else if (menu_ctrl_ptr->cur_item_index < menu_ctrl_ptr->tmp_cur_item_index)
            {
                MMILINK_AddNodeBeforeBaseNode(cur_item_node_ptr, pre_cur_item_node_ptr);

                // 改变头位置
                if (menu_ctrl_ptr->menu_list_info.list_head_ptr == cur_item_node_ptr)
                {
                    menu_ctrl_ptr->menu_list_info.list_head_ptr = pre_cur_item_node_ptr;
                }
            }
        }

        // 移动结束
        menu_ctrl_ptr->is_moving = FALSE;
        // 删除层的影响

#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
        MatrixMenuReleaseMoveIconLayer(menu_ctrl_ptr);
#endif
#endif

        // 由于这个时候需要清楚层，又要刷新softkey，如果不全刷，则状态栏不刷新，会有显示问题，因此需要send full paint
        MMK_PostMsg(menu_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL, NULL);
    }
    else
#endif
    {
        //SCI_TRACE_LOW:"HandleMatrixMenuOk msg_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_5233_112_2_18_2_33_36_125, (uint8 *)"d", msg_id);

        switch (msg_id)
        {
            case MSG_APP_WEB:
                if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
                {
                    is_handled = TRUE;
                    MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
                    break;
                }

                if (menu_ctrl_ptr->is_move_state)
                {
                    is_handled = FALSE;
                }

                break;

            case MSG_APP_OK:
                if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode() && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())
                {
                    is_handled = TRUE;
                    MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
                    break;
                }

                is_handled = FALSE;
                break;

            default:
                is_handled = FALSE;
                break;
        }
    }

    return is_handled;
}

/*****************************************************************************/
//  Description : reset list display item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuResetDisplayItem(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    int16  *offset_y_ptr
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    return MatrixMenuAdjuestOffsetY(menu_ctrl_ptr, offset_y_ptr, MatrixMenuGetItemLineIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index));
}

/*****************************************************************************/
//  Description : handle icon menu direction key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuDirectionKey(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
)
{
    int16           next_item_index  = 0;
    int16           offset_y  = 0;
    CTRLMAINMENU_ITEM_T menuitem_info = {0};
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
    SE_RECT_T       se_rect     = {0};
    GUI_RECT_T      disp_rect     = {0};
    BOOLEAN         is_support_3d = FALSE;
#endif

    MatrixMenuGetItemInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &menuitem_info);

#ifdef ROLL_MAINMENU_SUPPORT
    s_is_all = ROLL_DRAW_NONE;
#endif

    next_item_index = MatrixMenuGetNextItemIndex(menu_ctrl_ptr, msg_id);

    if (MatrixMenuResetDisplayItem(menu_ctrl_ptr, &offset_y))
    {
        if (offset_y != 0)
        {
            menu_ctrl_ptr->offset_y += offset_y;

            MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);
            // 显示
            MatrixMenuDraw(menu_ctrl_ptr);
            MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);
        }

        return;
    }

    MatrixMenuAdjuestOffsetY(menu_ctrl_ptr, &offset_y, MatrixMenuGetItemLineIndex(menu_ctrl_ptr, next_item_index));

    if (0 == offset_y)
    {
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        disp_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        is_support_3d = (menu_ctrl_ptr->main_menu_theme.is_hl_disappear  && (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetTiggerMode())) ? FALSE : TRUE;

        if (is_support_3d && !menu_ctrl_ptr->is_move_state)
        {
            if (CTRL_ICON_ID == menuitem_info.active_icon.type)
            {
                se_rect = MatrixMenuSavePrevIcon(
                              menu_ctrl_ptr->win_handle,
                              menuitem_info.active_icon.data.icon_id,
                              disp_rect,
                              msg_id
                          );
            }
        }
#endif

        //reset icon display
        MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        //save icon bg
        if (is_support_3d && !menu_ctrl_ptr->is_move_state && MMIAPI3D_IsIconMenuAnimOpen())
        {
            MMIAPI3D_SaveMenuIcon(PREV_BG_BUF, &se_rect);

            disp_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

            se_rect.left   = disp_rect.left;
            se_rect.top    = disp_rect.top;
            se_rect.right  = disp_rect.right;
            se_rect.bottom = disp_rect.bottom;
            //save next unselected icon for lucky board.
            MMIAPI3D_SaveMenuIcon(NEXT_BG_BUF, &se_rect);
        }
#endif

        menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;
        //get the next item index
        menu_ctrl_ptr->cur_item_index = next_item_index;

        if (MatrixMenuResetDisplayItem(menu_ctrl_ptr, &offset_y))
        {
            menu_ctrl_ptr->offset_y += offset_y;

            MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);

            MatrixMenuDraw(menu_ctrl_ptr);
            MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);
        }

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        if (is_support_3d && !menu_ctrl_ptr->is_move_state)
        {
            MatrixMenuSaveNextMenuIcon(menu_ctrl_ptr);
        }
#endif

#ifdef TOUCH_PANEL_SUPPORT

        if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)
        {
#ifdef UI_MULTILAYER_SUPPORT

            if (UILAYER_IsMultiLayerEnable())
            {
                MatrixMenuCreateMoveLayer(menu_ctrl_ptr);
                MatrixMenuSetBltMoveLayer(menu_ctrl_ptr);
                MatrixMenuRedrawMoveLayer(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index,
                MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index));
            }
            else
#endif
            {
                MatrixMenuDraw(menu_ctrl_ptr);
                MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);
            }
        }
        else
#endif
        {
            //display active icon
            MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
        }
    }
    else
    {
        // reset icon display
        MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;
        // get the next item index
        menu_ctrl_ptr->cur_item_index = next_item_index;

        if (MatrixMenuResetDisplayItem(menu_ctrl_ptr, &offset_y))
        {
            menu_ctrl_ptr->offset_y += offset_y;

            MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);
        }

        MatrixMenuDraw(menu_ctrl_ptr);
        MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);

#ifdef TOUCH_PANEL_SUPPORT

        if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)
        {
            GUI_RECT_T tmp_rect = {0};

            if (menu_ctrl_ptr->continue_move_type == CONTINUE_MOVE_DOWN || menu_ctrl_ptr->continue_move_type == CONTINUE_MOVE_UP)
            {
                tmp_rect = menu_ctrl_ptr->move_rect;
            }
            else
            {
                tmp_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);//
            }

            MatrixMenuClearItemRect(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index);

#ifdef UI_MULTILAYER_SUPPORT

            if (UILAYER_IsMultiLayerEnable())
            {
                MatrixMenuCreateMoveLayer(menu_ctrl_ptr);
                MatrixMenuSetBltMoveLayer(menu_ctrl_ptr);
                MatrixMenuRedrawMoveLayer(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index, tmp_rect);
            }

#endif
        }

#endif
    }
}

/*****************************************************************************/
//  Description : reset icon display
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuResetIconDisplay(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr,
    uint16 item_index
)
{
    GUI_RECT_T      item_rect  = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }


#ifdef UI_MULTILAYER_SUPPORT

    if (UILAYER_IsLayerActive(&s_item_layer_handle))
    {
        lcd_dev_info.block_id = s_item_layer_handle.block_id;
    }

    item_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, item_index);//

    //display bg
    MatrixMenuDrawBg(item_rect, menu_ctrl_ptr, PNULL);

    if (UILAYER_IsBltLayer(&menu_ctrl_ptr->highlight_layer_handle))
    {
        //消除highlight层的影响
        UILAYER_RemoveBltLayer(&(menu_ctrl_ptr->highlight_layer_handle));
    }

    MatrixMenuDrawItem(menu_ctrl_ptr, &lcd_dev_info, item_index, FALSE);

#else
    {
        CTRLMAINMENU_ITEM_T menuitem_info = {0};
        GUI_RECT_T disp_rect = {0};
        GUI_RECT_T cross_rect = {0};
        GUI_RECT_T menu_rect = {0};
        GUI_RECT_T icon_rect = {0};

        item_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

        MatrixMenuDrawBg(item_rect, menu_ctrl_ptr, PNULL);

        menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);
        disp_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);
        disp_rect = GUI_GetCenterRectExt(disp_rect, MENUICON_WIDTH, MENUICON_HEIGHT);

        if (!GUI_IntersectRect(&cross_rect, disp_rect, menu_rect))
        {
            return;
        }

        icon_rect = MatrixMenuGetIconDispRect(disp_rect, cross_rect, MENUICON_WIDTH, MENUICON_HEIGHT);
        MatrixMenuGetItemInfo(menu_ctrl_ptr, item_index, &menuitem_info);

        MMIMAINMENU_DrawItemIcon(menu_ctrl_ptr->win_handle, &menuitem_info, &lcd_dev_info, &icon_rect, &cross_rect);
    }
#endif
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : Clear Icon Rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuClearItemRect(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr,
    uint16 item_index
)
{
    GUI_RECT_T      item_rect  = {0};
    GUI_RECT_T      text_rect  = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    item_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, item_index);//

    item_rect.left -= 1;
    item_rect.top -= 1;
    item_rect.right += 1;
    item_rect.bottom += 1;

#ifdef UI_MULTILAYER_SUPPORT

    if (UILAYER_IsBltLayer(&menu_ctrl_ptr->highlight_layer_handle))
    {
        //消除highlight层的影响
        UILAYER_RemoveBltLayer(&(menu_ctrl_ptr->highlight_layer_handle));
        MMITHEME_ClearRect(&lcd_dev_info, &item_rect);
    }

#endif

    //display bg
    MatrixMenuDrawBg(item_rect, menu_ctrl_ptr, PNULL);

#ifdef UI_MULTILAYER_SUPPORT

    if (UILAYER_IsLayerActive(&s_item_layer_handle))
    {
        // clear lcd rect
        MMITHEME_ClearRect(&s_item_layer_handle, &item_rect);
    }

#endif

    if (!menu_ctrl_ptr->is_title_style)
    {
        text_rect = MatrixMenuGetTextRectExt(menu_ctrl_ptr, item_index);

        text_rect.left -= 1;
        text_rect.top -= 1;
        text_rect.right += 1;
        text_rect.bottom += 1;

        MatrixMenuDrawBg(text_rect, menu_ctrl_ptr, PNULL);

#ifdef UI_MULTILAYER_SUPPORT

        if (UILAYER_IsLayerActive(&s_item_layer_handle))
        {
            MMITHEME_ClearRect(&s_item_layer_handle, &text_rect);
        }

#endif
    }

    return;
}
#endif

/*****************************************************************************/
//  Description : get next item index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 MatrixMenuGetNextItemIndex(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
)
{
    uint16  next_item_index = 0;
    uint16  cur_item_index = 0;
    uint16  cur_col_index  = 0;
    uint16  cur_line_index  = 0;
    uint16  item_total_num = 0;
    int16   last_line_start_index = 0;
    uint32  icon_col_num = 0;
    uint16  total_line_num = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    icon_col_num = menu_ctrl_ptr->icon_data.page_col_num;
    // get current item index and total item
    cur_item_index = menu_ctrl_ptr->cur_item_index;

    // total item num
    item_total_num = menu_ctrl_ptr->menu_list_info.item_count;

    // get line number in one page
    total_line_num = MatrixMenuGetTotalLineNum(menu_ctrl_ptr);

    cur_line_index = MatrixMenuGetItemLineIndex(menu_ctrl_ptr, cur_item_index);

    cur_col_index = cur_item_index - MatrixMenuGetLineStartItemIndex(menu_ctrl_ptr, cur_line_index);

    if (0 == item_total_num)
    {
        return 0;
    }

    last_line_start_index = MatrixMenuGetLineStartItemIndex(menu_ctrl_ptr, total_line_num - 1);

    switch (msg_id)
    {
        case MSG_APP_UP:
        case MSG_KEYREPEAT_UP:   
            if (0 == cur_line_index) // 如果是首行,下一行为最后一行的开始index + 列号
            {
                next_item_index = last_line_start_index + cur_col_index;
            }
            else
            {
                next_item_index = (uint16)(cur_item_index - icon_col_num);
            }

            break;

        case MSG_APP_DOWN:
        case MSG_KEYREPEAT_DOWN:   
            if (cur_line_index == total_line_num - 1) // 如果已经在最后一行
            {
                next_item_index = cur_col_index; // 如果是最后一行，下一个如果循环的话，肯定为列的索引
            }
            else
            {
                next_item_index = (uint16)(cur_item_index + icon_col_num);

                if (next_item_index >= item_total_num && item_total_num > 0)
                {
                    next_item_index = item_total_num - 1;
                }
            }

            break;

        case MSG_APP_LEFT:
        case MSG_KEYREPEAT_LEFT:   
            if (0 < cur_item_index)
            {
                next_item_index = (uint16)(cur_item_index - 1);
            }
            else
            {
                next_item_index = (uint16)(item_total_num - 1);
            }

            break;

        case MSG_APP_RIGHT:
        case MSG_KEYREPEAT_RIGHT:       
            if ((item_total_num - 1) > cur_item_index)
            {
                next_item_index = (uint16)(cur_item_index + 1);
            }
            else
            {
                next_item_index = 0;
            }

            break;

        default:
            SCI_PASSERT(FALSE, ("MatrixMenuGetNextItemIndex:msg_id %d is error!", msg_id)); /*assert verified*/
            break;
    }

    // 纠正item索引号,防止超过范围
    if (next_item_index >= menu_ctrl_ptr->menu_list_info.item_count)
    {
        next_item_index = menu_ctrl_ptr->menu_list_info.item_count - 1;
    }

    return (next_item_index);
}

/*****************************************************************************/
//  Description : display active icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplayActiveItemTitle(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
)
{
    MMI_STRING_T        item_str = {0};
    GUI_COLOR_T         item_color = 0;
    MMI_IMAGE_ID_T      bg_image_id = IMAGE_THEME_BG;
    GUISTR_STYLE_T      str_style = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_EFFECT | GUISTR_STATE_ELLIPSIS | GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    CTRLMAINMENU_ITEM_T menuitem_info = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr || GUI_IsRectEmpty(menu_ctrl_ptr->icon_data.title_text_rect))
    {
        return;
    }

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    GUIRES_DisplayImg(PNULL,
                      &menu_ctrl_ptr->icon_data.title_text_rect,
                      &menu_ctrl_ptr->icon_data.title_text_rect,
                      menu_ctrl_ptr->win_handle,
                      bg_image_id,
                      (const GUI_LCD_DEV_INFO *)&lcd_dev_info);

    item_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);

    MatrixMenuGetItemInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &menuitem_info);

    if (CTRL_TEXT_ID == menuitem_info.text.type)
    {
        //display item text
        MMI_GetLabelTextByLang(menuitem_info.text.data.text_id, &item_str);
    }
    else
    {
        item_str.wstr_len = menuitem_info.text.data.str_info.wstr_len;
        item_str.wstr_ptr = menuitem_info.text.data.str_info.wstr_ptr;
    }

    str_style.effect = FONT_EFFECT_REVERSE_EDGE;
    str_style.align = ALIGN_HVMIDDLE;
    str_style.font = menu_ctrl_ptr->main_menu_theme.title_font;
    str_style.font_color = item_color;
    //display text
    GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                               (const GUI_RECT_T *)&menu_ctrl_ptr->icon_data.title_text_rect,
                               (const GUI_RECT_T *)&menu_ctrl_ptr->icon_data.title_text_rect,
                               (const MMI_STRING_T *)&item_str,
                               &str_style,
                               str_state,
                               GUISTR_TEXT_DIR_AUTO);
}

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuNumKey(
    uint16           num_index,
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    //set current item index
    if (num_index < menu_ctrl_ptr->menu_list_info.item_count)
    {
        menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;
        menu_ctrl_ptr->cur_item_index = num_index;

        MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->pre_cur_item_index);
        MatrixMenuDrawActiveIcon(menu_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuMove(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
)
{
    BOOLEAN result = TRUE;
    MMI_LINK_NODE_T *pre_cur_item_node_ptr = PNULL;
    MMI_LINK_NODE_T *cur_item_node_ptr = PNULL;

    if (menu_ctrl_ptr->is_moving && menu_ctrl_ptr->is_move_state) // 正在移動狀態
    {
        pre_cur_item_node_ptr = MatrixMenuSearchMenuItemByIndex(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index);
        cur_item_node_ptr = MatrixMenuSearchMenuItemByIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        if (menu_ctrl_ptr->cur_item_index == menu_ctrl_ptr->tmp_cur_item_index) // 相等的时候无变化
        {
            result = FALSE;
        }
        else
        {
            menu_ctrl_ptr->menu_list_info.list_head_ptr = MMILINK_RemoveNode(pre_cur_item_node_ptr, menu_ctrl_ptr->menu_list_info.list_head_ptr);

            if (menu_ctrl_ptr->cur_item_index > menu_ctrl_ptr->tmp_cur_item_index)
            {
                MMILINK_AddNodeAfterBaseNode(cur_item_node_ptr, pre_cur_item_node_ptr);
            }
            else if (menu_ctrl_ptr->cur_item_index < menu_ctrl_ptr->tmp_cur_item_index)
            {
                MMILINK_AddNodeBeforeBaseNode(cur_item_node_ptr, pre_cur_item_node_ptr);

                // 改变头位置
                if (menu_ctrl_ptr->menu_list_info.list_head_ptr == cur_item_node_ptr)
                {
                    menu_ctrl_ptr->menu_list_info.list_head_ptr = pre_cur_item_node_ptr;
                }
            }
            else
            {
                // 不可能走进来的分支
                SCI_ASSERT(0); /*assert verified*/
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"HandleMatrixMenuMove %d, %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_5986_112_2_18_2_33_38_126, (uint8 *)"dd", menu_ctrl_ptr->is_moving, menu_ctrl_ptr->is_move_state);
    }

    return result;
}

/*****************************************************************************/
//  Description : handle tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuTpMove(
    GUI_POINT_T *point_ptr,
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
	if ((PNULL == menu_ctrl_ptr) ||(PNULL == point_ptr))
    {
        return;
    }
	
    // 当move消息来时，停止timer
    if (IS_TP_MOVE_Y(menu_ctrl_ptr->slide_start_point.y, point_ptr->y)
        || !menu_ctrl_ptr->is_item_pressed)
    {
        if (!menu_ctrl_ptr->is_item_pressed || !menu_ctrl_ptr->is_move_state) // move icon状态下，不设置slide状态
        {
            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
        }
    }

    // 处理slide效果
    if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
    {
        return;
    }

    if (!menu_ctrl_ptr->is_moving && menu_ctrl_ptr->main_menu_theme.is_used_slide)
    {
        MMK_AddVelocityItem(point_ptr->x, point_ptr->y);
    }
    else
    {
        uint16      item_index = 0;
        GUI_RECT_T  tmp_rect = {0};

        //display move icon
        tmp_rect.left = MAX(point_ptr->x - menu_ctrl_ptr->tmp_width, 0);
        tmp_rect.top = MAX(point_ptr->y - menu_ctrl_ptr->tmp_height, 0);
        tmp_rect.right = tmp_rect.left + MENUICON_WIDTH - 1;
        tmp_rect.bottom = tmp_rect.top + MENUICON_HEIGHT - 1;

        menu_ctrl_ptr->move_rect = tmp_rect;

        item_index = MatrixMenuGetMenuItemIndexExt(menu_ctrl_ptr, point_ptr);

        if ((uint16) - 1 != item_index && item_index < menu_ctrl_ptr->item_total_num)
        {
            if (menu_ctrl_ptr->is_move_state) // 处理移动状态
            {
                // 移动状态但没有开始移动则返回
                if (menu_ctrl_ptr->is_moving)
                {
                    // 移动状态但没有开始移动时，返回，因为TP Down的位置可能不在item区域内
                    if (item_index != menu_ctrl_ptr->cur_item_index)
                    {
                        menu_ctrl_ptr->cur_item_index = item_index;

                        if (item_index >= menu_ctrl_ptr->item_total_num)
                        {
                            menu_ctrl_ptr->cur_item_index = menu_ctrl_ptr->item_total_num - 1;
                        }

                        if (HandleMatrixMenuMove(menu_ctrl_ptr))
                        {
                            menu_ctrl_ptr->tmp_cur_item_index = item_index;
                        }
                    }
                }
            }
            else // 处理非移动状态
            {
                menu_ctrl_ptr->cur_item_index = item_index;
            }
        }
    }

    MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_TP_PRESS_MOVE, point_ptr);

    return;
}

/*****************************************************************************/
//  Description : handle icon menu direction key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuAutoSwitchLine(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr
)
{
    // reset icon display
    MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

    menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;

    MatrixMenuDraw(menu_ctrl_ptr);
    MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);

    if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)
    {
        GUI_RECT_T tmp_rect = {0};

        if (menu_ctrl_ptr->continue_move_type == CONTINUE_MOVE_DOWN || menu_ctrl_ptr->continue_move_type == CONTINUE_MOVE_UP)
        {
            tmp_rect = menu_ctrl_ptr->move_rect;
        }
        else
        {
            tmp_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
        }

        MatrixMenuClearItemRect(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index);

#ifdef UI_MULTILAYER_SUPPORT

        if (UILAYER_IsMultiLayerEnable())
        {
            MatrixMenuCreateMoveLayer(menu_ctrl_ptr);
            MatrixMenuSetBltMoveLayer(menu_ctrl_ptr);
            MatrixMenuRedrawMoveLayer(menu_ctrl_ptr, menu_ctrl_ptr->tmp_cur_item_index, tmp_rect);
        }

#endif
    }
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMatrixMenuTimer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,     //事件类型
    DPARAM           param          //附带的处理参数
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (menu_ctrl_ptr->slide_timer_id == *(uint8 *)param)
    {
        GUI_POINT_T  point = {0};
        MMI_TP_STATUS_E state = MMI_TP_NONE;

        MMK_GetLogicTPMsg(&state, &point);

        MatrixMenuStopSlideTimer(menu_ctrl_ptr);

        if (MMK_IsFocusWin(menu_ctrl_ptr->win_handle)
            && menu_ctrl_ptr->main_menu_theme.is_used_slide)
        {
            MatrixMenuStartSlideTimer(menu_ctrl_ptr);

            if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_state)
            {
                if (!menu_ctrl_ptr->is_tp_pressed
                    || IS_TP_MOVE_Y(menu_ctrl_ptr->slide_pre_point.y, point.y))
                {
                    MatrixMenuSlide(menu_ctrl_ptr, &point);

                    menu_ctrl_ptr->slide_pre_point.x = point.x;
                    menu_ctrl_ptr->slide_pre_point.y = point.y;
                }
            }
        }
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description : reset Y offset
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL int16 MatrixMenuResetOffsetY(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
)
{
    int16  offset_y = 0;
    int16  max_offset_y = 0;
    GUI_RECT_T menu_rect = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return offset_y;
    }

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    if (menu_ctrl_ptr->offset_y >= 0)
    {
        offset_y = -menu_ctrl_ptr->offset_y;
    }
    else
    {
        if (MatrixMenuGetTotalLineNum(menu_ctrl_ptr) > menu_ctrl_ptr->icon_data.page_row_num)
        {
            // 计算最大偏移量
            max_offset_y = -s_total_line_height;

            if (menu_ctrl_ptr->offset_y < max_offset_y + (menu_rect.bottom - menu_rect.top + 1))
            {
                offset_y = max_offset_y + (menu_rect.bottom - menu_rect.top + 1) - menu_ctrl_ptr->offset_y;
            }
        }
        else
        {
            offset_y = -menu_ctrl_ptr->offset_y;
        }
    }

    return offset_y;
}


#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 拷贝不需要更新的buffer
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDoDoubleMemWriteBack(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    int32             offset_y
)
{
    GUI_RECT_T  src_rc = {0};
    GUI_RECT_T  dst_rc = {0};
    GUI_RECT_T  clip_rc = {0};
    int32       offset = 0;
    UILAYER_MEM_FORMAT_T old_format = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (!(menu_ctrl_ptr->main_menu_theme.is_used_slide
          && UILAYER_IsLayerActive(&s_item_layer_handle)))
    {
        return;
    }

    offset = menu_ctrl_ptr->offset_y - offset_y;

    //根据old format判断是否第一帧
    old_format = UILAYER_GetLayerMemFormat(&s_item_layer_handle);

    //切换双buffer
    if (!UILAYER_SwitchDoubleMem(&s_item_layer_handle, TRUE))
    {
        return;
    }

    //第一帧需要重画
    if (!(old_format & UILAYER_MEM_FORMAT_IMMUTABLE))
    {
        return;
    }

    if (0 == offset)
    {
        return;
    }

    dst_rc = src_rc = clip_rc = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    dst_rc.top    += offset;
    dst_rc.bottom += offset;

    UILAYER_Clear(&s_item_layer_handle);

    GUI_IntersectRect(&clip_rc, clip_rc, dst_rc);

    if (UILAYER_DoubleMemWriteBack(&s_item_layer_handle, &dst_rc, &clip_rc, &src_rc))
    {
        //如果回写bufer成功则将相应的区域值上
        menu_ctrl_ptr->writeback_rect = clip_rc;
    }
}
#endif

/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuResume(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    int16              move_space
)
{
    BOOLEAN     result      = TRUE;
    uint16      i           = 0;
    uint16      frame_num   = 8; // 移动频率
    int16       offset_y    = 0;
    int32       offset      = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T menu_rect = {0};
    GUI_LCD_DEV_INFO menu_lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &menu_rect);
    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &menu_lcd_dev_info);

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);
#endif

    offset_y = menu_ctrl_ptr->offset_y;
#ifdef UI_MULTILAYER_SUPPORT
    lcd_dev_info.block_id = s_item_layer_handle.block_id;

    //恢复双buffer copy
    UILAYER_SwitchDoubleMem(&s_item_layer_handle, FALSE);
#endif

    if (0 == move_space)
    {
        // 当layer无效的时候，显示背景
        MatrixMenuClearBackGround(menu_ctrl_ptr);

#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
        MatrixMenuRenderRollEffect(menu_ctrl_ptr);
#endif
#else
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_Clear(&s_item_layer_handle);

        //display item
        MatrixMenuBltItemLayerToMenuLayer(menu_ctrl_ptr);
#endif
        //display scroll groove
        MatrixMenuDrawScrollBar(menu_ctrl_ptr, TRUE);
#endif
    }
    else
    {
        for (i = 1; i <= frame_num; i++)
        {
            // 当layer无效的时候，显示背景
            MatrixMenuClearBackGround(menu_ctrl_ptr);

            offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, i);

            menu_ctrl_ptr->offset_y = offset_y + offset;

            // 重新设置start index
            MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
            UILAYER_Clear(&s_item_layer_handle);
#endif

#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
            MatrixMenuRenderRollEffect(menu_ctrl_ptr);
#endif
#else
#ifdef UI_MULTILAYER_SUPPORT
            //display item
            MatrixMenuBltItemLayerToMenuLayer(menu_ctrl_ptr);
#endif
            //display scroll groove
            MatrixMenuDrawScrollBar(menu_ctrl_ptr, TRUE);
#endif
            // 画softkey
            MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);
            {
#ifdef ROLL_MAINMENU_SUPPORT
                GUI_RECT_T rect = {0};
                uint16              lcd_width = 0;
                uint16              lcd_height = 0;

                GUILCD_GetLogicWidthHeight(menu_lcd_dev_info.lcd_id, &lcd_width, &lcd_height);
                rect.right = lcd_width - 1;
                rect.bottom = lcd_height - 1;
                GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, rect, 0);
#else
                GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, menu_rect, 0);
#endif
            }
#ifdef WIN32
            SCI_Sleep(10);
#endif
        }
    }

    MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif

    return result;
}
#endif


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : display second menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuSlide(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    GUI_POINT_T        *tp_point_ptr
)
{
    int32       offset_y = 0;
    int32       menu_height = 0;
    BOOLEAN     result = FALSE;
    GUI_RECT_T  menu_rect = {0};

    if ((PNULL == menu_ctrl_ptr) || (PNULL == tp_point_ptr))
    {
        return FALSE;
    }

    if (0 == menu_ctrl_ptr->item_total_num)
    {
        return result;
    }

    offset_y = menu_ctrl_ptr->offset_y;

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    menu_height = menu_rect.bottom - menu_rect.top + 1;

    if (MMK_TP_SLIDE_FLING == menu_ctrl_ptr->slide_state)
    {
#ifdef TOUCH_PANEL_SUPPORT
        menu_ctrl_ptr->offset_y = MMK_GetFlingOffset(menu_ctrl_ptr->offset_y, menu_ctrl_ptr->fling_velocity, (float)FLING_TIME, &menu_ctrl_ptr->fling_velocity);
#endif

        if (FLING_MIN_VELOCITY > abs((int32)menu_ctrl_ptr->fling_velocity))
        {
            MatrixMenuStopSlideTimer(menu_ctrl_ptr);

            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
            result = FALSE;
        }
        else if (0 > menu_ctrl_ptr->fling_velocity) // 向上
        {
            int16 end_offset_y = 0;

            // 计算最大偏移
            end_offset_y = -s_total_line_height;

            if (menu_ctrl_ptr->offset_y < end_offset_y + menu_height * 2 / 3) // 1/2处恢复
            {
                menu_ctrl_ptr->offset_y = end_offset_y + menu_height * 2 / 3;
                MatrixMenuStopSlideTimer(menu_ctrl_ptr);

                menu_ctrl_ptr->fling_velocity = 0;

                result = FALSE;
            }
            else
            {
                result = TRUE;
            }
        }
        else if (0 < menu_ctrl_ptr->fling_velocity) // 向下
        {
            // 第一条显示在第一行的偏移量
            if (menu_ctrl_ptr->offset_y > menu_height / 3)
            {
                menu_ctrl_ptr->offset_y = menu_height / 3;

                MatrixMenuStopSlideTimer(menu_ctrl_ptr);

                menu_ctrl_ptr->fling_velocity = 0;

                result = FALSE;
            }
            else
            {
                result = TRUE;
            }
        }
    }
    else if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state)
    {
        menu_ctrl_ptr->offset_y += (tp_point_ptr->y - menu_ctrl_ptr->slide_pre_point.y);

        if (menu_ctrl_ptr->offset_y > menu_height / 3)
        {
            menu_ctrl_ptr->offset_y = menu_height / 3;
        }
        else if (menu_ctrl_ptr->offset_y < -s_total_line_height + menu_height * 2 / 3)
        {
            menu_ctrl_ptr->offset_y = -s_total_line_height + menu_height * 2 / 3;
        }

        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

#ifdef UI_MULTILAYER_SUPPORT
    //消除highlight层的影响
    UILAYER_RemoveBltLayer(&(menu_ctrl_ptr->highlight_layer_handle));
#endif

    if (result)
    {
        // 当layer无效的时候，显示背景
        MatrixMenuClearBackGround(menu_ctrl_ptr);
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetDirectDraw(TRUE);
        UILAYER_SetMainLayerImmutable(TRUE);
#endif

#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
        //设置双buffer切换，不使用copy
        UILAYER_SwitchDoubleMem(&s_item_layer_handle, TRUE);

        MatrixMenuRenderRollEffect(menu_ctrl_ptr);
#endif
#else

        SCI_MEMSET(&menu_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));

#ifdef UI_MULTILAYER_SUPPORT
        //设置双buffer切换，不使用copy
        MatrixMenuDoDoubleMemWriteBack(menu_ctrl_ptr, offset_y);

        MatrixMenuBltItemLayerToMenuLayer(menu_ctrl_ptr);
#endif
        //display scroll groove
        MatrixMenuDrawScrollBar(menu_ctrl_ptr, TRUE);

        SCI_MEMSET(&menu_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));
#endif
    }
    else
    {
        MatrixMenuResume(menu_ctrl_ptr, MatrixMenuResetOffsetY(menu_ctrl_ptr));

        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

        if (!menu_ctrl_ptr->main_menu_theme.is_hl_disappear  || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
        {
            MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
        }
    }

    MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);

    return result;
}

/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int16 MatrixMenuGetMenuItemIndexExt(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)
{
    BOOLEAN is_find = FALSE;
    int16 row = 0;
    int16 line_index = 0;
    int16 item_index = 0;
    int16 start_item_index = 0;
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T menu_rect = {0};

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    if (GUI_PointIsInRect(*tp_point_ptr, menu_rect))
    {
        for (line_index = s_start_line_index; (!is_find && line_index <= s_end_line_index); line_index++)
        {
            start_item_index = MatrixMenuGetLineStartItemIndex(menu_ctrl_ptr, line_index);

            for (row = 0; row < menu_ctrl_ptr->icon_data.page_col_num; row++)
            {
                item_index = start_item_index + row;

                if (item_index < menu_ctrl_ptr->item_total_num)
                {
                    // 计算显示区域
                    disp_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

                    if (GUI_PointIsInRect(*tp_point_ptr, disp_rect))
                    {
                        is_find = TRUE;
                        break;
                    }
                }
            }
        }
    }

    if (!is_find)
    {
        item_index = -1;
    }

    return item_index;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuTransmitTpMsg(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *tp_point_ptr
)
{
    BOOLEAN     result = FALSE;
    GUI_RECT_T  rect = {0};

    if (PNULL != menu_ctrl_ptr
        && PNULL != menu_ctrl_ptr->icon_data.data_ptr)
    {
        MMI_MULTI_KEY_TP_MSG_PARAM_T para = {0};

        if (MSG_CTL_PAINT == msg_id)
        {
            result = TRUE;
        }
        else if (PNULL != tp_point_ptr)
        {
            para.cur_tp_point.x = tp_point_ptr->x;
            para.cur_tp_point.y = tp_point_ptr->y;

            IGUICTRL_GetRect((IGUICTRL_T *)menu_ctrl_ptr->icon_data.data_ptr, &rect);

            if (GUI_PointIsInRect(*tp_point_ptr, rect))
            {
                result = TRUE;
            }
        }

        if (result)
        {
            IGUICTRL_HandleEvent((IGUICTRL_T *)menu_ctrl_ptr->icon_data.data_ptr, msg_id, (DPARAM)&para);
        }
    }

    return result;
}
#else
/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuTransmitTpMsg(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *tp_point_ptr
)
{
    BOOLEAN     result = FALSE;
    GUI_RECT_T  rect = {0};
    IGUICTRL_T  *button_ptr = PNULL;
    uint16      i = 0;

    if (PNULL != menu_ctrl_ptr)
    {
        MMI_MULTI_KEY_TP_MSG_PARAM_T param = {0};

        if (MSG_CTL_PAINT == msg_id)
        {
            result = TRUE;
            button_ptr = MMK_GetCtrlPtr(menu_ctrl_ptr->button_handle[0]);

            if (PNULL != button_ptr)
            {
                IGUICTRL_HandleEvent(button_ptr, msg_id, (DPARAM)&param);
            }

            button_ptr = MMK_GetCtrlPtr(menu_ctrl_ptr->button_handle[1]);

            if (PNULL != button_ptr)
            {
                IGUICTRL_HandleEvent(button_ptr, msg_id, (DPARAM)&param);
            }
        }
        else if (PNULL != tp_point_ptr)
        {
            param.cur_tp_point.x = tp_point_ptr->x;
            param.cur_tp_point.y = tp_point_ptr->y;

            for (i = 0; i < GUIMENU_BUTTON_NUM; i++)
            {
                if (0 != menu_ctrl_ptr->button_handle[i])
                {
                    button_ptr = MMK_GetCtrlPtr(menu_ctrl_ptr->button_handle[i]);

                    IGUICTRL_GetRect(button_ptr, &rect);

                    if (GUI_PointIsInRect(*tp_point_ptr, rect))
                    {
                        result = TRUE;
                        IGUICTRL_HandleEvent(button_ptr, msg_id, (DPARAM)&param);
                        break;
                    }
                }
            }
        }
    }

    return result;
}
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuTpDown(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    int16       item_index = 0;
    uint16      page_item_num = 0;
    GUI_RECT_T  item_rect = {0};
    GUI_RECT_T  tmp_rect = {0};
    GUI_RECT_T  move_rect = {0};

    if ((PNULL == menu_ctrl_ptr) || (PNULL == tp_point_ptr))
    {
        return;
    }

    menu_ctrl_ptr->is_tp_pressed = TRUE; // 该变量在此表示触笔按下

    // 记录起始点
    menu_ctrl_ptr->slide_start_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->slide_start_point.y = tp_point_ptr->y;
    menu_ctrl_ptr->slide_pre_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->slide_pre_point.y = tp_point_ptr->y;

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_6912_112_2_18_2_33_40_128, (uint8 *)"dd", tp_point_ptr->y, menu_ctrl_ptr->offset_y);

    // 记录这个时候的滑动状态
    menu_ctrl_ptr->tpdown_slide_state = menu_ctrl_ptr->slide_state;

    MMK_AddVelocityItem(tp_point_ptr->x, tp_point_ptr->y);   // 增加速度的点

    if (MMK_TP_SLIDE_FLING == menu_ctrl_ptr->slide_state)   // fling的时候直接返回
    {
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
        return;
    }

    //display move icon
    page_item_num = MatrixMenuGetIconMenuPageItemNum(menu_ctrl_ptr);

    item_index = MatrixMenuGetMenuItemIndexExt(menu_ctrl_ptr, tp_point_ptr);

    if (0 <= item_index && item_index < menu_ctrl_ptr->item_total_num)
    {
        menu_ctrl_ptr->is_item_pressed = TRUE;

        item_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, item_index);

        // 记录tpdown时的偏移
        move_rect = GUI_GetCenterRectExt(item_rect,
                                         menu_ctrl_ptr->main_menu_theme.focus_item_w,
                                         menu_ctrl_ptr->main_menu_theme.focus_item_h);

        menu_ctrl_ptr->tmp_width = tp_point_ptr->x - move_rect.left;
        menu_ctrl_ptr->tmp_height = tp_point_ptr->y - move_rect.top;

        menu_ctrl_ptr->is_item_pressed = TRUE;

        tmp_rect.left = MAX(tp_point_ptr->x - menu_ctrl_ptr->tmp_width, 0);
        tmp_rect.top = MAX(tp_point_ptr->y - menu_ctrl_ptr->tmp_height, 0);

        tmp_rect.right = tmp_rect.left + menu_ctrl_ptr->main_menu_theme.focus_item_w - 1;
        tmp_rect.bottom = tmp_rect.top + menu_ctrl_ptr->main_menu_theme.focus_item_h - 1;
        menu_ctrl_ptr->move_rect = tmp_rect;

        if (item_index != menu_ctrl_ptr->cur_item_index)
        {
            menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;
            //set current item index
            menu_ctrl_ptr->cur_item_index = item_index;

            if (menu_ctrl_ptr->is_move_state) // 进入move状态时需要创建层
            {
                //reset icon display
                MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->pre_cur_item_index);
                // 进入正在移动状态
                menu_ctrl_ptr->is_moving = TRUE;

                //reset icon display
                MatrixMenuClearItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

                if (MMIMAINMENU_MATRIX_TITLE_E == MMIAPISET_GetMainmenuStyle())
                {
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
                    MatrixMenuDisplayActiveItemTitleEx(menu_ctrl_ptr);
#endif
#else
                    MatrixMenuDisplayActiveItemTitle(menu_ctrl_ptr);
#endif
                }    


#ifdef UI_MULTILAYER_SUPPORT

                if (UILAYER_IsMultiLayerEnable())
                {
                    MatrixMenuCreateMoveLayer(menu_ctrl_ptr);
                    MatrixMenuSetBltMoveLayer(menu_ctrl_ptr);
                    MatrixMenuRedrawMoveLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index,
                                              item_rect);
                }

#endif

                MatrixMenuResetMoveItemInfo(menu_ctrl_ptr);
            }
            else
            {
                //reset icon display
                MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->pre_cur_item_index);
                //display active icon
                MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
            }
        }
        else
        {

            if (menu_ctrl_ptr->is_move_state)
            {
                // 进入正在移动状态
                menu_ctrl_ptr->is_moving = TRUE;

                //reset icon display
                MatrixMenuClearItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

#ifdef UI_MULTILAYER_SUPPORT

                if (UILAYER_IsMultiLayerEnable())
                {
                    MatrixMenuCreateMoveLayer(menu_ctrl_ptr);
                    MatrixMenuSetBltMoveLayer(menu_ctrl_ptr);
                    MatrixMenuRedrawMoveLayer(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index,
                                              item_rect);
                }

#endif
                MatrixMenuResetMoveItemInfo(menu_ctrl_ptr);
            }
            else
            {
                MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
            }
        }
    }

    MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_TP_PRESS_DOWN, tp_point_ptr);

    if (menu_ctrl_ptr->main_menu_theme.is_used_slide)
    {
        MatrixMenuStartSlideTimer(menu_ctrl_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : create slide info
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 计算行区域，不包括行间距
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetLineRectButNotMargin(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,  //in
    uint16 line_index
)
{
    uint16 icon_line_height = 0;
    GUI_RECT_T line_rect = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return line_rect;
    }

    if (0 == s_total_row_num)
    {
        return line_rect;
    }

    if (line_index < s_total_row_num)
    {
        // 取menu rect
        line_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);
        icon_line_height = menu_ctrl_ptr->icon_data.item_height;

        // 计算区域的top和bottom值
        line_rect.top = s_icon_line_pos_ptr[line_index].line_offset_y + menu_ctrl_ptr->offset_y;
        line_rect.bottom = line_rect.top + icon_line_height - 1;
    }

    return line_rect;
}

/*****************************************************************************/
//  Description : reset list display item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuAdjuestOffsetY(
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    int16  *offset_y_ptr,
    uint16 line_index
)
{
    int16 offset_y = 0;
    BOOLEAN reslut = TRUE;
    GUI_RECT_T line_rect = {0};
    GUI_RECT_T inter_item_rect = {0};
    GUI_RECT_T menu_rect = {0};

    if (
        (PNULL == menu_ctrl_ptr) ||
        (PNULL == offset_y_ptr)
    )
    {
        return FALSE;
    }

    if (line_index >= s_total_row_num)
    {
        return FALSE;
    }

    // 获得menu区域
    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    // 获得line区域
    line_rect = MatrixMenuGetLineRectButNotMargin(menu_ctrl_ptr, line_index);

    if (GUI_IntersectRect(&inter_item_rect, line_rect, menu_rect)) // 有交集
    {
        // 不被完全覆盖
        if (!GUI_RectIsCovered(menu_rect, line_rect))
        {
            if (inter_item_rect.top > line_rect.top) // item的上半部被覆盖
            {
                // 被覆盖后，不管down key 还是 up key都只回复显示cur，而不改变cur index
                offset_y = inter_item_rect.top - line_rect.top; // 需要向下移动的位移
            }
            else if (inter_item_rect.bottom < line_rect.bottom) // item的下半部分被覆盖
            {
                offset_y = inter_item_rect.bottom - line_rect.bottom; // 需要向上移动的位移
            }
            else
            {
                reslut = FALSE;
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_7132_112_2_18_2_33_40_129, (uint8 *)"");
            }
        }
        else
        {
            reslut = FALSE;
        }
    }
    else // 完全没有交集
    {
        if (line_rect.top < menu_rect.top) // 在菜单区域之上
        {
            offset_y = menu_rect.top - line_rect.top; // 向下移动的位移
        }
        else if (line_rect.bottom > menu_rect.bottom)
        {
            offset_y = menu_rect.bottom - line_rect.bottom;
        }
        else
        {
            reslut = FALSE;
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_7153_112_2_18_2_33_40_130, (uint8 *)"");
        }
    }

    *offset_y_ptr = offset_y;

    return reslut;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle icon touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMatrixMenuTpUp(
    GUI_POINT_T       *tp_point_ptr,
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;
    uint16      item_index = 0;
    GUI_POINT_T tpup_point  =  {0};
    uint16      layer_width = 0;
    uint16      layer_height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T rect = {0};

    if ((PNULL == menu_ctrl_ptr) || (PNULL == tp_point_ptr))
    {
        return FALSE;
    }

    GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &rect);

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_7194_112_2_18_2_33_40_131, (uint8 *)"d", tp_point_ptr->y);

    menu_ctrl_ptr->is_item_pressed = FALSE;
    //get selected bar display rect
    menu_ctrl_ptr->is_tp_pressed = FALSE; // 该变量在此表示触笔弹起

    MatrixMenuStopSlideTimer(menu_ctrl_ptr);

    if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_state)
    {
        // add
        MMK_AddVelocityItem(tp_point_ptr->x, tp_point_ptr->y);

        // compute
#ifdef TOUCH_PANEL_SUPPORT
        MMK_ComputeCurrentVelocity(PNULL, &menu_ctrl_ptr->fling_velocity, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);
#endif
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_7210_112_2_18_2_33_40_132, (uint8 *)"f", menu_ctrl_ptr->fling_velocity);

        // 最小速度
        if (FLING_MIN_VELOCITY <= abs((int32)menu_ctrl_ptr->fling_velocity))
        {
            MatrixMenuStartSlideTimer(menu_ctrl_ptr);
            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;
        }
        else
        {

#ifdef TOUCH_PANEL_SUPPORT
            MMK_ResetVelocityItem();
#endif
            menu_ctrl_ptr->fling_velocity = 0;

            GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
            GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

            //tpup_point.x = tpup_point.x;
            tpup_point.y = MIN(tp_point_ptr->y, (int16)layer_height);

            // 恢复
            MatrixMenuResume(menu_ctrl_ptr, MatrixMenuResetOffsetY(menu_ctrl_ptr));

            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        }
    }
    else if (GUI_PointIsInRect(*tp_point_ptr, rect)) // 不在menu控件区域，例如可能在softkey或者title区域
    {
        item_index = MatrixMenuGetMenuItemIndexExt(menu_ctrl_ptr, tp_point_ptr);

        if ((uint16) - 1 != item_index && item_index < menu_ctrl_ptr->item_total_num &&
            menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)
        {
            if (item_index != menu_ctrl_ptr->cur_item_index)
            {
                //set current item index
                menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;
                menu_ctrl_ptr->cur_item_index = item_index;

                if (item_index >= menu_ctrl_ptr->item_total_num)
                {
                    menu_ctrl_ptr->cur_item_index = menu_ctrl_ptr->item_total_num - 1;
                }
            }
            result = TRUE;
        }
    }

    if (menu_ctrl_ptr->main_menu_theme.is_hl_disappear)
    {
        MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
    }
    else
    {
        MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
    }

    if (menu_ctrl_ptr->is_move_state && menu_ctrl_ptr->is_moving)
    {
        // 恢复,当移动到其它地方弹起的时候需要恢复
        if (!result)
        {
            menu_ctrl_ptr->is_moving = FALSE; // 移动结束

            menu_ctrl_ptr->cur_item_index = menu_ctrl_ptr->tmp_cur_item_index;

            menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->tmp_first_item_index;
            menu_ctrl_ptr->start_item_index = menu_ctrl_ptr->first_item_index;

            MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
            MatrixMenuReleaseMoveIconLayer(menu_ctrl_ptr);
#endif
#endif

            // 由于这个时候需要清楚层，又要刷新softkey，如果不全刷，则状态栏不刷新，会有显示问题，因此需要send full paint
            MMK_SendMsg(menu_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL);
        }
    }

    MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_TP_PRESS_UP, tp_point_ptr);

    //点中Matrix主菜单中的某一个菜单项
    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && GUI_PointIsInRect(*tp_point_ptr, rect))
    {
        item_index = MatrixMenuGetMenuItemIndexExt(menu_ctrl_ptr, tp_point_ptr);
        
        if ((uint16) - 1 != item_index && item_index < menu_ctrl_ptr->item_total_num && 
            !menu_ctrl_ptr->is_move_state &&
            item_index == menu_ctrl_ptr->cur_item_index)
        {
            MatrixMenuRunItem(menu_ctrl_ptr, item_index);
            result = TRUE;
        }
    }

    return (result);
}
#endif

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : alloc 3D buffer for icon main menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuAllocMenu3DBuffer(
                              MMI_HANDLE_T      win_handle,
                              MMI_IMAGE_ID_T    item_img_id
                              )
{
    uint16      img_width = 0;
    uint16      img_height = 0;
    uint32      buf_size = 0;

    if (MMIAPI3D_IsIconMenuAnimOpen())
    {
        //get icon width and height
        GUIRES_GetImgWidthHeight(&img_width,
            &img_height,
            item_img_id,
            win_handle);
    
        //set buffer size
        buf_size = img_width * img_height * sizeof(uint16);

        //alloc
        MMIAPI3D_Alloc3DMenuBuf((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_ICON_MENU, buf_size, win_handle);
    }
}

/*****************************************************************************/
//  Description : save previous menu icon for 3D
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL SE_RECT_T MatrixMenuSavePrevIcon(
                              MMI_HANDLE_T      win_handle,
                              MMI_IMAGE_ID_T    item_image_hl_id,
                              GUI_RECT_T        icon_rect,
                              MMI_MESSAGE_ID_E  msg_id
                              )
{
    SE_RECT_T       se_rect = {0};
    SE_DIRECTION_E  se_dir = SE_DIRECTION_LEFT;

    if (MMIAPI3D_IsIconMenuAnimOpen())
    {
        MatrixMenuAllocMenu3DBuffer(
            win_handle,
            item_image_hl_id
            );        
        //get previous icon rect
        se_rect.left   = icon_rect.left;
        se_rect.right  = icon_rect.right;
        se_rect.top    = icon_rect.top;
        se_rect.bottom = icon_rect.bottom;

        //save icon
        MMIAPI3D_SaveMenuIcon(PREV_BUF,&se_rect);

        //get icon move direction
        switch (msg_id)
        {
        case MSG_APP_UP:
        case MSG_KEYREPEAT_UP:       
            se_dir = SE_DIRECTION_UP;
            break;

        case MSG_APP_DOWN:
        case MSG_KEYREPEAT_DOWN:       
            se_dir = SE_DIRECTION_DOWN;
            break;

        case MSG_APP_LEFT:
        case MSG_KEYREPEAT_LEFT:       
            se_dir = SE_DIRECTION_LEFT;
            break;

        case MSG_APP_RIGHT:
        case MSG_KEYREPEAT_RIGHT:   
            se_dir = SE_DIRECTION_RIGHT;
            break;

        default:
            SCI_PASSERT(FALSE,("MatrixMenuSavePrevIcon:msg_id %d is error!",msg_id)); /*assert verified*/
            break;
        }

        //set icon move direction
        MMIAPI3D_SetIconMenuDirection(se_dir);
    }

    return (se_rect);
}

/*****************************************************************************/
//  Description : save next menu icon for 3D
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSaveNextMenuIcon(
    MMIMAINMENU_MATRIX_DATA_T  *menu_ctrl_ptr
)
{
    SE_RECT_T       se_rect = {0};
    GUI_RECT_T      icon_rect = {0};

    if (MMIAPI3D_IsIconMenuAnimOpen())
    {
        //display active icon
        MatrixMenuDrawActiveIcon(menu_ctrl_ptr);

        //get current icon rect
        icon_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);//

        // 重新调整一下区域
        icon_rect = GUI_GetCenterRectExt(icon_rect, MENUICON_WIDTH, MENUICON_HEIGHT);

        se_rect.left   = icon_rect.left;
        se_rect.right  = icon_rect.right;
        se_rect.top    = icon_rect.top;
        se_rect.bottom = icon_rect.bottom;

        //save icon
        MMIAPI3D_SaveMenuIcon(NEXT_BUF, &se_rect);

        //reset icon display
        MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        MMIAPI3D_StartIconMenuAnim();
    }
}
#endif

#ifdef ATEST_SUPPORT
/*****************************************************************************/
//  Description : get the item rect of Icon Menu for atest
//  Global resource dependence : none
//  Author: lin.chen
//  Note:
/*****************************************************************************/
PUBLIC void Atest_MatrixMenuMenuIconGetItemRectExt(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    int16 item_index,
    GUI_RECT_T *icon_rect
)
{
    if (PNULL != menu_ctrl_ptr)
    {
        *icon_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, item_index);
    }
}
#endif //ATEST_SUPPORT

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : blt layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetBltHeightLightLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (UILAYER_IsLayerActive(&s_item_layer_handle))
    {
        append_layer.lcd_dev_info = s_item_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    append_layer.lcd_dev_info = menu_ctrl_ptr->highlight_layer_handle;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);


    return;
}
#endif

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create highlight layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateHighlightLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T          icon_rect = {0};
    UILAYER_CREATE_T    create_info = {0};


    if ((0 == menu_ctrl_ptr->highlight_layer_handle.block_id ||
         UILAYER_NULL_HANDLE == menu_ctrl_ptr->highlight_layer_handle.block_id))
    {
        icon_rect = MatrixMenuGetHighLightRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = icon_rect.left;
        create_info.offset_y = icon_rect.top;
        create_info.width = (icon_rect.right + 1 - icon_rect.left);
        create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(
            &create_info,
            &(menu_ctrl_ptr->highlight_layer_handle)
        );

        UILAYER_SetLayerColorKey(&(menu_ctrl_ptr->highlight_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);

        UILAYER_Clear(&(menu_ctrl_ptr->highlight_layer_handle));
    }
}

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseHighlightLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->highlight_layer_handle);   /*lint !e506 !e774*/
    }

    return;
}
#endif

/*****************************************************************************/
//  Description : get rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MatrixMenuGetIconDispRect(
    GUI_RECT_T disp_rect,
    GUI_RECT_T cross_rect,
    uint16 width,
    uint16 height
)
{
    GUI_RECT_T icon_rect = {0};

    //set display point
    if (GUI_IntersectRect(&cross_rect, disp_rect, cross_rect))
    {
        icon_rect.right = width - 1;

        if (disp_rect.top < cross_rect.top)
        {
            icon_rect.top = (cross_rect.top - disp_rect.top);
            icon_rect.bottom = height - 1;
        }
        else if (disp_rect.bottom > cross_rect.bottom)
        {
            icon_rect.top = 0;
            icon_rect.bottom = height - (disp_rect.bottom - cross_rect.bottom);
        }
        else
        {
            icon_rect.top = 0;
            icon_rect.bottom = height - 1;
        }
    }

    return icon_rect;
}

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence :
//  Author: bin.ji
//  Note: xiyuan.ma edit
/*****************************************************************************/
LOCAL void MatrixMenuDrawHighlightItem(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
)
{
    GUI_RECT_T menu_rect = {0};
    GUI_RECT_T cross_rect = {0};
    GUI_RECT_T focus_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    focus_rect = MatrixMenuGetHighLightRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT
    //显示menu item高亮条
    if (UILAYER_IsLayerActive(&(menu_ctrl_ptr->highlight_layer_handle)))
    {
        UILAYER_SetLayerPosition(&(menu_ctrl_ptr->highlight_layer_handle), focus_rect.left, focus_rect.top);
        UILAYER_Clear(&(menu_ctrl_ptr->highlight_layer_handle));
        lcd_dev_info.block_id = menu_ctrl_ptr->highlight_layer_handle.block_id;
    }
#endif

    if (!GUI_IntersectRect(&cross_rect, focus_rect, menu_rect) || menu_ctrl_ptr->is_move_state)
    {
        return;
    }

#ifndef MMI_PDA_SUPPORT
    {
        CTRLMAINMENU_ITEM_T menuitem_info = {0};
        GUI_RECT_T layer_rect = {0};
        MatrixMenuGetItemInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &menuitem_info);
        layer_rect = MatrixMenuGetIconDispRect(focus_rect, cross_rect, focus_rect.right - focus_rect.left + 1, focus_rect.bottom - focus_rect.top + 1);

        GUIRES_DisplayImg(PNULL,
                          &cross_rect,
                          &layer_rect,
                          menu_ctrl_ptr->win_handle,
                          menuitem_info.active_icon.data.icon_id,
                          (const GUI_LCD_DEV_INFO *)&lcd_dev_info);
    }
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuDrawItem(menu_ctrl_ptr, &lcd_dev_info, menu_ctrl_ptr->cur_item_index, TRUE);
#endif

#else
    // 静态显示menu item图标
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_ClearRect(&s_item_layer_handle, MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index));
    MatrixMenuDrawItem(menu_ctrl_ptr, &lcd_dev_info, menu_ctrl_ptr->cur_item_index, TRUE);
    UILAYER_WeakLayerAlpha(&lcd_dev_info, MMIMENU_WEAKLAYERALPHA);
#endif
#endif
}



#ifdef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateMoveLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T          icon_rect = {0};
    GUI_RECT_T          disp_rect = {0};
    UILAYER_CREATE_T    create_info = {0};


    if (!MatrixMenuIsItemInPage(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index))
    {
        return;
    }

    //get display rect
    icon_rect = MatrixMenuGetItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

    disp_rect = GUI_GetCenterRectExt(icon_rect, menu_ctrl_ptr->main_menu_theme.focus_item_w, menu_ctrl_ptr->main_menu_theme.focus_item_h);

    if (0 == menu_ctrl_ptr->icon_move_layer_handle.block_id
        || UILAYER_NULL_HANDLE == menu_ctrl_ptr->icon_move_layer_handle.block_id)
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = disp_rect.left;
        create_info.offset_y = disp_rect.top;
        create_info.width = (disp_rect.right + 1 - disp_rect.left);
        create_info.height = (disp_rect.bottom + 1 - disp_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;

        UILAYER_CreateLayer(&create_info, &menu_ctrl_ptr->icon_move_layer_handle);

        UILAYER_SetLayerColorKey(&(menu_ctrl_ptr->icon_move_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);
    }
}

/*****************************************************************************/
//  Description : release
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseMoveIconLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->icon_move_layer_handle);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetBltMoveLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (UILAYER_IsLayerActive(&(menu_ctrl_ptr->icon_move_layer_handle)))
    {
        append_layer.lcd_dev_info = s_item_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        append_layer.lcd_dev_info = menu_ctrl_ptr->icon_move_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    return;
}

/*****************************************************************************/
//  Description : redraw move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuRedrawMoveLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr,  //in
    uint16 index,
    GUI_RECT_T update_rect
)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    GUI_RECT_T          disp_rect = {0};
    GUI_RECT_T          layer_rect = {0};

    disp_rect = GUI_GetCenterRectExt(update_rect,
                                     menu_ctrl_ptr->main_menu_theme.focus_item_w,
                                     menu_ctrl_ptr->main_menu_theme.focus_item_h);

    layer_rect.left = 0;
    layer_rect.top = 0;
    layer_rect.right = (disp_rect.right - disp_rect.left);
    layer_rect.bottom = (disp_rect.bottom - disp_rect.top);

    lcd_dev_info = menu_ctrl_ptr->icon_move_layer_handle;

    UILAYER_SetLayerPosition(&lcd_dev_info, disp_rect.left, disp_rect.top);

    UILAYER_Clear(&lcd_dev_info);

    MatrixMenuDrawItem(menu_ctrl_ptr, &lcd_dev_info, index, FALSE);
    return;
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMatrixMenuIconToIdle(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (PNULL != menu_ctrl_ptr &&
        menu_ctrl_ptr->main_menu_theme.is_drag_item2idle &&
        menu_ctrl_ptr->is_item_pressed)
    {
        GUI_POINT_T  current_point = {0};
        LCD_ANGLE_E  current_angle = LCD_ANGLE_0;
        LCD_ANGLE_E  idlewin_angle = LCD_ANGLE_0;
        int16 temp = 0;
        MMI_TP_STATUS_E state = MMI_TP_NONE;
        uint16  lcd_width = 0;
        uint16  lcd_height = 0;
#ifdef TOUCH_PANEL_SUPPORT
        MMK_GetLogicTPMsg(&state, &current_point);
#endif
        MMK_GetWinAngle(MAIN_IDLE_WIN_ID, &idlewin_angle);
        MMK_GetWinAngle(MAIN_MAINMENU_WIN_ID, &current_angle);
        GUILCD_GetWidthHeightByAngle(MAIN_LCD_ID, idlewin_angle, &lcd_width, &lcd_height);

        switch (current_angle - idlewin_angle)      /*lint !e656*/

        {
            case LCD_ANGLE_90:
                temp  = current_point.x;
                current_point.x = (int16)(lcd_width - current_point.y);
                current_point.y = (int16)temp;
                break;

            case LCD_ANGLE_180:
                current_point.x = (int16)(lcd_width - current_point.x);
                current_point.y = (int16)(lcd_height - current_point.y);
                break;

            case LCD_ANGLE_270:
                temp  = current_point.y;
                current_point.y = (int16)(lcd_height - current_point.x);
                current_point.x = (int16)(temp);
                break;

            default:
                break;
        }

        //如果menu_id为0时，不会传到idle。
        MMK_SendMsg(menu_ctrl_ptr->win_handle, MSG_NOTIFY_GRID_SHORTCUT2IDLE, (DPARAM)&current_point);

        result = MMI_RESULT_TRUE;
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleMatrixMenuGrayEvent(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    BOOLEAN *is_gray_ptr = (BOOLEAN *)param;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    if (MSG_APP_OK == msg_id || MSG_APP_WEB == msg_id || MSG_TP_PRESS_UP == msg_id)
    {
        if (PNULL != is_gray_ptr && (*is_gray_ptr))
        {
            MatrixMenuResetIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

            if (!menu_ctrl_ptr->main_menu_theme.is_hl_disappear  || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                MatrixMenuDrawActiveIcon(menu_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMatrixMenuSpecialMsg(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#ifdef UI_MULTILAYER_SUPPORT
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
#endif

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    HandleMatrixMenuGrayEvent(menu_ctrl_ptr, msg_id, param);

    switch (msg_id)
    {
        case MSG_CTL_LOSE_FOCUS:
            menu_ctrl_ptr->is_move_state = FALSE;
            menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;
            //若前一帧为direct draw，需要重画
#ifdef UI_MULTILAYER_SUPPORT

            if (UILAYER_IsPerDirectDraw())
            {
                GUI_RECT_T menu_rect = {0};

                GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &menu_rect);

                //display bg
                MatrixMenuDrawBg(menu_rect, menu_ctrl_ptr, PNULL);

                // 显示title
                MatrixMenuDisplayActiveItemTitle(menu_ctrl_ptr);

                // 显示滚动条
                MatrixMenuDrawScrollBar(menu_ctrl_ptr, TRUE);

                //set softkey param
                MatrixMenuSetSoftkey(menu_ctrl_ptr);

                // display icon item
                if (!menu_ctrl_ptr->is_move_state
                    && (!menu_ctrl_ptr->main_menu_theme.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())))
                {
                    MatrixMenuDrawHighlightItem(menu_ctrl_ptr);
                }

                MatrixMenuDrawItemIcon(menu_ctrl_ptr, &lcd_dev_info);
                MatrixMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);
            }

#endif
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
            MatrixMenuReleaseRollLayer();
#endif
#endif
            break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_LONG:
            result = HandleMatrixMenuIconToIdle(menu_ctrl_ptr, msg_id, param);
            break;
#endif//TOUCH_PANEL_SUPPORT

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : MatrixMenuCreateRollSrcLayer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateRollSrcLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr  //in
)
{
    UILAYER_CREATE_T    create_info = {0};
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    GUI_RECT_T          menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    //get selected bar display rect
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

    s_rollSrc_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_rollSrc_layer_handle.block_id = UILAYER_NULL_HANDLE;

    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = (menu_rect.right + 1 - menu_rect.left);
    create_info.height = 5 * (menu_rect.bottom + 1 - menu_rect.top);
    create_info.is_bg_layer = FALSE;

    create_info.is_static_layer = FALSE;
    UILAYER_CreateLayer(
        &create_info,
        &s_rollSrc_layer_handle
    );
    // 设置color key
    UILAYER_SetLayerColorKey(&s_rollSrc_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(&s_rollSrc_layer_handle);
    s_is_all = ROLL_DRAW_NONE;
    return;
}

/****************************************************************************/
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseRollLayer(void)
{
    if (s_rollSrc_layer_handle.block_id != UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_rollSrc_layer_handle);   /*lint !e506 !e774*/
    }
}
#endif

/*****************************************************************************/
// Description : MatrixMenuCreateRollEffect
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuCreateRollEffect(void)
{
    SE_ROLL_INIT_PARAM_T init = {0};

    init.format         = IMGREF_FORMAT_ARGB888;
    MatrixMenuDestroyRollEffect(s_roll_effect_handle);
    s_roll_effect_handle = SE_Create(SE_ID_ROLL, &init);

    if (NULL == s_roll_effect_handle)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// Description : MatrixMenuDestroyRollEffect
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void MatrixMenuDestroyRollEffect(SE_HANDLE handle)
{
    if (NULL != handle)
    {
        SE_Destory(handle);
        handle = NULL;
    }
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
// Description : MatrixMenuRenderRollEffect
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MatrixMenuRenderRollEffect(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    SE_ROLL_RENDER_IN_PARAM_T   input   = {0};
    SE_ROLL_RENDER_OUT_PARAM_T  output  = {0};
    SE_RESULT_E                 ret = SE_RESULT_SUCCESS;
    UILAYER_INFO_T src_layer_info = {0};
    UILAYER_INFO_T tag_layer_info = {0};
    uint8 *src_layer_buf_ptr = PNULL;
    GUI_RECT_T menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);
    uint8 *src = PNULL;
    uint32 ptr_offset = 0;
    int16 h_offset = 0;

    UILAYER_GetLayerInfo(&s_rollSrc_layer_handle, &src_layer_info);
    src_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_rollSrc_layer_handle);

    if (PNULL == src_layer_buf_ptr)
    {
        return FALSE;
    }

    UILAYER_GetLayerInfo(&s_item_layer_handle, &tag_layer_info);
    UILAYER_Clear(&s_item_layer_handle);
    MatrixMenuDrawRollSrcLayer(menu_ctrl_ptr);

    input.src_size.w = src_layer_info.layer_width;
    input.src_size.h = src_layer_info.layer_height * 3 / 5;
    ptr_offset = src_layer_info.layer_height * src_layer_info.layer_width * 4 / 5;
    h_offset = s_effect_offset - src_layer_info.layer_height / 5;

    if (h_offset > 0)
    {
        src = src_layer_buf_ptr + ptr_offset + h_offset * src_layer_info.layer_width * 4;
    }
    else
    {
        h_offset = 0 - h_offset;
        src = src_layer_buf_ptr + ptr_offset - h_offset * src_layer_info.layer_width * 4;
    }

    input.src_buf_ptr = (uint32 *)src;

    input.src_hot_line[0] = src_layer_info.layer_height / 5;
    input.src_hot_line[1] = input.src_hot_line[0] + src_layer_info.layer_height / 5;

    input.target_hot_line[0] = menu_rect.top;
    input.target_hot_line[1] = input.target_hot_line[0] + src_layer_info.layer_height / 5;

    input.target_buf_ptr = (uint32 *)UILAYER_GetLayerBufferPtr(&s_item_layer_handle);
    input.target_size.w = tag_layer_info.layer_width;
    input.target_size.h = tag_layer_info.layer_height;

    if (PNULL == input.target_buf_ptr)
    {
        return FALSE;
    }

    input.angle[0] = input.angle[1] = 140;
    input.eye.x = input.target_size.w / 2;
    input.eye.y = input.target_size.h / 2;
    input.eye.z = -(1 * input.target_size.h);

    ret = SE_Render(s_roll_effect_handle, &input, &output);

    if (SE_RESULT_SUCCESS != ret)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMENU_ICON_8290_112_2_18_2_33_42_134, (uint8 *)"d", ret);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : MatrixMenuBltItemLayerToAppointLayer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuBltItemLayerToAppointLayer(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO   *dev_info_ptr
)
{
    int16               row = 0; // 行
    int16               line_index = 0; // 列
    int16               start_item_index = 0; // 列
    int16               item_index = 0;
    int16               page_item_num = 0;
    GUI_RECT_T          disp_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          menu_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    IMG_EnableTransparentColor(TRUE);

    menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);

    page_item_num = MatrixMenuGetIconMenuPageItemNum(menu_ctrl_ptr);

    lcd_dev_info = *dev_info_ptr;

    MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);

    for (line_index = s_start_line_index; line_index <= s_end_line_index; line_index++)
    {
        start_item_index = MatrixMenuGetLineStartItemIndex(menu_ctrl_ptr, line_index);

        for (row = 0; row < menu_ctrl_ptr->icon_data.page_col_num; row++)
        {
            item_index = start_item_index + row;

            if (item_index == menu_ctrl_ptr->tmp_cur_item_index && menu_ctrl_ptr->is_moving)
            {
                continue;
            }

            if (item_index < menu_ctrl_ptr->item_total_num)
            {
                // 计算显示区域
                disp_rect = MatrixMenuGetTotalItemRect(menu_ctrl_ptr, item_index);

                if (menu_ctrl_ptr->menu_list_info.item_count > item_index
                    && GUI_IntersectRect(&cross_rect, disp_rect, menu_rect))
                {
                    MatrixMenuDrawItem(menu_ctrl_ptr, &lcd_dev_info, item_index, FALSE);
                }
            }
        }
    }

    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : MatrixMenuBltItemLayerToAppointLayer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawRollSrcLayer(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
)
{
    int16 offset_x = 0;
    int16 offset_y = 0;
    UILAYER_INFO_T src_layer_info = {0};
    uint8 *src_layer_buf_ptr = PNULL;
    GUI_RECT_T menu_rect = MatrixMenuAdjuestRect(menu_ctrl_ptr);
    int16 old_menu_offset_y = menu_ctrl_ptr->offset_y;
    uint16 menu_height = menu_rect.bottom - menu_rect.top + 1;
    int16 move = 0;
    GUI_RECT_T rect = {0};

    offset_x = menu_rect.left;

    UILAYER_GetLayerInfo(&s_rollSrc_layer_handle, &src_layer_info);
    src_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_rollSrc_layer_handle);

    if (ROLL_DRAW_NONE != s_is_all)
    {
        move = menu_ctrl_ptr->offset_y - s_init_menuoffset;

        //offset过大,重新绘制
        if (abs(move) > src_layer_info.layer_height / 5 * 2)
        {
            s_is_all = ROLL_DRAW_NONE;
        }
    }


    if (ROLL_DRAW_NONE == s_is_all)
    {
        s_is_all = ROLL_DRAW_FIRST;
        UILAYER_Clear(&s_rollSrc_layer_handle);

        menu_ctrl_ptr->offset_y = old_menu_offset_y + menu_height;
        offset_y = menu_rect.top - src_layer_info.layer_height / 5;
        UILAYER_SetLayerPosition(&s_rollSrc_layer_handle, offset_x, offset_y);
        MatrixMenuBltItemLayerToAppointLayer(menu_ctrl_ptr, &s_rollSrc_layer_handle);

        menu_ctrl_ptr->offset_y = old_menu_offset_y - menu_height;
        offset_y = menu_rect.top - (src_layer_info.layer_height / 5) * 3;
        UILAYER_SetLayerPosition(&s_rollSrc_layer_handle, offset_x, offset_y);
        MatrixMenuBltItemLayerToAppointLayer(menu_ctrl_ptr, &s_rollSrc_layer_handle);

        menu_ctrl_ptr->offset_y = old_menu_offset_y;
        offset_y = menu_rect.top - (src_layer_info.layer_height / 5) * 2;
        UILAYER_SetLayerPosition(&s_rollSrc_layer_handle, offset_x, offset_y);
        MatrixMenuBltItemLayerToAppointLayer(menu_ctrl_ptr, &s_rollSrc_layer_handle);

        s_effect_offset = s_init_effectOffset = src_layer_info.layer_height / 5;
        s_init_menuoffset = old_menu_offset_y;
    }
    else
    {
        if (ROLL_DRAW_FIRST == s_is_all)
        {
            s_is_all = ROLL_DRAW_SECOND;
            menu_ctrl_ptr->offset_y = s_init_menuoffset + 2 * menu_height;
            offset_y = menu_rect.top;
            UILAYER_SetLayerPosition(&s_rollSrc_layer_handle, offset_x, offset_y);
            MatrixMenuBltItemLayerToAppointLayer(menu_ctrl_ptr, &s_rollSrc_layer_handle);

            menu_ctrl_ptr->offset_y = s_init_menuoffset - 2 * menu_height;
            offset_y = menu_rect.top - (src_layer_info.layer_height / 5) * 4;
            UILAYER_SetLayerPosition(&s_rollSrc_layer_handle, offset_x, offset_y);
            MatrixMenuBltItemLayerToAppointLayer(menu_ctrl_ptr, &s_rollSrc_layer_handle);

            menu_ctrl_ptr->offset_y = old_menu_offset_y;
        }

        MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);
        move = menu_ctrl_ptr->offset_y - s_init_menuoffset;

        if (abs(move) <= src_layer_info.layer_height / 5)
        {
            s_effect_offset = s_init_effectOffset - move;
        }
        else
        {
            if (move > 0)
            {
                uint8 *dst = PNULL;
                int32 count = src_layer_info.layer_height * src_layer_info.layer_width * 16 / 5;

                if (PNULL != src_layer_buf_ptr)
                {
                    dst = src_layer_buf_ptr + src_layer_info.layer_height / 5 * src_layer_info.layer_width * 4;
                    memmove(dst, src_layer_buf_ptr, count);
                }

                old_menu_offset_y = menu_ctrl_ptr->offset_y;
                menu_ctrl_ptr->offset_y = s_init_menuoffset + 3 * menu_height;
                offset_y = menu_rect.top;
                UILAYER_SetLayerPosition(&s_rollSrc_layer_handle, offset_x, offset_y);
                rect = menu_rect;
                rect.bottom = rect.top + src_layer_info.layer_height / 5 - 1;
                UILAYER_ClearRect(&s_rollSrc_layer_handle, rect);
                MatrixMenuBltItemLayerToAppointLayer(menu_ctrl_ptr, &s_rollSrc_layer_handle);
                menu_ctrl_ptr->offset_y = old_menu_offset_y;
                MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);

                s_init_menuoffset = s_init_menuoffset + src_layer_info.layer_height / 5;
                s_effect_offset = s_init_effectOffset + src_layer_info.layer_height / 5 - move;
            }
            else
            {
                uint8 *src = PNULL;
                int32 count = src_layer_info.layer_height * src_layer_info.layer_width * 16 / 5;

                if (PNULL != src_layer_buf_ptr)
                {
                    src = src_layer_buf_ptr + src_layer_info.layer_height / 5 * src_layer_info.layer_width * 4;
                    memmove(src_layer_buf_ptr, src, count);
                }

                old_menu_offset_y = menu_ctrl_ptr->offset_y;
                menu_ctrl_ptr->offset_y = s_init_menuoffset - 3 * menu_height;
                offset_y = menu_rect.top - (src_layer_info.layer_height / 5) * 4;
                UILAYER_SetLayerPosition(&s_rollSrc_layer_handle, offset_x, offset_y);
                rect = menu_rect;
                rect.bottom = rect.top + src_layer_info.layer_height / 5 - 1;
                UILAYER_ClearRect(&s_rollSrc_layer_handle, rect);
                MatrixMenuBltItemLayerToAppointLayer(menu_ctrl_ptr, &s_rollSrc_layer_handle);
                menu_ctrl_ptr->offset_y = old_menu_offset_y;
                MatrixMenuGetTopAndBottomLineIndex(menu_ctrl_ptr);

                s_init_menuoffset = s_init_menuoffset - src_layer_info.layer_height / 5;
                s_effect_offset = s_init_effectOffset - src_layer_info.layer_height / 5 - move;

            }
        }
    }
}

/*****************************************************************************/
//  Description : Create Status Bar Layer
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    UILAYER_CREATE_T     create_info = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    GUI_RECT_T disp_rect = {0};

    if ((0 == s_homebutton_layer_handle.block_id)
        || (UILAYER_NULL_HANDLE == s_homebutton_layer_handle.block_id))
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_COMMON_HOME_ICON, menu_ctrl_ptr->win_handle);
        disp_rect = MMITHEME_GetIconMenuButtonRect();

        disp_rect = GUI_GetCenterRectExt(disp_rect, icon_width, icon_height);

        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->win_handle;


        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = disp_rect.right - disp_rect.left + 1;
        create_info.height = disp_rect.bottom - disp_rect.top + 1;


        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;

        create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;

        UILAYER_CreateLayer(
            &create_info,
            &s_homebutton_layer_handle
        );

        UILAYER_SetLayerColorKey(&s_homebutton_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

        UILAYER_Clear(&s_homebutton_layer_handle);
    }

    UILAYER_SetLayerPosition(&s_homebutton_layer_handle, disp_rect.left, disp_rect.top);

    return;
}

/*****************************************************************************/
//  Description : Create Status Bar Layer
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateActiveItemTitleLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    UILAYER_CREATE_T     create_info = {0};
    GUI_RECT_T disp_rect = {0};

    if (((0 == s_activeTitle_layer_handle.block_id)
         || (UILAYER_NULL_HANDLE == s_activeTitle_layer_handle.block_id))
        && !GUI_IsRectEmpty(menu_ctrl_ptr->icon_data.title_text_rect))
    {
        disp_rect = menu_ctrl_ptr->icon_data.title_text_rect;

        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->win_handle;


        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = disp_rect.right - disp_rect.left + 1;
        create_info.height = disp_rect.bottom - disp_rect.top + 1;


        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;

        UILAYER_CreateLayer(
            &create_info,
            &s_activeTitle_layer_handle
        );

        UILAYER_SetLayerColorKey(&s_activeTitle_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

        UILAYER_Clear(&s_activeTitle_layer_handle);
    }

    UILAYER_SetLayerPosition(&s_activeTitle_layer_handle, disp_rect.left, disp_rect.top);

    return;
}

/****************************************************************************/
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseStatusBarAndHomeButtonLayer(void)
{
    if (s_homebutton_layer_handle.block_id != UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_homebutton_layer_handle);   /*lint !e506 !e774*/
    }

    if (s_activeTitle_layer_handle.block_id != UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_activeTitle_layer_handle);   /*lint !e506 !e774*/
    }
}
/*****************************************************************************/
//  Description : Create Status Bar Layer
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetStatusBarAndHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    IGUICTRL_T    *stb_ctr_ptr  = (IGUICTRL_T *)MMK_GetCtrlPtrByWin(menu_ctrl_ptr->win_handle, MMITHEME_GetStatusBarCtrlId());
    IGUICTRL_T    *button_ctr_ptr  = (IGUICTRL_T *)(menu_ctrl_ptr->icon_data.data_ptr);

    GUIWIN_SetStbState(menu_ctrl_ptr->win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
    GUIWIN_SetStbBgIsDesplay(menu_ctrl_ptr->win_handle, TRUE);

    if (UILAYER_NULL_HANDLE != s_homebutton_layer_handle.block_id)
    {
        GUICTRL_SetLcdDevInfo(button_ctr_ptr, &s_homebutton_layer_handle);
    }
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuAppendStatusBarAndHomeButtonLayer(void)
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (0 != s_homebutton_layer_handle.block_id)
    {
        UILAYER_RemoveBltLayer(&s_homebutton_layer_handle);
    }

    lcd_dev_info.block_id = s_homebutton_layer_handle.block_id;

    if (0 != s_homebutton_layer_handle.block_id && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    if (0 != s_activeTitle_layer_handle.block_id)
    {
        UILAYER_RemoveBltLayer(&s_activeTitle_layer_handle);
    }

    lcd_dev_info.block_id = s_activeTitle_layer_handle.block_id;

    if (0 != s_activeTitle_layer_handle.block_id && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    return;
}

/*****************************************************************************/
//  Description : display active icon
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDisplayActiveItemTitleEx(
    MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr
        && !GUI_IsRectEmpty(menu_ctrl_ptr->icon_data.title_text_rect)
        && (UILAYER_IsLayerActive(&s_activeTitle_layer_handle)))
    {
        MMI_STRING_T        item_str = {0};
        GUI_COLOR_T         item_color = 0;
        GUISTR_STYLE_T      str_style = {0};
        GUISTR_STATE_T      str_state = GUISTR_STATE_EFFECT | GUISTR_STATE_ELLIPSIS | GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
        CTRLMAINMENU_ITEM_T menuitem_info = {0};

        UILAYER_Clear(&s_activeTitle_layer_handle);

        item_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);

        MatrixMenuGetItemInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &menuitem_info);

        if (CTRL_TEXT_ID == menuitem_info.text.type)
        {
            //display item text
            MMI_GetLabelTextByLang(menuitem_info.text.data.text_id, &item_str);
        }
        else
        {
            item_str.wstr_len = menuitem_info.text.data.str_info.wstr_len;
            item_str.wstr_ptr = menuitem_info.text.data.str_info.wstr_ptr;
        }

        str_style.effect = FONT_EFFECT_REVERSE_EDGE;
        str_style.align = ALIGN_HVMIDDLE;
        str_style.font = menu_ctrl_ptr->main_menu_theme.title_font;
        str_style.font_color = item_color;

        //display text
        GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO *)&s_activeTitle_layer_handle,
                                   (const GUI_RECT_T *)&menu_ctrl_ptr->icon_data.title_text_rect,
                                   (const GUI_RECT_T *)&menu_ctrl_ptr->icon_data.title_text_rect,
                                   (const MMI_STRING_T *)&item_str,
                                   &str_style,
                                   str_state,
                                   GUISTR_TEXT_DIR_AUTO);

    }
}
#endif
#elif defined MMI_PDA_SUPPORT //NEWMS00161615
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuCreateHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    UILAYER_CREATE_T     create_info = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    GUI_RECT_T disp_rect = {0};

    if ((0 == s_homebutton_layer_handle.block_id)
        || (UILAYER_NULL_HANDLE == s_homebutton_layer_handle.block_id))
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_COMMON_HOME_ICON, menu_ctrl_ptr->win_handle);
        disp_rect = MMITHEME_GetIconMenuButtonRect();
        disp_rect = GUI_GetCenterRectExt(disp_rect, icon_width, icon_height);

        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->win_handle;


        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = disp_rect.right - disp_rect.left + 1;
        create_info.height = disp_rect.bottom - disp_rect.top + 1;


        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;

        create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;

        UILAYER_CreateLayer(
            &create_info,
            &s_homebutton_layer_handle
        );

        UILAYER_SetLayerColorKey(&s_homebutton_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

        UILAYER_Clear(&s_homebutton_layer_handle);
    }

    UILAYER_SetLayerPosition(&s_homebutton_layer_handle, disp_rect.left, disp_rect.top);
    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuAppendHomeButtonLayer(
    void
)
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (0 != s_homebutton_layer_handle.block_id)
    {
        UILAYER_RemoveBltLayer(&s_homebutton_layer_handle);
    }

    lcd_dev_info.block_id = s_homebutton_layer_handle.block_id;

    if (0 != s_homebutton_layer_handle.block_id && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuSetHomeButtonLayer(
    MMIMAINMENU_MATRIX_DATA_T   *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && PNULL != menu_ctrl_ptr->icon_data.data_ptr)
    {
        IGUICTRL_T    *button_ctr_ptr  = (IGUICTRL_T *)(menu_ctrl_ptr->icon_data.data_ptr);

        if (UILAYER_NULL_HANDLE != s_homebutton_layer_handle.block_id)
        {
            GUICTRL_SetLcdDevInfo(button_ctr_ptr, &s_homebutton_layer_handle);
        }
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuReleaseHomeButtonLayer(void)
{
    if (s_homebutton_layer_handle.block_id != UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_homebutton_layer_handle);
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawBg(
    GUI_RECT_T        bg_rect,
    MMIMAINMENU_MATRIX_DATA_T    *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO  *lcd_dev_info_ptr
)
{
    //GUI_BG_T            common_bg = {0};
    MMI_IMAGE_ID_T      bg_img_id = menu_ctrl_ptr->main_menu_theme.bg_image;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (PNULL == lcd_dev_info_ptr)
    {
        CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    }
    else
    {
        lcd_dev_info = *lcd_dev_info_ptr;
    }

#ifdef BG_ON_BOTTOM_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable())
    {
        if (MMITHEME_GetBgImgId() == bg_img_id)
        {
            //通常背景
            MMITHEME_DisplayBg(&bg_rect, menu_ctrl_ptr->win_handle, &lcd_dev_info);
        }
        else if (0 != bg_img_id)
        {
            GUIRES_DisplayImg(PNULL,
                              &bg_rect,
                              &bg_rect,
                              menu_ctrl_ptr->win_handle,
                              bg_img_id,
                              (const GUI_LCD_DEV_INFO *)&lcd_dev_info);
        }
        else
        {
            GUI_FillRect(&lcd_dev_info,
                         bg_rect,
                         menu_ctrl_ptr->main_menu_theme.bg_color);
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
                              (const GUI_LCD_DEV_INFO *)&lcd_dev_info);
        }
        else
        {
            GUI_FillRect(&lcd_dev_info,
                         bg_rect,
                         menu_ctrl_ptr->main_menu_theme.bg_color);
        }
    }
	GUISTATUSBAR_UpdateCtrl(menu_ctrl_ptr->win_handle,TRUE);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: violent.wei
//  Note: 当有focus item变化时，朗读list内容
/*****************************************************************************/
LOCAL void HandleTextToSpeech(
                           MMIMAINMENU_MATRIX_DATA_T     *menu_ctrl_ptr  //in
                           )
{
    if (MMITheme_IsControlTTSOpen())
    {
        // 当focus page或focus item有变化时，才进行朗读
        if (menu_ctrl_ptr->pre_msg_loop_page_index != menu_ctrl_ptr->cur_page_index ||
            menu_ctrl_ptr->pre_msg_loop_item_index != menu_ctrl_ptr->cur_item_index ||
            menu_ctrl_ptr->pre_msg_loop_menu_level != menu_ctrl_ptr->menu_level)
        {
            MMI_STRING_T         wstring = {0};
            CTRLMAINMENU_ITEM_T  menuitem_info = {0};

            MatrixMenuGetItemInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &menuitem_info);

            if(CTRL_TEXT_BUFFER_USER == menuitem_info.text.type)
            {
                wstring.wstr_ptr = menuitem_info.text.data.str_info.wstr_ptr;
                wstring.wstr_len = menuitem_info.text.data.str_info.wstr_len;
            }
            else if(CTRL_TEXT_ID == menuitem_info.text.type)
            {
                MMITHEME_GetLabelTextByLang(menuitem_info.text.data.text_id, &wstring);
            }
            
            MMITheme_PlayControlText(&wstring, 1);
            menu_ctrl_ptr->pre_msg_loop_page_index = menu_ctrl_ptr->cur_page_index;
            menu_ctrl_ptr->pre_msg_loop_item_index = menu_ctrl_ptr->cur_item_index;
            menu_ctrl_ptr->pre_msg_loop_menu_level = menu_ctrl_ptr->menu_level;
        }
    }
}

/*****************************************************************************/
//  Description : Matrix Handle message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL MMI_RESULT_E MatrixMenuHandle(
    void *pm_data_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr = (MMIMAINMENU_MATRIX_DATA_T *)pm_data_ptr;
    GUI_POINT_T point = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        // 初始值
        menu_ctrl_ptr->pre_msg_loop_page_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_item_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_menu_level = (uint16)-1;          
        break;

    case MSG_CTL_PAINT:
        MatrixMenuDraw((MMIMAINMENU_MATRIX_DATA_T *)menu_ctrl_ptr);
        if (menu_ctrl_ptr->enter_anim_type > GUIMENU_ENTER_ANIM_NONE && menu_ctrl_ptr->enter_anim_type < GUIMENU_ENTER_ANIM_MAX)
        {
#ifdef EFFECT_ZOOM_SUPPORT
            MMIDEFAULT_SetCurWin3DEffect(MMI_WIN_ANIM_ZOOM_IN);
#endif
        }
        
        HandleTextToSpeech(menu_ctrl_ptr);
        break;
        
    case MSG_CTL_GET_FOCUS:
        // 初始值
        menu_ctrl_ptr->pre_msg_loop_page_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_item_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_menu_level = (uint16)-1;
        break;
        
    case MSG_CTL_LOSE_FOCUS:
        MMITheme_StopControlText();
        menu_ctrl_ptr->is_moving = FALSE;
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        HandleMatrixMenuSpecialMsg(menu_ctrl_ptr, msg_id, param);
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_CLOSE:
        MMITheme_StopControlText();
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_moving)
        {
            MatrixMenuHandleOkKey(menu_ctrl_ptr, msg_id);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;
        
    case MSG_NOTIFY_CANCEL:
    case MSG_APP_CANCEL:
        if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_moving)
        {
            HandleMatrixMenuCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;
       
        
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
        {
            MMITheme_StopControlText();
            HandleMatrixMenuDirectionKey(menu_ctrl_ptr,msg_id);
            result = MMI_RESULT_TRUE;
			HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;
        
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
        {
            HandleMatrixMenuNumKey(menu_ctrl_ptr->item_total_num, menu_ctrl_ptr);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT 
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        MMITheme_StopControlText();
        HandleMatrixMenuTpDown(&point, menu_ctrl_ptr);
        HandleTextToSpeech(menu_ctrl_ptr);
        break;
        
    case MSG_TP_PRESS_UP:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        HandleMatrixMenuTpUp(&point, menu_ctrl_ptr);
        HandleTextToSpeech(menu_ctrl_ptr);
        break;
        
    case MSG_TP_PRESS_MOVE:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        HandleMatrixMenuTpMove(&point, menu_ctrl_ptr);
        break;
        
    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_SHORT:
        // 处理tp short 消息
        HandleMatrixMenuSpecialMsg(menu_ctrl_ptr, msg_id, param);
        break;
        
    case MSG_NOTIFY_PENOK:
        if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_moving)
        {
            HandleMatrixMenuCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        else
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, GUIMENU_1548_112_2_18_3_20_20_236, (uint8 *)"dd", menu_ctrl_ptr->slide_state, menu_ctrl_ptr->is_moving);
        }
        break;

    case MSG_CTRLMAINMENU_SET_MOVE_STATE:
        menu_ctrl_ptr->is_move_state = TRUE;
        MatrixMenuMoveInit(menu_ctrl_ptr);
        break;

    case MSG_TIMER:
        HandleMatrixMenuTimer(menu_ctrl_ptr, msg_id, param);
        break;  
#endif // TOUCH_PANEL_SUPPORT
        
    case MSG_CTRLMAINMENU_CANCEL_MOVE_ICON:
        HandleMatrixMenuCancelKey(menu_ctrl_ptr, MSG_CTRLMAINMENU_CANCEL_MOVE_ICON);
        break;
        
    case MSG_CTRLMAINMENU_GET_ID:
        {
            MMIMAINMENU_ITEM_INFO_T *item_info_ptr = (MMIMAINMENU_ITEM_INFO_T*)param;
            CTRLMAINMENU_ITEM_T cur_item_info = {0};
            
            MatrixMenuGetItemInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, &cur_item_info);
            
            item_info_ptr->group_id = cur_item_info.group_id;
            item_info_ptr->menu_id = cur_item_info.menu_id;
        }
        break;
        
    case MSG_CTRLMAINMENU_IS_MOVE_STATE:
        *((BOOLEAN*)param) = menu_ctrl_ptr->is_move_state;
        break;
        
    case MSG_CTRLMAINMENU_SET_RECT:
        MatrixMenuModifyRect(menu_ctrl_ptr);
        break;
        
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL void MatrixMenuRunItem(
    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr,
    uint16 item_index
)
{
    CTRLMAINMENU_ITEM_T item = {0};

    MatrixMenuGetItemInfo(menu_ctrl_ptr, item_index, &item);

    if (
#ifdef DYNAMIC_MAINMENU_SUPPORT
    !MMIMENU_DyRunItem(item.group_id,item.menu_id) && 
#endif
    PNULL != item.call_back_start
    )
    {
        item.call_back_start(item.param_data1, item.param_data2);
    }

    return;
}

/*****************************************************************************/
//  Description : Matrix Menu Draw Active Icon
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MatrixMenuDrawActiveIcon(
                                    MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr
                                    )
{
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable())
    {
        MatrixMenuSetBltHeightLightLayer(menu_ctrl_ptr);
#ifdef ROLL_MAINMENU_SUPPORT
        MatrixMenuAppendStatusBarAndHomeButtonLayer();
#endif
    }
#endif
    MatrixMenuDrawHighlightItem(menu_ctrl_ptr);

    if (MMIMAINMENU_MATRIX_TITLE_E == MMIAPISET_GetMainmenuStyle())
    {
#ifdef ROLL_MAINMENU_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    MatrixMenuDisplayActiveItemTitleEx(menu_ctrl_ptr);
#endif
#else
    MatrixMenuDisplayActiveItemTitle(menu_ctrl_ptr);
#endif
    }    
    
    return;
}
#endif
