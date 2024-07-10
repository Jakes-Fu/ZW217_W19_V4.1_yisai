/*ATV 3rd Party SOURCE PCLINT Inhibition */
/*lint -e766*/

// ---------------------------------------------------------------------------
// Copyright 2009-2010 (c) RDA Microelectronics, Inc.
//
// File: rda5888sprd.c
// Revision: 0
// Author: wuhp
// Date: 2010-01-27
// Description: 
// ---------------------------------------------------------------------------

#include "rda5888sprd.h"
#include "rda5888app.h"
#include "fm_cfg.h"
#include "os_api.h"
#include "iram_mgr.h"
//#include "isp_service.h"
//#include "TV_Driver.h"
#include "deep_sleep.h"
#include "sci_types.h"
#include "i2c_api.h"
//#include "sensor_drv.h"
//#include "ldo_drvapi.h"
#include "sensor_drv.h"
//#include "I2c_drv.h"
//#include "os_api.h"
#include "fm_drv.h"
#include "Arm_reg.h"
#include "rda5888fm.h"

typedef struct _rdaatv_signal_tag
{
    xSignalHeaderRec   		sig;

    uint32        						service;
    uint32        						command;    
    uint32 								param0;
    uint32								param1;
    void          						*ptr;
} RDAATV_SIGNAL_T;

#define RDAATV_STACK_SIZE  	(1024*4) 
#define RDAATV_QUEUE_NUM    (50)

SCI_TIMER_PTR g_ptRdaAtvTimer = NULL;

// wuhp@20100331, add for create thread.
LOCAL uint32 s_rda_atv_thread_id = SCI_INVALID_BLOCK_ID;

LOCAL BOOLEAN s_rda_fm_init_state = FALSE;


static uint32 scan_start_chn = 0;
static uint32 scan_end_chn = 0;
static uint32 scan_chn_index = 0;

void RDAEXT_DelayMs(uint32 ms)
{
    SCI_Sleep(ms);
}

void RDAATV_InitScanParam(int region)
{
	scan_start_chn = 1;
	scan_end_chn = RDAAPP_GetChannelCount();

	scan_chn_index = scan_start_chn;

	SCI_TRACE_LOW("RDAATV_InitScanParam,  region = %d, scan_start_chn = %d,  scan_end_chn= %d \r\n", region, scan_start_chn, scan_end_chn);
}

uint32 RDAATV_ScanChn(uint32* chn_id, uint32* is_valid)
{
    int lock;

    SCI_TRACE_LOW("RDAATV_ScanChn, scan_chn_index = %d, end: %d \r\n", scan_chn_index, scan_end_chn);

    if (scan_chn_index > scan_end_chn)
    {
        return TRUE;
    }

    lock = RDAAPP_SetChannel(scan_chn_index, 1);  // scan one channel.

    *chn_id = scan_chn_index; 

    if (lock)
    {
        *is_valid = TRUE;
    }
    else
    {
        *is_valid = FALSE;
    }

    scan_chn_index++;

    return FALSE;
}

uint32 RDAATV_GetCurrSearchNum(void)
{
	return scan_chn_index;
}

uint32 RDAATV_GetTotalSearchNum(void)
{
	return scan_end_chn;
}

void RDAAPP_InitVideoSize(BOOLEAN IsFullScreen)
{
	SCI_TRACE_LOW("[rdamtv]RDAAPP_InitVideoSize, fullscreen:%d",IsFullScreen);
/*
    if (RDAAPP_GetTPMode())
    {
        //TV_SetISPTVSizeForColorBar();
    }
	else
	{
        //TV_SetISPTVSize(TV_IsNTSCVideoSize());
	}
*/
}

// ---------------------------------------------------------------------------
// Below section is FM Driver for SPRD.
// ---------------------------------------------------------------------------
#if 1 //def ANALOG_TV_TYPE_RDA5888 


