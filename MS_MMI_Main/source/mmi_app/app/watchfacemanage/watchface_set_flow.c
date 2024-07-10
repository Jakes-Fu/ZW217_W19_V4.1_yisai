/*****************************************************************************
** File Name:      watchface_flow.c                                          *
** Author:         bin.wang1                                                 *
** Date:           08/17/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define watch face flow function      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       bin.wang1            Create                                  *
******************************************************************************/
#ifdef WATCHFACE_MANAGE_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "watchface_config.h"
#include "watchface_set_flow.h"
#include "watchface_set_win_view.h"
#include "watchface_delete_query_win_view.h"
#include "watchface_set_note_win_view.h"
#include "watchface_set_operate_fail_note_win_view.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#ifdef PLATFORM_ANTISW3
#define WATCHFACE_CUR_ICON_FULL_PATH        "C:\\ur_root\\watchface\\res\\current.png"
#define WATCHFACE_SET_CUR_ICON_FULL_PATH    "C:\\ur_root\\watchface\\res\\set_current.png"
#define WATCHFACE_DEL_ICON_FULL_PATH        "C:\\ur_root\\watchface\\res\\delete.png"
#elif defined PLATFORM_UWS6121E
//217不支持C盘，故将表盘文件放到D盘
#define WATCHFACE_CUR_ICON_FULL_PATH        "D:\\ur_root\\watchface\\res\\current.png"
#define WATCHFACE_SET_CUR_ICON_FULL_PATH    "D:\\ur_root\\watchface\\res\\set_current.png"
#define WATCHFACE_DEL_ICON_FULL_PATH        "D:\\ur_root\\watchface\\res\\delete.png"
#endif
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/
typedef struct
{
    uint16      set_win_selected_item_index; //item index user selected
}WATCHFACE_SET_FLOW_DATA_T;
/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL WATCHFACE_SET_FLOW_DATA_T s_watch_face_set_flow_data = { 0 };

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL void WatchFace_Set_Flow_DestroyItemData( WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_item_data )
{
    if( PNULL == p_item_data )
    {
        TRACE_WATCHFACE(" p_item_data is PNULL");
        return;
    }

    if( PNULL != p_item_data->icon_info.full_path_wstr_ptr )
    {
        SCI_FREE( p_item_data->icon_info.full_path_wstr_ptr );
        p_item_data->icon_info.full_path_wstr_ptr = PNULL;
    }

    if( PNULL != p_item_data->str1.wstr_ptr )
    {
        SCI_FREE( p_item_data->str1.wstr_ptr );
        p_item_data->str1.wstr_ptr = PNULL;
    }

    if( PNULL != p_item_data->str2.wstr_ptr )
    {
        SCI_FREE( p_item_data->str2.wstr_ptr );
        p_item_data->str2.wstr_ptr = PNULL;
    }

    if( PNULL != p_item_data->button_info1.image_info.full_path_wstr_ptr )
    {
        SCI_FREE( p_item_data->button_info1.image_info.full_path_wstr_ptr );
        p_item_data->button_info1.image_info.full_path_wstr_ptr = PNULL;
    }

    if( PNULL != p_item_data->button_info1.str.wstr_ptr )
    {
        SCI_FREE( p_item_data->button_info1.str.wstr_ptr );
        p_item_data->button_info1.str.wstr_ptr = PNULL;
    }

    if( PNULL != p_item_data->button_info2.image_info.full_path_wstr_ptr )
    {
        SCI_FREE( p_item_data->button_info2.image_info.full_path_wstr_ptr );
        p_item_data->button_info2.image_info.full_path_wstr_ptr = PNULL;
    }

    if( PNULL != p_item_data->button_info2.str.wstr_ptr )
    {
        SCI_FREE( p_item_data->button_info2.str.wstr_ptr );
        p_item_data->button_info2.str.wstr_ptr = PNULL;
    }

    SCI_FREE( p_item_data );
}

LOCAL void WatchFace_Set_Flow_DestroyAllItem( uint16 total_num, WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_item_ptr_array )
{
    uint16 index = 0;

    TRACE_WATCHFACE(" enter total_num 0x%x p_item_ptr_array 0x%x", total_num, (uint32)p_item_ptr_array);//bug id 1941703 CID 420737

    if( PNULL == p_item_ptr_array )
    {
        TRACE_WATCHFACE(" p_item_ptr_array is PNULL");
        return;
    }

    for( index = 0; index < total_num; index++ )
    {
        if( PNULL != p_item_ptr_array[index] )
        {
            WatchFace_Set_Flow_DestroyItemData( p_item_ptr_array[index] );
        }
        p_item_ptr_array[index] = PNULL;
    }

    SCI_FREE( p_item_ptr_array );
}

