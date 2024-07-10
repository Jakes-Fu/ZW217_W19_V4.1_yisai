#ifndef _MV_CONTACT_API_H
#define _MV_CONTACT_API_H

#include "std_header.h"
#include "_Types.h"
#include "_ContactDialog.h"


_HANDLE CM_ContactDialog_Create(ContactDialogCallbackProc pCallbackProc,
										_VOID* pParam);

_BOOLEAN CM_ContactDialog_Release(_HANDLE hDialog);

_BOOLEAN CM_ContactDialog_Run(_HANDLE hDialog);



/************************************************************************/
//MV_ContactDialogCallbackProc
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
PUBLIC void MV_ContactDialogCallbackProc(TContactDialogResultEnum nResult);

/************************************************************************/
//获得联系人信息
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
PUBLIC _ContactItem * MV_GetContactInfo(uint32 handle, uint16 max_len);

#endif