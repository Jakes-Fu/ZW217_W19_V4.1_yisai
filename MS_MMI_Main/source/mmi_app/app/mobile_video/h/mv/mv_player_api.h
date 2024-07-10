#ifndef _MV_PLAYER_API_H
#define _MV_PLAYER_API_H

#include "_Types.h"
#include "sig_code.h"
#include "_MediaPlayer.h"


typedef enum
{
    PLAYER_CREATE = 0,  //����������
	PLAYER_CREATE_OK = 1, //�����������ɹ�
	PLAYER_CREATE_ERR = 2, //����������ʧ��
    PLAYER_NOT_SUPPORT = 3, //��ʽ��֧��
    CONNECT_SERV_OK  = 4, //���ӷ������ɹ�
	CONNECT_SERV_ERR	= 5, //���ӷ�����ʧ��
    REQUEST_SERV        = 6, //����������󲥷�
	REQUEST_SERV_OK  = 7,   //����ɹ�
	REQUEST_SERV_ERR = 8,//����ʧ��
    SERV_RES_OK      = 9, //�������ָ��ɹ�
	SERV_RES_ERR    = 10, //�������ظ�ʧ��
    DISCONNECT      = 11, //����Ͽ�
    DISCONNECT_OK   = 12, //����Ͽ��ɹ�
	DISCONNECT_ERR = 13,  //����Ͽ�ʧ��

	DATA_BUFEER = 14, //������
	PLAY_VIDEO_OK = 15,//������
	PAUSE_VIDEO_OK = 16,//��ͣ��
	STOP_VIDEO_OK = 17, //ֹͣ����
	PLAYER_RELEASE_OK = 18,//�ͷŲ�����
   
	PLAYER_ENDOFCLIP = 20,//���Ž���
	PLAYER_FullSrccen = 21,//FullSreen

	SUPPORTED_AUDIO_ONLY = 22, //��֧����Ƶ
	SUPPORTED_VIDEO_ONLY = 23, //��֧����Ƶ
	SUPPORTED_NEITHER = 24,//�޷����ţ�ͼ��������Ƶ�������֧��
	SUPPORTED_OVER_SCALED = 25,//ƬԴ������ֱ��ʣ�����ƽ̨��֧�ַ�Χ ƬԴ������ֱ��ʣ���640*480
}TMeidaPlayerStatus;



_HANDLE CM_MediaPlayer_Create(
									_INT left,
									_INT top,
									_INT width,
									_INT height,
									MediaPlayerCallbackProc pMediaPlayerCallbackProc,
									_VOID* pCallbackParam,
									_INT nProgram      //0 local, 1 �㲦, 2ֱ�� (rtsp)
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
 * DES:������Ļ��ʼֵ�Ƿ�ѡ���������ò�������ȫ��������ת����
 * AUTHOR:
 * DATE:2011.06.23
*/
_VOID MV_ReSetDisplayParam(BOOLEAN is_ratate, _VOID* dis_param);
//��������������������ʱ���ˢ������(ˢ�������¼����ڵĲ���)
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
