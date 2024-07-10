/******************************************************************************
 *
 ** File Name:      sprd_codec_ap_phy.h                                       *
 ** Author:         zhenfang.wang                                             *
 ** DATE:           8/5/2015                                                  *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec ap control  for sc1161                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 6/3/2016     ye.zhang             Create.                                 *
 *****************************************************************************/

#ifndef __SPRD_CODEC_AP_PHY_SC1161_H___
#define __SPRD_CODEC_AP_PHY_SC1161_H___

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_plf_export.h"

/**---------------------------------------------------------------------------*
 **                       A Die codec analog part                             *
 **                         Register description                              *
 **---------------------------------------------------------------------------*/

#define MASK_1_BIT   0x1
#define MASK_2_BITS  0x3
#define MASK_3_BITS  0x7
#define MASK_4_BITS  0xf
#define MASK_5_BITS  0x1f
#define MASK_6_BITS  0x3f
#define MASK_7_BITS  0x7f
#define MASK_8_BITS  0xff
#define MASK_16_BITS  0xffff

/// ANA_PMU0 0x00 ///

/* RG_AUD_VB_EN         [15]    RW  0   Audio LDO VB enable signal
 * RG_AUD_VB_NLEAK_PD   [14]    RW  0   Audio LDO VB prevent reverse flow back power down signal
                                        0 = power up          1 = power down
 * RG_AUD_VB_HDMC_SP_PD [13]    RW  0x1 Audio LDO VB SLEEP MODE PD signal
                                        0 = EN   1 = PD
 * RG_AUD_BG_EN         [12]    RW  0   Audio BG EN
 * RG_AUD_BIAS_EN       [11]    RW  0   Audio BIAS EN
 * RG_AUD_MICBIAS_EN    [10]    RW  0   Audio Microphone bias enable signal
 * RG_AUD_VCMI_SEL      [9]     RW  0   Audio Bandgap output voltage select signal
                                        0 = 1.6V output          1 = 1.5V output
 * RG_AUD_MICBIAS_SEL1P5[8]     RW  0   Audio MICBIAS reference voltage select signal
                                        0 = 1.6V reference          1 = 1.5V reference
 * RG_AUD_MICBIAS_PLGB  [7]     RW  0   Audio MICBIAS power down signal
                                        (do not control discharge circuit)
                                        0 = power down          1 = power up
 * reserved             [6: 0]  RO  0 */
#define VB_EN           (BIT_15)
#define VB_NLEAK_PD     (BIT_14)
#define VB_HDMC_SP_PD   (BIT_13)
#define BG_EN           (BIT_12)
#define BIAS_EN         (BIT_11)
#define MICBIAS_EN      (BIT_10)
#define VCMI_SEL        (BIT_9)
#define MICBIAS_SEL1P5  (BIT_8)
#define MICBIAS_PLGB    (BIT_7)

/// ANA_PMU1 0x04 ///

/* RG_AUD_VB_CAL        [15: 11] RW  0x10    Audio LDO_VB output voltage calibration signal
                                             00000 = -13.3%  00001 = -12.5% 00010 = -11.7%  00011 = -10.8%
                                             00100 = -10%    00101 = -9.2%  00110 = -8.4%   00111 = -7.5%
                                             01000 = -6.7%   01001 = -5.8%  01010 = -5%     01011 = -4.2%
                                             01100 = -3.3%   01101 = -2.5%  01110 = -1.67%  01111 = -0.83%
                                             10000 = 0       10001 = 0.83%  10010 = 1.67%   10011 = 2.5%
                                             10100 = 3.3%    10101 = 4.2%   10110 = 5%      10111 = 5.8%
                                             11000 = 6.7%    11001 = 7.5%   11010 = 8.4%    11011 = 9.2%
                                             11100 = 10%     11101 = 10.8%  11110 = 11.7%   11111 = 12.5%
 * RG_AUD_VB_V          [10: 6]  RW  0x10    Audio ADC/DAC/DRV VCM & LDO VB output voltage control bit
                                             (VB should be set larger than 3.0V)
                                             00000 -00011 = forbidden
                                             00100 = 3.0V        00101 = 3.025V        00110 = 3.05V        00111 = 3.075V
                                             01000 = 3.1V        01001 = 3.125V        01010 = 3.15V        01011 = 3.175V
                                             01100 = 3.2V        01101 = 3.225V        01110 = 3.25V        01111 = 3.275V
                                             10000 = 3.3V        10001 = 3.325V        10010 = 3.35V        10011 = 3.375V
                                             10100 = 3.4V        10101 = 3.425V        10110 = 3.45V        10111 = 3.475V
                                             11000 = 3.5V        11001 = 3.525V        11010 = 3.55V        11011 = 3.575V
                                             11100 = 3.6V        11101 - 11111 = forbidden
 * RG_AUD_PA_VCMAB_SEL  [5]      RW  0       Audio PA VCM select signal
                                             0 = self-generate,  1 = generate by bias current
 * RG_AUD_PA_VCM_V      [4: 3]   RW  0x1     Audio PA VCM voltage select signal
                                             00 = 0.55*Vbat,  01 = 0.5*Vbat, 10 = 0.45*Vbat,  11 = 0.4*Vbat
 * RG_AUD_MICBIAS_V     [2: 1]   RW  0x2     Audio MICBIAS output voltage select signal
                                             00 = 2.2V              01 = 2.6V
                                             10 = 2.8V              11 = 3.0V */
#define VB_CAL          (11)
#define VB_CAL_MASK     (MASK_5_BITS << VB_CAL)
#define VB_V            (6)
#define VB_V_MASK       (MASK_5_BITS << VB_V)
#define PA_VCMAB_SEL    (BIT_5)
#define VCM_V           (3)
#define VCM_V_MASK      (MASK_2_BITS << VCM_V)
#define MICBIAS_V       (1)
#define MICBIAS_V_MASK  (MASK_2_BITS << MICBIAS_V)

/// ANA_PMU2 0x08 ///

/* RG_AUD_HP_IB     [15: 14]    RW  0   AUD HP-PGA BIAS current:
                                        00:X1  11:X2
 * RG_AUD_HP_IBCUR3 [13: 11]    RW  0x2 AUD HP-PGA 3rd stage BIAS current:
                                        000: 5uA   001:7.5uA    010:10uA   011: 12.5uA
                                        100: 15uA  101: 17.5uA  110:20uA   111: 22.5uA
 * RG_AUD_PA_AB_I   [10: 9]     RW  0x1 Audio PA class-AB mode Quiescent current decreasing level
                                        00=3.5mA, 01=2.5mA, 10=1.9mA, 11=1.6mA
 * RG_AUD_ADPGA_I   [8: 5]      RW  0   Audio ADC & PGA ibias current control bit
                                        <3:2> control the ibias of the PGA
                                        00=10uA, 01=7.5uA, 10=5uA, 11=5uA
                                        <1:0> reversed
 * RG_AUD_DALR_LP_EN [4]        RW  0   Audio DAC low power mode enable signal
 * RG_AUD_DA_IJ      [3]        RW  0   Audio DACL & DACR output gain control bit
                                        0 = 0dB            1 = -1.5dB */
#define HP_IB           (14)
#define HP_IB_MASK      (MASK_2_BITS << HP_IB)
#define HP_IBCUR3       (11)
#define HP_IBCUR3_MASK  (MASK_3_BITS << HP_IBCUR3)
#define PA_AB_I         (9)
#define PA_AB_I_MASK    (MASK_2_BITS << PA_AB_I)
#define ADPGA_I         (7)
#define ADPGA_I_MASK    (MASK_2_BITS << ADPGA_I)
#define DALR_LP_EN      (BIT_4)
#define DA_IJ           (BIT_3)

/// ANA_PMU3 0x0C ///

