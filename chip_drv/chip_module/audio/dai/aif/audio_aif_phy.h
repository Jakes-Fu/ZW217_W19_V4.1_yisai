/******************************************************************************
 ** File Name:      vbc_phy_v6.h                                              *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/
#ifndef _AUDIO_AUDIO_AIF_PHY_H_
#define _AUDIO_AUDIO_AIF_PHY_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#include "audio_drvapi.h"


/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////
// clk for debug
#define REG_BASE_lpsApb     0x51705000
#define cfg_plls_clr        0x0000084c
#define cfg_plls_set        0x0000044c

#define REG_BASE_aonClk             0x51508800
#define cgm_i2s_bck_bf_div_sel_cfg  0x00000070
#define cgm_i2s_bck_bf_div_div_cfg  0x0000006c
#define cgm_codec_mclock_sel_cfg    0x00000064
#define cgm_codec_mclock_div_cfg    0x00000060

#define REG_BASE_analogG1               0x51501000
#define analog_iis_pll_apll_int_value   0x00000048

#define REG_BASE_sysCtrl        0x51500000
#define sysctrl_reg0_clr        0x000008a8
#define aon_clock_en0_clr       0x00000820
#define aon_soft_rst_ctrl0_clr  0x00000800
#define sysctrl_reg0_set        0x000004a8
#define sysctrl_reg0_set        0x000004a8
#define aon_clock_en0_set       0x00000420
#define aon_soft_rst_ctrl0_set  0x00000400

///////////////////////////////////////////////////////////////////////////////

#define REG_AUDIO_AUDIO_AIF_BASE 0x5140A000

//data
#define AUDIO_AIF_DATA0(n) (((n)&0xFFFF) << 0)
#define AUDIO_AIF_DATA1(n) (((n)&0xFFFF) << 16)

//ctrl
#define AUDIO_AIF_ENABLE_DISABLE 0
#define AUDIO_AIF_ENABLE_ENABLE 1

#define AUDIO_AIF_TX_OFF_TX_ON 0
#define AUDIO_AIF_TX_OFF_TX_OFF 1

typedef enum
{
    AUDIO_AIF_PARALLEL_SERL,
    AUDIO_AIF_PARALLEL_PARA,
    AUDIO_AIF_PARALLEL_MAX
} AUDIO_AIF_PARALLEL_E;

#define AUDIO_AIF_TX_STB_MODE 1

typedef enum
{
    AUDIO_AIF_LOOP_BACK_NORMAL,
    AUDIO_AIF_LOOP_BACK_LOOPBACK,
    AUDIO_AIF_LOOP_BACK_MAX
} AUDIO_AIF_LOOP_BACK_E;

//serial_ctrl
typedef enum
{
    AUDIO_AIF_SERIAL_MODE_I2S_PCM = 0,
    AUDIO_AIF_SERIAL_MODE_VOICE,
    AUDIO_AIF_SERIAL_MODE_DAI,
    AUDIO_AIF_SERIAL_MODE_MAX
} AUDIO_AIF_SERIAL_MODE_E;

typedef enum
{
    AUDIO_AIF_I2S_IN_SEL_I2S_IN_0,
    AUDIO_AIF_I2S_IN_SEL_I2S_IN_1,
    AUDIO_AIF_I2S_IN_SEL_I2S_IN_2,
    AUDIO_AIF_I2S_IN_SEL_I2S_IN_MAX
} AUDIO_AIF_I2S_IN_E;

typedef enum
{
    AUDIO_AIF_MASTER_MODE_SLAVE,
    AUDIO_AIF_MASTER_MODE_MASTER,
    AUDIO_AIF_MASTER_MODE_MAX
} AUDIO_AIF_MASTER_MODE_E;

typedef enum
{
    AUDIO_AIF_LSB_MSB,
    AUDIO_AIF_LSB_LSB,
    AUDIO_AIF_LSB_MAX
} AUDIO_AIF_LSB_E;

typedef enum
{
    AUDIO_AIF_LRCK_POL_LEFT_H_RIGHT_L,
    AUDIO_AIF_LRCK_POL_LEFT_L_RIGHT_H,
    AUDIO_AIF_LRCK_POL_MAX
} AUDIO_AIF_LRCK_POL_E;

#define AUDIO_AIF_LR_JUSTIFIED 1

typedef enum
{
    /// Indicates the delay between serial data in MSB and LRCK edge.
    /// "00" = Digital audio in MSB is aligned with LRCLK edge.
    AUDIO_AIF_RX_DLY_ALIGN = 0,
    /// "01" = Digital audio in MSB is 1 cycle delayed to LRCLK edge.
    AUDIO_AIF_RX_DLY_DLY_1,
    /// "10" = Digital audio in MSB is 2 cycle delayed to LRCLK edge.
    AUDIO_AIF_RX_DLY_DLY_2,
    /// "11" = Digital audio in MSB is 3 cycle delayed to LRCLK edge.
    AUDIO_AIF_RX_DLY_DLY_3,

    AUDIO_AIF_RX_DELAY_MAX
} AUDIO_AIF_RX_DELAY_E;

typedef enum
{
    /// Indicates the delay between serial data out MSB and LRCK edge.
    /// "00" = Digital audio out MSB is aligned with LRCLK edge.
    AUDIO_AIF_TX_DLY_ALIGN,
    /// "01" = Digital audio out MSB is 1 cycle delayed to LRCLK edge.
    AUDIO_AIF_TX_DLY_DLY_1,

    AUDIO_AIF_TX_DELAY_MAX
} AUDIO_AIF_TX_DELAY_E;

typedef enum
{
    AUDIO_AIF_TX_DLY_S_NO_DLY,
    AUDIO_AIF_TX_DLY_S_DLY,
    AUDIO_AIF_TX_DELAY_S_MAX
} AUDIO_AIF_TX_DELAY_S_E;

typedef enum
{
    /// Configure mono or stereo format for Audio data out.
    /// if data from IFC is stereo and AIF works in DAI or Voice mode,  the left
    /// channel is always selected.

    /// Stereo input from IFC, stereo output to pin.
    AUDIO_AIF_TX_MODE_STEREO_STEREO,
    /// Mono input from IFC, stereo output in left channel to pin
    AUDIO_AIF_TX_MODE_MONO_STEREO_CHAN_L,
    /// Mono input from IFC, stereo output duplicate in both channels to pin.
    AUDIO_AIF_TX_MODE_MONO_STEREO_DUPLI,

    AUDIO_AIF_TX_MODE_STEREO_TO_MONO,

    AUDIO_AIF_TX_MODE_MAX
} AUDIO_AIF_TX_MODE_E;

typedef enum
{
    /// Configure mono or stereo format for Audio data in.
    /// Stereo input from pin, stereo output to IFC.
    AUDIO_AIF_RX_MODE_STEREO_STEREO,
    /// Stereo input from pin, mono input to IFC selected from left channel.
    AUDIO_AIF_RX_MODE_STEREO_MONO_FROM_L,

    AUDIO_AIF_RX_MODE_MAX
} AUDIO_AIF_RX_MODE_E;

typedef enum
{
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_16,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_17,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_18,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_19,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_20,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_21,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_22,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_23,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_24,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_25,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_26,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_27,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_28,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_29,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_30,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_31,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_32,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_33,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_34,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_35,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_36,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_37,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_38,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_39,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_40,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_41,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_42,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_43,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_44,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_45,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_46,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_47,
    AUDIO_AIF_BCK_LRCK_BCK_LRCK_MAX
} AUDIO_AIF_BCK_LRCK_E;

#define AUDIO_AIF_BCK_POL_NORMAL 0
#define AUDIO_AIF_BCK_POL_INVERT 1

#define AUDIO_AIF_OUTPUT_HALF_CYCLE_DLY_NO_DLY 0
#define AUDIO_AIF_OUTPUT_HALF_CYCLE_DLY_DLY 1

#define AUDIO_AIF_INPUT_HALF_CYCLE_DLY_NO_DLY 0
#define AUDIO_AIF_INPUT_HALF_CYCLE_DLY_DLY 1

#define AUDIO_AIF_BCKOUT_GATE_NO_GATE 0
#define AUDIO_AIF_BCKOUT_GATE_GATED 1

//tone
#define AUDIO_AIF_ENABLE_H_DISABLE 0
#define AUDIO_AIF_ENABLE_H_ENABLE 1

#define AUDIO_AIF_TONE_SELECT_DTMF 0
#define AUDIO_AIF_TONE_SELECT_COMFORT_TONE 1

typedef enum
{
    AUDIO_AIF_DTMF_FREQ_COL_1209_HZ,
    AUDIO_AIF_DTMF_FREQ_COL_1336_HZ,
    AUDIO_AIF_DTMF_FREQ_COL_1477_HZ,
    AUDIO_AIF_DTMF_FREQ_COL_1633_HZ,
    AUDIO_AIF_DTMF_FREQ_COL_MAX
}AUDIO_AIF_DTMF_FREQ_COL_E;

typedef enum
{
    AUDIO_AIF_DTMF_FREQ_ROW_697_HZ,
    AUDIO_AIF_DTMF_FREQ_ROW_770_HZ,
    AUDIO_AIF_DTMF_FREQ_ROW_852_HZ,
    AUDIO_AIF_DTMF_FREQ_ROW_941_HZ,
    AUDIO_AIF_DTMF_FREQ_ROW_MAX
}AUDIO_AIF_DTMF_FREQ_ROW_E;

typedef enum
{
    AUDIO_AIF_COMFORT_FREQ_425_HZ,
    AUDIO_AIF_COMFORT_FREQ_950_HZ,
    AUDIO_AIF_COMFORT_FREQ_1400_HZ,
    AUDIO_AIF_COMFORT_FREQ_1800_HZ,
    AUDIO_AIF_COMFORT_FREQ_MAX
}AUDIO_AIF_COMFORT_FREQ_E;

//side_tone
#define AUDIO_AIF_SIDE_TONE_GAIN(n) ((n)&15)

//rx_load_pos
#define AUDIO_AIF_RX_LOAD_POSITION(n) ((n)&15)

//fm_record_ctrl
#define AUDIO_AIF_RECORD_EN 1
#define AUDIO_AIF_LR_SWAP 1

#ifndef REG32
#define REG32(x)               (*((volatile uint32 *)(x)))
#endif

/**---------------------------------------------------------------------------*
 **                         struct defination                                 *
 **---------------------------------------------------------------------------*/

