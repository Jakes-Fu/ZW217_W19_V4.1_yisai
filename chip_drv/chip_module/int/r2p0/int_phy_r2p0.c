/******************************************************************************
 ** File Name:      int_phy_v3.c                                                  *
 ** Author:         steve.zhan                                             *
 ** DATE:           10/14/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2002     steve.zhan    Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
//#include "../../adc/v3/adc_reg_v3.h"
//#include "../../tpc/v3/tpc_reg_v3.h"
//#include "../../watchdog/v3/watchdog_reg_v3.h"
#include "../int_phy.h"
#include "fiq_drvapi.h"
#include "task_monitor.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Data                                              *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
//define for irq
#if (ISR_UNIT_NUM == 1)
uint32 IrqUnitRegAddr[ISR_UNIT_NUM] = {INT_IRQ_STS};
PUBLIC uint32 IrqUnitRegAddrMask[ISR_UNIT_NUM] = {0xffffffff};

#elif (ISR_UNIT_NUM == 2)
uint32 IrqUnitRegAddr[ISR_UNIT_NUM] = {INT_IRQ_STS,ANA_INT_STATUS};
PUBLIC uint32 IrqUnitRegAddrMask[ISR_UNIT_NUM] = {0xffffffff,0x1FF};

#elif (ISR_UNIT_NUM == 3)
uint32 IrqUnitRegAddr[ISR_UNIT_NUM] = {INT0_IRQ_STS, INT1_IRQ_STS, ANA_INT_STATUS};
PUBLIC uint32 IrqUnitRegAddrMask[ISR_UNIT_NUM] = {0xffffffff, 0xffffffff, 0x1FF};

#elif (ISR_UNIT_NUM == 4)
#error "You must define Interrupt status register, and the Mask value of valid bits"

#endif

PUBLIC uint32 IrqUnitRegValue[ISR_UNIT_NUM] = {0};

//define for fiq
LOCAL uint32 FiqUnitRegAddr[ISR_UNIT_NUM] = {INT0_FIQ_STS, INT1_FIQ_STS};/*lint !e785 confirmed by xuepeng*/
PUBLIC uint32 FiqUnitRegValue[ISR_UNIT_NUM] = {0};

typedef struct ana_sub_int_ctrl
{
    uint32  mod_int_ctrl_addr;
    uint16  int_msk;
} ANA_SUB_INT_CTRL_T;


static const ANA_SUB_INT_CTRL_T sub_int_ctrl[ANA_MAX_INT_NUM] =
{
#ifdef NEED_MODIFY
    {ADC_INT_EN,        ADC_IRQ_EN_BIT  },
    {0,                 0               },          ///ana gpio
    {ANA_RTC_INT_EN,    RTC_INT_ALL_MSK },
    {WDG_CTRL,  WDG_INT_EN_BIT  },
    {TPC_INT_EN,        TPC_INT_ALL_MSK }
#endif
    {0,                 0               }
};


/* change dsp assert to passert,define struct by id and string */
typedef struct dsp_assert_info_tag
{
    uint32 id;
    uint8 *str;
} DSP_ASSERT_INFO_T, *DSP_ASSERT_INFO_PTR_T;
/*lint -e64*/
LOCAL const DSP_ASSERT_INFO_T s_dsp_assert_table[] =
{

    {0x0300, "Dsp_rtos_init.c: check memory integrity failed"},
    {0x0301, "Dsp_rtos_init.c: check memory integrity failed"},
    {0x0302, "Dsp_rtos_init.c: check memory integrity failed"},
    {0x0303, "Dsp_rtos_init.c: check memory integrity failed"},
    {0x0304, "Dsp_rtos_init.c: in EDGE mode, GL_null_frame_counter!=0"},
    {0x0305, "Dsp_rtos_init.c: check memory integrity failed"},
    {0x0306, "Dsp_rtos_init.c: wait for setting frequency stable, failed"},
    {0x0307, "Dsp_rtos_init.c: wait for setting frequency stable, failed"},
    {0x0308, "Dsp_rtos_init.c: wait for setting frequency stable, failed"},
    {0x0309, "Dsp_rtos_init.c: wait for setting frequency stable, failed"},
    {0x030A, "Dsp_rtos_init.c: wait for setting frequency stable, failed"},
    {0x030B, "Dsp_rtos_init.c: wait for setting frequency stable, failed"},
    {0x030C, "Dsp_rtos_init.c: wait for setting frequency stable, failed"},

    {0x0400, "Tx_interleaver.c: invalid channel_enum"},
    {0x0401, "Tx_interleaver.c: invalid channel_enum"},
    {0x0402, "Tx_interleaver.c: invalid channel_enum"},
    {0x0403, "Tx_interleaver.c: invalid channel_enum"},
    {0x0404, "Tx_interleaver.c: invalid channel_enum"},

    {0x0503, "Tx_encoder.c: invalid sync_sequence_index"},
    {0x0506, "Tx_encoder.c: invalid channel_type"},
    {0x0513, "Tx_encoder.c: channel_enum!=tx_channel_type[GL_TX_encode_count]"},

    {0x0800, "Sleep_control.c: invalid DSP_sleep_state"},
    {0x0802, "Sleep_control.c: invalid cmd_id"},
    {0x0803, "Sleep_control.c: tried 5 times to force sleep"},
    {0x0806, "Sleep_control.c: calibration state change to cal_done immediately after calibration start"},

    {0x0B00, "Rx_equalize.c: check memory integrity failed"},
    {0x0B01, "Rx_equalize.c: check memory integrity failed"},
    {0x0B02, "Rx_equalize.c: check memory integrity failed"},
    {0x0B03, "Rx_equalize.c: check memory integrity failed"},
    {0x0B04, "Rx_equalize.c: check memory integrity failed"},
    {0x0B05, "Rx_equalize.c: check memory integrity failed"},
    {0x0B06, "Rx_equalize.c: check memory integrity failed"},
    {0x0B07, "Rx_equalize.c: check memory integrity failed"},
    {0x0B08, "Rx_equalize.c: check memory integrity failed"},
    {0x0B09, "Rx_equalize.c: check memory integrity failed"},
    {0x0B0A, "Rx_equalize.c: check memory integrity failed"},
    {0x0B0B, "Rx_equalize.c: check memory integrity failed"},
    {0x0B0C, "description: Rx_equalize.c: check memory integrity failed"},

    {0x0C00, "Rx_agc.c: rx_block_num>=4"},
    {0x0C01, "Rx_agc.c: rx_block_num>=4"},
    {0x0C02, "Rx_agc.c: band_info>=4"},
    {0x0C03, "Rx_agc.c: rx_block_num>=MAX_RX_SLOT_NUM"},
    {0x0C04, "Rx_agc.c: rx_block_num>=4"},
    {0x0C05, "Rx_agc.c: rx_block_num>=4"},

    {0x1001, "Run_rx_dispatch_task.c: invalid channel_type"},
    {0x1100, "Run_rx_deinterleaver_task.c: invalid channel_type"},
    {0x1101, "Run_rx_deinterleaver_task.c: check memory integrity failed"},
    {0x1102, "Run_rx_deinterleaver_task.c: (slot_index + first_rx_slot) > 7"},
    {0x1103, "Run_rx_deinterleaver_task.c: invalid header_type"},
    {0x1104, "Run_rx_deinterleaver_task.c: check memory integrity failed"},
    {0x1105, "Run_rx_deinterleaver_task.c: rx_block_index>=4"},
    {0x1106, "Run_rx_deinterleaver_task.c: edge_rx_data_info_head_ptr==0"},
    {0x1107, "Run_rx_deinterleaver_task.c: invalid mod_type"},
    {0x1108, "Run_rx_deinterleaver_task.c: check memory integrity failed"},
    {0x1C00, "frame_eng_MCU.c: TX_num>2"},
    {0x1D00, "Frame_eng_main.c: invalid cs"},
    {0x1D01, "Frame_eng_main.c: tx_counter>=MAX_TX_INT_NUM"},
    {0x1D02, "Frame_eng_main.c: rx_block_num>4"},
    {0x1D03, "Frame_eng_main.c: number_of_event>8"},
    {0x1D06, "Frame_eng_main.c: GL_null_frame_counter > 0x10"},
    {0x1F00, "frame_eng_EDGE_operations.c: tx_slot_number>=3"},
    {0x1F01, "frame_eng_EDGE_operations.c: tx_slot_num>4"},
    {0x1F02, "frame_eng_EDGE_operations.c: tx_slot_index>7"},
    {0x1F03, "frame_eng_EDGE_operations.c: event_number>5"},
    {0x1F04, "frame_eng_EDGE_operations.c: event_number>5"},
    {0x1F05, "frame_eng_EDGE_operations.c: event_number>MAX_EVENT_NUM"},
    {0x1F06, "frame_eng_EDGE_operations.c: invalid slot_index"},

    {0x2200, "Event_table.c: number_of_Event>=8"},
    {0x2201, "Event_table.c: event_cnt>=5"},
    {0x2202, "Event_table.c: event_cnt>=5 or burst_index>=4"},
    {0x2300, "En_est.c: ts_index<0"},
    {0x2301, "En_est.c: ts_index<0"},
    {0x2303, "En_est.c: invalid k value"},
    {0x2304, "En_est.c: invalid ts_index"},
    {0x2305, "En_est.c: check memory integrity failed"},
    {0x2306, "En_est.c: check memory integrity failed"},
    {0x2500, "EDGE_TXRX_control.c: invalid slot_index"},
    {0x2501, "EDGE_TXRX_control.c: invalid data1_ps"},
    {0x2502, "EDGE_TXRX_control.c: invalid edge_mcs"},
    {0x2503, "EDGE_TXRX_control.c: invalid encode_index"},
    {0x2504, "EDGE_TXRX_control.c: invalid channel_type"},
    {0x2506, "EDGE_TXRX_control.c: invalid ts_index"},
    {0x2507, "EDGE_TXRX_control.c: invalid rx_block_index"},
    {0x2508, "EDGE_TXRX_control.c: invalid rx_block_index"},
    {0x2509, "EDGE_TXRX_control.c: invalid rx_block_index"},
    {0x250A, "EDGE_TXRX_control.c: invalid block_num"},
    {0x250B, "EDGE_TXRX_control.c: invalid rx_block_num"},
    {0x250C, "EDGE_TXRX_control.c: invalid rx_block_num"},
    {0x250D, "EDGE_TXRX_control.c: invalid header_type"},
    {0x250E, "EDGE_TXRX_control.c: invalid header_type"},
    {0x250F, "EDGE_TXRX_control.c: invalid interleave index"},
    {0x2510, "EDGE_TXRX_control.c: invalid encode_index"},
    {0x2511, "EDGE_TXRX_control.c: invalid encode_index"},
    {0x2512, "EDGE_TXRX_control.c: invalid rx_block_index"},
    {0x2513, "EDGE_TXRX_control.c: invalid channel_type"},
    {0x2514, "EDGE_TXRX_control.c: invalid header_bfi"},
    {0x2515, "EDGE_TXRX_control.c: invalid data_bfi"},
    {0x2516, "EDGE_TXRX_control.c: invalid cs_ps_type"},
    {0x2517, "EDGE_TXRX_control.c: invalid header_type"},
    {0x2518, "EDGE_TXRX_control.c: invalid ts_index"},
    {0x2519, "EDGE_TXRX_control.c: invalid edge_data_decode_task_num"},
    {0x2530, "EDGE_TXRX_control.c: invalid code_scheme"},
    {0x2800, "Edge_rx_chain_module.c: invalid depunc_type"},
    {0x2801, "Edge_rx_chain_module.c: invalid modulation_type"},
    {0x2803, "Edge_rx_chain_module.c: invalid mod_type"},
    {0x2804, "Edge_rx_chain_module.c: invalid header_type"},
    {0x2805, "Edge_rx_chain_module.c: invalid header_type"},
    {0x2C00, "EDGE_IR_module.c: invalid header_type"},
    {0x2C04, "EDGE_IR_module.c: failed in getting IR memory control"},
    {0x2C05, "EDGE_IR_module.c: invalid edge_depunc_size_table"},
    {0x2C06, "EDGE_IR_module.c: current_qbc_value>dma_wait_start_qbc"},

    {0x3000, "EDGE_channel_codec.c: edge_mcs!=EDGE_MCS1234_Head_UL"},
    {0x3001, "EDGE_channel_codec.c: invalid edge_mcs"},
    {0x3002, "EDGE_channel_codec.c: invalid edge_mcs"},
    {0x3003, "EDGE_channel_codec.c: invalid cs_ps_mode"},
    {0x3004, "EDGE_channel_codec.c: invalid edge_mcs"},
    {0x3005, "EDGE_channel_codec.c: invalid cs_ps_mode"},
    {0x3100, "EDGE_BEP_module.c: invalid code scheme"},
    {0x3101, "EDGE_BEP_module.c: temp_val32_2==0"},
    {0x3102, "EDGE_BEP_module.c: temp_val16_1>=21"},

    {0x8000, "frame_eng_MCU.c: FrameInstr_DSPOverRunMCU counter == counter_in"},
    {0x8003, "frame_eng_MCU.c: RespQ_DSPOverRunMCU counter == counter_out"},
    {0x8006, "frame_eng_MCU.c: audio_DMI_messages()"},
    {0x800C, "frame_eng_GPRS_operations.c: func_index > 255"},
    {0x800E, "Tx_encoder.c: tx_channel_type[GL_TX_encode_count] > 0x1c"},
    {0x8010, "Frame_eng_main.c: invalid real_tx_pwrlev"},
    {0x8011, "Frame_eng_main.c: compensate_segment_index > max_segement_index[band]"},
    {0x8012, "frame_eng_GPRS_operations.c: call_index >= 75"},
    {0x807D, "Fcch_detection.c: SEARCH_context.fw_FCCH_burst_mod8_count<-1"},
    {0x80FF, "Event_table.c: invalid tx_burst_type"},
    {0x8866, "Frame_eng_main.c: invalid event_type"},
    {0x8900, "Fcch_detection.c: fcch_detect_status.state<STATE_FCCH_DETECTING || fcch_detect_status.state>STATE_FCCH_DETECTED"},
    {0x8D10, "Run_rx_task.c: dsp_iq_data_save_pingpong() counter_in == counter_out"},
    {0x8D70, "Freq_estimation.c: z<0"},
    {0x8D71, "Freq_estimation.c: x<0"},
    {0x9010, "frame_eng_EDGE_operations.c: rx_data_setting_ptr->rx_slot_number > 5"},
    {0x9011, "frame_eng_EDGE_operations.c:  EDGE_set_event_table() enter conditions not satisfied"},
    {0x9012, "frame_eng_EDGE_operations.c: num_of_slots>=4"},
    {0x9013, "frame_eng_EDGE_operations.c: num_of_slots>=4"},
    {0x9014, "frame_eng_EDGE_operations.c: invalid band_select"},
    {0x901F, "frame_eng_EDGE_operations.c: normal_rx_slot_number > 4"},

    {0xA008, "Run_tone_task.c: Write_UploadVoiceData counter_in == counter_out"},
    {0xA009, "Run_tone_task.c: read_DownloadVoiceData() counter_in == counter_out"},

    {0xD180, "Td_meas_drv_che.c: 0xD180"},
    {0xD183, "Td_meas_drv_che.c: 0xD183"},
    {0xD187, "Td_meas_drv_che.c: 0xD187"},
    {0xD188, "Td_meas_drv_che.c: 0xD188"},
    {0xD455, "Td_meas_main.c: 0xD455"},
    {0xD503, "Td_meas_rft.c: 0xD503"},
    {0xD504, "Td_meas_rft.c: 0xD504"},
    {0xD535, "Td_meas_rft.c: 0xD535"},
    {0xD536, "Td_meas_rft.c: 0xD536"},
    {0xD53C, "Td_meas_rft.c: 0xD53C"},
    {0xD568, "Td_meas_rft.c: 0xD568"},
    {0xD5B1, "Td_meas_rft.c: 0xD5B1"},
    {0xD5B6, "Td_meas_rft.c: 0xD5B6"},
    {0xD5D1, "Td_meas_main.c: 0xD5D1"},
    {0xD5D2, "Td_meas_main.c: 0xD5D2"},
    {0xD5D7, "Td_meas_rft.c: 0xD5D7"},
    {0xD5D8, "Td_meas_rft.c: 0xD5D8"},
    {0xD5D9, "Td_meas_rft.c: 0xD5D9"},
    {0xD5E6, "Td_meas_rft.c: 0xD5E6"},
    {0xD5E7, "Td_meas_rft.c: 0xD5E7"},
    {0xD5E8, "Td_meas_rft.c: 0xD5E8"},
    {0xD5F0, "Td_meas_rft.c: 0xD5F0"},
    {0xD5F8, "Td_meas_rft.c: 0xD5F8"},
    {0xD5F9, "Td_meas_rft.c: 0xD5F9"},
    {0xD5FA, "Td_meas_rft.c: 0xD5FA"},
    {0xD5FB, "Td_meas_rft.c: 0xD5FB"},
    {0xD5FC, "Td_meas_rft.c: 0xD5FC"},
    {0xD5FD, "Td_meas_rft.c: 0xD5FD"},
    {0xD61E, "Td_meas_data_process.c: TP_CS_DWPTS_RETRY_CNT_ERROR"},
    {0xD623, "Td_meas_data_process.c: TP_CS_AGC_CALC_METHOD_ERROR"},
    {0xD647, "Td_meas_control.c: TP_CS_CODE_TYPE_ERROR"},
    {0xD648, "Td_meas_util.c: TP_CS_UTIL_LEN_ERROR"},
    {0xD652, "Td_meas_drv_fir.c: TP_TD_MEAS_FIR_INT_EMPTY"},
    {0xD653, "Td_meas_drv_fir.c: TP_TD_MEAS_FIR_BUF_FULL"},
    {0xD654, "Td_meas_drv_fir.c: TP_TD_MEAS_FIR_BUF_EMPTY"},
    {0xD655, "Td_meas_drv_fir.c: TP_TD_MEAS_TCC_BUF_EMPTY"},
    {0xD658, "Td_meas_drv_fir.c: TP_TD_MEAS_ASSERT_TOTAL_SEG_CNT"},
    {0xD65B, "Td_meas_drv_fir.c: TP_TD_MEAS_ASSERT_FIR_BUF_RW_ERROR"},
    {0xD65C, "Td_meas_drv_fir.c: TP_TD_MEAS_ASSERT_FIR_INT_NODE_ERROR"},
    {0xD661, "Td_meas_task.c: TP_TD_MEAS_ASSERT_FIR_MSG_QUEUE_FULL"},
    {0xD662, "Td_meas_task.c: TP_TD_MEAS_ASSERT_CHE_MSG_QUEUE_FULL"},
    {0xD665, "Td_meas_task.c: TP_TD_MEAS_FIR_QUEUE_W_ERROR"},
    {0xD666, "Td_meas_task.c: TP_TD_MEAS_FIR_QUEUE_R_ERROR"},
    {0xD668, "Td_meas_task.c: TP_TD_MEAS_CHE_QUEUE_W_ERROR"},
    {0xD669, "Td_meas_task.c: TP_TD_MEAS_CHE_QUEUE_R_ERROR"},
    {0xD66B, "Td_meas_task.c: TP_TD_MEAS_MSG_WRONG_BRANCH"},
    {0xD67B, "Td_meas_control.c: TP_TD_MEAS_ASSERT_WRONG_TASK"},
    {0xD67C, "Td_meas_control.c: TP_TD_MEAS_ASSERT_SM_BUFF_INDEX"},
    {0xD67E, "Td_meas_data_process.c: TP_TD_MEAS_ASSERT_INVALID_MEAS_BRACH"},
    {0xD67F, "Td_meas_data_process.c: TP_TD_MEAS_ASSERT_INVALID_FINESYNC_BRACH"},
    {0xD686, "Td_meas_data_process.c: TP_CS_PS_SUM_MAX_ZERO"},
    {0xD693, "Td_meas_data_process.c: TP_TD_MEAS_ASSERT_DEL_CELL_OVERFLOW"},
    {0xD6A3, "Td_meas_control.c: TP_TD_MEAS_ASSERT_INVALID_CODE_NUM"},
    {0xD6A4, "Td_meas_main.c: TP_TD_MEAS_ASSERT_L1C_CMD_ERROR"},
    {0xD6A9, "Td_meas_main.c: TP_TD_MEAS_L1C_FIR_NEED_CNT_ERROR"},
    {0xD6AD, "Td_meas_main.c: TP_TD_MEAS_L1C_CMD_BUFF_IND_ERROR"},
    {0xD6AE, "Td_meas_control.c: TP_CS_SEARCH_MODE_ERROR"},
    {0xD6B8, "Td_meas_drv_che.c: TP_TD_MEAS_CHE_DEL_NUM_ERROR"},
    {0xD6B9, "Td_meas_control.c: TP_TD_MEAS_CODE_FIND_NUM_ERROR"},
    {0xD6FA, "Td_meas_main.c: 0xD6FA"},
    {0xDB90, "Dm_sync_change.c: GL_null_frame_counter >= GSM_MAX_BOOT_FRAME_NUM"},
    {0xDB92, "Dm_sync_change.c: case RAT_CHG_STATUS_T2G2T"},
    {0xDB93, "Dm_sync_change.c: GL_null_frame_counter < 0"},
    {0xDB94, "Dm_sync_change.c: No g_pG2T_share_para->gsm_cell_is_valid[i] == 1"},
    {0xDB95, "Dm_sync_change.c: g_pG2T_share_para->gsm_afc_value[i] == 0"},
    {0xDDB0, "Dm_sync_change.c: RAT_CHG_STATUS_NULL != g_pG2T_share_para->rat_chg_status"},
    {0xDBB1, "Dm_sync_change.c: RAT_CHG_STATUS_T2G != g_pG2T_share_para->rat_chg_status"},
    {0xDBB2, "Dm_sync_change.c: (RAT_CHG_STATUS_T2G != g_pG2T_share_para->rat_chg_status) && (RAT_CHG_STATUS_G2T2G != g_pG2T_share_para->rat_chg_status)"},
    {0xDF01, "Td_meas_control.c: next_rx_ecc_start >= (TD_FRAME_LEN*8)"}

};
/*lint +e64*/

