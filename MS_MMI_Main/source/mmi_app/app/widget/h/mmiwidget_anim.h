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

#ifndef WIDGET_MMI_ANIM_H_
#define WIDGET_MMI_ANIM_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
///////////////////////////////////////////////////////////////////////////////
// Animation Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_ANIM
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "widget_mdu_def.h"
#endif

typedef enum WIDGET_MMI_ANIM_ID_E
{
    WIDGET_IMAGE_AMIN_NULL = (MMI_MODULE_WIDGET << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "widget_mdu_def.h"
#endif
    WIDGET_IMAGE_AMIN_MAX
} WIDGET_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE


#endif  //#ifdef MMI_AUDIO_PLAYER_SUPPORT

#ifdef __cplusplus
}
#endif

#endif //MP3_MMI_ANIM_H_

