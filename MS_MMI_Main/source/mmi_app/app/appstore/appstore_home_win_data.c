/*****************************************************************************
** File Name:      appstore_home_win_data.c                                  *
** Author:         xiuyun.wang                                               *
** Date:           08/12/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app  store data function      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       xiuyun.wang          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_home_win_data.h"
#include "mmi_string.h"
#include "appstore_control.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL APPSTORE_VIEW_APP_LIST_T* s_p_view_app_list = PNULL;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Free View app Node space
//  Parameter: [In]  p_view_app_node: the free view app node
//             [Out] None
//             [Return] free space result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Data_FreeViewAppNode(APPSTORE_VIEW_APP_NODE_T* p_view_app_node)
{
    if(PNULL == p_view_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //icon_info:
    if( PNULL != p_view_app_node->view_app_node.icon_info.full_path_wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space icon_info.full_path_wstr_ptr");
        SCI_FREE(p_view_app_node->view_app_node.icon_info.full_path_wstr_ptr);
        p_view_app_node->view_app_node.icon_info.full_path_wstr_ptr = PNULL;
    }

    //str1:
    if( PNULL != p_view_app_node->view_app_node.str1.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space str1.wstr_ptr");
        SCI_FREE(p_view_app_node->view_app_node.str1.wstr_ptr);
        p_view_app_node->view_app_node.str1.wstr_ptr = PNULL;
    }

    //button_info1.image_info:
    if( PNULL != p_view_app_node->view_app_node.button_info1.image_info.full_path_wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space button_info1.image_info.full_path_wstr_ptr");
        SCI_FREE(p_view_app_node->view_app_node.button_info1.image_info.full_path_wstr_ptr);
        p_view_app_node->view_app_node.button_info1.image_info.full_path_wstr_ptr = PNULL;
    }

    //button_info1.str:
    if( PNULL != p_view_app_node->view_app_node.button_info1.str.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space button_info1.str.wstr_ptr");
        SCI_FREE(p_view_app_node->view_app_node.button_info1.str.wstr_ptr);
        p_view_app_node->view_app_node.button_info1.str.wstr_ptr = PNULL;
    }

    //button_info2.image_info:
    if( PNULL != p_view_app_node->view_app_node.button_info2.image_info.full_path_wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space button_info2.image_info.full_path_wstr_ptr");
        SCI_FREE(p_view_app_node->view_app_node.button_info2.image_info.full_path_wstr_ptr);
        p_view_app_node->view_app_node.button_info2.image_info.full_path_wstr_ptr = PNULL;
    }

    //button_info2.str:
    if( PNULL != p_view_app_node->view_app_node.button_info2.str.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space button_info2.str.wstr_ptr");
        SCI_FREE(p_view_app_node->view_app_node.button_info2.str.wstr_ptr);
        p_view_app_node->view_app_node.button_info2.str.wstr_ptr = PNULL;
    }

    //str2:
    if( PNULL != p_view_app_node->view_app_node.str2.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space str2.wstr_ptr");
        SCI_FREE(p_view_app_node->view_app_node.str2.wstr_ptr);
        p_view_app_node->view_app_node.str2.wstr_ptr = PNULL;
    }

    //free app node space
    SCI_FREE(p_view_app_node);
    p_view_app_node = PNULL;

    TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:free space successful");
    return APPSTORE_RES_SUCC;
}


/*****************************************************************************/
//  Description : Create View app Node space
//  Parameter: [In]  view_app_node_len
//             [Out] p_view_app_node: the view app node
//             [Return] create space result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Data_CreateViewAppNode(APPSTORE_VIEW_APP_NODE_ITEM_STYLE_1ICON_2STR_2BUTTON_LEN_T view_app_node_len, APPSTORE_VIEW_APP_NODE_T** p_view_app_node)
{
    APPSTORE_VIEW_APP_NODE_T* p_view_temp_node = PNULL;

    if(PNULL == p_view_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_view_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    p_view_temp_node = (APPSTORE_VIEW_APP_NODE_T *)SCI_ALLOC_APPZ(sizeof(APPSTORE_VIEW_APP_NODE_T) );
    if(PNULL == p_view_temp_node)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node, 0x00, sizeof(APPSTORE_VIEW_APP_NODE_T) );

    //icon_info:
    p_view_temp_node->view_app_node.icon_info.full_path_wstr_ptr = (wchar *)SCI_ALLOC_APPZ((view_app_node_len.icon_info_len + 1) * sizeof(wchar));
    if(PNULL == p_view_temp_node->view_app_node.icon_info.full_path_wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        APPSTORE_Data_FreeViewAppNode(p_view_temp_node);
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node->view_app_node.icon_info.full_path_wstr_ptr, 0x00, (view_app_node_len.icon_info_len + 1) * sizeof(wchar));

    //str1:
    p_view_temp_node->view_app_node.str1.wstr_ptr= (wchar *)SCI_ALLOC_APPZ((view_app_node_len.str1_len + 1) * sizeof(wchar));
    if(PNULL == p_view_temp_node->view_app_node.str1.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        APPSTORE_Data_FreeViewAppNode(p_view_temp_node);
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node->view_app_node.str1.wstr_ptr, 0x00, (view_app_node_len.str1_len + 1) * sizeof(wchar));

    //button_info1.image_info:
    p_view_temp_node->view_app_node.button_info1.image_info.full_path_wstr_ptr= (wchar *)SCI_ALLOC_APPZ((view_app_node_len.button1_image_info_len + 1) * sizeof(wchar));
    if(PNULL == p_view_temp_node->view_app_node.button_info1.image_info.full_path_wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        APPSTORE_Data_FreeViewAppNode(p_view_temp_node);
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node->view_app_node.button_info1.image_info.full_path_wstr_ptr, 0x00, (view_app_node_len.button1_image_info_len + 1) * sizeof(wchar));

    //button_info1.str:
    p_view_temp_node->view_app_node.button_info1.str.wstr_ptr= (wchar *)SCI_ALLOC_APPZ((view_app_node_len.button1_str_len + 1) * sizeof(wchar));
    if(PNULL == p_view_temp_node->view_app_node.button_info1.str.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        APPSTORE_Data_FreeViewAppNode(p_view_temp_node);
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node->view_app_node.button_info1.str.wstr_ptr, 0x00, (view_app_node_len.button1_str_len + 1) * sizeof(wchar));

    //button_info2.image_info:
    p_view_temp_node->view_app_node.button_info2.image_info.full_path_wstr_ptr= (wchar *)SCI_ALLOC_APPZ((view_app_node_len.button2_image_info_len + 1) * sizeof(wchar));
    if(PNULL == p_view_temp_node->view_app_node.button_info2.image_info.full_path_wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        APPSTORE_Data_FreeViewAppNode(p_view_temp_node);
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node->view_app_node.button_info2.image_info.full_path_wstr_ptr, 0x00, (view_app_node_len.button2_image_info_len + 1) * sizeof(wchar));

    //button_info2.str:
    p_view_temp_node->view_app_node.button_info2.str.wstr_ptr= (wchar *)SCI_ALLOC_APPZ((view_app_node_len.button2_str_len + 1) * sizeof(wchar));
    if(PNULL == p_view_temp_node->view_app_node.button_info2.str.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        APPSTORE_Data_FreeViewAppNode(p_view_temp_node);
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node->view_app_node.button_info2.str.wstr_ptr, 0x00, (view_app_node_len.button2_str_len + 1) * sizeof(wchar));

    //str2:
    p_view_temp_node->view_app_node.str2.wstr_ptr= (wchar *)SCI_ALLOC_APPZ((view_app_node_len.str2_len + 1) * sizeof(wchar));
    if(PNULL == p_view_temp_node->view_app_node.str2.wstr_ptr)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        APPSTORE_Data_FreeViewAppNode(p_view_temp_node);
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_node->view_app_node.str2.wstr_ptr, 0x00, (view_app_node_len.str2_len + 1) * sizeof(wchar));

    *p_view_app_node = p_view_temp_node;

    TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space successful");
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : Create View app List space
//  Parameter: [In]  None
//             [Out] p_view_app_list: the view app list
//             [Return] create space result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Data_CreateViewAppList(APPSTORE_VIEW_APP_LIST_T** p_view_app_list)
{
    APPSTORE_VIEW_APP_LIST_T* p_view_temp_list = PNULL;

    if(PNULL == p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //malloc app list
    p_view_temp_list = (APPSTORE_VIEW_APP_LIST_T *)SCI_ALLOC_APPZ(sizeof(APPSTORE_VIEW_APP_LIST_T));
    if(PNULL == p_view_temp_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:malloc space fail");
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_view_temp_list, 0x00, sizeof(APPSTORE_VIEW_APP_LIST_T));

    *p_view_app_list = p_view_temp_list;
    TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:create app list succ, app_list:0x%x", *p_view_app_list);
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : Free View app list space
//  Parameter: [In]  p_view_app_list: the free view app list
//             [Out] None
//             [Return] free space result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Data_FreeViewAppList(APPSTORE_VIEW_APP_LIST_T* p_view_app_list)
{
    APPSTORE_VIEW_APP_NODE_T* p_view_temp_node= PNULL;
    if(PNULL == p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is PNULL");
        return APPSTORE_RES_SUCC;
    }

    //free every app node in this app list first
    while(PNULL != p_view_app_list->p_view_app_node_header)
    {
        p_view_temp_node = p_view_app_list->p_view_app_node_header->p_view_app_node_next;
        APPSTORE_Data_FreeViewAppNode(p_view_app_list->p_view_app_node_header);
        p_view_app_list->p_view_app_node_header = PNULL;
        p_view_app_list->p_view_app_node_header = p_view_temp_node;
    }
    //free app list
    if(PNULL != p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:free app list:0x%x",p_view_app_list);
        SCI_FREE(p_view_app_list);
        p_view_app_list = PNULL;
    }
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : add View app node to View app list
//  Parameter: [In]  view_app_node_len
//             [In]  p_new_view_app_node
//             [In/Out]  p_view_app_list: the app list
//             [Out] None
//             [Return] add result
//  Author:mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Data_AddViewAppNodeToViewAppList(APPSTORE_VIEW_APP_NODE_ITEM_STYLE_1ICON_2STR_2BUTTON_LEN_T view_app_node_len,APPSTORE_VIEW_APP_NODE_T* p_new_view_app_node, APPSTORE_VIEW_APP_LIST_T** p_view_app_list)
{
    APPSTORE_VIEW_APP_NODE_T* p_view_temp_node = PNULL;
    APPSTORE_VIEW_APP_LIST_T* p_view_temp_list = PNULL;
    APPSTORE_VIEW_APP_NODE_T* p_view_temp_list_node = PNULL;
    //p_view_temp_list_node的前一条node：
    APPSTORE_VIEW_APP_NODE_T* p_view_pre_node = PNULL;

    if(PNULL == p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL == *p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    //get app list
    p_view_temp_list = *p_view_app_list;

    APPSTORE_Data_CreateViewAppNode(view_app_node_len,&p_view_temp_node);
    if(PNULL == p_view_temp_node)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:Create View App Node fail");
        return APPSTORE_RES_FAIL;
    }

    SCI_MEMCPY(p_view_temp_node->view_app_node.icon_info.full_path_wstr_ptr,p_new_view_app_node->view_app_node.icon_info.full_path_wstr_ptr,view_app_node_len.icon_info_len * sizeof(wchar));
    p_view_temp_node->view_app_node.icon_info.full_path_wstr_len = view_app_node_len.icon_info_len;

    SCI_MEMCPY(p_view_temp_node->view_app_node.str1.wstr_ptr,p_new_view_app_node->view_app_node.str1.wstr_ptr,view_app_node_len.str1_len * sizeof(wchar));
    p_view_temp_node->view_app_node.str1.wstr_len=view_app_node_len.str1_len;

    SCI_MEMCPY(p_view_temp_node->view_app_node.button_info1.image_info.full_path_wstr_ptr,p_new_view_app_node->view_app_node.button_info1.image_info.full_path_wstr_ptr,view_app_node_len.button1_image_info_len * sizeof(wchar));
    p_view_temp_node->view_app_node.button_info1.image_info.full_path_wstr_len = view_app_node_len.button1_image_info_len;

    SCI_MEMCPY(p_view_temp_node->view_app_node.button_info1.str.wstr_ptr,p_new_view_app_node->view_app_node.button_info1.str.wstr_ptr,view_app_node_len.button1_str_len * sizeof(wchar));
    p_view_temp_node->view_app_node.button_info1.str.wstr_len = view_app_node_len.button1_str_len;

    SCI_MEMCPY(p_view_temp_node->view_app_node.button_info2.image_info.full_path_wstr_ptr,p_new_view_app_node->view_app_node.button_info2.image_info.full_path_wstr_ptr,view_app_node_len.button2_image_info_len * sizeof(wchar));
    p_view_temp_node->view_app_node.button_info2.image_info.full_path_wstr_len = view_app_node_len.button2_image_info_len;

    SCI_MEMCPY(p_view_temp_node->view_app_node.button_info2.str.wstr_ptr,p_new_view_app_node->view_app_node.button_info2.str.wstr_ptr,view_app_node_len.button2_str_len * sizeof(wchar));
    p_view_temp_node->view_app_node.button_info2.str.wstr_len = view_app_node_len.button2_str_len;

    SCI_MEMCPY(p_view_temp_node->view_app_node.str2.wstr_ptr,p_new_view_app_node->view_app_node.str2.wstr_ptr,view_app_node_len.str2_len* sizeof(wchar));
    p_view_temp_node->view_app_node.str2.wstr_len = view_app_node_len.str2_len;

    p_view_temp_node->p_view_app_node_next = PNULL;

    //add to view app list
    p_view_temp_list_node = p_view_temp_list->p_view_app_node_header;
    if(PNULL == p_view_temp_list_node)
    {
        p_view_temp_list->p_view_app_node_header = p_view_temp_node;
    }
    else
    {
        while(PNULL != p_view_temp_list_node) //循环链表
        {
            p_view_pre_node = p_view_temp_list_node;
            p_view_temp_list_node = p_view_temp_list_node->p_view_app_node_next;
        }
    p_view_pre_node->p_view_app_node_next = p_view_temp_node;
    }
    //add app numner for app list
    p_view_temp_list->app_number++;
    TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:app number:%d in app list", p_view_temp_list->app_number);
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : get View app node from View app list
//  Parameter: [In]  app_node_index
//             [Out] p_view_app_node
//             [Return] add result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Data_GetViewAppNodeFromViewAppList(uint16 app_node_index,
                                                                                         APPSTORE_VIEW_APP_NODE_T** p_view_app_node)
{
    uint32 cur_index = 0;
    APPSTORE_VIEW_APP_NODE_T* p_view_temp_node = PNULL;

    if(PNULL == p_view_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:app_node_index:0x%x",app_node_index);
    TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:app_number:%d",s_p_view_app_list->app_number);

    if(PNULL != *p_view_app_node || app_node_index >= s_p_view_app_list->app_number)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    p_view_temp_node = s_p_view_app_list->p_view_app_node_header;
    //get app node from app list
    while(cur_index < s_p_view_app_list->app_number)
    {
        if(app_node_index != cur_index)
        {
            p_view_temp_node = p_view_temp_node->p_view_app_node_next;
            cur_index++;
        }
        else
        {
            *p_view_app_node = p_view_temp_node;
            TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:app name:%ls",p_view_temp_node->view_app_node.str1.wstr_ptr);
            TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:app name len:%d",p_view_temp_node->view_app_node.str1.wstr_len);
            break;
        }
    }
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : Get List Item Data
//  Parameter: [In] list_item_index: the index of list item
//             [Out] p_list_item_data: the data used for display
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note: 获取提供给APP Store View模块显示用的数据,need free param space in view
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Data_GetListItemData(uint16 list_item_index,
                                                                  WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_list_item_data)
{
    APPSTORE_VIEW_APP_NODE_T* p_view_app_node = PNULL;
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_temp_item_data = PNULL;

    if(PNULL == p_list_item_data)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_list_item_data)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //判断入参是否有效
    if(list_item_index >= s_p_view_app_list->app_number)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //根据入参list_item_index在app_list表中找到相应的app_node
    APPSTORE_Data_GetViewAppNodeFromViewAppList(list_item_index,&p_view_app_node);

    if(PNULL == p_view_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:Get List Item Data fail");
        return APPSTORE_RES_FAIL;
    }

    p_temp_item_data = &(p_view_app_node->view_app_node);
    *p_list_item_data = p_temp_item_data;
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : Set App List
//  Parameter: [In] app_list: the list of all app
//             [Out] none
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note: 通知APP Store Data模块进行全表数据的更新
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Data_SetAppList(APPSTORE_APP_LIST_T* p_app_list)
{
    APPSTORE_VIEW_APP_NODE_T* p_new_view_app_node = PNULL;
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
    APPSTORE_VIEW_APP_NODE_ITEM_STYLE_1ICON_2STR_2BUTTON_LEN_T view_app_node_len = {0};
    uint8 button1[BUTTON_ICON_FULL_PATH_MAX_LEN] = {0};
    uint8 button2[BUTTON_ICON_FULL_PATH_MAX_LEN] = {0};

    //判断入参
    if(PNULL == p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //view_app_list判空，不空则释放
    if(PNULL != s_p_view_app_list)
    {
        APPSTORE_Data_FreeViewAppList(s_p_view_app_list);
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:Free View App List");
        s_p_view_app_list = PNULL;
    }

    APPSTORE_Data_CreateViewAppList(&s_p_view_app_list);
    if(PNULL == s_p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:Create View App List fail");
        return APPSTORE_RES_FAIL;
    }

    //将入参更新到view_app_list
    while(s_p_view_app_list->app_number < p_app_list->app_num)
    {
        p_app_node = PNULL;
        APPSTORE_GetAppNodeFromAppList(s_p_view_app_list->app_number,*p_app_list,&p_app_node);
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:app_number:%d",s_p_view_app_list->app_number);
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:app_id:%s",p_app_node->p_app_id);
        SCI_MEMSET(button1,0,BUTTON_ICON_FULL_PATH_MAX_LEN);
        SCI_MEMSET(button2,0,BUTTON_ICON_FULL_PATH_MAX_LEN);

        switch(p_app_node->app_node_state)
        {
            case APPSTORE_APP_STATE_UNINSTALL:
            {
                sprintf(button2,"%s%s",ICON_ROOT_NAME,ICON_DOWNLOAD_NAME);
                break;
            }
            case APPSTORE_APP_STATE_INSTALLED:
            {
                sprintf(button1,"%s%s",ICON_ROOT_NAME,ICON_OPEN_NAME);
                sprintf(button2,"%s%s",ICON_ROOT_NAME,ICON_UNINSTALL_NAME);
                break;
            }
            case APPSTORE_APP_STATE_INSTALLING:
            {
                sprintf(button2,"%s%s",ICON_ROOT_NAME,ICON_DOWNLOADING_NAME);
                break;
            }
            case APPSTORE_APP_STATE_NEED_UPDATE:
            {
                sprintf(button1,"%s%s",ICON_ROOT_NAME,ICON_UPDATE_NAME);
                sprintf(button2,"%s%s",ICON_ROOT_NAME,ICON_UNINSTALL_NAME);
                break;
            }
            case APPSTORE_APP_STATE_UPDATING:
            {
                sprintf(button1,"%s%s",ICON_ROOT_NAME,ICON_UPDATEING_NAME);
                sprintf(button2,"%s%s",ICON_ROOT_NAME,ICON_UNINSTALL_NAME);
                break;
            }
            case APPSTORE_APP_STATE_OFFLINE:
            {
                sprintf(button1,"%s%s",ICON_ROOT_NAME,ICON_OFFLINE_NAME);
                sprintf(button2,"%s%s",ICON_ROOT_NAME,ICON_UNINSTALL_NAME);
                break;
            }
            default:
            {
                TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:s_p_view_app_list->app_number:0x%x",s_p_view_app_list->app_number);
                TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:p_app_node->app_node_state:0x%x",p_app_node->app_node_state);
                TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:param is error");
                return APPSTORE_RES_PARAM_ERR;
            }
        }

        view_app_node_len.icon_info_len = SCI_STRLEN(p_app_node->p_app_icon_full_name);
        view_app_node_len.str1_len = SCI_STRLEN(p_app_node->p_app_name);
        view_app_node_len.button1_image_info_len = SCI_STRLEN(button1);
        view_app_node_len.button2_image_info_len = SCI_STRLEN(button2);
        view_app_node_len.str2_len = SCI_STRLEN(p_app_node->p_app_introduction);

        APPSTORE_Data_CreateViewAppNode(view_app_node_len,&p_new_view_app_node);

        GUI_UTF8ToWstr(p_new_view_app_node->view_app_node.icon_info.full_path_wstr_ptr,view_app_node_len.icon_info_len,p_app_node->p_app_icon_full_name,view_app_node_len.icon_info_len);
        p_new_view_app_node->view_app_node.icon_info.full_path_wstr_len = view_app_node_len.icon_info_len;
        GUI_UTF8ToWstr(p_new_view_app_node->view_app_node.str1.wstr_ptr,view_app_node_len.str1_len,p_app_node->p_app_name,view_app_node_len.str1_len);
        p_new_view_app_node->view_app_node.str1.wstr_len = view_app_node_len.str1_len;
        //BUTTON1 img
        GUI_UTF8ToWstr(p_new_view_app_node->view_app_node.button_info1.image_info.full_path_wstr_ptr,view_app_node_len.button1_image_info_len,button1,view_app_node_len.button1_image_info_len);
        p_new_view_app_node->view_app_node.button_info1.image_info.full_path_wstr_len = view_app_node_len.button1_image_info_len;
        //BUTTON2 img
        GUI_UTF8ToWstr(p_new_view_app_node->view_app_node.button_info2.image_info.full_path_wstr_ptr,view_app_node_len.button2_image_info_len,button2,view_app_node_len.button2_image_info_len);
        p_new_view_app_node->view_app_node.button_info2.image_info.full_path_wstr_len = view_app_node_len.button2_image_info_len;
        GUI_UTF8ToWstr(p_new_view_app_node->view_app_node.str2.wstr_ptr,view_app_node_len.str2_len,p_app_node->p_app_introduction,view_app_node_len.str2_len);
        p_new_view_app_node->view_app_node.str2.wstr_len = view_app_node_len.str2_len;
        p_new_view_app_node->p_view_app_node_next = PNULL;

        APPSTORE_Data_AddViewAppNodeToViewAppList(view_app_node_len,p_new_view_app_node,&s_p_view_app_list);
        APPSTORE_Data_FreeViewAppNode(p_new_view_app_node);
        p_new_view_app_node = PNULL;
    }
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : Get the app num of the app list
//  Parameter: [In] none
//             [Out] none
//             [Return] app num
//  Author: mingwei.jia
//  Note: 获取APP的总数目
/*****************************************************************************/
PUBLIC uint32 APPSTORE_GetAppTotalNum(void)
{
    if(PNULL == s_p_view_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:all app list is null");
        return 0;
    }
    TRACE_APPSTORE("[APPSTORE_HOME_WIN_DATA]:s_p_view_app_list->app_number:%d",s_p_view_app_list->app_number);
    return s_p_view_app_list->app_number;
}
