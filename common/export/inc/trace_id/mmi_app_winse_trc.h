/******************************************************************************
 ** File Name:      mmi_app_winse_trc.h                                         *
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
//trace_id:240
#ifndef _MMI_APP_WINSE_TRC_H_
#define _MMI_APP_WINSE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_WINSE_170_112_2_18_3_9_25_0 "WINSESEInitialise param check failed:0x%x,0x%x,0x%x!"
#define MMI_WINSE_181_112_2_18_3_9_25_1 "WINSESEInitialise :dst buffer error! "
#define MMI_WINSE_289_112_2_18_3_9_25_2 "WINSEDelay error delay time:%d"
#define MMI_WINSE_588_112_2_18_3_9_26_3 "PlayWinSe cost Time:%d ms"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_WINSE_TRC)
TRACE_MSG(MMI_WINSE_170_112_2_18_3_9_25_0,"WINSESEInitialise param check failed:0x%x,0x%x,0x%x!")
TRACE_MSG(MMI_WINSE_181_112_2_18_3_9_25_1,"WINSESEInitialise :dst buffer error! ")
TRACE_MSG(MMI_WINSE_289_112_2_18_3_9_25_2,"WINSEDelay error delay time:%d")
TRACE_MSG(MMI_WINSE_588_112_2_18_3_9_26_3,"PlayWinSe cost Time:%d ms")
END_TRACE_MAP(MMI_APP_WINSE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_WINSE_TRC_H_

