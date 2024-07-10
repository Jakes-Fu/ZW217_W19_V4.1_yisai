/******************************************************************************
**  File Name:      mmiatv_id.h                                               *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef _MMIATV_POSITION_H_
#define _MMIATV_POSITION_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_position.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_128X160         //-----------128X160-------------------------------------------------------
#define MMIATV_FULLSCREEN_VIDEO_WIDTH   160
#define MMIATV_FULLSCREEN_VIDEO_HEIGHT  128

//form
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     90//120     //320 - statusbar - softkey - button - progress - tip
#define MMIATV_FORM_SPACE               0
//video
#define MMIATV_VIDEO_WIDTH              128
#define MMIATV_VIDEO_HEIGHT             80//98

//form horizontal
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H   75//100    //240 - statusbar - button - progress - tip
#define MMIATV_FORM_SPACE_H             1
//video horizontal
#define MMIATV_VIDEO_WIDTH_H            128
#define MMIATV_VIDEO_HEIGHT_H           78

#define MMIATV_COMMON_TIP_HEIGHT        20
#define MMIATV_COMMON_TIP_HEIGHT_ADJUST 55
#define MMIATV_PROGRESS_GROOVE_HEIGHT   10
#define MMIATV_PROGRESS_LEFT_SPACE       1

#define MMIATV_PROGRESSBAR_SPACE         1
#define MMIATV_PROGRESSBAR_H_SPACE       1
#define MMIATV_BUTTONFORM_SPACE          0
#define MMIATV_BUTTONFORM_H_SPACE        0
#define MMIATV_FORM_BUTTON_SPACE         4
#define MMIATV_BUTTON_BUTTON_SPACE       8
#define MMIATV_FORM_BUTTON_SPACE_H      7
#define MMIATV_BUTTON_BUTTON_SPACE_H    7//33

#define MMIATV_ISTYLE_TITLE_HEIGHT     42//36 // (< 218-176=42 VIDEO_OWNDRAW_HEIGHT-VIDEO_HEIGHT)
#define MMIATV_BUTTONFORM_H_HEIGHT       25
#define MMIATV_BUTTON_FORM_WIDTH_H      55
#define MMIATV_LABEL_HIGHT_H            30
#define MMIATV_BUTTON_HEIGHT_H          30
#define MMIATV_LABEL_FONT_H             SONG_FONT_14
#define MMIATV_STATUS_HEIGHT_H          30
#define MMIATV_PROCESS_FORM_HEIGHT_H    30 //94
#define MMIATV_SPACE_HEIGHT_H           34

#if defined MAINLCD_DEV_SIZE_160X128
#define ATV_FULLSCREEN_RECT_H  {0, 0, 159, 127}
#define ATV_LAYOUT_OPT_RECT_H  {0, 103, 39, 127}
#define ATV_LAYOUT_PREV_RECT_H  {40, 103, 79, 127}
#define ATV_LAYOUT_NEXT_RECT_H  {80, 103, 119, 127}
#define ATV_LAYOUT_RET_RECT_H  {120, 103, 159, 127}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 30, 25}
#define ATV_LAYOUT_PROC_RECT_H {5, 80, 155, 95}
#define ATV_LAYOUT_NUM_RECT_H  {26, 6, 155, 23}
#define ATV_LAYOUT_TIP_RECT_H  {1, 45, 158, 70}
#define ATV_LAYOUT_COMMONTIP_RECT_H  {5, 97, 159, 122}
#define ATV_LAYOUT_NUM_HEIGHT_H 20 

#else
#define ATV_FULLSCREEN_RECT_H  {0, 0, 159, 127}
#define ATV_LAYOUT_OPT_RECT_H  {104, 96, 159, 127}
#define ATV_LAYOUT_PREV_RECT_H  {135, 64, 159, 95}
#define ATV_LAYOUT_NEXT_RECT_H  {135, 32, 159, 63}
#define ATV_LAYOUT_RET_RECT_H  {116, 0, 159, 31}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 30, 25}
#define ATV_LAYOUT_PROC_RECT_H  {5, 80, 110, 95}
#define ATV_LAYOUT_NUM_RECT_H  {26, 6, 115, 23}
#define ATV_LAYOUT_TIP_RECT_H  {1, 45, 131, 70}
#define ATV_OPT_WIDTH_H  50
#define ATV_LAYOUT_NUM_HEIGHT_H 20 
#endif

#elif MAINLCD_SIZE_176X220         //-----------176*220-------------------------------------------------------
#define MMIATV_FULLSCREEN_VIDEO_WIDTH   220
#define MMIATV_FULLSCREEN_VIDEO_HEIGHT  176

//form
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     120     //320 - statusbar - softkey - button - progress - tip
#define MMIATV_FORM_SPACE               0
//video
#define MMIATV_VIDEO_WIDTH              176
#define MMIATV_VIDEO_HEIGHT             118

//form horizontal
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H   100    //240 - statusbar - button - progress - tip
#define MMIATV_FORM_SPACE_H             1
//video horizontal
#define MMIATV_VIDEO_WIDTH_H            176
#define MMIATV_VIDEO_HEIGHT_H           98

#define MMIATV_COMMON_TIP_HEIGHT        20
#define MMIATV_COMMON_TIP_HEIGHT_ADJUST 25
#define MMIATV_PROGRESS_GROOVE_HEIGHT   10
#define MMIATV_PROGRESS_LEFT_SPACE      1

#define MMIATV_PROGRESSBAR_SPACE       1
#define MMIATV_PROGRESSBAR_H_SPACE       1
#define MMIATV_BUTTONFORM_SPACE       0
#define MMIATV_BUTTONFORM_H_SPACE       0
#define MMIATV_FORM_BUTTON_SPACE   4
#define MMIATV_BUTTON_BUTTON_SPACE  8
#define MMIATV_FORM_BUTTON_SPACE_H      10
#define MMIATV_BUTTON_BUTTON_SPACE_H     33

#define MMIATV_ISTYLE_TITLE_HEIGHT     42//36 // (< 218-176=42 VIDEO_OWNDRAW_HEIGHT-VIDEO_HEIGHT)
#define MMIATV_BUTTONFORM_H_HEIGHT       30//43


#define MMIATV_BUTTONFORM_H_HEIGHT       25
#define MMIATV_BUTTON_FORM_WIDTH_H      55
#define MMIATV_LABEL_HIGHT_H            30
#define MMIATV_BUTTON_HEIGHT_H          55
#define MMIATV_LABEL_FONT_H             SONG_FONT_15
#define MMIATV_STATUS_HEIGHT_H          30
#define MMIATV_PROCESS_FORM_HEIGHT_H    30 //94
#define MMIATV_SPACE_HEIGHT_H           64

#if defined MAINLCD_DEV_SIZE_220X176
#define ATV_FULLSCREEN_RECT_H  {0, 0, 219, 175}
#define ATV_LAYOUT_OPT_RECT_H  {0, 146, 54, 175}
#define ATV_LAYOUT_PREV_RECT_H  {55, 146, 109, 175}
#define ATV_LAYOUT_NEXT_RECT_H  {110, 146, 164, 175}
#define ATV_LAYOUT_RET_RECT_H  {165, 146, 219, 175}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 45, 45}
#define ATV_LAYOUT_PROC_RECT_H  {7, 85, 213, 103}
#define ATV_LAYOUT_NUM_RECT_H  {50, 10, 170, 23}
#define ATV_LAYOUT_TIP_RECT_H  {10, 65, 210, 80}
#define ATV_LAYOUT_COMMONTIP_RECT_H  {5, 105, 219, 133}
#define ATV_LAYOUT_NUM_HEIGHT_H 20 

#else
#define ATV_FULLSCREEN_RECT_H  {0, 0, 219, 175}
#define ATV_LAYOUT_OPT_RECT_H  {165, 136, 219, 175}
#define ATV_LAYOUT_PREV_RECT_H  {178, 46, 219, 90}
#define ATV_LAYOUT_NEXT_RECT_H  {178, 91, 219, 135}
#define ATV_LAYOUT_RET_RECT_H  {175, 0, 219, 45}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 45, 45}
#define ATV_LAYOUT_PROC_RECT_H  {15, 86, 180, 110}
#define ATV_LAYOUT_NUM_RECT_H  {50, 10, 170, 23}
#define ATV_LAYOUT_TIP_RECT_H  {10, 40, 170, 80}
#define ATV_OPT_WIDTH_H  70
#define ATV_LAYOUT_NUM_HEIGHT_H 20 
#endif

#elif defined MAINLCD_SIZE_240X320  //------------240*320-------------------------------------------------------
#define MMIATV_FULLSCREEN_VIDEO_WIDTH   320
#define MMIATV_FULLSCREEN_VIDEO_HEIGHT  240
#ifdef TOUCHPANEL_TYPE
#define MMIATV_FORM_BUTTON_SPACE        8
#define MMIATV_BUTTON_BUTTON_SPACE      23
#define MMIATV_FORM_BUTTON_SPACE_H      9
#define MMIATV_BUTTON_BUTTON_SPACE_H    20
#else
#define MMIATV_FORM_BUTTON_SPACE        10
#define MMIATV_BUTTON_BUTTON_SPACE      37
#define MMIATV_FORM_BUTTON_SPACE_H      8
#define MMIATV_BUTTON_BUTTON_SPACE_H     28
#endif
//form
#ifndef MMI_PDA_SUPPORT
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     184     //320 - statusbar- softkey- button - progress - tip
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H   134    //240 - statusbar - button - progress - tip
#else
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     216//197//210     //320- statusbar - button - progress - tip - space
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H   138    //240 - statusbar - button - progress - tip
#endif
#define MMIATV_FORM_SPACE               2
//video
#define MMIATV_VIDEO_WIDTH              240
#define MMIATV_VIDEO_HEIGHT             176

//form horizontal

#define MMIATV_FORM_SPACE_H             1
//video horizontal
#define MMIATV_VIDEO_WIDTH_H            176
#define MMIATV_VIDEO_HEIGHT_H           126

#define MMIATV_COMMON_TIP_HEIGHT        18
#define MMIATV_PROGRESS_GROOVE_HEIGHT   20
#define MMIATV_PROGRESS_LEFT_SPACE      5


#define MMIATV_PROGRESSBAR_SPACE       4
#define MMIATV_PROGRESSBAR_H_SPACE     2
#define MMIATV_BUTTONFORM_SPACE       15
#define MMIATV_BUTTONFORM_H_SPACE     8


#define MMIATV_ISTYLE_TITLE_HEIGHT     32 // (< 218-176=42 VIDEO_OWNDRAW_HEIGHT-VIDEO_HEIGHT)
#define MMIATV_BUTTONFORM_H_HEIGHT       37

///////////////////////////begin 宏组合添加 数据并非针对当前分辨率///////////////////////
#define MMIATV_COMMON_TIP_HEIGHT_ADJUST 40
#define ATV_FULLSCREEN_RECT_H  {0, 0, 159, 127}
#define ATV_LAYOUT_OPT_RECT_H  {104, 96, 159, 127}
#define ATV_LAYOUT_PREV_RECT_H  {132, 64, 159, 95}
#define ATV_LAYOUT_NEXT_RECT_H  {132, 32, 159, 63}
#define ATV_LAYOUT_RET_RECT_H  {116, 0, 159, 31}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 30, 25}
#define ATV_LAYOUT_PROC_RECT_H  {5, 80, 110, 95}
#define ATV_LAYOUT_NUM_RECT_H  {26, 6, 115, 23}
#define ATV_LAYOUT_TIP_RECT_H  {1, 45, 131, 70}
#define ATV_OPT_WIDTH_H  70
#define ATV_LAYOUT_NUM_HEIGHT_H 20 
///////////////////////////end   宏组合添加 数据并非针对当前分辨率///////////////////////


#elif defined MAINLCD_SIZE_128X64  //------------128*64------------------------------------------------------------
#define MMIATV_FULLSCREEN_VIDEO_WIDTH   320
#define MMIATV_FULLSCREEN_VIDEO_HEIGHT  240
#define MMIATV_FORM_BUTTON_SPACE        8
#define MMIATV_BUTTON_BUTTON_SPACE      23
#define MMIATV_FORM_BUTTON_SPACE_H      10
#define MMIATV_BUTTON_BUTTON_SPACE_H     33

//form
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     180     //320 - statusbar - softkey - button - progress - tip
#define MMIATV_FORM_SPACE               2
//video
#define MMIATV_VIDEO_WIDTH              240
#define MMIATV_VIDEO_HEIGHT             184

//form horizontal
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H   140    //240 - statusbar - button - progress - tip
#define MMIATV_FORM_SPACE_H             1
//video horizontal
#define MMIATV_VIDEO_WIDTH_H            200
#define MMIATV_VIDEO_HEIGHT_H           140

#define MMIATV_COMMON_TIP_HEIGHT        20
#define MMIATV_PROGRESS_GROOVE_HEIGHT   14
#define MMIATV_SEARCH_PROGRESS_WIDTH    218
#define MMIATV_SEARCH_PROGRESS_H_WIDTH    298
#define MMIATV_PROGRESS_LEFT_SPACE      5


#define MMIATV_PROGRESSBAR_SPACE       1
#define MMIATV_PROGRESSBAR_H_SPACE       1
#define MMIATV_BUTTONFORM_SPACE       15
#define MMIATV_BUTTONFORM_H_SPACE     8
#define MMIATV_ISTYLE_TITLE_HEIGHT     36 // (< 218-176=42 VIDEO_OWNDRAW_HEIGHT-VIDEO_HEIGHT)
#define MMIATV_BUTTONFORM_H_HEIGHT       43
///////////////////////////begin 宏组合添加 数据并非针对当前分辨率///////////////////////
#define MMIATV_COMMON_TIP_HEIGHT_ADJUST 40
#define ATV_FULLSCREEN_RECT_H  {0, 0, 159, 127}
#define ATV_LAYOUT_OPT_RECT_H  {104, 96, 159, 127}
#define ATV_LAYOUT_PREV_RECT_H  {132, 64, 159, 95}
#define ATV_LAYOUT_NEXT_RECT_H  {132, 32, 159, 63}
#define ATV_LAYOUT_RET_RECT_H  {116, 0, 159, 31}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 30, 25}
#define ATV_LAYOUT_PROC_RECT_H  {5, 80, 110, 95}
#define ATV_LAYOUT_NUM_RECT_H  {26, 6, 115, 23}
#define ATV_LAYOUT_TIP_RECT_H  {1, 45, 131, 70}
#define ATV_OPT_WIDTH_H  70
#define ATV_LAYOUT_NUM_HEIGHT_H 20 
///////////////////////////end   宏组合添加 数据并非针对当前分辨率///////////////////////

#elif defined MAINLCD_SIZE_240X400  //------------240*400------------------------------------------------------------------------
#define MMIATV_FULLSCREEN_VIDEO_WIDTH   400
#define MMIATV_FULLSCREEN_VIDEO_HEIGHT  240
#ifdef TOUCHPANEL_TYPE
#define MMIATV_FORM_BUTTON_SPACE        8
#define MMIATV_BUTTON_BUTTON_SPACE      23
#define MMIATV_FORM_BUTTON_SPACE_H      9
#define MMIATV_BUTTON_BUTTON_SPACE_H     33
#else
#define MMIATV_FORM_BUTTON_SPACE        10
#define MMIATV_BUTTON_BUTTON_SPACE      37
#define MMIATV_FORM_BUTTON_SPACE_H      16
#define MMIATV_BUTTON_BUTTON_SPACE_H     41
#endif
//form
#ifndef MMI_PDA_SUPPORT
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     224     //400 - statusbar - softkey - button- progress - tip
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H  136// 140    //240 - statusbar - button - progress - tip
#else
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     260//(250)   //400 - statusbar- button - progress - tip- space
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H  141// 140    //240 - statusbar - button - progress - tip
#endif
#define MMIATV_FORM_SPACE               5
//video
#define MMIATV_VIDEO_WIDTH              240
#define MMIATV_VIDEO_HEIGHT             184

//form horizontal

#define MMIATV_FORM_SPACE_H             2
//video horizontal
#define MMIATV_VIDEO_WIDTH_H            176
#define MMIATV_VIDEO_HEIGHT_H           128

#define MMIATV_COMMON_TIP_HEIGHT        20
#define MMIATV_PROGRESS_GROOVE_HEIGHT  15// 14

#define MMIATV_PROGRESS_LEFT_SPACE      6

#define MMIATV_PROGRESSBAR_SPACE       4
#define MMIATV_PROGRESSBAR_H_SPACE     2
#define MMIATV_BUTTONFORM_SPACE       25
#define MMIATV_BUTTONFORM_H_SPACE       8


#define MMIATV_ISTYLE_TITLE_HEIGHT     32 // (< 230-176=54 VIDEO_OWNDRAW_HEIGHT-VIDEO_HEIGHT)
#define MMIATV_BUTTONFORM_H_HEIGHT     40// 43
///////////////////////////begin 宏组合添加 数据并非针对当前分辨率///////////////////////
#define MMIATV_COMMON_TIP_HEIGHT_ADJUST 40
#define ATV_FULLSCREEN_RECT_H  {0, 0, 159, 127}
#define ATV_LAYOUT_OPT_RECT_H  {104, 96, 159, 127}
#define ATV_LAYOUT_PREV_RECT_H  {132, 64, 159, 95}
#define ATV_LAYOUT_NEXT_RECT_H  {132, 32, 159, 63}
#define ATV_LAYOUT_RET_RECT_H  {116, 0, 159, 31}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 30, 25}
#define ATV_LAYOUT_PROC_RECT_H  {5, 80, 110, 95}
#define ATV_LAYOUT_NUM_RECT_H  {26, 6, 115, 23}
#define ATV_LAYOUT_TIP_RECT_H  {1, 45, 131, 70}
#define ATV_OPT_WIDTH_H  70
#define ATV_LAYOUT_NUM_HEIGHT_H 20 
///////////////////////////end   宏组合添加 数据并非针对当前分辨率///////////////////////

#elif defined MAINLCD_SIZE_320X480 //------------320*480------------------------------------------------------------
#define MMIATV_FULLSCREEN_VIDEO_WIDTH   480
#define MMIATV_FULLSCREEN_VIDEO_HEIGHT  320
#ifdef TOUCHPANEL_TYPE
#define MMIATV_FORM_BUTTON_SPACE        10
#define MMIATV_BUTTON_BUTTON_SPACE      30
#define MMIATV_FORM_BUTTON_SPACE_H      13
#define MMIATV_BUTTON_BUTTON_SPACE_H    34
#else
#define MMIATV_FORM_BUTTON_SPACE        26
#define MMIATV_BUTTON_BUTTON_SPACE      46
#define MMIATV_FORM_BUTTON_SPACE_H      25
#define MMIATV_BUTTON_BUTTON_SPACE_H    46
#endif

//form
#ifndef MMI_PDA_SUPPORT
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     262   //480 - statusbar(25) - softkey(45) - button(60) - progress(30) - tip(30)
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H   174   //320 - statusbar - button - progress - tip
#else
#define MMIATV_VIDEO_OWNDRAW_HEIGHT     312   //480 - statusbar(25) - button(74) - progress(30) - tip(30) - space(6+2)
#define MMIATV_VIDEO_OWNDRAW_HEIGHT_H   184   //320 - statusbar - button - progress - tip
#endif
#define MMIATV_FORM_SPACE               6
//video
#define MMIATV_VIDEO_WIDTH              320  
#define MMIATV_VIDEO_HEIGHT             240

//form horizontal
#define MMIATV_FORM_SPACE_H             2
//video horizontal
#define MMIATV_VIDEO_WIDTH_H            224
#define MMIATV_VIDEO_HEIGHT_H           160

#define MMIATV_COMMON_TIP_HEIGHT        20
#define MMIATV_PROGRESS_GROOVE_HEIGHT   38

#define MMIATV_PROGRESS_LEFT_SPACE      4
#define MMIATV_PROGRESSBAR_SPACE       6
#define MMIATV_PROGRESSBAR_H_SPACE     4
#define MMIATV_ISTYLE_TITLE_HEIGHT     48 // (< 305-240=65 VIDEO_OWNDRAW_HEIGHT-VIDEO_HEIGHT) 8的倍数
#define MMIATV_BUTTONFORM_H_HEIGHT     50
///////////////////////////begin 宏组合添加 数据并非针对当前分辨率///////////////////////
#define MMIATV_COMMON_TIP_HEIGHT_ADJUST 40
#define ATV_FULLSCREEN_RECT_H  {0, 0, 159, 127}
#define ATV_LAYOUT_OPT_RECT_H  {104, 96, 159, 127}
#define ATV_LAYOUT_PREV_RECT_H  {132, 64, 159, 95}
#define ATV_LAYOUT_NEXT_RECT_H  {132, 32, 159, 63}
#define ATV_LAYOUT_RET_RECT_H  {116, 0, 159, 31}

#define ATV_LAYOUT_SIGNAL_RECT_H  {4, 4, 30, 25}
#define ATV_LAYOUT_PROC_RECT_H  {5, 80, 110, 95}
#define ATV_LAYOUT_NUM_RECT_H  {26, 6, 115, 23}
#define ATV_LAYOUT_TIP_RECT_H  {1, 45, 131, 70}
#define ATV_OPT_WIDTH_H  70
#define ATV_LAYOUT_NUM_HEIGHT_H 20 
///////////////////////////end   宏组合添加 数据并非针对当前分辨率///////////////////////
#else
#error
#endif

//common
#define MMIATV_VIDEO_LEFT               0//((MMIATV_FULLSCREEN_VIDEO_HEIGHT - MMIATV_VIDEO_WIDTH)/2)
#define MMIATV_VIDEO_TOP                (MMI_STATUSBAR_HEIGHT + (MMIATV_VIDEO_OWNDRAW_HEIGHT - MMIATV_VIDEO_HEIGHT)/2)//8的倍数

#define MMIATV_VIDEO_LEFT_H             ((MMIATV_FULLSCREEN_VIDEO_WIDTH - MMIATV_VIDEO_WIDTH_H)/2)
#define MMIATV_VIDEO_TOP_H              (MMI_STATUSBAR_HEIGHT + MMIATV_VIDEO_OWNDRAW_HEIGHT_H/2 - MMIATV_VIDEO_HEIGHT_H/2)


#endif  //_MMIATV_POSITION_H_
#endif
