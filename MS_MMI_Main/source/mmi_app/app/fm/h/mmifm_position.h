/****************************************************************************
** File Name:      mmifm_displaysize.h                                        *
** Author:                                                                 *
** Date:           13/04/2007                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the mmi cc internal api   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2007        maggie.ren         Create
****************************************************************************/

#ifndef _MMIFM_POSITION_H
#define _MMIFM_POSITION_H
#ifdef MAINLCD_SIZE_240X400
    #define  MMIFM_H_PAUSE_BUTTON_V_SPACE  8//to do
    #define  MMIFM_V_FREQUENCY_TEXT_START_Y 14
    #define  FREQUENCY_TEXT_HEIGHT          15
    #define  MMIFM_V_FREQUENCY_TEXT_START_X 0
    #define  MMIFM_FRE_LEFT_MERGIN        9//
    #define  MMIFM_FRE_RIGHT_MERGIN      9//
    #define  MMIFM_TITLE_HEIGHT             21
    #define  MMIFM_V_PAUSE_BUTTON_V_SPACE  9
    #define  MMIFM_V_LEFT_BUTTON_START_X  5
    #define  MMIFM_V_LEFT_BUTTON_START_Y  205
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_X 10
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_Y 293
    #define  MMIFM_V_SEARCH_BUTTON_H_SPACE 8
    #define  MMIFM_V_SEARCH_BUTTON_V_SPACE 14
    #define  MMIFM_V_LABEL_WIDTH  172
    #define  MMIFM_H_FREQUENCY_TEXT_START_Y  14 //to do
    #define  MMIFM_H_FREQUENCY_TEXT_START_X  15//to do

    #define  MMIFM_H_LEFT_BUTTON_START_X  85
    #define  MMIFM_H_LEFT_BUTTON_START_Y  101
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_X 70
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_Y 154
    #define  MMIFM_H_SEARCH_BUTTON_H_SPACE 28
    #define  MMIFM_H_SEARCH_BUTTON_V_SPACE 4
    #define  MMIFM_H_LABEL_WIDTH  164
    
    #define MMIFM_ICON_X                  13  
    #define MMIFM_ICON_Y                   73
    #define MMIFM_DIRECT_BUTTON_WINDTH     65  
    #define MMIFM_DIRECT_BUTTON_HEIGHT     29
    #define MMIFM_AUTO_UP_Y                244
    #define MMIFM_AUTO_DOWN_Y              304 
    #define MMIFM_LEFT_X                   7
    #define MMIFM_LEFT_Y                   274   
    #define MMIFM_RIGHT_Y                  274 
    #define MMIFM_SPK_Y                    274 
    #define MMIFM_ADJ_BG_Y                 216
    #define MMIFM_ADJ_BG_Y                 216
    #define MMIFM_ADJ_BUT_LEFT_Y           222
    #define MMIFM_ADJ_BUT_RIGHT_Y          222
    #define MMIFM_ADJ_BUT_RIGHT_Y          222
    #define MMIFM_ADJ_LEFT_Y               216
    #define MMIFM_ADJ_LEFT_X               4 
    #define MMIFM_ADJ_RIGHT_Y              216
    #define MMIFM_ADJ_RIGHT_X              215 
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_X  39
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_X 159
    #define MMIFM_ADJ_LEFT_TEXT_X          39 
    #define MMIFM_ADJ_RIGHT_TEXT_X         159  
    #define MMIFM_ADJ_LEFT_TEXT_Y          240
    #define MMIFM_ADJ_RIGHT_TEXT_Y         240
    #define MMIFM_CHANNEL_TEXT_Y           21  
    #define MMIFM_FREQUCE_TEXT_Y           123
    #define MMIFM_FREQUCE_TEXT_LEFT_X      130
    #define MMIFM_FREQUCE_TEXT_RIGHT_X     230
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_Y  240
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_Y 240
    #define MMIFM_ADJ_BUT_V_START_Y        0 //the space between stabar and the fre pic
    #define MMIFM_ADJ_BUT_H_START_Y        0 //the space between stabar and the fre pic
    #define MMIFM_VOLUME_TOP               281
    //#define MMIFM_VOLUME_LEFT              211
    #define MMIFM_VOLUME_WIDTH             28
    #define MMIFM_VOLUME_HEIGHT            24
    #define MMIFM_VOLUME_BOTTOM            321
    #define MMIFM_ADJ_BG_X                 4  
    #define MMIFM_ADJ_BUT_LEFT_X           39
    #define MMIFM_ADJ_BUT_RIGHT_X          199 
	#define MMIFM_ADJ_BUT_LEFT_X_H           25
	#define MMIFM_ADJ_BUT_LEFT_Y_H           50
	#define MMIFM_ADJ_BUT_LEFT_X_V           13
	#define MMIFM_ADJ_BUT_LEFT_Y_V           77
	#define MMIFM_FILENAME_FONT                                CAF_FONT_18
	#define MMIFM_LANDSCAPE_BUTTONFORM_HSPACE      0
	#define MMIFM_LANDSCAPE_ANIM_HSPACE                 0
	#define MMIFM_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
	#define MMIFM_TIME_OWNDRAW_HEIGHT                    50
	#define MMIFM_PORTRAIT_BUTTONFORM_HSPACE      60
	#define MMIFM_PORTRAIT_ANIM_HSPACE                 50
	#define MMIFMRECORD_TIME_FONT                                SONG_FONT_52
