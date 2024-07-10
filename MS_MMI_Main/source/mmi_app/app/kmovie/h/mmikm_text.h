/*****************************************************************************
** File Name:      mmikm_text.h                                            *
** Author:                                                                 *
** Date:           14/07/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011         tonny.chen          Create                              *
**																		   *
****************************************************************************/

#ifndef _MMIKM_TEXT_H_
#define _MMIKM_TEXT_H_

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
#include "kmovie_mdu_def.h"
#endif

typedef enum MMI_KM_TEXT_ID_E
{
	KM_TXT_NULL = MMI_MODULE_KMOVIE << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "kmovie_mdu_def.h"
#endif
	KM_TXT_MAX
} MMI_KM_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // _MMIKM_TEXT_H_

