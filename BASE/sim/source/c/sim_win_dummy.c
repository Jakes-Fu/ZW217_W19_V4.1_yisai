/******************************************************************************
** File Name:      sim_win_dummy.c                                           *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of window dummy thread that use to     *
**                 deliver signal to window test interface                   *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/
#include "sim_include.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

#ifdef SIM_SIMULATOR
#include "sim_win_dummy.h"

//the win dummy task id in the test project
STATIC const uint32 s_dummy_task_id = P_SIM_WIN_DUMMY;
//the simulator's window handler,used to communicate with the simulator
STATIC HWND s_hWnd;
//the messgae id used to communicate with the simulator
STATIC uint32   s_nMessage;


/**********************************************************************
//    Description:
//      Get the win dummy layer task id
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint32 SIMWDUMMY_GetTaskId(void)   //return value:the win dummy task id in the test project
{
    return s_dummy_task_id;
}

/**********************************************************************
//    Description:
//      set the simulator's window handle
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMWDUMMY_SetMessageHwnd(
    HWND hWnd    //the hwnd of the window of test sim
)
{
    s_hWnd = hWnd;
}


/**********************************************************************
//    Description:
//      set message id to communicate with the simulator
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMWDUMMY_SetMessageType(
    uint32 nMess //the message id that used as message from dummy to window
)
{
    s_nMessage = nMess;
}

void SCI_InitApplication()
{
}

void REF_PktGprsMsg()
{
}
void REF_GetAdcCalibrationPara(
    uint32 battery_calibration[2],
    uint32 program_calibration[2],
    uint32 invalid_calibration,
    uint32 chr_pm_ver_support
)
{
}
void MMICALL_InACall(void)
{

}
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
)
{
    xSignalHeaderRec *psig;
    SIM_SIG_TIW_TO_WIN_DUMMY_T *pwinsig;
    xSignalHeaderRec *pcnf;

    while(1)
    {
        SCI_RECEIVE_SIGNAL(psig, s_dummy_task_id);

        switch(psig->SignalCode)
        {
            case SIMTIW_TO_WIN_DUMMY:
                pwinsig = (SIM_SIG_TIW_TO_WIN_DUMMY_T *)psig;

                switch(pwinsig->operation_type)
                {
                    case POWER_ON_WITH_NO_SIM:
                        SCI_CREATE_SIGNAL(pcnf, SIM_START_UP_REQ, sizeof(SIM_SIG_START_UP_REQ_T), s_dummy_task_id);
                        SCI_SEND_SIGNAL(pcnf, g_sim_task_id);
                        g_sim_insert_status = TB_SIM_INSERTED_NO_SIM;
                        break;
                    case POWER_ON_WITH_WRONG_SIM:
                        SCI_CREATE_SIGNAL(pcnf, SIM_START_UP_REQ, sizeof(SIM_SIG_START_UP_REQ_T), s_dummy_task_id);
                        SCI_SEND_SIGNAL(pcnf, g_sim_task_id);
                        g_sim_insert_status = TB_SIM_INSERTED_WRONG_SIM;
                        break;
                    case INSERT_WRONG_SIM:
                        SCI_CREATE_SIGNAL(pcnf, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), s_dummy_task_id);
                        ((SIM_SIG_INT_IND_T *)pcnf)->int_type = TB_SIMINT_CARD_IN;
                        SCI_SEND_SIGNAL(pcnf, g_sim_task_id);
                        g_sim_insert_status = TB_SIM_INSERTED_WRONG_SIM;
                        break;
                    case POWER_ON_WITH_GOOD_SIM:
                        SCI_CREATE_SIGNAL(pcnf, SIM_START_UP_REQ, sizeof(SIM_SIG_START_UP_REQ_T), s_dummy_task_id);
                        SCI_SEND_SIGNAL(pcnf, g_sim_task_id);
                        g_sim_insert_status = TB_SIM_INSERTED_OK;
                        break;
                    case INSERT_GOOD_SIM:
                        SCI_CREATE_SIGNAL(pcnf, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), s_dummy_task_id);
                        ((SIM_SIG_INT_IND_T *)pcnf)->int_type = TB_SIMINT_CARD_IN;
                        SCI_SEND_SIGNAL(pcnf, g_sim_task_id);
                        g_sim_insert_status = TB_SIM_INSERTED_OK;
                        break;
                    case REJECT_SIM:
                        SCI_CREATE_SIGNAL(pcnf, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), s_dummy_task_id);
                        ((SIM_SIG_INT_IND_T *)pcnf)->int_type = TB_SIMINT_CARD_OUT;
                        SCI_SEND_SIGNAL(pcnf, g_sim_task_id);
                        break;
                    default:
                        SCI_ASSERT(FALSE);
                        break;
                        //error;
                }

                SCI_FREE_SIGNAL(psig);
                break;
            case SIM_INT_IND:

                //the card in signal
                if(TB_SIMINT_CARD_IN == ((SIM_SIG_INT_IND_T *)psig)->int_type)
                {
                    SCI_CREATE_SIGNAL(pcnf, SIM_RESPONSE_IND, sizeof(SIM_SIG_RESPONSE_IND_T), s_dummy_task_id);
                    SCI_SEND_SIGNAL(pcnf, g_sim_task_id);
                    SCI_FREE_SIGNAL(psig);
                }
                else
                {
                    SendMessage(s_hWnd, s_nMessage, 0, (LPARAM)psig);
                }

                break;
            default:
                SendMessage(s_hWnd, s_nMessage, 0, (LPARAM)psig);
                break;
        }
    }
}

#endif

uint32 REF_GetCalibrationParaSize()
{
    return 0;
}

uint32 REF_GetDspParaSize()
{
    return 0;
}

BOOLEAN SIM_IsCardNotPresent(void)
{
    return TRUE;
}
#ifdef   __cplusplus
}
#endif
