/*****************************************************************************
** File Name:      mmiuzone_position.h                                       *
** Author:         Jessica                                               *
** Date:           21/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to descirbe uzone position              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Jessica             Create
******************************************************************************/

#ifndef _MMIUZONE_POSITION_H_
#define _MMIUZONE_POSITION_H_ 

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

#if defined(MAINLCD_SIZE_240X400) || defined (MAINLCD_SIZE_240X320)
#define UZONE_ITEM_BG_WIDTH 109
#define UZONE_ITEM_BG_HEIGHT 144
#define UZONE_ITEM_SHADOW_HEIGHT_H   (UZONE_ITEM_BG_HEIGHT / 4)

#define UZONE_ITEM_ICON_POS_X 23
#define UZONE_ITEM_ICON_POS_Y 28

#define UZONE_ITEM_ICON_WIDTH 32
#define UZONE_ITEM_ICON_HEIGHT 32

#define UZONE_ITEM_STR_POS_X 22
#define UZONE_ITEM_STR_POS_Y 111

#define UZONE_ITEM_STR_WIDTH 65
#define UZONE_ITEM_STR_HEIGHT 18

#define UZONE_BIG_STAR_HEIGHT 23
#define UZONE_BIG_STAR_WIDTH 22

#define UZONE_SMALL_STAR_HEIGHT 17
#define UZONE_SMALL_STAR_WIDTH 16

#else //defined(MAINLCD_SIZE_240X400) || defined (MAINLCD_SIZE_240X320)

/*Need be updated when coordinate is provided*/
#define UZONE_ITEM_BG_WIDTH 143
#define UZONE_ITEM_BG_HEIGHT 168
#define UZONE_ITEM_SHADOW_HEIGHT_H   (UZONE_ITEM_BG_HEIGHT / 4)

#define UZONE_ITEM_ICON_POS_X 22
#define UZONE_ITEM_ICON_POS_Y 25

#define UZONE_ITEM_ICON_WIDTH 48
#define UZONE_ITEM_ICON_HEIGHT 48

#define UZONE_ITEM_STR_POS_X 23
#define UZONE_ITEM_STR_POS_Y 130

#define UZONE_ITEM_STR_WIDTH 97
#define UZONE_ITEM_STR_HEIGHT 18

#define UZONE_BIG_STAR_HEIGHT 33
#define UZONE_BIG_STAR_WIDTH 32

#define UZONE_SMALL_STAR_HEIGHT 27
#define UZONE_SMALL_STAR_WIDTH 26
#endif //defined(MAINLCD_SIZE_240X400) || defined (MAINLCD_SIZE_240X320)



/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
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

#endif //_MMIUZONE_POSITION_H_
