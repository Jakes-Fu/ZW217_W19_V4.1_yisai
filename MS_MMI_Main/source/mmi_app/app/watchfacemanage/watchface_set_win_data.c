/*****************************************************************************
** File Name:      watchface_set_win_data.c                                  *
** Author:         bin.wang1                                                 *
** Date:           08/12/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define watch face set data function  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       bin.wang1          Create                                    *
******************************************************************************/
#ifdef WATCHFACE_MANAGE_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "watchface_set_win_data.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/
typedef struct
{
    uint16 max_num;
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_item_ptr_array;  // list item data pointer array
}WATCHFACE_SET_DATA_WIN_DATA_T;

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/

LOCAL WATCHFACE_SET_DATA_WIN_DATA_T         s_watch_face_set_data_win_data = { 0 };

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

LOCAL void WatchFace_Set_Data_DestroyItemData( WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_item )
{
    if( PNULL == p_item )
    {
        TRACE_WATCHFACE(" p_item is PNULL");
        return;
    }

    if( PNULL != p_item->icon_info.full_path_wstr_ptr )
    {
        SCI_FREE( p_item->icon_info.full_path_wstr_ptr );
        p_item->icon_info.full_path_wstr_ptr = PNULL;
    }

    if( PNULL != p_item->str1.wstr_ptr )
    {
        SCI_FREE( p_item->str1.wstr_ptr );
        p_item->str1.wstr_ptr = PNULL;
    }

    if( PNULL != p_item->str2.wstr_ptr )
    {
        SCI_FREE( p_item->str2.wstr_ptr );
        p_item->str2.wstr_ptr = PNULL;
    }

    if( PNULL != p_item->button_info1.image_info.full_path_wstr_ptr )
    {
        SCI_FREE( p_item->button_info1.image_info.full_path_wstr_ptr );
        p_item->button_info1.image_info.full_path_wstr_ptr = PNULL;
    }

    if( PNULL != p_item->button_info1.str.wstr_ptr )
    {
        SCI_FREE( p_item->button_info1.str.wstr_ptr );
        p_item->button_info1.str.wstr_ptr = PNULL;
    }

    if( PNULL != p_item->button_info2.image_info.full_path_wstr_ptr )
    {
        SCI_FREE( p_item->button_info2.image_info.full_path_wstr_ptr );
        p_item->button_info2.image_info.full_path_wstr_ptr = PNULL;
    }

    if( PNULL != p_item->button_info2.str.wstr_ptr )
    {
        SCI_FREE( p_item->button_info2.str.wstr_ptr );
        p_item->button_info2.str.wstr_ptr = PNULL;
    }

    SCI_FREE( p_item );
}

LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* WatchFace_Set_Data_CreateItemData( WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_item_data )
{
    uint32  member_len = 0;
    wchar*  p_member = PNULL;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_item = PNULL;

    TRACE_WATCHFACE(" enter p_item_data 0x%x", (uint32)p_item_data);//bug id 1941703 CID 424470

    if( PNULL == p_item_data )
    {
        TRACE_WATCHFACE(" p_item_data is PNULL");
        return PNULL;
    }

    p_item = SCI_ALLOC_APPZ( sizeof(WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T) );
    if( PNULL == p_item )
    {
        TRACE_WATCHFACE(" p_item molloc memory fail");
        return PNULL;
    }

    //icon infor patch
    member_len = p_item_data->icon_info.full_path_wstr_len;
    p_member = (wchar*)SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" icon molloc memory fail");
        WatchFace_Set_Data_DestroyItemData( p_item );
        return PNULL;
    }
    MMIAPICOM_Wstrncpy( p_member, p_item_data->icon_info.full_path_wstr_ptr, member_len );
    p_item->icon_info.full_path_wstr_ptr = p_member;
    p_item->icon_info.full_path_wstr_len = member_len;

    //str1
    member_len = p_item_data->str1.wstr_len;
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" str1 molloc memory fail");
        WatchFace_Set_Data_DestroyItemData( p_item );
        return PNULL;
    }
    MMIAPICOM_Wstrncpy( p_member, p_item_data->str1.wstr_ptr, member_len );
    p_item->str1.wstr_ptr = p_member;
    p_item->str1.wstr_len = member_len;

    //str2
    member_len = p_item_data->str2.wstr_len;
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" str2 molloc memory fail");
        WatchFace_Set_Data_DestroyItemData( p_item );
        return PNULL;
    }
    MMIAPICOM_Wstrncpy( p_member, p_item_data->str2.wstr_ptr, member_len );
    p_item->str2.wstr_ptr = p_member;
    p_item->str2.wstr_len = member_len;

    //button1
    member_len = p_item_data->button_info1.image_info.full_path_wstr_len;
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" button2 molloc memory fail");
        WatchFace_Set_Data_DestroyItemData( p_item );
        return PNULL;
    }
    MMIAPICOM_Wstrncpy( p_member, p_item_data->button_info1.image_info.full_path_wstr_ptr, member_len );
    p_item->button_info1.str.wstr_ptr = PNULL;
    p_item->button_info1.str.wstr_len = 0;
    p_item->button_info1.image_info.full_path_wstr_ptr = p_member;
    p_item->button_info1.image_info.full_path_wstr_len = member_len;


    //button2
    member_len = p_item_data->button_info2.image_info.full_path_wstr_len;
    p_member = SCI_ALLOC_APPZ( (member_len+1)*sizeof(wchar) );
    if( PNULL == p_member )
    {
        TRACE_WATCHFACE(" button2 molloc memory fail");
        WatchFace_Set_Data_DestroyItemData( p_item );
        return PNULL;
    }
    MMIAPICOM_Wstrncpy( p_member, p_item_data->button_info2.image_info.full_path_wstr_ptr, member_len );
    p_item->button_info2.str.wstr_ptr = PNULL;
    p_item->button_info2.str.wstr_len = 0;
    p_item->button_info2.image_info.full_path_wstr_ptr = p_member;
    p_item->button_info2.image_info.full_path_wstr_len = member_len;


    return p_item;
}

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 销毁表盘管理窗体界面的数据
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_Set_Data_DestroyWinData( void )
{
    uint16 index = 0;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_item_ptr_array = PNULL;

    TRACE_WATCHFACE(" enter max num %d", s_watch_face_set_data_win_data.max_num);

    p_item_ptr_array = s_watch_face_set_data_win_data.p_item_ptr_array;

    if( PNULL == p_item_ptr_array )
    {
        TRACE_WATCHFACE(" p_item_ptr_array is PNULL");
        return;
    }

    for( index = 0; index < s_watch_face_set_data_win_data.max_num; index++ )
    {
        if( PNULL != p_item_ptr_array[index] )
        {
            WatchFace_Set_Data_DestroyItemData( p_item_ptr_array[index] );
        }
    }

    SCI_FREE( s_watch_face_set_data_win_data.p_item_ptr_array );

    s_watch_face_set_data_win_data.p_item_ptr_array = PNULL;
    s_watch_face_set_data_win_data.max_num = 0;
}