#define DSP_OVERRUN_REQ             0xffff
#define DSP_DEACTIVE_WAKEUP_REQ     0x3
#define DSP_SLEEP_START             0x0
#define DSP_END_WAKEUP              0x2


/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/, for
//       process default interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IrqHandleDefaultFunc (uint32 LogicBit)
{

    CHIPDRV_DisableIRQINT (LogicBit);

    return (uint32) ISR_DONE;
}

/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/,
//  for process default  interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_FiqHandleDefaultFunc (uint32 LogicBit)
{
    INT_PHY_FIQDisable (LogicBit);
    return (uint32) ISR_DONE;
}


/*****************************************************************************/
//  Description:    This function is called when an IRQ happens. Is used to get
//                   interrupt registers status value.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
//  For example, the interrupt status register like this:
//
// intReg1:
//
//     bit00
//     bit01                       bit10
//     bit02                       bit11
//     bit03 ----> intReg2 bit12
//     bit04                       bit13
//
//     IrqUnitRegValue[0] = REG32(addr of intReg1);
//     IrqUnitRegValue[1] = (REG32(addr of intReg1) & (0b)01000) ? REG32(addr of intReg2) : 0;
//
/*****************************************************************************/
PUBLIC void INT_PHY_GetIrqIntStatus (void)
{
    IrqUnitRegValue[0] = REG32 (IrqUnitRegAddr[0]);
    IrqUnitRegValue[1] = REG32 (IrqUnitRegAddr[1]);

    /*FPGA_SUPPORT_SC6531: TB_ANA_INT --BIT_4*/
    if (IrqUnitRegValue[1] & INTCTL_ANA_IRQ)
    {
        IrqUnitRegValue[2] = ANA_REG_GET (IrqUnitRegAddr[2]);
        TM_SendTestPointRequest (0x15170001, IrqUnitRegValue[2]);
    }
    else
    {
        IrqUnitRegValue[2] = 0;
    }

    return;
}

