/*****************************************************************************
** Copyright 2021-2022 Unisoc(Shanghai) Technologies Co.,Ltd                 *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*****************************************************************************
** File Name:       ual_audioplayer.h                                        *
** Author:            qi.liu1                                                *
** Date:            10/21/2021                                               *
** Description:    This file is used to define ual audioplayer export        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 10/21/2021          qi.liu1                  Create                       *
******************************************************************************/
#ifndef _UAL_AUDIOPLAYER_H_
#define _UAL_AUDIOPLAYER_H_
#include "ual_type.h"
#include "ual_cms.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
typedef  uint32 ual_audioplayer_handle;

#define  AUDIOPLAYER_INVALID_HANDLE 0xFFFFFFFF

#define UAL_AUDIOPLAYER_YEAR_LEN    10   //年份 size
#define UAL_AUDIOPLAYER_ARTIST_LEN  32   //作曲家 size
#define UAL_AUDIOPLAYER_ALBUM_LEN   32   //专辑名 size
#define UAL_AUDIOPLAYER_TITLE_LEN   32   //歌曲名 size
#define UAL_AUDIOPLAYER_TRACK_LEN   32   //音轨名 size
#define UAL_AUDIOPLAYER_GENRE_LEN   32   //流派名 size

//ual audioplayer 给app上报的消息
typedef enum
{
    UAL_AUDIOPLAYER_MSG_AUDIO_SUSPEND = UAL_MODULE_AUDIOPLAYER << 16,  //AUDIO服务被挂起通知   //bug2058023
    UAL_AUDIOPLAYER_MSG_AUDIO_RESUME,  //AUDIO服务恢复通知
    UAL_AUDIOPLAYER_MSG_AUDIO_ROUTE_CHANGED, //音频播放通道变化通知
    //数据流接收消息
    UAL_AUDIOPLAYER_MSG_AUDIO_DATA_IND, //数据流信息第二阶段
    UAL_AUDIOPLAYER_MSG_DOWNLOAD_PROGRESS,  //进度信息
    //播放
    UAL_AUDIOPLAYER_MSG_PLAY_IND, //AUDIO PLAYER 内部有play动作
    UAL_AUDIOPLAYER_MSG_PLAY_END, //播放结束
    UAL_AUDIOPLAYER_MSG_PLAY_PROGRESS,  //播放进度
    //暂停
    UAL_AUDIOPLAYER_MSG_PAUSE_CNF,
    UAL_AUDIOPLAYER_MSG_PAUSE_IND,
    //缓冲
    UAL_AUDIOPLAYER_MSG_BUFFERING_IND,
    UAL_AUDIOPLAYER_MSG_BUFFERING_END,
    //停止
    UAL_AUDIOPLAYER_MSG_STOP_CNF, //第二阶段
    UAL_AUDIOPLAYER_MSG_STOP_IND,
    //恢复
    UAL_AUDIOPLAYER_MSG_RESUME_IND,
    //媒体信息
    UAL_AUDIOPLAYER_MSG_UPDATE_MEDIA_INFO, //更新媒体信息
    //系统异常情况
    UAL_AUDIOPLAYER_MSG_ERR, //网络异常，存储空间满
    UAL_AUDIOPLAYER_MSG_NET_NOT_CONNECTED,   //网络未连接
    UAL_AUDIOPLAYER_MSG_NET_TIMEOUT,         //网络连接超时
    UAL_AUDIOPLAYER_MSG_NET_CONNECT_ERR,    //网络连接失败
    UAL_AUDIOPLAYER_MSG_SRC_FORMAT_NOT_SUPPORT,   //音频源格式不支持
    UAL_AUDIOPLAYER_MSG_MAX
}ual_audioplayer_msg_e;

