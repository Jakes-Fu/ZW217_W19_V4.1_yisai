/******************************************************************************
** File Name:      mmiwidget.c                                                   *
** Author:                                                                    *
** Date:           06/2007                                                    *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe button                       *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME               DESCRIPTION                              *
** 06/2007        bin.ji     Create                                   *                              *
******************************************************************************/
#ifndef _MMIWIDGET_H_
#define _MMIWIDGET_H_ 

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "mmk_app.h"
#include "guifont.h"
#include "guicommon.h"
#include "guictrl.h"
#include "guianim.h"
#include "ui_layer.h"
#include "mmitheme_widget.h"
#include "mmipb_export.h"
#include "mmiwidget_text.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_config.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

#if defined (LOW_MEMORY_SUPPORT_NONE) || !(defined (LOW_MEMORY_SUPPORT))

#define PAGE_LAYER_SIZE     1
#define WALLPAPER_LAYER_SIZE 1//2
//#define BACKGROUND_GRID_LINE_LAYER	//显示格子线的bg层
#else
#define PAGE_LAYER_SIZE     1
#define WALLPAPER_LAYER_SIZE 1
#define MMI_IDLE_WIDGET_CACHE
#endif

#ifdef BAR_PHONE_WIDGET
#if defined MMIWIDGET_MP3_SE
#define PAGE_LAYER_SIZE         3
#else
#define PAGE_LAYER_SIZE         1
#endif
#define WALLPAPER_LAYER_SIZE    1
#undef MMI_IDLE_WIDGET_CACHE
#endif



//widget最大个数
//#define MMIWIDGET_MAX_ITEM_NUM              20
#define MMIWIDGET_MAX_ITEM_NUM                MMIWIDGET_WIDGET_MAX
//shortcut最大个数
//#define MMIWIDGET_MAX_SHORTCUT_NUM          20
#define MMIWIDGET_MAX_SHORTCUT_NUM            MMIWIDGET_WIDGET_MAX
//最大页数
#define MMIWIDGET_MAX_PAGE_NUM              3

#define ARGB_GET_A(_argb) (((_argb) >> 24) & 0xff)
#define ARGB_GET_R(_argb) (((_argb) >> 16) & 0xff)
#define ARGB_GET_G(_argb) (((_argb) >> 8) & 0xff)
#define ARGB_GET_B(_argb) ((_argb) & 0xff)

#define ARGB_SET_A(_argb, _alpha)   ((_argb) | (((_alpha) << 24) & 0xff000000))
#define ARGB_SET_R(_argb, _r)       ((_argb) | (((_r) << 16) & 0xff0000))
#define ARGB_SET_G(_argb, _g)       ((_argb) | (((_g) << 8) & 0xff00))
#define ARGB_SET_B(_argb, _b)       ((_argb) | (((_b)) & 0xff))

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define RGB888_TO_RGB565(r, g, b)  (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | ((b >> 3) & 0x1f))
#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))
/*lint -e648 -e572 -e778*/
#define RGB5652ARGB8888(rgb565,alpha) ( ((alpha)<<24) |((((rgb565) >> 8) & 0xf8)<<16)|((((rgb565) >> 3) & 0xfc)<<8)|(((rgb565) << 3)  & 0xf8))
/*lint -e648 -e572 -e778*/
#define BLEND_MASK 0x7E0F81F

#ifdef MMI_GRID_IDLE_SUPPORT

#ifdef MAINLCD_SIZE_240X320
#define MMI_IDLE_GRID_ROW_NUM                    3
#define MMI_IDLE_GRID_COL_NUM                    4
#define MMI_IDLE_GRID_ROW_NUM_LANDSCAPE          3
#define MMI_IDLE_GRID_COL_NUM_LANDSCAPE          4
#elif defined MAINLCD_SIZE_240X400
#define MMI_IDLE_GRID_ROW_NUM                    4
#define MMI_IDLE_GRID_COL_NUM                    4   
#define MMI_IDLE_GRID_ROW_NUM_LANDSCAPE          3
#define MMI_IDLE_GRID_COL_NUM_LANDSCAPE          5
#else
#define MMI_IDLE_GRID_ROW_NUM                    4
#define MMI_IDLE_GRID_COL_NUM                    4
#define MMI_IDLE_GRID_ROW_NUM_LANDSCAPE          3
#define MMI_IDLE_GRID_COL_NUM_LANDSCAPE          5
#endif

#ifdef BAR_PHONE_WIDGET
#define MMI_GRID_PAGE_NUM                        7
#else
#define MMI_GRID_PAGE_NUM                        7
#endif
#define MMI_GRID_PAGE_LAYER_NUM                  3
#define MMIQBTHEME_MAX_WIDGET_PAGE_NUM             9

