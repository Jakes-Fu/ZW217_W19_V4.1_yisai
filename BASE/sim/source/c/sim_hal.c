/******************************************************************************
** File Name:      sim_hal.c                                                 *
** Author:         Wuding.Yang                                               *
** Date:           2001.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the sim hal file for ISO7816-3 protocol           *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2001.2         Wuding.Yang      Create                                    *
******************************************************************************/

#include "base_trc.h"
#include "chip.h"// for CHIP_TYPE_E
#ifdef SIM_6302_SWITCH
#include "gpio_prod_api.h"//GPIO_SPI_Quadsim_Get_Config
#endif
#include "sim_include.h"
#ifdef SIM_PLUG_IN_SUPPORT//sim card hot swap
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
#include "gpio_prod_cfg.h"
#include "gpio_prod_api.h"
#include "diag.h"
#endif
#endif

#ifdef   __cplusplus
extern   "C"
{
#endif

#define YWD_L_TEST 1

//---------------Macro define--------------------------
#define SIMCTL0_BIT_SIM_RST             (1 << 6)        //Reset the sim card module 
#define SIMCTL0_BIT_POWER_EN            (1 << 7)        //Enable the Power supply to sim card 
#define SIMCTL0_BIT_TX_DATA_OUT_LOW     (1 << 15)       //Enable the SIM I/O status LOW 

#define SIM_STATUS_WORD_TIMER_VALUE     700//the value of the timer to judge the status works or SIM data in the rx expected length
#define SIM_ATR_TIMER_VALUE             300//the value of the timer to decide if the there are any ATR data from SIM when reset the SIM
#define SIM_RESET_TIMER_VALUE           300//the timer value to make sure the ATR data of the SIM will be all transmited between two continues SIM driver reset
#define SIM_WAIT_LDO_TIMER_VALUE        125//the timer value to make sure the LDO is valid
#define SIM_WAIT_PPS_TIMER_VALUE        1000//the timer value to make sure the PPS procedure is valid
#define SIM_WAIT_DECODE_ATR_TIMER_VALUE 3000//the timer value to make sure the PPS procedure is valid. ***case 11520 etu.
#define SIM_RST_TB_TE_TIMER_VALUE       20// 20ms >=(400/f), f is 3.25MHz. The value is about 0.1ms, but some SIM need a long RST low time..
#define SIM_HOTSWAP_STABLE_TIMER_VALUE  400//avoid the user plug-in and pull-out sim very quickly and continuously

#define SIM_MAX_NULL_BYTE_COUNT         10
#define SIM_DEFAULT_WATER_MARK_NUM      7
#define SIMHAL_CAL_RX_INT_MARK(data_num)   ((data_num) >= SIM_DEFAULT_WATER_MARK_NUM) ? SIM_DEFAULT_WATER_MARK_NUM : (data_num);

#ifdef SIM_ENHANCED_PPS_SUPPORT
//The interval between the start leading edge of any character sent by the
//card and the start leading edge of the previous character (sent either by
//the card or by the interface device).
#define SIM_DMA_RX_LONG_TIMER           4100  //CR151784
#define SIM_DMA_RX_NORMAL_TIMER         2500  //normal time
#define SIM_DMA_TX_MAX_LEN              16    //The MAX len of SIM DMA TX
#define SIM_DMA_RX_LAST_BYTE_AGAIN_TIMER 300  //To get the last 1 byte again time
#define SIM_DMA_RX_LAST_BYTE_TIMER      10   //To get the last 1 byte time

#define SIM_DEFAULT_WI                  10    //The default value of WI
#endif


//---------------Global variable--------------------------
#ifdef MULTI_SIM_SYS_QUAD
static SIMDRV_WORKING_VOLT_E    s_sim_working_volt[SIM_CARD_NUM] = {SIMDRV_WORKING_VOLT_1_8V, SIMDRV_WORKING_VOLT_1_8V, SIMDRV_WORKING_VOLT_1_8V, SIMDRV_WORKING_VOLT_1_8V}; //the working voltage of the SIM
static TB_SIM_STATUS_E  s_sim_status[SIM_CARD_NUM] = {TB_SIM_NO_CARD, TB_SIM_NO_CARD, TB_SIM_NO_CARD, TB_SIM_NO_CARD}; //the SIM driver status
#elif defined (MULTI_SIM_SYS_TRI)
static SIMDRV_WORKING_VOLT_E    s_sim_working_volt[SIM_CARD_NUM] = {SIMDRV_WORKING_VOLT_1_8V, SIMDRV_WORKING_VOLT_1_8V, SIMDRV_WORKING_VOLT_1_8V}; //the working voltage of the SIM
static TB_SIM_STATUS_E  s_sim_status[SIM_CARD_NUM] = {TB_SIM_NO_CARD, TB_SIM_NO_CARD, TB_SIM_NO_CARD}; //the SIM driver status
#else
static SIMDRV_WORKING_VOLT_E    s_sim_working_volt[SIM_CARD_NUM] = {SIMDRV_WORKING_VOLT_1_8V, SIMDRV_WORKING_VOLT_1_8V}; //the working voltage of the SIM
static TB_SIM_STATUS_E  s_sim_status[SIM_CARD_NUM] = {TB_SIM_NO_CARD, TB_SIM_NO_CARD}; //the SIM driver status
#endif
static BOOLEAN s_have_ack_byte[2];                   //has the ack byte of the command been received
static SCI_TIMER_PTR s_sim_reset_bt_timer[2] = {NULL, NULL}; //the timer to control the time between to continuious reset(reset to change bit convention) to make
//sure that the ATR data of the first reset has been all received
static SCI_TIMER_PTR s_sim_driver_timer[2] = {NULL, NULL};  //the timer to help distinguish the status word and the really
//response data from SIM card.
static SCI_TIMER_PTR s_sim_atr_timer[2] = {NULL, NULL};   //the timer to wait the atr
static SCI_TIMER_PTR s_sim_reset_p_timer[2] = {NULL, NULL}; //the timer to control the time between to continuious reset(reset to change parity check) to make
//sure that the ATR data of the first reset has been all received
//timer,wait ldo is valid,then to init the sim control register
static SCI_TIMER_PTR s_sim_init_wait_ldo_timer[2] = {NULL, NULL};

//timer,wait ldo is valid,then to reset the sim card
static SCI_TIMER_PTR s_sim_reset_wait_ldo_timer[2] = {NULL, NULL};

//timer, to ensure PPS procedure can be completed.
static SCI_TIMER_PTR s_sim_pps_timer[2] = {NULL, NULL};

//timer, to ensure decode ATR can be completed.
static SCI_TIMER_PTR s_sim_decode_atr_timer[2] = {NULL, NULL};

//timer, to keep the SIM RST low accoding to ISO7816-3
static SCI_TIMER_PTR s_sim_tb_te_timer[2] = {NULL, NULL};

#ifdef SIM_ENHANCED_PPS_SUPPORT
#ifdef SIM_3G_USIM_SUPPORT
//timer, to check the 3G status instruction un-expected status words
static SCI_TIMER_PTR s_sim_3g_status_check_timer[2] = {NULL, NULL};
#define SIM_3G_STATUS_CHECK_TIMER_VALUE     20//the value of the timer to check the 3G status instruction un-expected status words
#define SIM_3G_STATUS_CHECK_TIMER_COUNT     10//the count to check the 3G status instruction un-expected status words
static uint8 s_sim_3g_status_check_count[2] = {0};
#endif
#endif

#ifdef SIM_PLUG_IN_SUPPORT
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
//timer, wait the hot swap stable. In fact, we support 1 sim hot swap. so I think 1 timer is enough....
static SCI_TIMER_PTR s_sim_hot_swap_stable_timer = NULL;
#endif
#endif

#ifdef SIM_ENHANCED_PPS_SUPPORT//for dma debug
static SCI_TIMER_PTR s_sim_dma_last_byte_timer[2] = {NULL, NULL};
__align(4) static TB_SIM_RX_BUF_T s_sim_rx_buf[2];    //the rx buf to save the data received from SIM
#else
static TB_SIM_RX_BUF_T s_sim_rx_buf[2];    //the rx buf to save the data received from SIM
#endif

static TB_SIM_TX_BUF_T s_sim_tx_buf[2];    //the tx buf to save the data that to be sent to SIM
static BOOLEAN s_card_resetting[SIM_CARD_NUM];        //is the card current resetting.this state can make sure the reset funciton not called interater
static BOOLEAN s_first_protocol_occurs[SIM_CARD_NUM]; //whether the first protocol has been find in the ATR decode process
static BOOLEAN s_not_t0_protocol_occurs[SIM_CARD_NUM]; //whether the not t0 protocol has been find in the ATR decode process
static uint8 s_sim_reset_times[SIM_CARD_NUM];         //the times the SIM has been reset
static uint8 s_sim_pow_reset_times[SIM_CARD_NUM];     //the times that SIM has been reset for power change
static uint8 s_sim_b_reset_times[SIM_CARD_NUM];       //the times that the SIM has been reset for bit convetion and logic level change
static uint8 s_sim_p_reset_times[SIM_CARD_NUM];       //the tiems that the SIM has been reset for parity change
static uint8 s_default_pps_data[TB_SIM_PPS_DEFAULT_SPEED] = {0xff, 0x00, 0xff}; //the pps data that used in the PPS procedure as request and response.ref to GSM 11.11 section 5.8.2 "PPS procedure"
static TB_SIM_PPS_INFO_T  s_pps_info[SIM_CARD_NUM];       //the pps information
static TB_SIM_ATR_DATA_T  s_atr_data[SIM_CARD_NUM];       //the atr data of the SIM card
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN s_previous_pps_error[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE}; //is there are any PPS error happen,use this variable to handle the pps error.
static uint8 s_sim_pps_req_times[SIM_CARD_NUM] = {0, 0, 0, 0}; //the times that the SIM has send the pps request
#elif defined (MULTI_SIM_SYS_TRI)
static BOOLEAN s_previous_pps_error[SIM_CARD_NUM] = {FALSE, FALSE, FALSE}; //is there are any PPS error happen,use this variable to handle the pps error.
static uint8 s_sim_pps_req_times[SIM_CARD_NUM] = {0, 0, 0};  //the times that the SIM has send the pps request
#else
static BOOLEAN s_previous_pps_error[SIM_CARD_NUM] = {FALSE, FALSE}; //is there are any PPS error happen,use this variable to handle the pps error.
static uint8 s_sim_pps_req_times[SIM_CARD_NUM] = {0, 0};   //the times that the SIM has send the pps request
#endif
static const uint16 s_clk_dvd_factors[] = {2, 4, 8, 16, 32, 64, 128, 256}; //clk division factors array
#ifdef MULTI_SIM_SYS_QUAD

static SIMDRV_CLK_MODE_E s_sim_clk_mode[SIM_CARD_NUM] = {SIM_ARM_CLK_DIV_8, SIM_ARM_CLK_DIV_8, SIM_ARM_CLK_DIV_8, SIM_ARM_CLK_DIV_8}; //the sim clock division mode, we use 26M_APBCLK/8=3.25MHz.

#elif defined (MULTI_SIM_SYS_TRI)

static SIMDRV_CLK_MODE_E s_sim_clk_mode[SIM_CARD_NUM] = {SIM_ARM_CLK_DIV_8, SIM_ARM_CLK_DIV_8, SIM_ARM_CLK_DIV_8}; //the sim clock division mode, we use 26M_APBCLK/8=3.25MHz.

#else

static SIMDRV_CLK_MODE_E s_sim_clk_mode[SIM_CARD_NUM] = {SIM_ARM_CLK_DIV_8, SIM_ARM_CLK_DIV_8}; //the sim clock division mode, we use 26M_APBCLK/8=3.25MHz.

#endif
static BOOLEAN sim_response_data_present[2] = {FALSE, FALSE};   //whether the response data is present
static BOOLEAN sim_send_data_present[2] = {FALSE, FALSE};   //whether the send data is present (5bytes_cmd 1byte_ack send_data)
static BOOLEAN s_have_received_response_data[2] = {FALSE, FALSE};  //whether the ME has received the response data
static uint8 s_null_byte_count[2] = {0, 0};                    //the count of null byte(0x60),only receive 10 null byte to notify upper layer once
static uint8 s_sim_atr_data_buf[SIM_CARD_NUM][50];        //the buffer used to save the atr data
static uint8 s_sim_atr_data_len[SIM_CARD_NUM] = {0};        //the length of whole atr data

static SIM_CARD_INDEX_E simhal_current_sel_card = SIM_CARD_ONE;//current sim drv selected card id

#ifdef MULTI_SIM_SYS_QUAD
static uint32 s_sim_ctl0_value[SIM_CARD_NUM] = {0x2003, 0x2003, 0x2003, 0x2003}; //To store the two different sim card's ctl0 value.
static BOOLEAN s_simhal_has_1_8v_only[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};         //To record whether there is a 1.8V only SIM card
#elif defined (MULTI_SIM_SYS_TRI)
static uint32 s_sim_ctl0_value[SIM_CARD_NUM] = {0x2003, 0x2003, 0x2003}; //To store the two different sim card's ctl0 value.
static BOOLEAN s_simhal_has_1_8v_only[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};        //To record whether there is a 1.8V only SIM card
#else
static uint32 s_sim_ctl0_value[SIM_CARD_NUM] = {0x2003, 0x2003};
static BOOLEAN s_simhal_has_1_8v_only[SIM_CARD_NUM] = {FALSE, FALSE};           //To record whether there is a 1.8V only SIM card
#endif
static uint8 s_simhal_current_sim_task_id = 0;              //When power on,which SIM task is running.
//static BOOLEAN s_simhal_atr_decode_1_8v_only[SIM_CARD_NUM] = {FALSE,FALSE};//the ATR data has showed this is a 1.8V only card?
//static uint32 s_sim_sel_gpio_id = 2;      //the GPIO for select SIM
//static uint32 s_sim2_rst_gpio_id = 0; //the GPIO used as the SIM2's RST
//static uint32 s_sim2_clk_gpio_id = 37; //the GPIO37 used as the SIM2's CLK in H2 chip
//static uint32 s_sim2_vsim_gpio_id= 0;//the GPIO used as the SIM2's Vsim
//static BOOLEAN s_sim_switch_to_card1 = FALSE; //SIM_GPIO_SEL's value  is s_sim_switch_to_card1, select SIM card1
//static BOOLEAN s_sim_switch_to_card2 = FALSE; //SIM_GPIO_SEL's value  is s_sim_switch_to_card2, select SIM card2
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN s_simhal_1_8v_detect_complete[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
static BOOLEAN s_simhal_atr_volt_get_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
static uint8 s_simhal_atr_volt_byte[SIM_CARD_NUM] = {0, 0, 0, 0};

#ifdef SIM_ENHANCED_PPS_SUPPORT
static uint8 s_enhanced_pps_data[SIM_CARD_NUM][TB_SIM_PPS_ENHANCED_SPEED] = {{0xff, 0x10, 0x94, 0x7b}, {0xff, 0x10, 0x94, 0x7b}, {0xff, 0x10, 0x94, 0x7b}, {0xff, 0x10, 0x94, 0x7b}};
#endif
#elif defined (MULTI_SIM_SYS_TRI)
static BOOLEAN s_simhal_1_8v_detect_complete[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
static BOOLEAN s_simhal_atr_volt_get_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
static uint8 s_simhal_atr_volt_byte[SIM_CARD_NUM] = {0, 0, 0};

#ifdef SIM_ENHANCED_PPS_SUPPORT
static uint8 s_enhanced_pps_data[SIM_CARD_NUM][TB_SIM_PPS_ENHANCED_SPEED] = {{0xff, 0x10, 0x94, 0x7b}, {0xff, 0x10, 0x94, 0x7b}, {0xff, 0x10, 0x94, 0x7b}};
#endif

#else
static BOOLEAN s_simhal_1_8v_detect_complete[SIM_CARD_NUM] = {FALSE, FALSE};
static BOOLEAN s_simhal_atr_volt_get_flag[SIM_CARD_NUM] = {FALSE, FALSE};
static uint8 s_simhal_atr_volt_byte[SIM_CARD_NUM] = {0, 0};

#ifdef SIM_ENHANCED_PPS_SUPPORT
static uint8 s_enhanced_pps_data[SIM_CARD_NUM][TB_SIM_PPS_ENHANCED_SPEED] = {{0xff, 0x10, 0x94, 0x7b}, {0xff, 0x10, 0x94, 0x7b}};
#endif
#endif

#ifdef SIM_ENHANCED_PPS_SUPPORT
static const TB_SIM_CLOCKRATE_FACTOR_T CLOCK_RATE_CONVERSION_TABLE[12] =
{
    /*F of TA1*//*Clock Rate Factor*/
    {0x00  , SIM_FACTOR_F_372},
    {0x10  , SIM_FACTOR_F_372},
    {0x20  , SIM_FACTOR_F_558},
    {0x30  , SIM_FACTOR_F_744},
    {0x40  , SIM_FACTOR_F_1116},
    {0x50  , SIM_FACTOR_F_1488},
    {0x60  , SIM_FACTOR_F_1860},
    {0x90  , SIM_FACTOR_F_512},
    {0xa0  , SIM_FACTOR_F_768},
    {0xb0  , SIM_FACTOR_F_1024},
    {0xc0  , SIM_FACTOR_F_1536},
    {0xd0  , SIM_FACTOR_F_2048}
};

/* Table 8 in ISO/IEC 7816-3:1997 */
static const TB_SIM_BITRATE_FACTOR_T BIT_RATE_CONVERSION_TABLE[9] =
{
    /*D of TA1*//*Clock Rate Factor*/
    {0x01  , SIM_FACTOR_D_1 },
    {0x02  , SIM_FACTOR_D_2 },
    {0x03  , SIM_FACTOR_D_4 } ,
    {0x04  , SIM_FACTOR_D_8 },
    {0x05  , SIM_FACTOR_D_16 },
    {0x06  , SIM_FACTOR_D_32 },
    {0x07  , SIM_FACTOR_D_64 },
    {0x08  , SIM_FACTOR_D_12 },
    {0x09  , SIM_FACTOR_D_20 }
};

#ifdef MULTI_SIM_SYS_QUAD
static TB_SIM_CLOCKRATE_FACTOR_E sim_F[SIM_CARD_NUM] = {TB_SIM_DEFAULT_FACTOR_F, TB_SIM_DEFAULT_FACTOR_F, TB_SIM_DEFAULT_FACTOR_F, TB_SIM_DEFAULT_FACTOR_F}; //The SIM's F of TA1
static TB_SIM_BITRATE_FACTOR_E sim_D[SIM_CARD_NUM] = {TB_SIM_DEFAULT_FACTOR_D, TB_SIM_DEFAULT_FACTOR_D, TB_SIM_DEFAULT_FACTOR_D, TB_SIM_DEFAULT_FACTOR_D}; //The SIM's D of TA1
static BOOLEAN sim_dma_support[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};//SIM DMA channel enable or not
static BOOLEAN sim_is_dma_last_one_byte_timer_restarted[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#elif defined (MULTI_SIM_SYS_TRI)
static TB_SIM_CLOCKRATE_FACTOR_E sim_F[SIM_CARD_NUM] = {TB_SIM_DEFAULT_FACTOR_F, TB_SIM_DEFAULT_FACTOR_F, TB_SIM_DEFAULT_FACTOR_F}; //The SIM's F of TA1
static TB_SIM_BITRATE_FACTOR_E sim_D[SIM_CARD_NUM] = {TB_SIM_DEFAULT_FACTOR_D, TB_SIM_DEFAULT_FACTOR_D, TB_SIM_DEFAULT_FACTOR_D}; //The SIM's D of TA1
static BOOLEAN sim_dma_support[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};//SIM DMA channel enable or not
static BOOLEAN sim_is_dma_last_one_byte_timer_restarted[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#else
static TB_SIM_CLOCKRATE_FACTOR_E sim_F[SIM_CARD_NUM] = {TB_SIM_DEFAULT_FACTOR_F, TB_SIM_DEFAULT_FACTOR_F}; //The SIM's F of TA1
static TB_SIM_BITRATE_FACTOR_E sim_D[SIM_CARD_NUM] = {TB_SIM_DEFAULT_FACTOR_D, TB_SIM_DEFAULT_FACTOR_D}; //The SIM's D of TA1
static BOOLEAN sim_dma_support[SIM_CARD_NUM] = {FALSE, FALSE};//SIM DMA channel enable or not
static BOOLEAN sim_is_dma_last_one_byte_timer_restarted[SIM_CARD_NUM] = {FALSE, FALSE};
#endif

static BOOLEAN sim_no_ack_byte[2] = {FALSE, FALSE};//The command has a ack byte or not
static BOOLEAN sim_dma_tx_not_finish[2] = {FALSE, FALSE};//There still som data remain need to send to the SIM
static uint32 dma_current_rx_total_size[2] = {0, 0}; //This time, the DMA RX should receive how many bytes.
static DMA_CHNCFG_T sim_rx_dma_cfg[2];//SIM RX DMA channel config
static DMA_CHNCFG_T sim_tx_dma_cfg[2];//SIM TX DMA channel config
static uint32 s_simhal_dma_rsp_time[2] = {SIM_DMA_RX_NORMAL_TIMER, SIM_DMA_RX_NORMAL_TIMER}; //This value must be shorter than SIM_CARD_PRESENT_TIME_LIMIT
static uint32 s_simhal_dma_rx_received_bytes[2] = {0, 0};
#endif
#ifdef MULTI_SIM_SYS_QUAD
static uint32 s_sim_clk_dvd_value[SIM_CARD_NUM] = {0xba0, 0xba0, 0xba0, 0xba0};
#elif defined (MULTI_SIM_SYS_TRI)
static uint32 s_sim_clk_dvd_value[SIM_CARD_NUM] = {0xba0, 0xba0, 0xba0};
#else
static uint32 s_sim_clk_dvd_value[SIM_CARD_NUM] = {0xba0, 0xba0};
#endif
static CHIP_TYPE_E s_sim_chip_type = SC6600L;//Initialize according to the 6600L project
uint32 s_sim_ldo_stable_time = SIM_WAIT_LDO_TIMER_VALUE;//ms
static uint8 s_simhal_rx_noise_data[SIM_CARD_NUM] = {0};//To record the noise data num in a RESET

#ifdef MULTI_SIM_SYS_TRI
static uint8 s_sim_atr_interface_bytes_times[SIM_CARD_NUM] = {1, 1, 1};
static uint8 s_sim_atr_ta_with_volt_position[SIM_CARD_NUM] = {0, 0, 0};
static uint32 s_sim_atr_wi[SIM_CARD_NUM] = {10, 10, 10};
static uint32 s_simhal_wwt_value[SIM_CARD_NUM] = {1019, 1019, 1019};
#elif defined (MULTI_SIM_SYS_QUAD)
static uint8 s_sim_atr_interface_bytes_times[SIM_CARD_NUM] = {1, 1, 1, 1};
static uint8 s_sim_atr_ta_with_volt_position[SIM_CARD_NUM] = {0, 0, 0, 0};
static uint32 s_sim_atr_wi[SIM_CARD_NUM] = {10, 10, 10, 10};
static uint32 s_simhal_wwt_value[SIM_CARD_NUM] = {1019, 1019, 1019, 1019};
#else
static uint8 s_sim_atr_interface_bytes_times[SIM_CARD_NUM] = {1, 1};
static uint8 s_sim_atr_ta_with_volt_position[SIM_CARD_NUM] = {0, 0};
static uint32 s_sim_atr_wi[SIM_CARD_NUM] = {10, 10};
static uint32 s_simhal_wwt_value[SIM_CARD_NUM] = {1019, 1019};
#endif

#ifdef SIM_PLUG_IN_SUPPORT
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
static TB_SIM_INT_TYPE_E s_sim_hot_swap_latest_int;
#endif
#endif

//NOTE: We do not care wether a SPI is a real one or a simulated one(via GPIOs) here.
//In SC6600L, it has 2 real SPI(SPI0 and SPI1). usually, we use 1 real SPI and a simulated SPI in quadsim.
//for example, set SIMHAL_1ST_6302_SPI_PORT 0 or 1,this is the real one;
//and set the SIMHAL_2ND_6302_SPI_PORT 2, it will be a simulated one.
//In SC6800H, it has only 1 real SPI(SPI0). so we should simulate a SPI by using 3 GPIOs.
//If we set SIMHAL_6302_SPI_PORT 1 or 2, it will be simulated by GPIO.
//The driver software will init the SPIs (real or simulated) automatically, we do not care.
#ifdef SIM_6302_SWITCH
static BOOLEAN s_sim_6302_switch_flag = TRUE;//for quad-sim platform. Maybe compile with QUAD macro, but only trisim,not connect by 6302
static uint8 s_simhal_6302_reg_val[SIMHAL_6302_REG_MAX] = {0, 0, 0, 0};
static uint32 simhal_6302_spi_logic_id = SPI0_0_CS0;//6530 use SPI1_3_CS0,6531efm use SPI0_0_CS0
static uint8 simhal_6302_bb_sim_index = 1;//sim 0/1/2, which is connected to 6302?//In 6531efm,6302 is connected to sim1;In 6530,6302 is connected to sim2

typedef enum _SIM_6302_OUTPUT_E
{
    SIM_6302_OUTPUT_0,
    SIM_6302_OUTPUT_1,
    SIM_6302_CARD_ID_NOT_CONNECT
} SIM_6302_OUTPUT_E;
#endif

static uint16 s_sim_cmd_send_times[SIM_CARD_NUM] = {0};
uint8 s_sim_max_reset_times = 2;

#ifdef DPHONE_VER2_TEST
static SCI_TIMER_PTR s_sim_dphone_ver2_test_timer = {NULL};
#endif

static BOOLEAN s_sim_p_reset_happen_flag[SIM_CARD_NUM] = {FALSE}; 

//---------------Local Function declare--------------------
/**********************************************************************
//    Description:
//      Handle the TX Empty Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_TX_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Handle the RX Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_RX_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Handle the RX Parity Error Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_RX_ParityErr_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Handle the TX Parity Error Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_TX_ParityErr_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Handle the unresponse Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_Unresponse_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Handle the card in Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_CardIn_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Handle the card out Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_CardOut_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Handle the early ATR Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_EarlyATR_Handler(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      When the drive have receive all the response data of an operation
//    from SIM card,the drive will send the response data to manager
//    layer to process it
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendResponseUp(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      the really function taht do the  data transmit to the SIM CARD
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TransmitInInt(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      check the Received buf and sent all received data up to Task
//    when all the data from SIM Card has been received
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CheckReceiveBuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM1_DriverTimerExpired(uint32 lparam);

/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM2_DriverTimerExpired(uint32 lparam);

/**********************************************************************
//    Description:
//      Notify the SIM layer the exeption interrupt
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIMHAL_NotifyExceptionInt(
    SIM_CARD_INDEX_E card_id,
    TB_SIM_INT_TYPE_E int_type //the exeption interrupt type
);

/**********************************************************************
//    Description:
//      report the SIM card insert status to the SIM manager task
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ReportCardStatus(
    SIM_CARD_INDEX_E card_id,
    SIMDRV_INSERT_STATUS_E insert_status   //the SIM card insert status
);

/**********************************************************************
//    Description:
//      to identify a two byte sequence data can be status word pair or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN IsStatusWord(
    uint8 byte1,    //the byte1 of the data
    uint8 byte2     //the byte 2 of the data
);

/**********************************************************************
//    Description:
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_ClearVariablePowerOff(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Stop all the timers in sim hal
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_StopAllTimer(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      PowerOff the SIM card. Because of the SC6600L chip's special Power_enable, I
//      have to make a new poweroff function.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_Hardware_PowerOffSIM(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Reset the SIM card with a new working voltage when the SIM card
//    is not working well at the beignning,but there are reset times limit,
//    if the rest time exeed the times limit,the SIM card will be Rejected
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN ResetSIMCard(
    BOOLEAN is_1_8v_detect,
    SIM_CARD_INDEX_E card_id, //return value:the SIM card reset succeed or not.
    TB_SIM_RESET_TYPE_E reset_type  //the reset type
);

/**********************************************************************
//    Description:
//      When the ATR data show the SIM card need a PPS procedure before
//    actived,Use this function to send the PPS procedure data to the
//    SIM card to start the PPS procedure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendPPSRequest(
    SIM_CARD_INDEX_E card_id,
    const uint8 *pps_req, //in:the ppas request data to be send
    uint16 req_len  //the data len
);

/**********************************************************************
//    Description:
//      Set the working voltage of the sim card.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SetSIMVoltage(SIM_CARD_INDEX_E card_id, SIMDRV_WORKING_VOLT_E work_vol);

/**********************************************************************
//    Description:
//      Clear the Tx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearTxBuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Clear the Rx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearRxBuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Clear ATR data structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearATRData(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Clear the PPS info structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearPPSInfo(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Send a byte data of TX buf to the SIM card,not the all left TX
//    buf data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN SendOneByteToSIM(SIM_CARD_INDEX_E card_id);   //return value:TRUE:all the TX have been transmit to SIM
//FALSE:there still exist TX data need to be transmit to the SIM

/**********************************************************************
//    Description:
//      Active the SIM card by start clock
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIMHAL_ActiveSIMCard(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      go no the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SIM1_InitDrvAfterLdoValid(
    uint32 lparam   //the function's input parameter
);

/**********************************************************************
//    Description:
//      go no the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SIM2_InitDrvAfterLdoValid(
    uint32 lparam   //the function's input parameter
);

/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM1_ResetAfterLdoValid(
    uint32 lparam   //the function's input parameter
);

/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM2_ResetAfterLdoValid(
    uint32 lparam   //the function's input parameter
);

/**********************************************************************
//    Description:
//      After Tb/Te time, call this function to set RST high.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIM1_RST_After_Tb_or_Te(uint32 lparam);

/**********************************************************************
//    Description:
//      After Tb/Te time, call this function to set RST high.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIM2_RST_After_Tb_or_Te(uint32 lparam);

/**********************************************************************
//    Description:
//      select card one or card two
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void  SIMHAL_Select_Card(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void  SIMHAL_SetRSTHigh(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      record the SIM card's volt info in ATR data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SetATRVoltInfo(SIM_CARD_INDEX_E card_id, uint8 atr_volt_byte);

/**********************************************************************
//    Description:
//       Judge the platform is an ext-dualsim(H1/H2/I) or dualsim(6600R)
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_IsExtDualSim(void);

#ifdef SIM_6302_SWITCH
/**********************************************************************
//    Description:
//     Get the 6302 port index according to the sim card id
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_6302_OUTPUT_E SIMHAL_6302_Get_Output_Index(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//      Set the SIM card's Vsim via chip 6302.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_6302_SetVsim(SIM_CARD_INDEX_E card_id, BOOLEAN is_pwr_off, SIMDRV_WORKING_VOLT_E vol);

/**********************************************************************
//    Description:
//      Init the SPI_x to control 6302 chip.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_6302_InitSPI(void);

/**********************************************************************
//    Description:
//      Init the SPI_x to control 6302 chip.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_6302_WriteSPI(SIM_CARD_INDEX_E card_id, SIMHAL_6302_REG_E module, uint8 value);
#endif//SIM_6302_SWITCH

/**********************************************************************
//    Description:
//      To judge wether the 6302 is the first or second one according to the card_id
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static uint8 SIMHAL_GetChipSimIndex(SIM_CARD_INDEX_E card_id, BOOLEAN is_6600l_sim_ldo);

/**********************************************************************
//    Description:  Set sim info about WWT(= 960*WI*Fi/f) in millisecond
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SetWWT(SIM_CARD_INDEX_E card_id);

#ifdef SIM_PLUG_IN_SUPPORT
/**********************************************************************
//      For the sim0 plug in/out gpio int callback
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static uint32 SIMHAL_SIM_Plugin_GPIO_Callback(BOOLEAN gpio_state);

/**********************************************************************
//    Description:
//     After the sim0 plug in/out gpio stable time out, we notify upper layer.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SIM_Hotswap_Stable_TimeOut(uint32 lparam);

/*****************************************************************************/
//  Description:    send the message of the SIM Card status by diag command.
//  Global resource dependence: NONE
//  Author: Mingxuan.Zhu
//  Note:
/*****************************************************************************/
static uint32 SetSIMCardStatus(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
);
#endif

#ifdef SIM_ENHANCED_PPS_SUPPORT//for dma debug
/**********************************************************************
//    Description:
//    Get the F factor and the D factor according to the TA1 value.
//   Once we find the F and D, we will config the PPS data and set the SIM I/O baudrate.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIMHAL_SetPPS(SIM_CARD_INDEX_E card_id, uint8 TA1);

/**********************************************************************
//    Description:
//    After we have received the ACK byte, then we can TX the remain data to SIM card.
//   Accoring to the ACK byte, now we can send only 1 byte to the SIM card. The rest
//    data's send will depend on the next ACK byte.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static BOOLEAN SIM_DMA_TX_one_byte(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//    After we have received the ACK byte, then we can TX the remain data to SIM card.
//   Accoring to the ACK byte, now we send all the remain data to the SIM card at one time.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_TX_all(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//    When we receive the real data from SIM, may be there will be some NULL bytes
//   before the real data. So we have to count and remove them.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static uint8 SIM_DMA_count_and_discard_null_byte(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//    This is the function to get and check the procedure byte before the real SIM data.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_procedure_byte(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//    This is the function check the rx buf.
//    If the data has NULL bytes, re-config the DMA RX and to get the rest data from sim card.
//    If all the data have been received, check the RX buf, then send to the SIM layer.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_checkbuf(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//    This function is called when the timer time out.
//  1, Sometimes we send a command to SIM, the excepted RX len is 0x10, for example. But
//      SIM think the command is error, so it send us only 2 status words, 67 00, for example.
//      Now the DMA RX interrupt will never come because we want 0x10 bytes data. So we
//      set a timer to avoid this happen.
//  2, Though we didn't get the right data from SIM card, we should still notify the upper layer.
//      Or the upper layer will regard the SIM driver is dead.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_timeout(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//    This function used to get the last 1 byte. Sometimes, the last 1 byte of data from
//         SIM card cannot get directly in SIM DMA RX interrupt. So I have to start a timer to
//      get this byte.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_lastonebyte(SIM_CARD_INDEX_E card_id);

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM1
***********************************************************************/
LOCAL void SIM1_DMA_TX_callback(uint32 num);

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM2
***********************************************************************/
LOCAL void SIM2_DMA_TX_callback(uint32 num);

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM RX channel when DMA SIM RX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM1
***********************************************************************/
LOCAL void SIM1_DMA_RX_callback(uint32 num);

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM2 RX channel when DMA SIM2 RX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM1
***********************************************************************/
LOCAL void SIM2_DMA_RX_callback(uint32 num);

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//    This is the function used to check un-expected SW in time only when 3G usim STATUS.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIMHAL_CheckStatusReponse(uint32 card_id);
#endif

#endif//SIM_ENHANCED_PPS_SUPPORT

//--------------------------functions define-------------------------------//
#ifdef SIM_ENHANCED_PPS_SUPPORT//2007.2.27

/**********************************************************************
//    Description:
//    Get the F factor and the D factor according to the TA1 value.
//   Once we find the F and D, we will config the PPS data and set the SIM I/O baudrate.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIMHAL_SetPPS(SIM_CARD_INDEX_E card_id, uint8 TA1)
{
    uint8 count = 0;
    BOOLEAN crFound = FALSE;
    BOOLEAN brFound = FALSE;
    TB_SIM_CLOCKRATE_FACTOR_E crFactor = TB_SIM_DEFAULT_FACTOR_F;
    TB_SIM_BITRATE_FACTOR_E brFactor = TB_SIM_DEFAULT_FACTOR_D;
    uint8 ppss = 0xff;
    uint8 pps0 = 0x10;
    uint8 pps1 = TA1;
    uint8 pck;
    SIM_CARD_INDEX_E card_index = card_id;

    if(s_sim_is_ext_dualsim)
    {
        card_index = simhal_current_sel_card;
    }

    if(0x97 == pps1)
    {
        //special case. 0x97 will lead a very high speed baudrate.
        //So we will work as 0x96, just like the other phones.
        pps1 = 0x96;
    }
    else
    {
        pps1 = TA1;
    }

    while((count < 12) && (crFound == FALSE))
    {
        if((pps1 & 0xf0) == CLOCK_RATE_CONVERSION_TABLE[count].f_ta1)
        {
            crFound = TRUE;
            crFactor =  CLOCK_RATE_CONVERSION_TABLE[count].crFactor;
        }

        count++;
    }

    count = 0;

    while((count < 9) && (brFound == FALSE))
    {
        if((pps1 & 0xf) == BIT_RATE_CONVERSION_TABLE[count].d_ta1)
        {
            brFound = TRUE;
            brFactor =  BIT_RATE_CONVERSION_TABLE[count].brFactor;
        }

        count++;
    }

    //The TA1 can be find from the 2 tables(7816-3). So we can config the PPS data
    //and set the SIM-ME I/O baudrate.
    s_pps_info[card_index].is_current = TRUE;

    if(crFound && brFound)
    {
        pck = ppss ^ pps0 ^ pps1;
        sim_F[card_index] = crFactor;
        sim_D[card_index] = brFactor;
        s_enhanced_pps_data[card_index][0] = ppss;
        s_enhanced_pps_data[card_index][1] = pps0;
        s_enhanced_pps_data[card_index][2] = pps1;
        s_enhanced_pps_data[card_index][3] = pck;
        s_pps_info[card_index].pps_data = s_enhanced_pps_data[card_index];
        s_pps_info[card_index].pps_type = TB_SIM_PPS_ENHANCED_SPEED;
        //SCI_TRACE_LOW("SIMHAL%d::crFound && brFound all true!!!",card_id);
    }
    else
    {
        s_pps_info[card_index].pps_data = s_default_pps_data;
        s_pps_info[card_index].pps_type = TB_SIM_PPS_DEFAULT_SPEED;
        sim_F[card_index] = TB_SIM_DEFAULT_FACTOR_F;
        sim_D[card_index] = TB_SIM_DEFAULT_FACTOR_D;
    }
}

/**********************************************************************
//    Description:
//  Init the DMA config when SIM init.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIMHAL_DMA_Init(SIM_CARD_INDEX_E card_id)
{
    //SCI_TRACE_LOW:"SIMHAL%d: init DMA"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_1008_112_2_17_22_59_0_4433, (uint8 *)"d", card_id);

    //SIM DMA TX parameter
    SCI_MEMSET(&sim_tx_dma_cfg[card_id], 0, sizeof(DMA_CHNCFG_T));
    sim_tx_dma_cfg[card_id].type = DMA_CHN_TYPE_HARD;
    sim_tx_dma_cfg[card_id].priority       = DMA_CHN_PRIORITY_0;
    sim_tx_dma_cfg[card_id].workmode       = DMA_CHN_WORKMODE_NORMAL;
    sim_tx_dma_cfg[card_id].src_datawidth  = DMA_DATAWIDTH_BYTE;
    sim_tx_dma_cfg[card_id].dest_datawidth = DMA_DATAWIDTH_BYTE;
    sim_tx_dma_cfg[card_id].src_address = (uint32)s_sim_tx_buf[card_id].buf;
    sim_tx_dma_cfg[card_id].requestmode    = DMA_CHN_REQMODE_NORMAL;
    sim_tx_dma_cfg[card_id].block_length   = 5;
    sim_tx_dma_cfg[card_id].total_length   = 5;
    sim_tx_dma_cfg[card_id].src_step       = 1;  // src increament
    sim_tx_dma_cfg[card_id].dest_step      = 0;  // des nochange

	if (SIM_CARD_ONE == card_id)
	{
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
        sim_tx_dma_cfg[SIM_CARD_ONE].dest_address = (uint32)((volatile unsigned char*)0x89000000); 
#else
        sim_tx_dma_cfg[SIM_CARD_ONE].dest_address = (uint32)((volatile unsigned char *)0x85000000);
#endif

    }
    else if(SIM_CARD_TWO == card_id)
    {
        SCI_ASSERT(0);/*assert verified */
    }

    //SIM DMA RX parameter
    SCI_MEMSET(&sim_rx_dma_cfg[card_id], 0, sizeof(DMA_CHNCFG_T));
    sim_rx_dma_cfg[card_id].type           = DMA_CHN_TYPE_HARD;
    sim_rx_dma_cfg[card_id].priority       = DMA_CHN_PRIORITY_0;
    sim_rx_dma_cfg[card_id].workmode       = DMA_CHN_WORKMODE_NORMAL;
    sim_rx_dma_cfg[card_id].src_datawidth  = DMA_DATAWIDTH_BYTE;
    sim_rx_dma_cfg[card_id].dest_datawidth = DMA_DATAWIDTH_BYTE;
    sim_rx_dma_cfg[card_id].dest_address   = (uint32)s_sim_rx_buf[card_id].buf;
    sim_rx_dma_cfg[card_id].requestmode    = DMA_CHN_REQMODE_NORMAL;
    sim_rx_dma_cfg[card_id].block_length   = 1;
    sim_rx_dma_cfg[card_id].total_length   = 1;
    sim_rx_dma_cfg[card_id].src_step       = 0;  // src nochange
    sim_rx_dma_cfg[card_id].dest_step      = 1;  // des increament

	if (SIM_CARD_ONE == card_id)
	{
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
        sim_rx_dma_cfg[SIM_CARD_ONE].src_address = (uint32)((volatile unsigned char*)0x89000004); 
#else
        sim_rx_dma_cfg[SIM_CARD_ONE].src_address = (uint32)((volatile unsigned char *)0x85000004);
#endif
    }
    else if(SIM_CARD_TWO == card_id)
    {
        SCI_ASSERT(0);/*assert verified */
    }

    if(0 == card_id)
    {
        SIMDRV_DMA_Init(card_id, SIM1_DMA_TX_callback, SIM1_DMA_RX_callback, &sim_tx_dma_cfg[card_id], &sim_rx_dma_cfg[card_id]);
    }
    else
    {
        SIMDRV_DMA_Init(card_id, SIM2_DMA_TX_callback, SIM2_DMA_RX_callback, &sim_tx_dma_cfg[card_id], &sim_rx_dma_cfg[card_id]);
    }
}

/**********************************************************************
//    Description:
//    After we have received the ACK byte, then we can TX the remain data to SIM card.
//   Accoring to the ACK byte, now we can send only 1 byte to the SIM card. The rest
//    data's send will depend on the next ACK byte.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static BOOLEAN SIM_DMA_TX_one_byte(SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;

#ifdef SIM_DMA_DEBUG_TRACE_ON
    SCI_TRACE_LOW("SIMHAL%d:SIM_DMA_TX_one_byte",card_id);
#endif

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    // 1, the 5 bytes instrunction have been send out.
    // 2, the 5 bytes instrunction have been send out, and we have send 1 bye data to SIM for
    //several times(or 1 times).
    //Now the SIM card still ask ME send the rest data in 1-byte-mode.
    //SCI_TRACE_LOW("SIMHAL::DMA TX 1 BYTE tx_data_len %d cur_pos %d", s_sim_tx_buf[card_index].tx_data_len, s_sim_tx_buf[card_index].cur_pos);

    if(s_sim_tx_buf[card_index].tx_data_len > s_sim_tx_buf[card_index].cur_pos)
    {
        SIMDRV_DMA_TX_Disable((uint8)card_index);
        sim_tx_dma_cfg[card_index].src_address = (uint32)(&(s_sim_tx_buf[card_index].buf[s_sim_tx_buf[card_index].cur_pos]));
        sim_tx_dma_cfg[card_index].total_length = 1;
        sim_tx_dma_cfg[card_index].block_length = 1;
        SIMDRV_DMA_TX_Cfg((uint8)card_index, &sim_tx_dma_cfg[card_index]);
        SIMDRV_DMA_TX_Enable((uint8)card_index);

        s_sim_tx_buf[card_index].cur_pos++;

        //all the data of the tx buf has been sent to the SIM card.
        if(s_sim_tx_buf[card_index].cur_pos >= s_sim_tx_buf[card_index].tx_data_len)
        {
            ClearTxBuf(card_id);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        ClearTxBuf(card_id);
        return TRUE;
    }
}

/**********************************************************************
//    Description:
//    After we have received the ACK byte, then we can TX the remain data to SIM card.
//   Accoring to the ACK byte, now we send all the remain data to the SIM card at one time.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_TX_all(SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;

#ifdef SIM_DMA_DEBUG_TRACE_ON
    SCI_TRACE_LOW("SIMHAL%d:SIM_DMA_TX_all",card_id);
#endif

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    // 1, the 5 bytes instrunction have been send out.(almostly)
    // 2, the 5 bytes instrunction have been send out, and we have send 1 bye data to SIM for
    //several times(or 1 times).
    //Now the SIM card still ask ME send all the rest data totally.
    //SCI_TRACE_LOW("SIMHAL::DMA TX ALL tx_data_len %d cur_pos %d", s_sim_tx_buf[card_index].tx_data_len, s_sim_tx_buf[card_index].cur_pos);

    if(s_sim_tx_buf[card_index].tx_data_len > s_sim_tx_buf[card_index].cur_pos)
    {
        SIMDRV_DMA_TX_Disable((uint8)card_index);
        sim_tx_dma_cfg[card_index].src_address = (uint32)(&(s_sim_tx_buf[card_index].buf[s_sim_tx_buf[card_index].cur_pos]));

        //the rest data is more than 16 bytes, we cannot send them all at once. (TX FIFO is 16 bytes only)
        if((s_sim_tx_buf[card_index].tx_data_len - s_sim_tx_buf[card_index].cur_pos) > SIM_DMA_TX_MAX_LEN)
        {
            sim_tx_dma_cfg[card_index].total_length = SIM_DMA_TX_MAX_LEN;
            sim_tx_dma_cfg[card_index].block_length = SIM_DMA_TX_MAX_LEN;
            s_sim_tx_buf[card_index].cur_pos += SIM_DMA_TX_MAX_LEN;
            sim_dma_tx_not_finish[card_index] = TRUE;//still have data to be sent
        }
        //the rest data is in 16 bytes, we can send them all at once.
        else
        {
            sim_tx_dma_cfg[card_index].total_length = s_sim_tx_buf[card_index].tx_data_len - s_sim_tx_buf[card_index].cur_pos;
            sim_tx_dma_cfg[card_index].block_length = s_sim_tx_buf[card_index].tx_data_len - s_sim_tx_buf[card_index].cur_pos;
            sim_dma_tx_not_finish[card_index] = FALSE;//all the data have been send out
            ClearTxBuf(card_id);//clear the tx buf
        }

        SIMDRV_DMA_TX_Cfg((uint8)card_index, &sim_tx_dma_cfg[card_index]);
        SIMDRV_DMA_TX_Enable((uint8)card_index);
    }
    else
    {
        ClearTxBuf(card_id);//clear the tx buf
    }

    return;
}

/**********************************************************************
//    Description:
//    When we receive the real data from SIM, may be there will be some NULL bytes
//   before the real data. So we have to count and remove them.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static uint8 SIM_DMA_count_and_discard_null_byte(SIM_CARD_INDEX_E card_id)
{
    uint16 i, data_len = 0;
    uint16 len_null = 0;
    SIM_CARD_INDEX_E card_index = card_id;

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);
    s_sim_rx_buf[card_index].check_pos = s_sim_rx_buf[card_index].cur_pos;

    if(TB_SIM_RESPONSE_NULL_BYTE == s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos])
    {
        //count the numbers of NULL bytes
        s_sim_rx_buf[card_index].check_pos++;
        len_null++;

        for(i = 1; i < dma_current_rx_total_size[card_index]; i++)
        {
            if(TB_SIM_RESPONSE_NULL_BYTE == s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos])
            {
                s_sim_rx_buf[card_index].check_pos++;
                len_null++;
            }
            else
            {
                break;
            }
        }

        data_len = dma_current_rx_total_size[card_index] - len_null;

        //discard the NULL bytes in the rx buf.
        for(i = s_sim_rx_buf[card_index].cur_pos; i < data_len; i++)
        {
            s_sim_rx_buf[card_index].buf[i] = s_sim_rx_buf[card_index].buf[i+len_null];
            s_sim_rx_buf[card_index].buf[i+len_null] = 0;//clear the contaminated rx buf
        }

        s_sim_rx_buf[card_index].cur_pos += data_len;

        if(data_len < len_null)
        {
            //TWO cases:
            // 1, data_len >= len_null. For example, 60 60 60 data1 data2 data3 data4 data5...
            //Now rx buf is data1 data2 data3 data4 data5 00 00 00... it's OK.
            // 2, data_len < len_null. For example, 60 60 60 60 60 data1 data2...
            //Now rx buf is data1 data2 60 60 60 00 00...
            //I think we'd better clear the remained 0x60.
            for(i = 0; i < (len_null - data_len); i++)
            {
                s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos + i] = 0;
            }
        }
    }

    return len_null;
}

/**********************************************************************
//    Description:
//    This is the function to get and check the procedure byte before the real SIM data.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_procedure_byte(SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;
    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

#ifdef SIM_DMA_DEBUG_TRACE_ON
    SCI_TRACE_LOW("SIMHAL%d:SIM_DMA_RX_procedure_byte",card_id);
#endif

    //Now we have send 5 bytes instruction out, and SIM response a procedure byte as ACK.
    // this ACK byte will be stored in s_sim_rx_buf[x].buf[0].

    //the procedure as below:
    // 1, The ACK byte has been received, set s_have_ack_byte TRUE, and begin to send the rest data.
    // 2, The NULL bytes have been received, we should discard them and continue to wait.
    //      So we should recover the SIM RX DMA settings, and quit.
    // 3, Error, the 5 bytes instruction is not OK. the sim card response status word direclty.(For example, 94 08)
    //ACK byte
    //we can send all the rest data to SIM at once
    if((s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS] == s_sim_rx_buf[card_index].buf[0])
        || (s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS] + 1 == s_sim_rx_buf[card_index].buf[0]))
    {
        s_have_ack_byte[card_index] = TRUE;
        s_null_byte_count[card_index] = 0;
        SIM_DMA_TX_all(card_id);
    }
    //we can send only 1 byte of the rest data to SIM.
    else if(((uint8)(~s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS]) == s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos])
            || ((uint8)((~s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS] + 1)) == s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos]))
    {
        s_null_byte_count[card_index] = 0;
        s_have_ack_byte[card_index] = SIM_DMA_TX_one_byte(card_id);
    }
    //ACK byte
    //NULL byte
    else if(TB_SIM_RESPONSE_NULL_BYTE == s_sim_rx_buf[card_index].buf[0])
    {
        s_null_byte_count[card_index]++;

        if(s_null_byte_count[card_index] == SIM_MAX_NULL_BYTE_COUNT)
        {
            s_null_byte_count[card_index] = 0;
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_NEED_WAITTING);

            if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
            {
                SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
            }

            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_ONE]);
            }
            else if(SIM_CARD_TWO == card_index)
            {
                SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_TWO]);
            }

            SCI_ActiveTimer(s_sim_driver_timer[card_index]);
        }

        //Before acy byte, we got hte NULL byte(s). We will discard the NULL byte(s), and re-configure
        // the SIM RX DMA registers (because the NULL byte(s) have contaminated the "totalsize" and "des_address").
        // ,and then enable the SIM DMA RX. Quit and wait the next SIM RX DMA interrupt.
        sim_rx_dma_cfg[card_index].total_length = dma_current_rx_total_size[card_index];
        sim_rx_dma_cfg[card_index].dest_address = (uint32)s_sim_rx_buf[card_index].buf;
        s_simhal_dma_rx_received_bytes[card_index] = 0;//clear for a new DMA transfer
        SIMDRV_DMA_RX_Cfg((uint8)card_index, &sim_rx_dma_cfg[card_index]);
        SIMDRV_DMA_RX_Enable((uint8)card_index);
    }
    //other case
    else
    {
        //SCI_TRACE_LOW:"SIMHAL%d::Wrong procedure byte 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_1385_112_2_17_22_59_0_4434, (uint8 *)"dd", card_index, s_sim_rx_buf[card_index].buf[0]);

        //SCI_ASSERT(FALSE);//Debug only
        if(sim_send_data_present[card_index])
        {
            //Error, the 5 bytes instruction is not OK. The sim card response status word direclty.(For example, 94 08)
            //Now the current byte is SW1, we will to get SW2.
            s_sim_rx_buf[card_index].cur_pos++;

            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_dma_last_byte_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_DMA_RX_LAST_BYTE_AGAIN_TIMER); //wuding 2013-07-31
            }
            else if(SIM_CARD_TWO == card_index)
            {
                SCI_ChangeTimer(s_sim_dma_last_byte_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_DMA_RX_LAST_BYTE_AGAIN_TIMER); //wuding 2013-07-31
            }

            SCI_ActiveTimer(s_sim_dma_last_byte_timer[card_index]);
        }
        else
        {
            //other case
            SIM_DMA_RX_timeout(card_id);
        }
    }
}

/**********************************************************************
//    Description:
//    This is the function check the rx buf.
//    If the data has NULL bytes, re-config the DMA RX and to get the rest data from sim card.
//    If all the data have been received, check the RX buf, then send to the SIM layer.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_checkbuf(SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;
    uint8 len_of_null = 0;

#ifdef SIM_DMA_DEBUG_TRACE_ON
    SCI_TRACE_LOW("SIMHAL%d:SIM_DMA_RX_checkbuf",card_id);
#endif
	
    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    SIMDRV_DMA_TX_Disable((uint8)card_index);//make sure Tx disabled
    SIMDRV_DisableTx((uint8)card_index);//wuding add according to RF Lab Test.

    if(SCI_IsTimerActive(s_sim_dma_last_byte_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_dma_last_byte_timer[card_index]);
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(SCI_IsTimerActive(s_sim_3g_status_check_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_3g_status_check_timer[card_index]);
    }

#endif

    //the data maybe like this:
    //NULL NULL NULL NULL data1 data2 ... data(rx_expected_len-2-4), and
    //data(rx_expected_len-2-3). data(rx_expected_len-2-2).data(rx_expected_len-2-1)
    //data(rx_expected_len-2). The last 4 bytes need to be gotten.
    //One of my SIM will response "0x60 0x9F 0x0C" when auth.
    len_of_null = SIM_DMA_count_and_discard_null_byte(card_id);

    if(len_of_null > 0)
    {
        //If too many null bytes, we need to restart up SIM layer's 4S timer and our s_sim_driver_timer.
        s_null_byte_count[card_index] += len_of_null;

        if(s_null_byte_count[card_index] >= SIM_MAX_NULL_BYTE_COUNT)
        {
            s_null_byte_count[card_index] = 0;
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_NEED_WAITTING);

            if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
            {
                SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
            }

            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_ONE]);
            }
            else if(SIM_CARD_TWO == card_index)
            {
                SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_TWO]);
            }

            SCI_ActiveTimer(s_sim_driver_timer[card_index]);
        }

        if(len_of_null >= dma_current_rx_total_size[card_index]) //all the received are NULL bytes.
        {
            //re-receive all the bytes.
            sim_rx_dma_cfg[card_index].total_length = dma_current_rx_total_size[card_index];
        }
        else
        {
            //re-receive all the un-received bytes.
            sim_rx_dma_cfg[card_index].total_length = len_of_null;
        }

        dma_current_rx_total_size[card_index] = sim_rx_dma_cfg[card_index].total_length;
        sim_rx_dma_cfg[card_index].dest_address = (uint32)(&(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos]));

        s_simhal_dma_rx_received_bytes[card_index] = 0;//clear for a new DMA transfer
        SIMDRV_DMA_RX_Cfg((uint8)card_index, &sim_rx_dma_cfg[card_index]);

        if(dma_current_rx_total_size[card_index] > 1) //there are more than 1 byte data to be received.
        {
            //More than 1 byte, re-enable the SIM RX DMA interrupt.
            SIMDRV_DMA_RX_Enable((uint8)card_index);
            return;
        }
        else//only 1 byte data to be received
        {
            sim_is_dma_last_one_byte_timer_restarted[card_id] = FALSE;

            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_dma_last_byte_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_DMA_RX_LAST_BYTE_TIMER); // 2 ms is enough, I guess.
            }
            else if(SIM_CARD_TWO == card_index)
            {
                SCI_ChangeTimer(s_sim_dma_last_byte_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_DMA_RX_LAST_BYTE_TIMER); // 2 ms is enough, I guess.
            }

            SCI_ActiveTimer(s_sim_dma_last_byte_timer[card_index]);
            return;
        }
    }

    // OK, now we get the right data, continue.
    // 1,After the Procedure byte, the real sim data is coming.
    // Now in the rx buf, the data is arranged as below:
    // data1 data2 .....data(rx_expected_len-2) status1 status2

    // 2,The procedure byte come with the real sim data.
    // Now in the rx buf, the data is arranged as below:
    // ack data1 data2 .....data(rx_expected_len-2+1) status1 status2
    // So we need to re-arrange the rx buf (remove the ACK byte).
    // But the re-arrange will waste too many time in a DMA INT, so we will
    //notify upper layer with the data pointer to "data1" directly in SendResponseUp.

    // Now all the data MUST be arranged as below:
    // data1 data2 .....data(rx_expected_len-2) status1 status2
    // Send to the SIM layer now.
    s_sim_rx_buf[card_index].cur_pos = s_sim_rx_buf[card_index].rx_expected_len;
    s_sim_rx_buf[card_index].is_locked = TRUE;//lock the rx buffer, then send it to SIM layer
    SendResponseUp(card_id);//Send the data to SIM layer
}

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM1
***********************************************************************/
LOCAL void SIM1_DMA_TX_callback(uint32 num)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_ONE;
    SIM_CARD_INDEX_E card_index = SIM_CARD_ONE;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);
    }
	
#ifdef SIM_DMA_DEBUG_TRACE_ON
	SCI_TRACE_LOW("SIMHAL%d:SIM1_DMA_TX_callback",card_id);
#endif

    //Stop the SIM DMA TX
    SIMDRV_DMA_TX_Disable(card_index);

    //SCI_TRACE_LOW("SIM1 TX callback %d, %d %d %d",card_id,sim_no_ack_byte[0],sim_response_data_present[0],s_have_ack_byte[0]);

    // No ACK byte, the SW1/SW2 will response directly.
    //In this case, all the data have been send out. "all the data" means only 5 bytes instruction.
    if(sim_no_ack_byte[SIM_CARD_ONE])
    {
        sim_rx_dma_cfg[SIM_CARD_ONE].total_length = s_sim_rx_buf[SIM_CARD_ONE].rx_expected_len;
    }
    //There has an ACK byte
    else if(!sim_response_data_present[SIM_CARD_ONE])
    {
        //The ACK byte has not been received. So we shall get the ack byte firstly.
        // 1, the 5 bytes instruction has been sent out. This is the first SIM DMA TX interrupt.
        // So we set the total_size as 1 to get the ack byte.
        // 2, We have got the ACK byte already. But the SIM ask ME send data 1-byte by 1-byte.
        // s_have_ack_byte will keep FALSE until all the data sent out.
        if(!s_have_ack_byte[SIM_CARD_ONE])
        {
            sim_rx_dma_cfg[SIM_CARD_ONE].total_length = 1;
        }
        //The ACK byte has been received.
        else
        {
            //Check: Has all the data been sent out? If not, continue to send data.
            //This is very special. Because I find when SIM DMA TX, the data cannot be TX all at once.
            //The MAX len is 16 bytes when DMA TX.
            if((s_sim_tx_buf[SIM_CARD_ONE].tx_data_len > (uint16)(SIM_DMA_TX_MAX_LEN + TB_SIM_APDU_CHEADER_NUM))
                && (s_sim_tx_buf[SIM_CARD_ONE].tx_data_len > s_sim_tx_buf[SIM_CARD_ONE].cur_pos)
                && (sim_dma_tx_not_finish[SIM_CARD_ONE]))
            {
                //If 6600R, the card_id must be 0; but if 6600L, maybe 0 or 1.
                SIM_DMA_TX_all(card_id);
                return;
            }
            //All the data have been sent out, now we get ready to receive SIM response.
            else
            {
                sim_rx_dma_cfg[SIM_CARD_ONE].total_length = s_sim_rx_buf[SIM_CARD_ONE].rx_expected_len;
            }
        }
    }
    //There is no alone ACK byte, the ACK byte will come with response-data.
    // Just like belows:ack data1 data2 ... datan status1 status2.
    // this will make "the num of received data" = rx_expected_len + 1.
    // In this case, the 5 byte instruction must have been sent out all at once.
    else
    {
        sim_rx_dma_cfg[SIM_CARD_ONE].total_length = s_sim_rx_buf[SIM_CARD_ONE].rx_expected_len + 1;
    }

    sim_dma_tx_not_finish[SIM_CARD_ONE] = FALSE;
    //Disable the SIM DMA RX
    SIMDRV_DMA_RX_Disable(card_index);
    dma_current_rx_total_size[SIM_CARD_ONE] = sim_rx_dma_cfg[SIM_CARD_ONE].total_length;
    sim_rx_dma_cfg[SIM_CARD_ONE].dest_address = (uint32)(&(s_sim_rx_buf[SIM_CARD_ONE].buf[s_sim_rx_buf[SIM_CARD_ONE].cur_pos]));
    s_simhal_dma_rx_received_bytes[SIM_CARD_ONE] = 0;//clear for a new DMA transfer
    SIMDRV_DMA_RX_Cfg(card_index, &sim_rx_dma_cfg[SIM_CARD_ONE]);

    if(SCI_IsTimerActive(s_sim_driver_timer[SIM_CARD_ONE]))
    {
        SCI_DeactiveTimer(s_sim_driver_timer[SIM_CARD_ONE]);
    }
	
#ifdef SIM_DMA_DEBUG_TRACE_ON
	SCI_TRACE_LOW("SIMHAL%d:active dma rsp timer %d ms",card_id,s_simhal_dma_rsp_time[SIM_CARD_ONE]);
#endif
	
    SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_ONE]);
    SCI_ActiveTimer(s_sim_driver_timer[SIM_CARD_ONE]);

    //Enable the SIM DMA RX
    SIMDRV_DMA_RX_Enable(card_index);
}

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM2
***********************************************************************/
LOCAL void SIM2_DMA_TX_callback(uint32 num)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_TWO;
    SIM_CARD_INDEX_E card_index = SIM_CARD_TWO;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            SCI_ASSERT(0);/*assert verified */
        }
    }

    //Stop the SIM DMA TX
    SIMDRV_DMA_TX_Disable(card_index);

    //SCI_TRACE_LOW("SIM2 TX callback %d, %d %d %d",card_id,sim_no_ack_byte[1],sim_response_data_present[1],s_have_ack_byte[1]);

    // No ACK byte, the SW1/SW2 will response directly.
    //In this case, all the data have been send out. "all the data" means only 5 bytes instruction.
    if(sim_no_ack_byte[SIM_CARD_TWO])
    {
        sim_rx_dma_cfg[SIM_CARD_TWO].total_length = s_sim_rx_buf[SIM_CARD_TWO].rx_expected_len;
    }
    //There has an ACK byte
    else if(!sim_response_data_present[SIM_CARD_TWO])
    {
        //The ACK byte has not been received. So we shall get the ack byte firstly.
        // 1, the 5 bytes instruction has been sent out. This is the first SIM DMA TX interrupt.
        // So we set the total_size as 1 to get the ack byte.
        // 2, We have got the ACK byte already. But the SIM ask ME send data 1-byte by 1-byte.
        // s_have_ack_byte will keep FALSE until all the data sent out.
        if(!s_have_ack_byte[SIM_CARD_TWO])
        {
            sim_rx_dma_cfg[SIM_CARD_TWO].total_length = 1;
        }
        //The ACK byte has been received.
        else
        {
            //Check: Has all the data been sent out? If not, continue to send data.
            //This is very special. Because I find when SIM DMA TX, the data cannot be TX all at once.
            //The MAX len is 16 bytes when DMA TX.
            if((s_sim_tx_buf[SIM_CARD_TWO].tx_data_len > (uint16)(SIM_DMA_TX_MAX_LEN + TB_SIM_APDU_CHEADER_NUM))
                && (s_sim_tx_buf[SIM_CARD_TWO].tx_data_len > s_sim_tx_buf[SIM_CARD_TWO].cur_pos)
                && (sim_dma_tx_not_finish[SIM_CARD_TWO]))
            {
                SIM_DMA_TX_all(SIM_CARD_TWO);
                return;
            }
            //All the data have been sent out, now we get ready to receive SIM response.
            else
            {
                sim_rx_dma_cfg[SIM_CARD_TWO].total_length = s_sim_rx_buf[SIM_CARD_TWO].rx_expected_len;
            }
        }
    }
    //There is no alone ACK byte, the ACK byte will come with response-data.
    // Just like belows:ack data1 data2 ... datan status1 status2.
    // this will make "the num of received data" = rx_expected_len + 1.
    // In this case, the 5 byte instruction must have been sent out all at once.
    else
    {
        sim_rx_dma_cfg[SIM_CARD_TWO].total_length = s_sim_rx_buf[SIM_CARD_TWO].rx_expected_len + 1;
    }

    sim_dma_tx_not_finish[SIM_CARD_TWO] = FALSE;
    //Disable the SIM DMA RX
    SIMDRV_DMA_RX_Disable(card_index);
    dma_current_rx_total_size[SIM_CARD_TWO] = sim_rx_dma_cfg[SIM_CARD_TWO].total_length;
    sim_rx_dma_cfg[SIM_CARD_TWO].dest_address = (uint32)(&(s_sim_rx_buf[SIM_CARD_TWO].buf[s_sim_rx_buf[SIM_CARD_TWO].cur_pos]));
    s_simhal_dma_rx_received_bytes[SIM_CARD_TWO] = 0;//clear for a new DMA transfer
    SIMDRV_DMA_RX_Cfg(card_index, &sim_rx_dma_cfg[SIM_CARD_TWO]);

    if(SCI_IsTimerActive(s_sim_driver_timer[SIM_CARD_TWO]))
    {
        SCI_DeactiveTimer(s_sim_driver_timer[SIM_CARD_TWO]);
    }

    SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_TWO]);
    SCI_ActiveTimer(s_sim_driver_timer[SIM_CARD_TWO]);

    //Enable the SIM DMA RX
    SIMDRV_DMA_RX_Enable(card_index);
}

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM RX channel when DMA SIM RX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM1
***********************************************************************/
LOCAL void SIM1_DMA_RX_callback(uint32 num)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_ONE;
    SIM_CARD_INDEX_E card_index = SIM_CARD_ONE;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);
    }
	
