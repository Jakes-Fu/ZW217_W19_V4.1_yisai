/******************************************************************************
 ** File Name:      usc15241.c                                                 *
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
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "chip_plf_export.h"
#include "audio_drvapi.h"
#include "sprd_codec_dp_phy.h"
#include "usc15241.h"
#include "audio_cfg.h"

#ifdef CODEC_DEBUG
#define USC15241_PRINT    SCI_TRACE_LOW
#else
#define USC15241_PRINT(...)
#endif

PUBLIC void _usc15241_reg_w(uint32 bus,uint32 reg,uint32 val)
{
    switch(bus)
    {
        case USE_I2C:
        {
            __i2c_reg_w(reg,val);
        }break;
        case USE_SPI:
        {
            __spi_reg_w(reg,val);
        }break;
        default:SCI_ASSERT(0);
    }
}

PUBLIC uint32 _usc15241_reg_r(uint32 bus,uint32 reg)
{
    switch(bus)
    {
        case USE_I2C:
        {
            return __i2c_reg_r(reg);
        }break;/*lint !e527 comfirmed by windy.wang*/
        case USE_SPI:
        {
            return __spi_reg_r(reg);
        }break;/*lint !e527 comfirmed by windy.wang*/
        default:SCI_ASSERT(0);
    }
    return 0xffffffff;/*lint !e527 comfirmed by windy.wang*/
}


LOCAL void _usc15241_en_op(uint32 en,uint32 reg,uint32 bit)
{
    //USC15241_PRINT("_usc15241_op 0x%x 0x%x",reg,bit);
    if( en )
    {
        USC15241_REG_SET(reg,(bit),(bit));
    }
    else
    {
        USC15241_REG_SET(reg,~(bit),(bit));
    }
}

LOCAL void _usc15241_chan_en(uint32 en,uint32 chan,uint32 reg,uint32 l_val,uint32 r_val)
{
    if( en )
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            USC15241_REG_SET(reg,(l_val),(l_val));
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            USC15241_REG_SET(reg,(r_val),(r_val));
        }
    }
    else
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            USC15241_REG_SET(reg,~(l_val),(l_val));
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            USC15241_REG_SET(reg,~(r_val),(r_val));
        }
    }
}

LOCAL void _usc15241_chan_mask(uint32 chan,uint32 reg,uint32 val
    ,uint32 r_mask,uint32 r_shif
    ,uint32 l_mask,uint32 l_shif
)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        USC15241_REG_SET(reg,(val<<l_shif),l_mask);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        USC15241_REG_SET(reg,(val<<r_shif),r_mask);
    }
}

PUBLIC void __usc15241_init(void)
{
    __usc15241_i2c_init();
    __usc15241_spi_init();
    //USC15241_PRINT:"__usc15241_init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_124_112_2_17_23_3_4_265,(uint8*)"");
}



PUBLIC void __usc15241_en(uint32 en)
{
    //USC15241_PRINT:"__usc15241_en %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_131_112_2_17_23_3_4_266,(uint8*)"d",en);
    if( en )
    {
        SPRD_CODEC_SWTICH_ARM_CTL(1);
        __sprd_codec_dp_en(1);
        __usc15241_bg_en(1);
    }
    else
    {
        __usc15241_bg_en(0);
        __sprd_codec_dp_en(0);
    }
}


PUBLIC void __usc15241_bg_en(uint32 en)
{
    _usc15241_en_op((uint32)(!en),POWER_SET,POWER_SET_BG_PD);
    _usc15241_en_op(en,POWER_SET1,POWER_SET1_BG2_EN);

#if 0
    _usc15241_chan_mask(USC15241_LEFT,POWER_SET,POWER_SET_BG_V_1200,
                0,0,
                POWER_SET_BG_V_MASK,POWER_SET_BG_V_SHIFT
    );

    _usc15241_chan_mask(USC15241_LEFT,POWER_SET1,POWER_SET1_BG2_I_150,
                0,0,
                POWER_SET1_BG2_I_MASK,POWER_SET1_BG2_I_SHIFT
    );
#endif


    _usc15241_en_op(en,DAC_CLK_SET,DAC_CLK_PADIN_EN);
    _usc15241_en_op(en,DAC_CLK_SET,DAC_CLK_ADC_EN);
    _usc15241_en_op(en,DAC_CLK_SET,DAC_CLK_PADOUT_SEL);
}

PUBLIC void __usc15241_vcm_en(uint32 en)
{
}

