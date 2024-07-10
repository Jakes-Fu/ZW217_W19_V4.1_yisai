/*****************************************************************************
** File Name:      mmiasp_position.h                                       *
** Author:                                                                   *
** Date:           23/03/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2009        byte.guo         Creat
******************************************************************************/
#ifndef _MMIASP_POSITION_H_
#define _MMIASP_POSITION_H_

#if defined MAINLCD_SIZE_176X220
#define MMIASP_PLAY_SPACE_HEIGHT        36
#define MMIASP_PLAY_SPACE_HEIGHT_H      13
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT        0
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT_H      0
#define MMIASP_PLAY_PROGRESS_WIDTH        156
#define MMIASP_PLAY_PROGRESS_WIDTH_H      200

#define MMIASP_RECORDING_OWNDRAW_HEIGHT                         30
#define MMIASP_RECORDING_OWNDRAW_HEIGHT_ISTYLE                  10
#define MMIASP_RECORDING_OWNDRAW_HEIGHT_H                       0
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT                10
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT_H              0

#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT                                 50    
#define MMIASP_PLAY_OWNDRAW_NULL_ISTYLE_HEIGHT                   68  
#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT_H                             5  
#define MMIASP_PLAY_OWNDRAW_PROGRESS_BG_NULL_HEIGHT                    15//进度条背景位置 

#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT                             45    //影响到下面播放栏
#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT_H                            26
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT                             45//iphone style  
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT_H                             20//iphone style
#define MMIASP_PLAY_PROGRESS_OWNER_DRAW_SPACE_HEIGHT                      10

#elif defined MAINLCD_SIZE_240X320
#ifdef MMI_PDA_SUPPORT
#define MMIASP_PLAY_SPACE_HEIGHT        75
#define MMIASP_PLAY_SPACE_HEIGHT_H      5
#else
#define MMIASP_PLAY_SPACE_HEIGHT        97
#define MMIASP_PLAY_SPACE_HEIGHT_H      17
#endif
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT        0
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT_H      0
#define MMIASP_PLAY_PROGRESS_WIDTH        220
#define MMIASP_PLAY_PROGRESS_WIDTH_H      300

#ifdef MMI_PDA_SUPPORT
//#define MMIASP_PLAY_SPACE_HEIGHT        (135 + 40 - 90 - 50)
//#define MMIASP_PLAY_SPACE_HEIGHT_H      (65 + 30 - 20)

#define MMIASP_RECORDING_OWNDRAW_HEIGHT                         30
#define MMIASP_RECORDING_OWNDRAW_HEIGHT_ISTYLE                  10
#define MMIASP_RECORDING_OWNDRAW_HEIGHT_H                       0
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT                10
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT_H              0
//#define MMIASP_RECORDING_NAME_NUMBER_SPACE_HEIGHT             16
//#define MMIASP_RECORDING_NAME_NUMBER_SPACE_HEIGHT_H         13

#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT                                 50    
#define MMIASP_PLAY_OWNDRAW_NULL_ISTYLE_HEIGHT                   68  
#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT_H                             5  
#define MMIASP_PLAY_OWNDRAW_PROGRESS_BG_NULL_HEIGHT                    15//进度条背景位置 

#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT                             45    //影响到下面播放栏
#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT_H                            26
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT                             45//iphone style  
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT_H                             20//iphone style
#define MMIASP_PLAY_PROGRESS_OWNER_DRAW_SPACE_HEIGHT                      10
#endif //MMI_PDA_SUPPORT

#elif defined MAINLCD_SIZE_128X64
#define MMIASP_PLAY_SPACE_HEIGHT        95
#define MMIASP_PLAY_SPACE_HEIGHT_H      25
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT        0
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT_H      0
#define MMIASP_PLAY_PROGRESS_WIDTH        220
#define MMIASP_PLAY_PROGRESS_WIDTH_H      300

#elif defined MAINLCD_SIZE_240X400
#define MMIASP_PLAY_SPACE_HEIGHT        103        //进度条显示和时间等等
#define MMIASP_PLAY_SPACE_HEIGHT_H      10
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT        14
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT_H      0
#define MMIASP_PLAY_PROGRESS_WIDTH        220
#define MMIASP_PLAY_PROGRESS_WIDTH_H      380

