/*****************************************************************************
** File Name:        watchface_multifunctional_dial.h                        *
** Author:           bin.wang1                                               *
** Date:             10/21/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define multifunctional dial          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 10/21/2021           bin.wang1             Create                         *
******************************************************************************/

#ifdef WATCHFACE_CTRL_EDIT_SUPPORT
#ifndef WATCHFACE_MULTIFUNCTIONAL_DIAL_H
#define WATCHFACE_MULTIFUNCTIONAL_DIAL_H

#include "mmk_type.h"

/*****************************************************************************
//  Description : center MTF WIN
//  Author: bin.wang1
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WATCHFACE_MTFDial_Win_Enter( uint16 watchface_index );

/*****************************************************************************
//  Description : center MTF list WIN
//  Author: bin.wang1
//  Return:
//  Note:
*****************************************************************************/


PUBLIC void WATCHFACE_MTFDial_List_Win_Enter(void);
/*****************************************************************************
//  Description : get whether edit multinumber or not
//  Author: qi.zhang
//  Return:s_is_change_multi
//  Note:
*****************************************************************************/

PUBLIC BOOLEAN WATCHFACE_Get_Is_Change_Multi( void );
/*****************************************************************************
//  Description : set s_is_change_multi
//  Author: qi.zhang
//  Return:
//  Note:
*****************************************************************************/

PUBLIC void WATCHFACE_Set_Is_Change_Multi( BOOLEAN is_change_multi );


#endif
#endif
