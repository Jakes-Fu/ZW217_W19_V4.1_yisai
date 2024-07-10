/*****************************************************************************
** File Name:      mmibrowser_manager_nv.h                                   *
** Author:          fen.xie                                                  *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser nv              *
*****************************************************************************/
#ifndef MMIBROWSER_MANAGER_NV_H
#define MMIBROWSER_MANAGER_NV_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
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
    MMINV_BM_DEFAULT_BROWSER = MMI_MODULE_MMIBROWSER_MANAGER<< 16,
    MMIBM_NV_MAX            
}MMIBM_NV_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBM_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
