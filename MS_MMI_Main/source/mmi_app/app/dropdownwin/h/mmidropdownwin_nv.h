/*************************************************************************
 ** File Name:      mmidropdownwin_nv.h                                  *
 ** Author:                                                              *
 ** Date:           2011/07/01                                           *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2011/07/01     Paul.Huang        Create.                             *
*************************************************************************/

#ifndef _MMIDROPDOWNWIN_NV_H_
#define _MMIDROPDOWNWIN_NV_H_

#ifdef PDA_UI_DROPDOWN_WIN

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

#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
             MMI_WriteNVItem(_item_id, _addr)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_DROPDOWN_NOTIFY_INFO = MMI_MODULE_DROPDOWNWIN<< 16,

    MMIDROPDOWN_MAX_NV_ITEM_NUM
} MMIDROPDOWN_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIDROPDOWNWIN_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif //PDA_UI_DROPDOWN_WIN

#endif

