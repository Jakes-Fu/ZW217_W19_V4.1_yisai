#ifndef _MV_PLAYER_API_H
#define _MV_PLAYER_API_H

#include "_Types.h"
#include "sig_code.h"
#include "_MediaPlayer.h"


typedef enum
{
    PLAYER_CREATE = 0,  //创建播放器
	PLAYER_CREATE_OK = 1, //创建播放器成功
	PLAYER_CREATE_ERR = 2, //创建播放器失败
    PLAYER_NOT_SUPPORT = 3, //格式不支持
    CONNECT_SERV_OK  = 4, //连接服务器成功
	CONNECT_SERV_ERR	= 5, //连接服务器失败
    REQUEST_SERV        = 6, //向服务器请求播放
	REQUEST_SERV_OK  = 7,   //请求成功
	REQUEST_SERV_ERR = 8,//请求失败
    SERV_RES_OK      = 9, //服务器恢复成功
	SERV_RES_ERR    = 10, //服务器回复失败
    DISCONNECT      = 11, //网络断开
    DISCONNECT_OK   = 12, //网络断开成功
	DISCONNECT_ERR = 13,  //网络断开失败

	DATA_BUFEER = 14, //缓冲中
	PLAY_VIDEO_OK = 15,//播放中
	PAUSE_VIDEO_OK = 16,//暂停中
	STOP_VIDEO_OK = 17, //停止播放
	PLAYER_RELEASE_OK = 18,//释放播放器
   
	PLAYER_ENDOFCLIP = 20,//播放结束
	PLAYER_FullSrccen = 21,//FullSreen

	SUPPORTED_AUDIO_ONLY = 22, //仅支持音频
	SUPPORTED_VIDEO_ONLY = 23, //仅支持视频
	SUPPORTED_NEITHER = 24,//无法播放，图像解码和音频解码均不支持
	SUPPORTED_OVER_SCALED = 25,//片源规格（最大分辨率）超出平台的支持范围 片源规格（最大分辨率）：640*480
}TMeidaPlayerStatus;



_HANDLE CM_MediaPlayer_Create(
									_INT left,
									_INT top,
									_INT width,
									_INT height,
									MediaPlayerCallbackProc pMediaPlayerCallbackProc,
									_VOID* pCallbackParam,
									_INT nProgram      //0 local, 1 点拨, 2直播 (rtsp)
);
_BOOLEAN CM_MediaPlayer_Release(_HANDLE hPlayer);

_BOOLEAN CM_MediaPlayer_Open(_HANDLE hPlayer,_CONST _WCHAR* pUrl,_INT nUrlSize,  _INT nProgram);

_BOOLEAN CM_MediaPlayer_Play(_HANDLE hPlayer);

_BOOLEAN CM_MediaPlayer_Pause(_HANDLE hPlayer);

_BOOLEAN CM_MediaPlayer_Seek(_HANDLE hPlayer,_INT nTimeStamp);

_BOOLEAN CM_MediaPlayer_Stop(_HANDLE hPlayer);

_BOOLEAN CM_MediaPlayer_SetPlayWnd(_HANDLE handle,
										_BOOLEAN bRotate,
										_BOOLEAN bZoom,
										_INT x,
										_INT y,
										_INT w,
										_INT h
);
_INT CM_MediaPlayer_GetMaxVolume(_HANDLE handle);

_INT  CM_MediaPlayer_GetCurVolume(_HANDLE handle);

_BOOLEAN CM_MediaPlayer_SetVolume(_HANDLE handle,_INT nVolume);

_INT CM_MediaPlayer_GetCurTime(_HANDLE handle);

_INT CM_MediaPlayer_GetTotalTime(_HANDLE handle);

_INT CM_MediaPlayer_GetPercent();

_BOOLEAN CM_MediaPlayer_CanBeCreate(void);

_HANDLE CM_MediaPlayer_RePlay(_HANDLE hPlayer);

/* function:ReSetDisPlayRect()
 * DES:根据屏幕初始值是否选中重新设置播放区域及全屏播放旋转处理
 * AUTHOR:
 * DATE:2011.06.23
*/
_VOID MV_ReSetDisplayParam(BOOLEAN is_ratate, _VOID* dis_param);
//下面俩函数用来处理并发时候的刷屏问题(刷掉并发事件窗口的残留)
#ifndef WIN32
PUBLIC _BOOLEAN CM_MediaPlayer_Suspend(_VOID);
PUBLIC _BOOLEAN CM_MediaPlayer_Reactive(_VOID);
#endif

/*****************************************************************************/
//  Description :MV_MediaPlayer_LoseFocus
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_LoseFocus(void);

/*****************************************************************************/
//  Description :MV_MediaPlayer_GetFocus
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_GetFocus(void);

/*****************************************************************************/
//  Description :MV_MediaPlayer_IsFullScreen
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MV_MediaPlayer_IsFullScreen(void);

/*****************************************************************************/
//  Description : MV_MediaPlayer_Create
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Create(void);

/*****************************************************************************/
//  Description : MV_MediaPlayer_Release
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Release(void);

/*****************************************************************************/
//  Description : MV_MediaPlayer_Exit
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Exit(void);
/*****************************************************************************/
//  Description : MV_MediaPlayer_Play
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Play(void);

/*****************************************************************************/
//  Description : this function is used to pause the player
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Pause(void);

/*****************************************************************************/
//  Description : this function is used to seek in media file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Seek(void);

/*****************************************************************************/
//  Description : MV_MediaPlayer_Stop
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_Stop(void);

/*****************************************************************************/
//  Description : MV_MediaPlayer_SetPlayWnd
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_SetPlayWnd(void);

/*****************************************************************************/
//  Description : this function is used to get current time
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_GetCurTime(void);

/*****************************************************************************/
//  Description : MV_MediaPlayer_UpdateVolume
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_MediaPlayer_UpdateVolume(void);

/*****************************************************************************/
//  Description :MV_IsPlaying
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MV_IsPlaying(void);
/*****************************************************************************/
//  Description :MV_IsLiving
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MV_IsLiving(void);

/*****************************************************************************/
//  Description :MV_Exit
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_Exit(void);

/*****************************************************************************/
//  Description :MV_Stop
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_Stop(void);

/*****************************************************************************/
//  Description For other app use, when lose focuse
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_Pause(void);

/*****************************************************************************/
// 	Description : environment is or not silent mode
//	Global resource dependence : none
//  Author: fuyan.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MV_IsSilentMode(void);

/*****************************************************************************/
// 	Description : to fill the special rect use the color key
//	Global resource dependence : none
//  Author: fuyan.zhang
//	Note:
/*****************************************************************************/
PUBLIC _VOID CM_MediaPlayer_FillSpecRect(_INT nX, _INT nY, _INT nWidth, _INT nHeight);

#endif
