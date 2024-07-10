/***************************************************************************
** File Name:      mmimail_text.h                                          *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to define mail relative menus         *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_TEXT_H_
#define _MMIMAIL_TEXT_H_

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
#include "push_mail_mdu_def.h"
#endif

typedef enum MMIMAIL_TEXT_ID_E
{
	MAIL_TXT_NULL = (MMI_MODULE_PUSHMAIL << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "push_mail_mdu_def.h"
#endif
	MAIL_TXT_MAX
} MMIMAIL_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // _MMIMAIL_TEXT_H_