#ifdef BAR_PHONE_WIDGET
#define MMIWIDGET_DEFAULT_PAGE_INDEX    0  //MMIWIDGET_PRELOAD_PAGE_1
#else
#define MMIWIDGET_DEFAULT_PAGE_INDEX    (MMI_GRID_PAGE_NUM/2)  //MMIWIDGET_PRELOAD_PAGE_1
#endif

#ifdef MMI_SAM_THEME
#define MMIWIDGET_SAM_DEFAULT_PAGE_INDEX    0 
#endif

#if defined QBTHEME_SUPPORT
#define MAX_PAGE_NUM   MMIQBTHEME_MAX_WIDGET_PAGE_NUM
#elif defined MMI_GRID_IDLE_SUPPORT
#define MAX_PAGE_NUM   MMI_GRID_PAGE_NUM
#endif

#define MMI_IDLE_GRID_PAGE_CELL_NUM              (MMI_IDLE_GRID_ROW_NUM * MMI_IDLE_GRID_COL_NUM)
#define MMI_IDLE_SC_ITEM_MAX                     (MMI_IDLE_GRID_PAGE_CELL_NUM * MMI_GRID_PAGE_NUM)
#define MMI_IDLE_GRID_PAGE_CELL_NUM_LANDSCAPE   (MMI_IDLE_GRID_ROW_NUM_LANDSCAPE * MMI_IDLE_GRID_COL_NUM_LANDSCAPE)
#define MMI_IDLE_SC_ITEM_MAX_LANDSCAPE          (MMI_IDLE_GRID_PAGE_CELL_NUM_LANDSCAPE * MMI_GRID_PAGE_NUM)


#define MMI_IDLE_INVALID_VAL                      0x0000
#define MMI_IDLE_WIDGET_MASK                      0x0100
#define MMI_IDLE_ICON_MASK                        0x0200
#define MMI_IDLE_FOLDER_MASK                      0x0400
#define MMI_IDLE_ITEM_MASK                        0x00FF
#define MMI_IDLE_AUX_MASK                         0x8000

#define IS_NODE_AUX(n)                            ((((uint16)n) & MMI_IDLE_AUX_MASK) == MMI_IDLE_AUX_MASK) 
#define IS_NODE_EMPTY(n)                          ((((uint16)n) | MMI_IDLE_INVALID_VAL) == MMI_IDLE_INVALID_VAL)
#define IS_NODE_WIDGET(n)                         ((((uint16)n) & MMI_IDLE_WIDGET_MASK) == MMI_IDLE_WIDGET_MASK)
#define IS_NODE_ICON(n)                           ((((uint16)n) & MMI_IDLE_ICON_MASK) == MMI_IDLE_ICON_MASK) 
#define GET_NODE_ID(n)                            (((uint16)n)  & MMI_IDLE_ITEM_MASK)

#define SET_NODE_EMPTY(n)                         {n = MMI_IDLE_INVALID_VAL;}       
#define SET_NODE_WIDGET(widget_id, grid_val)      {grid_val = ((widget_id & 0x00FF) | MMI_IDLE_WIDGET_MASK); };
#define SET_NODE_WIDGET_AUX(widget_id, grid_val)  {grid_val = ((widget_id & 0x00FF) | MMI_IDLE_WIDGET_MASK | MMI_IDLE_AUX_MASK); };                                        
#define SET_NODE_SHORTCUT(icon_index, grid_val)   {grid_val = ((icon_index & 0x00FF) | MMI_IDLE_ICON_MASK);};
#endif

//pb widget数目
#define MMIWIDGET_PB_WIDGET_NUM           5
#define MMIGRID_PB_WIDGET_NUM             10
#define MMIGRID_MEMO_WIDGET_NUM             5

#ifdef MAINLCD_DEV_SIZE_240X400
#ifdef MMI_PDA_SUPPORT
//#define MMI_IDLE_THUMBVIEW_SLIDE/*性能不行，跑不起来，先关闭*/
#endif
#define MMI_THUMBVIEW_MAX_DIS_PAGE        10
#define MMI_THUMBVIEW_CENTER_INTERNAL     210
#define MMI_THUMBVIEW_TOTAL_LEN           (MMI_THUMBVIEW_CENTER_INTERNAL*MMI_GRID_PAGE_NUM)
#define MMI_THUMBVIEW_CENTER_Y            170         
#define MMI_THUMBVIEW_MOVE_X_MIN_PIX      4
#define MMI_THUMBVIEW_MAX_ALPHA           255
#define MMI_THUMBVIEW_MIN_ALPHA           0
#define MMI_THUMBVIEW_MAX_W           190
#define MMI_THUMBVIEW_MAX_H           290
#define MMI_THUMBVIEW_MIN_W           10
#define MMI_THUMBVIEW_MIN_H           5
#else
#define MMI_THUMBVIEW_MAX_DIS_PAGE        10
#define MMI_THUMBVIEW_CENTER_INTERNAL     210
#define MMI_THUMBVIEW_TOTAL_LEN           (MMI_THUMBVIEW_CENTER_INTERNAL*MMI_GRID_PAGE_NUM)
#define MMI_THUMBVIEW_CENTER_Y            170         
#define MMI_THUMBVIEW_MOVE_X_MIN_PIX      4
#define MMI_THUMBVIEW_MAX_ALPHA           255
#define MMI_THUMBVIEW_MIN_ALPHA           0
#define MMI_THUMBVIEW_MAX_W           190
#define MMI_THUMBVIEW_MAX_H           290
#define MMI_THUMBVIEW_MIN_W           10
#define MMI_THUMBVIEW_MIN_H           5
#endif
                           
