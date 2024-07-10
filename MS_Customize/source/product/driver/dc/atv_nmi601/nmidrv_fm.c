/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmidrv_fm.c
**		This module implements the Nmi FM driver APIs.
**	Creator: Michael Jia
** 
*******************************************************************************/

#include "nmiport.h"
#include "nmiapi.h"
#include "nmicmn.h"
#include "nmidrv_atv.h"
#include "os_api.h"
#include "nmidrv_fm.h"
#include "nmidrv_custom.h"

/*lint -save -e774*/

static unsigned char is_scan_start = 1;
static int s_rssi = 0;
static unsigned char fm_step_100k_or_50k=0;  //step:0 means 100khz ;1 means 50khz; default 100k


static void nmi_fm_set_flag(void)
{
	is_scan_start = 1;
}

void nmi_fm_set_fm_step(unsigned char step)
{
    if(step==0)     
	fm_step_100k_or_50k = 0;  // 0-100K step
     else
	fm_step_100k_or_50k = 1;  // 1- 50K step	 	
}

unsigned char nmi_fm_get_fm_step(void)
{
	return fm_step_100k_or_50k ;
}


void  FMDrv_PowerOnReset(void)
{
	//int result = 0;
	tAtvInit cfg;

    //power on and init
    atv_nmi600_poweron_init();

    //set scan mode: 100Khz or 50Khz step
	if( nmi_fm_get_fm_step()==0 )  //100K step
	    atv_nmi600_set_region(nFmChina);
	else if( nmi_fm_get_fm_step()==1 )  //50K step
	    atv_nmi600_set_region(nFmThai);

	//set scan threshold
	//level 0-7 (the default is 1), the 0 is the most relax threshold,and 7 is the most strigent
    //nmi_atv_set_fm_scan_level(5);    


	nmi_fm_set_flag();
	
}

void  FMDrv_PowerOffProc(void)
{
    //FMDrv_Mute(1);
	nmi_drv_ctl(NMI_DRV_DEINIT, NULL);
	nmi_fm_set_flag();
}


unsigned char FMDrv_ValidStop( int freq, char signalvl, char is_step_up)
{

	unsigned int channelID = 0;
    tNmiFMSeek fmseek;

	channelID = (freq-FM_MIN_FRQ) +FM_NMI_OFFSET;
	if(freq==FM_MIN_FRQ || freq==FM_MAX_FRQ ) 
	{	
		is_scan_start=1;
	}

	fmseek.found = 0;
	fmseek.chn = channelID;
	fmseek.firstchannel = is_scan_start;		
	nmi_drv_ctl(NMI_DRV_FM_SEEK,(void *)&fmseek);
	
	if(is_scan_start)	is_scan_start = 0;	

	//dump_printf("FMDrv_ValidStop feq=%d, chn=%d, found=%d, rssi=%d",freq,channelID,fmseek.found,fmseek.rssi);

	if(fmseek.found==0)	       
		s_rssi=0;
	else		                           
		s_rssi= fmseek.rssi;	
	
	return fmseek.found;		
}


void  FMDrv_SetFreq( signed int curf )
{
	unsigned int channelID = 0;
	tNmiTune tune;

	channelID = (curf-FM_MIN_FRQ) +FM_NMI_OFFSET;
	tune.chn = channelID;
	tune.poll = 0;
	tune.waitforlock=0;
	nmi_drv_ctl(NMI_DRV_TUNE_TO_CHANNEL, (void *)&tune);		
	nmi_fm_set_flag();
}

unsigned char  FMDrv_GetSigLvl( int curf )
{
	return s_rssi;
}
void  FMDrv_Mute(unsigned char mute)
{
	tNmiMute fmmute;

	fmmute.mute=mute;
	nmi_drv_ctl(NMI_DRV_MUTE, (void*)&fmmute);	
	nmi_fm_set_flag();
}


void  FMDrv_SetVolumeLevel(unsigned char level)
{
	tNmiVolume volume;
	int volLevel=level;

	if(level  < 13)
	{
		volume.step= volLevel*7/12;	
		FMDrv_Mute(0);   
		nmi_drv_ctl(NMI_DRV_SET_VOLUME, (void *)&volume);
	}
	else if(level ==0)	
	{
		FMDrv_Mute(1);
	}
}

/*lint -restore */