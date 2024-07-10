/*****************************************************************************
** File Name:      ebook_menutable.c                                         *
** Author:         songbin.zeng                                              *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to handle ebook menutable               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/05/17     songbin.zeng      Creat                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef EBOOK_SUPPORT
#include "window_parse.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmieng_menutable.h"
#include "mmi_menutable.h"
#include "mmi_modu_main.h"
#include "mmiebook_menutable.h"
#include "mmiebook_id.h"
#include "mmiebook_text.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#ifdef TTS_SUPPORT
#include "mmiset_text.h"
#endif
#include "mmifmm_text.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
// begin of ebook,电子书,songbin,2006.06.16
#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_ebook_txt_list_opt[] =
{   
    //{ID_EBOOK_DETAIL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL, 0, 0, 0, MENU_NULL},
    {ID_EBOOK_CURRENT_BOOKS,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_EBOOK_LATEST_BOOKS, 0, 0, 0, MENU_NULL},	
    {ID_EBOOK_LIST_MAKEABLE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,  0, 0, 0, MENU_EBOOK_LIST_MARK_OPT},
    {ID_EBOOK_AUTO_READ,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_EBOOK_AUTO_READ, 0, 0, 0, MENU_NULL},
#ifdef IM_TRAD_CHINESE_SUPPORT
    {ID_EBOOK_CODING_TYPE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CODING_TYPE/*TXT_EBOOK_COLOR*/, 0, 0, 0, MENU_NULL},
#endif
#ifdef TTS_SUPPORT
    //{ID_EBOOK_SET_TTS,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_TTS_READ, 0, 0, 0, MENU_NULL},
    {ID_EBOOK_SET_TTS_OPEN_CLOSE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_TTS_AUTO_READ, 0, 0, 0, MENU_NULL},
#ifdef EBOOK_TTS_SELF_VOLUME
    {ID_EBOOK_SET_TTS_VOL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_TTS_READ_VOL, 0, 0, 0, MENU_NULL},
#endif
#endif
};
#else
const GUIMENU_ITEM_T menu_ebook_txt_list_opt[] =
{   
    {ID_EBOOK_DELETE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL},	
#ifdef BLUETOOTH_SUPPORT
    {ID_EBOOK_SENDBT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SEND_BY_BT,  0, 0, 0, MENU_NULL},
#endif
#ifdef MMS_SUPPORT
    {ID_EBOOK_SENDMMS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_MMS_SEND,  0, 0, 0, MENU_NULL},
#endif
    {ID_EBOOK_DETAIL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL, 0, 0, 0, MENU_NULL},
    {ID_EBOOK_RENAME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_RENAME, 0, 0, 0, MENU_NULL},
    {ID_EBOOK_COPY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_COPY, 0, 0, 0, MENU_NULL},
    {ID_EBOOK_ADVANCE_OPT,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_HIGHQUALITY, 0, 0, 0, MENU_EBOOK_ADVANCE_OPT},
};
#endif

const GUIMENU_ITEM_T menu_ebook_advance_opt[] =
{   
    {ID_EBOOK_CURRENT_BOOKS,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_EBOOK_LATEST_BOOKS, 0, 0, 0, MENU_NULL},
    {ID_EBOOK_LIST_MAKEABLE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,  0, 0, 0, MENU_EBOOK_LIST_MARK_OPT},
    {ID_EBOOK_AUTO_READ,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_EBOOK_AUTO_READ, 0, 0, 0, MENU_NULL},
#ifdef IM_TRAD_CHINESE_SUPPORT//BIG5_SUPPORT倚赖于该繁体中文宏开关
    {ID_EBOOK_CODING_TYPE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CODING_TYPE/*TXT_EBOOK_COLOR*/, 0, 0, 0, MENU_NULL},
#endif
#ifdef TTS_SUPPORT
    {ID_EBOOK_SET_TTS_OPEN_CLOSE,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_TTS_AUTO_READ,  0, 0, 0, MENU_NULL},
#ifdef EBOOK_TTS_SELF_VOLUME
    {ID_EBOOK_SET_TTS_VOL,          TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_SET_TTS_READ_VOL, 0, 0, 0, MENU_NULL},
#endif
#endif
};

const GUIMENU_ITEM_T menu_ebook_list_mark_opt[] =
{   
    {ID_EBOOK_LIST_MARK,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,  0, 0, 0, MENU_NULL},//TXT需修改
    {ID_EBOOK_LIST_CANCEL_MARK,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK, 0, 0, 0, MENU_NULL},
    {ID_EBOOK_LIST_MARKALL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MARK_ALL,  0, 0, 0, MENU_NULL},
    {ID_EBOOK_LIST_CANCEL_MARKALL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK_ALL, 0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T menu_ebook_bookmark_list_opt[] =
{
#ifndef MMI_PDA_SUPPORT
    {ID_EBOOK_READ,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_READ,  0, 0, 0, MENU_NULL},    
    {ID_EBOOK_DELETE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL},
#endif    
    {ID_EBOOK_DELALL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL, 0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T menu_ebook_latest_list_opt[] =
{
#ifndef MMIEBOOK_BOOKSHELF_SUPPORT
    {ID_EBOOK_DELETE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL},
#endif
    {ID_EBOOK_CLEAR,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_CLEAR, 0, 0, 0, MENU_NULL},//清空,需替换
};
// end of ebook


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiebook_menu_table[] = 
{
    {NULL},
    #include "mmiebook_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register ebook menu group
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_EBOOK, mmiebook_menu_table);
}
#endif//ENG_SUPPORT
