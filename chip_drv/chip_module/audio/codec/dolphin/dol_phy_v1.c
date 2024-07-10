/******************************************************************************
 ** File Name:      dol_phy_v1.c                                              *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Realizing a state machine to stablize the audio codec, and*
 **                 exporting interfaces to audio device HAL layer.           *
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
#include "sci_api.h"

#include "chip_plf_export.h"
#include "ldo_drvapi.h"
#include "dol_phy.h"

#include "dol_phy_v1.h"
#include "audio_cfg.h"


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
#ifdef DOL_DEBUG
#define DOL_DBG       SCI_TRACE_LOW
#define DOL_DBG_ASS   SCI_ASSERT
#else
#define DOL_DBG(...)
#define DOL_DBG_ASS(x)
#endif

#define DOL_PRT       SCI_TRACE_LOW



/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

typedef struct
{
    uint32 sample_rate;
    uint32 da_reg_set;
    uint32 ad_reg_set;
} DOL_SAMPLE_RATE_MAP_T;

typedef struct
{
    uint32 cur_vol;
    uint32 ldo_level;
} DOL_VBO_VOL_MAP_T;

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

LOCAL DOL_SAMPLE_RATE_MAP_T _sample_rate_map_tab [DOL_SAMPLERATE_NUM_MAX] =
{
    {8000,  VBCCR_DAC_FS_8000,  VBCCR_ADC_FS_8000},
    {9600,  VBCCR_DAC_FS_9600,  VBCCR_ADC_FS_9600},
    {11025, VBCCR_DAC_FS_11025, VBCCR_ADC_FS_11025},
    {12000, VBCCR_DAC_FS_12000, VBCCR_ADC_FS_12000},
    {16000, VBCCR_DAC_FS_16000, VBCCR_ADC_FS_16000},
    {22050, VBCCR_DAC_FS_22050, VBCCR_ADC_FS_22050},
    {24000, VBCCR_DAC_FS_24000, VBCCR_ADC_FS_24000},
    {32000, VBCCR_DAC_FS_32000, VBCCR_ADC_FS_32000},
    {44100, VBCCR_DAC_FS_44100, VBCCR_ADC_FS_44100},
    {48000, VBCCR_DAC_FS_48000, VBCCR_ADC_FS_48000},
    {96000, VBCCR_DAC_FS_96000, VBCCR_ADC_FS_96000},
};


LOCAL DOL_VBO_VOL_MAP_T _vbo_vol_map_tab[] =
{
    {3400,  LDO_VOLT_LEVEL3},
    {3600,  LDO_VOLT_LEVEL2},
    {3700,  LDO_VOLT_LEVEL0},
    {4300,  LDO_VOLT_LEVEL1},
};



/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

LOCAL void _dol_ifr_dummy_read(void)
{
    volatile uint32 value;
    //ken.kuang for check.
    value = DOL_REG_GET(VBIFR);
    value = DOL_REG_GET(VBIFR);
    value = DOL_REG_GET(VBIFR);
    value = DOL_REG_GET(VBIFR);
}

/*****************************************************************************/
// Description :    Gets the sample rate supportted by dolphin.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL uint32 _dol_get_sample_rate_map (uint32 sample_rate, BOOLEAN is_da)
{
    uint32 i;

    for (i = 0; i < DOL_SAMPLERATE_NUM_MAX; i++)
    {
        if (sample_rate == _sample_rate_map_tab[i].sample_rate)
        {
            break;
        }
    }

    if (i >= DOL_SAMPLERATE_NUM_MAX)
    {
        //DOL_PRT:"_dol_get_sample_rate_map, Can not support samplerate : %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_126_112_2_17_23_2_39_73,(uint8*)"d",sample_rate);

        i = 0;
    }

    if (is_da)
    {
        return _sample_rate_map_tab[i].da_reg_set;
    }
    else
    {
        return _sample_rate_map_tab[i].ad_reg_set;
    }

}


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    Calculates sample rate supportted by codec according to v-
//                  alue passed by HAL layer.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC uint32 __dol_calc_fs(uint32 fs)
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

PUBLIC void __dol_s2mono(BOOLEAN is_mono)
{
    if (is_mono)
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_MONO_ENABLE,
            VBCR1_MONO_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_MONO_DISABLE,
            VBCR1_MONO_MASK);
    }
    //DOL_DBG:"__dol_s2mono %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_216_112_2_17_23_2_39_74,(uint8*)"d",is_mono);
    AUDIO_HAL_PRTMEM ( (0xBB010000 
        | (is_mono & 0x0000FFFF)    ));
}