#ifdef SIM_DMA_DEBUG_TRACE_ON
    SCI_TRACE_LOW("SIMHAL%d:SIM1_DMA_RX_callback",card_id);
#endif
	

    //Stop the DMA SIM RX
    SIMDRV_DMA_RX_Disable(card_index);

    //SCI_TRACE_LOW("SIM1 RX callback %d, %d %d %d",card_id,sim_no_ack_byte[0],sim_response_data_present[0],s_have_ack_byte[0]);

    if(SCI_IsTimerActive(s_sim_driver_timer[SIM_CARD_ONE]))
    {
        SCI_DeactiveTimer(s_sim_driver_timer[SIM_CARD_ONE]);
    }

    //Check the receive buffer.
    if(!s_sim_rx_buf[SIM_CARD_ONE].is_locked)
    {
        //If there has the ACK byte, and the ACK byte will come alone.
        //and we didn't get it, then receive and check it.
        if((!sim_no_ack_byte[SIM_CARD_ONE]) && (!sim_response_data_present[SIM_CARD_ONE]) && (!s_have_ack_byte[SIM_CARD_ONE]))
        {
            SIM_DMA_RX_procedure_byte(card_id);
            return;
        }

        //First time, to get the Procedure byte.

        // 1,There is no ACK byte, the real sim data come directly (INVALIDATE,REHIBILITAE,SLEEP).
        // 2,There is no ACK byte alone, it come with the real sim data.
        // 3,We have received the ACK byte, the real sim data now is coming.
        if(sim_no_ack_byte[SIM_CARD_ONE] || sim_response_data_present[SIM_CARD_ONE] || s_have_ack_byte[SIM_CARD_ONE])
        {
            SIM_DMA_RX_checkbuf(card_id);
        }

        //After the Procedure byte, the real sim data is coming. ywd 2007.2.27
    }
    else
    {
        //The buffer is locked, assert.
        SCI_ASSERT(FALSE);/*assert verified */
    }
}

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM RX channel when DMA SIM RX interrupt happen
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:SIM2
***********************************************************************/
LOCAL void SIM2_DMA_RX_callback(uint32 num)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_TWO;
    SIM_CARD_INDEX_E card_index = SIM_CARD_TWO;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            SCI_ASSERT(0);/*assert verified */
        }
    }

    //Stop the DMA SIM RX
    SIMDRV_DMA_RX_Disable(card_index);

    //SCI_TRACE_LOW("SIM2 RX callback %d, %d %d %d",card_id,sim_no_ack_byte[1],sim_response_data_present[1],s_have_ack_byte[1]);

    if(SCI_IsTimerActive(s_sim_driver_timer[SIM_CARD_TWO]))
    {
        SCI_DeactiveTimer(s_sim_driver_timer[SIM_CARD_TWO]);
    }

    //Check the receive buffer.
    if(!s_sim_rx_buf[SIM_CARD_TWO].is_locked)
    {
        //If there has the ACK byte, and the ACK byte will come alone.
        //and we didn't get it, then receive and check it.
        if((!sim_no_ack_byte[SIM_CARD_TWO]) && (!sim_response_data_present[SIM_CARD_TWO]) && (!s_have_ack_byte[SIM_CARD_TWO]))
        {
            SIM_DMA_RX_procedure_byte(card_id);
            return;
        }

        //First time, to get the Procedure byte.

        // 1,There is no ACK byte, the real sim data come directly (INVALIDATE,REHIBILITAE,SLEEP).
        // 2,There is no ACK byte alone, it come with the real sim data.
        // 3,We have received the ACK byte, the real sim data now is coming.
        if(sim_no_ack_byte[SIM_CARD_TWO] || sim_response_data_present[SIM_CARD_TWO] || s_have_ack_byte[SIM_CARD_TWO])
        {
            SIM_DMA_RX_checkbuf(card_id);
        }

        //After the Procedure byte, the real sim data is coming. ywd 2007.2.27
    }
    else
    {
        //The buffer is locked, assert.
        SCI_ASSERT(FALSE);/*assert verified */
    }
}

