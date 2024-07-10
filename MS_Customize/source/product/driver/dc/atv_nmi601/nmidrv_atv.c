/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmidrv_atv.c
**		This module implements the Nmi analog TV driver APIs.
**	Creator: Michael Jia
** 
*******************************************************************************/

#include "ms_customize_trc.h"
#include "nmiport.h"
#include "nmiapi.h"
#include "nmicmn.h"
#include "nmidrv_atv.h"

static unsigned char is_atv_nmi600_active=0;
unsigned char nmi600_output_size=0;

unsigned int atv_nmi600_poweron_init(void);
unsigned int atv_nmi600_poweroff_deinit(void);
void atv_nmi600_set_uv_snr_threshold(double high, double low);
void atv_nmi600_set_scan_level(unsigned int scanlevel);
void atv_nmi600_set_volume(unsigned int vol);

void atv_nmi600_set_uv_iirfactor(double factor);
void atv_nmi600_set_uv_gain(unsigned int step);

/*****************************************************************************
*                                                                                                                                     *
*                                   Analog TV base function(power, play, scan...)                                 *
*                                                                                                                                     * 
******************************************************************************/

static uint32 atv_nmi600_power_on(unsigned char onff)
{
    uint32 rtn = SCI_SUCCESS;
    rtn = atv_nmi600_poweron_init();

    if(1 != rtn)
    {
        //SCI_TraceLow:"nmidrv_atv: nmi atv power on failed!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIDRV_ATV_39_112_2_18_0_29_34_622,(uint8*)"");
        rtn = SCI_ERROR;
    }
    else
    {
        rtn = SCI_SUCCESS;
    }
    
    return rtn;
}

void atv_nmi600_power_off(void)
{
	atv_nmi600_poweroff_deinit();
}                          

int8 atv_nmi600_init(void)
{
      int8 rtn = 0;
	rtn = (int8)atv_nmi600_power_on(TRUE);
	//atv_nmi600_set_scan_level(7);
	//nmi_atv_set_comb_filter()

	return rtn;
}  


/******************************************************************************  
*  function:
*      atv_nmi600_poweron_init
*  para:
*       NULL
*description:
*      power on ,and initilize iic and  NMI600 chip
*
*******************************************************************************/
unsigned int atv_nmi600_poweron_init(void)
{
	int result = 0;
	tAtvInit cfg;

	cfg.xo = nXO26MHz;
	cfg.io_volt=IO_2_8Volt;
	cfg.format = nYCbCr;	
	cfg.rate = nBT13_5MHz;	//Note: NMI601 don't support 6.75Mhz pclk
	cfg.size = nmi600_output_size = bt320x240;	
	cfg.clk_polarity=1;      //PCLK: 0: failing edge active, 1: rising edge active
	cfg.vsync_polarity=1;    //Vsync:  0:high active, 1:low active
	cfg.hsync_polarity=0;    //Hsync:  0:High active, 1:low active
	cfg.input_order=1;	     // 0:YUYV; 1: UYVY
	cfg.blank_interval=200;	
	cfg.interlace=0;         //interlace mode
	cfg.half_frame_rate=0;   //only for NMI601,decrease FPS to half( 25->12.5, 30->15)
	cfg.enable_pattern=1;    // 0: disable, 1:enable
	
	result = (int)nmi_drv_ctl(NMI_DRV_INIT, (void *)&cfg);
	//nmi_atv_set_driving_ability(7);

	return result;
}

/******************************************************************************  
*  function:
*      atv_nmi600_poweroff_deinit
*  para:
*       NULL
*description:
*      power off ,and release iic
*
*******************************************************************************/
unsigned int atv_nmi600_poweroff_deinit(void)
{	
    unsigned int ret;
	//atv_nmi600_set_mute(1);
	ret=nmi_drv_ctl(NMI_DRV_DEINIT, NULL);
	return ret;
}

/******************************************************************************  
*  function:
*      atv_nmi600_set_region
*  para:
*       region:  refer aRgnMap
*description:
*      set TV region
*
*******************************************************************************/
void  atv_nmi600_set_region(unsigned int region)
{
	tAtvRegion rgn;

	rgn.region = region;
	rgn.schannel = 0;
	rgn.echannel = 0;
	nmi_drv_ctl(NMI_DRV_SET_REGION, (void *)&rgn);
}

