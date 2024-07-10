#include "os_api.h"
#include "sprd_codec_phy_v2.h"
#include "adi_hal_internal.h"

//SC2720_ANA_PMU0
PRIVATE SC2720_ANA_PMU0_T g_SC2720_ANA_PMU0;

//SC2720_ANA_HDT0
PRIVATE SC2720_ANA_HDT0_T g_SC2720_ANA_HDT0;

//SC2720_ANA_HDT2
PRIVATE SC2720_ANA_HDT2_T g_SC2720_ANA_HDT2;

PRIVATE BOOL aud_CodecSC2720HeadmicConfirm(void)
{
    SC2720_ANA_STS7_T STS7_Status;

    STS7_Status.reg = ADI_Analogdie_reg_read(SC2720_ANA_STS7);

    return STS7_Status.reg_t.HEAD_INSERT;
}

PRIVATE BOOL aud_CodecSC2720HeadsetButtonConfirm(void)
{
    SC2720_ANA_STS7_T STS7_Status;

    STS7_Status.reg = ADI_Analogdie_reg_read(SC2720_ANA_STS7);

    return STS7_Status.reg_t.HEAD_BUTTON;
}

PUBLIC VOID aud_CodecSC2720HeadsetMicDet(BOOL on)
{
    if(on)
    {
        g_SC2720_ANA_PMU0.reg = ADI_Analogdie_reg_read(SC2720_ANA_PMU0);
        g_SC2720_ANA_PMU0.reg_t.HMIC_BIAS_EN = 1;
        g_SC2720_ANA_PMU0.reg_t.HMIC_SLEEP_EN = 1;
        g_SC2720_ANA_PMU0.reg_t.VB_EN = 1;
        g_SC2720_ANA_PMU0.reg_t.VB_HDMC_SP_PD = 0;
        ADI_Analogdie_reg_write(SC2720_ANA_PMU0, g_SC2720_ANA_PMU0.reg);

        g_SC2720_ANA_HDT0.reg = ADI_Analogdie_reg_read(SC2720_ANA_HDT0); 
        g_SC2720_ANA_HDT0.reg_t.HEDET_MICDET_EN = 1;
        ADI_Analogdie_reg_write(SC2720_ANA_HDT0, g_SC2720_ANA_HDT0.reg);

        g_SC2720_ANA_HDT0.reg_t.HEDET_BDET_EN = 1;
        ADI_Analogdie_reg_write(SC2720_ANA_HDT0, g_SC2720_ANA_HDT0.reg);

        g_SC2720_ANA_HDT2.reg = ADI_Analogdie_reg_read(SC2720_ANA_HDT2);
        g_SC2720_ANA_HDT2.reg_t.HEDET_BDET_REF_SEL=SC2720_RG_AUD_HEDET_BDET_REF_SEL(9);
        ADI_Analogdie_reg_write(SC2720_ANA_HDT2, g_SC2720_ANA_HDT2.reg);
    }
    else
    {
        g_SC2720_ANA_HDT0.reg = ADI_Analogdie_reg_read(SC2720_ANA_HDT0);
        g_SC2720_ANA_HDT0.reg_t.HEDET_BDET_EN = 0;
        ADI_Analogdie_reg_write(SC2720_ANA_HDT0, g_SC2720_ANA_HDT0.reg);

        g_SC2720_ANA_HDT0.reg_t.HEDET_MICDET_EN = 0;
        ADI_Analogdie_reg_write(SC2720_ANA_HDT0, g_SC2720_ANA_HDT0.reg);

        g_SC2720_ANA_HDT2.reg = ADI_Analogdie_reg_read(SC2720_ANA_HDT2);
        g_SC2720_ANA_HDT2.reg_t.HEDET_BDET_REF_SEL=SC2720_RG_AUD_HEDET_BDET_REF_SEL(0);
        ADI_Analogdie_reg_write(SC2720_ANA_HDT2, g_SC2720_ANA_HDT2.reg);

        g_SC2720_ANA_PMU0.reg = ADI_Analogdie_reg_read(SC2720_ANA_PMU0);
        g_SC2720_ANA_PMU0.reg_t.HMIC_BIAS_EN = 0;        
        ADI_Analogdie_reg_write(SC2720_ANA_PMU0, g_SC2720_ANA_PMU0.reg);
    }
}

PUBLIC  AUD_HEADSET_TYPE_T aud_CodecSC2720HeadsetMicAndButtonDetect(VOID)
{
    AUD_HEADSET_TYPE_T errStatus = AUD_HEADSET_TYPE_INSERT3P;
    UINT32 times_delay = 0;
    UINT32 num_3p = 0;
    UINT32 num_4p = 0;

    aud_CodecSC2720HeadsetMicDet(TRUE);

    while((times_delay <= 3000) && (!aud_CodecSC2720HeadmicConfirm()))
    {
        SCI_Sleep(1);
        times_delay++;
        //SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect aud_CodecSC2720HeadmicConfirm times_delay:%d ms\n", times_delay);
    }
    SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect aud_CodecSC2720HeadmicConfirm times_delay:%d ms\n", times_delay);
    if(times_delay >= 3000)
    {
        SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect fail\n");
        aud_CodecSC2720HeadsetMicDet(FALSE);
        errStatus = AUD_HEADSET_TYPE_INSERT3P;
        return errStatus;
    }

    if(times_delay >= 20)
    {
        SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect this headset is 4P\n");        
        errStatus = AUD_HEADSET_TYPE_INSERT4P;
        return errStatus;
    }

    times_delay = 0;

   while((times_delay <= 50) )
    {
        if(aud_CodecSC2720HeadsetButtonConfirm())
         {
             num_3p++;
             //SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect num_3p:%d\n", num_3p);
         }
         else
         {
             num_4p++;
             //SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect num_4p:%d\n", num_4p);
         }

         SCI_Sleep(1);
         times_delay++;
    }
    SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect num_3p:%d\n", num_3p);
    SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect num_4p:%d\n", num_4p);

    if(num_3p >= 10)
    {
        SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect this headset is 3P\n");
        aud_CodecSC2720HeadsetMicDet(FALSE);
        errStatus = AUD_HEADSET_TYPE_INSERT3P;
    }
    else
    {
        SCI_TRACE_LOW("aud_CodecSC2720HeadsetMicAndButtonDetect this headset is 4P\n");
        errStatus = AUD_HEADSET_TYPE_INSERT4P;
    }

    return errStatus;
}

