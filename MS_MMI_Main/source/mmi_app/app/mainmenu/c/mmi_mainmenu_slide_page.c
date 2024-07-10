/*****************************************************************************
** File Name:      mmi_mainmenu_slide_page.c                                     *
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
#if PDA_UI_SUPPORT_MANIMENU_GO
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_mainmenu_data.h"
#include "mmi_mainmenu_internal.h"
#include "mmitheme_mainmenu.h"
#include "mmi_app_menu_trc.h"
#include "guimenu.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_modu_main.h"
#include "mmidisplay_data.h"
#include "guistring.h"
#include "mmi_image.h"
#include "mmi_link.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmi_mainmenu_export.h"
#include "mmk_tp.h"
#include "mmitheme_special.h"
#include "mmi_id.h"
#include "mmiset_display.h"
#include "mmi_common.h"
#include "mmk_app.h"
#include "mmifm_export.h"
#include "guistatusbar.h"
#include "u_launcher_export.h"
#include "graphics_3d_rotate.h"
#include "mmimp3_export.h"
#include "mmiset_export.h"
#include "mmi_menutable.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#endif
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "guictrl_api.h"
#include "mmiset_display.h"
#include "u_launcher_internal.h"
#include "mmi_mainmenu_u_se.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

typedef struct SLIDE_MEM_PAGE_INFO
{
    GUI_LCD_DEV_INFO layer_handle;
    uint8  index;//取值为0，1，2 始终以1为中心
    BOOLEAN is_change;
} SLIDE_MEM_PAGE_INFO_T;

typedef struct
{
    uint16 line_index;
    int32  line_offset_y;
} MMI_SLIDEMENU_POS_T;

typedef struct
{
    MMI_HANDLE_T                win_handle;                             //window handle
    MMI_HANDLE_T                ctrl_handle;

    GUIMENU_STATE_T             menu_state;
    GUIMENU_SLIDEMENU_DATA_T    slide_data;
    MMITHEME_MAINMENU_COMMON_T  main_menu_theme;                        // main menu theme
    MMIMAINEMENU_ITEM_LIST_T    menu_list_info;                         // menu list information

    MMI_HANDLE_T                button_handle[GUIMENU_BUTTON_NUM];      // button控件

    GUI_LCD_DEV_INFO            page_layer_handle[GUIMENU_SLIDE_PAGE_NUM];    //页面移动层1
    GUI_LCD_DEV_INFO            titleidx_layer_handle;                  // 页面索引层
    GUI_LCD_DEV_INFO            highlight_layer_handle;                 // 高亮层
    GUI_LCD_DEV_INFO            icon_move_layer_handle;                 // 图标移动层
    GUI_LCD_DEV_INFO            cube_layer_handle;                      //  立方体层
    GUI_LCD_DEV_INFO			to_idle_layer_handle;                   //到主菜单图层
    GUI_LCD_DEV_INFO			delete_layer_handle;                    //删除图层
    MMIMAINEMENU_PAGE_LIST_T    menu_page_info;                         //menu list information

    BOOLEAN                     is_move_state;                          // item是否在抖动
    BOOLEAN                     is_delete_state;                        // item是否在抖动
    BOOLEAN                     is_active;                              // is active
    BOOLEAN                     is_tp_pressed;                          // is tp down
    BOOLEAN                     is_item_pressed;                        // tp down时，是否在item上
    BOOLEAN                     is_empty_space;                         //tp up时，是否在item上

    MMK_TP_SLIDE_E              slide_state;                            // current slide state
    GUI_POINT_T                 slide_start_point;                      // Slide时候使用
    GUI_POINT_T                 slide_pre_point;                        // Slide时候使用
    GUI_POINT_T                 pre_tp_point;                           // 上一次tp move的坐标
    CONTINUE_MOVE_TYPE_E        continue_move_type;                     // 连续移动的类型

    uint8                       redrew_timer_id;                        // 滑动时，slide和fling状态下，画图的timer
    uint8                       init_item_timer_id;
    uint8                       shake_icon_timer_id;
    uint8                       start_move_timer_id;
    MMI_MENU_ID_T               delete_menu_id;                         // 组织状态下使用,要删除的menu_id

    uint16                      pre_cur_item_index;                     // previous current index
    uint16                      first_item_index;                       // item index at the top
    uint16                      tmp_width;                              // 移动的时候使用
    uint16                      tmp_height;                             // 移动的时候使用
    uint16                      tmp_cur_item_index;                     // 移动的时候使用
    uint16                      tmp_first_item_index;                   // 移动的时候使用
    uint16                      tmp_page_item_index;                    // 移动到时候使用

    float                       fling_velocity;                         // 滑动速度

    BOOLEAN                     is_display_title;                       // 是否显示title
    BOOLEAN                     is_display_item_layer;

    uint16                      item_total_num;
    uint16                      cur_page_index;                         // default 0,from 0 to max-1
    uint16                      cur_item_index;                         // current selected menu index. in option page style, it is also item index
#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
    GUI_LCD_DEV_INFO            layer_bg;
    GUIMENU_GO_SLIDE_TYPE_E     support_slide_type;                     // 主菜单支持的滑动类型
    uint8                       highlight_timer_id;                     //  highlight显示的timer ID
#endif
    uint16                      pre_msg_loop_page_index;                // 每次消息循环后，focus page index
    uint16                      pre_msg_loop_item_index;                // 每次消息循环后，focus项index
    uint16                      pre_msg_loop_menu_level;                // 每次消息循环后，focus项的level
    uint16                      menu_level;                             //menu level

} MMIMAINMENU_SLIDEPAGE_DATA_T;


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL GO_GUIMENU_ITEM_BUF_T        s_slidemenu_item_layer[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM][MMITHEME_GOMENU_ITEM_MAX_NUM] = {0};
LOCAL GO_MAINMENU_PAGE_INFO_T*     s_cur_page_info_ptr   = PNULL;
LOCAL GO_MAINMENU_PAGE_INFO_T*     s_next_page_info_ptr  = PNULL;
LOCAL FIX16_POINT_T*               s_calc_point_buf      = PNULL;
LOCAL BOOLEAN                      s_go_is_initialized   = FALSE;
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
LOCAL MMIMENU_SELECT_EFFECT_INFO_T s_mmimenu_select_effect_info = {0};
#endif
LOCAL SLIDE_MEM_PAGE_INFO_T        s_go_item_page_info[GO_MEM_PAGE_NUM] = {0};
LOCAL uint8*                       s_go_alloc_mem_ptr    = PNULL;
LOCAL S3D_EXPANDPTR_T*             s_go_calc_param_ptr   = PNULL;
LOCAL uint8*                       s_go_shining_ctrl_ptr = PNULL;
LOCAL uint32                       s_random_cur_style    = 0;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Slide Initialize
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL void*           SlidePageInitialize(
    MMI_HANDLE_T ctrl_handle
);

/*****************************************************************************/
//  Description : Slide Destory
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN         SlidePageDestory(
    void* pm_data_ptr
);
/*****************************************************************************/
//  Description : Slide Handle
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL MMI_RESULT_E    SlidePageHandle(
    void *pm_data_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : 设置点击特效类型INFO
//  Global resource dependence :
//  Author: harry
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetSelectEffectStyleInfo(
    MMIMENU_SELECT_EFFECT_STYLE_E    mainmenu_select_style
);
#endif
/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDraw(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
);
/*****************************************************************************/
//  Description : 设置点击特效类型INFO
//  Global resource dependence :
//  Author: harry
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetSelectEffectStyle(void);
/*****************************************************************************/
//  Description : 设置选中随机
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetRandomSelectEffectStyle(void);

/*****************************************************************************/
//  Description : handle slide page menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleMenuUpKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle slide page menu down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleMenuDownKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle slide page menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleMenuLeftKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
);

/*****************************************************************************/
//  Description : handle slide page menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleMenuRightKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
);

/*****************************************************************************/
//  Description : reset icon display
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuResetSlidePageIconDisplay(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint16 item_index,
    BOOLEAN is_disp_move_bg
);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuDirectionKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id,
    GO_MOVE_DIRECTION_E  direction
);
/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleMenuNumKey(
    uint16           num_index,
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle page touch panel press down msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleSlidePageTpDown(GUI_POINT_T       *tp_point_ptr,
        MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr
                                             );

/*****************************************************************************/
//  Description : handle slide page touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleSlidePageTpUp(GUI_POINT_T       *tp_point_ptr,
        MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
                                              );


/*****************************************************************************/
// Description : get item
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *GetSlidePageMenuLinkListPtr(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *qbtheme_ptr,
    uint32                       page_index
);

/*****************************************************************************/
//  Description : get slide client rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T SlidePageMenuAdjustSlideMenuRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
);
/*****************************************************************************/
// 	Description : search in list
//	Global resource dependence :
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
// LOCAL MMI_LINK_NODE_T *SlidePageMenuSearchSlideMenuItemByIndex(
//     GUIMENULIST_ITEM_T *menu_list_ptr,
//     uint16 item_index
// );


/*****************************************************************************/
//  Description : 根据一个item的索引号，获得该行的第一个item索引号
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 如果索引号大于总数，则返回无效的数值-1
/*****************************************************************************/
LOCAL int16 SlidePageMenuGetLineStartItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           line_index,
    uint16           page_index
);

/*****************************************************************************/
//  Description : 当前的Item是否在页中显示,因为当触笔移动滚动条的时候,
//                  当前Item可能会不在页面中显示
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsItemInPage(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           item_index,
    uint16           page_index
);

/*****************************************************************************/
//  Description : 根据Item索引号获得其在页面中的位置索引
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 如果不在页内,返回-1
/*****************************************************************************/
LOCAL int16 SlidePageMenuGetPageIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           item_index
);

/*****************************************************************************/
//  Description : 当前的Item是否在页中显示,因为当触笔移动滚动条的时候,
//                  当前Item可能会不在页面中显示
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 根据当前页中的索引号计算实际的item索引号,否则返回-1
/*****************************************************************************/
LOCAL int16 SlidePageMenuGetItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index
);

/*****************************************************************************/
//  Description : get line index of item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetItemLineIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint16 item_index
);
/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuNotifyParentMsg(
    CAF_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id
);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideMenuGrayEvent(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
/*****************************************************************************/
//  Description : 获取一页能够显示的item number
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetPageItemNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           item_total_num
);
/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDrawBg(
    GUI_RECT_T        bg_rect,
    MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr,
    GUI_LCD_DEV_INFO  *lcd_dev_info_ptr
);
/*****************************************************************************/
//  Description : 获取一页显示的最大item number
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetInPageMaxItemNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
);
/*****************************************************************************/
// 	Description : 销毁单，释放内存
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDestoryGOMenu(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleCancelKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
);

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoDisplaySlidePageMainLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : get next item and page index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGetNextItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    uint16             page_index,
    uint16             item_index,
    MMI_MESSAGE_ID_E   msg_id,
    uint16             *next_item_index_ptr,
    uint16             *next_page_index_ptr
);

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetTotalLineNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint32          page_index
);

/*****************************************************************************/
//  Description : 获得最后一行的item num
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetItemNumInLastLine(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint32          page_index
);

/*****************************************************************************/
//  Description : 获得一页的item number
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetPageTotalItemNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint32          page_index
);
/*****************************************************************************/
//  Description : stop item text timer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartHighlightIconDispTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopHighlightIconDispTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleTpMove(GUI_POINT_T       *tp_point_ptr,
                                     MMIMAINMENU_SLIDEPAGE_DATA_T*menu_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : 根据当前的Item获得当前页面的第一个Item索引
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGetFirstItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index,
    uint16           cur_item_index,
    uint16           *item_index_ptr
);

/*****************************************************************************/
//  Description : 根据当前的Item设置当前页面的第一个Item索引
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 只有在上下移动的时候当前item不在当前页时，调用该接口才返回TRUE
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuSetFirstItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           cur_page_index,
    uint16           cur_item_index
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,		//事件类型
    DPARAM           param          //附带的处理参数
);


/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateSlideMenuBgLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : CreateHighlightLayer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateHighlightIconLayer(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr);

/*****************************************************************************/
//  Description : create HIGHLIGHT layer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleaseHighlightIconLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : auto slide page layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuAutoSlidePage(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 first_page_index,
    uint16 next_page_index,
    GO_MOVE_DIRECTION_E  direction
);

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SlidePageMenuValidateCurPage(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : set move layer position
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetMoveLayerPosition(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
// 	Description : get item
//	Global resource dependence :
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGetItemInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *item_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleSpecialMsg(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM               param
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T SlidePageMenuGetSlideMenuMoveLayerRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDestoryItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : 读取nv中存储的菜单数据
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuReadItemInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_PAGE_ITEM_INFO_T *item_page_ptr
);

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuInitSlideMenu(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : up date rect
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: peilong.zhou
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStoreUpdateRect(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr);
/*****************************************************************************/
//  Description : init Menu data
//  Global resource dependence :
//  Author: peilong.zhou
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuInitData(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr);
/*****************************************************************************/
//  Description :绘制函数
//  Global resource dependence :
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDrawGOMenuLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,  //in
    uint16 first_page,
    uint16 second_page,
    GO_MOVE_DIRECTION_E direction,
    uint16 current_frame,
    BOOLEAN is_display_highlight
);
/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence :
//  Author: jin.wang
//  RETRUN:  返回指针
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *SlidePageMenuAddItemList(
    MMIMAINEMENU_PAGE_LIST_T     *menu_page_ptr,
    uint16                       item_num
);

/*****************************************************************************/
//  Description : SlidePageMenuDisplayActiveIcon
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplayActiveIcon(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    BOOLEAN is_display_highlight
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGOChangehandleandindex(
    SLIDE_MEM_PAGE_INFO_T* page_array_info,
    BOOLEAN is_tpmove_right
);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoUpdateItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    BOOLEAN is_tpmove_right
);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoCreateItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 icon_width,
    uint16 icon_height,
    uint16 item_total_num,
    GUI_LCD_DEV_INFO *layer_handle
);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGoInitItemLayerInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 page_index,
    GUI_LCD_DEV_INFO *layer_handle
);
/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopRedrawTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartRedrawTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
/*****************************************************************************/
//  Description : SlidePageMenuCheckLayerMem
//  Global resource dependence :
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuCheckLayerMem(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
/*****************************************************************************/
// Description:  是否拖动到主菜单/删除
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsInToIdleRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    BOOLEAN is_handle_tp,
    BOOLEAN *is_need_redraw_ptr
);

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplayItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleRandomStyle(
    MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : 算法输出BUF
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateCalcDstLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,  //in
    GUI_RECT_T       rect
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuMoveGoMenuPageLayerX(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T*    point_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuMoveGoMenuPageLayerY(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T*    point_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleGoMenuSlideOverX(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleGoMenuSlideOverY(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
);

/*****************************************************************************/
//  Description :获取GO桌面需要的页面信息
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetRowNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence :
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO SlidePageMenuGetCurrentPageLyaerInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description :获取GO桌面需要的页面信息
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetColNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : CreateHighlightLayer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplayHighlightIconLayer(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr);

/*****************************************************************************/
// Description : get item
// Global resource dependence :
//  Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGetItemPtr(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 page_index,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T * menuitem_ptr
);
/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleNextPage(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
);

/*****************************************************************************/
//  Description : item是否可以被删除
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsItemEnableDeleted(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
);
/*****************************************************************************/
//  Description : 处理组织功能
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 处理抖动timer
/*****************************************************************************/
LOCAL void SlidePageMenuHandleOrigEvent(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : 创建移动的buffer
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuMoveInit(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : get slide client rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T SlidePageMenuAdjustRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description :申请PAGE信息存放BUF
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuCreatGoMemory(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleaseMoveIconLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateMoveIconLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
);
/*****************************************************************************/
//  Description : SlidePageMenuStartInitItemTimer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartInitItemTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplaySlidePageItem(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    uint16             page_index,
    GUI_LCD_DEV_INFO   lcd_dev_info
);

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO SlidePageMenuDisplayOneItemToLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T         *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO *dst_dev_ptr,
    GUI_RECT_T             *rect_ptr,
    uint16                  page_index,
    uint16                  item_index
);
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartShakeIconTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopShakeIconTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuTransmitTpMsg(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *tp_point_ptr
);

LOCAL void SlidePageMenuDisplaysoftkeybg(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,GUI_LCD_DEV_INFO  *lcd_dev_info_ptr,BOOLEAN is_left);
/*****************************************************************************/
//  Description : handle button1 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleButton1Click( MMI_HANDLE_T button_handle);

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleButton2Click( MMI_HANDLE_T button_handle);

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleButtonDraw(
    GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr
);


/*****************************************************************************/
//  Description : 创建button
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateIconMenuButton(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
#endif
/*****************************************************************************/
//  Description : 读取nv中存储的宫格菜单数据
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuReadItemList(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 *item_num_ptr
);
/*****************************************************************************/
// 	Description : 初始化链表
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void SlidePageMenuResetItemListInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartMoveStartTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopMoveStartTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
/*****************************************************************************/
// Description : get item info in list
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T* SlidePageMenuGetItemInfoByIndex(
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description :设置初始化的状态
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoSetInitialized(
    BOOLEAN is_initialized
);
/*****************************************************************************/
//  Description :申请PAGE信息存放BUF
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGoReleaseMemory(void);

/*****************************************************************************/
//  Description : remove all menu layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuRemoveAllMenuLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
);
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuRunItem(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 item_index
);

#ifdef DYNAMIC_MAINMENU_SUPPORT

/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence :
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReloadItem( MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr);
#endif
/*****************************************************************************/
//  Description : check display init ok?
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _IsInitItemFinished(
								  MMIMAINMENU_SLIDEPAGE_DATA_T     *slidepage_ptr
								  );
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get item
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *GetItemInfoPtr(
    MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr,
    uint16                        page_index,
    uint16                        item_index
)
{
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T   *item_info_ptr = PNULL;

    if (PNULL == menu_ctrl_ptr)
    {
        return item_info_ptr;
    }

    page_item_info_ptr = MMIMENU_GetItemList(&menu_ctrl_ptr->menu_page_info, page_index);

    if (PNULL != page_item_info_ptr)
    {
        item_info_ptr = MMIMENU_GetItemInfo(page_item_info_ptr, item_index);
    }

    return item_info_ptr;
}
/*****************************************************************************/
//  Description : HandleMenuOkKey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleOkKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
)
{
    BOOLEAN                 is_handled = FALSE;
    BOOLEAN                 is_grayed = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != qbtheme_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->is_move_state)
    {
        is_handled = FALSE;
    }
    else
    {
        MMIMAINMENU_ITEM_INFO_T *item_info_ptr = {0};

        item_info_ptr = GetItemInfoPtr(
                            menu_ctrl_ptr,
                            menu_ctrl_ptr->cur_page_index,
                            menu_ctrl_ptr->cur_item_index);

        if (PNULL != item_info_ptr)
        {
            is_grayed = MMIMAINMENU_IsItemDisable(item_info_ptr->menu_id);

            if (!is_grayed)
            {
                // 在app接到ok以后，可能先干一些事情再关菜单，这样可能会导致关菜单时停止tts失败，
                // 所以在发通知之前就关tts
                MMITheme_StopControlText();

                if (!SlidePageMenuRunItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index))
                {
                    SlidePageMenuNotifyParentMsg(menu_ctrl_ptr->ctrl_handle, msg_id);
                }
            }
            else
            {
                HandleSlideMenuGrayEvent(menu_ctrl_ptr, msg_id, (DPARAM)&is_grayed);
            }
        }
    }

    return (is_handled);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuRunItem(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 item_index
)
{
    CTRLMAINMENU_ITEM_T item = {0};
    BOOLEAN  result = FALSE;
    uint16   item_index_info = 0;

    item_index_info = menu_ctrl_ptr->cur_page_index * menu_ctrl_ptr->slide_data.page_col_num*menu_ctrl_ptr->slide_data.page_row_num + item_index;
    SlidePageMenuGetItemInfo(menu_ctrl_ptr, item_index_info, &item);

    if (
#ifdef DYNAMIC_MAINMENU_SUPPORT
    !MMIMENU_DyRunItem(item.group_id,item.menu_id) && 
#endif    
    PNULL != item.call_back_start
    )
    {
        item.call_back_start(item.param_data1, item.param_data2);
        result = TRUE;
    }

    return result;
}
/*****************************************************************************/
//  Description : Slide Page Initialize
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL void*           SlidePageInitialize(
    MMI_HANDLE_T ctrl_handle
)

{
    MMI_HANDLE_T                   win_handle = 0;
    MMIMAINMENU_SLIDEPAGE_DATA_T * slide_data_ptr = PNULL;

    slide_data_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_SLIDEPAGE_DATA_T));

    if (PNULL == slide_data_ptr)
    {
        SCI_TRACE_LOW("SlidePageMenuInit Faild");
        return PNULL;
    }

    SCI_MEMSET(slide_data_ptr, 0, sizeof(MMIMAINMENU_SLIDEPAGE_DATA_T));

    slide_data_ptr->ctrl_handle = ctrl_handle;
    slide_data_ptr->win_handle  = win_handle = MMK_GetWinHandleByCtrl(ctrl_handle);

    SlidePageMenuInitSlideMenu(slide_data_ptr);

    return (void*)slide_data_ptr;
}
/*****************************************************************************/
//  Description : Slide Page Destory
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN         SlidePageDestory(
    void* pm_data_ptr
)
{
    if (PNULL == pm_data_ptr)
    {
        SCI_TRACE_LOW("SlidePageMenu Faild");
        return FALSE;
    }
    SlidePageMenuDestoryGOMenu((MMIMAINMENU_SLIDEPAGE_DATA_T *)pm_data_ptr);

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
LOCAL MMI_RESULT_E SlidePageMenuNotifyParentMsg(
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
        SCI_TRACE_LOW("MAINMENU_NotifyParentMsg):msg_id = %d ", msg_id);
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
//  Description : SlidePageMenuHandleOkKey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// LOCAL BOOLEAN SlidePageMenuHandleOkKey(
//     MMIMAINMENU_SLIDEPAGE_DATA_T*menu_ctrl_ptr,
//     MMI_MESSAGE_ID_E  msg_id
// )
// {
//     GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
//
//     if(GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
//     {
//         direction = GO_MOVE_UP;
//     }
//
//     HandleMenuDirectionKey(menu_ctrl_ptr,MSG_APP_DOWN,direction);
//
//     return MMI_RESULT_TRUE;
// }
/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleCancelKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id
)
{
    return SlidePageMenuNotifyParentMsg(menu_ctrl_ptr->ctrl_handle, MSG_APP_CANCEL);
}
/*****************************************************************************/
//  Description : handle menu direction key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleDirectionKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E  msg_id
)
{
    //BOOLEAN         result = FALSE;
    MMI_RESULT_E    handle_result = MMI_RESULT_TRUE;

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        handle_result = SlidePageMenuHandleMenuUpKey(menu_ctrl_ptr);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        handle_result = SlidePageMenuHandleMenuDownKey(menu_ctrl_ptr);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        handle_result = SlidePageMenuHandleMenuLeftKey(menu_ctrl_ptr,msg_id);
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        handle_result = SlidePageMenuHandleMenuRightKey(menu_ctrl_ptr,msg_id);
        break;

    default:
        SCI_TRACE_LOW("SlidePageMenuHandleDirectionKey Faild ,msg_id = %d", msg_id);
        break;
    }

    return handle_result;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: violent.wei
