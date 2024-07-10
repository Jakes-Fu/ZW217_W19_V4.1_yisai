/******************************************************************************
 ** File Name:      fm_config.c                                                  *
 ** Author:        YueHuaZhong                                             *
 ** DATE:           05/29/2006                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of FM.   *
 **                                                                           *
 ******************************************************************************/

#include "ms_customize_trc.h"
#include  "fm_drv.h"
#include  "lv24ekit.h"
#include "os_api.h"
#include "arm_reg.h"
#include "gpio_prod_api.h"
#include "sc6800_reg.h"
#include "ref_outport.h"
#include "hardwareio_cfg.h"

LOCAL FM_SEEK_TYPE_E s_fm_seek_type = FM_SEEK_TYPE_MAX;
LOCAL const uint16 FM_SCANLEVEL[3] = {0x01,0x02,0x03};   //user can add and modify it according to different FM Chips. 	   

PUBLIC uint8  g_fm_freq_count = 0;
PUBLIC uint16 g_fm_autoscan_freq_tab[30];

LOCAL BOOLEAN s_fm_first_play = SCI_FALSE;

#define FM_DEFAULT_FREQ			899

/*****************************************************************************/
//  Description:    This function is used to init FM chip 
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL uint32 InitChip(void)
{
   	uint32 rc;
    
   	OpenFMPA(SCI_FALSE);

    CtlPower(SCI_TRUE);

    Open32kClock( SCI_TRUE );

    OS_TickDelay(10);

    rc = InitLv2400xChip();    
    
    if(SCI_SUCCESS == rc)
    {
    	s_fm_first_play = SCI_TRUE;
    	SetFrequency(FM_DEFAULT_FREQ * 10);
    	OS_TickDelay(20);
    }
    else 
    {
    	// fail
    	
    	OpenFMPA(SCI_FALSE);

     	CtlPower(SCI_FALSE);
	
     	Close3wHardwareIo();
     
     	Open32kClock(SCI_FALSE);
    }
    
    return rc;
   
}


/*****************************************************************************/
//  Description:    This function is used to Open FM
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL void Open(void)
{
   //PowerOnLv2400xChip();
   //Init3wHardwareIo();
}


