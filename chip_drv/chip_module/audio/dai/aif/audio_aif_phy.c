/******************************************************************************
 ** File Name:      audio_aif_phy.c                                           *
 ** Author:         rui.huang                                                 *
 ** DATE:           15/6/2022                                                 *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Physical layer of audio aif                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 15/6/2022      rui.huang        Create.                                   *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_api.h"
#include "audio_drvapi.h"
#include "audio_aif_phy.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
#define sprd_audioaif ((REG_AUDIO_AUDIO_AIF_T *)REG_AUDIO_AUDIO_AIF_BASE)

// bcl lrclk ratio limit
#define VOICE_BCK_LRCK_RATIO_MIN 16
#define VOICE_BCK_LRCK_RATIO_MAX 47
#define AUDIO_BCK_LRCK_RATIO_MIN 32
#define AUDIO_BCK_LRCK_RATIO_MAX 94

// aif status
#define AUDIO_AIF_STATUS_CLOSED        0x0    // AIF is closed with clk disabled
#define AUDIO_AIF_STATUS_PREPARED      0x1    // stream param is ready
#define AUDIO_AIF_STATUS_DL_RUNNING    0x2    // playback stream is running
#define AUDIO_AIF_STATUS_UL_RUNNING    0x4    // capture stream is running


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
    AUDIO_AIF_FS_8000 = 0,
    AUDIO_AIF_FS_11025,
    AUDIO_AIF_FS_12000,
    AUDIO_AIF_FS_16000,
    AUDIO_AIF_FS_22050,
    AUDIO_AIF_FS_24000,
    AUDIO_AIF_FS_32000,
    AUDIO_AIF_FS_44100,
    AUDIO_AIF_FS_48000,
    AUDIO_AIF_FS_MAX
} AUDIO_AIF_FS_E;

const uint32 s_aif_fs_tab[AUDIO_AIF_FS_MAX] = {
    8000, 11025, 12000,
    16000, 22050, 24000,
    32000, 44100, 48000};

// =============================================================================
// AUDIO_AIF_FREQ
// -----------------------------------------------------------------------------
/// Defines for readable tone
// =============================================================================
typedef enum
{
    AUDIO_AIF_FREQ_1 = 0,
    AUDIO_AIF_FREQ_2,
    AUDIO_AIF_FREQ_3,
    AUDIO_AIF_FREQ_A,
    AUDIO_AIF_FREQ_4,
    AUDIO_AIF_FREQ_5,
    AUDIO_AIF_FREQ_6,
    AUDIO_AIF_FREQ_B,
    AUDIO_AIF_FREQ_7,
    AUDIO_AIF_FREQ_8,
    AUDIO_AIF_FREQ_9,
    AUDIO_AIF_FREQ_C,
    AUDIO_AIF_FREQ_S,
    AUDIO_AIF_FREQ_0,
    AUDIO_AIF_FREQ_P,
    AUDIO_AIF_FREQ_D,
    AUDIO_AIF_FREQ_MAX
} AUDIO_AIF_FREQ_E;

LOCAL const uint16 aif_tone_freq_table[AUDIO_AIF_FREQ_MAX][2] = {
/*AUDIO_AIF_FREQ_1 */ {AUDIO_AIF_DTMF_FREQ_ROW_697_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1209_HZ},
/*AUDIO_AIF_FREQ_2 */ {AUDIO_AIF_DTMF_FREQ_ROW_697_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1336_HZ},
/*AUDIO_AIF_FREQ_3 */ {AUDIO_AIF_DTMF_FREQ_ROW_697_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1477_HZ},
/*AUDIO_AIF_FREQ_A */ {AUDIO_AIF_DTMF_FREQ_ROW_697_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1633_HZ},
/*AUDIO_AIF_FREQ_4 */ {AUDIO_AIF_DTMF_FREQ_ROW_770_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1209_HZ},
/*AUDIO_AIF_FREQ_5 */ {AUDIO_AIF_DTMF_FREQ_ROW_770_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1336_HZ},
/*AUDIO_AIF_FREQ_6 */ {AUDIO_AIF_DTMF_FREQ_ROW_770_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1477_HZ},
/*AUDIO_AIF_FREQ_B */ {AUDIO_AIF_DTMF_FREQ_ROW_770_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1633_HZ},
/*AUDIO_AIF_FREQ_7 */ {AUDIO_AIF_DTMF_FREQ_ROW_852_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1209_HZ},
/*AUDIO_AIF_FREQ_8 */ {AUDIO_AIF_DTMF_FREQ_ROW_852_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1336_HZ},
/*AUDIO_AIF_FREQ_9 */ {AUDIO_AIF_DTMF_FREQ_ROW_852_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1477_HZ},
/*AUDIO_AIF_FREQ_C */ {AUDIO_AIF_DTMF_FREQ_ROW_852_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1633_HZ},
/*AUDIO_AIF_FREQ_S */ {AUDIO_AIF_DTMF_FREQ_ROW_941_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1209_HZ},
/*AUDIO_AIF_FREQ_0 */ {AUDIO_AIF_DTMF_FREQ_ROW_941_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1336_HZ},
/*AUDIO_AIF_FREQ_P */ {AUDIO_AIF_DTMF_FREQ_ROW_941_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1477_HZ},
/*AUDIO_AIF_FREQ_D */ {AUDIO_AIF_DTMF_FREQ_ROW_941_HZ ,  AUDIO_AIF_DTMF_FREQ_COL_1633_HZ},
};

// clk_cfg
typedef struct
{
    uint32 iis_pll_int_val;
    uint32 clk_div;
} AUDIO_AIF_CLK_T;

