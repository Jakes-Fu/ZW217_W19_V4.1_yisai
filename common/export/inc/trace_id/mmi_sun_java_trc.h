/******************************************************************************
 ** File Name:      mmi_sun_java_trc.h                                         *
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
//trace_id:265
#ifndef _MMI_SUN_JAVA_TRC_H_
#define _MMI_SUN_JAVA_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define JAVA_SIGDISPATCHTASK_61_112_2_18_3_26_33_0 "java_SigDispatchTask : task start!"
#define JAVA_SIGDISPATCHTASK_68_112_2_18_3_26_33_1 "java_SigDispatchTask, get signal code =0x%x"
#define JAVA_SIGDISPATCHTASK_85_112_2_18_3_26_33_2 "java_CreateSigDispatchTask has been created"
#define JAVA_SIGDISPATCHTASK_101_112_2_18_3_26_33_3 "java_CreateSigDispatchTask created, task id = %d"
#define JAVACALL_LOGGING_58_112_2_18_3_26_37_4 "[ JAVA ][%5d:%10d]: javacall_logging_file_start"
#define JAVACALL_LOGGING_63_112_2_18_3_26_37_5 "[ JAVA ] create log file handle=%d when start"
#define JAVACALL_LOGGING_69_112_2_18_3_26_37_6 "[ JAVA ][%5d:%10d]: javacall_logging_file_end"
#define JAVACALL_LOGGING_95_112_2_18_3_26_37_7 "[ JAVA ][%5d:%10d]:%s"
#define JAVACALL_LOGGING_97_112_2_18_3_26_37_8 "[ JAVA ]:%s"
#define JAVACALL_LOGGING_104_112_2_18_3_26_37_9 "[ JAVA ] create log file handle=%d when print"
#define JAVACALL_MEMORY_53_112_2_18_3_26_38_10 "javacall_memory_heap_allocate>>>>>>>>>>>"
#define JAVACALL_MEMORY_61_112_2_18_3_26_38_11 "javacall_memory_heap_allocate<<<<<<the size is %d"
#define JAVACALL_MEMORY_81_112_2_18_3_26_38_12 "javacall_memory_get_app_heap_size<<<<<<jvm&app heap size is %d"
#define APPSMS_4SPREAD_200_112_2_18_3_26_38_13 "SMS SEND SUCCESS"
#define APPSMS_4SPREAD_203_112_2_18_3_26_38_14 "SMS SEND FAIL"
#define JAVACALL_TIME_BASED_ON_PLATFORM_TIMER_86_112_2_18_3_27_12_15 "No free timer\n"
#define JAVACALL_TIME_BASED_ON_PLATFORM_TIMER_119_112_2_18_3_27_12_16 "Timer deletion returns %d\n"
#define MMIJAVA_DC_1255_112_2_18_3_27_17_17 "[MMIJAVA]: dv_record_end_callback record end! i_type = %d"
#define MMIJAVA_PIM_175_112_2_18_3_27_18_18 "[JAVA PIM] MMIJAVA_PIM_Modify_Item type %d uuid 0x%x"
#define MMIJAVA_PIM_238_112_2_18_3_27_19_19 "[JAVA PIM] MMIJAVA_PIM_Add_Item type %d "
#define MMIJAVA_PIM_293_112_2_18_3_27_19_20 "[JAVA PIM] MMIJAVA_PIM_Remove_Item type %d uuid 0x%x"
#define MMIJAVA_PIM_331_112_2_18_3_27_19_21 "[JAVA PIM] vcard_parseuuid parameter error !"
#define MMIJAVA_PIM_345_112_2_18_3_27_19_22 "[JAVA PIM] vcard_parseuuid gropu id 0x%x storage_id 0x%x uuid 0x%x"
#define MMIJAVA_PIM_359_112_2_18_3_27_19_23 "[JAVA PIM] vcard_createuuid gropu id 0x%x storage_id 0x%x uuid 0x%x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_SUN_JAVA_TRC)
TRACE_MSG(JAVA_SIGDISPATCHTASK_61_112_2_18_3_26_33_0,"java_SigDispatchTask : task start!")
TRACE_MSG(JAVA_SIGDISPATCHTASK_68_112_2_18_3_26_33_1,"java_SigDispatchTask, get signal code =0x%x")
TRACE_MSG(JAVA_SIGDISPATCHTASK_85_112_2_18_3_26_33_2,"java_CreateSigDispatchTask has been created")
TRACE_MSG(JAVA_SIGDISPATCHTASK_101_112_2_18_3_26_33_3,"java_CreateSigDispatchTask created, task id = %d")
TRACE_MSG(JAVACALL_LOGGING_58_112_2_18_3_26_37_4,"[ JAVA ][%5d:%10d]: javacall_logging_file_start")
TRACE_MSG(JAVACALL_LOGGING_63_112_2_18_3_26_37_5,"[ JAVA ] create log file handle=%d when start")
TRACE_MSG(JAVACALL_LOGGING_69_112_2_18_3_26_37_6,"[ JAVA ][%5d:%10d]: javacall_logging_file_end")
TRACE_MSG(JAVACALL_LOGGING_95_112_2_18_3_26_37_7,"[ JAVA ][%5d:%10d]:%s")
TRACE_MSG(JAVACALL_LOGGING_97_112_2_18_3_26_37_8,"[ JAVA ]:%s")
TRACE_MSG(JAVACALL_LOGGING_104_112_2_18_3_26_37_9,"[ JAVA ] create log file handle=%d when print")
TRACE_MSG(JAVACALL_MEMORY_53_112_2_18_3_26_38_10,"javacall_memory_heap_allocate>>>>>>>>>>>")
TRACE_MSG(JAVACALL_MEMORY_61_112_2_18_3_26_38_11,"javacall_memory_heap_allocate<<<<<<the size is %d")
TRACE_MSG(JAVACALL_MEMORY_81_112_2_18_3_26_38_12,"javacall_memory_get_app_heap_size<<<<<<jvm&app heap size is %d")
TRACE_MSG(APPSMS_4SPREAD_200_112_2_18_3_26_38_13,"SMS SEND SUCCESS")
TRACE_MSG(APPSMS_4SPREAD_203_112_2_18_3_26_38_14,"SMS SEND FAIL")
TRACE_MSG(JAVACALL_TIME_BASED_ON_PLATFORM_TIMER_86_112_2_18_3_27_12_15,"No free timer\n")
TRACE_MSG(JAVACALL_TIME_BASED_ON_PLATFORM_TIMER_119_112_2_18_3_27_12_16,"Timer deletion returns %d\n")
TRACE_MSG(MMIJAVA_DC_1255_112_2_18_3_27_17_17,"[MMIJAVA]: dv_record_end_callback record end! i_type = %d")
TRACE_MSG(MMIJAVA_PIM_175_112_2_18_3_27_18_18,"[JAVA PIM] MMIJAVA_PIM_Modify_Item type %d uuid 0x%x")
TRACE_MSG(MMIJAVA_PIM_238_112_2_18_3_27_19_19,"[JAVA PIM] MMIJAVA_PIM_Add_Item type %d ")
TRACE_MSG(MMIJAVA_PIM_293_112_2_18_3_27_19_20,"[JAVA PIM] MMIJAVA_PIM_Remove_Item type %d uuid 0x%x")
TRACE_MSG(MMIJAVA_PIM_331_112_2_18_3_27_19_21,"[JAVA PIM] vcard_parseuuid parameter error !")
TRACE_MSG(MMIJAVA_PIM_345_112_2_18_3_27_19_22,"[JAVA PIM] vcard_parseuuid gropu id 0x%x storage_id 0x%x uuid 0x%x")
TRACE_MSG(MMIJAVA_PIM_359_112_2_18_3_27_19_23,"[JAVA PIM] vcard_createuuid gropu id 0x%x storage_id 0x%x uuid 0x%x")
END_TRACE_MAP(MMI_SUN_JAVA_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_SUN_JAVA_TRC_H_

