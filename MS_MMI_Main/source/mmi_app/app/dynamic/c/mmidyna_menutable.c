 /*****************************************************************************
** File Name:      mmidyna_menutable.c                                        *
** Author:                                                                   *
** Date:           09/24/2008                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2008        peng.chen          Create                                  *
******************************************************************************/

#define _MMIDYNA_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#ifdef DYNAMIC_MODULE_SUPPORT

#include "window_parse.h"
#include "guimenu.h"
#include "mmidyna_export.h.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidyna_text.h"
#include "mmi_text.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//normal 
#define TIP_NULL    0
const GUIMENU_ITEM_T menu_dyna_myapp_opt[] =
{
	{ID_DYNA_OPT_DETAIL, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN}, STXT_DETAIL,  0, 0, 0, NULL},//detail
	{ID_DYNA_OPT_DELAPP, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN}, TXT_DYNA_DEL_APP, 0, 0, 0, NULL},//del
	/*{ID_DYNA_OPT_ADDAPP, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN}, TXT_DYNA_ADD_APP, 0, 0, 0, NULL},//add*/
};


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T dyna_menu_table[] = 
{
    {NULL},
    #include "mmidyna_menutable.def"
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
// 	Description : register dyna menu group
//	Global resource dependence : none
//  Author: peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIDYNA_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_DYNA_APPLIST, dyna_menu_table);
}
#endif


