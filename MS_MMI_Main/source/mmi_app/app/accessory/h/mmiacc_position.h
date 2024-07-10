/*****************************************************************************
** File Name:      mmiacc_displaysize.h                                               
** Author:           maggie.ren    
** Date:            19/04/2007
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 19/04/2007       maggie.ren          Create                                   *
******************************************************************************/

#ifndef _MMIACC_POSITION_H
#define _MMIACC_POSITION_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmicalendar_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
     //countedtime
	#define CT_BUTTON_KEY_TOP       (MMI_CLIENT_RECT_BOTTOM - CT_BUTTON_KEY_HEIGHT)
	#define CT_BUTTON_KEY_BOTTOM    MMI_CLIENT_RECT_BOTTOM

	#define CT_BUTTON_KEY1_LEFT     0
	#define CT_BUTTON_KEY1_RIGHT    (CT_BUTTON_KEY1_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY2_LEFT     (CT_BUTTON_KEY1_RIGHT+1)
	#define CT_BUTTON_KEY2_RIGHT    (CT_BUTTON_KEY2_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY3_LEFT     (CT_BUTTON_KEY2_RIGHT+1)
	#define CT_BUTTON_KEY3_RIGHT    (CT_BUTTON_KEY3_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY4_LEFT     (CT_BUTTON_KEY3_RIGHT+1)
	#define CT_BUTTON_KEY4_RIGHT    (CT_BUTTON_KEY4_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY5_LEFT     (CT_BUTTON_KEY4_RIGHT+1)
	#define CT_BUTTON_KEY5_RIGHT    (CT_BUTTON_KEY5_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY6_LEFT     (CT_BUTTON_KEY5_RIGHT+1)
	#define CT_BUTTON_KEY6_RIGHT    (CT_BUTTON_KEY6_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY7_LEFT     (CT_BUTTON_KEY6_RIGHT+1)
	#define CT_BUTTON_KEY7_RIGHT    (CT_BUTTON_KEY7_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY8_LEFT     (CT_BUTTON_KEY7_RIGHT+1)
	#define CT_BUTTON_KEY8_RIGHT    (CT_BUTTON_KEY8_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY9_LEFT     (CT_BUTTON_KEY8_RIGHT+1)
	#define CT_BUTTON_KEY9_RIGHT    (CT_BUTTON_KEY9_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY0_LEFT     (CT_BUTTON_KEY9_RIGHT+1)
	#define CT_BUTTON_KEY0_RIGHT    (CT_BUTTON_KEY0_LEFT+CT_BUTTON_KEY_WINDTH)
	#define CT_BUTTON_KEY1_RECT		{CT_BUTTON_KEY1_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY1_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY2_RECT		{CT_BUTTON_KEY2_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY2_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY3_RECT		{CT_BUTTON_KEY3_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY3_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY4_RECT		{CT_BUTTON_KEY4_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY4_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY5_RECT		{CT_BUTTON_KEY5_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY5_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY6_RECT		{CT_BUTTON_KEY6_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY6_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY7_RECT		{CT_BUTTON_KEY7_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY7_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY8_RECT		{CT_BUTTON_KEY8_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY8_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY9_RECT		{CT_BUTTON_KEY9_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY9_RIGHT,CT_BUTTON_KEY_BOTTOM}
	#define CT_BUTTON_KEY0_RECT		{CT_BUTTON_KEY0_LEFT,CT_BUTTON_KEY_TOP,CT_BUTTON_KEY0_RIGHT,CT_BUTTON_KEY_BOTTOM}

#if defined MAINLCD_SIZE_128X160
#ifdef TOUCH_PANEL_SUPPORT
    #define WT_STANDARD_KEY_H_OFFSET    0
#else
    #define WT_STANDARD_KEY_H_OFFSET    5
#endif
    //calcaluator
#ifdef TOUCH_PANEL_SUPPORT   
    #define MMICALC_TEXT_FONT           SONG_FONT_14        
    #define MMICALC_TEXT_FONT_H           SONG_FONT_14    
    #define CALC_LINE_HIGHT             15	
    #define CALC_LINE_HIGHT_H            15    
    #define BUTTON_HORIZONAL_SPACE	 0
    #define BUTTON_VERTICAL_SPACE		0    
    #define BUTTON_HORIZONAL_SPACE_H	 1
    #define BUTTON_VERTICAL_SPACE_H		1    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 2    
    #define CALC_LINE_RIGHT_BORDER      4     
    #define CALC_LINE_RIGHT_BORDER_H      4  
#else
    #define MMICALC_TEXT_FONT           SONG_FONT_16        
    #define MMICALC_TEXT_FONT_H           SONG_FONT_14    
    #define CALC_LINE_HIGHT             17 //45	
    #define CALC_LINE_HIGHT_H           12 //38    
    #define BUTTON_HORIZONAL_SPACE	 2 //10
    #define BUTTON_VERTICAL_SPACE		2 // 4    
    #define BUTTON_HORIZONAL_SPACE_H	 1
    #define BUTTON_VERTICAL_SPACE_H		1    
    #define FORM_MARGIN   1 //2
    #define FORM_MARGIN_H 0    
    #define CALC_LINE_RIGHT_BORDER      0 //4     
    #define CALC_LINE_RIGHT_BORDER_H    6 
#endif
    //michael wang for bugzilla 2502
    #define MMICACL_OPERATOR_OFFSET_TOP     2
    #define MMICACL_OPERATOR_OFFSET_BOTTOM  0
    #define MMICACL_OPERATOR_OFFSET_SPACE   2

	// êà??μ?í???óò
	#define WT_WORLD_MAP_REGION_LEFT          5//4
	#define WT_WORLD_MAP_REGION_TOP           26//44
	#define WT_WORLD_MAP_REGION_RIGHT         172//235
	#define WT_WORLD_MAP_REGION_BOTTOM        103//167

	// ò??ˉ?á????oí2?êy
	#define WT_MOVE_AXES_WIDTH                5
	#define WT_MOVE_AXES_HIGHT                77// 124
	#define WT_MOVE_AXES_START_Y              26//44

	// °′?ü±3?°????2?êy
	#define WT_KEYSTOKE_BG_LEFT               5// 4
	#define WT_KEYSTOKE_BG_TOP                103// 258
	#define WT_KEYSTOKE_BG_WIDTH              172// 232
	#define WT_KEYSTOKE_BG_HIGHT              125// 23
	//icon
	#define WT_ICON_WIDTH                       5
    #define WT_DROPLIST_SPACE                   8    
	#define WT_ICON_HIGHT                       11
	#define WT_ICON_LEFT                        9
	#define WT_UP_ICON_TOP                      (129-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_ICON_TOP                    (165-WT_STANDARD_KEY_H_OFFSET)

	#define WT_TEXT_WIDTH                       125
	#define WT_TEXT_HIGHT                       21
	#define WT_TEXT_LEFT                        18
	#define WT_UP_TEXT_TOP                      (124-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_TEXT_TOP                    (161-WT_STANDARD_KEY_H_OFFSET)
	#define WT_UP_WORLDDIFF_TOP                 (WT_UP_TEXT_TOP+4) 
	#define WT_DOWN_WORLDDIFF_TOP               (WT_DOWN_TEXT_TOP+4)   
    #define WT_UP_WORLDTIME_TOP                 (WT_UP_TEXT_TOP + WT_TEXT_HIGHT + 2) 
	#define WT_DOWN_WORLDTIME_TOP               (WT_DOWN_TEXT_TOP + WT_TEXT_HIGHT + 2)    

    #define WT_WORLDTIME_LEFT                   10 
	#define WT_WORLDTIME_RIGHT                  140   
		 
	#define WT_KEYSTOKE_LEFT_ARROW_LEFT       WT_KEYSTOKE_BG_LEFT
	#define WT_KEYSTOKE_LEFT_ARROW_TOP        WT_KEYSTOKE_BG_TOP
	#define WT_KEYSTOKE_RIGHT_ARROW_LEFT      88// 120
	#define WT_KEYSTOKE_RIGHT_ARROW_TOP       WT_KEYSTOKE_BG_TOP

	#define WT_KEYSTOKE_ARROW_WIGTH           83// 117
	#define WT_KEYSTOKE_ARROW_HIGHT           21// 23
	//two line distance hight
	#define WT_TWOLINE_DISTANCE_HIGHT           MMI_DEFAULT_SMALL_FONT
	
	// ??±?±3?°????2?êy
	#define WT_TEXT_BG_LEFT                   6// 8
	#define WT_TEXT_BG_TOP1                   (126-WT_STANDARD_KEY_H_OFFSET)// 171
	#define WT_TEXT_BG_TOP2                   (126-WT_STANDARD_KEY_H_OFFSET)// 215
	#define WT_TEXT_BG_WIDTH                  137// 176
	#define WT_TEXT_BG_HIGHT                  20// 25

	// ??à-2?μ￥?ü????2?êy
	#define WT_DROPDOWN_KEY_LEFT              120// 160
	#define WT_DROPDOWN_KEY_TOP1              (105-WT_STANDARD_KEY_H_OFFSET)// 174
	#define WT_DROPDOWN_KEY_TOP2              (141-WT_STANDARD_KEY_H_OFFSET)// 219
	#define WT_DROPDOWN_KEY_WIDTH             20// 20
	#define WT_DROPDOWN_KEY_HIGHT             19// 19

	#define WT_TIME_DIFF_LEFT                 144//192
	#define WT_TIME_DIFF_RIGHT                168//225

	//counted time
#ifdef MULTI_LAN_SUPPORT_DE
	#define MMICT_TEXT_FONT         SONG_FONT_24//SONG_FONT_30
#else
	#define MMICT_TEXT_FONT         SONG_FONT_30
