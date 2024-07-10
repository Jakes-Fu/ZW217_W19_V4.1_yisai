/*************************************************************************
 ** File Name:      mmicc_nv.h                                           *
 ** Author:         bruce.chi                                            *
 ** Date:           2006/09/25                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/25     bruce.chi        Create.                               *
*************************************************************************/

#ifndef _MMICC_NV_H_
#define _MMICC_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#include "mn_type.h"
#include "mmicl_internal.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct _CC_MTFW_ENTRY_T
{
    uint8	number_len;
    uint8	npi_ton;        						//type of number
    MNBCD 	number[PHONEBOOK_MAX_DN_NUMBER_LEN];	//the dialling number    
} CC_SPEEDKEY_ENTRY_T;

typedef enum
{
        //add the nvitem for CC

        //missed call num
        MMI_CC_MISSED_CALL_NUM = MMI_MODULE_CC << 16,

#if defined(MMI_SPEEDDIAL_SUPPORT)
        //speed key
        MMI_CC_SPEEDKEY_2, 
        MMI_CC_SPEEDKEY_3,
        MMI_CC_SPEEDKEY_4,
        MMI_CC_SPEEDKEY_5,
        MMI_CC_SPEEDKEY_6,
        MMI_CC_SPEEDKEY_7,
        MMI_CC_SPEEDKEY_8,
        MMI_CC_SPEEDKEY_9,
#endif
        MMINV_CL_LASTCALL_TIME,
        MMINV_CL_ALLCALLS_TIME,
        MMINV_CL_RECEIVEDCALLS_TIME,
        MMINV_CL_DIALEDCALLS_TIME,
        MMINV_CL_CALL_ALL_HEADER,
        MMINV_CL_CALL_ALL_CONTENT_BEGIN,
        MMINV_CL_CALL_ALL_CONTENT_END=MMINV_CL_CALL_ALL_CONTENT_BEGIN+MMICL_RECORD_NVITEM_COUNT,
}CC_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMICC_NV_H_