//  Note: 当有focus item变化时，朗读list内容
/*****************************************************************************/
LOCAL void HandleTextToSpeech(
                           MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr  //in
                           )
{
    if (MMITheme_IsControlTTSOpen())
    {
        // 当focus page或focus item有变化时，才进行朗读
        if (menu_ctrl_ptr->pre_msg_loop_page_index != menu_ctrl_ptr->cur_page_index ||
            menu_ctrl_ptr->pre_msg_loop_item_index != menu_ctrl_ptr->cur_item_index ||
            menu_ctrl_ptr->pre_msg_loop_menu_level != menu_ctrl_ptr->menu_level)
        {
            MMI_IMAGE_ID_T       img_id = 0;
            MMI_STRING_T         wstring = {0};
            CTRLMAINMENU_ITEM_T  menuitem_info = {0};

            SlidePageMenuGetItemPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index, &menuitem_info);

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
//  Description : check display init ok?
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _IsInitItemFinished(
								  MMIMAINMENU_SLIDEPAGE_DATA_T     *slidepage_ptr
								  )
{
	BOOLEAN result = FALSE;
	
	if (PNULL != slidepage_ptr && 0 == slidepage_ptr->init_item_timer_id)
	{
		result = TRUE;
	}
	
	return result;
}
/*****************************************************************************/
//  Description : Slide Page Handle
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL MMI_RESULT_E    SlidePageHandle(
    void *pm_data_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    GUI_POINT_T                     tp_point = {0};
    MMI_RESULT_E                    result = MMI_RESULT_TRUE;
    MMIMAINMENU_SLIDEPAGE_DATA_T *  menu_ctrl_ptr = (MMIMAINMENU_SLIDEPAGE_DATA_T *)pm_data_ptr;

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

	if (!_IsInitItemFinished(menu_ctrl_ptr) && MSG_TIMER != msg_id)
	{
		return MMI_RESULT_FALSE;
	}

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        menu_ctrl_ptr->menu_state |= GUIMENU_STATE_ENTER;
        // 初始值
        menu_ctrl_ptr->pre_msg_loop_page_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_item_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_menu_level = (uint16)-1;        
        break;

    case MSG_CTL_PAINT:
        SlidePageMenuDraw(menu_ctrl_ptr);
        HandleTextToSpeech(menu_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        menu_ctrl_ptr->is_active = TRUE;
		if (menu_ctrl_ptr->is_move_state)
		{
    		SlidePageMenuStartShakeIconTimer(menu_ctrl_ptr);
		}
        break;

    case MSG_CTL_LOSE_ACTIVE:
        MMITheme_StopControlText();
        menu_ctrl_ptr->is_active = FALSE;
        break;

    case MSG_CTL_GET_FOCUS:
        // 初始值
        menu_ctrl_ptr->pre_msg_loop_page_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_item_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_menu_level = (uint16)-1;    
#ifdef DYNAMIC_MAINMENU_SUPPORT
        if (MMIMENU_DyCheckIsSyn())//check信息有变化
        {
            SlidePageMenuGoSetInitialized(FALSE); // 设置初始化状态
        }
#endif
        SlidePageMenuCreatGoMemory(menu_ctrl_ptr);
        break;

    case MSG_CTL_LOSE_FOCUS:
        MMITheme_StopControlText();    
        SlidePageMenuGoReleaseMemory();
        //GUIMENU_StopAutoSwitchTimer(menu_ctrl_ptr);
        SlidePageMenuStopRedrawTimer(menu_ctrl_ptr);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        SlidePageMenuStopHighlightIconDispTimer(menu_ctrl_ptr);
#endif
        menu_ctrl_ptr->is_move_state = FALSE;
		menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        menu_ctrl_ptr->is_display_item_layer = FALSE;
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state)
        {
			if (!SlidePageMenuHandleNextPage(menu_ctrl_ptr,msg_id))
			{
				SlidePageMenuHandleOkKey(menu_ctrl_ptr,msg_id);
			}
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;

    case MSG_APP_RED:
		HandleTextToSpeech(menu_ctrl_ptr);
        result = MMI_RESULT_FALSE;
		break;
		
    case MSG_NOTIFY_CANCEL:
    case MSG_APP_CANCEL:
        if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state)
        {
            SlidePageMenuHandleCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);

        }
        break;

    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state)
        {
            MMITheme_StopControlText();
            result = SlidePageMenuHandleDirectionKey(menu_ctrl_ptr,msg_id);
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
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state)
        {
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);
        
        MMITheme_StopControlText();
        SlidePageMenuHandleSlidePageTpDown(&tp_point,menu_ctrl_ptr);
        HandleTextToSpeech(menu_ctrl_ptr);
        break;

    case MSG_TP_PRESS_UP:
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        if (SlidePageMenuHandleSlidePageTpUp(&tp_point,menu_ctrl_ptr))
        {
            SlidePageMenuHandleOkKey(menu_ctrl_ptr,MSG_TP_PRESS_UP);
        }
        HandleTextToSpeech(menu_ctrl_ptr);
        break;

    case MSG_TP_PRESS_MOVE:
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);
        SlidePageMenuHandleTpMove(&tp_point,menu_ctrl_ptr);
        break;

    case MSG_NOTIFY_PENOK:
        if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state)
        {
            SlidePageMenuHandleCancelKey(menu_ctrl_ptr, MSG_APP_CANCEL);
            HandleTextToSpeech(menu_ctrl_ptr);
        }
        break;
#endif

    case MSG_TIMER:
        SlidePageMenuHandleTimer(menu_ctrl_ptr, msg_id,	param);
        break;

    case MSG_CTRLMAINMENU_SET_MOVE_STATE:
        menu_ctrl_ptr->is_move_state = TRUE;
        SlidePageMenuMoveInit(menu_ctrl_ptr);
        break;

    case MSG_CTRLMAINMENU_GET_ID:
    {
        MMIMAINMENU_ITEM_INFO_T *item_info_ptr = (MMIMAINMENU_ITEM_INFO_T*)param;
        MMIMAINMENU_ITEM_INFO_T *cur_item_info_ptr = PNULL;
        MMIMAINEMENU_ITEM_LIST_T *  page_item_info_ptr = PNULL;

        //get item index and rect
        page_item_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

        if (PNULL != page_item_info_ptr)
        {
            cur_item_info_ptr = SlidePageMenuGetItemInfoByIndex(page_item_info_ptr, menu_ctrl_ptr->cur_item_index);
        }

        if (PNULL != item_info_ptr && PNULL != cur_item_info_ptr)
        {
            item_info_ptr->group_id = cur_item_info_ptr->group_id;
            item_info_ptr->menu_id = cur_item_info_ptr->menu_id;
        }

    }
    break;

    case MSG_CTRLMAINMENU_IS_MOVE_STATE:
        *((BOOLEAN*)param) = menu_ctrl_ptr->is_move_state;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SlideRegister(
    CTRLMAINMENU_PM_APIS_T *apis
)
{
    if (PNULL == apis)
    {
        return FALSE;
    }

    apis->Initialize = SlidePageInitialize;
    apis->Destory = SlidePageDestory;
    apis->Handle = SlidePageHandle;

    return TRUE;
}

/*****************************************************************************/
//  Description : get slide client rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T SlidePageMenuAdjustSlideMenuRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    return SlidePageMenuAdjustRect(menu_ctrl_ptr);
}

/*****************************************************************************/
//  Description : get slide client rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T SlidePageMenuAdjustRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    GUI_RECT_T menu_rect = {0};

    menu_rect           = MMITHEME_GetClientRectEx(menu_ctrl_ptr->win_handle);
    menu_rect.top      += menu_ctrl_ptr->slide_data.top_margin;
    menu_rect.bottom   -= menu_ctrl_ptr->slide_data.bottom_margin;
    menu_rect.left     += menu_ctrl_ptr->slide_data.left_margin;
    menu_rect.right    -= menu_ctrl_ptr->slide_data.right_margin;

    return menu_rect;
}

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SlideMenuModifyRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    SlidePageMenuDestoryGOMenu(menu_ctrl_ptr);
    SlidePageMenuInitSlideMenu(menu_ctrl_ptr);
    return;
}

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleNextPage(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
)
{
    BOOLEAN is_handled = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

#ifndef MMI_PDA_SUPPORT
    if (MSG_APP_OK == msg_id)
    {
        uint16      next_item_index  = 0;
        uint16      next_page_index  = 0;
        uint16      first_page_index = 0;

        menu_ctrl_ptr->cur_item_index = 0;
        first_page_index = menu_ctrl_ptr->cur_page_index;

        next_page_index = first_page_index == (menu_ctrl_ptr->item_total_num-1) ? 0 : first_page_index + 1;

        menu_ctrl_ptr->cur_page_index = next_page_index;

        if (GO_SLIDE_SUPPORT_ONLY_Y == menu_ctrl_ptr->support_slide_type)
        {
            SlidePageMenuAutoSlidePage(menu_ctrl_ptr, first_page_index,next_page_index, GO_MOVE_UP);
        }
        else
        {
            SlidePageMenuAutoSlidePage(menu_ctrl_ptr, first_page_index,next_page_index, GO_MOVE_LEFT);
        }

        SlidePageMenuDisplayActiveIcon( menu_ctrl_ptr,TRUE);

        SlidePageMenuSetFirstItemIndex(menu_ctrl_ptr, next_page_index, next_item_index);

        is_handled = TRUE;
    }
    else if (MSG_APP_WEB == msg_id)
    {
        is_handled = FALSE;
    }
#endif
    return is_handled;
}

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle icon touch panel press down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuTransmitTpMsg(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
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

/*****************************************************************************/
//  Description : handle button1 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleButton1Click( MMI_HANDLE_T button_handle)
{
    GUICTRL_PostNotify(button_handle,MSG_APP_OK);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleButton2Click( MMI_HANDLE_T button_handle)
{
    GUICTRL_PostNotify(button_handle,MSG_NOTIFY_CANCEL);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle button2 msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleButtonDraw(
    GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr
)
{
    if (PNULL != owner_draw_ptr)
    {
        MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByGuid(MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle), SPRD_GUI_MAINMENU_ID);
        MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr = (MMIMAINMENU_SLIDEPAGE_DATA_T *)CTRLMAINMENU_GetPM(ctrl_handle);

        if (PNULL != menu_ctrl_ptr)
        {
            GUI_LCD_DEV_INFO  lcd_dev_info = {0};
            GUI_RECT_T        display_rect = owner_draw_ptr->display_rect;
            BOOLEAN           is_left = 0;
            uint16            width  = 0;
            uint16            height = 0;

            GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
            if (0 == owner_draw_ptr->display_rect.left)
            {
                display_rect.right = width/2 - 1;
                is_left = TRUE;
            }
            else if ((width - 1) == owner_draw_ptr->display_rect.right)
            {
                display_rect.left = width/2;
                is_left = FALSE;
            }
            UILAYER_ClearRect(&lcd_dev_info, display_rect);
            SlidePageMenuDrawBg(display_rect, menu_ctrl_ptr, PNULL);
#ifndef MMI_PDA_SUPPORT
            SlidePageMenuDisplaysoftkeybg(menu_ctrl_ptr,&lcd_dev_info,is_left);
#endif
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
LOCAL void SlidePageMenuCreateIconMenuButton(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    uint16                      i = 0;
    uint16                      lcd_width = 0;
    GUI_RECT_T                  button_rect[GUIMENU_BUTTON_NUM] = {0};
    GUIBUTTON_INIT_DATA_T       init_data = {0};
    MMI_CONTROL_CREATE_T        ctrl_create = {0};
    IGUICTRL_T *                button_ptr = PNULL;
    MMI_TEXT_ID_T               button_text[GUIMENU_BUTTON_NUM] = {0};
    GUIBUTTON_CALLBACK_FUNC_EXT button_func[GUIMENU_BUTTON_NUM] = {PNULL};
    GUIBUTTON_OWNER_DRAW_FUNC   button_draw_func[GUIMENU_BUTTON_NUM] = {PNULL};
    GUI_FONT_ALL_T              font_info = {0};

    if (PNULL != menu_ctrl_ptr && menu_ctrl_ptr->main_menu_theme.is_has_button)
    {
        font_info.color = ~MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);
        font_info.font = MMITHEME_GetSoftKeyTextFont();

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, PNULL);
        button_rect[0] = MMITHEME_GetSoftkeyRect(LEFT_BUTTON);
        button_rect[0].right = lcd_width*2/5;
        button_rect[1] = MMITHEME_GetSoftkeyRect(RIGHT_BUTTON);
        button_rect[1].left = lcd_width*3/5;
        button_text[0] = TXT_MAINMENU_NEXTPAGE;
        button_text[1] = STXT_RETURN;
        button_func[0] = SlidePageMenuHandleButton1Click;
        button_func[1] = SlidePageMenuHandleButton2Click;
        button_draw_func[0] = SlidePageMenuHandleButtonDraw;
        button_draw_func[1] = SlidePageMenuHandleButtonDraw;

        for (i = 0; i < GUIMENU_BUTTON_NUM; i++)
        {
            if (0 != menu_ctrl_ptr->button_handle[i])
            {
                continue;
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

            GUIBUTTON_SetRunSheen(menu_ctrl_ptr->button_handle[i] , FALSE);

            if (0 == i)
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
            GUIAPICTRL_SetLcdDevInfo(menu_ctrl_ptr->button_handle[i], &menu_ctrl_ptr->layer_bg);
            GUIBUTTON_SetCallBackFuncExt(menu_ctrl_ptr->button_handle[i], button_func[i]);
            GUIBUTTON_SetOwnerDraw(menu_ctrl_ptr->button_handle[i], button_draw_func[i]);
            IGUICTRL_SetRect(button_ptr, &button_rect[i]);
        }
    }

    return;
}

#endif

/*****************************************************************************/
// Description : get item
// Global resource dependence :
//  Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetSlideMenuItemRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    int16         item_in_page_index
)
{
    GUI_RECT_T item_rect = {0};

    if (item_in_page_index >=0 )
    {
        item_rect = GUI_AddupRect(&menu_ctrl_ptr->slide_data.item_image_display_rect[item_in_page_index],
                                  &menu_ctrl_ptr->slide_data.item_text_display_rect[item_in_page_index]);
    }

    return item_rect;
}

#ifdef ATEST_SUPPORT
PUBLIC GUI_RECT_T Atest_GetSlideMenuItemRect(void *menu_ctrl_ptr, int16 item_in_page_index)
{
	return GetSlideMenuItemRect((MMIMAINMENU_SLIDEPAGE_DATA_T *)menu_ctrl_ptr ,item_in_page_index);
}
#endif
/*****************************************************************************/
//  Description : remove all menu layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuRemoveAllMenuLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info[6] = {0};
    uint32 i = 0;

    lcd_dev_info[0] = menu_ctrl_ptr->page_layer_handle[0];
    lcd_dev_info[1] = menu_ctrl_ptr->page_layer_handle[1];
    lcd_dev_info[2] = menu_ctrl_ptr->icon_move_layer_handle;
    lcd_dev_info[3] = menu_ctrl_ptr->highlight_layer_handle;
    lcd_dev_info[4] = menu_ctrl_ptr->titleidx_layer_handle;
    lcd_dev_info[5] = menu_ctrl_ptr->cube_layer_handle;

    for (i = 0; i < ARR_SIZE(lcd_dev_info); i++)
    {
        if (0 != lcd_dev_info[i].block_id)
        {
            UILAYER_RemoveBltLayer(&lcd_dev_info[i]);
        }
    }
}
/*****************************************************************************/
//  Description : 合并层，由于最多只有两个Page合并到主层上，因此只需要两个层的参数
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetBltSlideMenuPageLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    UILAYER_HANDLE_T layer1,
    UILAYER_HANDLE_T layer2,
    UILAYER_HANDLE_T layer3
)
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    SlidePageMenuRemoveAllMenuLayer(menu_ctrl_ptr);

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

    lcd_dev_info.block_id = layer3;
    if (0 != layer3 && UILAYER_IsLayerActive(&lcd_dev_info))
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
//  Description : item是否可以被删除
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsItemEnableDeleted(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMAINMENU_ITEM_INFO_T *  cur_item_info_ptr  = PNULL;
    MMIMAINEMENU_ITEM_LIST_T * page_item_info_ptr = PNULL;

    //get item index and rect
    page_item_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

    if (PNULL != page_item_info_ptr)
    {
        cur_item_info_ptr = SlidePageMenuGetItemInfoByIndex(page_item_info_ptr, menu_ctrl_ptr->cur_item_index);
    }

    if (PNULL != cur_item_info_ptr)
    {
        if ((DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == cur_item_info_ptr->group_id) && !MMIAPIUDISK_UdiskIsRun())
        {
            return TRUE;
        }
    }
#endif
    return FALSE;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateSlideMenuBgLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        uint16              layer_width  = 0;
        uint16              layer_height = 0;
        UILAYER_CREATE_T    create_info  = {0};
        GUI_LCD_DEV_INFO    lcd_dev_info = {0};

        GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
        GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

        menu_ctrl_ptr->layer_bg.lcd_id = 0;
        menu_ctrl_ptr->layer_bg.block_id = UILAYER_NULL_HANDLE;

        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = layer_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = TRUE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(
            &create_info,
            &menu_ctrl_ptr->layer_bg
        );
        // 设置color key
        UILAYER_SetLayerColorKey(&menu_ctrl_ptr->layer_bg, TRUE, UILAYER_TRANSPARENT_COLOR);

        // 使用color清除层
        UILAYER_Clear(&menu_ctrl_ptr->layer_bg);
    }
}

/*****************************************************************************/
//  Description : 算法输出BUF
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateCalcDstLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,  //in
    GUI_RECT_T       rect
)
{
    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUI_RECT_T menu_rect = {0};

        // 申请之前先释放
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->page_layer_handle[0]);/*lint !e506 !e774*/

        menu_rect = rect;
        menu_ctrl_ptr->page_layer_handle[0].lcd_id = 0;
        menu_ctrl_ptr->page_layer_handle[0].block_id = UILAYER_NULL_HANDLE;

        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = menu_rect.left;
        create_info.offset_y = menu_rect.top;
        create_info.width = menu_rect.right-menu_rect.left+1;
        create_info.height = menu_rect.bottom-menu_rect.top+1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;
        UILAYER_CreateLayer(
            &create_info,
            &menu_ctrl_ptr->page_layer_handle[0]
        );

        UILAYER_SetLayerPosition(&menu_ctrl_ptr->page_layer_handle[0], menu_rect.left, menu_rect.top);
        UILAYER_SetLayerColorKey(&menu_ctrl_ptr->page_layer_handle[0], TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleaseCalcDstLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        // 释放层
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->page_layer_handle[0]);/*lint !e506 !e774*/
    }
}
/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleasePageBgLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        // 释放层
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->layer_bg);/*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : 获取高亮区域
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T SlidePageMenuGoGetHighLightRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    int16 item_index_inpage
)
{
#ifdef MMI_PDA_SUPPORT
    {
        GUI_RECT_T item_rect = {0};

        item_rect = menu_ctrl_ptr->slide_data.item_rect[item_index_inpage];

        return item_rect;
    }
#else
    {
        GUI_RECT_T icon_rect = {0};

        icon_rect = menu_ctrl_ptr->slide_data.item_image_display_rect[item_index_inpage];

        icon_rect = GUI_GetCenterRectExt( icon_rect, menu_ctrl_ptr->main_menu_theme.focus_item_w, menu_ctrl_ptr->main_menu_theme.focus_item_h);

        return icon_rect;
    }
#endif
}

/*****************************************************************************/
//  Description : CreateHighlightLayer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateHighlightIconLayer(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr)
{
    GUI_RECT_T item_rect = {0};
    UILAYER_CREATE_T    create_info = {0};

    // 申请之前先释放
    SlidePageMenuReleaseHighlightIconLayer(menu_ctrl_ptr);

    menu_ctrl_ptr->highlight_layer_handle.lcd_id = 0;
    menu_ctrl_ptr->highlight_layer_handle.block_id = UILAYER_NULL_HANDLE;

    item_rect = SlidePageMenuGoGetHighLightRect(menu_ctrl_ptr, 0);

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = item_rect.right - item_rect.left + 1;
    create_info.height = item_rect.bottom - item_rect.top + 1;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    UILAYER_CreateLayer(
        &create_info,
        &(menu_ctrl_ptr->highlight_layer_handle)
    );
    UILAYER_SetLayerColorKey(&(menu_ctrl_ptr->highlight_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);

    UILAYER_Clear(&(menu_ctrl_ptr->highlight_layer_handle));
}

/*****************************************************************************/
//  Description : CreateHighlightLayer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplayHighlightIconLayer(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr)
{
#ifndef MMI_PDA_SUPPORT
    GUI_RECT_T           layer_rect    = {0, 0, MENUICON_FOCUSITEM_W - 1,MENUICON_FOCUSITEM_H - 1};
    CTRLMAINMENU_ITEM_T  mainitem_info = {0};

    SlidePageMenuGetItemInfo(menu_ctrl_ptr,menu_ctrl_ptr->cur_item_index,&mainitem_info);

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888
        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->highlight_layer_handle,FALSE,FALSE);
    }
#if !defined (PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    UILAYER_Clear(&(menu_ctrl_ptr->highlight_layer_handle));

    GUIRES_DisplayImg(PNULL,
                      &layer_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      mainitem_info.active_icon.data.icon_id,
                      (const GUI_LCD_DEV_INFO*)&menu_ctrl_ptr->highlight_layer_handle);
#endif
    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888
        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->highlight_layer_handle,TRUE,TRUE);
    }

#else
    register uint32   i = 0;
    uint32 *          OutBuf = PNULL;
    register uint32 * buf = PNULL;
    register uint32   num = 0;
    register uint32   color = 0x00ffff00;//黄色
    UILAYER_INFO_T    layer_info_ptr = {0};

    if (!UILAYER_IsLayerActive(&menu_ctrl_ptr->highlight_layer_handle))
    {
        return;
    }

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //PAF888 TO ARGB888
        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->highlight_layer_handle,FALSE,FALSE);
    }

    if (DATA_TYPE_ARGB888 != UILAYER_GetLayerColorType(&menu_ctrl_ptr->highlight_layer_handle))
    {
        return;
    }

    OutBuf = (uint32*)UILAYER_GetLayerBufferPtr(&menu_ctrl_ptr->highlight_layer_handle);

    if (PNULL == OutBuf)
    {
        return;
    }

    buf = OutBuf;

    UILAYER_GetLayerInfo(&menu_ctrl_ptr->highlight_layer_handle,&layer_info_ptr);
    num = layer_info_ptr.layer_width*layer_info_ptr.layer_height;

    UILAYER_Clear(&(menu_ctrl_ptr->highlight_layer_handle));

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888
        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->highlight_layer_handle,TRUE,TRUE);
    }

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcInputDataType())
    {
        color = ConvertRgb565ToArgb888(MMITHEME_GetCurThemeFontColor(MMI_THEME_FORM_BG), 0x00);//0x00ffff00;//黄色
    }
    else
    {
        color = ConvertRgb565ToArgb888(MMITHEME_GetCurThemeFontColor(MMI_THEME_FORM_BG), 0xFF);//0xffffff00;//黄色
    }

    while ( i < num )
    {
        *buf = color;//黄色
        i++;
        buf++;
    }
#endif
}


