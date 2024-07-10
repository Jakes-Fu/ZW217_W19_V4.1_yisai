/*****************************************************************************
** File Name:      notifycenter_text.h                                       *
** Author:         longwei.qiao                                              *
** Date:           01/2021                                                   *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe notifycenter                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _WATCH_NOTIFYCENTER_TEXT_H_
#define _WATCH_NOTIFYCENTER_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "notifycenter_mdu_def.h"
#endif

typedef enum WATCH_NOTIFYCENTER_TEXT_ID_E
{
        DROPDOWNWIN_TXT_NULL = ( WATCH_MODULE_NOTIFYCENTER << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "notifycenter_mdu_def.h"
#endif
        DROPDOWNWIN_TXT_MAX
} WATCH_NOTIFYCENTER_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // _WATCH_NOTIFYCENTER_TEXT_H_

