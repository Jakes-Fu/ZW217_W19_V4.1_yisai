/*****************************************************************************
** File Name:      mmimenu_qbtheme.c                                       *
** Author:                                                                   *
** Date:           22/09/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011      jinwang              Creat
******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_menu_trc.h"
#include "guimenu.h"
#include "guires.h"
#include "window_parse.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_modu_main.h"
#include "mmidisplay_data.h"
#include "mmiset_display.h"
#include "guistring.h"
#include "mmi_image.h"
#include "ui_layer.h"
#include "mmi_link.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmk_tp.h"
#include "mmitheme_special.h"
#include "mmi_menutable.h"
#include "guiimg.h"
#include "mmiidle_export.h"
#include "img_dec_interface.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#endif
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
#include "mmi3d_export.h"
#endif
#include "graphics_draw.h"
#include "guitext.h"
#include "guictrl_api.h"
#include "mmipub.h"
#include "mmi_wallpaper_export.h"
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmk_app.h"
#include "u_launcher_export.h"
#include "graphics_3d_rotate.h"

#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_export.h"
#include "mmiudisk_export.h"

#include "mmi_mainmenu_export.h"
#include "mmi_mainmenu_qbtheme.h"
#include "mmi_mainmenu_internal.h"
#include "mmi_mainmenu_u_se.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//menu stack
typedef struct
{
    uint16              num_index;  //number index
    MMI_MESSAGE_ID_E    key_msg;    //number key message
} QBTHEME_NUMKEY_MAP_T;

typedef struct GO_MEM_PAGE_INFO{
    GUI_LCD_DEV_INFO layer_handle;
    uint8  index;//取值为0，1，2 始终以1为中心
    BOOLEAN is_change;
} QBTHEME_GO_MEM_PAGE_INFO_T;

//brief 菜单控件数据结构
typedef struct
{
    MMI_HANDLE_T                win_handle;                             //window handle
    MMI_HANDLE_T                ctrl_handle;

    MMITHEME_MAINMENU_COMMON_T  main_menu_theme;                        //main menu theme
    GUIMENU_QBTHEME_DATA_T      qbtheme_menu_data;                      //千变主题菜单数据以及排版 

    MMIMAINEMENU_PAGE_LIST_T    menu_page_info;                         //menu list information
    MMIMAINEMENU_ITEM_LIST_T    menu_bar_info;                          //menu bar information

    GUIMENU_STATE_T             menu_state;

    uint16                      cur_page_index;                         //default 0,from 0 to max-1
    uint16                      cur_item_index;                         //current selected menu index. in option page style, it is also item index
    uint16                      first_item_index;                       //item index at the top
    uint16                      pre_cur_item_index;                     //previous current index
    uint16                      page_total_num;                         //TODO: total_page_num;

    BOOLEAN                     is_active;                              //
    BOOLEAN                     is_delete_state;                        //item是否在抖动
    BOOLEAN                     is_move_state;                          //item是否在抖动
    BOOLEAN                     is_moving;                              //item是否被拖动
    BOOLEAN                     is_empty_space;                         //tp up时，是否在item上
    BOOLEAN                     is_item_pressed;                        //tp down时，是否在item上
    BOOLEAN                     is_tp_pressed;                          //is tp down?
    BOOLEAN                     is_bar_item;                            //is bar item

    uint16                      tmp_cur_item_index;                     //移动的时候使用
    uint16                      tmp_first_item_index;                   //移动的时候使用
    uint16                      tmp_page_item_index;                    //移动到时候使用
    BOOLEAN                     tmp_is_bar_item;                        //移动的时候使用
    uint16                      move_item_index;                        //移动的item项，用于给idle和删除用

    uint16                      tmp_width;                              //移动的时候使用
    uint16                      tmp_height;                             //移动的时候使用
    
    CONTINUE_MOVE_TYPE_E 	    continue_move_type;	                    // 连续移动的类型
    uint8                       tp_timer_id;				            // 为支持TP连续移动而加的timer
    uint8                       redrew_timer_id;				        // 滑动时，slide和fling状态下，画图的timer
    uint8                       start_move_timer_id;				    // 进入抖动状态的定时器
    uint8                       shake_icon_timer_id;				    // 进入抖动状态时，每次抖动的间隔
    uint8                       user_timer_id;				            // 应用自定义的timer
    uint8                       highlight_timer_id;				        // 

    BOOLEAN                     is_display_item_layer;                  // 是否画item layer的数据

    GUI_LCD_DEV_INFO            highlight_layer_handle;                 //高亮层
    GUI_LCD_DEV_INFO            icon_move_layer_handle;                 //图标移动层
	GUI_LCD_DEV_INFO			to_idle_layer_handle;                   //到主菜单图层

    MMK_TP_SLIDE_E              slide_state;                            //current slide state
    MMI_MENU_ID_T               delete_menu_id;                         //组织状态下使用,要删除的menu_id
    GUI_POINT_T                 slide_start_point;                      //Slide时候使用
    GUI_POINT_T                 slide_pre_point;                        //Slide时候使用
    GUI_POINT_T                 pre_tp_point;                           //上一次tp move的坐标
    GUI_LCD_DEV_INFO            page_layer_handle[GUIMENU_SLIDE_PAGE_NUM]; //页面移动层1
    GUI_LCD_DEV_INFO            titleidx_layer_handle;                  // 页面索引层
    // end

    float                       fling_velocity;                         //滑动速度
    GUIMENU_GO_SLIDE_TYPE_E      support_slide_type;                    //主菜单支持的滑动类型
    GUI_LCD_DEV_INFO            layer_bg; 

    GUIMENU_ENTER_ANIM_TYPE_E   enter_anim_type;                        // 进入时的动画类型
    uint8  init_item_timer_id;
    GUI_LCD_DEV_INFO			delete_layer_handle;                    //删除图层
// 以下应该用控件的数据
    GUI_LCD_DEV_INFO            lcd_dev_info;                           // 页面索引层
    GUI_RECT_T                  rect;
} MMIMAINMENU_QBTHEME_DATA_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL GUI_RECT_T s_left_direct_move_rect = {0};
LOCAL GUI_RECT_T s_right_direct_move_rect = {0};
LOCAL uint16     s_undraw_index  = 0xFFFF;
LOCAL uint16     s_cur_page_index = 0;

LOCAL QBTHEME_NUMKEY_MAP_T  s_guimenu_numkey_map[] = {
                                                      0, MSG_APP_1,
                                                      1, MSG_APP_2,
                                                      2, MSG_APP_3,
                                                      3, MSG_APP_4,
                                                      4, MSG_APP_5,
                                                      5, MSG_APP_6,
                                                      6, MSG_APP_7,
                                                      7, MSG_APP_8,
                                                      8, MSG_APP_9,
                                                      9, MSG_APP_STAR,
                                                      10,MSG_APP_0,
                                                      11,MSG_APP_HASH
                                                      };

LOCAL GO_GUIMENU_ITEM_BUF_T s_slidemenu_item_layer[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM][MMITHEME_GOMENU_ITEM_MAX_NUM] = {0};

LOCAL GO_MAINMENU_PAGE_INFO_T *s_cur_page_info_ptr = PNULL;
LOCAL GO_MAINMENU_PAGE_INFO_T *s_next_page_info_ptr = PNULL;
LOCAL FIX16_POINT_T* s_calc_point_buf = PNULL;

//LOCAL BOOLEAN s_go_is_initialized = FALSE;
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
LOCAL MMIMENU_SELECT_EFFECT_INFO_T s_mmimenu_select_effect_info = {0};
#endif

LOCAL QBTHEME_GO_MEM_PAGE_INFO_T s_go_item_page_info[GO_MEM_PAGE_NUM] = {0};

LOCAL uint8* s_go_alloc_mem_ptr = PNULL;

LOCAL S3D_EXPANDPTR_T* s_go_calc_param_ptr = PNULL;
 
//大小为SHINING_MAX_RADIUS + 1的shining ctrl buf
LOCAL uint8* s_go_shining_ctrl_ptr = PNULL;

LOCAL uint32 s_random_cur_style = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Matrix Init
//  Global resource dependence :
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL void *QbThemeMenuInit(
    MMI_HANDLE_T ctrl_handle
);

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void InitQbThemeMenu(
                          MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                          );

/*****************************************************************************/
// Description : 销毁单，释放内存
// Global resource dependence : 
//  Author:jin.wang
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestoryQbThemeMenu(
                             void* data_ptr
                             );

/*****************************************************************************/
// 	Description : handle menu msg function
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E QbThemeHandleMsg(
                                 void               *ctrl_ptr,  //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,  	//message
                                 DPARAM             param       //add data
                                 );

/*****************************************************************************/
// Description : 初始化链表
// Global resource dependence : 
//  Author:jin.wang
// Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void QbThemeDestroyPageList(
                                   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                   );

/*****************************************************************************/
//  Description : 读取DockBar的数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeReadDockbarInfo(
                                     MMIMAINMENU_PAGE_ITEM_INFO_T *dockbar_item_ptr
                                     );
#ifdef MMI_SAM_THEME
/*****************************************************************************/
//  Description : 读取三星的dockbar数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSamThemeDockbar(
                                 MMIMAINMENU_PAGE_ITEM_INFO_T *dockbar_item_ptr
                                 );
#endif
/*****************************************************************************/
//  Description : 生成菜单数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
//      1 读取nv中存储的静态数据
//      2 读取U盘、T卡中的动态数据
/*****************************************************************************/
LOCAL BOOLEAN ReadQbThemeMenuPageItemInfo(
                                         MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                         MMIMAINMENU_PAGE_ITEM_INFO_T *item_page_ptr,
                                         uint16                       *page_num_ptr
                                         );

/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeResetBarIndexRect(
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
    GUIMENU_QBTHEME_DATA_T      *menu_info_ptr, //in/out:
    uint16                      item_num
);

/*****************************************************************************/
//  Description : reset
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ResetQbThemeMenuAutoRect(
                                  MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                  );

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void InitQbThemeMenuGoInfo(
								 MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
								 );

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMoveBgLayer(
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
);

/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO DisplayOneItem(
                                      MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                                      uint16                        page_index,
                                      uint16                        item_index,
                                      GUIMENU_ITEM_POSITION_E       position
                                      );

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO DisplayOneItemToLayer(
                                             MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                             const GUI_LCD_DEV_INFO     *dst_dev_ptr,
                                             GUI_RECT_T                 *rect_ptr,
                                             uint16                     page_index,
                                             uint16                     item_index,
                                             GUIMENU_ITEM_POSITION_E    position
                                             );

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeMenu(
							  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
							  );

/*****************************************************************************/
//  Description : 画bar上的item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeBarItem(
								 MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
								 );

/*****************************************************************************/
//  Description : 画bar上的item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeBar(
                            MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
                            uint16                      cur_page_index,
                            GUI_LCD_DEV_INFO            lcd_dev_info,
                            BOOLEAN                     is_draw_all
                            );

/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeItem(
                             MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
                             uint16                         page_index,
                             GUI_LCD_DEV_INFO               lcd_dev_info
                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeActiveIcon(
									MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
									BOOLEAN                     is_display_highlight
									);

/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence : 
//  Author: jin.wang
//  Note: 处理抖动timer
/*****************************************************************************/
LOCAL void DisplayQbThemeSpecialMenu(
                                   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                   );

/*****************************************************************************/
//  Description : blt bar layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void BltQbThemeMenuBarLayer(
								  MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
								  );

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void BltPageItemLayerToMenuLayer(
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
    uint16                      page_index,
    GUI_LCD_DEV_INFO            lcd_dev_info,
    int16                       offset_x,
    BOOLEAN                     is_need_bar,
    BOOLEAN                     is_need_page
);

/*****************************************************************************/
//  Description : create highlight layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuHighlightLayer(
										   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
										   );

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuHighlightLayer(
                                          MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                          );

/*****************************************************************************/
//  Description : create bar layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuIndexBarLayer(
                                    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                    );

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuIndexBarLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                     );

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveIconLayer(
                               MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                               );

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMoveIconLayer(
                                      MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                      );

/*****************************************************************************/
//  Description : create move bg layer
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveBgLayer(
							 MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
							 );

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuPageLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                     );

/*****************************************************************************/
//  Description : get page rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetPageRect(
                             MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                             );

/*****************************************************************************/
// Description : get icon text rect
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void GetIconTextRect(
                           GUI_RECT_T  item_rect,
                           GUI_RECT_T* icon_rect,
                           GUI_RECT_T* text_rect
                           );

/*****************************************************************************/
//  Description : get cur item rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCurItemRect(
                             MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,  //in
                             GUI_RECT_T                 *rect_ptr        //out
                             );

/*****************************************************************************/
//  Description : get pubwin button rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note: 
/*****************************************************************************/
LOCAL void GetDockBarItemRect(
                          MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                          GUI_RECT_T                    rect_ptr[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX],
                          uint32                        num
                          );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetQbThemeMenuMoveLayerRect(
											 MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
											 );

/*****************************************************************************/
//  Description : is point in icon rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPointInIconRect(
                                MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
                                GUI_POINT_T                 point,
                                GUI_RECT_T                  rect
                                );

/*****************************************************************************/
// Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void GetItemPtr(
					  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
					  uint16                        page_index,
					  uint16                        item_index,
					  CTRLMAINMENU_ITEM_T           *menuitem_ptr
					  );

/*****************************************************************************/
// Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *GetItemInfoPtr(
                                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                                              uint16                        page_index,
                                              uint16                        item_index
                                              );

/*****************************************************************************/
// Description : 比较menu id，如果存在返回TRUE，如果不存在返回FALSE
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchMenuID(
                           MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                           MMI_MENU_ID_T                menu_id
                           );

/*****************************************************************************/
// Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *GetQbThemeMenuBarHeadPtr(
                                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                                  );

/*****************************************************************************/
//  Description : 是否显示删除菜单图标
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbMenuIsItemEnableDeleted(
										MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
										);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jin.wang
//  Note: 
/*****************************************************************************/
LOCAL void SaveQbThemeMenu(
                          MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                          BOOLEAN                       is_del_empty // 保存时是否删除空页
                          );

/*****************************************************************************/
//  Description : 保存菜单数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveQbThemeMenuPageItemNum(
                                       MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                       );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jin.wang
//  Note: 
/*****************************************************************************/
LOCAL void ValidateQbThemeMenuCurPage(
                                    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                    );

/*****************************************************************************/
//  Description : 获取一页显示的最大item number
//  Global resource dependence : 
//  Author: jin.wang
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetQbThemeMenuInPageMaxItemNum(
                                           MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                                           );

/*****************************************************************************/
//  Description : 获得一页的item number
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetQbThemeMenuPageTotalItemNum(
											MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
											uint16                      page_index
											);

/*****************************************************************************/
//  Description : 获得bar的item number
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetQbThemeMenuBarItemNum(
                                     MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                     );

/*****************************************************************************/
//  Description : 去掉主菜单 不显示的项
//  Global resource dependence :
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T* GetNextShownItem(
    MMI_LINK_NODE_T *cur_node_ptr
);

/*****************************************************************************/
//  Description : 设置item
//  Global resource dependence :
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL uint16 GetValidItemInfo(
							  MMIMAINMENU_ITEM_INFO_T *item_page_ptr
							  );

/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetQbThemeMenuMoveItemInfo(
                                       MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                       );

/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetQbThemeMenuMoveItemInfo(
                                         MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                         );

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void QbThemeMenuModifyRect(
                                MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                );

/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence : 
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetCurrentPageLyaerInfo(
                                               MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                               );

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void StartItemExchangeTimer(
                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                  );

/*****************************************************************************/
//  Description : stop item timer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void StopItemExchangeTimer(
                                 MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                 );

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStartShakeIconTimer(
                                MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                );

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStopShakeIconTimer(
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                              );

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStartMoveTimer(
                               MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                               );

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStopMoveTimer(
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                              );

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStopRedrawTimer(
                           MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                           );

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStartRedrawTimer(
                            MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                            );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeTimer(
									  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
									  MMI_MESSAGE_ID_E              msg_id,  //事件类型
									  DPARAM                        param          //附带的处理参数
									  );

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeMenuCancelKey(
                                        MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                        );

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidItemIndex(
                               uint16                       num_index,
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                               );

/*****************************************************************************/
//  Description : handle page touch panel press down msg
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeTpDown(
                              GUI_POINT_T                   *tp_point_ptr,
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                              );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeAutoMoveEvent(
                                        GUI_POINT_T                 *point_ptr, 
                                        MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                        );

/*****************************************************************************/
//  Description : handle icon menu ok
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeMenuMove(
                                   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL GUIMENU_ITEM_BUF_T DeleteQbThemeMenuItemLayer(
                                                   MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr, 
                                                   uint16                       page_index, 
                                                   uint16                       item_index,
                                                   BOOLEAN                      is_bar_item
                                                   );

/*****************************************************************************/
//  Description : handle item exchange
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeItemExchange(
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                              );

/*****************************************************************************/
//  Description : handle tp move message
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeMenuTpMove(
                                  GUI_POINT_T                   *point_ptr, 
                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                  );

/*****************************************************************************/
//  Description : handle slide menu touch panel press up msg
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeTpUp(
                               GUI_POINT_T                  *tp_point_ptr,
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                               );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeMenuGrayEvent(
                                      MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr, 
                                      MMI_MESSAGE_ID_E              msg_id,
                                      DPARAM                        param
                                      );

/*****************************************************************************/
//  Description : 确认移动
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConfirmQbThemeMenuItem(
                                    MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr,
                                    MMI_MESSAGE_ID_E                msg_id
                                    );

/*****************************************************************************/
//  Description : init move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void QbMenuInitMoveLayer(
							   MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr  //in
							   );

/*****************************************************************************/
//  Description : 创建移动的buffer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeMenuMoveInit(
                                 MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr
                                 );
#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
// Description: 整理item数据，防止出现异常
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void TrimQbThemePageInfo(
    MMIMAINMENU_PAGE_ITEM_INFO_T *item_bar_ptr
);

/*****************************************************************************/
// Description:  删除不合法的动态菜单
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void DeleteInvalidDyItem(
    MMIMAINMENU_ITEM_INFO_T* item_info_ptr,
    uint16 item_cnt,
    uint16 delete_idx
);

/*****************************************************************************/
//  Description : 重新加载主菜单
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ReloadItemToMainMenu(
    MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr
    );
#endif

/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void OpenQbThemeSetMenuWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//   Author: jin.wang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeSetWinMsg(
                                          MMI_WIN_ID_T win_id,     
                                          MMI_MESSAGE_ID_E msg_id,     
                                          DPARAM param
                                          );

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void InsertMenuNodeByLabel(
                 MMI_CTRL_ID_T      ctrl_id,        //控件id
                 MMI_IMAGE_ID_T     image_id,       //图片id, for PDA style pop menu
                 MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                 uint16             node_id,        //节点编号
                 uint16             parent_node_id, //父节点编号
                 uint16             index           //位置
                 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//   Author: jin.wang
//  Note:   
/*****************************************************************************/
LOCAL void  SetIldeStyle(
    MMI_CTRL_ID_T ctrl_id,
    MMI_WIN_ID_T win_id
);

/*****************************************************************************/
//  Description : check display init ok?
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _IsInitItemFinished(
								 MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
								 );

/*****************************************************************************/
//  Description : get item
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeMenuGetItemInfo(
    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
    GUIMENU_ITEM_POSITION_E         position,
    uint16 page_index,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *item_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN QbThemeMenuRunItem(
    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : HandleMenuOkKey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeHandleOkKey(
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                              MMI_MESSAGE_ID_E  msg_id
                              );

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyParentMsg(
                                   CAF_HANDLE_T     ctrl_handle,
                                   MMI_MESSAGE_ID_E msg_id
                                   );

/*****************************************************************************/
//  Description : handle menu cancel key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeHandleCancelKey(
                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                                  MMI_MESSAGE_ID_E  msg_id
                                  );

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeHandleNumKey(
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                               MMI_MESSAGE_ID_E msg_id
                               );

/*****************************************************************************/
//  Description : set state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetState( 
                    uint32* dst_state_ptr,
                    uint32  src_state,
                    BOOLEAN is_true
                    );

WINDOW_TABLE(MMIMENU_QBTHEME_SETSTYLE_TAB2) = 
{
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
    WIN_FUNC( (uint32)HandleQbThemeSetWinMsg ),    
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_ID(MMIMENU_QBTHEME_SETSTYLE_WIN_ID),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL void DisplayHighlightIconLayer(
    GUI_LCD_DEV_INFO    *highlight_layer_ptr
);

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuBgLayer(
                                 MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                 );

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuPageBgLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                     );

/*****************************************************************************/
//  Description : 算法输出BUF
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuCalcDstLayer(
                                          MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                                          GUI_RECT_T       rect
                                          );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuCalcDstLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                     );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeItemLayer(
                                  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                  );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DestoryQbThemeMenuItemLayer(void);


/*****************************************************************************/
//  Description :申请PAGE信息存放BUF
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbTheme_GO_CreatMemory(
                                      MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                      );

/*****************************************************************************/
//  Description :申请PAGE信息存放BUF
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbTheme_GO_ReleaseMemory(void);

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemePageMainLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                     );

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeBg(
                              GUI_RECT_T        bg_rect,
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                              GUI_LCD_DEV_INFO  *lcd_dev_info_ptr
                              );

/*****************************************************************************/
//  Description : 合并层，由于最多只有两个Page合并到主层上，因此只需要两个层的参数
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetBltQbThemeMenuPageLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                     UILAYER_HANDLE_T layer1,
                                     UILAYER_HANDLE_T layer2,
                                     UILAYER_HANDLE_T layer3
                                     );

/*****************************************************************************/
//  Description : remove all menu layer
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void RemoveAllQbThemeMenuLayer(
                                   MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void CreateItemLayer(
                          MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                          uint16 icon_width,
                          uint16 icon_height,
                          uint16 item_total_num,
                          GUI_LCD_DEV_INFO *layer_handle
                          );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckLayerMem( 
                           MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                           );

/*****************************************************************************/
//  Description : CheckQbThemeLayerMem
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckQbThemeLayerMem( 
                                   MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                   );

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayItemLayer(
                           MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                           );

/*****************************************************************************/
//  Description :绘制并刷新函数
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void DrawUpdateGOMenuLayer(
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                              uint16 first_page,
                              uint16 second_page,
                              GO_MOVE_DIRECTION_E direction,
                              uint16 current_frame,
                              BOOLEAN is_display_highlight
                              );

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : stop item text timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StartHighlightIconDispTimer(
                                        MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                        );

/*****************************************************************************/
//  Description : stop Highlight timer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopHighlightIconDispTimer(
                                   MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                   );
#endif

/*****************************************************************************/
//  Description : StartInitItemTimer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StartInitItemTimer(
    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
);

/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemePageItem(
                                MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
                                uint16             page_index,
                                GUI_LCD_DEV_INFO   lcd_dev_info
                                );

/*****************************************************************************/
//  Description : up date rect 
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: peilong.zhou
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStoreUpdateRect(
    MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
);

/*****************************************************************************/
//  Description :绘制函数
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void DrawGOMenuLayer(
                           MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                           uint16 first_page,
                           uint16 second_page,
                           GO_MOVE_DIRECTION_E direction,
                           uint16 current_frame,
                           BOOLEAN is_display_highlight
                           );

/*****************************************************************************/
//  Description :PAF 边界处理函数
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Go_Effect_Zero_Type_Calc(
    GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
    IMGREF_SIZE_T bg_window_size,//屏幕尺寸
    uint32* bg_window_buf,//背景buffer
    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
    uint32 *select_image_buf,
    IMGREF_SIZE_T highlight_icon_size,
    BOOLEAN is_display_highlight,
    uint16 page_index,
    GO_CALC_DATA_INFO_T* calc_data_info
);

/*****************************************************************************/
//  Description : 获取高亮区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetQbThemeHighlightRect(
                                        MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                        int16 item_index_inpage
                                        );
/*****************************************************************************/
//  Description :获取GO桌面需要的页面信息
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void GO_GetPageInfo(
    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
    uint16 page_index,
    GO_MAINMENU_PAGE_INFO_T *page_info_ptr
);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void InitQbThemeItemData(
                              MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                              );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GO_InitItemLayerInfo(
                                   MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                   uint16 page_index,
                                   GUI_LCD_DEV_INFO *layer_handle
                                   );

/*****************************************************************************/
//  Description : 显示高亮动画
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightAnim(
    uint8* target_buf,
    IMGREF_SIZE_T target_window_size,
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
);

/*****************************************************************************/
//  Description : 根据Item索引号获得其在页面中的位置索引
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 如果不在页内,返回-1
/*****************************************************************************/
LOCAL int16 GetQbThemeMenuPageIndex(
                                   MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                   uint16           item_index
                                   );

/*****************************************************************************/
//  Description : 当前的Item是否在页中显示,因为当触笔移动滚动条的时候,
//                  当前Item可能会不在页面中显示
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemInQbThemeMenuPage(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                     uint16           item_index,
                                     uint16           page_index
                                     );

/*****************************************************************************/
//  Description : CreateHighlightLayerEx
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateHighlightLayerByID(
    MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
    MMI_IMAGE_ID_T image_id,
    GUI_LCD_DEV_INFO *layer_handle_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL void MoveGOMenuPageLayer_X(
                                  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                  GUI_POINT_T*    point_ptr
                                  );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideSecondPage_X(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                GUI_POINT_T*    point_ptr,
                                BOOLEAN        *is_second_move
                               );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL void MoveGOMenuPageLayer_Y(
                                  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                  GUI_POINT_T*    point_ptr
                                  );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideSecondPage_Y(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                GUI_POINT_T*    point_ptr,
                                BOOLEAN        *is_second_move
                               );

/*****************************************************************************/
//  Description : x方向
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN XHandleGOMenuSlideOver(
                                    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                    GUI_POINT_T      *tp_point_ptr
                                    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GOMenuSlideOverDraw(
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr, 
                               uint16              pre_page_index,
                               uint16              second_page,
                               GO_MOVE_DIRECTION_E direction,
                               uint16              current_frame,
                               BOOLEAN             is_change_page
                               );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void GO_UpdateItemLayer(
                              MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                              BOOLEAN is_tpmove_right
                              );

/*****************************************************************************/
//  Description : handle slide page touch panel press up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSlideCurPage_X(
                              GUI_POINT_T      *tp_point_ptr,
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                              );

/*****************************************************************************/
//  Description : handle slide page touch panel press up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSlideCurPage_Y(
                              GUI_POINT_T      *tp_point_ptr,
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                              );

/*****************************************************************************/
//  Description : x方向
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN YHandleGOMenuSlideOver(
                                    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                    GUI_POINT_T      *tp_point_ptr
                                    );

/*****************************************************************************/
//  Description : set move layer position
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SetMoveLayerPosition(
                                 MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                 );

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void AppendMainMenuLayer(
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                               GUI_LCD_DEV_INFO lcd_dev_info,
                               UILAYER_LEVEL_E layer_level
                               );

/*****************************************************************************/
// Description:  是否拖动到主菜单/删除
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInToIdleRect(
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                              BOOLEAN is_handle_tp,
                              BOOLEAN *is_need_redraw_ptr
                              );

#ifdef DYNAMIC_MAINMENU_SUPPORT

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
                                            );

/*****************************************************************************/
// Description : 处理tpup消息
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void OpenQueryDeleteWin(void);

#endif

/*****************************************************************************/
//  Description : 显示delitem bar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayDelItemBar(
                                 MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                 );

/*****************************************************************************/
//  Description : 显示toidle bar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayToIdleBar(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                );

/*****************************************************************************/
//  Description : 判断tp是否在删除区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInDeleteitemRect(
                                    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                    );

/*****************************************************************************/
//  Description : 判断tp是否在toidle区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInToidleRect(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                );

/*****************************************************************************/
//  Description : create qbtheme icon layer
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void CreateOrigSpecialLayer(
                                  MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                                  BOOLEAN            is_to_idle
                                  );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void ReleaseOrigSpecialLayer(
                                    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                    );

/*****************************************************************************/
//  Description : 长按特效
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplaySpecialAnimation(
                                MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
                                BOOLEAN              is_disp_delete
                                );

/*****************************************************************************/
//  Description : 获取组织功能时动画特效初始区域
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void GetInitRect(
                       MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                       GUI_RECT_T *delete_rect_ptr,
                       GUI_RECT_T *to_idle_rect_ptr,
                       BOOLEAN is_need_delete,
                       uint16 lcd_width,
                       uint16 lcd_height
                       );

/*****************************************************************************/
// Description : get item info in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *GetMenuCurrentItemInfoPtr(
                                                         MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                                         );

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : 设置点击特效类型INFO
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetSelectEffectStyleInfo(
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style
);
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void GO_Change_handle_and_index(
    QBTHEME_GO_MEM_PAGE_INFO_T* page_array_info,
    BOOLEAN is_tpmove_right
);

/*****************************************************************************/
//  Description : 设置选中随机
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetRandomSelectEffectStyleInfo(void);

/*****************************************************************************/
//  Description : get slide client rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T AdjustSlideMenuRect(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                                     );
/*****************************************************************************/
//     Description : 销毁单，释放内存
//    Global resource dependence : 
//  Author:xiaoqing.lu
//    Note: 
/*****************************************************************************/
LOCAL void DestoryGOMenu(
                      MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                      );


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 注册千变主题菜单项的信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_RegisterQbTheme(
                                          CTRLMAINMENU_PM_APIS_T *apis
                                          )
{
    if (PNULL == apis)
    {
        return FALSE;
    }

    apis->Initialize = QbThemeMenuInit;
    apis->Destory = DestoryQbThemeMenu;
    apis->Handle = QbThemeHandleMsg;

    return TRUE;
}

/*****************************************************************************/
//  Description : Matrix Init
//  Global resource dependence :
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL void *QbThemeMenuInit(
    MMI_HANDLE_T ctrl_handle
)
{
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr = PNULL;

    qbtheme_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_QBTHEME_DATA_T));
    if (PNULL == qbtheme_ptr)
    {
        SCI_TRACE_LOW("QbThemeMenuInit Faild");
        return PNULL;
    }
    SCI_MEMSET(qbtheme_ptr, 0, sizeof(MMIMAINMENU_QBTHEME_DATA_T));

    qbtheme_ptr->ctrl_handle = ctrl_handle;
    qbtheme_ptr->win_handle = MMK_GetWinHandleByCtrl(ctrl_handle);

#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
    //默认支持滑动的方向为X，其他另行处理
    qbtheme_ptr->support_slide_type = GO_SLIDE_SUPPORT_ONLY_X;//默认为X方向 