/*****************************************************************************/
//  Description : CreateHighlightLayerEx
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuCreateHighlightLayerByID(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr, MMI_IMAGE_ID_T image_id, GUI_LCD_DEV_INFO *layer_handle_ptr)
{
    UILAYER_CREATE_T    create_info = {0};
    GUI_POINT_T      display_point   = {0,0};

    UILAYER_RELEASELAYER(layer_handle_ptr);

    layer_handle_ptr->lcd_id = 0;
    layer_handle_ptr->block_id = UILAYER_NULL_HANDLE;
    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888
        UILAYER_SetPreMultiplyAlpha(layer_handle_ptr,FALSE,FALSE);
    }

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&
                             create_info.height, image_id, menu_ctrl_ptr->ctrl_handle);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    UILAYER_CreateLayer(
        &create_info,
        layer_handle_ptr
    );

    UILAYER_SetLayerColorKey(layer_handle_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(layer_handle_ptr);

    if (!UILAYER_IsLayerActive(layer_handle_ptr))
    {
        UILAYER_RELEASELAYER(layer_handle_ptr);
        return FALSE;
    }
    //把图片画到层上
    GUIRES_DisplayImg(&display_point,
                      PNULL,
                      PNULL,
                      menu_ctrl_ptr->ctrl_handle,
                      image_id,
                      (const GUI_LCD_DEV_INFO*)layer_handle_ptr);
    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888
        UILAYER_SetPreMultiplyAlpha(layer_handle_ptr,TRUE,TRUE);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleaseHighlightIconLayerByHander(
    GUI_LCD_DEV_INFO *layer_handle_ptr  //in
)
{
    // 释放层
    UILAYER_RELEASELAYER(layer_handle_ptr);/*lint !e506 !e774*/
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleaseHighlightIconLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    // 释放层
    UILAYER_RELEASELAYER(&menu_ctrl_ptr->highlight_layer_handle);/*lint !e506 !e774*/
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGOChangehandleandindex(SLIDE_MEM_PAGE_INFO_T* page_array_info,
        BOOLEAN is_tpmove_right)
{
    int i = 0;

    //is_tpmove_right 为true时，则tp向右滑动，为FALSE时，则tp是向左滑动
    if (is_tpmove_right)
    {
        for (i=0; i<GO_MEM_PAGE_NUM; i++)
        {
            if ((GO_MEM_PAGE_NUM-1) == page_array_info[i].index)
            {
                //杀掉该layer,重新创建并初始化
                page_array_info[i].is_change = TRUE;
                page_array_info[i].index = 0;
            }
            else
            {
                page_array_info[i].index++;
            }
        }

    }
    else
    {
        for (i=0; i<GO_MEM_PAGE_NUM; i++)
        {
            if (0 == page_array_info[i].index)
            {
                //杀掉该layer,重新创建并初始化
                page_array_info[i].is_change = TRUE;
                page_array_info[i].index = GO_MEM_PAGE_NUM-1;
            }
            else
            {
                page_array_info[i].index--;
            }
        }
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoUpdateItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    BOOLEAN is_tpmove_right
)
{
    int    i = 0;
    uint16 icon_width  = 0;
    uint16 icon_height = 0;
    uint16 page_index  = 0;

    SlidePageMenuGOChangehandleandindex(s_go_item_page_info,is_tpmove_right);

    icon_width = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;
    if (is_tpmove_right)
    {
        //新的PAGE INDEX
        if (0 == menu_ctrl_ptr->cur_page_index)
        {
            page_index = menu_ctrl_ptr->item_total_num-1;
        }
        else
        {
            page_index = menu_ctrl_ptr->cur_page_index-1;
        }
    }
    else
    {
        //新的PAGE INDEX
        if ((menu_ctrl_ptr->item_total_num-1) == menu_ctrl_ptr->cur_page_index)
        {
            page_index = 0;
        }
        else
        {
            page_index = menu_ctrl_ptr->cur_page_index+1;
        }
    }

    for (i=0; i<GO_MEM_PAGE_NUM; i++)
    {
        if (s_go_item_page_info[i].is_change)
        {
            SlidePageMenuGoInitItemLayerInfo(menu_ctrl_ptr,page_index,&(s_go_item_page_info[i].layer_handle));
            //重置标志量
            s_go_item_page_info[i].is_change = FALSE;

            break;
        }
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoCreateItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 icon_width,
    uint16 icon_height,
    uint16 item_total_num,
    GUI_LCD_DEV_INFO *layer_handle
)
{
    UILAYER_CREATE_T    create_info = {0};

    if ( PNULL == menu_ctrl_ptr
            || PNULL == layer_handle )
    {
        return;
    }
    //创建所有LAYER的层
    layer_handle->lcd_id = 0;
    layer_handle->block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = icon_width;
    create_info.height = item_total_num*icon_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    //create_info.format = UILAYER_MEM_IMMUTABLE;

    UILAYER_CreateLayer(&create_info, layer_handle);

    UILAYER_SetLayerColorKey(layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

    // 使用color清除层
    UILAYER_Clear(layer_handle);
    return;
}


/*****************************************************************************/
// Description : search in list
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SlidePageMenuSearchItemByIndex(
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr,
    uint16                   item_index
)
{
    BOOLEAN is_find = FALSE;
    uint32  i = 0;
    MMI_LINK_NODE_T *tmp_ptr = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr = PNULL;

    if (PNULL != page_item_info_ptr)
    {
        tmp_head_ptr = tmp_ptr = page_item_info_ptr->list_head_ptr;

        if (PNULL != tmp_ptr)
        {
            if (0 == item_index)
            {
                tmp_ptr = tmp_head_ptr;
                is_find = TRUE;
            }
            else
            {
                for (i = 0; i < page_item_info_ptr->item_count; i++)
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

#ifdef ATEST_SUPPORT
PUBLIC MMI_LINK_NODE_T *Atest_SlidePageMenuSearchItemByIndex(
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr,
    uint16                   item_index)
{
	return SlidePageMenuSearchItemByIndex(page_item_info_ptr,item_index);
}
#endif
/*****************************************************************************/
// Description : get item info in list
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T* SlidePageMenuGetItemInfoByIndex(
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr,
    uint16 item_index
)
{
    MMI_LINK_NODE_T *item_node_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;

    item_node_ptr = SlidePageMenuSearchItemByIndex(page_item_info_ptr, item_index);

    if (PNULL != item_node_ptr)
    {
        item_info_ptr = (MMIMAINMENU_ITEM_INFO_T *)(item_node_ptr->data);
    }

    return item_info_ptr;
}

#ifdef ATEST_SUPPORT
PUBLIC MMIMAINMENU_ITEM_INFO_T* Atest_SlidePageMenuGetItemInfoByIndex(
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr,
    uint16 item_index)
{
	return SlidePageMenuGetItemInfoByIndex(page_item_info_ptr,item_index);
}
#endif
/*****************************************************************************/
// Description : get item
// Global resource dependence :
//  Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGetItemPtr(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 page_index,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T * menuitem_ptr
)
{
    MMIMAINEMENU_ITEM_LIST_T *  page_item_info_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T *   item_info_ptr = PNULL;

    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/

    //get item index and rect
    page_item_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, page_index);

    if (PNULL != page_item_info_ptr)
    {
        item_info_ptr = SlidePageMenuGetItemInfoByIndex(page_item_info_ptr, item_index);

        if (PNULL != item_info_ptr)
        {
            MMIMAINMENU_GetItemData(item_info_ptr->menu_id,item_info_ptr->group_id,menuitem_ptr);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO SlidePageMenuDisplayOneItemToLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T         *menu_ctrl_ptr,
    const GUI_LCD_DEV_INFO *dst_dev_ptr,
    GUI_RECT_T             *rect_ptr,
    uint16                  page_index,
    uint16                  item_index
)
{
    GUI_LCD_DEV_INFO             item_layer    = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
    GUIMENU_DISPLAY_ITEM_PARAM_T param         = {0};
    CTRLMAINMENU_ITEM_T          menuitem_info = {0};

    SlidePageMenuGetItemPtr(menu_ctrl_ptr, page_index, item_index, &menuitem_info);

    if ( PNULL == menu_ctrl_ptr)
    {
        return item_layer;
    }

    if ( PNULL == dst_dev_ptr
            || PNULL == rect_ptr )
    {
        item_layer = MMITHEME_DrawMainmenuItem( menu_ctrl_ptr->win_handle, &menuitem_info, &param );
    }
    else
    {
        if ( DATA_TYPE_RGB565 == UILAYER_GetLayerColorType( dst_dev_ptr ) )
        {
            item_layer = MMITHEME_DrawMainmenuItem( menu_ctrl_ptr->win_handle, &menuitem_info, &param );

            UILAYER_SetLayerPosition( &item_layer, rect_ptr->left, rect_ptr->top );
            UILAYER_BltLayerToLayer( dst_dev_ptr, &item_layer, rect_ptr, TRUE );
        }
        else
        {
            param.format |= GUIMENU_DISPLAY_ITEM_FORMAT_TO_LAYER;
            param.dst_layer = *dst_dev_ptr;
            param.dst_rect  = *rect_ptr;

            item_layer = MMITHEME_DrawMainmenuItem( menu_ctrl_ptr->win_handle, &menuitem_info, &param);
        }
    }

    return item_layer;
}


/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDrawTitle(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    uint16             cur_page_index,
    GUI_LCD_DEV_INFO lcd_dev_info
)
{
    uint16          i = 0;
    uint16          image_width = 0;
    uint16          image_height = 0;
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      title_rect = {0};
    GUI_RECT_T      disp_rect = {0};
    MMI_IMAGE_ID_T  title_img_id = IMAGE_NULL;

    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/

    menu_ctrl_ptr->is_display_title = TRUE; // 显示title

    IMG_EnableTransparentColor(TRUE);

    UILAYER_SetLayerColorKey(&lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(&lcd_dev_info);

    MMITHEME_GetSlideMenuTitleIdxRect(&title_rect);
    //display title image
    GUIRES_DisplayImg(PNULL,
                      &title_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      menu_ctrl_ptr->slide_data.title_image_id,
                      &lcd_dev_info);

    for (i = 0; i < menu_ctrl_ptr->item_total_num; i++)
    {
        disp_rect = menu_ctrl_ptr->slide_data.title_display_rect[i];

        //get title image id
        if (i == cur_page_index)
        {
            title_img_id = menu_ctrl_ptr->slide_data.title_sel_bg_image[i];
        }
        else
        {
            title_img_id = menu_ctrl_ptr->slide_data.title_unsel_bg_image[i];
        }

        GUIRES_GetImgWidthHeight(&image_width, &image_height, title_img_id, menu_ctrl_ptr->win_handle);

        disp_rect = GUI_GetCenterRectExt(disp_rect, image_width, image_height);
        image_rect = GUI_CreateRect(0, 0, image_width - 1, image_height - 1);

        //display title image
        GUIRES_DisplayImg(PNULL,
                          &disp_rect,
                          &image_rect,
                          menu_ctrl_ptr->win_handle,
                          title_img_id,
                          &lcd_dev_info);
    }

    IMG_EnableTransparentColor(FALSE);

    return;
}


/*****************************************************************************/
//  Description : create highlight layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateSlideMenuTitleIdxLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_RECT_T          icon_rect = {0};
    UILAYER_CREATE_T    create_info = {0};

    if (MMITHEME_GetSlideMenuTitleIdxRect(&icon_rect))
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->titleidx_layer_handle);/*lint !e506 !e774*/
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = icon_rect.left;
        create_info.offset_y = icon_rect.top;
        create_info.width = (icon_rect.right + 1 - icon_rect.left);
        create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;

        UILAYER_CreateLayer(
            &create_info,
            &(menu_ctrl_ptr->titleidx_layer_handle)
        );

        UILAYER_SetLayerColorKey(&(menu_ctrl_ptr->titleidx_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&(menu_ctrl_ptr->titleidx_layer_handle));
    }
}

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleaseTitleIdxLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (PNULL != menu_ctrl_ptr)
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->titleidx_layer_handle);   /*lint !e506 !e774*/
    }
    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGoInitItemLayerInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 page_index,
    GUI_LCD_DEV_INFO *layer_handle
)
{
    uint16 i            = 0;
    uint16 page_num     = 0;
    uint16 item_num     = 0;
    uint16 icon_width   = 0;
    uint16 icon_height  = 0;
    uint32* next_ptr    = PNULL;
    GUI_RECT_T dis_rect = {0};
    uint16 page_max_item_num = 0;

    if (PNULL==menu_ctrl_ptr)
    {
        return FALSE;
    }

    // 置空
    page_max_item_num = SlidePageMenuGetInPageMaxItemNum(menu_ctrl_ptr);

    if (page_index >= MMITHEME_MENUMAIN_SLIDE_PAGE_NUM)
    {
        SCI_TRACE_LOW("SlidePageMenuGoInitItemLayerInfo page index = %d error!", page_index);
        return FALSE;
    }

    for (i = 0; i < page_max_item_num; i++)
    {
        s_slidemenu_item_layer[page_index][i].icon_layer_buf= PNULL;
        s_slidemenu_item_layer[page_index][i].width  = 0;
        s_slidemenu_item_layer[page_index][i].height = 0;
    }

    if ( PNULL == layer_handle
            || layer_handle->block_id == UILAYER_NULL_HANDLE)
    {
        SCI_TRACE_LOW("SlidePageMenuGoInitItemLayerInfo111111  %d.",page_index);
        return FALSE;
    }

    if (!UILAYER_IsLayerActive(layer_handle))
    {
        SCI_TRACE_LOW("SlidePageMenuGoInitItemLayerInfo22222 %d, %d.", layer_handle->block_id, page_index);

        return FALSE;
    }


    item_num = SlidePageMenuGetPageTotalItemNum(menu_ctrl_ptr, page_index);

    if (0 == item_num)
    {
        // 如果是零页则不用进行初始化
        return TRUE;
    }

    page_num = MIN(menu_ctrl_ptr->item_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);

    next_ptr = (uint32*)UILAYER_GetLayerBufferPtr(layer_handle);

    if (PNULL == next_ptr)
    {
        SCI_TRACE_LOW("SlidePageMenuGoInitItemLayerInfo33333 %d, %d.", layer_handle->block_id, page_index);
        return FALSE;
    }

    icon_width  = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;

    if (DATA_TYPE_PMARGB888== SEAPI_GetCalcInputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(layer_handle,FALSE,FALSE);
    }
    // 在层上画icon
    // 使用color清除层
    UILAYER_Clear(layer_handle);

    dis_rect.left = GO_CALC_OFFSET;
    dis_rect.top = GO_CALC_OFFSET;
    dis_rect.right = dis_rect.left + icon_width - 1;
    dis_rect.bottom = dis_rect.top + icon_height - 1;

    for (i = 0; i < item_num; i++)
    {
        SlidePageMenuDisplayOneItemToLayer( menu_ctrl_ptr, layer_handle, &dis_rect, page_index, i );

        dis_rect.top    += icon_height;
        dis_rect.bottom += icon_height;
    }

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcInputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(layer_handle,TRUE,TRUE);
    }

    for (i = 0; i < item_num; i++)
    {
        s_slidemenu_item_layer[page_index][i].icon_layer_buf = next_ptr;
        next_ptr += icon_width*icon_height*sizeof(uint8);
        s_slidemenu_item_layer[page_index][i].width  = icon_width;
        s_slidemenu_item_layer[page_index][i].height = icon_height;
    }
    // 把剩下的置空
    for (; i < page_max_item_num; i++)
    {
        s_slidemenu_item_layer[page_index][i].icon_layer_buf= PNULL;
        s_slidemenu_item_layer[page_index][i].width  = 0;
        s_slidemenu_item_layer[page_index][i].height = 0;
    }

    return TRUE;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    uint16 item_num     = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint16 i            = 0;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    for (i=0; i<GO_MEM_PAGE_NUM; i++)
    {
        UILAYER_RELEASELAYER(&s_go_item_page_info[i].layer_handle);/*lint !e506 !e774*/
        s_go_item_page_info[i].index = 0;
        s_go_item_page_info[i].is_change = FALSE;
    }

    icon_width = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;
    item_num = SlidePageMenuGetInPageMaxItemNum(menu_ctrl_ptr);

    //创建当前页
    SlidePageMenuGoCreateItemLayer(menu_ctrl_ptr, icon_width,icon_height,item_num,&(s_go_item_page_info[1].layer_handle));
    if (s_go_item_page_info[1].layer_handle.block_id == UILAYER_NULL_HANDLE)
    {
        return;
    }

    //创建上一页
    SlidePageMenuGoCreateItemLayer(menu_ctrl_ptr, icon_width,icon_height,item_num,&(s_go_item_page_info[0].layer_handle));
    if (s_go_item_page_info[0].layer_handle.block_id == UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_go_item_page_info[1].layer_handle);/*lint !e506 !e774*/
        return;
    }

    //创建下一页
    SlidePageMenuGoCreateItemLayer(menu_ctrl_ptr, icon_width,icon_height,item_num,&(s_go_item_page_info[2].layer_handle));

    if (s_go_item_page_info[2].layer_handle.block_id == UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_go_item_page_info[1].layer_handle);/*lint !e506 !e774*/
        UILAYER_RELEASELAYER(&s_go_item_page_info[0].layer_handle);/*lint !e506 !e774*/
        return;
    }

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuInitItemData(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    uint16 page_index   = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    BOOLEAN result = FALSE;
    uint16 i            = 0;

    if (PNULL == menu_ctrl_ptr || 0 == menu_ctrl_ptr->item_total_num)
    {
        return;
    }

    for (i=0; i<GO_MEM_PAGE_NUM; i++)
    {
        s_go_item_page_info[i].index = 0;
        s_go_item_page_info[i].is_change = FALSE;
    }

    icon_width = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;

    //画当前页
    s_go_item_page_info[1].index = 1;
    result = SlidePageMenuGoInitItemLayerInfo(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index,&(s_go_item_page_info[1].layer_handle));

    //画上一页
    s_go_item_page_info[0].index = 0;
    if (0 == menu_ctrl_ptr->cur_page_index)
    {
        page_index = menu_ctrl_ptr->item_total_num-1;
    }
    else
    {
        page_index = menu_ctrl_ptr->cur_page_index-1;
    }
    result = SlidePageMenuGoInitItemLayerInfo(menu_ctrl_ptr,page_index,&(s_go_item_page_info[0].layer_handle));

    //画下一页
    s_go_item_page_info[2].index = 2;
    if ((menu_ctrl_ptr->item_total_num-1) == menu_ctrl_ptr->cur_page_index)
    {
        page_index = 0;
    }
    else
    {
        page_index = menu_ctrl_ptr->cur_page_index+1;
    }


    result = SlidePageMenuGoInitItemLayerInfo(menu_ctrl_ptr,page_index,&(s_go_item_page_info[2].layer_handle));

    //数据已经准备完毕，避免内存不足
    MMITHEME_DestroyMenuItemCache();

    return;
}

/*****************************************************************************/
//  Description :设置初始化的状态
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoSetInitialized(
    BOOLEAN is_initialized
)
{
    s_go_is_initialized = is_initialized;
}

/*****************************************************************************/
//  Description :申请PAGE信息存放BUF
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuCreatGoMemory(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    uint8* next_ptr = PNULL;
    uint32 alloc_size = 0;
    uint32 calc_point_buf_size = 0;//算法所需BUF
    uint32 page_icon_buf_size = 0;//每页icon信息BUF
    uint32 page_info_buf_size = 0;//每页的结构体信息BUF
    uint32 calc_param_size = 0;
    uint32 shining_ctrl_buf_size = SHINING_MAX_RADIUS+1;
    uint32 total_row_num = 0;
    uint32 total_col_num = 0;

    total_row_num = SlidePageMenuGetRowNum(menu_ctrl_ptr);
    total_col_num = SlidePageMenuGetColNum(menu_ctrl_ptr);

    calc_point_buf_size = 1024*sizeof(FIX16_POINT_T);//1024为算法所需
    page_info_buf_size = sizeof(GO_MAINMENU_PAGE_INFO_T);

    page_icon_buf_size = total_row_num*total_col_num*sizeof(GO_MAINMENU_ICON_INFO_T);

    calc_param_size = sizeof(S3D_EXPANDPTR_T);

    alloc_size = calc_point_buf_size\
                 + GO_PAGE_READY_NUM*page_info_buf_size\
                 + GO_PAGE_READY_NUM*page_icon_buf_size\
                 + calc_param_size\
                 +shining_ctrl_buf_size;

    if (PNULL != s_go_alloc_mem_ptr)
    {
        GO_FREEIF(s_go_alloc_mem_ptr);
    }

    s_go_alloc_mem_ptr = SCI_ALLOCA(alloc_size);
    if (PNULL == s_go_alloc_mem_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(s_go_alloc_mem_ptr, 0,alloc_size);

    //初始临时变量
    next_ptr = s_go_alloc_mem_ptr;

    //开始分配算法所需BUF
    s_calc_point_buf = (FIX16_POINT_T*)next_ptr;
    next_ptr += calc_point_buf_size;
    //第一页
    s_cur_page_info_ptr = (GO_MAINMENU_PAGE_INFO_T *)next_ptr;
    next_ptr += page_info_buf_size;

    s_cur_page_info_ptr->icon_info = (GO_MAINMENU_ICON_INFO_T*)next_ptr;
    next_ptr += page_icon_buf_size;

    //第二页
    s_next_page_info_ptr = (GO_MAINMENU_PAGE_INFO_T *)next_ptr;
    next_ptr += page_info_buf_size;

    s_next_page_info_ptr->icon_info = (GO_MAINMENU_ICON_INFO_T*)next_ptr;
    next_ptr += page_icon_buf_size;

    s_go_calc_param_ptr = (S3D_EXPANDPTR_T*)next_ptr;
    next_ptr += calc_param_size;

    s_go_shining_ctrl_ptr = next_ptr;
    next_ptr += shining_ctrl_buf_size;

    //临时变量置PNULL
    next_ptr = PNULL;

    return TRUE;
}

/*****************************************************************************/
//  Description :申请PAGE信息存放BUF
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGoReleaseMemory(void)
{
    if (PNULL != s_go_alloc_mem_ptr)
    {
        s_calc_point_buf = PNULL;
        s_cur_page_info_ptr = PNULL;
        s_next_page_info_ptr = PNULL;
        s_go_calc_param_ptr = PNULL;
        s_go_shining_ctrl_ptr = PNULL;

        GO_FREEIF(s_go_alloc_mem_ptr);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :获取GO桌面需要的页面信息
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetRowNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    uint16 number = 0;

    if (PNULL != menu_ctrl_ptr)
    {
        number = menu_ctrl_ptr->slide_data.page_row_num;
    }

    return number;
}

/*****************************************************************************/
//  Description :获取GO桌面需要的页面信息
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetColNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    uint16 number = 0;

    if (PNULL != menu_ctrl_ptr)
    {
        number = menu_ctrl_ptr->slide_data.page_col_num;
    }

    return number;
}

/*****************************************************************************/
//  Description :获取GO桌面需要的页面信息
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoGetPageInfo(MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
                                  uint16 page_index,
                                  GO_MAINMENU_PAGE_INFO_T *page_info_ptr)
{
    uint16       i                    = 0;
    GUI_RECT_T   buf_rect             = {0};
    GUI_RECT_T menu_rect = {0};

    if ((PNULL == menu_ctrl_ptr)||(PNULL == page_info_ptr)||(PNULL == page_info_ptr->icon_info))
    {
        return;
    }

    if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //AM888 ARGB888申请层的大小没有状态栏，算法获取的offset认为是相对于层的。
        menu_rect = SlidePageMenuAdjustRect(menu_ctrl_ptr);
    }
    else
    {
        //RGB565直接画到背景层，不需要调整OFFSET
        menu_rect.left = 0;
        menu_rect.top = 0;
    }
    page_info_ptr->total_row_num = SlidePageMenuGetRowNum(menu_ctrl_ptr);
    page_info_ptr->total_col_num = SlidePageMenuGetColNum(menu_ctrl_ptr);
    page_info_ptr->icon_num = SlidePageMenuGetPageTotalItemNum(menu_ctrl_ptr, page_index);

    SCI_MEMSET(page_info_ptr->icon_info, 0,page_info_ptr->total_row_num *page_info_ptr->total_col_num*sizeof(GO_MAINMENU_ICON_INFO_T));

    for (i=0; i<page_info_ptr->icon_num; i++)
    {
        buf_rect = GetSlideMenuItemRect(menu_ctrl_ptr, i);

        page_info_ptr->icon_info[i].icon_buf = s_slidemenu_item_layer[page_index][i].icon_layer_buf;
        page_info_ptr->icon_info[i].icon_size.w = s_slidemenu_item_layer[page_index][i].width;
        page_info_ptr->icon_info[i].icon_size.h = s_slidemenu_item_layer[page_index][i].height;
        //因为计算的原因，图标的上下左右都要留一定的像素，所以LAYER的启示点要调整
        //创建的DST layer有偏移，底层算法是相对于layer的，所以要去掉TITLE
        page_info_ptr->icon_info[i].icon_offset.x = buf_rect.left-GO_CALC_OFFSET-menu_rect.left;
        page_info_ptr->icon_info[i].icon_offset.y = buf_rect.top-GO_CALC_OFFSET-menu_rect.top;

        page_info_ptr->icon_info[i].item_row_num = i/page_info_ptr->total_col_num;//第几行
        page_info_ptr->icon_info[i].item_col_num = i%page_info_ptr->total_col_num;//第几列
    }
}

/*****************************************************************************/
//  Description :PAF 边界处理函数
//  Global resource dependence :
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGoEffectZeroTypeCalc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
        IMGREF_SIZE_T bg_window_size,//屏幕尺寸
        uint32* bg_window_buf,//背景buffer
        MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
        uint32 *select_image_buf,
        IMGREF_SIZE_T highlight_icon_size,
        BOOLEAN is_display_highlight,
        uint16 page_index,
        GO_CALC_DATA_INFO_T* calc_data_info
                                              )
{
    int i=0,row=0,column=0;
    FIX16_POINT_T window_offset={0};
    uint16 icon_num = 0;

    row=cur_page_info->total_row_num;
    column=cur_page_info->total_col_num;

#ifndef GO_STATIC_SELECT_USE_GREY
    //画高亮框
    if (is_display_highlight)
    {
        GUI_RECT_T menu_rect = {0};
        GUI_RECT_T item_rect = {0};
        FIX16_POINT_T h_window_offset={0};

        item_rect = SlidePageMenuGoGetHighLightRect(menu_ctrl_ptr, SlidePageMenuGetPageIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index));
        menu_rect = SlidePageMenuAdjustRect(menu_ctrl_ptr);

        h_window_offset.x = item_rect.left-menu_rect.left;
        h_window_offset.y = item_rect.top-menu_rect.top;

        //把高亮框先贴上
        S2D_TypeBlending(bg_window_size,bg_window_buf,calc_data_info->output_type,highlight_icon_size,h_window_offset,select_image_buf,calc_data_info->input_type, bg_window_buf,PNULL);
    }
#endif

    icon_num = SlidePageMenuGetPageTotalItemNum(menu_ctrl_ptr, page_index);

    for (i=0; i<icon_num; i++)
    {
        if (NULL!=(*(cur_page_info->icon_info+i)).icon_buf)
        {
            //贴图标
            window_offset.x=(cur_page_info->icon_info+i)->icon_offset.x;
            window_offset.y=(cur_page_info->icon_info+i)->icon_offset.y;
            //画高亮框
#ifdef GO_STATIC_SELECT_USE_GREY
            if (is_display_highlight&&(i==menu_ctrl_ptr->cur_item_index))
            {
                S2D_TypeBlending_BlockTpcy(bg_window_size, bg_window_buf,calc_data_info->output_type,(cur_page_info->icon_info+i)->icon_size,window_offset,(cur_page_info->icon_info +i)->icon_buf,128,calc_data_info->input_type, bg_window_buf,PNULL);
            }
            else
#endif
            {
                S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,(cur_page_info->icon_info+i)->icon_size,window_offset,(cur_page_info->icon_info +i)->icon_buf,calc_data_info->input_type, bg_window_buf,PNULL);
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :绘制并刷新函数
//  Global resource dependence :
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDrawUpdateGoMenuLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,  //in
    uint16 first_page,
    uint16 second_page,
    GO_MOVE_DIRECTION_E direction,
    uint16 current_frame,
    BOOLEAN is_display_highlight
)
{
    if (s_go_item_page_info[0].layer_handle.block_id == UILAYER_NULL_HANDLE
            ||s_go_item_page_info[1].layer_handle.block_id == UILAYER_NULL_HANDLE
            ||s_go_item_page_info[2].layer_handle.block_id == UILAYER_NULL_HANDLE
       )
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_2316_112_2_18_2_35_34_213,(uint8*)"");
        return;
    }

    SlidePageMenuDrawGOMenuLayer(menu_ctrl_ptr,first_page,second_page,direction,current_frame,is_display_highlight);

    SlidePageMenuStoreUpdateRect(menu_ctrl_ptr);

}
/*****************************************************************************/
//  Description :绘制函数
//  Global resource dependence :
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDrawGOMenuLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,  //in
    uint16 first_page,
    uint16 second_page,
    GO_MOVE_DIRECTION_E direction,
    uint16 current_frame,
    BOOLEAN is_display_highlight
)
{
    IMGREF_SIZE_T bg_window_size       = {0};
    uint16*       wallpaper_window_buf = PNULL;
    uint16        text_hight           = 0;
    uint8 *       layer_buf_ptr        = {0};
    IMGREF_SIZE_T highlight_icon_size  = {0};
    GO_MENU_STYLE_E cur_style = MMIAPISET_GetGOmenuStyle();
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 cost_time = 0;
    void* bg_window_buf = PNULL;
    go_calc_func calc_func = PNULL;
    GO_CALC_DATA_INFO_T calc_data_info = {0};
    static uint16 pre_frame = 0;
    UILAYER_INFO_T layer_info_ptr = {0};
    UILAYER_INFO_T dst_layer_info_ptr = {0};

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    if ( PNULL == s_cur_page_info_ptr
            || PNULL == s_next_page_info_ptr
            || PNULL == s_calc_point_buf
       )
    {
        return;
    }
    //如果祯数一样则不进行重画
    if ((pre_frame == current_frame)
            &&(current_frame!=0)
            &&(current_frame!=GO_MAINMENU_TOTAL_FRAME_MAX)
       )
    {
        return;
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_2373_112_2_18_2_35_34_214,(uint8*)"d", SCI_GetTickCount());

    SlidePageMenuGoGetPageInfo(menu_ctrl_ptr,first_page,s_cur_page_info_ptr);
	if (PNULL == s_cur_page_info_ptr->icon_info->icon_buf)
	{
        return;
	}
    SlidePageMenuGoGetPageInfo(menu_ctrl_ptr,second_page,s_next_page_info_ptr);

    if (DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        calc_data_info.input_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcInputDataType());
        calc_data_info.output_type = SEAPI_DataTypeAdapt(DATA_TYPE_RGB565);
        calc_data_info.param = s_go_calc_param_ptr;
        calc_data_info.param->shiningCtrl_ptr = s_go_shining_ctrl_ptr;

        if ((0 == current_frame)||(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame))
        {
            //UILAYER_SwitchDoubleMem( &menu_ctrl_ptr->page_layer_handle[0], FALSE );
        }
        else
        {
            UILAYER_SetDirectDraw( TRUE );
        }

        UILAYER_GetLayerInfo(&lcd_dev_info,&dst_layer_info_ptr);
        bg_window_size.w = dst_layer_info_ptr.layer_width;
        bg_window_size.h = dst_layer_info_ptr.layer_height;
        //lcd屏幕BUF
        bg_window_buf = (void*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
        if (PNULL == bg_window_buf)
        {
            return;
        }

        //背景图BUF
        wallpaper_window_buf = (uint16*)UILAYER_GetLayerBufferPtr(&menu_ctrl_ptr->layer_bg);
        if (PNULL == wallpaper_window_buf)
        {
            return;
        }

        SCI_MEMCPY(bg_window_buf, wallpaper_window_buf, bg_window_size.w*bg_window_size.h*sizeof(COLOR16));
    }
    else if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0],TRUE,FALSE);

        calc_data_info.input_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcInputDataType());
        calc_data_info.output_type = SEAPI_DataTypeAdapt(DATA_TYPE_PMARGB888);
        calc_data_info.param = s_go_calc_param_ptr;
        calc_data_info.param->shiningCtrl_ptr = s_go_shining_ctrl_ptr;
        if ((0 == current_frame)||(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame))
        {
            UILAYER_SwitchDoubleMem( &menu_ctrl_ptr->page_layer_handle[0], FALSE );
        }
        else
        {
            UILAYER_SetDirectDraw( TRUE );
            UILAYER_SetMainLayerImmutable( TRUE );
            //设置双buffer切换，不使用copy
            UILAYER_SwitchDoubleMem( &menu_ctrl_ptr->page_layer_handle[0], TRUE );
        }

        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);

        UILAYER_GetLayerInfo(&menu_ctrl_ptr->page_layer_handle[0],&dst_layer_info_ptr);
        bg_window_size.w = dst_layer_info_ptr.layer_width;
        bg_window_size.h = dst_layer_info_ptr.layer_height;

        //算法输出BUF
        if ( PNULL == ( bg_window_buf = (uint32*)UILAYER_GetLayerBufferPtr( &menu_ctrl_ptr->page_layer_handle[0] ) ) )
        {
            return;
        }
    }
    cost_time = SCI_GetTickCount();
    if ((0 == current_frame)
            &&(MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state)
       )
    {
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(&menu_ctrl_ptr->highlight_layer_handle);
        if (PNULL == layer_buf_ptr)
        {
			SCI_TRACE_LOW("NANJI layer_buf_ptr is NULL");
            return;
        }

        UILAYER_GetLayerInfo(&menu_ctrl_ptr->highlight_layer_handle,&layer_info_ptr);
        highlight_icon_size.w = layer_info_ptr.layer_width;
        highlight_icon_size.h = layer_info_ptr.layer_height;

		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr  is 0x%x",s_cur_page_info_ptr);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->icon_info->icon_buf      is 0x%x",s_cur_page_info_ptr->icon_info->icon_buf);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->icon_info->icon_offset   is %d",s_cur_page_info_ptr->icon_info->icon_offset);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->icon_info->icon_size     is %d",s_cur_page_info_ptr->icon_info->icon_size);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->icon_info->item_col_num  is %d",s_cur_page_info_ptr->icon_info->item_col_num);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->icon_info->item_row_num  is %d",s_cur_page_info_ptr->icon_info->item_row_num);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->icon_num       is %d",s_cur_page_info_ptr->icon_num);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->total_col_num  is %d",s_cur_page_info_ptr->total_col_num);
		SCI_TRACE_LOW("NANJI s_cur_page_info_ptr->total_row_num  is %d",s_cur_page_info_ptr->total_row_num);

		SCI_TRACE_LOW("NANJI bg_window_size.w     is %d,bg_window_size.h     is %d",bg_window_size.w, bg_window_size.h);
		SCI_TRACE_LOW("NANJI bg_window_buf        is 0x%x",bg_window_buf);
		SCI_TRACE_LOW("NANJI menu_ctrl_ptr        is 0x%x",menu_ctrl_ptr);
		SCI_TRACE_LOW("NANJI layer_buf_ptr        is 0x%x",layer_buf_ptr);
		SCI_TRACE_LOW("NANJI highlight_icon_size.w  is %d,highlight_icon_size.h is %d",highlight_icon_size.w, highlight_icon_size.h);
		SCI_TRACE_LOW("NANJI is_display_highlight is %d",is_display_highlight);
		SCI_TRACE_LOW("NANJI first_page           is %d",first_page);
		SCI_TRACE_LOW("NANJI calc_data_info.input_type is %d,calc_data_info.output_type is %d,calc_data_info.param is 0x%x",calc_data_info.input_type,calc_data_info.output_type,calc_data_info.param);

        SlidePageMenuGoEffectZeroTypeCalc(
            s_cur_page_info_ptr,//当前页信息
            bg_window_size,//屏幕尺寸
            bg_window_buf, //背景buffer
            menu_ctrl_ptr,
            (uint32*)layer_buf_ptr,
            highlight_icon_size,
            is_display_highlight,
            first_page,
            &calc_data_info
        );
    }
    else if (GO_MAINMENU_TOTAL_FRAME_MAX == current_frame
             &&(MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state))
    {
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(&menu_ctrl_ptr->highlight_layer_handle);
        if (PNULL == layer_buf_ptr)
        {
            return;
        }

        UILAYER_GetLayerInfo(&menu_ctrl_ptr->highlight_layer_handle,&layer_info_ptr);
        highlight_icon_size.w = layer_info_ptr.layer_width;
        highlight_icon_size.h = layer_info_ptr.layer_height;

        SlidePageMenuGoEffectZeroTypeCalc(
            s_next_page_info_ptr,//当前页信息
            bg_window_size,//屏幕尺寸
            bg_window_buf, //背景buffer
            menu_ctrl_ptr,
            (uint32*)layer_buf_ptr,
            highlight_icon_size,
            is_display_highlight,
            second_page,
            &calc_data_info
        );
    }
    else
    {
        text_hight = (menu_ctrl_ptr->slide_data.item_text_display_rect[0].bottom-menu_ctrl_ptr->slide_data.item_text_display_rect[0].top)+1;

        if (GO_MENU_STYLE_RANDOM == cur_style)
        {
            calc_func = UAPIMenu_GetCallBackfunc(s_random_cur_style);
        }
        else
        {
            calc_func = UAPIMenu_GetCallBackfunc(cur_style);
        }

        if (PNULL == calc_func)
        {
            return;
        }

#ifdef GO_CALC_ANIM_NO_STRING
        s_cur_page_info_ptr->icon_size.h -= (MENUTEXT_HEIGHT+GO_CALC_OFFSET);
        s_next_page_info_ptr->icon_size.h -= (MENUTEXT_HEIGHT+GO_CALC_OFFSET);
#endif
        calc_func(
            s_cur_page_info_ptr,//当前页信息
            s_next_page_info_ptr,//下一页信息
            PNULL,//可选项
            current_frame,//当前帧
            GO_MAINMENU_TOTAL_FRAME_MAX,//总帧数   暂定20
            bg_window_size,//屏幕尺寸
            bg_window_buf,//背景buffer
            direction,//移动方向
            s_calc_point_buf, //申请计算所需的BUFFER
            &calc_data_info
        );
    }

    cost_time = SCI_GetTickCount() - cost_time;
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_2534_112_2_18_2_35_34_215,(uint8*)"ddd", cost_time,cur_style,current_frame);
    pre_frame = current_frame;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideSecondPage_X(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T*    point_ptr,
    BOOLEAN        *is_second_move
)
{
    uint16  second_page = 0;

    if (0 == menu_ctrl_ptr->cur_page_index) // 当前页在第一页
    {
        if (point_ptr->x > menu_ctrl_ptr->slide_start_point.x) // 向右移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->item_total_num - 1;
            }
        }
        else if (point_ptr->x < menu_ctrl_ptr->slide_start_point.x) // 向左移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index + 1;
            }
        }
    }
    else if (menu_ctrl_ptr->item_total_num - 1 == menu_ctrl_ptr->cur_page_index) // 当前页在最后一页
    {
        if (point_ptr->x > menu_ctrl_ptr->slide_start_point.x) // 向右移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->x < menu_ctrl_ptr->slide_start_point.x) // 向左移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = 0;
            }
        }
    }
    else
    {
        if (point_ptr->x > menu_ctrl_ptr->slide_start_point.x) // 向右移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->x < menu_ctrl_ptr->slide_start_point.x) // 向左移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index + 1;
            }
        }
    }

    return second_page;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideSecondPage_Y(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T*    point_ptr,
    BOOLEAN        *is_second_move
)
{
    uint16  second_page = 0;

    if (0 == menu_ctrl_ptr->cur_page_index) // 当前页在第一页
    {
        if (point_ptr->y > menu_ctrl_ptr->slide_start_point.y) // 向下移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->item_total_num - 1;
            }
        }
        else if (point_ptr->y < menu_ctrl_ptr->slide_start_point.y) // 向上移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index + 1;
            }
        }
    }
    else if (menu_ctrl_ptr->item_total_num - 1 == menu_ctrl_ptr->cur_page_index) // 当前页在最后一页
    {
        if (point_ptr->y > menu_ctrl_ptr->slide_start_point.y) // 向下移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->y < menu_ctrl_ptr->slide_start_point.y) // 向上移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = 0;
            }
        }
    }
    else
    {
        if (point_ptr->y > menu_ctrl_ptr->slide_start_point.y) // 向下移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->y < menu_ctrl_ptr->slide_start_point.y) // 向下移动
        {
            if (menu_ctrl_ptr->item_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = menu_ctrl_ptr->cur_page_index + 1;
            }
        }
    }

    return second_page;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuMoveGoMenuPageLayerX(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T*    point_ptr
)