typedef struct
{
    AUDIO_AIF_SERIAL_MODE_E mode;

    AUDIO_AIF_MASTER_MODE_E aifIsMaster;

    AUDIO_AIF_LSB_E lsbFirst;

    AUDIO_AIF_LRCK_POL_E polarity;

    AUDIO_AIF_RX_DELAY_E rxDelay;

    AUDIO_AIF_TX_DELAY_E txDelay;

    AUDIO_CH_E channel;

    // LRCK frequency (sampling)
    AUDIO_AIF_FS_E fs_id;

    // BCK/LRCK ratio
    uint32 bckLrckRatio;

    // if Master Mode, invert BCLK out. if slave Mode, invert BCLK in.
    uint8 invertBck;

    // Delay Audio output data or LRCK by half cycle.
    uint8 outputHalfCycleDelay;

    // Delay Audio input data or LRCK by half cycle.
    uint8 inputHalfCycleDelay;

    // Sets the Bck gating. This bit decide if AIF continue to output BCK
    // clock after 16-bit data has been sent.
    uint8 enableBckOutGating;

    AUDIO_AIF_CLK_T clk_cfg;

    // aif is running
    uint32 status;

    // tone is configured
    AUDIO_AIF_TONE_TYPE_E tone;
#if AUDIO_CODEC_TYPE_ES8311
    uint32 mclock_div;
#endif
    // tone is running
    uint8 tone_pasued;
} CFG_AUDIO_AUDIO_AIF_SERIAL_T;

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL CFG_AUDIO_AUDIO_AIF_SERIAL_T s_audioaif_cfg = {
    /*mode                   */ AUDIO_AIF_SERIAL_MODE_I2S_PCM,
    /*aifIsMaster            */ AUDIO_AIF_MASTER_MODE_MASTER,
    /*lsbFirst               */ AUDIO_AIF_LSB_MSB,
    /*polarity               */ AUDIO_AIF_LRCK_POL_LEFT_H_RIGHT_L,
    /*rxDelay                */ AUDIO_AIF_RX_DLY_DLY_1,
    /*txDelay                */ AUDIO_AIF_TX_DLY_DLY_1,
    /*channel                */ AUDIO_CH_MAX,               // set by hal
    /*fs_id                  */ AUDIO_AIF_FS_MAX,           // set by hal
    /*bckLrckRatio           */ 32,
    /*invertBck              */ FALSE,
    /*outputHalfCycleDelay   */ TRUE,
    /*inputHalfCycleDelay    */ FALSE,
    /*enableBckOutGating     */ FALSE,
    /*clk_cfg                */ {0, 0},

    /*status                 */ AUDIO_AIF_STATUS_CLOSED,
    /*tone                   */ AUDIO_AIF_TONE_SILENCE,
    /*tone_paused            */ TRUE,
};

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void __aif_print_cfg(CFG_AUDIO_AUDIO_AIF_SERIAL_T *ptr)
{
    if (ptr != NULL) {
        AUDDRV_DEBUG("/--------aif status------------------/");
        AUDDRV_DEBUG("    mode %d", ptr->mode);
        AUDDRV_DEBUG("    aifIsMaster %d", ptr->aifIsMaster);
        AUDDRV_DEBUG("    lsbFirst %d", ptr->lsbFirst);
        AUDDRV_DEBUG("    polarity %d", ptr->polarity);
        AUDDRV_DEBUG("    rxDelay %d", ptr->rxDelay);
        AUDDRV_DEBUG("    txDelay %d", ptr->txDelay);
        AUDDRV_DEBUG("    channel %d", ptr->channel);
        AUDDRV_DEBUG("    fs_id %d (%dHz)", ptr->fs_id, s_aif_fs_tab[ptr->fs_id]);
        AUDDRV_DEBUG("    invertBck %d", ptr->invertBck);
        AUDDRV_DEBUG("    bckLrckRatio %d", ptr->bckLrckRatio);
        AUDDRV_DEBUG("    outputHalfCycleDelay %d", ptr->outputHalfCycleDelay);
        AUDDRV_DEBUG("    inputHalfCycleDelay %d", ptr->inputHalfCycleDelay);
        AUDDRV_DEBUG("    enableBckOutGating %d", ptr->enableBckOutGating);
        AUDDRV_DEBUG("    status %d", ptr->status);
        AUDDRV_DEBUG("/------------------------------------/");
    }
}

PUBLIC void __aif_print_reg()
{
    uint32 i;
    uint32 *ptr = (uint32 *)REG_AUDIO_AUDIO_AIF_BASE;
    AUDDRV_DEBUG("/--------aif reg---------------------/");
    for (i = 0; i < sizeof(REG_AUDIO_AUDIO_AIF_T)/sizeof(uint32); i++) {
        AUDDRV_DEBUG("    {0x%04x} -- {0x%08x}", i*4, *(ptr+i));
    }
    AUDDRV_DEBUG("/------------------------------------/");
}

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
//data
LOCAL void __aif_data_write(uint32 data0, uint32 data1)
{
    sprd_audioaif->data.dwValue = AUDIO_AIF_DATA0(data0) | AUDIO_AIF_DATA1(data1);
}

//ctrl
LOCAL void __aif_ctrl_clr(void)
{
    // close all bits, clr to reset status
    sprd_audioaif->ctrl.dwValue = 0x0;
}


LOCAL void __aif_ctrl_enable(uint32 en)
{
    if (en) {
        sprd_audioaif->ctrl.mBits.Enable = AUDIO_AIF_ENABLE_ENABLE;
    } else {
        sprd_audioaif->ctrl.mBits.Enable = AUDIO_AIF_ENABLE_DISABLE;
    }
}

LOCAL void __aif_ctrl_txoff(uint32 txoff)
{
    // if you want to do record only,
    // you must set this bit otherwise AIF state machine will not start.
    if (txoff) {
        sprd_audioaif->ctrl.mBits.Tx_Off = AUDIO_AIF_TX_OFF_TX_OFF;
    } else {
        sprd_audioaif->ctrl.mBits.Tx_Off = AUDIO_AIF_TX_OFF_TX_ON;
    }
}

LOCAL void __aif_ctrl_parallel_out_set(uint32 val)
{
    if (val < AUDIO_AIF_PARALLEL_MAX) {
        sprd_audioaif->ctrl.mBits.Parallel_Out_Set = val;
    } else {
        AUDDRV_ERROR("invalid val %d", val);
    }
}

LOCAL void __aif_ctrl_parallel_out_clr(uint32 val)
{
    if (val < AUDIO_AIF_PARALLEL_MAX) {
        sprd_audioaif->ctrl.mBits.Parallel_Out_Clr = val;
    } else {
        AUDDRV_ERROR("invalid val %d", val);
    }

}

