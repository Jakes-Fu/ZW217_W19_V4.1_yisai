//
//	ContactDialog.h
//
#ifndef	__CONTACT_DIALOG_H
#define	__CONTACT_DIALOG_H

#include "_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	EContactDialogResult_OK,
	EContactDialogResult_Cancel,
	EContactDialogResultCount
} TContactDialogResultEnum;

typedef struct _tagContactItem  _ContactItem;
struct _tagContactItem
{
	_WCHAR* m_strName;
	_WCHAR* m_strTel;
	_ContactItem*	m_pNext;
};

typedef _BOOLEAN (*ContactDialogCallbackProc)(_HANDLE hDialog, _VOID* pParam, _INT nResult, _ContactItem* pItem);

_HANDLE ContactDialog_Create(_CONST _WCHAR* strTitle, ContactDialogCallbackProc pCallbackProc, _VOID* pParam);
_VOID ContactDialog_Release(_HANDLE hDialog);
_VOID ContactDialog_Run(_HANDLE hDialog);

#ifdef __cplusplus
}
#endif

#endif