#ifdef MMI_PDA_SUPPORT
//#define MMIASP_PLAY_SPACE_HEIGHT        (135 + 40 - 90 - 50)
//#define MMIASP_PLAY_SPACE_HEIGHT_H      (65 + 30 - 20)

#define MMIASP_RECORDING_OWNDRAW_HEIGHT                                20
#define MMIASP_RECORDING_OWNDRAW_HEIGHT_ISTYLE                  10
#define MMIASP_RECORDING_OWNDRAW_HEIGHT_H                            0
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT                48
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT_H            0
//#define MMIASP_RECORDING_NAME_NUMBER_SPACE_HEIGHT             16
//#define MMIASP_RECORDING_NAME_NUMBER_SPACE_HEIGHT_H         13

#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT                                 60  //文字到titile距离
#define MMIASP_PLAY_OWNDRAW_NULL_ISTYLE_HEIGHT                   60  
#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT_H                             10
#define MMIASP_PLAY_OWNDRAW_NULL_ISTYLE_HEIGHT_H                       2
#define MMIASP_PLAY_OWNDRAW_PROGRESS_BG_NULL_HEIGHT                    0

#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT                             66     //进度条背景大小
#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT_H                             20   //文字到滚动条间隔
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT                             62//iphone style
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT_H                             0//iphone style
#define MMIASP_PLAY_PROGRESS_OWNER_DRAW_SPACE_HEIGHT                       7
#endif //MMI_PDA_SUPPORT

#elif defined MAINLCD_SIZE_320X480
#ifndef MMI_PDA_SUPPORT
#define MMIASP_PLAY_SPACE_HEIGHT        135
#define MMIASP_PLAY_SPACE_HEIGHT_H      65
#else
#define MMIASP_PLAY_SPACE_HEIGHT        (135 + 40 - 90 - 50)
#define MMIASP_PLAY_SPACE_HEIGHT_H      (65 + 30 - 20)

#define MMIASP_RECORDING_OWNDRAW_HEIGHT                                50
#define MMIASP_RECORDING_OWNDRAW_HEIGHT_H                            1//10
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT                47
#define MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT_H            0//14
//#define MMIASP_RECORDING_NAME_NUMBER_SPACE_HEIGHT             16
//#define MMIASP_RECORDING_NAME_NUMBER_SPACE_HEIGHT_H         13

#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT                                 80
#define MMIASP_PLAY_OWNDRAW_NULL_ISTYLE_HEIGHT                   60  
#define MMIASP_PLAY_OWNDRAW_NULL_HEIGHT_H                             20

#define MMIASP_PLAY_OWNDRAW_PROGRESS_BG_NULL_HEIGHT                             5

#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT                             75
#define MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT_H                             49
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT                             55//iphone style
#define MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT_H                             29//iphone style

#define MMIASP_PLAY_PROGRESS_OWNER_DRAW_SPACE_HEIGHT                             10
#endif
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT        20
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT_H      0
#define MMIASP_PLAY_PROGRESS_WIDTH        300
#define MMIASP_PLAY_PROGRESS_WIDTH_H      460

#else
#define MMIASP_PLAY_SPACE_HEIGHT        75
#define MMIASP_PLAY_SPACE_HEIGHT_H      5
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT        0
#define MMIASP_PLAY_TEXT_SPACE_HEIGHT_H      0
#define MMIASP_PLAY_PROGRESS_WIDTH        220
#define MMIASP_PLAY_PROGRESS_WIDTH_H      300
#endif

#if defined MAINLCD_SIZE_240X320 && defined MMI_PDA_SUPPORT
#define MMIASP_TIME_FONT    SONG_FONT_20
#define MMIASP_TIME_WIDTH               (80)
#define MMIASP_TIME_HEIGHT              (14)
#define MMIASP_PROGRESS_HORIZ_MARGIN   (4)
#define MMIASP_LANDSCAPE_TIME_FONT    SONG_FONT_16
#else
#define MMIASP_TIME_FONT    SONG_FONT_14
#define MMIASP_TIME_WIDTH               (80)
#define MMIASP_TIME_HEIGHT              (14)
#define MMIASP_PROGRESS_HORIZ_MARGIN   (5)
#define MMIASP_LANDSCAPE_TIME_FONT    SONG_FONT_14
#endif

#endif
