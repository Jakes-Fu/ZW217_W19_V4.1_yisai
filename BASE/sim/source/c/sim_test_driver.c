/******************************************************************************
** File Name:      sim_test_driver.c                                         *
** Author:         Raislin.Kong                                              *
** Date:           2002.6                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of the test SIM driver,use to test GSM *
**                 stack software on other's stable hardware                 *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.6         Raislin.Kong     Create                                    *
******************************************************************************/
// @ lin.liu
// must.
#define NUCLEUS_HISR
#include "base_trc.h"
#include "sim_include.h"
#include "ms.h"
#include "ci_reg.h"
#include "CI.h"
#include "CI_sici.h"
#include "CI_ctrl.h"
#include "CI_acces.h"
#include "CI_driver.h"
#include "CI_trace.h"
//#include "tb_hal.h"
#include "sim_drv.h"


//#include "Ci_driver.h"
#ifdef   __cplusplus
extern   "C"
{
#endif
//---------------extern variable-----------------------------
extern unsigned char    CI_d_used;
extern unsigned char    CI_f_used;
extern unsigned char    CI_di;
extern unsigned char    CI_fi;
extern unsigned char    CI_wi;
//---------------Local Variable----------------------------
const static uint32 s_sim_driver_task_id = P_SIM;
const static uint32 s_sim_manager_task_id = P_SIM;
static BOOLEAN s_is_reseted;
static uint16 s_t_protocol;
static SIMDRV_WORKING_VOLT_E    s_sim_working_volt;
static TB_SIM_STATUS_E  s_sim_command_status;
static SIM_CI_COMMAND_STATUS_E s_sim_ci_command_status;
static BOOLEAN s_pps_response;
//the rx tx buf
static TB_SIM_TX_BUF_T s_sim_tx_buf;
static uint16 s_pecx_len;
//the SIM CARD register map
static BOOLEAN s_not_t0_protocal;
static uint8 s_sim_reset_times;
static uint8 s_instr_code;          //the instruction code that has been send to the SIM card
static BOOLEAN  s_transfor_mode_changed;
//---------------Local Function declare--------------------
static void SendResponseUp(void);



/********************************************************************
FUNCTION SIM_InitDriver

DESCRIPTION
Initialize the SIM card Driver,make it ready to work when needed

RETURN VALUE    void
***********************************************************************/
uint32 SIM_InitDriver(void)
{
    s_sim_ci_command_status = TB_SIM_NO_CARD;
    s_is_reseted = FALSE;
    return 0;
}



/********************************************************************
FUNCTION SIM_SendInstrCode

DESCRIPTION
Send a Instruction to the SIM card to start an operation on The SIM
card we needed

RETURN VALUE    void
***********************************************************************/
void SIM_SendInstrCode(
    TB_SIM_APDU_T *apdu_ptr    //the instruction code to be sent to SIM card
)
{
    uint16 i = 0;
    sim_cmdDir_type cmd_dir;

    s_sim_tx_buf.cur_pos = 0;
    s_sim_tx_buf.tx_data_len = TB_SIM_APDU_CHEADER_NUM;

    //save the instruction instr code
    s_instr_code = apdu_ptr->instr_code[TB_SIM_APDU_INS];
    //copy the instruction code to the rx tx buffer
    SCI_MEMCPY(s_sim_tx_buf.buf, apdu_ptr->instr_code, TB_SIM_APDU_CHEADER_NUM);

    //is send data exist
    if((TB_SIMINSTR_SEND_DATA == apdu_ptr->instr_type) ||
        (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type))
    {
        //copy the data after the command code
        if(0 != apdu_ptr->instr_code[TB_SIM_APDU_P3])
        {
            s_sim_tx_buf.tx_data_len += apdu_ptr->instr_code[TB_SIM_APDU_P3];
            SCI_MEMCPY(s_sim_tx_buf.buf + TB_SIM_APDU_CHEADER_NUM, apdu_ptr->instr_data, apdu_ptr->instr_code[TB_SIM_APDU_P3]);
        }

        cmd_dir = sim_tx_cmd;
    }
    else
    {
        cmd_dir = sim_rx_cmd;
    }

    //Trace the data that send to the SIM card;

    /*    SCI_TRACE("\n\rSIM::TX(command):");
        for (i = 0;i < s_sim_tx_buf.tx_data_len;i++)
        {
            SCI_TRACE("0X%2X ",s_sim_tx_buf.buf[i]);
        }

    */
    //Debug_Print("WriteCMD");
    SIM_WriteCmd(cmd_dir, s_sim_tx_buf.tx_data_len, s_sim_tx_buf.buf);
    //    SCI_TRACE("Write Command");
}


/********************************************************************
FUNCTION SendResponseUp

DESCRIPTION
When the drive have receive all the response data of an operation from SIM
card,the drive will send the response data to manager layer to process it

RETURN VALUE    void
***********************************************************************/
extern ci_cmd_rsp_type cmd_response;
static const char dight[] = "0123456789ABCDEF";
//extern unsigned char  bStartShowSendCMD;
void SendResponseUp(void)
{
    SIM_SIG_RESPONSE_IND_T *psig = NULL;
    int status;

    //static char rx_buf[256 + 4];
    //    uint16  len, count;
    //    char    *str;

    SCI_CREATE_SIGNAL(psig, SIM_RESPONSE_IND, sizeof(SIM_SIG_RESPONSE_IND_T), s_sim_driver_task_id);

    psig->data = CI_rx_cmd_data_ptr;
    psig->data_len = CI_rx_cmd_data_length;
    psig->sw1 = cmd_response.sw1;
    psig->sw2 = cmd_response.sw2;
#if 0
    len = 0;
    count = 0;
    str = (char *)CI_rx_cmd_data_ptr;

    while((len < 256) && (count < (CI_rx_cmd_data_length + 2)))
    {
        rx_buf[len++] = dight[((*str) >> 4) & 0x0F];
        rx_buf[len++] = dight[(*str) & 0xF];
        rx_buf[len++] = ' ';
        str++;
        count++;
    }

    rx_buf[len] = 0;
    //SCI_TRACE_LOW:"Rx_Buf : %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_174_112_2_17_23_0_9_4731, (uint8 *)"s", rx_buf);
#endif
    /*SCI_TRACE("\n\rSIM::RX(Response):SW1=%X SW2=%X Data_len=%d",psig->sw1,psig->sw2,psig->data_len);

    if (psig->data_len > 0)
    {
        SCI_TRACE("\n\r\t\tData:");
        for (i = 0;i < psig->data_len;i++)
        {
            SCI_TRACE("0X%2X ");
        }
    }*/
    //Debug_Print("SendRspUP");
    //zhi@ 9/25/2002
    status = NU_Send_To_Queue(_SCI_BLOCKID_TO_PID(s_sim_manager_task_id), &psig, UNSIGNED_PER_PTR,
                              NU_NO_SUSPEND);

    // If an invalid queue status is received it is assumed to mean
    // the queue has been deleted so just free the signal.

    if(NU_SUCCESS != status)
    {
        SCI_FREE_SIGNAL(psig);
        nu_exception(__LINE__, __FILE__, NU_UNAVAILABLE);
    }

    //    SCI_SEND_SIGNAL(psig,s_sim_manager_task_id);
    //end zhi@
    //if( bStartShowSendCMD )
    //{
    //    Debug_Print("HasSend");
    //}
    //SCI_TRACE("SendRsp UP!");
    //trace the data


}

void SIM_ResetDriver(void)
{
}

void SendCardStatus(uint16 card_status)
{
    xSignalHeaderRec *psig = NULL;
    int status;

    if(TB_SIM_NO_CARD == s_sim_ci_command_status)
    {

        if(CCI_SIM_INSERTED == card_status)
        {
            SCI_CREATE_SIGNAL(psig, SIM_CARD_INSERTED_IND, sizeof(SIM_SIG_CARD_INSERTED_IND_T), s_sim_driver_task_id);
            ((SIM_SIG_CARD_INSERTED_IND_T *)psig)->t_protocol = TB_SIM_T_0_PROTOCOL;
            ((SIM_SIG_CARD_INSERTED_IND_T *)psig)->insert_status = TB_SIM_INSERTED_OK;
            s_sim_ci_command_status = TB_SIM_RECEIVING_RESPONSE;

            //SCI_TRACE_LOW:"SIM::SIM card inserted ok!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_220_112_2_17_23_0_9_4732, (uint8 *)"");
        }
        else
        {
            SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), s_sim_driver_task_id);
            ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_OUT;
            //SCI_TRACE_LOW:"SIM::SIM removed1!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_225_112_2_17_23_0_9_4733, (uint8 *)"");
        }
    }
    else
    {
        SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), s_sim_driver_task_id);

        if(CCI_SIM_INSERTED == card_status)
        {
            ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_IN;
            //SCI_TRACE_LOW:"SIM::SIM inserted!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_235_112_2_17_23_0_9_4734, (uint8 *)"");
        }
        else if(CCI_SIM_REMOVED == card_status)
        {
            ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_OUT;
            //SCI_TRACE_LOW:"SIM::SIM Removed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_240_112_2_17_23_0_9_4735, (uint8 *)"");
        }
        else if(CCI_SIM_ERROR == card_status)
        {
            ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;
            //SCI_TRACE_LOW:"SIM::SIM error1!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_245_112_2_17_23_0_9_4736, (uint8 *)"");
        }
        else if(0x03 == card_status)
        {
            ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;
            //SCI_TRACE_LOW:"SIM::SIM error2!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_250_112_2_17_23_0_9_4737, (uint8 *)"");
        }
        else
        {
            ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;
            //SCI_TRACE_LOW:"SIM::SIM error3!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_TEST_DRIVER_255_112_2_17_23_0_9_4738, (uint8 *)"");
        }
    }

    //zhi@ 9/25/2002
    status = NU_Send_To_Queue(_SCI_BLOCKID_TO_PID(s_sim_manager_task_id), &psig, UNSIGNED_PER_PTR,
                              NU_NO_SUSPEND);

    // If an invalid queue status is received it is assumed to mean
    // the queue has been deleted so just free the signal.

    if(NU_SUCCESS != status)
    {
        SCI_FREE_SIGNAL(psig);
        nu_exception(__LINE__, __FILE__, NU_UNAVAILABLE);
    }

    //    SCI_SEND_SIGNAL(psig,s_sim_manager_task_id);
    //end zhi@
}

