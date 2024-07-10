/******************************************************************************
**  File Name:      mmimtv_nv.c                                               *
**  Author:         xin.li                                                    *
**  Date:           2008/12                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:    This file defines the function about nv                   *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  12/2008       xin.li           Create.                                    *
******************************************************************************/
//#include "std_header.h"
#ifdef CMMB_SUPPORT
#ifndef _MMIMTV_NV_C
#define _MMIMTV_NV_C

#include "sci_types.h"
#include "mmimtv_nv.h"
#include "mmi_modu_main.h"
#include "mmimtv_export.h"
#include "mmimtv_main.h"
#ifdef MBBMS_SUPPORT
#include "mmimbbms_main.h"
#endif
/*the length of mtv nv*/
LOCAL uint16 mmimtv_nv_len[] =
{
    sizeof(MMIMTV_NV_CTL_T) * MMIMTV_NV_CTRL_TABLE_MAX_NUM,
    sizeof(MMIMTV_PROG_ID_T),
    sizeof(MMIMTV_SETTING_INFO_T),
#ifdef MBBMS_SUPPORT
    sizeof(MMIMBBMS_SETTING_INFO_T),
    sizeof(anyURI)
#endif
};

/*****************************************************************************/
// Description: register mtv module nv len and max item
// Global resource dependence: none
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_RegMtvNv(void)
{
    // actually, MMI_RegModuleNv() function shoulde be boolean return type,
    // we can't get any success or fail info from this interface! 
    MMI_RegModuleNv(MMI_MODULE_MTV, mmimtv_nv_len, sizeof(mmimtv_nv_len) / sizeof(uint16));
    return TRUE;
}

#endif /* _MMIMTV_NV_C */
#endif

