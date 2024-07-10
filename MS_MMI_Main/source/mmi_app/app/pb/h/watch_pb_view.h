/*****************************************************************************
** File Name:      watch_pb_view.h                                           *
** Author:         yuming.yang                                               *
** Date:           04/05/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw pb list                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 04/2020               yuming.yang            Create                       *
******************************************************************************/

#ifndef _WATCH_PB_VIEW_H_
#define _WATCH_PB_VIEW_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipb_adapt.h"
#include "mmipb_export.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct _MMIPB_CONTACTLISTINFO_T
{
	uint32           count;
	MMIPB_CONTACT_T  contactArr[WATCHPB_CONTACT_NUM_MAX];
}MMIPB_CONTACTLISTINFO_T;


/*---------------------------------------------------------------------------*/
/*                         PUBLIC FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: Open pb main menu window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_MainMenuWin_Enter(void);

/*****************************************************************************/
//  Description : Close pb main menu window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_MainMenuWin_Exit(void);

/*****************************************************************************/
//  Description : enter SMS Contact list win
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_ContactWin_Enter(void);
#endif
