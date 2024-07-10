/*****************************************************************************
** File Name:      mmivp_text_id.h                                           *
** Author:                                                                   *
** Date:           2010-3-26                                                  *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#ifndef MMIVP_TEXT_ID_H_
#define MMIVP_TEXT_ID_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmivp_text.h"

#ifdef __cplusplus
extern   "C" {
#endif
    
///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////
#if defined (KEYPAD_TYPE_COMPACT_KEYPAD) || defined (KEYPAD_TYPE_FOUR_KEY_UNIVERSE)

#define TXT_VP_HISTORYLIST_ID TXT_VP_HISTORY_LIST
#define TXT_VP_BOOKMARK_ID TXT_COMMON_FAVORITE
#define TXT_VP_FULLSCREEN_ID TXT_VP_FULLSCREEN_NO_KEY
#define TXT_VP_BRIGHTNESS_ID TXT_BRIGHT_ADJUST
#define TXT_VP_SAVE_BOOKMARK_ID TXT_VP_SAVE_BOOKMARK_NO_KEY
#define TXT_VP_CANCEL_MUTE_ID TXT_TURNOFF_MUTE
#define TXT_VP_MUTE_ID TXT_TURNON_MUTE
#define TXT_VP_HALFSCREEN_ID TXT_VP_HALFSCREEN_NO_KEY

#if defined MMI_VIDEOPLAYER_HELP_SUPPORT 
#ifndef MMI_STREAMING_PLAYER_SUPPORT//NEWMS00171339 
#define TXT_VP_HELP_CONTENT_ID TXT_VP_HELP_CONTENT_NO_STREAMING_NO_KEY
#else
#define TXT_VP_HELP_CONTENT_ID TXT_VP_HELP_CONTENT_NO_KEY
#endif
#endif
#else

#define TXT_VP_HISTORYLIST_ID TXT_VP_HISTORYLIST
#define TXT_VP_BOOKMARK_ID TXT_VP_BOOKMARK
#define TXT_VP_FULLSCREEN_ID TXT_VP_FULLSCREEN
#define TXT_VP_BRIGHTNESS_ID TXT_COMMON_BRIGHTNESS_5
#define TXT_VP_SAVE_BOOKMARK_ID TXT_VP_SAVE_BOOKMARK
#define TXT_VP_CANCEL_MUTE_ID TXT_VP_CANCEL_MUTE
#define TXT_VP_MUTE_ID TXT_VP_MUTE
#define TXT_VP_HALFSCREEN_ID TXT_VP_HALFSCREEN

#if defined MMI_VIDEOPLAYER_HELP_SUPPORT 
#ifndef MMI_STREAMING_PLAYER_SUPPORT

#if defined (MAINLCD_LOGIC_ANGLE_90) || defined (MAINLCD_LOGIC_ANGLE_270)
#define TXT_VP_HELP_CONTENT_ID TXT_VP_HELP_CONTENT_1_SENSOR
#else
#define TXT_VP_HELP_CONTENT_ID TXT_VP_HELP_CONTENT_1
#endif

#else

#if defined (MAINLCD_LOGIC_ANGLE_90) || defined (MAINLCD_LOGIC_ANGLE_270)
#define TXT_VP_HELP_CONTENT_ID TXT_VP_HELP_CONTENT_SENSOR
#else
#ifndef MOTION_SENSOR_TYPE 
#define TXT_VP_HELP_CONTENT_ID TXT_VP_HELP_CONTENT
#else
#define TXT_VP_HELP_CONTENT_ID TXT_NULL
#endif
#endif

#endif
#endif
#endif 

#endif

#endif