/******************************************************************************  
*  function:
*      atv_nmi600_get_total_channel_num
*  para:
*       region: input,  refer aRgnMap
         startID: output, start channel in this region
         endID: output, start channel in this region
          
*description:
*      get startID and endID in region
*
*******************************************************************************/
unsigned int atv_nmi600_get_total_channel_num(unsigned int region,unsigned int* startID,unsigned int* endID)
{
	int32 channl_num = 0;
	tAtvRegion rgn;

	rgn.region = region; 	
	rgn.schannel = 0;
	rgn.echannel = 0;
	nmi_drv_ctl(NMI_DRV_SET_REGION, (void *)&rgn);

	channl_num =  (rgn.echannel - rgn.schannel) + 1; 	
	*startID = rgn.schannel;
	*endID = rgn.echannel;

	return channl_num;
}

/******************************************************************************  
*  function:
*      atv_nmi600_fast_set_channel
*  para:
*       channelID: input
*description:
*      fast set TV channel, without wait for lock. if lock or not, return lock. it uses in play channel
*      this function doesn't fit scan.scan use function 'atv_nmi600_scan_one_channel' 
*
*******************************************************************************/
unsigned int atv_nmi600_fast_set_channel(unsigned int channelID)
{
	tNmiTune tune;
	tune.chn = channelID;
	tune.poll = 0;
	tune.waitforlock=0;

	nmi_drv_ctl(NMI_DRV_TUNE_TO_CHANNEL, (void *)&tune);

	//atv_nmi600_set_uv_iirfactor(0.9);
	//atv_nmi600_set_volume(6);
	//atv_nmi600_set_uv_gain(5);
	//atv_nmi600_set_luma_gain(2);
	//atv_nmi600_set_luma_offset(1);
	//atv_nmi600_set_uv_gain(uv_gain);
	//atv_nmi600_set_luma_gain(luma_gain);
	//atv_nmi600_set_luma_offset(luma_offset);
	//atv_nmi600_set_uv_snr_threshold(24,1);
	//nmi_atv_set_spur_table();
	//nmi_atv_set_frame_triger_thrd(48);
	nmi_atv_set_motiondetect();
	nmi_atv_set_fieldoutput(0x249);		

	return 1;
}

/******************************************************************************  
*  function:
*       atv_nmi600_scan_one_channel
*  para:
*       channelID: input
*  description:
*       scan one channel, rerutn lock or not. 
*
*******************************************************************************/
unsigned int atv_nmi600_scan_one_channel(unsigned int channelID)
{
	tNmiTune tune;
	tune.chn = channelID;
	tune.poll = 0;
	tune.waitforlock=1;

	nmi_drv_ctl(NMI_DRV_TUNE_TO_CHANNEL, (void *)&tune);
	return 	tune.lock;
}


/******************************************************************************  
*  function:
*     atv_nmi600_get_rssi
*  para:
*       rssilevel: output (0-10, best rssi level is 10, worst level is 0)
*  description:
*       get current RSSI. Normally, if rssi level is below 2 or 3, it means low signal quality
*
*******************************************************************************/

void atv_nmi600_get_rssi(unsigned short *rssilevel)
{
	tNmiSq status;

	nmi_drv_ctl(NMI_DRV_GET_STATUS, (void *)&status);
	*rssilevel=status.sq;		
}





/*****************************************************************************
*                                                                                                                                     *
*                                   Analog TV auxiliary function(volume,luma,UV...)                             *
*                                                                                                                                     * 
******************************************************************************/


/******************************************************************************  
*  function:
*      atv_nmi600_set_active
*  para:
*       active: input
*description:
*     set nmi600 active, select nmi600 as current image sensor device
*
*******************************************************************************/
void atv_nmi600_set_active(unsigned char  active)
{
	is_atv_nmi600_active = active;
}

/******************************************************************************  
*  function:
*      atv_nmi600_get_active
*  para:
*       active: input
*description:
*     get nmi600 active state
*
*******************************************************************************/
unsigned char  atv_nmi600_get_active(void)
{
	return is_atv_nmi600_active ;
}



