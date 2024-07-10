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

#ifndef _MMIBEACON_NV_H_
#define _MMIBEACON_NV_H_

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
#define  BEACON_MAC_MAX_LEN 20
typedef struct _BEACON_MAC_ADDR
{
    char addr[BEACON_MAC_MAX_LEN];
} BEACON_MAC_ADDR;


typedef enum
{
        //add the nvitem for CDMA
        MMI_BEACON_NV_RUNMODE = (MMI_MODULE_BEACON << 16),
        MMI_BEACON_NV_MAC,
        MMI_BEACON_NV_MAX_ID
}BEACON_NV_ITEM_E;


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

