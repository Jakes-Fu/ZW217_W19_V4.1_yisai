#ifndef ZMT_HANZI_TEXT_H
#define ZMT_HANZI_TEXT_H
/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           04/2009                                                   *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

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
#include "zmt_hanzi_mdu_def.h"
#endif

typedef enum ZMT_HANZI_MMI_TEXT_ID_E
{
	ZMT_HANZI_TXT_NULL = ( MMI_MODULE_ZMT_HANZI << 16 ) | MMI_SYS_RES_FLAG,

#include "zmt_hanzi_mdu_def.h"

	ZMT_HANZI_TXT_MAX
} ZMT_HANZI_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif