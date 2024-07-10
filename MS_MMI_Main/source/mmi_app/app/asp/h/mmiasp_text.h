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
#ifndef ASP_MMI_TEXT_H_
#define ASP_MMI_TEXT_H_

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
#include "asp_mdu_def.h"
#endif

typedef enum ASP_MMI_TEXT_ID_E
{
	ASP_TEXT_NULL = ( MMI_MODULE_ASP << 16 ) | MMI_SYS_RES_FLAG,    
#ifndef CODE_MATCH_RES_CHECK
#include "asp_mdu_def.h"
#endif
    ASP_TEXT_MAX_ID
}ASP_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif 
