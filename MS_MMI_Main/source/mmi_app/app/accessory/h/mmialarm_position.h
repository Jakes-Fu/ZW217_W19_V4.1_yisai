/****************************************************************************
** File Name:      mmialarm_displaysize.h                            
** Author:                                                                  
** Date:            2007/04/29
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to be included by  the file mmialarm.c
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/29/2007     yongwei.he          Create
** 
****************************************************************************/
#ifndef _MMIALARM_POSITION_H     
#define _MMIALARM_POSITION_H

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

#define ALM_EDIT_MODE_LIST_LEFT     ALM_EDIT_MODE_LIST_X
#define ALM_EDIT_MODE_LIST_RIGHT    (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - ALM_EDIT_MODE_LIST_X)
#ifdef MAINLCD_SIZE_128X160
	//176*220的数据
	#define ALM_PROMPT_ALM_TXTBG_LEFT           4
	#define ALM_PROMPT_ALM_TXTBG_TOP            18//175 // 18

	#define ALM_PROMPT_ALM_TEXT_LEFT            4
	#define ALM_PROMPT_ALM_TEXT_TOP             (18+4)//175 // (18+4)
	#define ALM_PROMPT_ALM_TEXT_RIGHT           171//239 // 171

	#define ALM_PROMPT_ALM_IMAGE_LEFT           4//0   // 4
	#define ALM_PROMPT_ALM_IMAGE_TOP            66//39  // 66
	#define ALM_PROMPT_ALM_IMAGE_RIGHT          127//240 // 171
	#define ALM_PROMPT_ALM_IMAGE_BOTTOM         159//175 // 189

	#define ALM_PROMPT_SCH_TXTBG_LEFT           4
	#define ALM_PROMPT_SCH_TXTBG_TOP            18//175 // 18

	// 编辑窗口的控件位置信息 begin
	#define ALM_EDIT_HIGHT   16

	#define ALM_EDIT_X1      7//12
	#define ALM_EDIT_X2      65//50//63
	#define ALM_EDIT_X3      150//150//197

	#define ALM_EDIT_Y0	     MMI_TITLE_HEIGHT+1
	#define ALM_EDIT_Y1     ALM_EDIT_Y0+22// 29//39//53
	#define ALM_EDIT_Y2      ALM_EDIT_Y1+22//54//63//82
	#define ALM_EDIT_Y3      ALM_EDIT_Y2+20//79//85//112
	#define ALM_EDIT_Y4      ALM_EDIT_Y3+19//99//105//140
	#define ALM_EDIT_LIST_BOTTOM  159

	#define ALM_EDIT_MODE_X			(ALM_EDIT_X1 + 4)
	#define ALM_EDIT_MODE_LIST_X		(ALM_EDIT_MODE_X + 17)
	#define ALM_EDIT_ONCE_Y			(ALM_EDIT_Y3 + 2 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYDAY_Y		(ALM_EDIT_ONCE_Y + 5 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYWEEK_Y		(ALM_EDIT_EVERYDAY_Y + 5 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_PONIT_ONCE_Y		(ALM_EDIT_ONCE_Y + 3)
	#define ALM_EDIT_PONIT_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 3)
	#define ALM_EDIT_PONIT_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 3)
	#define ALM_EDIT_LIST_ONCE_Y		(ALM_EDIT_ONCE_Y + 4)
	#define ALM_EDIT_LIST_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 4)
	#define ALM_EDIT_LIST_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 4)


//	#define ALM_EDIT_FOLDER_LEFT  149//155//202
	#define ALM_EDIT_FOLDER_WIDTH  21
	#define ALM_EDIT_FOLDER_HIGHT  17
	// button控件的信息
	#define ALM_EDIT_BUTTON_HIGHT  24//42
	#define ALM_EDIT_BUTTON_WIDTH 24//34


	#define ALM_EDIT_RADIO_HIGHT 16//13
	#define ALM_EDIT_RADIO_WIDTH 16//13

	#define ALM_WEEK_DAY_Y 127
	#define ALM_WEEK_SUN_X 5
	#define ALM_WEEK_MON_X 29
	#define ALM_WEEK_TUE_X 53
	#define ALM_WEEK_WED_X 77
	#define ALM_WEEK_THU_X 101
	#define ALM_WEEK_FRI_X 125
	#define ALM_WEEK_SAT_X 149
