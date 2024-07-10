/*****************************************************************************
** File Name:      mmivp_historylist.h                                                   *
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

#ifndef _MMIVP_HISTORYLIST_H_
#define _MMIVP_HISTORYLIST_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
#include "mmifmm_export.h"
#include "mmivp_export.h"
#include "mmivp_internal.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VP_MAX_HISTORY                  30                         //最大历史记录个数
#define VP_HISTORYLIST_FILENAME         L"movie_historylist.dat"   //保存历史记录的默认文件名
#define VP_HISTORYLIST_FILENAME_LEN     21                         //保存历史记录的默认文件名长度
#define VP_HISTORYLIST_VER             "VPHISTORY.10.00.01"        //默认文件版本号
#define VP_HISTORYLIST_VER_LEN          18                         //默认文件的版本号长度

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   

//历史记录列表文件的头信息
typedef struct
{
    uint8                      version[VP_HISTORYLIST_VER_LEN+1]; //历史记录文件版本号    
}VP_HISTORYLIST_HEAD_T;

//历史记录列表信息
typedef struct
{
    uint32                  item_count;         //历史记录的数目
    VP_HISTORY_ITEM_T        *item_info_ptr;    //历史记录项信息
}VP_HISTORY_CONTEXT_T;
/*****************************************************************************/
//  Description : 初始化历史记录列表，加载历史记录信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_Load( void );

/*****************************************************************************/
//  Description : 释放历史记录资源
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_HistoryList_Release( void );

/*****************************************************************************/
//  Description : 释放历史记录资源
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_HistoryList_GetNum( void );

/*****************************************************************************/
//  Description : 获取某一历史记录的信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC const VP_HISTORY_ITEM_T*     VP_HistoryList_GetInfo(uint32    historylist_index);

/*****************************************************************************/
//  Description : 删除某一历史播放记录
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_DelItem( uint32 index );

/*****************************************************************************/
//  Description : 删除全部历史记录
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_DelAllItem(void );

/*****************************************************************************/
//  Description : 添加历史记录
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_AddItem(VP_HISTORY_ITEM_T  history_list_item);

/*****************************************************************************/
//  Description : 为正在播放的历史记录设置最后一次停止播放的位置
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_SetLastPlayPoint(uint32        last_play_point );

/*****************************************************************************/
//  Description : 修改历史记录
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_ModifyItem(                                         
                                         uint32 last_play_point,
                                         uint32 total_time,
                                         uint32 video_height,
                                         uint32 video_width,
                                         uint32 media_length,
                                         uint16 title_len,
                                         wchar  *title_ptr);


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
#endif