PUBLIC void INT_PHY_GetFiqIntStatus (void)
{
    FiqUnitRegValue[0] = REG32 (FiqUnitRegAddr[0]);
    FiqUnitRegValue[1] = REG32 (FiqUnitRegAddr[1]);
}
/*****************************************************************************/
//  Description:    This function is called when system startup, only init. once.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_Init (void)
{
    INT_PHY_IRQDisableAll ();

    ANA_REG_SET (ANA_INT_EN,0);

    CHIPDRV_EnableIRQINT (TB_ANA_INT);

    return;
}


/*****************************************************************************/
//  Description:    using interrupt ligic number to clear the interrupt.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQClear (uint32 logicNum)
{

}

/*****************************************************************************/
//  Description:    Enable certain interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQEnable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    if (i >= MAX_ISR_NUM)
    {
        CHIP_DRV_PASSERT (0, ("logicNum=%d\n",logicNum)); /*assert verified*/
    }
    else
    {
        INT_DISABLE_ARM_IRQ_FIQ();

        if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_0)
        {
            CHIP_REG_SET (INT0_FIQ_DISABLE, irq_isr_array[i].IntRegBitMask);
            CHIP_REG_OR (INT0_IRQ_EN, irq_isr_array[i].IntRegBitMask);
        }
        else if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_1)
        {
            CHIP_REG_SET (INT1_FIQ_DISABLE, irq_isr_array[i].IntRegBitMask);
            CHIP_REG_OR (INT1_IRQ_EN, irq_isr_array[i].IntRegBitMask);
        }

        if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_2)
        {
            ANA_REG_OR (ANA_INT_EN, irq_isr_array[i].IntRegBitMask);
        }
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        if (logicNum == TB_GPIO_INT)
        {
            ANA_REG_OR (ANA_IRQ_EN, ANA_GPIO_IRQ);
        }
