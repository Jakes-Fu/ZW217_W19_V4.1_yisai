/*****************************************************************************
** File Name:      browser_manager_anim.h                                            *
** Author:         fen.xie                                                    *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser anim resource   *
*****************************************************************************/
#ifndef BROWSER_MANAGER_ANIM_H_
#define BROWSER_MANAGER_ANIM_H_

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
#define MACRO_MDU_TYPE  MACRO_MDU_ANIM
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "browser_manager_mdu_def.h"
#endif

typedef enum BM_MMI_ANIM_ID_E
{
	BM_ANIM_NULL = ( MMI_MODULE_MMIBROWSER_MANAGER << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "browser_manager_mdu_def.h"
#endif
	BM_ANIM_MAX
} BM_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // BROWSER_ANIM_H_