#endif

    MMITHEME_GetMainMenuTheme(&qbtheme_ptr->main_menu_theme);
    MMITHEME_GetQbthemeMenuInfo(qbtheme_ptr->win_handle, &qbtheme_ptr->qbtheme_menu_data);

    InitQbThemeMenu(qbtheme_ptr);

    qbtheme_ptr->cur_item_index = qbtheme_ptr->main_menu_theme.default_item_index;

    return (void *)qbtheme_ptr;
}

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void InitQbThemeMenu(
                          MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                          )
{
    uint16 page_index = 0;
    uint16 page_num = 0;
    MMIMAINMENU_PAGE_ITEM_INFO_T page_item_info[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM] = {0};
    MMIMAINMENU_PAGE_ITEM_INFO_T bar_item_info = {0};
    
    qbtheme_ptr->is_move_state = FALSE;

    qbtheme_ptr->qbtheme_menu_data.is_disp_anim = TRUE;

    qbtheme_ptr->main_menu_theme.is_used_org = TRUE;

	qbtheme_ptr->support_slide_type = UAPIMenu_GetCalcStyleSlideType(MMIAPISET_GetGOmenuStyle());

    // 释放item链表
    QbThemeDestroyPageList(qbtheme_ptr);

    // 读取页面
    ReadQbThemeMenuPageItemInfo(qbtheme_ptr, page_item_info, &page_num);
    qbtheme_ptr->page_total_num = page_num;

    for (page_index = 0; page_index < qbtheme_ptr->page_total_num; page_index++)
    {
        MMIMAINEMENU_ITEM_LIST_T    *new_item_list_ptr = PNULL;

        new_item_list_ptr = MMIMENU_AddOnePage(&qbtheme_ptr->menu_page_info);

        MMIMENU_AddItemInPage(
            new_item_list_ptr,
            page_item_info[page_index].menu_item_info,
            page_item_info[page_index].item_num);
    }

    // 读取dockbar信息
    QbThemeReadDockbarInfo(&bar_item_info);
    MMIMENU_AddItemInPage(
        &qbtheme_ptr->menu_bar_info,
        bar_item_info.menu_item_info,
        bar_item_info.item_num);

    //// 删除空页
    //DeleteQbThemeMenuEmptyPage(qbtheme_ptr);
    
    //if (page_num != qbtheme_ptr->page_total_num) // 当两者不等时，重新保存页数.此时说明已经删除了
    //{
    //    // 重新赋值给page num，因为第一次初始化的时候，可能会改变nv中存储的page num
    //    page_num = qbtheme_ptr->page_total_num;
    //}

    // 设置索引指示栏的位置
    QbThemeResetBarIndexRect(qbtheme_ptr, &qbtheme_ptr->qbtheme_menu_data, page_num);

    // 读NV中的当前页数值
    qbtheme_ptr->cur_page_index = s_cur_page_index;
    ValidateQbThemeMenuCurPage(qbtheme_ptr);

    //创建高亮图层
    CreateQbThemeMenuHighlightLayer(qbtheme_ptr);

    // 创建bar显示层
    CreateQbThemeMenuIndexBarLayer(qbtheme_ptr);

    // 设置自动滑动的区域
    ResetQbThemeMenuAutoRect(qbtheme_ptr);

	// 初始化go开始
	InitQbThemeMenuGoInfo(qbtheme_ptr);

	GUIWIN_SetStbState(qbtheme_ptr->win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
    GUIWIN_SetStbBgIsDesplay(qbtheme_ptr->win_handle,TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR    
	GUIWIN_SetStbBgImageId(qbtheme_ptr->win_handle, IMAGE_IDLE_STATUS_BAR);
#endif
}

/*****************************************************************************/
// Description : 销毁单，释放内存
// Global resource dependence : 
//  Author:jin.wang
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestoryQbThemeMenu(
                             void* data_ptr
                             )
{
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr = (MMIMAINMENU_QBTHEME_DATA_T*)data_ptr;

    qbtheme_ptr->is_move_state = FALSE;

    //// 保存当前页索引
    s_cur_page_index = qbtheme_ptr->cur_page_index;

    // 释放item数据链表
    QbThemeDestroyPageList(qbtheme_ptr);

    // 释放高亮图层
    ReleaseQbThemeMenuHighlightLayer(qbtheme_ptr);

    // 释放索引指示栏
    ReleaseQbThemeMenuIndexBarLayer(qbtheme_ptr);

    // 释放PageLayer
    ReleaseQbThemeMenuPageLayer(qbtheme_ptr);

    ReleaseQbThemeMoveIconLayer(qbtheme_ptr);

//#ifdef DYNAMIC_MAINMENU_SUPPORT
//    //释放动态菜单内存
//    MMIMENU_DyFreeData();
//#endif

    // 释放GO菜单
	DestoryGOMenu(qbtheme_ptr);
	//_MMIMENU_GO_SetInitialized(FALSE); // 设置初始化状态

    SCI_FREE(qbtheme_ptr);

    return TRUE;
}

/*****************************************************************************/
// 	Description : handle menu msg function
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E QbThemeHandleMsg(
                                 void               *ctrl_ptr,  //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,  	//message
                                 DPARAM             param       //add data
                                 )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_POINT_T         tp_point = {0};
    MMIMAINMENU_PAGE_ITEM_INFO_T bar_item_info = {0};
    MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr = (MMIMAINMENU_QBTHEME_DATA_T*)ctrl_ptr;

    if (PNULL == qbtheme_ptr)
    {
        return MMI_RESULT_FALSE;
    }

	if (!_IsInitItemFinished(qbtheme_ptr) && msg_id != MSG_TIMER)
	{
		return MMI_RESULT_FALSE;
	}

// @CR NEWMS00220875 全键盘下Menu对数字键没有响应
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
	msg_id = MMIAPICOM_ConvertKeyMsg(msg_id);
#endif
//end

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        SetState(&qbtheme_ptr->menu_state, GUIMENU_STATE_ENTER, TRUE);

        // 由于主菜单与idle界面消息先后顺序问题，在这里重新加载dockbar
        MMIMENU_DestroyItemList(&qbtheme_ptr->menu_bar_info);
        QbThemeReadDockbarInfo(&bar_item_info);
        MMIMENU_AddItemInPage(
            &qbtheme_ptr->menu_bar_info,
            bar_item_info.menu_item_info,
            bar_item_info.item_num);
        break;

    case MSG_CTL_PAINT:
        //display menu
        DisplayQbThemeMenu(qbtheme_ptr);

        SetState(&qbtheme_ptr->menu_state, GUIMENU_STATE_ENTER, FALSE);

#ifdef EFFECT_ZOOM_SUPPORT
        if (qbtheme_ptr->enter_anim_type > GUIMENU_ENTER_ANIM_NONE
            && qbtheme_ptr->enter_anim_type < GUIMENU_ENTER_ANIM_MAX)
        {
            MMIDEFAULT_SetCurWin3DEffect(MMI_WIN_ANIM_ZOOM_IN);
        }
#endif

        //// TODO:
        //HandleTextToSpeech(qbtheme_ptr);
        break;

    case MSG_CTL_GET_FOCUS:
        //// 初始值
        //qbtheme_ptr->pre_msg_loop_page_index = (uint16)-1;
        //qbtheme_ptr->pre_msg_loop_item_index = (uint16)-1;
        //qbtheme_ptr->pre_msg_loop_menu_level = (uint16)-1;

        if (qbtheme_ptr->is_move_state)
        {
            QbThemeStartShakeIconTimer(qbtheme_ptr);
        }

//#ifdef UI_MULTILAYER_SUPPORT
//#ifdef DYNAMIC_MAINMENU_SUPPORT
//        // 如果支持多层，创建所需动画的层
//        // TODO: check
//        if (UILAYER_IsMultiLayerEnable())
//        {
//            if (MMIMENU_DyCheckIsSyn())//check信息有变化
//            {
//    			_MMIMENU_GO_SetInitialized(FALSE); // 设置初始化状态
//    		}
//        }
//#endif
//#endif
        break;
        
    case MSG_CTL_GET_ACTIVE:
        if (qbtheme_ptr->is_move_state)
        {
            QbThemeStartShakeIconTimer(qbtheme_ptr);
        }
        break;
    case MSG_CTRLMAINMENU_RELOAD_ITEM:
#ifdef DYNAMIC_MAINMENU_SUPPORT
	if (MMIMENU_DyCheckIsSyn())//check信息有变化
	{
		ReloadItemToMainMenu(qbtheme_ptr);
	}
#endif
		
        break;
    case MSG_CTL_LOSE_FOCUS:
        MMITheme_StopControlText();

        // reset all timers
        QbThemeStopMoveTimer(qbtheme_ptr);

        QbThemeStopShakeIconTimer(qbtheme_ptr);

#ifdef UI_MULTILAYER_SUPPORT
        // 由于多层画图，失去焦点的时候，需要把主菜单信息画在主层上，因此这里需要在lose focus时画图
        if (UILAYER_IsMultiLayerEnable())
        {
            GUI_LCD_DEV_INFO    lcd_dev_info    = {0};

            // fix cr248659
            qbtheme_ptr->is_move_state = FALSE;
			//fix Bug 132051
			qbtheme_ptr->slide_state = MMK_TP_SLIDE_NONE;

            QbThemeStopRedrawTimer(qbtheme_ptr);

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
            StopHighlightIconDispTimer(qbtheme_ptr);
#endif

            qbtheme_ptr->is_display_item_layer = FALSE;
            
            qbtheme_ptr->qbtheme_menu_data.is_disp_item_bg
                = MMIAPIQBTHEME_IsIconHaveBg();
            
            //若前一帧为direct draw，需要重画
            if ( UILAYER_IsPerDirectDraw() )
            {
                DisplayQbThemeItem(qbtheme_ptr, qbtheme_ptr->cur_page_index, lcd_dev_info); // 画icon item
            }
        }
#endif
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_CLOSE:
        MMITheme_StopControlText();

        // stop all timers
        QbThemeStopMoveTimer(qbtheme_ptr);

        QbThemeStopShakeIconTimer(qbtheme_ptr);

        qbtheme_ptr->is_moving = FALSE;

        qbtheme_ptr->slide_state = MMK_TP_SLIDE_NONE;
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (MMK_TP_SLIDE_NONE == qbtheme_ptr->slide_state && !qbtheme_ptr->is_moving)
        {   
            QbThemeHandleOkKey(qbtheme_ptr,msg_id);

            //// TODO:
            //HandleTextToSpeech(qbtheme_ptr);
        }
        break;

    case MSG_APP_RED:
        if (MMK_TP_SLIDE_TP_SCROLL == qbtheme_ptr->slide_state || qbtheme_ptr->is_moving)
        {
            result = MMI_RESULT_TRUE;
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_NOTIFY_CANCEL:    
    case MSG_APP_CANCEL:
        if (MMK_TP_SLIDE_TP_SCROLL != qbtheme_ptr->slide_state && !qbtheme_ptr->is_moving)
        { 
            QbThemeHandleCancelKey(qbtheme_ptr, MSG_APP_CANCEL);

            //// TODO:
            //HandleTextToSpeech(qbtheme_ptr);
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
        if (MMK_TP_SLIDE_NONE == qbtheme_ptr->slide_state && !qbtheme_ptr->is_move_state)
        {
            // 先停止，避免和3D产生纠结，导致exp
            MMITheme_StopControlText();

            //// TODO:
            //HandleTextToSpeech(qbtheme_ptr);
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
        if (MMK_TP_SLIDE_NONE == qbtheme_ptr->slide_state && !qbtheme_ptr->is_move_state)
        {
            QbThemeHandleNumKey(qbtheme_ptr,msg_id);
            //HandleTextToSpeech(qbtheme_ptr);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        // 先停止，避免和3D产生纠结，导致exp
        MMITheme_StopControlText();

        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        HandleQbThemeTpDown(&tp_point,qbtheme_ptr);

        //// TODO:
        //HandleTextToSpeech(qbtheme_ptr);
        break;

    case MSG_TP_PRESS_UP:
        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        if (HandleQbThemeTpUp(&tp_point,qbtheme_ptr))
        {
            //enter the next menu or notify ok msg
            QbThemeHandleOkKey(qbtheme_ptr,MSG_TP_PRESS_UP);
        }

        //// TODO:
        //HandleTextToSpeech(qbtheme_ptr);
        break;

    case MSG_TP_PRESS_MOVE:
        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        HandleQbThemeMenuTpMove(&tp_point,qbtheme_ptr);
        break;

    case MSG_TP_PRESS_SHORT:
        // 处理tp short 消息
        if ( qbtheme_ptr->is_item_pressed )
        {
            if (qbtheme_ptr->is_move_state)
            {
                QbMenuInitMoveLayer( qbtheme_ptr );
            }
        }
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_PENOK:
        if (MMK_TP_SLIDE_TP_SCROLL != qbtheme_ptr->slide_state && !qbtheme_ptr->is_moving)
        {
            QbThemeHandleCancelKey(qbtheme_ptr, MSG_APP_CANCEL);
            //HandleTextToSpeech(qbtheme_ptr);
        }
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_TIMER:
        if (qbtheme_ptr->start_move_timer_id == *(uint8*)param)
        {
            GUIMENU_MOVE_STATE_T move_state = GUIMENU_STATE_MOVE_NONE;

            QbThemeStopMoveTimer(qbtheme_ptr);

            move_state = GUIMENU_STATE_MOVE_SET;

            MMITHEME_Vibrate(MMITHEME_MENUMAIN_ICON_VIBRATE); // 震动一次

            MMK_SendMsg(qbtheme_ptr->win_handle, MSG_NOTIFY_MENU_SET_STATUS, (DPARAM)&move_state);
        }
        else if (qbtheme_ptr->shake_icon_timer_id == *(uint8*)param)
        {
			MMITHEME_StopVibrate();
			
            QbThemeStopShakeIconTimer(qbtheme_ptr);
            
            DisplayQbThemeSpecialMenu(qbtheme_ptr);
        }
        else
        {
            result = HandleQbThemeTimer(qbtheme_ptr, msg_id, param);
        }
        break;

    case MSG_CTRLMAINMENU_SET_MOVE_STATE:
        qbtheme_ptr->is_move_state = TRUE;
        QbThemeMenuMoveInit(qbtheme_ptr);
        break;

    case MSG_CTRLMAINMENU_CANCEL_MOVE_ICON:
        HandleQbThemeMenuCancelKey(qbtheme_ptr);
        break;

    case MSG_CTRLMAINMENU_GET_ID:
        {
            MMIMAINMENU_ITEM_INFO_T *item_info_ptr = (MMIMAINMENU_ITEM_INFO_T*)param;
            MMIMAINMENU_ITEM_INFO_T *cur_item_info_ptr = PNULL;

            cur_item_info_ptr = GetItemInfoPtr(
                    qbtheme_ptr,
                    qbtheme_ptr->cur_page_index,
                    qbtheme_ptr->cur_item_index);

            item_info_ptr->group_id = cur_item_info_ptr->group_id;
            item_info_ptr->menu_id = cur_item_info_ptr->menu_id;
        }
        break;

    case MSG_CTRLMAINMENU_IS_MOVE_STATE:
        *((BOOLEAN*)param) = qbtheme_ptr->is_move_state;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
   
    return (result);
}

/*****************************************************************************/
// Description : 初始化链表
// Global resource dependence : 
//  Author:jin.wang
// Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void QbThemeDestroyPageList(
                                   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                   )
{
    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    // 释放item数据链表
    MMIMENU_DestroyPageList(&qbtheme_ptr->menu_page_info);

    // 释放DOCK栏数据链表
    MMIMENU_DestroyItemList(&qbtheme_ptr->menu_bar_info);
}

/*****************************************************************************/
//  Description : 重写qb的dock数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIMENU_ResetQbThemeDockbar(
    MMIMAINMENU_PAGE_ITEM_INFO_T *menu_info_ptr
)
{
    int32 i= 0;
    int32 j= 0;
    GUIMENU_GROUP_T * menu_group_ptr = PNULL;

    if (PNULL == menu_info_ptr)
    {
        return 0;
    }

    menu_group_ptr = (GUIMENU_GROUP_T *)MMI_GetMenuGroup(MENU_QBTHEME_BAR);
    menu_info_ptr->item_num = MIN(menu_group_ptr->item_count,MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM);

    for(i = 0; i<  menu_info_ptr->item_num ; i++)
    {
#ifndef MAINLCD_SIZE_320X480
        if(ID_COMMUNICATION_MESSAGE == menu_group_ptr->item_ptr[j].menu_id)
        {
			i--;
			j++;
            continue;
        }
#endif

        menu_info_ptr->menu_item_info[i].menu_id = menu_group_ptr->item_ptr[j].menu_id  ;     
        menu_info_ptr->menu_item_info[i].group_id = 0;//MENU_QBTHEME_BAR  ; 
		j++;
    }

    MMI_WriteNVItem( MMINV_QBTHEME_DOCKBAR_INFO, menu_info_ptr);

    return menu_info_ptr->item_num;
}

/*****************************************************************************/
//  Description : 读取千变的dockbar数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMENU_ReadQbThemeDockbar(
                                     MMIMAINMENU_PAGE_ITEM_INFO_T *dockbar_item_ptr
                                     )
{
    MN_RETURN_RESULT_E  nv_result = MN_RETURN_FAILURE;

    if (PNULL == dockbar_item_ptr)
    {
        return FALSE;
    }

    //get icon menu item number from nv
    MMINV_READ(MMINV_QBTHEME_DOCKBAR_INFO, dockbar_item_ptr, nv_result);

    if (MN_RETURN_SUCCESS != nv_result)
    {
        MMIAPIMENU_ResetQbThemeDockbar(dockbar_item_ptr);
    }

    return TRUE;
}
#ifdef MMI_SAM_THEME
/*****************************************************************************/
//  Description : 读取三星的dockbar数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSamThemeDockbar(
                                 MMIMAINMENU_PAGE_ITEM_INFO_T *dockbar_item_ptr
                                 )
{
    uint16 i = 0;
    GUIMENU_GROUP_T * menu_group_ptr = PNULL;

    if (PNULL == dockbar_item_ptr)
    {
        return FALSE;
    }

    menu_group_ptr = (GUIMENU_GROUP_T *)MMI_GetMenuGroup(MENU_SAMSUNG_BAR);
    dockbar_item_ptr->item_num = menu_group_ptr->item_count;
    
    for (i = 0; i < dockbar_item_ptr->item_num; i++)
    {
        dockbar_item_ptr->menu_item_info[i].group_id = 0;//MENU_SAMSUNG_BAR;
	    dockbar_item_ptr->menu_item_info[i].menu_id = menu_group_ptr->item_ptr[i].menu_id;
	}

    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : 读取DockBar的数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeReadDockbarInfo(
                                     MMIMAINMENU_PAGE_ITEM_INFO_T *dockbar_item_ptr
                                     )
{
    BOOLEAN result = FALSE;

    // dock栏数据
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        result = ReadSamThemeDockbar(dockbar_item_ptr);
    }
    else
#endif
    {
        result = MMIAPIMENU_ReadQbThemeDockbar(dockbar_item_ptr);
    }

#ifdef DYNAMIC_MAINMENU_SUPPORT
	//整理动态菜单图标
	if (result)
	{
    	TrimQbThemePageInfo(dockbar_item_ptr);
	}
#endif

    return result;
}

/*****************************************************************************/
//  Description : 生成菜单数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
//      1 读取nv中存储的静态数据
//      2 读取U盘、T卡中的动态数据
/*****************************************************************************/
LOCAL BOOLEAN ReadQbThemeMenuPageItemInfo(
                                         MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                         MMIMAINMENU_PAGE_ITEM_INFO_T *item_page_ptr,
                                         uint16                       *page_num_ptr
                                         )
{
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = PNULL;
    MMI_LINK_NODE_T          *node_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T  *item_ptr = PNULL;
	uint16  max_page_num = 0;
	uint16 page_index = 0;
	uint16 item_index = 0;
    uint16 count_num = 0;

    if ( PNULL == item_page_ptr )
    {
        return FALSE;
    }

    // 拿原始item数据，这里获取的是所有的item数据
    item_list_ptr = MMIMAINMENU_GetItemList();
    if (PNULL == item_list_ptr)
    {
        return FALSE;
    }

    node_ptr = item_list_ptr->list_head_ptr;
    count_num = item_list_ptr->item_count;

	// 页数
	max_page_num = count_num / MMITHEME_QBMENU_ITEM_MAX_NUM;
	max_page_num = count_num % MMITHEME_QBMENU_ITEM_MAX_NUM == 0 ? max_page_num : max_page_num + 1; 
	max_page_num = MIN( max_page_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM );
	
	//读取静态菜单信息
	for (page_index = 0; page_index < max_page_num; page_index++)
	{
		for (item_index = 0; item_index < MMITHEME_QBMENU_ITEM_MAX_NUM; item_index++)
		{
            if (PNULL == node_ptr)
            {
                break;
            }

            item_ptr = (MMIMAINMENU_ITEM_INFO_T *)node_ptr->data;

			item_page_ptr[page_index].menu_item_info[item_index].group_id = item_ptr->group_id;
			item_page_ptr[page_index].menu_item_info[item_index].menu_id = item_ptr->menu_id;
			item_page_ptr[page_index].item_num++;

            node_ptr = GetNextShownItem(node_ptr);
            if (PNULL == node_ptr || node_ptr == item_list_ptr->list_head_ptr)
            {
                break;
            }
		}

        // 所有的item都查完，结束
        if (PNULL == node_ptr || node_ptr == item_list_ptr->list_head_ptr)
        {
            break;
        }
	}

    if (PNULL != page_num_ptr)
    {
        *page_num_ptr = page_index + 1;
    }

    return TRUE;
}

///*****************************************************************************/
////  Description : 
////  Global resource dependence : 
////  Author:jin.wang
////  Note: 
///*****************************************************************************/
//LOCAL void DeleteQbThemeMenuEmptyPage(
//                                    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
//                                    )
//{
//#if FANGHUA
//    uint16              page_index = 0;
//    GUIMENULIST_ITEM_T  *item_list_ptr = PNULL;

//    for (page_index = 0; page_index < qbtheme_ptr->page_total_num; page_index++)
//    {
//        item_list_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, page_index);

//        if (PNULL != item_list_ptr && 0 == item_list_ptr->item_total_num)
//        {
//            MMIMENU_DeleteOnePage(qbtheme_ptr, page_index);

//            qbtheme_ptr->page_total_num = qbtheme_ptr->menu_page_info.page_count;

//            DeleteQbThemeMenuEmptyPage(qbtheme_ptr);
//        }
//    }
//#endif
//}

/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeResetBarIndexRect(
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
    GUIMENU_QBTHEME_DATA_T      *menu_info_ptr, //in/out:
    uint16                      item_num
)
{
    BOOLEAN    result     = FALSE;
    GUI_RECT_T total_rect = menu_info_ptr->bar_rect;
    GUI_RECT_T title_rect = {0};
    uint16 image_width = 0;
    uint16 image_height = 0;
    int32 interval_x = 0;
    int32 start_x = 0;
    GUI_POINT_T start_point = {0};
    uint16 i = 0;
    
    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }
    
    GUIRES_GetImgWidthHeight(
            &image_width, &image_height,
            IMAGE_THEME_TAG_PAGE1,
            qbtheme_ptr->win_handle);

    title_rect = total_rect;
#ifndef MAINLCD_SIZE_240X320
    title_rect.top = title_rect.bottom - MMITHEME_QBTHEMEMENU_ITEM_HEIGHT
                        - MENUTEXT_HEIGHT - image_height + 1
                        - MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN;
#else
    title_rect.top = title_rect.bottom - image_height + 1 - MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN;
#endif	
    interval_x = (title_rect.right - title_rect.left + 1 
                    - MMITHEME_MENUMAIN_SLIDE_PAGE_NUM * image_width)
                 / (MMITHEME_MENUMAIN_SLIDE_PAGE_NUM + 1);

    start_x = (title_rect.right + title_rect.left - item_num * (image_width+interval_x) + interval_x)/2;
    
    for (i = 0; i < item_num; i++)
    {
        start_point.y = title_rect.top;
        start_point.x =  (int16)(start_x + (interval_x + (int32)image_width) * i);          

        menu_info_ptr->bar_index_rect[i].left = start_point.x;
        menu_info_ptr->bar_index_rect[i].right = start_point.x + image_width;
        menu_info_ptr->bar_index_rect[i].top = start_point.y;
        menu_info_ptr->bar_index_rect[i].bottom = start_point.y + image_height;
        
        result = TRUE;
    }       

    return result;
}

/*****************************************************************************/
//  Description : reset
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ResetQbThemeMenuAutoRect(
                                  MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                  )
{
    uint16 lcd_width = 0; // lcd宽度
    uint16 lcd_height = 0; // lcd高度
    uint16 move_space = 0;
    
    GUILCD_GetLogicWidthHeight(qbtheme_ptr->lcd_dev_info.lcd_id, &lcd_width, &lcd_height);
    
    move_space = MIN(lcd_height, lcd_width);

    s_left_direct_move_rect = qbtheme_ptr->rect;
    s_left_direct_move_rect.left = 0;
    s_left_direct_move_rect.right = move_space/10;
    s_left_direct_move_rect.bottom = qbtheme_ptr->qbtheme_menu_data.bar_rect.top - 1;

    s_right_direct_move_rect = qbtheme_ptr->rect;
    s_right_direct_move_rect.right = lcd_width - 1;
    s_right_direct_move_rect.left = s_right_direct_move_rect.right - move_space/10;
    s_right_direct_move_rect.bottom = qbtheme_ptr->qbtheme_menu_data.bar_rect.top - 1;
}

/*****************************************************************************/
//  Description : open menu,set menu parameter
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void InitQbThemeMenuGoInfo(
								 MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
								 )
{
    GUI_RECT_T page_rect = GetPageRect( qbtheme_ptr );

	//_MMIMENU_GO_SetInitialized(FALSE); // 设置初始化状态
	CreateQbThemeMenuBgLayer(qbtheme_ptr);
	
	CreateQbThemeMenuCalcDstLayer(qbtheme_ptr, page_rect);
	CreateQbThemeItemLayer(qbtheme_ptr);
	QbTheme_GO_CreatMemory(qbtheme_ptr);
	//_MMIMENU_GO_SetInitialized(TRUE);
}

/*****************************************************************************/
//     Description : 销毁单，释放内存
//    Global resource dependence : 
//  Author:xiaoqing.lu
//    Note: 
/*****************************************************************************/
LOCAL void DestoryGOMenu(
                      MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                      )
{
    // 释放
    ReleaseQbThemeMenuPageBgLayer(qbtheme_ptr);

    ReleaseQbThemeMenuCalcDstLayer(qbtheme_ptr);

    // 释放
    DestoryQbThemeMenuItemLayer();

    QbTheme_GO_ReleaseMemory();

    //s_go_is_initialized = FALSE;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMoveBgLayer(
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
)
{
    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    // 释放层
    if (UILAYER_IsMultiLayerEnable())    
    {
        GUI_LCD_DEV_INFO lcd_info = {0};

        lcd_info = GetCurrentPageLyaerInfo(qbtheme_ptr);
        UILAYER_Clear(&lcd_info);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : remove all menu layer
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void RemoveAllQbThemeMenuLayer(
                                   MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                   )
{
    GUI_LCD_DEV_INFO lcd_dev_info[6] = {0};
    uint32 i = 0;

    lcd_dev_info[0] = qbtheme_ptr->page_layer_handle[0];
    lcd_dev_info[1] = qbtheme_ptr->page_layer_handle[1];
    lcd_dev_info[2] = qbtheme_ptr->icon_move_layer_handle;
    lcd_dev_info[3] = qbtheme_ptr->highlight_layer_handle;
    lcd_dev_info[4] = qbtheme_ptr->titleidx_layer_handle;

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
LOCAL void SetBltQbThemeMenuPageLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                     UILAYER_HANDLE_T layer1,
                                     UILAYER_HANDLE_T layer2,
                                     UILAYER_HANDLE_T layer3
                                     )
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    
    RemoveAllQbThemeMenuLayer(qbtheme_ptr);

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
}

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeMenu(
							  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
							  )
{
    //滑动抖动过程中不重画
    if (( 0 != qbtheme_ptr->tp_timer_id 
            || 0 != qbtheme_ptr->redrew_timer_id
            || qbtheme_ptr->is_move_state )
        && qbtheme_ptr->is_display_item_layer )
    {
        return;
    }

    if (MMK_TP_SLIDE_NONE != qbtheme_ptr->slide_state)
    {
        return;
    }
	
	if(!MMK_IsFocusWin(qbtheme_ptr->win_handle))
    {
		DisplayQbThemePageMainLayer(qbtheme_ptr);

		DisplayQbThemeItem(
            qbtheme_ptr, qbtheme_ptr->cur_page_index,
            GetCurrentPageLyaerInfo(qbtheme_ptr));

        return;
    }
	
	qbtheme_ptr->is_delete_state = FALSE;

#ifdef DYNAMIC_MAINMENU_SUPPORT
    if (MMIMENU_DyCheckIsSyn())//check信息有变化
    {
        ReloadItemToMainMenu(qbtheme_ptr);
    }
#endif

	//if (!_MMIMENU_GO_IsInitialized())
	//{
	//	InitQbThemeMenuGoInfo(qbtheme_ptr);
	//}

	//SCI_TRACE_LOW:"[GO_MENU]   DisplaySlidePageMenu "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_QBTHEME_4247_112_2_18_2_34_45_169,(uint8*)"");
	
    // 检查内存是否申请成功
	if(!CheckQbThemeLayerMem(qbtheme_ptr))
	{
        return;
	}

    qbtheme_ptr->qbtheme_menu_data.is_disp_item_bg = MMIAPIQBTHEME_IsIconHaveBg();
		
	DisplayQbThemePageMainLayer(qbtheme_ptr);
	
	//画高亮层上的内容
    DisplayHighlightIconLayer(&qbtheme_ptr->highlight_layer_handle);

	if(DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        if (!UILAYER_IsLayerActive(&qbtheme_ptr->layer_bg))
        {
            return;
        }
    }
    else if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        if (!UILAYER_IsLayerActive(&qbtheme_ptr->page_layer_handle[0]))
        {
            return;
        }

        //合并计算的层到主层
        SetBltQbThemeMenuPageLayer(qbtheme_ptr, qbtheme_ptr->page_layer_handle[0].block_id, 0, 0);
    }

	BltQbThemeMenuBarLayer(qbtheme_ptr);

	// 显示千变主题的索引bar
    DisplayQbThemeBar(
        qbtheme_ptr,
        qbtheme_ptr->cur_page_index,
        qbtheme_ptr->titleidx_layer_handle,
        TRUE);

	DisplayItemLayer(qbtheme_ptr);
}

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemePageMainLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                     )
{
    uint16     layer_width = 0;
    uint16     layer_height = 0;
    GUI_RECT_T rect = {0};
    
    SCI_ASSERT(PNULL != qbtheme_ptr);/*assert verified*/

    GUILCD_GetLogicWidthHeight(qbtheme_ptr->lcd_dev_info.lcd_id, &layer_width, &layer_height);
    
    rect.left = 0;
    rect.top = 0;
    rect.right = layer_width - 1;
    rect.bottom = layer_height - 1;

    if(DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        //display bg
        DisplayQbThemeBg(rect,qbtheme_ptr,&qbtheme_ptr->layer_bg);
        return;
    }
    else
    {
        //display bg
        DisplayQbThemeBg(rect,qbtheme_ptr, PNULL);

        return;
    }
}