/*****************************************************************************/
//  Description:    This function is used to Close FM
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL void Close(void)
{
     OpenFMPA(SCI_FALSE);

     CtlPower(SCI_FALSE);
	
     ShutDownLv2400xChip();
     
     Open32kClock(SCI_FALSE);     
}
/*****************************************************************************/
//  Description:    This function is used to Play FM
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL uint32 PlayStart(uint16 freq)
{

    Open32kClock( SCI_TRUE );
	OpenFMPA(SCI_FALSE);
	
	if(s_fm_first_play)
	{
    	SetFrequency(freq * 10);
		SCI_Sleep(200);
		SetFrequency(freq * 10 - 10);
		SCI_Sleep(200);
		SetFrequency(freq * 10);

		s_fm_first_play = SCI_FALSE;
	}
	else
	{
		SetFrequency(freq * 10);	
	}

	OpenFMPA( SCI_TRUE );
	Open32kClock( SCI_FALSE );


	return FM_SUCCESS;

}
/*****************************************************************************/
//  Description:    This function is used to Stop FM
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL uint32 PlayStop()
{
    OpenFMPA(SCI_FALSE);

	return FM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to get seek type
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC FM_SEEK_TYPE_E FM_GetSeekType(void)
{
	return s_fm_seek_type;
}

/*****************************************************************************/
//  Description:    This function is used to set seek type
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void SetSeekType(FM_SEEK_TYPE_E seek_type)
{
	s_fm_seek_type = seek_type;
}

/*****************************************************************************/
//  Description:    This function is used to manual seek FM
//  Author:        yuehz
// 7 Note:           
/*****************************************************************************/
LOCAL uint32 ManualSeek(
							uint16 freq,
                            FM_SCAN_DIR_E  seek_dir, 
                            uint8 steps, 
                            FM_SINGLE_CHANNEL_INFO_T *single_channel_info
                        )
{
	uint16 dest_freq;
    uint8 i=steps;

    SetSeekType(FM_SEEK_TYPE_MANUAL);
     
    Open32kClock( SCI_TRUE );

    if(seek_dir==FM_SCAN_UP)
    {
        if((freq+i)*10>g_wDisplayBandHigh)
        {
            SetFrequency(g_wDisplayBandLow);
            dest_freq=g_wDisplayBandLow/10;
        }
        else
        {
            SetFrequency((freq+i)*10);
            dest_freq=freq+i;
        }
    }
    else
    {
        if((freq-i)*10<g_wDisplayBandLow)
        {
            SetFrequency(g_wDisplayBandHigh);
            dest_freq=g_wDisplayBandHigh/10;
        }
        else
        {
             SetFrequency((freq-i)*10);
             dest_freq=freq-i;
        } 
    }


    single_channel_info->freq = dest_freq;
    single_channel_info->signal_strength = GetFmFieldStrength();
    single_channel_info->is_stereo = GetStereoState();
	
    Open32kClock( SCI_FALSE );

	return FM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to auto seek FM
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL uint32 AutoSeek( 
						uint16 freq, 
                        FM_SCAN_DIR_E seek_dir,
                        FM_SCAN_MODE_E mode, 
                        FM_ALL_CHANNEL_INFO_T  *all_channel_info 
                     )
{
    uint8  scan_param = 0;
        
    OpenFMPA(SCI_FALSE);
	Open32kClock( SCI_TRUE );
	
	if(FM_SCAN_UP == seek_dir)
	{
	    scan_param |= 0x01;	
	}

	//SetFrequency(freq * 10);
	
	if(FM_SCAN_ALL == mode)
	{
		SetFrequency(freq * 10);
	    SetSeekType(FM_SEEK_TYPE_AUTO_ALL);
	}
	else if(FM_SCAN_ONE == mode)
	{
	   SetSeekType(FM_SEEK_TYPE_AUTO_ONE);
	}	
	
	ScanStation(scan_param);	
    
    all_channel_info->freq_count = g_fm_freq_count;
    all_channel_info->auto_freq_tab_ptr = &g_fm_autoscan_freq_tab[0];

    if(FM_SCAN_ONE == mode)
    {
    	all_channel_info->freq_count = 1;
        all_channel_info->auto_freq_tab_ptr[0]=GetDisplayFrequency(5) / 10;
    }
       

	Open32kClock( SCI_FALSE );
	OpenFMPA(SCI_TRUE);

	return FM_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to set scan level
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL uint32 SetScanLevel(FM_SCANLEVEL_E level)
{
  // int len = sizeof(FM_SCANLEVEL)/sizeof(FM_SCANLEVEL[0]);
     
    if(level < FM_SCANLEVEL_LOW)
	{
	    level = FM_SCANLEVEL_LOW;
	}
	else if(level > FM_SCANLEVEL_HIGH)
	{
		level =  FM_SCANLEVEL_HIGH;
	}
	
	g_byScanLevel = FM_SCANLEVEL[level];

	//SCI_TRACE_LOW:"FM: SetScanLevel %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_LV2400X_273_112_2_18_0_32_48_1444,(uint8*)"d", g_byScanLevel);
	
	return FM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to set FM play volume
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL uint32 SetVol(FM_VOL_LEVEL_E vol_level)
{
    uint8 byCurVolume;
	uint8 new_vol;
	int32 i;


#define FM_VOLUME_MIN		0
#define FM_VOLUME_MAX		20
#define FM_VOL_STEP			1
	

	Open32kClock( SCI_TRUE );	
	
	byCurVolume =GetChipVolume();
	
	if(byCurVolume > FM_VOLUME_MAX)
	{
		byCurVolume = FM_VOLUME_MAX;
	}

	if(vol_level > FM_VOL_LEVEL_7)
	{
		vol_level = FM_VOL_LEVEL_7;
	}
	else if(FM_VOL_LEVEL_0 > vol_level)
	{
		vol_level = FM_VOL_LEVEL_0;
	}
	if(vol_level == FM_VOL_LEVEL_0)
	{
		SetChipAudioMute(TRUE, MUTESRC_VOLUME);
		return SCI_SUCCESS;
	}
	else
	{
		SetChipAudioMute(FALSE, MUTESRC_VOLUME);
	}
	
	new_vol = vol_level * FM_VOL_STEP + 6;
	

	if(byCurVolume < new_vol)
	{
		for(i = byCurVolume + 1; i <= new_vol; i++)
		{
			SetChipVolume(i);	
			OS_TickDelay(50);
		}
	}
	else
	{
		for(i = byCurVolume - 1; i >= new_vol; i--)
		{
			SetChipVolume(i);		
			OS_TickDelay(50);
		}
	}

	Open32kClock( SCI_FALSE );

	return SCI_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to set FM play volume
//  Author:        yuehz
//  Note:           
/*****************************************************************************/
LOCAL uint32  SetFMRegion(FM_REGION_E  region)
{
    g_byRegion=(uint8)region;
    
    SetRegion(g_byRegion);

    return FM_SUCCESS;

}

//Add by luther.li 2012-12-11 
/*****************************************************************************/
//  FUNCTION:     Lv2400x_FM_Ioctl
//  Description:  This function will contrl fm command
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	 
LOCAL uint32 Lv2400x_FM_Ioctl(uint32 cmd, void *data)
{
    return 0;
}

//FM operations set
PUBLIC FM_OPERATION_T g_Lv2400x_ops=
{
    InitChip,   
    Open,     
    Close ,   

    PlayStart,
    PlayStop,

    ManualSeek,
    AutoSeek,

    SetScanLevel,
    SetVol,

    SetFMRegion,
    Lv2400x_FM_Ioctl
};





 
