/*****************************************************************************
** File Name:      mmiautodemo_text.h                                            *
** Author:                                                                   *
** Date:           17/03/2012                                                   *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe auto demo                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef MMIAUTODEMO_TEXT_H_
#define MMIAUTODEMO_TEXT_H_

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

//#include "autodemo_mdu_def.h"

typedef enum AD_MMI_TEXT_ID_E
{
	AD_TXT_NULL = ( MMI_MODULE_AD << 16 ) | MMI_SYS_RES_FLAG,

    #include "autodemo_mdu_def.h"

	AD_TXT_MAX
} AD_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // AUTODEMO_MMI_TEXT_H_
