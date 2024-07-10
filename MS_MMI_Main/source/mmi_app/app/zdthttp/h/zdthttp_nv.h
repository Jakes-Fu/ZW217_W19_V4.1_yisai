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

#ifndef _MMIZDTHTTP_NV_H_
#define _MMIZDTHTTP_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#include "mn_type.h"

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

typedef enum
{
        MMI_ZDTHTTP_NV_RUNMODE = (MMI_MODULE_ZDTHTTP << 16),
        MMI_ZDTHTTP_NV_NET_SYS,
#ifdef ZDT_HTTP_APP_SUPPORT
        MMI_ZDTHTTP_NV_HTTP_RCV_COUNT,
        MMI_ZDTHTTP_NV_HTTP_RCV_MONTH,
        MMI_ZDTHTTP_NV_HTTP_RCV_SWITCH,
#endif
        MMI_ZDTHTTP_NV_MAX_ID
}ZDTHTTP_NV_ITEM_E;



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

