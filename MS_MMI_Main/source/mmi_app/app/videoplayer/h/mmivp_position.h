/*****************************************************************************
** File Name:      mmivp_position.h                                       *
** Author:                                                                   *
** Date:           2010/03/26                                                *
** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/03/26     apple.zhang              Creat
******************************************************************************/
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#ifndef _MMIVP_POSITION_H_
#define _MMIVP_POSITION_H_

#include "mmi_position.h"

#ifdef MAINLCD_SIZE_128X160
#define VP_BUTTON_SPACE  1
#define VP_PROCESS_WIDTH 120
#define VP_PROCESS_WIDTH_H 90 //158 leiming ma
#define VP_PROCESS_HEIGHT 30    
#define VP_PROCESS_HEIGHT_H 26  //30 leiming ma    
#define VP_STATION_FONT MMI_DEFAULT_NORMAL_FONT
#define VP_STATION_MARGIN_H 5  //10  
#define VP_STATION_MARGIN_V 1 //10  
#define VP_TIME_FONT    SONG_FONT_12  //SONG_FONT_14   
#define VP_NAME_FONT    SONG_FONT_14  //SONG_FONT_18 
#define VP_LABEL_FONT   SONG_FONT_13 
#define VP_TITLE_MARGIN 5   
#define VP_VIDEO_AREA_HEIGHT  158   
#define VP_VIDEO_AREA_HEIGHT_H 70//110  
#define VP_VIDEO_AREA_MARGIN_H 68
#define VP_TIME_PROCESS_DISTANCE 0     
#define VP_OWNERDRAW_PROCESS_TOPMARGIN  0  //13  
#define VP_OWNERDRAW_PROCESS_LEFTMARGIN 2  //12 //NEWMS00180223
#define VP_DEFAULTPIC_STATION_DISTANCE 0  //避免在low memory工程中 默认图片和station重叠
#define VP_VIDEO_TITLE_WIDTH_H 70 //6530_low_memory 显示播放文件title的宽度
#define VP_VIDEO_HIGHT_ADJUST_H 6 //调整视频播放区域高度
#define VP_VIDEO_AREA_WIDTH_H 130  //设置视频区域form宽度
#define VP_STATION_ADJUST 2
#define VP_ADJUST_STATION_HEIGHT 3
#define VP_VIDEO_AREA_LEFT 20
#define VP_TIME_ADJUST 0//2
#define VP_SPACE_LABEL_HEIGHT_H 1 
#define VP_MAGRIN 0
#define VP_TITLE_FORM_HEIGHT_H 27
#define VP_LABEL_BACK_WIDTH_H 50
#define VP_STATION_ADJUST_RIGHT 10
#define VP_BUTTON_WIDTH         16
#define VP_RANDOM_BUTTON_RECT_H    {  0,   0,  28,  20}
#define VP_REPEAT_BUTTON_RECT_H    { 29,   0,  57,  20}
#define VP_NAME_LABEL_RECT_H       { 58,   0, 159,  20}
#if defined MMI_STREAMING_PLAYER_SUPPORT
#define VP_NAME_LABEL_NET_RECT_H_LEFT      0
#endif
#ifdef MAINLCD_DEV_SIZE_160X128
#define VP_BUTTON_RIGHT     0
#define VP_VIDEO_OWNDRAW_RECT_H    {  0,  20, 159,  88}
#define VP_PROCESS_OWNDRAW_RECT_H  {  0,  89, 159, 107}
#define VP_OPT_BUTTON_RECT_H       {  0, 107,  25, 127}
#define VP_PRE_BUTTON_RECT_H       { 26, 107,  61, 127}
#define VP_PLAY_BUTTON_RECT_H      { 62, 107,  97, 127}
#define VP_NEXT_BUTTON_RECT_H      { 98, 107, 133, 127}
#define VP_RETURN_BUTTON_RECT_H    {134, 107, 159, 127}
#else  //128X160
//#define VP_BACK_LABEL_RECT_H       {127,   0, 159,  20}
#define VP_BUTTON_RIGHT     20
#define VP_VIDEO_OWNDRAW_RECT_H    {  0,  20, 159, 107}
#define VP_PRE_BUTTON_RECT_H       {140,  87, 156, 103}
#define VP_PLAY_BUTTON_RECT_H      {140,  56, 156,  72}
#define VP_NEXT_BUTTON_RECT_H      {140,  25, 156,  41}
#define VP_PROCESS_OWNDRAW_RECT_H  {  0, 108, 159, 127}
//#define VP_OPTION_LABEL_RECT_H     {107, 108, 159, 127}
#endif