#endif
        INT_ENABLE_ARM_IRQ_FIQ();
    }
}

/*****************************************************************************/
//  Description:    Disable certain interrupt source. It is IRQ interrupt safe.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQDisable (uint32 logicNum)
{
    int i ;
    TX_IRQ_SAVE_AREA

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    if (i>= MAX_ISR_NUM)
    {
        CHIP_DRV_PASSERT (0, ("logicNum=%d\n",logicNum)); /*assert verified*/
    }
    else
    {
        TX_DISABLE_IRQ

        if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_0)
        {
            CHIP_REG_SET (INT0_IRQ_DISABLE, irq_isr_array[i].IntRegBitMask);
        }

        if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_1)
        {
            CHIP_REG_SET (INT1_IRQ_DISABLE, irq_isr_array[i].IntRegBitMask);
        }

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_2)
        {
            ANA_REG_AND (ANA_INT_EN, ~irq_isr_array[i].IntRegBitMask);
        }
#else
        if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_2)
        {
            ANA_REG_AND (ANA_IRQ_EN, ~irq_isr_array[i].IntRegBitMask);
        }

        if (logicNum == TB_GPIO_INT)
        {
            ANA_REG_AND (ANA_INT_EN, (~ANA_GPIO_IRQ));
        }
#endif
        TX_RESTORE_IRQ
    }
}

