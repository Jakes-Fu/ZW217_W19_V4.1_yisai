/*****************************************************************************
** File Name:      mmivp_menutable.c                                        *
** Author:                                                                   *
** Date:           2010-3-26                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010-3         apple.zhang         Create                                  *
******************************************************************************/
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#define _MMIVP_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guimenu.h"
#include "mmivp_menutable.h"
#include "mmivp_text.h"
#include "mmivp_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmivp_text_id.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TIP_NULL    0

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifndef MMI_PDA_SUPPORT
//normal 
const GUIMENU_ITEM_T menu_vp_net_url_opt[] =
{
    {ID_VP_OPEN,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_OPEN,     		0, 0, 0, MENU_VP_OPEN_OPT},
    {ID_VP_VIEW,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VIEW,           0, 0, 0, MENU_VP_VIEW_OPT},
   // {ID_VP_SETTING,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SETTING,        0, 0, 0, NULL},
    {ID_VP_FILEATTR, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FILEATTR,      0, 0, 0, NULL},
    //{ID_VP_FLOWRATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FLOWRATE,      0, 0, 0, NULL}
};

const GUIMENU_ITEM_T menu_vp_setting_as_opt[] = 
{
#ifdef SCREENSAVER_SUPPORT
    {ID_VP_SCEENSAVE,   TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},    TXT_COMMON_SCREENSAVE,0, 0, 0, NULL},
#endif
    {ID_VP_POWER_ON_SET,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},  TXT_COMMON_POWER_ON_ANIMATION,   0, 0, 0, NULL},
    {ID_VP_POWER_OFF_SET, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},  TXT_COMMON_POWER_OFF_ANIMATION,  0, 0, 0, NULL},
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    {ID_VP_VIDEO_WALLPAPER,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},    TXT_VIDEO_WALLPAPER,0, 0, 0, NULL},
#endif
};

const GUIMENU_ITEM_T menu_vp_open_opt[] =
{
    {ID_VP_OPEN_LOCALFILE,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_LOCALFILE_VIEW,         0, 0, 0, NULL},
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    {ID_VP_OPEN_NET,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_NET_VIEW,           0, 0, 0, NULL},
#endif
    {ID_VP_OPEN_LOCALLIST,          TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_LOCALLIST,           0, 0, 0, NULL}
};

const GUIMENU_ITEM_T menu_vp_view_opt[] =
{
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    {ID_VP_VIEW_HISTORYLIST,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_HISTORYLIST_ID,         0, 0, 0, NULL},
#endif
	{ID_VP_VIEW_BOOKMARK,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_BOOKMARK_ID,           0, 0, 0, NULL}  
};

