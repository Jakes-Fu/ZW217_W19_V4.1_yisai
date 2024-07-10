/*****************************************************************************
** File Name:      mmitheme_mainmenu.c                                       *
** Author:                                                                   *
** Date:           10/16/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmitheme_mainmenu.h"
#include "ctrlmainmenu_export.h"
#include "mmi_mainmenu_matrix.h"
#include "mmi_mainmenu_sweets.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#include "mmi_modu_main.h"
#include "mmi_image.h"
#include "mmi_image.h"
#include "guires.h"
#include "mmi_mainmenu_qbtheme.h"
#include "mmi_mainmenu_istyle.h"
#include "mmi_mainmenu_slide_page.h"
#include "mmi_mainmenu_export.h"
#include "mmi_mainmenu_synchronize.h"
#include "ctrlmainmenu_export.h"
#if defined QBTHEME_SUPPORT
#include "mmiqbtheme_export.h"
#endif
#include "mmi_cache.h"
#include "mmistk_export.h"
#include "mmiconnection_menutable.h"
#include "guimenu.h"

#include "mmi_app_theme_trc.h"
#include "std_header.h" /* lint !e766 */
#include "mmi_menutable.h"
#include "mmicc_export.h"
#include "mmisms_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_MENU_ITEM_EXCHANGE_TIME        100   // 交换item位置的timer
#define MMITHEME_MENU_MOVE_TIME                 500   // 图标移动timer
#define MMITHEME_MENU_MOVE_START_TIME           1000  // 自动切换页面的timer
#define MMITHEME_MENU_SHAKE_ICON_TIME           100   // 图标震动的timer

#define MMITHEME_MENU_ZOOM_OUT                  120

#define RIGHT_MARGIN            2    

#define FOLDER_THUMBNAIL_MAX    9
#define FOLDER_THUMBNAIL_ROW    3
#define FOLDER_THUMBNAIL_MARGIN 4
#define FOLDER_THUMBNAIL_WIDTH ((MENUICON_WIDTH - FOLDER_THUMBNAIL_MARGIN*2) / FOLDER_THUMBNAIL_ROW)

//缓冲2页item
#ifdef MMI_FPS_ACCELERATE_SUPPORT
#define MMITHEME_MENU_DEFAULT_ITEM_CACHE   ((MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM * 2 + 1) * MMITHEME_MENU_ITEM_WIDTH * MMITHEME_MENU_ITEM_HEIGHT * sizeof(uint32) )
#else
#define MMITHEME_MENU_DEFAULT_ITEM_CACHE   ((MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM * 3 / 2 + 1) * MMITHEME_MENU_ITEM_WIDTH * MMITHEME_MENU_ITEM_HEIGHT * sizeof(uint32) )
#endif

#define MMITHEME_MENU_FOLDER_CACHE_SIZE ((MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM * 2 + 1) * FOLDER_THUMBNAIL_WIDTH * FOLDER_THUMBNAIL_WIDTH * sizeof(uint32))


#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMITHEME_MAINMENU_GAP_WIDTH             2

#elif defined MAINLCD_SIZE_128X64
#define MMITHEME_MAINMENU_GAP_WIDTH             0

#elif defined MAINLCD_SIZE_240X320
#define MMITHEME_MAINMENU_GAP_WIDTH             5

#elif  defined MAINLCD_SIZE_240X400
#define MMITHEME_MAINMENU_GAP_WIDTH             5

#elif  defined MAINLCD_SIZE_320X480
#define MMITHEME_MAINMENU_GAP_WIDTH             5

#else
#error
#endif

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
    
/*! @enum MMITHEME_MAINMENU_DATA_TYPE
@brief 动态菜单数据类型
*/
typedef enum
{   
    MMITHEME_MAINMENU_DATA_TYPE_NONE_STYLE,      /*!< 无 */
    MMITHEME_MAINMENU_DATA_TYPE_STATIC,          /*!< 静态主菜单 */
    MMITHEME_MAINMENU_DATA_TYPE_DYNAMIC,         /*!< 动态主菜单 */
    MMITHEME_MAINMENU_DATA_TYPE_FILEFOLD,
    MMITHEME_MAINMENU_DATA_TYPE_MAX     
} MMITHEME_MAINMENU_DATA_TYPE;

//
typedef struct  
{
    MMIMAINMENU_TYPE_E type;
    CTRLMAINMENU_REGISTER_PM reg_func;
} THEMEMAINMENU_TYPE_APIS_T;

typedef struct  
{
    uint32  menu_num;
    BOOLEAN is_gray;
    BOOLEAN is_delete_icon;
}MMI_MENU_CACHE_INFO_T;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
//
static THEMEMAINMENU_TYPE_APIS_T const s_theme_mainmenu_type_apis[] =
{
#ifdef MATRIX_MAIN_MENU_SUPPORT
    {MMIMAINMENU_MATRIX_E, MMIMAINMENU_MatrixRegister},
    {MMIMAINMENU_MATRIX_TITLE_E, MMIMAINMENU_MatrixRegister},
#endif
#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)
    {MMIMAINMENU_QBTHEME_E, MMIMAINMENU_RegisterQbTheme},
#endif
#ifdef MMI_ISTYLE_SUPPORT
    {MMIMAINMENU_ISTYLE_E, MMIMAINMENU_IStyleRegister},
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    {MMIMAINMENU_GO_SLIDE_E, MMIMAINMENU_SlideRegister},
#endif
#ifdef MENU_SWEETS_SUPPORT
    {MMIMAINMENU_SWEETS_E, MMIMAINMENU_SweetsRegister},
#endif
};

// 定义滑入时的起始坐标值
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)//