/*---------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                   */
/*---------------------------------------------------------------------------*/

typedef struct 
{
    void   *data_ptr;
    uint32  data_type;  //0: RGB565, 1: ARGB888
    uint16  width;
    uint16  height;
    int16   start_x;
    int16   start_y;
}CLOCK_ALPHA_IMG_T;

typedef enum
{
    MMIWIDGET_INVALID_ID = 0,

#ifdef MMIWIDGET_SUPPORT
    //MMIWIDGET_GAME_ID,
    MMIWIDGET_PB_ID,
    MMIWIDGET_CUSTOM_WORD_ID,   
    MMIWIDGET_NEWSMS_ID, 
    MMIWIDGET_CALLLOG_ID,
    MMIWIDGET_ELVE_ID,
    MMIWIDGET_AUTOANSWER_LOG_ID,
#endif
#ifdef WEATHER_SUPPORT
    MMIWIDGET_WEATHER_ID,
    MMIWIDGET_WEATHER2_ID,
#endif
    MMIWIDGET_MP3_ID,
    MMIWIDGET_CLOCK_ID,
#if 0
    MMIWIDGET_MEMO_ID,
#else
    MMIWIDGET_MEMO_ID,
    MMIWIDGET_MEMO_ID0,
    MMIWIDGET_MEMO_ID_MAX = (MMIWIDGET_MEMO_ID0 + MMIGRID_MEMO_WIDGET_NUM - 1),
#endif
    MMIWIDGET_SHORTCUT_MENU_ID,
    MMIWIDGET_CALENDAR_ID, 
    MMIWIDGET_SCHEDULE_ID,
    MMIWIDGET_NETWORK_NAME_ID,
    MMIWIDGET_FM_ID,
    MMIWIDGET_ALARM_ID,
    MMIWIDGET_SHORTCUT_SET_ID,
    MMIWIDGET_ACLOCK_ID,    
    MMIWIDGET_ACLOCK_ID2,    
    MMIWIDGET_ACLOCK_ID3,
    MMIWIDGET_SEARCH_ID,
    MMIWIDGET_TIME_ID,
#ifdef MMI_ENABLE_DCD
    MMIWIDGET_DCD_ID,
#endif
    MMIWIDGET_DUAL_CLOCK_ID,
    MMIWIDGET_DUAL_ACLOCK_ID, 
#ifdef MMI_GRID_IDLE_SUPPORT
    MMIWIDGET_IMG_VIEWER_ID,
    MMIWIDGET_NETSEARCH_ID, 
    MMIWIDGET_COUNTEDTIME_ID, 
    MMIWIDGET_CONTACT_ADD_ID,
    MMIWIDGET_CONTACT_ID0,
    MMIWIDGET_CONTACT_ID_MAX = (MMIWIDGET_CONTACT_ID0 + MMIGRID_PB_WIDGET_NUM - 1),
	
#endif

    MMIWIDGET_SPEED_DIAL_ID, 

#ifdef SNS_SUPPORT
    MMIWIDGET_SNS_SINA_ID,
    MMIWIDGET_SNS_FACEBOOK_ID,
    MMIWIDGET_SNS_TIWTTER_ID,
#endif
    MMIWIDGET_PWRSAVING_ID,
    MMIWIDGET_WIDGET_MAX,
}MMIWIDGET_WIDGET_ID_E;


typedef enum
{
    MMIWIDGET_TP_CLICK_INVALID = 0,
    MMIWIDGET_TP_CLICK_IDLE_EMPTY,           //click idle empty
    MMIWIDGET_TP_CLICK_SHORTCUT_EMPTY,       //click shortcut empty 
    MMIWIDGET_TP_CLICK_SHORTCUT_WIDGET,      //click shortcut widget
    MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET,  //click shortcut icon, which isn't in shortcut bar list.
    MMIWIDGET_TP_CLICK_ACTIVE_WIDGET,        //click active widget
    MMIWIDGET_TP_CLICK_SHORTCUT_BTN,         //click shortcut button
    MMIWIDGET_TP_CLICK_MAX
}MMIWIDGET_TP_CLICK_TYPE_E;

