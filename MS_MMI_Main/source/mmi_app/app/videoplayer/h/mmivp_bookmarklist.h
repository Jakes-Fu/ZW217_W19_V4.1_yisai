/**************************************************************************
** File Name:      mmivp_bookmarklist.h                                                   *
** Author:                                                                   *
** Date:           2010/3/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/18      apple.zhang       Create
******************************************************************************/

#ifndef _MMIVP_BOOKMARKLIST_H_
#define _MMIVP_BOOKMARKLIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmivp_export.h"
#include "mmivp_internal.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VP_BOOKMARKLIST_FILENAME         L"movie_bookmarklist.dat"  //存放书签的默认文件名
#define VP_BOOKMARKLIST_FILENAME_LEN     22                         //存放书签的默认文件名长度
#define VP_BOOKMARKLIST_VER             "VPBOOKMARK.10.00.10"       //书签默认文件的版本号
#define VP_BOOKMARKLIST_VER_LEN          19                         //书签默认文件版本号长度

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/  

//书签文件的头信息
typedef struct
{
    uint8                      version[VP_BOOKMARKLIST_VER_LEN+1]; //书签文件版本号    
}VP_BOOKMARKLIST_HEAD_T;

//收藏夹信息
typedef struct
{
    uint32                 item_count;                       //书签的数目
    VP_NET_MOVIE_INFO_T    item_info[VP_BOOKMARK_MAX_ITEM];                       //书签信息
} VP_BOOKMARK_CONTEXT_T;
/*****************************************************************************/
//  Description : 初始化历史记录列表，加载历史记录信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_Load( void );

/*****************************************************************************/
//  Description : 释放书签列表资源
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_BookmarkList_Release( void );

/*****************************************************************************/
//  Description : 获取书签总数
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_BookmarkList_GetNum( void );

/*****************************************************************************/
//  Description :获取某一书签的信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC const VP_NET_MOVIE_INFO_T * VP_BookmarkList_GetInfo(uint32  bookmark_list_index);

/*****************************************************************************/
//  Description :删除某一书签记录
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_DelItem( uint32 index );

/*****************************************************************************/
//  Description :删除全部书签
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_DelAllItem(void );

/*****************************************************************************/
//  Description :添加书签
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LIST_ERROR_E VP_BookmarkList_AddItem(VP_NET_MOVIE_INFO_T     bookmark_data);

/*****************************************************************************/
//  Description :修改书签
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LIST_ERROR_E VP_BookmarkList_ModifyItem( 
                                                uint16               bookmark_list_index,
                                                VP_NET_MOVIE_INFO_T     bookmark_data
                                                );
/*****************************************************************************/
//  Description :获取预置CMCC流媒体门户网址信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_NET_MOVIE_INFO_T VP_GetCMCCAddressInfo(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif



