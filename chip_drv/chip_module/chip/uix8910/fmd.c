#include "spzip_api.h"
#include "efs_comm.h" 
#include "nv_type.h"
#include "wcn_fm_dsp.h"
#include "wcn_rf_if.h"
#include "fmd.h"

#ifdef FM_S_ANT_SUPPORT
extern BOOLEAN CHG_PHY_IsChargerPresent(void);
#endif

typedef enum
{
   ///87.5-108m  (US)
   FMD_BAND_US_EUROPE=0,
   ///76-91M       (JAPAN)
   FMD_BAND_JAPAN,
   ///76-108M
   FMD_BAND_WORLD,
   FMD_BAND_EAST_EUPORE,
   FMD_BAND_QTY
}FMD_BAND_T;

// DATAPATH_CTRL3 interval
const UINT16 g_halFMDsp_Datapath_Ctrl3_Interval = 0x2eff;

// DATAPATH_CTRL3 step19
const UINT16 g_halFMDsp_Datapath_Ctrl3_Step19 = 0x3A5E;

// NOISE_CANCEL_CTRL1
const UINT8 g_halFMDsp_Noise_Cancel_Ctrl3_Fircut = 4;

// DATAPATH_CTRL0 gainct
const UINT8 g_halFMDsp_Datapath_Ctrl0_Gainct = 0x88;

//AFC_CTRL afc_ct_sk
const UINT8 g_halFMDsp_Afc_Ctrl_Afc_Ct_Sk = 0x3F;

//AFC_CTRL afc_range
const UINT8 g_halFMDsp_Afc_Ctrl_Afc_Range = 0x32;

//AFC_CTRL noise_cancel_ctrl2
const UINT16 g_halFMDsp_Noise_Cancel_Ctrl2 = 0x7FFF;

//DATAPATH_CTRL1  SOFTMUTE RSSI
const UINT8 g_halFMDsp_Datapath_Ctrl1_SoftMute_Th1 = 0x7;

//DATAPATH_CTRL1  SOFTMUTE NIOSE
const UINT8 g_halFMDsp_Datapath_Ctrl1_SoftMute_Th2 = 0x5;

//DATAPATH_CTRL1  SOFTMUTE SNR
const UINT8 g_halFMDsp_Datapath_Ctrl1_SoftMute_Th3 = 0xd;

//DATAPATH_CTRL2  SOFTMUTE offset
const UINT8 g_halFMDsp_Datapath_Ctrl2_SoftMute_Th4 = 0x32;

//DATAPATH_CTRL2  SmRate
const UINT8 g_halFMDsp_Datapath_Ctrl2_SmRate = 0;

//DATAPATH_CTRL2  SmAttenu
const UINT8 g_halFMDsp_Datapath_Ctrl2_SmAttenu = 0xf;

//DATAPATH_CTRL2  Thsoftblend1
const UINT8 g_halFMDsp_Datapath_Ctrl2_ThSoftBlend1 = 0;

//DATAPATH_CTRL2  Thsoftblend2
const UINT8 g_halFMDsp_Datapath_Ctrl2_ThSoftBlend2 = 0x1E;

//DATAPATH_CTRL2  deemph hcc
const UINT8 g_halFMDsp_Datapath_Ctrl2_Deemph_Hcc = 0;

//DATAPATH_CTRL2  Lpf_bw
const UINT8 g_halFMDsp_Datapath_Ctrl2_Lpf_Bw = 0x5;

//CTRL  DeemphMode
const UINT8 g_halFMDsp_Ctrl_DeemphMode = FM_DSP_DEEMPH_MODE_75US;

#define BASE_FREQ_USEUROPE  87000
#define BASE_FREQ_JAPAN     76000
#define BASE_FREQ_WORLD     76000
#define BASE_FREQ_EAST_EUROPE     65000

UINT32 g_baseFreq[FMD_BAND_QTY] = {BASE_FREQ_USEUROPE, BASE_FREQ_JAPAN, BASE_FREQ_WORLD, BASE_FREQ_EAST_EUROPE};
UINT32 g_chanSpacing[3] = {100,200,50};
FMD_BAND_T g_band = FMD_BAND_US_EUROPE;

static uint8 s_fm_record_index = 0;
static uint32 s_fm_record_freq[206] = {0};
static uint8 s_fm_record_rssi[206] = {0};
static uint8 s_fm_record_offset[206] = {0};
static uint8 s_fm_record_noise[206] = {0};
static uint8 s_fm_record_snr_out[206] = {0};
static uint8 s_fm_record_snr19k_out[206] = {0};
static uint8 s_fm_record_pilot[206] = {0};
static uint8 s_fm_record_condition1[206] = {0};
static uint8 s_fm_record_condition2[206] = {0};
static uint8 s_fm_record_condition3[206] = {0};
static uint8 s_fm_record_condition4[206] = {0};

extern uint8 g_fm_longant_condition1_rssi;
extern uint8 g_fm_longant_condition1_offset;
extern uint8 g_fm_longant_condition1_noise;
extern uint8 g_fm_longant_condition2_rssi;
extern uint8 g_fm_longant_condition2_offset;
extern uint8 g_fm_longant_condition2_noise;
extern uint8 g_fm_longant_condition3_rssi;
extern uint8 g_fm_longant_condition3_offset;
extern uint8 g_fm_longant_condition3_noise;
extern uint8 g_fm_longant_condition4_rssi;
extern uint8 g_fm_longant_condition4_offset;
extern uint8 g_fm_longant_condition4_noise;
#ifdef FM_S_ANT_SUPPORT
extern uint8 g_fm_shortant_condition1_rssi;
extern uint8 g_fm_shortant_condition1_offset;
extern uint8 g_fm_shortant_condition1_noise;
extern uint8 g_fm_shortant_condition2_rssi;
extern uint8 g_fm_shortant_condition2_offset;
extern uint8 g_fm_shortant_condition2_noise;
extern uint8 g_fm_shortant_condition3_rssi;
extern uint8 g_fm_shortant_condition3_offset;
extern uint8 g_fm_shortant_condition3_noise;
#endif

#define FM_E  1

void rdafm_Start_Fm_Band(void)
{
    hwp_rf_if->fm_control  &= ~WCN_RF_IF_FM_BAND_SEL_MASK;
    hwp_rf_if->fm_control  |= WCN_RF_IF_FM_BAND_SEL(g_band);
}

void rdafm_Start_Fm_Chn(uint32 chn)
{
    double internal_hex,freq_adc,step_19k,freq_vco,freq;
    uint8 div_fm_lo_clk,div_fm_lo_clk2;
    uint16 div;

    if(chn == 104000)
    {
        hwp_fm_dsp->datapath_ctrl4 = 0x27a933;
        hwp_fm_dsp->datapath_ctrl5 = 0x24dec;
    }
    else if(chn == 104100)
    {
         hwp_fm_dsp->datapath_ctrl4 = 0x27FC81;
         hwp_fm_dsp->datapath_ctrl5 = 0x779D;
    }
    else if(chn == 103900)
    {
         hwp_fm_dsp->datapath_ctrl4 = 0x26EB39;
         hwp_fm_dsp->datapath_ctrl5 = 0x40425;
    }
    else
    {
        hwp_fm_dsp->datapath_ctrl4 |= FM_DSP_NOTCH_FLT_BYPASS;
    }
    hwp_rf_if->fm_control &= ~WCN_RF_IF_FM_TUNE;
    //hwp_rf_if->power_reg &= ~WCN_RF_IF_PU_ADC_REG;
    //div= (chn -g_baseFreq[g_band])/100*4;
    div = (chn -g_baseFreq[g_band])/100*4-1;// 150K
    hwp_rf_if->fm_control  &= ~WCN_RF_IF_FM_CHAN_REG_MASK;
    hwp_rf_if->fm_control |= WCN_RF_IF_FM_CHAN_REG(div);
    hwp_rf_if->fm_control |= WCN_RF_IF_FM_TUNE;
}

void  fmd_ValidStop(UINT32 freqKHz)
{
    UINT8 result;
    UINT32 errStatus;
    UINT8 i = 0;

    rdafm_Start_Fm_Chn(freqKHz);
/*
	//waiting for FmReady
	do
	{
		i++;
		if(i>4) return; 
		SCI_Sleep(10);
		errStatus = hwp_fm_dsp->status0;
	} while((errStatus&FM_DSP_SEEK_READY)==0);	
    SCI_TRACE_LOW("fmd_ValidStop_freqKHz=%d",freqKHz);
    if ((errStatus & FM_DSP_SEEK_DONE) == 0)
    {
        SCI_TRACE_LOW("FM_DSP_SEEK_DONE");
     
    }
    else
    {
       SCI_TRACE_LOW("FM_DSP_SEEK_NOTDONE");
    }
    
*/
}

#define FMD_TEST_COUNT 40
PUBLIC UINT8 fmd_ValidRecord(uint32 freq)
{
    UINT8 result;
    UINT32 errStatus;
    UINT8 i = 0;
    UINT32 tmp_offset = 0,tmp_offset_sum = 0;
    UINT32 tmp_noise = 0,tmp_noise_sum = 0;
    UINT32 tmp_rssi = 0,tmp_rssi_sum = 0;
    UINT32 tmp_snr_out = 0,tmp_snr_out_sum = 0;
    UINT32 tmp_snr19k_out = 0,tmp_snr19k_out_sum = 0;
    UINT32 tmp_pilot = 0,tmp_pilot_sum = 0;
    UINT8 FMD_TestBak[12] = {0};
#ifdef FM_S_ANT_SUPPORT
    BOOLEAN ear_detect = FALSE;
    BOOLEAN usb_flag = FALSE;
#endif

    //waiting for Fm Ready
    do
    {
        i++;
        if(i>3)
        {
            return 0;
        }
        SCI_Sleep(10);
        errStatus = hwp_fm_dsp->status0;
    }while((errStatus&FM_DSP_SEEK_READY)==0);	

    for(i=0;i<12;i++)
    {
        FMD_TestBak[i] = 0;
    }

    for (i=0; i<FMD_TEST_COUNT+2; i++)
    { 
        tmp_offset = ((hwp_fm_dsp->status0 & FM_DSP_OFFSET2_MASK)>>FM_DSP_OFFSET2_SHIFT);
        tmp_snr_out = ((hwp_fm_dsp->status0 & FM_DSP_SNR_OUT_MASK)>>FM_DSP_SNR_OUT_SHIFT);
        tmp_noise = ((hwp_fm_dsp->status1 & FM_DSP_NOISE_DB2_MASK)>>FM_DSP_NOISE_DB2_SHIFT);
        tmp_snr19k_out = ((hwp_fm_dsp->status1 & FM_DSP_SNR19K_OUT_MASK)>>FM_DSP_SNR19K_OUT_SHIFT);
        tmp_pilot = ((hwp_fm_dsp->status1 & FM_DSP_PILOT_DB2_MASK)>>FM_DSP_PILOT_DB2_SHIFT);
        tmp_rssi    = ((hwp_fm_dsp->status0 & FM_DSP_RSSI_DB2_MASK)>>FM_DSP_RSSI_DB2_SHIFT);
        tmp_offset_sum += tmp_offset;
        tmp_noise_sum += tmp_noise;
        tmp_rssi_sum += tmp_rssi;
        tmp_snr_out_sum += tmp_snr_out;
        tmp_snr19k_out_sum += tmp_snr19k_out;
        tmp_pilot_sum += tmp_pilot;

        if(FMD_TestBak[0] ==0)
        {
            FMD_TestBak[0] = FMD_TestBak[1] = tmp_offset;
        }
        else
        {
            FMD_TestBak[0] = MIN(tmp_offset,FMD_TestBak[0]);
            FMD_TestBak[1] = MAX(tmp_offset,FMD_TestBak[1]);
        }

        if(FMD_TestBak[2] ==0)
        {
            FMD_TestBak[2] = FMD_TestBak[3] = tmp_noise;
        }
        else
        {
            FMD_TestBak[2] = MIN(tmp_noise,FMD_TestBak[2]);
            FMD_TestBak[3] = MAX(tmp_noise,FMD_TestBak[3]);
        }

        if(FMD_TestBak[4] ==0)
        {
            FMD_TestBak[4] = FMD_TestBak[5] = tmp_rssi;
        }
        else
        {
            FMD_TestBak[4] = MIN(tmp_rssi,FMD_TestBak[4]);
            FMD_TestBak[5] = MAX(tmp_rssi,FMD_TestBak[5]);
        }

        if(FMD_TestBak[6] ==0)
        {
            FMD_TestBak[6] = FMD_TestBak[7] = tmp_snr_out;
        }
        else
        {
            FMD_TestBak[6] = MIN(tmp_snr_out,FMD_TestBak[6]);
            FMD_TestBak[7] = MAX(tmp_snr_out,FMD_TestBak[7]);
        }

        if(FMD_TestBak[8] ==0)
        {
            FMD_TestBak[8] = FMD_TestBak[9] = tmp_snr19k_out;
        }
        else
        {
            FMD_TestBak[8] = MIN(tmp_snr19k_out,FMD_TestBak[8]);
            FMD_TestBak[9] = MAX(tmp_snr19k_out,FMD_TestBak[9]);
        }

        if(FMD_TestBak[10] ==0)
        {
            FMD_TestBak[10] = FMD_TestBak[11] = tmp_snr19k_out;
        }
        else
        {
            FMD_TestBak[10] = MIN(tmp_snr19k_out,FMD_TestBak[10]);
            FMD_TestBak[11] = MAX(tmp_snr19k_out,FMD_TestBak[11]);
        }
    }

    tmp_offset_sum -= FMD_TestBak[0];
    tmp_offset_sum -= FMD_TestBak[1];
    tmp_noise_sum -= FMD_TestBak[2];
    tmp_noise_sum -= FMD_TestBak[3];
    tmp_rssi_sum -= FMD_TestBak[4];
    tmp_rssi_sum -= FMD_TestBak[5];
    tmp_snr_out_sum -= FMD_TestBak[6];
    tmp_snr_out_sum -= FMD_TestBak[7];
    tmp_snr19k_out_sum -= FMD_TestBak[8];
    tmp_snr19k_out_sum -= FMD_TestBak[9];
    tmp_pilot_sum -= FMD_TestBak[10];
    tmp_pilot_sum -= FMD_TestBak[11];

    tmp_offset = tmp_offset_sum/FMD_TEST_COUNT;
    tmp_noise = tmp_noise_sum/FMD_TEST_COUNT;
    tmp_rssi = tmp_rssi_sum/FMD_TEST_COUNT;
    tmp_snr_out = tmp_snr_out_sum/FMD_TEST_COUNT;
    tmp_snr19k_out = tmp_snr19k_out_sum/FMD_TEST_COUNT;
    tmp_pilot = tmp_pilot_sum/FMD_TEST_COUNT;

#if 1
    s_fm_record_freq[s_fm_record_index] = freq-100;
    s_fm_record_rssi[s_fm_record_index] = tmp_rssi;
    s_fm_record_offset[s_fm_record_index] = tmp_offset;
    s_fm_record_noise[s_fm_record_index] = tmp_noise;
    s_fm_record_snr_out[s_fm_record_index] = tmp_snr_out;
    s_fm_record_snr19k_out[s_fm_record_index] = tmp_snr19k_out;
    s_fm_record_pilot[s_fm_record_index] = tmp_pilot;
    s_fm_record_condition1[s_fm_record_index] = 0;
    s_fm_record_condition2[s_fm_record_index] = 0;
    s_fm_record_condition3[s_fm_record_index] = 0;
    s_fm_record_condition4[s_fm_record_index] = 0;
#endif

#ifdef FM_S_ANT_SUPPORT
    ear_detect = GPIO_CheckHeadsetStatus();
    if (ear_detect)
#endif
    {
#if 1
        if (((tmp_noise < g_fm_longant_condition1_noise) && (tmp_offset < g_fm_longant_condition1_offset) && (tmp_rssi > g_fm_longant_condition1_rssi)) 
			|| ((tmp_noise < g_fm_longant_condition2_noise) && (tmp_offset < g_fm_longant_condition2_offset) && (tmp_rssi > g_fm_longant_condition2_rssi)) 
			|| ((tmp_noise < g_fm_longant_condition3_noise) && (tmp_offset < g_fm_longant_condition3_offset)) 
			|| ((tmp_offset < g_fm_longant_condition4_offset) && (tmp_rssi > g_fm_longant_condition4_rssi)))
        {
            result = TRUE;

#if 1
            if ((tmp_noise < g_fm_longant_condition1_noise) && (tmp_offset < g_fm_longant_condition1_offset) && (tmp_rssi > g_fm_longant_condition1_rssi))
            {
                s_fm_record_condition1[s_fm_record_index] = 1;
            }

            if ((tmp_noise < g_fm_longant_condition2_noise) && (tmp_offset < g_fm_longant_condition2_offset) && (tmp_rssi > g_fm_longant_condition2_rssi))
            {
                s_fm_record_condition2[s_fm_record_index] = 1;
            }

            if ((tmp_noise < g_fm_longant_condition3_noise) && (tmp_offset < g_fm_longant_condition3_offset))
            {
                s_fm_record_condition3[s_fm_record_index] = 1;
            }

            if ((tmp_offset < g_fm_longant_condition4_offset) && (tmp_rssi > g_fm_longant_condition4_rssi))
            {
                s_fm_record_condition4[s_fm_record_index] = 1;
            }
#endif
        }
        else 
        {
            result = FALSE;
        }
#else
        if (((tmp_noise < 68) && (tmp_offset < 11) && (tmp_rssi > 35)) ||  \
			((tmp_noise < 60) && (tmp_offset < 11)) || ( (tmp_offset < 11) && (tmp_rssi > 45)))
        {
            result = TRUE;
        }
        else 
        {
            result = FALSE;
        }
#endif
    }
#ifdef FM_S_ANT_SUPPORT
/*
    else
    {
        if (((tmp_noise < 64) && (tmp_offset < 30) && (tmp_rssi > 45)) || ((tmp_noise < 67) && (tmp_offset < 11) && (tmp_rssi > 40)) ||  \
        ((tmp_noise < 60) && (tmp_offset < 11)) || ( (tmp_offset < 11) && (tmp_rssi > 54)) )
        {
            result = TRUE;
        }
        else 
        {
            result = FALSE;
        }
    }
*/
    else
    {
        usb_flag = CHG_PHY_IsChargerPresent();
        if (usb_flag)
        {
            if ((((tmp_noise <= 69) && (tmp_offset < 20) && (tmp_rssi > 35)) 
    			|| ((tmp_noise < 60) && (tmp_offset < 11)) 
    			|| ((tmp_offset < 11) && (tmp_rssi > 45))) 
    			&& ((0 != tmp_snr19k_out) || (tmp_offset <= 2)) 
    			&& ((tmp_pilot > 40) || ((0 == tmp_offset) && (0 == tmp_snr19k_out))))
            {
                result = TRUE;

#if 1
                if (((tmp_noise <= 69) && (tmp_offset < 20) && (tmp_rssi > 35)) 
    			|| ((tmp_noise < 60) && (tmp_offset < 11)) 
    			|| ((tmp_offset < 11) && (tmp_rssi > 45)))
                {
                    s_fm_record_condition1[s_fm_record_index] = 1;
                }
                if ((0 != tmp_snr19k_out) || (tmp_offset <= 2))
                {
                    s_fm_record_condition2[s_fm_record_index] = 1;
                }
                if ((tmp_pilot > 40) || ((0 == tmp_offset) && (0 == tmp_snr19k_out)))
                {
                    s_fm_record_condition3[s_fm_record_index] = 1;
                }
#endif
            }
            else 
            {
                result = FALSE;
            }
        }
        else
        {
            if (((tmp_noise < g_fm_shortant_condition1_noise) && (tmp_offset < g_fm_shortant_condition1_offset) && (tmp_rssi > g_fm_shortant_condition1_rssi)) 
    			|| ((tmp_noise < g_fm_shortant_condition2_noise) && (tmp_offset < g_fm_shortant_condition2_offset)) 
    			|| ((tmp_offset < g_fm_shortant_condition3_offset) && (tmp_rssi > g_fm_shortant_condition3_rssi)))
            {
                result = TRUE;

#if 1
                if ((tmp_noise < g_fm_shortant_condition1_noise) && (tmp_offset < g_fm_shortant_condition1_offset) && (tmp_rssi > g_fm_shortant_condition1_rssi))
                {
                    s_fm_record_condition1[s_fm_record_index] = 1;
                }
                if ((tmp_noise < g_fm_shortant_condition2_noise) && (tmp_offset < g_fm_shortant_condition2_offset))
                {
                    s_fm_record_condition2[s_fm_record_index] = 1;
                }
                if ((tmp_offset < g_fm_shortant_condition3_offset) && (tmp_rssi > g_fm_shortant_condition3_rssi))
                {
                    s_fm_record_condition3[s_fm_record_index] = 1;
                }
#endif

                if ((tmp_offset < g_fm_shortant_condition3_offset) && (tmp_rssi > g_fm_shortant_condition3_rssi))//bug 1104738
                {
                    if ((tmp_rssi > 35) && (tmp_noise > 69))//Both the signal and the noise are strong 
                    {
                        result = FALSE;
#if 1
                        s_fm_record_condition3[s_fm_record_index] = 0xFF;
#endif
                    }
                }
            }
            else 
            {
                result = FALSE;
            }
        }
    }
#endif

#ifdef FM_S_ANT_SUPPORT
    SCI_TRACE_LOW("FM fmd_ValidRecord %d, %d, ear_detect=%d, usb=%d, noise=%d, offset=%d, rssi=%d, snr=%d, snr19k=%d, pilot=%d",result,(freq-100),ear_detect,usb_flag,tmp_noise,tmp_offset,tmp_rssi,tmp_snr_out,tmp_snr19k_out,tmp_pilot);
#else
    SCI_TRACE_LOW("FM fmd_ValidRecord %d, %d, noise=%d, offset=%d, rssi=%d",result,(freq-100),tmp_noise,tmp_offset,tmp_rssi);
#endif

#if 1
    s_fm_record_index++;
    if (s_fm_record_index >= 206)
    {
        s_fm_record_index = 0;
    }
#endif

    return result;
}

void hal_Fm_Dsp_Enable_IQSwitchBeforeMixer(bool on) 
{
    if(on)
        hwp_fm_dsp->ctrl |= FM_DSP_IMGREJ_DSP;
    else
	hwp_fm_dsp->ctrl &= ~FM_DSP_IMGREJ_DSP;
}


void hal_Fm_Dsp_Disable_AFC(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |=  FM_DSP_AFC_DISABLE_DISABLE;	
    }
    else
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_AFC_DISABLE;
	 hwp_fm_dsp->ctrl |=  FM_DSP_AFC_DISABLE_ENABLE;
    }
}

void hal_Fm_Dsp_Enable_SoftMute(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_SOFTMUTE_EN_ENABLE;
    }
    else
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_SOFTMUTE_EN;
        hwp_fm_dsp->ctrl |= FM_DSP_SOFTMUTE_EN_DISABLE;
    }
}

void hal_Fm_Dsp_Enable_Softblend(bool on)
{
    if(on)
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_SOFTBLEND_OFF;
	 hwp_fm_dsp->ctrl |= FM_DSP_SOFTBLEND_OFF_ENABLE;			
    }
    else
    {
        hwp_fm_dsp->ctrl |= FM_DSP_SOFTBLEND_OFF_DISABLE;
    }
}

void hal_Fm_Dsp_Enable_Mute(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_MUTE_MUTE;
    }
    else
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_MUTE;
        hwp_fm_dsp->ctrl |= FM_DSP_MUTE_NORMAL;
    }
}

void hal_FM_DSP_Deemph_Bypass(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_DEEMPH_BYPASS;
        
    }
    else
    {
        hwp_fm_dsp->ctrl |= FM_DSP_DEEMPH_BYPASS;
    }
}

void hal_FM_DSP_LPF_Bypass(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl &= ~FM_DSP_LPF_BYPASS;
        
    }
    else
    {
        hwp_fm_dsp->ctrl |= FM_DSP_LPF_BYPASS;
    }
}

void hal_FM_DSP_Enable_I2s(bool on) 
{
    if(on)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_I2S_EN_ENABLE;
        
    }
    else
    {
        hwp_fm_dsp->ctrl &= FM_DSP_I2S_EN;
	 hwp_fm_dsp->ctrl |= FM_DSP_I2S_EN_DISABLE;
    }
}

void hal_FM_DSP_Fmdem_Sel(bool sel) 
{
    if(sel)
    {
        hwp_fm_dsp->ctrl |= FM_DSP_FMDEM_SRC_SEL_OUTPUT_DIG_GAIN;
        
    }
    else
    {
        hwp_fm_dsp->ctrl &= FM_DSP_FMDEM_SRC_SEL;
	 hwp_fm_dsp->ctrl |= FM_DSP_FMDEM_SRC_SEL_OUTPUT_LPFIL;
    }
}

void hal_FM_DSP_Set_All_SelFircut(UINT8 val) 
{
    hwp_fm_dsp->noise_cancel_ctrl1 &= ~(FM_DSP_SEL_FIRCUT_SK_MASK|FM_DSP_SEL_FIRCUT3_MASK|FM_DSP_SEL_FIRCUT2_MASK|FM_DSP_SEL_FIRCUT1_MASK);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT_SK(val*0x9);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT3(val);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT2(val);
    hwp_fm_dsp->noise_cancel_ctrl1 |= FM_DSP_SEL_FIRCUT1(val);
}

void hal_Fm_Dsp_Seek_Ctrl_Init(void)
{
    hwp_fm_dsp->seek_ctrl0 &= ~(FM_DSP_SK_CMP_GRP_EN_MASK | FM_DSP_SNR_CNT_TH_MASK);
    hwp_fm_dsp->seek_ctrl0 |= FM_DSP_SK_CMP_GRP_EN(0x0b) | FM_DSP_SNR_CNT_TH(30);
    hwp_fm_dsp->seek_ctrl1 &= ~(0xffffff);
    hwp_fm_dsp->seek_ctrl1 |= FM_DSP_OFFSET_CNT_TH(80) | FM_DSP_NOISE_CNT_TH(20) | FM_DSP_RSSI_CNT_TH(20);
    hwp_fm_dsp->seek_ctrl2 &= ~(FM_DSP_RSSI_H_TH_MASK|FM_DSP_RSSI_L_TH_MASK|FM_DSP_NOISE_H_TH_MASK|FM_DSP_NOISE_L_TH_MASK);
    hwp_fm_dsp->seek_ctrl2 |= FM_DSP_RSSI_H_TH(65)|FM_DSP_RSSI_L_TH(50)|FM_DSP_NOISE_H_TH(60)|FM_DSP_NOISE_L_TH(60) ; //55
    hwp_fm_dsp->seek_ctrl3 &= ~(FM_DSP_OFFSET_L_TH_MASK|FM_DSP_OFFSET_H_TH_MASK|FM_DSP_PILOT_H_TH_MASK|FM_DSP_PILOT_L_TH_MASK);
    hwp_fm_dsp->seek_ctrl3 |= FM_DSP_OFFSET_L_TH(16) | FM_DSP_OFFSET_H_TH(25)|FM_DSP_PILOT_H_TH(15)|FM_DSP_PILOT_L_TH(8);
    hwp_fm_dsp->seek_ctrl4 &= ~(FM_DSP_SK_TIMER1_MASK|FM_DSP_SK_TIMER2_MASK|FM_DSP_SK_TIMER3_MASK);
    hwp_fm_dsp->seek_ctrl4 |= FM_DSP_SK_TIMER1(4)|FM_DSP_SK_TIMER2(24);//|FM_DSP_SK_TIMER3(32);
    #ifndef FM_E
    hwp_fm_dsp->agc_ctrl0 &= ~(FM_DSP_AGC_TARGET_PWR_MASK | FM_DSP_AGC_LOOP_GAIN1_MASK | FM_DSP_AGC_UPDATE1_MASK | FM_DSP_AGC_THD_MASK |FM_DSP_AGC_INDEX_IN_MASK);
    hwp_fm_dsp->agc_ctrl0 |= FM_DSP_AGC_TARGET_PWR(34) | FM_DSP_AGC_LOOP_GAIN1(5) | FM_DSP_AGC_UPDATE1(1) | FM_DSP_AGC_THD(17) | FM_DSP_AGC_INDEX_IN(5);
    #endif
    hwp_fm_dsp->agc_ctrl1 &= ~(FM_DSP_OVER_STEP_MASK | FM_DSP_LOG_AGC_TH_MASK);
    hwp_fm_dsp->agc_ctrl1 |= FM_DSP_OVER_STEP(36) | FM_DSP_LOG_AGC_TH(40);
    hwp_fm_dsp->log_ctrl0 &= ~FM_DSP_CT_U_NOISE_MASK;
    hwp_fm_dsp->log_ctrl0 |= FM_DSP_CT_U_NOISE(0x3c);
}


