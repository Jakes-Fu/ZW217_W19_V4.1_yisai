/******************************************************************************
 ** File Name:      mmi_app_memo_trc.h                                         *
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
//trace_id:191
#ifndef _MMI_APP_MEMO_TRC_H_
#define _MMI_APP_MEMO_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIMEMO_FUNC_216_112_2_18_2_32_41_0 "[MEMO]MMIMEMO_GetContent: buf_ptr ALLOC FAIL!"
#define MMIMEMO_FUNC_226_112_2_18_2_32_41_1 "[MEMO]MMIMEMO_GetContent: buf_ptr error!1"
#define MMIMEMO_FUNC_241_112_2_18_2_32_41_2 "[MEMO]MMIMEMO_GetContent: buf_ptr error!"
#define MMIMEMO_WINTABLE_836_112_2_18_2_32_44_3 "HandleMemoOpenMainWaitingWin, msg_id = 0x%x"
#define MMIMEMO_WINTABLE_890_112_2_18_2_32_44_4 "HandleMemoOpenMainWaitingWin, msg_id = 0x%x end"
#define MMIMEMO_WINTABLE_1439_112_2_18_2_32_45_5 "MemoDeleteMarkedItem error;item_index = %d"
#define MMIMEMO_WINTABLE_1577_112_2_18_2_32_46_6 "MMIAPIMEMO_Exit"
#define MMIMEMO_WINTABLE_1618_112_2_18_2_32_46_7 "MemoSetOptMenu get item error"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MEMO_TRC)
TRACE_MSG(MMIMEMO_FUNC_216_112_2_18_2_32_41_0,"[MEMO]MMIMEMO_GetContent: buf_ptr ALLOC FAIL!")
TRACE_MSG(MMIMEMO_FUNC_226_112_2_18_2_32_41_1,"[MEMO]MMIMEMO_GetContent: buf_ptr error!1")
TRACE_MSG(MMIMEMO_FUNC_241_112_2_18_2_32_41_2,"[MEMO]MMIMEMO_GetContent: buf_ptr error!")
TRACE_MSG(MMIMEMO_WINTABLE_836_112_2_18_2_32_44_3,"HandleMemoOpenMainWaitingWin, msg_id = 0x%x")
TRACE_MSG(MMIMEMO_WINTABLE_890_112_2_18_2_32_44_4,"HandleMemoOpenMainWaitingWin, msg_id = 0x%x end")
TRACE_MSG(MMIMEMO_WINTABLE_1439_112_2_18_2_32_45_5,"MemoDeleteMarkedItem error;item_index = %d")
TRACE_MSG(MMIMEMO_WINTABLE_1577_112_2_18_2_32_46_6,"MMIAPIMEMO_Exit")
TRACE_MSG(MMIMEMO_WINTABLE_1618_112_2_18_2_32_46_7,"MemoSetOptMenu get item error")
END_TRACE_MAP(MMI_APP_MEMO_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MEMO_TRC_H_