#elif MAINLCD_SIZE_176X220
	//176*220的数据
	#define ALM_PROMPT_ALM_TXTBG_LEFT           4
	#define ALM_PROMPT_ALM_TXTBG_TOP            18//175 // 18

	#define ALM_PROMPT_ALM_TEXT_LEFT            4
	#define ALM_PROMPT_ALM_TEXT_TOP             (18+4)//175 // (18+4)
	#define ALM_PROMPT_ALM_TEXT_RIGHT           171//239 // 171

	#define ALM_PROMPT_ALM_IMAGE_LEFT           4//0   // 4
	#define ALM_PROMPT_ALM_IMAGE_TOP            66//39  // 66
	#define ALM_PROMPT_ALM_IMAGE_RIGHT          171//240 // 171
	#define ALM_PROMPT_ALM_IMAGE_BOTTOM         189//175 // 189

	#define ALM_PROMPT_SCH_TXTBG_LEFT           4
	#define ALM_PROMPT_SCH_TXTBG_TOP            18//175 // 18

	// 编辑窗口的控件位置信息 begin
	#define ALM_EDIT_HIGHT   16

	#define ALM_EDIT_X1      7//12
	#define ALM_EDIT_X2      65//50//63
	#define ALM_EDIT_X3      165//150//197

	#define ALM_EDIT_Y0	     MMI_TITLE_HEIGHT+1
	#define ALM_EDIT_Y1     ALM_EDIT_Y0+22// 29//39//53
	#define ALM_EDIT_Y2      ALM_EDIT_Y1+22//54//63//82
	#define ALM_EDIT_Y3      ALM_EDIT_Y2+20//79//85//112
	#define ALM_EDIT_Y4      ALM_EDIT_Y3+19//99//105//140
	#define ALM_EDIT_LIST_BOTTOM  220

	#define ALM_EDIT_MODE_X			(ALM_EDIT_X1 + 4)
	#define ALM_EDIT_MODE_LIST_X		(ALM_EDIT_MODE_X + 17)
	#define ALM_EDIT_ONCE_Y			(ALM_EDIT_Y3 + 2 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYDAY_Y		(ALM_EDIT_ONCE_Y + 5 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYWEEK_Y		(ALM_EDIT_EVERYDAY_Y + 5 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_PONIT_ONCE_Y		(ALM_EDIT_ONCE_Y + 3)
	#define ALM_EDIT_PONIT_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 3)
	#define ALM_EDIT_PONIT_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 3)
	#define ALM_EDIT_LIST_ONCE_Y		(ALM_EDIT_ONCE_Y + 4)
	#define ALM_EDIT_LIST_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 4)
	#define ALM_EDIT_LIST_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 4)


//	#define ALM_EDIT_FOLDER_LEFT  149//155//202
	#define ALM_EDIT_FOLDER_WIDTH  21
	#define ALM_EDIT_FOLDER_HIGHT  17
	// button控件的信息
	#define ALM_EDIT_BUTTON_HIGHT 24//42
	#define ALM_EDIT_BUTTON_WIDTH 24//34


	#define ALM_EDIT_RADIO_HIGHT 16//13
	#define ALM_EDIT_RADIO_WIDTH 16//13

	#define ALM_WEEK_DAY_Y 171
	#define ALM_WEEK_SUN_X 5
	#define ALM_WEEK_MON_X 29
	#define ALM_WEEK_TUE_X 53
	#define ALM_WEEK_WED_X 77
	#define ALM_WEEK_THU_X 101
	#define ALM_WEEK_FRI_X 125
	#define ALM_WEEK_SAT_X 149
#elif defined MAINLCD_SIZE_240X320
	#define ALM_PROMPT_ALM_TXTBG_LEFT           4
	#define ALM_PROMPT_ALM_TXTBG_TOP            175 // 18

	#define ALM_PROMPT_ALM_TEXT_LEFT            4
	#define ALM_PROMPT_ALM_TEXT_TOP             175 // (18+4)
	#define ALM_PROMPT_ALM_TEXT_RIGHT           239 // 171
	#define ALM_PROMPT_ALM_TEXT_BOTTOM          224 // MMI_CLIENT_RECT_BOTTOM //(66-4)

	#define ALM_PROMPT_ALM_IMAGE_LEFT           0   // 4
	#define ALM_PROMPT_ALM_IMAGE_TOP            39  // 66
	#define ALM_PROMPT_ALM_IMAGE_RIGHT          240 // 171
	#define ALM_PROMPT_ALM_IMAGE_BOTTOM         175 // 189

	#define ALM_PROMPT_SCH_TXTBG_LEFT           4
	#define ALM_PROMPT_SCH_TXTBG_TOP            175 // 18

	// 编辑窗口的控件位置信息 begin
	#define ALM_EDIT_HIGHT				24

	#define ALM_EDIT_X1					10//12
	#define ALM_EDIT_X2					94//63  //expand for arabic long string
	#define ALM_EDIT_X3					231//197 //expand for arabic long string

	#define ALM_EDIT_Y0					29
	#define ALM_EDIT_Y1					58
	#define ALM_EDIT_Y2					87
	#define ALM_EDIT_Y3					117
	#define ALM_EDIT_Y4					140
	#define ALM_EDIT_LIST_BOTTOM		220

	#define ALM_EDIT_MODE_X			(ALM_EDIT_X1 + 4)
	#define ALM_EDIT_MODE_LIST_X		(ALM_EDIT_MODE_X + 25)
	#define ALM_EDIT_ONCE_Y			(ALM_EDIT_Y3 + 10 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYDAY_Y		(ALM_EDIT_ONCE_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYWEEK_Y		(ALM_EDIT_EVERYDAY_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_PONIT_ONCE_Y		(ALM_EDIT_ONCE_Y + 5)
	#define ALM_EDIT_PONIT_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 5)
	#define ALM_EDIT_PONIT_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 5)
	#define ALM_EDIT_LIST_ONCE_Y		(ALM_EDIT_ONCE_Y)
	#define ALM_EDIT_LIST_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 1)
	#define ALM_EDIT_LIST_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 3)

