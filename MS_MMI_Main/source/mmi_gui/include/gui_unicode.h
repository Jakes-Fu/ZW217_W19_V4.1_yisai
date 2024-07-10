/*****************************************************************************
** File Name:      gui_unicode.h                                               *
** Author:                                                                   *
** Date:           03/13/2007                                                *
** Copyright:      2003 Spreadtrum,  Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe gui textbox control         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       wancan.you          Create                                   *
******************************************************************************/

#ifndef _GUI_UNICODE_H_
#define _GUI_UNICODE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include    "sci_types.h"



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
 /**Unicode define**/
/*Latin*/
#define BASIC_LATIN_START       0x0000  //ascii
#define BASIC_LATIN_END         0x007F

#define BASIC_LATIN_1_START     0x0080  //Latin-1 iso 8859-1
#define BASIC_LATIN_1_END       0x00FF

#define LATIN_EXT_A_START       0x0100
#define LATIN_EXT_A_END         0x017F

#define LATIN_EXT_B_START       0x0180
#define LATIN_EXT_B_END         0x024F

#define LATIN_EXT_C_START       0x2C60
#define LATIN_EXT_C_END         0x2CFF

#define LATIN_EXT_D_START       0xA720
#define LATIN_EXT_D_END         0xA7FF

#define LATIN_EXT_ADD_START     0x1E00
#define LATIN_EXT_ADD_END       0x1EFF

#define LATIN_LIGATURE_START    0xFB00
#define LATIN_LIGATURE_END      0xFB4F

#define LATIN_SMALL_FORM_START  0xFE50
#define LATIN_SMALL_FORM_END    0xFE6F

#define CURRENCY_SYMBOLS_START  0x20A0
#define CURRENCY_SYMBOLS_END    0x20CF

/*Greek*/
#define GREEK_UNICODE_START     0x0370
#define GREEK_UNICODE_END       0x03FF

#define GREEK_EXT_UNICODE_START     0x1F00
#define GREEK_EXT_UNICODE_END       0x1FFF

/*Cyrillic(Russian)*/
#define CYRILLIC_UNICODE_START      0x0400
#define CYRILLIC_UNICODE_END        0x04FF

#define CYRILLIC_SUPPLEMENT_STRAT   0x0500
#define CYRILLIC_SUPPLEMENT_END     0x052F

/*CJK(Chinese Japanese Korea)*/
#define CJK_UNICODE_START 0x4E00
#define CJK_UNICODE_END 0x9FBF

#define CJK_PUNCTUATION_START 0x3000
#define CJK_PUNCTUATIONE_END 0x303F

#define CJK_BOPOMOFO_START 0x3100
#define CJK_BOPOMOFO_END     0x312F
/*end of Unicode define*/

/*unicode char width defined*/
#define GUI_ASCII_CHAR_WIDTH        1
#define GUI_LATIN_CHAR_WIDTH        GUI_ASCII_CHAR_WIDTH
#define GUI_CYRILLIC_CHAR_WIDTH     GUI_ASCII_CHAR_WIDTH
#define GUI_CJK_CGAR_WIDTH              (2*GUI_ASCII_CHAR_WIDTH)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
// 	Description : This function Check is Ascii Character
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsAsciiCode(uint32 unicode);

/*****************************************************************************/
// 	Description : This function Check is Latin Character
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsLatinUnicode(uint32 unicode);

/*****************************************************************************/
// 	Description : This function Get Latin font Index
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_GetLatinFontIndex(uint32 unicode);

/*****************************************************************************/
// 	Description : This function Check is Cyrillic Unicode(Including Russian)
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsCyrillicUnicode(uint32 unicode);

/*****************************************************************************/
// 	Description : This function Check is Greek Unicode
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsGreekUnicode(uint32 unicode);

/*****************************************************************************/
// 	Description : This function Check is CJK Unicode
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsCJKUnicode(uint32 unicode);

/*****************************************************************************/
// 	Description : This function Get Cyrillic font Index
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_GetCyrillicFontIndex(uint32 unicode);

/*****************************************************************************/
// 	Description : This function Check is Char One Ascii Char Width?
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsCharOneAsciiWidth(uint32 unicode);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


