/*****************************************************************************
** File Name:      mmiwlan_text.h                                            *
** Author:         li.li                                                     *
** Date:           2009/11/18                                                *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe wifi                        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef MMIWLAN_TEXT_H_
#define MMIWLAN_TEXT_H_

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
#include "wifi_mdu_def.h"
#endif

typedef enum MMIWLAN_TEXT_ID_E
{
	MMIWLAN_TEXT_NULL = ( MMI_MODULE_WIFI << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "wifi_mdu_def.h"
#endif
	MMIWLAN_TEXT_MAX
} MMIWLAN_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // MMIWLAN_TEXT_H_