void hal_Fm_Dsp_Datapath_Init(void)
{
    //notchfit flt bypass
    //hwp_fm_dsp->datapath_ctrl4 |= FM_DSP_NOTCH_FLT_BYPASS;

    //diable irq switch before mixer 
    hal_Fm_Dsp_Enable_IQSwitchBeforeMixer(FALSE);

    //disable mute
    hal_Fm_Dsp_Enable_Mute(FALSE);

    //disable softmute
    hal_Fm_Dsp_Enable_SoftMute(FALSE);

    //enable afc
    hal_Fm_Dsp_Disable_AFC(FALSE);

    //deemph bypass
    hal_FM_DSP_Deemph_Bypass(TRUE);

    //disable blend
    hal_Fm_Dsp_Enable_Softblend(TRUE);

   //lpf bypass
    hal_FM_DSP_LPF_Bypass(FALSE);
   
    //noise src sel
    hwp_fm_dsp->ctrl &= ~FM_DSP_NOISE_SRC_SEL_MASK;
    hwp_fm_dsp->ctrl |= FM_DSP_NOISE_SRC_SEL_DANGLE0;
		
    //dc cancel control
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_DC_CT_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_DC_CT(0xc);

    //sin  limit gain 
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_SINC_LIMIT_GAIN_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_SINC_LIMIT_GAIN_6DB;

    //limit sel =1
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_LIMIT_SEL;

    //over threshold selection
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_OVER_TH_SEL_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_OVER_TH_SEL_3;

    //gainct
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_GAINCT_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_GAINCT(g_halFMDsp_Datapath_Ctrl0_Gainct);

    //noise_cancel_ctrl1 fircut	
     hwp_fm_dsp->noise_cancel_ctrl1 = FM_DSP_SEL_FIRCUT_SK(9) |FM_DSP_SEL_FIRCUT3(5) |FM_DSP_SEL_FIRCUT2(5) | FM_DSP_SEL_FIRCUT1(5)|FM_DSP_TH_MIN_HCC(32);
    
    //output dig gain
    hal_FM_DSP_Fmdem_Sel(1);

    //noise_cancel_ctrl0 fmdem sel grp
    hwp_fm_dsp->noise_cancel_ctrl0 &= ~FM_DSP_FMDEM_SEL_GRP_MASK;
    hwp_fm_dsp->noise_cancel_ctrl0 |= FM_DSP_FMDEM_SEL_GRP(0);

    //afc_ctrl afc_ct
    hwp_fm_dsp->afc_ctrl &= ~(FM_DSP_AFC_CT_MASK|FM_DSP_AFC_CT_SK_MASK|FM_DSP_AFC_INV|FM_DSP_AFC_RANGE_MASK);
    hwp_fm_dsp->afc_ctrl |= FM_DSP_AFC_CT(0) |FM_DSP_AFC_CT_SK(g_halFMDsp_Afc_Ctrl_Afc_Ct_Sk) | FM_DSP_AFC_RANGE(g_halFMDsp_Afc_Ctrl_Afc_Range);

    //noise_cancel_ctrl2
    hwp_fm_dsp->noise_cancel_ctrl2 = 0xb100b100;//32;
    
    //datapath_ctrl1           nco_ct, limit gain, softmute rssi,noise,snr
    hwp_fm_dsp->datapath_ctrl1 = 0;
    hwp_fm_dsp->datapath_ctrl1 |= FM_DSP_NCO_CT_BW(3)|FM_DSP_NCO_CT_U_DT(0x4) | FM_DSP_SOFTMUTE_TH1(g_halFMDsp_Datapath_Ctrl1_SoftMute_Th1) |FM_DSP_SOFTMUTE_TH2(g_halFMDsp_Datapath_Ctrl1_SoftMute_Th2) | FM_DSP_SOFTMUTE_TH3(g_halFMDsp_Datapath_Ctrl1_SoftMute_Th3);

    //datapath_ctrl2  softmute_th4
    hwp_fm_dsp->datapath_ctrl2 &= ~(FM_DSP_SOFTMUTE_TH4_MASK|FM_DSP_SM_RATE_MASK|FM_DSP_SM_ATTENU_MASK|FM_DSP_TH_SOFTBLEND1_MASK|FM_DSP_TH_SOFTBLEND2_MASK|FM_DSP_DEEMPH_HCC_MASK|FM_DSP_LPF_BW_REG_MASK);
    hwp_fm_dsp->datapath_ctrl2 |= FM_DSP_SOFTMUTE_TH4(g_halFMDsp_Datapath_Ctrl2_SoftMute_Th4) | FM_DSP_SM_RATE(g_halFMDsp_Datapath_Ctrl2_SmRate) | FM_DSP_SM_ATTENU(g_halFMDsp_Datapath_Ctrl2_SmAttenu) | FM_DSP_TH_SOFTBLEND1(g_halFMDsp_Datapath_Ctrl2_ThSoftBlend1) | FM_DSP_TH_SOFTBLEND2(g_halFMDsp_Datapath_Ctrl2_ThSoftBlend2)|FM_DSP_DEEMPH_HCC(0)|FM_DSP_LPF_BW_DR|FM_DSP_LPF_BW_REG(g_halFMDsp_Datapath_Ctrl2_Lpf_Bw);

    //datapath0    limit_sel  sin_limit gain 
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_SINC_LIMIT_GAIN_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_SINC_LIMIT_GAIN_6DB|FM_DSP_LIMIT_SEL;

    //over threshold selection
    hwp_fm_dsp->datapath_ctrl0 &= ~FM_DSP_OVER_TH_SEL_MASK;
    hwp_fm_dsp->datapath_ctrl0 |= FM_DSP_OVER_TH_SEL_3;

    //ctrl  DeemphMode
    hwp_fm_dsp->ctrl &= ~(FM_DSP_DEEMPH_MODE | FM_DSP_NOISE_SRC_SEL_MASK);
    #ifndef FM_E
    hwp_fm_dsp->ctrl |= g_halFMDsp_Ctrl_DeemphMode | FM_DSP_NOISE_SRC_SEL(2)|FM_DSP_SEEK_RSSI_SRC_SEL_SIGNAL_DB1|FM_DSP_ADC_CLK_INVERT|FM_DSP_TX_EN_ENABLE|FM_DSP_ADC_DATA_FORMAT;
    hwp_fm_dsp->ana_gain_rssi_tb0  = 0xfaf4f4f4;
    hwp_fm_dsp->ctrl  = 0x8ad50002;
    #else
    hwp_fm_dsp->ctrl  = 0x84d50002;
    hwp_fm_dsp->ctrl |= g_halFMDsp_Ctrl_DeemphMode | FM_DSP_NOISE_SRC_SEL(2)|FM_DSP_SEEK_RSSI_SRC_SEL_SIGNAL_DB1|FM_DSP_ADC_CLK_INVERT|FM_DSP_FORCE_CLK_ON;
    #endif

    hwp_fm_dsp->datapath_ctrl3 = 0x39e83924;
    hwp_fm_dsp->datapath_ctrl4 = 0x27a933;
    hwp_fm_dsp->datapath_ctrl5 = 0x24dec;
	 
    hal_Fm_Dsp_Seek_Ctrl_Init();
    
}


