/*****************************************************************************
** File Name:      mmiuzone_display.h                                                *
** Author:         Jessica                                               *
** Date:           21/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to descirbe uzone                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Jessica             Create
******************************************************************************/

#ifndef _MMIUZONE_DISPLAY_H_
#define _MMIUZONE_DISPLAY_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

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
#if defined(MAINLCD_SIZE_320X480)
    #define MMIUZONE_TEXT_FONT         SONG_FONT_18
#else 
    #define MMIUZONE_TEXT_FONT         SONG_FONT_14 
#endif
#define MMIUZONE_TEXT_COLOR        0xffbf//白色反色后边框为纯黑，与透明色一样，描边不起效果//MMI_WHITE_COLOR


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : fill uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_DrawWindow(void);

 /*****************************************************************************/
//  Description : fill uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_ExistDraw(void);

 /*****************************************************************************/
//  Description : fill uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_FillLayers(void);

/*****************************************************************************/
//  Description : draw focus item callback
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_DrawFocusItem(uint16 item_index);

/*****************************************************************************/
//  Description : draw bg to main layer
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_DrawBgToMainLayer(void);
    

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIUZONE_DISPLAY_H_