LOCAL WATCHFACE_STATUS_RES_E WatchFace_Set_Flow_CreateItemData( BOOLEAN is_cur_watchface, WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_item_data, COMMON_CONFIG_OBJECT_T* p_cfg_data )
{
    uint32                          member_len = 0;
    wchar*                          p_member = PNULL;
    char*                           p_icon_full_path = PNULL;
    char                            icon_full_path[MAX_PATH_LEN+1] = { 0 };

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_item = PNULL;

    TRACE_WATCHFACE(" enter p_item_data 0x%x p_cfg_data 0x%x", (uint32)p_item_data, (uint32)p_cfg_data);//bug id  1941703 CID 424284 425296

    if( PNULL == p_item_data || PNULL == p_cfg_data )
    {
        TRACE_WATCHFACE(" p_list_node or p_cfg_data is PNULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    p_item = SCI_ALLOC_APPZ( sizeof(WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T) );
    if( PNULL == p_item )
    {
        TRACE_WATCHFACE(" p_item molloc memory fail");
        return WATCHFACE_STATUS_RES_FAIL;
    }

    //icon infor patch
    //directory + "\" + iocn name
    member_len = SCI_STRLEN( p_cfg_data->url ) + 1 + SCI_STRLEN( p_cfg_data->icon );
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" icon molloc memory fail");
        WatchFace_Set_Flow_DestroyItemData( p_item );
        return WATCHFACE_STATUS_RES_FAIL;
    }

    sprintf( icon_full_path, "%s\\%s", p_cfg_data->url, p_cfg_data->icon );
    GUI_UTF8ToWstr( p_member, member_len, icon_full_path, member_len);
    p_item->icon_info.full_path_wstr_ptr = p_member;
    p_item->icon_info.full_path_wstr_len = member_len;

    //str1
    member_len = SCI_STRLEN( p_cfg_data->app_name );
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" str1 molloc memory fail");
        WatchFace_Set_Flow_DestroyItemData( p_item );
        return WATCHFACE_STATUS_RES_FAIL;
    }
    GUI_UTF8ToWstr( p_member, member_len, p_cfg_data->app_name, member_len);
    p_item->str1.wstr_ptr = p_member;
    p_item->str1.wstr_len = member_len;

    //str2
    member_len = SCI_STRLEN( p_cfg_data->introduction );
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" str2 molloc memory fail");
        WatchFace_Set_Flow_DestroyItemData( p_item );
        return WATCHFACE_STATUS_RES_FAIL;
    }
    GUI_UTF8ToWstr( p_member, member_len, p_cfg_data->introduction, member_len);
    p_item->str2.wstr_ptr = p_member;
    p_item->str2.wstr_len = member_len;

    //button1
    if( is_cur_watchface )
    {
        p_icon_full_path = PNULL;

        p_item->button_info1.image_info.full_path_wstr_ptr = PNULL;
        p_item->button_info1.image_info.full_path_wstr_len = 0;
    }
    else
    {
        p_icon_full_path = WATCHFACE_SET_CUR_ICON_FULL_PATH;

        member_len = SCI_STRLEN( p_icon_full_path );
        p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
        if( PNULL == p_member )
        {
            TRACE_WATCHFACE(" button2 molloc memory fail");
            WatchFace_Set_Flow_DestroyItemData( p_item );
            return WATCHFACE_STATUS_RES_FAIL;
        }
        GUI_UTF8ToWstr( p_member, member_len, p_icon_full_path, member_len);

        p_item->button_info1.image_info.full_path_wstr_ptr = p_member;
        p_item->button_info1.image_info.full_path_wstr_len = member_len;
    }

    p_item->button_info1.str.wstr_ptr = PNULL;
    p_item->button_info1.str.wstr_len = 0;



    //button2
    if( is_cur_watchface )
    {
        p_icon_full_path = WATCHFACE_CUR_ICON_FULL_PATH;
    }
    else
    {
        p_icon_full_path = WATCHFACE_DEL_ICON_FULL_PATH;
    }
    member_len = SCI_STRLEN( p_icon_full_path );
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" button2 molloc memory fail");
        WatchFace_Set_Flow_DestroyItemData( p_item );
        return WATCHFACE_STATUS_RES_FAIL;
    }
    GUI_UTF8ToWstr( p_member, member_len, p_icon_full_path, member_len);
    p_item->button_info2.str.wstr_ptr = PNULL;
    p_item->button_info2.str.wstr_len = 0;
    p_item->button_info2.image_info.full_path_wstr_ptr = p_member;
    p_item->button_info2.image_info.full_path_wstr_len = member_len;

    *p_item_data = p_item;

    return WATCHFACE_STATUS_RES_SUCCESS;
}

LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** WatchFace_Set_Flow_CreateAllItem( uint16* p_total_num )
{
    uint16                      index = 0;
    uint16                      cur_index = 0;
    uint16                      total_num = 0;
    WATCHFACE_STATUS_RES_E      res = WATCHFACE_STATUS_RES_SUCCESS;
    COMMON_CONFIG_OBJECT_T*     p_cfg_data = PNULL;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T**    p_item_ptr_array = PNULL;

    TRACE_WATCHFACE(" enter ");

    if( PNULL == p_total_num )
    {
        TRACE_WATCHFACE(" p_total_num is PNULL ");
        return PNULL;
    }

    res = WATCHFACE_Get_CurWatchface_Index( &cur_index );
    if( WATCHFACE_STATUS_RES_SUCCESS != res)
    {
        return PNULL;
    }
    total_num = WATCHFACE_Get_Watchface_Num();

    p_item_ptr_array = SCI_ALLOC_APPZ( total_num*sizeof(WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T*) );
    if( PNULL == p_item_ptr_array )
    {
        TRACE_WATCHFACE(" p_item_ptr_array molloc memory fail");
        return PNULL;
    }
    p_cfg_data = SCI_ALLOC_APPZ( sizeof(COMMON_CONFIG_OBJECT_T) );
    if( PNULL == p_cfg_data )
    {
        SCI_FREE( p_item_ptr_array );

        TRACE_WATCHFACE(" p_cfg_data molloc memory fail");
        return PNULL;
    }

    for( index  = 0; index < total_num; index++ )
    {
        SCI_MEMSET( p_cfg_data, 0x0, sizeof(COMMON_CONFIG_OBJECT_T) );

        res = WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED, index, p_cfg_data );
        if( WATCHFACE_STATUS_RES_SUCCESS != res )
        {
            TRACE_WATCHFACE(" call WATCHFACE_Get_WatchfaceCfg_Info fail");
            break;
        }

        if( cur_index == index )
        {
            res = WatchFace_Set_Flow_CreateItemData( TRUE, &(p_item_ptr_array[index]), p_cfg_data );
        }
        else
        {
            res = WatchFace_Set_Flow_CreateItemData( FALSE, &(p_item_ptr_array[index]), p_cfg_data);
        }

        if( WATCHFACE_STATUS_RES_SUCCESS != res )
        {
            TRACE_WATCHFACE(" create node fail index %d", index);
            break;
        }
    }

    SCI_FREE( p_cfg_data );

    if( WATCHFACE_STATUS_RES_SUCCESS != res )
    {
        WatchFace_Set_Flow_DestroyAllItem( total_num, p_item_ptr_array );
        *p_total_num = 0;

        return PNULL;
    }
    else
    {
        *p_total_num = total_num;

        return p_item_ptr_array;
    }
}

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 进入表盘管理
//  Parameter:
//             [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Enter( void )
{
    uint16                      total_num = 0;
    WATCHFACE_STATUS_RES_E      res = WATCHFACE_STATUS_RES_SUCCESS;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T**    p_item_ptr_array = PNULL;

    TRACE_WATCHFACE(" enter ");

    p_item_ptr_array = WatchFace_Set_Flow_CreateAllItem( &total_num );
    if( PNULL == p_item_ptr_array )
    {
        return WATCHFACE_STATUS_RES_FAIL;
    }

    res = WATCHFACE_Set_Data_CreateWinData( total_num, p_item_ptr_array );
    WatchFace_Set_Flow_DestroyAllItem( total_num, p_item_ptr_array );

    WATCHFACE_Set_Win_Enter();

    return res;
}


