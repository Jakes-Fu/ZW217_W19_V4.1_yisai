/******************************************************************************
 ** File Name:      mmi_app_gps_trc.h                                         *
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
//trace_id:181
#ifndef _MMI_APP_GPS_TRC_H_
#define _MMI_APP_GPS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIGPS_269_112_2_18_2_21_51_0 "copy one file false"
#define MMIGPS_297_112_2_18_2_21_51_1 "open false :%s"
#define MMIGPS_307_112_2_18_2_21_51_2 "open false :%s"
#define MMIGPS_OSAPI_181_112_2_18_2_21_52_3 "\n open file %s,%d false\n"
#define MMIGPS_OSAPI_204_112_2_18_2_21_52_4 "GPS_FileSeek  fp is NULL"
#define MMIGPS_OSAPI_226_112_2_18_2_21_52_5 "GPS_FileRead  fp is NULL"
#define MMIGPS_OSAPI_230_112_2_18_2_21_52_6 "[GPS_FileRead]:read Len:%x"
#define MMIGPS_OSAPI_254_112_2_18_2_21_52_7 "GPS_FileWrite  fp is NULL"
#define MMIGPS_OSAPI_279_112_2_18_2_21_52_8 "GPS_FileWrite  fp is NULL"
#define MMIGPS_OSAPI_309_112_2_18_2_21_52_9 "GPS_FileStat  fp is NULL"
#define MMIGPS_OSAPI_334_112_2_18_2_21_52_10 "GPS_FindFile  filename is NULL"
#define MMIGPS_OSAPI_354_112_2_18_2_21_52_11 "GPS_FindNextFile  pFind is NULL"
#define MMIGPS_OSAPI_395_112_2_18_2_21_52_12 "OSCreateThread"
#define MMIGPS_OSAPI_409_112_2_18_2_21_53_13 "s_GpsThread%x=%x"
#define MMIGPS_OSAPI_481_112_2_18_2_21_53_14 "s_iCurrentFile=%d"
#define MMIGPS_OSAPI_494_112_2_18_2_21_53_15 "SCI_DeleteThread=%x"
#define MMIGPS_OSAPI_497_112_2_18_2_21_53_16 "SCI_DeleteThread OK"
#define MMIGPS_OSAPI_512_112_2_18_2_21_53_17 "OpenWrite"
#define MMIGPS_OSAPI_528_112_2_18_2_21_53_18 "CloseWrite"
#define MMIGPS_OSAPI_549_112_2_18_2_21_53_19 "GPSSoundCallback=%d,s_pSoundLen=%d"
#define MMIGPS_OSAPI_593_112_2_18_2_21_53_20 "ritusoundenqueued=%d,store=%d"
#define MMIGPS_OSAPI_614_112_2_18_2_21_53_21 "GPS_stopplay,s_audiohandle:%x,thread_id:%x"
#define MMIGPS_OSAPI_627_112_2_18_2_21_53_22 "ritusoundAUDIO_CloseHandle"
#define MMIGPS_OSAPI_643_112_2_18_2_21_53_23 "ritusoundSignalCode=%d"
#define MMIGPS_OSAPI_669_112_2_18_2_21_53_24 "MMIGPS_StopPlay,s_audiohandle:%x,thread_id:%x"
#define MMIGPS_OSAPI_706_112_2_18_2_21_53_25 "GPS_FindNextFile  pFind is NULL"
#define MMIGPS_OSAPI_738_112_2_18_2_21_53_26 "s_audiohandle=%d,thread:%x"
#define MMIGPS_OSAPI_752_112_2_18_2_21_53_27 "s_pSoundLen=%d"
#define MMIGPS_OSAPI_813_112_2_18_2_21_53_28 "GPS_PlayPCM_TTS,s_audiohandle=%d,thread:%x"
#define MMIGPS_OSAPI_882_112_2_18_2_21_53_29 "RituGps_OsMalloc ulSize = %d"
#define MMIGPS_OSAPI_901_112_2_18_2_21_53_30 "RituGps_OsFree s_buf_ptr =%x"
#define MMIGPS_OSAPI_937_112_2_18_2_21_54_31 "ritu:%s\n"
#define MMIGPS_OSAPI_1055_112_2_18_2_21_54_32 "s_gps_cipher_result [0]:%x,"
#define MMIGPS_OSAPI_1056_112_2_18_2_21_54_33 "s_gps_cipher_result [1]:%x,"
#define MMIGPS_OSAPI_1057_112_2_18_2_21_54_34 "s_gps_cipher_result [2]:%x,"
#define MMIGPS_OSAPI_1058_112_2_18_2_21_54_35 "s_gps_cipher_result [3]:%x,"
#define MMIGPS_OSAPI_1093_112_2_18_2_21_54_36 "GPS_IsFolderExist  full_path_ptr is NULL"
#define MMIGPS_OSAPI_1099_112_2_18_2_21_54_37 ": s_find first sfs_handle = %d"
#define MMIGPS_WINTAB_103_112_2_18_2_21_54_38 "HandleGpsWinMsg,%x,gpsTask:%x"
#define MMIGPS_WINTAB_154_112_2_18_2_21_55_39 "MSG_KEYDOWN_OK"
#define MMIGPS_WINTAB_170_112_2_18_2_21_55_40 "MSG_KEYDOWN_LEFT"
#define MMIGPS_WINTAB_180_112_2_18_2_21_55_41 "MSG_KEYDOWN_RIGHT"
#define MMIGPS_WINTAB_188_112_2_18_2_21_55_42 "MSG_KEYDOWN_UP"
#define MMIGPS_WINTAB_197_112_2_18_2_21_55_43 "MSG_KEYDOWN_DOWN"
#define MMIGPS_WINTAB_214_112_2_18_2_21_55_44 "MSG_TP_PRESS_DOWN,point x=%d,y=%d"
#define MMIGPS_WINTAB_215_112_2_18_2_21_55_45 "ul=%d"
#define MMIGPS_WINTAB_227_112_2_18_2_21_55_46 "MSG_TP_PRESS_UP,point x=%d,y=%d"
#define MMIGPS_WINTAB_228_112_2_18_2_21_55_47 "ul=%d"
#define MMIGPS_WINTAB_241_112_2_18_2_21_55_48 "MSG_TP_PRESS_MOVE"
#define MMIGPS_WINTAB_277_112_2_18_2_21_55_49 "close gps win over"
#define MMIGPS_WINTAB_297_112_2_18_2_21_55_50 "GPS_CopyResource  dest_path_ptr OR src_path_ptr is NULL"
#define MMIGPS_WINTAB_300_112_2_18_2_21_55_51 "GPS_CopyResource begin"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_GPS_TRC)
TRACE_MSG(MMIGPS_269_112_2_18_2_21_51_0,"copy one file false")
TRACE_MSG(MMIGPS_297_112_2_18_2_21_51_1,"open false :%s")
TRACE_MSG(MMIGPS_307_112_2_18_2_21_51_2,"open false :%s")
TRACE_MSG(MMIGPS_OSAPI_181_112_2_18_2_21_52_3,"\n open file %s,%d false\n")
TRACE_MSG(MMIGPS_OSAPI_204_112_2_18_2_21_52_4,"GPS_FileSeek  fp is NULL")
TRACE_MSG(MMIGPS_OSAPI_226_112_2_18_2_21_52_5,"GPS_FileRead  fp is NULL")
TRACE_MSG(MMIGPS_OSAPI_230_112_2_18_2_21_52_6,"[GPS_FileRead]:read Len:%x")
TRACE_MSG(MMIGPS_OSAPI_254_112_2_18_2_21_52_7,"GPS_FileWrite  fp is NULL")
TRACE_MSG(MMIGPS_OSAPI_279_112_2_18_2_21_52_8,"GPS_FileWrite  fp is NULL")
TRACE_MSG(MMIGPS_OSAPI_309_112_2_18_2_21_52_9,"GPS_FileStat  fp is NULL")
TRACE_MSG(MMIGPS_OSAPI_334_112_2_18_2_21_52_10,"GPS_FindFile  filename is NULL")
TRACE_MSG(MMIGPS_OSAPI_354_112_2_18_2_21_52_11,"GPS_FindNextFile  pFind is NULL")
TRACE_MSG(MMIGPS_OSAPI_395_112_2_18_2_21_52_12,"OSCreateThread")
TRACE_MSG(MMIGPS_OSAPI_409_112_2_18_2_21_53_13,"s_GpsThread%x=%x")
TRACE_MSG(MMIGPS_OSAPI_481_112_2_18_2_21_53_14,"s_iCurrentFile=%d")
TRACE_MSG(MMIGPS_OSAPI_494_112_2_18_2_21_53_15,"SCI_DeleteThread=%x")
TRACE_MSG(MMIGPS_OSAPI_497_112_2_18_2_21_53_16,"SCI_DeleteThread OK")
TRACE_MSG(MMIGPS_OSAPI_512_112_2_18_2_21_53_17,"OpenWrite")
TRACE_MSG(MMIGPS_OSAPI_528_112_2_18_2_21_53_18,"CloseWrite")
TRACE_MSG(MMIGPS_OSAPI_549_112_2_18_2_21_53_19,"GPSSoundCallback=%d,s_pSoundLen=%d")
TRACE_MSG(MMIGPS_OSAPI_593_112_2_18_2_21_53_20,"ritusoundenqueued=%d,store=%d")
TRACE_MSG(MMIGPS_OSAPI_614_112_2_18_2_21_53_21,"GPS_stopplay,s_audiohandle:%x,thread_id:%x")
TRACE_MSG(MMIGPS_OSAPI_627_112_2_18_2_21_53_22,"ritusoundAUDIO_CloseHandle")
TRACE_MSG(MMIGPS_OSAPI_643_112_2_18_2_21_53_23,"ritusoundSignalCode=%d")
TRACE_MSG(MMIGPS_OSAPI_669_112_2_18_2_21_53_24,"MMIGPS_StopPlay,s_audiohandle:%x,thread_id:%x")
TRACE_MSG(MMIGPS_OSAPI_706_112_2_18_2_21_53_25,"GPS_FindNextFile  pFind is NULL")
TRACE_MSG(MMIGPS_OSAPI_738_112_2_18_2_21_53_26,"s_audiohandle=%d,thread:%x")
TRACE_MSG(MMIGPS_OSAPI_752_112_2_18_2_21_53_27,"s_pSoundLen=%d")
TRACE_MSG(MMIGPS_OSAPI_813_112_2_18_2_21_53_28,"GPS_PlayPCM_TTS,s_audiohandle=%d,thread:%x")
TRACE_MSG(MMIGPS_OSAPI_882_112_2_18_2_21_53_29,"RituGps_OsMalloc ulSize = %d")
TRACE_MSG(MMIGPS_OSAPI_901_112_2_18_2_21_53_30,"RituGps_OsFree s_buf_ptr =%x")
TRACE_MSG(MMIGPS_OSAPI_937_112_2_18_2_21_54_31,"ritu:%s\n")
TRACE_MSG(MMIGPS_OSAPI_1055_112_2_18_2_21_54_32,"s_gps_cipher_result [0]:%x,")
TRACE_MSG(MMIGPS_OSAPI_1056_112_2_18_2_21_54_33,"s_gps_cipher_result [1]:%x,")
TRACE_MSG(MMIGPS_OSAPI_1057_112_2_18_2_21_54_34,"s_gps_cipher_result [2]:%x,")
TRACE_MSG(MMIGPS_OSAPI_1058_112_2_18_2_21_54_35,"s_gps_cipher_result [3]:%x,")
TRACE_MSG(MMIGPS_OSAPI_1093_112_2_18_2_21_54_36,"GPS_IsFolderExist  full_path_ptr is NULL")
TRACE_MSG(MMIGPS_OSAPI_1099_112_2_18_2_21_54_37,": s_find first sfs_handle = %d")
TRACE_MSG(MMIGPS_WINTAB_103_112_2_18_2_21_54_38,"HandleGpsWinMsg,%x,gpsTask:%x")
TRACE_MSG(MMIGPS_WINTAB_154_112_2_18_2_21_55_39,"MSG_KEYDOWN_OK")
TRACE_MSG(MMIGPS_WINTAB_170_112_2_18_2_21_55_40,"MSG_KEYDOWN_LEFT")
TRACE_MSG(MMIGPS_WINTAB_180_112_2_18_2_21_55_41,"MSG_KEYDOWN_RIGHT")
TRACE_MSG(MMIGPS_WINTAB_188_112_2_18_2_21_55_42,"MSG_KEYDOWN_UP")
TRACE_MSG(MMIGPS_WINTAB_197_112_2_18_2_21_55_43,"MSG_KEYDOWN_DOWN")
TRACE_MSG(MMIGPS_WINTAB_214_112_2_18_2_21_55_44,"MSG_TP_PRESS_DOWN,point x=%d,y=%d")
TRACE_MSG(MMIGPS_WINTAB_215_112_2_18_2_21_55_45,"ul=%d")
TRACE_MSG(MMIGPS_WINTAB_227_112_2_18_2_21_55_46,"MSG_TP_PRESS_UP,point x=%d,y=%d")
TRACE_MSG(MMIGPS_WINTAB_228_112_2_18_2_21_55_47,"ul=%d")
TRACE_MSG(MMIGPS_WINTAB_241_112_2_18_2_21_55_48,"MSG_TP_PRESS_MOVE")
TRACE_MSG(MMIGPS_WINTAB_277_112_2_18_2_21_55_49,"close gps win over")
TRACE_MSG(MMIGPS_WINTAB_297_112_2_18_2_21_55_50,"GPS_CopyResource  dest_path_ptr OR src_path_ptr is NULL")
TRACE_MSG(MMIGPS_WINTAB_300_112_2_18_2_21_55_51,"GPS_CopyResource begin")
END_TRACE_MAP(MMI_APP_GPS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_GPS_TRC_H_

