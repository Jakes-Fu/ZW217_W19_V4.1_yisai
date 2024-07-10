/*****************************************************************************
** File Name:      xxx_mmi_anim.h                                            *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef WLDT_MMI_ANIM_H_
#define WLDT_MMI_ANIM_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Animation Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_ANIM
#include "macro_mdu_def.h"

typedef enum WLDT_MMI_ANIM_ID_E
{
	WLDT_IMAGE_AMIN_NULL = ( MMI_MODULE_WLDT << 16 ) | MMI_SYS_RES_FLAG,
#include "wldt_mdu_def.h"
	WLDT_IMAGE_AMIN_MAX
} WLDT_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //CC_MMI_ANIM_H_
