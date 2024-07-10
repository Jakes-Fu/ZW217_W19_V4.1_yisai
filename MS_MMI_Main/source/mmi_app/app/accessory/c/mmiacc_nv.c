/*************************************************************************
 ** File Name:      mmiacc_nv.c                                          *
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

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmiacc_nv.h"
#include "mmi_modu_main.h"

//#include "mmi_pccamera.h"

const uint16 acc_nv_len[] =
{
    sizeof(SCI_DATE_T) + sizeof(uint32)*3,//STEPS_DATA_INFO_T
#if defined WORLD_CLOCK_SUPPORT
    sizeof(WT_CITY_RECORD_INFO)*WT_DEFAULT_CITY_NUM,// s_default_city_info[WT_DEFAULT_CITY_NUM]
#endif
};

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIACC_RegNv(void)
{
//#if defined WORLD_CLOCK_SUPPORT
    MMI_RegModuleNv(MMI_MODULE_ACC, acc_nv_len, sizeof(acc_nv_len) / sizeof(uint16));
//#endif
}