// 0x00000000
typedef union _aif_data_tag {
    struct _aif_data_map {
            // contains data to be read or written by IFC
        volatile unsigned int data0     :16;    //[15:0]
            // In mono mode, data1 is after data0.
            // In stereo mode, data1 is in right channel.
        volatile unsigned int data1     :16;    //[31:16]
            // In mono mode, data0 is before data1.
            // In stereo mode, data0 is in left channel.
    } mBits;
    volatile unsigned int dwValue;
} AUDIO_AIF_DATA_U;

// 0x00000004
typedef union _aif_ctrl_tag {
    struct _aif_ctrl_map {
        volatile unsigned int Enable            :1;     //[0]
            // Audio Interface Enable
            // if AUDIO_AIF_Tone[0] is also 0, AIF is disabled.
            // If AUDIO_AIF_Tone[0] is also '1', Tx fifo continue to fetch and distribute data from IFC when tone is enable. However, these data are not used.
        volatile unsigned int reserved0         :3;     //[3:1]
        volatile unsigned int Tx_Off            :1;     //[4]
            // if you want to do record only, you must set this bit otherwise AIF state machine will not start.
            // 0 = Both Tx Rx enabled.
            // 1 = Rx enabled only, Tx disabled.
        volatile unsigned int reserved1         :3;     //[7:5]
        volatile unsigned int Parallel_Out_Set  :1;     //[8]
            // parallel Selects parallel audio interface connected to analog front-end.
            // 0 = serial; 1 = parallel;
        volatile unsigned int Parallel_Out_Clr  :1;     //[9]
        volatile unsigned int Parallel_In_Set   :1;     //[10]
        volatile unsigned int parallel_In_Clr   :1;     //[11]
        volatile unsigned int Tx_Stb_Mode       :1;     //[12]
            // In parallel mode, select AIF Tx Strobe mode. Reserved in serial mode.
            // 0 = Tx STB edge is in middle of data.
            // 1 = Tx STB edge is aligned to data edge.
        volatile unsigned int reserved2         :3;     //[15:13]
        volatile unsigned int out_underflow     :1;     //[16]
            // This bit indicates if the AIF had needed some data while the Out Fifo was empty.
            // In case of data famine, the last available data will be sent again.
            // Write one to clear the out_underflow status bit. This bit is auto clear.
        volatile unsigned int in_overflow       :1;     //[17]
            // This bit indicates if the AIF had received some data while the Input Fifo was full.
            // If the input Fifo is full, the newly received data will be lost.
            // Write one to clear the in_overflow status bit. This bit is auto clear.
        volatile unsigned int reserved3         :13;    //[30:18]
        volatile unsigned int Loop_back         :1;     //[31]
            // 0      NORMAL
            // 1      LOOPBACK. The feature is for debug only and can not work in DAI mode.
    } mBits;
    volatile unsigned int dwValue;
} AUDIO_AIF_CTRL_U;

// 0x00000008
typedef union _aif_serial_ctrl_tag {
    struct _aif_serial_ctrl_map {
        volatile unsigned int SERIAL_MODE               :2;     //[1:0]
            // 0      I2S_PCM
            // 1      VOICE
            // 2      DAI Configure serial AIF mode. "11" is reserved.
        volatile unsigned int I2S_IN_SEL                :2;     //[3:2]
            // 0      I2S IN 0
            // 1      I2S IN 1
            // 2      I2S IN 2 Select AIF I2S input.
        volatile unsigned int MASTER_MODE               :1;     //[4]
            // 0      SLAVE
            // 1      MASTER
        volatile unsigned int LSB                       :1;     //[5]
            // 0      MSB
            // 1      LSB When high, the output data format is with the least significant bit first.
        volatile unsigned int LRCK_Pol                  :1;     //[6]
            // 0 = high level on LRCK means left channel, low level on LRCK means right channel.
            // 1 = high level on LRCK means right channel, low level on LRCK means left channel.
            // Note: this bit should be set to '0' (LEFT_H_RIGHT_L) in voice mode.
        volatile unsigned int LR_justified              :1;     //[7]
            // Reserved
        volatile unsigned int Rx_DLY                    :2;     //[9:8]
            // "00" = Digital audio in MSB is aligned with LRCLK edge.
            // "01" = Digital audio in MSB is 1 cycle delayed to LRCLK edge.
            // "10" = Digital audio in MSB is 2 cycle delayed to LRCLK edge.
            // "11" = Digital audio in MSB is 3 cycle delayed to LRCLK edge.
        volatile unsigned int Tx_DLY                    :1;     //[10]
            // "0" = Digital audio out MSB is aligned with LRCLK edge.
            // "1" = Digital audio out MSB is 1 cycle delayed to LRCLK edge.
        volatile unsigned int Tx_DLY_s                  :1;     //[11]
            // "0" = No supplementary Tx delay.
            // "1" = One Cycle supplementary Tx delay.
        volatile unsigned int Tx_Mode                   :2;     //[13:12]
            // "00" = stereo input from IFC, stereo output to pin.
            // "01" = mono input from IFC, stereo output in left channel to pin.
            // This value is reserved in parallel EXT mode.
            // "10" = mono input from IFC, stereo output duplicate in both channels to pin.
            // "11" = stereo input from IFC, mono output to left and right channel. This mode is only used for parallel stereo interface.
            // if AIF works in DAI or Voice mode, always select "00" mode STEREO_STEREO.
        volatile unsigned int Rx_Mode                   :1;     //[14]
            // 0 = stereo input from pin, stereo output to IFC.
            // 1 = stereo input from pin, mono input to IFC selected from left channel.
            // Users can change LRCK polarity to choose mono input from right channel.
        volatile unsigned int reserved0                 :1;     //[15]
        volatile unsigned int BCK_LRCK                  :5;     //[20:16]
            // configure the ratio of BCK and LRCK cycle from 16 to 47.
            // Voice_Mode: "XXXX": each sample takes 16 + "XXXX" BCLK cycle.
            // Audio_Mode: "XXXX": each sample takes 2*(16 + "XXXX") BCLK cycle. 2 times than Voice Mode because in audio mode each sample occupies two channels.
        volatile unsigned int reserved1                 :3;     //[23:21]
        volatile unsigned int Bck_Pol                   :1;     //[24]
            // 0      NORMAL
            // 1      INVERT if Master Mode, invert BCLK out. if slave Mode, invert BCLK in.
        volatile unsigned int Output_Half_Cycle_DLY     :1;     //[25]
            // 0      NO DLY
            // 1      DLY delayed Audio output data or LRCK by half cycle.
        volatile unsigned int Input_Half_Cycle_DLY      :1;     //[26]
            // 0      NO DLY
            // 1      DLY delayed Audio input data by half cycle.
        volatile unsigned int reserved2                 :1;     //[27]
        volatile unsigned int BckOut_Gate               :1;     //[28]
            // 0      NO GATE
            // 1      GATED Sets the BckOut gating. This bit decide if AIF continue to output BCK clock after 16-bit data has been sent.
        volatile unsigned int reserved3                 :3;     //[31:29]
    } mBits;
    volatile unsigned int dwValue;
} AUDIO_AIF_SERIAL_CTRL_U;