/**********************************************************************
//    Description:
//    This function is called when the timer time out.
//  1, Sometimes we send a command to SIM, the excepted RX len is 0x10, for example. But
//      SIM think the command is error, so it send us only 2 status words, 67 00, for example.
//      Now the DMA RX interrupt will never come because we want 0x10 bytes data. So we
//      set a timer to avoid this happen.
//  2, Though we didn't get the right data from SIM card, we should still notify the upper layer.
//      Or the upper layer will regard the SIM driver is dead.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_timeout(SIM_CARD_INDEX_E card_id)
{
    uint8 len_of_null = 0;
    uint8 current_rx_len = 0;
    uint8 card_index = 0;
    uint16 i = 0;
    uint32 temp_dma_rx_received_bytes = 0;
    uint32 temp_chip_id;

    SCI_TRACE_LOW("SIMHAL%d:SIM_DMA_RX_timeout",card_id);
	//SIMHAL_PrintRegisterBuf(card_id);
	
    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    temp_dma_rx_received_bytes = SIMDRV_DMA_RX_GetTransferedBytes(card_index);

    //SCI_TRACE_LOW("SIM%d:DMA_RX_timeout %d, %d. time=%d.",card_id,temp_dma_rx_received_bytes,s_simhal_dma_rx_received_bytes[card_index]);

    //DMA is still transfer data.
    if(temp_dma_rx_received_bytes > s_simhal_dma_rx_received_bytes[card_index])
    {
        temp_chip_id = CHIP_GetHWChipID();
        if (0x65300000 == temp_chip_id)
        {
            s_simhal_dma_rx_received_bytes[card_index] = temp_dma_rx_received_bytes;
        }
        else
        {
            s_simhal_dma_rx_received_bytes[card_index] = 0;
        }

        //Re-start the timer
        if(SIM_CARD_ONE == card_index)
        {
            SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_ONE]);
        }
        else
        {
            SCI_ChangeTimer(s_sim_driver_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, s_simhal_dma_rsp_time[SIM_CARD_TWO]);
        }

        SCI_ActiveTimer(s_sim_driver_timer[card_index]);
        return;//continue to wait
    }

    //SCI_ASSERT(FALSE);//Only when debug

    //Stop the DMA SIM RX
    SIMDRV_DMA_RX_Disable(card_index);
    SIMDRV_DMA_ClearAllInt(card_index);

    //SCI_TRACE_LOW("SIMHAL%d::DMA RX time out",simhal_current_sel_card);
    if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_dma_last_byte_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_dma_last_byte_timer[card_index]);
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(SCI_IsTimerActive(s_sim_3g_status_check_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_3g_status_check_timer[card_index]);
    }

#endif
    len_of_null = SIM_DMA_count_and_discard_null_byte(card_id);

    //case1: 60 60 60 00 00 00 ...
    //case2: 60 60 data1 data2 00 00... //got only part of the data
    //case3: 00 00 00...
    //case4: data1 data2 00 00...
    current_rx_len = sim_rx_dma_cfg[card_index].total_length;
    current_rx_len = dma_current_rx_total_size[card_index] - current_rx_len;

    if(current_rx_len < len_of_null)
    {
        current_rx_len = 2;//pre-set as 2
        //From the rear to the front, the first NON_ZERO byte is the last received data.
        for(i = (TB_SIM_MAX_MSG_SIZE - 1); i >= 1; i--) //(TB_SIM_MAX_MSG_SIZE-1) for i is array_index
        {
            //I will clear s_sim_rx_buf.buf as all 0 in SIMHAL_SendInstrCode
            if(0 == s_sim_rx_buf[card_index].buf[i])
            {
                continue;
            }
            else
            {
                //the current byte is 6X(or 9X) as SW1, then the next byte is SW2(must be 00).
                if((i != (TB_SIM_MAX_MSG_SIZE - 1)) &&
                    ((0x60 == ((s_sim_rx_buf[card_index].buf[i]) & 0xF0))
                     || (0x90 == ((s_sim_rx_buf[card_index].buf[i]) & 0xF0))))
                {
                    //Now, the current byte is 6x or 9x
                    if((0x91 == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x98 == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x9E == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x9F == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x67 == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x6B == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x6D == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x6E == s_sim_rx_buf[card_index].buf[i-1])
                        || (0x6F == s_sim_rx_buf[card_index].buf[i-1]))//In this loop, i>=1.
                    {
                        //In these cases, the current byte is sw2.
                        current_rx_len = i + 1; //array_index+1
                    }
                    else
                    {
                        //the current byte is sw1
                        current_rx_len = (i + 1) + 1; //the first +1 is roll back, the 2nd +1 is array_index+1. wuding.
                    }
                }
                else
                {
                    //we will regard the current byte as SW2, the previous byte should be SW1. wuding.
                    current_rx_len = i + 1; //array_index+1
                }

                break;//quit the loop
            }
        }
    }
    else
    {
        current_rx_len = current_rx_len - len_of_null;

        if(current_rx_len < 2)
        {
            current_rx_len = 2;
        }
        else if(current_rx_len > s_sim_rx_buf[card_index].rx_expected_len)
        {
            current_rx_len = s_sim_rx_buf[card_index].rx_expected_len;
        }
    }

    s_sim_rx_buf[card_index].cur_pos = current_rx_len;

    SCI_TRACE_LOW("SIMHAL%d::DMA RX T.O, %d 0x%x 0x%x. %d %d %d %d. i %d. %d %d", \
                  simhal_current_sel_card, s_sim_rx_buf[card_index].cur_pos, \
                  s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2], \
                  s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1], \
                  sim_rx_dma_cfg[card_index].total_length, \
                  dma_current_rx_total_size[card_index], len_of_null, \
                  s_sim_rx_buf[card_index].rx_expected_len, i, \
                  temp_dma_rx_received_bytes, s_simhal_dma_rx_received_bytes[card_index]);

    //check the SIM card's response data, decide whether this is a abnormal response or a normal response.
    if(s_sim_rx_buf[card_index].cur_pos >= 2)
    {
        //check SW1, if the value is 0x6X or 0x9X, I think the hardware(or SIM card) is OK.
        if((0x60 == ((s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2]) & 0xF0))
            || (0x90 == ((s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2]) & 0xF0)))
        {
            SendResponseUp(card_id);//Send the data to SIM layer and return
            return;
        }
        else
        {
            //Special Datang USIM in SS test.
            if (2 == s_sim_rx_buf[card_index].cur_pos)
            {
                //Unexpected but normal ending. ACK SW1 SW2
                if ((s_sim_tx_buf[card_index].buf[1] == s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2]) 
                    && ((0x60 == ((s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1]) & 0xF0)) 
                    || (0x90 == ((s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1]) & 0xF0))))
                {
                    //adjust the position for SendResponseUp
                    s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2] = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1];
                    s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1] = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos];
                    SendResponseUp(card_id);//Send the data to SIM layer and return
                    return;
                }
            }
        }
    }

    // 1, (s_sim_rx_buf.cur_pos < 2), indicates no data or only 1 byte data, there must has problem.
    // 2, (s_sim_rx_buf.cur_pos >= 2), but the data is a NOISE, not a SW1, there must has problem.
    SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);
    DMA_HAL_EnableDMAPause(TRUE);//enable DMA pause
}

/**********************************************************************
//    Description:
//    This is the function used to get the last one byte.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIM_DMA_RX_lastonebyte(SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;
    uint8 tmp_rx_fifo_cnt = 0; 

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    if(!sim_is_dma_last_one_byte_timer_restarted[card_id])
    {
        tmp_rx_fifo_cnt = SIMDRV_GetRxFifoCount(card_index);
        SCI_TRACE_LOW("SIMHAL%d:: DMA RX last 1 byte to get timer, fifo count num %d",card_id, tmp_rx_fifo_cnt);
        if(tmp_rx_fifo_cnt == 0)
        {
            SCI_TRACE_LOW("SIMHAL%d:: DMA RX last 1 byte to get restart timer, fifo count num %d",card_id, tmp_rx_fifo_cnt);
            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_dma_last_byte_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_DMA_RX_LAST_BYTE_AGAIN_TIMER); 
            }
            else if(SIM_CARD_TWO == card_index)
            {
                SCI_ChangeTimer(s_sim_dma_last_byte_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_DMA_RX_LAST_BYTE_AGAIN_TIMER); 
            }

            SCI_ActiveTimer(s_sim_dma_last_byte_timer[card_index]);
            sim_is_dma_last_one_byte_timer_restarted[card_id] = TRUE;
            return;
        }
    }
    
    s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos] = (uint8)SIMDRV_RecvData(card_index);

    // But the re-arrange will waste too many time in a DMA INT, so we will
    //notify upper layer with the data pointer to "data1" directly in SendResponseUp.

    //SCI_TRACE_LOW:"SIMHAL%d:DMA got the last 1 byte 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_2086_112_2_17_22_59_2_4437, (uint8 *)"dd", card_id, s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos]);

    if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
    }

    // Now all the data MUST be arranged as below:
    // data1 data2 .....data(rx_expected_len-2) status1 status2
    // Send to the SIM layer now.
    s_sim_rx_buf[card_index].cur_pos = s_sim_rx_buf[card_index].rx_expected_len;
    s_sim_rx_buf[card_index].is_locked = TRUE;//lock the rx buffer, then send it to SIM layer
    SendResponseUp(card_id);//Send the data to SIM layer
}

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//  This is the function used to check un-expected SW in time only when 3G usim STATUS.
//    Global resource dependence :
//    Author:Wuding.Yang
//    Note:
***********************************************************************/
static void SIMHAL_CheckStatusReponse(uint32 card_id)
{
    uint32 card_index = card_id;
    BOOLEAN send_resp_up = FALSE;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    if((0x6C == s_sim_rx_buf[card_index].buf[0])
        && (0x0 != s_sim_rx_buf[card_index].buf[1]))
    {
        send_resp_up = TRUE;
    }
    else if((0x91 == s_sim_rx_buf[card_index].buf[0])
       &&  (0x0b == s_sim_rx_buf[card_index].buf[1]))
    {
        SCI_TRACE_LOW("SIMHAL%d::only receive 0x91 0x0b sw in status command.",card_id);
        send_resp_up = TRUE;
    }

    if(send_resp_up)
    {
        SIMDRV_DMA_RX_Disable(card_index);

        if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
        {
            SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
        }

        if(SCI_IsTimerActive(s_sim_dma_last_byte_timer[card_index]))
        {
            SCI_DeactiveTimer(s_sim_dma_last_byte_timer[card_index]);
        }

        s_sim_rx_buf[card_index].cur_pos = 2;
        s_sim_rx_buf[card_index].is_locked = TRUE;//lock the rx buffer, then send it to SIM layer
        SendResponseUp(card_id);//Send the data to SIM layer
    }
    else
    {
        s_sim_3g_status_check_count[card_index]++;
        if(s_sim_3g_status_check_count[card_index] < SIM_3G_STATUS_CHECK_TIMER_COUNT)
        {
            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_3g_status_check_timer[card_index], SIM1_DriverTimerExpired, SIM_3G_STATUS_CHECK_TIMER_VALUE);
            }
            else
            {
                SCI_ChangeTimer(s_sim_3g_status_check_timer[card_index], SIM2_DriverTimerExpired, SIM_3G_STATUS_CHECK_TIMER_VALUE);
            }

            SCI_ActiveTimer(s_sim_3g_status_check_timer[card_index]);
            SCI_TRACE_LOW("SIMHAL%d:SIMHAL_CheckStatusReponse start timer %d again.",card_id,s_sim_3g_status_check_count[card_index]);
        }
    }
}
#endif//SIM_3G_USIM_SUPPORT

#endif//SIM_ENHANCED_PPS_SUPPORT

#ifdef DPHONE_VER2_TEST
extern void SIMSIGNAL_SIMDphoneInitReq(uint32 cur_sim_task, BOOLEAN is_entire_service);
/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM_DphoneTimerExpired(
    uint32 lparam   //the function's input parameter
)
{
    SCI_TRACE_LOW("SIM: Dphone SIM_DphoneTimerExpired is called!");
    SIMSIGNAL_SIMDphoneInitReq(0, TRUE);
}

#endif

/**********************************************************************
//    Description:
//      Reset the SIM card with or without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_ResetSIMCard(
    BOOLEAN is_1_8v_detect,
    SIM_CARD_INDEX_E card_id,
    BOOLEAN bChange //TRUE: reset the sim card with power change
    //FALSE:reset the sim card without power change
)
{
    //first clear the variable
    s_sim_reset_times[card_id] = 0;
    s_sim_b_reset_times[card_id] = 0;
    s_sim_pow_reset_times[card_id] = 0;
    s_sim_p_reset_times[card_id] = 0;

    if(bChange && (SIMDRV_WORKING_VOLT_1_8V == s_sim_working_volt[card_id]))
    {
        return ResetSIMCard(is_1_8v_detect, card_id, TB_SIM_RESET_FOR_POWER);
    }
    else
    {
        return ResetSIMCard(is_1_8v_detect, card_id, TB_SIM_RESET_NO_CHANGE);
    }
}

/**********************************************************************
//    Description:
//      Clear the PPS info structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearPPSInfo(SIM_CARD_INDEX_E card_id)
{
    s_pps_info[card_id].is_current = FALSE;
    //note:now we only support the default sppeed pps procedure,
    //but not the enhanced speed pps procedure
    s_pps_info[card_id].pps_data = s_default_pps_data;
    s_pps_info[card_id].pps_type = TB_SIM_PPS_DEFAULT_SPEED;
}

/**********************************************************************
//    Description:
//      Clear ATR data structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearATRData(SIM_CARD_INDEX_E card_id)
{
    s_atr_data[card_id].t_protocol = TB_SIM_T_0_PROTOCOL;
    s_atr_data[card_id].TA1_pos = 0;
    s_atr_data[card_id].TB1_pos = 0;
    s_atr_data[card_id].TC1_pos = 0;
    s_atr_data[card_id].TC2_pos = 0;
    s_atr_data[card_id].F = TB_SIM_DEFAULT_FACTOR_F;
    s_atr_data[card_id].D = TB_SIM_DEFAULT_FACTOR_D;
    s_atr_data[card_id].I = TB_SIM_DEFAULT_FACTOR_I;
    s_atr_data[card_id].P = TB_SIM_DEFAULT_FACTOR_P;
    s_atr_data[card_id].N = TB_SIM_DEFAULT_FACTOR_N;
}

/**********************************************************************
//    Description:
//      Notify the SIM layer the exeption interrupt
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIMHAL_NotifyExceptionInt(
    SIM_CARD_INDEX_E card_id,
    TB_SIM_INT_TYPE_E int_type //the exeption interrupt type
)
{
    SIMHAL_MSG msg;
    TB_SIM_INT_T exp_int;

    exp_int.int_type = int_type;
    msg.message = SIM_EXCEPTION_INT;

    if(s_sim_is_ext_dualsim)
    {
        msg.wparam = simhal_current_sel_card;
    }
    else
    {
        msg.wparam = card_id;
    }

    msg.lparam      = &exp_int;
    DriverNotifySIM(&msg);
}

/**********************************************************************
//    Description:
//      The callback function when SIM interrupt happen
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_Interrupt_Handler(
    TB_MSG *msg //the message parameter of the function
)
{
    SIM_CARD_INDEX_E card_id = msg->wparam;
    TB_SIM_INT_TYPE_E sim_int_type = ((TB_SIM_INT_T *)msg->lparam)->int_type;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
    }

    switch(sim_int_type)
    {
        case TB_SIMINT_TX_EMPTY:
            SIMHAL_TX_Handler(card_id);
            break;
        case TB_SIMINT_RX_FULL:
            SIMHAL_RX_Handler(card_id);
            break;
        case TB_SIMINT_RX_PARITY_ERROR:
            SIMHAL_RX_ParityErr_Handler(card_id);
            break;
        case TB_SIMINT_TX_PARITY_ERROR:
            SIMHAL_TX_ParityErr_Handler(card_id);
            break;
        case TB_SIMINT_CARD_UNRESPONSE:
            SIMHAL_Unresponse_Handler(card_id);
            break;
        case TB_SIMINT_CARD_IN:
            SIMHAL_CardIn_Handler(card_id);
            break;
        case TB_SIMINT_CARD_OUT:
            SIMHAL_CardOut_Handler(card_id);
            break;
        case TB_SIMINT_EARLY_ATR:
            SIMHAL_EarlyATR_Handler(card_id);
            break;
        case TB_SIMINT_ACTIVE_DONE:
            //SCI_TRACE_LOW:"SIMHAL:do not deal active done Int"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_2306_112_2_17_22_59_2_4438, (uint8 *)"");
            break;
        default:
            SCI_ASSERT(FALSE);/*assert verified */
            break;
    }
}

/**********************************************************************
//    Description:
//      Initialize the SIM HAL and Driver
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMHAL_Init(SIM_CARD_INDEX_E card_id)
{
    static BOOLEAN first_enter_flag = TRUE;
    static uint32 first_init_sim_controller_index = 0;
    static BOOLEAN second_enter_flag = TRUE;

    uint32 status = TB_SUCCESS;
#ifdef SIM_PLUG_IN_SUPPORT
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
    GPIO_CFG_INFO_T_PTR sim_plugin_gpio = PNULL;
#endif
#endif

	uint8 card_index = (uint8)card_id;

#ifdef DPHONE_VER2_TEST

    if(0 == card_index)
    {
        SCI_TRACE_LOW("SIM:Dphone time out open here!");
        s_sim_dphone_ver2_test_timer = SCI_CreateTimer("SIM Dphone Timer",
                                                       SIM_DphoneTimerExpired, 0, 90000, SCI_AUTO_ACTIVATE);
    }

#endif

    //these code should be called when the 1st time, while the card_id may not be 0.
    if(first_enter_flag)
    {
        first_enter_flag = FALSE;

        SIMHAL_IsExtDualSim();//Get the paltform ang BB chip type;
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE); //donot move!
        first_init_sim_controller_index = card_index;
        s_sim_ldo_stable_time = SIMDRV_GetLDOStableTime();//Get the Chip's SIM LDO stable time
#ifdef SIM_6302_SWITCH
        s_sim_ldo_stable_time += 10;//The external switch(6302) LDO output stable time is very very small, measured by wuding.
        SIMHAL_6302_InitSPI();//init SPI and 6302 in the first time
#endif

        SIMDRV_DisableSIMLDO(SIM_CARD_ONE);//Disable the Vsim1 LDO
        SIMDRV_DisableSIMLDO(SIM_CARD_TWO);//Disable the Vsim2 LDO

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        if(SC6531EFM == s_sim_chip_type)
#else
        if(SC6530 == s_sim_chip_type)
#endif
        {
            SIMDRV_DisableSIMLDO(2);//Disable the Vsim3 LDO
        }

        status = SIMDRV_InitDriver(card_index, SIMHAL_Interrupt_Handler);
#ifdef SIM_ENHANCED_PPS_SUPPORT

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            SIMHAL_DMA_Init((SIM_CARD_INDEX_E)card_index);
        }

#endif

        //init all timers
        s_sim_driver_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Driver Timer",
                                                           SIM1_DriverTimerExpired, 1, SIM_STATUS_WORD_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_atr_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM ATR Timer",
                                                        SIM1_DriverTimerExpired, 2, SIM_ATR_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_reset_p_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Reset P Timer",
                                                            SIM1_DriverTimerExpired, 3, SIM_RESET_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_reset_bt_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Reset BT Timer",
                                                             SIM1_DriverTimerExpired, 4, SIM_RESET_TIMER_VALUE, SCI_NO_ACTIVATE);

#ifdef SIM_ENHANCED_PPS_SUPPORT
        s_sim_dma_last_byte_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Reset WAIT LDO Timer",
                                                                  SIM1_DriverTimerExpired, 5, SIM_WAIT_LDO_TIMER_VALUE, SCI_NO_ACTIVATE);
#endif
        s_sim_pps_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM PPS Timer",
                                                        SIM1_DriverTimerExpired, 6, SIM_WAIT_PPS_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_init_wait_ldo_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Init WAIT LDO Timer",
                                                                  SIM1_InitDrvAfterLdoValid, card_id, s_sim_ldo_stable_time, SCI_NO_ACTIVATE);

        s_sim_reset_wait_ldo_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Reset WAIT LDO Timer",
                                                                   SIM1_ResetAfterLdoValid, card_id, s_sim_ldo_stable_time, SCI_NO_ACTIVATE);

        s_sim_decode_atr_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Deocde ATR Timer",
                                                               SIM1_DriverTimerExpired, 7, SIM_WAIT_DECODE_ATR_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_tb_te_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM Tb Te Timer",
                                                          SIM1_RST_After_Tb_or_Te, card_id, SIM_RST_TB_TE_TIMER_VALUE, SCI_NO_ACTIVATE);

#ifdef SIM_ENHANCED_PPS_SUPPORT
#ifdef SIM_3G_USIM_SUPPORT
        s_sim_3g_status_check_timer[SIM_CARD_ONE] = SCI_CreateTimer("SIM 3G Status Timer",
                                                                    SIM1_DriverTimerExpired, 8, SIM_3G_STATUS_CHECK_TIMER_VALUE, SCI_NO_ACTIVATE);
        s_sim_3g_status_check_count[SIM_CARD_ONE] = 0;
#endif
#endif
#ifdef SIM_PLUG_IN_SUPPORT
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
        s_sim_hot_swap_stable_timer = SCI_CreateTimer("SIM hotswap Timer",
                                                      SIMHAL_SIM_Hotswap_Stable_TimeOut, card_id, SIM_HOTSWAP_STABLE_TIMER_VALUE, SCI_NO_ACTIVATE);
#endif
#endif
        s_sim_driver_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Driver Timer",
                                                           SIM2_DriverTimerExpired, 1, SIM_STATUS_WORD_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_atr_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM ATR Timer",
                                                        SIM2_DriverTimerExpired, 2, SIM_ATR_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_reset_p_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Reset P Timer",
                                                            SIM2_DriverTimerExpired, 3, SIM_RESET_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_reset_bt_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Reset BT Timer",
                                                             SIM2_DriverTimerExpired, 4, SIM_RESET_TIMER_VALUE, SCI_NO_ACTIVATE);

#ifdef SIM_ENHANCED_PPS_SUPPORT
        s_sim_dma_last_byte_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Reset WAIT LDO Timer",
                                                                  SIM2_DriverTimerExpired, 5, SIM_WAIT_LDO_TIMER_VALUE, SCI_NO_ACTIVATE);
#endif
        s_sim_pps_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM PPS Timer",
                                                        SIM2_DriverTimerExpired, 6, SIM_WAIT_PPS_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_init_wait_ldo_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Init WAIT LDO Timer",
                                                                  SIM2_InitDrvAfterLdoValid, card_id, s_sim_ldo_stable_time, SCI_NO_ACTIVATE);

        s_sim_reset_wait_ldo_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Reset WAIT LDO Timer",
                                                                   SIM2_ResetAfterLdoValid, card_id, s_sim_ldo_stable_time, SCI_NO_ACTIVATE);

        s_sim_decode_atr_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Deocde ATR Timer",
                                                               SIM2_DriverTimerExpired, 7, SIM_WAIT_DECODE_ATR_TIMER_VALUE, SCI_NO_ACTIVATE);

        s_sim_tb_te_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM Tb Te Timer",
                                                          SIM2_RST_After_Tb_or_Te, card_id, SIM_RST_TB_TE_TIMER_VALUE, SCI_NO_ACTIVATE);

#ifdef SIM_ENHANCED_PPS_SUPPORT
#ifdef SIM_3G_USIM_SUPPORT
        s_sim_3g_status_check_timer[SIM_CARD_TWO] = SCI_CreateTimer("SIM 3G Status Timer",
                                                                    SIM2_DriverTimerExpired, 8, SIM_3G_STATUS_CHECK_TIMER_VALUE, SCI_NO_ACTIVATE);
        s_sim_3g_status_check_count[SIM_CARD_TWO] = 0;                                                                    
#endif
#endif

#ifdef SIM_PLUG_IN_SUPPORT
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
        //the sim x must match the correct gpio. for example, sim0 is connected with gpio66.
        DIAG_RegisterCmdRoutine(DIAG_SIMCARD_F, SetSIMCardStatus);

        sim_plugin_gpio = GPIO_PROD_GetCfgInfo(GPIO_PROD_SIM_PLUG_IN_ID);

        if(sim_plugin_gpio)
        {
            sim_plugin_gpio->gpio_cb_fun = (GPIO_CB)SIMHAL_SIM_Plugin_GPIO_Callback;
            GPIO_PROD_SetCfgInfo(sim_plugin_gpio);
        }
#endif
#endif
    }
    else if(second_enter_flag)
    {
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE); //chip_id should be here!!!

        if(first_init_sim_controller_index == card_index)
        {
            return status;//the sim controller has been inited, wait for the next sim_task id.
        }
        else
        {
            second_enter_flag = FALSE;
        }

        if(s_sim_is_ext_dualsim) //L only init once
        {        
        #if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
			if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
		#else
            if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
		#endif
            {
                return status;
            }
        }

        status = SIMDRV_InitDriver(card_index, SIMHAL_Interrupt_Handler);
#ifdef SIM_ENHANCED_PPS_SUPPORT

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            SIMHAL_DMA_Init((SIM_CARD_INDEX_E)card_index);
        }

#endif
    }

    return status;
}

/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM1_ResetAfterLdoValid(
    uint32 lparam   //the function's input parameter
)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_ONE;
    SIM_CARD_INDEX_E card_index = SIM_CARD_ONE;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);
    }

    //SCI_TRACE_LOW:"SIMHAL0::Reset wait ldo timer expired. %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_2565_112_2_17_22_59_3_4439, (uint8 *)"d", card_id);

    //Set the Vcc on, then CLK on ,then I/O on.
    {
        SIMDRV_StartToReset(card_index);
    }

    //Start a Timer here, to keep a Tb/Te interval's SIM_RST LOW after SIM_Vcc/CLK on.
    SCI_ChangeTimer(s_sim_tb_te_timer[SIM_CARD_ONE], SIM1_RST_After_Tb_or_Te, SIM_RST_TB_TE_TIMER_VALUE);
    SCI_ActiveTimer(s_sim_tb_te_timer[SIM_CARD_ONE]);

    return;
}

/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM2_ResetAfterLdoValid(
    uint32 lparam   //the function's input parameter
)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_TWO;
    SIM_CARD_INDEX_E card_index = SIM_CARD_TWO;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            SCI_ASSERT(0);/*assert verified */
        }
    }

    //SCI_TRACE_LOW:"SIMHAL1::Reset wait ldo timer expired."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_2635_112_2_17_22_59_3_4440, (uint8 *)"");

    //Set the Vcc on, then CLK on ,then I/O on.
    {
        SIMDRV_StartToReset(card_index);
    }

    //Start a Timer here, to keep a Tb/Te interval's SIM_RST LOW after SIM_Vcc/CLK on.
    SCI_ChangeTimer(s_sim_tb_te_timer[SIM_CARD_TWO], SIM2_RST_After_Tb_or_Te, SIM_RST_TB_TE_TIMER_VALUE);
    SCI_ActiveTimer(s_sim_tb_te_timer[SIM_CARD_TWO]);

    return;
}