#elif defined MAINLCD_SIZE_240X320
    #define  MMIFM_H_PAUSE_BUTTON_V_SPACE  8//to do
    #define  MMIFM_V_FREQUENCY_TEXT_START_Y 165
    #define  FREQUENCY_TEXT_HEIGHT          30
    #define  MMIFM_V_FREQUENCY_TEXT_START_X 34

    #define  MMIFM_FRE_LEFT_MERGIN        9//
    #define  MMIFM_FRE_RIGHT_MERGIN      9//
    #define  MMIFM_TITLE_HEIGHT             21
    #define  MMIFM_V_PAUSE_BUTTON_V_SPACE  9
    #define  MMIFM_V_LEFT_BUTTON_START_X  5
    #define  MMIFM_V_LEFT_BUTTON_START_Y  165
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_X 10
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_Y 227
    #define  MMIFM_V_SEARCH_BUTTON_H_SPACE 8
    #define  MMIFM_V_SEARCH_BUTTON_V_SPACE 10
    #define  MMIFM_V_LABEL_WIDTH  172

    #define  MMIFM_H_FREQUENCY_TEXT_START_Y  101
    #define  MMIFM_H_FREQUENCY_TEXT_START_X  78

    #define  MMIFM_H_LEFT_BUTTON_START_X  49
    #define  MMIFM_H_LEFT_BUTTON_START_Y  101
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_X 40
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_Y 160
    #define  MMIFM_H_SEARCH_BUTTON_H_SPACE 18
    #define  MMIFM_H_SEARCH_BUTTON_V_SPACE 4
    #define  MMIFM_H_LABEL_WIDTH  164
    
    #define MMIFM_ICON_X                  13  
    #define MMIFM_ICON_Y                   42
    #define MMIFM_DIRECT_BUTTON_WINDTH     65  
    #define MMIFM_DIRECT_BUTTON_HEIGHT     29
    #define MMIFM_AUTO_UP_Y                194   
    #define MMIFM_AUTO_DOWN_Y              254 
    #define MMIFM_LEFT_X                   7
    #define MMIFM_LEFT_Y                   224  
    #define MMIFM_RIGHT_Y                  224
    #define MMIFM_SPK_Y                    224 
    #define MMIFM_KPD_Y                    188
    #define MMIFM_ADJ_BG_Y                 151
    #define MMIFM_ADJ_BUT_LEFT_Y           157
    #define MMIFM_ADJ_BUT_RIGHT_Y          157
    #define MMIFM_ADJ_LEFT_Y               151
    #define MMIFM_ADJ_LEFT_X               4 
    #define MMIFM_ADJ_RIGHT_Y              151
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_X  39
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_X 159
    #define MMIFM_ADJ_LEFT_TEXT_X          39 
    #define MMIFM_ADJ_RIGHT_TEXT_X         159 
    #define MMIFM_ADJ_LEFT_TEXT_Y          175
    #define MMIFM_ADJ_RIGHT_X             215  
    #define MMIFM_ADJ_RIGHT_TEXT_Y         175
    #define MMIFM_CHANNEL_TEXT_Y           16  
    #define MMIFM_FREQUCE_TEXT_Y           90
    #define MMIFM_FREQUCE_TEXT_LEFT_X      89
    #define MMIFM_FREQUCE_TEXT_RIGHT_X     239
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_Y  175
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_Y 175
    #define MMIFM_ADJ_BUT_V_START_Y        1//the space between stabar and the fre pic
    #define MMIFM_ADJ_BUT_H_START_Y        1//the space between stabar and the fre pic
    #define MMIFM_VOLUME_TOP               221
    //#define MMIFM_VOLUME_LEFT              211
    #define MMIFM_VOLUME_WIDTH             28
    #define MMIFM_VOLUME_HEIGHT            24
    #define MMIFM_VOLUME_BOTTOM            261
    #define MMIFM_ADJ_BG_X                 4  
    #define MMIFM_ADJ_BUT_LEFT_X           39
    #define MMIFM_ADJ_BUT_RIGHT_X          199 
	#define MMIFM_ADJ_BUT_LEFT_X_H           22
	#define MMIFM_ADJ_BUT_LEFT_Y_H           38
	#define MMIFM_ADJ_BUT_LEFT_X_V           8
	#define MMIFM_ADJ_BUT_LEFT_Y_V           42
	#define MMIFM_FILENAME_FONT                                CAF_FONT_14
	#define MMIFM_LANDSCAPE_BUTTONFORM_HSPACE      15
	    #define MMIFM_LANDSCAPE_ANIM_HSPACE                 0
	#define MMIFM_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
	#define MMIFM_TIME_OWNDRAW_HEIGHT                    45
	#define MMIFM_PORTRAIT_BUTTONFORM_HSPACE      12
	#define MMIFM_PORTRAIT_ANIM_HSPACE                 0
	#define MMIFMRECORD_TIME_FONT           SONG_FONT_25