const GUIMENU_ITEM_T menu_vp_set_opt[]=
{
  //  {ID_VP_SET_REPEATE_MODE,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_REPEAT,         0, 0, 0, NULL},
   // {ID_VP_SET_PLAY_MODE,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PLAY_MODE_SETTING,           0, 0, 0, NULL},
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
     {ID_VP_SET_NET1,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM1,           0, 0, 0, NULL},
     {ID_VP_SET_NET2,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM2,           0, 0, 0, NULL},
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    {ID_VP_SET_NET3,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM3,           0, 0, 0, NULL},
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    {ID_VP_SET_NET4,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM4,           0, 0, 0, NULL},
#endif
#else
    {ID_VP_SET_NET,              TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_NETWORK_CONNECT_SET,           0, 0, 0, NULL},
#endif
	{ID_VP_SET_PLAYBACK_MODE,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SET_PLAY_METHOD,           0, 0, 0, NULL},
    {ID_VP_SET_RTCP_RTP,         TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SET_RTCP,           0, 0, 0, NULL},
    {ID_VP_SET_AGENT,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_PROXY,           0, 0, 0, NULL},
#ifdef BROWSER_SUPPORT  
     {ID_VP_FLOWRATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_FLOW_STATISTICS,      0, 0, 0, NULL},
#endif
#endif 
     {ID_VP_SET_RESET,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_RESET,           0, 0, 0, NULL},
    #ifdef MMI_VIDEOPLAYER_HELP_SUPPORT 
    {ID_VP_HELP,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_HELP,        0, 0, 0, NULL}
    #endif
};
#ifdef MMI_STREAMING_PLAYER_SUPPORT
const GUIMENU_ITEM_T menu_vp_bookmark_opt[] =
{
    {ID_VP_DELETE_CUR,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE,         0, 0, 0, NULL},
    {ID_VP_DELETE_ALL,          TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL,           0, 0, 0, NULL},
 //   {ID_VP_DELETE,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE,     		0, 0, 0, MENU_VP_DELETE_OPT},
    {ID_VP_OPERATER,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_OPERATE,     		0, 0, 0, MENU_VP_OPERATER_OPT},
    {ID_VP_OPEN,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_OPEN,     		0, 0, 0, MENU_VP_OPEN_OPT},
    {ID_VP_VIEW,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VIEW,           0, 0, 0, MENU_VP_VIEW_OPT},
 //   {ID_VP_SETTING,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SETTING,        0, 0, 0, NULL},
 //   {ID_VP_FLOWRATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FLOWRATE,      0, 0, 0, NULL},
    {ID_VP_FILEATTR,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FILEATTR,        0, 0, 0, NULL}
};
#endif //MMI_STREAMING_PLAYER_SUPPORT
const GUIMENU_ITEM_T menu_vp_operate_opt[] =
{
    {ID_VP_MODIFY,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MODIFY,         0, 0, 0, NULL},
    {ID_VP_ADD,          TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_ADD,           0, 0, 0, NULL}
};
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
const GUIMENU_ITEM_T menu_vp_history_opt[] =
{   
    {ID_VP_DELETE_CUR,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE,         0, 0, 0, NULL},
    {ID_VP_DELETE_ALL,          TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL,           0, 0, 0, NULL},
    {ID_VP_OPEN,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_OPEN,     		0, 0, 0, MENU_VP_OPEN_OPT},   
    {ID_VP_VIEW,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VIEW,           0, 0, 0, MENU_VP_VIEW_OPT},
   // {ID_VP_SETTING,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SETTING,        0, 0, 0, NULL},    
    {ID_VP_FILEATTR,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FILEATTR,        0, 0, 0, NULL},
  //  {ID_VP_FLOWRATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FLOWRATE,      0, 0, 0, NULL},
};
#endif
const GUIMENU_ITEM_T menu_vp_default_locallist_opt[] =
{  
    {ID_VP_OPEN,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_OPEN,     		0, 0, 0, MENU_VP_OPEN_OPT},
   // {ID_VP_ADD, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_ADD_VIDEO,        0, 0, 0, NULL},
  //  {ID_VP_UPDATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_UPDATE,        0, 0, 0, NULL},
  //  {ID_VP_DELETE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_DEL_LOCALLIST,        0, 0, 0, NULL},
    {ID_VP_SETTING_AS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SETTING_AS,        0, 0, 0, MENU_VP_SETTING_AS_OPT},
 //   {ID_VP_MARK_OPT, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,        0, 0, 0, MENU_VP_MARK_OPT},
  //  {ID_VP_SETTING,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SETTING,        0, 0, 0, NULL},    
    {ID_VP_FILEATTR,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL/*TXT_VP_FILEATTR*/,        0, 0, 0, NULL},
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
    {ID_VP_UPDATE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_UPDATE,   0, 0, 0, NULL},
#endif

};

const GUIMENU_ITEM_T menu_vp_user_locallist_opt[] =
{  
    {ID_VP_OPEN,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_OPEN,     		0, 0, 0, MENU_VP_OPEN_OPT},
    {ID_VP_ADD, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_ADD_VIDEO,        0, 0, 0, NULL},
    {ID_VP_UPDATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_UPDATE,        0, 0, 0, NULL},
    {ID_VP_DELETE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_DEL_LOCALLIST,        0, 0, 0, NULL},
    {ID_VP_SETTING_AS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SETTING_AS,        0, 0, 0, MENU_VP_SETTING_AS_OPT},
// #ifdef MMI_STREAMING_PLAYER_SUPPORT
    {ID_VP_MARK_OPT, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,        0, 0, 0, MENU_VP_MARK_OPT},
// #endif
  //  {ID_VP_SETTING,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SETTING,        0, 0, 0, NULL},    
    {ID_VP_FILEATTR,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL/*TXT_VP_FILEATTR*/,        0, 0, 0, NULL},
};
// #ifdef MMI_STREAMING_PLAYER_SUPPORT
const GUIMENU_ITEM_T menu_vp_mark_opt[] =
{
    {ID_VP_SUB_MARK,           TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,           0, 0, 0, NULL},
    {ID_VP_CANCEL_MARK,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK,    0, 0, 0, NULL},
    {ID_VP_MARK_ALL,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_MARK_ALL,       0, 0, 0, NULL},
    {ID_VP_CANCEL_ALL_MARK,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK_ALL,0, 0, 0, NULL}
};
//#endif
#else //PDA style menu