/*****************************************************************************/
//  Description : 处理左侧button（设置为当前表盘）的函数
//  Parameter:
//             [In] list_item_index : 点击的buttion所在的item在list中的index
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleListItemButton1( uint16 list_item_index )
{
    uint32                  member_len = 0;
    wchar*                  p_pre_button1_path = PNULL;
    wchar*                  p_pre_button2_path = PNULL;
    wchar*                  p_cur_button2_path = PNULL;
    uint16                  pre_index = 0;
    WATCHFACE_STATUS_RES_E  res = WATCHFACE_STATUS_RES_SUCCESS;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T*    p_pre_item_data = PNULL;
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T*    p_item_data = PNULL;

    TRACE_WATCHFACE(" enter ");

    res = WATCHFACE_Get_CurWatchface_Index( &pre_index );
    if( WATCHFACE_STATUS_RES_SUCCESS != res )
    {
        WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
        return res;
    }

    if( pre_index == list_item_index )
    {
        // do nothing
        TRACE_WATCHFACE(" same index no need change ");
        return WATCHFACE_STATUS_RES_SUCCESS;
    }

    res = WATCHFACE_Set_CurWatchface_Index( list_item_index );
    if( WATCHFACE_STATUS_RES_SUCCESS == res )
    {
        res = WATCHFACE_Set_Data_GetListItemData( pre_index, &p_pre_item_data );
        if( WATCHFACE_STATUS_RES_SUCCESS != res )
        {
            WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
            return res;
        }

        res = WATCHFACE_Set_Data_GetListItemData( list_item_index, &p_item_data );
        if( WATCHFACE_STATUS_RES_SUCCESS != res )
        {
            WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
            return res;
        }

        //molloc buttion path string buffer
        member_len = SCI_STRLEN( WATCHFACE_SET_CUR_ICON_FULL_PATH );
        p_pre_button1_path = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );

        member_len = SCI_STRLEN( WATCHFACE_DEL_ICON_FULL_PATH );
        p_pre_button2_path = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );

        member_len = SCI_STRLEN( WATCHFACE_CUR_ICON_FULL_PATH );
        p_cur_button2_path = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );

        if( (PNULL == p_pre_button1_path) || (PNULL == p_pre_button2_path) || (PNULL == p_cur_button2_path) )
        {
            if( PNULL != p_pre_button1_path )
            {
                SCI_FREE( p_pre_button1_path );
            }
            if( PNULL != p_pre_button2_path )
            {
                SCI_FREE( p_pre_button2_path );
            }
            if( PNULL != p_cur_button2_path )
            {
                SCI_FREE( p_cur_button2_path );
            }

            WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
            return WATCHFACE_STATUS_RES_FAIL;
        }

        //update pre item
        //buttion1
        MMIAPICOM_StrToWstr( WATCHFACE_SET_CUR_ICON_FULL_PATH, p_pre_button1_path );
        if( PNULL != p_pre_item_data->button_info1.image_info.full_path_wstr_ptr )
        {
            SCI_FREE( p_pre_item_data->button_info1.image_info.full_path_wstr_ptr );
        }
        p_pre_item_data->button_info1.str.wstr_ptr = PNULL;
        p_pre_item_data->button_info1.str.wstr_len = 0;
        p_pre_item_data->button_info1.image_info.full_path_wstr_ptr = p_pre_button1_path;
        p_pre_item_data->button_info1.image_info.full_path_wstr_len = MMIAPICOM_Wstrlen( p_pre_button1_path );

        //button2
        MMIAPICOM_StrToWstr( WATCHFACE_DEL_ICON_FULL_PATH, p_pre_button2_path );
        if( PNULL != p_pre_item_data->button_info2.image_info.full_path_wstr_ptr )
        {
            SCI_FREE( p_pre_item_data->button_info2.image_info.full_path_wstr_ptr );
        }
        p_pre_item_data->button_info2.str.wstr_ptr = PNULL;
        p_pre_item_data->button_info2.str.wstr_len = 0;
        p_pre_item_data->button_info2.image_info.full_path_wstr_ptr = p_pre_button2_path;
        p_pre_item_data->button_info2.image_info.full_path_wstr_len = MMIAPICOM_Wstrlen( p_pre_button2_path );

        //update cur item
        //button1
        if( PNULL != p_item_data->button_info1.image_info.full_path_wstr_ptr )
        {
            SCI_FREE( p_item_data->button_info1.image_info.full_path_wstr_ptr );
        }
        p_item_data->button_info1.str.wstr_ptr = PNULL;
        p_item_data->button_info1.str.wstr_len = 0;
        p_item_data->button_info1.image_info.full_path_wstr_ptr = PNULL;
        p_item_data->button_info1.image_info.full_path_wstr_len = 0;

        //button2
        MMIAPICOM_StrToWstr( WATCHFACE_CUR_ICON_FULL_PATH, p_cur_button2_path );
        if( PNULL != p_item_data->button_info2.image_info.full_path_wstr_ptr )
        {
            SCI_FREE( p_item_data->button_info2.image_info.full_path_wstr_ptr );
        }
        p_item_data->button_info2.str.wstr_ptr = PNULL;
        p_item_data->button_info2.str.wstr_len = 0;
        p_item_data->button_info2.image_info.full_path_wstr_ptr = p_cur_button2_path;
        p_item_data->button_info2.image_info.full_path_wstr_len = MMIAPICOM_Wstrlen( p_cur_button2_path );
    }
    else
    {
        WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
        return WATCHFACE_STATUS_RES_FAIL;
    }
    WATCHFACE_Set_Note_Win_Enter();

    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : 处理右侧button（删除表盘）的函数
