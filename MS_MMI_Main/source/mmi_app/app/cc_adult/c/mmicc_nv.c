/*************************************************************************
 ** File Name:      mmicc_nv.c                                          *
 ** Author:         bruce.chi                                           *
 ** Date:           2006/09/25                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/25     bruce.chi        Create.                              *
*************************************************************************/


#define _MMICC_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmicc_nv.h"
#include "mmi_modu_main.h"

#include "mmicc_internal.h"
#include "mmicl_internal.h"


/*the length of set nv*/
const uint16 cc_nv_len[] =
{
    //add the nv for cc 

    //missed call num
    sizeof(uint32),                 ////未接来电的数量

#if defined(MMI_SPEEDDIAL_SUPPORT)
    //Speed key
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_2,
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_3,
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_4,
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_5,
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_6,
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_7,
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_8,
    sizeof(CC_SPEEDKEY_ENTRY_T),    //MMI_CC_SPEEDKEY_9,
#endif
    (sizeof(CC_CALL_TIME_COUNT_T)*MMI_DUAL_SYS_MAX),         //MMINV_CL_LASTCALL_TIME,
    (sizeof(CC_CALL_TIME_COUNT_T)*MMI_DUAL_SYS_MAX),         //MMINV_CL_ALLCALLS_TIME,
    (sizeof(CC_CALL_TIME_COUNT_T)*MMI_DUAL_SYS_MAX),         //MMINV_CL_RECEIVEDCALLS_TIME,
    (sizeof(CC_CALL_TIME_COUNT_T)*MMI_DUAL_SYS_MAX),         //MMINV_CL_DIALEDCALLS_TIME,
    sizeof(MMICL_CALL_ARRAY_BCDINFO_HEADER_T),               //MMINV_CL_CALL_ALL_HEADER
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_CC, cc_nv_len, ARR_SIZE(cc_nv_len)+MMICL_RECORD_NVITEM_COUNT);
}

/*****************************************************************************/
// 	Description : cc nv size
//	Global resource dependence : none
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICC_GetNvSize(void)
{
    uint32 allcount = 0;
    uint16 i = 0;
    
    //sum cc_nv_len
    for (i = 0;i<ARR_SIZE(cc_nv_len);i++)
    {
        allcount += cc_nv_len[i];
    }

    //add MMICL_RECORD_NVITEM_COUNT
    allcount += sizeof (MMICL_CALL_ARRAY_BCDINFO_NVITEM_T)*MMICL_RECORD_NVITEM_COUNT;

    return allcount;
}