//normal 
const GUIMENU_ITEM_T menu_vp_net_url_opt[] =
{
    {ID_VP_OPEN,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_OPEN,   IMAGE_NULL, 0, 0, MENU_VP_OPEN_OPT},
    {ID_VP_VIEW,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VIEW,   IMAGE_NULL, 0, 0, MENU_VP_VIEW_OPT},
    {ID_VP_FILEATTR, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FILEATTR, IMAGE_NULL, 0, 0, NULL}
};

const GUIMENU_ITEM_T menu_vp_setting_as_opt[] = 
{
#ifdef SCREENSAVER_SUPPORT
    {ID_VP_SCEENSAVE,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},    TXT_VP_PDA_SCREENSAVE,  0, 0, 0, NULL},
#endif
    {ID_VP_POWER_ON_SET,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},  TXT_VP_PDA_POWER_ON_ANIMATION,   0, 0, 0, NULL},
    {ID_VP_POWER_OFF_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},  TXT_VP_PDA_POWER_OFF_ANIMATION,  0, 0, 0, NULL},
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    {ID_VP_VIDEO_WALLPAPER,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},    TXT_VP_PDA_VIDEO_WALLPAPER,0, 0, 0, NULL},
#endif
};

const GUIMENU_ITEM_T menu_vp_open_opt[] =
{
    {ID_VP_OPEN_LOCALFILE,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_LOCALFILE_VIEW,  0, 0, 0, NULL},
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    {ID_VP_OPEN_NET,           TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_NET_VIEW,        0, 0, 0, NULL},
#endif
    {ID_VP_OPEN_LOCALLIST,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_LOCALLIST,       0, 0, 0, NULL}
};

const GUIMENU_ITEM_T menu_vp_view_opt[] =
{
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    {ID_VP_VIEW_HISTORYLIST,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_HISTORYLIST_ID,   0, 0, 0, NULL},
#endif
	{ID_VP_VIEW_BOOKMARK,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_BOOKMARK_ID,      0, 0, 0, NULL}  
 };

const GUIMENU_ITEM_T menu_vp_set_opt[]=
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
     {ID_VP_SET_NET1,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM1,           0, 0, 0, NULL},
     {ID_VP_SET_NET2,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM2,           0, 0, 0, NULL},
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    {ID_VP_SET_NET3,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM3,           0, 0, 0, NULL},
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    {ID_VP_SET_NET4,              TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_NET_SIM4,           0, 0, 0, NULL},
#endif
#else
    {ID_VP_SET_NET,              TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_NETWORK_CONNECT_SET,           0, 0, 0, NULL},
#endif
	{ID_VP_SET_PLAYBACK_MODE,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SET_PLAY_METHOD,           0, 0, 0, NULL},
    {ID_VP_SET_RTCP_RTP,         TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_VP_SET_RTCP,           0, 0, 0, NULL},
    {ID_VP_SET_AGENT,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_PROXY,           0, 0, 0, NULL},
#ifdef BROWSER_SUPPORT  
     {ID_VP_FLOWRATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_FLOW_STATISTICS,      0, 0, 0, NULL},
#endif
#endif 
     {ID_VP_SET_RESET,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SET_RESET,           0, 0, 0, NULL},
    #ifdef MMI_VIDEOPLAYER_HELP_SUPPORT 
     {ID_VP_HELP,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_HELP,        0, 0, 0, NULL}
    #endif
};

const GUIMENU_ITEM_T menu_vp_bookmark_opt[] =
{
    {ID_VP_ADD,                 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_ADD, IMAGE_COMMON_OPTMENU_ADD, 0, 0, NULL},
    {ID_VP_DELETE_ALL,          TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, 0, 0, NULL}
};

const GUIMENU_ITEM_T menu_vp_operate_opt[] =
{
    {ID_VP_MODIFY,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MODIFY,         0, 0, 0, NULL},
    {ID_VP_ADD,          TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_ADD,           0, 0, 0, NULL}
};
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
const GUIMENU_ITEM_T menu_vp_history_opt[] =
{
    {ID_VP_DELETE_ALL,          TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL,  IMAGE_COMMON_OPTMENU_REMOVE_ALL, 0, 0, NULL}
};
#endif

