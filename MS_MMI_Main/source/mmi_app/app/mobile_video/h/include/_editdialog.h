//
//	EditDialog.h
//
#ifndef __EDIT_DIALOG_H
#define __EDIT_DIALOG_H

#include "_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	EEditDialogResult_OK,
	EEditDialogResult_Cancel,
	EEditDialogResultCount
} TEditDialogResultEnum;

typedef _BOOLEAN (*EditDialogCallbackProc)(_HANDLE hDialog, _VOID* pParam, _INT nResult, _CONST _WCHAR* strString);
typedef _UINT (*EditNotifyCallbackProc)(_INT nNotify, _VOID* wParam, _VOID* pParam);

typedef enum
{
	EEditDialogOptions_Numeric = 0x0001,
	EEditDialogOptions_English = 0x0002,
	EEditDialogOptions_Chinese = 0x0004,
	EEditDialogOptionsAll = 0xFFFF
} TEditDialogOptions;

typedef struct _tagEditDialogData _EditDialogData;
struct _tagEditDialogData
{
	_UINT32	uFlags;
	_INT	nMaxSize;
	_CONST _WCHAR*	strTitle;
	_CONST _WCHAR*	strDefaultText;
	
	EditDialogCallbackProc	pCallbackProc;
	_VOID*	pCallbackProcParam;
	EditNotifyCallbackProc	pNotifyCallbackProc;
};

_HANDLE EditDialog_Create(_EditDialogData* pData);
_VOID EditDialog_Release(_HANDLE hDialog);
_VOID EditDialog_Run(_HANDLE hDialog);

#ifdef __cplusplus
}
#endif

#endif