#ifdef MMI_PDA_SUPPORT
LOCAL GUI_POINT_T s_bar_item_start_point[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
{
    {-150, 448}, {-25, 530}, {298, 528}, {430, 448}
};

// LOCAL GUI_POINT_T s_bar_item_start_point_h[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
// {
//     {-118, 384}, {58, 384}, {390, 416}, {572, 346}
// };
#endif

#elif defined MAINLCD_SIZE_240X320 //

#ifdef MMI_PDA_SUPPORT
LOCAL GUI_POINT_T s_bar_item_start_point[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
{
    {-150, 448}, {-25, 530}, {298, 528}, {430, 448}
};

// LOCAL GUI_POINT_T s_bar_item_start_point_h[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
// {
//     {-118, 384}, {58, 384}, {390, 416}, {572, 346}
// };
#endif

#elif defined MAINLCD_SIZE_240X400 //

#ifdef MMI_PDA_SUPPORT
LOCAL GUI_POINT_T s_bar_item_start_point[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
{
    {-150, 448}, {-25, 530}, {298, 528}, {430, 448}
};

// LOCAL GUI_POINT_T s_bar_item_start_point_h[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
// {
//     {-118, 384}, {58, 384}, {390, 416}, {572, 346}
// };
#endif

#elif defined MAINLCD_SIZE_320X480 //

#ifdef MMI_PDA_SUPPORT
LOCAL GUI_POINT_T s_bar_item_start_point[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
{
    {-150, 448}, {-25, 530}, {298, 528}, {430, 448}
};

// LOCAL GUI_POINT_T s_bar_item_start_point_h[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
// {
//     {-118, 384}, {58, 384}, {390, 416}, {572, 346}
// };
#endif

#elif defined MAINLCD_SIZE_128X64
#ifdef MMI_PDA_SUPPORT
LOCAL GUI_POINT_T s_bar_item_start_point[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
{
    {-150, 448}, {-25, 530}, {298, 528}, {430, 448}
};

// LOCAL GUI_POINT_T s_bar_item_start_point_h[MMITHEME_ISTYLEMENU_BAR_ITEM_MAX] = 
// {
//     {-118, 384}, {58, 384}, {390, 416}, {572, 346}
// };
#endif

#endif

#ifndef MMI_RES_LOW_COST
LOCAL uint32*           s_menu_item_flag_ptr    = PNULL; 
//LOCAL SCI_MUTEX_PTR     s_menu_item_mutex_ptr   = PNULL;      //互斥信号量
     
LOCAL MMI_CACHE_INFO_T* s_menu_item_cache_ptr   = PNULL;
#endif
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO  s_menu_item_layer       = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
#endif
#ifdef MMI_ISTYLE_SUPPORT
LOCAL MMI_CACHE_INFO_T* s_menu_folder_cache_ptr = PNULL;
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO  s_menu_folder_layer     = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
#endif
#endif
#ifndef MMI_RES_LOW_COST
LOCAL BOOLEAN           s_menu_item_cache_enable = FALSE;
#endif
#ifdef MMI_ISTYLE_SUPPORT
LOCAL BOOLEAN           s_menu_folder_cache_enable = FALSE;
#endif


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifndef MMI_RES_LOW_COST
/*****************************************************************************/
//  Description : get menu cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMenuCache(  
                           MMI_CACHE_INFO_T* menu_cache_ptr,
                           uint32            menu_id,
                           uint8**           data1_ptr,
                           uint32*           data1_len,
                           uint8**           data2_ptr,
                           uint32*           data2_len
                           );

/*****************************************************************************/
//  Description : set menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMenuCache(  
                           MMI_CACHE_INFO_T* menu_cache_ptr,
                           uint32            menu_id,
                           uint8*            data1_ptr,
                           uint32            data1_len,
                           uint8*            data2_ptr,
                           uint32            data2_len
                           );
#endif

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : get menu icon number
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetMenuIconNumber(  
                               MMI_MENU_ID_T       menu_id
                               );

/*****************************************************************************/
//  Description : display number icon in menu
//  Global resource dependence : 
//  Author: xinhe.yan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawMenuNumberIcon(  
                                 MMI_WIN_ID_T        win_handle,
                                 uint32              count_num,
                                 GUI_RECT_T          disp_rect,  //number icon rect
                                 GUI_LCD_DEV_INFO    lcd_dev_info //icon layer handle
                                 );



/*****************************************************************************/
//  Description : is menu item gray
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMenuItemGray(  
                             MMI_MENU_ID_T       menu_id
                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateItemLayer(
                                       BOOLEAN is_thumbnail
                                       );
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayMenuIcon(
                           MMI_HANDLE_T     win_handle,
                           GUI_LCD_DEV_INFO lcd_dev_info,
                           MMI_IMAGE_ID_T   icon_id
                           );

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 显示文件系统中存储的图片信息
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDyMenuIconInFile(
                                   GUIMAINMENU_DY_ITEM_T *item_ptr, 
                                   GUI_LCD_DEV_INFO lcd_dev_info 
                                   );
#endif

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayFolderThumbnail(  
                                  MMI_HANDLE_T win_handle,
                                  const GUIMENU_DISPLAY_ITEM_PARAM_T* param
                                  );
#endif

#ifndef MMI_RES_LOW_COST
/*****************************************************************************/
//  Description : destroy menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:垃圾回收的回调
/*****************************************************************************/
LOCAL uint32 MenuItemCacheGCCallBack(  
                                     uint32 id,
                                     void* param
                                     );
#endif

/*****************************************************************************/
//  Description : init menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void InitMenuItemCache(  
                             void
                             );

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : init menu folder cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void InitMenuFolderCache(  
                               void
                               );
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : main menu get register
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC CTRLMAINMENU_REGISTER_PM THEMEMAINMENU_GetRegister(MMIMAINMENU_TYPE_E type)
{
    CTRLMAINMENU_REGISTER_PM reg_func_ptr = PNULL;
    uint32 i = 0;

    for (i = 0; i < ARR_SIZE(s_theme_mainmenu_type_apis); i++)
    {
        if (type == s_theme_mainmenu_type_apis[i].type)
        {
            reg_func_ptr = s_theme_mainmenu_type_apis[i].reg_func;
            break;
        }
    }

    return reg_func_ptr;
}

/*****************************************************************************/
//  Description : get menu common theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetMainMenuTheme(
                                 MMITHEME_MAINMENU_COMMON_T     *theme_ptr //in/out:
                                 )
{
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    theme_ptr->is_used_slide    = TRUE;

#ifdef MMI_PDA_SUPPORT
    theme_ptr->is_circle        = FALSE;
    theme_ptr->is_has_button    = TRUE;
    theme_ptr->is_used_org      = FALSE;
    theme_ptr->is_hl_disappear  = TRUE;
#else
    theme_ptr->is_circle        = TRUE;
    theme_ptr->is_has_button    = TRUE;
    theme_ptr->is_used_org      = TRUE;
    theme_ptr->is_hl_disappear  = FALSE;
#endif

#if (defined MMI_GRID_IDLE_SUPPORT)
	theme_ptr->is_drag_item2idle = TRUE;
#else
	theme_ptr->is_drag_item2idle = FALSE;
#endif

    theme_ptr->bg_image         = 0;//IMAGE_THEME_BG;

    theme_ptr->gap_width        = MMITHEME_MAINMENU_GAP_WIDTH;
    theme_ptr->bg_color         = MMI_BACKGROUND_COLOR;

        //set default item index,when icon main menu
    theme_ptr->default_item_index = MMITHEME_ICON_MENU_DEFAULT_INDEX;
    theme_ptr->default_page_index = 0;

    //set menu item font
    theme_ptr->item_font = theme_ptr->cur_item_font = MMITHEME_MAINMENU_TEXT_FONT;
    theme_ptr->title_font = MMI_DEFAULT_TEXT_FONT;    
    theme_ptr->is_moving_page = TRUE; // 临时变量
    
    theme_ptr->focus_item_w = MENUICON_FOCUSITEM_W;
    theme_ptr->focus_item_h = MENUICON_FOCUSITEM_H;
#ifdef MMI_MAINMENU_MINI_SUPPORT
	theme_ptr->text_image_id = IMAGE_NULL;
#else
    theme_ptr->text_image_id = IMAGE_MAINMENU_ITEM_TEXT_BG;
#endif
#if defined QBTHEME_SUPPORT || defined PDA_UI_SUPPORT_MANIMENU_GO
	theme_ptr->to_idle_icon_id = IMAGE_MAINMENU_HOME_ICON;
    theme_ptr->to_idle_sle_icon_id = IMAGE_MAINMENU_HOME_ICON_ON;
    theme_ptr->delete_icon_id = IMAGE_MAINMENU_REMOVE_ICON;
    theme_ptr->delete_sel_icon_id = IMAGE_MAINMENU_REMOVE_ICON_ON;
#endif

    theme_ptr->item_exchange_period = MMITHEME_MENU_ITEM_EXCHANGE_TIME;
    theme_ptr->auto_move_period = MMITHEME_MENU_MOVE_START_TIME;
    theme_ptr->icon_shake_period = MMITHEME_MENU_SHAKE_ICON_TIME;
    theme_ptr->icon_move_start_period = MMITHEME_MENU_MOVE_TIME;

    return;
}

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetIStyleMenuInfo(
                                          MMI_HANDLE_T          win_handle,
                                          GUIMENU_ISTYLE_DATA_T *menu_info_ptr //in/out:
                                          )
{
#ifdef MMI_ISTYLE_SUPPORT
    GUI_RECT_T menu_rect = MMITHEME_GetClientRectEx( win_handle );
    GUI_RECT_T bar_rect  = {0};
    GUI_RECT_T item_rect[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0};
    uint16     item_height = ( menu_rect.bottom - menu_rect.top + 1 - MMITHEME_ISTYLEMENU_INDEX_HEIGHT ) / ( MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM + 1 );
    uint16     margin = ( menu_rect.right - menu_rect.left + 1 - MENUTEXT_WIDTH * MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM ) / ( MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM + 1) /2;
    uint32     i = 0;
    
    //I_ASSERT( PNULL != menu_info_ptr );   /*assert verified*/
    if (PNULL == menu_info_ptr)
    {
	    return FALSE;
	}
    menu_info_ptr->icon_mask_alpha    = MMI_MASK_ALPHA;
    menu_info_ptr->bg_mask_alpha    = MMI_MASK_ALPHA/2;
    menu_info_ptr->zoom_out           = MMITHEME_MENU_ZOOM_OUT;
    menu_info_ptr->item_font          = SONG_FONT_14;
    menu_info_ptr->bar_image_id       = IMAGE_ISTYLEMENU_BAR_BG;
    menu_info_ptr->bar_unsel_bg_image = IMAGE_SLIDE_MAINMENU_TITLE_OFF;
    menu_info_ptr->bar_sel_bg_image   = IMAGE_SLIDE_MAINMENU_TITLE_ON;
    menu_info_ptr->folder_image_id    = IMAGE_ISTYLEMENU_FOLDER_BG;
    menu_info_ptr->folder_bg_id       = IMAGE_ISTYLEMENU_FOLDER_BG;
    
    menu_info_ptr->page_row_num = MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM;
    menu_info_ptr->page_col_num = MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM;
    menu_info_ptr->page_num     = MMITHEME_MENUMAIN_SLIDE_PAGE_NUM;
    
    menu_info_ptr->item_width = MENUICON_WIDTH;
    menu_info_ptr->item_height = MENUICON_WIDTH;
    
    bar_rect = menu_rect;
    bar_rect.top = bar_rect.bottom - item_height - MMITHEME_ISTYLEMENU_INDEX_HEIGHT + 1;
    
    menu_rect.bottom = bar_rect.top - 1;
    
    menu_info_ptr->bar_rect = bar_rect;
    
    menu_rect.left += margin;
    menu_rect.right -= margin;

    GUI_CreateMatrixRect(&menu_rect,
        0, 
        0, 
        0, 
        0, 
        MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM , 
        MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM,
        item_rect
        );
    
    for ( i = 0; i < MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM; i++ )
    {
        item_rect[i] = GUI_GetCenterRect( item_rect[i], MENUTEXT_WIDTH, MENUICON_HEIGHT + MENUTEXT_HEIGHT + MENUNUMICON_HEIGHT/3);
    }
    
    SCI_MEMCPY( menu_info_ptr->item_rect, item_rect, sizeof(menu_info_ptr->item_rect) );
    
    bar_rect.top += MMITHEME_ISTYLEMENU_INDEX_HEIGHT;
    bar_rect.left += margin;
    bar_rect.right -= margin;

    GUI_CreateMatrixRect(&bar_rect,
        0, 
        0, 
        0, 
        0, 
        1, 
        MMITHEME_ISTYLEMENU_BAR_ITEM_MAX,
        item_rect
        );
    
    for ( i = 0; i < MMITHEME_ISTYLEMENU_BAR_ITEM_MAX; i++ )
    {
        item_rect[i] = GUI_GetCenterRect( item_rect[i], MENUTEXT_WIDTH, MENUICON_HEIGHT + MENUTEXT_HEIGHT + MENUNUMICON_HEIGHT/3);
    }
    
    SCI_MEMCPY( menu_info_ptr->bar_item_rect, item_rect, sizeof(menu_info_ptr->bar_item_rect) );
    
    
    // 计算初始化位置
//     if (MMITHEME_IsMainScreenLandscape())
//     {
//         for (i = 0; i < MMITHEME_ISTYLEMENU_BAR_ITEM_MAX; i++)
//         {
//             menu_info_ptr->bar_start_point[i] = s_bar_item_start_point_h[i];
//         }
//     }
//     else
    {
        for (i = 0; i < MMITHEME_ISTYLEMENU_BAR_ITEM_MAX; i++)
        {
            menu_info_ptr->bar_start_point[i] = s_bar_item_start_point[i];
        }
    }
    
#endif

    return TRUE;
}
#endif

#ifdef MATRIX_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN GetIconMenuInfo(
                               MMI_HANDLE_T             win_handle,
                               GUIMENU_ICONMENU_DATA_T *icon_menu_info_ptr
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
		item_width  = (menu_rect.right - menu_rect.left+ 1 - left_margin - right_margin) / row_num; //均分menu
		
		icon_menu_info_ptr->item_height   = item_height; 
		icon_menu_info_ptr->item_width    = item_width;
		icon_menu_info_ptr->page_row_num  = line_num;
		icon_menu_info_ptr->page_col_num  = row_num;
		icon_menu_info_ptr->page_num      = 1;              //icon menu仅一页
		
		icon_menu_info_ptr->bottom_margin    = margin_bottom;
		icon_menu_info_ptr->top_margin       = top_margin;
		icon_menu_info_ptr->left_margin      = left_margin;
		icon_menu_info_ptr->right_margin     = right_margin;
		icon_menu_info_ptr->icon_text_margin = icon_text_margin;
	    icon_menu_info_ptr->icon_top_space   = icon_top_space;
		
		result = TRUE;
	}
	return result;
}

