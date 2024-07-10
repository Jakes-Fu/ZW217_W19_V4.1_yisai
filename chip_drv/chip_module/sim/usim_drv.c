/**************************************************************************

  Copyright (C) 2001 SPREADTRUM COMMUNICATION CO.
  All rights reserved.

  This document contains proprietary information blonging to
  SPREADTRUM. Passing on and copying this document, use and
  communication of its contents is not permitted without prior
  writed authorization.

***************************************************************************/

/***************************************************************************

  Reversion information:
  File Name:    usim_drv.c
  Version:      0.1
  Author:       Raislin.Kong
  Date:         2002.2
  Description:  This is the sim driver file
***************************************************************************/
/***************************************************************************

                Edit Histroy
  Date          Name            Description
  --------------------------------------------------------------------------
  2001.2        Raislin.Kong    Create
  2002.10       Raislin.Kong    change the SIM driver and move it under the new
                                driver framework
***************************************************************************/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "com_drvapi.h"
#include "usim_outport.h"
#include "usim_interface.h"
#include "usim_drv.h"
#ifdef PLATFORM_SC8800G
#include "usim_reg_v3.h"
#include "ldo_drvapi.h"
#else
#include "isr_drvapi.h"
#endif


#ifdef USIM_DMA_SUPPORT
/* +binli add dma test */
#include "dma_drv_internal.h"
/* -binli add dma test */
#endif

#ifdef   __cplusplus
extern   "C"
{
#endif
/*  dual card dev binli 20110123 */
/* if we can not find g_usim_drv_log, you can add this boolean which value is FALSE */
extern BOOLEAN g_set_usim_drv_log_ctl;
extern BOOLEAN g_is_set_sim_ins_only;   /*CR206004 Eddie.Wang added for GCF case*/
extern BOOLEAN  DMNV_GetUsimDrvLogFlagFromNV (void); /*CR173855 Eddie.Wang*/
extern void sim_store_atr_raw_data(int32 card_id, uint8 atr_len, uint8*atr_data_ptr);  /*+CR232991 E*/

/* CR218847 remove real nw flag  delete  g_real_nw_opt_enable */
BOOLEAN g_user_mode_flag[USIMDRV_CARD_NUM] = {FALSE, FALSE}; /* cr210334 */
BOOLEAN g_wwt_no_sim_flag[USIMDRV_CARD_NUM]  = {TRUE, TRUE};/* cr210334 */
BOOLEAN g_status_tmer_wwt_value_flag[USIMDRV_CARD_NUM] = {TRUE, TRUE}; /* CR218847 remove real nw flag  */

#define TB_SIM_RESPONSE_NULL_BYTE 0x60
#define SIM_MAX_NULL_BYTE_COUNT 10
SIM_INPUT_BUFFER_INIT (64)    /*CR248506 E Changed form 8 to 64*/
SIM_ISR_WRITE_BUFFER_DEF
SIM_THREAD_READ_BUFFER_DEF

#define USIMDRV_DMA_CLR_ALLTRNSF_INT(chn) {*(volatile uint32*)(DMA_TRANSF_INT_CLR) |= 1<<(chn);}
#define TB_SIM_DMA_MAX_BUF_SIZE 255
/* +binli opt sim */
#define USIM_DEFAULT_WATER_MARK_NUM   3
#define CAL_RX_INT_MARK(data_num) ( (data_num >= USIM_DEFAULT_WATER_MARK_NUM) ? USIM_DEFAULT_WATER_MARK_NUM : data_num);
//#define SET_RX_INT_MARK(sim_register_map,mark) (((sim_register_map)->ctl1 & 0xfff0) | mark)
/* -binli opt sim */
//---------------Local Variable----------------------------
static SCI_TIMER_PTR s_sim_clk_stop_timer[USIMDRV_CARD_NUM] = {NULL, NULL};   //the timer to stop clk
/* +cr206965 */
//record the sim clock stop ability from MF property
static sim_clock_phase_enum s_sim_clk_stop_mode_from_mf[USIMDRV_CARD_NUM] = \
							{SIM_CLOCK_UNKNOWN, SIM_CLOCK_UNKNOWN};
/* -cr206965 */
//工作电压,1.8 3v 5v,分别对应c, b, a类卡
static TB_SIM_WORKING_VOLT_E s_sim_working_volt[USIMDRV_CARD_NUM]; //the working voltage of the SIM
//记录当前卡的状态
static TB_SIM_STATUS_E s_sim_status[USIMDRV_CARD_NUM];   //the SIM driver status
//控制两次连续reset的时间值，这个reset是变换bit的高低位用的
static SCI_TIMER_PTR s_sim_reset_bt_timer[USIMDRV_CARD_NUM] = {NULL, NULL}; //the timer to control the time between to continuious reset(reset to change bit convention) to make
//sure that the ATR data of the first reset has been all reveived
//等待timer,等待ATR的最长时间
static SCI_TIMER_PTR s_sim_atr_timer[USIMDRV_CARD_NUM] = {NULL, NULL};   //the timer to wait the atr
//奇偶校验错误后的reset最长时间
static SCI_TIMER_PTR s_sim_reset_p_timer[USIMDRV_CARD_NUM] = {NULL, NULL};  //the timer to control the time between to continuious reset(reset to change parity check) to make
//sure that the ATR data of the first reset has been all reveived
//如果没有收到指定的个数后，就等待sw1 sw2??
//是已经收到了sw1 sw2，但是并不是期望的长度，使用这个定时器用于确信确实收到的是sw1 sw2
static SCI_TIMER_PTR s_sim_status_word_timer[USIMDRV_CARD_NUM] = {NULL, NULL};   //the timer to help distinguish the status word and the really response data from SIM card.
/*  bo.chen 20070620 add wwt timer   */
static SCI_TIMER_PTR s_sim_wwt_timer[USIMDRV_CARD_NUM] = {NULL, NULL};
static SCI_TIMER_PTR s_sim_cwt_timer[USIMDRV_CARD_NUM] = {NULL, NULL};
/*  bo.chen 20070730 add rx timer out timer */
static SCI_TIMER_PTR s_sim_rx_timeout_timer[USIMDRV_CARD_NUM] = {NULL, NULL};
/*  bo.chen 20070718 add wwt flag to control the timer
    with this flag we will start up wwt timer at the end
    of check receive buffer*/
static BOOLEAN s_sim_wwt_active[USIMDRV_CARD_NUM] = {FALSE, FALSE};
// bwt = block wait time
// 等待block的回应最大时间
static SCI_TIMER_PTR s_sim_bwt_timer[USIMDRV_CARD_NUM] = {NULL, NULL};   //the timer to handle the bwt time-out
//接收的最大时间
static SCI_TIMER_PTR s_sim_wait_resp_timer[USIMDRV_CARD_NUM] = {NULL, NULL};   //the timer to control the time of receiving response data
//timer,wait ldo is valid,then to init the sim control register
static SCI_TIMER_PTR s_sim_init_wait_ldo_timer[USIMDRV_CARD_NUM] = {NULL, NULL};
//timer,wait ldo is valid,then to reset the sim card
static SCI_TIMER_PTR s_sim_reset_wait_ldo_timer[USIMDRV_CARD_NUM] = {NULL, NULL};
static SCI_TIMER_PTR s_sim_fta_test_mode_timer[USIMDRV_CARD_NUM] = {NULL,NULL}; /* cr210334 */
#ifdef PLATFORM_SC8800G
//the register of the SIM interface in SC8800G
static sim_s *s_sim_register_map[USIMDRV_CARD_NUM] = {(sim_s *) SIM0_BASE, (sim_s *) SIM1_BASE};
#else
//0x8500 0000 8800中的地址值,the register of the SIM interface
static sim_s *s_sim_register_map = (sim_s *) SIM_BASE;
#endif
//从rx中收到的内容放入此buf中
static TB_SIM_RX_BUF_T s_sim_rx_buf[USIMDRV_CARD_NUM];    //the rx buf to save the data received from SIM
/* the buffer for the rx_block buf  */
static TB_SIM_RX_BLOCK_BUF_T s_sim_rx_block_buf[USIMDRV_CARD_NUM];
//发送的数据buf
static TB_SIM_TX_BUF_T s_sim_tx_buf[USIMDRV_CARD_NUM];    //the tx buf to save the data that to be sent to SIM
//pps信息
static TB_SIM_PPS_INFO_T  s_pps_info[USIMDRV_CARD_NUM];       //the pps information
//atr 结构
static TB_SIM_ATR_DATA_BUF_T  s_atr_data_buf[USIMDRV_CARD_NUM];       //the atr data of the SIM card
//分频模式
static TB_SIM_CLK_MODE_E s_sim_clk_mode[USIMDRV_CARD_NUM];
//block模式下的接收
static TB_SIM_RX_BLOCK_T s_sim_rx_block[USIMDRV_CARD_NUM];
//block模式下的发送
static TB_SIM_TX_BLOCK_T s_sim_tx_block[USIMDRV_CARD_NUM];
static BOOLEAN s_previous_pps_error[USIMDRV_CARD_NUM];    //is there are any PPS error happen,use this variable to handle the pps error.
static BOOLEAN s_have_ack_byte[USIMDRV_CARD_NUM]; //has the ack byte of the command been reveived
static BOOLEAN s_card_resetting[USIMDRV_CARD_NUM] = {FALSE, FALSE};  //is the card cold resetting.this state can make sure the reset funciton not called interater
static BOOLEAN s_card_warm_resetting[USIMDRV_CARD_NUM] = {FALSE, FALSE};  //is the card warm resetting.in this state,the upper layer cannot call the reset api
static BOOLEAN s_sim_is_processing_cmd[USIMDRV_CARD_NUM] = {FALSE, FALSE};
// only record the number of null(0x60)
static uint8 s_null_byte_count[USIMDRV_CARD_NUM] = {0, 0};                     //the count of null byte(0x60),only receive 10 null byte to notify upper layer once
static BOOLEAN sim_response_data_present[USIMDRV_CARD_NUM]= {FALSE, FALSE};    //whether the response data is present
static BOOLEAN s_have_received_response_data[USIMDRV_CARD_NUM] = {FALSE, FALSE};   //whether the ME has received the response data
//为pcb字段确定N(S)
static uint8 s_pcb_ns[USIMDRV_CARD_NUM] = {1, 1};
static uint8 s_pcb_nr[USIMDRV_CARD_NUM] = {0, 0};
/*  20070709 add s_cur_ns_sent to check the R-block received    */
//static uint8 s_valid_pcb_ns = 0;
static BOOLEAN s_rx_parity_recv[USIMDRV_CARD_NUM] = {FALSE, FALSE};
/*  确定ifsd, default value is 32  */
static uint16 s_sim_ifsd[USIMDRV_CARD_NUM] = {32, 32};
//sim重启的次数
static uint8 s_sim_reset_times[USIMDRV_CARD_NUM];         //the times the SIM has been reset
static uint8 s_sim_pow_reset_times[USIMDRV_CARD_NUM];     //the times that SIM has been reset for power change
static uint8 s_sim_b_reset_times[USIMDRV_CARD_NUM];       //the times that the SIM has been reset for bit convetion and logic level change
static uint8 s_sim_p_reset_times[USIMDRV_CARD_NUM];       //the tiems that the SIM has been reset for parity change
static uint16 s_sim_rxfifo_count[USIMDRV_CARD_NUM];
static uint16 s_sim_rxfifo_int_count[USIMDRV_CARD_NUM];
/*  bo.chen add for (SIMCTL0_B_BIT_CONVENTION | SIMCTL0_B_LOGIC_LEVEL)  */
static uint8 s_sim_bit_converion_value[USIMDRV_CARD_NUM] = {0, 0};
/*  bo.chen add wwt timer value according to WI */
static uint32 s_wwt_timer_value[USIMDRV_CARD_NUM] = \
							{SIM_STATUS_WORD_TIMER_VALUE, SIM_STATUS_WORD_TIMER_VALUE};

static uint32 s_cwt_timer_value[USIMDRV_CARD_NUM] = \
							{SIM_STATUS_WORD_TIMER_VALUE, SIM_STATUS_WORD_TIMER_VALUE};

//指令长度
static uint16 s_sim_tx_instr_len[USIMDRV_CARD_NUM];       //the length of the instruction inlcuding data
static uint16 s_sim_rx_rexpect_len[USIMDRV_CARD_NUM];     //the expected data length for the instruction

//控制block重发送的次数
static uint8  s_sim_block_resent_count[USIMDRV_CARD_NUM] = {0, 0}; //the resent count

//callback函数
static TB_CALLBACK s_sim_call_back_func[USIMDRV_CARD_NUM] = {NULL, NULL};
/*  record the class from sim task  */
static uint8    s_sim_instr_class[USIMDRV_CARD_NUM] = {0x00, 0x00};
// set SIM_WAIT_RESP_TIMER_VALUE
static uint32 s_sim_wait_resp_timer_value[USIMDRV_CARD_NUM] = {0, 0};
/* +interrupt_refactor_dev */
TB_SIM_DATA_BUF_T s_uicc_data_buf[USIMDRV_CARD_NUM];
/* -interrupt_refactor_dev */
static BOOLEAN s_is_reset_flag_set[USIMDRV_CARD_NUM] = {FALSE, FALSE}; /* cr204609 */
//见11.11 p21
static uint8 s_default_pps_data[TB_SIM_PPS_DEFAULT_SPEED] = {0xff,0x00,0xff};
static uint8 s_enhanced_pps_data1[TB_SIM_PPS_ENHANCED_SPEED] = {0xff,0x10,0x94,0x7b}; //512,8
static uint8 s_enhanced_pps_data2[TB_SIM_PPS_ENHANCED_SPEED] = {0xff,0x10,0x95,0x7a};//512,16
static uint8 s_enhanced_pps_data3[TB_SIM_PPS_ENHANCED_SPEED] = {0xff,0x10,0x96,0x79};//512,32

//7816-3 p12
static const uint16 s_clk_dvd_factors[] = {2,4,8,16,32,64,128,256};
static const uint16 s_trans_factor_F[16] = {372,372,558,744,1116,1488,1860,372,372,512,768,1024,1536,2048,372,372};
static const uint8 s_trans_factor_D[16] = {1,1,2,4,8,16,32,1,12,20,1,1,1,1,1,1};

//各种不同block之间的序列
static const BLOCK_VALIDITY_E s_sim_check_block[BLOCK_TYPE_TOTAL_NUM][BLOCK_TYPE_TOTAL_NUM] =
    /*+----------+-----+-----+-----+-----+-----------------------------------------------------------------------------------------+*/
    /*-------recive-|I(N(s),0)     |I(N(s),1)      |R(OK)          |R(ERROR)      |S(REQ)          |S(REP)          |NOT_DEF       */
    /*-----------+-----+-----+-----+-----+-----+-----------------------------------------------------------------------------------+*/
    /*send  */
{
    /*I(N(s),0)*/   {BLOCK_VALID,   BLOCK_SEND_R,  BLOCK_INVALID,   BLOCK_RESEND,  BLOCK_SEND_SREP, BLOCK_INVALID,   BLOCK_INVALID},
    /*I(N(s),1)*/   {BLOCK_INVALID, BLOCK_INVALID, BLOCK_SEND_NEXT, BLOCK_RESEND,  BLOCK_SEND_SREP, BLOCK_INVALID,   BLOCK_INVALID},
    /*R(OK)*/       {BLOCK_VALID,   BLOCK_SEND_R,  BLOCK_INVALID,   BLOCK_INVALID, BLOCK_SEND_SREP, BLOCK_INVALID,   BLOCK_INVALID},
    /*R(ERROR)*/    {BLOCK_VALID,   BLOCK_SEND_R,  BLOCK_SEND_NEXT, BLOCK_RESEND,  BLOCK_SEND_SREP, BLOCK_INVALID,   BLOCK_INVALID},
    /*S(REQ)*/      {BLOCK_INVALID, BLOCK_INVALID, BLOCK_INVALID,   BLOCK_INVALID, BLOCK_INVALID,   BLOCK_SEND_SREP, BLOCK_INVALID},
    /*S(REP)*/      {BLOCK_VALID,   BLOCK_SEND_R,  BLOCK_SEND_NEXT, BLOCK_INVALID, BLOCK_SEND_SREP, BLOCK_INVALID,   BLOCK_INVALID},
    /*NOT_DEF*/     {BLOCK_INVALID, BLOCK_INVALID, BLOCK_INVALID,   BLOCK_INVALID, BLOCK_SEND_SREP, BLOCK_INVALID,   BLOCK_INVALID}
};

//status conditions returned by the uicc
static const uint8 status_word[][2]=
{
    /* +cr119678 */
    /* bin.li 93 00 is consider as a status word in stk/usat */
    //#ifdef SIM_APPLICATION_TOOLKIT
    {0x93,0x00},
    //#endif
    /* -cr119678 */
    {0x90,0x00},
    {0x91,0xff},
    {0x61,0xff},
    {0x6c,0xff},
    {0x62,0x00},
    {0x62,0x81},
    {0x62,0x82},
    {0x62,0x83},
    {0x63,0xcf},
    {0x64,0x00},
    {0x65,0x00},
    {0x65,0x81},
    {0x67,0x00},
    {0x67,0xff},
    {0x6b,0x00},
    {0x6d,0x00},
    {0x6e,0x00},
    {0x6f,0x00},
    {0x6f,0xff},
    {0x68,0x00},
    {0x68,0x81},
    {0x68,0x82},
    {0x69,0x00},
    {0x69,0x81},
    {0x69,0x82},
    {0x69,0x83},
    {0x69,0x84},
    {0x69,0x85},
    {0x69,0x86},
    {0x69,0x99},        /* CR255704 */
    {0x6a,0x80},
    {0x6a,0x81},
    {0x6a,0x82},
    {0x6a,0x83},
    {0x6a,0x86},
    {0x6a,0x87},
    {0x6a,0x88},
    {0x98,0x02},
    {0x98,0x04},
    {0x98,0x08},
    {0x98,0x10},
    {0x98,0x40},
    {0x98,0x50},
    {0x98,0x62},
    {0x9e,0xff},
    {0x9f,0xff},
    {0x92,0xff},
    {0x92,0x40},
    {0x94,0x00},
    {0x94,0x02},
    {0x94,0x04},
    {0x94,0x08}
};


// add by bo.chen
// check if TCK is send 7816-3 p11 6.4.5
static BOOLEAN s_tck_will_be_sent[USIMDRV_CARD_NUM] = {FALSE, FALSE};

//bo.chen
#define SIM_ENHANCED_PPS_SUPPORT
// bo.chen 20051205 add log for authentication
static BOOLEAN s_auth_inited[USIMDRV_CARD_NUM] = {FALSE, FALSE};
// bo.chen 20051205 add log for authentication
/*+NEWMS00106599 E*/
#define SIM_MAX_JUNK_DATA_NUM  100  /*to determine if the sim-out */
static uint16  s_sim_junk_data_num[USIMDRV_CARD_NUM] = {0, 0};  /*Junk data generated by the short-circuit of I/O and GND*/
static uint16  s_sim_max_junk_data_num[USIMDRV_CARD_NUM] = {SIM_MAX_JUNK_DATA_NUM, SIM_MAX_JUNK_DATA_NUM};
static volatile BOOLEAN  s_is_sim_out[USIMDRV_CARD_NUM] = {FALSE, FALSE}; /*TRUE:SIM is pulled out*/
/*NEWMS00106599 E*/

#ifdef USIM_DMA_SUPPORT
static SCI_TIMER_PTR s_sim_dma_byte_timer[USIMDRV_CARD_NUM] = {NULL, NULL};
static BOOLEAN sim_dma_support[USIMDRV_CARD_NUM] = {FALSE,FALSE};//SIM DMA channel enable or not
static DMA_CHNCFG_T sim_rx_dma_cfg[USIMDRV_CARD_NUM];//SIM RX DMA channel config
static DMA_CHNCFG_T sim_tx_dma_cfg[USIMDRV_CARD_NUM];//SIM TX DMA channel config
static uint16 s_sim_dma_callback_count[USIMDRV_CARD_NUM] = {0, 0};
static uint16 s_sim_dma_data_count[USIMDRV_CARD_NUM] = {0, 0};


/**********************************************************************
//    Description:
//      SIM DMA TX channel enable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_TX_Enable (int32 card_id);

/**********************************************************************
//    Description:
//      SIM DMA RX channel enable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_RX_Enable (int32 card_id);

/**********************************************************************
//    Description:
//      SIM DMA TX channel disable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_TX_Disable (int32 card_id);

/**********************************************************************
//    Description:
//      SIM DMA RX channel disable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_RX_Disable (int32 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Tx Empty Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIMTXEmptyInt (int32 card_id);

/**********************************************************************
//    Description:
//      Enable SIM Tx Empty Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_EnableSIMTXEmptyInt (int32 card_id);

/**********************************************************************
//    Description:
//      Enable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_EnableSIMRXInt (int32 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIMRXInt (int32 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Tx Parity Err Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIM_TxParityInt (int32 card_id);

/**********************************************************************
//    Description:
//      Enable SIM Tx Parity Err Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_EnableSIM_TxParityInt (int32 card_id);

/**********************************************************************
//    Description: SIM DMA TX channel config
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void USIMDRV_DMA_TX_Cfg (int32 card_id, DMA_CTL_CFG_PTR tx_dma_cfg);

/**********************************************************************
//    Description: SIM DMA RX channel config
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void USIMDRV_DMA_RX_Cfg (int32 card_id, DMA_CTL_CFG_PTR rx_dma_cfg);

/**********************************************************************
//    Description: Send a S-Block.In following cases, a S-block should be sent
//    1. When want to sent an S(...request)
//    2. When an S(...request) has been sent and either a BWT time-out occurs or the
//       received response is not an S(...response),S(...request) should be resent.
//    3. When received an S(...request),an (...response)shoud be send.
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendSBLOCK (
    int32 card_id,
    SEND_SBLOCK_REASON_E send_reason,
    TB_SIM_S_BLOCK_TYPE_E sblock_type,
    uint8 info_data);

/**********************************************************************
//    Description: Send a r-block.In following cases, a r-block should be sent:
//    1. When an I-block has been sent and a BWT time out occurs or an
//       invalid block has been received.
//    2. When an R-Block was sent and an invalid block is received or BWT time-out
//    3. When an S(...response) has been sent and an invalid block is received or
//       BWT time-out.
//    4. When received an I(1) block
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendRBlock (
    int32 card_id,
    SEND_RBLOCK_REASON_E send_reason,
    TB_SIM_R_BLOCK_TYPE_E   error_code
);

/**********************************************************************
//    Description: Check if the Received block is valid in DMA mode
//    Global resource dependence :
//    Author:Edie.Wang
//    Note:
***********************************************************************/
static BOOLEAN DMA_CheckBlockValid (
    int32 card_id,
    TB_SIM_TX_BLOCK_T *tx_block,
    TB_SIM_RX_BLOCK_T *rx_block
);

/**********************************************************************
 //    Description: Check the Received buf and sent all received data up to Task
//    when all the data from SIM Card has been received
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_CheckReceiveBuf (	int32 card_id);

/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX interrupt happen (ONLY for T=0)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_TX_Callback_T0 (int32 card_id,uint32 num);

/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX interrupt happen (ONLY for T=1)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_TX_Callback_T1 (int32 card_id,uint32 num);

/**********************************************************************
//    Description: Called when Complement of INS or complement of INS + 1 is responsed from SIM
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static BOOLEAN DMA_SendOneByteToSIM (int32 card_id,uint32 num);

/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM1_DMA_TX_Callback (int32 card_id,uint32 num);
/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM2_DMA_TX_Callback (int32 card_id,uint32 num);

/**********************************************************************
//    Description: Callback function DMA SIM RX interrupt happen (ONLY for T=0 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_RX_Callback_T0 (int32 card_id,uint32 num);

/**********************************************************************
//    Description: Callback function DMA SIM RX interrupt happen (ONLY for T=1 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_RX_Callback_T1 (int32 card_id,uint32 num);

/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM RX channel when DMA SIM RX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM1_DMA_RX_Callback (uint32 num);
/**********************************************************************
//    Description:
//    This is the Callback function of DMA SIM RX channel when DMA SIM RX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM2_DMA_RX_Callback (uint32 num);

/**********************************************************************
//    Description:
//  clean all dma int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_ClearAllInt (int32 card_id);

/**********************************************************************
//    Description:
//  Init the DMA config when SIM init.
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM_DMA_Init (int32 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIMRXInt (int32 card_id);

/**********************************************************************
//    Description:
//      Disable SIM Tx Parity Err Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIM_TxParityInt (int32 card_id);

/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card to start an operation on
//    The SIM card we needed
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
SIM_SEND_INSTR_RESULT_E USIM_DMA_SendInstrCode (
    int32 card_id,
    TB_USIM_APDU_T *apdu_ptr
    );

/**********************************************************************
//    Description: Send a Instruction to the SIM card(only for T0 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendInstrCode_T0 (int32 card_id, TB_USIM_APDU_T *apdu_ptr);

/**********************************************************************
//    Description: Send a Instruction to the SIM card(only for T1 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendInstrCode_T1 (int32 card_id,TB_USIM_APDU_T *apdu_ptr);

/**********************************************************************
//    Description: The funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim1DMATimerExpired (
    uint32 lparam   //the function's input parameter
);
/**********************************************************************
//    Description: The funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void DMATimerExpired (
    uint32 lparam   //the function's input parameter
);
#endif


//---------------Local Function declare--------------------
/**********************************************************************
//    Description:
//      Initialize the SIM card Driver,make it ready to work when needed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ResetDriver (int32 card_id);
/**********************************************************************
//    Description:
//      The card is current in,when the power is on,if there is a sim
//    card exist,the ARM SIM will send a interupt to tell the SIM layer
//    that the SIM card is in,tell other layer that card is in
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardInIntP (int32 card_id);

/**********************************************************************
//    Description:
//      The card is current in,when the power is on,if there is a sim
//    card exist,the ARM SIM will send a interupt to tell the SIM layer
//    that the SIM card is in,tell other layer that card is in
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardOutIntP (int32 card_id);

/**********************************************************************
//    Description:
//      when the SIM card start to ATR,the ARM will send a Early ATR
//    interrupt,this is the interrupt handler function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void EarlyATRIntP (int32 card_id);

/**********************************************************************
//    Description:
//      When the SIM has beening Actived the ARM CPU will send a Active
//    done interrupt,this is the interrupt function.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ActiveDoneIntP (int32 card_id);
/**********************************************************************
//    Description:
//      if there are Parity Error when receive data from sim card,the
//    ARM will send a RX Parity Error interrupt,this is the interrupt
//    handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxParityErrorIntP (int32 card_id);

/**********************************************************************
//    Description:
//      if there are Parity Error when transmit data to sim card,the
//    ARM will send a TX Parity Error interrupt,this is the interrupt
//    handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TxParityErrorIntP (int32 card_id);

/**********************************************************************
//    Description:
//      if the SIM card do not response for sometime,the ARM CPU will
//    send a CARD UNRESPONSE interrtupt,this is the interrupt handle
//    function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardUnResponseIntP (int32 card_id);

/**********************************************************************
//    Description:
//      ARM receive data from SIM card and together it to the RX fifo
//    when the data up to the RX_mark byte,ARM CPU will send a RX full
//    interrupt to tell the drive get data from the fifo.This is the
//    interrupt handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxFifoFullIntP (int32 card_id);
/* +interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      the funciton to proccess the HISR call back func
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void  SIMIntHisrFunc (uint32 count, void *data);

/**********************************************************************
//    Description:
//      ARM receive data from SIM card and together it to the RX fifo
//    when the data up to the RX_mark byte,ARM CPU will send a RX full
//    interrupt to tell the drive get data from the fifo.This is the
//    interrupt handle function
//    Global resource dependence :
//    Author:binli.kong
//    Note:
***********************************************************************/
static void RxFifoFullHisrP (TB_SIM_DATA_BUF_T *sim_int_data);
/**********************************************************************
//    Description:
//      When the drive send data to the sim card.It will write the data
//    to the SIM TX ARM fifo,and let the ARM done the Transimit job.
//    when the data in the fifo below the tx fifo mark byte,The arm
//    cpu will send a TX empty interrupt.and we can write more data to
//    the TX fifo or finish the Transmit.
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void TxEmptyHisrP (int32 card_id);
/* -interrupt_refactor_dev */
/**********************************************************************
//    Description:
//    save the received data to the rx buffer and check the data in the buffer
//  only for T0 Protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxFifoFullIntP_T0 (TB_SIM_DATA_BUF_T *sim_int_data);
/**********************************************************************
//    Description:
//    save the received data to the rx buffer and check the data in the buffer
//  only for T1 Protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxFifoFullIntP_T1 (TB_SIM_DATA_BUF_T *sim_int_data);
/**********************************************************************
//    Description:
//      When the drive send data to the sim card.It will write the data
//    to the SIM TX ARM fifo,and let the ARM done the Transimit job.
//    when the data in the fifo below the tx fifo mark byte,The arm
//    cpu will send a TX empty interrupt.and we can write more data to
//    the TX fifo or finish the Transmit.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TxEmptyIntP (int32 card_id);
/**********************************************************************
//    Description:
//      the really function taht do the  data transmit to the SIM CARD
//  only for T0 protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TransmitInInt_T0 (int32 card_id);
/**********************************************************************
//    Description:
//      the really function taht do the  data transmit to the SIM CARD
//  only for T1 protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TransmitInInt_T1 (int32 card_id);

/**********************************************************************
//    Description:
//      Clear the Tx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearTxBuf (int32 card_id);

/**********************************************************************
//    Description:
//      Clear the Rx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearRxBuf (int32 card_id);
/**********************************************************************
//    Description:
//      Clear the Tx block of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearTxBlock (int32 card_id);
/**********************************************************************
//    Description:
//      Clear the Rx block of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearRxBlock (int32 card_id);
/**********************************************************************
//    Description:
//      Clear ATR data structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearATRData (int32 card_id);

/**********************************************************************
//    Description:
//      Clear the PPS info structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearPPSInfo (int32 card_id);

/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card(only for T0 protocol)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendInstrCode_T0 (int32 card_id,TB_USIM_APDU_T *apdu_ptr);
/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card(only for T1 protocol)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendInstrCode_T1 (int32 card_id,TB_USIM_APDU_T *apdu_ptr);

/**********************************************************************
//    Description:
//      Send a byte data of TX buf to the SIM card,not the all left TX
//    buf data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN SendOneByteToSIM (int32 card_id);  //return value:TRUE:all the TX have
// been transmit to SIM
//FALSE:there still exist TX data need
// to be transmit to the SIM
/**********************************************************************
//    Description:
//      When the ATR data show the SIM card need a PPS procedure before
//    actived,Use this function to send the PPS procedure data to the
//    SIM card to start the PPS procedure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendPPSRequest (
    int32 card_id,
    const uint8 *pps_req, //in:the ppas request data to be send
    uint16 req_len  //the data len
);

/**********************************************************************
//    Description:
//      check the Received buf and sent all received data up to Task
//    when all the data from SIM Card has been received
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CheckReceiveBuf (int32 card_id);
/**********************************************************************
//    Description:
//      Check whether the received block is an valid block
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static BOOLEAN CheckBlockValid (int32 card_id,TB_SIM_TX_BLOCK_T *tx_block,TB_SIM_RX_BLOCK_T *rx_block);
/**********************************************************************
//    Description:
//     check the Received n(s)
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static BOOLEAN CheckNRValid (int32 card_id,TB_SIM_RX_BLOCK_T *rx_block);
/**********************************************************************
//    Description:
//     check the length of s-block r-block and i-block
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static BOOLEAN CheckBlockLenValid (
    int32 card_id,
    uint16                      *len_ptr,
    TB_SIM_BASE_BLOCK_INFO_T    *rx_block
);

/**********************************************************************
//      When the drive have receive all the response data of an operation
//    from SIM card,the drive will send the response data to manager
//    layer to process it
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendResponseUp (int32 card_id);
/**********************************************************************
//    Description:
//      resend the instruction previous send and now saved in the tx buffer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RedoPreInstr (int32 card_id,BOOLEAN need_modify_p3,uint8 p3_byte);



/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim1DriverTimerExpired (
    uint32 lparam   //the function's input parameter
);
/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim2DriverTimerExpired (
    uint32 lparam   //the function's input parameter
);
/**********************************************************************
//    Description:
//      When BWT time-out or received an Invalid block,handler these errors
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void ErrorHandler (
    int32 card_id,
    T1_ERROR_CODE_E error_code
);

/**********************************************************************
//    Description:
//      Notify the SIM layer the exeption interrupt
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void NotifyExceptionError (
    int32 card_id,
    TB_SIM_EXCEPTION_TYPE_E exp_type //the exeption interrupt type
);

/**********************************************************************
//    Description:
//      report the SIM card insert status to the SIM manager task
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void ReportCardStatus (
    int32 card_id,
    TB_SIM_INSERT_STATUS_E insert_status   //the SIM card insert status
);

/**********************************************************************
    Description:
      report the SIM card reset status to the SIM task
    Global resource dependence :
    Author:Raislin.kong
    Note:
***********************************************************************/
static void ReportCardResetStatus (
    int32 card_id,
    TB_SIM_RESET_STATUS_E reset_status   /*the SIM card reset status*/
);
/**********************************************************************
    Description:
      Active the SIM card by start clock
    Global resource dependence :
    Author:Raislin.kong
    Note:
***********************************************************************/
static void ActiveSIMCard (int32 card_id);

/**********************************************************************
//    Description:
//      Reset the SIM card with a new working voltage when the SIM card
//    is not working well at the beignning,but there are reset times limit,
//    if the rest time exeed the times limit,the SIM card will be Rejected
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN ResetSIMCard (   //return value:the SIM card reset succeed or not.
    int32 card_id,
    TB_SIM_RESET_TYPE_E reset_type  //the reset type
);

/**********************************************************************
//    Description:
//      Set the logic level in the clock pin when the clock is disable
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SetClockStopLevel (int32 card_id,BOOLEAN bHighLevel);
/**********************************************************************
//    Description:
//      Make the SIM card to the Idle state,stop the clock of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void IdleSIMCard (int32 card_id);
/**********************************************************************
//    Description:
//      to identify a two byte sequence data can be status word pair or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN IsStatusWord (
    uint8 byte1,    //the byte1 of the data
    uint8 byte2     //the byte 2 of the data
);

/**********************************************************************
//    Description:
//      Calculate edc of one block
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static uint8 GetEDC (uint8 *block_data,uint16 data_len);
/**********************************************************************
//    Description:
//      Decode the PCB byte to get the base information of the block
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static BOOLEAN GetBlockBaseInfo (int32 card_id,uint8 pcb_byte,TB_SIM_BASE_BLOCK_INFO_T *block);
/**********************************************************************
//    Description:
//    get the detail block type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BLOCK_TYPE_E GetBlockType (TB_SIM_BASE_BLOCK_INFO_T block);
/**********************************************************************
//    Description:
//      Decode one byte ATR data
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void DecodeATRData (int32 card_id,uint8 cur_pos,uint8 *pdata,uint16 *except_len);
/**********************************************************************
//    Description:
//      judge whether the supply voltage switching is need,whether this
//      switching is supported by the ME
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void CheckSupplyVol (
                            TB_SIM_WORKING_VOLT_E cur_work_vol,
                            uint8 card_support_vol,
                            BOOLEAN *change_is_need,
                            BOOLEAN *change_is_support);
/**********************************************************************
//    Description:
//      When received an 0x61 byte,send an get response command to the sim card
//    to get the response data
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SendGetRespCmd (int32 card_id,uint8 sw2_byte);
/**********************************************************************
//    Description:
//    Calculate the WWT from the WI(T0)
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static uint32 GetT0WWT (int32 card_id,uint8 WI);
/**********************************************************************
//    Description:
//    Calculate the CWT from the CWI(T1) cwt= (11 + (1<<cwi)) etu
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static uint32 GetT1CWT (int32 card_id,uint8 CWI);

/**********************************************************************
//      Calculate the BWT from the BWI
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static uint32 GetBWT (int32 card_id,uint8 BWI);
/**********************************************************************
//    Description:
//    Send a r-block.In following cases, a r-block should be sent:
//    1. When an I-block has been sent and a BWT time out occurs or an
//       invalid block has been received.
//    2. When an R-Block was sent and an invalid block is received or BWT time-out
//    3. When an S(...response) has been sent and an invalid block is received or
//       BWT time-out.
//    4. When received an I(1) block
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SendRBlock (
    int32 card_id,
    SEND_RBLOCK_REASON_E send_reason,
    TB_SIM_R_BLOCK_TYPE_E error_code
);
/**********************************************************************
//    Description:
//    Send a S-Block.In following cases, a S-block should be sent
//    1. When want to sent an S(...request)
//    2. When an S(...request) has been sent and either a BWT time-out occurs or the
//       received response is not an S(...response),S(...request) should be resent.
//    3. When received an S(...request),an (...response)shoud be send.
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SendSBLOCK (int32 card_id,
                        SEND_SBLOCK_REASON_E send_reason,   //the reason to send s-block
                        TB_SIM_S_BLOCK_TYPE_E sblock_type,  //sblock type
                        uint8 info_data         //data in the info data field
                       );
/*****************************************************************************/
//  Description:  The driver call the callback function to notify the transport
//                layer what event has happened.
//  Author:Yongxia.zhang
//  Note:
/*****************************************************************************/
static void DriverCallBack (int32 card_id, TB_MSG *msg);


extern void SCI_SIM_EnableDeepSleep (uint32 mode);
extern BOOLEAN sim_is_in_process_reset (int32 card_id);
extern BOOLEAN sim_get_is_need_pps (int32 card_id);

#ifdef SIM_SUPPORT_1D8V_3V
/**********************************************************************
//    Description:
//      According the current supply voltage to get the next switch class
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN GetNextSupplyVoltage (int32 card_id,TB_SIM_WORKING_VOLT_E cur_work_vol,TB_SIM_WORKING_VOLT_E *next_work_vol);
/**********************************************************************
//    Description:
//      Set the working voltage of the sim card.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SetSIMPower (int32 card_id,TB_SIM_WORKING_VOLT_E work_vol);
#endif

/**********************************************************************
//    Description:
//      go on the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim1InitDrvAfterLdoValid (
    uint32 lparam   //the function's input parameter
);
/**********************************************************************
//    Description:
//      go on the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim2InitDrvAfterLdoValid (
    uint32 lparam   //the function's input parameter
);
/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Arthor:bin.li
//    Note:
***********************************************************************/
static void Sim1ResetAfterLdoValid (
    uint32 lparam   //the function's input parameter
);
/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Arthor:bin.li
//    Note:
***********************************************************************/
static void Sim2ResetAfterLdoValid (
    uint32 lparam   //the function's input parameter
);
/**********************************************************************
//    Description:
//    handle S-Block request/rsp from usim
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static void HandleUICCSBlock (
    int32 card_id,
    TB_SIM_RX_BLOCK_T *rx_block
);
/**********************************************************************
//    Description:
//    reset the ns&nr and the buffer we need send after reasync is done
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static void HandleResyncRsp (
    int32 card_id
);

//--------------------------functions define-------------------------------
/* add macro for rx buf print */
#define USIM_PRINT_RX_BUF(_CARD_NUM) \
    {\
        uint8  rx_buf_idx = 0; \
        while(s_sim_rx_buf[_CARD_NUM].cur_pos > rx_buf_idx *16) \
        { \
            SCI_TRACE_LOW("USIMDRV:[%d]:DATA::RX 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X", _CARD_NUM + 1,s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 0],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 1],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 2],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 3], s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 4],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 5],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 6],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 7], s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 8],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 9],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 10],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 11], s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 12],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 13],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 14],s_sim_rx_buf[_CARD_NUM].buf[rx_buf_idx*16 + 15]);\
            rx_buf_idx++; \
            if(16 <= rx_buf_idx ) \
            { \
                break; \
            } \
        } \
    }
/* +interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      the funciton to proccess the HISR call back func
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void SIMIntHisrFunc (uint32 count, void *data)
{
    uint32 card_id = 0;
    TB_SIM_DATA_BUF_T  value ;

    /* SIM0 and SIM1 share the same HISR task, all transactions of all cards
       will be handled here */
    for (card_id = 0; card_id < USIMDRV_CARD_NUM; card_id++)
    {
        SCI_MEMSET(&value, 0x0, sizeof(TB_SIM_DATA_BUF_T));

        /* Set the card ID first, which will be handled */
        value.card_id = card_id;

        /* Handle all transaction of the card */
        while (!SimthreadReadBuffer ((TB_SIM_DATA_BUF_T*)(&value)))
        {
            switch (value.int_type)
            {
                 case SIMICLR_B_RX_PARITY_ERR:
                    RxParityErrorIntP(value.card_id);
                    break;

                 case SIMICLR_B_TX_PARITY_ERR:
                    TxParityErrorIntP(value.card_id);
                    break;

                 case SIMICLR_B_UNRESP_CARD:
                    RxFifoFullHisrP(&value);
                    break;

                 case SIMICLR_B_CARD_IN:
                    CardInIntP(value.card_id);
                    break;

                case SIMICLR_B_EARLY_ATR:
                    EarlyATRIntP(value.card_id);
                    break;

                 case SIMICLR_B_ACTIVE_DONE:
                    ActiveDoneIntP(value.card_id);
                    break;

                case SIMICLR_B_CARD_OUT:
                    CardOutIntP(value.card_id);
                    break;

                case  SIMSTS0_B_RX_FIFO_FULL:
                    RxFifoFullHisrP(&value);
                    break;

                case SIMSTS0_B_TX_FIFO_EMPTY:
                    TxEmptyHisrP(value.card_id);
                    break;

               default:
                     break;
            }
        }
    }

    return;
}
/* -interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim1SetClkStopMode (
    uint32 lparam   //the function's input parameter
)
{
    IdleSIMCard(USIMDRV_CARD_ONE);
    SCI_SIM_EnableDeepSleep (1);
}
/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim2SetClkStopMode (
    uint32 lparam   //the function's input parameter
)
{
    IdleSIMCard(USIMDRV_CARD_TWO);
    SCI_SIM_EnableDeepSleep (1);
}
/* +interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      This is the function that dispatch different SIM interrupt to
//    their Handle function
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
ISR_EXE_T SIM_InterruptHandler (uint32 lparam)
{
    //中断处理函数
    TB_SIM_DATA_BUF_T sim_data;
    uint32 card_id = 0x00;
    uint32 isr_status = 0;
    volatile uint32 register_value = 0x00;
    uint32 sim_enable_flag = 0x00;

    /* SIM0 and SIM1 share the same interrupt resource on 8800G,so all SIM
    controllers have to be checked to make known where the interrupt come from */
    for (card_id = 0; card_id < USIMDRV_CARD_NUM; card_id++)
    {
        /* check if the card module is enable */
        if (0 == card_id)
        {
            register_value =  CHIP_REG_GET (GR_GEN0);
            sim_enable_flag = (register_value & GEN0_SIM0_EN);
        }
        else
        {
            register_value =  CHIP_REG_GET (GR_GEN0);
            sim_enable_flag = (register_value & GEN0_SIM1_EN);
        }

        if (!sim_enable_flag)
        {
            /* if some module disable ,do nothing and skip to handle the next one */
            continue;
        }

        SCI_MEMSET(&sim_data, 0x0, sizeof(TB_SIM_DATA_BUF_T));

        //There is RX parity error interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_RX_PARITY_ERR)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_RX_PARITY_ERR))
        {
            sim_data.int_type = SIMIE_B_RX_PARITY_ERR;
            sim_data.card_id = card_id;
            SimIsrWriteBuffer(sim_data);

            s_sim_register_map[card_id]->iclr = SIMICLR_B_RX_PARITY_ERR;
        }

        //There is TX parity error interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_TX_PARITY_ERR)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_TX_PARITY_ERR))
        {
            //process the TX parity Error interrupt
            sim_data.card_id = card_id;
            sim_data.int_type = SIMIE_B_TX_PARITY_ERR;
            SimIsrWriteBuffer(sim_data);
            //clear the TX parity Error interrupt
            s_sim_register_map[card_id]->iclr = SIMICLR_B_TX_PARITY_ERR;
        }

        //There is Card Un response interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_UNRESP_CARD)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_UNRESP_CARD))
        {
            //process the Card Un response interrupt
            CardUnResponseIntP(card_id);

            //clear the Card Un response interrupt
            s_sim_register_map[card_id]->iclr = SIMICLR_B_UNRESP_CARD;
        }

        //There is Card in interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_CARD_IN)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_CARD_IN))
        {
            //Process the Card in interrupt
            sim_data.card_id = card_id;
            sim_data.int_type = SIMICLR_B_CARD_IN;
            SimIsrWriteBuffer(sim_data);
            //CardInIntP();
            //clear the Card in interrupt
            s_sim_register_map[card_id]->iclr = SIMICLR_B_CARD_IN;
        }

        //There is Early ATR interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_EARLY_ATR)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_EARLY_ATR))
        {
            //Process the Early ATR interrupt
            sim_data.card_id = card_id;
            sim_data.int_type = SIMICLR_B_EARLY_ATR;
            SimIsrWriteBuffer(sim_data);
            //clear the Earlu ATR interrupt
            s_sim_register_map[card_id]->iclr = SIMICLR_B_EARLY_ATR;
        }

        //There is Active done interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_ACTIVE_DONE)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_ACTIVE_DONE))
        {
            //process the Active done interrupt
            sim_data.card_id = card_id;
            sim_data.int_type = SIMICLR_B_ACTIVE_DONE;
            SimIsrWriteBuffer(sim_data);
            //clear the interrupt
            s_sim_register_map[card_id]->iclr = SIMICLR_B_ACTIVE_DONE;
        }

        //There is card out interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_CARD_OUT)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_CARD_OUT))
        {
            //Process the Card out interrupt
            sim_data.card_id = card_id;
            sim_data.int_type = SIMICLR_B_CARD_OUT;
            SimIsrWriteBuffer(sim_data);
            //clear the Card out interrupt
            s_sim_register_map[card_id]->iclr = SIMICLR_B_CARD_OUT;
        }
    /* +interrupt_refactor_dev */
        //There is the TX Empty interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_TX_FIFO_EMPTY) &&
                (s_sim_register_map[card_id]->ie & SIMIE_B_TX_EMPTY))
        {
            TxEmptyIntP(card_id);
            //clear the TX Empty interrupt
            //continue Transfer data
           sim_data.card_id = card_id;
           s_sim_register_map[card_id]->iclr = SIMSTS0_B_TX_FIFO_EMPTY;
        }

        //There is RX full interrupt
        if ( (s_sim_register_map[card_id]->sts0 & SIMSTS0_B_RX_FIFO_FULL)
                && (s_sim_register_map[card_id]->ie & SIMIE_B_RX_FULL))
        {
            RxFifoFullIntP(card_id);
            //clear the RX full int
            s_sim_register_map[card_id]->iclr = SIMSTS0_B_RX_FIFO_FULL;
            //Receive the RX data
        }

    }

    return CALL_HISR;

/* -interrupt_refactor_dev */
}
/* -interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      Initialize the Driver timer of the idle mode
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint32 SIM_InitDriver (int32 card_id, TB_CALLBACK handler)
{
    //    uint32 status;
    /* CR218847 remove real nw flag  */
    //register the SIM layer call back function
    s_sim_call_back_func[card_id] = handler;

    /* +cr204609 */
    //Get if print driver log para from NV
    g_set_usim_drv_log_ctl = DMNV_GetUsimDrvLogFlagFromNV(); /*CR173855 Eddie.Wang*/
    /* -cr204609 */

    /* if timer is create already, we should not create again */
    if (NULL == s_sim_clk_stop_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_clk_stop_timer[card_id]," clk stop Timer",
                                        DriverTimerExpired,1,10,SCI_NO_ACTIVATE);
    }

    /*  set SIM_WAIT_RESP_TIMER_VALUE   */

    s_sim_wait_resp_timer_value[card_id] = SIM_WAIT_RESP_TIMER_VALUE;     /* CR218847 remove real nw flag  */


    /* if timer is create already, we should not create again */
    if (NULL == s_sim_atr_timer[card_id])
    {
       USIMDRV_CREATE_TIMER(card_id,s_sim_atr_timer[card_id]," ATR Timer",
                              DriverTimerExpired,1,SIM_ATR_TIMER_VALUE,SCI_NO_ACTIVATE);
    }

    if (NULL == s_sim_reset_p_timer[card_id])
    {
       USIMDRV_CREATE_TIMER(card_id,s_sim_reset_p_timer[card_id]," Reset P Timer",
                            DriverTimerExpired,2,SIM_RESET_TIMER_VALUE,SCI_NO_ACTIVATE);
    }

    if (NULL == s_sim_reset_bt_timer[card_id])
    {
       USIMDRV_CREATE_TIMER(card_id,s_sim_reset_bt_timer[card_id]," Reset BT Timer",
                              DriverTimerExpired,3,SIM_RESET_TIMER_VALUE,SCI_NO_ACTIVATE);
    }

    if (NULL == s_sim_status_word_timer[card_id])
    {
       USIMDRV_CREATE_TIMER(card_id,s_sim_status_word_timer[card_id]," Status Word Timer",
                              DriverTimerExpired,4,SIM_STATUS_WORD_TIMER_VALUE,SCI_NO_ACTIVATE);
    }
    if (NULL == s_sim_bwt_timer[card_id])
    {
       USIMDRV_CREATE_TIMER(card_id,s_sim_bwt_timer[card_id]," BWT Timer",
                                       DriverTimerExpired,5,SIM_BWT_TIMER_VALUE,SCI_NO_ACTIVATE);
    }
    if (NULL == s_sim_wait_resp_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_wait_resp_timer[card_id]," WAIT RESPONSE Timer",
                        DriverTimerExpired,6,s_sim_wait_resp_timer_value[card_id],SCI_NO_ACTIVATE);
    }

    if (NULL == s_sim_wwt_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_wwt_timer[card_id]," wwt Timer",
                               DriverTimerExpired,7,s_wwt_timer_value[card_id],SCI_NO_ACTIVATE);
    }
    if (NULL == s_sim_cwt_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_cwt_timer[card_id]," cwt Timer",
                                       DriverTimerExpired,8,s_cwt_timer_value[card_id],SCI_NO_ACTIVATE);
    }

    if (NULL == s_sim_rx_timeout_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_rx_timeout_timer[card_id]," rx timeout Timer",
                            DriverTimerExpired,9,   SIM_RX_TIMEOUT_VALUE, SCI_NO_ACTIVATE);
    }
    if (NULL == s_sim_init_wait_ldo_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_init_wait_ldo_timer[card_id]," Init WAIT LDO Timer",
                          InitDrvAfterLdoValid,0,SIM_WAIT_LDO_TIMER_VALUE,SCI_NO_ACTIVATE);

    }

    if (NULL == s_sim_reset_wait_ldo_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_reset_wait_ldo_timer[card_id]," Reset WAIT LDO Timer",
                           ResetAfterLdoValid,0,SIM_WAIT_LDO_TIMER_VALUE,SCI_NO_ACTIVATE);
    }

    if (NULL == s_sim_fta_test_mode_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_fta_test_mode_timer[card_id]," FTA test mode Timer",
                             DriverTimerExpired, 10, 300000, SCI_NO_ACTIVATE);
        USIMDRV_CHANGE_TIMER(card_id,s_sim_fta_test_mode_timer[card_id],DriverTimerExpired,300000);
        SCI_ActiveTimer(s_sim_fta_test_mode_timer[card_id] );
    }
/* +interrupt_refactor_dev */
    if (USIMDRV_CARD_ONE == card_id)
    {
        ISR_RegHandler_Ex (TB_SIM0_INT, SIM_InterruptHandler, SIMIntHisrFunc, CHIPDRV_HISR_PRIO_1, NULL);
    }
    else
    {
        ISR_RegHandler_Ex (TB_SIM1_INT, SIM_InterruptHandler, SIMIntHisrFunc, CHIPDRV_HISR_PRIO_1, NULL);
    }
    //status = USIMDRV_RegHandler (SIM_InterruptHandler);
    /* 20061024 delete the sci_assert for the usim reinit */
/* -interrupt_refactor_dev */
    //reset static variable and registers to the default value
    ResetDriver(card_id);
    return TB_SUCCESS;
}

/**********************************************************************
//    Description:
//      Initialize the SIM card Driver,make it ready to work when needed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ResetDriver (int32 card_id)
{
    //initialize the register map to the right position
    //与硬件相关
#ifdef PLATFORM_SC8800G
    if (USIMDRV_CARD_ONE == card_id)
    {
        s_sim_register_map[card_id] = (sim_s *) SIM0_BASE;
    }
    else
    {
        s_sim_register_map[card_id] = (sim_s *) SIM1_BASE;
    }
#else
    s_sim_register_map = (sim_s *) SIM_BASE;
#endif

    //initialize global variable
    s_previous_pps_error[card_id] = FALSE;
    s_sim_working_volt[card_id] = TB_SIM_WORKING_VOLT_1_8V;
    s_sim_reset_times[card_id] = 0;
    s_sim_b_reset_times[card_id] = 0;
    s_sim_pow_reset_times[card_id] = 0;
    s_sim_p_reset_times[card_id] = 0;
    s_card_resetting[card_id] = FALSE;
    s_card_warm_resetting[card_id] = FALSE;
    s_have_ack_byte[card_id] = FALSE;
    s_sim_is_processing_cmd[card_id] = FALSE;
    s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;
    s_null_byte_count[card_id] = 0;
    s_sim_clk_stop_mode_from_mf[card_id] = SIM_CLOCK_UNKNOWN;    /* cr206965 */

#ifdef SIM_SUPPORT_1D8V_3V
    SetSIMPower (card_id,s_sim_working_volt[card_id]);
#endif

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetDriver] Cur_Volt[%d],Tick[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1590_112_2_17_23_6_33_1161,(uint8*)"ddd", TRACE_CARD_ID, s_sim_working_volt[card_id], SCI_GetTickCount());

    s_sim_rx_block[card_id].base_info.block_type = TB_SIM_BLOCK_NOT_DEF;
    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearRxBlock(card_id);
    ClearTxBlock(card_id);
    ClearATRData(card_id);
    ClearPPSInfo(card_id);

#ifdef USIM_DMA_SUPPORT
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetDriver] DMA_Support_Flag[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1602_112_2_17_23_6_33_1162,(uint8*)"dd", TRACE_CARD_ID, sim_dma_support[card_id]);

    if (sim_dma_support[card_id])
    {
        USIM_DMA_Init(card_id);
    }

#endif

}
/**********************************************************************
//    Description:
//      go on the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim1InitDrvAfterLdoValid (
    uint32 lparam   //the function's input parameter
)
{
    uint32 cur_clk;
    TB_SIM_CLK_MODE_E clk_mode = SIM_ARM_CLK_DIV_4;

    //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1InitDrvAfterLdoValid] Tick[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1625_112_2_17_23_6_33_1163,(uint8*)"d",SCI_GetTickCount());

#ifdef PLATFORM_SC8800G
    * (volatile uint32 *) GR_GEN0 |= GEN0_SIM0_EN;
#else
    //enable SIM module
    //硬件相关8b00 0000 + 8
    CHIP_REG_OR (GR_GEN0, GEN0_SIM);
#endif

    //check the current ARM clock
    cur_clk = CHIP_GetAPBClk();

    if (ARM_CLK_13M == cur_clk)
    {
        clk_mode = SIM_ARM_CLK_DIV_4;
        s_sim_register_map[USIMDRV_CARD_ONE]->ctl1 = SIM_13M_CTL1;
    }
    else if (ARM_CLK_26M == cur_clk)
    {
        clk_mode = SIM_ARM_CLK_DIV_8;
        s_sim_register_map[USIMDRV_CARD_ONE]->ctl1 = SIM_39M_CTL1;
    }

    //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1InitDrvAfterLdoValid] Cur_Clk[%d],Clock_Mode[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1649_112_2_17_23_6_33_1164,(uint8*)"dd",cur_clk,clk_mode);
    //set the clock mode and clock dividor
    s_sim_register_map[USIMDRV_CARD_ONE]->rx_clkd = TB_SIM_DEFAULT_FACTOR_F/TB_SIM_DEFAULT_FACTOR_D * s_clk_dvd_factors[clk_mode];
    s_sim_register_map[USIMDRV_CARD_ONE]->tx_clkd = TB_SIM_DEFAULT_FACTOR_F/TB_SIM_DEFAULT_FACTOR_D * s_clk_dvd_factors[clk_mode];
    s_sim_clk_mode[USIMDRV_CARD_ONE] = clk_mode;

    //set the SIM register value
    s_sim_register_map[USIMDRV_CARD_ONE]->sts0    = SIM_DEFAULT_STS0;
    s_sim_register_map[USIMDRV_CARD_ONE]->sts1    = SIM_DEFAULT_STS1;
    s_sim_register_map[USIMDRV_CARD_ONE]->ie      = 0;                 //disable all the interrupter at the beginning
    s_sim_register_map[USIMDRV_CARD_ONE]->iclr    = SIM_DEFAULT_ICLR;
    s_sim_register_map[USIMDRV_CARD_ONE]->ctl0    = SIM_DEFAULT_CTL0;
    s_sim_register_map[USIMDRV_CARD_ONE]->she     = SIM_DEFAULT_SHE;
    s_sim_register_map[USIMDRV_CARD_ONE]->tgc     = SIM_DEFAULT_TGC;
    s_sim_register_map[USIMDRV_CARD_ONE]->wdt     = 0;
    s_sim_register_map[USIMDRV_CARD_ONE]->imsk    = SIM_DEFAULT_IMASKED;
    s_sim_register_map[USIMDRV_CARD_ONE]->wdt = SIM_DEFAULT_WDT | BIT_0;  //bit_1=1;bit_11=1;
    s_sim_register_map[USIMDRV_CARD_ONE]->wdt1 = 0x400;                             //  Watch dog count limit for rx time out

    /*  set rx/tx watermark, the value of watermark should more than 0 */
    s_sim_register_map[USIMDRV_CARD_ONE]->ctl1 |= BIT_0 | BIT_4;

#ifdef PLATFORM_SC8800G
    /*  enable SIM interrupt ref 0x20a00010 */
    //  *(volatile uint32*)0x20a00010 |= INTCTL_SIM0_IRQ;
    //* (volatile uint32 *) INT_IRQ_EN |= BIT_12;
    //enable SIM interrupt
    CHIPDRV_EnableIRQINT (TB_SIM_INT);
#else
    /*  enable SIM interrupt ref 0x20a00010 */
    CHIP_REG_OR (0x20a00010, INTCTL_SIM_IRQ);
#endif

    ResetSIMCard (USIMDRV_CARD_ONE, TB_SIM_RESET_NO_CHANGE);
}
/**********************************************************************
//    Description:
//      go on the initialization after the SIM card LDO valid
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim2InitDrvAfterLdoValid (
    uint32 lparam   //the function's input parameter
)
{
    uint32 cur_clk;
    TB_SIM_CLK_MODE_E clk_mode = SIM_ARM_CLK_DIV_4;

    //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2InitDrvAfterLdoValid] Tick[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1697_112_2_17_23_6_33_1165,(uint8*)"d",SCI_GetTickCount());

    * (volatile uint32 *) GR_GEN0 |= GEN0_SIM1_EN;

    //check the current ARM clock
    cur_clk = CHIP_GetAPBClk();

    if (ARM_CLK_13M == cur_clk)
    {
        clk_mode = SIM_ARM_CLK_DIV_4;
        s_sim_register_map[USIMDRV_CARD_TWO]->ctl1 = SIM_13M_CTL1;
    }
    else if (ARM_CLK_26M == cur_clk)
    {
        clk_mode = SIM_ARM_CLK_DIV_8;
        s_sim_register_map[USIMDRV_CARD_TWO]->ctl1 = SIM_39M_CTL1;
    }

    //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2InitDrvAfterLdoValid] Cur_Clk[%d],Clock_Mode[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1715_112_2_17_23_6_33_1166,(uint8*)"dd",cur_clk,clk_mode);
    //set the clock mode and clock dividor
    s_sim_register_map[USIMDRV_CARD_TWO]->rx_clkd = TB_SIM_DEFAULT_FACTOR_F/TB_SIM_DEFAULT_FACTOR_D * s_clk_dvd_factors[clk_mode];
    s_sim_register_map[USIMDRV_CARD_TWO]->tx_clkd = TB_SIM_DEFAULT_FACTOR_F/TB_SIM_DEFAULT_FACTOR_D * s_clk_dvd_factors[clk_mode];
    s_sim_clk_mode[USIMDRV_CARD_TWO] = clk_mode;

    //set the SIM register value
    s_sim_register_map[USIMDRV_CARD_TWO]->sts0    = SIM_DEFAULT_STS0;
    s_sim_register_map[USIMDRV_CARD_TWO]->sts1    = SIM_DEFAULT_STS1;
    s_sim_register_map[USIMDRV_CARD_TWO]->ie      = 0;                 //disable all the interrupter at the beginning
    s_sim_register_map[USIMDRV_CARD_TWO]->iclr    = SIM_DEFAULT_ICLR;
    s_sim_register_map[USIMDRV_CARD_TWO]->ctl0    = SIM_DEFAULT_CTL0;
    s_sim_register_map[USIMDRV_CARD_TWO]->she     = SIM_DEFAULT_SHE;
    s_sim_register_map[USIMDRV_CARD_TWO]->tgc     = SIM_DEFAULT_TGC;
    s_sim_register_map[USIMDRV_CARD_TWO]->wdt     = 0;
    s_sim_register_map[USIMDRV_CARD_TWO]->imsk    = SIM_DEFAULT_IMASKED;
    s_sim_register_map[USIMDRV_CARD_TWO]->wdt = SIM_DEFAULT_WDT | BIT_0;  //bit_1=1;bit_11=1;
    s_sim_register_map[USIMDRV_CARD_TWO]->wdt1 = 0x400;                             //  Watch dog count limit for rx time out

    /*  set rx/tx watermark, the value of watermark should more than 0 */
    s_sim_register_map[USIMDRV_CARD_TWO]->ctl1 |= BIT_0 | BIT_4;

    /*  enable SIM interrupt ref 0x20a00010 */
    //  *(volatile uint32*)0x20a00010 |= INTCTL_SIM0_IRQ;
    // * (volatile uint32 *) INT_IRQ_EN |= BIT_12;
    CHIPDRV_EnableIRQINT (TB_SIM1_INT);

    ResetSIMCard (USIMDRV_CARD_TWO, TB_SIM_RESET_NO_CHANGE);
}
/**********************************************************************
//    Description:
//      Reset the SIM card with or without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_ResetSIMCard (int32 card_id)
{
    s_sim_reset_times[card_id] = 0;
    s_sim_pow_reset_times[card_id] = 0;
    s_sim_b_reset_times[card_id] = 0;
    s_sim_p_reset_times[card_id] = 0;

    s_sim_is_processing_cmd[card_id] = FALSE;
    SCI_DISABLE_IRQ;

    if (SCI_IsTimerActive (s_sim_init_wait_ldo_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_init_wait_ldo_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_init_wait_ldo_timer[card_id],InitDrvAfterLdoValid,SIM_WAIT_LDO_TIMER_VALUE);

    SCI_ActiveTimer (s_sim_init_wait_ldo_timer[card_id]);
    SCI_RESTORE_IRQ;
}

/**********************************************************************
//    Description:
//     Warm Reset the SIM card without change the working voltage for
//    the SIM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_WarmResetSimCard (int32 card_id)
{
    uint32 temp;
    uint16 rx_fifo_count,i;


    //看是否正在RESET s_card_warm_resetting和s_card_resetting
    /* binli modify for d-phone */
    /*
    SCI_DISABLE_IRQ;
    if (s_card_resetting || s_card_warm_resetting)
    {
        SCI_RESTORE_IRQ;
        return;
    }
    s_card_warm_resetting = TRUE;
    SCI_RESTORE_IRQ;
     */
    //SCI_TRACE_LOW:"USIMDRV:[%d]:API [SIM_WarmResetSimCard] Warm Reset the SIM card!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1797_112_2_17_23_6_33_1167,(uint8*)"d", TRACE_CARD_ID);

    //deactive these timer to avoid the timer expiration cause SIM card Reset nest
    SCI_DeactiveTimer (s_sim_reset_p_timer[card_id]);
    SCI_DeactiveTimer (s_sim_reset_bt_timer[card_id]);
    SCI_DeactiveTimer (s_sim_atr_timer[card_id]);
    //重启ATR时间

    USIMDRV_CHANGE_TIMER(card_id,s_sim_atr_timer[card_id],DriverTimerExpired,SIM_ATR_TIMER_VALUE);


    //clear the rx fifo if needed
    //读完rx fifo中残留的数据
    rx_fifo_count = (uint16) GET_RX_FIFO_COUNT (s_sim_register_map[card_id]);

    for (i = 0; i < rx_fifo_count; i++)
    {
        temp = s_sim_register_map[card_id]->rx;
    }

    //clear previous interrupt status
    s_sim_register_map[card_id]->iclr = 0xffff;
    s_sim_register_map[card_id]->she = 0;

    /* +Cr200737 */
    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id], 1);
    s_sim_register_map[card_id]->rx_clkd = TB_SIM_DEFAULT_FACTOR_F/TB_SIM_DEFAULT_FACTOR_D * s_clk_dvd_factors[s_sim_clk_mode[card_id]];
    s_sim_register_map[card_id]->tx_clkd = TB_SIM_DEFAULT_FACTOR_F/TB_SIM_DEFAULT_FACTOR_D * s_clk_dvd_factors[s_sim_clk_mode[card_id]];
    /* -Cr200737 */

    /* +cr147103 */
    /* bin.li correct refresh function */
    s_tck_will_be_sent[card_id] = FALSE;

    /* -cr147103 */
    // rst = L i/o ?
    for (i=0; i<100; i++)
    {
    }

    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_TX_DATA_OUT_LOW;

    for (i=0; i<100; i++)
    {
    }

    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_SIM_RST;
    s_sim_register_map[card_id]->ie &= 0;

    for (i=0; i<1000; i++)
    {
    }

    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearATRData(card_id);

    //initialize global variable
    s_sim_rx_buf[card_id].is_locked = FALSE;
    s_have_ack_byte[card_id] = FALSE;
    s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;

    s_sim_reset_times[card_id]++;
    s_sim_register_map[card_id]->ie = SIM_DEFAULT_IE;
    /*  bo.chen add trace for power enable */
    //SCI_TRACE_LOW:"USIMDRV:[%d]:API [SIM_WarmResetSimCard] Tick[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1863_112_2_17_23_6_33_1168,(uint8*)"dd", TRACE_CARD_ID, SCI_GetTickCount());
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_POWER_EN;

    if (SCI_IsTimerActive (s_sim_atr_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_atr_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_atr_timer,DriverTimerExpired,SIM_ATR_TIMER_VALUE);

    SCI_ActiveTimer (s_sim_atr_timer[card_id]);

    //enactive the SIM card
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_EN_ACTIVE;
#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        USIM_DMA_Init(card_id);
        USIMDRV_DMA_RX_Enable(card_id);
    }
    else
    {
        //enable the rx to receive the tx data
        s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
    }

#else
    //enable the rx to receive the tx data
    if (!s_is_sim_out[card_id])   /*NEWMS00106599 E*/
        s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
#endif

    return ;
}
/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card to start an operation on
//    The SIM card we needed
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SEND_INSTR_RESULT_E SIM_SendInstrCode (int32 card_id,TB_USIM_APDU_T *apdu_ptr)
{
/*+NEWMS00106599 E*/
    if (s_is_sim_out[card_id]){
        //SCI_TraceLow:"USIMDRV:[%d]: s_is_sim_out = TRUE;TX:Need De-active SIM!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_1908_112_2_17_23_6_34_1169,(uint8*)"d", card_id);
        NotifyExceptionError (card_id, TB_SIMEXP_CARD_OUT);  /*notify the upper layer*/
        return;}
        
    /*Max length of junk data need ref to the expected respond length from SIM*/
    s_sim_max_junk_data_num[card_id] = 
        (apdu_ptr->rx_expected_len>SIM_MAX_JUNK_DATA_NUM)? \
        (apdu_ptr->rx_expected_len+2):SIM_MAX_JUNK_DATA_NUM;
/*-NEWMS00106599 E*/
    //check the parameters
    s_null_byte_count[card_id] = 0;

    // this type need response data
    if ( (TB_SIMINSTR_RECV_DATA == apdu_ptr->instr_type) ||
            (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type))
    {
        sim_response_data_present[card_id] = TRUE;
    }
    else
    {
        sim_response_data_present[card_id] = FALSE;
    }

    s_have_received_response_data[card_id] = FALSE;


    if (apdu_ptr == NULL)
    {
        USIM_ASSERT (FALSE);/*assert verified*/
    }

    /* +cr/162983 */
    /*
        if(apdu_ptr->rx_expected_len > 256)
        {
            return SIM_SEND_INSTR_INVALID_PARA;
        }
    */
    /* -cr/162983 */

    if (apdu_ptr->instr_type >= TB_SIMINSTR_MAX)
    {
        USIM_ASSERT (FALSE);/*assert verified*/
    }

    //check if whether the usim card is processing a command
    if (s_sim_is_processing_cmd[card_id])
    {
        return SIM_SEND_INSTR_CARD_BUSY;
    }

    if (SCI_IsTimerActive (s_sim_clk_stop_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_clk_stop_timer[card_id]);
    }

    //before start to send the instruction code,check the clock of the SIM card first.
    //if the clock has been stoped,enable the clock first
    // 激活clock
    if (! (s_sim_register_map[card_id]->ctl1 & SIMCTL1_B_CLK_ENABLE))
    {
        ActiveSIMCard(card_id);
    }

    //enable the retransmit fucntion to handler rx parity error.
    s_sim_register_map[card_id]->she = SIM_ENABLED_SHE;

    // 设置s_sim_is_processing_cmd标识位
    s_sim_is_processing_cmd[card_id] = TRUE;

    //clear tx buf，需要重新开始发送数据
    ClearTxBuf(card_id);
    s_have_ack_byte[card_id] = FALSE;

    /*  except status cmd   */
    s_sim_instr_class[card_id] = apdu_ptr->instr_code[TB_SIM_APDU_CLA];

    //send instruction
#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        return (USIM_DMA_SendInstrCode (card_id,apdu_ptr));
    }
    else
    {
        if (s_atr_data_buf.atr_data.t_protocol == 0)
        {
            SendInstrCode_T0 (card_id, apdu_ptr);
        }
        else
        {
            SendInstrCode_T1 (card_id, apdu_ptr);
        }

        SCI_SIM_EnableDeepSleep (0);

        return SIM_SEND_INSTR_OK;
    }

#else

    if (s_atr_data_buf[card_id].atr_data.t_protocol == 0)
    {
        SendInstrCode_T0 (card_id, apdu_ptr);
    }
    else
    {
        SendInstrCode_T1 (card_id, apdu_ptr);
    }

    SCI_SIM_EnableDeepSleep (0);

    return SIM_SEND_INSTR_OK;
#endif
}
/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card(only for t0 protocol)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendInstrCode_T0 (
    int32 card_id,
    TB_USIM_APDU_T *apdu_ptr    //in:the instruction code to be sent to SIM card
)
{
    uint16 i;
    uint32 temp,fifo_index;  /*NEWMS00106599 E*/
    uint16 fifo_count;  /*NEWMS00106599 E*/
    //copy the instruction code to the rx tx buffer
    // 先拷贝cla ins p1 p2 p3
    memcpy (s_sim_tx_buf[card_id].buf,apdu_ptr->instr_code,TB_SIM_APDU_CHEADER_FIVE);
    /* +cr163006 */
    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_PARITY_ERR;
    /* -cr163006 */
    /* +binli opt sim */
    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id], 1);
    /* +binli opt sim */

    // bo.chen 20051205 add log for authentication
    if (0x88 == apdu_ptr->instr_code[TB_SIM_APDU_INS])
    {
        s_auth_inited[card_id] = TRUE;
    }

    if (0xa4 == apdu_ptr->instr_code[TB_SIM_APDU_INS])
    {
        apdu_ptr->rx_expected_len = 0;
    }

    /*  20080324    */
    s_is_reset_flag_set[card_id] = FALSE;

    if (0xf2 == apdu_ptr->instr_code[TB_SIM_APDU_INS])
    {
        if (s_is_reset_flag_set[card_id])
        {
            NotifyExceptionError (card_id, TB_SIMEXP_CARD_UNRESPONSE);
            s_is_reset_flag_set[card_id] = FALSE;
            return;
        }
        else
        {
            s_is_reset_flag_set[card_id] = TRUE;
        }
    }

    // 至少现在需要发送5个字节
    s_sim_tx_buf[card_id].tx_data_len = TB_SIM_APDU_CHEADER_FIVE;

    //is send data exist
    // 需要发送数据，表明有命令,因此TB_SIMINSTR_SEND_DATA比较重要
    if ( (TB_SIMINSTR_SEND_DATA == apdu_ptr->instr_type) ||
            (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type))
    {
        //copy the data after the command code
        if (0 != apdu_ptr->instr_code[TB_SIM_APDU_P3])
        {
            s_sim_tx_buf[card_id].tx_data_len += apdu_ptr->instr_code[TB_SIM_APDU_P3];
            memcpy ( (s_sim_tx_buf[card_id].buf + TB_SIM_APDU_CHEADER_FIVE),apdu_ptr->instr_data,
                     apdu_ptr->instr_code[TB_SIM_APDU_P3]);
        }
    }

    /* +cr204609 */
    if (s_auth_inited[card_id] || g_set_usim_drv_log_ctl) /*CR173855 Eddie.Wang*/
        /* -cr204609 */
    {
        uint32 ii = 0;
        //SCI_TRACE_LOW:"USIMDRV:[%d]:tx_data_len[%d], rx_expected_len[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2098_112_2_17_23_6_34_1170,(uint8*)"ddd", TRACE_CARD_ID, s_sim_tx_buf[card_id].tx_data_len, apdu_ptr->rx_expected_len + TB_SIM_RX_STATUS_WORD_NUM);

        while (s_sim_tx_buf[card_id].tx_data_len > ii*16)
        {
            //SCI_TRACE_LOW:"USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2106_112_2_17_23_6_34_1171,(uint8*)"ddddddddddddddddd", TRACE_CARD_ID,s_sim_tx_buf[card_id].buf[ii*16 + 0],s_sim_tx_buf[card_id].buf[ii*16 + 1],s_sim_tx_buf[card_id].buf[ii*16 + 2],s_sim_tx_buf[card_id].buf[ii*16 + 3],s_sim_tx_buf[card_id].buf[ii*16 + 4],s_sim_tx_buf[card_id].buf[ii*16 + 5],s_sim_tx_buf[card_id].buf[ii*16 + 6],s_sim_tx_buf[card_id].buf[ii*16 + 7],s_sim_tx_buf[card_id].buf[ii*16 + 8],s_sim_tx_buf[card_id].buf[ii*16 + 9],s_sim_tx_buf[card_id].buf[ii*16 + 10],s_sim_tx_buf[card_id].buf[ii*16 + 11],s_sim_tx_buf[card_id].buf[ii*16 + 12],s_sim_tx_buf[card_id].buf[ii*16 + 13],s_sim_tx_buf[card_id].buf[ii*16 + 14],s_sim_tx_buf[card_id].buf[ii*16 + 15]);
            ii++;

            /*  if tx_data_len > 255 break; */
            if (16 <= ii)
            {
                break;
            }
        }
    }
/*+NEWMS00106599 E*/
    /*clear the rx fifo if needed*/
    fifo_count = (uint16) GET_RX_FIFO_COUNT (s_sim_register_map[card_id]);
    for (fifo_index = 0; fifo_index < fifo_count; fifo_index++)
    {
        temp = s_sim_register_map[card_id]->rx;
    }
/*-NEWMS00106599 E*/ 
    //open the lock of the sim recv data buf
    //准备接收
    s_sim_rx_buf[card_id].is_locked = FALSE;

    //get the expected len of the response data
    //需要接收，则期望的长度，由上层指定的长度不需要加上sw1 sw2，由驱动来添加
    if ( (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type)
            || (TB_SIMINSTR_RECV_DATA == apdu_ptr->instr_type))
    {
        s_sim_rx_buf[card_id].rx_expected_len = apdu_ptr->rx_expected_len + TB_SIM_RX_STATUS_WORD_NUM;
    }
    else
    {
        //the 2 byte status words.
        //如果不需要接收数据就只有sw1 sw2
        s_sim_rx_buf[card_id].rx_expected_len = TB_SIM_RX_STATUS_WORD_NUM;
    }

    // 用于redo??
    s_sim_tx_instr_len[card_id] = s_sim_tx_buf[card_id].tx_data_len;
    s_sim_rx_rexpect_len[card_id] = s_sim_rx_buf[card_id].rx_expected_len;

    /*Turn on the interrupt of RX_FULL*/
    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;  /*NEWMS00106599 E*/

    //send the command header
    for (i = 0; i < TB_SIM_APDU_CHEADER_FIVE; i++)
    {
        s_sim_register_map[card_id]->tx = s_sim_tx_buf[card_id].buf[i];
        s_sim_tx_buf[card_id].cur_pos++;
    }

    /*  set rx timeout timer 20070730   */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);

    SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);

    s_sim_rxfifo_count[card_id] = 0;
    s_sim_rxfifo_int_count[card_id] = 0;
    /* +cr224711 */
    if ((!g_status_tmer_wwt_value_flag[card_id]) && (SIM_WAIT_RESP_TIMER_VALUE != s_sim_wait_resp_timer_value[card_id]))
    {
        s_sim_wait_resp_timer_value[card_id] = SIM_WAIT_RESP_TIMER_VALUE;
    }
    /* -cr224711 */
    USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);

    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);

    //if all the data of the apdu has been sent,start the wait response timer
    if (s_sim_tx_buf[card_id].cur_pos == s_sim_tx_buf[card_id].tx_data_len)
    {

        if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
        }

        USIMDRV_CHANGE_TIMER(card_id,s_sim_wwt_timer[card_id],DriverTimerExpired,s_wwt_timer_value[card_id]);

        SCI_ActiveTimer (s_sim_wwt_timer[card_id]);
    }

}
/**********************************************************************
//    Description:
//      Send a Instruction to the SIM card(only for T1 protocol)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendInstrCode_T1 (int32 card_id,TB_USIM_APDU_T *apdu_ptr)
{
    uint16 block_ifsc_size;
    uint16 real_data_len;
    uint8 block_count = 0;
    uint8 *block_begin_ptr = s_sim_tx_buf[card_id].buf;
    uint8 *block_begin_pos = s_sim_tx_buf[card_id].buf;
    uint8 *instruction_data_ptr = apdu_ptr->instr_data;
    uint16 data_of_the_last_block = 0;
    uint16 i,data_len;
    uint16 tx_data_len;
    uint16 first_send_data_num;//the data num of the first time to sent
    uint16 tx_transmit_num;//the free space in the tx fifo
    BOOLEAN is_next_block_exist = TRUE;
    BOOLEAN is_data_sent_all = FALSE;

    USIM_ASSERT (NULL != apdu_ptr);/*assert verified*/

    //disable the retransmit fucntion to handler rx parity error.
    s_sim_register_map[card_id]->she = SIM_DEFAULT_SHE;

    //clear variable
    ClearTxBlock(card_id);
    ClearRxBlock(card_id);

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SendInstrCode_T1] IFSC[0x%x], Ins_Type[0x%x], P3[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2224_112_2_17_23_6_34_1172,(uint8*)"dddd", TRACE_CARD_ID, s_atr_data_buf[card_id].atr_data.IFSC, apdu_ptr->instr_type, apdu_ptr->instr_code[TB_SIM_APDU_P3]);

    //get the current fifo free space
    tx_transmit_num = (uint16) (GET_TX_FIFO_COUNT (s_sim_register_map[card_id]));
    tx_transmit_num = (uint16) SIM_FIFO_BUF_SIZE - tx_transmit_num;

    //calculate the block count
    block_ifsc_size = s_atr_data_buf[card_id].atr_data.IFSC;


    //calculate data length in the information field
    tx_data_len = TB_SIM_APDU_CHEADER_FIVE;

    if (TB_SIMINSTR_SEND_DATA == apdu_ptr->instr_type)
    {
        //copy the data after the command code
        if (0 != apdu_ptr->instr_code[TB_SIM_APDU_P3])
        {
            tx_data_len += apdu_ptr->instr_code[TB_SIM_APDU_P3];
        }
    }
    else if (apdu_ptr->instr_type == TB_SIMINSTR_SEND_AND_RECV_DATA)
    {
        tx_data_len += (apdu_ptr->instr_code[TB_SIM_APDU_P3] + 1);
    }

    //calculate the block count and data num in the block
    block_count = tx_data_len/block_ifsc_size;
    data_of_the_last_block = tx_data_len%block_ifsc_size;

    if (data_of_the_last_block!= 0)
    {
        block_count += 1;
    }
    else
    {
        data_of_the_last_block = block_ifsc_size;

    }

    s_sim_tx_buf[card_id].tx_data_len = tx_data_len + TB_SIM_BLOCK_EXT_BYTE_NUM* block_count;


    //record the base info of the first block to the tx block
    s_sim_tx_block[card_id].block_begin_pos = 0;

    if (block_count == 1)
    {
        s_sim_tx_block[card_id].block_end_pos = data_of_the_last_block + 4;
        s_sim_tx_block[card_id].base_info.i_block.chain_M = FALSE;
    }
    else
    {
        s_sim_tx_block[card_id].block_end_pos = block_ifsc_size + 4;
        s_sim_tx_block[card_id].base_info.i_block.chain_M = TRUE;
    }

    /*  20070614    */
    s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_I_BLOCK;

    //fill the data into the tx buf
    for (i = 0; i< block_count; i++)
    {
        data_len = block_ifsc_size;

        if (i == (block_count -1))
        {
            data_len = data_of_the_last_block;
            is_next_block_exist = FALSE;
        }

        real_data_len = data_len;   //data in the block except for the command header
        block_begin_pos = block_begin_ptr;

        //fill the prologue and epilogue field
        *block_begin_ptr = SIM_DEFAULT_NAD;         //NAD
        block_begin_ptr += 1;
        s_pcb_ns[card_id] = (s_pcb_ns[card_id] + 1) %2;
        *block_begin_ptr = SIM_DEFAULT_PCB_IBLOCK | (s_pcb_ns[card_id] << USIM_CONST_NUM6) | (is_next_block_exist << USIM_CONST_NUM5); //PCB

        block_begin_ptr += 1;
        *block_begin_ptr = data_len;                //LEN
        block_begin_ptr += 1;

        //fill the command header into the information field
        if (i == 0) //copy the command header
        {
            if (apdu_ptr->instr_type == TB_SIMINSTR_SEND_AND_RECV_DATA)
            {
                //copy 6 command header
                memcpy (block_begin_ptr,apdu_ptr->instr_code,TB_SIM_APDU_CHEADER_NUM);
                block_begin_ptr += TB_SIM_APDU_CHEADER_NUM;
                real_data_len = data_len - TB_SIM_APDU_CHEADER_NUM;   //data in the block except for the command header
            }
            else
            {
                //copy 5 command header
                memcpy (block_begin_ptr,apdu_ptr->instr_code,TB_SIM_APDU_CHEADER_FIVE);
                block_begin_ptr += TB_SIM_APDU_CHEADER_FIVE;
                real_data_len = data_len - TB_SIM_APDU_CHEADER_FIVE;  //data in the block except for the command header
            }
        }

        //copy data
        memcpy (block_begin_ptr,instruction_data_ptr,real_data_len);
        block_begin_ptr += real_data_len;

        /*  20071207    */
        if (!is_next_block_exist)
        {
            if (apdu_ptr->instr_type == TB_SIMINSTR_SEND_AND_RECV_DATA)
            {
                * (block_begin_ptr-1) = 0;
            }
        }

        //fill the epilogue field
        *block_begin_ptr = GetEDC (block_begin_pos,block_begin_ptr - block_begin_pos);
        block_begin_ptr += 1;

        //regulate the pointer
        instruction_data_ptr += real_data_len;
    }

    //get the expected len of the response data
    if ( (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type)
            || (TB_SIMINSTR_RECV_DATA == apdu_ptr->instr_type))
    {
        s_sim_rx_buf[card_id].rx_expected_len = apdu_ptr->rx_expected_len + TB_SIM_RX_STATUS_WORD_NUM;
    }
    else
    {
        //the 2 byte status words.
        s_sim_rx_buf[card_id].rx_expected_len = TB_SIM_RX_STATUS_WORD_NUM;
    }

    s_sim_tx_instr_len[card_id] = s_sim_tx_buf[card_id].tx_data_len;
    s_sim_rx_rexpect_len[card_id] = s_sim_rx_buf[card_id].rx_expected_len;

    //calculate the data number of the first time to send
    first_send_data_num = s_sim_tx_block[card_id].block_end_pos;

    if (tx_transmit_num < first_send_data_num)
    {
        first_send_data_num = tx_transmit_num;
    }
    else
    {
        is_data_sent_all = TRUE;
    }


    /*  20070710    */
    s_sim_tx_block[card_id].base_info.i_block.ns = \
                                          GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM6);
    s_sim_tx_block[card_id].base_info.i_block.chain_M = \
            GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM5);

    //send the first set data
    for (i = 0; i < first_send_data_num; i++)
    {
        s_sim_register_map[card_id]->tx = s_sim_tx_buf[card_id].buf[i];
        s_sim_tx_buf[card_id].cur_pos++;
    }

    /*  20070615 for test   */
    s_sim_register_map[card_id]->ie |=  SIMIE_B_TX_EMPTY;


    /*  set rx timeout timer 20070730   */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);

    SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);

    //if all the data of the block has been sent,start the bwt timer
    if (is_data_sent_all)
    {
        USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DriverTimerExpired,s_atr_data_buf[card_id].BWT);
        SCI_ActiveTimer (s_sim_bwt_timer[card_id]);
    }
}

/**********************************************************************
//    Description:
//      Reject the SIM card(deactive the SIM card and Power off the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void RejectSIMCard (int32 card_id)
{
    uint32 i;
    //7816-3 p7
    // 1 , rst = L
    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_SIM_RST;

    for (i = 0; i < 100; i++)
    {
    }

    // 2.1, clk = L
    SetClockStopLevel (card_id, FALSE);
    // 2.2, disable clk
    s_sim_register_map[card_id]->ctl1 &= ~SIMCTL1_B_CLK_ENABLE;

    for (i = 0; i < 100; i++)
    {
    }


    //这一位是i/o =1 表示Z????
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_TX_DATA_OUT_LOW;

    for (i = 0; i < 100; i++)
    {
    }

    /*  bo.chen add trace for power enable */
    SCI_TRACE_LOW ("USIMDRV:[%d]:[RejectSIMCard] Tick[%d], Power_tatus[%d], Rst[%d],L0", TRACE_CARD_ID, \
                   SCI_GetTickCount(), (s_sim_register_map[card_id]->ctl0 & SIMCTL0_B_POWER_EN) ?1:0, \
                   (s_sim_register_map[card_id]->ctl0 & SIMCTL0_B_SIM_RST) ?1:0);

#ifdef PLATFORM_SC8800G
#else
    /* set  4.7k pull-up for SIM DATA pads */
    CHIP_REG_AND (PIN_SIMDA0_REG,~PIN_PU_EN);
#endif

    /*  20070716 add time for power disable */
    for (i = 0; i < 1000; i++)
    {
    }

    // vcc deactivated
    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_POWER_EN;
    /*+CR200528 Eddie.Wang*/
#ifdef PLATFORM_SC8800G
    if (USIMDRV_CARD_ONE == card_id)
    {
        LDO_TurnOffLDO (LDO_LDO_SIM0);
    }
    else
    {
        LDO_TurnOffLDO (LDO_LDO_SIM1);
    }
#endif
    /*-CR200528 Eddie.Wang*/
}
/**********************************************************************
//    Description:
//      PowerOff the SIM card and disable the SIM module
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PowerOffSIM (void)
{
   SIM_PowerOffSIMEx(0);
   SIM_PowerOffSIMEx(1);
}
/**********************************************************************
//    Description:
//      PowerOff the SIM card and disable the SIM module
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PowerOffSIMEx (int32 card_id)
{
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SIM_PowerOffSIM]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2494_112_2_17_23_6_35_1173,(uint8*)"d", TRACE_CARD_ID);

    /*+CR178132 Eddie.Wang*/
    RejectSIMCard(card_id);
#ifdef PLATFORM_SC8800G
#else
    s_sim_register_map[card_id]->ie = 0;
    /*-CR178132 Eddie.Wang*/
#endif

    /*  20070712 reject all cmd from ps */
    s_sim_is_processing_cmd[card_id] = TRUE;

    s_card_resetting[card_id] = FALSE;
    s_card_warm_resetting[card_id] = FALSE;
    s_previous_pps_error[card_id] = FALSE;
    s_sim_clk_mode[card_id] = SIM_ARM_CLK_DIV_4;
    s_sim_working_volt[card_id] = TB_SIM_WORKING_VOLT_1_8V;
    s_sim_reset_times[card_id] = 0;
    s_sim_b_reset_times[card_id] = 0;
    s_sim_pow_reset_times[card_id] = 0;
    s_sim_p_reset_times[card_id] = 0;
    s_have_ack_byte[card_id] = FALSE;
    s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;
    /* s_sim_clk_stop_mode_from_mf = SIM_CLOCK_UNKNOWN;*/    /* 210334 */

#ifdef SIM_SUPPORT_1D8V_3V
    SetSIMPower (card_id, s_sim_working_volt[card_id]);
#endif
    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearRxBlock(card_id);
    ClearTxBlock(card_id);
    ClearATRData(card_id);
    ClearPPSInfo(card_id);

#ifdef USIM_DMA_SUPPORT
    USIMDRV_DMA_RX_Disable(card_id);
    USIMDRV_DMA_TX_Disable(card_id);
#endif

    //reset the registers
    s_sim_register_map[card_id]->ie = 0;
    /* +cr200462 */
    s_sim_register_map[card_id]->ctl0 = (0 | SIMCTL0_B_TX_DATA_OUT_LOW);
    /* -cr200462 */
    s_sim_register_map[card_id]->ctl1 = 0;
    s_sim_register_map[card_id]->rx_clkd = 0;
    s_sim_register_map[card_id]->tx_clkd = 0;
    s_sim_register_map[card_id]->she = 0;
    s_sim_register_map[card_id]->tgc = 0;
    s_sim_register_map[card_id]->wdt = 0;

    /* +cr148488 */
    if (NULL != s_sim_atr_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_atr_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_atr_timer[card_id]);
        }
    }

    if (NULL != s_sim_reset_p_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_reset_p_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_reset_p_timer[card_id]);
        }
    }

    if (NULL != s_sim_reset_bt_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_reset_bt_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_reset_bt_timer[card_id]);
        }
    }

    if (NULL != s_sim_status_word_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_status_word_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_status_word_timer[card_id]);
        }
    }

    if (NULL != s_sim_bwt_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_bwt_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_bwt_timer[card_id]);
        }
    }

    if (NULL != s_sim_wait_resp_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
        }
    }

    if (NULL != s_sim_wwt_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
        }
    }

    /* +cr151161 */
    if (NULL != s_sim_cwt_timer[card_id])
        /* -cr151161 */
    {
        if (SCI_IsTimerActive (s_sim_cwt_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_cwt_timer[card_id]);
        }
    }

    if (NULL != s_sim_rx_timeout_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }
    }

    if (NULL != s_sim_init_wait_ldo_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_init_wait_ldo_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_init_wait_ldo_timer[card_id]);
        }
    }

    if (NULL != s_sim_reset_wait_ldo_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_reset_wait_ldo_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_reset_wait_ldo_timer[card_id]);
        }
    }

#ifdef USIM_DMA_SUPPORT

    if (NULL != s_sim_dma_byte_timer[card_id])
    {
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }
    }

#endif

    /* -cr148488 */

    //disable SIM interrupt
    //  *(volatile uint32*)INT_IRQ_EN &= ~INTCTL_SIM_IRQ;

    //disable SIM module
    if (USIMDRV_CARD_ONE == card_id)
    {
        CHIP_REG_AND (GR_GEN0, ~GEN0_SIM0_EN);
    }
    else
    {
        CHIP_REG_AND (GR_GEN0, ~GEN0_SIM1_EN);
    }
}

/**********************************************************************
//    Description:
//      Change the current of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_ChangeCurrent (int32 card_id,uint8 current)
{
}
/**********************************************************************
    Description:
       receive clock phase and supply voltage informations which are gotten from
           response FCP data of selecting MF, and do nothing for now but just the check
           whether it is the same or different as the one from ATR and print something.
    Global resource dependence :
    Author:amy.wang
    Note:
***********************************************************************/
void SIM_SelMFAckClkVol (int32 card_id,
                         sim_supply_voltage_enum  supply_voltage,
                         sim_clock_phase_enum     clk_phase ,
                         BOOLEAN *is_need_reset)
{
    BOOLEAN voltage_flag=FALSE;

    if (clk_phase != s_atr_data_buf[card_id].atr_data.X)
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[SIM_SelMFAckClkVol] Clk_Phase_in_MF != Clk_Phase_in_ATR, Clk_Phase_in_MF[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2693_112_2_17_23_6_35_1174,(uint8*)"dd", TRACE_CARD_ID,clk_phase);

        /* +cr206965 */
        s_sim_clk_stop_mode_from_mf[card_id] = clk_phase;
        /* -cr206965 */
    }

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SIM_SelMFAckClkVol] Expected_Voltage[%d], Current_Voltage[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2700_112_2_17_23_6_35_1175,(uint8*)"ddd", TRACE_CARD_ID, supply_voltage, s_sim_working_volt);

    switch (supply_voltage)
    {
        case SIM_VOLTAGE_1_8_V:

            if (s_sim_working_volt[card_id] !=TB_SIM_WORKING_VOLT_1_8V) //TB_SIM_WORKING_VOLT_1D8V)
                /* +cr117796 */
                /* bin.li modify for 1.8v only, but can't active with 1.8V */
            {
                voltage_flag = FALSE;
            }

            /* -cr117796 */
            break;
        case SIM_VOLTAGE_3V:

            if (s_sim_working_volt[card_id] != TB_SIM_WORKING_VOLT_3V)
            {
                voltage_flag = TRUE;
            }

            break;
        case SIM_VOLTAGE_3V_1_8_V:
    /* +CR218847 remove real nw flag  */
            if ( (s_sim_working_volt[card_id] != TB_SIM_WORKING_VOLT_3V) ) /* cr199148 */
            {
                voltage_flag = FALSE;
            }
    /* -CR218847 remove real nw flag  */
            break;
        case SIM_VOLTAGE_5V:

            if (s_sim_working_volt[card_id] != TB_SIM_WORKING_VOLT_5V)
            {
                voltage_flag = FALSE;
            }

            break;
        case SIM_VOLTAGE_5V_3V:

            if (s_sim_working_volt[card_id] ==TB_SIM_WORKING_VOLT_1_8V) //TB_SIM_WORKING_VOLT_1D8V)
            {
                voltage_flag = TRUE;
            }

            break;
        case SIM_VOLTAGE_5V_3V_1_8_V:
    /* +CR218847 remove real nw flag  */
            if ( (s_sim_working_volt[card_id] == TB_SIM_WORKING_VOLT_1_8V) ) /* cr199148 */
            {
                voltage_flag = FALSE;
            }
    /* -CR218847 remove real nw flag  */
            break;
        default:
            break;
    }

    if (voltage_flag)
    {
        *is_need_reset = TRUE;
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[SIM_SelMFAckClkVol] Expected_Voltage != Working_Voltage ,SIM Reset Needed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2762_112_2_17_23_6_35_1176,(uint8*)"d", TRACE_CARD_ID);
        return;
    }

    ReportCardStatus (card_id, TB_SIM_INSERTED_OK);
}

/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim1DriverTimerExpired (
    uint32 lparam   //the function's input parameter
)
{
    uint32 register_value = CHIP_REG_GET (GR_GEN0);
    uint32 sim_enable_flag = (register_value & GEN0_SIM0_EN);

    if (!sim_enable_flag)
    {
        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] ERROR:SIM1 Module Disalbe but Receive Timer Interrupt[%d]!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2784_112_2_17_23_6_35_1177,(uint8*)"d",lparam);
        return;
    }

    /*SCI_TRACE_LOW("USIM:: DriverTimerExpired[0x%x,0x%x,0x%x,0x%x]", \
     lparam, s_sim_rxfifo_count, s_sim_rxfifo_int_count,s_sim_rx_buf.rx_expected_len);
    */

    //the atr wait timer
    if (1 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] ATR Wait timer expired"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2792_112_2_17_23_6_35_1178,(uint8*)"");
        USIM_PRINT_RX_BUF(USIMDRV_CARD_ONE);

        /*  20070717 check the sim card */
        if (0 == s_sim_rx_buf[USIMDRV_CARD_ONE].cur_pos)
        {
            /* +binli opt sim */
            /*  if no data received we need try the next volt */
            /* if(g_sim_ut_flag) */
            {
                s_sim_pow_reset_times[USIMDRV_CARD_ONE] = TB_SIM_MAX_POWER_RESET_TIMES;
            }
            /* -binli opt sim */
        }

        if (!ResetSIMCard (USIMDRV_CARD_ONE,TB_SIM_RESET_FOR_POWER))
        {
            NotifyExceptionError (USIMDRV_CARD_ONE, TB_SIMEXP_CARD_OUT);
        }
    }
    //parity change timer
    else if (2 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] Parity_Change_Timer expired"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2815_112_2_17_23_6_35_1179,(uint8*)"");

        if (!ResetSIMCard (USIMDRV_CARD_ONE, TB_SIM_RESET_FOR_PARITY))
        {
            NotifyExceptionError (USIMDRV_CARD_ONE,TB_SIMEXP_RX_PARITY_ERROR);
        }
    }
    //the bit convention timer
    else if (3 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] Bit_Convention_Timer expired"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2825_112_2_17_23_6_35_1180,(uint8*)"");

        if (!ResetSIMCard (USIMDRV_CARD_ONE,TB_SIM_RESET_FOR_BT))
        {
            NotifyExceptionError (USIMDRV_CARD_ONE,TB_SIMEXP_CARD_UNRESPONSE);
        }
    }
    //status word timer
    else if (4 == lparam)
    {
        // 表明没有收到期望长度，但是收到了sw1 sw2
        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] Status_Word_Timer expired SW1[0x%02x], SW2[0x%02x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2837_112_2_17_23_6_35_1181,(uint8*)"dd",s_sim_rx_buf[USIMDRV_CARD_ONE].buf[s_sim_rx_buf[USIMDRV_CARD_ONE].cur_pos - 2],s_sim_rx_buf[USIMDRV_CARD_ONE].buf[s_sim_rx_buf[USIMDRV_CARD_ONE].cur_pos - 1]);
        s_sim_register_map[USIMDRV_CARD_ONE]->ie &= ~SIMIE_B_RX_FULL;   /*NEWMS00106599 E*/
        SendResponseUp(USIMDRV_CARD_ONE);
    }
    //bwt timer
    else if (5 == lparam)
    {
        uint8 temp;
        temp = (uint8) GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_ONE]);

        if (0 == temp)
        {
            ErrorHandler (USIMDRV_CARD_ONE,T1_ERROR_CODE_BWT_TIMER_OUT);
        }
        else
        {
            /* data received    */
        }

        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] BWT_Timer expired,Tick[0x%x],Rx_Cur_Pos[0x%x],Rx_Blk_Ext_Byte_Cnt[0x%x],Temp[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2858_112_2_17_23_6_35_1182,(uint8*)"dddd", SCI_GetTickCount(), s_sim_rx_buf[USIMDRV_CARD_ONE].cur_pos, s_sim_rx_block[USIMDRV_CARD_ONE].cur_ext_byte_count,temp);

        //SIM_PrintRxBuf();

    }
    //wait response timer
    else if (6 == lparam)
    {
        /* +cr119609 */
        /* +cr120672 */
        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired ,Tick[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2868_112_2_17_23_6_35_1183,(uint8*)"d", SCI_GetTickCount());
        /* -cr120672 */
        {
            //            DMA_CHx_CTL_T * dma_ch_in  = (DMA_CHx_CTL_T * )(DMA_REG_BASE + 0x400 + 0x20*DMA_SIM0_RX);
            //            SCI_TRACE_LOW("Eddie.Wang DMA TEST:: WRT time out,rx_fifo_count=%d, DMA total_count=%d",
            //                                        (uint8)GET_RX_FIFO_COUNT(s_sim_register_map), dma_ch_in->size);
        }

        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired, SIM_Status[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2876_112_2_17_23_6_36_1184,(uint8*)"d", s_sim_status[USIMDRV_CARD_ONE]);

        if ( (s_sim_status[USIMDRV_CARD_ONE] != TB_SIM_PROCESSING_PPS) &&
                (s_sim_status[USIMDRV_CARD_ONE] != TB_SIM_PROCESSING_ATR) &&
                (s_sim_status[USIMDRV_CARD_ONE] != TB_SIM_WAITING_FOR_ATR))
        {
            NotifyExceptionError (USIMDRV_CARD_ONE,TB_SIMEXP_CARD_UNRESPONSE);
        }
        else
        {
            if ( (TB_SIM_WAITING_FOR_ATR == s_sim_status[USIMDRV_CARD_ONE])  ||
                    (TB_SIM_PROCESSING_ATR == s_sim_status[USIMDRV_CARD_ONE]))
            {
                /* +cr204609 */
                SIM_PowerOffSIMEx(USIMDRV_CARD_ONE);
                NotifyExceptionError (USIMDRV_CARD_ONE,TB_SIMEXP_CARD_OUT);
                /* -cr204609 */
                return;
            }

            ResetDriver(USIMDRV_CARD_ONE);
            s_previous_pps_error[USIMDRV_CARD_ONE] = TRUE;

            if (!ResetSIMCard (USIMDRV_CARD_ONE,TB_SIM_RESET_NO_CHANGE))
            {
                //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired, add NotifyExceptionError (TB_SIM_PROCESSING_PPS) !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2901_112_2_17_23_6_36_1185,(uint8*)"");
                NotifyExceptionError (USIMDRV_CARD_ONE,TB_SIMEXP_CARD_OUT);
            }
        }

        /* -cr119609 */

    }
    /*  bo.chen add wwt timer out   */
    else if (7 == lparam)
    {
        if (0 == (uint8) GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_ONE]))
        {
            /*  no data after wwt timer expired */
            //SCI_TRACE_LOW("usimdrv:: t0 wwt timer expired reset [0x%x]", s_auth_inited);
            /*  20070731 if authentication is sent stop the wwt timer   */
            if (g_wwt_no_sim_flag[USIMDRV_CARD_ONE])  /* CR218847 remove real nw flag  */
            {
                NotifyExceptionError (USIMDRV_CARD_ONE,TB_SIMEXP_CARD_UNRESPONSE);
                SIM_PowerOffSIMEx(USIMDRV_CARD_ONE);
            }
        }
        else
        {
            /* data received    */
        }

    }
    else if (8 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] T1_CWT_Timer expired, Tick[0x%x], Parity[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2932_112_2_17_23_6_36_1186,(uint8*)"dd", SCI_GetTickCount(), s_rx_parity_recv[USIMDRV_CARD_ONE]);
        /*  invalid len received, check the data received and edc   */
        s_rx_parity_recv[USIMDRV_CARD_ONE] = FALSE;
        ErrorHandler (USIMDRV_CARD_ONE,T1_ERROR_CODE_EDC_ERROR);
        ClearRxBlock(USIMDRV_CARD_ONE);
    }
    else if (9 == lparam)
    {

        if (0 != GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_ONE]))
        {

            //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] RX_Timeout_Timer expired, Tick[0x%x],Rcv_Byte_Cnt[0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2945_112_2_17_23_6_36_1187,(uint8*)"dd", SCI_GetTickCount(),GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_ONE]));
/* +interrupt_refactor_dev */
            SCI_DisableIRQ();
            RxFifoFullIntP(USIMDRV_CARD_ONE);
            SCI_RestoreIRQ();
            //RxFifoFullHisrP();
            SIMIntHisrFunc(1, NULL);
/* -interrupt_refactor_dev */
        }
        else
        {
            /*  set rx timeout timer 20070730   */
            if (SCI_IsTimerActive (s_sim_rx_timeout_timer[USIMDRV_CARD_ONE]))
            {
                SCI_DeactiveTimer (s_sim_rx_timeout_timer[USIMDRV_CARD_ONE]);
            }

            USIMDRV_CHANGE_TIMER(USIMDRV_CARD_ONE,s_sim_rx_timeout_timer[USIMDRV_CARD_ONE], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);
            SCI_ActiveTimer (s_sim_rx_timeout_timer[USIMDRV_CARD_ONE]);
        }
    }
    /* +cr210334 */
    else if (10 == lparam)
    {
         g_user_mode_flag[USIMDRV_CARD_ONE] = TRUE;
         //SCI_TRACE_LOW:"USIMDRV:[0]: [Sim1DriverTimerExpired] ERROR:Invalid_Timer g_user_mode_flag is TRUE."
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2968_112_2_17_23_6_36_1188,(uint8*)"");
    }
    /* -cr210334 */

}
/**********************************************************************
//    Description:
//      the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim2DriverTimerExpired (
    uint32 lparam   //the function's input parameter
)
{
    uint32 register_value = CHIP_REG_GET (GR_GEN0);
    uint32 sim_enable_flag = (register_value & GEN0_SIM1_EN);

    if (!sim_enable_flag)
    {
        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] ERROR:SIM2 Module Disalbe but Receive Timer Interrupt[%d]!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2988_112_2_17_23_6_36_1189,(uint8*)"d",lparam);
        return;
    }

    /*SCI_TRACE_LOW("USIM:: DriverTimerExpired[0x%x,0x%x,0x%x,0x%x]", \
     lparam, s_sim_rxfifo_count, s_sim_rxfifo_int_count,s_sim_rx_buf.rx_expected_len);
    */
    //the atr wait timer
    if (1 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] ATR_Wait_Timer expired"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_2995_112_2_17_23_6_36_1190,(uint8*)"");
        USIM_PRINT_RX_BUF(USIMDRV_CARD_TWO);

        /*  20070717 check the sim card */
        if (0 == s_sim_rx_buf[USIMDRV_CARD_TWO].cur_pos)
        {
            /* +binli opt sim */
            /*  if no data received we need try the next volt */
            /* if(g_sim_ut_flag) */
            {
                s_sim_pow_reset_times[USIMDRV_CARD_TWO] = TB_SIM_MAX_POWER_RESET_TIMES;
            }
            /* -binli opt sim */
        }

        if (!ResetSIMCard (USIMDRV_CARD_TWO,TB_SIM_RESET_FOR_POWER))
        {
            NotifyExceptionError (USIMDRV_CARD_TWO, TB_SIMEXP_CARD_OUT);
        }
    }
    //parity change timer
    else if (2 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] Parity_Change_Timer expired"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3018_112_2_17_23_6_36_1191,(uint8*)"");

        if (!ResetSIMCard (USIMDRV_CARD_TWO, TB_SIM_RESET_FOR_PARITY))
        {
            NotifyExceptionError (USIMDRV_CARD_TWO,TB_SIMEXP_RX_PARITY_ERROR);
        }
    }
    //the bit convention timer
    else if (3 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] Bit_Convention_Timer expired"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3028_112_2_17_23_6_36_1192,(uint8*)"");

        if (!ResetSIMCard (USIMDRV_CARD_TWO,TB_SIM_RESET_FOR_BT))
        {
            NotifyExceptionError (USIMDRV_CARD_TWO,TB_SIMEXP_CARD_UNRESPONSE);
        }
    }
    //status word timer
    else if (4 == lparam)
    {
        // 表明没有收到期望长度，但是收到了sw1 sw2
        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] Status_Word_Timer expired, SW1[0x%02x], SW2[0x%02x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3040_112_2_17_23_6_36_1193,(uint8*)"dd",s_sim_rx_buf[USIMDRV_CARD_TWO].buf[s_sim_rx_buf[USIMDRV_CARD_TWO].cur_pos - 2],s_sim_rx_buf[USIMDRV_CARD_TWO].buf[s_sim_rx_buf[USIMDRV_CARD_TWO].cur_pos - 1]);
			 s_sim_register_map[USIMDRV_CARD_TWO]->ie &= ~SIMIE_B_RX_FULL;   /*NEWMS00106599 E*/
        SendResponseUp(USIMDRV_CARD_TWO);
    }
    //bwt timer
    else if (5 == lparam)
    {
        uint8 temp;
        temp = (uint8) GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_TWO]);

        if (0 == temp)
        {
            ErrorHandler (USIMDRV_CARD_TWO,T1_ERROR_CODE_BWT_TIMER_OUT);
        }
        else
        {
            /* data received    */
        }

        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] BWT_Timer expired,Tick[0x%x],Rx_Cur_Pos[0x%x],Rx_Blk_Ext_Byte_Cnt[0x%x],Temp[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3061_112_2_17_23_6_36_1194,(uint8*)"dddd", SCI_GetTickCount(), s_sim_rx_buf[USIMDRV_CARD_TWO].cur_pos, s_sim_rx_block[USIMDRV_CARD_TWO].cur_ext_byte_count,temp);

        //SIM_PrintRxBuf();

    }
    //wait response timer
    else if (6 == lparam)
    {
        /* +cr119609 */
        /* +cr120672 */
        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired ,Tick[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3071_112_2_17_23_6_36_1195,(uint8*)"d", SCI_GetTickCount());
        /* -cr120672 */
        {
            //            DMA_CHx_CTL_T * dma_ch_in  = (DMA_CHx_CTL_T * )(DMA_REG_BASE + 0x400 + 0x20*DMA_SIM0_RX);
            //            SCI_TRACE_LOW("Eddie.Wang DMA TEST:: WRT time out,rx_fifo_count=%d, DMA total_count=%d",
            //                                        (uint8)GET_RX_FIFO_COUNT(s_sim_register_map), dma_ch_in->size);
        }

        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired, SIM_Status[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3079_112_2_17_23_6_36_1196,(uint8*)"d", s_sim_status[USIMDRV_CARD_TWO]);

        if ( (s_sim_status[USIMDRV_CARD_TWO] != TB_SIM_PROCESSING_PPS) &&
                (s_sim_status[USIMDRV_CARD_TWO] != TB_SIM_PROCESSING_ATR) &&
                (s_sim_status[USIMDRV_CARD_TWO] != TB_SIM_WAITING_FOR_ATR))
        {
            NotifyExceptionError (USIMDRV_CARD_TWO,TB_SIMEXP_CARD_UNRESPONSE);
        }
        else
        {
            if ( (TB_SIM_WAITING_FOR_ATR == s_sim_status[USIMDRV_CARD_TWO])  ||
                    (TB_SIM_PROCESSING_ATR == s_sim_status[USIMDRV_CARD_TWO]))
            {
                /* +cr204609 */
                SIM_PowerOffSIMEx(USIMDRV_CARD_TWO);
                NotifyExceptionError (USIMDRV_CARD_TWO,TB_SIMEXP_CARD_OUT);
                /* -cr204609 */
                return;
            }

            ResetDriver(USIMDRV_CARD_TWO);
            s_previous_pps_error[USIMDRV_CARD_TWO] = TRUE;

            if (!ResetSIMCard (USIMDRV_CARD_TWO,TB_SIM_RESET_NO_CHANGE))
            {
                //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired, NotifyExceptionError (TB_SIM_PROCESSING_PPS) !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3104_112_2_17_23_6_36_1197,(uint8*)"");
                NotifyExceptionError (USIMDRV_CARD_TWO,TB_SIMEXP_CARD_OUT);
            }
        }

        /* -cr119609 */

    }
    /*  bo.chen add wwt timer out   */
    else if (7 == lparam)
    {
#if 0
        //SCI_TRACE_LOW:"usimdrv:: t0 wwt timer expired [0x%x],rcv byte[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3117_112_2_17_23_6_36_1198,(uint8*)"dd", SCI_GetTickCount(), GET_RX_FIFO_COUNT (s_sim_register_map));
#endif

        if (0 == (uint8) GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_TWO]))
        {
            /*  no data after wwt timer expired */
            //SCI_TRACE_LOW("usimdrv:: t0 wwt timer expired reset [0x%x]", s_auth_inited);
            /*  20070731 if authentication is sent stop the wwt timer   */
            if (g_wwt_no_sim_flag[USIMDRV_CARD_TWO])  /* CR218847 remove real nw flag  */
            {
                NotifyExceptionError (USIMDRV_CARD_TWO,TB_SIMEXP_CARD_UNRESPONSE);
                SIM_PowerOffSIMEx(USIMDRV_CARD_TWO);
            }
        }
        else
        {
            /* data received    */
        }

    }
    else if (8 == lparam)
    {
        //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] T1_CWT_Timer expired, Tick[0x%x], Parity[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3140_112_2_17_23_6_36_1199,(uint8*)"dd", SCI_GetTickCount(), s_rx_parity_recv[USIMDRV_CARD_TWO]);
        /*  invalid len received, check the data received and edc   */
        s_rx_parity_recv[USIMDRV_CARD_ONE] = FALSE;
        ErrorHandler (USIMDRV_CARD_TWO,T1_ERROR_CODE_EDC_ERROR);
        ClearRxBlock(USIMDRV_CARD_TWO);
    }
    else if (9 == lparam)
    {

        if (0 != GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_TWO]))
        {

            //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] RX_Timeout_Timer expired, Tick[0x%x],Rcv_Byte_Cnt[0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3153_112_2_17_23_6_36_1200,(uint8*)"dd", SCI_GetTickCount(),GET_RX_FIFO_COUNT (s_sim_register_map[USIMDRV_CARD_TWO]));
/* +interrupt_refactor_dev */
            SCI_DisableIRQ();
            RxFifoFullIntP(USIMDRV_CARD_TWO);
            SCI_RestoreIRQ();
            //RxFifoFullHisrP();
            SIMIntHisrFunc(1, NULL);
/* -interrupt_refactor_dev */
        }
        else
        {
            /*  set rx timeout timer 20070730   */
            if (SCI_IsTimerActive (s_sim_rx_timeout_timer[USIMDRV_CARD_TWO]))
            {
                SCI_DeactiveTimer (s_sim_rx_timeout_timer[USIMDRV_CARD_TWO]);
            }

            USIMDRV_CHANGE_TIMER(USIMDRV_CARD_TWO,s_sim_rx_timeout_timer[USIMDRV_CARD_TWO], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);
            SCI_ActiveTimer (s_sim_rx_timeout_timer[USIMDRV_CARD_TWO]);
        }
    }
    /* +cr210334 */
    else if (10 == lparam)
    {
         g_user_mode_flag[USIMDRV_CARD_TWO] = TRUE;
         //SCI_TRACE_LOW:"USIMDRV:[1]: [Sim2DriverTimerExpired] ERROR:Invalid_Timer g_user_mode_flag is TRUE."
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3176_112_2_17_23_6_36_1201,(uint8*)"");
    }
    /* -cr210334 */

}
/**********************************************************************
//    Description:
//      Clear the PPS info structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearPPSInfo (int32 card_id)
{
    s_pps_info[card_id].is_current = FALSE;
    //note:now we only support the default sppeed pps procedure,
    //but not the enhanced speed pps procedure
    s_pps_info[card_id].pps_data = s_default_pps_data;
    s_pps_info[card_id].pps_type = TB_SIM_PPS_DEFAULT_SPEED;
}
/**********************************************************************
//    Description:
//      Clear the ATR data buffer for initialize
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearATRData (int32 card_id)
{
    s_atr_data_buf[card_id].is_x_u_present = FALSE;
    s_atr_data_buf[card_id].Tai_pos = 0;
    s_atr_data_buf[card_id].Tbi_pos = 0;
    s_atr_data_buf[card_id].Tci_pos = 0;
    s_atr_data_buf[card_id].Tdi_pos = TB_SIM_ATR_T0_POS;
    s_atr_data_buf[card_id].td_count = 0;
    s_atr_data_buf[card_id].hb_count = 0;
    s_atr_data_buf[card_id].first_occur_protocol = TRUE;
    s_atr_data_buf[card_id].first_ta1_after_t1 = TRUE;
    s_atr_data_buf[card_id].first_ta1_after_t15 = TRUE;
    s_atr_data_buf[card_id].first_tb1_after_t1 = TRUE;
    s_atr_data_buf[card_id].first_tc1_after_t1 = TRUE;
    s_atr_data_buf[card_id].hisbyte_is_end = FALSE;
    s_atr_data_buf[card_id].is_atr_data_end = FALSE;
    s_atr_data_buf[card_id].is_support_t1 = FALSE;
    s_atr_data_buf[card_id].is_t15_exist = FALSE;
    s_atr_data_buf[card_id].is_ta2_exist = FALSE;
    s_atr_data_buf[card_id].BWT = TB_SIM_DEFAULT_FACTOR_BWT;

    // 默认使用的协议是0
    s_atr_data_buf[card_id].atr_data.t_protocol = TB_SIM_T_0_PROTOCOL;
    s_atr_data_buf[card_id].atr_data.F = TB_SIM_DEFAULT_FACTOR_F;
    s_atr_data_buf[card_id].atr_data.D = TB_SIM_DEFAULT_FACTOR_D;
    s_atr_data_buf[card_id].atr_data.I = TB_SIM_DEFAULT_FACTOR_I;
    s_atr_data_buf[card_id].atr_data.P = TB_SIM_DEFAULT_FACTOR_P;
    s_atr_data_buf[card_id].atr_data.N = TB_SIM_DEFAULT_FACTOR_N;
    s_atr_data_buf[card_id].atr_data.X = TB_SIM_DEFAULT_FACTOR_X;
    s_atr_data_buf[card_id].atr_data.U = TB_SIM_DEFAULT_FACTOR_U;
    s_atr_data_buf[card_id].atr_data.IFSC = TB_SIM_DEFAULT_FACTOR_IFSC;
    s_atr_data_buf[card_id].atr_data.CWI = TB_SIM_DEFAULT_FACTOR_CWI;
    s_atr_data_buf[card_id].atr_data.BWI = TB_SIM_DEFAULT_FACTOR_BWI;
    s_atr_data_buf[card_id].atr_data.EDC = TB_SIM_DEFAULT_FACTOR_EDC;
    s_atr_data_buf[card_id].atr_data.his_byte_len = 0;
}

/**********************************************************************
//    Description:
//      Clear the Tx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearTxBuf (int32 card_id)
{
    s_sim_tx_buf[card_id].cur_pos = 0;
    s_sim_tx_buf[card_id].tx_data_len = 0;
}

/**********************************************************************
//    Description:
//      Clear the Rx buf of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearRxBuf (int32 card_id)
{
    s_sim_rx_buf[card_id].check_pos = 0;
    s_sim_rx_buf[card_id].cur_pos = 0;
    s_sim_rx_buf[card_id].rx_expected_len = 0;
    s_sim_rx_buf[card_id].is_locked = TRUE;
}

/**********************************************************************
//    Description:
//      Clear the Tx block which is used to save the block information of the current sent block
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearTxBlock (int32 card_id)
{
    s_sim_tx_block[card_id].block_begin_pos = 0;
    s_sim_tx_block[card_id].block_end_pos = 0;
    s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_NOT_DEF;
}

/**********************************************************************
//    Description:
//      Clear the Rx block which is used to save the current received block information
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ClearRxBlock (int32 card_id)
{
    s_sim_rx_block[card_id].info_data_len = 0;
    s_sim_rx_block[card_id].have_edc = FALSE;
    s_sim_rx_block[card_id].cur_ext_byte_count = 0;
    s_sim_rx_block[card_id].cur_info_data_count = 0;
}
/**********************************************************************
//    Description:
//      report the SIM card insert status to the SIM manager task
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void ReportCardStatus (
    int32 card_id,
    TB_SIM_INSERT_STATUS_E insert_status   //the SIM card insert status
)
{
    TB_MSG msg;
    TB_SIM_CARD_INSERT_T card_insert;

    card_insert.insert_status = insert_status;
    card_insert.working_volt = s_sim_working_volt[card_id];
    card_insert.card_id = card_id;
    /* amy.wang delete    card_insert.atr_data = s_atr_data_buf.atr_data;*/
    msg.message = SIM_CARD_INSERT;
    msg.wparam = 0;
    msg.lparam = &card_insert;

    /*  bo.chen 20060417 add log for card status */
    USIM_PRINT_RX_BUF(card_id);
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[ReportCardStatus] insert_status[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3314_112_2_17_23_6_36_1202,(uint8*)"dd", TRACE_CARD_ID, card_insert.insert_status);
    DriverCallBack (card_id, &msg);

#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }
    }
    else
    {
        /*  20070730    */
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }
    }

#else

    /*  20070730    */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

#endif

    SCI_SIM_EnableDeepSleep (1);
}

/*********************************************************************
    Description:
      report the SIM card reset status to the SIM task
    Global resource dependence :
    Author:Raislin.kong
    Note:
***********************************************************************/
static void ReportCardResetStatus (
    int32 card_id,
    TB_SIM_RESET_STATUS_E reset_status   /*the SIM card reset status*/
)
{
    TB_MSG msg;
    TB_SIM_CARD_RESET_T card_reset;

    card_reset.reset_status = reset_status;
    card_reset.working_volt = s_sim_working_volt[card_id];
    card_reset.card_id = card_id;
    /* amy.wang delete   card_reset.atr_data = s_atr_data_buf.atr_data;*/

    msg.message = SIM_CARD_RESET;
    msg.wparam = 0;
    msg.lparam = &card_reset;
    /* +cr224711 */
     if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
    {
        s_sim_wait_resp_timer_value[card_id] = 17000;
    }
    /* -cr224711 */
    DriverCallBack (card_id, &msg);

#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }
    }
    else
    {
        /*  20070730    */
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }
    }

#else

    /*  20070730    */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

#endif
    /* amy.wang different */
    SCI_SIM_EnableDeepSleep (1);
}

/**********************************************************************
//    Description:
//      Notify the SIM layer the exeption interrupt
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void NotifyExceptionError (
    int32 card_id,
    TB_SIM_EXCEPTION_TYPE_E exp_type //the exeption error type
)
{
    TB_MSG msg;
    TB_SIM_EXCEPTION_T exp_error;

    exp_error.exp_type = exp_type;
    exp_error.card_id = card_id;
    msg.message     = SIM_EXCEPTION_INT;
    msg.wparam      = 0;
    msg.lparam      = &exp_error;

    DriverCallBack (card_id, &msg);

#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }
    }
    else
    {
        /*  20070730    */
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }
    }

#else

    /*  20070730    */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

#endif
    SCI_SIM_EnableDeepSleep (1);
}

/* +interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      When the drive send data to the sim card.It will write the data
//    to the SIM TX ARM fifo,and let the ARM done the Transimit job.
//    when the data in the fifo below the tx fifo mark byte,The arm
//    cpu will send a TX empty interrupt.and we can write more data to
//    the TX fifo or finish the Transmit.
//    Global resource dependence:
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TxEmptyHisrP (int32 card_id)
{
    /*  20070730    */
    if (s_atr_data_buf[card_id].atr_data.t_protocol == 0)
    {
        TransmitInInt_T0(card_id);
    }
    else
    {
        TransmitInInt_T1(card_id);
    }
}
/* -interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      the really function taht do the  data transmit to the SIM CARD only for T0 protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TransmitInInt_T0 (int32 card_id)
{
    uint16 tx_transmit_num,i;

    //disable the TX empty interrupt to make sure the data transmit not be broken by
    //tx data interrupt in the data transform process
    //s_sim_register_map->ie &= ~SIMIE_B_TX_EMPTY;/* interrupt_refactor_dev */

    //get the current fifo free space
    tx_transmit_num = (uint16) (GET_TX_FIFO_COUNT (s_sim_register_map[card_id]));
    tx_transmit_num = (uint16) SIM_FIFO_BUF_SIZE - tx_transmit_num;

    //there still some instruction data to be sent
    if (s_sim_tx_buf[card_id].tx_data_len > s_sim_tx_buf[card_id].cur_pos)
    {
        // 接着发送未完的数据
        i = s_sim_tx_buf[card_id].tx_data_len - s_sim_tx_buf[card_id].cur_pos;

        //all the transmit data will can be transmited just one time
        if (i <= tx_transmit_num)
        {
            tx_transmit_num = i;
        }

        //fill the fifo buf by the unsent instuction data
        for (i = 0; i < tx_transmit_num; i++)
        {
            s_sim_register_map[card_id]->tx = (uint32) s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos];
            s_sim_tx_buf[card_id].cur_pos++;
        }

        //if all the data of the apdu has been sent,start the wait response timer
        if (s_sim_tx_buf[card_id].cur_pos == s_sim_tx_buf[card_id].tx_data_len)
        {
            // 全部发送完
            USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
            SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);

            if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
            {
                SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
            }

            USIMDRV_CHANGE_TIMER(card_id,s_sim_wwt_timer[card_id], DriverTimerExpired,s_wwt_timer_value[card_id]);
            SCI_ActiveTimer (s_sim_wwt_timer[card_id]);
        }

        //enable the TX to spring the next data transmit
        s_sim_register_map[card_id]->ie |= SIMIE_B_TX_EMPTY;
    }
    //all the command instruciton has been transmit
    else
    {
        //clear the tx buf
        //没有数据要发了
        ClearTxBuf(card_id);
    }
}
/**********************************************************************
//    Description:
//      the really function taht do the  data transmit to the SIM CARD only for T1 protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TransmitInInt_T1 (int32 card_id)
{
    uint16 tx_transmit_num,i;
    BOOLEAN is_data_sent_all = FALSE;

    //disable the TX empty interrupt to make sure the data transmit not be broken by
    //tx data interrupt in the data transform process
   // s_sim_register_map->ie &= ~SIMIE_B_TX_EMPTY; /* -interrupt_refactor_dev */

    //get the current fifo free space
    tx_transmit_num = (uint16) (GET_TX_FIFO_COUNT (s_sim_register_map[card_id]));
    tx_transmit_num = (uint16) SIM_FIFO_BUF_SIZE - tx_transmit_num;

    //there still some instruction data to be sent
    if (s_sim_tx_buf[card_id].tx_data_len > s_sim_tx_buf[card_id].cur_pos)
    {
        //whether some data need to be sent in current block
        if (s_sim_tx_buf[card_id].cur_pos < s_sim_tx_block[card_id].block_end_pos)
        {
            i = s_sim_tx_block[card_id].block_end_pos - s_sim_tx_buf[card_id].cur_pos;

            //all the transmit data will can be transmited just one time
            if (i <= tx_transmit_num)
            {
                is_data_sent_all = TRUE;
                tx_transmit_num = i;
            }
        }
        else
        {
            return;
        }

        //fill the fifo buf by the unsent instuction data
        for (i = 0; i < tx_transmit_num; i++)
        {
            s_sim_register_map[card_id]->tx = (uint32) s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos];
            s_sim_tx_buf[card_id].cur_pos++;
        }

        //start the bwt timer
        if (is_data_sent_all)
        {
            USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DriverTimerExpired,s_atr_data_buf[card_id].BWT);
            SCI_ActiveTimer (s_sim_bwt_timer[card_id]);
        }

        //enable the TX to spring the next data transmit
        s_sim_register_map[card_id]->ie |= SIMIE_B_TX_EMPTY;
    }
    //all the command instruciton has been transmit
    else
    {
        //clear the tx buf
        /*  20070629 donot clear tx buf until we send data again    */
        // ClearTxBuf();
    }
}
/* +interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      ARM receive data from SIM card and together it to the RX fifo
//    when the data up to the RX_mark byte,ARM CPU will send a RX full
//    interrupt to tell the drive get data from the fifo.This is the
//    interrupt handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxFifoFullHisrP (TB_SIM_DATA_BUF_T* sim_int_data)
{
    int32 card_id = sim_int_data->card_id;

    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);

    SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);

    if (s_atr_data_buf[card_id].atr_data.t_protocol == 0)
    {
        RxFifoFullIntP_T0(sim_int_data);
    }
    else
    {
        RxFifoFullIntP_T1(sim_int_data);
    }
}
/* -interrupt_refactor_dev */
/**********************************************************************
//    Description:
//    save the received data to the rx buffer and check the data in the buffer
//  only for T0 Protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxFifoFullIntP_T0 (TB_SIM_DATA_BUF_T *sim_int_data)
{
    uint8 temp;
    uint16 i = 0;
/* +interrupt_refactor_dev */
    uint8   fifo_buf_end_pos = 0;
    uint8   fifo_buf[16];
	int32 card_id = sim_int_data->card_id;
/*+NEWMS00106599 E*/
    if (s_is_sim_out[card_id])
    {
        //SCI_TraceLow:"USIMDRV:[%d] s_is_sim_out = TRUE;RX:Need De-active SIM!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3662_112_2_17_23_6_37_1203,(uint8*)"d",card_id);
        s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_FULL;
        /*Notify upper layer*/
        NotifyExceptionError (card_id, TB_SIMEXP_CARD_OUT);
        return;
    }
/*-NEWMS00106599 E*/
    //disable the rx interrupt
   // s_sim_register_map->ie &= ~SIMIE_B_RX_FULL;

    s_sim_rxfifo_int_count[card_id]++;

    SCI_MEMSET(fifo_buf, 0x0, sizeof(fifo_buf));

    SCI_MEMCPY(fifo_buf, sim_int_data->data, sim_int_data->data_len);
    fifo_buf_end_pos = sim_int_data->data_len;
     s_sim_rxfifo_count[card_id] += sim_int_data->data_len;
    /* -interrupt_refactor_dev */

    //receive response data from the rx fifo buf
    for (i = 0; i < fifo_buf_end_pos; i++)
    {
        temp = fifo_buf[i];

        //the sim recv buf is been locked,give up the data.
        //表示rx 没有被lock，如果被lock了所有数据都将丢失掉
        if (!s_sim_rx_buf[card_id].is_locked)
        {
            //to the SIM command,there are ACK byte and NULL byte should be process
            //different with the command SIM data
            if ( (!s_have_ack_byte[card_id])  && 
                (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]) && 
                (!s_have_received_response_data[card_id]))  /*NEWMS001087836 E*/
            {
                //没有得到确认字节,cur_pos = 0，状态是接收响应

                //ack
                if ( (s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS] == temp)
                        || (s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS] + 1 == temp))
                {
                    /* +binli opt sim */
                    uint8 water_mark = 0;
                    water_mark = CAL_RX_INT_MARK (s_sim_rx_buf[card_id].rx_expected_len);
                    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id], water_mark);
                    /* -binli opt sim */
                    //是 ins和是ins + 1，表明连续发送
                    TxEmptyHisrP(card_id); /* -interrupt_refactor_dev */
                    s_have_ack_byte[card_id] = TRUE;
                }
                else if ( (0xE0 == temp) &&
                          (0xF2 == s_sim_tx_buf[card_id].buf[1]))  //the instruction should be a "STATUS" instruction
                {
                    SCI_TRACE_LOW ("USIMDRV:[%d]:[RxFifoFullIntP_T0] 0xF2 indentify to 0xE0 by the chip!!!", TRACE_CARD_ID); //trace to indicate the "0xe0" problem appears for
                    //debug,should be delete in release code for better performance in interrupter
                    s_have_ack_byte[card_id] = TRUE;
                }
                else if ( ( (uint8) (~s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS]) == temp)
                          || ( (uint8) (~ (s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS] + 1)) == temp))
                {
                    //是 非ins和是非ins + 1，表明发送一个字节
                    // 没有全部发完，表示需要等待下一个ack_byte，认为现在没有收到ack_byte，返回false
                    // 没有字节需要发送或者发完了最后一个字节，返回true
                    s_have_ack_byte[card_id] = SendOneByteToSIM(card_id);

                    /* +binli opt sim */
                    if (s_have_ack_byte[card_id])
                    {
                        uint8 water_mark = 0;
                        water_mark = CAL_RX_INT_MARK (s_sim_rx_buf[card_id].rx_expected_len);
                        s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id], water_mark);
                    }

                    /* -binli opt sim */
                    if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                    {
                        SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
                    }
                    
					USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id], DriverTimerExpired, s_sim_wait_resp_timer_value[card_id]);
                    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
                }
                //it's the null data from the SIM
                else if (TB_SIM_RESPONSE_NULL_BYTE == temp)
                {
                    s_null_byte_count[card_id]++;

                     /* cr217123  delete code */
                }
                else
                {
                    //不是ack和NULL
                    s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos] = temp;
                    s_sim_rx_buf[card_id].cur_pos++;
                    /* sim_response_data_present = TRUE; */
                    s_have_received_response_data[card_id] = TRUE;

                    if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                    {
                        s_sim_rx_buf[card_id].cur_pos = 0;
                    }
                }
            }
            //in all other case,this byte data is a real byte SIM data
            else
            {
                // more ack need, and deal the null between real data and ack
                if ( (temp == TB_SIM_RESPONSE_NULL_BYTE) &&
                        /* (!sim_response_data_present)&&*/
                        (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]) &&
                        (!s_have_received_response_data[card_id]))
                {
                    //收到ack字节后，并且是鉴权指令才把0x60当作NULL byte，否则为有效数据
                    /* +cr156002 */
                    if (s_have_ack_byte[card_id] /* && (!s_auth_inited)*/)
                        /* -cr156002 */
                    {
                        // 非ack，因为未必就会有ack和null，而是真实的数据
                        s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos] = temp;
                        s_sim_rx_buf[card_id].cur_pos++;
                        s_have_received_response_data[card_id] = TRUE;

                        if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                        {
                            //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T0] Section1, cur_pos[%d]"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3783_112_2_17_23_6_37_1204,(uint8*)"dd", TRACE_CARD_ID, s_sim_rx_buf[card_id].cur_pos);
                            s_sim_rx_buf[card_id].cur_pos = 0;
                        }
                    }
                    else
                    {
                        s_null_byte_count[card_id]++;
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T0] NULL byte recieved before ACK!!!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3790_112_2_17_23_6_37_1205,(uint8*)"d", TRACE_CARD_ID);

                        /* cr217123  delete code */

                        // the last ack has been received and , all data have been send
                        // and at the same time s_sim_wait_resp_timer has been activate
                        // so when we now receive 0x60, we must change s_sim_wait_resp_timer
                        /* +cr217123 */
                        if (s_null_byte_count[card_id] <= 60 )
                        {
                        if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                        {
                            SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
                        }
                        USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
                        SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
                    }
                        /* -cr217123 */
                    }
                }
                else
                {
                    // 非ack或者NULL，因为未必就会有ack和null，而是真实的数据
                    s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos] = temp;
                    s_sim_rx_buf[card_id].cur_pos++;

                    /* +cr151346 */
                    if ( (temp == TB_SIM_RESPONSE_NULL_BYTE))
                    {
                         /* +cr217123  */
                        s_null_byte_count[card_id] ++;
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T0] NULL byte(0x60) received!!!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3821_112_2_17_23_6_37_1206,(uint8*)"d",TRACE_CARD_ID);
                        if (s_null_byte_count[card_id] <= 60 )
                        {
                            USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
                            SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
                        }
                         /* -cr217123 */
                    }

                    /* -cr151346*/
                    s_have_received_response_data[card_id] = TRUE;

                    if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                    {
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T0] Section2, cur_pos[%d]"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3835_112_2_17_23_6_37_1207,(uint8*)"dd", TRACE_CARD_ID,s_sim_rx_buf[card_id].cur_pos);
                        s_sim_rx_buf[card_id].cur_pos = 0;
                    }
                }
            }
        }
    }

    //if there are new rx data received,check the rx data
    if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
    {
        /*  we received some byte so we need stop wwt timer which will start
            if the next byte is waited  */
        SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
    }

    // 如果没有被lock应该都接收完了fifo中的数据,现在接收到了一些新的数据((s_sim_rx_buf.cur_pos > s_sim_rx_buf.check_pos))
    if ( (!s_sim_rx_buf[card_id].is_locked) &&
            (s_sim_rx_buf[card_id].cur_pos > s_sim_rx_buf[card_id].check_pos))
    {
        //the SIM driver timer is active,then dective the driver timer
        if (SCI_IsTimerActive (s_sim_status_word_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_status_word_timer[card_id]);
        }

        CheckReceiveBuf(card_id);
    }
/*+NEWMS00106599 E*/
    if (s_sim_is_processing_cmd[card_id])
    {
        //enable the rx full interrupt
        if (!s_is_sim_out[card_id])
            s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
    }
    else if (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id])
    {
        /*Disable RX_FULL interrupt*/
        s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_FULL;
    }
/*-NEWMS00106599 E*/
}

/**********************************************************************
//    Description:
//    save the received data to the rx buffer and check the data in the buffer
//  only for T1 Protocol
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxFifoFullIntP_T1 (TB_SIM_DATA_BUF_T* sim_int_data)
{
    uint8 temp;
    uint16 i,rx_fifo_count = 0;
    int32 card_id = sim_int_data->card_id;

/* +interrupt_refactor_dev */
    //disable the rx interrupt
    //s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_FULL;
    //get the rx data number
   // rx_fifo_count = (uint8) GET_RX_FIFO_COUNT (s_sim_register_map[card_id]);
    rx_fifo_count = sim_int_data->data_len;
/* -interrupt_refactor_dev */
    /*  20070614    bwt should be stopped*/
    if (SCI_IsTimerActive (s_sim_bwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_bwt_timer[card_id]);
    }

    /*  20070628    start cwt timer
        this timer will be used as controller of whole block though
        we need check each character in the spec    */
    if ( (!SCI_IsTimerActive (s_sim_cwt_timer[card_id])) && (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]))
    {
        USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, \
                         (TB_SIM_BLOCK_EXT_BYTE_NUM + 100) * s_cwt_timer_value[card_id]);
        SCI_ActiveTimer (s_sim_cwt_timer[card_id]);
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T1] Tick[0x%x], CWT[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3909_112_2_17_23_6_38_1208,(uint8*)"ddd", TRACE_CARD_ID,SCI_GetTickCount(), s_cwt_timer_value[card_id]);
    }

    //receive response data from the rx fifo buf
    for (i = 0; i < rx_fifo_count; i++)
    {
/* +interrupt_refactor_dev */
        //temp = (uint8) s_sim_register_map[card_id]->rx;
        temp = sim_int_data->data[i];
/* -interrupt_refactor_dev */
        if (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id])
        {
            //when in receive response status,first put the prologue field into the rx block
            if (s_sim_rx_block[card_id].cur_ext_byte_count < TB_SIM_BLOCK_PRO_BYTE_NUM)
            {
                s_sim_rx_block[card_id].ext_byte[s_sim_rx_block[card_id].cur_ext_byte_count] = temp;

                if (TB_SIM_BLOCK_PRO_NAD == s_sim_rx_block[card_id].cur_ext_byte_count)
                {
                    s_sim_rx_block[card_id].base_info.valid_block = 0;

                    /*  the first byte-- nad */
                    if (0 != s_sim_rx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_NAD])
                    {
                        s_sim_rx_block[card_id].base_info.valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_NAD;
                    }
                    else
                    {
                        s_sim_rx_block[card_id].base_info.valid_block |= TB_SIM_BLOCK_FIELD_VALID_NAD;/*lint !e655 */
                    }
                }
                else if (s_sim_rx_block[card_id].cur_ext_byte_count == TB_SIM_BLOCK_PRO_PCB)
                {
                    if (!GetBlockBaseInfo (card_id,temp,&s_sim_rx_block[card_id].base_info))
                    {
                        NotifyExceptionError (card_id,TB_SIMEXP_CARD_UNRESPONSE);
                        //reject the SIM card by power the SIM Card
                        RejectSIMCard(card_id);
                        return;
                    }

                    if (TB_SIM_BLOCK_I_BLOCK == s_sim_rx_block[card_id].base_info.block_type)
                    {
                        /*  if i block received, reset this temp buf    */
                        s_sim_rx_block_buf[card_id].cur_pos = 0;
                    }
                }
                else if (s_sim_rx_block[card_id].cur_ext_byte_count == TB_SIM_BLOCK_PRO_LEN)
                {
                    /*  20070627 check the len   */
                    s_sim_rx_block[card_id].info_data_len = temp;

                    if (SCI_IsTimerActive (s_sim_cwt_timer[card_id]))
                    {
                        SCI_DeactiveTimer (s_sim_cwt_timer[card_id]);
                    }

                    /*  20070704 len is error change the timer  */
                    if (s_sim_ifsd[card_id] < s_sim_rx_block[card_id].info_data_len)
                    {
                        USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired,100);
                    }
                    else
                    {
                        if (TB_SIM_BLOCK_I_BLOCK == s_sim_rx_block[card_id].base_info.block_type)
                        {
                            USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, \
                                         (100 + s_sim_ifsd[card_id] + TB_SIM_BLOCK_EXT_BYTE_NUM + 1) * s_cwt_timer_value[card_id]);
                        }
                        else
                        {
                            USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, \
                                         (10 + TB_SIM_BLOCK_EXT_BYTE_NUM + 1) * s_cwt_timer_value[card_id]);
                        }
                    }

                    if (CheckBlockLenValid (card_id,&s_sim_rx_block[card_id].info_data_len, \
                                            &s_sim_rx_block[card_id].base_info))
                    {
                        s_sim_rx_block[card_id].base_info.valid_block |= TB_SIM_BLOCK_FIELD_VALID_LEN; /*lint !e655 */
                    }
                    else
                    {
                        s_sim_rx_block[card_id].base_info.valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_LEN;
                    }

                    SCI_ActiveTimer (s_sim_cwt_timer[card_id]);
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T1] Tick[0x%x],CWT[0x%x],Temp[0x%x],IFSD[0x%x]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_3995_112_2_17_23_6_38_1209,(uint8*)"ddddd", TRACE_CARD_ID,SCI_GetTickCount(), s_cwt_timer_value[card_id], temp, s_sim_ifsd);
                }

                s_sim_rx_block[card_id].cur_ext_byte_count++;
            }
            //put the info field into the rx buffer,edc into the rx block
            else
            {
                if (s_sim_rx_block[card_id].cur_info_data_count < s_sim_rx_block[card_id].info_data_len)
                {
                    s_sim_rx_block[card_id].cur_info_data_count++;

                    if (s_sim_rx_block[card_id].base_info.block_type == TB_SIM_BLOCK_I_BLOCK)
                    {
                        //put the information data into the rx buffer
                        s_sim_rx_block_buf[card_id].buf[s_sim_rx_block_buf[card_id].cur_pos] = temp;
                        s_sim_rx_block_buf[card_id].cur_pos++;

                        if (s_sim_rx_block_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                        {
                            s_sim_rx_block_buf[card_id].cur_pos = 0;
                        }
                    }
                    else /* it is an S-Block or R-Block,can not put the data into the rx buffer    */
                    {
                        if (9 > s_sim_rx_block[card_id].cur_ext_byte_count)
                        {
                            s_sim_rx_block[card_id].ext_byte[s_sim_rx_block[card_id].cur_ext_byte_count] = temp;
                        }

                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T1]Ext_count[0x%x], Ext_byte[0x%x,0x%x,0x%x,0x%x,0x%x],EDC_Flag[0x%x]"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4029_112_2_17_23_6_38_1210,(uint8*)"dddddddd", TRACE_CARD_ID,s_sim_rx_block[card_id].cur_ext_byte_count, s_sim_rx_block[card_id].ext_byte[0], s_sim_rx_block[card_id].ext_byte[1], s_sim_rx_block[card_id].ext_byte[2], s_sim_rx_block[card_id].ext_byte[3], s_sim_rx_block[card_id].ext_byte[4], s_sim_rx_block[card_id].have_edc);


                        s_sim_rx_block[card_id].cur_ext_byte_count++;
                    }
                }
                else
                {
                    //put the edc into the rx block
                    s_sim_rx_block[card_id].ext_byte[s_sim_rx_block[card_id].cur_ext_byte_count] = temp;
                    s_sim_rx_block[card_id].have_edc = TRUE;
                }
            }
        }
        //when in atr or pps status,directly put the data into rx buffer
        else
        {
            s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos] = temp;
            s_sim_rx_buf[card_id].cur_pos++;

            if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
            {
                s_sim_rx_buf[card_id].cur_pos = 0;
            }
        }
    }

    //if there are new rx data received,check the rx data
    if ( (s_sim_rx_buf[card_id].cur_pos > s_sim_rx_buf[card_id].check_pos)
            || (s_sim_rx_block[card_id].have_edc == TRUE))
    {

        if (s_sim_rx_block[card_id].have_edc)
        {
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxFifoFullIntP_T1]Ext_count[0x%x],Ext_byte[0x%x,0x%x,0x%x,0x%x,0x%x],EDC_Flag[0x%x],cur_pos[0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4067_112_2_17_23_6_38_1211,(uint8*)"ddddddddd",TRACE_CARD_ID, s_sim_rx_block[card_id].cur_ext_byte_count, s_sim_rx_block[card_id].ext_byte[0], s_sim_rx_block[card_id].ext_byte[1], s_sim_rx_block[card_id].ext_byte[2], s_sim_rx_block[card_id].ext_byte[3], s_sim_rx_block[card_id].ext_byte[4], s_sim_rx_block[card_id].have_edc, s_sim_rx_block_buf[card_id].cur_pos);
        }

        if (CheckBlockValid (card_id,&s_sim_tx_block,&s_sim_rx_block[card_id]))
        {
            CheckReceiveBuf(card_id);
        }
    }

    //enable the rx full interrupter
    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
}

/**********************************************************************
//    Description:
//      The card is current in,when the power is on,if there is a sim
//    card exist,the ARM SIM will send a interupt to tell the SIM layer
//    that the SIM card is in,tell other layer that card is in
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardInIntP (int32 card_id)
{
    NotifyExceptionError (card_id, TB_SIMEXP_CARD_IN);
}


/**********************************************************************
//    Description:
//      the Card is beening taking out when working or there are no card
//    when Power is set on the SIM,there comes a interrupt from the ARM.
//    this is the interrupt handler funtion,that to tell the Manager
//    layer the card is out
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardOutIntP (int32 card_id)
{
    NotifyExceptionError (card_id, TB_SIMEXP_CARD_OUT);
    //reject the SIM card by power the SIM Card
    RejectSIMCard(card_id);
}
/* -interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      the Card is beening taking out when working or there are no card
//    when Power is set on the SIM,there comes a interrupt from the ARM.
//    this is the interrupt handler funtion,that to tell the Manager
//    layer the card is out
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void TxEmptyIntP (int32 card_id)
{
        TB_SIM_DATA_BUF_T sim_data;


        s_sim_register_map[card_id]->ie &= ~SIMIE_B_TX_EMPTY;
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }

        USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);

        SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);

        sim_data.card_id = card_id;
        sim_data.data_len = 0;
        sim_data.int_type = SIMSTS0_B_TX_FIFO_EMPTY;
        SimIsrWriteBuffer(sim_data);

}
/* +interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      the Card is beening taking out when working or there are no card
//    when Power is set on the SIM,there comes a interrupt from the ARM.
//    this is the interrupt handler funtion,that to tell the Manager
//    layer the card is out
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void RxFifoFullIntP(int32 card_id)
{
        uint8 rx_fifo_count = 0;
        uint32 i = 0;
        TB_SIM_DATA_BUF_T sim_data;

       // s_sim_register_map->ie &= ~SIMIE_B_RX_FULL;
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }

        sim_data.card_id = card_id;
        sim_data.int_type = SIMSTS0_B_RX_FIFO_FULL;
        sim_data.data_len = 0;
        rx_fifo_count = (uint8) GET_RX_FIFO_COUNT (s_sim_register_map[card_id]);
        /*  make sure that we can only support 16 byte(by hardware)
            if we increase the fifo count to 256, add the buf size
            of fifo buf(fifo_buf[16])*/
        SCI_PASSERT (!(s_sim_register_map[card_id]->sts0 & BIT_11), ("rx fifo over-flow!"));   /*assert verified*//*CR241522 E*/
        SCI_PASSERT (rx_fifo_count < 16, ("cur max fifo count < 16"));/*assert verified*/ /*CR241522 E*/

        for (i = 0; i < rx_fifo_count; i++)
        {
            sim_data.data[sim_data.data_len++] = (uint8) s_sim_register_map[card_id]->rx;
/*+NEWMS00106599 E*/
            if (0 == sim_data.data[i])
                s_sim_junk_data_num[card_id]++;
            else
                s_sim_junk_data_num[card_id] = 0;
        }
        if (s_sim_max_junk_data_num[card_id] < s_sim_junk_data_num[card_id]){
            /*SIM is pulled-out*/
            s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_FULL;
            s_sim_junk_data_num[card_id] = 0;
            s_is_sim_out[card_id] = TRUE;}
/*-NEWMS00106599 E*/

        SimIsrWriteBuffer(sim_data);
}
/* -interrupt_refactor_dev */
/**********************************************************************
//    Description:
//      when the SIM card start to ATR,the ARM will send a Early ATR
//    interrupt,this is the interrupt handler function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void EarlyATRIntP (int32 card_id)
{
    if ( (s_sim_register_map[card_id]->ctl0 & SIMCTL0_B_CARD_OUT_LATCHED) != 0)
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[EarlyATRIntP] card OUT in early ATR interrup"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4200_112_2_17_23_6_38_1212,(uint8*)"d", TRACE_CARD_ID);
    }

    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_TX_EN;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[EarlyATRIntP] Early ATR Interrupt!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4205_112_2_17_23_6_38_1213,(uint8*)"d", TRACE_CARD_ID);
}


/**********************************************************************
//    Description:
//      When the SIM has beening Actived the ARM CPU will send a Active
//    done interrupt,this is the interrupt function.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ActiveDoneIntP (int32 card_id)
{

}


/**********************************************************************
//    Description:
//      if there are Parity Error when receive data from sim card,the
//    ARM will send a RX Parity Error interrupt,this is the interrupt
//    handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RxParityErrorIntP (int32 card_id)
{
    //    uint16 rx_fifo_count;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[RxParityErrorIntP] sim_status[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4234_112_2_17_23_6_38_1214,(uint8*)"dd", TRACE_CARD_ID, s_sim_status[card_id]);

    if (s_sim_status[card_id] != TB_SIM_RECEIVING_RESPONSE)
    {
        /* +CR255705 */
        if (TB_SIM_PROCESSING_PPS == s_sim_status[card_id])
        {
            //SCI_TRACE_LOW:"USIMDRV:: RX parity error happens in PPS procedure"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4241_112_2_17_23_6_38_1215,(uint8*)"");
            s_pps_info[card_id].pps_type = TB_SIM_PPS_DEFAULT_SPEED;
        }
        /* -CR255705 */
#if 0

        // 目前没有进入传输数据阶段
        //the time reset times exeed the max reset time
        if (s_sim_reset_times >= TB_SIM_MAX_RESET_TIMES)
        {
            ReportCardResetStatus (TB_SIM_RESETTED_ERROR);
        }
        //set the time to reset the sim card
        else
        {
            //disable the parity error interrupt and set the she register map to zero
            s_sim_register_map->ie &= ~SIMIE_B_RX_PARITY_ERR;
            s_sim_register_map->ie &= ~SIMIE_B_RX_FULL;
            s_sim_register_map->she = 0;

            /*  bo.chen 20070618 stop atr timer */
            if (SCI_IsTimerActive (s_sim_atr_timer))
            {
                SCI_DeactiveTimer (s_sim_atr_timer);
            }

            SCI_DeactiveTimer (s_sim_reset_p_timer);
            USIMDRV_CHANGE_TIMER(card_id,s_sim_reset_p_timer,DriverTimerExpired,SIM_RESET_TIMER_VALUE);
            SCI_ActiveTimer (s_sim_reset_p_timer);
        }

#endif
    }
    else
    {
        /*  bo.chen 20070616    if rx parity error, we need to send R-Block for T=1 */
        if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
        {
            // 如果已经进入传输阶段了，就只能poweroff，否则还能reset
            //reject the SIM card by power the SIM Card
            /* +cr163006 */
            //SIM_PowerOffSIM();
            /* -cr163006 */
            //disable the RX parity error interrupt to make sure RX parity error interrupt
            //will not come continuously
            s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_PARITY_ERR;
            NotifyExceptionError (card_id,TB_SIMEXP_RX_PARITY_ERROR);
        }
        else
        {
            s_rx_parity_recv[card_id] = TRUE;
        }
    }
}


/**********************************************************************
//    Description:
//      if there are Parity Error when transmit data to sim card,the
//    ARM will send a TX Parity Error interrupt,this is the interrupt
//    handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void TxParityErrorIntP (int32 card_id)
{
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[TxParityErrorIntP] WARNING"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4307_112_2_17_23_6_38_1216,(uint8*)"d", TRACE_CARD_ID);

    if (s_sim_status[card_id] != TB_SIM_RECEIVING_RESPONSE)
    {
        //change the odd parity check setting in the chip,then reset the SIM card
        if (!ResetSIMCard (card_id, TB_SIM_RESET_FOR_PARITY))
        {
            ReportCardResetStatus (card_id, TB_SIM_RESETTED_ERROR);
        }
    }
    else
    {
        /*  bo.chen 20070616    if rx parity error, we need to send R-Block for T=1 */
        if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
        {

            //disable the TX parity error interrupt to make sure TX parity error interrupt
            //will not come continuously
            s_sim_register_map[card_id]->ie &= ~SIMIE_B_TX_PARITY_ERR;

            NotifyExceptionError (card_id, TB_SIMEXP_TX_PARITY_ERROR);
            //reject the SIM card by power the SIM Card
            /* +cr204609 */
            //            SIM_PowerOffSIM(); /*CR178132 Eddie.Wang inactivated*/
            /* -cr204609 */
        }
        else
        {
            /*  we need reset r req for the error. timer or we check
                the edc ??  */
            ErrorHandler (card_id, T1_ERROR_CODE_NAD_ERROR);
        }
    }
}


/**********************************************************************
//    Description:
//      if the SIM card do not response for sometime,the ARM CPU will
//    send a CARD UNRESPONSE interrtupt,this is the interrupt handle
//    function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardUnResponseIntP (int32 card_id)
{
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CardUnResponseIntP] Tick[0x%x],Rx_Count[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4353_112_2_17_23_6_39_1217,(uint8*)"ddd", TRACE_CARD_ID,SCI_GetTickCount(), GET_RX_FIFO_COUNT (s_sim_register_map[card_id]));
/* +interrupt_refactor_dev */
    RxFifoFullIntP(card_id);
/* -interrupt_refactor_dev */
    if (s_sim_status[card_id] == TB_SIM_RECEIVING_RESPONSE)
    {
#if 0
        //notify the SIM layer that the SIM card is unresponse
        NotifyExceptionError (TB_SIMEXP_CARD_UNRESPONSE);
#endif
    }
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
static void SendResponseUp (int32 card_id)
{
    TB_MSG msg;
    TB_SIM_DATA_RESPONSE_T data_resp;

    msg.message = SIM_DATA_RESPONSE;
    msg.wparam  = 0;
    msg.lparam  = &data_resp;
    data_resp.data_len = 0;
    data_resp.card_id = card_id;

    if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
    }

    //if SW1 = 0x61 or 0x6c
    if (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2] == TB_SIM_RESPONSE_GET_RESP_BYTE)
    {
        // 61 -4 p 30,表示还有数据需要接收,其中sw2就是确切的字数
        SendGetRespCmd (card_id, s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1]);
        s_sim_rx_buf[card_id].cur_pos -= 2;
        return;
    }
    else if (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2] == TB_SIM_RESONSE_REP_CMD_BYTE)
    {
        // 6c表示p3长度错误 html -3 p 23 pdf -3 p 30
        // bo.chen 20050331
        // ClearRxBuf();
        RedoPreInstr (card_id, TRUE,s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1]);
        return;
    }
    else if (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2] == TB_SIM_WARNNING_BYTE)
    {
        SendGetRespCmd (card_id,0);
        s_sim_rx_buf[card_id].cur_pos = 0;
        return;
    }
    else if (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2] == 0x9f)
    {
        // 61 -4 p 30,表示还有数据需要接收,其中sw2就是确切的字数
        SendGetRespCmd (card_id, s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1]);
        s_sim_rx_buf[card_id].cur_pos -= 2;
        return;
    }

    //the response is a instruction response,ref to ISO 7816 part-3
    //"procedure bytes sent by the card"
    if (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id])
    {
        //there are only two byte response data from the SIM card,
        //the data is start with the SW1 and the seconde byte is the
        //SW2
        if (2 == s_sim_rx_buf[card_id].cur_pos)
        {
            //the staus word of the response of instruciton
            data_resp.sw1 = s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2];
            data_resp.sw2 = s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1];
        }
        //there are more that 2 byte response data from the SIM card
        else if (2 < s_sim_rx_buf[card_id].cur_pos)
        {


            //delete the not really response data from the receive buf
            data_resp.data_len = s_sim_rx_buf[card_id].cur_pos - 2;

            if (data_resp.data_len > 0)
            {
                // bo.chen 20051221 remove redundant 0x60
                // need verify this change
                {
                    uint8 buf_index = 0;

                    if (s_sim_rx_buf[card_id].cur_pos > s_sim_rx_buf[card_id].rx_expected_len)
                    {
                        while (0x60 == s_sim_rx_buf[card_id].buf[buf_index])
                        {
                            if (s_sim_rx_buf[card_id].cur_pos <= (buf_index + s_sim_rx_buf[card_id].rx_expected_len))
                            {
                                break;
                            }

                            /* +cr156002 */
                            ++buf_index;
                            data_resp.data_len--;
                            //SCI_TRACE_LOW:"USIMDRV:[%d]:[SendResponseUp] NULL byte data!!!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4457_112_2_17_23_6_39_1218,(uint8*)"d",TRACE_CARD_ID);
                            /* -cr156002 */
                        }

                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[SendResponseUp] rev_data_len > exp_data_len"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4461_112_2_17_23_6_39_1219,(uint8*)"d",TRACE_CARD_ID);
                    }

                    data_resp.data = &s_sim_rx_buf[card_id].buf[buf_index];
                }
                // bo.chen 20051221 remove redundant 0x60
            }
            else
            {
                data_resp.data = NULL;
            }

            //the staus word of the response of instruciton
            data_resp.sw1 = s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2];
            data_resp.sw2 = s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1];
        }
        else
        {
            USIM_ASSERT (FALSE);/*assert verified*/
        }
    }

    //the response data is a ATR data or PPS data
    else
    {
        data_resp.data_len = s_sim_rx_buf[card_id].cur_pos;
        data_resp.data = s_sim_rx_buf[card_id].buf;
    }

    DriverCallBack (card_id, &msg);
    s_sim_is_processing_cmd[card_id] = FALSE;


    /* +cr204609 */
    if (s_auth_inited[card_id] || g_set_usim_drv_log_ctl) /*CR173855 Eddie.Wang*/
        /* -cr204609 */
    {
        uint32 i = 0;
        s_auth_inited[card_id] = FALSE;
        //SCI_TRACE_LOW:"USIMDRV:[%d]:resp_data_len[%d],sw [0x%02x, 0x%02x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4501_112_2_17_23_6_39_1220,(uint8*)"dddd", TRACE_CARD_ID,data_resp.data_len, data_resp.sw1, data_resp.sw2);

        while (data_resp.data_len > i*16)
        {
            //SCI_TRACE_LOW:"USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4509_112_2_17_23_6_39_1221,(uint8*)"ddddddddddddddddd",TRACE_CARD_ID,data_resp.data[i*16 + 0],data_resp.data[i*16 + 1],data_resp.data[i*16 + 2],data_resp.data[i*16 + 3],data_resp.data[i*16 + 4],data_resp.data[i*16 + 5],data_resp.data[i*16 + 6],data_resp.data[i*16 + 7],data_resp.data[i*16 + 8],data_resp.data[i*16 + 9],data_resp.data[i*16 + 10],data_resp.data[i*16 + 11],data_resp.data[i*16 + 12],data_resp.data[i*16 + 13],data_resp.data[i*16 + 14],data_resp.data[i*16 + 15]);
            i++;

            /*  if tx_data_len > 255 break; */
            if (16 <= i)
            {
                break;
            }
        }
    }

    /* +cr204609 */
    if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
    {
        s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_PARITY_ERR;
    }

    /* -cr204609 */
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SendResponseUp] [0x%x, 0x%x, 0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4528_112_2_17_23_6_39_1222,(uint8*)"dddd", TRACE_CARD_ID,s_sim_rxfifo_count[card_id], s_sim_rxfifo_int_count[card_id], s_sim_rx_buf[card_id].rx_expected_len);

    ClearRxBuf(card_id);
    ClearTxBlock(card_id);

    if (SCI_IsTimerActive (s_sim_clk_stop_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_clk_stop_timer[card_id]);
    }
    USIMDRV_CHANGE_TIMER(card_id,s_sim_clk_stop_timer[card_id],SetClkStopMode,10);

    SCI_ActiveTimer (s_sim_clk_stop_timer[card_id]);

    //disable the retransmit fucntion,the rx parity error handler by software.
    s_sim_register_map[card_id]->she = SIM_DEFAULT_SHE;
#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }
    }
    else
    {
        /*  20070730    */
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }
    }

#else

    /*  20070730    */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

#endif

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
static void SendPPSRequest (
    int32 card_id,
    const uint8 *pps_req, //in:the ppas request data to be send
    uint16 req_len  //the data len
)
{
    ClearTxBuf(card_id);

    USIM_ASSERT (NULL != pps_req);/*assert verified*/
    USIM_ASSERT ( (TB_SIM_PPS_DEFAULT_SPEED == req_len) || /*assert verified*/
                  (TB_SIM_PPS_ENHANCED_SPEED == req_len));

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SendPPSRequest] DO PPS Request"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4594_112_2_17_23_6_39_1223,(uint8*)"d", TRACE_CARD_ID);
#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        USIMDRV_DMA_TX_Disable(card_id);
        s_sim_tx_buf[card_id].tx_data_len = req_len;
        memcpy (s_sim_tx_buf[card_id].buf,pps_req,req_len);
        s_sim_rx_buf[card_id].is_locked = FALSE;
        USIM_DMA_TX_Callback (card_id, 0);
    }
    else
    {
        s_sim_tx_buf[card_id].tx_data_len = req_len;
        memcpy (s_sim_tx_buf[card_id].buf,pps_req,req_len);

        //open the lock of the recv data buf.
        s_sim_rx_buf[card_id].is_locked = FALSE;

        TxEmptyHisrP(card_id); /* interrupt_refactor_dev */
    }

#else
    s_sim_tx_buf[card_id].tx_data_len = req_len;
    memcpy (s_sim_tx_buf[card_id].buf,pps_req,req_len);

    //open the lock of the recv data buf.
    s_sim_rx_buf[card_id].is_locked = FALSE;

    //transmit the data for the first time
    TxEmptyHisrP(card_id); /* interrupt_refactor_dev */
#endif
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
static BOOLEAN ResetSIMCard (   //return value:the SIM card reset succeed or not.
    int32 card_id,
    TB_SIM_RESET_TYPE_E reset_type  //the reset type
)
{
    uint32 temp,i;
    uint16 rx_fifo_count;
    TB_SIM_WORKING_VOLT_E next_volt;
    LDO_VOLT_LEVEL_E ldo_lvl = LDO_VOLT_LEVEL0;
    //是否正在reset
    SCI_DISABLE_IRQ;
    if (s_card_resetting[card_id] || s_card_warm_resetting[card_id])
    {
        SCI_RESTORE_IRQ;
        return TRUE;
    }
    s_card_resetting[card_id] = TRUE;
    SCI_RESTORE_IRQ;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard] Reset_type[%d]!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4655_112_2_17_23_6_39_1224,(uint8*)"dd", TRACE_CARD_ID,reset_type);
    //deactive these timer to avoid the timer expiration cause SIM card Reset nest
    SCI_DeactiveTimer (s_sim_reset_p_timer[card_id]);
    SCI_DeactiveTimer (s_sim_reset_bt_timer[card_id]);
    SCI_DeactiveTimer (s_sim_atr_timer[card_id]);

    USIMDRV_CHANGE_TIMER(card_id,s_sim_atr_timer[card_id],DriverTimerExpired,SIM_ATR_TIMER_VALUE);

    //clear the rx fifo if needed
    rx_fifo_count = (uint16) GET_RX_FIFO_COUNT (s_sim_register_map[card_id]);

    for (i = 0; i < rx_fifo_count; i++)
    {
        temp = s_sim_register_map[card_id]->rx;
    }

    //clear previous interrupt status
    s_sim_register_map[card_id]->iclr = 0xffff;
    s_sim_register_map[card_id]->she = 0;

    /* +binli opt sim */
    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id], 1);
    /* -binli opt sim */

    RejectSIMCard(card_id);

    s_sim_register_map[card_id]->ie &= 0;


    for (i = 0; i< 1000; i++)
    {
    }


    //the rx tx buf initialize
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);
    ClearATRData(card_id);

    // by bo.chen for   s_tck_will_be_sent
    s_tck_will_be_sent[card_id] = FALSE;

    //initialize global variable
    s_sim_rx_buf[card_id].is_locked = FALSE;
    s_have_ack_byte[card_id] = FALSE;
    s_sim_status[card_id] = TB_SIM_WAITING_FOR_ATR;

    //do the different operation between differecnt type restet operation
    // 同时限制同一种reset类型不停的重启
    switch (reset_type)
    {
        case TB_SIM_RESET_NO_CHANGE:
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard] Reset the first time!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4707_112_2_17_23_6_39_1225,(uint8*)"d",TRACE_CARD_ID);

            //check reset times
            if (s_sim_reset_times[card_id] > TB_SIM_MAX_RESET_TIMES)
            {
                RejectSIMCard(card_id);
                return FALSE;
            }

            s_sim_reset_times[card_id]++;
            break;
        case TB_SIM_RESET_FOR_POWER:

            s_sim_pow_reset_times[card_id]++;

            //check reset times2
            if (s_sim_pow_reset_times[card_id] >= TB_SIM_MAX_POWER_RESET_TIMES)
            {
                s_sim_pow_reset_times[card_id] = 0;
            }

            if (s_sim_pow_reset_times[card_id] == 0)
            {

#ifdef SIM_SUPPORT_1D8V_3V

                //如果目前是1.8v 并且因为电压不对，那么就跳至3v,否则就不在变化电压
                if (GetNextSupplyVoltage (card_id, s_sim_working_volt[card_id],&next_volt))
                {
                    /*  bo.chen end 20060406 make sure that power is enable */
                    SetSIMPower (card_id, next_volt);
                    s_sim_working_volt[card_id] = next_volt;
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard] Reset for change_power!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4739_112_2_17_23_6_39_1226,(uint8*)"d",TRACE_CARD_ID);
                }
                else
                {
#endif
                    /*+CR161638 Eddie.Wamg*/
                    if (!sim_is_in_process_reset(card_id))
                    {
                        RejectSIMCard(card_id);
                    }

                    /*-CR161638 Eddie.Wamg*/
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard] Reject for NO next available voltage any more!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4751_112_2_17_23_6_39_1227,(uint8*)"d",TRACE_CARD_ID);
                    return FALSE;
#ifdef SIM_SUPPORT_1D8V_3V
                }

#endif
            }
            else
            {
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard] Reset for no_change!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4760_112_2_17_23_6_39_1228,(uint8*)"d",TRACE_CARD_ID);
            }
            if (USIMDRV_CARD_ONE == card_id)
            {
                ldo_lvl = LDO_GetVoltLevel (LDO_LDO_SIM0);
            }
            else
            {
                ldo_lvl = LDO_GetVoltLevel (LDO_LDO_SIM1);
            }
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard]  Reset for next_power LDO_Level[%d]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4770_112_2_17_23_6_39_1229,(uint8*)"dd",TRACE_CARD_ID, ldo_lvl);
            /*  bo.chen 20060407 wait for 200ms */
            SCI_DISABLE_IRQ;

            USIMDRV_CHANGE_TIMER(card_id,s_sim_reset_wait_ldo_timer[card_id],ResetAfterLdoValid,SIM_WAIT_LDO_TIMER_VALUE);

            SCI_ActiveTimer (s_sim_reset_wait_ldo_timer[card_id]);
            SCI_RESTORE_IRQ;

            return TRUE;


        case TB_SIM_RESET_FOR_BT:
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard] Reset for bit_convention,bit_convent[%d]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4783_112_2_17_23_6_39_1230,(uint8*)"dd",TRACE_CARD_ID, s_sim_bit_converion_value[card_id]);

            //check reset times
            if (s_sim_b_reset_times[card_id] > TB_SIM_MAX_B_RESET_TIMES)
            {
                RejectSIMCard(card_id);
                return FALSE;
            }

            s_sim_b_reset_times[card_id]++;

            //change the bit convetion and the logic level
            // 变换第一位和第二位
            /*  bo.chen only one bit may be change  */
            /*  s_sim_register_map[card_id]->ctl0 ^= (SIMCTL0_B_BIT_CONVENTION | SIMCTL0_B_LOGIC_LEVEL); */
            s_sim_register_map[card_id]->ctl0 ^= s_sim_bit_converion_value[card_id];
            /*  bo.chen 20060407 wait for 200ms */
            SCI_DISABLE_IRQ;
            USIMDRV_CHANGE_TIMER(card_id,s_sim_reset_wait_ldo_timer[card_id],ResetAfterLdoValid,SIM_WAIT_LDO_TIMER_VALUE);

            SCI_ActiveTimer (s_sim_reset_wait_ldo_timer[card_id]);
            SCI_RESTORE_IRQ;

            return TRUE;

        case TB_SIM_RESET_FOR_PARITY:
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ResetSIMCard] Reset for change_parity!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_4809_112_2_17_23_6_39_1231,(uint8*)"d",TRACE_CARD_ID);

            if (s_sim_p_reset_times[card_id] > TB_SIM_MAX_B_RESET_TIMES)
            {
                RejectSIMCard(card_id);
                return FALSE;
            }

            s_sim_p_reset_times[card_id]++;

            //change the parity checking setting
            // 是不是应该 s_sim_register_map[card_id]->ctl0 ^= SIMCTL0_B_EVEN_PARITY ??
            s_sim_register_map[card_id]->ctl0 ^= SIMCTL0_B_EVEN_PARITY;
            break;
        default:
            USIM_ASSERT (FALSE);/*assert verified*/
            return FALSE;//lint !e527 confirmed by windy.wang
    }


    // 打开中断，开始启动s_sim_atr_timer[card_id]
    s_sim_register_map[card_id]->ie = SIM_DEFAULT_IE;

#ifdef PLATFORM_SC8800G
#else
    /* set  4.7k pull-up for SIM DATA pads */
    CHIP_REG_OR (PIN_SIMDA0_REG, PIN_PU_EN);
    REG32 (GR_PCTL) |= BIT_11;

    for (i = 0; i < 100; i++)
    {
    }

#endif
    // vcc enable
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_POWER_EN;
    /*+CR200528 Eddie.Wang*/
#ifdef PLATFORM_SC8800G
    if (USIMDRV_CARD_ONE == card_id)
    {
        LDO_TurnOnLDO (LDO_LDO_SIM0);
    }
    else
    {
        LDO_TurnOnLDO (LDO_LDO_SIM1);
    }
#endif
    /*-CR200528 Eddie.Wang*/
    SCI_DISABLE_IRQ;

    if (SCI_IsTimerActive (s_sim_atr_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_atr_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_atr_timer[card_id],DriverTimerExpired,SIM_ATR_TIMER_VALUE);
    SCI_ActiveTimer (s_sim_atr_timer[card_id]);


#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }

        USIMDRV_CHANGE_TIMER(card_id,s_sim_dma_byte_timer[card_id], DriverTimerExpired, SIM_DMA_TIMER_VALUE);
        SCI_ActiveTimer (s_sim_dma_byte_timer[card_id]);
        SCI_RESTORE_IRQ;
    }

    else
    {
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }

        USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);
        SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);

        SCI_RESTORE_IRQ;
    }

#else

    /*  20070730    */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);
    SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);

    SCI_RESTORE_IRQ;
#endif

    // i/o 设置成 A?
    for (i = 0; i < 100; i++)
    {
    }

    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_TX_DATA_OUT_LOW;

    for (i = 0; i < 100; i++)
    {
    }

    //enactive the SIM card
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_EN_ACTIVE;

#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        /* disable rx,rx ,tx parity int */
        USIMDRV_DisableSIMRXInt(card_id);
        USIMDRV_DisableSIM_TxParityInt(card_id);
        USIMDRV_DisableSIMTXEmptyInt(card_id);
        s_sim_register_map[card_id]->ctl0 |= (SIMCTL0_B_TX_EN|SIMCTL0_B_RX_EN);
        USIMDRV_DMA_TX_Disable(card_id);
        USIMDRV_DMA_RX_Disable(card_id);
        sim_rx_dma_cfg.total_length = 1;
        sim_rx_dma_cfg.block_length = 1;
        USIMDRV_DMA_RX_Cfg (&sim_rx_dma_cfg[card_id]);
        USIMDRV_DMA_RX_Enable(card_id);
    }
    else
    {
        //enable the rx to receive the tx data
        s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
    }

#else
    /* -binli add dma test */
    //enable the rx to receive the tx data
    if (!s_is_sim_out[card_id])  /*NEWMS00106599 E*/
        s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
#endif

    /* if cold reset simcard ,not need enable deepsleep before atr received */
    SCI_SIM_EnableDeepSleep (0);

    s_card_resetting[card_id] = FALSE;

    return TRUE;
}

/**********************************************************************
//    Description:
//      Make the SIM card to the Idle state,stop the clock of the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void IdleSIMCard (int32 card_id)
{
    //check if the clk has been stoped
    if (! (s_sim_register_map[card_id]->ctl1 & SIMCTL1_B_CLK_ENABLE))
    {
        return;
    }

    switch (s_atr_data_buf[card_id].atr_data.X)
    {
        case 0x01:
            SetClockStopLevel (card_id,FALSE);   //State L
            break;
        case 0x02:
            SetClockStopLevel (card_id,TRUE);    //State H
            break;
        case 0x03:
            SetClockStopLevel (card_id,FALSE);   //no preference,select State L
            break;
        default:
            if(SIM_CLOCK_STOP_LOW == s_sim_clk_stop_mode_from_mf[card_id])
            {
                SetClockStopLevel (card_id,FALSE);   //State L
            }
            else if(SIM_CLOCK_STOP_HIGH == s_sim_clk_stop_mode_from_mf[card_id])
            {
                SetClockStopLevel (card_id, TRUE);    //State H
            }
            else if(SIM_CLOCK_STOP_ANY == s_sim_clk_stop_mode_from_mf[card_id])
            {
                SetClockStopLevel (card_id, FALSE);   //no preference,select State L
            }
            else
            {
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[IdleSIMCard] ATR Default ,MF == CLOCK_STOP_NOT_SUPPORT ,the UICC NOT support clock stop mode"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5000_112_2_17_23_6_40_1232,(uint8*)"d",TRACE_CARD_ID);
                return;
            }

            break;
            /* -cr206965 */
    }

    //disable the clock
    s_sim_register_map[card_id]->ctl1 &= ~SIMCTL1_B_CLK_ENABLE;

    return;
}


/**********************************************************************
//    Description:
//      Active the SIM card by start clock
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ActiveSIMCard (int32 card_id)
{
    uint32 i = 0;
    uint32 k = 0;   /*CR208180 Eddie.Wang added time-delay coefficient*/
    //enable the clock
    s_sim_register_map[card_id]->ctl1 |= SIMCTL1_B_CLK_ENABLE;
/*+CR208180 Eddie.Wang modified to 15k for SC8800G2 for case 6.2/6.3*/
#ifdef PLATFORM_SC8800G
    k = 25; //SC8800G2 = 25; for SC8800G1 12 is fine
#else
    k= 4;  //SC8800H
#endif

    for (i = 0; i< 1000*k; i++)
    {
    }

    /*-CR208180 Eddie.Wang*/
}

/**********************************************************************
//    Description:
//      Send a byte data of TX buf to the SIM card,not the all left TX
//    buf data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN SendOneByteToSIM (int32 card_id)  //return value:TRUE:all the TX have been transmit to SIM
//FALSE:there still exist TX data need to be transmit to the SIM
{
    //there still some instruction data to be sent
    // 是否所有的数据都已经发送完，如果true表示都发完了
    if (s_sim_tx_buf[card_id].tx_data_len > s_sim_tx_buf[card_id].cur_pos)
    {
        s_sim_register_map[card_id]->tx = s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos];
        s_sim_tx_buf[card_id].cur_pos++;

        //all the data of the tx buf has been sent to the SIM card.
        if (s_sim_tx_buf[card_id].cur_pos >= s_sim_tx_buf[card_id].tx_data_len)
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
//      to identify a two byte sequence data can be status word pair or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN IsStatusWord (
    uint8 byte1,    //the byte1 of the data
    uint8 byte2     //the byte 2 of the data
)
{
    uint8 i,array_num;

    array_num = sizeof (status_word) / (sizeof (uint8) *2);

    for (i = 0; i < array_num; i++)
    {
        if (byte1 == status_word[i][0])
        {
            if (byte2 == status_word[i][1])   //sw1 and sw2 are all status word
            {
                return TRUE;
            }
            else if (status_word[i][1] == 0xff)  //sw1 is equal,sw2= XX
            {
                return TRUE;
            }
        }
    }

    if ( (0x63 == byte1) && ( (byte2 & 0xf0) == 0xC0)) //0x63CX
    {
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************
//    Description:
//      Set the logic level in the clock pin when the clock is disable
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SetClockStopLevel (
    int32 card_id,
    BOOLEAN bHighLevel  //is the logic level high
)
{
    if (bHighLevel)
    {
        s_sim_register_map[card_id]->ctl1 |= SIMCTL1_B_CLK_POLARITY;
    }
    else
    {
        s_sim_register_map[card_id]->ctl1 &= ~SIMCTL1_B_CLK_POLARITY;
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
static void CheckReceiveBuf (int32 card_id)
{
    // 注意这个函数并不是把所有的响应数据都读入到RX_BUF中后才会调用，而是有个中断就会被调用
    uint16  i;
    BOOLEAN change_is_need;
    BOOLEAN change_is_support;

    s_sim_wwt_active[card_id] = FALSE;

    switch (s_sim_status[card_id])
    {
            //after the power is on reiceve the start of the ATR character
        case TB_SIM_WAITING_FOR_ATR:

            // 如果是这个状态，那么等待的数据就是ATR了
            // 格式 ts t0 ta1 tb1 tc1 td1 ... t1 t2.. tk tck

            SCI_DeactiveTimer (s_sim_atr_timer[card_id]);

            //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] ATR[0x%x], cur_pos[0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5157_112_2_17_23_6_40_1233,(uint8*)"ddd", TRACE_CARD_ID,s_sim_rx_buf[card_id].buf[0], s_sim_rx_buf[card_id].cur_pos);

            //the TS character must be a valide TS value
            if ( (TB_SIM_ATR_TS_VALUE1 == s_sim_rx_buf[card_id].buf[0]) ||
                    (TB_SIM_ATR_TS_VALUE2 == s_sim_rx_buf[card_id].buf[0]))
            {
                // 3f 3b是TS的值，参看7816-3 p10
                // 至少有TS 和 T0，以后的长度在调用DecodeATRData的时候累加
                s_sim_rx_buf[card_id].rx_expected_len = TB_SIM_ATR_ALWAYS_CHARACTER_NUM;

                // 接下来的状态就是TB_SIM_PROCESSING_ATR，通过接收剩余的ATR response
                s_sim_status[card_id] = TB_SIM_PROCESSING_ATR;
                // check_pos++现在应该等于1了，因为已经接收完TS了，现在指向T0
                s_sim_rx_buf[card_id].check_pos++;
                goto PROCESS_ATR_CHARACTOR;
            }
            else if ( (TB_SIM_ATR_TS_VALUE3 == s_sim_rx_buf[card_id].buf[0]) ||
                      (TB_SIM_ATR_TS_VALUE4 == s_sim_rx_buf[card_id].buf[0]))
            {
                //use timer to reset for change the bit convention
                // 如果是fc dc就正好对应3f 3b，bit位对调一下正好
                /* bo.chen fc&dc->  SIMCTL0_B_LOGIC_LEVEL*/
                s_sim_bit_converion_value[card_id] = SIMCTL0_B_LOGIC_LEVEL;
                SCI_DeactiveTimer (s_sim_reset_bt_timer[card_id]);
                USIMDRV_CHANGE_TIMER(card_id,s_sim_reset_bt_timer[card_id],DriverTimerExpired,SIM_RESET_TIMER_VALUE);
                SCI_ActiveTimer (s_sim_reset_bt_timer[card_id]);
            }/* 0x03 == 0000 0011 ->1111 1100==fc ->0011 1111 -> 0x3f

            0x3b== 0011 1011 -> 1101 1100 == dc -> 0010 0011 -> 0x23*/
            else if ( (TB_SIM_ATR_TS_VALUE5 == s_sim_rx_buf[card_id].buf[0]) ||
                      (TB_SIM_ATR_TS_VALUE6 == s_sim_rx_buf[card_id].buf[0]))
            {
                //use timer to reset for change the bit convention
                //bo.chen 0x03&0x23 --> 0x3f&0x3b (SIMCTL0_B_BIT_CONVENTION | SIMCTL0_B_LOGIC_LEVEL)
                s_sim_bit_converion_value[card_id] = SIMCTL0_B_BIT_CONVENTION | SIMCTL0_B_LOGIC_LEVEL;
                SCI_DeactiveTimer (s_sim_reset_bt_timer[card_id]);
                USIMDRV_CHANGE_TIMER(card_id,s_sim_reset_bt_timer[card_id],DriverTimerExpired,SIM_RESET_TIMER_VALUE);
                SCI_ActiveTimer (s_sim_reset_bt_timer[card_id]);
            }
            else if ( (TB_SIM_ATR_TS_VALUE7 == s_sim_rx_buf[card_id].buf[0]) ||
                      (TB_SIM_ATR_TS_VALUE8 == s_sim_rx_buf[card_id].buf[0]))
            {
                //use timer to reset for change the logic level
                s_sim_bit_converion_value[card_id] = SIMCTL0_B_BIT_CONVENTION;
                SCI_DeactiveTimer (s_sim_reset_bt_timer[card_id]);
                USIMDRV_CHANGE_TIMER(card_id,s_sim_reset_bt_timer[card_id],DriverTimerExpired,SIM_RESET_TIMER_VALUE);

                SCI_ActiveTimer (s_sim_reset_bt_timer[card_id]);
            }
            else
            {
                // 未知的atr 数据，不是TS,启动atr timer，过时认为atr接收失败
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] corrupted ATR data"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5209_112_2_17_23_6_40_1234,(uint8*)"d",TRACE_CARD_ID);
                //when no atr byte has been received,change the supply power
                //use timer to do the reset operate for the supply power change
                USIMDRV_CHANGE_TIMER(card_id,s_sim_atr_timer[card_id],DriverTimerExpired,SIM_RESET_TIMER_VALUE);
                SCI_ActiveTimer (s_sim_atr_timer[card_id]);
                /* +cr119609 */
                if (!SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] active s_sim_wait_resp_timer!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5217_112_2_17_23_6_40_1235,(uint8*)"d",TRACE_CARD_ID);
                    USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id] , DriverTimerExpired, s_sim_wait_resp_timer_value[card_id]);
                    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
                }
                /* -cr119609 */
            }

            break;
            //receive the ATR procedure response
        case TB_SIM_PROCESSING_ATR:
PROCESS_ATR_CHARACTOR:
            //get the total ATR data len according the received ATR data

            // 得到了新的数据，需要解码ATR
            while (s_sim_rx_buf[card_id].cur_pos > s_sim_rx_buf[card_id].check_pos)
            {
                // 一个一个的解码
                for (i = s_sim_rx_buf[card_id].check_pos; i < s_sim_rx_buf[card_id].cur_pos; i++)
                {
                    DecodeATRData (card_id,s_sim_rx_buf[card_id].check_pos,s_sim_rx_buf[card_id].buf,&s_sim_rx_buf[card_id].rx_expected_len);
                    s_sim_rx_buf[card_id].check_pos++;
                }
            }

            /*  bo.chen just add this function to print all ATR data */
            //  SIM_PrintRxBuf();
            /*  bo.chen 20060407 */

            // 是否所有的ATR都收到了，没有的话不会做任何操作等待新的RXfifofullintp
            //all the ATR data has been received
            if (s_sim_rx_buf[card_id].cur_pos == s_sim_rx_buf[card_id].rx_expected_len)
            {
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] ALL ATR received ,cur_pos[0x%x], exp_len[0x%x]"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5250_112_2_17_23_6_40_1236,(uint8*)"ddd",TRACE_CARD_ID, s_sim_rx_buf[card_id].cur_pos, s_sim_rx_buf[card_id].rx_expected_len);

                /* 20080608 bin.li add */
                /* +cr119609 */
                if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] deactive s_sim_wait_resp_timer!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5256_112_2_17_23_6_40_1237,(uint8*)"d",TRACE_CARD_ID);
                    SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
                }

                /* -cr119609 */
                /*  bo.chen trace all atr data for test */
                USIM_PRINT_RX_BUF(card_id);
				sim_store_atr_raw_data(card_id,s_sim_rx_buf[card_id].rx_expected_len, s_sim_rx_buf[card_id].buf);  /*CR232991 E*/

                /*  set s_status_timer_value    */
                s_wwt_timer_value[card_id] = GetT0WWT (card_id,s_atr_data_buf[card_id].atr_data.CWI);
                s_cwt_timer_value[card_id] = GetT1CWT (card_id,s_atr_data_buf[card_id].atr_data.CWI);

                /*  the N should be 0 or 0xff according to 11.10-1 for gsm */
                if ( (0 != s_atr_data_buf[card_id].atr_data.N) && (0xff != s_atr_data_buf[card_id].atr_data.N))
                {
                    /*  start wait atr timer    */
                    if (SCI_IsTimerActive (s_sim_atr_timer[card_id]))
                    {
                        SCI_DeactiveTimer (s_sim_atr_timer);
                    }
                    USIMDRV_CHANGE_TIMER(card_id,s_sim_atr_timer[card_id],DriverTimerExpired,SIM_ATR_TIMER_VALUE);
                    SCI_ActiveTimer (s_sim_atr_timer[card_id]);

                    return;
                }

                s_sim_pow_reset_times[card_id] = 0;
                ClearRxBuf(card_id);

                //if card indicates supply voltage class
                if (s_atr_data_buf[card_id].is_x_u_present)
                {
                    CheckSupplyVol(s_sim_working_volt[card_id],s_atr_data_buf[card_id].atr_data.U,&change_is_need,&change_is_support);

                    if (change_is_need)
                    {
                        if (change_is_support)
                        {
                            /*  bo.chen 20060404, if the change is get from ATR
                                we should reset sim card with the another power,
                                so we think current power reset time is full */
                            s_sim_pow_reset_times[card_id] = TB_SIM_MAX_POWER_RESET_TIMES;
                            //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf]  Reset for next_voltage ATR_Vol[%d]"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5299_112_2_17_23_6_41_1238,(uint8*)"dd",TRACE_CARD_ID, s_atr_data_buf[card_id].atr_data.U);

                            if (!ResetSIMCard (card_id, TB_SIM_RESET_FOR_POWER))
                            {
                                RejectSIMCard(card_id);
                                ReportCardResetStatus (card_id, TB_SIM_RESETTED_ERROR);
                            }

                            return;
                        }
                        else
                        {
                            RejectSIMCard(card_id);
                            ReportCardResetStatus (card_id, TB_SIM_RESETTED_ERROR);
                        }

                        return;
                    }
                }

                //check the logic level on the reset pin,if the logic level is low,
                //up the logic level
                // bo.chen
                if ( (SIMCTL0_B_SIM_RST & s_sim_register_map[card_id]->ctl0) == 0)
                {
                    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_SIM_RST;
                }

                //lock the recv data buf,to make sure there no more data coming
                s_sim_rx_buf[card_id].is_locked = TRUE;

                //check whether the pps is need
                if (s_card_warm_resetting[card_id])
                {
                    //when in warm reset,the pps procedure is not need
                    s_pps_info[card_id].is_current = FALSE;
                }

#ifdef SIM_ENHANCED_PPS_SUPPORT

                if ( (s_atr_data_buf[card_id].atr_data.F == 512) && (s_atr_data_buf[card_id].atr_data.D == 8))
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 8"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5341_112_2_17_23_6_41_1239,(uint8*)"d",TRACE_CARD_ID);
                    s_pps_info[card_id].is_current = TRUE;
                    s_pps_info[card_id].pps_data = s_enhanced_pps_data1;
                    s_pps_info[card_id].pps_type = TB_SIM_PPS_ENHANCED_SPEED;
                }
                else if ( (s_atr_data_buf[card_id].atr_data.F == 512) && (s_atr_data_buf[card_id].atr_data.D == 16))
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 16"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5348_112_2_17_23_6_41_1240,(uint8*)"d",TRACE_CARD_ID);
                    s_pps_info[card_id].is_current = TRUE;
                    s_pps_info[card_id].pps_data = s_enhanced_pps_data2;
                    s_pps_info[card_id].pps_type = TB_SIM_PPS_ENHANCED_SPEED;
                }
//#ifndef PLATFORM_SC8800G   /*CR206045 E removed support for 8800G*/
/*+CR183039 E*/
                else if ( (s_atr_data_buf[card_id].atr_data.F == 512) && (s_atr_data_buf[card_id].atr_data.D == 32))
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 32"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5356_112_2_17_23_6_41_1241,(uint8*)"d",TRACE_CARD_ID);
                    s_pps_info[card_id].is_current = TRUE;
                    s_pps_info[card_id].pps_data = s_enhanced_pps_data3;
                    s_pps_info[card_id].pps_type = TB_SIM_PPS_ENHANCED_SPEED;
                }
/*-CR183039 E*/
//#endif /*End of PLATFORM_SC8800G*/  /*CR206045 CR248506 E*/
                else
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] use default PPS data 372 1"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5364_112_2_17_23_6_41_1242,(uint8*)"d",TRACE_CARD_ID);
/*+NEWMS00106643 E*/
 //                   s_pps_info[card_id].is_current = g_is_set_sim_ins_only?TRUE:FALSE;  /*CR206004 Eddie.Wang added TRUE for GCF case 27.11.2.4*/
					s_pps_info[card_id].is_current = TRUE;  
/*-NEWMS00106643 E*/
                    s_pps_info[card_id].pps_data = s_default_pps_data;
                    s_pps_info[card_id].pps_type = TB_SIM_PPS_DEFAULT_SPEED;
                }

#endif  //SIM_ENHANCED_PPS_SUPPORT

                // 如果支持enhanced pps procedure，就会发送SendPPSRequest
                if (s_pps_info[card_id].is_current)
                {
                    /*+CR161638 Eddie.Wamg*/
                    if ( (s_previous_pps_error[card_id]) || (!sim_get_is_need_pps(card_id)))
                        /*#else
                         if ((s_previous_pps_error) )
                        #endif*/
                        /*-CR161638 Eddie.Wamg*/
                    {
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] NOT do PPS procedure!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5383_112_2_17_23_6_41_1243,(uint8*)"d", TRACE_CARD_ID);
                        // s_previous_pps_error 表示上次协商失败，因此再次reset就不在发起pps
                        s_pps_info[card_id].is_current = FALSE;
                        s_sim_status[card_id] = TB_SIM_RECEIVING_RESPONSE;
                        ReportCardResetStatus (card_id, TB_SIM_RESETTED_OK);
                    }
                    else
                    {
                        s_sim_status[card_id] = TB_SIM_PROCESSING_PPS;
                        SendPPSRequest (card_id, s_pps_info[card_id].pps_data, (uint16) s_pps_info[card_id].pps_type);

                        /*  for the pps we donot star wwt temp  */
                        if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
                        {
                            SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
                        }
                    }
                }
                else
                {
                    s_sim_status[card_id] = TB_SIM_RECEIVING_RESPONSE;
                    ReportCardResetStatus (card_id, TB_SIM_RESETTED_OK);
                }

                s_card_warm_resetting[card_id] = TRUE;

                 /* CR218847 remove real nw flag delete code  */
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] All ATR received! sim_status[0x%x]"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5410_112_2_17_23_6_41_1244,(uint8*)"dd",TRACE_CARD_ID, s_sim_status[card_id]);
            }
            /* +cr119609 */
            else
            {
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] NOT all ATR received !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5415_112_2_17_23_6_41_1245,(uint8*)"d",TRACE_CARD_ID);

                if (!SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] active s_sim_wait_resp_timer!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5419_112_2_17_23_6_41_1246,(uint8*)"d",TRACE_CARD_ID);
                    USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id] , DriverTimerExpired, s_sim_wait_resp_timer_value[card_id]);
                    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
                }
            }

            /* -cr119609 */
            break;
            //receive the PPS procesdure card response
        case TB_SIM_PROCESSING_PPS:

            while (s_sim_rx_buf[card_id].check_pos < s_sim_rx_buf[card_id].cur_pos)
            {
                //the pps reponse data is error
                if (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].check_pos] != * (s_pps_info[card_id].pps_data + s_sim_rx_buf[card_id].check_pos))
                {
                    // rx_buf 不等于 pps_info发出的pps数据
                    if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                    {
                        SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
                    }

                    if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
                    {
                        SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
                    }

                    /* +cr204609 */
                    /*+CR183042 Eddie.Wang*/
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] Received PPS data error!!! Reset with no PPS!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5448_112_2_17_23_6_41_1247,(uint8*)"d",TRACE_CARD_ID);
                    /*  bo.chen add the trace for pps data */
                    USIM_PRINT_RX_BUF(card_id);

                    ResetDriver(card_id);
                    s_previous_pps_error[card_id] = TRUE;

                    if (!ResetSIMCard (card_id,TB_SIM_RESET_NO_CHANGE))
                    {
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] NotifyExceptionError (TB_SIMEXP_CARD_OUT) !"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5457_112_2_17_23_6_41_1248,(uint8*)"d",TRACE_CARD_ID);
                        NotifyExceptionError (card_id, TB_SIMEXP_CARD_OUT);
                    }

                    /*-CR183042 Eddie.Wang*/
                    /* -cr204609 */
                    return;
                }

                s_sim_rx_buf[card_id].check_pos++;
            }

            //has receive all the PPS response data
            //此处实现还不完全，参看-3 p15，目前始终认为pps req = pps rsp
            if (s_sim_rx_buf[card_id].cur_pos >= s_pps_info[card_id].pps_type)
            {
                if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                {
                    SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
                }

                if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
                {
                    SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
                }

                //lock the recv data buf,to make sure there no more data coming
                ClearRxBuf(card_id);
                s_sim_status[card_id] = TB_SIM_RECEIVING_RESPONSE;
                ReportCardResetStatus (card_id, TB_SIM_RESETTED_OK);

#ifdef SIM_ENHANCED_PPS_SUPPORT

                if (TB_SIM_PPS_ENHANCED_SPEED == s_pps_info[card_id].pps_type)
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] Enhanced pps supported!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5492_112_2_17_23_6_41_1249,(uint8*)"d",TRACE_CARD_ID);
                    s_sim_register_map[card_id]->rx_clkd = \
                        s_atr_data_buf[card_id].atr_data.F/s_atr_data_buf[card_id].atr_data.D * \
                        s_clk_dvd_factors[s_sim_clk_mode[card_id]];
                    s_sim_register_map[card_id]->tx_clkd = \
                        s_atr_data_buf[card_id].atr_data.F/s_atr_data_buf[card_id].atr_data.D * \
                        s_clk_dvd_factors[s_sim_clk_mode[card_id]];
                }

#endif
            }

            break;
        case TB_SIM_RECEIVING_RESPONSE:

            //both status word has been received,or all the expected data has been received
            if (s_atr_data_buf[card_id].atr_data.t_protocol == 0)
            {
                if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_RX_STATUS_WORD_NUM)
                {
                    //the posible SW1 and SW2,reference to GSM 11.11,section9.4
                    // 主要原因是并不一定就能收到期望长度的返回，例如select成功会返回一个fcp，但是
                    // 失败则只有一个sw1 sw2
                    if (IsStatusWord (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2],
                                      s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1]))
                    {
                        //do not reach the expected length of the response,start the timer to
                        //make sure it's really the end of the response data.
                        //也可能会把正常数据当成sw1 sw2，所以才需要这个定时器，如果定时器用完还每收到
                        // 就可能最后两个字节就是sw1 sw2
                        /* +cr159679 */
                        if ( (s_sim_rx_buf[card_id].cur_pos < s_sim_rx_buf[card_id].rx_expected_len) || (0x60 == s_sim_rx_buf[card_id].buf[0]))
                        {
                            if (s_sim_rx_buf[card_id].cur_pos >= s_sim_rx_buf[card_id].rx_expected_len)
                            {
                                //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] received NULL_Byte(0x60), like status word, not sendresponse up, some data may report later !"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5527_112_2_17_23_6_41_1250,(uint8*)"d",TRACE_CARD_ID);
                            }

                            /* -cr159679 */
                            // 还没有收完
                            /*  bo.chen 20070620 此时启动wwt的timer作为status word  */
                            /*+CR178365 Eddie.Wang*/
                            //                      SCI_ChangeTimer(s_sim_status_word_timer,DriverTimerExpired,s_wwt_timer_value);
/* +cr210334 */
                            if (!g_user_mode_flag[card_id] &&  g_status_tmer_wwt_value_flag[card_id] )   /* CR218847 remove real nw flag  */
                            {
                                USIMDRV_CHANGE_TIMER(card_id,s_sim_status_word_timer[card_id],DriverTimerExpired,s_wwt_timer_value[card_id]);
                            }
                            else
                            {
                                USIMDRV_CHANGE_TIMER(card_id,s_sim_status_word_timer[card_id],DriverTimerExpired,SIM_STATUS_WORD_TIMER_VALUE);
                            }
/* -cr210334 */
                            /*-CR178365 Eddie.Wang*/
                            SCI_ActiveTimer (s_sim_status_word_timer[card_id]);
                        }
                        else
                        {
                            // 接收结束
                            //lock the recv data buf,to make sure there no more data coming
                            s_sim_rx_buf[card_id].is_locked = TRUE;
                            SendResponseUp(card_id);
                        }
                    }
                    else
                    {
                        // add by bo.chen
                        // 用于定位期望长度比实际数据少的情况
                        // 例如期望长度10，但是实际数据20,这样收完10个后，drv不会上报
                        if (s_sim_rx_buf[card_id].cur_pos >= s_sim_rx_buf[card_id].rx_expected_len)
                        {
                            //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckReceiveBuf] expected_len[%d] <= cur_pos[%d]"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5562_112_2_17_23_6_41_1251,(uint8*)"ddd",TRACE_CARD_ID, s_sim_rx_buf[card_id].rx_expected_len, s_sim_rx_buf[card_id].cur_pos);
                        }
                        else
                        {
                            /* +binli opt sim */
                            uint8 water_mark = 0;
                            water_mark = CAL_RX_INT_MARK ( (s_sim_rx_buf[card_id].rx_expected_len - s_sim_rx_buf[card_id].cur_pos));
                            s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id], water_mark);
                            /* -binli opt sim */

                            s_sim_wwt_active[card_id] = TRUE;
                        }
                    }
                }
                else
                {
                    s_sim_wwt_active[card_id] = TRUE;
                }
            }
            else
            {
                // T = 1
                //if the response data len is less than 2 byte or the data is not end with status word,it is an invalid i-block

                // add by bo.chen
                // 此处是否检验过??
                // IFS比rx的16字节要大，可能也需要接收多次
                // 因此需要判断IFS+4或5与cur_pos的大小??
                // 或者重定水线值??(不予考虑，因为可能一个block 17个byte)
                if ( (s_sim_rx_buf[card_id].cur_pos < TB_SIM_RX_STATUS_WORD_NUM) ||
                        !IsStatusWord (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2],
                                        s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1]))
                {
                    /*  bo.chen 20070616 so if we do not receive status word
                        send R-Block again. this error may not happen  */
                    ErrorHandler (card_id, T1_ERROR_CODE_EDC_ERROR);
                }
                else
                {
                    /*  bo.chen 20070626 reset block send count */
                    s_sim_block_resent_count[card_id] = 0;
                    SendResponseUp(card_id);
                }
            }

            break;
        case TB_SIM_ERROR:
            //clear all the received error data

            // 这个状态从未进来过??
            ClearRxBuf(card_id);
            break;
        default:
            //assert error
            USIM_ASSERT (FALSE);/*assert verified*/
            break;
    }

    if (s_sim_wwt_active[card_id])
    {
        /*  bo.chen 20070620 add status time for wwt    */
        if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
        }
        USIMDRV_CHANGE_TIMER(card_id,s_sim_wwt_timer[card_id],DriverTimerExpired,s_wwt_timer_value[card_id]);

        SCI_ActiveTimer (s_sim_wwt_timer[card_id]);
    }
}
/**********************************************************************
//    Description:
//     check the Received block,and judge whether it is valid
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN CheckBlockValid (int32 card_id,TB_SIM_TX_BLOCK_T *tx_block,TB_SIM_RX_BLOCK_T *rx_block)
{
    BLOCK_VALIDITY_E block_validity;
    BLOCK_TYPE_E tx_block_type;
    BLOCK_TYPE_E rx_block_type;
    uint8 edc,edc1;
    uint16 offset;
    BOOLEAN result = TRUE;

    USIM_ASSERT (tx_block != NULL);/*assert verified*/
    USIM_ASSERT (rx_block != NULL);/*assert verified*/

    //only when receive response status and the edc byte have been received,check block validity
    if (TB_SIM_RECEIVING_RESPONSE != s_sim_status[card_id])
    {
        return TRUE;
    }
    else if (rx_block->have_edc != TRUE)
    {
        return FALSE;
    }

    /*  wait cwt timerout   */
    if (s_rx_parity_recv[card_id])
    {
        /*  if rx parity happen donot check this block for the invalid
            parity  */
        return FALSE;
    }

    /*  stop the wwt timer because we receive the end of block  */
    if (SCI_IsTimerActive (s_sim_cwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_cwt_timer[card_id]);
    }


    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckBlockValid] Section1 Tick[0x%x], CWT_Timer[0x%x], block_valid[0x%x], block_type[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5677_112_2_17_23_6_41_1252,(uint8*)"ddddd", TRACE_CARD_ID,SCI_GetTickCount(), s_cwt_timer_value[card_id], rx_block->base_info.valid_block, tx_block->base_info.s_block.block_type);


    if (! (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_NAD)) /*lint !e655 */
    {
        ErrorHandler (card_id, T1_ERROR_CODE_NAD_ERROR);
        return FALSE;
    }
    else if (! (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_PCB)) /*lint !e655 */
    {
        ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
        return FALSE;
    }
    else if (! (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_LEN)) /*lint !e655 */
    {
        ErrorHandler (card_id, T1_ERROR_CODE_LEN_ERROR);
        return FALSE;
    }

    /*  20070716 check pcb status
        if req is sent, rsp is expected
        */
    if (TB_SIM_BLOCK_S_BLOCK == tx_block->base_info.block_type)
    {
        if (! (tx_block->base_info.s_block.is_response))
        {
            if (TB_SIM_BLOCK_S_BLOCK == rx_block->base_info.block_type)
            {
                if ( (rx_block->base_info.s_block.block_type) == \
                        ( (tx_block->base_info.s_block.block_type) | (1 << USIM_CONST_NUM5)))
                {
                    /*  the rsp is not the correctly for that req*/
                    ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
                    return FALSE;
                }
            }
            else
            {
                ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
                return FALSE;
            }
        }
    }

    //check edc
    edc = GetEDC (rx_block->ext_byte,rx_block->cur_ext_byte_count);

    if ( (s_sim_rx_block[card_id].info_data_len != 0) && \
            (TB_SIM_BLOCK_I_BLOCK == rx_block->base_info.block_type))   //I-block or R-Block which has info data
    {
        offset = s_sim_rx_block_buf[card_id].cur_pos - s_sim_rx_block[card_id].info_data_len;
        edc1 = GetEDC (s_sim_rx_block_buf[card_id].buf + offset,s_sim_rx_block[card_id].info_data_len);
        edc ^= edc1;
    }

    if (rx_block->ext_byte[rx_block->cur_ext_byte_count] != edc)
    {
        ErrorHandler (card_id, T1_ERROR_CODE_EDC_ERROR);
        return FALSE;
    }

    //check block validity
    /*  bo.chen 20070621 需要判断这个block是否n(r)正确  */
    tx_block_type = GetBlockType (tx_block->base_info);
    rx_block_type = GetBlockType (rx_block->base_info);
    block_validity = s_sim_check_block[tx_block_type][rx_block_type];

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckBlockValid] Section2 tx_block_type[0x%x], rx_block_type[0x%x],block_validity[0x%x],Tick[0x%x],info_len[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5745_112_2_17_23_6_41_1253,(uint8*)"dddddd", TRACE_CARD_ID,tx_block_type,rx_block_type,block_validity, SCI_GetTickCount(), rx_block->info_data_len);


    if (TB_SIM_BLOCK_I_BLOCK == rx_block->base_info.block_type)
    {
        if ( (BLOCK_VALID == block_validity) || \
                (BLOCK_SEND_R == block_validity))
        {
            s_sim_block_resent_count[card_id] = 0;
            /*  this I-block is valid   */
            SCI_MEMCPY (&s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos], \
                        &s_sim_rx_block_buf[card_id].buf[0], s_sim_rx_block_buf[card_id].cur_pos);
            s_sim_rx_buf[card_id].cur_pos += s_sim_rx_block_buf[card_id].cur_pos;
            /*  add pcb nr  */
            s_pcb_nr[card_id] = (rx_block->base_info.i_block.ns + 1) % 2;
        }
    }


    if (BLOCK_TYPE_R_OK == rx_block_type)
    {
        /*  if we receive R(ok) check the last tx I-block*/
        if (tx_block->base_info.i_block.chain_M)
        {
            s_sim_block_resent_count[card_id] = 0;
            block_validity = BLOCK_SEND_NEXT;
        }
    }



    switch (block_validity)
    {
        case BLOCK_VALID:

            if (BLOCK_TYPE_I_ZERO == rx_block_type)
            {
                result = TRUE;
            }
            else if (BLOCK_TYPE_I_ONE == rx_block_type)
            {
                result = FALSE;
            }
            else
            {
                result = FALSE;
            }

            break;
        case BLOCK_SEND_NEXT:
            //regulate the tx block poiter,then to send the next block
            tx_block->block_begin_pos += s_atr_data_buf[card_id].atr_data.IFSC+4;
            tx_block->block_end_pos += s_atr_data_buf[card_id].atr_data.IFSC+4;

            if (tx_block->block_end_pos > s_sim_tx_buf[card_id].tx_data_len)
            {
                tx_block->block_end_pos = s_sim_tx_buf[card_id].tx_data_len;
            }

            //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckBlockValid] send_next block_begin_pos[0x%x], endpos[0x%x], tx_pcb[0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5806_112_2_17_23_6_42_1254,(uint8*)"dddd", TRACE_CARD_ID,tx_block->block_begin_pos, tx_block->block_end_pos, s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos+TB_SIM_BLOCK_PRO_PCB]);

            s_sim_tx_block[card_id].base_info.i_block.ns = \
                                                  GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM6);
            s_sim_tx_block[card_id].base_info.i_block.chain_M = \
                    (BOOLEAN) GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM5);

            s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_I_BLOCK;

            TransmitInInt_T1(card_id);
            result = FALSE;
            break;
            /*  R-Block error for the i-block from usim */
        case BLOCK_RESEND:
            {
                /*  resend the last i-block */
                s_sim_tx_buf[card_id].cur_pos = tx_block->block_begin_pos;
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckBlockValid] resend block_begin_pos[0x%x], endpos[0x%x], tx_pcb[0x%x]"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5825_112_2_17_23_6_42_1255,(uint8*)"dddd", TRACE_CARD_ID,tx_block->block_begin_pos, tx_block->block_end_pos, s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos+TB_SIM_BLOCK_PRO_PCB]);

                s_sim_tx_block[card_id].base_info.i_block.ns = \
                                                      GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM6);
                s_sim_tx_block[card_id].base_info.i_block.chain_M = \
                        (BOOLEAN) GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM5);
                s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_I_BLOCK;

                TransmitInInt_T1(card_id);
                result = FALSE;
                break;
            }
        case BLOCK_SEND_R:

            if (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_PCB) /*lint !e655 */
            {
                SendRBlock (card_id, SEND_RBLOCK_FOR_IBLOCK_ACK, TB_SIM_R_BLOCK_ERROR_FREE);
            }
            else
            {
                SendRBlock (card_id, SEND_RBLOCK_FOR_IBLOCK_ERROR, TB_SIM_R_BLOCK_OTHER_ERROR);
            }

            result = FALSE;
            break;
        case BLOCK_SEND_SREP:
            /*  add handler for S-block */
            HandleUICCSBlock (card_id, rx_block);
            result = FALSE;
            break;
        case BLOCK_INVALID:
            ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
            return FALSE;

        default:
            result = FALSE;
            break;
    }

    ClearRxBlock(card_id);
    return result;
}
/**********************************************************************
//    Description:
//     check the Received n(r)
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static BOOLEAN CheckNRValid (int32 card_id,TB_SIM_RX_BLOCK_T *rx_block)
{
    BOOLEAN nr_validity = TRUE;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckNRValid] input pcb_nr[0x%x], ns[0x%x], block_type[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5878_112_2_17_23_6_42_1256,(uint8*)"dddd", TRACE_CARD_ID,s_pcb_nr[card_id], rx_block->base_info.i_block.ns, rx_block->base_info.block_type);

    /*  check nr    */
    if (TB_SIM_BLOCK_I_BLOCK == rx_block->base_info.block_type)
    {
        if (rx_block->base_info.i_block.ns != s_pcb_nr[card_id])
        {
            /*  the nr is not correct   */
            nr_validity = FALSE;
        }
    }

    return nr_validity;
}
/**********************************************************************
//    Description:
//     check the length of s-block r-block and i-block
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static BOOLEAN CheckBlockLenValid (
    int32                       card_id,
    uint16                      *len_ptr,
    TB_SIM_BASE_BLOCK_INFO_T    *rx_block
)
{
    BOOLEAN len_valid = FALSE;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[CheckBlockLenValid] ifsd[0x%x],act_len[0x%x],block_type[0x%x],chain[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_5907_112_2_17_23_6_42_1257,(uint8*)"ddddd", TRACE_CARD_ID,s_sim_ifsd[card_id], *len_ptr, rx_block->block_type, rx_block->i_block.chain_M);

    switch (rx_block->block_type)
    {
        case TB_SIM_BLOCK_I_BLOCK:
            {
                /*  check if the len of i block is large than
                    the ifsd    */
                if (s_sim_ifsd[card_id] < *len_ptr)
                {
                    if (rx_block->i_block.chain_M)
                    {
                        /*  if chain_m then len_ptr should be the max ifsd  */
                        //*len_ptr = s_sim_ifsd;
                    }
                    else
                    {
                        /*  the last block, which size may be 0 TBD */
                        //*len_ptr = 0;
                    }
                }
                else
                {
                    len_valid = TRUE;
                }

                break;
            }
        case TB_SIM_BLOCK_R_BLOCK:
            {
                if (0 != *len_ptr)
                {
                    //*len_ptr = 0;
                }
                else
                {
                    len_valid = TRUE;
                }

                break;
            }
        case TB_SIM_BLOCK_S_BLOCK:
            {
                switch (rx_block->s_block.block_type)
                {
                    case TB_SIM_S_BLOCK_INFO_FIELD_REQ:
                    case TB_SIM_S_BLOCK_EXT_BWT_REQ:
                    case TB_SIM_S_BLOCK_INFO_FIELD_RSP:
                    case TB_SIM_S_BLOCK_EXT_BWT_RSP:
                        {
                            if (1 != *len_ptr)
                            {
                                //*len_ptr = 1;
                            }
                            else
                            {
                                len_valid = TRUE;
                            }

                            break;
                        }
                    default:
                        {
                            if (0 != *len_ptr)
                            {
                                //*len_ptr = 0;
                            }
                            else
                            {
                                len_valid = TRUE;
                            }

                            break;
                        }
                }

                break;
            }
        default:
            {
                SCI_ASSERT (FALSE);/*assert to do*/
                break;
            }
    }

    return len_valid;
}

/**********************************************************************
//    Decode the atr data in the data buffer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void DecodeATRData (int32 card_id,uint8 cur_check_pos,uint8 *pdata,uint16 *except_len)
{
    // td_i用于计数
    uint8 temp,data_byte;
    uint8 td_i, protocol;

    USIM_ASSERT (pdata != NULL);/*assert verified*/

    temp = pdata[cur_check_pos];

    // 初始化的时候s_atr_data_buf.Tdi_pos是等于1，这个时候应该是指向T0的
    if (cur_check_pos == s_atr_data_buf[card_id].Tdi_pos)    //tdi
    {
        td_i = 0;
        // 初始值是0，现在加1等于1，表示指向的是T0了，td_count用于计算td的个数，包括t0
        s_atr_data_buf[card_id].td_count ++;

        if (s_atr_data_buf[card_id].td_count == 1)
        {
            // 这个是T0，bit0-3是historical byte个数
            //*except_len += (temp & TB_SIM_ATR_HISTORY_CHARACTER_MARK) + 1; // + history byte len + TCK
            //bo.chen donot add TCK at first
            *except_len += (temp & TB_SIM_ATR_HISTORY_CHARACTER_MARK); // + history byte len
            s_atr_data_buf[card_id].atr_data.his_byte_len = temp & TB_SIM_ATR_HISTORY_CHARACTER_MARK;
        }
        else
        {
            //此处解析是不是有问题??应该是在上一个if的else中??否则如果没有Td0就错了
            //bo.chen
            protocol = temp & TB_SIM_ATR_T_PROTOCAL_MARK;

            switch (protocol)
            {
                case 0x01:
                    s_atr_data_buf[card_id].is_support_t1 = TRUE;
                    /* fall through */
                    //lint -fallthrough
                case 0x00:

                    if (s_atr_data_buf[card_id].first_occur_protocol)
                    {
                        s_atr_data_buf[card_id].atr_data.t_protocol = protocol;
                        s_atr_data_buf[card_id].first_occur_protocol = FALSE;
                    }

                    /*  bo.chen add cwi init because of the reuse of cwi    */
                    if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
                    {
                        /*  ref to 7816-3 8.2 p15   */
                        s_atr_data_buf[card_id].atr_data.CWI = 10;
                    }
                    else
                    {
                        s_atr_data_buf[card_id].atr_data.CWI = TB_SIM_DEFAULT_FACTOR_CWI;
                    }

                    break;
                case 0x0f:
                    s_atr_data_buf[card_id].is_t15_exist = TRUE;
                    break;
                default:
                    break;
            }
        }

        //by bo.chen
        //检验是否会发送TCK
        //SCI_TRACE_LOW("condition is %d", ( (s_atr_data_buf.is_support_t1 || s_atr_data_buf.is_t15_exist) && (s_tck_will_be_sent)));
        if ( (s_atr_data_buf[card_id].is_support_t1 || s_atr_data_buf[card_id].is_t15_exist) && (!s_tck_will_be_sent[card_id]))
        {
            //SCI_TRACE_LOW("(*except_len)--;");
            (*except_len) ++;
            s_tck_will_be_sent[card_id] = TRUE;
        }

        // 依次看TAi TBi TCi TDi是否存在，如果存在，则在buf中的位置，分别存于s_atr_data_buf中
        // 一次记录一个i值的TA TB TC TD
        if (temp & TB_SIM_ATR_TA_CHARACTER_MARK)
        {
            td_i++;
            s_atr_data_buf[card_id].Tai_pos = cur_check_pos + td_i;
        }

        if (temp & TB_SIM_ATR_TB_CHARACTER_MARK)
        {
            td_i++;
            s_atr_data_buf[card_id].Tbi_pos = cur_check_pos + td_i;
        }

        if (temp & TB_SIM_ATR_TC_CHARACTER_MARK)
        {
            td_i++;
            s_atr_data_buf[card_id].Tci_pos = cur_check_pos + td_i;
        }

        if (temp & TB_SIM_ATR_TD_CHARACTER_MARK)
        {
            td_i++;
            s_atr_data_buf[card_id].Tdi_pos = cur_check_pos + td_i;
        }

        //这个返回的长度，第一次读T0的时候，得到historical byte 加 tck的长度
        //再加TA1 TB1 TC1 TD1(如果存在)，例如都存在则加4
        // 第二次读TD1的时候，就是原来的值再加上TA2 TB2 TC2 TD2的个数(如果存在)
        // 最后一次进入此处返回的就是一个完整的ATR长度
        *except_len += td_i;
    }
    else if (cur_check_pos == s_atr_data_buf[card_id].Tai_pos)   //tai
    {
        // 初值为0，所以解析T0的时候不会进入
        if (s_atr_data_buf[card_id].td_count == 1)
        {
            // 等于1表示是TA1,因为只解析了T0
            data_byte = (temp & 0xf0) >> 4;
            s_atr_data_buf[card_id].atr_data.F = s_trans_factor_F[data_byte];
            data_byte = temp & 0x0f;
            s_atr_data_buf[card_id].atr_data.D = s_trans_factor_D[data_byte];
        }
        else if (s_atr_data_buf[card_id].td_count == 2)
        {
            // TA2
            s_atr_data_buf[card_id].is_ta2_exist = TRUE;
            s_atr_data_buf[card_id].ta2_byte = temp;
        }
        else if (s_atr_data_buf[card_id].td_count == 3 \
                 && s_atr_data_buf[card_id].is_support_t1 && s_atr_data_buf[card_id].first_ta1_after_t1)
        {
            // 7816-3 p20
            /*  bo.chen 20070621 add the check for ta3 temp  */
            s_atr_data_buf[card_id].atr_data.IFSC = temp;
            s_atr_data_buf[card_id].first_ta1_after_t1 = FALSE;
        }
        else if (s_atr_data_buf[card_id].is_t15_exist && s_atr_data_buf[card_id].first_ta1_after_t15)
        {
            //7816-3 p11
            s_atr_data_buf[card_id].atr_data.X = (temp & 0xc0) >> 6;
            s_atr_data_buf[card_id].atr_data.U = temp & 0x3f;
            s_atr_data_buf[card_id].first_ta1_after_t15 = FALSE;
            s_atr_data_buf[card_id].is_x_u_present = TRUE;
        }
    }
    else if (cur_check_pos == s_atr_data_buf[card_id].Tbi_pos) //tbi
    {
        if (s_atr_data_buf[card_id].td_count == 1)
        {
            //TB1 有误??没有按bit对应? 7816-3 p11
            // modify by bo.chen
            s_atr_data_buf[card_id].atr_data.I = (temp & 0x60) >> 5;
            s_atr_data_buf[card_id].atr_data.P = temp & 0x1f;
        }
        else if (s_atr_data_buf[card_id].td_count == 2)
        {
            //TB2
            s_atr_data_buf[card_id].atr_data.P = temp;
        }
        else if (s_atr_data_buf[card_id].is_support_t1 && s_atr_data_buf[card_id].first_tb1_after_t1)
        {
            // -3 p20 同TA1
            s_atr_data_buf[card_id].atr_data.BWI = (temp & 0xf0) >> 4;

            /*  bo.chen 20070718 if t=0, we donot change the cwi used for
                t=0 */
            if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
            {
            }
            else
            {
                s_atr_data_buf[card_id].atr_data.CWI = (temp & 0x0f);
            }

            if (0 == s_atr_data_buf[card_id].atr_data.BWI)
            {
                s_atr_data_buf[card_id].atr_data.BWI = TB_SIM_DEFAULT_FACTOR_BWI;
            }

            if (0 == s_atr_data_buf[card_id].atr_data.CWI)
            {
                if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
                {
                    /*  ref to 7816-3 8.2 p15   */
                    s_atr_data_buf[card_id].atr_data.CWI = 10;
                }
            }

            s_atr_data_buf[card_id].BWT = GetBWT (card_id,s_atr_data_buf[card_id].atr_data.BWI);
            s_atr_data_buf[card_id].first_tb1_after_t1 = FALSE;
        }
    }
    else if (cur_check_pos == s_atr_data_buf[card_id].Tci_pos)   //tci
    {
        if (s_atr_data_buf[card_id].td_count == 1)
        {
            s_atr_data_buf[card_id].atr_data.N = temp;
        }
        else if (s_atr_data_buf[card_id].is_support_t1 && s_atr_data_buf[card_id].first_tc1_after_t1)
        {
            s_atr_data_buf[card_id].atr_data.EDC = temp & 0x01;
            s_atr_data_buf[card_id].first_tc1_after_t1 = FALSE;
        }/* bo.chen 20070620 tc(2) for t=0 */
        else if (!s_atr_data_buf[card_id].is_support_t1)
        {
            /*  bo.chen 暂时复用cwi作为t=0的控制 */
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[DecodeATRData] TC1 default[0x%x]to be [0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6203_112_2_17_23_6_42_1258,(uint8*)"ddd", TRACE_CARD_ID,s_atr_data_buf[card_id].atr_data.CWI, temp);
            s_atr_data_buf[card_id].atr_data.CWI = temp;
        }
    }
    else if (s_atr_data_buf[card_id].hisbyte_is_end == FALSE) //history byte
    {
        // historical byte
        s_atr_data_buf[card_id].atr_data.history_byte[s_atr_data_buf[card_id].hb_count] = temp;
        s_atr_data_buf[card_id].hb_count++;

        if (s_atr_data_buf[card_id].hb_count == s_atr_data_buf[card_id].atr_data.his_byte_len)
        {
            s_atr_data_buf[card_id].hisbyte_is_end = TRUE;
        }
    }
    else
    {
        s_atr_data_buf[card_id].is_atr_data_end = TRUE;
    }

}
/**********************************************************************
//    Description:
//    When a BWT time-out occurs or the invalid block has been Received,calling this function
//    to do error handler
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void ErrorHandler (int32 card_id,T1_ERROR_CODE_E error_code)
{

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] reset_count[0x%x], block_type[0x%x], error_code[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6237_112_2_17_23_6_42_1259,(uint8*)"dddd", TRACE_CARD_ID,s_sim_block_resent_count[card_id], s_sim_tx_block[card_id].base_info.block_type, error_code);

    /*  bo.chen 20070820 temp   */
    if (TB_SIM_BLOCK_R_BLOCK == s_sim_rx_block[card_id].base_info.block_type)
    {
        s_sim_block_resent_count[card_id] = 0;
    }

    if (s_sim_block_resent_count[card_id] < TB_SIM_MAX_RESENT_COUNT)
    {
        switch (error_code)
        {
            case T1_ERROR_CODE_BWT_TIMER_OUT:
                {
                    //if I-Block has been sent and a BWT time-out occurs,an R-Block is sent
                    if (s_sim_tx_block[card_id].base_info.block_type == TB_SIM_BLOCK_I_BLOCK)
                    {
                        SendRBlock (card_id, SEND_RBLOCK_FOR_IBLOCK_ERROR, TB_SIM_R_BLOCK_OTHER_ERROR);
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] When I-Block BWT time-out,send a r-block"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6255_112_2_17_23_6_43_1260,(uint8*)"d",TRACE_CARD_ID);
                        s_sim_block_resent_count[card_id]++;
                    }
                    //if an R-Block been sent and bwt time-out,the R-Block shall be resent
                    else if (s_sim_tx_block[card_id].base_info.block_type == TB_SIM_BLOCK_R_BLOCK)
                    {
                        SendRBlock (card_id, SEND_RBLOCK_FOR_RBLOCK_ERROR, TB_SIM_R_BLOCK_OTHER_ERROR);
                        s_sim_block_resent_count[card_id]++;
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] When R-block BWT time-out,resent a s-block request"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6263_112_2_17_23_6_43_1261,(uint8*)"d",TRACE_CARD_ID);
                    }
                    else if (s_sim_tx_block[card_id].base_info.block_type == TB_SIM_BLOCK_S_BLOCK)
                    {
                        //when an S(...request)has been sent and a BWT time-out occurs,the S(...request) shall be resent
                        if (s_sim_tx_block[card_id].base_info.s_block.is_response == FALSE)
                        {
                            SendSBLOCK (card_id, SEND_SBLOCK_FOR_SBLOCK_REQUEST, s_sim_rx_block[card_id].base_info.s_block.block_type, 0);
                            s_sim_block_resent_count[card_id]++;
                            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] When S-block request BWT time-out,resent a s-block request"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6272_112_2_17_23_6_43_1262,(uint8*)"d",TRACE_CARD_ID);
                        }
                        //when an S(...response)has been sent and a BWT time-out occurs,an R-Block shall be sent
                        else
                        {
                            SendRBlock (card_id, SEND_RBLOCK_FOR_SBLOCK_ERROR, TB_SIM_R_BLOCK_OTHER_ERROR);
                            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] When S-block response BWT time-out,send a r-block"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6278_112_2_17_23_6_43_1263,(uint8*)"d",TRACE_CARD_ID);
                            s_sim_block_resent_count[card_id]++;
                        }
                    }
                    else
                    {
                        /*  USIM_ASSERT(FALSE); */
                    }

                    break;
                }
            case T1_ERROR_CODE_EDC_ERROR:
            case T1_ERROR_CODE_LEN_ERROR:
            case T1_ERROR_CODE_NAD_ERROR:
            case T1_ERROR_CODE_PCB_ERROR:
                {
                    SendRBlock (card_id, SEND_RBLOCK_FOR_IBLOCK_ERROR, TB_SIM_R_BLOCK_EDC_PARITY_ERROR);
                    s_sim_block_resent_count[card_id]++;
                    break;
                }
            case T1_ERROR_CODE_WRONG_INSTR:
                {
                    SendRBlock (card_id, SEND_RBLOCK_FOR_IBLOCK_ERROR, TB_SIM_R_BLOCK_OTHER_ERROR);
                    s_sim_block_resent_count[card_id]++;
                    break;
                }
            default:
                {
                    break;
                }
        }
    }
    else if (s_sim_block_resent_count[card_id] < TB_SIM_MAX_RESENT_COUNT + 3)
    {

        if (TB_SIM_BLOCK_NOT_DEF == s_sim_rx_block[card_id].base_info.block_type)
        {
            /*  I R R poweroff  */
            /*  ref 7.3.13 start
                we do not receive any data from uicc then we need
                not send resync, the init of block_type in
                s_sim_rx_block is TB_SIM_BLOCK_NOT_DEF
                */
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] Unknown Block Type ,refer to 7.3.13"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6321_112_2_17_23_6_43_1264,(uint8*)"d",TRACE_CARD_ID);
            NotifyExceptionError (card_id, TB_SIMEXP_T1_CARD_UNRESPONSE);   //CR205519 Edie.Wang
            //SIM_PowerOffSIM();   //CR205519 Edie.Wang inactived
        }
        else
        {
            /*  I R R S S S poweroff*/
            /*  we need to send resynch req, resent count should be 3 */
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] resynch req needed, refer to 7.3.13"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6329_112_2_17_23_6_43_1265,(uint8*)"d",TRACE_CARD_ID);
            SendSBLOCK (card_id, SEND_SBLOCK_FOR_SBLOCK_REQUEST, TB_SIM_S_BLOCK_RESYN_REQ, 0);
            s_sim_block_resent_count[card_id]++;
        }
    }
    else
    {
        NotifyExceptionError (card_id,TB_SIMEXP_T1_CARD_UNRESPONSE);   //CR205519 Edie.Wang
        //SIM_PowerOffSIM();  //CR205519 Edie.Wang inactived
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[ErrorHandler] continous BWT time-out or invalid block to the max time"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6338_112_2_17_23_6_43_1266,(uint8*)"d",TRACE_CARD_ID);
    }

    /*  20070629 reset the rx block for the next block    */
    ClearRxBlock(card_id);
}

/**********************************************************************
//    Description:
//    check whether the supply voltage switching is need,and whether this switching is available,
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CheckSupplyVol (
                            TB_SIM_WORKING_VOLT_E cur_work_vol,
                            uint8 card_support_vol,
                            BOOLEAN *change_is_need,
                            BOOLEAN *change_is_support)
{
    USIM_ASSERT (cur_work_vol >= TB_SIM_WORKING_VOLT_1_8V);/*assert verified*/
    USIM_ASSERT (cur_work_vol <= TB_SIM_WORKING_VOLT_3V);/*assert verified*/

    // card_support_vol表明卡支持那个类别的电压,-3补充文档中定义了ui

    // 默认是01，表示A 0000 0010 b 0000 0100 c
    if ( (card_support_vol!= 0x1) &&
            (card_support_vol!= 0x2) &&
            (card_support_vol!= 0x3) &&
            (card_support_vol!= 0x4) &&
            (card_support_vol!= 0x6) &&
            (card_support_vol!= 0x7))
    {
        //表示RFU
        *change_is_need = FALSE;
        return;
    }

    if (cur_work_vol == TB_SIM_WORKING_VOLT_1_8V)
    {
        // C类，并且UI表示支持C，所以也不需要变换
        if ( (card_support_vol== 0x04)
                || (card_support_vol== 0x06)
                || (card_support_vol== 0x07))
        {
            *change_is_need = FALSE;
            return;
        }
        else if (card_support_vol== 0x01)
        {
            // A类，需要变换电压,并且不支持C类
            *change_is_need = TRUE;
            *change_is_support = FALSE;
            return;
        }
        else if ( (card_support_vol == 0x02)
                  || (card_support_vol== 0x03))
        {
            // B 或 AB, 需要加压,并且如果1.8和3v通用，则支持当前的这个电压
            *change_is_need = TRUE;
#ifdef SIM_SUPPORT_1D8V_3V
            *change_is_support = TRUE;
#else
            *change_is_support = FALSE;
#endif
        }
    }
    else //if current supply voltage is 3v
    {
        if ( (card_support_vol== 0x02) //card support 3v only
                || (card_support_vol== 0x03) //card support 3v and 5v
                || (card_support_vol== 0x06) //card support 1.8v and 3v
                || (card_support_vol== 0x07)) //card support 1.8v,3v and 5v
        {
            *change_is_need = FALSE;
            return;
        }
        else if (card_support_vol== 0x01) //card only support 5v
        {
            *change_is_need = TRUE;
            *change_is_support = FALSE;
            return;
        }
        else if (card_support_vol == 0x04) //card only support 1.8v
        {
            *change_is_need = TRUE;
#ifdef SIM_SUPPORT_1D8V_3V
            *change_is_support = TRUE;
#else
            *change_is_support = FALSE;
#endif
        }
    }
}

/**********************************************************************
//    Description:
//    When received a ack byte such as 0x61,then calling this function to send an get
//  response instruction to the USIM card (only for T0 protocol)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendGetRespCmd (int32 card_id,uint8 sw2_byte)
{
    TB_USIM_APDU_T cmd_apdu;

    s_sim_is_processing_cmd[card_id] = FALSE;
    /*  bo.chen 20070820
        A4 ------->
        61 14 <----
        61 0a <----
        the s_sim_rx_buf.rx_expected_len == rx_expected_len + status word
    */
    cmd_apdu.rx_expected_len = (uint16) sw2_byte + \
                               s_sim_rx_buf[card_id].rx_expected_len - 2;
    cmd_apdu.instr_type = TB_SIMINSTR_RECV_DATA;
    cmd_apdu.instr_code[TB_SIM_APDU_CLA] = s_sim_instr_class[card_id];
    cmd_apdu.instr_code[TB_SIM_APDU_INS] = 0xC0;
    cmd_apdu.instr_code[TB_SIM_APDU_P1] = 0x00;
    cmd_apdu.instr_code[TB_SIM_APDU_P2] = 0x00;
    cmd_apdu.instr_code[TB_SIM_APDU_P3] = sw2_byte;

    //begin to send get response cmd
    SIM_SendInstrCode (card_id, &cmd_apdu);
}
/**********************************************************************
//    Description:
//    Calculate the WWT from the WI(T0)
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static uint32 GetT0WWT (int32 card_id,uint8 WI)
{
    uint32 wwt,cur_clk;

    USIM_ASSERT (s_atr_data_buf[card_id].atr_data.D != 0);/*assert verified*/


    cur_clk = CHIP_GetAPBClk();

    /*+CR181647 Eddie.Wang*/
    //  wwt = WI*960*s_atr_data_buf.atr_data.F/s_atr_data_buf.atr_data.D;
    wwt = WI*960*s_atr_data_buf[card_id].atr_data.F; /* cr203907 */
    /*-CR181647 Eddie.Wang*/

    cur_clk = cur_clk/1000;     //calculate the time in ms
    wwt = wwt*s_clk_dvd_factors[s_sim_clk_mode[card_id]]/cur_clk;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetT0WWT] WI[0x%x] WWT[0x%x] REAL_WWT = WWT +20"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6485_112_2_17_23_6_43_1267,(uint8*)"ddd",TRACE_CARD_ID, WI, wwt);

    wwt += 20;

    return wwt;
}
/**********************************************************************
//    Description:
//    Calculate the CWT from the CWI(T1) cwt= (11 + (1<<cwi)) etu
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static uint32 GetT1CWT (int32 card_id,uint8 CWI)
{
    uint32 cwt,cur_clk;

    USIM_ASSERT (s_atr_data_buf[card_id].atr_data.D != 0);/*assert verified*/


    cur_clk = CHIP_GetAPBClk();

    cwt = (11 + (1 << CWI)) *s_atr_data_buf[card_id].atr_data.F/s_atr_data_buf[card_id].atr_data.D;

    cur_clk = cur_clk/1000;     //calculate the time in ms
    cwt = cwt*s_clk_dvd_factors[s_sim_clk_mode[card_id]]/cur_clk;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetT1CWT] CWI[0x%x], CWT[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6511_112_2_17_23_6_43_1268,(uint8*)"ddd",TRACE_CARD_ID, CWI, cwt);


    return cwt;
}

/**********************************************************************
//    Description:
//    Calculate the BWT from the BWI
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static uint32 GetBWT (int32 card_id,uint8 BWI)
{
    uint32 bwt,cur_clk;

    USIM_ASSERT (BWI <= 15);/*assert verified*/
    USIM_ASSERT (s_atr_data_buf[card_id].atr_data.D != 0);/*assert verified*/

    //the values 10 to 15 are reserved for future use.
    if (BWI >= 10)
    {
        return s_atr_data_buf[card_id].BWT;
    }

    cur_clk = CHIP_GetAPBClk();

    bwt = (11*s_atr_data_buf[card_id].atr_data.F/s_atr_data_buf[card_id].atr_data.D +
           (1 << BWI) *960*372);

    cur_clk = cur_clk/1000;     //calculate the time in ms
    bwt = bwt*s_clk_dvd_factors[s_sim_clk_mode[card_id]]/cur_clk;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetBWT] BWI[0x%x] BWT[0x%x], NEW_BWT[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6544_112_2_17_23_6_43_1269,(uint8*)"dddd",TRACE_CARD_ID, BWI, bwt, bwt+30);

    /*  bo.chen 20070622    */
    //bwt += 50;
    bwt += 30;

    /*     s_atr_data_buf.BWT = TB_SIM_DEFAULT_FACTOR_BWT;   */

    return bwt;
}
/**********************************************************************
//    Description:
//    Redo the previous instruction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void RedoPreInstr (int32 card_id,BOOLEAN need_modify_p3,uint8 p3_byte)
{
    uint16 i;
    // bo.chen 20050331 ref to sendresponseup
    ClearRxBuf(card_id);
    ClearTxBuf(card_id);

    if (!need_modify_p3)
    {
        s_sim_tx_buf[card_id].tx_data_len = s_sim_tx_instr_len[card_id];
        s_sim_rx_buf[card_id].rx_expected_len = s_sim_rx_rexpect_len[card_id];
    }
    else
    {
        //question,it is assumed that the previous instruction type is TB_SIMINSTR_RECV_DATA
        s_sim_tx_buf[card_id].tx_data_len = TB_SIM_APDU_CHEADER_FIVE;
        s_sim_rx_buf[card_id].rx_expected_len = TB_SIM_RX_STATUS_WORD_NUM + p3_byte;
        s_sim_tx_buf[card_id].buf[TB_SIM_APDU_P3] = p3_byte;
    }

    s_sim_rx_buf[card_id].is_locked = FALSE;

    s_have_received_response_data[card_id] = FALSE;  /*NEWMS110357 E*/
    s_have_ack_byte[card_id] = FALSE;    /*NEWMS110357 E*/
    
    //send the command header
    for (i = 0; i < 5; i++)
    {
        s_sim_register_map[card_id]->tx = s_sim_tx_buf[card_id].buf[i];
        s_sim_tx_buf[card_id].cur_pos++;
    }

    /* +cr152252 */
    /* binli modify for sim no response, sim task queue full! */
#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[card_id])
    {
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }
        USIMDRV_CHANGE_TIMER(card_id,s_sim_dma_byte_timer[card_id], DriverTimerExpired, SIM_DMA_TIMER_VALUE);

        SCI_ActiveTimer (s_sim_dma_byte_timer[card_id]);
    }

    else
    {
        if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
        }

        USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);

        SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);
    }

#else

    /*  set rx timeout timer 20090924   */
    if (SCI_IsTimerActive (s_sim_rx_timeout_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_rx_timeout_timer[card_id]);
    }
    USIMDRV_CHANGE_TIMER(card_id,s_sim_rx_timeout_timer[card_id], DriverTimerExpired, SIM_RX_TIMEOUT_VALUE);
    SCI_ActiveTimer (s_sim_rx_timeout_timer[card_id]);

#endif

    s_sim_rxfifo_count[card_id] = 0;
    s_sim_rxfifo_int_count[card_id] = 0;

    USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
    /* -cr152252 */
}
/**********************************************************************
//    Description:
//    Decode the PCB byte to get the basic block information
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BOOLEAN GetBlockBaseInfo (int32 card_id,uint8 pcb_byte,TB_SIM_BASE_BLOCK_INFO_T *block)
{
    USIM_ASSERT (block != NULL);/*assert verified*/

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetBlockBaseInfo] PCB_Byte[0x%x], Tick[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6648_112_2_17_23_6_43_1270,(uint8*)"ddd", TRACE_CARD_ID,pcb_byte, SCI_GetTickCount());

    if (GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM7) == USIM_CONST_NUM0)
    {
        block->block_type = TB_SIM_BLOCK_I_BLOCK;
        block->i_block.ns = GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM6);
        block->i_block.chain_M = GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM5);

        /*  20070704 check the pcb of i-block
            check the valid N(s)& b1-b5
            0 N(s) M 0 0 0 0 0
        */
        if ( (CheckNRValid (card_id, &s_sim_rx_block[card_id])) && (0 == (pcb_byte & 0x1f)))
        {
            block->valid_block |= TB_SIM_BLOCK_FIELD_VALID_PCB; /*lint !e655 */
        }
        else
        {
            block->valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_PCB;
        }
    }
    else if ( (GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM6) == USIM_CONST_NUM1)
              && (GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM7) == USIM_CONST_NUM1))
    {
        block->block_type = TB_SIM_BLOCK_S_BLOCK;

        block->s_block.block_type = (TB_SIM_S_BLOCK_TYPE_E) (pcb_byte & 0x3f);

        block->s_block.is_response = \
                                     (GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM5) && \
                                      (TB_SIM_S_BLOCK_ERROR_VPP != block->s_block.block_type));

        /*  20070704 check the pcb of s-block
            1 1 000000 resynch req
                100000 resynch rsp
                000001 IFS req
                100001 IFS rsp
                000010 Abort req
                100010 Abort rsp
                000011 WTX req
                100011 WTX rsp
                100100 Error on VPP state
        */
        if ( (0x3 >= block->s_block.block_type) ||
                ( (0x20 <= block->s_block.block_type) && (0x24 >= block->s_block.block_type)))
        {
            /*  is valid pcb    */
            block->valid_block |= TB_SIM_BLOCK_FIELD_VALID_PCB; /*lint !e655 */
        }
        else
        {
            /*  the wrong instru    */
            block->valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_PCB;
            block->s_block.block_type = TB_SIM_S_BLOCK_RFU;
        }

    }
    else if ( (GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM7) == USIM_CONST_NUM1)
              && (GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM6) == USIM_CONST_NUM0))
    {
        block->block_type = TB_SIM_BLOCK_R_BLOCK;
        block->r_block.nr = GET_BIT_VALUE (pcb_byte,USIM_CONST_NUM4);
        block->r_block.block_type = (TB_SIM_R_BLOCK_TYPE_E) (pcb_byte & 0x0f);
        /*  20070704 check the pcb of r-block
            1 0 0 n(r) 0000--0010
        */
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetBlockBaseInfo] NR[0x%x], NS[0x%x], block_type[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6716_112_2_17_23_6_44_1271,(uint8*)"dddd", TRACE_CARD_ID,block->r_block.nr, s_sim_tx_block[card_id].base_info.i_block.ns, block->r_block.block_type);

        if ( (TB_SIM_R_BLOCK_RFU > block->r_block.block_type) && \
                (0 == GET_BIT_VALUE (pcb_byte, USIM_CONST_NUM5)))
        {
            if ( (block->r_block.nr == s_sim_tx_block[card_id].base_info.i_block.ns) && \
                    (TB_SIM_R_BLOCK_ERROR_FREE != block->r_block.block_type))
            {
                /*  is valid pcb    */
                block->valid_block |= TB_SIM_BLOCK_FIELD_VALID_PCB; /*lint !e655 */
            }
            else if ( (block->r_block.nr != s_sim_tx_block[card_id].base_info.i_block.ns) && \
                      (TB_SIM_R_BLOCK_ERROR_FREE == block->r_block.block_type))
            {
                /*  is valid pcb    */
                block->valid_block |= TB_SIM_BLOCK_FIELD_VALID_PCB; /*lint !e655 */
            }
            else
            {
                block->valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_PCB;
            }
        }
        else
        {
            block->valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_PCB;
        }


    }
    else
    {
        s_sim_rx_block[card_id].base_info.valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_PCB;
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetBlockBaseInfo] ERROR: Invalid PCB"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6748_112_2_17_23_6_44_1272,(uint8*)"d",TRACE_CARD_ID);

        /*  20070706     */
        block->block_type = TB_SIM_BLOCK_I_BLOCK;
    }

    return TRUE;
}

/**********************************************************************
//    Description:
//    get the detail block type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static BLOCK_TYPE_E GetBlockType (TB_SIM_BASE_BLOCK_INFO_T block)
{
    BLOCK_TYPE_E block_type = 0;

    if (block.block_type == TB_SIM_BLOCK_I_BLOCK)
    {
        if (block.i_block.chain_M == TRUE)
        {
            block_type = BLOCK_TYPE_I_ONE;
        }
        else
        {
            block_type = BLOCK_TYPE_I_ZERO;
        }
    }
    else if (block.block_type == TB_SIM_BLOCK_R_BLOCK)
    {
        if (block.r_block.block_type == TB_SIM_R_BLOCK_ERROR_FREE)
        {
            block_type = BLOCK_TYPE_R_OK;
        }
        else
        {
            block_type = BLOCK_TYPE_R_ERROR;
        }
    }
    else if (block.block_type == TB_SIM_BLOCK_S_BLOCK)
    {
        if (block.s_block.is_response == FALSE)
        {
            block_type = BLOCK_TYPE_S_REQ;
        }
        else
        {
            block_type = BLOCK_TYPE_S_RES;
        }
    }
    else if (block.block_type == TB_SIM_BLOCK_NOT_DEF)
    {
        block_type = BLOCK_TYPE_NOT_DEF;
    }
    else
    {
        USIM_ASSERT (FALSE);/*assert verified*/
    }

    return block_type;
}

/**********************************************************************
//    Description:
//      get edc vlaue of the block
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static  uint8 GetEDC (uint8 *block_data,uint16 data_len)
{
    uint16 i;
    uint8 edc;

    USIM_ASSERT (block_data != NULL);/*assert verified*/
    /*  20070704 the max length is (TB_SIM_MAX_INF_BYTE_NUM + TB_SIM_BLOCK_PRO_BYTE_NUM)
        and the data_len should add 1   */
    USIM_ASSERT (data_len <= (TB_SIM_MAX_INF_BYTE_NUM + TB_SIM_BLOCK_PRO_BYTE_NUM + 1));/*assert verified*/
    USIM_ASSERT (data_len != 0);/*assert verified*/

    edc = block_data[0];

    for (i = 1; i < data_len; i++)
    {
        edc ^= block_data[i];
    }

    return edc;
}
/**********************************************************************
//    Description:
//    Send a r-block.In following cases, a r-block should be sent:
//    1. When an I-block has been sent and a BWT time out occurs or an
//       invalid block has been received.
//    2. When an R-Block was sent and an invalid block is received or BWT time-out
//    3. When an S(...response) has been sent and an invalid block is received or
//       BWT time-out.
//    4. When received an I(1) block
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SendRBlock (
    int32 card_id,
    SEND_RBLOCK_REASON_E send_reason,
    TB_SIM_R_BLOCK_TYPE_E   error_code
)
{
    uint16 i;
    uint8 tempbuf[TB_SIM_BLOCK_EXT_BYTE_NUM];/*assert verified*/

    USIM_ASSERT (send_reason < SEND_RBLOCK_REASON_NUM);/*assert verified*/
    USIM_ASSERT (error_code < TB_SIM_R_BLOCK_RFU);/*assert verified*/

    //record the tx block information
    s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_R_BLOCK;
    s_sim_tx_block[card_id].base_info.r_block.block_type = error_code;


    //fill the r-block
    tempbuf[TB_SIM_BLOCK_PRO_NAD] = 0;
    tempbuf[TB_SIM_BLOCK_PRO_PCB] = SIM_DEFAULT_PCB_RBLOCK | error_code;

    //decide the ns in the pcb byte
    if (send_reason == SEND_RBLOCK_FOR_IBLOCK_ACK)
    {
        /*  bo.chen 20070615 ns+1 % 2   */
        tempbuf[TB_SIM_BLOCK_PRO_PCB] |= \
                                         ( (s_pcb_nr[card_id] & 0x01) << USIM_CONST_NUM4);
        // s_sim_rx_block.base_info.r_block.nr = (s_sim_rx_block.base_info.i_block.ns+1)%2;
    }
    else if (send_reason == SEND_RBLOCK_FOR_IBLOCK_ERROR)
    {
        /*  need retransfer for i block */
        tempbuf[TB_SIM_BLOCK_PRO_PCB] |= \
                                         ( (s_pcb_nr[card_id] & 0x01) << USIM_CONST_NUM4);
        // s_sim_rx_block.base_info.r_block.nr = s_sim_tx_block.base_info.i_block.ns;
    }
    else if (send_reason == SEND_RBLOCK_FOR_RBLOCK_ERROR)
    {
        /*  bo.chen if the r-block is sent and timer out
            we need resend the r-block.  */
        tempbuf[TB_SIM_BLOCK_PRO_PCB] |= \
                                         ( (s_pcb_nr[card_id] & 0x01) << USIM_CONST_NUM4);
        // s_sim_rx_block.base_info.r_block.nr = s_sim_tx_block.base_info.i_block.ns;
    }

    tempbuf[TB_SIM_BLOCK_PRO_LEN] = 0;
    tempbuf[TB_SIM_BLOCK_EPI_EDC] = GetEDC (tempbuf,TB_SIM_BLOCK_PRO_BYTE_NUM);


    //send the r-block
    for (i = 0; i < TB_SIM_BLOCK_EXT_BYTE_NUM; i++)
    {
        s_sim_register_map[card_id]->tx = tempbuf[i];
    }

    /*  bo.chen 20070616 warnning we need enable the tx empty interrupt  */
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SendRBlock] Tick[0x%x] temp[0x%x,0x%x,0x%x,0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6909_112_2_17_23_6_44_1273,(uint8*)"dddddd", TRACE_CARD_ID,SCI_GetTickCount(), tempbuf[0], tempbuf[1], tempbuf[2], tempbuf[3]);

    //active bwt timer
    if (SCI_IsTimerActive (s_sim_bwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_bwt_timer[card_id]);
    }

    /*  temp test   */
    if (0 == s_atr_data_buf[card_id].BWT)
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[SendRBlock] BWT[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_6920_112_2_17_23_6_44_1274,(uint8*)"dd",TRACE_CARD_ID, s_atr_data_buf[card_id].BWT);
        s_atr_data_buf[card_id].BWT = 1000;
    }

    //active bwt timer
    USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DriverTimerExpired,s_atr_data_buf[card_id].BWT);
    SCI_ActiveTimer (s_sim_bwt_timer[card_id]);
}
/**********************************************************************
//    Description:
//    Send a S-Block.In following cases, a S-block should be sent
//    1. When want to sent an S(...request)
//    2. When an S(...request) has been sent and either a BWT time-out occurs or the
//       received response is not an S(...response),S(...request) should be resent.
//    3. When received an S(...request),an (...response)shoud be send.
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SendSBLOCK (
    int32 card_id,
    SEND_SBLOCK_REASON_E send_reason,
    TB_SIM_S_BLOCK_TYPE_E sblock_type,
    uint8 info_data)
{
    uint8 i;

    USIM_ASSERT (send_reason < SEND_SBLOCK_REASON_NUM);/*assert verified*/

    if (send_reason == SEND_SBLOCK_FOR_SBLOCK_ERROR)
    {
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_NAD] = 0;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_PCB] = sblock_type|SIM_DEFAULT_PCB_SBLOCK;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_LEN] = 1;
        s_sim_tx_block[card_id].cur_ext_byte_count = 3;

        if (sblock_type == TB_SIM_S_BLOCK_INFO_FIELD_REQ)
        {
            s_sim_tx_block[card_id].cur_ext_byte_count++;
            s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = info_data;
        }

        s_sim_tx_block[card_id].cur_ext_byte_count++;
        s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = \
            GetEDC (s_sim_tx_block[card_id].ext_byte,s_sim_tx_block[card_id].cur_ext_byte_count - 1);

        //record the tx block information
        s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_S_BLOCK;
        s_sim_tx_block[card_id].base_info.s_block.is_response = FALSE;
        s_sim_tx_block[card_id].base_info.s_block.block_type = sblock_type;
    }
    else if (send_reason == SEND_SBLOCK_FOR_SBLOCK_RESPONSE)
    {
        //send a s(...response)
        /*  bo.chen 20070616 all s reponse should be check all S(response)
            bytes are sent to the usim */
        memcpy (s_sim_tx_block[card_id].ext_byte,s_sim_rx_block[card_id].ext_byte,s_sim_rx_block[card_id].cur_ext_byte_count);
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_PCB] |= (1 << USIM_CONST_NUM5) ;
        s_sim_tx_block[card_id].ext_byte[s_sim_rx_block[card_id].cur_ext_byte_count] = \
                GetEDC (s_sim_tx_block[card_id].ext_byte,s_sim_rx_block[card_id].cur_ext_byte_count);

        //record the tx block information
        s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_S_BLOCK;
        s_sim_tx_block[card_id].base_info.s_block.is_response = TRUE;
        s_sim_tx_block[card_id].base_info.s_block.block_type = s_sim_rx_block[card_id].base_info.s_block.block_type;

        s_sim_tx_block[card_id].cur_ext_byte_count = s_sim_rx_block[card_id].cur_ext_byte_count;
    }
    /*  send ifs req to sim 20070614 bo.chen    */
    else if (send_reason == SEND_SBLOCK_FOR_SBLOCK_REQUEST)
    {
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_NAD] = 0;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_PCB] = sblock_type|SIM_DEFAULT_PCB_SBLOCK;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_LEN] = 0;
        s_sim_tx_block[card_id].cur_ext_byte_count = 2;

        if ( (sblock_type == TB_SIM_S_BLOCK_INFO_FIELD_REQ) || \
                (TB_SIM_S_BLOCK_EXT_BWT_REQ == sblock_type))
        {
            s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_LEN] = 1;
            s_sim_tx_block[card_id].cur_ext_byte_count++;
            s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = info_data;
        }

        s_sim_tx_block[card_id].cur_ext_byte_count++;
        s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = GetEDC (s_sim_tx_block[card_id].ext_byte,s_sim_tx_block[card_id].cur_ext_byte_count);

        //record the tx block information
        s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_S_BLOCK;
        s_sim_tx_block[card_id].base_info.s_block.is_response = FALSE;
        s_sim_tx_block[card_id].base_info.s_block.block_type = sblock_type;
    }

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetT0WWT]  reason[0x%x],0x%x,[0x%x,0x%x,0x%x,0x%x,0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7019_112_2_17_23_6_44_1275,(uint8*)"dddddddd", TRACE_CARD_ID, SCI_GetTickCount(), send_reason, s_sim_tx_block[card_id].cur_ext_byte_count, s_sim_tx_block[card_id].ext_byte[0], s_sim_tx_block[card_id].ext_byte[1], s_sim_tx_block[card_id].ext_byte[2], s_sim_tx_block[card_id].ext_byte[3], s_sim_tx_block[card_id].ext_byte[4] );

    /*  warnning need tx empty interrupt */
    //send the s-block
    for (i = 0; i < s_sim_tx_block[card_id].cur_ext_byte_count + 1; i++)
    {
        s_sim_register_map[card_id]->tx = s_sim_tx_block[card_id].ext_byte[i];
    }

    //active bwt timer
    if (SCI_IsTimerActive (s_sim_bwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_bwt_timer[card_id]);
    }

    /*  temp test   */
    if (0 == s_atr_data_buf[card_id].BWT)
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetT0WWT] sendsblock the bwt is 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7037_112_2_17_23_6_44_1276,(uint8*)"dd", TRACE_CARD_ID, s_atr_data_buf[card_id].BWT);
        s_atr_data_buf[card_id].BWT = 1000;
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DriverTimerExpired,s_atr_data_buf[card_id].BWT);
    SCI_ActiveTimer (s_sim_bwt_timer[card_id]);

}
/*****************************************************************************/
//  Description:  The driver call the callback function to notify the transport
//                layer what event has happened.
//  Author:Yongxia.zhang
//  Note:
/*****************************************************************************/
static void DriverCallBack (int32 card_id, TB_MSG *msg)
{
    if (s_sim_call_back_func[card_id] == NULL)
    {
        //SCI_TRACE_LOW:"\r\nUSIMDRV::Callback func is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7053_112_2_17_23_6_44_1277,(uint8*)"", TRACE_CARD_ID);
        return;
    }
    else
    {
        (*s_sim_call_back_func[card_id]) (msg);
    }
}
#ifdef SIM_SUPPORT_1D8V_3V
/**********************************************************************
//    Description:
//      According the current supply voltage to get the next switch class
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN GetNextSupplyVoltage (int32 card_id,TB_SIM_WORKING_VOLT_E cur_work_vol,TB_SIM_WORKING_VOLT_E *next_work_vol)
{
    // 1.8v 到3v
    if (TB_SIM_WORKING_VOLT_1_8V == s_sim_working_volt[card_id])
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[GetNextSupplyVoltage] 1.8V -> 3V"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7073_112_2_17_23_6_44_1278,(uint8*)"d", TRACE_CARD_ID);
        *next_work_vol = TB_SIM_WORKING_VOLT_3V;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/**********************************************************************
//    Description:
//      Set the working voltage of the sim card.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SetSIMPower (int32 card_id,TB_SIM_WORKING_VOLT_E work_vol)
{
#ifndef PLATFORM_SC8800G
    /*  bo.chen change for the sim power    */
    /*
    [b1_set, b1_rst]设为[1,0], [b0_set,b0_rst]设为[1,0]--------->1.8V
    [b1_set, b1_rst]设为[0,1], [b0_set,b0_rst]设为[0,1]--------->3.0V
    */
    volatile uint32 ldo_value;


    ldo_value = REG32 ( (GR_LDO_CTL2));


    SCI_TRACE_LOW ("USIMDRV:[%d]:[SetSIMPower] ldo_ctl2[%d] (1[3v] 2[1.8v] 3[?])", TRACE_CARD_ID, ldo_value& (0x500) ?1: ( (ldo_value& (0xa00) ?2:3)));


    if (work_vol == TB_SIM_WORKING_VOLT_1_8V)
    {
        SCI_DisableIRQ();

        ldo_value = REG32 (GR_LDO_CTL2);


        ldo_value |=  BIT_11;
        ldo_value &= ~BIT_10;
        ldo_value |=  BIT_9;
        ldo_value &= ~BIT_8;

        REG32 (GR_LDO_CTL2) = ldo_value;

        SCI_RestoreIRQ();
    }
    else if (work_vol == TB_SIM_WORKING_VOLT_3V)
    {
        SCI_DisableIRQ();

        ldo_value = REG32 (GR_LDO_CTL2);

        ldo_value |=  BIT_10;
        ldo_value &= ~BIT_11;
        ldo_value |=  BIT_8;
        ldo_value &= ~BIT_9;

        REG32 ( (GR_LDO_CTL2)) = ldo_value;

        SCI_RestoreIRQ();
    }
    else
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[SetSIMPower] do not support 5v"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7137_112_2_17_23_6_44_1279,(uint8*)"d", TRACE_CARD_ID);
    }

    s_sim_working_volt = work_vol;
    SCI_TRACE_LOW ("USIMDRV:[%d]:[SetSIMPower] ldo_ctl2 %d(1[1.8v] 2[3v] 3[?])", TRACE_CARD_ID, ldo_value& (0x500) ?1: ( (ldo_value& (0xa00) ?2:3)));

#else
    /*+CR200528 Eddie.Wang*/
    LDO_VOLT_LEVEL_E ldo_lvl;
    LDO_ID_E ldo_id;
    if (USIMDRV_CARD_ONE == card_id)
    {
      ldo_id = LDO_LDO_SIM0;
    }
    else
    {
      ldo_id = LDO_LDO_SIM1;
    }
    ldo_lvl = LDO_GetVoltLevel (ldo_id);
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SetSIMPower] Before setting LDO Level[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7155_112_2_17_23_6_44_1280,(uint8*)"dd", TRACE_CARD_ID, ldo_lvl);

    if (work_vol == TB_SIM_WORKING_VOLT_1_8V)
    {
        LDO_SetVoltLevel (ldo_id, LDO_VOLT_LEVEL0);
    }
    else if (work_vol == TB_SIM_WORKING_VOLT_3V)
    {
        LDO_SetVoltLevel (ldo_id, LDO_VOLT_LEVEL2);
    }
    else
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[SetSIMPower] do not support 5v"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7167_112_2_17_23_6_44_1281,(uint8*)"d", TRACE_CARD_ID);
    }

    s_sim_working_volt[card_id] = work_vol;

    ldo_lvl = LDO_GetVoltLevel (ldo_id);
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[SetSIMPower] After seting LDO Level[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7173_112_2_17_23_6_44_1282,(uint8*)"dd", TRACE_CARD_ID, ldo_lvl);
    /*-CR200528 Eddie.Wang*/
#endif /* PLATFORM_SC8800G*/
}
#endif

/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Arthor:bin.li
//    Note:
***********************************************************************/
static void Sim1ResetAfterLdoValid (
    uint32 lparam   //the function's input parameter
)
{
    uint32 i;

    //SCI_TRACE_LOW:"USIMDRV:[0]:[Sim1ResetAfterLdoValid] Tick[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7191_112_2_17_23_6_44_1283,(uint8*)"d", SCI_GetTickCount());

    // 打开中断，开始启动s_sim_atr_timer
    s_sim_register_map[USIMDRV_CARD_ONE]->ie = SIM_DEFAULT_IE;

    s_sim_register_map[USIMDRV_CARD_ONE]->ctl0 |= SIMCTL0_B_TX_EN;//enable the tx_en before send the instruction

#ifdef PLATFORM_SC8800G
#else
    /* set  4.7k pull-up for SIM DATA pads */
    CHIP_REG_OR (PIN_SIMDA0_REG, PIN_PU_EN);
    CHIP_REG_OR (GR_PCTL, BIT_11);

    for (i = 0; i < 100; i++)
    {
    }

#endif

    s_sim_register_map[USIMDRV_CARD_ONE]->ctl0 |= SIMCTL0_B_POWER_EN;
    /*+CR200528 Eddie.Wang*/
#ifdef PLATFORM_SC8800G
    LDO_TurnOnLDO (LDO_LDO_SIM0);
#endif
    /*-CR200528 Eddie.Wang*/

    SCI_DISABLE_IRQ;

    if (SCI_IsTimerActive (s_sim_atr_timer[USIMDRV_CARD_ONE]))
    {
        SCI_DeactiveTimer (s_sim_atr_timer[USIMDRV_CARD_ONE]);
    }

    USIMDRV_CHANGE_TIMER(USIMDRV_CARD_ONE,s_sim_atr_timer[USIMDRV_CARD_ONE],DriverTimerExpired,SIM_ATR_TIMER_VALUE);
    SCI_ActiveTimer (s_sim_atr_timer[USIMDRV_CARD_ONE]);
    SCI_RESTORE_IRQ;

    for (i = 0; i < 100; i++)
    {
    }

    s_sim_register_map[USIMDRV_CARD_ONE]->ctl0 &= ~SIMCTL0_B_TX_DATA_OUT_LOW;

    for (i = 0; i < 100; i++)
    {
    }

    //enactive the SIM card
    s_sim_register_map[USIMDRV_CARD_ONE]->ctl0 |= SIMCTL0_B_EN_ACTIVE;
#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[USIMDRV_CARD_ONE])
    {
        USIMDRV_DisableSIMRXInt(USIMDRV_CARD_ONE);
        USIMDRV_DisableSIM_TxParityInt(USIMDRV_CARD_ONE);
        USIMDRV_DisableSIMTXEmptyInt(USIMDRV_CARD_ONE);
        s_sim_register_map[USIMDRV_CARD_ONE]->ctl0 |= (SIMCTL0_B_TX_EN|SIMCTL0_B_RX_EN);
        s_sim_register_map[USIMDRV_CARD_ONE]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[USIMDRV_CARD_ONE], 1);
        s_sim_register_map[USIMDRV_CARD_ONE]->ctl0 |= (SIMCTL0_B_TX_FIFO_RST | SIMCTL0_B_RX_FIFO_RST);
        USIMDRV_DMA_TX_Disable(USIMDRV_CARD_ONE);
        USIMDRV_DMA_RX_Disable(USIMDRV_CARD_ONE);
        sim_rx_dma_cfg[USIMDRV_CARD_ONE].total_length = 1;
        sim_rx_dma_cfg[USIMDRV_CARD_ONE].block_length = 1;
        USIMDRV_DMA_RX_Cfg (USIMDRV_CARD_ONE,&sim_rx_dma_cfg[USIMDRV_CARD_ONE]);
        USIMDRV_DMA_RX_Enable(USIMDRV_CARD_ONE);
    }
    else
    {
        //enable the rx to receive the tx data
        s_sim_register_map[USIMDRV_CARD_ONE]->ie |= SIMIE_B_RX_FULL;
    }

#else
    //enable the rx to receive the tx data
    if (!s_is_sim_out[USIMDRV_CARD_ONE])   /*NEWMS00106599 E*/
        s_sim_register_map[USIMDRV_CARD_ONE]->ie |= SIMIE_B_RX_FULL;
#endif

    s_card_resetting[USIMDRV_CARD_ONE] = FALSE;
}
/**********************************************************************
//    Description:
//      go on reset the sim card after the ldo is valid
//    Global resource dependence :
//    Arthor:bin.li
//    Note:
***********************************************************************/
static void Sim2ResetAfterLdoValid (
    uint32 lparam   //the function's input parameter
)
{
    uint32 i;

    //SCI_TRACE_LOW:"USIMDRV:[1]:[Sim2ResetAfterLdoValid] Tick[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7283_112_2_17_23_6_45_1284,(uint8*)"d", SCI_GetTickCount());

    // 打开中断，开始启动s_sim_atr_timer
    s_sim_register_map[USIMDRV_CARD_TWO]->ie = SIM_DEFAULT_IE;

    s_sim_register_map[USIMDRV_CARD_TWO]->ctl0 |= SIMCTL0_B_TX_EN;//enable the tx_en before send the instruction

#ifdef PLATFORM_SC8800G
#else
    /* set  4.7k pull-up for SIM DATA pads */
    CHIP_REG_OR (PIN_SIMDA0_REG, PIN_PU_EN);
    CHIP_REG_OR (GR_PCTL, BIT_11);

    for (i = 0; i < 100; i++)
    {
    }

#endif

    s_sim_register_map[USIMDRV_CARD_TWO]->ctl0 |= SIMCTL0_B_POWER_EN;
    /*+CR200528 Eddie.Wang*/
#ifdef PLATFORM_SC8800G
    LDO_TurnOnLDO (LDO_LDO_SIM1);
#endif
    /*-CR200528 Eddie.Wang*/

    SCI_DISABLE_IRQ;

    if (SCI_IsTimerActive (s_sim_atr_timer[USIMDRV_CARD_TWO]))
    {
        SCI_DeactiveTimer (s_sim_atr_timer[USIMDRV_CARD_TWO]);
    }

    USIMDRV_CHANGE_TIMER(USIMDRV_CARD_TWO,s_sim_atr_timer[USIMDRV_CARD_TWO],DriverTimerExpired,SIM_ATR_TIMER_VALUE);
    SCI_ActiveTimer (s_sim_atr_timer[USIMDRV_CARD_TWO]);
    SCI_RESTORE_IRQ;

    for (i = 0; i < 100; i++)
    {
    }

    s_sim_register_map[USIMDRV_CARD_TWO]->ctl0 &= ~SIMCTL0_B_TX_DATA_OUT_LOW;

    for (i = 0; i < 100; i++)
    {
    }

    //enactive the SIM card
    s_sim_register_map[USIMDRV_CARD_TWO]->ctl0 |= SIMCTL0_B_EN_ACTIVE;
#ifdef USIM_DMA_SUPPORT

    if (sim_dma_support[USIMDRV_CARD_TWO])
    {
        USIMDRV_DisableSIMRXInt(USIMDRV_CARD_TWO);
        USIMDRV_DisableSIM_TxParityInt(USIMDRV_CARD_TWO);
        USIMDRV_DisableSIMTXEmptyInt(USIMDRV_CARD_TWO);
        s_sim_register_map[USIMDRV_CARD_TWO]->ctl0 |= (SIMCTL0_B_TX_EN|SIMCTL0_B_RX_EN);
        s_sim_register_map[USIMDRV_CARD_TWO]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[USIMDRV_CARD_TWO], 1);
        s_sim_register_map[USIMDRV_CARD_TWO]->ctl0 |= (SIMCTL0_B_TX_FIFO_RST | SIMCTL0_B_RX_FIFO_RST);
        USIMDRV_DMA_TX_Disable(USIMDRV_CARD_TWO);
        USIMDRV_DMA_RX_Disable(USIMDRV_CARD_TWO);
        sim_rx_dma_cfg[USIMDRV_CARD_TWO].total_length = 1;
        sim_rx_dma_cfg[USIMDRV_CARD_TWO].block_length = 1;
        USIMDRV_DMA_RX_Cfg (USIMDRV_CARD_TWO,&sim_rx_dma_cfg[USIMDRV_CARD_TWO]);
        USIMDRV_DMA_RX_Enable(USIMDRV_CARD_TWO);
    }
    else
    {
        //enable the rx to receive the tx data
        s_sim_register_map[USIMDRV_CARD_TWO]->ie |= SIMIE_B_RX_FULL;
    }

#else
    //enable the rx to receive the tx data
    if (!s_is_sim_out[USIMDRV_CARD_TWO])   /*NEWMS00106599 E*/
        s_sim_register_map[USIMDRV_CARD_TWO]->ie |= SIMIE_B_RX_FULL;
#endif

    s_card_resetting[USIMDRV_CARD_TWO] = FALSE;
}

/**********************************************************************
//    Description:
//    handle S-Block request from usim
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static void HandleUICCSBlock (int32 card_id,TB_SIM_RX_BLOCK_T *rx_block)
{
    //uint8 i;
    SEND_SBLOCK_REASON_E sblock_reason = SEND_SBLOCK_FOR_SBLOCK_REQUEST;

    USIM_ASSERT (PNULL != rx_block);/*assert verified*/


    if (TB_SIM_S_BLOCK_EXT_BWT_REQ >= rx_block->base_info.s_block.block_type)
    {
        sblock_reason = SEND_SBLOCK_FOR_SBLOCK_RESPONSE;
    }


    switch (rx_block->base_info.s_block.block_type)
    {
        case TB_SIM_S_BLOCK_RESYN_REQ:
            {
                break;
            }
        case TB_SIM_S_BLOCK_INFO_FIELD_REQ:
            {
                s_atr_data_buf[card_id].atr_data.IFSC = rx_block->ext_byte[TB_SIM_BLOCK_PRO_LEN+1];
                break;
            }
        case TB_SIM_S_BLOCK_ABORTION_REQ:
            {
                break;
            }
        case TB_SIM_S_BLOCK_EXT_BWT_REQ:
            {
                /*  if the req is ok get wtx req, bwt should be reset, ref to Scenario 14 7816-3 p25 */
                s_atr_data_buf[card_id].BWT *= rx_block->ext_byte[TB_SIM_BLOCK_PRO_LEN+1];
                break;
            }
        case TB_SIM_S_BLOCK_ERROR_VPP:
            {
                break;
            }
        case TB_SIM_S_BLOCK_RESYN_RSP:
            {
#if 0
                HandleResyncRsp();
                TransmitInInt_T1();
#endif
                NotifyExceptionError (card_id, TB_SIMEXP_CARD_UNRESPONSE);
                SIM_PowerOffSIMEx(card_id);
                break;
            }
        default:
            {
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[HandleUICCSBlock] wrong block type 0x%x?"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7422_112_2_17_23_6_45_1285,(uint8*)"dd", TRACE_CARD_ID, rx_block->base_info.s_block.block_type);
                ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
                break;
            }
    }


    if (TB_SIM_S_BLOCK_EXT_BWT_REQ >= rx_block->base_info.s_block.block_type)
    {
        /*  the info data is filled by copying the req to the tx buf*/
#ifdef USIM_DMA_SUPPORT
        if (sim_dma_support[card_id])
        {
            DMA_SendSBLOCK (card_id, sblock_reason,rx_block->base_info.s_block.block_type,0);
        }
        else
        {
            SendSBLOCK (card_id, sblock_reason,rx_block->base_info.s_block.block_type,0);
        }

#else
        SendSBLOCK (card_id, sblock_reason,rx_block->base_info.s_block.block_type,0);
#endif
    }

    return;
}

/**********************************************************************
//    Description:
//    reset the ns&nr and the buffer we need send after reasync is done
//    Global resource dependence :
//    Author:bo.chen
//    Note:
***********************************************************************/
static void HandleResyncRsp (
    int32 card_id
)
{
    uint16 check_point = TB_SIM_BLOCK_PRO_PCB;

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[HandleResyncRsp]  handle resync rsp nr[0x%x]ns[0x%x]tx_len[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7463_112_2_17_23_6_45_1286,(uint8*)"dddd", TRACE_CARD_ID, s_pcb_nr[card_id], s_pcb_ns[card_id], s_sim_tx_buf[card_id].tx_data_len);

    s_sim_block_resent_count[card_id] = 0;
    s_sim_tx_buf[card_id].cur_pos = 0;
    /*  reset the nr and ns */
    s_pcb_nr[card_id] = 0;
    s_pcb_ns[card_id] = 0;

    s_sim_tx_block[card_id].base_info.i_block.ns = 0;

    /*  check the ns in s_sim_tx_buf    */
    while (check_point < s_sim_tx_buf[card_id].tx_data_len)
    {
        s_sim_tx_buf[card_id].buf[check_point] &= ~0x40;
        s_sim_tx_buf[card_id].buf[check_point] |= ( (s_pcb_ns[card_id] & 0x1) << USIM_CONST_NUM6);

        check_point += s_atr_data_buf[card_id].atr_data.IFSC + 4;
        s_pcb_ns[card_id] = (s_pcb_ns[card_id] + 1) % 2;
    }

    //regulate the tx block poiter,then to send the next block
    s_sim_tx_block[card_id].block_begin_pos = 0;

    if (s_sim_tx_buf[card_id].tx_data_len > s_atr_data_buf[card_id].atr_data.IFSC)
    {
        s_sim_tx_block[card_id].block_end_pos = s_atr_data_buf[card_id].atr_data.IFSC+4;
    }
    else
    {
        s_sim_tx_block[card_id].block_end_pos = s_sim_tx_buf[card_id].tx_data_len;
    }


}
 /* +CR218847 remove real nw flag */
/**********************************************************************
//    Description:
//
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
uint8 SIM_GetUiccTprotocol (
    int32 card_id
)
{
    return s_atr_data_buf[card_id].atr_data.t_protocol;
}
/* -CR218847 remove real nw flag */

#ifdef USIM_DMA_SUPPORT
/* +binli add dma test */
/**********************************************************************
//    Description:
//      SIM DMA TX channel enable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_TX_Enable (int32 card_id)
{
    DMA_CAHCECONFIG_T cacheConfig[1];
    uint32 channel_id = DMA_SIM0_TX;
    s_sim_register_map[card_id]->she |= BIT_9;//enable sim tx fifo control by DMA

    cacheConfig[0].bufferAddr = (uint32) sim_tx_dma_cfg[card_id].src_address;
    cacheConfig[0].bufferSize = sim_tx_dma_cfg[card_id].total_length;
    cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    DMA_HAL_ChnEnable_Ex (channel_id, SCI_TRUE, cacheConfig, 1);

//    DMA_HAL_ChnEnable (DMA_SIM0_TX, TRUE);
}
/**********************************************************************
//    Description:
//      SIM DMA RX channel enable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_RX_Enable (int32 card_id)
{
    DMA_CAHCECONFIG_T cacheConfig[1];
    uint32 channel_id = DMA_SIM0_TX;
    s_sim_register_map->she |= BIT_8;//enable sim rx fifo control by DMA

//sim_rx_dma_cfg.total_size
    cacheConfig[0].bufferAddr = (uint32) sim_rx_dma_cfg[card_id].des_address;
    cacheConfig[0].bufferSize = sim_rx_dma_cfg[card_id].total_length;
    cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    DMA_HAL_ChnEnable_Ex (channel_id, SCI_TRUE, cacheConfig, 1);
//    MMU_InvalideDCACHE();
//    DMA_HAL_ChnEnable (DMA_SIM0_RX, TRUE);
}
/**********************************************************************
//    Description:
//      SIM DMA TX channel disable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_TX_Disable (int32 card_id)
{
    uint32 channel_id = DMA_SIM0_TX;
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    DMA_HAL_ChnEnable (channel_id, FALSE);
    s_sim_register_map[card_id]->she &= ~BIT_9;//disable sim tx fifo control by DMA
}
/**********************************************************************
//    Description:
//      SIM DMA RX channel disable
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_RX_Disable (int32 card_id)
{
    uint32 channel_id = DMA_SIM0_TX;
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    DMA_HAL_ChnEnable (channel_id, FALSE);
    s_sim_register_map[card_id]->she &= ~BIT_8;//disable sim rx fifo control by DMA
}
/**********************************************************************
//    Description:
//      Disable SIM Tx Empty Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIMTXEmptyInt (int32 card_id)
{
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_TX_EMPTY;
}
/**********************************************************************
//    Description:
//      Enable SIM Tx Empty Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_EnableSIMTXEmptyInt (int32 card_id)
{
    s_sim_register_map[card_id]->ie |= SIMIE_B_TX_EMPTY;
}
/**********************************************************************
//    Description:
//      Enable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_EnableSIMRXInt (int32 card_id)
{
    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_TOUT;
}
/**********************************************************************
//    Description:
//      Disable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIMRXInt (int32 card_id)
{
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_FULL;
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_TOUT;
}

/**********************************************************************
//    Description:
//      Disable SIM Tx Parity Err Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DisableSIM_TxParityInt (int32 card_id)
{
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_TX_PARITY_ERR;
}

/**********************************************************************
//    Description:
//      Enable SIM Tx Parity Err Int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_EnableSIM_TxParityInt(int32 card_id)
{
    s_sim_register_map[card_id]->ie |= SIMIE_B_TX_PARITY_ERR;
}

/**********************************************************************
//    Description:SIM DMA TX channel config
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void USIMDRV_DMA_TX_Cfg (int32 card_id,DMA_CHNCFG_PTR tx_dma_cfg)
{
    uint32 channel_id = DMA_SIM0_TX;
    tx_dma_cfg->block_length = 1;
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    DMA_HAL_ChnConfig (channel_id, tx_dma_cfg);

    s_sim_register_map[card_id]->ctl1 = \
        SET_TX_INT_MARK (s_sim_register_map[card_id], sim_tx_dma_cfg[card_id].block_length);
}

/**********************************************************************
//    Description: SIM DMA RX channel config
//    Global resource dependence :
//    Author: Eddie.Wang
//    Note:
***********************************************************************/
static void USIMDRV_DMA_RX_Cfg (int32 card_id,DMA_CHNCFG_PTR rx_dma_cfg)
{
    uint32 channel_id = DMA_SIM0_TX;
    if (rx_dma_cfg->total_length <9)
    {
        rx_dma_cfg->block_length = rx_dma_cfg->total_length;
    }
    else
    {
        int8 i=0;

        for (i=8; i>0; i--)
        {
            if (0 == rx_dma_cfg->total_length % i)
            {
                rx_dma_cfg->block_length = (uint32) i;
                break;
            }
        }
    }

    //    SCI_TRACE_LOW("Eddie.Wang DMA TEST RX CFG:: burst_size=%d, 0x%x[%d]", rx_dma_cfg->burst_size, sim_rx_dma_cfg.des_address, s_sim_rx_buf.cur_pos);
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    DMA_HAL_ChnConfig (channel_id, rx_dma_cfg);

    s_sim_register_map[card_id]->ctl1 = \
        SET_RX_INT_MARK (s_sim_register_map[card_id], sim_rx_dma_cfg[card_id].block_length);
}

/**********************************************************************
//    Description: Send a S-Block.In following cases, a S-block should be sent
//    1. When want to sent an S(...request)
//    2. When an S(...request) has been sent and either a BWT time-out occurs or the
//       received response is not an S(...response),S(...request) should be resent.
//    3. When received an S(...request),an (...response)shoud be send.
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendSBLOCK (
    int32 card_id,
    SEND_SBLOCK_REASON_E send_reason,
    TB_SIM_S_BLOCK_TYPE_E sblock_type,
    uint8 info_data)
{
    uint32 channel_id = DMA_SIM0_TX;
    USIM_ASSERT (send_reason < SEND_SBLOCK_REASON_NUM);/*assert verified*/
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    if (send_reason == SEND_SBLOCK_FOR_SBLOCK_ERROR)
    {
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_NAD] = 0;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_PCB] = sblock_type|SIM_DEFAULT_PCB_SBLOCK;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_LEN] = 1;
        s_sim_tx_block[card_id].cur_ext_byte_count = 3;

        if (sblock_type == TB_SIM_S_BLOCK_INFO_FIELD_REQ)
        {
            s_sim_tx_block[card_id].cur_ext_byte_count++;
            s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = info_data;
        }

        s_sim_tx_block[card_id].cur_ext_byte_count++;
        s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = GetEDC (s_sim_tx_block[card_id].ext_byte,s_sim_tx_block[card_id].cur_ext_byte_count - 1);

        //record the tx block information
        s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_S_BLOCK;
        s_sim_tx_block[card_id].base_info.s_block.is_response = FALSE;
        s_sim_tx_block[card_id].base_info.s_block.block_type = sblock_type;
    }
    else if (send_reason == SEND_SBLOCK_FOR_SBLOCK_RESPONSE)
    {
        //send a s(...response)
        /*  bo.chen 20070616 all s reponse should be check all S(response)
            bytes are sent to the usim */
        memcpy (s_sim_tx_block[card_id].ext_byte,s_sim_rx_block.ext_byte,s_sim_rx_block.cur_ext_byte_count);
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_PCB] |= (1 << USIM_CONST_NUM5) ;
        s_sim_tx_block[card_id].ext_byte[s_sim_rx_block.cur_ext_byte_count] = GetEDC (s_sim_tx_block[card_id].ext_byte,s_sim_rx_block.cur_ext_byte_count);

        //record the tx block information
        s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_S_BLOCK;
        s_sim_tx_block[card_id].base_info.s_block.is_response = TRUE;
        s_sim_tx_block[card_id].base_info.s_block.block_type = s_sim_rx_block[card_id].base_info.s_block.block_type;

        s_sim_tx_block[card_id].cur_ext_byte_count = s_sim_rx_block[card_id].cur_ext_byte_count;
    }
    /*  send ifs req to sim 20070614 bo.chen    */
    else if (send_reason == SEND_SBLOCK_FOR_SBLOCK_REQUEST)
    {
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_NAD] = 0;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_PCB] = sblock_type|SIM_DEFAULT_PCB_SBLOCK;
        s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_LEN] = 0;
        s_sim_tx_block[card_id].cur_ext_byte_count = 2;

        if ( (sblock_type == TB_SIM_S_BLOCK_INFO_FIELD_REQ) || \
                (TB_SIM_S_BLOCK_EXT_BWT_REQ == sblock_type))
        {
            s_sim_tx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_LEN] = 1;
            s_sim_tx_block[card_id].cur_ext_byte_count++;
            s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = info_data;
        }

        s_sim_tx_block[card_id].cur_ext_byte_count++;
        s_sim_tx_block[card_id].ext_byte[s_sim_tx_block[card_id].cur_ext_byte_count] = GetEDC (s_sim_tx_block[card_id].ext_byte,s_sim_tx_block[card_id].cur_ext_byte_count);

        //record the tx block information
        s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_S_BLOCK;
        s_sim_tx_block[card_id].base_info.s_block.is_response = FALSE;
        s_sim_tx_block[card_id].base_info.s_block.block_type = sblock_type;
    }

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_SendSBLOCK] Tick[0x%x],Reason[0x%x],Ext_Cnt[0x%x],Ext_data[0x%x,0x%x,0x%x,0x%x,0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7803_112_2_17_23_6_46_1287,(uint8*)"ddddddddd", TRACE_CARD_ID, SCI_GetTickCount(), send_reason, s_sim_tx_block[card_id].cur_ext_byte_count, s_sim_tx_block[card_id].ext_byte[0], s_sim_tx_block[card_id].ext_byte[1], s_sim_tx_block[card_id].ext_byte[2], s_sim_tx_block[card_id].ext_byte[3], s_sim_tx_block[card_id].ext_byte[4] );

    //Set DMA TX CFG and send R-Block
    USIMDRV_DMA_TX_Disable(card_id);
    sim_tx_dma_cfg[card_id].total_length = (uint32) (s_sim_tx_block[card_id].cur_ext_byte_count + 1);
    sim_tx_dma_cfg[card_id].block_length = (uint32) (s_sim_tx_block[card_id].cur_ext_byte_count + 1);
    sim_tx_dma_cfg[card_id].src_address  = (uint32) s_sim_tx_block[card_id].ext_byte;
    DMA_HAL_ChnConfig (channel_id, &sim_tx_dma_cfg[card_id]);
    USIMDRV_DMA_TX_Enable(card_id);

    //active bwt timer
    if (SCI_IsTimerActive (s_sim_bwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_bwt_timer[card_id]);
    }

    /*  temp test   */
    if (0 == s_atr_data_buf[card_id].BWT)
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_SendSBLOCK] ATR.BWT[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7822_112_2_17_23_6_46_1288,(uint8*)"dd", TRACE_CARD_ID, s_atr_data_buf[card_id].BWT);
        s_atr_data_buf[card_id].BWT = 1000;
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DMATimerExpired,s_atr_data_buf[card_id].BWT);
    SCI_ActiveTimer (s_sim_bwt_timer[card_id]);

}

/**********************************************************************
 //    Description: Send a r-block.In following cases, a r-block should be sent:
//    1. When an I-block has been sent and a BWT time out occurs or an
//       invalid block has been received.
//    2. When an R-Block was sent and an invalid block is received or BWT time-out
//    3. When an S(...response) has been sent and an invalid block is received or
//       BWT time-out.
//    4. When received an I(1) block
//    send_reason: input,indicates the four case
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendRBlock (
    int32 card_id,
    SEND_RBLOCK_REASON_E send_reason,
    TB_SIM_R_BLOCK_TYPE_E   error_code
)
{
    uint8 tempbuf[TB_SIM_BLOCK_EXT_BYTE_NUM];
    uint32 channel_id = DMA_SIM0_TX;

    USIM_ASSERT (send_reason < SEND_RBLOCK_REASON_NUM);/*assert verified*/
    USIM_ASSERT (error_code < TB_SIM_R_BLOCK_RFU);/*assert verified*/
    if (USIMDRV_CARD_TWO == card_id)
    {
        channel_id = DMA_SIM1_TX;
    }
    //record the tx block information
    s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_R_BLOCK;
    s_sim_tx_block[card_id].base_info.r_block.block_type = error_code;


    //fill the r-block
    tempbuf[TB_SIM_BLOCK_PRO_NAD] = 0;
    tempbuf[TB_SIM_BLOCK_PRO_PCB] = SIM_DEFAULT_PCB_RBLOCK | error_code;

    //decide the ns in the pcb byte
    if (send_reason == SEND_RBLOCK_FOR_IBLOCK_ACK)
    {
        /*  bo.chen 20070615 ns+1 % 2   */
        tempbuf[TB_SIM_BLOCK_PRO_PCB] |= \
                                         ( (s_pcb_nr & 0x01) << USIM_CONST_NUM4);
        // s_sim_rx_block.base_info.r_block.nr = (s_sim_rx_block.base_info.i_block.ns+1)%2;
    }
    else if (send_reason == SEND_RBLOCK_FOR_IBLOCK_ERROR)
    {
        /*  need retransfer for i block */
        tempbuf[TB_SIM_BLOCK_PRO_PCB] |= \
                                         ( (s_pcb_nr & 0x01) << USIM_CONST_NUM4);
        // s_sim_rx_block.base_info.r_block.nr = s_sim_tx_block.base_info.i_block.ns;
    }
    else if (send_reason == SEND_RBLOCK_FOR_RBLOCK_ERROR)
    {
        /*  bo.chen if the r-block is sent and timer out
            we need resend the r-block.  */
        tempbuf[TB_SIM_BLOCK_PRO_PCB] |= \
                                         ( (s_pcb_nr & 0x01) << USIM_CONST_NUM4);
        // s_sim_rx_block.base_info.r_block.nr = s_sim_tx_block.base_info.i_block.ns;
    }

    tempbuf[TB_SIM_BLOCK_PRO_LEN] = 0;
    tempbuf[TB_SIM_BLOCK_EPI_EDC] = GetEDC (tempbuf,TB_SIM_BLOCK_PRO_BYTE_NUM);


    //Set DMA TX CFG and send R-Block
    USIMDRV_DMA_TX_Disable(card_id);
    sim_tx_dma_cfg[card_id].total_length = TB_SIM_BLOCK_EXT_BYTE_NUM;
    sim_tx_dma_cfg[card_id].block_length = TB_SIM_BLOCK_EXT_BYTE_NUM;
    sim_tx_dma_cfg[card_id].src_address  = (uint32) tempbuf;
    DMA_HAL_ChnConfig (channel_id, &sim_tx_dma_cfg[card_id]);
    USIMDRV_DMA_TX_Enable(card_id);

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_SendRBlock] Tick[0x%x], Buff[0x%x,0x%x,0x%x,0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7905_112_2_17_23_6_46_1289,(uint8*)"dddddd", TRACE_CARD_ID, SCI_GetTickCount(), tempbuf[0], tempbuf[1], tempbuf[2], tempbuf[3]);

    //active bwt timer
    if (SCI_IsTimerActive (s_sim_bwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_bwt_timer[card_id]);
    }

    /*  temp test   */
    if (0 == s_atr_data_buf[card_id].BWT)
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_SendRBlock] ATR.BWT[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7916_112_2_17_23_6_46_1290,(uint8*)"dd", TRACE_CARD_ID, s_atr_data_buf[card_id].BWT);
        s_atr_data_buf[card_id].BWT = 1000;
    }

    //active bwt timer
    USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DMATimerExpired,s_atr_data_buf[card_id].BWT);
    SCI_ActiveTimer (s_sim_bwt_timer[card_id]);
}

/**********************************************************************
//    Description: Check if the Received block is valid in DMA mode
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static BOOLEAN DMA_CheckBlockValid (int32 card_id,TB_SIM_TX_BLOCK_T *tx_block,TB_SIM_RX_BLOCK_T *rx_block)
{
    BLOCK_VALIDITY_E block_validity;
    BLOCK_TYPE_E tx_block_type;
    BLOCK_TYPE_E rx_block_type;
    uint8 edc,edc1;
    uint16 offset;
    BOOLEAN result = TRUE;

    USIM_ASSERT (tx_block != NULL);/*assert verified*/
    USIM_ASSERT (rx_block != NULL);/*assert verified*/

    //only when receive response status and the edc byte have been received,check block validity
    if (TB_SIM_RECEIVING_RESPONSE != s_sim_status[card_id])
    {
        return TRUE;
    }
    else if (rx_block->have_edc != TRUE)
    {
        return FALSE;
    }

    /*  wait cwt timerout   */
    if (s_rx_parity_recv[card_id])
    {
        /*  if rx parity happen donot check this block for the invalid
            parity  */
        return FALSE;
    }

    /*  stop the wwt timer because we receive the end of block  */
    if (SCI_IsTimerActive (s_sim_cwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_cwt_timer[card_id]);
    }


    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_CheckBlockValid] Tick[0x%x], CWT_Timer[0x%x], Status[0x%x], Tx_Block_Type[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_7969_112_2_17_23_6_46_1291,(uint8*)"ddddd", TRACE_CARD_ID, SCI_GetTickCount(), s_cwt_timer_value[card_id], rx_block->base_info.valid_block, tx_block->base_info.s_block.block_type);


    if (! (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_NAD))
    {
        ErrorHandler (card_id, T1_ERROR_CODE_NAD_ERROR);
        return FALSE;
    }
    else if (! (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_PCB))
    {
        ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
        return FALSE;
    }
    else if (! (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_LEN))
    {
        ErrorHandler (card_id, T1_ERROR_CODE_LEN_ERROR);
        return FALSE;
    }

    /*  20070716 check pcb status
        if req is sent, rsp is expected
        */
    if (TB_SIM_BLOCK_S_BLOCK == tx_block->base_info.block_type)
    {
        if (! (tx_block->base_info.s_block.is_response))
        {
            if (TB_SIM_BLOCK_S_BLOCK == rx_block->base_info.block_type)
            {
                if ( (rx_block->base_info.s_block.block_type) == \
                        ( (tx_block->base_info.s_block.block_type) | (1 << USIM_CONST_NUM5)))
                {
                    /*  the rsp is not the correctly for that req*/
                    ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
                    return FALSE;
                }
            }
            else
            {
                ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
                return FALSE;
            }
        }
    }

    //check edc
    edc = GetEDC (rx_block->ext_byte,rx_block->cur_ext_byte_count);

    if ( (s_sim_rx_block[card_id].info_data_len != 0) && \
            (TB_SIM_BLOCK_I_BLOCK == rx_block->base_info.block_type))   //I-block or R-Block which has info data
    {
        offset = s_sim_rx_block_buf[card_id].cur_pos - s_sim_rx_block[card_id].info_data_len;
        edc1 = GetEDC (s_sim_rx_block_buf[card_id].buf + offset,s_sim_rx_block[card_id].info_data_len);
        edc ^= edc1;
    }

    if (rx_block->ext_byte[rx_block->cur_ext_byte_count] != edc)
    {
        ErrorHandler (card_id,T1_ERROR_CODE_EDC_ERROR);
        return FALSE;
    }

    //check block validity
    /*  bo.chen 20070621 需要判断这个block是否n(r)正确  */
    tx_block_type = GetBlockType (tx_block->base_info);
    rx_block_type = GetBlockType (rx_block->base_info);
    block_validity = s_sim_check_block[tx_block_type][rx_block_type];

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_CheckBlockValid] tx_type[0x%x], rx_type[0x%x],block_validity[0x%x],Tick[0x%x], Info_Len[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8037_112_2_17_23_6_46_1292,(uint8*)"dddddd", TRACE_CARD_ID, tx_block_type,rx_block_type,block_validity, SCI_GetTickCount(), rx_block->info_data_len);


    if (TB_SIM_BLOCK_I_BLOCK == rx_block->base_info.block_type)
    {
        if ( (BLOCK_VALID == block_validity) || \
                (BLOCK_SEND_R == block_validity))
        {
            s_sim_block_resent_count[card_id] = 0;
            /*  this I-block is valid   */
            SCI_MEMCPY (&s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos], \
                        &s_sim_rx_block_buf[card_id].buf[0], s_sim_rx_block_buf[card_id].cur_pos);
            s_sim_rx_buf[card_id].cur_pos += s_sim_rx_block_buf[card_id].cur_pos;
            /*  add pcb nr  */
            s_pcb_nr = (rx_block->base_info.i_block.ns + 1) % 2;
        }
    }


    if (BLOCK_TYPE_R_OK == rx_block_type)
    {
        /*  if we receive R(ok) check the last tx I-block*/
        if (tx_block->base_info.i_block.chain_M)
        {
            s_sim_block_resent_count[card_id] = 0;
            block_validity = BLOCK_SEND_NEXT;
        }
    }



    switch (block_validity)
    {
        case BLOCK_VALID:

            if (BLOCK_TYPE_I_ZERO == rx_block_type)
            {
                result = TRUE;
            }
            else if (BLOCK_TYPE_I_ONE == rx_block_type)
            {
                result = FALSE;
            }
            else
            {
                result = FALSE;
            }

            break;
        case BLOCK_SEND_NEXT:
            //regulate the tx block poiter,then to send the next block
            tx_block->block_begin_pos += s_atr_data_buf[card_id].atr_data.IFSC+4;
            tx_block->block_end_pos += s_atr_data_buf[card_id].atr_data.IFSC+4;

            if (tx_block->block_end_pos > s_sim_tx_buf[card_id].tx_data_len)
            {
                tx_block->block_end_pos = s_sim_tx_buf[card_id].tx_data_len;
            }

            //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_CheckBlockValid] Send_Next curpos[0x%x], endpos[0x%x], tx pcb[0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8098_112_2_17_23_6_46_1293,(uint8*)"dddd", TRACE_CARD_ID, tx_block->block_begin_pos, tx_block->block_end_pos, s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos+TB_SIM_BLOCK_PRO_PCB]);

            s_sim_tx_block[card_id].base_info.i_block.ns = \
                                                  GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM6);
            s_sim_tx_block[card_id].base_info.i_block.chain_M = \
                    (BOOLEAN) GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM5);

            s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_I_BLOCK;

            DMA_TX_Callback_T1 (card_id, 0); ////////////////////////////////////////////////////////////////////////////
            result = FALSE;
            break;
            /*  R-Block error for the i-block from usim */
        case BLOCK_RESEND:
            {
                /*  resend the last i-block */
                s_sim_tx_buf[card_id].cur_pos = tx_block->block_begin_pos;
                //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_CheckBlockValid] Resend curpos[0x%x], endpos[0x%x], tx pcb[0x%x]"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8117_112_2_17_23_6_46_1294,(uint8*)"dddd", TRACE_CARD_ID, tx_block->block_begin_pos, tx_block->block_end_pos, s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos+TB_SIM_BLOCK_PRO_PCB]);

                s_sim_tx_block[card_id].base_info.i_block.ns = \
                                                      GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM6);
                s_sim_tx_block[card_id].base_info.i_block.chain_M = \
                        (BOOLEAN) GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM5);
                s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_I_BLOCK;

                DMA_TX_Callback_T1 (card_id,0); //////////////////////////////////////////////////////////////////////////////
                result = FALSE;
                break;
            }
        case BLOCK_SEND_R:

            if (rx_block->base_info.valid_block & TB_SIM_BLOCK_FIELD_VALID_PCB)
            {
                DMA_SendRBlock (card_id, SEND_RBLOCK_FOR_IBLOCK_ACK, TB_SIM_R_BLOCK_ERROR_FREE);
            }
            else
            {
                DMA_SendRBlock (card_id, SEND_RBLOCK_FOR_IBLOCK_ERROR, TB_SIM_R_BLOCK_OTHER_ERROR);
            }

            result = FALSE;
            break;
        case BLOCK_SEND_SREP:
            /*  add handler for S-block */
            HandleUICCSBlock (card_id, rx_block);
            result = FALSE;
            break;
        case BLOCK_INVALID:
            ErrorHandler (card_id, T1_ERROR_CODE_PCB_ERROR);
            return FALSE;

        default:
            result = FALSE;
            break;
    }

    ClearRxBlock(card_id);
    return result;
}
/**********************************************************************
//    Description: Check the Received buf and sent all received data up to Task
//    when all the data from SIM Card has been received
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_CheckReceiveBuf (int32 card_id)
{
    if ( (TB_SIM_RECEIVING_RESPONSE == s_sim_status) &&
            (0 == s_atr_data_buf[card_id].atr_data.t_protocol) &&
            (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_RX_STATUS_WORD_NUM) &&
            (!IsStatusWord (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 2],\
                        s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos - 1])))
    {
        if (s_sim_rx_buf[card_id].cur_pos < s_sim_rx_buf[card_id].rx_expected_len)
        {
            USIMDRV_DMA_RX_Disable(card_id);

            if ( (s_sim_rx_buf[card_id].rx_expected_len - s_sim_rx_buf[card_id].cur_pos) > TB_SIM_DMA_MAX_BUF_SIZE)
            {
                sim_rx_dma_cfg[card_id].total_size = TB_SIM_DMA_MAX_BUF_SIZE;
            }
            else
            {
                sim_rx_dma_cfg[card_id].total_size = (uint32) (s_sim_rx_buf[card_id].rx_expected_len - s_sim_rx_buf[card_id].cur_pos);
            }

            sim_rx_dma_cfg[card_id].des_address = (uint32) (&s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos]);
            USIMDRV_DMA_RX_Cfg (card_id, &sim_rx_dma_cfg[card_id]);
        }
        else
        {
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_CheckReceiveBuf] expected len[%d] <= cur_pos[%d]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8191_112_2_17_23_6_46_1295,(uint8*)"ddd", TRACE_CARD_ID, s_sim_rx_buf[card_id].rx_expected_len, s_sim_rx_buf[card_id].cur_pos);
            USIMDRV_DMA_RX_Disable(card_id);
            sim_rx_dma_cfg[card_id].total_size = 1;
            sim_rx_dma_cfg[card_id].des_address = (uint32) (&s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos]);
            USIMDRV_DMA_RX_Cfg (card_id, &sim_rx_dma_cfg[card_id]);
        }

        //DMA RX timer
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }

        USIMDRV_CHANGE_TIMER(card_id,s_sim_dma_byte_timer[card_id], DMATimerExpired, SIM_DMA_TIMER_VALUE);
        SCI_ActiveTimer (s_sim_dma_byte_timer[card_id]);

        USIMDRV_DMA_RX_Enable(card_id);

        if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
        }
        USIMDRV_CHANGE_TIMER(card_id,s_sim_wwt_timer[card_id],DMATimerExpired,s_wwt_timer_value[card_id]);
        SCI_ActiveTimer (s_sim_wwt_timer[card_id]);
    }
    else
    {
        CheckReceiveBuf(card_id);
    }
}

/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM1_DMA_TX_Callback (uint32 num)
{
    if (0 == s_atr_data_buf[USIMDRV_CARD_ONE].atr_data.t_protocol)
    {
        DMA_TX_Callback_T0 (USIMDRV_CARD_ONE, num);
    }
    else if (1 == s_atr_data_buf[USIMDRV_CARD_ONE].atr_data.t_protocol)
    {
        DMA_TX_Callback_T1 (USIMDRV_CARD_ONE, num);
    }
}
/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX channel when DMA SIM TX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM2_DMA_TX_Callback (uint32 num)
{
    if (0 == s_atr_data_buf[USIMDRV_CARD_TWO].atr_data.t_protocol)
    {
        DMA_TX_Callback_T0 (USIMDRV_CARD_TWO, num);
    }
    else if (1 == s_atr_data_buf[USIMDRV_CARD_TWO].atr_data.t_protocol)
    {
        DMA_TX_Callback_T1 (USIMDRV_CARD_TWO, num);
    }
}
/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX interrupt happen (ONLY for T=0)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_TX_Callback_T0 (int32 card_id,uint32 num)
{
    uint16 tx_transmit_num = 0;

    //Disable DMA TX interrupt to make sure the data transmit wont be crupted by TX interruption in the data transform process
    USIMDRV_DMA_TX_Disable(card_id);
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_TX_Callback_T0] tx_data_len[%d],tx_cur_pos[%d], ACK_Flag[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8265_112_2_17_23_6_47_1296,(uint8*)"dddd", TRACE_CARD_ID,s_sim_tx_buf[card_id].tx_data_len, s_sim_tx_buf[card_id].cur_pos, s_have_ack_byte[card_id]);

    if (s_have_ack_byte[card_id] || (TB_SIM_PROCESSING_PPS == s_sim_status[card_id]))
    {
        //Instruction data need to be sent
        if (s_sim_tx_buf[card_id].tx_data_len > s_sim_tx_buf[card_id].cur_pos)
        {
            tx_transmit_num = s_sim_tx_buf[card_id].tx_data_len - s_sim_tx_buf[card_id].cur_pos;

            //Set DMA TX CFG and send
            if (tx_transmit_num > TB_SIM_DMA_MAX_BUF_SIZE)
            {
                sim_tx_dma_cfg[card_id].total_size = TB_SIM_DMA_MAX_BUF_SIZE;
                tx_transmit_num = TB_SIM_DMA_MAX_BUF_SIZE;
            }
            else
            {
                sim_tx_dma_cfg[card_id].total_size = (uint32) tx_transmit_num;
            }

            sim_tx_dma_cfg[card_id].src_address = (uint32) (&s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos]);

            USIMDRV_DMA_TX_Cfg (card_id, &sim_tx_dma_cfg[card_id]);

            s_sim_tx_buf[card_id].cur_pos += tx_transmit_num;

            if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
            {
                SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
            }
            USIMDRV_CHANGE_TIMER(card_id,s_sim_wwt_timer[card_id], DMATimerExpired,s_wwt_timer_value[card_id]);
            SCI_ActiveTimer (s_sim_wwt_timer[card_id]);

            //Enable the TX for the next data transmission
            USIMDRV_DMA_TX_Enable(card_id);
        }
        else
        {
            //start the wait response timer
            USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
            SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);

            if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
            {
                SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
            }
            USIMDRV_CHANGE_TIMER(card_id,s_sim_wwt_timer[card_id], DMATimerExpired, s_wwt_timer_value[card_id]);
            SCI_ActiveTimer (s_sim_wwt_timer[card_id]);

            //all the command instruciton has been transmit, clear TX buff
            ClearTxBuf(card_id);
        }
    }
    else
    {
        //Set DMA Timer
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }

        USIMDRV_CHANGE_TIMER(card_id,s_sim_dma_byte_timer[card_id], DMATimerExpired, SIM_DMA_TIMER_VALUE);
        SCI_ActiveTimer (s_sim_dma_byte_timer[card_id]);

        //SET and enable RX
        sim_rx_dma_cfg[card_id].dest_address = (uint32) s_sim_rx_buf[card_id].buf;
        sim_rx_dma_cfg[card_id].total_length = 1;
        USIMDRV_DMA_RX_Cfg (card_id, &sim_rx_dma_cfg[card_id]);
        USIMDRV_DMA_RX_Enable(card_id);
    }
}

/**********************************************************************
//    Description: This is the Callback function of DMA SIM TX interrupt happen (ONLY for T=1)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_TX_Callback_T1 (int32 card_id,uint32 num)
{
    uint16 tx_transmit_num;
    BOOLEAN is_data_sent_all = FALSE;

    //Disable DMA TX interrupt to make sure the data transmit wont be crupted by TX interruption in the data transform process
    USIMDRV_DMA_TX_Disable(card_id);

    //If there is still any data to be sent
    if (s_sim_tx_buf[card_id].tx_data_len > s_sim_tx_buf[card_id].cur_pos)
    {
        //Check if some data need to be sent in current block
        if (s_sim_tx_buf[card_id].cur_pos < s_sim_tx_block[card_id].block_end_pos)
        {
            tx_transmit_num = s_sim_tx_block[card_id].block_end_pos - s_sim_tx_buf[card_id].cur_pos;
        }
        else
        {
            return;
        }

        //SET DMA CFG and send the first set data
        if (tx_transmit_num > TB_SIM_DMA_MAX_BUF_SIZE)
        {
            sim_tx_dma_cfg[card_id].total_length = TB_SIM_DMA_MAX_BUF_SIZE;
            sim_tx_dma_cfg[card_id].block_length = TB_SIM_DMA_MAX_BUF_SIZE;
            tx_transmit_num = TB_SIM_DMA_MAX_BUF_SIZE;
        }
        else
        {
            sim_tx_dma_cfg[card_id].total_length = (uint32) tx_transmit_num;
            sim_tx_dma_cfg[card_id].block_length = (uint32) tx_transmit_num;
            is_data_sent_all = TRUE;
        }

        sim_tx_dma_cfg[card_id].src_address = (uint32) (&s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos]);
        if (USIMDRV_CARD_ONE == card_id)
        {
            DMA_HAL_ChnConfig (DMA_SIM0_TX, &sim_tx_dma_cfg[card_id]);
        }
        else
        {
            DMA_HAL_ChnConfig (DMA_SIM1_TX, &sim_tx_dma_cfg[card_id]);
        }
        s_sim_tx_buf[card_id].cur_pos += tx_transmit_num;

        USIMDRV_DMA_TX_Enable(card_id);

        //Start the bwt timer
        if (is_data_sent_all)
        {
            USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DMATimerExpired,s_atr_data_buf[card_id].BWT);
            SCI_ActiveTimer (s_sim_bwt_timer[card_id]);
        }
    }
}

/**********************************************************************
//    Description: Called when Complement of INS or
//                      complement of INS + 1 is responsed from SIM
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static BOOLEAN DMA_SendOneByteToSIM (int32 card_id,uint32 num)
{
    //More instruction data to be sent
    if (s_sim_tx_buf[card_id].tx_data_len > s_sim_tx_buf[card_id].cur_pos)
    {
        USIMDRV_DMA_TX_Disable(card_id);
        sim_tx_dma_cfg[card_id].src_address = (uint32) (&s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos]);
        sim_tx_dma_cfg[card_id].total_length = 1;
        USIMDRV_DMA_TX_Cfg (card_id, &sim_tx_dma_cfg);

        s_sim_tx_buf[card_id].cur_pos++;

        //Enable the TX for the next data transmission
        USIMDRV_DMA_TX_Enable(card_id);

        //Check if all the data in tx buf has been sent to the SIM card.
        if (s_sim_tx_buf[card_id].cur_pos >= s_sim_tx_buf[card_id].tx_data_len)
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
//    Description: This is the Callback function of DMA SIM RX channel
//                      when DMA SIM RX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM1_DMA_RX_Callback (uint32 num)
{
    if (0 == s_atr_data_buf[USIMDRV_CARD_ONE].atr_data.t_protocol)
    {
        DMA_RX_Callback_T0 (USIMDRV_CARD_ONE, num);
    }
    else if (1 == s_atr_data_buf[USIMDRV_CARD_ONE].atr_data.t_protocol)
    {
        DMA_RX_Callback_T1 (USIMDRV_CARD_ONE, num);
    }
}
/**********************************************************************
//    Description: This is the Callback function of DMA SIM RX channel
//                      when DMA SIM RX interrupt happen
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM2_DMA_RX_Callback (uint32 num)
{
    if (0 == s_atr_data_buf[USIMDRV_CARD_TWO].atr_data.t_protocol)
    {
        DMA_RX_Callback_T0 (USIMDRV_CARD_TWO, num);
    }
    else if (1 == s_atr_data_buf[USIMDRV_CARD_TWO].atr_data.t_protocol)
    {
        DMA_RX_Callback_T1 (USIMDRV_CARD_TWO, num);
    }
}
/**********************************************************************
//    Description: Callback function DMA SIM RX interrupt happen (ONLY for T=0 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_RX_Callback_T0 (int32 card_id,uint32 num)
{
    uint16 dma_rx_count = 0, dma_data_cont = 0;
    uint8   data_temp;

    //Disable the DMA RX interrupt
    USIMDRV_DMA_RX_Disable(card_id);

    //Deactive DMA timer
    if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
    }
    s_sim_dma_callback_count[card_id]++;
    s_sim_dma_data_count[card_id] += dma_rx_count;

    //Receive response data from the rx dma buf
    for (dma_data_cont= 0; dma_data_cont < dma_rx_count; dma_data_cont++)
    {
        data_temp = s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos];
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T0] data_temp[0x%2x], rx_cur_pos[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8495_112_2_17_23_6_47_1297,(uint8*)"ddd", TRACE_CARD_ID, data_temp, s_sim_rx_buf[card_id].cur_pos);

        //Check if the sim recv buf is been locked.
        if (s_sim_rx_buf[card_id].is_locked)
        {
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T0] rx_buf is locked!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8500_112_2_17_23_6_47_1298,(uint8*)"d", TRACE_CARD_ID);
            break;
        }

        //Case for the SIM command, there are ACK bytes and NULL bytes should be waited
        if ( (!s_have_ack_byte[card_id]) && (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]))
        {
            //Havent received ACK,cur_pos = 0, in TB_SIM_RECEIVING_RESPONSE
            if ( (s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS] == data_temp) || (s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS] + 1 == data_temp))
            {
                //INS or INS + 1, send the rest of the command
                s_have_ack_byte[card_id] = TRUE;
                USIM_DMA_TX_Callback (card_id, 0); /////////////////////////////////////////////////////////////////////////////////
                //Set DMA RX CFG
                if (s_sim_rx_buf[card_id].rx_expected_len > TB_SIM_DMA_MAX_BUF_SIZE)
                {
                    sim_rx_dma_cfg[card_id].total_size = TB_SIM_DMA_MAX_BUF_SIZE;
                }
                else
                {
                    sim_rx_dma_cfg[card_id].total_size = (uint32) s_sim_rx_buf[card_id].rx_expected_len;
                }
            }
            else if ( (0xE0 == data_temp) && (0xF2 == s_sim_tx_buf[card_id].buf[1]))
            {
                //The instruction should be a "STATUS" instruction
                SCI_TRACE_LOW ("USIMDRV:[%d]:[DMA_RX_Callback_T0] 0xF2 indentify to 0xE0 by the chip!!!", TRACE_CARD_ID); //trace to indicate the "0xe0" problem appears for
                //debug,should be delete in release code for better performance in interrupter
                s_have_ack_byte[card_id] = TRUE;
            }
            else if ( ( (uint8) (~s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS]) == data_temp) || ( (uint8) (~ (s_sim_tx_buf[card_id].buf[TB_SIM_APDU_INS] + 1)) == data_temp))
            {
                //Complement of INS or complement of INS + 1, send next byte
                s_have_ack_byte[card_id] = DMA_SendOneByteToSIM (card_id, 0); //////////////////////////////////////////////////////////////////////////////

                if (s_have_ack_byte[card_id])
                {
                    //Set DMA RX CFG, and receive
                    if (s_sim_rx_buf[card_id].rx_expected_len > TB_SIM_DMA_MAX_BUF_SIZE)
                    {
                        sim_rx_dma_cfg[card_id].total_length = TB_SIM_DMA_MAX_BUF_SIZE;
                    }
                    else
                    {
                        sim_rx_dma_cfg[card_id].total_length = (uint32) s_sim_rx_buf[card_id].rx_expected_len;
                    }
                }
            }
            else if (TB_SIM_RESPONSE_NULL_BYTE == data_temp)
            {
                //NULL data from the SIM
                s_null_byte_count[card_id]++;

                 /* cr217123  delete code */

                //Set DMA RX CFG
                sim_rx_dma_cfg[card_id].total_size = 1;
            }
            else
            {
                //NOT ACK or NULL ????????
                s_sim_rx_buf[card_id].cur_pos++;
                s_have_received_response_data[card_id] = TRUE;

                if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                {
                    s_sim_rx_buf[card_id].cur_pos = 0;
                }
            }
        }
        else
        {
            if ( (TB_SIM_RESPONSE_NULL_BYTE == data_temp) && (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]) && (!s_have_received_response_data[card_id]))
            {
                // more ack need, and deal the null between real data and ack
                if (s_have_ack_byte[card_id])
                {
                    //NOT ACK, need ACK response
                    s_sim_rx_buf[card_id].cur_pos++;
                    s_have_received_response_data[card_id] = TRUE;

                    if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                    {
                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T0]  reset s_sim_rx_buf[card_id], cur_pos[%d]"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8582_112_2_17_23_6_47_1299,(uint8*)"dd", TRACE_CARD_ID, s_sim_rx_buf[card_id].cur_pos);
                        s_sim_rx_buf[card_id].cur_pos = 0;
                    }
                }
                else
                {
                    s_null_byte_count[card_id]++;
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T0]  NULL_byte(0x60) before ACK!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8589_112_2_17_23_6_47_1300,(uint8*)"d", TRACE_CARD_ID);

                     /* +cr217123 */

                    // the last ack has been received and , all data have been send
                    // and at the same time s_sim_wait_resp_timer[card_id] has been activate
                    // so when we now receive 0x60, we must change s_sim_wait_resp_timer[card_id]
                     /* cr217123  delete code */
                    if (s_null_byte_count[card_id] <= 60 )
                    {
                    if (SCI_IsTimerActive (s_sim_wait_resp_timer[card_id]))
                    {
                        SCI_DeactiveTimer (s_sim_wait_resp_timer[card_id]);
                    }
                    USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
                    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
                }
                     /* -cr217123  */
                }
            }
            else
            {
                //DATA byte from SIM
                if ( (data_temp == TB_SIM_RESPONSE_NULL_BYTE))
                {
                    USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
                    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
                }

                s_have_received_response_data[card_id] = TRUE;

                s_sim_rx_buf[card_id].cur_pos++;

                if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                {
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T0]  reset s_sim_rx_buf[card_id], cur_pos[%d]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8624_112_2_17_23_6_47_1301,(uint8*)"dd", TRACE_CARD_ID, s_sim_rx_buf[card_id].cur_pos);
                    s_sim_rx_buf[card_id].cur_pos = 0;
                }
            }
        }
    }

    //if there are new rx data received,check the rx data
    if (SCI_IsTimerActive (s_sim_wwt_timer[card_id]))
    {
        /*  we received some byte so we need stop wwt timer which will start
            if the next byte is waited  */
        SCI_DeactiveTimer (s_sim_wwt_timer[card_id]);
    }

    if ( (!s_sim_rx_buf[card_id].is_locked) && (s_sim_rx_buf[card_id].cur_pos > s_sim_rx_buf[card_id].check_pos))
    {
        //the SIM driver timer is active,then dective the driver timer
        if (SCI_IsTimerActive (s_sim_status_word_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_status_word_timer[card_id]);
        }

        DMA_CheckReceiveBuf(card_id);
    }

    //Set DMA RX CFG, and receive
    if ( (TB_SIM_WAITING_FOR_ATR == s_sim_status[card_id]) ||
            (TB_SIM_PROCESSING_ATR == s_sim_status[card_id]) ||
            (TB_SIM_PROCESSING_PPS == s_sim_status[card_id]) ||
            (s_sim_rx_buf[card_id].cur_pos < 2) && (s_sim_is_processing_cmd[card_id]) && (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]))
    {
        sim_rx_dma_cfg[card_id].des_address = & (s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos]);
        //DMA RX CFG
        USIMDRV_DMA_RX_Cfg (card_id, &sim_rx_dma_cfg[card_id]);

        //DMA RX timer
        if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
        {
            SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
        }
        USIMDRV_CHANGE_TIMER(card_id,s_sim_dma_byte_timer[card_id], DMATimerExpired, SIM_DMA_TIMER_VALUE);
        SCI_ActiveTimer (s_sim_dma_byte_timer[card_id]);

        USIMDRV_DMA_RX_Enable(card_id);
    }
}

/**********************************************************************
//    Description: Callback function DMA SIM RX interrupt happen (ONLY for T=1 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_RX_Callback_T1 (int32 card_id,uint32 num)
{
    uint8 data_temp;
    uint16 i;
    uint32 rx_dma_count = 0;

    //disable the rx interrupt
    USIMDRV_DMA_RX_Disable(card_id);

    //get the rx data number
    rx_dma_count = (uint16) sim_rx_dma_cfg[card_id].total_size;

    if (SCI_IsTimerActive (s_sim_bwt_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_bwt_timer[card_id]);
    }

    /*start cwt timer
        this timer will be used as controller of whole block though
        we need check each character in the spec    */
    if ( (!SCI_IsTimerActive (s_sim_cwt_timer[card_id])) && (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id]))
    {
        USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, (TB_SIM_BLOCK_EXT_BYTE_NUM + 100) * s_cwt_timer_value[card_id]);
        SCI_ActiveTimer (s_sim_cwt_timer[card_id]);
        //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T1] rxfifofull Tick[0x%x], CWT_Timer[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8701_112_2_17_23_6_47_1302,(uint8*)"ddd", TRACE_CARD_ID, SCI_GetTickCount(), s_cwt_timer_value[card_id]);
    }

    //receive response data from the rx fifo buf
    for (i = 0; i < rx_dma_count; i++)
    {
        data_temp = (uint8) s_sim_rx_buf[card_id].buf[s_sim_rx_buf[card_id].cur_pos++];

        if (TB_SIM_RECEIVING_RESPONSE == s_sim_status[card_id])
        {
            //when in receive response status,first put the prologue field into the rx block
            if (s_sim_rx_block[card_id].cur_ext_byte_count < TB_SIM_BLOCK_PRO_BYTE_NUM)
            {
                s_sim_rx_block[card_id].ext_byte[s_sim_rx_block[card_id].cur_ext_byte_count] = data_temp;

                if (TB_SIM_BLOCK_PRO_NAD == s_sim_rx_block[card_id].cur_ext_byte_count)
                {
                    s_sim_rx_block[card_id].base_info.valid_block = 0;

                    /*  the first byte-- nad */
                    if (0 != s_sim_rx_block[card_id].ext_byte[TB_SIM_BLOCK_PRO_NAD])
                    {
                        s_sim_rx_block[card_id].base_info.valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_NAD;
                    }
                    else
                    {
                        s_sim_rx_block[card_id].base_info.valid_block |= TB_SIM_BLOCK_FIELD_VALID_NAD;
                    }
                }
                else if (s_sim_rx_block[card_id].cur_ext_byte_count == TB_SIM_BLOCK_PRO_PCB)
                {
                    if (!GetBlockBaseInfo (card_id, data_temp,&s_sim_rx_block[card_id].base_info))
                    {
                        NotifyExceptionError (card_id, TB_SIMEXP_CARD_UNRESPONSE);
                        //reject the SIM card by power the SIM Card
                        RejectSIMCard(card_id);
                        return;
                    }

                    if (TB_SIM_BLOCK_I_BLOCK == s_sim_rx_block[card_id].base_info.block_type)
                    {
                        /*  if i block received, reset this temp buf    */
                        s_sim_rx_block_buf[card_id].cur_pos = 0;
                    }
                }
                else if (s_sim_rx_block[card_id].cur_ext_byte_count == TB_SIM_BLOCK_PRO_LEN)
                {
                    /*  check the len   */
                    s_sim_rx_block[card_id].info_data_len = data_temp;

                    if (SCI_IsTimerActive (s_sim_cwt_timer[card_id]))
                    {
                        SCI_DeactiveTimer (s_sim_cwt_timer[card_id]);
                    }

                    /* len is error change the timer  */
                    if (USIMDRV_CARD_ONE == card_id)
                    {
                        if (s_sim_ifsd[card_id] < s_sim_rx_block[card_id].info_data_len)
                        {
                            USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, 100);
                        }
                        else
                        {
                            if (TB_SIM_BLOCK_I_BLOCK == s_sim_rx_block[card_id].base_info.block_type)
                            {
                                USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, (100 + s_sim_ifsd[card_id] \
                                    + TB_SIM_BLOCK_EXT_BYTE_NUM + 1) * s_cwt_timer_value[card_id]);
                            }
                            else
                            {
                                USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, \
                                    (10 + TB_SIM_BLOCK_EXT_BYTE_NUM + 1) * s_cwt_timer_value[card_id]);
                            }
                        }
                    }
                    else
                    {
                        if (s_sim_ifsd[card_id] < s_sim_rx_block[card_id].info_data_len)
                        {
                            USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, 100);
                        }
                        else
                        {
                            if (TB_SIM_BLOCK_I_BLOCK == s_sim_rx_block[card_id].base_info.block_type)
                            {
                                USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, \
                                    (100 + s_sim_ifsd[card_id] + TB_SIM_BLOCK_EXT_BYTE_NUM + 1) * s_cwt_timer_value[card_id]);
                            }
                            else
                            {
                                USIMDRV_CHANGE_TIMER(card_id,s_sim_cwt_timer[card_id], DriverTimerExpired, \
                                    (10 + TB_SIM_BLOCK_EXT_BYTE_NUM + 1) * s_cwt_timer_value[card_id]);
                            }
                        }

                    }
                    if (CheckBlockLenValid (&s_sim_rx_block[card_id].info_data_len, &s_sim_rx_block[card_id].base_info))
                    {
                        s_sim_rx_block[card_id].base_info.valid_block |= TB_SIM_BLOCK_FIELD_VALID_LEN;
                    }
                    else
                    {
                        s_sim_rx_block[card_id].base_info.valid_block &= ~TB_SIM_BLOCK_FIELD_VALID_LEN;
                    }

                    SCI_ActiveTimer (s_sim_cwt_timer[card_id]);
                    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T1] rxfifofull t1 Tick[0x%x],CWT_Timer[0x%x],[0x%x,0x%x]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8808_112_2_17_23_6_48_1303,(uint8*)"ddddd", TRACE_CARD_ID, SCI_GetTickCount(), s_cwt_timer_value, data_temp, s_sim_ifsd);
                }

                s_sim_rx_block[card_id].cur_ext_byte_count++;
            }
            else
            {
                //put the info field into the rx buffer,edc into the rx block
                if (s_sim_rx_block[card_id].cur_info_data_count < s_sim_rx_block[card_id].info_data_len)
                {
                    s_sim_rx_block[card_id].cur_info_data_count++;

                    if (s_sim_rx_block[card_id].base_info.block_type == TB_SIM_BLOCK_I_BLOCK)
                    {
                        //put the information data into the rx buffer
                        s_sim_rx_block_buf[card_id].buf[s_sim_rx_block_buf[card_id].cur_pos] = data_temp;
                        s_sim_rx_block_buf[card_id].cur_pos++;

                        if (s_sim_rx_block_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
                        {
                            s_sim_rx_block_buf[card_id].cur_pos = 0;
                        }
                    }
                    else
                    {
                        /* it is an S-Block or R-Block,can not put the data into the rx buffer    */
                        if (9 > s_sim_rx_block[card_id].cur_ext_byte_count)
                        {
                            s_sim_rx_block[card_id].ext_byte[s_sim_rx_block[card_id].cur_ext_byte_count] = data_temp;
                        }

                        //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T1] Section1 s_sim_rx_block Ext_Cnt[0x%x], EXT[0x%x,0x%x,0x%x,0x%x,0x%x], EDC[0x%x]"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8843_112_2_17_23_6_48_1304,(uint8*)"dddddddd", TRACE_CARD_ID, s_sim_rx_block[card_id].cur_ext_byte_count, s_sim_rx_block[card_id].ext_byte[0], s_sim_rx_block[card_id].ext_byte[1], s_sim_rx_block[card_id].ext_byte[2], s_sim_rx_block[card_id].ext_byte[3], s_sim_rx_block[card_id].ext_byte[4], s_sim_rx_block[card_id].have_edc);


                        s_sim_rx_block[card_id].cur_ext_byte_count++;
                    }
                }
                else
                {
                    //put the edc into the rx block
                    s_sim_rx_block[card_id].ext_byte[s_sim_rx_block[card_id].cur_ext_byte_count] = data_temp;
                    s_sim_rx_block[card_id].have_edc = TRUE;
                }
            }
        }
        else
        {
            //when in atr or pps status,directly put the data into rx buffer
            if (s_sim_rx_buf[card_id].cur_pos >= TB_SIM_MAX_MSG_SIZE)
            {
                s_sim_rx_buf[card_id].cur_pos = 0;
            }
        }
    }

    //if there are new rx data received,check the rx data
    if ( (s_sim_rx_buf[card_id].cur_pos > s_sim_rx_buf[card_id].check_pos) || (s_sim_rx_block[card_id].have_edc == TRUE))
    {
        if (s_sim_rx_block[card_id].have_edc)
        {
            //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_RX_Callback_T1] Section2 Ext_Cnt[0x%x], EXT[0x%x,0x%x,0x%x,0x%x,0x%x],EDC[0x%x],rcv[0x%x]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8876_112_2_17_23_6_48_1305,(uint8*)"ddddddddd", TRACE_CARD_ID, s_sim_rx_block[card_id].cur_ext_byte_count, s_sim_rx_block[card_id].ext_byte[0], s_sim_rx_block[card_id].ext_byte[1], s_sim_rx_block[card_id].ext_byte[2], s_sim_rx_block[card_id].ext_byte[3], s_sim_rx_block[card_id].ext_byte[4], s_sim_rx_block[card_id].have_edc, s_sim_rx_block_buf[card_id].cur_pos);
        }

        if (DMA_CheckBlockValid (card_id, &s_sim_tx_block[card_id],&s_sim_rx_block[card_id]))
        {
            DMA_CheckReceiveBuf(card_id);
        }
    }

    //enable the rx full interrupter
    USIMDRV_DMA_RX_Enable(card_id);
}
/**********************************************************************
//    Description:
//  clean all dma int
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIMDRV_DMA_ClearAllInt (int32 card_id)
{
}
/**********************************************************************
//    Description:
//  Init the DMA config when SIM init.
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void USIM_DMA_Init (int32 card_id)
{
    //SCI_TRACE_LOW:"USIMDRV:[%d]:[USIM_DMA_Init]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_8905_112_2_17_23_6_48_1306,(uint8*)"d", TRACE_CARD_ID);

    /* DMA TX parameter  */
    sim_tx_dma_cfg[card_id].type         = DMA_CHN_TYPE_HARD;
    sim_tx_dma_cfg[card_id].priority     = DMA_CHN_PRIORITY_2;
    sim_tx_dma_cfg[card_id].workmode     = DMA_CHN_WORKMODE_NORMAL;
    sim_tx_dma_cfg[card_id].src_datawidth = DMA_DATAWIDTH_BYTE;
    sim_tx_dma_cfg[card_id].dest_datawidth = DMA_DATAWIDTH_BYTE;
    sim_tx_dma_cfg[card_id].src_address  = s_sim_tx_buf[card_id].buf;
    sim_tx_dma_cfg[card_id].requestmode  = DMA_CHN_REQMODE_NORMAL;
    sim_tx_dma_cfg[card_id].block_length = 1;
    sim_tx_dma_cfg[card_id].total_length = 5;
    sim_tx_dma_cfg[card_id].src_step     = 1;
    sim_tx_dma_cfg[card_id].dest_step    = 0;
    sim_tx_dma_cfg[card_id].src_blockmode = DMA_BLOCKMODE_INCR;
    sim_tx_dma_cfg[card_id].dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;

    /* DMA RX parameter */
    sim_rx_dma_cfg[card_id].type         = DMA_CHN_TYPE_HARD;
    sim_rx_dma_cfg[card_id].priority     = DMA_CHN_PRIORITY_0;
    sim_rx_dma_cfg[card_id].workmode     = DMA_CHN_WORKMODE_NORMAL;
    sim_rx_dma_cfg[card_id].src_datawidth = DMA_DATAWIDTH_BYTE;
    sim_rx_dma_cfg[card_id].dest_datawidth = DMA_DATAWIDTH_BYTE;
    sim_rx_dma_cfg[card_id].src_address  = SIM_RX+3;
    sim_rx_dma_cfg[card_id].dest_address = s_sim_rx_buf[card_id].buf;
    sim_rx_dma_cfg[card_id].requestmode  = DMA_CHN_REQMODE_NORMAL;
    sim_rx_dma_cfg[card_id].block_length = 1;
    sim_rx_dma_cfg[card_id].total_length = 1;
    sim_rx_dma_cfg[card_id].src_step     = 0;
    sim_rx_dma_cfg[card_id].dest_step    = 1;
    sim_rx_dma_cfg[card_id].src_blockmode = DMA_BLOCKMODE_SINGLETRANS;
    sim_rx_dma_cfg[card_id].dest_blockmode = DMA_BLOCKMODE_INCR;

    if (USIMDRV_CARD_ONE == card_id)
    {
        sim_tx_dma_cfg[card_id].dest_address = SIM_TX+3;
        /* reg dma sim tx/rx callback */
        DMA_HAL_ChnIntConfig (DMA_SIM0_TX, DMA_CHN_INT_TYPE_TRANSDONE, USIM1_DMA_TX_Callback);
        DMA_HAL_ChnIntConfig (DMA_SIM0_RX, DMA_CHN_INT_TYPE_TRANSDONE, USIM1_DMA_RX_Callback);
        /* if timer is create already, we should not create again */
    }
    else
    {
        sim_tx_dma_cfg[card_id].dest_address = SIM1_TX+3;
        /* reg dma sim tx/rx callback */
        DMA_HAL_ChnIntConfig (DMA_SIM1_TX, DMA_CHN_INT_TYPE_TRANSDONE, USIM2_DMA_TX_Callback);
        DMA_HAL_ChnIntConfig (DMA_SIM1_RX, DMA_CHN_INT_TYPE_TRANSDONE, USIM2_DMA_RX_Callback);
        /* if timer is create already, we should not create again */
    }

    if (NULL == s_sim_dma_byte_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_dma_byte_timer[card_id],"SIM DMA RX Timer",
                            DMATimerExpired,1,SIM_DMA_TIMER_VALUE,SCI_NO_ACTIVATE);
    }

    //Re-register timer for DMA
    if (NULL == s_sim_bwt_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_bwt_timer[card_id],"SIM BWT Timer",
                        DMATimerExpired,5,SIM_BWT_TIMER_VALUE,SCI_NO_ACTIVATE);
    }

    if (NULL == s_sim_wwt_timer[card_id])
    {
        USIMDRV_CREATE_TIMER(card_id,s_sim_wwt_timer[card_id],"SIM wwt Timer",
                         DMATimerExpired,7,s_wwt_timer_value[card_id],SCI_NO_ACTIVATE);
    }

    /* clean all sim dma int */
    USIMDRV_DMA_ClearAllInt(card_id);

    /* sim dma  channel config */
    USIMDRV_DMA_TX_Cfg (&sim_tx_dma_cfg[card_id]);
    USIMDRV_DMA_RX_Cfg (&sim_rx_dma_cfg[card_id]);

}
/* -binli add dma test */
/**********************************************************************
//    Description: Send a Instruction to the SIM card to start an operation on
//    The SIM card we needed
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
SIM_SEND_INSTR_RESULT_E USIM_DMA_SendInstrCode (int32 card_id,TB_USIM_APDU_T *apdu_ptr)
{
    if (0 == s_atr_data_buf[card_id].atr_data.t_protocol)
    {
        DMA_SendInstrCode_T0 (card_id, apdu_ptr);
    }
    else if (1 == s_atr_data_buf[card_id].atr_data.t_protocol)
    {
        DMA_SendInstrCode_T1 (card_id, apdu_ptr);
    }

    SCI_SIM_EnableDeepSleep (0);

    return SIM_SEND_INSTR_OK;
}
/**********************************************************************
//    Description:Send a Instruction to the SIM card in DMA mode(only for T0 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendInstrCode_T0 (int32 card_id,TB_USIM_APDU_T *apdu_ptr)
{
    uint32 tx_data_count = 0;

    //Disable RX DMA
    USIMDRV_DMA_RX_Disable(card_id);

    //log for authentication
    if( 0x88 == apdu_ptr->instr_code[TB_SIM_APDU_INS] )
    {
        s_auth_inited[card_id] = TRUE;
    }

    if (0xA4 == apdu_ptr->instr_code[TB_SIM_APDU_INS])
    {
        apdu_ptr->rx_expected_len = 0;
    }

    s_is_reset_flag_set[card_id] = FALSE;

    if (0xf2 == apdu_ptr->instr_code[TB_SIM_APDU_INS])
    {
        if (s_is_reset_flag_set[card_id])
        {
            NotifyExceptionError (TB_SIMEXP_CARD_UNRESPONSE);
            s_is_reset_flag_set[card_id] = FALSE;
            return;
        }
        else
        {
            s_is_reset_flag_set[card_id] = TRUE;
        }
    }

    //Disable TX DMA
    USIMDRV_DMA_TX_Disable(card_id);

    //FIVE bytes to be sent
    s_sim_tx_buf[card_id].tx_data_len = TB_SIM_APDU_CHEADER_FIVE;

    //Copy instruction code to the tx buffer
    //CLA INS P1 P2 P3
    memcpy (s_sim_tx_buf[card_id].buf,apdu_ptr->instr_code,TB_SIM_APDU_CHEADER_FIVE);

    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_PARITY_ERR;

    //is send data exist
    if ( (TB_SIMINSTR_SEND_DATA == apdu_ptr->instr_type) || (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type))
    {
        //copy the data after the command code
        if (0 != apdu_ptr->instr_code[TB_SIM_APDU_P3])
        {
            s_sim_tx_buf[card_id].tx_data_len += apdu_ptr->instr_code[TB_SIM_APDU_P3];
            memcpy ( (s_sim_tx_buf[card_id].buf + TB_SIM_APDU_CHEADER_FIVE),apdu_ptr->instr_data, apdu_ptr->instr_code[TB_SIM_APDU_P3]);
        }
    }

    //USIM DRV LOG
    if (s_auth_inited[card_id])
    {
        //SCI_TRACE_LOW:"USIMDRV:[%d]:tx_data_len is[%d], rx_expected_len[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9070_112_2_17_23_6_48_1307,(uint8*)"ddd", TRACE_CARD_ID, s_sim_tx_buf[card_id].tx_data_len, apdu_ptr->rx_expected_len + TB_SIM_RX_STATUS_WORD_NUM);

        while (s_sim_tx_buf[card_id].tx_data_len > tx_data_count*16)
        {
            //SCI_TRACE_LOW:"USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9078_112_2_17_23_6_48_1308,(uint8*)"ddddddddddddddddd", TRACE_CARD_ID,s_sim_tx_buf[card_id].buf[tx_data_count*16 + 0],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 1],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 2],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 3],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 4],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 5],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 6],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 7],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 8],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 9],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 10],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 11],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 12],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 13],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 14],s_sim_tx_buf[card_id].buf[tx_data_count*16 + 15]);
            tx_data_count++;

            if (16 <= tx_data_count)
            {
                break;
            }
        }
    }

    //Unlock the RX buf
    s_sim_rx_buf[card_id].is_locked = FALSE;

    //get the expected len of the response data
    if ( (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type) || (TB_SIMINSTR_RECV_DATA == apdu_ptr->instr_type))
    {
        s_sim_rx_buf[card_id].rx_expected_len = apdu_ptr->rx_expected_len + TB_SIM_RX_STATUS_WORD_NUM;
    }
    else
    {
        //Two bytes status words.
        s_sim_rx_buf[card_id].rx_expected_len = TB_SIM_RX_STATUS_WORD_NUM;
    }

    s_sim_tx_instr_len[card_id] = s_sim_tx_buf[card_id].tx_data_len;
    s_sim_rx_rexpect_len[card_id] = s_sim_rx_buf[card_id].rx_expected_len;

    //Set TX DMA CFG, and send the command header
    sim_tx_dma_cfg[card_id].src_address = (uint32) s_sim_tx_buf[card_id].buf;
    sim_tx_dma_cfg[card_id].total_size = TB_SIM_APDU_CHEADER_FIVE;
    USIMDRV_DMA_TX_Cfg (card_id, &sim_tx_dma_cfg[card_id]);


    s_sim_tx_buf[card_id].cur_pos += TB_SIM_APDU_CHEADER_FIVE;

    USIMDRV_DMA_TX_Enable(card_id);

    s_sim_dma_data_count[card_id] = 0;
    s_sim_dma_callback_count[card_id] = 0;

    //Start the wait response timer
    USIMDRV_CHANGE_TIMER(card_id,s_sim_wait_resp_timer[card_id],DriverTimerExpired,s_sim_wait_resp_timer_value[card_id]);
    SCI_ActiveTimer (s_sim_wait_resp_timer[card_id]);
}

/**********************************************************************
//    Description: Send a Instruction to the SIM card(only for T1 protocol)
//    Global resource dependence :
//    Author:Eddie.Wang
//    Note:
***********************************************************************/
static void DMA_SendInstrCode_T1 (int32 card_id,TB_USIM_APDU_T *apdu_ptr)
{
    uint16 block_ifsc_size;
    uint16 real_data_len;
    uint8 block_count = 0;
    uint8 *block_begin_ptr = s_sim_tx_buf[card_id].buf;
    uint8 *block_begin_pos = s_sim_tx_buf[card_id].buf;
    uint8 *instruction_data_ptr = apdu_ptr->instr_data;
    uint16 data_of_the_last_block = 0;
    uint16 i,data_len;
    uint16 tx_data_len;
    uint16 first_send_data_num;//the data num of the first time to sent
    BOOLEAN is_next_block_exist = TRUE;
    BOOLEAN is_data_sent_all = FALSE;

    USIM_ASSERT (NULL != apdu_ptr);/*assert verified*/

    //Disable the retransmit fucntion to handler rx parity error.
    s_sim_register_map[card_id]->she = SIM_DEFAULT_SHE;

    USIMDRV_DMA_RX_Disable(card_id);

    //Clear variable
    ClearTxBlock(card_id);
    ClearRxBlock(card_id);

    //SCI_TRACE_LOW:"USIMDRV:[%d]:[DMA_SendInstrCode_T1] IFSC[0x%x], instr_type[0x%x], P3[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9154_112_2_17_23_6_48_1309,(uint8*)"dddd", TRACE_CARD_ID, s_atr_data_buf[card_id].atr_data.IFSC, apdu_ptr->instr_type, apdu_ptr->instr_code[TB_SIM_APDU_P3]);

    //Get the block count
    block_ifsc_size = s_atr_data_buf[card_id].atr_data.IFSC;

    //Calculate data length in the information field
    tx_data_len = TB_SIM_APDU_CHEADER_FIVE;

    if (TB_SIMINSTR_SEND_DATA == apdu_ptr->instr_type)
    {
        //copy the data after the command code
        if (0 != apdu_ptr->instr_code[TB_SIM_APDU_P3])
        {
            tx_data_len += apdu_ptr->instr_code[TB_SIM_APDU_P3];
        }
    }
    else if (apdu_ptr->instr_type == TB_SIMINSTR_SEND_AND_RECV_DATA)
    {
        tx_data_len += (apdu_ptr->instr_code[TB_SIM_APDU_P3] + 1);
    }

    //Calculate the block count and data num in the block
    block_count = tx_data_len/block_ifsc_size;
    data_of_the_last_block = tx_data_len%block_ifsc_size;

    if (data_of_the_last_block!= 0)
    {
        block_count += 1;
    }
    else
    {
        data_of_the_last_block = block_ifsc_size;

    }

    s_sim_tx_buf[card_id].tx_data_len = tx_data_len + TB_SIM_BLOCK_EXT_BYTE_NUM* block_count;


    //Record the base info of the first block to the tx block
    s_sim_tx_block[card_id].block_begin_pos = 0;

    if (block_count == 1)
    {
        s_sim_tx_block[card_id].block_end_pos = data_of_the_last_block + 4;
        s_sim_tx_block[card_id].base_info.i_block.chain_M = FALSE;
    }
    else
    {
        s_sim_tx_block[card_id].block_end_pos = block_ifsc_size + 4;
        s_sim_tx_block[card_id].base_info.i_block.chain_M = TRUE;
    }

    s_sim_tx_block[card_id].base_info.block_type = TB_SIM_BLOCK_I_BLOCK;

    //Disable DMA TX and fill the data into the tx buf
    USIMDRV_DMA_TX_Disable(card_id);

    for (i = 0; i< block_count; i++)
    {
        data_len = block_ifsc_size;

        if (i == (block_count -1))
        {
            data_len = data_of_the_last_block;
            is_next_block_exist = FALSE;
        }

        real_data_len = data_len;   //data in the block except for the command header
        block_begin_pos = block_begin_ptr;

        //Fill the prologue and epilogue field
        *block_begin_ptr = SIM_DEFAULT_NAD;         //NAD
        block_begin_ptr += 1;
        s_pcb_ns[card_id] = (s_pcb_ns[card_id] + 1) %2;
        *block_begin_ptr = SIM_DEFAULT_PCB_IBLOCK | (s_pcb_ns[card_id] << USIM_CONST_NUM6) | (is_next_block_exist << USIM_CONST_NUM5); //PCB

        block_begin_ptr += 1;
        *block_begin_ptr = data_len;                //LEN
        block_begin_ptr += 1;

        //Fill the command header into the information field
        if (i == 0) //copy the command header
        {
            if (apdu_ptr->instr_type == TB_SIMINSTR_SEND_AND_RECV_DATA)
            {
                //Copy 6 command header
                memcpy (block_begin_ptr,apdu_ptr->instr_code,TB_SIM_APDU_CHEADER_NUM);
                block_begin_ptr += TB_SIM_APDU_CHEADER_NUM;
                real_data_len = data_len - TB_SIM_APDU_CHEADER_NUM;   //data in the block except for the command header
            }
            else
            {
                //Copy 5 command header
                memcpy (block_begin_ptr,apdu_ptr->instr_code,TB_SIM_APDU_CHEADER_FIVE);
                block_begin_ptr += TB_SIM_APDU_CHEADER_FIVE;
                real_data_len = data_len - TB_SIM_APDU_CHEADER_FIVE;  //data in the block except for the command header
            }
        }

        //Copy data
        memcpy (block_begin_ptr,instruction_data_ptr,real_data_len);
        block_begin_ptr += real_data_len;

        if (!is_next_block_exist)
        {
            if (apdu_ptr->instr_type == TB_SIMINSTR_SEND_AND_RECV_DATA)
            {
                * (block_begin_ptr-1) = 0;
            }
        }

        //Fill the epilogue field
        *block_begin_ptr = GetEDC (block_begin_pos,block_begin_ptr - block_begin_pos);
        block_begin_ptr += 1;

        //Regulate the pointer
        instruction_data_ptr += real_data_len;
    }

    //Get the expected length of the response data
    if ( (TB_SIMINSTR_SEND_AND_RECV_DATA == apdu_ptr->instr_type) || (TB_SIMINSTR_RECV_DATA == apdu_ptr->instr_type))
    {
        s_sim_rx_buf[card_id].rx_expected_len = apdu_ptr->rx_expected_len + TB_SIM_RX_STATUS_WORD_NUM;
    }
    else
    {
        //Two byte status words.
        s_sim_rx_buf[card_id].rx_expected_len = TB_SIM_RX_STATUS_WORD_NUM;
    }

    s_sim_tx_instr_len[card_id] = s_sim_tx_buf[card_id].tx_data_len;
    s_sim_rx_rexpect_len[card_id]= s_sim_rx_buf[card_id].rx_expected_len;

    //Calculate the data number to send in the first time
    first_send_data_num = s_sim_tx_block[card_id].block_end_pos;

    s_sim_tx_block[card_id].base_info.i_block.ns = GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM6);
    s_sim_tx_block[card_id].base_info.i_block.chain_M = GET_BIT_VALUE (s_sim_tx_buf[card_id].buf[s_sim_tx_buf[card_id].cur_pos + (uint16) TB_SIM_BLOCK_PRO_PCB], USIM_CONST_NUM5);

    //SET DMA CFG and send the first set data
    if (first_send_data_num > TB_SIM_DMA_MAX_BUF_SIZE)
    {
        sim_tx_dma_cfg[card_id].total_length= TB_SIM_DMA_MAX_BUF_SIZE;
        sim_tx_dma_cfg[card_id].block_length= TB_SIM_DMA_MAX_BUF_SIZE;
        first_send_data_num = TB_SIM_DMA_MAX_BUF_SIZE;
    }
    else
    {
        sim_tx_dma_cfg[card_id].total_length = (uint32) first_send_data_num;
        sim_tx_dma_cfg[card_id].block_length = (uint32) first_send_data_num;
        is_data_sent_all = TRUE;
    }

    sim_tx_dma_cfg[card_id].src_address= (uint32) s_sim_tx_buf[card_id].buf;
    if (USIMDRV_CARD_ONE == card_id)
    {
        DMA_HAL_ChnConfig (DMA_SIM0_TX, &sim_tx_dma_cfg[card_id]);
    }
    else
    {
        DMA_HAL_ChnConfig (DMA_SIM1_TX, &sim_tx_dma_cfg[card_id]);
    }
    s_sim_tx_buf[card_id].cur_pos += (uint16) sim_tx_dma_cfg[card_id].total_length;

    USIMDRV_DMA_TX_Enable(card_id);

    //DMA RX CFG is default CFG....
    USIMDRV_DMA_RX_Enable(card_id);

    //Set DMA Timer
    if (SCI_IsTimerActive (s_sim_dma_byte_timer[card_id]))
    {
        SCI_DeactiveTimer (s_sim_dma_byte_timer[card_id]);
    }

    USIMDRV_CHANGE_TIMER(card_id,s_sim_dma_byte_timer[card_id], DMATimerExpired, SIM_RX_TIMEOUT_VALUE);

    SCI_ActiveTimer (s_sim_dma_byte_timer[card_id]);

    //if all the data of the block has been sent,start the bwt timer
    if (is_data_sent_all)
    {
        USIMDRV_CHANGE_TIMER(card_id,s_sim_bwt_timer[card_id],DMATimerExpired,s_atr_data_buf[card_id].BWT);
        SCI_ActiveTimer (s_sim_bwt_timer[card_id]);
    }
}
/**********************************************************************
//    Description: the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim1DMATimerExpired (
    uint32 lparam   //the function's input parameter
)
{
    DMA_CHx_CTL_T *dma_ch_in  = (DMA_CHx_CTL_T *) (DMA_REG_BASE + 0x400 + 0x20*DMA_SIM0_RX);

    switch (lparam)
    {
        case 1:
            {
                if (dma_ch_in->size < sim_rx_dma_cfg[USIMDRV_CARD_ONE].total_size)
                {
                    //SCI_TRACE_LOW:"USIMDRV:[0]:[Sim1DMATimerExpired] Tick[0x%x],rcv_byte[0x%x]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9358_112_2_17_23_6_49_1310,(uint8*)"dd", SCI_GetTickCount(),sim_rx_dma_cfg[USIMDRV_CARD_ONE].total_size - dma_ch_in->size);

                    USIM1_DMA_RX_Callback (0);
                }
                else
                {
                    if (SCI_IsTimerActive (s_sim_dma_byte_timer[USIMDRV_CARD_ONE]))
                    {
                        SCI_DeactiveTimer (s_sim_dma_byte_timer[USIMDRV_CARD_ONE]);
                    }

                    USIMDRV_CHANGE_TIMER(USIMDRV_CARD_ONE,s_sim_dma_byte_timer[USIMDRV_CARD_ONE], DMATimerExpired, SIM_DMA_TIMER_VALUE);
                    SCI_ActiveTimer (s_sim_dma_byte_timer[USIMDRV_CARD_ONE]);
                }

                break;
            }

        case 5:
            {
                uint32 data_temp;
                data_temp = sim_rx_dma_cfg[USIMDRV_CARD_ONE].total_size - dma_ch_in->size;

                if (0 == data_temp)
                {
                    ErrorHandler (USIMDRV_CARD_ONE, T1_ERROR_CODE_BWT_TIMER_OUT);
                }
                else
                {
                    /* data received    */
                }

                //SCI_TRACE_LOW:"USIMDRV:[0]:[Sim1DMATimerExpired] DMA BWT timer expired[0x%x][0x%x,0x%x],0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9392_112_2_17_23_6_49_1311,(uint8*)"dddd", SCI_GetTickCount(), s_sim_rx_buf[USIMDRV_CARD_ONE].cur_pos, s_sim_rx_block[USIMDRV_CARD_ONE].cur_ext_byte_count,data_temp);

                //SIM_PrintRxBuf();
                break;
            }

        case 7:
            {
                uint32 data_temp;
                data_temp = sim_rx_dma_cfg[USIMDRV_CARD_ONE].total_size - dma_ch_in->size;
                //SCI_TRACE_LOW:"USIMDRV:[0]:[Sim1DMATimerExpired] DMA WWT timer expired[0x%x][0x%x],0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9404_112_2_17_23_6_49_1312,(uint8*)"ddd", SCI_GetTickCount(), s_sim_rx_buf[USIMDRV_CARD_ONE].cur_pos, data_temp);

                if (0 == data_temp)
                {
                    /*  No data after wwt timer expired */
                    /*  If authentication is sent stop the wwt timer    */
                    if  (!s_auth_inited[USIMDRV_CARD_ONE])
                    {
                        NotifyExceptionError (USIMDRV_CARD_ONE,TB_SIMEXP_CARD_UNRESPONSE);
                        SIM_PowerOffSIMEx(USIMDRV_CARD_ONE);
                    }
                }
                else
                {
                    /* data received    */
                }

                break;
            }

        default:
            break;
    }

}
/**********************************************************************
//    Description: the funciton to proccess the driver timer expire
//    Global resource dependence :
//    Author:bin.li
//    Note:
***********************************************************************/
static void Sim2DMATimerExpired (
    uint32 lparam   //the function's input parameter
)
{
    DMA_CHx_CTL_T *dma_ch_in  = (DMA_CHx_CTL_T *) (DMA_REG_BASE + 0x400 + 0x20*DMA_SIM0_RX);

    switch (lparam)
    {
        case 1:
            {
                if (dma_ch_in->size < sim_rx_dma_cfg[USIMDRV_CARD_TWO].total_size)
                {
                    //SCI_TRACE_LOW:"USIMDRV:[1]:[Sim2DMATimerExpired]  DMATimerExpired[0x%x],rcv byte[0x%x]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9447_112_2_17_23_6_49_1313,(uint8*)"dd", SCI_GetTickCount(),sim_rx_dma_cfg[USIMDRV_CARD_TWO].total_size - dma_ch_in->size);

                    USIM2_DMA_RX_Callback (0);
                }
                else
                {
                    if (SCI_IsTimerActive (s_sim_dma_byte_timer[USIMDRV_CARD_TWO]))
                    {
                        SCI_DeactiveTimer (s_sim_dma_byte_timer[USIMDRV_CARD_TWO]);
                    }

                    USIMDRV_CHANGE_TIMER(USIMDRV_CARD_TWO,s_sim_dma_byte_timer[USIMDRV_CARD_TWO], DMATimerExpired, SIM_DMA_TIMER_VALUE);
                    SCI_ActiveTimer (s_sim_dma_byte_timer[USIMDRV_CARD_TWO]);
                }

                break;
            }

        case 5:
            {
                uint32 data_temp;
                data_temp = sim_rx_dma_cfg[USIMDRV_CARD_TWO].total_size - dma_ch_in->size;

                if (0 == data_temp)
                {
                    ErrorHandler (USIMDRV_CARD_TWO, T1_ERROR_CODE_BWT_TIMER_OUT);
                }
                else
                {
                    /* data received    */
                }

                //SCI_TRACE_LOW:"USIMDRV:[1]:[Sim2DMATimerExpired] DMA BWT timer expired[0x%x][0x%x,0x%x],0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9481_112_2_17_23_6_49_1314,(uint8*)"dddd", SCI_GetTickCount(), s_sim_rx_buf[USIMDRV_CARD_TWO].cur_pos, s_sim_rx_block[USIMDRV_CARD_TWO].cur_ext_byte_count,data_temp);

                //SIM_PrintRxBuf();
                break;
            }

        case 7:
            {
                uint32 data_temp;
                data_temp = sim_rx_dma_cfg[USIMDRV_CARD_TWO].total_size - dma_ch_in->size;
                //SCI_TRACE_LOW:"USIMDRV:[1]:[Sim2DMATimerExpired] DMA WWT timer expired[0x%x][0x%x],0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USIM_DRV_9493_112_2_17_23_6_49_1315,(uint8*)"ddd", SCI_GetTickCount(), s_sim_rx_buf[USIMDRV_CARD_TWO].cur_pos, data_temp);

                if (0 == data_temp)
                {
                    /*  No data after wwt timer expired */
                    /*  If authentication is sent stop the wwt timer    */
                    if  (!s_auth_inited[USIMDRV_CARD_TWO])
                    {
                        NotifyExceptionError (USIMDRV_CARD_TWO,TB_SIMEXP_CARD_UNRESPONSE);
                        SIM_PowerOffSIMEx(USIMDRV_CARD_TWO);
                    }
                }
                else
                {
                    /* data received    */
                }

                break;
            }

        default:
            break;
    }

}
#endif
#ifdef   __cplusplus
}
#endif
