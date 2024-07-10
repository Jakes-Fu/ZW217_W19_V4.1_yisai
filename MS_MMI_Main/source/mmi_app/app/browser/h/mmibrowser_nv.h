/*****************************************************************************
** File Name:      mmibrowser_nv.h                                           *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser nv              *
*****************************************************************************/
#ifndef MMIBROWSER_NV_H
#define MMIBROWSER_NV_H
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
    MMIBROWSER_SETTING = MMI_MODULE_MMIBROWSER<< 16,
    MMINV_BROWSER_IS_BOOKMARK_LOAD,
#ifdef BROWSER_SEARCH_SUPPORT
    MMINV_BROWSER_KEY,
    MMINV_BROWSER_SEARCH_ENGINE,
#endif

#ifdef MMIDORADO_MY_NAV_SUPPORT
    MMINV_BROWSER_MY_NAV_INFO,
#endif

#ifdef MMIDORADO_BOOKMARK_BY_NV
    MMINV_BROWSER_BOOKMARK_INFO,
#endif

    MMIBROWSER_NV_MAX
}MMIBROWSER_NV_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
