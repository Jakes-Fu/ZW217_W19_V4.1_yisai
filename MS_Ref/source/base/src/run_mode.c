/******************************************************************************
 ** File Name:      run_mode.c                                                *
 ** Author:         Jim.zhang                                                 *
 ** DATE:           29/03/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain code to get the run mode of ARM .       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 39/03/2004     Jim.zhang        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "run_mode.h"
#include "os_api.h"
#include "nv_type.h"
#include "nvitem.h"
#include "arm_reg.h"
#include "cmddef.h"
#include "tb_comm.h"
#include "tb_hal.h"
#include "sio.h"
#include "vcom_usb.h"
#include "driver_export.h"
#include "boot_drvapi.h"
#include "power.h"
#include "chip.h"
#include "sio.h"
#include "dal_chr.h"
#include "mn_type.h"        /* MN_MAX_IMEI_LENGTH     */
#include "bt_abs.h"         /* struct _BT_ADDRESS     */
//#include "bt_cfg.h"         /* struct _BT_NV_PARAM    */
#include "gps_drv.h"        /* struct gps_nv_info_tag */
#include "production_test.h"
#include "uartcom_drv.h"
#include "boot_drvapi.h"

#if defined (PLATFORM_UWS6121E)
#include "../../../../chip_drv/chip_module/pin/uws6121e/pinmap.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                              External variable
 **---------------------------------------------------------------------------*/

extern void SCI_Calibration_EnableDeepSleep(uint32 mode);
extern uint32 REFPARAM_GetUartMode(void);
extern BOOLEAN REFPARAM_GetEnableArmSleepFlag(void);


/**---------------------------------------------------------------------------*
 **                         Macro Define
 **---------------------------------------------------------------------------*/
//#define _RM_DEBUG


#define  RM_RESPONSE_WAIT_TIME      500      /* wait 500ms for PC response (ms)*/

#ifdef MODEM_PLATFORM
#define  RM_VCOM_MAX_WAIT_TIME      3000    /* wait max 3s for PC response when virtual com is used(ms)*/
#define  RM_VCOM_INITEND_MAXTIME    1000     /* max usb virtual com enumeration time (ms)*/
#else
#define  RM_VCOM_MAX_WAIT_TIME      5000    /* wait max 5s for PC response when virtual com is used(ms)*/
#define  RM_VCOM_INITEND_MAXTIME    500     /* max usb virtual com enumeration time (ms)*/ //win7 enum cost more time
#endif

#define  RM_VCOM_MAX_DELAY_TIME     150      /* wait time after sending the received cmd to pc again (ms)*/

#define  RM_FIXED_PROTO_LEN         10
#define  RM_PROTO_HEAD                0x7E
#define  RM_RUNMODE_REQ_COMMAND     0x01
#define  RM_PROTO_ESCAPE            0x7D
#define  RM_COMMAND_T               0xFE
#define  RM_AUTODL_MODE             0x82

//sub command type define.
#define  RM_CALIBRATION_MOD             0x01    //in calibration mode.
#define  RM_BOOT_MOD                    0x02    //in boot mode:reset and enter boot mode.
#define  RM_PIN_TEST_MOD                0x03    //in production test mode
#define  RM_IQC_TEST_MOD                0x04    //in IQC fixture test mode
#define  RM_CALIBR_POST_MOD             0x05    //in calibration post mode
#define  RM_CALIBR_POST_NO_LCM_MODE     0x06    //in calibration post mode without lcm,app
/*used for td cft related*/
#define  RM_TD_CALIBRATION_MOD          0x07    //in td calibration mode.
#define  RM_TD_CALIBR_POST_MOD          0x08    //in td calibration post mode
#define  RM_TD_CALIBR_POST_NO_LCM_MODE  0x09    //in td calibration post mode without lcm,app
#define  RM_CALIBR_NV_ACCESS_MODE       0x0a
/* sub command type for LTE calibration*/
#define  RM_LTE_CALIBRATION_MOD         0x00    //in td calibration mode.
#define  RM_LTE_CALIBR_POST_MOD         0x01    //in td calibration post mode

#define     RM_ENABLE_SLEEP                0x40    //enable arm deep sleep
#define     RM_U0_AS_DIAG                  0x80    //uart0 used as diag port(debug)

/* key pushed   */
#define KEY_ON     (0x1)
/* key released */
#define KEY_OFF    (0x0)
#if defined(CALI_MODE_PORT_UART0)
    #define RM_UART    (GetUART0BaseAddr())
#else
    #define RM_UART    (getRMUartBaseAddr())
#endif
/**---------------------------------------------------------------------------*
 **  Protocol functions
 **---------------------------------------------------------------------------*/
/*
RM get char, src port type
*/
#define  RM_FROM_UART           0x00
#define  RM_FROM_USB            0x01
#define  RM_FROM_OTHERS         0x02

#define  RM_FRAME_NONE          0x00
#define  RM_FRAME_HEAD          0x01
#define  RM_FRAME_TAIL          0x02

#define  RM_CALI_NV_IMEI1_MSK   0x01
#define  RM_CALI_NV_IMEI2_MSK   0x02
#define  RM_CALI_NV_BT_MSK      0x04
#define  RM_CALI_NV_GPS_MSK     0x08
#define  RM_CALI_NV_GUID_MSK    0x10
#define  RM_CALI_NV_SN_MSK      0x20

/**---------------------------------------------------------------------------*
 **                         Struct Define
 **---------------------------------------------------------------------------*/
//
#ifndef  WIN32
#define    __PACKED    __packed
#else
#define     __PACKED
#endif

typedef __PACKED struct tag_Command /*lint !e140*/
{
    uint32 reserved;
    uint16 frameSize;
    uint8  command;
    uint8  subCommand;
}COMMAND_T;

#if 0
typedef struct _REF_NVWriteDirect {
    uint8          imei1[MN_MAX_IMEI_LENGTH];  /*imei1 number 8byte*/
    uint8          imei2[MN_MAX_IMEI_LENGTH];  /*imei1 number 8byte*/
    BT_ADDRESS    bt_add;
    GPS_NV_INFO_T  gps_info;
    uint8          guid[GUID_LEN];
    uint8          sn[SN_LEN];
}REF_NVWriteDirect_T;
#endif
/**---------------------------------------------------------------------------*
 **                         Local var Define
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN is_uart0_as_data = 0xFF;                // SCI_TRUE: UART0 is used as DATA port
                                                    // SCI_FALSE:Uart0 as DEBUG port.
                                                    // It CAN not  be initlized as  0 or 1 !!!

LOCAL BOOLEAN is_calibration_mode =SCI_FALSE;        // SCI_TRUE: in calibration mode.
                                                    // SCI_FALSE: not in calibration mode.


LOCAL BOOLEAN is_calibrationpost_mode =SCI_FALSE;    // SCI_TRUE: in calibration post mode.
                                                    // SCI_FALSE: not in calibration post mode.

LOCAL BOOLEAN is_calibrationpostnolcm_mode =SCI_FALSE;    // SCI_TRUE: in calibration post no lcm mode.
                                                        // SCI_FALSE: not in calibration post no lcm mode.

LOCAL BOOLEAN is_pin_test_mode  =SCI_FALSE;            // SCI_TRUE:  in pin-test mode
                                                    // SCI_FALSE: not in pin-test mode.

LOCAL BOOLEAN is_iqctest_mode        =SCI_FALSE;        // SCI_TRUE:  in IQC test mode
                                                    // SCI_FALSE: not in IQC test mode.

PUBLIC uint8 enable_deep_sleep = 0xFF;    /*lint -esym(765,enable_deep_sleep)*/    // Must be initlized as 0xFF, it will be set in run_mode.c

