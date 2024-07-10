/*****************************************************************************
** File Name:      mmipicview_menutable.c                                    *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#define _MMIPICVIEW_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32 
#include "std_header.h"
#endif

#include "guimenu.h"
#include "mmi_menutable.h"
#include "mmipicview_menutable.h"
#include "mmi_text.h"
#include "mmipicview_text.h"
#include "mmipicview_image.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//list popmenu
#ifndef PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
const GUIMENU_ITEM_T menu_picfolder_list_opt[] =
{
    //{ID_PICVIEW_CHANGE_FOLDER,  TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_CHANGE_FOLDER, IMAGE_PICVIEW_CHANGE_FOLDER, 0, 0, MENU_NULL},  
    //{ID_PICVIEW_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,          IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_PICVIEW_MARK},
    {ID_PICVIEW_SETTING,        TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_SETTING,     IMAGE_COMMON_OPTMENU_SET, 0, 0, MENU_NULL},
    //{ID_PICVIEW_VIEW,           TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_VIEW,          0, 0, 0, MENU_PICVIEW_VIEW},
};
#endif
const GUIMENU_ITEM_T menu_picview_list_opt[] =
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
#ifndef FLASH_SUPPORT
    {ID_PICVIEW_WALLPAPER,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_SETAS_WALLPAPER,     0, 0, 0, MENU_NULL},
#endif
    {ID_PICVIEW_POWER_ON_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_ON_ANIMATION,     0, 0, 0, MENU_NULL},
    {ID_PICVIEW_POWER_OFF_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_OFF_ANIMATION,     0, 0, 0, MENU_NULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {ID_PICVIEW_PB,      TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_PB,     0, 0, 0, MENU_NULL},
#endif
#endif
    {ID_PICVIEW_DELETE,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_DELETE,        0, 0, 0, MENU_NULL},
//    {ID_PICVIEW_CHANGE_FOLDER,  TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_CHANGE_FOLDER, 0, 0, 0, MENU_NULL},
#ifndef MMI_PICVIEW_MINI_SUPPORT
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)	    
    {ID_PICVIEW_SEND,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SHARE,          0, 0, 0, MENU_PICVIEW_SEND},
#endif
#endif
#ifdef PIC_EDITOR_SUPPORT
    {ID_PICVIEW_EDIT,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_EDIT,          0, 0, 0, MENU_NULL},
#endif 
    {ID_PICVIEW_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,          0, 0, 0, MENU_PICVIEW_MARK},
    {ID_PICVIEW_RENAME,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_RENAME,        0, 0, 0, MENU_NULL},
    {ID_PICVIEW_SLIDE_PREVIEW,  TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SLIDE_PREVIEW, 0, 0, 0, MENU_NULL},
    {ID_PICVIEW_DETAIL,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},STXT_DETAIL,        0, 0, 0, MENU_NULL},
//     {ID_PICVIEW_SLIDE_SET,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SLIDE_SET,     0, 0, 0, MENU_NULL},
//     {ID_PICVIEW_VIEW,           TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_VIEW,          0, 0, 0, MENU_PICVIEW_VIEW},
};

//send
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
const GUIMENU_ITEM_T menu_picview_send[] =
{
#ifdef MMS_SUPPORT
	{ID_PICVIEW_MMS,    TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_MMS,       0, 0, 0, MENU_NULL},
#endif
#ifdef BLUETOOTH_SUPPORT
    {ID_PICVIEW_BT,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_BLUETOOTH,        0, 0, 0, MENU_NULL},
#endif
#ifdef SNS_SUPPORT
#ifdef WRE_WEIBO_SUPPORT
    {ID_PICVIEW_SHARE_TO_SINA,        TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_SINA,       0, 0, 0, MENU_NULL}, 
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {ID_PICVIEW_SHARE_TO_FACEBOOK,        TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_FACEBOOK,       0, 0, 0, MENU_NULL},    
#endif
#ifdef WRE_TWITTER_SUPPORT
    {ID_PICVIEW_SHARE_TO_TIWTTER,        TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_TWITTER,       0, 0, 0, MENU_NULL},    
#endif    
#endif

};
#endif

// const GUIMENU_ITEM_T menu_picview_mark_opt[] =
// {
//     {ID_PICVIEW_DELETE,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_DELETE,        0, 0, 0, MENU_NULL},
// 	{ID_PICVIEW_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,              0, 0, 0, MENU_PICVIEW_MARK},
// };

//mark
const GUIMENU_ITEM_T menu_picview_mark[] =
{
	{ID_PICVIEW_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,              0, 0, 0, MENU_NULL},
    {ID_PICVIEW_UNMARKED,       TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK,       0, 0, 0, MENU_NULL},
    {ID_PICVIEW_MARKED_ALL,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_MARK_ALL,          0, 0, 0, MENU_NULL},
    {ID_PICVIEW_UNMARKED_ALL,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK_ALL,   0, 0, 0, MENU_NULL},
};

//view
const GUIMENU_ITEM_T menu_picview_view[] =
{
	{ID_PICVIEW_VIEW_LIST,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_LIST,     0, 0, 0, MENU_NULL},
    {ID_PICVIEW_VIEW_ICONLIST,  TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_VIEW_ICONLIST, 0, 0, 0, MENU_NULL},
};

//preview popmenu
const GUIMENU_ITEM_T menu_picview_preview_opt[] =
{
#ifndef FLASH_SUPPORT
    {ID_PICVIEW_WALLPAPER,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_SETAS_WALLPAPER,     0, 0, 0, MENU_NULL},
#endif
    {ID_PICVIEW_POWER_ON_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_ON_ANIMATION,     0, 0, 0, MENU_NULL},
    {ID_PICVIEW_POWER_OFF_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_OFF_ANIMATION,     0, 0, 0, MENU_NULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {ID_PICVIEW_PB,      TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_PB,     0, 0, 0, MENU_NULL},
#endif
    {ID_PICVIEW_DELETE,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_DELETE,        0, 0, 0, MENU_NULL},
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT)||defined(SNS_SUPPORT)
    {ID_PICVIEW_SEND,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SHARE,          0, 0, 0, MENU_PICVIEW_SEND},
#endif
#ifdef PIC_EDITOR_SUPPORT
    {ID_PICVIEW_EDIT,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_EDIT,          0, 0, 0, MENU_NULL},
#endif
    {ID_PICVIEW_RENAME,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_RENAME,        0, 0, 0, MENU_NULL},
    {ID_PICVIEW_SLIDE_PREVIEW,  TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SLIDE_PREVIEW, 0, 0, 0, MENU_NULL},
#ifdef TOUCHPANEL_TYPE_NONE//this is needed only when there's no touch panel
#ifdef PICVIEW_VIEW_ZOOM
    {ID_PICVIEW_ZOOM,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_ZOOM,          0, 0, 0, MENU_NULL},
#endif       
#endif
    {ID_PICVIEW_DETAIL,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},STXT_DETAIL,        0, 0, 0, MENU_NULL},
//     {ID_PICVIEW_SLIDE_SET,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SLIDE_SET,     0, 0, 0, MENU_NULL},
};

#else //PDA style menu
// const GUIMENU_ITEM_T menu_picfolder_list_opt[] =
// {
//     //{ID_PICVIEW_CHANGE_FOLDER,  TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_CHANGE_FOLDER, IMAGE_PICVIEW_CHANGE_FOLDER, 0, 0, MENU_NULL},  
//     //{ID_PICVIEW_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,          IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_PICVIEW_MARK},
//     {ID_PICVIEW_SETTING,        TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_SETTING,     IMAGE_COMMON_OPTMENU_SET, 0, 0, MENU_NULL},
//     //{ID_PICVIEW_VIEW,           TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_VIEW,          0, 0, 0, MENU_PICVIEW_VIEW},
// };

// const GUIMENU_ITEM_T menu_picview_list_opt[] =
// {
//     //{ID_PICVIEW_CHANGE_FOLDER,  TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_CHANGE_FOLDER, IMAGE_PICVIEW_CHANGE_FOLDER, 0, 0, MENU_NULL},  
//     {ID_PICVIEW_SLIDE_PREVIEW,  TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SLIDE_PREVIEW, IMAGE_PICVIEW_SLIDEPLAY_ICON, 0, 0, MENU_NULL},
//     {ID_PICVIEW_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,          IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_PICVIEW_MARK},
//     //{ID_PICVIEW_SETTING,        TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_SETTING,     IMAGE_COMMON_OPTMENU_SET, 0, 0, MENU_NULL},
//     //{ID_PICVIEW_VIEW,           TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_VIEW,          0, 0, 0, MENU_PICVIEW_VIEW},
// };

//send
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined (SNS_SUPPORT)
const GUIMENU_ITEM_T menu_picview_send[] =
{
#ifdef MMS_SUPPORT
	{ID_PICVIEW_MMS,    TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_MMS,       0, 0, 0, MENU_NULL},
#endif
#ifdef BLUETOOTH_SUPPORT
    {ID_PICVIEW_BT,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_BLUETOOTH,        0, 0, 0, MENU_NULL},
#endif
#ifdef SNS_SUPPORT
#ifdef WRE_WEIBO_SUPPORT
    {ID_PICVIEW_SHARE_TO_SINA,        TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_SINA,       0, 0, 0, MENU_NULL}, 
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {ID_PICVIEW_SHARE_TO_FACEBOOK,        TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_FACEBOOK,       0, 0, 0, MENU_NULL},    
#endif
#ifdef WRE_TWITTER_SUPPORT
    {ID_PICVIEW_SHARE_TO_TIWTTER,        TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_TWITTER,       0, 0, 0, MENU_NULL},    
#endif    

#endif
};
#endif

//mark
// const GUIMENU_ITEM_T menu_picview_mark[] =
// {
// 	{ID_PICVIEW_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,              0, 0, 0, MENU_NULL},
//     {ID_PICVIEW_UNMARKED,       TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK,       0, 0, 0, MENU_NULL},
//     {ID_PICVIEW_MARKED_ALL,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_MARK_ALL,          0, 0, 0, MENU_NULL},
//     {ID_PICVIEW_UNMARKED_ALL,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK_ALL,   0, 0, 0, MENU_NULL},
// };

//view
const GUIMENU_ITEM_T menu_picview_view[] =
{
	{ID_PICVIEW_VIEW_LIST,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_LIST,     0, 0, 0, MENU_NULL},
    {ID_PICVIEW_VIEW_ICONLIST,  TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_VIEW_ICONLIST, 0, 0, 0, MENU_NULL},
};

//preview popmenu
const GUIMENU_ITEM_T menu_picview_preview_opt[] =
{
    {ID_PICVIEW_WALLPAPER,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_SETAS_WALLPAPER,     IMAGE_COMMON_OPTMENU_SET_WALLPAPER, 0, 0, MENU_NULL},
    {ID_PICVIEW_POWER_ON_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_ON_ANIMATION,     0, 0, 0, MENU_NULL},
    {ID_PICVIEW_POWER_OFF_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_OFF_ANIMATION,     0, 0, 0, MENU_NULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {ID_PICVIEW_PB,      TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_PB,     0, 0, 0, MENU_NULL},
#endif
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined (SNS_SUPPORT)
    {ID_PICVIEW_SEND,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SHARE,          IMAGE_COMMON_OPTMENU_SEND, 0, 0, MENU_PICVIEW_SEND},
#endif
#ifdef PIC_EDITOR_SUPPORT
    {ID_PICVIEW_EDIT,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_EDIT,          IMAGE_PICVIEW_EDIT_IMAGE, 0, 0, MENU_NULL},
#endif
//    {ID_PICVIEW_DELETE,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_DELETE,        IMAGE_COMMON_OPTMENU_DELETE, 0, 0, MENU_NULL},
//    {ID_PICVIEW_SLIDE_PREVIEW,  TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SLIDE_PREVIEW, IMAGE_PICVIEW_SLIDEPLAY_ICON, 0, 0, MENU_NULL},
    {ID_PICVIEW_RENAME,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_RENAME,        0, 0, 0, MENU_NULL},//NEWMS00182068
    {ID_PICVIEW_DETAIL,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},STXT_DETAIL,        0, 0, 0, MENU_NULL},
    //{ID_PICVIEW_SETTING,        TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_SETTING,     0, 0, 0, MENU_NULL},
};

//new add for pda style
const GUIMENU_ITEM_T menu_picview_list_longok_opt[] =
{
    {ID_PICVIEW_WALLPAPER,      TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_SETAS_WALLPAPER,     0, 0, 0, MENU_NULL},
    {ID_PICVIEW_POWER_ON_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_ON_ANIMATION,     0, 0, 0, MENU_NULL},
    {ID_PICVIEW_POWER_OFF_SET,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_POWER_OFF_ANIMATION,     0, 0, 0, MENU_NULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT        
    {ID_PICVIEW_PB,      TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_PB,     0, 0, 0, MENU_NULL},
#endif
    {ID_PICVIEW_DELETE,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_DELETE,        0, 0, 0, MENU_NULL},
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)	    
    {ID_PICVIEW_SEND,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SHARE,          0, 0, 0, MENU_PICVIEW_SEND},
#endif 
#ifdef PIC_EDITOR_SUPPORT
    {ID_PICVIEW_EDIT,           TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_EDIT,          0, 0, 0, MENU_NULL},
#endif
    {ID_PICVIEW_SLIDE_PREVIEW,  TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_PICVIEW_SLIDE_PREVIEW, 0, 0, 0, MENU_NULL},
    {ID_PICVIEW_RENAME,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_RENAME,        0, 0, 0, MENU_NULL},//NEWMS00182068
    {ID_PICVIEW_DETAIL,         TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},STXT_DETAIL,        0, 0, 0, MENU_NULL},
};

#endif //MMI_PDA_SUPPORT


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T picview_menu_table[] = 
{
    {NULL},
    #include "mmipicview_menutable.def"
};

#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register picture viewer menu group
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_PIC_VIEWER, picview_menu_table);
}
#endif //#ifdef PIC_VIEWER_SUPPORT