PUBLIC void __dol_mic_sel(uint8 mic_sel)
{
    switch (mic_sel)
    {
        case 0:
            DOL_WRITE_BITS (VBCR2,
                VBCR2_MICSEL_MIC1,
                VBCR2_MICSEL_MASK);
            break;

        case 1:
            DOL_WRITE_BITS (VBCR2,
                VBCR2_MICSEL_MIC2,
                VBCR2_MICSEL_MASK);
            break;

        default:
            break;
    }
    //DOL_DBG:"__dol_mic_sel %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_240_112_2_17_23_2_39_75,(uint8*)"d",mic_sel);
    AUDIO_HAL_PRTMEM ((0xBB020000 
        | (mic_sel & 0x0000FFFF)    ));
}

/*****************************************************************************/
// Description :    Initailizes the codec. It's called by HAL layer when system
//                  starts up.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __dol_init(void)
{
    __dol_switch_to(1);
    __dol_reg_dummy();
    //DOL_DBG:"__dol_init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_252_112_2_17_23_2_39_76,(uint8*)"");
    AUDIO_HAL_PRTMEM ( 0xBB030000);
}

PUBLIC void __dol_mclk(uint32 en)
{
    if( en )
    {
        REG32 (VB_CLK_CTL) |= (ARM_VB_MCLKON | ARM_VB_ANAON);
    }
    else
    {
        REG32 (VB_CLK_CTL) &= ~(ARM_VB_MCLKON | ARM_VB_ANAON);
    }
}

/*****************************************************************************/
// Description :    Initializes the parameters of state machine to open the codec
//                  according to current state.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __dol_open (void)
{

    DOL_WRITE_BITS (VBPMR2,
        VBPMR2_SB_MC_POWER_DOWN,
        VBPMR2_SB_MC_MASK);
    
    // Set Serial interface, IIS mode
    DOL_WRITE_BITS (VBAICR, 0x0F, 0x0F);

#if 0
    // ADC : 16-bit, DAC : 16-bit
    DOL_WRITE_BITS (VBCR2,
        0,
        (VBCR2_ADCADWL_MASK | VBCR2_DACADWL_MASK) );
#else
    // ADC : 24-bit, DAC : 24-bit
    DOL_WRITE_BITS (VBCR2,
        (VBCR2_ADCADWL_MASK | VBCR2_DACADWL_MASK),
        (VBCR2_ADCADWL_MASK | VBCR2_DACADWL_MASK) );
#endif
    //DOL_DBG:"__dol_open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_293_112_2_17_23_2_40_77,(uint8*)"");
    AUDIO_HAL_PRTMEM ( 0xBB040000);
}


PUBLIC void __dol_reset(void)
{
    //RESET
    DOL_PHY_RESET_M();
    //DOL_DBG:"__dol_reset"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_302_112_2_17_23_2_40_78,(uint8*)"");
    AUDIO_HAL_PRTMEM ( 0xBB050000);
}


PUBLIC void __dol_sb(uint32 is_en)
{
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR2,
            VBPMR2_SB_POWER_ON,
            VBPMR2_SB_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBPMR2,
            VBPMR2_SB_POWER_DOWN,
            VBPMR2_SB_MASK);
    }
    //DOL_DBG:"__dol_sb %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_321_112_2_17_23_2_40_79,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB060000 
        | (is_en & 0x0000FFFF)    ));
}


PUBLIC void __dol_sb_sleep(uint32 is_en)
{
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR2,
            VBPMR2_SB_SLEEP_POWER_ON,
            VBPMR2_SB_SLEEP_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBPMR2,
            VBPMR2_SB_SLEEP_POWER_DOWN,
            VBPMR2_SB_SLEEP_MASK);
    }
    //DOL_DBG:"__dol_sb_sleep %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_341_112_2_17_23_2_40_80,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB070000 
        | (is_en & 0x0000FFFF)    ));
}

LOCAL void _dol_sb_micbias(uint32 is_en)
{
    if( is_en )
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_SB_MICBIAS_ENABLE,
            VBCR1_SB_MICBIAS_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_SB_MICBIAS_DISABLE,
            VBCR1_SB_MICBIAS_MASK);
    }
    //DOL_DBG:"_dol_sb_micbias %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_360_112_2_17_23_2_40_81,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB080000 
        | (is_en & 0x0000FFFF)    ));
}


PUBLIC void __dol_sb_adc(uint32 is_en)
{
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_ADC_POWER_ON,
            VBPMR1_SB_ADC_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_ADC_POWER_DOWN,
            VBPMR1_SB_ADC_MASK);
    }
    //DOL_DBG:"__dol_sb_adc %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_380_112_2_17_23_2_40_82,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB090000 
        | (is_en & 0x0000FFFF)    ));
    //adc need open mic bias.
    _dol_sb_micbias(is_en);
}

