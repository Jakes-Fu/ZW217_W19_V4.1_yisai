/******************************************************************************
 ** File Name:      mmi_app_mobile_video_trc.h                                         *
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
//trace_id:197
#ifndef _MMI_APP_MOBILE_VIDEO_TRC_H_
#define _MMI_APP_MOBILE_VIDEO_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIMV_75_112_2_18_2_39_6_0 "[MV]mmimv.c:enter function HandlePsAndRefMsg(), MVIDEO_TIMER = 0x%x msg_id = 0x%x"
#define MMIMV_EXPORT_196_112_2_18_2_39_7_1 "[MV]--WD[MMIAPIMV_Entry]:enter mv"
#define MMIMV_WINTAB_159_112_2_18_2_39_8_2 "[MV] HandleMainWinMsg msg_id = 0x%x"
#define MMIMV_WINTAB_319_112_2_18_2_39_8_3 "[MV] run in handleeditcommonwinmsg msg_id = %x"
#define MMIMV_WINTAB_328_112_2_18_2_39_8_4 "[MV] run in allow_inputmethod = %d, max_len = %d, callback = %x, defaul_len = %d, title_len = %d"
#define MMIMV_WINTAB_378_112_2_18_2_39_8_5 "[MV] run in createeditbox allow_inputmethod = %d"
#define MMIMV_WINTAB_448_112_2_18_2_39_9_6 "[MV] run in handlecontactwinmsg msg_id = %x"
#define MV_BASE_API_63_112_2_18_2_39_9_7 "[MV] MV_GetDiskID %x, %x"
#define MV_BASE_API_123_112_2_18_2_39_9_8 "[MV] CM_GetMachineInfo pBuffer = %x, nSize = %d, nIndex = %d"
#define MV_BASE_API_174_112_2_18_2_39_9_9 "[MV] [CM_Exit] mobile video exit "
#define MV_BASE_API_185_112_2_18_2_39_9_10 "[MV][MV_SetFreqHandler] "
#define MV_BASE_API_301_112_2_18_2_39_9_11 "[CM_HighLight_Always]:this is CM_HighLight_Always \r\n"
#define MV_BASE_API_324_112_2_18_2_39_9_12 "[MV] CM_GetDiskSpace get error "
#define MV_BASE_API_338_112_2_18_2_39_9_13 "[MV] CM_GetDiskSpace result = %d KB"
#define MV_CONTACT_API_79_112_2_18_2_39_10_14 "[MV] GetContactInfo pb_select_num = %d"
#define MV_CONTACT_API_155_112_2_18_2_39_10_15 "[MV] SetContactItem id =%d"
#define MV_CONTACT_API_183_112_2_18_2_39_10_16 "[MV] [InitContactItem]run in InitContactItem"
#define MV_CONTACT_API_193_112_2_18_2_39_10_17 "[MV] [InitContactItem] exit InitContactItem"
#define MV_CONTACT_API_203_112_2_18_2_39_10_18 "[MV] MV_ContactDialogCallbackProc nResult = %d"
#define MV_CONTACT_API_229_112_2_18_2_39_10_19 "[MV] CM_ContactDialog_Create input param is null"
#define MV_CONTACT_API_235_112_2_18_2_39_10_20 "--WD[CM_ContactDialog_Create]contact window d'not open"
#define MV_CONTACT_API_257_112_2_18_2_39_10_21 "run in CM_ContactDialog_Release"
#define MV_DATE_API_50_112_2_18_2_39_11_22 "[MV]--WD[CM_GetCurrentTime]mday=%d,mon=%d,year=%d,wday=%d"
#define MV_DATE_API_56_112_2_18_2_39_11_23 "[MV]--WD[CM_GetCurrentTime]sec=%d,min=%d,hour=%d"
#define MV_DATE_API_67_112_2_18_2_39_11_24 "[MV]--WD[CM_GetCurrentTime] TM_GetSysTime time_sec = %d"
#define MV_DATE_API_93_112_2_18_2_39_11_25 "[MV]--WD[CM_GetSystemTime]wYear=%d,wMonth=%d,wDay=%d"
#define MV_DATE_API_94_112_2_18_2_39_11_26 "[MV]--WD[CM_GetSystemTime]wHour=%d,wMinute=%d,wDay=%d"
#define MV_DATE_API_95_112_2_18_2_39_11_27 "[MV]--WD[CM_GetSystemTime]wDayOfWeek=%d,wDayOfYear=%d"
#define MV_FS_API_125_112_2_18_2_39_11_28 "[MV] SFS_GetAttr_MV *second = %d"
#define MV_FS_API_143_112_2_18_2_39_11_29 "[MV] [CM_File_Open] dwMode[%d]"
#define MV_FS_API_183_112_2_18_2_39_11_30 "[MV][CM_File_Open] open file fail"
#define MV_FS_API_189_112_2_18_2_39_11_31 "[MV][CM_File_Open] open file ok handle=%x"
#define MV_FS_API_211_112_2_18_2_39_11_32 "[MV][CM_File_Close] close file error code = %x "
#define MV_FS_API_244_112_2_18_2_39_11_33 "[MV] [CM_File_Read] read file error = %x"
#define MV_FS_API_270_112_2_18_2_39_11_34 "[MV] [CM_File_Write] begin"
#define MV_FS_API_279_112_2_18_2_39_11_35 "[MV] [CM_File_Write] file write error = %x"
#define MV_FS_API_283_112_2_18_2_39_11_36 "[MV] [CM_File_Write] end"
#define MV_FS_API_313_112_2_18_2_39_12_37 "[MV] [CM_File_Flush] file flush error code = %x"
#define MV_FS_API_319_112_2_18_2_39_12_38 "[MV]--WD:[CM_File_Flush] file flush error code = %x"
#define MV_FS_API_373_112_2_18_2_39_12_39 "[MV]--WD:[CM_File_Seek]file position parameter error = %x"
#define MV_FS_API_383_112_2_18_2_39_12_40 "[MV]--WD:[CM_File_Seek] set file pointer error code = %x"
#define MV_FS_API_391_112_2_18_2_39_12_41 "[MV]--WD:[CM_File_Seek] set file pointer error code = %x"
#define MV_FS_API_442_112_2_18_2_39_12_42 "[MV]--WD:[CM_FileExists] file exists and close file ok "
#define MV_FS_API_448_112_2_18_2_39_12_43 "[MV]--WD:[CM_FileExists] file exists and  close file fail "
#define MV_FS_API_456_112_2_18_2_39_12_44 "[MV]--WD:[CM_FileExists] file dose not exists "
#define MV_FS_API_481_112_2_18_2_39_12_45 "[MV] CM_MakeFolder after MMIAPIFMM_CreateDirectory() err: %x"
#define MV_FS_API_494_112_2_18_2_39_12_46 "[MV]--WD:[CM_MakeFolder]create directory error code = %x"
#define MV_FS_API_525_112_2_18_2_39_12_47 "[MV]--WD:[CM_DeleteFolder] Delete directory error code = %x"
#define MV_FS_API_543_112_2_18_2_39_12_48 "[MV]--WD:[CM_MoveFile] Start Move File \r\n"
#define MV_FS_API_554_112_2_18_2_39_12_49 "[MV]--WD:[CM_MoveFile] Start Move File \r\n"
#define MV_FS_API_568_112_2_18_2_39_12_50 "[MV]--WD:[CM_MoveFile] Rename File error code = %x"
#define MV_FS_API_588_112_2_18_2_39_12_51 "[MV]--WD:[CM_FindFirstFile] Start Find First File \r\n"
#define MV_FS_API_589_112_2_18_2_39_12_52 "[MV]--WD:[CM_FindFirstFile] pathname = %s"
#define MV_FS_API_615_112_2_18_2_39_12_53 "[MV]--WD:[CM_FindFirstFile] pFindData->uAttr= %d"
#define MV_FS_API_616_112_2_18_2_39_12_54 "[MV]--WD:[CM_FindFirstFile] Find First File handle= %x"
#define MV_FS_API_635_112_2_18_2_39_12_55 "[MV] [CM_FindNextFile] start Find Next File "
#define MV_FS_API_661_112_2_18_2_39_12_56 "[MV]--WD:[CM_FindNextFile] Find Next File error code = %x"
#define MV_FS_API_662_112_2_18_2_39_12_57 "[MV]--WD:[CM_FindFirstFile] pFindData->uAttr= %d"
#define MV_FS_API_680_112_2_18_2_39_12_58 "[MV]--WD:[CM_FindClose] Find close \r\n"
#define MV_FS_API_696_112_2_18_2_39_12_59 "[MV]--WD:[CM_FindClose] Find close error code = %x"
#define MV_FS_API_723_112_2_18_2_39_12_60 "[MV] [CM_File_Time]start get File Time "
#define MV_FS_API_729_112_2_18_2_39_12_61 "[MV][CM_File_Time] Get File Time error code = %x "
#define MV_FS_API_746_112_2_18_2_39_12_62 "[MV] :[CM_DeleteFile] start Delete File"
#define MV_FS_API_761_112_2_18_2_39_12_63 "[MV] [CM_DeleteFile] Delete File error code = %x"
#define MV_FS_API_778_112_2_18_2_39_13_64 "[MV] CM_GetCurFolder Entry"
#define MV_FS_API_856_112_2_18_2_39_13_65 "[MV] CM_GetUserAgent ua: %s"
#define MV_MM_API_56_112_2_18_2_39_13_66 "CM_Malloc_Mv: file=%s line=%d addr=0x%x"
#define MV_MM_API_64_112_2_18_2_39_13_67 "[MV]--WD:[CM_Malloc_MV] malloc memory error!"
#define MV_MM_API_88_112_2_18_2_39_13_68 "[MV]CM_Malloc_Mv: file=%s line=%d addr=0x%x"
#define MV_MM_API_101_112_2_18_2_39_13_69 "[MV]--WD:[CM_Realloc] Realloc memory error!"
#define MV_MM_API_118_112_2_18_2_39_13_70 "[MV]CM_Free: addr=0x%x"
#define MV_MM_API_123_112_2_18_2_39_13_71 "[MV]--WD:[CM_Free] free memory ok!"
#define MV_MM_API_130_112_2_18_2_39_13_72 "[MV]--WD:[CM_Free] free memory error!"
#define MV_MM_API_145_112_2_18_2_39_13_73 "[MV]--WD:[CM_MemSet] set memory"
#define MV_MM_API_159_112_2_18_2_39_13_74 "[MV]--WD:[CM_MemCopy] memory copy"
#define MV_MM_API_173_112_2_18_2_39_13_75 "[MV]--WD:[CM_MemCmp] memory copy parameter error!"
#define MV_MM_API_180_112_2_18_2_39_13_76 "[MV]--WD:[CM_MemCmp] memory copy"
#define MV_MM_API_199_112_2_18_2_39_14_77 "[MV]--WD:[CM_MemMove] NULL == pDes || NULL == pSrc memory move Fail!"
#define MV_MM_API_214_112_2_18_2_39_14_78 "[MV]--WD:[CM_MemMove] memory move"
#define MV_MM_API_227_112_2_18_2_39_14_79 "[MV] MV_CreatMemHandle!"
#define MV_MM_API_248_112_2_18_2_39_14_80 "[MV] MV_CreatMemHandle:Mem Handle has been initiated!"
#define MV_MM_API_258_112_2_18_2_39_14_81 "[MV] MV_CreatMemHandle fail!"
#define MV_MM_API_271_112_2_18_2_39_14_82 "[MV] MV_DeleteMemHandle!"
#define MV_NET_API_108_112_2_18_2_39_14_83 "[MV]--WD[CM_Socket_Create],socket_type error,we are use tcp socket"
#define MV_NET_API_118_112_2_18_2_39_14_84 "[MV]--WD[CM_Socket_Create], Create socket INVALID e = %d"
#define MV_NET_API_131_112_2_18_2_39_14_85 "[MV]--WD[CM_Socket_Create]socket task ID is  INVALID"
#define MV_NET_API_137_112_2_18_2_39_14_86 "[MV]--WD[CM_Socket_Create] socket  thread ID = %x"
#define MV_NET_API_143_112_2_18_2_39_14_87 "[MV]--WD[CM_Socket_Create],socket_id = %x"
#define MV_NET_API_166_112_2_18_2_39_14_88 "[MV]--WD[CM_Socket_Connect] ip dwIPv4Addr %s, nHostPort[%d]"
#define MV_NET_API_178_112_2_18_2_39_14_89 "[MV]-NOT -CHIP_ENDIAN_LITTLE-WD[CM_Socket_Connect]IPAddress[0x%x]"
#define MV_NET_API_183_112_2_18_2_39_14_90 "[MV]-CHIP_ENDIAN_LITTLE-WD[CM_Socket_Connect] IPAddress[0x%x]"
#define MV_NET_API_188_112_2_18_2_39_14_91 "[MV]--WD[CM_Socket_Connect] ip address %x, Tempaddr= %x, nHostPort = %x"
#define MV_NET_API_195_112_2_18_2_39_14_92 "[MV]--WD[CM_Socket_Connect] sa.family=0x%x ,sa.ip_addr=0x%x,sa.port=0x%x,sa.sa_data=%s"
#define MV_NET_API_214_112_2_18_2_39_14_93 "[MV]--WD[CM_Socket_Connect]  IP=%x, Port=%x, connect error, err = %d, ret = %d"
#define MV_NET_API_224_112_2_18_2_39_14_94 "[MV]--WD[CM_Socket_Connect] connect network use tcp socket ok"
#define MV_NET_API_231_112_2_18_2_39_14_95 "[MV]--WD[CM_Socket_Connect] 2 connect network use tcp socket error"
#define MV_NET_API_246_112_2_18_2_39_14_96 "[MV]--WD[CM_Socket_Release] Release socket"
#define MV_NET_API_260_112_2_18_2_39_14_97 "[MV]WD[CM_Socket_Close] close socket"
#define MV_NET_API_279_112_2_18_2_39_14_98 "[MV]--WD[CM_Socket_Receive]nBufSize = %d"
#define MV_NET_API_283_112_2_18_2_39_14_99 "[MV]--WD[CM_Socket_Receive]: nResLen=%d"
#define MV_NET_API_305_112_2_18_2_39_15_100 "[MV]--WD[CM_Socket_Send], total_len = %d"
#define MV_NET_API_315_112_2_18_2_39_15_101 "[MV]--WD[CM_Socket_Send], sent_len = %d"
#define MV_NET_API_337_112_2_18_2_39_15_102 "[MV]--WD[MV_SetPdpConnectType] Set Pdp Connect Type = %d"
#define MV_NET_API_348_112_2_18_2_39_15_103 "[MV]--WD[MV_SetPdpActiveStatus]==> set pdp status = %d"
#define MV_NET_API_366_112_2_18_2_39_15_104 "[MV]--WD[HandlePdpActiveCnf]==> pdp active result =%d"
#define MV_NET_API_375_112_2_18_2_39_15_105 "[MV]--WD[HandlePdpActiveCnf]  pdp Active successfully"
#define MV_NET_API_404_112_2_18_2_39_15_106 "[MV]--WD[HandleMVPDPMsg]MV PDP Messsage msg_ptr=PNULL"
#define MV_NET_API_409_112_2_18_2_39_15_107 "[MV]--WD[HandleMVPDPMsg]MV PDP Message msg_id=%d"
#define MV_NET_API_415_112_2_18_2_39_15_108 "[MV]--WD[HandleMVPDPMsg]MV PDP Messsage MMIPDP_ACTIVE_CNF"
#define MV_NET_API_423_112_2_18_2_39_15_109 "[MV]--WD[MV_ActivePdpContext]MV Deactive Pdp Context "
#define MV_NET_API_436_112_2_18_2_39_15_110 "[MV]--WD[MV_ActivePdpContext]MV deactive Pdp Context fail"
#define MV_NET_API_472_112_2_18_2_39_15_111 "[MV]--WD[MV_ActivePdpContext]MV Active Pdp Context"
#define MV_NET_API_488_112_2_18_2_39_15_112 "[MV]--WD[MV_ActivePdpContext]MV Active Pdp Context fail"
#define MV_NET_API_508_112_2_18_2_39_15_113 "[MV]--WD[MV_DeactivePdpContext] Deactive Pdp fail!"
#define MV_NET_API_523_112_2_18_2_39_15_114 "[MV]--WD[CM_Network_Init] network initalize ok"
#define MV_NET_API_537_112_2_18_2_39_15_115 "[MV]--WD[CM_Network_Release] network release ok"
#define MV_NET_API_549_112_2_18_2_39_15_116 "[MV]--WD[CM_Network_Start]mobile video network start hNetwork = %d,nIAP=%d"
#define MV_NET_API_580_112_2_18_2_39_15_117 "[MV]--WD[CM_Network_Stop]mobile video network stop"
#define MV_NET_API_594_112_2_18_2_39_15_118 "[MV]--WD[CM_GetNetworkStatus] get network status = %d"
#define MV_NET_API_613_112_2_18_2_39_15_119 "[MV]--WD[mv_socket_task]: begin - %d "
#define MV_NET_API_621_112_2_18_2_39_15_120 "[MV]--WD[mv_socket_task] connect event flag,time=%x"
#define MV_NET_API_633_112_2_18_2_39_15_121 "[MV]--WD[mv_socket_task] connect failed"
#define MV_NET_API_641_112_2_18_2_39_15_122 "[MV]--WD[mv_socket_task] close event flag, time=%x"
#define MV_NET_API_655_112_2_18_2_39_15_123 "[MV]--WD[mv_socket_task] read event flag, time=%x"
#define MV_NET_API_667_112_2_18_2_39_15_124 "[MV]--WD[mv_socket_task] write event flag"
#define MV_NET_API_678_112_2_18_2_39_15_125 "[MV]--WD[mv_socket_task]: unresolved EVENT - %d "
#define MV_OUTPUT_API_93_112_2_18_2_39_16_126 "[MV]--WD[CM_Font_Create]: create font failed"
#define MV_OUTPUT_API_103_112_2_18_2_39_16_127 "[MV]--WD[CM_Font_Create]: create font ok nSIze = %d, nStyle = %d"
#define MV_OUTPUT_API_116_112_2_18_2_39_16_128 "[MV]--WD[CM_Font_Release]: release font"
#define MV_OUTPUT_API_186_112_2_18_2_39_16_129 "[MV] CM_Font_TextOut hFont = %d, wchar = %x, nLength = %d, pBUffer = %x, x= %d, y =%d, vMode = %d"
#define MV_OUTPUT_API_189_112_2_18_2_39_16_130 "[MV] CM_Font_TextOut vBufWidth = %d, vBufHeight = %x, clipx = %d, clipy = %x, clipright= %d, clipbottom =%d, vColor = %d"
#define MV_OUTPUT_API_201_112_2_18_2_39_16_131 "[MV] CM_Font_TextOut str_data.wstr_len = %d, ptr = %x, %x"
#define MV_OUTPUT_API_214_112_2_18_2_39_16_132 "[MV] CM_Font_TextOut txt_color = %d"
#define MV_OUTPUT_API_399_112_2_18_2_39_16_133 "[MV]--WD[CM_GetScreenBuffer]: lcdbuff_ptr = %x"
#define MV_OUTPUT_API_407_112_2_18_2_39_16_134 "[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
#define MV_OUTPUT_API_414_112_2_18_2_39_16_135 "[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
#define MV_OUTPUT_API_421_112_2_18_2_39_16_136 "[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
#define MV_OUTPUT_API_428_112_2_18_2_39_16_137 "[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
#define MV_PATH_TRANSFER_63_112_2_18_2_39_17_138 "MV_path_transfer	new_path[%s]"
#define MV_PLAYER_API_104_112_2_18_2_39_17_139 "[MV] DisplayErrorCode error_code = %d"
#define MV_PLAYER_API_143_112_2_18_2_39_17_140 "[MV] HandleFsmMsg param is error"
#define MV_PLAYER_API_149_112_2_18_2_39_17_141 "[MV] HandleFsmMsg msg_ptr->msg_id = %x"
#define MV_PLAYER_API_176_112_2_18_2_39_17_142 "[MV] HandleFsmMsg play_type = %d, total_time = %d"
#define MV_PLAYER_API_188_112_2_18_2_39_17_143 "[MV] HandleFsmMsg buffering s_mv_control_info.state = %d"
#define MV_PLAYER_API_208_112_2_18_2_39_17_144 "[MV] HandleFsmMsg buffering s_mv_control_info.state = %d"
#define MV_PLAYER_API_227_112_2_18_2_39_17_145 "[MV] HandleFsmMsg buffer end s_mv_control_info.state = %d"
#define MV_PLAYER_API_250_112_2_18_2_39_17_146 "[MV] HandleFsmMsg play_result.error_code = %d,  s_video_player_control_info.process_time = %d"
#define MV_PLAYER_API_256_112_2_18_2_39_17_147 "[MV] HandleFsmMsg play cnf s_mv_control_info.state = %d"
#define MV_PLAYER_API_273_112_2_18_2_39_17_148 "[MV] HandleFsmMsg s_mv_control_info.timer_id = %d"
#define MV_PLAYER_API_279_112_2_18_2_39_17_149 "[MV] HandleFsmMsg play cnf error s_mv_control_info.state = %d"
#define MV_PLAYER_API_295_112_2_18_2_39_17_150 "[MV] HandleFsmMsg pause_result.error_code = %d"
#define MV_PLAYER_API_300_112_2_18_2_39_17_151 "[MV] HandleFsmMsg pause cnf s_mv_control_info.state = %d"
#define MV_PLAYER_API_327_112_2_18_2_39_18_152 "[MV] HandleFsmMsg seek_result.error_code = %d"
#define MV_PLAYER_API_349_112_2_18_2_39_18_153 "[MV] HandleFsmMsg fatal_error.error_code = %d"
#define MV_PLAYER_API_386_112_2_18_2_39_18_154 "[MV] HandleFsmMsg stop cnf s_mv_control_info.state = %d, s_mv_control_info.is_replay = %d"
#define MV_PLAYER_API_443_112_2_18_2_39_18_155 "[MV] MV_MediaPlayer_Create after:MMIAPIVP_FSMInit s_mv_control_info.vp_handle = %x"
#define MV_PLAYER_API_484_112_2_18_2_39_18_156 "[MV] CM_MediaPlayer_Create left = %d, top = %d, width = %d, height = %d, pMediaPlayerCallbackProc = %x, nProgram = %d"
#define MV_PLAYER_API_523_112_2_18_2_39_18_157 "[MV] CM_MediaPlayer_Create vp_handle = %x"
#define MV_PLAYER_API_542_112_2_18_2_39_18_158 "[MV] MV_MediaPlayer_Exit s_mv_control_info.vp_handle = %x"
#define MV_PLAYER_API_572_112_2_18_2_39_18_159 "[MV] MV_MediaPlayer_Release s_mv_control_info.vp_handle = %x"
#define MV_PLAYER_API_602_112_2_18_2_39_18_160 "[MV] CM_MediaPlayer_Release hPlayer= %x, state = %d"
#define MV_PLAYER_API_663_112_2_18_2_39_18_161 "[MV] CM_MediaPlayer_Open hPlayer = %x, pUrl= %x, nUrlSize = %d, play_type = %d"
#define MV_PLAYER_API_687_112_2_18_2_39_18_162 "[MV] CM_MediaPlayer_Open full_path_len = %d"
#define MV_PLAYER_API_693_112_2_18_2_39_18_163 "[MV] CM_MediaPlayer_Open() s_mv_control_info.total_time=%d"
#define MV_PLAYER_API_732_112_2_18_2_39_18_164 "[MV] MV_MediaPlayer_Play()    s_mv_control_info.(state:%d, play_type:%d, vp_handle:%d)"
#define MV_PLAYER_API_861_112_2_18_2_39_19_165 "[CM] CM_MediaPlayer_Play()   s_mv_control_info.state = %d"
#define MV_PLAYER_API_887_112_2_18_2_39_19_166 "[MV] MV_MediaPlayer_Pause()  s_mv_control_info.state = %d,type = %d"
#define MV_PLAYER_API_903_112_2_18_2_39_19_167 "[MV] MV_MediaPlayer_Pause()  vdeotype = PROGRAM_LIVE will MMIAPIVP_FSMStop()"
#define MV_PLAYER_API_958_112_2_18_2_39_19_168 "[MV] CM_MediaPlayer_Pause "
#define MV_PLAYER_API_985_112_2_18_2_39_19_169 "[MV] MV_MediaPlayer_Seek s_mv_control_info.process_time = %d"
#define MV_PLAYER_API_1014_112_2_18_2_39_19_170 "[MV] CM_MediaPlayer_Seek nTimeStamp = %d"
#define MV_PLAYER_API_1057_112_2_18_2_39_19_171 "[MV] MV_MediaPlayer_Release hPlayer= %x"
#define MV_PLAYER_API_1174_112_2_18_2_39_19_172 "[MV] CM_MediaPlayer_GetCurVolume s_mv_control_info.vol = %d"
#define MV_PLAYER_API_1193_112_2_18_2_39_19_173 "[MV] MV_MediaPlayer_UpdateVolume nVolume = %d"
#define MV_PLAYER_API_1210_112_2_18_2_39_19_174 "[MV] CM_MediaPlayer_SetVolume nVolume = %d"
#define MV_PLAYER_API_1247_112_2_18_2_39_19_175 "[MV] MV_MediaPlayer_GetCurTime process_time = %d"
#define MV_PLAYER_API_1261_112_2_18_2_39_20_176 "[MV] MV_MediaPlayer_GetCurTime s_mv_control_info.timer_id = %d"
#define MV_PLAYER_API_1290_112_2_18_2_39_20_177 "[MV] CM_MediaPlayer_GetCurTime process_time = %d"
#define MV_PLAYER_API_1324_112_2_18_2_39_20_178 "[MV] CM_MediaPlayer_GetPercent percent = %d"
#define MV_PLAYER_API_1337_112_2_18_2_39_20_179 "[MV] CM_MediaPlayer_CanBeCreate s_mv_control_info.vp_handle = %x"
#define MV_PLAYER_API_1358_112_2_18_2_39_20_180 "[MV] CM_MediaPlayer_RePlay hPlayer = %x, state = %d"
#define MV_PLAYER_API_1457_112_2_18_2_39_20_181 "[MV] MV_IsPlaying s_mv_control_info.state = %d"
#define MV_PLAYER_API_1477_112_2_18_2_39_20_182 "[MV] MV_IsLiving state = %d, play_type = %d"
#define MV_PLAYER_API_1560_112_2_18_2_39_20_183 "[MV] MV_IsSilentMode mode=0x%x"
#define MV_SMS_API_110_112_2_18_2_39_21_184 "[MV] MMISMS_HandleSMSMTInd ascii_vaild = %s"
#define MV_SMS_API_146_112_2_18_2_39_21_185 "[MV] MMISMS_HandleSMSMTInd check_sms = %d"
#define MV_TIMER_API_91_112_2_18_2_39_21_186 "--WD[mv_timer] call mv_callback,tick=%d"
#define MV_TIMER_API_131_112_2_18_2_39_21_187 "[MV]--WD[mv_init] s_mv_chng_freq_req_handler = %d"
#define MV_TIMER_API_162_112_2_18_2_39_21_188 "[MV]--WD[MV_SetFreqHandler] s_mv_chng_freq_req_handler = %d"
#define MV_TIMER_API_191_112_2_18_2_39_21_189 "--WD[CM_SetTimer],delta_tick=%d;nElapse=%d,%d;callback=0x%x,0x%x,task=0x%x,%x"
#define MV_TIMER_API_216_112_2_18_2_39_21_190 "[MV]--WD[CM_CancelTimer] cancel mv timer"
#define MV_TIMER_API_287_112_2_18_2_39_22_191 "[MV] MV_GetMvTaskID s_mv_task_id = %d"
#define MV_UI_API_84_112_2_18_2_39_22_192 "[MV] CM_EditDialog_Create max_len = %d, title_len = %d, default_len = %d, inputmethod = %d"
#define MV_UI_API_103_112_2_18_2_39_22_193 "[MV] cm_editdialog_release"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MOBILE_VIDEO_TRC)
TRACE_MSG(MMIMV_75_112_2_18_2_39_6_0,"[MV]mmimv.c:enter function HandlePsAndRefMsg(), MVIDEO_TIMER = 0x%x msg_id = 0x%x")
TRACE_MSG(MMIMV_EXPORT_196_112_2_18_2_39_7_1,"[MV]--WD[MMIAPIMV_Entry]:enter mv")
TRACE_MSG(MMIMV_WINTAB_159_112_2_18_2_39_8_2,"[MV] HandleMainWinMsg msg_id = 0x%x")
TRACE_MSG(MMIMV_WINTAB_319_112_2_18_2_39_8_3,"[MV] run in handleeditcommonwinmsg msg_id = %x")
TRACE_MSG(MMIMV_WINTAB_328_112_2_18_2_39_8_4,"[MV] run in allow_inputmethod = %d, max_len = %d, callback = %x, defaul_len = %d, title_len = %d")
TRACE_MSG(MMIMV_WINTAB_378_112_2_18_2_39_8_5,"[MV] run in createeditbox allow_inputmethod = %d")
TRACE_MSG(MMIMV_WINTAB_448_112_2_18_2_39_9_6,"[MV] run in handlecontactwinmsg msg_id = %x")
TRACE_MSG(MV_BASE_API_63_112_2_18_2_39_9_7,"[MV] MV_GetDiskID %x, %x")
TRACE_MSG(MV_BASE_API_123_112_2_18_2_39_9_8,"[MV] CM_GetMachineInfo pBuffer = %x, nSize = %d, nIndex = %d")
TRACE_MSG(MV_BASE_API_174_112_2_18_2_39_9_9,"[MV] [CM_Exit] mobile video exit ")
TRACE_MSG(MV_BASE_API_185_112_2_18_2_39_9_10,"[MV][MV_SetFreqHandler] ")
TRACE_MSG(MV_BASE_API_301_112_2_18_2_39_9_11,"[CM_HighLight_Always]:this is CM_HighLight_Always \r\n")
TRACE_MSG(MV_BASE_API_324_112_2_18_2_39_9_12,"[MV] CM_GetDiskSpace get error ")
TRACE_MSG(MV_BASE_API_338_112_2_18_2_39_9_13,"[MV] CM_GetDiskSpace result = %d KB")
TRACE_MSG(MV_CONTACT_API_79_112_2_18_2_39_10_14,"[MV] GetContactInfo pb_select_num = %d")
TRACE_MSG(MV_CONTACT_API_155_112_2_18_2_39_10_15,"[MV] SetContactItem id =%d")
TRACE_MSG(MV_CONTACT_API_183_112_2_18_2_39_10_16,"[MV] [InitContactItem]run in InitContactItem")
TRACE_MSG(MV_CONTACT_API_193_112_2_18_2_39_10_17,"[MV] [InitContactItem] exit InitContactItem")
TRACE_MSG(MV_CONTACT_API_203_112_2_18_2_39_10_18,"[MV] MV_ContactDialogCallbackProc nResult = %d")
TRACE_MSG(MV_CONTACT_API_229_112_2_18_2_39_10_19,"[MV] CM_ContactDialog_Create input param is null")
TRACE_MSG(MV_CONTACT_API_235_112_2_18_2_39_10_20,"--WD[CM_ContactDialog_Create]contact window d'not open")
TRACE_MSG(MV_CONTACT_API_257_112_2_18_2_39_10_21,"run in CM_ContactDialog_Release")
TRACE_MSG(MV_DATE_API_50_112_2_18_2_39_11_22,"[MV]--WD[CM_GetCurrentTime]mday=%d,mon=%d,year=%d,wday=%d")
TRACE_MSG(MV_DATE_API_56_112_2_18_2_39_11_23,"[MV]--WD[CM_GetCurrentTime]sec=%d,min=%d,hour=%d")
TRACE_MSG(MV_DATE_API_67_112_2_18_2_39_11_24,"[MV]--WD[CM_GetCurrentTime] TM_GetSysTime time_sec = %d")
TRACE_MSG(MV_DATE_API_93_112_2_18_2_39_11_25,"[MV]--WD[CM_GetSystemTime]wYear=%d,wMonth=%d,wDay=%d")
TRACE_MSG(MV_DATE_API_94_112_2_18_2_39_11_26,"[MV]--WD[CM_GetSystemTime]wHour=%d,wMinute=%d,wDay=%d")
TRACE_MSG(MV_DATE_API_95_112_2_18_2_39_11_27,"[MV]--WD[CM_GetSystemTime]wDayOfWeek=%d,wDayOfYear=%d")
TRACE_MSG(MV_FS_API_125_112_2_18_2_39_11_28,"[MV] SFS_GetAttr_MV *second = %d")
TRACE_MSG(MV_FS_API_143_112_2_18_2_39_11_29,"[MV] [CM_File_Open] dwMode[%d]")
TRACE_MSG(MV_FS_API_183_112_2_18_2_39_11_30,"[MV][CM_File_Open] open file fail")
TRACE_MSG(MV_FS_API_189_112_2_18_2_39_11_31,"[MV][CM_File_Open] open file ok handle=%x")
TRACE_MSG(MV_FS_API_211_112_2_18_2_39_11_32,"[MV][CM_File_Close] close file error code = %x ")
TRACE_MSG(MV_FS_API_244_112_2_18_2_39_11_33,"[MV] [CM_File_Read] read file error = %x")
TRACE_MSG(MV_FS_API_270_112_2_18_2_39_11_34,"[MV] [CM_File_Write] begin")
TRACE_MSG(MV_FS_API_279_112_2_18_2_39_11_35,"[MV] [CM_File_Write] file write error = %x")
TRACE_MSG(MV_FS_API_283_112_2_18_2_39_11_36,"[MV] [CM_File_Write] end")
TRACE_MSG(MV_FS_API_313_112_2_18_2_39_12_37,"[MV] [CM_File_Flush] file flush error code = %x")
TRACE_MSG(MV_FS_API_319_112_2_18_2_39_12_38,"[MV]--WD:[CM_File_Flush] file flush error code = %x")
TRACE_MSG(MV_FS_API_373_112_2_18_2_39_12_39,"[MV]--WD:[CM_File_Seek]file position parameter error = %x")
TRACE_MSG(MV_FS_API_383_112_2_18_2_39_12_40,"[MV]--WD:[CM_File_Seek] set file pointer error code = %x")
TRACE_MSG(MV_FS_API_391_112_2_18_2_39_12_41,"[MV]--WD:[CM_File_Seek] set file pointer error code = %x")
TRACE_MSG(MV_FS_API_442_112_2_18_2_39_12_42,"[MV]--WD:[CM_FileExists] file exists and close file ok ")
TRACE_MSG(MV_FS_API_448_112_2_18_2_39_12_43,"[MV]--WD:[CM_FileExists] file exists and  close file fail ")
TRACE_MSG(MV_FS_API_456_112_2_18_2_39_12_44,"[MV]--WD:[CM_FileExists] file dose not exists ")
TRACE_MSG(MV_FS_API_481_112_2_18_2_39_12_45,"[MV] CM_MakeFolder after MMIAPIFMM_CreateDirectory() err: %x")
TRACE_MSG(MV_FS_API_494_112_2_18_2_39_12_46,"[MV]--WD:[CM_MakeFolder]create directory error code = %x")
TRACE_MSG(MV_FS_API_525_112_2_18_2_39_12_47,"[MV]--WD:[CM_DeleteFolder] Delete directory error code = %x")
TRACE_MSG(MV_FS_API_543_112_2_18_2_39_12_48,"[MV]--WD:[CM_MoveFile] Start Move File \r\n")
TRACE_MSG(MV_FS_API_554_112_2_18_2_39_12_49,"[MV]--WD:[CM_MoveFile] Start Move File \r\n")
TRACE_MSG(MV_FS_API_568_112_2_18_2_39_12_50,"[MV]--WD:[CM_MoveFile] Rename File error code = %x")
TRACE_MSG(MV_FS_API_588_112_2_18_2_39_12_51,"[MV]--WD:[CM_FindFirstFile] Start Find First File \r\n")
TRACE_MSG(MV_FS_API_589_112_2_18_2_39_12_52,"[MV]--WD:[CM_FindFirstFile] pathname = %s")
TRACE_MSG(MV_FS_API_615_112_2_18_2_39_12_53,"[MV]--WD:[CM_FindFirstFile] pFindData->uAttr= %d")
TRACE_MSG(MV_FS_API_616_112_2_18_2_39_12_54,"[MV]--WD:[CM_FindFirstFile] Find First File handle= %x")
TRACE_MSG(MV_FS_API_635_112_2_18_2_39_12_55,"[MV] [CM_FindNextFile] start Find Next File ")
TRACE_MSG(MV_FS_API_661_112_2_18_2_39_12_56,"[MV]--WD:[CM_FindNextFile] Find Next File error code = %x")
TRACE_MSG(MV_FS_API_662_112_2_18_2_39_12_57,"[MV]--WD:[CM_FindFirstFile] pFindData->uAttr= %d")
TRACE_MSG(MV_FS_API_680_112_2_18_2_39_12_58,"[MV]--WD:[CM_FindClose] Find close \r\n")
TRACE_MSG(MV_FS_API_696_112_2_18_2_39_12_59,"[MV]--WD:[CM_FindClose] Find close error code = %x")
TRACE_MSG(MV_FS_API_723_112_2_18_2_39_12_60,"[MV] [CM_File_Time]start get File Time ")
TRACE_MSG(MV_FS_API_729_112_2_18_2_39_12_61,"[MV][CM_File_Time] Get File Time error code = %x ")
TRACE_MSG(MV_FS_API_746_112_2_18_2_39_12_62,"[MV] :[CM_DeleteFile] start Delete File")
TRACE_MSG(MV_FS_API_761_112_2_18_2_39_12_63,"[MV] [CM_DeleteFile] Delete File error code = %x")
TRACE_MSG(MV_FS_API_778_112_2_18_2_39_13_64,"[MV] CM_GetCurFolder Entry")
TRACE_MSG(MV_FS_API_856_112_2_18_2_39_13_65,"[MV] CM_GetUserAgent ua: %s")
TRACE_MSG(MV_MM_API_56_112_2_18_2_39_13_66,"CM_Malloc_Mv: file=%s line=%d addr=0x%x")
TRACE_MSG(MV_MM_API_64_112_2_18_2_39_13_67,"[MV]--WD:[CM_Malloc_MV] malloc memory error!")
TRACE_MSG(MV_MM_API_88_112_2_18_2_39_13_68,"[MV]CM_Malloc_Mv: file=%s line=%d addr=0x%x")
TRACE_MSG(MV_MM_API_101_112_2_18_2_39_13_69,"[MV]--WD:[CM_Realloc] Realloc memory error!")
TRACE_MSG(MV_MM_API_118_112_2_18_2_39_13_70,"[MV]CM_Free: addr=0x%x")
TRACE_MSG(MV_MM_API_123_112_2_18_2_39_13_71,"[MV]--WD:[CM_Free] free memory ok!")
TRACE_MSG(MV_MM_API_130_112_2_18_2_39_13_72,"[MV]--WD:[CM_Free] free memory error!")
TRACE_MSG(MV_MM_API_145_112_2_18_2_39_13_73,"[MV]--WD:[CM_MemSet] set memory")
TRACE_MSG(MV_MM_API_159_112_2_18_2_39_13_74,"[MV]--WD:[CM_MemCopy] memory copy")
TRACE_MSG(MV_MM_API_173_112_2_18_2_39_13_75,"[MV]--WD:[CM_MemCmp] memory copy parameter error!")
TRACE_MSG(MV_MM_API_180_112_2_18_2_39_13_76,"[MV]--WD:[CM_MemCmp] memory copy")
TRACE_MSG(MV_MM_API_199_112_2_18_2_39_14_77,"[MV]--WD:[CM_MemMove] NULL == pDes || NULL == pSrc memory move Fail!")
TRACE_MSG(MV_MM_API_214_112_2_18_2_39_14_78,"[MV]--WD:[CM_MemMove] memory move")
TRACE_MSG(MV_MM_API_227_112_2_18_2_39_14_79,"[MV] MV_CreatMemHandle!")
TRACE_MSG(MV_MM_API_248_112_2_18_2_39_14_80,"[MV] MV_CreatMemHandle:Mem Handle has been initiated!")
TRACE_MSG(MV_MM_API_258_112_2_18_2_39_14_81,"[MV] MV_CreatMemHandle fail!")
TRACE_MSG(MV_MM_API_271_112_2_18_2_39_14_82,"[MV] MV_DeleteMemHandle!")
TRACE_MSG(MV_NET_API_108_112_2_18_2_39_14_83,"[MV]--WD[CM_Socket_Create],socket_type error,we are use tcp socket")
TRACE_MSG(MV_NET_API_118_112_2_18_2_39_14_84,"[MV]--WD[CM_Socket_Create], Create socket INVALID e = %d")
TRACE_MSG(MV_NET_API_131_112_2_18_2_39_14_85,"[MV]--WD[CM_Socket_Create]socket task ID is  INVALID")
TRACE_MSG(MV_NET_API_137_112_2_18_2_39_14_86,"[MV]--WD[CM_Socket_Create] socket  thread ID = %x")
TRACE_MSG(MV_NET_API_143_112_2_18_2_39_14_87,"[MV]--WD[CM_Socket_Create],socket_id = %x")
TRACE_MSG(MV_NET_API_166_112_2_18_2_39_14_88,"[MV]--WD[CM_Socket_Connect] ip dwIPv4Addr %s, nHostPort[%d]")
TRACE_MSG(MV_NET_API_178_112_2_18_2_39_14_89,"[MV]-NOT -CHIP_ENDIAN_LITTLE-WD[CM_Socket_Connect]IPAddress[0x%x]")
TRACE_MSG(MV_NET_API_183_112_2_18_2_39_14_90,"[MV]-CHIP_ENDIAN_LITTLE-WD[CM_Socket_Connect] IPAddress[0x%x]")
TRACE_MSG(MV_NET_API_188_112_2_18_2_39_14_91,"[MV]--WD[CM_Socket_Connect] ip address %x, Tempaddr= %x, nHostPort = %x")
TRACE_MSG(MV_NET_API_195_112_2_18_2_39_14_92,"[MV]--WD[CM_Socket_Connect] sa.family=0x%x ,sa.ip_addr=0x%x,sa.port=0x%x,sa.sa_data=%s")
TRACE_MSG(MV_NET_API_214_112_2_18_2_39_14_93,"[MV]--WD[CM_Socket_Connect]  IP=%x, Port=%x, connect error, err = %d, ret = %d")
TRACE_MSG(MV_NET_API_224_112_2_18_2_39_14_94,"[MV]--WD[CM_Socket_Connect] connect network use tcp socket ok")
TRACE_MSG(MV_NET_API_231_112_2_18_2_39_14_95,"[MV]--WD[CM_Socket_Connect] 2 connect network use tcp socket error")
TRACE_MSG(MV_NET_API_246_112_2_18_2_39_14_96,"[MV]--WD[CM_Socket_Release] Release socket")
TRACE_MSG(MV_NET_API_260_112_2_18_2_39_14_97,"[MV]WD[CM_Socket_Close] close socket")
TRACE_MSG(MV_NET_API_279_112_2_18_2_39_14_98,"[MV]--WD[CM_Socket_Receive]nBufSize = %d")
TRACE_MSG(MV_NET_API_283_112_2_18_2_39_14_99,"[MV]--WD[CM_Socket_Receive]: nResLen=%d")
TRACE_MSG(MV_NET_API_305_112_2_18_2_39_15_100,"[MV]--WD[CM_Socket_Send], total_len = %d")
TRACE_MSG(MV_NET_API_315_112_2_18_2_39_15_101,"[MV]--WD[CM_Socket_Send], sent_len = %d")
TRACE_MSG(MV_NET_API_337_112_2_18_2_39_15_102,"[MV]--WD[MV_SetPdpConnectType] Set Pdp Connect Type = %d")
TRACE_MSG(MV_NET_API_348_112_2_18_2_39_15_103,"[MV]--WD[MV_SetPdpActiveStatus]==> set pdp status = %d")
TRACE_MSG(MV_NET_API_366_112_2_18_2_39_15_104,"[MV]--WD[HandlePdpActiveCnf]==> pdp active result =%d")
TRACE_MSG(MV_NET_API_375_112_2_18_2_39_15_105,"[MV]--WD[HandlePdpActiveCnf]  pdp Active successfully")
TRACE_MSG(MV_NET_API_404_112_2_18_2_39_15_106,"[MV]--WD[HandleMVPDPMsg]MV PDP Messsage msg_ptr=PNULL")
TRACE_MSG(MV_NET_API_409_112_2_18_2_39_15_107,"[MV]--WD[HandleMVPDPMsg]MV PDP Message msg_id=%d")
TRACE_MSG(MV_NET_API_415_112_2_18_2_39_15_108,"[MV]--WD[HandleMVPDPMsg]MV PDP Messsage MMIPDP_ACTIVE_CNF")
TRACE_MSG(MV_NET_API_423_112_2_18_2_39_15_109,"[MV]--WD[MV_ActivePdpContext]MV Deactive Pdp Context ")
TRACE_MSG(MV_NET_API_436_112_2_18_2_39_15_110,"[MV]--WD[MV_ActivePdpContext]MV deactive Pdp Context fail")
TRACE_MSG(MV_NET_API_472_112_2_18_2_39_15_111,"[MV]--WD[MV_ActivePdpContext]MV Active Pdp Context")
TRACE_MSG(MV_NET_API_488_112_2_18_2_39_15_112,"[MV]--WD[MV_ActivePdpContext]MV Active Pdp Context fail")
TRACE_MSG(MV_NET_API_508_112_2_18_2_39_15_113,"[MV]--WD[MV_DeactivePdpContext] Deactive Pdp fail!")
TRACE_MSG(MV_NET_API_523_112_2_18_2_39_15_114,"[MV]--WD[CM_Network_Init] network initalize ok")
TRACE_MSG(MV_NET_API_537_112_2_18_2_39_15_115,"[MV]--WD[CM_Network_Release] network release ok")
TRACE_MSG(MV_NET_API_549_112_2_18_2_39_15_116,"[MV]--WD[CM_Network_Start]mobile video network start hNetwork = %d,nIAP=%d")
TRACE_MSG(MV_NET_API_580_112_2_18_2_39_15_117,"[MV]--WD[CM_Network_Stop]mobile video network stop")
TRACE_MSG(MV_NET_API_594_112_2_18_2_39_15_118,"[MV]--WD[CM_GetNetworkStatus] get network status = %d")
TRACE_MSG(MV_NET_API_613_112_2_18_2_39_15_119,"[MV]--WD[mv_socket_task]: begin - %d ")
TRACE_MSG(MV_NET_API_621_112_2_18_2_39_15_120,"[MV]--WD[mv_socket_task] connect event flag,time=%x")
TRACE_MSG(MV_NET_API_633_112_2_18_2_39_15_121,"[MV]--WD[mv_socket_task] connect failed")
TRACE_MSG(MV_NET_API_641_112_2_18_2_39_15_122,"[MV]--WD[mv_socket_task] close event flag, time=%x")
TRACE_MSG(MV_NET_API_655_112_2_18_2_39_15_123,"[MV]--WD[mv_socket_task] read event flag, time=%x")
TRACE_MSG(MV_NET_API_667_112_2_18_2_39_15_124,"[MV]--WD[mv_socket_task] write event flag")
TRACE_MSG(MV_NET_API_678_112_2_18_2_39_15_125,"[MV]--WD[mv_socket_task]: unresolved EVENT - %d ")
TRACE_MSG(MV_OUTPUT_API_93_112_2_18_2_39_16_126,"[MV]--WD[CM_Font_Create]: create font failed")
TRACE_MSG(MV_OUTPUT_API_103_112_2_18_2_39_16_127,"[MV]--WD[CM_Font_Create]: create font ok nSIze = %d, nStyle = %d")
TRACE_MSG(MV_OUTPUT_API_116_112_2_18_2_39_16_128,"[MV]--WD[CM_Font_Release]: release font")
TRACE_MSG(MV_OUTPUT_API_186_112_2_18_2_39_16_129,"[MV] CM_Font_TextOut hFont = %d, wchar = %x, nLength = %d, pBUffer = %x, x= %d, y =%d, vMode = %d")
TRACE_MSG(MV_OUTPUT_API_189_112_2_18_2_39_16_130,"[MV] CM_Font_TextOut vBufWidth = %d, vBufHeight = %x, clipx = %d, clipy = %x, clipright= %d, clipbottom =%d, vColor = %d")
TRACE_MSG(MV_OUTPUT_API_201_112_2_18_2_39_16_131,"[MV] CM_Font_TextOut str_data.wstr_len = %d, ptr = %x, %x")
TRACE_MSG(MV_OUTPUT_API_214_112_2_18_2_39_16_132,"[MV] CM_Font_TextOut txt_color = %d")
TRACE_MSG(MV_OUTPUT_API_399_112_2_18_2_39_16_133,"[MV]--WD[CM_GetScreenBuffer]: lcdbuff_ptr = %x")
TRACE_MSG(MV_OUTPUT_API_407_112_2_18_2_39_16_134,"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d")
TRACE_MSG(MV_OUTPUT_API_414_112_2_18_2_39_16_135,"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d")
TRACE_MSG(MV_OUTPUT_API_421_112_2_18_2_39_16_136,"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d")
TRACE_MSG(MV_OUTPUT_API_428_112_2_18_2_39_16_137,"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d")
TRACE_MSG(MV_PATH_TRANSFER_63_112_2_18_2_39_17_138,"MV_path_transfer	new_path[%s]")
TRACE_MSG(MV_PLAYER_API_104_112_2_18_2_39_17_139,"[MV] DisplayErrorCode error_code = %d")
TRACE_MSG(MV_PLAYER_API_143_112_2_18_2_39_17_140,"[MV] HandleFsmMsg param is error")
TRACE_MSG(MV_PLAYER_API_149_112_2_18_2_39_17_141,"[MV] HandleFsmMsg msg_ptr->msg_id = %x")
TRACE_MSG(MV_PLAYER_API_176_112_2_18_2_39_17_142,"[MV] HandleFsmMsg play_type = %d, total_time = %d")
TRACE_MSG(MV_PLAYER_API_188_112_2_18_2_39_17_143,"[MV] HandleFsmMsg buffering s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_208_112_2_18_2_39_17_144,"[MV] HandleFsmMsg buffering s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_227_112_2_18_2_39_17_145,"[MV] HandleFsmMsg buffer end s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_250_112_2_18_2_39_17_146,"[MV] HandleFsmMsg play_result.error_code = %d,  s_video_player_control_info.process_time = %d")
TRACE_MSG(MV_PLAYER_API_256_112_2_18_2_39_17_147,"[MV] HandleFsmMsg play cnf s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_273_112_2_18_2_39_17_148,"[MV] HandleFsmMsg s_mv_control_info.timer_id = %d")
TRACE_MSG(MV_PLAYER_API_279_112_2_18_2_39_17_149,"[MV] HandleFsmMsg play cnf error s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_295_112_2_18_2_39_17_150,"[MV] HandleFsmMsg pause_result.error_code = %d")
TRACE_MSG(MV_PLAYER_API_300_112_2_18_2_39_17_151,"[MV] HandleFsmMsg pause cnf s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_327_112_2_18_2_39_18_152,"[MV] HandleFsmMsg seek_result.error_code = %d")
TRACE_MSG(MV_PLAYER_API_349_112_2_18_2_39_18_153,"[MV] HandleFsmMsg fatal_error.error_code = %d")
TRACE_MSG(MV_PLAYER_API_386_112_2_18_2_39_18_154,"[MV] HandleFsmMsg stop cnf s_mv_control_info.state = %d, s_mv_control_info.is_replay = %d")
TRACE_MSG(MV_PLAYER_API_443_112_2_18_2_39_18_155,"[MV] MV_MediaPlayer_Create after:MMIAPIVP_FSMInit s_mv_control_info.vp_handle = %x")
TRACE_MSG(MV_PLAYER_API_484_112_2_18_2_39_18_156,"[MV] CM_MediaPlayer_Create left = %d, top = %d, width = %d, height = %d, pMediaPlayerCallbackProc = %x, nProgram = %d")
TRACE_MSG(MV_PLAYER_API_523_112_2_18_2_39_18_157,"[MV] CM_MediaPlayer_Create vp_handle = %x")
TRACE_MSG(MV_PLAYER_API_542_112_2_18_2_39_18_158,"[MV] MV_MediaPlayer_Exit s_mv_control_info.vp_handle = %x")
TRACE_MSG(MV_PLAYER_API_572_112_2_18_2_39_18_159,"[MV] MV_MediaPlayer_Release s_mv_control_info.vp_handle = %x")
TRACE_MSG(MV_PLAYER_API_602_112_2_18_2_39_18_160,"[MV] CM_MediaPlayer_Release hPlayer= %x, state = %d")
TRACE_MSG(MV_PLAYER_API_663_112_2_18_2_39_18_161,"[MV] CM_MediaPlayer_Open hPlayer = %x, pUrl= %x, nUrlSize = %d, play_type = %d")
TRACE_MSG(MV_PLAYER_API_687_112_2_18_2_39_18_162,"[MV] CM_MediaPlayer_Open full_path_len = %d")
TRACE_MSG(MV_PLAYER_API_693_112_2_18_2_39_18_163,"[MV] CM_MediaPlayer_Open() s_mv_control_info.total_time=%d")
TRACE_MSG(MV_PLAYER_API_732_112_2_18_2_39_18_164,"[MV] MV_MediaPlayer_Play()    s_mv_control_info.(state:%d, play_type:%d, vp_handle:%d)")
TRACE_MSG(MV_PLAYER_API_861_112_2_18_2_39_19_165,"[CM] CM_MediaPlayer_Play()   s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_887_112_2_18_2_39_19_166,"[MV] MV_MediaPlayer_Pause()  s_mv_control_info.state = %d,type = %d")
TRACE_MSG(MV_PLAYER_API_903_112_2_18_2_39_19_167,"[MV] MV_MediaPlayer_Pause()  vdeotype = PROGRAM_LIVE will MMIAPIVP_FSMStop()")
TRACE_MSG(MV_PLAYER_API_958_112_2_18_2_39_19_168,"[MV] CM_MediaPlayer_Pause ")
TRACE_MSG(MV_PLAYER_API_985_112_2_18_2_39_19_169,"[MV] MV_MediaPlayer_Seek s_mv_control_info.process_time = %d")
TRACE_MSG(MV_PLAYER_API_1014_112_2_18_2_39_19_170,"[MV] CM_MediaPlayer_Seek nTimeStamp = %d")
TRACE_MSG(MV_PLAYER_API_1057_112_2_18_2_39_19_171,"[MV] MV_MediaPlayer_Release hPlayer= %x")
TRACE_MSG(MV_PLAYER_API_1174_112_2_18_2_39_19_172,"[MV] CM_MediaPlayer_GetCurVolume s_mv_control_info.vol = %d")
TRACE_MSG(MV_PLAYER_API_1193_112_2_18_2_39_19_173,"[MV] MV_MediaPlayer_UpdateVolume nVolume = %d")
TRACE_MSG(MV_PLAYER_API_1210_112_2_18_2_39_19_174,"[MV] CM_MediaPlayer_SetVolume nVolume = %d")
TRACE_MSG(MV_PLAYER_API_1247_112_2_18_2_39_19_175,"[MV] MV_MediaPlayer_GetCurTime process_time = %d")
TRACE_MSG(MV_PLAYER_API_1261_112_2_18_2_39_20_176,"[MV] MV_MediaPlayer_GetCurTime s_mv_control_info.timer_id = %d")
TRACE_MSG(MV_PLAYER_API_1290_112_2_18_2_39_20_177,"[MV] CM_MediaPlayer_GetCurTime process_time = %d")
TRACE_MSG(MV_PLAYER_API_1324_112_2_18_2_39_20_178,"[MV] CM_MediaPlayer_GetPercent percent = %d")
TRACE_MSG(MV_PLAYER_API_1337_112_2_18_2_39_20_179,"[MV] CM_MediaPlayer_CanBeCreate s_mv_control_info.vp_handle = %x")
TRACE_MSG(MV_PLAYER_API_1358_112_2_18_2_39_20_180,"[MV] CM_MediaPlayer_RePlay hPlayer = %x, state = %d")
TRACE_MSG(MV_PLAYER_API_1457_112_2_18_2_39_20_181,"[MV] MV_IsPlaying s_mv_control_info.state = %d")
TRACE_MSG(MV_PLAYER_API_1477_112_2_18_2_39_20_182,"[MV] MV_IsLiving state = %d, play_type = %d")
TRACE_MSG(MV_PLAYER_API_1560_112_2_18_2_39_20_183,"[MV] MV_IsSilentMode mode=0x%x")
TRACE_MSG(MV_SMS_API_110_112_2_18_2_39_21_184,"[MV] MMISMS_HandleSMSMTInd ascii_vaild = %s")
TRACE_MSG(MV_SMS_API_146_112_2_18_2_39_21_185,"[MV] MMISMS_HandleSMSMTInd check_sms = %d")
TRACE_MSG(MV_TIMER_API_91_112_2_18_2_39_21_186,"--WD[mv_timer] call mv_callback,tick=%d")
TRACE_MSG(MV_TIMER_API_131_112_2_18_2_39_21_187,"[MV]--WD[mv_init] s_mv_chng_freq_req_handler = %d")
TRACE_MSG(MV_TIMER_API_162_112_2_18_2_39_21_188,"[MV]--WD[MV_SetFreqHandler] s_mv_chng_freq_req_handler = %d")
TRACE_MSG(MV_TIMER_API_191_112_2_18_2_39_21_189,"--WD[CM_SetTimer],delta_tick=%d;nElapse=%d,%d;callback=0x%x,0x%x,task=0x%x,%x")
TRACE_MSG(MV_TIMER_API_216_112_2_18_2_39_21_190,"[MV]--WD[CM_CancelTimer] cancel mv timer")
TRACE_MSG(MV_TIMER_API_287_112_2_18_2_39_22_191,"[MV] MV_GetMvTaskID s_mv_task_id = %d")
TRACE_MSG(MV_UI_API_84_112_2_18_2_39_22_192,"[MV] CM_EditDialog_Create max_len = %d, title_len = %d, default_len = %d, inputmethod = %d")
TRACE_MSG(MV_UI_API_103_112_2_18_2_39_22_193,"[MV] cm_editdialog_release")
END_TRACE_MAP(MMI_APP_MOBILE_VIDEO_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MOBILE_VIDEO_TRC_H_