PUBLIC void __usc15241_vcm_buf_en(uint32 en)
{
}

PUBLIC void __usc15241_adc_clk_pwr(uint32 en)
{
    _usc15241_en_op((uint32)(!en),ADC_SET1,ADC_SET1_CLK_PD);
    _usc15241_en_op((uint32)(!en),ADC_SET1,ADC_SET1_BUF_PD);
    _usc15241_en_op((uint32)(!en),ADC_SET1,ADC_SET1_VCOM_PD);
    _usc15241_en_op(en,ADC_CLK_SEL,ADC_CLK_SEL_PAD);
}

PUBLIC void __usc15241_adc(uint32 en,uint32 chan)
{
    //USC15241_PRINT:"__usc15241_adc %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_187_112_2_17_23_3_4_267,(uint8*)"d",en);

#if 1    
    _usc15241_en_op(en,DAC_CLK_SET,DAC_CLK_DAC_EN);
    _usc15241_en_op(en,DAC_SET1,DAC_SET1_CLK_PAD);
    _usc15241_en_op(en,DAC_SET1,DAC_SET1_CLK_OUT_EN);
#endif
    
    __usc15241_adc_clk_pwr(en);
}


PUBLIC void __usc15241_adc_pwr(uint32 en,uint32 chan)
{
    _usc15241_en_op((uint32)(!en),ADC_SET2,ADC_SET2_ADC1_PD);
    _usc15241_en_op((uint32)(!en),ADC_SET1,ADC_SET1_ADC2_PD);

    //_usc15241_en_op(en,ADC_SET2,ADC_SET2_PGA1_BYP);

    __usc15241_mic_en(en,0);
    __usc15241_mic_en(en,1);
    
    _usc15241_en_op((uint32)(!en),ADC_SET2,ADC_SET2_PGA1_PD);
    _usc15241_en_op((uint32)(!en),ADC_SET,ADC_SET_G2_PGA2_PD);

    _usc15241_en_op(en,ADC_SET1,ADC_SET1_VREFX15_EN);
    _usc15241_en_op(1,ADC_SET1,ADC_SET1_FM2_PGA_PD);
    _usc15241_en_op(1,ADC_SET2,ADC_SET2_FM1_PGA_PD);

#if 1 //MAX ADC PGA
    _usc15241_chan_mask(AUDIO_CHAN_L,ADC_SET,0xFFFF,
                0,0,
                ADC_SET_G2_PGA2_MASK,ADC_SET_G2_PGA2_SHIFT
    );
    _usc15241_chan_mask(AUDIO_CHAN_L,ADC_SET1,0xFFFF,
                0,0,
                ADC_SET1_G1_PGA2_MASK,ADC_SET1_G1_PGA2_SHIFT
    );
    
    _usc15241_chan_mask(AUDIO_CHAN_L,ADC_SET2,0xFFFF,
                0,0,
                ADC_SET2_G2_PGA1_MASK,ADC_SET2_G2_PGA1_SHIFT
    );
    _usc15241_chan_mask(AUDIO_CHAN_L,ADC_SET2,0xFFFF,
                0,0,
                ADC_SET2_G1_PGA1_MASK,ADC_SET2_G1_PGA1_SHIFT
    );
#endif
}

PUBLIC void __usc15241_mic_en(uint32 en,uint32 id)
{
    switch(id)
    {
        case 0:_usc15241_en_op(en,ADC_SET,ADC_SET_MIC2_SEL);break;
        case 1:_usc15241_en_op(en,ADC_SET2,ADC_SET2_MIC1_SEL);break;
        default:break;
    }
}


PUBLIC void __usc15241_mic_bias(uint32 en)
{
    _usc15241_en_op((uint32)(!en),POWER_SET2,POWER_SET2_MICBIAS_PD);
    //_usc15241_en_op(1,POWER_SET2,POWER_SET2_MICBIAS_PD);
}

PUBLIC void __usc15241_linein_rec(uint32 en,uint32 chan)
{
    __sprd_codec_dp_adc_en(en,chan);
    __usc15241_adc_pwr(en,chan);
    __usc15241_mic_bias(en);
}










