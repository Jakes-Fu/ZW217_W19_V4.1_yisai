/******************************************************************************
 ** File Name:      mmi_app_connection_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:162
#ifndef _MMI_APP_CONNECTION_TRC_H_
#define _MMI_APP_CONNECTION_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMICONN_MANAGER_WINTAB_1626_112_2_18_2_12_8_0 "SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x"
#define MMICONN_MANAGER_WINTAB_1742_112_2_18_2_12_9_1 "[MMICONNECTION]: HandleLinkListLongOKMenuWinMsg, msg_id = 0x%x"
#define MMICONN_MANAGER_WINTAB_1866_112_2_18_2_12_9_2 "mmiconnection_wintab.c, HandleConnectionMenuWinMsg(), msg_id = %x"
#define MMICONN_MANAGER_WINTAB_2010_112_2_18_2_12_9_3 "mmiconnection_wintab.c, HandleConnectionMenuWinMsg(), msg_id = %x"
#define MMICONN_MANAGER_WINTAB_2294_112_2_18_2_12_10_4 "[MMICONNECTION]:HandleSettingListWinMsg() msg_id = %d"
#define MMICONN_MANAGER_WINTAB_2841_112_2_18_2_12_11_5 "MMIAPICONNECTION_GetGPRSStatus gprs_settings=%d"
#define MMICONN_MANAGER_WINTAB_3336_112_2_18_2_12_12_6 "setting_detail is null"
#define MMICONN_MANAGER_WINTAB_4987_112_2_18_2_12_15_7 "MMIAPICONNECTION_ChangeSettingListLanguage"
#define MMICONN_MANAGER_WINTAB_5026_112_2_18_2_12_16_8 "MMICONNECTION_ResetSettingList is_clean_user_data=%d, settinglink_num=%d"
#define MMICONN_MANAGER_WINTAB_5123_112_2_18_2_12_16_9 "MMICONNECTION_ResetSettingListEx is_clean_user_data=%d, settinglink_num=%d"
#define MMICONN_MANAGER_WINTAB_5398_112_2_18_2_12_16_10 "[MMICONNECTION]:MMIAPICONNECTION_OpenSettingListWin sim_ok=%d"
#define MMICONN_MANAGER_WINTAB_5440_112_2_18_2_12_16_11 "HandleSIMPlugNotifyConnectionFunc dual_sys=%d, notify_event=%d"
#define MMICONN_MANAGER_WINTAB_5476_112_2_18_2_12_16_12 "[MMICONNECTION]:MMICONNECTION_Init setting read Failed"
#define MMICONN_MANAGER_WINTAB_5786_112_2_18_2_12_17_13 "AutoAdapting dual_sys=%d, account_type=%d, is_need_clean=%d"
#define MMICONN_MANAGER_WINTAB_5808_112_2_18_2_12_17_14 "AutoAdapting hplmn=%d, conection_plmn_name=%d"
#define MMICONN_MANAGER_WINTAB_5882_112_2_18_2_12_17_15 "AutoAdapting is_need_add_default=%d"
#define MMICONN_MANAGER_WINTAB_6086_112_2_18_2_12_18_16 "[MMICONNECTION]:MMICONNECTION_GetAccountDefaultIndex dual_sys=%d,account_type=%d"
#define MMICONN_MANAGER_WINTAB_6099_112_2_18_2_12_18_17 "[MMICONNECTION]:MMICONNECTION_GetAccountDefaultIndex setting read Failed"
#define MMICONN_MANAGER_WINTAB_6374_112_2_18_2_12_18_18 "ParseNetworkAccountInfo is_res_data_ok=%d, account_num=%d"
#define MMICONN_MANAGER_WINTAB_6454_112_2_18_2_12_18_19 "LoadNetworkAccountInfoFromRes is_res_data_exist=%d, account_num=%d"
#define MMICONN_MANAGER_WINTAB_6732_112_2_18_2_12_19_20 "MMIAUT::GetCurSIMNoProxyAccountIndex  item_num ||detail_item is NULL"
#define MMICONN_MANAGER_WINTAB_6763_112_2_18_2_12_19_21 "MMIAUT::SearchDirectLinkAccountIndex total_settinglink_num =%d,dual_sys =%d,settinglink_dual_sys_index=%d"
#define MMICONN_MANAGER_WINTAB_6768_112_2_18_2_12_19_22 "MMIAUT::SearchDirectLinkAccountIndex * item_num =%d"
#define MMICONN_MANAGER_WINTAB_6784_112_2_18_2_12_19_23 "MMIAUT::GetNoProxyAccountIndex  item_num ||detail_item is NULL"
#define MMICONN_MANAGER_WINTAB_6820_112_2_18_2_12_19_24 "MMIAUT::SearchDirectLinkAccountIndex total_settinglink_num =%d,dual_sys =%d,settinglink_dual_sys_index=%d"
#define MMICONN_MANAGER_WINTAB_6826_112_2_18_2_12_19_25 "MMIAUT::SearchDirectLinkAccountIndex * item_num =%d"
#define MMICONN_MANAGER_WINTAB_7030_112_2_18_2_12_19_26 "HandleNetworkAccountDownloadCB enter"
#define MMICONNECTION_347_112_2_18_2_12_21_27 "MMICONNECTION_Setting_GetDataRoamingStatus data_roaming_status=%d"
#define MMITV_OUT_259_112_2_18_2_12_22_28 "mmitv_out.c, MMIAPITVOUT_StartTvOut(), tvout_mode = %d. mode = %d."
#define MMITV_OUT_406_112_2_18_2_12_22_29 "mmitv_out.c, MMIAPITVOUT_Close()"
#define MMITV_OUT_412_112_2_18_2_12_22_30 "mmitv_out.c, MMIAPITVOUT_Close(), not close"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_CONNECTION_TRC)
TRACE_MSG(MMICONN_MANAGER_WINTAB_1626_112_2_18_2_12_8_0,"SMS: HandleSmsChatMenuOptWinMsg, msg_id = 0x%x")
TRACE_MSG(MMICONN_MANAGER_WINTAB_1742_112_2_18_2_12_9_1,"[MMICONNECTION]: HandleLinkListLongOKMenuWinMsg, msg_id = 0x%x")
TRACE_MSG(MMICONN_MANAGER_WINTAB_1866_112_2_18_2_12_9_2,"mmiconnection_wintab.c, HandleConnectionMenuWinMsg(), msg_id = %x")
TRACE_MSG(MMICONN_MANAGER_WINTAB_2010_112_2_18_2_12_9_3,"mmiconnection_wintab.c, HandleConnectionMenuWinMsg(), msg_id = %x")
TRACE_MSG(MMICONN_MANAGER_WINTAB_2294_112_2_18_2_12_10_4,"[MMICONNECTION]:HandleSettingListWinMsg() msg_id = %d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_2841_112_2_18_2_12_11_5,"MMIAPICONNECTION_GetGPRSStatus gprs_settings=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_3336_112_2_18_2_12_12_6,"setting_detail is null")
TRACE_MSG(MMICONN_MANAGER_WINTAB_4987_112_2_18_2_12_15_7,"MMIAPICONNECTION_ChangeSettingListLanguage")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5026_112_2_18_2_12_16_8,"MMICONNECTION_ResetSettingList is_clean_user_data=%d, settinglink_num=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5123_112_2_18_2_12_16_9,"MMICONNECTION_ResetSettingList is_clean_user_data=%d, settinglink_num=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5398_112_2_18_2_12_16_10,"[MMICONNECTION]:MMIAPICONNECTION_OpenSettingListWin sim_ok=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5440_112_2_18_2_12_16_11,"HandleSIMPlugNotifyConnectionFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5476_112_2_18_2_12_16_12,"[MMICONNECTION]:MMICONNECTION_Init setting read Failed")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5786_112_2_18_2_12_17_13,"AutoAdapting dual_sys=%d, account_type=%d, is_need_clean=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5808_112_2_18_2_12_17_14,"AutoAdapting hplmn=%d, conection_plmn_name=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_5882_112_2_18_2_12_17_15,"AutoAdapting is_need_add_default=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6086_112_2_18_2_12_18_16,"[MMICONNECTION]:MMICONNECTION_GetAccountDefaultIndex dual_sys=%d,account_type=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6099_112_2_18_2_12_18_17,"[MMICONNECTION]:MMICONNECTION_GetAccountDefaultIndex setting read Failed")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6374_112_2_18_2_12_18_18,"ParseNetworkAccountInfo is_res_data_ok=%d, account_num=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6454_112_2_18_2_12_18_19,"LoadNetworkAccountInfoFromRes is_res_data_exist=%d, account_num=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6732_112_2_18_2_12_19_20,"MMIAUT::GetCurSIMNoProxyAccountIndex  item_num ||detail_item is NULL")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6763_112_2_18_2_12_19_21,"MMIAUT::SearchDirectLinkAccountIndex total_settinglink_num =%d,dual_sys =%d,settinglink_dual_sys_index=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6768_112_2_18_2_12_19_22,"MMIAUT::SearchDirectLinkAccountIndex * item_num =%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6784_112_2_18_2_12_19_23,"MMIAUT::GetNoProxyAccountIndex  item_num ||detail_item is NULL")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6820_112_2_18_2_12_19_24,"MMIAUT::SearchDirectLinkAccountIndex total_settinglink_num =%d,dual_sys =%d,settinglink_dual_sys_index=%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_6826_112_2_18_2_12_19_25,"MMIAUT::SearchDirectLinkAccountIndex * item_num =%d")
TRACE_MSG(MMICONN_MANAGER_WINTAB_7030_112_2_18_2_12_19_26,"HandleNetworkAccountDownloadCB enter")
TRACE_MSG(MMICONNECTION_347_112_2_18_2_12_21_27,"MMICONNECTION_Setting_GetDataRoamingStatus data_roaming_status=%d")
TRACE_MSG(MMITV_OUT_259_112_2_18_2_12_22_28,"mmitv_out.c, MMIAPITVOUT_StartTvOut(), tvout_mode = %d. mode = %d.")
TRACE_MSG(MMITV_OUT_406_112_2_18_2_12_22_29,"mmitv_out.c, MMIAPITVOUT_Close()")
TRACE_MSG(MMITV_OUT_412_112_2_18_2_12_22_30,"mmitv_out.c, MMIAPITVOUT_Close(), not close")
END_TRACE_MAP(MMI_APP_CONNECTION_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_CONNECTION_TRC_H_

