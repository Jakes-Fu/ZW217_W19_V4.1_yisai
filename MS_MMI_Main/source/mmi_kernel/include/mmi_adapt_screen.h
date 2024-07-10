
/*****************************************************************************
** File Name:      mmi_adapt_sreen.h
** Author:         fulu.song
** Date:           2021.01.09
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.
** Description:    This file is used to describe macro and interface for dp to px, which 
                   used by auto resolution adaption.
******************************************************************************/
#ifndef __MMI_ADAPT_SCREEN_H__
#define __MMI_ADAPT_SCREEN_H__

#include "mmk_type.h"
#ifdef ADAPT_SCREEN_ENABLE
//only support basedp == 240*240 now-- 2021.01.09
#ifdef DP_SCREEN_SIZE_240X240
    #if PROJECT_SCREEN_SIZE_240X240
		#define DP2PX_K 1
		#define PX2DP_K 1
    #elif defined(PROJECT_SCREEN_SIZE_320X320)
        #define DP2PX_K 1.3333333
        #define PX2DP_K 0.75
    #elif defined(PROJECT_SCREEN_SIZE_360X360)
        #define DP2PX_K 1.5
        #define PX2DP_K 0.6666667
    #elif defined(PROJECT_SCREEN_SIZE_400X400)
        #define DP2PX_K 1.6666667
        #define PX2DP_K 0.6
    #elif defined(PROJECT_SCREEN_SIZE_128X160)
        #define DP2PX_K 0.533333
        #define PX2DP_K 1.875

    #else
        #error
    #endif
#else
	#error
#endif

#ifndef ROUND
#define ROUND(f) (int)((f)>0.0? (f+0.5):(f-0.5))
#endif

#define  DP2PX_VALUE(x)                                (ROUND(DP2PX_K * (x))) //DP to Pixel for value
#define  DP2PX_POINT(x, y)                            {DP2PX_VALUE(x), DP2PX_VALUE(y)} //DP to Pixel for point
#define  DP2PX_RECT(left, top, right, bottom) {DP2PX_VALUE(left), DP2PX_VALUE(top), DP2PX_VALUE(right), DP2PX_VALUE(bottom)} //DP to Pixel for rect
#define  DP2PX_FONT(a)                                  (DP2PX_VALUE(a) > SONG_FONT_64? SONG_FONT_64 : DP2PX_VALUE(a))  //DP to pixel for font 

#define  PX2DP_VALUE(x)                                (ROUND(PX2DP_K * (x)))                                 //Pixel to DP for value
#define  PX2DP_POINT(x, y)                            {PX2DP_VALUE(x), PX2DP_VALUE(y)}         //Pixel to DP for point
#define  PX2DP_RECT(left, top, right, bottom) {PX2DP_VALUE(left), PX2DP_VALUE(top), PX2DP_VALUE(right), PX2DP_VALUE(bottom)}            //Pixel to DP for rect
#else
#define  DP2PX_VALUE(x)                                (x)                                  //DP to Pixel for value
#define  DP2PX_POINT(x, y)                            {x, y}                               //DP to Pixel for point
#define  DP2PX_RECT(left, top, right, bottom)    {left, top, right, bottom}      //DP to Pixel for rect
#define  DP2PX_FONT(a)                                  (DP2PX_VALUE(a) > SONG_FONT_64? SONG_FONT_64 : DP2PX_VALUE(a))  //DP to pixel for font 

#define  PX2DP_VALUE(x)                                (x)                                  //Pixel to DP for value
#define  PX2DP_POINT(x, y)                            {x, y}                               //Pixel to DP for point
#define  PX2DP_RECT(left, top, right, bottom)    {left, top, right, bottom}     //Pixel to DP for rect
#endif

/*****************************************************************************/
//  Discription: convert fixed value from DP to PX
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIAdaptScreen_Dp2Px_Value(int16 dp);


/*****************************************************************************/
//  Discription: convert Point from DP to PX
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMIAdaptScreen_Dp2Px_Point(GUI_POINT_T point);


/*****************************************************************************/
//  Discription: convert Rect from DP to PX
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T  MMIAdaptScreen_Dp2Px_Rect(GUI_RECT_T rect);


/*****************************************************************************/
//  Discription: convert fixed value from PX to DP
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIAdaptScreen_Px2Dp_Value(int16 px);


/*****************************************************************************/
//  Discription: convert Point from PX to DP
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMIAdaptScreen_Px2Dp_Point(GUI_POINT_T point);


/*****************************************************************************/
//  Discription: convert Rect from PX to DP
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T  MMIAdaptScreen_Px2Dp_Rect(GUI_RECT_T rect);

#endif
