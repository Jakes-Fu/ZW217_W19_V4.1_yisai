/*****************************************************************************
** File Name:      mmiweather_menutable.h                                    *
** Author:         Gaily.Wang                                                *
** Date:           2011/04/18                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe weather app                 *
*****************************************************************************/

#ifndef MMIBROWSER_MENUTAB_H_
#define MMIBROWSER_MENUTAB_H_

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
    MMIWEATHER_MENU_ID_START = (MMI_MODULE_WEATHER<< 16),
    MMIWEATHER_MENU_ID_ADD_CITY,
    MMIWEATHER_MENU_ID_DEL_CITY,
    MMIWEATHER_MENU_ID_UPDATE_WEATHER,
    MMIWEATHER_MENU_ID_FORWARD_WEATHER,
    MMIWEATHER_MENU_ID_SETTING,
    MMIWEATHER_MENU_ID_PLUNGE_SETTING,
    MMIWEATHER_MENU_ID_ABOUT,
    MMIWEATHER_MENU_ID_SHORTCUT_SETTING,
    MMIWEATHER_MENU_ID_BG_TEXTCOLOUR_SETTING,
    
}MMIWEATHER_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	MMIWEATHER_MENU_LABLE_START = (MMI_MODULE_WEATHER<< 16),
	#include "mmiweather_menutable.def"
	MMIWEATHER_MENU_LABLE_MAX
} MMIWEATHER_MENU_LABEL_E;

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register weather module menu group
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
