/*****************************************************************************
** File Name:      mmi_mainmenu_istyle.h                                     *
** Author:                                                                   *
** Date:           10/17/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_ISTYLE_H_
#define _MMI_MAINMENU_ISTYLE_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_link.h"
#include "mmitheme_mainmenu.h"
#include "mmi_mainmenu_data.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//page define
#define ISTYLE_MAX_FILEFOLD_NUM 25
#define ISTYLE_PAGE_CNT_MAX (MMITHEME_MENUMAIN_SLIDE_PAGE_NUM)
#define ISTYLE_PAGE_ITEM_CNT_MAX (MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM)

typedef struct
{
    uint32 cnt;
    MMIMAINMENU_ITEM_INFO_T item[ISTYLE_PAGE_ITEM_CNT_MAX];
} ISTYLE_PAGE_INFO_T;

typedef struct  
{
    uint32 cnt;
    ISTYLE_PAGE_INFO_T page[ISTYLE_PAGE_CNT_MAX];
} ISTYLE_PAGES_INFO_T;


//bar define
#define ISTYLE_BAR_ITEM_CNT_MAX (MMITHEME_ISTYLEMENU_BAR_ITEM_MAX)

typedef struct  
{
    uint32 cnt;
    MMIMAINMENU_ITEM_INFO_T item[ISTYLE_BAR_ITEM_CNT_MAX];
} ISTYLE_BAR_INFO_T;

//folder
#define ISTYLE_FOLDER_CNT_MAX (ISTYLE_MAX_FILEFOLD_NUM)
#define ISTYLE_FOLDER_ITEM_CNT_MAX (GUIMENU_FILEFOLD_MAX_ITEM_NUM)
#define ISTYLE_FOLDER_NAME_LEN_MAX (GUIMENU_DYMAINMENU_STR_MAX_NUM)

typedef struct
{
    uint32 menu_id;
    uint16 text[ISTYLE_FOLDER_NAME_LEN_MAX + 1];
    uint16 text_len;
    uint32 cnt;
    MMIMAINMENU_ITEM_INFO_T item[ISTYLE_FOLDER_ITEM_CNT_MAX];
} ISTYLE_FOLDER_INFO_T;

typedef struct  
{
    uint32 cnt;
    ISTYLE_FOLDER_INFO_T folder[ISTYLE_FOLDER_CNT_MAX];
} ISTYLE_FOLDERS_INFO_T;

typedef MMIMAINEMENU_PAGE_LIST_T MMIMAINEMENU_FOLDER_LIST_T;

typedef struct mmimainmenu_istyle_data_tag MMIMAINMENU_ISTYLE_DATA_T;

struct mmimainmenu_istyle_data_tag
{
    MMI_HANDLE_T                win_handle;                             //window handle
    MMI_HANDLE_T                ctrl_handle;

    BOOLEAN                     is_move_state;                          //item是否在抖动
    BOOLEAN                     is_delete_state;                        // item是否删除
    BOOLEAN                     is_moving;                              //item是否被拖动
    BOOLEAN                     is_empty_space;                         //tp up时，是否在item上
    BOOLEAN                     is_item_pressed;                        //tp down时，是否在item上
    BOOLEAN                     is_tp_pressed;                          //is tp down?
    BOOLEAN                     is_press_without_moving;                // for compiler
    BOOLEAN                     is_press_in_folder_rect;

    BOOLEAN                     is_bar_item;                            //is bar item
    uint16                      cur_page_index;                         //default 0,from 0 to max-1
    uint16                      cur_item_index;                         //current selected menu index. in option page style, it is also item index

    GUIMENU_ISTYLE_DATA_T       default_data;                         //主菜单的信息
    MMITHEME_MAINMENU_COMMON_T  theme_data;                        //main menu theme

    ISTYLE_PAGES_INFO_T         page_info;
    ISTYLE_BAR_INFO_T           bar_info;
    ISTYLE_FOLDERS_INFO_T       folder_info;

    //open folder info
    BOOLEAN                     is_folder_open;
    BOOLEAN                     is_folder_open_from_bar;
    int16                       open_folder_page_index;
    int16                       open_folder_item_index;
    int16                       open_folder_index;
    MMI_HANDLE_T                ctrl_foldername_handle;

    BOOLEAN                     is_temp_filefold_create;                //是否建立了临时文件夹icon
    int16                       temp_filefold_index;                    // 文件夹形成时代index
    

    //for istyle search
    BOOLEAN                     is_search_display;    //search page data already exist
    BOOLEAN                     is_cur_pagesearch;    //搜索页是不是当前页
    BOOLEAN                     is_search_entry;      //enter search page
    BOOLEAN                     is_search_listslide;  //record list slide vertical or menu slide horizontal
    GUI_LCD_DEV_INFO            tmp_layer_handle;       //temp layer handle

    uint16                      tmp_width;                              //移动的时候使用
    uint16                      tmp_height;                             //移动的时候使用
    uint16                      tmp_cur_item_index;                     //移动的时候使用
    uint16                      tmp_cur_page_index;                    //移动到时候使用
    BOOLEAN                     tmp_is_bar_item;                        //移动的时候使用

    uint8                       tp_timer_id;                            // 为支持TP连续移动而加的timer
    uint8                       redrew_timer_id;                     // 滑动时，slide和fling状态下，画图的timer
    uint8                       user_timer_id;                          // 应用自定义的timer
    uint8                       shake_icon_timer_id;                    // 进入抖动状态时，每次抖动的间隔
    CONTINUE_MOVE_TYPE_E        continue_move_type;                     // 连续移动的类型
    uint8                       start_move_timer_id;

    MMK_TP_SLIDE_E              slide_state;                            //current slide state
    MMI_MENU_ID_T               delete_menu_id;                         //组织状态下使用,要删除的menu_id
    GUI_POINT_T                 slide_start_point;                      //Slide时候使用
    GUI_POINT_T                 slide_pre_point;                        //Slide时候使用
    GUI_POINT_T                 pre_tp_point;                           //上一次tp move的坐标

    GUI_LCD_DEV_INFO            icon_move_layer_handle;                 //图标移动层
    GUI_LCD_DEV_INFO            page_layer_handle[GUIMENU_SLIDE_PAGE_NUM]; //页面移动层1
    GUI_LCD_DEV_INFO            titleidx_layer_handle;                  // 页面索引层
    GUI_LCD_DEV_INFO            bar_item_layer;                             // 二级菜单画item的层信息

    float                       fling_velocity;                         //滑动速度

    BOOLEAN (*U_SetPageInfo)(MMIMAINMENU_ISTYLE_DATA_T *, uint16, void *);                // 设置U动画信息
};



/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IStyleRegister(
                                          CTRLMAINMENU_PM_APIS_T *apis
                                          );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_MAINMENU_ISTYLE_H_

/*@}*/