//反馈给app发生错误的原因
typedef enum
{
    UAL_AUDIOPLAYER_ERR_NONE,
    UAL_AUDIOPLAYER_PARAM_ERROR,
    UAL_AUDIOPLAYER_NO_ENOUGH_MMEMORY,
    UAL_AUDIOPLAYER_SET_VOLUME_FAIL,
    UAL_AUDIOPLAYER_SET_ROUTE_FAIL,
    UAL_AUDIOPLAYER_GET_PLAY_PROGRESS_FAIL,
    UAL_AUDIOPLAYER_PLAY_FAIL,
    UAL_AUDIOPLAYER_STOP_FAIL,
    UAL_AUDIOPLAYER_RELEASE_FAIL,
    UAL_AUDIOPLAYER_PAUSE_FAIL,
    UAL_AUDIOPLAYER_RESUME_FAIL,
    UAL_AUDIOPLAYER_SEEK_FAIL,
    UAL_AUDIOPLAYER_UNKNOW_ERROR,
    UAL_AUDIOPLAYER_CREATE_FAIL,
    UAL_AUDIOPLAYER_ERR_MAX
}ual_audioplayer_err_e;

//音频通路的类型
typedef enum
{
    UAL_AUDIOPLAYER_ROUTE_NONE,
    UAL_AUDIOPLAYER_ROUTE_AUTO = UAL_AUDIOPLAYER_ROUTE_NONE,
    UAL_AUDIOPLAYER_ROUTE_BLUETOOTH = (0x0001),     //蓝牙通路
    UAL_AUDIOPLAYER_ROUTE_EARPHONE =  (0x0001<<1),  //耳机通路
    UAL_AUDIOPLAYER_ROUTE_EARFREE =   (0x0001<<2),  //耳机和外放通路
    UAL_AUDIOPLAYER_ROUTE_SPEAKER =   (0x0001<<3),  //外放通路
    UAL_AUDIOPLAYER_ROUTE_HANDHOLD =  (0x0001<<4),  //call and key pad声音通路
    UAL_AUDIOPLAYER_ROUTE_MAX
}ual_audioplayer_route_e;

//audio player状态机的状态
typedef enum
{
    UAL_AUDIOPLAYER_STATE_IDLE,
    UAL_AUDIOPLAYER_STATE_READY,
    UAL_AUDIOPLAYER_STATE_SOURCE_DATA_PREPARE,
    UAL_AUDIOPLAYER_STATE_PLAY_WAITING,
    UAL_AUDIOPLAYER_STATE_BUFFERING,
    UAL_AUDIOPLAYER_STATE_PLAYING,
    UAL_AUDIOPLAYER_STATE_PAUSE,
    UAL_AUDIOPLAYER_STATE_DESTROYING,
    UAL_AUDIOPLAYER_STATE_MAX
}ual_audioplayer_state_e;

//seek 方式
typedef enum
{
    UAL_AUDIOPLAYER_SEEK_TYPE_BYTE,  //按字节方式跳转
    UAL_AUDIOPLAYER_SEEK_TYPE_TIME,  //按时间方式跳转
    UAL_AUDIOPLAYER_SEEK_TYPE_MAX
}ual_audioplayer_seek_e;


//play接口的入参结构体
typedef struct
{
    wchar*  p_audio_source; //本地音频路径或网络url路径
    uint32  audio_source_len;
    uint32  offset_time;    //播放起始时间
}ual_audioplayer_play_param_t;

//seek接口入参
typedef struct
{
    uint32  seek_pos;
    uint32  seek_time;  //单位:s
    ual_audioplayer_seek_e  seek_type;
}ual_audioplayer_seek_param_t;

//播放进度,单位(秒/s)
typedef struct
{
    uint32  cur_time;    //当前播放长度
    uint32  total_time;  //总长度
}ual_audioplayer_play_progress_info_t;

//下载进度
typedef struct
{
    uint32  cur_recv_data_length;  //当前收到的长度
    uint32  total_data_length;     //数据总长度
    uint32  download_time;         //下载偏移,单位(秒/s)
}ual_audioplayer_download_progress_info_t;

//媒体信息
typedef struct
{
    uint32 total_time;
    uint32  samplerate;    //采样率
    uint32  channel_num;   //通道
    uint32  bitrate;       //波特率
    wchar   year[UAL_AUDIOPLAYER_YEAR_LEN+1];     //年
    wchar   artist[UAL_AUDIOPLAYER_ARTIST_LEN+1]; //作者
    wchar   album[UAL_AUDIOPLAYER_ALBUM_LEN+1];   //专辑
    wchar   title[UAL_AUDIOPLAYER_TITLE_LEN+1];   //音乐名
    wchar   track[UAL_AUDIOPLAYER_TRACK_LEN+1];   //音轨
    wchar   genre[UAL_AUDIOPLAYER_GENRE_LEN+1];   //流派
    uint8*  p_apic_info;    //专辑图片
    uint32  apic_info_size;//专辑图片数据大小
}ual_audioplayer_media_info_t;