{
    uint16              first_page     = 0;
    uint16              second_page     = 0;
    BOOLEAN             is_second_move  = FALSE;
    uint16              current_frame = 0;
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
    BOOLEAN is_disp_active = FALSE;

    SCI_ASSERT(PNULL != point_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/

    first_page    = menu_ctrl_ptr->cur_page_index;

    if (point_ptr->x > menu_ctrl_ptr->slide_start_point.x) // 向右移动
    {
        direction = GO_MOVE_RIGHT;
    }
    else
    {
        direction = GO_MOVE_LEFT;
    }

    current_frame = (abs(point_ptr->x - menu_ctrl_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);

    if (current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }

    second_page = GetSlideSecondPage_X(menu_ctrl_ptr, point_ptr, &is_second_move);

    if ((!menu_ctrl_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }

    SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,second_page,direction,current_frame,is_disp_active);

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuMoveGoMenuPageLayerY(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    GUI_POINT_T*    point_ptr
)

{
    uint16              first_page     = 0;
    uint16              second_page     = 0;
    BOOLEAN             is_second_move  = FALSE;
    uint16              current_frame = 0;
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
    BOOLEAN is_disp_active = FALSE;

    SCI_ASSERT(PNULL != point_ptr);/*assert verified*/
    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/

    first_page    = menu_ctrl_ptr->cur_page_index;

    if (point_ptr->y > menu_ctrl_ptr->slide_start_point.y) // 向下移动
    {
        direction = GO_MOVE_DOWN;
    }
    else
    {
        direction = GO_MOVE_UP;
    }

    current_frame = (abs(point_ptr->y - menu_ctrl_ptr->slide_start_point.y)/GO_ONE_FRAME_PIX);

    if (current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }
    second_page = GetSlideSecondPage_Y(menu_ctrl_ptr, point_ptr, &is_second_move);

    if ((!menu_ctrl_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }

    SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,second_page,direction,current_frame,is_disp_active);

    return;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SlidePageMenuValidateCurPage(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (0 == menu_ctrl_ptr->item_total_num)
    {
        menu_ctrl_ptr->cur_page_index = 0;
    }
    else if (menu_ctrl_ptr->cur_page_index >= menu_ctrl_ptr->item_total_num)
    {
        menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->item_total_num - 1;
    }

    return;
}

/*****************************************************************************/
//  Description : 当前的Item是否在页中显示,因为当触笔移动滚动条的时候,
//                  当前Item可能会不在页面中显示
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsItemInPage(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           item_index,
    uint16           page_index
)
{
    uint16              page_item_num   = 0;
    BOOLEAN             result          = FALSE;
    MMIMAINEMENU_ITEM_LIST_T  *menu_list_ptr  = PNULL;

    menu_list_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, page_index);

    if (PNULL != menu_ctrl_ptr && PNULL != menu_list_ptr)
    {
        {
            page_item_num = menu_ctrl_ptr->slide_data.page_col_num*menu_ctrl_ptr->slide_data.page_row_num;
        }

        if (page_item_num > (item_index))
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 根据Item索引号获得其在页面中的位置索引
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 如果不在页内,返回-1
/*****************************************************************************/
LOCAL int16 SlidePageMenuGetPageIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           item_index
)
{
    int16 page_index = -1;
    MMIMAINEMENU_ITEM_LIST_T  *menu_list_ptr  = PNULL;

    menu_list_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

    if (PNULL != menu_ctrl_ptr
            && PNULL != menu_list_ptr
            && SlidePageMenuIsItemInPage(menu_ctrl_ptr, item_index, menu_ctrl_ptr->cur_page_index))
    {
        page_index = item_index - 0;
    }

    return page_index;
}

/*****************************************************************************/
//  Description : 当前的Item是否在页中显示,因为当触笔移动滚动条的时候,
//                  当前Item可能会不在页面中显示
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 根据当前页中的索引号计算实际的item索引号,否则返回-1
/*****************************************************************************/
LOCAL int16 SlidePageMenuGetItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           page_item_index
)
{
    int16 item_index = -1;
    uint16 page_item_num = 0;
    MMIMAINEMENU_ITEM_LIST_T  *menu_list_ptr  = PNULL;

    menu_list_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

    if (PNULL != menu_ctrl_ptr && PNULL != menu_list_ptr)
    {
        page_item_num = menu_ctrl_ptr->slide_data.page_col_num*menu_ctrl_ptr->slide_data.page_row_num;
#if 1
        if (page_item_index < page_item_num)
        {
            item_index = 0 + page_item_index;
        }
#else
        item_index = menu_ctrl_ptr->cur_page_index * page_item_num + page_item_index;
#endif
    }

    return item_index;
}

/*****************************************************************************/
//  Description : get line index of item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetItemLineIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint16 item_index
)
{
    uint16 line_index = 0;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    if (0 != menu_ctrl_ptr->slide_data.page_col_num)
    {
        line_index = item_index/menu_ctrl_ptr->slide_data.page_col_num;
    }

    return (line_index);
}

/*****************************************************************************/
//  Description : 获取一页显示的item number
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetPageItemNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           item_total_num
)
{
    uint16 page_item_num = 0;

    if (PNULL != menu_ctrl_ptr)
    {
        {
            page_item_num = menu_ctrl_ptr->slide_data.page_col_num*menu_ctrl_ptr->slide_data.page_row_num;
        }

        page_item_num = MIN(page_item_num, item_total_num);
    }

    return page_item_num;
}

/*****************************************************************************/
//  Description : 获取一页显示的最大item number
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetInPageMaxItemNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    uint16 page_item_num = 0;

    if (PNULL != menu_ctrl_ptr)
    {
        page_item_num = menu_ctrl_ptr->slide_data.page_col_num*menu_ctrl_ptr->slide_data.page_row_num;
    }

    return page_item_num;
}


/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetTotalLineNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint32          page_index
)
{
    uint16      total_line_num = 0;
    MMIMAINEMENU_ITEM_LIST_T *menu_list_ptr = PNULL;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    menu_list_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, page_index);

    if (PNULL != menu_list_ptr && 0 != menu_ctrl_ptr->slide_data.page_col_num)
    {
        total_line_num = menu_list_ptr->item_count/menu_ctrl_ptr->slide_data.page_col_num;

        if (0 != menu_list_ptr->item_count%menu_ctrl_ptr->slide_data.page_col_num)
        {
            total_line_num++;
        }
    }

    return (total_line_num);
}

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetItemNumInLastLine(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint32          page_index
)
{
    uint16      item_num = 0;
    MMIMAINEMENU_ITEM_LIST_T *menu_list_ptr = PNULL;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    menu_list_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, page_index);

    if (PNULL != menu_list_ptr && 0 != menu_ctrl_ptr->slide_data.page_col_num)
    {
        item_num = menu_list_ptr->item_count%menu_ctrl_ptr->slide_data.page_col_num;

        if (0 != menu_list_ptr->item_count && 0 == item_num)
        {
            item_num = menu_ctrl_ptr->slide_data.page_col_num;
        }
    }

    return (item_num);
}

/*****************************************************************************/
//  Description : 获得一页的item number
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 SlidePageMenuGetPageTotalItemNum(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint32          page_index
)
{
    uint16      page_item_num = 0;
    MMIMAINEMENU_ITEM_LIST_T *menu_list_ptr = PNULL;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    menu_list_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, page_index);

    if (PNULL != menu_list_ptr)
    {
        page_item_num = menu_list_ptr->item_count;
    }

    return (page_item_num);
}

/*****************************************************************************/
//  Description : 根据一个item的索引号，获得该行的第一个item索引号
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 如果索引号大于总数，则返回无效的数值-1
/*****************************************************************************/
LOCAL int16 SlidePageMenuGetLineStartItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           line_index,
    uint16           page_index
)
{
    int16 item_index = -1;

    if (PNULL != menu_ctrl_ptr && line_index < SlidePageMenuGetTotalLineNum(menu_ctrl_ptr, page_index))
    {
        item_index = line_index*menu_ctrl_ptr->slide_data.page_col_num;
    }

    return item_index;
}

/*****************************************************************************/
//  Description : get next item and page index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGetNextItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    uint16             page_index,
    uint16             item_index,
    MMI_MESSAGE_ID_E   msg_id,
    uint16             *next_item_index_ptr,
    uint16             *next_page_index_ptr
)
{
    BOOLEAN result = TRUE;
    uint16  next_item_index = 0;
    uint16  next_page_index = 0;
    uint16  cur_item_index  = 0;
    uint16  cur_line_index  = 0;
    uint16  cur_col_index   = 0;
    uint16  page_item_total_num  = 0;
    uint16  total_page_num  = 0;
    uint16  total_item_num  = 0;
    uint32  icon_col_num    = 0;
    uint32  icon_row_num    = 0;
    uint16  total_line_num  = 0;
    uint16  last_line_item_num  = 0;
    int16   last_line_start_index = 0;
    MMIMAINEMENU_ITEM_LIST_T *menu_list_ptr = PNULL;
    uint16  next_page_item_total_num = 0;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    if (PNULL == next_item_index_ptr && PNULL == next_page_index_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_3195_112_2_18_2_35_36_216,(uint8*)"");
        return FALSE;
    }

    menu_list_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, page_index);

    if (PNULL == menu_list_ptr)
    {
        return FALSE;
    }
    // item总数
    total_item_num = menu_list_ptr->item_count;
    // 列数
    icon_col_num = menu_ctrl_ptr->slide_data.page_col_num;
    // 行数
    icon_row_num = menu_ctrl_ptr->slide_data.page_row_num;

    //get current item index and total item
    cur_item_index = item_index;

    // total page number
    total_page_num = menu_ctrl_ptr->item_total_num;

    // max item num in page
    page_item_total_num = SlidePageMenuGetInPageMaxItemNum(menu_ctrl_ptr);

    //get line number in one page
    total_line_num = SlidePageMenuGetTotalLineNum(menu_ctrl_ptr, page_index);

    // 最后一行的item数
    last_line_item_num = SlidePageMenuGetItemNumInLastLine(menu_ctrl_ptr, page_index);

    // 当前item所在的行
    cur_line_index = SlidePageMenuGetItemLineIndex(menu_ctrl_ptr, item_index);

    // 当前item所在的列
    cur_col_index = item_index - SlidePageMenuGetLineStartItemIndex(menu_ctrl_ptr, cur_line_index, menu_ctrl_ptr->cur_page_index);

    // 最后一行的开始index
    last_line_start_index = SlidePageMenuGetLineStartItemIndex(menu_ctrl_ptr, total_line_num - 1, menu_ctrl_ptr->cur_page_index);

    // 记录当前页
    next_page_index = page_index;
    next_item_index = item_index;

    switch (msg_id)
    {
    case MSG_KEYREPEAT_UP:
    case MSG_APP_UP:
        if (GO_SLIDE_SUPPORT_ONLY_Y == menu_ctrl_ptr->support_slide_type)
        {
            if (0 == cur_line_index)
            {
                if (0 == next_page_index) // 当前页为第一页，下一页为最后一页
                {
                    next_page_index = (uint16)(total_page_num - 1);
                }
                else
                {
                    next_page_index--; // 计算下页
                }

                next_page_item_total_num = SlidePageMenuGetPageTotalItemNum(menu_ctrl_ptr, next_page_index);
                next_item_index = next_page_item_total_num - 1;
            }
            else
            {
                if (CONTINUE_MOVE_UP == menu_ctrl_ptr->continue_move_type)
                {
                    cur_item_index = 0;
                }

                next_item_index = (uint16)(cur_item_index - icon_col_num);
            }
        }
        else
        {
            if (0 == cur_line_index)
            {
                next_item_index = last_line_start_index + cur_col_index;
            }
            else
            {
                if (CONTINUE_MOVE_UP == menu_ctrl_ptr->continue_move_type)
                {
                    cur_item_index = 0;
                }

                next_item_index = (uint16)(cur_item_index - icon_col_num);
            }
            // 纠正item索引号,防止超过范围
            if (next_item_index >= menu_list_ptr->item_count)
            {
                next_item_index = menu_list_ptr->item_count - 1;
            }
            // 纠正item索引号,防止超过范围
            if (0 == menu_list_ptr->item_count)
            {
                next_item_index = 0;
            }
        }
        break;

    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_DOWN:
        if (GO_SLIDE_SUPPORT_ONLY_Y == menu_ctrl_ptr->support_slide_type)
        {
            if ((total_line_num-1) == cur_line_index)
            {
                if ((total_page_num - 1) == next_page_index)
                {
                    next_page_index = 0;
                }
                else
                {
                    next_page_index++; // 计算下页
                }

                next_item_index = 0;
            }
            else
            {
                if (CONTINUE_MOVE_DOWN == menu_ctrl_ptr->continue_move_type)
                {
                    cur_item_index = 0 + cur_line_index*icon_col_num;
                }
                next_item_index = (uint16)(cur_item_index + icon_col_num);
            }
        }
        else
        {
            if (cur_line_index == total_line_num - 1) // 如果已经在最后一行
            {
                next_item_index = cur_col_index; // 如果是最后一行，下一个如果循环的话，肯定为列的索引
            }
            else
            {
                if (CONTINUE_MOVE_DOWN == menu_ctrl_ptr->continue_move_type)
                {
                    cur_item_index = 0 + cur_line_index*icon_col_num;
                }
                next_item_index = (uint16)(cur_item_index + icon_col_num);
            }
            // 纠正item索引号,防止超过范围
            if (next_item_index >= menu_list_ptr->item_count)
            {
                next_item_index = menu_list_ptr->item_count - 1;
            }
            // 纠正item索引号,防止超过范围
            if (0 == menu_list_ptr->item_count)
            {
                next_item_index = 0;
            }
        }
        break;

    case MSG_KEYREPEAT_LEFT:
    case MSG_APP_LEFT:
        if (GO_SLIDE_SUPPORT_ONLY_Y == menu_ctrl_ptr->support_slide_type)
        {
            if (0 < cur_item_index) // 在当前页移动
            {
                next_item_index = (uint16)(cur_item_index - 1);
            }
            else // 换页时
            {
                if (0 == next_page_index) // 当前页为第一页，下一页为最后一页
                {
                    next_page_index = (uint16)(total_page_num - 1);
                }
                else
                {
                    next_page_index--; // 计算下页
                }
                next_page_item_total_num = SlidePageMenuGetPageTotalItemNum(menu_ctrl_ptr, next_page_index);
                next_item_index = next_page_item_total_num-1; // 设置最后一项
            }
        }
        else
        {
            if (0 < cur_item_index) // 在当前页移动
            {
                next_item_index = (uint16)(cur_item_index - 1);
            }
            else // 换页时
            {
                next_item_index = 0; // 设置第0项
                if (0 == next_page_index) // 当前页为第一页，下一页为最后一页
                {
                    next_page_index = (uint16)(total_page_num - 1);
                }
                else
                {
                    next_page_index--; // 计算下页
                }
            }
        }
        break;

    case MSG_KEYREPEAT_RIGHT:
    case MSG_APP_RIGHT:
        if (GO_SLIDE_SUPPORT_ONLY_Y == menu_ctrl_ptr->support_slide_type)
        {
            if ((total_item_num - 1) > cur_item_index) // 在当前页移动
            {
                next_item_index = (uint16)(cur_item_index + 1);
            }
            else // 换页时候
            {
                next_item_index = 0; // 设置为0
                if (total_page_num - 1 == next_page_index) // 如果是最后一页
                {
                    next_page_index = 0; // 下一页为首页
                }
                else
                {
                    next_page_index++; // 下页累加
                }
            }

        }
        else
        {
            if ((total_item_num - 1) > cur_item_index) // 在当前页移动
            {
                next_item_index = (uint16)(cur_item_index + 1);
            }
            else // 换页时候
            {
                next_item_index = 0; // 设置为0
                if (total_page_num - 1 == next_page_index) // 如果是最后一页
                {
                    next_page_index = 0; // 下一页为首页
                }
                else
                {
                    next_page_index++; // 下页累加
                }
            }
        }
        break;

    default:
        result = FALSE;
        SCI_PASSERT(FALSE,("SlidePageMenuGetNextItemIndex:msg_id %d is error!",msg_id));/*assert verified*/
        break;
    }

    // 返回值
    if (PNULL != next_item_index_ptr)
    {
        *next_item_index_ptr = next_item_index;
    }
    if (PNULL != next_page_index_ptr)
    {
        *next_page_index_ptr = next_page_index;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 根据当前的Item获得当前页面的第一个Item索引
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuGetFirstItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           page_index,
    uint16           cur_item_index,
    uint16           *item_index_ptr
)
{
    int16   line_number         = 0;
    uint16  start_item_index    = 0;
    uint16  start_line_index    = 0;
    uint16  end_line_index      = 0;
    BOOLEAN result              = FALSE;
    MMIMAINEMENU_ITEM_LIST_T  *page_list_info_ptr = PNULL;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/


    page_list_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

    if (PNULL != page_list_info_ptr && PNULL != item_index_ptr)
    {
        start_item_index = 0;
        if (!SlidePageMenuIsItemInPage(menu_ctrl_ptr, cur_item_index, page_index))
        {
            start_line_index = SlidePageMenuGetItemLineIndex(menu_ctrl_ptr, 0);

            end_line_index = SlidePageMenuGetItemLineIndex(menu_ctrl_ptr, cur_item_index);

            result = TRUE;

            if (end_line_index < start_line_index) // 当前行在开始行之上
            {
                start_item_index = SlidePageMenuGetLineStartItemIndex(menu_ctrl_ptr, end_line_index, page_index);
            }
            else if (start_line_index < end_line_index) // 当前行在开始行之下
            {
                line_number = end_line_index - start_line_index + 1 - menu_ctrl_ptr->slide_data.page_row_num;
                if (line_number >= 0) // 当结束行和开始行之差值大于一页的时候
                {
                    start_item_index += line_number*menu_ctrl_ptr->slide_data.page_col_num;
                }
                else
                {
                    result = FALSE;
                }
            }
            else
            {
                result = FALSE;
            }
        }

        *item_index_ptr = start_item_index;
    }

    return result;
}