/*****************************************************************************/
//  Description : display menu background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeBg(
                              GUI_RECT_T        bg_rect,
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                              GUI_LCD_DEV_INFO  *lcd_dev_info_ptr
                              )
{
    MMI_IMAGE_ID_T      bg_img_id = qbtheme_ptr->main_menu_theme.bg_image;
    GUI_LCD_DEV_INFO    *dst_lcd_dev_info_ptr = lcd_dev_info_ptr;

    if (PNULL == dst_lcd_dev_info_ptr)
    {
        dst_lcd_dev_info_ptr = &qbtheme_ptr->lcd_dev_info;
    }

#ifdef BG_ON_BOTTOM_SUPPORT        
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable())
    {
        if (MMITHEME_GetBgImgId() == bg_img_id)
        {
            //通常背景
            MMITHEME_DisplayBg(&bg_rect, qbtheme_ptr->win_handle, dst_lcd_dev_info_ptr);
        }
        else if (0 != bg_img_id)
        {
            GUIRES_DisplayImg(PNULL,
                &bg_rect,
                &bg_rect,
                qbtheme_ptr->win_handle,
                bg_img_id,
                (const GUI_LCD_DEV_INFO*)dst_lcd_dev_info_ptr);
        }
        else
        {
            GUI_FillRect(dst_lcd_dev_info_ptr,
                bg_rect,
                qbtheme_ptr->main_menu_theme.bg_color);
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
                qbtheme_ptr->win_handle,
                bg_img_id,
                (const GUI_LCD_DEV_INFO*)dst_lcd_dev_info_ptr);
        }
        else
        {
            GUI_FillRect(dst_lcd_dev_info_ptr,
                bg_rect,
                qbtheme_ptr->main_menu_theme.bg_color);
        }
    }
	GUISTATUSBAR_UpdateCtrl(qbtheme_ptr->win_handle,TRUE);
}

/*****************************************************************************/
//  Description : display slide page menu
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayItemLayer(
                           MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                           )
{
#if 0
    int16 current_frame = 0;
    int16 first_page = 0;
    int16 next_page =0;
    int16 page_num = 0;
    BOOLEAN is_disp_active = FALSE;
    
    if (PNULL == qbtheme_ptr)
    {
        return;
    }
    
    InitQbThemeItemData(qbtheme_ptr);
    
    page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
    
    first_page = qbtheme_ptr->cur_page_index;
    if (0 != page_num)
    {
        next_page = (first_page+page_num-1)%page_num;
    }
    
    if ((!qbtheme_ptr->common_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }
    
#if defined(U_MENU_HAVE_ENTER_ANIM)
    if (GUIMENU_GetStatePtr(qbtheme_ptr, GUIMENU_STATE_ENTER))
    {
        //过场动画，显示主菜单特效
        if(GO_SLIDE_SUPPORT_ONLY_Y==qbtheme_ptr->support_slide_type)
        {
            for(current_frame = (GO_MAINMENU_TOTAL_FRAME_MAX/2);current_frame>=0;current_frame-=GO_FRAME_STEP_NUM)
            {
                DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,is_disp_active);
                GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
            }
            if(-GO_FRAME_STEP_NUM != current_frame)
            {
                current_frame = 0;
                DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,is_disp_active);
                GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
            }
        }
        else
        {
            for(current_frame = (GO_MAINMENU_TOTAL_FRAME_MAX/2);current_frame>=0;current_frame-=GO_FRAME_STEP_NUM)
            {
                DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,is_disp_active);
                GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
            }
            if(-GO_FRAME_STEP_NUM != current_frame)
            {
                current_frame = 0;
                DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,is_disp_active);
                GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
            }
        }
    }
    else
#else
    {
        
        if(GO_SLIDE_SUPPORT_ONLY_Y==qbtheme_ptr->support_slide_type)
        {
            DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,is_disp_active);
        }
        else
        {
            DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,is_disp_active);
        }
    }    
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    StartHighlightIconDispTimer(qbtheme_ptr);
#endif

#else
    int16 current_frame = 0;
    int16 first_page = 0;
    int16 next_page =0;
    int16 page_num = 0;
    
    if ((!qbtheme_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        //默认要显示高亮
        InitQbThemeItemData(qbtheme_ptr);
        
        page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        first_page = qbtheme_ptr->cur_page_index;
        next_page = (first_page+page_num-1)%page_num;
        DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,TRUE);
    }
    else
    {
        if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
        {
            UILAYER_SetPreMultiplyAlpha(&qbtheme_ptr->page_layer_handle[0],FALSE,FALSE);
        }

        UILAYER_Clear( &qbtheme_ptr->page_layer_handle[0] );
        
        DisplayQbThemePageItem(
            qbtheme_ptr,
            qbtheme_ptr->cur_page_index,
            GetCurrentPageLyaerInfo(qbtheme_ptr));

        //GUIWIN_UpdateStbIcon();
        //GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        
        //初始化三页内容放在显示后面，加快进入速度
        //InitQbThemeItemData(qbtheme_ptr);
        StartInitItemTimer(qbtheme_ptr);
    }
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    StartHighlightIconDispTimer(qbtheme_ptr);
#endif

#endif
}

/*****************************************************************************/
//  Description :绘制并刷新函数
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void DrawUpdateGOMenuLayer(
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                              uint16 first_page,
                              uint16 second_page,
                              GO_MOVE_DIRECTION_E direction,
                              uint16 current_frame,
                              BOOLEAN is_display_highlight
                              )
{
    if(s_go_item_page_info[0].layer_handle.block_id == UILAYER_NULL_HANDLE
        ||s_go_item_page_info[1].layer_handle.block_id == UILAYER_NULL_HANDLE
        ||s_go_item_page_info[2].layer_handle.block_id == UILAYER_NULL_HANDLE
        )
    {
        //SCI_TRACE_LOW:"s_go_item_page_info handle block_id== PNULL"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_2316_112_2_18_2_35_34_213,(uint8*)"");
        return;
    }
    
    DrawGOMenuLayer(qbtheme_ptr,first_page,second_page,direction,current_frame,is_display_highlight);

//    TransmitGoMenuTpMsg(qbtheme_ptr, MSG_CTL_PAINT, PNULL);
    
    QbThemeStoreUpdateRect(qbtheme_ptr);
}

/*****************************************************************************/
//  Description :绘制函数
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void DrawGOMenuLayer(
                           MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                           uint16 first_page,
                           uint16 second_page,
                           GO_MOVE_DIRECTION_E direction,
                           uint16 current_frame,
                           BOOLEAN is_display_highlight
                           )
{     
    IMGREF_SIZE_T bg_window_size = {0};
    uint16* wallpaper_window_buf = PNULL;
    uint8 *layer_buf_ptr = {0};
    IMGREF_SIZE_T highlight_icon_size = {0};

    GO_MENU_STYLE_E cur_style = MMIAPISET_GetGOmenuStyle();
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 cost_time = 0;
    void* bg_window_buf = PNULL;
    go_calc_func calc_func = PNULL;
    GO_CALC_DATA_INFO_T calc_data_info = {0};
    static uint16 pre_frame = 0;
    UILAYER_INFO_T layer_info_ptr = {0};
    UILAYER_INFO_T dst_layer_info_ptr = {0};    
     
    SCI_ASSERT(PNULL != qbtheme_ptr);/*assert verified*/

    if( PNULL == s_cur_page_info_ptr
        || PNULL == s_next_page_info_ptr
        || PNULL == s_calc_point_buf
        )
    {
        return;
    }
    //如果祯数一样则不进行重画
    if((pre_frame == current_frame)
        &&(current_frame!=0)
        &&(current_frame!=GO_MAINMENU_TOTAL_FRAME_MAX)
        )
    {
        return;
    }

    //SCI_TRACE_LOW:"[GO_MENU]DrawGOMenuLayer current time  = %d "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_2373_112_2_18_2_35_34_214,(uint8*)"d", SCI_GetTickCount());

    GO_GetPageInfo(qbtheme_ptr,first_page,s_cur_page_info_ptr);
    
    GO_GetPageInfo(qbtheme_ptr,second_page,s_next_page_info_ptr);    
    
    if(DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        calc_data_info.input_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcInputDataType());
        calc_data_info.output_type = SEAPI_DataTypeAdapt(DATA_TYPE_RGB565); 
        calc_data_info.param = s_go_calc_param_ptr;
        calc_data_info.param->shiningCtrl_ptr = s_go_shining_ctrl_ptr;
        
        if((0 == current_frame)||(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame))
        {
            //UILAYER_SwitchDoubleMem( &qbtheme_ptr->page_layer_handle[0], FALSE );
        }
        else
        {
            
            UILAYER_SetDirectDraw( TRUE );
            //UILAYER_SetMainLayerImmutable( TRUE );
            //设置双buffer切换，不使用copy
            //UILAYER_SwitchDoubleMem( &qbtheme_ptr->page_layer_handle[0], TRUE );
        }
        
        //GUILCD_GetLogicWidthHeight(qbtheme_ptr->lcd_dev_info.lcd_id, &bg_window_size.w, &bg_window_size.h);
        UILAYER_GetLayerInfo(&lcd_dev_info,&dst_layer_info_ptr);
        bg_window_size.w = dst_layer_info_ptr.layer_width;
        bg_window_size.h = dst_layer_info_ptr.layer_height;
        //lcd屏幕BUF
        bg_window_buf = (void*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
        if(PNULL == bg_window_buf)
        {
            return;
        }
        
        //背景图BUF
        wallpaper_window_buf = (uint16*)UILAYER_GetLayerBufferPtr(&qbtheme_ptr->layer_bg);
        if(PNULL == wallpaper_window_buf)
        {
            return;
        }
        
        SCI_MEMCPY(bg_window_buf, wallpaper_window_buf, bg_window_size.w*bg_window_size.h*sizeof(COLOR16));    
    }
    else if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        UILAYER_SetPreMultiplyAlpha(&qbtheme_ptr->page_layer_handle[0],TRUE,FALSE);

        calc_data_info.input_type = SEAPI_DataTypeAdapt(SEAPI_GetCalcInputDataType());
        calc_data_info.output_type = SEAPI_DataTypeAdapt(DATA_TYPE_PMARGB888); 
        calc_data_info.param = s_go_calc_param_ptr;
        calc_data_info.param->shiningCtrl_ptr = s_go_shining_ctrl_ptr;
        if((0 == current_frame)||(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame))
        {
            //UILAYER_SwitchDoubleMem( &qbtheme_ptr->page_layer_handle[0], FALSE );
            //UILAYER_SetDirectDraw( TRUE );    
            //UILAYER_SetMainLayerImmutable( TRUE );
            //设置双buffer切换，不使用copy
            UILAYER_SwitchDoubleMem( &qbtheme_ptr->page_layer_handle[0], FALSE );
        }
        else
        {
//#ifndef WIN32            
            UILAYER_SetDirectDraw( TRUE );
            UILAYER_SetMainLayerImmutable( TRUE );
            //设置双buffer切换，不使用copy
            UILAYER_SwitchDoubleMem( &qbtheme_ptr->page_layer_handle[0], TRUE );
//#endif
        }
        
        UILAYER_Clear(&qbtheme_ptr->page_layer_handle[0]);
        
        //GUILCD_GetLogicWidthHeight(qbtheme_ptr->lcd_dev_info.lcd_id, &bg_window_size.w, &bg_window_size.h);
        UILAYER_GetLayerInfo(&qbtheme_ptr->page_layer_handle[0],&dst_layer_info_ptr);
        bg_window_size.w = dst_layer_info_ptr.layer_width;
        bg_window_size.h = dst_layer_info_ptr.layer_height;
        
        //算法输出BUF
        if( PNULL == ( bg_window_buf = (uint32*)UILAYER_GetLayerBufferPtr( &qbtheme_ptr->page_layer_handle[0] ) ) )
        {
            return;
        }  
    }
    cost_time = SCI_GetTickCount();
    if((0 == current_frame)
        &&(MMK_TP_SLIDE_NONE == qbtheme_ptr->slide_state)
        )
    {
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(&qbtheme_ptr->highlight_layer_handle);
        if(PNULL == layer_buf_ptr)
        {
            return;
        }
        
        UILAYER_GetLayerInfo(&qbtheme_ptr->highlight_layer_handle,&layer_info_ptr);
        highlight_icon_size.w = layer_info_ptr.layer_width;
        highlight_icon_size.h = layer_info_ptr.layer_height;
        Go_Effect_Zero_Type_Calc(
            s_cur_page_info_ptr,//当前页信息
            bg_window_size,//屏幕尺寸
            bg_window_buf, //背景buffer
            qbtheme_ptr,
            (uint32*)layer_buf_ptr,
            highlight_icon_size,
            is_display_highlight,
            first_page,
            &calc_data_info
            );
    }
    else if(GO_MAINMENU_TOTAL_FRAME_MAX == current_frame
        &&(MMK_TP_SLIDE_NONE == qbtheme_ptr->slide_state))
    {
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(&qbtheme_ptr->highlight_layer_handle);
        if(PNULL == layer_buf_ptr)
        {
            return;
        }
        
        UILAYER_GetLayerInfo(&qbtheme_ptr->highlight_layer_handle,&layer_info_ptr);
        highlight_icon_size.w = layer_info_ptr.layer_width;
        highlight_icon_size.h = layer_info_ptr.layer_height;
        
        Go_Effect_Zero_Type_Calc(
            s_next_page_info_ptr,//当前页信息
            bg_window_size,//屏幕尺寸
            bg_window_buf, //背景buffer
            qbtheme_ptr,
            (uint32*)layer_buf_ptr,
            highlight_icon_size,
            is_display_highlight,
            second_page,
            &calc_data_info
            );
    }
    else
    {
        if(GO_MENU_STYLE_RANDOM == cur_style)
        {
            calc_func = UAPIMenu_GetCallBackfunc(s_random_cur_style);
        }
        else
        {
            calc_func = UAPIMenu_GetCallBackfunc(cur_style);
        }
        
        if(PNULL == calc_func)
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
    //SCI_TRACE_LOW:"[GO_MENU]MMIMENU_GO_Effect_calc calcfunc cost time = %d         STYLE = %d      frame = %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_2534_112_2_18_2_35_34_215,(uint8*)"ddd", cost_time,cur_style,current_frame);
    pre_frame = current_frame;
}

/*****************************************************************************/
//  Description :PAF 边界处理函数
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Go_Effect_Zero_Type_Calc(
    GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
    IMGREF_SIZE_T bg_window_size,//屏幕尺寸
    uint32* bg_window_buf,//背景buffer
    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
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
    if(is_display_highlight)
    { 
        GUI_RECT_T menu_rect = {0};
        GUI_RECT_T item_rect = {0};
        FIX16_POINT_T h_window_offset={0}; 
        
        item_rect = GetQbThemeHighlightRect(qbtheme_ptr, GetQbThemeMenuPageIndex(qbtheme_ptr, qbtheme_ptr->cur_item_index));
        menu_rect = AdjustSlideMenuRect(qbtheme_ptr);
        
        h_window_offset.x = item_rect.left-menu_rect.left;
        h_window_offset.y = item_rect.top-menu_rect.top;
        
        //把高亮框先贴上
        S2D_TypeBlending(bg_window_size,bg_window_buf,calc_data_info->output_type,highlight_icon_size,h_window_offset,select_image_buf,calc_data_info->input_type, bg_window_buf,PNULL);
    }
#endif
    
    icon_num = GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, page_index);
    
    for(i=0;i<icon_num;i++)
    {
        if(NULL!=(*(cur_page_info->icon_info+i)).icon_buf)
        {    
            //贴图标
            window_offset.x=(cur_page_info->icon_info+i)->icon_offset.x;
            window_offset.y=(cur_page_info->icon_info+i)->icon_offset.y;
            //画高亮框
#ifdef GO_STATIC_SELECT_USE_GREY
            if(is_display_highlight&&(i==qbtheme_ptr->cur_item_index))
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
//  Description : display slide page menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO DisplayOneItem(
                                      MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                                      uint16                        page_index,
                                      uint16                        item_index,
                                      GUIMENU_ITEM_POSITION_E       position
                                      )
{
    return DisplayOneItemToLayer( qbtheme_ptr, PNULL, PNULL, page_index, item_index, position );
}

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO DisplayOneItemToLayer(
                                             MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                             const GUI_LCD_DEV_INFO     *dst_dev_ptr,
                                             GUI_RECT_T                 *rect_ptr,
                                             uint16                     page_index,
                                             uint16                     item_index,
                                             GUIMENU_ITEM_POSITION_E    position
                                             )
{
    GUI_LCD_DEV_INFO             item_layer = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
    CTRLMAINMENU_ITEM_T         menuitem_info = {0};
    GUIMENU_DISPLAY_ITEM_PARAM_T param      = {0};

    if ( PNULL == qbtheme_ptr )
    {
        return item_layer;
    }
    
    QbThemeMenuGetItemInfo( qbtheme_ptr,  position, page_index, item_index,&menuitem_info );
    
    if ( PNULL == dst_dev_ptr || PNULL == rect_ptr )
    {
        item_layer = MMITHEME_DrawMainmenuItem( qbtheme_ptr->win_handle, &menuitem_info, &param);
    }
    else
    {
        if ( DATA_TYPE_RGB565 == UILAYER_GetLayerColorType( dst_dev_ptr ) )
        {
            item_layer = MMITHEME_DrawMainmenuItem( qbtheme_ptr->win_handle, &menuitem_info, &param);
            
            UILAYER_SetLayerPosition( &item_layer, rect_ptr->left, rect_ptr->top );
            UILAYER_BltLayerToLayer( dst_dev_ptr, &item_layer, rect_ptr, TRUE );
        }
        else
        {
            param.format |= GUIMENU_DISPLAY_ITEM_FORMAT_TO_LAYER;
            param.dst_layer = *dst_dev_ptr;
            param.dst_rect  = *rect_ptr;
            
            item_layer = MMITHEME_DrawMainmenuItem( qbtheme_ptr->win_handle, &menuitem_info, &param);
        }
    }

    return item_layer;
}

/*****************************************************************************/
//  Description : 画bar上的item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeBarItem(
								 MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
								 )
{
    uint16              i = 0;
    GUI_RECT_T          disp_rect = {0};
    uint16              item_num = 0;
    GUI_RECT_T          bar_rect[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX] = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    GUI_LCD_DEV_INFO    item_layer = {0};

    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    lcd_dev_info = qbtheme_ptr->titleidx_layer_handle;
        
    item_num = GetQbThemeMenuBarItemNum( qbtheme_ptr );
    
    GetDockBarItemRect( qbtheme_ptr, bar_rect, item_num );

    for (i = 0; i < item_num; i++)
    { 
		if (i != s_undraw_index)
		{
			disp_rect = bar_rect[i];
			
			disp_rect.bottom = disp_rect.top + MMITHEME_MENU_ITEM_HEIGHT - 1;
			
			item_layer = DisplayOneItemToLayer(qbtheme_ptr, PNULL, PNULL, 0, i, GUIMENU_ITEM_IN_BAR );
			
			UILAYER_SetLayerPosition(&item_layer, disp_rect.left, disp_rect.top);
			
			UILAYER_BltLayerToLayer(&lcd_dev_info, &item_layer, &disp_rect, TRUE );
		}
    }
}

/*****************************************************************************/
//  Description : display bar
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeBar(
                            MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
                            uint16                      cur_page_index,
                            GUI_LCD_DEV_INFO            lcd_dev_info,
                            BOOLEAN                     is_draw_all
                            )
{

    uint16          i = 0;
    uint16          image_width = 0;
    uint16          image_height = 0;
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      bar_rect = {0};
    GUI_RECT_T      disp_rect = {0};
    MMI_IMAGE_ID_T  title_img_id = IMAGE_NULL;

    if (PNULL == qbtheme_ptr)
    {
        return;
    }
    
    IMG_EnableTransparentColor(TRUE);

	UILAYER_Clear(&lcd_dev_info);
	
	bar_rect = qbtheme_ptr->qbtheme_menu_data.bar_rect;
	
	GUIRES_GetImgWidthHeight(
        &image_width, &image_height, 
		qbtheme_ptr->qbtheme_menu_data.bar_image_id,
		qbtheme_ptr->win_handle);
	
	bar_rect.top = bar_rect.bottom - image_height + 1;

#ifndef MAINLCD_SIZE_240X320	
	//display title image
	GUIRES_DisplayImg(PNULL,
		&bar_rect,
		PNULL,
		qbtheme_ptr->win_handle,
		qbtheme_ptr->qbtheme_menu_data.bar_image_id,
		&lcd_dev_info);
#endif
	
	UILAYER_WeakLayerAlpha(&lcd_dev_info, qbtheme_ptr->qbtheme_menu_data.icon_mask_alpha);	
	
	DisplayQbThemeBarItem(qbtheme_ptr);
	
    for (i = 0; i < qbtheme_ptr->page_total_num; i++)
    {
        disp_rect = qbtheme_ptr->qbtheme_menu_data.bar_index_rect[i];
		
        //get title image id
        if (i == cur_page_index)
        {
            title_img_id = IMAGE_THEME_TAG_PAGE1 + i;
        }
        else
        {
            title_img_id = qbtheme_ptr->qbtheme_menu_data.bar_unsel_bg_image;
        }
		
        GUIRES_GetImgWidthHeight(&image_width, &image_height, title_img_id, qbtheme_ptr->win_handle);
		
        disp_rect = GUI_GetCenterRectExt(disp_rect, image_width, image_height);
        image_rect = GUI_CreateRect(0, 0, image_width - 1, image_height - 1);
		
        //display title image
        GUIRES_DisplayImg(PNULL,
            &disp_rect,
            &image_rect,
            qbtheme_ptr->win_handle,
            title_img_id,
            &lcd_dev_info);
    }
	
    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemePageItem(
                                MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
                                uint16             page_index,
                                GUI_LCD_DEV_INFO   lcd_dev_info
                                )
{
    uint16              i = 0;
    uint16              page_item_num = 0;
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;

    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, page_index);

    if (PNULL != page_item_info_ptr)
    {
        page_item_num = page_item_info_ptr->item_count;
        
        if (MMIMENU_MAIN_OPTION_PAGE_DEFAULT_SEL >= GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, page_index))
        {
            qbtheme_ptr->cur_item_index = 0;
        }
        
        for (i = 0; i < page_item_num; i++)
        {
			if (i == qbtheme_ptr->qbtheme_menu_data.select_idx)
			{
				MMIMAINEMENU_ITEM_LIST_T  *tmp_page_item_info_ptr = PNULL;
				MMIMAINMENU_ITEM_INFO_T   *item_info_ptr = PNULL;
				
				tmp_page_item_info_ptr = MMIMENU_GetItemList(
                        &qbtheme_ptr->menu_page_info,
                        qbtheme_ptr->cur_page_index);

				if ( PNULL != tmp_page_item_info_ptr ) 
				{
					item_info_ptr = MMIMENU_GetItemInfo(
                            tmp_page_item_info_ptr,
                            qbtheme_ptr->qbtheme_menu_data.select_idx);
				}
				
				if (qbtheme_ptr->is_delete_state &&
					(PNULL == item_info_ptr || item_info_ptr->menu_id == qbtheme_ptr->delete_menu_id))
					continue;
			}
			
            DisplayOneItemToLayer(
                qbtheme_ptr, &lcd_dev_info, 
                &qbtheme_ptr->qbtheme_menu_data.item_rect[i],
                page_index, i, GUIMENU_ITEM_IN_PAGE );
        }
    }

#if 0
    page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, page_index);
    //page_item_info_ptr = GetQbThemeMenuLinkListPtr(qbtheme_ptr, page_index);

    if (PNULL != page_item_info_ptr)
    {
        page_item_num = GetQbThemeMenuInPageMaxItemNum(qbtheme_ptr, page_item_info_ptr->item_count);
        
        if (MMIMENU_MAIN_OPTION_PAGE_DEFAULT_SEL >= GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, page_index))
        {
            qbtheme_ptr->cur_item_index = 0;
        }
        
        for (i = 0; i < page_item_num; i++)
        {
            disp_rect = qbtheme_ptr->qbtheme_menu_data.item_rect[i];

            if (i == qbtheme_ptr->cur_item_index)
            {
                MMIMAINMENU_ITEM_INFO_T* item_info_ptr = PNULL;
                
                item_info_ptr = GetMenuCurrentItemInfoPtr(qbtheme_ptr);
                
                if (qbtheme_ptr->is_delete_state && 
                    (PNULL == item_info_ptr || item_info_ptr->menu_id == qbtheme_ptr->delete_menu_id))
                    continue;
            }

            DisplayOneItemToLayer( qbtheme_ptr, &lcd_dev_info, &disp_rect, page_index, i, GUIMENU_ITEM_IN_PAGE );
        }
    }
#endif
}

/*****************************************************************************/
//  Description : display slide page menu item
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeItem(
                             MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
                             uint16                         page_index,
                             GUI_LCD_DEV_INFO               lcd_dev_info
                             )
{
    uint16              i = 0;
    uint16              item_num = 0;
    GUI_RECT_T          bar_rect[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX] = {0};

    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    // 画item
    DisplayQbThemePageItem(qbtheme_ptr, page_index, lcd_dev_info);

    // 画DockBar
    item_num = GetQbThemeMenuBarItemNum( qbtheme_ptr );
    
    GetDockBarItemRect( qbtheme_ptr, bar_rect, item_num );
    
    for (i = 0; i < item_num; i++)
    {
        DisplayOneItemToLayer( qbtheme_ptr, &lcd_dev_info, &bar_rect[i], 0, i, GUIMENU_ITEM_IN_BAR );
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayQbThemeActiveIcon(
									MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
									BOOLEAN                     is_display_highlight
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
    if(SELECT_EFFECT_STATIC == mainmenu_select_style)//静态
    {
        page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        first_page = qbtheme_ptr->cur_page_index;
        next_page = (first_page+page_num-1)%page_num;
        DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,is_display_highlight);
    }
    else//动态高亮框
    {
        //异步刷屏
        //target_buf = MMITHEME_GetAnotherMainLcdBufferPtr();
        if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
        {
            target_buf = UILAYER_GetLayerBufferPtr(&qbtheme_ptr->page_layer_handle[0]);
            UILAYER_GetLayerInfo(&qbtheme_ptr->page_layer_handle[0],&dst_layer_info_ptr);
            target_window_size.w = dst_layer_info_ptr.layer_width;
            target_window_size.h = dst_layer_info_ptr.layer_height;
        }
        else
        {
            target_buf = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
            GUILCD_GetLogicWidthHeight(qbtheme_ptr->lcd_dev_info.lcd_id, (uint16*)&target_window_size.w, (uint16*)&target_window_size.h);
        }
        
        if(PNULL == target_buf)
        {
            return;
        }
        
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        StartHighlightIconDispTimer(qbtheme_ptr);
#endif

        DisplayHighlightAnim(target_buf, target_window_size, qbtheme_ptr);
#ifdef WIN32
        SCI_Sleep(30);
#endif
        //GUIWIN_UpdateStbIcon();
        //QbThemeStoreUpdateRect(qbtheme_ptr);
        //MMITHEME_UpdateRect();
   
    }
#else
    {
        page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        first_page = qbtheme_ptr->cur_page_index;
        next_page = (first_page+page_num-1)%page_num;
        DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,is_display_highlight);
    }
#endif
}

