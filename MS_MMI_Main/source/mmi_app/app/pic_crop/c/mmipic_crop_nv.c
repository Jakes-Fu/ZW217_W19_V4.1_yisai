/*************************************************************************
 ** File Name:      mmipic_crop_nv.c                                          *
 ** Author:           chunyou                                           *
 ** Date:             12/25/2011                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 12/2011       chunyou         Create.                              *
*************************************************************************/
#define _MMIPIC_CROP_NV_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmipic_crop_nv.h"
#include "mmi_modu_main.h"
#include "mmipic_crop.h"

/*the length of record nv*/
const uint16 piccrop_nv_len[] =
{
    sizeof(MMIPICCROP_SETTING_INFO_T)    //MMINV_RECORD_STORAGE
};

/*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICCROP_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_PIC_CROP, piccrop_nv_len, sizeof(piccrop_nv_len) / sizeof(uint16));
}