/*****************************************************************************/
//  Description : 根据当前的Item设置当前页面的第一个Item索引
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 只有在上下移动的时候当前item不在当前页时，调用该接口才返回TRUE
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuSetFirstItemIndex(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16           cur_page_index,
    uint16           cur_item_index
)
{
    BOOLEAN result = FALSE;
    uint16  start_item_index = 0;
    MMIMAINEMENU_ITEM_LIST_T *page_list_info_ptr = PNULL;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/



    page_list_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, cur_page_index);

    if (SlidePageMenuGetFirstItemIndex(menu_ctrl_ptr, cur_page_index, cur_item_index, &start_item_index))
    {
        result = TRUE;
    }
    menu_ctrl_ptr->first_item_index = 0;

    return result;
}

/*****************************************************************************/
// 	Description : search in list
//	Global resource dependence :
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
// LOCAL MMI_LINK_NODE_T *SlidePageMenuSearchSlideMenuItemByIndex(
//     GUIMENULIST_ITEM_T *menu_list_ptr,
//     uint16 item_index
// )
// {
//     BOOLEAN is_find = FALSE;
//     uint32  i = 0;
//     MMI_LINK_NODE_T *tmp_ptr = PNULL;
//     MMI_LINK_NODE_T *tmp_head_ptr = PNULL;
//
//     if (PNULL != menu_list_ptr)
//     {
//         tmp_head_ptr = tmp_ptr = menu_list_ptr->item_head_ptr;
//
//         if (PNULL != tmp_ptr)
//         {
//             if (0 == item_index)
//             {
//                 tmp_ptr = tmp_head_ptr;
//                 is_find = TRUE;
//             }
//             else
//             {
//                 for (i = 0; i < menu_list_ptr->item_count; i++)
//                 {
//                     // 当回到起始点的时候，肯定是没找到
//                     if (i == item_index && tmp_ptr != tmp_head_ptr)
//                     {
//                         is_find = TRUE;
//                         break;
//                     }
//                     // 指向下一个指针
//                     tmp_ptr = tmp_ptr->next_ptr;
//                 }
//             }
//         }
//     }
//
//     if (!is_find)
//     {
//         // 如果没找到，初始化为PNULL
//         tmp_ptr = PNULL;
//     }
//
//     return tmp_ptr;
// }

/*****************************************************************************/
//  Description : 读取nv中存储的菜单数据
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuReadItemInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    MMIMAINMENU_PAGE_ITEM_INFO_T *item_page_ptr
)
{
    uint16  i = 0;
    uint16  j = 0;
    uint16  page_num = 0;
    uint16  page_max_num = MMITHEME_GOMENU_ITEM_MAX_NUM;
    BOOLEAN result = FALSE;
#ifdef DYNAMIC_MAINMENU_SUPPORT
    //MMI_IMAGE_ID_T select_icon_id = 0;
#endif

    if (PNULL != item_page_ptr)
    {
        uint16            count_num = menu_ctrl_ptr->item_total_num;
        MMI_LINK_NODE_T * node_ptr = menu_ctrl_ptr->menu_list_info.list_head_ptr;
        MMI_LINK_NODE_T * temp_node_ptr = PNULL;

        if (MMITHEME_IsMainScreenLandscape())
        {
            page_max_num = MMITHEME_GOMENU_H_ITEM_MAX_NUM;
        }

        page_num = count_num / page_max_num;
        page_num = count_num % page_max_num == 0 ? page_num : page_num + 1;
        page_num = MIN( page_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM );
        menu_ctrl_ptr->item_total_num = page_num;

        // 读取静态菜单信息
        for (i = 0; i < page_num; i++)
        {
            for (j = 0; j < page_max_num && menu_ctrl_ptr->menu_list_info.list_head_ptr != temp_node_ptr; j++)
            {
                item_page_ptr[i].menu_item_info[j].group_id = ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->group_id;
                item_page_ptr[i].menu_item_info[j].menu_id = ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id;
                item_page_ptr[i].item_num++;
                node_ptr = node_ptr->next_ptr;
                temp_node_ptr = node_ptr;
            }
        }
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuInitSlideMenu(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    MMITHEME_GetMainMenuTheme(&menu_ctrl_ptr->main_menu_theme);

    MMITHEME_GetGoMenuInfo(menu_ctrl_ptr->win_handle,&menu_ctrl_ptr->slide_data);

    s_go_is_initialized = FALSE;
    SlidePageMenuInitData(menu_ctrl_ptr);

    MMITHEME_ResetSlideMenuItemTitleRect(&menu_ctrl_ptr->slide_data, menu_ctrl_ptr->item_total_num);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDestoryItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    uint16 i = 0;

    for (i=0; i<GO_MEM_PAGE_NUM; i++)
    {
        UILAYER_RELEASELAYER(&s_go_item_page_info[i].layer_handle);/*lint !e506 !e774*/
        s_go_item_page_info[i].index = 0;
        s_go_item_page_info[i].is_change = FALSE;
    }

    SCI_MEMSET(s_slidemenu_item_layer, 0, sizeof(s_slidemenu_item_layer));

    return;
}

/*****************************************************************************/
// 	Description : 销毁单，释放内存
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDestoryGOMenu(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{

    // 释放
    SlidePageMenuDestoryItemLayer(menu_ctrl_ptr);
    SlidePageMenuReleaseCalcDstLayer(menu_ctrl_ptr);

    // 保存当前页
    SlidePageMenuValidateCurPage(menu_ctrl_ptr);

    SlidePageMenuReleaseMoveIconLayer(menu_ctrl_ptr);

    // 释放页
    MMIMENU_DestroyPageList(&menu_ctrl_ptr->menu_page_info);

    // 释放
    SlidePageMenuResetItemListInfo(menu_ctrl_ptr);

    // 释放
    SlidePageMenuReleasePageBgLayer(menu_ctrl_ptr);
#ifdef DYNAMIC_MAINMENU_SUPPORT
    //释放动态菜单内存
    MMIMENU_DyFreeData();
#endif
    //释放高亮层
    SlidePageMenuReleaseHighlightIconLayer(menu_ctrl_ptr);

    // 释放
    SlidePageMenuReleaseTitleIdxLayer(menu_ctrl_ptr);

    SlidePageMenuGoReleaseMemory();

    s_go_is_initialized = FALSE;
    return;
}

#ifdef ATEST_SUPPORT
PUBLIC MMIMAINEMENU_ITEM_LIST_T *Atest_GetSlidePageMenuLinkListPtr(
    void   *qbtheme_ptr,
    uint32                       page_index)
{
	return GetSlidePageMenuLinkListPtr((MMIMAINMENU_SLIDEPAGE_DATA_T*)qbtheme_ptr, page_index);
}
#endif
/*****************************************************************************/
// Description : get item
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *GetSlidePageMenuLinkListPtr(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *qbtheme_ptr,
    uint32                       page_index
)
{
    uint32              i              = 0;
    MMI_LINK_NODE_T     *link_node_ptr = PNULL;
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;

    if (PNULL == qbtheme_ptr)
    {
        return PNULL;
    }

    link_node_ptr = qbtheme_ptr->menu_page_info.list_head_ptr;

    for (i = 0; i < qbtheme_ptr->menu_page_info.page_count; i++)
    {
        if ( PNULL == link_node_ptr )
        {
            break;
        }

        if (i == page_index)
        {
            page_item_info_ptr = (MMIMAINEMENU_ITEM_LIST_T *)link_node_ptr->data;
            break;
        }

        link_node_ptr = link_node_ptr->next_ptr;
    }

    return page_item_info_ptr;
}
/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplaySlidePageItem(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    uint16             page_index,
    GUI_LCD_DEV_INFO   lcd_dev_info
)
{
    uint16              i = 0;
    uint16              page_item_num = 0;
    GUI_RECT_T          disp_rect = {0};
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    page_item_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, page_index);

    if (PNULL != page_item_info_ptr)
    {
        page_item_num = page_item_info_ptr->item_count;

        if (MMIMENU_MAIN_OPTION_PAGE_DEFAULT_SEL >= SlidePageMenuGetPageTotalItemNum(menu_ctrl_ptr, page_index))
        {
            menu_ctrl_ptr->cur_item_index = 0;
        }

        for (i = 0; i < page_item_num; i++)
        {
            disp_rect = GetSlideMenuItemRect(menu_ctrl_ptr, i);

            if (i == menu_ctrl_ptr->cur_item_index)
            {
                MMIMAINMENU_ITEM_INFO_T* item_info_ptr = PNULL;

                item_info_ptr = SlidePageMenuGetItemInfoByIndex(page_item_info_ptr,i);

                if (menu_ctrl_ptr->is_delete_state &&
                        (PNULL == item_info_ptr || item_info_ptr->menu_id == menu_ctrl_ptr->delete_menu_id))
                    continue;
            }

            SlidePageMenuDisplayOneItemToLayer( menu_ctrl_ptr, &lcd_dev_info, &disp_rect, page_index, i );
        }
    }
}
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : display softkey bg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplaysoftkeybg(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,GUI_LCD_DEV_INFO  *lcd_dev_info_ptr, BOOLEAN is_left)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          softkey_width = 0;
    GUI_RECT_T      softkey_rect = {0};
    GUI_LCD_DEV_INFO    *dst_lcd_dev_info_ptr = lcd_dev_info_ptr;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle,&lcd_dev_info);
    if (PNULL == dst_lcd_dev_info_ptr)
    {
        dst_lcd_dev_info_ptr = &lcd_dev_info;
    }
    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if (is_left)
    {
        softkey_rect.left   = 0;
        softkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
        softkey_rect.right  = (int16)(lcd_width/2 - 1);
        softkey_rect.bottom = (int16)(lcd_height - 1);
    }
    else
    {
        softkey_rect.left   = (int16)(lcd_width/2);
        softkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
        softkey_rect.right  = (int16)(lcd_width - 1);
        softkey_rect.bottom = (int16)(lcd_height - 1);
    }

#if !defined MMI_MAINMENU_MINI_SUPPORT
    GUIRES_DisplayImg(PNULL,
                      &softkey_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      IMAGE_MAINMENU_SOFTKEY_BG,
                      (const GUI_LCD_DEV_INFO*)dst_lcd_dev_info_ptr);
#else
    GUIRES_DisplayImg(PNULL,
                      &softkey_rect,
                      PNULL,
                      menu_ctrl_ptr->win_handle,
                      IMAGE_NULL,
                      (const GUI_LCD_DEV_INFO*)dst_lcd_dev_info_ptr);
#endif
}
#endif
/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDrawBg(
    GUI_RECT_T        bg_rect,
    MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr,
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
//  Description : display slide page menu
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDraw(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
    GUI_RECT_T rect = {0};

    //滑动抖动过程中不重画
    if ( (0 != menu_ctrl_ptr->redrew_timer_id
            || menu_ctrl_ptr->is_move_state )
            && menu_ctrl_ptr->is_display_item_layer )
    {
        return;
    }

    if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_state)
    {
        return;
    }

    rect = MMITHEME_GetFullScreenRect();

    SlidePageMenuDrawBg(rect,menu_ctrl_ptr, PNULL);

    //lose focus 时有回拷动作，不需要画任何东东
    if (!MMK_IsFocusWin(menu_ctrl_ptr->win_handle))
    {
        SlidePageMenuGoDisplaySlidePageMainLayer(menu_ctrl_ptr);
        SlidePageMenuDisplaySlidePageItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, SlidePageMenuGetCurrentPageLyaerInfo(menu_ctrl_ptr));
#ifndef MMI_PDA_SUPPORT
        SlidePageMenuTransmitTpMsg(menu_ctrl_ptr, MSG_CTL_PAINT, PNULL);
#endif
        return;
    }

    menu_ctrl_ptr->is_delete_state = FALSE;
#ifdef DYNAMIC_MAINMENU_SUPPORT
    if (MMIMENU_DyCheckIsSyn())//check信息有变化
    {
        SlidePageMenuReloadItem(menu_ctrl_ptr);
    }
#endif

    if (!s_go_is_initialized)
    {
        SlidePageMenuInitData(menu_ctrl_ptr);
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_4226_112_2_18_2_35_38_217,(uint8*)"");

    // 检查内存是否申请成功
    if (!SlidePageMenuCheckLayerMem(menu_ctrl_ptr))
    {
        return;
    }

    // 画背景层上的内容
    SlidePageMenuGoDisplaySlidePageMainLayer(menu_ctrl_ptr);

    //画高亮层上的内容
    SlidePageMenuDisplayHighlightIconLayer(menu_ctrl_ptr);
    if (DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        if (!UILAYER_IsLayerActive(&menu_ctrl_ptr->layer_bg))
        {
            return;
        }
        //合并计算的层到主层
        SlidePageMenuSetBltSlideMenuPageLayer(menu_ctrl_ptr,menu_ctrl_ptr->titleidx_layer_handle.block_id,0, 0 );
    }
    else if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        if (!UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0]))
        {
            return;
        }
        //合并计算的层到主层
        SlidePageMenuSetBltSlideMenuPageLayer(menu_ctrl_ptr,menu_ctrl_ptr->page_layer_handle[0].block_id,menu_ctrl_ptr->titleidx_layer_handle.block_id, 0 );
    }

    SlidePageMenuDrawTitle(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle);

    SlidePageMenuDisplayItemLayer(menu_ctrl_ptr);

    //初始化选择框INFO
    SlidePageMenuSetSelectEffectStyle();

    return;
}
/*****************************************************************************/
//  Description : SlidePageMenuStartInitItemTimer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartInitItemTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
#define MMIMENU_STARTINITITEM_TIMER 5

    MMK_StopTimer(menu_ctrl_ptr->init_item_timer_id);

    menu_ctrl_ptr->init_item_timer_id = MMK_CreateWinTimer(
                                            menu_ctrl_ptr->ctrl_handle,
                                            MMIMENU_STARTINITITEM_TIMER,
                                            FALSE );
}
/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplayItemLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
    int16 current_frame = 0;
    int16 first_page = 0;
    int16 next_page =0;
    int16 page_num = 0;

    if ((!menu_ctrl_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        //默认要显示高亮
        SlidePageMenuInitItemData(menu_ctrl_ptr);

        page_num = MIN(menu_ctrl_ptr->item_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        first_page = menu_ctrl_ptr->cur_page_index;
        next_page = (first_page+page_num-1)%page_num;
        SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,TRUE);
    }
    else
    {
        if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
        {
            UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0],FALSE,FALSE);
        }

        UILAYER_Clear( &menu_ctrl_ptr->page_layer_handle[0] );

        SlidePageMenuDisplaySlidePageItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, SlidePageMenuGetCurrentPageLyaerInfo(menu_ctrl_ptr));

        SlidePageMenuStartInitItemTimer(menu_ctrl_ptr);
    }
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    SlidePageMenuStartHighlightIconDispTimer(menu_ctrl_ptr);
#endif
}

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGoDisplaySlidePageMainLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
    uint16            layer_width  = 0;
    uint16            layer_height = 0;
    GUI_RECT_T        rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0};

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);

    rect.left = 0;
    rect.top = 0;
    rect.right = layer_width - 1;
    rect.bottom = layer_height - 1;

    if (DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        SlidePageMenuDrawBg(rect,menu_ctrl_ptr,&menu_ctrl_ptr->layer_bg);
    }
    else
    {
        SlidePageMenuDrawBg(rect,menu_ctrl_ptr, PNULL);
    }

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleMenuDirectionKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id,
    GO_MOVE_DIRECTION_E  direction
)
{
    uint16      next_item_index = 0;
    uint16      next_page_index = 0;
    uint16      first_page_index =0;

    SCI_ASSERT(PNULL != menu_ctrl_ptr);/*assert verified*/

    SlidePageMenuGetNextItemIndex(
        menu_ctrl_ptr,
        menu_ctrl_ptr->cur_page_index,
        menu_ctrl_ptr->cur_item_index,
        msg_id,
        &next_item_index,
        &next_page_index);


    if (menu_ctrl_ptr->cur_page_index == next_page_index)
    {
        if (!SlidePageMenuSetFirstItemIndex(menu_ctrl_ptr, next_page_index, next_item_index))
        {
            menu_ctrl_ptr->cur_item_index = next_item_index;
            SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr,TRUE);
        }
        else
        {
            menu_ctrl_ptr->cur_item_index = next_item_index;
            SlidePageMenuDraw(menu_ctrl_ptr);
        }
    }
    else
    {
        if (direction != GO_MOVE_NONE)
        {
            first_page_index = menu_ctrl_ptr->cur_page_index;
            menu_ctrl_ptr->cur_item_index = next_item_index;
            menu_ctrl_ptr->cur_page_index = next_page_index;

            SlidePageMenuAutoSlidePage(menu_ctrl_ptr, first_page_index,next_page_index, direction);

            SlidePageMenuDisplayActiveIcon( menu_ctrl_ptr,TRUE);

            SlidePageMenuSetFirstItemIndex(menu_ctrl_ptr, next_page_index, next_item_index);
        }

    }
}
/*****************************************************************************/
//  Description : handle slide page menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleMenuUpKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;

    if (GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
    {
        direction = GO_MOVE_DOWN;
    }

    HandleMenuDirectionKey(menu_ctrl_ptr,MSG_APP_UP,direction);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle slide page menu down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleMenuDownKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;

    if (GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
    {
        direction = GO_MOVE_UP;
    }

    HandleMenuDirectionKey(menu_ctrl_ptr,MSG_APP_DOWN,direction);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle slide page menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleMenuLeftKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
)
{
    BOOLEAN     result = FALSE;
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;

    if (GO_SLIDE_SUPPORT_ONLY_X==menu_ctrl_ptr->support_slide_type)
    {
        direction = GO_MOVE_RIGHT;
    }
    else if (GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
    {
        direction = GO_MOVE_DOWN;
    }

    HandleMenuDirectionKey(menu_ctrl_ptr,msg_id,direction);
    return (result);
}

/*****************************************************************************/
//  Description : handle slide page menu up key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleMenuRightKey(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E   msg_id
)
{
    BOOLEAN     result = FALSE;
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;

    if (GO_SLIDE_SUPPORT_ONLY_X==menu_ctrl_ptr->support_slide_type)
    {
        direction = GO_MOVE_LEFT;
    }
    else if (GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
    {
        direction = GO_MOVE_UP;
    }

    HandleMenuDirectionKey(menu_ctrl_ptr,msg_id,direction);
    return (result);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SlidePageMenuHandleTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,		//事件类型
    DPARAM           param          //附带的处理参数
)
{

    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if (menu_ctrl_ptr->redrew_timer_id == *(uint8*)param)
    {
        GUI_POINT_T     point = {0};
        MMI_TP_STATUS_E state = MMI_TP_NONE;
#ifdef TOUCH_PANEL_SUPPORT
        MMK_GetLogicTPMsg(&state, &point);
#endif
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_4532_112_2_18_2_35_39_218,(uint8*)"d", SCI_GetTickCount());

        SlidePageMenuStopRedrawTimer(menu_ctrl_ptr);

        SlidePageMenuStartRedrawTimer(menu_ctrl_ptr);
        if ((abs(point.x - menu_ctrl_ptr->slide_pre_point.x) > 1
                &&GO_SLIDE_SUPPORT_ONLY_X == menu_ctrl_ptr->support_slide_type)
                ||(abs(point.y - menu_ctrl_ptr->slide_pre_point.y) > 1
                   &&GO_SLIDE_SUPPORT_ONLY_Y == menu_ctrl_ptr->support_slide_type)
           )
        {
            if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state
                    &&(GO_SLIDE_SUPPORT_ONLY_X==menu_ctrl_ptr->support_slide_type))
            {
                SlidePageMenuMoveGoMenuPageLayerX(menu_ctrl_ptr, &point);

                menu_ctrl_ptr->slide_pre_point.x = point.x;
                menu_ctrl_ptr->slide_pre_point.y = point.y;
            }
            else if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state
                     &&(GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type))
            {
                SlidePageMenuMoveGoMenuPageLayerY(menu_ctrl_ptr, &point);

                menu_ctrl_ptr->slide_pre_point.x = point.x;
                menu_ctrl_ptr->slide_pre_point.y = point.y;
            }
            else if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state
                     &&(GO_SLIDE_SUPPORT_XY==menu_ctrl_ptr->support_slide_type))
            {
                //根据距离计算
            }
        }
    }
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    else if (menu_ctrl_ptr->highlight_timer_id == *(uint8*)param)
    {
        SlidePageMenuStopHighlightIconDispTimer(menu_ctrl_ptr);

        if (menu_ctrl_ptr->is_active)
        {
            SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr,TRUE);
        }
    }
#endif
    else if (menu_ctrl_ptr->init_item_timer_id == *(uint8*)param)
    {
        MMK_StopTimer(menu_ctrl_ptr->init_item_timer_id);
        menu_ctrl_ptr->init_item_timer_id = 0;

        //初始化三页内容放在显示后面，加快进入速度
        SlidePageMenuInitItemData(menu_ctrl_ptr);
    }
    else if (menu_ctrl_ptr->shake_icon_timer_id == *(uint8*)param)
    {
        SlidePageMenuHandleOrigEvent(menu_ctrl_ptr);
    }
    else if (menu_ctrl_ptr->start_move_timer_id == *(uint8*)param)
    {
        GUIMENU_MOVE_STATE_T move_state = GUIMENU_STATE_MOVE_NONE;

        SlidePageMenuStopMoveStartTimer(menu_ctrl_ptr);

        move_state = GUIMENU_STATE_MOVE_SET;

        MMITHEME_Vibrate(MMITHEME_MENUMAIN_ICON_VIBRATE); // 震动一次

        MMK_SendMsg(menu_ctrl_ptr->win_handle, MSG_NOTIFY_MENU_SET_STATUS, (DPARAM)&move_state);
    }
    else
    {
        result = FALSE;
    }
    return result;
}


/*****************************************************************************/
//  Description : auto slide page layer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuAutoSlidePage(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 first_page_index,
    uint16 next_page_index,
    GO_MOVE_DIRECTION_E  direction
)
{
    uint16  current_frame  = 0;
    BOOLEAN	is_disp_active = FALSE;

    if (menu_ctrl_ptr->cur_page_index == first_page_index
            || 0 == menu_ctrl_ptr->item_total_num)
    {
        return;
    }

    SlidePageMenuHandleRandomStyle(menu_ctrl_ptr);

    menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

    if ((!menu_ctrl_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }

    for (current_frame = 0; current_frame <=GO_MAINMENU_TOTAL_FRAME_MAX; current_frame+=GO_FRAME_STEP_NUM)
    {
        SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page_index,next_page_index,direction,current_frame,is_disp_active);
        GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
    }

    if ((GO_MAINMENU_TOTAL_FRAME_MAX+GO_FRAME_STEP_NUM) != current_frame)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
        SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page_index,next_page_index,direction,current_frame,is_disp_active);
        GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
    }

    if ((direction==GO_MOVE_LEFT)||(direction==GO_MOVE_UP))
    {
        SlidePageMenuGoUpdateItemLayer(menu_ctrl_ptr,FALSE);
    }
    else if ((direction==GO_MOVE_RIGHT)||(direction==GO_MOVE_DOWN))
    {
        SlidePageMenuGoUpdateItemLayer(menu_ctrl_ptr,TRUE);
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_4658_112_2_18_2_35_39_219,(uint8*)"d",direction);
    }

    SlidePageMenuDrawTitle(menu_ctrl_ptr, next_page_index, menu_ctrl_ptr->titleidx_layer_handle);

    SlidePageMenuSetRandomSelectEffectStyle();

    return;
}

