/******************************************************************************
文件名称:mmipim_menutable.h
文件描述:PIM应用菜单定义头文件。
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

#ifndef _MMIPIM_MENUTABLE_H_
#define _MMIPIM_MENUTABLE_H_

#include "mmi_module.h"
#include "mmipim.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/


//Menu ID
typedef enum
{
    MMIPIM_MENU_ID_MIN = (MMI_MODULE_PIM<< 16),
    MMIPIM_MENU_ID_SERVICE_INDEX,
    MMIPIM_MENU_ID_PAY_INDEX,
    MMIPIM_MENU_ID_SYNC_ENTER,
    MMIPIM_MENU_ID_SYNC_REGISTER,
    MMIPIM_MENU_ID_SYNC_BEGIN,
    MMIPIM_MENU_ID_SYNC_PHONEBOOK,
    MMIPIM_MENU_ID_SYNC_NETQUERY,
    MMIPIM_MENU_ID_SYNC_SETTING,
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    MMIPIM_MENU_ID_SYNC_BEGIN_VCAL,
    MMIPIM_MENU_ID_SYNCML_SETTING_VCAL,
#endif /* HS_PIM_VCAL */

    // chenxiang 20071101 pim_setting begin
    MMIPIM_MENU_ID_NET_SETTING,
    MMIPIM_MENU_ID_GATEWAY,
    MMIPIM_MENU_ID_PROXY,
    MMIPIM_MENU_ID_SYNCML_SETTING,
    MMIPIM_MENU_ID_WAP_URL,
    MMIPIM_MENU_ID_RESET_SETTING,
    MMIPIM_MENU_ID_NET_SETTING_APN,
    MMIPIM_MENU_ID_NET_SETTING_USER,
    MMIPIM_MENU_ID_NET_SETTING_PASSWORD,
    MMIPIM_MENU_ID_GATEWAY_IP,
    MMIPIM_MENU_ID_GATEWAY_PORT,
    MMIPIM_MENU_ID_GATEWAY_USER,
    MMIPIM_MENU_ID_GATEWAY_PASSWORD,
    MMIPIM_MENU_ID_SYNCML_SETTING_SERVER,
    MMIPIM_MENU_ID_SYNCML_SETTING_PORT,
    MMIPIM_MENU_ID_SYNCML_SETTING_USER,
    MMIPIM_MENU_ID_SYNCML_SETTING_PASSWORD,
    MMIPIM_MENU_ID_SYNCML_SETTING_PB,
    MMIPIM_MENU_ID_SYNCML_SETTING_CODE,
    MMIPIM_MENU_ID_SYNCML_SETTING_AUTH,
    MMIPIM_MENU_ID_WAP_URL_REGISTER_OR_CANCEL,
    MMIPIM_MENU_ID_WAP_URL_NET_QUERY,
    MMIPIM_MENU_ID_WAP_URL_PAY_INDEX,
    // chenxiang 20071101 pim_setting end
    MMIPIM_MENU_ID_DEBUG_SETTING,
    MMIPIM_MENU_ID_UA_SETTING,
    MMIPIM_MENU_ID_DELETE_ANCHOR,
    MMIPIM_MENU_ID_DEBUG_SERVER_SETTING,
    MMIPIM_MENU_ID_SERVER1,
    MMIPIM_MENU_ID_SERVER2,
    MMIPIM_MENU_ID_USER1,
    MMIPIM_MENU_ID_USER2,
    MMIPIM_MENU_ID_USER3,
    MMIPIM_MENU_ID_USER4,
    MMIPIM_MENU_ID_SYNC_TYPE,
    MMIPIM_MENU_ID_MAN,
    MMIPIM_MENU_ID_OEM,
    MMIPIM_MENU_ID_MOD,
    MMIPIM_MENU_ID_UA,
    MMIPIM_MENU_ID_MAX
}MMIPIM_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIPIM_MENU_TABEL_ID_MIN = (MMI_MODULE_PIM<< 16),
    #include "mmipim_menutable.def"
    MMIPIM_MENU_TABLE_ID_MAX
} MMIPIM_MENU_TABEL_ID_E;

#undef MENU_DEF





/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* MENUTABLE_H */

