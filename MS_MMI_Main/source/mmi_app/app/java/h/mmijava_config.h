/*************************************************************************
 ** File Name:      mmijava_config.h                                     *
 ** Author:         jian.ma                                              *
 ** Date:           2009/12/14                                           *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about dm config info *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/12/14    jian.ma      Create.                                   *
*************************************************************************/
#ifndef _MMIJAVA_CONFIG_H
#define _MMIJAVA_CONFIG_H




/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef SC6600L_ARM7
#define MMIJAVA_MAX_SIZE        (1280*1024)
//#define MMIJAVA_MAX_SIZE        (1152*1024)
#else
#define MMIJAVA_MAX_SIZE        2*(2560*1024)//(2816*1024)//(3072*1024)//(2560*1024)  
#endif
// ucs2 encoded of "JAVA"
#define MMIJAVA_JAVA_MAIN_DIR_UCS2           L"JAVA"
#define MMIJAVA_JAVA_MAIN_DIR_BYTE_LEN       (4)

#define SHUTDOWN_JAVA               0
#define LAUNCH_INPUT_METHOD         1
#define LAUNCH_FILE_BROWSER         2
#define LAUNCH_WAP_BROWSER          3
#define START_AMS                   4
#define CREATE_STATUS_BAR           5
#define DESTROY_STATUS_BAR          6
#define REFLUSH_JAVA_WIN            7
#define LAUNCH_MIDLET               8
#define LAUNCH_VM_EXIT_WIN    9
#define BACKGROUND_TIMER_EVENT    10
#define PDP_ACTIVE    11
#define PDP_DEACTIVE    12
#define JAVA_VIDEO_START    13
#define JAVA_VIDEO_CLOSE    14
#define JAVA_VIDEO_ACQUIRE_DEVICE    15
#define JAVA_VIDEO_STOP    16
#define JAVA_VIDEO_PAUSE    17
#define JAVA_VIDEO_RESUME    18
#define JAVA_VIDEO_SET_LOCATION    19
#define JAVA_VIDEO_SET_VISIBLE    20
#define JAVA_VIDEO_SET_FULLSCREEN    21
#define JAVA_VIDEO_RELEASE_DEVICE    22
#define JAVA_VIDEO_SEEK_FRAME    23
#define JAVA_VIDEO_SKIP_FRAME    24
#define JAVA_VIDEO_GET_TIME      25
#define JAVA_VIDEO_GET_DURATION  26
#define JAVA_VIDEO_GET_SIZE  27

typedef enum
{
    JAVA_PAUSE_NONE=0,   // none module paused java
    JAVA_PAUSE_BY_CC,    // cc mt processing
    JAVA_PAUSE_BY_ALARM, // alarm incoming
    JAVA_PAUSE_BY_CT,//countedtime arrive        
    JAVA_PAUSE_BY_UDISK, // udisk plus in
    JAVA_PAUSE_BY_SD,    //
    JAVA_PAUSE_MAX
}MMIJAVA_PAUSE_TYPE_E;
#endif