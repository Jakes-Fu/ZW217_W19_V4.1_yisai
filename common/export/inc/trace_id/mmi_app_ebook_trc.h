/******************************************************************************
 ** File Name:      mmi_app_ebook_trc.h                                         *
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
//trace_id:171
#ifndef _MMI_APP_EBOOK_TRC_H_
#define _MMI_APP_EBOOK_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIEBOOK_BOOKSHELF_334_112_2_18_2_15_8_0 "MMIEBOOK: FmmSendMMS."
#define MMIEBOOK_BOOKSHELF_1753_112_2_18_2_15_11_1 "mmiebook_wintab.c ReadLatestList read_ok is %d"
#define MMIEBOOK_BOOKSHELF_2117_112_2_18_2_15_12_2 "EnableTxtListOptMenuItem list_data_ptr->checked = %d"
#define MMIEBOOK_BOOKSHELF_2185_112_2_18_2_15_12_3 "EnableTxtListOptMenuItem list_data_ptr->checked = %d"
#define MMIEBOOK_FILE_374_112_2_18_2_15_14_4 "MMIEBOOK_OpenShowFile invalid rights."
#define MMIEBOOK_FILE_379_112_2_18_2_15_14_5 "MMIEBOOK_OpenShowFile consume rights."
#define MMIEBOOK_FILE_394_112_2_18_2_15_14_6 "MMIEBOOK_OpenShowFile open fail"
#define MMIEBOOK_FILE_436_112_2_18_2_15_14_7 "MMIEBOOK_GetDevNameByType file_dev=%d"
#define MMIEBOOK_FILE_533_112_2_18_2_15_14_8 "GetFileOptMode error: mode is %s"
#define MMIEBOOK_FILE_641_112_2_18_2_15_14_9 "MMIEBOOK_FFS_fwrite: size is %d,len is %d"
#define MMIEBOOK_FILE_689_112_2_18_2_15_14_10 "MMIEBOOK_FFS_fclose file_handle = PNULL"
#define MMIEBOOK_FILE_722_112_2_18_2_15_14_11 "MMIEBOOK_FILE MMIEBOOK_InitFileBrief s_ebook_file_brief out of memory"
#define MMIEBOOK_FILE_826_112_2_18_2_15_15_12 "MMIEBOOK_OpenLatestListFile file_handle is SFS_INVALID_HANDLE"
#define MMIEBOOK_FILE_954_112_2_18_2_15_15_13 "MMIEBOOK_ReadLatestListFile:  type is %d"
#define MMIEBOOK_FILE_970_112_2_18_2_15_15_14 "MMIEBOOK_ReadLatestListFile FILE_VER_ID fail ! str_len is %d"
#define MMIEBOOK_FILE_976_112_2_18_2_15_15_15 "MMIEBOOK_ReadLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_996_112_2_18_2_15_15_16 "MMIEBOOK_ReadLatestListFile FILE_INFO fail ! str_len is %d"
#define MMIEBOOK_FILE_1026_112_2_18_2_15_15_17 "MMIEBOOK_ReadLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_1061_112_2_18_2_15_15_18 "MMIEBOOK_WriteLatestListFile type is %d"
#define MMIEBOOK_FILE_1077_112_2_18_2_15_15_19 "MMIEBOOK_WriteLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_1096_112_2_18_2_15_15_20 "MMIEBOOK_WriteLatestListFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_1107_112_2_18_2_15_15_21 "MMIEBOOK_WriteLatestListFile result is %d"
#define MMIEBOOK_FILE_1190_112_2_18_2_15_15_22 "MMIEBOOK_IsLatestListFileVersionVaild version is wrong"
#define MMIEBOOK_FILE_1198_112_2_18_2_15_15_23 "MMIEBOOK_IsLatestListFileVersionVaild result is %d"
#define MMIEBOOK_FILE_1261_112_2_18_2_15_15_24 "MMIAPIEBOOK_IsFileUsed  %d, %d ,%d , %d"
#define MMIEBOOK_FILE_1291_112_2_18_2_15_16_25 "MMIAPIEBOOK_IsFileUsed  result %d"
#define MMIEBOOK_FILE_1330_112_2_18_2_15_16_26 "Bookmark File open fail"
#define MMIEBOOK_FILE_1427_112_2_18_2_15_16_27 "MMIEBOOK_ReadBookmarkFile FILE_INFO fail ! str_len is %d"
#define MMIEBOOK_FILE_1433_112_2_18_2_15_16_28 "MMIEBOOK_ReadBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_1452_112_2_18_2_15_16_29 "MMIEBOOK_ReadBookmarkFile FILE_INFO fail ! str_len is %d"
#define MMIEBOOK_FILE_1458_112_2_18_2_15_16_30 "MMIEBOOK_ReadBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_1514_112_2_18_2_15_16_31 "MMIEBOOK_WriteBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_1534_112_2_18_2_15_16_32 "MMIEBOOK_WriteBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d"
#define MMIEBOOK_FILE_1545_112_2_18_2_15_16_33 "MMIEBOOK_WriteBookmarkFile result is %d"
#define MMIEBOOK_TASK_297_112_2_18_2_15_19_34 "MMIEBOOK_TASK MMIEBOOK_AllocEbookTaskRes enter"
#define MMIEBOOK_TASK_374_112_2_18_2_15_20_35 "MMIEBOOK_TASK MMIEBOOK_AllocEbookTaskRes error"
#define MMIEBOOK_TASK_389_112_2_18_2_15_20_36 "MMIEBOOK_TASK MMIEBOOK_FreeEbookTaskRes enter"
#define MMIEBOOK_TASK_415_112_2_18_2_15_20_37 "MMIEBOOK_TASK MMIEbook_HandleTaskMessage msg_id == %x"
#define MMIEBOOK_TASK_420_112_2_18_2_15_20_38 "MMIEBOOK_task MMIEBOOK_MSG_SHOW_TXT_WIN_OPEN file_dev = %d"
#define MMIEBOOK_TASK_587_112_2_18_2_15_20_39 "MMIEBOOK_task CountOnePageLines font height = %d, lines = %d"
#define MMIEBOOK_TASK_610_112_2_18_2_15_20_40 "MMIEBOOK_task LayoutSegmentLineInfo enter time = %d"
#define MMIEBOOK_TASK_635_112_2_18_2_15_20_41 "MMIEBOOK_task LayoutSegmentLineInfo leave time = %d, line num = %d"
#define MMIEBOOK_TASK_823_112_2_18_2_15_20_42 "MMIEBOOK_task MMIEBOOK_TurnLineOrPage enter"
#define MMIEBOOK_TASK_1021_112_2_18_2_15_21_43 "MMIEbook_TASK PutShowTxt total_page is %d"
#define MMIEBOOK_TASK_1058_112_2_18_2_15_21_44 "MMIEBOOK_task PutShowTxt from next segment start_line = %d"
#define MMIEBOOK_TASK_1208_112_2_18_2_15_21_45 "MMIEBOOK_TASK ReadSegmentDataFromFile enter time = %d"
#define MMIEBOOK_TASK_1249_112_2_18_2_15_21_46 "MMIEBOOK_TASK ReadSegmentDataFromFile cur_show_txt_ptr->ucs2_str_len = %d"
#define MMIEBOOK_TASK_1253_112_2_18_2_15_21_47 "MMIEBOOK_TASK ReadSegmentDataFromFile leave time = %d"
#define MMIEBOOK_TASK_1336_112_2_18_2_15_21_48 "MMIEBOOK_task MakePreSeg enter time = %d"
#define MMIEBOOK_TASK_1410_112_2_18_2_15_22_49 "MMIEBOOK_task MakePreSeg leave time = %d"
#define MMIEBOOK_TASK_1423_112_2_18_2_15_22_50 "MMIEBOOK_task MakeNextSeg enter time = %d"
#define MMIEBOOK_TASK_1448_112_2_18_2_15_22_51 "MMIEBOOK_task MakeNextSeg leave time = %d"
#define MMIEBOOK_TASK_1463_112_2_18_2_15_22_52 "MMIEBOOK_task ReMakeSegmentAfterScreenSwitch enter time = %d"
#define MMIEBOOK_TASK_1494_112_2_18_2_15_22_53 "MMIEBOOK_task SwitchAndMakePreSegment enter time = %d"
#define MMIEBOOK_TASK_1537_112_2_18_2_15_22_54 "MMIEBOOK_task SwitchAndMakePreSegment leave time = %d"
#define MMIEBOOK_TASK_1548_112_2_18_2_15_22_55 "MMIEBOOK_task SwitchAndMakeNextSegment enter time = %d"
#define MMIEBOOK_TASK_1562_112_2_18_2_15_22_56 "MMIEBOOK_task SwitchAndMakeNextSegment leave time = %d"
#define MMIEBOOK_TASK_1572_112_2_18_2_15_22_57 "MMIEBOOK_task ClearFileSegmentInfo enter"
#define MMIEBOOK_TASK_1656_112_2_18_2_15_22_58 "MMIEBOOK_task InitEbookShowRectInfo enter"
#define MMIEBOOK_TASK_1697_112_2_18_2_15_22_59 "MMIEBOOK_TASK MMIEbook_GetFont font is %d"
#define MMIEBOOK_TASK_1737_112_2_18_2_15_22_60 "MMIEBOOK_TASK MMIEBOOK_IsInLastPage == %d"
#define MMIEBOOK_TASK_1757_112_2_18_2_15_22_61 "MMIEBOOK_TASK MMIEBOOK_IsInFirstPage == %d"
#define MMIEBOOK_TASK_1786_112_2_18_2_15_22_62 "MMIEBOOK_AddBookmark show_ptr=%d,tab=%d"
#define MMIEBOOK_TASK_1792_112_2_18_2_15_22_63 "MMIEBOOK_AddBookmark:s_ebook_show_info.cur_line is %d,s_ebook_show_info.cur_show_txt_ptr->total_line_num is %d"
#define MMIEBOOK_TASK_1797_112_2_18_2_15_22_64 "MMIEBOOK_AddBookmark s_ebook_show_info.bookmark_info.total_num=%d"
#define MMIEBOOK_TASK_1968_112_2_18_2_15_23_65 "MMIEBOOK_TASK InitFileSegmentInfo enter"
#define MMIEBOOK_TASK_1984_112_2_18_2_15_23_66 "MMIEBOOK_task MakeSegmentByOffset msg == %x"
#define MMIEBOOK_TASK_1997_112_2_18_2_15_23_67 "MakeSegmentByOffset:msg is %d"
#define MMIEBOOK_TASK_2440_112_2_18_2_15_24_68 "MMIEBOOK_TASK MMIEBOOK_IsTurnLineOrPage result = %d"
#define MMIEBOOK_WINTAB_1343_112_2_18_2_15_27_69 "MMIEBOOK_WINTAB MMIAPIEBOOK_ManagerWinMsg_4Fmm file_dev = %d, name_ptr = %x, name_length = %d, file_size= %d, full_name_ptr = %x, full_name_length =%d"
#define MMIEBOOK_WINTAB_1366_112_2_18_2_15_27_70 "EnterTxtorList"
#define MMIEBOOK_WINTAB_1514_112_2_18_2_15_27_71 "HandleEnterTxtContentWinMsg: MSG_KEYDOWN_RED "
#define MMIEBOOK_WINTAB_1546_112_2_18_2_15_27_72 "HandleEnterTxtContentWinMsg MMIAPICC_IsAnimWinOpen"
#define MMIEBOOK_WINTAB_1553_112_2_18_2_15_27_73 "mmiebook_wintab.c EnterTxtContent msg_id is %x"
#define MMIEBOOK_WINTAB_1609_112_2_18_2_15_27_74 "ebook EnterTxtContent msg_id is %x"
#define MMIEBOOK_WINTAB_1665_112_2_18_2_15_28_75 "MMIEBOOK_WINTAB HandleTxtShowWinMsg msg_id is %x"
#define MMIEBOOK_WINTAB_2112_112_2_18_2_15_28_76 "MMIEBOOK_WINTAB HandleTxtShowWinMsg MMIAPITTS_PlayText()  MMIEBOOK_MSG_TTS_PLAY"
#define MMIEBOOK_WINTAB_2209_112_2_18_2_15_29_77 "MMIEBOOK_WINTAB HandleTxtShowWinMsg  MMITTS MSG_TTS_OVER MSG received, tts play state = %d"
#define MMIEBOOK_WINTAB_3496_112_2_18_2_15_31_78 "mmiebook_wintab.c HandleTxtShowOptWinMsg node_id is %d"
#define MMIEBOOK_WINTAB_4072_112_2_18_2_15_33_79 "MMIEBOOK_SetTTSInfo cur_selection = %d"
#define MMIEBOOK_WINTAB_4257_112_2_18_2_15_33_80 "EnterTxtorList"
#define MMIEBOOK_WINTAB_4267_112_2_18_2_15_33_81 "MMIEBOOK_WINTAB EnterTxtorList enter"
#define MMIEBOOK_WINTAB_4306_112_2_18_2_15_33_82 "MMIEBOOK_WINTAB HandleEbookSetTTSOpenCloseWindow msg_id = %x"
#define MMIEBOOK_WINTAB_4362_112_2_18_2_15_33_83 "MMIEBOOK_WINTAB HandleEbookTTSVolSetWindow msg_id = %x"
#define MMIEBOOK_WINTAB_4532_112_2_18_2_15_33_84 "MMIAPIFMM_IsFileExist: find first sfs_handle = %d"
#define MMIEBOOK_WINTAB_4562_112_2_18_2_15_34_85 "MMIAPIFMM_IsFileExist: result = %d"
#define MMIEBOOK_WINTAB_4610_112_2_18_2_15_34_86 "MMIEbook: SetEbookFontSize font_size=%d,"
#define MMIEBOOK_WINTAB_4647_112_2_18_2_15_34_87 "MMIEBOOK_TASK MMIEBOOK_GetIsFullScreenOff not fullsecreen == %d"
#define MMIEBOOK_WINTAB_6303_112_2_18_2_15_37_88 "HandleEbookSlideTpDown:slide_state=%d"
#define MMIEBOOK_WINTAB_6341_112_2_18_2_15_37_89 "HandleEbookSlideTpMove:slide_state=%d"
#define MMIEBOOK_WINTAB_6421_112_2_18_2_15_37_90 "HandleEbookSlideTpUp:slide_state=%d"
#define MMIEBOOK_WINTAB_6633_112_2_18_2_15_38_91 "RenderEffect:time=%d"
#define MMIEBOOK_WINTAB_6873_112_2_18_2_15_38_92 "UpdateLayer:error!"
#define MMIEBOOK_WINTAB_6940_112_2_18_2_15_38_93 "mmi EbookApplet_HandleEvent msg_id = 0x%04x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_EBOOK_TRC)
TRACE_MSG(MMIEBOOK_BOOKSHELF_334_112_2_18_2_15_8_0,"MMIEBOOK: FmmSendMMS.")
TRACE_MSG(MMIEBOOK_BOOKSHELF_1753_112_2_18_2_15_11_1,"mmiebook_wintab.c ReadLatestList read_ok is %d")
TRACE_MSG(MMIEBOOK_BOOKSHELF_2117_112_2_18_2_15_12_2,"EnableTxtListOptMenuItem list_data_ptr->checked = %d")
TRACE_MSG(MMIEBOOK_BOOKSHELF_2185_112_2_18_2_15_12_3,"EnableTxtListOptMenuItem list_data_ptr->checked = %d")
TRACE_MSG(MMIEBOOK_FILE_374_112_2_18_2_15_14_4,"MMIEBOOK_OpenShowFile invalid rights.")
TRACE_MSG(MMIEBOOK_FILE_379_112_2_18_2_15_14_5,"MMIEBOOK_OpenShowFile consume rights.")
TRACE_MSG(MMIEBOOK_FILE_394_112_2_18_2_15_14_6,"MMIEBOOK_OpenShowFile open fail")
TRACE_MSG(MMIEBOOK_FILE_436_112_2_18_2_15_14_7,"MMIEBOOK_GetDevNameByType file_dev=%d")
TRACE_MSG(MMIEBOOK_FILE_533_112_2_18_2_15_14_8,"GetFileOptMode error: mode is %s")
TRACE_MSG(MMIEBOOK_FILE_641_112_2_18_2_15_14_9,"MMIEBOOK_FFS_fwrite: size is %d,len is %d")
TRACE_MSG(MMIEBOOK_FILE_689_112_2_18_2_15_14_10,"MMIEBOOK_FFS_fclose file_handle = PNULL")
TRACE_MSG(MMIEBOOK_FILE_722_112_2_18_2_15_14_11,"MMIEBOOK_FILE MMIEBOOK_InitFileBrief s_ebook_file_brief out of memory")
TRACE_MSG(MMIEBOOK_FILE_826_112_2_18_2_15_15_12,"MMIEBOOK_OpenLatestListFile file_handle is SFS_INVALID_HANDLE")
TRACE_MSG(MMIEBOOK_FILE_954_112_2_18_2_15_15_13,"MMIEBOOK_ReadLatestListFile:  type is %d")
TRACE_MSG(MMIEBOOK_FILE_970_112_2_18_2_15_15_14,"MMIEBOOK_ReadLatestListFile FILE_VER_ID fail ! str_len is %d")
TRACE_MSG(MMIEBOOK_FILE_976_112_2_18_2_15_15_15,"MMIEBOOK_ReadLatestListFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_996_112_2_18_2_15_15_16,"MMIEBOOK_ReadLatestListFile FILE_INFO fail ! str_len is %d")
TRACE_MSG(MMIEBOOK_FILE_1026_112_2_18_2_15_15_17,"MMIEBOOK_ReadLatestListFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_1061_112_2_18_2_15_15_18,"MMIEBOOK_WriteLatestListFile type is %d")
TRACE_MSG(MMIEBOOK_FILE_1077_112_2_18_2_15_15_19,"MMIEBOOK_WriteLatestListFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_1096_112_2_18_2_15_15_20,"MMIEBOOK_WriteLatestListFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_1107_112_2_18_2_15_15_21,"MMIEBOOK_WriteLatestListFile result is %d")
TRACE_MSG(MMIEBOOK_FILE_1190_112_2_18_2_15_15_22,"MMIEBOOK_IsLatestListFileVersionVaild version is wrong")
TRACE_MSG(MMIEBOOK_FILE_1198_112_2_18_2_15_15_23,"MMIEBOOK_IsLatestListFileVersionVaild result is %d")
TRACE_MSG(MMIEBOOK_FILE_1261_112_2_18_2_15_15_24,"MMIAPIEBOOK_IsFileUsed  %d, %d ,%d , %d")
TRACE_MSG(MMIEBOOK_FILE_1291_112_2_18_2_15_16_25,"MMIAPIEBOOK_IsFileUsed  result %d")
TRACE_MSG(MMIEBOOK_FILE_1330_112_2_18_2_15_16_26,"Bookmark File open fail")
TRACE_MSG(MMIEBOOK_FILE_1427_112_2_18_2_15_16_27,"MMIEBOOK_ReadBookmarkFile FILE_INFO fail ! str_len is %d")
TRACE_MSG(MMIEBOOK_FILE_1433_112_2_18_2_15_16_28,"MMIEBOOK_ReadBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_1452_112_2_18_2_15_16_29,"MMIEBOOK_ReadBookmarkFile FILE_INFO fail ! str_len is %d")
TRACE_MSG(MMIEBOOK_FILE_1458_112_2_18_2_15_16_30,"MMIEBOOK_ReadBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_1514_112_2_18_2_15_16_31,"MMIEBOOK_WriteBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_1534_112_2_18_2_15_16_32,"MMIEBOOK_WriteBookmarkFile: MMIEBOOK_FFS_fseek error ret is %d")
TRACE_MSG(MMIEBOOK_FILE_1545_112_2_18_2_15_16_33,"MMIEBOOK_WriteBookmarkFile result is %d")
TRACE_MSG(MMIEBOOK_TASK_297_112_2_18_2_15_19_34,"MMIEBOOK_TASK MMIEBOOK_AllocEbookTaskRes enter")
TRACE_MSG(MMIEBOOK_TASK_374_112_2_18_2_15_20_35,"MMIEBOOK_TASK MMIEBOOK_AllocEbookTaskRes error")
TRACE_MSG(MMIEBOOK_TASK_389_112_2_18_2_15_20_36,"MMIEBOOK_TASK MMIEBOOK_FreeEbookTaskRes enter")
TRACE_MSG(MMIEBOOK_TASK_415_112_2_18_2_15_20_37,"MMIEBOOK_TASK MMIEbook_HandleTaskMessage msg_id == %x")
TRACE_MSG(MMIEBOOK_TASK_420_112_2_18_2_15_20_38,"MMIEBOOK_task MMIEBOOK_MSG_SHOW_TXT_WIN_OPEN file_dev = %d")
TRACE_MSG(MMIEBOOK_TASK_587_112_2_18_2_15_20_39,"MMIEBOOK_task CountOnePageLines font height = %d, lines = %d")
TRACE_MSG(MMIEBOOK_TASK_610_112_2_18_2_15_20_40,"MMIEBOOK_task LayoutSegmentLineInfo enter time = %d")
TRACE_MSG(MMIEBOOK_TASK_635_112_2_18_2_15_20_41,"MMIEBOOK_task LayoutSegmentLineInfo leave time = %d, line num = %d")
TRACE_MSG(MMIEBOOK_TASK_823_112_2_18_2_15_20_42,"MMIEBOOK_task MMIEBOOK_TurnLineOrPage enter")
TRACE_MSG(MMIEBOOK_TASK_1021_112_2_18_2_15_21_43,"MMIEbook_TASK PutShowTxt total_page is %d")
TRACE_MSG(MMIEBOOK_TASK_1058_112_2_18_2_15_21_44,"MMIEBOOK_task PutShowTxt from next segment start_line = %d")
TRACE_MSG(MMIEBOOK_TASK_1208_112_2_18_2_15_21_45,"MMIEBOOK_TASK ReadSegmentDataFromFile enter time = %d")
TRACE_MSG(MMIEBOOK_TASK_1249_112_2_18_2_15_21_46,"MMIEBOOK_TASK ReadSegmentDataFromFile cur_show_txt_ptr->ucs2_str_len = %d")
TRACE_MSG(MMIEBOOK_TASK_1253_112_2_18_2_15_21_47,"MMIEBOOK_TASK ReadSegmentDataFromFile leave time = %d")
TRACE_MSG(MMIEBOOK_TASK_1336_112_2_18_2_15_21_48,"MMIEBOOK_task MakePreSeg enter time = %d")
TRACE_MSG(MMIEBOOK_TASK_1410_112_2_18_2_15_22_49,"MMIEBOOK_task MakePreSeg leave time = %d")
TRACE_MSG(MMIEBOOK_TASK_1423_112_2_18_2_15_22_50,"MMIEBOOK_task MakeNextSeg enter time = %d")
TRACE_MSG(MMIEBOOK_TASK_1448_112_2_18_2_15_22_51,"MMIEBOOK_task MakeNextSeg leave time = %d")
TRACE_MSG(MMIEBOOK_TASK_1463_112_2_18_2_15_22_52,"MMIEBOOK_task ReMakeSegmentAfterScreenSwitch enter time = %d")
TRACE_MSG(MMIEBOOK_TASK_1494_112_2_18_2_15_22_53,"MMIEBOOK_task SwitchAndMakePreSegment enter time = %d")
TRACE_MSG(MMIEBOOK_TASK_1537_112_2_18_2_15_22_54,"MMIEBOOK_task SwitchAndMakePreSegment leave time = %d")
TRACE_MSG(MMIEBOOK_TASK_1548_112_2_18_2_15_22_55,"MMIEBOOK_task SwitchAndMakeNextSegment enter time = %d")
TRACE_MSG(MMIEBOOK_TASK_1562_112_2_18_2_15_22_56,"MMIEBOOK_task SwitchAndMakeNextSegment leave time = %d")
TRACE_MSG(MMIEBOOK_TASK_1572_112_2_18_2_15_22_57,"MMIEBOOK_task ClearFileSegmentInfo enter")
TRACE_MSG(MMIEBOOK_TASK_1656_112_2_18_2_15_22_58,"MMIEBOOK_task InitEbookShowRectInfo enter")
TRACE_MSG(MMIEBOOK_TASK_1697_112_2_18_2_15_22_59,"MMIEBOOK_TASK MMIEbook_GetFont font is %d")
TRACE_MSG(MMIEBOOK_TASK_1737_112_2_18_2_15_22_60,"MMIEBOOK_TASK MMIEBOOK_IsInLastPage == %d")
TRACE_MSG(MMIEBOOK_TASK_1757_112_2_18_2_15_22_61,"MMIEBOOK_TASK MMIEBOOK_IsInFirstPage == %d")
TRACE_MSG(MMIEBOOK_TASK_1786_112_2_18_2_15_22_62,"MMIEBOOK_AddBookmark show_ptr=%d,tab=%d")
TRACE_MSG(MMIEBOOK_TASK_1792_112_2_18_2_15_22_63,"MMIEBOOK_AddBookmark:s_ebook_show_info.cur_line is %d,s_ebook_show_info.cur_show_txt_ptr->total_line_num is %d")
TRACE_MSG(MMIEBOOK_TASK_1797_112_2_18_2_15_22_64,"MMIEBOOK_AddBookmark s_ebook_show_info.bookmark_info.total_num=%d")
TRACE_MSG(MMIEBOOK_TASK_1968_112_2_18_2_15_23_65,"MMIEBOOK_TASK InitFileSegmentInfo enter")
TRACE_MSG(MMIEBOOK_TASK_1984_112_2_18_2_15_23_66,"MMIEBOOK_task MakeSegmentByOffset msg == %x")
TRACE_MSG(MMIEBOOK_TASK_1997_112_2_18_2_15_23_67,"MakeSegmentByOffset:msg is %d")
TRACE_MSG(MMIEBOOK_TASK_2440_112_2_18_2_15_24_68,"MMIEBOOK_TASK MMIEBOOK_IsTurnLineOrPage result = %d")
TRACE_MSG(MMIEBOOK_WINTAB_1343_112_2_18_2_15_27_69,"MMIEBOOK_WINTAB MMIAPIEBOOK_ManagerWinMsg_4Fmm file_dev = %d, name_ptr = %x, name_length = %d, file_size= %d, full_name_ptr = %x, full_name_length =%d")
TRACE_MSG(MMIEBOOK_WINTAB_1366_112_2_18_2_15_27_70,"EnterTxtorList")
TRACE_MSG(MMIEBOOK_WINTAB_1514_112_2_18_2_15_27_71,"HandleEnterTxtContentWinMsg: MSG_KEYDOWN_RED ")
TRACE_MSG(MMIEBOOK_WINTAB_1546_112_2_18_2_15_27_72,"HandleEnterTxtContentWinMsg MMIAPICC_IsAnimWinOpen")
TRACE_MSG(MMIEBOOK_WINTAB_1553_112_2_18_2_15_27_73,"mmiebook_wintab.c EnterTxtContent msg_id is %x")
TRACE_MSG(MMIEBOOK_WINTAB_1609_112_2_18_2_15_27_74,"ebook EnterTxtContent msg_id is %x")
TRACE_MSG(MMIEBOOK_WINTAB_1665_112_2_18_2_15_28_75,"MMIEBOOK_WINTAB HandleTxtShowWinMsg msg_id is %x")
TRACE_MSG(MMIEBOOK_WINTAB_2112_112_2_18_2_15_28_76,"MMIEBOOK_WINTAB HandleTxtShowWinMsg MMIAPITTS_PlayText()  MMIEBOOK_MSG_TTS_PLAY")
TRACE_MSG(MMIEBOOK_WINTAB_2209_112_2_18_2_15_29_77,"MMIEBOOK_WINTAB HandleTxtShowWinMsg  MMITTS MSG_TTS_OVER MSG received, tts play state = %d")
TRACE_MSG(MMIEBOOK_WINTAB_3496_112_2_18_2_15_31_78,"mmiebook_wintab.c HandleTxtShowOptWinMsg node_id is %d")
TRACE_MSG(MMIEBOOK_WINTAB_4072_112_2_18_2_15_33_79,"MMIEBOOK_SetTTSInfo cur_selection = %d")
TRACE_MSG(MMIEBOOK_WINTAB_4257_112_2_18_2_15_33_80,"EnterTxtorList")
TRACE_MSG(MMIEBOOK_WINTAB_4267_112_2_18_2_15_33_81,"MMIEBOOK_WINTAB EnterTxtorList enter")
TRACE_MSG(MMIEBOOK_WINTAB_4306_112_2_18_2_15_33_82,"MMIEBOOK_WINTAB HandleEbookSetTTSOpenCloseWindow msg_id = %x")
TRACE_MSG(MMIEBOOK_WINTAB_4362_112_2_18_2_15_33_83,"MMIEBOOK_WINTAB HandleEbookTTSVolSetWindow msg_id = %x")
TRACE_MSG(MMIEBOOK_WINTAB_4532_112_2_18_2_15_33_84,"MMIAPIFMM_IsFileExist: find first sfs_handle = %d")
TRACE_MSG(MMIEBOOK_WINTAB_4562_112_2_18_2_15_34_85,"MMIAPIFMM_IsFileExist: result = %d")
TRACE_MSG(MMIEBOOK_WINTAB_4610_112_2_18_2_15_34_86,"MMIEbook: SetEbookFontSize font_size=%d,")
TRACE_MSG(MMIEBOOK_WINTAB_4647_112_2_18_2_15_34_87,"MMIEBOOK_TASK MMIEBOOK_GetIsFullScreenOff not fullsecreen == %d")
TRACE_MSG(MMIEBOOK_WINTAB_6303_112_2_18_2_15_37_88,"HandleEbookSlideTpDown:slide_state=%d")
TRACE_MSG(MMIEBOOK_WINTAB_6341_112_2_18_2_15_37_89,"HandleEbookSlideTpMove:slide_state=%d")
TRACE_MSG(MMIEBOOK_WINTAB_6421_112_2_18_2_15_37_90,"HandleEbookSlideTpUp:slide_state=%d")
TRACE_MSG(MMIEBOOK_WINTAB_6633_112_2_18_2_15_38_91,"RenderEffect:time=%d")
TRACE_MSG(MMIEBOOK_WINTAB_6873_112_2_18_2_15_38_92,"UpdateLayer:error!")
TRACE_MSG(MMIEBOOK_WINTAB_6940_112_2_18_2_15_38_93,"mmi EbookApplet_HandleEvent msg_id = 0x%04x")
END_TRACE_MAP(MMI_APP_EBOOK_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_EBOOK_TRC_H_

