/******************************************************************************
 ** File Name:      mmi_app_kmovie_trc.h                                         *
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
//trace_id:188
#ifndef _MMI_APP_KMOVIE_TRC_H_
#define _MMI_APP_KMOVIE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIKM_MAIN_67_112_2_18_2_31_15_0 "[MMIKM] MMIAPIKM_IsSupportfile:s_is_kmv_file = %d"
#define MMIKM_MAIN_87_112_2_18_2_31_15_1 "[MMIKM] MMIAPIKM_IsSupportfile:s_is_hmv_file = %d"
#define MMIKM_MAIN_98_112_2_18_2_31_15_2 "[MMIKM] MMIAPIKM_IsSupportfile:s_is_kmv_file = %d"
#define MMIKM_MAIN_150_112_2_18_2_31_15_3 "[MMIKM] MMIAPIKM_CkeckSupportfile:s_is_kmv_file = %d"
#define MMIKM_MAIN_192_112_2_18_2_31_15_4 "[MMIKM] MMIAPIKM_MainEntry call running =%d"
#define MMIKM_MAIN_208_112_2_18_2_31_15_5 "[MMIKM] MMIAPIKM_MainEntry:sdcard status is  DEVICE_STATUS_ERROR"
#define MMIKM_MAIN_237_112_2_18_2_31_15_6 "[MMIKM] MMIAPIKM_MainEntry:Alloc memory fail "
#define MMIKM_PLAYLIST_637_112_2_18_2_31_17_7 "[MMIKM] ReadFromFile entry"
#define MMIKM_PLAYLIST_672_112_2_18_2_31_17_8 "[MMIKM] ReadFromFile save_result = %d"
#define MMIKM_PLAYLIST_697_112_2_18_2_31_17_9 "[MMIKM]: MMIKM_ReadFile: recode = %d"
#define MMIKM_PLAYLIST_736_112_2_18_2_31_17_10 "[MMIKM]: MMIKM_WritePlayListFile: recode = %d"
#define MMIKM_PLAYLIST_753_112_2_18_2_31_17_11 "[MMIKM] GetFilerInfo param is null"
#define MMIKM_PLAYLIST_902_112_2_18_2_31_18_12 "[MMIKM] MMIKM_PlayList_BeginLoad locallist_type = %d"
#define MMIKM_PLAYLIST_968_112_2_18_2_31_18_13 "[MMIKM] MMIKM_PlayList_BeginLoad leave "
#define MMIKM_PLAYLIST_993_112_2_18_2_31_18_14 "[MMIKM] MMIKM_PlayList_BeginLoad locallist_type = %d"
#define MMIKM_PLAYLIST_1025_112_2_18_2_31_18_15 "[MMIKM] MMIKM_PlayList_BeginLoad s_km_play_list_array_ptr[locallist_type] 1 = %d"
#define MMIKM_PLAYLIST_1063_112_2_18_2_31_18_16 "[MMIKM] MMIKM_PlayList_BeginLoad sd2_file_array = %d"
#define MMIKM_PLAYLIST_1065_112_2_18_2_31_18_17 "[MMIKM] MMIKM_PlayList_BeginLoad s_km_play_list_array_ptr[locallist_type] 2 = %d"
#define MMIKM_PLAYLIST_1072_112_2_18_2_31_18_18 "[MMIKM] MMIKM_PlayList_BeginLoad leave "
#define MMIKM_PLAYLIST_1083_112_2_18_2_31_18_19 "[MMIKM] MMIKM_PlayList_EndLoad locallist_type = %d"
#define MMIKM_PLAYLIST_1099_112_2_18_2_31_18_20 "[MMIKM] SavePlaylistToFile locallist_type = %d"
#define MMIKM_PLAYLIST_1129_112_2_18_2_31_18_21 "[MMIKM] SavePlaylistToFilesave_result = %d"
#define MMIKM_PLAYLIST_1162_112_2_18_2_31_18_22 "[MMIKM] GetPlaylistNum list_type = %d, locallist_num = %d"
#define MMIKM_PLAYLIST_1179_112_2_18_2_31_18_23 "[MMIKM] GetPlaylistItemInfo list_type = %d, list_index = %d"
#define MMIKM_PLAYLIST_1184_112_2_18_2_31_18_24 "[[MMIKM] GetPlaylistItemInfo list_type = %d, result = %d, list_index = %d"
#define MMIKM_PLAYLIST_1200_112_2_18_2_31_18_25 "[MMIKM] DeletePlaylistItemInfo list_type = %d, list_index = %d"
#define MMIKM_PLAYLIST_1205_112_2_18_2_31_18_26 "[[MMIKM] DeletePlaylistItemInfo list_type = %d, result = %d, list_index = %d"
#define MMIKM_PLAYLIST_1226_112_2_18_2_31_18_27 "SetPlaylistCount : total_num = %d"
#define MMIKM_PLAYLIST_1303_112_2_18_2_31_18_28 "[MMIKM] MMIKM_PlayListGetItem result = %d"
#define MMIKM_PLAYLIST_1318_112_2_18_2_31_18_29 "[MMIKM] GetPlaylistAllItem entry"
#define MMIKM_PLAYLIST_1322_112_2_18_2_31_19_30 "[MMIKM] GetPlaylistAllItem list_num = %d"
#define MMIKM_PLAYLIST_1374_112_2_18_2_31_19_31 "[MMIKM] GetPlaylistInforItemInfo result = %d"
#define MMIKM_PLAYLIST_1390_112_2_18_2_31_19_32 "[MMIKM] SavePlaylistInforToFile locallist_type = %d"
#define MMIKM_PLAYLIST_1420_112_2_18_2_31_19_33 "[MMIKM] SavePlaylistInforToFile save_result = %d"
#define MMIKM_PLAYLIST_1436_112_2_18_2_31_19_34 "[MMIKM] SavePlayInforToFile list_type = %d"
#define MMIKM_PLAYLIST_1466_112_2_18_2_31_19_35 "[MMIKM] SavePlayInforToFile save_result = %d"
#define MMIKM_PLAYLIST_1485_112_2_18_2_31_19_36 "[MMIKM] SetPlaylistTypeNum type_num[%d] = %d"
#define MMIKM_PLAYLIST_1618_112_2_18_2_31_19_37 "[KM] GetFileItemTypeInfor:file_type_e = %d"
#define MMIKM_PLAYLIST_1648_112_2_18_2_31_19_38 "[KM] GetFileItemInfor THIS IS SMV FILE"
#define MMIKM_PLAYLIST_1656_112_2_18_2_31_19_39 "[KM] GetFileItemInfor THIS IS KingMV FILE"
#define MMIKM_PLAYLIST_1677_112_2_18_2_31_19_40 "[KM] GetFileItemInfor:result = %d"
#define MMIKM_PLAYLIST_1681_112_2_18_2_31_19_41 "[KM] GetFileItemInfor:file name = %s"
#define MMIKM_PLAYLIST_1748_112_2_18_2_31_20_42 "[KM] GetFileItemInfor: type = %d"
#define MMIKM_PLAYLIST_1760_112_2_18_2_31_20_43 "[KM] GetFileItemInfor:time len = %d"
#define MMIKM_PLAYLIST_1772_112_2_18_2_31_20_44 "[KM] GetFileItemInfor:year len = %d"
#define MMIKM_PLAYLIST_1795_112_2_18_2_31_20_45 "[MMIKM] GetFileNameForReadWrite locallist_type = %d"
#define MMIKM_PLAYLIST_1818_112_2_18_2_31_20_46 "[MMIKM] GetFileNameForReadWrite result = %d"
#define MMIKM_PLAYLIST_1945_112_2_18_2_31_20_47 "[MMIKM] MMIKM_GetLastPlayInfor recode = %d"
#define MMIKM_PLAYLIST_1985_112_2_18_2_31_20_48 "[MMIKM] MMIKM_GetMovieTypeNum num = %d"
#define MMIKM_PLAYLIST_2143_112_2_18_2_31_20_49 "[MMIKM] MMIKM_ResetAllInfo :recode=%d"
#define MMIKM_PLAYLIST_2168_112_2_18_2_31_20_50 "[MMIKM] MMIKM_MemSetForPlaylistInfo :recode=%d"
#define MMIKM_PLAYLIST_2204_112_2_18_2_31_20_51 "[MMIKM] MMIKM_AlloclMemoForPlaylistInfo :fail "
#define MMIKM_PLAYLIST_2210_112_2_18_2_31_21_52 "[MMIKM] MMIKM_AlloclMemoForPlaylistInfo :ok "
#define MMIKM_PLAYLIST_2285_112_2_18_2_31_21_53 "[KM] OpenVideoSecurty handle=0x%x"
#define MMIKM_PLAYLIST_2298_112_2_18_2_31_21_54 "[KM] OpenVideoSecurty handle=0x%x"
#define MMIKM_PLAYLIST_2305_112_2_18_2_31_21_55 "[KM] OpenVideoSecurty recode=%d"
#define MMIKM_PLAYLIST_2318_112_2_18_2_31_21_56 "[KM] CloseVideoSecurty handle=0x%x"
#define MMIKM_PLAYLIST_2337_112_2_18_2_31_21_57 "[KM] CloseVideoSecurty handle=0x%x"
#define MMIKM_PLAYLIST_2411_112_2_18_2_31_21_58 "[KM] GetVideoSecurtyThumbnailData result = %d"
#define MMIKM_PLAYLIST_2418_112_2_18_2_31_21_59 "[KM] GetVideoSecurtyThumbnailData recode = %d"
#define MMIKM_PLAYLIST_2652_112_2_18_2_31_21_60 "[KM] MMIKM_GetSmvFileContent:file_full_path_ptr = NULL"
#define MMIKM_PLAYLIST_2660_112_2_18_2_31_21_61 "[KM] MMIKM_GetSmvFileContent:Mp4FileOpen fail"
#define MMIKM_PLAYLIST_2666_112_2_18_2_31_21_62 "[KM] MMIKM_GetSmvFileContent data_len =%d"
#define MMIKM_PLAYLIST_2719_112_2_18_2_31_22_63 "[KM] MMIKM_GetKmvFileContent:file_full_path_ptr = NULL"
#define MMIKM_PLAYLIST_2727_112_2_18_2_31_22_64 "[KM] MMIKM_GetKmvFileContent:OpenVideoSecurty fail"
#define MMIKM_PLAYLIST_2733_112_2_18_2_31_22_65 "[KM] GetKmvFileContent data_len =%d"
#define MMIKM_PLAYLIST_2785_112_2_18_2_31_22_66 "[Mmikm_playlist.c]: ParseKmvFile"
#define MMIKM_PLAYLIST_2790_112_2_18_2_31_22_67 "[Mmikm_playlist.c]: buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2804_112_2_18_2_31_22_68 "[Mmikm_playlist.c]: item_size_offset= %d"
#define MMIKM_PLAYLIST_2812_112_2_18_2_31_22_69 "[Mmikm_playlist.c]: get year buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2815_112_2_18_2_31_22_70 "[Mmikm_playlist.c]: buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2817_112_2_18_2_31_22_71 "[Mmikm_playlist.c]: get year item_size %0x,%d"
#define MMIKM_PLAYLIST_2835_112_2_18_2_31_22_72 "[Mmikm_playlist.c]: item_size_offset= %d"
#define MMIKM_PLAYLIST_2841_112_2_18_2_31_22_73 "[Mmikm_playlist.c]: get time buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2845_112_2_18_2_31_22_74 "[Mmikm_playlist.c]: get time item_size %0x,%d"
#define MMIKM_PLAYLIST_2863_112_2_18_2_31_22_75 "[Mmikm_playlist.c]: item_size_offset= %d"
#define MMIKM_PLAYLIST_2870_112_2_18_2_31_22_76 "[Mmikm_playlist.c]: get name buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2874_112_2_18_2_31_22_77 "[Mmikm_playlist.c]: get name item_size %0x,%d"
#define MMIKM_PLAYLIST_2895_112_2_18_2_31_22_78 "[Mmikm_playlist.c]: item_size_offset= %d"
#define MMIKM_PLAYLIST_2902_112_2_18_2_31_22_79 "[Mmikm_playlist.c]: get actor buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2907_112_2_18_2_31_22_80 "[Mmikm_playlist.c]: get actor item_size %0x,%d"
#define MMIKM_PLAYLIST_2928_112_2_18_2_31_22_81 "[Mmikm_playlist.c]: item_size_offset= %d"
#define MMIKM_PLAYLIST_2935_112_2_18_2_31_22_82 "[Mmikm_playlist.c]: get synop buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2940_112_2_18_2_31_22_83 "[Mmikm_playlist.c]: get synop item_size %0x,%d"
#define MMIKM_PLAYLIST_2961_112_2_18_2_31_22_84 "[Mmikm_playlist.c]: item_size_offset= %d"
#define MMIKM_PLAYLIST_2968_112_2_18_2_31_22_85 "[Mmikm_playlist.c]: get type buf_ptr %0x,%0x,%0x,%0x"
#define MMIKM_PLAYLIST_2973_112_2_18_2_31_22_86 "[Mmikm_playlist.c]: get type item_size %0x,%d"
#define MMIKM_PLAYLIST_2990_112_2_18_2_31_22_87 "[Mmikm_playlist.c]: item_size_offset= %d"
#define MMIKM_PLAYLIST_3065_112_2_18_2_31_22_88 "[MMIKM] MMIAPIKM_FileArrayDestroy :leave"
#define MMIKM_WINTAB_298_112_2_18_2_31_23_89 "[KM]ShowFormAnimCtrl entry"
#define MMIKM_WINTAB_301_112_2_18_2_31_23_90 "[KM]ShowFormAnimCtrl file_item_info_ptr = NULL"
#define MMIKM_WINTAB_311_112_2_18_2_31_23_91 "[KM]ShowFormAnimCtrl width = %d"
#define MMIKM_WINTAB_312_112_2_18_2_31_23_92 "[KM]ShowFormAnimCtrl height = %d"
#define MMIKM_WINTAB_427_112_2_18_2_31_23_93 "[KM] GetFileItemInfor THIS IS KingMV FILE"
#define MMIKM_WINTAB_438_112_2_18_2_31_23_94 "[KM] GetFileItemInfor:actor len = %d"
#define MMIKM_WINTAB_449_112_2_18_2_31_23_95 "[KM] GetFileItemInfor:synop len = %d"
#define MMIKM_WINTAB_462_112_2_18_2_31_23_96 "[KM]ShowFormLableCtrl acotr len = %d"
#define MMIKM_WINTAB_482_112_2_18_2_31_23_97 "[KM]ShowFormLableCtrl final acotr len = %d"
#define MMIKM_WINTAB_493_112_2_18_2_31_23_98 "[KM]ShowFormLableCtrl synop len = %d"
#define MMIKM_WINTAB_600_112_2_18_2_31_24_99 "HandlePlaylistSearchWaitingWin entry!"
#define MMIKM_WINTAB_731_112_2_18_2_31_24_100 "HandlePlaylistSearchWaitingWin MSG_KM_LOADING_SAVE!"
#define MMIKM_WINTAB_1418_112_2_18_2_31_25_101 " PNULL == need_item_data_ptr "
#define MMIKM_WINTAB_1429_112_2_18_2_31_25_102 " PNULL == need_item_content_ptr"
#define MMIKM_WINTAB_1587_112_2_18_2_31_25_103 "CreateKMMainWinMenuList alloc fail !"
#define MMIKM_WINTAB_1591_112_2_18_2_31_25_104 "CreateKMMainWinMenuList is_lastplay = %d"
#define MMIKM_WINTAB_1697_112_2_18_2_31_26_105 "MMIKM_MemSetForPlaylistInfo fail"
#define MMIKM_WINTAB_1781_112_2_18_2_31_26_106 "[MMIKM] MainWinPopWinOpenMenuHandle :%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_KMOVIE_TRC)
TRACE_MSG(MMIKM_MAIN_67_112_2_18_2_31_15_0,"[MMIKM] MMIAPIKM_IsSupportfile:s_is_kmv_file = %d")
TRACE_MSG(MMIKM_MAIN_87_112_2_18_2_31_15_1,"[MMIKM] MMIAPIKM_IsSupportfile:s_is_hmv_file = %d")
TRACE_MSG(MMIKM_MAIN_98_112_2_18_2_31_15_2,"[MMIKM] MMIAPIKM_IsSupportfile:s_is_kmv_file = %d")
TRACE_MSG(MMIKM_MAIN_150_112_2_18_2_31_15_3,"[MMIKM] MMIAPIKM_CkeckSupportfile:s_is_kmv_file = %d")
TRACE_MSG(MMIKM_MAIN_192_112_2_18_2_31_15_4,"[MMIKM] MMIAPIKM_MainEntry call running =%d")
TRACE_MSG(MMIKM_MAIN_208_112_2_18_2_31_15_5,"[MMIKM] MMIAPIKM_MainEntry:sdcard status is  DEVICE_STATUS_ERROR")
TRACE_MSG(MMIKM_MAIN_237_112_2_18_2_31_15_6,"[MMIKM] MMIAPIKM_MainEntry:Alloc memory fail ")
TRACE_MSG(MMIKM_PLAYLIST_637_112_2_18_2_31_17_7,"[MMIKM] ReadFromFile entry")
TRACE_MSG(MMIKM_PLAYLIST_672_112_2_18_2_31_17_8,"[MMIKM] ReadFromFile save_result = %d")
TRACE_MSG(MMIKM_PLAYLIST_697_112_2_18_2_31_17_9,"[MMIKM]: MMIKM_ReadFile: recode = %d")
TRACE_MSG(MMIKM_PLAYLIST_736_112_2_18_2_31_17_10,"[MMIKM]: MMIKM_WritePlayListFile: recode = %d")
TRACE_MSG(MMIKM_PLAYLIST_753_112_2_18_2_31_17_11,"[MMIKM] GetFilerInfo param is null")
TRACE_MSG(MMIKM_PLAYLIST_902_112_2_18_2_31_18_12,"[MMIKM] MMIKM_PlayList_BeginLoad locallist_type = %d")
TRACE_MSG(MMIKM_PLAYLIST_968_112_2_18_2_31_18_13,"[MMIKM] MMIKM_PlayList_BeginLoad leave ")
TRACE_MSG(MMIKM_PLAYLIST_993_112_2_18_2_31_18_14,"[MMIKM] MMIKM_PlayList_BeginLoad locallist_type = %d")
TRACE_MSG(MMIKM_PLAYLIST_1025_112_2_18_2_31_18_15,"[MMIKM] MMIKM_PlayList_BeginLoad s_km_play_list_array_ptr[locallist_type] 1 = %d")
TRACE_MSG(MMIKM_PLAYLIST_1063_112_2_18_2_31_18_16,"[MMIKM] MMIKM_PlayList_BeginLoad sd2_file_array = %d")
TRACE_MSG(MMIKM_PLAYLIST_1065_112_2_18_2_31_18_17,"[MMIKM] MMIKM_PlayList_BeginLoad s_km_play_list_array_ptr[locallist_type] 2 = %d")
TRACE_MSG(MMIKM_PLAYLIST_1072_112_2_18_2_31_18_18,"[MMIKM] MMIKM_PlayList_BeginLoad leave ")
TRACE_MSG(MMIKM_PLAYLIST_1083_112_2_18_2_31_18_19,"[MMIKM] MMIKM_PlayList_EndLoad locallist_type = %d")
TRACE_MSG(MMIKM_PLAYLIST_1099_112_2_18_2_31_18_20,"[MMIKM] SavePlaylistToFile locallist_type = %d")
TRACE_MSG(MMIKM_PLAYLIST_1129_112_2_18_2_31_18_21,"[MMIKM] SavePlaylistToFilesave_result = %d")
TRACE_MSG(MMIKM_PLAYLIST_1162_112_2_18_2_31_18_22,"[MMIKM] GetPlaylistNum list_type = %d, locallist_num = %d")
TRACE_MSG(MMIKM_PLAYLIST_1179_112_2_18_2_31_18_23,"[MMIKM] GetPlaylistItemInfo list_type = %d, list_index = %d")
TRACE_MSG(MMIKM_PLAYLIST_1184_112_2_18_2_31_18_24,"[[MMIKM] GetPlaylistItemInfo list_type = %d, result = %d, list_index = %d")
TRACE_MSG(MMIKM_PLAYLIST_1200_112_2_18_2_31_18_25,"[MMIKM] DeletePlaylistItemInfo list_type = %d, list_index = %d")
TRACE_MSG(MMIKM_PLAYLIST_1205_112_2_18_2_31_18_26,"[[MMIKM] DeletePlaylistItemInfo list_type = %d, result = %d, list_index = %d")
TRACE_MSG(MMIKM_PLAYLIST_1226_112_2_18_2_31_18_27,"SetPlaylistCount : total_num = %d")
TRACE_MSG(MMIKM_PLAYLIST_1303_112_2_18_2_31_18_28,"[MMIKM] MMIKM_PlayListGetItem result = %d")
TRACE_MSG(MMIKM_PLAYLIST_1318_112_2_18_2_31_18_29,"[MMIKM] GetPlaylistAllItem entry")
TRACE_MSG(MMIKM_PLAYLIST_1322_112_2_18_2_31_19_30,"[MMIKM] GetPlaylistAllItem list_num = %d")
TRACE_MSG(MMIKM_PLAYLIST_1374_112_2_18_2_31_19_31,"[MMIKM] GetPlaylistInforItemInfo result = %d")
TRACE_MSG(MMIKM_PLAYLIST_1390_112_2_18_2_31_19_32,"[MMIKM] SavePlaylistInforToFile locallist_type = %d")
TRACE_MSG(MMIKM_PLAYLIST_1420_112_2_18_2_31_19_33,"[MMIKM] SavePlaylistInforToFile save_result = %d")
TRACE_MSG(MMIKM_PLAYLIST_1436_112_2_18_2_31_19_34,"[MMIKM] SavePlayInforToFile list_type = %d")
TRACE_MSG(MMIKM_PLAYLIST_1466_112_2_18_2_31_19_35,"[MMIKM] SavePlayInforToFile save_result = %d")
TRACE_MSG(MMIKM_PLAYLIST_1485_112_2_18_2_31_19_36,"[MMIKM] SetPlaylistTypeNum type_num[%d] = %d")
TRACE_MSG(MMIKM_PLAYLIST_1618_112_2_18_2_31_19_37,"[KM] GetFileItemTypeInfor:file_type_e = %d")
TRACE_MSG(MMIKM_PLAYLIST_1648_112_2_18_2_31_19_38,"[KM] GetFileItemInfor THIS IS SMV FILE")
TRACE_MSG(MMIKM_PLAYLIST_1656_112_2_18_2_31_19_39,"[KM] GetFileItemInfor THIS IS KingMV FILE")
TRACE_MSG(MMIKM_PLAYLIST_1677_112_2_18_2_31_19_40,"[KM] GetFileItemInfor:result = %d")
TRACE_MSG(MMIKM_PLAYLIST_1681_112_2_18_2_31_19_41,"[KM] GetFileItemInfor:file name = %s")
TRACE_MSG(MMIKM_PLAYLIST_1748_112_2_18_2_31_20_42,"[KM] GetFileItemInfor: type = %d")
TRACE_MSG(MMIKM_PLAYLIST_1760_112_2_18_2_31_20_43,"[KM] GetFileItemInfor:time len = %d")
TRACE_MSG(MMIKM_PLAYLIST_1772_112_2_18_2_31_20_44,"[KM] GetFileItemInfor:year len = %d")
TRACE_MSG(MMIKM_PLAYLIST_1795_112_2_18_2_31_20_45,"[MMIKM] GetFileNameForReadWrite locallist_type = %d")
TRACE_MSG(MMIKM_PLAYLIST_1818_112_2_18_2_31_20_46,"[MMIKM] GetFileNameForReadWrite result = %d")
TRACE_MSG(MMIKM_PLAYLIST_1945_112_2_18_2_31_20_47,"[MMIKM] MMIKM_GetLastPlayInfor recode = %d")
TRACE_MSG(MMIKM_PLAYLIST_1985_112_2_18_2_31_20_48,"[MMIKM] MMIKM_GetMovieTypeNum num = %d")
TRACE_MSG(MMIKM_PLAYLIST_2143_112_2_18_2_31_20_49,"[MMIKM] MMIKM_ResetAllInfo :recode=%d")
TRACE_MSG(MMIKM_PLAYLIST_2168_112_2_18_2_31_20_50,"[MMIKM] MMIKM_MemSetForPlaylistInfo :recode=%d")
TRACE_MSG(MMIKM_PLAYLIST_2204_112_2_18_2_31_20_51,"[MMIKM] MMIKM_AlloclMemoForPlaylistInfo :fail ")
TRACE_MSG(MMIKM_PLAYLIST_2210_112_2_18_2_31_21_52,"[MMIKM] MMIKM_AlloclMemoForPlaylistInfo :ok ")
TRACE_MSG(MMIKM_PLAYLIST_2285_112_2_18_2_31_21_53,"[KM] OpenVideoSecurty handle=0x%x")
TRACE_MSG(MMIKM_PLAYLIST_2298_112_2_18_2_31_21_54,"[KM] OpenVideoSecurty handle=0x%x")
TRACE_MSG(MMIKM_PLAYLIST_2305_112_2_18_2_31_21_55,"[KM] OpenVideoSecurty recode=%d")
TRACE_MSG(MMIKM_PLAYLIST_2318_112_2_18_2_31_21_56,"[KM] CloseVideoSecurty handle=0x%x")
TRACE_MSG(MMIKM_PLAYLIST_2337_112_2_18_2_31_21_57,"[KM] CloseVideoSecurty handle=0x%x")
TRACE_MSG(MMIKM_PLAYLIST_2411_112_2_18_2_31_21_58,"[KM] GetVideoSecurtyThumbnailData result = %d")
TRACE_MSG(MMIKM_PLAYLIST_2418_112_2_18_2_31_21_59,"[KM] GetVideoSecurtyThumbnailData recode = %d")
TRACE_MSG(MMIKM_PLAYLIST_2652_112_2_18_2_31_21_60,"[KM] MMIKM_GetSmvFileContent:file_full_path_ptr = NULL")
TRACE_MSG(MMIKM_PLAYLIST_2660_112_2_18_2_31_21_61,"[KM] MMIKM_GetSmvFileContent:Mp4FileOpen fail")
TRACE_MSG(MMIKM_PLAYLIST_2666_112_2_18_2_31_21_62,"[KM] MMIKM_GetSmvFileContent data_len =%d")
TRACE_MSG(MMIKM_PLAYLIST_2719_112_2_18_2_31_22_63,"[KM] MMIKM_GetKmvFileContent:file_full_path_ptr = NULL")
TRACE_MSG(MMIKM_PLAYLIST_2727_112_2_18_2_31_22_64,"[KM] MMIKM_GetKmvFileContent:OpenVideoSecurty fail")
TRACE_MSG(MMIKM_PLAYLIST_2733_112_2_18_2_31_22_65,"[KM] GetKmvFileContent data_len =%d")
TRACE_MSG(MMIKM_PLAYLIST_2785_112_2_18_2_31_22_66,"[Mmikm_playlist.c]: ParseKmvFile")
TRACE_MSG(MMIKM_PLAYLIST_2790_112_2_18_2_31_22_67,"[Mmikm_playlist.c]: buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2804_112_2_18_2_31_22_68,"[Mmikm_playlist.c]: item_size_offset= %d")
TRACE_MSG(MMIKM_PLAYLIST_2812_112_2_18_2_31_22_69,"[Mmikm_playlist.c]: get year buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2815_112_2_18_2_31_22_70,"[Mmikm_playlist.c]: buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2817_112_2_18_2_31_22_71,"[Mmikm_playlist.c]: get year item_size %0x,%d")
TRACE_MSG(MMIKM_PLAYLIST_2835_112_2_18_2_31_22_72,"[Mmikm_playlist.c]: item_size_offset= %d")
TRACE_MSG(MMIKM_PLAYLIST_2841_112_2_18_2_31_22_73,"[Mmikm_playlist.c]: get time buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2845_112_2_18_2_31_22_74,"[Mmikm_playlist.c]: get time item_size %0x,%d")
TRACE_MSG(MMIKM_PLAYLIST_2863_112_2_18_2_31_22_75,"[Mmikm_playlist.c]: item_size_offset= %d")
TRACE_MSG(MMIKM_PLAYLIST_2870_112_2_18_2_31_22_76,"[Mmikm_playlist.c]: get name buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2874_112_2_18_2_31_22_77,"[Mmikm_playlist.c]: get name item_size %0x,%d")
TRACE_MSG(MMIKM_PLAYLIST_2895_112_2_18_2_31_22_78,"[Mmikm_playlist.c]: item_size_offset= %d")
TRACE_MSG(MMIKM_PLAYLIST_2902_112_2_18_2_31_22_79,"[Mmikm_playlist.c]: get actor buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2907_112_2_18_2_31_22_80,"[Mmikm_playlist.c]: get actor item_size %0x,%d")
TRACE_MSG(MMIKM_PLAYLIST_2928_112_2_18_2_31_22_81,"[Mmikm_playlist.c]: item_size_offset= %d")
TRACE_MSG(MMIKM_PLAYLIST_2935_112_2_18_2_31_22_82,"[Mmikm_playlist.c]: get synop buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2940_112_2_18_2_31_22_83,"[Mmikm_playlist.c]: get synop item_size %0x,%d")
TRACE_MSG(MMIKM_PLAYLIST_2961_112_2_18_2_31_22_84,"[Mmikm_playlist.c]: item_size_offset= %d")
TRACE_MSG(MMIKM_PLAYLIST_2968_112_2_18_2_31_22_85,"[Mmikm_playlist.c]: get type buf_ptr %0x,%0x,%0x,%0x")
TRACE_MSG(MMIKM_PLAYLIST_2973_112_2_18_2_31_22_86,"[Mmikm_playlist.c]: get type item_size %0x,%d")
TRACE_MSG(MMIKM_PLAYLIST_2990_112_2_18_2_31_22_87,"[Mmikm_playlist.c]: item_size_offset= %d")
TRACE_MSG(MMIKM_PLAYLIST_3065_112_2_18_2_31_22_88,"[MMIKM] MMIAPIKM_FileArrayDestroy :leave")
TRACE_MSG(MMIKM_WINTAB_298_112_2_18_2_31_23_89,"[KM]ShowFormAnimCtrl entry")
TRACE_MSG(MMIKM_WINTAB_301_112_2_18_2_31_23_90,"[KM]ShowFormAnimCtrl file_item_info_ptr = NULL")
TRACE_MSG(MMIKM_WINTAB_311_112_2_18_2_31_23_91,"[KM]ShowFormAnimCtrl width = %d")
TRACE_MSG(MMIKM_WINTAB_312_112_2_18_2_31_23_92,"[KM]ShowFormAnimCtrl height = %d")
TRACE_MSG(MMIKM_WINTAB_427_112_2_18_2_31_23_93,"[KM] GetFileItemInfor THIS IS KingMV FILE")
TRACE_MSG(MMIKM_WINTAB_438_112_2_18_2_31_23_94,"[KM] GetFileItemInfor:actor len = %d")
TRACE_MSG(MMIKM_WINTAB_449_112_2_18_2_31_23_95,"[KM] GetFileItemInfor:synop len = %d")
TRACE_MSG(MMIKM_WINTAB_462_112_2_18_2_31_23_96,"[KM]ShowFormLableCtrl acotr len = %d")
TRACE_MSG(MMIKM_WINTAB_482_112_2_18_2_31_23_97,"[KM]ShowFormLableCtrl final acotr len = %d")
TRACE_MSG(MMIKM_WINTAB_493_112_2_18_2_31_23_98,"[KM]ShowFormLableCtrl synop len = %d")
TRACE_MSG(MMIKM_WINTAB_600_112_2_18_2_31_24_99,"HandlePlaylistSearchWaitingWin entry!")
TRACE_MSG(MMIKM_WINTAB_731_112_2_18_2_31_24_100,"HandlePlaylistSearchWaitingWin MSG_KM_LOADING_SAVE!")
TRACE_MSG(MMIKM_WINTAB_1418_112_2_18_2_31_25_101," PNULL == need_item_data_ptr ")
TRACE_MSG(MMIKM_WINTAB_1429_112_2_18_2_31_25_102," PNULL == need_item_content_ptr")
TRACE_MSG(MMIKM_WINTAB_1587_112_2_18_2_31_25_103,"CreateKMMainWinMenuList alloc fail !")
TRACE_MSG(MMIKM_WINTAB_1591_112_2_18_2_31_25_104,"CreateKMMainWinMenuList is_lastplay = %d")
TRACE_MSG(MMIKM_WINTAB_1697_112_2_18_2_31_26_105,"MMIKM_MemSetForPlaylistInfo fail")
TRACE_MSG(MMIKM_WINTAB_1781_112_2_18_2_31_26_106,"[MMIKM] MainWinPopWinOpenMenuHandle :%d")
END_TRACE_MAP(MMI_APP_KMOVIE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_KMOVIE_TRC_H_

