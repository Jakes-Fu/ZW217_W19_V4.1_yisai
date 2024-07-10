/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           03/2008                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef ZDTHTTP_MMI_TEXT_H_
#define ZDTHTTP_MMI_TEXT_H_

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

typedef enum ZDTHTTP_MMI_TEXT_ID_E
{
	ZDTHTTP_TXT_NULL = ( MMI_MODULE_ZDTHTTP << 16 ) | MMI_SYS_RES_FLAG,
        #include "zdthttp_mdu_def.h"
	ZDTHTTP_TXT_MAX
} ZDTHTTP_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // CC_MMI_TEXT_H_
