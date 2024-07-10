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

#ifndef MP3_MMI_ANIM_H_
#define MP3_MMI_ANIM_H_

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
#include "audioplayer_mdu_def.h"
#endif

typedef enum MP3_MMI_ANIM_ID_E
{
    MP3_IMAGE_AMIN_NULL = MMI_MODULE_MP3 << 16,
#ifndef CODE_MATCH_RES_CHECK
#include "audioplayer_mdu_def.h"
#endif
    MP3_IMAGE_AMIN_MAX
} MP3_MMI_ANIM_ID_E;

#undef MACRO_MDU_TYPE


#endif  //#ifdef MMI_AUDIO_PLAYER_SUPPORT

#ifdef __cplusplus
}
#endif

#endif //MP3_MMI_ANIM_H_
