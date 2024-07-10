/*****************************************************************************
** File Name:      mmiidle_position.h                                        *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMIIDLE_POSITION_H_
#define _MMIIDLE_POSITION_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


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
#if  defined (MAINLCD_SIZE_128X160)
#define	MMIIDLE_SHORTCUT_HEIGHT				49	//the height of shortcut

//mission(up)
#define	MISSION_SHORTCUT_ANIM_TOP			60
#define	MISSION_SHORTCUT_STRING_GAPS		0	//the gaps of two string 

//funtion(down)
#define	FUNCTION_SHORTCUT_ANIM_TOP			115
#define	FUNCTION_SHORTCUT_ICON_HEIGHT		49	//the height of icon 
#define	FUNCTION_SHORTCUT_ICON_WIDTH		33	//the width of icon

#elif defined (MAINLCD_SIZE_176X220)
#define	MMIIDLE_SHORTCUT_HEIGHT				49	//the height of shortcut

//mission(up)
#define	MISSION_SHORTCUT_ANIM_TOP			60
#define	MISSION_SHORTCUT_STRING_GAPS		0	//the gaps of two string 

//funtion(down)
#define	FUNCTION_SHORTCUT_ANIM_TOP			115
#define	FUNCTION_SHORTCUT_ICON_HEIGHT		49	//the height of icon 
#define	FUNCTION_SHORTCUT_ICON_WIDTH		33	//the width of icon
#else
#define	MMIIDLE_SHORTCUT_HEIGHT				64	//the height of shortcut

//mission(up)
#define	MISSION_SHORTCUT_ANIM_TOP			134
#define	MISSION_SHORTCUT_STRING_GAPS		5	//the gaps of two string 

//funtion(down)
#define	FUNCTION_SHORTCUT_ANIM_TOP			200
#define	FUNCTION_SHORTCUT_ICON_HEIGHT		64	//the height of icon 
#define	FUNCTION_SHORTCUT_ICON_WIDTH		44	//the width of icon
#endif

#if  defined (MAINLCD_SIZE_128X160)
//margin space
#define MMIIDLE_MARGIN_WIDTH    1

#elif defined (MAINLCD_SIZE_176X220)
#define MMIIDLE_MARGIN_WIDTH    1

#else
//margin space
#define MMIIDLE_MARGIN_WIDTH    2
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif