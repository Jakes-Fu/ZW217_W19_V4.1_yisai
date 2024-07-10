/*************************************************************************
 ** File Name:      FM_nv.c                                              *
 ** Author:         liqing.peng                                          *
 ** Date:           2006/05/17                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/05/17     liqing.peng      Create.                              *
*************************************************************************/
#define FM_NV_C_

#include "std_header.h"
#ifdef FM_SUPPORT
//#include "sci_types.h"
#include "mmifm_nv.h"
#include "mmi_modu_main.h"
#include "mmifm_export.h"
#include "mmifm_internal.h"
/*the length of fm nv*/
const uint16 fm_nv_len[] =
{
  1
};

/*****************************************************************************/
// 	Description : register fm module nv len and max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIFM_RegFMNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_FM, fm_nv_len, ((uint32)MMIFM_NV_LAST_ITEM - MMIFM_NV_FIRST_ITEM));
}

/*****************************************************************************/
//     Description : get sum size of all fm nv itme
//    Global resource dependence : 
//  Author: jian.ma
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFM_GetSumNvSize(void)
{
    uint32 fm_nv_sum_size = 0;    
    
    /* get items nv size */
    fm_nv_sum_size += sizeof(MMIFM_CUR_INFO_T);
    /* get all record items nv size */
    fm_nv_sum_size += MMIFM_MAX_CHANNEL_NUM*(sizeof(MMIFM_CHANNEL_T));

#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    fm_nv_sum_size += sizeof(MMIFM_TIMER_RECORD_INFO_T);
#endif

    return fm_nv_sum_size;
}
#endif