PUBLIC void __dol_linein_rec(uint32 is_en)
{
    if( is_en )
    {
        DOL_LINEIN_REC_EN(SCI_TRUE);
    }
    else
    {
        DOL_LINEIN_REC_EN(SCI_FALSE);
    }
    //DOL_DBG:"__dol_linein_rec %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_397_112_2_17_23_2_40_83,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB0A0000 
        | (is_en & 0x0000FFFF)    ));
    //when linein record , mic is not selected.
    _dol_sb_micbias(0);
}

PUBLIC void __dol_sb_dac(uint32 is_en)
{
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_DAC_POWER_ON,
            VBPMR1_SB_DAC_MASK);
        DOL_WRITE_BITS (VBCR1,
            VBCR1_DACSEL_SELECTED,
            VBCR1_DACSEL_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_DACSEL_IGNORED,
            VBCR1_DACSEL_MASK);
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_DAC_POWER_DOWN,
            VBPMR1_SB_DAC_MASK);
    }
    //DOL_DBG:"__dol_sb_dac %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_424_112_2_17_23_2_40_84,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB0B0000 
        | (is_en & 0x0000FFFF)    ));
}

PUBLIC void __dol_sb_lin(uint32 is_en)
{
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_LIN_POWER_ON,
            VBPMR1_SB_LIN_MASK);
        DOL_WRITE_BITS (VBCR1,
            VBCR1_BYPASS_SELECTED,
            VBCR1_BYPASS_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_BYPASS_IGNORED,
            VBCR1_BYPASS_MASK);
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_LIN_POWER_DOWN,
            VBPMR1_SB_LIN_MASK);
    }
    //DOL_DBG:"__dol_sb_lin %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_449_112_2_17_23_2_40_85,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB0C0000 
        | (is_en & 0x0000FFFF)    ));
}

PUBLIC void __dol_sb_mix(uint32 is_en)
{
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_MIX_POWER_ON,
            VBPMR1_SB_MIX_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_MIX_POWER_DOWN,
            VBPMR1_SB_MIX_MASK);
    }
    //DOL_DBG:"__dol_sb_mix %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_468_112_2_17_23_2_40_86,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB0D0000 
        | (is_en & 0x0000FFFF)    ));
}

PUBLIC void __dol_sb_lout(uint32 is_en)
{
    
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_LOUT_POWER_ON,
            VBPMR1_SB_LOUT_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_LOUT_POWER_DOWN,
            VBPMR1_SB_LOUT_MASK);
    }
    //DOL_DBG:"__dol_sb_lout %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_488_112_2_17_23_2_40_87,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB0E0000 
        | (is_en & 0x0000FFFF)    ));
}

PUBLIC uint32 __dol_sb_out(uint32 is_en)
{
    uint32 reg;
    do
    {
        __dol_clr_ramp(is_en);
    }while( __dol_ramp_done(is_en) );
    //SCI_Sleep(1);
    //DOL_DBG:"__dol_sb_out %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_501_112_2_17_23_2_40_88,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB0F0000 
        | (is_en & 0x0000FFFF)    ));

    reg = DOL_REG_GET(VBPMR1);
    
    
    if( is_en )
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_OUT_POWER_ON,
            VBPMR1_SB_OUT_MASK);
        if( VBPMR1_SB_OUT_POWER_ON != (reg & VBPMR1_SB_OUT_MASK) )
        {
            return 1;
        }
    }
    else
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_OUT_POWER_DOWN,
            VBPMR1_SB_OUT_MASK);
        if( VBPMR1_SB_OUT_POWER_DOWN != (reg & VBPMR1_SB_OUT_MASK) )
        {
            return 1;
        }
    }
    return 0;
}


PUBLIC uint32 __dol_dac_mute(uint32 is_mute)
{
    uint32 reg;
    do
    {
        __dol_clr_gain((uint32)!is_mute);
    }while( __dol_gain_done((uint32)!is_mute) );
    //SCI_Sleep(1);
    //DOL_DBG:"__dol_dac_mute %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_540_112_2_17_23_2_40_89,(uint8*)"d",is_mute);
    AUDIO_HAL_PRTMEM ( (0xBB100000 
        | (is_mute & 0x0000FFFF)    ));
    
    reg = DOL_REG_GET(VBCR1);
    
    if( is_mute )
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_DAC_MUTE_ENABLE,
            VBCR1_DAC_MUTE_MASK);
        if( VBCR1_DAC_MUTE_ENABLE != (reg & VBCR1_DAC_MUTE_MASK) )
        {
            return 1;
        }
    }
    else
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_DAC_MUTE_DISABLE,
            VBCR1_DAC_MUTE_MASK);
        if( VBCR1_DAC_MUTE_DISABLE != (reg & VBCR1_DAC_MUTE_MASK) )
        {
            return 1;
        }
    }
    return 0;
}

