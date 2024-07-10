/****************************************************************************
** File Name:      mmisrv_adapt_gui.h                                               *
** Author:                                                                 *
** Date:           07/26/2012                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the mmi service adapt     *
**                 (为了不破坏mmisrv模块独立性，mmisrv中调用的gui的函数声明）                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/26/2012        bin.ji       Create
** 
****************************************************************************/

#ifndef _MMISRV_ADAPT_GUI_H_
#define _MMISRV_ADAPT_GUI_H_

#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_font.h"

//typedef uint32 MMISRV_HANDLE_T;

PUBLIC uint8 GUI_GetFontWidth(//
                        GUI_FONT_T	font_type,      //×??aààDí
                        uint16 unicode
                        );

PUBLIC uint16 GUI_Divide(
                         uint16 a,
                         uint16 b
                         );
                         
PUBLIC uint32 GUI_GBToWstr(					//return the number of UCS2 code that have been converted.
                           wchar *wstr_ptr, 	//destination: string pointer to UCS2 string.
                           const uint8 *gb_ptr, //source: string pointer to GB string
                           uint32 str_len			//the length to convert from GB to UCS2.
                           );                         
                            
PUBLIC uint32 GUI_UCS2L2UCS2B(//ucs2b len
                              uint8 *ucs2b_ptr,//out
                              uint32 ucs2b_buf_len,//in
                              const uint8 *ucs2l_ptr,//in
                              uint32 ucs2l_len//in
                              );   
                              
PUBLIC uint32 GUI_UCS2B2UCS2L(//ucs2l len
                              uint8 *ucs2l_ptr,//out
                              uint32 ucs2l_buf_len,//in
                              const uint8 *ucs2b_ptr,//in
                              uint32 ucs2b_len//in
                              );   
                              
PUBLIC BOOLEAN GUI_IsLeapYear(
                              uint32 year
                              );
                              
PUBLIC int32 GUI_StringCompare(
                               uint8				*str1,
                               uint16 				str1_len,
                               uint8				*str2,
                               uint16 				str2_len
                               );   
                               
PUBLIC BOOLEAN GUIRES_DisplayImg(
                                 GUI_POINT_T            *display_point_ptr, //in:may PNULL′ú±í???¨rect??óò??ê?
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL′ú±í???¨?eê?μ???ê?
                                 GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL′ú±í′óí???(0,0)μ???ê?
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 );
                                                                                                                                                                             
#endif