/*****************************************************************************/
//  Description : display icon menu
//  Global resource dependence : 
//  Author: jin.wang
//  Note: 处理抖动timer
/*****************************************************************************/
LOCAL void DisplayQbThemeSpecialMenu(
                                   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                   )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == qbtheme_ptr)
    {
        return;
    }
    
    QbThemeStopShakeIconTimer(qbtheme_ptr);

    qbtheme_ptr->pre_cur_item_index = qbtheme_ptr->cur_item_index;
    
    if (qbtheme_ptr->is_move_state)
    {
        QbThemeStartShakeIconTimer(qbtheme_ptr);
    }
    
    if (MMK_TP_SLIDE_NONE != qbtheme_ptr->slide_state)
    {
        return;
    }
    
    // 设置合并的层，这里只需要把第一个page的层合并即可
    if (qbtheme_ptr->is_move_state || qbtheme_ptr->is_moving)
    {   
        SetMoveLayerPosition( qbtheme_ptr );

        if (!IsInToIdleRect(qbtheme_ptr, FALSE, PNULL))
        {
            lcd_dev_info = GetCurrentPageLyaerInfo(qbtheme_ptr);

            BltPageItemLayerToMenuLayer(
                qbtheme_ptr, 
                qbtheme_ptr->cur_page_index, 
                lcd_dev_info, 
                0,
                FALSE,
                FALSE);
        }
        else
        {
            if ((MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
#ifdef MMI_SAM_THEME            
                || ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
                    && (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()))
#endif                
                ) 
                
            {
                QbThemeStopMoveTimer(qbtheme_ptr);
                StopItemExchangeTimer(qbtheme_ptr);
                QbThemeStopShakeIconTimer(qbtheme_ptr);
                QbThemeStopRedrawTimer(qbtheme_ptr);

                if (qbtheme_ptr->is_item_pressed)
                {
                    GUI_POINT_T  current_point = {0};
                    LCD_ANGLE_E  current_angle = LCD_ANGLE_0;
                    LCD_ANGLE_E  idlewin_angle = LCD_ANGLE_0;
                    int16 temp = 0;
                    MMI_TP_STATUS_E state = MMI_TP_NONE;    
                    uint16  lcd_width = 0; 
                    uint16  lcd_height= 0;

                    MMK_GetLogicTPMsg(&state, &current_point);   
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

                    qbtheme_ptr->cur_item_index = qbtheme_ptr->move_item_index;

#ifdef MMI_SAM_THEME            
                    if (  (MMISET_IDLE_SUBSTYLE_SAM == MMIAPISET_GetIdleSubStyle())
                       && (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()))
                    {
                        MMK_SendMsg(qbtheme_ptr->win_handle, MSG_NOTIFY_GRID_SHORTCUT2IDLE, (DPARAM)&current_point); 
                    }
                    else
#endif
                    {
                        MMK_SendMsg(qbtheme_ptr->win_handle, MSG_NOTIFY_QBTHEME_SHORTCUT2IDLE, (DPARAM)&current_point);  
                    }
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : 显示高亮动画
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayHighlightAnim(
    uint8* target_buf,
    IMGREF_SIZE_T target_window_size,
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
)
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
     uint16 icon_num = 0;
    S3D_DATA_TYPE_E  input_type = S3D_DATA_ARGB888;
    S3D_DATA_TYPE_E  output_type = S3D_DATA_ARGB888;
    FIX16_POINT_T window_offset={0};
    BOOLEAN ret = TRUE;
    
     if( PNULL == s_cur_page_info_ptr )
     {
         return;
     }
 
     if( !MMK_IsFocusWin( qbtheme_ptr->win_handle ) )
     {
         return;
     }
     
     //入参检测
     if (PNULL == qbtheme_ptr)
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
     ret = CreateHighlightLayerByID(qbtheme_ptr, highlight_image_id_index, &highlight_icon_layer_handle);
     if(FALSE == ret)
     {
        return;
     }
     
     UILAYER_GetLayerInfo(&highlight_icon_layer_handle, &layer_struct_info);
     layer_buf_ptr = UILAYER_GetLayerBufferPtr(&highlight_icon_layer_handle);
     if(PNULL == layer_buf_ptr)
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
     pre_item_index = qbtheme_ptr->pre_cur_item_index;
     cur_item_index = qbtheme_ptr->cur_item_index;
     qbtheme_ptr->pre_cur_item_index = cur_item_index;


     //重画背景图

     if(DATA_TYPE_PMARGB888 != SEAPI_GetCalcOutputDataType())
     {
          wallpaper_window_buf = (uint16*)UILAYER_GetLayerBufferPtr(&qbtheme_ptr->layer_bg);
         if(PNULL == wallpaper_window_buf)
         {
             return;
         }
        SCI_MEMCPY(target_buf,wallpaper_window_buf,target_window_size.w*target_window_size.h*sizeof(uint16));
     }
     else
        {
            UILAYER_SetPreMultiplyAlpha(&qbtheme_ptr->page_layer_handle[0],TRUE,TRUE);

            UILAYER_Clear(&qbtheme_ptr->page_layer_handle[0]);
        }  
    //获取当前页的结构体指针(全局)
    GO_GetPageInfo(qbtheme_ptr,qbtheme_ptr->cur_page_index,s_cur_page_info_ptr);
    highlight_offset.x = (s_cur_page_info_ptr->icon_info+GetQbThemeMenuPageIndex(qbtheme_ptr, cur_item_index))->icon_offset.x;
    highlight_offset.y = (s_cur_page_info_ptr->icon_info+GetQbThemeMenuPageIndex(qbtheme_ptr, cur_item_index))->icon_offset.y;

    highlight_offset.x -= (highlight_size.w-(s_cur_page_info_ptr->icon_info+GetQbThemeMenuPageIndex(qbtheme_ptr, cur_item_index))->icon_size.w)/2;
    highlight_offset.y -= (highlight_size.h-(s_cur_page_info_ptr->icon_info+GetQbThemeMenuPageIndex(qbtheme_ptr, cur_item_index))->icon_size.h)/2;
    highlight_offset.y -= 2*GO_CALC_OFFSET;
    //menu_rect = MMITHEME_GetClientRectEx(qbtheme_ptr->win_handle);
    
    //highlight_offset.x -= menu_rect.left;
    //highlight_offset.y -= menu_rect.top;

    
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
    icon_num = GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, qbtheme_ptr->cur_page_index);

    for(i=0;i<icon_num;i++)
    {
        if(NULL!=(*(s_cur_page_info_ptr->icon_info+i)).icon_buf)
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
    UILAYER_RELEASELAYER(&highlight_icon_layer_handle);/*lint !e506 !e774*/

    return;
 #endif
}

/*****************************************************************************/
//  Description : 长按特效
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplaySpecialAnimation(
                                MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
                                BOOLEAN              is_disp_delete
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

    if (PNULL == qbtheme_ptr)
    {
        return;
    }
   
    // 获取图片的宽高
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, qbtheme_ptr->main_menu_theme.to_idle_icon_id, qbtheme_ptr->win_handle);

    // 获取lcd 宽高
    GUILCD_GetLogicWidthHeight(qbtheme_ptr->lcd_dev_info.lcd_id, &lcd_width, &lcd_height);

    // 获得左右区域
    GetInitRect(qbtheme_ptr, &delete_rect, &to_idle_rect, is_disp_delete, lcd_width, lcd_height);

    UILAYER_GetLayerInfo(&qbtheme_ptr->to_idle_layer_handle, &layer_info);

    move_space = layer_info.layer_width;
    
    for (cur_frame = 1; cur_frame <= frame_num; cur_frame++)
    {
        int16 to_idle_move_space = 0;
        int16 delete_move_space = 0;
        int16 offset_y = to_idle_rect.top;
        int16 offset_x = 0;

        to_idle_move_space = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);
        
        offset_x = lcd_width - to_idle_move_space;

		if (0 != qbtheme_ptr->to_idle_layer_handle.block_id
			&& UILAYER_NULL_HANDLE != qbtheme_ptr->to_idle_layer_handle.block_id)
		{
            UILAYER_SetLayerPosition(&qbtheme_ptr->to_idle_layer_handle, offset_x, offset_y);
		}

        if (is_disp_delete)
        {
            UILAYER_INFO_T delete_layer_info = {0};

			UILAYER_GetLayerInfo(&qbtheme_ptr->delete_layer_handle, &delete_layer_info);

            delete_move_space = move_space - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, move_space, frame_num, cur_frame);

            //offset_x = (layer_info.layer_width - icon_width) - delete_move_space;
            offset_x = (delete_layer_info.layer_width - icon_width) - delete_move_space;

            UILAYER_SetLayerPosition(&qbtheme_ptr->delete_layer_handle, offset_x, offset_y);
        }

        GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
#ifdef WIN32
        SCI_Sleep(100);
#endif
    }
}

/*****************************************************************************/
//  Description : 显示delitem bar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayDelItemBar(
                                 MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                 )
{
    BOOLEAN result = FALSE;
    BOOLEAN is_delete_item = IsTpInDeleteitemRect(qbtheme_ptr);
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T icon_rect = {0};
    MMI_IMAGE_ID_T delete_icon_id = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
       
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height,
        qbtheme_ptr->main_menu_theme.delete_icon_id,
        qbtheme_ptr->win_handle);
    
    icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);

    disp_rect = icon_rect;
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &qbtheme_ptr->delete_layer_handle, &disp_rect, &disp_rect);
    
    UILAYER_Clear(&qbtheme_ptr->delete_layer_handle);

    delete_icon_id = is_delete_item?qbtheme_ptr->main_menu_theme.delete_sel_icon_id:qbtheme_ptr->main_menu_theme.delete_icon_id;

    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(PNULL,
            &disp_rect,
            &icon_rect,
            qbtheme_ptr->win_handle,
            delete_icon_id,
            &qbtheme_ptr->delete_layer_handle);  
    
    IMG_EnableTransparentColor(FALSE);

    return result;
}

/*****************************************************************************/
//  Description : 显示toidle bar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayToIdleBar(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                )
{
    BOOLEAN result = FALSE;
    BOOLEAN is_disp_to_idle = IsTpInToidleRect(qbtheme_ptr);
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T icon_rect = {0};
    MMI_IMAGE_ID_T to_idle_icon_id = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;

    if (0 == qbtheme_ptr->to_idle_layer_handle.block_id
		|| UILAYER_NULL_HANDLE == qbtheme_ptr->to_idle_layer_handle.block_id)
    {
        return FALSE;
    }

    GUIRES_GetImgWidthHeight(&icon_width, &icon_height,
        qbtheme_ptr->main_menu_theme.to_idle_icon_id,
        qbtheme_ptr->win_handle);
    
    icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);

    disp_rect = icon_rect;
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &qbtheme_ptr->to_idle_layer_handle, &disp_rect, &disp_rect);
    
    UILAYER_Clear(&qbtheme_ptr->to_idle_layer_handle);

    to_idle_icon_id = is_disp_to_idle?qbtheme_ptr->main_menu_theme.to_idle_sle_icon_id:qbtheme_ptr->main_menu_theme.to_idle_icon_id;

    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(PNULL,
            &disp_rect,
            &icon_rect,
            qbtheme_ptr->win_handle,
            to_idle_icon_id,
            &qbtheme_ptr->to_idle_layer_handle);  
    
    IMG_EnableTransparentColor(FALSE);

    return result;
}

/*****************************************************************************/
//  Description : up date rect 
//  Global resource dependence : g_menu_cube_proc, g_menu_cube_proc_org
//  Author: peilong.zhou
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStoreUpdateRect(
    MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0,0};
    GUI_RECT_T menu_rect = {0};
    GUI_RECT_T title_rect = {0};
    uint16 i = 0,j = 0;

    //把title 区域不显示
    menu_rect = AdjustSlideMenuRect(qbtheme_ptr);
    
    title_rect.bottom = menu_rect.top-1;
    title_rect.right = menu_rect.right;

    if(DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        uint16* buf_ptr = PNULL;
        
        buf_ptr = (uint16*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
        
        if(PNULL == buf_ptr)
        {
            return;
        }
        
        for(i=0;i<=title_rect.bottom;i++)
        {
            for(j=0;j<=title_rect.right;j++)
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
//  Description : blt bar layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void BltQbThemeMenuBarLayer(
								  MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
								  )
{
	UILAYER_APPEND_BLT_T append_layer = {0};
	append_layer.lcd_dev_info = qbtheme_ptr->titleidx_layer_handle;
	append_layer.layer_level = UILAYER_LEVEL_HIGH;
	UILAYER_AppendBltLayer(&append_layer);

	UILAYER_SetLayerColorKey(&qbtheme_ptr->titleidx_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
	UILAYER_Clear(&qbtheme_ptr->titleidx_layer_handle);
}

/*****************************************************************************/
//  Description : display icon item bg and text
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void BltPageItemLayerToMenuLayer(
    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
    uint16                      page_index,
    GUI_LCD_DEV_INFO            lcd_dev_info,
    int16                       offset_x,
    BOOLEAN                     is_need_bar,
    BOOLEAN                     is_need_page
)
{
    uint16              i = 0;
    uint16              page_item_num = 0;
    GUI_RECT_T          disp_rect = {0};
    uint16              item_num = 0;
    GUI_RECT_T          bar_rect_array[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX] = {0};

    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    IMG_EnableTransparentColor(TRUE);

    if (is_need_page)
    {
        page_item_num = GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, page_index);
        
        for (i = 0; i < page_item_num; i++)
        {
            disp_rect = qbtheme_ptr->qbtheme_menu_data.item_rect[i];

            disp_rect.left += (offset_x);
            disp_rect.right += (offset_x);
            disp_rect.bottom = disp_rect.top + MMITHEME_MENU_ITEM_HEIGHT - 1;

            DisplayOneItemToLayer(qbtheme_ptr, &lcd_dev_info, &disp_rect, page_index, i, GUIMENU_ITEM_IN_PAGE );
        }
    }

    if ( is_need_bar )
    {
        GUI_RECT_T all_bar_rect = {0};

        all_bar_rect = qbtheme_ptr->qbtheme_menu_data.bar_rect;

        UILAYER_ClearRect(&lcd_dev_info, all_bar_rect);
        
        item_num = GetQbThemeMenuBarItemNum( qbtheme_ptr );
        
        GetDockBarItemRect( qbtheme_ptr, bar_rect_array, item_num );
        
        for (i = 0; i < item_num; i++)
        {
            //不画current item
            if ( i == qbtheme_ptr->tmp_cur_item_index 
                && qbtheme_ptr->is_moving
                && qbtheme_ptr->tmp_is_bar_item )
            {
                continue;
            }
            
            disp_rect = bar_rect_array[i];
            disp_rect.bottom = disp_rect.top + MMITHEME_MENU_ITEM_HEIGHT - 1;

            DisplayOneItemToLayer(qbtheme_ptr, &lcd_dev_info, &disp_rect, 0, i, GUIMENU_ITEM_IN_BAR );
        }
    }

    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckLayerMem( 
                           MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                           )
{
    BOOLEAN result = TRUE;
    int i = 0;

    if(!UILAYER_IsLayerActive(&qbtheme_ptr->page_layer_handle[0]))
    {
        result = FALSE;
    }
    else
    {
        for(i=0;i<GO_MEM_PAGE_NUM;i++)
        {
            if(!UILAYER_IsLayerActive(&s_go_item_page_info[i].layer_handle))
            {
                SCI_TRACE_LOW("CheckQbThemeLayerMem invalid layer i = %d, handle = %d", i, s_go_item_page_info[i].layer_handle.block_id);
                result = FALSE;
                break;
            }    
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : CheckQbThemeLayerMem
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckQbThemeLayerMem( 
                                   MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                   )
{
    BOOLEAN result = TRUE;
    
    if(PNULL == qbtheme_ptr)
    {
        return FALSE;
    }
    
    result = CheckLayerMem(qbtheme_ptr);
    
    if (!result)
    {
        //释放缓冲区
        BLKMEM_GarbageClean();

        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
        UILAYER_SetLayerMemFormat(&qbtheme_ptr->page_layer_handle[0],UILAYER_MEM_NONE);

        //重新初始化 申请内存
        UILAYER_ActiveAllLayerOfHandle( qbtheme_ptr->ctrl_handle );
        //MMIMENU_InitMenuData(qbtheme_ptr);

        //再次check
        result = CheckLayerMem(qbtheme_ptr);
        
        if (!result)
        {
            MMISET_IDLE_STYLE_E     idle_style = MMIAPISET_GetIdleStyle();
            MMISET_IDLE_SUBSTYLE_E cur_idle_substyle = MMIAPISET_GetIdleSubStyle();
            
            //只有默认系统下可以切换主菜单风格重新进入，其他风格只能退出到IDLE
            if(MMISET_IDLE_STYLE_GRID == idle_style&&MMISET_IDLE_SUBSTYLE_DEFAULT == cur_idle_substyle)
            {
                MMK_CloseWin(qbtheme_ptr->win_handle);
                MMIAPISET_SetMainmenuStyle(MMIMAINMENU_MATRIX_E);
                MMIAPIMENU_CreatMainMenu();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
                MMK_CloseWin(qbtheme_ptr->win_handle);
            }

        }
    }

    return result;
}

/*****************************************************************************/
//  Description : create highlight layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuHighlightLayer(
										   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
										   )
{   
    UILAYER_CREATE_T    create_info = {0};

	ReleaseQbThemeMenuHighlightLayer(qbtheme_ptr);

    if ((0 == qbtheme_ptr->highlight_layer_handle.block_id 
        || UILAYER_NULL_HANDLE == qbtheme_ptr->highlight_layer_handle.block_id))
    {
		create_info.lcd_id = GUI_MAIN_LCD_ID;
		create_info.owner_handle = qbtheme_ptr->ctrl_handle;
		create_info.offset_x = 0;
		create_info.offset_y = 0;
		create_info.width = MENUICON_FOCUSITEM_W;
		create_info.height = MENUICON_FOCUSITEM_H;
		create_info.is_bg_layer = FALSE;
		create_info.is_static_layer = FALSE;

		UILAYER_CreateLayer(
			&create_info,  &(qbtheme_ptr->highlight_layer_handle));

		UILAYER_SetLayerColorKey(&(qbtheme_ptr->highlight_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);
		
		if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
		{
			//ARGB888 TO PAF888  
			UILAYER_SetPreMultiplyAlpha(&qbtheme_ptr->highlight_layer_handle,TRUE,TRUE);
		}
		
		UILAYER_Clear(&(qbtheme_ptr->highlight_layer_handle));
    }
}

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuHighlightLayer(
                                          MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                          )
{
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&qbtheme_ptr->highlight_layer_handle);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : create bar layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuIndexBarLayer(
                                    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                    )
{   
    GUI_RECT_T          icon_rect = qbtheme_ptr->qbtheme_menu_data.bar_rect;
    UILAYER_CREATE_T    create_info = {0};
    
	// 先释放再申请
	UILAYER_RELEASELAYER(&qbtheme_ptr->titleidx_layer_handle);
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = qbtheme_ptr->ctrl_handle;
    create_info.offset_x = icon_rect.left;
    create_info.offset_y = icon_rect.top;
    create_info.width = (icon_rect.right + 1 - icon_rect.left);
    create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_FPS_ACCELERATE;
    
    UILAYER_CreateLayer(
        &create_info, &(qbtheme_ptr->titleidx_layer_handle));

    UILAYER_SetLayerColorKey(&(qbtheme_ptr->titleidx_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(&(qbtheme_ptr->titleidx_layer_handle));
}

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuIndexBarLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                     )
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&qbtheme_ptr->titleidx_layer_handle);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveIconLayer(
                               MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                               )
{   
    GUI_RECT_T cur_rect = {0};
    
    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    if (!GetCurItemRect( qbtheme_ptr, &cur_rect ) )
    {
        return;
    }

    if (0 == qbtheme_ptr->icon_move_layer_handle.block_id
        || UILAYER_NULL_HANDLE == qbtheme_ptr->icon_move_layer_handle.block_id)
    {
        UILAYER_INFO_T      layer_in    = {0};
        UILAYER_INFO_T      layer_out   = {0};
        SCALE_IMAGE_IN_T    scale_in    = {0};
        SCALE_IMAGE_OUT_T   scale_out   = {0};
        UILAYER_CREATE_T    create_info = {0};
        int16               width       = (cur_rect.right + 1 - cur_rect.left) * qbtheme_ptr->qbtheme_menu_data.zoom_out/100;
        int16               height      = (cur_rect.bottom + 1 - cur_rect.top) * qbtheme_ptr->qbtheme_menu_data.zoom_out/100;
        GUI_LCD_DEV_INFO    layer_handle = {0};
    
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = qbtheme_ptr->ctrl_handle;
        create_info.width = (int16)width;
        create_info.height = (int16)height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        create_info.format = UILAYER_MEM_FPS_ACCELERATE;

        UILAYER_CreateLayer(
            &create_info, &qbtheme_ptr->icon_move_layer_handle);

        SetMoveLayerPosition( qbtheme_ptr );

        UILAYER_SetLayerColorKey(&qbtheme_ptr->icon_move_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&qbtheme_ptr->icon_move_layer_handle);

        layer_handle = DisplayOneItem(
            qbtheme_ptr, qbtheme_ptr->cur_page_index, qbtheme_ptr->cur_item_index, 
            qbtheme_ptr->is_bar_item ? GUIMENU_ITEM_IN_BAR : GUIMENU_ITEM_IN_PAGE );

        UILAYER_GetLayerInfo(&layer_handle, &layer_in);
        UILAYER_GetLayerInfo(&qbtheme_ptr->icon_move_layer_handle, &layer_out);

        // src data info
        scale_in.src_format           = IMGREF_FORMAT_ARGB888;
        scale_in.src_chn.chn0.ptr     = UILAYER_GetLayerBufferPtr(&layer_handle);
        if (PNULL == scale_in.src_chn.chn0.ptr)
        {
            return;
        }

        scale_in.src_chn.chn0.size    = layer_in.layer_height*layer_in.layer_width*sizeof(uint32);
        scale_in.src_size.w           = layer_in.layer_width;
        scale_in.src_size.h           = layer_in.layer_height;        
        scale_in.src_trim_rect.w      = layer_in.layer_width;
        scale_in.src_trim_rect.h      = layer_in.layer_height;        // dest data info
        
        scale_in.target_format        = IMGREF_FORMAT_ARGB888;
        scale_in.target_buf.size      = layer_out.layer_height*layer_out.layer_width*sizeof(uint32) + 255;
        scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);
        scale_in.target_size.w        = layer_out.layer_width;
        scale_in.target_size.h        = layer_out.layer_height;
        
        if(SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            uint32  src_height     = MIN( layer_out.layer_height, scale_out.output_size.h );
            uint32  src_width      = MIN( layer_out.layer_width, scale_out.output_size.w );
            UILAYER_COPYBUF_PARAM_T     copy_param = {0};
            UILAYER_COPYBUF_T           copy_buf = {0};

            copy_param.rect = UILAYER_GetLayerRect(&qbtheme_ptr->icon_move_layer_handle);
            copy_param.rect.right = copy_param.rect.left+(int16)src_width-1;
            copy_param.rect.bottom = copy_param.rect.top+(int16)src_height-1;
            
            copy_buf.buf_ptr = (uint8*)scale_out.output_chn.chn0.ptr;
            copy_buf.data_type = DATA_TYPE_ARGB888;
            copy_buf.width = scale_out.output_size.w;
            copy_buf.height = scale_out.output_size.h;            

            UILAYER_CopyBufferToLayer(&qbtheme_ptr->icon_move_layer_handle, &copy_param, &copy_buf);
        }

        SCI_FREE(scale_in.target_buf.ptr);
    }
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMoveIconLayer(
                                      MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                      )
{
    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    // 释放层
    if (UILAYER_IsMultiLayerEnable())    
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&qbtheme_ptr->icon_move_layer_handle);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : set move layer position
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void SetMoveLayerPosition(
                                 MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                 )
{
    if (PNULL != qbtheme_ptr && qbtheme_ptr->is_moving)
    {
        GUI_RECT_T move_rect = GetQbThemeMenuMoveLayerRect(qbtheme_ptr);
        
        AppendMainMenuLayer(qbtheme_ptr, qbtheme_ptr->icon_move_layer_handle, UILAYER_LEVEL_HIGH);

        UILAYER_SetLayerPosition(&qbtheme_ptr->icon_move_layer_handle, move_rect.left, move_rect.top);
    }
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void AppendMainMenuLayer(
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                               GUI_LCD_DEV_INFO lcd_dev_info,
                               UILAYER_LEVEL_E layer_level
                               )
{
    UILAYER_APPEND_BLT_T append_layer = {0};


    if (PNULL != qbtheme_ptr && UILAYER_IsLayerActive(&lcd_dev_info))
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = layer_level;//UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    return;
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuBgLayer(
                                 MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                 )
{
    if(DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        uint16              layer_width = 0;
        uint16              layer_height = 0;
        UILAYER_CREATE_T    create_info = {0};
        
        GUILCD_GetLogicWidthHeight(qbtheme_ptr->lcd_dev_info.lcd_id, &layer_width, &layer_height);

        qbtheme_ptr->layer_bg.lcd_id = 0;
        qbtheme_ptr->layer_bg.block_id = UILAYER_NULL_HANDLE;
        
        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = qbtheme_ptr->ctrl_handle;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = layer_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = TRUE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(
            &create_info,
            &qbtheme_ptr->layer_bg
            );

        // 设置color key
        UILAYER_SetLayerColorKey(&qbtheme_ptr->layer_bg, TRUE, UILAYER_TRANSPARENT_COLOR);

        // 使用color清除层
        UILAYER_Clear(&qbtheme_ptr->layer_bg);

        //GUIBUTTON_SetDev(MAINMENUWIN_BUTTON_CHANGE_CTRL_ID,&qbtheme_ptr->layer_bg);
        //GUIWIN_SetStbDev(qbtheme_ptr->win_id, &qbtheme_ptr->layer_bg);
    }
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuPageBgLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                     )
{ 
    if(DATA_TYPE_RGB565 == SEAPI_GetCalcOutputDataType())
    {
        // 释放层
        UILAYER_RELEASELAYER(&qbtheme_ptr->layer_bg);/*lint !e506 !e774*/ 
    }
}

/*****************************************************************************/
//  Description : 算法输出BUF
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeMenuCalcDstLayer(
                                          MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                                          GUI_RECT_T       rect
                                          )
{
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUI_RECT_T menu_rect = {0};

        // 申请之前先释放
        UILAYER_RELEASELAYER(&qbtheme_ptr->page_layer_handle[0]);/*lint !e506 !e774*/
    
        menu_rect = rect;//AdjustSlideMenuRect(qbtheme_ptr);
        qbtheme_ptr->page_layer_handle[0].lcd_id = 0;
        qbtheme_ptr->page_layer_handle[0].block_id = UILAYER_NULL_HANDLE;
        
        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = qbtheme_ptr->ctrl_handle;
        create_info.offset_x = menu_rect.left;
        create_info.offset_y = menu_rect.top;
        create_info.width = menu_rect.right-menu_rect.left+1;
        create_info.height = menu_rect.bottom-menu_rect.top+1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;
        UILAYER_CreateLayer(
            &create_info,
            &qbtheme_ptr->page_layer_handle[0]
            );
        UILAYER_SetLayerPosition(&qbtheme_ptr->page_layer_handle[0], menu_rect.left, menu_rect.top);
        //ARGB888 TO PAF888  只需要设置layer type，BUF不需要转换
        // UILAYER_SetPreMultiplyAlpha(&qbtheme_ptr->page_layer_handle[0],TRUE,FALSE);
        // 设置color key
        UILAYER_SetLayerColorKey(&qbtheme_ptr->page_layer_handle[0], TRUE, UILAYER_TRANSPARENT_COLOR);
        // 使用color清除层
        UILAYER_Clear(&qbtheme_ptr->page_layer_handle[0]);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuCalcDstLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                     )
{  
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        // 释放层
        UILAYER_RELEASELAYER(&qbtheme_ptr->page_layer_handle[0]);/*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void CreateQbThemeItemLayer(
                                  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                  )
{
    uint16 item_num     = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint16 i            = 0;
    
    if(PNULL == qbtheme_ptr)
    {
        return;
    }
    
    for(i=0;i<GO_MEM_PAGE_NUM;i++)
    {
        UILAYER_RELEASELAYER(&s_go_item_page_info[i].layer_handle);/*lint !e506 !e774*/
        s_go_item_page_info[i].index = 0;
        s_go_item_page_info[i].is_change = FALSE;
    }
    
    icon_width = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;
    item_num = GetQbThemeMenuInPageMaxItemNum(qbtheme_ptr);
    
    //创建当前页
    CreateItemLayer(qbtheme_ptr, icon_width,icon_height,item_num,&(s_go_item_page_info[1].layer_handle));
    if(s_go_item_page_info[1].layer_handle.block_id == UILAYER_NULL_HANDLE)
    {
        return;
    }
    
    //创建上一页
    CreateItemLayer(qbtheme_ptr, icon_width,icon_height,item_num,&(s_go_item_page_info[0].layer_handle));
    if(s_go_item_page_info[0].layer_handle.block_id == UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_go_item_page_info[1].layer_handle);/*lint !e506 !e774*/
        return;
    }
    
    //创建下一页
    CreateItemLayer(qbtheme_ptr, icon_width,icon_height,item_num,&(s_go_item_page_info[2].layer_handle));
    
    if(s_go_item_page_info[2].layer_handle.block_id == UILAYER_NULL_HANDLE)
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
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void CreateItemLayer(
                          MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                          uint16 icon_width,
                          uint16 icon_height,
                          uint16 item_total_num,
                          GUI_LCD_DEV_INFO *layer_handle
                          )
{
    UILAYER_CREATE_T    create_info = {0};
    
    if( PNULL == qbtheme_ptr
        || PNULL == layer_handle )
    {
        return;
    }
    //创建所有LAYER的层
    layer_handle->lcd_id = 0;
    layer_handle->block_id = UILAYER_NULL_HANDLE;
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = qbtheme_ptr->ctrl_handle;
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
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DestoryQbThemeMenuItemLayer(void)
{
    uint16 i = 0;

    for(i=0;i<GO_MEM_PAGE_NUM;i++)
    {
        UILAYER_RELEASELAYER(&s_go_item_page_info[i].layer_handle);/*lint !e506 !e774*/
        s_go_item_page_info[i].index = 0;
        s_go_item_page_info[i].is_change = FALSE;
    }

    SCI_MEMSET(s_slidemenu_item_layer, 0, sizeof(s_slidemenu_item_layer));
    
    return;
}

/*****************************************************************************/
//  Description : create qbtheme icon layer
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void CreateOrigSpecialLayer(
                                  MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,  //in
                                  BOOLEAN            is_to_idle
                                  )
{
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T icon_rect = {0};
    UILAYER_CREATE_T create_info = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    BOOLEAN is_enable_del = FALSE;

    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = qbtheme_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 

    if (is_to_idle)
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height,
            qbtheme_ptr->main_menu_theme.to_idle_icon_id,
            qbtheme_ptr->win_handle);
        
        create_info.width = icon_width;
        create_info.height = icon_height;
        
        UILAYER_RELEASELAYER(&qbtheme_ptr->to_idle_layer_handle);/*lint !e774*/
        UILAYER_CreateLayer(&create_info, &qbtheme_ptr->to_idle_layer_handle);
        UILAYER_SetLayerColorKey(&qbtheme_ptr->to_idle_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        AppendMainMenuLayer(qbtheme_ptr, qbtheme_ptr->to_idle_layer_handle, UILAYER_LEVEL_HIGH);
        
        disp_rect = icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);
        
        LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &qbtheme_ptr->to_idle_layer_handle, &disp_rect, &disp_rect);
        
        IMG_EnableTransparentColor(TRUE);
        
        GUIRES_DisplayImg(PNULL,
            &disp_rect,
            &icon_rect,
            qbtheme_ptr->win_handle,
            qbtheme_ptr->main_menu_theme.to_idle_icon_id,
            &qbtheme_ptr->to_idle_layer_handle);   
    }

    is_enable_del = QbMenuIsItemEnableDeleted(qbtheme_ptr);

    UILAYER_RELEASELAYER(&qbtheme_ptr->delete_layer_handle);/*lint !e774*/

    //动态图标支持删除，静态图标不支持删除功能
    if (is_enable_del) 
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, qbtheme_ptr->main_menu_theme.delete_icon_id,qbtheme_ptr->win_handle);
        create_info.width = icon_width;
        create_info.height = icon_height;

        UILAYER_CreateLayer(&create_info, &qbtheme_ptr->delete_layer_handle);
        UILAYER_SetLayerColorKey(&qbtheme_ptr->delete_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        AppendMainMenuLayer(qbtheme_ptr, qbtheme_ptr->delete_layer_handle, UILAYER_LEVEL_HIGH);
        
        disp_rect = icon_rect = GUI_CreateRect(0, 0, icon_width, icon_height);
        
        LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &qbtheme_ptr->delete_layer_handle, &disp_rect, &disp_rect);
        
        GUIRES_DisplayImg(PNULL,
            &disp_rect,
            &icon_rect,
            qbtheme_ptr->win_handle,
            qbtheme_ptr->main_menu_theme.delete_icon_id,
            &qbtheme_ptr->delete_layer_handle); 
    }
    
    IMG_EnableTransparentColor(FALSE);
    
    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void ReleaseOrigSpecialLayer(
                                    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
                                    )
{
    if (PNULL == qbtheme_ptr)
    {
        return;
    }
    
    // 释放层
    if (UILAYER_IsMultiLayerEnable())/*lint !e774*/
    {
        UILAYER_RELEASELAYER(&qbtheme_ptr->to_idle_layer_handle);/*lint !e506 !e774*/
        UILAYER_RELEASELAYER(&qbtheme_ptr->delete_layer_handle);/*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description :申请PAGE信息存放BUF
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbTheme_GO_CreatMemory(
                                      MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
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

    total_row_num = qbtheme_ptr->qbtheme_menu_data.page_row_num;
    total_col_num = qbtheme_ptr->qbtheme_menu_data.page_col_num;
    
    calc_point_buf_size = 1024*sizeof(FIX16_POINT_T);//1024为算法所需
    page_info_buf_size = sizeof(GO_MAINMENU_PAGE_INFO_T);

    page_icon_buf_size = total_row_num*total_col_num*sizeof(GO_MAINMENU_ICON_INFO_T);    

    calc_param_size = sizeof(S3D_EXPANDPTR_T);
    
    alloc_size = calc_point_buf_size\
        + GO_PAGE_READY_NUM*page_info_buf_size\
        + GO_PAGE_READY_NUM*page_icon_buf_size\
        + calc_param_size\
        +shining_ctrl_buf_size;

    if(PNULL != s_go_alloc_mem_ptr)
    {
        GO_FREEIF(s_go_alloc_mem_ptr);
    }
    
    s_go_alloc_mem_ptr = SCI_ALLOCA(alloc_size);
    if(PNULL == s_go_alloc_mem_ptr)
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
LOCAL BOOLEAN QbTheme_GO_ReleaseMemory(void)
{    
    if(PNULL != s_go_alloc_mem_ptr)
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
//  Description : create move bg layer
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveBgLayer(
							 MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
							 )
{
    GUI_LCD_DEV_INFO lcd_info = {0};
    
    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    lcd_info = GetCurrentPageLyaerInfo(qbtheme_ptr);

    UILAYER_WeakLayerAlpha(
        &lcd_info, qbtheme_ptr->qbtheme_menu_data.icon_mask_alpha);
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseQbThemeMenuPageLayer(
                                     MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr  //in
                                     )
{
    if (PNULL == qbtheme_ptr)
    {
        return;
    }
    
    // 释放层
    if (UILAYER_IsMultiLayerEnable())    
    {
        //消除highlight层的影响
        UILAYER_RELEASELAYER(&qbtheme_ptr->page_layer_handle[0]);   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : CreateHighlightLayerEx
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateHighlightLayerByID(
    MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
    MMI_IMAGE_ID_T image_id,
    GUI_LCD_DEV_INFO *layer_handle_ptr
)
{   
    UILAYER_CREATE_T    create_info = {0};
    GUI_POINT_T      display_point   = {0,0};

    UILAYER_RELEASELAYER(layer_handle_ptr);
    
    layer_handle_ptr->lcd_id = 0;
    layer_handle_ptr->block_id = UILAYER_NULL_HANDLE;
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888  
        UILAYER_SetPreMultiplyAlpha(layer_handle_ptr,FALSE,FALSE);
    }

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = qbtheme_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&
        create_info.height, image_id, qbtheme_ptr->ctrl_handle);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    UILAYER_CreateLayer(
        &create_info,  
        layer_handle_ptr
        );
    
    UILAYER_SetLayerColorKey(layer_handle_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(layer_handle_ptr);
    
    if(!UILAYER_IsLayerActive(layer_handle_ptr))
    {
        UILAYER_RELEASELAYER(layer_handle_ptr);
        return FALSE;
    }
    //把图片画到层上
    GUIRES_DisplayImg(&display_point,
        PNULL,
        PNULL,
        qbtheme_ptr->ctrl_handle,
        image_id,
        (const GUI_LCD_DEV_INFO*)layer_handle_ptr);
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888  
        UILAYER_SetPreMultiplyAlpha(layer_handle_ptr,TRUE,TRUE);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : get page rect
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetPageRect(
                             MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                             )
{
    GUI_RECT_T page_rect = AdjustSlideMenuRect(qbtheme_ptr);
    page_rect.bottom = qbtheme_ptr->qbtheme_menu_data.bar_rect.top - 1;
    
    return page_rect;
}

/*****************************************************************************/
// Description : get icon text rect
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void GetIconTextRect(
                           GUI_RECT_T  item_rect,
                           GUI_RECT_T* icon_rect,
                           GUI_RECT_T* text_rect
                           )
{
    if ( PNULL != icon_rect )
    {
        *icon_rect = item_rect;
        *icon_rect = GUI_GetCenterRect( *icon_rect, MENUICON_WIDTH, MENUICON_WIDTH );
    }

    if ( PNULL != text_rect )
    {
        *text_rect = item_rect;
        text_rect->top = item_rect.top + MMITHEME_QBTHEMEMENU_ITEM_HEIGHT;
        text_rect->bottom += MENUTEXT_HEIGHT;
    }
}

/*****************************************************************************/
//  Description : get cur item rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCurItemRect(
                             MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,  //in
                             GUI_RECT_T                 *rect_ptr        //out
                             )
{
    BOOLEAN result = FALSE;

    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }

    if ( PNULL != rect_ptr )
    {
        if ( qbtheme_ptr->is_bar_item )
        {
            uint16 item_num = GetQbThemeMenuBarItemNum( qbtheme_ptr );
            
            if ( qbtheme_ptr->cur_item_index < item_num )
            {
                GUI_RECT_T bar_item_rect[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX] = {0};
                
                GetDockBarItemRect( qbtheme_ptr, bar_item_rect, item_num );
                
                *rect_ptr = bar_item_rect[qbtheme_ptr->cur_item_index];
                
                result = TRUE;
            }
        }
        else
        {
            if ( qbtheme_ptr->cur_item_index < GetQbThemeMenuPageTotalItemNum( qbtheme_ptr, qbtheme_ptr->cur_page_index ) )
            {
                *rect_ptr = qbtheme_ptr->qbtheme_menu_data.item_rect[qbtheme_ptr->cur_item_index];
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : get pubwin button rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note: 
/*****************************************************************************/
LOCAL void GetDockBarItemRect(
                          MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                          GUI_RECT_T                    rect_ptr[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX],
                          uint32                        num
                          )
{
    GUI_RECT_T bar_rect = qbtheme_ptr->qbtheme_menu_data.bar_rect;
    uint16     i = 0;
    uint16     image_width = 0;
    uint16     image_height = 0;
    uint16     margin = MMITHEME_QBMENU_LEFT_RIGHT_MARGIN;//( bar_rect.right - bar_rect.left + 1 - MENUTEXT_WIDTH * MMITHEME_QBMENU_PAGE_ROW_NUM ) / ( MMITHEME_QBMENU_PAGE_ROW_NUM + 1) /2;

    GUIRES_GetImgWidthHeight(
            &image_width, &image_height,
            IMAGE_THEME_TAG_PAGE1, qbtheme_ptr->win_handle);

    bar_rect.top += image_height;
    bar_rect.bottom -= MENUTEXT_HEIGHT;
	bar_rect.left += margin;
	bar_rect.right -= margin;

    GUI_CreateMatrixRect(&bar_rect, 0, 0, 0, 0, 1, num, rect_ptr);

    for ( i = 0; i < MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX; i++ )
    {
        rect_ptr[i] = GUI_GetCenterRect( rect_ptr[i], MENUTEXT_WIDTH, MMITHEME_MENU_ITEM_HEIGHT );
    }
}

/*****************************************************************************/
//  Description : 获取高亮区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetQbThemeHighlightRect(
                                        MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                        int16 item_index_inpage
                                        )
{
#ifdef MMI_PDA_SUPPORT
    {
        GUI_RECT_T item_rect = {0};

        item_rect = qbtheme_ptr->qbtheme_menu_data.item_rect[item_index_inpage];

        return item_rect;
    }
#else
    {
        GUI_RECT_T icon_rect = {0};

        icon_rect = qbtheme_ptr->qbtheme_menu_data.item_rect[item_index_inpage]; 
        
        // 获取中心区域，定高和宽
        icon_rect = GUI_GetCenterRectExt( icon_rect, qbtheme_ptr->main_menu_theme.focus_item_w, qbtheme_ptr->main_menu_theme.focus_item_h);

        return icon_rect;
    }
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetQbThemeMenuMoveLayerRect(
											 MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
											 )
{
    GUI_RECT_T tmp_rect = {0};
    GUI_POINT_T point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    int16 tmp_width  = (int16)(qbtheme_ptr->tmp_width * qbtheme_ptr->qbtheme_menu_data.zoom_out/100);
    int16 tmp_height = (int16)(qbtheme_ptr->tmp_height * qbtheme_ptr->qbtheme_menu_data.zoom_out/100);
    int16 width      = (int16)(qbtheme_ptr->qbtheme_menu_data.item_width * qbtheme_ptr->qbtheme_menu_data.zoom_out/100);
    int16 height     = (int16)(qbtheme_ptr->qbtheme_menu_data.item_height * qbtheme_ptr->qbtheme_menu_data.zoom_out/100);
    
    MMK_GetLogicTPMsg(&state, &point);
    
    tmp_rect.left = MAX(point.x - tmp_width, 0);
    tmp_rect.top = MAX(point.y - tmp_height, 0);
    tmp_rect.right = tmp_rect.left + width - 1;
    tmp_rect.bottom = tmp_rect.top + height - 1;

    return tmp_rect;
}

/*****************************************************************************/
//  Description : is point in icon rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPointInIconRect(
                                MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
                                GUI_POINT_T                 point,
                                GUI_RECT_T                  rect
                                )

{
    GUI_RECT_T icon_rect = {0};

    GetIconTextRect( rect, &icon_rect, PNULL );

    return GUI_PointIsInRect( point, icon_rect );
}

/*****************************************************************************/
// Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL void GetItemPtr(
					  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
					  uint16                        page_index,
					  uint16                        item_index,
					  CTRLMAINMENU_ITEM_T           *menuitem_ptr
					  )
{
    GUIMENU_ITEM_POSITION_E position = GUIMENU_ITEM_IN_PAGE;
    
    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    if ( qbtheme_ptr->is_bar_item )
    {
        position = GUIMENU_ITEM_IN_BAR;
    }
    else
    {
        position = GUIMENU_ITEM_IN_PAGE;
    }

    QbThemeMenuGetItemInfo(qbtheme_ptr, position, page_index, item_index, menuitem_ptr);
}

/*****************************************************************************/
// Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *GetItemInfoPtr(
                                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                                              uint16                        page_index,
                                              uint16                        item_index
                                              )
{
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T   *item_info_ptr = PNULL;
    
    if (PNULL == qbtheme_ptr)
    {
        return item_info_ptr;
    }
    
    if ( qbtheme_ptr->is_bar_item )
    {
        page_item_info_ptr = GetQbThemeMenuBarHeadPtr(qbtheme_ptr);
    }
    else
    {
        page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, page_index);
    }
    
    if (PNULL != page_item_info_ptr)
    {
        item_info_ptr = MMIMENU_GetItemInfo(page_item_info_ptr, item_index);
    }

    return item_info_ptr;
}

/*****************************************************************************/
// Description : 比较menu id，如果存在返回TRUE，如果不存在返回FALSE
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchMenuID(
                           MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                           MMI_MENU_ID_T                menu_id
                           )
{
    uint32 page_num = 0;
    uint32 item_num = 0;
    uint16 page_index = 0;
    BOOLEAN is_exist = FALSE;
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T   *item_info_ptr = PNULL;
    
    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }
    
    //查找item链表
    for (page_num = 0; page_num < qbtheme_ptr->page_total_num; page_num++)
    {
        page_index = page_num;
        page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, page_index);
        if (PNULL == page_item_info_ptr)
        {
            continue;
        }

        for (item_num = 0; item_num < page_item_info_ptr->item_count; item_num++)
        {
            item_info_ptr = MMIMENU_GetItemInfo(page_item_info_ptr, item_num);

            if (PNULL != item_info_ptr && item_info_ptr->menu_id == menu_id)
            {
                is_exist = TRUE;
                break;
            }
        }

        if (is_exist)
        {
            break;
        }
    }

    // 查找DockBar链表
    if ( !is_exist )
    {
        page_item_info_ptr = GetQbThemeMenuBarHeadPtr(qbtheme_ptr);

        for (item_num = 0; item_num < page_item_info_ptr->item_count; item_num++)
        {
            item_info_ptr = MMIMENU_GetItemInfo(page_item_info_ptr, item_num);

            if (PNULL != item_info_ptr && item_info_ptr->menu_id == menu_id)
            {
                is_exist = TRUE;
                break;
            }
        }
    }

    return is_exist;
}

/*****************************************************************************/
//  Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *GetQbThemeMenuBarHeadPtr(
                                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                                  )
{
    return &qbtheme_ptr->menu_bar_info;
    //return GetQbThemeMenuHeadPtr( qbtheme_ptr, &qbtheme_ptr->menu_bar_info, 0 );
}

/*****************************************************************************/
//  Description : 是否显示删除菜单图标
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbMenuIsItemEnableDeleted(
										MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr  //in
										)
{
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T  *item_info_ptr = PNULL;
    
    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }

    page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, qbtheme_ptr->cur_page_index);

    item_info_ptr = MMIMENU_GetItemInfo(
            page_item_info_ptr,
            qbtheme_ptr->qbtheme_menu_data.select_idx);

    if (PNULL != item_info_ptr)
    {
        if ( DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == item_info_ptr->group_id
			&& !MMIAPIUDISK_UdiskIsRun())
        {
            return TRUE;
        }
    }