#elif MAINLCD_SIZE_176X220
#define VP_BUTTON_SPACE  1
#define VP_PROCESS_WIDTH 168
#define VP_PROCESS_WIDTH_H 145 //212 leiming ma
#define VP_PROCESS_HEIGHT 30
#ifdef MAINLCD_DEV_SIZE_220X176
#define VP_PROCESS_HEIGHT_H 20
#else
#define VP_PROCESS_HEIGHT_H 30 //30
#endif
#define VP_STATION_FONT MMI_DEFAULT_NORMAL_FONT
#define VP_STATION_MARGIN_H 5//10
#define VP_STATION_MARGIN_V 0//5
#define VP_TIME_FONT    SONG_FONT_15
#define VP_NAME_FONT    SONG_FONT_18
#define VP_LABEL_FONT   SONG_FONT_18
#define VP_TITLE_MARGIN 5
#define VP_VIDEO_AREA_HEIGHT  166
#define VP_BUTTON_NUM   5
#define VP_VIDEO_AREA_MARGIN_H 68
#ifdef MAINLCD_DEV_SIZE_220X176 /*MMI_VIDEO_PLAYER_SCREEN_H_SUPPORT*/
#define VP_TIME_PROCESS_DISTANCE 0
#else
#define VP_TIME_PROCESS_DISTANCE 0
#endif
#ifdef MAINLCD_DEV_SIZE_220X176 //6530_MINI 220X176
#define VP_OWNERDRAW_PROCESS_TOPMARGIN 3
#else
#define VP_OWNERDRAW_PROCESS_TOPMARGIN 5//2//5//确定process在process框中的位置
#endif //end of  MMI_VIDEO_PLAYER_SCREEN_H_SUPPORT //6530_MINI 220X176
#define VP_OWNERDRAW_PROCESS_LEFTMARGIN 2//5
#define VP_DEFAULTPIC_STATION_DISTANCE 5  //避免在low memory工程中 默认图片和station重叠
#define VP_VIDEO_TITLE_WIDTH_H 110 //6530_low_memory 显示播放文件title的宽度
#define VP_VIDEO_HIGHT_ADJUST_H 0  //调整视频播放区域高度
#define VP_VIDEO_AREA_WIDTH_H 180  //设置视频区域form宽度
#define VP_STATION_ADJUST 6
#define VP_VIDEO_AREA_LEFT 20
#ifdef MAINLCD_DEV_SIZE_220X176
#define VP_TIME_ADJUST 0
#else
#define VP_TIME_ADJUST 0
#endif
#define VP_SPACE_LABEL_HEIGHT_H 5
#define VP_MAGRIN 0
#define VP_TITLE_FORM_HEIGHT_H 27
#define VP_PLAY_LABEL_WIDTH_H  1
#define VP_LABEL_BACK_WIDTH_H 50
#ifdef MAINLCD_DEV_SIZE_220X176
#define VP_VIDEO_AREA_HEIGHT_H 87
#else
#define VP_VIDEO_AREA_HEIGHT_H 115
#endif
#define VP_STATION_ADJUST_RIGHT 0
#if defined MMI_STREAMING_PLAYER_SUPPORT
#define VP_NAME_LABEL_NET_RECT_H_LEFT          0
#endif
#ifdef MAINLCD_DEV_SIZE_220X176
    #define VP_BUTTON_RIGHT            0
    #define VP_BUTTON_WIDTH            0
    #define VP_RANDOM_BUTTON_RECT_H    {  0,   0,  32,  24}
    #define VP_REPEAT_BUTTON_RECT_H    { 33,   0,  61,  24}
    #define VP_NAME_LABEL_RECT_H       { 62,   0, 219,  24}
    #define VP_VIDEO_OWNDRAW_RECT_H    {  0,  24, 219, 127}
    #define VP_OPT_BUTTON_RECT_H       {  0, 151,  37, 175}
    #define VP_PRE_BUTTON_RECT_H       { 38, 151,  85, 175}
    #define VP_PLAY_BUTTON_RECT_H      { 86, 151, 133, 175}
    #define VP_NEXT_BUTTON_RECT_H      {134, 151, 181, 175}
    #define VP_RETURN_BUTTON_RECT_H    {182, 151, 219, 175}
    #define VP_PROCESS_OWNDRAW_RECT_H  {  0, 128, 219, 150}
