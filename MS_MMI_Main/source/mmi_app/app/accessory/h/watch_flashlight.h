/*****************************************************************************
** File Name:       watch_flashlight.h                                       *
** Author:          miao.xi                                                  *
** Date:            2021/1/27                                                *
** Copyright:                                                                *
** Description:This file is used to process msg to main window of flashlight *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 01/27/2021             miao.xi              Create                        *
******************************************************************************/
#ifndef _WATCH_FLASHLIGHT_H_
#define _WATCH_FLASHLIGHT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "sci_types.h"

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: enter flash light window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchFlashLight_MainWin_Enter( void );
/*****************************************************************************/
//  Description: exist flash light window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchFlashLight_MainWin_Exit( void );

#endif
