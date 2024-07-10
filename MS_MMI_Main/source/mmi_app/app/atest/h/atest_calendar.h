/*****************************************************************************
** File Name:      atest_calendar.h                                             *
** Author:         lin.chen                                                  *
** Date:           18/04/2012                                               *
** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe Calendar Lunar       		 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE           NAME             DESCRIPTION                             *
**  18/04/2012     lin.chen          Created                                 **
******************************************************************************/

#ifndef _ATEST_CALENDAR_H
#define _ATEST_CALENDAR_H 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "stdio.h"

#ifdef WIN32
/**--------------------------------------------------------------------*
 **                         Compiler Flag                              *
 **--------------------------------------------------------------------*/

#define  ATEST_YEAR_START   1900
#define  ATEST_YEAR_END		2100
#define  ATEST_MAX_PATH		500
#define  ATEST_MONTH_START  1
#define  ATEST_MONTH_END	12
#define  ATEST_DAY_START	1
#define  ATEST_FEB_MONTH	1
#define  ATEST_FEB_MAX_DAYS	29
#define  ATEST_FEB_SEC_DAYS	28

/**-----------------------------------------------------------------------*
 **                                Function Prototypes                    *
 **-----------------------------------------------------------------------*/
PUBLIC void Atest_GetLunarDataFromMocor(char *LangTestingFilePath);

#endif//WIN32

#endif //_ATEST_CALENDAR_H