/*****************************************************************************/
//  Description:    Disable All interrupt source. It is IRQ interrupt safe.
//                       General use in system startup  and debug, and so on.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQDisableAll (void)
{
    CHIP_REG_SET (INT0_IRQ_DISABLE, 0xffffffff);
    CHIP_REG_SET (INT1_IRQ_DISABLE, 0xffffffff);

    return ;
}

/*****************************************************************************/
//  Description:    Get IRQ status, get uint1..group
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetUint1Status()
{
    return REG32 (INT1_IRQ_STS);
}

/*****************************************************************************/
//  Description:    Get IRQ status, get uint0..group
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetUint0Status (void)
{
    return REG32 (INT0_IRQ_STS);
}

/*****************************************************************************/
//  Description:    Get IRQ IE state
//
//  Dependency:
//  Author:         Yong.Li
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetIEState (void)
{
    return REG32 (INT_IRQ_EN);
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQEnable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) fiq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    if (i >= MAX_ISR_NUM)
    {
        CHIP_DRV_PASSERT (0, ("logicNum=%d\n",logicNum)); /*assert verified*/
    }
    else
    {
        INT_DISABLE_ARM_IRQ_FIQ();

        if (fiq_isr_array[i].IntRegValueIndex == ISR_UNIT_0)
        {
            CHIP_REG_SET (INT0_IRQ_DISABLE, fiq_isr_array[i].IntRegBitMask);
            CHIP_REG_OR (INT0_FIQ_EN, fiq_isr_array[i].IntRegBitMask);
        }
        else if (fiq_isr_array[i].IntRegValueIndex == ISR_UNIT_1)
        {
            CHIP_REG_SET (INT1_IRQ_DISABLE, fiq_isr_array[i].IntRegBitMask);
            CHIP_REG_OR (INT1_FIQ_EN, fiq_isr_array[i].IntRegBitMask);
        }

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        if (fiq_isr_array[i].IntRegValueIndex == ISR_UNIT_2)
        {
            ANA_REG_OR (ANA_FIQ_EN, fiq_isr_array[i].IntRegBitMask);
        }
#endif

        INT_ENABLE_ARM_IRQ_FIQ();
    }
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQDisable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) fiq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    if (i>= MAX_ISR_NUM)
    {
        CHIP_DRV_PASSERT (0, ("logicNum=%d\n",logicNum)); /*assert verified*/
    }
    else
    {
        if (fiq_isr_array[i].IntRegValueIndex == ISR_UNIT_0)
        {
            CHIP_REG_SET (INT0_FIQ_DISABLE, fiq_isr_array[i].IntRegBitMask);
        }
        else if (fiq_isr_array[i].IntRegValueIndex == ISR_UNIT_1)
        {
            CHIP_REG_SET (INT1_FIQ_DISABLE, fiq_isr_array[i].IntRegBitMask);
        }

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        if (fiq_isr_array[i].IntRegValueIndex == ISR_UNIT_2)
        {
            ANA_REG_AND (ANA_FIQ_EN, ~fiq_isr_array[i].IntRegBitMask);
        }
#endif

    }
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQDisableAll (void)
{
    CHIP_REG_SET (INT0_FIQ_DISABLE, 0xffffffff);
    CHIP_REG_SET (INT1_FIQ_DISABLE, 0xffffffff);

    return ;
}


PUBLIC void INT_PHYFiqDebug (void)
{
    extern uint32 _tx_pc_val_before_fiq;
    TM_SendTestPointRequest (_tx_pc_val_before_fiq, REG32 (APB_PERI_CTL0));
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
