/******************************************************************************
 ** File Name:      sprd_codec_ap_phy.c                                    *
 ** Author:        zhenfang.wang                                             *
 ** DATE:           10/16/2015                                                *
 ** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec ap control                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/30/2016     ye.zhang      Create for sc1161.                           *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "audio_drvapi.h"
#include "../sprd_codec_phy.h"
#include "sprd_codec_ap_phy_sc1161.h"
#include "ldo_drvapi.h"
#include "adc_drvapi.h"

#ifdef CODEC_DEBUG
#define SPRD_CODEC_AP_PRINT    SCI_TRACE_LOW
#else
#define SPRD_CODEC_AP_PRINT(...)
#endif

#define SPRD_CODEC_AP_REG_BF_SET_VAL(_Reg, _Value, _Lsb, _Mask)	SPRD_CODEC_AP_WRITE_BITS(_Reg,(_Value<<_Lsb),_Mask)

LOCAL SPRD_CODEC_AP_REG_CTL_T * aud_codec_ctrl_ptr = (SPRD_CODEC_AP_REG_CTL_T *)SC1161_AP_REG_BASE;

#define VB_LDO_V_29  0x00
#define VB_LDO_V_30  0x04
#define VB_LDO_V_31  0x08
#define VB_LDO_V_32  0x0C
#define VB_LDO_V_33  0x10
enum
{
    SPRD_CODEC_MAIN_MIC = BIT_0,
    SPRD_CODEC_AUX_MIC  = BIT_1,
    SPRD_CODEC_HP_MIC   = BIT_2,

    SPRD_CODEC_NONE     = 0
};

typedef enum
{
    SPRD_CODEC_MAIN_MIC_NONE_OCCUPIED = 0,
    SPRD_CODEC_MAIN_MIC_HP_OCCUPIED   = BIT_0,
    SPRD_CODEC_MAIN_MIC_MIC_OCCUPIED  = BIT_1,
}MAIN_MIC_OCCUPIED_E;

#define CONFIG_REGS_NUM 18

// There are 22 regs in all for audio codec:
// put first 18 regs to table (0x00 to 0x44)
// sts1 sts2 are read-only, sts3 higher half is read-only
// sts4 used for calibration, needs to be configured later
/*
   const static uint16 g_audio_calibration_table[CONFIG_REGS_NUM] = {
   0xBC80, 0x840C, 0x1200, 0xE1E0, 0x3D8D, 0x2400,
   0x12,   0x4,    0x0,    0x1244, 0x40,   0x23,
   0x0,    0xFFFF, 0x0,    0x2CD8, 0x7E6C, 0x3820,
   };
   */
const static uint16 g_audio_calibration_table[CONFIG_REGS_NUM] = {
    0xBB80, 0x840C, 0x1200, 0xE060, 0x3D8D, 0x2400,
    0x12,   0x0,    0x0,    0x1444, 0x40,   0x23,
    0x0,    0xFFFF, 0x0,    0x2CD8, 0x7E6C, 0x3820,
};

static uint16 g_codec_ap_regs[CONFIG_REGS_NUM] = {0};

typedef enum
{
    UNKNOWN_MIC_TYPE  = 0,
    _3_POLE_MIC_TYPE  = 3,
    _4_POLE_MIC_TYPE  = 4
}E_MIC_TYPE;

typedef struct HeadPhoneSt{
    uint32 Connected;
    E_MIC_TYPE MicType;
}T_HEADPHONE_STATUS;

T_HEADPHONE_STATUS g_HeadPhoneStatus = {0,UNKNOWN_MIC_TYPE};

// big scale
#define HEADMIC_ADC_2_685V 3054 //2.685V

// small scale
#define HEADMIC_ADC_0_9V   3071 //0.9V
#define HEADMIC_ADC_0_1V   341  //0.1V

LOCAL void _sprd_codec_load_calibration_table()
{
    uint32 i = 0;
    uint32 reg_add = SC1161_AP_REG_BASE;
    for(i = 0; i < CONFIG_REGS_NUM; i++,reg_add += 4)
        SPRD_CODEC_AP_REG_SET(reg_add, g_audio_calibration_table[i]);
    SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_sts3, 0x0);
}

LOCAL void _sprd_codec_dc_calibration_start()
{
    SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_sts4, 0x6006);
    SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_sts4, 0xE006);
}

LOCAL void _sprd_codec_ap_save_regs()
{
    uint32 i = 0;
    uint32 reg_add = SC1161_AP_REG_BASE;
    for(i = 0; i < CONFIG_REGS_NUM; i++,reg_add += 4)
        g_codec_ap_regs[i] = SPRD_CODEC_AP_REG_GET(reg_add);
}

LOCAL void _sprd_codec_ap_restore_regs()
{
    uint32 i = 0;
    uint32 reg_add = SC1161_AP_REG_BASE;
    for(i = 0; i < CONFIG_REGS_NUM; i++,reg_add += 4)
        SPRD_CODEC_AP_REG_SET(reg_add, g_codec_ap_regs[i]);
}

LOCAL void _sprd_codec_ap_en_op(uint32 en, void* reg, uint32 bit)
{
    if (en) {
        SPRD_CODEC_AP_WRITE_BITS((uint32)reg, (bit), (bit));
    } else {
        SPRD_CODEC_AP_WRITE_BITS((uint32)reg, ~(bit), (bit));
    }
}

LOCAL void _sprd_codec_ap_chan_en(uint32 en, uint32 chan, void* reg,
        uint32 l_val, uint32 r_val)
{
    if (en) {
        if (AUDIO_IS_CHAN_L(chan))
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg, (l_val), (l_val));
        if (AUDIO_IS_CHAN_R(chan))
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg, (r_val), (r_val));
    } else {
        if (AUDIO_IS_CHAN_L(chan))
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg, ~(l_val), (l_val));
        if (AUDIO_IS_CHAN_R(chan))
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg, ~(r_val), (r_val));
    }
}

LOCAL void _sprd_codec_ap_path_op(uint32 path, void* reg, uint32 en,
        uint32 adc_bit, uint32 dac_bit)
{
    switch (path) {
        case AUDIO_PLAYBACK:
            _sprd_codec_ap_en_op(en, reg, adc_bit);
            break;
        case AUDIO_CAPTRUE:
            _sprd_codec_ap_en_op(en, reg, dac_bit);
            break;
        default:
            break;
    }
}

LOCAL void _sprd_codec_ap_path_mask(uint32 path, void* reg, uint32 val,
        uint32 adc_mask, uint32 adc_shif,
        uint32 dac_mask, uint32 dac_shif)
{
    switch (path) {
        case AUDIO_PLAYBACK:
            SPRD_CODEC_AP_WRITE_BITS(reg, (val<<dac_shif), dac_mask);
            break;
        case AUDIO_CAPTRUE:
            SPRD_CODEC_AP_WRITE_BITS(reg, (val<<adc_shif), adc_mask);
            break;
        default:
            break;
    }
}

LOCAL void _sprd_codec_ap_chan_mask(uint32 chan, void* reg, uint32 val,
        uint32 r_mask,uint32 r_shif,
        uint32 l_mask,uint32 l_shif)
{
    if (AUDIO_IS_CHAN_L(chan))
        SPRD_CODEC_AP_WRITE_BITS(reg, (val<<l_shif), l_mask);
    if (AUDIO_IS_CHAN_R(chan))
        SPRD_CODEC_AP_WRITE_BITS(reg, (val<<r_shif), r_mask);
}

/// ANA_PMU0

PUBLIC void __sprd_codec_ap_vb_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_pmu0, VB_EN);
}
LOCAL __sprd_codec_ap_vbo_set(uint32 vol){
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_pmu1,
            vol, VB_V, VB_V_MASK);
}
LOCAL __inline void __sprd_codec_ap_vb_sleep_mode_en(uint32 en)
{
    _sprd_codec_ap_en_op(!en, &aud_codec_ctrl_ptr->ana_pmu0, VB_HDMC_SP_PD);
}

