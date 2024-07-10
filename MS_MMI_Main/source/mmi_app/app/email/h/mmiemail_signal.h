/****************************************************************************
** File Name:      mmiemail_signal.h                                         *
** Author:                                                          			 *
** Date:           05/13/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/
#ifndef _MMIEMAIL_SIGNAL_H_
#define _MMIEMAIL_SIGNAL_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "guianim.h"
#include "mmifmm_export.h"
#include "mmiemail_def.h"

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
   MMIEMAIL_INVALIED = 0,

   MMIEMAIL_MMI_APP,
   MMIEMAIL_EMAIL_APP,
   MMIEMAIL_END = 0x7fffffff
}MMIEMAIL_TASK_TYPE_E;

typedef enum
{
	MMIEMAIL_EVT_INVALIED = 0,
	MMIEMAIL_EVT_SAVE_EMAIL,
    MMIEMAIL_EVT_SAVE_AND_SEND_EMAIL,
	MMIEMAIL_EVT_END = 0x7fffffff
}MMIEMAIL_EVT_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: create edit window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
unsigned int MMIEMAIL_CreateEmailTaskThread(void);
unsigned int MMIEMAIL_DeleteEmailTaskThread(void);
unsigned int MMIEMAIL_EmailEventInterface(int vAppType , int vEventType, void* pMsg, void *hWnd);
PUBLIC void MMIEmail_ThreadEnable(void);
PUBLIC void MMIEmail_ThreadDisable(void);
PUBLIC void MMIEMAIL_GetMutex(void);
PUBLIC void MMIEMAIL_PutMutex(void);
/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* _MMIEMAIL_EDIT_H*/