/* RG_AUD_PA_OTP_PD    [15]     RW  0x1 Audio PA over temperature protection circuit power down signal
                                        0 = power up          1 = power down
 * RG_AUD_PA_OTP_T     [14: 12] RW  0x6 Audio PA over temperature protection circuit temperature select
                                        000: 25C -> 13C       001: 43C -> 31C
                                        010: 61C -> 49C       011: 79C -> 67C
                                        100: 96C -> 85C       101: 114C -> 102C
                                        110: 132C -> 120C     111: 150C -> 138C
 * RG_AUD_PA_STOP_EN   [11]     RW  0   Audio PA Driver stop output enable signal
 * RG_AUD_PA_SH_DET_EN [10]     RW  0   Audio PA output short to VBAT detect enable signal
 * RG_AUD_PA_SL_DET_EN [9]      RW  0   Audio PA output short to GND detect enable signal
 * RG_AUD_DRV_OCP_PD   [8: 5]   RW  0   Audio Driver over current protection power down signal
                                        0 = power up                1 = power down
                                        <3> reserved                <2> reserved
                                        <1> for Headphone/Earpiece  <0> for SPK/PA
 * RG_AUD_DRV_OCP_S    [4: 1]   RW  0   Audio Driver over current protection current select
                                        <3> ADC_VCM_SEL
                                        <2> for HP/RCV 0 = 200mA, 1 = 300mA
                                        <1> for HP/RCV 0 = 200mA, 1 = 300mA
                                        <0> for SPKL 0 = 600mA, 1 = 900mA */
#define PA_OTP_PD       (BIT_15)
#define PA_OTP_T        (12)
#define PA_OTP_T_MASK   (MASK_3_BITS << PA_OTP_T)
#define PA_STOP_EN      (BIT_11)
#define PA_SH_DET_EN    (BIT_10)
#define PA_SL_DET_EN    (BIT_9)
#define DRV_OCP_PD      (5)
#define DRV_OCP_PD_MASK (0xF<<DRV_OCP_PD)
#define DRV_OCP_S               //TODO:

/// ANA_CLK0 0x10 ///

/* RG_AUD_DIG_CLK_6P5M_EN  [13]    RW  0   Audio digital core clcok input enable signal
 * RG_AUD_DIG_CLK_LOOP_EN  [12]    RW  0   Audio digital loop clcok input enable signal
 * RG_AUD_ANA_CLK_EN       [11]    RW  0   Audio analog core clcok input enable signal
 * RG_AUD_AD_CLK_EN        [10]    RW  0   Audio analog ADC clock input enable signal
 * RG_AUD_AD_CLK_RST       [9]     RW  0   Audio analog ADC clock reset enable signal
 * RG_AUD_DA_CLK_EN        [8]     RW  0   Audio DAC clock input enable signal
 * RG_AUD_DRV_CLK_EN       [7]     RW  0   Audio DRV clock input enable signal
 * RG_AUD_AD_CLK_F         [6]     RW  0   Audio ADC clock frequency select (based on Fclk=6.5MHz)
                                           0 = Fclk            1 = Fclk / 2
 * RG_AUD_DA_CLK_F         [5: 4]  RW  0   Audio DAC clock frequency select (based on Fclk=6.5MHz)
                                           00 = Fclk              01/11 = Fclk x 2
                                           10 = Fclk / 2
 * RG_AUD_CLK_PN_SEL       [3: 0]  RW  0   Audio clock PN select (If RG_AUD_AD_CLK_F[1:0]=0 &
                                           RG_AUD_DA_CLK_F[1:0]=00, RG_AUD_CLK_PN_SEL)
                                           <0>CLK_AUD_DIG_LOOP        <1> CLK_AUD_DIG_6P5M
                                           <2>CLK_AUD_DA              <3> CLK_AUD_AD */
#define DIG_CLK_6P5M_EN (BIT_13)
#define DIG_CLK_LOOP_EN (BIT_12)
#define ANA_CLK_EN      (BIT_11)
#define AD_CLK_EN       (BIT_10)
#define AD_CLK_RST      (BIT_9)
#define DA_CLK_EN       (BIT_8)
#define DRV_CLK_EN      (BIT_7)
#define AD_CLK_F        (BIT_6)
#define DA_CLK_F        (4)
#define DA_CLK_F_MASK   (MASK_2_BITS << DA_CLK_F)
#define CLK_PN_SEL      //TODO

/// ANA_CDC0 0x14 ///

/* RG_AUD_ADPGA_IBIAS_EN   [15]    RW  0   Audio PGA&ADC BIAS en signal
 * RG_AUD_ADPGA_EN         [14]    RW  0   Audio ADC PGA enable signal
 * RG_AUD_ADPGA_BYP        [13:12] RW  0   Audio ADC PGA bypass select signal
                                           00 = normal input
                                           01 = HEADMIC to ADCL
                                           10/11 = All disconnected
 * RG_AUD_AD_EN            [11]    RW  0   Audio ADC enable signal
 * RG_AUD_AD_RST           [10]    RW  0   Audio ADC reset enable signal
 * RG_AUD_SHMIC_DPOPVCM_EN [9]     RW  0   Audio ADC option signal
 * RG_AUD_AD_NI_LPM        [8]     RW  0   Audio ADC option signal
 * RG_AUD_AD_REFSEL        [7:6]   RW  0   Audio ADC option signal */
#define ADPGA_IBIAS_EN   (BIT_15)
#define ADPGA_EN         (BIT_14)
#define ADPGA_BYP        (12)
#define ADPGA_BYP_MASK   (MASK_2_BITS << ADPGA_BYP)
#define AD_EN            (BIT_11)
#define AD_RST           (BIT_10)
#define SHMIC_DPOPVCM_EN (BIT_9)
#define AD_NI_LPM        (BIT_8)
#define AD_REFSEL        (6)    //TODO:????
#define AD_REFSEL_MASK   (MASK_2_BITS << AD_REFSEL)

/// ANA_CDC1 0x18 ///

/* RG_AUD_ADPGA_G   [8: 6]  RW  0x1 Audio ADC PGAR Gain control
                                    000 = 0dB       001 = 3dB
                                    010 = 6dB       011 = 12dB
                                    100 = 18dB      101 = 24dB
                                    110 = 30dB      111 = 36dB
 * RG_AUD_DALR_OS_D [5: 3]  RW  0   Audio DACL/R dc offset trim bit
                                    000 = 0         001 = +1/20*VFS
                                    010 = +2/20*VFS 011 = +1/20*VFS
                                    100 = 0         101 = -1/20*VFS
                                    110 = -2/20*VFS 111 = -1/20*VFS
 * RG_AUD_DAS_OS_D  [2: 0]  RW  0   Audio DACS dc offset trim bit
                                    000 = 0         001 = +1/20*VFS
                                    010 = +2/20*VFS 011 = +1/20*VFS
                                    100 = 0         101 = -1/20*VFS
                                    110 = -2/20*VFS 111 = -1/20*VFS */
#define ADPGA_G         (6)
#define ADPGA_G_MASK    (MASK_3_BITS << ADPGA_G)
#define DALR_OS_D       (3)
#define DALR_OS_D_MASK  (MASK_3_BITS << DALR_OS_D)
#define DAS_OS_D        (0)
#define DAS_OS_D_MASK   (MASK_3_BITS << DAS_OS_D)

/// ANA_CDC2 0x1C ///