PUBLIC void __dol_hp_mute(uint32 is_mute)
{
    if (is_mute)
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_HP_DIS_ENABLE,
            VBCR1_HP_DIS_MASK);
        __dol_ext_hp_pa_ctl(0);
    }
    else
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_HP_DIS_DISABLE,
            VBCR1_HP_DIS_MASK);
        __dol_ext_hp_pa_ctl(1);
    }
    //DOL_DBG:"__dol_hp_mute %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_585_112_2_17_23_2_40_90,(uint8*)"d",is_mute);
    AUDIO_HAL_PRTMEM ( (0xBB110000 
        | (is_mute & 0x0000FFFF)    ));
}

PUBLIC void __dol_sb_blt(uint32 is_en)
{
    if (is_en)
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_BTL_POWER_ON,
            VBPMR1_SB_BTL_MASK);
    }
    else
    {
        DOL_WRITE_BITS (VBPMR1,
            VBPMR1_SB_BTL_POWER_DOWN,
            VBPMR1_SB_BTL_MASK);
    }
    //DOL_DBG:"__dol_sb_blt %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_604_112_2_17_23_2_40_91,(uint8*)"d",is_en);
    AUDIO_HAL_PRTMEM ( (0xBB120000 
        | (is_en & 0x0000FFFF)    ));
}

PUBLIC void __dol_ep_mute(uint32 is_mute)
{
    if (is_mute)
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_BTL_MUTE_ENABLE,
            VBCR1_BTL_MUTE_MASK);
        __dol_sb_blt(0);
    }
    else
    {
        DOL_WRITE_BITS (VBCR1,
            VBCR1_BTL_MUTE_DISABLE,
            VBCR1_BTL_MUTE_MASK);
        __dol_sb_blt(1);
    }
    //DOL_DBG:"__dol_ep_mute %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_625_112_2_17_23_2_40_92,(uint8*)"d",is_mute);
    AUDIO_HAL_PRTMEM ( (0xBB130000 
        | (is_mute & 0x0000FFFF)    ));
}


PUBLIC uint32 __dol_ramp_done(uint32 up)
{
    _dol_ifr_dummy_read();
    //DOL_DBG:"__dol_ramp_done %d 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_634_112_2_17_23_2_40_93,(uint8*)"dd",up,DOL_REG_GET(VBIFR) );
    AUDIO_HAL_PRTMEM ( (0xBB140000 
        | (DOL_REG_GET(VBIFR) & 0x000000FF)
        | (up & 0x0000FF00)    ));
    if( up )
    {
        return (VBIFR_RAMP_UP_DONE_MASK & DOL_REG_GET(VBIFR) );
    }
    else
    {
        return (VBIFR_RAMP_DOWN_DONE_MASK & DOL_REG_GET(VBIFR) );
    }
}

PUBLIC void __dol_clr_ramp(uint32 up)
{
    if( up )
    {
        DOL_WRITE_BITS (VBIFR,
            VBIFR_RAMP_UP_DONE_MASK,
            VBIFR_RAMP_UP_DONE_MASK); //Ru
    }
    else
    {
        DOL_WRITE_BITS (VBIFR,
            VBIFR_RAMP_DOWN_DONE_MASK,
            VBIFR_RAMP_DOWN_DONE_MASK); //Rd
    }
    _dol_ifr_dummy_read();
    //DOL_DBG:"__dol_clr_ramp %d 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_663_112_2_17_23_2_40_94,(uint8*)"dd",up,DOL_REG_GET(VBIFR) );
    AUDIO_HAL_PRTMEM ( (0xBB150000 
        | (DOL_REG_GET(VBIFR) & 0x000000FF)
        | (up & 0x0000FF00)    ));
}


PUBLIC uint32 __dol_gain_done(uint32 up)
{
    _dol_ifr_dummy_read();
    //DOL_DBG:"__dol_gain_done %d 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_673_112_2_17_23_2_40_95,(uint8*)"dd",up,DOL_REG_GET(VBIFR) );
    AUDIO_HAL_PRTMEM ( (0xBB160000 
        | (DOL_REG_GET(VBIFR) & 0x000000FF)
        | (up & 0x0000FF00)    ));
    if( up )
    {
        return (VBIFR_GAIN_UP_DONE_MASK & DOL_REG_GET(VBIFR) );
    }
    else
    {
        return (VBIFR_GAIN_DOWN_DONE_MASK & DOL_REG_GET(VBIFR) );
    }
}