//下载的帧数据
typedef struct
{
    uint8*  p_data_buffer;  //单次下载数据DATA
    uint16  data_size;      //单次下载数据SIZE
}ual_audioplayer_download_data_info_t;

//play接口的入参结构体
typedef struct
{
    ual_audioplayer_route_e cur_route;
}ual_audioplayer_route_info_t;

//回调函数的参数结构
typedef struct
{
    ual_audioplayer_handle handle;
    void *param_ptr;
    uint32 size_of_param;
} ual_audioplayer_callback_param_t;

/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: 初始化ual audioplayer
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: zirui.li
//  Note:开机时调用此函数进行ual audioplayer的初始化
/*****************************************************************************/
void ual_audioplayer_init(void);

/*****************************************************************************/
//  Description: 申请audio player Service 的接口
//  Parameter: [In] p_callback //注册的回调函数
//             [Out] p_handle //创建的Audio Player的handle
//             [Return] ual_audioplayer_err_e //创建结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_create(_ual_cms_client_register_callback p_callback,
                                                 ual_audioplayer_handle* p_handle);

/*****************************************************************************/
//  Description: 销毁audio player Service 的接口
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //销毁结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_destroy(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: 音乐播放器播放接口
//  Parameter: [In] handle //音乐播放器的handle
//             [In] p_play_param //播放参数
//             [Out] None
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_play(ual_audioplayer_handle handle,
                                               ual_audioplayer_play_param_t* p_play_param);

/*****************************************************************************/
//  Description: 音乐播放器暂停接口
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_pause(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: 音乐播放器恢复接口
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_resume(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: 音乐播放器停止接口
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] None
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_stop(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: 音乐播放器seek接口
//  Parameter: [In] handle //音乐播放器的handle
//             [In] p_seek_param //seek参数
//             [Out] None
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_seek(ual_audioplayer_handle handle,
                                                ual_audioplayer_seek_param_t* p_seek_param);

/*****************************************************************************/
//  Description: 获取当前audio player service 的状态
//  Parameter: [In] audioplayer_handle //音乐播放器的handle
//             [Out] None
//             [Return] ual_audioplayer_state_e //获取的当前状态
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_state_e ual_audioplayer_get_state(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: 获取音乐播放器的播放进度
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] p_progress_info //播放进度信息
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_get_play_progress_info(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_play_progress_info_t* p_progress_info);

/*****************************************************************************/
//  Description: 获取音乐播放器的媒体信息
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] p_media_info //媒体信息
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_get_media_info(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_media_info_t* p_media_info);

/*****************************************************************************/
//  Description: 获取当前下载的进度信息/缓冲进度
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] p_progress_info //缓冲进度信息
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_get_download_progress_info(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_download_progress_info_t* p_progress_info);

/*****************************************************************************/
//  Description: 设置音量
//  Parameter: [In] handle //音乐播放器的handle
//             [In] volume //音量值
//             [Out] None
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_set_volume(
                                               ual_audioplayer_handle handle,
                                               uint32 volume);

/*****************************************************************************/
//  Description: 获取音量
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] None
//             [Return] uint32 //获取的音量值
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
uint32 ual_audioplayer_get_volume(ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: 通路切换
//  Parameter: [In] handle //音乐播放器的handle
//             [In] volume //音量值
//             [Out] None
//             [Return] ual_audioplayer_err_e //接口调用结果
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_err_e ual_audioplayer_set_route(
                                               ual_audioplayer_handle handle,
                                               ual_audioplayer_route_info_t audio_route);

/*****************************************************************************/
//  Description: 获取当前通路
//  Parameter: [In] handle //音乐播放器的handle
//             [Out] None
//             [Return] ual_audioplayer_route_e //获取的当前通路
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
ual_audioplayer_route_e ual_audioplayer_get_route(
                                               ual_audioplayer_handle handle);

/*****************************************************************************/
//  Description: 销毁所有的实例
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
void ual_audioplayer_destroy_all_instances(void);

/*****************************************************************************/
//  Description: 恢复出厂设置
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
void ual_audioplayer_restore_factory(void);
#endif
