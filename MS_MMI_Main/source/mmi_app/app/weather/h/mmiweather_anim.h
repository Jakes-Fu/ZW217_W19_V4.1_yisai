/*****************************************************************************
** File Name:      mmiweather_anim.h                                         *
** Author:         Gaily.Wang                                                *
** Date:           04/2011                                                   *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe weather forecast            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef MMIWEATHER_ANIM_H_
#define MMIWEATHER_ANIM_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Animation Resource ID
///////////////////////////////////////////////////////////////////////////////

//anim_id

#define MACRO_MDU_TYPE  MACRO_MDU_ANIM
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "weather_mdu_def.h"
#endif

typedef enum MMIWEATHER_ANIM_ID_E
{
	MMIWEATHER_AMIN_NULL = (MMI_MODULE_WEATHER << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "weather_mdu_def.h"
#endif
	MMIWEATHER_AMIN_MAX
} MMIWEATHER_ANIM_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //MMIWEATHER_ANIM_H_
