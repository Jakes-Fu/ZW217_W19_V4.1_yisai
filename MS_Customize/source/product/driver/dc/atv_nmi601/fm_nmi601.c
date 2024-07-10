/************************************************************************
 *										
 *  This file is added by jackey.ling @Spreadtrum, to adapt the NMIFM driver.
 *	
 ***************************************************************************/


#include "ms_customize_trc.h"
#include "os_api.h"
#include "fm_drv.h"
#include "fm_cfg.h"
#include "nmidrv_fm.h"
#include "sensor_drv.h"
#include "deep_sleep.h"
#include "i2c_api.h"
#include "gpio_prod_cfg.h"
#include "gpio_prod_api.h"

#define NMI600_FREQ_SWITCH_UNIT 100000
#define NMI600_FREQ_UP_LIMIT 1080
#define NMI600_FREQ_DOWN_LIMIT 875
#define NMI600_FREQ_CNF_LIMIT 135
#define NMI600_RSSI_LIMIT 20

#define SENSOR_ONE_I2C	1
#define SENSOR_ZERO_I2C	0
#define SENSOR_16_BITS_I2C	2
#define SENSOR_I2C_FREQ      (400*1000) 
#define SENSOR_I2C_PORT_0		0
#define SENSOR_I2C_ACK_TRUE		1
#define SENSOR_I2C_ACK_FALSE		0
#define SENSOR_I2C_STOP    1
#define SENSOR_I2C_NOSTOP    0
#define SENSOR_I2C_NULL_HANDLE  -1
#define SENSOR_ADDR_BITS_8   1
#define SENSOR_ADDR_BITS_16   2
#define SENSOR_CMD_BITS_8   1
#define SENSOR_CMD_BITS_16   2
#define SENSOR_LOW_SEVEN_BIT     0x7f
#define SENSOR_LOW_EIGHT_BIT     0xff
#define SENSOR_HIGN_SIXTEEN_BIT  0xffff0000
#define SENSOR_LOW_SIXTEEN_BIT  0xffff



typedef enum
{
	FM_RSSI_0 = 0
	,FM_RSSI_1
	,FM_RSSI_2
	,FM_RSSI_3
	,FM_RSSI_4
	,FM_RSSI_MAX
}FM_RSSI;


LOCAL uint32 currFMFreq = 0;
LOCAL uint16 s_fm_seek_freq = 0;
LOCAL uint32 s_fm_seek_state = 0;
LOCAL BOOLEAN s_fm_init_state = 0;


//extern SENSOR_INFO_T g_nmi600_yuv_info;

LOCAL void GPIO_SetNmi600PowerOn (BOOLEAN power)
{	
    _GPIO_PROD_SetVal (GPIO_PROD_ATV_POWER_ID, power);  
    _GPIO_PROD_SetVal (GPIO_PROD_ATV_LDO_ID, power);	
}

LOCAL void Power_SetDefaultPower(BOOLEAN status)
{
	if(SCI_TRUE==status)
	{
		Sensor_SetVoltage(SENSOR_AVDD_1800MV, SENSOR_AVDD_2800MV, SENSOR_AVDD_2800MV);	// power up IIC 
		GPIO_SetNmi600PowerOn( TRUE );
	}
	else
	{
		Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED); //power down IIC
		GPIO_SetNmi600PowerOn( FALSE );
	}
}

LOCAL BOOLEAN FM_SearchNextChn()
{
	BOOLEAN ret = FALSE;
	uint32 freq = currFMFreq;
	uint32 rssi1 = 0;
	
	if(freq >= NMI600_FREQ_UP_LIMIT)
		freq = NMI600_FREQ_DOWN_LIMIT;
	else
		freq++;

	ret = FMDrv_ValidStop(freq, rssi1, 0);//para 2,3 unused
	
	currFMFreq = freq;

	return ret;
}

LOCAL BOOLEAN FM_SearchLastChn()
{
	BOOLEAN ret = FALSE;
	uint32 freq = currFMFreq;
	uint32 rssi1 = 0;
	
	if(freq <= NMI600_FREQ_DOWN_LIMIT)
		freq = NMI600_FREQ_UP_LIMIT;
	else
		freq--;

	ret = FMDrv_ValidStop(freq, rssi1, 0);//para 2,3 unused
	
	currFMFreq = freq;

	return ret;
}

LOCAL void FM_SetCurrFreq(uint32 ch)
{
	currFMFreq = ch;
}
LOCAL uint32 FM_GetCurrentCh()
{
	return currFMFreq;	
}