LOCAL void __aif_ctrl_parallel_in_set(uint32 val)
{
    if (val < AUDIO_AIF_PARALLEL_MAX) {
        sprd_audioaif->ctrl.mBits.Parallel_In_Set = val;
    } else {
        AUDDRV_ERROR("invalid val %d", val);
    }
}

LOCAL void __aif_ctrl_parallel_in_clr(uint32 val)
{
    if (val < AUDIO_AIF_PARALLEL_MAX) {
        sprd_audioaif->ctrl.mBits.parallel_In_Clr = val;
    } else {
        AUDDRV_ERROR("invalid val %d", val);
    }
}

LOCAL void __aif_ctrl_tx_stb_mode_set(uint32 val)
{
    if (val) {
        sprd_audioaif->ctrl.mBits.Tx_Stb_Mode = AUDIO_AIF_TX_STB_MODE;
    } else {
        sprd_audioaif->ctrl.mBits.Tx_Stb_Mode = 0x0;
    }
}

LOCAL void __aif_ctrl_loopback_set(uint32 loop)
{
    if (loop < AUDIO_AIF_LOOP_BACK_MAX) {
        sprd_audioaif->ctrl.mBits.Loop_back = loop;
    } else {
        AUDDRV_ERROR("invalid loop %d", loop);
    }
}

//serial_ctrl
LOCAL void __aif_serial_serial_mode_set(uint32 mode)
{
    if (mode < AUDIO_AIF_SERIAL_MODE_MAX) {
        sprd_audioaif->serial_ctrl.mBits.SERIAL_MODE = mode;
    } else {
        AUDDRV_ERROR("invalid mode %d", mode);
    }
}

LOCAL void __aif_serial_iis_in_sel_set(uint32 sel)
{
    if (sel < AUDIO_AIF_I2S_IN_SEL_I2S_IN_MAX) {
        sprd_audioaif->serial_ctrl.mBits.I2S_IN_SEL = sel;
    } else {
        AUDDRV_ERROR("invalid sel %d", sel);
    }
}

LOCAL void __aif_serial_iis_master_set(uint32 master)
{
    if (master < AUDIO_AIF_MASTER_MODE_MAX) {
        sprd_audioaif->serial_ctrl.mBits.MASTER_MODE = AUDIO_AIF_MASTER_MODE_MASTER;
    } else {
        AUDDRV_ERROR("invalid master %d", master);
    }
}

LOCAL void __aif_serial_lsb_set(uint32 lsb)
{
    if (lsb < AUDIO_AIF_LSB_MAX) {
        sprd_audioaif->serial_ctrl.mBits.LSB = lsb;
    } else {
        AUDDRV_ERROR("invalid lsb %d", lsb);
    }

}

LOCAL void __aif_serial_lrclk_set(uint32 lrclk)
{
    if (lrclk < AUDIO_AIF_LRCK_POL_MAX) {
        sprd_audioaif->serial_ctrl.mBits.LRCK_Pol = lrclk;
    } else {
        AUDDRV_ERROR("invalid lrclk %d", lrclk);
    }
}

LOCAL void __aif_serial_lr_set(uint32 lr)
{
    if (lr) {
        sprd_audioaif->serial_ctrl.mBits.LR_justified = AUDIO_AIF_LR_JUSTIFIED;
    } else {
        sprd_audioaif->serial_ctrl.mBits.LR_justified = 0;
    }
}

LOCAL void __aif_serial_rx_dly_set(uint32 dly)
{
    if (dly < AUDIO_AIF_RX_DELAY_MAX) {
        sprd_audioaif->serial_ctrl.mBits.Rx_DLY = dly;
    } else {
        AUDDRV_TRACE("invalid dly %d", dly);
    }
}

LOCAL void __aif_serial_tx_dly_set(uint32 dly)
{
    if (dly < AUDIO_AIF_TX_DELAY_MAX) {
        sprd_audioaif->serial_ctrl.mBits.Tx_DLY = dly;
    } else {
        AUDDRV_ERROR("invalid dly %d", dly);
    }
}

LOCAL void __aif_serial_tx_dly_s_set(uint32 dly)
{
    if (dly < AUDIO_AIF_TX_DELAY_S_MAX) {
        sprd_audioaif->serial_ctrl.mBits.Tx_DLY_s = dly;
    } else {
        AUDDRV_ERROR("invalid dly %d", dly);
    }
}

LOCAL void __aif_serial_tx_mode_set(uint32 mode)
{
    if (mode < AUDIO_AIF_TX_MODE_MAX) {
        sprd_audioaif->serial_ctrl.mBits.Tx_Mode = mode;
    } else {
        AUDDRV_ERROR("invalid mode %d", mode);
    }
}

LOCAL void __aif_serial_rx_mode_set(uint32 mode)
{
    if (mode < AUDIO_AIF_RX_MODE_MAX) {
        sprd_audioaif->serial_ctrl.mBits.Rx_Mode = mode;
    } else {
        AUDDRV_ERROR("invalid mode %d", mode);
    }
}

LOCAL void __aif_serial_bck_lrck_set(uint32 bck_lrck)
{
    if (bck_lrck < AUDIO_AIF_BCK_LRCK_BCK_LRCK_MAX) {
        sprd_audioaif->serial_ctrl.mBits.BCK_LRCK = bck_lrck;
    } else {
        AUDDRV_ERROR("invalid bck_lrck %d", bck_lrck);
    }
}

LOCAL void __aif_serial_bck_pol_set(uint8 invert)
{
    if (invert) {
        sprd_audioaif->serial_ctrl.mBits.Bck_Pol = AUDIO_AIF_BCK_POL_INVERT;
    } else {
        sprd_audioaif->serial_ctrl.mBits.Bck_Pol = AUDIO_AIF_BCK_POL_NORMAL;
    }
}

LOCAL void __aif_serial_output_half_cycle_dly_set(uint8 dly)
{
    if (dly) {
        sprd_audioaif->serial_ctrl.mBits.Output_Half_Cycle_DLY =
            AUDIO_AIF_OUTPUT_HALF_CYCLE_DLY_DLY;
    } else {
        sprd_audioaif->serial_ctrl.mBits.Output_Half_Cycle_DLY =
            AUDIO_AIF_OUTPUT_HALF_CYCLE_DLY_NO_DLY;
    }
}

LOCAL void __aif_serial_input_half_cycle_dly_set(uint8 dly)
{
    if (dly) {
        sprd_audioaif->serial_ctrl.mBits.Input_Half_Cycle_DLY =
            AUDIO_AIF_INPUT_HALF_CYCLE_DLY_DLY;
    } else {
        sprd_audioaif->serial_ctrl.mBits.Input_Half_Cycle_DLY =
            AUDIO_AIF_INPUT_HALF_CYCLE_DLY_NO_DLY;
    }
}

LOCAL void __aif_serial_bakout_gate_set(uint8 gate)
{
    if (gate) {
        sprd_audioaif->serial_ctrl.mBits.BckOut_Gate =
            AUDIO_AIF_BCKOUT_GATE_GATED;
    } else {
        sprd_audioaif->serial_ctrl.mBits.BckOut_Gate =
            AUDIO_AIF_BCKOUT_GATE_NO_GATE;
    }
}

//tone
LOCAL void __aif_tone_enable_h_set(uint8 en)
{
    if (en) {
        sprd_audioaif->tone.mBits.Enable_H =
            AUDIO_AIF_ENABLE_H_ENABLE;
    } else {
        sprd_audioaif->tone.mBits.Enable_H =
            AUDIO_AIF_ENABLE_H_DISABLE;
    }
}

LOCAL void __aif_tone_tone_sel_set(uint8 sel)
{
    if (sel) {
        sprd_audioaif->tone.mBits.Tone_Select =
            AUDIO_AIF_TONE_SELECT_COMFORT_TONE;
    } else {
        sprd_audioaif->tone.mBits.Tone_Select =
            AUDIO_AIF_TONE_SELECT_DTMF;
    }
}

LOCAL void __aif_tone_dtmf_freq_col_set(uint32 freq)
{
    if (freq < AUDIO_AIF_DTMF_FREQ_COL_MAX) {
        sprd_audioaif->tone.mBits.DTMF_Freq_Col = freq;
    } else {
        AUDDRV_ERROR("invalid freq %d", freq);
    }
}

LOCAL void __aif_tone_dtmf_freq_row_set(uint32 freq)
{
    if (freq < AUDIO_AIF_DTMF_FREQ_ROW_MAX) {
        sprd_audioaif->tone.mBits.DTMF_Freq_Row = freq;
    } else {
        AUDDRV_ERROR("invalid freq %d", freq);
    }
}

LOCAL void __aif_tone_comfort_freq_set(uint32 freq)
{
    if (freq < AUDIO_AIF_COMFORT_FREQ_MAX) {
        sprd_audioaif->tone.mBits.Comfort_Freq = freq;
    } else {
        AUDDRV_ERROR("invalid freq %d", freq);
    }
}

LOCAL void __aif_tone_gain_set(uint32 gain)
{
    if (gain <= AUDIO_AIF_TONE_GAIN_MAX) {
        sprd_audioaif->tone.mBits.Tone_Gain = gain;
    } else {
        AUDDRV_ERROR("invalid gain %d", gain);
    }
}
//side_tone
LOCAL void __aif_side_tone_gain_set(uint32 gain)
{
    sprd_audioaif->side_tone.mBits.Side_Tone_Gain = AUDIO_AIF_SIDE_TONE_GAIN(gain);
}

//rx_load_pos
LOCAL void __aif_rx_load_pos_set(uint32 pos)
{
    sprd_audioaif->rx_load_pos.mBits.rx_load_position = AUDIO_AIF_RX_LOAD_POSITION(pos);
}

//fm_record_ctrl
LOCAL void __aif_fm_rcd_ctrl_enable_set(uint8 en)
{
    if (en) {
        sprd_audioaif->fm_record_ctrl.mBits.record_en = AUDIO_AIF_RECORD_EN;
    } else {
        sprd_audioaif->fm_record_ctrl.mBits.record_en = 0;
    }
}

//fm_record_ctrl
LOCAL void __aif_fm_rcd_ctrl_lr_swap_set(uint32 swap)
{
    if (swap) {
        sprd_audioaif->fm_record_ctrl.mBits.lr_swap = AUDIO_AIF_LR_SWAP;
    } else {
        sprd_audioaif->fm_record_ctrl.mBits.lr_swap = 0;
    }
}


/**---------------------------------------------------------------------------*
 **                         Function Prototypes for clk                       *
 **---------------------------------------------------------------------------*/
/*
mclk = i2spll/8/4/(cgm_codec_mclock_div_cfg[3:0]+1)
K = i2spll/26m (bit26-20)
N= fraction*2^20=fraction*1048576 (bit19-0)
FS =mclk / clkDiv

//es8374
Best LRCK/BCK ratio: i2spll(500M~1000M)
    Fs BCLK     Per Sample  Clk divider i2spll(div:8)      KN(i2spll/26m)
    8,000      	50          260         832M               0x2000000
    11,025      40          236         832.608M           0x2005FC9
    12,000      38          228         831.744M           0x1FFD7AC
    16,000      56          116         831.488M           0x1FFAF58
    22,050      62          76          831.9168M          0x1FF8178
    24,000      36          120         829.44M            0x1FE6CB4
    32,000      58          56          831.488M           0x1FFAF58
    44,100      42          56          829.7856M          0x1FEA328
    48,000      36          60          829.44M            0x1FE6CB4


//es8311: slave mclk: 1.152M / 1.0584M
    Fs BCLK     Per Sample  Clk divide  i2spll(div:8)       KN(i2spll/26m)
    8,000       64          144         589.824M            0X16AF7F7
    11,025      64          96          541.9008M           0X14D7A37
    12,000      64          96          589.824M            0X16AF7F7
    16,000      64          72          589.824M            0X16AF7F7
    22,050      64          48          541.9008M           0X14D7A37
    24,000      64          48          589.824M            0X16AF7F7
    32,000      64          36          589.824M            0X16AF7F7
    44,100      64          24          541.9008M           0X14D7A37
    48,000      64          24          589.824M            0X16AF7F7

*/


PUBLIC AUDIO_AIF_FS_E __aif_clk_cfg(uint32 fs)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