LOCAL uint32  s_dloader_mode = 0x0;                 // 0: dloader by uart, 1: by usb

LOCAL uint32 runmode_request_time =0;                // Static var for record the request time

LOCAL uint32 s_calibratiomode  = 0;                 // used for td cali post select

#define    MAX_IMEI_CMD_LEN    (2048)
LOCAL uint8    *rm_cmd_buffer;
LOCAL uint32     cmd_data_len = 0;
LOCAL uint32  s_usb_calibration_mode=0;

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
 LOCAL uint32 RM_RecDataFromTool(uint8 * desPtr);
 /*****************************************************************************/
//  Description:    The function used to whether system run in USB calibration.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:           SCI_TRUE: Enable
//                  SCI_FALSE: Disable
/*****************************************************************************/
 BOOLEAN RM_IsUsbCalibrationMode(void)
 {
    return (s_usb_calibration_mode==1);
 }
/*****************************************************************************/
//  Description:    The function gets enable arm sleep flag.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:           SCI_TRUE: Enable
//                  SCI_FALSE: Disable
/*****************************************************************************/
PUBLIC uint8 RM_GetEnableArmSleepFlag(void)   // Return the flag.
{
    if (0xFF != enable_deep_sleep)
    {
        return enable_deep_sleep;
    }
    else
    {
        return (REFPARAM_GetEnableArmSleepFlag());
    }
}

/*****************************************************************************/
//  Description:    The function gets enable arm sleep flag.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:           SCI_TRUE: Enable
//                  SCI_FALSE: Disable
/*****************************************************************************/
PUBLIC void RM_SetEnableArmSleepFlag(
    uint8 is_deep_sleep            //deep sleep enable/disable flag
    )
{
    enable_deep_sleep = is_deep_sleep;
}


/******************************************************************************/
//  Description:  get the current uart0 mode.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          The NV parameter and command send from PC when power up
//                also should be considered.
/******************************************************************************/
PUBLIC uint32 RM_GetUart0Mode(void)
{
    if ( is_uart0_as_data == SCI_TRUE )
    {
        return COM_DATA;
    }
    else if ( is_uart0_as_data == SCI_FALSE )
    {
        return COM_DEBUG;
    }
    else
    {
        return (REFPARAM_GetUartMode());
    }
}

/******************************************************************************/
//  Description:  get the current uart0 mode.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          The NV parameter and command send from PC when power up
//                also should be considered.
/******************************************************************************/
PUBLIC void RM_SetUart0Mode(
    uint32 mode                //uarto mode to set.
    )
{
    if (COM_DATA == mode)
    {
        is_uart0_as_data = SCI_TRUE;
    }
    else if ( COM_DEBUG == mode)
    {
        is_uart0_as_data = SCI_FALSE;
    }
    else
    {
    }
}

/******************************************************************************/
//  Description:  Set current pc connect mode when doing cft
//    Global resource dependence:
//  Author:       weihua.wang
//    Note:          After cft, the nv should be written back,we should know how to
//                download the NV, uart or usb
/******************************************************************************/
PUBLIC void RM_SetDloaderMode(
    uint32 mode                //uarto mode to set.
    )
{
    if(mode == 0){  // uart mode
        s_dloader_mode = 0;
    }else if(mode == 1){ //usb mode
        s_dloader_mode = 1;
    }else{
    }
}

/******************************************************************************/
//  Description:  Set current pc connect mode when doing cft
//    Global resource dependence:
//  Author:       weihua.wang
//    Note:          After cft, the nv should be written back,we should know how to
//                download the NV, uart or usb
/******************************************************************************/
PUBLIC uint32 RM_GetDloaderMode(void)
{
    return s_dloader_mode;
}

/*****************************************************************************/
//  Description:    The function gets IQC test flag.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:           SCI_TRUE:  in  IQC test mode
//                  SCI_FALSE: not in IQC test mode
/*****************************************************************************/
PUBLIC void RM_SetIQCTestFlag(
    BOOLEAN is_iqctest        //SCI_TRUE:  in  IQC test mode
    )
{
    is_iqctest_mode = is_iqctest;
}


/*****************************************************************************/
//  Description:    The function gets IQC test flag.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:           SCI_TRUE:  in  IQC test mode
//                  SCI_FALSE: not in IQC test mode
/*****************************************************************************/
PUBLIC BOOLEAN RM_GetIQCTestFlag(void)   // Return the flag.
{
    return is_iqctest_mode;
}


/*****************************************************************************/
//  Description:    The function gets pin test flag.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:           SCI_TRUE:  in pin-test mode
//                  SCI_FALSE: not in pin-test mode
/*****************************************************************************/
PUBLIC BOOLEAN RM_GetPinTestFlag(void)   // Return the flag.
{
    return is_pin_test_mode;
}

/*****************************************************************************/
//  Description:    The function sets pin test flag.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:           SCI_TRUE:  in pin test mode
//                  SCI_FALSE: not pin test mode
/*****************************************************************************/
PUBLIC void RM_SetPinTestFlag(
    BOOLEAN is_pintest_mode        // The flag value to be set
    )
{
    is_pin_test_mode = is_pintest_mode;
}



/******************************************************************************/
//  Description:  Set the calibration post mode flag
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          SCI_TRUE: in calibration post mode
//                SCI_FALSE:not in calibration post mode.
/******************************************************************************/
PUBLIC void RM_SetCalibrationPostMode(
    BOOLEAN is_calibrationpost            //calibraton post mode flag to set
    )
{
    is_calibrationpost_mode = is_calibrationpost;
}

/******************************************************************************/
//  Description:  Get the calibration post mode flag
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          SCI_TRUE: in calibration post mode
//                SCI_FALSE:not in calibration post mode.
/******************************************************************************/
PUBLIC BOOLEAN RM_GetCalibrationPostMode(void)
{
    return is_calibrationpost_mode;
}
/******************************************************************************/
//  Description:  Set the calibration post no lcm mode flag
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          SCI_TRUE: in calibration post mode
//                SCI_FALSE:not in calibration post mode.
/******************************************************************************/
PUBLIC void RM_SetCalibrationPostNoLcmMode(
    BOOLEAN is_calibrationpostnolcm            //calibraton post mode flag to set
    )
{
    is_calibrationpostnolcm_mode = is_calibrationpostnolcm;
}