/******************************************************************************  
*  function:
*     atv_nmi600_set_volume
*  para:
*       vol: input	0 to 7 (default: 4 )   
	
*  description:
*       set nmi600 volume
*
*******************************************************************************/
void atv_nmi600_set_volume(unsigned int vol)
{
	tNmiVolume volume;

	if(vol<=7)
	{
		volume.step=vol;
		nmi_drv_ctl(NMI_DRV_SET_VOLUME, (void *)&volume);
	}
}

/******************************************************************************  
*  function:
*     atv_nmi600_set_mute
*  para:
*       vol: mute: 1-mute; 0-unmute
	
*  description:
*       set nmi600 audio mute
*
*******************************************************************************/
void  atv_nmi600_set_mute(unsigned char mute)
{
	tNmiMute tvmute;

	tvmute.mute=mute;
	nmi_drv_ctl(NMI_DRV_MUTE, (void*)&tvmute);	

}


/******************************************************************************  
*  function:
*     atv_nmi600_set_scan_level
*  para:
*       scanlevel (input): 1 to 7 is the valid range 
*                                   the 7 is the most relax threshold and 1 is the most strigent (the default is 4).
*	
*  description:
*       set nmi600 scan level
*
*******************************************************************************/
void atv_nmi600_set_scan_level(unsigned int scanlevel)
{	
	nmi_drv_ctl(NMI_DRV_SET_SCAN_LEVEL, (void *)&scanlevel);
}

/******************************************************************************  
*  function:
*     atv_nmi600_set_luma_gain
*  para:
*       lumagain: input  0-8(default is 4)	
	
*  description:
*       set luma gain. 
*
*******************************************************************************/
void atv_nmi600_set_luma_gain(unsigned char lumagain)
{
	if( lumagain<=8)
	{
	    nmi_atv_set_luma_gain(lumagain);	
	}	
}


/******************************************************************************  
*  function:
*     atv_nmi600_set_luma_offset
*  para:
*       lumagain: input  -8~8(default is 0)	
	
*  description:
*       set luma offset. the value is bigger, the video will brighter. 
*
*******************************************************************************/
void atv_nmi600_set_luma_offset(int lumaoffset)
{
	if( lumaoffset >= -8 && lumaoffset<=8)
	{
	    nmi_atv_set_luma_offset(lumaoffset);	
	}	
}


/******************************************************************************  
*  function:
*     atv_nmi600_set_uv_gain
*  para:
*       step: input	0 to 10 (default is 8)
	
*  description:
*       set uv gain. 
*
*******************************************************************************/
void atv_nmi600_set_uv_gain(unsigned int step)
{
	tNmiUVGain gain;
	
	if(step<=10)
	{
		gain.uvGain = step-5;
		nmi_drv_ctl(NMI_DRV_UV_GAIN, (void *)&gain);
	}	
}


/******************************************************************************  
*  function:
*     atv_nmi600_set_uv_snr_threshold
*  para:
*       high: input	high threshold
	  low:input      low threshold
*  description:
*       set snr threshold to adjust uv gain. Default high threshold is 20, low is 1. 
*
*******************************************************************************/
void atv_nmi600_set_uv_snr_threshold(double high, double low)
{
	tNmiUVSnrTh snr;
	snr.snrHigh = high;
	snr.snrLow = low;
	nmi_drv_ctl(NMI_DRV_UV_SNR_THRESHOLD, (void *)&snr);
	
}

/******************************************************************************  
*  function:
*     atv_nmi600_set_uv_iirfactor
*  para:
*       factor (input): A value from 0 ~ 1(default is 1.0 )  
*                            0 means no change (fixed gain) and 1 has fastest chnage of color.
*	
*  description:
*       set the UV adjustment filter, make color change smooth
*
*******************************************************************************/
void atv_nmi600_set_uv_iirfactor(double factor)
{	
	nmi_drv_ctl(NMI_DRV_SET_UV_IIR_FACTOR, (void *)&factor);
}

