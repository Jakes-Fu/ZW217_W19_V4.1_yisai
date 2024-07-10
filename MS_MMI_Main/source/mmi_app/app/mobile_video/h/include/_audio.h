//
//	_Audio.h
//
#ifndef _AUDIO_API_H
#define _AUDIO_API_H

#ifdef _cplusplus
extern "C"
{
#endif

typedef _VOID (*AudioCallbackProc)(_HANDLE, _UINT, _UINT32, _UINT32, _VOID*);

extern _HANDLE Audio_Create();
extern _VOID Audio_SetProperty(_HANDLE hAudio, _INT nSampleRate, _INT nChannels,
					AudioCallbackProc pCallbackFunc, _VOID* pCallbackFuncParam);
extern _BOOLEAN Audio_OpenDevices(_HANDLE hAudio);
extern _VOID Audio_Release(_HANDLE hAudio);
extern _BOOLEAN Audio_OpenFile(_HANDLE hAudio, _WCHAR* wstrFilename);
extern _BOOLEAN Audio_AppendPCMBuffer(_HANDLE hAudio, _BYTE* pBuffer, _INT nSize);
extern _BOOLEAN Audio_Play(_HANDLE hAudio);
extern _BOOLEAN Audio_Stop(_HANDLE hAudio);
extern _BOOLEAN Audio_Pause(_HANDLE hAudio);
extern _BOOLEAN Audio_Resume(_HANDLE hAudio);
extern _BOOLEAN Audio_Seek(_HANDLE hAudio, _INT nTimeStamp);
extern _INT  Audio_GetLength();
extern _INT  Audio_GetPosition(_HANDLE hAudio);

#ifdef _cplusplus
}
#endif

#endif