#if AUDIO_CODEC_TYPE_ES8311
    const uint32 iisPllIntValGrp[9] = {
        0x16AF7F7, 0x14D7A38, 0x16AF7F7,
        0x142A386, 0x14D7A38, 0x16AF7F7,
        0x142A386, 0x1BCA2F5, 0X1E3F54A};
    const uint32 clkDivGrp[9] = {
        288, 192, 192,
        128, 96, 96,
        64, 64, 64};
    const uint32 bckLrckRatioGrp[9] = {
        32, 32, 32,
        32, 32, 32,
        32, 32, 32};
    const uint32 mclock_div_cfg[9] = {
        8, 5,  5,
        3,  2,  2,
        1,  1,  1};
#else
    const uint32 iisPllIntValGrp[9] = {
        0x2000000, 0x2005FC9, 0x1FFD7AC,
        0x1FFAF58, 0x1FF8178, 0x1FE6CB4,
        0x1FFAF58, 0x1FEA328, 0x1FE6CB4};
    const uint32 clkDivGrp[9] = {
        260, 236, 228,
        116, 76,  120,
        56,  56,  60};
    const uint32 bckLrckRatioGrp[9] = {
        50, 40, 38,
        56, 62, 36,
        58, 42, 36};
#endif
    uint32 i = 0;

    for (i = 0; i < AUDIO_AIF_FS_MAX; i++) {
        if (fs == s_aif_fs_tab[i]) {
            cfg->clk_cfg.iis_pll_int_val = iisPllIntValGrp[i];
            cfg->clk_cfg.clk_div = clkDivGrp[i];
            cfg->bckLrckRatio = bckLrckRatioGrp[i];
#if AUDIO_CODEC_TYPE_ES8311
            cfg->mclock_div = mclock_div_cfg[i];
#endif
            if (AUDIO_AIF_SERIAL_MODE_VOICE == cfg->mode) {
                cfg->bckLrckRatio = cfg->bckLrckRatio >> 1;
            }
            return i;
        }
    }

    AUDDRV_ERROR("not support fs %d", fs);
    return AUDIO_AIF_FS_MAX;
}