/* RG_AUD_DAS_EN   [13] RW  0   Audio DACS enable signal
 * RG_AUD_DAL_EN   [12] RW  0   Audio DACL enable signal
 * RG_AUD_DAR_EN   [11] RW  0   Audio DACR enable signal
 * HPL_FLOOPEN     [10] RW  0   Audio Driver HPL dummy loop enable signal
 * HPL_FLOOP_END   [9]  RW  0   Audio Driver HPL dummy loop end enable signal
 * HPR_FLOOPEN     [8]  RW  0   Audio Driver HPR dummy loop enable signal
 * HPR_FLOOP_END   [7]  RW  0   Audio Driver HPR dummy loop end enable signal
 * RCV_FLOOPEN     [6]  RW  0   Audio Driver RCV dummy loop enable signal
 * RCV_FLOOP_END   [5]  RW  0   Audio Driver RCV dummy loop end enable signal
 * RG_AUD_HPL_EN   [4]  RW  0   Audio Driver HPL output enable signal
 * RG_AUD_HPR_EN   [3]  RW  0   Audio Driver HPR output enable signal
 * RG_AUD_HPBUF_EN [2]  RW  0   Audio Driver vcm buffer enable signal
 * RG_AUD_RCV_EN   [1]  RW  0   Audio Receiver/Earpiece Driver
                                RCV_P/RCV_N output enable signal (RG_AUD_CP_EN[1:0]=1)
 * RG_AUD_PA_EN    [0]  RW  0   Audio Speaker PA (Driver SPKL) enable signal */
#define DAS_EN          (BIT_13)
#define DAL_EN          (BIT_12)
#define DAR_EN          (BIT_11)
#define HPL_FLOOPEN     (BIT_10)
#define HPL_FLOOP_END   (BIT_9)
#define HPR_FLOOPEN     (BIT_8)
#define HPR_FLOOP_END   (BIT_7)
#define RCV_FLOOPEN     (BIT_6)
#define RCV_FLOOP_END   (BIT_5)
#define HPL_EN          (BIT_4)
#define HPR_EN          (BIT_3)
#define HPBUF_EN        (BIT_2)
#define RCV_EN          (BIT_1)
#define PA_EN           (BIT_0)

/// ANA_CDC3 0x20 ///

/* RG_AUD_DALR_OS_EN    [12] RW  0   Audio DACL/R dc offset enable signal
 * RG_AUD_DAS_OS_EN     [11] RW  0   Audio DACS dc offset enable signal
 * RG_AUD_NG_PA_EN      [10] RW  0   NG_PA enable control
 * RG_AUD_SDALHPL       [9]  RW  0   Audio DACL to HPL enable signal
 * RG_AUD_SDARHPR       [8]  RW  0   Audio DACR to HPR enable signal
 * RG_AUD_SDALRCV       [7]  RW  0   Audio DACL to Receiver/Earpiece enable signal
 * RG_AUD_SHMIC_DPOP    [6]  RW  0   Headmic button release depop signal
 * RG_AUD_SDAPA         [5]  RW  0   Audio DACS to PA enable signal
 * RG_AUD_SHMICPA_DEBUG [4]  RW  0   Audio HMIC to PA enable signal
                                     0 = disable   1 = enable
                                     when debug=1, HMIC to PA path on, no matter "RG_AUD_SDAPA"
                                     when debug=0, HMIC to PA path off, "RG_AUD_SDAPA" is enable
 * RG_AUD_SMICDRV_DEBUG [3]  RW  0   Audio MIC to LDRV enable signal
                                     0 = disable   1 = enable
                                     when debug=1, MIC to LDRV path on,
                                     "RG_AUD_SDALHPL"/"RG_AUD_SDALRCV" is dis-enable
                                     when debug=0, MIC to LDRV path off,
                                     "RG_AUD_SDALHPL"/"RG_AUD_SDALRCV" is enable
 * RG_AUD_RCV_PATH_OPT  [2]  RW  0   Audio DAC output to RCV
                                     0: DACL2RCV    1: DACL+DACR-RCV
 * RG_AUD_SMICPGA       [1]  RW  0   MIC to Audio ADC PGAL enable signal
 * RG_AUD_SHMICPGA      [0]  RW  0   HEADMIC to Audio ADC PGAL enable signal */
#define DALR_OS_EN      (BIT_12)
#define DAS_OS_EN       (BIT_11)
#define NG_PA_EN        (BIT_10)
#define SDALHPL         (BIT_9)
#define SDARHPR         (BIT_8)
#define SDALRCV         (BIT_7)
#define SHMIC_DPOP      (BIT_6)
#define SDAPA           (BIT_5)
#define SHMICPA_DEBUG   (BIT_4)
#define SMICDRV_DEBUG   (BIT_3)
#define RCV_PATH_OPT    (BIT_2)
#define SMICPGA         (BIT_1)
#define SHMICPGA        (BIT_0)

/// ANA_CDC4 0x24 ///

/* RG_AUD_SPK_G [13:12] RW  0x1 Audio Speaker Driver PGA Gain control
                                00 = -3dB       01 = 0dB
                                10 =  3dB       11 = 6dB
 * RG_AUD_RCV_G [11:8]  RW  0x2 Audio Receiver/Earpiece Driver RCV_P/RCV_N PGA Gain control
                                0010 =  6dB     0011 =  3dB
                                0100 =  0dB     0101 = -3dB
                                0110 = -6dB     0111 = -9dB
                                1000 = -12dB    1001 = -18dB
                                1010 = -18dB    1111 = mute
 * RG_AUD_HPL_G [7:4]   RW  0x4 Audio Headphone left channel Gain control
                                0010 =  6dB     0011 =  3dB
                                0100 =  0dB     0101 = -3dB
                                0110 = -6dB     0111 = -9dB
                                1000 = -12dB    1001 = -18dB
                                1010 = -18dB    1111 = mute
 * RG_AUD_HPR_G [3:0]   RW  0x4 Audio Headphone right channel Gain control
                                0010 =   6dB    0011 =   3dB
                                0100 =  0dB     0101 =  -3dB
                                0110 = -6dB     0111 = -9dB
                                1000 = -12dB    1001 = -18dB
                                1010 = -18dB    1111 = mute */
#define SPK_G           (12)
#define SPK_G_MASK      (MASK_2_BITS << SPK_G)
#define RCV_G           (8)
#define RCV_G_MASK      (MASK_4_BITS << RCV_G)
#define HPL_G           (4)
#define HPL_G_MASK      (MASK_4_BITS << HPL_G)
#define HPR_G           (0)
#define HPR_G_MASK      (MASK_4_BITS << HPR_G)

/// ANA_HDT0 0x28 ///

/* RG_AUD_V2AD_EN           [11]  RW 0 Audio signal input to AuxADC enable signal
 * RG_AUD_AUXADC_SCALE_SEL  [10]  RW 0 HEADDET to AuxADC Buffer enable:
                                       "0" is ADC litle scale(ADC_SCALE=1.2V);
                                       "1" is ADC large scale(ADC_SCALE=3.6V);
 * RG_AUD_V2AD_SEL          [9:7] RW 0 Audio signal input to AuxADC select
                                       000 =HEADMIC_IN_DET  001 =VDDVB;
                                       010 =  Micbias;      011~111 =forbidden
 * RG_AUD_BUT_DET_PD        [6]   RW 1 Audio headset button detect circuit power down signal
                                       0 = power up          1 = power down
 * RG_AUD_HEAD_SDET         [5:4] RW 0 Audio headset insert HEADMIC_IN_DET detect
                                       voltage select signal (HEADMIC_BIAS=2.8V)
                                       00 = 2.7V              01 = 2.5V
                                       10 = 2.3V              11 = 2.1V
 * RG_AUD_HEAD_SBUT         [3:0] RW 0 Audio headset button pressed detect voltage select signal
                                       (HEADMIC_BIAS=2.8V)
                                       0000 = 1.0V         0001 = 0.95V
                                       0010 = 0.9V         0011 = 0.85V
                                       0100 = 0.8V         0101 = 0.75V
                                       0110 = 0.7V         0111 = 0.65V
                                       1000 = 0.6V         1001 = 0.55V
                                       1010 = 0.5V         1011 = 0.45V
                                       1100 = 0.4V         1101/1110/1111 = forbidden */
#define V2AD_EN             (BIT_11)
#define AUXADC_SCALE_SEL    (BIT_10)
#define V2AD_SEL            (7)
#define V2AD_SEL_MASK       (MASK_3_BITS << V2AD_SEL)
#define BUT_DET_PD          (BIT_6)
#define HEAD_SDET           (4)
#define HEAD_SDET_MASK      (MASK_2_BITS << HEAD_SDET)
#define HEAD_SBUT           (0)
#define HEAD_SBUT_MASK      (MASK_4_BITS << HEAD_SBUT)

/// ANA_DCL0 0x2C ///

/* RG_AUD_DCL_EN        [5]    RW  0   Audio digital control logic enable signal
 * RG_AUD_DCL_RST       [4]    RW  0x1 Audio digital control logic reset enable signal
 * RG_AUD_DRV_SOFT_T    [3:1]  RW  0x1 Audio DRV delay timer control signal
                                       000 = 0us          001 = 30us
                                       010 = 60us         011 = 90us
                                       100 = 120us        101 = 150us
                                       110 = 180us        111 = 210us
 * RG_AUD_DRV_SOFT_EN   [0]    RW  0   Audio DRV soft start enable signal */
#define DCL_EN          (BIT_5)
#define DCL_RST         (BIT_4)
#define DRV_SOFT_T      (1)
#define DRV_SOFT_T_MASK (MASK_3_BITS << DRV_SOFT_T)
#define DRV_SOFT_EN     (BIT_0)

/// ANA_DCL2 0x30 ///

/* RG_AUD_PA_OTP_DEG_EN  [12]   RW 0  Audio PA over temperature protection circuit
                                      alert deglitch enable signal
 * RG_AUD_PA_OTP_DEG_T   [11:9] RW 0  Audio PA over temperature protection circuit
                                      alert deglitch timer control signal
                                      000 = 0ms             001 = 0.06ms
                                      010 = 0.24ms          011 = 1ms
                                      100 = 4ms             101 = 16ms
                                      110 = 64ms            111 = 256ms
 * RG_AUD_PA_OTP_MUTE_EN [8]    RW 0  Audio PA over temperature protection circuit
                                      mute enable signal
 * RG_AUD_PA_OCP_DEG_EN  [7]    RW 0  Audio PA over current protection circuit
                                      alert deglitch enable signal
 * RG_AUD_PA_OCP_DEG_T   [6:4]  RW 0  Audio PA over current protection circuit
                                      alert deglitch timer control signal
                                      000 = 0ms             001 = 0.06ms
                                      010 = 0.24ms          011 = 1ms
                                      100 = 4ms             101 = 16ms
                                      110 = 64ms            111 = 256ms
 * RG_AUD_PA_OCP_MUTE_EN [3]    RW 0  Audio PA over current protection circuit mute power down signal
                                      0 = enable mute        1 = disable mute
 * RG_AUD_PA_OCP_MUTE_T  [2:0]  RW 0  Audio PA over current protection circuit mute timer control signal
                                      000 = 0ms             001 = 1ms
                                      010 = 4ms             011 = 16ms
                                      100 = 64ms            101 = 256ms
                                      110 = 1s              111 = 4s */
#define PA_OTP_DEG_EN       (BIT_12)
#define PA_OTP_DEG_T        (9)
#define PA_OTP_DEG_T_MASK   (MASK_3_BITS << PA_OTP_DEG_T)
#define PA_OTP_MUTE_EN      (BIT_8)
#define PA_OCP_DEG_EN       (BIT_8)
#define PA_OCP_DEG_T        (4)
#define PA_OCP_DEG_T_MASK   (MASK_3_BITS << PA_OCP_DEG_T)
#define PA_OCP_MUTE_EN      (BIT_8)
#define PA_OCP_MUTE_T       (0)
#define PA_OCP_MUTE_T_MASK  (MASK_3_BITS << PA_OCP_MUTE_T)

/// ANA_DCL9 0x34 ///

/* RG_HPL_DEPOP_CHG_CURSEL [15:8] RW 0xff HPL depop DAC current setting
 * RG_HPR_DEPOP_CHG_CURSEL [7: 0] RW 0xff HPR depop DAC current setting */
#define HPL_DEPOP_CHG_CURSEL        (8)
#define HPL_DEPOP_CHG_CURSEL_MASK   (MASK_8_BITS << HPL_DEPOP_CHG_CURSEL)
#define HPR_DEPOP_CHG_CURSER        (0)
#define HPR_DEPOP_CHG_CURSER_MASK   (MASK_8_BITS << HPR_DEPOP_CHG_CURSER)

/// ANA_DCL10 0x38 ///

/// ANA_DCL11 0x3C ///

/* RG_CALDC_WAIT_T         [14:12] RW 0 Audio dc-calibration waiting time, every data change
                                        000 = 2Tclk    001 = 3Tclk    010 = 4Tclk    011 = 5Tclk
                                        100 = 6Tclk    101 = 7Tclk    110 = 8Tclk    111 = 9Tclk
 * RG_AUD_HPL_DPOP_CLKN1   [11:10] RW 0 Audio DePOP HPL DAC clock(start-up)
                                        00 = 1Tclk 01 = 2Tclk 10 = 4Tclk 11 = 8Tclk
 * RG_AUD_HPL_DPOP_N1      [9:8]   RW 0 Audio DePOP HPL DAC data increase step(start-up)
                                        00 = +1    01 = +2    10 = +4    11 = +8
 * RG_AUD_HPL_DPOP_VAL1    [7:5]   RW 6 Audio DePOP HPL DAC data final value(start-up)
                                        000 = 2   001 = 4   010 = 8   011 = 16
                                        100 = 32  101 = 64  110 = 128 111 = 256
 * RG_AUD_HPL_DPOP_CLKN2   [4:3]   RW 0 Audio DePOP HPL DAC clock(rising/falling)
                                        00 = 1Tclk 01 = 2Tclk 10 = 4Tclk 11 = 8Tclk
 * RG_AUD_HPL_DPOP_N2      [2:1]   RW 0 Audio DePOP HPL DAC data increase step(rising/falling)
                                        00 = +1    01 = +2    10 = +4    11 = +8
 * RG_AUD_DPOP_AUTO_RST    [0]     RW 1 Soft reset dpop module . 0:disable , 1:enable */
#define CALDC_WAIT_T        (12)
#define CALDC_WAIT_T_MASK   (MASK_3_BITS << CALDC_WAIT_T)
#define HPL_DPOP_CLKN1      (10)
#define HPL_DPOP_CLKN1_MASK (MASK_2_BITS << HPL_DPOP_CLKN1)
#define HPL_DPOP_N1         (8)
#define HPL_DPOP_N1_MASK    (MASK_2_BITS << HPL_DPOP_N1)
#define HPL_DPOP_VAL1       (5)
#define HPL_DPOP_VAL1_MASK  (MASK_3_BITS << HPL_DPOP_VAL1)
#define HPL_DPOP_CLKN2      (3)
#define HPL_DPOP_CLKN2_MASK (MASK_2_BITS << HPL_DPOP_CLKN2)
#define HPL_DPOP_N2         (1)
#define HPL_DPOP_N2_MASK    (MASK_2_BITS << HPL_DPOP_N2)
#define DPOP_AUTO_RST       (BIT_0)

/// ANA_DCL12 0x40 ///