#endif
    #define MMICT_TITLE_FONT         SONG_FONT_20
    #define CT_LABEL_MARGIN  5  //10
	#define CT_EDIT_MARGIN  15 // 15 定义边框
	#define CT_BUTTON_MARGIN  4 //定义边框
	#define CT_BUTTON_HEIGHT_V  20  //20
	#define CT_BUTTON_HEIGHT_H  10  //10
	#define CT_PICKER_HEIGHT  40   //30
	#define CT_PICKER_ACTIVE_FONT  SONG_FONT_29
	#define CT_PICKER_NORMAL_FONT  SONG_FONT_24

	//stop watch
	#define MMIST_TIME1_OFFSET_Y_V  7//18
	#define MMIST_TIME1_OFFSET_Y_H  3
	#define MMIST_TIME2_OFFSET_Y_V  7//28
	#define MMIST_TIME2_OFFSET_Y_H  3
	#define ST_TIMER1_HEIGHT_V 12
	#define ST_TIMER1_HEIGHT_H 10
	#define ST_TIMER2_HEIGHT_V 30
	#define ST_TIMER2_HEIGHT_H 26
	#define ST_BUTTON_HEIGHT_V  40
	#define ST_BUTTON_HEIGHT_H  32
	#define ST_BUTTON_OFFSET_X_V  9
	#define ST_BUTTON_OFFSET_X_H  18
	#define ST_TIMER1_STR_OFFSET_X_V  (ST_BUTTON_OFFSET_X_V + 6)
	#define ST_TIMER1_STR_OFFSET_X_H  (ST_BUTTON_OFFSET_X_H + 6)
	#define ST_BUTTON_MARGIN  8

	//calculator begin
    #define CAL_BUTTON_KEY1_RECT 		{0,122,58,139}
	#define CAL_BUTTON_KEY2_RECT 		{59,122,117,139}
	#define CAL_BUTTON_KEY3_RECT 		{118,122,175,139}
	#define CAL_BUTTON_KEY4_RECT 		{0,141,58,158}
	#define CAL_BUTTON_KEY5_RECT 		{59,141,117,158}
	#define CAL_BUTTON_KEY6_RECT 		{118,141,175,158}	
    #define CAL_BUTTON_KEY7_RECT 		{0,160,58,177}
	#define CAL_BUTTON_KEY8_RECT 		{59,160,117,177}
	#define CAL_BUTTON_KEY9_RECT 		{118,160,175,177}
	
	#define CAL_BUTTON_KEYSTAR_RECT 	{0,178,58,195}
	#define CAL_BUTTON_KEY0_RECT 		{59,178,117,195}
	#define CAL_BUTTON_KEYHASH_RECT 	{118,178,175,195}
	#define CAL_BUTTON_KEYPLUS_RECT 			{59,64,117,82}
	#define CAL_BUTTON_KEYMINUS_RECT 			{59,102,117,120}
	#define CAL_BUTTON_KEYMUL_RECT 			{0,83,58,101}
	#define CAL_BUTTON_KEYDIV_RECT 			{118,83,175,101}
	#define CAL_BUTTON_KEYEQUAL_RECT 			{59,83,117,101}
	#define CAL_BUTTON_KEYCLEAR_RECT 	{0,64,58,82}
	#define CAL_BUTTON_KEYBACKSPACE_RECT 	{118,64,175,82}

	//calendar begin
	#define CALENDAR_BLOCK_WIDTH_V                      18 //18
	#define CALENDAR_BLOCK_HEIGHT_V                     15 //14
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_V             17 // weekly 里面块的高度
    #define CALENDAR_WEEKLY_DATE_Y_V     			    19 // weekly 里面界面的顶端
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V		    {15, 3, 115, CALENDAR_WEEKLY_DATE_Y_V-1} //weekly 标题的显示位置
	#define CALENDAR_YEAR_TITLE_RECT_V 	                {10, 5, 45, 20} //junxue zhu
    #define CALENDAR_MONTH_TITLE_RECT_V 	            {5, 2, 120, 19	}  //2012-4显示区域
    #define CALENDAR_MONTH_LINE0_TOP_V                  35//103
    #define CALENDAR_MONTH_ITEM0_LEFT_V                 1
    #define CALENDAR_SCH_TEXT_OFFSET_V                  2//10
    #define CALENDAR_POINT_Y_V                          385
    #define CALENDAR_LUNAR_TITLE_RECT_V 	            {1, 123, 126, 138} //junxue zhu
    #define CALENDAR_DAILY_START_Y_OFFSET_V             19
    #define CALENDAR_DAILY_TITLE_RECT_V 	            {20, 2, 100, CALENDAR_DAILY_START_Y_OFFSET_V-1} //45,21,185,
    #define CALENDAR_DAILY_ITEM_HEIGH_V                 17
    #define CALENDAR_DAILY_MAX_ITEM_NUM_V               7
    #define CALENDAR_DAILY_TIME_RIGHT_V                 40 //57
    #define CALENDAR_MONTH_WEEKLY_Y_V    				20     //星期显示的高度
	#define CALENDAR_WEEKLY_TITLE_SPACE_V    		    2
    
	#define CALENDAR_BLOCK_WIDTH_H                      22 //42
	#define CALENDAR_BLOCK_HEIGHT_H                     13 //24
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_H   	        13 
    #define CALENDAR_WEEKLY_DATE_Y_H    				14
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H			{5, 0, 155, CALENDAR_WEEKLY_DATE_Y_H-1}
	#define CALENDAR_YEAR_TITLE_RECT_H				    {47, 25, 87, 51}
	#define CALENDAR_MONTH_TITLE_RECT_H 				{20, 2, 140, 15}
    #define CALENDAR_MONTH_LINE0_TOP_H                  30
    #define CALENDAR_MONTH_ITEM0_LEFT_H                 2
	#define CALENDAR_SCH_TEXT_OFFSET_H                  3
    #define CALENDAR_POINT_Y_H                          230
    #define CALENDAR_LUNAR_TITLE_RECT_H 	            {90, 23, 399, 49}
    #define CALENDAR_DAILY_START_Y_OFFSET_H             16
    #define CALENDAR_DAILY_TITLE_RECT_H 	            {40, 2, 120, CALENDAR_DAILY_START_Y_OFFSET_V-1}
    #define CALENDAR_DAILY_ITEM_HEIGH_H                 15
    #define CALENDAR_DAILY_MAX_ITEM_NUM_H               6
    #define CALENDAR_DAILY_TIME_RIGHT_H                 52
    #define CALENDAR_MONTH_WEEKLY_Y_H    				16
    #define CALENDAR_WEEKLY_TITLE_SPACE_H    		    5

    #define CALENDAR_SCH_TEXT_FONT                      SONG_FONT_14
    #define CALENDAR_MONTH_FONT                         SONG_FONT_15  //18
    #define CALENDAR_YEAR_FONT                          SONG_FONT_10  //15
    #define CALENDAR_DATE_FONT                          SONG_FONT_12
    #define CALENDAR_WEEKLY_TITLE_FONT                  SONG_FONT_12  //17
    #define CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H       (CALENDAR_BLOCK_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V             50
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H             40
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V     (CALENDAR_WEEKLY_SELECT_HEIGHT_V-2)
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H     (CALENDAR_WEEKLY_SELECT_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V      47
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H      40
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_H          (CALENDAR_DAILY_ITEM_HEIGH_H-4)
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_V          (CALENDAR_DAILY_ITEM_HEIGH_V-2)   
#ifdef  MMI_UNITCONVERSION_SUPPORT
    #define MMIUC_FONT_SIZE  CAF_FONT_13
#ifdef MAINLCD_DEV_SIZE_128X160 
    #define MMIUC_OWNDRAW_HEIGHT  25
    #define MMIUC_EDIT_FORM_HEIGHT  25
    #define MMIUC_FORM_VSAPCE  2 
#else
    #define MMIUC_OWNDRAW_HEIGHT  19 
    #define MMIUC_EDIT_FORM_HEIGHT  22
    #define MMIUC_FORM_VSAPCE  1
#endif            
    #define MMIUC_EDIT_MARGIN 2       
    #define MMIUC_FORM_HSAPCE  2    
    #define MMIUC_FORM_MARGIN  2    
    #define MMIUC_PICKER_HEIGHT_V  54
    #define MMIUC_PICKER_HEIGHT_H  40
    #define MMIUC_LABEL_WIDTH_PERCENT  33
    #define MMIUC_OWNDRAW_TITLE_MARGIN  10    //自绘控件中文字前的空白区大小，in 176X220 and 128X160
#endif 	
	
#elif defined MAINLCD_SIZE_176X220
#ifdef TOUCH_PANEL_SUPPORT
    #define WT_STANDARD_KEY_H_OFFSET    0
#else
    #define WT_STANDARD_KEY_H_OFFSET    5
#endif
    //calcaluator
#ifdef TOUCH_PANEL_SUPPORT   
    #define MMICALC_TEXT_FONT           SONG_FONT_16        
    #define MMICALC_TEXT_FONT_H           SONG_FONT_16    
    #define CALC_LINE_HIGHT             20	
    #define CALC_LINE_HIGHT_H           18    
    #define BUTTON_HORIZONAL_SPACE	    10
    #define BUTTON_VERTICAL_SPACE		10   
    #define BUTTON_HORIZONAL_SPACE_H	 1
    #define BUTTON_VERTICAL_SPACE_H		 1 //3    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 2    
    #define CALC_LINE_RIGHT_BORDER      4     
    #define CALC_LINE_RIGHT_BORDER_H      4  
#else
    #define MMICALC_TEXT_FONT           SONG_FONT_14 //16        
    #define MMICALC_TEXT_FONT_H           SONG_FONT_16    
    #define CALC_LINE_HIGHT             20 //60 //15	
    #define CALC_LINE_HIGHT_H           18 //45    
    #define BUTTON_HORIZONAL_SPACE	 10
    #define BUTTON_VERTICAL_SPACE		10    
    #define BUTTON_HORIZONAL_SPACE_H	 1
    #define BUTTON_VERTICAL_SPACE_H		 1 //3    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 2//4    
    #define CALC_LINE_RIGHT_BORDER      4     
    #define CALC_LINE_RIGHT_BORDER_H      4  
#endif

    //michael wang for bugzilla 2502
    #define MMICACL_OPERATOR_OFFSET_TOP     10//20
    #define MMICACL_OPERATOR_OFFSET_BOTTOM    0
    #define MMICACL_OPERATOR_OFFSET_SPACE   2
	// êà??μ?í???óò
	#define WT_WORLD_MAP_REGION_LEFT          5//4
	#define WT_WORLD_MAP_REGION_TOP           26//44
	#define WT_WORLD_MAP_REGION_RIGHT         172//235
	#define WT_WORLD_MAP_REGION_BOTTOM        103//167

	// ò??ˉ?á????oí2?êy
	#define WT_MOVE_AXES_WIDTH                5
	#define WT_MOVE_AXES_HIGHT                77// 124
	#define WT_MOVE_AXES_START_Y              26//44

	// °′?ü±3?°????2?êy
	#define WT_KEYSTOKE_BG_LEFT               5// 4
	#define WT_KEYSTOKE_BG_TOP                103// 258
	#define WT_KEYSTOKE_BG_WIDTH              172// 232
	#define WT_KEYSTOKE_BG_HIGHT              125// 23
	//icon
	#define WT_ICON_WIDTH                       5
    #define WT_DROPLIST_SPACE                   8    
	#define WT_ICON_HIGHT                       11
	#define WT_ICON_LEFT                        9
	#define WT_UP_ICON_TOP                      (129-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_ICON_TOP                    (165-WT_STANDARD_KEY_H_OFFSET)

	#define WT_TEXT_WIDTH                       125
	#define WT_TEXT_HIGHT                       21
	#define WT_TEXT_LEFT                        18
	#define WT_UP_TEXT_TOP                      (124-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_TEXT_TOP                    (161-WT_STANDARD_KEY_H_OFFSET)
	#define WT_UP_WORLDDIFF_TOP                 (WT_UP_TEXT_TOP+4) 
	#define WT_DOWN_WORLDDIFF_TOP               (WT_DOWN_TEXT_TOP+4)   
    #define WT_UP_WORLDTIME_TOP                 (WT_UP_TEXT_TOP + WT_TEXT_HIGHT + 2) 
	#define WT_DOWN_WORLDTIME_TOP               (WT_DOWN_TEXT_TOP + WT_TEXT_HIGHT + 2)    

    #define WT_WORLDTIME_LEFT                   10 
	#define WT_WORLDTIME_RIGHT                  140   
		 
	#define WT_KEYSTOKE_LEFT_ARROW_LEFT       WT_KEYSTOKE_BG_LEFT
	#define WT_KEYSTOKE_LEFT_ARROW_TOP        WT_KEYSTOKE_BG_TOP
	#define WT_KEYSTOKE_RIGHT_ARROW_LEFT      88// 120
	#define WT_KEYSTOKE_RIGHT_ARROW_TOP       WT_KEYSTOKE_BG_TOP

	#define WT_KEYSTOKE_ARROW_WIGTH           83// 117
	#define WT_KEYSTOKE_ARROW_HIGHT           21// 23
	//two line distance hight
	#define WT_TWOLINE_DISTANCE_HIGHT           MMI_DEFAULT_SMALL_FONT
	
	// ??±?±3?°????2?êy
	#define WT_TEXT_BG_LEFT                   6// 8
	#define WT_TEXT_BG_TOP1                   (126-WT_STANDARD_KEY_H_OFFSET)// 171
	#define WT_TEXT_BG_TOP2                   (126-WT_STANDARD_KEY_H_OFFSET)// 215
	#define WT_TEXT_BG_WIDTH                  137// 176
	#define WT_TEXT_BG_HIGHT                  20// 25

	// ??à-2?μ￥?ü????2?êy
	#define WT_DROPDOWN_KEY_LEFT              120// 160
	#define WT_DROPDOWN_KEY_TOP1              (105-WT_STANDARD_KEY_H_OFFSET)// 174
	#define WT_DROPDOWN_KEY_TOP2              (141-WT_STANDARD_KEY_H_OFFSET)// 219
	#define WT_DROPDOWN_KEY_WIDTH             20// 20
	#define WT_DROPDOWN_KEY_HIGHT             19// 19

	#define WT_TIME_DIFF_LEFT                 144//192
	#define WT_TIME_DIFF_RIGHT                168//225

	//counted time
#ifdef MULTI_LAN_SUPPORT_DE
	#define MMICT_TEXT_FONT         SONG_FONT_24//SONG_FONT_30
#else
	#define MMICT_TEXT_FONT         SONG_FONT_30
#endif
    #define MMICT_TITLE_FONT         SONG_FONT_24
#ifdef TOUCH_PANEL_SUPPORT
    #define CT_LABEL_MARGIN  0 //10
	#define CT_EDIT_MARGIN  5 // 30 定义边框
	#define CT_BUTTON_MARGIN  4 //4定义边框
	#define CT_BUTTON_HEIGHT_V  30  //30
	#define CT_BUTTON_HEIGHT_H  30  //10
	#define CT_PICKER_HEIGHT  30  //40
#else
    #define CT_LABEL_MARGIN  10
    #define CT_EDIT_MARGIN  30 //定义边框
    #define CT_BUTTON_MARGIN  4 //定义边框
    #define CT_BUTTON_HEIGHT_V  30  //30
    #define CT_BUTTON_HEIGHT_H  10
	#define CT_PICKER_HEIGHT  40
#endif
	#define CT_PICKER_ACTIVE_FONT  SONG_FONT_29
	#define CT_PICKER_NORMAL_FONT  SONG_FONT_24

	//stop watch
	#define MMIST_TIME1_OFFSET_Y_V  15
	#define MMIST_TIME1_OFFSET_Y_H  10
	#define MMIST_TIME2_OFFSET_Y_V  10 //28
	#define MMIST_TIME2_OFFSET_Y_H  10
	#define ST_TIMER1_HEIGHT_V 15
	#define ST_TIMER1_HEIGHT_H 14
	#define ST_TIMER2_HEIGHT_V 30
	#define ST_TIMER2_HEIGHT_H 30
	#define ST_BUTTON_HEIGHT_V  30
	#define ST_BUTTON_HEIGHT_H  25
	#define ST_BUTTON_OFFSET_X_V  9
	#define ST_BUTTON_OFFSET_X_H  18
	#define ST_BUTTON_MARGIN  8
	#define ST_TIMER1_STR_OFFSET_X_V  (ST_BUTTON_OFFSET_X_V + 6)
	#define ST_TIMER1_STR_OFFSET_X_H  (ST_BUTTON_OFFSET_X_H + 6)
	//calculator begin
    #define CAL_BUTTON_KEY1_RECT 		{0,122,58,139}
	#define CAL_BUTTON_KEY2_RECT 		{59,122,117,139}
	#define CAL_BUTTON_KEY3_RECT 		{118,122,175,139}
	#define CAL_BUTTON_KEY4_RECT 		{0,141,58,158}
	#define CAL_BUTTON_KEY5_RECT 		{59,141,117,158}
	#define CAL_BUTTON_KEY6_RECT 		{118,141,175,158}	
    #define CAL_BUTTON_KEY7_RECT 		{0,160,58,177}
	#define CAL_BUTTON_KEY8_RECT 		{59,160,117,177}
	#define CAL_BUTTON_KEY9_RECT 		{118,160,175,177}
	
	#define CAL_BUTTON_KEYSTAR_RECT 	{0,178,58,195}
	#define CAL_BUTTON_KEY0_RECT 		{59,178,117,195}
	#define CAL_BUTTON_KEYHASH_RECT 	{118,178,175,195}
	#define CAL_BUTTON_KEYPLUS_RECT 			{59,64,117,82}
	#define CAL_BUTTON_KEYMINUS_RECT 			{59,102,117,120}
	#define CAL_BUTTON_KEYMUL_RECT 			{0,83,58,101}
	#define CAL_BUTTON_KEYDIV_RECT 			{118,83,175,101}
	#define CAL_BUTTON_KEYEQUAL_RECT 			{59,83,117,101}
	#define CAL_BUTTON_KEYCLEAR_RECT 	{0,64,58,82}
	#define CAL_BUTTON_KEYBACKSPACE_RECT 	{118,64,175,82}

	//calendar begin
	#define CALENDAR_BLOCK_WIDTH_V                      22 
	#define CALENDAR_BLOCK_HEIGHT_V                     20
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_V             24
    #define CALENDAR_WEEKLY_DATE_Y_V     			    25 
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V		    {26, 7, 149, CALENDAR_WEEKLY_DATE_Y_V-1}
	#define CALENDAR_YEAR_TITLE_RECT_V 	                {45, 25, 239, 43}
    #define CALENDAR_MONTH_TITLE_RECT_V 	            {11, 9, 164, 26}
    #define CALENDAR_MONTH_LINE0_TOP_V                  56
    #define CALENDAR_MONTH_ITEM0_LEFT_V                 11
    #define CALENDAR_SCH_TEXT_OFFSET_V                  10
    #define CALENDAR_POINT_Y_V                          385
    #define CALENDAR_LUNAR_TITLE_RECT_V 	            {11, 180, 164, 194}
    #define CALENDAR_DAILY_START_Y_OFFSET_V             25
    #define CALENDAR_DAILY_TITLE_RECT_V 	            {40, 5, 136, CALENDAR_DAILY_START_Y_OFFSET_V-1}
    #define CALENDAR_DAILY_ITEM_HEIGH_V                 24
    #define CALENDAR_DAILY_MAX_ITEM_NUM_V               7
    #define CALENDAR_DAILY_TIME_RIGHT_V                 57
    #define CALENDAR_MONTH_WEEKLY_Y_V    				28
    
	#define CALENDAR_BLOCK_WIDTH_H                      28
	#define CALENDAR_BLOCK_HEIGHT_H                     16 //18
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_H   	        18 
    #define CALENDAR_WEEKLY_DATE_Y_H    				24
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H			{27,3,196,CALENDAR_WEEKLY_DATE_Y_H-1}
	#define CALENDAR_YEAR_TITLE_RECT_H				    {47, 25, 87, 51}
	#define CALENDAR_MONTH_TITLE_RECT_H 				{12, 2, 208, 17}  //{12, 5, 208, 20}
    #define CALENDAR_MONTH_LINE0_TOP_H                  38
    #define CALENDAR_MONTH_ITEM0_LEFT_H                 12
	#define CALENDAR_SCH_TEXT_OFFSET_H                  8
    #define CALENDAR_POINT_Y_H                          230
    #define CALENDAR_LUNAR_TITLE_RECT_H 	            {30, 135, 190, 151}
    #define CALENDAR_DAILY_START_Y_OFFSET_H             20
    #define CALENDAR_DAILY_TITLE_RECT_H 	            {50, 3, 170, CALENDAR_DAILY_START_Y_OFFSET_H-1}
    #define CALENDAR_DAILY_ITEM_HEIGH_H                 18
    #define CALENDAR_DAILY_MAX_ITEM_NUM_H               7
    #define CALENDAR_DAILY_TIME_RIGHT_H                 57
    #define CALENDAR_MONTH_WEEKLY_Y_H    				20

    #define CALENDAR_SCH_TEXT_FONT                      SONG_FONT_15
    #define CALENDAR_MONTH_FONT                         SONG_FONT_18 //18
    #define CALENDAR_YEAR_FONT                          SONG_FONT_15
    #define CALENDAR_DATE_FONT                          SONG_FONT_16
    #define CALENDAR_WEEKLY_TITLE_FONT                  SONG_FONT_16
    #define CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H       (CALENDAR_BLOCK_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V             50
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H             40
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V     (CALENDAR_WEEKLY_SELECT_HEIGHT_V-2)
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H     (CALENDAR_WEEKLY_SELECT_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V      47
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H      40
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_H          (CALENDAR_DAILY_ITEM_HEIGH_H-4)
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_V          (CALENDAR_DAILY_ITEM_HEIGH_V-2)   
#ifdef  MMI_UNITCONVERSION_SUPPORT
    #define MMIUC_FONT_SIZE  SONG_FONT_20
    #define MMIUC_OWNDRAW_HEIGHT  28    
    #define MMIUC_EDIT_FORM_HEIGHT  30      
    #define MMIUC_EDIT_MARGIN 2       
    #define MMIUC_FORM_VSAPCE  4
    #define MMIUC_FORM_HSAPCE  4    
    #define MMIUC_FORM_MARGIN  4    
    #define MMIUC_PICKER_HEIGHT_V  54
    #define MMIUC_PICKER_HEIGHT_H  40
    #define MMIUC_LABEL_WIDTH_PERCENT  35
    #define MMIUC_OWNDRAW_TITLE_MARGIN  3    //自绘控件中文字前的空白区大小，in 176X220 and 128X160
#endif    
    //è?3ì°′??2é?′????,?à1?×?±ê,end  
#elif defined MAINLCD_SIZE_240X320

#ifdef MMI_PDA_SUPPORT
#define CALENDAR_WEEK_TEXT_FONT   SONG_FONT_16
#else
#define ACC_CALC_BL_IMAGE_RECT		{CALC_LINE_START_X, 5, MMICALC_TEXT_LEFT, 67}
#define ACC_CALC_BL_DISP_RECT	    {CALC_LINE_START_X, CALC_LINE_START_Y, CALC_LINE_END_X,CALC_LINE_END_Y}
#endif
    //calcaluator
#ifdef TOUCH_PANEL_SUPPORT    
#ifdef MULTI_LAN_SUPPORT_DE
     #define MMICALC_TEXT_FONT           SONG_FONT_32     //36
     #define MMICALC_TEXT_FONT_H           SONG_FONT_32   //34
#else
     #define MMICALC_TEXT_FONT           SONG_FONT_36
     #define MMICALC_TEXT_FONT_H           SONG_FONT_26  //34
#endif
    #define CALC_LINE_HIGHT             68  
    #define CALC_LINE_HIGHT_H            58    
    #define BUTTON_HORIZONAL_SPACE   4
    #define BUTTON_VERTICAL_SPACE       4    
    #define BUTTON_HORIZONAL_SPACE_H     4
    #define BUTTON_VERTICAL_SPACE_H     4    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 4    
    #define CALC_LINE_RIGHT_BORDER      18     
    #define CALC_LINE_RIGHT_BORDER_H      20
#else
#ifdef MULTI_LAN_SUPPORT_DE
    #define MMICALC_TEXT_FONT           SONG_FONT_32     //36
    #define MMICALC_TEXT_FONT_H           SONG_FONT_32   //34
#else
    #define MMICALC_TEXT_FONT           SONG_FONT_28 //26    //36
    #define MMICALC_TEXT_FONT_H           SONG_FONT_26  //34
#endif
    #define CALC_LINE_HIGHT             58//68  
    #define CALC_LINE_HIGHT_H            58    
    #define BUTTON_HORIZONAL_SPACE   4
    #define BUTTON_VERTICAL_SPACE       4    
    #define BUTTON_HORIZONAL_SPACE_H     3
    #define BUTTON_VERTICAL_SPACE_H     5    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 4    
    #define CALC_LINE_RIGHT_BORDER      14     
    #define CALC_LINE_RIGHT_BORDER_H      14
#endif
	//yanyan.an for Bug 163320 
	#define MMICACL_OPERATOR_OFFSET_TOP     10

#ifdef TOUCH_PANEL_SUPPORT
    #define WT_STANDARD_KEY_H_OFFSET    0
#else
    #define WT_STANDARD_KEY_H_OFFSET    5
#endif
	// êà??μ?í???óò
	#define WT_WORLD_MAP_REGION_LEFT          0
	#define WT_WORLD_MAP_REGION_TOP           28
	#define WT_WORLD_MAP_REGION_RIGHT         235
	#define WT_WORLD_MAP_REGION_BOTTOM        167

	// ò??ˉ?á????oí2?êy
	#define WT_MOVE_AXES_WIDTH                5
	#define WT_MOVE_AXES_HIGHT                124
	#define WT_MOVE_AXES_START_Y              MMI_TITLE_HEIGHT

	// °′?ü±3?°????2?êy
	#define WT_KEYSTOKE_BG_LEFT               0
	#define WT_KEYSTOKE_BG_TOP                168//258
	#define WT_KEYSTOKE_BG_WIDTH              240
	#define WT_KEYSTOKE_BG_HIGHT              23
	// ICON
	#define WT_ICON_WIDTH                       5
    #define WT_DROPLIST_SPACE                   9
	#define WT_ICON_HIGHT                       11
	#define WT_ICON_LEFT                        9
    #define WT_UP_ICON_TOP                      (200-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_ICON_TOP                    (245-WT_STANDARD_KEY_H_OFFSET)
	// TEXT
	#define WT_TEXT_WIDTH                       173
	#define WT_TEXT_HIGHT                       25
	#define WT_TEXT_LEFT                        18
	#define WT_UP_TEXT_TOP                      (193-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_TEXT_TOP                    (238-WT_STANDARD_KEY_H_OFFSET)
	#define WT_UP_WORLDDIFF_TOP                 (WT_UP_TEXT_TOP+4) 
	#define WT_DOWN_WORLDDIFF_TOP               (WT_DOWN_TEXT_TOP+4)   
    #define WT_UP_WORLDTIME_TOP                 (WT_UP_TEXT_TOP + WT_TEXT_HIGHT + 2) 
	#define WT_DOWN_WORLDTIME_TOP               (WT_DOWN_TEXT_TOP + WT_TEXT_HIGHT + 2)    

    #define WT_WORLDTIME_LEFT                   15 
	#define WT_WORLDTIME_RIGHT                  175   
	
	#define WT_KEYSTOKE_LEFT_ARROW_LEFT       WT_KEYSTOKE_BG_LEFT
	#define WT_KEYSTOKE_LEFT_ARROW_TOP        WT_KEYSTOKE_BG_TOP
	#define WT_KEYSTOKE_RIGHT_ARROW_LEFT      120
	#define WT_KEYSTOKE_RIGHT_ARROW_TOP       WT_KEYSTOKE_BG_TOP

	#define WT_KEYSTOKE_ARROW_WIGTH           120
	#define WT_KEYSTOKE_ARROW_HIGHT           23
	//two line distance hight
	#define WT_TWOLINE_DISTANCE_HIGHT           MMI_DEFAULT_SMALL_FONT
	// ??±?±3?°????2?êy
	#define WT_TEXT_BG_LEFT                   8
	#define WT_TEXT_BG_TOP1                   (171-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_TOP2                   (193-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_WIDTH                  184
	#define WT_TEXT_BG_HIGHT                  25//17//@zhaohui,cr108515

	// ??à-2?μ￥?ü????2?êy
	#define WT_DROPDOWN_KEY_LEFT              160
	#define WT_DROPDOWN_KEY_TOP1              (174-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_TOP2              (219-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_WIDTH             20
	#define WT_DROPDOWN_KEY_HIGHT             19

	#define WT_TIME_DIFF_LEFT                 185
	#define WT_TIME_DIFF_RIGHT                225

	//counted time
	#define MMICT_TEXT_FONT	    SONG_FONT_26  //    SONG_FONT_30,fixed for bug609489
	#define CT_EDIT_MARGIN  8 //定义边框
	#define CT_BUTTON_MARGIN  4 //定义边框
	#define CT_BUTTON_HEIGHT_V  40
	#define CT_BUTTON_HEIGHT_H  32
	#define CT_PICKER_HEIGHT  40
	#define CT_PICKER_ACTIVE_FONT  SONG_FONT_29
	#define CT_PICKER_NORMAL_FONT  SONG_FONT_24

	//stop watch
	#define MMIST_TIME1_OFFSET_Y_V	18
	#define MMIST_TIME1_OFFSET_Y_H	10
	#define MMIST_TIME2_OFFSET_Y_V	28
	#define MMIST_TIME2_OFFSET_Y_H	10
	#define ST_TIMER1_HEIGHT_V 15
	#define ST_TIMER1_HEIGHT_H 14
	#define ST_TIMER2_HEIGHT_V 44
	#define ST_TIMER2_HEIGHT_H 31
	#define ST_BUTTON_HEIGHT_V  40
	#define ST_BUTTON_HEIGHT_H  32
	#define ST_BUTTON_OFFSET_X_V  9
	#define ST_BUTTON_OFFSET_X_H  18
	#define ST_TIMER1_STR_OFFSET_X_V  (ST_BUTTON_OFFSET_X_V + 6)
	#define ST_TIMER1_STR_OFFSET_X_H  (ST_BUTTON_OFFSET_X_H + 6)
	#define ST_BUTTON_MARGIN  8

#ifdef NON_STANDARD_KEY_LAYOUT_SUPPORT
    #define CAL_BUTTON_KEY1_RECT 		{0,181, 59, 215}
	#define CAL_BUTTON_KEY2_RECT 		{60,181, 119, 215}
	#define CAL_BUTTON_KEY3_RECT 		{120,181, 179, 215}
	#define CAL_BUTTON_KEY4_RECT 		{0,217, 59, 251}
	#define CAL_BUTTON_KEY5_RECT 		{60,217, 119, 251}
	#define CAL_BUTTON_KEY6_RECT 		{120,217, 179, 251}
    #define CAL_BUTTON_KEY7_RECT 		{0,253, 59, 288}
	#define CAL_BUTTON_KEY8_RECT 		{60,253, 119, 288}
	#define CAL_BUTTON_KEY9_RECT 		{120,253, 179, 288}	
	#define CAL_BUTTON_KEYSTAR_RECT 	{180,181, 239, 215}
	#define CAL_BUTTON_KEY0_RECT 		{180,217, 239, 251}
	#define CAL_BUTTON_KEYHASH_RECT 		{180,253, 239, 288}

	#define CAL_BUTTON_KEYPLUS_RECT 		{80,99, 159, 125}
	#define CAL_BUTTON_KEYMINUS_RECT 	{80,153, 159, 179}
	#define CAL_BUTTON_KEYMUL_RECT 		{0,126, 79, 152}
	#define CAL_BUTTON_KEYDIV_RECT 		{160,126, 239, 152}
	#define CAL_BUTTON_KEYEQUAL_RECT 	{80,126, 159, 152}
	#define CAL_BUTTON_KEYCLEAR_RECT 	{0, 99, 79, 125}
	#define CAL_BUTTON_KEYBACKSPACE_RECT 	{160, 99, 239, 125}
	
#else
    //calculator begin
	#define CAL_BUTTON_KEY1_RECT 		{0, 181, 79, 206}
	#define CAL_BUTTON_KEY2_RECT 		{80, 181, 159, 206}
	#define CAL_BUTTON_KEY3_RECT 		{160, 181, 239, 206}
	#define CAL_BUTTON_KEY4_RECT 		{0, 208, 79, 233}
	#define CAL_BUTTON_KEY5_RECT 		{80, 208, 159, 233}
	#define CAL_BUTTON_KEY6_RECT 		{160, 208, 239, 233}
    #define CAL_BUTTON_KEY7_RECT 		{0, 235, 79, 260}
	#define CAL_BUTTON_KEY8_RECT 		{80, 235, 159, 260}
	#define CAL_BUTTON_KEY9_RECT 		{160, 235, 239, 260}	
	#define CAL_BUTTON_KEYSTAR_RECT 	{0, 261, 79, 287}
	#define CAL_BUTTON_KEY0_RECT 		{80, 261, 159, 287}
	#define CAL_BUTTON_KEYHASH_RECT 		{160, 261, 239, 287}
	#define CAL_BUTTON_KEYPLUS_RECT 		{80, 99, 159, 125}
	#define CAL_BUTTON_KEYMINUS_RECT 	{80, 153, 159, 179}
	#define CAL_BUTTON_KEYMUL_RECT 		{0, 126, 79, 153}
	#define CAL_BUTTON_KEYDIV_RECT 		{160, 126, 239, 153}
	#define CAL_BUTTON_KEYEQUAL_RECT 	{80, 126, 155, 153}
	#define CAL_BUTTON_KEYCLEAR_RECT 	{0, 99, 79, 125}
	#define CAL_BUTTON_KEYBACKSPACE_RECT 	{160, 99, 239, 125}
#endif

#ifdef MMI_CALENDAR_KEY_STYLE
    #define CALENDAR_BLOCK_HEIGHT_V                     29
    #define CALENDAR_WEEKLY_SELECT_HEIGHT_V             33
    #define CALENDAR_DAILY_ITEM_HEIGH_V                 28
#else
	#define CALENDAR_BLOCK_HEIGHT_V                     32
    #define CALENDAR_WEEKLY_SELECT_HEIGHT_V             33
    #define CALENDAR_DAILY_ITEM_HEIGH_V                 30
#endif
    #define CALENDAR_BLOCK_WIDTH_V                      34 
    #define CALENDAR_WEEKLY_DATE_Y_V     			    59 
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V		    {0, 20, 239, CALENDAR_WEEKLY_DATE_Y_V-1}
	#define CALENDAR_YEAR_TITLE_RECT_V 	                {45, 25, 239, 43}
    #define CALENDAR_MONTH_TITLE_RECT_V 	            {5, 25, 41, 61}
    #define CALENDAR_MONTH_LINE0_TOP_V                  87
    #define CALENDAR_MONTH_ITEM0_LEFT_V                 1
    #define CALENDAR_SCH_TEXT_OFFSET_V                  5
    #define CALENDAR_POINT_Y_V                          305
    #define CALENDAR_LUNAR_TITLE_RECT_V 	            {45, 43, 239, 61}
    #define CALENDAR_DAILY_START_Y_OFFSET_V             58
    #define CALENDAR_DAILY_TITLE_RECT_V 	            {45, 21, 185, CALENDAR_DAILY_START_Y_OFFSET_V-1}
    #define CALENDAR_DAILY_MAX_ITEM_NUM_V               8
    #define CALENDAR_DAILY_TIME_RIGHT_V                 75
    #define CALENDAR_MONTH_WEEKLY_Y_V    				70

#ifdef MMI_CALENDAR_KEY_STYLE
    #define CALENDAR_BLOCK_HEIGHT_H                     22
    #define CALENDAR_WEEKLY_SELECT_HEIGHT_H   	        24
    #define CALENDAR_DAILY_ITEM_HEIGH_H                 28
#else
	#define CALENDAR_BLOCK_HEIGHT_H                     25
    #define CALENDAR_WEEKLY_SELECT_HEIGHT_H   	        26 
    #define CALENDAR_DAILY_ITEM_HEIGH_H                 30
#endif
	#define CALENDAR_BLOCK_WIDTH_H                      45
    #define CALENDAR_WEEKLY_DATE_Y_H    				41
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H			{0,21,319,CALENDAR_WEEKLY_DATE_Y_H-1}
	#define CALENDAR_YEAR_TITLE_RECT_H				    {47, 25, 95, 51}
	#define CALENDAR_MONTH_TITLE_RECT_H 				{5, 25, 41, 51}
    #define CALENDAR_MONTH_LINE0_TOP_H                  76
    #define CALENDAR_MONTH_ITEM0_LEFT_H                 2
	#define CALENDAR_SCH_TEXT_OFFSET_H                  5
    #define CALENDAR_POINT_Y_H                          230
    #define CALENDAR_LUNAR_TITLE_RECT_H 	            {98, 25, 319, 51}
    #define CALENDAR_DAILY_START_Y_OFFSET_H             41
    #define CALENDAR_DAILY_TITLE_RECT_H 	            {40, 21, 280, CALENDAR_DAILY_START_Y_OFFSET_H-1}
    #define CALENDAR_DAILY_MAX_ITEM_NUM_H               6
    #define CALENDAR_DAILY_TIME_RIGHT_H                 75
    #define CALENDAR_MONTH_WEEKLY_Y_H                   57
#ifdef VECTOR_FONT_SUPPORT_ETRUMP
    #define CALENDAR_ETRUMP_ADJUST_SIZE                 2
    #define CALENDAR_SCH_TEXT_FONT                      (SONG_FONT_17-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_MONTH_FONT                         (SONG_FONT_24-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_YEAR_FONT                          (SONG_FONT_18-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_DATE_FONT                          (SONG_FONT_16-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_WEEKLY_TITLE_FONT                  (SONG_FONT_22-CALENDAR_ETRUMP_ADJUST_SIZE)
#else
    #define CALENDAR_SCH_TEXT_FONT                      SONG_FONT_17
    #define CALENDAR_MONTH_FONT                         SONG_FONT_24
    #define CALENDAR_YEAR_FONT                          SONG_FONT_16
    #define CALENDAR_DATE_FONT                          SONG_FONT_16
    #define CALENDAR_WEEKLY_TITLE_FONT                  SONG_FONT_22
#endif
    #define CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H       (CALENDAR_BLOCK_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V             50
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H             40
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V     (CALENDAR_WEEKLY_SELECT_HEIGHT_V-2)
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H     (CALENDAR_WEEKLY_SELECT_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V      47
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H      40
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_H          (CALENDAR_DAILY_ITEM_HEIGH_H-4)
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_V          (CALENDAR_DAILY_ITEM_HEIGH_V-2)
#ifdef  MMI_UNITCONVERSION_SUPPORT
    #define MMIUC_FONT_SIZE  SONG_FONT_20
    #define MMIUC_OWNDRAW_HEIGHT  32    
    #define MMIUC_EDIT_FORM_HEIGHT  36      
    #define MMIUC_EDIT_MARGIN 2       
    #define MMIUC_FORM_VSAPCE  12
    #define MMIUC_FORM_HSAPCE  6    
    #define MMIUC_FORM_MARGIN  4    
    #define MMIUC_PICKER_HEIGHT_V  54
    #define MMIUC_PICKER_HEIGHT_H  40
    #define MMIUC_LABEL_WIDTH_PERCENT  25
#endif    
  
#elif defined MAINLCD_SIZE_128X64
    #define ACC_CALC_BL_IMAGE_RECT		{CALC_LINE_START_X, 5, MMICALC_TEXT_LEFT, 67}
    #define ACC_CALC_BL_DISP_RECT	    {CALC_LINE_START_X, CALC_LINE_START_Y, CALC_LINE_END_X,CALC_LINE_END_Y}

    //calcaluator
#ifdef TOUCH_PANEL_SUPPORT 
    #define MMICALC_TEXT_FONT           SONG_FONT_14    
    #define MMICALC_TEXT_FONT_H           SONG_FONT_14    
    #define CALC_LINE_HIGHT             15  
    #define CALC_LINE_HIGHT_H            15    
    #define BUTTON_HORIZONAL_SPACE   0
    #define BUTTON_VERTICAL_SPACE       0    
    #define BUTTON_HORIZONAL_SPACE_H     0
    #define BUTTON_VERTICAL_SPACE_H     3    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 4    
    #define CALC_LINE_RIGHT_BORDER      4     
    #define CALC_LINE_RIGHT_BORDER_H      4  
#else
    #define MMICALC_TEXT_FONT           SONG_FONT_14    
    #define MMICALC_TEXT_FONT_H           SONG_FONT_14    
    #define CALC_LINE_HIGHT             15  
    #define CALC_LINE_HIGHT_H            15    
    #define BUTTON_HORIZONAL_SPACE   0
    #define BUTTON_VERTICAL_SPACE       0    
    #define BUTTON_HORIZONAL_SPACE_H     0
    #define BUTTON_VERTICAL_SPACE_H     3    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 4    
    #define CALC_LINE_RIGHT_BORDER      4     
    #define CALC_LINE_RIGHT_BORDER_H      4  
#endif

#ifdef TOUCH_PANEL_SUPPORT
    #define WT_STANDARD_KEY_H_OFFSET    0
#else
    #define WT_STANDARD_KEY_H_OFFSET    5
#endif
	// êà??μ?í???óò
	#define WT_WORLD_MAP_REGION_LEFT          0
	#define WT_WORLD_MAP_REGION_TOP           28
	#define WT_WORLD_MAP_REGION_RIGHT         235
	#define WT_WORLD_MAP_REGION_BOTTOM        167

	// ò??ˉ?á????oí2?êy
	#define WT_MOVE_AXES_WIDTH                5
	#define WT_MOVE_AXES_HIGHT                124
	#define WT_MOVE_AXES_START_Y              MMI_TITLE_HEIGHT

	// °′?ü±3?°????2?êy
	#define WT_KEYSTOKE_BG_LEFT               0
	#define WT_KEYSTOKE_BG_TOP                168//258
	#define WT_KEYSTOKE_BG_WIDTH              240
	#define WT_KEYSTOKE_BG_HIGHT              23
	// ICON
	#define WT_ICON_WIDTH                       5
    #define WT_DROPLIST_SPACE                   9
	#define WT_ICON_HIGHT                       11
	#define WT_ICON_LEFT                        9
    #define WT_UP_ICON_TOP                      (200-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_ICON_TOP                    (245-WT_STANDARD_KEY_H_OFFSET)
	// TEXT
	#define WT_TEXT_WIDTH                       173
	#define WT_TEXT_HIGHT                       25
	#define WT_TEXT_LEFT                        18
	#define WT_UP_TEXT_TOP                      (193-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_TEXT_TOP                    (238-WT_STANDARD_KEY_H_OFFSET)
	#define WT_UP_WORLDDIFF_TOP                 (WT_UP_TEXT_TOP+4) 
	#define WT_DOWN_WORLDDIFF_TOP               (WT_DOWN_TEXT_TOP+4)   
    #define WT_UP_WORLDTIME_TOP                 (WT_UP_TEXT_TOP + WT_TEXT_HIGHT + 2) 
	#define WT_DOWN_WORLDTIME_TOP               (WT_DOWN_TEXT_TOP + WT_TEXT_HIGHT + 2)    

    #define WT_WORLDTIME_LEFT                   15 
	#define WT_WORLDTIME_RIGHT                  175   
	
	#define WT_KEYSTOKE_LEFT_ARROW_LEFT       WT_KEYSTOKE_BG_LEFT
	#define WT_KEYSTOKE_LEFT_ARROW_TOP        WT_KEYSTOKE_BG_TOP
	#define WT_KEYSTOKE_RIGHT_ARROW_LEFT      120
	#define WT_KEYSTOKE_RIGHT_ARROW_TOP       WT_KEYSTOKE_BG_TOP

	#define WT_KEYSTOKE_ARROW_WIGTH           120
	#define WT_KEYSTOKE_ARROW_HIGHT           23
	//two line distance hight
	#define WT_TWOLINE_DISTANCE_HIGHT           MMI_DEFAULT_SMALL_FONT
	// ??±?±3?°????2?êy
	#define WT_TEXT_BG_LEFT                   8
	#define WT_TEXT_BG_TOP1                   (171-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_TOP2                   (193-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_WIDTH                  184
	#define WT_TEXT_BG_HIGHT                  25//17//@zhaohui,cr108515

	// ??à-2?μ￥?ü????2?êy
	#define WT_DROPDOWN_KEY_LEFT              160
	#define WT_DROPDOWN_KEY_TOP1              (174-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_TOP2              (219-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_WIDTH             20
	#define WT_DROPDOWN_KEY_HIGHT             19

	#define WT_TIME_DIFF_LEFT                 185
	#define WT_TIME_DIFF_RIGHT                225

	//counted time
	#define MMICT_TEXT_FONT         SONG_FONT_30
	#define CT_EDIT_MARGIN  5 //定义边框
	#define CT_BUTTON_MARGIN  4 //定义边框
	#define CT_BUTTON_HEIGHT_V  40
	#define CT_BUTTON_HEIGHT_H  32
	#define CT_PICKER_HEIGHT  40
	#define CT_PICKER_ACTIVE_FONT  SONG_FONT_29
	#define CT_PICKER_NORMAL_FONT  SONG_FONT_24

	//stop watch
	#define MMIST_TIME1_OFFSET_Y_V  18
	#define MMIST_TIME1_OFFSET_Y_H  10
	#define MMIST_TIME2_OFFSET_Y_V  28
	#define MMIST_TIME2_OFFSET_Y_H  10
	#define ST_TIMER1_HEIGHT_V 15
	#define ST_TIMER1_HEIGHT_H 14
	#define ST_TIMER2_HEIGHT_V 44
	#define ST_TIMER2_HEIGHT_H 31
	#define ST_BUTTON_HEIGHT_V  40
	#define ST_BUTTON_HEIGHT_H  32
	#define ST_BUTTON_OFFSET_X_V  9
	#define ST_BUTTON_OFFSET_X_H  18
	#define ST_TIMER1_STR_OFFSET_X_V  (ST_BUTTON_OFFSET_X_V + 6)
	#define ST_TIMER1_STR_OFFSET_X_H  (ST_BUTTON_OFFSET_X_H + 6)
	#define ST_BUTTON_MARGIN  8
    
#ifdef NON_STANDARD_KEY_LAYOUT_SUPPORT
    #define CAL_BUTTON_KEY1_RECT 		{0,181, 59, 215}
	#define CAL_BUTTON_KEY2_RECT 		{60,181, 119, 215}
	#define CAL_BUTTON_KEY3_RECT 		{120,181, 179, 215}
	#define CAL_BUTTON_KEY4_RECT 		{0,217, 59, 251}
	#define CAL_BUTTON_KEY5_RECT 		{60,217, 119, 251}
	#define CAL_BUTTON_KEY6_RECT 		{120,217, 179, 251}
    #define CAL_BUTTON_KEY7_RECT 		{0,253, 59, 288}
	#define CAL_BUTTON_KEY8_RECT 		{60,253, 119, 288}
	#define CAL_BUTTON_KEY9_RECT 		{120,253, 179, 288}	
	#define CAL_BUTTON_KEYSTAR_RECT 	{180,181, 239, 215}
	#define CAL_BUTTON_KEY0_RECT 		{180,217, 239, 251}
	#define CAL_BUTTON_KEYHASH_RECT 		{180,253, 239, 288}

	#define CAL_BUTTON_KEYPLUS_RECT 		{80,99, 159, 125}
	#define CAL_BUTTON_KEYMINUS_RECT 	{80,153, 159, 179}
	#define CAL_BUTTON_KEYMUL_RECT 		{0,126, 79, 152}
	#define CAL_BUTTON_KEYDIV_RECT 		{160,126, 239, 152}
	#define CAL_BUTTON_KEYEQUAL_RECT 	{80,126, 159, 152}
	#define CAL_BUTTON_KEYCLEAR_RECT 	{0, 99, 79, 125}
	#define CAL_BUTTON_KEYBACKSPACE_RECT 	{160, 99, 239, 125}
	
#else
    //calculator begin
	#define CAL_BUTTON_KEY1_RECT 		{0, 181, 79, 206}
	#define CAL_BUTTON_KEY2_RECT 		{80, 181, 159, 206}
	#define CAL_BUTTON_KEY3_RECT 		{160, 181, 239, 206}
	#define CAL_BUTTON_KEY4_RECT 		{0, 208, 79, 233}
	#define CAL_BUTTON_KEY5_RECT 		{80, 208, 159, 233}
	#define CAL_BUTTON_KEY6_RECT 		{160, 208, 239, 233}
    #define CAL_BUTTON_KEY7_RECT 		{0, 235, 79, 260}
	#define CAL_BUTTON_KEY8_RECT 		{80, 235, 159, 260}
	#define CAL_BUTTON_KEY9_RECT 		{160, 235, 239, 260}	
	#define CAL_BUTTON_KEYSTAR_RECT 	{0, 261, 79, 287}
	#define CAL_BUTTON_KEY0_RECT 		{80, 261, 159, 287}
	#define CAL_BUTTON_KEYHASH_RECT 		{160, 261, 239, 287}
	#define CAL_BUTTON_KEYPLUS_RECT 		{80, 99, 159, 125}
	#define CAL_BUTTON_KEYMINUS_RECT 	{80, 153, 159, 179}
	#define CAL_BUTTON_KEYMUL_RECT 		{0, 126, 79, 153}
	#define CAL_BUTTON_KEYDIV_RECT 		{160, 126, 239, 153}
	#define CAL_BUTTON_KEYEQUAL_RECT 	{80, 126, 155, 153}
	#define CAL_BUTTON_KEYCLEAR_RECT 	{0, 99, 79, 125}
	#define CAL_BUTTON_KEYBACKSPACE_RECT 	{160, 99, 239, 125}
#endif
	//è?3ìêú?á
	#define CALENDAR_BLOCK_WIDTH_V            	31 
	#define CALENDAR_BLOCK_HEIGHT_V           	18
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_V   	29//22  
	#define CALENDAR_QUERY_TEXT_BG_X_V        	4
	#define CALENDAR_QUERY_TEXT_BG_Y_V        	114
	#define CALENDAR_QUERY_SOLAR_TOP_V       	50
	#define CALENDAR_QUERY_PIC_START_X_V      	124
	#define CALENDAR_QUERY_PIC_START_Y_V      	114
	#define CALENDAR_QUERY_FEAST_LEFT_V       	80
	#define CALENDAR_QUERY_FEAST_RIGHT_V      	140
	#define CALENDAR_QUERY_FEAST_TOP_V        	CALENDAR_QUERY_YEAR_TOP_V
	#define CALENDAR_QUERY_FEAST_BOTTOM_V     	CALENDAR_QUERY_YEAR_BOTTOM_V
	#define CALENDAR_QUERY_ARROW_LEFT1_V      	145
	#define CALENDAR_QUERY_ARROW_LEFT2_V      	159
	#define CALENDAR_QUERY_ARROW_TOP_V        	(CALENDAR_QUERY_SOLAR_TOP_V + 8)
	#define CALENDAR_MONTHLY_YEAR_LA_RECT_V		{6, 47, 37, 85}			//LEFT ARROW
	#define CALENDAR_MONTHLY_YEAR_RA_RECT_V		{104, 47, 135, 85}		//RIGHT ARROW
	#define CALENDAR_MONTHLY_MON_LA_RECT_V		{134, 47, 165, 85}	//LEFT ARROW
	#define CALENDAR_MONTHLY_MON_RA_RECT_V		{208, 47, 239, 85}	//RIGHT ARROW
    #define CALENDAR_WEEKLY_LEFT_X_V          	38       
    #define CALENDAR_WEEKLY_LEFT_WIDTH_V     	25
    #define CALENDAR_WEEKLY_RIGHT_X_V         	50
    #define CALENDAR_WEEKLY_RIGHT_WIDTH_V     	30//32
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT1_V		{91, 80, 115, 102}
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT2_V		{116, 80, 140, 102}
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT3_V		{141, 80, 180, 102}
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V			{26, 47, 214, 80}
	#define CALENDAR_WEEKLY_YEAR_CONTENT_RECT_V			{38, 43, 92, 75}
	#define CALENDAR_WEEKLY_MON_CONTENT_RECT_V			{160, 43, 201, 75}
	#define CALENDAR_WEEKLY_DATE_X_V          			30//38   
	#define CALENDAR_WEEKLY_DATE_Y_V     				84//109  
	#define CALENDAR_WEEKLY_LINE_RIGHT_V     			239//232 
	#define CALENDAR_WEEKLY_WEEK_LA_RECT_V				{0, 47, 32, 80}		//LEFT ARROW
	#define CALENDAR_WEEKLY_WEEK_RA_RECT_V				{208, 47, 240, 80}		//RIGHT ARROW
	#define CALENDAR_WEEKLY_YEAR_LA_RECT_V				{6, 36, 31, 73}			//LEFT ARROW
	#define CALENDAR_WEEKLY_YEAR_RA_RECT_V				{104, 36, 129, 73}		//RIGHT ARROW
	#define CALENDAR_WEEKLY_MON_LA_RECT_V				{134, 36, 159, 73}
	#define CALENDAR_WEEKLY_MON_RA_RECT_V				{208, 36, 233, 73}
	#define CALENDAR_WEEKLY_DATE_LA_RECT_V				{210, 140, 235, 161}
	#define CALENDAR_WEEKLY_DATE_RA_RECT_V				{210, 209, 235, 230}
	#define CALENDAR_DATE_RECT_V						{5, 112, 236, 244}
	#define CALENDAR_DATE_BLOCK_WIDTH_V					33
	#define CALENDAR_DATE_BLOCK_HEIGHT_V				22
	#define YEAR_TITLE_RECT_V 				{41, 47, 95, 80}
	#define MONTH_TITLE_RECT_V 				{163, 47, 204, 80}
	#define CALENDAR_LUN_LEFT_V             0
	#define CALENDAR_LUN_TOP_V              254
    #define CALENDAR_MONTH_LINE0_TOP_V      112
    #define CALENDAR_MONTH_LINE1_TOP_V      134//136
    #define CALENDAR_MONTH_LINE2_TOP_V      156//160
    #define CALENDAR_MONTH_LINE3_TOP_V      178//184
    #define CALENDAR_MONTH_LINE4_TOP_V      200//208
    #define CALENDAR_MONTH_LINE5_TOP_V      222//232
    #define CALENDAR_MONTH_ITEM0_LEFT_V     5//"è?"μ?x×?±ê
    #define CALENDAR_MONTH_ITEM1_LEFT_V     38//"ò?"μ?x×?±ê
    #define CALENDAR_MONTH_ITEM2_LEFT_V     71//"?t"μ?x×?±ê
    #define CALENDAR_MONTH_ITEM3_LEFT_V     104//"èy"μ?x×?±ê
    #define CALENDAR_MONTH_ITEM4_LEFT_V     137//"??"μ?x×?±ê
    #define CALENDAR_MONTH_ITEM5_LEFT_V     170//"??"μ?x×?±ê
    #define CALENDAR_MONTH_ITEM6_LEFT_V     203//"áù"μ?x×?±ê
	#define CALENDAR_LUN_RIGHT_V            239
	#define CALENDAR_LUN_BOTTOM_V           286  
	#define CALENDAR_WEEK_DOT_X_V           217 
	#define CALENDAR_WEEK_DOT_Y_V           180 
	#define CALENDAR_WEEK_DOT_X_H           295 
	#define CALENDAR_WEEK_DOT_Y_H           133 
	//è?3ìoá?á
	#define CALENDAR_BLOCK_WIDTH_H          	31 //
	#define CALENDAR_BLOCK_HEIGHT_H            	18//
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_H    	21//19 
	#define CALENDAR_QUERY_TEXT_BG_X_H         	63//
	#define CALENDAR_QUERY_TEXT_BG_Y_H         	80//
	#define CALENDAR_QUERY_SOLAR_TOP_H         	37
	#define CALENDAR_QUERY_PIC_START_X_H       	124
	#define CALENDAR_QUERY_PIC_START_Y_H       	114
	#define CALENDAR_QUERY_FEAST_LEFT_H        	80
	#define CALENDAR_QUERY_FEAST_RIGHT_H       	140
	#define CALENDAR_QUERY_FEAST_TOP_H         	CALENDAR_QUERY_YEAR_TOP_H
	#define CALENDAR_QUERY_FEAST_BOTTOM_H      	CALENDAR_QUERY_YEAR_BOTTOM_H
	#define CALENDAR_QUERY_ARROW_LEFT1_H       	145
	#define CALENDAR_QUERY_ARROW_LEFT2_H       	159
	#define CALENDAR_QUERY_ARROW_TOP_H         	(CALENDAR_QUERY_SOLAR_TOP_H + 8)
	#define CALENDAR_MONTHLY_YEAR_LA_RECT_H 			{15, 33, 40, 58}			//LEFT ARROW
	#define CALENDAR_MONTHLY_YEAR_RA_RECT_H 			{85, 33, 110, 58}		//RIGHT ARROW
	#define CALENDAR_MONTHLY_MON_LA_RECT_H 				{210, 33, 235, 58}	//LEFT ARROW
	#define CALENDAR_MONTHLY_MON_RA_RECT_H 				{280, 33, 305, 58}	//RIGHT ARROW
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT1_H 		{91, 80, 115, 102}
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT2_H 		{116, 80, 140, 102}
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT3_H 		{141, 80, 180, 102}
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H 		{35, 33, 285, 65}//
	#define CALENDAR_WEEKLY_YEAR_CONTENT_RECT_H 		{30, 35, 69, 61}//
	#define CALENDAR_WEEKLY_MON_CONTENT_RECT_H 			{129, 30, 157, 56}//
	#define CALENDAR_WEEKLY_DATE_X_H           			30//36
	#define CALENDAR_WEEKLY_DATE_Y_H      				61//  
	#define CALENDAR_WEEKLY_LINE_RIGHT_H      			319 
	#define CALENDAR_WEEKLY_WEEK_LA_RECT_H 				{2, 33, 34, 60}		//LEFT ARROW//merlin
	#define CALENDAR_WEEKLY_WEEK_RA_RECT_H 				{286, 33, 318, 60}		//RIGHT ARROW//merlin
	#define CALENDAR_WEEKLY_YEAR_LA_RECT_H 				{4, 30, 29, 55}			//LEFT ARROW//merlin
	#define CALENDAR_WEEKLY_YEAR_RA_RECT_H 				{70, 30, 96, 55}		//RIGHT ARROW
	#define CALENDAR_WEEKLY_MON_LA_RECT_H 				{103, 30, 128, 55}
	#define CALENDAR_WEEKLY_MON_RA_RECT_H 				{158, 30, 183, 55}//merlin
	#define CALENDAR_WEEKLY_DATE_LA_RECT_H 				{290, 94, 315, 115}
	#define CALENDAR_WEEKLY_DATE_RA_RECT_H 				{290, 162, 315, 183}
	#define CALENDAR_DATE_RECT_H 						{13, 88, 302, 208}//merlin
	#define CALENDAR_DATE_BLOCK_WIDTH_H 				44//
	#define CALENDAR_DATE_BLOCK_HEIGHT_H 				20//
	#define YEAR_TITLE_RECT_H  							{41, 35, 84, 58}
	#define MONTH_TITLE_RECT_H  						{236, 35, 279, 58}
	#define CALENDAR_LUN_LEFT_H                			0
	#define CALENDAR_LUN_TOP_H                 			35
	#define CALENDAR_MONTH_LINE0_TOP_H      			88//??ê?D??ú"è?~áù"μ?y×?±ê
	#define CALENDAR_MONTH_LINE1_TOP_H      			108//??ê???ì?è??úμ?y×?±ê,ò?125DD
	#define CALENDAR_MONTH_LINE2_TOP_H      			128
	#define CALENDAR_MONTH_LINE3_TOP_H      			148
	#define CALENDAR_MONTH_LINE4_TOP_H      			168
	#define CALENDAR_MONTH_LINE5_TOP_H      			188
	#define CALENDAR_MONTH_ITEM0_LEFT_H     			13//"è?"μ?x×?±ê
	#define CALENDAR_MONTH_ITEM1_LEFT_H     			57//"ò?"μ?x×?±ê
	#define CALENDAR_MONTH_ITEM2_LEFT_H     			101//"?t"μ?x×?±ê
	#define CALENDAR_MONTH_ITEM3_LEFT_H     			145//"èy"μ?x×?±ê
	#define CALENDAR_MONTH_ITEM4_LEFT_H     			189//"??"μ?x×?±ê
	#define CALENDAR_MONTH_ITEM5_LEFT_H     			233//"??"μ?x×?±ê
	#define CALENDAR_MONTH_ITEM6_LEFT_H     			277//"áù"μ?x×?±ê
	#define CALENDAR_LUN_RIGHT_H             			319
	#define CALENDAR_LUN_BOTTOM_H             			60   
	#define CALENDAR_WEEKLY_LEFT_X_H           			36       
	#define CALENDAR_WEEKLY_LEFT_WIDTH_H       			25
	#define CALENDAR_WEEKLY_RIGHT_X_H          			50   
	#define CALENDAR_WEEKLY_RIGHT_WIDTH_H      			30//32

	#define CALENDAR_TITLE_FONT             SONG_FONT_18
	#define CALENDAR_TEXT_FONT              SONG_FONT_16
	
#elif defined MAINLCD_SIZE_240X400
#ifdef MMI_PDA_SUPPORT
#define CALENDAR_WEEK_TEXT_FONT   SONG_FONT_16
#endif

    //calcaluator
#ifdef TOUCH_PANEL_SUPPORT  
    #define MMICALC_TEXT_FONT           SONG_FONT_42  
    #define MMICALC_TEXT_FONT_H           SONG_FONT_34   
    #define CALC_LINE_HIGHT             84  
    #define CALC_LINE_HIGHT_H            58    
    #define BUTTON_HORIZONAL_SPACE   4
    #define BUTTON_VERTICAL_SPACE       4    
    #define BUTTON_HORIZONAL_SPACE_H     4
    #define BUTTON_VERTICAL_SPACE_H     4    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 4    
    #define CALC_LINE_RIGHT_BORDER      18    
    #define CALC_LINE_RIGHT_BORDER_H      20  
#else
    #define MMICALC_TEXT_FONT           SONG_FONT_42  
    #define MMICALC_TEXT_FONT_H           SONG_FONT_34   
    #define CALC_LINE_HIGHT             94  
    #define CALC_LINE_HIGHT_H            58    
    #define BUTTON_HORIZONAL_SPACE   4
    #define BUTTON_VERTICAL_SPACE       4    
    #define BUTTON_HORIZONAL_SPACE_H     3
    #define BUTTON_VERTICAL_SPACE_H     5    
    #define FORM_MARGIN 2
    #define FORM_MARGIN_H 6    
    #define CALC_LINE_RIGHT_BORDER      14   
    #define CALC_LINE_RIGHT_BORDER_H      14  
#endif

#ifdef TOUCH_PANEL_SUPPORT
    #define WT_STANDARD_KEY_H_OFFSET    0
#else
    #define WT_STANDARD_KEY_H_OFFSET    5
#endif
	// êà??μ?í???óò
	#define WT_WORLD_MAP_REGION_LEFT          0
	#define WT_WORLD_MAP_REGION_TOP           28
	#define WT_WORLD_MAP_REGION_RIGHT         235
	#define WT_WORLD_MAP_REGION_BOTTOM        167

	// ò??ˉ?á????oí2?êy
	#define WT_MOVE_AXES_WIDTH                5
	#define WT_MOVE_AXES_HIGHT                124
	#define WT_MOVE_AXES_START_Y              MMI_TITLE_HEIGHT

	// °′?ü±3?°????2?êy
	#define WT_KEYSTOKE_BG_LEFT               0
	#define WT_KEYSTOKE_BG_TOP                168//258
	#define WT_KEYSTOKE_BG_WIDTH              240
	#define WT_KEYSTOKE_BG_HIGHT              23
	// ICON
	#define WT_ICON_WIDTH                       5
    #define WT_DROPLIST_SPACE                   9 
	#define WT_ICON_HIGHT                       11
	#define WT_ICON_LEFT                        9
	#define WT_UP_ICON_TOP                      (WT_UP_TEXT_TOP+7)
	#define WT_DOWN_ICON_TOP                    (WT_DOWN_TEXT_TOP+7)
	// TEXT
	#define WT_TEXT_WIDTH                       160
	#define WT_TEXT_HIGHT                       25
	#define WT_TEXT_LEFT                        18
	#define WT_UP_TEXT_TOP                      (219-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_TEXT_TOP                    (298-WT_STANDARD_KEY_H_OFFSET)
	#define WT_UP_WORLDDIFF_TOP                 (WT_UP_TEXT_TOP) 
	#define WT_DOWN_WORLDDIFF_TOP               (WT_DOWN_TEXT_TOP + 2)    
	#define WT_UP_WORLDTIME_TOP                 (WT_UP_TEXT_TOP + WT_TEXT_HIGHT + 15) 
	#define WT_DOWN_WORLDTIME_TOP               (WT_DOWN_TEXT_TOP + WT_TEXT_HIGHT + 15)    

    #define WT_WORLDTIME_LEFT                   15 
	#define WT_WORLDTIME_RIGHT                  175   
	
	#define WT_KEYSTOKE_LEFT_ARROW_LEFT       WT_KEYSTOKE_BG_LEFT
	#define WT_KEYSTOKE_LEFT_ARROW_TOP        WT_KEYSTOKE_BG_TOP
	#define WT_KEYSTOKE_RIGHT_ARROW_LEFT      120
	#define WT_KEYSTOKE_RIGHT_ARROW_TOP       WT_KEYSTOKE_BG_TOP

	#define WT_KEYSTOKE_ARROW_WIGTH           120
	#define WT_KEYSTOKE_ARROW_HIGHT           23
	//two line distance hight
	#define WT_TWOLINE_DISTANCE_HIGHT           MMI_DEFAULT_SMALL_FONT
	
	// ??±?±3?°????2?êy
	#define WT_TEXT_BG_LEFT                   8
	#define WT_TEXT_BG_TOP1                   (171-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_TOP2                   (215-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_WIDTH                  184
	#define WT_TEXT_BG_HIGHT                  25//@zhaohui,cr108515

	// ??à-2?μ￥?ü????2?êy
	#define WT_DROPDOWN_KEY_LEFT              160
	#define WT_DROPDOWN_KEY_TOP1              (174-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_TOP2              (219-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_WIDTH             20
	#define WT_DROPDOWN_KEY_HIGHT             19

	#define WT_TIME_DIFF_LEFT                 185
	#define WT_TIME_DIFF_RIGHT                225

	//counted time
	#define MMICT_TEXT_FONT	SONG_FONT_36
	#define CT_EDIT_MARGIN  5 //定义边框
	#define CT_BUTTON_MARGIN  4 //定义边框
	#define CT_BUTTON_HEIGHT_V  40
	#define CT_BUTTON_HEIGHT_H  32
	#define CT_PICKER_HEIGHT  50
	#define CT_PICKER_ACTIVE_FONT  SONG_FONT_29
	#define CT_PICKER_NORMAL_FONT  SONG_FONT_24

    	//stop watch
	#define MMIST_TIME1_OFFSET_Y_V	18
	#define MMIST_TIME1_OFFSET_Y_H	10
	#define MMIST_TIME2_OFFSET_Y_V	28
	#define MMIST_TIME2_OFFSET_Y_H	10
	#define ST_TIMER1_HEIGHT_V 15
	#define ST_TIMER1_HEIGHT_H 14
	#define ST_TIMER2_HEIGHT_V 44
	#define ST_TIMER2_HEIGHT_H 31
	#define ST_BUTTON_HEIGHT_V  40
	#define ST_BUTTON_HEIGHT_H  32
	#define ST_BUTTON_OFFSET_X_V  9
	#define ST_BUTTON_OFFSET_X_H  18
	#define ST_TIMER1_STR_OFFSET_X_V  (ST_BUTTON_OFFSET_X_V + 6)
	#define ST_TIMER1_STR_OFFSET_X_H  (ST_BUTTON_OFFSET_X_H + 6)
	#define ST_BUTTON_MARGIN  8

	//calculator begin
    #define CAL_BUTTON_KEY_OFFSET 		80 
    #define CAL_BUTTON_KEY1_RECT 		{0, 181+CAL_BUTTON_KEY_OFFSET, 79, 206+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEY2_RECT 		{80, 181+CAL_BUTTON_KEY_OFFSET, 159, 206+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEY3_RECT 		{160, 181+CAL_BUTTON_KEY_OFFSET, 239, 206+CAL_BUTTON_KEY_OFFSET}  	
	#define CAL_BUTTON_KEY4_RECT 		{0, 208+CAL_BUTTON_KEY_OFFSET, 79, 233+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEY5_RECT 		{80, 208+CAL_BUTTON_KEY_OFFSET, 159, 233+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEY6_RECT 		{160, 208+CAL_BUTTON_KEY_OFFSET, 239, 233+CAL_BUTTON_KEY_OFFSET}
    #define CAL_BUTTON_KEY7_RECT 		{0, 235+CAL_BUTTON_KEY_OFFSET, 79, 260+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEY8_RECT 		{80, 235+CAL_BUTTON_KEY_OFFSET, 159, 260+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEY9_RECT 		{160, 235+CAL_BUTTON_KEY_OFFSET, 239, 260+CAL_BUTTON_KEY_OFFSET}
	
	
	#define CAL_BUTTON_KEYSTAR_RECT 	{0, 261+CAL_BUTTON_KEY_OFFSET, 79, 287+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEY0_RECT 		{80, 261+CAL_BUTTON_KEY_OFFSET, 159, 287+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYHASH_RECT 		{160, 261+CAL_BUTTON_KEY_OFFSET, 239, 287+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYPLUS_RECT 		{80, 99+CAL_BUTTON_KEY_OFFSET, 159, 125+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYMINUS_RECT 	{80, 153+CAL_BUTTON_KEY_OFFSET, 159, 179+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYMUL_RECT 		{0, 126+CAL_BUTTON_KEY_OFFSET, 79, 153+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYDIV_RECT 		{160, 126+CAL_BUTTON_KEY_OFFSET, 239, 153+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYEQUAL_RECT 	{80, 126+CAL_BUTTON_KEY_OFFSET, 155, 153+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYCLEAR_RECT 	{0, 99+CAL_BUTTON_KEY_OFFSET, 79, 125+CAL_BUTTON_KEY_OFFSET}
	#define CAL_BUTTON_KEYBACKSPACE_RECT 	{160, 99+CAL_BUTTON_KEY_OFFSET, 239, 125+CAL_BUTTON_KEY_OFFSET}

  
#ifdef MMI_CALENDAR_KEY_STYLE
    #define CALENDAR_DAILY_MAX_ITEM_NUM_V               10
#else
    #define CALENDAR_DAILY_MAX_ITEM_NUM_V               11
#endif
	#define CALENDAR_BLOCK_WIDTH_V                      34 
	#define CALENDAR_BLOCK_HEIGHT_V                     38
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_V             45
    #define CALENDAR_WEEKLY_DATE_Y_V     			    52 
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V		    {0, 21, 239, CALENDAR_WEEKLY_DATE_Y_V-1}
	#define CALENDAR_YEAR_TITLE_RECT_V 	                {45, 25, 239, 43}
    #define CALENDAR_MONTH_TITLE_RECT_V 	            {5, 25, 41, 61}
    #define CALENDAR_MONTH_LINE0_TOP_V                  87
    #define CALENDAR_MONTH_ITEM0_LEFT_V                 1
    #define CALENDAR_SCH_TEXT_OFFSET_V                  10
    #define CALENDAR_POINT_Y_V                          385
    #define CALENDAR_LUNAR_TITLE_RECT_V 	            {45, 43, 239, 61}
    #define CALENDAR_DAILY_START_Y_OFFSET_V             52
    #define CALENDAR_DAILY_TITLE_RECT_V 	            {45, 21, 185, CALENDAR_DAILY_START_Y_OFFSET_V-1}
    #define CALENDAR_DAILY_ITEM_HEIGH_V                 30
    #define CALENDAR_DAILY_TIME_RIGHT_V                 75
    #define CALENDAR_MONTH_WEEKLY_Y_V    				70

#ifdef MMI_CALENDAR_KEY_STYLE
    #define CALENDAR_BLOCK_HEIGHT_H                     22
    #define CALENDAR_WEEKLY_SELECT_HEIGHT_H   	        24
    #define CALENDAR_DAILY_ITEM_HEIGH_H                 28
#else
	#define CALENDAR_BLOCK_HEIGHT_H                     25
    #define CALENDAR_WEEKLY_SELECT_HEIGHT_H   	        26 
    #define CALENDAR_DAILY_ITEM_HEIGH_H                 30
#endif
	#define CALENDAR_BLOCK_WIDTH_H                      42 
    #define CALENDAR_WEEKLY_DATE_Y_H    				41
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H			{0,21,399,CALENDAR_WEEKLY_DATE_Y_H-1}
	#define CALENDAR_YEAR_TITLE_RECT_H				    {47, 25, 95, 51}
	#define CALENDAR_MONTH_TITLE_RECT_H 				{5, 25, 41, 51}
    #define CALENDAR_MONTH_LINE0_TOP_H                  68
    #define CALENDAR_MONTH_ITEM0_LEFT_H                 0
	#define CALENDAR_SCH_TEXT_OFFSET_H                  8
    #define CALENDAR_POINT_Y_H                          230
    #define CALENDAR_LUNAR_TITLE_RECT_H 	            {98, 25, 399, 51}
    #define CALENDAR_DAILY_START_Y_OFFSET_H             41
    #define CALENDAR_DAILY_TITLE_RECT_H 	            {90, 21, 310, CALENDAR_DAILY_START_Y_OFFSET_H-1}
    #define CALENDAR_DAILY_MAX_ITEM_NUM_H               6
    #define CALENDAR_DAILY_TIME_RIGHT_H                 75
    #define CALENDAR_MONTH_WEEKLY_Y_H    				53
#ifdef VECTOR_FONT_SUPPORT_ETRUMP
    #define CALENDAR_ETRUMP_ADJUST_SIZE                 2
    #define CALENDAR_SCH_TEXT_FONT                      (SONG_FONT_17-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_MONTH_FONT                         (SONG_FONT_24-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_YEAR_FONT                          (SONG_FONT_18-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_DATE_FONT                          (SONG_FONT_16-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_WEEKLY_TITLE_FONT                  (SONG_FONT_22-CALENDAR_ETRUMP_ADJUST_SIZE)
#else
    #define CALENDAR_SCH_TEXT_FONT                      SONG_FONT_17
    #define CALENDAR_MONTH_FONT                         SONG_FONT_24
    #define CALENDAR_YEAR_FONT                          SONG_FONT_18
    #define CALENDAR_DATE_FONT                          SONG_FONT_16
    #define CALENDAR_WEEKLY_TITLE_FONT                  SONG_FONT_22
#endif
    #define CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H       (CALENDAR_BLOCK_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V             50
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H             40
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V     (CALENDAR_WEEKLY_SELECT_HEIGHT_V-2)
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H     (CALENDAR_WEEKLY_SELECT_HEIGHT_H-4)
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V      47
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H      40
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_H          (CALENDAR_DAILY_ITEM_HEIGH_H-4)
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_V          (CALENDAR_DAILY_ITEM_HEIGH_V-2)
#ifdef  MMI_UNITCONVERSION_SUPPORT       
    #define MMIUC_FONT_SIZE  SONG_FONT_18
    #define MMIUC_OWNDRAW_HEIGHT  32    
    #define MMIUC_EDIT_FORM_HEIGHT  36      
    #define MMIUC_EDIT_MARGIN 2       
    #define MMIUC_FORM_VSAPCE  21
    #define MMIUC_FORM_HSAPCE  6    
    #define MMIUC_FORM_MARGIN  4    
    #define MMIUC_PICKER_HEIGHT_V  54
    #define MMIUC_PICKER_HEIGHT_H  40
    #define MMIUC_LABEL_WIDTH_PERCENT  25
 #endif      
    //è?3ì°′??2é?′????,?à1?×?±ê,end  
#elif defined MAINLCD_SIZE_320X480
    
#ifdef MMI_PDA_SUPPORT
#define CALENDAR_WEEK_TEXT_FONT   SONG_FONT_16
#endif

    //calcaluator
#ifdef TOUCH_PANEL_SUPPORT 
    #define MMICALC_TEXT_FONT           SONG_FONT_40  //50    
    #define MMICALC_TEXT_FONT_H           SONG_FONT_48  
    #define CALC_LINE_HIGHT             100  
    #define CALC_LINE_HIGHT_H            72    
    #define BUTTON_HORIZONAL_SPACE   8
    #define BUTTON_VERTICAL_SPACE       8    
    #define BUTTON_HORIZONAL_SPACE_H     8
    #define BUTTON_VERTICAL_SPACE_H     8    
    #define FORM_MARGIN 4
    #define FORM_MARGIN_H 4    
    #define CALC_LINE_RIGHT_BORDER      18    
    #define CALC_LINE_RIGHT_BORDER_H      26  
#else
    #define MMICALC_TEXT_FONT           SONG_FONT_50    
    #define MMICALC_TEXT_FONT_H           SONG_FONT_48   
    #define CALC_LINE_HIGHT             100  
    #define CALC_LINE_HIGHT_H            72    
    #define BUTTON_HORIZONAL_SPACE   6
    #define BUTTON_VERTICAL_SPACE       5    
    #define BUTTON_HORIZONAL_SPACE_H     5
    #define BUTTON_VERTICAL_SPACE_H     5    
    #define FORM_MARGIN 5
    #define FORM_MARGIN_H 5    
    #define CALC_LINE_RIGHT_BORDER      16    
    #define CALC_LINE_RIGHT_BORDER_H      26  
#endif

#ifdef TOUCH_PANEL_SUPPORT
    #define WT_STANDARD_KEY_H_OFFSET    0
#else
    #define WT_STANDARD_KEY_H_OFFSET    5
#endif

	// êà??μ?í???óò
	#define WT_WORLD_MAP_REGION_LEFT          0
	#define WT_WORLD_MAP_REGION_TOP           28
	#define WT_WORLD_MAP_REGION_RIGHT         235
	#define WT_WORLD_MAP_REGION_BOTTOM        167

	// ò??ˉ?á????oí2?êy
	#define WT_MOVE_AXES_WIDTH                5
	#define WT_MOVE_AXES_HIGHT                124
	#define WT_MOVE_AXES_START_Y              MMI_TITLE_HEIGHT

	// °′?ü±3?°????2?êy
	#define WT_KEYSTOKE_BG_LEFT               0
	#define WT_KEYSTOKE_BG_TOP                168//258
	#define WT_KEYSTOKE_BG_WIDTH              240
	#define WT_KEYSTOKE_BG_HIGHT              23
	// ICON
	#define WT_ICON_WIDTH                       5
    #define WT_DROPLIST_SPACE                   9 
	#define WT_ICON_HIGHT                       11
	#define WT_ICON_LEFT                        9
	#define WT_UP_ICON_TOP                      (WT_UP_TEXT_TOP+7)
	#define WT_DOWN_ICON_TOP                    (WT_DOWN_TEXT_TOP+7)
	// TEXT
	#define WT_TEXT_WIDTH                       160
	#define WT_TEXT_HIGHT                       25
	#define WT_TEXT_LEFT                        18
	#define WT_UP_TEXT_TOP                      (219-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DOWN_TEXT_TOP                    (298-WT_STANDARD_KEY_H_OFFSET)
	#define WT_UP_WORLDDIFF_TOP                 (WT_UP_TEXT_TOP) 
	#define WT_DOWN_WORLDDIFF_TOP               (WT_DOWN_TEXT_TOP + 2)    
	#define WT_UP_WORLDTIME_TOP                 (WT_UP_TEXT_TOP + WT_TEXT_HIGHT + 15) 
	#define WT_DOWN_WORLDTIME_TOP               (WT_DOWN_TEXT_TOP + WT_TEXT_HIGHT + 15)    

    #define WT_WORLDTIME_LEFT                   15 
	#define WT_WORLDTIME_RIGHT                  175   
	
	#define WT_KEYSTOKE_LEFT_ARROW_LEFT       WT_KEYSTOKE_BG_LEFT
	#define WT_KEYSTOKE_LEFT_ARROW_TOP        WT_KEYSTOKE_BG_TOP
	#define WT_KEYSTOKE_RIGHT_ARROW_LEFT      120
	#define WT_KEYSTOKE_RIGHT_ARROW_TOP       WT_KEYSTOKE_BG_TOP

	#define WT_KEYSTOKE_ARROW_WIGTH           120
	#define WT_KEYSTOKE_ARROW_HIGHT           23
	//two line distance hight
	#define WT_TWOLINE_DISTANCE_HIGHT           MMI_DEFAULT_SMALL_FONT
	
	// ??±?±3?°????2?êy
	#define WT_TEXT_BG_LEFT                   8
	#define WT_TEXT_BG_TOP1                   (171-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_TOP2                   (215-WT_STANDARD_KEY_H_OFFSET)
	#define WT_TEXT_BG_WIDTH                  184
	#define WT_TEXT_BG_HIGHT                  25//@zhaohui,cr108515

	// ??à-2?μ￥?ü????2?êy
	#define WT_DROPDOWN_KEY_LEFT              160
	#define WT_DROPDOWN_KEY_TOP1              (174-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_TOP2              (219-WT_STANDARD_KEY_H_OFFSET)
	#define WT_DROPDOWN_KEY_WIDTH             20
	#define WT_DROPDOWN_KEY_HIGHT             19

	#define WT_TIME_DIFF_LEFT                 185
	#define WT_TIME_DIFF_RIGHT                225

	//counted time
	#define MMICT_TEXT_FONT	SONG_FONT_48
	#define CT_EDIT_MARGIN  5 //定义边框
	#define CT_BUTTON_MARGIN  4 //定义边框
	#define CT_BUTTON_HEIGHT_V  48
	#define CT_BUTTON_HEIGHT_H  42
	#define CT_PICKER_HEIGHT  60
	#define CT_PICKER_ACTIVE_FONT  SONG_FONT_45
	#define CT_PICKER_NORMAL_FONT  SONG_FONT_36

    	//stop watch
	#define MMIST_TIME1_OFFSET_Y_V  24
	#define MMIST_TIME1_OFFSET_Y_H  15
	#define MMIST_TIME2_OFFSET_Y_V  32
	#define MMIST_TIME2_OFFSET_Y_H  15
	#define ST_TIMER1_HEIGHT_V 20
	#define ST_TIMER1_HEIGHT_H 18
	#define ST_TIMER2_HEIGHT_V 56
	#define ST_TIMER2_HEIGHT_H 44
	#define ST_BUTTON_HEIGHT_V  48
	#define ST_BUTTON_HEIGHT_H  42
	#define ST_BUTTON_OFFSET_X_V  9
	#define ST_BUTTON_OFFSET_X_H  18
	#define ST_TIMER1_STR_OFFSET_X_V  (ST_BUTTON_OFFSET_X_V + 6)
	#define ST_TIMER1_STR_OFFSET_X_H  (ST_BUTTON_OFFSET_X_H + 6)
	#define ST_BUTTON_MARGIN  8
    
    #define CALENDAR_BLOCK_WIDTH_V                      45 
	#define CALENDAR_BLOCK_HEIGHT_V                     45
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_V             52
    #define CALENDAR_WEEKLY_DATE_Y_V     			    85 
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V		    {0, 20, 319, CALENDAR_WEEKLY_DATE_Y_V-1}
	#define CALENDAR_YEAR_TITLE_RECT_V 	                {65, 27, 479, 51}
    #define CALENDAR_MONTH_TITLE_RECT_V 	            {10, 27, 60, 75}
    #define CALENDAR_MONTH_LINE0_TOP_V                  115
    #define CALENDAR_MONTH_ITEM0_LEFT_V                 2
    #define CALENDAR_SCH_TEXT_OFFSET_V                  10
    #define CALENDAR_POINT_Y_V                          465
    #define CALENDAR_LUNAR_TITLE_RECT_V 	            {65, 52, 479, 75}
    #define CALENDAR_DAILY_START_Y_OFFSET_V             85
    #define CALENDAR_DAILY_TITLE_RECT_V 	            {70, 21, 250, CALENDAR_DAILY_START_Y_OFFSET_V-1}
    #define CALENDAR_DAILY_ITEM_HEIGH_V                 37
    #define CALENDAR_DAILY_MAX_ITEM_NUM_V               10
    #define CALENDAR_DAILY_TIME_RIGHT_V                 100
    #define CALENDAR_MONTH_WEEKLY_Y_V    				92
    
	#define CALENDAR_BLOCK_WIDTH_H                      45
	#define CALENDAR_BLOCK_HEIGHT_H                     30
	#define CALENDAR_WEEKLY_SELECT_HEIGHT_H   	        33  
    #define CALENDAR_WEEKLY_DATE_Y_H    				58
	#define CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H			{0,21,399,CALENDAR_WEEKLY_DATE_Y_H-1}
	#define CALENDAR_YEAR_TITLE_RECT_H				    {65, 28, 479, 52}
	#define CALENDAR_MONTH_TITLE_RECT_H 				{10, 28, 60, 76}
    #define CALENDAR_MONTH_LINE0_TOP_H                  110
    #define CALENDAR_MONTH_ITEM0_LEFT_H                 0
	#define CALENDAR_SCH_TEXT_OFFSET_H                  8
    #define CALENDAR_POINT_Y_H                          305
    #define CALENDAR_LUNAR_TITLE_RECT_H 	            {65, 53, 479, 76}
    #define CALENDAR_DAILY_START_Y_OFFSET_H             56
    #define CALENDAR_DAILY_TITLE_RECT_H 	            {100, 21, 380, CALENDAR_DAILY_START_Y_OFFSET_H-1}
    #define CALENDAR_DAILY_ITEM_HEIGH_H                 38
    #define CALENDAR_DAILY_MAX_ITEM_NUM_H               6
    #define CALENDAR_DAILY_TIME_RIGHT_H                 100
    #define CALENDAR_MONTH_WEEKLY_Y_H    				70
#ifdef VECTOR_FONT_SUPPORT_ETRUMP
    #define CALENDAR_ETRUMP_ADJUST_SIZE                 2
    #define CALENDAR_SCH_TEXT_FONT                      (SONG_FONT_20-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_MONTH_FONT                         (SONG_FONT_36-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_YEAR_FONT                          (SONG_FONT_26-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_DATE_FONT                          (SONG_FONT_24-CALENDAR_ETRUMP_ADJUST_SIZE)
    #define CALENDAR_WEEKLY_TITLE_FONT                  (SONG_FONT_26-CALENDAR_ETRUMP_ADJUST_SIZE)
#else
    #define CALENDAR_SCH_TEXT_FONT                      SONG_FONT_20
    #define CALENDAR_MONTH_FONT                         SONG_FONT_34
    #define CALENDAR_YEAR_FONT                          SONG_FONT_24
    #define CALENDAR_DATE_FONT                          SONG_FONT_22
    #define CALENDAR_WEEKLY_TITLE_FONT                  SONG_FONT_24
#endif
    #define CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H       (CALENDAR_BLOCK_HEIGHT_H-2)
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V             60
    #define CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H             50
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V     (CALENDAR_WEEKLY_SELECT_HEIGHT_V)
    #define CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H     (CALENDAR_WEEKLY_SELECT_HEIGHT_H-1)
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V      60
    #define CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H      50
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_H          (CALENDAR_DAILY_ITEM_HEIGH_H)
    #define CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_V          (CALENDAR_DAILY_ITEM_HEIGH_V)
 #ifdef  MMI_UNITCONVERSION_SUPPORT      
    #define MMIUC_FONT_SIZE  SONG_FONT_22
    #define MMIUC_OWNDRAW_HEIGHT  42    
    #define MMIUC_EDIT_FORM_HEIGHT  46      
    #define MMIUC_EDIT_MARGIN 3       
    #define MMIUC_FORM_VSAPCE  21
    #define MMIUC_FORM_HSAPCE  8    
    #define MMIUC_FORM_MARGIN  6    
    #define MMIUC_PICKER_HEIGHT_V  64
    #define MMIUC_PICKER_HEIGHT_H  50
    #define MMIUC_LABEL_WIDTH_PERCENT  25
#endif
#else
    #error	
#endif //_MMIACC_POSITION_H

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
