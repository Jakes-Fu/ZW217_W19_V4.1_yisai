/******************************************************************************
 ** File Name:      usc15241.h                                                *
 ** Author:         ken.kuang                                                 *
 ** DATE:           09/08/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    usc15241 control                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/08/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifndef __USC15241_H___
#define __USC15241_H___
#include "Chip_plf_export.h"
#include "ctl_bus/usc15241_spi_drv.h"
#include "ctl_bus/usc15241_i2c_drv.h"
#include "audio_hal.h"

PUBLIC void _usc15241_reg_w(uint32 bus,uint32 reg,uint32 val);
PUBLIC uint32 _usc15241_reg_r(uint32 bus,uint32 reg);
PUBLIC void test_usc15241_reg(void);

enum{
    USE_NOTHING=0,
    USE_I2C,
    USE_SPI
};


#define USC15241_REG_GET_BUS(_Bus,_Reg)  _usc15241_reg_r(_Bus,_Reg)

#define USC15241_REG_SET_BUS(_Bus,_Reg, _Value, _Mask)        \
    DRV_MACRO_START                                         \
        uint32  val = USC15241_REG_GET_BUS(_Bus,_Reg);  \
        val &= ~(_Mask);                           \
        val |= (_Value & _Mask);                   \
        _usc15241_reg_w(_Bus,_Reg,val);             \
    DRV_MACRO_END


LOCAL uint16 _usc15241_reg_split(uint32 _Reg,uint16 * p_bus)
{
    uint16 bus;
    uint16 reg;
    bus = (_Reg >> 16) & 0xffff;
    reg = (_Reg) & 0xffff;
    if( p_bus )
    {
        *p_bus = bus;
    }
    return reg;
}

LOCAL uint32 _usc15241_reg_r_mix(uint32 _Reg)
{
    uint16 bus;
    uint16 reg;
    reg = _usc15241_reg_split(_Reg,&bus);
    return USC15241_REG_GET_BUS(bus,reg);
}

LOCAL uint32 _usc15241_reg_w_mix(uint32 _Reg,uint32 _Value,uint32 _Mask)
{
    uint16 bus; 
    uint16 reg; 
    reg = _usc15241_reg_split(_Reg,&bus);
    USC15241_REG_SET_BUS(bus,reg,_Value, _Mask);
}


#define USC15241_REG_GET  _usc15241_reg_r_mix
#define USC15241_REG_SET _usc15241_reg_w_mix


#define REG_BUS_SEL(BUS,REG)    (((BUS<<16)&0XFFFF0000) | ((REG)&0XFFFF))
#define REG_I2C(REG) REG_BUS_SEL(USE_I2C,REG)
#define REG_SPI(REG) REG_BUS_SEL(USE_SPI,REG)


#define POWER_SET                   (REG_I2C(0x05))
#define POWER_SET_DRV_LDO_PD        (BIT_0)
#define POWER_SET_LDO_V_SHIFT       (1)
#define POWER_SET_LDO_V_MASK        (0x3<<POWER_SET_LDO_V_SHIFT)
#define POWER_SET_LDO_V_30          (0)
#define POWER_SET_LDO_V_32          (1)
#define POWER_SET_LDO_V_33          (2)
#define POWER_SET_LDO_V_34          (3)
#define POWER_SET_LDO_PD            (BIT_3)
#define POWER_SET_BG_V_SHIFT        (4)
#define POWER_SET_BG_V_MASK         (0x7<<POWER_SET_BG_V_SHIFT)
#define POWER_SET_BG_V_1200         (0)
#define POWER_SET_BG_V_1231         (1)
#define POWER_SET_BG_V_1262         (2)
#define POWER_SET_BG_V_1293         (3)
#define POWER_SET_BG_V_1076         (4)
#define POWER_SET_BG_V_1107         (5)
#define POWER_SET_BG_V_1138         (6)
#define POWER_SET_BG_V_1169         (7)
#define POWER_SET_BG_PD             (BIT_7)

#define POWER_SET1                  (REG_I2C(0x06))
#define POWER_SET1_BG2_PAD_EN       (BIT_0)
#define POWER_SET1_BG2_I_SHIFT      (1)
#define POWER_SET1_BG2_I_MASK       (0x3<<POWER_SET1_BG2_I_SHIFT)
#define POWER_SET1_BG2_I_150        (0)
#define POWER_SET1_BG2_I_200        (1)
#define POWER_SET1_BG2_I_250        (2)
#define POWER_SET1_BG2_I_300        (3)
#define POWER_SET1_BG2_RST          (BIT_4)
#define POWER_SET1_BG2_EN           (BIT_6)
#define POWER_SET1_OCP_PD           (BIT_7)


#define POWER_SET2                  (REG_I2C(0x07))
#define POWER_SET2_BG_PAD_EN        (BIT_3)
#define POWER_SET2_MICBIAS_PD       (BIT_4)
#define POWER_SET2_MICBIAS_V_SHIFT  (5)
#define POWER_SET2_MICBIAS_V_MASK   (0x3<<POWER_SET2_MICBIAS_V_SHIFT)
#define POWER_SET2_MICBIAS_V_30     (0)
#define POWER_SET2_MICBIAS_V_32     (1)
#define POWER_SET2_MICBIAS_V_33     (2)
#define POWER_SET2_MICBIAS_V_34     (3)

#define DAC_SET                     (REG_SPI(19))
#define DAC_SET_DR_EN               (BIT_10)
#define DAC_SET_VIN_R_EN            (BIT_11)
#define DAC_SET_ILP_R_EN            (BIT_12)
#define DAC_SET_ILN_R_EN            (BIT_13)
#define DAC_SET_VHP_R_EN            (BIT_14)
#define DAC_SET_VHN_R_EN            (BIT_15)

#define DAC_SET1                    (REG_SPI(20))
#define DAC_SET1_IHP_R_EN           (BIT_0)
#define DAC_SET1_IHN_R_EN           (BIT_1)
#define DAC_SET1_DATA_PAD           (BIT_2)
#define DAC_SET1_SAMP_POS           (BIT_3)
#define DAC_SET1_CLK_OUT_EN         (BIT_4)
#define DAC_SET1_DAC_EN             (BIT_5)
#define DAC_SET1_CLK_DAC2_EN        (BIT_6)
#define DAC_SET1_CLK_DAC1_EN        (BIT_7)
#define DAC_SET1_CLK_PAD            (BIT_8)
#define DAC_SET1_IHN_L_EN           (BIT_9)
#define DAC_SET1_IHP_L_EN           (BIT_10)
#define DAC_SET1_VHN_L_EN           (BIT_11)
#define DAC_SET1_VHP_L_EN           (BIT_12)
#define DAC_SET1_ILN_L_EN           (BIT_13)
#define DAC_SET1_ILP_L_EN           (BIT_14)
#define DAC_SET1_VIN_L_EN           (BIT_15)

#define ADC_CLK_SEL                 (REG_SPI(21))
#define ADC_CLK_SEL_PAD             (BIT_2)

#define ADC_SET                     (REG_SPI(22))
#define ADC_SET_FM2_PGA_BYP         (BIT_11)
#define ADC_SET_MIC2_SEL            (BIT_12)
#define ADC_SET_G2_PGA2_SHIFT       (13)
#define ADC_SET_G2_PGA2_MASK        (0x3<<ADC_SET_G2_PGA2_SHIFT)
#define ADC_SET_G2_PGA2_PD          (BIT_15)

#define ADC_SET1                    (REG_SPI(23))
#define ADC_SET1_G1_PGA2_SHIFT      (0)
#define ADC_SET1_G1_PGA2_MASK       (0x7<<ADC_SET1_G1_PGA2_SHIFT)
#define ADC_SET1_G0_PGA2_SHIFT      (3)
#define ADC_SET1_G0_PGA2_MASK       (0x3<<ADC_SET1_G0_PGA2_SHIFT)
#define ADC_SET1_FM2_PGA_PD         (BIT_5)
#define ADC_SET1_ADC2_RST           (BIT_6)
#define ADC_SET1_ADC2_PD            (BIT_7)
#define ADC_SET1_FMR2_EN            (BIT_8)
#define ADC_SET1_FML2_EN            (BIT_9)
#define ADC_SET1_CLK_PD             (BIT_10)
#define ADC_SET1_CLK_RST            (BIT_11)
#define ADC_SET1_BUF_PD             (BIT_12)
#define ADC_SET1_VCOM_PD            (BIT_13)
#define ADC_SET1_VREFX15_EN         (BIT_14)
#define ADC_SET1_FML1_EN            (BIT_15)

#define ADC_SET2                    (REG_SPI(24))
#define ADC_SET2_FMR1_EN            (BIT_0)
#define ADC_SET2_ADC1_PD            (BIT_1)
#define ADC_SET2_ADC1_RST           (BIT_2)
#define ADC_SET2_FM1_PGA_PD         (BIT_3)
#define ADC_SET2_PGA1_PD            (BIT_4)
#define ADC_SET2_G0_PGA1_SHIFT      (5)
#define ADC_SET2_G0_PGA1_MASK       (0x3<<ADC_SET2_G0_PGA1_SHIFT)
#define ADC_SET2_G1_PGA1_SHIFT      (7)
#define ADC_SET2_G1_PGA1_MASK       (0x7<<ADC_SET2_G1_PGA1_SHIFT)
#define ADC_SET2_G2_PGA1_SHIFT      (10)
#define ADC_SET2_G2_PGA1_MASK       (0x3<<ADC_SET2_G2_PGA1_SHIFT)
#define ADC_SET2_MIC1_SEL           (BIT_12)
#define ADC_SET2_ADC1_TEST          (BIT_13)
#define ADC_SET2_PGA1_BYP           (BIT_14)
#define ADC_SET2_FM1_BYP            (BIT_15)


#define DAC_CLK_SET                 (REG_SPI(25))
#define DAC_CLK_PADOUT_SEL          (BIT_4)
#define DAC_CLK_ADC_EN              (BIT_5)
#define DAC_CLK_DAC_EN              (BIT_6)
#define DAC_CLK_PADIN_EN            (BIT_7)


PUBLIC void     __usc15241_init(void);
PUBLIC void     __usc15241_en(uint32 en);
PUBLIC void     __usc15241_bg_en(uint32 en);
PUBLIC void     __usc15241_adc(uint32 en,uint32 chan);
PUBLIC void     __usc15241_vcm_en(uint32 en);
PUBLIC void     __usc15241_vcm_buf_en(uint32 en);
PUBLIC void     __usc15241_adc_clk_pwr(uint32 en);
PUBLIC void     __usc15241_adc(uint32 en,uint32 chan);
PUBLIC void     __usc15241_adc_pwr(uint32 en,uint32 chan);
PUBLIC void     __usc15241_mic_en(uint32 en,uint32 id);
PUBLIC void     __usc15241_mic_bias(uint32 en);
PUBLIC void     __usc15241_linein_rec(uint32 en,uint32 chan);
PUBLIC void     __usc15241_dac_clk_en(uint32 en);
PUBLIC void     __usc15241_dac_pwr(uint32 en,uint32 chan);
PUBLIC void     __usc15241_dac(uint32 en,uint32 chan);
PUBLIC void     __usc15241_hp_en(uint32 en,uint32 chan);
PUBLIC void     __usc15241_hp_mixer(uint32 en,uint32 chan);
PUBLIC void     __usc15241_hp_mixer_adcbuf(uint32 en,uint32 chan);
PUBLIC void     __usc15241_hp(uint32 en,uint32 chan,uint32 dac,uint32 lin);
PUBLIC void     __usc15241_lout_en(uint32 en,uint32 chan);
PUBLIC void     __usc15241_lout_mixer(uint32 en,uint32 chan);
PUBLIC void     __usc15241_lout_mixer_adcbuf(uint32 en,uint32 chan);
PUBLIC void     __usc15241_lout(uint32 en,uint32 chan,uint32 dac,uint32 lin);
PUBLIC uint32   __usc15241_calc_fs(uint32 fs);
PUBLIC int      __usc15241_reg_print(void);
PUBLIC void     __run_callback(uint32 is_open);
PUBLIC void     __usc15241_ext_pa_ctl(uint32 en);
PUBLIC void     __usc15241_ext_hp_pa_ctl(uint32 en);
PUBLIC uint32   __usc15241_is_arm_ctl(void);

PUBLIC void     __usc15241_create_task(void);


#endif //__USC15241_H___
