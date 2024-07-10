/*****************************************************************************
** File Name:      mmisns_menutable.h                                    *
** Author:          CBK                                                   *
** Date:           31/12/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser menutable       *
*****************************************************************************/
#ifdef SNS_SUPPORT
#ifndef MMISNS_MENUTAB_H_
#define MMISNS_MENUTAB_H_

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
    ID_SNS_MENU_START = (MMI_MODULE_MMISNS<< 16),
    ID_SNS_MENU_ACCOUNT_TYPE
}ID_SNS_MENU_E;
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	MMI_SNS_BEGIN_MENU_START = (MMI_MODULE_MMISNS<< 16),
	#include "mmisns_menutable.def"
	MENUTABLE_MAX
} MMI_SNS_MENUTAB_E;
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