//	#define ALM_EDIT_FOLDER_LEFT  155//202
	#define ALM_EDIT_FOLDER_WIDTH  24
	#define ALM_EDIT_FOLDER_HIGHT  21
	// button控件的信息
	#define ALM_EDIT_BUTTON_HIGHT		42
	#define ALM_EDIT_BUTTON_WIDTH		34

	#define ALM_EDIT_RADIO_HIGHT		20
	#define ALM_EDIT_RADIO_WIDTH		20

	#define ALM_WEEK_DAY_Y 238
	#define ALM_WEEK_SUN_X 4
	#define ALM_WEEK_MON_X 37
	#define ALM_WEEK_TUE_X 70
	#define ALM_WEEK_WED_X 103
	#define ALM_WEEK_THU_X 136
	#define ALM_WEEK_FRI_X 169
	#define ALM_WEEK_SAT_X 202  
	
#elif defined MAINLCD_SIZE_128X64
	#define ALM_PROMPT_ALM_TXTBG_LEFT           4
	#define ALM_PROMPT_ALM_TXTBG_TOP            175 // 18

	#define ALM_PROMPT_ALM_TEXT_LEFT            4
	#define ALM_PROMPT_ALM_TEXT_TOP             175 // (18+4)
	#define ALM_PROMPT_ALM_TEXT_RIGHT           239 // 171
	#define ALM_PROMPT_ALM_TEXT_BOTTOM          224 // MMI_CLIENT_RECT_BOTTOM //(66-4)

	#define ALM_PROMPT_ALM_IMAGE_LEFT           0   // 4
	#define ALM_PROMPT_ALM_IMAGE_TOP            39  // 66
	#define ALM_PROMPT_ALM_IMAGE_RIGHT          240 // 171
	#define ALM_PROMPT_ALM_IMAGE_BOTTOM         175 // 189

	#define ALM_PROMPT_SCH_TXTBG_LEFT           4
	#define ALM_PROMPT_SCH_TXTBG_TOP            175 // 18

	// 编辑窗口的控件位置信息 begin
	#define ALM_EDIT_HIGHT				14    //note:support default font is SONG_FONT_14;

	#define ALM_EDIT_X0				0   // currently we don't use this position for the zero line lable.
	#define ALM_EDIT_X1					(ALM_EDIT_X0)//the start x position for alingn left lable.
	#define ALM_EDIT_X2					70  //the start x position for align right lable.
	#define ALM_EDIT_X3					(MMI_MAINSCREEN_RIGHT_MAX_PIXEL-2) //the end x position for alingn right lable. 

	#define ALM_EDIT_Y0					(1)   
	#define ALM_EDIT_Y1					(ALM_EDIT_Y0)  // the start y position for 1st line lable.
	#define ALM_EDIT_Y2					(ALM_EDIT_Y1+ALM_EDIT_HIGHT +2)//the start y position for second line lable.
	#define ALM_EDIT_Y3					(ALM_EDIT_Y2+ALM_EDIT_HIGHT+2)//the start y position for 3rd line lable.
	#define ALM_EDIT_Y4					(ALM_EDIT_Y3+ALM_EDIT_HIGHT+2)
    
    //2 below value need change by application.
	#define ALM_EDIT_LIST_BOTTOM		220

	#define ALM_EDIT_MODE_X			(ALM_EDIT_X1 + 4)
	#define ALM_EDIT_MODE_LIST_X		(ALM_EDIT_MODE_X + 25)
	#define ALM_EDIT_ONCE_Y			(ALM_EDIT_Y3 + 10 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYDAY_Y		(ALM_EDIT_ONCE_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYWEEK_Y		(ALM_EDIT_EVERYDAY_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_PONIT_ONCE_Y		(ALM_EDIT_ONCE_Y + 5)
	#define ALM_EDIT_PONIT_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 5)
	#define ALM_EDIT_PONIT_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 5)
	#define ALM_EDIT_LIST_ONCE_Y		(ALM_EDIT_ONCE_Y)
	#define ALM_EDIT_LIST_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 1)
	#define ALM_EDIT_LIST_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 3)