//  Parameter:
//             [In] list_item_index : 点击的buttion所在的item在list中的index
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleListItemButton2( uint16 list_item_index )
{
    uint16                  cur_index = 0;
    WATCHFACE_STATUS_RES_E  res = WATCHFACE_STATUS_RES_SUCCESS;

    TRACE_WATCHFACE(" enter ");

    res = WATCHFACE_Get_CurWatchface_Index( &cur_index );
    if( WATCHFACE_STATUS_RES_SUCCESS != res )
    {
        WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
        return res;
    }

    if( cur_index == list_item_index )
    {
        TRACE_WATCHFACE(" current watch face don't delete ");
        return res;
    }
    if(list_item_index < DEFAULT_WATCHFACE_NUM)
    {
        TRACE_WATCHFACE(" current watch face don't delete ");
        WATCHFACE_Delete_Note_Win_Enter();
        return res;
    }
    s_watch_face_set_flow_data.set_win_selected_item_index = list_item_index;

    WATCHFACE_Delete_Query_Win_Enter();

    return res;
}

/*****************************************************************************/
//  Description : 为Watch face delete QueryWin提供的处理左侧button（处理取消）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleDeleteQueryWinButton1( void )
{
    TRACE_WATCHFACE(" enter ");

    //do nothing

    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : 为Watch face delete QueryWin提供的处理右侧button（处理取消）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleDeleteQueryWinButton2( void )
{
    WATCHFACE_STATUS_RES_E res = WATCHFACE_STATUS_RES_SUCCESS;

    TRACE_WATCHFACE(" enter ");

    res = WATCHFACE_DeleteWatchface( WATCHFACE_CONFIG_TYPE_INSTALLED , s_watch_face_set_flow_data.set_win_selected_item_index );
    if( WATCHFACE_STATUS_RES_SUCCESS != res )
    {
        WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
    }

    return res;
}

/*****************************************************************************/
//  Description : 删除表盘后更新界面的函数
//  Parameter:
//             [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_DeleteResulte( BOOLEAN is_success )
{
    uint16                      total_num = 0;
    WATCHFACE_STATUS_RES_E      res = WATCHFACE_STATUS_RES_SUCCESS;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T**    p_item_ptr_array = PNULL;

    TRACE_WATCHFACE(" enter is_success %d", is_success);

    if( TRUE == is_success )
    {
        if( TRUE == MMK_IsOpenWin(WATCHFACE_SET_WIN_ID) )
        {
            p_item_ptr_array = WatchFace_Set_Flow_CreateAllItem( &total_num );
            if( PNULL == p_item_ptr_array )
            {
                WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
                return WATCHFACE_STATUS_RES_FAIL;
            }

            WATCHFACE_Set_Data_DestroyWinData();

            res = WATCHFACE_Set_Data_CreateWinData( total_num, p_item_ptr_array );

            WatchFace_Set_Flow_DestroyAllItem( total_num, p_item_ptr_array );

            if( TRUE == MMK_IsFocusWin(WATCHFACE_SET_WIN_ID) )
            {
                MMK_PostMsg( WATCHFACE_SET_WIN_ID, MSG_FULL_PAINT, PNULL, 0 );
            }

            if( TRUE == MMK_IsOpenWin(WATCHFACE_DELETE_QUERY_WIN_ID) )
            {
                MMK_CloseWin( WATCHFACE_DELETE_QUERY_WIN_ID );
            }
        }
    }
    else
    {
        WATCHFACE_Set_Operate_Fail_Note_Win_Enter();
    }

    return res;
}
#endif