typedef enum
{
    MMIGRID_TP_CLICK_INVALID = 0,
    MMIGRID_TP_CLICK_IDLE_EMPTY,           //click idle empty
    MMIGRID_TP_CLICK_WIDGET,
    MMIGRID_TP_CLICK_LONG_WIDGET,        //click active widget
    MMIGRID_TP_CLICK_SHORTCUT,
    MMIGRID_TP_CLICK_LONG_SHORTCUT,
    MMIGRID_TP_CLICK_THUMBVIEW, 
    MMIGRID_TP_CLICK_MOVING_LAYER, 
    MMIGRID_TP_CLICK_THUMBVIEW_SLIDE_BAR,
    MMIGRID_TP_CLICK_MAX
}MMI_GRID_TP_CLICK_TYPE_E;

typedef enum
{
    MMIWIDGET_TP_MOVE_NONE = 0,
    MMIWIDGET_TP_MOVE_BAR_TO_BAR,
    MMIWIDGET_TP_MOVE_ICON_ON_BAR,    
    MMIWIDGET_TP_MOVE_BAR_TO_IDLE,
    MMIWIDGET_TP_MOVE_IDLE_TO_BAR,
    MMIWIDGET_TP_MOVE_IDLE_TO_IDLE,
    MMIWIDGET_TP_MOVE_BAR_BTN,
    MMIWIDGET_TP_MOVE_PAGE,    
    MMIWIDGET_TP_MOVE_MAX
}MMIWIDGET_TP_MOVE_TYPE_E;

typedef enum
{
    MMIGRID_TP_MOVE_NONE = 0,
    MMIGRID_TP_MOVE_IDLE_TO_IDLE = 1,
    MMIGRID_TP_MOVE_PAGE = 2,
    MMIGRID_TP_MOVE_THUMBVIEW = 4,    
    MMIGRID_TP_MOVE_MAX
}MMI_GRID_TP_MOVE_TYPE_E;

typedef enum
{
    MMIWIDGET_ELVE_DISPLAY_INVALID,
    MMIWIDGET_ELVE_NORMAL_DISPLAY,
    MMIWIDGET_ELVE_CLICK_DISPLAY,
    MMIWIDGET_ELVE_MOVING_DISPLAY
}MMIWIDGET_ELVE_DISPLAY_E;

//enum: two list in idle widget system.
typedef enum
{
    MMI_WIDGET_LIST_NONE,
    MMI_WIDGET_SHORTCUT_LIST,
    MMI_WIDGET_ACTIVE_LIST,
    MMI_WIDGET_LIST_MAX
}MMIWIDGET_LIST_E;

typedef enum
{
    MMI_WIDGET_INVALID_AREA,
    MMI_WIDGET_TP_BAR_AREA,
    MMI_WIDGET_TP_BAR_BTN_AREA,
    MMI_WIDGET_TP_BAR_EX_AREA,
    MMI_WIDGET_TP_IDLE_AREA
}MMIWIDGET_TP_AREA_E;

//widget控制id
typedef enum
{
    //目前只支持idle界面widget
    MMIWIDGET_CTRL_IDLE_ID,

    MMIWIDGET_CTRL_MAX
}MMIWIDGET_CTRL_E;

typedef enum
{
  MMIWIDGET_ROLLBACK_UP   = 1,
  MMIWIDGET_ROLLBACK_DOWN = 2,   
}MMIWIDGET_ROLLBACK_DIRECT_E;

typedef enum
{
    MMIWIDGET_SLIDE_UP_TO_DOWN   = 1,
    MMIWIDGET_SLIDE_DOWN_TO_UP   = 2,   
}MMIWIDGET_SLIDE_DIRECT_E;

typedef enum
{
    MMIWIDGET_SHORTCUT_VIBARTE_LEFT,
    MMIWIDGET_SHORTCUT_VIBARTE_RIGHT
}MMIWIDGET_VIBRATE_DIRECT_T;

typedef enum
{
   MMIWIDGET_PEN_MOVE_INVALID,
   MMIWIDGET_PEN_MOVE_LEFT,
   MMIWIDGET_PEN_MOVE_RIGHT
}MMIWIDGET_PEN_MOVE_DIRECT_T;

typedef enum
{
    MMIGRID_SOFTKEY_BG,
    MMIGRID_SOFTKEY_LEFT ,
    MMIGRID_SOFTKEY_MIDDLE,
    MMIGRID_SOFTKEY_RIGHT,
    MMIGRID_SOFTKEY_ADD_1,
    #ifdef UPB_IDLE_SUPPORT
    MMIGRID_SOFTKEY_ADD_2,
    #endif
    MMIGRID_SOFTKEY_MAX
}MMIGRID_SOFTKEY_TYPE_E;

