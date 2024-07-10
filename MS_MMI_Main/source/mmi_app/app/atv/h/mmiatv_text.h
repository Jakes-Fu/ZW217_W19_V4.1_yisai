/******************************************************************************
**  File Name:      mmiatv_text.h                                             *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef ATV_MMI_TEXT_H_
#define ATV_MMI_TEXT_H_

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
#include "atv_mdu_def.h"
#endif

typedef enum ATV_MMI_TEXT_ID_E
{
	ATV_TXT_NULL = MMI_MODULE_ATV << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "atv_mdu_def.h"
#endif
	ATV_TXT_MAX
} ATV_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE


#ifdef KEYPAD_TYPE_COMPACT_KEYPAD
#define TXT_ATV_HELP_DETAIL_ID TXT_ATV_HELP_DETAIL_NO_KEY
#define TXT_ATV_HELP_ID TXT_HELP
#define TXT_ATV_BRIGHTNESS_ID TXT_BRIGHT_ADJUST
#else
#ifdef MMI_ATV_RECORD_SUPPORT
#define TXT_ATV_HELP_DETAIL_ID TXT_ATV_HELP_DETAIL
#else
#define TXT_ATV_HELP_DETAIL_ID TXT_ATV_HELP_DETAIL_NO_RECORD
#endif
#define TXT_ATV_HELP_ID TXT_ATV_HELP
#define TXT_ATV_BRIGHTNESS_ID TXT_COMMON_BRIGHTNESS_5
#endif


#ifdef __cplusplus
}
#endif

#endif // ATV_MMI_TEXT_H_
#endif