//================================================================
//API for Spreadtrum 
//================================================================
#if 1//def FM_SUPPORT_RDA5888
uint32 RDAFM2_Initial(void);
void RDAFM2_Open(void);
void RDAFM2_Close(void);
uint32 RDAFM2_PlayStart(uint16 freq);
uint32 RDAFM2_PlayStop(void);
uint32 RDAFM2_ManualSeek( uint16 freq,FM_SCAN_DIR_E seek_dir, 
						  uint8 steps, 
						  FM_SINGLE_CHANNEL_INFO_T *single_channel_info );
uint32 RDAFM2_AutoSeek(	uint16 freq, 
						FM_SCAN_DIR_E seek_dir,FM_SCAN_MODE_E mode,
						FM_ALL_CHANNEL_INFO_T  *all_channel_info );
uint32 RDAFM2_SetScanLevel(FM_SCANLEVEL_E level);
uint32 RDAFM2_SetVol(FM_VOL_LEVEL_E vol_level);
uint32 RDAFM2_SetFMRegion(FM_REGION_E region);
#endif
/***************************************************
* Extern variables
****************************************************/

/***************************************************
* Local variables
****************************************************/
#if 1 //def FM_SUPPORT_RDA5888
static FM_FREQ_RANGE_T s_fm_freq_range;
//static uint16 RDAFMChipID;
//static uint16 FMshadowReg[45];
static uint16 g_fm_autoscan_freq_tab[20];
static FM_REGION_E g_fm_region=FM_REGION_US;
static FM_SCANLEVEL_E g_fm_seekth=FM_SCANLEVEL_MID;
static uint16 m_nCurFmFreq = 0;
//static uint8 m_nFmSeekThreshold = 5; 

#endif

#if 1 //def FM_SUPPORT_RDA5888

LOCAL void RDAFM2_SystemDelay(uint32 ms)
{
    uint32 end = 0;
	if (SCI_InThreadContext()) 
	{
		SCI_Sleep(ms);
	}
	else 
	{		
		end = SCI_GetTickCount() + ms;
		while (SCI_GetTickCount() < end)
		{
		    ;
		}
	}	
}




//================================================================
LOCAL uint8 FmInit(void)
{
    /*if (1)
    {
        LDO_SetVoltLevel (LDO_LDO_AUX1, LDO_VOLT_LEVEL0);
        LDO_TurnOnLDO (LDO_LDO_AUX1);

        LDO_ControVCAMPad (SCI_TRUE);
        LDO_SetVoltLevel (LDO_LDO_AUX2, LDO_VOLT_LEVEL0);
        LDO_TurnOnLDO (LDO_LDO_AUX2);

    }
    else
    {
        LDO_TurnOffLDO (LDO_LDO_AUX1);
        LDO_ControVCAMPad (SCI_FALSE);
        LDO_TurnOffLDO (LDO_LDO_AUX2);
    }*/
    
    Sensor_SetSensorType(SENSOR_TYPE_ATV);
	Sensor_Init();
	Sensor_Open();
	RDA5888FM_Init();
	return 1;
}

//================================================================
LOCAL uint8 FmGetCurrentState (FM_SINGLE_CHANNEL_INFO_T  *single_channel_info)
{
	single_channel_info->freq = m_nCurFmFreq;
	single_channel_info->is_stereo= 0;
	single_channel_info->signal_strength = RDA5888FM_GetRSSI();

	return 1;
}

//================================================================
LOCAL uint8 FmSetFMRegion(FM_REGION_E region)
{
	return 1;
}

//================================================================
LOCAL uint8 FmSetScanLevel(FM_SCANLEVEL_E level)
{
	uint8 destlevel;
	
	switch( level )
	{
		case FM_SCANLEVEL_LOW:
				destlevel=0x03;
			break;
		case FM_SCANLEVEL_MID:
				destlevel=0x05;
			break;
		case FM_SCANLEVEL_HIGH:
				destlevel=0x07;
			break;
		default:
			break;
	}

	//m_nFmSeekThreshold = level;
	
	return 1;
}	

