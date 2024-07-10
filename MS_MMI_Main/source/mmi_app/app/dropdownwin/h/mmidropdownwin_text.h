/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           07/2011                                                   *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe dropdownwin                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef DROPDOWNWIN_MMI_TEXT_H_
#define DROPDOWNWIN_MMI_TEXT_H_

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
#include "dropdownwin_mdu_def.h"
#endif

typedef enum DROPDOWNWIN_MMI_TEXT_ID_E
{
        DROPDOWNWIN_TXT_NULL = ( MMI_MODULE_DROPDOWNWIN << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "dropdownwin_mdu_def.h"
#endif
        DROPDOWNWIN_TXT_MAX
} DROPDOWNWIN_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // DROPDOWNWIN_MMI_TEXT_H_