/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM1_DriverTimerExpired(
    uint32 lparam   //the function's input parameter
)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_ONE;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
    }

    //the status words distinguish timer
    if(1 == lparam)
    {
#ifdef SIM_ENHANCED_PPS_SUPPORT

        if(sim_dma_support[card_id])
        {
            SIM_DMA_RX_timeout(card_id);
            return;
        }

#endif
        SendResponseUp(card_id);
    }
    //the atr wait timer
    else if(2 == lparam)
    {

        ReportCardStatus(card_id, TB_SIM_INSERTED_CLASS_CHANGE);

    }
    else if(3 == lparam)
    {
        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_FOR_PARITY))
        {
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_RX_PARITY_ERROR);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }
        }
    }
    else if(4 == lparam)
    {
        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_FOR_BT))
        {
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }
        }
    }

#ifdef SIM_ENHANCED_PPS_SUPPORT
    else if(5 == lparam)
    {
        SIM_DMA_RX_lastonebyte(card_id);
    }

#endif
    else if(6 == lparam)
    {
        s_sim_pps_req_times[card_id]++;

        if(s_sim_pps_req_times[card_id] > 2)
        {
            s_previous_pps_error[card_id] = TRUE;
            s_sim_reset_times[card_id] -= 1;//to avoid exceed the reset time limit
        }

        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_NO_CHANGE))
        {
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_OUT);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }
        }
    }
    else if(7 == lparam)
    {
        //Decode ATR time out
        if(s_sim_is_ext_dualsim)
        {
            s_sim_status[card_id] = TB_SIM_NO_CARD;
        }

        SIMHAL_NotifyExceptionInt(SIM_CARD_ONE, TB_SIMINT_CARD_OUT);
    }

#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_ENHANCED_PPS_SUPPORT
    else if(8 == lparam)
    {
        SIMHAL_CheckStatusReponse(card_id);
    }

#endif
#endif
}

/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIM2_DriverTimerExpired(
    uint32 lparam   //the function's input parameter
)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_TWO;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
    }

    //the status words distinguish timer
    if(1 == lparam)
    {
#ifdef SIM_ENHANCED_PPS_SUPPORT

        if(sim_dma_support[card_id])
        {
            SIM_DMA_RX_timeout(card_id);
            return;
        }

#endif
        SendResponseUp(card_id);
    }
    //the atr wait timer
    else if(2 == lparam)
    {

        ReportCardStatus(card_id, TB_SIM_INSERTED_CLASS_CHANGE);

    }
    else if(3 == lparam)
    {
        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_FOR_PARITY))
        {
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_RX_PARITY_ERROR);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }
        }
    }
    else if(4 == lparam)
    {
        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_FOR_BT))
        {
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }
        }
    }

#ifdef SIM_ENHANCED_PPS_SUPPORT
    else if(5 == lparam)
    {
        SIM_DMA_RX_lastonebyte(card_id);
    }

#endif
    else if(6 == lparam)
    {
        s_sim_pps_req_times[card_id]++;

        if(s_sim_pps_req_times[card_id] > 2)
        {
            s_previous_pps_error[card_id] = TRUE;
            s_sim_reset_times[card_id] -= 1;//to avoid exceed the reset time limit
        }

        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_NO_CHANGE))
        {
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_OUT);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }
        }
    }
    else if(7 == lparam)
    {
        //Decode ATR time out
        if(s_sim_is_ext_dualsim)
        {
            s_sim_status[card_id] = TB_SIM_NO_CARD;
        }

        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_OUT);
    }

#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_ENHANCED_PPS_SUPPORT
    else if(8 == lparam)
    {
        SIMHAL_CheckStatusReponse(card_id);
    }

#endif
#endif
}

/**********************************************************************
//    Description:
//      Clear the Tx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearTxBuf(SIM_CARD_INDEX_E card_id)
{
#ifdef MULTI_SIM_SYS_QUAD
    uint8 card_index = 0;
#else
    uint8 card_index = (uint8)card_id;
#endif

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    s_sim_tx_buf[card_index].cur_pos = 0;
    s_sim_tx_buf[card_index].tx_data_len = 0;
}

/**********************************************************************
//    Description:
//      Clear the Rx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearRxBuf(SIM_CARD_INDEX_E card_id)
{
#ifdef MULTI_SIM_SYS_QUAD
    uint8 card_index = 0;
#else
    uint8 card_index = (uint8)card_id;
#endif

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    s_sim_rx_buf[card_index].check_pos = 0;
    s_sim_rx_buf[card_index].cur_pos = 0;
    s_sim_rx_buf[card_index].next_atr_td_pos = 0;
    s_sim_rx_buf[card_index].rx_expected_len = 0;
    s_sim_rx_buf[card_index].is_locked = TRUE;
}

/**********************************************************************
//    Description:
//      report the SIM card insert status to the SIM manager task
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ReportCardStatus(
    SIM_CARD_INDEX_E card_id,
    SIMDRV_INSERT_STATUS_E insert_status   //the SIM card insert status
)
{
    SIMHAL_MSG msg;
    SIMDRV_CARD_INSERT_T card_insert;
    SIM_CARD_INDEX_E card_index = card_id;

    if(s_sim_is_ext_dualsim)
    {
        card_index = simhal_current_sel_card;
        SCI_TRACE_LOW("SIMHAL%d:ReportCardStatus %d, %d", card_id, card_index, insert_status);
    }

    card_insert.insert_status = insert_status;
    card_insert.t_protocol = s_atr_data[card_id].t_protocol;
    card_insert.working_volt = s_sim_working_volt[card_index];

    msg.message = SIM_CARD_INSERT;
    msg.wparam = card_index;
    msg.lparam = &card_insert;
    DriverNotifySIM(&msg);
}

/**********************************************************************
//    Description:
//      Initialize the SIM card Driver,make it ready to work when needed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_ResetDriver(
    BOOLEAN is_1_8v_detect,
    SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

#ifdef MULTI_SIM_SYS_TRI
    //SCI_TRACE_LOW:"SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_2988_112_2_17_22_59_4_4442, (uint8 *)"dddddddd", card_id, is_1_8v_detect, s_simhal_1_8v_detect_complete[0], s_simhal_1_8v_detect_complete[1], s_simhal_1_8v_detect_complete[2], s_simhal_has_1_8v_only[0], s_simhal_has_1_8v_only[1], s_simhal_has_1_8v_only[2]);
#elif defined MULTI_SIM_SYS_QUAD
    //SCI_TRACE_LOW:"SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d, %d %d %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_2992_112_2_17_22_59_4_4443, (uint8 *)"dddddddddd", card_id, is_1_8v_detect, s_simhal_1_8v_detect_complete[0], s_simhal_1_8v_detect_complete[1], s_simhal_1_8v_detect_complete[2], s_simhal_1_8v_detect_complete[3], s_simhal_has_1_8v_only[0], s_simhal_has_1_8v_only[1], s_simhal_has_1_8v_only[2], s_simhal_has_1_8v_only[3]);
#else
    //SCI_TRACE_LOW:"SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_2995_112_2_17_22_59_4_4444, (uint8 *)"dddddd", card_id, is_1_8v_detect, s_simhal_1_8v_detect_complete[0], s_simhal_1_8v_detect_complete[1], s_simhal_has_1_8v_only[0], s_simhal_has_1_8v_only[1]);
#endif


    //the SIM card working status
    if(is_1_8v_detect)
    {
        s_sim_status[card_id] = TB_SIM_1_8V_DETECT;
    }
    else
    {
        s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;
    }

    //set the driver variable
    s_sim_reset_times[card_id] = 0;
    s_sim_b_reset_times[card_id] = 0;
    s_sim_pow_reset_times[card_id] = 0;
    s_sim_p_reset_times[card_id] = 0;

    //the SIM card Transfer protocal _protocol
    s_not_t0_protocol_occurs[card_id] = FALSE;
    s_first_protocol_occurs[card_id] = FALSE;

    //s_previous_pps_error[card_id] = FALSE;
#ifdef SIM_ENHANCED_PPS_SUPPORT//for dma debug
    sim_dma_support[card_id] = FALSE;
#endif

    //initialize the buffer
    s_sim_atr_data_len[card_id] = 0;
    s_have_ack_byte[card_index] = FALSE;
    s_null_byte_count[card_index] = 0;

    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearATRData(card_id);
    ClearPPSInfo(card_id);

    if(s_sim_is_ext_dualsim)
    {
        SIMHAL_Select_Card(card_id);
    }

    if((!s_simhal_1_8v_detect_complete[card_id]) || s_simhal_has_1_8v_only[card_id])
    {
        s_sim_working_volt[card_id] = SIMDRV_WORKING_VOLT_1_8V;
    }
    else
    {
        s_sim_working_volt[card_id] = SIMDRV_WORKING_VOLT_3V;// 1.8v detect, we only afford 1.8v volt
    }

    SIMHAL_SetSIMVoltage(card_id, s_sim_working_volt[card_id]);

    if(s_sim_is_ext_dualsim)
    {
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, TRUE);
            SCI_TRACE_LOW("SIMHAL%d:enable SIM LDO.",card_id);
            SIMDRV_EnableSIMLDO(card_index);//SC6600L, sim0_pe=sim0_ldo_set|(sim_pe&!sim0_ldo_rst),the same as sim1.
            card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE); //recover
        }
    }

    if(SIM_CARD_ONE == card_index)
    {
        SCI_ChangeTimer(s_sim_init_wait_ldo_timer[SIM_CARD_ONE], SIM1_InitDrvAfterLdoValid, s_sim_ldo_stable_time);
        SCI_ActiveTimer(s_sim_init_wait_ldo_timer[SIM_CARD_ONE]);
    }
    else
    {
        SCI_ChangeTimer(s_sim_init_wait_ldo_timer[SIM_CARD_TWO], SIM2_InitDrvAfterLdoValid, s_sim_ldo_stable_time);
        SCI_ActiveTimer(s_sim_init_wait_ldo_timer[SIM_CARD_TWO]);
    }
}

/**********************************************************************
//    Description:
//      go no the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SIM1_InitDrvAfterLdoValid(
    uint32 lparam   //the function's input parameter
)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_ONE;
    uint8 card_index = (uint8)card_id;

    //SCI_TRACE_LOW:"SIMHAL0::Init wait ldo timer expired. %d %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3085_112_2_17_22_59_4_4445, (uint8 *)"dd", s_sim_status[SIM_CARD_ONE], s_sim_status[SIM_CARD_TWO]);
    SCI_TRACE_LOW("SIMHAL%d:Init wait ldo timer expired.",simhal_current_sel_card);

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
    }

    //enable SIM module

    s_sim_clk_mode[card_id] = SIMDRV_InitAfterLDOValid(card_index);

    s_sim_clk_dvd_value[card_id] = 0xba0;//set as the default value.
}

/**********************************************************************
//    Description:
//      go no the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SIM2_InitDrvAfterLdoValid(
    uint32 lparam   //the function's input parameter
)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_TWO;
    uint8 card_index = (uint8)card_id;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            SCI_ASSERT(0);/*assert verified */
        }
    }

    //SCI_TRACE_LOW:"SIMHAL%d::Init wait ldo timer expired."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3152_112_2_17_22_59_4_4446, (uint8 *)"d", card_id);

    s_sim_clk_mode[card_id] = SIMDRV_InitAfterLDOValid(card_index);

    s_sim_clk_dvd_value[card_id] = 0xba0;//set as the default value.
}

/**********************************************************************
//    Description:
//     Handle the TX Empty Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_TX_Handler(SIM_CARD_INDEX_E card_id)
{
    TransmitInInt(card_id);
}

/**********************************************************************
//    Description:
//     Handle the RX Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_RX_Handler(SIM_CARD_INDEX_E card_id)
{
    uint8 temp, water_mark;
    uint16 i = 0;
    uint8 rx_fifo_count = 0;
    SIM_CARD_INDEX_E card_index = card_id;
#ifdef MULTI_SIM_SYS_TRI
    static uint8 null_bytes_num[SIM_CARD_NUM] = {0, 0, 0};
#elif defined (MULTI_SIM_SYS_QUAD)
    static uint8 null_bytes_num[SIM_CARD_NUM] = {0, 0, 0, 0};
#else
    static uint8 null_bytes_num[SIM_CARD_NUM] = {0, 0};
#endif

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    rx_fifo_count = SIMDRV_GetRxIntBufCount(card_index);//get the rx data number

    //Some destroyed SIM cards will send too many empty data to chip, these too many Interrupts will leads to a DSP over run.
    //If we get 25 EMPTY data continously, we think these happen.
    if((TB_SIM_1_8V_DETECT == s_sim_status[card_id]) || (TB_SIM_WAITING_FOR_ATR == s_sim_status[card_id]))
    {
        if(0 == rx_fifo_count)
        {

            temp = SIMDRV_ReadIntRxBufData(card_index);

            s_simhal_rx_noise_data[card_id]++;

            if(s_simhal_rx_noise_data[card_id] > 25)
            {
                s_simhal_rx_noise_data[card_id] = 0;
                //we will reject this SIM, the Timers about reset will make the procedure continue.
                SIMHAL_RejectSIMCard(card_id);
                return;
            }
        }
        else
        {
            s_simhal_rx_noise_data[card_id] = 0;
        }
    }

    //receive response data from the rx fifo buf
    for(i = 0; i < rx_fifo_count; i++)
    {
        temp = SIMDRV_ReadIntRxBufData(card_index);

        if(((TB_SIM_1_8V_DETECT == s_sim_status[card_id]) || (TB_SIM_WAITING_FOR_ATR == s_sim_status[card_id]))
            && (SIMDRV_WORKING_VOLT_1_8V == s_sim_working_volt[card_id]))
        {
            if(0 == temp)
            {
                null_bytes_num[card_id]++;
            }
            else
            {
                null_bytes_num[card_id] = 0;
            }

            if(null_bytes_num[card_id] > 20)
            {
                null_bytes_num[card_id] = 0;
                SIMHAL_Hardware_PowerOffSIM(card_id);
                return;
            }
        }

        //the sim recv buf is been locked,give up the data.
        if(!s_sim_rx_buf[card_index].is_locked)
        {
            //to the SIM command,there are ACK byte and NULL byte should be process
            //different with the command SIM data
            if((!s_have_ack_byte[card_index])
                && (0 == s_sim_rx_buf[card_index].cur_pos)
                && (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id] || TB_SIM_1_8V_DETECT == s_sim_status[card_id]))
            {
                //Raisling.Kong 2002-8-28   reference to the ISO-7816 part3
                //if the ACK byte is the (INS) or (INS + 1),all the remainning data
                //byte are transferred subsquently
                if((s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS] == temp)
                    || (s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS] + 1 == temp))
                {
                    //if have receive the ack byte,regulate the water mark
                    water_mark = SIMHAL_CAL_RX_INT_MARK(s_sim_rx_buf[card_index].rx_expected_len);
                    SIMDRV_SetRxWaterMark(card_index, water_mark);
                    //////////////////////
                    SIMDRV_DisableTx(card_index);
                    SIMDRV_ResetTxFifo(card_index);//wuding add to reset chip's Tx fifo according to RF Lab's test
                    //////////////////////
                    TransmitInInt(card_id);
                    s_have_ack_byte[card_index] = TRUE;
                    s_null_byte_count[card_index] = 0;
                }
                //if the ACK byte is ~(INS) or ~(INS + 1),transform the next byte data
                else if(((uint8)(~s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS]) == temp)
                        || ((uint8)(~(s_sim_tx_buf[card_index].buf[TB_SIM_APDU_INS] + 1)) == temp))
                {
                    //////////////////////
                    SIMDRV_DisableTx((uint8)card_index);
                    SIMDRV_ResetTxFifo((uint8)card_index);//wuding add to reset chip's Tx fifo according to RF Lab's test
                    //////////////////////
                    s_have_ack_byte[card_index] = SendOneByteToSIM(card_id);

                    if(s_have_ack_byte[card_index])
                    {
                        //if have receive the ack byte,regulate the water mark
                        water_mark = SIMHAL_CAL_RX_INT_MARK(s_sim_rx_buf[card_index].rx_expected_len);
                        SIMDRV_SetRxWaterMark(card_index, water_mark);
                    }

                    s_null_byte_count[card_index] = 0;
                }
                //it's the null data from the SIM
                else if(TB_SIM_RESPONSE_NULL_BYTE == temp)
                {
                    s_null_byte_count[card_index]++;

                    if(s_null_byte_count[card_index] == SIM_MAX_NULL_BYTE_COUNT)
                    {
                        s_null_byte_count[card_index] = 0;
                        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_NEED_WAITTING);
                    }
                }
                //if the data is neither the ack not null byte,put it into the rx fifo.
                else
                {
                    s_sim_rx_buf[card_index].buf[0] = temp;
                    s_sim_rx_buf[card_index].cur_pos = 1;
                    sim_response_data_present[card_index] = TRUE;
                    s_null_byte_count[card_index] = 0;
                }
            }
            //in all other case,this byte data is a real byte SIM data
            else
            {
                //if there is null byte between ACK byte and Status word,discard it.
                if((temp == TB_SIM_RESPONSE_NULL_BYTE)
                    && (!sim_response_data_present[card_index])
                    && (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id])
                    && (!s_have_received_response_data[card_index]))
                {
                    s_null_byte_count[card_index]++;

                    if(s_null_byte_count[card_index] == SIM_MAX_NULL_BYTE_COUNT)
                    {
                        s_null_byte_count[card_index] = 0;
                        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_NEED_WAITTING);
                    }
                }
                else
                {
                    if(s_sim_rx_buf[card_index].cur_pos < TB_SIM_MAX_MSG_SIZE)
                    {
                        s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos] = temp;
                        s_have_received_response_data[card_index] = TRUE;
                        s_sim_rx_buf[card_index].cur_pos++;
                    }
                    else
                    {
                        SIMDRV_DisableRx(card_index);
                        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);
                        return;
                    }
                }
            }
        }
        //this byte data come when there should be no SIM data from,abandon this byte data
        else
        {
            //unknown data whithout rx parity error
            //delete the unknown data buffer. It just waste the memory. wuding.
        }
    }

    //if there are new rx data received,check the rx data
    if((!s_sim_rx_buf[card_index].is_locked)
        && (s_sim_rx_buf[card_index].cur_pos > s_sim_rx_buf[card_index].check_pos)
        && (rx_fifo_count != 0))
    {
        //the SIM driver timer is active,the dective the driver timer
        if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
        {
            SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
        }

        CheckReceiveBuf(card_id);
    }
}

/**********************************************************************
//    Description:
//      Handle the RX Parity Error Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_RX_ParityErr_Handler(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    if(s_sim_status[card_id] != TB_SIM_RECEIVING_RESPONSE)
    {
        //the time reset times exeed the max reset time
        if(s_sim_p_reset_times[card_id] >= s_sim_max_reset_times)
        {

            ReportCardStatus(card_id, TB_SIM_INSERTED_ERROR);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }

            SIMHAL_Hardware_PowerOffSIM(card_id);
        }
        //set the time to reset the sim card
        else
        {
            //disable the parity error interrupt and set the she register map to zero
            ClearRxBuf(card_id);
            SIMDRV_DisableRx(card_index);
            SIMDRV_DisableSIMAllInt(card_index);
            SCI_DeactiveTimer(s_sim_pps_timer[card_index]);
            SCI_DeactiveTimer(s_sim_reset_bt_timer[card_index]);
            SCI_DeactiveTimer(s_sim_atr_timer[card_index]);
            SCI_DeactiveTimer(s_sim_reset_p_timer[card_index]);
            SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);
            SCI_DeactiveTimer(s_sim_tb_te_timer[card_index]);
            s_card_resetting[card_id] = FALSE;

            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_reset_p_timer[card_index], SIM1_DriverTimerExpired, SIM_RESET_TIMER_VALUE);
            }
            else
            {
                SCI_ChangeTimer(s_sim_reset_p_timer[card_index], SIM2_DriverTimerExpired, SIM_RESET_TIMER_VALUE);
            }

            SCI_ActiveTimer(s_sim_reset_p_timer[card_index]);
            s_card_resetting[card_id] = FALSE; //We have stoped RESET, so we should clear the flag. Or dead-wait.
        }
    }
    else
    {
#ifdef SIM_ENHANCED_PPS_SUPPORT

        //Stop the DMA SIM RX
        if(sim_dma_support[card_id])
        {
            SIMDRV_DMA_RX_Disable(card_index);
        }

        if(s_sim_rx_buf[card_index].is_locked)
        {
            //when the data is not the expected response data,just discard it
            //disable the rx interrupt
            SIMDRV_DisableSIMRXInt(card_index);
            SIMDRV_ResetRxFifo(card_index);
            //enable the rx full interrupter
            SIMDRV_EnableSIMRXInt(card_index);
        }
        else
        {
            //I think we should not ASSERT always, notify the upper layer.
            //the SIM task will restore SIM card.
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);
        }

        //SCI_TRACE_LOW:"SIMHAL%d::rx parity error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3463_112_2_17_22_59_5_4447, (uint8 *)"d", card_id);

#else

        //raislin.kong 2003-9-17
        if(s_sim_rx_buf[card_index].is_locked)
        {
            //when the data is not the expected response data,just discard it
            //disable the rx interrupt
            SIMDRV_DisableSIMRXInt(card_index);
            SIMDRV_ResetRxFifo(card_index);
            //enable the rx full interrupter
            SIMDRV_EnableSIMRXInt(card_index);
        }
        else
        {
            //I think we should not ASSERT always, notify the upper layer.
            //the SIM task will restore SIM card.
            SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);
        }

        //SCI_TRACE_LOW:"SIMHAL%d::rx parity error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3482_112_2_17_22_59_5_4448, (uint8 *)"d", card_id);
#endif
    }
}

/**********************************************************************
//    Description:
//      Handle the TX Parity Error Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_TX_ParityErr_Handler(SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    //SCI_TRACE_LOW:"SIMHAL%d::TxParityErrorIntP"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3499_112_2_17_22_59_5_4449, (uint8 *)"d", card_id);

    if(s_sim_status[card_id] != TB_SIM_RECEIVING_RESPONSE)
    {
        //change the odd parity check setting in the chip,then reset the SIM card
        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_FOR_PARITY))
        {

            ReportCardStatus(card_id, TB_SIM_INSERTED_ERROR);

            if(s_sim_is_ext_dualsim)
            {
                s_sim_status[card_id] = TB_SIM_NO_CARD;
            }

            SIMHAL_Hardware_PowerOffSIM(card_id);
        }
    }
    else
    {
#ifdef SIM_ENHANCED_PPS_SUPPORT

        if(sim_dma_support[card_id])
        {
            SIMDRV_DMA_TX_Disable(card_index);
        }

#endif
        //disable the TX parity error interrupt to make sure TX parity error interrupt
        //will not come continuously
        SIMDRV_DisableSIM_TxParityInt((uint8)card_index);
        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_TX_PARITY_ERROR);
        //reject the SIM card by power the SIM Card
        SIMHAL_PowerOffSIM(card_id);
    }
}

/**********************************************************************
//    Description:
//      Handle the unresponse Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_Unresponse_Handler(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    if(s_sim_status[card_id] == TB_SIM_RECEIVING_RESPONSE)
    {
#ifdef SIM_ENHANCED_PPS_SUPPORT

        if(sim_dma_support[card_id])
        {
            SIMDRV_DMA_RX_Disable(card_index);
            SIMDRV_DMA_TX_Disable(card_index);
            //SCI_TRACE_LOW:"SIMHAL::DMA Card Unresponse Int"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3553_112_2_17_22_59_5_4450, (uint8 *)"");
        }

#endif
        //notify the SIM layer that the SIM card is unresponse
        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);
    }
    else
    {
        if(s_sim_p_reset_times[card_id] < s_sim_max_reset_times)
        {
            //disable the parity error interrupt and set the she register map to zero
            ClearRxBuf(card_id);
            SIMDRV_DisableRx(card_index);
            SIMDRV_DisableSIMAllInt(card_index);
            SCI_DeactiveTimer(s_sim_pps_timer[card_index]);
            SCI_DeactiveTimer(s_sim_reset_bt_timer[card_index]);
            SCI_DeactiveTimer(s_sim_atr_timer[card_index]);
            SCI_DeactiveTimer(s_sim_reset_p_timer[card_index]);
            SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);
            SCI_DeactiveTimer(s_sim_tb_te_timer[card_index]);

            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_reset_p_timer[card_index], SIM1_DriverTimerExpired, SIM_RESET_TIMER_VALUE);
            }
            else
            {
                SCI_ChangeTimer(s_sim_reset_p_timer[card_index], SIM2_DriverTimerExpired, SIM_RESET_TIMER_VALUE);
            }

            SCI_ActiveTimer(s_sim_reset_p_timer[card_index]);
            s_card_resetting[card_id] = FALSE;//We have stoped RESET, so we should clear the flag. Or dead-wait.
        }
    }

    SCI_TRACE_LOW("SIMHAL%d:CardUnResponseIntP. %d", card_id, s_sim_status[card_id]);
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_HAL_3559_112_2_17_22_59_5_4451,(uint8*)"d",card_id);
}

/**********************************************************************
//    Description:
//      Handle the card in Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_CardIn_Handler(SIM_CARD_INDEX_E card_id)
{
#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    //SC6530 has the guard_time_err interrupt instead of card_in interrupt.
    //We donot need the guard_time_err interrupt, so I discard it.
    //SC6530 sim_drv will also disable this interrupt by Yong.Li.
    SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_IN);
#endif
}

/**********************************************************************
//    Description:
//      Handle the card out Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_CardOut_Handler(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_OUT);
#ifdef SIM_ENHANCED_PPS_SUPPORT

    if(sim_dma_support[card_id])
    {
        SIMDRV_DMA_RX_Disable(card_index);
        SIMDRV_DMA_TX_Disable(card_index);
    }

#endif

    if(s_sim_is_ext_dualsim)
    {
        s_sim_status[card_id] = TB_SIM_NO_CARD;
    }

    //reject the SIM card by power the SIM Card
    SIMHAL_RejectSIMCard(card_id);
}

/**********************************************************************
//    Description:
//      Handle the early ATR Interrupt from sim drv
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_EarlyATR_Handler(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    if(SIMDRV_IsCardOutLatched((uint8)card_index))
    {
        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_OUT);
        SCI_DeactiveTimer(s_sim_atr_timer[card_index]);

        if(s_sim_is_ext_dualsim)
        {
            s_sim_status[card_id] = TB_SIM_NO_CARD;
        }

        SIMDRV_DisableSIMAllInt((uint8)card_index);
    }

    SIMDRV_EnableTx((uint8)card_index);
}

/**********************************************************************
//    Description:
//       decrease the system loading for midi_task can play smoothly when power on.
//    The SIM card we needed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIMHAL_DecreaseSystemLoading(uint32 cur_sim_task)
{
    if((TB_SIM_PPS_ENHANCED_SPEED == s_pps_info[cur_sim_task].pps_type)
        && ((s_sim_chip_type > SC6600L_ID_BASE) && (s_sim_chip_type < SC6600L_ID_MAX)))
    {
#if defined (MULTI_SIM_SYS_QUAD)

        if(s_sim_cmd_send_times[cur_sim_task] < 600)
        {
            s_sim_cmd_send_times[cur_sim_task]++;
            SCI_SLEEP(20);
        }

#elif defined (MULTI_SIM_SYS_TRI)

        if(s_sim_cmd_send_times[cur_sim_task] < 200)
        {
            s_sim_cmd_send_times[cur_sim_task]++;
            SCI_SLEEP(20);
        }

#elif defined (MULTI_SIM_SYS_DUAL)

        if(s_sim_cmd_send_times[cur_sim_task] < 200)
        {
            s_sim_cmd_send_times[cur_sim_task]++;
            SCI_SLEEP(10);
        }

#else

        if(s_sim_cmd_send_times[cur_sim_task] < 300)
        {
            s_sim_cmd_send_times[cur_sim_task]++;
            SCI_SLEEP(10);
        }

#endif
    }
}

/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card to start an operation on
//    The SIM card we needed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_SendInstrCode(
    SIM_CARD_INDEX_E card_id,
    TB_SIM_APDU_T *apdu_ptr    //in:the instruction code to be sent to SIM card
)
{
    uint16 i = 0;
    uint32 tx_transmit_num = 0;
    uint8 card_index = (uint8)card_id;
#if defined (SIM_SELF_TEST_SIM0_RESTORE) || defined (SIM_SELF_TEST_SIM1_RESTORE) || defined (SIM_SELF_TEST_SIM2_RESTORE)
    static uint32 send_times[3] = {0};
#endif

    SCI_ASSERT(NULL != apdu_ptr);/*assert verified */

    if(s_sim_is_ext_dualsim)
    {
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
        SIMHAL_Select_Card(card_id);
    }

    SIMDRV_ResetRxFifo(card_index);//clear Rx FIFO before send command
    SIMDRV_ResetTxFifo(card_index);//clear Tx FIFO before send command

    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    SCI_MEMSET(s_sim_rx_buf[card_index].buf, 0, TB_SIM_MAX_MSG_SIZE);
