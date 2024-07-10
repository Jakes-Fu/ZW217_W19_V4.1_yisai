
/*****************************************************************************
** File Name:      mmidcd_menutable.c    	                             *
** Author:                                                                   *
** Date:           2007/9/24												 *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    						             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/9/24      violent.wei      Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "guimenu.h"

#include "mmi_modu_main.h"
#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#ifdef CMCC_UI_STYLE	//!宏定义开头
#include "mmics_id.h"
#include "mmics_menutable.h"
#include "mmics_main.h"
#include "mmics_text.h"
#include "Mmiset_text.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#undef TIP_NULL
#define TIP_NULL 0


const GUIMENU_ITEM_T menu_cs[] =                       
{

    {ID_CS_PHONE_SERVICE_GUIDE,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_SERVICE_GUIDE,  0,      0, 0, MENU_NULL},
    //{ID_CS_E100,  TIP_NULL,   {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CS_E100,  0,      0, 0, MENU_NULL},
    {ID_CS_HOTLINE,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_HOT_LINE,  0,      0, 0, MENU_NULL},
    {ID_CS_SMS_OFFICE,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_SMS_OFFICE,  0,      0, 0, MENU_NULL},
    {ID_CS_POCKET_HALL,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_POCKET_BUSINESS_HALL,  0,      0, 0, MENU_NULL},
    {ID_CS_CUSTMER_MANAGER,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_CUSTMER_MANAGER,  0,      0, 0, MENU_NULL},
        //{ID_CS_MY_MONERNET,  TIP_NULL,   {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CS_MY_MONERNET,  0,      0, 0, MENU_NULL},
    {ID_CS_CUSTMER_SETTING,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_SERVICE_SETTING,  0,      0, 0, MENU_NULL},
    {ID_CS_ABOUT_E100,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_ABOUT_E100,  0,      0, 0, MENU_NULL},
	
};

const GUIMENU_ITEM_T menu_e100[] =                       
{

    {ID_CS_HOTLINE,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_HOT_LINE,  0,      0, 0, MENU_NULL},
    {ID_CS_SMS_OFFICE,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_SMS_OFFICE,  0,      0, 0, MENU_NULL},
    {ID_CS_ABOUT_E100,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_ABOUT_E100,  0,      0, 0, MENU_NULL},
	
};

const GUIMENU_ITEM_T menu_service_setting[] =                       
{

    {ID_CS_SERVICE_HOTLINE,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_SERVICE_HOTLINE,  0,      0, 0, MENU_NULL},
    {ID_CS_MANAGER_NUMBER,  TIP_NULL,   {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_CS_MANAGER_NUMBER,  0,      0, 0, MENU_NULL},
	
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T cs_menu_table[] = 
{
    {NULL},
    #include "mmics_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
//      Description : Register dcd menu group
//      Global resource dependence : none
//      Author: ryan.xu
//      Note:
/*****************************************************************************/
PUBLIC void MMICS_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_CMCS, cs_menu_table);
}
#endif /*#ifdef MMI_ENABLE_DCD*/



