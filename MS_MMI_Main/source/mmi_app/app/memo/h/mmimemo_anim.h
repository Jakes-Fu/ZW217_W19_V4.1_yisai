/*****************************************************************************
** File Name:      browser_anim.h                                            *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser anim resource   *
*****************************************************************************/
#ifndef BROWSER_ANIM_H_
#define BROWSER_ANIM_H_

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
#include "memo_mdu_def.h"
#endif

typedef enum MEMO_MMI_ANIM_ID_E
{
	MEMO_ANIM_NULL = ( MMI_MODULE_MEMO << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "memo_mdu_def.h"
#endif
	MEMO_ANIM_MAX
} MEMO_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // BROWSER_ANIM_H_
