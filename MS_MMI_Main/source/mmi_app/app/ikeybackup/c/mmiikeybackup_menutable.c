/****************************************************************************
** File Name:      mmiikeybackup_menutable.c                                *
** Author:         haiwu.chen                                               *
** Date:           12/25/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/25/2011     haiwu.chen        Create                                 *
****************************************************************************/
#ifdef IKEYBACKUP_SUPPORT


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "guimenu.h"
#include "mmiikeybackup_menutable.h"
#include "mmi_text.h"
#include "mmiikeybackup_id.h"


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
//#define TIP_NULL    0


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//list popmenu
const GUIMENU_ITEM_T menu_ikeybackup_list_opt[] ={0};

#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T mmiikb_restore_list_longok_opt_menu[] =
{
    {MMIIKB_ID_MENU_DELETE_PACK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, 
        TXT_DELETE, 0, 0, 0, MENU_NULL},
};
#endif

const GUIMENU_ITEM_T mmiikb_restore_list_opt_menu[] =
{
    {MMIIKB_ID_MENU_LEFT_OPT_RESTORE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, 
        TXT_IKB_RESTORE, 0, 0, 0, MENU_NULL},
    {MMIIKB_ID_MENU_LEFT_OPT_DELETE_PACK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, 
        TXT_DELETE, 0, 0, 0, MENU_NULL},
    {MMIIKB_ID_MENU_LEFT_OPT_DELETE_ALL_PACK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, 
        TXT_DELALL, 0, 0, 0, MENU_NULL},
};
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T ikeybackup_menu_table[] = 
{
    {NULL},
    #include "mmiikeybackup_menutable.def"
};
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINITION                        */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register 1-keybackup menu group
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_IKEYBACKUP, ikeybackup_menu_table);
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(IKEYBACKUP_SUPPORT)