LOCAL __inline void __sprd_codec_ap_vcmi_sel(uint32 voltage_sel)
{
    _sprd_codec_ap_en_op(voltage_sel, &aud_codec_ctrl_ptr->ana_pmu0, VCMI_SEL);
}

LOCAL void _sprd_codec_ap_bg_ibias_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_pmu0, BIAS_EN);
}

LOCAL void _sprd_codec_ap_mic_bias(uint32 en)
{
    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_mic_bias %d", en);
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_pmu0, MICBIAS_PLGB);
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_pmu0, MICBIAS_SEL1P5);
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_pmu0, MICBIAS_EN);
}

/// ANA_PMU2

LOCAL void _sprd_codec_ap_adc_pga_ibias_current(uint32 current)
{
    if (current > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_pmu2,
            current, ADPGA_I, ADPGA_I_MASK);
}

/// ANA_PMU3

LOCAL void _sprd_codec_ap_drv_over_cur_protect(uint32 val)
{
    if (val > 0xF)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_pmu3,
            val, DRV_OCP_PD, DRV_OCP_PD_MASK);
}

/// ANA_CLK0

LOCAL void _sprd_codec_ap_digital_core_clock_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_clk0, DIG_CLK_6P5M_EN);
}

LOCAL void _sprd_codec_ap_digital_loop_clock_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_clk0, DIG_CLK_LOOP_EN);
}

LOCAL void _sprd_codec_ap_analog_core_clock_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_clk0, ANA_CLK_EN);
}

LOCAL void _sprd_codec_ap_drv_clk_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_clk0, DRV_CLK_EN);
}

LOCAL void _sprd_codec_ap_adc_clk_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_clk0, AD_CLK_EN);
}

LOCAL void _sprd_codec_ap_dac_clk_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_clk0, DA_CLK_EN);
}

LOCAL void _sprd_codec_ap_adc_clk_rst(void)
{
    _sprd_codec_ap_en_op(1, &aud_codec_ctrl_ptr->ana_clk0, AD_CLK_RST);
    _sprd_codec_ap_en_op(0, &aud_codec_ctrl_ptr->ana_clk0, AD_CLK_RST);
}

LOCAL void _sprd_codec_ap_clk_en(uint32 en)
{
    _sprd_codec_ap_digital_core_clock_en(en);
    _sprd_codec_ap_digital_loop_clock_en(en);
    _sprd_codec_ap_analog_core_clock_en(en);

    // TODO: add api
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_clk0, 0xD, 0, (0xF << 0));
}

/// ANA_CDC0

LOCAL void _sprd_codec_ap_adc_pga_bias_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc0, ADPGA_IBIAS_EN);
}

LOCAL void _sprd_codec_ap_adc_pga_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc0, ADPGA_EN);
}

LOCAL void _sprd_codec_ap_adc_pga_bypass_select(uint32 sel)
{
    if (sel > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc0,
            sel, ADPGA_BYP, ADPGA_BYP_MASK);
}

LOCAL void _sprd_codec_ap_adc_ref_select(uint32 ref_sel)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc0,
            ref_sel, AD_REFSEL, AD_REFSEL_MASK);
}

//TODO: no detail
LOCAL void _sprd_codec_ap_adc_ni_lpm(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc0, AD_NI_LPM);
}

LOCAL void _sprd_codec_ap_adc_reset(void)
{
    _sprd_codec_ap_en_op(1, &aud_codec_ctrl_ptr->ana_cdc0, AD_RST);
    _sprd_codec_ap_en_op(0, &aud_codec_ctrl_ptr->ana_cdc0, AD_RST);
}

PUBLIC void __sprd_codec_ap_adc_en(uint32 en, uint32 chan)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc0, AD_EN);
    //reset adc
    _sprd_codec_ap_adc_reset();
}

/// ANA_CDC1

LOCAL void _sprd_codec_ap_adc_pga_gain(uint32 gain)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc1,
            gain, ADPGA_G, ADPGA_G_MASK);
}

LOCAL void _sprd_codec_ap_dac_dc_offset_trim_select(uint32 sel)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc1,
            sel, DALR_OS_D, DALR_OS_D_MASK);
}

LOCAL void _sprd_codec_ap_das_dc_offset_trim_select(uint32 sel)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc1,
            sel, DAS_OS_D, DAS_OS_D_MASK);
}

PUBLIC void __sprd_codec_ap_dac_dc_trim_select(uint32 val)
{
    _sprd_codec_ap_dac_dc_offset_trim_select(val);
    _sprd_codec_ap_das_dc_offset_trim_select(val);
}
/// ANA_CDC2

LOCAL void _sprd_codec_ap_vcm_buf_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc2, HPBUF_EN);
}

LOCAL void _sprd_codec_ap_hpl_dummy_loop_en(uint32 en)
{
    uint32 temp_reg = 0;
    if (en) {
        temp_reg = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc2);
        temp_reg &= ~HPL_FLOOP_END;
        temp_reg |= HPL_FLOOPEN;
        SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_cdc2, temp_reg);
    } else {
        temp_reg = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc2);
        temp_reg |= HPL_FLOOP_END;
        temp_reg &= ~HPL_FLOOPEN;
        SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_cdc2, temp_reg);
    }
}

LOCAL void _sprd_codec_ap_hplr_dummy_loop_dis_all()
{
    uint32 temp_reg;
    temp_reg = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc2);
    temp_reg &= ~HPL_FLOOP_END;
    temp_reg &= ~HPL_FLOOPEN;
    temp_reg &= ~HPR_FLOOP_END;
    temp_reg &= ~HPR_FLOOPEN;
    SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_cdc2, temp_reg);
}

LOCAL void _sprd_codec_ap_hpr_dummy_loop_en(uint32 en)
{
    uint32 temp_reg = 0;
    if (en) {
        temp_reg = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc2);
        temp_reg &= ~HPR_FLOOP_END;
        temp_reg |= HPR_FLOOPEN;
        SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_cdc2, temp_reg);
    } else {
        temp_reg = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc2);
        temp_reg |= HPR_FLOOP_END;
        temp_reg &= ~HPR_FLOOPEN;
        SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_cdc2, temp_reg);
    }
}

LOCAL void _sprd_codec_ap_receiver_dummy_loop_end_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc2, RCV_FLOOP_END);
}

/// ANA_CDC3

LOCAL void _sprd_codec_ap_dac_dc_offset_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, DALR_OS_EN);
}

LOCAL void _sprd_codec_ap_dacs_dc_offset_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, DAS_OS_EN);
}

LOCAL void _sprd_codec_ap_ng_pa_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, NG_PA_EN);
}

LOCAL void _sprd_codec_ap_dacs_to_pa(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, SDAPA);
}

LOCAL void _sprd_codec_ap_ear_stereo(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, RCV_PATH_OPT);
}

LOCAL void _sprd_codec_ap_mic_to_adc_pga_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, SMICPGA);
}

LOCAL void _sprd_codec_ap_headmic_to_adc_pga_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, SHMICPGA);
}

LOCAL void _sprd_codec_ap_main_mic(uint32 en, MAIN_MIC_OCCUPIED_E flag)
{
    static MAIN_MIC_OCCUPIED_E s_MainMicOccupiedFlag = SPRD_CODEC_MAIN_MIC_NONE_OCCUPIED;
    if(en){
        if(SPRD_CODEC_MAIN_MIC_NONE_OCCUPIED == s_MainMicOccupiedFlag){
            _sprd_codec_ap_mic_bias(1);
            _sprd_codec_ap_mic_to_adc_pga_en(1);
        }
        s_MainMicOccupiedFlag |= flag;
    }else{
        s_MainMicOccupiedFlag &= ~flag;
        if(SPRD_CODEC_MAIN_MIC_NONE_OCCUPIED == s_MainMicOccupiedFlag){
            _sprd_codec_ap_mic_bias(0);
            _sprd_codec_ap_mic_to_adc_pga_en(0);
        }
    }
}

