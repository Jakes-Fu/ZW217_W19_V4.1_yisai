/*****************************************************************************
** File Name:      mmivp_locallist.h                                                   *
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
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#ifndef _MMIVP_LOCALLIST_H_
#define _MMIVP_LOCALLIST_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmivp_export.h"
#include "mmifilearray_export.h"
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
#define VP_LOCALLIST_FILENAME           L"movie_list.dat"  //本地播放列表存放默认文件名
#define VP_LOCALLIST_FILENAME_LEN       14                 //本地播放列表存放的默认文件名长度
#define VP_LOCALLIST1_FILENAME           L"movie_list1.dat"  //本地播放列表存放默认文件名
#define VP_LOCALLIST1_FILENAME_LEN       15                 //本地播放列表存放的默认文件名长度
#define VP_FILE_SUFFIX_MAX_LEN          7                   //文件后缀的最大长度

#define VP_LOCALLIST_MIN_SAPCE     (1024<<5)  //add by sandy for cr246623, the same as MMIMP3_LIST_MIN_MEM;

#define VP_LOCALLIST_UDISK_UNICODE   'D'  //add by sandy for cr246623
#define VP_LOCALLIST_SDCARD_UNICODE   'E'  //add by sandy for cr246623
#define VP_LOCALLIST_SDCARD1_UNICODE   'F'  //add by sandy for cr246623
#define VP_LOCALLIST_MAX_DEV_UNICODE   'G'  //add by sandy for cr246623

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
typedef enum
{
    VP_LOCALLIST_DEFAULT, //系统目录列表
    VP_LOCALLIST_USER, //用户自定义列表
    VP_LOCALLIST_MAX
}VP_LOCALLIST_TYPE_E;  //本地播放列表类型

//本地视频文件的支持情况以及相关信息
typedef struct 
{
    uint8   suffix[VP_FILE_SUFFIX_MAX_LEN]; //视频文件后缀
    uint32  type;                           //视频文件类型
    BOOLEAN is_support;                     //是否支持
}VP_SUFFIX_T;

//正在播放列表项信息
typedef struct  
{
    int32     index;        //本地播放列表索引
    BOOLEAN    is_ok;       //视频文件是否可以正常播放
}VP_PLAYINGLIST_ITEM_T;

//正在播放列表
typedef struct
{
    int32      start_index;                                 //起始播放视频在正在播放列表中的索引
    int32      active_index;                                //当前播放的视频在正在播放列表中的索引
    VP_PLAYINGLIST_ITEM_T     *playing_list_ptr;            //正在播放列表项信息
} VP_PLAYINGLIST_T;

typedef struct  
{
    VP_HANDLE           vp_handle;
    int32               locallist_index;   //某一个本地播放列表中的索引
    VP_LOCALLIST_TYPE_E locallist_type;  //本地播放列表类型
}VP_LOCALLIST_INFO_T; //本地播放列表信息

typedef enum
{
    VP_OP_LIST_SUCCESS,
    VP_OP_LIST_SAVE_ERROR,
    VP_OP_LIST_NO_SPACE,
    VP_OP_LIST_ERROR,
    VP_OP_LIST_MAX
}VP_OP_LIST_RESULT_E;

typedef void (*VP_OPLISTCALLBACK)( VP_OP_LIST_RESULT_E    result, VP_LOCALLIST_TYPE_E locallist_type);  //列表操作的回调函数
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 重新生成随机播放列表
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_ResetCurPlayList(BOOLEAN   is_random_play);

/*****************************************************************************/
//  Description : 根据模式获取本地播放列表中的上一条视频信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetPreByMode(wchar *full_path_ptr,
                                            uint16 *full_path_len_ptr,
                                            VP_REPEAT_PLAY_MODE_E mode,
                                            BOOLEAN   is_random_play
                                           );

/*****************************************************************************/
//  Description : 根据模式获取本地播放列表中的下一条视频信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetNextByMode(wchar *full_path_ptr,
                                            uint16 *full_path_len_ptr,
                                            VP_REPEAT_PLAY_MODE_E mode,
                                            BOOLEAN   is_random_play
                                           );
