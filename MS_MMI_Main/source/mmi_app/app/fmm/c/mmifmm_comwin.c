/*****************************************************************************
** File Name:      mmifmm_comwin.c                                           *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#define _MMIFMM_COMWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_comwin.h"
#include "mmk_msg.h"
#include "mmk_app.h"
#include "mmi_text.h"
#include "mmifmm_id.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_appmsg.h"
#include "mmi_filetask.h"
#include "mmi_image.h"
#include "mmi_modu_main.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleOverWriteQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIFMM_QUERYWIN_DATA_T *win_data_p = (MMIFMM_QUERYWIN_DATA_T*) MMK_GetWinUserData (win_id);

    switch (msg_id)
    {
    case MSG_APP_OK:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:

        if (PNULL != win_data_p)
        {
            if (MMK_IsOpenWin (win_data_p->win_id))
            {
                MMK_SendMsg (win_data_p->win_id, MSG_PROMPTWIN_OK, PNULL);
            }
        }

        MMK_CloseWin (win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        //enter fmm explorer
        if (PNULL != win_data_p)
        {
            if (MMK_IsOpenWin (win_data_p->win_id))
            {
                MMK_SendMsg (win_data_p->win_id, MSG_PROMPTWIN_CANCEL, PNULL);
            }
        }

        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMIFMM_QueryCloseCtrlFunc (win_id);
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_QueryAddCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_QUERYWIN_DATA_T *query_data
)
{
    if (PNULL != query_data)
    {
        MMIFMM_QUERYWIN_DATA_T *query_data_ptr = (MMIFMM_QUERYWIN_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_QUERYWIN_DATA_T));

        query_data_ptr->win_id = query_data->win_id;
        query_data_ptr->win_id2 = query_data->win_id2;

        MMK_SetWinUserData (win_id, (void *) query_data_ptr);

        return MMI_RESULT_TRUE;
    }

    return MMI_RESULT_FALSE;

}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_QueryCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_QUERYWIN_DATA_T *win_data_p = (MMIFMM_QUERYWIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        SCI_FREE (win_data_p);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : open wait win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenComWaitWin (MMIPUB_HANDLE_FUNC    func)
{
    MMI_STRING_T wait_text = {0};
    MMI_GetLabelTextByLang (TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin (1,
                        &wait_text,
                        PNULL,
                        PNULL,
                        MMIFMM_FILE_WAITING_WIN_ID,
                        IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,
                        WIN_ONE_LEVEL,
                        MMIPUB_SOFTKEY_ONE,
                        (MMIPUB_HANDLE_FUNC) func);
}