typedef enum
{
    MMIGRID_ITEM_TYPE_INVALID,
    MMIGRID_ITEM_TYPE_WIDGET,
    MMIGRID_ITEM_TYPE_ICON,
    MMIGRID_ITEM_TYPE_FOLDER,
    MMIGRID_ITEM_TYPE_PAGE_BAR,
    MMIGRID_ITEM_TYPE_MAX
}MMIGRID_ITEM_TYPE_E;

typedef enum
{
    MMIGRID_RUN_MODE_NORMAL,
    MMIGRID_RUN_MODE_THUMBVIEW,
    MMIGRID_RUN_MODE_MOVABLE,
    MMIGRID_RUN_MODE_EDITING,   //编辑状态，相对于normal状态,千变主题并且没有前2个状态    
    MMIGRID_RUN_MODE_THUMBVIEW_SLIDE,//缩略图滑动模式
    MMIGRID_RUN_MODE_MAX
}MMIGRID_RUN_MODE_E;

typedef enum
{
    CONTACT_INIT,
    CONTACT_EMPTY,
    CONTACT_NOTEMPTY,    
    CONTACT_HASPHOTO,
}CONTACT_STATE_E;

typedef enum
{
    MMI_IDLE_WIDGET,
    MMI_IDLE_GRID,
    MMI_IDLE_QBTHEME,
}MMI_IDLE_TYPE_E;

typedef struct
{
    uint32                  menu_num;
    MMIMAINMENU_ITEM_INFO_T grid_menu[MMIMENU_MAIN_MAX_NUM];
}MMIGRID_MENU_INFO_T;


// Widget move info
typedef struct
{
    MMIWIDGET_TP_CLICK_TYPE_E     click_type;
    uint32                        click_widget_id; 
    uint32                        click_widget_bar_pos;
    MMIWIDGET_PEN_MOVE_DIRECT_T   pen_move_dir;
}MMIWIDGET_TP_MOVE_INFO_T;

// Widget move info
typedef struct
{
    MMI_GRID_TP_CLICK_TYPE_E      click_type;
    uint32                        click_id;
    MMIWIDGET_PEN_MOVE_DIRECT_T   pen_move_dir;    
}MMI_GRID_TP_MOVE_INFO_T;

// Widget save info
typedef struct
{
    uint32 widget_id;
    GUI_POINT_T save_pos;
}MMIWIDGET_SAVE_ITEM_T;

// icon save item info
typedef struct
{
    MMIMAINMENU_ITEM_INFO_T icon_info;
    GUI_POINT_T             save_pos;
}MMI_IDLE_SC_ICON_NV_T;

// Widget save info
typedef struct
{
    uint32                  item_num;
    MMIWIDGET_SAVE_ITEM_T   item_info[MMIWIDGET_MAX_SHORTCUT_NUM];
}MMIWIDGET_SAVE_ITEM_LIST_T;

// Widget save info
typedef struct
{
    uint8                       cur_page_index;   //当前页
    int16                       shortcut_offset;      //快捷栏方向的偏移量（像素）
    MMIWIDGET_SAVE_ITEM_LIST_T  widget_shortcut_list;
    MMIWIDGET_SAVE_ITEM_LIST_T  widget_active_list;  //多页的active widget应用
}MMIWIDGET_SAVE_INFO_T;

#if defined MMI_GRID_IDLE_SUPPORT
// Widget save info
typedef struct
{
    BOOLEAN                     is_nv_set;
    uint8                       cur_page_index;   //当前页    
    uint8                       page_num;          //added this for qbtheme idle
    MMIWIDGET_SAVE_ITEM_LIST_T  widget_shortcut_list;    
    uint16                      sc_grid_data_tab[MAX_PAGE_NUM][MMI_IDLE_GRID_PAGE_CELL_NUM];
    MMI_IDLE_SC_ICON_NV_T       icon_item_info[MAX_PAGE_NUM][MMI_IDLE_GRID_PAGE_CELL_NUM];
}MMI_GRID_SAVE_INFO_T;
#endif

#ifdef BAR_PHONE_WIDGET 

typedef MMI_RESULT_E (*WIDGET_SOFTKEY_CALLBACK_FUNC)(void);

typedef struct
{
    uint32          widget_id;      //widget标示
    MMI_WIN_ID_T    win_id; //窗口id
    MMI_IMAGE_ID_T normal_icon_id;
    MMI_IMAGE_ID_T pressed_icon_id;
    MMI_TEXT_ID_T  normal_text_id;
    MMI_TEXT_ID_T  pressed_text_id;
}MMI_WIDGET_SOFTKEY_INFO_T;
#endif