//	#define ALM_EDIT_FOLDER_LEFT  155//202
	#define ALM_EDIT_FOLDER_WIDTH  24
	#define ALM_EDIT_FOLDER_HIGHT  21
	// button控件的信息
	#define ALM_EDIT_BUTTON_HIGHT		42
	#define ALM_EDIT_BUTTON_WIDTH		34

	#define ALM_EDIT_RADIO_HIGHT		20
	#define ALM_EDIT_RADIO_WIDTH		20

	#define ALM_WEEK_DAY_Y 308//
	#define ALM_WEEK_SUN_X 4
	#define ALM_WEEK_MON_X 37
	#define ALM_WEEK_TUE_X 70
	#define ALM_WEEK_WED_X 103
	#define ALM_WEEK_THU_X 136
	#define ALM_WEEK_FRI_X 169
	#define ALM_WEEK_SAT_X 202

#elif defined MAINLCD_SIZE_240X400
	#define ALM_PROMPT_ALM_TXTBG_LEFT           4
	#define ALM_PROMPT_ALM_TXTBG_TOP            175 // 18

	#define ALM_PROMPT_ALM_TEXT_LEFT            4
	#define ALM_PROMPT_ALM_TEXT_TOP             175 // (18+4)
	#define ALM_PROMPT_ALM_TEXT_RIGHT           239 // 171
	#define ALM_PROMPT_ALM_TEXT_BOTTOM          224 // MMI_CLIENT_RECT_BOTTOM //(66-4)

	#define ALM_PROMPT_ALM_IMAGE_LEFT           0   // 4
	#define ALM_PROMPT_ALM_IMAGE_TOP            39  // 66
	#define ALM_PROMPT_ALM_IMAGE_RIGHT          240 // 171
	#define ALM_PROMPT_ALM_IMAGE_BOTTOM         175 // 189

	#define ALM_PROMPT_SCH_TXTBG_LEFT           4
	#define ALM_PROMPT_SCH_TXTBG_TOP            175 // 18

	// 编辑窗口的控件位置信息 begin
	#define ALM_EDIT_HIGHT				24

	#define ALM_EDIT_X1					10//12
	#define ALM_EDIT_X2					94//63  //expand for arabic long string
	#define ALM_EDIT_X3					231//197 //expand for arabic long string

	#define ALM_EDIT_Y0					(MMI_TITLE_HEIGHT + 10)//29
	#define ALM_EDIT_Y1					(ALM_EDIT_Y0+35)//58
	#define ALM_EDIT_Y2					(ALM_EDIT_Y1+35)//87
	#define ALM_EDIT_Y3					(ALM_EDIT_Y2+55)//117
	#define ALM_EDIT_Y4					(ALM_EDIT_Y3+35)//140
	#define ALM_EDIT_LIST_BOTTOM		220

	#define ALM_EDIT_MODE_X			(ALM_EDIT_X1 + 4)
	#define ALM_EDIT_MODE_LIST_X		(ALM_EDIT_MODE_X + 25)
	#define ALM_EDIT_ONCE_Y			(ALM_EDIT_Y3 + 10 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYDAY_Y		(ALM_EDIT_ONCE_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYWEEK_Y		(ALM_EDIT_EVERYDAY_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_PONIT_ONCE_Y		(ALM_EDIT_ONCE_Y + 5)
	#define ALM_EDIT_PONIT_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 5)
	#define ALM_EDIT_PONIT_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 5)
	#define ALM_EDIT_LIST_ONCE_Y		(ALM_EDIT_ONCE_Y)
	#define ALM_EDIT_LIST_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 1)
	#define ALM_EDIT_LIST_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 3)