PUBLIC void __usc15241_dac_clk_en(uint32 en)
{
    _usc15241_en_op((uint32)(!en),POWER_SET,POWER_SET_DRV_LDO_PD);
    _usc15241_en_op((uint32)(!en),POWER_SET,POWER_SET_LDO_PD);
    _usc15241_en_op(en,DAC_CLK_SET,DAC_CLK_DAC_EN);
    _usc15241_en_op(en,DAC_SET1,DAC_SET1_CLK_PAD);
    _usc15241_en_op(en,DAC_SET1,DAC_SET1_CLK_DAC1_EN);
    _usc15241_en_op(en,DAC_SET1,DAC_SET1_CLK_DAC2_EN);
}



PUBLIC void __usc15241_dac_pwr(uint32 en,uint32 chan)
{
    //USC15241_PRINT:"__usc15241_dac_pwr %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_264_112_2_17_23_3_4_268,(uint8*)"d",en);
    _usc15241_en_op(en,DAC_SET1,DAC_SET1_DAC_EN);
    if( AUDIO_IS_CHAN_L(chan) )
    {
        _usc15241_en_op(en,DAC_SET1,(DAC_SET1_VHP_L_EN | DAC_SET1_VHN_L_EN));
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        _usc15241_en_op(en,DAC_SET, (DAC_SET_VHP_R_EN  | DAC_SET_VHN_R_EN ));
    }
}


PUBLIC void __usc15241_dac(uint32 en,uint32 chan)
{
    //USC15241_PRINT:"__usc15241_dac %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_279_112_2_17_23_3_4_269,(uint8*)"d",en);
    __sprd_codec_dp_dac_en(en,chan);
    __usc15241_dac_pwr(en,chan);
}


PUBLIC void __usc15241_hp_en(uint32 en,uint32 chan)
{
    //_usc15241_en_op(en,DAC_SET,DAC_SET_DR_EN);
}

PUBLIC void __usc15241_hp_mixer(uint32 en,uint32 chan)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        _usc15241_en_op(en,DAC_SET1,DAC_SET1_VIN_L_EN);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        _usc15241_en_op(en,DAC_SET ,DAC_SET_VIN_R_EN );
    }
}


PUBLIC void __usc15241_hp_mixer_adcbuf(uint32 en,uint32 chan)
{
}


PUBLIC void __usc15241_hp(uint32 en,uint32 chan,uint32 dac,uint32 lin)
{
    __usc15241_hp_en(en,chan);

    if( dac )
    {
        __usc15241_hp_mixer(en,chan);
    }
    if( lin )
    {
        __usc15241_hp_mixer_adcbuf(en,chan);
    }
}

PUBLIC void __usc15241_lout_en(uint32 en,uint32 chan)
{
    _usc15241_en_op(en,DAC_SET,DAC_SET_DR_EN);
}

PUBLIC void __usc15241_lout_mixer(uint32 en,uint32 chan)
{
}


PUBLIC void __usc15241_lout_mixer_adcbuf(uint32 en,uint32 chan)
{
}


PUBLIC void __usc15241_lout(uint32 en,uint32 chan,uint32 dac,uint32 lin)
{
    __usc15241_lout_en(en,chan);


    if( dac )
    {
        __usc15241_lout_mixer(en,chan);
    }
    if( lin )
    {
        __usc15241_lout_mixer_adcbuf(en,chan);
    }
}





PUBLIC void test_usc15241_reg(void)
{
    uint32 reg;
    uint32 val;
    uint32 test_fail=0;
    //SCI_TRACE_LOW:"USC15241 I2C/SPI TEST START"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_361_112_2_17_23_3_4_270,(uint8*)"");

#if 1
    //test I2C
    reg = 0x2;
    val = _usc15241_reg_r(USE_I2C,reg);
    if( val != 0x1e )
    {
        //SCI_TRACE_LOW:"USE_I2C def reg(0x%x)=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_369_112_2_17_23_3_4_271,(uint8*)"dd",reg,val);
        test_fail |= 0x1;
    }
#endif

#if 1
    //test SPI
    reg = 513;
    val = 0xa5f5;
    _usc15241_reg_w(USE_SPI,reg,val);
    val = _usc15241_reg_r(USE_SPI,reg);
    if( val != 0xa5f5 )
    {
        //SCI_TRACE_LOW:"USE_SPI wr reg(0x%x)=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_382_112_2_17_23_3_4_272,(uint8*)"dd",reg,val);
        test_fail |= 0x2;
    }

    val = 0x5a0a;
    _usc15241_reg_w(USE_SPI,reg,val);
    val = _usc15241_reg_r(USE_SPI,reg);
    if( val != 0x5a0a )
    {
        //SCI_TRACE_LOW:"USE_SPI wr2 reg(0x%x)=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_391_112_2_17_23_3_4_273,(uint8*)"dd",reg,val);
        test_fail |= 0x2;
    }
    
#endif
    if( 0!=test_fail )
    {
        if( 0x1 & test_fail )
        {
            //SCI_TRACE_LOW:"USE_I2C test FAILE!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_400_112_2_17_23_3_4_274,(uint8*)"");
        }
        if( 0x2 & test_fail )
        {
            //SCI_TRACE_LOW:"USE_SPI test FAILE!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_404_112_2_17_23_3_4_275,(uint8*)"");
        }
        while(1) //lint !e716
        {
            if( 0x1 & test_fail )
            {
                _usc15241_reg_w(USE_I2C,0x2,0xf5);
                _usc15241_reg_r(USE_I2C,0x2);
            }
            if( 0x2 & test_fail )
            {
                _usc15241_reg_w(USE_SPI,513,0xa5a5);
                _usc15241_reg_r(USE_SPI,513);
            }
        }
    }
    //SCI_TRACE_LOW:"USC15241 I2C/SPI TEST END\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_420_112_2_17_23_3_4_276,(uint8*)"");
}


