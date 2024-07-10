
#ifndef __EJTTSPLAYER_H
#define __EJTTSPLAYER_H
/************************************************************************/
/* 此文件用于展讯平台                                                   */
/************************************************************************/
#include "eJTTS.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TTSAPI
	
#define ERRCODE				jtErrCode
#define JTTSPARAM			jtTTSParam

//与引擎层相同的错误
#define TTS_PLAYER_ERR_NONE				jtTTS_ERR_NONE
#define TTS_PLAYER_ERR_PARAM			jtTTS_ERR_INPUT_PARAM
#define TTS_PLAYER_ERR_TOO_MORE_TEXT	jtTTS_ERR_TOO_MORE_TEXT
#define TTS_PLAYER_ERR_NOTINIT			jtTTS_ERR_NOT_INIT
#define TTS_PLAYER_ERR_OPENFILE			jtTTS_ERR_OPEN_DATA
#define TTS_PLAYER_ERR_ENGINE			jtTTS_ERR_ENGINE

//Player层独有的错误
typedef enum _TTS_PLAYER_ERR_MORE
{
	TTS_PLAYER_ERR_ALREADYINIT = 50,
	TTS_PLAYER_ERR_MEMORY,
	TTS_PLAYER_ERR_DONOTHING,
	TTS_PLAYER_ERR_PLAYING,
	TTS_PLAYER_ERR_AUDIO_DEVICE
}TTS_PLAYER_ERR_MORE;

//回调通知类型
typedef enum _TTS_PLAYER_NOTIFY
{
	TTS_PLAYER_NOTIFY_BEGIN,
	TTS_PLAYER_NOTIFY_END,
	TTS_PLAYER_NOTIFY_ERROR
}TTS_PLAYER_NOTIFY;

//播放类型
typedef enum _TTS_PLAYER_PLAY_MODE
{
	TTS_PLAYER_PLAY_ASYNC,
	TTS_PLAYER_PLAY_SYNC
}jtTTSPlayMode;

//停止类型
typedef enum _TTS_PLAYER_STOP_MODE
{
	TTS_PLAYER_STOP_ASYNC,
	TTS_PLAYER_STOP_BLOCK
}jtTTSStopMode;

typedef enum _TTS_PLAYER_STATUS
{
	TTS_PLAYER_NOT_INIT,
	TTS_PLAYER_IDLE,
	TTS_PLAYER_SYNTHESIZING,
	TTS_PLAYER_PLAYING,
	TTS_PLAYER_PAUSE
}jtTTSPlayerStatus;
	
// user define callback type
typedef jtErrCode (* JTTSCALLBACKPROCEX)(
	long uMessage,				// [in] Message
	long lParam,				// [in] parameter 
	unsigned long wParam);		// [in] parameter 

/*****************************************************************************/
//  Description : ejTTSPlayer_Init 播放器初始化
//  Global resource dependence : none
//  Author: yaye.jiang
// params	const char *szCNLib	中文音库
//			const char *szEnLib 英文音库
//			const char *szDMLib 定制库 
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Init(const char *szCNLib, const char *szENLib, const char *szDMLib);

/*****************************************************************************/
//  Description : 退出 ejttsPlayer
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_End(void);

/*****************************************************************************/
//  Description : ejttsplayer 播放
//  Global resource dependence : none
//  Author: yaye.jiang
//jtTTSPlayMode ePlayMode 播放方式
//params char * pcszText 合成文本指针
//JTTSCALLBACKPROCEX lpfnCallback 回调函数
//DWORD dwUserData  
/*****************************************************************************/ 
PUBLIC uint32 ejTTSPlayer_Play(jtTTSPlayMode ePlayMode, char *pszText, JTTSCALLBACKPROCEX lpfnCallback, unsigned long dwUserData);

/*****************************************************************************/
//  Description : ejttsplayer 将合成数据保存到文件
//  Global resource dependence : none
//  Author: yaye.jiang
//params char * pcszText 合成文本指针
//char* pcszFileName 保存到文件名字
/*****************************************************************************/ 
PUBLIC uint32 ejTTSPlayer_PlayToFile(char *pszText, const char* pszFileName);

/*****************************************************************************/
//  Description :ejttsplayer 停止
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Stop(jtTTSStopMode eStopMode);

/*****************************************************************************/
//  Description :ejttsplayer 继续播放
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Resume(void);

/*****************************************************************************/
//  Description :ejttsplayer 暂停
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Pause(void);

/*****************************************************************************/
//  Description :ejttsplayer 获取当前的状态
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 *piStatus 状态
/*****************************************************************************/
PUBLIC uint32	ejTTSPlayer_GetStatus(int *piStatus);

/*****************************************************************************/
//  Description :ejttsPlayer 设置参数
//  Global resource dependence : none
//  Author: yaye.jiang
//JTTSPARAM nParam 参数类型
//DWORD dwValue 参数值
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_SetParam(JTTSPARAM nParam, unsigned long dwValue);
/*****************************************************************************/
//  Description :ejttsPlayer获取参数
//  Global resource dependence : none
//  Author: yaye.jiang
//JTTSPARAM nParam 参数类型
//unsigned long *pdwValue 参数值
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_GetParam(JTTSPARAM nParam, unsigned long *pdwValue);

PUBLIC uint32 ejTTSPlayer_TestFile(const char *szCNLib, char *pcszText, const char *pcszFileName);


#ifdef __cplusplus
}

#endif
#endif	//__EJTTSPLAYER_H

