/*****************************************************************************
** File Name:      sns_anim.h                                            *
** Author:                                                        *
** Date:                                                       *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser anim resource   *
*****************************************************************************/
#ifdef SNS_SUPPORT
#ifndef SNS_ANIM_H_
#define SNS_ANIM_H_

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
#include "sns_mdu_def.h"
#endif

typedef enum BROWSER_MMI_ANIM_ID_E
{
	SNS_ANIM_NULL = ( MMI_MODULE_MMISNS << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "sns_mdu_def.h"
#endif
	SNS_ANIM_MAX
} SNS_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // SNS_ANIM_H_
#endif