//================================================================
LOCAL uint8 FmSeek(uint8 seekDirection)
{
	uint8 fTemp=0;
	uint8 i=0;
	uint16 dest_freq = m_nCurFmFreq;
	
	RDA5888FM_SetMute(1);
	
	for (i = 0; i <= (s_fm_freq_range.max_freq - s_fm_freq_range.min_freq); i++)
	{
		if (seekDirection==1)	// seek up
		{
			dest_freq++;
			if (dest_freq > s_fm_freq_range.max_freq)
			{
				dest_freq = s_fm_freq_range.min_freq;
			}
		}
		else
		{
			dest_freq--;
			if (dest_freq < s_fm_freq_range.min_freq)
			{
				dest_freq = s_fm_freq_range.max_freq;
			}
		}
		
		//check whether SF==1 STC==0 & RSSI value//Skip 960
		if (RDA5888FM_IsValidFreq(dest_freq))
		{
			fTemp = 1 ;
			break;
		}
	}

	RDA5888FM_SetMute(0);
	m_nCurFmFreq = dest_freq;

	if(!fTemp)
		return(0);		
	else
		return(1);
}

//================================================================
LOCAL uint16 FmSetFreq(uint16 curf)
{
    RDA5888FM_SetMute(0);
	RDA5888FM_SetFreq(curf);

	m_nCurFmFreq = curf;
	
	return 1;
}

//================================================================
LOCAL uint16 FmGetFreq(void)
{
	return (m_nCurFmFreq);
}

//================================================================
LOCAL  uint8 FmEnterSleep(void) 
{
    //TV_DeinitGpio();

	return 1;	
}

//================================================================
LOCAL uint8 FmExitSleep(void)
{
    FmInit();
	
	return 1; 	
}

//================================================================
LOCAL uint8 FmSetMute(void)
{
    RDA5888FM_SetMute(1);
		
	return 1;
}

//================================================================
LOCAL uint8 FmSetNoMute(void)
{
    RDA5888FM_SetMute(0);
	
	return 1;
}

//================================================================
LOCAL uint8 FmSetVolume(uint8 value)//0~9
{
#if (RDA5888_CHIP_VER == 4)
	uint16 arrVolMap[] = {0, 3, 7, 12, 18, 25, 28, 38, 41, 45};
#elif (RDA5888_CHIP_VER == 5)
	uint16 arrVolMap[] = {0, 1, 3, 5, 7, 10, 12, 15, 18, 22};
#elif (RDA5888_CHIP_VER == 6)
    uint16 arrVolMap[] = {0, 1, 6, 10, 15, 22, 24, 27, 30, 32};
#elif (RDA5888_CHIP_VER == 7) // 0~15
    uint16 arrVolMap[] = {0, 2, 4, 6, 7, 8, 9, 11, 13, 15};
#endif

    if (value > 9)
		value = 9;

	SCI_TRACE_LOW("[rdamtv]FmSetVolume, value = %d\n", value); 

	if (value == 0)
	{
	    FmSetMute();
	}
	else
	{
	    RDAAPP_SetVolume(arrVolMap[value]);
	    FmSetNoMute();
	}

	return 1;		
}

//================================================================
LOCAL uint8 FmSeekAllChannel(	uint16 bottomOfBand, 
							uint16 topOfBand, 
							uint16 maxChanNum, 
							uint16 *pList) 
{
	uint8 num=0;
	uint16 curFreq=0;

	RDA5888FM_SetMute(1);
	for( curFreq=bottomOfBand; curFreq<=topOfBand; curFreq++ )
	{
		if( RDA5888FM_IsValidFreq(curFreq) )
		{
			*pList= curFreq ;
			pList++;
			num++;
			if (num>=maxChanNum)
			break;
		}
	}

	RDA5888FM_SetMute(0);

	return (num);
}

//extern PUBLIC void FM_ConfigAudioPath (BOOLEAN param);