LOCAL uint32 _nmi600_FM_PowerOn(uint32 power_on)
{

    //BOOLEAN reset_level = (BOOLEAN)g_nmi600_yuv_info.reset_pulse_level;
    //uint32 reset_width = g_nmi600_yuv_info.reset_pulse_width;

    //SCI_TRACE_LOW:"[FM_DRV]:_nmi600_FM_PowerOn %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_133_112_2_18_0_29_7_609,(uint8*)"d",power_on);
    
    if(SCI_TRUE==power_on)
    {
        SCI_TV_EnableDeepSleep(DISABLE_AHB_SLEEP);       
        Power_SetDefaultPower(SCI_TRUE);
        SCI_Sleep(20);
        GPIO_SetAnalogTVResetLevel(0);
        SCI_Sleep(20);
        GPIO_SetAnalogTVResetLevel(1);
        SCI_Sleep(100);
    }
    else
    {   
        GPIO_SetAnalogTVResetLevel(20);    
        Power_SetDefaultPower(SCI_FALSE);
        SCI_TV_EnableDeepSleep(ENABLE_AHB_SLEEP);
    }
    
    return SCI_SUCCESS;
}


/*****************************************************************************/
//  FUNCTION:     NMI600_Initial
//  Description:  This function will init the NMI600 FM chip
//  return:     
//  Author:         
//  date:  
//	Note:         FM_SUCCESS: init success, FM_ERROR: init failed  
/*****************************************************************************/
LOCAL uint32 NMI600_Initial(void)
{
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_Initial()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_164_112_2_18_0_29_7_610,(uint8*)"");
	_nmi600_FM_PowerOn(SCI_TRUE);
	FMDrv_PowerOnReset();
	currFMFreq = NMI600_FREQ_DOWN_LIMIT;
	s_fm_seek_state = FM_SUCCESS; 
	s_fm_init_state = TRUE;
	return FM_SUCCESS;
}

/*****************************************************************************/
//  FUNCTION:     NMI600_PlayStart
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 NMI600_PlayStart(uint16 freq)
{
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_PlayStart() ,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_181_112_2_18_0_29_8_611,(uint8*)"d",freq);
    if(!s_fm_init_state)
    {
        NMI600_Initial();
    }
	FMDrv_SetFreq(freq);
    return FM_SUCCESS;
}

/*****************************************************************************/
//  FUNCTION:     NMI600_PlayStop
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL uint32 NMI600_PlayStop(void)  
{
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_PlayStop()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_198_112_2_18_0_29_8_612,(uint8*)"");
	
	FMDrv_PowerOffProc();
	// close it by power down
	if(s_fm_init_state)
    {
	    _nmi600_FM_PowerOn(SCI_FALSE);
        s_fm_init_state = FALSE;
    }
	return FM_SUCCESS;
}

/*****************************************************************************/
//  FUNCTION:     NMI600_Open
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void NMI600_Open(void)  
{
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_Open "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_218_112_2_18_0_29_8_613,(uint8*)"");
}

/*****************************************************************************/
//  FUNCTION:     NMI600_Close
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void NMI600_Close(void)  
{
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_Close "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_229_112_2_18_0_29_8_614,(uint8*)"");
	FMDrv_PowerOffProc();
	// DO NOT power off again
	if(s_fm_init_state)
    {
	    _nmi600_FM_PowerOn(SCI_FALSE);
        s_fm_init_state = FALSE;
    }
}

#if 0
/*****************************************************************************/
//  FUNCTION:     NMI600_PAUSE
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void NMI600_Pause(void)  
{
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_Pause "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_248_112_2_18_0_29_8_615,(uint8*)"");
	FMDrv_Mute(TRUE);
}


/*****************************************************************************/
//  FUNCTION:     NMI600_PAUSE
//  Description:  
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/
LOCAL void NMI600_Resume(void)  
{
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_Resume "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_261_112_2_18_0_29_8_616,(uint8*)"");
	FMDrv_Mute(FALSE);
}
#endif

/*****************************************************************************/
//  FUNCTION:     NMI600_ManualSeek
//  Description:  This function will tune to certain frequency
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL  uint32 NMI600_ManualSeek(uint16 freq,
                                            FM_SCAN_DIR_E  seek_dir, 
                                            uint8 steps, 
                                            FM_SINGLE_CHANNEL_INFO_T *single_channel_info)
{
	uint16 rssi = 0;	
	
    // 20110705: Will.Jiang_cr253058
    if(!s_fm_init_state)
    {
        NMI600_Initial();
    }

    FMDrv_ValidStop(freq, 0, 0);
	rssi = FMDrv_GetSigLvl(0);
	//SCI_TRACE_LOW:"[FM_DRV]NMI600_ManualSeek , rssi %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_287_112_2_18_0_29_8_617,(uint8*)"d",rssi);
		
    if(single_channel_info)
    {
    	if(rssi > 60)
    		single_channel_info->signal_strength = FM_SCANLEVEL_HIGH;
    	else if(rssi >46)
    		single_channel_info->signal_strength = FM_SCANLEVEL_MID;
    	else
    		single_channel_info->signal_strength = FM_SCANLEVEL_LOW;

    	single_channel_info->freq = freq;    
    }
        
	FMDrv_SetFreq(freq);	
		
	return FM_SUCCESS;
}
/*****************************************************************************/
//  FUNCTION:     NMI600_AutoSeek
//  Description:  This function will seek from current channel automaticly,if want to seek from certain channel,  
//  use NMI600_ManualSeek to certain channel ahead
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	

LOCAL  uint32 NMI600_AutoSeek( uint16 freq, 
                                         FM_SCAN_DIR_E seek_dir,
                                         	FM_SCAN_MODE_E mode, 
                                         		FM_ALL_CHANNEL_INFO_T  *all_channel_info )
{
    uint16 rssi = 0;	   
	BOOLEAN ret = FALSE;
	uint8 i = 0;
	SCI_ASSERT(	all_channel_info); /*assert verified*/
    // 20110705: Will.Jiang_ncr127162
    if(!s_fm_init_state)
    {
        NMI600_Initial();
    }
	// 20100107: willjiang: handle starting-seek-freq
	if(FM_NOT_COMPLETE == s_fm_seek_state)
	    FM_SetCurrFreq(s_fm_seek_freq); // if not complete, use previous value
	else
	    FM_SetCurrFreq(freq);
	    
	do{
			
		if(FM_SCAN_UP == seek_dir)
			ret = FM_SearchNextChn();
		else
			ret = FM_SearchLastChn();

        s_fm_seek_freq = FM_GetCurrentCh();

        // 20100107: willjiang: check if seek up to NMI600_FREQ_UP_LIMIT 
        if(NMI600_FREQ_UP_LIMIT == s_fm_seek_freq)
            break;
            
		i++;
	}	while((!ret) && (i<10) );

    rssi = FMDrv_GetSigLvl(0); 
/*lint -esym(613, all_channel_info) */    
	all_channel_info->freq_count = 1;
	all_channel_info->auto_freq_tab_ptr = &s_fm_seek_freq;

    
	if(ret&&(rssi>NMI600_RSSI_LIMIT))
	{
  		//FMDrv_SetFreq(FM_GetCurrentCh());	
		s_fm_seek_state = FM_SUCCESS;
	}
	else if(s_fm_seek_freq < NMI600_FREQ_UP_LIMIT)
	{
		s_fm_seek_state = FM_NOT_COMPLETE ;
	}	
	else   // 20100107: willjiang: here indicates seek up to NMI600_FREQ_UP_LIMIT 
	{
        s_fm_seek_freq = NMI600_FREQ_DOWN_LIMIT;
        all_channel_info->auto_freq_tab_ptr = &s_fm_seek_freq;	
        s_fm_seek_state = FM_SUCCESS; // return success to avoid WARNING
	}
	
	SCI_TRACE_LOW("[FM_DRV]NMI600_AutoSeek ret = %d, state = %d, freq = %d, rssi = %d",ret, s_fm_seek_state, all_channel_info->auto_freq_tab_ptr[0],rssi);	
	return s_fm_seek_state;
}


/*****************************************************************************/
//  Description:  This function control fm volume;
//                  value: 0~15
//  Author:        youwj
//  Note:           
/*****************************************************************************/
LOCAL uint32  NMI600_SetVol(FM_VOL_LEVEL_E value)   
{
    // 20110705: Will.Jiang_ncr127162
    if(!s_fm_init_state)
    {
        NMI600_Initial();
    }
	//SCI_TRACE_LOW:"NMI600_SetVol  ,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_387_112_2_18_0_29_8_619,(uint8*)"d" ,value);
	FMDrv_SetVolumeLevel(value);
	return FM_SUCCESS;
}
/*****************************************************************************/
//  FUNCTION:     NMI600_SetFMRegion
//  Description:  This function will select band region.
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32  NMI600_SetFMRegion(FM_REGION_E  region)  
{
    // 20110705: Will.Jiang_ncr127162
    if(!s_fm_init_state)
    {
        NMI600_Initial();
    }
	//SCI_TRACE_LOW:"NMI600_SetFMRegion "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_404_112_2_18_0_29_8_620,(uint8*)"");
	return FM_SUCCESS;	
}
/*****************************************************************************/
//  FUNCTION:     NMI600_SetScanLevel
//  Description:  This function will config scane rssi level
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	
LOCAL uint32 NMI600_SetScanLevel(FM_SCANLEVEL_E level)  
{
    // 20110705: Will.Jiang_ncr127162
    if(!s_fm_init_state)
    {
        NMI600_Initial();
    }
	//SCI_TRACE_LOW:"NMI600_SetScanLevel "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_NMI601_420_112_2_18_0_29_8_621,(uint8*)"");
	return FM_SUCCESS;
}

//Add by luther.li 2012-12-11 
/*****************************************************************************/
//  FUNCTION:     CL6012X_FM_Ioctl
//  Description:  This function will contrl fm command
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	 
LOCAL uint32 NMI600_FM_Ioctl(uint32 cmd, void *data)
{
    return 0;
}
  
//FM operations set
PUBLIC FM_OPERATION_T g_NMI600_ops =
{
	NMI600_Initial,   
	NMI600_Open,     
	NMI600_Close,   
	//NMI600_Pause,
	//NMI600_Resume,

	NMI600_PlayStart,
	NMI600_PlayStop,

	NMI600_ManualSeek,
	NMI600_AutoSeek,

	NMI600_SetScanLevel,
	NMI600_SetVol,
	NMI600_SetFMRegion,
	NMI600_FM_Ioctl
};


