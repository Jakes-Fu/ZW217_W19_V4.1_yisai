/****************************************************************************
** File Name:      mmiweather_export.c                                     *
** Author:         Gaily.Wang                                              *
** Date:           April/04/2011                                           *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file define the external APIs for weather app      *
****************************************************************************/

/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mn_type.h"
#include "mmiweather.h"
#include "mmiweather_win.h"
#include "mmiweather_export.h"
#include "mmiweather_id.h"
#include "mmiweather_nv.h"
#include "mmiweather_menutable.h"

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
// 	Description : MMIAPIWEATHER_InitModule
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_InitModule(void)
{
    MMIWEATHER_RegMenuGroup(); 
    MMIWEATHER_RegWeatherNv();
    MMIWEATHER_RegWinIdNameArr();
}


/*****************************************************************************/
//  Discription: MMIAPIWEATHER_InitApp
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_InitApp(void)
{
    MMIWEATHER_InitApp();
}

/*****************************************************************************/
//  Discription: MMIAPIWEATHER_ResetFactory
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_ResetFactory(void)
{
    MMIWEATHER_ResetApp();
}


/*****************************************************************************/
//  Discription: MMIAPIWEATHER_OpenMainWin
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_OpenMainWin(void)
{
    MMIWEATHER_OpenMainWin();
}




