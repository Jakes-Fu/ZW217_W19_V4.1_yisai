/******************************************************************************
 ** File Name:      sprd_codec_phy.c                                          *
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
#include "sprd_codec_phy.h"
#include "audio_cfg.h"

#ifdef CODEC_DEBUG
#define SPRD_CODEC_DBG    SCI_TRACE_LOW
#else
#define SPRD_CODEC_DBG(...)
#endif

PUBLIC uint32 __sprd_codec_init(void)
{
    __sprd_codec_switch_to(1);
    AUDIO_HAL_PRTMEM ( 0xBB030000);
    return 0;
}

PUBLIC void __sprd_codec_en(uint32 en)
{
    if( en )
    {
        __sprd_codec_dp_en(1);
        __sprd_codec_ap_en(1);
        __sprd_codec_update_vcm();
        __sprd_codec_update_ldo();
    }
    else
    {
        __sprd_codec_ap_en(0);
        __sprd_codec_dp_en(0);
    }
}

PUBLIC void __sprd_codec_switch_to(uint32 arm)
{
    SPRD_CODEC_SWTICH_ARM_CTL(arm);
}

PUBLIC void __sprd_codec_clear(uint32 path)
{
    __sprd_codec_dp_clear(path);
    __sprd_codec_ap_clear(path);
}

PUBLIC void __sprd_codec_dac_en(uint32 en,uint32 chan)
{
    __sprd_codec_dp_dac_en(en,chan);
    __sprd_codec_ap_dac_en(en,chan);
}

PUBLIC void __sprd_codec_adc_en(uint32 en,uint32 chan)
{
    __sprd_codec_dp_adc_en(en,chan);
    __sprd_codec_ap_adc_en(en,chan);
}

PUBLIC void __sprd_codec_open(void)
{
    __sprd_codec_dp_sdm_mapping_set(1);
}

PUBLIC void __sprd_codec_reset(void)
{
    //RESET
    SPRD_CODEC_PHY_RESET_M();
}

PUBLIC void __sprd_codec_mclk(uint32 en)
{
    SPRD_CODEC_PHY_MCLK_M(en);
}

PUBLIC void __sprd_codec_register(uint32 en)
{
    SPRD_CODEC_PHY_REG_EN(en);
}

PUBLIC void __sprd_codec_hp_mute(uint32 is_mute)
{
#ifdef SKIP_SMOOTH_ADJ
	// Don't do this on SC6531EFM, HP mute will cause pop noise
	return;
#endif
    //SPRD_CODEC_DBG:"__sprd_codec_hp_mute:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_PHY_99_112_2_17_23_2_53_192,(uint8*)"d",is_mute);
    if( is_mute )
    {
        __sprd_codec_ap_hp_mute();
#if !(PGA_SMOOTH_ADJUST)
        __sprd_codec_ext_hp_pa_ctl(0);
        if( !__sprd_codec_is_use_ext_hp() )
            __sprd_codec_ap_hp(0,AUDIO_ALL_CHAN);
#endif
    }
    else
    {
        __sprd_codec_ap_hp(1,AUDIO_ALL_CHAN);
        __sprd_codec_ext_hp_pa_ctl(1);
    }
}


PUBLIC void __sprd_codec_ep_mute(uint32 is_mute)
{
    //SPRD_CODEC_DBG:"__sprd_codec_ep_mute:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_PHY_116_112_2_17_23_2_53_193,(uint8*)"d",is_mute);
    if( is_mute )
    {
        __sprd_codec_ap_ear_mute();
#if !(PGA_SMOOTH_ADJUST)
        __sprd_codec_ap_ear(0);
#endif
    }
    else
    {
        __sprd_codec_ap_ear(1);
    }
}

PUBLIC void __sprd_codec_pa_mute(uint32 is_mute)
{
    SPRD_CODEC_DBG("__sprd_codec_pa_mute:%d",is_mute);
    if( is_mute )
    {
        __sprd_codec_ap_pa_mute();
#if !(PGA_SMOOTH_ADJUST)
        __sprd_codec_ext_pa_ctl(0,0);
        __sprd_codec_ap_lout(0);
#endif
    }
    else
    {
        __sprd_codec_ap_lout(1);
        __sprd_codec_ext_pa_ctl(1,0);
    }
}

PUBLIC void __sprd_codec_ext_pa_ctl(uint32 en,uint32 id)
{
    //SPRD_CODEC_DBG:"__sprd_codec_ext_pa_ctl:%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_PHY_131_112_2_17_23_2_53_194,(uint8*)"dd",en,id);
    codec_pa_ctl(en,id);
}

PUBLIC uint32 __sprd_codec_is_use_ext_hp(void)
{
    return (1==codec_hp_pa_test_and_get_param(0));
}

PUBLIC uint32 __sprd_codec_get_ext_hp_delay_time(void)
{
    uint32 delay_time = 0;
    if( codec_hp_pa_test_and_get_param(&delay_time) )
    {
        return delay_time;
    }
    return 0;
}

PUBLIC void __sprd_codec_ext_hp_pa_ctl_inter(uint32 en)
{
    codec_hp_pa_ctl(en);
}

PUBLIC void __sprd_codec_ext_hp_pa_ctl(uint32 en)
{
    __sprd_codec_ext_hp_pa_ctl_with_mute(en);
}

PUBLIC uint32 __sprd_codec_is_arm_ctl(void)
{
    return (SPRD_CODEC_IS_ARM_CTL());
}


PUBLIC uint32 __sprd_codec_update_ldo_vol(uint32 cur_vol)
{
    uint32 ret = 0;
    //SPRD_CODEC_DBG:"__sprd_codec_update_ldo_vol %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_PHY_150_112_2_17_23_2_53_196,(uint8*)"d",cur_vol);

    if( __sprd_codec_update_pa_ldo_vol(cur_vol) )
    {
        ret = (uint32)-1;
    }
    
    return ret;
}

PUBLIC void CODEC_SetHeadphoneConnectStatus(BOOLEAN status)
{
#if defined(PLATFORM_SC6531EFM)   //|| defined(PLATFORM_UWS6121E)
    SPRD_CODEC_DBG("headphone_connect_status:%d", status);
    __sprd_codec_set_headset_connection_status(status);
#endif
}