/********************************************************************
FUNCTION SIM_ResetSIMCard

DESCRIPTION
Reset the SIM card with a new working voltage when the SIM card is not
working well at the beignning,but there are reset times limit,if the rest
time exeed the times limit,the SIM card will be Rejected

RETURN VALUE    BOOELAN
    FALSE:  The SIM card reset time has exeed the max time,
            reject the SIM card insead of reset it
    TRUE:   Reset the SIM card with a new working voltage
***********************************************************************/
BOOLEAN SIM_ResetSIMCard(
    BOOLEAN bChange //it the SIM card set need be reset
)
{
    //    Debug_Print("ResetCard");
    //    SCI_TRACE("ResetCard");
    if(!s_is_reseted)
    {
        s_is_reseted = TRUE;
        CIInit();
    }
    else
    {
        SIM_ResetCard(1);
    }
}

void SIM_RejectSIMCard(void)
{
}
/********************************************************************
FUNCTION SIM_PowerOnSIMCard

DESCRIPTION
Power on the SIM card

RETURN VALUE    void
***********************************************************************/
void SIM_PowerOnSIMCard(void)
{
}


/********************************************************************
FUNCTION SIM_IdleSIMCard

DESCRIPTION
Make the SIM card to the Idle state

RETURN VALUE    void
***********************************************************************/
void SIM_IdleSIMCard(void)
{
}


/**********************************************************************
//    Description:
//      This is the function that dispatch different SIM interrupt to
//    their Handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_InteruptHandler(void)
{
}

// @lin.liu
void SIM_InitDriverTimer(void)
{
}


#ifdef   __cplusplus
}
#endif