//================================================================
//API for Spreadtrum 
//================================================================
uint32 RDAFM2_Initial(void)
{
	SCI_TRACE_LOW("[rdamtv]RDAFM2_Initial\n"); 

	RDAFM2_SystemDelay(50);
#ifdef RDA5888_AUXCLK_26M_ENABLE
    SCI_TRACE_LOW("RDAFM2_Initial, RDA5888_AUXCLK_26M_ENABLE is defined, MTVCFG_Open_26MClock() start ");
	MTVCFG_Open_26MClock();
#endif 
	
#ifdef RDA5888_26MCRYSTAL_ENABLE 
    SCI_TRACE_LOW("RDAFM2_Initial, RDA5888_26MCRYSTAL_ENABLE is defined, SCI_TV_EnableDeepSleep(DISABLE_AHB_SLEEP) start ");
  	SCI_TV_EnableDeepSleep(DISABLE_AHB_SLEEP);
#endif
	FmInit();
	FmSetFMRegion(g_fm_region);
	RDAFM2_SetScanLevel(g_fm_seekth);

	//FM_ConfigAudioPath(TRUE);
	s_rda_fm_init_state = TRUE;
	return(FM_SUCCESS);
}

//================================================================
void RDAFM2_Open(void)
{
	SCI_TRACE_LOW("[rdamtv]RDAFM2_Open\n"); 
	FmExitSleep();
#ifdef RDA5888_26MCRYSTAL_ENABLE
	SCI_TV_EnableDeepSleep(DISABLE_AHB_SLEEP);
#endif
}

//================================================================
void RDAFM2_Close(void)
{
	SCI_TRACE_LOW("[rdamtv]RDAFM2_Close\n");

   	//RDA5888_SPRD_Leave();
    //GPIO_TVPowerEn(FALSE);
	//TV_AudioDeinit();
	RDA5888FM_SetMute(0);
	FmEnterSleep();
#ifdef RDA5888_26MCRYSTAL_ENABLE
	SCI_TV_EnableDeepSleep(ENABLE_AHB_SLEEP);
#endif
#ifdef RDA5888_AUXCLK_26M_ENABLE
	MTVCFG_Close_26MClock();
#endif
    //harry
    //_Fm_IicHandlerRelease();
}

//================================================================
uint32 RDAFM2_PlayStart(uint16 freq)
{
	SCI_TRACE_LOW("[rdamtv]RDAFM2_PlayStart, freq = %d\n", freq); 

	if(!s_rda_fm_init_state)
    {
        RDAFM2_Initial();
    }

	if ( FmSetFreq(freq) )
		return FM_SUCCESS;
	else
		return FM_ERROR;
}

//================================================================
uint32 RDAFM2_PlayStop(void)
{
    SCI_TRACE_LOW("[rdamtv]RDAFM2_PlayStop\n");
    s_rda_fm_init_state = FALSE;
	if (FmSetMute())
	{
		return FM_SUCCESS;
    }
	else
	{
		return FM_ERROR;
    }
}

//================================================================
uint32 RDAFM2_ManualSeek(	uint16 freq,FM_SCAN_DIR_E seek_dir, 
								uint8 steps, 
								FM_SINGLE_CHANNEL_INFO_T *single_channel_info )
{
	uint16 dest_freq;
	uint8 fTemp = 0;

	SCI_TRACE_LOW("[rdamtv]RDAFM2_ManualSeek, freq = %d, steps = %d\n", freq, steps); 
	if (seek_dir == FM_SCAN_UP)
	{
		if((freq+steps) > s_fm_freq_range.max_freq)
		{
			dest_freq = s_fm_freq_range.min_freq;
		}
		else
		{
			dest_freq = freq+steps;
		}
	}
	else
	{
		if((freq-steps) < s_fm_freq_range.min_freq)
		{
			dest_freq = s_fm_freq_range.max_freq;
		}
		else
		{
			 dest_freq = freq-steps;
		} 
	}
	
	fTemp = FmSetFreq(dest_freq);

	if(PNULL != single_channel_info)
	{
		FmGetCurrentState(single_channel_info);
	}

	if (fTemp)	
		return FM_SUCCESS;
	else
		return FM_ERROR;
}