/// ANA_CDC4

LOCAL void _sprd_codec_ap_set_speaker_gain(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc4,
            val, SPK_G, SPK_G_MASK);
}

LOCAL void _sprd_codec_ap_set_receiver_gain(uint32 val)
{
    if (val > 0xF)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc4,
            val, RCV_G, RCV_G_MASK);
}

LOCAL void _sprd_codec_ap_set_headphone_l_gain(uint32 val)
{
    if (val > 0xF)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc4,
            val, HPL_G, HPL_G_MASK);
}

LOCAL void _sprd_codec_ap_set_headphone_r_gain(uint32 val)
{
    if (val > 0xF)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_cdc4,
            val, HPR_G, HPR_G_MASK);
}

/// ANA_DCL0

// must be called
LOCAL void _sprd_codec_ap_digi_ctrl_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_dcl0, DCL_EN);
}

LOCAL void _sprd_codec_ap_digi_reset_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_dcl0, DCL_RST);
}

LOCAL void _sprd_codec_ap_drv_soft_start_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_dcl0, DRV_SOFT_EN);
}

LOCAL __inline void _sprd_codec_ap_drv_delay_timer_control(uint32 time_us)
{
    SPRD_CODEC_AP_WRITE_BITS(&aud_codec_ctrl_ptr->ana_dcl0, time_us<<DRV_SOFT_T, DRV_SOFT_T_MASK);
}

/// ANA_DCL11

// should be changed when set DAC to hp/ear
// reference 1
LOCAL void _sprd_codec_ap_dc_calibration_wait_time(uint32 val)
{
    if (val > 7)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl11,
            val, CALDC_WAIT_T, CALDC_WAIT_T_MASK);
}
// reference 3
LOCAL void _sprd_codec_ap_hpl_depop_clk_n1(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl11,
            val, HPL_DPOP_CLKN1, HPL_DPOP_CLKN1_MASK);
}
// reference 0
LOCAL void _sprd_codec_ap_hpl_depop_n1(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl11,
            val, HPL_DPOP_N1, HPL_DPOP_N1_MASK);
}
// reference 6
LOCAL void _sprd_codec_ap_hpl_depop_val1(uint32 val)
{
    if (val > 7)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl11,
            val, HPL_DPOP_VAL1, HPL_DPOP_VAL1_MASK);
}
// reference 3
LOCAL void _sprd_codec_ap_hpl_depop_clkn2(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl11,
            val, HPL_DPOP_CLKN2, HPL_DPOP_CLKN2_MASK);
}
// reference 0
LOCAL void _sprd_codec_ap_hpl_depop_n2(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl11,
            val, HPL_DPOP_N2, HPL_DPOP_N2_MASK);
}

LOCAL void _sprd_codec_ap_dpop_auto_reset(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_dcl11, DPOP_AUTO_RST);
}

/// ANA_DCL12 ///
LOCAL void _sprd_codec_ap_depopl_pcur_opt(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl12,
            val, DEPOPL_PCUR_OPT, DEPOPL_PCUR_OPT_MASK);
}

LOCAL void _sprd_codec_ap_depopr_pcur_opt(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl12,
            val, DEPOPR_PCUR_OPT, DEPOPR_PCUR_OPT_MASK);
}

// reference 3
LOCAL void _sprd_codec_ap_hpr_depop_clk_n1(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl12,
            val, HPR_DPOP_CLKN1, HPR_DPOP_CLKN1_MASK);
}
// reference 0
LOCAL void _sprd_codec_ap_hpr_depop_n1(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl12,
            val, HPR_DPOP_N1, HPR_DPOP_N1_MASK);
}
// reference 6
LOCAL void _sprd_codec_ap_hpr_depop_val1(uint32 val)
{
    if (val > 7)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl12,
            val, HPR_DPOP_VAL1, HPR_DPOP_VAL1_MASK);
}
// reference 3
LOCAL void _sprd_codec_ap_hpr_depop_clkn2(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl12,
            val, HPR_DPOP_CLKN2, HPR_DPOP_CLKN2_MASK);
}
// reference 0
LOCAL void _sprd_codec_ap_hpr_depop_n2(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl12,
            val, HPR_DPOP_N2, HPR_DPOP_N2_MASK);
}

/// ANA_STS0
// reference 0
LOCAL void _sprd_codec_ap_depop_wait_t1(uint32 val)
{
    if (val > 7)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_sts0,
            val, HP_DEPOP_WAIT_T1, HP_DEPOP_WAIT_T1_MASK);
}

LOCAL void _sprd_codec_ap_depop_wait_t2(uint32 val)
{
    if (val > 7)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_sts0,
            val, HP_DEPOP_WAIT_T2, HP_DEPOP_WAIT_T2_MASK);
}

LOCAL void _sprd_codec_ap_depop_wait_t3(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_sts0,
            val, HP_DEPOP_WAIT_T3, HP_DEPOP_WAIT_T3_MASK);
}
// reference 0
LOCAL void _sprd_codec_ap_depop_wait_t4(uint32 val)
{
    if (val > 3)
        return;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_sts0,
            val, HP_DEPOP_WAIT_T4, HP_DEPOP_WAIT_T4_MASK);
}

/// ANA_STS4
LOCAL void _sprd_codec_ap_depop_charge_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_sts4, DEPOP_CHG_EN);
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_sts4, DEPOP_EN);
}

LOCAL void _sprd_codec_ap_hp_plug_in(uint32 val)
{
    _sprd_codec_ap_en_op(val, &aud_codec_ctrl_ptr->ana_sts4, AUD_PLUGIN);
}

LOCAL void _sprd_codec_ap_depop_charge_start(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_sts4, DEPOP_CHG_START);
}
////////////////////////// Audio codec digital part //////////////////////////
LOCAL SPRD_AUD_CFGA_REG_CTL_T * aud_cfga_ctrl_ptr = (SPRD_AUD_CFGA_REG_CTL_T *)SC1161_AUD_CFGA_REG_BASE;


/// AUD_CFGAL_CLR 0x00
LOCAL void _sprd_codec_aud_ear_shutdown_clr(void)
{
    _sprd_codec_ap_en_op(1, &aud_cfga_ctrl_ptr->aud_cfgal_clr, EAR_SHUTDOWN_CLR);
}

LOCAL void _sprd_codec_aud_hp_shutdown_clr(void)
{
    _sprd_codec_ap_en_op(1, &aud_cfga_ctrl_ptr->aud_cfgal_clr, HP_SHUTDOWN_CLR);
}

LOCAL void _sprd_codec_aud_pa_shutdown_clr(void)
{
    _sprd_codec_ap_en_op(1, &aud_cfga_ctrl_ptr->aud_cfgal_clr, PA_SHUTDOWN_CLR);
}

LOCAL void _sprd_codec_ap_aud_int_clr(uint32 clr_bits)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfgal_clr, clr_bits, BIT_0, AUD_INT_CLR_MASK);
}


/// AUD_CFGA_HID_CFG0 0x04
LOCAL void _sprd_code_ap_aud_head_butten_out_debounce_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_hid_cfg0, AUD_HEAD_BUTTON_OUT_DEBOUNCE_EN);
}

LOCAL void _sprd_code_ap_aud_head_insert_debounce_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_hid_cfg0, AUD_HEAD_INSERT_DEBOUNCE_EN);
}

LOCAL void _sprd_code_ap_aud_hid_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_hid_cfg0, HID_EN);
}


/// AUD_CFGA_HID_CFG1 0x08
LOCAL void _sprd_code_ap_aud_head_insert_high_debounce_thd_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg1, val, HID_HIGH_DBNC_THD0, HID_HIGH_DBNC_THD0_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_insert_high_debounce_thd_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg1)&HID_HIGH_DBNC_THD0_MASK)>>HID_HIGH_DBNC_THD0;
}

