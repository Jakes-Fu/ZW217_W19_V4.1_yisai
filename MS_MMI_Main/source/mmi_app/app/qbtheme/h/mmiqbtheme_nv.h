/*************************************************************************
 ** File Name:      mmiqbtheme_nv.h                                              *
 ** Author:         cheng.luo                                         *
 ** Date:           2011/09/17                                          *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2011/09/17     cheng.luo       Create.                              *
*************************************************************************/

#ifndef QBTHEME_NV_H_
#define QBTHEME_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmiqbtheme_internal.h"
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
    QBTHEME_NV_MAINMENU_ICON = MMI_MODULE_QBTHEME << 16, // 0:system;1:theme
	QBTHEME_NV_ICON_BG,		// 0:system;1:theme
	QBTHEME_NV_FONT,		// 0:system;1:theme
    QBTHEME_NV_WALLPAPER,
	QBTHEME_NV_CUR_THEME
}MMIQBTHEME_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                        *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIQBTHEME_RegQbThemeNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

