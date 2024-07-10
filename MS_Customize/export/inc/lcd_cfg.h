/******************************************************************************
 ** File Name:     lcd_cfg.h                                                  *
 ** Description:														 	  *
 ** This file contains main parameter for lcd.						      	  *
 ** Author:         Jim zhang                                                 *
 ** DATE:           03/09/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2004     Jimmy.Jia        Create for SP7100EU						  *
 ******************************************************************************/
#ifndef _LCD_CFG_H_
#define _LCD_CFG_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines
 **---------------------------------------------------------------------------*/
#if defined MAINLCD_DEV_SIZE_128X160
#define LCD_WIDTH      128
// #define LCD_HEIGHT     160
#define LCD_HEIGHT     128
#elif defined MAINLCD_DEV_SIZE_160X128
#define LCD_WIDTH      160
#define LCD_HEIGHT     128
#elif defined MAINLCD_DEV_SIZE_176X220
#define LCD_WIDTH      176
#define LCD_HEIGHT     220
#elif defined MAINLCD_DEV_SIZE_240X400
#define LCD_WIDTH      240
#define LCD_HEIGHT     400
#elif defined MAINLCD_DEV_SIZE_240X320
#define LCD_WIDTH      240
#if defined MMI_WATCH_UI
#define LCD_HEIGHT     240
#else
#define LCD_HEIGHT     320
#endif
#elif defined MAINLCD_DEV_SIZE_128X64
#define LCD_WIDTH      128
#define LCD_HEIGHT     64
#elif defined MAINLCD_DEV_SIZE_320X480
#define LCD_WIDTH      320
#define LCD_HEIGHT     480
#elif defined MAINLCD_DEV_SIZE_64X128
#define LCD_WIDTH      128
#define LCD_HEIGHT     64
#elif defined MAINLCD_DEV_SIZE_220X176
#define LCD_WIDTH      220
#define LCD_HEIGHT     176
#elif defined MAINLCD_DEV_SIZE_400X240
#define LCD_WIDTH      400
#define LCD_HEIGHT     240
#elif defined MAINLCD_DEV_SIZE_320X240
#define LCD_WIDTH      320
#define LCD_HEIGHT     240
#elif defined MAINLCD_DEV_SIZE_480X320
#define LCD_WIDTH      480
#define LCD_HEIGHT     320
#elif defined MAINLCD_DEV_SIZE_128X64
#define LCD_WIDTH      64
#define LCD_HEIGHT     128
#elif  defined(DRV_LCD_7789P3_240X284)||defined(W217_W18_DRV_LCD_7789P3_240X284)||defined(MAINLCD_DEV_SIZE_240X284)
#define LCD_WIDTH      240
#define LCD_HEIGHT     284 // wuxx modify
#elif defined MAINLCD_DEV_SIZE_240X240
#define LCD_WIDTH      240
#define LCD_HEIGHT     240
#else
#define LCD_WIDTH      1
#define LCD_HEIGHT     1
#endif


#if defined SUBLCD_DEV_SIZE_96X64
#define SUBLCD_WIDTH   96
#define SUBLCD_HEIGHT  64
#elif defined SUBLCD_DEV_SIZE_96X96
#define SUBLCD_WIDTH   96
#define SUBLCD_HEIGHT  96
#else
#define SUBLCD_WIDTH   1
#define SUBLCD_HEIGHT  1
#endif

#define QVGA_LCD_WIDTH  	240
#if   defined(DRV_LCD_7789P3_240X284)||defined(W217_W18_DRV_LCD_7789P3_240X284)||defined(MAINLCD_DEV_SIZE_240X284)
#define QVGA_LCD_HEIGHT 	284
#else
#define QVGA_LCD_HEIGHT 	320
#endif 

#define WQVGA_LCD_WIDTH     (240)
#if   defined(DRV_LCD_7789P3_240X284)||defined(W217_W18_DRV_LCD_7789P3_240X284)||defined(MAINLCD_DEV_SIZE_240X284)
#define WQVGA_LCD_HEIGHT    (284)
#else
#define WQVGA_LCD_HEIGHT    (400)
#endif


/**--------------------------------------------------------------------------*
 **                         Data Structures                                  *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
// end lcd_cfg.c