#ifdef YWD_L_TEST
    s_sim_rx_buf[card_index].next_atr_td_pos = 0xEEEE;
#endif

#ifdef SIM_ENHANCED_PPS_SUPPORT
    sim_no_ack_byte[card_index] = FALSE;
    dma_current_rx_total_size[card_index] = 0;
    sim_rx_dma_cfg[card_index].total_length = 0;
    sim_dma_tx_not_finish[card_index] = TRUE;
    s_simhal_dma_rx_received_bytes[card_index] = 0;//clear
#endif
    s_have_ack_byte[card_index] = FALSE;
    s_have_received_response_data[card_index] = FALSE;
    s_null_byte_count[card_index] = 0;
    s_sim_tx_buf[card_index].tx_data_len = TB_SIM_APDU_CHEADER_NUM;

    //copy the instruction code to the rx tx buffer
    TB_MEMCPY(s_sim_tx_buf[card_index].buf, apdu_ptr->instr_code, TB_SIM_APDU_CHEADER_NUM);

    sim_send_data_present[card_index] = FALSE;
    sim_response_data_present[card_index] = FALSE;

    //is send data exist
    if(SIMHAL_SIMINSTR_SEND_DATA == apdu_ptr->instr_type)
    {
        sim_send_data_present[card_index] = TRUE;

        //copy the data after the command code
        if(0 != apdu_ptr->instr_code[TB_SIM_APDU_P3])
        {
            s_sim_tx_buf[card_index].tx_data_len += apdu_ptr->instr_code[TB_SIM_APDU_P3];
            TB_MEMCPY((s_sim_tx_buf[card_index].buf + TB_SIM_APDU_CHEADER_NUM), apdu_ptr->instr_data, apdu_ptr->instr_code[TB_SIM_APDU_P3]);
        }
    }

    //get the expected len of the response data
    if(SIMHAL_SIMINSTR_RECV_DATA == apdu_ptr->instr_type)
    {
        sim_response_data_present[card_index] = TRUE;
        s_sim_rx_buf[card_index].rx_expected_len = apdu_ptr->rx_expected_len + TB_SIM_RESPONSE_DATA_EXPEND_LEN;
    }
    else
    {
        //the 2 byte status words.
        s_sim_rx_buf[card_index].rx_expected_len = TB_SIM_RESPONSE_DATA_EXPEND_LEN;
    }

    SIMHAL_DecreaseSystemLoading((uint32)card_id);

#ifdef SIM_DROP_RESTORE_SUPPORT
{
    SIMDRV_SetSIMClkDivisor(card_index, s_sim_clk_dvd_value[card_id]);//double check
    for(i = 0;i < 100;i++)
    {
    }
}
#endif

	SIMHAL_ActiveSIMCard((SIM_CARD_INDEX_E)card_id);

#ifdef SIM_DROP_RESTORE_SUPPORT
{
    SCI_Sleep(30);//Delay more time
}
#endif

	tx_transmit_num = SIMDRV_BeforeSendData(card_index);

#ifdef SIM_DROP_RESTORE_SUPPORT
{
    uint32 rx_fifo_count = 0;
    rx_fifo_count = SIMDRV_GetRxFifoCount(card_index);
    if (0 != rx_fifo_count)
    {
		SCI_TRACE_LOW("SIMHAL:rx fifo count is not 0");
        SCI_Sleep(30);//Delay more time
        SIMDRV_CompleteRX(card_index);
        for(i = 0;i < 100;i++)
        {
        }
        tx_transmit_num = SIMDRV_BeforeSendData(card_index);
    }
}
#endif

//This is only for SIM developer's self-test, shoule NEVER be used in release code.
//Notify: The number 100 can be adjusted by the developer. A normal sim card will send about 200 times when power on.
#ifdef SIM_SELF_TEST_SIM0_RESTORE

    if(0 == card_id)
    {
        send_times[0]++;

        if(send_times[0] >= 220)
        {
            send_times[0] = 0;
            tx_transmit_num = 1;
        }
    }

#endif
#ifdef SIM_SELF_TEST_SIM1_RESTORE

    if(1 == card_id)
    {
        send_times[1]++;

        if(send_times[1] >= 150)
        {
            send_times[1] = 0;
            tx_transmit_num = 1;
        }
    }

#endif
#ifdef SIM_SELF_TEST_SIM2_RESTORE

    if(2 == card_id)
    {
        send_times[2]++;

        if(send_times[2] >= 150)
        {
            send_times[2] = 0;
            tx_transmit_num = 1;
        }
    }

#endif

    if(tx_transmit_num > 0)
    {
        //SCI_TRACE_LOW:"SIMHAL%d::Tx fifo not empty"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3794_112_2_17_22_59_5_4452, (uint8 *)"d", card_id);
        SIMHAL_NotifyExceptionInt(card_id, TB_SIMINT_CARD_UNRESPONSE);
        return;
    }

    //#if 0//test code
    if((0x10 == s_sim_tx_buf[card_index].buf[1]) || (0x20 == s_sim_tx_buf[card_index].buf[1]))
    {
        //SCI_TRACE_LOW:"SIMHAL%d:cmd 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3805_112_2_17_22_59_5_4453, (uint8 *)"ddddddddddddddddd", card_id, s_sim_tx_buf[card_index].buf[0], s_sim_tx_buf[card_index].buf[1], s_sim_tx_buf[card_index].buf[2], \
                     s_sim_tx_buf[card_index].buf[3], s_sim_tx_buf[card_index].buf[4], s_sim_tx_buf[card_index].buf[5], s_sim_tx_buf[card_index].buf[6], s_sim_tx_buf[card_index].buf[7], s_sim_tx_buf[card_index].buf[8], \
                     s_sim_tx_buf[card_index].buf[9], s_sim_tx_buf[card_index].buf[10], s_sim_tx_buf[card_index].buf[11], s_sim_tx_buf[card_index].buf[12], s_sim_tx_buf[card_index].buf[13], s_sim_tx_buf[card_index].buf[14], \
                     s_sim_tx_buf[card_index].buf[15], s_sim_tx_buf[card_index].buf[16]);
    }
    else
    {
        //SCI_TRACE_LOW:"SIMHAL%d:cmd 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3809_112_2_17_22_59_5_4454, (uint8 *)"dddddddd", card_id, s_sim_tx_buf[card_index].buf[0], s_sim_tx_buf[card_index].buf[1], s_sim_tx_buf[card_index].buf[2], s_sim_tx_buf[card_index].buf[3], s_sim_tx_buf[card_index].buf[4], s_sim_tx_buf[card_index].buf[5], s_sim_tx_buf[card_index].buf[6]);
    }

    //#endif

#ifdef SIM_ENHANCED_PPS_SUPPORT

    if(sim_dma_support[card_id])
    {
        s_simhal_dma_rsp_time[card_index] = SIMHAL_GetWWT(card_id);
        s_simhal_dma_rsp_time[card_index] += 200;//for more compability

        if(SIMHAL_SIMINSTR_NONE_DATA == apdu_ptr->instr_type)
        {
            sim_no_ack_byte[card_index] = TRUE;
        }

#ifdef SIM_3G_USIM_SUPPORT

        if((0x80 == apdu_ptr->instr_code[TB_SIM_APDU_CLA])
            && (0xF2 == apdu_ptr->instr_code[TB_SIM_APDU_INS])
            && (0x00 == apdu_ptr->instr_code[TB_SIM_APDU_P3]))
        {
            s_sim_3g_status_check_count[card_index] = 0; 
            if(SIM_CARD_ONE == card_index)
            {
                SCI_ChangeTimer(s_sim_3g_status_check_timer[card_index], SIM1_DriverTimerExpired, SIM_3G_STATUS_CHECK_TIMER_VALUE);
            }
            else
            {
                SCI_ChangeTimer(s_sim_3g_status_check_timer[card_index], SIM2_DriverTimerExpired, SIM_3G_STATUS_CHECK_TIMER_VALUE);
            }

            SCI_ActiveTimer(s_sim_3g_status_check_timer[card_index]);
        }

#endif

        for(i = 0; i < 2; i++)
        {
            if(DMA_HAL_IsDMAPause())
            {
                //DMA is paused now
                SCI_Sleep(19);//18ms for ATV is the max
            }
            else
            {
                break;
            }
        }

        DMA_HAL_EnableDMAPause(FALSE);//Disable DMA pause

        //SIM DMA, no TX empty or RX int
        SIMDRV_DisableSIMTXEmptyInt(card_index);
        SIMDRV_DisableSIMRXInt(card_index);
        s_sim_tx_buf[card_index].cur_pos = TB_SIM_APDU_CHEADER_NUM;
        s_sim_rx_buf[card_index].is_locked = FALSE;
        sim_tx_dma_cfg[card_index].src_address = (uint32)s_sim_tx_buf[card_index].buf;
        sim_tx_dma_cfg[card_index].total_length = TB_SIM_APDU_CHEADER_NUM;
        sim_tx_dma_cfg[card_index].block_length = TB_SIM_APDU_CHEADER_NUM;
        SIMDRV_DMA_TX_Cfg(card_index, &sim_tx_dma_cfg[card_index]);
        SIMDRV_DMA_TX_Enable(card_index);
#ifdef SIM_DMA_DEBUG_TRACE_ON		
		SCI_TRACE_LOW("SIMHAL_SendInstrCode print reg after tx cmd");
    	//SIMHAL_PrintRegisterBuf(card_id);
#endif		
    }
    else//low speed sim card
    {
        //SIMDRV_EnableSIMTXEmptyInt(card_index);
        SIMDRV_EnableSIMRXInt(card_index);
        //open the lock of the sim recv data buf
        s_sim_rx_buf[card_index].is_locked = FALSE;

        for(i = 0; i < 5; i++)
        {
            SIMDRV_SendData(card_index, s_sim_tx_buf[card_index].buf[i]);
            s_sim_tx_buf[card_index].cur_pos++;
        }
    }

#else
    SIMDRV_EnableSIMRXInt(card_index);

    //open the lock of the sim recv data buf
    s_sim_rx_buf[card_index].is_locked = FALSE;

    for(i = 0; i < TB_SIM_APDU_CHEADER_NUM; i++)
    {
        SIMDRV_SendData(card_index, s_sim_tx_buf[card_index].buf[i]);
        s_sim_tx_buf[card_index].cur_pos++;
    }

#endif
}

/**********************************************************************
//    Description:
//      When the drive have receive all the response data of an operation
//    from SIM card,the drive will send the response data to manager
//    layer to process it
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendResponseUp(SIM_CARD_INDEX_E card_id)
{
    SIMHAL_MSG msg;
    SIMDRV_DATA_RESPONSE_T data_resp;
#ifdef MULTI_SIM_SYS_QUAD
    uint8 card_index = 0;
#else
    uint8 card_index = (uint8)card_id;
#endif
    TB_SIM_INT_T exp_int;

    msg.message = SIM_DATA_RESPONSE;
    msg.wparam  = card_id;
    msg.lparam  = &data_resp;
    data_resp.data_len = 0;

    if(s_sim_is_ext_dualsim)
    {
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

        if(TB_SIM_1_8V_DETECT == s_sim_status[card_id])
        {
            msg.wparam = s_simhal_current_sim_task_id;
        }
    }

    //the response is a instruction response,ref to ISO 7816 part-3 "procedure bytes sent by the card"
    if((TB_SIM_1_8V_DETECT == s_sim_status[card_id]) || (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]))
    {
#if 0
        //SCI_TRACE_LOW:"SIMHAL%d: status words 0x%02x,0x%02x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_3948_112_2_17_22_59_6_4455, (uint8 *)"ddd", card_id, s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2], s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1]);
#endif

        //there are only two byte response data from the SIM card,
        //the data is start with the SW1 and the seconde byte is the SW2
        if(2 == s_sim_rx_buf[card_index].cur_pos)
        {
            //the staus word of the response of instruciton
            data_resp.sw1 = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2];
            data_resp.sw2 = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1];
        }
        //there are more that 2 byte response data from the SIM card
        else if(2 < s_sim_rx_buf[card_index].cur_pos)
        {
            //delete the not really response data from the receive buf
            data_resp.data_len = s_sim_rx_buf[card_index].cur_pos - 2;
#ifdef SIM_ENHANCED_PPS_SUPPORT

            if(sim_dma_support[card_id] && sim_response_data_present[card_index] && (s_sim_rx_buf[card_index].rx_expected_len > TB_SIM_RESPONSE_DATA_EXPEND_LEN))
            {
                // The procedure byte come with the real sim data.
                // Now in the rx buf, the data is arranged as below:
                // ack data1 data2 .....data(rx_expected_len-2+1) status1 status2
                // So we need to re-arrange the rx buf (remove the ACK byte).
                // But the re-arrange will waste too many time in a DMA INT, so we will
                // notify upper layer with the data pointer to "data1" directly in SendResponseUp.
                data_resp.data = &(s_sim_rx_buf[card_index].buf[1]);
                data_resp.sw1 = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1];
                data_resp.sw2 = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos];
            }
            else
#endif
            {
                data_resp.data = s_sim_rx_buf[card_index].buf;
                data_resp.sw1 = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2];
                data_resp.sw2 = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1];
            }
        }
        else
        {
            msg.message = SIM_EXCEPTION_INT;
            msg.wparam = card_id;
            exp_int.int_type = TB_SIMINT_CARD_UNRESPONSE;
            msg.lparam = &exp_int;
        }
    }
    //the response data is a ATR data or PPS data
    else
    {
        data_resp.data_len = s_sim_rx_buf[card_index].cur_pos;
        data_resp.data = s_sim_rx_buf[card_index].buf;
    }

    DriverNotifySIM(&msg);
#ifdef SIM_ENHANCED_PPS_SUPPORT
    DMA_HAL_EnableDMAPause(TRUE);//enable DMA pause
#endif
    //clear the RX buf
    ClearRxBuf(card_id);
    SIMDRV_CompleteRX(card_index);
}

/**********************************************************************
//    Description:
//      When the ATR data show the SIM card need a PPS procedure before
//    actived,Use this function to send the PPS procedure data to the
//    SIM card to start the PPS procedure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendPPSRequest(
    SIM_CARD_INDEX_E card_id,
    const uint8 *pps_req, //in:the ppas request data to be send
    uint16 req_len  //the data len
)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    ClearTxBuf(card_id);

    s_sim_tx_buf[card_index].tx_data_len = req_len;
    TB_MEMCPY(s_sim_tx_buf[card_index].buf, pps_req, req_len);

    //open the lock of the recv data buf.
    s_sim_rx_buf[card_index].is_locked = FALSE;

    //transmit the data for the first time
    TransmitInInt(card_id);
}

/**********************************************************************
//    Description:
//      Set the working voltage of the sim card.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SetSIMVoltage(SIM_CARD_INDEX_E card_id, SIMDRV_WORKING_VOLT_E work_vol)
{
    uint8 card_index = (uint8)card_id;
    SIMDRV_WORKING_VOLT_E volt = work_vol;
#ifdef SIM_6302_SWITCH
    uint8 i = 0;
    SIM_6302_OUTPUT_E is_6302_connect = SIM_6302_CARD_ID_NOT_CONNECT;
#endif

    card_index = SIMHAL_GetChipSimIndex(card_id, TRUE);

    if(s_simhal_has_1_8v_only[card_id])
    {
        volt = SIMDRV_WORKING_VOLT_1_8V;
    }

#ifdef SIM_6302_SWITCH
    is_6302_connect = SIMHAL_6302_Get_Output_Index(card_id);

    if(SIM_6302_CARD_ID_NOT_CONNECT != is_6302_connect)
    {
        volt = SIMDRV_WORKING_VOLT_3V;//sim card_id is connected to 6302.
    }

#endif

    SCI_TRACE_LOW("SIMHAL%d:set SIM voltage volt=%d.",card_id,volt);
    SIMDRV_SetSIMVoltage((uint8)card_index, volt);

#ifdef SIM_6302_SWITCH

    if(SIM_6302_CARD_ID_NOT_CONNECT != is_6302_connect)
    {
        for(i = 0; i < 100; i++) //the LDO need 125us to be stable
            {}

        //SCI_TRACE_LOW:"SIMHAL%d: set volt %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4233_112_2_17_22_59_6_4457, (uint8 *)"dd", card_id, work_vol);
        //SPI to set SIM card1,2 volt
        SIMHAL_6302_SetVsim(card_id, FALSE, work_vol);
    }

#endif
}

/**********************************************************************
//    Description:
//      Make the SIM card to the Idle state,stop the clock of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_IdleSIMCard(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;
    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

#ifdef SIM_6302_SWITCH

    if(s_sim_6302_switch_flag)
    {
        SIM_6302_OUTPUT_E sim_6302_index = SIMHAL_6302_Get_Output_Index(card_id);

        if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
        {
            s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;
            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
        }
    }

#endif

    SIMDRV_DisableSIMCLK((uint8)card_index);
}

/**********************************************************************
//    Description:
//      Reset the SIM card with a new working voltage when the SIM card
//    is not working well at the beignning,but there are reset times limit,
//    if the rest time exeed the times limit,the SIM card will be Rejected
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN ResetSIMCard(    //return value:the SIM card reset succeed or not.
    BOOLEAN             is_1_8v_detect,
    SIM_CARD_INDEX_E    card_id,
    TB_SIM_RESET_TYPE_E reset_type
)
{
    uint8 card_index = (uint8)card_id;

    //SCI_TRACE_LOW:"SIMHAL%d::Reset the SIM card!%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4349_112_2_17_22_59_6_4458, (uint8 *)"dd", card_id, s_card_resetting[card_id]);

    if(s_card_resetting[card_id])
    {
        return TRUE;
    }

    s_card_resetting[card_id] = TRUE;

    if(s_sim_is_ext_dualsim)
    {
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
        SIMHAL_Select_Card(card_id);//card select
    }

    //deactive these timer to avoid the timer expiration cause SIM card Reset nest
    SCI_DeactiveTimer(s_sim_pps_timer[card_index]);
    SCI_DeactiveTimer(s_sim_reset_p_timer[card_index]);
    SCI_DeactiveTimer(s_sim_reset_bt_timer[card_index]);
    SCI_DeactiveTimer(s_sim_atr_timer[card_index]);
    SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);
    SCI_DeactiveTimer(s_sim_tb_te_timer[card_index]);

    if(SIM_CARD_ONE == card_index)
    {
        SCI_ChangeTimer(s_sim_atr_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_ATR_TIMER_VALUE);
    }
    else
    {
        SCI_ChangeTimer(s_sim_atr_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_ATR_TIMER_VALUE);
    }

    //the SIM card Transfer protocal _protocol
    s_not_t0_protocol_occurs[card_id] = FALSE;
    s_first_protocol_occurs[card_id] = FALSE;
    //If PPS failure, we will RESET again without PPS.
    //So, keep the s_previous_pps_error value, don't clear.wuding.yang
#ifdef SIM_ENHANCED_PPS_SUPPORT//ywd 2007.2.27
    sim_dma_support[card_id] = FALSE;
#endif
    s_simhal_rx_noise_data[card_id] = 0;
    //initialize the buffer
    s_sim_atr_data_len[card_id] = 0;
    s_have_ack_byte[card_index] = FALSE;
    s_null_byte_count[card_index] = 0;

    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearATRData(card_id);
    ClearPPSInfo(card_id);
    s_sim_rx_buf[card_index].is_locked = FALSE;

    //the SIM card working status
    if(is_1_8v_detect)
    {
        s_sim_status[card_id] = TB_SIM_1_8V_DETECT;
    }
    else
    {
        s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;
    }

    SIMDRV_ClearBeforeReset(card_index);
    SIMHAL_SetClockStopLevel(card_id, FALSE);//clear
    //SIMHAL_RejectSIMCard(card_id);
    /*SIMDRV_DisableSIMAllInt(card_index);*/
#ifdef SIM_ENHANCED_PPS_SUPPORT
    s_sim_clk_mode[card_id] = SIMDRV_ConfigSIMClk(card_index);

    s_sim_clk_dvd_value[card_id] = 0xba0;//set as the default value.
#endif

    SCI_TRACE_LOW("SIMHAL%d:Set chip RST LOW.",card_id);
    SIMDRV_SetSIMRST(card_index, FALSE);//Low the RST pike in 1.8-->3V

    //do the different operation between differecnt type restet operation
    switch(reset_type)
    {
        case TB_SIM_RESET_NO_CHANGE:
            //SCI_TRACE_LOW:"SIMHAL::Reset first time!%d"
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4429_112_2_17_22_59_7_4459, (uint8 *)"d", s_sim_reset_times[card_id]);
            SCI_TRACE_LOW("SIMHAL%d:Reset first time!%d",card_id,s_sim_reset_times[card_id]);
            
            //check reset times
            if(s_sim_reset_times[card_id] > s_sim_max_reset_times)
            {
                SIMHAL_RejectSIMCard(card_id);
                s_card_resetting[card_id] = FALSE;
                return FALSE;
            }

            s_sim_reset_times[card_id]++;
            break;
        case TB_SIM_RESET_FOR_POWER:

            if(s_sim_pow_reset_times[card_id] > TB_SIM_MAX_POWER_RESET_TIMES)
            {
                s_sim_pow_reset_times[card_id] = 0;
            }

            if(s_sim_pow_reset_times[card_id] == 0)
            {
                //chage the power on the sim card
                if(SIMDRV_WORKING_VOLT_1_8V == s_sim_working_volt[card_id])
                {
                    //SCI_TRACE_LOW:"SIMHAL%d::Reset to change power."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4450_112_2_17_22_59_7_4460, (uint8 *)"d", card_id);
                    SIMHAL_SetSIMVoltage(card_id, SIMDRV_WORKING_VOLT_3V);
                    s_sim_working_volt[card_id] = SIMDRV_WORKING_VOLT_3V;

                    if(SIM_CARD_ONE == card_index)
                    {
                        SCI_ChangeTimer(s_sim_reset_wait_ldo_timer[SIM_CARD_ONE], SIM1_ResetAfterLdoValid, s_sim_ldo_stable_time);
                        SCI_ActiveTimer(s_sim_reset_wait_ldo_timer[SIM_CARD_ONE]);
                    }
                    else
                    {
                        SCI_ChangeTimer(s_sim_reset_wait_ldo_timer[SIM_CARD_TWO], SIM2_ResetAfterLdoValid, s_sim_ldo_stable_time);
                        SCI_ActiveTimer(s_sim_reset_wait_ldo_timer[SIM_CARD_TWO]);
                    }

                    s_sim_pow_reset_times[card_id]++;
                    return TRUE;
                }
                else
                {
                    //SCI_TRACE_LOW:"SIMHAL%d::Reject for no next available volt. %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4468_112_2_17_22_59_7_4461, (uint8 *)"dd", card_id, s_sim_working_volt[card_id]);
                    SIMHAL_RejectSIMCard(card_id);
                    s_card_resetting[card_id] = FALSE;
                    return FALSE;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"SIMHAL%d::Reset %d time for power change."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4476_112_2_17_22_59_7_4462, (uint8 *)"dd", card_id, s_sim_pow_reset_times[card_id]);
            }

            s_sim_pow_reset_times[card_id]++;
            break;
        case TB_SIM_RESET_FOR_BT:
            //SCI_TRACE_LOW:"SIMHAL%d::Reset to change bit convention! %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4481_112_2_17_22_59_7_4463, (uint8 *)"dd", card_id, s_sim_b_reset_times[card_id]);

            //check reset times
            if(s_sim_b_reset_times[card_id] > TB_SIM_MAX_B_RESET_TIMES)
            {
                SIMHAL_RejectSIMCard(card_id);
                s_card_resetting[card_id] = FALSE;
                return FALSE;
            }

            s_sim_b_reset_times[card_id]++;

            //change the bit convetion and the logic level
            SIMHAL_ChangeConvetion(card_id);
            break;
        case TB_SIM_RESET_FOR_PARITY:
            //SCI_TRACE_LOW:"SIMHAL%d::Reset to change parity check! %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4495_112_2_17_22_59_7_4464, (uint8 *)"dd", card_id, s_sim_p_reset_times[card_id]);

            if(s_sim_p_reset_times[card_id] > TB_SIM_MAX_B_RESET_TIMES)
            {
                SIMHAL_RejectSIMCard(card_id);
                s_card_resetting[card_id] = FALSE;
                return FALSE;
            }

            s_sim_p_reset_times[card_id]++;

            s_sim_p_reset_happen_flag[card_id] = TRUE;
            
            //change the parity checking setting
            SIMDRV_ChangeParity(card_index);
            break;
        default:
            s_card_resetting[card_id] = FALSE;
            //SCI_TRACE_LOW:"ResetSIMCard reset_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4509_112_2_17_22_59_7_4465, (uint8 *)"d", reset_type);
            return FALSE;
    }

#ifdef SIM_6302_SWITCH

    if(s_sim_6302_switch_flag)
    {
        SIM_6302_OUTPUT_E sim_6302_output_index = SIMHAL_6302_Get_Output_Index(card_id);

        if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_output_index)
        {
            if(SIM_6302_OUTPUT_0 == sim_6302_output_index)
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_2;//power on
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] |= BIT_0;//CLK the same ad BB
                s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_0;//DATA the same ad BB
            }
            else
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_3;//power on
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] |= BIT_1;//CLK the same ad BB
                s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_1;//DATA the same ad BB
            }

            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_VSIM, s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM]);

            SCI_TRACE_LOW("SIMHAL%d:Enable chip SIM CLK.",card_id);
            
            SIMDRV_EnableSIMCLK(card_index);//we should enable clk before channel switch in uad-sim via 6302.

            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
        }
    }

#endif

    {
        SIMDRV_StartToReset(card_index);
    }

    //Start a Timer here, to keep a Tb/Te interval's SIM_RST LOW after SIM_Vcc/CLK on.
    if(SIM_CARD_ONE == card_index)
    {
        SCI_ChangeTimer(s_sim_tb_te_timer[SIM_CARD_ONE], SIM1_RST_After_Tb_or_Te, SIM_RST_TB_TE_TIMER_VALUE);
        SCI_ActiveTimer(s_sim_tb_te_timer[SIM_CARD_ONE]);
    }
    else
    {
        SCI_ChangeTimer(s_sim_tb_te_timer[SIM_CARD_TWO], SIM2_RST_After_Tb_or_Te, SIM_RST_TB_TE_TIMER_VALUE);
        SCI_ActiveTimer(s_sim_tb_te_timer[SIM_CARD_TWO]);
    }

    return TRUE;
}

