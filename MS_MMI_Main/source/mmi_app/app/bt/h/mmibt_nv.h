/*************************************************************************
 ** File Name:      mmibt_nv.h                                           *
 ** Author:         kelly.li                                         *
 ** Date:           2006/08/21                                           *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/08/21     kelly.li      Create.                              *
*************************************************************************/

#ifndef MMIBT_NV_H_
#define MMIBT_NV_H_

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
typedef enum
{
    MMINV_BT_FILE_LOCATION = MMI_MODULE_BT << 16,
    MMINV_BT_SHAIRED_DIRECTORY,
    MMINV_BT_SHAIRED_AUTHORITY,
#ifdef WIN32
    BT_REF_INFO,
#endif
#ifdef UAL_BT_HFU_SUPPORT
    MMINV_BT_BOUND_ADDR,
#endif
    BT_MAX_NV_ITEM_NUM
}BT_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

