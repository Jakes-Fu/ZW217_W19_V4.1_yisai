#define _MV_UI_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_id.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv_ui_api.h"
#include "sig_code.h"
#include "mv_base_api.h"
#include "mmk_app.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

LOCAL MMIMV_TEXTEDITBOX_INIT_DATA_T  s_edit_init_data = {0};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : CM_EditDialog_Create
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIMV_TEXTEDITBOX_INIT_DATA_T MV_GetEditDialogInitData(void)
{
    return s_edit_init_data;
}

/*****************************************************************************/
//  Description : CM_EditDialog_Create
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_HANDLE CM_EditDialog_Create(_EditDialogData* pEditDialogData)
{
    

	if (PNULL == pEditDialogData)
	{
        return PNULL;
	}
    
	
	s_edit_init_data.max_ucs2_len = (uint16)(pEditDialogData->nMaxSize);
	s_edit_init_data.title.wstr_ptr = (wchar *)(pEditDialogData->strTitle);
    if (PNULL != s_edit_init_data.title.wstr_ptr)
    {
        s_edit_init_data.title.wstr_len = (uint16)MMIAPICOM_Wstrlen(s_edit_init_data.title.wstr_ptr);
    }
    else
    {
        s_edit_init_data.title.wstr_len = 0;
    }
    
	s_edit_init_data.default_text.wstr_ptr = (wchar *)(pEditDialogData->strDefaultText);
    if (PNULL != s_edit_init_data.default_text.wstr_ptr)
    {
        s_edit_init_data.default_text.wstr_len = (uint16)MMIAPICOM_Wstrlen(s_edit_init_data.default_text.wstr_ptr);
    }
    else
    {
        s_edit_init_data.default_text.wstr_len = 0;
    }

	s_edit_init_data.CallbackProc = pEditDialogData->pCallbackProc;
	s_edit_init_data.CallbackProcParam = pEditDialogData->pCallbackProcParam;
    s_edit_init_data.allow_inputmethod = pEditDialogData->uFlags;

    //MV_TRACE_LOW:"[MV] CM_EditDialog_Create max_len = %d, title_len = %d, default_len = %d, inputmethod = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_UI_API_84_112_2_18_2_39_22_192,(uint8*)"dddd",s_edit_init_data.max_ucs2_len, s_edit_init_data.title.wstr_len, s_edit_init_data.default_text.wstr_len,s_edit_init_data.allow_inputmethod);
    CM_SendSignalToMMI(MVIDEO_OPEN_EDIT_WIN, PNULL);  


	return (_HANDLE)MMI_EDITCOMMON_WIN_ID;
	
}

/*****************************************************************************/
//  Description : CM_EditDialog_Release
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_EditDialog_Release(_HANDLE hDialog)
{
	if ((_HANDLE)MMI_EDITCOMMON_WIN_ID != hDialog)
	{
        return _FALSE;
    }

	//MV_TRACE_LOW:"[MV] cm_editdialog_release"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_UI_API_103_112_2_18_2_39_22_193,(uint8*)"");

	return _TRUE;

}


/*****************************************************************************/
//  Description : CM_EditDialog_Run
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_EditDialog_Run(_HANDLE hDialog)
{
    UNUSED(hDialog);
	return MMK_IsOpenWin(MMI_EDITCOMMON_WIN_ID);	
}

#ifdef WIN32

_INT CM_Key_CallBack(_INT keyCode, _INT Digital, _INT TypeOfPress)
{
    return 0;
}

_INT CM_Touch_CallBack(_INT vEvent, _INT x, _INT y, _INT vParam)
{
    return 0;
}
#endif