#elif defined MAINLCD_SIZE_176X220
    #define  MMIFM_TITLE_HEIGHT             24
    #define  MMIFM_V_FREQUENCY_TEXT_START_Y 29
    #define  FREQUENCY_TEXT_HEIGHT          16
    #define  MMIFM_V_FREQUENCY_TEXT_START_X 0
    #define  MMIFM_V_LEFT_BUTTON_START_X  5
    #define  MMIFM_V_LEFT_BUTTON_START_Y  195
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_X 14
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_Y 263
    #define  MMIFM_V_PAUSE_BUTTON_V_SPACE  16
    #define  MMIFM_V_SEARCH_BUTTON_H_SPACE 30
    #define  MMIFM_V_SEARCH_BUTTON_V_SPACE 19
    #define  MMIFM_V_LABEL_WIDTH  172

    #define  MMIFM_H_FREQUENCY_TEXT_START_Y  22 //to do
    #define  MMIFM_H_FREQUENCY_TEXT_START_X  0//to do
    #define  MMIFM_H_LEFT_BUTTON_START_X  85
    #define  MMIFM_H_LEFT_BUTTON_START_Y  91
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_X 26
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_Y 130
    #define  MMIFM_H_PAUSE_BUTTON_V_SPACE  8//to do
    #define  MMIFM_H_SEARCH_BUTTON_H_SPACE 40
    #define  MMIFM_H_SEARCH_BUTTON_V_SPACE 11
    #define  MMIFM_H_LABEL_WIDTH  220
    #define MMIFM_ICON_X                   10  
    #define MMIFM_ICON_Y                   23
    #define MMIFM_DIRECT_BUTTON_WINDTH     40  
    #define MMIFM_DIRECT_BUTTON_HEIGHT     20
    #define MMIFM_AUTO_UP_Y                129   
    #define MMIFM_AUTO_DOWN_Y              171 
    #define MMIFM_LEFT_X                   9
    #define MMIFM_LEFT_Y                   150  
    #define MMIFM_RIGHT_Y                  150 
    #define MMIFM_SPK_Y                    150 
    #define MMIFM_KPD_Y                    188
    #define MMIFM_ADJ_BG_Y                 86
    #define MMIFM_ADJ_BUT_LEFT_Y           92
    #define MMIFM_ADJ_BUT_RIGHT_Y          92
    #define MMIFM_ADJ_LEFT_Y               86
    #define MMIFM_ADJ_LEFT_X               0 
    #define MMIFM_ADJ_RIGHT_Y              86
    #define MMIFM_ADJ_RIGHT_X              156 
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_X  24
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_X 108
    #define MMIFM_ADJ_LEFT_TEXT_X          24 
    #define MMIFM_ADJ_RIGHT_TEXT_X         108  
    #define MMIFM_ADJ_LEFT_TEXT_Y          108
    #define MMIFM_ADJ_RIGHT_TEXT_Y         108
    #define MMIFM_CHANNEL_TEXT_Y           10  
    #define MMIFM_FREQUCE_TEXT_Y           50
    #define MMIFM_FREQUCE_TEXT_LEFT_X      60
    #define MMIFM_FREQUCE_TEXT_RIGHT_X     175
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_Y  108
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_Y 108
    #define MMIFM_VOLUME_TOP               140
    #define MMIFM_VOLUME_WIDTH             25
    #define MMIFM_VOLUME_HEIGHT            16
    #define MMIFM_VOLUME_BOTTOM            175
    #define MMIFM_ADJ_BG_X                1
    #define MMIFM_ADJ_BUT_V_START_Y        5//the space between wintitle and the fre pic
    #define MMIFM_ADJ_BUT_H_START_Y        4//the space between wintitle and the fre pic
    #define MMIFM_ADJ_BUT_LEFT_X          20
    #define MMIFM_ADJ_BUT_RIGHT_X         146
	#define MMIFM_ADJ_BUT_LEFT_X_H           25
	#define MMIFM_ADJ_BUT_LEFT_Y_H           52
	#define MMIFM_ADJ_BUT_LEFT_X_V           13
	#define MMIFM_ADJ_BUT_LEFT_Y_V           80
	#define MMIFM_FILENAME_FONT                                CAF_FONT_14
	#define MMIFM_LANDSCAPE_BUTTONFORM_HSPACE      0
	    #define MMIFM_LANDSCAPE_ANIM_HSPACE                 0
	#define MMIFM_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
	#define MMIFM_TIME_OWNDRAW_HEIGHT                    50
	#define MMIFM_PORTRAIT_BUTTONFORM_HSPACE      15
	#define MMIFM_PORTRAIT_ANIM_HSPACE                 0
	#define MMIFMRECORD_TIME_FONT           SONG_FONT_25