/*****************************************************************************/
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN GetTitleIconMenuInfo(
									MMI_HANDLE_T            win_handle,
                                    GUIMENU_ICONMENU_DATA_T *icon_menu_info_ptr
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
		
		icon_menu_info_ptr->item_height   = item_height; 
		icon_menu_info_ptr->item_width    = item_width;
		icon_menu_info_ptr->page_row_num  = line_num;
		icon_menu_info_ptr->page_col_num  = row_num;
		icon_menu_info_ptr->page_num      = 1;              //icon menu仅一页
		
		icon_menu_info_ptr->bottom_margin    = margin_bottom;
		icon_menu_info_ptr->top_margin       = top_margin;
		icon_menu_info_ptr->left_margin      = left_margin;
		icon_menu_info_ptr->right_margin     = right_margin;
		icon_menu_info_ptr->title_text_rect  = title_text_display_rect;
		
        result = TRUE;
    }

    return result;
}
/*****************************************************************************/
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetIconMenuInfo(
										MMI_HANDLE_T            win_handle,
                                        GUIMENU_ICONMENU_DATA_T *icon_menu_info_ptr, //in/out:
                                        BOOLEAN                is_title_style
                                        )
{
    //uint32      i       = 0;
    BOOLEAN     result  = FALSE;

    if (PNULL == icon_menu_info_ptr)
    {
        return result;
    }

    if (!is_title_style)
    {
        result = GetIconMenuInfo(win_handle, icon_menu_info_ptr);
    }
    else
    {
        result = GetTitleIconMenuInfo(win_handle, icon_menu_info_ptr);
    }

#ifdef MMI_PDA_SUPPORT
    icon_menu_info_ptr->is_need_button = TRUE;
#else
    icon_menu_info_ptr->is_need_button = FALSE;
#endif
    
    return result;
}
#endif