/******************************************************************************/
//  Description:  Get the calibration post mode flag
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          SCI_TRUE: in calibration post mode
//                SCI_FALSE:not in calibration post mode.
/******************************************************************************/
PUBLIC BOOLEAN RM_GetCalibrationPostNoLcmMode(void)
{
    return is_calibrationpostnolcm_mode;
}

/******************************************************************************/
//  Description:  Set the calibration mode flag
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          SCI_TRUE: in calibration mode
//                SCI_FALSE:not in calibration mode.
/******************************************************************************/
PUBLIC void RM_SetCalibrationMode(
    BOOLEAN is_calibration            //calibraton mode flag to set
    )
{
    is_calibration_mode = is_calibration;
}

/******************************************************************************/
//  Description:  Get the calibration mode flag
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:          SCI_TRUE: in calibration post mode
//                SCI_FALSE:not in calibration mode.
/******************************************************************************/
PUBLIC BOOLEAN RM_GetCalibrationMode(void)
{
    return is_calibration_mode;
}
uint8 Mode_95 = 0;
LOCAL void RM_Set95Mode(uint8 data )
{
    Mode_95 = data;
}

PUBLIC uint8  RM_Get95Mode(void )
{
    return Mode_95;
}

/******************************************************************************/
//  Description:  analyze the received data to enter spefic mode.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:
/******************************************************************************/
LOCAL void RM_SetRunMode(
    uint8 *rx_buf,             //the received char buffer pointer.
    uint8 len                //the length of received char to process.
    )
{
    uint8    command;
    uint16  frequency;

    SCI_ASSERT( PNULL != rx_buf );/*assert verified*/
    /*lint -esym(613, rx_buf) */

    if ((*rx_buf == RM_PROTO_HEAD) && (*(rx_buf+len-1)==RM_PROTO_HEAD))
    {
        if ( (*(rx_buf+7) == RM_COMMAND_T)&&(*(rx_buf+len-2) != 0x1 ) )
        {
            command = *(rx_buf+len-2);

            if(RM_AUTODL_MODE == command) /* usb download mode */
            {
                osiReset(OSI_RESET_FORCE_DOWNLOAD);
                while(1) //never be here
                {
                }
            }

            if( (command & RM_U0_AS_DIAG) != 0 )
            {
                //UART0 used as DIAG(that is DEBUG_PORT)
                RM_SetUart0Mode(COM_DEBUG);
            }
            else
            {
                //UART0 used as AT(that is DATA_PORT)
                RM_SetUart0Mode(COM_DATA);
            }
                        /* in order to set 0x95 and 0x85 in  autotestmode */
            if (command == 0x95)
                RM_Set95Mode(0x01);
             if(command == 0x85)
                RM_Set95Mode(0x02);

            command &= 0x0f;
            s_calibratiomode = command;

            //disable arm deep sleep.
            SCI_Calibration_EnableDeepSleep(0);

            //In calibration mode.
            if ( (command == RM_CALIBRATION_MOD )
               ||(command == RM_TD_CALIBRATION_MOD)
               ||(command == RM_LTE_CALIBRATION_MOD)) {
                BOOT_SetResetMode (CALIBRATION_MODE);

                RM_SetCalibrationMode(SCI_TRUE);

            }
            //In calibration paramter check mode:not start MMI
            else if ((command  == RM_CALIBR_POST_MOD )||(command  == RM_TD_CALIBR_POST_MOD))
            {
                BOOT_SetResetMode (CALIBRATION_POST_MODE);

                frequency = ((*(rx_buf+1))<<8)+*(rx_buf+2);
                POWER_SetCalibrationPostModeFreq(frequency);

                RM_SetCalibrationPostMode(SCI_TRUE);
            }
            else if (( command  == RM_CALIBR_POST_NO_LCM_MODE )||( command  == RM_TD_CALIBR_POST_NO_LCM_MODE ))
            {
                BOOT_SetResetMode (CALIBRATION_POST_NO_LCM_MODE);

                RM_SetCalibrationPostNoLcmMode(SCI_TRUE);

                frequency = ((*(rx_buf+1))<<8)+*(rx_buf+2);
                POWER_SetCalibrationPostModeFreq(frequency);
            }
            //In boot mode.
            else if (command  == RM_BOOT_MOD )
            {
                //POWER_BootReset();

                // Change MCU mode.
                BOOT_SetResetMode (RESET_MODE);

                SCI_DisableIRQ();
                HAL_MCUBootReset();
                SCI_RestoreIRQ();
            }
            //In production UT test mode.
            else if ( command  == RM_PIN_TEST_MOD )
            {
                BOOT_SetResetMode (PIN_TEST_MODE);

                RM_SetPinTestFlag(SCI_TRUE);    //in production-test mode.
            }
            else if (command  == RM_IQC_TEST_MOD)
            {
                BOOT_SetResetMode (IQC_TEST_MODE);

                RM_SetIQCTestFlag(SCI_TRUE);
                RM_SetPinTestFlag(SCI_TRUE);
            }
            else if(command == RM_CALIBR_NV_ACCESS_MODE)
            {
                BOOT_SetResetMode (CALIBRATION_NV_ACCESS_MODE);
            }
        }
    }
}