PUBLIC void __dol_clr_gain(uint32 up)
{
    if( up )
    {
        DOL_WRITE_BITS (VBIFR,
            VBIFR_GAIN_UP_DONE_MASK,
            VBIFR_GAIN_UP_DONE_MASK); //Gu
    }
    else
    {
        DOL_WRITE_BITS (VBIFR,
            VBIFR_GAIN_DOWN_DONE_MASK,
            VBIFR_GAIN_DOWN_DONE_MASK); //Gd
    }
    _dol_ifr_dummy_read();
    //DOL_DBG:"__dol_clr_gain %d 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_702_112_2_17_23_2_40_96,(uint8*)"dd",up,DOL_REG_GET(VBIFR) );
    AUDIO_HAL_PRTMEM ( (0xBB170000 
        | (DOL_REG_GET(VBIFR) & 0x000000FF)
        | (up & 0x0000FF00)    ));
}


PUBLIC void __dol_set_pag_gim(uint32 val)
{
    DOL_WRITE_BITS (VBPMR2,
        val<< VBPMR2_GIM_SHIFT,
        VBPMR2_GIM_MASK);
    //DOL_DBG:"__dol_set_pag_gim 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_714_112_2_17_23_2_40_97,(uint8*)"d",val);
}

PUBLIC void __dol_set_pag_gi(uint32 val)
{
    DOL_WRITE_BITS (VBCGR10,
        val << VBCGR10_GI_SHIFT,
        VBCGR10_GI_MASK);
    //DOL_DBG:"__dol_set_pag_gi 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_722_112_2_17_23_2_40_98,(uint8*)"d",val);
}

PUBLIC void __dol_set_pag_golr(uint32 val)
{
    DOL_LINEIN_REC_S(val);
    //DOL_DBG:"__dol_set_pag_golr 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_728_112_2_17_23_2_40_99,(uint8*)"d",val);
}

PUBLIC void __dol_set_pag_gobtl(uint32 val)
{
    DOL_WRITE_BITS (VBCGR3,
        val << VBCGR3_GOBTL_SHIFT,
        VBCGR3_GOBTL_MASK);
    //DOL_DBG:"__dol_set_pag_gobtl 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_736_112_2_17_23_2_40_100,(uint8*)"d",val);
}

PUBLIC void __dol_set_pag_lrgod(uint32 uiChannel,uint32 lval,uint32 rval)
{
    // 00:independent;10,11:GODL;01:GODR
    DOL_WRITE_BITS (VBPMR2,
        0,
        VBPMR2_RLGOD_MASK);

    // Left channel
    if (0 == uiChannel)
    {
        DOL_WRITE_BITS (VBCGR1,
            lval << VBCGR1_GODL_SHIFT,
            VBCGR1_GODL_MASK);
    }
    // Right channel
    else if (1 == uiChannel)
    {
        DOL_WRITE_BITS (VBCGR1,
            rval << VBCGR1_GODR_SHIFT,
            VBCGR1_GODR_MASK);
    }
    //DOL_DBG:"__dol_set_pag_lrgod %d 0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_760_112_2_17_23_2_41_101,(uint8*)"ddd",uiChannel,lval,rval);
}

PUBLIC void __dol_set_pag_lrgo(uint32 uiChannel,uint32 lval,uint32 rval)
{
    // 00:independent;01:GOR;10,11:GOL
    DOL_WRITE_BITS (VBCGR8,
        0,
        VBCGR8_LRGO_MASK);

    // Left channel
    if (0 == uiChannel)
    {
        DOL_WRITE_BITS (VBCGR8,
            lval << VBCGR8_GOL_SHIFT,
            VBCGR8_GOL_MASK);
    }
    // Right channel
    else if (1 == uiChannel)
    {

        DOL_WRITE_BITS (VBCGR9,
            rval << VBCGR9_GOR_SHIFT,
            VBCGR9_GOR_MASK);

    }
    //DOL_DBG:"__dol_set_pag_lrgo %d 0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_786_112_2_17_23_2_41_102,(uint8*)"ddd",uiChannel,lval,rval);
}

