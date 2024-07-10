/*****************************************************************************
** File Name:      xxx_mmi_text.h                                            *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool					              *
******************************************************************************/

#ifndef __MET_MEX_TEXT_H__
#define __MET_MEX_TEXT_H__

#include "sci_types.h"
#ifdef MET_MEX_SUPPORT
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif // __cplusplus

///////////////////////////////////////////////////////////////////////////////
// ÎÄ×Ö×ÊÔ´ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum MET_MEX_MMI_TEXT_ID_E
{
	MET_MEX_TXT_NULL = ( MMI_MODULE_MET_MEX << 16 ) | MMI_SYS_RES_FLAG,
#include "met_mex_mdu_def.h"
	MET_MEX_TXT_MAX
} MET_MEX_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MET_MEX_SUPPORT
#endif // __MET_MEX_TEXT_H__