/* DEPOPL_PCUR_OPT         [14:13] RW 1 depop_hpl_current_sel
                                        00: X2   01:X1   10:X2/3  11:X1/2
 * DEPOPR_PCUR_OPT         [12:11] RW 1 depop_hpr_current_sel
                                        00: X2   01:X1   10:X2/3  11:X1/2
 * RG_AUD_HPR_DPOP_CLKN1   [10:9]  RW 0 Audio DePOP HPR DAC clock(start-up)
                                        00 = 1Tclk    01 = 2Tclk    10 = 4Tclk    11 = 8Tclk
 * RG_AUD_HPR_DPOP_N1      [8:7]   RW 0 Audio DePOP HPR DAC data increase step(start-up)
                                        00 = +1    01 = +2    10 = +4    11 = +8
 * RG_AUD_HPR_DPOP_VAL1    [6:4]   RW 6 Audio DePOP HPR DAC data final value(start-up)
                                        000 = 2   001 = 4   010 = 8   011 = 16
                                        100 = 32  101 = 64  110 = 128 111 = 256
 * RG_AUD_HPR_DPOP_CLKN2   [3:2]   RW 0 Audio DePOP HPR DAC clock(rising/falling)
                                        00 = 1Tclk    01 = 2Tclk    10 = 4Tclk    11 = 8Tclk
 * RG_AUD_HPR_DPOP_N2      [1:0]   RW 0 Audio DePOP HPR DAC data increase step(rising/falling)
                                        00 = +1    01 = +2    10 = +4    11 = +8 */
#define DEPOPL_PCUR_OPT         (13)
#define DEPOPL_PCUR_OPT_MASK    (MASK_2_BITS << DEPOPL_PCUR_OPT)
#define DEPOPR_PCUR_OPT         (11)
#define DEPOPR_PCUR_OPT_MASK    (MASK_2_BITS << DEPOPR_PCUR_OPT)

#define HPR_DPOP_CLKN1      (9)
#define HPR_DPOP_CLKN1_MASK (MASK_2_BITS << HPR_DPOP_CLKN1)
#define HPR_DPOP_N1         (7)
#define HPR_DPOP_N1_MASK    (MASK_2_BITS << HPR_DPOP_N1)
#define HPR_DPOP_VAL1       (4)
#define HPR_DPOP_VAL1_MASK  (MASK_3_BITS << HPR_DPOP_VAL1)
#define HPR_DPOP_CLKN2      (2)
#define HPR_DPOP_CLKN2_MASK (MASK_2_BITS << HPR_DPOP_CLKN2)
#define HPR_DPOP_N2         (0)
#define HPR_DPOP_N2_MASK    (MASK_2_BITS << HPR_DPOP_N2)

/// ANA_STS0 0x44 ///

/* RG_HP_DEPOP_WAIT_T1 [15:13] RW 0x4 depop_runing time
                                      000: 10ms   001: 20ms   010: 40ms  011: 80ms
                                      100: 160ms  101: 320ms  110: 640ms 111: 1280ms
 * RG_HP_DEPOP_WAIT_T2 [12:10] RW 0x4 depop_finish waiting time
                                      000: 10ms   001: 20ms   010: 40ms
                                      011: 80ms   100: 160ms  101: 320ms
                                      110: 640ms  111: 1280ms
 * RG_HP_DEPOP_WAIT_T3 [9:8]   RW 0x2 CHG_EN_Delay time
                                      00:  1Tclk  01: 2Tclk  10: 4Tclk  11: 8Tclk
 * RG_HP_DEPOP_WAIT_T4 [7:6]   RW 0x2 depop path on delay time
                                      00:  1Tclk   01: 2Tclk  10: 4Tclk  11: 8Tclk
 * RG_DC_CALI_IDACVAL  [5:3]   RW 0x4 DCCALI_IDAC_repeat_goal
                                      000: 8  001:9   010:10  011:11
                                      100:12  101:13  110:14  111:7
 * DC_CALI_IDAC_CURSEL [2:1]   RW 0   IDAC LSB SETTING:
                                      00: 10nA  01:15nA  10:5nA  11:10nA
 * RG_DCCALI_RDACI_ADJ [0]     RW 0   RDAC current enhancement
                                      0 = X1       1 = X2 */
#define HP_DEPOP_WAIT_T1        (13)
#define HP_DEPOP_WAIT_T1_MASK   (MASK_3_BITS << HP_DEPOP_WAIT_T1)
#define HP_DEPOP_WAIT_T2        (10)
#define HP_DEPOP_WAIT_T2_MASK   (MASK_3_BITS << HP_DEPOP_WAIT_T2)
#define HP_DEPOP_WAIT_T3        (8)
#define HP_DEPOP_WAIT_T3_MASK   (MASK_2_BITS << HP_DEPOP_WAIT_T3)
#define HP_DEPOP_WAIT_T4        (6)
#define HP_DEPOP_WAIT_T4_MASK   (MASK_2_BITS << HP_DEPOP_WAIT_T4)
#define DC_CALI_IDACVAL         (3)
#define DC_CALI_IDACVAL_MASK    (MASK_3_BITS << DC_CALI_IDACVAL)
#define DC_CALI_IDAC_CURSEL     (1)
#define DC_CALI_IDAC_CURSEL_MASK (MASK_2_BITS << DC_CALI_IDAC_CURSEL)
#define DCCALI_RDACI_ADJ        (BIT_0)

/// ANA_STS1 0x48 ///

/* RG_AUD_HEAD_INSERT_2 [9] RO  0   0.0
 * RG_AUD_HEAD_BUTTON   [8] RO  0   Audio headset microphone button press alert signal
                                    (need software anti-dither)
                                    0 = normal             1 = button press
 * RG_AUD_HEAD_INSERT   [7] RO  0   Audio headset microphone insert alert signal
                                    (need software anti-dither)
                                    0 = normal             1 = plug in
 * RG_AUD_PA_SH_FLAG    [6] RO  0   Audio PA output short to VBAT detect ALERT signal
                                    0 = normal          1 = short
 * RG_AUD_PA_SL_FLAG    [5] RO  0   Audio PA output short to GND detect ALERT signal
                                    0 = normal          1 = short
 * RG_AUD_PA_OTP_FLAG   [4] RO  0   Audio PA over temperature protection circuit alert signal
                                    0 = normal             1 = over temperature
 * RG_AUD_DRV_OCP_FLAG  [3:0] RO 0  Audio Driver over current protection circuit alert signal
                                    <3:2> for SPK      <1:0> for Headphone/Earpiece */

/// ANA_STS2 0x4C ///

/* HPL_DCCAL_RDACL      [15:8] RO  0   HPL_DCCALI_RDAC_VALUE
 * HPR_DCCAL_RDACL      [7: 4] RO  0   HPR_DCCALI_RDAC_VALUE
 * HPL_DCCAL_IDACL_SEL  [3: 2] RO  0   HPL_DCCALI_IDAC_path
 * HPR_DCCAL_IDACL_SEL  [1: 0] RO  0   HPR_DCCALI_IDAC_path */
#define HPL_DCCAL_RDACL				(8)
#define HPL_DCCAL_RDACL_MASK		(0xFF<<HPL_DCCAL_RDACL)
#define HPR_DCCAL_RDACL				(4)
#define HPR_DCCAL_RDACL_MASK		(0xF<<HPR_DCCAL_RDACL)
#define HPL_DCCAL_IDACL_SEL			(2)
#define HPL_DCCAL_IDACL_SEL_MASK	(0x3<<HPL_DCCAL_IDACL_SEL)
#define HPR_DCCAL_IDACL_SEL			(0)
#define HPR_DCCAL_IDACL_SEL_MASK	(0x3<<HPR_DCCAL_IDACL_SEL)


/// ANA_STS3 0x50 ///

