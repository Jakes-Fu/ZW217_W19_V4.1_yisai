/*****************************************************************************
** File Name:        appstore_home_win_data.h                                *
** Author:           xiuyun.wang                                             *
** Date:             08/12/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define app store data api            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/12/2021           xiuyun.wang           Create                         *
******************************************************************************/
#ifndef _APPSTORE_HOME_DATA_H_
#define _APPSTORE_HOME_DATA_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_common.h"
#include "watch_common_list.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define BUTTON_ICON_FULL_PATH_MAX_LEN (100)
#define ICON_ROOT_NAME "D:\\ur_root\\c_app\\appstore\\res\\"
#define ICON_DOWNLOAD_NAME "res_aw_store_button_download_graph_blue_48dp.png"
#define ICON_DOWNLOADING_NAME "res_aw_store_button_download_graph_gray_48dp.png"
#define ICON_OPEN_NAME "res_aw_store_button_open_graph_48dp.png"
#define ICON_OFFLINE_NAME "res_aw_store_button_off_line_graph_48dp.png"
#define ICON_UNINSTALL_NAME "res_aw_store_button_uninstall_graph_red_48dp.png"
#define ICON_UPDATE_NAME "res_aw_store_button_update_graph_blue_48dp.png"
#define ICON_UPDATEING_NAME "res_aw_store_button_update_graph_gray_48dp.png"

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/*define APP Node struct information*/
typedef struct
{
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T view_app_node;
    struct APPSTORE_VIEW_APP_NODE_T* p_view_app_node_next;
}APPSTORE_VIEW_APP_NODE_T;

typedef struct
{
    uint32 app_number;
    APPSTORE_VIEW_APP_NODE_T* p_view_app_node_header;
}APPSTORE_VIEW_APP_LIST_T;

typedef struct
{
    uint16 icon_info_len;
    uint16 str1_len;
    uint16 button1_image_info_len;
    uint16 button1_str_len;
    uint16 button2_image_info_len;
    uint16 button2_str_len;
    uint16 str2_len;
}APPSTORE_VIEW_APP_NODE_ITEM_STYLE_1ICON_2STR_2BUTTON_LEN_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get List Item Data
//  Parameter: [In] list_item_index: the index of list item
//             [Out] p_list_item_data: the data used for display
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note: 获取提供给APP Store View模块显示用的数据
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Data_GetListItemData(uint16 list_item_index,
                                                                  WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_list_item_data);

/*****************************************************************************/
//  Description : Set App List
//  Parameter: [In] none
//             [Out] app_list: the list of all app
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note: 通知APP Store Data模块进行全表数据的更新
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Data_SetAppList(APPSTORE_APP_LIST_T* app_list);

/*****************************************************************************/
//  Description : Get the app num of the app list
//  Parameter: [In] none
//             [Out] none
//             [Return] app num
//  Author: mingwei.jia
//  Note: 获取APP的总数目
/*****************************************************************************/
PUBLIC uint32 APPSTORE_GetAppTotalNum(void);

#endif