/**********************************************************************
//    Description:
//      Reject the SIM card(deactive the SIM card and Power off the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_RejectSIMCard(SIM_CARD_INDEX_E card_id)
{
    uint8 i = 0;
    SIM_CARD_INDEX_E card_index = card_id;
    SIM_CARD_INDEX_E current_sel_card = card_id;

    if(s_sim_is_ext_dualsim)
    {
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);
        SIMHAL_Select_Card(card_id);
        current_sel_card = card_id;
    }

    //SCI_TRACE_LOW("SIMHAL%d:RejectSIMCard.",simhal_current_sel_card);

#ifdef SIM_ENHANCED_PPS_SUPPORT

    if(sim_dma_support[card_id])
    {
        SIMDRV_DMA_RX_Disable(card_index);
        SIMDRV_DMA_TX_Disable(card_index);
    }

#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
    if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
    {
        SIMDRV_DisableSIMAllInt(card_index);//Stop ASAP
        SIMDRV_DisableRx(card_index);
        SIMDRV_DisableTx(card_index);

        SIMDRV_ResetRxFifo(card_index);//A Brasil SIM

        SIMDRV_SetSIMRST(card_index, FALSE);

        for(i = 0; i < 100; i++)
        {
        }

        SIMHAL_SetClockStopLevel(card_id, FALSE);

        for(i = 0; i < 100; i++)
        {
        }

        SIMDRV_DisableSIMCLK(card_index);

        for(i = 0; i < 100; i++)
        {
        }

        SIMDRV_Set_IO_State(card_index, FALSE);

        for(i = 0; i < 100; i++)
        {
        }

#ifdef SIM_6302_SWITCH

        if(s_sim_6302_switch_flag)
        {
            SIM_6302_OUTPUT_E sim_6302_output_index = SIMHAL_6302_Get_Output_Index(card_id);

            if(SIM_6302_CARD_ID_NOT_CONNECT == sim_6302_output_index)
            {
                SIMDRV_DisableSIMLDO(card_id);
            }
            else//sims are connected to 6302
            {
                //we use "simhal_current_sel_card" instead of "card_id" for slot_exchange.
                if(SIM_6302_OUTPUT_0 == sim_6302_output_index)
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] &= ~(BIT_2 | BIT_0);
                }
                else
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] &= ~(BIT_3 | BIT_1);
                }

                SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_VSIM, s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM]);
            }
        }

#else
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, TRUE);
        SIMDRV_DisableSIMLDO(card_index);
        card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE); //recover
#endif
    }

    //Get the current SIM ctl0 value.
    s_sim_ctl0_value[current_sel_card] = SIMDRV_GetCTL0Value(card_index);
    //SCI_TRACE_LOW:"SIMHAL%d:RejectSIMCard 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_4950_112_2_17_22_59_8_4466, (uint8 *)"dd", current_sel_card, s_sim_ctl0_value[current_sel_card]);
}

/**********************************************************************
//    Description:
//      Active the SIM card by start clock
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SIMHAL_ActiveSIMCard(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    SIMDRV_EnableSIMCLK(card_index);//we should enable clk before channel switch in tri-sim via 6302.

#ifdef SIM_6302_SWITCH

    if(s_sim_6302_switch_flag)
    {
        SIM_6302_OUTPUT_E sim_6302_output_index = SIMHAL_6302_Get_Output_Index(card_id);

        if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_output_index)
        {
            if(SIM_6302_OUTPUT_0 == sim_6302_output_index)
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] &= ~BIT_2;
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] |= BIT_0;
            }
            else
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] &= ~BIT_3;
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] |= BIT_1;
            }

            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
        }
    }

#endif

    SCI_Sleep(2);
}

/**********************************************************************
//    Description:
//      Send a byte data of TX buf to the SIM card,not the all left TX
//    buf data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN SendOneByteToSIM(SIM_CARD_INDEX_E card_id)   //return value:TRUE:all the TX have been transmit to SIM
//FALSE:there still exist TX data need to be transmit to the SIM
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    //there still some instruction data to be sent
    if(s_sim_tx_buf[card_index].tx_data_len > s_sim_tx_buf[card_index].cur_pos)
    {
        SIMDRV_EnableTx(card_index);//enable the tx
        SIMDRV_SendData(card_index, (uint32)(s_sim_tx_buf[card_index].buf[s_sim_tx_buf[card_index].cur_pos]));
        s_sim_tx_buf[card_index].cur_pos++;

        //all the data of the tx buf has been sent to the SIM card.
        if(s_sim_tx_buf[card_index].cur_pos >= s_sim_tx_buf[card_index].tx_data_len)
        {
            ClearTxBuf(card_id);
            return TRUE;
        }
    }
    else
    {
        ClearTxBuf(card_id);
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************
//    Description:
//      the really function taht do the  data transmit to the SIM CARD
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TransmitInInt(SIM_CARD_INDEX_E card_id)
{
    uint16 tx_transmit_num, i;
    uint8 card_index = (uint8)card_id;
    uint16 tx_buf_size = 0;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    tx_buf_size = SIMDRV_GetFIFOBufSize(card_index);

    //disable the TX empty interrupt to make sure the data transmit not be broken by
    //tx data interrupt in the data transform process
    SIMDRV_DisableSIMTXEmptyInt(card_index);

    //get the current fifo free space
    tx_transmit_num = SIMDRV_GetTxFifoCount(card_index);
    tx_transmit_num =  tx_buf_size - tx_transmit_num;

    //enable the tx
    SIMDRV_EnableTx(card_index);

    //there still some instruction data to be sent
    if(s_sim_tx_buf[card_index].tx_data_len > s_sim_tx_buf[card_index].cur_pos)
    {
        i = s_sim_tx_buf[card_index].tx_data_len - s_sim_tx_buf[card_index].cur_pos;

        //all the transmit data will can be transmited just one time
        if(i <= tx_transmit_num)
        {
            tx_transmit_num = i;
        }

        //fill the fifo buf by the unsent instuction data
        for(i = 0; i < tx_transmit_num; i++)
        {
            SIMDRV_SendData(card_index, (uint32)(s_sim_tx_buf[card_index].buf[s_sim_tx_buf[card_index].cur_pos]));
            s_sim_tx_buf[card_index].cur_pos++;
        }

        //enable the TX to spring the next data transmit
        SIMDRV_EnableSIMTXEmptyInt(card_index);
    }
    //all the command instruciton has been transmit
    else
    {
        //clear the tx buf
        ClearTxBuf(card_id);
    }
}

/**********************************************************************
//    Description:
//      check the Received buf and sent all received data up to Task
//    when all the data from SIM Card has been received
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CheckReceiveBuf(SIM_CARD_INDEX_E card_id)
{
#ifdef MULTI_SIM_SYS_TRI
    BOOLEAN is_fisrt_t15_occur[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#elif defined (MULTI_SIM_SYS_QUAD)
    BOOLEAN is_fisrt_t15_occur[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#else
    BOOLEAN is_fisrt_t15_occur[SIM_CARD_NUM] = {FALSE, FALSE};
#endif
    uint16 i, water_mark;
    uint8 card_index = (uint8)card_id;
    uint32 wait_time = 0;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    switch(s_sim_status[card_id])
    {
            //after the power is on reiceve the start of the ATR character
        case TB_SIM_WAITING_FOR_ATR:
            SCI_DeactiveTimer(s_sim_atr_timer[card_index]);

            //the TS character must be a valide TS value
            if((TB_SIM_ATR_TS_VALUE1 == s_sim_rx_buf[card_index].buf[0]) ||
                (TB_SIM_ATR_TS_VALUE2 == s_sim_rx_buf[card_index].buf[0]))
            {
                s_sim_rx_buf[card_index].rx_expected_len = TB_SIM_ATR_ALWAYS_CHARACTER_NUM;
                s_sim_status[card_id] = TB_SIM_PROCESSING_ATR;
                s_sim_rx_buf[card_index].check_pos++;
                s_sim_rx_buf[card_index].next_atr_td_pos = s_sim_rx_buf[card_index].check_pos;
                //Start a timer to protect a special SIM card cannot send out the last byte
                SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);

                if(SIM_CARD_ONE == card_index)
                {
                    SCI_ChangeTimer(s_sim_decode_atr_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_WAIT_DECODE_ATR_TIMER_VALUE);
                }
                else
                {
                    SCI_ChangeTimer(s_sim_decode_atr_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_WAIT_DECODE_ATR_TIMER_VALUE);
                }

                SCI_ActiveTimer(s_sim_decode_atr_timer[card_index]);
                goto PROCESS_ATR_CHARACTOR;
            }
            else
            {
                SCI_DeactiveTimer(s_sim_reset_p_timer[card_index]);

                //use timer to do the reset operate
                if(!SCI_IsTimerActive(s_sim_reset_bt_timer[card_index]))
                {
                    if(SIM_CARD_ONE == card_index)
                    {
                        SCI_ChangeTimer(s_sim_reset_bt_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_RESET_TIMER_VALUE);
                    }
                    else
                    {
                        SCI_ChangeTimer(s_sim_reset_bt_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_RESET_TIMER_VALUE);
                    }

                    SCI_ActiveTimer(s_sim_reset_bt_timer[card_index]);
                }

                ClearRxBuf(card_id);
                SIMDRV_DisableRx(card_index);
                SIMDRV_DisableSIMAllInt(card_index);
            }

            break;
            //receive the ATR procedure response
        case TB_SIM_PROCESSING_ATR:
PROCESS_ATR_CHARACTOR:

            //get the total ATR data len acording the received ATR data
            while(s_sim_rx_buf[card_index].cur_pos > s_sim_rx_buf[card_index].check_pos)
            {
                if(s_sim_rx_buf[card_index].check_pos == s_sim_rx_buf[card_index].next_atr_td_pos)
                {
                    i = 0;

                    //the s_sim_rx_buf.check_pos point character is T0
                    if(TB_SIM_ATR_T0_POS == s_sim_rx_buf[card_index].check_pos)
                    {
                        //add the history character number
                        s_sim_rx_buf[card_index].rx_expected_len +=
                            s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_HISTORY_CHARACTER_MARK;
                    }
                    else
                    {
                        //if the first protocol has not occurs,the first protocol will be the used protocol
                        if(!s_first_protocol_occurs[card_id])
                        {
                            s_atr_data[card_id].t_protocol = s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_T_PROTOCAL_MARK;
                            s_first_protocol_occurs[card_id] = TRUE;
                        }

                        //if the not t0 protocal has not occurs
                        if(!s_not_t0_protocol_occurs[card_id])
                        {
                            if((s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_T_PROTOCAL_MARK) != TB_SIM_T_0_PROTOCOL)
                            {
                                s_sim_rx_buf[card_index].rx_expected_len++;
                                s_not_t0_protocol_occurs[card_id] = TRUE;
                            }
                        }

                        if(s_sim_atr_interface_bytes_times[card_id] > 2)
                        {
                            //SCI_TRACE_LOW("SIMHAL: td is 0x%x ",s_sim_rx_buf.buf[s_sim_rx_buf.check_pos]);
                            if(0xf == (s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_T_PROTOCAL_MARK))
                            {
                                is_fisrt_t15_occur[card_id] = TRUE;
                            }
                        }
                    }

                    //next TA exist
                    if(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_TA_CHARACTER_MARK)
                    {
                        i++;

                        //the s_sim_rx_buf.check_pos point character is T0
                        if(TB_SIM_ATR_T0_POS == s_sim_rx_buf[card_index].check_pos)
                        {
                            s_atr_data[card_id].TA1_pos = s_sim_rx_buf[card_index].check_pos + i;
                        }
                        else
                        {
                            if(is_fisrt_t15_occur[card_id])
                            {
                                s_sim_atr_ta_with_volt_position[card_id] = s_sim_rx_buf[card_index].check_pos + i;
                            }
                        }
                    }

                    if(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_TB_CHARACTER_MARK)
                    {
                        i++;

                        //the s_sim_rx_buf.check_pos point character is T0
                        if(TB_SIM_ATR_T0_POS == s_sim_rx_buf[card_index].check_pos)
                        {
                            s_atr_data[card_id].TB1_pos = s_sim_rx_buf[card_index].check_pos + i;
                        }
                    }

                    if(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_TC_CHARACTER_MARK)
                    {
                        i++;

                        //the s_sim_rx_buf.check_pos point character is T0
                        if(TB_SIM_ATR_T0_POS == s_sim_rx_buf[card_index].check_pos)
                        {
                            s_atr_data[card_id].TC1_pos = s_sim_rx_buf[card_index].check_pos + i;
                        }
                        else if(2 == s_sim_atr_interface_bytes_times[card_id])
                        {
                            s_atr_data[card_id].TC2_pos = s_sim_rx_buf[card_index].check_pos + i;
                        }
                    }

                    //the TD(i) decide the next
                    if(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] & TB_SIM_ATR_TD_CHARACTER_MARK)
                    {
                        i++;
                        s_sim_rx_buf[card_index].next_atr_td_pos += i;
                        s_sim_atr_interface_bytes_times[card_id]++;
                    }

                    //add the length of the data
                    s_sim_rx_buf[card_index].rx_expected_len += i;
                }

                //the TA1
                if(s_sim_rx_buf[card_index].check_pos == s_atr_data[card_id].TA1_pos)
                {
                    //2003-10-9 added by reaistlin.kong
                    //support the enhanced PPS sim card
                    switch(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos])
                    {
                        case TB_SIM_ATR_DEFAULT_A1_1:
                        case TB_SIM_ATR_DEFAULT_A1_2:
                            break;
                            //define SIM_ENHANCED_PPS_SUPPORT macro to supprot the enhanced pps
                        default:
#ifdef SIM_ENHANCED_PPS_SUPPORT
                            //SCI_TRACE_LOW:"SIMHAL%d:high TA1 0x%x"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_5300_112_2_17_22_59_8_4467, (uint8 *)"dd", card_id, s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos]);
                            SIMHAL_SetPPS(card_id, s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos]);
#else
                            s_pps_info[card_id].is_current = TRUE;
                            s_pps_info[card_id].pps_data = s_default_pps_data;
                            s_pps_info[card_id].pps_type = TB_SIM_PPS_DEFAULT_SPEED;
#endif
                            break;
                    }

                    //2003-10-9 end raistlin.kong change
                }

                //the TB1
                if(s_sim_rx_buf[card_index].check_pos == s_atr_data[card_id].TB1_pos)
                {
#if 0
                    //should reject the SIM card soon;
                    if(0 != (TB_SIM_ATR_Pl1_MARK & s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos]))
                    {
                        SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);

                        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_NO_CHANGE))
                        {

                            ReportCardStatus(card_id, TB_SIM_INSERTED_WRONG_SIM);

                        }

                        s_sim_atr_ta_with_volt_position[card_id] = 0;
                        s_sim_atr_interface_bytes_times[card_id] = 1;
                        return;
                    }
#endif
                }

                //the TC1
                if(s_sim_rx_buf[card_index].check_pos == s_atr_data[card_id].TC1_pos)
                {
                    if((TB_SIM_ATR_TC1_VALUE1 != s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos])
                        && (TB_SIM_ATR_TC1_VALUE2 != s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos]))
                    {
                        SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);

                        if(!ResetSIMCard(FALSE, card_id, TB_SIM_RESET_NO_CHANGE))
                        {
                            SIMHAL_RejectSIMCard(card_id);

                            ReportCardStatus(card_id, TB_SIM_INSERTED_WRONG_SIM);

                        }

                        s_sim_atr_ta_with_volt_position[card_id] = 0;
                        s_sim_atr_interface_bytes_times[card_id] = 1;
                        return;
                    }
                }

                //the TC2
                if(s_sim_rx_buf[card_index].check_pos == s_atr_data[card_id].TC2_pos)
                {
                    if(0 != s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos])
                    {
                        s_sim_atr_wi[card_id] = (uint32)s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos];

                        if(s_sim_atr_wi[card_id] > 10)
                        {
                            s_sim_atr_wi[card_id] = 10;//9600etu is ok
                        }
                    }

                    //SCI_TRACE_LOW("SIMHAL%d: atr TC2=0x%x, %d",card_id,s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos],s_sim_rx_buf[card_index].check_pos);
                }

                //add the start pos
                s_sim_rx_buf[card_index].check_pos++;
            }

            //all the ATR data has been received,or the ATR data has exceed the
            if(s_sim_rx_buf[card_index].cur_pos == s_sim_rx_buf[card_index].rx_expected_len)
            {
                //save the atr data into the s_sim_atr_data_buf
                SCI_ASSERT(s_sim_rx_buf[card_index].cur_pos < 50);/*assert verified:Check_value */

                if(SCI_IsTimerActive(s_sim_decode_atr_timer[card_index]))
                {
                    SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);
                }

                s_sim_atr_data_len[card_id] = s_sim_rx_buf[card_index].cur_pos;
                SCI_MEMCPY(s_sim_atr_data_buf[card_id], s_sim_rx_buf[card_index].buf, s_sim_rx_buf[card_index].cur_pos);

                if(0 != s_sim_atr_ta_with_volt_position[card_id])
                {
                    //You should use a SIM card whose ATR has "ta_with_volt_position"...
#ifdef SIM0_SELF_TEST_ATR_1_8V_INFO
                    if(0 == simhal_current_sel_card)
                    {
                        s_sim_rx_buf[card_index].buf[s_sim_atr_ta_with_volt_position[card_id]] = 0x4;
                    }

#endif
#ifdef SIM1_SELF_TEST_ATR_1_8V_INFO

                    if(1 == simhal_current_sel_card)
                    {
                        s_sim_rx_buf[card_index].buf[s_sim_atr_ta_with_volt_position[card_id]] = 0x4;
                    }

#endif
#ifdef SIM2_SELF_TEST_ATR_1_8V_INFO

                    if(2 == simhal_current_sel_card)
                    {
                        s_sim_rx_buf[card_index].buf[s_sim_atr_ta_with_volt_position[card_id]] = 0x4;
                    }

#endif

                    if(0x4 == (0x7 & s_sim_rx_buf[card_index].buf[s_sim_atr_ta_with_volt_position[card_id]]))
                    {
                        //only bit2 is 1, this is a 1.8V only SIM.
                        SIMHAL_SetVoltLimit(card_id, TRUE);
                        SIMHAL_SetVoltDetectComplete(card_id, TRUE);
                    }

                    //bits 8 and 7 indicate whether the card supports clock stop (!= 00) or not (= 00) and,
                    //when supported, which state is preferred on the electrical circuit CLK when the clock
                    //is stopped.
                    if(0x80 == (0xc0 & s_sim_rx_buf[card_index].buf[s_sim_atr_ta_with_volt_position[card_id]]))
                    {
                        SIMHAL_SetClockStopLevel(card_id, TRUE);
                    }
                    else
                    {
                        SIMHAL_SetClockStopLevel(card_id, FALSE);
                    }

                    SIMHAL_SetATRVoltInfo(card_id, s_sim_rx_buf[card_index].buf[s_sim_atr_ta_with_volt_position[card_id]]);
                    //SCI_TRACE_LOW:"SIMHAL%d:atr 1.8V only is %d, 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_5404_112_2_17_22_59_8_4468, (uint8 *)"ddd", card_id, s_simhal_has_1_8v_only[card_id], s_sim_rx_buf[card_index].buf[s_sim_atr_ta_with_volt_position[card_id]]);
                    s_sim_atr_ta_with_volt_position[card_id] = 0;
                    s_sim_atr_interface_bytes_times[card_id] = 1;
                }

                ClearRxBuf(card_id);//lock the recv data buf,to make sure there no more data coming

                if(SIMDRV_IsSIMRSTHigh(card_index))
                {
                    SIMDRV_SetSIMRST(card_index, TRUE);
                }

                // refer to GSM 11.11 5.8.3 , If the SIM does not answer the PPS request within the initial waiting time
                //the ME shall reset the SIM. After two failed PPS attempts using F=512 and D=8 or values indicated in TA1,
                //(no PPS response from the SIM) the ME shall initiate PPS procedure using default values.
                //If this also fails (no PPS response from the SIM) the ME may proceed using default values without requesting PPS.
                if((s_pps_info[card_id].is_current) && (!s_previous_pps_error[card_id]))
                {
                    if(s_sim_pps_req_times[card_id] > 1)
                    {
                        s_pps_info[card_id].is_current = TRUE;
                        s_pps_info[card_id].pps_data = s_default_pps_data;
                        s_pps_info[card_id].pps_type = TB_SIM_PPS_DEFAULT_SPEED;
                    }

                    s_sim_status[card_id] = TB_SIM_PROCESSING_PPS;
                    SendPPSRequest(card_id, s_pps_info[card_id].pps_data, (uint16)s_pps_info[card_id].pps_type);

                    if(SIM_CARD_ONE == card_index)
                    {
                        SCI_ChangeTimer(s_sim_pps_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_WAIT_PPS_TIMER_VALUE);
                    }
                    else
                    {
                        SCI_ChangeTimer(s_sim_pps_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_WAIT_PPS_TIMER_VALUE);
                    }

                    SCI_ActiveTimer(s_sim_pps_timer[card_index]);
#ifdef SIM_ENHANCED_PPS_SUPPORT
                    //SCI_TRACE_LOW:"SIMHAL%d::PPS to sim 0x%x 0x%x 0x%x 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_5441_112_2_17_22_59_9_4469, (uint8 *)"ddddd", card_id, *(s_pps_info[card_id].pps_data), *(s_pps_info[card_id].pps_data + 1), *(s_pps_info[card_id].pps_data + 2), *(s_pps_info[card_id].pps_data + 3));
#endif
                }
                else
                {
#ifdef SIM_ENHANCED_PPS_SUPPORT
                    //If PPS fail last time (s_previous_pps_error), we should clear the F and D as the default value.
                    sim_F[card_id] = SIM_FACTOR_F_372;
                    sim_D[card_id] = SIM_FACTOR_D_1;
#endif
                    s_sim_status[card_id] = TB_SIM_RECEIVING_RESPONSE;

                    ReportCardStatus(card_id, TB_SIM_INSERTED_OK);

                    if(s_sim_is_ext_dualsim)
                    {
                        s_sim_ctl0_value[card_id] = SIMDRV_GetCTL0Value(card_index);
                    }
                }

                SIMHAL_SetWWT(card_id);//After decode the ATR, set the WWT

                //SCI_TRACE_LOW:"SIMHAL%d::enable the seh function for b5 and higher version chip!0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_5463_112_2_17_22_59_9_4470, (uint8 *)"dd", card_id, s_sim_ctl0_value[card_id]);
                SIMDRV_DisableRetransmit(card_index);

                if(SCI_IsTimerActive(s_sim_reset_bt_timer[card_index]))
                {
                    SCI_DeactiveTimer(s_sim_reset_bt_timer[card_index]);
                }

                if(SCI_IsTimerActive(s_sim_atr_timer[card_index]))
                {
                    SCI_DeactiveTimer(s_sim_atr_timer[card_index]);
                }

                if(SCI_IsTimerActive(s_sim_reset_p_timer[card_index]))
                {
                    SCI_DeactiveTimer(s_sim_reset_p_timer[card_index]);
                }
            }

            break;
            //receive the PPS procesdure card response
        case TB_SIM_PROCESSING_PPS:

            while(s_sim_rx_buf[card_index].check_pos < s_sim_rx_buf[card_index].cur_pos)
            {
#ifdef SIM_SELF_TEST_SIM_PPS_ERR
                s_sim_rx_buf[card_index].buf[3] = 0x1;//test code only!
#endif

                //the pps reponse data is error
                if(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].check_pos] != *(s_pps_info[card_id].pps_data + s_sim_rx_buf[card_index].check_pos))
                {
                    SIMHAL_RejectSIMCard(card_id);
                    s_previous_pps_error[card_id] = TRUE;

                    ReportCardStatus(card_id, TB_SIM_INSERTED_ERROR);

                    if(SCI_IsTimerActive(s_sim_pps_timer[card_index]))
                    {
                        SCI_DeactiveTimer(s_sim_pps_timer[card_index]);
                    }

                    return;
                }

                s_sim_rx_buf[card_index].check_pos++;
            }

            //has receive all the PPS response data
            if(s_sim_rx_buf[card_index].cur_pos >= s_pps_info[card_id].pps_type)
            {
                if(SCI_IsTimerActive(s_sim_pps_timer[card_index]))
                {
                    SCI_DeactiveTimer(s_sim_pps_timer[card_index]);
                }

                //lock the recv data buf,to make sure there no more data coming
                ClearRxBuf(card_id);
                s_sim_status[card_id] = TB_SIM_RECEIVING_RESPONSE;

                //note:if the mobile support the enhanced speed pps procedure,
                //then should modify the clockd dividor of accord the enhance one here
                //2003-10-9 Raistlin.Kong change
                //add the clock divedor change for the enhanced pps
#ifdef SIM_ENHANCED_PPS_SUPPORT
                s_sim_clk_dvd_value[card_id] = (uint32)((sim_F[card_id] / sim_D[card_id]) * s_clk_dvd_factors[s_sim_clk_mode[card_index]]);
                SIMDRV_SetSIMClkDivisor(card_index, s_sim_clk_dvd_value[card_id]);
                sim_dma_support[card_id] = TRUE;
                //SCI_TRACE_LOW:"SIMHAL%d::SIM DMA enable!0x%x."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_5522_112_2_17_22_59_9_4471, (uint8 *)"dd", card_id, ((sim_F[card_id] / (uint32)sim_D[card_id]) * s_clk_dvd_factors[s_sim_clk_mode[card_index]]));
#endif//SIM_ENHANCED_PPS_SUPPORT

                ReportCardStatus(card_id, TB_SIM_INSERTED_OK);

                if(s_sim_is_ext_dualsim)
                {
                    s_sim_ctl0_value[card_id]  = SIMDRV_GetCTL0Value(card_index);
                }
            }

            break;
            //the time when receive the command response
        case TB_SIM_RECEIVING_RESPONSE:

            //both status word has been received,or all the expected data has been received
            if(s_sim_rx_buf[card_index].cur_pos >= TB_SIM_RX_STATUS_WORD_NUM)
            {
                SIMDRV_DisableTx(card_index);//wuding add according to RF Lab Test.
                //the posible SW1 and SW2,reference to GSM 11.11,section9.4
                if(IsStatusWord(s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 2], s_sim_rx_buf[card_index].buf[s_sim_rx_buf[card_index].cur_pos - 1]))
                {
                    //do not reach the expected length of the response,start the timer to
                    //make sure it's really the end of the response data.
                    if(s_sim_rx_buf[card_index].cur_pos < s_sim_rx_buf[card_index].rx_expected_len)
                    {
                        wait_time = SIMHAL_GetWWT(card_id);

                        if(SIM_CARD_ONE == card_index)
                        {
                            SCI_ChangeTimer(s_sim_driver_timer[card_index], SIM1_DriverTimerExpired, (200 + wait_time));
                        }
                        else
                        {
                            SCI_ChangeTimer(s_sim_driver_timer[card_index], SIM2_DriverTimerExpired, (200 + wait_time));
                        }

                        SCI_ActiveTimer(s_sim_driver_timer[card_index]);
                    }
                    else
                    {
                        //lock the recv data buf,to make sure there no more data coming
                        s_sim_rx_buf[card_index].is_locked = TRUE;

                        if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
                        {
                            SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
                        }

                        SendResponseUp(card_id);
                    }
                }

                if(s_sim_rx_buf[card_index].cur_pos < s_sim_rx_buf[card_index].rx_expected_len)
                {
                    //regulate the water mark according to the expected data length
                    water_mark = SIMHAL_CAL_RX_INT_MARK(s_sim_rx_buf[card_index].rx_expected_len - s_sim_rx_buf[card_index].cur_pos);
                    SIMDRV_SetRxWaterMark(card_index, water_mark);
                }
            }
#ifdef SIM_3G_USIM_SUPPORT
            else
            {
                if ((1 == s_sim_rx_buf[card_index].cur_pos) && (2 == s_sim_rx_buf[card_index].rx_expected_len))
                {
                    wait_time = SIMHAL_GetWWT(card_id);
                    wait_time += 10;//add some time

                    if(SIM_CARD_ONE == card_index)
                    {
                        SCI_ChangeTimer(s_sim_driver_timer[card_index], SIM1_DriverTimerExpired, wait_time);
                    }
                    else
                    {
                        SCI_ChangeTimer(s_sim_driver_timer[card_index], SIM2_DriverTimerExpired, wait_time);
                    }
                    SCI_ActiveTimer(s_sim_driver_timer[card_index]);
                }
            }
#endif

            break;
        case TB_SIM_ERROR:
        case TB_SIM_1_8V_DETECT:
        default:
            ClearRxBuf(card_id);
            //SCI_TRACE_LOW:"SIMHAL%d:unexpected rx. %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_5597_112_2_17_22_59_9_4472, (uint8 *)"dd", card_id, s_sim_status[card_id]);
            SIMDRV_ResetRxFifo(card_index);
            break;
    }
}