#endif
    return FALSE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jin.wang
//  Note: 
/*****************************************************************************/
LOCAL void SaveQbThemeMenu(
                          MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                          BOOLEAN                       is_del_empty // 保存时是否删除空页
                          )
{
    if (is_del_empty)
    {
        //DeleteQbThemeMenuEmptyPage(qbtheme_ptr);
        QbThemeResetBarIndexRect(qbtheme_ptr, &qbtheme_ptr->qbtheme_menu_data, qbtheme_ptr->page_total_num);
    }

    // 校验当前页
    ValidateQbThemeMenuCurPage(qbtheme_ptr);

    SaveQbThemeMenuPageItemNum(qbtheme_ptr);
}

/*****************************************************************************/
//  Description : 保存菜单数据
//  Global resource dependence :
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveQbThemeMenuPageItemNum(
                                       MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                       )
{
    BOOLEAN result = FALSE;
    uint16 j = 0;
    uint16 item_num = 0;
    MMIMAINMENU_PAGE_ITEM_INFO_T static_page_info[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM] = {0};
//#ifdef DYNAMIC_MAINMENU_SUPPORT
//    MMIMAINMENU_PAGE_ITEM_INFO_T dymatic_page_info[MMITHEME_MENUMAIN_SLIDE_PAGE_NUM] = {0};
//#endif
    
    if (PNULL != qbtheme_ptr)
    {
        MMIMAINEMENU_ITEM_LIST_T    *page_item_info_ptr = PNULL;
        MMIMAINMENU_ITEM_INFO_T     *item_ptr      = PNULL;

        // 保存索引栏数据
        SCI_MEMSET(static_page_info, 0, sizeof(static_page_info));

        item_num = GetQbThemeMenuBarItemNum( qbtheme_ptr );

        page_item_info_ptr = GetQbThemeMenuBarHeadPtr(qbtheme_ptr);
        
        for (j = 0; j < page_item_info_ptr->item_count; j++)
        {
            item_ptr = MMIMENU_GetItemInfo(page_item_info_ptr, j);
            
            if (PNULL != item_ptr)
            {
                static_page_info[0].item_num++;
                static_page_info[0].menu_item_info[j].group_id = item_ptr->group_id;
                static_page_info[0].menu_item_info[j].menu_id = item_ptr->menu_id;
            }
        }
        
        //need save dy item here
#ifdef MMI_SAM_THEME
        if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif
        {
            MMINV_WRITE(MMINV_QBTHEME_DOCKBAR_INFO, static_page_info);
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jin.wang
//  Note: 
/*****************************************************************************/
LOCAL void ValidateQbThemeMenuCurPage(
                                    MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                    )
{
    if (0 == qbtheme_ptr->page_total_num)
    {
        qbtheme_ptr->cur_page_index = 0;
    }
    else if (qbtheme_ptr->cur_page_index >= qbtheme_ptr->page_total_num)
    {
        qbtheme_ptr->cur_page_index = qbtheme_ptr->page_total_num - 1;
    }
}

/*****************************************************************************/
//  Description : 获取一页显示的最大item number
//  Global resource dependence : 
//  Author: jin.wang
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetQbThemeMenuInPageMaxItemNum(
                                           MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                                           )
{
    uint16 page_item_num = 0;
    
    if (PNULL != qbtheme_ptr)
    {
        page_item_num = qbtheme_ptr->qbtheme_menu_data.page_col_num*qbtheme_ptr->qbtheme_menu_data.page_row_num;
    }
    
    return page_item_num;
}

/*****************************************************************************/
//  Description : 获得一页的item number
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetQbThemeMenuPageTotalItemNum(
											MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr,
											uint16                      page_index
											)
{
    uint16      page_item_num = 0;
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr = PNULL;

    if (PNULL == qbtheme_ptr)
    {
        return page_item_num;
    }

    page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, page_index);

    if (PNULL != page_item_info_ptr)
    {
        page_item_num = page_item_info_ptr->item_count;
    }

    return (page_item_num);
}

/*****************************************************************************/
//  Description : 获得bar的item number
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetQbThemeMenuBarItemNum(
                                     MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                     )
{
    uint16             total_num = 0;
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr = PNULL;
    
    if (PNULL == qbtheme_ptr)
    {
        return total_num;
    }
    
    page_item_info_ptr = GetQbThemeMenuBarHeadPtr(qbtheme_ptr);
    
    if (PNULL != page_item_info_ptr)
    {
        total_num = MIN( page_item_info_ptr->item_count, MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX );
    }
    
    return total_num;
}

/*****************************************************************************/
//  Description : 去掉主菜单 不显示的项
//  Global resource dependence :
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T* GetNextShownItem(
    MMI_LINK_NODE_T *cur_node_ptr
)
{
    BOOLEAN is_shown_item = FALSE;
    MMI_LINK_NODE_T* next_node_ptr = cur_node_ptr->next_ptr;
    MMIMAINMENU_ITEM_INFO_T  *item_ptr = PNULL;

    while (PNULL != next_node_ptr && next_node_ptr != cur_node_ptr)
    {
        item_ptr = (MMIMAINMENU_ITEM_INFO_T *)next_node_ptr->data;

#ifdef MMI_SAM_THEME
        if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
        {
            switch (item_ptr->menu_id)
            {
            case ID_MAINMENU_STYLE_SET:
            case ID_MAINMENU_QBTHEMEAPP:
            case ID_MAINMENU_QBTHEMEMORE:
                break;

            default:
                is_shown_item = TRUE;
                break;
            }
        }
        else
#endif
        {
            switch (item_ptr->menu_id)
            {
            case ID_MAINMENU_STYLE_SET:
                break;

            default:
                is_shown_item = TRUE;
                break;
            }
        }

        if (is_shown_item)
        {
            break;
        }
        else
        {
            next_node_ptr = next_node_ptr->next_ptr;
        }
    }

    return next_node_ptr;
}

/*****************************************************************************/
//  Description : 设置item
//  Global resource dependence :
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL uint16 GetValidItemInfo(
							  MMIMAINMENU_ITEM_INFO_T *item_page_ptr
							  )
{
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = PNULL;
    MMI_LINK_NODE_T          *node_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T  *item_ptr = PNULL;
	uint16 item_index = 0;

    if ( PNULL == item_page_ptr )
    {
        return 0;
    }

    // 拿原始item数据
    item_list_ptr = MMIMAINMENU_GetItemList();
    if (PNULL == item_list_ptr)
    {
        return 0;
    }

    node_ptr = item_list_ptr->list_head_ptr;

	for (item_index = 0; item_index < item_list_ptr->item_count; item_index++)
	{
        if (PNULL == node_ptr)
        {
            break;
        }

        item_ptr = (MMIMAINMENU_ITEM_INFO_T *)node_ptr->data;

		item_page_ptr[item_index].group_id = item_ptr->group_id;
		item_page_ptr[item_index].menu_id = item_ptr->menu_id;

        node_ptr = GetNextShownItem(node_ptr);
        if (PNULL == node_ptr || node_ptr == item_list_ptr->list_head_ptr)
        {
            break;
        }
	}

    return item_index + 1;
}

/*****************************************************************************/
//  Description : 根据Item索引号获得其在页面中的位置索引
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 如果不在页内,返回-1
/*****************************************************************************/
LOCAL int16 GetQbThemeMenuPageIndex(
                                   MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                   uint16           item_index
                                   )
{
    int16 page_index = -1;
    
    if (PNULL != qbtheme_ptr 
        && IsItemInQbThemeMenuPage(qbtheme_ptr, item_index, qbtheme_ptr->cur_page_index))
    {
        page_index = item_index;
    }
    
    return page_index;
}

/*****************************************************************************/
//  Description : 当前的Item是否在页中显示,因为当触笔移动滚动条的时候,
//                  当前Item可能会不在页面中显示
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemInQbThemeMenuPage(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                     uint16           item_index,
                                     uint16           page_index
                                     )
{
    uint16              page_item_num   = 0;
    BOOLEAN             result          = FALSE;
    
    if (PNULL != qbtheme_ptr)
    {
        page_item_num = qbtheme_ptr->qbtheme_menu_data.page_col_num * qbtheme_ptr->qbtheme_menu_data.page_row_num;
        
        if (page_item_num > item_index)
        {
            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetQbThemeMenuMoveItemInfo(
                                       MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                       )
{
    BOOLEAN result = FALSE;
    
    if (PNULL != qbtheme_ptr && qbtheme_ptr->is_move_state) // 记录当前和起始位置
    {
        result = TRUE;
        qbtheme_ptr->tmp_cur_item_index = qbtheme_ptr->cur_item_index;
        qbtheme_ptr->tmp_first_item_index = qbtheme_ptr->first_item_index;
        qbtheme_ptr->tmp_page_item_index = qbtheme_ptr->cur_page_index;
        qbtheme_ptr->tmp_is_bar_item  = qbtheme_ptr->is_bar_item;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 重新设置移动时的索引号信息
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetQbThemeMenuMoveItemInfo(
                                         MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                         )
{
    BOOLEAN result = FALSE;
    
    if (PNULL != qbtheme_ptr && qbtheme_ptr->is_move_state) // 记录当前和起始位置
    {
        result = TRUE;
        qbtheme_ptr->cur_item_index = qbtheme_ptr->tmp_cur_item_index;
        qbtheme_ptr->first_item_index = qbtheme_ptr->tmp_first_item_index;
        qbtheme_ptr->cur_page_index = qbtheme_ptr->tmp_page_item_index;
        qbtheme_ptr->is_bar_item = qbtheme_ptr->tmp_is_bar_item;
    }
    
    return result;
}

/*****************************************************************************/
// Description : get item info in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *GetMenuCurrentItemInfoPtr(
                                                         MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                                         )
{
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T* item_info_ptr = PNULL;

    page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, qbtheme_ptr->cur_page_index);
    if ( PNULL != page_item_info_ptr ) 
    {
        item_info_ptr = MMIMENU_GetItemInfo(
            page_item_info_ptr,
            qbtheme_ptr->qbtheme_menu_data.select_idx);
    }

    return item_info_ptr;
}

/*****************************************************************************/
//  Description : handle menu modify rect
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void QbThemeMenuModifyRect(
                                MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                )
{
    //set icon menu current index
    MMITHEME_GetQbthemeMenuInfo( qbtheme_ptr->win_handle, &qbtheme_ptr->qbtheme_menu_data);
    QbThemeResetBarIndexRect(qbtheme_ptr, &qbtheme_ptr->qbtheme_menu_data, qbtheme_ptr->page_total_num);

    // title图片显示位置
    ReleaseQbThemeMenuIndexBarLayer(qbtheme_ptr);
    CreateQbThemeMenuIndexBarLayer(qbtheme_ptr);

    // 自动切换的区域需要重新设置
    ResetQbThemeMenuAutoRect(qbtheme_ptr);
}

/*****************************************************************************/
//  Description : get slide client rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL GUI_RECT_T AdjustSlideMenuRect(
                                     MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                                     )
{
    GUI_RECT_T menu_rect = {0};
    
    menu_rect           = MMITHEME_GetClientRectEx(qbtheme_ptr->win_handle);
    //menu_rect.top      += qbtheme_ptr->main_menu_data.slide_menu_data.top_margin;    
    //menu_rect.bottom   -= qbtheme_ptr->main_menu_data.slide_menu_data.bottom_margin;
    //menu_rect.left     += qbtheme_ptr->main_menu_data.slide_menu_data.left_margin;
    //menu_rect.right    -= qbtheme_ptr->main_menu_data.slide_menu_data.right_margin;
    
    return menu_rect;
}

/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence : 
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetCurrentPageLyaerInfo(
                                               MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                               )
{
    GUI_LCD_DEV_INFO lcd_dev_info = qbtheme_ptr->lcd_dev_info;

    if (UILAYER_IsMultiLayerEnable() 
        && UILAYER_IsLayerActive(&qbtheme_ptr->page_layer_handle[0]))
    {
        lcd_dev_info = qbtheme_ptr->page_layer_handle[0];
    }

    return lcd_dev_info;
}

/*****************************************************************************/
//  Description : 获取组织功能时动画特效初始区域
//  Global resource dependence : 
//  Author: jin,wang
//  Note:
/*****************************************************************************/
LOCAL void GetInitRect(
                       MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                       GUI_RECT_T *delete_rect_ptr,
                       GUI_RECT_T *to_idle_rect_ptr,
                       BOOLEAN is_need_delete,
                       uint16 lcd_width,
                       uint16 lcd_height
                       )
{    
    uint16        i                = 0;
    uint16        item_width        = 0;

    uint16        margin            = 0;
    uint16        max_page_cnt    = 0;
    uint16        page_row_num    = 0;    
    
    GUI_RECT_T    *item_rect_ptr = PNULL;
    GUI_RECT_T    menu_rect        = {0};
    GUI_RECT_T    item_rect1[MMITHEME_QBMENU_PAGE_ROW_NUM] = {0};
    uint16        item_height        = 0;
    uint16        page_col_num    = 0;
    GUI_RECT_T    bar_rect        = {0};
    BOOLEAN        is_get            = FALSE;

    if ((PNULL == qbtheme_ptr)||(PNULL == delete_rect_ptr)||(PNULL == to_idle_rect_ptr))
    {
        return;
    }
    
    menu_rect = MMITHEME_GetClientRectEx( qbtheme_ptr->win_handle);

    margin = MMITHEME_QBMENU_LEFT_RIGHT_MARGIN;

    page_col_num = qbtheme_ptr->qbtheme_menu_data.page_col_num;
    page_row_num = qbtheme_ptr->qbtheme_menu_data.page_row_num;
    item_width = qbtheme_ptr->qbtheme_menu_data.item_rect[0].right
        - qbtheme_ptr->qbtheme_menu_data.item_rect[0].left + 1;

    item_rect_ptr = &item_rect1; /*lint !e64*/

    item_height = (menu_rect.bottom - menu_rect.top + 1 - MMITHEME_QBTHEMEMENU_INDEX_HEIGHT)/(page_col_num + 1);
    
    bar_rect = menu_rect;
    bar_rect.top = bar_rect.bottom - item_height - MMITHEME_QBTHEMEMENU_INDEX_HEIGHT + 1;
    
    menu_rect.bottom = bar_rect.top - 1;

    if (item_width > lcd_width || PNULL == item_rect_ptr) /*lint !e774*/
    {
        return;
    }
    else
    {

        menu_rect.left += margin;
        menu_rect.right -= margin;
        
        GUI_CreateMatrixRect(&menu_rect, 0, 0, 0, 0, page_row_num, 1, item_rect_ptr);
        
//         for ( i = 0; i < page_row_num; i++ )/*lint !e681*/
//         {
//             item_rect_ptr[i] = GUI_GetCenterRect( item_rect_ptr[i], lcd_width, MENUICON_HEIGHT + MENUTEXT_HEIGHT );
//         }
        
        for (i = 0; i < page_row_num; i++)/*lint !e681*/
        {
            if (GUI_PointIsInRect(qbtheme_ptr->pre_tp_point, item_rect_ptr[i]))
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
// Description:  是否拖动到主菜单/删除
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInToIdleRect(
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                              BOOLEAN is_handle_tp,
                              BOOLEAN *is_need_redraw_ptr
                              )
{
    BOOLEAN is_need_delete = FALSE;
    BOOLEAN is_disp_to_idle = FALSE;
    BOOLEAN is_disp_delete = FALSE;
    
    // 判断是不是可以删除
    is_need_delete = QbMenuIsItemEnableDeleted(qbtheme_ptr);
       
    // 判断当前move的区域与toidle bar的区域是否有重合
    is_disp_to_idle = IsTpInToidleRect(qbtheme_ptr);

    if (is_need_delete)
    {
        // 判断当前move的区域与deleteitem bar是否有从何
        is_disp_delete = IsTpInDeleteitemRect(qbtheme_ptr);
    }

    //是否处理tp tp消息
    if (!is_handle_tp)
    {
        // 显示toidle bar
        DisplayToIdleBar(qbtheme_ptr);

        if (is_need_delete)
        {
            // 显示delitem bar
            DisplayDelItemBar(qbtheme_ptr);
        }

        MMITHEME_UpdateRect();
    }
#ifdef DYNAMIC_MAINMENU_SUPPORT
    else if (is_disp_delete)//需要删除此应用
    {
        do 
        {
            MMIMAINMENU_ITEM_INFO_T* item_info_ptr = PNULL;
            CTRLMAINMENU_ITEM_T menuitem_info = {0};

            UILAYER_Clear(&qbtheme_ptr->icon_move_layer_handle);

            item_info_ptr = GetMenuCurrentItemInfoPtr(qbtheme_ptr);
            
            if ( PNULL == item_info_ptr ) 
                break;

            if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID != item_info_ptr->group_id) 
                break;

            MMIMAINMENU_GetItemData(item_info_ptr->menu_id, item_info_ptr->group_id, &menuitem_info);
            
            //if ((GUIMENU_MAINMENU_DATA_TYPE_DYNAMIC == menuitem_info.)
            //    && (PNULL != menuitem_info.menu_data.dynamic_menu.ori_data_ptr))
            //{
                qbtheme_ptr->delete_menu_id = item_info_ptr->menu_id;

                OpenQueryDeleteWin();
                
                if (PNULL != is_need_redraw_ptr)
                {
                    *is_need_redraw_ptr = FALSE;
                }
            //}
        } while(0);
    }
#endif

    return is_disp_to_idle;
}

/*****************************************************************************/
//  Description : 判断tp是否在删除区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInDeleteitemRect(
                                    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                    )
{
    BOOLEAN       is_disp_delete = FALSE;
    GUI_RECT_T move_rect = {0};
    GUI_RECT_T clip_rect = {0};
    GUI_RECT_T delete_rect = {0};

    move_rect = GetQbThemeMenuMoveLayerRect(qbtheme_ptr);

    delete_rect = UILAYER_GetLayerRect(&qbtheme_ptr->delete_layer_handle);
        
    if (GUI_IntersectRect(&clip_rect, move_rect, delete_rect))
    {
        is_disp_delete = TRUE;
    }

    return is_disp_delete;
}

/*****************************************************************************/
//  Description : 判断tp是否在toidle区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInToidleRect(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                                )
{
    BOOLEAN       is_disp_to_idle = FALSE;
    GUI_RECT_T move_rect = {0};
    GUI_RECT_T clip_rect = {0};
    GUI_RECT_T to_idle_rect = {0};

	if (0 == qbtheme_ptr->to_idle_layer_handle.block_id)
	{
        return FALSE;
	}

    move_rect = GetQbThemeMenuMoveLayerRect(qbtheme_ptr);

    to_idle_rect = UILAYER_GetLayerRect(&qbtheme_ptr->to_idle_layer_handle);
    
    if (GUI_IntersectRect(&clip_rect, move_rect, to_idle_rect))
    {
        is_disp_to_idle = TRUE;
    }

    return is_disp_to_idle;
}

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 删除动态菜单项
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DeleteDynamicItem(void)
{
    MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr = PNULL;
    
    qbtheme_ptr = (MMIMAINMENU_QBTHEME_DATA_T *)GUIMAINMENU_GetPM(MAINMENUWIN_MENU_CTRL_ID);

    if (PNULL != qbtheme_ptr)
    {
        CTRLMAINMENU_ITEM_T menuitem_info = {0};
        
        MMIMAINMENU_GetItemData(
            qbtheme_ptr->delete_menu_id,
            DYNAMIC_MAINMENU_DEFAULT_GROUP_ID,
            &menuitem_info);

        //判断此要删除的item信息是否正确
        if (0 != menuitem_info.group_id
            && PNULL != menuitem_info.call_back_delete)
        {                
            GUIMAINMENU_DY_ITEM_T item_info = {0};
            BOOLEAN is_info_get = FALSE;
            BOOLEAN is_delete = FALSE;
            
            is_info_get = MMIMENU_DySlideGetRunInfo(qbtheme_ptr->delete_menu_id, &item_info);
            
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
				qbtheme_ptr->is_delete_state = TRUE;
                MMIMENU_DyDeleteItemEx(&menuitem_info);
            }
        }
    }
}

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
        DeleteDynamicItem();
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
LOCAL void OpenQueryDeleteWin(void)
{
    MMI_WIN_ID_T query_win_id = MAIN_MAINMENU_QBTHEME_QUERY_WIN_ID;
    
    MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, 
        IMAGE_PUBWIN_QUERY,  
        &query_win_id, 
        HandleQbthemeQueryWinMsg);
    
    return;
}
#endif

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void StartItemExchangeTimer(
                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                  )
{
    if (PNULL != qbtheme_ptr && 0 == qbtheme_ptr->user_timer_id)
    {
        qbtheme_ptr->user_timer_id = MMK_CreateWinTimer(
            qbtheme_ptr->ctrl_handle, 
            qbtheme_ptr->main_menu_theme.item_exchange_period,
            FALSE );
    }
}

/*****************************************************************************/
//  Description : stop item timer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void StopItemExchangeTimer(
                                 MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                 )
{
    if (PNULL != qbtheme_ptr && 0 != qbtheme_ptr->user_timer_id)
    {
        MMK_StopTimer(qbtheme_ptr->user_timer_id);
        qbtheme_ptr->user_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStartShakeIconTimer(
                                MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                )
{
    if (PNULL != qbtheme_ptr && 0 == qbtheme_ptr->shake_icon_timer_id)
    {
        qbtheme_ptr->shake_icon_timer_id = MMK_CreateWinTimer(
                    qbtheme_ptr->ctrl_handle, 
                    qbtheme_ptr->main_menu_theme.icon_shake_period,
                    FALSE);
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStopShakeIconTimer(
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                              )
{
    if (PNULL != qbtheme_ptr && 0 != qbtheme_ptr->shake_icon_timer_id)
    {
        MMK_StopTimer(qbtheme_ptr->shake_icon_timer_id);
        qbtheme_ptr->shake_icon_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStartMoveTimer(
                               MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                               )
{
    if (PNULL != qbtheme_ptr && 0 == qbtheme_ptr->start_move_timer_id)
    {
        qbtheme_ptr->start_move_timer_id = MMK_CreateWinTimer(
            qbtheme_ptr->ctrl_handle, 
            qbtheme_ptr->main_menu_theme.icon_move_start_period,
            FALSE );
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStopMoveTimer(
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                              )
{
    if (PNULL != qbtheme_ptr && 0 != qbtheme_ptr->start_move_timer_id)
    {
        MMK_StopTimer(qbtheme_ptr->start_move_timer_id);
        qbtheme_ptr->start_move_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStartRedrawTimer(
                            MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                            )
{
    if (PNULL != qbtheme_ptr && 0 == qbtheme_ptr->redrew_timer_id)
    {
        qbtheme_ptr->redrew_timer_id = MMK_CreateWinTimer(
            qbtheme_ptr->ctrl_handle, 
            MMITHEME_MENU_REDRAW_TIME,
            FALSE );
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeStopRedrawTimer(
                           MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                           )
{
    if (PNULL != qbtheme_ptr && 0 != qbtheme_ptr->redrew_timer_id)
    {
        MMK_StopTimer(qbtheme_ptr->redrew_timer_id);
        qbtheme_ptr->redrew_timer_id = 0;
    }
}

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : stop item text timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StartHighlightIconDispTimer(
                                        MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                        )
{
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;

    mainmenu_select_style = MMIAPISET_GetMainmenuSelectStyle();  /*lint !e718*//*lint !e746*//*lint !e628*/

    if (SELECT_EFFECT_STATIC != mainmenu_select_style
        &&(mainmenu_select_style<SELECT_EFFECT_MAX))
    {
        if (PNULL != qbtheme_ptr && 0 == qbtheme_ptr->highlight_timer_id)
        {
            qbtheme_ptr->highlight_timer_id = MMK_CreateWinTimer(
                qbtheme_ptr->ctrl_handle, 
                GO_HIGHLIGHT_DISPLAY_TIMER_OUT,
                FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : stop Highlight timer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopHighlightIconDispTimer(
                                   MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
                                   )
{
    if (PNULL != qbtheme_ptr && 0 != qbtheme_ptr->highlight_timer_id)
    {
        MMK_StopTimer(qbtheme_ptr->highlight_timer_id);
        qbtheme_ptr->highlight_timer_id = 0;
    }
}

#endif

/*****************************************************************************/
//  Description : StartInitItemTimer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StartInitItemTimer(
    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
)
{
#define MMIMENU_STARTINITITEM_TIMER 5

    MMK_StopTimer(qbtheme_ptr->init_item_timer_id);

    qbtheme_ptr->init_item_timer_id = MMK_CreateWinTimer(
                                            qbtheme_ptr->ctrl_handle, 
                                            MMIMENU_STARTINITITEM_TIMER,
                                            FALSE );
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeTimer(
									  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
									  MMI_MESSAGE_ID_E              msg_id,  //事件类型
									  DPARAM                        param          //附带的处理参数
									  )
{
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    if (qbtheme_ptr->redrew_timer_id == *(uint8*)param)
    {
        GUI_POINT_T  point = {0};
        MMI_TP_STATUS_E state = MMI_TP_NONE;
#ifdef TOUCH_PANEL_SUPPORT        
        MMK_GetLogicTPMsg(&state, &point);
#endif
        //SCI_TRACE_LOW:"[GO_MENU]_HandleSlidePageMenuTimer current time  = %d "
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_4532_112_2_18_2_35_39_218,(uint8*)"d", SCI_GetTickCount());

        QbThemeStopRedrawTimer(qbtheme_ptr);
        if (qbtheme_ptr->is_moving)
        {
            return result;
        }

        QbThemeStartRedrawTimer(qbtheme_ptr);
        if ((abs(point.x - qbtheme_ptr->slide_pre_point.x) > 1
                &&GO_SLIDE_SUPPORT_ONLY_X == qbtheme_ptr->support_slide_type)
                ||(abs(point.y - qbtheme_ptr->slide_pre_point.y) > 1
                &&GO_SLIDE_SUPPORT_ONLY_Y == qbtheme_ptr->support_slide_type)
                )
        {
            if(MMK_TP_SLIDE_TP_SCROLL == qbtheme_ptr->slide_state
            &&(GO_SLIDE_SUPPORT_ONLY_X==qbtheme_ptr->support_slide_type))
            {
                MoveGOMenuPageLayer_X(qbtheme_ptr, &point);

                qbtheme_ptr->slide_pre_point.x = point.x;
                qbtheme_ptr->slide_pre_point.y = point.y;
            }
            else if(MMK_TP_SLIDE_TP_SCROLL == qbtheme_ptr->slide_state
            &&(GO_SLIDE_SUPPORT_ONLY_Y==qbtheme_ptr->support_slide_type))
            {
                MoveGOMenuPageLayer_Y(qbtheme_ptr, &point);

                qbtheme_ptr->slide_pre_point.x = point.x;
                qbtheme_ptr->slide_pre_point.y = point.y;
            }
            else if(MMK_TP_SLIDE_TP_SCROLL == qbtheme_ptr->slide_state
            &&(GO_SLIDE_SUPPORT_XY==qbtheme_ptr->support_slide_type))
            {
                //根据距离计算
            }
        }
    }
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    else if (qbtheme_ptr->highlight_timer_id == *(uint8*)param)
    {
        StopHighlightIconDispTimer(qbtheme_ptr);
        
        if(qbtheme_ptr->is_active)
        {
            //if(statusbar_ctrl_ptr==PNULL)
            //{
            // DisplayQbThemeActiveIcon(qbtheme_ptr,TRUE);
            //return MMI_RESULT_TRUE;
            //}
            //if(!statusbar_ctrl_ptr->is_press_down)
            {
                DisplayQbThemeActiveIcon(qbtheme_ptr,TRUE);
            }
            //else
            {
                //BOOLEAN is_active = button_ctrl_ptr->is_active;
                //StartHighlightIconDispTimer(qbtheme_ptr);
            }
        }
        else
        {
            //BOOLEAN is_focus = qbtheme_ptr->is_active;
            //StartHighlightIconDispTimer(qbtheme_ptr);
        }
    }
#endif
    else if(qbtheme_ptr->init_item_timer_id == *(uint8*)param)
    {
        MMK_StopTimer(qbtheme_ptr->init_item_timer_id);
        qbtheme_ptr->init_item_timer_id = 0;
        
        //初始化三页内容放在显示后面，加快进入速度
        InitQbThemeItemData(qbtheme_ptr);
    }
    else if (qbtheme_ptr->tp_timer_id == *(uint8*)param)
    {
        switch(qbtheme_ptr->continue_move_type)
        {
        case CONTINUE_MOVE_LEFT:
            if (qbtheme_ptr->cur_page_index > 0)
            {
                //HandleQbThemeMenuAutoMove(qbtheme_ptr, qbtheme_ptr->continue_move_type, TRUE);
            }
            break;

        case CONTINUE_MOVE_RIGHT:
            if (qbtheme_ptr->cur_page_index < qbtheme_ptr->page_total_num - 1)
            {
                //HandleQbThemeMenuAutoMove(qbtheme_ptr, qbtheme_ptr->continue_move_type, TRUE);
            }
            break;

        default:
            result = MMI_RESULT_FALSE;
            //SCI_TRACE_LOW:"HandleTimer: the move_type is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_QBTHEME_5587_112_2_18_2_34_48_170,(uint8*)"");
            break;
        }

        if (!result)
        {
            qbtheme_ptr->continue_move_type = CONTINUE_MOVE_INVALID;
        }
    }
    else if (qbtheme_ptr->user_timer_id == *(uint8*)param)
    {
        HandleQbThemeItemExchange( qbtheme_ptr );
        DisplayQbThemeBar(qbtheme_ptr, qbtheme_ptr->cur_page_index, qbtheme_ptr->titleidx_layer_handle, FALSE);
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle icon menu up key
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeMenuCancelKey(
                                        MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                        )
{
    BOOLEAN is_handled = FALSE;

    if (qbtheme_ptr->is_move_state)
    {
        InitQbThemeMenu(qbtheme_ptr);
            
        // 释放层
        ReleaseQbThemeMoveIconLayer(qbtheme_ptr);

        // 设置状态
        qbtheme_ptr->is_move_state = FALSE;
        qbtheme_ptr->is_moving = FALSE;

        is_handled = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"HandleQbThemeMenuCancelKey not move state!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_QBTHEME_5707_112_2_18_2_34_48_171,(uint8*)"");
    }

    return is_handled;
}

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidItemIndex(
                               uint16                       num_index,
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                               )
{
    BOOLEAN             result = FALSE;
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    
    page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, qbtheme_ptr->cur_page_index);
    
    if (PNULL != page_item_info_ptr)
    {
        //set current item index
        if (num_index < page_item_info_ptr->item_count)
        {
            qbtheme_ptr->cur_item_index = num_index;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleRandomStyle(
                            MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                            )
{
    if(GO_MENU_STYLE_RANDOM== MMIAPISET_GetGOmenuStyle())
    {        
        do
        {
            s_random_cur_style = rand()%GO_MENU_STYLE_MAX;
        } while((s_random_cur_style == GO_MENU_STYLE_RANDOM)
            ||(UAPIMenu_GetCalcStyleSlideType(s_random_cur_style) == GO_SLIDE_SUPPORT_ONLY_Y));

        qbtheme_ptr->support_slide_type = UAPIMenu_GetCalcStyleSlideType(s_random_cur_style);
    }
}

/*****************************************************************************/
//  Description : handle page touch panel press down msg
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeTpDown(
                              GUI_POINT_T                   *tp_point_ptr,
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                              )
{
    uint16              i = 0;
    uint16              total_num = 0;
    GUI_RECT_T          disp_rect = {0};
    GUI_RECT_T          item_rect = {0};
    uint16              item_index = 0;
    BOOLEAN             is_find = FALSE;
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;
    
    if ((PNULL == tp_point_ptr)||(PNULL == qbtheme_ptr))
    {
        return;
    }
    s_undraw_index = 0xFFFF;
    qbtheme_ptr->is_tp_pressed = TRUE;
    qbtheme_ptr->is_bar_item = FALSE;
    qbtheme_ptr->qbtheme_menu_data.is_disp_anim = FALSE;

    // 增加速度点
    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );

    //get title total number
    total_num = qbtheme_ptr->page_total_num;

    // 停止timer
    QbThemeStopMoveTimer(qbtheme_ptr);

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    StopHighlightIconDispTimer(qbtheme_ptr);
#endif

    // 首先设置页面的滑动相关数据
    // 记录起始点
    // 记录前一个点
    qbtheme_ptr->slide_start_point = qbtheme_ptr->slide_pre_point = qbtheme_ptr->pre_tp_point = *tp_point_ptr;

    // 下面检查tp是否在item区域，并作相应处理    
    page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, qbtheme_ptr->cur_page_index);
    
    if (PNULL == page_item_info_ptr)
    {
        total_num = 0;
    }
    else
    {
        //get item total number
        total_num = page_item_info_ptr->item_count;//GetQbThemeMenuPageItemNum(qbtheme_ptr, page_item_info_ptr->item_total_num);
    }
    
    //if press item icon
    for (i = 0; i < total_num; i++)
    {
        item_index = i;//GetQbThemeMenuItemIndex(qbtheme_ptr, i);
        
        item_rect = qbtheme_ptr->qbtheme_menu_data.item_rect[i];
        
        //if press item
        if (GUI_PointIsInRect(*tp_point_ptr,item_rect))
        {
            disp_rect = item_rect;
            qbtheme_ptr->tmp_width = tp_point_ptr->x - disp_rect.left;
            qbtheme_ptr->tmp_height = tp_point_ptr->y - disp_rect.top;
            
            qbtheme_ptr->is_item_pressed = TRUE;
            qbtheme_ptr->is_bar_item = FALSE;
            qbtheme_ptr->move_item_index = qbtheme_ptr->cur_item_index = item_index;
			qbtheme_ptr->qbtheme_menu_data.select_idx = item_index;

            DisplayQbThemeActiveIcon(qbtheme_ptr, TRUE);

            is_find = TRUE;
            break;
        }
    }
    
    if ( !qbtheme_ptr->is_item_pressed )
    {
        GUI_RECT_T bar_rect[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX] = {0};
        
        total_num = GetQbThemeMenuBarItemNum( qbtheme_ptr );
        
        GetDockBarItemRect( qbtheme_ptr, bar_rect, total_num );
        
        //if press item icon
        for (i = 0; i < total_num; i++)
        {
            item_index = i;
            item_rect = bar_rect[i];
            
            //if press item
            if (GUI_PointIsInRect(*tp_point_ptr,item_rect))
            {
                disp_rect = item_rect;
     
                qbtheme_ptr->tmp_width = tp_point_ptr->x - disp_rect.left;
                qbtheme_ptr->tmp_height = tp_point_ptr->y - disp_rect.top;
                
                qbtheme_ptr->is_item_pressed = TRUE;
                qbtheme_ptr->is_bar_item = TRUE;
                
                qbtheme_ptr->cur_item_index = item_index;
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY                                
                GUI_WeekLayer(&qbtheme_ptr->titleidx_layer_handle, &disp_rect, MMIMENU_WEAKLAYERALPHA);
                GUILCD_InvalidateRect(0, disp_rect, 0);
#endif
                is_find = FALSE;
                break;
            }
        }
    }
    
    if ( qbtheme_ptr->is_item_pressed )
    {
        // 不是移动图标状态,启动Timer,dockbar上的item不支持移动
        if (qbtheme_ptr->main_menu_theme.is_used_org && !qbtheme_ptr->is_move_state
            && is_find)
        {
            QbThemeStartMoveTimer(qbtheme_ptr);
        }
    }

    if ( !GUI_PointIsInRect( qbtheme_ptr->slide_start_point, qbtheme_ptr->qbtheme_menu_data.bar_rect ))
    {
        // 不是移动图标状态,启动Timer
        if (!qbtheme_ptr->is_move_state || !qbtheme_ptr->is_item_pressed)
        {
            QbThemeStartRedrawTimer(qbtheme_ptr);
        }
    }

	HandleRandomStyle(qbtheme_ptr);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeAutoMoveEvent(
                                        GUI_POINT_T                 *point_ptr, 
                                        MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                        )
{
    BOOLEAN result = TRUE;
    
    if ((PNULL == point_ptr)||(PNULL == qbtheme_ptr))
    {
        return FALSE;
    }
    
    if (GUI_PointIsInRect(*point_ptr, s_left_direct_move_rect))
    {
        qbtheme_ptr->continue_move_type = CONTINUE_MOVE_LEFT;
    }
    else if (GUI_PointIsInRect(*point_ptr, s_right_direct_move_rect))
    {
        qbtheme_ptr->continue_move_type = CONTINUE_MOVE_RIGHT;
    }
    else
    {
        result = FALSE;
    }

    if ( !result )
    {
        qbtheme_ptr->continue_move_type = CONTINUE_MOVE_INVALID;
    }
        
    return result;
}

/*****************************************************************************/
//  Description : handle icon menu ok
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeMenuMove(
                                   MMIMAINMENU_QBTHEME_DATA_T  *qbtheme_ptr
                                   )
{
    BOOLEAN result = TRUE;
    GUIMENU_ITEM_BUF_T item_layer = {0};
    MMI_LINK_NODE_T *pre_item_node_ptr = PNULL;
    MMI_LINK_NODE_T *cur_item_node_ptr = PNULL;
    MMIMAINEMENU_ITEM_LIST_T *cur_menu_list_ptr = PNULL;
    MMIMAINEMENU_ITEM_LIST_T *pre_menu_list_ptr = PNULL;

    if (qbtheme_ptr->is_moving && qbtheme_ptr->is_move_state)
    {
        if (!qbtheme_ptr->is_bar_item && !qbtheme_ptr->tmp_is_bar_item)
        {
            return FALSE;
        }

		if ( qbtheme_ptr->tmp_is_bar_item )   //item原属于bar
		{
			// 获得移动前的页
			pre_menu_list_ptr = GetQbThemeMenuBarHeadPtr( qbtheme_ptr );
		}
		else        //item原属于宫格
        {
            // 获得移动前的页
            pre_menu_list_ptr = MMIMENU_GetItemList(
                &qbtheme_ptr->menu_page_info,
                qbtheme_ptr->tmp_page_item_index);
        }

        // 获得被移动的item指针
        pre_item_node_ptr = MMIMENU_GetItemInfoNode(pre_menu_list_ptr, qbtheme_ptr->tmp_cur_item_index);

        if ( qbtheme_ptr->is_bar_item )   //当前移动到bar上
        {
            // 获得当前页的列表
            cur_menu_list_ptr = GetQbThemeMenuBarHeadPtr(qbtheme_ptr);
        }
		else        //当前移动到宫格上
		{
			// 获得当前页的列表
			cur_menu_list_ptr = MMIMENU_GetItemList(
			    &qbtheme_ptr->menu_page_info,
			    qbtheme_ptr->cur_page_index);
		}

        // 获取目的地item指针
        cur_item_node_ptr = MMIMENU_GetItemInfoNode(cur_menu_list_ptr, qbtheme_ptr->cur_item_index);

        if (PNULL != pre_item_node_ptr)
        {
            // 处理在当前页移动的情况
            if ( qbtheme_ptr->cur_page_index == qbtheme_ptr->tmp_page_item_index
                && qbtheme_ptr->is_bar_item == qbtheme_ptr->tmp_is_bar_item )
            {
                if (qbtheme_ptr->cur_item_index != qbtheme_ptr->tmp_cur_item_index ) // 相等的时候无变化
                {
                    if (PNULL != pre_menu_list_ptr && PNULL != cur_menu_list_ptr && PNULL != cur_item_node_ptr)
                    {
                        item_layer = DeleteQbThemeMenuItemLayer(
                                qbtheme_ptr, qbtheme_ptr->cur_page_index, 
                                qbtheme_ptr->tmp_cur_item_index,
                                qbtheme_ptr->tmp_is_bar_item ); // 取出来 

                        // 首先删除之前的节点
                        pre_menu_list_ptr->list_head_ptr = MMILINK_RemoveNode(
                                pre_item_node_ptr, pre_menu_list_ptr->list_head_ptr);

                        pre_menu_list_ptr->item_count--;

                        // 添加节点到新的位置
                        if (qbtheme_ptr->cur_item_index > qbtheme_ptr->tmp_cur_item_index)
                        {
                            MMILINK_AddNodeAfterBaseNode(cur_item_node_ptr, pre_item_node_ptr);
                            cur_menu_list_ptr->item_count++;
                        }
                        else
                        {
                            MMILINK_AddNodeBeforeBaseNode(cur_item_node_ptr, pre_item_node_ptr);
                            cur_menu_list_ptr->item_count++;
                            
                            // 改变头位置
                            if (cur_menu_list_ptr->list_head_ptr == cur_item_node_ptr)
                            {
                                cur_menu_list_ptr->list_head_ptr= pre_item_node_ptr;
                            }
                        }

                        qbtheme_ptr->qbtheme_menu_data.is_need_save = TRUE;
                        
                        qbtheme_ptr->tmp_cur_item_index = qbtheme_ptr->cur_item_index;
                    }
                }
                else
                {
                    result = FALSE;
                }
            }
            else  //处理跨页移动的情况
            {
                if ( PNULL != cur_menu_list_ptr)
                {
                    if ( qbtheme_ptr->is_bar_item )
                    {
                        // DockBar里的个数已经满了，不能再添加
                        if ( cur_menu_list_ptr->item_count >= MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX )
                        {
                            result = FALSE;
                        }
                    }
                }
                
                if (result)
                {
                    if (PNULL != pre_menu_list_ptr && PNULL != cur_menu_list_ptr )
                    {
                        // layer数组进行除旧迎新
                        item_layer = DeleteQbThemeMenuItemLayer(
                                qbtheme_ptr, qbtheme_ptr->tmp_page_item_index, 
                                qbtheme_ptr->tmp_cur_item_index,
                                qbtheme_ptr->tmp_is_bar_item );
                        
                        qbtheme_ptr->qbtheme_menu_data.is_need_save = TRUE;

						if (!qbtheme_ptr->is_bar_item)
						{
							// 首先删除之前的节点
							// TODO: delete ???
							pre_menu_list_ptr->list_head_ptr = MMILINK_RemoveNode(
							        pre_item_node_ptr, pre_menu_list_ptr->list_head_ptr);

							pre_menu_list_ptr->item_count--;
                            qbtheme_ptr->tmp_cur_item_index = qbtheme_ptr->move_item_index;
							return TRUE;
						}

                        if (PNULL == cur_menu_list_ptr->list_head_ptr
                            || 0 == cur_menu_list_ptr->item_count) // 当前页没有item
                        {
                            cur_menu_list_ptr->list_head_ptr = pre_item_node_ptr;
                            qbtheme_ptr->cur_item_index = 0;
                            qbtheme_ptr->tmp_cur_item_index = qbtheme_ptr->cur_item_index;
                            cur_menu_list_ptr->item_count = 1;
                            pre_item_node_ptr->next_ptr = pre_item_node_ptr->prev_ptr = pre_item_node_ptr;
                        }
                        else
                        {
                            MMIMAINMENU_ITEM_INFO_T *item_info_ptr = {0};
                            MMIMAINMENU_ITEM_INFO_T *new_item_info_ptr = PNULL;
                            MMI_LINK_NODE_T         *tmp_node_ptr = PNULL;

                            // 获取item数据
                            item_info_ptr = (MMIMAINMENU_ITEM_INFO_T*)(pre_item_node_ptr->data);

                            // 创建新的item数据
                            new_item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));
                            SCI_MEMCPY(new_item_info_ptr, item_info_ptr, sizeof(MMIMAINMENU_ITEM_INFO_T));

                            // 创建item节点
                            tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_info_ptr);

							if (qbtheme_ptr->is_empty_space) // 如果在空区域，则要增加在末尾
							{
								MMILINK_AddNodeBeforeBaseNode(cur_menu_list_ptr->list_head_ptr, tmp_node_ptr); //添加到链尾
								cur_menu_list_ptr->item_count++;
								
								qbtheme_ptr->cur_item_index = cur_menu_list_ptr->item_count - 1; // 改变当前的索引
								
								qbtheme_ptr->tmp_cur_item_index = qbtheme_ptr->cur_item_index;
                            }
                            else if (PNULL != cur_item_node_ptr)
                            {
                                MMILINK_AddNodeBeforeBaseNode(cur_item_node_ptr, tmp_node_ptr);
                                cur_menu_list_ptr->item_count++;
                                
                                if (0 == qbtheme_ptr->cur_item_index) // 如果是头部，则改变链头
                                {
                                    cur_menu_list_ptr->list_head_ptr= tmp_node_ptr;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            result = FALSE;
        }

        qbtheme_ptr->is_empty_space = FALSE; // 移动结束 
    }
    else
    {
        result = FALSE;
        //SCI_TRACE_LOW:"HandleIStyleMenuMove!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_QBTHEME_6655_112_2_18_2_34_50_174,(uint8*)"");
    }
    
    return result;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL GUIMENU_ITEM_BUF_T DeleteQbThemeMenuItemLayer(
                                                   MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr, 
                                                   uint16                       page_index, 
                                                   uint16                       item_index,
                                                   BOOLEAN                      is_bar_item
                                                   )
{
    GUIMENU_ITEM_BUF_T item_buf = {0};
    return item_buf;
}

/*****************************************************************************/
//  Description : handle item exchange
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeItemExchange(
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                              )
{
    int32       i = 0;
    uint16      item_index = 0;
    uint16      page_item_num = 0;
    GUI_RECT_T  item_rect = {0};
    BOOLEAN     is_bar_item = FALSE;
    GUI_POINT_T  point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    BOOLEAN     is_bar_item_tmp = FALSE;
    uint16      item_index_tmp = 0;
    BOOLEAN     is_find_in_bar = FALSE;
    
    StopItemExchangeTimer( qbtheme_ptr );
    
    if (qbtheme_ptr->is_move_state)// && qbtheme_ptr->is_moving ) // 处理移动状态
    {
        MMK_GetLogicTPMsg(&state, &point);
        
        if ( GUI_PointIsInRect( point, qbtheme_ptr->qbtheme_menu_data.bar_rect ) )
        {
            //get item total number
            page_item_num = GetQbThemeMenuBarItemNum(qbtheme_ptr);
            
            item_index = page_item_num;
            
            if ( 0 != page_item_num )
            {
                GUI_RECT_T bar_item_rect[MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX] = {0};
                
                GetDockBarItemRect( qbtheme_ptr, bar_item_rect, page_item_num );
                
                //bar内部
                if ( qbtheme_ptr->is_bar_item )
                {
                    //向右
                    if ( point.x > qbtheme_ptr->slide_start_point.x )
                    {
                        for (i = page_item_num - 1; i >= 0; i--)
                        {
                            item_rect = bar_item_rect[i];
                            
                            if ( point.x > item_rect.left ) 
                            {
                                item_index = i;
                                if( point.x > (item_rect.left +( item_rect.right - item_rect.left)/4 )
                                    && point.x < (item_rect.left +( item_rect.right - item_rect.left)*3/4)
                                    )
                                {
                                    is_find_in_bar = TRUE;
                                }
                                break;
                            }
                        }
                    }
                    else    //向左
                    {
                        for (i = 0; i < page_item_num; i++)
                        {
                            item_rect = bar_item_rect[i];
                            
                            if ( point.x < item_rect.right )
                            {
                                item_index = i;
                                if( point.x < (item_rect.right -( item_rect.right - item_rect.left)/4 )
                                    && point.x > (item_rect.right -( item_rect.right - item_rect.left)*3/4)
                                    )
                                {
                                    is_find_in_bar = TRUE;
                                }		
                                break;
                            }
                        }
                    }
                }
                else
                {
					if (MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX == page_item_num)
					{
						return;
					}
					
					{
						GUI_RECT_T  item_rect_first = {0};
						GUI_RECT_T  item_rect_last  = {0};
						GUI_RECT_T  item_rect_front = {0};
						GUI_RECT_T  item_rect_back  = {0};
						uint16 first_center_x = 0;
						uint16 last_center_x  = 0;
						uint16 front_center_x = 0;
						uint16 back_center_x  = 0;

						item_rect_first = bar_item_rect[0];
                        first_center_x  = ( item_rect_first.left + item_rect_first.right ) / 2;
						item_rect_last  = bar_item_rect[page_item_num - 1];
                        last_center_x   = ( item_rect_last.left + item_rect_last.right ) / 2;

						if (point.x < first_center_x)
						{
                            item_index = 0;
						}
						else if (point.x > last_center_x)
						{
                            item_index = page_item_num;
						}
                        else
                        {
							for (i = 0; i+1 < page_item_num; i++)
							{
                                item_rect_front = bar_item_rect[i];
                                item_rect_back  = bar_item_rect[i+1];
                                front_center_x  = ( item_rect_front.left + item_rect_front.right ) / 2;
                                back_center_x   = ( item_rect_back.left + item_rect_back.right ) / 2;
								if (point.x > front_center_x && point.x < back_center_x)
								{
									item_index = i + 1;
									break;
								}
							}
                        }
					}
                }
            }
            is_bar_item    = TRUE;
			s_undraw_index = item_index;
        }
        else
        {
            s_undraw_index  = 0xFFFF;
            item_index = qbtheme_ptr->move_item_index;//qbtheme_ptr->tmp_cur_item_index
        }

        if (item_index != qbtheme_ptr->cur_item_index
            || is_bar_item != qbtheme_ptr->is_bar_item)
        {
            // save state
            is_bar_item_tmp = qbtheme_ptr->is_bar_item;

            item_index_tmp = qbtheme_ptr->cur_item_index;
            
            //set current item index
            qbtheme_ptr->cur_item_index = item_index;
            qbtheme_ptr->is_bar_item = is_bar_item;

			if ( item_index >= page_item_num )
			{
				if ( is_bar_item_tmp != qbtheme_ptr->is_bar_item 
					&& qbtheme_ptr->is_bar_item)
				{
					qbtheme_ptr->is_empty_space = TRUE;
				}
			}
            
            if(qbtheme_ptr->is_moving)
            {
                if (HandleQbThemeMenuMove(qbtheme_ptr))
                {
                    qbtheme_ptr->tmp_page_item_index = qbtheme_ptr->cur_page_index;

					if(is_bar_item)
                    {
                        qbtheme_ptr->tmp_cur_item_index = qbtheme_ptr->cur_item_index;
                    }
                    
                    qbtheme_ptr->tmp_is_bar_item = is_bar_item;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : handle tp move message
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeMenuTpMove(
                                  GUI_POINT_T                   *point_ptr, 
                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr
                                  )
{
    if ((PNULL == point_ptr) || (PNULL == qbtheme_ptr))
    {
        return;
    }

    // 增加速度点
    MMK_AddVelocityItem( point_ptr->x, point_ptr->y );
    
    //清理高亮层
    if (UILAYER_IsMultiLayerEnable())    
    {
		//消除highlight层的影响
        //UILAYER_Clear(&qbtheme_ptr->highlight_layer_handle);
        UILAYER_RemoveBltLayer(&qbtheme_ptr->highlight_layer_handle);
    }
    
    if ( abs( point_ptr->x - qbtheme_ptr->pre_tp_point.x ) > MMIMAINMENU_MOVE_SPACING
        || abs( point_ptr->y - qbtheme_ptr->pre_tp_point.y ) > MMIMAINMENU_MOVE_SPACING )
    {
        StopItemExchangeTimer( qbtheme_ptr );
        
        SetMoveLayerPosition( qbtheme_ptr );
    }

    qbtheme_ptr->pre_tp_point.x = point_ptr->x;
    qbtheme_ptr->pre_tp_point.y = point_ptr->y;

    // 当move消息来时，停止timer
    if (abs(point_ptr->x - qbtheme_ptr->slide_start_point.x) > MMIMAINMENU_MOVE_SPACING
        || !qbtheme_ptr->is_item_pressed)
    {
        if ( !GUI_PointIsInRect( qbtheme_ptr->slide_start_point, qbtheme_ptr->qbtheme_menu_data.bar_rect ) )
        {
            if (!qbtheme_ptr->is_item_pressed || !qbtheme_ptr->is_move_state) // move icon状态下，不设置slide状态
            {
                qbtheme_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
            }
            
            QbThemeStopMoveTimer(qbtheme_ptr);

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
            StopHighlightIconDispTimer(qbtheme_ptr);
#endif
        }
    }

    // 处理slide效果
    if (MMK_TP_SLIDE_TP_SCROLL == qbtheme_ptr->slide_state && !qbtheme_ptr->is_move_state)
    {
        return;
    }

    // 处理auto move
    if (qbtheme_ptr->is_move_state && qbtheme_ptr->is_moving 
        && HandleQbThemeAutoMoveEvent(point_ptr, qbtheme_ptr))
    {
        return;
    }

    if (qbtheme_ptr->is_move_state && qbtheme_ptr->is_moving ) // 处理移动状态
    {
        StartItemExchangeTimer( qbtheme_ptr );
    }
}

/*****************************************************************************/
//  Description : handle slide menu touch panel press up msg
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleQbThemeTpUp(
                               GUI_POINT_T                  *tp_point_ptr,
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                               )
{
    BOOLEAN             result = FALSE;
    float               fling_velocity = 0;

	s_undraw_index = 0xFFFF;
    if ((PNULL == tp_point_ptr) || (PNULL == qbtheme_ptr) || !qbtheme_ptr->is_tp_pressed)
    {
        return FALSE;
    }

    //恢复双buffer copy
    UILAYER_SwitchDoubleMem( &qbtheme_ptr->page_layer_handle[0], FALSE );

    qbtheme_ptr->is_tp_pressed = FALSE;
    qbtheme_ptr->qbtheme_menu_data.is_dy_item = FALSE;
    
    // 增加速度点
    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );

    MMK_ComputeCurrentVelocity( &fling_velocity, PNULL, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY );/*lint !e747*/

    qbtheme_ptr->fling_velocity = fling_velocity;

    QbThemeStopMoveTimer(qbtheme_ptr);
    QbThemeStopRedrawTimer(qbtheme_ptr);

    if (qbtheme_ptr->is_moving)
    {
        if (qbtheme_ptr->cur_page_index == qbtheme_ptr->page_total_num - 1)
        {
            DisplayQbThemeBar(qbtheme_ptr, qbtheme_ptr->cur_page_index, qbtheme_ptr->titleidx_layer_handle, FALSE);
        }
    }

    result = qbtheme_ptr->is_item_pressed;
    qbtheme_ptr->is_item_pressed = FALSE;

    if (MMK_TP_SLIDE_TP_SCROLL == qbtheme_ptr->slide_state)
    {   
		qbtheme_ptr->slide_state = MMK_TP_SLIDE_NONE;
		
        //SCI_TRACE_LOW:"[GO_MENU]HandleSlidePageTpUp current time  = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_QBTHEME_7249_112_2_18_2_34_51_175,(uint8*)"d", SCI_GetTickCount());

        // 处理滑动
        if(GO_SLIDE_SUPPORT_ONLY_X==qbtheme_ptr->support_slide_type)
        {
            result = XHandleGOMenuSlideOver(qbtheme_ptr, tp_point_ptr);
        }
        else if(GO_SLIDE_SUPPORT_ONLY_Y==qbtheme_ptr->support_slide_type)
        {
            result = YHandleGOMenuSlideOver(qbtheme_ptr, tp_point_ptr);
        }
        else if(GO_SLIDE_SUPPORT_XY==qbtheme_ptr->support_slide_type)
        {
            //PNULL;
        }
		
		DisplayQbThemeBar(qbtheme_ptr, qbtheme_ptr->cur_page_index, qbtheme_ptr->titleidx_layer_handle, FALSE);

        return FALSE;
    }
        
	if (qbtheme_ptr->main_menu_theme.is_hl_disappear)
	{
		DisplayQbThemeActiveIcon(qbtheme_ptr, FALSE);
	}
	else
	{
		DisplayQbThemeActiveIcon(qbtheme_ptr, TRUE);
	}

    if (qbtheme_ptr->is_move_state && qbtheme_ptr->is_moving)
    {
        BOOLEAN is_need_redraw = TRUE;
            
        IsInToIdleRect(qbtheme_ptr, TRUE, &is_need_redraw);

        qbtheme_ptr->is_empty_space = FALSE;
        qbtheme_ptr->is_moving = FALSE; // 移动结束
        qbtheme_ptr->slide_state = MMK_TP_SLIDE_NONE;
        qbtheme_ptr->is_move_state = FALSE;
        qbtheme_ptr->is_item_pressed = FALSE;

        ResetQbThemeMenuMoveItemInfo(qbtheme_ptr);
        
        // 释放层
        ReleaseQbThemeMoveIconLayer(qbtheme_ptr);
        ReleaseQbThemeMoveBgLayer(qbtheme_ptr);
        ReleaseOrigSpecialLayer(qbtheme_ptr);
        
        //清理高亮层
        if (UILAYER_IsMultiLayerEnable())    
        {
			//消除highlight层的影响
            UILAYER_Clear(&qbtheme_ptr->highlight_layer_handle);
            UILAYER_RemoveBltLayer(&qbtheme_ptr->highlight_layer_handle);
        }

        if (qbtheme_ptr->qbtheme_menu_data.is_need_save)
        {
            SaveQbThemeMenu(qbtheme_ptr, TRUE);
            qbtheme_ptr->qbtheme_menu_data.is_need_save = FALSE;
        }
        
        if (is_need_redraw)
        {
            DisplayQbThemeMenu(qbtheme_ptr);
        }

        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : x方向
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN XHandleGOMenuSlideOver(
                                    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                    GUI_POINT_T      *tp_point_ptr
                                    )
{
    BOOLEAN result = FALSE;

    if (PNULL != qbtheme_ptr && PNULL != tp_point_ptr)
    {
        uint16              second_page     = 0; // 第二页
        uint16              pre_page_index  = 0; // 记录移动之前的页
        GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
        uint16 current_frame = 0;
        uint16 page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        
        BOOLEAN is_change_page = FALSE;
        
        pre_page_index = qbtheme_ptr->cur_page_index;
        
        
        // 设置下一页
        SetSlideCurPage_X(tp_point_ptr, qbtheme_ptr);
        
        if(pre_page_index == qbtheme_ptr->cur_page_index)
        {
            is_change_page = FALSE;
        }
        else
        {
            is_change_page = TRUE;
            SetRandomSelectEffectStyleInfo();
        }
        
        if (tp_point_ptr->x > qbtheme_ptr->slide_start_point.x) // 向右移动
        {
            direction = GO_MOVE_RIGHT;
            second_page = (pre_page_index+page_num-1)%page_num;
        }
        else
        {
            direction = GO_MOVE_LEFT;
            second_page = (pre_page_index+1)%page_num;
        }
        
        current_frame = (abs(tp_point_ptr->x - qbtheme_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);
        
        
        if(current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
        {
            current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
        }
        
        GOMenuSlideOverDraw(qbtheme_ptr,pre_page_index,second_page,direction,current_frame,is_change_page);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        StartHighlightIconDispTimer(qbtheme_ptr);
#endif
    }

    return result;
}

/*****************************************************************************/
//  Description : x方向
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN YHandleGOMenuSlideOver(
                                    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                                    GUI_POINT_T      *tp_point_ptr
                                    )
{
    BOOLEAN result = FALSE;

    if (PNULL != qbtheme_ptr && PNULL != tp_point_ptr)
    {
        uint16              second_page     = 0; // 第二页
        uint16              pre_page_index  = 0; // 记录移动之前的页
        GO_MOVE_DIRECTION_E direction = GO_MOVE_NONE;
        uint16 current_frame = 0;
        uint16 page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
        
        BOOLEAN is_change_page = FALSE;
        
        pre_page_index = qbtheme_ptr->cur_page_index;
        
        
        // 设置下一页
        SetSlideCurPage_Y(tp_point_ptr, qbtheme_ptr);
        
        if(pre_page_index == qbtheme_ptr->cur_page_index)
        {
            is_change_page = FALSE;
        }
        else
        {
            is_change_page = TRUE;
            SetRandomSelectEffectStyleInfo();
        }
        
        if (tp_point_ptr->y > qbtheme_ptr->slide_start_point.y) // 向下移动
        {
            direction = GO_MOVE_DOWN;
            second_page = (pre_page_index+page_num-1)%page_num;
        }
        else
        {
            direction = GO_MOVE_UP;
            second_page = (pre_page_index+1)%page_num;
        }
        
        current_frame = (abs(tp_point_ptr->y - qbtheme_ptr->slide_start_point.y)/GO_ONE_FRAME_PIX);
        
        
        if(current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
        {
            current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
        }
        
        GOMenuSlideOverDraw(qbtheme_ptr,pre_page_index,second_page,direction,current_frame,is_change_page);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        StartHighlightIconDispTimer(qbtheme_ptr);
#endif
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GOMenuSlideOverDraw(
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr, 
                               uint16              pre_page_index,
                               uint16              second_page,
                               GO_MOVE_DIRECTION_E direction,
                               uint16              current_frame,
                               BOOLEAN             is_change_page
                               )
{
    int16 i = 0; // 
    BOOLEAN is_disp_active = FALSE;

    if ((!qbtheme_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }
    
    if(TRUE == is_change_page)
    {
        //MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,GO_MAINMENU_TOTAL_FRAME_MAX-current_frame+1,);
        for (i = current_frame; i<= GO_MAINMENU_TOTAL_FRAME_MAX; i+=GO_FRAME_STEP_NUM)
        {
            
            DrawUpdateGOMenuLayer(qbtheme_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        }  
        
        if((GO_MAINMENU_TOTAL_FRAME_MAX+GO_FRAME_STEP_NUM) != i)
        {
            i = GO_MAINMENU_TOTAL_FRAME_MAX;
            DrawUpdateGOMenuLayer(qbtheme_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        }
        
        if((direction==GO_MOVE_LEFT)||(direction==GO_MOVE_UP))
        {
            GO_UpdateItemLayer(qbtheme_ptr,FALSE);
        }
        else if((direction==GO_MOVE_RIGHT)||(direction==GO_MOVE_DOWN))
        {
            GO_UpdateItemLayer(qbtheme_ptr,TRUE);
        }
        else
        {
            //SCI_TRACE_LOW:"[GO_MENU]GOMenuSlideOverDraw-------direction = %d"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_SLIDE_PAGE_GO_5371_112_2_18_2_35_40_221,(uint8*)"d",direction);
        }
    }
    else
    {
        for (i = current_frame; i>= 0;i-=GO_FRAME_STEP_NUM)
        {
            DrawUpdateGOMenuLayer(qbtheme_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        }
        
        if(-GO_FRAME_STEP_NUM != i)
        {
            i = 0;
            DrawUpdateGOMenuLayer(qbtheme_ptr,pre_page_index,second_page,direction,i,is_disp_active);
            GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void GO_UpdateItemLayer(
                              MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                              BOOLEAN is_tpmove_right
                              )
{
    int i = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint16 page_index   = 0;
    
    //is_tpmove_right 为true时，则tp向右滑动，为FALSE时，则tp是向左滑动
    GO_Change_handle_and_index(s_go_item_page_info,is_tpmove_right);
    
    icon_width = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;
    if(is_tpmove_right)
    {
        //新的PAGE INDEX
        if(0 == qbtheme_ptr->cur_page_index)
        {
            page_index = qbtheme_ptr->page_total_num-1;
        }
        else
        {
            page_index = qbtheme_ptr->cur_page_index-1;
        }
    }
    else
    {
        //新的PAGE INDEX
        if((qbtheme_ptr->page_total_num-1) == qbtheme_ptr->cur_page_index)
        {
            page_index = 0;
        }
        else
        {
            page_index = qbtheme_ptr->cur_page_index+1;
        }
    }
    
    //item_num = GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, page_index);
    //item_num = GetQbThemeMenuInPageMaxItemNum(qbtheme_ptr);

    for(i=0;i<GO_MEM_PAGE_NUM;i++)
    {
        if(s_go_item_page_info[i].is_change)
        {
            //需要释放后重新创建
            //UILAYER_RELEASELAYER(&s_go_item_page_info[i].layer_handle);/*lint !e506 !e774*/
            //CreateItemLayer(qbtheme_ptr, icon_width,icon_height,item_num,&(s_go_item_page_info[i].layer_handle));
            
            GO_InitItemLayerInfo(qbtheme_ptr,page_index,&(s_go_item_page_info[i].layer_handle));
            //重置标志量
            s_go_item_page_info[i].is_change = FALSE;
            
            break;
        }
    }
}

/*****************************************************************************/
//  Description : handle slide page touch panel press up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSlideCurPage_X(
                              GUI_POINT_T      *tp_point_ptr,
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                              )
{
    uint16              current_frame = 0;
    if (0 ==qbtheme_ptr->page_total_num)
    {
        return FALSE;
    }
    
    current_frame = (abs(tp_point_ptr->x - qbtheme_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);
    
    if (tp_point_ptr->x < qbtheme_ptr->slide_start_point.x) // 向左移动
    {
        // 判断是否最后一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/2)
            || qbtheme_ptr->fling_velocity < -FLING_MIN_VELOCITY)
        {
            if (qbtheme_ptr->cur_page_index < qbtheme_ptr->page_total_num - 1) // 不是最后一页
            {
                qbtheme_ptr->cur_page_index++; // 当前页索引号增加
                qbtheme_ptr->cur_page_index = qbtheme_ptr->cur_page_index%qbtheme_ptr->page_total_num;
            }
            else
            {
                qbtheme_ptr->cur_page_index = 0;
            }
            
            qbtheme_ptr->cur_item_index = 0;
        }
    }
    else // 向右移动
    {
        // 判断是否第一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/2)
            || qbtheme_ptr->fling_velocity > FLING_MIN_VELOCITY)
        {
            if (qbtheme_ptr->cur_page_index != 0)
            {
                qbtheme_ptr->cur_page_index--;
            }
            else
            {
                qbtheme_ptr->cur_page_index = qbtheme_ptr->page_total_num - 1;
            }
            
            qbtheme_ptr->cur_item_index = 0;
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
                              MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr
                              )
{
    uint16              current_frame = 0;
    if (0 ==qbtheme_ptr->page_total_num)
    {
        return FALSE;
    }
    
    current_frame = (abs(tp_point_ptr->y - qbtheme_ptr->slide_start_point.y)/GO_ONE_FRAME_PIX);
    
    if (tp_point_ptr->y < qbtheme_ptr->slide_start_point.y) // 向左移动
    {
        // 判断是否最后一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/3)
            || qbtheme_ptr->fling_velocity < -FLING_MIN_VELOCITY)
        {
            if (qbtheme_ptr->cur_page_index < qbtheme_ptr->page_total_num - 1) // 不是最后一页
            {
                qbtheme_ptr->cur_page_index++; // 当前页索引号增加
                qbtheme_ptr->cur_page_index = qbtheme_ptr->cur_page_index%qbtheme_ptr->page_total_num;
            }
            else
            {
                qbtheme_ptr->cur_page_index = 0;
            }
            
            qbtheme_ptr->cur_item_index = 0;
        }
    }
    else // 向右移动
    {
        // 判断是否第一页
        if ((current_frame>=GO_MAINMENU_TOTAL_FRAME_MAX/3)
            || qbtheme_ptr->fling_velocity > FLING_MIN_VELOCITY)
        {
            if (qbtheme_ptr->cur_page_index != 0)
            {
                qbtheme_ptr->cur_page_index--;
            }
            else
            {
                qbtheme_ptr->cur_page_index = qbtheme_ptr->page_total_num - 1;
            }
            
            qbtheme_ptr->cur_item_index = 0;
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleQbThemeMenuGrayEvent(
                                      MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr, 
                                      MMI_MESSAGE_ID_E              msg_id,
                                      DPARAM                        param
                                      )
{
    BOOLEAN *is_gray_ptr = (BOOLEAN *)param;
    
    if (PNULL == qbtheme_ptr)
    {
        return;
    }

    if (MSG_APP_OK == msg_id || MSG_APP_WEB == msg_id || MSG_TP_PRESS_UP == msg_id)
    {
        if ( PNULL != is_gray_ptr && (*is_gray_ptr))
        {
            if (!qbtheme_ptr->main_menu_theme.is_hl_disappear
                || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
            {
                DisplayQbThemeActiveIcon(qbtheme_ptr, TRUE);
            }
            else
            {
                DisplayQbThemeActiveIcon(qbtheme_ptr, FALSE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : 确认移动
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConfirmQbThemeMenuItem(
                                    MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr,
                                    MMI_MESSAGE_ID_E                msg_id
                                    )
{
    BOOLEAN result = FALSE;
    
    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }

    if (qbtheme_ptr->is_move_state)
    {
        SaveQbThemeMenu(qbtheme_ptr, TRUE);

        // 释放item拖动层
        ReleaseQbThemeMoveIconLayer(qbtheme_ptr);

        // 设置状态
        qbtheme_ptr->is_move_state = FALSE;
        qbtheme_ptr->is_moving = FALSE;
        qbtheme_ptr->slide_state = MMK_TP_SLIDE_NONE;

        DisplayQbThemeMenu(qbtheme_ptr);

        result = TRUE;

        if (MSG_APP_RED != msg_id)
        {
            return result;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : init move layer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void QbMenuInitMoveLayer(
							   MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr  //in
							   )
{
    qbtheme_ptr->is_moving = TRUE;  

    if (PNULL == qbtheme_ptr)
    {
        return;
    }
    
    if (UILAYER_IsMultiLayerEnable())
    {
        CreateOrigSpecialLayer(qbtheme_ptr, TRUE);
        CreateMoveIconLayer(qbtheme_ptr);
        CreateMoveBgLayer(qbtheme_ptr);
        DisplaySpecialAnimation(qbtheme_ptr, QbMenuIsItemEnableDeleted(qbtheme_ptr));
    }

    // 记录移动的信息
    SetQbThemeMenuMoveItemInfo(qbtheme_ptr);

    QbThemeStartShakeIconTimer(qbtheme_ptr);
}

/*****************************************************************************/
//  Description : 创建移动的buffer
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeMenuMoveInit(
                                 MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr
                                 )
{
    BOOLEAN result = FALSE;
    
    if (PNULL != qbtheme_ptr && qbtheme_ptr->is_move_state) // 记录当前和起始位置
    {
        if (UILAYER_IsMultiLayerEnable())
        {
            if (qbtheme_ptr->is_item_pressed)
            {
                QbMenuInitMoveLayer( qbtheme_ptr );
            }
        }
        else
        {
            qbtheme_ptr->is_move_state = FALSE;
        }

        result = TRUE;
    }
    
    return result;
}

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
// Description: 整理item数据，防止出现异常
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void TrimQbThemePageInfo(
    MMIMAINMENU_PAGE_ITEM_INFO_T *item_bar_ptr
)
{
    uint16 i = 0;
    BOOLEAN is_deleted = FALSE;
    if (PNULL == item_bar_ptr)
    {
        return;
    }

    item_bar_ptr->item_num = MIN(item_bar_ptr->item_num , MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX);
    
    for (i = 0; i < item_bar_ptr->item_num; i++)
    {
        BOOLEAN is_exist = FALSE;
        
        if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID 
            != item_bar_ptr->menu_item_info[i].group_id) continue;
            
        is_exist = MMIMENU_IsDyItemExist(item_bar_ptr->menu_item_info[i].menu_id);

        if (!is_exist)
        {
            DeleteInvalidDyItem(item_bar_ptr->menu_item_info, item_bar_ptr->item_num, i);
            item_bar_ptr->item_num--;
            is_deleted = TRUE;	
        }
    }
	if(is_deleted)

	{		
		uint16 j = 0;
		MMIMAINMENU_PAGE_ITEM_INFO_T mainmenu_dockbar_info = {0};					
					
		for (j = 0; j < item_bar_ptr->item_num; j++)
		{
				mainmenu_dockbar_info.item_num++;
				mainmenu_dockbar_info.menu_item_info[j].group_id = item_bar_ptr->menu_item_info[j].group_id;
				mainmenu_dockbar_info.menu_item_info[j].menu_id = item_bar_ptr->menu_item_info[j].menu_id;
		}
				
		MMINV_WRITE(MMINV_QBTHEME_DOCKBAR_INFO, &mainmenu_dockbar_info);		
	}
}

/*****************************************************************************/
// Description:  删除不合法的动态菜单
// Global resource dependence:
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void DeleteInvalidDyItem(
    MMIMAINMENU_ITEM_INFO_T* item_info_ptr,
    uint16 item_cnt,
    uint16 delete_idx
)
{
    uint16 item_idx = 0;
    BOOLEAN is_deleted = FALSE;

    if (PNULL == item_info_ptr)
    {
        return;
    }
    
    for (item_idx = delete_idx; item_idx < item_cnt - 1; item_idx++)
    {
        item_info_ptr[item_idx] = item_info_ptr[item_idx + 1];
        is_deleted = TRUE;
    }

    if (is_deleted)
    {
        item_info_ptr[item_cnt - 1].group_id = 0;
        item_info_ptr[item_cnt - 1].menu_id = 0;
    }
}

/*****************************************************************************/
//  Description : 重新加载主菜单
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ReloadItemToMainMenu(
    MMIMAINMENU_QBTHEME_DATA_T      *qbtheme_ptr
    )
{
    BOOLEAN is_move_state = FALSE;
    
    is_move_state = qbtheme_ptr->is_move_state;    
    
    // 释放
    ValidateQbThemeMenuCurPage(qbtheme_ptr);

    //DeleteQbThemeMenuEmptyPage(qbtheme_ptr);
    QbThemeResetBarIndexRect(qbtheme_ptr, &qbtheme_ptr->qbtheme_menu_data, qbtheme_ptr->page_total_num);

    // 释放
    QbThemeDestroyPageList(qbtheme_ptr);

    // 释放
    ReleaseQbThemeMenuPageLayer(qbtheme_ptr);

    // 释放
    ReleaseQbThemeMenuIndexBarLayer(qbtheme_ptr);
    
    //释放高亮图层
    ReleaseQbThemeMenuHighlightLayer(qbtheme_ptr);

    //释放动态菜单内存
    MMIMENU_DyFreeData();

    ////保存当前页码
    //MMINV_WRITE(MMINV_SLIDEMENU_CUR_PAGEINDEX, &qbtheme_ptr->cur_page_index);
    
	QbTheme_GO_ReleaseMemory();
	
	//_MMIMENU_GO_SetInitialized(FALSE);
    InitQbThemeMenu(qbtheme_ptr);

    qbtheme_ptr->is_move_state = is_move_state;
}
  
///*****************************************************************************/
////  Description :获取系统selected_icon id
////  Global resource dependence : 
////  Author:jin.wang
////  Note:返回selected_icon id
///*****************************************************************************/
//LOCAL MMI_IMAGE_ID_T GetSelectedIconId(
//                                       MMIMAINMENU_PAGE_ITEM_INFO_T *item_page_ptr
//                                       )
//{
//    uint16 page_index = 0;
//    uint16 item_index = 0;
//    MMI_IMAGE_ID_T select_icon_id = 0;
//    GUIMENU_ITEM_T *menu_item_ptr  = PNULL;
//    CTRLMAINMENU_ITEM_T menu_item = {0};

//    for (page_index = 0; page_index < MMITHEME_MENUMAIN_SLIDE_PAGE_NUM; page_index++)
//    {
//        for (item_index = 0; item_index < item_page_ptr[page_index].item_num; item_index++)
//        {
//            if (item_page_ptr[page_index].menu_item_info[item_index].group_id 
//                != DYNAMIC_MAINMENU_DEFAULT_GROUP_ID)
//            {
//                if (MMIMAINMENU_GetItemData())
//                {
//                }
//                menu_item_ptr = GUIMENU_GetItemPtr(
//                    item_page_ptr[page_index].menu_item_info[item_index].group_id, 
//                    item_page_ptr[page_index].menu_item_info[item_index].menu_id);

//                if ( PNULL != menu_item_ptr )
//                {
//                    select_icon_id = menu_item_ptr->select_icon_id;

//                    return select_icon_id;
//                }
//            }
//        }
//    }

//    return select_icon_id;
//}
  
#endif  // end of DYNAMIC_MAINMENU_SUPPORT

/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void OpenQbThemeSetMenuWin(void)
{
    MMI_WIN_ID_T            win_id  = MMIMENU_QBTHEME_SETSTYLE_WIN_ID;
    MMI_CTRL_ID_T           ctrl_id = MAINMENUWIN_SETIDLESTYLE_MENU_CTRL_ID;
    uint32                  i = 0;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    GUIMENU_BUTTON_INFO_T   button_info = {0};
    MMI_STRING_T            string = {0};
    
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    
    MMK_CreateWin((uint32*)MMIMENU_QBTHEME_SETSTYLE_TAB2, PNULL);
    
    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP_RADIO);

    InsertMenuNodeByLabel(
        ctrl_id,
        IMAGE_NULL,
        TXT_QB_DEFAULT_IDLE,
        ID_MAINMENU_IDLE_STYLE_GRID,
        0, 
        i++
        );
 #if defined MMI_ISTYLE_SUPPORT   
    InsertMenuNodeByLabel(
        ctrl_id,
        IMAGE_NULL,
        TXT_QB_SMART_IDLE,
        ID_MAINMENU_IDLE_ISTYLE,
        0, 
        i++
        );
  #endif 
    InsertMenuNodeByLabel(
        ctrl_id,
        IMAGE_NULL,
        TXT_QB_QBTHEME_IDLE,
        ID_MAINMENU_IDLE_QBTHEME,
        0, 
        i++
        );

#ifdef MMI_SAM_THEME
    InsertMenuNodeByLabel(
        ctrl_id,
        IMAGE_NULL,
        TXT_QB_SAMSUNG_IDLE,
        ID_MAINMENU_IDLE_SAMSUNG,
        0, 
        i++
        );
#endif
	
    //set menu title
    MMI_GetLabelTextByLang(TXT_IDLE_SWITH, &string);
    GUIMENU_SetMenuTitle(&string, ctrl_id);

    //set current select
    pop_item_info.is_static = FALSE;
    pop_item_info.ctrl_id = ctrl_id;
    
    switch(MMIAPISET_GetIdleStyle())
    {
    case MMISET_IDLE_STYLE_GRID:
#ifdef MMI_SAM_THEME	
        if (MMISET_IDLE_SUBSTYLE_SAM == MMIAPISET_GetIdleSubStyle())
        {
            pop_item_info.node_id = ID_MAINMENU_IDLE_SAMSUNG;
        }
        else
#endif
        {
            pop_item_info.node_id = ID_MAINMENU_IDLE_STYLE_GRID;
        }	
        break;
 #if defined MMI_ISTYLE_SUPPORT         
    case MMISET_IDLE_ISTYLE:
        pop_item_info.node_id = ID_MAINMENU_IDLE_ISTYLE; 
        break;
 #endif       
    case MMISET_IDLE_QBTHEME:
		pop_item_info.node_id = ID_MAINMENU_IDLE_QBTHEME;
        break;
        
    default:
        break;
    }

    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);

    //set context menu button style
    SCI_MEMSET(&button_info, 0, sizeof(button_info));
    button_info.is_static   = FALSE;
    //button_info.node_id     = ID_MAINMENU_IDLE_STYLE_GRID;
    button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
    GUIMENU_SetButtonStyle(ctrl_id, &button_info);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//   Author: jin.wang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQbThemeSetWinMsg(
                                          MMI_WIN_ID_T win_id,     
                                          MMI_MESSAGE_ID_E msg_id,     
                                          DPARAM param
                                          )
{
    MMI_RESULT_E    recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MAINMENUWIN_SETIDLESTYLE_MENU_CTRL_ID;
    
    //SCI_TRACE_LOW:"HandleQbThemeSetWinMsg  msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_QBTHEME_8258_112_2_18_2_34_53_176,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        SetIldeStyle(ctrl_id, win_id);
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void InsertMenuNodeByLabel(
                 MMI_CTRL_ID_T      ctrl_id,        //控件id
                 MMI_IMAGE_ID_T     image_id,       //图片id, for PDA style pop menu
                 MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                 uint16             node_id,        //节点编号
                 uint16             parent_node_id, //父节点编号
                 uint16             index           //位置
                 )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    
#ifdef MMI_PDA_SUPPORT
    node_item.select_icon_id = image_id;
#endif
    
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//   Author: jin.wang
//  Note:   
/*****************************************************************************/
LOCAL void  SetIldeStyle(
    MMI_CTRL_ID_T ctrl_id,
    MMI_WIN_ID_T win_id
)
{
    MMI_MENU_ID_T       menu_id = MMISET_IDLE_STYLE_MAX;
    MMISET_IDLE_STYLE_E cur_idle_style = MMIAPISET_GetIdleStyle();
    MMISET_IDLE_STYLE_E set_idle_style = MMISET_IDLE_STYLE_MAX;
	MMISET_IDLE_SUBSTYLE_E cur_idle_substyle = MMIAPISET_GetIdleSubStyle();
	MMISET_IDLE_SUBSTYLE_E set_idle_substyle = MMISET_IDLE_SUBSTYLE_MAX;

    if (cur_idle_style >= MMISET_IDLE_STYLE_GRID && cur_idle_style < MMISET_IDLE_STYLE_MAX)
    {
        //get set style
        menu_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch(menu_id)
        {
        case ID_MAINMENU_IDLE_STYLE_GRID:
            set_idle_style = MMISET_IDLE_STYLE_GRID;
	        set_idle_substyle = MMISET_IDLE_SUBSTYLE_DEFAULT;
            break;
#if defined MMI_ISTYLE_SUPPORT             
        case ID_MAINMENU_IDLE_ISTYLE:
            set_idle_style = MMISET_IDLE_ISTYLE;
            break;
#endif            
        case ID_MAINMENU_IDLE_QBTHEME:
            set_idle_style = MMISET_IDLE_QBTHEME;
            break;
            
#ifdef MMI_SAM_THEME
        case ID_MAINMENU_IDLE_SAMSUNG:
            set_idle_style = MMISET_IDLE_STYLE_GRID;
	        set_idle_substyle = MMISET_IDLE_SUBSTYLE_SAM;
            break;
#endif
            
        default:
            break;
        }

        if ((set_idle_style != cur_idle_style || set_idle_substyle != cur_idle_substyle)
            && set_idle_style >= MMISET_IDLE_STYLE_GRID
            && set_idle_style < MMISET_IDLE_STYLE_MAX)
        {
            MMK_SetWinNeedCopyLcdInLoseFocus(MAIN_IDLE_WIN_ID,FALSE);
            //退出千变主题，需要释放内存
            if(MMIAPIQBTHEME_IsSupport(cur_idle_style)
		        && !MMIAPIQBTHEME_IsSupport(set_idle_style))
            {
                MMIAPIQBTHEME_StyleStop();
            }
            
    		MMISET_SetIdleAndSubStyle(set_idle_style, set_idle_substyle);
                       
            //istyle风格需要做特殊处理
#if defined MMI_ISTYLE_SUPPORT             
            if (MMISET_IDLE_ISTYLE == set_idle_style)
            {
                GUI_BOTH_RECT_T both_rect = {0};
                
                both_rect  = MMITHEME_GetFullScreenBothRect();
                GUIAPICTRL_SetRect(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &both_rect.v_rect);  
            }
#endif            
			MMK_CloseWin(win_id);
            MMK_ReturnIdleWin(); 

            //千变主题需要做一些初始化
            if (!MMIAPIQBTHEME_IsSupport(cur_idle_style)
		        && MMIAPIQBTHEME_IsSupport(set_idle_style))
            {
                MMIAPIQBTHEME_StyleStart();
            }
#if defined MMI_WALLPAPER_SUPPORT
            MMIAPIIDLE_CreatWallpaperCtrl();
#endif
			MMIPUB_OpenAlertWaitingWin(TXT_SWITCHING_SYSTEM);

            MMK_SetWinNeedCopyLcdInLoseFocus(MAIN_IDLE_WIN_ID,TRUE);
        }
        else if(set_idle_style == cur_idle_style)
        {
            MMK_SendMsg(win_id, MSG_CTL_CANCEL, PNULL);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : set idle style
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_SetQbThemeIdleStyle(void)
{
    OpenQbThemeSetMenuWin();
}

/*****************************************************************************/
//  Description : read all menu item info.
//  Global resource dependence : 
//  Author: jin.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ReadQbthemeMenuItemInfo(MMIMAINMENU_ITEM_INFO_T *item_ptr, uint32 *num_ptr)
{
    uint16 new_item_idx = 0;

    new_item_idx = GetValidItemInfo(item_ptr);

    if (new_item_idx < MMIMENU_MAIN_MAX_NUM)
    {
        *num_ptr = new_item_idx;
    }
    else
    {
        *num_ptr = MMIMENU_MAIN_MAX_NUM;
    }

    return;
}

/*****************************************************************************/
//  Description : check display init ok?
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _IsInitItemFinished(
								 MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr
								 )
{
	BOOLEAN result = FALSE;

	if (PNULL != qbtheme_ptr && 0 == qbtheme_ptr->init_item_timer_id)
	{
		result = TRUE;
	}

	return result;
}

#ifdef ATEST_SUPPORT
PUBLIC void Atest_QbThemeMenuGetItemInfo(
    void     *qbtheme_ptr,
    GUIMENU_ITEM_POSITION_E         position,
    uint16 page_index,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *item_ptr
)
{
	QbThemeMenuGetItemInfo((MMIMAINMENU_QBTHEME_DATA_T *)qbtheme_ptr, position, page_index,item_index,item_ptr);
	return; 
}
#endif
/*****************************************************************************/
//  Description : get item
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void QbThemeMenuGetItemInfo(
    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
    GUIMENU_ITEM_POSITION_E         position,
    uint16 page_index,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *item_ptr
)
{
    MMIMAINMENU_ITEM_INFO_T item = {0};
    MMIMAINEMENU_ITEM_LIST_T  *page_item_info_ptr = PNULL;

    if ( GUIMENU_ITEM_IN_BAR == position )
    {
        page_item_info_ptr = GetQbThemeMenuBarHeadPtr(qbtheme_ptr);
    }
    else
    {
        page_item_info_ptr = MMIMENU_GetItemList(&qbtheme_ptr->menu_page_info, page_index);
    }

    MMIMAINMENU_GetItem(page_item_info_ptr, item_index, &item);

    if (ID_MAINMENU_RETURNIDLE == item.menu_id)
    {
        uint16 i = 0;
        GUIMENU_GROUP_T * menu_group_ptr = PNULL;
        GUIMENU_ITEM_T  * menu_item_ptr = PNULL;

        menu_group_ptr = (GUIMENU_GROUP_T *)MMI_GetMenuGroup(MENU_QBTHEME_BAR);

        for(i = 0; i < menu_group_ptr->item_count; i++)
        {
            menu_item_ptr = (GUIMENU_ITEM_T*)&(menu_group_ptr->item_ptr[i]);

            if (ID_MAINMENU_RETURNIDLE == menu_item_ptr->menu_id)
            {
                item_ptr->group_id = item.group_id;
                item_ptr->menu_id = item.menu_id;

                item_ptr->icon.type = CTRL_ICON_ID;
                item_ptr->icon.data.icon_id = menu_item_ptr->unselect_icon_id;

                item_ptr->active_icon.type = CTRL_ICON_ID;
                item_ptr->active_icon.data.icon_id  = menu_item_ptr->select_icon_id;

                item_ptr->text.type = CTRL_TEXT_ID;
                item_ptr->text.data.text_id = menu_item_ptr->text_str_id;

                break;
            }
        }
    }
    else
    {
        MMIMAINMENU_GetItemData(item.menu_id, item.group_id, item_ptr);
    }

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
LOCAL BOOLEAN QbThemeMenuRunItem(
    MMIMAINMENU_QBTHEME_DATA_T     *qbtheme_ptr,
    uint16 item_index
)
{
    BOOLEAN             is_sent = FALSE;
    CTRLMAINMENU_ITEM_T item = {0};

    GetItemPtr(qbtheme_ptr, qbtheme_ptr->cur_page_index, item_index, &item);

    if (
#ifdef DYNAMIC_MAINMENU_SUPPORT    
    !MMIMENU_DyRunItem(item.group_id,item.menu_id) && 
#endif    
    PNULL != item.call_back_start)
    {
        item.call_back_start(item.param_data1, item.param_data2);
        is_sent = TRUE;
    }

    return is_sent;
}

/*****************************************************************************/
//  Description : HandleMenuOkKey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeHandleOkKey(
                              MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                              MMI_MESSAGE_ID_E  msg_id
                              )
{
    BOOLEAN                 is_handled = FALSE;
    BOOLEAN                 is_grayed = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != qbtheme_ptr); /*assert verified*/
    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }

    if (qbtheme_ptr->is_move_state)
    {
        is_handled = FALSE;
    }
    else
    {
        MMIMAINMENU_ITEM_INFO_T *item_info_ptr = {0};

        item_info_ptr = GetItemInfoPtr(
                qbtheme_ptr,
                qbtheme_ptr->cur_page_index,
                qbtheme_ptr->cur_item_index);

        if (PNULL != item_info_ptr)
        {
            is_grayed = MMIMAINMENU_IsItemDisable(item_info_ptr->menu_id);

            if (!is_grayed)
            {
                // 在app接到ok以后，可能先干一些事情再关菜单，这样可能会导致关菜单时停止tts失败，
                // 所以在发通知之前就关tts
                MMITheme_StopControlText();

                if (!QbThemeMenuRunItem(qbtheme_ptr, qbtheme_ptr->cur_item_index))
                {
                    NotifyParentMsg(qbtheme_ptr->ctrl_handle, msg_id);
                }
            }
            else
            {
                HandleQbThemeMenuGrayEvent(qbtheme_ptr, msg_id, (DPARAM)&is_grayed);
            }
        }
    }
    
    return (is_handled);
}

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyParentMsg(
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
        break;
    }

    if (MSG_NOTIFY_START != notify_msg)
    {
        //notify parent control
        result = GUICTRL_PostNotify(ctrl_handle,notify_msg);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle menu cancel key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeHandleCancelKey(
                                  MMIMAINMENU_QBTHEME_DATA_T    *qbtheme_ptr,
                                  MMI_MESSAGE_ID_E  msg_id
                                  )
{
    BOOLEAN     is_handled = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != qbtheme_ptr); /*assert verified*/
    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }

    is_handled = ConfirmQbThemeMenuItem(qbtheme_ptr, msg_id);

    if (!is_handled)
    {
        //notify parent control or parent window
        NotifyParentMsg(qbtheme_ptr->ctrl_handle, MSG_APP_CANCEL);
    }

    return (is_handled);
}

/*****************************************************************************/
//  Description : handle menu number key
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN QbThemeHandleNumKey(
                               MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
                               MMI_MESSAGE_ID_E msg_id
                               )
{
    BOOLEAN             result = FALSE;
    uint16              i = 0;
    uint16              key_max_num = 0;
    uint16              num_idnex = 0;
    uint16              total_num = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != qbtheme_ptr); /*assert verified*/
    if (PNULL == qbtheme_ptr)
    {
        return FALSE;
    }

    //get number key index
    key_max_num = sizeof(s_guimenu_numkey_map)/sizeof(QBTHEME_NUMKEY_MAP_T);
    for (i=0; i<key_max_num; i++)
    {
        if (msg_id == s_guimenu_numkey_map[i].key_msg)
        {
            num_idnex = s_guimenu_numkey_map[i].num_index;
            break;
        }
    }

    //set total number
    total_num = GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, qbtheme_ptr->cur_page_index);

    if (num_idnex < total_num)
    {
        if (IsValidItemIndex(num_idnex,qbtheme_ptr))
        {
            //enter the next menu or notify ok msg
            result = QbThemeHandleOkKey(qbtheme_ptr,MSG_APP_WEB);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetState( 
                    uint32* dst_state_ptr,
                    uint32  src_state,
                    BOOLEAN is_true
                    )
{
    uint32 state;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != dst_state_ptr); /*assert verified*/
    if (PNULL == dst_state_ptr)
    {
        return;
    }
    
    state = *dst_state_ptr;
    
    if( is_true )
    {
        state |= src_state;
    }
    else
    {
        state &= ~src_state;
    }
    
    *dst_state_ptr = state;
}

LOCAL void DisplayHighlightIconLayer(
    GUI_LCD_DEV_INFO    *highlight_layer_ptr
)
{
    register uint32 i = 0;
    uint32 *out_buf = PNULL;
   // IMGREF_SIZE_T size = {0};
    register uint32 *buf = PNULL;
    register uint32 num = 0;
    register uint32 color = 0x00ffff00;//黄色
    UILAYER_INFO_T layer_info_ptr = {0};
    
    if(PNULL == highlight_layer_ptr || !UILAYER_IsLayerActive(highlight_layer_ptr))
    {
        return;
    }
    
	if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //PAF888 TO ARGB888  
        UILAYER_SetPreMultiplyAlpha(highlight_layer_ptr,FALSE,FALSE);
    }
    
    if(DATA_TYPE_ARGB888 != UILAYER_GetLayerColorType(highlight_layer_ptr))
    {
        return;
    }
    
    out_buf = (uint32*)UILAYER_GetLayerBufferPtr(highlight_layer_ptr);
    
    if(PNULL == out_buf)
    {
        return;
    }
    
    buf = out_buf;
    
    UILAYER_GetLayerInfo(highlight_layer_ptr,&layer_info_ptr);
    num = layer_info_ptr.layer_width*layer_info_ptr.layer_height;

	UILAYER_Clear(highlight_layer_ptr);
	
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //ARGB888 TO PAF888  
        UILAYER_SetPreMultiplyAlpha(highlight_layer_ptr,TRUE,TRUE);
    }

    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcInputDataType())
    {  
        color = ConvertRgb565ToArgb888(MMITHEME_GetCurThemeFontColor(MMI_THEME_FORM_BG), 0x00);//0x00ffff00;//黄色
    }
    else
    {
        color = ConvertRgb565ToArgb888(MMITHEME_GetCurThemeFontColor(MMI_THEME_FORM_BG), 0xFF);//0xffffff00;//黄色
    }
    
    while( i < num )
    {
        *buf = color;//黄色
        i++;
        buf++;
    }
}

/*****************************************************************************/
//  Description :获取GO桌面需要的页面信息
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void GO_GetPageInfo(
    MMIMAINMENU_QBTHEME_DATA_T   *qbtheme_ptr,
    uint16 page_index,
    GO_MAINMENU_PAGE_INFO_T *page_info_ptr
)
{
    uint16       i                    = 0;
    GUI_RECT_T   buf_rect             = {0};
    GUI_RECT_T menu_rect = {0};
        
    if((PNULL == qbtheme_ptr)||(PNULL == page_info_ptr)||(PNULL == page_info_ptr->icon_info))
    {
        return;
    }
    
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcOutputDataType())
    {
        //AM888 ARGB888申请层的大小没有状态栏，算法获取的offset认为是相对于层的。
        menu_rect = AdjustSlideMenuRect(qbtheme_ptr);
    }
    else
    {
        //RGB565直接画到背景层，不需要调整OFFSET
        menu_rect.left = 0;
        menu_rect.top = 0;
    }
    page_info_ptr->total_row_num = qbtheme_ptr->qbtheme_menu_data.page_row_num;
    page_info_ptr->total_col_num = qbtheme_ptr->qbtheme_menu_data.page_col_num;

    page_info_ptr->icon_num = GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, page_index);
    

    SCI_MEMSET(page_info_ptr->icon_info, 0,page_info_ptr->total_row_num *page_info_ptr->total_col_num*sizeof(GO_MAINMENU_ICON_INFO_T));
    
    for(i=0;i<page_info_ptr->icon_num;i++)
    {
        buf_rect = qbtheme_ptr->qbtheme_menu_data.item_rect[i];
        
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
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void InitQbThemeItemData(
                              MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr
                              )
{
    uint16 page_index   = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    BOOLEAN result = FALSE;
    uint16 i            = 0;
    
    if(PNULL == qbtheme_ptr || 0 == qbtheme_ptr->page_total_num)
    {
        return;
    }
    
    for(i=0;i<GO_MEM_PAGE_NUM;i++)
    {
        s_go_item_page_info[i].index = 0;
        s_go_item_page_info[i].is_change = FALSE;
    }
    
    icon_width = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;
    
    //画当前页
    s_go_item_page_info[1].index = 1;
    result = GO_InitItemLayerInfo(qbtheme_ptr,qbtheme_ptr->cur_page_index,&(s_go_item_page_info[1].layer_handle));
    
    //画上一页
    s_go_item_page_info[0].index = 0;
    if(0 == qbtheme_ptr->cur_page_index)
    {
        page_index = qbtheme_ptr->page_total_num-1;
    }
    else
    {
        page_index = qbtheme_ptr->cur_page_index-1;
    }
    result = GO_InitItemLayerInfo(qbtheme_ptr,page_index,&(s_go_item_page_info[0].layer_handle));
    
    //画下一页
    s_go_item_page_info[2].index = 2;
    if((qbtheme_ptr->page_total_num-1) == qbtheme_ptr->cur_page_index)
    {
        page_index = 0;
    }
    else
    {
        page_index = qbtheme_ptr->cur_page_index+1;
    }

    
    result = GO_InitItemLayerInfo(qbtheme_ptr,page_index,&(s_go_item_page_info[2].layer_handle));
    
    //数据已经准备完毕，避免内存不足
    MMITHEME_DestroyMenuItemCache();

    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:  kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GO_InitItemLayerInfo(
                                   MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                   uint16 page_index,
                                   GUI_LCD_DEV_INFO *layer_handle
                                   )
{
    uint16 i            = 0;
    uint16 page_num     = 0;
    uint16 item_num     = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint32* next_ptr = PNULL;
    GUI_RECT_T dis_rect = {0};
    uint16 page_max_item_num = 0;
    
    if(PNULL==qbtheme_ptr)
    {
        return FALSE;
    }
    
    //置空
    page_max_item_num = GetQbThemeMenuInPageMaxItemNum(qbtheme_ptr);
    
    if (page_index >= MMITHEME_MENUMAIN_SLIDE_PAGE_NUM)
    {
        SCI_TRACE_LOW("GO_InitItemLayerInfo page index = %d error!", page_index);
        return FALSE;
    }

    for (i = 0; i < page_max_item_num; i++)
    {
        s_slidemenu_item_layer[page_index][i].icon_layer_buf= PNULL;
        s_slidemenu_item_layer[page_index][i].width  = 0;
        s_slidemenu_item_layer[page_index][i].height = 0;
    }
    
    if( PNULL == layer_handle
        || layer_handle->block_id == UILAYER_NULL_HANDLE)
    {
        SCI_TRACE_LOW("GO_InitItemLayerInfo111111  %d.",page_index); 
        return FALSE;
    }
    
    if(!UILAYER_IsLayerActive(layer_handle))
    {
        SCI_TRACE_LOW("GO_InitItemLayerInfo22222 %d, %d.", layer_handle->block_id, page_index);

        return FALSE;
    }    


    item_num = GetQbThemeMenuPageTotalItemNum(qbtheme_ptr, page_index);

    if(0 == item_num)
    {
        //如果是零页则不用进行初始化
        return TRUE;
    }
    
    page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
    
    next_ptr = (uint32*)UILAYER_GetLayerBufferPtr(layer_handle);

    if(PNULL == next_ptr)
    {
        SCI_TRACE_LOW("GO_InitItemLayerInfo33333 %d, %d.", layer_handle->block_id, page_index);
        return FALSE;
    }
    
    icon_width  = MMITHEME_MENU_ITEM_WIDTH+2*GO_CALC_OFFSET;
    icon_height = MMITHEME_MENU_ITEM_HEIGHT+2*GO_CALC_OFFSET;
    
    if(DATA_TYPE_PMARGB888== SEAPI_GetCalcInputDataType())
    {    
        UILAYER_SetPreMultiplyAlpha(layer_handle,FALSE,FALSE);
    }
    //在层上画icon
    // 使用color清除层
    UILAYER_Clear(layer_handle);
    
    dis_rect.left = GO_CALC_OFFSET;
    dis_rect.top = GO_CALC_OFFSET;
    dis_rect.right = dis_rect.left + icon_width - 1;
    dis_rect.bottom = dis_rect.top + icon_height - 1;
    
    for (i = 0; i < item_num; i++)
    {
        DisplayOneItemToLayer( qbtheme_ptr, layer_handle, &dis_rect, page_index, i, GUIMENU_ITEM_IN_PAGE );

        dis_rect.top    += icon_height;
        dis_rect.bottom += icon_height;
    }
    
    if(DATA_TYPE_PMARGB888 == SEAPI_GetCalcInputDataType())
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
    //把剩下的置空
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
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL void MoveGOMenuPageLayer_X(
                                  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
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
    SCI_ASSERT(PNULL != qbtheme_ptr); /*assert verified*/
    
    first_page    = qbtheme_ptr->cur_page_index;
    
    if (point_ptr->x > qbtheme_ptr->slide_start_point.x) // 向右移动
    {
        direction = GO_MOVE_RIGHT;
    }
    else
    {
        direction = GO_MOVE_LEFT;
    }
    
    current_frame = (abs(point_ptr->x - qbtheme_ptr->slide_start_point.x)/GO_ONE_FRAME_PIX);
    
    if(current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }
    
    second_page = GetSlideSecondPage_X(qbtheme_ptr, point_ptr, &is_second_move);      
    
    if ((!qbtheme_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }

    DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,second_page,direction,current_frame,is_disp_active); 

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideSecondPage_X(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                GUI_POINT_T*    point_ptr,
                                BOOLEAN        *is_second_move
                               )
{
    uint16  second_page = 0;
    
    if (0 == qbtheme_ptr->cur_page_index) // 当前页在第一页
    {
        if (point_ptr->x > qbtheme_ptr->slide_start_point.x) // 向右移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {               
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->page_total_num - 1;
            }
        }
        else if (point_ptr->x < qbtheme_ptr->slide_start_point.x) // 向左移动
        {
            if (qbtheme_ptr->page_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index + 1;
            }
        }
    }
    else if (qbtheme_ptr->page_total_num - 1 == qbtheme_ptr->cur_page_index) // 当前页在最后一页
    {
        if (point_ptr->x > qbtheme_ptr->slide_start_point.x) // 向右移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->x < qbtheme_ptr->slide_start_point.x) // 向左移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {
                *is_second_move = TRUE;               
                second_page = 0;
            }
        }
    }
    else
    {
        if (point_ptr->x > qbtheme_ptr->slide_start_point.x) // 向右移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->x < qbtheme_ptr->slide_start_point.x) // 向左移动
        {
            if (qbtheme_ptr->page_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index + 1;
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
LOCAL void MoveGOMenuPageLayer_Y(
                                  MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
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
    SCI_ASSERT(PNULL != qbtheme_ptr); /*assert verified*/
    
    first_page    = qbtheme_ptr->cur_page_index;
    
    if (point_ptr->y > qbtheme_ptr->slide_start_point.y) // 向下移动
    {
        direction = GO_MOVE_DOWN;
    }
    else
    {
        direction = GO_MOVE_UP;
    }
    
    current_frame = (abs(point_ptr->y - qbtheme_ptr->slide_start_point.y)/GO_ONE_FRAME_PIX);
    
    if(current_frame>GO_MAINMENU_TOTAL_FRAME_MAX)
    {
        current_frame = GO_MAINMENU_TOTAL_FRAME_MAX;
    }
    second_page = GetSlideSecondPage_Y(qbtheme_ptr, point_ptr, &is_second_move);     
    
    if ((!qbtheme_ptr->main_menu_theme.is_hl_disappear || MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
    {
        is_disp_active = TRUE;
    }

    DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,second_page,direction,current_frame,is_disp_active); 

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wei.ren
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideSecondPage_Y(
                                MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr,
                                GUI_POINT_T*    point_ptr,
                                BOOLEAN        *is_second_move
                               )
{
    uint16  second_page = 0;
    
    if (0 == qbtheme_ptr->cur_page_index) // 当前页在第一页
    {
        if (point_ptr->y > qbtheme_ptr->slide_start_point.y) // 向下移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {               
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->page_total_num - 1;
            }
        }
        else if (point_ptr->y < qbtheme_ptr->slide_start_point.y) // 向上移动
        {
            if (qbtheme_ptr->page_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index + 1;
            }
        }
    }
    else if (qbtheme_ptr->page_total_num - 1 == qbtheme_ptr->cur_page_index) // 当前页在最后一页
    {
        if (point_ptr->y > qbtheme_ptr->slide_start_point.y) // 向下移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->y < qbtheme_ptr->slide_start_point.y) // 向上移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {
                *is_second_move = TRUE;               
                second_page = 0;
            }
        }
    }
    else
    {
        if (point_ptr->y > qbtheme_ptr->slide_start_point.y) // 向下移动
        {
            if (qbtheme_ptr->page_total_num > 1)
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index - 1;
            }
        }
        else if (point_ptr->y < qbtheme_ptr->slide_start_point.y) // 向下移动
        {
            if (qbtheme_ptr->page_total_num > 1)// 判断右侧是否还有下一页
            {
                *is_second_move = TRUE;
                second_page = qbtheme_ptr->cur_page_index + 1;
            }
        }
    }
    
    return second_page;
}

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : 设置点击特效类型INFO
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetSelectEffectStyleInfo(
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style
)
{

    switch(mainmenu_select_style)
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
            //SCI_PASSERT(FALSE,("MMIMENU_SetSelectEffectStyleInfo:mainmenu_select_style %d is error!",mainmenu_select_style));
			SCI_TRACE_LOW("MMIMENU_SetSelectEffectStyleInfo:mainmenu_select_style %d is error!",mainmenu_select_style);
            break;
    }

}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void GO_Change_handle_and_index(
    QBTHEME_GO_MEM_PAGE_INFO_T* page_array_info,
    BOOLEAN is_tpmove_right
)
{
    int i = 0;
    
    //is_tpmove_right 为true时，则tp向右滑动，为FALSE时，则tp是向左滑动
    if(is_tpmove_right)
    {
        for(i=0;i<GO_MEM_PAGE_NUM;i++)
        {
            if((GO_MEM_PAGE_NUM-1) == page_array_info[i].index)
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
        for(i=0;i<GO_MEM_PAGE_NUM;i++)
        {
            if(0 == page_array_info[i].index)
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
//  Description : 设置选中随机
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetRandomSelectEffectStyleInfo(void)
{
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;

    mainmenu_select_style = MMIAPISET_GetMainmenuSelectStyle();
    
    if(SELECT_EFFECT_RANDOM == mainmenu_select_style)
    {
        mainmenu_select_style = rand()%(SELECT_EFFECT_RANDOM-1) + 1;
        
        SetSelectEffectStyleInfo(mainmenu_select_style);
    }
#endif
}

#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL void ChangeGOMenuStyle(MMI_HANDLE_T win_handle)
{
    int16 current_frame = 0;
    int16 first_page = 0;
    int16 next_page =0;
    int16 page_num = 0;
    MMIMAINMENU_TYPE_E  mainmenu_style = MMIMAINMENU_TYPE_MAX;
    MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr = PNULL;
    GO_MENU_STYLE_E cur_style = MMIAPISET_GetGOmenuStyle();

    mainmenu_style = GUIMAINMENU_GetStyle(MMK_GetCtrlHandleByWin(win_handle, MAINMENUWIN_MENU_CTRL_ID));

    if (MMIMAINMENU_QBTHEME_E == mainmenu_style)
    {
        qbtheme_ptr = (MMIMAINMENU_QBTHEME_DATA_T *)GUIMAINMENU_GetPM(MMK_GetCtrlHandleByWin(win_handle, MAINMENUWIN_MENU_CTRL_ID));
        
        if (PNULL != qbtheme_ptr)
        {
            do{
                cur_style +=1;
                cur_style = (uint32)cur_style%GO_MENU_STYLE_MAX;
                MMISET_SetGOmenuStyle(cur_style);
                
            }while(0);// GO_MENU_STYLE_RANDOM == cur_style);

            qbtheme_ptr->support_slide_type = UAPIMenu_GetCalcStyleSlideType(MMIAPISET_GetGOmenuStyle());
            
            page_num = MIN(qbtheme_ptr->page_total_num, MMITHEME_MENUMAIN_SLIDE_PAGE_NUM);
            
            first_page = qbtheme_ptr->cur_page_index;
            next_page = (first_page+page_num-1)%page_num;
            
            //过场动画，显示主菜单特效
            if(GO_SLIDE_SUPPORT_ONLY_Y==qbtheme_ptr->support_slide_type)
            {
                for(current_frame = (GO_MAINMENU_TOTAL_FRAME_MAX/2);current_frame>=0;current_frame-=GO_FRAME_STEP_NUM)
                {
                    DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
                if(-GO_FRAME_STEP_NUM != current_frame)
                {
                    current_frame = 0;
                    DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
            }
            else
            {
                for(current_frame = (GO_MAINMENU_TOTAL_FRAME_MAX/2);current_frame>=0;current_frame-=GO_FRAME_STEP_NUM)
                {
                    DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_RIGHT,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
                if(-GO_FRAME_STEP_NUM!= current_frame)
                {
                    current_frame = 0;
                    DrawUpdateGOMenuLayer(qbtheme_ptr,first_page,next_page,GO_MOVE_DOWN,current_frame,FALSE);
                    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                }
            }
            //DisplayQbThemeActiveIcon(qbtheme_ptr,TRUE);
        }  
    }
}

/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMENU_ChangeQbThemeGOStyle(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T    win_handle = 0;
    MMI_HANDLE_T    ctrl_handle = 0;
    MMIMAINMENU_QBTHEME_DATA_T *qbtheme_ptr = PNULL;

    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MAINMENUWIN_MENU_CTRL_ID);
    qbtheme_ptr = (MMIMAINMENU_QBTHEME_DATA_T *)GUIMAINMENU_GetPM(ctrl_handle);
    
    if (PNULL == qbtheme_ptr)
    {
        //如果menu控件指针为NULL，意味着主菜单窗口没有打开，重进主菜单
        //在idle点击这个icon的时候，就不会出现没有响应的问题
        win_handle = MMIAPIMENU_CreatMainMenu();
    }  
    else
    {
        win_handle = qbtheme_ptr->win_handle;
    }
    
    ChangeGOMenuStyle(win_handle);
}

#endif

#endif