LOCAL void _sprd_code_ap_aud_head_insert_low_debounce_thd_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg1, val, HID_LOW_DBNC_THD0, HID_LOW_DBNC_THD0_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_insert_low_debounce_thd_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg1)&HID_LOW_DBNC_THD0_MASK)>>HID_LOW_DBNC_THD0;
}

/// AUD_CFGA_HID_CFG2 0x0c
LOCAL void _sprd_code_ap_aud_head_insert_detect_T1T2_step_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg2, val, HID_TMR_T1T2_STEP, HID_TMR_T1T2_STEP_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_insert_detect_T1T2_step_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg2)&HID_TMR_T1T2_STEP_MASK)>>HID_TMR_T1T2_STEP;
}

LOCAL void _sprd_code_ap_aud_head_insert_detect_T0_timer_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg2, val, HID_TMR_T0, HID_TMR_T0_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_insert_detect_T0_timer_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg2)&HID_TMR_T0_MASK)>>HID_TMR_T0;
}

/// AUD_CFGA_HID_CFG3 0x10
LOCAL void _sprd_code_ap_aud_head_insert_detect_T1_timer_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg3, val, HID_TMR_T1, HID_TMR_T1_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_insert_detect_T1_timer_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg3)&HID_TMR_T1_MASK)>>HID_TMR_T1;
}

/// AUD_CFGA_HID_CFG4 0x14
LOCAL void _sprd_code_ap_aud_head_insert_detect_T2_timer_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg4, val, HID_TMR_T2, HID_TMR_T2_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_insert_detect_T2_timer_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg4)&HID_TMR_T2_MASK)>>HID_TMR_T2;
}

/// AUD_CFGA_HID_CFG5 0x18
LOCAL void _sprd_code_ap_aud_head_button_out_high_debounce_thd_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg5, val, HID_HIGH_DBNC_THD1, HID_HIGH_DBNC_THD1_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_button_out_high_debounce_thd_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg5)&HID_HIGH_DBNC_THD1_MASK)>>HID_HIGH_DBNC_THD1;
}

LOCAL void _sprd_code_ap_aud_head_button_out_low_debounce_thd_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg5, val, HID_LOW_DBNC_THD1, HID_LOW_DBNC_THD1_MASK);
}

LOCAL uint32 _sprd_code_ap_aud_head_button_out_low_debounce_thd_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_cfg5)&HID_LOW_DBNC_THD1_MASK)>>HID_LOW_DBNC_THD1;
}

/// AUD_CFGA_HID_STS0 0x1C
LOCAL uint32 _sprd_code_ap_ear_shutdown_sts(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_sts0)&EAR_SHUTDOWN)? 1:0;
}

LOCAL uint32 _sprd_code_ap_hp_shutdown_sts(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_sts0)&HP_SHUTDOWN)? 1:0;
}

LOCAL uint32 _sprd_code_ap_pa_shutdown_sts(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_sts0)&PA_SHUTDOWN)? 1:0;
}

LOCAL uint32 _sprd_code_ap_head_insert_out_sts(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_sts0)&AUDIO_HEAD_INSERT_OUT)? 1:0;
}

LOCAL uint32 _sprd_code_ap_head_botton_out_sts(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_sts0)&AUDIO_HEAD_BUTTON_OUT)? 1:0;
}

LOCAL uint32 _sprd_code_ap_u1_debounce_fsm_sts1(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_sts0)&AUD_DBNC_STS1_MASK)>>AUD_DBNC_STS1;
}

LOCAL uint32 _sprd_code_ap_u1_debounce_fsm_sts0(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_hid_sts0)&AUD_DBNC_STS0_MASK)>>AUD_DBNC_STS0;
}

/// AUD_CFGA_PRT_CFG_0 0x20
LOCAL void _sprd_code_ap_ear_shutdown_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, EAR_SHUTDOWN_EN);
}

LOCAL void _sprd_code_ap_hp_shutdown_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, HP_SHUTDOWN_EN);
}

LOCAL void _sprd_code_ap_pa_shutdown_en2(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, PA_SHUTDOWN_EN2);
}

LOCAL void _sprd_code_ap_pa_shutdown_en1(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, PA_SHUTDOWN_EN1);
}

LOCAL void _sprd_code_ap_pa_shutdown_en0(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, PA_SHUTDOWN_EN0);
}

LOCAL void _sprd_code_ap_aud_32k_sel(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, AUD_CLK_SEL);
}

LOCAL void _sprd_code_ap_aud_protect_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, AUD_PRT_EN);
}

LOCAL void _sprd_code_ap_otp_prt_thd_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0, val, OTP_PD_THD, OTP_PD_THD_MASK);
}

LOCAL uint32 _sprd_code_ap_otp_prt_thd_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_0)&OTP_PD_THD_MASK)>>OTP_PD_THD;
}

/// AUD_CFGA_PRT_CFG_1 0x24
LOCAL void _sprd_code_ap_otp_precis_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1, val, OTP_PRECIS, OTP_PRECIS_MASK);
}

LOCAL uint32 _sprd_code_ap_otp_precis_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1)&OTP_PRECIS_MASK)>>OTP_PRECIS;
}

LOCAL void _sprd_code_ap_ovp_protc_thd_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1, val, OVP_PD_THD, OVP_PD_THD_MASK);
}

LOCAL uint32 _sprd_code_ap_ovp_protc_thd_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1)&OVP_PD_THD_MASK)>>OVP_PD_THD;
}

LOCAL void _sprd_code_ap_ovp_precis_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1, val, OVP_PRECIS, OVP_PRECIS_MASK);
}

LOCAL uint32 _sprd_code_ap_ovp_precis_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1)&OVP_PRECIS_MASK)>>OVP_PRECIS;
}

LOCAL void _sprd_code_ap_ocp_protc_thd_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1, val, OCP_PD_THD, OCP_PD_THD_MASK);
}

LOCAL uint32 _sprd_code_ap_ocp_protc_thd_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1)&OCP_PD_THD_MASK)>>OCP_PD_THD;
}

LOCAL void _sprd_code_ap_ocp_precis_set(uint32 val)
{
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1, val, OCP_PRECIS, OCP_PRECIS_MASK);
}

LOCAL uint32 _sprd_code_ap_ocp_precis_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_prt_cfg_1)&OCP_PRECIS_MASK)>>OCP_PRECIS;
}

/// AUD_CFGA_RD_STS 0x28
LOCAL uint32 _sprd_code_ap_aud_irq_raw_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_rd_sts)&AUDIO_IRQ_RAW_MASK)>> AUDIO_IRQ_RAW;
}

LOCAL uint32 _sprd_code_ap_aud_irq_get(void)
{
    return SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_rd_sts)&AUDIO_IRQ_MASK;
}

/// AUD_CFGA_INT_MODULE_CTRL 0x2C
LOCAL __inline void _sprd_code_ap_aud_irq_en(uint32 en, uint32 ctrl_bits)
{
    uint32 int_module_ctrl = SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->aud_cfga_int_module_ctrl)&AUDIO_IRQ_EN_MASK;
    if(en)
        SPRD_CODEC_AP_REG_SET(&aud_cfga_ctrl_ptr->aud_cfga_int_module_ctrl, int_module_ctrl | ctrl_bits);
    else
        SPRD_CODEC_AP_REG_SET(&aud_cfga_ctrl_ptr->aud_cfga_int_module_ctrl, int_module_ctrl & ~ctrl_bits);
}

/// AUD_CFGA_LP_MODULE_CTRL 0x30
LOCAL __inline void _sprd_code_ap_aud_loop_map_sel(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_lp_module_ctrl, AUDIO_LOOP_MAP_SEL);
}

// loop DAC to ADC
LOCAL __inline void _sprd_code_ap_aud_adie_loop_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_cfga_ctrl_ptr->aud_cfga_lp_module_ctrl, AUDIO_ADIE_LOOP_EN);
}

LOCAL void _sprd_codec_cfga_adc_en(uint32 en, uint32 chan)
{
    _sprd_codec_ap_chan_en(en, chan, &aud_cfga_ctrl_ptr->aud_cfga_lp_module_ctrl,
            ADC_EN_L, ADC_EN_R);
}

