/******************************************************************************
 ** File Name:      dol_phy_v1.h                                              *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                   *
 *****************************************************************************/
#ifndef _DOL_PHY_V1_H_
#define _DOL_PHY_V1_H_

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
#define VBAICR                      (DOLPHIN_REG_BASE + 0x00)
#define VBCR1                       (DOLPHIN_REG_BASE + 0x04)
#define VBCR2                       (DOLPHIN_REG_BASE + 0x08)
#define VBCCR1                      (DOLPHIN_REG_BASE + 0x0C)
#define VBCCR2                      (DOLPHIN_REG_BASE + 0x10)
#define VBPMR1                      (DOLPHIN_REG_BASE + 0x14)
#define VBPMR2                      (DOLPHIN_REG_BASE + 0x18)
#define VBCRR                       (DOLPHIN_REG_BASE + 0x1C)
#define VBICR                       (DOLPHIN_REG_BASE + 0x20)
#define VBIFR                       (DOLPHIN_REG_BASE + 0x24)
#define VBCGR1                      (DOLPHIN_REG_BASE + 0x28)
#define VBCGR2                      (DOLPHIN_REG_BASE + 0x2C)
#define VBCGR3                      (DOLPHIN_REG_BASE + 0x30)
// 0x0134~0x0140 Reserved
#define VBCGR8                      (DOLPHIN_REG_BASE + 0x44)
#define VBCGR9                      (DOLPHIN_REG_BASE + 0x48)
#define VBCGR10                     (DOLPHIN_REG_BASE + 0x4C)
#define VBTR1                       (DOLPHIN_REG_BASE + 0x50)
#define VBTR2                       (DOLPHIN_REG_BASE + 0x54)

//VBAICR

//VBCR1
#define VBCR1_BTL_MUTE_SHIFT        (1)
#define VBCR1_BTL_MUTE_MASK         (1 << VBCR1_BTL_MUTE_SHIFT)
#define VBCR1_BTL_MUTE_DISABLE      (0 << VBCR1_BTL_MUTE_SHIFT)
#define VBCR1_BTL_MUTE_ENABLE       (1 << VBCR1_BTL_MUTE_SHIFT)

#define VBCR1_BYPASS_SHIFT          (2)
#define VBCR1_BYPASS_MASK           (1 << VBCR1_BYPASS_SHIFT)
#define VBCR1_BYPASS_IGNORED        (0 << VBCR1_BYPASS_SHIFT)
#define VBCR1_BYPASS_SELECTED       (1 << VBCR1_BYPASS_SHIFT)

#define VBCR1_DACSEL_SHIFT          (3)
#define VBCR1_DACSEL_MASK           (1 << VBCR1_DACSEL_SHIFT)
#define VBCR1_DACSEL_IGNORED        (0 << VBCR1_DACSEL_SHIFT)
#define VBCR1_DACSEL_SELECTED       (1 << VBCR1_DACSEL_SHIFT)

#define VBCR1_HP_DIS_SHIFT          (4)
#define VBCR1_HP_DIS_MASK           (1 << VBCR1_HP_DIS_SHIFT)
#define VBCR1_HP_DIS_DISABLE        (0 << VBCR1_HP_DIS_SHIFT)
#define VBCR1_HP_DIS_ENABLE         (1 << VBCR1_HP_DIS_SHIFT)

#define VBCR1_DAC_MUTE_SHIFT        (5)
#define VBCR1_DAC_MUTE_MASK         (1 << VBCR1_DAC_MUTE_SHIFT)
#define VBCR1_DAC_MUTE_DISABLE      (0 << VBCR1_DAC_MUTE_SHIFT)
#define VBCR1_DAC_MUTE_ENABLE       (1 << VBCR1_DAC_MUTE_SHIFT)

#define VBCR1_MONO_SHIFT            (6)
#define VBCR1_MONO_MASK             (1 << VBCR1_MONO_SHIFT)
#define VBCR1_MONO_DISABLE          (0 << VBCR1_MONO_SHIFT)
#define VBCR1_MONO_ENABLE           (1 << VBCR1_MONO_SHIFT)

#define VBCR1_SB_MICBIAS_SHIFT      (7)
#define VBCR1_SB_MICBIAS_MASK       (1 << VBCR1_SB_MICBIAS_SHIFT)
#define VBCR1_SB_MICBIAS_ENABLE     (0 << VBCR1_SB_MICBIAS_SHIFT)
#define VBCR1_SB_MICBIAS_DISABLE    (1 << VBCR1_SB_MICBIAS_SHIFT)

//VBCR2
#define VBCR2_MICSEL_SHIFT          (1)
#define VBCR2_MICSEL_MASK           (1 << VBCR2_MICSEL_SHIFT)

