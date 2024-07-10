/*************************************************************************
 ** File Name:      mmirecord_nv.c                                       *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     haiyang.hu       Create.                              *
*************************************************************************/
#define _MMIRECORD_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmirecord_nv.h"
#include "mmi_modu_main.h"
#include "mmirecord_setting.h"
/*the length of record nv*/
const uint16 record_nv_len[] =
{
    sizeof(MMIRECORD_SETTING_INFO_T)    //MMINV_RECORD_STORAGE
};

/*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_RECORD, record_nv_len, sizeof(record_nv_len) / sizeof(uint16));
}