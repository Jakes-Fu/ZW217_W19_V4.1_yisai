/******************************************************************************
 ** File Name:      mmi_app_java_trc.h                                         *
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
//trace_id:186
#ifndef _MMI_APP_JAVA_TRC_H_
#define _MMI_APP_JAVA_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIJAVA_EXPORT_366_112_2_18_2_30_32_0 "MMIAPIJAVA_Init for sun no support"
#define MMIJAVA_MAIN_284_112_2_18_2_30_33_1 "mmijava_main.c PenEventConvert platformPenEvent=%d"
#define MMIJAVA_MAIN_403_112_2_18_2_30_33_2 "mmijava_export.c JavaJadFile_otacalback"
#define MMIJAVA_MAIN_413_112_2_18_2_30_33_3 "mmijava_export.c JavaJadFile_otacalback MMIAPIVT_IsVtCalling"
#define MMIJAVA_MAIN_459_112_2_18_2_30_34_4 "mmijava_export.c JavaJarFile_otacallback"
#define MMIJAVA_MAIN_471_112_2_18_2_30_34_5 "mmijava_export.c JavaJarFile_otacallback MMIAPIVT_IsVtCalling"
#define MMIJAVA_MAIN_557_112_2_18_2_30_34_6 "mmijava_main.c RemoveTmpDownloadFileWhenInit url = %s"
#define MMIJAVA_MAIN_565_112_2_18_2_30_34_7 "mmijava_main.c RemoveTmpDownloadFileWhenInit user_data = %s"
#define MMIJAVA_MAIN_1504_112_2_18_2_30_36_8 "java status bar create enter"
#define MMIJAVA_MAIN_1517_112_2_18_2_30_36_9 "java status bar destroy enter"
#define MMIJAVA_MAIN_1534_112_2_18_2_30_36_10 "java BACKGROUND_TIMER_EVENT"
#define MMIJAVA_MAIN_1716_112_2_18_2_30_36_11 "MMIJAVA_REMOVE_MIDLET"
#define MMIJAVA_MAIN_1746_112_2_18_2_30_36_12 "HandleJavaTextEditWinMsg msg_id = %x"
#define MMIJAVA_MAIN_1907_112_2_18_2_30_37_13 "HandleVMExitSelectWindow item_num = %d"
#define MMIJAVA_MAIN_2123_112_2_18_2_30_38_14 "MMIJAVA_Shutdown enter vm_state is %d"
#define MMIJAVA_MAIN_2130_112_2_18_2_30_38_15 "MMIJAVA_Shutdown start shutdown_jvm"
#define MMIJAVA_MAIN_2138_112_2_18_2_30_38_16 "MMIJAVA_Shutdown leave"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_JAVA_TRC)
TRACE_MSG(MMIJAVA_EXPORT_366_112_2_18_2_30_32_0,"MMIAPIJAVA_Init for sun no support")
TRACE_MSG(MMIJAVA_MAIN_284_112_2_18_2_30_33_1,"mmijava_main.c PenEventConvert platformPenEvent=%d")
TRACE_MSG(MMIJAVA_MAIN_403_112_2_18_2_30_33_2,"mmijava_export.c JavaJadFile_otacalback")
TRACE_MSG(MMIJAVA_MAIN_413_112_2_18_2_30_33_3,"mmijava_export.c JavaJadFile_otacalback MMIAPIVT_IsVtCalling")
TRACE_MSG(MMIJAVA_MAIN_459_112_2_18_2_30_34_4,"mmijava_export.c JavaJarFile_otacallback")
TRACE_MSG(MMIJAVA_MAIN_471_112_2_18_2_30_34_5,"mmijava_export.c JavaJarFile_otacallback MMIAPIVT_IsVtCalling")
TRACE_MSG(MMIJAVA_MAIN_557_112_2_18_2_30_34_6,"mmijava_main.c RemoveTmpDownloadFileWhenInit url = %s")
TRACE_MSG(MMIJAVA_MAIN_565_112_2_18_2_30_34_7,"mmijava_main.c RemoveTmpDownloadFileWhenInit user_data = %s")
TRACE_MSG(MMIJAVA_MAIN_1504_112_2_18_2_30_36_8,"java status bar create enter")
TRACE_MSG(MMIJAVA_MAIN_1517_112_2_18_2_30_36_9,"java status bar destroy enter")
TRACE_MSG(MMIJAVA_MAIN_1534_112_2_18_2_30_36_10,"java BACKGROUND_TIMER_EVENT")
TRACE_MSG(MMIJAVA_MAIN_1716_112_2_18_2_30_36_11,"MMIJAVA_REMOVE_MIDLET")
TRACE_MSG(MMIJAVA_MAIN_1746_112_2_18_2_30_36_12,"HandleJavaTextEditWinMsg msg_id = %x")
TRACE_MSG(MMIJAVA_MAIN_1907_112_2_18_2_30_37_13,"HandleVMExitSelectWindow item_num = %d")
TRACE_MSG(MMIJAVA_MAIN_2123_112_2_18_2_30_38_14,"MMIJAVA_Shutdown enter vm_state is %d")
TRACE_MSG(MMIJAVA_MAIN_2130_112_2_18_2_30_38_15,"MMIJAVA_Shutdown start shutdown_jvm")
TRACE_MSG(MMIJAVA_MAIN_2138_112_2_18_2_30_38_16,"MMIJAVA_Shutdown leave")
END_TRACE_MAP(MMI_APP_JAVA_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_JAVA_TRC_H_

