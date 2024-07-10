/*****************************************************************************
** File Name:      mmiqbtheme_menuwin.h                                          *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_MENUWIN_H_
#define _MMIQBTHEME_MENUWIN_H_ 


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
//  Description : creat grid idle menu win.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_CreatePopupAddWin(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
PUBLIC void MMIWIDGET_CreateGridMenuWin(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

#ifdef QBTHEME_SUPPORT
/*****************************************************************************/
//  FUNCTION:      MMIQBIDLE_CreateShortCutWin
//  Description :
//  Global resource dependence : 
//  Author:        juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBIDLE_CreateAddShortCutWin(void);

/*****************************************************************************/
//  FUNCTION:      MMIQBIDLE_CreateAddWidgetWin
//  Description :
//  Global resource dependence : 
//  Author:        juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBIDLE_CreateAddWidgetWin(void);
#endif


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
