/*****************************************************************************
** File Name:      vt_menutable.c                                            *
** Author:                                                                   *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to handle fm menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/05/17     liqing.peng      Creat                                     *
******************************************************************************/

#define _VT_MENUTABLE_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef VT_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmieng_menutable.h"
#include "mmi_menutable.h"
#include "mmi_modu_main.h"
#include "mmivt_menutable.h"
#include "mmivt_id.h"
#include "mmivt_text.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

//@xiongxiaoyan 20060330 add begin
//call video menu of setting
const GUIMENU_ITEM_T menu_set_call_video_opt[] =
{//@zhaohui modified
    {ID_SET_VIDEO_FALLBACK, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SET_VT_FALLBACK_HANDLE, 0, 0, 0, MENU_NULL},//自动回退
    {ID_SET_REPLACE_PIC,TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_SET_FORCED_REPLACEPIC, 0, 0, 0, MENU_NULL}//无条件替代画面
};
/*
const GUIMENU_ITEM_T menu_set_call_video_opt2[] =
{
	{ID_SET_MY_RATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_SET_MY_RATE, 0, 0, 0, MENU_NULL},
	{ID_SET_REMOTE_RATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_SET_REMOTE_RATE, 0, 0, 0, MENU_NULL},
	{ID_SET_SWITCH_TO_NORMAL, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_VT_AUDIOMODE, 0, 0, 0, MENU_NULL}
};
*/
//@xiongxiaoyan 20060330 add end

//@zhaohui add begin
const GUIMENU_ITEM_T menu_set_call_video_replacepic_opt[] =
/*无条件替换画面菜单 :"缺省画面"，"选择图片"*/
{
	{ID_SET_DEFAULT_PIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SET_DEFAULTPIC, 0, 0, 0, MENU_NULL},
	{ID_SET_SELECT_PIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SET_SELECTPIC, 0, 0, 0, MENU_NULL}
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T vt_menu_table[] = 
{
    {NULL},
    #include "mmivt_menutable.def"
};



/*****************************************************************************/
// 	Description : Register fm menu group
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_VT, vt_menu_table);
}


#endif

