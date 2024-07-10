/******************************************************************************
 ** File Name:      mmi_app_keylock_trc.h                                         *
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
//trace_id:187
#ifndef _MMI_APP_KEYLOCK_TRC_H_
#define _MMI_APP_KEYLOCK_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIKL_DRAGUNLOCK_1007_112_2_18_2_30_44_0 "HandleKLWinEffect6Msg: 0x%x"
#define MMIKL_DRAWGRAPHIC_2177_112_2_18_2_30_50_1 "HandleSetKLPasswardWindow: 0x%x"
#define MMIKL_EFFECTPSW_1224_112_2_18_2_30_56_2 "HandleKLWinPswMsg: 0x%x"
#define MMIKL_EFFECTPSW_1537_112_2_18_2_30_56_3 "HandleKLPswInputWinMsg: 0x%x"
#define MMIKL_ISTYLESLIDE_858_112_2_18_2_31_2_4 "HandleKLWinEffect4Msg: 0x%x"
#define MMIKL_ISTYLESLIDE_924_112_2_18_2_31_2_5 "MSG_KEY_DBLCLK, key = 0x%x"
#define MMIKL_KEYLOCK_557_112_2_18_2_31_4_6 "error: DrawAClock target_buf_ptr alloc failed,  return directly"
#define MMIKL_KEYLOCK_600_112_2_18_2_31_4_7 "error: foreground image rect is not in background image rect, return directly"
#define MMIKL_KEYLOCK_1070_112_2_18_2_31_5_8 "HandleKLDispWinMsg: 0x%x"
#define MMIKL_KEYLOCK_1356_112_2_18_2_31_6_9 "not in any range"
#define MMIKL_SLIDEDOWN_487_112_2_18_2_31_8_10 "HandleKLWinEffect1Msg: 0x%x"
#define MMIKL_SLIDESCROLLKEY_373_112_2_18_2_31_9_11 "MMIKL_HandleSlideScrollKeyWinMsg: 0x%x"
#define MMIKL_SLIDESCROLLKEY_437_112_2_18_2_31_9_12 "IdleWin_HandleMsg MSG_KEYLONG_HASH!"
#define MMIKL_SMARTSLIDE_1019_112_2_18_2_31_12_13 "HandleKLWinEffect3Msg: 0x%x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_KEYLOCK_TRC)
TRACE_MSG(MMIKL_DRAGUNLOCK_1007_112_2_18_2_30_44_0,"HandleKLWinEffect6Msg: 0x%x")
TRACE_MSG(MMIKL_DRAWGRAPHIC_2177_112_2_18_2_30_50_1,"HandleSetKLPasswardWindow: 0x%x")
TRACE_MSG(MMIKL_EFFECTPSW_1224_112_2_18_2_30_56_2,"HandleKLWinPswMsg: 0x%x")
TRACE_MSG(MMIKL_EFFECTPSW_1537_112_2_18_2_30_56_3,"HandleKLPswInputWinMsg: 0x%x")
TRACE_MSG(MMIKL_ISTYLESLIDE_858_112_2_18_2_31_2_4,"HandleKLWinEffect4Msg: 0x%x")
TRACE_MSG(MMIKL_ISTYLESLIDE_924_112_2_18_2_31_2_5,"MSG_KEY_DBLCLK, key = 0x%x")
TRACE_MSG(MMIKL_KEYLOCK_557_112_2_18_2_31_4_6,"error: DrawAClock target_buf_ptr alloc failed,  return directly")
TRACE_MSG(MMIKL_KEYLOCK_600_112_2_18_2_31_4_7,"error: foreground image rect is not in background image rect, return directly")
TRACE_MSG(MMIKL_KEYLOCK_1070_112_2_18_2_31_5_8,"HandleKLDispWinMsg: 0x%x")
TRACE_MSG(MMIKL_KEYLOCK_1356_112_2_18_2_31_6_9,"not in any range")
TRACE_MSG(MMIKL_SLIDEDOWN_487_112_2_18_2_31_8_10,"HandleKLWinEffect1Msg: 0x%x")
TRACE_MSG(MMIKL_SLIDESCROLLKEY_373_112_2_18_2_31_9_11,"MMIKL_HandleSlideScrollKeyWinMsg: 0x%x")
TRACE_MSG(MMIKL_SLIDESCROLLKEY_437_112_2_18_2_31_9_12,"IdleWin_HandleMsg MSG_KEYLONG_HASH!")
TRACE_MSG(MMIKL_SMARTSLIDE_1019_112_2_18_2_31_12_13,"HandleKLWinEffect3Msg: 0x%x")
END_TRACE_MAP(MMI_APP_KEYLOCK_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_KEYLOCK_TRC_H_