/*****************************************************************************/
//  Description : 显示高亮动画
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplayHighlightAnim(uint8* target_buf, IMGREF_SIZE_T target_window_size, MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr )
{
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    int32 i = 0;
    uint16* wallpaper_window_buf = PNULL;
    uint32* highlight_bg_image_buf = PNULL;
    uint32* highlight_fg_image_buf = PNULL;
    IMGREF_SIZE_T highlight_size = {0};
    FIX16_POINT_T highlight_offset= {0};
    UILAYER_INFO_T layer_struct_info = {0};
    uint8 *layer_buf_ptr = PNULL;
    GUI_LCD_DEV_INFO highlight_icon_layer_handle = {0,UILAYER_NULL_HANDLE};
    uint16 pre_item_index = 0;
    uint16 cur_item_index = 0;
    MMI_IMAGE_ID_T  highlight_image_id_begin = s_mmimenu_select_effect_info.select_image_id_begin;
    MMI_IMAGE_ID_T  highlight_image_id_end   = s_mmimenu_select_effect_info.select_image_id_end;
    MMI_IMAGE_ID_T  highlight_image_id_index = s_mmimenu_select_effect_info.select_image_id_index;
    MMIMENU_SELECT_EFFECT_RENDER_MODE_E select_effect_render_mode = s_mmimenu_select_effect_info.select_effect_render_mode;
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    uint16 icon_num = 0;
    S3D_DATA_TYPE_E  input_type = S3D_DATA_ARGB888;
    S3D_DATA_TYPE_E  output_type = S3D_DATA_ARGB888;
    FIX16_POINT_T window_offset={0};
    BOOLEAN ret = TRUE;

    if ( PNULL == s_cur_page_info_ptr )
    {
        return;
    }

    if ( !MMK_IsFocusWin( menu_ctrl_ptr->win_handle ) )
    {
        return;
    }

    //入参检测
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }
    if (PNULL == target_buf)
    {
        return;
    }

    //高亮图数据index循环
    if (highlight_image_id_index < highlight_image_id_end)
        highlight_image_id_index++;
    else
        highlight_image_id_index = highlight_image_id_begin;

    s_mmimenu_select_effect_info.select_image_id_index = highlight_image_id_index;

    //高亮图片解码
    ret = SlidePageMenuCreateHighlightLayerByID(menu_ctrl_ptr, highlight_image_id_index, &highlight_icon_layer_handle);
    if (FALSE == ret)
    {
        return;
    }

    UILAYER_GetLayerInfo(&highlight_icon_layer_handle, &layer_struct_info);
    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&highlight_icon_layer_handle);
    if (PNULL == layer_buf_ptr)
    {
        UILAYER_RELEASELAYER(&highlight_icon_layer_handle);
        return;
    }

    //获取高亮框的图片宽高和指针
    highlight_size.w = layer_struct_info.layer_width;
    if (select_effect_render_mode == SELECT_EFFECT_RENDER_BG)
    {
        highlight_size.h = layer_struct_info.layer_height;
        highlight_bg_image_buf = (uint32*)layer_buf_ptr;
    }
    else if (select_effect_render_mode == SELECT_EFFECT_RENDER_FG)
    {
        highlight_size.h = layer_struct_info.layer_height;
        highlight_fg_image_buf = (uint32*)layer_buf_ptr;
    }
    else //SELECT_EFFECT_RENDER_BOTH
    {
        highlight_size.h = layer_struct_info.layer_height/2;
        highlight_bg_image_buf = (uint32*)layer_buf_ptr;
        highlight_fg_image_buf = (uint32*)(layer_buf_ptr + highlight_size.w*highlight_size.h*(sizeof(uint32)));
    }
    //获取图标区域数据
    pre_item_index = menu_ctrl_ptr->pre_cur_item_index;
    cur_item_index = menu_ctrl_ptr->cur_item_index;
    menu_ctrl_ptr->pre_cur_item_index = cur_item_index;


    //重画背景图

    if (DATA_TYPE_PMARGB888 != SEAPI_GetCalcOutputDataType())
    {
        wallpaper_window_buf = (uint16*)UILAYER_GetLayerBufferPtr(&menu_ctrl_ptr->layer_bg);
        if (PNULL == wallpaper_window_buf)
        {
            return;
        }
        SCI_MEMCPY(target_buf,wallpaper_window_buf,target_window_size.w*target_window_size.h*sizeof(uint16));
    }
    else
    {
        UILAYER_SetPreMultiplyAlpha(&menu_ctrl_ptr->page_layer_handle[0],TRUE,TRUE);

        UILAYER_Clear(&menu_ctrl_ptr->page_layer_handle[0]);
    }
    //获取当前页的结构体指针(全局)
    SlidePageMenuGoGetPageInfo(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index,s_cur_page_info_ptr);
    highlight_offset.x = (s_cur_page_info_ptr->icon_info+SlidePageMenuGetPageIndex(menu_ctrl_ptr, cur_item_index))->icon_offset.x;
    highlight_offset.y = (s_cur_page_info_ptr->icon_info+SlidePageMenuGetPageIndex(menu_ctrl_ptr, cur_item_index))->icon_offset.y;

    highlight_offset.x -= (highlight_size.w-(s_cur_page_info_ptr->icon_info+SlidePageMenuGetPageIndex(menu_ctrl_ptr, cur_item_index))->icon_size.w)/2;
    highlight_offset.y -= (highlight_size.h-(s_cur_page_info_ptr->icon_info+SlidePageMenuGetPageIndex(menu_ctrl_ptr, cur_item_index))->icon_size.h)/2;
    highlight_offset.y -= 2*GO_CALC_OFFSET;

    //开始画当前帧数据
    input_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcInputDataType());
    output_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcOutputDataType());

    //画背景高亮数据(在图标下面)
    if (select_effect_render_mode == SELECT_EFFECT_RENDER_BG || select_effect_render_mode == SELECT_EFFECT_RENDER_BOTH)
    {
        S2D_TypeBlending(target_window_size, target_buf,output_type,highlight_size,highlight_offset,highlight_bg_image_buf, input_type,target_buf,PNULL);
    }

    //画所有图标(带字)
#if 1
    page_item_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);
    icon_num = SlidePageMenuGetPageTotalItemNum(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

    for (i=0; i<icon_num; i++)
    {
        if (NULL!=(*(s_cur_page_info_ptr->icon_info+i)).icon_buf)
        {
            //贴图标
            window_offset.x=(s_cur_page_info_ptr->icon_info+i)->icon_offset.x;
            window_offset.y=(s_cur_page_info_ptr->icon_info+i)->icon_offset.y;

            S2D_TypeBlending(target_window_size, target_buf,output_type,(s_cur_page_info_ptr->icon_info+i)->icon_size,window_offset,(s_cur_page_info_ptr->icon_info +i)->icon_buf,input_type, target_buf,PNULL);
        }
    }
#endif
    //画前景高亮数据(在图标上面)
    if (select_effect_render_mode == SELECT_EFFECT_RENDER_FG || select_effect_render_mode == SELECT_EFFECT_RENDER_BOTH)
    {
        S2D_TypeBlending(target_window_size, target_buf,output_type, highlight_size,highlight_offset, highlight_fg_image_buf, input_type,target_buf,PNULL);
    }

    //高亮图片层handle释放
    SlidePageMenuReleaseHighlightIconLayerByHander(&highlight_icon_layer_handle);

    return;
#endif
}

/*****************************************************************************/
//  Description : SlidePageMenuDisplayActiveIcon
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplayActiveIcon(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    BOOLEAN is_display_highlight
)
{
    int16 current_frame = 0;
    int16 first_page = 0;
    int16 next_page =0;
    int16 page_num = 0;
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint8 *target_buf = PNULL;
    IMGREF_SIZE_T target_window_size = {0};
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;
    UILAYER_INFO_T dst_layer_info_ptr = {0};

#endif

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    mainmenu_select_style = MMIAPISET_GetMainmenuSelectStyle();
    if (SELECT_EFFECT_STATIC == mainmenu_select_style)//静态
    {
        page_num = MIN(menu_ctrl_ptr->item_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        first_page = menu_ctrl_ptr->cur_page_index;
        next_page = (first_page+page_num-1)%page_num;
        SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,is_display_highlight);
    }
    else//动态高亮框
    {
        if (DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
        {
            target_buf = UILAYER_GetLayerBufferPtr(&menu_ctrl_ptr->page_layer_handle[0]);
            UILAYER_GetLayerInfo(&menu_ctrl_ptr->page_layer_handle[0],&dst_layer_info_ptr);
            target_window_size.w = dst_layer_info_ptr.layer_width;
            target_window_size.h = dst_layer_info_ptr.layer_height;
        }
        else
        {
            GUI_LCD_DEV_INFO temp_lcd_dev_info = {0};
            GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle,&temp_lcd_dev_info);
            target_buf = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
            GUILCD_GetLogicWidthHeight(temp_lcd_dev_info.lcd_id, (uint16*)&target_window_size.w, (uint16*)&target_window_size.h);
        }

        if (PNULL == target_buf)
        {
            return;
        }

        SlidePageMenuStartHighlightIconDispTimer(menu_ctrl_ptr);

        SlidePageMenuDisplayHighlightAnim(target_buf, target_window_size, menu_ctrl_ptr);
#ifdef WIN32
        SCI_Sleep(30);
#endif
    }
#else
    {
        page_num = MIN(menu_ctrl_ptr->item_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        first_page = menu_ctrl_ptr->cur_page_index;
        next_page = (first_page+page_num-1)%page_num;
        SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,is_display_highlight);
    }
#endif
}

/*****************************************************************************/
//  Description : reset icon display
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuResetSlidePageIconDisplay(
    MMIMAINMENU_SLIDEPAGE_DATA_T  *menu_ctrl_ptr,
    uint16 item_index,
    BOOLEAN is_disp_move_bg
)
{
    SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr,FALSE);
}

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleMenuNumKey(
    uint16           num_index,
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    return FALSE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleRandomStyle(
    MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr
)
{
    if (GO_MENU_STYLE_RANDOM== MMIAPISET_GetGOmenuStyle())
    {
        do
        {
            s_random_cur_style = rand()%GO_MENU_STYLE_MAX;
        }
        while ((s_random_cur_style == GO_MENU_STYLE_RANDOM)
                ||(UAPIMenu_GetCalcStyleSlideType(s_random_cur_style) == GO_SLIDE_SUPPORT_ONLY_Y)
              );

        menu_ctrl_ptr->support_slide_type = UAPIMenu_GetCalcStyleSlideType(s_random_cur_style);
    }
}
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartMoveStartTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->start_move_timer_id)
    {
        menu_ctrl_ptr->start_move_timer_id = MMK_CreateWinTimer(
                                                 menu_ctrl_ptr->ctrl_handle,
                                                 menu_ctrl_ptr->main_menu_theme.icon_move_start_period,
                                                 FALSE );
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopMoveStartTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->start_move_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->start_move_timer_id);
        menu_ctrl_ptr->start_move_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle page touch panel press down msg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleSlidePageTpDown(GUI_POINT_T       *tp_point_ptr,
        MMIMAINMENU_SLIDEPAGE_DATA_T    *menu_ctrl_ptr
                                             )
{
    uint16              i = 0;
    uint16              total_num = 0;
    GUI_RECT_T          item_rect = {0};
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    uint16              item_index = 0;
    GUI_POINT_T      tp_point = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    tp_point.x = tp_point_ptr->x;
    tp_point.y = tp_point_ptr->y;
    menu_ctrl_ptr->is_tp_pressed = TRUE;

    // 增加速度点
    MMK_AddVelocityItem( tp_point.x, tp_point.y );

    //get title total number
    total_num = menu_ctrl_ptr->item_total_num;

    // 停止timer
    SlidePageMenuStopRedrawTimer(menu_ctrl_ptr);
    SlidePageMenuStopMoveStartTimer(menu_ctrl_ptr);

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    SlidePageMenuStopHighlightIconDispTimer(menu_ctrl_ptr);
#endif

    // 首先设置页面的滑动相关数据
    // 记录起始点
    menu_ctrl_ptr->slide_start_point.x = tp_point.x;
    menu_ctrl_ptr->slide_start_point.y = tp_point.y;

    // 记录前一个点
    menu_ctrl_ptr->slide_pre_point.x = tp_point.x;
    menu_ctrl_ptr->slide_pre_point.y = tp_point.y;

    menu_ctrl_ptr->pre_tp_point = *tp_point_ptr;

    // 下面检查tp是否在item区域，并作相应处理
    page_item_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

    if (PNULL == page_item_info_ptr)
    {
        total_num = 0;
    }
    else
    {
        //get item total number
        total_num = SlidePageMenuGetPageItemNum(menu_ctrl_ptr, page_item_info_ptr->item_count);
    }

    //if press item icon
    for (i = 0; i < total_num; i++)
    {
        item_index = SlidePageMenuGetItemIndex(menu_ctrl_ptr, i);
        //set item item_rect
        item_rect = GetSlideMenuItemRect(menu_ctrl_ptr, i);

        //if press item
        if (GUI_PointIsInRect(tp_point,item_rect))
        {
            menu_ctrl_ptr->tmp_width = tp_point.x - item_rect.left;
            menu_ctrl_ptr->tmp_height = tp_point.y - item_rect.top;

            menu_ctrl_ptr->is_item_pressed = TRUE;

            menu_ctrl_ptr->cur_item_index = item_index;
            //item_index = SlidePageMenuGetItemIndex(menu_ctrl_ptr, i);
            //switch current slide page


            SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr, TRUE);
            break;
        }
    }

    if ( menu_ctrl_ptr->is_item_pressed )
    {
        BOOLEAN             is_grid_idle = FALSE;
        MMISET_IDLE_STYLE_E idle_style = MMISET_IDLE_STYLE_COMMON;

        idle_style =  MMIAPISET_GetIdleStyle();

        if (MMISET_IDLE_STYLE_GRID == idle_style || MMISET_IDLE_QBTHEME == idle_style)
        {
            is_grid_idle = TRUE;
        }

        // 不是移动图标状态,启动Timer,dockbar上的item不支持移动
        //if (is_grid_idle && !menu_ctrl_ptr->is_move_state)
		if (!menu_ctrl_ptr->is_move_state)
        {
            SlidePageMenuStartMoveStartTimer(menu_ctrl_ptr);
        }
    }
#ifndef MMI_PDA_SUPPORT
    SlidePageMenuTransmitTpMsg(menu_ctrl_ptr, MSG_TP_PRESS_DOWN, tp_point_ptr);
#endif
    // 不是移动图标状态,启动Timer
    SlidePageMenuStartRedrawTimer(menu_ctrl_ptr);

    SlidePageMenuHandleRandomStyle(menu_ctrl_ptr);

    return ;
}

/*****************************************************************************/
//  Description : handle tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuHandleTpMove(GUI_POINT_T       *tp_point_ptr,
                                     MMIMAINMENU_SLIDEPAGE_DATA_T*menu_ctrl_ptr
                                    )
{
    GUI_POINT_T      tp_point = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    tp_point.x = tp_point_ptr->x;
    tp_point.y = tp_point_ptr->y;
    // 增加速度点
    MMK_AddVelocityItem( tp_point.x, tp_point.y );

    // 当move消息来时，停止timer
    if ((abs(tp_point.x - menu_ctrl_ptr->slide_start_point.x) > 2
            &&GO_SLIDE_SUPPORT_ONLY_X == menu_ctrl_ptr->support_slide_type)
            ||(abs(tp_point.y - menu_ctrl_ptr->slide_start_point.y) > 2
               &&GO_SLIDE_SUPPORT_ONLY_Y == menu_ctrl_ptr->support_slide_type)
       )
    {
        if (!menu_ctrl_ptr->is_item_pressed || !menu_ctrl_ptr->is_move_state)
        {
            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
        }

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        SlidePageMenuStopHighlightIconDispTimer(menu_ctrl_ptr);
#endif

        SlidePageMenuStopMoveStartTimer(menu_ctrl_ptr);

        SlidePageMenuSetMoveLayerPosition( menu_ctrl_ptr );
    }
#ifndef MMI_PDA_SUPPORT
    SlidePageMenuTransmitTpMsg(menu_ctrl_ptr, MSG_TP_PRESS_MOVE, tp_point_ptr);
#endif
    menu_ctrl_ptr->pre_tp_point.x = tp_point_ptr->x;
    menu_ctrl_ptr->pre_tp_point.y = tp_point_ptr->y;

    return ;
}

/*****************************************************************************/
//  Description : handle slide page touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSlideCurPage_X(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    uint16              current_frame = 0;
    if (0 ==menu_ctrl_ptr->item_total_num)
    {
        return FALSE;
    }

    current_frame = (abs(tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);

    // 向左移动
    if (tp_point_ptr->x < menu_ctrl_ptr->slide_start_point.x)
    {
        // 判断是否最后一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/2)
                || menu_ctrl_ptr->fling_velocity < -FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index < menu_ctrl_ptr->item_total_num - 1)
            {
                menu_ctrl_ptr->cur_page_index++;
                menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->cur_page_index%menu_ctrl_ptr->item_total_num;
            }
            else
            {
                menu_ctrl_ptr->cur_page_index = 0;
            }

            menu_ctrl_ptr->cur_item_index = 0;
        }
    }
    // 向右移动
    else
    {
        // 判断是否第一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/2)
                || menu_ctrl_ptr->fling_velocity > FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index != 0)
            {
                menu_ctrl_ptr->cur_page_index--;
            }
            else
            {
                menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->item_total_num - 1;
            }

            menu_ctrl_ptr->cur_item_index = 0;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : handle slide page touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSlideCurPage_Y(
    GUI_POINT_T      *tp_point_ptr,
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    uint16              current_frame = 0;
    if (0 ==menu_ctrl_ptr->item_total_num)
    {
        return FALSE;
    }

    current_frame = (abs(tp_point_ptr->y - menu_ctrl_ptr->slide_start_point.y)/GO_ONE_FRAME_PIX);

    // 向左移动
    if (tp_point_ptr->y < menu_ctrl_ptr->slide_start_point.y)
    {
        // 判断是否最后一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/3)
                || menu_ctrl_ptr->fling_velocity < -FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index < menu_ctrl_ptr->item_total_num - 1)
            {
                menu_ctrl_ptr->cur_page_index++;
                menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->cur_page_index%menu_ctrl_ptr->item_total_num;
            }
            else
            {
                menu_ctrl_ptr->cur_page_index = 0;
            }

            menu_ctrl_ptr->cur_item_index = 0;
        }
    }
    else // 向右移动
    {
        // 判断是否第一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/3)
                || menu_ctrl_ptr->fling_velocity > FLING_MIN_VELOCITY)
        {
            if (menu_ctrl_ptr->cur_page_index != 0)
            {
                menu_ctrl_ptr->cur_page_index--;
            }
            else
            {
                menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->item_total_num - 1;
            }

            menu_ctrl_ptr->cur_item_index = 0;
        }
    }

    return TRUE;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSlideOverDraw(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    uint16              pre_page_index,
    uint16              second_page,
    GO_MOVE_DIRECTION_E direction,
    uint16              current_frame,
    BOOLEAN             is_change_page
)
{
    int16 i = 0;
    BOOLEAN is_disp_active = FALSE;

    if ((!menu_ctrl_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }

    if (TRUE == is_change_page)
    {
        for (i = current_frame; i<= GO_MAINMENU_TOTAL_FRAME_MAX; i+=GO_FRAME_STEP_NUM)
        {

            SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );

        }

        if ((GO_MAINMENU_TOTAL_FRAME_MAX+GO_FRAME_STEP_NUM) != i)
        {
            i = GO_MAINMENU_TOTAL_FRAME_MAX;
            SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        }

        if ((direction==GO_MOVE_LEFT)||(direction==GO_MOVE_UP))
        {
            SlidePageMenuGoUpdateItemLayer(menu_ctrl_ptr,FALSE);
        }
        else if ((direction==GO_MOVE_RIGHT)||(direction==GO_MOVE_DOWN))
        {
            SlidePageMenuGoUpdateItemLayer(menu_ctrl_ptr,TRUE);
        }
        else
        {
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_5371_112_2_18_2_35_40_221,(uint8*)"d",direction);
        }

    }
    else
    {
        for (i = current_frame; i>= 0; i-=GO_FRAME_STEP_NUM)
        {
            SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        }

        if (-GO_FRAME_STEP_NUM != i)
        {
            i = 0;
            SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        }
    }

    SlidePageMenuDrawTitle(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->titleidx_layer_handle);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleGoMenuSlideOverX(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)

{
    BOOLEAN             result         = FALSE;
    uint16              second_page    = 0;
    uint16              pre_page_index = 0;
    uint16              current_frame  = 0;
    uint16              page_num  = MIN(menu_ctrl_ptr->item_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;

    BOOLEAN is_change_page = FALSE;

    pre_page_index = menu_ctrl_ptr->cur_page_index;

    // 设置下一页
    (void)SetSlideCurPage_X(tp_point_ptr, menu_ctrl_ptr);

    if (pre_page_index == menu_ctrl_ptr->cur_page_index)
    {
        is_change_page = FALSE;
    }
    else
    {
        is_change_page = TRUE;
        SlidePageMenuSetRandomSelectEffectStyle();
    }

    if (tp_point_ptr->x > menu_ctrl_ptr->slide_start_point.x)
    {
        direction = GO_MOVE_RIGHT;
        second_page = (pre_page_index+page_num-1)%page_num;
    }
    else
    {
        direction = GO_MOVE_LEFT;
        second_page = (pre_page_index+1)%page_num;
    }

    current_frame = (abs(tp_point_ptr->x - menu_ctrl_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);


    if (current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }

    SlidePageMenuSlideOverDraw(menu_ctrl_ptr,pre_page_index,second_page,direction,current_frame,is_change_page);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    SlidePageMenuStartHighlightIconDispTimer(menu_ctrl_ptr);
#endif
    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleGoMenuSlideOverY(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    GUI_POINT_T      *tp_point_ptr
)

{
    BOOLEAN             result         = FALSE;
    BOOLEAN             is_change_page = FALSE;
    uint16              second_page    = 0;
    uint16              pre_page_index = 0;
    uint16              current_frame  = 0;
    uint16              page_num  = MIN(menu_ctrl_ptr->item_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
    GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;

    pre_page_index = menu_ctrl_ptr->cur_page_index;

    // 设置下一页
    (void)SetSlideCurPage_Y(tp_point_ptr, menu_ctrl_ptr);

    if (pre_page_index == menu_ctrl_ptr->cur_page_index)
    {
        is_change_page = FALSE;
    }
    else
    {
        is_change_page = TRUE;
        SlidePageMenuSetRandomSelectEffectStyle();
    }

    if (tp_point_ptr->y > menu_ctrl_ptr->slide_start_point.y)
    {
        direction = GO_MOVE_DOWN;
        second_page = (pre_page_index+page_num-1)%page_num;
    }
    else
    {
        direction = GO_MOVE_UP;
        second_page = (pre_page_index+1)%page_num;
    }

    current_frame = (abs(tp_point_ptr->y - menu_ctrl_ptr->slide_start_point.y)/GO_ONE_FRAME_PIX);


    if (current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }

    SlidePageMenuSlideOverDraw(menu_ctrl_ptr,pre_page_index,second_page,direction,current_frame,is_change_page);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    SlidePageMenuStartHighlightIconDispTimer(menu_ctrl_ptr);
#endif
    return result;
}
/*****************************************************************************/
//  Description : 判断tp是否在toidle区域
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsTpInToidleRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN	   is_disp_to_idle = FALSE;
    GUI_RECT_T move_rect = {0};
    GUI_RECT_T clip_rect = {0};
    GUI_RECT_T to_idle_rect = {0};

	if (0 == menu_ctrl_ptr->to_idle_layer_handle.block_id)
	{
        return FALSE;
	}

    move_rect = SlidePageMenuGetSlideMenuMoveLayerRect(menu_ctrl_ptr);

    to_idle_rect = UILAYER_GetLayerRect(&menu_ctrl_ptr->to_idle_layer_handle);

    if (GUI_IntersectRect(&clip_rect, move_rect, to_idle_rect))
    {
        is_disp_to_idle = TRUE;
    }

    return is_disp_to_idle;
}
/*****************************************************************************/
//  Description : 判断tp是否在删除区域
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsTpInDeleteitemRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
    BOOLEAN	   is_disp_delete = FALSE;
    GUI_RECT_T move_rect = {0};
    GUI_RECT_T clip_rect = {0};
    GUI_RECT_T delete_rect = {0};

    move_rect = SlidePageMenuGetSlideMenuMoveLayerRect(menu_ctrl_ptr);

    delete_rect = UILAYER_GetLayerRect(&menu_ctrl_ptr->delete_layer_handle);

    if (GUI_IntersectRect(&clip_rect, move_rect, delete_rect))
    {
        is_disp_delete = TRUE;
    }

    return is_disp_delete;
}
/*****************************************************************************/
//  Description : 显示toidle bar
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuDisplayToIdleBar(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_disp_to_idle = SlidePageMenuIsTpInToidleRect(menu_ctrl_ptr);
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T icon_rect = {0};
    MMI_IMAGE_ID_T to_idle_icon_id = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;

    if (0 == menu_ctrl_ptr->to_idle_layer_handle.block_id
		|| UILAYER_NULL_HANDLE == menu_ctrl_ptr->to_idle_layer_handle.block_id)
    {
        return FALSE;
    }

    GUIRES_GetImgWidthHeight(&icon_width, &icon_height,
                             menu_ctrl_ptr->main_menu_theme.to_idle_icon_id,
                             menu_ctrl_ptr->win_handle);

    icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);

    disp_rect = icon_rect;
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &menu_ctrl_ptr->to_idle_layer_handle, &disp_rect, &disp_rect);

    UILAYER_Clear(&menu_ctrl_ptr->to_idle_layer_handle);

    to_idle_icon_id = is_disp_to_idle?menu_ctrl_ptr->main_menu_theme.to_idle_sle_icon_id:menu_ctrl_ptr->main_menu_theme.to_idle_icon_id;

    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(PNULL,
                      &disp_rect,
                      &icon_rect,
                      menu_ctrl_ptr->win_handle,
                      to_idle_icon_id,
                      &menu_ctrl_ptr->to_idle_layer_handle);

    IMG_EnableTransparentColor(FALSE);

    return result;
}
/*****************************************************************************/
//  Description : 显示delitem bar
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuDisplayDelItemBar(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_delete_item = SlidePageMenuIsTpInDeleteitemRect(menu_ctrl_ptr);
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T icon_rect = {0};
    MMI_IMAGE_ID_T delete_icon_id = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;

    GUIRES_GetImgWidthHeight(&icon_width, &icon_height,
                             menu_ctrl_ptr->main_menu_theme.delete_icon_id,
                             menu_ctrl_ptr->win_handle);

    icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);

    disp_rect = icon_rect;
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &menu_ctrl_ptr->delete_layer_handle, &disp_rect, &disp_rect);

    UILAYER_Clear(&menu_ctrl_ptr->delete_layer_handle);

    delete_icon_id = is_delete_item?menu_ctrl_ptr->main_menu_theme.delete_sel_icon_id:menu_ctrl_ptr->main_menu_theme.delete_icon_id;

    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(PNULL,
                      &disp_rect,
                      &icon_rect,
                      menu_ctrl_ptr->win_handle,
                      delete_icon_id,
                      &menu_ctrl_ptr->delete_layer_handle);

    IMG_EnableTransparentColor(FALSE);

    return result;
}

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 删除动态菜单项
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDeleteDynamicItem(void)
{
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = (MMIMAINMENU_SLIDEPAGE_DATA_T *)GUIMAINMENU_GetPM(MAINMENUWIN_MENU_CTRL_ID);

    if (PNULL != menu_ctrl_ptr)
    {
        CTRLMAINMENU_ITEM_T menuitem_info = {0};

        MMIMAINMENU_GetItemData(
            menu_ctrl_ptr->delete_menu_id,
            DYNAMIC_MAINMENU_DEFAULT_GROUP_ID,
            &menuitem_info);

        //判断此要删除的item信息是否正确
        if (0 != menuitem_info.group_id
                && PNULL != menuitem_info.call_back_delete)
        {
            GUIMAINMENU_DY_ITEM_T item_info = {0};
            BOOLEAN is_info_get = FALSE;
            BOOLEAN is_delete = FALSE;

            is_info_get = MMIMENU_DySlideGetRunInfo(menu_ctrl_ptr->delete_menu_id, &item_info);

            if (is_info_get
                    && PNULL != item_info.ori_data_ptr
                    && MMIMENU_DyIsRunInfoValid(&(item_info.ori_data_ptr->dynamic_menu_info)))
            {
                //运行删除应用的回调
                is_delete = (menuitem_info.call_back_delete)(
                                (uint32)item_info.ori_data_ptr->dynamic_menu_info.param1ptr,
                                (uint32)item_info.ori_data_ptr->dynamic_menu_info.param2ptr
                            );
            }

            MMIMENU_DyFreeParmInfo(&item_info);

            //if (is_delete)
            {
                menu_ctrl_ptr->is_delete_state = TRUE;
                MMIMENU_DyDeleteItemEx(&menuitem_info);
            }
        }
    }
}
#endif

