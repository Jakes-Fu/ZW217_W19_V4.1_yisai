/*************************************************************************
 ** File Name:      mmiweather_text.h                                    *
 ** Author:         Gaily.Wang                                           *
 ** Date:           2011/04/18                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the text id                        *
 *************************************************************************/

#ifndef MMIWEATHER_TEXT_H_
#define MMIWEATHER_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "weather_mdu_def.h"
#endif

typedef enum MMIWEATHER_TEXT_ID_E
{
	MMIWEATHER_TEXT_NULL = ( MMI_MODULE_WEATHER << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "weather_mdu_def.h"
#endif
	MMIWEATHER_TEXT_MAX
} MMIWEATHER_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // MMIWEATHER_TEXT_H_