#ifndef MAIN_MENU_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSlideMenuInfo(
										 MMI_HANDLE_T          win_handle,
                                         GUIMENU_SLIDEMENU_DATA_T *slide_menu_info_ptr
                                         )
{
    BOOLEAN result = FALSE;
	uint32 i = 0;

    GUI_RECT_T title_display_rect = MMITHEME_SLIDEMENU_TITLE_ICON_RECT;    
    GUI_RECT_T h_title_display_rect = MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT;
    int16  title_h_item_space = MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC;

    uint16 line_num = MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM;
    uint16 row_num  = MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM;
	
    GUI_RECT_T item_rect[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0};
	GUI_RECT_T icon_rect[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0};
	GUI_RECT_T text_rect[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM] = {0};
	GUI_RECT_T menu_rect   = {0};
	
    uint16     item_height = 0;
    uint16     item_width  = 0;
    uint16     margin      = 0;
    uint16     icon_text_margin  = MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V;
	uint16     margin_bottom     = MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V;
	uint16     margin_top        = MMITHEME_SLIDEMENU_TOP_MARGIN_V;
	uint16     text_height = 0;
	uint16     icon_top_space = MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V;
	
    //横屏
    if (MMITHEME_IsMainScreenLandscape())
    {        
        title_display_rect = h_title_display_rect;     
        title_h_item_space = MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC;

        line_num          = MMITHEME_MENUMAIN_SLIDE_PAGE_ROW_NUM;
        row_num           = MMITHEME_MENUMAIN_SLIDE_PAGE_LINE_NUM;
		margin_bottom     = MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H;
		margin_top        = MMITHEME_SLIDEMENU_TOP_MARGIN_H;
		icon_text_margin  = MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H;
		icon_top_space    = MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H;
    }
	
	if (PNULL != slide_menu_info_ptr)
	{
		//title
		slide_menu_info_ptr->title_image_id = IMAGE_SLIDE_MAINMENU_TITLE_BG;
		
		for (i = 0; i < MMITHEME_MENUMAIN_SLIDE_PAGE_NUM; i++)
		{
			slide_menu_info_ptr->title_unsel_bg_image[i] = IMAGE_SLIDE_MAINMENU_TITLE_OFF;//slide_menu_page_off_icon[i];
			slide_menu_info_ptr->title_sel_bg_image[i] = IMAGE_SLIDE_MAINMENU_TITLE_ON;//slide_menu_page_on_icon[i];
		}
		
		GUI_CreateMatrixRect(&title_display_rect,
			0, 
			0, 
			title_h_item_space, 
			0, 
			1, 
			MMITHEME_MENUMAIN_SLIDE_PAGE_NUM,
			slide_menu_info_ptr->title_display_rect
			);
		
		//menu
		menu_rect        = MMITHEME_GetClientRectEx(win_handle);
		menu_rect.bottom = menu_rect.bottom - ( title_display_rect.bottom - title_display_rect.top );
		item_height      = (menu_rect.bottom - menu_rect.top + 1 ) / line_num;
		margin           = (menu_rect.right - menu_rect.left + 1 - MENUTEXT_WIDTH * row_num ) / ( row_num + 1) /2;
		item_width       = (menu_rect.right - menu_rect.left+ 1 - margin*2 ) / row_num;
		
		menu_rect.left   += margin;  
		menu_rect.right  -= margin;
        menu_rect.top    += margin_top;
		menu_rect.bottom -= margin_bottom;
		
		GUI_CreateMatrixRect(&menu_rect,
			0, 
			0, 
			0, 
			0, 
			line_num , 
			row_num,
			item_rect
			);
		
		for ( i = 0; i < MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM; i++ )
		{
			//calculate icon rect
			icon_rect[i]        = item_rect[i];
			icon_rect[i].left   = (item_rect[i].left + item_rect[i].right + 1 - MENUICON_WIDTH) / 2;
            icon_rect[i].right  = icon_rect[i].left + MENUICON_WIDTH - 1;
			icon_rect[i].top    = item_rect[i].top + icon_top_space;
			icon_rect[i].bottom = item_rect[i].top + icon_top_space + MENUICON_HEIGHT - 1;
			//calculate text rect
			text_height         = item_rect[i].bottom - icon_rect[i].bottom - icon_text_margin;			
			text_rect[i]        = item_rect[i];
			text_rect[i].left   = (item_rect[i].left + item_rect[i].right + 1 - MENUTEXT_WIDTH) / 2;
            text_rect[i].right  = text_rect[i].left + MENUTEXT_WIDTH - 1; 
			text_rect[i].top    = icon_rect[i].bottom + icon_text_margin + 1;
			text_rect[i].bottom = item_rect[i].bottom + 1;
		}
		
		SCI_MEMCPY( slide_menu_info_ptr->item_rect, item_rect, sizeof(slide_menu_info_ptr->item_rect) );
		SCI_MEMCPY( slide_menu_info_ptr->item_image_display_rect, icon_rect, sizeof(slide_menu_info_ptr->item_image_display_rect) );
		SCI_MEMCPY( slide_menu_info_ptr->item_text_display_rect, text_rect, sizeof(slide_menu_info_ptr->item_text_display_rect) );
		
		slide_menu_info_ptr->item_height  = item_height;
		slide_menu_info_ptr->item_width   = item_width;
		slide_menu_info_ptr->page_row_num = line_num;
		slide_menu_info_ptr->page_col_num = row_num;
		slide_menu_info_ptr->page_num     = MMITHEME_MENUMAIN_SLIDE_PAGE_NUM;
		
		slide_menu_info_ptr->bottom_margin = title_display_rect.bottom - title_display_rect.top;
		slide_menu_info_ptr->top_margin    = margin_top;
		
		slide_menu_info_ptr->left_margin  = margin;
		slide_menu_info_ptr->right_margin = margin;
		
		result = TRUE;
	}

    return result;
}
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetGoMenuInfo(
										 MMI_HANDLE_T          win_handle,
                                         GUIMENU_SLIDEMENU_DATA_T *slide_menu_info_ptr
                                         )
{
    BOOLEAN result = FALSE;
	uint32 i = 0;

    GUI_RECT_T title_display_rect = MMITHEME_SLIDEMENU_TITLE_ICON_RECT;    
    GUI_RECT_T h_title_display_rect = MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT;
    int16  title_h_item_space = MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC;

    uint16 line_num = MMITHEME_GOMENU_PAGE_LINE_NUM;
    uint16 row_num  = MMITHEME_GOMENU_PAGE_ROW_NUM;
	
    GUI_RECT_T item_rect[MMITHEME_GOMENU_ITEM_MAX_NUM] = {0};
	GUI_RECT_T icon_rect[MMITHEME_GOMENU_ITEM_MAX_NUM] = {0};
	GUI_RECT_T text_rect[MMITHEME_GOMENU_ITEM_MAX_NUM] = {0};
	GUI_RECT_T menu_rect   = {0};
	
    uint16     item_height = 0;
    uint16     item_width  = 0;
    uint16     margin      = 0;
    uint16     icon_text_margin  = MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_V;
	uint16     margin_bottom     = MMITHEME_SLIDEMENU_BOTTOM_MARGIN_V;
	uint16     margin_top        = MMITHEME_SLIDEMENU_TOP_MARGIN_V;
	uint16     text_height = 0;
	uint16     icon_top_space = MMITHEME_SLIDEMENU_ICON_TOP_SPACE_V;
	
    //横屏
    if (MMITHEME_IsMainScreenLandscape())
    {        
        title_display_rect = h_title_display_rect;     
        title_h_item_space = MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC;

        line_num          = MMITHEME_GOMENU_H_PAGE_LINE_NUM;
        row_num           = MMITHEME_GOMENU_H_PAGE_ROW_NUM;
		margin_bottom     = MMITHEME_SLIDEMENU_BOTTOM_MARGIN_H;
		margin_top        = MMITHEME_SLIDEMENU_TOP_MARGIN_H;
		icon_text_margin  = MMITHEME_SLIDEMENU_ICON_TEXT_MARGIN_H;
		icon_top_space    = MMITHEME_SLIDEMENU_ICON_TOP_SPACE_H;

    }
	
	if (PNULL != slide_menu_info_ptr)
	{
		//title
		slide_menu_info_ptr->title_image_id = IMAGE_SLIDE_MAINMENU_TITLE_BG;
		
		for (i = 0; i < MMITHEME_MENUMAIN_SLIDE_PAGE_NUM; i++)
		{
			slide_menu_info_ptr->title_unsel_bg_image[i] = IMAGE_SLIDE_MAINMENU_TITLE_OFF;//slide_menu_page_off_icon[i];
			slide_menu_info_ptr->title_sel_bg_image[i] = IMAGE_SLIDE_MAINMENU_TITLE_ON;//slide_menu_page_on_icon[i];
		}
		
		GUI_CreateMatrixRect(&title_display_rect,
			0, 
			0, 
			title_h_item_space, 
			0, 
			1, 
			MMITHEME_MENUMAIN_SLIDE_PAGE_NUM,
			slide_menu_info_ptr->title_display_rect
			);
		
		//menu
		menu_rect        = MMITHEME_GetClientRectEx(win_handle);
		menu_rect.bottom = menu_rect.bottom - ( title_display_rect.bottom - title_display_rect.top );
		item_height      = (menu_rect.bottom - menu_rect.top + 1 ) / line_num;
		margin           = (menu_rect.right - menu_rect.left + 1 - MENUTEXT_WIDTH * row_num ) / ( row_num + 1) /2;
		item_width       = (menu_rect.right - menu_rect.left+ 1 - margin*2 ) / row_num;
		
		menu_rect.left   += margin;  
		menu_rect.right  -= margin;
        menu_rect.top    += margin_top;
		menu_rect.bottom -= margin_bottom;
		
		GUI_CreateMatrixRect(&menu_rect,
			0, 
			0, 
			0, 
			0, 
			line_num , 
			row_num,
			item_rect
			);
		
		for ( i = 0; i < MMITHEME_GOMENU_ITEM_MAX_NUM; i++ )
		{
			//calculate icon rect
			icon_rect[i]        = item_rect[i];
			icon_rect[i].left   = (item_rect[i].left + item_rect[i].right + 1 - MENUICON_WIDTH) / 2;
            icon_rect[i].right  = icon_rect[i].left + MENUICON_WIDTH - 1;
			icon_rect[i].top    = item_rect[i].top + icon_top_space;
			icon_rect[i].bottom = item_rect[i].top + icon_top_space + MENUICON_HEIGHT - 1;
			//calculate text rect
			text_height         = item_rect[i].bottom - icon_rect[i].bottom - icon_text_margin;			
			text_rect[i]        = item_rect[i];
			text_rect[i].left   = (item_rect[i].left + item_rect[i].right + 1 - MENUTEXT_WIDTH) / 2;
            text_rect[i].right  = text_rect[i].left + MENUTEXT_WIDTH - 1; 
			text_rect[i].top    = icon_rect[i].bottom + icon_text_margin + 1;
			text_rect[i].bottom = item_rect[i].bottom + 1;
		}
		
		SCI_MEMCPY( slide_menu_info_ptr->item_rect, item_rect, sizeof(slide_menu_info_ptr->item_rect) );
		SCI_MEMCPY( slide_menu_info_ptr->item_image_display_rect, icon_rect, sizeof(slide_menu_info_ptr->item_image_display_rect) );
		SCI_MEMCPY( slide_menu_info_ptr->item_text_display_rect, text_rect, sizeof(slide_menu_info_ptr->item_text_display_rect) );
		
		slide_menu_info_ptr->item_height  = item_height;
		slide_menu_info_ptr->item_width   = item_width;
		slide_menu_info_ptr->page_row_num = line_num;
		slide_menu_info_ptr->page_col_num = row_num;
		slide_menu_info_ptr->page_num     = MMITHEME_MENUMAIN_SLIDE_PAGE_NUM;
		
		slide_menu_info_ptr->bottom_margin = margin_bottom;
		slide_menu_info_ptr->top_margin    = margin_top;
		
		slide_menu_info_ptr->left_margin  = margin;
		slide_menu_info_ptr->right_margin = margin;
		
		result = TRUE;
	}

    return result;
}
#endif

#ifdef QBTHEME_SUPPORT
/*****************************************************************************/
//  Description : 获取千变菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetQbthemeMenuInfo(
                                          MMI_HANDLE_T          win_handle,
                                          GUIMENU_QBTHEME_DATA_T *menu_info_ptr //in/out:
                                          )
{
    GUI_RECT_T menu_rect = MMITHEME_GetClientRectEx( win_handle );
    GUI_RECT_T bar_rect  = {0};
    GUI_RECT_T item_rect[MMITHEME_QBMENU_ITEM_MAX_NUM] = {0};
    //uint16     item_height = ( menu_rect.bottom - menu_rect.top + 1 - MMITHEME_QBTHEMEMENU_INDEX_HEIGHT ) / ( MMITHEME_QBMENU_PAGE_LINE_NUM + 1 );
    uint16     margin = MMITHEME_QBMENU_LEFT_RIGHT_MARGIN;//( menu_rect.right - menu_rect.left + 1 - MENUTEXT_WIDTH * MMITHEME_QBMENU_PAGE_ROW_NUM ) / ( MMITHEME_QBMENU_PAGE_ROW_NUM + 1) /2;
    uint32     i = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;
    
    //SCI_ASSERT( PNULL != menu_info_ptr );   /*assert verified*/
    if(PNULL == menu_info_ptr)
    {
	    return FALSE;
	}
    menu_info_ptr->icon_mask_alpha    = MMI_MASK_ALPHA;
    menu_info_ptr->bg_mask_alpha    = MMI_MASK_ALPHA;
    menu_info_ptr->zoom_out           = MMITHEME_MENU_ZOOM_OUT;
    menu_info_ptr->item_font          = SONG_FONT_14;
    menu_info_ptr->bar_image_id       = IMAGE_THEME_MAINMENU_DOCKBAR;
    menu_info_ptr->bar_unsel_bg_image = IMAGE_THEME_TAG_DEACTIVE;
    menu_info_ptr->item_bg_id = IMAGE_THEME_MAINMENU_ICON_BG;
    menu_info_ptr->bg_id = IMAGE_THEME_MAINMENU_BG;
    menu_info_ptr->is_disp_item_bg = FALSE;
    menu_info_ptr->is_need_save = FALSE;
    
    menu_info_ptr->page_row_num = MMITHEME_QBMENU_PAGE_LINE_NUM;
    menu_info_ptr->page_col_num = MMITHEME_QBMENU_PAGE_ROW_NUM;
    menu_info_ptr->page_num = MMITHEME_MENUMAIN_SLIDE_PAGE_NUM;
    
    menu_info_ptr->item_width = MENUICON_WIDTH;
    menu_info_ptr->item_height = MENUICON_WIDTH;

    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_THEME_TAG_PAGE1, win_handle);

    bar_rect = menu_rect;
#ifndef MAINLCD_SIZE_240X320
    bar_rect.top = bar_rect.bottom - MMITHEME_QBTHEMEMENU_ITEM_HEIGHT - MENUTEXT_HEIGHT - image_height + 1 - MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN;
    menu_rect.bottom = bar_rect.top - MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN;//MMITHEME_QBTHEMEMENU_MARGIN;
