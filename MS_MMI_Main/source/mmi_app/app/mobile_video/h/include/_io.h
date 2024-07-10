//
//	_IO.h
//
#ifndef __IO_API_H
#define	__IO_API_H

#ifdef	__cplusplus
extern "C"
{
#endif

//////////////////////////////////////////////////////////////////////////
//	Keyboard & Mouse
enum TKeyboardEvent
{
	KeyEvent_Press,
	KeyEvent_LongPress
};
enum TMouseEvent
{
	MouseEvent_Click,
	MouseEvent_Move,
};
typedef _BOOLEAN (*KeyboardCallbackProc)(_VOID* pCallbackProcParam, _INT nType, _UINT32, _UINT32);
typedef _BOOLEAN (*MouseCallbackProc)(_VOID* pCallbackProcParam, _INT nType, _UINT32, _UINT32);

_HANDLE Keyboard_AppendCallbackFunc(_INT nPriority, KeyboardCallbackProc pKeyboardCallbackProc, _VOID* pCallbackProcParam);
_VOID Keyboard_RemoveCallbackFunc(_HANDLE);

_HANDLE Mouse_AppendCallbackFunc(_INT nPriority, MouseCallbackProc pMouseCallbackProc, _VOID* pCallbackProcParam);
_VOID Mouse_RemoveCallbackFunc(_HANDLE pMouseCallbackProc);

//////////////////////////////////////////////////////////////////////////
//	File & Dir
#define	_MAX_FILENAME_LEN	(512)//zane

enum TOpenFileEnum
{
	fileRead	= 0x00,
	fileWrite	= 0x01,
	fileReadWrite= 0x01,
	fileCreate	= 0x02,
};
enum TSeekFileEnum
{
	seekFromBegin,
	seekFromEnd,
	seekFromCurrent,
};

extern _HANDLE	File_Open(_CONST _WCHAR*, _UINT32);
extern _VOID	File_Close(_HANDLE);
extern _INT		File_Read(_HANDLE, _VOID*, _INT);
extern _INT		File_Write(_HANDLE, _CONST _VOID*, _INT);
extern _VOID	File_Flush(_HANDLE);
extern _INT		File_GetLength(_HANDLE);
extern _BOOLEAN File_Seek(_HANDLE, _INT, _UINT);
extern _INT		File_GetPosition(_HANDLE);
extern _TIME	File_DateTime(_HANDLE);

extern _BOOLEAN _FileExists(_CONST _WCHAR*);
extern _BOOLEAN _MakeFolder(_CONST _WCHAR*);
extern _BOOLEAN _DeleteFolder(_CONST _WCHAR*);
extern _INT  _GetCurFolder(_WCHAR*, _INT);
extern _BOOLEAN _SetCurFolder(_CONST _WCHAR*);
extern _BOOLEAN _MoveFile(_CONST _WCHAR*, _CONST _WCHAR*);
extern _BOOLEAN _DeleteFile(_CONST _WCHAR*);

#define	FILEATTR_DIRECTORY	(0x0001)

struct tagFindFileData
{
	_WCHAR	wszFileName[_MAX_FILENAME_LEN];	//找到的文件全路径名
	_UINT	uAttr;	//文件属性: Bit:0,是否是目录
};
typedef struct tagFindFileData	FINDFILE_DATA;

extern _HANDLE _FindFirstFile(_CONST _WCHAR*, FINDFILE_DATA*);
extern _BOOLEAN _FindNextFile(_HANDLE, FINDFILE_DATA*);
extern _VOID _FindClose(_HANDLE);

//////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus
}
#endif

#endif
