/*************************************************************************
 ** File Name:      mmipb_nv.h                                           *
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

#ifndef _MMIPB_NV_H_
#define _MMIPB_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "mmi_module.h"
//#include "mn_type.h"
//#include "mmi_signal_ext.h"
#include "mmipb_common.h"
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
// 手机中电话簿记录的最大数
#ifndef WIN32
#ifdef PB_SUPPORT_LOW_MEMORY
//LOW MEMORY Platform
#ifdef MMIPB_MAX_COUNT_100
    #define MMINV_MAX_PHONEBOOK_RECORDS     100
#else
    #define MMINV_MAX_PHONEBOOK_RECORDS     300
#endif
#elif NANDBOOT_SUPPORT
    #define MMINV_MAX_PHONEBOOK_RECORDS     1000
#else
    #define MMINV_MAX_PHONEBOOK_RECORDS     500
#endif
#else
    #define MMINV_MAX_PHONEBOOK_RECORDS     1000
 #endif

 //for cr NEWMS00172066
#ifdef CMCC_UI_STYLE

#ifdef MMINV_MAX_PHONEBOOK_RECORDS
#undef MMINV_MAX_PHONEBOOK_RECORDS
#endif

#define MMINV_MAX_PHONEBOOK_RECORDS     1500
#endif

// 手机中电话簿最大组个数
//#define MMINV_MAX_PHONEBOOK_GROUP_NUM	10
//#define MMINV_MAX_PHONEBOOK_GROUP_NUM    32//(27 + MMI_DUAL_SYS_MAX)
		
typedef enum
{
        MMINV_PHONEBOOK_DISPLAY_MODE = MMI_MODULE_PB << 16,
        MMINV_PHONEBOOK_SAVE_POSTION,
        MMINV_PHONEBOOK_DISPLAY_OPTION,
        MMINV_PHONEBOOK_DISPLAY_PERIOD,
        MMINV_PHONEBOOK_UPDATE_PERIOD,
       
#ifdef MMIPB_GROUP_MANAGE_SUPPORT 
        MMINV_PHONEBOOK_GROUP_0, 

        // 电话簿的组信息
        MMINV_PHONEBOOK_GROUP_MAX = MMINV_PHONEBOOK_GROUP_0 + MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM - 1,
#endif
        // 电话簿记录
        MMINV_PHONEBOOK_MAIN_FIRST_ENTRY,			// 
        MMINV_PHONEBOOK_MAIN_LAST_ENTRY = MMINV_PHONEBOOK_MAIN_FIRST_ENTRY + MMINV_MAX_PHONEBOOK_RECORDS - 1,
#ifdef MMIPB_MOST_USED_SUPPORT        
        // 常用联系人
        MMINV_PHONEBOOK_MOSTUSED_FIRST_ENTRY,			// 
        MMINV_PHONEBOOK_MOSTUSED_LAST_ENTRY = MMINV_PHONEBOOK_MOSTUSED_FIRST_ENTRY + MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS - 1
#endif
      
                
}PB_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMICC_NV_H_

