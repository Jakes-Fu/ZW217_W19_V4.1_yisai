/*****************************************************************************
** File Name:      mmieng_position.h                                         *
** Author:                                                                   *
** Date:           13/04/2007                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007     gang.tong              Creat
******************************************************************************/
#ifndef _MMIENG_POSITION_H_
#define _MMIENG_POSITION_H_

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
#if defined (MAINLCD_SIZE_128X160)
#ifdef MAINLCD_DEV_SIZE_128X160
	#define MMIENG_TESTPAD_MARGIN_X			5
	#define MMIENG_TESTPAD_MARGIN_Y			4
	#define MMIENG_TESTPAD_KEY_HIGHT		25
#else //defined (MAINLCD_DEV_SIZE_160x128)
	#define MMIENG_TESTPAD_MARGIN_X			0
	#define MMIENG_TESTPAD_MARGIN_Y			0
	#define MMIENG_TESTPAD_KEY_HIGHT		26
#endif
#elif defined (MAINLCD_SIZE_176X220)
	#define MMIENG_TESTPAD_MARGIN_X			5
	#define MMIENG_TESTPAD_MARGIN_Y			4
	#define MMIENG_TESTPAD_KEY_HIGHT		25
#elif defined MAINLCD_SIZE_240X320
	#define MMIENG_TESTPAD_MARGIN_X			8
	#define MMIENG_TESTPAD_MARGIN_Y			8
	#define MMIENG_TESTPAD_KEY_HIGHT		40
#elif defined MAINLCD_SIZE_128X64
	#define MMIENG_TESTPAD_MARGIN_X			8
	#define MMIENG_TESTPAD_MARGIN_Y			8
	#define MMIENG_TESTPAD_KEY_HIGHT		50
#elif defined MAINLCD_SIZE_240X400
	#define MMIENG_TESTPAD_MARGIN_X			8
	#define MMIENG_TESTPAD_MARGIN_Y			8
	#define MMIENG_TESTPAD_KEY_HIGHT		50
#elif defined MAINLCD_SIZE_320X480
	#define MMIENG_TESTPAD_MARGIN_X			8
	#define MMIENG_TESTPAD_MARGIN_Y			8
	#define MMIENG_TESTPAD_KEY_HIGHT		50
#else
    #error	
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UITEST_WIN_SEPERATE	80

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