typedef struct
{
    uint32          widget_id;      //widget标示
    MMI_RESULT_E    (*HandleWidgetWinMsg)(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
    MMI_RESULT_E    (*DisplayWidgetCondition)(void);  //just for dynamic widiget item to diplay or hide.
    void            (*ResetChildWinItem)(void);
    void            (*GetWidgetImage)(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);
    MMI_WIN_ID_T    win_id; //窗口id
    MMI_IMAGE_ID_T  small_icon_id;     //缩略图图标
    MMI_IMAGE_ID_T  large_bg_img;         //背景图（用背景图的宽高生成widget宽，高）
    MMI_TEXT_ID_T   text_id;    
    GUI_POINT_T     start_pos;
    BOOLEAN         is_dynamic;       //flag to identify the item is dynamic or static.
}MMIWIDGET_ITEM_INFO_T;


// widget item data
typedef struct _mmiwidget_item_tag
{
    MMIWIDGET_ITEM_INFO_T       item_info;          //item的详细信息
    BOOLEAN                     is_active;          //激活显示为放大效果图，非激活显示为缩略图藏在shortcut中
    GUI_LCD_DEV_INFO            lcd_dev_cache;      //绘图cache，绘图时首先画到cache中，然后built到需要的层上
    struct _mmiwidget_item_tag  *zorder_next_ptr;   //重叠时显示在上面的item
    struct _mmiwidget_item_tag  *zorder_prev_ptr;   //重叠时显示在下面的item
}MMIWIDGET_ITEM_T;

typedef struct 
{
    uint32 widget_id;       //shortcut中的item信息
}MMIWIDGET_SHORTCUT_ITEM_T;

typedef struct 
{
    uint32                      shortcut_num;    //shortcut item的数目
    MMIWIDGET_SHORTCUT_ITEM_T   shortcut_item[MMIWIDGET_MAX_SHORTCUT_NUM];    //所有shortcut item
    GUI_POINT_T                 shortcut_item_pos[MMIWIDGET_MAX_SHORTCUT_NUM];  //all shortcut item pos on shortcut bar.
    uint32                      click_pos;                                  //正在移除的位置
    BOOLEAN                     is_shortcut_on;     //shortcut是否展开
}MMIWIDGET_SHORTCUT_T;

typedef struct _WIDGET_SLIDE_ANIM_T
{
    int32   slide_flag;    //0:no slide 1:tp up slide; 2: direction key slide
    GUI_POINT_T move_point;   
    int16 offset_x;
    int16 offset_y;
    uint16 page_width;
    int16  old_offset_x;
    int32 dst_page_index;
    MMIWIDGET_PEN_MOVE_DIRECT_T move_direct;
    uint8  timer_id;
}MMIWIDGET_SLIDE_ANIM_T;

typedef struct
{
    MMIMAINMENU_ITEM_INFO_T   icon_info;
    uint16                    icon_num;
}MMI_IDLE_SC_ICON_T;

typedef struct
{
    MMI_STRING_T       path_string;    
    MMI_IMAGE_ID_T     small_icon_id;     //缩略图图标
    GUI_POINT_T        start_pos;
}MMI_IDLE_SC_FOLDER_T;

#ifdef MMI_GRID_IDLE_SUPPORT                                                                         
typedef struct 
{
    uint16 valid_cell_array[MMI_IDLE_GRID_PAGE_CELL_NUM + 1];
}MMI_GRID_CELL_T;

typedef struct _GRID_CELL_INFO_
{
    BOOLEAN              is_valid;
    uint16               page;
    MMI_GRID_CELL_T      filled_grid_cell;
}MMI_GRID_FILLED_INFO_T;

typedef struct 
{
    GUI_RECT_T     rc;
    MMI_IMAGE_ID_T normal_img_id;
    MMI_IMAGE_ID_T pressed_img_id;
    MMI_TEXT_ID_T  normal_text_id;
    MMI_TEXT_ID_T  pressed_text_id;
    BOOLEAN        is_pressed;
}MMI_GRID_SOFTKE_INFO_T;

typedef struct
{
    BOOLEAN  is_valid;
    uint16   page_idx;
    GUI_RECT_T old_rect;
}MMI_GRID_HIGHT_RECT_T;

// grid shortcut position
typedef struct
{
    uint32 menu_id;
    uint16 col_index;
    uint16 row_index;
    uint16 preload_page;
}MMIGRID_SHOTRCUT_PRELOAD_POSITION_T;

// grid widget position
typedef struct
{
    uint32 widget_id;
    uint16 col_index;
    uint16 row_index;
    uint16 preload_page;
}MMIGRID_WIDGET_PRELOAD_POSITION_T;
#endif

// Widget Control Structure
typedef struct _gui_widget_ctrl_tag
{
    MMIWIDGET_CTRL_E            ctrl_id;
    GUI_BOTH_RECT_T             both_rect;
	MMI_WIN_ID_T			    win_id; // window id
	uint32					    max_item_num;          // the number of maximum widget item 
	uint32					    total_item_num;        // the number of total widget item
    uint32                      active_item_num;    //active widget数目
    MMITHEME_WIDGET_T           display_theme;

    //状态
    MMIWIDGET_SHORTCUT_T        shortcut;          //shortcut
    MMIWIDGET_ITEM_T           *item_ptr;          //item
    MMIWIDGET_ITEM_T           *zorder_item_ptr;   //刷新，TP消息响应顺序
    uint8                       cur_page_index;     //当前页

    MMIWIDGET_TP_MOVE_TYPE_E    widget_mov_type;       //move type to call various move interaface.
    MMIWIDGET_TP_MOVE_INFO_T    tp_move_info;  //tp move info
    GUI_POINT_T                 save_point;
    GUI_POINT_T                 down_point;
    BOOLEAN                     is_hit_msg;    //点按（点击后没有移动，然后抬起）
    GUI_LCD_DEV_INFO            wallpaper_layer;     //wallpaper layer.
    GUI_LCD_DEV_INFO            widget_layer;   //移动widget所在层
    GUI_LCD_DEV_INFO            page_layer;
    GUI_LCD_DEV_INFO            bar_layer;      //widget bar layer.
    GUI_LCD_DEV_INFO            elve_layer;     //desktop elve layer.
    GUI_LCD_DEV_INFO            softkey_layer;  //softkey layer.
    GUI_LCD_DEV_INFO            stb_layer;      //status bar layer.
    GUI_LCD_DEV_INFO            title_layer; //page title layer.
    uint16                      tp_move_next_interval;   //每处理一次tp_move消息隔tp_move_next_interval个消息再处理下一个
    BOOLEAN                     is_child_win_btn;     //click child window button.
    BOOLEAN                     is_child_win_tp_move; //child window tp move is valid.
    int16                       shortcut_offset; 
//    uint16                      page_width;     //页宽
    BOOLEAN                     is_main_screen_landscape;       //横屏
    int16                       page_layer_offset_x;  //TP Down 记录下来
    int16                       page_layer_offset_y;  //TP Down 记录下来    
    BOOLEAN                     tp_move_is_valid;       //MOVE 有效的标志 
    MMIWIDGET_SLIDE_ANIM_T      slide_anim;
} MMIWIDGET_CTRL_T;

#if defined MMI_GRID_IDLE_SUPPORT || defined QBTHEME_SUPPORT
// idle grid data structure
typedef struct 
{
    MMIWIDGET_CTRL_E            ctrl_id;
    GUI_BOTH_RECT_T             both_rect;
    GUI_RECT_T                  display_rect;
	MMI_WIN_ID_T			    win_id;
	MMI_IDLE_TYPE_E             cur_idle_type;
	int32					    max_item_num;             // the number of maximum widget item 
	int32					    total_item_num;           // the number of total widget item
    int32                       active_item_num;          //active widget数目
    MMITHEME_WIDGET_T           display_theme;

    int32                       page_num;       //current page num,because adding/removing page,page num is not constant
    int32                       cur_page_index;           //current page index.    
    int32                       new_page_index;           //new page index.
    //int32                       cur_page_layer_idx;       //current page layer index.
    int32                       page_layer_size ;       
    int32                       wallpaper_layer_size;      
    GUI_POINT_T                 page_layer_offset;          //PAGE LAYER OFFSET
    MMIWIDGET_SHORTCUT_T        shortcut;                 //没有添加到窗口的widget信息都保存在这里
    MMIWIDGET_ITEM_T           *item_ptr;                 //widget item        MMI_GRID_TP_MOVE_TYPE_E     widget_mov_type;          //move type to call various move interaface.
    MMI_GRID_TP_MOVE_TYPE_E     widget_mov_type;          //move type to call various move interaface.
    MMI_GRID_TP_MOVE_INFO_T     tp_move_info;             //tp move info
    GUI_POINT_T                 save_point;
    GUI_POINT_T                 down_point;
    GUI_LCD_DEV_INFO            wallpaper_layer;          
    GUI_LCD_DEV_INFO            move_layer;               
    GUI_LCD_DEV_INFO            page_layer;
    GUI_LCD_DEV_INFO            page_cache[PAGE_LAYER_SIZE];	//当PAGE_LAYER_SIZE > 1时，屏幕的左中右3页各有1个绘图图层，
                                                                //cache0指向middle， cahce(0 - 1)指向left，cahce(0 + 1)指向right    
    uint32                      page_cache_flag[PAGE_LAYER_SIZE];    //0: invalid; 1:valid
    GUI_LCD_DEV_INFO            softkey_layer; 
    GUI_LCD_DEV_INFO            titlebar_layer;
    GUI_LCD_DEV_INFO            recycle_layer;  
    GUI_LCD_DEV_INFO            stb_layer;      
    GUI_LCD_DEV_INFO            thumb_view_layer;         //thumbnail view layer. 
    GUI_LCD_DEV_INFO            editbg_layer;             //edit bg layer. 
    BOOLEAN                     is_child_win_btn;         //click child window button.
    BOOLEAN                     is_child_win_tp_move;     //child window tp move is valid.
    BOOLEAN                     is_main_screen_landscape; 
    BOOLEAN                     tp_move_is_valid;  
    MMIWIDGET_SLIDE_ANIM_T      slide_anim;    
    uint16                      grid_data_tab[MAX_PAGE_NUM][MMI_IDLE_GRID_PAGE_CELL_NUM];  
    GUI_RECT_T                  page_grid_rect[MMI_IDLE_GRID_PAGE_CELL_NUM];     
    uint16                      previous_grid_data_tab[MAX_PAGE_NUM][MMI_IDLE_GRID_PAGE_CELL_NUM];  //保存上一次的数据，保存NV 生成缩略图使用
    MMIGRID_MENU_INFO_T         grid_menu_info;     //主菜单中所有menu icon的信息 
    uint32                      total_icon_num;
    MMI_IDLE_SC_ICON_T         *icon_ptr;           //idle上添加的menu icon的数组
    uint32                      total_folder_num;
    MMI_IDLE_SC_FOLDER_T       *folder_ptr;
    uint16                      grid_row;
    uint16                      grid_col;  
    uint16                      grid_width;
    uint16                      grid_height;
    MMI_GRID_FILLED_INFO_T      filled_grid;
    MMI_GRID_FILLED_INFO_T      old_grid;
    BOOLEAN                     is_item_upon_recycle; 
    uint32                      previous_win_state;     //last win state  lost fucos /foucs
    MMI_GRID_SOFTKE_INFO_T      softkey_info[MMIGRID_SOFTKEY_MAX]; 
    MMIGRID_RUN_MODE_E          run_mode;                 //grid idle run on normal mode or thumbnail view mode etc.
    BOOLEAN                     is_thumb_showed;
    BOOLEAN                     is_editable;
    MMI_IMAGE_ID_T              delect_icon_id;           //delete icon
    MMI_GRID_HIGHT_RECT_T       showed_rect_info;         //has been drawn the green rect frame.
    MMI_GRID_TP_MOVE_INFO_T     movable_tp_info;          //tp move info
//added for qbthem idle    
    GUI_LCD_DEV_INFO            thumb_move_layer;           //thumb move layer when in dockbar area
    GUI_LCD_DEV_INFO            background_layer;           //stb page tilte dockbar background,tp move不移动的都放在此层    
    int32                       current_gird;   //in editing mode, which indicates pressed grid
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
    GUI_LCD_DEV_INFO            thumb_view_slide_layer;
    GUI_LCD_DEV_INFO            thumb_view_slide_bg_layer;
#endif
#ifdef BAR_PHONE_WIDGET 
    MMI_WIDGET_SOFTKEY_INFO_T  widget_softkey;
#endif
#ifdef PDA_UI_SUPPORT_U_IDLE
    BOOLEAN                     is_Uidle_1st_mov;
#endif
} MMI_IDLE_GRID_DATA_T;
#endif

//Widget pb nv 
typedef struct MMIWIDGETPB_FAVORITE_INTERNAL_ENTRY_T
{
    uint16			entry_id;                           // 记录的id号
    uint16			storage_id_1;                           // 存储id
    uint8           number_index;
    CONTACT_STATE_E	s_is_contact_null;
}MMIWIDGETPB_FAVORITE_INTERNAL_ENTRY_T;

typedef struct MMIWIDGETPB_FAVORITE_ENTRY_T
{ 
	MMIWIDGETPB_FAVORITE_INTERNAL_ENTRY_T 		internal_entry_t[MMIWIDGET_PB_WIDGET_NUM];
} MMIWIDGETPB_FAVORITE_ENTRY_T;

typedef struct
{
    uint16  total_num; 
	MMIWIDGETPB_FAVORITE_INTERNAL_ENTRY_T 		internal_entry_t[MMIGRID_PB_WIDGET_NUM];
} MMIGRID_PB_FAVORITE_ENTRY_T;

typedef struct
{
    wchar       folder_name[MMIFILE_FULL_PATH_MAX_LEN+1]; //folder name.
    uint16      name_len;
}MMIGRID_IMGVIEWWER_FOLDER_T;

typedef struct
{
    BOOLEAN             is_invalid;//是否有效
    uint16              index;//宫格索引
    uint32              widget_id;
    uint32              shortcut_id;
    MMIGRID_ITEM_TYPE_E type;//宫格类型
}MMIGRID_GRID_INDEX_TYPE_T;

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

