/*************************************************************************
 ** File Name:      mmiebook_nv.c                                        *
 ** Author:         maggie.ren                                           *
 ** Date:           2007/01/15                                           *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/01/15     maggie.ren       Create.                              *
*************************************************************************/

#ifndef _MMIEBOOK_NV_H_
#define _MMIEBOOK_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#ifdef EBOOK_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
	MMINV_EBOOK_NEW_BOOKMARK = MMI_MODULE_EBOOK << 16,
	MMINV_EBOOK_CODING_TYPE,	
	MMINV_EBOOK_FONT_SIZE,
#ifdef TTS_SUPPORT
    MMINV_EBOOK_TTS_FLAG,
    MMINV_EBOOK_TTS_VOL,
#endif
	MMINV_EBOOK_COLOR_FOR_FONT,
	MMINV_EBOOK_COLOR_FOR_BACK,	
	MMINV_EBOOK_LAST_BOOK_NAME,
	MMINV_EBOOK_IS_FULL_SCREEN_OFF,
	MMINV_EBOOK_AUTO_READ_TYPE,	
	MMINV_EBOOK_DAY_NIGHT_MODE,
	EBOOK_MAX_NV_ITEM_NUM
} MMIEBOOK_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif//ENG_SUPPORT
#endif





