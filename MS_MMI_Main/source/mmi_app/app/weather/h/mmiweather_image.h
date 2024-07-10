/*****************************************************************************
** File Name:      mmiweather_image.h                                        *
** Author:                                                                   *
** Date:           2011/04/18                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe weather app                 *
******************************************************************************/

#ifndef MMIWEATHER_IMAGE_H_
#define MMIWEATHER_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "weather_mdu_def.h"
#endif

typedef enum MMIWEATHER_IMAGE_ID_E
{
	MMIWEATHER_IMAGE_NULL = ( MMI_MODULE_WEATHER << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "weather_mdu_def.h"
#endif
	MMIWEATHER_IMAGE_MAX_ID
} MMIWEATHER_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //MMIWEATHER_IMAGE_H_