//	#define ALM_EDIT_FOLDER_LEFT  155//202
	#define ALM_EDIT_FOLDER_WIDTH  24
	#define ALM_EDIT_FOLDER_HIGHT  21
	// button控件的信息
	#define ALM_EDIT_BUTTON_HIGHT		42
	#define ALM_EDIT_BUTTON_WIDTH		34

	#define ALM_EDIT_RADIO_HIGHT		20
	#define ALM_EDIT_RADIO_WIDTH		20

	#define ALM_WEEK_DAY_Y 308//
	#define ALM_WEEK_SUN_X 4
	#define ALM_WEEK_MON_X 37
	#define ALM_WEEK_TUE_X 70
	#define ALM_WEEK_WED_X 103
	#define ALM_WEEK_THU_X 136
	#define ALM_WEEK_FRI_X 169
	#define ALM_WEEK_SAT_X 202

#elif defined MAINLCD_SIZE_320X480
	#define ALM_PROMPT_ALM_TXTBG_LEFT           4
	#define ALM_PROMPT_ALM_TXTBG_TOP            175 // 18

	#define ALM_PROMPT_ALM_TEXT_LEFT            4
	#define ALM_PROMPT_ALM_TEXT_TOP             175 // (18+4)
	#define ALM_PROMPT_ALM_TEXT_RIGHT           239 // 171
	#define ALM_PROMPT_ALM_TEXT_BOTTOM          224 // MMI_CLIENT_RECT_BOTTOM //(66-4)

	#define ALM_PROMPT_ALM_IMAGE_LEFT           0   // 4
	#define ALM_PROMPT_ALM_IMAGE_TOP            39  // 66
	#define ALM_PROMPT_ALM_IMAGE_RIGHT          240 // 171
	#define ALM_PROMPT_ALM_IMAGE_BOTTOM         175 // 189

	#define ALM_PROMPT_SCH_TXTBG_LEFT           4
	#define ALM_PROMPT_SCH_TXTBG_TOP            175 // 18

	// 编辑窗口的控件位置信息 begin
	#define ALM_EDIT_HIGHT				24

	#define ALM_EDIT_X1					10//12
	#define ALM_EDIT_X2					94//63  //expand for arabic long string
	#define ALM_EDIT_X3					231//197 //expand for arabic long string

	#define ALM_EDIT_Y0					(MMI_TITLE_HEIGHT + 10)//29
	#define ALM_EDIT_Y1					(ALM_EDIT_Y0+35)//58
	#define ALM_EDIT_Y2					(ALM_EDIT_Y1+35)//87
	#define ALM_EDIT_Y3					(ALM_EDIT_Y2+55)//117
	#define ALM_EDIT_Y4					(ALM_EDIT_Y3+35)//140
	#define ALM_EDIT_LIST_BOTTOM		220

	#define ALM_EDIT_MODE_X			(ALM_EDIT_X1 + 4)
	#define ALM_EDIT_MODE_LIST_X		(ALM_EDIT_MODE_X + 25)
	#define ALM_EDIT_ONCE_Y			(ALM_EDIT_Y3 + 10 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYDAY_Y		(ALM_EDIT_ONCE_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_EVERYWEEK_Y		(ALM_EDIT_EVERYDAY_Y + 6 + ALM_EDIT_HIGHT)
	#define ALM_EDIT_PONIT_ONCE_Y		(ALM_EDIT_ONCE_Y + 5)
	#define ALM_EDIT_PONIT_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 5)
	#define ALM_EDIT_PONIT_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 5)
	#define ALM_EDIT_LIST_ONCE_Y		(ALM_EDIT_ONCE_Y)
	#define ALM_EDIT_LIST_EVERYDAY_Y		(ALM_EDIT_EVERYDAY_Y + 1)
	#define ALM_EDIT_LIST_EVERYWEEK_Y		(ALM_EDIT_EVERYWEEK_Y + 3)

//	#define ALM_EDIT_FOLDER_LEFT  155//202
	#define ALM_EDIT_FOLDER_WIDTH  24
	#define ALM_EDIT_FOLDER_HIGHT  21
	// button控件的信息
	#define ALM_EDIT_BUTTON_HIGHT		42
	#define ALM_EDIT_BUTTON_WIDTH		34

	#define ALM_EDIT_RADIO_HIGHT		20
	#define ALM_EDIT_RADIO_WIDTH		20

	#define ALM_WEEK_DAY_Y 308//
	#define ALM_WEEK_SUN_X 4
	#define ALM_WEEK_MON_X 37
	#define ALM_WEEK_TUE_X 70
	#define ALM_WEEK_WED_X 103
	#define ALM_WEEK_THU_X 136
	#define ALM_WEEK_FRI_X 169
	#define ALM_WEEK_SAT_X 202
#else
    #error	 	
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIALARM_POSITION_H