#elif defined MAINLCD_SIZE_128X160
    #define  MMIFM_TITLE_HEIGHT             21
    #define  MMIFM_V_FREQUENCY_TEXT_START_Y 14
    #define  FREQUENCY_TEXT_HEIGHT          15
    #define  MMIFM_V_FREQUENCY_TEXT_START_X 0
    #define  MMIFM_V_LEFT_BUTTON_START_X  5
    #define  MMIFM_V_LEFT_BUTTON_START_Y  145
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_X 11
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_Y 155
    #define  MMIFM_V_PAUSE_BUTTON_V_SPACE  9
    #define  MMIFM_V_SEARCH_BUTTON_H_SPACE 21
    #define  MMIFM_V_SEARCH_BUTTON_V_SPACE 12
    #define  MMIFM_V_LABEL_WIDTH  120

    #define  MMIFM_H_FREQUENCY_TEXT_START_Y  11 
    #define  MMIFM_H_FREQUENCY_TEXT_START_X  15//to do
    #define  MMIFM_H_LEFT_BUTTON_START_X  85//to do
    #define  MMIFM_H_LEFT_BUTTON_START_Y  91//to do
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_X 24
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_Y 130//to do
    #define  MMIFM_H_PAUSE_BUTTON_V_SPACE  3
    #define  MMIFM_H_SEARCH_BUTTON_H_SPACE 24
    #define  MMIFM_H_SEARCH_BUTTON_V_SPACE 6
    #define  MMIFM_H_LABEL_WIDTH  114//to do
    #define MMIFM_ICON_X                   10  
    #define MMIFM_ICON_Y                   23
    #define MMIFM_DIRECT_BUTTON_WINDTH     40  
    #define MMIFM_DIRECT_BUTTON_HEIGHT     20
    #define MMIFM_AUTO_UP_Y                129   
    #define MMIFM_AUTO_DOWN_Y              125 
    #define MMIFM_LEFT_X                   9
    #define MMIFM_LEFT_Y                   120  
    #define MMIFM_RIGHT_Y                  120 
    #define MMIFM_SPK_Y                    120 
    #define MMIFM_KPD_Y                    148
    #define MMIFM_ADJ_BG_Y                 86
    #define MMIFM_ADJ_BUT_LEFT_Y           92
    #define MMIFM_ADJ_BUT_RIGHT_Y          92
    #define MMIFM_ADJ_LEFT_Y               86
    #define MMIFM_ADJ_LEFT_X               0 
    #define MMIFM_ADJ_RIGHT_Y              86
    #define MMIFM_ADJ_RIGHT_X              126 
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_X  24
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_X 108
    #define MMIFM_ADJ_LEFT_TEXT_X          24 
    #define MMIFM_ADJ_RIGHT_TEXT_X         108  
    #define MMIFM_ADJ_LEFT_TEXT_Y          108
    #define MMIFM_ADJ_RIGHT_TEXT_Y         108
    #define MMIFM_CHANNEL_TEXT_Y           10  
    #define MMIFM_FREQUCE_TEXT_Y           50
    #define MMIFM_FREQUCE_TEXT_LEFT_X      60
    #define MMIFM_FREQUCE_TEXT_RIGHT_X     127
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_Y  108
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_Y 108
    #define MMIFM_VOLUME_TOP               140
    #define MMIFM_VOLUME_WIDTH             25
    #define MMIFM_VOLUME_HEIGHT            16
    #define MMIFM_VOLUME_BOTTOM            127
    #define MMIFM_ADJ_BG_X                1
    #define MMIFM_ADJ_BUT_V_START_Y        4//the space between wintitle and the fre pic
    #define MMIFM_ADJ_BUT_H_START_Y        3
    #define MMIFM_ADJ_BUT_LEFT_X          20
    #define MMIFM_ADJ_BUT_RIGHT_X         106
	#define MMIFM_ADJ_BUT_LEFT_X_H           25
	#define MMIFM_ADJ_BUT_LEFT_Y_H           52
	#define MMIFM_ADJ_BUT_LEFT_X_V           13
	#define MMIFM_ADJ_BUT_LEFT_Y_V           80
	#define MMIFM_FILENAME_FONT                                CAF_FONT_14
	#define MMIFM_LANDSCAPE_BUTTONFORM_HSPACE      0
	    #define MMIFM_LANDSCAPE_ANIM_HSPACE                 0
	#define MMIFM_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
	#define MMIFM_TIME_OWNDRAW_HEIGHT                    50
	#define MMIFM_PORTRAIT_BUTTONFORM_HSPACE      15
	#define MMIFM_PORTRAIT_ANIM_HSPACE                 0
	#define MMIFMRECORD_TIME_FONT           SONG_FONT_20  
