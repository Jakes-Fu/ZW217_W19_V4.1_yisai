/******************************************************************************
** File Name:      sim_win_dummy.h                                           *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This header file have the windows dummy thread funcitons  *
**                 declare                                                   *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#ifndef _SIM_WIN_DUMMY_H_
#define _SIM_WIN_DUMMY_H_
#ifdef   __cplusplus
extern   "C"
{
#endif
/**********************************************************************
//    Description:
//      Get the win dummy layer task id
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint32 SIMWDUMMY_GetTaskId(void);   //return value:the win dummy task id in the test project


/**********************************************************************
//    Description:
//      set the simulator's window handle
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMWDUMMY_SetMessageHwnd(
    HWND hWnd    //the hwnd of the window of test sim
);


/**********************************************************************
//    Description:
//      set message id to communicate with the simulator
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMWDUMMY_SetMessageType(
    uint32 nMess //the message id that used as message from dummy to window
);

/**********************************************************************
//    Description:
//      the win dummy layer's entry function.while is the main body of win
//    dummy task
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void thread_win_dummy_entry(
    uint32 agrc,    //the argument num
    void *argv      //the argument array
);
#ifdef   __cplusplus
}
#endif

#endif  //_SIM_WIN_DUMMY_H_