//================================================================
uint32 RDAFM2_AutoSeek(uint16 freq, 
								FM_SCAN_DIR_E seek_dir,FM_SCAN_MODE_E mode,
								FM_ALL_CHANNEL_INFO_T  *all_channel_info )
{
	uint8 fTemp=0;

	SCI_TRACE_LOW("[rdamtv]RDAFM2_AutoSeek\n"); 
	if(FM_SCAN_ALL == mode)
	{
		all_channel_info->freq_count =	FmSeekAllChannel(s_fm_freq_range.min_freq, s_fm_freq_range.max_freq, sizeof(g_fm_autoscan_freq_tab) , &g_fm_autoscan_freq_tab[0] );
		all_channel_info->auto_freq_tab_ptr = &g_fm_autoscan_freq_tab[0];
		fTemp=all_channel_info->freq_count;
	}
	else if(FM_SCAN_ONE == mode)
	{
		if(seek_dir==FM_SCAN_UP)
			fTemp=FmSeek(1);
		else
			fTemp=FmSeek(0);	
		g_fm_autoscan_freq_tab[0]=FmGetFreq();
		all_channel_info->freq_count = 1;
		all_channel_info->auto_freq_tab_ptr= &g_fm_autoscan_freq_tab[0];
	}	
	if ( fTemp )	
		return FM_SUCCESS;
	else
		return FM_ERROR;
}

//================================================================
uint32 RDAFM2_SetScanLevel(FM_SCANLEVEL_E level)
{
	SCI_TRACE_LOW("[rdamtv]RDAFM2_SetScanLevel\n"); 
	
	g_fm_seekth = level;

	if ( FmSetScanLevel(g_fm_seekth) )
		return FM_SUCCESS;
	else
		return FM_ERROR;	
}

//================================================================
uint32 RDAFM2_SetVol(FM_VOL_LEVEL_E vol_level)
{
	SCI_TRACE_LOW("[rdamtv]RDAFM2_SetVol\n"); 

	if (FmSetVolume(vol_level))		 //modify by lei.hong for set FM volume 
		return FM_SUCCESS;
	else
		return FM_ERROR;	
}

//================================================================
uint32 RDAFM2_SetFMRegion(FM_REGION_E region)
{
	SCI_TRACE_LOW("[rdamtv]RDAFM2_SetFMRegion\n"); 
	
	switch(region)
	{
		case FM_REGION_US:
			s_fm_freq_range.max_freq=1080;
			s_fm_freq_range.min_freq=875;
			break;
		case FM_REGION_JAPAN:
			s_fm_freq_range.max_freq=910;
			s_fm_freq_range.min_freq=7600;
			break;			
		default:
			break;
	}
	g_fm_region = region;
	
	if (FmSetFMRegion(g_fm_region) )
		return FM_SUCCESS;
	else
		return FM_ERROR;	
}

void RDAFM2_SetMute(BOOLEAN onoff)
{
		SCI_TRACE_LOW("[rdamtv]RDA5888FM_SetMute, onoff = %d\n"); 
	 if (onoff) 							//modify by lei.hong to adjust the up layer function
		FmSetNoMute();
	 else
		FmSetMute();
}

//FM operations set
PUBLIC FM_OPERATION_T g_RDA5888_FM_ops=
{
	RDAFM2_Initial,	 
	RDAFM2_Open, 	
	RDAFM2_Close ,	

	RDAFM2_PlayStart,
	RDAFM2_PlayStop,

	RDAFM2_ManualSeek,
	RDAFM2_AutoSeek,

	RDAFM2_SetScanLevel,
	RDAFM2_SetVol,

	RDAFM2_SetFMRegion
};