#else
    #define VP_BUTTON_RIGHT            33
    #define VP_BUTTON_WIDTH            26
    #define VP_RANDOM_BUTTON_RECT_H    {  0,   0,  30,  24}
    #define VP_REPEAT_BUTTON_RECT_H    { 31,   0,  61,  24}
    #define VP_NAME_LABEL_RECT_H       { 62,   0, 219,  24}
//     #define VP_BACK_LABEL_RECT_H       {171,   0, 219,  24}
    #define VP_VIDEO_OWNDRAW_RECT_H    {  0,  24, 219, 150}
    #define VP_PRE_BUTTON_RECT_H       {188, 118, 214, 144}
    #define VP_PLAY_BUTTON_RECT_H      {188,  74, 214, 100}
    #define VP_NEXT_BUTTON_RECT_H      {188,  30, 214,  56}
    #define VP_PROCESS_OWNDRAW_RECT_H  {  0, 149, 219, 175}
//     #define VP_OPTION_LABEL_RECT_H     {158, 149, 219, 175}
#endif //MAINLCD_DEV_SIZE_220X176


#elif defined MAINLCD_SIZE_240X320
#ifdef MMI_PDA_SUPPORT
#define VP_BUTTON_SPACE  5//button之间的距离
#define VP_PROCESS_WIDTH 240//232//进度条长度 NEWMS00142298
#define VP_PROCESS_WIDTH_H 320//312//320-2*4 NEWMS00142298
#define VP_PROCESS_HEIGHT 39//进度条的宽度
#define VP_PROCESS_HEIGHT_H 39//
#define VP_STATION_FONT MMI_DEFAULT_NORMAL_FONT
#define VP_STATION_MARGIN_H 10
#define VP_STATION_MARGIN_V 10
#define VP_TIME_FONT    SONG_FONT_14
#define VP_NAME_FONT    SONG_FONT_18
#define VP_TITLE_MARGIN 0//
#define VP_VIDEO_AREA_HEIGHT  162//
#define VP_VIDEO_AREA_HEIGHT_H 110//
#define  VP_VIDEO_AREA_MARGIN_H  68
#define VP_TIME_PROCESS_DISTANCE 1
#else
#define VP_BUTTON_SPACE  10
#define VP_PROCESS_WIDTH 240
#define VP_PROCESS_WIDTH_H 320
#ifdef TOUCH_PANEL_SUPPORT
#define VP_PROCESS_HEIGHT 39
#else
#define VP_PROCESS_HEIGHT 40
#endif
#define VP_PROCESS_HEIGHT_H 39
#define VP_STATION_FONT MMI_DEFAULT_NORMAL_FONT
#define VP_STATION_MARGIN_H 10
#define VP_STATION_MARGIN_V 10
#define VP_TIME_FONT    SONG_FONT_14
#define VP_NAME_FONT    SONG_FONT_18
#if defined TOUCH_PANEL_SUPPORT
#define VP_TITLE_MARGIN 5
#else
#define VP_TITLE_MARGIN 4
#endif
#define VP_VIDEO_AREA_HEIGHT  166
#define VP_VIDEO_AREA_HEIGHT_H 118
#define VP_VIDEO_AREA_MARGIN_H 68
#define VP_TIME_PROCESS_DISTANCE 1
//////////////////begin 宏组合添加，数据内容并非针对当前分辨率///////////////
#define VP_BUTTON_WIDTH            0
#define VP_STATION_ADJUST 2
#define VP_TIME_ADJUST 2
#define VP_ADJUST_STATION_HEIGHT 3
#define VP_VIDEO_AREA_LEFT 20
#define VP_STATION_ADJUST_RIGHT 10
#define VP_VIDEO_AREA_WIDTH_H 180  //设置视频区域宽度
#if defined MMI_STREAMING_PLAYER_SUPPORT
#define VP_NAME_LABEL_NET_RECT_H_LEFT          0
#endif
#define VP_RANDOM_BUTTON_RECT_H    {  0,   0,  30,  24}
#define VP_REPEAT_BUTTON_RECT_H    { 31,   0,  61,  24}
#define VP_NAME_LABEL_RECT_H       { 62,   0, 174,  24}
#define VP_BACK_LABEL_RECT_H       {175,   0, 219,  24}
#define VP_VIDEO_OWNDRAW_RECT_H    {  0,  24, 219, 150}
#define VP_PRE_BUTTON_RECT_H       {188, 118, 214, 144}
#define VP_PLAY_BUTTON_RECT_H      {188,  74, 214, 100}
#define VP_NEXT_BUTTON_RECT_H      {188,  30, 214,  56}
#define VP_PROCESS_OWNDRAW_RECT_H  {  0, 149, 162, 175}
#define VP_OPTION_LABEL_RECT_H     {162, 149, 219, 175}
//////////////////end  宏组合添加，数据内容并非针对当前分辨率///////////////
#endif