// 0x0000000C
typedef union _aif_tone_tag {
    struct _aif_tone_map {
        volatile unsigned int Enable_H      :1;     //[0]
            // When this bit is set, the audio interface is enabled and a comfort tone or DTMF tone is output
            // on the audio interface instead of the regular data, even if the AUDIO_AIF_CTRL[0] enable bit is 0.
            // 0 = AIF is disabled if the AUDIO_AIF_CTRL[0] is also 0.
            // 1 = AIF is enabled and generates a tone.
        volatile unsigned int Tone_Select   :1;     //[1]
            // Select whether a DTMF of a comfort tone is generated
            // 0     DTMF
            // 1     COMFORT TONE.
        volatile unsigned int reserved0     :2;     //[3:2]
        volatile unsigned int DTMF_Freq_Col :2;     //[5:4]
            // Frequency of the first DTMF sine wave
            // 0      1209 Hz
            // 1      1336 Hz
            // 2      1477 Hz
            // 3      1633 Hz.
        volatile unsigned int DTMF_Freq_Row :2;     //[7:6]
            // Frequency of the second DTMF sine wave
            // 0      697 Hz
            // 1      770 Hz
            // 2      852 Hz
            // 3      941 Hz.
        volatile unsigned int Comfort_Freq  :2;     //[9:8]
            // Frequency of comfort tone
            // 0      425 Hz
            // 1      950 Hz
            // 2      1400 Hz
            // 3      1800 Hz.
        volatile unsigned int reserved1     :2;     //[11:10]
        volatile unsigned int Tone_Gain     :2;     //[13:12]
            // Tone attenuation. The Comfort Tone or DTMF is attenuated according to this programmable gain.
            // 0      0 dB
            // 1      -3 dB
            // 2      -9 dB
            // 3      -15 dB
        volatile unsigned int reserved2     :18;    //[31:14]
    } mBits;
    volatile unsigned int dwValue;
} AUDIO_AIF_TONE_U;