PUBLIC void __dol_set_pag_lrgob(uint32 uiChannel,uint32 lval,uint32 rval)
{
    // 00:independent;10,11:GOBL;01:GOBR
    DOL_WRITE_BITS (VBCGR2,
        0,
        VBCGR2_LRGOB_MASK);

    // Left channel
    if (0 == uiChannel)
    {
        DOL_WRITE_BITS (VBCGR2,
            lval << VBCGR2_GOBL_SHIFT,
            VBCGR2_GOBL_MASK);
    }
    // Right channel
    else if (1 == uiChannel)
    {
        DOL_WRITE_BITS (VBCGR3,
            rval << VBCGR3_GOBR_SHIFT,
            VBCGR3_GOBR_MASK);
    }
    //DOL_DBG:"__dol_set_pag_lrgob %d 0x%x,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_810_112_2_17_23_2_41_103,(uint8*)"ddd",uiChannel,lval,rval);
}


////////// END OF DOL STATE FUNTION ////////////////////////////////////////////

PUBLIC void __dol_pga_bug(void)
{
    DOL_WRITE_BITS (VBCGR1,
        0x1 << VBCGR1_GODL_SHIFT,
        VBCGR1_GODL_MASK);
    DOL_WRITE_BITS (VBCGR1,
        0x1 << VBCGR1_GODR_SHIFT,
        VBCGR1_GODR_MASK);
    DOL_WRITE_BITS (VBCGR1,
        0x0 << VBCGR1_GODL_SHIFT,
        VBCGR1_GODL_MASK);
    DOL_WRITE_BITS (VBCGR1,
        0x0 << VBCGR1_GODR_SHIFT,
        VBCGR1_GODR_MASK);

}


/*****************************************************************************/
// Description :    Dolphin register dummy read operation. A dummy operation to
//                  dolphin register is must before the real operation.
// Author :         ken.kuang *
// Note :
/*****************************************************************************/
PUBLIC void __dol_reg_dummy ()
{
    volatile uint32 value;

    value = DOL_REG_GET (VBAICR);
    value = DOL_REG_GET (VBAICR);
    value = DOL_REG_GET (VBAICR);
    value = DOL_REG_GET (VBAICR);

    value = DOL_REG_GET (VBCR1);
    value = DOL_REG_GET (VBCR1);
    value = DOL_REG_GET (VBCR1);
    value = DOL_REG_GET (VBCR1);

    value = DOL_REG_GET (VBCR2);
    value = DOL_REG_GET (VBCR2);
    value = DOL_REG_GET (VBCR2);
    value = DOL_REG_GET (VBCR2);

    value = DOL_REG_GET (VBCCR1);
    value = DOL_REG_GET (VBCCR1);
    value = DOL_REG_GET (VBCCR1);
    value = DOL_REG_GET (VBCCR1);

    value = DOL_REG_GET (VBCCR2);
    value = DOL_REG_GET (VBCCR2);
    value = DOL_REG_GET (VBCCR2);
    value = DOL_REG_GET (VBCCR2);

    value = DOL_REG_GET (VBPMR1);
    value = DOL_REG_GET (VBPMR1);
    value = DOL_REG_GET (VBPMR1);
    value = DOL_REG_GET (VBPMR1);

    value = DOL_REG_GET (VBPMR2);
    value = DOL_REG_GET (VBPMR2);
    value = DOL_REG_GET (VBPMR2);
    value = DOL_REG_GET (VBPMR2);

    value = DOL_REG_GET (VBCRR);
    value = DOL_REG_GET (VBCRR);
    value = DOL_REG_GET (VBCRR);
    value = DOL_REG_GET (VBCRR);

    value = DOL_REG_GET (VBICR);
    value = DOL_REG_GET (VBICR);
    value = DOL_REG_GET (VBICR);
    value = DOL_REG_GET (VBICR);

    value = DOL_REG_GET (VBIFR);
    value = DOL_REG_GET (VBIFR);
    value = DOL_REG_GET (VBIFR);
    value = DOL_REG_GET (VBIFR);

    value = DOL_REG_GET (VBCGR1);
    value = DOL_REG_GET (VBCGR1);
    value = DOL_REG_GET (VBCGR1);
    value = DOL_REG_GET (VBCGR1);

    value = DOL_REG_GET (VBCGR2);
    value = DOL_REG_GET (VBCGR2);
    value = DOL_REG_GET (VBCGR2);
    value = DOL_REG_GET (VBCGR2);

    value = DOL_REG_GET (VBCGR3);
    value = DOL_REG_GET (VBCGR3);
    value = DOL_REG_GET (VBCGR3);
    value = DOL_REG_GET (VBCGR3);

    value = DOL_REG_GET (VBCGR8);
    value = DOL_REG_GET (VBCGR8);
    value = DOL_REG_GET (VBCGR8);
    value = DOL_REG_GET (VBCGR8);

    value = DOL_REG_GET (VBCGR9);
    value = DOL_REG_GET (VBCGR9);
    value = DOL_REG_GET (VBCGR9);
    value = DOL_REG_GET (VBCGR9);

    value = DOL_REG_GET (VBCGR10);
    value = DOL_REG_GET (VBCGR10);
    value = DOL_REG_GET (VBCGR10);
    value = DOL_REG_GET (VBCGR10);

    value = DOL_REG_GET (VBTR1);
    value = DOL_REG_GET (VBTR1);
    value = DOL_REG_GET (VBTR1);
    value = DOL_REG_GET (VBTR1);

    value = DOL_REG_GET (VBTR2);
    value = DOL_REG_GET (VBTR2);
    value = DOL_REG_GET (VBTR2);
    value = DOL_REG_GET (VBTR2);
}