PUBLIC int __usc15241_reg_print(void)
{
    uint32 i;
    uint32 len;
    uint32 usc15241_i2c_reg[] = {0x05,0x06,0x07};
    uint32 usc15241_spi_reg[] = {19,20,21,22,23,24,25};

    //SCI_TRACE_LOW:"USC15241 REG"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_431_112_2_17_23_3_4_277,(uint8*)"");
    len = sizeof(usc15241_i2c_reg)/sizeof(usc15241_i2c_reg[0]);
    for(i=0;i<len;i++)
    {
        //SCI_TRACE_LOW:"I2C[0x%02x]=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_435_112_2_17_23_3_4_278,(uint8*)"dd",usc15241_i2c_reg[i],USC15241_REG_GET(REG_I2C(usc15241_i2c_reg[i])));
    }

    len = sizeof(usc15241_spi_reg)/sizeof(usc15241_spi_reg[0]);
    for(i=0;i<len;i++)
    {
        //SCI_TRACE_LOW:"SPI[%04d]=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_441_112_2_17_23_3_4_279,(uint8*)"dd",usc15241_spi_reg[i],USC15241_REG_GET(REG_SPI(usc15241_spi_reg[i])));
    }
    return 0;
}

PUBLIC uint32 __usc15241_calc_fs(uint32 fs)
{
    uint32 fs_ret;

    // 8000,9600,11025,12000,16000,22050,24000,32000,44100,48000,96000
    if (fs < 8800)
    {
        fs_ret = 8000;
    }
    else if (fs < 10312)
    {
        fs_ret = 9600;
    }
    else if (fs < 11512)
    {
        fs_ret = 11025;
    }
    else if (fs  < 14000)
    {
        fs_ret = 12000;
    }
    else if (fs < 19025)
    {
        fs_ret = 16000;
    }
    else if (fs < 23025)
    {
        fs_ret = 22050;
    }
    else if (fs < 28000)
    {
        fs_ret = 24000;
    }
    else if (fs < 38050)
    {
        fs_ret = 32000;
    }
    else if (fs < 46050)
    {
        fs_ret = 44100;
    }
    else if (fs < 72000)
    {
        fs_ret = 48000;
    }
    else// if (fs <= 96000)
    {
        fs_ret = 96000;
    }

    return fs_ret;
}

PUBLIC void __usc15241_ext_pa_ctl(uint32 en)
{
    //USC15241_PRINT:"__usc15241_ext_pa_ctl:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_501_112_2_17_23_3_4_280,(uint8*)"d",en);
    codec_pa_ctl(en,0);
}

PUBLIC void __usc15241_ext_hp_pa_ctl(uint32 en)
{
    uint32 delay_time = 0;
    //USC15241_PRINT:"__usc15241_ext_hp_pa_ctl:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_507_112_2_17_23_3_4_281,(uint8*)"d",en);
    if( codec_hp_pa_test_and_get_param(&delay_time) && en )
    {
        USC15241_PRINT("delay_time:%d",delay_time);
        SCI_Sleep(delay_time);
    }
    codec_hp_pa_ctl(en);
}

PUBLIC uint32 __usc15241_is_arm_ctl(void)
{
    return 1;
}