PUBLIC int32 __aif_clk_enable() {
#if 1 // for debug
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;
    uint32 val;

    REG32(REG_BASE_analogG1 + analog_iis_pll_apll_int_value) = cfg->clk_cfg.iis_pll_int_val;
    REG32(REG_BASE_lpsApb + cfg_plls_clr) = 0x8000;
    SCI_SLEEP(10);//osiDelayUS(1000);
#if AUDIO_CODEC_TYPE_ES8311
    /* if cgm_codec_mclock_sel_cfg 0x2 (i2spll) */
    REG32(REG_BASE_sysCtrl + sysctrl_reg0_set) |= (1 << 11);   //(iis_pll_ref_en)
    /* endif */
#endif

    val = REG32(REG_BASE_aonClk + cgm_codec_mclock_sel_cfg) & 0xFFFFFFF8;
    REG32(REG_BASE_aonClk + cgm_codec_mclock_sel_cfg) = val | 0x2;      //[2:0] (codec mclock_sel)  AUD_CLK_OUT_FREQ_133M(0x2)
    val = REG32(REG_BASE_aonClk + cgm_codec_mclock_div_cfg) & 0xFFFFFFF0;
#if AUDIO_CODEC_TYPE_ES8311
    REG32(REG_BASE_aonClk + cgm_codec_mclock_div_cfg) = val | cfg->mclock_div;      //[3:0] (codec mclock_div)  1 when AUD_CLK_OUT_FREQ_133M； 0 for others
#else
    REG32(REG_BASE_aonClk + cgm_codec_mclock_div_cfg) = val | 0x0;      //[3:0] (codec mclock_div)  1 when AUD_CLK_OUT_FREQ_133M； 0 for others
#endif
    REG32(REG_BASE_sysCtrl + aon_clock_en0_set) |= (1 << 29);           //(codec mclock_en)


    REG32(REG_BASE_sysCtrl + aon_clock_en0_set) = ((1 << 16) | (1 << 9));        //>aon_clock_en0_set |= SYS_CTRL_AIF_EN | SYS_CTRL_AONIFC_EN;
    REG32(REG_BASE_sysCtrl + aon_soft_rst_ctrl0_clr) = (1 << 13);               //>aon_soft_rst_ctrl0_clr = SYS_CTRL_AIF_SOFT_RST

    val = REG32(REG_BASE_aonClk + cgm_i2s_bck_bf_div_sel_cfg) & 0xFFFFFFF8;
    REG32(REG_BASE_aonClk + cgm_i2s_bck_bf_div_sel_cfg) = val | 0x3;             //set [2:0]       AUD_CLK_OUT_FREQ_122_88M (0x3)
    val = REG32(REG_BASE_aonClk + cgm_i2s_bck_bf_div_div_cfg) & 0xFFFFF000;
    REG32(REG_BASE_aonClk + cgm_i2s_bck_bf_div_div_cfg) = val | ((cfg->clk_cfg.clk_div - 1) & 0xFFF);   //set [11:0]      clkDiv-1
    REG32(REG_BASE_sysCtrl + sysctrl_reg0_set) = ((1 << 11) | (1 << 8));        //set bit11|bit8  SYS_CTRL_IIS_PLL_REF_EN | SYS_CTRL_AUD_SCLK_O_PN_SEL
#endif

    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __aif_clk_disable() {
#if 1 // for debug
    REG32(REG_BASE_sysCtrl + aon_soft_rst_ctrl0_set) |= ((1 << 13) | (1 << 17));    //set bit13|bit17 SYS_CTRL_AUDIO_AIF_SOFT_RST | SYS_CTRL_AUD_2AD_SOFT_RST;
    REG32(REG_BASE_sysCtrl + aon_clock_en0_clr) |= ((1 << 16));// | (1 << 9));          //set bit16|bit9  SYS_CTRL_AUDIO_AIF_EN | SYS_CTRL_AONIFC_EN;
    REG32(REG_BASE_sysCtrl + sysctrl_reg0_clr) |= ((1 << 11) | (1 << 8));           //set bit11|bit8  SYS_CTRL_IIS_PLL_REF_EN | SYS_CTRL_AUD_SCLK_O_PN_SEL;

#if AUDIO_CODEC_TYPE_ES8311
    /* if cgm_codec_mclock_sel_cfg 0x2 (i2spll) */
    REG32(REG_BASE_lpsApb + cfg_plls_set) |= (1 << 15);     //set bit15   (iispll_pd_force set)
    /* endif */
#endif

    REG32(REG_BASE_sysCtrl + aon_clock_en0_clr) |= (1 << 29);   //set bit29   (mclock_clr)
#endif
    return AUDIO_HAL_SUCCESS;
}

/**---------------------------------------------------------------------------*
 **                         Function Prototypes for dai                       *
 **---------------------------------------------------------------------------*/
 PUBLIC int32 __aif_can_close(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;
    AUDDRV_TRACE("aif status 0x%04x", cfg->status);
    return (!(AUDIO_AIF_STATUS_DL_RUNNING & cfg->status ||
        AUDIO_AIF_STATUS_UL_RUNNING & cfg->status));
}

PUBLIC int32 __aif_close(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    __aif_ctrl_clr();
    __aif_serial_iis_master_set(AUDIO_AIF_MASTER_MODE_MASTER);
    __aif_side_tone_gain_set(0);

    // disable clk; and aif/ifc reg is unavaliable
    __aif_clk_disable();

    AUDDRV_TRACE("aif close from 0x%04x", cfg->status);

    // init cfg
    cfg->fs_id = AUDIO_AIF_FS_MAX;
    cfg->channel = AUDIO_CH_MAX;
    cfg->status = AUDIO_AIF_STATUS_CLOSED; // clear all

    return AUDIO_HAL_SUCCESS;
}


PUBLIC uint32 __aif_fs_check(uint32 fs) // Hz
{
    uint32 i = 0;
    for (i = 0; i < AUDIO_AIF_FS_MAX; i++) {
        if (fs == s_aif_fs_tab[i]) {
            return fs;
        }
    }
    return 0;
}


// only for HAL_AUDIO_AIF_IF_SERIAL
// open aif based on s_audioaif_cfg
PUBLIC int32 __aif_open(uint32 fs, uint32 channel) // Hz, channel(1/2)
{
    uint32 lrck;
    uint32 bck;
    uint32 bckLrckDivider;
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    // check status
    if (AUDIO_AIF_STATUS_PREPARED & cfg->status) {
        if (fs != s_aif_fs_tab[cfg->fs_id] ||
            channel != cfg->channel) {
            AUDDRV_ERROR("aif opened different %d/%d, %d/%d",
                fs,s_aif_fs_tab[cfg->fs_id],
                channel,cfg->channel);
            return AUDIO_HAL_ERROR;
        } else {
            AUDDRV_TRACE("aif opened same fs %d, channel %d",
                s_aif_fs_tab[cfg->fs_id],
                cfg->channel);
            return AUDIO_HAL_SUCCESS;
        }
    }

    // check channel
    if (AUDIO_CH_MAX <= channel || AUDIO_CH_MONO > channel) {
        AUDDRV_ERROR("invalid channel %d", channel);
        goto __aif_open_fail;
    }

    // check aifIsMaster
    if (AUDIO_AIF_MASTER_MODE_MASTER != cfg->aifIsMaster) {
        AUDDRV_ERROR("need master %d",
            cfg->aifIsMaster);
        goto __aif_open_fail;
    }

    // check fs
    // update clk param based on fs
    cfg->fs_id = __aif_clk_cfg(fs);
    if (AUDIO_AIF_FS_MAX == cfg->fs_id) {
        AUDDRV_ERROR("invalid fs %d", fs);
        goto __aif_open_fail;
    }

    // enable clk
    if (AUDIO_HAL_ERROR == __aif_clk_enable()) {
        AUDDRV_ERROR("__aif_clk_enable failed");
        goto __aif_open_fail;
    }

    // Master
    __aif_serial_iis_master_set(cfg->aifIsMaster);

    // LSB first
    __aif_serial_lsb_set(cfg->lsbFirst);

    // LRCK polarity
    __aif_serial_lrclk_set(cfg->polarity);

    // Rx delay
    __aif_serial_rx_dly_set(cfg->rxDelay);

    // Tx delay
    __aif_serial_tx_dly_set(cfg->txDelay);

    /// serialCfgReg |= AUDIO_AIF_TX_DLY_S_DLY;  //20210302

    // Rx Mode & Tx Mode
    if(AUDIO_CH_MONO == channel) {
        __aif_serial_rx_mode_set(AUDIO_AIF_RX_MODE_STEREO_MONO_FROM_L);
        __aif_serial_tx_mode_set(AUDIO_AIF_TX_MODE_MONO_STEREO_DUPLI);
    } else if(AUDIO_CH_STEREO == channel) {
        __aif_serial_rx_mode_set(AUDIO_AIF_RX_MODE_STEREO_STEREO);
        __aif_serial_tx_mode_set(AUDIO_AIF_TX_MODE_STEREO_STEREO);
    } else {
        AUDDRV_ERROR("invalid channel %d",
            cfg->channel);
        goto __aif_open_fail;
    }
    cfg->channel = channel;

    // The ratio is calculated differently depending
    // on whether we're using voice or audio mode.
    if (AUDIO_AIF_SERIAL_MODE_VOICE == cfg->mode) {
        // Check the ratio
        AUDDRV_DEBUG("In voice mode, "
            "ratio is from %d to %d,bckLrckRatio:%d ",
             VOICE_BCK_LRCK_RATIO_MIN,
             VOICE_BCK_LRCK_RATIO_MAX,
             cfg->bckLrckRatio);

        AUDDRV_PASSERT(
            (cfg->bckLrckRatio >= VOICE_BCK_LRCK_RATIO_MIN &&
            cfg->bckLrckRatio <= VOICE_BCK_LRCK_RATIO_MAX),
            ("Improper bckLrckRatio "));

        bckLrckDivider = cfg->bckLrckRatio - 16;

    } else {
        AUDDRV_DEBUG("In audio mode, "
            "ratio is from %d to %d, bckLrckRatio:%d ",
             AUDIO_BCK_LRCK_RATIO_MIN,
             AUDIO_BCK_LRCK_RATIO_MAX,
             cfg->bckLrckRatio);

        AUDDRV_PASSERT(
            (cfg->bckLrckRatio >= AUDIO_BCK_LRCK_RATIO_MIN &&
            cfg->bckLrckRatio <= AUDIO_BCK_LRCK_RATIO_MAX),
            ("Improper bckLrckRatio "));
        bckLrckDivider = (cfg->bckLrckRatio / 2) - 16;
    }
    __aif_serial_bck_lrck_set(bckLrckDivider);

    // BCK polarity
    __aif_serial_bck_pol_set(cfg->invertBck);

    // Output Half Cycle Delay
    __aif_serial_output_half_cycle_dly_set(cfg->outputHalfCycleDelay);

    // Input Half Cycle Delay
    __aif_serial_input_half_cycle_dly_set(cfg->inputHalfCycleDelay);

    // BckOut gating
    __aif_serial_bakout_gate_set(cfg->enableBckOutGating);

    //if ((config->channel == AUDIO_AIF_CHANNEL_STEREO) || (config->PARALLEL_FOR_FM)) {
    if(AUDIO_CH_STEREO == cfg->channel) {
        __aif_fm_rcd_ctrl_enable_set(1);
    } else {
        __aif_fm_rcd_ctrl_enable_set(0);
    }

    __aif_rx_load_pos_set(AUDIO_AIF_RX_LOAD_POSITION(15));

    cfg->status |= AUDIO_AIF_STATUS_PREPARED;
    AUDDRV_TRACE("aif open success");

    __aif_print_cfg(cfg);
    __aif_print_reg();

    return AUDIO_HAL_SUCCESS;

__aif_open_fail:
    __aif_close();
    return AUDIO_HAL_ERROR;

}

// aif tone stream start after playback stream (with a no data buffer) started
PUBLIC int32 __aif_tone_start(uint32 type, uint32 gain)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;
    // Let's just say that it's not improper to stop
    // the tone even when it's already been stopped, ok ?
    //if (hal_SysGetRequestFreq(HAL_SYS_FREQ_AIF) == HAL_SYS_FREQ_32K)
    //{
        //HAL_ASSERT(FALSE, "AIF Tone when resource not active");
    //}

    AUDDRV_TRACE("aif start TONE now status 0x%04x", cfg->status);

    if(!(AUDIO_AIF_STATUS_DL_RUNNING & cfg->status)) {
        AUDDRV_ERROR("start aif dl for dl not start 0x%04x", cfg->status);
        __aif_playback_start();

    } else if (AUDIO_AIF_TONE_SILENCE != cfg->tone) {
        AUDDRV_ERROR("not start for tone %d is running", cfg->tone);
        /// or stop the prevoius tone?
        return AUDIO_HAL_ERROR;

    } else if (AUDIO_AIF_FS_8000 != cfg->fs_id) {
        AUDDRV_ERROR("not start for fs %d not support", s_aif_fs_tab[cfg->fs_id]);
        return AUDIO_HAL_ERROR;
    }

    //  Setup the proper configuration word.
    // Tone frequency
    switch (type) {
    case AUDIO_AIF_DTMF_0:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_0][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_0][1]);
        break;
    case AUDIO_AIF_DTMF_1:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_1][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_1][1]);
        break;
    case AUDIO_AIF_DTMF_2:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_2][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_2][1]);
        break;
    case AUDIO_AIF_DTMF_3:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_3][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_3][1]);
        break;
    case AUDIO_AIF_DTMF_4:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_4][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_4][1]);
        break;
    case AUDIO_AIF_DTMF_5:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_5][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_5][1]);
        break;
    case AUDIO_AIF_DTMF_6:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_6][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_6][1]);
        break;
    case AUDIO_AIF_DTMF_7:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_7][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_7][1]);
        break;
    case AUDIO_AIF_DTMF_8:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_8][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_8][1]);
        break;
    case AUDIO_AIF_DTMF_9:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_9][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_9][1]);
        break;
    case AUDIO_AIF_DTMF_A:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_A][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_A][1]);
        break;
    case AUDIO_AIF_DTMF_B:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_B][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_B][1]);
        break;
    case AUDIO_AIF_DTMF_C:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_C][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_C][1]);
        break;
    case AUDIO_AIF_DTMF_D:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_D][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_D][1]);
        break;
    case AUDIO_AIF_DTMF_S:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_S][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_S][1]);
        break;
    case AUDIO_AIF_DTMF_P:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_DTMF);
        __aif_tone_dtmf_freq_row_set(aif_tone_freq_table[AUDIO_AIF_FREQ_P][0]);
        __aif_tone_dtmf_freq_col_set(aif_tone_freq_table[AUDIO_AIF_FREQ_P][1]);
        break;
    case AUDIO_AIF_COMFORT_425:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_COMFORT_TONE);
        __aif_tone_comfort_freq_set(AUDIO_AIF_COMFORT_FREQ_425_HZ);
        break;
    case AUDIO_AIF_COMFORT_950:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_COMFORT_TONE);
        __aif_tone_comfort_freq_set(AUDIO_AIF_COMFORT_FREQ_950_HZ);
        break;
    case AUDIO_AIF_COMFORT_1400:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_COMFORT_TONE);
        __aif_tone_comfort_freq_set(AUDIO_AIF_COMFORT_FREQ_1400_HZ);
        break;
    case AUDIO_AIF_COMFORT_1800:
        __aif_tone_tone_sel_set(AUDIO_AIF_TONE_SELECT_COMFORT_TONE);
        __aif_tone_comfort_freq_set(AUDIO_AIF_COMFORT_FREQ_1800_HZ);
        break;
    default:
        AUDDRV_ERROR("not start for invalid tone type %d", type);
        return AUDIO_HAL_ERROR;
        //break;   /*clean warning*/
    }

    //  Setup the gain.
    if (AUDIO_AIF_TONE_GAIN_MAX <= gain && 0 > gain) {
        gain = AUDIO_AIF_TONE_GAIN_0_DB;
    }
    __aif_tone_gain_set(gain);

    // Done during the opening
    // Enable tones
    __aif_tone_enable_h_set(AUDIO_AIF_ENABLE_H_ENABLE);

    // tone is running
    cfg->tone = type;
    cfg->tone_pasued = FALSE;

    AUDDRV_TRACE("aif start tone {type=%d gain=%d} success", type, gain);

    __aif_print_reg();

    return AUDIO_HAL_SUCCESS;
}

// playback stop will lead tone stop
PUBLIC int32 __aif_tone_stop(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    if (AUDIO_AIF_TONE_SILENCE != cfg->tone) {
        __aif_side_tone_gain_set(AUDIO_AIF_TONE_GAIN_M15_DB);
        __aif_tone_enable_h_set(AUDIO_AIF_ENABLE_H_DISABLE);
        AUDDRV_TRACE("aif stop tone {type=%d} success", cfg->tone);
        cfg->tone = AUDIO_AIF_TONE_SILENCE;
        cfg->tone_pasued = TRUE;
    } else {
        AUDDRV_TRACE("no tone is running");
    }
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __aif_tone_pasue(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    if (AUDIO_AIF_TONE_SILENCE != cfg->tone &&
        FALSE == cfg->tone_pasued) {
        __aif_tone_enable_h_set(AUDIO_AIF_ENABLE_H_DISABLE);
        AUDDRV_TRACE("aif pause tone {type=%d}", cfg->tone);
        cfg->tone_pasued = TRUE;
    } else {
        AUDDRV_ERROR("aif cann't pause tone {type=%d state=%d}",
            cfg->tone, cfg->tone_pasued);
        return AUDIO_HAL_ERROR;
    }
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __aif_tone_resume(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;
    if (AUDIO_AIF_TONE_SILENCE != cfg->tone &&
        TRUE == cfg->tone_pasued) {
        __aif_tone_enable_h_set(AUDIO_AIF_ENABLE_H_ENABLE);
        AUDDRV_TRACE("aif resume tone {type=%d}", cfg->tone);
        cfg->tone_pasued = TRUE;
    } else {
        AUDDRV_ERROR("aif cann't resume tone {type=%d state=%d}",
            cfg->tone, cfg->tone_pasued);
        return AUDIO_HAL_ERROR;
    }
    return AUDIO_HAL_SUCCESS;
}

// aif stream start after ifc configured
PUBLIC int32 __aif_playback_start(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    AUDDRV_TRACE("aif start DL now status 0x%04x", cfg->status);

    if (AUDIO_AIF_STATUS_DL_RUNNING & cfg->status) {
        //nothing to do

    } else if (AUDIO_AIF_STATUS_UL_RUNNING & cfg->status) {
        __aif_ctrl_txoff(0);

    } else if (AUDIO_AIF_STATUS_PREPARED & cfg->status) {
        __aif_ctrl_parallel_out_clr(AUDIO_AIF_PARALLEL_PARA);
        __aif_ctrl_parallel_in_clr(AUDIO_AIF_PARALLEL_PARA);
        __aif_ctrl_loopback_set(AUDIO_AIF_LOOP_BACK_NORMAL);

        __aif_ctrl_txoff(0);
        __aif_ctrl_enable(1);

    } else {
        AUDDRV_ERROR("not start for status 0x%04x", cfg->status);
        return AUDIO_HAL_ERROR;
    }

    cfg->status |= AUDIO_AIF_STATUS_DL_RUNNING;
    AUDDRV_TRACE("aif start DL 0x%04x", cfg->status);

    __aif_print_reg();
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __aif_playback_stop(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    if (AUDIO_AIF_STATUS_DL_RUNNING & cfg->status) {
        __aif_ctrl_txoff(1);
        if (!(AUDIO_AIF_STATUS_UL_RUNNING & cfg->status)){
            // no stream running
            __aif_ctrl_parallel_in_set(AUDIO_AIF_PARALLEL_PARA);
            __aif_ctrl_parallel_out_set(AUDIO_AIF_PARALLEL_PARA);
            AUDDRV_TRACE("aif no stream actived");
        }
    }

    if (AUDIO_AIF_TONE_SILENCE != cfg->tone) {
        __aif_tone_stop();
    }

    AUDDRV_TRACE("aif DL stop from 0x%04x", cfg->status);
    cfg->status &= ~AUDIO_AIF_STATUS_DL_RUNNING;

    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __aif_capture_start(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    AUDDRV_TRACE("aif start UL now status 0x%04x", cfg->status);

    if (AUDIO_AIF_STATUS_UL_RUNNING & cfg->status) {
        //nothing to do

    } else if (AUDIO_AIF_STATUS_DL_RUNNING & cfg->status) {
        //nothing to do

    } else if (AUDIO_AIF_STATUS_PREPARED & cfg->status) {
        __aif_ctrl_parallel_out_clr(AUDIO_AIF_PARALLEL_PARA);
        __aif_ctrl_parallel_in_clr(AUDIO_AIF_PARALLEL_PARA);
        __aif_ctrl_loopback_set(AUDIO_AIF_LOOP_BACK_NORMAL);

        __aif_ctrl_txoff(1);
        __aif_ctrl_enable(1);

        // clear data
        __aif_data_write(0, 0);
        __aif_data_write(0, 0);
        __aif_data_write(0, 0);
        __aif_data_write(0, 0);

    } else {
        AUDDRV_ERROR("not start for status 0x%04x", cfg->status);
        return AUDIO_HAL_ERROR;
    }

    cfg->status |= AUDIO_AIF_STATUS_UL_RUNNING;
    AUDDRV_TRACE("aif start UL 0x%04x", cfg->status);

    __aif_print_reg();
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __aif_capture_stop(void)
{
    CFG_AUDIO_AUDIO_AIF_SERIAL_T *cfg = &s_audioaif_cfg;

    if (AUDIO_AIF_STATUS_UL_RUNNING & cfg->status) {
        if (!(AUDIO_AIF_STATUS_DL_RUNNING & cfg->status)) {
            __aif_ctrl_parallel_in_set(AUDIO_AIF_PARALLEL_PARA);
            __aif_ctrl_parallel_out_set(AUDIO_AIF_PARALLEL_PARA);
            AUDDRV_TRACE("aif no stream actived");
        }
    }

    AUDDRV_TRACE("aif UL stop from 0x%04x",cfg->status);
    cfg->status &= ~AUDIO_AIF_STATUS_UL_RUNNING;

    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __aif_ctrl_xun_status_get(void)
{
    return (sprd_audioaif->ctrl.mBits.out_underflow |
            sprd_audioaif->ctrl.mBits.in_overflow);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