#else
    #define  MMIFM_TITLE_HEIGHT             21
    #define  MMIFM_V_FREQUENCY_TEXT_START_Y 14
    #define  FREQUENCY_TEXT_HEIGHT          15
    #define  MMIFM_V_FREQUENCY_TEXT_START_X 0
    #define  MMIFM_V_LEFT_BUTTON_START_X  21
    #define  MMIFM_V_LEFT_BUTTON_START_Y  251
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_X 25
    #define  MMIFM_V_SEARCH_UPPER_BUTTON_START_Y 351
    #define  MMIFM_V_SEARCH_BUTTON_H_SPACE 15
    #define  MMIFM_V_PAUSE_BUTTON_V_SPACE  9
    #define  MMIFM_V_SEARCH_BUTTON_V_SPACE 22
    #define  MMIFM_V_LABEL_WIDTH  204

    #define  MMIFM_H_FREQUENCY_TEXT_START_Y  14 //to do
    #define  MMIFM_H_FREQUENCY_TEXT_START_X  15//to do
    #define  MMIFM_H_LEFT_BUTTON_START_X  76
    #define  MMIFM_H_LEFT_BUTTON_START_Y  138
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_X 80
    #define  MMIFM_H_SEARCH_UPPER_BUTTON_START_Y 217
    #define  MMIFM_H_PAUSE_BUTTON_V_SPACE  9//to do
    #define  MMIFM_H_SEARCH_BUTTON_H_SPACE 40
    #define  MMIFM_H_SEARCH_BUTTON_V_SPACE 5
    #define  MMIFM_H_LABEL_WIDTH  254    
    #define MMIFM_ICON_X                  13  
    #define MMIFM_ICON_Y                   42
    #define MMIFM_DIRECT_BUTTON_WINDTH     65  
    #define MMIFM_DIRECT_BUTTON_HEIGHT     29
    #define MMIFM_ICON_Y                   42
    #define MMIFM_AUTO_UP_Y                194   
    #define MMIFM_AUTO_DOWN_Y              254 
    #define MMIFM_LEFT_Y                   224  
    #define MMIFM_RIGHT_Y                  224
    //#define MMIFM_SPK_X                  73 
    #define MMIFM_SPK_Y                    224 
    #define MMIFM_KPD_Y                    188
    #define MMIFM_ADJ_BG_Y                 43
    #define MMIFM_ADJ_BUT_LEFT_Y           157
    #define MMIFM_ADJ_BUT_RIGHT_Y          157
    #define MMIFM_LEFT_X                    7
    #define MMIFM_ADJ_LEFT_Y               151
    #define MMIFM_ADJ_LEFT_X              4 
    #define MMIFM_ADJ_RIGHT_Y              151
    #define MMIFM_ADJ_RIGHT_X              156
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_X  39
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_X 159
    #define MMIFM_ADJ_LEFT_TEXT_X          39 
    #define MMIFM_ADJ_RIGHT_TEXT_X         159 
    #define MMIFM_ADJ_LEFT_TEXT_Y          175
    #define MMIFM_ADJ_RIGHT_TEXT_Y         175
    #define MMIFM_CHANNEL_TEXT_Y           16  
    #define MMIFM_FREQUCE_TEXT_Y           90
    #define MMIFM_FREQUCE_TEXT_LEFT_X      89
    #define MMIFM_FREQUCE_TEXT_RIGHT_X     239
    #define MMIFM_ADJ_FREQUCE_LEFT_TEXT_Y  175
    #define MMIFM_ADJ_FREQUCE_RIGHT_TEXT_Y 175
    #define MMIFM_ADJ_BUT_V_START_Y        0//the space between stabar and the fre pic
    #define MMIFM_ADJ_BUT_H_START_Y        0//the space between stabar and the fre pic
    //#define MMIFM_VOLUME_LEFT              211
    #define MMIFM_VOLUME_TOP               221
    #define MMIFM_VOLUME_WIDTH             28
    #define MMIFM_VOLUME_HEIGHT            24
    #define MMIFM_VOLUME_BOTTOM            (MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - MMIFM_VOLUME_HEIGHT)
    #define MMIFM_ADJ_BG_X                 0  
    #define MMIFM_ADJ_BUT_LEFT_X           39
	#define MMIFM_ADJ_BUT_RIGHT_X          199 
	#define MMIFM_ADJ_BUT_LEFT_X_H           25
	#define MMIFM_ADJ_BUT_LEFT_Y_H           52
	#define MMIFM_ADJ_BUT_LEFT_X_V           13
	#define MMIFM_ADJ_BUT_LEFT_Y_V           80
	#define MMIFM_FILENAME_FONT                                CAF_FONT_18
	#define MMIFM_LANDSCAPE_BUTTONFORM_HSPACE      30
	#define MMIFM_LANDSCAPE_ANIM_HSPACE                 10
	#define MMIFM_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
	#define MMIFM_TIME_OWNDRAW_HEIGHT                    110
	#define MMIFM_PORTRAIT_BUTTONFORM_HSPACE      60
	#define MMIFM_PORTRAIT_ANIM_HSPACE                 30
	#define MMIFMRECORD_TIME_FONT                                SONG_FONT_52