/* HPL_DCCAL_IDACL [15: 11] RO 0   HPL_DCCALI_IDAC_VALUE
 * HPR_DCCAL_IDACL [10: 6]  RO 0   HPR_DCCALI_IDAC_VALUE
 * RG_AUD_HWSW_SEL [5: 0]   RW 0   Hardware control/software control sel
                                   <2>: 0: RG_AUD_VCMI_SEL change, repeat dccali
                                        1: change RG_AUD_VCMI_SEL, get two dccali value,
                                        choose any according RG_AUD_VCMI_SEL =0/=1
                                   <1>: 0: hw control  DC_CALI_IDAC_CURSEL,
                                        1: sw control DC_CALI_IDAC_CURSEL
                                   <0>: 0: hw control  RG_HPL_PU_ENB, RG_HPR_PU_ENB, RG_INSBUF_EN
                                        1: sw control  RG_HPL_PU_ENB, RG_HPR_PU_ENB, RG_INSBUF_EN */
#define AUD_HWSW_SEL        (0)
#define AUD_HWSW_SEL_MASK   (0x3F << AUD_HWSW_SEL)
#define AUD_HWSW_SEL_0      (0)


/// ANA_STS4 0x54 ///

/* RG_CALDC_START       [15]    RW  0   DC-calibraion start signal
                                        0 ---> 1 start calibration
 * RG_CALDC_EN          [14]    RW  0   DC-calibraion enable signal (digital)
 * RG_CALDC_ENO         [13]    RW  0   DC-calibraion enable signal (analog)
 * RG_AUD_DCCAL_STS     [12]    RO  0   Audio DC-calibration status signal
                                        0 = unfinish/have never done       1 = finish
 * RG_DCCALI_STS_BYPASS [11]    RW  0   DCCALI_STS_BYPASS=0, not bypass DCCALI_process
                                        DCCALI_STS_BYPASS=1, bypass DCCALI_process
 * RG_AUD_HP_DPOP_DVLD  [10]    RO  0   Audio DC-calibration finish insert signal
                                        0 = unfinish        1 = finish
 * RG_DEPOP_CHG_START   [9]     RW  0   depop start signal
                                        0 ---> 1 start calibration
 * RG_DEPOP_CHG_EN      [8]     RW  0   depop charge en
 * RG_AUD_PLUGIN        [7]     RW  0   plug_in=1, headphone has been inserted
 * RG_AUD_PLUGOUT       [6]     RW  0   no use
 * RG_DEPOP_EN          [5]     RW  0   depop_ana_en
 * RG_DEPOP_CHG_STS     [4]     RO  0   Audio plug-in depop status signal
                                        0 = depop not finish        1 = depop finish
 * RG_AUD_RCV_DPOP_DVLD [3]     RO  0   Audio plug-in depop charge finish insert signal
                                        0 = unfinish        1 = finish
 * RG_HPL_PU_ENB        [2]     RW  0   HPL_pull_up enable
                                        0: pull up enable   1: pull up disable
 * RG_HPR_PU_ENB        [1]     RW  0   HPR_pull_up enable
                                        0: pull up enable   1: pull up disable
 * RG_INSBUF_EN         [0]     RW  0   INSBUF_EN */
#define CALDC_START         (BIT_15)
#define CALDC_EN            (BIT_14)
#define CALDC_ENO           (BIT_13)
#define AUD_DCCAL_STS       (BIT_12) //RO
#define DCCALI_STS_BYPASS   (BIT_11)
#define AUD_DCCAL_INSERT    (BIT_10) //RO
#define DEPOP_CHG_START     (BIT_9)
#define DEPOP_CHG_EN        (BIT_8)
#define AUD_PLUGIN          (BIT_7)
#define AUD_PLUGOUT         (BIT_6)
#define DEPOP_EN            (BIT_5)
#define DEPOP_CHG_STS       (BIT_4) //RO
#define DEPOP_CHG_INSERT    (BIT_3) //RO
#define HPL_PU_ENB          (BIT_2)
#define HPR_PU_ENB          (BIT_1)
#define INSBUF_EN           (BIT_0)


/**---------------------------------------------------------------------------*
 **                         register mapping definition                       *
 **---------------------------------------------------------------------------*/

#define SC1161_AP_REG_BASE  (0x82001380)

typedef struct
{
    uint32  ana_pmu0;           //0x00
    uint32  ana_pmu1;           //0x04
    uint32  ana_pmu2;           //0x08
    uint32  ana_pmu3;           //0x0c

    uint32  ana_clk0;           //0x10

    uint32  ana_cdc0;           //0x14
    uint32  ana_cdc1;           //0x18
    uint32  ana_cdc2;           //0x1c
    uint32  ana_cdc3;           //0x20
    uint32  ana_cdc4;           //0x24

    uint32  ana_hdt0;           //0x28
    uint32  ana_dcl0;           //0x2c
    uint32  ana_dcl2;           //0x30
    uint32  ana_dcl9;           //0x34
    uint32  ana_dcl10;          //0x38
    uint32  ana_dcl11;          //0x3c
    uint32  ana_dcl12;          //0x40

    uint32  ana_sts0;           //0x44
    uint32  ana_sts1;           //0x48
    uint32  ana_sts2;           //0x4C
    uint32  ana_sts3;           //0x50
    uint32  ana_sts4;           //0x54
} SPRD_CODEC_AP_REG_CTL_T;



/**---------------------------------------------------------------------------*
 **                       A Die codec digital part                            *
 **                         Register description                              *
 **---------------------------------------------------------------------------*/

/// AUD_CFGA_CLR 0x00 ///
/// aud_cfgal clr
/* ear_shutdown_clr     [9]     WC	0	ear shut down clear
 * hp_shutdown_clr      [8]     WC	0	hp shut down clear
 * pa_shutdown_clr      [7]     WC	0	pa shut down clear
 * aud_int_clr	        [6:0]	WC	0	[6:0]={audio_pacal_irq,audio_hp_dpop_irq,ovp_irq,otp_irq,pa_ocp_irq,ear_ocp_irq,hp_ocp_irq} */
#define EAR_SHUTDOWN_CLR                (BIT_9)
#define HP_SHUTDOWN_CLR                 (BIT_8)
#define PA_SHUTDOWN_CLR                 (BIT_7)
#define AUD_INT_CLR_MASK			    (MASK_7_BITS)
#define AUD_INT_CLR_ALL                 (AUD_INT_CLR_MASK)
#define AUD_INT_CLR_AUDIO_PACAL_IRQ     (BIT_6) //TODO: maybe BIT_0
#define AUD_INT_CLR_AUDIO_HP_DPOP_IRQ   (BIT_5)
#define AUD_INT_CLR_OVP_IRQ             (BIT_4)
#define AUD_INT_CLR_OTP_IRQ             (BIT_3)
#define AUD_INT_CLR_PA_OCP_IRQ          (BIT_2)
#define AUD_INT_CLR_EAR_OCP_IRQ         (BIT_1)
#define AUD_INT_CLR_HP_OCP_IRQ          (BIT_0)


/// AUD_CFGA_HID_CFG0 0x04 ///
/// audio head insert detect status
/* UD_DBNC_EN           [2: 1]  RW  0   [0]: audio HEAD_INSERT debounce enable [1]: audio HEAD_BUTTON_OUT debounce enable
 * HID_EN               [0]     RW  0 */
#define AUD_HEAD_BUTTON_OUT_DEBOUNCE_EN   (BIT_2)
#define AUD_HEAD_INSERT_DEBOUNCE_EN       (BIT_1)
#define HID_EN                            (BIT_0)

// TODO: low or high ??
/// AUD_CFGA_HID_CFG1 0x08 ///
/// audio head insert detect status
/* HID_HIGH_DBNC_THD0	[15: 8]	RW	0	low debounce threshold(clock 1k), for HEAD_INSERT signal
 * HID_LOW_DBNC_THD0	[7: 0]	RW	0	high debounce threshold(clock 1k), for HEAD_INSERT signal */