/******************************************************************************/
//  Description:  get current network state for calipost mode
//    Global resource dependence:
//  Author:
//    Note:          s_calibratiomode
//                return 1: under td mode, 0: gsm mode
/******************************************************************************/
PUBLIC uint32  Calibration_GetNetWork()/*lint -esym(765,Calibration_GetNetWork )*/
{
    if((s_calibratiomode == RM_TD_CALIBRATION_MOD)  \
       ||(s_calibratiomode == RM_TD_CALIBR_POST_MOD) \
       ||(s_calibratiomode == RM_TD_CALIBR_POST_NO_LCM_MODE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/******************************************************************************/
//  Description:  set calibration mode according to reset mode
//    Global resource dependence:
//  Author:
//    Note:          s_calibratiomode
//                return none
/******************************************************************************/
PUBLIC void RM_HandlePowerResetMode(uint32 mode)
{

    switch(mode)
    {
        case TD_CALIBRATION_POST_MODE:
            s_calibratiomode = RM_TD_CALIBR_POST_MOD;
            break;
        case TD_CALIBRATION_POST_NO_LCM_MODE:
            s_calibratiomode = RM_TD_CALIBR_POST_NO_LCM_MODE;
            break;
        case TD_CALIBRATION_MODE:
            s_calibratiomode = RM_TD_CALIBRATION_MOD;
            break;
        default:
            s_calibratiomode = 0;
            break;
    }
}
/******************************************************************************/
//  Description:  send mode require command to PC or other device.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:
/******************************************************************************/
extern uint32 UART_PHY_WriteData (uint32 port, const uint8 *buffer, uint32 length, BOOLEAN irq_en);

LOCAL void RM_SendModeReq(void)
{
    uint8 temp;
    COMMAND_T    packet;

    packet.reserved   = 0x00;
    packet.frameSize  = RM_FIXED_PROTO_LEN - 2;
    packet.command    = RM_COMMAND_T;
    packet.subCommand = RM_RUNMODE_REQ_COMMAND;


#if 0
    *(volatile uint32 *)RM_UART = RM_PROTO_HEAD;

    for(i = 0;  i < (int)sizeof(COMMAND_T);  i++)
        *(volatile uint32 *)RM_UART = * ( (uint8 *)&packet + i );

    *(volatile uint32 *)RM_UART = RM_PROTO_HEAD;
#endif
    temp = RM_PROTO_HEAD;
    UART_PHY_WriteData(UART_COM0,&temp,1,FALSE);
    UART_PHY_WriteData(UART_COM0,(uint8 *)&packet,sizeof(COMMAND_T),FALSE);
    UART_PHY_WriteData(UART_COM0,&temp,1,FALSE);
}


/******************************************************************************/
//  Description:   Open the uart used in RUN MODE
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:
/******************************************************************************/

LOCAL void RM_OpenCom(void)
{
#if defined(PLATFORM_UWS6121E)
    uint32 BaudRate = 921600; //200000(8910)
#else
    uint32 BaudRate = 115200;
#endif

#if defined(CALI_MODE_PORT_UART0)
    SIO_InitUartLowLevel(UART_COM0, BaudRate );
#else
    SIO_InitUartLowLevel(UART_COM0, BaudRate );
#endif
}

/******************************************************************************/
//  Description:  Get the ARM run mode
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:
/******************************************************************************/
extern void UART_PHY_SetFifoClr (uint32 port, uint32 clear_flag);
extern uint32 UART_PHY_GetRxFifoCnt (uint32 port);
extern int32 UART_PHY_GetChar (uint32 port);

PUBLIC void RM_SendRunModeRequest(void)
{
    uint32     i, tmp, count;

    RM_OpenCom();
#if defined(PLATFORM_UWS6121E)
    //clr rx fifo
    UART_PHY_SetFifoClr (UART_COM0, 2); //CLR RX FIFO

#if 0
    // clear the rx fifo of UART
    count = ( *(volatile uint32*)(RM_UART+0x20) ) & 0xFF; // Rx fifo count
    for(i = 0;  i < count; i++)
    {
        tmp = *(volatile uint32*)(RM_UART+0x04);
    }
    //@Nick.Zhao clear the RX fifo of Uart for CR26233
    for(i = 0; i < UART_HAL_FifoSize(UART_COM2); i++)  /*lint !e737 */
    {
        tmp = *(volatile uint32*)(RM_UART+0x04);
    }
#endif

#if 0
    {
        //put gpio20 pin to uart3_rx/gpio21 pin to uart3_tx/gpio22 pin to uart3_cts/gpio23 pin to uart3_rts
        CHIP_REG_AND(PIN_GPIO_20_CFG_REG, ~0xF);
        CHIP_REG_OR(PIN_GPIO_20_CFG_REG, 2);
        CHIP_REG_AND(PIN_GPIO_21_CFG_REG, ~0xF);
        CHIP_REG_OR(PIN_GPIO_21_CFG_REG, 2);
        CHIP_REG_AND(PIN_GPIO_22_CFG_REG, ~0xF);
        CHIP_REG_OR(PIN_GPIO_22_CFG_REG, 2);
        CHIP_REG_AND(PIN_GPIO_23_CFG_REG, ~0xF);
        CHIP_REG_OR(PIN_GPIO_23_CFG_REG, 2);
        CHIP_REG_AND(PIN_GPIO_19_CFG_REG, ~0xF);
    }
#endif
#else
#if 0
    // clear the rx fifo of UART
    count = ( *(volatile uint32*)(RM_UART+0x0C) ) & 0xFF; // Rx fifo count
    for(i = 0;  i < count; i++)
    {
        tmp = *(volatile uint32*)(RM_UART+0x04);
    }
    //@Nick.Zhao clear the RX fifo of Uart for CR26233
    for(i = 0; i < UART_HAL_FifoSize(UART_COM0); i++)  /*lint !e737 */
    {
        tmp = *(volatile uint32*)(RM_UART+0x04);
    }
#endif
#endif
    RM_SendModeReq();

    // Wait for RM_RESPONSE_WAIT_TIME (500) ms to get the responce
    runmode_request_time = SCI_GetTickCount();
}

/******************************************************************************/
//  Description:  Get the ARM run mode
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:
/******************************************************************************/
extern void Debug_Uart4_puts(char *Str);

PUBLIC void RM_GetRunModeResponse(void)
{
    uint32     i, count;
    uint32  tick_new;
    uint8    rx_buffer[64];
    uint8    fifo_data_len;
    uint32    mode;
    uint8   *buffer_ptr = rx_buffer;

    //    probe Set Mode Command for RM_RESPONSE_WAIT_TIME ms.
    do{
#if !defined(PLATFORM_UWS6121E)
        count = *(volatile uint32*)(RM_UART+0x0C) & 0xFF;
#else
        count = UART_PHY_GetRxFifoCnt(UART_COM0);
#endif
        if(count >= RM_FIXED_PROTO_LEN)
        {
            break;
        }
        tick_new = SCI_GetTickCount();
        if(tick_new <= runmode_request_time)
        {
            tick_new = runmode_request_time;
        }
    }while((tick_new-runmode_request_time) < RM_RESPONSE_WAIT_TIME );

    if( count >= RM_FIXED_PROTO_LEN )
    {
        //receive Set Mode command
        SCI_MEMSET(rx_buffer, 0, sizeof(rx_buffer));
        for(i=0; i<RM_FIXED_PROTO_LEN; i++)
        {
            //*(buffer_ptr ++) = *(volatile uint32 *)(RM_UART+0x04);
            *(buffer_ptr ++) = UART_PHY_GetChar (UART_COM0);
        }
        mode = *(rx_buffer+RM_FIXED_PROTO_LEN-2);
        if(mode == 0x8a)
        {////receive first command in CALIBRATION_NV_ACCESS_MODE
            rm_cmd_buffer = (uint8*)SCI_ALLOC_APP(MAX_IMEI_CMD_LEN);
            if(rm_cmd_buffer != NULL)
            {
                while((fifo_data_len = UART_PHY_GetRxFifoCnt(UART_COM0) & 0xFF)==0){};
                cmd_data_len = RM_RecDataFromTool(rm_cmd_buffer);
            }
        }
        RM_SetRunMode(rx_buffer, RM_FIXED_PROTO_LEN);
        RM_SetDloaderMode(0);
        Debug_Uart4_puts("RM_GetRunModeResponse - 10\n");
    }
    else
    {
        Debug_Uart4_puts("RM_GetRunModeResponse - 0\n");
    }
}

/******************************************************************************/
//  Description:  Get the ARM run mode
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:
/******************************************************************************/
PUBLIC void RM_GetRunMode(void) //NO USE
{
    uint32     i, tmp, count;
    uint32  tick_old, tick_new;
    uint8    rx_buffer[128];
    uint8   *buffer_ptr = rx_buffer;

    SCI_MEMSET(rx_buffer, 0, sizeof(rx_buffer));

    RM_OpenCom();

    // clear the rx fifo of UART

    count = ( *(volatile uint32*)(RM_UART+0x0C) ) & 0xFF; // Rx fifo count
    for(i=0; i<count; i++)
    {
        tmp = *(volatile uint32*)(RM_UART+0x04);
    }
    //@Nick.Zhao clear the RX fifo of Uart for CR26233
    for(i=0; i<128; i++)
    {
        tmp = *(volatile uint32*)(RM_UART+0x04);
    }

    RM_SendModeReq();

    // wait for RM_RESPONSE_WAIT_TIME (500) ms to get the responce

    tick_old = tick_new = SCI_GetTickCount();

    while( (tick_new-tick_old) < RM_RESPONSE_WAIT_TIME )
    {
        tick_new = SCI_GetTickCount();
    }

    //read the received chars in rx fifo of uart.

    count = *(volatile uint32*)(RM_UART+0x0C) & 0xFF;

    if( count == RM_FIXED_PROTO_LEN )
    {
        for(i=0; i<count; i++)
        {
            *(buffer_ptr ++) = *(volatile uint32 *)(RM_UART+0x04);
        }

        RM_SetRunMode(rx_buffer, RM_FIXED_PROTO_LEN);
        RM_SetDloaderMode(0);
    }

}

/******************************************************************************/
//  Description:   when usb is connect, first checking uart, then checking usb
//    Global resource dependence:
//  Author:        weihua.wang
//    Note:
/******************************************************************************/

#if 0//def USB_ENTER_CALI_USE_ROM_API

extern void rom_usb_init(void);
extern int rom_usb_getchar(void);
extern int rom_usb_putchar(char ch);
extern void rom_usb_flush(void);
extern uint32 rom_usb_getdev_state(void);
extern void rom_usb_deinit(void);

PUBLIC void RM_GetVcomRunMode(void)
{
    uint32     count, value;
    uint32  old_tick, new_tick, last_tick;
    uint32  pwrkey_sts, chg_sts ;
    uint32  condition;
    int32   rec_char;
    uint8    rx_buffer[32];

    SCI_MEMSET(rx_buffer, 0, sizeof(rx_buffer));
    s_usb_calibration_mode = 1;
    /* usb init*/
    rom_usb_init();

    old_tick   = new_tick = SCI_GetTickCount();
    last_tick  = new_tick;
    value      = 0;
    count      = 0;
    pwrkey_sts = KEY_OFF;
    chg_sts    = KEY_OFF;
    condition  = POWER_GetRestartCondition();

    /*
    * here we don't care new_tick less than old_tick
    * because syscnt is set to zero when hw reset
    */
    while( (new_tick - old_tick) < RM_VCOM_MAX_WAIT_TIME )
    {
        new_tick = SCI_GetTickCount();
        if (last_tick != new_tick)
        {
            last_tick = new_tick;
            value++;
        }

           /*
           * after usb ldo is on, we should get the standard/vendor/class cmd right away
           * vcom dev state set to present/ready only when received data as below
           * 21 22 ** ** 00 00 00 00
           * so we can judge whether usb cft line is connected by this info
           */
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        if (VCOM_DEV_NO_PRESENT == rom_usb_getdev_state())
        {
            if ((new_tick - old_tick) > RM_VCOM_INITEND_MAXTIME)
            {
                   if(VCOM_DEV_NO_PRESENT == rom_usb_getdev_state())
                   {
                    rom_usb_deinit();
                    s_usb_calibration_mode = 0;
                    return;
                   }
            }
        }
#else
        if(VCOM_DEV_NO_PRESENT == rom_usb_getdev_state())
        {
            if((new_tick - old_tick) > RM_VCOM_INITEND_MAXTIME)
            {
                if(VCOM_DEV_NO_PRESENT == rom_usb_getdev_state())
                {
                    rom_usb_deinit();
                    return;
                }
            }
        }

        if (value%512 == 0)
        {
            /*
            *check staus of power key, rtc alarm, charge on per 512ms
            */

            /*power key detect*/
            if(SCI_TRUE == CheckPowerButtonState())
            {
                if(pwrkey_sts == KEY_OFF)
                {
                    pwrkey_sts = KEY_ON;
                }
                else
                {
                    condition = RESTART_BY_POWER_BUTTON;
                    break;
                }
            }
            else
            {
                pwrkey_sts = KEY_OFF;
                if(CheckRTCState() & BIT_4)
                {
                    /*rtc alarm detect*/
                    condition = RESTART_BY_ALARM;
                    break;
                }

                if(CHGMNG_IsChargeConnect())
                {
                    /*charge on detect*/
                    if(chg_sts == KEY_OFF)
                    {
                        chg_sts = KEY_ON;
                    }
                    else
                    {
                        condition = RESTART_BY_CHARGE;
                        break;
                    }
                }
                else
                {
                    chg_sts    = KEY_OFF;
                }

            }/*if(SCI_TRUE == CheckPowerButtonState())*/
        }/*if(value%512 == 0)*/
#endif

        // receive data from pc to out endp 2/4
        if (-1 != (rec_char = rom_usb_getchar()))
        {
            rx_buffer[count] = (uint8)(rec_char);
            count++;
        }

        if (count == RM_FIXED_PROTO_LEN)
        {
            RM_SetRunMode(rx_buffer, RM_FIXED_PROTO_LEN);
            RM_SetDloaderMode(1);

            /*
            * send the received cmd back to pc
            */
            for(value = 0; value < count; value++)
            {
                rom_usb_putchar(rx_buffer[value]);
            }

            rom_usb_flush();

            if (s_calibratiomode != RM_CALIBR_NV_ACCESS_MODE)
            {
                old_tick = new_tick = SCI_GetTickCount();
                while((new_tick - old_tick) < RM_VCOM_MAX_DELAY_TIME)
                  {
                      rom_usb_getchar();
                    if (VCOM_DEV_NO_PRESENT == rom_usb_getdev_state())
                    {
                        break;
                    }
                    new_tick = SCI_GetTickCount();
                }
                rom_usb_deinit();
            }
            return;
        }
    }

    if ((new_tick - old_tick) < RM_VCOM_MAX_WAIT_TIME)
    {
        POWER_SetRestartCondition(condition);
    }

    s_usb_calibration_mode = 0;
    rom_usb_deinit();
}

#else

PUBLIC void RM_GetVcomRunMode(void)
{
    uint32     count, value;
    uint32  old_tick, new_tick, last_tick;
    uint32  pwrkey_sts, chg_sts ;
    uint32  condition;
    int32   rec_char;
    uint8    rx_buffer[32];

    SCI_MEMSET(rx_buffer, 0, sizeof(rx_buffer));
    s_usb_calibration_mode = 1;
    /* usb init*/
    VCOM_usb_boot();

    old_tick   = new_tick = SCI_GetTickCount();
    last_tick  = new_tick;
    value      = 0;
    count      = 0;
    pwrkey_sts = KEY_OFF;
    chg_sts    = KEY_OFF;
    condition  = POWER_GetRestartCondition();

    /*
    * here we don't care new_tick less than old_tick
    * because syscnt is set to zero when hw reset
    */
    while( (new_tick - old_tick) < RM_VCOM_MAX_WAIT_TIME )
    {
        new_tick = SCI_GetTickCount();
        if (last_tick != new_tick)
        {
            last_tick = new_tick;
            value++;
        }

           /*
           * after usb ldo is on, we should get the standard/vendor/class cmd right away
           * vcom dev state set to present/ready only when received data as below
           * 21 22 ** ** 00 00 00 00
           * so we can judge whether usb cft line is connected by this info
           */
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        if (VCOM_DEV_NO_PRESENT == VCOM_GetDevState())
        {
            if ((new_tick - old_tick) > RM_VCOM_INITEND_MAXTIME)
            {
                   if(VCOM_DEV_NO_PRESENT == VCOM_GetDevState())
                   {
                    VCOM_disconnect();
                    s_usb_calibration_mode = 0;
                    return;
                   }
            }
        }
#else
        if(VCOM_DEV_NO_PRESENT == VCOM_GetDevState())
        {
            if((new_tick - old_tick) > RM_VCOM_INITEND_MAXTIME)
            {
                if(VCOM_DEV_NO_PRESENT == VCOM_GetDevState())
                {
                    VCOM_disconnect();
                    return;
                }
            }
        }

        if (value%512 == 0)
        {
            /*
            *check staus of power key, rtc alarm, charge on per 512ms
            */

            /*power key detect*/
            if(SCI_TRUE == CheckPowerButtonState())
            {
                if(pwrkey_sts == KEY_OFF)
                {
                    pwrkey_sts = KEY_ON;
                }
                else
                {
                    condition = RESTART_BY_POWER_BUTTON;
                    break;
                }
            }
            else
            {
                pwrkey_sts = KEY_OFF;
                if(CheckRTCState() & BIT_4)
                {
                    /*rtc alarm detect*/
                    condition = RESTART_BY_ALARM;
                    break;
                }

                if(CHGMNG_IsChargeConnect())
                {
                    /*charge on detect*/
                    if(chg_sts == KEY_OFF)
                    {
                        chg_sts = KEY_ON;
                    }
                    else
                    {
                        condition = RESTART_BY_CHARGE;
                        break;
                    }
                }
                else
                {
                    chg_sts    = KEY_OFF;
                }

            }/*if(SCI_TRUE == CheckPowerButtonState())*/
        }/*if(value%512 == 0)*/
#endif

        // receive data from pc to out endp 2/4
        if (-1 != (rec_char = VCOM_GetChar()))
        {
            rx_buffer[count] = (uint8)(rec_char);
            count++;
        }

        if (count == RM_FIXED_PROTO_LEN)
        {
            RM_SetRunMode(rx_buffer, RM_FIXED_PROTO_LEN);
            RM_SetDloaderMode(1);

            /*
            * send the received cmd back to pc
            */
            for(value = 0; value < count; value++)
            {
                VCOM_PutTxChar(rx_buffer[value]);
            }

            VCOM_Flush();

            if (s_calibratiomode != RM_CALIBR_NV_ACCESS_MODE)
            {
                old_tick = new_tick = SCI_GetTickCount();
                while((new_tick - old_tick) < RM_VCOM_MAX_DELAY_TIME)
                  {
                      VCOM_GetChar();
                    if (VCOM_DEV_NO_PRESENT == VCOM_GetDevState())
                    {
                        break;
                    }
                    new_tick = SCI_GetTickCount();
                }
                VCOM_disconnect();
            }
            return;
        }
    }

    if ((new_tick - old_tick) < RM_VCOM_MAX_WAIT_TIME)
    {
        POWER_SetRestartCondition(condition);
    }

    s_usb_calibration_mode = 0;
    VCOM_disconnect();
}

#endif

/******************************************************************************/
//  Description:  Get the ARM run mode for DIAG command
//    Global resource dependence:
//  Author:         shijun.cui
//    Note:
/******************************************************************************/
PUBLIC uint32 RM_GetMcuMode()
{
    MCU_MODE_E mode;
    uint32      ret = 0;

    mode = POWER_GetResetMode();
    switch(mode)
    {
        case CALIBRATION_MODE:
            ret = RM_CALIBRATION_MOD;
            break;
        case TD_CALIBRATION_MODE:
            ret = RM_TD_CALIBRATION_MOD;
            break;
        case CALIBRATION_POST_MODE:
            ret = RM_CALIBR_POST_MOD;
            break;
        case TD_CALIBRATION_POST_MODE:
            ret = RM_TD_CALIBR_POST_MOD;
            break;
        case RESET_MODE:
            ret = RM_BOOT_MOD;
            break;
        case PIN_TEST_MODE:
            ret = RM_PIN_TEST_MOD;
            break;
        case IQC_TEST_MODE:
            ret = RM_IQC_TEST_MOD;
            break;
        case CALIBRATION_POST_NO_LCM_MODE:
            ret = RM_CALIBR_POST_NO_LCM_MODE;
            break;
        case TD_CALIBRATION_POST_NO_LCM_MODE:
            ret = RM_TD_CALIBR_POST_NO_LCM_MODE;
            break;
        default:
            break;
    }
    if(RM_GetEnableArmSleepFlag() == SCI_TRUE)
    {
        //enable arm sleep
        ret |= RM_ENABLE_SLEEP;
    }
    if(RM_GetUart0Mode() == COM_DEBUG)
    {
        ret |= RM_U0_AS_DIAG;
    }

    return ret;
}

LOCAL int32 RM_GetChar(uint32 type)
{
    int32 ret = -1;
    if(RM_FROM_UART == type)
    {
        #if 0
        if(( *(volatile uint32*)(RM_UART+0x0C) ) & 0xFF)
        {
             ret = *(volatile uint32*)(RM_UART+0x04);
        }
        #endif

        if((UART_PHY_GetRxFifoCnt(UART_COM0)) & 0xFF)
        {
             ret = UART_PHY_GetChar(UART_COM0);
        }
    }
    else if(RM_FROM_USB == type)
    {
        ret =  VCOM_GetChar();
    }
    else{
        ret =  -1;
    }

    return ret;
}

/*
* type:0, by uart, 1: by usb
*/
LOCAL void RM_SendChar(uint8 ch, uint32 type)
{
    uint32 count = 0;

    if(RM_FROM_USB == type)
    {
        VCOM_PutTxChar(ch)  ;
    }
    else if(RM_FROM_UART == type)
    {
        #if 0
        do{
            count = (*(volatile uint32*)(getRMUartBaseAddr() + 0x0C) >>8 )&0xFF;
        }
        while(count >= UART_HAL_FifoSize(UART_COM0));

        *(volatile uint32 *)RM_UART  = ch;
        #endif
        UART_PHY_WriteData(UART_COM0,&ch,1,FALSE);
    }
    else
    {}
}

/*
flush remaind data to pc
*/
extern void UART_PHY_WaiTxFifoEmpty (uint32 port, uint32 fifo_size);

LOCAL void RM_SendFlush(uint32 type)
{
    int32 count;
    if(RM_FROM_USB == type)
    {
        VCOM_Flush();
    }
    else if(RM_FROM_UART == type)
    {
        /*wait uart tx fifo flush out*/
        #if 0
        do{
            count = (*(volatile uint32*)(getRMUartBaseAddr() + 0x0C) >>8 ) & 0xFF;
        }while(count > 0);
        #endif
        UART_PHY_WaiTxFifoEmpty (UART_COM0, 0);
    }
    else
    {}
}


/*
*send  reponse to tools
src: data to sent
size: data size to sent
*/
 void RM_SendRes(uint8* src, int32 size)
{
    uint32 chnl;
    int32 i;
    uint8 value;

    if(1 == RM_GetDloaderMode())
    {
        chnl = RM_FROM_USB;
    }
    else
    {
        chnl = RM_FROM_UART;
    }

    RM_SendChar(RM_PROTO_HEAD, chnl);

    for(i = 0; i < size;  i++)
    {
         value = *src++;
         if(value == 0x7E)
         {
            RM_SendChar(0x7D, chnl);
        RM_SendChar(0x5E, chnl);
         }
     else if(value == 0x7D)
     {
            RM_SendChar(0x7D, chnl);
        RM_SendChar(0x5D, chnl);
     }
     else
     {
            RM_SendChar(value, chnl);
     }
    }

    RM_SendChar(RM_PROTO_HEAD, chnl);

    RM_SendFlush(chnl);

}

/*
* receive data from tools
* desPtr: destination data buffer
* return value: received data size except header and tail's 0x7e
*/
 LOCAL uint32 RM_RecDataFromTool(uint8 * desPtr)
{
    int32 rec_char ;
    uint32 s_frame_type, cnt, type;

    rec_char         = -1;
    s_frame_type  = RM_FRAME_NONE;
    cnt                 = 0;

    if(1 == RM_GetDloaderMode())
    {
        type = RM_FROM_USB;
    }
    else
    {
        type = RM_FROM_UART;
    }

    //The more conventional form of
    //  infinite loop prefix is for(;;)
    //while(1)
    for(;;)
    {
        if(RM_FRAME_TAIL == s_frame_type)
        {
        break;
        }

        if(-1 == (rec_char  = RM_GetChar(type)))
        {
            continue;
        }

        if ((rec_char != RM_PROTO_HEAD)&&(s_frame_type != RM_FRAME_HEAD))
        {
            continue;
        }

        if (rec_char == RM_PROTO_HEAD)
        {
            switch(s_frame_type){
                case RM_FRAME_NONE:
                    s_frame_type = RM_FRAME_HEAD;
                    break;
                case RM_FRAME_HEAD:
                    s_frame_type = RM_FRAME_TAIL;
                    break;
                default:
                    break;
            }
        }
        else if (rec_char == RM_PROTO_ESCAPE)
        {
            while(-1 == (rec_char  = RM_GetChar(type))){};
            if(rec_char == 0x5D)
         {
                    *desPtr++ = 0x7D;
         }
         else  if(rec_char == 0x5E)
         {
                 *desPtr++ = 0x7E;
         }
         else
         {
                 *desPtr++ = rec_char;
         }
            cnt++;
        }
     else
     {
             *desPtr++ = rec_char;
             cnt++;
        }
    }
    return cnt;
}
uint32  IMEI_RM_RecvCMD(uint8 *buffer)
{
    uint32 i;
    if(cmd_data_len == 0)
    {
            return RM_RecDataFromTool(buffer);
    }
    else if(cmd_data_len < MAX_IMEI_CMD_LEN)
    {
            for(i=0;i<cmd_data_len;i++)
            {
                buffer[i] = rm_cmd_buffer[i];
            }
            cmd_data_len = 0;
            SCI_FREE(rm_cmd_buffer);
            return i;
    }
    return 0;
}
#if 0
  //relealized in imei.c
/*
* RM_NV_WriteMode handle, called in init.c SCI_InitRef
* do the precedure
* rec data
*    -> handle data
*           -> write nv
*                   -> retrun response
*                                -> while(1)
*/
PUBLIC void RM_Cali_NvAccess(void)
{
    uint8 * rm_recdata_fromtool = NULL;
    uint8               * nv_org_ptr          = NULL;
    REF_NVWriteDirect_T * rm_recdata_ptr      = NULL;
    MSG_HEAD_T          * cmdinfo_ptr         = NULL;
    TEST_DATA_INFO_T    * prod_info_ptr       = NULL;
    uint32       rec_count, size;
    MSG_HEAD_T   cmd_head ;
    BT_NV_PARAM    nv_param;
    uint8 subtype,writed_type;
    BOOLEAN status;

    /*write imei, bt address,gps config directly*/
    /*got data uart/usb should all surpport*/
    /*dispatch data, according mask bit write nv*/
    /*write data according mask*/
    /*return res according result*/

    /*in nor+sram platform, return buf base address
    *  in nand+sdram, return 0 directly
    */
    nv_org_ptr = (uint8*)NVITEM_MallocCaliBuf();

    rm_recdata_fromtool    = (uint8*)SCI_ALLOC_APP(sizeof(REF_NVWriteDirect_T) + 10);

    SCI_MEMSET(rm_recdata_fromtool, 0, (sizeof(REF_NVWriteDirect_T) + 10));

    /*got data uart/usb should all surpport*/
    /*rec data format as below
       7e + msg header(seq num 4byte + len 2 byte + type 1byte + subtype 1byte) +
       data ( REF_NVWriteDirect_T , imei1, imei2, bt address, gps address ) +  7e
    */
    rec_count = RM_RecDataFromTool(rm_recdata_fromtool);

#ifdef _RM_DEBUG
    RM_SendRes(rm_recdata_fromtool,rec_count);
#endif

    cmdinfo_ptr =    (MSG_HEAD_T*)rm_recdata_fromtool;
    /**/
    rm_recdata_ptr =  (REF_NVWriteDirect_T*)(rm_recdata_fromtool + 8);

    cmd_head.seq_num = cmdinfo_ptr->seq_num;
    cmd_head.len          = cmdinfo_ptr->len;
    cmd_head.type        = cmdinfo_ptr->type;
    cmd_head.subtype   = cmdinfo_ptr->subtype;

    if((rec_count != cmdinfo_ptr->len)||(DIAG_DIRECT_NV !=  cmdinfo_ptr->type))
    {
        /*rec data size is not equal, or cmd type is not valid, return false*/
     status = SCI_FALSE;
    }
    else
    {
        subtype = cmd_head.subtype ;
        writed_type = 0;

        if(subtype & RM_CALI_NV_IMEI1_MSK)
        {
            if(NVERR_NONE == NVITEM_UpdateCaliBuf(NV_IMEI, MN_MAX_IMEI_LENGTH, rm_recdata_ptr->imei1, nv_org_ptr))
            {
                writed_type |= RM_CALI_NV_IMEI1_MSK;
            }
#ifdef _RM_DEBUG
//    RM_SendRes((uint8*)rm_recdata_ptr->imei1,MN_MAX_IMEI_LENGTH);
#endif
        }

        if(subtype & RM_CALI_NV_IMEI2_MSK)
        {
            if(NVERR_NONE == NVITEM_UpdateCaliBuf(NV_IMEI1, MN_MAX_IMEI_LENGTH, rm_recdata_ptr->imei2, nv_org_ptr))
            {
                writed_type |= RM_CALI_NV_IMEI2_MSK;
            }

#ifdef _RM_DEBUG
//    RM_SendRes((uint8*)rm_recdata_ptr->imei2,MN_MAX_IMEI_LENGTH);
#endif

    }

        if(subtype & RM_CALI_NV_BT_MSK)
        {
            EFS_NvitemRead(NV_BT_CONFIG, sizeof(BT_NV_PARAM), (uint8*)&nv_param);
#ifdef _RM_DEBUG
//    RM_SendRes((uint8*)rm_recdata_ptr->bt_add.addr,sizeof(BT_ADDRESS));
//    RM_SendRes((uint8*)&nv_param,sizeof(BT_NV_PARAM));
#endif
            SCI_MEMCPY((uint8*)&(nv_param.bd_addr),  (uint8*)rm_recdata_ptr->bt_add.addr,  sizeof(BT_ADDRESS));
            if(NVERR_NONE ==NVITEM_UpdateCaliBuf(NV_BT_CONFIG,  sizeof(BT_NV_PARAM),  (uint8*)&nv_param,  nv_org_ptr))
            {
                writed_type |= RM_CALI_NV_BT_MSK;
            }

#ifdef _RM_DEBUG
//    RM_SendRes((uint8*)&nv_param,sizeof(BT_NV_PARAM));
#endif

        }

        if(subtype & RM_CALI_NV_GPS_MSK)
        {
            if(NVERR_NONE ==NVITEM_UpdateCaliBuf(NV_GPS_PARAM, sizeof(GPS_NV_INFO_T) , (uint8*)&(rm_recdata_ptr->gps_info), nv_org_ptr))
            {
                writed_type |= RM_CALI_NV_GPS_MSK;
            }

#ifdef _RM_DEBUG
//    RM_SendRes((uint8*)&(rm_recdata_ptr->gps_info),sizeof(GPS_NV_INFO_T));
#endif
        }

        if(subtype & (RM_CALI_NV_SN_MSK | RM_CALI_NV_GUID_MSK))
        {
            /*it's prod test info, diffierent with fixed nv item*/
            /*first alloc memory for TEST_DATA_INFO_T*/
            /*read production partition info from flash*/
            /*update info according flag*/
            /*write back to flash*/

            size = sizeof(TEST_DATA_INFO_T);
            prod_info_ptr = (TEST_DATA_INFO_T *)SCI_ALLOC(size + 4);

            NVITEM_GetProductInfo((uint8*)prod_info_ptr,size);

            if(subtype & RM_CALI_NV_SN_MSK)
            {
#ifdef _RM_DEBUG
    RM_SendRes(rm_recdata_ptr->sn,SN_LEN);
    RM_SendRes((uint8*)prod_info_ptr->header.SN,SN_LEN);
#endif
                /*write production SN info*/
                SCI_MEMCPY((uint8*)prod_info_ptr->header.SN, rm_recdata_ptr->sn, SN_LEN);

                writed_type |= RM_CALI_NV_SN_MSK;

            }

            if(subtype & RM_CALI_NV_GUID_MSK)
            {
#ifdef _RM_DEBUG
    RM_SendRes(rm_recdata_ptr->guid,GUID_LEN);
    RM_SendRes((uint8*)prod_info_ptr->header.Guid, GUID_LEN);
#endif
                /*write production guid info*/
                SCI_MEMCPY((uint8*)prod_info_ptr->header.Guid, rm_recdata_ptr->guid, GUID_LEN);

                writed_type |= RM_CALI_NV_GUID_MSK;
            }
        }

        status = SCI_TRUE;

        if(subtype)
        {
            /*update nv*/
            if(subtype == writed_type)
            {
                /*in nandflash+sdram, nvupdate is performed at NVITEM_UpdateCaliBuf()
                * so we should calc the ret value according the return of NVITEM_UpdateCaliBuf
                 * in nor+sram , NVITEM_UpdateCaliBuf always return true
                */
                if(subtype & (RM_CALI_NV_IMEI1_MSK| RM_CALI_NV_IMEI2_MSK | RM_CALI_NV_BT_MSK| RM_CALI_NV_GPS_MSK))
                {
                    status &= NVITEM_UpdateNvParam(nv_org_ptr);
                }

                if(subtype & (RM_CALI_NV_GUID_MSK| RM_CALI_NV_SN_MSK))
                {
                    status &= NVITEM_UpdateProductInfo(FLASH_GetProductInfoAddr(),(uint8*)prod_info_ptr, size);  /*lint !e644 */
                }
            }
        }
        else
        {
            status = SCI_FALSE;
        }
    }

    if(status)
    {
        /*success*/
        cmd_head.subtype = SCI_TRUE;
    }else{
        /*fail*/
        cmd_head.subtype = SCI_FALSE;
    }

    /*send response*/
    cmd_head.len = sizeof(MSG_HEAD_T);
    RM_SendRes((uint8*)&cmd_head, sizeof(cmd_head));

    while(1);

    /*sci_free don't need here, for it's dead loop*/

}
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
