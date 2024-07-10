/******************************************************************************
 ** File Name:      mmi_app_screensaver_trc.h                                         *
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
//trace_id:219
#ifndef _MMI_APP_SCREENSAVER_TRC_H_
#define _MMI_APP_SCREENSAVER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMISS_SCREENCLOSE_SPECIAL_363_112_2_18_2_49_45_0 "CRT Render:The %d  time's CRT full Render start time is: %d; end time is: %d;loop time is %d;"
#define MMISS_SCREENCLOSE_SPECIAL_371_112_2_18_2_49_45_1 "CRT Render:The %d  time's CRT full Render start time is: %d; end time is: %d;loop time is %d;"
#define MMISS_SCREENSAVER_200_112_2_18_2_49_45_2 "MMIAPIIDLESS_OpenScreenSaver,open saver win fail! "
#define MMISS_SCREENSAVER_205_112_2_18_2_49_45_3 "MMIAPIIDLESS_OpenScreenSaver: backligt is close"
#define MMISS_SCREENSAVER_261_112_2_18_2_49_45_4 "[MMIMPEG4]: MMIAPIIDLESS_SetDefaultScreenSaver, ENTERS!"
#define MMISS_SCREENSAVER_284_112_2_18_2_49_45_5 "[VP] HandleVideoScreenSaverMsg msg_ptr->msg_id = %d"
#define MMISS_SPECIAL_123_112_2_18_2_49_49_6 "MMIIDLE:MMIIDLE_OpenSpeScrSaverPreviewWin PNULL == scr_win_ptr"
#define MMISS_SPECIAL_143_112_2_18_2_49_49_7 "MMIIDLE:MMIAPIIDLESS_OpenSpecialScreenSaver PNULL == scr_win_ptr"
#define MMISS_SPECIAL_172_112_2_18_2_49_49_8 "HandleSpecialScreenSaverWinMsg ERROR! PNULL == scr_win_ptr"
#define MMISS_SPECIAL_234_112_2_18_2_49_50_9 "HandleSpecialScreenSaverWinMsg ERROR! MSG_APP_LEFT sel_item_index =%d"
#define MMISS_SPECIAL_275_112_2_18_2_49_50_10 "HandleSpecialScreenSaverWinMsg ERROR! MSG_APP_RIGHT sel_item_index =%d"
#define MMISS_SPRITE_507_112_2_18_2_49_51_11 "MMIIDLE_PressMoveSprite ERROR! MMIIDLE_SPRITE_NONE == s_pressed_sprite"
#define MMISS_SPRITE_513_112_2_18_2_49_51_12 "MMIIDLE_PressMoveSprite ERROR! s_pressed_sprite =%d"
#define MMISS_SPRITE_618_112_2_18_2_49_51_13 "GetSpriteRect ERROR! PNULL == rect_ptr"
#define MMISS_SPRITE_639_112_2_18_2_49_51_14 "GetSpriteOldRect ERROR! PNULL == rect_ptr"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_SCREENSAVER_TRC)
TRACE_MSG(MMISS_SCREENCLOSE_SPECIAL_363_112_2_18_2_49_45_0,"CRT Render:The %d  time's CRT full Render start time is: %d; end time is: %d;loop time is %d;")
TRACE_MSG(MMISS_SCREENCLOSE_SPECIAL_371_112_2_18_2_49_45_1,"CRT Render:The %d  time's CRT full Render start time is: %d; end time is: %d;loop time is %d;")
TRACE_MSG(MMISS_SCREENSAVER_200_112_2_18_2_49_45_2,"MMIAPIIDLESS_OpenScreenSaver,open saver win fail! ")
TRACE_MSG(MMISS_SCREENSAVER_205_112_2_18_2_49_45_3,"MMIAPIIDLESS_OpenScreenSaver: backligt is close")
TRACE_MSG(MMISS_SCREENSAVER_261_112_2_18_2_49_45_4,"[MMIMPEG4]: MMIAPIIDLESS_SetDefaultScreenSaver, ENTERS!")
TRACE_MSG(MMISS_SCREENSAVER_284_112_2_18_2_49_45_5,"[VP] HandleVideoScreenSaverMsg msg_ptr->msg_id = %d")
TRACE_MSG(MMISS_SPECIAL_123_112_2_18_2_49_49_6,"MMIIDLE:MMIIDLE_OpenSpeScrSaverPreviewWin PNULL == scr_win_ptr")
TRACE_MSG(MMISS_SPECIAL_143_112_2_18_2_49_49_7,"MMIIDLE:MMIAPIIDLESS_OpenSpecialScreenSaver PNULL == scr_win_ptr")
TRACE_MSG(MMISS_SPECIAL_172_112_2_18_2_49_49_8,"HandleSpecialScreenSaverWinMsg ERROR! PNULL == scr_win_ptr")
TRACE_MSG(MMISS_SPECIAL_234_112_2_18_2_49_50_9,"HandleSpecialScreenSaverWinMsg ERROR! MSG_APP_LEFT sel_item_index =%d")
TRACE_MSG(MMISS_SPECIAL_275_112_2_18_2_49_50_10,"HandleSpecialScreenSaverWinMsg ERROR! MSG_APP_RIGHT sel_item_index =%d")
TRACE_MSG(MMISS_SPRITE_507_112_2_18_2_49_51_11,"MMIIDLE_PressMoveSprite ERROR! MMIIDLE_SPRITE_NONE == s_pressed_sprite")
TRACE_MSG(MMISS_SPRITE_513_112_2_18_2_49_51_12,"MMIIDLE_PressMoveSprite ERROR! s_pressed_sprite =%d")
TRACE_MSG(MMISS_SPRITE_618_112_2_18_2_49_51_13,"GetSpriteRect ERROR! PNULL == rect_ptr")
TRACE_MSG(MMISS_SPRITE_639_112_2_18_2_49_51_14,"GetSpriteOldRect ERROR! PNULL == rect_ptr")
END_TRACE_MAP(MMI_APP_SCREENSAVER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_SCREENSAVER_TRC_H_

