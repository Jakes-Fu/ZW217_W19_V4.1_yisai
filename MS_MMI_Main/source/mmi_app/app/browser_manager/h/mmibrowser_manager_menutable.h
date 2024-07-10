/*****************************************************************************
** File Name:      mmibrowser_manager_menutable.h                            *
** Author:          fen.xie                                                  *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser menutable       *
*****************************************************************************/
#ifndef MMIBROWSER_MANAGER_MENUTAB_H_
#define MMIBROWSER_MANAGER_MENUTAB_H_

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
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    ID_BM_MENU_START = (MMI_MODULE_MMIBROWSER_MANAGER<< 16),
        
    ID_BM_CC_VOICE,
    ID_BM_CC_IP,
    ID_BM_CC_VIDEO,

    ID_BM_MENU_TOTAL
}ID_BM_MENU_E;
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	MMI_BM_BEGIN_MENU_START = (MMI_MODULE_MMIBROWSER_MANAGER<< 16),
	#include "mmibrowser_manager_menutable.def"
	MMI_BM_BEGIN_MENU_MAX
} MMI_BM_MENUTAB_E;
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : reg menu
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
