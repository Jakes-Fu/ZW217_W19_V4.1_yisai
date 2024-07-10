/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                  *
******************************************************************************/

#ifndef SEARCH_MMI_TEXT_H_
#define SEARCH_MMI_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "search_mdu_def.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// ÎÄ×Ö×ÊÔ´ID
///////////////////////////////////////////////////////////////////////////////

typedef enum SEARCH_MMI_TEXT_ID_E
{
	SEARCH_TXT_NULL = (MMI_MODULE_SEARCH << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "search_mdu_def.h"
#endif
    SEARCH_TXT_MAX
} SEARCH_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // SEARCH_MMI_TEXT_H_
