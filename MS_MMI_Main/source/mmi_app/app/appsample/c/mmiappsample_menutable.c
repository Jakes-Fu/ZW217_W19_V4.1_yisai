/*****************************************************************************
 ** File Name:   mmiappsample_menutable.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe menutable
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/

#define _MMIAPPSAMPLE_MENUTABLE_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
//#include "sci_types.h"
#include "mmi_module.h"
#include "guimenu.h"

#include "mmi_menutable.h"

#include "mmi_text.h"
#include "mmiappsample_text.h"
#include "mmiappsample_id.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiappsample_menutable.h"



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


const GUIMENU_ITEM_T menu_appsample_popup_menu[] =
{
    {ID_APPSAMPLE_WINDATA, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_APPSAMPLE_FUNCWINLIST, 0, 0, 0, MENU_NULL},

    {ID_APPSAMPLE_EVENT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_APPSAMPLE_EVENT, 0, 0, 0, MENU_NULL},
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
                 _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT,

const GUIMENU_GROUP_T mmiappsample_menu_table[] =
{
    {NULL},
#include "mmiappsample_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
//  Description : Register APPSAMPLE menu group
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPPSAMPLE_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_APPSAMPLE, mmiappsample_menu_table);
}

