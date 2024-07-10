/*****************************************************************************
** File Name:      HEROAPP_mmi_text.h                                         *
** Author:                                                                   *
** Date:           11/2009                                                   *
** Copyright:      sky-mobi.                                                 *
** Description:    This file is used to describe mrapp image resource        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _HEROAPP_MMI_TEXT_H_
#define _HEROAPP_MMI_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// ÎÄ×Ö×ÊÔ´ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum HEROAPP_MMI_TEXT_ID_E
{
	HEROAPP_TEXT_NULL = ( MMI_MODULE_HEROAPP << 16 ) | MMI_SYS_RES_FLAG,
#include "heroapp_mdu_def.h"
	HEROAPP_TXT_MAX
} HEROAPP_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif // _HEROAPP_MMI_TEXT_H_
