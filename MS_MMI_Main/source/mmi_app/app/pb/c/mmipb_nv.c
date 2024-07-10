/*************************************************************************
 ** File Name:      mmipb_nv.c                                          *
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

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmipb_nv.h"
//#include "mmipb_internal.h"
#include "mmi_modu_main.h"
#include "mmipb_export.h"



/*the length of set nv*/
const uint16 pb_nv_len[] =
{
	4,	//dummy, no use MMINV_PHONEBOOK_DISPLAY_MODE
        sizeof(uint32),// MMINV_PHONEBOOK_SAVE_POSTION,
        sizeof(uint32) ,//MMINV_PHONEBOOK_DISPLAY_OPTION,
        sizeof(uint32) ,//MMINV_PHONEBOOK_DISPLAY_PERIOD,
        sizeof(uint32) //MMINV_PHONEBOOK_UPDATE_PERIOD,
};


/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_RegNv(void)
{
    uint16 nv_max_item = ARR_SIZE(pb_nv_len);

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    nv_max_item += MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM;
#endif

    nv_max_item += MMINV_MAX_PHONEBOOK_RECORDS;
    
#ifdef MMIPB_MOST_USED_SUPPORT 
    nv_max_item += MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS;
#endif

    MMI_RegModuleNv(MMI_MODULE_PB, pb_nv_len, nv_max_item);    
}



/*****************************************************************************/
//     Description : get sum size of all pb nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetSumNvSize(void)
{
    //uint16 count = 0;
    uint32 pb_nv_sum_size = 0;    
    uint16 i = 0;
    
    //pb setting nv
    for (i = 0;i<ARR_SIZE(pb_nv_len);i++)
    {
        pb_nv_sum_size += pb_nv_len[i];
    }    
#ifdef MMIPB_GROUP_MANAGE_SUPPORT    /* get all group items nv size */
    pb_nv_sum_size += (MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM)*(sizeof(MMIPB_GROUP_T));
#endif
    /* get all record items nv size */
    pb_nv_sum_size += MMINV_MAX_PHONEBOOK_RECORDS*(sizeof(MMIPB_CONTACT_PHONE_T));
#ifdef MMIPB_MOST_USED_SUPPORT
    pb_nv_sum_size += MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS*(sizeof(MMIPB_CONTACT_MOSTUSED_T));
#endif
    return pb_nv_sum_size;
}
