/*************************************************************************
 ** File Name:      mmiasp_nv.h                                          *
 ** Author:         byte.guo                                             *
 ** Date:           2009/03/24                                           *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/03/24     byte.guo          Create.                             *
*************************************************************************/
#ifndef _MMIASP_NV_H_
#define _MMIASP_NV_H_

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
        MMINV_ASP_SETTING = MMI_MODULE_ASP<< 16,
		MMIST_MAX_NV_ITEM_NUM
}MMIASP_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMIASP_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

