/*****************************************************************************
** File Name:      mmiatv_nv.c                                               *
** Author:                                                                   *
** Date:           2010-05	                                                 *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe atv nv                      *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_modu_main.h"
#include "mmiatv_main.h"
#include "mmiatv_wintab.h"
#include "mmiatv_nv.h"

/*the length of atv nv*/
uint16 mmiatv_nv_len[] =
{
    sizeof(MMIATV_SAVE_PROG_T),
    sizeof(MMIATV_SETTING_INFO_T)
};

/*****************************************************************************/
// Description: register atv module nv len and max item
// Global resource dependence: none
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_RegAtvNv(void)
{
    // actually, MMI_RegModuleNv() function shoulde be boolean return type,
    // we can't get any success or fail info from this interface! 
    MMI_RegModuleNv(MMI_MODULE_ATV, mmiatv_nv_len, sizeof(mmiatv_nv_len) / sizeof(uint16));
    return TRUE;
}