#else
    bar_rect.top = bar_rect.bottom - image_height + 1 - MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN;
    menu_rect.bottom = bar_rect.top - MMITHEME_QBTHEMEMENU_BAR_BOTTOM_MARGIN;
#endif    
    menu_info_ptr->bar_rect = bar_rect;
    
    menu_rect.left += margin;
    menu_rect.right -= margin;

    GUI_CreateMatrixRect(&menu_rect,
        0, 
        0, 
        0, 
        0, 
        MMITHEME_QBMENU_PAGE_LINE_NUM , 
        MMITHEME_QBMENU_PAGE_ROW_NUM,
        item_rect
        );
    
    for ( i = 0; i < MMITHEME_QBMENU_ITEM_MAX_NUM; i++ )
    {
        item_rect[i] = GUI_GetCenterRect( item_rect[i], MENUTEXT_WIDTH, MMITHEME_MENU_ITEM_HEIGHT);
    }
    
    SCI_MEMCPY( menu_info_ptr->item_rect, item_rect, sizeof(menu_info_ptr->item_rect) );
    
    bar_rect.top += image_height;

    GUI_CreateMatrixRect(&bar_rect,
        0, 
        0, 
        0, 
        0, 
        1, 
        MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX,
        menu_info_ptr->bar_item_rect
        );
    
    for ( i = 0; i < MMITHEME_QBTHEMEMENU_BAR_ITEM_MAX; i++ )
    {
        menu_info_ptr->bar_item_rect[i] = GUI_GetCenterRect( menu_info_ptr->bar_item_rect[i], MENUTEXT_WIDTH, MMITHEME_MENU_ITEM_HEIGHT );
    }
	
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : 获得文字与图片的间距
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC uint16 MMITHEME_MainmenuIconTextMargin(void)
{
	uint16 icon_text_margin = 0;
	if (MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
	{
		icon_text_margin = MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN; // menu item的icon和text间距,默认值，用户可调
		
		// 横屏状态下
		if (MMITHEME_IsMainScreenLandscape())
		{            
			icon_text_margin  = MMITHEME_QBTHEMEMENU_TEXT_ICON_MARGIN;
		}
	}
    else
	{
		icon_text_margin  = MMITHEME_ICONMENU_ICON_TEXT_MARGIN_V; // menu item的icon和text间距,默认值，用户可调
		
		// 横屏状态下
		if (MMITHEME_IsMainScreenLandscape())
		{            
			icon_text_margin  = MMITHEME_ICONMENU_ICON_TEXT_MARGIN_H;
		}        
	}
	
    return icon_text_margin;
}

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSlideMenuTitleIdxRect(
                                                 GUI_RECT_T *rect_ptr
                                                 )
{
    GUI_RECT_T rect = MMITHEME_SLIDEMENU_TITLE_ICON_RECT;
    GUI_RECT_T h_rect = MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT;

    if (MMITHEME_IsMainScreenLandscape())
    {
        rect = h_rect;
    }

    if (PNULL != rect_ptr)
    {
        *rect_ptr = rect;

        return GUI_CheckRectValid(rect);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_ResetSlideMenuItemTitleRect(
                                                    GUIMENU_SLIDEMENU_DATA_T *slide_menu_info_ptr, //in/out:
                                                    uint16 item_num
                                                    )
{
    BOOLEAN    result               = FALSE;
    int16      title_h_item_space   = MMITHEME_SLIDEMENU_TITLE_ICON_H_SPC;
    GUI_RECT_T title_display_rect   = MMITHEME_SLIDEMENU_TITLE_ICON_RECT;
    GUI_RECT_T h_title_display_rect = MMITHEME_H_SLIDEMENU_TITLE_ICON_RECT;


    if (MMITHEME_IsMainScreenLandscape())
    {
        title_display_rect = h_title_display_rect;
        
        title_h_item_space = MMITHEME_H_SLIDEMENU_TITLE_ICON_H_SPC;
    }

    if (PNULL != slide_menu_info_ptr)
    {
        GUI_CreateMatrixRect(&title_display_rect,
            0, 
            0, 
            title_h_item_space, 
            0, 
            1, 
            item_num,
            slide_menu_info_ptr->title_display_rect
            );

        result = TRUE;
    }

    return result;
}

#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetIconMenuButtonRect(void)
{
    GUI_RECT_T h_rect = MMITHEME_MENUMAIN_BUTTON_RECT_H;
    GUI_RECT_T v_rect = MMITHEME_MENUMAIN_BUTTON_RECT;
    GUI_RECT_T button_rect = {0};
    
    button_rect = v_rect;

    if (MMITHEME_IsMainScreenLandscape())
    {
        button_rect = h_rect;
    }

    return button_rect;
}

/*****************************************************************************/
//  Description : 获取滑动菜单项的位置信息
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetSlideMenuButtonRect(void)
{
    GUI_RECT_T h_rect = MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT_H;
    GUI_RECT_T v_rect = MMITHEME_MENUMAIN_SLIDEPAGE_BUTTON_RECT;
    GUI_RECT_T button_rect = {0};
    
    button_rect = v_rect;

    if (MMITHEME_IsMainScreenLandscape())
    {
        button_rect = h_rect;
    }

    return button_rect;
}

#endif


#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : get menu icon number
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetMenuIconNumber(  
                               MMI_MENU_ID_T       menu_id
                               )
{
    uint32  count_num = 0; 

    //get number of item
    if( ID_COMMUNICATION_CL == menu_id 
		|| ID_COMMUNICATION_DIAL == menu_id)
    {
        count_num = MMIAPICC_GetMissedCallNumber();
    }
    else if( ID_COMMUNICATION_MESSAGE == menu_id )
    {
        count_num = MMIAPISMS_GetAllUnreadMsgCount()
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
            + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
#endif
            ;
    }

    return count_num;
}

/*****************************************************************************/
//  Description : display number icon in menu
//  Global resource dependence : 
//  Author: xinhe.yan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawMenuNumberIcon(  
                                 MMI_WIN_ID_T        win_handle,
                                 uint32              count_num,
                                 GUI_RECT_T          disp_rect,  //number icon rect
                                 GUI_LCD_DEV_INFO    lcd_dev_info //icon layer handle
                                 )
{ 
    char    count_str[10] = {0};
    wchar   count_wstr[10]  = {0}; 
    uint32  str_len = 0;
    MMI_STRING_T    text_str  = {0};
    GUISTR_STYLE_T  text_style = {0}; 
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN;
    //BOOLEAN result = FALSE;

    if( 0 == count_num )
    {
        return FALSE;
    }

    count_num = MIN( count_num, 999 );

    //convert uint32 num to wchar
    sprintf(count_str,"%d",count_num);
    str_len = strlen(count_str);
    MMI_STRNTOWSTR(count_wstr,10,(const uint8*)count_str,10,str_len);
    text_str.wstr_ptr = count_wstr ;
    text_str.wstr_len = str_len;    
    
    //set number icon string state and style
    text_style.font_color = MMI_WHITE_COLOR;  
    
    if( count_num <10)
    {
        text_style.font=SONG_FONT_14;
    }
    else
    {
        text_style.font=SONG_FONT_12;
    }    
    
    text_style.align=ALIGN_HVMIDDLE;

    GUIRES_DisplaySabmImg(&disp_rect,&disp_rect,PNULL,win_handle,IMAGE_MAINMENU_NUMICON,&lcd_dev_info);
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,&disp_rect,&disp_rect,&text_str,&text_style,text_state,GUISTR_TEXT_DIR_AUTO);    

    return TRUE;
}



/*****************************************************************************/
//  Description : is menu item gray
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMenuItemGray(  
                             MMI_MENU_ID_T menu_id
                             )
{
    BOOLEAN result = FALSE;

    if( (MMI_MENU_ID_T)ID_CONNECTION_GPRS == menu_id )
    {
        if ( MMIAPISET_GetMultiSysSettingNum(PNULL, 0) <= 1 )
        {
            result = TRUE;
        }
    }
    else if( ID_COMMUNICATION_STK == menu_id )
    {
        if ( 0 == MMIAPISTK_GetStkCardNum( PNULL, 0 ) )
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateItemLayer(
                                       BOOLEAN is_thumbnail
                                       )
{
    GUI_LCD_DEV_INFO  item_layer = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
    UILAYER_CREATE_T  create_info = {0};

    create_info.lcd_id = GUI_MAIN_LCD_ID;

    if ( is_thumbnail )
    {
        create_info.width = create_info.height = FOLDER_THUMBNAIL_WIDTH;
    }
    else
    {
        create_info.width  = MMITHEME_MENU_ITEM_WIDTH;
        create_info.height = MMITHEME_MENU_ITEM_HEIGHT;
    }

    create_info.is_static_layer = TRUE; 
    
    UILAYER_CreateLayer(&create_info, &item_layer);

    return item_layer;
}
#endif

#ifndef MMI_RES_LOW_COST
/*****************************************************************************/
//  Description : get menu cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMenuCache(  
                           MMI_CACHE_INFO_T* menu_cache_ptr,
                           uint32            menu_id,
                           uint8**           data1_ptr,
                           uint32*           data1_len,
                           uint8**           data2_ptr,
                           uint32*           data2_len
                           )
{
    BOOLEAN result = FALSE;
    
    if ( PNULL != menu_cache_ptr )
    {
        MMI_CACHE_INT_KEY_ITEM_T cache_item = {0};
        
        if ( MMICACHE_GetItemByInt( menu_cache_ptr, menu_id, &cache_item ) )
        {
            if ( PNULL != data1_ptr
                && PNULL != data1_len  )
            {
                *data1_ptr = cache_item.data1_ptr;
                *data1_len = cache_item.data1_len;
            }
            
            if ( PNULL != data2_ptr
                && PNULL != data2_len  )
            {
                *data2_ptr = cache_item.data2_ptr;
                *data2_len = cache_item.data2_len;
            }

            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMenuCache(  
                           MMI_CACHE_INFO_T* menu_cache_ptr,
                           uint32            menu_id,
                           uint8*            data1_ptr,
                           uint32            data1_len,
                           uint8*            data2_ptr,
                           uint32            data2_len
                           )
{
    BOOLEAN result = FALSE;
    
    if ( PNULL != menu_cache_ptr )
    {
        MMI_CACHE_INT_KEY_ITEM_T cache_item = {0};
        
        cache_item.key_id = menu_id;
        cache_item.data1_ptr = data1_ptr;
        cache_item.data1_len = data1_len;
        cache_item.data2_ptr = data2_ptr;
        cache_item.data2_len = data2_len;
        
        result = MMICACHE_PutItemByInt( menu_cache_ptr, &cache_item, TRUE );
    }
    
    return result;
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayMenuIcon(
                           MMI_HANDLE_T     win_handle,
                           GUI_LCD_DEV_INFO lcd_dev_info,
                           MMI_IMAGE_ID_T   icon_id
                           )
{
    uint16     img_width = 0;
    uint16     img_height = 0;
    GUI_RECT_T icon_rect = {0, 0, MMITHEME_MENU_ITEM_WIDTH - 1, MENUICON_HEIGHT - 1};
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, icon_id, win_handle );

    icon_rect = GUI_GetCenterRect( icon_rect, img_width, img_height );
    
    //display icon
    GUIRES_DisplayImg(PNULL,
        &icon_rect,
        PNULL,
        win_handle,
        icon_id,
        &lcd_dev_info);
}

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 显示文件系统中存储的图片信息
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDyMenuIconInFile(
                                   GUIMAINMENU_DY_ITEM_T *item_ptr, 
                                   GUI_LCD_DEV_INFO lcd_dev_info 
                                   )
{
    GUI_POINT_T img_point = {0};
    GUIIMG_BITMAP_T img_map_info = {0};
    GUIIMG_DISPLAYBMP_PARAM_T display_param = {0};
    GUI_RECT_T icon_rect = {0, 0, MMITHEME_MENU_ITEM_WIDTH - 1, MENUICON_HEIGHT - 1};
    
    if (PNULL == item_ptr
        || PNULL == item_ptr->ori_data_ptr
        || PNULL == item_ptr->ori_data_ptr->dynamic_menu_info.icon_ptr)
    {
        //SCI_TRACE_LOW:"DisplayDyMenuIconInFile: input is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_MENU_1864_112_2_18_3_3_6_19,(uint8*)"");
        return;
    }

    //图片可能与主菜单下图片尺寸不统一，需要重新计算
    icon_rect = GUI_GetCenterRect(icon_rect, 
        item_ptr->ori_data_ptr->dynamic_menu_info.icon_width, 
        item_ptr->ori_data_ptr->dynamic_menu_info.icon_height);
    
    img_point.x = icon_rect.left;
    img_point.y = icon_rect.top;
    
    img_map_info.bit_ptr    = item_ptr->ori_data_ptr->dynamic_menu_info.icon_ptr;
    img_map_info.img_width  = item_ptr->ori_data_ptr->dynamic_menu_info.icon_width;
    img_map_info.img_height = item_ptr->ori_data_ptr->dynamic_menu_info.icon_height;
    img_map_info.date_type  = GUIIMG_DATE_TYPE_ARGB888;
    
    display_param.is_horizontal = FALSE;
    display_param.rect_ptr = &icon_rect;
    display_param.point_ptr = &img_point;
    display_param.bitmap_ptr = &img_map_info;
    display_param.lcd_dev_ptr = &lcd_dev_info;
    display_param.is_transparent_mode = TRUE;
    display_param.transparent_color = 0x00;
    GUIIMG_DisplayBmpEx(&display_param);
}
#endif

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayFolderThumbnail(  
                                  MMI_HANDLE_T win_handle,
                                  const GUIMENU_DISPLAY_ITEM_PARAM_T* param
                                  )
{
#ifdef UI_MULTILAYER_SUPPORT
    uint32            i = 0;
    uint16            display_num = 0;
    UILAYER_INFO_T    layer_in    = {0};
    UILAYER_INFO_T    layer_out   = {0};
    SCALE_IMAGE_IN_T  scale_in    = {0};
    SCALE_IMAGE_OUT_T scale_out   = {0};
    GUI_LCD_DEV_INFO  folder_layer = {0};
    GUI_RECT_T        icon_rect = {0, 0, MMITHEME_MENU_ITEM_WIDTH - 1, MENUICON_HEIGHT - 1};
    uint8*            buf_ptr     = PNULL;
    uint32            buf_size    = 0;
    uint8*            cache_ptr   = PNULL;
    uint32            cache_size  = 0;
#endif

    if ( PNULL == param || 0 == param->menu_item_num )
    {
        return;
    }

    InitMenuFolderCache();

#ifdef UI_MULTILAYER_SUPPORT
    icon_rect = GUI_GetCenterRect( icon_rect, MENUICON_WIDTH, MENUICON_HEIGHT );

#ifdef MMI_ISTYLE_SUPPORT
    GUIRES_DisplayImg(PNULL,
        &icon_rect,
        PNULL,
        win_handle,
        IMAGE_ISTYLEMENU_FOLDER_BG,
        &s_menu_item_layer);
#endif
#endif

    if ( !( param->format & GUIMENU_DISPLAY_ITEM_FORMAT_FOLDER_THUMBNAIL ) )
    {
        return;
    }
#ifdef UI_MULTILAYER_SUPPORT

    display_num = MIN( param->menu_item_num, GUIMENU_FILEFOLD_MAX_ITEM_NUM );
    display_num = MIN( display_num, FOLDER_THUMBNAIL_MAX );

    UILAYER_GetLayerInfo( &s_menu_folder_layer, &layer_out );
    buf_ptr  = UILAYER_GetLayerBufferPtr( &s_menu_folder_layer );
    buf_size = layer_out.layer_width * layer_out.layer_height * sizeof(uint32);
    
    folder_layer = CreateItemLayer( FALSE );
    UILAYER_GetLayerInfo( &folder_layer, &layer_in );

    scale_in.target_format        = IMGREF_FORMAT_ARGB888;
    scale_in.target_buf.size      = buf_size;
    scale_in.target_buf.ptr       = buf_ptr;
    scale_in.target_size.w        = layer_out.layer_width;
    scale_in.target_size.h        = layer_out.layer_height;
    
    layer_in.layer_height = MIN( layer_in.layer_height, MENUICON_HEIGHT );
    // src data info
    scale_in.src_format           = IMGREF_FORMAT_ARGB888;
    scale_in.src_chn.chn0.ptr     = UILAYER_GetLayerBufferPtr(&folder_layer);                
    scale_in.src_chn.chn0.size    = layer_in.layer_height*layer_in.layer_width*sizeof(uint32);
    scale_in.src_size.w           = layer_in.layer_width;
    scale_in.src_size.h           = layer_in.layer_height;
    scale_in.src_trim_rect.w      = layer_in.layer_width;
    scale_in.src_trim_rect.h      = layer_in.layer_height;
    scale_in.scale_by_software    = TRUE;
    
    if ( PNULL != scale_in.target_buf.ptr
        && PNULL != scale_in.src_chn.chn0.ptr )
    {
        for ( i = 0; i < display_num; i++ )
        {
            GUI_RECT_T          cur_rect = {0};
            MMI_MENU_ID_T       menu_id  = param->menu_id[i];
            MMI_MENU_GROUP_ID_T group_id = param->group_id[i];
            
            cur_rect.left = icon_rect.left + (int16)(( i % FOLDER_THUMBNAIL_ROW ) * FOLDER_THUMBNAIL_WIDTH + FOLDER_THUMBNAIL_MARGIN);
            cur_rect.right = cur_rect.left + FOLDER_THUMBNAIL_WIDTH - 1;
            cur_rect.top = icon_rect.top + (int16)(( i / FOLDER_THUMBNAIL_ROW ) * FOLDER_THUMBNAIL_WIDTH + FOLDER_THUMBNAIL_MARGIN);
            cur_rect.bottom = cur_rect.top + FOLDER_THUMBNAIL_WIDTH - 1;
            
            if ( GetMenuCache( s_menu_folder_cache_ptr, menu_id, &cache_ptr, &cache_size, PNULL, PNULL ) )
            {
                cache_size = MIN( cache_size, buf_size );
                SCI_MEMCPY( buf_ptr, cache_ptr, cache_size );
            }
            else
            {   
#if defined DYNAMIC_MAINMENU_SUPPORT  && !defined MMI_MAINMENU_MINI_SUPPORT
                if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == group_id)//如果是动态菜单，从缓存表中读取信息
                {
                    GUIMAINMENU_DY_ITEM_T dy_data_info = {0};
                    
                    if ( MMIMENU_DySlideGetDispInfo(menu_id, &dy_data_info) 
						&& (PNULL != dy_data_info.ori_data_ptr)
                        && dy_data_info.ori_data_ptr->dynamic_menu_info.has_icon
                        && MMIMENU_DySlideGetIcon(menu_id,dy_data_info.ori_data_ptr->dynamic_menu_info.icon_ptr)
                        )
                    {
                        DisplayDyMenuIconInFile( &dy_data_info, folder_layer );
                    }
                    else
                    {
                        DisplayMenuIcon( win_handle, folder_layer, IMAGE_MAINMENU_ICON_DYNAMIC_DEFAUL );
                    }
                    
                    if( (PNULL != dy_data_info.ori_data_ptr) && (PNULL != dy_data_info.ori_data_ptr->dynamic_menu_info.icon_ptr) )
                    {
                        //SCI_FREE( dy_data_info.ori_data_ptr->dynamic_menu_info.icon_ptr);
                    }
                }
                else//静态菜单信息读取
#endif
                {
                    CTRLMAINMENU_ITEM_T item_info = {0};// GUIMENU_GetItemPtr( group_id, menu_id );
                    MMIMAINMENU_GetItemData(menu_id, group_id, &item_info);
                    
                    DisplayMenuIcon( win_handle, folder_layer, item_info.icon.data.icon_id);//item_ptr->unselect_icon_id );
                }
                
                if( SCI_SUCCESS == GRAPH_ScaleImage( &scale_in, &scale_out ) )
                {
                    SetMenuCache( s_menu_folder_cache_ptr, param->menu_id[i], buf_ptr, buf_size, PNULL, PNULL );
                }
            }
            
            UILAYER_SetLayerPosition( &s_menu_folder_layer, cur_rect.left, cur_rect.top );
            UILAYER_BltLayerToLayer( &s_menu_item_layer, &s_menu_folder_layer, &cur_rect, TRUE );
        }
    }
    
    UILAYER_RELEASELAYER(&folder_layer);   /*lint !e506 !e774*/
#endif
}
#endif

#ifndef MMI_RES_LOW_COST
/*****************************************************************************/
//  Description : destroy menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:垃圾回收的回调
/*****************************************************************************/
LOCAL uint32 MenuItemCacheGCCallBack(  
                                     uint32 id,
                                     void* param
                                     )
{
    //GetCacheMutex();
    if ( P_APP == SCI_IdentifyThread() )
    {
        //回收缓冲区的内存
        if ( PNULL != s_menu_item_cache_ptr )
        {
            MMICACHE_Destroy( s_menu_item_cache_ptr );
            s_menu_item_cache_ptr = PNULL;
        }

        s_menu_item_cache_enable = FALSE;
    }
    //PutCacheMutex();
    
    return 0;
}
#endif

/*****************************************************************************/
//  Description : init menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void InitMenuItemCache(  
                             void
                             )
{
#ifdef UI_MULTILAYER_SUPPORT
    //先申请图层，避免大内存分配后分不到
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_menu_item_layer))
    {
        s_menu_item_layer = CreateItemLayer( FALSE );
    }
#endif

#ifndef MMI_RES_LOW_COST
    //未使能缓冲区，不进行初始化
    if ( !s_menu_item_cache_enable )
    {
        return;
    }

    if ( PNULL == s_menu_item_cache_ptr
        && !MMK_TestMemory( MMK_TEST_LOW_MEM, MMITHEME_MENU_DEFAULT_ITEM_CACHE ) )
    {
        s_menu_item_cache_ptr = MMICACHE_Init("menu item", MMITHEME_MENU_DEFAULT_ITEM_CACHE, CKT_INT);
    }
    
/* rtos do not support BLKMEM API, so invalid this part*/
    //最后注册，避免申请大内存时被垃圾回收的回调
    if ( PNULL == s_menu_item_flag_ptr )
    {
        BLKMEM_CONFIG_T config = {0};

        config.func_callback = MenuItemCacheGCCallBack;

        BLKMEM_Register( (uint32)MenuItemCacheGCCallBack, &config );

        s_menu_item_flag_ptr = BLKMEM_Malloc( (uint32)MenuItemCacheGCCallBack, sizeof(uint32));
    }

    s_menu_item_cache_enable = FALSE;
#endif
}

/*****************************************************************************/
//  Description : init menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:使能缓冲区
/*****************************************************************************/
PUBLIC void MMITHEME_InitMenuItemCache(  
                                       void
                                       )
{
#ifndef MMI_RES_LOW_COST
    s_menu_item_cache_enable = TRUE;
#endif
}

/*****************************************************************************/
//  Description : destroy menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DestroyMenuItemCache(  
                                          void
                                          )
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_RELEASELAYER(&s_menu_item_layer);   /*lint !e506 !e774*/
#endif

    //GetCacheMutex();