#endif

#define MMIFM_SPACE                  0
#define MMIFM_AUTO_UP_X            (MMIFM_LEFT_X+ MMIFM_DIRECT_BUTTON_WINDTH) 
#define MMIFM_AUTO_UP_RECT         {MMIFM_AUTO_UP_X,MMIFM_AUTO_UP_Y,MMIFM_AUTO_UP_X+MMIFM_DIRECT_BUTTON_WINDTH,MMIFM_AUTO_UP_Y+MMIFM_DIRECT_BUTTON_HEIGHT}
#define MMIFM_AUTO_DOWN_X            (MMIFM_LEFT_X+ MMIFM_DIRECT_BUTTON_WINDTH)    
#define MMIFM_AUTO_DOWN_RECT        {MMIFM_AUTO_DOWN_X,MMIFM_AUTO_DOWN_Y,MMIFM_AUTO_DOWN_X+MMIFM_DIRECT_BUTTON_WINDTH,MMIFM_AUTO_DOWN_Y+MMIFM_DIRECT_BUTTON_HEIGHT} 
#define MMIFM_AUTO_LEFT_RECT        {MMIFM_LEFT_X,MMIFM_LEFT_Y,MMIFM_LEFT_X+MMIFM_DIRECT_BUTTON_WINDTH, MMIFM_LEFT_Y+MMIFM_DIRECT_BUTTON_HEIGHT}
#define MMIFM_RIGHT_X                (MMIFM_SPK_X + MMIFM_DIRECT_BUTTON_WINDTH)
#define MMIFM_AUTO_RIGHT_RECT        {MMIFM_RIGHT_X,MMIFM_RIGHT_Y,MMIFM_RIGHT_X+MMIFM_DIRECT_BUTTON_WINDTH,MMIFM_RIGHT_Y+MMIFM_DIRECT_BUTTON_HEIGHT} 
#define MMIFM_SPK_X                  (MMIFM_LEFT_X+ MMIFM_DIRECT_BUTTON_WINDTH)
#define MMIFM_SPK_RECT        {MMIFM_SPK_X,MMIFM_SPK_Y,MMIFM_SPK_X+MMIFM_DIRECT_BUTTON_WINDTH,MMIFM_SPK_Y+MMIFM_DIRECT_BUTTON_HEIGHT}
#define MMIFM_KPD_X                  4  
#define MMIFM_ADJ_BUT_WID       (MMIFM_ADJ_BUT_RIGHT_X - MMIFM_ADJ_BUT_LEFT_X) 
#define MMIFM_ADJ_LEFT_RECT        {MMIFM_ADJ_LEFT_X,MMIFM_ADJ_LEFT_Y,MMIFM_ADJ_LEFT_X+20,MMIFM_ADJ_LEFT_Y+35}
#define MMIFM_ADJ_RIGHT_RECT        {MMIFM_ADJ_RIGHT_X,MMIFM_ADJ_RIGHT_Y,MMIFM_ADJ_RIGHT_X+20,MMIFM_ADJ_RIGHT_Y+35} 
#define MMIFM_CHANNEL_TEXT_X           3  
#define MMIFM_ADJ_FREQUCE_TEXT_W       42
#define MMIFM_ADJ_FREQUCE_TEXT_H       11
#define MMIFM_VOLUME_RECT            {MMIFM_VOLUME_LEFT,MMIFM_VOLUME_TOP,MMIFM_VOLUME_LEFT+29,MMIFM_VOLUME_TOP+40}