#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
const GUIMENU_ITEM_T menu_vp_default_locallist_opt[] =
{
    {ID_VP_UPDATE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_UPDATE,   IMAGE_COMMON_OPTMENU_SEARCH, 0, 0, NULL}
};
#else
const GUIMENU_ITEM_T menu_vp_default_locallist_opt[] =
{  
    {ID_VP_OPEN,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_OPEN,     		0, 0, 0, MENU_VP_OPEN_OPT},
    {ID_VP_SETTING_AS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SETTING_AS,        0, 0, 0, MENU_VP_SETTING_AS_OPT},
    {ID_VP_FILEATTR,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL/*TXT_VP_FILEATTR*/,        0, 0, 0, NULL}
};
#endif




const GUIMENU_ITEM_T menu_vp_user_locallist_opt[] =
{
    {ID_VP_ADD, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_ADD_VIDEO,   IMAGE_VP_MENU_ADD_VIDEO, 0, 0, NULL},
    {ID_VP_UPDATE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_UPDATE,   IMAGE_COMMON_OPTMENU_SEARCH, 0, 0, NULL},
    {ID_VP_MARK_OPT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,     IMAGE_COMMON_OPTMENU_MARK, 0, 0, NULL}//NEWMS00125002

};

const GUIMENU_ITEM_T menu_vp_mark_opt[] =
{
    {ID_VP_SUB_MARK,           TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,           0, 0, 0, NULL},
    {ID_VP_CANCEL_MARK,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK,    0, 0, 0, NULL},
    {ID_VP_MARK_ALL,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_MARK_ALL,       0, 0, 0, NULL},
    {ID_VP_CANCEL_ALL_MARK,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK_ALL,0, 0, 0, NULL}
};

//new add for pda style
const GUIMENU_ITEM_T menu_vp_bookmark_longok_opt[] =
{
    {ID_VP_DELETE_CUR,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE,         0, 0, 0, NULL},
    {ID_VP_MODIFY,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MODIFY,         0, 0, 0, NULL},
    {ID_VP_FILEATTR,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FILEATTR,        0, 0, 0, NULL}
};
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
const GUIMENU_ITEM_T menu_vp_history_longok_opt[] =
{   
    {ID_VP_DELETE_CUR,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE,         0, 0, 0, NULL},
    {ID_VP_FILEATTR,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_FILEATTR,        0, 0, 0, NULL}
};
#endif
const GUIMENU_ITEM_T menu_vp_default_locallist_longok_opt[] =
{
#ifdef SCREENSAVER_SUPPORT
    {ID_VP_SCEENSAVE,    TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_PDA_SCREENSAVE,     0, 0, 0, NULL},
#endif
    {ID_VP_POWER_ON_SET,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},  TXT_VP_PDA_POWER_ON_ANIMATION,   0, 0, 0, NULL},
    {ID_VP_POWER_OFF_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},  TXT_VP_PDA_POWER_OFF_ANIMATION,  0, 0, 0, NULL},
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    {ID_VP_VIDEO_WALLPAPER,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},    TXT_VP_PDA_VIDEO_WALLPAPER,0, 0, 0, NULL},
#endif
    {ID_VP_FILEATTR,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL/*TXT_VP_FILEATTR*/,        0, 0, 0, NULL}
};

const GUIMENU_ITEM_T menu_vp_user_locallist_longok_opt[] =
{
    {ID_VP_DELETE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_DEL_LOCALLIST,        0, 0, 0, NULL},
#ifdef SCREENSAVER_SUPPORT
    {ID_VP_SCEENSAVE,    TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_VP_PDA_SCREENSAVE,     0, 0, 0, NULL},
#endif
    {ID_VP_POWER_ON_SET,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},  TXT_VP_PDA_POWER_ON_ANIMATION,   0, 0, 0, NULL},
    {ID_VP_POWER_OFF_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},  TXT_VP_PDA_POWER_OFF_ANIMATION,  0, 0, 0, NULL},
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    {ID_VP_VIDEO_WALLPAPER,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},    TXT_VP_PDA_VIDEO_WALLPAPER,0, 0, 0, NULL},
#endif
    {ID_VP_FILEATTR,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL/*TXT_VP_FILEATTR*/,        0, 0, 0, NULL}
};

#endif //MMI_PDA_SUPPORT

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T vp_menu_table[] = 
{
    {NULL},
    #include "mmivp_menutable.def"
};


#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
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
//  Description : register vp menu group
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIVP_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_VP, vp_menu_table);
}
#endif

