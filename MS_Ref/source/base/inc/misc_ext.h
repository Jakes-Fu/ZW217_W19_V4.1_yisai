/******************************************************************************
 ** File Name:      misc_ext.h                                                *
 ** Author:         Eric.zhou                                                 *
 ** DATE:           03/06/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/06/2003     Eric.zhou        Create.                                   *
 ** 07/08/2003     Xueliang.Wang    Modify                                    *
 ******************************************************************************/
#ifndef _MISC_EXT_H   
#define _MISC_EXT_H

#include "os_api.h"
//#include "misc_task.h"
#include "tb_hal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Main thread of diagnostic task. It manages the command 
//                  messages from client application tools and send response
//                  back to channel manager. 
//	Global resource dependence: 
//  Author:         Eric.zhou
//	Note:			It recever messages from MISC.
/*****************************************************************************/
PUBLIC void thread_misc_task_entry(uint32 argc, void *argv);    

/*****************************************************************************/
//  Description:    This function is used to register to TBIOS. It will be
//                  called to handle the message from lower layer.
//  Author:         Eric.zhou
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_RtcCallback(
    TB_MSG * msg_ptr    // Message to be handled.
    );

/*****************************************************************************/
// 	Description :   Adjust simulate minute interrupt timer.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
#ifdef SIMULATE_MIN_INT
PUBLIC void SCI_AdjustMiniuteIntTimer(
    uint32 sec_num
    );
#endif
/*****************************************************************************/
// 	Description :   Update RTC calibration result.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
PUBLIC void SCI_UpdateRtcCalibration(BOOLEAN is_clear);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  // _MISC_EXT_H
