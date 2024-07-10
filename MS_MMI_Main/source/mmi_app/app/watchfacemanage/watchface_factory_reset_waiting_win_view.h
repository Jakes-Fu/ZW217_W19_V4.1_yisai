/*****************************************************************************
** File Name:      watchface_factory_reset_waiting_win_view.h                *
** Author:         qi.zhang                                                  *
** Date:           07/18/2022                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define watch face note win view      *
**                 function                                                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/18/2022     qi.zhang         Create                                    *
******************************************************************************/

#ifndef _WATCHFACE_FACTORY_RESET_WAITING_WIN_VIEW_H_
#define _WATCHFACE_FACTORY_RESET_WAITING_WIN_VIEW_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : WATCHFACE_WaitingFactoryResetWinEnter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_WaitingFactoryResetWinEnter( void );

/*****************************************************************************/
//  Description : 关闭表盘恢复出厂设置等待界面
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]NONE
//  Author:qi.zhang
/*****************************************************************************/
PUBLIC void WATCHFACE_CloseWaitingFactoryResetWin(void);



#endif


