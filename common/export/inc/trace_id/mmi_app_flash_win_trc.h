/******************************************************************************
 ** File Name:      mmi_app_flash_win_trc.h                                         *
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
//trace_id:177
#ifndef _MMI_APP_FLASH_WIN_TRC_H_
#define _MMI_APP_FLASH_WIN_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIFLASH_WINTAB_343_112_2_18_2_19_17_0 "MMIFlash_IdleGetMp3Str"
#define MMIFLASH_WINTAB_457_112_2_18_2_19_18_1 "MMIAPIFlash_UpdateIdleItem"
#define MMIFLASH_WINTAB_505_112_2_18_2_19_18_2 "[idle] MMIAPIFlashWin_HandleMsg msg_id 0x%x"
#define MMIFLASH_WINTAB_679_112_2_18_2_19_18_3 "flash_printf:@@@@@Screen update"
#define MMIFLASH_WINTAB_734_112_2_18_2_19_18_4 "[idle] MMIAPIFlashWin_HandleMsg MSG_IDLE_UPDATE_MP3"
#define MMIFLASH_WINTAB_893_112_2_18_2_19_18_5 "IDLEWIN_HandleWinMsg: handle headset MSG!"
#define MMIFLASH_WINTAB_904_112_2_18_2_19_18_6 "IdleWin_HandleMsg:received timer msg!"
#define MMIFLASH_WINTAB_1013_112_2_18_2_19_19_7 "MMIAPIFlashWin_HandleMsg MSG_KEYLONG_HASH!"
#define MMIFLASH_WINTAB_1121_112_2_18_2_19_19_8 "MMIAPIFlashWin_HandleMsg id error!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_FLASH_WIN_TRC)
TRACE_MSG(MMIFLASH_WINTAB_343_112_2_18_2_19_17_0,"MMIFlash_IdleGetMp3Str")
TRACE_MSG(MMIFLASH_WINTAB_457_112_2_18_2_19_18_1,"MMIAPIFlash_UpdateIdleItem")
TRACE_MSG(MMIFLASH_WINTAB_505_112_2_18_2_19_18_2,"[idle] MMIAPIFlashWin_HandleMsg msg_id 0x%x")
TRACE_MSG(MMIFLASH_WINTAB_679_112_2_18_2_19_18_3,"flash_printf:@@@@@Screen update")
TRACE_MSG(MMIFLASH_WINTAB_734_112_2_18_2_19_18_4,"[idle] MMIAPIFlashWin_HandleMsg MSG_IDLE_UPDATE_MP3")
TRACE_MSG(MMIFLASH_WINTAB_893_112_2_18_2_19_18_5,"IDLEWIN_HandleWinMsg: handle headset MSG!")
TRACE_MSG(MMIFLASH_WINTAB_904_112_2_18_2_19_18_6,"IdleWin_HandleMsg:received timer msg!")
TRACE_MSG(MMIFLASH_WINTAB_1013_112_2_18_2_19_19_7,"MMIAPIFlashWin_HandleMsg MSG_KEYLONG_HASH!")
TRACE_MSG(MMIFLASH_WINTAB_1121_112_2_18_2_19_19_8,"MMIAPIFlashWin_HandleMsg id error!")
END_TRACE_MAP(MMI_APP_FLASH_WIN_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_FLASH_WIN_TRC_H_

