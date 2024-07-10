/***************************************************************************
** File Name:      mmikm_menutable.c                                       *
** Author:                                                                 *
** Date:           07/14/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011         tonny.chen          Create                              *
**																		   *
****************************************************************************/
/**-------------------------------------------------------------------------*/
/**                         Include Files                                   */
/**-------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "window_parse.h"
//#include "guilcd.h"
//#include "mmidisplay_data.h"
//#include "guifont.h"
//#include "mmipub.h"
#include "guistring.h"
#include "guimenu.h"

#include "mmikm_menutable.h"
#include "mmikm_id.h"
#include "mmikm_text.h"
#include "mmikm_internal.h"
#include "mmikm_export.h"
#include "mmikm_playlist.h"
#include "mmikm_text.h"

#ifndef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T mmikm_menu_main_win_pop[] =
{
    {ID_KM_OPEN,        TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMM_OPEN,        NULL, 0, 0, NULL},
    {ID_KM_UPDATE_LIST, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_KM_UPDATE_LIST, NULL, 0, 0, NULL},
};

const GUIMENU_ITEM_T mmikm_disp_type_win_pop[] =
{
    {ID_PLAY,      TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_PLAY,        NULL, 0, 0, NULL},
    {ID_KM_MARK,           TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,           0, 0, 0, MMIKM_DISP_TYPE_MARK_OPT},
    {ID_DELETE,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, NULL, 0, 0, NULL},
    {ID_MORE_INFO, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_KM_MOVIE_INFO, NULL, 0, 0, NULL},
};
#else
const GUIMENU_ITEM_T mmikm_menu_main_win_pop[] =
{
    {ID_KM_UPDATE_LIST, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_KM_UPDATE_LIST, IMAGE_COMMON_OPTMENU_REFRESH, 0, 0, NULL},
};

const GUIMENU_ITEM_T mmikm_disp_type_win_pop[] =
{
    {ID_DELETE,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, NULL, 0, 0, NULL},
    {ID_MORE_INFO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_KM_MOVIE_INFO, NULL, 0, 0, NULL},
};

const GUIMENU_ITEM_T mmikm_disp_type_pda_popmenu[] =
{
    {ID_KM_MARK,           TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,           IMAGE_COMMON_OPTMENU_MARK, 0, 0, NULL},//MMIKM_DISP_TYPE_MARK_OPT},
};
#endif

const GUIMENU_ITEM_T mmikm_disp_type_mark_opt[] =
{
    {ID_KM_SUB_MARK,           TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MARK,           0, 0, 0, NULL},
    {ID_KM_CANCEL_MARK,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK,    0, 0, 0, NULL},
    {ID_KM_MARK_ALL,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_MARK_ALL,       0, 0, 0, NULL},
    {ID_KM_CANCEL_ALL_MARK,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CANCEL_MARK_ALL,0, 0, 0, NULL},
};
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T km_menu_table[] = 
{
    {NULL},
    #include "mmikm_menutable.def"
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
//  Description : register km menu group
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIKM_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_KMOVIE, km_menu_table);
}