#endif

void RDA_Chip_Identify(void)
{
	unsigned short reg;
	RDA5888_ReadReg(0x0, &reg);
	SCI_TRACE_LOW("[rdamtv]chipid: %x\n", reg); // 0x31E2
}

void RDAAPP_TimerHandler(uint32 param)
{
    RDAATV_SIGNAL_T     *sig_ptr = NULL;
    xSignalHeaderRec    *sig=NULL;
    
    sig_ptr = (RDAATV_SIGNAL_T *)SCI_ALLOC(sizeof(RDAATV_SIGNAL_T));

//    SCI_PASSERT(SCI_NULL!= sig_ptr, ("ISP_SERVICE: the sig_ptr is PNULL"));  /*assert verified*/

    sig = &sig_ptr->sig;
    sig->SignalSize  = sizeof(RDAATV_SIGNAL_T);
    sig->Sender      = s_rda_atv_thread_id;
    sig->SignalCode  = 0;
    sig_ptr->command = 0 ;
    sig_ptr->service = 0 ;
    sig_ptr->ptr     = NULL ;
    sig_ptr->param0  = 0;
    sig_ptr->param1  = 0;
    
    SCI_SendSignal((xSignalHeader)sig_ptr, s_rda_atv_thread_id);	
}

#define RDAATV_THREAD_DEBUG_MSG

LOCAL void _rda_atv_thread(void)//(uint32 argc, void *argv)
{
    RDAATV_SIGNAL_T *sig_ptr=PNULL;

    while (TRUE)/*lint !e716 */
    {
        sig_ptr = (RDAATV_SIGNAL_T *)SCI_GetSignal(s_rda_atv_thread_id);

#ifdef RDAATV_THREAD_DEBUG_MSG
        SCI_TraceLow("[rdamtv]_rda_atv_thread, curr_tick = %d\n", SCI_GetTickCount());
#endif

		RDAAPP_RealTimeProcess();

        SCI_FREE(sig_ptr);
        sig_ptr = PNULL;
    }
}

LOCAL void RDA_CreateThread(void)
{
    if(SCI_INVALID_BLOCK_ID == s_rda_atv_thread_id)
    {
        s_rda_atv_thread_id = SCI_CreateAppThread(
                                                    "T_RDAATV_SERVICE",
                                                    "T_RDAATV_QUEUE",
                                                    _rda_atv_thread,/*lint !e64 */
                                                    0,
                                                    0,
                                                    RDAATV_STACK_SIZE,
                                                    RDAATV_QUEUE_NUM,
                                                    29,
                                                    SCI_PREEMPT,
                                                    SCI_AUTO_START);

    }
}

PUBLIC void RDA5888_SPRD_Enter(void)
{
    /*RDA Test Area*/
    RDA_Chip_Identify();
    /*RDA Test Area*/

#ifdef RDA5888_VIDENHANCE_ENABLE
    ISP_ATV_FrameProcessFuncRegister(&RDAAPP_ATVFrameProcess);
#endif

	RDA_CreateThread();
	
	if (g_ptRdaAtvTimer == NULL)
	{
	   g_ptRdaAtvTimer = SCI_CreateTimer("T_ATV_TIMER", RDAAPP_TimerHandler,0,RDAAPP_TIMER_CYCLE,SCI_NO_ACTIVATE);
       SCI_TRACE_LOW( "[rdamtv]TV_PowerOn, start::timer\n");
	} 
}

PUBLIC void RDA5888_SPRD_Leave(void)
{
    if (g_ptRdaAtvTimer != NULL) 
	{
	    SCI_DeleteTimer(g_ptRdaAtvTimer);
		g_ptRdaAtvTimer = NULL;
	}
	
#if ((RDA5888_CHIP_VER == 6) || (RDA5888_CHIP_VER == 7))		
	RDAAPP_Exit();
#endif
}


#endif // END RDA888_FM_ENABLE

/*lint +e766*/
