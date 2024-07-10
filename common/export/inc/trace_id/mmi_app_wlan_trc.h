/******************************************************************************
 ** File Name:      mmi_app_wlan_trc.h                                         *
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
//trace_id:241
#ifndef _MMI_APP_WLAN_TRC_H_
#define _MMI_APP_WLAN_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIWLAN_MANAGER_202_112_2_18_3_9_30_0 "WIFI:error:Manager:%s: notify_info_ptr is PNULL!"
#define MMIWLAN_MANAGER_212_112_2_18_3_9_30_1 "WIFI:error:Manager:%s: wifi_subscriber_list create failed!"
#define MMIWLAN_MANAGER_221_112_2_18_3_9_30_2 "WIFI:info:Manager:%s:app %d reg inserted"
#define MMIWLAN_MANAGER_421_112_2_18_3_9_30_3 "wifi scan status %d"
#define MMIWLAN_MANAGER_529_112_2_18_3_9_30_4 "MMIWIFI_DirectConnect curr profile state = %d"
#define MMIWLAN_MANAGER_573_112_2_18_3_9_30_5 "MMIWIFI_Connect: protocol = %d, credential = %d, status = %d"
#define MMIWLAN_MANAGER_577_112_2_18_3_9_30_6 "MMIWIFI_Connect %s is connecting (%d)"
#define MMIWLAN_MANAGER_583_112_2_18_3_9_30_7 "MMIWIFI_Connect Scaning"
#define MMIWLAN_MANAGER_731_112_2_18_3_9_31_8 "mmiwifi error MMIWIFI_DisDhcp: profile_ptr is PNULL"
#define MMIWLAN_MANAGER_737_112_2_18_3_9_31_9 "mmiwifi error MMIWIFI_DisDhcp: ip type is self define"
#define MMIWLAN_MANAGER_764_112_2_18_3_9_31_10 "mmiwifi error MMIWIFI_DisConnect: profile_ptr is PNULL"
#define MMIWLAN_MANAGER_772_112_2_18_3_9_31_11 "MMIWIFI_DisConnect:%d\r\n"
#define MMIWLAN_MANAGER_774_112_2_18_3_9_31_12 "MMIWIFI_DisConnect:%d\r\n"
#define MMIWLAN_MANAGER_802_112_2_18_3_9_31_13 "\n mmiwifi error MMIWIFI_Dhcp profile_ptr is null"
#define MMIWLAN_MANAGER_806_112_2_18_3_9_31_14 "\n mmiwifi info MMIWIFI_Dhcp begin dhcp progress!"
#define MMIWLAN_MANAGER_814_112_2_18_3_9_31_15 "\n mmiwifi error MMIWIFI_Dhcp sci_async_dhcp return %d"
#define MMIWLAN_MANAGER_854_112_2_18_3_9_31_16 "MMIWIFI_GetCurrProfile profile_ptr=%p"
#define MMIWLAN_MANAGER_865_112_2_18_3_9_31_17 "MMIWIFI_GetCurrProfile current_profile_ptr=%p"
#define MMIWLAN_MANAGER_884_112_2_18_3_9_31_18 "mmiwifi error MMIWIFI_HandleSuppMsg: wrong msg(%d)!"
#define MMIWLAN_MANAGER_888_112_2_18_3_9_31_19 "MMIWIFI_HandleSuppMsg msg = (%d)!"
#define MMIWLAN_MANAGER_897_112_2_18_3_9_31_20 "\n mmiwifi recv WIFISUPP_SIG_ON_CNF"
#define MMIWLAN_MANAGER_930_112_2_18_3_9_31_21 "\n mmiwifi recv WIFISUPP_SIG_OFF_CNF"
#define MMIWLAN_MANAGER_962_112_2_18_3_9_31_22 "\n mmiwifi recv WIFISUPP_SIG_SCAN_CNF, result=%d"
#define MMIWLAN_MANAGER_996_112_2_18_3_9_31_23 "mmiwifi recv WIFISUPP_SIG_SCANBEGIN_IND sig"
#define MMIWLAN_MANAGER_1011_112_2_18_3_9_31_24 "\n mmiwifi recv WIFISUPP_SIG_SCAN_IND, ssid_len=%d,ssid=%s,signal_qua=%d"
#define MMIWLAN_MANAGER_1036_112_2_18_3_9_31_25 "WIFI:info:Manager:recv TCPIP_DHCP_RESULT_IND %d"
#define MMIWLAN_MANAGER_1046_112_2_18_3_9_31_26 "WIFI:info:Manager:recv WIFISUPP_SIG_DISCONNECT_CNF %d"
#define MMIWLAN_MANAGER_1052_112_2_18_3_9_31_27 "\n mmiwifi recv miscellaneous message, msg_id=%d"
#define MMIWLAN_MANAGER_1106_112_2_18_3_9_32_28 "WIFI:error:%s profile_ptr is PNULL!"
#define MMIWLAN_MANAGER_1118_112_2_18_3_9_32_29 "WIFI:info:%s: result(%d) is failed!"
#define MMIWLAN_MANAGER_1123_112_2_18_3_9_32_30 "WIFI:info:Manager:%s host IP      :%0x"
#define MMIWLAN_MANAGER_1124_112_2_18_3_9_32_31 "WIFI:info:Manager:%s subnet mask  :%0x"
#define MMIWLAN_MANAGER_1125_112_2_18_3_9_32_32 "WIFI:info:Manager:%s gateway      :%0x"
#define MMIWLAN_MANAGER_1126_112_2_18_3_9_32_33 "WIFI:info:Manager:%s primary DNS  :%0x"
#define MMIWLAN_MANAGER_1127_112_2_18_3_9_32_34 "WIFI:info:Manager:%s secondary DNS:%0x"
#define MMIWLAN_MANAGER_1128_112_2_18_3_9_32_35 "WIFI:info:Manager:%s DHCP server  :%0x"
#define MMIWLAN_MANAGER_1129_112_2_18_3_9_32_36 "WIFI:info:Manager:%s netid        :%0x"
#define MMIWLAN_MANAGER_1203_112_2_18_3_9_32_37 "WIFI:error:%s not find profile_ptr!"
#define MMIWLAN_MANAGER_1268_112_2_18_3_9_32_38 "WIFI:error:Manager:%s wifi_on_cnf_ptr is PNULL"
#define MMIWLAN_MANAGER_1295_112_2_18_3_9_32_39 "WIFI:info:%s scan indication hide SSID AP"
#define MMIWLAN_MANAGER_1300_112_2_18_3_9_32_40 "WIFI:info:Manager:%s ESSID =%s"
#define MMIWLAN_MANAGER_1301_112_2_18_3_9_32_41 "WIFI:info:Manager:%s network_mode =%d"
#define MMIWLAN_MANAGER_1302_112_2_18_3_9_32_42 "WIFI:info:Manager:%s encryp_protocol =%d"
#define MMIWLAN_MANAGER_1303_112_2_18_3_9_32_43 "WIFI:info:Manager:%s credential_type =%d"
#define MMIWLAN_MANAGER_1304_112_2_18_3_9_32_44 "WIFI:info:Manager:%s chanel =%d"
#define MMIWLAN_MANAGER_1305_112_2_18_3_9_32_45 "WIFI:info:Manager:%s pairwise_cipher =%d"
#define MMIWLAN_MANAGER_1306_112_2_18_3_9_32_46 "WIFI:info:Manager:%s group_cipher =%d"
#define MMIWLAN_MANAGER_1307_112_2_18_3_9_32_47 "WIFI:info:Manager:%s rssi =%d"
#define MMIWLAN_MANAGER_1308_112_2_18_3_9_32_48 "WIFI:info:Manager:%s rate =%d"
#define MMIWLAN_MANAGER_1353_112_2_18_3_9_32_49 "WIFI:info:%s Not Update AP info for weak rssi, profile.rssi is %d"
#define MMIWLAN_MANAGER_1358_112_2_18_3_9_32_50 "WIFI:info:%s Update AP info"
#define MMIWLAN_MANAGER_1440_112_2_18_3_9_32_51 "WIFI:info:%s New AP info added"
#define MMIWLAN_MANAGER_1456_112_2_18_3_9_32_52 "mmiwifi error %s: AddProfileToList failed!"
#define MMIWLAN_MANAGER_1465_112_2_18_3_9_32_53 "mmiwifi error %s: AddProfileToList failed!"
#define MMIWLAN_MANAGER_1522_112_2_18_3_9_32_54 "WIFI:info:MMI:ip address   - %s"
#define MMIWLAN_MANAGER_1523_112_2_18_3_9_32_55 "WIFI:info:MMI:subnet mask  - %s"
#define MMIWLAN_MANAGER_1524_112_2_18_3_9_32_56 "WIFI:info:MMI:gateway      - %s"
#define MMIWLAN_MANAGER_1525_112_2_18_3_9_32_57 "WIFI:info:MMI:first dns    - %s"
#define MMIWLAN_MANAGER_1526_112_2_18_3_9_32_58 "WIFI:info:MMI:second dns   - %s"
#define MMIWLAN_MANAGER_1531_112_2_18_3_9_32_59 "mmiwifi error T_P_APP WifiHandleSuppSigConnectCnf: TCPIP_SetIpAddress failed(%d)!"
#define MMIWLAN_MANAGER_1613_112_2_18_3_9_33_60 "WIFI:error:Manager:%s profile_ptr is PNULL!"
#define MMIWLAN_MANAGER_1646_112_2_18_3_9_33_61 "WIFI:info:Manager:%s profile_ptr is PNULL"
#define MMIWLAN_MANAGER_1737_112_2_18_3_9_33_62 "WLAN new_siganl = %d , old_singal = %d"
#define MMIWLAN_MANAGER_1759_112_2_18_3_9_33_63 "WLAN profile bssid = %s, rssi bssid = %s,ssid = %s,old_sig = %d,new_sig = %d "
#define MMIWLAN_MANAGER_1762_112_2_18_3_9_33_64 "WLAN found connected bssid equal"
#define MMIWLAN_MANAGER_1853_112_2_18_3_9_33_65 "\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
#define MMIWLAN_MANAGER_1887_112_2_18_3_9_33_66 "\n mmiwifi dispatch WIFISUPP_SIG_CONNECT_CNF, result = %d"
#define MMIWLAN_MANAGER_1898_112_2_18_3_9_33_67 "\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
#define MMIWLAN_MANAGER_1932_112_2_18_3_9_33_68 "\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
#define MMIWLAN_MANAGER_1941_112_2_18_3_9_33_69 "\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND"
#define MMIWLAN_MANAGER_1950_112_2_18_3_9_33_70 "\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
#define MMIWLAN_MANAGER_1983_112_2_18_3_9_33_71 "\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
#define MMIWLAN_MANAGER_1992_112_2_18_3_9_33_72 "\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND"
#define MMIWLAN_MANAGER_2001_112_2_18_3_9_33_73 "\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
#define MMIWLAN_MANAGER_2035_112_2_18_3_9_33_74 "\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
#define MMIWLAN_MANAGER_2044_112_2_18_3_9_34_75 "\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND"
#define MMIWLAN_MANAGER_2053_112_2_18_3_9_34_76 "\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
#define MMIWLAN_MANAGER_2087_112_2_18_3_9_34_77 "\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND"
#define MMIWLAN_MANAGER_2097_112_2_18_3_9_34_78 "\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND"
#define MMIWLAN_MANAGER_2234_112_2_18_3_9_34_79 "MMIWIFI_AutoConnect "
#define MMIWLAN_MANAGER_2238_112_2_18_3_9_34_80 "MMIWIFI_AutoConnect : Fly Mode or OFF"
#define MMIWLAN_MANAGER_2244_112_2_18_3_9_34_81 "Index Name   ON_AIR TRUSTED STATUS AUTO_CON"
#define MMIWLAN_MANAGER_2251_112_2_18_3_9_34_82 "%d    %s    %d    %d    %d    %d  "
#define MMIWLAN_MANAGER_2260_112_2_18_3_9_34_83 "MMIWIFI_AutoConnect %s is connecting or connected"
#define MMIWLAN_MANAGER_2274_112_2_18_3_9_34_84 "MMIWIFI_AutoConnect %s is connecting or connected from_index = %d"
#define MMIWLAN_MANAGER_2297_112_2_18_3_9_34_85 "MMIWIFI_AutoConnect Connect -> %s"
#define MMIWLAN_MANAGER_2321_112_2_18_3_9_34_86 "MMIWIFI_StartAutoScan Wlan Or AutoScan OFF"
#define MMIWLAN_MANAGER_2356_112_2_18_3_9_34_87 "MMIWIFI_StartAutoScan autoscan_status = %d"
#define MMIWLAN_MANAGER_2374_112_2_18_3_9_34_88 "MMIWIFI_StopAutoScan"
#define MMIWLAN_MANAGER_2384_112_2_18_3_9_34_89 "MMIWIFI_StartAutoScan Wlan Or AutoScan OFF"
#define MMIWLAN_MANAGER_2417_112_2_18_3_9_34_90 "MMIWIFI_IsConnecting ret = %d"
#define MMIWLAN_NV_171_112_2_18_3_9_35_91 "MMIWIFI_GetIsWlanOn is_on = %d"
#define MMIWLAN_NV_772_112_2_18_3_9_36_92 "MMIWIFI_SetServiceProvider No mem"
#define MMIWLAN_NV_824_112_2_18_3_9_37_93 "MMIWIFI_SetServiceProvider No mem"
#define MMIWLAN_NV_876_112_2_18_3_9_37_94 "MMIWIFI_SetServiceProvider No mem"
#define MMIWLAN_NV_928_112_2_18_3_9_37_95 "MMIWIFI_SetServiceProvider No mem"
#define MMIWLAN_NV_980_112_2_18_3_9_37_96 "MMIWIFI_SetServiceProvider No mem"
#define MMIWLAN_NV_1032_112_2_18_3_9_37_97 "MMIWIFI_SetServiceProvider No mem"
#define MMIWLAN_NV_1084_112_2_18_3_9_37_98 "MMIWIFI_SetServiceProvider No mem"
#define MMIWLAN_PORTAL_1645_112_2_18_3_9_41_99 "HandleWlanPortalTimer timer_id=%d, s_wlan_context.status_timer_id=%d"
#define MMIWLAN_PORTAL_1715_112_2_18_3_9_41_100 "WlanMsgPortalCallback event_id=%d,result=%d,s_wlan_context.portal_status=%d"
#define MMIWLAN_PORTAL_1932_112_2_18_3_9_41_101 "ImplementWlanOperation s_wlan_context.portal_status=%d"
#define MMIWLAN_PORTAL_2010_112_2_18_3_9_41_102 "HandleCMCCAutoLoginQueryWinMsg() msg_id=0x%04x"
#define MMIWLAN_PORTAL_2068_112_2_18_3_9_41_103 "WIFI_OpenPortalWin win_type= %d, s_wlan_context.portal_status=%d"
#define MMIWLAN_PORTAL_2109_112_2_18_3_9_41_104 "MMIWIFI_CMCCAutoLogin login=%d, notify=%d, s_wlan_context.portal_status=%d"
#define MMIWLAN_PORTAL_2134_112_2_18_3_9_41_105 "MMIWIFI_CMCCLogoff status = %d"
#define MMIWLAN_PORTAL_2210_112_2_18_3_9_42_106 "MMIWIFI_Update File_handle is NULL"
#define MMIWLAN_PORTAL_2219_112_2_18_3_9_42_107 "MMIWIFI_Update File size Incorrect(%d)"
#define MMIWLAN_PORTAL_2228_112_2_18_3_9_42_108 "MMIWIFI_Update No mem"
#define MMIWLAN_PORTAL_2236_112_2_18_3_9_42_109 "MMIWIFI_Update Read File error"
#define MMIWLAN_PORTAL_2295_112_2_18_3_9_42_110 "MMIWIFI_HandlePortalLogin login_res=%d"
#define MMIWLAN_PORTAL_2341_112_2_18_3_9_42_111 "MMIWIFI_HandlePortalOffline offline_res=%d"
#define MMIWLAN_PORTAL_2569_112_2_18_3_9_42_112 "wifi_ParseXML Bad param"
#define MMIWLAN_PORTAL_NET_492_112_2_18_3_9_44_113 "HandleWlanPortalPsMsg() enter msg_id= %d"
#define MMIWLAN_PORTAL_NET_498_112_2_18_3_9_44_114 "HandleWlanPortalPsMsg: MMIPDP_ACTIVE_CNF nsapi=0x%x"
#define MMIWLAN_PORTAL_NET_528_112_2_18_3_9_44_115 "HandleWlanPortalPsMsg: MMIPDP_DEACTIVE_CNF"
#define MMIWLAN_PORTAL_NET_538_112_2_18_3_9_44_116 "HandleWlanPortalPsMsg: MMIPDP_DEACTIVE_IND"
#define MMIWLAN_PORTAL_NET_575_112_2_18_3_9_44_117 "PortalRequest Input Invalid"
#define MMIWLAN_PORTAL_NET_584_112_2_18_3_9_44_118 "PortalRequest method=%d, url_len=%d, body_len=%d"
#define MMIWLAN_PORTAL_NET_652_112_2_18_3_9_44_119 "PortalRequest url error"
#define MMIWLAN_PORTAL_NET_664_112_2_18_3_9_45_120 "PortalRequest method=%d, url_len=%d, body_len=%d, portal_url_ptr=%s"
#define MMIWLAN_PORTAL_NET_676_112_2_18_3_9_45_121 "PortalRequest no memory"
#define MMIWLAN_PORTAL_NET_748_112_2_18_3_9_45_122 "PortalRequest no memory"
#define MMIWLAN_PORTAL_NET_793_112_2_18_3_9_45_123 "PortalRequest no memory"
#define MMIWLAN_PORTAL_NET_880_112_2_18_3_9_45_124 "HandlePortalHttpInitCnf not Portal task"
#define MMIWLAN_PORTAL_NET_1023_112_2_18_3_9_45_125 "HandlePortalHttpDataInd recv_len=%d, total_len=%d, data_len=%d, response_code=%d"
#define MMIWLAN_PORTAL_NET_1109_112_2_18_3_9_46_126 "HandlePortalHttpHeaderInd response_code=%d, content_len=%d"
#define MMIWLAN_PORTAL_NET_1146_112_2_18_3_9_46_127 "HandlePortalHttpHeaderInd No Memory"
#define MMIWLAN_PORTAL_NET_1340_112_2_18_3_9_46_128 "HandlePortalHttpRedirectInd cur_page_url_ptr=%s"
#define MMIWLAN_PORTAL_NET_1387_112_2_18_3_9_46_129 "JXC wlan login type=%d"
#define MMIWLAN_PORTAL_NET_1411_112_2_18_3_9_46_130 "HandlePortalHttpPostCnf login_res=%d, offline_res=%d"
#define MMIWLAN_PORTAL_NET_1510_112_2_18_3_9_47_131 "HandlePortalHttpPostCnf url = %d"
#define MMIWLAN_PORTAL_NET_1595_112_2_18_3_9_47_132 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_INIT_CNF"
#define MMIWLAN_PORTAL_NET_1600_112_2_18_3_9_47_133 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND"
#define MMIWLAN_PORTAL_NET_1605_112_2_18_3_9_47_134 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND"
#define MMIWLAN_PORTAL_NET_1610_112_2_18_3_9_47_135 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_GET_CNF"
#define MMIWLAN_PORTAL_NET_1615_112_2_18_3_9_47_136 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_HEAD_CNF"
#define MMIWLAN_PORTAL_NET_1620_112_2_18_3_9_47_137 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_DATA_IND"
#define MMIWLAN_PORTAL_NET_1625_112_2_18_3_9_47_138 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_HEADER_IND"
#define MMIWLAN_PORTAL_NET_1630_112_2_18_3_9_47_139 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_ERROR_IND"
#define MMIWLAN_PORTAL_NET_1635_112_2_18_3_9_47_140 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_CLOSE_CNF"
#define MMIWLAN_PORTAL_NET_1640_112_2_18_3_9_47_141 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_CANCEL_CNF"
#define MMIWLAN_PORTAL_NET_1645_112_2_18_3_9_47_142 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_NET_PROG_IND"
#define MMIWLAN_PORTAL_NET_1650_112_2_18_3_9_47_143 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_REDIRECT_IND"
#define MMIWLAN_PORTAL_NET_1655_112_2_18_3_9_47_144 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_POST_CNF"
#define MMIWLAN_PORTAL_NET_1661_112_2_18_3_9_47_145 "MMIWIFI_HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND"
#define MMIWLAN_PORTAL_NET_1667_112_2_18_3_9_47_146 "MMIWIFI_HandleHttpMsg Not handle msg_id = %x"
#define MMIWLAN_PORTAL_NET_1689_112_2_18_3_9_47_147 "StartPortalHttpRequest No Memory"
#define MMIWLAN_PORTAL_NET_1717_112_2_18_3_9_47_148 "StartPortalHttpRequest http_error=%d"
#define MMIWLAN_PORTAL_NET_1743_112_2_18_3_9_47_149 "MMIWIFI_WlanPortalNetConnect status=%d"
#define MMIWLAN_PORTAL_NET_1759_112_2_18_3_9_47_150 "WlanPortalNetConnect:MMIAPIPDP_Active fail"
#define MMIWLAN_PORTAL_NET_1816_112_2_18_3_9_47_151 "WlanPortalFindActionUrl Input Invalid"
#define MMIWLAN_PORTAL_NET_1835_112_2_18_3_9_47_152 "WlanPortalFindActionUrl Find Myform"
#define MMIWLAN_PORTAL_NET_1867_112_2_18_3_9_47_153 "WlanPortalFindActionUrl Find LoginForm"
#define MMIWLAN_PORTAL_NET_1887_112_2_18_3_9_47_154 "JXC WlanPortalFindActionUrl urlptr=%s,len=%d,isloginform=%d"
#define MMIWLAN_PORTAL_NET_1907_112_2_18_3_9_47_155 "WlanPortalFindNameValue Input Invalid"
#define MMIWLAN_PORTAL_NET_1916_112_2_18_3_9_47_156 "WlanPortalFindNameValue Find!"
#define MMIWLAN_PORTAL_NET_1948_112_2_18_3_9_48_157 "WlanPortalFindMethod Input Invalid"
#define MMIWLAN_PORTAL_NET_2003_112_2_18_3_9_48_158 "WlanPortalLoginRes Input Invalid"
#define MMIWLAN_PORTAL_NET_2017_112_2_18_3_9_48_159 "WlanPortalLoginRes res_num=%d"
#define MMIWLAN_PORTAL_NET_2085_112_2_18_3_9_48_160 "WlanPortalOfflineRes Input Invalid"
#define MMIWLAN_PORTAL_NET_2099_112_2_18_3_9_48_161 "WlanPortalOfflineRes res_num=%d"
#define MMIWLAN_PORTAL_NET_2212_112_2_18_3_9_48_162 "WlanPortalParseXhtmlData Input Invalid"
#define MMIWLAN_PORTAL_NET_2267_112_2_18_3_9_48_163 "WlanPortalParseXhtmlData No Memory"
#define MMIWLAN_PORTAL_NET_2426_112_2_18_3_9_49_164 "WlanPortalParseXhtmlData *body_len_ptr=%d, temp_body_len=%d, url=%s, body=%s"
#define MMIWLAN_PORTAL_NET_2430_112_2_18_3_9_49_165 "WlanPortalParseXhtmlData No Memory"
#define MMIWLAN_PORTAL_NET_2486_112_2_18_3_9_49_166 "WlanPortalParseLoginSuccessData Input Invalid"
#define MMIWLAN_PORTAL_NET_2530_112_2_18_3_9_49_167 "WlanPortalParseXhtmlData No Memory"
#define MMIWLAN_PORTAL_NET_2574_112_2_18_3_9_49_168 "WlanPortalParseLoginSuccessData *body_len_ptr=%d, temp_body_len=%d, url=%s, body=%s"
#define MMIWLAN_PORTAL_NET_2578_112_2_18_3_9_49_169 "WlanPortalParseLoginSuccessData No Memory"
#define MMIWLAN_PROFILE_717_112_2_18_3_9_50_170 "MMIWIFI_FindProfileByAP"
#define MMIWLAN_PROFILE_751_112_2_18_3_9_51_171 "MMIWIFI_FindProfileByAP: status = %d"
#define MMIWLAN_PROFILE_1055_112_2_18_3_9_51_172 "MMIWIFI_FindProfileByAP"
#define MMIWLAN_PROFILE_1080_112_2_18_3_9_51_173 "MMIAPIWIFI_APIsAvailable"
#define MMIWLAN_PROFILE_1141_112_2_18_3_9_51_174 "MMIWIFI_IsCMCCAvailable"
#define MMIWLAN_PROFILE_1160_112_2_18_3_9_51_175 "MMIWIFI_GetCMCCProfile"
#define MMIWLAN_WIN_3303_112_2_18_3_9_58_176 "WIFI:error:%s Profile Initialize faild!"
#define MMIWLAN_WIN_3379_112_2_18_3_9_58_177 "MMIWIFI_OpenSelectServiceWin reselect = %d"
#define MMIWLAN_WIN_3390_112_2_18_3_9_58_178 "MMIWIFI_OpenSelectServiceWin No Profile exist"
#define MMIWLAN_WIN_3398_112_2_18_3_9_58_179 "MMIWIFI_OpenSelectServiceWin s_last_tick_time = %ld, tick_time = %ld"
#define MMIWLAN_WIN_3484_112_2_18_3_9_59_180 "WifiCreateListPopMenu window already opened !"
#define MMIWLAN_WIN_3726_112_2_18_3_9_59_181 "WIFI:info:MMI recv MMIWIFI_MSG_ON_CNF, result is %d"
#define MMIWLAN_WIN_3761_112_2_18_3_9_59_182 "WIFI:info:MMI recv MMIWIFI_MSG_OFF_CNF, result is %d"
#define MMIWLAN_WIN_3801_112_2_18_3_9_59_183 "WIFI:info:MMI recv MMIWIFI_MSG_SCAN_CNF, result is %d"
#define MMIWLAN_WIN_3826_112_2_18_3_9_59_184 "WIFI:info:MMI recv MMIWIFI_MSG_SCAN_BEGIN_IND sig"
#define MMIWLAN_WIN_3851_112_2_18_3_9_59_185 "\n mmiwifi qq recv MMIWIFI_MSG_SCAN_IND"
#define MMIWLAN_WIN_3861_112_2_18_3_9_59_186 "WIFI:info:MMI recv MMIWIFI_MSG_CONNECT_CNF, result is %d"
#define MMIWLAN_WIN_3865_112_2_18_3_9_59_187 "\n mmiwifi error MMIWIFI_MSG_CONNECT_CNF profile_ptr is PNULL!"
#define MMIWLAN_WIN_3945_112_2_18_3_10_0_188 "WIFI:info:MMI:recv MMIWIFI_MSG_DISCONNECT_CNF, result is %d"
#define MMIWLAN_WIN_3962_112_2_18_3_10_0_189 "WIFI:info:MMI:recv MMIWIFI_MSG_DISCONNECT_IND"
#define MMIWLAN_WIN_3985_112_2_18_3_10_0_190 "WIFI:info:MMI recv MMIWIFI_MSG_DHCP_CNF, result is %d"
#define MMIWLAN_WIN_4009_112_2_18_3_10_0_191 "WIFI:error:MMI recv MMIWIFI_MSG_DHCP_CNF, but profile_ptr is PNULL!"
#define MMIWLAN_WIN_4041_112_2_18_3_10_0_192 "WIFI:info:MMI recv MMIWIFI_MSG_BSSID_CHANGED_IND"
#define MMIWLAN_WIN_4046_112_2_18_3_10_0_193 "WIFI:info:MMI recv MMIWIFI_MSG_RSSI_CHANGED_IND new ssi is %d"
#define MMIWLAN_WIN_4082_112_2_18_3_10_0_194 "WifiHandleListWinMsg msg_id = %x"
#define MMIWLAN_WIN_4141_112_2_18_3_10_0_195 "WifiHandleListWinMsg wlan_on = %d, wifi_on = %d"
#define MMIWLAN_WIN_4267_112_2_18_3_10_0_196 "mmipdp error %s: profile_ptr is PNULL!"
#define MMIWLAN_WIN_4285_112_2_18_3_10_0_197 "mmipdp error %s: profile_ptr is PNULL!"
#define MMIWLAN_WIN_4425_112_2_18_3_10_1_198 "WifiAppendListItem ESSID =%s order_index = %d"
#define MMIWLAN_WIN_4477_112_2_18_3_10_1_199 "WifiAppendSelectAPListItem Error "
#define MMIWLAN_WIN_6609_112_2_18_3_10_5_200 "WifiHandleAuthSettingWAPICERTIFICATEWinMsg profile_ptr is Error index = %d"
#define MMIWLAN_WIN_7786_112_2_18_3_10_8_201 "OpenSelectClientCertificateFileWinCallBack array size == 0"
#define MMIWLAN_WIN_7791_112_2_18_3_10_8_202 "OpenSelectClientCertificateFileWinCallBack failed"
#define MMIWLAN_WIN_7826_112_2_18_3_10_8_203 "OpenSelectClientCertificateFileForConnectWinCallBack array size == 0"
#define MMIWLAN_WIN_7831_112_2_18_3_10_8_204 "OpenSelectClientCertificateFileForConnectWinCallBack failed"
#define MMIWLAN_WIN_7860_112_2_18_3_10_8_205 "OpenSelectParentCertificateFileWinCallBack array size == 0"
#define MMIWLAN_WIN_7865_112_2_18_3_10_8_206 "OpenSelectParentCertificateFileWinCallBack failed"
#define MMIWLAN_WIN_7900_112_2_18_3_10_8_207 "OpenSelectParentCertificateFileForconnectWinCallBack array size == 0"
#define MMIWLAN_WIN_7905_112_2_18_3_10_8_208 "OpenSelectParentCertificateFileForconnectWinCallBack failed"
#define MMIWLAN_WIN_8863_112_2_18_3_10_11_209 "MMIWIFI_OpenMainWin in_focus_win = %d"
#define MMIWLAN_WIN_8912_112_2_18_3_10_11_210 "MMIWIFI_OpenMainWin Bad param"
#define MMIWLAN_WIN_8935_112_2_18_3_10_11_211 "MMIWIFI_OpenDefAPSettingWin "
#define MMIWLAN_WIN_8961_112_2_18_3_10_11_212 "WifiHandleSettinglistChildWinMsg msg_id = %x"
#define MMIWLAN_WIN_9182_112_2_18_3_10_12_213 "WifiHandleAPEAPSimIndexWinMsg msg_id = %x"
#define MMIWLAN_WIN_9610_112_2_18_3_10_12_214 "WifiHandleSearchWinMsg msg_id = %x"
#define MMIWLAN_WIN_9669_112_2_18_3_10_13_215 "WifiHandleWpaStateSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_9723_112_2_18_3_10_13_216 "WifiHandleAPEditWpaStateSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_9771_112_2_18_3_10_13_217 "WifiHandleAPNetworkStateWinMsg msg_id = %x"
#define MMIWLAN_WIN_9816_112_2_18_3_10_13_218 "WifiHandleAPAuthTypeWinMsg msg_id = %x"
#define MMIWLAN_WIN_9860_112_2_18_3_10_13_219 "WifiHandleAPConnectStatetWinMsg msg_id = %x"
#define MMIWLAN_WIN_9904_112_2_18_3_10_13_220 "WifiHandleWapiStateSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_9957_112_2_18_3_10_13_221 "WifiHandleAPEditWapiStateSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10004_112_2_18_3_10_13_222 "WifiHandleAPEditWapiTypeWinMsg msg_id = %x"
#define MMIWLAN_WIN_10051_112_2_18_3_10_13_223 "WifiHandleWepStateSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10106_112_2_18_3_10_13_224 "WifiHandleWepEncyptSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10160_112_2_18_3_10_13_225 "WifiHandleWepModeSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10212_112_2_18_3_10_14_226 "WifiHandleWepKeyIndexSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10265_112_2_18_3_10_14_227 "WifiHandleWepStateSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10316_112_2_18_3_10_14_228 "WifiHandleWepEncyptSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10365_112_2_18_3_10_14_229 "WifiHandleWepModeSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10413_112_2_18_3_10_14_230 "WifiHandleWepKeyIndexSetWinMsg msg_id = %x"
#define MMIWLAN_WIN_10462_112_2_18_3_10_14_231 "WifiHandleRejectListPopWinMsg msg_id = %x"
#define MMIWLAN_WIN_10543_112_2_18_3_10_14_232 "WifiHandleApListPopWinMsg msg_id = %x"
#define MMIWLAN_WIN_10562_112_2_18_3_10_14_233 "MMIWIFI WifiHandleApListPopWinMsg menu_id=%d"
#define MMIWLAN_WIN_10594_112_2_18_3_10_14_234 "MMIWIFI_Connect Scaning"
#define MMIWLAN_WIN_10705_112_2_18_3_10_15_235 "WifiHandleApTrustListPopWinMsg msg_id = %x"
#define MMIWLAN_WIN_10757_112_2_18_3_10_15_236 "MMIWIFI WifiHandlelistPopMenuWinMsg MMIWIFI_POPMENU_ID_DELETE index=%d,profile_ptr=%d"
#define MMIWLAN_WIN_10888_112_2_18_3_10_15_237 "WifiSetApEditWAPIWin called"
#define MMIWLAN_WIN_11023_112_2_18_3_10_15_238 "WifiHandleAPEditWAPIWinMsg msg_id = %x"
#define MMIWLAN_WIN_11198_112_2_18_3_10_16_239 "WifiHandleAuthSettingWAPICERTIFICATEWinMsg profile_ptr is Error index = %d"
#define MMIWLAN_WIN_11234_112_2_18_3_10_16_240 "WifiHandleEditAPSSIDWinMsg win_id=%d"
#define MMIWLAN_WIN_11325_112_2_18_3_10_16_241 "WIFI:%s ESSID =%s"
#define MMIWLAN_WIN_11490_112_2_18_3_10_16_242 "HandleAPPropertyAlertWinMsg msg_id = %x"
#define MMIWLAN_WIN_11575_112_2_18_3_10_16_243 "MMIWIFI_Connect Scaning"
#define MMIWLAN_WIN_11632_112_2_18_3_10_16_244 "HandleQueryEditConnectedAP msg_id = %x"
#define MMIWLAN_WIN_11748_112_2_18_3_10_17_245 "WifiHandleAPEditAuthSetting auth_type = %x"
#define MMIWLAN_WIN_11922_112_2_18_3_10_17_246 "WifiHandleGPRSToWLANWinMsg msg_id = %x"
#define MMIWLAN_WIN_11988_112_2_18_3_10_17_247 "WifiHandleGPRSToWLANWinMsg msg_id = %x"
#define MMIWLAN_WIN_12055_112_2_18_3_10_17_248 "WifiHandleGPRSToWLANWinMsg msg_id = %x"
#define MMIWLAN_WIN_12536_112_2_18_3_10_18_249 "WifiHandleDefAPSettingWinMsg msg_id = %x"
#define MMIWLAN_WIN_12612_112_2_18_3_10_18_250 "WifiHandleHandleMenuAction node_id = %x"
#define MMIWLAN_WIN_12957_112_2_18_3_10_19_251 "WifiHandleWlanSettingWin win_id=%d"
#define MMIWLAN_WIN_13161_112_2_18_3_10_19_252 "WifiSetWlanWindow called"
#define MMIWLAN_WIN_13340_112_2_18_3_10_20_253 "WIFI:info:MMI:ip address   - %s"
#define MMIWLAN_WIN_13341_112_2_18_3_10_20_254 "WIFI:info:MMI:subnet mask  - %s"
#define MMIWLAN_WIN_13342_112_2_18_3_10_20_255 "WIFI:info:MMI:gateway      - %s"
#define MMIWLAN_WIN_13343_112_2_18_3_10_20_256 "WIFI:info:MMI:first dns    - %s"
#define MMIWLAN_WIN_13344_112_2_18_3_10_20_257 "WIFI:info:MMI:second dns   - %s"
#define MMIWLAN_WIN_13349_112_2_18_3_10_20_258 "mmiwifi error T_P_APP WifiHandleSuppSigConnectCnf: TCPIP_SetIpAddress failed(%d)!"
#define MMIWLAN_WIN_13410_112_2_18_3_10_20_259 "WLAN port is %d"
#define MMIWLAN_WIN_13434_112_2_18_3_10_20_260 "WifiHandleWlanSettingWin win_id=%d"
#define MMIWLAN_WIN_13578_112_2_18_3_10_20_261 "WifiSetOwnAttributeWindow called"
#define MMIWLAN_WIN_13805_112_2_18_3_10_21_262 "WifiHandleProxySettingWin win_id=%d"
#define MMIWLAN_WIN_13927_112_2_18_3_10_21_263 "WifiHandleProxyRejectListWin win_id=%d"
#define MMIWLAN_WIN_14038_112_2_18_3_10_21_264 "WifiHandleProxyRejectListPopMenuWin win_id=%d"
#define MMIWLAN_WIN_14167_112_2_18_3_10_22_265 "WifiHandleProxyRejectListPopMenuWin win_id=%d"
#define MMIWLAN_WIN_14267_112_2_18_3_10_22_266 "WifiHandleProxyRejectListPopMenuWin win_id=%d"
#define MMIWLAN_WIN_14445_112_2_18_3_10_22_267 "WifiSetProxySettingWindow called"
#define MMIWLAN_WIN_14641_112_2_18_3_10_23_268 "WifiSetEditAPWindow called"
#define MMIWLAN_WIN_14867_112_2_18_3_10_23_269 "WifiSetEditWEPWindow called"
#define MMIWLAN_WIN_15077_112_2_18_3_10_23_270 "WifiSetEditWEPWindow called"
#define MMIWLAN_WIN_15222_112_2_18_3_10_24_271 "WifiSetEditWPAWPA2Window called"
#define MMIWLAN_WIN_15352_112_2_18_3_10_24_272 "WifiSetEditWPAWPA2Window called"
#define MMIWLAN_WIN_15409_112_2_18_3_10_24_273 "WifiSetEditWAPIWindow called"
#define MMIWLAN_WIN_15466_112_2_18_3_10_24_274 "WifiSetEditWPAWPA2Window called"
#define MMIWLAN_WIN_15535_112_2_18_3_10_25_275 "WifiSetEditWPAWPA2Window called"
#define MMIWLAN_WIN_15591_112_2_18_3_10_25_276 "WifiSetEditWAPICertificateWindow called"
#define MMIWLAN_WIN_15626_112_2_18_3_10_25_277 "WifiSetEditWAPICertificateForConnectWindow called"
#define MMIWLAN_WIN_15689_112_2_18_3_10_25_278 "MMIAPIWIFI_IsExistInExcludingList result is %d"
#define MMIWLAN_WIN_15825_112_2_18_3_10_25_279 "MMIWIFI_ReSelectAP"
#define MMIWLAN_WIN_15831_112_2_18_3_10_25_280 "MMIWIFI_ReSelectAP MMIWIFI_AutoConnect"
#define MMIWLAN_WIN_15856_112_2_18_3_10_25_281 "MMIWIFI_ChangeServiceToGPRS"
#define MMIWLAN_WIN_15862_112_2_18_3_10_25_282 "MMIWIFI_ChangeServiceToGPRS Query"
#define MMIWLAN_WIN_15882_112_2_18_3_10_25_283 "WifiHandleListWinMsg msg_id = %x"
#define MMIWLAN_WIN_15948_112_2_18_3_10_25_284 "MMIWIFI_IsNeedServiceChange ret = %d"
#define MMIWLAN_WIN_15968_112_2_18_3_10_25_285 "MMIWIFI_HandleChangeService msg_id = %x"
#define MMIWLAN_WIN_16313_112_2_18_3_10_26_286 "WifiListIsAPItemSelected index = %d, ret = %d"
#define MMIWLAN_WIN_16395_112_2_18_3_10_26_287 "WifiHandleMainParentWinMsg msg_id = %x"
#define MMIWLAN_WIN_16438_112_2_18_3_10_26_288 "WifiHandleTrustedlistChildWinMsg msg_id = %x"
#define MMIWLAN_WIN_16687_112_2_18_3_10_27_289 "mmipdp error %s: profile_ptr is PNULL!"
#define MMIWLAN_WIN_16798_112_2_18_3_10_27_290 "MMIWIFI WifiHandlelistPopMenuWinMsg MMIWIFI_POPMENU_ID_DELETE index=%d,profile_ptr=%d"
#define MMIWLAN_WIN_16945_112_2_18_3_10_27_291 "WifiCreateListMenu window already opened !"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_WLAN_TRC)
TRACE_MSG(MMIWLAN_MANAGER_202_112_2_18_3_9_30_0,"WIFI:error:Manager:%s: notify_info_ptr is PNULL!")
TRACE_MSG(MMIWLAN_MANAGER_212_112_2_18_3_9_30_1,"WIFI:error:Manager:%s: wifi_subscriber_list create failed!")
TRACE_MSG(MMIWLAN_MANAGER_221_112_2_18_3_9_30_2,"WIFI:info:Manager:%s:app %d reg inserted")
TRACE_MSG(MMIWLAN_MANAGER_421_112_2_18_3_9_30_3,"wifi scan status %d")
TRACE_MSG(MMIWLAN_MANAGER_529_112_2_18_3_9_30_4,"MMIWIFI_DirectConnect curr profile state = %d")
TRACE_MSG(MMIWLAN_MANAGER_573_112_2_18_3_9_30_5,"MMIWIFI_Connect: protocol = %d, credential = %d, status = %d")
TRACE_MSG(MMIWLAN_MANAGER_577_112_2_18_3_9_30_6,"MMIWIFI_Connect %s is connecting (%d)")
TRACE_MSG(MMIWLAN_MANAGER_583_112_2_18_3_9_30_7,"MMIWIFI_Connect Scaning")
TRACE_MSG(MMIWLAN_MANAGER_731_112_2_18_3_9_31_8,"mmiwifi error MMIWIFI_DisDhcp: profile_ptr is PNULL")
TRACE_MSG(MMIWLAN_MANAGER_737_112_2_18_3_9_31_9,"mmiwifi error MMIWIFI_DisDhcp: ip type is self define")
TRACE_MSG(MMIWLAN_MANAGER_764_112_2_18_3_9_31_10,"mmiwifi error MMIWIFI_DisConnect: profile_ptr is PNULL")
TRACE_MSG(MMIWLAN_MANAGER_772_112_2_18_3_9_31_11,"MMIWIFI_DisConnect:%d\r\n")
TRACE_MSG(MMIWLAN_MANAGER_774_112_2_18_3_9_31_12,"MMIWIFI_DisConnect:%d\r\n")
TRACE_MSG(MMIWLAN_MANAGER_802_112_2_18_3_9_31_13,"\n mmiwifi error MMIWIFI_Dhcp profile_ptr is null")
TRACE_MSG(MMIWLAN_MANAGER_806_112_2_18_3_9_31_14,"\n mmiwifi info MMIWIFI_Dhcp begin dhcp progress!")
TRACE_MSG(MMIWLAN_MANAGER_814_112_2_18_3_9_31_15,"\n mmiwifi error MMIWIFI_Dhcp sci_async_dhcp return %d")
TRACE_MSG(MMIWLAN_MANAGER_854_112_2_18_3_9_31_16,"MMIWIFI_GetCurrProfile profile_ptr=%p")
TRACE_MSG(MMIWLAN_MANAGER_865_112_2_18_3_9_31_17,"MMIWIFI_GetCurrProfile current_profile_ptr=%p")
TRACE_MSG(MMIWLAN_MANAGER_884_112_2_18_3_9_31_18,"mmiwifi error MMIWIFI_HandleSuppMsg: wrong msg(%d)!")
TRACE_MSG(MMIWLAN_MANAGER_888_112_2_18_3_9_31_19,"MMIWIFI_HandleSuppMsg msg = (%d)!")
TRACE_MSG(MMIWLAN_MANAGER_897_112_2_18_3_9_31_20,"\n mmiwifi recv WIFISUPP_SIG_ON_CNF")
TRACE_MSG(MMIWLAN_MANAGER_930_112_2_18_3_9_31_21,"\n mmiwifi recv WIFISUPP_SIG_OFF_CNF")
TRACE_MSG(MMIWLAN_MANAGER_962_112_2_18_3_9_31_22,"\n mmiwifi recv WIFISUPP_SIG_SCAN_CNF, result=%d")
TRACE_MSG(MMIWLAN_MANAGER_996_112_2_18_3_9_31_23,"mmiwifi recv WIFISUPP_SIG_SCANBEGIN_IND sig")
TRACE_MSG(MMIWLAN_MANAGER_1011_112_2_18_3_9_31_24,"\n mmiwifi recv WIFISUPP_SIG_SCAN_IND, ssid_len=%d,ssid=%s,signal_qua=%d")
TRACE_MSG(MMIWLAN_MANAGER_1036_112_2_18_3_9_31_25,"WIFI:info:Manager:recv TCPIP_DHCP_RESULT_IND %d")
TRACE_MSG(MMIWLAN_MANAGER_1046_112_2_18_3_9_31_26,"WIFI:info:Manager:recv WIFISUPP_SIG_DISCONNECT_CNF %d")
TRACE_MSG(MMIWLAN_MANAGER_1052_112_2_18_3_9_31_27,"\n mmiwifi recv miscellaneous message, msg_id=%d")
TRACE_MSG(MMIWLAN_MANAGER_1106_112_2_18_3_9_32_28,"WIFI:error:%s profile_ptr is PNULL!")
TRACE_MSG(MMIWLAN_MANAGER_1118_112_2_18_3_9_32_29,"WIFI:info:%s: result(%d) is failed!")
TRACE_MSG(MMIWLAN_MANAGER_1123_112_2_18_3_9_32_30,"WIFI:info:Manager:%s host IP      :%0x")
TRACE_MSG(MMIWLAN_MANAGER_1124_112_2_18_3_9_32_31,"WIFI:info:Manager:%s subnet mask  :%0x")
TRACE_MSG(MMIWLAN_MANAGER_1125_112_2_18_3_9_32_32,"WIFI:info:Manager:%s gateway      :%0x")
TRACE_MSG(MMIWLAN_MANAGER_1126_112_2_18_3_9_32_33,"WIFI:info:Manager:%s primary DNS  :%0x")
TRACE_MSG(MMIWLAN_MANAGER_1127_112_2_18_3_9_32_34,"WIFI:info:Manager:%s secondary DNS:%0x")
TRACE_MSG(MMIWLAN_MANAGER_1128_112_2_18_3_9_32_35,"WIFI:info:Manager:%s DHCP server  :%0x")
TRACE_MSG(MMIWLAN_MANAGER_1129_112_2_18_3_9_32_36,"WIFI:info:Manager:%s netid        :%0x")
TRACE_MSG(MMIWLAN_MANAGER_1203_112_2_18_3_9_32_37,"WIFI:error:%s not find profile_ptr!")
TRACE_MSG(MMIWLAN_MANAGER_1268_112_2_18_3_9_32_38,"WIFI:error:Manager:%s wifi_on_cnf_ptr is PNULL")
TRACE_MSG(MMIWLAN_MANAGER_1295_112_2_18_3_9_32_39,"WIFI:info:%s scan indication hide SSID AP")
TRACE_MSG(MMIWLAN_MANAGER_1300_112_2_18_3_9_32_40,"WIFI:info:Manager:%s ESSID =%s")
TRACE_MSG(MMIWLAN_MANAGER_1301_112_2_18_3_9_32_41,"WIFI:info:Manager:%s network_mode =%d")
TRACE_MSG(MMIWLAN_MANAGER_1302_112_2_18_3_9_32_42,"WIFI:info:Manager:%s encryp_protocol =%d")
TRACE_MSG(MMIWLAN_MANAGER_1303_112_2_18_3_9_32_43,"WIFI:info:Manager:%s credential_type =%d")
TRACE_MSG(MMIWLAN_MANAGER_1304_112_2_18_3_9_32_44,"WIFI:info:Manager:%s chanel =%d")
TRACE_MSG(MMIWLAN_MANAGER_1305_112_2_18_3_9_32_45,"WIFI:info:Manager:%s pairwise_cipher =%d")
TRACE_MSG(MMIWLAN_MANAGER_1306_112_2_18_3_9_32_46,"WIFI:info:Manager:%s group_cipher =%d")
TRACE_MSG(MMIWLAN_MANAGER_1307_112_2_18_3_9_32_47,"WIFI:info:Manager:%s rssi =%d")
TRACE_MSG(MMIWLAN_MANAGER_1308_112_2_18_3_9_32_48,"WIFI:info:Manager:%s rate =%d")
TRACE_MSG(MMIWLAN_MANAGER_1353_112_2_18_3_9_32_49,"WIFI:info:%s Not Update AP info for weak rssi, profile.rssi is %d")
TRACE_MSG(MMIWLAN_MANAGER_1358_112_2_18_3_9_32_50,"WIFI:info:%s Update AP info")
TRACE_MSG(MMIWLAN_MANAGER_1440_112_2_18_3_9_32_51,"WIFI:info:%s New AP info added")
TRACE_MSG(MMIWLAN_MANAGER_1456_112_2_18_3_9_32_52,"mmiwifi error %s: AddProfileToList failed!")
TRACE_MSG(MMIWLAN_MANAGER_1465_112_2_18_3_9_32_53,"mmiwifi error %s: AddProfileToList failed!")
TRACE_MSG(MMIWLAN_MANAGER_1522_112_2_18_3_9_32_54,"WIFI:info:MMI:ip address   - %s")
TRACE_MSG(MMIWLAN_MANAGER_1523_112_2_18_3_9_32_55,"WIFI:info:MMI:subnet mask  - %s")
TRACE_MSG(MMIWLAN_MANAGER_1524_112_2_18_3_9_32_56,"WIFI:info:MMI:gateway      - %s")
TRACE_MSG(MMIWLAN_MANAGER_1525_112_2_18_3_9_32_57,"WIFI:info:MMI:first dns    - %s")
TRACE_MSG(MMIWLAN_MANAGER_1526_112_2_18_3_9_32_58,"WIFI:info:MMI:second dns   - %s")
TRACE_MSG(MMIWLAN_MANAGER_1531_112_2_18_3_9_32_59,"mmiwifi error T_P_APP WifiHandleSuppSigConnectCnf: TCPIP_SetIpAddress failed(%d)!")
TRACE_MSG(MMIWLAN_MANAGER_1613_112_2_18_3_9_33_60,"WIFI:error:Manager:%s profile_ptr is PNULL!")
TRACE_MSG(MMIWLAN_MANAGER_1646_112_2_18_3_9_33_61,"WIFI:info:Manager:%s profile_ptr is PNULL")
TRACE_MSG(MMIWLAN_MANAGER_1737_112_2_18_3_9_33_62,"WLAN new_siganl = %d , old_singal = %d")
TRACE_MSG(MMIWLAN_MANAGER_1759_112_2_18_3_9_33_63,"WLAN profile bssid = %s, rssi bssid = %s,ssid = %s,old_sig = %d,new_sig = %d ")
TRACE_MSG(MMIWLAN_MANAGER_1762_112_2_18_3_9_33_64,"WLAN found connected bssid equal")
TRACE_MSG(MMIWLAN_MANAGER_1853_112_2_18_3_9_33_65,"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_1887_112_2_18_3_9_33_66,"\n mmiwifi dispatch WIFISUPP_SIG_CONNECT_CNF, result = %d")
TRACE_MSG(MMIWLAN_MANAGER_1898_112_2_18_3_9_33_67,"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_1932_112_2_18_3_9_33_68,"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND")
TRACE_MSG(MMIWLAN_MANAGER_1941_112_2_18_3_9_33_69,"\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_1950_112_2_18_3_9_33_70,"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_1983_112_2_18_3_9_33_71,"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND")
TRACE_MSG(MMIWLAN_MANAGER_1992_112_2_18_3_9_33_72,"\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_2001_112_2_18_3_9_33_73,"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_2035_112_2_18_3_9_33_74,"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND")
TRACE_MSG(MMIWLAN_MANAGER_2044_112_2_18_3_9_34_75,"\n mmiwifi dispatch WIFISUPP_SIG_BSSID_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_2053_112_2_18_3_9_34_76,"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_2087_112_2_18_3_9_34_77,"\n mmiwifi dispatch WIFISUPP_SIG_DISCONNECT_IND")
TRACE_MSG(MMIWLAN_MANAGER_2097_112_2_18_3_9_34_78,"\n mmiwifi dispatch WIFISUPP_SIG_RSSI_CHANGED_IND")
TRACE_MSG(MMIWLAN_MANAGER_2234_112_2_18_3_9_34_79,"MMIWIFI_AutoConnect ")
TRACE_MSG(MMIWLAN_MANAGER_2238_112_2_18_3_9_34_80,"MMIWIFI_AutoConnect : Fly Mode or OFF")
TRACE_MSG(MMIWLAN_MANAGER_2244_112_2_18_3_9_34_81,"Index Name   ON_AIR TRUSTED STATUS AUTO_CON")
TRACE_MSG(MMIWLAN_MANAGER_2251_112_2_18_3_9_34_82,"%d    %s    %d    %d    %d    %d  ")
TRACE_MSG(MMIWLAN_MANAGER_2260_112_2_18_3_9_34_83,"MMIWIFI_AutoConnect %s is connecting or connected")
TRACE_MSG(MMIWLAN_MANAGER_2274_112_2_18_3_9_34_84,"MMIWIFI_AutoConnect %s is connecting or connected from_index = %d")
TRACE_MSG(MMIWLAN_MANAGER_2297_112_2_18_3_9_34_85,"MMIWIFI_AutoConnect Connect -> %s")
TRACE_MSG(MMIWLAN_MANAGER_2321_112_2_18_3_9_34_86,"MMIWIFI_StartAutoScan Wlan Or AutoScan OFF")
TRACE_MSG(MMIWLAN_MANAGER_2356_112_2_18_3_9_34_87,"MMIWIFI_StartAutoScan autoscan_status = %d")
TRACE_MSG(MMIWLAN_MANAGER_2374_112_2_18_3_9_34_88,"MMIWIFI_StopAutoScan")
TRACE_MSG(MMIWLAN_MANAGER_2384_112_2_18_3_9_34_89,"MMIWIFI_StartAutoScan Wlan Or AutoScan OFF")
TRACE_MSG(MMIWLAN_MANAGER_2417_112_2_18_3_9_34_90,"MMIWIFI_IsConnecting ret = %d")
TRACE_MSG(MMIWLAN_NV_171_112_2_18_3_9_35_91,"MMIWIFI_GetIsWlanOn is_on = %d")
TRACE_MSG(MMIWLAN_NV_772_112_2_18_3_9_36_92,"MMIWIFI_SetServiceProvider No mem")
TRACE_MSG(MMIWLAN_NV_824_112_2_18_3_9_37_93,"MMIWIFI_SetServiceProvider No mem")
TRACE_MSG(MMIWLAN_NV_876_112_2_18_3_9_37_94,"MMIWIFI_SetServiceProvider No mem")
TRACE_MSG(MMIWLAN_NV_928_112_2_18_3_9_37_95,"MMIWIFI_SetServiceProvider No mem")
TRACE_MSG(MMIWLAN_NV_980_112_2_18_3_9_37_96,"MMIWIFI_SetServiceProvider No mem")
TRACE_MSG(MMIWLAN_NV_1032_112_2_18_3_9_37_97,"MMIWIFI_SetServiceProvider No mem")
TRACE_MSG(MMIWLAN_NV_1084_112_2_18_3_9_37_98,"MMIWIFI_SetServiceProvider No mem")
TRACE_MSG(MMIWLAN_PORTAL_1645_112_2_18_3_9_41_99,"HandleWlanPortalTimer timer_id=%d, s_wlan_context.status_timer_id=%d")
TRACE_MSG(MMIWLAN_PORTAL_1715_112_2_18_3_9_41_100,"WlanMsgPortalCallback event_id=%d,result=%d,s_wlan_context.portal_status=%d")
TRACE_MSG(MMIWLAN_PORTAL_1932_112_2_18_3_9_41_101,"ImplementWlanOperation s_wlan_context.portal_status=%d")
TRACE_MSG(MMIWLAN_PORTAL_2010_112_2_18_3_9_41_102,"HandleCMCCAutoLoginQueryWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWLAN_PORTAL_2068_112_2_18_3_9_41_103,"WIFI_OpenPortalWin win_type= %d, s_wlan_context.portal_status=%d")
TRACE_MSG(MMIWLAN_PORTAL_2109_112_2_18_3_9_41_104,"MMIWIFI_CMCCAutoLogin login=%d, notify=%d, s_wlan_context.portal_status=%d")
TRACE_MSG(MMIWLAN_PORTAL_2134_112_2_18_3_9_41_105,"MMIWIFI_CMCCLogoff status = %d")
TRACE_MSG(MMIWLAN_PORTAL_2210_112_2_18_3_9_42_106,"MMIWIFI_Update File_handle is NULL")
TRACE_MSG(MMIWLAN_PORTAL_2219_112_2_18_3_9_42_107,"MMIWIFI_Update File size Incorrect(%d)")
TRACE_MSG(MMIWLAN_PORTAL_2228_112_2_18_3_9_42_108,"MMIWIFI_Update No mem")
TRACE_MSG(MMIWLAN_PORTAL_2236_112_2_18_3_9_42_109,"MMIWIFI_Update Read File error")
TRACE_MSG(MMIWLAN_PORTAL_2295_112_2_18_3_9_42_110,"MMIWIFI_HandlePortalLogin login_res=%d")
TRACE_MSG(MMIWLAN_PORTAL_2341_112_2_18_3_9_42_111,"MMIWIFI_HandlePortalOffline offline_res=%d")
TRACE_MSG(MMIWLAN_PORTAL_2569_112_2_18_3_9_42_112,"wifi_ParseXML Bad param")
TRACE_MSG(MMIWLAN_PORTAL_NET_492_112_2_18_3_9_44_113,"HandleWlanPortalPsMsg() enter msg_id= %d")
TRACE_MSG(MMIWLAN_PORTAL_NET_498_112_2_18_3_9_44_114,"HandleWlanPortalPsMsg: MMIPDP_ACTIVE_CNF nsapi=0x%x")
TRACE_MSG(MMIWLAN_PORTAL_NET_528_112_2_18_3_9_44_115,"HandleWlanPortalPsMsg: MMIPDP_DEACTIVE_CNF")
TRACE_MSG(MMIWLAN_PORTAL_NET_538_112_2_18_3_9_44_116,"HandleWlanPortalPsMsg: MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_575_112_2_18_3_9_44_117,"PortalRequest Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_584_112_2_18_3_9_44_118,"PortalRequest method=%d, url_len=%d, body_len=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_652_112_2_18_3_9_44_119,"PortalRequest url error")
TRACE_MSG(MMIWLAN_PORTAL_NET_664_112_2_18_3_9_45_120,"PortalRequest method=%d, url_len=%d, body_len=%d, portal_url_ptr=%s")
TRACE_MSG(MMIWLAN_PORTAL_NET_676_112_2_18_3_9_45_121,"PortalRequest no memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_748_112_2_18_3_9_45_122,"PortalRequest no memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_793_112_2_18_3_9_45_123,"PortalRequest no memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_880_112_2_18_3_9_45_124,"HandlePortalHttpInitCnf not Portal task")
TRACE_MSG(MMIWLAN_PORTAL_NET_1023_112_2_18_3_9_45_125,"HandlePortalHttpDataInd recv_len=%d, total_len=%d, data_len=%d, response_code=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1109_112_2_18_3_9_46_126,"HandlePortalHttpHeaderInd response_code=%d, content_len=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1146_112_2_18_3_9_46_127,"HandlePortalHttpHeaderInd No Memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_1340_112_2_18_3_9_46_128,"HandlePortalHttpRedirectInd cur_page_url_ptr=%s")
TRACE_MSG(MMIWLAN_PORTAL_NET_1387_112_2_18_3_9_46_129,"JXC wlan login type=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1411_112_2_18_3_9_46_130,"HandlePortalHttpPostCnf login_res=%d, offline_res=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1510_112_2_18_3_9_47_131,"HandlePortalHttpPostCnf url = %d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1595_112_2_18_3_9_47_132,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_INIT_CNF")
TRACE_MSG(MMIWLAN_PORTAL_NET_1600_112_2_18_3_9_47_133,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1605_112_2_18_3_9_47_134,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1610_112_2_18_3_9_47_135,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_GET_CNF")
TRACE_MSG(MMIWLAN_PORTAL_NET_1615_112_2_18_3_9_47_136,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_HEAD_CNF")
TRACE_MSG(MMIWLAN_PORTAL_NET_1620_112_2_18_3_9_47_137,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_DATA_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1625_112_2_18_3_9_47_138,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_HEADER_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1630_112_2_18_3_9_47_139,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_ERROR_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1635_112_2_18_3_9_47_140,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_CLOSE_CNF")
TRACE_MSG(MMIWLAN_PORTAL_NET_1640_112_2_18_3_9_47_141,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_CANCEL_CNF")
TRACE_MSG(MMIWLAN_PORTAL_NET_1645_112_2_18_3_9_47_142,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_NET_PROG_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1650_112_2_18_3_9_47_143,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_REDIRECT_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1655_112_2_18_3_9_47_144,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_POST_CNF")
TRACE_MSG(MMIWLAN_PORTAL_NET_1661_112_2_18_3_9_47_145,"MMIWIFI_HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND")
TRACE_MSG(MMIWLAN_PORTAL_NET_1667_112_2_18_3_9_47_146,"MMIWIFI_HandleHttpMsg Not handle msg_id = %x")
TRACE_MSG(MMIWLAN_PORTAL_NET_1689_112_2_18_3_9_47_147,"StartPortalHttpRequest No Memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_1717_112_2_18_3_9_47_148,"StartPortalHttpRequest http_error=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1743_112_2_18_3_9_47_149,"MMIWIFI_WlanPortalNetConnect status=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1759_112_2_18_3_9_47_150,"WlanPortalNetConnect:MMIAPIPDP_Active fail")
TRACE_MSG(MMIWLAN_PORTAL_NET_1816_112_2_18_3_9_47_151,"WlanPortalFindActionUrl Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_1835_112_2_18_3_9_47_152,"WlanPortalFindActionUrl Find Myform")
TRACE_MSG(MMIWLAN_PORTAL_NET_1867_112_2_18_3_9_47_153,"WlanPortalFindActionUrl Find LoginForm")
TRACE_MSG(MMIWLAN_PORTAL_NET_1887_112_2_18_3_9_47_154,"JXC WlanPortalFindActionUrl urlptr=%s,len=%d,isloginform=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_1907_112_2_18_3_9_47_155,"WlanPortalFindNameValue Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_1916_112_2_18_3_9_47_156,"WlanPortalFindNameValue Find!")
TRACE_MSG(MMIWLAN_PORTAL_NET_1948_112_2_18_3_9_48_157,"WlanPortalFindMethod Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_2003_112_2_18_3_9_48_158,"WlanPortalLoginRes Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_2017_112_2_18_3_9_48_159,"WlanPortalLoginRes res_num=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_2085_112_2_18_3_9_48_160,"WlanPortalOfflineRes Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_2099_112_2_18_3_9_48_161,"WlanPortalOfflineRes res_num=%d")
TRACE_MSG(MMIWLAN_PORTAL_NET_2212_112_2_18_3_9_48_162,"WlanPortalParseXhtmlData Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_2267_112_2_18_3_9_48_163,"WlanPortalParseXhtmlData No Memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_2426_112_2_18_3_9_49_164,"WlanPortalParseXhtmlData *body_len_ptr=%d, temp_body_len=%d, url=%s, body=%s")
TRACE_MSG(MMIWLAN_PORTAL_NET_2430_112_2_18_3_9_49_165,"WlanPortalParseXhtmlData No Memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_2486_112_2_18_3_9_49_166,"WlanPortalParseLoginSuccessData Input Invalid")
TRACE_MSG(MMIWLAN_PORTAL_NET_2530_112_2_18_3_9_49_167,"WlanPortalParseXhtmlData No Memory")
TRACE_MSG(MMIWLAN_PORTAL_NET_2574_112_2_18_3_9_49_168,"WlanPortalParseLoginSuccessData *body_len_ptr=%d, temp_body_len=%d, url=%s, body=%s")
TRACE_MSG(MMIWLAN_PORTAL_NET_2578_112_2_18_3_9_49_169,"WlanPortalParseLoginSuccessData No Memory")
TRACE_MSG(MMIWLAN_PROFILE_717_112_2_18_3_9_50_170,"MMIWIFI_FindProfileByAP")
TRACE_MSG(MMIWLAN_PROFILE_751_112_2_18_3_9_51_171,"MMIWIFI_FindProfileByAP: status = %d")
TRACE_MSG(MMIWLAN_PROFILE_1055_112_2_18_3_9_51_172,"MMIWIFI_FindProfileByAP")
TRACE_MSG(MMIWLAN_PROFILE_1080_112_2_18_3_9_51_173,"MMIAPIWIFI_APIsAvailable")
TRACE_MSG(MMIWLAN_PROFILE_1141_112_2_18_3_9_51_174,"MMIWIFI_IsCMCCAvailable")
TRACE_MSG(MMIWLAN_PROFILE_1160_112_2_18_3_9_51_175,"MMIWIFI_GetCMCCProfile")
TRACE_MSG(MMIWLAN_WIN_3303_112_2_18_3_9_58_176,"WIFI:error:%s Profile Initialize faild!")
TRACE_MSG(MMIWLAN_WIN_3379_112_2_18_3_9_58_177,"MMIWIFI_OpenSelectServiceWin reselect = %d")
TRACE_MSG(MMIWLAN_WIN_3390_112_2_18_3_9_58_178,"MMIWIFI_OpenSelectServiceWin No Profile exist")
TRACE_MSG(MMIWLAN_WIN_3398_112_2_18_3_9_58_179,"MMIWIFI_OpenSelectServiceWin s_last_tick_time = %ld, tick_time = %ld")
TRACE_MSG(MMIWLAN_WIN_3484_112_2_18_3_9_59_180,"WifiCreateListPopMenu window already opened !")
TRACE_MSG(MMIWLAN_WIN_3726_112_2_18_3_9_59_181,"WIFI:info:MMI recv MMIWIFI_MSG_ON_CNF, result is %d")
TRACE_MSG(MMIWLAN_WIN_3761_112_2_18_3_9_59_182,"WIFI:info:MMI recv MMIWIFI_MSG_OFF_CNF, result is %d")
TRACE_MSG(MMIWLAN_WIN_3801_112_2_18_3_9_59_183,"WIFI:info:MMI recv MMIWIFI_MSG_SCAN_CNF, result is %d")
TRACE_MSG(MMIWLAN_WIN_3826_112_2_18_3_9_59_184,"WIFI:info:MMI recv MMIWIFI_MSG_SCAN_BEGIN_IND sig")
TRACE_MSG(MMIWLAN_WIN_3851_112_2_18_3_9_59_185,"\n mmiwifi qq recv MMIWIFI_MSG_SCAN_IND")
TRACE_MSG(MMIWLAN_WIN_3861_112_2_18_3_9_59_186,"WIFI:info:MMI recv MMIWIFI_MSG_CONNECT_CNF, result is %d")
TRACE_MSG(MMIWLAN_WIN_3865_112_2_18_3_9_59_187,"\n mmiwifi error MMIWIFI_MSG_CONNECT_CNF profile_ptr is PNULL!")
TRACE_MSG(MMIWLAN_WIN_3945_112_2_18_3_10_0_188,"WIFI:info:MMI:recv MMIWIFI_MSG_DISCONNECT_CNF, result is %d")
TRACE_MSG(MMIWLAN_WIN_3962_112_2_18_3_10_0_189,"WIFI:info:MMI:recv MMIWIFI_MSG_DISCONNECT_IND")
TRACE_MSG(MMIWLAN_WIN_3985_112_2_18_3_10_0_190,"WIFI:info:MMI recv MMIWIFI_MSG_DHCP_CNF, result is %d")
TRACE_MSG(MMIWLAN_WIN_4009_112_2_18_3_10_0_191,"WIFI:error:MMI recv MMIWIFI_MSG_DHCP_CNF, but profile_ptr is PNULL!")
TRACE_MSG(MMIWLAN_WIN_4041_112_2_18_3_10_0_192,"WIFI:info:MMI recv MMIWIFI_MSG_BSSID_CHANGED_IND")
TRACE_MSG(MMIWLAN_WIN_4046_112_2_18_3_10_0_193,"WIFI:info:MMI recv MMIWIFI_MSG_RSSI_CHANGED_IND new ssi is %d")
TRACE_MSG(MMIWLAN_WIN_4082_112_2_18_3_10_0_194,"WifiHandleListWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_4141_112_2_18_3_10_0_195,"WifiHandleListWinMsg wlan_on = %d, wifi_on = %d")
TRACE_MSG(MMIWLAN_WIN_4267_112_2_18_3_10_0_196,"mmipdp error %s: profile_ptr is PNULL!")
TRACE_MSG(MMIWLAN_WIN_4285_112_2_18_3_10_0_197,"mmipdp error %s: profile_ptr is PNULL!")
TRACE_MSG(MMIWLAN_WIN_4425_112_2_18_3_10_1_198,"WifiAppendListItem ESSID =%s order_index = %d")
TRACE_MSG(MMIWLAN_WIN_4477_112_2_18_3_10_1_199,"WifiAppendSelectAPListItem Error ")
TRACE_MSG(MMIWLAN_WIN_6609_112_2_18_3_10_5_200,"WifiHandleAuthSettingWAPICERTIFICATEWinMsg profile_ptr is Error index = %d")
TRACE_MSG(MMIWLAN_WIN_7786_112_2_18_3_10_8_201,"OpenSelectClientCertificateFileWinCallBack array size == 0")
TRACE_MSG(MMIWLAN_WIN_7791_112_2_18_3_10_8_202,"OpenSelectClientCertificateFileWinCallBack failed")
TRACE_MSG(MMIWLAN_WIN_7826_112_2_18_3_10_8_203,"OpenSelectClientCertificateFileForConnectWinCallBack array size == 0")
TRACE_MSG(MMIWLAN_WIN_7831_112_2_18_3_10_8_204,"OpenSelectClientCertificateFileForConnectWinCallBack failed")
TRACE_MSG(MMIWLAN_WIN_7860_112_2_18_3_10_8_205,"OpenSelectParentCertificateFileWinCallBack array size == 0")
TRACE_MSG(MMIWLAN_WIN_7865_112_2_18_3_10_8_206,"OpenSelectParentCertificateFileWinCallBack failed")
TRACE_MSG(MMIWLAN_WIN_7900_112_2_18_3_10_8_207,"OpenSelectParentCertificateFileForconnectWinCallBack array size == 0")
TRACE_MSG(MMIWLAN_WIN_7905_112_2_18_3_10_8_208,"OpenSelectParentCertificateFileForconnectWinCallBack failed")
TRACE_MSG(MMIWLAN_WIN_8863_112_2_18_3_10_11_209,"MMIWIFI_OpenMainWin in_focus_win = %d")
TRACE_MSG(MMIWLAN_WIN_8912_112_2_18_3_10_11_210,"MMIWIFI_OpenMainWin Bad param")
TRACE_MSG(MMIWLAN_WIN_8935_112_2_18_3_10_11_211,"MMIWIFI_OpenDefAPSettingWin ")
TRACE_MSG(MMIWLAN_WIN_8961_112_2_18_3_10_11_212,"WifiHandleSettinglistChildWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9182_112_2_18_3_10_12_213,"WifiHandleAPEAPSimIndexWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9610_112_2_18_3_10_12_214,"WifiHandleSearchWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9669_112_2_18_3_10_13_215,"WifiHandleWpaStateSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9723_112_2_18_3_10_13_216,"WifiHandleAPEditWpaStateSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9771_112_2_18_3_10_13_217,"WifiHandleAPNetworkStateWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9816_112_2_18_3_10_13_218,"WifiHandleAPAuthTypeWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9860_112_2_18_3_10_13_219,"WifiHandleAPConnectStatetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9904_112_2_18_3_10_13_220,"WifiHandleWapiStateSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_9957_112_2_18_3_10_13_221,"WifiHandleAPEditWapiStateSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10004_112_2_18_3_10_13_222,"WifiHandleAPEditWapiTypeWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10051_112_2_18_3_10_13_223,"WifiHandleWepStateSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10106_112_2_18_3_10_13_224,"WifiHandleWepEncyptSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10160_112_2_18_3_10_13_225,"WifiHandleWepModeSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10212_112_2_18_3_10_14_226,"WifiHandleWepKeyIndexSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10265_112_2_18_3_10_14_227,"WifiHandleWepStateSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10316_112_2_18_3_10_14_228,"WifiHandleWepEncyptSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10365_112_2_18_3_10_14_229,"WifiHandleWepModeSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10413_112_2_18_3_10_14_230,"WifiHandleWepKeyIndexSetWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10462_112_2_18_3_10_14_231,"WifiHandleRejectListPopWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10543_112_2_18_3_10_14_232,"WifiHandleApListPopWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10562_112_2_18_3_10_14_233,"MMIWIFI WifiHandleApListPopWinMsg menu_id=%d")
TRACE_MSG(MMIWLAN_WIN_10594_112_2_18_3_10_14_234,"MMIWIFI_Connect Scaning")
TRACE_MSG(MMIWLAN_WIN_10705_112_2_18_3_10_15_235,"WifiHandleApTrustListPopWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_10757_112_2_18_3_10_15_236,"MMIWIFI WifiHandlelistPopMenuWinMsg MMIWIFI_POPMENU_ID_DELETE index=%d,profile_ptr=%d")
TRACE_MSG(MMIWLAN_WIN_10888_112_2_18_3_10_15_237,"WifiSetApEditWAPIWin called")
TRACE_MSG(MMIWLAN_WIN_11023_112_2_18_3_10_15_238,"WifiHandleAPEditWAPIWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_11198_112_2_18_3_10_16_239,"WifiHandleAuthSettingWAPICERTIFICATEWinMsg profile_ptr is Error index = %d")
TRACE_MSG(MMIWLAN_WIN_11234_112_2_18_3_10_16_240,"WifiHandleEditAPSSIDWinMsg win_id=%d")
TRACE_MSG(MMIWLAN_WIN_11325_112_2_18_3_10_16_241,"WIFI:%s ESSID =%s")
TRACE_MSG(MMIWLAN_WIN_11490_112_2_18_3_10_16_242,"HandleAPPropertyAlertWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_11575_112_2_18_3_10_16_243,"MMIWIFI_Connect Scaning")
TRACE_MSG(MMIWLAN_WIN_11632_112_2_18_3_10_16_244,"HandleQueryEditConnectedAP msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_11748_112_2_18_3_10_17_245,"WifiHandleAPEditAuthSetting auth_type = %x")
TRACE_MSG(MMIWLAN_WIN_11922_112_2_18_3_10_17_246,"WifiHandleGPRSToWLANWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_11988_112_2_18_3_10_17_247,"WifiHandleGPRSToWLANWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_12055_112_2_18_3_10_17_248,"WifiHandleGPRSToWLANWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_12536_112_2_18_3_10_18_249,"WifiHandleDefAPSettingWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_12612_112_2_18_3_10_18_250,"WifiHandleHandleMenuAction node_id = %x")
TRACE_MSG(MMIWLAN_WIN_12957_112_2_18_3_10_19_251,"WifiHandleWlanSettingWin win_id=%d")
TRACE_MSG(MMIWLAN_WIN_13161_112_2_18_3_10_19_252,"WifiSetWlanWindow called")
TRACE_MSG(MMIWLAN_WIN_13340_112_2_18_3_10_20_253,"WIFI:info:MMI:ip address   - %s")
TRACE_MSG(MMIWLAN_WIN_13341_112_2_18_3_10_20_254,"WIFI:info:MMI:subnet mask  - %s")
TRACE_MSG(MMIWLAN_WIN_13342_112_2_18_3_10_20_255,"WIFI:info:MMI:gateway      - %s")
TRACE_MSG(MMIWLAN_WIN_13343_112_2_18_3_10_20_256,"WIFI:info:MMI:first dns    - %s")
TRACE_MSG(MMIWLAN_WIN_13344_112_2_18_3_10_20_257,"WIFI:info:MMI:second dns   - %s")
TRACE_MSG(MMIWLAN_WIN_13349_112_2_18_3_10_20_258,"mmiwifi error T_P_APP WifiHandleSuppSigConnectCnf: TCPIP_SetIpAddress failed(%d)!")
TRACE_MSG(MMIWLAN_WIN_13410_112_2_18_3_10_20_259,"WLAN port is %d")
TRACE_MSG(MMIWLAN_WIN_13434_112_2_18_3_10_20_260,"WifiHandleWlanSettingWin win_id=%d")
TRACE_MSG(MMIWLAN_WIN_13578_112_2_18_3_10_20_261,"WifiSetOwnAttributeWindow called")
TRACE_MSG(MMIWLAN_WIN_13805_112_2_18_3_10_21_262,"WifiHandleProxySettingWin win_id=%d")
TRACE_MSG(MMIWLAN_WIN_13927_112_2_18_3_10_21_263,"WifiHandleProxyRejectListWin win_id=%d")
TRACE_MSG(MMIWLAN_WIN_14038_112_2_18_3_10_21_264,"WifiHandleProxyRejectListPopMenuWin win_id=%d")
TRACE_MSG(MMIWLAN_WIN_14167_112_2_18_3_10_22_265,"WifiHandleProxyRejectListPopMenuWin win_id=%d")
TRACE_MSG(MMIWLAN_WIN_14267_112_2_18_3_10_22_266,"WifiHandleProxyRejectListPopMenuWin win_id=%d")
TRACE_MSG(MMIWLAN_WIN_14445_112_2_18_3_10_22_267,"WifiSetProxySettingWindow called")
TRACE_MSG(MMIWLAN_WIN_14641_112_2_18_3_10_23_268,"WifiSetEditAPWindow called")
TRACE_MSG(MMIWLAN_WIN_14867_112_2_18_3_10_23_269,"WifiSetEditWEPWindow called")
TRACE_MSG(MMIWLAN_WIN_15077_112_2_18_3_10_23_270,"WifiSetEditWEPWindow called")
TRACE_MSG(MMIWLAN_WIN_15222_112_2_18_3_10_24_271,"WifiSetEditWPAWPA2Window called")
TRACE_MSG(MMIWLAN_WIN_15352_112_2_18_3_10_24_272,"WifiSetEditWPAWPA2Window called")
TRACE_MSG(MMIWLAN_WIN_15409_112_2_18_3_10_24_273,"WifiSetEditWAPIWindow called")
TRACE_MSG(MMIWLAN_WIN_15466_112_2_18_3_10_24_274,"WifiSetEditWPAWPA2Window called")
TRACE_MSG(MMIWLAN_WIN_15535_112_2_18_3_10_25_275,"WifiSetEditWPAWPA2Window called")
TRACE_MSG(MMIWLAN_WIN_15591_112_2_18_3_10_25_276,"WifiSetEditWAPICertificateWindow called")
TRACE_MSG(MMIWLAN_WIN_15626_112_2_18_3_10_25_277,"WifiSetEditWAPICertificateForConnectWindow called")
TRACE_MSG(MMIWLAN_WIN_15689_112_2_18_3_10_25_278,"MMIAPIWIFI_IsExistInExcludingList result is %d")
TRACE_MSG(MMIWLAN_WIN_15825_112_2_18_3_10_25_279,"MMIWIFI_ReSelectAP")
TRACE_MSG(MMIWLAN_WIN_15831_112_2_18_3_10_25_280,"MMIWIFI_ReSelectAP MMIWIFI_AutoConnect")
TRACE_MSG(MMIWLAN_WIN_15856_112_2_18_3_10_25_281,"MMIWIFI_ChangeServiceToGPRS")
TRACE_MSG(MMIWLAN_WIN_15862_112_2_18_3_10_25_282,"MMIWIFI_ChangeServiceToGPRS Query")
TRACE_MSG(MMIWLAN_WIN_15882_112_2_18_3_10_25_283,"WifiHandleListWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_15948_112_2_18_3_10_25_284,"MMIWIFI_IsNeedServiceChange ret = %d")
TRACE_MSG(MMIWLAN_WIN_15968_112_2_18_3_10_25_285,"MMIWIFI_HandleChangeService msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_16313_112_2_18_3_10_26_286,"WifiListIsAPItemSelected index = %d, ret = %d")
TRACE_MSG(MMIWLAN_WIN_16395_112_2_18_3_10_26_287,"WifiHandleMainParentWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_16438_112_2_18_3_10_26_288,"WifiHandleTrustedlistChildWinMsg msg_id = %x")
TRACE_MSG(MMIWLAN_WIN_16687_112_2_18_3_10_27_289,"mmipdp error %s: profile_ptr is PNULL!")
TRACE_MSG(MMIWLAN_WIN_16798_112_2_18_3_10_27_290,"MMIWIFI WifiHandlelistPopMenuWinMsg MMIWIFI_POPMENU_ID_DELETE index=%d,profile_ptr=%d")
TRACE_MSG(MMIWLAN_WIN_16945_112_2_18_3_10_27_291,"WifiCreateListMenu window already opened !")
END_TRACE_MAP(MMI_APP_WLAN_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_WLAN_TRC_H_