#ifndef MMI_RES_LOW_COST
    if ( PNULL != s_menu_item_cache_ptr )
    {
        MMICACHE_Destroy( s_menu_item_cache_ptr );
        s_menu_item_cache_ptr = PNULL;
    }

    //PutCacheMutex();

    //释放,反注册
     if ( PNULL != s_menu_item_flag_ptr )
     {
         BLKMEM_Free( (uint32)MenuItemCacheGCCallBack, s_menu_item_flag_ptr );
         s_menu_item_flag_ptr = PNULL;
         BLKMEM_UnRegister( (uint32)MenuItemCacheGCCallBack );
     }

    s_menu_item_cache_enable = FALSE;
#endif
}

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : init menu folder cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void InitMenuFolderCache(  
                               void
                               )
{
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_menu_folder_layer))
    {
        s_menu_folder_layer = CreateItemLayer( TRUE );
    }
#endif

    if ( !s_menu_folder_cache_enable )
    {
        return;
    }
    
    if ( PNULL == s_menu_folder_cache_ptr )
    {
        s_menu_folder_cache_ptr = MMICACHE_Init("menu folder", MMITHEME_MENU_FOLDER_CACHE_SIZE, CKT_INT);
    }

    s_menu_folder_cache_enable = FALSE;
}

/*****************************************************************************/
//  Description : init menu folder cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_InitMenuFolderCache(  
                                         void
                                         )
{
    s_menu_folder_cache_enable = TRUE;
}

