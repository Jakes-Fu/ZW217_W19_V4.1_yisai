/******************************************************************************
文件名称:mmipim_id.h
文件描述:PIM应用窗口ID定义头文件。
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

#ifndef _MMIPIM_ID_H_
#define _MMIPIM_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    MMI_PIM_WIN_ID_START = (MMI_MODULE_PIM << 16),

#include "mmipim_id.def"

    MMIPIM_WIN_ID_MAX
}MMIPIM_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIPIM_CTRL_ID_MIN = MMIPIM_WIN_ID_MAX, //(MMI_MODULE_PIM << 16),
    MMIPIM_MAINMENU_CTRL_ID,
    MMIPIM_SYNC_ENTER_MENU_CTRL_ID,
    MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID,
    MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID1,
    MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID2,
    MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3,
    MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID4,
    MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID5,
    MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6,
    MMIPIM_SYNC_PRGBOX_CTRL_ID,
    
    MMIPIM_SYNC_BEGIN_CTRL_ID,
     // chenxiang 20071101 pim_setting begin
    MMIPIM_SETTINGMENU_CTRL_ID,
    MMIPIM_NET_SETTING_MENU_CTRL_ID,
    MMIPIM_GATEWAY_MENU_CTRL_ID,
    MMIPIM_PROXY_RADIO_CTRL_ID,
    MMIPIM_SYNCML_SETTING_MENU_CTRL_ID,
    MMIPIM_WAP_URL_MENU_CTRL_ID,
    MMIPIM_SYNCML_SETTING_CODE_WIN_ID,
    MMIPIM_SYNCML_SETTING_AUTH_WIN_ID,
    MMIPIM_SYNCML_SETTING_CODE_RADIO_CTRL_ID,
    MMIPIM_SYNCML_SETTING_AUTH_RADIO_CTRL_ID,
     // chenxiang 20071101 pim_setting end
    MMIPIM_DEBUG_SETTING_MENU_CTRL_ID,
    //MS00216980 cheney 
    MMIPIM_SYNCML_SETTING_UA_RADIO_CTRL_ID,
    MMIPIM_DEBUG_SERVER_SETTING_RADIO_CTRL_ID,
    MMIPIM_SYNCML_SETTING_SYNC_TYPE_WIN_ID,
    MMIPIM_SYNCML_SETTING_SYNC_TYPE_RADIO_CTRL_ID,
    MMIPIM_DEBUG_SETTING_CUSTOM_UA_MENU_CTRL_ID,
    MMIPIM_DEBUG_SETTING_CUSTOM_UA_EDIT_CTRL_ID,
    MMIPIM_COM_SELECT_SIM_CTRL_ID,
    MMIPIM_CTRL_ID_MAX
}MMIPIM_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIPIM_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