#define HID_HIGH_DBNC_THD0       (8)
#define HID_HIGH_DBNC_THD0_MASK  (MASK_8_BITS << HID_HIGH_DBNC_THD0)
#define HID_LOW_DBNC_THD0        (0)
#define HID_LOW_DBNC_THD0_MASK   (MASK_8_BITS << HID_LOW_DBNC_THD0)

/// AUD_CFGA_HID_CFG2 0x0c ///
/// audio head insert detect status
/* HID_TMR_T1T2_STEP	[9: 5]	RW	0	head insert detect T1/T2 timer step (clock 1k)
 * HID_TMR_T0	        [4: 0]	RW	0	head insert detect T0 timer , (clock 1k) */
#define HID_TMR_T1T2_STEP       (5)
#define HID_TMR_T1T2_STEP_MASK  (MASK_5_BITS << HID_TMR_T1T2_STEP)
#define HID_TMR_T0              (0)
#define HID_TMR_T0_MASK         (MASK_5_BITS << HID_TMR_T0)

/// AUD_CFGA_HID_CFG3 0x10 ///
/// audio head insert detect status
/* HID_TMR_T1           [15: 0]	RW	0	head insert detect T1 timer ,step: HID_TMR_T1T2_STEP */
#define HID_TMR_T1          (0)
#define HID_TMR_T1_MASK     (MASK_16_BITS << HID_TMR_T1)

/// AUD_CFGA_HID_CFG4 0x14 ///
/// audio head insert detect status
/* HID_TMR_T2           [15: 0]	RW	0	head insert detect T2 timer ,step: HID_TMR_T1T2_STEP */
#define HID_TMR_T2          (0)
#define HID_TMR_T2_MASK     (MASK_16_BITS << HID_TMR_T2)

/// AUD_CFGA_HID_CFG5 0x18 ///
/// audio head insert detect status
/* HID_HIGH_DBNC_THD1	[15: 8]	RW	0	low debounce threshold(clock 1k), for HEAD_BUTTON_OUT signal
 * HID_LOW_DBNC_THD1	[7: 0]	RW	0	high debounce threshold(clock 1k), for HEAD_BUTTON_OUT signal */
#define HID_HIGH_DBNC_THD1              (8)
#define HID_HIGH_DBNC_THD1_MASK         (MASK_8_BITS << HID_HIGH_DBNC_THD1)
#define HID_LOW_DBNC_THD1               (0)
#define HID_LOW_DBNC_THD1_MASK          (MASK_8_BITS << HID_LOW_DBNC_THD1)

/// AUD_CFGA_HID_STS0 0x1c ///
/// audio head insert detect status
/* EAR_SHUTDOWN	            [10]	RO	0	ear_shutdown
 * HP_SHUTDOWN	            [9]	    RO	0	hp_shutdown
 * PA_SHUTDOWN	            [8]	    RO	0	pa_shutdown
 * AUDIO_HEAD_INSERT_OUT	[7]	    RO	0	head insert detect out: AUDIO_HEAD_INSERT_OUT state
 * AUDIO_HEAD_BUTTON_OUT	[6]	    RO	0	head button detect out : AUDIO_HEAD_BUTTON_OUT state
 * AUD_DBNC_STS1	        [5: 3]	RO	0	u1 debounce state machine  status
 * AUD_DBNC_STS0	        [2: 0]	RO	0	u0 debounce state machine  status */
#define EAR_SHUTDOWN            (BIT_10)
#define HP_SHUTDOWN             (BIT_9)
#define PA_SHUTDOWN             (BIT_8)
#define AUDIO_HEAD_INSERT_OUT   (BIT_7)
#define AUDIO_HEAD_BUTTON_OUT   (BIT_6)
#define AUD_DBNC_STS1           (3)
#define AUD_DBNC_STS1_MASK      (MASK_3_BITS << AUD_DBNC_STS1)
#define AUD_DBNC_STS0           (0)
#define AUD_DBNC_STS0_MASK      (MASK_3_BITS << AUD_DBNC_STS0)


/// AUD_CFGA_PRT_CFG_0 0x20 ///
/// aud protect module configuration
/* EAR_SHUTDOWN_EN      [9]     RW	0	ear_shutdown_enable
 * HP_SHUTDOWN_EN       [8]     RW	0	hp_shutdown_enable
 * PA_SHUTDOWN_EN2      [7]     RW	0	pa_shutdown_enable2
 * PA_SHUTDOWN_EN1      [6]     RW	0	pa_shutdown_enable1
 * PA_SHUTDOWN_EN0	    [5]     RW	0	pa_shutdown_enable0
 * AUD_CLK_SEL	        [4]     RW	0x1	1:32k_clk 0:1k_clk
 * AUD_PRT_EN	        [3]     RW	0	protect enable
 * OTP_PD_THD	        [2:0]	RW	0x3	over-temperature protection threshold */
#define EAR_SHUTDOWN_EN	    (BIT_9)
#define HP_SHUTDOWN_EN	    (BIT_8)
#define PA_SHUTDOWN_EN2	    (BIT_7)
#define PA_SHUTDOWN_EN1	    (BIT_6)
#define PA_SHUTDOWN_EN0	    (BIT_5)
#define AUD_CLK_SEL	        (BIT_4)
#define AUD_PRT_EN	        (BIT_3)
#define OTP_PD_THD	        (0)
#define OTP_PD_THD_MASK	    (MASK_3_BITS << OTP_PD_THD)

/// AUD_CFGA_PRT_CFG_1 0x24 ///
/// aud protect module configuration
/* OTP_PRECIS	[14: 12]	RW	0x2	over-temperature protection precis
 * OVP_PD_THD	[11: 9]     RW	0x2	overvoltage protection threshold
 * OVP_PRECIS	[8: 6]      RW	0	overvoltage protection precis
 * OCP_PD_THD	[5: 3]      RW	0x3	overcurrent protection threshold
 * OCP_PRECIS	[2: 0]      RW	0x2	overcurrent protection precis */
#define OTP_PRECIS          (12)
#define OTP_PRECIS_MASK     (MASK_3_BITS << OTP_PRECIS)
#define OVP_PD_THD          (9)
#define OVP_PD_THD_MASK     (MASK_3_BITS << OVP_PD_THD)
#define OVP_PRECIS          (6)
#define OVP_PRECIS_MASK     (MASK_3_BITS << OVP_PRECIS)
#define OCP_PD_THD          (3)
#define OCP_PD_THD_MASK     (MASK_3_BITS << OCP_PD_THD)
#define OCP_PRECIS          (0)
#define OCP_PRECIS_MASK     (MASK_3_BITS << OCP_PRECIS)

/// AUD_CFGA_RD_STS 0x28 ///
/// aud read states0&1
/* aud_irq_raw	[13: 7]	RO	0	int status: {audio_pacal_irq,audio_hp_dpop_irq,ovp_irq,otp_irq,pa_ocp_irq,ear_ocp_irq,hp_ocp_irq}
 * aud_irq_msk	[6: 0]	RO	0	int mask = aud_irq_raw & aud_int_en */
#define AUDIO_IRQ_RAW                  (7)
#define AUDIO_IRQ_RAW_MASK             (MASK_7_BITS << AUDIO_IRQ_RAW)
#define AUDIO_PACAL_IRQ_RAW_STATUS     (BIT_13) // TODO: Maybe BIT_7
#define AUDIO_HP_DPOP_IRQ_RAW_STATUS   (BIT_12)
#define OVP_IRQ_RAW_STATUS             (BIT_11)
#define OTP_IRQ_RAW_STATUS             (BIT_10)
#define PA_OCP_IRQ_RAW_STATUS          (BIT_9)
#define EAR_OCP_IRQ_RAW_STATUS         (BIT_8)
#define HP_OCP_IRQ_RAW_STATUS          (BIT_7)

