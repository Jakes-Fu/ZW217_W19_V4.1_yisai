/******************************************************************************
 ** File Name:      production_test_current.c                            *
 ** Author:         Yuhua.Shi                                                 *
 ** DATE:           03/06/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/06/2008     Yuhua.Shi        Create.                                   *
 ******************************************************************************/

#include "ms_ref_base_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "production_test.h"
#include "deepsleep_drvapi.h"
#include "diag.h"
#include "cmddef.h"
#include "layer1_engineering.h"
#include "com_drvapi.h"
#include "chip.h"
//#include "fiq_drvapi.h"
#include "ref_outport.h"
#include "run_mode.h"
#include "tb_hal.h"
#include "ipc_drvapi.h"
#include "ldo_drvapi.h"


extern uint8  g_usb_plugout_flag;
extern void current_test_flag_set(void);

//#define LPS_Debug_Step	hwp_idle->IDLE_RES9
//#define LPS_Debug_Cnt	hwp_idle->IDLE_RES10

/*lint -e63 -e26*/
static uint8  s_current_test_subtype = CURRENT_TEST_STOP;

static uint32 Handle_Current_Test (uint8 **dest_ptr,       // Pointer of the response message.
                                   uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                   const uint8 *src_ptr,   // Pointer of the input message.
                                   uint16 src_len)
{
    MSG_HEAD_T   *msg_head = (MSG_HEAD_T *) src_ptr;
    int usb_cnt = 0;
    unsigned int dataLen = 0;
    void *p_calibData;
    unsigned int reqLen;
    unsigned int cmdId;
    //SCI_TRACE_LOW:"ProductTest : CurrentTest (%d/%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PRODUCTION_TEST_CURRENT_38_112_2_18_1_5_28_53,(uint8*)"dd",msg_head->type, msg_head->subtype);

    switch (msg_head->subtype)
    {
        case CURRENT_TEST_STOP:

            switch (s_current_test_subtype)
            {
                    // The test is handled by system.
                case CURRENT_TEST_TX_MAX_PEAK_VALUE:
                    L1API_DSPMaxPowerTX (FALSE);
                    break;
                case CURRENT_TEST_RX_MIN_PEAK_VALUE:
                    L1API_DSPMinPowerRX (FALSE, 0);
                    break;

                case CURRENT_TEST_CHARGING:
                case CURRENT_TEST_LED_ON:
                case CURRENT_TEST_VIBRATOR_ON:
                    SCI_ProductTest_Current_Stop();   // Stop the current test
                    break;
                default:
                    break;
            }

            break;
        case CURRENT_TEST_RF_CLOSED:  // Not support now.
            break;
        case CURRENT_TEST_DEEP_SLEEP:
            current_test_flag_set();
            cmdId = (uint32)((uint16)msg_head->type | ((uint16)msg_head->subtype << 8));
            reqLen = (msg_head->len - sizeof(MSG_HEAD_T));

            // use aud calib buffer to transform calib cmd and data
            p_calibData = Get_AudCalibBuf(&dataLen);
            SCI_ASSERT(reqLen <= dataLen);

            SCI_MEMCPY(p_calibData, (uint16 *)(src_ptr + sizeof(MSG_HEAD_T)), reqLen);

            NV_IPC_sendCmdToCp(cmdId, NV_CMD_CALIB, (uint32)p_calibData, (msg_head->len - sizeof(MSG_HEAD_T)));

            while (g_usb_plugout_flag)
            {
                usb_cnt++;
                mon_Event(__LINE__);
                mon_Event(usb_cnt);
                SCI_Sleep(100);
            }

            WDG_TimerStop();

            LDO_DeepSleepInit();

            SCI_ProductTest_Current_Prepare_DeepSleep();

            ANA_DecisionCodecTpFunc(0,0);
            ANA_DecisionCodecTpFunc(1,0);

            UART_PHY_SuspendResumeLogout(UART_COM0);
            UART_PHY_SuspendResumeLogout(UART_COM1);
            UART_PHY_SuspendResumeLogout(UART_COM2);

            OS_TickDelay (100);

            // Let ARM enter deep sleep
            SCI_Deep_Sleep_Test();/*lint !e718 !e746 -esym(628,SCI_Deep_Sleep_Test)*/

            HAL_TurnOffPower();

            //lint -e716
            while (1)
            {
                ; // loop forever, don't send the response.
            }

            //lint +e716
            break;  /*lint !e527*/
        case CURRENT_TEST_LED_ON:
            SCI_ProductTest_Current_LED_On();
            //GPIO_SetKeyPadBackLight( 1 );
            //GPIO_SetLcdBackLight( 1 );
            break;
        case CURRENT_TEST_VIBRATOR_ON:
            SCI_ProductTest_Current_Vibrator_On();
            //GPIO_SetVibrator( 1 );
            break;
        case CURRENT_TEST_RX_MIN_PEAK_VALUE:
            L1API_DSPMinPowerRX (TRUE, 0);
            break;
        case CURRENT_TEST_TX_MAX_PEAK_VALUE:
            L1API_DSPMaxPowerTX (TRUE);
            break;
        case CURRENT_TEST_CHARGING:
            SCI_ProductTest_Current_Charge();
            //GPIO_EnableCharge( 1 );
            break;
        case CURRENT_TEST_DEEP_GET_SLEEP_FLAG:
            //RM_GetEnableArmSleepFlag();     //coverity 36213 only return value
            break;
        case CURRENT_TEST_DEEP_SLEEP_FLAG_ENABLE:
            RM_SetEnableArmSleepFlag (1);
            break;
        case CURRENT_TEST_DEEP_SLEEP_FLAG_DISABLE:
            RM_SetEnableArmSleepFlag (0);
            break;
        case CURRENT_TEST_UART_ENABLESLEEP:
            // right now, only care about UART1
            SIO_SetUartRxWakeup(UART_COM1, SCI_FALSE);            
            DPSLP_UART_EnableApbSleep(UART1_E, ENABLE_APB_SLEEP);
            break;  
        case CURRENT_TEST_POWER_OFF:
            SCI_ProductTest_Current_Stop();
            HAL_TurnOffPower();
            break;              
        default:
            break;
    }

    s_current_test_subtype = msg_head->subtype;
    return 1;
}

// Register
void Register_Current_Test_CmdRoutine (void)
{
    //SCI_TRACE_LOW:"production_test_current: Register Cmd Routine"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PRODUCTION_TEST_CURRENT_142_112_2_18_1_5_28_54,(uint8*)"");

    DIAG_RegisterCmdRoutine (DIAG_CURRENT_TEST_F,  Handle_Current_Test);
}

// Unregister
//LOCAL void UnRegister_Current_Test_CmdRoutine (void)
//{
//    DIAG_UnRegisterCmdRoutine (DIAG_CURRENT_TEST_F);
//}