#define VBCR2_MICSEL_MIC1           (0 << VBCR2_MICSEL_SHIFT)
#define VBCR2_MICSEL_MIC2           (1 << VBCR2_MICSEL_SHIFT)

#define VBCR2_ADCHPF_SHIFT          (2)
#define VBCR2_ADCHPF_MASK           (1 << VBCR2_ADCHPF_SHIFT)
#define VBCR2_ADCHPF_DIS            (0 << VBCR2_ADCHPF_SHIFT)
#define VBCR2_ADCHPF_EN             (1 << VBCR2_ADCHPF_SHIFT)

#define VBCR2_ADCADWL_SHIFT         (3)
#define VBCR2_ADCADWL_MASK          (0x3<<VBCR2_ADCADWL_SHIFT)

#define VBCR2_DACADWL_SHIFT         (5)
#define VBCR2_DACADWL_MASK          (0x3<<VBCR2_DACADWL_SHIFT)

#define VBCR2_DACDEEMP_SHIFT        (7)

//VBCCR
#define VBCCR_ADC_FS_SHIFT          (0)
#define VBCCR_ADC_FS_MASK           (0xF << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_96000          (0 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_48000          (1 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_44100          (2 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_32000          (3 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_24000          (4 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_22050          (5 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_16000          (6 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_12000          (7 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_11025          (8 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_9600           (9 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_8000           (10<< VBCCR_ADC_FS_SHIFT)

#define VBCCR_DAC_FS_SHIFT          (4)
#define VBCCR_DAC_FS_MASK           (0xF << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_96000          (0 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_48000          (1 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_44100          (2 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_32000          (3 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_24000          (4 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_22050          (5 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_16000          (6 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_12000          (7 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_11025          (8 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_9600           (9 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_8000           (10 << VBCCR_DAC_FS_SHIFT)

//VBPMR1
#define VBPMR1_SB_LOUT_SHIFT        (1)
#define VBPMR1_SB_LOUT_MASK         (1 << VBPMR1_SB_LOUT_SHIFT)
#define VBPMR1_SB_LOUT_POWER_ON     (0 << VBPMR1_SB_LOUT_SHIFT)
#define VBPMR1_SB_LOUT_POWER_DOWN   (1 << VBPMR1_SB_LOUT_SHIFT)

#define VBPMR1_SB_BTL_SHIFT         (2)
#define VBPMR1_SB_BTL_MASK          (1 << VBPMR1_SB_BTL_SHIFT)
#define VBPMR1_SB_BTL_POWER_ON      (0 << VBPMR1_SB_BTL_SHIFT)
#define VBPMR1_SB_BTL_POWER_DOWN    (1 << VBPMR1_SB_BTL_SHIFT)

#define VBPMR1_SB_LIN_SHIFT         (3)
#define VBPMR1_SB_LIN_MASK          (1 << VBPMR1_SB_LIN_SHIFT)
#define VBPMR1_SB_LIN_POWER_ON      (0 << VBPMR1_SB_LIN_SHIFT)
#define VBPMR1_SB_LIN_POWER_DOWN    (1 << VBPMR1_SB_LIN_SHIFT)

#define VBPMR1_SB_ADC_SHIFT         (4)
#define VBPMR1_SB_ADC_MASK          (1 << VBPMR1_SB_ADC_SHIFT)
#define VBPMR1_SB_ADC_POWER_ON      (0 << VBPMR1_SB_ADC_SHIFT)
#define VBPMR1_SB_ADC_POWER_DOWN    (1 << VBPMR1_SB_ADC_SHIFT)

#define VBPMR1_SB_MIX_SHIFT         (5)
#define VBPMR1_SB_MIX_MASK          (1 << VBPMR1_SB_MIX_SHIFT)
#define VBPMR1_SB_MIX_POWER_ON      (0 << VBPMR1_SB_MIX_SHIFT)
#define VBPMR1_SB_MIX_POWER_DOWN    (1 << VBPMR1_SB_MIX_SHIFT)

#define VBPMR1_SB_OUT_SHIFT         (6)
#define VBPMR1_SB_OUT_MASK          (1 << VBPMR1_SB_OUT_SHIFT)
#define VBPMR1_SB_OUT_POWER_ON      (0 << VBPMR1_SB_OUT_SHIFT)
#define VBPMR1_SB_OUT_POWER_DOWN    (1 << VBPMR1_SB_OUT_SHIFT)

#define VBPMR1_SB_DAC_SHIFT         (7)
#define VBPMR1_SB_DAC_MASK          (1 << VBPMR1_SB_DAC_SHIFT)
#define VBPMR1_SB_DAC_POWER_ON      (0 << VBPMR1_SB_DAC_SHIFT)
#define VBPMR1_SB_DAC_POWER_DOWN    (1 << VBPMR1_SB_DAC_SHIFT)

//VBPMR2
#define VBPMR2_SB_SLEEP_SHIFT       (0)
#define VBPMR2_SB_SLEEP_MASK        (1 << VBPMR2_SB_SLEEP_SHIFT)
#define VBPMR2_SB_SLEEP_POWER_ON    (0 << VBPMR2_SB_SLEEP_SHIFT)
#define VBPMR2_SB_SLEEP_POWER_DOWN  (1 << VBPMR2_SB_SLEEP_SHIFT)

#define VBPMR2_SB_SHIFT             (1)
#define VBPMR2_SB_MASK              (1 << VBPMR2_SB_SHIFT)
#define VBPMR2_SB_POWER_ON          (0 << VBPMR2_SB_SHIFT)
#define VBPMR2_SB_POWER_DOWN        (1 << VBPMR2_SB_SHIFT)

#define VBPMR2_SB_MC_SHIFT          (2)
#define VBPMR2_SB_MC_MASK           (1 << VBPMR2_SB_MC_SHIFT)
#define VBPMR2_SB_MC_POWER_ON       (0 << VBPMR2_SB_MC_SHIFT)
#define VBPMR2_SB_MC_POWER_DOWN     (1 << VBPMR2_SB_MC_SHIFT)


#define VBPMR2_GIM_SHIFT            (3)
#define VBPMR2_GIM_MASK             (1 << VBPMR2_GIM_SHIFT)
#define VBPMR2_GIM_POWER_ON         (0 << VBPMR2_GIM_SHIFT)
#define VBPMR2_GIM_POWER_DOWN       (1 << VBPMR2_GIM_SHIFT)

#define VBPMR2_RLGOD_SHIFT          (4)
#define VBPMR2_RLGOD_MASK           (0x3 << VBPMR2_RLGOD_SHIFT)

//VBIFR
#define VBIFR_GAIN_DOWN_DONE_SHIFT  (0)
#define VBIFR_GAIN_DOWN_DONE_MASK   (1 << VBIFR_GAIN_DOWN_DONE_SHIFT)

#define VBIFR_GAIN_UP_DONE_SHIFT    (1)
#define VBIFR_GAIN_UP_DONE_MASK     (1 << VBIFR_GAIN_UP_DONE_SHIFT)

#define VBIFR_RAMP_DOWN_DONE_SHIFT  (2)
#define VBIFR_RAMP_DOWN_DONE_MASK   (1 << VBIFR_RAMP_DOWN_DONE_SHIFT)

#define VBIFR_RAMP_UP_DONE_SHIFT    (3)
#define VBIFR_RAMP_UP_DONE_MASK     (1 << VBIFR_RAMP_UP_DONE_SHIFT)

//VBCGR1
#define VBCGR1_GODL_SHIFT           (0)
#define VBCGR1_GODL_MASK            (0xF << VBCGR1_GODL_SHIFT)

#define VBCGR1_GODR_SHIFT           (4)
#define VBCGR1_GODR_MASK            (0xF << VBCGR1_GODR_SHIFT)

//VBCGR2
#define VBCGR2_GOBL_SHIFT           (0)
#define VBCGR2_GOBL_MASK            (0x1F << VBCGR2_GOBL_SHIFT)

#define VBCGR2_LRGOB_SHIFT          (6)
#define VBCGR2_LRGOB_MASK           (0x3 << VBCGR2_LRGOB_SHIFT)

//VBCGR3
#define VBCGR3_GOBR_SHIFT           (0)
#define VBCGR3_GOBR_MASK            (0x1F << VBCGR3_GOBR_SHIFT)

#define VBCGR3_GOBTL_SHIFT          (7)
#define VBCGR3_GOBTL_MASK           (0x1 << VBCGR3_GOBTL_SHIFT)

//VBCGR8
#define VBCGR8_GOL_SHIFT            (0)
#define VBCGR8_GOL_MASK             (0x1F << VBCGR8_GOL_SHIFT)

#define VBCGR8_LRGO_SHIFT           (6)
#define VBCGR8_LRGO_MASK            (0x3 << VBCGR8_LRGO_SHIFT)

//VBCGR9
#define VBCGR9_GOR_SHIFT            (0)
#define VBCGR9_GOR_MASK             (0x1F << VBCGR9_GOR_SHIFT)

//VBCGR10
#define VBCGR10_GI_SHIFT            (4)
#define VBCGR10_GI_MASK             (0xF << VBCGR10_GI_SHIFT)


#define DOL_SAMPLERATE_NUM_MAX      11

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  //_DOL_PHY_V1_H_


