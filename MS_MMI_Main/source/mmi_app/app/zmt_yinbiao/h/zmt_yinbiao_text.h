/*****************************************************************************
** File Name:      zmt_YINBIAO_text.h                                               *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#ifndef ZMT_YINBIAO_TEXT_H
#define ZMT_YINBIAO_TEXT_H

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
#include "zmt_yinbiao_mdu_def.h"
#endif

typedef enum ZMT_YINBIAO_MMI_TEXT_ID_E
{
	ZMT_YINBIAO_TXT_NULL = ( MMI_MODULE_ZMT_YINBIAO << 16 ) | MMI_SYS_RES_FLAG,

#include "zmt_yinbiao_mdu_def.h"

	ZMT_YINBIAO_TXT_MAX
} ZMT_YINBIAO_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif