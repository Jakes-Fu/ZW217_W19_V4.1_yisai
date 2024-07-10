/******************************************************************************
 ** File Name:      oscfg.c                                                   *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           11/08/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines most of system's static tasks.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/08/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "tasks_id.h"
#include "oscfg.h"
#include "os_kernel_cfg.c"
#include "os_base_cfg.c"
#include "os_system_cfg.c"
#include "os_app_cfg.c"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Task Definition                                   *
 **---------------------------------------------------------------------------*/
/* Normal mode task group. Used for normal power up.*/
STATIC_TASK_DEF_BEGIN(Normal)/*lint -esym(765, SCI_CreateTasksNormal*/
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)
//zhouqin modify

    DEF_TASK(   KPDSVR,         PRI_KPDSVR,         (1500),            40      )   //TASK_ID:2
    DEF_TASK(   EFS,            PRI_EFS,         (1024*3),            10      )   //TASK_ID:6
    DEF_TASK(   TIMESVR,        PRI_TIMESVR,         (1024 * 2),        10      )   //TASK_ID:9
#ifndef TOUCHPANEL_TYPE_NONE
    DEF_TASK(   P_TP,           PRI_TP,         (1024*2),            100      )   //TASK_ID:11
#endif
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT //jerryscript need 65KB+ RAM
    DEF_TASK(	P_APP,			 PRI_APP,		 (1024 * 95),			 500	 )	 //TASK_ID:21
#else
    DEF_TASK(   P_APP,          PRI_APP,         (1024 * 30),           500     )   //TASK_ID:21
#endif    
#ifndef WIN32
    DEF_TASK(   FFS,            PRI_FFS,         (1024 * 6),       10      )   //TASK_ID:25
    DEF_TASK(   AUDIO,          PRI_AUDIO,         (1024 * 4),        100     )   //TASK_ID:4
#endif

#if 1
#ifndef WIN32
    DEF_TASK(   CM,             PRI_CM,         (1024*2),            50      )   //TASK_ID:0
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    DEF_TASK(   DIAG,           PRI_DIAG,         (1024*6),            20      )   //TASK_ID:1
#else
    DEF_TASK(   DIAG,           PRI_DIAG,         (1024*3),            20      )   //TASK_ID:1
#endif
#endif
#endif
#if 0
    DEF_TASK(   KPDSVR,         PRI_KPDSVR,         (1500),            40      )   //TASK_ID:2
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:3 Reserved for LCD SERVER
#ifndef WIN32
    DEF_TASK(   AUDIO,          PRI_AUDIO,         (1024 * 4),        100     )   //TASK_ID:4
#endif
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:5 Reserved for VIDEO
#ifndef WIN32
    DEF_TASK(   EFS,            PRI_EFS,         (1024*3),            10      )   //TASK_ID:6
#else
    DEF_TASK(   EFS,            PRI_EFS,         (1024*3),            10      )   //TASK_ID:6
#endif
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:7 Reserved for CHR_SVR
#ifndef WIN32
    //DEF_TASK(   UPM,            30,         (1024 * 3),        40      )   //TASK_ID:8          
#endif
    DEF_TASK(   TIMESVR,        PRI_TIMESVR,         (1024 * 2),        10      )   //TASK_ID:9
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:10 Reserved for MP3
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:11 Reserved
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:12~18 Reserved for PS
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:19 Reserved for P_MN
#ifndef WIN32
//    DEF_TASK(   RECORD,         PRI_RECORD,         (0x800),           10      )   //TASK_ID:20
#endif
    DEF_TASK(   P_APP,          PRI_APP,         (1024 * 12),           500     )   //TASK_ID:21
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:22 Reserved for P_SIM
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:23 Reserved for P_SIM_WIN_DUMMY
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:24 Reserved for P_ATC
#ifndef WIN32
    DEF_TASK(   FFS,            PRI_FFS,         (1024 * 6),       10      )   //TASK_ID:25
#endif
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:26 Reserved for MPEG
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:27~38 Reserved for PS
//  DEF_TASK(STL_COMPACTION,    31,         1536,           1       )   //TASK_ID:39
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:40 Reserved 
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:41 Reserved
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:42~49 Reserved for CUSTOM
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:50~65 Reserved for PS
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:66~143 Reserved
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:144(0x90) Reserved for GPRS_LOG_MSG_ID
#endif
STATIC_TASK_DEF_END

/* Calibration mode task group. Used for CALIBRATION_MODE, LAYER1_TEST_MODE 
 * and DSP_CODE_DOWNLOAD_BACK_CALIBRATION*/
STATIC_TASK_DEF_BEGIN(Cal)/*lint -esym(765, SCI_CreateTasksCal*/
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)
#ifndef WIN32
    DEF_TASK(   CM,             PRI_CM,         (4096),            50      )   //TASK_ID:0
    DEF_TASK(   DIAG,           PRI_DIAG,         (5120)/*4096)*/,            10      )   //TASK_ID:1
    DEF_TASK(   AUDIO,          PRI_AUDIO,         (1024 * 8),        100     )   //TASK_ID:4
#endif
#ifndef WIN32
    DEF_TASK(   EFS,            PRI_EFS,         (8192),            10      )   //TASK_ID:6
#else
    DEF_TASK(   EFS,            PRI_EFS,         (8192),            10      )   //TASK_ID:6
#endif
#ifndef WIN32
   // DEF_TASK(   UPM,            30,         (1024 * 3),        40      )   //TASK_ID:8  
#endif
    DEF_TASK(   TIMESVR,        PRI_TIMESVR,         (1024 * 4),        10      )   //TASK_ID:9
#ifndef WIN32
//    DEF_TASK(   RECORD,         PRI_RECORD,         (0x800),           10      )   //TASK_ID:20
    DEF_TASK(   FFS,            PRI_FFS,         (1024 * 12),       10      )   //TASK_ID:25 
#endif
STATIC_TASK_DEF_END

STATIC_TASK_DEF_BEGIN(CalNoApp)/*lint -esym(765, SCI_CreateTasksCalNoApp*/
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)
#ifndef WIN32
    DEF_TASK(   CM,             PRI_CM,         (4096),            50      )   //TASK_ID:0
    DEF_TASK(   DIAG,           PRI_DIAG,         (5120)/*4096)*/,            10      )   //TASK_ID:1
#endif
    DEF_TASK(   KPDSVR,         PRI_KPDSVR,         (1500),            40      )   //TASK_ID:2
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:3 Reserved for LCD SERVER
    DEF_TASK(   AUDIO,          PRI_AUDIO,         (1024 * 8),        100     )   //TASK_ID:4
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:5 Reserved for VIDEO

    DEF_TASK(   EFS,            PRI_EFS,         (8192),            10      )   //TASK_ID:6

//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:7 Reserved for CHR_SVR
   // DEF_TASK(   UPM,            30,         (1024 * 3),        40      )   //TASK_ID:8          
    DEF_TASK(   TIMESVR,        PRI_TIMESVR,         (1024 * 4),        10      )   //TASK_ID:9
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:10 Reserved for MP3
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:11 Reserved
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:12~18 Reserved for PS
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:19 Reserved for P_MN
//    DEF_TASK(   RECORD,         PRI_RECORD,         (0x800),           10      )   //TASK_ID:20
//  DEF_TASK(   P_APP,          28,         8000,           500     )   //TASK_ID:21
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:22 Reserved for P_SIM
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:23 Reserved for P_SIM_WIN_DUMMY
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:24 Reserved for P_ATC
    DEF_TASK(   FFS,            PRI_FFS,         (1024 * 8),        10      )   //TASK_ID:25
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:26 Reserved for MPEG
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:27~38 Reserved for PS
//  DEF_TASK(STL_COMPACTION,    31,         1536,           1       )   //TASK_ID:39
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:40 Reserved 
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:41 Reserved
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:42~49 Reserved for CUSTOM
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:50~65 Reserved for PS
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:66~143 Reserved
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:144(0x90) Reserved for GPRS_LOG_MSG_ID
STATIC_TASK_DEF_END



/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/* ThreadEntry Example:(See also: oscfg.h)
 * THREAD_ENTRY(CM)
 * {
 *     while(1);
 * }
 */
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

//End of oscfg.c