#define AUDIO_IRQ                        (0)
#define AUDIO_IRQ_MASK                   (MASK_7_BITS << AUDIO_IRQ)
#define AUDIO_PACAL_IRQ_MASK_STATUS      (BIT_6) // TODO: Maybe BIT_0
#define AUDIO_HP_DPOP_IRQ_MASK_STATUS    (BIT_5)
#define OVP_IRQ_MASK_STATUS              (BIT_4)
#define OTP_IRQ_MASK_STATUS              (BIT_3)
#define PA_OCP_IRQ_MASK_STATUS           (BIT_2)
#define EAR_OCP_IRQ_MASK_STATUS          (BIT_1)
#define HP_OCP_IRQ_MASK_STATUS           (BIT_0)

/// AUD_CFGA_INT_MODULE_CTRL 0x2c ///
/// aud interrupt module control
/* AUD_INT_EN	[6: 0]	RW	0	int enable: {audio_pacal_irq,audio_hp_dpop_irq,ovp_irq,otp_irq,pa_ocp_irq,ear_ocp_irq,hp_ocp_irq} */
#define AUDIO_IRQ_EN_MASK       (MASK_7_BITS)
#define AUDIO_PACAL_IRQ_EN      (BIT_6) // TODO: Maybe BIT_0
#define AUDIO_HP_DPOP_IRQ_EN    (BIT_5)
#define OVP_IRQ_EN              (BIT_4)
#define OTP_IRQ_EN              (BIT_3)
#define PA_OCP_IRQ_EN           (BIT_2)
#define EAR_OCP_IRQ_EN          (BIT_1)
#define HP_OCP_IRQ_EN           (BIT_0)


/// AUD_CFGA_LP_MODULE_CTRL 0x30 ///
/// aud loop module control
/* ADC_EN_R             [5]	RW	0	adc right enable
 * DAC_EN_R             [4]	RW	0	dac right enable
 * ADC_EN_L             [3]	RW	0	adc left enable
 * DAC_EN_L             [2]	RW	0	adc right enable
 * AUDIO_LOOP_MAP_SEL	[1]	RW	0x1
 * AUDIO_ADIE_LOOP_EN	[0]	RW	0 */
#define ADC_EN_R            (BIT_5)
#define DAC_EN_R            (BIT_4)
#define ADC_EN_L            (BIT_3)
#define DAC_EN_L            (BIT_2)
#define AUDIO_LOOP_MAP_SEL  (BIT_1)
#define AUDIO_ADIE_LOOP_EN  (BIT_0)


/// ANA_ET2 0x34 ///
/// Audio DACL & DACR output mixer select
/* DEM_BYPASS           [4]     RW	0	bypass DEM
 * RG_AUD_DALR_MIX_SEL	[3:2]	RW	0	Audio DACL & DACR output mixer select
                                        00 = R/L         01 = R/0
                                        10 = 0/L         11 = 0/0
 * RG_AUD_DAS_MIX_SEL	[1:0]	RW	0	Audio DACL & DACR output mixer select
                                        00 = L + R       01 = 2 x L
                                        10 = 2 x R       11 = 0 */
#define DEM_BYPASS
#define AUD_DALR_MIX_SEL        (2)
#define AUD_DALR_MIX_SEL_MASK   (MASK_2_BITS << AUD_DALR_MIX_SEL)
#define AUD_DALR_MIX_RL_MODE    (0x00)
#define AUD_DALR_MIX_R0_MODE    (0x01)
#define AUD_DALR_MIX_0L_MODE    (0x10)
#define AUD_DALR_MIX_00_MODE    (0x11)
#define AUD_DAS_MIX_SEL         (0)
#define AUD_DAS_MIX_SEL_MASK    (MASK_2_BITS << AUD_DALR_MIX_SEL)
#define AUD_DAS_MIX_RL_MODE     (0x00)
#define AUD_DAS_MIX_2L_MODE     (0x01)
#define AUD_DAS_MIX_2R_MODE     (0x10)
#define AUD_DAS_MIX_00_MODE     (0x11)


/// CLK_EN 0x38 ///
/// clock enable
/* CLK_AUD_LOOP_INV_EN	[7]	RW	0   // invalid
 * CLK_AUD_6P5M_EN      [6]	RW	0   // invalid
 * CLK_AUD_LOOP_EN      [5]	RW	0
 * CLK_AUD_HBD_EN       [4]	RW	0
 * CLK_AUD_HID_EN       [3]	RW	0
 * CLK_AUD_1K_EN        [2]	RW	0
 * CLK_AUD_IMPD_EN      [1]	RW	0
 * CLK_AUD_32K_EN       [0]	RW	0 */
#define CLK_AUD_MASK         (MASK_7_BITS)
#define CLK_AUD_LOOP_INV     (BIT_7) // invalid
#define CLK_AUD_6P5M         (BIT_6) // invalid
#define CLK_AUD_LOOP         (BIT_5)
#define CLK_AUD_HBD          (BIT_4)
#define CLK_AUD_HID          (BIT_3)
#define CLK_AUD_1K           (BIT_2)
#define CLK_AUD_IMPD         (BIT_1)
#define CLK_AUD_32K          (BIT_0)


/// SOFT_RST 0x3c ///
/// audio soft reset
/* AUD_DAC_POST_SOFT_RST    [7]	WC	0
 * AUD_DIG_6P5M_SOFT_RST    [6]	WC	0
 * AUD_DIG_LOOP_SOFT_RST    [5]	WC	0
 * AUD_1K_SOFT_RST	        [4]	WC	0
 * AUD_HBD_SOFT_RST	        [3]	WC	0
 * AUD_HID_SOFT_RST	        [2]	WC	0
 * AUD_IMPD_SOFT_RST        [1]	WC	0
 * AUD_32K_SOFT_RST	        [0]	WC	0 */
#define AUD_DAC_POST_SOFT_RST   (BIT_7)
#define AUD_DIG_6P5M_SOFT_RST   (BIT_6)
#define AUD_DIG_LOOP_SOFT_RS    (BIT_5)
#define AUD_1K_SOFT_RST         (BIT_4)
#define AUD_HBD_SOFT_RST        (BIT_3)
#define AUD_HID_SOFT_RST	    (BIT_2)
#define AUD_IMPD_SOFT_RST       (BIT_1)
#define AUD_32K_SOFT_RST	    (BIT_0)


#define SC1161_AUD_CFGA_REG_BASE  (0x82001280)

typedef struct
{
    uint32  aud_cfgal_clr;           	 //0x00

    uint32  aud_cfga_hid_cfg0;           //0x04
    uint32  aud_cfga_hid_cfg1;           //0x08
    uint32  aud_cfga_hid_cfg2;           //0x0c
    uint32  aud_cfga_hid_cfg3;           //0x10
    uint32  aud_cfga_hid_cfg4;           //0x14
    uint32  aud_cfga_hid_cfg5;           //0x18

    uint32  aud_cfga_hid_sts0;           //0x1c

    uint32  aud_cfga_prt_cfg_0;          //0x20
    uint32  aud_cfga_prt_cfg_1;          //0x24

    uint32  aud_cfga_rd_sts;             //0x28

    uint32  aud_cfga_int_module_ctrl;    //002c
    uint32  aud_cfga_lp_module_ctrl;     //0030

    uint32  ana_et2;           			 //0034
    uint32  clk_en;          			 //0038
    uint32  soft_rst;          			 //003c
} SPRD_AUD_CFGA_REG_CTL_T;


#endif//__SPRD_CODEC_AP_PHY_SC1161_H___
