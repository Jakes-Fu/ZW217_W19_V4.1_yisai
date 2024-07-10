/****************************************************************************
** File Name:      mmiflash.h                                                *
** Author:                                                                 *
** Date:           27/05/2011                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011       jesse.yu         Create
** 
****************************************************************************/
#ifndef  _MMI_FLASH_H_    
#define  _MMI_FLASH_H_   


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "Sci_types.h"
#include "Mmk_type.h"
#include "caf.h"
#include "mmiflash_export.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
	FLASH_SCREEN_ID_INVALID,
	FLASH_SCREEN_ID_S_PB,
	FLASH_SCREEN_ID_S_MESSAGE,
	FLASH_SCREEN_ID_S_CALENDAR,
	FLASH_SCREEN_ID_CL,
	FLASH_SCREEN_ID_PB,
	FLASH_SCREEN_ID_BROWSER,
	FLASH_SCREEN_ID_PLAYER,
	FLASH_SCREEN_ID_MESSAGE,
	FLASH_SCREEN_ID_TOOLS,
	FLASH_SCREEN_ID_CAMERA,
	FLASH_SCREEN_ID_FAVORATE,
	FLASH_SCREEN_ID_TV,
	FLASH_SCREEN_ID_GAMES,
	FLASH_SCREEN_ID_ENTERTAINMENT,
	FLASH_SCREEN_ID_SETTING,
	FLASH_SCREEN_ID_AUDIO,
	FLASH_SCREEN_ID_CALENDAR,
	FLASH_SCREEN_ID_KEYUP,
	FLASH_SCREEN_ID_KEYDOWN,
	FLASH_SCREEN_ID_KEYLEFT,
	FLASH_SCREEN_ID_KEYRIGHT,
	// TODO: add your self-defined IDs
	FLASH_SCREEN_ID_MAX,
}FLASH_SCREEN_ID_ENUM;




/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