void hal_Rf_SoftReset( void ) 
{
    hwp_rf_if->sys_control = 0x0;
    hwp_rf_if->sys_control = WCN_RF_IF_SOFT_RESETN;
}

void hal_Rf_Self_Calibration(void) 
{
    hwp_rf_if->lna_rmx_setting = 0x8101d8;
    #ifndef FM_E
    hwp_rf_if->rxflt_cal_setting0 = 0x4b90270;
    #else
    hwp_rf_if->rxflt_cal_setting0 = 0x4b902ff;
    #endif
    hwp_rf_if->pll_sdm_setting0 = 0x144800;
    hwp_rf_if->sys_control &= ~WCN_RF_IF_CHIP_SELF_CAL_ENABLE;
    hwp_rf_if->sys_control |= WCN_RF_IF_CHIP_SELF_CAL_ENABLE;
    while (((hwp_rf_if->cal_status) & (WCN_RF_IF_BT_SELF_CAL_READY|WCN_RF_IF_FM_SELF_CAL_READY|WCN_RF_IF_WF_SELF_CAL_READY)) == 0) 
    {
    }
}

void hal_Rf_Set_Rf_Mode( uint8 rf_mode ) 
{ 
    if (rf_mode == 0)    //bt mode
    {
	   hwp_rf_if->sys_control &= ~ WCN_RF_IF_RF_MODE_MASK;
	   hwp_rf_if->adc_setting0 |= WCN_RF_IF_ADC_CLK_DIV2;
    } 
    else                         //fm mode
    {
          hwp_rf_if->sys_control &= ~ WCN_RF_IF_RF_MODE_MASK;
	   hwp_rf_if->sys_control |=  WCN_RF_IF_RF_MODE_FM;
	   hwp_rf_if->adc_setting0 &= ~WCN_RF_IF_ADC_CLK_DIV2;
    }
    hwp_rf_if->sys_control |= WCN_RF_IF_BT_TUNE_DIFF_EN | WCN_RF_IF_BT_HOPPING_EN;
}

void hal_Rf_set_Fm_AdcClk_Mode(uint8 clkmode)
{
    hwp_rf_if->sys_control &= ~ WCN_RF_IF_FM_ADC_CLK_MODE;
    if(clkmode == 0)   
    {
        hwp_rf_if->sys_control |= WCN_RF_IF_FM_ADC_CLK_MODE_DIVIDER;
    }
    else 
    {
         hwp_rf_if->sys_control |= WCN_RF_IF_FM_ADC_CLK_MODE_ADPLL;
    }
}


void hal_Rf_FM_Tune(uint8 on)
{
     if(on == TRUE)
         hwp_rf_if->fm_control |= WCN_RF_IF_FM_TUNE;
     else
	  hwp_rf_if->fm_control &= ~WCN_RF_IF_FM_TUNE;
}
  
void hal_Rf_Vco(uint8 on)
{
     if(on == TRUE)  //CLOSE
     {
         hwp_rf_if->vco_setting &= ~WCN_RF_IF_VCO_VAR_BIT_MASK;
	  hwp_rf_if->vco_setting |= WCN_RF_IF_VCO_VAR_BIT(6);
	  hwp_rf_if->pll_setting1 &= ~ (WCN_RF_IF_PLL_GAIN_BIT_TX_MASK|WCN_RF_IF_PLL_GAIN_BIT_RX_MASK|WCN_RF_IF_PLL_CP_R_BIT_TX_MASK|WCN_RF_IF_PLL_CP_R_BIT_RX_MASK|WCN_RF_IF_PLL_R_BIT_TX_MASK);
	   hwp_rf_if->pll_setting1|=WCN_RF_IF_PLL_GAIN_BIT_TX(4) | WCN_RF_IF_PLL_GAIN_BIT_RX(5)|WCN_RF_IF_PLL_CP_R_BIT_TX(5)|WCN_RF_IF_PLL_CP_R_BIT_RX(5)|WCN_RF_IF_PLL_R_BIT_TX(1);
	  hwp_rf_if->vco_pkd_cal_setting |=  WCN_RF_IF_VCO_IBIT_DR;
	  hwp_rf_if->power_dr |= WCN_RF_IF_PU_VCO_RXLO_DR;
     } 
     else
    {
         hwp_rf_if->vco_setting &= ~WCN_RF_IF_VCO_VAR_BIT_MASK;
	  hwp_rf_if->vco_setting |= WCN_RF_IF_VCO_VAR_BIT(3);
	  hwp_rf_if->pll_setting1 &= ~ (WCN_RF_IF_PLL_GAIN_BIT_TX_MASK|WCN_RF_IF_PLL_GAIN_BIT_RX_MASK|WCN_RF_IF_PLL_CP_R_BIT_TX_MASK|WCN_RF_IF_PLL_CP_R_BIT_RX_MASK|WCN_RF_IF_PLL_R_BIT_TX_MASK);
	  hwp_rf_if->pll_setting1 |=WCN_RF_IF_PLL_GAIN_BIT_TX(8) | WCN_RF_IF_PLL_GAIN_BIT_RX(8)|WCN_RF_IF_PLL_CP_R_BIT_TX(7)|WCN_RF_IF_PLL_CP_R_BIT_RX(7)|WCN_RF_IF_PLL_R_BIT_TX(2);
	  hwp_rf_if->vco_pkd_cal_setting &=  ~WCN_RF_IF_VCO_IBIT_DR;
	  hwp_rf_if->power_dr &= ~ WCN_RF_IF_PU_VCO_RXLO_DR;
     }
}

	
void hal_RF_Gain_Table_Init(void)
{
     SCI_TRACE_LOW("fm_7\n");
     #ifndef FM_E
     hwp_rf_if->fm_gain_table_0 = 0x4200430;
     hwp_rf_if->fm_gain_table_1 = 0x44004c0;
     hwp_rf_if->fm_gain_table_2 = 0x16c01740;
     hwp_rf_if->fm_gain_table_3 = 0x274027c0;
     hwp_rf_if->fm_gain_table_4 = 0x39c039d0;
     hwp_rf_if->fm_gain_table_5 = 0x39e039f0;
     hwp_rf_if->fm_gain_table_6 = 0x3bf43bf8;
     hwp_rf_if->fm_gain_table_7 = 0x3bfc3bfe;
    
     #else
     hwp_rf_if->fm_gain_table_0 = 0x10201030;
     hwp_rf_if->fm_gain_table_1 = 0x10401240;
     hwp_rf_if->fm_gain_table_2 = 0x12c01340;
     hwp_rf_if->fm_gain_table_3 = 0x15401dc0;
     hwp_rf_if->fm_gain_table_4 = 0x27c027d0;
     hwp_rf_if->fm_gain_table_5 = 0x27e02ff0;
     hwp_rf_if->fm_gain_table_6 = 0x2ff42ff8;
     hwp_rf_if->fm_gain_table_7 = 0x2ffc2ffe;
     hwp_fm_dsp->ana_gain_rssi_tb0 = 0xc0600fa;
     hwp_fm_dsp->ana_gain_rssi_tb1= 0x231e1712;
     hwp_fm_dsp->ana_gain_rssi_tb2 = 0x3c36302a;
     hwp_fm_dsp->ana_gain_rssi_tb3 = 0x544e4842;
     hwp_fm_dsp->agc_ctrl0 = 0x369e0e7c;
     #endif
    // hwp_rf_if->direct_gain_setting &=  ~WCN_RF_IF_FM_AGC_GAIN_REG_MASK;
     //hwp_rf_if->direct_gain_setting |=  WCN_RF_IF_FM_AGC_GAIN_DR|WCN_RF_IF_FM_AGC_GAIN_REG(15);
     SCI_TRACE_LOW("fm_7\n");
}
    
