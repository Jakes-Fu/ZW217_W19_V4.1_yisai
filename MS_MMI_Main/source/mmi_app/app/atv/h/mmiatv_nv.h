/******************************************************************************
**  File Name:      mmiatv_nv.h                                               *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef _MMIATV_NV_H_ /* compile limiting */
#define _MMIATV_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmiatv_data.h"
#include "mn_type.h"

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
    MMINV_ATV_PROG_ID = MMI_MODULE_ATV << 16,
    MMINV_ATV_SETTING_INFO,
    MMINV_ATV_MAX_NV_ITEM_NUM
}MMIATV_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description: register atv module nv len and max item
// Global resource dependence: none
// Author: xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_RegAtvNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
