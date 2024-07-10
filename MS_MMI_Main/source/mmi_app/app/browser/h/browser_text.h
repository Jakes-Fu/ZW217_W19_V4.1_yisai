/*****************************************************************************
** File Name:      browser_text.h                                            *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser text resource   *
*****************************************************************************/
#ifndef BROWSER_TEXT_H_
#define BROWSER_TEXT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "browser_mdu_def.h"
#endif

typedef enum BROWSER_MMI_TEXT_ID_E
{
	BROWSER_TXT_NULL = ( MMI_MODULE_MMIBROWSER << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "browser_mdu_def.h"
#endif
	BROWSER_TXT_MAX
} BROWSER_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // BROWSER_TEXT_H_