/*****************************************************************************/
//  Description : to handle the asp message list of delete query window
//  Global resource dependence :
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQbthemeQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T  query_win_id = MAIN_MAINMENU_QBTHEME_QUERY_WIN_ID;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MMIPUB_CloseQuerytWin(&query_win_id);
#if defined DYNAMIC_MAINMENU_SUPPORT
        SlidePageMenuDeleteDynamicItem();
#endif
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}
/*****************************************************************************/
// Description : 处理tpup消息
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void SlidePageMenuOpenQueryDeleteWin(void)
{
    MMI_WIN_ID_T query_win_id = MAIN_MAINMENU_QBTHEME_QUERY_WIN_ID;

    MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY,
                                IMAGE_PUBWIN_QUERY,
                                &query_win_id,
                                HandleQbthemeQueryWinMsg);

    return;
}

/*****************************************************************************/
// Description:  是否拖动到主菜单/删除
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuIsInToIdleRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    BOOLEAN is_handle_tp,
    BOOLEAN *is_need_redraw_ptr
)
{
    BOOLEAN is_need_delete = FALSE;
    BOOLEAN is_disp_to_idle = FALSE;
    BOOLEAN is_disp_delete = FALSE;

    // 判断是不是可以删除
    is_need_delete = SlidePageMenuIsItemEnableDeleted(menu_ctrl_ptr);

    // 判断当前move的区域与toidle bar的区域是否有重合
    is_disp_to_idle = SlidePageMenuIsTpInToidleRect(menu_ctrl_ptr);

    if (is_need_delete)
    {
        // 判断当前move的区域与deleteitem bar是否有从何
        is_disp_delete = SlidePageMenuIsTpInDeleteitemRect(menu_ctrl_ptr);
    }

    //是否处理tp tp消息
    if (!is_handle_tp)
    {
        // 显示toidle bar
        SlidePageMenuDisplayToIdleBar(menu_ctrl_ptr);

        if (is_need_delete)
        {
            // 显示delitem bar
            SlidePageMenuDisplayDelItemBar(menu_ctrl_ptr);
        }

        MMITHEME_UpdateRect();
    }
#ifdef DYNAMIC_MAINMENU_SUPPORT
    else if (is_disp_delete)//需要删除此应用
    {
        do
        {
            MMIMAINMENU_ITEM_INFO_T* item_info_ptr = PNULL;
            MMIMAINEMENU_ITEM_LIST_T *  page_item_info_ptr = PNULL;
            CTRLMAINMENU_ITEM_T menuitem_info = {0};

            UILAYER_Clear(&menu_ctrl_ptr->icon_move_layer_handle);

            //get item index and rect
            page_item_info_ptr = GetSlidePageMenuLinkListPtr(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);

            if (PNULL != page_item_info_ptr)
            {
                item_info_ptr = SlidePageMenuGetItemInfoByIndex(page_item_info_ptr, menu_ctrl_ptr->cur_item_index);
            }

            if ( PNULL == item_info_ptr )
                break;

            if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID != item_info_ptr->group_id)
                break;

            MMIMAINMENU_GetItemData(item_info_ptr->menu_id, item_info_ptr->group_id, &menuitem_info);

            {
                menu_ctrl_ptr->delete_menu_id = menuitem_info.menu_id;

                SlidePageMenuOpenQueryDeleteWin();

                if (PNULL != is_need_redraw_ptr)
                {
                    *is_need_redraw_ptr = FALSE;
                }
            }
        }
        while (0);
    }
#endif

    return is_disp_to_idle;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void SlidePageMenuReleaseOrigSpecialLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    // 释放层
    if (UILAYER_IsMultiLayerEnable())/*lint !e774*/
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->to_idle_layer_handle);/*lint !e506 !e774*/
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->delete_layer_handle);/*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : handle slide menu touch panel press up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuHandleSlidePageTpUp(
    GUI_POINT_T       *tp_point_ptr,
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    float               fling_velocity_x = 0;
    float               fling_velocity_y = 0;
    uint16              page_item_num    = 0;
    BOOLEAN             result = FALSE;
    BOOLEAN             is_in_menu_ctrl  = TRUE;
    GUI_POINT_T         tp_point = {0};

    if (PNULL == menu_ctrl_ptr || !menu_ctrl_ptr->is_tp_pressed)
    {
        return result;
    }

    tp_point.x = tp_point_ptr->x;
    tp_point.y = tp_point_ptr->y;

#ifdef TOUCH_PANEL_SUPPORT
    // 增加速度点
    MMK_AddVelocityItem( tp_point.x, tp_point.y );

    MMK_ComputeCurrentVelocity((float*)&fling_velocity_x, (float*)&fling_velocity_y,(float)FLING_MAX_VELOCITY,(float)FLING_MAX_VELOCITY );
#endif

    if (GO_SLIDE_SUPPORT_ONLY_X==menu_ctrl_ptr->support_slide_type)
    {
        menu_ctrl_ptr->fling_velocity = fling_velocity_x;
    }
    else if (GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
    {
        menu_ctrl_ptr->fling_velocity = fling_velocity_y;
    }
    else if (GO_SLIDE_SUPPORT_XY==menu_ctrl_ptr->support_slide_type)
    {
        menu_ctrl_ptr->fling_velocity = 0;
    }

    //GUIMENU_StopAutoSwitchTimer(menu_ctrl_ptr);
    SlidePageMenuStopRedrawTimer(menu_ctrl_ptr);
    SlidePageMenuStopMoveStartTimer(menu_ctrl_ptr);

    menu_ctrl_ptr->is_tp_pressed = FALSE;
    result = menu_ctrl_ptr->is_item_pressed;
    menu_ctrl_ptr->is_item_pressed = FALSE;

    if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state)
    {
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_5692_112_2_18_2_35_41_222,(uint8*)"d", SCI_GetTickCount());

        // 处理滑动
        if (GO_SLIDE_SUPPORT_ONLY_X==menu_ctrl_ptr->support_slide_type)
        {
            result = SlidePageMenuHandleGoMenuSlideOverX(menu_ctrl_ptr, &tp_point);
        }
        else if (GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
        {
            result = SlidePageMenuHandleGoMenuSlideOverY(menu_ctrl_ptr, &tp_point);
        }
        else if (GO_SLIDE_SUPPORT_XY==menu_ctrl_ptr->support_slide_type)
        {
            //result = SlidePageMenuHandleGoMenuSlideOverXy(menu_ctrl_ptr, &tp_point);
        }
        return FALSE;
    }

    page_item_num = SlidePageMenuGetInPageMaxItemNum(menu_ctrl_ptr);

    if (menu_ctrl_ptr->main_menu_theme.is_hl_disappear)
    {
        SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr,FALSE);
    }
    else
    {
        SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr,TRUE);
    }

    if (menu_ctrl_ptr->is_move_state)
    {
        BOOLEAN is_need_redraw = TRUE;

        SlidePageMenuIsInToIdleRect(menu_ctrl_ptr, TRUE, &is_need_redraw);

        menu_ctrl_ptr->is_empty_space = FALSE;
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        menu_ctrl_ptr->is_move_state = FALSE;
        menu_ctrl_ptr->is_item_pressed = FALSE;

        menu_ctrl_ptr->cur_item_index = menu_ctrl_ptr->tmp_cur_item_index;
        menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->tmp_first_item_index;
        menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->tmp_page_item_index;

        // 释放层
        SlidePageMenuReleaseMoveIconLayer(menu_ctrl_ptr);

        SlidePageMenuReleaseOrigSpecialLayer(menu_ctrl_ptr);

        result = FALSE;
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_5745_112_2_18_2_35_41_223,(uint8*)"dd", result, is_in_menu_ctrl);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    SlidePageMenuStartHighlightIconDispTimer(menu_ctrl_ptr);
#endif

#ifndef MMI_PDA_SUPPORT
    SlidePageMenuTransmitTpMsg(menu_ctrl_ptr, MSG_TP_PRESS_UP, tp_point_ptr);
#endif

    //if press item
//     if (GUI_PointIsInRect(tp_point,GetSlideMenuItemRect(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index)))
// 	{
// 	    int16 item_index = 0;
//         uint16 page_item_num1 = menu_ctrl_ptr->slide_data.page_col_num*menu_ctrl_ptr->slide_data.page_row_num;
//
// 	    SlidePageMenuGetItemIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
// 	    item_index = page_item_num1 * menu_ctrl_ptr->cur_page_index + menu_ctrl_ptr->cur_item_index;
//         SlidePageMenuRunItem(menu_ctrl_ptr,item_index);
// 	}
    return (result);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN U_CheckLayerMem(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = TRUE;
    int i = 0;

    if (!UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0]))
    {
        result = FALSE;
    }
    else
    {
        for (i=0; i<GO_MEM_PAGE_NUM; i++)
        {
            if (!UILAYER_IsLayerActive(&s_go_item_page_info[i].layer_handle))
            {
                SCI_TRACE_LOW("SlidePageMenuCheckLayerMem invalid layer i = %d, handle = %d", i, s_go_item_page_info[i].layer_handle.block_id);
                result = FALSE;
                break;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : SlidePageMenuCheckLayerMem
//  Global resource dependence :
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuCheckLayerMem(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = TRUE;

    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    result = U_CheckLayerMem(menu_ctrl_ptr);

    if (!result)
    {
        //释放缓冲区
        BLKMEM_GarbageClean();

        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
        UILAYER_SetLayerMemFormat(&menu_ctrl_ptr->page_layer_handle[0],UILAYER_MEM_NONE);

        //重新初始化 申请内存
        UILAYER_ActiveAllLayerOfHandle( menu_ctrl_ptr->ctrl_handle );

        //再次check
        result = U_CheckLayerMem(menu_ctrl_ptr);

        if (!result)
        {
            MMISET_IDLE_STYLE_E     idle_style = MMIAPISET_GetIdleStyle();
            MMISET_IDLE_SUBSTYLE_E  cur_idle_substyle = MMIAPISET_GetIdleSubStyle();

            if (MMISET_IDLE_STYLE_GRID == idle_style&&MMISET_IDLE_SUBSTYLE_DEFAULT == cur_idle_substyle)
            {
                MMK_CloseWin(menu_ctrl_ptr->win_handle);
                MMIAPISET_SetMainmenuStyle(MMIMAINMENU_MATRIX_E);
                MMIAPIMENU_CreatMainMenu();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
                MMK_CloseWin(menu_ctrl_ptr->win_handle);
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideMenuGrayEvent(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
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
        if ( PNULL != is_gray_ptr && (*is_gray_ptr))
        {
            if (!menu_ctrl_ptr->main_menu_theme.is_hl_disappear || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr, TRUE);
            }
            else
            {
                SlidePageMenuDisplayActiveIcon(menu_ctrl_ptr, FALSE);
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
LOCAL MMI_RESULT_E SlidePageMenuHandleSpecialMsg(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM          param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    HandleSlideMenuGrayEvent(menu_ctrl_ptr, msg_id, param);

    switch (msg_id)
    {
    case MSG_CTL_LOSE_FOCUS:
        SlidePageMenuGoReleaseMemory();
        //GUIMENU_StopAutoSwitchTimer(menu_ctrl_ptr);
        SlidePageMenuStopRedrawTimer(menu_ctrl_ptr);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        SlidePageMenuStopHighlightIconDispTimer(menu_ctrl_ptr);
#endif
        menu_ctrl_ptr->is_move_state = FALSE;

        menu_ctrl_ptr->is_display_item_layer = FALSE;
        break;

    case MSG_CTL_OPEN:
        menu_ctrl_ptr->menu_state |= GUIMENU_STATE_ENTER;

    case MSG_CTL_GET_FOCUS: /*lint !e616 !e825*/
#ifdef DYNAMIC_MAINMENU_SUPPORT
        if (MMIMENU_DyCheckIsSyn())
        {
            SlidePageMenuGoSetInitialized(FALSE);
        }
#endif
        SlidePageMenuCreatGoMemory(menu_ctrl_ptr);
        break;

    case MSG_TIMER:
        if (MMK_IsFocusWin( menu_ctrl_ptr->win_handle ))
        {
            SlidePageMenuHandleTimer(menu_ctrl_ptr, msg_id, param);
        }
        else
        {
            //GUIMENU_StopAutoSwitchTimer(menu_ctrl_ptr);
            SlidePageMenuStopRedrawTimer(menu_ctrl_ptr);

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
            SlidePageMenuStopHighlightIconDispTimer(menu_ctrl_ptr);
#endif
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : up date rect
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: peilong.zhou
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStoreUpdateRect(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr)
{
    uint16     i = 0,j = 0;
    GUI_RECT_T menu_rect = {0};
    GUI_RECT_T title_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    menu_rect = SlidePageMenuAdjustRect(menu_ctrl_ptr);

    title_rect.bottom = menu_rect.top-1;
    title_rect.right = menu_rect.right;

    if (DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        uint16* buf_ptr = PNULL;

        buf_ptr = (uint16*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);

        if (PNULL == buf_ptr)
        {
            return;
        }

        for (i=0; i<=title_rect.bottom; i++)
        {
            for (j=0; j<=title_rect.right; j++)
            {
                *buf_ptr = 0X0000;
                buf_ptr++;
            }
        }
        GUIWIN_UpdateStbIcon();
        MMITHEME_StoreUpdateRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info, menu_rect);
    }
}
/*****************************************************************************/
// Description : 创建一个服务信息结点指针
// Global resource dependence :
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *SlidePageMenuCreateMenuItemNodeInfo(
    uint16 item_num
)
{
    MMIMAINEMENU_ITEM_LIST_T *new_item_list_ptr = PNULL;

    new_item_list_ptr = SCI_ALLOC_APP(sizeof(MMIMAINEMENU_ITEM_LIST_T));
    if (PNULL != new_item_list_ptr)
    {
        //new_item_list_ptr->item_count = item_num;
        new_item_list_ptr->list_head_ptr = PNULL;
    }
    if (0 != new_item_list_ptr)
    {
        SCI_MEMSET(new_item_list_ptr,0,sizeof(MMIMAINEMENU_ITEM_LIST_T));
    }
    return new_item_list_ptr;
}

/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence :
//  Author: jin.wang
//  RETRUN:  返回指针
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *SlidePageMenuAddItemList(
    MMIMAINEMENU_PAGE_LIST_T     *menu_page_ptr,
    uint16                       item_num
)
{
    MMIMAINEMENU_ITEM_LIST_T *new_item_list_ptr = PNULL;

    if (PNULL == menu_page_ptr || 0 == item_num)
    {
        return new_item_list_ptr;
    }

    // 创建一个服务信息节点,这里申请了内存
    new_item_list_ptr = SlidePageMenuCreateMenuItemNodeInfo(item_num);

    if (PNULL != new_item_list_ptr)
    {
        if (PNULL == menu_page_ptr->list_head_ptr)
        {
            menu_page_ptr->list_head_ptr = MMILINK_CreateHead((uint32)new_item_list_ptr);
        }
        else
        {
            MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_list_ptr);
            MMILINK_AddNodeBeforeBaseNode(menu_page_ptr->list_head_ptr, tmp_node_ptr);
        }

        menu_page_ptr->page_count++;
    }

    return new_item_list_ptr;
}
/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence :
//  Author: jin.wang
//  RETRUN:  返回指针
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *SlidePageMenuAddItemEx(
    MMIMAINEMENU_ITEM_LIST_T    *item_list_ptr,
    MMIMAINMENU_ITEM_INFO_T     *item_info_ptr
)
{
    MMIMAINMENU_ITEM_INFO_T *new_item_info_ptr = PNULL;

    if (PNULL == item_list_ptr || PNULL == item_info_ptr)
    {
        return new_item_info_ptr;
    }

    // 创建一个服务信息节点,这里申请了内存
    new_item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));
    SCI_MEMCPY(new_item_info_ptr, item_info_ptr, sizeof(MMIMAINMENU_ITEM_INFO_T));

    if (PNULL == item_list_ptr->list_head_ptr)
    {
        item_list_ptr->list_head_ptr = MMILINK_CreateHead((uint32)new_item_info_ptr);
    }
    else
    {
        MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_info_ptr);
        MMILINK_AddNodeBeforeBaseNode(item_list_ptr->list_head_ptr, tmp_node_ptr);
    }

    item_list_ptr->item_count++;

    return new_item_info_ptr;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuCreateMenuList(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *qbtheme_ptr,
    MMIMAINEMENU_PAGE_LIST_T     *menu_page_ptr,
    MMIMAINMENU_ITEM_INFO_T      *item_info_ptr,
    uint16                       item_num
)
{
    BOOLEAN result = FALSE;
    uint16 item_index = 0;
    MMIMAINEMENU_ITEM_LIST_T *new_item_list_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T   new_item = {0};

    // 添加一页信息
    new_item_list_ptr = SlidePageMenuAddItemList(menu_page_ptr, item_num);

    if (PNULL != new_item_list_ptr)
    {
        // 添加item
        for (item_index = 0; item_index < item_num; item_index++)
        {
            new_item.menu_id = item_info_ptr[item_index].menu_id;
            new_item.group_id = item_info_ptr[item_index].group_id;
            SlidePageMenuAddItemEx(new_item_list_ptr, &new_item);
        }

        result = TRUE;
    }

    return result;
}



