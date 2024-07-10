/*************************************************************************
 ** File Name:      mmipic_crop_menutable.c                                          *
 ** Author:           chunyou                                           *
 ** Date:             12/25/2011                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about crop menutable             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 12/2011       chunyou         Create.                              *
*************************************************************************/

#ifdef WIN32
#include "std_header.h"
#endif

#ifdef MMI_PIC_CROP_SUPPORT
#include "guimenu.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmipic_crop_text.h"
#include "mmipic_crop_menutable.h"
#include "mmipic_crop_image.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmipic_crop_menu_table[] = 
{
    {NULL},
    //#include "mmipic_crop_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
//  Description : Register pic menu group
//  Global resource dependence : none
//  Author: CHUNYOU
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICCROP_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_PIC_CROP, mmipic_crop_menu_table);
}

#ifdef   __cplusplus
    }
#endif
#endif