#define MMIFM_DETAILS_NAME_TOP        MMI_TITLE_HEIGHT
#define MMIFM_DETAILS_NAME_EDIT_TOP  (MMIFM_DETAILS_NAME_TOP + MMI_LIST_ITEM_HEIGHT)
#define MMIFM_DETAILS_FREQ_TOP       (MMIFM_DETAILS_NAME_EDIT_TOP + MMI_LIST_ITEM_HEIGHT)
#define MMIFM_DETAILS_FREQ_EDIT_TOP  (MMIFM_DETAILS_FREQ_TOP + MMI_LIST_ITEM_HEIGHT)
#define MMIFM_DETAILS_NAME_EDIT_RECT  {4, (MMIFM_DETAILS_NAME_EDIT_TOP -1),(MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 9),(MMIFM_DETAILS_NAME_EDIT_TOP + MMI_LIST_ITEM_HEIGHT+1)}
#define MMIFM_DETAILS_FREQ_EDIT_RECT   {4, MMIFM_DETAILS_FREQ_EDIT_TOP -1, MMI_MAINSCREEN_RIGHT_MAX_PIXEL -9, MMIFM_DETAILS_FREQ_EDIT_TOP + MMI_LIST_ITEM_HEIGHT +1}

#define MMIFM_VOLUME_LEFT            (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMIFM_VOLUME_WIDTH)
#define MMIFM_VOLUME_INCREASE_RECT   {MMIFM_VOLUME_LEFT, (MMIFM_VOLUME_TOP - MMIFM_VOLUME_HEIGHT), (MMIFM_VOLUME_LEFT+MMIFM_VOLUME_WIDTH), MMIFM_VOLUME_TOP}
#define MMIFM_VOLUME_DECREASE_RECT   {MMIFM_VOLUME_LEFT,MMIFM_VOLUME_BOTTOM,(MMIFM_VOLUME_LEFT+MMIFM_VOLUME_WIDTH),(MMIFM_VOLUME_BOTTOM+MMIFM_VOLUME_WIDTH)}
#define MMIFM_VOLUME_TOTAL_RECT      {MMIFM_VOLUME_LEFT,(MMIFM_VOLUME_TOP - MMIFM_VOLUME_HEIGHT),(MMIFM_VOLUME_LEFT+MMIFM_VOLUME_WIDTH),(MMIFM_VOLUME_BOTTOM+MMIFM_VOLUME_HEIGHT)}


#endif //_MMIFM_POSITION_H