/**********************************************************************
//    Description:
//      to identify a two byte sequence data can be status word pair or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN IsStatusWord(
    uint8 byte1,    //the byte1 of the data
    uint8 byte2     //the byte 2 of the data
)
{
    if((0x90 == byte1) && (0x00 == byte2))
    {
        return TRUE;
    }

    if((0x9f == byte1) || (0x61 == byte1))
    {
        return TRUE;
    }

    if(0x91 == byte1)
    {
        return TRUE;
    }

    if((0x93 == byte1) && (0x00 == byte2))
    {
        return TRUE;
    }

    if(0x9e == byte1)
    {
        return TRUE;
    }

    if((0x92 == byte1) && ((byte2 & 0xf0) == 0))
    {
        return TRUE;
    }
    else if((0x92 == byte1) && (0x40 == byte2))
    {
        return TRUE;
    }

    if((0x94 == byte1) && (0x00 == byte2))
    {
        return TRUE;
    }

    if((0x94 == byte1) && (0x02 == byte2))
    {
        return TRUE;
    }

    if((0x94 == byte1) && (0x04 == byte2))
    {
        return TRUE;
    }

    if((0x94 == byte1) && (0x08 == byte2))
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x02 == byte2))
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x04 == byte2))
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x08 == byte2))
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x10 == byte2))
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x40 == byte2))
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x50 == byte2))
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x62 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x98 == byte1) && (0x63 == byte2))
    {
        return TRUE;
    }

    //here is something wrong
    if((0x62 == byte1) && (0x00 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x62 == byte1) && (0x81 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x62 == byte1) && (0x82 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x62 == byte1) && (0x83 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x62 == byte1) && (0x85 == byte2))
    {
        return TRUE;
    }
    else if((0x62 == byte1) && (0xF1 == byte2))
    {
        return TRUE;
    }
    else if((0x62 == byte1) && (0xF2 == byte2))
    {
        return TRUE;
    }
    else if((0x62 == byte1) && (0xF3 == byte2))
    {
        return TRUE;
    }

    if(0x63 == byte1) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x64 == byte1) && (0x00 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x65 == byte1) && (0x00 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x65 == byte1) && (0x81 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if(0x67 == byte1)
    {
        return TRUE;
    }

    if((0x68 == byte1) && (0x00 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x68 == byte1) && (0x81 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x68 == byte1) && (0x82 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x69 == byte1) && (0x00 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x69 == byte1) && (0x81 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x69 == byte1) && (0x82 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x69 == byte1) && (0x83 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x69 == byte1) && (0x84 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x69 == byte1) && (0x85 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x69 == byte1) && (0x86 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x80 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x81 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x82 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x83 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x84 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x86 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x87 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if((0x6a == byte1) && (0x88 == byte2)) //according to ETSI 102.221
    {
        return TRUE;
    }

    if(0x6b == byte1)
    {
        return TRUE;
    }

    if(0x6c == byte1)
    {
        return TRUE;
    }

    if(0x6d == byte1)
    {
        return TRUE;
    }

    if(0x6e == byte1)
    {
        return TRUE;
    }

    //when select the three level ef file,the high byte of file id of the
    //file is 0x6f,so the status  word must be distinguished.
    if(0x6f == byte1)
    {
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************
//    Description:
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_ClearVariablePowerOff(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    s_not_t0_protocol_occurs[card_id] = FALSE;
    s_first_protocol_occurs[card_id] = FALSE;
    s_card_resetting[card_id] = FALSE;

    s_sim_clk_mode[card_id] = SIM_ARM_CLK_DIV_8;

    s_sim_working_volt[card_id] = SIMDRV_WORKING_VOLT_1_8V;
    s_sim_reset_times[card_id] = 0;
    s_sim_b_reset_times[card_id] = 0;
    s_sim_pow_reset_times[card_id] = 0;
    s_sim_p_reset_times[card_id] = 0;
    ClearATRData(card_id);
    ClearPPSInfo(card_id);
    s_have_ack_byte[card_index] = FALSE;
}

/**********************************************************************
//    Description:
//      Stop all the timers in sim hal
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_StopAllTimer(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    if(SCI_IsTimerActive(s_sim_driver_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_driver_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_atr_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_atr_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_reset_p_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_reset_p_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_reset_bt_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_reset_bt_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_init_wait_ldo_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_init_wait_ldo_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_reset_wait_ldo_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_reset_wait_ldo_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_pps_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_pps_timer[card_index]);
    }

#ifdef SIM_ENHANCED_PPS_SUPPORT

    if(SCI_IsTimerActive(s_sim_dma_last_byte_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_dma_last_byte_timer[card_index]);
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(SCI_IsTimerActive(s_sim_3g_status_check_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_3g_status_check_timer[card_index]);
    }

#endif
#endif

    if(SCI_IsTimerActive(s_sim_decode_atr_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);
    }

    if(SCI_IsTimerActive(s_sim_tb_te_timer[card_index]))
    {
        SCI_DeactiveTimer(s_sim_tb_te_timer[card_index]);
    }
}

/**********************************************************************
//    Description:
//      PowerOff the SIM card. Because of the SC6600L chip's special Power_enable, I
//      have to make a new poweroff function.
//    If you call this function, the SIM0 or 1's power will be shut down.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_Hardware_PowerOffSIM(SIM_CARD_INDEX_E card_id)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
    if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
    {
        SIMHAL_RejectSIMCard(card_id);
    }
}

/**********************************************************************
//    Description:
//      PowerOff the SIM card and disable the SIM module
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_PowerOffSIM(SIM_CARD_INDEX_E card_id)
{
    if(s_sim_is_ext_dualsim)
    {
        //L chip platform need select SIM card here, because the other SIM may continue to work
        // after the current SIM power off.
        SIMHAL_Select_Card(card_id);
        s_sim_status[card_id] = TB_SIM_1_8V_DETECT;
    }
    else
    {
        s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;
    }

    SIMHAL_ClearVariablePowerOff(card_id);
    SIMHAL_StopAllTimer(card_id);

    SCI_TRACE_LOW("SIMHAL%d::Power Off SIM card. %d", card_id, s_sim_status[card_id]);

    SIMHAL_Hardware_PowerOffSIM(card_id);
}

/**********************************************************************
//    Description:
//      Set the logic level in the clock pin when the clock is disable
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_SetClockStopLevel(
    SIM_CARD_INDEX_E card_id,
    BOOLEAN bHighLevel  //is the logic level high
)
{
    uint8 card_index = (uint8)card_id;
#ifdef _SUPPORT_IPC_//Delos SSTC clk-stop cases 
    uint8 temp = 0;
#endif
    BOOLEAN sim_clk_stop_level = bHighLevel;

    card_index = SIMHAL_GetChipSimIndex(card_id, TRUE);

#ifdef _SUPPORT_IPC_//Delos SSTC clk-stop cases 
    if (SIMHAL_GetATRVoltInfo(card_id, &temp))//ATR clk info is more important than MF
    {
        temp &= (BIT_7|BIT_6);
        if (0x80 == temp)//State H
        {
            sim_clk_stop_level = TRUE;
        }
        else
        {
            sim_clk_stop_level = FALSE;//other cases should stop in LOW.
        }
    }
#endif

    SIMDRV_SetSIMClkStopLevel((uint8)card_index, sim_clk_stop_level);
}

/**********************************************************************
//    Description:
//      judge whether the card is present or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_IsCardNotPresent(SIM_CARD_INDEX_E card_id)
{
    uint16 data_num_in_tx_fifo;
    uint8   rx_fifo_count;
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    data_num_in_tx_fifo = SIMDRV_GetRxFifoCount((uint8)card_index);
    rx_fifo_count = SIMDRV_GetTxFifoCount((uint8)card_index);

    if(TB_SIM_RECEIVING_RESPONSE != s_sim_status[card_id])
    {
        return TRUE;
    }
    else if((!s_have_ack_byte[card_index])
            && (0 == s_sim_rx_buf[card_index].cur_pos)
            && (0 == s_null_byte_count[card_index])
            && (0 == data_num_in_tx_fifo)
            && (0 == rx_fifo_count))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************
//    Description:
//      Get the number of the null byte,it is used to judge whether the
//      ME need to waiting for the sim card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint8 SIMHAL_GetNullByteNum(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    return s_null_byte_count[card_index];
}

/**********************************************************************
//    Description:
//      Get the status of the sim card,so the tool can see the status of the card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
TB_SIM_STATUS_E SIM_GetStatus(SIM_CARD_INDEX_E card_id)
{
    return s_sim_status[card_id];
}

/**********************************************************************
//    Description:
//      select card one or card two
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SIMHAL_Select_Card(SIM_CARD_INDEX_E card_id)
{
    static BOOLEAN first_enter_flag = TRUE;
    static BOOLEAN first_enter_sim1_flag = TRUE;
    static BOOLEAN first_enter_sim2_flag = TRUE;

#ifdef MULTI_SIM_SYS_TRI
    static BOOLEAN first_enter_sim3_flag = TRUE;
#endif

#ifdef MULTI_SIM_SYS_QUAD
    static BOOLEAN first_enter_sim3_flag = TRUE;
    static BOOLEAN first_enter_sim4_flag = TRUE;
    
#endif

#ifdef SIM_6302_SWITCH
    SIM_6302_OUTPUT_E sim_6302_index = SIM_6302_CARD_ID_NOT_CONNECT;
    uint8 ctl0_logic_index_6302_1 = 0;
    uint8 ctl0_logic_index_6302_2 = 0;
#endif

    BOOLEAN enter_flag = FALSE;
    
#ifdef SIM_ENHANCED_PPS_SUPPORT
    int8 i = 0;
#endif
    uint32 ctl0_value = 0;
    uint32 card_index = (uint32)card_id;
    SIM_CARD_INDEX_E current_sel_card = SIM_CARD_ONE;

    if(!s_sim_is_ext_dualsim)
    {
        return;
    }

    current_sel_card = simhal_current_sel_card;
    enter_flag = first_enter_flag;

    if((card_id != current_sel_card) || enter_flag)
    {
#ifdef MULTI_SIM_SYS_TRI
        //SCI_TRACE_LOW:"SIMHAL%d:switch to sim%d. 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x."
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_6249_112_2_17_22_59_10_4475, (uint8 *)"ddddd", current_sel_card, card_id, s_sim_ctl0_value[0], s_sim_ctl0_value[1], s_sim_ctl0_value[2]);
        SCI_TRACE_LOW("SIMHAL%d:from card%d switch to sim%d. 0x%x 0x%x 0x%x.",card_id,current_sel_card, card_id, s_sim_ctl0_value[0], s_sim_ctl0_value[1], s_sim_ctl0_value[2]);
#elif defined (MULTI_SIM_SYS_QUAD)
        //SCI_TRACE_LOW:"SIMHAL%d:switch to sim%d. 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x. %d %d"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_6254_112_2_17_22_59_10_4476, (uint8 *)"dddddddddddd", current_sel_card, card_id, s_sim_ctl0_value[0], s_sim_ctl0_value[1], s_sim_ctl0_value[2], s_sim_ctl0_value[3], s_simhal_6302_reg_val[SIMHAL_6302_REG_RST], s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK], s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA], \
        //             s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM], first_enter_flag, simhal_6302_bb_sim_index, simhal_6302_spi_logic_id, s_sim_6302_switch_flag);
        SCI_TRACE_LOW("SIMHAL%d:from card%d switch to sim%d. 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x.",card_id,current_sel_card, card_id, s_sim_ctl0_value[0], s_sim_ctl0_value[1], s_sim_ctl0_value[2], s_sim_ctl0_value[3], s_simhal_6302_reg_val[SIMHAL_6302_REG_RST], s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK], s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA], \
                     s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM], first_enter_flag, simhal_6302_bb_sim_index, simhal_6302_spi_logic_id, s_sim_6302_switch_flag);                     
#else
        //SCI_TRACE_LOW("SIMHAL%d:switch to sim%d. 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x",simhal_current_sel_card,card_id,s_sim_ctl0_value[0],s_sim_ctl0_value[1],s_sim_ctl0_value[2],s_sim_clk_dvd_value[0],s_sim_clk_dvd_value[1],s_sim_clk_dvd_value[2]);
#endif

        //get the card_index
        card_index = SIMHAL_GetChipSimIndex(current_sel_card, FALSE);

        if(SIMDRV_IsSIMCLKEnabled(card_index))
        {
            SCI_Sleep(2);//make sure wait sim clk enough
            SIMHAL_IdleSIMCard(current_sel_card);
        }

        //remap the card_index to the new card_id
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

#ifdef SIM_ENHANCED_PPS_SUPPORT

        if(s_sim_clk_dvd_value[current_sel_card] != s_sim_clk_dvd_value[card_id])
        {
            SIMDRV_SetSIMClkDivisor(card_index, s_sim_clk_dvd_value[card_id]);
        }

        for(i = 0; i < 100; i++)
        {
        }

#endif

        if(s_sim_ctl0_value[current_sel_card] != s_sim_ctl0_value[card_id])
        {
            if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[current_sel_card] & SIMCTL0_BIT_SIM_RST))
            {
                ctl0_value = (SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN | s_sim_ctl0_value[card_id]);
                ctl0_value &= ~SIMCTL0_BIT_TX_DATA_OUT_LOW;
            }
            else
            {
                ctl0_value = s_sim_ctl0_value[card_id];
            }

            SIMDRV_SetCTL0Value(card_index, ctl0_value);
        }

#if defined (SIM_6302_SWITCH)

        if(s_sim_6302_switch_flag)
        {
            sim_6302_index = SIMHAL_6302_Get_Output_Index(current_sel_card);

            if(0 == simhal_6302_bb_sim_index)
            {
                ctl0_logic_index_6302_1 = 0;
                ctl0_logic_index_6302_2 = 1;
            }
            else if(1 == simhal_6302_bb_sim_index)
            {
                ctl0_logic_index_6302_1 = 1;
                ctl0_logic_index_6302_2 = 2;
            }
            else//if (2 == simhal_6302_bb_sim_index)
            {
                ctl0_logic_index_6302_1 = 2;
                ctl0_logic_index_6302_2 = 3;
            }
        }

        if(SIM_CARD_ONE == card_id)
        {
            if(s_sim_6302_switch_flag)
            {
                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= (BIT_0 | BIT_1);

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_1] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_2;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_2;
                    }

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_2] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_3;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_3;
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~(BIT_1 | BIT_0); //disconnect SIM2,3 IO
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//Force both clk to low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }

                if((0 == simhal_6302_bb_sim_index) && (SIM_CARD_TWO == current_sel_card))
                {
                    //Donot need to switch the sim controller
                }
                else
                {
                    SIMDRV_L_Switch(SIM_CARD_ONE);
                }

                sim_6302_index = SIMHAL_6302_Get_Output_Index(card_id);

                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    if(0 == simhal_6302_bb_sim_index)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_0;
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_1;

                        if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[SIM_CARD_TWO] & SIMCTL0_BIT_SIM_RST))
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_3;
                        }
                        else
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_3;
                        }

                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~BIT_1;//disconnect IO
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_0;//connect IO
                    }
                    else
                    {
                        SCI_ASSERT(0);//That's impossible!
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//force both clk low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }
            }
            else
            {
                SIMDRV_L_Switch(SIM_CARD_ONE);
            }

            if(first_enter_sim1_flag)
            {
                first_enter_sim1_flag = FALSE;
                s_sim_ctl0_value[card_id] &= ~(SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN);
                s_sim_ctl0_value[card_id] |= SIMCTL0_BIT_TX_DATA_OUT_LOW;
                SIMDRV_SetCTL0Value(SIM_CARD_ONE, s_sim_ctl0_value[card_id]);
            }
        }
        else if(SIM_CARD_TWO == card_id)
        {
            if(s_sim_6302_switch_flag)
            {
                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= (BIT_0 | BIT_1);

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_1] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_2;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_2;
                    }

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_2] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_3;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_3;
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~(BIT_1 | BIT_0); //disconnect SIM2,3 IO
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//Force both clk to low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }

                if(((0 == simhal_6302_bb_sim_index) && (SIM_CARD_ONE == current_sel_card))
                    || ((1 == simhal_6302_bb_sim_index) && (SIM_CARD_THREE == current_sel_card)))
                {
                    //Donot need to switch the sim controller
                }
                else
                {
                    if(0 == simhal_6302_bb_sim_index)
                    {
                        SIMDRV_L_Switch(SIM_CARD_ONE);
                    }
                    else//the 6302 is connected to bb sim_1 or bb sim_2
                    {
                        SIMDRV_L_Switch(SIM_CARD_TWO);
                    }
                }

                sim_6302_index = SIMHAL_6302_Get_Output_Index(card_id);

                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    if(0 == simhal_6302_bb_sim_index)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_1;
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_0;

                        if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[SIM_CARD_ONE] & SIMCTL0_BIT_SIM_RST))
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_2;
                        }
                        else
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_2;
                        }

                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~BIT_0;//disconnect IO
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_1;//connect IO
                    }
                    else if(1 == simhal_6302_bb_sim_index)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_0;
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_1;

                        if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[SIM_CARD_THREE] & SIMCTL0_BIT_SIM_RST))
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_3;
                        }
                        else
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_3;
                        }

                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~BIT_1;//disconnect IO
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_0;//connect IO
                    }
                    else
                    {
                        SCI_ASSERT(0);//That's impossible!
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//force both clk low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }
            }
            else
            {
                SIMDRV_L_Switch(SIM_CARD_TWO);
            }

            if(first_enter_sim2_flag)
            {
                first_enter_sim2_flag = FALSE;
                s_sim_ctl0_value[card_id] &= ~(SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN);
                s_sim_ctl0_value[card_id] |= SIMCTL0_BIT_TX_DATA_OUT_LOW;
                SIMDRV_SetCTL0Value(SIM_CARD_ONE, s_sim_ctl0_value[card_id]);
            }
        }
        else if(SIM_CARD_THREE == card_id)
        {
            if(s_sim_6302_switch_flag)
            {
                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= (BIT_0 | BIT_1);

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_1] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_2;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_2;
                    }

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_2] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_3;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_3;
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~(BIT_1 | BIT_0); //disconnect SIM2,3 IO
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//Force both clk to low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }

                if(((1 == simhal_6302_bb_sim_index) && (SIM_CARD_TWO == current_sel_card))
#ifdef MULTI_SIM_SYS_QUAD
                    || ((2 == simhal_6302_bb_sim_index) && (SIM_CARD_FOUR == current_sel_card))
#endif                    
                    )
                {
                    //Donot need to switch the sim controller
                }
                else
                {
                    if((0 == simhal_6302_bb_sim_index) || (1 == simhal_6302_bb_sim_index))
                    {
                        SIMDRV_L_Switch(SIM_CARD_TWO);
                    }
                    else//the 6302 is connected to bb sim_3
                    {
                        SIMDRV_L_Switch(SIM_CARD_THREE);
                    }
                }

                sim_6302_index = SIMHAL_6302_Get_Output_Index(card_id);

                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    if(1 == simhal_6302_bb_sim_index)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_1;
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_0;

                        if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[SIM_CARD_TWO] & SIMCTL0_BIT_SIM_RST))
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_2;
                        }
                        else
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_2;
                        }

                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~BIT_0;//disconnect IO
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_1;//connect IO
                    }
#ifdef MULTI_SIM_SYS_QUAD                    
                    else if(2 == simhal_6302_bb_sim_index)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_0;
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_1;

                        if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[SIM_CARD_FOUR] & SIMCTL0_BIT_SIM_RST))
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_3;
                        }
                        else
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_3;
                        }

                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~BIT_1;//disconnect IO
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_0;//connect IO
                    }
#endif                    
                    else
                    {
                        SCI_ASSERT(0);//That's impossible!
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//force both clk low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }
            }
            else
            {
                SIMDRV_L_Switch(SIM_CARD_THREE);
            }

            if(first_enter_sim3_flag)
            {
                first_enter_sim3_flag = FALSE;
                s_sim_ctl0_value[card_id] &= ~(SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN);
                s_sim_ctl0_value[card_id] |= SIMCTL0_BIT_TX_DATA_OUT_LOW;
                SIMDRV_SetCTL0Value(SIM_CARD_ONE, s_sim_ctl0_value[card_id]);
            }
        }
#ifdef MULTI_SIM_SYS_QUAD        
        else if(SIM_CARD_FOUR == card_id)
        {
            if(s_sim_6302_switch_flag)
            {
                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= (BIT_0 | BIT_1);

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_1] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_2;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_2;
                    }

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[ctl0_logic_index_6302_2] & SIMCTL0_BIT_SIM_RST))
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_3;
                    }
                    else
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_3;
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~(BIT_1 | BIT_0); //disconnect SIM2,3 IO
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//Force both clk to low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(current_sel_card, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }

                if((2 == simhal_6302_bb_sim_index) && (SIM_CARD_THREE == current_sel_card))
                {
                    //Donot need to switch the sim controller
                }
                else
                {
                    SIMDRV_L_Switch(SIM_CARD_THREE);
                }

                sim_6302_index = SIMHAL_6302_Get_Output_Index(card_id);

                if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
                {
                    if(2 == simhal_6302_bb_sim_index)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_1;
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_0;

                        if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[SIM_CARD_THREE] & SIMCTL0_BIT_SIM_RST))
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] |= BIT_2;
                        }
                        else
                        {
                            s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_2;
                        }

                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] &= ~BIT_0;//disconnect IO
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_1;//connect IO
                    }
                    else
                    {
                        SCI_ASSERT(0);//That's impossible!
                    }

                    s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] = 0;//force both clk low

                    if(SIMCTL0_BIT_SIM_RST == (s_sim_ctl0_value[card_id] & SIMCTL0_BIT_SIM_RST))
                    {
                        SIMDRV_SetSIMRST(SIM_CARD_ONE, TRUE);
                    }

                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
                    SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
                }
            }
            else
            {
                SIMDRV_L_Switch(SIM_CARD_THREE);
            }

            if(first_enter_sim4_flag)
            {
                first_enter_sim4_flag = FALSE;
                s_sim_ctl0_value[card_id] &= ~(SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN);
                s_sim_ctl0_value[card_id] |= SIMCTL0_BIT_TX_DATA_OUT_LOW;
                SIMDRV_SetCTL0Value(SIM_CARD_ONE, s_sim_ctl0_value[card_id]);
            }
        }
#endif        
        else
        {
            SCI_ASSERT(0);/*assert verified */
        }

#else

        if(SIM_CARD_ONE == card_id)
        {
#ifdef MULTI_SIM_SYS_DUAL
#if defined (MULTI_SIM_SYS_DUAL_6620_USE_SIM1_AND_SIM2)
            SIMDRV_L_Switch(SIM_CARD_TWO);
#else
        SIMDRV_L_Switch(SIM_CARD_ONE);
#endif
#else
        SIMDRV_L_Switch(SIM_CARD_ONE);
#endif

            if(first_enter_sim1_flag)
            {
                first_enter_sim1_flag = FALSE;
                s_sim_ctl0_value[card_id] &= ~(SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN);
                s_sim_ctl0_value[card_id] |= SIMCTL0_BIT_TX_DATA_OUT_LOW;
                SIMDRV_SetCTL0Value(SIM_CARD_ONE, s_sim_ctl0_value[card_id]);
            }
        }
        else if(SIM_CARD_TWO == card_id)
        {
#ifdef MULTI_SIM_SYS_DUAL
#if defined (MULTI_SIM_SYS_DUAL_6620_USE_SIM0_AND_SIM2) || defined (MULTI_SIM_SYS_DUAL_6620_USE_SIM1_AND_SIM2)
            SIMDRV_L_Switch(2);//SIM_CARD_THREE
#else
        SIMDRV_L_Switch(SIM_CARD_TWO);
#endif
#else
        SIMDRV_L_Switch(SIM_CARD_TWO);
#endif

            if(first_enter_sim2_flag)
            {
                first_enter_sim2_flag = FALSE;
                s_sim_ctl0_value[card_id] &= ~(SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN);
                s_sim_ctl0_value[card_id] |= SIMCTL0_BIT_TX_DATA_OUT_LOW;
                SIMDRV_SetCTL0Value(SIM_CARD_ONE, s_sim_ctl0_value[card_id]);
            }
        }

#ifdef MULTI_SIM_SYS_TRI
        else if(SIM_CARD_THREE == card_id)
        {
            SIMDRV_L_Switch(SIM_CARD_THREE);

            if(first_enter_sim3_flag)
            {
                first_enter_sim3_flag = FALSE;
                s_sim_ctl0_value[card_id] &= ~(SIMCTL0_BIT_SIM_RST | SIMCTL0_BIT_POWER_EN);
                s_sim_ctl0_value[card_id] |= SIMCTL0_BIT_TX_DATA_OUT_LOW;
                SIMDRV_SetCTL0Value(SIM_CARD_ONE, s_sim_ctl0_value[card_id]);
            }
        }

#endif
        else
        {
            SCI_TRACE_HIGH("SIMHAL::invalid card id.");/*assert verified */
        }

#endif//SIM_6302_SWITCH

        first_enter_flag = FALSE;
        simhal_current_sel_card = card_id;
    }
}

/**********************************************************************
//    Description:
//     Set the SIM card's RST high
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void  SIMHAL_SetRSTHigh(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

    SIMDRV_SetSIMRST(card_index, TRUE);

    //SIMDRV_SetSIMActive(card_index);//this will lead a pike when 1.8V
    if(s_sim_is_ext_dualsim)
    {
        s_sim_ctl0_value[card_id] = SIMDRV_GetCTL0Value(card_index);
    }

#ifdef MULTI_SIM_SYS_TRI
    //SCI_TRACE_LOW:"SIMHAL%d:Set RST HIGH 0x%x 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_7259_112_2_17_22_59_12_4478, (uint8 *)"dddd", card_id, s_sim_ctl0_value[0], s_sim_ctl0_value[1], s_sim_ctl0_value[2]);
#elif defined (MULTI_SIM_SYS_QUAD)
    //SCI_TRACE_LOW:"SIMHAL%d:Set RST HIGH 0x%x 0x%x 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_7261_112_2_17_22_59_12_4479, (uint8 *)"ddddd", card_id, s_sim_ctl0_value[0], s_sim_ctl0_value[1], s_sim_ctl0_value[2], s_sim_ctl0_value[3]);
#else
    //SCI_TRACE_LOW:"SIMHAL%d:Set RST HIGH 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_7263_112_2_17_22_59_12_4480, (uint8 *)"ddd", card_id, s_sim_ctl0_value[0], s_sim_ctl0_value[1]);
#endif
}

/**********************************************************************
//    Description:
//      SIM task call this function to set the whether there is a 1.8V only SIM card inserted.
//      1, when ATR has 1.8V only infomation, call this func.
//      1, when RESP of DFgsm has 1.8V only infomation, call this func.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_SetVoltLimit(SIM_CARD_INDEX_E card_id, BOOLEAN is_1_8V_only)
{
    s_simhal_has_1_8v_only[card_id] = is_1_8V_only;
}

/**********************************************************************
//    Description:
//      SIM task call this function to get the whether there is a 1.8V only SIM card inserted.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_GetVoltLimit(SIM_CARD_INDEX_E card_id)
{
    return s_simhal_has_1_8v_only[card_id];
}

/**********************************************************************
//    Description:
//      SIM task call this function to set the whether the 1.8V detect procedure finished
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_SetVoltDetectComplete(SIM_CARD_INDEX_E card_id, BOOLEAN is_complete)
{
    s_simhal_1_8v_detect_complete[card_id] = is_complete;
}

/**********************************************************************
//    Description:
//      record the SIM card's volt info in ATR data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SetATRVoltInfo(SIM_CARD_INDEX_E card_id, uint8 atr_volt_byte)
{
    s_simhal_atr_volt_get_flag[card_id] = TRUE;
    s_simhal_atr_volt_byte[card_id] = atr_volt_byte;
}

/**********************************************************************
//    Description:
//     get the SIM card's volt info in ATR data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_GetATRVoltInfo(SIM_CARD_INDEX_E card_id, uint8 *atr_volt_byte)
{
    if(s_simhal_atr_volt_get_flag[card_id])
    {
        *atr_volt_byte = s_simhal_atr_volt_byte[card_id];
    }

    return s_simhal_atr_volt_get_flag[card_id];
}

/**********************************************************************
//    Description:
//       Judge the platform is an ext-dualsim(H1/H2/I) or dualsim(6600R)
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_IsExtDualSim(void)
{
	//Get the BB chip type
	s_sim_chip_type = CHIP_GetChipType();
#if defined(CHIP_VER_6531)
	if(SC6531 == s_sim_chip_type)
    {
        s_sim_chip_type = SC6530;
    }

#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
    if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
    {
        s_sim_is_ext_dualsim = TRUE;
#ifdef SIM_6302_SWITCH
        s_sim_6302_switch_flag = TRUE;
#endif
    }
}

/**********************************************************************
//    Description:
//      After Tb/Te time, call this function to set RST high.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIM1_RST_After_Tb_or_Te(uint32 lparam)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_ONE;
    uint8 card_index = (uint8)card_id;
    uint32 temp = SIMDRV_GetCTL0Value(0);

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
    }

    //SCI_TRACE_LOW:"SIMHAL%d:SIM1_RST_After_Tb_or_Te. 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_7407_112_2_17_22_59_13_4481, (uint8 *)"dd", card_id, temp);

    //Start the ATR monitor timer, we set it as 300ms.
    //In spec ISO7816-3, the time is between (400/f) and (40 000/f).
    if(SCI_IsTimerActive(s_sim_atr_timer[SIM_CARD_ONE]))
    {
        SCI_DeactiveTimer(s_sim_atr_timer[SIM_CARD_ONE]);
    }

    SCI_ChangeTimer(s_sim_atr_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_ATR_TIMER_VALUE);
    SCI_ActiveTimer(s_sim_atr_timer[SIM_CARD_ONE]);

#ifdef SIM_6302_SWITCH

    if(s_sim_6302_switch_flag)
    {
        SIM_6302_OUTPUT_E sim_6302_output_index = SIMHAL_6302_Get_Output_Index(card_id);

        if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_output_index)
        {
            if(SIM_6302_OUTPUT_0 == sim_6302_output_index)
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_0;//the same with BB
            }
            else
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_RST] &= ~BIT_1;//the same with BB
            }

            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_RST, s_simhal_6302_reg_val[SIMHAL_6302_REG_RST]);
        }
    }

#endif

    //Set the RST high
    SIMHAL_SetRSTHigh(card_id);
    SIMDRV_EnableSIMRXInt(card_index);
	//SIMHAL_PrintRegisterBuf(card_id);

    s_card_resetting[card_id] = FALSE;
    return;
}

/**********************************************************************
//    Description:
//       After Tb/Te time, call this function to set RST high.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIM2_RST_After_Tb_or_Te(uint32 lparam)
{
    SIM_CARD_INDEX_E card_id = SIM_CARD_TWO;
    uint8 card_index = (uint8)card_id;

    if(s_sim_is_ext_dualsim)
    {
        card_id = simhal_current_sel_card;
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		if((s_sim_chip_type > SC6531EFM_ID_BASE) && (s_sim_chip_type < SC6531EFM_ID_MAX))
#else
        if((s_sim_chip_type > SC6530_ID_BASE) && (s_sim_chip_type < SC6530_ID_MAX))
#endif
        {
            SCI_ASSERT(0);/*assert verified */
        }
    }

    //SCI_TRACE_LOW:"SIMHAL1:SIM2_RST_After_Tb_or_Te"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_7523_112_2_17_22_59_13_4483, (uint8 *)"");

    //Start the ATR monitor timer, we set it as 300ms.
    //In spec ISO7816-3, the time is between (400/f) and (40 000/f).
    if(SCI_IsTimerActive(s_sim_atr_timer[SIM_CARD_TWO]))
    {
        SCI_DeactiveTimer(s_sim_atr_timer[SIM_CARD_TWO]);
    }

    SCI_ChangeTimer(s_sim_atr_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_ATR_TIMER_VALUE);
    SCI_ActiveTimer(s_sim_atr_timer[SIM_CARD_TWO]);

    //Set the RST high
    SIMHAL_SetRSTHigh(card_id);
    SIMDRV_EnableSIMRXInt(card_index);

    s_card_resetting[card_id] = FALSE;
    return;
}

#ifdef SIM_6302_SWITCH
#ifdef SIM_6302_DEBUG
uint32 SIMHAL_GetCurTaskId(void)
{
    uint32 cur_task_id = 0;
    if (P_SIM == SCI_IdentifyThread())
    {
        cur_task_id = 0;
    }
    else if (P_SIM_2 == SCI_IdentifyThread())
    {
        cur_task_id = 1;
    }
#ifdef MULTI_SIM_SYS_TRI     
    else if (P_SIM_3 == SCI_IdentifyThread())
    {
        cur_task_id = 2;
    }
#endif
#ifdef MULTI_SIM_SYS_QUAD
    else if (P_SIM_4 == SCI_IdentifyThread())
    {
        cur_task_id = 3;
    }
#endif
    else
    {
        cur_task_id = simhal_current_sel_card;
    }
    
    return cur_task_id;
}

char *Get6302RstSetting(uint8 val)
{
    switch(val)
    {
        case 0:return "RST output1:same as bb;    output2:same as bb";
        case 1:return "RST output1:low;           output2:same as bb";
        case 2:return "RST output1:same as bb;    output2:low";
        case 3:return "RST output1:low;           output2:low";
        case 4:return "RST invalid setting value 4";
        case 5:return "RST output1:high;          output2:same as bb";
        case 6:return "RST invalid setting value 6";
        case 7:return "RST output1:high;          output2:low";
        case 8:return "RST invalid setting value 8";
        case 9:return "RST invalid setting value 9";
        case 10:return "RST output1:same as bb;   output2:high";
        case 11:return "RST output1:low;          output2:high";
        case 12:return "RST invalid setting value 12";
        case 13:return "RST invalid setting value 13";
        case 14:return "RST invalid setting value 14";
        case 15:return "RST output1:high;         output2:high";
        default:return "RST invalid setting value";
    }                
}

char *Get6302ClkSetting(uint8 val)
{
    switch(val)
    {
        case 0x20:return "CLK output1:stop at low;      output2:stop at low";
        case 0x21:return "CLK output1:same as bb;       output2:stop at low";
        case 0x22:return "CLK output1:stop at low;      output2:same as bb";
        case 0x23:return "CLK output1:same as bb;       output2:same as bb";
        case 0x24:return "CLK invalid setting value 0x24";
        case 0x25:return "CLK output1:stop at high;     output2:stop at low";
        case 0x26:return "CLK invalid setting value 0x26";
        case 0x27:return "CLK output1:stop at high;     output2:same as bb";
        case 0x28:return "CLK invalid setting value 0x28";
        case 0x29:return "CLK invalid setting value 0x29";
        case 0x2a:return "CLK output1:stop at low;      output2:stop at high";
        case 0x2b:return "CLK output1:same as bb;       output2:stop at high";
        case 0x2c:return "CLK invalid setting value 0x2c";
        case 0x2d:return "CLK invalid setting value 0x2d";
        case 0x2e:return "CLK invalid setting value 0x2e";
        case 0x2f:return "CLK output1:stop at high;     output2:stop at high";
        default:return "CLK invalid setting value";
    }                
}

char *Get6302DataSetting(uint8 val)
{
    switch(val)
    {
        case 0x40:return "DATA output1:disconnect;       output2:disconnect";
        case 0x41:return "DATA output1:connect normal;   output2:disconnect";
        case 0x42:return "DATA output1:disconnect;       output2:connect normal";
        case 0x43:return "DATA output1:connect normal;   output2:connect normal";
        case 0x44:return "DATA invalid setting value 0x44";
        case 0x45:return "DATA output1:connect force low;output2:disconnect";
        case 0x46:return "DATA invalid setting value 0x46";
        case 0x47:return "DATA output1:connect force low;output2:connect normal";
        case 0x48:return "DATA invalid setting value 0x48";
        case 0x49:return "DATA invalid setting value 0x49";
        case 0x4a:return "DATA output1:disconnect;       output2:connect force low";
        case 0x4b:return "DATA output1:connect normal;   output2:connect force low";
        case 0x4c:return "DATA invalid setting value 0x4c";
        case 0x4d:return "DATA invalid setting value 0x4d";
        case 0x4e:return "DATA invalid setting value 0x4e";
        case 0x4f:return "DATA output1:connect force low; output2:connect force low";
        default:return "DATA invalid setting value";
    }                
}

char *Get6302VsimSetting(uint8 val)
{
    switch(val)
    {
        case 0x60:return "VSIM output1:poweroff;         output2:poweroff";
        case 0x61:return "VSIM invalid setting value 0x61";
        case 0x62:return "VSIM invalid setting value 0x62";
        case 0x63:return "VSIM invalid setting value 0x63";
        case 0x64:return "VSIM output1:poweron 1.8v;    output2:poweroff";
        case 0x65:return "VSIM output1:poweron 3v;      output2:poweroff";
        case 0x66:return "VSIM invalid setting value 0x66";
        case 0x67:return "VSIM invalid setting value 0x67";
        case 0x68:return "VSIM output1:poweroff;        output2:poweron 1.8v";
        case 0x69:return "VSIM invalid setting value 0x69";
        case 0x6a:return "VSIM output1:poweroff;        output2:poweron 3v";
        case 0x6b:return "VSIM invalid setting value 0x6b";
        case 0x6c:return "VSIM output1:poweron 1.8v;    output2: poweron 1.8v";
        case 0x6d:return "VSIM output1:poweron 3v;      output2: poweron 1.8v";
        case 0x6e:return "VSIM output1:poweron 1.8v;    output2: poweron 3v";
        case 0x6f:return "VSIM output1:poweron 3v;      output2: poweron 3v";
        default:return "VSIM invalid setting value";
    }                
}
#endif

