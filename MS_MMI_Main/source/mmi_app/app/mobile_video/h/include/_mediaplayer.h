//
//	_MediaPlayer.h
//
#ifndef	__MEDIAPLAYER_API_H
#define	__MEDIAPLAYER_API_H

#include "_API.h"

#ifdef	__cplusplus
extern "C"
{
#endif	

typedef enum
{
	PROGRAM_LOCAL,
	PROGRAM_DEMAND,
	PROGRAM_LIVE,
	PROGRAMCOUNT
}TMeidaPlayer_Program;

typedef _VOID (*MediaPlayerCallbackProc)(_HANDLE hPlayer, _VOID* pProcParam, _INT32 nType, _INT32 nParam);

extern _HANDLE MediaPlayerObject_Create(_INT x, _INT y, _INT w, _INT h,MediaPlayerCallbackProc pProc, _VOID* pProcParam, _INT nProgramType);
extern _BOOLEAN MediaPlayerObject_Release(_HANDLE handle);
extern _BOOLEAN MediaPlayerObject_Delete(_HANDLE handle);

extern _VOID MediaPlayerObject_SetPlayWnd(_HANDLE handle, _BOOLEAN bRotate, _BOOLEAN bZoom,_INT x, _INT y, _INT w, _INT h);
extern _BOOLEAN MediaPlayerObject_Open(_HANDLE handle,_CONST _WCHAR* strFileBuffer,_INT strFileSize);
extern _BOOLEAN MediaPlayerObject_Play(_HANDLE handle);
extern _BOOLEAN MediaPlayerObject_Seek(_HANDLE handle,_INT);
extern _BOOLEAN MediaPlayerObject_Pause(_HANDLE handle);
extern _BOOLEAN MediaPlayerObject_Stop(_HANDLE handle);
extern _VOID MediaPlayerObject_VideoClose(_HANDLE handle,_BOOLEAN);
extern _VOID MediaPlayerObject_FrameSize(_HANDLE handle,_INT* aWidth,_INT* aHeight);
extern _INT  MediaPlayerObject_GetMaxVolume(_HANDLE handle);
extern _INT  MediaPlayerObject_GetCurVolume(_HANDLE handle);
extern _BOOLEAN MediaPlayerObject_SetVolume(_HANDLE handle,_INT);
extern _VOID MediaPlayerObject_SetMute(_HANDLE handle,_BOOLEAN nFlag);
extern _VOID MediaPlayerObject_SetIAP(_HANDLE handle,_INT);
extern _INT  MediaPlayerObject_GetCurTime(_HANDLE handle);
extern _INT  MediaPlayerObject_GetTotalTime(_HANDLE handle);
extern _INT  MediaPlayerObject_GetPercent(_HANDLE handle);
extern _BOOLEAN MediaPlayerObject_RePlay(_HANDLE handle);
extern _BOOLEAN MediaPlayerObject_Resume(_HANDLE handle);
extern _VOID MediaPlayerObject_GetVersion(_WCHAR*	szVer, _INT nSize);
extern _BOOLEAN MediaPlayerObject_TimeSlice(_HANDLE hPlayer);
extern _BOOLEAN MediaPlayerObject_Draw(_HANDLE hPlayer, _HANDLE hCanvas);

extern _BOOLEAN MediaPlayerObject_MoveWindow(_HANDLE hPlayer, _INT x, _INT y, _INT nWidth, _INT nHeight, _INT nRotate, _INT nFullScreen);

//***2009-06-04`
extern _BOOLEAN MediaPlayerObject_Media_IS_CREAT(_VOID);
extern _BOOLEAN MediaPlayerObject_CanBeCreate();
extern _BOOLEAN MediaPlayerObject_GetVideoCloseFlag(_HANDLE handle);
extern _VOID MediaPlayerObject_SetVideoCloseFlag(_HANDLE handle);
extern _VOID MediaPlayerObject_VideoClose(_HANDLE handle,_BOOLEAN nSwith);

#ifdef	__cplusplus
}
#endif

#endif
