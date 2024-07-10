/****************************************************************************
** File Name:      mmiota_menutable.c                                      *
** Author:         minghu.mao                                                 *
** Date:           08/10/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2009        minghu.mao          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "guimenu.h"
#include "mmi_text.h"
#include "mmieng_menutable.h"
#include "mmi_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#include "mmiota_menutable.h"
#include "mmimms_text.h"
#include "mmisms_text.h"
#include "mmiota_text.h"

#include "mmi_modu_main.h"

#ifdef MMI_OTA_SUPPORT
//mms setting config


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
const GUIMENU_ITEM_T menu_ota_config_set_option[] =
{
    {ID_OTA_CONFIG_SET_DELETE,TIP_NULL,{TXT_NULL,TXT_NULL,TXT_NULL},
        TXT_DELETE,0,0,0,MENU_NULL},        //删除
    {ID_OTA_CONFIG_SET_SAVE,TIP_NULL,{TXT_NULL,TXT_NULL,TXT_NULL},
    STXT_SAVE,0,0,0,MENU_NULL},          //保存
    {ID_OTA_CONFIG_SET_DETAIL,TIP_NULL,{TXT_NULL,TXT_NULL,TXT_NULL},
    STXT_DETAIL,0,0,0,MENU_NULL},        //详情
//     {ID_OTA_CONFIG_SET_CALL,TIP_NULL,{TXT_NULL,TXT_NULL,TXT_NULL},
//     TXT_CALL,0,0,0,MENU_NULL},
//    {ID_MMS_CALL_OPT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//    TXT_CALL,  0, 0, 0, MENU_MMS_CALL_OPT},  //呼叫
//     {ID_OTA_CONFIG_SET_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_ADDCONTACT,  0, 0, 0, MENU_NULL}, //保存至电话簿
//     {ID_OTA_CONFIG_SET_EXISTNUM,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, 
//     TXT_ADDTOPB_EXIST, 0, 0, 0, MENU_NULL},//加至已有联系人
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT 
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    {ID_PUSH_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE,  0, 0, 0, MENU_PUSH_SECURITY_SELECT},
#endif
#endif
};

const GUIMENU_ITEM_T menu_cmsbrw_pushmsg[] =
{
    {ID_CMSBRW_PUSHMSG_ENTER,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_COMMON_ENTER, 0, 0, 0, MENU_NULL},
        //  {ID_CMSBRW_PUSHMSG_DETAIL,      TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CMSBRW_DETAIL, 0, 0, 0, MENU_NULL},
    {ID_CMSBRW_PUSHMSG_DELETE,      TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL},
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT 
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    {ID_PUSH_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MOVE,  0, 0, 0, MENU_PUSH_SECURITY_SELECT},
#endif
#endif
    //  {ID_CMSBRW_PUSHMSG_DELETEALL,   TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CMSBRW_DELETEALL, 0, 0, 0, MENU_NULL},
};

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
const GUIMENU_ITEM_T menu_push_security_select[] =
{
    {ID_PUSH_MENU_BOX_SECURITY_ITEM, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS_SECURITY, 0, 0, 0, MENU_NULL},
    {ID_PUSH_MENU_BOX_PREVIEW_ITEM, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS_PREVIEW, 0, 0, 0, MENU_NULL}
};
#endif

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiota_menu_table[] = 
{
    {NULL},
    #include "mmiota_menutable.def"
};
#undef MENU_DEF

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : Register Input method menu group
//    Global resource dependence : none
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIOTA_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_OTA, mmiota_menu_table);
}
#endif
