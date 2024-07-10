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

#ifndef WRE_MMI_ANIM_H_
#define WRE_MMI_ANIM_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Animation Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE   MACRO_MDU_ANIM
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "wre_boot_mdu_def.h"
#endif

typedef enum WRE_MMI_ANIM_ID_E
{
	WRE_IMAGE_AMIN_NULL = ( MMI_MODULE_WRE << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "wre_boot_mdu_def.h"
#endif
	WRE_IMAGE_AMIN_MAX
} WRE_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE



#define MACRO_MDU_TYPE MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "wre_boot_mdu_def.h"
#endif
typedef enum WRE_MMI_IMAGE_ID_E
{
	WRE_IMAGE_IMAGE_NULL = ( MMI_MODULE_WRE << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "wre_boot_mdu_def.h"
#endif
	WRE_IMAGE_IMAGE_MAX
} WRE_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE


#ifdef __cplusplus
}
#endif

#endif //WRE_MMI_ANIM_H_
