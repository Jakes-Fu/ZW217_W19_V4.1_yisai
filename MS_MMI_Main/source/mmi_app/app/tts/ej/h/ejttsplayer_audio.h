
#ifndef __EJTTSPLAYER_H
#define __EJTTSPLAYER_H
/************************************************************************/
/* ���ļ�����չѶƽ̨                                                   */
/************************************************************************/
#include "eJTTS.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TTSAPI
	
#define ERRCODE				jtErrCode
#define JTTSPARAM			jtTTSParam

//���������ͬ�Ĵ���
#define TTS_PLAYER_ERR_NONE				jtTTS_ERR_NONE
#define TTS_PLAYER_ERR_PARAM			jtTTS_ERR_INPUT_PARAM
#define TTS_PLAYER_ERR_TOO_MORE_TEXT	jtTTS_ERR_TOO_MORE_TEXT
#define TTS_PLAYER_ERR_NOTINIT			jtTTS_ERR_NOT_INIT
#define TTS_PLAYER_ERR_OPENFILE			jtTTS_ERR_OPEN_DATA
#define TTS_PLAYER_ERR_ENGINE			jtTTS_ERR_ENGINE

//Player����еĴ���
typedef enum _TTS_PLAYER_ERR_MORE
{
	TTS_PLAYER_ERR_ALREADYINIT = 50,
	TTS_PLAYER_ERR_MEMORY,
	TTS_PLAYER_ERR_DONOTHING,
	TTS_PLAYER_ERR_PLAYING,
	TTS_PLAYER_ERR_AUDIO_DEVICE
}TTS_PLAYER_ERR_MORE;

//�ص�֪ͨ����
typedef enum _TTS_PLAYER_NOTIFY
{
	TTS_PLAYER_NOTIFY_BEGIN,
	TTS_PLAYER_NOTIFY_END,
	TTS_PLAYER_NOTIFY_ERROR
}TTS_PLAYER_NOTIFY;

//��������
typedef enum _TTS_PLAYER_PLAY_MODE
{
	TTS_PLAYER_PLAY_ASYNC,
	TTS_PLAYER_PLAY_SYNC
}jtTTSPlayMode;

//ֹͣ����
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
//  Description : ejTTSPlayer_Init ��������ʼ��
//  Global resource dependence : none
//  Author: yaye.jiang
// params	const char *szCNLib	��������
//			const char *szEnLib Ӣ������
//			const char *szDMLib ���ƿ� 
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Init(const char *szCNLib, const char *szENLib, const char *szDMLib);

/*****************************************************************************/
//  Description : �˳� ejttsPlayer
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_End(void);

/*****************************************************************************/
//  Description : ejttsplayer ����
//  Global resource dependence : none
//  Author: yaye.jiang
//jtTTSPlayMode ePlayMode ���ŷ�ʽ
//params char * pcszText �ϳ��ı�ָ��
//JTTSCALLBACKPROCEX lpfnCallback �ص�����
//DWORD dwUserData  
/*****************************************************************************/ 
PUBLIC uint32 ejTTSPlayer_Play(jtTTSPlayMode ePlayMode, char *pszText, JTTSCALLBACKPROCEX lpfnCallback, unsigned long dwUserData);

/*****************************************************************************/
//  Description : ejttsplayer ���ϳ����ݱ��浽�ļ�
//  Global resource dependence : none
//  Author: yaye.jiang
//params char * pcszText �ϳ��ı�ָ��
//char* pcszFileName ���浽�ļ�����
/*****************************************************************************/ 
PUBLIC uint32 ejTTSPlayer_PlayToFile(char *pszText, const char* pszFileName);

/*****************************************************************************/
//  Description :ejttsplayer ֹͣ
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Stop(jtTTSStopMode eStopMode);

/*****************************************************************************/
//  Description :ejttsplayer ��������
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Resume(void);

/*****************************************************************************/
//  Description :ejttsplayer ��ͣ
//  Global resource dependence : none
//  Author: yaye.jiang
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_Pause(void);

/*****************************************************************************/
//  Description :ejttsplayer ��ȡ��ǰ��״̬
//  Global resource dependence : none
//  Author: yaye.jiang
//params uint32 *piStatus ״̬
/*****************************************************************************/
PUBLIC uint32	ejTTSPlayer_GetStatus(int *piStatus);

/*****************************************************************************/
//  Description :ejttsPlayer ���ò���
//  Global resource dependence : none
//  Author: yaye.jiang
//JTTSPARAM nParam ��������
//DWORD dwValue ����ֵ
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_SetParam(JTTSPARAM nParam, unsigned long dwValue);
/*****************************************************************************/
//  Description :ejttsPlayer��ȡ����
//  Global resource dependence : none
//  Author: yaye.jiang
//JTTSPARAM nParam ��������
//unsigned long *pdwValue ����ֵ
/*****************************************************************************/
PUBLIC uint32 ejTTSPlayer_GetParam(JTTSPARAM nParam, unsigned long *pdwValue);

PUBLIC uint32 ejTTSPlayer_TestFile(const char *szCNLib, char *pcszText, const char *pcszFileName);


#ifdef __cplusplus
}

#endif
#endif	//__EJTTSPLAYER_H

