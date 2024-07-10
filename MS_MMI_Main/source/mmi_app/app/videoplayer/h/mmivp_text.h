/*****************************************************************************
** File Name:      mmivp_text.h                                           *
** Author:                                                                   *
** Date:           2010-3-26                                                  *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef MMIVP_TEXT_H_
#define MMIVP_TEXT_H_

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
#include "videoplayer_mdu_def.h"
#endif
    
    typedef enum VP_MMI_TEXT_ID_E
    {
        VP_TXT_NULL = MMI_MODULE_VP << 16 | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "videoplayer_mdu_def.h"
#endif
            VP_TXT_MAX
    } VP_MMI_TEXT_ID_E;
    
#undef MACRO_MDU_TYPE
    
#ifdef __cplusplus
}
#endif

#endif // COMMON_MMI_TEXT_H_
