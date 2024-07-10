/***************************************************************************
** File Name:      mmidl_image.h                                             *
** Author:                                                                 *
** Date:           28/07/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to download image define        *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2009        wancan.you    Create
****************************************************************************/
#ifndef _MMIDL_IMAGE_H_
#define _MMIDL_IMAGE_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "dl_mdu_def.h"
#endif

typedef enum MMIDL_IMAGE_ID_E
{
    MMIDL_IMAGE_NULL = ( MMI_MODULE_DL << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "dl_mdu_def.h"
#endif
    MMIDL_IMAGE_MAX
} MMIDL_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // _MMIDL_IMAGE_H_