/*****************************************************************************/
//  Description : init Menu data
//  Global resource dependence :
//  Author: peilong.zhou
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuInitData(MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr)
{
    uint16            page_index = 0;
    uint16            item_num = 0;
    GO_MENU_STYLE_E   go_menu_style  = MMIAPISET_GetGOmenuStyle();

    MMIMAINMENU_PAGE_ITEM_INFO_T page_item_info[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM] = {0};

    menu_ctrl_ptr->support_slide_type = UAPIMenu_GetCalcStyleSlideType(go_menu_style);

    // get item data
    SlidePageMenuResetItemListInfo(menu_ctrl_ptr);
    SlidePageMenuReadItemList(menu_ctrl_ptr, &item_num);
    menu_ctrl_ptr->item_total_num = item_num;

    // insert data to page
    SlidePageMenuReadItemInfo(menu_ctrl_ptr,page_item_info);

    // add to list
    for (page_index = 0; page_index < menu_ctrl_ptr->item_total_num; page_index++)
    {
        SlidePageMenuCreateMenuList(
            menu_ctrl_ptr,
            &menu_ctrl_ptr->menu_page_info,
            page_item_info[page_index].menu_item_info,
            page_item_info[page_index].item_num);

    }

    // 创建bg层
    SlidePageMenuCreateSlideMenuBgLayer(menu_ctrl_ptr);

    SlidePageMenuCreateHighlightIconLayer(menu_ctrl_ptr);

    // 创建title显示层
    SlidePageMenuCreateSlideMenuTitleIdxLayer(menu_ctrl_ptr);

#ifndef MMI_PDA_SUPPORT
    // 创建softkey
    SlidePageMenuCreateIconMenuButton(menu_ctrl_ptr);
#endif
    //申请算法计算完成输出层
    SlidePageMenuCreateCalcDstLayer(menu_ctrl_ptr, SlidePageMenuAdjustRect(menu_ctrl_ptr));

    SlidePageMenuCreateItemLayer(menu_ctrl_ptr);
    //申请计算MEM
    SlidePageMenuCreatGoMemory(menu_ctrl_ptr);

    GUIWIN_SetStbState(menu_ctrl_ptr->win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
    GUIWIN_SetStbBgIsDesplay(menu_ctrl_ptr->win_handle,FALSE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    GUIWIN_SetStbBgImageId(menu_ctrl_ptr->win_handle, IMAGE_IDLE_STATUS_BAR);
#endif
    menu_ctrl_ptr->cur_item_index = 0;

    s_go_is_initialized = TRUE;
}

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : 设置点击特效类型INFO
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetSelectEffectStyleInfo(MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style)
{

    switch (mainmenu_select_style)
    {
    case SELECT_EFFECT_STATIC:
        break;
    case SELECT_EFFECT_RANDOM:
    case SELECT_EFFECT_ROUNDLIGHT:
        s_mmimenu_select_effect_info.select_image_id_begin = IMAGE_MAINMENU_HIGHLIGHT_ROUNDLIGHT0;
        s_mmimenu_select_effect_info.select_image_id_end   = IMAGE_MAINMENU_HIGHLIGHT_ROUNDLIGHT9;
        s_mmimenu_select_effect_info.select_image_id_index = IMAGE_MAINMENU_HIGHLIGHT_ROUNDLIGHT0;
        s_mmimenu_select_effect_info.select_effect_render_mode = SELECT_EFFECT_RENDER_FG;
        break;

    case SELECT_EFFECT_FIRE:
        s_mmimenu_select_effect_info.select_image_id_begin = IMAGE_MAINMENU_HIGHLIGHT_FIRE0;
        s_mmimenu_select_effect_info.select_image_id_end   = IMAGE_MAINMENU_HIGHLIGHT_FIRE9;
        s_mmimenu_select_effect_info.select_image_id_index = IMAGE_MAINMENU_HIGHLIGHT_FIRE0;
        s_mmimenu_select_effect_info.select_effect_render_mode = SELECT_EFFECT_RENDER_FG;
        break;

    case SELECT_EFFECT_LIGHTING:
        s_mmimenu_select_effect_info.select_image_id_begin = IMAGE_MAINMENU_HIGHLIGHT_LIGHTING0;
        s_mmimenu_select_effect_info.select_image_id_end   = IMAGE_MAINMENU_HIGHLIGHT_LIGHTING9;
        s_mmimenu_select_effect_info.select_image_id_index = IMAGE_MAINMENU_HIGHLIGHT_LIGHTING0;
        s_mmimenu_select_effect_info.select_effect_render_mode = SELECT_EFFECT_RENDER_BG;
        break;
    case SELECT_EFFECT_WATER:
        s_mmimenu_select_effect_info.select_image_id_begin = IMAGE_MAINMENU_HIGHLIGHT_WATER0;
        s_mmimenu_select_effect_info.select_image_id_end   = IMAGE_MAINMENU_HIGHLIGHT_WATER17;
        s_mmimenu_select_effect_info.select_image_id_index = IMAGE_MAINMENU_HIGHLIGHT_WATER0;
        s_mmimenu_select_effect_info.select_effect_render_mode = SELECT_EFFECT_RENDER_FG;
        break;
    case SELECT_EFFECT_SNOW:
        s_mmimenu_select_effect_info.select_image_id_begin = IMAGE_MAINMENU_HIGHLIGHT_SNOW0;
        s_mmimenu_select_effect_info.select_image_id_end   = IMAGE_MAINMENU_HIGHLIGHT_SNOW19;
        s_mmimenu_select_effect_info.select_image_id_index = IMAGE_MAINMENU_HIGHLIGHT_SNOW0;
        s_mmimenu_select_effect_info.select_effect_render_mode = SELECT_EFFECT_RENDER_BG;
        break;
    default:
        //SCI_PASSERT(FALSE,("SlidePageMenuSetSelectEffectStyleInfo:mainmenu_select_style %d is error!",mainmenu_select_style));
		SCI_TRACE_LOW("SlidePageMenuSetSelectEffectStyleInfo:mainmenu_select_style %d is error!",mainmenu_select_style);
		break;
    }

}
#endif
/*****************************************************************************/
//  Description : 设置点击特效类型INFO
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetSelectEffectStyle(void)
{
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;

    mainmenu_select_style = MMIAPISET_GetMainmenuSelectStyle();

    SlidePageMenuSetSelectEffectStyleInfo(mainmenu_select_style);
#endif
}

/*****************************************************************************/
//  Description : 设置选中随机
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetRandomSelectEffectStyle(void)
{
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;

    mainmenu_select_style = MMIAPISET_GetMainmenuSelectStyle();

    if (SELECT_EFFECT_RANDOM == mainmenu_select_style)
    {
        mainmenu_select_style = rand()%(SELECT_EFFECT_RANDOM-1) + 1;

        SlidePageMenuSetSelectEffectStyleInfo(mainmenu_select_style);
    }
#endif
}

#ifdef DYNAMIC_MAINMENU_SUPPORT

/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence :
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReloadItem( MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr)
{

    // 释放
    SlidePageMenuDestoryItemLayer(menu_ctrl_ptr);

    // 释放
    SlidePageMenuResetItemListInfo(menu_ctrl_ptr);

    // 释放
    SlidePageMenuReleasePageBgLayer(menu_ctrl_ptr);

    // 释放页
    MMIMENU_DestroyPageList(&menu_ctrl_ptr->menu_page_info);
#ifdef DYNAMIC_MAINMENU_SUPPORT
    //释放动态菜单内存
    MMIMENU_DyFreeData();
#endif
    //释放高亮层
    SlidePageMenuReleaseHighlightIconLayer(menu_ctrl_ptr);

    // 释放
    SlidePageMenuReleaseTitleIdxLayer(menu_ctrl_ptr);

    SlidePageMenuGoReleaseMemory();

    s_go_is_initialized = FALSE;

    SlidePageMenuInitData(menu_ctrl_ptr);

    // 保存当前页
    SlidePageMenuValidateCurPage(menu_ctrl_ptr);

    MMITHEME_ResetSlideMenuItemTitleRect(&menu_ctrl_ptr->slide_data, menu_ctrl_ptr->item_total_num);

}
#endif
#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence :
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuChangeGoMenuStyle(MMI_HANDLE_T win_handle)
{
    int16 current_frame = 0;
    int16 first_page = 0;
    int16 next_page =0;
    int16 page_num = 0;
    MMIMAINMENU_TYPE_E  mainmenu_style = MMIMAINMENU_TYPE_MAX;
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr = PNULL;
    GO_MENU_STYLE_E cur_style = MMIAPISET_GetGOmenuStyle();

    mainmenu_style = GUIMAINMENU_GetStyle(MMK_GetCtrlHandleByWin(win_handle, MAINMENUWIN_MENU_CTRL_ID));

    if (MMIMAINMENU_GO_SLIDE_E == mainmenu_style)
    {
        menu_ctrl_ptr = (MMIMAINMENU_SLIDEPAGE_DATA_T *)GUIMAINMENU_GetPM(MMK_GetCtrlHandleByWin(win_handle, MAINMENUWIN_MENU_CTRL_ID));

        if (PNULL != menu_ctrl_ptr)
        {
            do
            {
                cur_style +=1;
                cur_style = (uint32)cur_style%GO_MENU_STYLE_MAX;
                MMISET_SetGOmenuStyle(cur_style);

            }
            while (0);

            menu_ctrl_ptr->support_slide_type = UAPIMenu_GetCalcStyleSlideType(MMIAPISET_GetGOmenuStyle());

            page_num = MIN(menu_ctrl_ptr->item_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);

            first_page = menu_ctrl_ptr->cur_page_index;
            next_page = (first_page+page_num-1)%page_num;

            //过场动画，显示主菜单特效
            if (GO_SLIDE_SUPPORT_ONLY_Y==menu_ctrl_ptr->support_slide_type)
            {
                for (current_frame = (GO_MAINMENU_TOTAL_FRAME_MAX/2); current_frame>=0; current_frame-=GO_FRAME_STEP_NUM)
                {
                    SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
                if (-GO_FRAME_STEP_NUM != current_frame)
                {
                    current_frame = 0;
                    SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
            }
            else
            {
                for (current_frame = (GO_MAINMENU_TOTAL_FRAME_MAX/2); current_frame>=0; current_frame-=GO_FRAME_STEP_NUM)
                {
                    SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
                if (-GO_FRAME_STEP_NUM!= current_frame)
                {
                    current_frame = 0;
                    SlidePageMenuDrawUpdateGoMenuLayer(menu_ctrl_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
            }
            //SlidePageMenuDisplayActiveIcon( menu_ctrl_ptr,TRUE);
        }
    }
}

/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence :
//  Author:kyle jin
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ChangeGOMenuStyle(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T    win_handle = 0;
    MMI_HANDLE_T    ctrl_handle = 0;
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr = PNULL;

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MAINMENUWIN_MENU_CTRL_ID);
    menu_ctrl_ptr = (MMIMAINMENU_SLIDEPAGE_DATA_T *)GUIMAINMENU_GetPM(ctrl_handle);

    if (PNULL == menu_ctrl_ptr)
    {
        //如果menu控件指针为NULL，意味着主菜单窗口没有打开，重进主菜单
        //在idle点击这个icon的时候，就不会出现没有响应的问题
        win_handle = MMIAPIMENU_CreatMainMenu();
    }
    else
    {
        win_handle = menu_ctrl_ptr->win_handle;
    }

    SlidePageMenuChangeGoMenuStyle(win_handle);
}

#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T SlidePageMenuGetSlideMenuMoveLayerRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    GUI_RECT_T      tmp_rect = {0};
    GUI_POINT_T     point = {0};
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
/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuAppendMainMenuLayer(
    GUI_LCD_DEV_INFO lcd_dev_info,
    UILAYER_LEVEL_E layer_level
)
{
    UILAYER_APPEND_BLT_T append_layer = {0};


    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = layer_level;//UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    return;
}

/*****************************************************************************/
//  Description : set move layer position
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuSetMoveLayerPosition(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (PNULL != menu_ctrl_ptr)
    {
        GUI_RECT_T move_rect = SlidePageMenuGetSlideMenuMoveLayerRect(menu_ctrl_ptr);

        SlidePageMenuAppendMainMenuLayer(menu_ctrl_ptr->icon_move_layer_handle, UILAYER_LEVEL_HIGH);

        UILAYER_SetLayerPosition(&menu_ctrl_ptr->icon_move_layer_handle, move_rect.left, move_rect.top);
    }
}

/*****************************************************************************/
//  Description : 处理组织功能
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 处理抖动timer
/*****************************************************************************/
LOCAL void SlidePageMenuHandleOrigEvent(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    SlidePageMenuStopShakeIconTimer(menu_ctrl_ptr);

    menu_ctrl_ptr->pre_cur_item_index = menu_ctrl_ptr->cur_item_index;

    if (menu_ctrl_ptr->is_move_state)
    {
        SlidePageMenuStartShakeIconTimer(menu_ctrl_ptr);
    }

    if (MMK_TP_SLIDE_NONE != menu_ctrl_ptr->slide_state)
    {
        return;
    }

    // 设置合并的层，这里只需要把第一个page的层合并即可
    if (menu_ctrl_ptr->is_move_state)
    {
        SlidePageMenuSetMoveLayerPosition( menu_ctrl_ptr );

        if (SlidePageMenuIsInToIdleRect(menu_ctrl_ptr, FALSE, PNULL))
        {
            if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
            {
                SlidePageMenuStopMoveStartTimer(menu_ctrl_ptr);
                SlidePageMenuStopShakeIconTimer(menu_ctrl_ptr);
                SlidePageMenuStopRedrawTimer(menu_ctrl_ptr);

                if (menu_ctrl_ptr->is_item_pressed)
                {
                    GUI_POINT_T  current_point = {0};
                    LCD_ANGLE_E  current_angle = LCD_ANGLE_0;
                    LCD_ANGLE_E  idlewin_angle = LCD_ANGLE_0;
                    int16 temp = 0;
                    MMI_TP_STATUS_E state = MMI_TP_NONE;
                    uint16  lcd_width = 0;
                    uint16  lcd_height= 0;
#ifdef TOUCH_PANEL_SUPPORT
                    MMK_GetLogicTPMsg(&state, &current_point);
#endif
                    MMK_GetWinAngle(MAIN_IDLE_WIN_ID,&idlewin_angle);
                    MMK_GetWinAngle(MAIN_MAINMENU_WIN_ID,&current_angle);
                    GUILCD_GetWidthHeightByAngle(MAIN_LCD_ID,idlewin_angle,&lcd_width,&lcd_height);

                    switch (current_angle - idlewin_angle)		/*lint !e656*/
                    {
                    case LCD_ANGLE_90:
                        temp  = current_point.x;
                        current_point.x = (int16)(lcd_width - current_point.y);
                        current_point.y =  (int16)temp;
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

                    MMK_SendMsg(menu_ctrl_ptr->win_handle, MSG_NOTIFY_QBTHEME_SHORTCUT2IDLE, (DPARAM)&current_point);
                }
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuReleaseMoveIconLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    // 释放层
    if (UILAYER_IsMultiLayerEnable())
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->icon_move_layer_handle);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateMoveIconLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    int16				page_index = 0;
    GUI_RECT_T			icon_rect = {0};
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_RESULT_E	result = UILAYER_RESULT_SUCCESS;

    if (!SlidePageMenuIsItemInPage(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, menu_ctrl_ptr->cur_page_index))
    {
        return;
    }

    //不创建move_icon的时机:
	//1.icon是静态menu,当idle不是qbidle,也不是grididle时
	if (!SlidePageMenuIsItemEnableDeleted(menu_ctrl_ptr)
		&& MMISET_IDLE_STYLE_GRID != MMIAPISET_GetIdleStyle() 
		&& MMISET_IDLE_QBTHEME != MMIAPISET_GetIdleStyle())
	{
        return;
	}

    SlidePageMenuReleaseMoveIconLayer(menu_ctrl_ptr);

    //get selected bar display rect
    page_index = SlidePageMenuGetPageIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);

    icon_rect = GetSlideMenuItemRect(menu_ctrl_ptr, 0);

    if (0 == menu_ctrl_ptr->icon_move_layer_handle.block_id
            || UILAYER_NULL_HANDLE == menu_ctrl_ptr->icon_move_layer_handle.block_id)
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = icon_rect.left;
        create_info.offset_y = icon_rect.top;
        create_info.width = (icon_rect.right + 1 - icon_rect.left);
        create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_FPS_ACCELERATE;

        result = UILAYER_CreateLayer(
                     &create_info,
                     &menu_ctrl_ptr->icon_move_layer_handle
                 );

        if (UILAYER_RESULT_SUCCESS == result)
        {
            UILAYER_Clear(&menu_ctrl_ptr->icon_move_layer_handle);

            SlidePageMenuDisplayOneItemToLayer( menu_ctrl_ptr, &menu_ctrl_ptr->icon_move_layer_handle, &icon_rect,
                                                menu_ctrl_ptr->cur_page_index, menu_ctrl_ptr->cur_item_index );

            SlidePageMenuSetMoveLayerPosition( menu_ctrl_ptr );
        }
    }
}

/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence :
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetCurrentPageLyaerInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    if (UILAYER_IsMultiLayerEnable()
            && UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0]))
    {
        lcd_dev_info = menu_ctrl_ptr->page_layer_handle[0];
    }

    return lcd_dev_info;
}

/*****************************************************************************/
//  Description : create move bg layer
//  Global resource dependence :
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void SlideMenuCreateMoveBgLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr  //in
)
{
    GUI_LCD_DEV_INFO lcd_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    lcd_info = GetCurrentPageLyaerInfo(menu_ctrl_ptr);
    UILAYER_WeakLayerAlpha(&lcd_info, MMIMENU_WEAKLAYERALPHA);
}
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartShakeIconTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->shake_icon_timer_id)
    {
        menu_ctrl_ptr->shake_icon_timer_id = MMK_CreateWinTimer(
                                                 menu_ctrl_ptr->ctrl_handle,
                                                 menu_ctrl_ptr->main_menu_theme.icon_shake_period,
                                                 FALSE);
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopShakeIconTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->shake_icon_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->shake_icon_timer_id);
        menu_ctrl_ptr->shake_icon_timer_id = 0;
    }
}
/*****************************************************************************/
//  Description : create qbtheme icon layer
//  Global resource dependence :
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuCreateOrigSpecialLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,  //in
    BOOLEAN			is_to_idle
)
{
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T icon_rect = {0};
    UILAYER_CREATE_T create_info = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    BOOLEAN is_enable_del = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;

    if (is_to_idle)
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height,
                                 menu_ctrl_ptr->main_menu_theme.to_idle_icon_id,
                                 menu_ctrl_ptr->win_handle);

        create_info.width = icon_width;
        create_info.height = icon_height;

        UILAYER_RELEASELAYER(&menu_ctrl_ptr->to_idle_layer_handle);/*lint !e774*/
        UILAYER_CreateLayer(&create_info, &menu_ctrl_ptr->to_idle_layer_handle);
        UILAYER_SetLayerColorKey(&menu_ctrl_ptr->to_idle_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        SlidePageMenuAppendMainMenuLayer(menu_ctrl_ptr->to_idle_layer_handle, UILAYER_LEVEL_HIGH);

        disp_rect = icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);

        LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &menu_ctrl_ptr->to_idle_layer_handle, &disp_rect, &disp_rect);

        IMG_EnableTransparentColor(TRUE);

        GUIRES_DisplayImg(PNULL,
                          &disp_rect,
                          &icon_rect,
                          menu_ctrl_ptr->win_handle,
                          menu_ctrl_ptr->main_menu_theme.to_idle_icon_id,
                          &menu_ctrl_ptr->to_idle_layer_handle);
    }

    is_enable_del = SlidePageMenuIsItemEnableDeleted(menu_ctrl_ptr);

    UILAYER_RELEASELAYER(&menu_ctrl_ptr->delete_layer_handle);/*lint !e774*/

    //动态图标支持删除，静态图标不支持删除功能
    if (is_enable_del)
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, menu_ctrl_ptr->main_menu_theme.delete_icon_id,menu_ctrl_ptr->win_handle);

        create_info.width = icon_width;
        create_info.height = icon_height;

        UILAYER_CreateLayer(&create_info, &menu_ctrl_ptr->delete_layer_handle);
        UILAYER_SetLayerColorKey(&menu_ctrl_ptr->delete_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        SlidePageMenuAppendMainMenuLayer(menu_ctrl_ptr->delete_layer_handle, UILAYER_LEVEL_HIGH);

        disp_rect = icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);

        LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &menu_ctrl_ptr->delete_layer_handle, &disp_rect, &disp_rect);

        GUIRES_DisplayImg(PNULL,
                          &disp_rect,
                          &icon_rect,
                          menu_ctrl_ptr->win_handle,
                          menu_ctrl_ptr->main_menu_theme.delete_icon_id,
                          &menu_ctrl_ptr->delete_layer_handle);
    }

    IMG_EnableTransparentColor(FALSE);

    return;
}
/*****************************************************************************/
//  Description : 获取组织功能时动画特效初始区域
//  Global resource dependence :
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void GetInitRect(
    MMIMAINMENU_SLIDEPAGE_DATA_T   *menu_ctrl_ptr,
    GUI_RECT_T *delete_rect_ptr,
    GUI_RECT_T *to_idle_rect_ptr,
    BOOLEAN is_need_delete,
    uint16 lcd_width,
    uint16 lcd_height
)
{
    uint16		i				= 0;
    uint16		item_width		= 0;

    uint16		margin			= 0;
    uint16		max_page_cnt	= 0;
    uint16		page_row_num	= 0;

    GUI_RECT_T	*item_rect_ptr = PNULL;
    GUI_RECT_T	menu_rect		= {0};
    GUI_RECT_T	item_rect2[MMITHEME_GOMENU_PAGE_ROW_NUM] = {0};
    BOOLEAN		is_get			= FALSE;

    if ((PNULL == menu_ctrl_ptr)||(PNULL == delete_rect_ptr)||(PNULL == to_idle_rect_ptr))
    {
        return;
    }

    menu_rect = MMITHEME_GetClientRectEx( menu_ctrl_ptr->win_handle);

    margin = MMITHEME_QBMENU_LEFT_RIGHT_MARGIN;

    page_row_num = menu_ctrl_ptr->slide_data.page_row_num;
    item_width = menu_ctrl_ptr->slide_data.item_rect[0].right
                 - menu_ctrl_ptr->slide_data.item_rect[0].left + 1;

    item_rect_ptr = &item_rect2; /*lint !e64*/

    menu_rect = SlidePageMenuAdjustSlideMenuRect(menu_ctrl_ptr);

    if (item_width > lcd_width || PNULL == item_rect_ptr) /*lint !e774*/
    {
        return;
    }
    else
    {

        menu_rect.left += margin;
        menu_rect.right -= margin;

        GUI_CreateMatrixRect(&menu_rect,
                             0,
                             0,
                             0,
                             0,
                             page_row_num,
                             1,
                             item_rect_ptr
                            );

//         for ( i = 0; i < page_row_num; i++ )/*lint !e681*/
//         {
//             item_rect_ptr[i] = GUI_GetCenterRect( item_rect_ptr[i], lcd_width, MENUICON_HEIGHT + MENUTEXT_HEIGHT );
//         }

        for (i = 0; i < page_row_num; i++)/*lint !e681*/
        {
            if (GUI_PointIsInRect(menu_ctrl_ptr->pre_tp_point, item_rect_ptr[i]))
            {
                is_get = TRUE;
                break;
            }
        }

        if (is_get)
        {
            to_idle_rect_ptr->left = lcd_width;
            to_idle_rect_ptr->right = lcd_width + item_width;

            if (0 == i)
            {
                item_rect_ptr[1] = GUI_GetCenterRect( item_rect_ptr[1], lcd_width, MENUICON_HEIGHT + MENUTEXT_HEIGHT ); /*lint !e661 !e415*/
                to_idle_rect_ptr->top = item_rect_ptr[1].top; /*lint !e661 !e415*/
                to_idle_rect_ptr->bottom = item_rect_ptr[1].bottom; /*lint !e661 !e415*/
            }
            else if (page_row_num == i)
            {
                uint16 rect_idx = 0;

                max_page_cnt = page_row_num;

                if (max_page_cnt > MMIMENU_DEFAULT_PAGE_CNT)/*lint !e774*/
                {
                    rect_idx = max_page_cnt - MMIMENU_DEFAULT_PAGE_CNT;
                }
                else
                {
                    rect_idx = 0;
                }
                item_rect_ptr[rect_idx] = GUI_GetCenterRect( item_rect_ptr[rect_idx], lcd_width, MENUICON_HEIGHT + MENUTEXT_HEIGHT );
                to_idle_rect_ptr->top = item_rect_ptr[rect_idx].top;
                to_idle_rect_ptr->bottom = item_rect_ptr[rect_idx].bottom;
            }
            else
            {
                item_rect_ptr[i-1] = GUI_GetCenterRect( item_rect_ptr[i-1], lcd_width, MENUICON_HEIGHT + MENUTEXT_HEIGHT );
                to_idle_rect_ptr->top = item_rect_ptr[i-1].top;
                to_idle_rect_ptr->bottom = item_rect_ptr[i-1].bottom;
            }

            if (is_need_delete)
            {
                delete_rect_ptr->left -= item_width;
                delete_rect_ptr->right = 0;
                delete_rect_ptr->top = to_idle_rect_ptr->top;
                delete_rect_ptr->bottom = to_idle_rect_ptr->bottom;
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Description : 长按特效
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuDisplaySpecialAnimation(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr,
    BOOLEAN			  is_disp_delete
)
{
    GUI_RECT_T to_idle_rect = {0};
    GUI_RECT_T delete_rect = {0};
    uint16 frame_num  = 10; // 移动频率
    uint16 cur_frame = 0; // 计数
    int16 move_space = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    UILAYER_INFO_T layer_info = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    // 获取图片的宽高
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, menu_ctrl_ptr->main_menu_theme.to_idle_icon_id, menu_ctrl_ptr->win_handle);

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    // 获取lcd 宽高
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    // 获得左右区域
    GetInitRect(menu_ctrl_ptr, &delete_rect, &to_idle_rect, is_disp_delete, lcd_width, lcd_height);

    UILAYER_GetLayerInfo(&menu_ctrl_ptr->to_idle_layer_handle, &layer_info);

    move_space = layer_info.layer_width;

    for (cur_frame = 1; cur_frame <= frame_num; cur_frame++)
    {
        int16 to_idle_move_space = 0;
        int16 delete_move_space = 0;
        int16 offset_y = to_idle_rect.top;
        int16 offset_x = 0;

        to_idle_move_space = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);

        offset_x = lcd_width - to_idle_move_space;

		if (0 != menu_ctrl_ptr->to_idle_layer_handle.block_id
			&& UILAYER_NULL_HANDLE != menu_ctrl_ptr->to_idle_layer_handle.block_id)
		{
            UILAYER_SetLayerPosition(&menu_ctrl_ptr->to_idle_layer_handle, offset_x, offset_y);
		}

        if (is_disp_delete)
        {
            UILAYER_INFO_T delete_layer_info = {0};

			UILAYER_GetLayerInfo(&menu_ctrl_ptr->delete_layer_handle, &delete_layer_info);

            delete_move_space = move_space - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);

            offset_x = (delete_layer_info.layer_width - icon_width) - delete_move_space;

            UILAYER_SetLayerPosition(&menu_ctrl_ptr->delete_layer_handle, offset_x, offset_y);
        }

        GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
#ifdef WIN32
        SCI_Sleep(100);
#endif
    }
}

/*****************************************************************************/
//  Description : init move layer
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SlideMenuInitMoveLayer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr  //in
)
{
    BOOLEAN			is_to_idle = FALSE;

    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

	//if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
	if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle() || MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
	{
        is_to_idle = TRUE;
	}

    if (UILAYER_IsMultiLayerEnable())
    {
        SlidePageMenuCreateOrigSpecialLayer(menu_ctrl_ptr, is_to_idle);
        SlidePageMenuCreateMoveIconLayer(menu_ctrl_ptr);
        SlideMenuCreateMoveBgLayer(menu_ctrl_ptr);
        SlidePageMenuDisplaySpecialAnimation(menu_ctrl_ptr, SlidePageMenuIsItemEnableDeleted(menu_ctrl_ptr));
    }

    // 记录移动的信息
    menu_ctrl_ptr->tmp_cur_item_index = menu_ctrl_ptr->cur_item_index;
    menu_ctrl_ptr->tmp_first_item_index = menu_ctrl_ptr->first_item_index;
    menu_ctrl_ptr->tmp_page_item_index = menu_ctrl_ptr->cur_page_index;

    SlidePageMenuStartShakeIconTimer(menu_ctrl_ptr);
}


/*****************************************************************************/
//  Description : 创建移动的buffer
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuMoveInit(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL != menu_ctrl_ptr)
    {
        result = TRUE;

        if (UILAYER_IsMultiLayerEnable())
        {
            if (menu_ctrl_ptr->is_item_pressed)
            {
                SlideMenuInitMoveLayer( menu_ctrl_ptr );
                SlidePageMenuResetSlidePageIconDisplay(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, TRUE);
            }
        }
        else
        {
            menu_ctrl_ptr->is_move_state = FALSE;
        }

    }

    return result;
}

/*****************************************************************************/
//  Description : 读取nv中存储的宫格菜单数据
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlidePageMenuReadItemList(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
    uint16 *item_num_ptr
)
{
    uint32 i = 0;
    uint32 item_num = 0;
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;
    MMI_LINK_NODE_T          *node_ptr = PNULL;

    item_list_ptr = MMIMAINMENU_GetItemList();

    if (PNULL == item_list_ptr)
    {
        return FALSE;
    }

    menu_ctrl_ptr->menu_list_info.item_count = item_list_ptr->item_count;
    node_ptr = item_list_ptr->list_head_ptr;

    for (i = 0; i < item_list_ptr->item_count; i++)
    {
        if (PNULL == node_ptr)
        {
            break;
        }
#ifdef QBTHEME_SUPPORT
        if ((ID_MAINMENU_QBTHEMEAPP != ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id)
                && (ID_MAINMENU_QBTHEMEMORE != ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id))
#endif
        {
            item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));

            SCI_MEMSET(item_info_ptr, 0, sizeof(MMIMAINMENU_ITEM_INFO_T));

            item_info_ptr->group_id = ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->group_id;
            item_info_ptr->menu_id = ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id;

            MMIMAINMENU_AddItemNode(&menu_ctrl_ptr->menu_list_info.list_head_ptr, (uint32)item_info_ptr);
            item_num++;
        }

        node_ptr = node_ptr->next_ptr;
    }

    if (PNULL != item_num_ptr)
    {
        *item_num_ptr = item_num;
    }

    return TRUE;
}
/*****************************************************************************/
// 	Description : 初始化链表
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void SlidePageMenuResetItemListInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
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
//  Description : 获取当前层信息
//  Global resource dependence :
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO SlidePageMenuGetCurrentPageLyaerInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);

    if (UILAYER_IsMultiLayerEnable()
            && UILAYER_IsLayerActive(&menu_ctrl_ptr->page_layer_handle[0]))
    {
        lcd_dev_info = menu_ctrl_ptr->page_layer_handle[0];
    }

    return lcd_dev_info;
}
/*****************************************************************************/
// 	Description : get item
//	Global resource dependence :
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL void SlidePageMenuGetItemInfo(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr,
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
//  Description : stop item text timer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartHighlightIconDispTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T     *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->highlight_timer_id)
    {
        menu_ctrl_ptr->highlight_timer_id = MMK_CreateWinTimer(
                                                menu_ctrl_ptr->ctrl_handle,
                                                GO_HIGHLIGHT_DISPLAY_TIMER_OUT,
                                                FALSE);
    }
}
/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopHighlightIconDispTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->highlight_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->highlight_timer_id);
        menu_ctrl_ptr->highlight_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStopRedrawTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->redrew_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->redrew_timer_id);
        menu_ctrl_ptr->redrew_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SlidePageMenuStartRedrawTimer(
    MMIMAINMENU_SLIDEPAGE_DATA_T *menu_ctrl_ptr
)
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->redrew_timer_id)
    {
        menu_ctrl_ptr->redrew_timer_id = MMK_CreateWinTimer(
                                             menu_ctrl_ptr->ctrl_handle,
                                             MMITHEME_MENU_REDRAW_TIME,
                                             FALSE );
    }
}
#endif

