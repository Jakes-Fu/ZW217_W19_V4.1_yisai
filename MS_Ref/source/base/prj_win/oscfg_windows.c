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
STATIC_TASK_DEF_BEGIN(Normal)
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)
#ifndef PRJ_TDPS_UEIT_MMI
#ifdef WIN32
    DEF_TASK(   P_LAYER1,            PRI_BASE_LAYER1,         1024 * 2,       50     )   //TASK_ID:25
#endif
#endif
// @hongliang.xin 2010-8-16 Win32 for simulator with monkey test
//#ifndef WIN32
#ifndef PRJ_TDPS_UEIT_MMI
    DEF_TASK(   CM,             PRI_CM,         4096,           50      )   //TASK_ID:0
    DEF_TASK(   DIAG,           PRI_DIAG,         4096,           10      )   //TASK_ID:1
#endif
//#endif

    DEF_TASK(   KPDSVR,         PRI_KPDSVR,         800,            40      )   //TASK_ID:2
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:3 Reserved for LCD SERVER

    DEF_TASK(   AUDIO,          PRI_AUDIO,         1024 * 8,       100     )   //TASK_ID:4

//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:5 Reserved for VIDEO
#ifdef _NORMCP_DEF_
    DEF_TASK(   EFS,            PRI_EFS,         8192,           10      )   //TASK_ID:6
#elif defined WIN32
    DEF_TASK(   EFS,            PRI_EFS,         8192,           10      )   //TASK_ID:6
#endif
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:7 Reserved for CHR_SVR
#ifndef WIN32
    DEF_TASK(   UPM,            PRI_USB_SERVICE,         1024 * 20,      40      )   //TASK_ID:8          
#endif
    DEF_TASK(   TIMESVR,        PRI_TIMESVR,         1024 * 4,       10      )   //TASK_ID:9
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:10 Reserved for MP3
#ifdef _TP_ENABLE_
  DEF_TASK(   P_TPP,          PRI_TP,         800,            20      )   //TASK_ID:11  
#endif
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:12~18 Reserved for PS
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:19 Reserved for P_MN

    DEF_TASK(   RECORD,         PRI_RECORD,         0x800,          10      )   //TASK_ID:20

#ifndef WIN32
    DEF_TASK(   P_APP,          PRI_APP,         8000,           500     )   //TASK_ID:21
#endif
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:22 Reserved for P_SIM
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:23 Reserved for P_SIM_WIN_DUMMY
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)   //TASK_ID:24 Reserved for P_ATC
#ifndef WIN32
    DEF_TASK(   FFS,            PRI_FFS,         1024 * 8,       10      )   //TASK_ID:25
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
STATIC_TASK_DEF_END

/* Calibration mode task group. Used for CALIBRATION_MODE, LAYER1_TEST_MODE 
 * and DSP_CODE_DOWNLOAD_BACK_CALIBRATION*/
STATIC_TASK_DEF_BEGIN(Cal)
//  DEF_TASK(<THREAD_NAME>,  <PRIORITY>, <STACK_SIZE>,   <QUEUE_NUM>)
#ifndef WIN32
    DEF_TASK(   CM,             PRI_CM,         4096,           50      )   //TASK_ID:0
    DEF_TASK(   DIAG,           PRI_DIAG,         4096,           10      )   //TASK_ID:1
    DEF_TASK(   AUDIO,          PRI_AUDIO,         1024 * 8,       100     )   //TASK_ID:4
#endif
#if defined _NORMCP_DEF_  || defined WIN32
    DEF_TASK(   EFS,            PRI_EFS,         8192,           10      )   //TASK_ID:6
#endif
#ifndef WIN32
    DEF_TASK(   UPM,            PRI_USB_SERVICE,         1024 * 20,      40      )   //TASK_ID:8  
#endif
    DEF_TASK(   TIMESVR,        PRI_TIMESVR,         1024 * 4,       10      )   //TASK_ID:9
#ifndef WIN32
    DEF_TASK(   RECORD,         PRI_RECORD,         0x800,          10      )   //TASK_ID:20
    DEF_TASK(   FFS,            PRI_FFS,         1024 * 8,       10      )   //TASK_ID:25 
#endif
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