void hal_RF_Init(void)
{
  //  hal_Rf_SoftReset();
  #ifndef FM_E
    hal_Rf_Self_Calibration();
  #endif
    hal_Rf_set_Fm_AdcClk_Mode(1);
    hal_RF_Gain_Table_Init();
    #ifdef FM_E
    hal_Rf_Self_Calibration();
    #endif
    //hwp_rf_if->test_buf |= WCN_RF_IF_DAC_OUT_EN;
   // hwp_rf_if->power_dr |= WCN_RF_IF_PU_ADC_REG;
    //hwp_rf_if->power_reg |= WCN_RF_IF_PU_BT_DAC_REG;
    //hwp_rf_if->bt_dac_setting = 0x5108e;
    hwp_rf_if->fm_dsp_setting &= ~(WCN_RF_IF_FM_DSP_RESETN_DELAY_MASK);
    hwp_rf_if->fm_dsp_setting  |= WCN_RF_IF_FM_DSP_RESETN_DELAY(3);
    hwp_rf_if->pll_setting2 &= ~(WCN_RF_IF_MDLL_DIV_RX_MASK | WCN_RF_IF_MDLL_DIV_TX_MASK);
    hwp_rf_if->pll_setting2 |= WCN_RF_IF_PLL_MDLL_REFCLK_EN_DR | WCN_RF_IF_PLL_MDLL_REFCLK_EN_REG|WCN_RF_IF_MDLL_DIV_TX(2) |WCN_RF_IF_MDLL_DIV_RX(2);

    hwp_rf_if->pga_setting0 = 0x3000;
    hwp_rf_if->pga_setting1 = 0x6C222;
    hwp_rf_if->rxflt_setting = 0x736110A;
}

void fmd_Open(void)
{
    hal_RF_Init();
    hal_Rf_Vco(TRUE);
    hal_Fm_Dsp_Datapath_Init();
   // hal_FM_DSP_Enable_I2s(TRUE);
    hal_Rf_Set_Rf_Mode(2);
    rdafm_Start_Fm_Band();
}

void fmd_Close(void)
{
    hal_Rf_FM_Tune(FALSE);
    hal_Rf_Vco(FALSE);

}
void fmd_Sysctrl(VOID)
{
        hwp_wcnSysCtrl->clk_cfg &=  ~(WCN_SYS_CTRL_REG_PULP_MCU_HCLK_EN | WCN_SYS_CTRL_REG_WDT_CLK_EN | \
        WCN_SYS_CTRL_REG_BT_DBM_HCLK_EN |WCN_SYS_CTRL_REG_BT_HCLK_EN |WCN_SYS_CTRL_REG_MEM_HCLK_EN | \
        WCN_SYS_CTRL_REG_JTAG_HCLK_EN | WCN_SYS_CTRL_REG_LOW_POWER_CLK_EN |WCN_SYS_CTRL_REG_MASTER_CLK_EN | \
        WCN_SYS_CTRL_REG_WAKEUP_CTRL_CLK_EN);
        hwp_wcnSysCtrl->clk_cfg1 &=  ~(WCN_SYS_CTRL_REG_UART_CLK_DIV_EN);
	 hwp_wcnSysCtrl->clk_208m_cfg &=  ~(WCN_SYS_CTRL_REG_DBG_HST_DIV_EN);
	 //reset cpu
	 hwp_wcnSysCtrl->soft_reset &= ~WCN_SYS_CTRL_REG_PULP_MCU_HRESETN;
	 //switch bt clock
	hwp_wcnSysCtrl->clk_cfg1  &= ~(1<<21);
 
}


