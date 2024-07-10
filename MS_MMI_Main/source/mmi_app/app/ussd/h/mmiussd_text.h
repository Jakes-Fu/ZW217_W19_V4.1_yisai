/*****************************************************************************
** File Name:      mmiussd_text.h                                            *
** Author:                                                                   *
** Date:           01/2011                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe USSD                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef USSD_MMI_TEXT_H_
#define USSD_MMI_TEXT_H_

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
#include "USSD_mdu_def.h"
#endif

typedef enum USSD_MMI_TEXT_ID_E
{
	USSD_TXT_NULL = ( MMI_MODULE_USSD << 16 ) | MMI_SYS_RES_FLAG,

#ifndef CODE_MATCH_RES_CHECK
#include "ussd_mdu_def.h"
#endif

	USSD_TXT_MAX
} USSD_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // USSD_MMI_TEXT_H_