/*****************************************************************************/
// Description :    Sets the sample rate register of DAC.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void __dol_set_da_samplerate (uint32 sample_rate)
{
    uint32 da_sr, reg_val;

    //DOL_DBG:"__dol_set_da_samplerate : samplerate: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_940_112_2_17_23_2_41_104,(uint8*)"d", sample_rate);
    if (0 == sample_rate)
    {
        //DOL_PRT:"__dol_set_da_samplerate, invalid DA samplerate: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_943_112_2_17_23_2_41_105,(uint8*)"d", sample_rate);
        return;
    }

    da_sr = __dol_calc_fs(sample_rate);
    reg_val = _dol_get_sample_rate_map (da_sr, 1);
    DOL_WRITE_BITS (VBCCR2, reg_val, VBCCR_DAC_FS_MASK);
}

/*****************************************************************************/
// Description :    Sets the sample rate register of ADC.
//  Author:         ken.kuang
//  Note:
/*****************************************************************************/
PUBLIC void __dol_set_ad_samplerate (uint32 sample_rate)
{
    uint32 ad_sr, reg_val;
    //DOL_DBG:"__dol_set_ad_samplerate : samplerate: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_958_112_2_17_23_2_41_106,(uint8*)"d", sample_rate);

    if (0 == sample_rate)
    {
        //DOL_PRT:"__dol_set_ad_samplerate, invalid AD samplerate: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_962_112_2_17_23_2_41_107,(uint8*)"d", sample_rate);
        return;
    }

    ad_sr = __dol_calc_fs(sample_rate);
    reg_val = _dol_get_sample_rate_map (ad_sr, 0);
    DOL_WRITE_BITS (VBCCR2, reg_val, VBCCR_ADC_FS_MASK);
}

PUBLIC uint32 __dol_reg_rw_test(void)
{
    uint32 reg_val = 0;
    uint32 test_reg_val = 0x1;
    reg_val = DOL_REG_GET(VBCGR1);
    DOL_WRITE_BITS(VBCGR1,0,0xFF);
    if( 0 != DOL_REG_GET(VBCGR1) )
    {
        return 0;
    }
    DOL_WRITE_BITS(VBCGR1,test_reg_val,0xFF);
    if( test_reg_val != DOL_REG_GET(VBCGR1) )
    {
        return 0;
    }
    DOL_WRITE_BITS(VBCGR1,reg_val,0xFF);
    return 1;
}

