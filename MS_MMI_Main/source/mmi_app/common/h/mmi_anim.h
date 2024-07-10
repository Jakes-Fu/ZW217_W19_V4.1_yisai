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

#ifndef COMMON_MMI_ANIM_H_
#define COMMON_MMI_ANIM_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Animation Resource ID
///////////////////////////////////////////////////////////////////////////////

//anim_id
//#define RES_ADD_IMG(IMAGE_ID, param1, param2, param3)   //none
//#define RES_ADD_ANIM(ANIM_ID, param1, param2, param3, param4)   ANIM_ID,
//#define RES_ADD_STRING(TXT_ID, default_string)     //none
#define MACRO_MDU_TYPE  MACRO_MDU_ANIM
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "common_mdu_def.h"
#endif

typedef enum COMMON_MMI_ANIM_ID_E
{
	COMMON_IMAGE_AMIN_NULL = (MMI_MODULE_COMMON << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "common_mdu_def.h"
#endif
	COMMON_IMAGE_AMIN_MAX
} COMMON_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //COMMON_MMI_ANIM_H_