/**********************************************************************
//    Description:
//     Get the 6302 port index according to the sim card id
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_6302_OUTPUT_E SIMHAL_6302_Get_Output_Index(SIM_CARD_INDEX_E card_id)
{
    SIM_6302_OUTPUT_E sim_6302_index = SIM_6302_CARD_ID_NOT_CONNECT;

#ifdef SIM_6302_DEBUG	
    uint32 cur_task_id = SIMHAL_GetCurTaskId();
#endif
    
    if(s_sim_6302_switch_flag)
    {
        if((0 == simhal_6302_bb_sim_index) &&
            ((SIM_CARD_ONE == card_id) || (SIM_CARD_TWO == card_id)))
        {
            if(SIM_CARD_ONE == card_id)
            {
                sim_6302_index = SIM_6302_OUTPUT_0;
            }
            else
            {
                sim_6302_index = SIM_6302_OUTPUT_1;
            }
        }
        else if((1 == simhal_6302_bb_sim_index) &&
                ((SIM_CARD_TWO == card_id) || (SIM_CARD_THREE == card_id)))
        {
            if(SIM_CARD_TWO == card_id)
            {
                sim_6302_index = SIM_6302_OUTPUT_0;
            }
            else
            {
                sim_6302_index = SIM_6302_OUTPUT_1;
            }
        }
#ifdef MULTI_SIM_SYS_QUAD        
        else if((2 == simhal_6302_bb_sim_index) &&
                ((SIM_CARD_THREE == card_id) || (SIM_CARD_FOUR == card_id)))
        {
            if(SIM_CARD_THREE == card_id)
            {
                sim_6302_index = SIM_6302_OUTPUT_0;
            }
            else
            {
                sim_6302_index = SIM_6302_OUTPUT_1;
            }
        }
#endif        
    }

#ifdef SIM_6302_DEBUG
    if(sim_6302_index == SIM_6302_CARD_ID_NOT_CONNECT)
    {
        SCI_TRACE_LOW("SIMHAL%d:card%d is not connect to 6302",cur_task_id,card_id);
    }
    else
    {
        SCI_TRACE_LOW("SIMHAL%d:card%d is connect to 6302 output%d",cur_task_id,card_id,(sim_6302_index + 1));
    }
#endif
    
    return sim_6302_index;
}

/**********************************************************************
//    Description:
//      Set the SIM card's Vsim via chip 6302.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_6302_SetVsim(SIM_CARD_INDEX_E card_id, BOOLEAN is_pwr_off, SIMDRV_WORKING_VOLT_E vol)
{
    SIM_6302_OUTPUT_E sim_6302_index = SIM_6302_CARD_ID_NOT_CONNECT;

    if(s_sim_6302_switch_flag)
    {
        sim_6302_index = SIMHAL_6302_Get_Output_Index(card_id);

        if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
        {
            if(SIM_6302_OUTPUT_0 == sim_6302_index)
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] &= ~(BIT_0 | BIT_2); //clear the old value

                if(!is_pwr_off)
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_2;

                    if(SIMDRV_WORKING_VOLT_3V == vol)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_0;
                    }
                }
            }
            else
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] &= ~(BIT_1 | BIT_3); //clear the old value

                if(!is_pwr_off)
                {
                    s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_3;

                    if(SIMDRV_WORKING_VOLT_3V == vol)
                    {
                        s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_1;
                    }
                }
            }

            //SPI to write the Vcc controller
            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_VSIM, s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM]);
        }
    }
}

/**********************************************************************
//    Description:
//      Init the SPI_x to control 6302 chip.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_6302_InitSPI(void)
{
    static BOOLEAN entered_flag = FALSE;

    if(entered_flag)
    {
        return;
    }

    entered_flag = TRUE;
    //wuding: we int both the 6302-chips at once.

    simhal_6302_spi_logic_id = GPIO_SPI_Quadsim_Get_Config(&simhal_6302_bb_sim_index);

    if((0xFF != simhal_6302_spi_logic_id)
        && ((0 == simhal_6302_bb_sim_index) || (1 == simhal_6302_bb_sim_index) || (2 == simhal_6302_bb_sim_index)))
    {
        s_sim_6302_switch_flag = TRUE;
        //SCI_TRACE_LOW:"SIMHAL:SPI init"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_7804_112_2_17_22_59_13_4484, (uint8 *)"");
		SCI_TRACE_LOW("SIMHAL:SPI init, 6302 is connected to sim%d",simhal_6302_bb_sim_index + 1);
    }
    else
    {
		SCI_TRACE_LOW("SIMHAL:SPI init, 6302 is not connected.");
        s_sim_6302_switch_flag = FALSE;
    }

#if 0//Self-test code
    {
        uint8 temp = 0x6F;//set both Vsim 3V
        uint32 cs_sig;
        cs_sig = SPI_CS_LOW;

        SPI_HAL_Ioctl(simhal_6302_spi_logic_id, SPI_CTL_S_CS, &cs_sig);

        SCI_ASSERT(0);

        SPI_HAL_Write(simhal_6302_spi_logic_id, &temp, 1);

        cs_sig = SPI_CS_HIGH;
        SPI_HAL_Ioctl(simhal_6302_spi_logic_id, SPI_CTL_S_CS, &cs_sig);
        //Self-test code

        SCI_ASSERT(0);
    }
#endif
}

/**********************************************************************
//    Description:
//      Init the SPI_x to control 6302 chip.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_6302_WriteSPI(SIM_CARD_INDEX_E card_id, SIMHAL_6302_REG_E module, uint8 value)
{
    SIM_6302_OUTPUT_E sim_6302_index = SIM_6302_CARD_ID_NOT_CONNECT;
    uint8 temp = 0;
    uint32 cs_sig;

#ifdef SIM_6302_DEBUG	
    uint32 cur_task_id = SIMHAL_GetCurTaskId();
#endif

    if(s_sim_6302_switch_flag)
    {
        sim_6302_index = SIMHAL_6302_Get_Output_Index(card_id);

        if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_index)
        {
            temp = value;
            temp &= (BIT_3 | BIT_2 | BIT_1 | BIT_0); //MSB 4 bits should be 0

            if(SIMHAL_6302_REG_RST == module)
            {
#ifdef SIM_6302_DEBUG			
                SCI_TRACE_LOW("SIMHAL%d:SPI to 6302 RST reg, value 0x%x, setting=%s,for card%d.", cur_task_id,temp,Get6302RstSetting(temp), card_id);
#endif				
            }
            else if(SIMHAL_6302_REG_CLK == module)
            {
                temp |= BIT_5;
#ifdef SIM_6302_DEBUG				
                SCI_TRACE_LOW("SIMHAL%d:SPI to 6302 CLK reg, value 0x%x, setting=%s,for card%d.", cur_task_id,temp,Get6302ClkSetting(temp), card_id);
#endif				
            }
            else if(SIMHAL_6302_REG_DATA == module)
            {
                temp |= BIT_6;
#ifdef SIM_6302_DEBUG				
                SCI_TRACE_LOW("SIMHAL%d:SPI to 6302 DATA reg, value 0x%x, setting=%s,for card%d.", cur_task_id,temp,Get6302DataSetting(temp), card_id);
#endif				
            }
            else if(SIMHAL_6302_REG_VSIM == module)
            {
                temp |= (BIT_6 | BIT_5);
#ifdef SIM_6302_DEBUG				
                SCI_TRACE_LOW("SIMHAL%d:SPI to 6302 VSIM reg, value 0x%x, setting=%s,for card%d.", cur_task_id,temp,Get6302VsimSetting(temp), card_id);
#endif				
            }
            else
            {
                SCI_ASSERT(0);/*assert verified */
            }

#ifndef SIM_6302_DEBUG
            SCI_TRACE_LOW("SIMHAL%d:SPI to 6302 reg%d, value 0x%x", card_id, module, temp);
#endif			

            cs_sig = SPI_CS_LOW;
            SPI_HAL_Ioctl(simhal_6302_spi_logic_id, SPI_CTL_S_CS, &cs_sig);

            SPI_HAL_Write(simhal_6302_spi_logic_id, &temp, 1);

            cs_sig = SPI_CS_HIGH;
            SPI_HAL_Ioctl(simhal_6302_spi_logic_id, SPI_CTL_S_CS, &cs_sig);
        }
    }
}

/**********************************************************************
//      Get the baseband chip is connected with 6302 or 6303. TRUE:6302, FALSE:6303.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_MultiSim_Is6302(void)
{
    return TRUE;
}

/**********************************************************************
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_MultiSim_Set_ParallelDual6302(BOOLEAN flag)
{
    return;
}

/**********************************************************************
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_MultiSim_IsParallelDual6302(void)
{
    return FALSE;
}
#endif//SIM_6302_SWITCH

/**********************************************************************
//    Description:
//      To judge wether the sim is the first or second one according to the card_id
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static uint8 SIMHAL_GetChipSimIndex(SIM_CARD_INDEX_E card_id, BOOLEAN is_6600l_sim_ldo)
{
    uint8 chip_sim_index = card_id;

    if(is_6600l_sim_ldo)
    {
        if(0 == card_id)
        {
#ifdef MULTI_SIM_SYS_DUAL
#ifdef MULTI_SIM_SYS_DUAL_6620_USE_SIM1_AND_SIM2
            chip_sim_index = 1;
#else
            chip_sim_index = 0;
#endif
#else
            chip_sim_index = 0;
#endif
        }
        else if(1 == card_id)
        {
#ifdef MULTI_SIM_SYS_DUAL
#if defined (MULTI_SIM_SYS_DUAL_6620_USE_SIM0_AND_SIM2) || defined (MULTI_SIM_SYS_DUAL_6620_USE_SIM1_AND_SIM2)
            chip_sim_index = 2;
#else
            chip_sim_index = 1;
#endif
#else
            chip_sim_index = 1;
#endif
        }
        else
        {
            chip_sim_index = 2;
        }

#ifdef SIM_6302_SWITCH
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        if(2 == card_id)
        {
            chip_sim_index = 1;//in 6531efm,6302 is connected to sim1,and card2 is connected to 6302,so the chip_sim_index=1.
        }
#endif		
#endif		
    }
    else
    {
        chip_sim_index = 0;
    }

    return chip_sim_index;
}

/**********************************************************************
//      Get the baseband chip is parallel dualsim or not. (6800H currrently)
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_IsParallelDualSIM(void)
{
    return FALSE;
}

#ifdef SIM_PLUG_IN_SUPPORT
/**********************************************************************
//      For the sim0 plug in/out gpio int callback
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static uint32 SIMHAL_SIM_Plugin_GPIO_Callback(BOOLEAN gpio_state)
{
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
#ifdef MULTI_SIM_SYS_QUAD
    SIM_CARD_INDEX_E current_sel_card = SIM_CARD_FOUR;//In SC6530 openphone, the sim3 is connected for plugin/out
#else
    SIM_CARD_INDEX_E current_sel_card = SIM_CARD_ONE;
#endif
    //The sim card id is decided by the hardware connection. For example, if the GPIO66
    // is connected with SIM 0, then all the interrupts from GPIO66 are for the sim0,
    //you should use SIMHAL_CARD_ONE.
    //SCI_TRACE_LOW:"SIMHAL_SIM0_Plugin_GPIO_Callback %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_8495_112_2_17_22_59_15_4489, (uint8 *)"d", gpio_state);

    if(s_sim_is_ext_dualsim)
    {
        if(current_sel_card == simhal_current_sel_card)
        {
            if((TB_SIM_WAITING_FOR_ATR == s_sim_status[current_sel_card])
                || (TB_SIM_PROCESSING_ATR == s_sim_status[current_sel_card])
                || (TB_SIM_PROCESSING_PPS == s_sim_status[current_sel_card]))
            {
                //Do nothing
            }
            else
            {
                SIMHAL_RejectSIMCard(current_sel_card);//the current sim's hot swap
            }

            SIMHAL_StopAllTimer(current_sel_card);
        }
    }

    if(SCI_IsTimerActive(s_sim_hot_swap_stable_timer))
    {
        SCI_DeactiveTimer(s_sim_hot_swap_stable_timer);
    }

    if(gpio_state)
    {
        //GPIO state is high, sim is pulled out.
        s_sim_hot_swap_latest_int = TB_SIMINT_PULL_OUT;
        //notify sim is pulled out at once...
        SIMHAL_SIM_Hotswap_Stable_TimeOut(0);
    }
    else
    {
        //GPIO state is low, sim is inserted.
        s_sim_hot_swap_latest_int = TB_SIMINT_PLUG_IN;

        //To avoid the user plug and pull sim card very quickly and continously, we set a
        //timer once we get a GPIO int.
        SCI_ChangeTimer(s_sim_hot_swap_stable_timer, SIMHAL_SIM_Hotswap_Stable_TimeOut, SIM_HOTSWAP_STABLE_TIMER_VALUE);
        SCI_ActiveTimer(s_sim_hot_swap_stable_timer);
    }
#endif
    return 1;
}

/**********************************************************************
//    Description:
//     After the sim0 plug in/out gpio stable time out, we notify upper layer.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SIM_Hotswap_Stable_TimeOut(uint32 lparam)
{
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
    TB_MSG msg;
    TB_SIM_INT_T exp_int;

    //SCI_TRACE_LOW:"SIMHAL_SIM0_Hotswap_Stable_TimeOut %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_8565_112_2_17_22_59_15_4490, (uint8 *)"d", s_sim_hot_swap_latest_int);
    exp_int.int_type = s_sim_hot_swap_latest_int;
    msg.message = SIM_EXCEPTION_INT;
#ifdef MULTI_SIM_SYS_QUAD
    msg.wparam = SIM_CARD_FOUR;//the sim3 of openphone is connect to simplugin/out slot.
#else
    msg.wparam = SIM_CARD_ONE;//must be matched with the GPIO
#endif
    msg.lparam = &exp_int;
    DriverNotifySIM(&msg);
#endif
}

/*****************************************************************************/
//  Description:    send the message of the SIM Card status by diag command.
//  Global resource dependence: NONE
//  Author: Mingxuan.Zhu
//  Note:
/*****************************************************************************/
static uint32 SetSIMCardStatus(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8
)
{
#ifndef SIM_PLUG_IN_SUPPORT_BY_CHECK
    typedef enum
    {
        PLUGIN,
        PLUGOUT
    } SIMCARD_STATUS_E;

    uint32  *sim_card_status_ptr = PNULL;
    uint32   sim_card_status = 0;

    sim_card_status_ptr = (uint32 *)(src_ptr + sizeof(MSG_HEAD_T));
    sim_card_status = *sim_card_status_ptr;

    // Build response to channel server
    *dest_ptr = (uint8 *)SCI_ALLOC_BASE(sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);
    SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T *)(*dest_ptr))->len = *dest_len_ptr;

    if(PLUGOUT == sim_card_status) // SIM Card plug-out
    {
        //Send message to SIM
        SIMHAL_SIM_Plugin_GPIO_Callback(PLUGOUT);
    }
    else if(PLUGIN == sim_card_status)  // SD Card plug-in
    {
        //Send message to SIM
        SIMHAL_SIM_Plugin_GPIO_Callback(PLUGIN);
    }

    //SCI_TRACE_LOW:"SIMCard Status: generated SIMCard code=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_8612_112_2_17_22_59_15_4491, (uint8 *)"d", sim_card_status);
#endif

    return 1;
}
#endif

/*****************************************************************************/
//  Description:
//     Clear the global variables because of a different sim card inserted.
//  Global resource dependence: NONE
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMHAL_HotSwap_ClearStaticVariable(SIM_CARD_INDEX_E card_id)
{
    s_sim_atr_interface_bytes_times[card_id] = 1;
    s_sim_atr_ta_with_volt_position[card_id] = 0;
    SIMHAL_SetVoltDetectComplete(card_id, FALSE);
    SIMHAL_SetVoltLimit(card_id, FALSE);
    s_simhal_atr_volt_get_flag[card_id] = FALSE;
    s_simhal_atr_volt_byte[card_id] = 0;
    s_simhal_rx_noise_data[card_id] = 0;
    s_previous_pps_error[card_id] = FALSE;
    s_sim_pps_req_times[card_id] = 0;
    s_sim_cmd_send_times[card_id] = 0;
}

/**********************************************************************
//      Trace the data in the sim rx buffer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_PrintRxBuf(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    if(s_sim_is_ext_dualsim)
    {
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
    }

    //SCI_TRACE_LOW:"SIMHAL%d::Rx data buf(1-16):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_8654_112_2_17_22_59_15_4492, (uint8 *)"ddddddddddddddddd", card_id, s_sim_rx_buf[card_index].buf[0], s_sim_rx_buf[card_index].buf[1], s_sim_rx_buf[card_index].buf[2], s_sim_rx_buf[card_index].buf[3], s_sim_rx_buf[card_index].buf[4], \
                 s_sim_rx_buf[card_index].buf[5], s_sim_rx_buf[card_index].buf[6], s_sim_rx_buf[card_index].buf[7], s_sim_rx_buf[card_index].buf[8], s_sim_rx_buf[card_index].buf[9], s_sim_rx_buf[card_index].buf[10], s_sim_rx_buf[card_index].buf[11], s_sim_rx_buf[card_index].buf[12], \
                 s_sim_rx_buf[card_index].buf[13], s_sim_rx_buf[card_index].buf[14], s_sim_rx_buf[card_index].buf[15]);
    return;
}

/**********************************************************************
//    Description:
//      Trace the data in the sim tx buffer
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMHAL_PrintTxBuf(SIM_CARD_INDEX_E card_id)
{
    uint32 total_len, sent_len;
    uint8 card_index = (uint8)card_id;

    if(s_sim_is_ext_dualsim)
    {
        card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
    }

    total_len = s_sim_tx_buf[card_index].tx_data_len;
    sent_len = s_sim_tx_buf[card_index].cur_pos;

    //SCI_TRACE_LOW:"SIMHAL%d::Tx total len:%d,Sent len:%d,Tx buf(1-16):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_8681_112_2_17_22_59_15_4493, (uint8 *)"ddddddddddddddddddd", card_id, total_len, sent_len, s_sim_tx_buf[card_index].buf[0], s_sim_tx_buf[card_index].buf[1], s_sim_tx_buf[card_index].buf[2], s_sim_tx_buf[card_index].buf[3], \
                 s_sim_tx_buf[card_index].buf[4], s_sim_tx_buf[card_index].buf[5], s_sim_tx_buf[card_index].buf[6], s_sim_tx_buf[card_index].buf[7], s_sim_tx_buf[card_index].buf[8], s_sim_tx_buf[card_index].buf[9], s_sim_tx_buf[card_index].buf[10], s_sim_tx_buf[card_index].buf[11], \
                 s_sim_tx_buf[card_index].buf[12], s_sim_tx_buf[card_index].buf[13], s_sim_tx_buf[card_index].buf[14], s_sim_tx_buf[card_index].buf[15]);
    return;
}

/**********************************************************************
//    Description:
//      Trace the atr data sent by the sim card during the ATR
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_PrintATRDataBuf(SIM_CARD_INDEX_E card_id)
{
    uint32 i = 0;

    //SCI_TRACE_LOW:"SIMHAL::atr data len = %d, data buf(%d-%d):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_8699_112_2_17_22_59_15_4494, (uint8 *)"ddddddddddddddddddd", s_sim_atr_data_len[card_id], i * 16, i * 16 + 15, s_sim_atr_data_buf[card_id][i*16 + 0], s_sim_atr_data_buf[card_id][i*16 + 1], s_sim_atr_data_buf[card_id][i*16 + 2], \
                 s_sim_atr_data_buf[card_id][i*16 + 3], s_sim_atr_data_buf[card_id][i*16 + 4], s_sim_atr_data_buf[card_id][i*16 + 5], s_sim_atr_data_buf[card_id][i*16 + 6], s_sim_atr_data_buf[card_id][i*16 + 7], s_sim_atr_data_buf[card_id][i*16 + 8], s_sim_atr_data_buf[card_id][i*16 + 9], \
                 s_sim_atr_data_buf[card_id][i*16 + 10], s_sim_atr_data_buf[card_id][i*16 + 11], s_sim_atr_data_buf[card_id][i*16 + 12], s_sim_atr_data_buf[card_id][i*16 + 13], s_sim_atr_data_buf[card_id][i*16 + 14], s_sim_atr_data_buf[card_id][i*16 + 15]);

    i++;
    //SCI_TRACE_LOW:"SIMHAL::atr data buf(%d-%d):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_HAL_8706_112_2_17_22_59_15_4495, (uint8 *)"dddddddddddddddddd", i * 16, i * 16 + 15, s_sim_atr_data_buf[card_id][i*16 + 0], s_sim_atr_data_buf[card_id][i*16 + 1], s_sim_atr_data_buf[card_id][i*16 + 2], s_sim_atr_data_buf[card_id][i*16 + 3], s_sim_atr_data_buf[card_id][i*16 + 4], \
                 s_sim_atr_data_buf[card_id][i*16 + 5], s_sim_atr_data_buf[card_id][i*16 + 6], s_sim_atr_data_buf[card_id][i*16 + 7], s_sim_atr_data_buf[card_id][i*16 + 8], s_sim_atr_data_buf[card_id][i*16 + 9], s_sim_atr_data_buf[card_id][i*16 + 10], s_sim_atr_data_buf[card_id][i*16 + 11], s_sim_atr_data_buf[card_id][i*16 + 12], \
                 s_sim_atr_data_buf[card_id][i*16 + 13], s_sim_atr_data_buf[card_id][i*16 + 14], s_sim_atr_data_buf[card_id][i*16 + 15]);
    return;
}

/**********************************************************************
//    Description:
//      trace the register value when exception occurs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMHAL_PrintRegisterBuf(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    SIMDRV_PrintRegisterBuf((uint8)card_index);
}

/**********************************************************************
//    Description:  get SIM ATR raw data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_GetAtrRawData(uint8 card_id, uint16 *len, uint8 **buff)
{
    *len = s_sim_atr_data_len[card_id];

    if(0 != s_sim_atr_data_len[card_id])
    {
        *buff = s_sim_atr_data_buf[card_id];
    }
}

/**********************************************************************
//    Description:  get sim info. ---working volt and atr
//    Global resource dependence :
//    Author:shuyan.hu
//    Note: This is only for DIAG tools!
***********************************************************************/
void SIMHAL_GetAtrBuf(uint8 *buff, uint8 card_id)
{
    SCI_ASSERT(buff != NULL);/*assert verified */
    SCI_MEMCPY(buff, (uint8 *)&s_sim_working_volt[card_id], sizeof(SIMDRV_WORKING_VOLT_E));
    SCI_MEMCPY(buff + sizeof(SIMDRV_WORKING_VOLT_E), "####", 4);

    if(0 != s_sim_atr_data_len[card_id])
    {
        buff[sizeof(SIMDRV_WORKING_VOLT_E)+4] =   s_sim_atr_data_len[card_id];
        SCI_MEMCPY(buff + sizeof(SIMDRV_WORKING_VOLT_E) + 5, s_sim_atr_data_buf[card_id], s_sim_atr_data_len[card_id]);
    }
}

/**********************************************************************
//    Description:
//          Initialize the SIM card Driver, make it ready to warm reset
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_WARM_ResetDriver(SIM_CARD_INDEX_E card_id)
{
    SIM_CARD_INDEX_E card_index = card_id;

    card_index = (SIM_CARD_INDEX_E)SIMHAL_GetChipSimIndex(card_id, FALSE);

    //the SIM card working status
    s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;

    //set the driver variable
    s_sim_reset_times[card_id] = 0;
    s_sim_b_reset_times[card_id] = 0;
    s_sim_pow_reset_times[card_id] = 0;
    s_sim_p_reset_times[card_id] = 0;

    //the SIM card Transfer protocal _protocol
    s_not_t0_protocol_occurs[card_id] = FALSE;
    s_first_protocol_occurs[card_id] = FALSE;

#ifdef SIM_ENHANCED_PPS_SUPPORT//for dma debug
    sim_dma_support[card_id] = FALSE;
#endif

    //initialize the buffer
    s_sim_atr_data_len[card_id] = 0;
    s_have_ack_byte[card_index] = FALSE;
    s_null_byte_count[card_index] = 0;

    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearATRData(card_id);
    ClearPPSInfo(card_id);

    if(s_sim_is_ext_dualsim)
    {
        SIMHAL_Select_Card(card_id);
    }
}

/**********************************************************************
//    Description:
//      Warm RESET the SIM Card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_WARM_ResetSIMCard(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = (uint8)card_id;

    SCI_TRACE_LOW("SIMHAL%d::Warm RESET SIM. %d", card_index, s_card_resetting[card_id]);

    card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
    SIMHAL_Select_Card(card_id);//card select

    //deactive these timer to avoid the timer expiration cause SIM card Reset nest
    SCI_DeactiveTimer(s_sim_pps_timer[card_index]);
    SCI_DeactiveTimer(s_sim_reset_p_timer[card_index]);
    SCI_DeactiveTimer(s_sim_reset_bt_timer[card_index]);
    SCI_DeactiveTimer(s_sim_atr_timer[card_index]);
    SCI_DeactiveTimer(s_sim_decode_atr_timer[card_index]);
    SCI_DeactiveTimer(s_sim_tb_te_timer[card_index]);

    if(SIM_CARD_ONE == card_index)
    {
        SCI_ChangeTimer(s_sim_atr_timer[SIM_CARD_ONE], SIM1_DriverTimerExpired, SIM_ATR_TIMER_VALUE);
    }
    else
    {
        SCI_ChangeTimer(s_sim_atr_timer[SIM_CARD_TWO], SIM2_DriverTimerExpired, SIM_ATR_TIMER_VALUE);
    }

    //the SIM card Transfer protocal _protocol
    s_not_t0_protocol_occurs[card_id] = FALSE;
    s_first_protocol_occurs[card_id] = FALSE;
    //If PPS failure, we will RESET again without PPS.
    //So, keep the s_previous_pps_error value, don't clear.wuding.yang
#ifdef SIM_ENHANCED_PPS_SUPPORT//ywd 2007.2.27
    sim_dma_support[card_id] = FALSE;
#endif
    s_simhal_rx_noise_data[card_id] = 0;
    //initialize the buffer
    s_sim_atr_data_len[card_id] = 0;
    s_have_ack_byte[card_index] = FALSE;
    s_null_byte_count[card_index] = 0;

    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearATRData(card_id);
    ClearPPSInfo(card_id);
    s_sim_rx_buf[card_index].is_locked = FALSE;
    s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;

    SIMDRV_ClearBeforeReset(card_index);

    SIMHAL_SetClockStopLevel(card_id, FALSE);//clear
    s_sim_clk_mode[card_id] = SIMDRV_ConfigSIMClk(card_index);
    s_sim_clk_dvd_value[card_id] = 0xba0;//set as the default value.

    SIMHAL_ActiveSIMCard(card_id);

    SIMDRV_SetSIMRST(card_index, FALSE);//Low the RST pike in 1.8-->3V

#ifdef SIM_6302_SWITCH

    if(s_sim_6302_switch_flag)
    {
        SIM_6302_OUTPUT_E sim_6302_output_index = SIMHAL_6302_Get_Output_Index(card_id);

        if(SIM_6302_CARD_ID_NOT_CONNECT != sim_6302_output_index)
        {
            if(SIM_6302_OUTPUT_0 == sim_6302_output_index)
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_2;//power on
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] |= BIT_0;//CLK the same ad BB
                s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_0;//DATA the same ad BB
            }
            else
            {
                s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM] |= BIT_3;//power on
                s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK] |= BIT_1;//CLK the same ad BB
                s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA] |= BIT_1;//DATA the same ad BB
            }

            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_VSIM, s_simhal_6302_reg_val[SIMHAL_6302_REG_VSIM]);

            SIMDRV_EnableSIMCLK(card_index);//we should enable clk before channel switch in uad-sim via 6302.

            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_CLK, s_simhal_6302_reg_val[SIMHAL_6302_REG_CLK]);
            SIMHAL_6302_WriteSPI(card_id, SIMHAL_6302_REG_DATA, s_simhal_6302_reg_val[SIMHAL_6302_REG_DATA]);
        }
    }

#endif

    SIMDRV_StartToReset(card_index);

    //Start a Timer here, to keep a Tb/Te interval's SIM_RST LOW after SIM_Vcc/CLK on.
    if(SIM_CARD_ONE == card_index)
    {
        SCI_ChangeTimer(s_sim_tb_te_timer[SIM_CARD_ONE], SIM1_RST_After_Tb_or_Te, SIM_RST_TB_TE_TIMER_VALUE);
        SCI_ActiveTimer(s_sim_tb_te_timer[SIM_CARD_ONE]);
    }
    else
    {
        SCI_ChangeTimer(s_sim_tb_te_timer[SIM_CARD_TWO], SIM2_RST_After_Tb_or_Te, SIM_RST_TB_TE_TIMER_VALUE);
        SCI_ActiveTimer(s_sim_tb_te_timer[SIM_CARD_TWO]);
    }

    return;
}

/**********************************************************************
//    Description:  Set sim info about WWT(= 960*WI*Fi/f) in millisecond
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
static void SIMHAL_SetWWT(SIM_CARD_INDEX_E card_id)
{
    uint32 wwt_in_ms = 0;
    uint32 temp_1 = 96;//
    uint32 temp_clk_freq = 325;//SIM CLK 3.25MHz
    uint32 temp_F = 0;//Fi

#ifdef SIM_ENHANCED_PPS_SUPPORT
    temp_F = (uint32)sim_F[card_id];
#else
    temp_F = (uint32)SIM_FACTOR_F_372;//default value
#endif

    //WWT = (960*WI*Fi/f)*1000 ms
    wwt_in_ms = (temp_1 * s_sim_atr_wi[card_id] * temp_F) / temp_clk_freq; //millisecond

    SCI_TRACE_LOW("SIMHAL%d:WI=%d F=%d WWT=%d", card_id, s_sim_atr_wi[card_id], temp_F, wwt_in_ms);

    //SCI_ASSERT(wwt_in_ms);

    s_simhal_wwt_value[card_id] = wwt_in_ms;
}

/**********************************************************************
//    Description:  get sim info about WWT(= 960*WI*Fi/f) in millisecond
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
uint32 SIMHAL_GetWWT(SIM_CARD_INDEX_E card_id)
{
    return s_simhal_wwt_value[card_id];
}

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_DisablePPSFeature(SIM_CARD_INDEX_E card_id)
{
    s_previous_pps_error[card_id] = TRUE;
}

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMHAL_GetParityResetFlag(SIM_CARD_INDEX_E card_id)
{
    return s_sim_p_reset_happen_flag[card_id];
}

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_SetParityResetFlag(SIM_CARD_INDEX_E card_id, BOOLEAN flag)
{
    s_sim_p_reset_happen_flag[card_id] = flag;
}

/**********************************************************************
//    Description:  
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
void SIMHAL_ChangeConvetion(SIM_CARD_INDEX_E card_id)
{
    uint8 card_index = SIMHAL_GetChipSimIndex(card_id, FALSE);
    SCI_TRACE_LOW("SIMHAL%d:ChangeConvetion",card_id);
    SIMDRV_ChangeConvetion(card_index);
}

#ifdef   __cplusplus
}
#endif
