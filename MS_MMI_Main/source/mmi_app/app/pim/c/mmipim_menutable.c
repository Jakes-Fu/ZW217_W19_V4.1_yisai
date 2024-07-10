/******************************************************************************
文件名称:mmipim_menutable.c
文件描述:PIM应用菜单定义实现。
文件说明:
开发人员:王晓林
首次开发日期:2007-9.24
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2007-09-24           王晓林                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/


#define _MMIPIM_SIGNAL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guimenu.h"
#include "mmieng_menutable.h"
#include "mmi_menutable.h"
#include "mmipim_menutable.h"
#include "mmipim_id.h"
#include "mmipim_text.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipim_text.h"
#include "mmipim.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

// begin of pim,号簿管家,2007.09
const GUIMENU_ITEM_T mmi_pim_main_menu[] =
{   
    {MMIPIM_MENU_ID_SERVICE_INDEX,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SERVICE_INDEX,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_PAY_INDEX,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_PAY_INDEX, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNC_ENTER,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_ENTER, 0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T mmi_pim_sync_enter_menu[] =
{   
    {MMIPIM_MENU_ID_SYNC_REGISTER,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_REGISTER_OR_CANCEL,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNC_BEGIN,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_START, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNC_PHONEBOOK,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_PHONEBOOK, 0, 0, 0, MENU_NULL},
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    {MMIPIM_MENU_ID_SYNC_BEGIN_VCAL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_START_VCAL, 0, 0, 0, MENU_NULL},
#endif /* HS_PIM_VCAL */
    {MMIPIM_MENU_ID_SYNC_NETQUERY,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_NET_QUERY,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNC_SETTING,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, STXT_MAIN_SETTING, 0, 0, 0, MENU_NULL},
};

// chenxiang 20071101 pim_setting begin
const GUIMENU_ITEM_T mmi_pim_setting_menu[] =
{
    {MMIPIM_MENU_ID_SYNC_TYPE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_TYPE,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_NET_SETTING,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_NET_SETTING,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_GATEWAY,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_GATEWAY, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_PROXY,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_PROXY,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNCML_SETTING,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNCML_SETTING, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_WAP_URL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_WAP_URL,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_RESET_SETTING,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_RESET_DEFAULT,  0, 0, 0, MENU_NULL},

    // chenxiang change this menu to "#*3456#"
    //{MMIPIM_MENU_ID_DEBUG_SETTING,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_DEBUG_SETTING,  0, 0, 0, MENU_NULL},

};

const GUIMENU_ITEM_T mmi_pim_net_setting_menu[] =
{   
    {MMIPIM_MENU_ID_NET_SETTING_APN,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_APN,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_NET_SETTING_USER,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_USERNAME, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_NET_SETTING_PASSWORD,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_PASSWORD,  0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T mmi_pim_gateway_menu[] =
{   
    {MMIPIM_MENU_ID_GATEWAY_IP,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_IP,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_GATEWAY_PORT,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_PORT, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_GATEWAY_USER,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_USERNAME,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_GATEWAY_PASSWORD,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_PASSWORD, 0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T mmi_pim_syncml_setting_menu[] =
{   
    {MMIPIM_MENU_ID_SYNCML_SETTING_SERVER,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SERVER_ADDR,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNCML_SETTING_PORT,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_PORT, 0, 0, 0, MENU_NULL}, // chenxiang 20080506 deleted port 
    {MMIPIM_MENU_ID_SYNCML_SETTING_USER,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_USERNAME,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNCML_SETTING_PASSWORD,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_PASSWORD, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNCML_SETTING_PB,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_CONTACT,  0, 0, 0, MENU_NULL},
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    {MMIPIM_MENU_ID_SYNCML_SETTING_VCAL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_CALENDAR,  0, 0, 0, MENU_NULL},
#endif /* HS_PIM_VCAL */
};

const GUIMENU_ITEM_T mmi_pim_wap_url_menu[] =
{   
    {MMIPIM_MENU_ID_WAP_URL_REGISTER_OR_CANCEL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_REGISTER_OR_CANCEL,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_WAP_URL_NET_QUERY,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_NET_QUERY, 0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_WAP_URL_PAY_INDEX,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_PAY_INDEX,  0, 0, 0, MENU_NULL},
};
// chenxiang 20071101 pim_setting end

const GUIMENU_ITEM_T mmi_pim_debug_setting_menu[] =
{   
    {MMIPIM_MENU_ID_SYNCML_SETTING_CODE,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_CODING_TYPE,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SYNCML_SETTING_AUTH,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_AUTH,  0, 0, 0, MENU_NULL},
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16 
    {MMIPIM_MENU_ID_SYNC_TYPE,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SYNC_TYPE,  0, 0, 0, MENU_NULL},
#endif /*  */
    {MMIPIM_MENU_ID_UA_SETTING,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_UA_SETTING,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_DEBUG_SERVER_SETTING,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_DEBUG_SERVER_SETTING,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_DELETE_ANCHOR,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_DELETE_ANCHOR,  0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T mmi_pim_debug_server_setting_menu[] =
{   
    {MMIPIM_MENU_ID_SERVER1,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SERVER1,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_SERVER2,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_SERVER2,  0, 0, 0, MENU_NULL},
};
const GUIMENU_ITEM_T mmi_pim_custom_ua_menu[] =
{   
    {MMIPIM_MENU_ID_MAN,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_CUSTOM_MAN,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_OEM,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_CUSTOM_OEM,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_MOD,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_CUSTOM_MOD,  0, 0, 0, MENU_NULL},
    {MMIPIM_MENU_ID_UA,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIM_CUSTOM_UA,  0, 0, 0, MENU_NULL},
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmipim_menu_table[] = 
{
    {NULL},
    #include "mmipim_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register PIM menu group
//	Global resource dependence : none
//  Author: 王晓林
//	Note:
/*****************************************************************************/
PUBLIC void MMIPIM_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_PIM, mmipim_menu_table);
}


