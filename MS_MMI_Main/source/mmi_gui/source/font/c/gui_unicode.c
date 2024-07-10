/*****************************************************************************
** File Name:      gui_unicode.c                                              *
** Author:                                                                   *
** Date:           03/13/2007                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from UCS2 string to GB2312 string                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/13/2004    wancan.you           Create                                  *
******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "guifont.h"
#include "gui_unicode.h"

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
#define BASIC_LATIN_FONT_NUM            128          //0x00-0x7F
#define BASIC_LATIN_1_FONT_NUM            128    //0xA0-0xFF

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : This function Check is Ascii Character
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsAsciiCode(uint32 unicode)
{
    if (unicode <= BASIC_LATIN_END)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : This function Check is Latin Character
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsLatinUnicode(uint32 unicode)
{
    BOOLEAN result = FALSE;

    if ((unicode <= BASIC_LATIN_END)
        ||(BASIC_LATIN_1_START <= unicode && unicode <= BASIC_LATIN_1_END)
        ||(LATIN_EXT_A_START <= unicode && unicode <= LATIN_EXT_A_END)
        ||(LATIN_EXT_B_START <= unicode && unicode <= LATIN_EXT_B_END)
        ||(LATIN_EXT_C_START <= unicode && unicode <= LATIN_EXT_C_END)
        ||(LATIN_EXT_D_START <= unicode && unicode <= LATIN_EXT_D_END)
        ||(LATIN_EXT_ADD_START <= unicode && unicode <= LATIN_EXT_ADD_END)
        ||(LATIN_LIGATURE_START <= unicode && unicode <= LATIN_LIGATURE_END)
        ||(LATIN_SMALL_FORM_START <= unicode && unicode <= LATIN_SMALL_FORM_END)
        ||(CURRENCY_SYMBOLS_START <= unicode && unicode <= CURRENCY_SYMBOLS_END))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : This function Check is Cyrillic Unicode(Including Russian)
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsCyrillicUnicode(uint32 unicode)
{
    if ((CYRILLIC_UNICODE_START <= unicode && unicode <= CYRILLIC_UNICODE_END)
        ||(CYRILLIC_SUPPLEMENT_STRAT <= unicode && unicode <= CYRILLIC_SUPPLEMENT_END))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : This function Check is Greek Unicode
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsGreekUnicode(uint32 unicode)
{
    if ((GREEK_UNICODE_START <= unicode && unicode <= GREEK_UNICODE_END)
        ||(GREEK_EXT_UNICODE_START <= unicode && unicode <= GREEK_EXT_UNICODE_END))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : This function Check is CJK Unicode
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsCJKUnicode(uint32 unicode)
{
    if (CJK_UNICODE_START <= unicode && unicode <= CJK_UNICODE_END)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : This function Get Latin font Index
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_GetLatinFontIndex(uint32 unicode)
{
    uint32 latin_index = 0;

    if ((unicode <= BASIC_LATIN_END)
        ||(BASIC_LATIN_1_START <= unicode && unicode <= BASIC_LATIN_1_END)
        ||(LATIN_EXT_A_START <= unicode && unicode <= LATIN_EXT_A_END)
        ||(LATIN_EXT_B_START <= unicode && unicode <= LATIN_EXT_B_END))
    {
        latin_index = unicode;
    }
    else if (LATIN_EXT_ADD_START <= unicode && unicode <= LATIN_EXT_ADD_END)
    {
        latin_index = unicode - LATIN_EXT_ADD_START + LATIN_EXT_B_END + 1;
    }
    else if (CURRENCY_SYMBOLS_START <= unicode && unicode <= CURRENCY_SYMBOLS_END)
    {
        latin_index = unicode - CURRENCY_SYMBOLS_START + (LATIN_EXT_ADD_END - LATIN_EXT_ADD_START + 1) + LATIN_EXT_B_END + 1;
    }

    return latin_index;
}

/*****************************************************************************/
// 	Description : This function Get Cyrillic font Index
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_GetCyrillicFontIndex(uint32 unicode)
{
    uint32 cyrillic_index = 0;

    if (CYRILLIC_UNICODE_START<= unicode && unicode <= CYRILLIC_SUPPLEMENT_END)
    {
        cyrillic_index = unicode - CYRILLIC_UNICODE_START;
    }
    else
    {
        //SCI_TRACE_LOW:"== GUI_GetCyrillicFontIndex == Cyrillic Index exceed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUI_UNICODE_167_112_2_18_3_17_0_146,(uint8*)"");
    }

    return cyrillic_index;
}

/*****************************************************************************/
// 	Description : This function Check is Char One Ascii Char Width?
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsCharOneAsciiWidth(uint32 unicode)
{
    if (GUI_IsLatinUnicode(unicode)
        ||GUI_IsCyrillicUnicode(unicode))
    {
        return TRUE;
    }

    return FALSE;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