/*****************************************************************************/
//  Description : 创建表盘管理窗体界面的数据
//  Parameter: [In] num 表盘数据总数
//             [In] 列表数据指针数组
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Data_CreateWinData( uint16 num, WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_item_ptr_array )
{
    uint16 index = 0;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T* p_item = PNULL;
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_array = PNULL;

    TRACE_WATCHFACE(" enter total num %d p_item_ptr_array 0x%x", num, (uint32)p_item_ptr_array);//bug id 1941703 CID 424981

    if( PNULL == p_item_ptr_array )
    {
        TRACE_WATCHFACE(" p_data_list is PNULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    WATCHFACE_Set_Data_DestroyWinData();

    p_array = (WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T**)SCI_ALLOC_APPZ( ((num+1)*sizeof(WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T*)) );
    if( PNULL == p_array )
    {
        TRACE_WATCHFACE(" p_array is PNULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    s_watch_face_set_data_win_data.p_item_ptr_array = p_array;

    // list data
    for( index = 0; index < num; index++ )
    {
        p_item = WatchFace_Set_Data_CreateItemData( p_item_ptr_array[index] );

        if( PNULL == p_item )
        {
            TRACE_WATCHFACE(" create node fail index %d", index);

            WATCHFACE_Set_Data_DestroyWinData();
            return WATCHFACE_STATUS_RES_FAIL;
        }

        p_array[index] = p_item;
        s_watch_face_set_data_win_data.max_num++;
    }

    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : 获取表盘列表中表盘的总数
//  Parameter: [In] None
//             [Out] None
//             [Return] 表盘总数
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC uint16 WATCHFACE_Set_Data_GetListItemNum( void )
{
    TRACE_WATCHFACE(" max num %d", s_watch_face_set_data_win_data.max_num);

    return s_watch_face_set_data_win_data.max_num;
}

/*****************************************************************************/
//  Description : 删除表盘列表中的一个表盘
//  Parameter: [In] 表盘索引
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Data_DelListItem( uint16 index )
{
    uint16  max_num = 0;
    uint16  dest_index = 0;
    uint16  source_index = 0;

    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T*     p_item = PNULL;
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T**    p_item_ptr_array = PNULL;

    TRACE_WATCHFACE(" enter index %d", index);

    if( index >= s_watch_face_set_data_win_data.max_num )
    {
        TRACE_WATCHFACE(" index error %d max num %d", index, s_watch_face_set_data_win_data.max_num);
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    if( PNULL == s_watch_face_set_data_win_data.p_item_ptr_array )
    {
        TRACE_WATCHFACE(" p_list is PNULL");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    p_item_ptr_array = (WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T**)SCI_ALLOC_APPZ( (s_watch_face_set_data_win_data.max_num*sizeof(WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T*)) );
    if( PNULL == p_item_ptr_array )
    {
        TRACE_WATCHFACE(" p_item_ptr_array is PNULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    for( source_index = 0; source_index <= s_watch_face_set_data_win_data.max_num; source_index++ )
    {
        if( source_index == index )
        {
            WatchFace_Set_Data_DestroyItemData( s_watch_face_set_data_win_data.p_item_ptr_array[source_index] );
            s_watch_face_set_data_win_data.p_item_ptr_array[source_index] = PNULL;
        }
        else
        {
            p_item_ptr_array[dest_index] = s_watch_face_set_data_win_data.p_item_ptr_array[source_index];
            dest_index++;
        }
    }

    SCI_FREE( s_watch_face_set_data_win_data.p_item_ptr_array );

    s_watch_face_set_data_win_data.p_item_ptr_array = p_item_ptr_array;
    s_watch_face_set_data_win_data.max_num--;

    return TRUE;
}

/*****************************************************************************/
//  Description : 获取表盘列表中的表盘数据
//  Parameter: [In] index 表盘索引
//             [Out] 表盘数据地址
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Data_GetListItemData( uint16 index, WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_list_item_data )
{
    WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_array = PNULL;

    TRACE_WATCHFACE(" enter index %d", index);

    if( index >= s_watch_face_set_data_win_data.max_num )
    {
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    if( PNULL == s_watch_face_set_data_win_data.p_item_ptr_array )
    {
        return WATCHFACE_STATUS_RES_FAIL;
    }

    *p_list_item_data = s_watch_face_set_data_win_data.p_item_ptr_array[index];

    TRACE_WATCHFACE(" enter data ptr 0x%x", (uint32)(*p_list_item_data));//bug id 1941703 CID 422873
    return WATCHFACE_STATUS_RES_SUCCESS;
}
#endif