/*****************************************************************************/
//  Description : destroy menu item cache
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DestroyMenuFolderCache(  
                                            void
                                            )
{
    if ( PNULL != s_menu_folder_cache_ptr )
    {
        MMICACHE_Destroy( s_menu_folder_cache_ptr );
        s_menu_folder_cache_ptr = PNULL;
    }
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_RELEASELAYER(&s_menu_folder_layer);   /*lint !e506 !e774*/
#endif
    s_menu_folder_cache_enable = FALSE;
}

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:删除缓冲区保存的item
/*****************************************************************************/
PUBLIC void MMITHEME_DeleteMenuItem(
                                    MMI_MENU_ID_T menu_id
                                    )
{
    //GetCacheMutex();
#ifndef MMI_RES_LOW_COST
    MMICACHE_RemoveItemByInt( s_menu_item_cache_ptr, menu_id );
#endif
    //PutCacheMutex();

    MMICACHE_RemoveItemByInt( s_menu_folder_cache_ptr, menu_id );
}
#endif

/*****************************************************************************/
//  Description : display menu item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:绘制菜单的通用item，内建缓冲区进行优化
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMITHEME_DrawMainmenuItem(
                                                  MMI_HANDLE_T win_handle,
                                                  CTRLMAINMENU_ITEM_T *menu_item_ptr,
                                                  const GUIMENU_DISPLAY_ITEM_PARAM_T* param
                                                  )
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_INFO_T   layer_info  = {0};
    uint8*           buf_ptr     = PNULL;
    uint32           buf_size    = 0;

    InitMenuItemCache();

    UILAYER_GetLayerInfo( &s_menu_item_layer, &layer_info );
    buf_ptr  = UILAYER_GetLayerBufferPtr( &s_menu_item_layer );
    buf_size = layer_info.layer_width * layer_info.layer_height * sizeof(uint32);

    if ( PNULL != buf_ptr )
    {
        int16            x           = 0;
        int16            y           = 0;
        GUI_RECT_T       icon_rect   = {0, 0, MMITHEME_MENU_ITEM_WIDTH - 1, MENUICON_HEIGHT - 1};
        GUI_RECT_T       text_rect   = {0};
        MMI_STRING_T     item_str    = {0};
        GUISTR_STYLE_T   str_style   = {0};
        GUISTR_STATE_T   str_state   = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS | GUISTR_STATE_EFFECT;
        GUI_POINT_T      delete_point  = {2,2};
#ifndef MMI_RES_LOW_COST
        uint8*           cache_ptr   = PNULL;
        uint32           cache_size  = 0;
#endif
        BOOLEAN          is_to_layer = FALSE;
        GUI_RECT_T       num_full_rect = icon_rect;

        if (MMIMAINMENU_MATRIX_TITLE_E != MMIAPISET_GetMainmenuStyle())
        {
            text_rect.left = 0;
            text_rect.top = icon_rect.bottom + MMITHEME_ICONMENU_ICON_TEXT_MARGIN;
            text_rect.right = MMITHEME_MENU_ITEM_WIDTH - 1;
            text_rect.bottom = MMITHEME_MENU_ITEM_HEIGHT - 1;
        }
       
        if ( PNULL != param ) 
        {
            if ( param->format & GUIMENU_DISPLAY_ITEM_FORMAT_TO_LAYER )
            {
                is_to_layer = TRUE;
            }            
        }

        str_style.align = ALIGN_HVMIDDLE;
        str_style.font = MMITHEME_MAINMENU_TEXT_FONT;
        str_style.font_color = ~MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);
        str_style.effect = FONT_EFFECT_REVERSE_EDGE;
        str_style.edge_alpha = 0x40;

        UILAYER_GetLayerPosition( &s_menu_item_layer, &x, &y );
        UILAYER_SetLayerPosition( &s_menu_item_layer, 0, 0 );

        if ( PNULL != menu_item_ptr )
        {
            MMI_MENU_ID_T menu_id = 0;
            uint16 menu_data_type = MMITHEME_MAINMENU_DATA_TYPE_STATIC;
            
            menu_id = menu_item_ptr->menu_id;

#ifdef DYNAMIC_MAINMENU_SUPPORT
            if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == menu_item_ptr->group_id)
            {
                menu_data_type = MMITHEME_MAINMENU_DATA_TYPE_DYNAMIC;
            }
            else