LOCAL void _sprd_codec_cfga_dac_en(uint32 en, uint32 chan)
{
    _sprd_codec_ap_chan_en(en, chan, &aud_cfga_ctrl_ptr->aud_cfga_lp_module_ctrl,
            DAC_EN_L, DAC_EN_R);
}


/// ANA_ET2 0x34
LOCAL __inline void _sprd_code_ap_aud_adc_lr_mix_sel(uint32 mix_mode)
{
    SPRD_CODEC_AP_REG_SET(&aud_cfga_ctrl_ptr->ana_et2, (mix_mode << AUD_DALR_MIX_SEL)&AUD_DALR_MIX_SEL_MASK);
}

LOCAL __inline void _sprd_code_ap_aud_ads_lr_mix_sel(uint32 mix_mode)
{
    SPRD_CODEC_AP_REG_SET(&aud_cfga_ctrl_ptr->ana_et2, (mix_mode << AUD_DAS_MIX_SEL)&AUD_DAS_MIX_SEL_MASK);
}

/// CLK_EN 0x38
LOCAL void _sprd_codec_ap_aud_clk_en(uint32 en, uint32 ctrl_bits)
{
    uint32 clk_ctrl_reg = SPRD_CODEC_AP_REG_GET(&aud_cfga_ctrl_ptr->clk_en) & CLK_AUD_MASK;
    if(en)
        SPRD_CODEC_AP_REG_SET(&aud_cfga_ctrl_ptr->clk_en, clk_ctrl_reg | ctrl_bits);
    else
        SPRD_CODEC_AP_REG_SET(&aud_cfga_ctrl_ptr->clk_en, clk_ctrl_reg & ~ctrl_bits);
}

/// SOFT_RST 0x3C
LOCAL __inline void _sprd_code_ap_aud_soft_rst(uint32 clr_bits)
{
    SPRD_CODEC_AP_REG_SET(&aud_cfga_ctrl_ptr->soft_rst, clr_bits);
}

PUBLIC void __sprd_codec_aud_cfga_clk_en(uint32 en)
{
    _sprd_codec_ap_aud_clk_en(en, CLK_AUD_HBD  | CLK_AUD_HID |
            CLK_AUD_1K   | CLK_AUD_IMPD |
            CLK_AUD_32K);
}

LOCAL uint32 _sprd_codec_ap_dc_calibration_is_done()
{
    return ((AUD_DCCAL_STS|AUD_DCCAL_INSERT)==(SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_sts4)&(AUD_DCCAL_STS|AUD_DCCAL_INSERT)))? 1:0;
}

LOCAL uint32 _sprd_codec_ap_depop_chg_is_done()
{
    return ((DEPOP_CHG_STS|DEPOP_CHG_INSERT)==(SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_sts4)&(DEPOP_CHG_STS|DEPOP_CHG_INSERT)))? 1:0;
}

LOCAL void _sprd_codec_ap_dc_calibration()
{
    if (!_sprd_codec_ap_dc_calibration_is_done()) {
        uint32 tmp;
        _sprd_codec_ap_digi_reset_en(0);
        _sprd_codec_ap_drv_soft_start_en(1);
        _sprd_codec_ap_dpop_auto_reset(0);
        _sprd_codec_ap_save_regs();
        _sprd_codec_load_calibration_table();
        _sprd_codec_ap_vcm_buf_en(1);
        SCI_Sleep(10);
        _sprd_codec_dc_calibration_start();
        do{
            SCI_Sleep(10);
            tmp = SPRD_CODEC_AP_REG_GET(&(aud_codec_ctrl_ptr->ana_sts4));
        } while(!(tmp & AUD_DCCAL_STS) || !(tmp & AUD_DCCAL_INSERT));

        _sprd_codec_ap_restore_regs();
        SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dc_calibration done.");
    }
}

LOCAL void _sprd_codec_ap_depop_charge()
{
    if (_sprd_codec_ap_dc_calibration_is_done()) {
        if (!_sprd_codec_ap_depop_chg_is_done()) {
            uint32 t = 0;
            uint8  switch_power = SCI_TRUE;

            // If ADC is working, don't switch off power.
            if (SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc0) & AD_EN)
                switch_power = SCI_FALSE;

            if (switch_power) {
                // power down
                __sprd_codec_ap_vb_sleep_mode_en(1);
                __sprd_codec_ap_bg_en(0);
                _sprd_codec_ap_bg_ibias_en(0);
            }

            _sprd_codec_ap_depop_charge_start(0);
            _sprd_codec_ap_depop_charge_en(1);
            SCI_Sleep(10);
            _sprd_codec_ap_hp_plug_in(1);
            SCI_Sleep(100);
            _sprd_codec_ap_depop_charge_start(1);
            do {
                t = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_sts4);
            } while(!((DEPOP_CHG_STS & t) && (DEPOP_CHG_INSERT & t)));
            SPRD_CODEC_AP_PRINT("__sprd_codec_ap_depop_charge done.");

            if (switch_power) {
                // power up
                __sprd_codec_ap_vb_sleep_mode_en(0);
                __sprd_codec_ap_bg_en(1);
                _sprd_codec_ap_bg_ibias_en(1);
            }
        }
    } else {
        SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dc_calibration is not done yet!");
    }
}

/////////////////////////////////////////////////////////////////////

// obsolete API
PUBLIC void __sprd_codec_ap_clear(uint32 path) {}
PUBLIC void __sprd_codec_ap_bg_ibais_en(uint32 en) {}
PUBLIC void __sprd_codec_ap_lin_adc(uint32 en, uint32 chan) {}
PUBLIC void __sprd_codec_ap_mic_bias_ldo_set(uint32 en) {}
PUBLIC void __sprd_codec_ap_mic_bias_ldo_v_sel(uint32 v_sel) {}
PUBLIC void __sprd_codec_ap_pa_d_en(uint32 en) {}
PUBLIC void __sprd_codec_ap_pa_ldo_en(uint32 en) {}
PUBLIC void __sprd_codec_ap_pa_ldo_v_sel(uint32 v_sel) {}
PUBLIC void __sprd_codec_ap_aux_mic(uint32 en, uint32 chan) {}
PUBLIC void __sprd_codec_ap_vbo_en(uint32 en) { }
PUBLIC void __sprd_codec_ap_vcm_buf_en(uint32 en) { }
PUBLIC void __sprd_codec_ap_vcm_en(uint32 en) { }
PUBLIC void __sprd_codec_ap_lin(uint32 en, uint32 chan) { }
PUBLIC void __sprd_codec_ap_hp_mixer_adcbuf(uint32 en, uint32 chan) { }
PUBLIC void __sprd_codec_ap_pa_demi_en(uint32 en) { }
PUBLIC void __sprd_codec_ap_linein_rec(uint32 en, uint32 chan) { }
PUBLIC void __sprd_codec_ap_pa_dtri_f_sel(uint32 freq) {}
PUBLIC void __sprd_codec_ap_lout_mixer_adcbuf(uint32 en, uint32 chan) { }

PUBLIC uint32 __sprd_codec_update_pa_ldo_vol(uint32 cur_vol) { return 0; }
PUBLIC uint32 __sprd_codec_update_vb_vbo_vcm_vol(uint32 cur_vol) { return 0; }

LOCAL void _sprd_codec_headmic_detect_en(uint32 en) {
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_hdt0, V2AD_EN);
}
LOCAL void _sprd_codec_headset_button_detect_en(uint32 en) {
    _sprd_codec_ap_en_op(!en, &aud_codec_ctrl_ptr->ana_hdt0, BUT_DET_PD);
}

LOCAL void _sprd_codec_set_aux_adc_scale(uint32 is_big_scale) {
    _sprd_codec_ap_en_op(is_big_scale, &aud_codec_ctrl_ptr->ana_hdt0, AUXADC_SCALE_SEL);
}

PUBLIC void __sprd_codec_set_headset_connection_status(uint32 status)
{
    if (status) {
        _sprd_codec_ap_en_op(1, ANA_MODULE_EN0, ANA_AUD_EN);
        __sprd_codec_ap_vb_en(1);
    }else{
        //pull out close detect circuit regardless 3_pole 4_pole
        _sprd_codec_headmic_detect_en(0);
        _sprd_codec_headset_button_detect_en(0);
        //hp mic clean up
        _sprd_codec_ap_main_mic(0, SPRD_CODEC_MAIN_MIC_HP_OCCUPIED);
        _sprd_codec_ap_headmic_to_adc_pga_en(0);
        //#Bug671680: reset adc after headset plug out
        _sprd_codec_ap_adc_reset();
    }
    g_HeadPhoneStatus.Connected = status;
    g_HeadPhoneStatus.MicType = UNKNOWN_MIC_TYPE;
}

LOCAL void _sprd_codec_headmic_type_detect()
{
#define CHECK_TIME (5)
#define EFUSE_headmicADC_factor  	327
    uint32 i = 0;
    uint32 average = 0;
    uint32 total = 0;
    uint32 max = 0;
    uint32 min = 0xffffffff;
    uint32 Multiply = 0;
    uint32 val = 0;
    uint8 DetectCnt = 0;
    uint32  headmicADC_efuse[2] = {0};
    uint32 Efuse_average = 0;
    uint32 headmicADC_average = 0;
    uint32 headmicEfuse_subtraction = 0;
    BOOLEAN ret = SCI_FALSE;
 
    if(UNKNOWN_MIC_TYPE == g_HeadPhoneStatus.MicType){
        _sprd_codec_headmic_detect_en(1);

        // small scale
        _sprd_codec_set_aux_adc_scale(0);
        while(Multiply<10&&DetectCnt<5){
            DetectCnt++;
            total = 0;
            max = 0;
            min = 0xFFFFFFFF;
            for (i = 0; i<CHECK_TIME; i++) {
                val = ADC_GetResultDirectly(6, ADC_SCALE_1V250);
                total +=  val;
                if(val > max)
                    max = val;
                if(val < min)
                    min = val;
            }
            if(max!=min){
                max <<=10;
                min <<=10;
                Multiply = min/(max-min);
            }
            else
                Multiply = 0xFFFFFFFF;
        }
        if(DetectCnt <= 5){
            average = total/CHECK_TIME;
            ret = EFUSE_HAL_GetADCHeadmicCali(headmicADC_efuse);
            if(ret)
            {
                headmicEfuse_subtraction = headmicADC_efuse[0] - headmicADC_efuse[1];
                Efuse_average = EFUSE_headmicADC_factor * (9*average- 10*headmicADC_efuse[1]+ headmicADC_efuse[0])/headmicEfuse_subtraction;
                headmicADC_average = Efuse_average;
                SCI_TRACE_LOW("HeadMic pole detect:headmicADC_efuse[0] = %d, headmicADC_efuse[1] = %d ,Efuse_average = %d ",headmicADC_efuse[0],headmicADC_efuse[1],Efuse_average);
            }
            else
                headmicADC_average = average;
            if(headmicADC_average>HEADMIC_ADC_0_9V)
                g_HeadPhoneStatus.MicType = _4_POLE_MIC_TYPE;
            else if(headmicADC_average<HEADMIC_ADC_0_1V)
                g_HeadPhoneStatus.MicType = _3_POLE_MIC_TYPE;
            else
                g_HeadPhoneStatus.MicType = _3_POLE_MIC_TYPE;
            SPRD_CODEC_AP_PRINT("HeadMic pole detect:SMALL ave=%d max*1024=%d min*1024=%d Multiply=%u MicType=%d", average, max, min, Multiply, g_HeadPhoneStatus.MicType);
        }else{
            g_HeadPhoneStatus.MicType = _3_POLE_MIC_TYPE;
            SPRD_CODEC_AP_PRINT("HeadMic pole detect:Variance is too large.Small scale detect faild.ave=%d max*1024=%d min*1024=%d Multiply=%u MicType=%d", average, max, min, Multiply, g_HeadPhoneStatus.MicType);
        }

        //if 3_POLE Close HP Mic detect and bias.
        if(_3_POLE_MIC_TYPE == g_HeadPhoneStatus.MicType){
            _sprd_codec_headmic_detect_en(0);
            _sprd_codec_headset_button_detect_en(0);
        }else{
            _sprd_codec_headset_button_detect_en(1);
        }
    }
}

PUBLIC void __sprd_codec_ap_bg_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_pmu0, BG_EN);
}

PUBLIC void __sprd_codec_ap_digi_ctrl_en(uint32 en)
{
    _sprd_codec_ap_digi_ctrl_en(en);
}

PUBLIC void __sprd_codec_ap_en(uint32 en)
{
    SPRD_CODEC_AP_PRINT("__sprd_codec1161_ap_en %d", en);

    if (en) {
        // global settings for codec
        // check "sc6531efm_audio_cfg.h" for clock related settings
        _sprd_codec_ap_en_op(1, ANA_MODULE_EN0, ANA_AUD_EN);

#ifdef FPGA_VERIFICATION
        // set pin regs
        SPRD_CODEC_AP_REG_SET(ANA_PIN_AUD_ADSYNC_REG, 0x301);
        SPRD_CODEC_AP_REG_SET(ANA_PIN_AUD_ADD0_REG,   0x301);
        SPRD_CODEC_AP_REG_SET(ANA_PIN_AUD_SCLK_REG,   0x302);
#endif

        __sprd_codec_ap_vb_en(1);
        //Audio LDO VB SLEEP MODE PD signal 0 = PD, 1 = EN
        __sprd_codec_ap_vb_sleep_mode_en(0);
        //Audio Bandgap output voltage select signal 0 = 1.6V output, 1 = 1.5V output
        __sprd_codec_ap_vcmi_sel(1);
        _sprd_codec_ap_clk_en(1);

        _sprd_codec_ap_dc_calibration();

        // a-die audio digital clock
        __sprd_codec_aud_cfga_clk_en(1);
        __sprd_codec_ap_digi_ctrl_en(1);
        __sprd_codec_ap_bg_en(1);
        _sprd_codec_ap_bg_ibias_en(1);
        if(g_HeadPhoneStatus.Connected)
            _sprd_codec_headmic_type_detect();
        __sprd_codec_ap_vbo_set(VB_LDO_V_30);
    }
    else
    {
        // Enable LDO VB SLEEP MODE
        __sprd_codec_ap_vb_sleep_mode_en(1);

        __sprd_codec_ap_bg_en(0);
        _sprd_codec_ap_bg_ibias_en(0);
        _sprd_codec_ap_clk_en(0);

        __sprd_codec_ap_digi_ctrl_en(0);
        __sprd_codec_aud_cfga_clk_en(0);

        // The last step: disable codec
        _sprd_codec_ap_en_op(0, ANA_MODULE_EN0, ANA_AUD_EN);
    }
}

PUBLIC void __sprd_codec_ap_drv_clk_en(uint32 en)
{
    _sprd_codec_ap_drv_clk_en(en);
}

////// DAC related //////
PUBLIC void __sprd_codec_ap_dac(uint32 en, uint32 chan)
{
    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dac %d", en);
    _sprd_codec_ap_dac_dc_offset_en(en);

    _sprd_codec_cfga_dac_en(en, chan);

    __sprd_codec_ap_dac_dc_trim_select(2);
    __sprd_codec_ap_dac_en(en, chan);
}

PUBLIC void __sprd_codec_ap_dac_clk_en(uint32 en)
{
    _sprd_codec_ap_dac_clk_en(en);
}

PUBLIC void __sprd_codec_ap_dac_en(uint32 en, uint32 chan)
{
    _sprd_codec_ap_chan_en(en, chan, &aud_codec_ctrl_ptr->ana_cdc2,
            DAL_EN, DAR_EN);
    _sprd_codec_ap_dac_dc_offset_en(en);
}

////// receiver related
PUBLIC void __sprd_codec_ap_ear(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc2, RCV_EN);
    if (en || (!en && !g_HeadPhoneStatus.Connected))
        _sprd_codec_ap_vcm_buf_en(en);
    _sprd_codec_ap_receiver_dummy_loop_end_en(en);
    _sprd_codec_ap_ear_stereo(en);
    __sprd_codec_ap_ear_mixer(en);
}

PUBLIC void __sprd_codec_ap_ear_pga_gain(uint32 val)
{
    _sprd_codec_ap_set_receiver_gain(val);
}

PUBLIC void __sprd_codec_ap_ear_pga_gain_set(uint32 val)
{
    _sprd_codec_ap_set_receiver_gain(val);
}

PUBLIC void __sprd_codec_ap_ear_mute(void)
{
    _sprd_codec_ap_set_receiver_gain(0xF);
}

PUBLIC uint32 __sprd_codec_ap_ear_pga_gain_get(void)
{
    return (SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc4)
            & RCV_G_MASK) >> RCV_G;
}

PUBLIC void __sprd_codec_ap_ear_mixer(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, SDALRCV);
}

////// headphone related
PUBLIC void __sprd_codec_ap_hp_pop_set(uint32 en)
{
#if 0
    if (en) {
        _sprd_codec_ap_dc_calibration_wait_time(2);

        _sprd_codec_ap_hpl_depop_clk_n1(3);
        _sprd_codec_ap_hpr_depop_clk_n1(3);

        _sprd_codec_ap_hpl_depop_n1(0);
        _sprd_codec_ap_hpr_depop_n1(0);

        _sprd_codec_ap_hpl_depop_val1(6);
        _sprd_codec_ap_hpr_depop_val1(6);

        _sprd_codec_ap_hpl_depop_clkn2(3);
        _sprd_codec_ap_hpr_depop_clkn2(3);

        _sprd_codec_ap_hpl_depop_n2(0);
        _sprd_codec_ap_hpr_depop_n2(0);

        _sprd_codec_ap_depop_wait_t1(1);
        _sprd_codec_ap_depop_wait_t2(2);
        _sprd_codec_ap_depop_wait_t3(0);
        _sprd_codec_ap_depop_wait_t4(0);
    } else {
        _sprd_codec_ap_dc_calibration_wait_time(0);

        _sprd_codec_ap_hpl_depop_clk_n1(0);
        _sprd_codec_ap_hpr_depop_clk_n1(0);

        _sprd_codec_ap_hpl_depop_clkn2(0);
        _sprd_codec_ap_hpr_depop_clkn2(0);

        _sprd_codec_ap_depop_wait_t1(4);
        _sprd_codec_ap_depop_wait_t2(4);
        _sprd_codec_ap_depop_wait_t3(2);
        _sprd_codec_ap_depop_wait_t4(2);
    }
#endif
    _sprd_codec_ap_depopl_pcur_opt(3);
    _sprd_codec_ap_depopr_pcur_opt(3);

    _sprd_codec_ap_dc_calibration_wait_time(2);

    _sprd_codec_ap_hpl_depop_clk_n1(3);
    _sprd_codec_ap_hpr_depop_clk_n1(3);

    _sprd_codec_ap_hpl_depop_n1(0);
    _sprd_codec_ap_hpr_depop_n1(0);

    _sprd_codec_ap_hpl_depop_val1(6);
    _sprd_codec_ap_hpr_depop_val1(6);

    _sprd_codec_ap_hpl_depop_clkn2(3);
    _sprd_codec_ap_hpr_depop_clkn2(3);

    _sprd_codec_ap_hpl_depop_n2(0);
    _sprd_codec_ap_hpr_depop_n2(0);

    _sprd_codec_ap_depop_wait_t1(1);
    _sprd_codec_ap_depop_wait_t2(6);
    _sprd_codec_ap_depop_wait_t3(0);
    _sprd_codec_ap_depop_wait_t4(0);
}

PUBLIC void __sprd_codec_ap_hp(uint32 en, uint32 chan)
{
    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_hp %d", en);
    if (en) {
        __sprd_codec_ap_hp_pop_set(en);

        // depop charge
        _sprd_codec_ap_depop_charge();

        _sprd_codec_ap_vcm_buf_en(en);

        // HP depop required
        __sprd_codec_ap_dac_en(0, AUDIO_ALL_CHAN);

        _sprd_codec_ap_en_op(0, &aud_codec_ctrl_ptr->ana_cdc2, HPL_EN);
        _sprd_codec_ap_en_op(0, &aud_codec_ctrl_ptr->ana_cdc2, HPR_EN);
        // enable dummy loop
        _sprd_codec_ap_hpl_dummy_loop_en(1);
        _sprd_codec_ap_hpr_dummy_loop_en(1);

        _sprd_codec_ap_en_op(1, &aud_codec_ctrl_ptr->ana_cdc2, HPL_EN);
        _sprd_codec_ap_en_op(1, &aud_codec_ctrl_ptr->ana_cdc2, HPR_EN);

        SCI_Sleep(10);
        // disable dummy loop
        _sprd_codec_ap_hpl_dummy_loop_en(0);
        _sprd_codec_ap_hpr_dummy_loop_en(0);

        __sprd_codec_ap_dac_en(1, AUDIO_ALL_CHAN);

        //_sprd_codec_ap_dac_dc_offset_en(en);
    } else {
        //_sprd_codec_ap_dac_dc_offset_en(en);
        //__sprd_codec_ap_dac_en(en, AUDIO_ALL_CHAN);

        // Don't close hp buf, otherwise there will be pop noise
        // _sprd_codec_ap_vcm_buf_en(en);

        _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc2, HPL_EN);
        _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc2, HPR_EN);

        _sprd_codec_ap_hplr_dummy_loop_dis_all();
    }
}

PUBLIC void __sprd_codec_ap_hp_mixer(uint32 en, uint32 chan)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, SDALHPL);
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc3, SDARHPR);
}

// TODO: value to db mapping
PUBLIC void __sprd_codec_ap_hp_pga_gain(uint32 chan, uint32 val)
{
    if (AUDIO_IS_CHAN_L(chan))
        _sprd_codec_ap_set_headphone_l_gain(val);

    if (AUDIO_IS_CHAN_R(chan))
        _sprd_codec_ap_set_headphone_r_gain(val);
}

PUBLIC void __sprd_codec_ap_hp_pga_gain_set(uint32 chan, uint32 val)
{
    __sprd_codec_ap_hp_pga_gain(chan, val);
}

// TODO: value to db mapping
PUBLIC uint32 __sprd_codec_ap_hp_pga_gain_get(uint32 chan)
{
    if (AUDIO_IS_CHAN_L(chan)) {
        return (SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc4)
                & HPL_G_MASK) >> HPL_G;
    } else if (AUDIO_IS_CHAN_R(chan)) {
        return (SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_cdc4)
                & HPR_G_MASK) >> HPR_G;
    }
    return 0;
}

PUBLIC void __sprd_codec_ap_hp_mute(void)
{
    __sprd_codec_ap_hp_pga_gain(AUDIO_ALL_CHAN, 0xF);
}

////// speaker
PUBLIC void __sprd_codec_ap_pa_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc2, DAS_EN);
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_cdc2, PA_EN);
}

PUBLIC void __sprd_codec_ap_lout(uint32 en)
{
    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_lout %d", en);
    __sprd_codec_ap_pa_en(en);
}

PUBLIC void __sprd_codec_ap_lout_mixer(uint32 en, uint32 chan)
{
    if (en) {
        _sprd_codec_ap_dacs_dc_offset_en(1);
        _sprd_codec_ap_dacs_to_pa(1);
    } else {
        _sprd_codec_ap_dacs_to_pa(0);
        _sprd_codec_ap_dacs_dc_offset_en(0);
    }
}

PUBLIC void __sprd_codec_ap_pa_pga_gain(uint32 val)
{
    _sprd_codec_ap_set_speaker_gain(val);
}

PUBLIC void __sprd_codec_ap_pa_pga_gain_set(uint32 chan, uint32 val)
{
    _sprd_codec_ap_set_speaker_gain(val);
}

PUBLIC uint32 __sprd_codec_ap_pa_pga_gain_get(uint32 chan)
{
    return 0;
}

PUBLIC void __sprd_codec_ap_pa_mute(void)
{
    _sprd_codec_ap_set_speaker_gain(0xF);
}

////// ADC related //////

PUBLIC void __sprd_codec_ap_adc(uint32 en, uint32 chan)
{
    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_adc %d", en);

    _sprd_codec_cfga_adc_en(en, chan);

    _sprd_codec_ap_adc_clk_en(en);

    // PGA related setting
    _sprd_codec_ap_adc_pga_bias_en(en);
    _sprd_codec_ap_adc_pga_en(en);
    _sprd_codec_ap_adc_ref_select(en ? 3 : 0);

    __sprd_codec_ap_adc_en(en, chan);
}

PUBLIC void __sprd_codec_ap_adc_clk_en(uint32 en)
{
    _sprd_codec_ap_adc_clk_en(en);
}

PUBLIC void __sprd_codec_ap_adc_pga_gain(uint32 chan, uint32 val)
{
    // Only use the setting for left channel
    if (AUDIO_IS_CHAN_L(chan))
        _sprd_codec_ap_adc_pga_gain(val);
}

PUBLIC void __sprd_codec_ap_adc_pga_gain_set(uint32 chan, uint32 val)
{
    _sprd_codec_ap_adc_pga_gain(val);
}

PUBLIC void __sprd_codec_ap_adc_mute(void)
{
    // TODO: how to mute adc???
}

////// hp mic related
PUBLIC void __sprd_codec_ap_hp_mic(uint32 en, uint32 chan)
{
    if (!g_HeadPhoneStatus.Connected)
		return;

    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_hp_mic %d,MicType =%d", en, g_HeadPhoneStatus.MicType);

    if (UNKNOWN_MIC_TYPE == g_HeadPhoneStatus.MicType)
        _sprd_codec_headmic_type_detect();

    // #Bug671680: reset adc before open head mic
    if (en)
        _sprd_codec_ap_adc_reset();

    if (_4_POLE_MIC_TYPE == g_HeadPhoneStatus.MicType)
        _sprd_codec_ap_headmic_to_adc_pga_en(en);
    else
        _sprd_codec_ap_main_mic(en, SPRD_CODEC_MAIN_MIC_HP_OCCUPIED);
}

////// main mic related
PUBLIC void __sprd_codec_ap_main_mic(uint32 en, uint32 chan)
{
    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_main_mic %d", en);
//bug847354 拔电池之后重新开机第一次免提无声
    if (en)
        _sprd_codec_ap_adc_reset();
    _sprd_codec_ap_main_mic(en, SPRD_CODEC_MAIN_MIC_MIC_OCCUPIED);
}

////// interrupt related
PUBLIC void __sprd_codec_ap_int_clr(uint32 mask)
{
    //
}

PUBLIC uint32 __sprd_codec_ap_int_mask(void)
{
    //
    return 1;
}

PUBLIC void __sprd_codec_ap_int_set(uint32 en, uint32 mask)
{
    //
}

////// hp depop related
PUBLIC void __sprd_codec_ap_clr_ramp(uint32 up)
{
    //TODO: hp pop
}

PUBLIC uint32 __sprd_codec_ap_hp_ramp_done(uint32 up)
{
    return 1;
}

PUBLIC void __sprd_codec_ap_hp_pop(uint32 ctl)
{
    //TODO: hp pop
}

//TODO: hp pop
PUBLIC uint32 __sprd_codec_ap_hp_pop_en(uint32 en)
{
    return 1;
}

//////////////////////////////////////////////////////////////////////////////

typedef struct
{
    SPRD_CODEC_PGA_T        pga_setting_0;
    SPRD_CODEC_PGA_T        pga_setting_1;
    SPRD_CODEC_PGA_T        is_mute;
}SPRD_CODEC_PGA_SETTING_T;

LOCAL SPRD_CODEC_PGA_SETTING_T s_sprd_codec_pga_setting;

PUBLIC SPRD_CODEC_PGA_T* __sprd_codec_ap_pga_setting(uint32 chan)
{
    if (AUDIO_IS_CHAN_L(chan)) {
        return &(s_sprd_codec_pga_setting.pga_setting_0);
    }

    if (AUDIO_IS_CHAN_R(chan)) {
        return &(s_sprd_codec_pga_setting.pga_setting_1);
    }

    return &(s_sprd_codec_pga_setting.pga_setting_0);
}

PUBLIC uint32 __sprd_codec_ap_adc_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.adcpga;
}

PUBLIC uint32 __sprd_codec_ap_hp_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.hp;
}

PUBLIC uint32 __sprd_codec_ap_ep_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.ear;
}

PUBLIC uint32 __sprd_codec_ap_pa_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.pa;
}

PUBLIC void __sprd_codec_ap_adc_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.adcpga = 0;
}

PUBLIC void __sprd_codec_ap_hp_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.hp = 0;
}

PUBLIC void __sprd_codec_ap_ep_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.ear = 0;
}

PUBLIC void __sprd_codec_ap_pa_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.pa = 0;
}

PUBLIC uint32 __sprd_codec_ap_adc_pga_gain_get(uint32 chan)
{
    return 0;
}

////////////////////////////////////////////////////////////////////

PUBLIC void __sprd_codec_ap_reg_print(void)
{
    uint32 reg = SC1161_AP_REG_BASE;
    uint32 offset = 0;

    for(; offset < 0x60; offset += 0x10) {
        SPRD_CODEC_AP_PRINT("sc1161:codec ana  0x%04x | 0x%04x 0x%04x 0x%04x 0x%04x\n", offset,
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x0)),
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x4)),
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x8)),
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0xc)));
    }

    for(reg = SC1161_AUD_CFGA_REG_BASE, offset = 0; offset < 0x40; offset += 0x10) {
        SPRD_CODEC_AP_PRINT("sc1161:codec cfga 0x%04x | 0x%04x 0x%04x 0x%04x 0x%04x\n", offset,
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x0)),
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x4)),
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x8)),
                SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0xc)));
    }

    reg = 0x82001400;
    offset=0;
    SPRD_CODEC_AP_PRINT("sc1161:global 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x\n",
            SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x0)),
            SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x4)),
            SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x8)),
            SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0xc)),
            SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x134)));
}

PUBLIC uint32 _sprd_codec_ap_headmic_adcget(void)
{
    uint32 adc_Value = 0;
    uint32 i = 0;
    
    //AUDIO ADC ENABLE 
    ANA_REG_OR(ANA_MODULE_EN0,ANA_AUD_EN|ANA_ADC_EN);
    
     /* Enable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_AdcEnable]"
    ANA_REG_OR (ANA_ARM_CLK_EN0,  (ANA_CLK_AUXADC_EN | ANA_CLK_AUXAD_EN));// | ANA_CLK_AUD_SCLK_EN |ANA_CLK_AUD_IF_EN|ANA_CLK_AUD_IF_6P5M_EN));

    // ANA_PMU 
     __sprd_codec_ap_vb_en(1);
     _sprd_codec_ap_bg_ibias_en(1);
    //NAN_HDT0   V2AD_EN and small scale
     _sprd_codec_headmic_detect_en(1);
    _sprd_codec_set_aux_adc_scale(0);
    
    for (i = 0 ; i < 16 ; i++)
    {
       adc_Value += ADC_GetResultDirectly ( 6, ADC_SCALE_1V250);
    }
    adc_Value >>= 4;
    
    SCI_TRACE_LOW("adc headmic channel adc average= %d",adc_Value);
    
   return adc_Value;
    
}


