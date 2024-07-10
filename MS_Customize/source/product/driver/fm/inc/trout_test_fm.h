#include "_hardware.h"


#ifndef _TROUT_TEST_FM_H_
#define _TROUT_TEST_FM_H_


#define RDS_BUF_SIZE	1024
#define RDS_READ_CNT	RDS_BUF_SIZE/8
#define FM_MAX_RFEQ		1080
#define FM_MIN_RFEQ		875

typedef struct
{
	uint32 chan;
	uint32 seek;
}fm_chan_info_t;

//------------------------- Data Prototype ------------------------------------
typedef union _fm_ctrl_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _fm_ctrl_map
    {
        volatile unsigned int no_lpf               :  2 ; // [ 1 :  0] 
        volatile unsigned int reserved_2           :  2 ; // [ 3 :  2] 
        volatile unsigned int dem                  :  2 ; // [ 5 :  4] 
        volatile unsigned int reserved_6           : 10 ; // [15 :  6] 
        volatile unsigned int rxm                  :  1 ; // [     16] 
        volatile unsigned int pol_dcon             :  1 ; // [     17] 
        volatile unsigned int ster_swap            :  1 ; // [     18] 
        volatile unsigned int seek_up              :  1 ; // [     19] 
        volatile unsigned int reserved_20          : 12 ; // [31 : 20] 
    } mBits ;

