/*****************************************************************************
** File Name:      xxx_mmi_rect.h                                            *
** Author:                                                                   *
** Date:           03/2008                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef COMMON_MMI_RECT_H_
#define COMMON_MMI_RECT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Rect Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_RECT
#include "macro_mdu_def.h"

typedef enum COMMON_MMI_RECT_ID_E
{
	COMMON_RECT_NULL = MMI_MODULE_COMMON << 16 | MMI_SYS_RES_FLAG,
#include "common_mdu_def.h"	
	COMMON_RECT_MAX
} COMMON_MMI_RECT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // COMMON_MMI_RECT_H_