PUBLIC void __dol_reg_print(void)
{
    //DOL_DBG:"[REG] VBAICR =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_992_112_2_17_23_2_41_108,(uint8*)"d",DOL_REG_GET(VBAICR ));
    //DOL_DBG:"[REG] VBCR1  =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_993_112_2_17_23_2_41_109,(uint8*)"d",DOL_REG_GET(VBCR1  ));
    //DOL_DBG:"[REG] VBCR2  =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_994_112_2_17_23_2_41_110,(uint8*)"d",DOL_REG_GET(VBCR2  ));
    //DOL_DBG:"[REG] VBCCR1 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_995_112_2_17_23_2_41_111,(uint8*)"d",DOL_REG_GET(VBCCR1 ));
    //DOL_DBG:"[REG] VBCCR2 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_996_112_2_17_23_2_41_112,(uint8*)"d",DOL_REG_GET(VBCCR2 ));
    //DOL_DBG:"[REG] VBPMR1 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_997_112_2_17_23_2_41_113,(uint8*)"d",DOL_REG_GET(VBPMR1 ));
    //DOL_DBG:"[REG] VBPMR2 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_998_112_2_17_23_2_41_114,(uint8*)"d",DOL_REG_GET(VBPMR2 ));
    //DOL_DBG:"[REG] VBCRR  =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_999_112_2_17_23_2_41_115,(uint8*)"d",DOL_REG_GET(VBCRR  ));
    //DOL_DBG:"[REG] VBICR  =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1000_112_2_17_23_2_41_116,(uint8*)"d",DOL_REG_GET(VBICR  ));
    //DOL_DBG:"[REG] VBIFR  =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1001_112_2_17_23_2_41_117,(uint8*)"d",DOL_REG_GET(VBIFR  ));
    //DOL_DBG:"[REG] VBCGR1 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1002_112_2_17_23_2_41_118,(uint8*)"d",DOL_REG_GET(VBCGR1 ));
    //DOL_DBG:"[REG] VBCGR2 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1003_112_2_17_23_2_41_119,(uint8*)"d",DOL_REG_GET(VBCGR2 ));
    //DOL_DBG:"[REG] VBCGR3 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1004_112_2_17_23_2_41_120,(uint8*)"d",DOL_REG_GET(VBCGR3 ));
    //DOL_DBG:"[REG] VBCGR8 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1005_112_2_17_23_2_41_121,(uint8*)"d",DOL_REG_GET(VBCGR8 ));
    //DOL_DBG:"[REG] VBCGR9 =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1006_112_2_17_23_2_41_122,(uint8*)"d",DOL_REG_GET(VBCGR9 ));
    //DOL_DBG:"[REG] VBCGR10=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1007_112_2_17_23_2_41_123,(uint8*)"d",DOL_REG_GET(VBCGR10));
    //DOL_DBG:"[REG] VBTR1  =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1008_112_2_17_23_2_41_124,(uint8*)"d",DOL_REG_GET(VBTR1  ));
    //DOL_DBG:"[REG] VBTR2  =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1009_112_2_17_23_2_41_125,(uint8*)"d",DOL_REG_GET(VBTR2  ));

    AUDIO_HAL_PRTMEM ( (0xFFFF0000 
        | (DOL_REG_GET(VBAICR ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFFE0000 
        | (DOL_REG_GET(VBCR1  ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFFD0000 
        | (DOL_REG_GET(VBCR2  ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFFC0000 
        | (DOL_REG_GET(VBCCR1 ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFFB0000 
        | (DOL_REG_GET(VBCCR2 ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFFA0000 
        | (DOL_REG_GET(VBPMR1 ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFF90000 
        | (DOL_REG_GET(VBPMR2 ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFF80000 
        | (DOL_REG_GET(VBCRR  ) & 0x0000FFFF)    ));
    AUDIO_HAL_PRTMEM ( (0xFFF60000 
        | (DOL_REG_GET(VBIFR  ) & 0x0000FFFF)    ));

}

PUBLIC void __dol_exit_from_dsp(void)
{
    //maybe close by dsp.
    //ARM state need open all of this.
    __dol_sb_lout(1);
    __dol_sb_adc(1);
}

PUBLIC void __dol_ext_pa_ctl(uint32 en)
{
    //DOL_DBG:"__dol_ext_pa_ctl:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1042_112_2_17_23_2_41_126,(uint8*)"d",en);
    codec_pa_ctl(en,0);
}

PUBLIC uint32 __dol_is_use_ext_hp(void)
{
    return (1==codec_hp_pa_test_and_get_param(0));
}

PUBLIC uint32 __dol_get_ext_hp_delay_time(void)
{
    uint32 delay_time = 0;
    if( codec_hp_pa_test_and_get_param(&delay_time) )
    {
        return delay_time;
    }
    return 0;
}

PUBLIC void __dol_ext_hp_pa_ctl_inter(uint32 en)
{
    codec_hp_pa_ctl(en);
}

PUBLIC void __dol_ext_hp_pa_ctl(uint32 en)
{
    //DOL_DBG:"__dol_ext_hp_pa_ctl:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_PHY_V1_1048_112_2_17_23_2_41_127,(uint8*)"d",en);
    __dol_ext_hp_pa_ctl_with_mute(en);
}

PUBLIC uint32 __dol_is_arm_ctl(void)
{
    return (DOL_IS_ARM_CTL());
}

PUBLIC uint32 __dol_update_ldo_vol(uint32 cur_vol)
{
    uint32 i = 0;
    uint32 len = sizeof(_vbo_vol_map_tab)/sizeof(_vbo_vol_map_tab[0]);
    for(i=0;i<len;i++)
    {
        if(cur_vol <= _vbo_vol_map_tab[i].cur_vol)
        {
            LDO_SetVoltLevel (LDO_LDO_AVBO, _vbo_vol_map_tab[i].ldo_level);
            return 0;
        }
    }
    return (uint32)(-1);
}

PUBLIC void __dol_switch_to(uint32 arm)
{
    DOL_SWTICH_ARM_CTL(arm);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

