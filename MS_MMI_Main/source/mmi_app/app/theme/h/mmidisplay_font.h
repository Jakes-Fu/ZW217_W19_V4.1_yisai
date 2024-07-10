/*****************************************************************************
** File Name:      mmidisplay_font.h                                         *
** Author:                                                                   *
** Date:           02/20/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define font macro                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      fangfang.yao       Create                                    *
******************************************************************************/

#ifndef _MMIDISPLAY_FONT_H_
#define _MMIDISPLAY_FONT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#if (defined DP_SCREEN_SIZE_240X240) || (defined MAINLCD_DEV_SIZE_240X320) || (defined MAINLCD_DEV_SIZE_240X240)
    #include "mmidisplay_font_240x320.h"
#elif defined MAINLCD_DEV_SIZE_128X160
    #include "mmidisplay_font_128x160.h"
#else
    #error
#endif

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
// list number font type
#define MMI_LISTNUM_FONT                        SONG_FONT_10        // 显示在右上角的ListNumber信息的字体

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
