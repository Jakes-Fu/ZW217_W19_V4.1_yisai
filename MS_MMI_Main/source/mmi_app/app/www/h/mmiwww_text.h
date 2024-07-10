/***************************************************************************
** File Name:      mmiwww_text.h                                             *
** Author:                                                                 *
** Date:           15/06/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to www browser internal define        *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2009        jiaoyou.wu    Create
****************************************************************************/

#ifndef _MMIWWW_TEXT_H_
#define _MMIWWW_TEXT_H_

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
#include "www_mdu_def.h"
#endif

typedef enum MMIWWW_TEXT_ID_E
{
    MMIWWW_TEXT_NULL = ( MMI_MODULE_WWW << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "www_mdu_def.h"
#endif
    MMIWWW_TEXT_MAX
} MMIWWW_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // _MMIWWW_TEXT_H_
