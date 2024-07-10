/****************************************************************************
** File Name:      watch_bt_main_win.h                                      *
** Author:         bin.wang1                                                *
** Date:           04/19/20120                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth display menu *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/19/2020    bin.wang1          Create                                 *
**                                                                         *
****************************************************************************/

#ifndef _WATCH_BT_MAIN_WIN_H_    
#define  _WATCH_BT_MAIN_WIN_H_    

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
//#include "guilistbox.h"
//#include "window_parse.h"
//#include "guimenu.h"
//#include "mmibt_export.h"
//#include "mmibt_id.h"
//#include "mmibt_menutable.h"
//#include "mmifilearray_export.h"
//#include "mmibt_nv.h"
#include "bt_abs.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: Open the blue tooth main menu win  
//  Global resource dependence: none 
//  Author: bin.wang1 
//  Note:
//
/*****************************************************************************/
PUBLIC void BT_MainWin_Enter(void);

/****************************************************************************/
//  Description : process ps msg
//  Global resource dependence :
//  Author:zhikun.lv
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN WatchBtMain_ProcessBtPsMsg(uint16 msg_id, const BT_MSG_T *msg_body_ptr);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif

