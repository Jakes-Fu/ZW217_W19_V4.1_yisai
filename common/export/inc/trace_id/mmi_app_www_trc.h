/******************************************************************************
 ** File Name:      mmi_app_www_trc.h                                         *
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
//trace_id:243
#ifndef _MMI_APP_WWW_TRC_H_
#define _MMI_APP_WWW_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIWWW_156_112_2_18_3_11_27_0 "MMIAPIWWW_StartWWW start type=%d,is_connection_customized=%d,connection_index=%d"
#define MMIWWW_433_112_2_18_3_11_27_1 "\"\"  MMIWWW_SetFreq   \"#"
#define MMIWWW_450_112_2_18_3_11_27_2 "\"\"  MMIWWW_SetFreq   \"#"
#define MMIWWW_461_112_2_18_3_11_27_3 "MMIWWW_Initialize"
#define MMIWWW_501_112_2_18_3_11_27_4 "MMIWWW_Finalize"
#define MMIWWW_529_112_2_18_3_11_28_5 "WWWApplet_HandleEvent msg_id = 0x%04x"
#define MMIWWW_615_112_2_18_3_11_28_6 "MMIWWW_NetworkIsReady=%d"
#define MMIWWW_696_112_2_18_3_11_28_7 "MMIWWW_SetPDPState:old %d-new %d"
#define MMIWWW_724_112_2_18_3_11_28_8 "MMIWWW_NetConnectEx err dual_sys:%d"
#define MMIWWW_730_112_2_18_3_11_28_9 "MMIWWW_NetConnectEx:MMIAPIPHONE_IsSimOk not OK"
#define MMIWWW_757_112_2_18_3_11_28_10 "MMIWWW_NetConnectEx:MMIAPIPDP_Active fail"
#define MMIWWW_775_112_2_18_3_11_28_11 "MMIWWW_NetConnect pdp_state=%d"
#define MMIWWW_786_112_2_18_3_11_28_12 "MMIWWW_NetConnect PDP Connecting now..."
#define MMIWWW_798_112_2_18_3_11_28_13 "MMIWWW_NetDisconnect directly"
#define MMIWWW_799_112_2_18_3_11_28_14 "MMIWWW_NetDisconnect is_ready = %d"
#define MMIWWW_807_112_2_18_3_11_28_15 "MMIWWW_NetDisconnect..."
#define MMIWWW_831_112_2_18_3_11_28_16 "HandleWWWPDPActiveCnf() msg_ptr->result=%d"
#define MMIWWW_860_112_2_18_3_11_28_17 "HandleWWWPDPDeactiveCnf() msg_ptr->result=%d"
#define MMIWWW_885_112_2_18_3_11_28_18 "HandleWWWPDPDeactiveInd() msg_ptr->result=%d"
#define MMIWWW_917_112_2_18_3_11_28_19 "HandleWWWPsMsg receive SOCKET_ASYNC_GETHOSTBYNAME_CNF"
#define MMIWWW_946_112_2_18_3_11_28_20 "HandleWWWPDPMsg() enter msg_id= %d"
#define MMIWWW_952_112_2_18_3_11_28_21 "HandleWWWPDPMsg: MMIPDP_ACTIVE_CNF"
#define MMIWWW_960_112_2_18_3_11_28_22 "HandleWWWPDPMsg: MMIPDP_DEACTIVE_CNF"
#define MMIWWW_968_112_2_18_3_11_28_23 "HandleWWWPDPMsg: MMIPDP_DEACTIVE_IND"
#define MMIWWW_1007_112_2_18_3_11_28_24 "MMIAPIWWW_HandleWWWTimer:FALSE"
#define MMIWWW_1030_112_2_18_3_11_29_25 "MMIAPIWWW_HandleWWWTimer:TRUE"
#define MMIWWW_1034_112_2_18_3_11_29_26 "MMIAPIWWW_HandleWWWTimer:FALSE 22"
#define MMIWWW_BOOKMARK_159_112_2_18_3_11_29_27 "RenameFile full_path_len %d new_file_name_len %d !!"
#define MMIWWW_BOOKMARK_553_112_2_18_3_11_30_28 "MMIWWW_UpdateBookmarkTotalNum bookmark_total_num=%d"
#define MMIWWW_BOOKMARK_577_112_2_18_3_11_30_29 "MMIWWW_Bookmark_Initialize bookmark_num=%d"
#define MMIWWW_BOOKMARK_1157_112_2_18_3_11_31_30 "MMIWWW_ChangeBookmarkUrl in_index = %d"
#define MMIWWW_BOOKMARK_1187_112_2_18_3_11_31_31 "MMIWWW_ChangeBookmarkUrl buf=%s"
#define MMIWWW_BOOKMARK_1193_112_2_18_3_11_31_32 "MMIWWW_ChangeBookmarkUrl result = %d"
#define MMIWWW_BOOKMARK_1370_112_2_18_3_11_32_33 "MMIWWW_Bookmark_GotoURL: url_len=%d, url_ptr= %s"
#define MMIWWW_BOOKMARK_1411_112_2_18_3_11_32_34 "MMIWWW_SwapBookmark Bad Param"
#define MMIWWW_BOOKMARK_1420_112_2_18_3_11_32_35 "MMIWWW_SwapBookmark get %d's title failed "
#define MMIWWW_BOOKMARK_1427_112_2_18_3_11_32_36 "MMIWWW_SwapBookmark Title No mem "
#define MMIWWW_BOOKMARK_1440_112_2_18_3_11_32_37 "MMIWWW_SwapBookmark get %d's url failed "
#define MMIWWW_BOOKMARK_1447_112_2_18_3_11_32_38 "MMIWWW_SwapBookmark Url No mem "
#define MMIWWW_BOOKMARK_1460_112_2_18_3_11_32_39 "MMIWWW_SwapBookmark get %d's title failed "
#define MMIWWW_BOOKMARK_1465_112_2_18_3_11_32_40 "MMIWWW_SwapBookmark index_1 = %d, index = %d "
#define MMIWWW_BOOKMARK_1473_112_2_18_3_11_32_41 "MMIWWW_SwapBookmark get %d's url failed "
#define MMIWWW_BOOKMARK_1482_112_2_18_3_11_32_42 "MMIWWW_SwapBookmark index_2 = %d, index = %d "
#define MMIWWW_BOOKMARK_1708_112_2_18_3_11_32_43 "MMIWWW_CleanBookmarkData root_file_array == PNULL"
#define MMIWWW_BOOKMARK_1754_112_2_18_3_11_33_44 "MMIWWW_CleanBookmarkData result=%d"
#define MMIWWW_FUNC_738_112_2_18_3_11_34_45 "MMIWWW_GenerateSearchWeb search_engine = %d"
#define MMIWWW_FUNC_1383_112_2_18_3_11_36_46 "MMIWWW_SetCurrentPageZoomPercent act_index = %d zoom_percent = %d"
#define MMIWWW_FUNC_1397_112_2_18_3_11_36_47 "MMIWWW_GetCurrentPageZoomPercent act_index = %d zoom_percent = %d"
#define MMIWWW_FUNC_1402_112_2_18_3_11_36_48 "MMIWWW_GetCurrentPageZoomPercent act_index = %d zoom_percent = %d"
#define MMIWWW_FUNC_1412_112_2_18_3_11_36_49 "WwwPlayBgsoundCallBack, result=%d"
#define MMIWWW_FUNC_1431_112_2_18_3_11_36_50 "MMIWWW_Play_BG_sound ret=%d, fVolume=%d,fLength=%d,fLoop=%d,fMimeType=%d,fDataSize=%d"
#define MMIWWW_FUNC_1662_112_2_18_3_11_36_51 "MMIWWW_SetDefaultBookmartk ret = %d"
#define MMIWWW_FUNC_1668_112_2_18_3_11_36_52 "MMIWWW_SetDefaultBookmartk NO memory"
#define MMIWWW_FUNC_1695_112_2_18_3_11_37_53 "MMIWWW_SetDefaultBookmartk ret = %d"
#define MMIWWW_FUNC_1699_112_2_18_3_11_37_54 "MMIWWW_SetDefaultBookmartk NO memory"
#define MMIWWW_FUNC_1943_112_2_18_3_11_37_55 "MMIAPIWWW_GetNetSettingIndex dual_sys = %d"
#define MMIWWW_FUNC_1969_112_2_18_3_11_37_56 "MMIWWW_SetNetSettingIndex dual_sys = %d, index = %d"
#define MMIWWW_FUNC_2002_112_2_18_3_11_37_57 "MMIWWW_ConnectionCallback,index=%d, dual_sys=%d"
#define MMIWWW_FUNC_2149_112_2_18_3_11_37_58 "Upload_OpenFileCallBack is_success = %d"
#define MMIWWW_FUNC_2160_112_2_18_3_11_37_59 "Upload_OpenFileCallBack, file_num = %d"
#define MMIWWW_FUNC_2168_112_2_18_3_11_37_60 "Upload_OpenFileCallBack pathlen = %d"
#define MMIWWW_FUNC_2357_112_2_18_3_11_38_61 "MMIWWW_SetHTTPProxy gateway=%s, gateway_len=%d, port=%d, pdp_state=%d,bear_type=%d"
#define MMIWWW_FUNC_2396_112_2_18_3_11_38_62 "MMIPDP_INTERFACE_WIFI gateway=%s, is_use_proxy = %d, detail_struct.gateway, detail_struct.is_use_proxy"
#define MMIWWW_FUNC_2431_112_2_18_3_11_38_63 "MMIWWW_SetInitUA ua_len=%d"
#define MMIWWW_FUNC_2652_112_2_18_3_11_38_64 "MMIWWW_SetCustomizedConnection is_connection_customized=%d,connection_index=%d"
#define MMIWWW_FUNC_2692_112_2_18_3_11_39_65 "MMIWWW_GetCustomizedConnection is_connection_customized=%d,connection_index=%d"
#define MMIWWW_FUNC_2709_112_2_18_3_11_39_66 "MMIWWW_ResetNetConnect null"
#define MMIWWW_FUNC_2761_112_2_18_3_11_39_67 "MMIWWW_ResetConnectionSettings index=%d,is_connection_customized=%d,connection_index=%d,default=%d"
#define MMIWWW_FUNC_2778_112_2_18_3_11_39_68 "MMIWWW_SetBrowserReloadFlag act_index=%d,is_reload=%d"
#define MMIWWW_FUNC_2822_112_2_18_3_11_39_69 "MMIWWW_SetBrowserFocusFlag is_focus=%d"
#define MMIWWW_FUNC_2972_112_2_18_3_11_39_70 "MMIWWW_HandleExScheme: in_url empty"
#define MMIWWW_FUNC_2979_112_2_18_3_11_39_71 "MMIWWW_HandleExScheme: Err No memory "
#define MMIWWW_FUNC_2985_112_2_18_3_11_39_72 "MMIWWW_HandleExScheme: Err No memory "
#define MMIWWW_FUNC_3131_112_2_18_3_11_39_73 "mmiwww smp temp_str = %s"
#define MMIWWW_FUNC_3316_112_2_18_3_11_40_74 "MMIWWW_SetImageDisplayMode: in_mode=%d"
#define MMIWWW_FUNC_3381_112_2_18_3_11_40_75 "MMIWWW_SetCopyMode: in_mode=%d"
#define MMIWWW_FUNC_3405_112_2_18_3_11_40_76 "MMIWWW_GetCopyMode: copy_mode=%d"
#define MMIWWW_FUNC_3462_112_2_18_3_11_40_77 "MMIWWW_GetUserInputMSG"
#define MMIWWW_FUNC_3478_112_2_18_3_11_40_78 "MMIWWW_SetUserInputMSG"
#define MMIWWW_FUNC_3508_112_2_18_3_11_40_79 "MMIAPIWWW_CheckTimerDisable"
#define MMIWWW_FUNC_3512_112_2_18_3_11_40_80 "MMIAPIWWW_CheckTimerDisable No instance"
#define MMIWWW_FUNC_3520_112_2_18_3_11_40_81 "MMIAPIWWW_CheckTimerDisable tab error"
#define MMIWWW_FUNC_3579_112_2_18_3_11_40_82 "MMIWWW_SetHomepageURL No mem"
#define MMIWWW_NETFRONT_156_112_2_18_3_11_42_83 "Netfront_InitPeerData: width=%d, height=%d"
#define MMIWWW_NETFRONT_191_112_2_18_3_11_42_84 "RTOS:Netfront_InitPeerData GUILCD_GetInfo fail"
#define MMIWWW_NETFRONT_251_112_2_18_3_11_42_85 "Netfront_NFBrowserNew mem = %d"
#define MMIWWW_NETFRONT_252_112_2_18_3_11_42_86 "Netfront_NFBrowserNew NF Version: %s"
#define MMIWWW_NETFRONT_563_112_2_18_3_11_43_87 "MMIWWW_HandleMsgEvent www_instance Is NULL"
#define MMIWWW_NETFRONT_568_112_2_18_3_11_43_88 "MMIWWW_HandleMsgEvent msg_id=%d"
#define MMIWWW_NETFRONT_892_112_2_18_3_11_44_89 "SCI_Key_To_NF_Key keycode = 0x%04x, keychar = 0x%04x, keypress = %d"
#define MMIWWW_NETFRONT_924_112_2_18_3_11_44_90 "MMIWWW_Netfront_StartBrowser ret=%d"
#define MMIWWW_NETFRONT_953_112_2_18_3_11_44_91 "MMIWWW_Netfront_StopBrowser ret=%d"
#define MMIWWW_NETFRONT_967_112_2_18_3_11_44_92 "MMIWWW_Netfront_SuspendBrowser ret=%d"
#define MMIWWW_NETFRONT_1004_112_2_18_3_11_44_93 "MMIWWW_Netfront_ResumeBrowser ret=%d"
#define MMIWWW_NETFRONT_1041_112_2_18_3_11_44_94 "MMIWWW_Netfront_ReloadPage() state=%d"
#define MMIWWW_NETFRONT_1045_112_2_18_3_11_44_95 "MMIWWW_Netfront_ReloadPage abort ret =%d"
#define MMIWWW_NETFRONT_1049_112_2_18_3_11_44_96 "MMIWWW_Netfront_ReloadPage ret=%d"
#define MMIWWW_NETFRONT_1063_112_2_18_3_11_44_97 "MMIWWW_Netfront_Abort ret=%d"
#define MMIWWW_NETFRONT_1117_112_2_18_3_11_44_98 "MMIWW_Netfront_PageZoom mode=%d, fZoom_percent=%d, act_index=%d"
#define MMIWWW_NETFRONT_1121_112_2_18_3_11_44_99 "MMIWWW_Netfront_PageZoom tab error"
#define MMIWWW_NETFRONT_1187_112_2_18_3_11_44_100 "MMIWWW_Netfront_PageZoom percent=%d"
#define MMIWWW_NETFRONT_1207_112_2_18_3_11_44_101 "MMIWWW_Netfront_SearchText flag=0x%x, wstr_len=%d"
#define MMIWWW_NETFRONT_1222_112_2_18_3_11_44_102 "MMIWWW_Netfront_ViewMode mode=%d"
#define MMIWWW_NETFRONT_1995_112_2_18_3_11_46_103 "MMIWWW_Netfront_Resize is full=%d"
#define MMIWWW_NETFRONT_2110_112_2_18_3_11_46_104 "MMIWWW_Netfront_SetLocation ret=%d"
#define MMIWWW_NETFRONT_2467_112_2_18_3_11_47_105 "MMIWWW_Netfront_GetImageAddress Alloc failed!"
#define MMIWWW_NETFRONT_2473_112_2_18_3_11_47_106 "MMIWWW_Netfront_GetImageAddress Error!"
#define MMIWWW_NETFRONT_2497_112_2_18_3_11_47_107 "MMIWWW_Netfront_GetImageData SUCC size = %d"
#define MMIWWW_NETFRONT_2509_112_2_18_3_11_47_108 "MMIWWW_Netfront_GetImageData Alloc failed"
#define MMIWWW_NETFRONT_2515_112_2_18_3_11_47_109 "MMIWWW_Netfront_GetImageData failed"
#define MMIWWW_NETFRONT_2545_112_2_18_3_11_47_110 "MMIWWW_Netfront_GetImageContentType Alloc failed!"
#define MMIWWW_NETFRONT_2552_112_2_18_3_11_47_111 "MMIWWW_Netfront_GetImageContentType Error!"
#define MMIWWW_NETFRONT_2665_112_2_18_3_11_47_112 "MMIWWW_ReplyRequestAuth reply_ret=%d"
#define MMIWWW_NETFRONT_CMD_78_112_2_18_3_11_48_113 "MMIWWW_Netfront_NotifyErrorProc in_index=%d, in_code=%d, in_nf=%d, is_alert = %d"
#define MMIWWW_NETFRONT_CMD_147_112_2_18_3_11_48_114 "MMIWWW_Netfront_NotifyErrorProc in_index=%d, in_code=%d, in_nf=%d"
#define MMIWWW_NETFRONT_CMD_173_112_2_18_3_11_48_115 "MMIWWW_Netfront_NotifyContentErrorProc in_index=%d, in_code=%d, in_nf=%d,pdp_state=%d"
#define MMIWWW_NETFRONT_CMD_390_112_2_18_3_11_48_116 "RequestExSchemeRequestProc: in_url=%s"
#define MMIWWW_NETFRONT_CMD_457_112_2_18_3_11_49_117 "MMIWWW_Netfront_RequestJSAlertProc ret=%d"
#define MMIWWW_NETFRONT_CMD_654_112_2_18_3_11_49_118 "MMIWWW_Netfront_HelperProc Neednot Download!"
#define MMIWWW_SLIM_PEER_ALT_125_112_2_18_3_11_51_119 "MMIWWW_CreateMemory mem_size = %d"
#define MMIWWW_SLIM_PEER_ALT_130_112_2_18_3_11_51_120 "MMIWWW_CreateMemory mem_handle No mem"
#define MMIWWW_SLIM_PEER_ALT_141_112_2_18_3_11_51_121 "MMIWWW_CreateMemory buf_ptr No mem"
#define MMIWWW_SLIM_PEER_ALT_289_112_2_18_3_11_51_122 "slimMemoryAltMallocPeer size=%d"
#define MMIWWW_SLIM_PEER_ALT_318_112_2_18_3_11_51_123 "slimMemoryAltFreePeer"
#define MMIWWW_SLIM_PEER_DEBUG_60_112_2_18_3_11_51_124 "%s"
#define MMIWWW_SLIM_PEER_DEBUG_77_112_2_18_3_11_51_125 "%s"
#define MMIWWW_SLIM_PEER_DEBUG_109_112_2_18_3_11_51_126 "%x, %x, %x, %x"
#define MMIWWW_SLIM_PEER_DEBUG_113_112_2_18_3_11_51_127 "%s"
#define MMIWWW_SLIM_PEER_NET_245_112_2_18_3_11_53_128 "_net_get_unused_sockobj socket num exceed"
#define MMIWWW_SLIM_PEER_NET_274_112_2_18_3_11_53_129 "_net_release_sockobj in_desc=%d"
#define MMIWWW_SLIM_PEER_NET_293_112_2_18_3_11_53_130 "_net_dup_ip_addr in_desc=%d"
#define MMIWWW_SLIM_PEER_NET_316_112_2_18_3_11_53_131 "SocketOpenPeer: start in_domain =%d, type=%d"
#define MMIWWW_SLIM_PEER_NET_329_112_2_18_3_11_54_132 "SocketOpenPeer, only support SLIM_IP_ADDR_TYPE_V4"
#define MMIWWW_SLIM_PEER_NET_337_112_2_18_3_11_54_133 "SocketOpenPeer, e = %d"
#define MMIWWW_SLIM_PEER_NET_348_112_2_18_3_11_54_134 "sci_sock_setsockopt, result=%d"
#define MMIWWW_SLIM_PEER_NET_353_112_2_18_3_11_54_135 "SocketOpenPeer, return %d"
#define MMIWWW_SLIM_PEER_NET_364_112_2_18_3_11_54_136 "SocketClosePeer, close sokcet: %d"
#define MMIWWW_SLIM_PEER_NET_425_112_2_18_3_11_54_137 "SocketCanReadWritePeer, s=%d, return %d"
#define MMIWWW_SLIM_PEER_NET_454_112_2_18_3_11_54_138 "SocketConnectPeer():modify net_id=%d"
#define MMIWWW_SLIM_PEER_NET_463_112_2_18_3_11_54_139 "SocketConnectPeer(): connection is established, in_desc=%d"
#define MMIWWW_SLIM_PEER_NET_471_112_2_18_3_11_54_140 "SocketConnectPeer, s=%d, return %d"
#define MMIWWW_SLIM_PEER_NET_484_112_2_18_3_11_54_141 "sci_err_to_netfront_err=%d"
#define MMIWWW_SLIM_PEER_NET_519_112_2_18_3_11_54_142 "ipaddr_to_sockaddr,in_port=%d, fType=%d,fAddr=%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %dn"
#define MMIWWW_SLIM_PEER_NET_550_112_2_18_3_11_54_143 "SocketReceivePeer, res = %d"
#define MMIWWW_SLIM_PEER_NET_557_112_2_18_3_11_54_144 "SocketReceivePeer, error EWOULDBLOCK"
#define MMIWWW_SLIM_PEER_NET_563_112_2_18_3_11_54_145 "SocketReceivePeer, EPIPE"
#define MMIWWW_SLIM_PEER_NET_569_112_2_18_3_11_54_146 "SocketReceivePeer, error SLIM_E_GENERIC code = %d"
#define MMIWWW_SLIM_PEER_NET_575_112_2_18_3_11_54_147 "SocketReceivePeer, s=%d, return %d"
#define MMIWWW_SLIM_PEER_NET_596_112_2_18_3_11_54_148 "SocketSendPeer, error SLIM_E_WOULDBLOCK\n"
#define MMIWWW_SLIM_PEER_NET_601_112_2_18_3_11_54_149 "SocketSendPeer, error SLIM_E_STRPIPE\n"
#define MMIWWW_SLIM_PEER_NET_606_112_2_18_3_11_54_150 "SocketSendPeer, error SLIM_E_GENERIC code = %d\n"
#define MMIWWW_SLIM_PEER_NET_611_112_2_18_3_11_54_151 "SocketSendPeer, s=%d, return %d"
#define MMIWWW_SLIM_PEER_NET_730_112_2_18_3_11_55_152 "slimTCPOpenPeer() start"
#define MMIWWW_SLIM_PEER_NET_736_112_2_18_3_11_55_153 "slimTCPOpenPeer:desc=%d"
#define MMIWWW_SLIM_PEER_NET_781_112_2_18_3_11_55_154 "[wv_peer_net.c] slimTCPClosePeer() start"
#define MMIWWW_SLIM_PEER_NET_805_112_2_18_3_11_55_155 "slimTCPCanReadWritePeer, s=%d"
#define MMIWWW_SLIM_PEER_NET_829_112_2_18_3_11_55_156 "slimTCPIsConnectedPeer:desc=%d"
#define MMIWWW_SLIM_PEER_NET_849_112_2_18_3_11_55_157 "slimTCPIsConnectPeer:Open success"
#define MMIWWW_SLIM_PEER_NET_856_112_2_18_3_11_55_158 "slimTCPIsConnectPeer:Open err"
#define MMIWWW_SLIM_PEER_NET_863_112_2_18_3_11_55_159 "slimTCPIsConnectPeer:Open success already"
#define MMIWWW_SLIM_PEER_NET_919_112_2_18_3_11_55_160 "slimTCPConnectPeer:decs=%d,sock=%d fType=%d,in_port=%d"
#define MMIWWW_SLIM_PEER_NET_1004_112_2_18_3_11_55_161 "slimTCPBindPeer in_addr=%s,in_port=%d"
#define MMIWWW_SLIM_PEER_NET_1117_112_2_18_3_11_55_162 "slimDNSInitializePeer"
#define MMIWWW_SLIM_PEER_NET_1145_112_2_18_3_11_55_163 "slimDNSFinalizePeer"
#define MMIWWW_SLIM_PEER_NET_1158_112_2_18_3_11_55_164 "slimDNSSetServerPeer not supported"
#define MMIWWW_SLIM_PEER_NET_1200_112_2_18_3_11_56_165 "slimDNSRequestResolvePeer in_domain=%s, in_len=%d"
#define MMIWWW_SLIM_PEER_NET_1223_112_2_18_3_11_56_166 "slimDNSRequestResolvePeer gateway=%s, gateway_len=%d, bearer_type=%d"
#define MMIWWW_SLIM_PEER_NET_1255_112_2_18_3_11_56_167 "slimDNSRequestCancelPeer"
#define MMIWWW_SLIM_PEER_NET_1287_112_2_18_3_11_56_168 "slimDNSIsResolvedPeer in_ticket_id=%d"
#define MMIWWW_SLIM_PEER_NET_1330_112_2_18_3_11_56_169 "slimDNSIsResolvedPeer get_dns_result=%d"
#define MMIWWW_SLIM_PEER_NET_1375_112_2_18_3_11_56_170 "slimDNSGetAddressPeer"
#define MMIWWW_SLIM_PEER_NET_1406_112_2_18_3_11_56_171 "slimDNSSetModePeer"
#define MMIWWW_SLIM_PEER_NET_1417_112_2_18_3_11_56_172 "slimDNSSetTimeout"
#define MMIWWW_SLIM_PEER_NET_1426_112_2_18_3_11_56_173 "slimDNSSetDNSPortPeer"
#define MMIWWW_SLIM_PEER_NET_1435_112_2_18_3_11_56_174 "slimDNSSetMaxRetryPeer"
#define MMIWWW_SLIM_PEER_NET_1444_112_2_18_3_11_56_175 "slimDNSSetRetryIntervalPeer"
#define MMIWWW_SLIM_PEER_NET_1489_112_2_18_3_11_56_176 "MMIWWW_HandleDNSCnf request_id=%d, error_code=%d"
#define MMIWWW_WINTAB_2243_112_2_18_3_12_5_177 "mmiwww_wintab.c:MMIWWW_HandleCommonEditWinMsg() enter"
#define MMIWWW_WINTAB_3478_112_2_18_3_12_7_178 "mmiwww_wintab.c:HandleWWWMainMenuWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_3768_112_2_18_3_12_8_179 "MMIWWW_OpenWWWBrowserWin in_type=%d, in_url=%s"
#define MMIWWW_WINTAB_3956_112_2_18_3_12_8_180 "mmiwww_HandleNetAccess() msg_id=0x%04x"
#define MMIWWW_WINTAB_4019_112_2_18_3_12_8_181 "mmiwww_HandleNetAccessPagememo() msg_id=0x%04x"
#define MMIWWW_WINTAB_4074_112_2_18_3_12_8_182 "MMIWWW_HandleDelHistory() msg_id=0x%04x"
#define MMIWWW_WINTAB_4098_112_2_18_3_12_8_183 "MMIWWW_HandleDelHistory Error"
#define MMIWWW_WINTAB_4122_112_2_18_3_12_9_184 "HandleWWWBrowserWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_4127_112_2_18_3_12_9_185 "HandleWWWBrowserWinMsg() No action"
#define MMIWWW_WINTAB_4445_112_2_18_3_12_9_186 "HandleWWW'Msg wrong timer id"
#define MMIWWW_WINTAB_4570_112_2_18_3_12_10_187 "HandleWWWBrowserWinMsg() Before msg_type =0x%04x"
#define MMIWWW_WINTAB_4601_112_2_18_3_12_10_188 "HandleWWWBrowserWinMsg() After msg_type =0x%04x"
#define MMIWWW_WINTAB_4977_112_2_18_3_12_10_189 "mmiwww_HandleBrowserLoadmsg() param =%d, active = %d"
#define MMIWWW_WINTAB_5017_112_2_18_3_12_10_190 "mmiwww_HandleBrowserLoadmsg() param =%d, active = %d"
#define MMIWWW_WINTAB_5027_112_2_18_3_12_11_191 "MMIWWW_Netfront_StartBrowser out_content_header->fData=%s"
#define MMIWWW_WINTAB_5062_112_2_18_3_12_11_192 "mmiwww_HandleBrowserCancel() state=%d"
#define MMIWWW_WINTAB_5128_112_2_18_3_12_11_193 "mmiwww_HandleBrowserOK() state=%d"
#define MMIWWW_WINTAB_5171_112_2_18_3_12_11_194 "mmiwww_HandleBrowserTPDown()"
#define MMIWWW_WINTAB_5313_112_2_18_3_12_11_195 "mmiwww_SetLoadPercent() percent = %d"
#define MMIWWW_WINTAB_5386_112_2_18_3_12_11_196 "mmiwww_wintab.c:HandleWWWPageOptMenuWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_5686_112_2_18_3_12_12_197 "HandleWWWPageOptMenuWinMsg() not handled menu"
#define MMIWWW_WINTAB_5855_112_2_18_3_12_12_198 "mmiwww_CheckMenuGrayed shotcut_id = %d, is_need_gray = %d"
#define MMIWWW_WINTAB_5872_112_2_18_3_12_12_199 "mmiwww_CheckPageZoom mode=%d, percent=%d, act_index=%d"
#define MMIWWW_WINTAB_6558_112_2_18_3_12_14_200 "mmiwww_wintab.c:HandleWWWMainmenuOptWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_6738_112_2_18_3_12_14_201 "mmiwww_wintab.c:HandleWWWSetMenuWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_6823_112_2_18_3_12_14_202 "HandleWWWSetMenuWinMsg account err"
#define MMIWWW_WINTAB_6928_112_2_18_3_12_14_203 "mmiwww_wintab.c:HandleWWWHistoryWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_7066_112_2_18_3_12_15_204 "mmiwww_wintab.c:HandleWWWPageMemoWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_7110_112_2_18_3_12_15_205 "mmiwww_wintab.c:HandleWWWPageMemoWinMsg() strlen  =%d"
#define MMIWWW_WINTAB_7197_112_2_18_3_12_15_206 "mmiwww_wintab.c:HandleWWWCertificateWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_7293_112_2_18_3_12_15_207 "mmiwww_wintab.c:HandleWWWHistoryOptWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_7519_112_2_18_3_12_16_208 "mmiwww_wintab.c:HandleWWWPageMemoOptWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_7988_112_2_18_3_12_17_209 "mmiwww_wintab.c:HandleWWWMatchKeyWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_8064_112_2_18_3_12_17_210 "mmiwww_wintab.c:HandleWWWMatchURLWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_8137_112_2_18_3_12_17_211 "mmiwww_wintab.c:HandleWWWMatchURLWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_8227_112_2_18_3_12_17_212 "mmiwww_wintab.c:HandleCerAttributeDetailWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_8269_112_2_18_3_12_17_213 "mmiwww_wintab.c:HandleWWWSetWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_8630_112_2_18_3_12_18_214 "mmiwww_wintab.c:HandleWWWInlineIMEWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_9172_112_2_18_3_12_20_215 "mmiwww_wintab.c:MMIWWW_Handle_BG_Sound is_exist=%d, index=%d"
#define MMIWWW_WINTAB_9197_112_2_18_3_12_20_216 "MMIWWW_OpenJSAlertWin() enter"
#define MMIWWW_WINTAB_9223_112_2_18_3_12_20_217 "MMIWWW_ConvertToMMIString() enter"
#define MMIWWW_WINTAB_9233_112_2_18_3_12_20_218 "MMIWWW_ConvertToMMIString() len=%d wstr_ptr=0X%x"
#define MMIWWW_WINTAB_9255_112_2_18_3_12_20_219 "MMIWWW_OpenJSConfirmWin enter"
#define MMIWWW_WINTAB_9297_112_2_18_3_12_20_220 "HandleWWWJSConfirmWin enter OK ret=%d"
#define MMIWWW_WINTAB_9304_112_2_18_3_12_20_221 "HandleWWWJSConfirmWin enter CANCEL ret=%d"
#define MMIWWW_WINTAB_9327_112_2_18_3_12_20_222 "MMIWWW_OpenJSPromptWin enter"
#define MMIWWW_WINTAB_9441_112_2_18_3_12_20_223 "HandleWWWJSPromptWin enter OK"
#define MMIWWW_WINTAB_9452_112_2_18_3_12_20_224 "HandleWWWJSPromptWin enter OK ret=%d"
#define MMIWWW_WINTAB_9464_112_2_18_3_12_20_225 "HandleWWWJSPromptWin enter CANCEL"
#define MMIWWW_WINTAB_9466_112_2_18_3_12_20_226 "HandleWWWJSPromptWin enter CANCEL ret=%d"
#define MMIWWW_WINTAB_9522_112_2_18_3_12_20_227 "mmiwww_OpenJSPromptEditWin enter"
#define MMIWWW_WINTAB_9638_112_2_18_3_12_21_228 "mmiwww_wintab.c:HandleLinkListWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_9695_112_2_18_3_12_21_229 "mmiwww_wintab.c:HandleCharsetListWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_9782_112_2_18_3_12_21_230 "mmiwww_wintab.c:HandleCharsetListWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_9861_112_2_18_3_12_21_231 "mmiwww_wintab.c:HandleWWWShotcutKeyWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_10278_112_2_18_3_12_22_232 "MMIWWW_OpenWMLSAlertWin() enter"
#define MMIWWW_WINTAB_10300_112_2_18_3_12_22_233 "MMIWWW_OpenWMLSConfirmWin enter"
#define MMIWWW_WINTAB_10370_112_2_18_3_12_22_234 "MMIWWW_OpenWMLSPromptWin enter"
#define MMIWWW_WINTAB_10439_112_2_18_3_12_22_235 "HandleWWWWMLSPromptWin enter OK"
#define MMIWWW_WINTAB_10450_112_2_18_3_12_22_236 "HandleWWWWMLSPromptWin enter OK ret=%d"
#define MMIWWW_WINTAB_10460_112_2_18_3_12_23_237 "HandleWWWWMLSPromptWin enter CANCEL"
#define MMIWWW_WINTAB_10462_112_2_18_3_12_23_238 "HandleWWWWMLSPromptWin enter CANCEL ret=%d"
#define MMIWWW_WINTAB_10518_112_2_18_3_12_23_239 "mmiwww_OpenWMLSPromptEditWin enter"
#define MMIWWW_WINTAB_10649_112_2_18_3_12_23_240 "mmiwww_wintab.c:HandleWWWAuthWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_10791_112_2_18_3_12_23_241 "mmiwww_wintab.c:HandleWWWAuthWinMsg() msg_id=%d"
#define MMIWWW_WINTAB_10948_112_2_18_3_12_24_242 "HandleWWWHomepageSetWinMsg No mem"
#define MMIWWW_WINTAB_11259_112_2_18_3_12_24_243 "mmiwww_wintab.c:HandleWWWBookmarkWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_11388_112_2_18_3_12_24_244 "mmiwww_wintab.c:HandleWWWBookmarkWinMsg() String is NULL"
#define MMIWWW_WINTAB_11607_112_2_18_3_12_25_245 "mmiwww_wintab.c:HandleWWWBookmarkOptWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_11901_112_2_18_3_12_25_246 "mmiwww_wintab.c:HandleWWWBookmarkDetailSubMenuWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_12014_112_2_18_3_12_26_247 "HandleBookmarkMoveWaitWinMsg  copy_result=%d"
#define MMIWWW_WINTAB_12071_112_2_18_3_12_26_248 "MMIWWW_HandleMoveBookmark() msg_id=0x%04x"
#define MMIWWW_WINTAB_12465_112_2_18_3_12_27_249 "mmiwww_wintab.c:HandleInputBookmarkWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_12584_112_2_18_3_12_27_250 "HandleInputBookmarkWinMsg: ret = %d, high = %d, low = %d"
#define MMIWWW_WINTAB_12856_112_2_18_3_12_27_251 "mmiwww_wintab.c:HandleShowBookmarkWinMsg() msg_id=0x%04x"
#define MMIWWW_WINTAB_12910_112_2_18_3_12_27_252 "MMIWWW_HandleReplaceDuplicatedBookmark() msg_id=0x%04x"
#define MMIWWW_WINTAB_13094_112_2_18_3_12_28_253 "MMIWWW_HandleSelectWLAN() msg_id=0x%04x"
#define MMIWWW_WINTAB_13678_112_2_18_3_12_29_254 "mmiwww_DeleteSubWindow err index  = %d, win_num = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_WWW_TRC)
TRACE_MSG(MMIWWW_156_112_2_18_3_11_27_0,"MMIAPIWWW_StartWWW start type=%d,is_connection_customized=%d,connection_index=%d")
TRACE_MSG(MMIWWW_433_112_2_18_3_11_27_1,"\"\"  MMIWWW_SetFreq   \"#")
TRACE_MSG(MMIWWW_450_112_2_18_3_11_27_2,"\"\"  MMIWWW_SetFreq   \"#")
TRACE_MSG(MMIWWW_461_112_2_18_3_11_27_3,"MMIWWW_Initialize")
TRACE_MSG(MMIWWW_501_112_2_18_3_11_27_4,"MMIWWW_Finalize")
TRACE_MSG(MMIWWW_529_112_2_18_3_11_28_5,"WWWApplet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMIWWW_615_112_2_18_3_11_28_6,"MMIWWW_NetworkIsReady=%d")
TRACE_MSG(MMIWWW_696_112_2_18_3_11_28_7,"MMIWWW_SetPDPState:old %d-new %d")
TRACE_MSG(MMIWWW_724_112_2_18_3_11_28_8,"MMIWWW_NetConnectEx err dual_sys:%d")
TRACE_MSG(MMIWWW_730_112_2_18_3_11_28_9,"MMIWWW_NetConnectEx:MMIAPIPHONE_IsSimOk not OK")
TRACE_MSG(MMIWWW_757_112_2_18_3_11_28_10,"MMIWWW_NetConnectEx:MMIAPIPDP_Active fail")
TRACE_MSG(MMIWWW_775_112_2_18_3_11_28_11,"MMIWWW_NetConnect pdp_state=%d")
TRACE_MSG(MMIWWW_786_112_2_18_3_11_28_12,"MMIWWW_NetConnect PDP Connecting now...")
TRACE_MSG(MMIWWW_798_112_2_18_3_11_28_13,"MMIWWW_NetDisconnect directly")
TRACE_MSG(MMIWWW_799_112_2_18_3_11_28_14,"MMIWWW_NetDisconnect is_ready = %d")
TRACE_MSG(MMIWWW_807_112_2_18_3_11_28_15,"MMIWWW_NetDisconnect...")
TRACE_MSG(MMIWWW_831_112_2_18_3_11_28_16,"HandleWWWPDPActiveCnf() msg_ptr->result=%d")
TRACE_MSG(MMIWWW_860_112_2_18_3_11_28_17,"HandleWWWPDPDeactiveCnf() msg_ptr->result=%d")
TRACE_MSG(MMIWWW_885_112_2_18_3_11_28_18,"HandleWWWPDPDeactiveInd() msg_ptr->result=%d")
TRACE_MSG(MMIWWW_917_112_2_18_3_11_28_19,"HandleWWWPsMsg receive SOCKET_ASYNC_GETHOSTBYNAME_CNF")
TRACE_MSG(MMIWWW_946_112_2_18_3_11_28_20,"HandleWWWPDPMsg() enter msg_id= %d")
TRACE_MSG(MMIWWW_952_112_2_18_3_11_28_21,"HandleWWWPDPMsg: MMIPDP_ACTIVE_CNF")
TRACE_MSG(MMIWWW_960_112_2_18_3_11_28_22,"HandleWWWPDPMsg: MMIPDP_DEACTIVE_CNF")
TRACE_MSG(MMIWWW_968_112_2_18_3_11_28_23,"HandleWWWPDPMsg: MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIWWW_1007_112_2_18_3_11_28_24,"MMIAPIWWW_HandleWWWTimer:FALSE")
TRACE_MSG(MMIWWW_1030_112_2_18_3_11_29_25,"MMIAPIWWW_HandleWWWTimer:TRUE")
TRACE_MSG(MMIWWW_1034_112_2_18_3_11_29_26,"MMIAPIWWW_HandleWWWTimer:FALSE 22")
TRACE_MSG(MMIWWW_BOOKMARK_159_112_2_18_3_11_29_27,"RenameFile full_path_len %d new_file_name_len %d !!")
TRACE_MSG(MMIWWW_BOOKMARK_553_112_2_18_3_11_30_28,"MMIWWW_UpdateBookmarkTotalNum bookmark_total_num=%d")
TRACE_MSG(MMIWWW_BOOKMARK_577_112_2_18_3_11_30_29,"MMIWWW_Bookmark_Initialize bookmark_num=%d")
TRACE_MSG(MMIWWW_BOOKMARK_1157_112_2_18_3_11_31_30,"MMIWWW_ChangeBookmarkUrl in_index = %d")
TRACE_MSG(MMIWWW_BOOKMARK_1187_112_2_18_3_11_31_31,"MMIWWW_ChangeBookmarkUrl buf=%s")
TRACE_MSG(MMIWWW_BOOKMARK_1193_112_2_18_3_11_31_32,"MMIWWW_ChangeBookmarkUrl result = %d")
TRACE_MSG(MMIWWW_BOOKMARK_1370_112_2_18_3_11_32_33,"MMIWWW_Bookmark_GotoURL: url_len=%d, url_ptr= %s")
TRACE_MSG(MMIWWW_BOOKMARK_1411_112_2_18_3_11_32_34,"MMIWWW_SwapBookmark Bad Param")
TRACE_MSG(MMIWWW_BOOKMARK_1420_112_2_18_3_11_32_35,"MMIWWW_SwapBookmark get %d's title failed ")
TRACE_MSG(MMIWWW_BOOKMARK_1427_112_2_18_3_11_32_36,"MMIWWW_SwapBookmark Title No mem ")
TRACE_MSG(MMIWWW_BOOKMARK_1440_112_2_18_3_11_32_37,"MMIWWW_SwapBookmark get %d's url failed ")
TRACE_MSG(MMIWWW_BOOKMARK_1447_112_2_18_3_11_32_38,"MMIWWW_SwapBookmark Url No mem ")
TRACE_MSG(MMIWWW_BOOKMARK_1460_112_2_18_3_11_32_39,"MMIWWW_SwapBookmark get %d's title failed ")
TRACE_MSG(MMIWWW_BOOKMARK_1465_112_2_18_3_11_32_40,"MMIWWW_SwapBookmark index_1 = %d, index = %d ")
TRACE_MSG(MMIWWW_BOOKMARK_1473_112_2_18_3_11_32_41,"MMIWWW_SwapBookmark get %d's url failed ")
TRACE_MSG(MMIWWW_BOOKMARK_1482_112_2_18_3_11_32_42,"MMIWWW_SwapBookmark index_2 = %d, index = %d ")
TRACE_MSG(MMIWWW_BOOKMARK_1708_112_2_18_3_11_32_43,"MMIWWW_CleanBookmarkData root_file_array == PNULL")
TRACE_MSG(MMIWWW_BOOKMARK_1754_112_2_18_3_11_33_44,"MMIWWW_CleanBookmarkData result=%d")
TRACE_MSG(MMIWWW_FUNC_738_112_2_18_3_11_34_45,"MMIWWW_GenerateSearchWeb search_engine = %d")
TRACE_MSG(MMIWWW_FUNC_1383_112_2_18_3_11_36_46,"MMIWWW_SetCurrentPageZoomPercent act_index = %d zoom_percent = %d")
TRACE_MSG(MMIWWW_FUNC_1397_112_2_18_3_11_36_47,"MMIWWW_GetCurrentPageZoomPercent act_index = %d zoom_percent = %d")
TRACE_MSG(MMIWWW_FUNC_1402_112_2_18_3_11_36_48,"MMIWWW_GetCurrentPageZoomPercent act_index = %d zoom_percent = %d")
TRACE_MSG(MMIWWW_FUNC_1412_112_2_18_3_11_36_49,"WwwPlayBgsoundCallBack, result=%d")
TRACE_MSG(MMIWWW_FUNC_1431_112_2_18_3_11_36_50,"MMIWWW_Play_BG_sound ret=%d, fVolume=%d,fLength=%d,fLoop=%d,fMimeType=%d,fDataSize=%d")
TRACE_MSG(MMIWWW_FUNC_1662_112_2_18_3_11_36_51,"MMIWWW_SetDefaultBookmartk ret = %d")
TRACE_MSG(MMIWWW_FUNC_1668_112_2_18_3_11_36_52,"MMIWWW_SetDefaultBookmartk NO memory")
TRACE_MSG(MMIWWW_FUNC_1695_112_2_18_3_11_37_53,"MMIWWW_SetDefaultBookmartk ret = %d")
TRACE_MSG(MMIWWW_FUNC_1699_112_2_18_3_11_37_54,"MMIWWW_SetDefaultBookmartk NO memory")
TRACE_MSG(MMIWWW_FUNC_1943_112_2_18_3_11_37_55,"MMIAPIWWW_GetNetSettingIndex dual_sys = %d")
TRACE_MSG(MMIWWW_FUNC_1969_112_2_18_3_11_37_56,"MMIWWW_SetNetSettingIndex dual_sys = %d, index = %d")
TRACE_MSG(MMIWWW_FUNC_2002_112_2_18_3_11_37_57,"MMIWWW_ConnectionCallback,index=%d, dual_sys=%d")
TRACE_MSG(MMIWWW_FUNC_2149_112_2_18_3_11_37_58,"Upload_OpenFileCallBack is_success = %d")
TRACE_MSG(MMIWWW_FUNC_2160_112_2_18_3_11_37_59,"Upload_OpenFileCallBack, file_num = %d")
TRACE_MSG(MMIWWW_FUNC_2168_112_2_18_3_11_37_60,"Upload_OpenFileCallBack pathlen = %d")
TRACE_MSG(MMIWWW_FUNC_2357_112_2_18_3_11_38_61,"MMIWWW_SetHTTPProxy gateway=%s, gateway_len=%d, port=%d, pdp_state=%d,bear_type=%d")
TRACE_MSG(MMIWWW_FUNC_2396_112_2_18_3_11_38_62,"MMIPDP_INTERFACE_WIFI gateway=%s, is_use_proxy = %d, detail_struct.gateway, detail_struct.is_use_proxy")
TRACE_MSG(MMIWWW_FUNC_2431_112_2_18_3_11_38_63,"MMIWWW_SetInitUA ua_len=%d")
TRACE_MSG(MMIWWW_FUNC_2652_112_2_18_3_11_38_64,"MMIWWW_SetCustomizedConnection is_connection_customized=%d,connection_index=%d")
TRACE_MSG(MMIWWW_FUNC_2692_112_2_18_3_11_39_65,"MMIWWW_GetCustomizedConnection is_connection_customized=%d,connection_index=%d")
TRACE_MSG(MMIWWW_FUNC_2709_112_2_18_3_11_39_66,"MMIWWW_ResetNetConnect null")
TRACE_MSG(MMIWWW_FUNC_2761_112_2_18_3_11_39_67,"MMIWWW_ResetConnectionSettings index=%d,is_connection_customized=%d,connection_index=%d,default=%d")
TRACE_MSG(MMIWWW_FUNC_2778_112_2_18_3_11_39_68,"MMIWWW_SetBrowserReloadFlag act_index=%d,is_reload=%d")
TRACE_MSG(MMIWWW_FUNC_2822_112_2_18_3_11_39_69,"MMIWWW_SetBrowserFocusFlag is_focus=%d")
TRACE_MSG(MMIWWW_FUNC_2972_112_2_18_3_11_39_70,"MMIWWW_HandleExScheme: in_url empty")
TRACE_MSG(MMIWWW_FUNC_2979_112_2_18_3_11_39_71,"MMIWWW_HandleExScheme: Err No memory ")
TRACE_MSG(MMIWWW_FUNC_2985_112_2_18_3_11_39_72,"MMIWWW_HandleExScheme: Err No memory ")
TRACE_MSG(MMIWWW_FUNC_3131_112_2_18_3_11_39_73,"mmiwww smp temp_str = %s")
TRACE_MSG(MMIWWW_FUNC_3316_112_2_18_3_11_40_74,"MMIWWW_SetImageDisplayMode: in_mode=%d")
TRACE_MSG(MMIWWW_FUNC_3381_112_2_18_3_11_40_75,"MMIWWW_SetCopyMode: in_mode=%d")
TRACE_MSG(MMIWWW_FUNC_3405_112_2_18_3_11_40_76,"MMIWWW_GetCopyMode: copy_mode=%d")
TRACE_MSG(MMIWWW_FUNC_3462_112_2_18_3_11_40_77,"MMIWWW_GetUserInputMSG")
TRACE_MSG(MMIWWW_FUNC_3478_112_2_18_3_11_40_78,"MMIWWW_SetUserInputMSG")
TRACE_MSG(MMIWWW_FUNC_3508_112_2_18_3_11_40_79,"MMIAPIWWW_CheckTimerDisable")
TRACE_MSG(MMIWWW_FUNC_3512_112_2_18_3_11_40_80,"MMIAPIWWW_CheckTimerDisable No instance")
TRACE_MSG(MMIWWW_FUNC_3520_112_2_18_3_11_40_81,"MMIAPIWWW_CheckTimerDisable tab error")
TRACE_MSG(MMIWWW_FUNC_3579_112_2_18_3_11_40_82,"MMIWWW_SetHomepageURL No mem")
TRACE_MSG(MMIWWW_NETFRONT_156_112_2_18_3_11_42_83,"Netfront_InitPeerData: width=%d, height=%d")
TRACE_MSG(MMIWWW_NETFRONT_191_112_2_18_3_11_42_84,"RTOS:Netfront_InitPeerData GUILCD_GetInfo fail")
TRACE_MSG(MMIWWW_NETFRONT_251_112_2_18_3_11_42_85,"Netfront_NFBrowserNew mem = %d")
TRACE_MSG(MMIWWW_NETFRONT_252_112_2_18_3_11_42_86,"Netfront_NFBrowserNew NF Version: %s")
TRACE_MSG(MMIWWW_NETFRONT_563_112_2_18_3_11_43_87,"MMIWWW_HandleMsgEvent www_instance Is NULL")
TRACE_MSG(MMIWWW_NETFRONT_568_112_2_18_3_11_43_88,"MMIWWW_HandleMsgEvent msg_id=%d")
TRACE_MSG(MMIWWW_NETFRONT_892_112_2_18_3_11_44_89,"SCI_Key_To_NF_Key keycode = 0x%04x, keychar = 0x%04x, keypress = %d")
TRACE_MSG(MMIWWW_NETFRONT_924_112_2_18_3_11_44_90,"MMIWWW_Netfront_StartBrowser ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_953_112_2_18_3_11_44_91,"MMIWWW_Netfront_StopBrowser ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_967_112_2_18_3_11_44_92,"MMIWWW_Netfront_SuspendBrowser ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_1004_112_2_18_3_11_44_93,"MMIWWW_Netfront_ResumeBrowser ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_1041_112_2_18_3_11_44_94,"MMIWWW_Netfront_ReloadPage() state=%d")
TRACE_MSG(MMIWWW_NETFRONT_1045_112_2_18_3_11_44_95,"MMIWWW_Netfront_ReloadPage abort ret =%d")
TRACE_MSG(MMIWWW_NETFRONT_1049_112_2_18_3_11_44_96,"MMIWWW_Netfront_ReloadPage ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_1063_112_2_18_3_11_44_97,"MMIWWW_Netfront_Abort ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_1117_112_2_18_3_11_44_98,"MMIWW_Netfront_PageZoom mode=%d, fZoom_percent=%d, act_index=%d")
TRACE_MSG(MMIWWW_NETFRONT_1121_112_2_18_3_11_44_99,"MMIWWW_Netfront_PageZoom tab error")
TRACE_MSG(MMIWWW_NETFRONT_1187_112_2_18_3_11_44_100,"MMIWWW_Netfront_PageZoom percent=%d")
TRACE_MSG(MMIWWW_NETFRONT_1207_112_2_18_3_11_44_101,"MMIWWW_Netfront_SearchText flag=0x%x, wstr_len=%d")
TRACE_MSG(MMIWWW_NETFRONT_1222_112_2_18_3_11_44_102,"MMIWWW_Netfront_ViewMode mode=%d")
TRACE_MSG(MMIWWW_NETFRONT_1995_112_2_18_3_11_46_103,"MMIWWW_Netfront_Resize is full=%d")
TRACE_MSG(MMIWWW_NETFRONT_2110_112_2_18_3_11_46_104,"MMIWWW_Netfront_SetLocation ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_2467_112_2_18_3_11_47_105,"MMIWWW_Netfront_GetImageAddress Alloc failed!")
TRACE_MSG(MMIWWW_NETFRONT_2473_112_2_18_3_11_47_106,"MMIWWW_Netfront_GetImageAddress Error!")
TRACE_MSG(MMIWWW_NETFRONT_2497_112_2_18_3_11_47_107,"MMIWWW_Netfront_GetImageData SUCC size = %d")
TRACE_MSG(MMIWWW_NETFRONT_2509_112_2_18_3_11_47_108,"MMIWWW_Netfront_GetImageData Alloc failed")
TRACE_MSG(MMIWWW_NETFRONT_2515_112_2_18_3_11_47_109,"MMIWWW_Netfront_GetImageData failed")
TRACE_MSG(MMIWWW_NETFRONT_2545_112_2_18_3_11_47_110,"MMIWWW_Netfront_GetImageContentType Alloc failed!")
TRACE_MSG(MMIWWW_NETFRONT_2552_112_2_18_3_11_47_111,"MMIWWW_Netfront_GetImageContentType Error!")
TRACE_MSG(MMIWWW_NETFRONT_2665_112_2_18_3_11_47_112,"MMIWWW_ReplyRequestAuth reply_ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_CMD_78_112_2_18_3_11_48_113,"MMIWWW_Netfront_NotifyErrorProc in_index=%d, in_code=%d, in_nf=%d, is_alert = %d")
TRACE_MSG(MMIWWW_NETFRONT_CMD_147_112_2_18_3_11_48_114,"MMIWWW_Netfront_NotifyErrorProc in_index=%d, in_code=%d, in_nf=%d")
TRACE_MSG(MMIWWW_NETFRONT_CMD_173_112_2_18_3_11_48_115,"MMIWWW_Netfront_NotifyContentErrorProc in_index=%d, in_code=%d, in_nf=%d,pdp_state=%d")
TRACE_MSG(MMIWWW_NETFRONT_CMD_390_112_2_18_3_11_48_116,"RequestExSchemeRequestProc: in_url=%s")
TRACE_MSG(MMIWWW_NETFRONT_CMD_457_112_2_18_3_11_49_117,"MMIWWW_Netfront_RequestJSAlertProc ret=%d")
TRACE_MSG(MMIWWW_NETFRONT_CMD_654_112_2_18_3_11_49_118,"MMIWWW_Netfront_HelperProc Neednot Download!")
TRACE_MSG(MMIWWW_SLIM_PEER_ALT_125_112_2_18_3_11_51_119,"MMIWWW_CreateMemory mem_size = %d")
TRACE_MSG(MMIWWW_SLIM_PEER_ALT_130_112_2_18_3_11_51_120,"MMIWWW_CreateMemory mem_handle No mem")
TRACE_MSG(MMIWWW_SLIM_PEER_ALT_141_112_2_18_3_11_51_121,"MMIWWW_CreateMemory buf_ptr No mem")
TRACE_MSG(MMIWWW_SLIM_PEER_ALT_289_112_2_18_3_11_51_122,"slimMemoryAltMallocPeer size=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_ALT_318_112_2_18_3_11_51_123,"slimMemoryAltFreePeer")
TRACE_MSG(MMIWWW_SLIM_PEER_DEBUG_60_112_2_18_3_11_51_124,"%s")
TRACE_MSG(MMIWWW_SLIM_PEER_DEBUG_77_112_2_18_3_11_51_125,"%s")
TRACE_MSG(MMIWWW_SLIM_PEER_DEBUG_109_112_2_18_3_11_51_126,"%x, %x, %x, %x")
TRACE_MSG(MMIWWW_SLIM_PEER_DEBUG_113_112_2_18_3_11_51_127,"%s")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_245_112_2_18_3_11_53_128,"_net_get_unused_sockobj socket num exceed")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_274_112_2_18_3_11_53_129,"_net_release_sockobj in_desc=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_293_112_2_18_3_11_53_130,"_net_dup_ip_addr in_desc=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_316_112_2_18_3_11_53_131,"SocketOpenPeer: start in_domain =%d, type=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_329_112_2_18_3_11_54_132,"SocketOpenPeer, only support SLIM_IP_ADDR_TYPE_V4")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_337_112_2_18_3_11_54_133,"SocketOpenPeer, e = %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_348_112_2_18_3_11_54_134,"sci_sock_setsockopt, result=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_353_112_2_18_3_11_54_135,"SocketOpenPeer, return %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_364_112_2_18_3_11_54_136,"SocketClosePeer, close sokcet: %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_425_112_2_18_3_11_54_137,"SocketCanReadWritePeer, s=%d, return %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_454_112_2_18_3_11_54_138,"SocketConnectPeer():modify net_id=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_463_112_2_18_3_11_54_139,"SocketConnectPeer(): connection is established, in_desc=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_471_112_2_18_3_11_54_140,"SocketConnectPeer, s=%d, return %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_484_112_2_18_3_11_54_141,"sci_err_to_netfront_err=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_519_112_2_18_3_11_54_142,"ipaddr_to_sockaddr,in_port=%d, fType=%d,fAddr=%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %dn")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_550_112_2_18_3_11_54_143,"SocketReceivePeer, res = %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_557_112_2_18_3_11_54_144,"SocketReceivePeer, error EWOULDBLOCK")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_563_112_2_18_3_11_54_145,"SocketReceivePeer, EPIPE")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_569_112_2_18_3_11_54_146,"SocketReceivePeer, error SLIM_E_GENERIC code = %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_575_112_2_18_3_11_54_147,"SocketReceivePeer, s=%d, return %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_596_112_2_18_3_11_54_148,"SocketSendPeer, error SLIM_E_WOULDBLOCK\n")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_601_112_2_18_3_11_54_149,"SocketSendPeer, error SLIM_E_STRPIPE\n")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_606_112_2_18_3_11_54_150,"SocketSendPeer, error SLIM_E_GENERIC code = %d\n")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_611_112_2_18_3_11_54_151,"SocketSendPeer, s=%d, return %d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_730_112_2_18_3_11_55_152,"slimTCPOpenPeer() start")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_736_112_2_18_3_11_55_153,"slimTCPOpenPeer:desc=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_781_112_2_18_3_11_55_154,"[wv_peer_net.c] slimTCPClosePeer() start")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_805_112_2_18_3_11_55_155,"slimTCPCanReadWritePeer, s=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_829_112_2_18_3_11_55_156,"slimTCPIsConnectedPeer:desc=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_849_112_2_18_3_11_55_157,"slimTCPIsConnectPeer:Open success")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_856_112_2_18_3_11_55_158,"slimTCPIsConnectPeer:Open err")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_863_112_2_18_3_11_55_159,"slimTCPIsConnectPeer:Open success already")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_919_112_2_18_3_11_55_160,"slimTCPConnectPeer:decs=%d,sock=%d fType=%d,in_port=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1004_112_2_18_3_11_55_161,"slimTCPBindPeer in_addr=%s,in_port=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1117_112_2_18_3_11_55_162,"slimDNSInitializePeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1145_112_2_18_3_11_55_163,"slimDNSFinalizePeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1158_112_2_18_3_11_55_164,"slimDNSSetServerPeer not supported")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1200_112_2_18_3_11_56_165,"slimDNSRequestResolvePeer in_domain=%s, in_len=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1223_112_2_18_3_11_56_166,"slimDNSRequestResolvePeer gateway=%s, gateway_len=%d, bearer_type=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1255_112_2_18_3_11_56_167,"slimDNSRequestCancelPeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1287_112_2_18_3_11_56_168,"slimDNSIsResolvedPeer in_ticket_id=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1330_112_2_18_3_11_56_169,"slimDNSIsResolvedPeer get_dns_result=%d")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1375_112_2_18_3_11_56_170,"slimDNSGetAddressPeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1406_112_2_18_3_11_56_171,"slimDNSSetModePeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1417_112_2_18_3_11_56_172,"slimDNSSetTimeout")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1426_112_2_18_3_11_56_173,"slimDNSSetDNSPortPeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1435_112_2_18_3_11_56_174,"slimDNSSetMaxRetryPeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1444_112_2_18_3_11_56_175,"slimDNSSetRetryIntervalPeer")
TRACE_MSG(MMIWWW_SLIM_PEER_NET_1489_112_2_18_3_11_56_176,"MMIWWW_HandleDNSCnf request_id=%d, error_code=%d")
TRACE_MSG(MMIWWW_WINTAB_2243_112_2_18_3_12_5_177,"mmiwww_wintab.c:MMIWWW_HandleCommonEditWinMsg() enter")
TRACE_MSG(MMIWWW_WINTAB_3478_112_2_18_3_12_7_178,"mmiwww_wintab.c:HandleWWWMainMenuWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_3768_112_2_18_3_12_8_179,"MMIWWW_OpenWWWBrowserWin in_type=%d, in_url=%s")
TRACE_MSG(MMIWWW_WINTAB_3956_112_2_18_3_12_8_180,"mmiwww_HandleNetAccess() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_4019_112_2_18_3_12_8_181,"mmiwww_HandleNetAccessPagememo() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_4074_112_2_18_3_12_8_182,"MMIWWW_HandleDelHistory() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_4098_112_2_18_3_12_8_183,"MMIWWW_HandleDelHistory Error")
TRACE_MSG(MMIWWW_WINTAB_4122_112_2_18_3_12_9_184,"HandleWWWBrowserWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_4127_112_2_18_3_12_9_185,"HandleWWWBrowserWinMsg() No action")
TRACE_MSG(MMIWWW_WINTAB_4445_112_2_18_3_12_9_186,"HandleWWW'Msg wrong timer id")
TRACE_MSG(MMIWWW_WINTAB_4570_112_2_18_3_12_10_187,"HandleWWWBrowserWinMsg() Before msg_type =0x%04x")
TRACE_MSG(MMIWWW_WINTAB_4601_112_2_18_3_12_10_188,"HandleWWWBrowserWinMsg() After msg_type =0x%04x")
TRACE_MSG(MMIWWW_WINTAB_4977_112_2_18_3_12_10_189,"mmiwww_HandleBrowserLoadmsg() param =%d, active = %d")
TRACE_MSG(MMIWWW_WINTAB_5017_112_2_18_3_12_10_190,"mmiwww_HandleBrowserLoadmsg() param =%d, active = %d")
TRACE_MSG(MMIWWW_WINTAB_5027_112_2_18_3_12_11_191,"MMIWWW_Netfront_StartBrowser out_content_header->fData=%s")
TRACE_MSG(MMIWWW_WINTAB_5062_112_2_18_3_12_11_192,"mmiwww_HandleBrowserCancel() state=%d")
TRACE_MSG(MMIWWW_WINTAB_5128_112_2_18_3_12_11_193,"mmiwww_HandleBrowserOK() state=%d")
TRACE_MSG(MMIWWW_WINTAB_5171_112_2_18_3_12_11_194,"mmiwww_HandleBrowserTPDown()")
TRACE_MSG(MMIWWW_WINTAB_5313_112_2_18_3_12_11_195,"mmiwww_SetLoadPercent() percent = %d")
TRACE_MSG(MMIWWW_WINTAB_5386_112_2_18_3_12_11_196,"mmiwww_wintab.c:HandleWWWPageOptMenuWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_5686_112_2_18_3_12_12_197,"HandleWWWPageOptMenuWinMsg() not handled menu")
TRACE_MSG(MMIWWW_WINTAB_5855_112_2_18_3_12_12_198,"mmiwww_CheckMenuGrayed shotcut_id = %d, is_need_gray = %d")
TRACE_MSG(MMIWWW_WINTAB_5872_112_2_18_3_12_12_199,"mmiwww_CheckPageZoom mode=%d, percent=%d, act_index=%d")
TRACE_MSG(MMIWWW_WINTAB_6558_112_2_18_3_12_14_200,"mmiwww_wintab.c:HandleWWWMainmenuOptWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_6738_112_2_18_3_12_14_201,"mmiwww_wintab.c:HandleWWWSetMenuWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_6823_112_2_18_3_12_14_202,"HandleWWWSetMenuWinMsg account err")
TRACE_MSG(MMIWWW_WINTAB_6928_112_2_18_3_12_14_203,"mmiwww_wintab.c:HandleWWWHistoryWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_7066_112_2_18_3_12_15_204,"mmiwww_wintab.c:HandleWWWPageMemoWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_7110_112_2_18_3_12_15_205,"mmiwww_wintab.c:HandleWWWPageMemoWinMsg() strlen  =%d")
TRACE_MSG(MMIWWW_WINTAB_7197_112_2_18_3_12_15_206,"mmiwww_wintab.c:HandleWWWCertificateWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_7293_112_2_18_3_12_15_207,"mmiwww_wintab.c:HandleWWWHistoryOptWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_7519_112_2_18_3_12_16_208,"mmiwww_wintab.c:HandleWWWPageMemoOptWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_7988_112_2_18_3_12_17_209,"mmiwww_wintab.c:HandleWWWMatchKeyWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_8064_112_2_18_3_12_17_210,"mmiwww_wintab.c:HandleWWWMatchURLWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_8137_112_2_18_3_12_17_211,"mmiwww_wintab.c:HandleWWWMatchURLWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_8227_112_2_18_3_12_17_212,"mmiwww_wintab.c:HandleCerAttributeDetailWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_8269_112_2_18_3_12_17_213,"mmiwww_wintab.c:HandleWWWSetWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_8630_112_2_18_3_12_18_214,"mmiwww_wintab.c:HandleWWWInlineIMEWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_9172_112_2_18_3_12_20_215,"mmiwww_wintab.c:MMIWWW_Handle_BG_Sound is_exist=%d, index=%d")
TRACE_MSG(MMIWWW_WINTAB_9197_112_2_18_3_12_20_216,"MMIWWW_OpenJSAlertWin() enter")
TRACE_MSG(MMIWWW_WINTAB_9223_112_2_18_3_12_20_217,"MMIWWW_ConvertToMMIString() enter")
TRACE_MSG(MMIWWW_WINTAB_9233_112_2_18_3_12_20_218,"MMIWWW_ConvertToMMIString() len=%d wstr_ptr=0X%x")
TRACE_MSG(MMIWWW_WINTAB_9255_112_2_18_3_12_20_219,"MMIWWW_OpenJSConfirmWin enter")
TRACE_MSG(MMIWWW_WINTAB_9297_112_2_18_3_12_20_220,"HandleWWWJSConfirmWin enter OK ret=%d")
TRACE_MSG(MMIWWW_WINTAB_9304_112_2_18_3_12_20_221,"HandleWWWJSConfirmWin enter CANCEL ret=%d")
TRACE_MSG(MMIWWW_WINTAB_9327_112_2_18_3_12_20_222,"MMIWWW_OpenJSPromptWin enter")
TRACE_MSG(MMIWWW_WINTAB_9441_112_2_18_3_12_20_223,"HandleWWWJSPromptWin enter OK")
TRACE_MSG(MMIWWW_WINTAB_9452_112_2_18_3_12_20_224,"HandleWWWJSPromptWin enter OK ret=%d")
TRACE_MSG(MMIWWW_WINTAB_9464_112_2_18_3_12_20_225,"HandleWWWJSPromptWin enter CANCEL")
TRACE_MSG(MMIWWW_WINTAB_9466_112_2_18_3_12_20_226,"HandleWWWJSPromptWin enter CANCEL ret=%d")
TRACE_MSG(MMIWWW_WINTAB_9522_112_2_18_3_12_20_227,"mmiwww_OpenJSPromptEditWin enter")
TRACE_MSG(MMIWWW_WINTAB_9638_112_2_18_3_12_21_228,"mmiwww_wintab.c:HandleLinkListWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_9695_112_2_18_3_12_21_229,"mmiwww_wintab.c:HandleCharsetListWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_9782_112_2_18_3_12_21_230,"mmiwww_wintab.c:HandleCharsetListWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_9861_112_2_18_3_12_21_231,"mmiwww_wintab.c:HandleWWWShotcutKeyWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_10278_112_2_18_3_12_22_232,"MMIWWW_OpenWMLSAlertWin() enter")
TRACE_MSG(MMIWWW_WINTAB_10300_112_2_18_3_12_22_233,"MMIWWW_OpenWMLSConfirmWin enter")
TRACE_MSG(MMIWWW_WINTAB_10370_112_2_18_3_12_22_234,"MMIWWW_OpenWMLSPromptWin enter")
TRACE_MSG(MMIWWW_WINTAB_10439_112_2_18_3_12_22_235,"HandleWWWWMLSPromptWin enter OK")
TRACE_MSG(MMIWWW_WINTAB_10450_112_2_18_3_12_22_236,"HandleWWWWMLSPromptWin enter OK ret=%d")
TRACE_MSG(MMIWWW_WINTAB_10460_112_2_18_3_12_23_237,"HandleWWWWMLSPromptWin enter CANCEL")
TRACE_MSG(MMIWWW_WINTAB_10462_112_2_18_3_12_23_238,"HandleWWWWMLSPromptWin enter CANCEL ret=%d")
TRACE_MSG(MMIWWW_WINTAB_10518_112_2_18_3_12_23_239,"mmiwww_OpenWMLSPromptEditWin enter")
TRACE_MSG(MMIWWW_WINTAB_10649_112_2_18_3_12_23_240,"mmiwww_wintab.c:HandleWWWAuthWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_10791_112_2_18_3_12_23_241,"mmiwww_wintab.c:HandleWWWAuthWinMsg() msg_id=%d")
TRACE_MSG(MMIWWW_WINTAB_10948_112_2_18_3_12_24_242,"HandleWWWHomepageSetWinMsg No mem")
TRACE_MSG(MMIWWW_WINTAB_11259_112_2_18_3_12_24_243,"mmiwww_wintab.c:HandleWWWBookmarkWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_11388_112_2_18_3_12_24_244,"mmiwww_wintab.c:HandleWWWBookmarkWinMsg() String is NULL")
TRACE_MSG(MMIWWW_WINTAB_11607_112_2_18_3_12_25_245,"mmiwww_wintab.c:HandleWWWBookmarkOptWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_11901_112_2_18_3_12_25_246,"mmiwww_wintab.c:HandleWWWBookmarkDetailSubMenuWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_12014_112_2_18_3_12_26_247,"HandleBookmarkMoveWaitWinMsg  copy_result=%d")
TRACE_MSG(MMIWWW_WINTAB_12071_112_2_18_3_12_26_248,"MMIWWW_HandleMoveBookmark() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_12465_112_2_18_3_12_27_249,"mmiwww_wintab.c:HandleInputBookmarkWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_12584_112_2_18_3_12_27_250,"HandleInputBookmarkWinMsg: ret = %d, high = %d, low = %d")
TRACE_MSG(MMIWWW_WINTAB_12856_112_2_18_3_12_27_251,"mmiwww_wintab.c:HandleShowBookmarkWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_12910_112_2_18_3_12_27_252,"MMIWWW_HandleReplaceDuplicatedBookmark() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_13094_112_2_18_3_12_28_253,"MMIWWW_HandleSelectWLAN() msg_id=0x%04x")
TRACE_MSG(MMIWWW_WINTAB_13678_112_2_18_3_12_29_254,"mmiwww_DeleteSubWindow err index  = %d, win_num = %d")
END_TRACE_MAP(MMI_APP_WWW_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_WWW_TRC_H_

