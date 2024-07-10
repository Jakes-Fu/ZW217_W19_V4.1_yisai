/*****************************************************************************
** File Name:      xxx_mmi_color.h                                            *
** Author:                                                                   *
** Date:           03/2008                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef COMMON_MMI_COLOR_H_
#define COMMON_MMI_COLOR_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Color Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_COLOR
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "common_mdu_def.h"
#endif

typedef enum COMMON_MMI_COLOR_ID_E
{
	COMMON_COLOR_NULL = MMI_MODULE_COMMON << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "common_mdu_def.h"
#endif
	COMMON_COLOR_MAX
} COMMON_MMI_COLOR_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // COMMON_MMI_COLOR_H_
