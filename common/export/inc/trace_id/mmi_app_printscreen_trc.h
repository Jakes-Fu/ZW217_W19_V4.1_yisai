/******************************************************************************
 ** File Name:      mmi_app_printscreen_trc.h                                         *
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
//trace_id:213
#ifndef _MMI_APP_PRINTSCREEN_TRC_H_
#define _MMI_APP_PRINTSCREEN_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPTSCR_MAIN_113_112_2_18_2_47_34_0 "MMIAPIPTSCR_PreCheck: sd not exist!"
#define MMIPTSCR_MAIN_120_112_2_18_2_47_34_1 "MMIAPIPTSCR_PreCheck: sd no space!"
#define MMIPTSCR_MAIN_162_112_2_18_2_47_34_2 "MMIAPIPTSCR_Print -- :GetBltInfoForPrintScreen result is failed!"
#define MMIPTSCR_MAIN_183_112_2_18_2_47_34_3 "MMIAPIPTSCR_Print -- :MMIAPIFMM_CreateDir result is failed!"
#define MMIPTSCR_MAIN_193_112_2_18_2_47_34_4 "MMIAPIPTSCR_Print -- :GenPrexfixName result is failed!"
#define MMIPTSCR_MAIN_215_112_2_18_2_47_34_5 "MMIAPIPTSCR_Print -- :MMIAPIPTSCR_WriteBMPFile result is failed!"
#define MMIPTSCR_MAIN_223_112_2_18_2_47_34_6 "MMIAPIPTSCR_Print -- :MMIAPIFMM_CombineFullPath result is failed!"
#define MMIPTSCR_MAIN_288_112_2_18_2_47_34_7 "MMIAPIPTSCR_Print -- :WriteBMPFile result is failed!"
#define MMIPTSCR_MAIN_369_112_2_18_2_47_34_8 "MMIAPIPTSCR_ChangeRunMode: sd not exist!"
#define MMIPTSCR_MAIN_403_112_2_18_2_47_34_9 "StartOrStopAutoRunTimer, MMK_CreateTimerCallback MMIPTSCR_AUTORUN_TIMEOUT = %d"
#define MMIPTSCR_MAIN_465_112_2_18_2_47_34_10 "HandleAutoRunTimeTimerEvent -- :PTSCR_Print result is failed!"
#define MMIPTSCR_SAVE_128_112_2_18_2_47_35_11 "MMIAPIPTSCR_WriteBMPFile MMIAPIFMM_CreateFile fail!"
#define MMIPTSCR_SAVE_163_112_2_18_2_47_35_12 "MMIAPIPTSCR_WriteBMPFile WriteBMPFile bmp is to large! w = %d, nLineBytes = %d"
#define MMIPTSCR_SAVE_171_112_2_18_2_47_35_13 "MMIAPIPTSCR_WriteBMPFile WriteBMPFile alloc fail!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PRINTSCREEN_TRC)
TRACE_MSG(MMIPTSCR_MAIN_113_112_2_18_2_47_34_0,"MMIAPIPTSCR_PreCheck: sd not exist!")
TRACE_MSG(MMIPTSCR_MAIN_120_112_2_18_2_47_34_1,"MMIAPIPTSCR_PreCheck: sd no space!")
TRACE_MSG(MMIPTSCR_MAIN_162_112_2_18_2_47_34_2,"MMIAPIPTSCR_Print -- :GetBltInfoForPrintScreen result is failed!")
TRACE_MSG(MMIPTSCR_MAIN_183_112_2_18_2_47_34_3,"MMIAPIPTSCR_Print -- :MMIAPIFMM_CreateDir result is failed!")
TRACE_MSG(MMIPTSCR_MAIN_193_112_2_18_2_47_34_4,"MMIAPIPTSCR_Print -- :GenPrexfixName result is failed!")
TRACE_MSG(MMIPTSCR_MAIN_215_112_2_18_2_47_34_5,"MMIAPIPTSCR_Print -- :MMIAPIPTSCR_WriteBMPFile result is failed!")
TRACE_MSG(MMIPTSCR_MAIN_223_112_2_18_2_47_34_6,"MMIAPIPTSCR_Print -- :MMIAPIFMM_CombineFullPath result is failed!")
TRACE_MSG(MMIPTSCR_MAIN_288_112_2_18_2_47_34_7,"MMIAPIPTSCR_Print -- :WriteBMPFile result is failed!")
TRACE_MSG(MMIPTSCR_MAIN_369_112_2_18_2_47_34_8,"MMIAPIPTSCR_ChangeRunMode: sd not exist!")
TRACE_MSG(MMIPTSCR_MAIN_403_112_2_18_2_47_34_9,"StartOrStopAutoRunTimer, MMK_CreateTimerCallback MMIPTSCR_AUTORUN_TIMEOUT = %d")
TRACE_MSG(MMIPTSCR_MAIN_465_112_2_18_2_47_34_10,"HandleAutoRunTimeTimerEvent -- :PTSCR_Print result is failed!")
TRACE_MSG(MMIPTSCR_SAVE_128_112_2_18_2_47_35_11,"MMIAPIPTSCR_WriteBMPFile MMIAPIFMM_CreateFile fail!")
TRACE_MSG(MMIPTSCR_SAVE_163_112_2_18_2_47_35_12,"MMIAPIPTSCR_WriteBMPFile WriteBMPFile bmp is to large! w = %d, nLineBytes = %d")
TRACE_MSG(MMIPTSCR_SAVE_171_112_2_18_2_47_35_13,"MMIAPIPTSCR_WriteBMPFile WriteBMPFile alloc fail!")
END_TRACE_MAP(MMI_APP_PRINTSCREEN_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PRINTSCREEN_TRC_H_