/******************************************************************************  
*  function:
*     atv_nmi600_set_output_size
*  para:
*       sizetype:  	
		bt320x240,
		bt80x60,
		bt176x144,
		bt220x176,
		bt240x180,
		bt426x320,
		bt360x240,
		bt480x280,
		bt480x360,

	
*  description:
*       set nmi600 bt output size,,it can be set in run time,
*
*******************************************************************************/
void atv_nmi600_set_output_size(unsigned int sizetype)
{
       nmi600_output_size=sizetype;
	nmi_drv_ctl(NMI_DRV_SET_OUTPUT_SIZE, (void *)&sizetype);    
}


/******************************************************************************  
*  function:
*     atv_nmi600_set_fullscreen_mode
*  para:
*       isfull:  1-full screen mode; 0-smaller window mode
	
*  description:
*       set nmi600 mode
*
*******************************************************************************/
void atv_nmi600_set_fullscreen_mode(unsigned int isfull)
{
#if 0
	if(isfull)
	{
		nmi_atv_set_frame_triger_thrd(239);
	}
	else
	{
		nmi_atv_set_frame_triger_thrd(32);
	}
#endif	
}

/******************************************************************************  
*  function:
*      atv_nmi600_power_save_for_camera
*  para:
*       NULL
*description:
*      power save mode, keep VDDIO on , NMI600 don't driver the YUV bus. 
*      this function should be called before camera power-on and initialization
*
*******************************************************************************/
void atv_nmi600_power_save_for_camera(void)
{	
    #if 1  
	   //recommand method
	   //condition(1): VDDIO on, Vcore off, reset low
       nmi_power_save_mode();
    #else   
	   //condition(2): both VDDIO and  Vcore on, reset high
	nmi_power_sleep_mode();
	nmi_atv_set_sleep_mode();
	#endif
} 



/*****************************************************************************
*                                                                                                                                     *
*                                  test function                                                                                 *
*                                                                                                                                     * 
******************************************************************************/



/******************************************************************************  
*  function:
*     atv_nmi600_show_pattern
*  para:
*       enable:  1-enable; 0-disable
	
*  description:
*       set nmi600 pattern output for test
*
*******************************************************************************/
void atv_nmi600_show_pattern(unsigned int enable)
{
	tNmiPattern pattern;

	pattern.enable=enable;
	
	nmi_drv_ctl(NMI_DRV_SHOW_PATTERN, (void *)&pattern);
}


/******************************************************************************  
*  function:
*     atv_nmi600_get_color_lock
*  para:
*       None
*  return: if lock, return 1; else return 0	
*  description:
*       set nmi600 pattern output for test
*
*******************************************************************************/
unsigned int atv_nmi600_get_color_lock(void)
{
    return nmi_atv_get_color_lock();
}

static uint32 scan_start_chn = 0;
static uint32 scan_end_chn = 0;
static uint16 scan_chn_index = 0;


void atv_nmi_init_scan_param(int region)
{
	atv_nmi600_get_total_channel_num(region, &scan_start_chn, &scan_end_chn);

	scan_chn_index = scan_start_chn;

	//SCI_TRACE_LOW:"atv_nmi_init_scan_param region = %d,  scan_start_chn = %d,  scan_end_chn= %d \r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIDRV_ATV_539_112_2_18_0_29_35_623,(uint8*)"ddd", region, scan_start_chn, scan_end_chn);

}

uint32 atv_nmi_scan_chn(uint32* chn_id, uint32* is_valid)
{
    int lock;

    //SCI_TRACE_LOW:"atv_nmi_scan_chn scan_chn_index = %d \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIDRV_ATV_547_112_2_18_0_29_35_624,(uint8*)"d", scan_chn_index);

    if(scan_chn_index > scan_end_chn)
    {
        return TRUE;
    }

    lock=atv_nmi600_scan_one_channel(scan_chn_index);

    *chn_id=scan_chn_index; 

    if(lock)
    {
        *is_valid=TRUE;
    }
    else
    {
        *is_valid=FALSE;
    }

    scan_chn_index++;

    return FALSE;
}

uint32 NMIAPP_GetCurrSearchNum(void)
{
	return scan_chn_index;
}

uint32 NMIAPP_GetTotalSearchNum(void)
{
	return scan_end_chn;
}