// 0x00000010
typedef union _aif_side_tone_tag {
    struct _aif_side_tone_map {
        volatile unsigned int Side_Tone_Gain    :4;     //[3:0]
            // Side Tone attenuation. The side tone is attenuated according to this programmable gain.
            //    0000 = mute.
            //    0001 = -36 dB.    0010 = -33 dB.    0011 = -30 dB.    0100 = -27 dB.
            //    0101 = -24 dB.    0110 = -21 dB.    0111 = -18 dB.    1000 = -15 dB.
            //    1001 = -12 dB.    1010 = -9 dB.     1011 = -6 dB.     1100 = -3 dB.
            //    1101 = 0 dB.      1110 = +3 dB.     1111 = +6 dB.
        volatile unsigned int reserved0         :28;    //[31:4]
    } mBits;
    volatile unsigned int dwValue;
} AUDIO_AIF_SIDE_TONE_U;

// 0x00000014
typedef union _aif_rx_load_pos_tag {
    struct _aif_rx_load_pos_map {
        volatile unsigned int rx_load_position      :4;     //[3:0]
            // set rx load position delay, the range is 0 to 15.
        volatile unsigned int reserved0             :28;    //[31:4]
    } mBits;
    volatile unsigned int dwValue;
} AUDIO_AIF_RX_LOAD_POS_U;

