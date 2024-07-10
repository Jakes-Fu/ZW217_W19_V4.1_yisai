/***************************************************************************************
** File Name:      mmiemail_box.h                                                      *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:                                                                        *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_box.h                               *
**                                                                                     *
***************************************************************************************/

#ifndef _MMIEMAIL_BOX_H_
#define _MMIEMAIL_BOX_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmiemail_vector.h"

/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef enum
{
    MMIEMAIL_BOX_FROM_INBOX,
    MMIEMAIL_BOX_FROM_OUTBOX,
    MMIEMAIL_BOX_FROM_SENTBOX,
    MMIEMAIL_BOX_FROM_DRAFT,
    MMIEMAIL_BOX_FROM_MAX
} MMIEMAIL_BOX_REASON_TYPE_E;

typedef enum
{
    MMIEMAIL_TRANS_MSGHEAD,
    MMIEMAIL_TRANS_MSG,
    MMIEMAIL_TRANS_EMLBODY,
    MMIEMAIL_TRANS_MAX
} MMIEMAIL_TRANS_TYPE_E;

/*-------------------------------------------------------------------------------------*
**                           FUNCTION DECLARE                                          *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : to create box window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_CreateBoxWin(MMIEMAIL_BOX_REASON_TYPE_E box_type, void *box_data_ptr);
/**************************************************************************************/
// Description : to retrieve email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_PublicProc_Retrieve(void);
/**************************************************************************************/
// Description : to retrieve email body.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_PublicProc_RetrieveEmlBody(uint32 box_msg_id, uint32 net_id);
/**************************************************************************************/
// Description : to get vector info and select index.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_GetVectorinfoAndSelectIndex(uint32 *box_index_ptr, EMAIL_VECTOR_T **vector_pptr);
/**************************************************************************************/
// Description : handle to delete dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC MMI_RESULT_E MMIEMAIL_DelDlg_HandleEvent(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                );
/**************************************************************************************/
// Description : to get the number of box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_GetEmailNum(
                                    uint32 wParam,
                                    uint32 dwParam
                                    );
/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* _MMIEMAIL_BOX_H*/