#define VP_OWNERDRAW_PROCESS_TOPMARGIN 5//确定process在process框中的位置
#define VP_OWNERDRAW_PROCESS_LEFTMARGIN 5


#elif defined MAINLCD_SIZE_240X400
#define VP_BUTTON_SPACE  1

#define VP_PROCESS_WIDTH 240


#define VP_PROCESS_WIDTH_H 400


#ifdef MMI_PDA_SUPPORT
#define VP_PROCESS_HEIGHT 39
#define VP_PROCESS_HEIGHT_H 39
#else
#define VP_PROCESS_HEIGHT 39
#define VP_PROCESS_HEIGHT_H 39
#endif

#define VP_STATION_FONT MMI_DEFAULT_NORMAL_FONT
#define VP_STATION_MARGIN_H 10
#define VP_STATION_MARGIN_V 10
#define VP_TIME_FONT    SONG_FONT_14
#define VP_NAME_FONT    SONG_FONT_18

#ifdef MMI_PDA_SUPPORT
#define VP_TITLE_MARGIN 0
#else
#define VP_TITLE_MARGIN 5
#endif

#define VP_VIDEO_AREA_HEIGHT  166
#define VP_VIDEO_AREA_HEIGHT_H 118
#define VP_VIDEO_AREA_MARGIN_H 68
#define VP_TIME_PROCESS_DISTANCE 1


#define VP_OWNERDRAW_PROCESS_TOPMARGIN 4//NEWMS00176580
#define VP_OWNERDRAW_PROCESS_LEFTMARGIN 5
//////////////////begin 宏组合添加，数据内容并非针对当前分辨率///////////////
#define VP_BUTTON_WIDTH            0
#define VP_STATION_ADJUST 2
#define VP_TIME_ADJUST 2
#define VP_ADJUST_STATION_HEIGHT 3
#define VP_VIDEO_AREA_LEFT 20
#define VP_STATION_ADJUST_RIGHT 10
#define VP_VIDEO_AREA_WIDTH_H 180  //设置视频区域宽度
#if defined MMI_STREAMING_PLAYER_SUPPORT
#define VP_NAME_LABEL_NET_RECT_H_LEFT          0
#endif
#define VP_RANDOM_BUTTON_RECT_H    {  0,   0,  30,  24}
#define VP_REPEAT_BUTTON_RECT_H    { 31,   0,  61,  24}
#define VP_NAME_LABEL_RECT_H       { 62,   0, 174,  24}
#define VP_BACK_LABEL_RECT_H       {175,   0, 219,  24}
#define VP_VIDEO_OWNDRAW_RECT_H    {  0,  24, 219, 150}
#define VP_PRE_BUTTON_RECT_H       {188, 118, 214, 144}
#define VP_PLAY_BUTTON_RECT_H      {188,  74, 214, 100}
#define VP_NEXT_BUTTON_RECT_H      {188,  30, 214,  56}
#define VP_PROCESS_OWNDRAW_RECT_H  {  0, 149, 162, 175}
#define VP_OPTION_LABEL_RECT_H     {162, 149, 219, 175}
//////////////////end  宏组合添加，数据内容并非针对当前分辨率///////////////

#elif  defined MAINLCD_SIZE_320X480

#ifdef MMI_PDA_SUPPORT//andy.he_crNEWMS00112870
#define VP_BUTTON_SPACE  10
#else
#define VP_BUTTON_SPACE  21
#endif


#define VP_PROCESS_WIDTH 320



#define VP_PROCESS_WIDTH_H 480



#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
#define VP_PROCESS_HEIGHT   60
#else
#define VP_PROCESS_HEIGHT   60
#endif

 #ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
 #define VP_PROCESS_HEIGHT_H 60
 #else
#define VP_PROCESS_HEIGHT_H 60
#endif

