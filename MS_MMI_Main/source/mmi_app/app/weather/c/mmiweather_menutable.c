/*****************************************************************************
** File Name:      mmiweather_menutable.c                                    *
** Author:         Gaily.Wang                                                *
** Date:           2011/04/18                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe weather app                 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_menutable.h"
#include "mmi_mainmenu_export.h"
#include "mmi_modu_main.h"
#include "mmiweather_text.h"
#include "mmiweather_menutable.h"
#include "mmiweather_text.h"
#include "mmiwidget_text.h"
#include "mmiset_text.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


const GUIMENU_ITEM_T  mmiweather_plunge_setting_opt[] =
{
#if defined(MMI_PDA_SUPPORT)
    {MMIWEATHER_MENU_ID_SHORTCUT_SETTING,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_IDLE_SHORTCUT,            IMAGE_COMMON_OPTMENU_SET, 0,0,MENU_NULL},
	{MMIWEATHER_MENU_ID_BG_TEXTCOLOUR_SETTING,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WIDGET_WEATHER_BG_TEXTCOLOUR_SETTING,       IMAGE_COMMON_OPTMENU_SET_WALLPAPER, 0,0,MENU_NULL},   
#else
    {MMIWEATHER_MENU_ID_SHORTCUT_SETTING,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WIDGET_WEATHER_SHORTCUT_SETTING,            0, 0,0,MENU_NULL},
	{MMIWEATHER_MENU_ID_BG_TEXTCOLOUR_SETTING,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WIDGET_WEATHER_BG_TEXTCOLOUR_SETTING,       0, 0,0,MENU_NULL},   
#endif
};

const GUIMENU_ITEM_T  mmiweather_mainmenu_opt[] =
{
#if defined(MMI_PDA_SUPPORT)
    {MMIWEATHER_MENU_ID_ADD_CITY,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_ADD_CITY,           IMAGE_COMMON_OPTMENU_ADD, 0,0,MENU_NULL},
	{MMIWEATHER_MENU_ID_DEL_CITY,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_DEL_CITY,           IMAGE_COMMON_OPTMENU_DELETE, 0,0,MENU_NULL},
    {MMIWEATHER_MENU_ID_UPDATE_WEATHER,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_UPDATE_WEATHER,     IMAGE_COMMON_OPTMENU_REFRESH, 0,0,MENU_NULL},
    {MMIWEATHER_MENU_ID_FORWARD_WEATHER,    TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_FORWARD_WEATHER,    IMAGE_COMMON_OPTMENU_TRANSMIT, 0,0,MENU_NULL},
    {MMIWEATHER_MENU_ID_SETTING,            TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_MAIN_SETTING,              IMAGE_COMMON_OPTMENU_SET, 0,0,MENU_NULL},
#ifdef MMI_WEATHER_JSON_PARSE
    {MMIWEATHER_MENU_ID_ABOUT,              TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},     TXT_WEATHER_ABOUT,              IMAGE_COMMON_OPTMENU_ABOUT, 0,0,MENU_NULL},
#endif
#ifdef MMI_WIDGET_WEATHER1
    {MMIWEATHER_MENU_ID_PLUNGE_SETTING,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_PLUNGE_SETTING,     IMAGE_COMMON_OPTMENU_MARK, 0,0,MMIWEATHER_PLUNGE_SETTING_OPT},
#endif
#else
    {MMIWEATHER_MENU_ID_ADD_CITY,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_ADD_CITY,           0, 0,0,MENU_NULL},
	{MMIWEATHER_MENU_ID_DEL_CITY,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_DEL_CITY,           0, 0,0,MENU_NULL},
    {MMIWEATHER_MENU_ID_UPDATE_WEATHER,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_UPDATE_WEATHER,     0, 0,0,MENU_NULL},
    {MMIWEATHER_MENU_ID_FORWARD_WEATHER,    TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_FORWARD_WEATHER,    0, 0,0,MENU_NULL},
    {MMIWEATHER_MENU_ID_SETTING,            TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_MAIN_SETTING,              0, 0,0,MENU_NULL},
#ifdef MMI_WEATHER_JSON_PARSE
    {MMIWEATHER_MENU_ID_ABOUT,              TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},     TXT_WEATHER_ABOUT,               0, 0,0,MENU_NULL},
#endif
#ifdef MMI_WIDGET_WEATHER1
    {MMIWEATHER_MENU_ID_PLUNGE_SETTING,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WEATHER_PLUNGE_SETTING,     0, 0,0,MMIWEATHER_PLUNGE_SETTING_OPT},
#endif
#endif
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T weather_menu_table[] = 
{
    {0},   
    #include "mmiweather_menutable.def"
};

#undef MENU_DEF



/*****************************************************************************/
// 	Description : Register weather menu group
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_WEATHER, weather_menu_table);
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