#endif
            if (MAINMENU_FILEFOLD_DEFAULT_GROUP_ID == menu_item_ptr->group_id)
            {
                menu_data_type = MMITHEME_MAINMENU_DATA_TYPE_FILEFOLD;
            }
                        
            if (0 != menu_id )
            {
                uint32                 old_size = 0;
                uint8*                 old_ptr  = PNULL;
                MMI_MENU_CACHE_INFO_T  cur_cache_info = {0};
                BOOLEAN                is_in_cache = FALSE;
                MMI_IMAGE_ID_T         delete_icon_id = 0;

                cur_cache_info.menu_num = GetMenuIconNumber( menu_id );

                if (0 == menu_item_ptr->group_id)
                {
                    if ( IsMenuItemGray( menu_id ) )
                    {
                        cur_cache_info.is_gray = TRUE;
                        //文字灰显
                        str_style.font_color = MMI_DARK_GRAY_COLOR;
                    }
                }
                
                if ( PNULL != param )
                {
                    if ( param->format & GUIMENU_DISPLAY_ITEM_FORMAT_DELETE_ICON )
                    {
                        cur_cache_info.is_delete_icon = TRUE;
                        #ifdef MMI_ISTYLE_SUPPORT
                        delete_icon_id = ( 0 == param->delete_icon_id ) ? IMAGE_ISTYLEMENU_DELETE_ICON : param->delete_icon_id;
                        #else
                        delete_icon_id = ( 0 == param->delete_icon_id ) ? IMAGE_ANDROIDMENU_DELETE_ICON : param->delete_icon_id;
                        #endif
                    }
                }

#ifndef MMI_RES_LOW_COST
                //GetCacheMutex();

                is_in_cache = GetMenuCache( s_menu_item_cache_ptr, menu_id, &cache_ptr, &cache_size, &old_ptr, &old_size );

                //若未命中缓冲区，则绘制
                if ( is_in_cache
                    && PNULL != cache_ptr
                    && PNULL != old_ptr
                    && buf_size == cache_size
                    && sizeof(MMI_MENU_CACHE_INFO_T) == old_size
                    && 0 == memcmp( &cur_cache_info, old_ptr, sizeof(MMI_MENU_CACHE_INFO_T) ))
                {
                    if ( !is_to_layer )
                    {
                        SCI_MEMCPY( buf_ptr, cache_ptr, cache_size );
                    }
                }
                else
#endif
                {
                    UILAYER_Clear(&s_menu_item_layer);
#if defined QBTHEME_SUPPORT
                    if (MMIAPIQBTHEME_IsIconHaveBg()
                        && ID_MAINMENU_RETURNIDLE != menu_id
                        && MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())//((MMIQBTHEME_IsIconHaveBg))
                    {
                        uint16 img_width = 0;
                        uint16 img_height = 0;
                        GUI_RECT_T bg_img_display_rc = {0};
                        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_THEME_MAINMENU_ICON_BG, win_handle);
                        //display bg icon here
                        
                        bg_img_display_rc.left   = 0;
                        bg_img_display_rc.top    = -5;
                        bg_img_display_rc.right  = MMITHEME_MENU_ITEM_WIDTH - 1;
                        bg_img_display_rc.bottom = img_height- 1 - 5;
                        
                        
                        bg_img_display_rc = GUI_GetCenterRect( bg_img_display_rc, 
                            img_width,img_height);
                        GUIRES_DisplayImg(PNULL,
                            &bg_img_display_rc,
                            PNULL,
                            win_handle,
                            IMAGE_THEME_MAINMENU_ICON_BG, 
                            &s_menu_item_layer);
                        
                    }
#endif                    
                    switch( menu_data_type )
                    {
                    case MMITHEME_MAINMENU_DATA_TYPE_STATIC:
                        DisplayMenuIcon( win_handle, s_menu_item_layer, menu_item_ptr->icon.data.icon_id );
						
						if (CTRL_TEXT_ID == menu_item_ptr->text.type )
                        {
							MMI_GetLabelTextByLang( menu_item_ptr->text.data.text_id, &item_str );
                        }
                        else
                        {
                            item_str.wstr_ptr = menu_item_ptr->text.data.str_info.wstr_ptr;
                            item_str.wstr_len = menu_item_ptr->text.data.str_info.wstr_len;
                        }
                        break;
#ifdef MMI_ISTYLE_SUPPORT                        
                    case MMITHEME_MAINMENU_DATA_TYPE_FILEFOLD:
                        DisplayFolderThumbnail( win_handle, param );
                        
                        if (CTRL_TEXT_ID == menu_item_ptr->text.type )
                        {
                            MMI_GetLabelTextByLang( TXT_FILEFOLD, &item_str );
                        }
                        else
                        {                            
							item_str.wstr_ptr = menu_item_ptr->text.data.str_info.wstr_ptr;
                            item_str.wstr_len = menu_item_ptr->text.data.str_info.wstr_len;
                        }
                        break;
#endif                        
#ifdef DYNAMIC_MAINMENU_SUPPORT
                    case MMITHEME_MAINMENU_DATA_TYPE_DYNAMIC:
                        MMIAPIMENU_DisplayDynamicMenuIcon( menu_item_ptr, 
                            &icon_rect,
                            &icon_rect,
                            &s_menu_item_layer,
                            win_handle );   
                        
                        if(CTRL_TEXT_ID == menu_item_ptr->text.type)
                        {
                            MMI_GetLabelTextByLang(TXT_UNKNOWN, &item_str);	
                        }
                        else
                        {
                            item_str.wstr_len = menu_item_ptr->text.data.str_info.wstr_len;
                            item_str.wstr_ptr = menu_item_ptr->text.data.str_info.wstr_ptr;
                            if(0 == item_str.wstr_len || PNULL == item_str.wstr_ptr)
                            {
                                MMI_GetLabelTextByLang(TXT_UNKNOWN, &item_str);						            	
                            }
                        }
                        break;
#endif
                    default:
                        break;
                    }
                    
                    //display text
                    GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&s_menu_item_layer,
                        (const GUI_RECT_T*)&text_rect,
                        (const GUI_RECT_T*)&text_rect,
                        (const MMI_STRING_T*)&item_str, 
                        &str_style, 
                        str_state,
                        GUISTR_TEXT_DIR_AUTO);
                    
                    //draw menu num
					num_full_rect.left   = icon_rect.right - MENUICON_WIDTH/2 - RIGHT_MARGIN;
					num_full_rect.right  = icon_rect.right - RIGHT_MARGIN;
                    num_full_rect.bottom = icon_rect.top + MENUICON_WIDTH/2;
                    DrawMenuNumberIcon( win_handle, cur_cache_info.menu_num, num_full_rect, s_menu_item_layer );
                    
                    //绘制删除按钮
                    if ( cur_cache_info.is_delete_icon )
                    {
                        GUIRES_DisplayImg(&delete_point, 
                            PNULL, 
                            PNULL, 
                            win_handle, 
                            delete_icon_id, 
                            &s_menu_item_layer
                            );
                    }
                    
#ifndef MMI_RES_LOW_COST
                    SetMenuCache( s_menu_item_cache_ptr, menu_id, buf_ptr, buf_size, (uint8*)&cur_cache_info, sizeof(MMI_MENU_CACHE_INFO_T) ); 
                    
                    cache_ptr = buf_ptr;
#endif
                }
                
                if ( is_to_layer )
                {
                    UILAYER_COPYBUF_PARAM_T copy_param = {0};
                    UILAYER_COPYBUF_T       copy_buf = {0};
                    if ( PNULL != param )
                    {
                        copy_param.rect = param->dst_rect;
                        copy_param.clip_rect_ptr = param->clip_rect_ptr;
                        
#ifndef MMI_RES_LOW_COST
                        copy_buf.buf_ptr   = cache_ptr;
#else
                        copy_buf.buf_ptr   = buf_ptr;
#endif
                        copy_buf.data_type = layer_info.data_type;
                        copy_buf.width     = layer_info.layer_width;
                        copy_buf.height    = layer_info.layer_height;
                        
                        UILAYER_CopyBufferToLayer( &param->dst_layer, &copy_param, &copy_buf );/*lint !e605*/
                    }                    
                }
                
                UILAYER_SetLayerPosition( &s_menu_item_layer, x, y );
            }
        }
    }

    return s_menu_item_layer;
#else
    GUI_LCD_DEV_INFO  menu_item_layer = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};
    return menu_item_layer;
#endif
}


