/******************************************************************************
 ** File Name:      sprd_codec_dp_phy_v1.c                                    *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec dp control                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "chip_plf_export.h"

#include "audio_drvapi.h"
#include "../sprd_codec_dp_phy.h"
#include "sprd_codec_dp_phy_v1.h"

#ifdef CODEC_DEBUG
#define SPRD_CODEC_DP_PRINT    SCI_TRACE_LOW
#else
#define SPRD_CODEC_DP_PRINT(...)
#endif

LOCAL SPRD_CODEC_DP_CTL_T * aud_ptr = (SPRD_CODEC_DP_CTL_T*)SPRD_CODEC_DP_REG_BASE;

LOCAL void _sprd_codec_dp_en_op(uint32 en,void* reg,uint32 bit)
{
    //SPRD_CODEC_DP_PRINT("_sprd_codec_dp_op 0x%x 0x%x",reg,bit);
    if( en )
    {
        SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),(bit),(bit));
    }
    else
    {
        SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),~(bit),(bit));
    }
}

LOCAL uint32 _sprd_codec_dp_en_op_test(uint32 en,void* reg,uint32 bit)
{
    uint32 val = SPRD_CODEC_DP_REG_GET((uint32)(reg));

    _sprd_codec_dp_en_op(en,reg,bit);
    
    return (uint32)((!!(val&bit)) != (!!en));//lint !e731
}

LOCAL void _sprd_codec_dp_lr_en(uint32 en,uint32 chan,void* reg,uint32 l_val,uint32 r_val)
{
    if( en )
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),(l_val),(l_val));
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),(r_val),(r_val));
        }
    }
    else
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),~(l_val),(l_val));
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),~(r_val),(r_val));
        }
    }
}

LOCAL void _sprd_codec_dp_path_op(uint32 path,void* reg,uint32 en,
    uint32 adc_bit,uint32 dac_bit)
{
    switch(path)
    {
        case AUDIO_PLAYBACK:
        {
            _sprd_codec_dp_en_op(en,reg,adc_bit);
        }break;
        case AUDIO_CAPTRUE:
        {
            _sprd_codec_dp_en_op(en,reg,dac_bit);
        }break;
        default:break;
    }
}

LOCAL void _sprd_codec_dp_path_mask(uint32 path,void* reg,uint32 val
    ,uint32 adc_mask,uint32 adc_shif
    ,uint32 dac_mask,uint32 dac_shif
)
{
    switch(path)
    {
        case AUDIO_PLAYBACK:
        {
            SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),(val<<dac_shif),dac_mask);
        }break;
        case AUDIO_CAPTRUE:
        {
            SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),(val<<adc_shif),adc_mask);
        }break;
        default:break;
    }
}

LOCAL void _sprd_codec_dp_lr_mask(uint32 chan,void* reg,uint32 val
    ,uint32 r_mask,uint32 r_shif
    ,uint32 l_mask,uint32 l_shif
)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),(val<<l_shif),l_mask);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        SPRD_CODEC_DP_WRITE_BITS((uint32)(reg),(val<<r_shif),r_mask);
    }
}


PUBLIC void __sprd_codec_dp_en(uint32 en)
{
    if( en )
    {
        //__sprd_codec_dp_i2s_clkdiv_set(0);
        //__sprd_codec_dp_i2s_lr_sel(AUDIO_PLAYBACK    ,0);
        //__sprd_codec_dp_i2s_lr_sel(AUDIO_CAPTRUE     ,0);
        
        //__sprd_codec_dp_i2s_io_mode(AUDIO_PLAYBACK   ,ADC_I2S_IO_MODE_I2S);
        //__sprd_codec_dp_i2s_io_mode(AUDIO_CAPTRUE    ,ADC_I2S_IO_MODE_I2S);
        //__sprd_codec_dp_i2s_iowl(AUDIO_PLAYBACK      ,DAC_I2S_IOWL_24BIT);
        //__sprd_codec_dp_i2s_iowl(AUDIO_CAPTRUE       ,DAC_I2S_IOWL_24BIT);
        
        __sprd_codec_dp_dac_mute_div0(26);
        __sprd_codec_dp_dac_mute_div1(0);
        SPRD_CODEC_AUDIF_EN(1,1);
        __sprd_codec_dp_sdm_set();

    }
    else
    {
        SPRD_CODEC_AUDIF_EN(0,1);
    }
}

PUBLIC void __sprd_codec_dp_clear(uint32 path)
{
    //CLEAR
    _sprd_codec_dp_path_op(path,&(aud_ptr->aud_clr),1,(ADC_PATH_CLR),(DAC_PATH_CLR));
}

PUBLIC void __sprd_codec_dp_dac_en(uint32 en,uint32 chan)
{
#ifdef CHIP_VER_6531EFM
    SPRD_CODEC_DP_REG_SET(&(aud_ptr->dac_sdm_dc_l),0x9999);
    SPRD_CODEC_DP_REG_SET(&(aud_ptr->dac_sdm_dc_h),0x1);
#endif
    _sprd_codec_dp_lr_en(en,chan,&(aud_ptr->top_ctl),DAC_EN_L,DAC_EN_R);
}

PUBLIC void __sprd_codec_dp_adc_en(uint32 en,uint32 chan)
{
    _sprd_codec_dp_lr_en(en,chan,&(aud_ptr->top_ctl),ADC_EN_L,ADC_EN_R);
}


PUBLIC void __sprd_codec_dp_i2s_sel(uint32 path,uint32 port)
{
    _sprd_codec_dp_path_mask(path,&(aud_ptr->top_ctl),port
        ,ADC_I2S_SEL_MASK,ADC_I2S_SEL_SHIFT
        ,DAC_I2S_SEL_MASK,DAC_I2S_SEL_SHIFT
    );
}


PUBLIC void __sprd_codec_dp_adc_sdm_in_sel(uint32 port)
{
    
    SPRD_CODEC_DP_WRITE_BITS((uint32)(&(aud_ptr->top_ctl)),(port<<ADC_SDM_SEL_SHIFT),ADC_SDM_SEL_MASK);
}

PUBLIC void __sprd_codec_dp_i2s_clkdiv_set(uint32 clkdiv)
{
    _sprd_codec_dp_en_op(clkdiv,&(aud_ptr->i2s_ctl),I2S_CLK_DIV_2);
}

PUBLIC void __sprd_codec_dp_i2s_lr_sel(uint32 path,uint32 hforl)
{

    _sprd_codec_dp_path_op(path,&(aud_ptr->i2s_ctl),hforl,(ADC_LR_HFORL),(DAC_LR_HFORL));
}

PUBLIC void __sprd_codec_dp_i2s_io_mode(uint32 path,uint32 mode)
{
    _sprd_codec_dp_path_mask(path,&(aud_ptr->i2s_ctl),mode
        ,ADC_I2S_IO_MODE_MASK,ADC_I2S_IO_MODE_SHIFT
        ,DAC_I2S_IO_MODE_MASK,DAC_I2S_IO_MODE_SHIFT
    );
}

PUBLIC void __sprd_codec_dp_i2s_iowl(uint32 path,uint32 iowl)
{
    _sprd_codec_dp_path_mask(path,&(aud_ptr->i2s_ctl),iowl
        ,ADC_I2S_IOWL_MASK,ADC_I2S_IOWL_SHIFT
        ,DAC_I2S_IOWL_MASK,DAC_I2S_IOWL_SHIFT
    );
}


PUBLIC uint32 __sprd_codec_dp_adc_fs_mode_calc(uint32 sample_rate,uint32 *fs_mode)
{
    uint32 sample_rate_set;
    uint32 fs_mode_ret;

    for( fs_mode_ret = 1; fs_mode_ret<16; fs_mode_ret++)
    {
        if( sample_rate < (4000*fs_mode_ret+2000) )
        {
            break; 
        }
    }
    sample_rate_set = 4000*fs_mode_ret;
    if( fs_mode )
    {
        *fs_mode = fs_mode_ret;
    }
    return sample_rate_set;
}

PUBLIC void __sprd_codec_dp_adc_fs_mode_set(uint32 sample_rate)
{
    uint32 fs_mode;
    
    __sprd_codec_dp_adc_fs_mode_calc(sample_rate,&fs_mode);

    SPRD_CODEC_DP_WRITE_BITS((uint32)(&(aud_ptr->adc_ctl)),(fs_mode<<ADC_SRC_N_SHIFT),ADC_SRC_N_MASK);
}



PUBLIC void __sprd_codec_dp_dac_fs_mode_set(uint32 sample_rate)
{
    uint32 fs_mode;
    
    __sprd_codec_dp_dac_fs_mode_calc(sample_rate,&fs_mode);

    SPRD_CODEC_DP_WRITE_BITS((uint32)(&(aud_ptr->dac_ctl)),(fs_mode<<DAC_FS_MODE_SHIFT),DAC_FS_MODE_MASK);
}

PUBLIC void __sprd_codec_dp_dac_mute_div0(uint32 cnt)
{
    //SPRD_CODEC_DP_PRINT:"__sprd_codec_dp_dac_mute_div0 %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_DP_PHY_V0_251_112_2_17_23_2_56_222,(uint8*)"d",cnt);
    _sprd_codec_dp_path_mask(AUDIO_PLAYBACK,&(aud_ptr->dac_ctl),cnt
        ,0,0
        ,DAC_MUTE_DIV0_MASK,DAC_MUTE_DIV0_SHIFT
    );
}

PUBLIC void __sprd_codec_dp_dac_mute_div1(uint32 cnt)
{
    _sprd_codec_dp_path_mask(AUDIO_PLAYBACK,&(aud_ptr->dac_ctl),cnt
        ,0,0
        ,DAC_MUTE_DIV1_MASK,DAC_MUTE_DIV1_SHIFT
    );
}


PUBLIC void __sprd_codec_dp_dac_mute_en(uint32 en)
{
    //SPRD_CODEC_DP_PRINT:"__sprd_codec_dp_dac_mute_en %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_DP_PHY_V0_269_112_2_17_23_2_56_223,(uint8*)"d",en);
    _sprd_codec_dp_en_op(en,&(aud_ptr->dac_ctl),DAC_MUTE_EN);
}

PUBLIC uint32 __sprd_codec_dp_dac_mute_sts(void)
{
    uint32 val ;
    val = SPRD_CODEC_DP_REG_GET(&(aud_ptr->aud_sts0));
    return ((val&MUTE_ST_MASK)>>MUTE_ST_SHIFT);
}

PUBLIC uint32 __sprd_codec_dp_dac_mute(uint32 mute)
{
    __sprd_codec_dp_dac_mute_en(1);
    return _sprd_codec_dp_en_op_test(mute,&(aud_ptr->dac_ctl),DAC_MUTE_START);
}


PUBLIC uint32 __sprd_codec_dp_gain_done(uint32 up)
{
    if( up )
    {
        if( MUTE_IDLE == __sprd_codec_dp_dac_mute_sts() )
        {
            return 1;
        }
    }
    else
    {
        if( MUTE_WAIT == __sprd_codec_dp_dac_mute_sts() )
        {
            return 1;
        }
    }
    return 0;
}

PUBLIC void __sprd_codec_dp_clr_gain(uint32 up)
{
}


PUBLIC void __sprd_codec_dp_sdm_set(void)
{
    SPRD_CODEC_DP_REG_SET(&(aud_ptr->sdm_ctl0),0x100);
    SPRD_CODEC_DP_REG_SET(&(aud_ptr->sdm_ctl1),0x8);
    __sprd_codec_dp_sdm_mapping_set(1);
}

PUBLIC void __sprd_codec_dp_sdm_mapping_set(uint32 en)
{
    _sprd_codec_dp_en_op(en,&(aud_ptr->sdm_ctl1),BIT_3);
}


PUBLIC uint32 __sprd_codec_dp_int_sts_d_raw(void)
{
    uint32 val ;
    val = SPRD_CODEC_DP_REG_GET(&(aud_ptr->aud_sts0));
    return (val&MUTE_GAIN_D_DONE_INT_RAW);
}

PUBLIC uint32 __sprd_codec_dp_int_sts_d(void)
{
    uint32 val ;
    val = SPRD_CODEC_DP_REG_GET(&(aud_ptr->aud_sts0));
    return (val&MUTE_GAIN_D_DONE_INT_MASK);
}

PUBLIC uint32 __sprd_codec_dp_int_sts_u_raw(void)
{
    uint32 val ;
    val = SPRD_CODEC_DP_REG_GET(&(aud_ptr->aud_sts0));
    return (val&MUTE_GAIN_U_DONE_INT_RAW);
}

PUBLIC uint32 __sprd_codec_dp_int_sts_u(void)
{
    uint32 val ;
    val = SPRD_CODEC_DP_REG_GET(&(aud_ptr->aud_sts0));
    return (val&MUTE_GAIN_U_DONE_INT_MASK);
}

PUBLIC void __sprd_codec_dp_int_clr_d(void)
{
    _sprd_codec_dp_en_op(1,&(aud_ptr->int_clr),MUTE_GAIN_D_DONE_INT_CLR);
}

PUBLIC void __sprd_codec_dp_int_clr_u(void)
{
    _sprd_codec_dp_en_op(1,&(aud_ptr->int_clr),MUTE_GAIN_U_DONE_INT_CLR);
}

PUBLIC void __sprd_codec_dp_int_en_d(uint32 en)
{
    _sprd_codec_dp_en_op(en,&(aud_ptr->int_en),MUTE_GAIN_D_DONE_INT_EN);
}

PUBLIC void __sprd_codec_dp_int_en_u(uint32 en)
{
    _sprd_codec_dp_en_op(en,&(aud_ptr->int_en),MUTE_GAIN_U_DONE_INT_EN);
}


PUBLIC void __sprd_codec_dp_loop_en(uint32 en)
{
    _sprd_codec_dp_en_op(en,&(aud_ptr->loop_ctl),AUD_LOOP_TEST);
}


PUBLIC uint32 __sprd_codec_dp_dac_fs_mode_calc(uint32 sample_rate,uint32 *fs_mode)
{
    uint32 sample_rate_set;
    uint32 fs_mode_ret;

    // 8000,9600,11025,12000,16000,22050,24000,32000,44100,48000,96000
    if (sample_rate < 8800)
    {
        sample_rate_set = 8000;
        fs_mode_ret = DAC_FS_MODE_8K;
    }
    else if (sample_rate < 10312)
    {
        sample_rate_set = 9600;
        fs_mode_ret = DAC_FS_MODE_9K;
    }
    else if (sample_rate < 11512)
    {
        sample_rate_set = 11025;
        fs_mode_ret = DAC_FS_MODE_11K;
    }
    else if (sample_rate  < 14000)
    {
        sample_rate_set = 12000;
        fs_mode_ret = DAC_FS_MODE_12K;
    }
    else if (sample_rate < 19025)
    {
        sample_rate_set = 16000;
        fs_mode_ret = DAC_FS_MODE_16K;
    }
    else if (sample_rate < 23025)
    {
        sample_rate_set = 22050;
        fs_mode_ret = DAC_FS_MODE_22K;
    }
    else if (sample_rate < 28000)
    {
        sample_rate_set = 24000;
        fs_mode_ret = DAC_FS_MODE_24K;
    }
    else if (sample_rate < 38050)
    {
        sample_rate_set = 32000;
        fs_mode_ret = DAC_FS_MODE_32K;
    }
    else if (sample_rate < 46050)
    {
        sample_rate_set = 44100;
        fs_mode_ret = DAC_FS_MODE_44K;
    }
    else if (sample_rate < 72000)
    {
        sample_rate_set = 48000;
        fs_mode_ret = DAC_FS_MODE_48K;
    }
    else if (sample_rate <= 96000)
    {
        sample_rate_set = 96000;
        fs_mode_ret = DAC_FS_MODE_96K;
    }
    else
    {
        sample_rate_set = 8000;
        fs_mode_ret = DAC_FS_MODE_8K;
    }
    if( fs_mode )
    {
        *fs_mode = fs_mode_ret;
    }
    return sample_rate_set;
}


PUBLIC void __sprd_codec_dp_reg_print(void)
{
    uint32 reg = SPRD_CODEC_DP_REG_BASE;
    uint32 offset = 0;
    for(;offset<0x2C;offset+=0x10)
    {
        //SPRD_CODEC_DP_PRINT:"0x%02x | 0x%04x 0x%04x 0x%04x 0x%04x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_DP_PHY_V0_484_112_2_17_23_2_56_224,(uint8*)"ddddd",offset,SPRD_CODEC_DP_REG_GET((void*)(reg+offset+0x0)),SPRD_CODEC_DP_REG_GET((void*)(reg+offset+0x4)),SPRD_CODEC_DP_REG_GET((void*)(reg+offset+0x8)),SPRD_CODEC_DP_REG_GET((void*)(reg+offset+0xc)));
    }
}