#else

    struct _fm_ctrl_map
    {
        volatile unsigned int reserved_20          : 12 ; // [31 : 20] 
        volatile unsigned int seek_up              :  1 ; // [     19] 
        volatile unsigned int ster_swap            :  1 ; // [     18] 
        volatile unsigned int pol_dcon             :  1 ; // [     17] 
        volatile unsigned int rxm                  :  1 ; // [     16] 
        volatile unsigned int reserved_6           : 10 ; // [15 :  6] 
        volatile unsigned int dem                  :  2 ; // [ 5 :  4] 
        volatile unsigned int reserved_2           :  2 ; // [ 3 :  2] 
        volatile unsigned int no_lpf               :  2 ; // [ 1 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} FM_CTRL_U ;


//-----------------------------------------------------------------------------
// Register name             : FM_EN
// Register offset           : 0x0004
// Register description      : FM Enable Control Register
// Register field description:
//    fld_glben            : [     31] FM Global Enable bit. 
//    fld_adc_1bit         : [     13] ADC valid bits
//                                     0: 1.5bit
//                                     1: 1bit 
//    fld_aud_bld          : [     12] Audio blend enable 
//    fld_sw_mute          : [     11] Software mute enable 
//    fld_hw_mute          : [     10] Hardware mute enable. 
//    fld_adp_lpf          : [      9] Adaptive configuration ADC decimation LPF enable 
//    fld_plt_dect         : [      7] Pilot Detect enable 
//    fld_foc              : [      6] Frequency Offset Cancel enable 
//    fld_fom              : [      5] Frequency Offset Measure enable 
//    fld_agc_adps         : [      4] AGC Adaptive scaling pre-demodulation enable 
//    fld_agc              : [      3] AGC enable 
//    fld_rssi             : [      2] RSSI enable 
//    fld_dcc              : [      1] DC Offset Cancel enable 
//    fld_seek_adps        : [      0] Seek Adaptive scaling pre-demodulation enable 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define FM_EN_SEEK_ADPS                          BIT_0
#define FM_EN_DCC                                BIT_1
#define FM_EN_RSSI                               BIT_2
#define FM_EN_AGC                                BIT_3
#define FM_EN_AGC_ADPS                           BIT_4
#define FM_EN_FOM                                BIT_5
#define FM_EN_FOC                                BIT_6
#define FM_EN_PLT_DECT                           BIT_7
#define FM_EN_ADP_LPF                            BIT_9
#define FM_EN_HW_MUTE                            BIT_10
#define FM_EN_SW_MUTE                            BIT_11
#define FM_EN_AUD_BLD                            BIT_12
#define FM_EN_ADC_1BIT                           BIT_13
#define FM_EN_GLBEN                              BIT_31

//------------------------- Data Prototype ------------------------------------
typedef union _fm_en_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE
    struct _fm_en_map
    {
        volatile unsigned int seek_adps            :  1 ; // [      0] 
        volatile unsigned int dcc                  :  1 ; // [      1] 
        volatile unsigned int rssi                 :  1 ; // [      2] 
        volatile unsigned int agc                  :  1 ; // [      3] 
        volatile unsigned int agc_adps             :  1 ; // [      4] 
        volatile unsigned int fom                  :  1 ; // [      5] 
        volatile unsigned int foc                  :  1 ; // [      6] 
        volatile unsigned int plt_dect             :  1 ; // [      7] 
        volatile unsigned int reserved_8           :  1 ; // [      8] 
        volatile unsigned int adp_lpf              :  1 ; // [      9] 
        volatile unsigned int hw_mute              :  1 ; // [     10] 
        volatile unsigned int sw_mute              :  1 ; // [     11] 
        volatile unsigned int aud_bld              :  1 ; // [     12] 
        volatile unsigned int adc_1bit             :  1 ; // [     13] 
        volatile unsigned int reserved_14          : 17 ; // [30 : 14] 
        volatile unsigned int glben                :  1 ; // [     31] 
    } mBits ;

#else

    struct _fm_en_map
    {
        volatile unsigned int glben                :  1 ; // [     31] 
        volatile unsigned int reserved_14          : 17 ; // [30 : 14] 
        volatile unsigned int adc_1bit             :  1 ; // [     13] 
        volatile unsigned int aud_bld              :  1 ; // [     12] 
        volatile unsigned int sw_mute              :  1 ; // [     11] 
        volatile unsigned int hw_mute              :  1 ; // [     10] 
        volatile unsigned int adp_lpf              :  1 ; // [      9] 
        volatile unsigned int reserved_8           :  1 ; // [      8] 
        volatile unsigned int plt_dect             :  1 ; // [      7] 
        volatile unsigned int foc                  :  1 ; // [      6] 
        volatile unsigned int fom                  :  1 ; // [      5] 
        volatile unsigned int agc_adps             :  1 ; // [      4] 
        volatile unsigned int agc                  :  1 ; // [      3] 
        volatile unsigned int rssi                 :  1 ; // [      2] 
        volatile unsigned int dcc                  :  1 ; // [      1] 
        volatile unsigned int seek_adps            :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} FM_EN_U ;


//-----------------------------------------------------------------------------
// Register name             : RF_INIT_GAIN
// Register offset           : 0x0008
// Register description      : RF Initial Gain register
// Register field description:
//    fld_val            : [15 :  0] RF initial gain value. 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define RF_INIT_GAIN_VAL_SHIFT                 0
#define RF_INIT_GAIN_VAL_MSK                   (0xFFFF << RF_INIT_GAIN_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _rf_init_gain_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _rf_init_gain_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _rf_init_gain_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} RF_INIT_GAIN_U ;


//-----------------------------------------------------------------------------
// Register name             : CHAN
// Register offset           : 0x000c
// Register description      : Channel selection frequency
// Register field description:
//    fld_freq           : [15 :  0] Channel selection frequency  which is used for SEEK,Tune or directly play 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define CHAN_FREQ_SHIFT                        0
#define CHAN_FREQ_MSK                          (0xFFFF << CHAN_FREQ_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _chan_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _chan_map
    {
        volatile unsigned int freq               : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _chan_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int freq               : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} CHAN_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_TBL_CLK
// Register offset           : 0x0010
// Register description      : AGC DB Table clock control
// Register field description:
//    fld_ind            : [17 : 16] Clock indicator.
//                                   [17]: 1 to indicate the APB clock is selected.
//                                   [16]: 1 to indicate the FM 26M clock is selected. 
//    fld_sel            : [      0] Clock select.
//                                   0: Slect FM 26M clock for AGC DB table
//                                   1: Select APB clock for AGC DB table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_TBL_CLK_SEL                        BIT_0
#define AGC_TBL_CLK_IND_SHIFT                  16
#define AGC_TBL_CLK_IND_MSK                    (0x3 << AGC_TBL_CLK_IND_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_tbl_clk_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_tbl_clk_map
    {
        volatile unsigned int sel                :  1 ; // [      0] 
        volatile unsigned int reserved_1         : 15 ; // [15 :  1] 
        volatile unsigned int ind                :  2 ; // [17 : 16] 
        volatile unsigned int reserved_18        : 14 ; // [31 : 18] 
    } mBits ;

#else

    struct _agc_tbl_clk_map
    {
        volatile unsigned int reserved_18        : 14 ; // [31 : 18] 
        volatile unsigned int ind                :  2 ; // [17 : 16] 
        volatile unsigned int reserved_1         : 15 ; // [15 :  1] 
        volatile unsigned int sel                :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_TBL_CLK_U ;


//-----------------------------------------------------------------------------
// Register name             : SEEK_LOOP
// Register offset           : 0x0014
// Register description      : Seek loop config register
// Register field description:
//    fld_val            : [ 9 :  0] Seek loop config value. 
//                                   Seek_loop=(band_lmt_high-band_lmt_low)/band_space when the seek mode is set. 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SEEK_LOOP_VAL_SHIFT                    0
#define SEEK_LOOP_VAL_MSK                      (0x3FF << SEEK_LOOP_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _seek_loop_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _seek_loop_map
    {
        volatile unsigned int val                : 10 ; // [ 9 :  0] 
        volatile unsigned int reserved_10        : 22 ; // [31 : 10] 
    } mBits ;

#else

    struct _seek_loop_map
    {
        volatile unsigned int reserved_10        : 22 ; // [31 : 10] 
        volatile unsigned int val                : 10 ; // [ 9 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SEEK_LOOP_U ;


//-----------------------------------------------------------------------------
// Register name             : FMCTL_STI
// Register offset           : 0x0018
// Register description      : Seek/Tune mode indicator register.
// Register field description:
//    fld_val           : [ 1 :  0] Seek/Tune indicator config value.
//                                  0: NORMAL
//                                  1: SEEK
//                                  2: TUNE 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define FMCTL_STI_VAL_SHIFT                   0
#define FMCTL_STI_VAL_MSK                     (0x3 << FMCTL_STI_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _fmctl_sti_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _fmctl_sti_map
    {
        volatile unsigned int val               :  2 ; // [ 1 :  0] 
        volatile unsigned int reserved_2        : 30 ; // [31 :  2] 
    } mBits ;

#else

    struct _fmctl_sti_map
    {
        volatile unsigned int reserved_2        : 30 ; // [31 :  2] 
        volatile unsigned int val               :  2 ; // [ 1 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} FMCTL_STI_U ;


//-----------------------------------------------------------------------------
// Register name             : BAND_LMT
// Register offset           : 0x001C
// Register description      : Frequence band limit for seek.
// Register field description:
//    fld_high        : [31 : 16] High band limit for seek 
//    fld_low         : [15 :  0] Low band limit for seek 
//-----------------------------------------------------------------------------

#define FM_INT_NUM  44
#define FM_DATA_CAP_INT_NUM 12

//------------------------- Fields Definition ---------------------------------
#define BAND_LMT_LOW_SHIFT                  0
#define BAND_LMT_LOW_MSK                    (0xFFFF << BAND_LMT_LOW_SHIFT)
#define BAND_LMT_HIGH_SHIFT                 16
#define BAND_LMT_HIGH_MSK                   (0xFFFF << BAND_LMT_HIGH_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _band_lmt_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _band_lmt_map
    {
        volatile unsigned int low             : 16 ; // [15 :  0] 
        volatile unsigned int high            : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _band_lmt_map
    {
        volatile unsigned int high            : 16 ; // [31 : 16] 
        volatile unsigned int low             : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} BAND_LMT_U ;


//-----------------------------------------------------------------------------
// Register name             : BAND_SPACE
// Register offset           : 0x0020
// Register description      : Channel spacing. 
// Register field description:
//    fld_val            : [15 :  0] Channel spacing config value 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define BAND_SPACE_VAL_SHIFT                   0
#define BAND_SPACE_VAL_MSK                     (0xFFFF << BAND_SPACE_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _band_space_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _band_space_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _band_space_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} BAND_SPACE_U ;


//-----------------------------------------------------------------------------
// Register name             : INT_EN
// Register offset           : 0x0030
// Register description      : Interrupt Enable Register
// Register field description:
//    fld_stc           : [      0] Seek/Tune Complete Interrupt enable bit. 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define INT_EN_STC                            BIT_0

//------------------------- Data Prototype ------------------------------------
typedef union _int_en_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _int_en_map
    {
        volatile unsigned int stc               :  1 ; // [      0] 
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
    } mBits ;

#else

    struct _int_en_map
    {
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
        volatile unsigned int stc               :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} INT_EN_U ;


//-----------------------------------------------------------------------------
// Register name             : INT_CLR
// Register offset           : 0x0034
// Register description      : Interrupt Clear Register
// Register field description:
//    fld_stc           : [      0] Seek/Tune Complete Interrupt clear bit. 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define INT_CLR_STC                           BIT_0

//------------------------- Data Prototype ------------------------------------
typedef union _int_clr_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _int_clr_map
    {
        volatile unsigned int stc               :  1 ; // [      0] 
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
    } mBits ;

#else

    struct _int_clr_map
    {
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
        volatile unsigned int stc               :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} INT_CLR_U ;


//-----------------------------------------------------------------------------
// Register name             : INT_STS
// Register offset           : 0x0038
// Register description      : Interrupt Status
// Register field description:
//    fld_stc           : [      0] Seek/Tune Complete Interrupt status. 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define INT_STS_STC                           BIT_0

//------------------------- Data Prototype ------------------------------------
typedef union _int_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _int_sts_map
    {
        volatile unsigned int stc               :  1 ; // [      0] 
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
    } mBits ;

#else

    struct _int_sts_map
    {
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
        volatile unsigned int stc               :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} INT_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : SEEK_CH_TH
// Register offset           : 0x0040
// Register description      : Seek Channel valid Threshold
// Register field description:
//    fld_val            : [15 :  0] Seek channel valid threshold 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SEEK_CH_TH_VAL_SHIFT                   0
#define SEEK_CH_TH_VAL_MSK                     (0xFFFF << SEEK_CH_TH_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _seek_ch_th_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _seek_ch_th_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _seek_ch_th_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SEEK_CH_TH_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_TBL_RFRSSI0
// Register offset           : 0x0044
// Register description      : AGC table RFRSSI control register0
// Register field description:
//    fld_upd_cnt        : [25 : 16] AGC table RF RSSI update counter 
//    fld_hyst           : [13 :  8] AGC table RF RSSI hysteresis 
//    fld_mode           : [ 1 :  0] AGC table RF RSSI mode 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_TBL_RFRSSI0_MODE_SHIFT             0
#define AGC_TBL_RFRSSI0_MODE_MSK               (0x3 << AGC_TBL_RFRSSI0_MODE_SHIFT)
#define AGC_TBL_RFRSSI0_HYST_SHIFT             8
#define AGC_TBL_RFRSSI0_HYST_MSK               (0x3F << AGC_TBL_RFRSSI0_HYST_SHIFT)
#define AGC_TBL_RFRSSI0_UPD_CNT_SHIFT          16
#define AGC_TBL_RFRSSI0_UPD_CNT_MSK            (0x3FF << AGC_TBL_RFRSSI0_UPD_CNT_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_tbl_rfrssi0_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_tbl_rfrssi0_map
    {
        volatile unsigned int mode               :  2 ; // [ 1 :  0] 
        volatile unsigned int reserved_2         :  6 ; // [ 7 :  2] 
        volatile unsigned int hyst               :  6 ; // [13 :  8] 
        volatile unsigned int reserved_14        :  2 ; // [15 : 14] 
        volatile unsigned int upd_cnt            : 10 ; // [25 : 16] 
        volatile unsigned int reserved_26        :  6 ; // [31 : 26] 
    } mBits ;

#else

    struct _agc_tbl_rfrssi0_map
    {
        volatile unsigned int reserved_26        :  6 ; // [31 : 26] 
        volatile unsigned int upd_cnt            : 10 ; // [25 : 16] 
        volatile unsigned int reserved_14        :  2 ; // [15 : 14] 
        volatile unsigned int hyst               :  6 ; // [13 :  8] 
        volatile unsigned int reserved_2         :  6 ; // [ 7 :  2] 
        volatile unsigned int mode               :  2 ; // [ 1 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_TBL_RFRSSI0_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_TBL_RFRSSI1
// Register offset           : 0x0048
// Register description      : AGC table RFRSSI control register1
// Register field description:
//    fld_avg_len        : [30 : 24] RFRSSI average length 
//    fld_avg_sh         : [18 : 16] RFRSSI average acc shift bits 
//    fld_th             : [15 :  0] AGC table RF RSSI threshold 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_TBL_RFRSSI1_TH_SHIFT               0
#define AGC_TBL_RFRSSI1_TH_MSK                 (0xFFFF << AGC_TBL_RFRSSI1_TH_SHIFT)
#define AGC_TBL_RFRSSI1_AVG_SH_SHIFT           16
#define AGC_TBL_RFRSSI1_AVG_SH_MSK             (0x7 << AGC_TBL_RFRSSI1_AVG_SH_SHIFT)
#define AGC_TBL_RFRSSI1_AVG_LEN_SHIFT          24
#define AGC_TBL_RFRSSI1_AVG_LEN_MSK            (0x7F << AGC_TBL_RFRSSI1_AVG_LEN_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_tbl_rfrssi1_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_tbl_rfrssi1_map
    {
        volatile unsigned int th                 : 16 ; // [15 :  0] 
        volatile unsigned int avg_sh             :  3 ; // [18 : 16] 
        volatile unsigned int reserved_19        :  5 ; // [23 : 19] 
        volatile unsigned int avg_len            :  7 ; // [30 : 24] 
        volatile unsigned int reserved_31        :  1 ; // [     31] 
    } mBits ;

#else

    struct _agc_tbl_rfrssi1_map
    {
        volatile unsigned int reserved_31        :  1 ; // [     31] 
        volatile unsigned int avg_len            :  7 ; // [30 : 24] 
        volatile unsigned int reserved_19        :  5 ; // [23 : 19] 
        volatile unsigned int avg_sh             :  3 ; // [18 : 16] 
        volatile unsigned int th                 : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_TBL_RFRSSI1_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_TBL_WBRSSI
// Register offset           : 0x004C
// Register description      : WB RSSI measure control
// Register field description:
//    fld_th                  : [23 : 16] AGC table WBRSSI Threshold 
//    fld_hyst                : [13 :  8] AGC table WBRSSI hysteresis 
//    fld_wbrssi_shway        : [      4] WNRSSI scaling method 
//    fld_wbrssi_sh           : [ 3 :  2] WBRSSI scaling shift bits 
//    fld_mode                : [ 1 :  0] AGC table WBRSSI mode
//                                        [0]: mode
//                                        [1]: mode2 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_TBL_WBRSSI_MODE_SHIFT                   0
#define AGC_TBL_WBRSSI_MODE_MSK                     (0x3 << AGC_TBL_WBRSSI_MODE_SHIFT)
#define AGC_TBL_WBRSSI_WBRSSI_SH_SHIFT              2
#define AGC_TBL_WBRSSI_WBRSSI_SH_MSK                (0x3 << AGC_TBL_WBRSSI_WBRSSI_SH_SHIFT)
#define AGC_TBL_WBRSSI_WBRSSI_SHWAY                 BIT_4
#define AGC_TBL_WBRSSI_HYST_SHIFT                   8
#define AGC_TBL_WBRSSI_HYST_MSK                     (0x3F << AGC_TBL_WBRSSI_HYST_SHIFT)
#define AGC_TBL_WBRSSI_TH_SHIFT                     16
#define AGC_TBL_WBRSSI_TH_MSK                       (0xFF << AGC_TBL_WBRSSI_TH_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_tbl_wbrssi_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_tbl_wbrssi_map
    {
        volatile unsigned int mode                    :  2 ; // [ 1 :  0] 
        volatile unsigned int wbrssi_sh               :  2 ; // [ 3 :  2] 
        volatile unsigned int wbrssi_shway            :  1 ; // [      4] 
        volatile unsigned int reserved_5              :  3 ; // [ 7 :  5] 
        volatile unsigned int hyst                    :  6 ; // [13 :  8] 
        volatile unsigned int reserved_14             :  2 ; // [15 : 14] 
        volatile unsigned int th                      :  8 ; // [23 : 16] 
        volatile unsigned int reserved_24             :  8 ; // [31 : 24] 
    } mBits ;

#else

    struct _agc_tbl_wbrssi_map
    {
        volatile unsigned int reserved_24             :  8 ; // [31 : 24] 
        volatile unsigned int th                      :  8 ; // [23 : 16] 
        volatile unsigned int reserved_14             :  2 ; // [15 : 14] 
        volatile unsigned int hyst                    :  6 ; // [13 :  8] 
        volatile unsigned int reserved_5              :  3 ; // [ 7 :  5] 
        volatile unsigned int wbrssi_shway            :  1 ; // [      4] 
        volatile unsigned int wbrssi_sh               :  2 ; // [ 3 :  2] 
        volatile unsigned int mode                    :  2 ; // [ 1 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_TBL_WBRSSI_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_IDX_TH
// Register offset           : 0x0050
// Register description      : AGC table Index Threshold
// Register field description:
//    fld_max            : [19 : 16] Seek AGC index High Limit 
//    fld_mid            : [11 :  8] Seek AGC index Middle value 
//    fld_min            : [ 3 :  0] Seek AGC index Low Limit 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_IDX_TH_MIN_SHIFT                   0
#define AGC_IDX_TH_MIN_MSK                     (0xF << AGC_IDX_TH_MIN_SHIFT)
#define AGC_IDX_TH_MID_SHIFT                   8
#define AGC_IDX_TH_MID_MSK                     (0xF << AGC_IDX_TH_MID_SHIFT)
#define AGC_IDX_TH_MAX_SHIFT                   16
#define AGC_IDX_TH_MAX_MSK                     (0xF << AGC_IDX_TH_MAX_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_idx_th_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_idx_th_map
    {
        volatile unsigned int min                :  4 ; // [ 3 :  0] 
        volatile unsigned int reserved_4         :  4 ; // [ 7 :  4] 
        volatile unsigned int mid                :  4 ; // [11 :  8] 
        volatile unsigned int reserved_12        :  4 ; // [15 : 12] 
        volatile unsigned int max                :  4 ; // [19 : 16] 
        volatile unsigned int reserved_20        : 12 ; // [31 : 20] 
    } mBits ;

#else

    struct _agc_idx_th_map
    {
        volatile unsigned int reserved_20        : 12 ; // [31 : 20] 
        volatile unsigned int max                :  4 ; // [19 : 16] 
        volatile unsigned int reserved_12        :  4 ; // [15 : 12] 
        volatile unsigned int mid                :  4 ; // [11 :  8] 
        volatile unsigned int reserved_4         :  4 ; // [ 7 :  4] 
        volatile unsigned int min                :  4 ; // [ 3 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_IDX_TH_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_RSSI_TH
// Register offset           : 0x0054
// Register description      : AGC RSSI Threshold
// Register field description:
//    fld_max            : [23 : 16] Seek AGC RSSI High Limit 
//    fld_min            : [ 7 :  0] Seek AGC RSSI Low Limit 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_RSSI_TH_MIN_SHIFT                  0
#define AGC_RSSI_TH_MIN_MSK                    (0xFF << AGC_RSSI_TH_MIN_SHIFT)
#define AGC_RSSI_TH_MAX_SHIFT                  16
#define AGC_RSSI_TH_MAX_MSK                    (0xFF << AGC_RSSI_TH_MAX_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_rssi_th_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_rssi_th_map
    {
        volatile unsigned int min                :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8         :  8 ; // [15 :  8] 
        volatile unsigned int max                :  8 ; // [23 : 16] 
        volatile unsigned int reserved_24        :  8 ; // [31 : 24] 
    } mBits ;

#else

    struct _agc_rssi_th_map
    {
        volatile unsigned int reserved_24        :  8 ; // [31 : 24] 
        volatile unsigned int max                :  8 ; // [23 : 16] 
        volatile unsigned int reserved_8         :  8 ; // [15 :  8] 
        volatile unsigned int min                :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_RSSI_TH_U ;


//-----------------------------------------------------------------------------
// Register name             : SEEK_ADPS
// Register offset           : 0x0058
// Register description      : Seek Adaptive Scaling pre-demodulation 
// Register field description:
//    fld_sft            : [27 : 24] Seek adaptive scaling pre-demodulation bit Shift 
//    fld_lbnd           : [23 : 16] Seek adaptive scaling pre-demodulation Low-Boundary 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SEEK_ADPS_LBND_SHIFT                   16
#define SEEK_ADPS_LBND_MSK                     (0xFF << SEEK_ADPS_LBND_SHIFT)
#define SEEK_ADPS_SFT_SHIFT                    24
#define SEEK_ADPS_SFT_MSK                      (0xF << SEEK_ADPS_SFT_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _seek_adps_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _seek_adps_map
    {
        volatile unsigned int reserved_0         : 16 ; // [15 :  0] 
        volatile unsigned int lbnd               :  8 ; // [23 : 16] 
        volatile unsigned int sft                :  4 ; // [27 : 24] 
        volatile unsigned int reserved_28        :  4 ; // [31 : 28] 
    } mBits ;

#else

    struct _seek_adps_map
    {
        volatile unsigned int reserved_28        :  4 ; // [31 : 28] 
        volatile unsigned int sft                :  4 ; // [27 : 24] 
        volatile unsigned int lbnd               :  8 ; // [23 : 16] 
        volatile unsigned int reserved_0         : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SEEK_ADPS_U ;


//-----------------------------------------------------------------------------
// Register name             : STER_PWR
// Register offset           : 0x005C
// Register description      : FM Stereo Power Threshold
// Register field description:
//    fld_th            : [ 8 :  0] FM Stereo Power Threshold 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define STER_PWR_TH_SHIFT                     0
#define STER_PWR_TH_MSK                       (0x1FF << STER_PWR_TH_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _ster_pwr_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _ster_pwr_map
    {
        volatile unsigned int th                :  9 ; // [ 8 :  0] 
        volatile unsigned int reserved_9        : 23 ; // [31 :  9] 
    } mBits ;

#else

    struct _ster_pwr_map
    {
        volatile unsigned int reserved_9        : 23 ; // [31 :  9] 
        volatile unsigned int th                :  9 ; // [ 8 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} STER_PWR_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_CTRL
// Register offset           : 0x0060
// Register description      : AGC Control Register
// Register field description:
//    fld_tab_size         : [27 : 24] AGC table size 
//    fld_sw_gain          : [23 : 16] Software maunal config gain value when AGC is disabled. 
//    fld_gain_step        : [11 :  8] AGC Gain Step  
//    fld_mthd             : [ 5 :  4] AGC calculate method 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_CTRL_MTHD_SHIFT                      4
#define AGC_CTRL_MTHD_MSK                        (0x3 << AGC_CTRL_MTHD_SHIFT)
#define AGC_CTRL_GAIN_STEP_SHIFT                 8
#define AGC_CTRL_GAIN_STEP_MSK                   (0xF << AGC_CTRL_GAIN_STEP_SHIFT)
#define AGC_CTRL_SW_GAIN_SHIFT                   16
#define AGC_CTRL_SW_GAIN_MSK                     (0xFF << AGC_CTRL_SW_GAIN_SHIFT)
#define AGC_CTRL_TAB_SIZE_SHIFT                  24
#define AGC_CTRL_TAB_SIZE_MSK                    (0xF << AGC_CTRL_TAB_SIZE_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_ctrl_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_ctrl_map
    {
        volatile unsigned int reserved_0           :  4 ; // [ 3 :  0] 
        volatile unsigned int mthd                 :  2 ; // [ 5 :  4] 
        volatile unsigned int reserved_6           :  2 ; // [ 7 :  6] 
        volatile unsigned int gain_step            :  4 ; // [11 :  8] 
        volatile unsigned int reserved_12          :  4 ; // [15 : 12] 
        volatile unsigned int sw_gain              :  8 ; // [23 : 16] 
        volatile unsigned int tab_size             :  4 ; // [27 : 24] 
        volatile unsigned int reserved_28          :  4 ; // [31 : 28] 
    } mBits ;

#else

    struct _agc_ctrl_map
    {
        volatile unsigned int reserved_28          :  4 ; // [31 : 28] 
        volatile unsigned int tab_size             :  4 ; // [27 : 24] 
        volatile unsigned int sw_gain              :  8 ; // [23 : 16] 
        volatile unsigned int reserved_12          :  4 ; // [15 : 12] 
        volatile unsigned int gain_step            :  4 ; // [11 :  8] 
        volatile unsigned int reserved_6           :  2 ; // [ 7 :  6] 
        volatile unsigned int mthd                 :  2 ; // [ 5 :  4] 
        volatile unsigned int reserved_0           :  4 ; // [ 3 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_CTRL_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_ITV_TH
// Register offset           : 0x0064
// Register description      : AGC Interval Threshold
// Register field description:
//    fld_max            : [23 : 16] AGC Interval Threshold MAX value 
//    fld_min            : [ 7 :  0] AGC Interval Threshold MIN value 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_ITV_TH_MIN_SHIFT                   0
#define AGC_ITV_TH_MIN_MSK                     (0xFF << AGC_ITV_TH_MIN_SHIFT)
#define AGC_ITV_TH_MAX_SHIFT                   16
#define AGC_ITV_TH_MAX_MSK                     (0xFF << AGC_ITV_TH_MAX_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_itv_th_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_itv_th_map
    {
        volatile unsigned int min                :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8         :  8 ; // [15 :  8] 
        volatile unsigned int max                :  8 ; // [23 : 16] 
        volatile unsigned int reserved_24        :  8 ; // [31 : 24] 
    } mBits ;

#else

    struct _agc_itv_th_map
    {
        volatile unsigned int reserved_24        :  8 ; // [31 : 24] 
        volatile unsigned int max                :  8 ; // [23 : 16] 
        volatile unsigned int reserved_8         :  8 ; // [15 :  8] 
        volatile unsigned int min                :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_ITV_TH_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_ADPS0
// Register offset           : 0x0068
// Register description      : AGC Adaptive Scaling pre-demodulation
// Register field description:
//    fld_hbnd           : [31 : 24] Pre-demodulation High Boundary 
//    fld_lbnd           : [23 : 16] Pre-demodulation :Low Boundary 
//    fld_sft_lmt        : [ 3 :  0] Pre-demodulation Bit Shift Limit 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_ADPS0_SFT_LMT_SHIFT                0
#define AGC_ADPS0_SFT_LMT_MSK                  (0xF << AGC_ADPS0_SFT_LMT_SHIFT)
#define AGC_ADPS0_LBND_SHIFT                   16
#define AGC_ADPS0_LBND_MSK                     (0xFF << AGC_ADPS0_LBND_SHIFT)
#define AGC_ADPS0_HBND_SHIFT                   24
#define AGC_ADPS0_HBND_MSK                     (0xFF << AGC_ADPS0_HBND_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_adps0_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_adps0_map
    {
        volatile unsigned int sft_lmt            :  4 ; // [ 3 :  0] 
        volatile unsigned int reserved_4         : 12 ; // [15 :  4] 
        volatile unsigned int lbnd               :  8 ; // [23 : 16] 
        volatile unsigned int hbnd               :  8 ; // [31 : 24] 
    } mBits ;

#else

    struct _agc_adps0_map
    {
        volatile unsigned int hbnd               :  8 ; // [31 : 24] 
        volatile unsigned int lbnd               :  8 ; // [23 : 16] 
        volatile unsigned int reserved_4         : 12 ; // [15 :  4] 
        volatile unsigned int sft_lmt            :  4 ; // [ 3 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_ADPS0_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_ADPS1
// Register offset           : 0x006C
// Register description      : AGC Adaptive Scaling pre-demodulation
// Register field description:
//    fld_hlev          : [31 : 24] Pre-demodulation High Level for method not equal 0 
//    fld_llev          : [23 : 16] Pre-demodulation :Low Boundary for method not eque 0 
//    fld_mthd          : [ 1 :  0] Pre-demodulation adaptive scaling method 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_ADPS1_MTHD_SHIFT                  0
#define AGC_ADPS1_MTHD_MSK                    (0x3 << AGC_ADPS1_MTHD_SHIFT)
#define AGC_ADPS1_LLEV_SHIFT                  16
#define AGC_ADPS1_LLEV_MSK                    (0xFF << AGC_ADPS1_LLEV_SHIFT)
#define AGC_ADPS1_HLEV_SHIFT                  24
#define AGC_ADPS1_HLEV_MSK                    (0xFF << AGC_ADPS1_HLEV_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_adps1_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_adps1_map
    {
        volatile unsigned int mthd              :  2 ; // [ 1 :  0] 
        volatile unsigned int reserved_2        : 14 ; // [15 :  2] 
        volatile unsigned int llev              :  8 ; // [23 : 16] 
        volatile unsigned int hlev              :  8 ; // [31 : 24] 
    } mBits ;

#else

    struct _agc_adps1_map
    {
        volatile unsigned int hlev              :  8 ; // [31 : 24] 
        volatile unsigned int llev              :  8 ; // [23 : 16] 
        volatile unsigned int reserved_2        : 14 ; // [15 :  2] 
        volatile unsigned int mthd              :  2 ; // [ 1 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_ADPS1_U ;


//-----------------------------------------------------------------------------
// Register name             : PDP_TH
// Register offset           : 0x0070
// Register description      : Pilot Detect PowerThreshold
// Register field description:
//    fld_val            : [12 :  0] Pilot Detect PowerThreshold 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define PDP_TH_VAL_SHIFT                       0
#define PDP_TH_VAL_MSK                         (0x1FFF << PDP_TH_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _pdp_th_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _pdp_th_map
    {
        volatile unsigned int val                : 13 ; // [12 :  0] 
        volatile unsigned int reserved_13        : 19 ; // [31 : 13] 
    } mBits ;

#else

    struct _pdp_th_map
    {
        volatile unsigned int reserved_13        : 19 ; // [31 : 13] 
        volatile unsigned int val                : 13 ; // [12 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} PDP_TH_U ;


//-----------------------------------------------------------------------------
// Register name             : PDP_DEV
// Register offset           : 0x0074
// Register description      : Pilot Detect Power Deviation
// Register field description:
//    fld_val            : [12 :  0] Pilot Detect Power Deviation 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define PDP_DEV_VAL_SHIFT                      0
#define PDP_DEV_VAL_MSK                        (0x1FFF << PDP_DEV_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _pdp_dev_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _pdp_dev_map
    {
        volatile unsigned int val                : 13 ; // [12 :  0] 
        volatile unsigned int reserved_13        : 19 ; // [31 : 13] 
    } mBits ;

#else

    struct _pdp_dev_map
    {
        volatile unsigned int reserved_13        : 19 ; // [31 : 13] 
        volatile unsigned int val                : 13 ; // [12 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} PDP_DEV_U ;


//-----------------------------------------------------------------------------
// Register name             : ADP_ST_CONF
// Register offset           : 0x0078
// Register description      : Adaptive Stereo Configuration Register
// Register field description:
//    fld_mode           : [29 : 28] Adaptive stereo configuration mode 
//    fld_hys            : [24 : 16] Stereo Hysteresis Adaptive Configuration  
//    fld_th             : [ 8 :  0] Stereo Threshold Adaptive Configuration  
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define ADP_ST_CONF_TH_SHIFT                   0
#define ADP_ST_CONF_TH_MSK                     (0x1FF << ADP_ST_CONF_TH_SHIFT)
#define ADP_ST_CONF_HYS_SHIFT                  16
#define ADP_ST_CONF_HYS_MSK                    (0x1FF << ADP_ST_CONF_HYS_SHIFT)
#define ADP_ST_CONF_MODE_SHIFT                 28
#define ADP_ST_CONF_MODE_MSK                   (0x3 << ADP_ST_CONF_MODE_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _adp_st_conf_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _adp_st_conf_map
    {
        volatile unsigned int th                 :  9 ; // [ 8 :  0] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int hys                :  9 ; // [24 : 16] 
        volatile unsigned int reserved_25        :  3 ; // [27 : 25] 
        volatile unsigned int mode               :  2 ; // [29 : 28] 
        volatile unsigned int reserved_30        :  2 ; // [31 : 30] 
    } mBits ;

#else

    struct _adp_st_conf_map
    {
        volatile unsigned int reserved_30        :  2 ; // [31 : 30] 
        volatile unsigned int mode               :  2 ; // [29 : 28] 
        volatile unsigned int reserved_25        :  3 ; // [27 : 25] 
        volatile unsigned int hys                :  9 ; // [24 : 16] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int th                 :  9 ; // [ 8 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} ADP_ST_CONF_U ;


//-----------------------------------------------------------------------------
// Register name             : ADP_LPF_CONF
// Register offset           : 0x007C
// Register description      : Adaptive LPF Configuration Register
// Register field description:
//    fld_val           : [ 2 :  0] Adaptive LPF Configuration 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define ADP_LPF_CONF_VAL_SHIFT                0
#define ADP_LPF_CONF_VAL_MSK                  (0x7 << ADP_LPF_CONF_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _adp_lpf_conf_tag
{ 
#ifdef CHIP_ENDIAN_LITTLE
    struct _adp_lpf_conf_map
    {
        volatile unsigned int val               :  3 ; // [ 2 :  0] 
        volatile unsigned int reserved_3        : 29 ; // [31 :  3] 
    } mBits ;

#else

    struct _adp_lpf_conf_map
    {
        volatile unsigned int reserved_3        : 29 ; // [31 :  3] 
        volatile unsigned int val               :  3 ; // [ 2 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE
    volatile unsigned int dwValue ;
} ADP_LPF_CONF_U ;


//-----------------------------------------------------------------------------
// Register name             : DEPHA_SCAL
// Register offset           : 0x0080
// Register description      : De-emphasis scaling factor
// Register field description:
//    fld_val           : [ 2 :  0] De-emphasis scaling factor value. 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define DEPHA_SCAL_VAL_SHIFT                  0
#define DEPHA_SCAL_VAL_MSK                    (0x7 << DEPHA_SCAL_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _depha_scal_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _depha_scal_map
    {
        volatile unsigned int val               :  3 ; // [ 2 :  0] 
        volatile unsigned int reserved_3        : 29 ; // [31 :  3] 
    } mBits ;

#else

    struct _depha_scal_map
    {
        volatile unsigned int reserved_3        : 29 ; // [31 :  3] 
        volatile unsigned int val               :  3 ; // [ 2 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} DEPHA_SCAL_U ;


//-----------------------------------------------------------------------------
// Register name             : HW_MUTE
// Register offset           : 0x0084
// Register description      : Hardware mute control
// Register field description:
//    fld_mnd           : [      0] Mandatory to use hardware mute function 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define HW_MUTE_MND                           BIT_0

//------------------------- Data Prototype ------------------------------------
typedef union _hw_mute_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _hw_mute_map
    {
        volatile unsigned int mnd               :  1 ; // [      0] 
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
    } mBits ;

#else

    struct _hw_mute_map
    {
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
        volatile unsigned int mnd               :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} HW_MUTE_U ;


//-----------------------------------------------------------------------------
// Register name             : SW_MUTE
// Register offset           : 0x0088
// Register description      : Software mute control
// Register field description:
//    fld_hbnd           : [24 : 16] Software mute high boundary 
//    fld_lbnd           : [ 8 :  0] Software mute low boundary 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SW_MUTE_LBND_SHIFT                     0
#define SW_MUTE_LBND_MSK                       (0x1FF << SW_MUTE_LBND_SHIFT)
#define SW_MUTE_HBND_SHIFT                     16
#define SW_MUTE_HBND_MSK                       (0x1FF << SW_MUTE_HBND_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _sw_mute_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _sw_mute_map
    {
        volatile unsigned int lbnd               :  9 ; // [ 8 :  0] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int hbnd               :  9 ; // [24 : 16] 
        volatile unsigned int reserved_25        :  7 ; // [31 : 25] 
    } mBits ;

#else

    struct _sw_mute_map
    {
        volatile unsigned int reserved_25        :  7 ; // [31 : 25] 
        volatile unsigned int hbnd               :  9 ; // [24 : 16] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int lbnd               :  9 ; // [ 8 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SW_MUTE_U ;


//-----------------------------------------------------------------------------
// Register name             : UPD_CTRL
// Register offset           : 0x008C
// Register description      : Update counter control registers
// Register field description:
//    fld_stdif_dly           : [25 : 24] Stereo Dif Delay 
//    fld_agc_vs_rssi         : [23 : 20] AGC Update Counter vs RSSI 
//    fld_seek_agc_dly        : [19 : 16] SEEK AGC delay  
//    fld_agc_adps            : [15 : 12] AGC adaptive scaling pre-demodulation update time 
//    fld_predem_dly          : [11 :  8] AGC adaptive scaling pre-demodulation delay 
//    fld_soft_mute           : [ 7 :  4] Softmute update counter 
//    fld_adp_st              : [ 3 :  0] Adaptive stereo config update counter 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define UPD_CTRL_ADP_ST_SHIFT                       0
#define UPD_CTRL_ADP_ST_MSK                         (0xF << UPD_CTRL_ADP_ST_SHIFT)
#define UPD_CTRL_SOFT_MUTE_SHIFT                    4
#define UPD_CTRL_SOFT_MUTE_MSK                      (0xF << UPD_CTRL_SOFT_MUTE_SHIFT)
#define UPD_CTRL_PREDEM_DLY_SHIFT                   8
#define UPD_CTRL_PREDEM_DLY_MSK                     (0xF << UPD_CTRL_PREDEM_DLY_SHIFT)
#define UPD_CTRL_AGC_ADPS_SHIFT                     12
#define UPD_CTRL_AGC_ADPS_MSK                       (0xF << UPD_CTRL_AGC_ADPS_SHIFT)
#define UPD_CTRL_SEEK_AGC_DLY_SHIFT                 16
#define UPD_CTRL_SEEK_AGC_DLY_MSK                   (0xF << UPD_CTRL_SEEK_AGC_DLY_SHIFT)
#define UPD_CTRL_AGC_VS_RSSI_SHIFT                  20
#define UPD_CTRL_AGC_VS_RSSI_MSK                    (0xF << UPD_CTRL_AGC_VS_RSSI_SHIFT)
#define UPD_CTRL_STDIF_DLY_SHIFT                    24
#define UPD_CTRL_STDIF_DLY_MSK                      (0x3 << UPD_CTRL_STDIF_DLY_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _upd_ctrl_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _upd_ctrl_map
    {
        volatile unsigned int adp_st                  :  4 ; // [ 3 :  0] 
        volatile unsigned int soft_mute               :  4 ; // [ 7 :  4] 
        volatile unsigned int predem_dly              :  4 ; // [11 :  8] 
        volatile unsigned int agc_adps                :  4 ; // [15 : 12] 
        volatile unsigned int seek_agc_dly            :  4 ; // [19 : 16] 
        volatile unsigned int agc_vs_rssi             :  4 ; // [23 : 20] 
        volatile unsigned int stdif_dly               :  2 ; // [25 : 24] 
        volatile unsigned int reserved_26             :  6 ; // [31 : 26] 
    } mBits ;

#else

    struct _upd_ctrl_map
    {
        volatile unsigned int reserved_26             :  6 ; // [31 : 26] 
        volatile unsigned int stdif_dly               :  2 ; // [25 : 24] 
        volatile unsigned int agc_vs_rssi             :  4 ; // [23 : 20] 
        volatile unsigned int seek_agc_dly            :  4 ; // [19 : 16] 
        volatile unsigned int agc_adps                :  4 ; // [15 : 12] 
        volatile unsigned int predem_dly              :  4 ; // [11 :  8] 
        volatile unsigned int soft_mute               :  4 ; // [ 7 :  4] 
        volatile unsigned int adp_st                  :  4 ; // [ 3 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} UPD_CTRL_U ;


//-----------------------------------------------------------------------------
// Register name             : AUD_BLD0
// Register offset           : 0x0090
// Register description      : Audio blend control register 0
// Register field description:
//    fld_hbnd           : [24 : 16] Audio blend high boudary 
//    fld_lbnd           : [ 8 :  0] Audio blend low boundary 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AUD_BLD0_LBND_SHIFT                    0
#define AUD_BLD0_LBND_MSK                      (0x1FF << AUD_BLD0_LBND_SHIFT)
#define AUD_BLD0_HBND_SHIFT                    16
#define AUD_BLD0_HBND_MSK                      (0x1FF << AUD_BLD0_HBND_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _aud_bld0_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _aud_bld0_map
    {
        volatile unsigned int lbnd               :  9 ; // [ 8 :  0] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int hbnd               :  9 ; // [24 : 16] 
        volatile unsigned int reserved_25        :  7 ; // [31 : 25] 
    } mBits ;

#else

    struct _aud_bld0_map
    {
        volatile unsigned int reserved_25        :  7 ; // [31 : 25] 
        volatile unsigned int hbnd               :  9 ; // [24 : 16] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int lbnd               :  9 ; // [ 8 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AUD_BLD0_U ;


//-----------------------------------------------------------------------------
// Register name             : AUD_BLD1
// Register offset           : 0x0094
// Register description      : Audio blend control register 1
// Register field description:
//    fld_mode           : [     31] Audio blend mode 
//    fld_lev2           : [24 : 16] Audio blend level 2 
//    fld_lev1           : [ 8 :  0] Audio blend level 1 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AUD_BLD1_LEV1_SHIFT                    0
#define AUD_BLD1_LEV1_MSK                      (0x1FF << AUD_BLD1_LEV1_SHIFT)
#define AUD_BLD1_LEV2_SHIFT                    16
#define AUD_BLD1_LEV2_MSK                      (0x1FF << AUD_BLD1_LEV2_SHIFT)
#define AUD_BLD1_MODE                          BIT_31

//------------------------- Data Prototype ------------------------------------
typedef union _aud_bld1_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _aud_bld1_map
    {
        volatile unsigned int lev1               :  9 ; // [ 8 :  0] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int lev2               :  9 ; // [24 : 16] 
        volatile unsigned int reserved_25        :  6 ; // [30 : 25] 
        volatile unsigned int mode               :  1 ; // [     31] 
    } mBits ;

#else

    struct _aud_bld1_map
    {
        volatile unsigned int mode               :  1 ; // [     31] 
        volatile unsigned int reserved_25        :  6 ; // [30 : 25] 
        volatile unsigned int lev2               :  9 ; // [24 : 16] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int lev1               :  9 ; // [ 8 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AUD_BLD1_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_HYS
// Register offset           : 0x0098
// Register description      : AGC Hysteresis control register
// Register field description:
//    fld_thresh         : [11 :  8] AGC hysteresis thresh 
//    fld_step           : [ 3 :  0] AGC hysteresis step 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_HYS_STEP_SHIFT                     0
#define AGC_HYS_STEP_MSK                       (0xF << AGC_HYS_STEP_SHIFT)
#define AGC_HYS_THRESH_SHIFT                   8
#define AGC_HYS_THRESH_MSK                     (0xF << AGC_HYS_THRESH_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_hys_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_hys_map
    {
        volatile unsigned int step               :  4 ; // [ 3 :  0] 
        volatile unsigned int reserved_4         :  4 ; // [ 7 :  4] 
        volatile unsigned int thresh             :  4 ; // [11 :  8] 
        volatile unsigned int reserved_12        : 20 ; // [31 : 12] 
    } mBits ;

#else

    struct _agc_hys_map
    {
        volatile unsigned int reserved_12        : 20 ; // [31 : 12] 
        volatile unsigned int thresh             :  4 ; // [11 :  8] 
        volatile unsigned int reserved_4         :  4 ; // [ 7 :  4] 
        volatile unsigned int step               :  4 ; // [ 3 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_HYS_U ;


//-----------------------------------------------------------------------------
// Register name             : MONO_PWR
// Register offset           : 0x009C
// Register description      : FM Mono Power Threshold
// Register field description:
//    fld_th2            : [24 : 16] FM Mono Power Threshold2 
//    fld_th1            : [ 8 :  0] FM Mono Power Threshold1 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define MONO_PWR_TH1_SHIFT                     0
#define MONO_PWR_TH1_MSK                       (0x1FF << MONO_PWR_TH1_SHIFT)
#define MONO_PWR_TH2_SHIFT                     16
#define MONO_PWR_TH2_MSK                       (0x1FF << MONO_PWR_TH2_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _mono_pwr_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _mono_pwr_map
    {
        volatile unsigned int th1                :  9 ; // [ 8 :  0] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int th2                :  9 ; // [24 : 16] 
        volatile unsigned int reserved_25        :  7 ; // [31 : 25] 
    } mBits ;

#else

    struct _mono_pwr_map
    {
        volatile unsigned int reserved_25        :  7 ; // [31 : 25] 
        volatile unsigned int th2                :  9 ; // [24 : 16] 
        volatile unsigned int reserved_9         :  7 ; // [15 :  9] 
        volatile unsigned int th1                :  9 ; // [ 8 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} MONO_PWR_U ;


//-----------------------------------------------------------------------------
// Register name             : RF_CFG_DLY
// Register offset           : 0x00A0
// Register description      : RF Config Delay
// Register field description:
//    fld_val            : [15 :  0] RF Config Delay 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define RF_CFG_DLY_VAL_SHIFT                   0
#define RF_CFG_DLY_VAL_MSK                     (0xFFFF << RF_CFG_DLY_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _rf_cfg_dly_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _rf_cfg_dly_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _rf_cfg_dly_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} RF_CFG_DLY_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_TBL_STS
// Register offset           : 0x00A4
// Register description      : AGC Table Status
// Register field description:
//    fld_sel           : [      4] AGC Table Select Status 
//    fld_idx           : [ 3 :  0] AGC Table Index Status 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_TBL_STS_IDX_SHIFT                 0
#define AGC_TBL_STS_IDX_MSK                   (0xF << AGC_TBL_STS_IDX_SHIFT)
#define AGC_TBL_STS_SEL                       BIT_4

//------------------------- Data Prototype ------------------------------------
typedef union _agc_tbl_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_tbl_sts_map
    {
        volatile unsigned int idx               :  4 ; // [ 3 :  0] 
        volatile unsigned int sel               :  1 ; // [      4] 
        volatile unsigned int reserved_5        : 27 ; // [31 :  5] 
    } mBits ;

#else

    struct _agc_tbl_sts_map
    {
        volatile unsigned int reserved_5        : 27 ; // [31 :  5] 
        volatile unsigned int sel               :  1 ; // [      4] 
        volatile unsigned int idx               :  4 ; // [ 3 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_TBL_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : ADP_BIT_SFT
// Register offset           : 0x00A8
// Register description      : Pre-demodulation Adaptive Bit Shift
// Register field description:
//    fld_val           : [ 3 :  0] Pre-demodulation Adaptive Bit Shift 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define ADP_BIT_SFT_VAL_SHIFT                 0
#define ADP_BIT_SFT_VAL_MSK                   (0xF << ADP_BIT_SFT_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _adp_bit_sft_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _adp_bit_sft_map
    {
        volatile unsigned int val               :  4 ; // [ 3 :  0] 
        volatile unsigned int reserved_4        : 28 ; // [31 :  4] 
    } mBits ;

#else

    struct _adp_bit_sft_map
    {
        volatile unsigned int reserved_4        : 28 ; // [31 :  4] 
        volatile unsigned int val               :  4 ; // [ 3 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} ADP_BIT_SFT_U ;


//-----------------------------------------------------------------------------
// Register name             : SEEK_CNT
// Register offset           : 0x00AC
// Register description      : Current Seek channel Counter
// Register field description:
//    fld_val            : [ 9 :  0] Current Seek channel Counter 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SEEK_CNT_VAL_SHIFT                     0
#define SEEK_CNT_VAL_MSK                       (0x3FF << SEEK_CNT_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _seek_cnt_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _seek_cnt_map
    {
        volatile unsigned int val                : 10 ; // [ 9 :  0] 
        volatile unsigned int reserved_10        : 22 ; // [31 : 10] 
    } mBits ;

#else

    struct _seek_cnt_map
    {
        volatile unsigned int reserved_10        : 22 ; // [31 : 10] 
        volatile unsigned int val                : 10 ; // [ 9 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SEEK_CNT_U ;


//-----------------------------------------------------------------------------
// Register name             : RSSI_STS
// Register offset           : 0x00B0
// Register description      : RSSI Status Register
// Register field description:
//    fld_pilot_det        : [19 : 18] Pilot detection status 
//    fld_st               : [     17] Stero Indicator.
//                                     0: Mono
//                                     1: Stereo 
//    fld_sf               : [     16] Seek Fail Status.
//                                     0: Seek successful
//                                     1: Seek failure 
//    fld_rssi             : [ 7 :  0] Received Signal Strength Indicator 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define RSSI_STS_RSSI_SHIFT                      0
#define RSSI_STS_RSSI_MSK                        (0xFF << RSSI_STS_RSSI_SHIFT)
#define RSSI_STS_SF                              BIT_16
#define RSSI_STS_ST                              BIT_17
#define RSSI_STS_PILOT_DET_SHIFT                 18
#define RSSI_STS_PILOT_DET_MSK                   (0x3 << RSSI_STS_PILOT_DET_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _rssi_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _rssi_sts_map
    {
        volatile unsigned int rssi                 :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8           :  8 ; // [15 :  8] 
        volatile unsigned int sf                   :  1 ; // [     16] 
        volatile unsigned int st                   :  1 ; // [     17] 
        volatile unsigned int pilot_det            :  2 ; // [19 : 18] 
        volatile unsigned int reserved_20          : 12 ; // [31 : 20] 
    } mBits ;

#else

    struct _rssi_sts_map
    {
        volatile unsigned int reserved_20          : 12 ; // [31 : 20] 
        volatile unsigned int pilot_det            :  2 ; // [19 : 18] 
        volatile unsigned int st                   :  1 ; // [     17] 
        volatile unsigned int sf                   :  1 ; // [     16] 
        volatile unsigned int reserved_8           :  8 ; // [15 :  8] 
        volatile unsigned int rssi                 :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} RSSI_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : CHAN_FREQ_STS
// Register offset           : 0x00B8
// Register description      : Current channel frequence
// Register field description:
//    fld_val            : [15 :  0] Current channel frequence status  
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define CHAN_FREQ_STS_VAL_SHIFT                0
#define CHAN_FREQ_STS_VAL_MSK                  (0xFFFF << CHAN_FREQ_STS_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _chan_freq_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _chan_freq_sts_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _chan_freq_sts_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} CHAN_FREQ_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : FREQ_OFF_STS
// Register offset           : 0x00BC
// Register description      : Frequence offset status
// Register field description:
//    fld_val            : [15 :  0] Frequence offset status 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define FREQ_OFF_STS_VAL_SHIFT                 0
#define FREQ_OFF_STS_VAL_MSK                   (0xFFFF << FREQ_OFF_STS_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _freq_off_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _freq_off_sts_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _freq_off_sts_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} FREQ_OFF_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : INPWR_STS
// Register offset           : 0x00C0
// Register description      : Input power status
// Register field description:
//    fld_val           : [ 8 :  0] Input power status 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define INPWR_STS_VAL_SHIFT                   0
#define INPWR_STS_VAL_MSK                     (0x1FF << INPWR_STS_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _inpwr_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _inpwr_sts_map
    {
        volatile unsigned int val               :  9 ; // [ 8 :  0] 
        volatile unsigned int reserved_9        : 23 ; // [31 :  9] 
    } mBits ;

#else

    struct _inpwr_sts_map
    {
        volatile unsigned int reserved_9        : 23 ; // [31 :  9] 
        volatile unsigned int val               :  9 ; // [ 8 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} INPWR_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : RF_RSSI_STS
// Register offset           : 0x00C4
// Register description      : RF RSSI status
// Register field description:
//    fld_val            : [15 :  0] RF RSSI status value 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define RF_RSSI_STS_VAL_SHIFT                  0
#define RF_RSSI_STS_VAL_MSK                    (0xFFFF << RF_RSSI_STS_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _rf_rssi_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _rf_rssi_sts_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _rf_rssi_sts_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} RF_RSSI_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : NO_LPF_STS
// Register offset           : 0x00C8
// Register description      : NO LPF status
// Register field description:
//    fld_val           : [ 1 :  0] NO LPF status value 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define NO_LPF_STS_VAL_SHIFT                  0
#define NO_LPF_STS_VAL_MSK                    (0x3 << NO_LPF_STS_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _no_lpf_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _no_lpf_sts_map
    {
        volatile unsigned int val               :  2 ; // [ 1 :  0] 
        volatile unsigned int reserved_2        : 30 ; // [31 :  2] 
    } mBits ;

#else

    struct _no_lpf_sts_map
    {
        volatile unsigned int reserved_2        : 30 ; // [31 :  2] 
        volatile unsigned int val               :  2 ; // [ 1 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} NO_LPF_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : SMUTE_STS
// Register offset           : 0x00CC
// Register description      : Software mute status
// Register field description:
//    fld_val           : [      0] Software mute status value 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SMUTE_STS_VAL                         BIT_0

//------------------------- Data Prototype ------------------------------------
typedef union _smute_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _smute_sts_map
    {
        volatile unsigned int val               :  1 ; // [      0] 
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
    } mBits ;

#else

    struct _smute_sts_map
    {
        volatile unsigned int reserved_1        : 31 ; // [31 :  1] 
        volatile unsigned int val               :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SMUTE_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : WBRSSI_STS
// Register offset           : 0x00D0
// Register description      : WideBand RSSI status
// Register field description:
//    fld_val           : [ 7 :  0] Wideband RSSI status value 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define WBRSSI_STS_VAL_SHIFT                  0
#define WBRSSI_STS_VAL_MSK                    (0xFF << WBRSSI_STS_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _wbrssi_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _wbrssi_sts_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _wbrssi_sts_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} WBRSSI_STS_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL0
// Register offset           : 0x100
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL0_VAL_SHIFT                0
#define AGC_BITS_TBL0_VAL_MSK                  (0xFFFF << AGC_BITS_TBL0_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl0_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl0_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl0_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL0_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL1
// Register offset           : 0x104
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL1_VAL_SHIFT                0
#define AGC_BITS_TBL1_VAL_MSK                  (0xFFFF << AGC_BITS_TBL1_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl1_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl1_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl1_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL1_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL2
// Register offset           : 0x108
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL2_VAL_SHIFT                0
#define AGC_BITS_TBL2_VAL_MSK                  (0xFFFF << AGC_BITS_TBL2_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl2_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl2_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl2_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL2_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL3
// Register offset           : 0x10c
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL3_VAL_SHIFT                0
#define AGC_BITS_TBL3_VAL_MSK                  (0xFFFF << AGC_BITS_TBL3_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl3_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl3_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl3_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL3_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL4
// Register offset           : 0x110
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL4_VAL_SHIFT                0
#define AGC_BITS_TBL4_VAL_MSK                  (0xFFFF << AGC_BITS_TBL4_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl4_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl4_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl4_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL4_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL5
// Register offset           : 0x114
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL5_VAL_SHIFT                0
#define AGC_BITS_TBL5_VAL_MSK                  (0xFFFF << AGC_BITS_TBL5_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl5_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl5_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl5_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL5_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL6
// Register offset           : 0x118
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL6_VAL_SHIFT                0
#define AGC_BITS_TBL6_VAL_MSK                  (0xFFFF << AGC_BITS_TBL6_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl6_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl6_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl6_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL6_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL7
// Register offset           : 0x11c
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL7_VAL_SHIFT                0
#define AGC_BITS_TBL7_VAL_MSK                  (0xFFFF << AGC_BITS_TBL7_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl7_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl7_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl7_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL7_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL8
// Register offset           : 0x120
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL8_VAL_SHIFT                0
#define AGC_BITS_TBL8_VAL_MSK                  (0xFFFF << AGC_BITS_TBL8_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl8_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl8_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl8_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL8_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL9
// Register offset           : 0x124
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL9_VAL_SHIFT                0
#define AGC_BITS_TBL9_VAL_MSK                  (0xFFFF << AGC_BITS_TBL9_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl9_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl9_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl9_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL9_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL10
// Register offset           : 0x128
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL10_VAL_SHIFT               0
#define AGC_BITS_TBL10_VAL_MSK                 (0xFFFF << AGC_BITS_TBL10_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl10_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl10_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl10_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL10_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL11
// Register offset           : 0x12c
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL11_VAL_SHIFT               0
#define AGC_BITS_TBL11_VAL_MSK                 (0xFFFF << AGC_BITS_TBL11_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl11_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl11_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl11_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL11_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL12
// Register offset           : 0x130
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL12_VAL_SHIFT               0
#define AGC_BITS_TBL12_VAL_MSK                 (0xFFFF << AGC_BITS_TBL12_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl12_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl12_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl12_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL12_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL13
// Register offset           : 0x134
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL13_VAL_SHIFT               0
#define AGC_BITS_TBL13_VAL_MSK                 (0xFFFF << AGC_BITS_TBL13_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl13_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl13_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl13_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL13_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL14
// Register offset           : 0x138
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL14_VAL_SHIFT               0
#define AGC_BITS_TBL14_VAL_MSK                 (0xFFFF << AGC_BITS_TBL14_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl14_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl14_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl14_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL14_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL15
// Register offset           : 0x13c
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL15_VAL_SHIFT               0
#define AGC_BITS_TBL15_VAL_MSK                 (0xFFFF << AGC_BITS_TBL15_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl15_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl15_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl15_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL15_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL16
// Register offset           : 0x140
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL16_VAL_SHIFT               0
#define AGC_BITS_TBL16_VAL_MSK                 (0xFFFF << AGC_BITS_TBL16_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl16_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl16_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl16_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL16_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BITS_TBL17
// Register offset           : 0x144
// Register description      : AGC Bits Table
// Register field description:
//    fld_val            : [15 :  0] AGC Bits Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BITS_TBL17_VAL_SHIFT               0
#define AGC_BITS_TBL17_VAL_MSK                 (0xFFFF << AGC_BITS_TBL17_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bits_tbl17_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bits_tbl17_map
    {
        volatile unsigned int val                : 16 ; // [15 :  0] 
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
    } mBits ;

#else

    struct _agc_bits_tbl17_map
    {
        volatile unsigned int reserved_16        : 16 ; // [31 : 16] 
        volatile unsigned int val                : 16 ; // [15 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BITS_TBL17_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL0
// Register offset           : 0x200
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL0_VAL_SHIFT                0
#define AGC_VAL_TBL0_VAL_MSK                  (0xFF << AGC_VAL_TBL0_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl0_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl0_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl0_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL0_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL1
// Register offset           : 0x204
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL1_VAL_SHIFT                0
#define AGC_VAL_TBL1_VAL_MSK                  (0xFF << AGC_VAL_TBL1_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl1_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl1_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl1_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL1_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL2
// Register offset           : 0x208
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL2_VAL_SHIFT                0
#define AGC_VAL_TBL2_VAL_MSK                  (0xFF << AGC_VAL_TBL2_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl2_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl2_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl2_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL2_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL3
// Register offset           : 0x20c
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL3_VAL_SHIFT                0
#define AGC_VAL_TBL3_VAL_MSK                  (0xFF << AGC_VAL_TBL3_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl3_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl3_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl3_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL3_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL4
// Register offset           : 0x210
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL4_VAL_SHIFT                0
#define AGC_VAL_TBL4_VAL_MSK                  (0xFF << AGC_VAL_TBL4_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl4_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl4_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl4_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL4_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL5
// Register offset           : 0x214
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL5_VAL_SHIFT                0
#define AGC_VAL_TBL5_VAL_MSK                  (0xFF << AGC_VAL_TBL5_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl5_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl5_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl5_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL5_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL6
// Register offset           : 0x218
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL6_VAL_SHIFT                0
#define AGC_VAL_TBL6_VAL_MSK                  (0xFF << AGC_VAL_TBL6_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl6_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl6_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl6_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL6_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL7
// Register offset           : 0x21c
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL7_VAL_SHIFT                0
#define AGC_VAL_TBL7_VAL_MSK                  (0xFF << AGC_VAL_TBL7_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl7_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl7_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl7_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL7_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL8
// Register offset           : 0x220
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL8_VAL_SHIFT                0
#define AGC_VAL_TBL8_VAL_MSK                  (0xFF << AGC_VAL_TBL8_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl8_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl8_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl8_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL8_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL9
// Register offset           : 0x224
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL9_VAL_SHIFT                0
#define AGC_VAL_TBL9_VAL_MSK                  (0xFF << AGC_VAL_TBL9_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl9_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl9_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl9_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL9_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL10
// Register offset           : 0x228
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL10_VAL_SHIFT               0
#define AGC_VAL_TBL10_VAL_MSK                 (0xFF << AGC_VAL_TBL10_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl10_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl10_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl10_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL10_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL11
// Register offset           : 0x22c
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL11_VAL_SHIFT               0
#define AGC_VAL_TBL11_VAL_MSK                 (0xFF << AGC_VAL_TBL11_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl11_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl11_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl11_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL11_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL12
// Register offset           : 0x230
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL12_VAL_SHIFT               0
#define AGC_VAL_TBL12_VAL_MSK                 (0xFF << AGC_VAL_TBL12_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl12_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl12_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl12_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL12_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL13
// Register offset           : 0x234
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL13_VAL_SHIFT               0
#define AGC_VAL_TBL13_VAL_MSK                 (0xFF << AGC_VAL_TBL13_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl13_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl13_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl13_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL13_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL14
// Register offset           : 0x238
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL14_VAL_SHIFT               0
#define AGC_VAL_TBL14_VAL_MSK                 (0xFF << AGC_VAL_TBL14_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl14_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl14_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl14_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL14_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL15
// Register offset           : 0x23c
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL15_VAL_SHIFT               0
#define AGC_VAL_TBL15_VAL_MSK                 (0xFF << AGC_VAL_TBL15_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl15_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl15_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl15_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL15_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL16
// Register offset           : 0x240
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL16_VAL_SHIFT               0
#define AGC_VAL_TBL16_VAL_MSK                 (0xFF << AGC_VAL_TBL16_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl16_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl16_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl16_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL16_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_VAL_TBL17
// Register offset           : 0x244
// Register description      : AGC Value Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Value Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_VAL_TBL17_VAL_SHIFT               0
#define AGC_VAL_TBL17_VAL_MSK                 (0xFF << AGC_VAL_TBL17_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_val_tbl17_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_val_tbl17_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_val_tbl17_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_VAL_TBL17_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL0
// Register offset           : 0x300
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL0_VAL_SHIFT               0
#define AGC_BOND_TBL0_VAL_MSK                 (0xFF << AGC_BOND_TBL0_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl0_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl0_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl0_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL0_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL1
// Register offset           : 0x304
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL1_VAL_SHIFT               0
#define AGC_BOND_TBL1_VAL_MSK                 (0xFF << AGC_BOND_TBL1_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl1_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl1_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl1_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL1_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL2
// Register offset           : 0x308
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL2_VAL_SHIFT               0
#define AGC_BOND_TBL2_VAL_MSK                 (0xFF << AGC_BOND_TBL2_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl2_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl2_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl2_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL2_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL3
// Register offset           : 0x30c
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL3_VAL_SHIFT               0
#define AGC_BOND_TBL3_VAL_MSK                 (0xFF << AGC_BOND_TBL3_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl3_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl3_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl3_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL3_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL4
// Register offset           : 0x310
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL4_VAL_SHIFT               0
#define AGC_BOND_TBL4_VAL_MSK                 (0xFF << AGC_BOND_TBL4_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl4_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl4_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl4_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL4_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL5
// Register offset           : 0x314
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL5_VAL_SHIFT               0
#define AGC_BOND_TBL5_VAL_MSK                 (0xFF << AGC_BOND_TBL5_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl5_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl5_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl5_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL5_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL6
// Register offset           : 0x318
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL6_VAL_SHIFT               0
#define AGC_BOND_TBL6_VAL_MSK                 (0xFF << AGC_BOND_TBL6_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl6_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl6_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl6_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL6_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL7
// Register offset           : 0x31c
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL7_VAL_SHIFT               0
#define AGC_BOND_TBL7_VAL_MSK                 (0xFF << AGC_BOND_TBL7_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl7_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl7_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl7_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL7_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL8
// Register offset           : 0x320
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL8_VAL_SHIFT               0
#define AGC_BOND_TBL8_VAL_MSK                 (0xFF << AGC_BOND_TBL8_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl8_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl8_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl8_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL8_U ;


//-----------------------------------------------------------------------------
// Register name             : AGC_BOND_TBL9
// Register offset           : 0x324
// Register description      : AGC Boundary Table
// Register field description:
//    fld_val           : [ 7 :  0] AGC Boundary Table 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define AGC_BOND_TBL9_VAL_SHIFT               0
#define AGC_BOND_TBL9_VAL_MSK                 (0xFF << AGC_BOND_TBL9_VAL_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _agc_bond_tbl9_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _agc_bond_tbl9_map
    {
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
    } mBits ;

#else

    struct _agc_bond_tbl9_map
    {
        volatile unsigned int reserved_8        : 24 ; // [31 :  8] 
        volatile unsigned int val               :  8 ; // [ 7 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} AGC_BOND_TBL9_U ;


//-----------------------------------------------------------------------------
// Register name             : SPI_CTRL
// Register offset           : 0x0810
// Register description      : SPI Control Register
// Register field description:
//    fld_rd_stp          : [20 : 16] Start position of read data[0] when spi read 
//    fld_tri_en          : [     12] Tri-state enable 
//    fld_twocs_en        : [     11] 0:single chip select signals
//                                    1:two chip select signals 
//    fld_wclk_inv        : [     10] 1:Invert clk_spi when SPI write 
//    fld_rclk_inv        : [      9] 1:Invert clk_spi when SPI read 
//    fld_mode            : [      8] SPI work Mode.
//                                    0: Normal mode
//                                    1:Renasense mode 
//    fld_len             : [ 4 :  0] SPI receive/transmit data bit width 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SPI_CTRL_LEN_SHIFT                      0
#define SPI_CTRL_LEN_MSK                        (0x1F << SPI_CTRL_LEN_SHIFT)
#define SPI_CTRL_MODE                           BIT_8
#define SPI_CTRL_RCLK_INV                       BIT_9
#define SPI_CTRL_WCLK_INV                       BIT_10
#define SPI_CTRL_TWOCS_EN                       BIT_11
#define SPI_CTRL_TRI_EN                         BIT_12
#define SPI_CTRL_RD_STP_SHIFT                   16
#define SPI_CTRL_RD_STP_MSK                     (0x1F << SPI_CTRL_RD_STP_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _spi_ctrl_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _spi_ctrl_map
    {
        volatile unsigned int len                 :  5 ; // [ 4 :  0] 
        volatile unsigned int reserved_5          :  3 ; // [ 7 :  5] 
        volatile unsigned int mode                :  1 ; // [      8] 
        volatile unsigned int rclk_inv            :  1 ; // [      9] 
        volatile unsigned int wclk_inv            :  1 ; // [     10] 
        volatile unsigned int twocs_en            :  1 ; // [     11] 
        volatile unsigned int tri_en              :  1 ; // [     12] 
        volatile unsigned int reserved_13         :  3 ; // [15 : 13] 
        volatile unsigned int rd_stp              :  5 ; // [20 : 16] 
        volatile unsigned int reserved_21         : 11 ; // [31 : 21] 
    } mBits ;

#else

    struct _spi_ctrl_map
    {
        volatile unsigned int reserved_21         : 11 ; // [31 : 21] 
        volatile unsigned int rd_stp              :  5 ; // [20 : 16] 
        volatile unsigned int reserved_13         :  3 ; // [15 : 13] 
        volatile unsigned int tri_en              :  1 ; // [     12] 
        volatile unsigned int twocs_en            :  1 ; // [     11] 
        volatile unsigned int wclk_inv            :  1 ; // [     10] 
        volatile unsigned int rclk_inv            :  1 ; // [      9] 
        volatile unsigned int mode                :  1 ; // [      8] 
        volatile unsigned int reserved_5          :  3 ; // [ 7 :  5] 
        volatile unsigned int len                 :  5 ; // [ 4 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SPI_CTRL_U ;


//-----------------------------------------------------------------------------
// Register name             : SPI_WD0
// Register offset           : 0x0814
// Register description      : SPI Write Data Register0
// Register field description:
//    fld_data        : [31 :  0] In write mode, DATA will be send by SPI, if for read action, SPI_WD DATA is not be used 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SPI_WD0_DATA_SHIFT                  0
#define SPI_WD0_DATA_MSK                    (0xFFFFFFFF << SPI_WD0_DATA_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _spi_wd0_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _spi_wd0_map
    {
        volatile unsigned int data            : 32 ; // [31 :  0] 
    } mBits ;

#else

    struct _spi_wd0_map
    {
        volatile unsigned int data            : 32 ; // [31 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SPI_WD0_U ;


//-----------------------------------------------------------------------------
// Register name             : SPI_WD1
// Register offset           : 0x0818
// Register description      : SPI Write Data Register1
// Register field description:
//    fld_data        : [31 :  0] In write mode, DATA will be send by SPI, if for read action, SPI_WD DATA is not be used 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SPI_WD1_DATA_SHIFT                  0
#define SPI_WD1_DATA_MSK                    (0xFFFFFFFF << SPI_WD1_DATA_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _spi_wd1_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _spi_wd1_map
    {
        volatile unsigned int data            : 32 ; // [31 :  0] 
    } mBits ;

#else

    struct _spi_wd1_map
    {
        volatile unsigned int data            : 32 ; // [31 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SPI_WD1_U ;


//-----------------------------------------------------------------------------
// Register name             : SPI_RD
// Register offset           : 0x081C
// Register description      : SPI Read Data Regsiter
// Register field description:
//    fld_data        : [31 :  0] The return data of SPI read action 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SPI_RD_DATA_SHIFT                   0
#define SPI_RD_DATA_MSK                     (0xFFFFFFFF << SPI_RD_DATA_SHIFT)

//------------------------- Data Prototype ------------------------------------
typedef union _spi_rd_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _spi_rd_map
    {
        volatile unsigned int data            : 32 ; // [31 :  0] 
    } mBits ;

#else

    struct _spi_rd_map
    {
        volatile unsigned int data            : 32 ; // [31 :  0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SPI_RD_U ;


//-----------------------------------------------------------------------------
// Register name             : SPI_FIFO_STS
// Register offset           : 0x0820
// Register description      : SPI FIFO status
// Register field description:
//    fld_ie            : [      3] Input FIFO Empyt 
//    fld_if            : [      2] Input FIFO Full 
//    fld_oe            : [      1] Output FIFO Empty 
//    fld_of            : [      0] Output FIFO Full 
//-----------------------------------------------------------------------------

//------------------------- Fields Definition ---------------------------------
#define SPI_FIFO_STS_OF                       BIT_0
#define SPI_FIFO_STS_OE                       BIT_1
#define SPI_FIFO_STS_IF                       BIT_2
#define SPI_FIFO_STS_IE                       BIT_3

//------------------------- Data Prototype ------------------------------------
typedef union _spi_fifo_sts_tag
{ 

#ifdef CHIP_ENDIAN_LITTLE

    struct _spi_fifo_sts_map
    {
        volatile unsigned int of                :  1 ; // [      0] 
        volatile unsigned int oe                :  1 ; // [      1] 
        volatile unsigned int inf               :  1 ; // [      2]  //if
        volatile unsigned int ie                :  1 ; // [      3] 
        volatile unsigned int reserved_4        : 28 ; // [31 :  4] 
    } mBits ;

#else

    struct _spi_fifo_sts_map
    {
        volatile unsigned int reserved_4        : 28 ; // [31 :  4] 
        volatile unsigned int ie                :  1 ; // [      3] 
        volatile unsigned int inf               :  1 ; // [      2]  //if
        volatile unsigned int oe                :  1 ; // [      1] 
        volatile unsigned int of                :  1 ; // [      0] 
    } mBits ;

#endif // CHIP_ENDIAN_LITTLE

    volatile unsigned int dwValue ;
} SPI_FIFO_STS_U ;
#endif

#if 0
typedef enum
{
	RF_SWITCH_K2_1,
	RF_SWITCH_K2_2,
	RF_SWITCH_K2_3,
	RF_SWITCH_K2_4,
	RF_SWITCH_K2_5,
	RF_SWITCH_K2_6,
	RF_SWITCH_K2_7,
	RF_SWITCH_K2_8,
	RF_SWITCH_K4_1,
}RF_SWITCH_NAME_E;

typedef enum
{
	RF_SWITCH_MODE_DIS,
	RF_SWITCH_MODE_EN,
}RF_SWITCH_MODE_E;
#endif

#define ARRAY_SIZE(_x) ((sizeof(_x))/(sizeof((_x)[0])))

int32 trout_test_fm_reg_check(void);
LOCAL BOOLEAN trout_fm_open_clk(void);
LOCAL BOOLEAN trout_fm_write_rf_reg(uint32 addr, uint32 reg_data);
LOCAL BOOLEAN trout_fm_cfg_rf_reg(void);
LOCAL BOOLEAN trout_fm_cfg_rf_reg(void);
void agc_tbl_reg_cfg(void);
LOCAL BOOLEAN trout_fm_reg_cfg(void);
LOCAL BOOLEAN trout_fm_iis_pin_cfg(void);
int fm_audio_play(void);
void vbc_test_entry ( void );
int32 trout_fm_init(void);
PUBLIC void trout_fm_print_sts(void);
PUBLIC void trout_fm_status_reg_read(void);
PUBLIC void trout_test_fm_status_reg_print(void);
void trout_fm_irq_handler(uint32 int_sts);
PUBLIC void trout_fm_wait_int(void);
PUBLIC void trout_fm_open(void);
void trout_fm_int_en(void);
void trout_fm_int_dis(void);
PUBLIC void trout_fm_dis(void);
PUBLIC void trout_fm_en(void);
PUBLIC void trout_fm_seek_up(void);
PUBLIC void trout_fm_seek_down(void);
PUBLIC void trout_fm_tune(uint32 chan, uint32 flag);
PUBLIC void trout_fm_seek(uint32 direction);
PUBLIC void trout_fm_continue_seek(uint32 freq);
PUBLIC void trout_test_fm_init(void);
PUBLIC void trout_test_fm_rf_reg_read(void);
PUBLIC void trout_test_fm_continue_seek(void);
PUBLIC void trout_test_fm_seek_up(void);
PUBLIC void trout_test_fm_seek_down(void);
PUBLIC void trout_test_fm_seek_new(void);
PUBLIC void trout_test_fm_tune(void);
void trout_test_fm_tune_repeat();
PUBLIC void trout_test_fm_seek_manual(void);
void trout_test_data_capture();
PUBLIC void FM_audio_debug();
void FM_reg_dump();
void trout_sw_mute_test();
void trout_test_rds_print_reg();
void trout_test_rds_read_fifo();
void trout_test_rds_arm_mode();
void IQ_imbalance_calibration();
int32 trout_init(void);
int32 trout_test_main(void);
int32 test_entry(void);

extern  BOOLEAN EFUSE_HAL_GetBTCaliData (uint32 *cali_data);

#define FM_SC6531EFM_FPGA 0

#define powr_offset (0)
#define BOUNDARY_OFFSET (6)
#define FM_SEEK_STEP (100)
#define FM_AGC_TBL_NUM_EVB  (13)
//#define 	FM_AGC_TBL_NUM 			(26)
#define 	FM_AGC_TBL_NUM 			(11)//(13)
//#define 	FM_AGC_TBL_NUM 			(30)


#define FM_REG_NUM	(sizeof(fm_reg_list)/sizeof(REG_ELE_T))

fm_chan_info_t fm_chan_info[100];
VOLATILE uint32 rds_int_happen = FALSE;

#if  FM_SC6531EFM_FPGA
uint32 value_table[FM_AGC_TBL_NUM] = 
{
	7, 13, 21, 29, 35, 41, 47, 53, 59, 65, 71,
};

uint32 bits_table[FM_AGC_TBL_NUM] = 
{
	0,
};

uint32 tia_dcoc[FM_AGC_TBL_NUM] = 
{
	0x0, 0x1, 0x101, 0x301, 0x302, 0x303, 0x313, 0x323, 0x333, 0x334, 0x335,
};

uint32 pga_dcoc[FM_AGC_TBL_NUM] = 
{
	0,
};

uint32 boundary[FM_AGC_TBL_NUM + 1] = 
{
	//136, 179, 182, 188, 194, 200, 206, 212, 218, 224, 230, 10 //pre
	136, 179, 185, 191, 197, 203, 209, 215, 221, 227, 233, 20
	//136,185,188,194,200,206,212,218,224,230,236,242
};
#else  ////nemog openphone
uint32 value_table[FM_AGC_TBL_NUM_EVB] = 
{
    ////  1,  7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73//marlin2
    (1+powr_offset),  (7+powr_offset),  (13+powr_offset),  (19+powr_offset),  (25+powr_offset),  (31+powr_offset),  (37+powr_offset),  (43+powr_offset),  (49+powr_offset),  (55+powr_offset),  (61+powr_offset),  (67+powr_offset),  (73+powr_offset)//0909CASE2 //pre
       //7, 13, 21, 29, 35, 41, 47, 53, 59, 65, 71, //marlin2 fpga
   //     1,  7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  67// 2 xuede.wang
};

uint32 bits_table[FM_AGC_TBL_NUM_EVB] = 
{
	0x7A,  0x78,  0x76,  0x66,  0x56,  0x46,  0x36,  0x26,  0x24,  0x14,  0x4,  0x02,  0x00//0909CASE2 pre
	//0x7A,  0x78,  0x76,  0x66,  0x56,  0x46,  0x36,  0x26,  0x24,  0x14,  0x4,  0x00,  0x00//0909CASE2 pre
	//0x7A,  0x78,  0x76,  0x66,  0x56,  0x46,  0x36,  0x34,  0x32,  0x30,  0x20,  0x10,  0x00 /////1 shine.zhao
	///0x7A,  0x78,  0x76,  0x66,  0x56,  0x46,  0x44,  0x42,  0x40,  0x30,  0x20,  0x10,  0x00 /////2 shine.zhao
	//0x7A,  0x78,  0x76,  0x74,  0x72,  0x70,  0x60,  0x50,  0x40,  0x30,  0x20,  0x10,  0x00 //  ///1 xuede.wang get better
	//0x7A,  0x78,  0x76,  0x66,  0x56,  0x46,  0x36,  0x26,  0x24,  0x14,  0x12,  0x10,  0x10 //  ///2 xuede.wang
	//0x7A,  0x78,  0x76,  0x74,  0x72,  0x62,  0x52,  0x42,  0x32,  0x22,  0x20,  0x10,  0x00 //  ///3 xuede.wang
	//0x7A,  0x78,  0x76,  0x74,  0x64,  0x54,  0x44,  0x34,  0x32,  0x22,  0x20,  0x10,  0x00 //  ///4 xuede.wang
	
	
};

uint32 tia_dcoc[FM_AGC_TBL_NUM_EVB] = 
{
	//0x0, 0x1, 0x101, 0x301, 0x302, 0x303, 0x313, 0x323, 0x333, 0x334, 0x335,  //marlin2 fpga
	0,
};
uint32 pga_dcoc[FM_AGC_TBL_NUM_EVB] = 
{
	0,
};

uint32 boundary[FM_AGC_TBL_NUM_EVB + 1] = 
{
//136, 176, 182, 188, 194, 200, 206, 212, 218, 224, 230, 236, 242, 16//20150923 pre
136, (176-BOUNDARY_OFFSET), (182-BOUNDARY_OFFSET), \
(188-BOUNDARY_OFFSET), (194-BOUNDARY_OFFSET),( 200-BOUNDARY_OFFSET), \
(206-BOUNDARY_OFFSET), (212-BOUNDARY_OFFSET), (218-BOUNDARY_OFFSET),\
(224-BOUNDARY_OFFSET), (230-BOUNDARY_OFFSET), (236-BOUNDARY_OFFSET), \
(242-BOUNDARY_OFFSET), 16// -60dbm baohe
};

#endif

// when tune or seek is done, some info are stored to it
VOLATILE trout_fm_info_t trout_fm_info = 
{
    TRUE,	// 	search_type
    TRUE,	//	search_direct
    FALSE,	// 	int_happen;
    FALSE,	//	seek_success;
    0,		//    freq_set;
    0,		//	freq_seek;
    0,		//	freq_offset;
    0,		//	rssi;
    0, 		//	rf_rssi;
    0, 		//	seek_cnt;
    0, 		//	inpwr_sts;
    0,  	//	fm_sts;
    0,  	//  agc_sts
    0,  	//  pilot_sts
  //  0,		//  SNR
    trout_fm_irq_handler, //  (*irq_handler)(U32);
};


#if  FM_SC6531EFM_FPGA
LOCAL  trout_reg_cfg_t fm_reg_init_des[] =
{
    {FM_REG_FM_CTL	,		0x1001011},
 	{FM_REG_FM_EN	,		0x112EE},//0x36EE
    {FM_REG_CHAN	,		(1 << 20) | 980},
    {FM_REG_AGC_TBL_CLK,	0},
    //{FM_REG_SEEK_LOOP	,	(1080-800)},
	{FM_REG_SEEK_LOOP	,	50},
    {FM_REG_FM_CTL_STI	,	0},
    //{FM_REG_BAND_LMT	,	(((1080-1)*2)<<16)|((860-1)*2)},
	{FM_REG_BAND_LMT	,	875/*650*/},
    {FM_REG_BAND_SPACE	,	(1 << 24) | 1080},
    {FM_REG_ADC_INFCTRL, 	0x7fff}, //new
    {FM_REG_SEEK_CH_TH	,	0x5dc},			//0x320  800
    //{FM_REG_AGC_TBL_RFRSSI0,	0x200800},	//zl@here for trout2
    {FM_REG_AGC_TBL_RFRSSI0,0x6f0800},		// bit30:16 rfRegAddr for gain ctrl
    {FM_REG_AGC_TBL_RFRSSI1,(31 << 24) | (5 << 16) | (3000)},		//0x1f050bb8
    {FM_REG_AGC_TBL_WBRSSI,	0x00F60C12},
    //{FM_REG_AGC_IDX_TH	,	0x00020607},
	{FM_REG_AGC_IDX_TH	,	0x00020607},
    {FM_REG_AGC_RSSI_TH	,	0x00F600D8},
    {FM_REG_SEEK_ADPS	,	0x0CD80000},
    {FM_REG_STER_PWR	,	0x000001A6},//
    {FM_REG_MONO_PWR	,	0x01A30199},//0x019C0192
    {FM_REG_AGC_CTL	,		0x09040120},
    {FM_REG_AGC_ITV_TH	,	0x00FE00EE},
    {FM_REG_AGC_ADPS0	,	0xFDF70009},
    {FM_REG_AGC_ADPS1	,	0xFEF80003},
    {FM_REG_PDP_TH	,		400},
    {FM_REG_PDP_DEV	,		(0x78<<16)|0x78},
    {FM_REG_ADP_ST_CONF	,	0x200501BA},  
    {FM_REG_ADP_LPF_CONF,	2},
    {FM_REG_DEPHA_SCAL	,	5},
    {FM_REG_HW_MUTE	,		0x70100},
    {FM_REG_SW_MUTE	,		0x01A601A1},
    //{FM_REG_UPD_CTRL	,	0x01013356},
    {FM_REG_UPD_CTRL	,	0x0B0A3356}, /**/
    {FM_REG_AUD_BLD0	,	0x01AD01A1},
    {FM_REG_AUD_BLD1	,	0x01A901A5},	/*1a901a5*/	//bit_31???		//??? mode assign 1
    {FM_REG_AGC_HYS	,		0x00000202},
	
    {FM_REG_RF_CFG_DLY	,	128000},
    {FM_REG_SPUR_FREQ0    , 0x084E081C},
    {FM_REG_SPUR_FREQ1    , 0x06DC063C},
	{FM_REG_IIS_CONFIG4 ,	0x100190},
	{FM_REG_REG_CTRL1   , 	0x2E35A0},
	{FM_REG_AMP_ADJ   	, 	0x01000581},
	{FM_REG_DB_COMP   	, 	0x41E},/*0x413*/
	{FM_REG_RF_INIT_GAIN  ,	0x0313},
};
#else
#define fm_rf_fracL_addr 34
#define fm_rf_integer_addr  32
#define fm_rf_afc_aac_start_addr  41
#define fm_rf_fracH_addr 33
#define fm_rf_gain_addr  31
#define fm_rf_lodiv_addr  35
#define fm_rf_short_ant_addr   ((0x1a<<27)| 44)
#define fm_rf_dcoc_addr  (FM_RX_TIADCOC_CTRL0)
#define fm_rf_rc_dcoc_cal_addr  (REG_FM_RC_DCOC_CAL)

LOCAL  trout_reg_cfg_t fm_reg_init_des[] =
{
    {FM_REG_FM_CTL	,		0x2001011}, //modify by xuede.wang   //[0]
    {FM_REG_FM_EN	,		0x112EE},//0x36EE                           //[1]
    {FM_REG_CHAN	,		(128 << 20) | 98000},                            //[2]
    {FM_REG_AGC_TBL_CLK,	0},                                                        //[3]
    {FM_REG_SEEK_LOOP	,	20}, //pre:10 marlin: 50 modified by george.wang 20160402  //[4]
    {FM_REG_FM_CTL_STI	,	0},                                                 //[5]
    {FM_REG_BAND_LMT	,	87500},                                                 //[6]
    {FM_REG_BAND_SPACE	,	(100 << 24) | 108000},                   //[7]
    //{FM_REG_RFCTRL0,		(226<< 16) | 230},  //maybe change xuede confirm
    //{FM_REG_RFCTRL0,		(31<< 16) | 35}, //fm_rf_gain_addr  fm_rf_lodiv_addr
    {FM_REG_RFCTRL0,		(fm_rf_gain_addr<< 16) | fm_rf_lodiv_addr},                     //[8]
   // {FM_REG_ADC_BITMAP,		0xDC0000 | (fm_rf_dcoc_addr&0x7FFF)},//fm dcoc work by debug mode
    {FM_REG_ADC_BITMAP,		0xDC0000 | (0x7FFF)}, // fm dcoc work by normal mode   //[9]
    //{FM_REG_ADC_INFCTRL, 	(fm_rf_rc_dcoc_cal_addr&0x7FFF)}, //fm dcoc work by debug mode
    {FM_REG_ADC_INFCTRL, 0x7FFF}, //dcoc offset addr of rf, pga         //[10]
    {FM_REG_SEEK_CH_TH	,	3000},	                                      //[11]
    {FM_REG_AGC_TBL_RFRSSI0,0x6f0800},		// bit30:16 rfRegAddr for gain ctrl    //[12]
    {FM_REG_AGC_TBL_RFRSSI1,(31 << 24) | (5 << 16) | (3000)},		//0x1f050bb8  //[13]
    {FM_REG_AGC_TBL_WBRSSI,	0x00F60C12},                                //[14]
    {FM_REG_AGC_IDX_TH	,	0x00020607},                                //[15]
    {FM_REG_AGC_RSSI_TH	,	0x00F600D8},                                //[16]
    {FM_REG_SEEK_ADPS	,	0x0CD80000},                                //[17]
    {FM_REG_STER_PWR	,	0x000001A6},                                       //[18]
    {FM_REG_MONO_PWR	,	0x019C0197},//0x019C0192//-100 -105         //[19]
    {FM_REG_AGC_CTL	,		0x0D040120},//0x09040120 modify 20160401  //[20]
    {FM_REG_AGC_ITV_TH	,	0x00FE00EE},                                //[21]
    {FM_REG_AGC_ADPS0	,	0xFDF70009},                                //[22]
    {FM_REG_AGC_ADPS1	,	0xFEF80003},                                //[23]
    {FM_REG_PDP_TH	,		400},                                            //[24]
    {FM_REG_PDP_DEV	,		(0x64<<16)|0x64},                       //[25]
    {FM_REG_ADP_ST_CONF	,	0x200501BA},                                //[26]
    {FM_REG_ADP_LPF_CONF,	2},                                               //[27]
    {FM_REG_DEPHA_SCAL	,	5},                                                //[28]
    {FM_REG_HW_MUTE	,		0x70100},                                     //[29]
    {FM_REG_SW_MUTE	,		0x01A601A1},                                //[30]
    {FM_REG_UPD_CTRL	,	0x0B0A3356}, /**/                               //[31]
    {FM_REG_AUD_BLD0	,	0x01AD01A1},                                       //[32]
    {FM_REG_AUD_BLD1	,	0x01A901A5},	/*1a901a5*/	//bit_31???		//??? mode assign 1
    {FM_REG_AGC_HYS	,		0x00000202},                                 //[33]
    //{FM_REG_RF_CFG_DLY	,	128000}, //50ms
     {FM_REG_RF_CFG_DLY	,	128000 | (1 << 20)},//// 25ms       //[34]
    {FM_REG_SPUR_FREQ0    , 0x084ECB20},  //pre:0x084E081C, calc:0xcb20*2/1000=104MHz//0x084ECB20  //[35]
    {FM_REG_SPUR_FREQ1    , 0x06DC063C},                                      //[36]
    {FM_REG_IIS_CONFIG4 ,	0x100190},                                           //[37]
    {FM_REG_CIC16_FREQ,		0x7BE76C},//center freq codic,default +128K   //[38]
    {FM_REG_REG_CTRL1   , 	0x2E35A0},                                           //[39]
    {FM_REG_AMP_ADJ   	, 	0x3000540},
    //{FM_REG_RFCTRL1   	, 	(34 << 16) | 32}, //{fracL,interger}
    //{FM_REG_RFCTRL2   	, 	(41 << 16) | 33},   //{afc/aac_start,fracH}
    //{FM_REG_RFCTRL3   	, 	(0x1a<<27)| 44},   	 //{gain,lodiv}
    {FM_REG_RFCTRL1   	, 	(fm_rf_fracL_addr << 16) | fm_rf_integer_addr}, //{fracL,interger}   //[40]
    {FM_REG_RFCTRL2   	, 	(fm_rf_afc_aac_start_addr << 16) | fm_rf_fracH_addr},   //{afc/aac_start,fracH}   //[41]
    {FM_REG_RFCTRL3   	, 	fm_rf_short_ant_addr},   	 //{gain,lodiv}        //[42]
    {FM_REG_DB_COMP   	, 	(0<<8)|(24)},  //0x420  power value cali       //[43]
    {FM_REG_RF_INIT_GAIN  ,	0x28},                                                     //[44]
};

#endif


typedef struct 
{
uint16 stra_sel;
uint16 seek_ch_th;
uint16 mono_pwr_th1;
uint16 mono_pwr_th2;
uint16 mono_pwr_th3;
uint16 seek_vldch_fo_th1;
uint16 seek_vldch_fo_th2;
uint16 noise_pwr_th0;
uint16 noise_pwr_th1;
uint16 noise_pwr_th2;
uint16 pdp_th;
uint16 pdp_dev1;
uint16 pdp_dev2;
}fm_seek_para_t;


typedef struct
{
uint32 seek_loop;
uint32 band_lmt_L;  //fm seek freq high limit. 875: 87.5MHz, in china
uint32 band_lmt_H;  //fm seek freq high limit. 1080: 108MHz, in china
uint32 band_space; //bandwidth, 1:100khz
}fm_bandlmt_para_t;

typedef struct
{
uint32 rf_gain_tl_en;
uint32 rf_gain_value;
}fm_rf_fix_gain_t;

typedef struct
{
uint32 addr[5];
uint32 data[5];
}fm_reserved_t;

typedef struct
{
uint32 longantcondition[5];
uint32 shortantcondition[5];
}fm_search_condition_t;

typedef struct 
{
uint16 fm_nv_config_en; 
////fm nv para config enable 0:disable(fm use internal para), 1:enable(fm use nv para) 
////bit[0]:fm_seek_para, bit[1]:fm_bandlmt_para,bit[2]:ster_pwr, bit[3]:fm_reserved
fm_seek_para_t fm_seek_para;
fm_bandlmt_para_t fm_bandlmt_para;
uint32 ster_pwr;
fm_rf_fix_gain_t fm_rf_fix_gain;
fm_search_condition_t searchcondition;
}fm_config_t;

fm_config_t  fm_config;


//#define FM_NV_ID 0x417



