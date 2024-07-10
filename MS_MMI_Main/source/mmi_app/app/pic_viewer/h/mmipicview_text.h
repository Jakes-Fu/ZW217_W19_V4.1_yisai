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
#ifdef PIC_VIEWER_SUPPORT
#ifndef PIC_VIEWER_MMI_TEXT_H_
#define PIC_VIEWER_MMI_TEXT_H_

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
#include "pic_viewer_mdu_def.h"
#endif

typedef enum PIC_VIEWER_MMI_TEXT_ID_E
{
	PIC_VIEWER_TXT_NULL = (MMI_MODULE_PIC_VIEWER << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "pic_viewer_mdu_def.h"
#endif
	PIC_VIEWER_TXT_MAX
} PIC_VIEWER_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // PIC_VIEWER_MMI_TEXT_H_
#endif //#ifdef PIC_VIEWER_SUPPORT