/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetIsCurPlayListItemOK(BOOLEAN   is_ok);

/*****************************************************************************/
//  Description : 销毁随机播放列表
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_DestroyPlayList(void);
/*****************************************************************************/
//  Description : 生成随机播放列表
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_CreateCurPlayList(BOOLEAN   is_random_play);

/*****************************************************************************/
//  Description : 开始异步加载本地播放列表。出厂首次打开播放器以及本地列表为空，自动搜索指定目录中的视频文件；
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note:  Sandy.Wan,2011-03-31， 增加一个file_dev_type 输入参数用于兼容以后平台多个存储卡的情况；
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_BeginLoad(
                                        MMI_WIN_ID_T        win_id,  
                                        MMI_MESSAGE_ID_E    msg_id,
                                        VP_LOCALLIST_TYPE_E  locallist_type,
                                        MMIFILE_DEVICE_E     file_dev_type);

/*****************************************************************************/
//  Description : 异步加载本地播放列表完成，保存本地播放列表。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_EndLoad(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 释放本地播放列表资源；
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_Release(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 获取本地播放列表中某一项的信息。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetInfo(
                                    int32               locallist_index,
                                    FILEARRAY_DATA_T    *f_array_data_ptr,
                                    VP_LOCALLIST_TYPE_E  locallist_type
                                    );

/*****************************************************************************/
//  Description : 获取本地播放列表的列表项的数目。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_LocalList_GetNum( VP_LOCALLIST_TYPE_E  locallist_type );

/*****************************************************************************/
//  Description : 从文件管理器中添加视频文件到播放列表中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_AddItem(VP_OPLISTCALLBACK callback,
                                 VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 更新当前本地播放列表。搜索u盘和sd卡上所有支持的视频文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_BeginUpdate(                                     
                                        MMI_WIN_ID_T        win_id,  
                                        MMI_MESSAGE_ID_E    msg_id,
                                        VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 异步更新本地播放列表完成，保存本地播放列表
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_EndUpdate(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 删除本地播放列表中的某一项
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_DelItemWithouSaveFile(int32        index,
                                                  VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 保存本地播放列表信息到默认文件中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_Locallist_SaveToFile(VP_LOCALLIST_TYPE_E  locallist_type);

/*****************************************************************************/
//  Description : 获取本地播放列表当前播放文件的索引。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC int32 VP_LocalList_GetCurrentIndex(VP_LOCALLIST_TYPE_E locallist_type);

/*****************************************************************************/
//  Description : 设置当前播放的视频文件位于本地播放列表中的索引
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetCurrentIndex(int32 index);

/*****************************************************************************/
//  Description : 获取当前本地播放列表类型。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LOCALLIST_TYPE_E VP_LocalList_GetCurListType(void);

/*****************************************************************************/
//  Description : 设置当前本地播放列表中的类型
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_LocalList_SetCurListType(VP_LOCALLIST_TYPE_E type);

/*****************************************************************************/
//  Description : 获取本地播放列表中的下一条视频信息。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetNext(wchar *full_path_ptr,
                                    uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : 获取本地播放列表的上一条视频信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_GetPrev(wchar *full_path_ptr,
                                    uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : 初始化随机播放的顺序列表
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_InitDifferentRandomList(void);

/*****************************************************************************/
//  Description : 检查是否有合适的存储器且存储器空间足够；
//  Global resource dependence : 
//  Author:Sandy.Wan
//  Note: add for cr246623
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E VP_LocalList_GetSuitableDeviceState(void);

/*****************************************************************************/
//  Description : 删除其他盘符中与输入的文件名同名同路径文件；
//  Global resource dependence : 
//  Author:Sandy.Wan
//  Note:  add for cr246623
/*****************************************************************************/
PUBLIC void VP_LocalList_DeleteSameNameFile( uint16     *file_name, int16     file_name_len);
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
/*****************************************************************************/
//  Description : 是否更新播放列表 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_LocalList_IsNeedReSearchLocallist( void );
/*****************************************************************************/
//  Description : 加载用户自定义播放列表 
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN VP_LocalList_LoadUserLocalList(void);
#endif


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif

#endif