// 0x00000018
typedef union _aif_fm_rcd_ctrl_tag {
    struct _aif_fm_rcd_ctrl_map {
        volatile unsigned int record_en     :1;     //[0]
        volatile unsigned int reserved0     :3;     //[3:1]
        volatile unsigned int lr_swap       :1;     //[4]
            // "1" swap fm left and right channel.
        volatile unsigned int reserved1     :27;    //[31:5]
    } mBits;
    volatile unsigned int dwValue;
} AUDIO_AIF_FM_RCD_CTRL_U;

// REG_AUDIO_AUDIO_AIF_BASE
typedef volatile struct
{
    AUDIO_AIF_DATA_U          data;           //0x00000000
    AUDIO_AIF_CTRL_U          ctrl;           //0x00000004
    AUDIO_AIF_SERIAL_CTRL_U   serial_ctrl;    //0x00000008
    AUDIO_AIF_TONE_U          tone;           //0x0000000C
    AUDIO_AIF_SIDE_TONE_U     side_tone;      //0x00000010
    AUDIO_AIF_RX_LOAD_POS_U   rx_load_pos;    //0x00000014
    AUDIO_AIF_FM_RCD_CTRL_U   fm_record_ctrl; //0x00000018
} REG_AUDIO_AUDIO_AIF_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC int32 __aif_can_close(void);
PUBLIC uint32 __aif_fs_check(uint32 fs);

PUBLIC int32 __aif_close(void);
PUBLIC int32 __aif_open(uint32 fs, uint32 channel);

PUBLIC int32 __aif_tone_start(uint32 type, uint32 gain);
PUBLIC int32 __aif_tone_stop(void);
PUBLIC int32 __aif_tone_pasue(void);
PUBLIC int32 __aif_tone_resume(void);

PUBLIC int32 __aif_playback_start(void);
PUBLIC int32 __aif_playback_stop(void);

PUBLIC int32 __aif_capture_start(void);
PUBLIC int32 __aif_capture_stop(void);

PUBLIC int32 __aif_ctrl_xun_status_get(void);

PUBLIC void __aif_print_reg();
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_AUDIO_AUDIO_AIF_PHY_H_