#define VP_STATION_FONT MMI_DEFAULT_NORMAL_FONT
#define VP_STATION_MARGIN_H 10
#define VP_STATION_MARGIN_V 10
#define VP_TIME_FONT    SONG_FONT_14
#define VP_NAME_FONT    SONG_FONT_18
#define VP_TITLE_MARGIN     11
#define VP_VIDEO_AREA_HEIGHT  260
#define VP_VIDEO_AREA_HEIGHT_H 186
#define VP_VIDEO_AREA_MARGIN_H 27
#define VP_TIME_PROCESS_DISTANCE 4


#define VP_OWNERDRAW_PROCESS_TOPMARGIN 13
#define VP_OWNERDRAW_PROCESS_LEFTMARGIN 12//NEWMS00180223
//////////////////begin 宏组合添加，数据内容并非针对当前分辨率///////////////
#define VP_BUTTON_WIDTH            0
#define VP_STATION_ADJUST 2
#define VP_ADJUST_STATION_HEIGHT 3
#define VP_TIME_ADJUST 2
#define VP_VIDEO_AREA_LEFT 20
#define VP_STATION_ADJUST_RIGHT 10
#define VP_VIDEO_AREA_WIDTH_H 180  //设置视频区域宽度
#define VP_RANDOM_BUTTON_RECT_H    {  0,   0,  30,  24}
#define VP_REPEAT_BUTTON_RECT_H    { 31,   0,  61,  24}
#define VP_NAME_LABEL_RECT_H       { 62,   0, 174,  24}
#define VP_BACK_LABEL_RECT_H       {175,   0, 219,  24}
#define VP_VIDEO_OWNDRAW_RECT_H    {  0,  24, 219, 150}
#define VP_PRE_BUTTON_RECT_H       {188, 118, 214, 144}
#define VP_PLAY_BUTTON_RECT_H      {188,  74, 214, 100}
#define VP_NEXT_BUTTON_RECT_H      {188,  30, 214,  56}
#define VP_PROCESS_OWNDRAW_RECT_H  {  0, 149, 162, 175}
#define VP_OPTION_LABEL_RECT_H     {162, 149, 219, 175}
//////////////////end  宏组合添加，数据内容并非针对当前分辨率///////////////


#else
#define VP_BUTTON_SPACE  10
#define VP_PROCESS_WIDTH 230
#define VP_PROCESS_WIDTH_H 310
#define VP_PROCESS_HEIGHT 22
#define VP_PROCESS_HEIGHT_H 20
#define VP_STATION_FONT MMI_DEFAULT_NORMAL_FONT
#define VP_STATION_MARGIN_H 10
#define VP_STATION_MARGIN_V 10
#define VP_TIME_FONT    SONG_FONT_14
#define VP_NAME_FONT    SONG_FONT_18
#define VP_TITLE_MARGIN     5
#define VP_VIDEO_AREA_HEIGHT  180
#define VP_VIDEO_AREA_HEIGHT_H 134
#define VP_VIDEO_AREA_MARGIN_H 68
#define VP_TIME_PROCESS_DISTANCE 0
//////////////////begin 宏组合添加，数据内容并非针对当前分辨率///////////////
#define VP_BUTTON_WIDTH            0
#define VP_STATION_ADJUST 2
#define VP_ADJUST_STATION_HEIGHT 3
#define VP_TIME_ADJUST 2
#define VP_VIDEO_AREA_LEFT 20
#define VP_STATION_ADJUST_RIGHT 10
#define VP_VIDEO_AREA_WIDTH_H 180  //设置视频区域宽度
#define VP_RANDOM_BUTTON_RECT_H    {  0,   0,  30,  24}
#define VP_REPEAT_BUTTON_RECT_H    { 31,   0,  61,  24}
#define VP_NAME_LABEL_RECT_H       { 62,   0, 174,  24}
#define VP_BACK_LABEL_RECT_H       {175,   0, 219,  24}
#define VP_VIDEO_OWNDRAW_RECT_H    {  0,  24, 219, 150}
#define VP_PRE_BUTTON_RECT_H       {188, 118, 214, 144}
#define VP_PLAY_BUTTON_RECT_H      {188,  74, 214, 100}
#define VP_NEXT_BUTTON_RECT_H      {188,  30, 214,  56}
#define VP_PROCESS_OWNDRAW_RECT_H  {  0, 149, 162, 175}
#define VP_OPTION_LABEL_RECT_H     {162, 149, 219, 175}
//////////////////end  宏组合添加，数据内容并非针对当前分辨率///////////////
#endif

#endif

#endif
