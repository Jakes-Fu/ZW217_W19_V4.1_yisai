#ifndef _MV_UI_API_H
#define _MV_UI_API_H

#include "_Types.h"
#include "_EditDialog.h"
#include "mmk_type.h"



typedef struct 
{
    MMI_STRING_T default_text;
    MMI_STRING_T  title;
    uint16       max_ucs2_len;
	uint32       allow_inputmethod;
    EditDialogCallbackProc CallbackProc;
    void        *CallbackProcParam;
}MMIMV_TEXTEDITBOX_INIT_DATA_T;



_HANDLE CM_EditDialog_Create(_EditDialogData* pEditDialogData);

_BOOLEAN CM_EditDialog_Release(_HANDLE hDialog);

_BOOLEAN CM_EditDialog_Run(_HANDLE hDialog);

/*****************************************************************************/
//  Description : CM_EditDialog_Create
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIMV_TEXTEDITBOX_INIT_DATA_T MV_GetEditDialogInitData(void);
#endif





















