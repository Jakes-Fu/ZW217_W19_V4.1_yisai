/*************************************************************************
 ** File Name:      mmiengtd_nv.h                                          *
 ** Author:         songbin.zeng                                           *
 ** Date:           2006/10/09                                          *
 ** Copyright:      2006 Spreadrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/10/09  songbin.zeng  Create.                             *
*************************************************************************/

#ifndef _MMIENGTD_NV_H_
#define _MMIENGTD_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

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
 
 
#define MMIENG_MAX_APN_LEN 102

typedef enum
{
    MMINV_ENG_THRESHOLD = MMI_MODULE_ENGTD<< 16,

    MMINV_ENG_PERIODS,//@shangke add
    MMINV_ENG_TIMER,//@shangke add
    MMINV_APN_SETTING,	//apn setting
    MMINV_VT_TYPE,
    MMIENGTD_NV_MAX_ITEM_ID
}MMIENGTD_NV_ITEM_ID_E;


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIENGTD_NV_H_

