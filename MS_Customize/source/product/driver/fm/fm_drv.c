/******************************************************************************
 ** File Name:      fm_drv.c                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           02/09/2006                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of FM.   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/26/2005     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 

#include "ms_customize_trc.h"
#include "os_api.h"
#include "fm_drv.h"
#include "fm_cfg.h"
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "_reg_ahb.h"
#include "_reg_apb.h"
#include "_hardware.h"
#include "trout_fm_reg_def.h"
#include "trout_drv.h"
#ifndef MODEM_PLATFORM
#include "trout_test_fm.h"
#endif
#include "sci_types.h"
#include "chip_drv_common_io.h"
#include "reg_check.h"
#include "tb_comm.h"
#include "trout_test.h"
//#include "Uart_drv.h"
#include "sprd_reg.h"
#include "sprd_reg_int.h"
#include "SC6531EFM_FM_RF_reg_addr.h"
#include "SC6531EFM_FM_reg_por_value.h"
#include "nv_item_id.h"
#include "nvitem.h"
//#include "fmd.h"
//#include "../../../../../chip_drv/chip_plf/sc6531efm/sc6531efm_int_cfg.h"
#endif
#include "pmic_26M_clock.h"
#ifndef MODEM_PLATFORM
#ifdef FM_SUPPORT_SPRD_V0
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#define DEBUG_FM
#define FM_TAB_NUMS   20
#define SEARCH_TIME  500

#ifdef DEBUG_FM
#define FM_PRINT   SCI_TRACE_LOW 
#else
#define FM_PRINT( _format_string )  
#endif
 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/


typedef enum
{
	FM_STATUS_NOT_INIT = 0,
	FM_STATUS_CLOSE,	
	FM_STATUS_INIT,
	FM_STATUS_OPEN,
	FM_STATUS_PLAYING,
	FM_STATUS_STOP,

	FM_STATUS_MAX			/*lint -esym(749, FM_STATUS_MAX)*/
}FM_STATUS_E;


//fm region and frequency range relationship
typedef struct
{
      FM_REGION_E    fm_region;
      FM_FREQ_RANGE_T   fm_freq_range;
}FM_REGION_FREQRANGE_T;

typedef struct
{
    UINT8 search_type;
    FM_SCAN_DIR_E search_direct;
    UINT8 search_timer;
    UINT8 rssi[FM_TAB_NUMS];
}FM_Search_status_t;


typedef struct
{
  UINT8 status;           // (1:playing ; 2:pause ; 3(searching).
  UINT8 station;          //当前电台对应的电台表项号。
  UINT32  freq;             //当前电台的频率。单位为1k (最高位恒为1)
  UINT8 stereo_status;
  UINT8 rssi;
  UINT8 tab_num;
  UINT16 tabs[FM_TAB_NUMS];
  UINT16 magic;
}FM_play_status_t;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

FM_play_status_t    *FMStatus;                //当前电台信息
FM_Search_status_t  *FmSearchPtr = NULL;

uint16   fm_auto_freq_tab[100];
uint16   fm_auto_seek_freq=101700;
/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
 LOCAL FM_STATUS_E s_fm_status = FM_STATUS_NOT_INIT;

 LOCAL FM_OPERATION_T *s_fm_operations = PNULL;
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
 FM_OPERATION_T s_fm_operations_def = 
 	{
  	 FM_OPERATION_init,
  	FM_OPERATION_open,
  	FM_OPERATION_close,
   	FM_OPERATION_playstart,            //play FM
   	FM_OPERATION_playstop,            //stop FM   
   	FM_OPERATION_manualseek,    //manual seek
   	FM_OPERATION_autoseek,                //auto seek
   	FM_OPERATION_setscanlevel,     //set fm scan level
   	FM_OPERATION_setvol,               //set fm play volume
   	FM_OPERATION_setfmregion,    //set fm region
   	FM_OPERATION_ioctl
 	};
#else
 
    FM_OPERATION_T s_fm_operations_def = {PNULL};
#endif
 LOCAL FM_FREQ_RANGE_T s_fm_freq_range = {0};

 LOCAL FM_OUTPUT_MODE_E s_fm_output_path = FM_OUTPUT_HEADSET;
 
 LOCAL BOOLEAN s_is_output_disabled = SCI_FALSE;
 
 LOCAL const  FM_REGION_FREQRANGE_T s_region_freqrange[]=
 	{
 	   { FM_REGION_US,  		{87000,108000} 	},
 	   { FM_REGION_JAPAN,  		{76000,90000} 	},
 	   { FM_REGION_JAPAN_WIDE, 	{76000,108000} 	}
 	};
 	   
LOCAL FM_TYPE_E s_fm_type = FM_TYPE_INVALID;

uint8 g_fm_longant_condition1_rssi = 29;
uint8 g_fm_longant_condition1_offset = 30;
uint8 g_fm_longant_condition1_noise = 66;

uint8 g_fm_longant_condition2_rssi = 20;
uint8 g_fm_longant_condition2_offset = 11;
uint8 g_fm_longant_condition2_noise = 68;

uint8 g_fm_longant_condition3_rssi = 0xFF;
uint8 g_fm_longant_condition3_offset = 11;
uint8 g_fm_longant_condition3_noise = 60;

uint8 g_fm_longant_condition4_rssi = 40;
uint8 g_fm_longant_condition4_offset  = 11;
uint8 g_fm_longant_condition4_noise  = 0xFF;

#ifdef FM_S_ANT_SUPPORT
uint8 g_fm_shortant_condition1_rssi = 29;
uint8 g_fm_shortant_condition1_offset  = 16;
uint8 g_fm_shortant_condition1_noise  = 69;

uint8 g_fm_shortant_condition2_rssi = 0xFF;
uint8 g_fm_shortant_condition2_offset  = 11;
uint8 g_fm_shortant_condition2_noise  = 60;

uint8 g_fm_shortant_condition3_rssi = 35;
uint8 g_fm_shortant_condition3_offset  = 11;
uint8 g_fm_shortant_condition3_noise  = 0xFF;
#endif
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to Set FM status.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
void FM_main(void);
 uint32 fm_cali_run_flag=0;
LOCAL void FM_SetStatus(FM_STATUS_E status)
{
	//FM_PRINT:"FM_SetStatus: status = %d, s_fm_status = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_108_112_2_18_0_32_43_1392,(uint8*)"dd", status, s_fm_status);

	if(s_fm_status != status)
	{
	   s_fm_status = status;
	}

}

/*****************************************************************************/
//  Description:    This function is used to get FM status.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL FM_STATUS_E FM_GetStatus(void)
{
	return s_fm_status;
}

/*****************************************************************************/
//  Description:    This function is used to delay system.    
//  Author:        
//  Note:           
/*****************************************************************************/

LOCAL void FM_SystemDelay(uint32 ms)
{
	if (SCI_InThreadContext()) 
	{
		SCI_Sleep(ms);
	}
	else 
	{
		uint32 end = SCI_GetTickCount() + ms;
		while (SCI_GetTickCount() < end){};
	}	
}


/*****************************************************************************/
//  Description:  set FM into sleep mode during phone power on
//  Note:  fm_ops[i]->close() should implement "entersleep" always         
/*****************************************************************************/
PUBLIC uint32 FM_Entersleep(void)
{
    int32 i;
    FM_OPERATION_T** fm_ops=PNULL;
    
	//FM_PRINT:"FM_Entersleep"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_150_112_2_18_0_32_44_1393,(uint8*)"");

	// First, Open clock and LDO
	FM_ConfigClk(SCI_TRUE);
	FM_ConfigPower(SCI_TRUE);

    fm_ops=(FM_OPERATION_T**)FM_GetOpsTab();
    for(i = 0; i <= (int32)(FM_GetOpsTabLen() -2); i++)
    { 
       if(fm_ops[i] && (fm_ops[i]->close))
       {
          fm_ops[i]->close();
          break;
       }
    }
    // close clock and power
    FM_ConfigClk(SCI_FALSE);
    FM_ConfigPower(SCI_FALSE);     
    
	return FM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to init FM    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_Init(void)
{
    uint8 rc = FM_ERROR;

    SCI_TRACE_LOW("FM_Init 0x%x",s_fm_operations);
    //FM_PRINT:"FM_Init"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_179_112_2_18_0_32_44_1394,(uint8*)"");
    // 20101220: will.jiang_cr215111 : init() always
    // SCI_TRACE_LOW("FM_SUCCESS= %d,FM_ERROR= %d\n",FM_SUCCESS,FM_ERROR);
    // First, Open clock and LDO
    // FM_ConfigClk(SCI_TRUE);
    // FM_ConfigPower(SCI_TRUE);
    //SCI_TRACE_LOW("fm_5\n");
    // Secord, select the right FM solution from table

    if (PNULL == s_fm_operations)   //fm has not been initialized yet.
    {
        int32 i;
        FM_OPERATION_T** fm_ops=PNULL;

        fm_ops=(FM_OPERATION_T**)FM_GetOpsTab();
        for(i = 0; i <= (int32)(FM_GetOpsTabLen() -2); i++)
        { 
            if(PNULL != fm_ops[i])
            {
                if(PNULL != (fm_ops[i]->init))
                {
                    if(FM_SUCCESS == (fm_ops[i]->init()))
                    {
                        s_fm_operations = fm_ops[i];
                        SCI_DEBUG("FM_STATUS_INIT= %d",FM_STATUS_INIT);
                        FM_SetStatus(FM_STATUS_INIT);
                        rc = FM_SUCCESS;
                        break;
                    }
                }
            }
        }
        //no useable FM
        if(i >(int32)(FM_GetOpsTabLen() -2))
        {
            //FM_PRINT:"FM_Init2: Fail to init FM!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_218_112_2_18_0_32_44_1396,(uint8*)"");

            //if initailize fail, pls close clock and power
            //FM_ConfigClk(SCI_FALSE);
            //FM_ConfigPower(SCI_FALSE);

            rc = FM_ERROR;
        }
    }
    else
    {
        //init fm chip
        //s_fm_operations->init();
        // if(s_fm_operations->init && (FM_SUCCESS == s_fm_operations->init()) )
        if (FM_SUCCESS == s_fm_operations->init() )
        {	
            FM_SetStatus(FM_STATUS_INIT);
            rc = FM_SUCCESS;
        }
        else
        {
            //FM_PRINT:"FM_Init1: Fail to init FM!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_237_112_2_18_0_32_44_1397,(uint8*)"");
            //if initailize fail, pls close clock and power
            FM_ConfigClk(SCI_FALSE);
            FM_ConfigPower(SCI_FALSE);
            rc = FM_ERROR;
        }
    }

    return rc;
}

/*****************************************************************************/
//  Description:    This function is used to Open FM playing    
//  Author:         Liangwen.Zhen
//  Note: 20101223: will.jiang: open() not used right now, use init() instead
/*****************************************************************************/
PUBLIC uint32 FM_Open(void)
{
    //FM_PRINT:"FM_Open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_253_112_2_18_0_32_44_1398,(uint8*)"");
    if(!FM_IsInit())
    {
        //FM_PRINT:"FM not init !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_256_112_2_18_0_32_44_1399,(uint8*)"");
        //return FM_ERROR;
        FM_Init();
    }	

    if(FM_STATUS_CLOSE == FM_GetStatus() || FM_STATUS_INIT== FM_GetStatus())
    {
        s_fm_operations->open();
        FM_SetStatus(FM_STATUS_OPEN);
    }

    return FM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to start FM playing    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_PlayStart(uint16 freq)
{

	//FM_PRINT:"FM_Start: freq = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_276_112_2_18_0_32_44_1400,(uint8*)"d", freq);
	
	if(!FM_IsInit())
	{
	    //FM_PRINT:"FM not init !!"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_280_112_2_18_0_32_44_1401,(uint8*)"");
	    //return FM_ERROR;
	    FM_Init();
    }	
	// 20101220: will.jiang_cr215111 : no need to check states here
	//if(FM_STATUS_STOP > FM_GetStatus() && FM_STATUS_NOT_INIT < FM_GetStatus() )
	{
	   	s_fm_operations->playstart(freq);
		// delay sometime, so that the ouput audio signal become to stable
	   	FM_SystemDelay(100);
	   	FM_SetStatus(FM_STATUS_PLAYING);
 	}	

	return FM_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to stop FM playing    
//  Author:         Liangwen.Zhen
//  Note:  20101223: will.jiang: PlayStop() only mute it        
/*****************************************************************************/
PUBLIC uint32 FM_PlayStop(void)
{
	//FM_PRINT:"FM_PlayStop"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_300_112_2_18_0_32_44_1402,(uint8*)"");
	if(!FM_IsInit())
	{
		//FM_PRINT:"FM_PlayStop: not init, Don't need to stop"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_303_112_2_18_0_32_44_1403,(uint8*)"");
		return FM_SUCCESS;
	}
	
       s_fm_operations->playstop();	
       s_fm_operations->close();    
	//FM_SetStatus(FM_STATUS_STOP);
	FM_SetStatus(FM_STATUS_CLOSE);
	return FM_SUCCESS;

}

/*****************************************************************************/
//  Description:    This function is used to close FM function    
//  Author:         Liangwen.Zhen
//  Note: 20101223: will.jiang: Close() power-off it
/*****************************************************************************/
PUBLIC uint32 FM_Close(void)
{
	//FM_PRINT:"FM_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_320_112_2_18_0_32_44_1404,(uint8*)"");


	if(!FM_IsInit())
	{
		//FM_PRINT:"FM_Close: not init, Don't need to Close"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_325_112_2_18_0_32_44_1405,(uint8*)"");
		return FM_SUCCESS;
	}
	
	s_fm_operations->close();    

	// Close clock and power
	//FM_ConfigClk(SCI_FALSE);
	//FM_ConfigPower(SCI_FALSE);

	// 20101220: will.jiang_cr215111 : close it properly, avoid to fast to re-open()
       //FM_SystemDelay(200);
	FM_SetStatus(FM_STATUS_CLOSE);
	
	return FM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to seek radio station manually
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_ManualSeek(
							uint16 						freq,
	                        FM_SCAN_DIR_E  				seek_dir,
	                     	uint8 						steps, 
	                     	FM_SINGLE_CHANNEL_INFO_T *	single_channel_info
                     	)
{
    SCI_TRACE_LOW("FM_ManualSeek freq=%d",freq);
    //FM_PRINT:"FM_ManualSeek: freq = %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_353_112_2_18_0_32_44_1406,(uint8*)"d", freq);

    if (!FM_IsInit())
    {
        //FM_PRINT:"FM not init !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_357_112_2_18_0_32_44_1407,(uint8*)"");
        //return FM_ERROR;
        //s_fm_operations->init();
        FM_Init();
    }	

    s_fm_operations->manualseek(freq,seek_dir,steps,single_channel_info);
    // FM_main();
    FM_SetStatus(FM_STATUS_PLAYING);
    return FM_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to seek radio station automatically.
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_AutoSeek(
							uint16 					freq,
		                    FM_SCAN_DIR_E  			seek_dir,    
							FM_SCAN_MODE_E 			mode,			
							FM_ALL_CHANNEL_INFO_T  *all_channel_info
						)
{
    uint32 ret = FM_SUCCESS;
	SCI_TRACE_LOW("FM_AutoSeek\n");
	 SCI_TRACE_LOW("fm_32\n");
	//FM_PRINT:"FM_AutoSeek: seek_dir = %d, mode = %d, freq = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_378_112_2_18_0_32_44_1408,(uint8*)"ddd", seek_dir, mode, freq);

	if(!FM_IsInit())
	{
	    //FM_PRINT:"FM not init !!"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_382_112_2_18_0_32_44_1409,(uint8*)"");
	    //return FM_ERROR;
	    FM_Init();
    }	
	
	ret = s_fm_operations->autoseek(freq,seek_dir,mode,all_channel_info);
	//FM_main();
	// still disable here, enable in play()
    // FM_EnableAudioOutput(SCI_TRUE);

    return ret;
}



/*****************************************************************************/
//  Description:    This function is used to Set scan level for fm.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_SetScanLevel(FM_SCANLEVEL_E level)	
{
	//FM_PRINT:"FM_SetScanLevel: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_400_112_2_18_0_32_44_1410,(uint8*)"d", level);
	if(!FM_IsInit())
	{
	    //FM_PRINT:"FM not init !!"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_403_112_2_18_0_32_44_1411,(uint8*)"");
	    return FM_ERROR;
    }	
    s_fm_operations->setscanlevel(level);
	
	return FM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is used to command from api for FM.
//  Note:           
/*****************************************************************************/
PUBLIC uint32 FM_Ioctl(uint32 cmd, void *data)
{
    if(!FM_IsInit())
    {
	    //FM_PRINT:"FM not init !!"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_403_112_2_18_0_32_44_1411,(uint8*)"");
	    return FM_ERROR;
    }
    if(s_fm_operations->ioctl)
        s_fm_operations->ioctl(cmd, data);

    return FM_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to Check if FM has init.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN FM_IsInit(void)
{
	if(FM_GetStatus() >= FM_STATUS_INIT)
	{
		return SCI_TRUE;
	}
	else
	{
		return SCI_FALSE;
	}
}
/*****************************************************************************/
//  Description:    This function is used to set FM region.    
//  Author:         yuehz
//  Note:           20060627
/*****************************************************************************/
PUBLIC uint32 FM_SetRegion(FM_REGION_E  fm_region)
{
    uint8 i;
	if(!FM_IsInit())
	{
	    //FM_PRINT:"FM not init !!"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FM_DRV_435_112_2_18_0_32_44_1412,(uint8*)"");
	    return FM_ERROR;
    }	
    s_fm_operations->setfmregion(fm_region);
     
 	for(i=0;i<sizeof(s_region_freqrange)/sizeof(FM_REGION_FREQRANGE_T);i++)
    {
        if(s_region_freqrange[i].fm_region == fm_region)
        {
           	s_fm_freq_range.min_freq  = s_region_freqrange[i].fm_freq_range.min_freq;
           	s_fm_freq_range.max_freq = s_region_freqrange[i].fm_freq_range.max_freq;
            break;
        }
    }
     

    return FM_SUCCESS; 
}


/*****************************************************************************/
//  Description:    This function is used to get current FM frequency range.    
//  Author:         yuehz
//  Note:           20060627
/*****************************************************************************/
PUBLIC uint32 FM_GetFreqRange(FM_FREQ_RANGE_T *fm_freq_range)
{
    fm_freq_range->min_freq  = s_fm_freq_range.min_freq;
    fm_freq_range->max_freq = s_fm_freq_range.max_freq;

    return FM_SUCCESS;
}

PUBLIC FM_TYPE_E FM_GetFmType(void)
{
#ifdef FM_SUPPORT_AUTO
extern PUBLIC BOOLEAN FM_IDENTIFY_BK1080(void);
    if (s_fm_type != FM_TYPE_INVALID)
    {
        return s_fm_type;
    }

    if (FM_IDENTIFY_BK1080())
    {
        s_fm_type = FM_TYPE_EXTERNAL;
        return FM_TYPE_EXTERNAL;
    }
    else
    {
        s_fm_type = FM_TYPE_BUILDIN;
        return FM_TYPE_BUILDIN;
    }
#endif

#ifdef FM_SUPPORT_SPRD_V0
    s_fm_type = FM_TYPE_BUILDIN;
    return FM_TYPE_BUILDIN;
#else
    s_fm_type = FM_TYPE_EXTERNAL;
    return FM_TYPE_EXTERNAL;
#endif
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of fm_drv.c


#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
//FM_test
//#define fm_sc6531efm_ref
#define FM_VERSION_DATE 201703080
#define FM_VERSION_DATEx  0x170308A

uint32 g_efuse_fm[64];
uint32 g_fm_if_ctrl=1;
uint32 g_fm_apll_ctrl_pre=0;
uint32 g_fm_apll_ctrl_cur=0;

extern void CHIP_FineTuneAPllClk(APLL_FTUNE_MODE_E clkft_mode);
extern uint32 LAYER1_RxADCOnOff(BOOLEAN  flag);
void FM_delay1000()
{
	uint32 i;
	for (i = 0; i < 1000; i++) ;

}

void FM_delay100()
{
	uint32 i;
	for (i = 0; i < 100; i++) ;

}

void FM_apb_module_enable_ctl()
{
    //REG32(REG_APB_EB0) |= 0xffffffff;
    REG32(REG_APB_EB) |= (BIT_APB_FM_EB | BIT_APB_UART1_EB | BIT_APB_UART0_EB );
    REG32(REG_APB_EB1) |= (BIT_APB_SYST_EB|BIT_APB_SYST_RTC_EB);

    //REG32(REG_AHB_EB0) |= (BIT_19 |BIT_14);
}

#if 0
PUBLIC void Uart_Init (uint32 port_id)
{
    unsigned long baud_rate = 0;

    baud_rate = UART_BASE_CLK / 115200;
   // debug_port = port_id;

    switch (port_id)
    {
    case 0:
        REG32 (CTL_BASE_UART0 + 0x10) = 0x00;
        REG32 (CTL_BASE_UART0 + 0x18) = 0x1C;
        REG32 (CTL_BASE_UART0 + 0x1C) = 0x00;
        REG32 (CTL_BASE_UART0 + 0x24) = (uint16) baud_rate; //115200
        REG32 (CTL_BASE_UART0 + 0x28) = (baud_rate >> 8);
        break;

    case 1:
        REG32 (CTL_BASE_UART1 + 0x10) = 0x00;
        REG32 (CTL_BASE_UART1 + 0x18) = 0x1C;
        REG32 (CTL_BASE_UART1 + 0x1C) = 0x00;
        REG32 (CTL_BASE_UART1 + 0x24) = (uint16) baud_rate; //115200
        REG32 (CTL_BASE_UART1 + 0x28) = (baud_rate >> 8);
        break;

    default:
        break;
    }
}
#endif

#if 0
uint32 SCI_TraceLow (const char *x_format, ...)
 {
      return(0);
 }
#endif

BOOLEAN trout_write_reg(uint32 reg_addr, uint32 reg_data)
{
    CHIP_REG_SET(reg_addr, reg_data);
    return(TRUE);
}

int32 trout_read_reg(uint32 reg_addr, uint32 *reg_data)
{
	//uint32 i;
	
	if (reg_data == NULL)
	{
		return -1;
	}
	
    *reg_data = CHIP_REG_GET(reg_addr);

	//for (i = 0; i < 10000; i++) ;
	FM_delay1000();

	return 0;
}

BOOLEAN trout_write_rf_reg(uint32 addr, uint32 data)
{
	uint32  reg_data=0;
	BOOLEAN ret=FALSE;
	//volatile SPI_PHY_CTL *phy_ctl = (SPI_PHY_CTL *)CTL_BASE_RF;
	reg_data = ((addr&0x7fff)<<16)|(data&0xffff);

	trout_write_reg(CTL_BASE_RF, reg_data);
	//for (i = 0; i < 10000; i++) ;
	FM_delay1000();
	//spi_write_waitforidle();
	ret = TRUE;
	
	return ret;
}

BOOLEAN trout_read_rf_reg(uint32 addr, uint32 *data)
{
	uint32 reg_data=0;
	BOOLEAN ret=FALSE;
	//trout_adi_rd_int_clr();
	//volatile SPI_PHY_CTL *phy_ctl = (SPI_PHY_CTL *)CTL_BASE_RF;
	reg_data = ((addr&0x7fff)<<16)|BIT_31;

	trout_write_reg(CTL_BASE_RF, reg_data);
	//for (i = 0; i < 10000; i++) ;
	FM_delay1000();
	//spi_write_waitforidle();
	trout_read_reg(CTL_BASE_RF, &reg_data);
	//for (i = 0; i < 10000; i++) ;
	FM_delay1000();
	//spi_read_waitforidle();
	//trout_read_reg(RF_SPI_REG_START_ADDR+(addr&0xfff), &reg_data);
	//for (i = 0; i < 100; i++) ;
	*data = reg_data & 0xffff;
	ret =  TRUE;

	return ret;
}

LOCAL BOOLEAN trout_fm_write_rf_reg(uint32 addr, uint32 reg_data)
{
	uint32 data;
	if (trout_write_rf_reg(addr, reg_data) == FALSE)
	//if (trout_write_rf_reg(addr, reg_data) == 0)
	{
		SCI_TraceLow("trout write fm rf reg error!\n");
		return FALSE;
	}
	// read it after write
	if (trout_read_rf_reg(addr, &data) == FALSE)
	{
		//SCI_TraceLow("trout read fm rf reg error!\n");
		return FALSE;
	}
	// check if the reg is wrote successfully
	if (data != reg_data)
	{
		SCI_TraceLow("rf [%04x] write:%04x, read:%04x  @@@ error @@@\n", addr, reg_data, data);
		return FALSE;
	}
	else
	{
		//SCI_TraceLow("rf [%04x] write:%04x, read:%04x\n", addr, reg_data, data);
		return TRUE;
	}
}

PUBLIC void IRQ_INT_EN (uint32 Channel_ID)
{
        * (volatile uint32 *) INT_IRQ_EN |= (1 << Channel_ID);
}
#if 0
uint32 ISR_RegHandler(
        uint32 int_num,      // Interrupt number
        TB_ISR isr_handler   // Interrupt service routine pointer.
    )
    {
        if (INT_NUM_MAX <= int_num)
        {
            return TB_INT_ERROR;
        }

        if (TB_NULL == (void *) isr_handler)
        {
            SCI_TraceLow("ISR Handler is NULL!\n");
            return TB_PTR_ERROR;
        }

        if (TB_NULL != irq_handler[int_num])
        {
            //SCI_TraceLow("IRQ Handler is not NULL!\n");
            irq_handler[int_num] = isr_handler;
            return TB_SUCCESS;
        }
        else
        {
            irq_handler[int_num] = isr_handler;
            return TB_SUCCESS;
        }
    }
#endif

void trout_fm_irq_handler(uint32 int_sts)
{
//SCI_TraceLow("sts is %x\n",REG32(FM_REG_INT_STS));
	if ((REG32(FM_REG_INT_STS) & 0x1) == 0x1)
	{
    	if ((REG32(FM_REG_RSSI_STS) >> 16) & 0x1)
        	trout_fm_info.seek_success 	= FALSE;
    	else
        	trout_fm_info.seek_success 	= TRUE;

    	trout_fm_info.int_happen 		= TRUE;

		REG32(FM_REG_INT_EN) &= 0xE;
		REG32(FM_REG_INT_CLR) |= 0x1;
	}
//SCI_TraceLow("FM_irq sts is %x\n",REG32(FM_REG_INT_STS));
	//if ((REG32(FM_REG_INT_STS) & 0x8) == 0x8)
	//{
	//	rds_int_happen = TRUE;
	//	REG32(FM_REG_INT_CLR) |= 0x8;
	//}

}

void trout_fm_poll_handler(uint32 int_sts)
{
SCI_TraceLow("poll s\r\n");
SCI_TraceLow("poll sts is %x\r\n",REG32(FM_REG_INT_STS));
	if ((REG32(FM_REG_INT_STS) & 0x1) == 0x1)
	{
    	if ((REG32(FM_REG_RSSI_STS) >> 16) & 0x1)
        	trout_fm_info.seek_success 	= FALSE;
    	else
        	trout_fm_info.seek_success 	= TRUE;

    	trout_fm_info.int_happen 		= TRUE;

		REG32(FM_REG_INT_EN) &= 0xE;
		REG32(FM_REG_INT_CLR) |= 0x1;
	}
SCI_TraceLow("sts is %x\r\n",REG32(FM_REG_INT_STS));
	//if ((REG32(FM_REG_INT_STS) & 0x8) == 0x8)
	//{
	//	rds_int_happen = TRUE;
	//	REG32(FM_REG_INT_CLR) |= 0x8;
	//}

}

void FM_reg_dump()
{
    uint32 i, j, reg_temp;
    //for(i=0x400b0000;i<=0x400b0300;i++);
    

	#if 1
	 i=0;
    	 j=0;
            while(i<=(CTL_BASE_FM + 0x160))
        {
           i=CTL_BASE_FM+j;
            trout_read_reg(i  , &reg_temp);
            SCI_TraceLow("FM addr[%04x]  read:%08x\r\n", i,  reg_temp);
            j=j+4;
        }

	#endif
	#if 1
	    i=0;
   	 j=0;
    while(i<=0x1d8)
        {
         trout_read_rf_reg(j, &reg_temp);
         SCI_TraceLow("FM RF addr[%04x]  read:%04x\r\n", i,  reg_temp);
         i=i+4;
         j++;
        }
	#endif
}

void fm_por_init()
{

        if(SC6531EFM_AB == CHIP_GetChipType())
        {
              trout_fm_write_rf_reg(RF_FM_OTHERS_CTRL1,	RF_FM_OTHERS_CTRL1_value_AB);
        }
        else
        {
             trout_fm_write_rf_reg(RF_FM_OTHERS_CTRL1,	RF_FM_OTHERS_CTRL1_value);
        }

	trout_fm_write_rf_reg(FM_ADC_CTRL0,	              FM_ADC_CTRL0_value);
	trout_fm_write_rf_reg(FM_ADC_CTRL1,	              FM_ADC_CTRL1_value);
	trout_fm_write_rf_reg(REG_RF_FM_OTHERS_CTRL0,	REG_RF_FM_OTHERS_CTRL0_value);
	trout_fm_write_rf_reg(RF_FM_OTHERS_CTRL0,	RF_FM_OTHERS_CTRL0_value);
	trout_fm_write_rf_reg(FM_RX_RF_CTRL3,	      FM_RX_RF_CTRL3_value);
	trout_fm_write_rf_reg(FM_RX_RF_CTRL0,	     FM_RX_RF_CTRL0_value);
}

int32 trout_init()
{
	uint32 i;
	//uint32 j;
	uint32 temp_data,data;
	SCI_TRACE_LOW("trout_init  B\n");
	 #if  FM_SC6531EFM_FPGA
	//fm enable
	REG32(REG_AON_APB_EB0) |= BIT_29;
	for (i = 0; i < 1000; i++) ;
	REG32(REG_AON_APB_RST0) |= BIT_23;
	for (i = 0; i < 1000; i++) ; 
	REG32(REG_AON_APB_RST0) &= ~BIT_23;
	SCI_TraceLow("trout_init  z\n");
    	REG32(FM_REG_INT_STS) &= ~BIT_1;
	
	//REG32(0x8B000160)  |= (BIT_0 | BIT_1 | BIT_6 | BIT_7); //APB_INT_SET_CLR0
	REG32(REG_APB_INT_SET_CLR0)  |= (BIT_0 | BIT_1 | BIT_6 | BIT_7); //APB_INT_SET_CLR0
	SCI_TraceLow("trout_init  z1\n");
 	REG32(INT_FIQ_DISABLE) = 0xFFFFFFFF;
 	REG32(INT_IRQ_DISABLE) = 0xFFFFFFFF;
	SCI_TraceLow("trout_init  z2\n");
 	REG32(INT_IRQ_EN) = 0x1000;//data capture int enable
	SCI_TRACE_LOW("trout_init  B2\n");	
 	IRQ_INT_EN(FM_INT_NUM-32);//int num 44, 44-32=12  match bit12
//	IRQ_INT_EN(FM_DATA_CAP_INT_NUM);
 	ISR_RegHandler(FM_INT_NUM, (TB_ISR)trout_fm_irq_handler);
//	ISR_RegHandler(FM_DATA_CAP_INT_NUM, (TB_ISR)trout_fm_data_capture_irq_handler);
      	SCI_TRACE_LOW("trout_init  B3\n");
	//00=RF; 01=ADC
    //*(volatile uint32*)(0x603003f0) |= BIT_26;//00=RF; 01=ADC
    //*(volatile uint32*)(0x603003f0) &= ~BIT_27;//00=RF; 01=ADC
   
	
    	REG32(REG_AHB_RST0) = 0x80000;  //bt sofr rst for adc write
    	SCI_TraceLow("trout_init  z3\n");
     //for(j=0;j<10000;j++) { ; }
     	FM_delay1000();
       trout_fm_write_rf_reg(0x80, 0);
	trout_fm_write_rf_reg(0x1400, 0);
	trout_fm_write_rf_reg(0x102, 0);
	trout_fm_write_rf_reg(0x1340, 0);

     	FM_delay1000();
    	REG32(REG_AHB_RST0) = 0x40000;  //fm sofr rst  for 2351 write
	// for(j=0;j<10000;j++) { ; }
	SCI_TraceLow("trout_init  z5\n");
	FM_delay1000();
#else
//fm enable
	REG32(REG_AON_APB_EB0) |= BIT_29;
	for (i = 0; i < 1000; i++);
	REG32(REG_AON_APB_RST0) |= BIT_23;
	for (i = 0; i < 1000; i++); 
	REG32(REG_AON_APB_RST0) &= ~BIT_23;


	fm_por_init();

	trout_read_rf_reg(FM_MODE, &data);  // fm state ctrl
	data &= ~0xF000;
	data |= 0x5000;
	trout_fm_write_rf_reg(FM_MODE, data);/*lint !e516*/

	//fm_apb_en_glb = REG32(REG_AON_APB_EB);
	//REG32(REG_AON_APB_EB) = 0xFFFFFFFF;

	//REG32(REG_AON_APB_RST) |= BIT_13;
	//for (i = 0; i < 1000; i++);
	//REG32(REG_AON_APB_RST) &= ~BIT_13;

	// control adc
	//REG32(0x40060258) |= BIT_0;
	//REG32(0x4006020C) |= BIT_16 | BIT_15;
	 	// control adc
       //REG32(REG_AON_APB_ADDA_TEST_CTRL2) |= BIT_25; //bb fm adc enable
       REG32(REG_APB_FM_RF1) |= BIT_10;  //adc enable
	REG32(REG_APB_ANALOG_BB_TOP3) |= BIT_21;  //64MHz enable


         //*(volatile uint32*)(0X8200146c)=0;//o:open ldo bt_pa, 1:close ldo
         //*(volatile uint32*)(0X82001164)=0x100;//enable a die 3.3v ///mux pin out

         *(volatile uint32*)(0X8b0002c8)|=1<<2;//select  fm_pdn_sel  controlled by  PWR_CTRL

	 // *(volatile uint32*)(0X8b000330)=0X6175303;//add ldo vdd for 64M, ctr'd by platform
	
	//IRQ_INT_EN(FM_INT_NUM); //for mini code
	_CHIPDRV_EnableIRQINT(FM_INT_NUM); //fro platform//*\MS_Code\chip_drv\chip_plf\sc6531efm\sc6531efm_int_cfg.h*/
	//IRQ_INT_EN(FM_DATA_CAP_INT_NUM);
	ISR_RegHandler(FM_INT_NUM, (TB_ISR)trout_fm_irq_handler);
	//ISR_RegHandler(FM_DATA_CAP_INT_NUM, (TB_ISR)trout_fm_data_capture_irq_handler);//22
	/*lint -esym(628, _CHIPDRV_EnableIRQINT) */
	//_CHIPDRV_EnableIRQINT(TB_FM_INT);/*lint !e718 !e746 */
	
   #endif
   return(0);
}

#if 0
void SCI_Sleep(uint32 ms)
{
    //Delay(ms);
    uint32 i;
	for(i=0;i<2000;i++) ;
}
#endif

int32 trout_write_reg_cfg(trout_reg_cfg_t *reg_cfg, uint32 cfg_cnt)
{
    uint32 i;

	if (NULL == reg_cfg)
	{
		return -1;
	}
	
    for (i = 0; i < cfg_cnt; i++)
    {
        trout_write_reg(reg_cfg[i].reg_addr, reg_cfg[i].reg_data);
    }
	
    return 1;
}

void agc_tbl_reg_cfg()
{
	uint32 i, agc_tbl_base_addr, value_table_val, bits_table_val, reg_data;
	uint32 tia_dcoc_val, pga_dcoc_val, boundary_val0, boundary_val1;

	agc_tbl_base_addr = FM_REG_AGC_TBL;
	// agc table size is set to 11, max is 30
	trout_read_reg(FM_REG_AGC_CTL, &reg_data);
	reg_data &= ~0x0F000000;
	reg_data |= (FM_AGC_TBL_NUM_EVB << 24);
	trout_write_reg(FM_REG_AGC_CTL, reg_data );

	// agc table clock should be enabled if you want to config it
	REG32(FM_REG_AGC_TBL_CLK) |= 0x1;
	FM_delay1000();

	// you should config agc table according its format
	for(i = 0; i < FM_AGC_TBL_NUM_EVB; i++)   ////evb 13, fpga 11
	{
		// get bits tia pga value
		value_table_val = value_table[i] & 0xFF;	//8 bits
		bits_table_val 	= bits_table[i]  & 0x7F;	//7 bits
		tia_dcoc_val 	= tia_dcoc[i] 	 & 0x3FF;	//10 bits
		pga_dcoc_val 	= pga_dcoc[i] 	 & 0x3FFF;	//14 bits
		boundary_val0 	= boundary[i] 	 & 0xFF;	//8 bits
		boundary_val1 	= boundary[i + 1]& 0xFF;	//8 bits
		//boundary_val0 = boundary[8-i]  & 0xFF;	//8 bits
		//boundary_val1 = boundary[8-i + 1] & 0xFF;	//8 bits

		// value format
		trout_write_reg(agc_tbl_base_addr + i*8, (tia_dcoc_val << 15) | (bits_table_val << 8) | value_table_val);
		trout_write_reg(agc_tbl_base_addr + i*8 + 4, (((boundary_val1 << 8) + boundary_val0) << 14) | (pga_dcoc_val));
		//REG32(agc_tbl_base_addr + i*8) = (tia_dcoc_val << 15) | (bits_table_val << 8) | value_table_val;
		//REG32(agc_tbl_base_addr + i*8 + 4) = (((boundary_val1 << 8) + boundary_val0) << 14) | (pga_dcoc_val);
	}
	// agc table clock should be disabled when config is done
	REG32(FM_REG_AGC_TBL_CLK) &= ~0x1;
	FM_delay1000();
}


BOOLEAN trout_fm_reg_cfg(void)
{
	//uint32 i;

	trout_write_reg_cfg(fm_reg_init_des, ARRAY_SIZE(fm_reg_init_des));

	//REG32(0x60500000) = 0x4040313;
 	//10us
 	//for (i = 0; i < 100000; i++);
	
	trout_write_reg(FM_REG_AGC_TBL_CLK, 0x00000001);
	//trout_write_regs(FM_REG_AGC_DB_TBL_BEGIN, FM_REG_AGC_DB_TBL_CNT, fm_reg_agc_db_tbl) EQ FALSE)
	agc_tbl_reg_cfg();
	trout_write_reg(FM_REG_AGC_TBL_CLK, 0x00000000);
	return TRUE;
}


BOOLEAN trout_fm_cfg_rf_reg()
{
	uint32 data;

#if  FM_SC6531EFM_FPGA
	// used for 2351a
	//disable WB control by BB [15:12]
    trout_fm_write_rf_reg(0x077, 0xF000);
	//disable WAGC control by BB [8] = 0
    trout_fm_write_rf_reg(0x614, 0xB036);
    trout_fm_write_rf_reg(0x6f,	 0x601);
	//enable RC tuner read back
    trout_fm_write_rf_reg(0x07D, 0x0100);
	//enable FM overload detect
    trout_fm_write_rf_reg(0x431, 0x8022);
    trout_fm_write_rf_reg(0x478, 0x009f);
    trout_fm_write_rf_reg(0x404, 0x0313);
    trout_fm_write_rf_reg(0x400, 0x0011);
    trout_fm_write_rf_reg(0x402, 0x07A6);
	//trout_fm_write_rf_reg(0x430, 0x50);
	trout_fm_write_rf_reg(0x7A,  0x8C51);
	trout_fm_write_rf_reg(0x44F, 0x4000);
	trout_fm_write_rf_reg(0x40A, 0xF);
#else
	trout_read_rf_reg(FM_MODE, &data);
	data &= ~0xF000;
	data |= 0x5000;
	trout_fm_write_rf_reg(FM_MODE, data);

	trout_read_rf_reg(FM_SX_AFC_START, &data);
	data |= 0x8000;
	trout_fm_write_rf_reg(FM_SX_AFC_START, data);
#endif

// used for 2351c
#if 0
	trout_fm_write_rf_reg(0x477, 0xFFFF);
	trout_fm_write_rf_reg(0x478, 0x1F);
	trout_fm_write_rf_reg(0x478, 0x9F);
	trout_fm_write_rf_reg(0x430, 0x50);

	trout_fm_write_rf_reg(0x404, 0x335);
	trout_fm_write_rf_reg(0x400, 0x11);

	//disable SR2351 internal adc
	trout_fm_write_rf_reg(0x7A,  0x8C51);
	//trout_fm_write_rf_reg(0x44F, 0x4000);
	//trout_fm_write_rf_reg(0x40A, 0x7);
	//trout_fm_write_rf_reg(0x06f, 0x201);
#endif

	return 0;
}

BOOLEAN trout_fm_iis_pin_cfg()
{
       uint32 reg_data=1;
	reg_data = REG32(0x8200303c);
	reg_data &= (~0x3);
	reg_data |= 0x1;
	REG32(0x8200303c) = reg_data;
	return 0;
}

void nemog_fm_IQmonitor()
{
uint32 reg1,reg2,reg3,reg4;


reg1=*(volatile uint32*)(0x8C0001A0);            //GPIO34 PIN_U0CTS_REG
reg2=*(volatile uint32*)(0x8C0001A4);           // GPIO35  PIN_U0CTS_REG
reg3=*(volatile uint32*)(0x8C00019C);           //GPIO33  PIN_U0CTS_REG
reg4=*(volatile uint32*)(0x8C000194);          //  GPIO32  PIN_U0CTS_REG

trout_fm_write_rf_reg(TEST_PIN_DEBUG, 0x1000);  //default 0x0

#if 0
*(volatile uint32*)(0x8C0001A0) = (BIT_13 | 0x1<<19 | 0x3<<4);            //GPIO34   u0cts  IN  //0x00082030 //defaut:0x82000
*(volatile uint32*)(0x8C0001A4) = (BIT_13 | 0x1<<19 | 0x3<<4);            // GPIO35   u0rts QN //defaut:0x82080
*(volatile uint32*)(0x8C00019C) = (BIT_13 | 0x1<<19 | 0x3<<4);            // GPIO33   u0rxd IP //defaut:0x82090
*(volatile uint32*)(0x8C000194) = (BIT_13 | 0x1<<19 | 0x3<<4);            // GPIO32   u0txd QP //defaut:0x82010
#endif
//*(volatile uint32*)(0x8C0001A0) = reg1;             //GPIO34 PIN_U0CTS_REG
//*(volatile uint32*)(0x8C0001A4) = reg2;            // GPIO35  PIN_U0CTS_REG
//*(volatile uint32*)(0x8C00019C) = reg3;           //GPIO33  PIN_U0CTS_REG
//*(volatile uint32*)(0x8C000194) = reg4;            //  GPIO32  PIN_U0CTS_REG
#if 1////configure IQ pin       huijie update
(*(volatile uint32 *)(0x8c000194)) = 0x82030;
(*(volatile uint32 *)(0x8c00019c)) = 0x82030;
(*(volatile uint32 *)(0x8c0001a0)) = 0x82030;
(*(volatile uint32 *)(0x8c0001a4)) = 0x82030;
(*(volatile uint32 *)(0x8a000108)) &= ~(0xf); //select gpio input mode
(*(volatile uint32 *)(0x8a000128)) &= ~(0xf); //disable gpio input function
#endif

}


void nemog_fm_IQrecover()
{

trout_fm_write_rf_reg(TEST_PIN_DEBUG, 0x0000);  //default 0x0

*(volatile uint32*)(0x8C0001A0) = 0x82000;            //GPIO34   u0cts  IN  //0x00082030 //defaut:0x82000
*(volatile uint32*)(0x8C0001A4) = 0x82080;            // GPIO35   u0rts QN //defaut:0x82080
*(volatile uint32*)(0x8C00019C) = 0x82090;            // GPIO33   u0rxd IP //defaut:0x82090
*(volatile uint32*)(0x8C000194) = 0x82010;            // GPIO32   u0txd QP //defaut:0x82010
}

int32 trout_fm_init(void)
{
    uint32 reg_data,i;

	REG32(FM_REG_FM_CTL) |= BIT_13;
	for(i=5000;i>0;i--) ;
	REG32(FM_REG_FM_CTL) &= ~BIT_13;
	
	//trout_fm_open_clk();
    //SCI_Sleep(1);
    FM_delay1000();
	// fm reg config
	trout_fm_reg_cfg();
	
	//IQ_config_c_d_factor();

	// rf reg config
	trout_fm_cfg_rf_reg();
    trout_fm_iis_pin_cfg();  //iis sel FM

	

    //trout_fm_info.freq_seek = 980*2;
	trout_fm_info.freq_seek = INIT_FREQ;
	
	// data capture channel enable
	//REG32(0x400600DC) |= BIT_31 | BIT_6;

    return TRUE;
}

void trout_test_fm_init()
{
    SCI_TraceLow("trout_test_fm_init()\r\n");
    trout_fm_init();
	//RDS_init();
}

void trout_fm_int_en()
{
    trout_fm_info.int_happen = FALSE;
    trout_write_reg(FM_REG_INT_EN, 0x01);			//1 enable fm int
}

// fm int disable
void trout_fm_int_dis()
{
    trout_fm_info.int_happen = FALSE;
    trout_write_reg(FM_REG_INT_EN, 0x0);			//1  disable fm int
}
void trout_fm_int_clr()
{
	trout_write_reg(FM_REG_INT_CLR, 0x1);
}
void trout_fm_en(void)
{
    uint32 reg_data;
    trout_read_reg(FM_REG_FM_EN, &reg_data);
    reg_data |= BIT_31;
    trout_write_reg(FM_REG_FM_EN, reg_data);		//1 enable fm
}

void trout_fm_wait_int()
{
	uint32 i;
	SCI_TraceLow("fm wait int\r\n");
    while (1)//lint !e716
    {
   // for (i = 0; i < 50000; i++) ;
   // trout_fm_poll_handler(0);
    	// polling if int happen
		if (trout_fm_info.int_happen == 1)
		{
			break;
		}
		for (i = 0; i < 1000; i++) ;
    }

	SCI_TraceLow("fm catch int\r\n");
	//trout_fm_info.int_happen = FALSE;
	//for (i = 0; i < 1000000; i++) ;

}

// fm enable
void trout_fm_dis(void)
{
    uint32 reg_data;
    trout_read_reg(FM_REG_FM_EN, &reg_data);
    reg_data &= ~BIT_31;
    trout_write_reg(FM_REG_FM_EN, reg_data);		//1 disable fm, force fm's fsm to idle state

}

void trout_fm_tune(uint32 chan, uint32 flag)
{
    uint32 val = 100;

    SCI_TRACE_LOW("FM tune chan %d, flag %x\r\n",chan, flag);

    trout_fm_dis();
    // reg freeze first
    REG32(FM_REG_FM_CTL) |= BIT_22;
    //REG32(FM_REG_ADC_INFCTRL) |= BIT_16;	//switch the I and Q signal

    trout_write_reg(FM_REG_FM_CTL_STI, FM_CTL_STI_MODE_TUNE);	//1 tune mode

#if 0
	if (flag == 0)
	{
		val = 100;
		REG32(FM_REG_CIC16_FREQ) = 0x7CCCCC;
	} else if (flag == 1)
	{
		val = 128;
		REG32(FM_REG_CIC16_FREQ) = 0x7BE76C;
	} else if (flag == 2)
	{
		val = 0x19C;
    	REG32(FM_REG_CIC16_FREQ) = 0x33333;
	} else if (flag == 3)
	{
		val = 0x180;
		REG32(FM_REG_CIC16_FREQ) = 0x41893;
	}
#endif

#if FM_SC6531EFM_FPGA
	if (flag == 0)
	{
		// 100K
		val = 0x1;
    		REG32(FM_REG_CIC16_FREQ) = 0x33333;//0x32B02;
    	// 200K
	//	val = 0x2;
	//	REG32(FM_REG_CIC16_FREQ) = 0x66666;
		// 128K
		//REG32(FM_REG_CIC16_FREQ) = 0x41893;
		// 50K
		//REG32(FM_REG_CIC16_FREQ) = 0x19999;//0x32B02;


		// Config 2351's BB filter for +100K receive
		trout_fm_write_rf_reg(0x400, 0x11);
	}
	else
	{
		// -100K
		val = 0x1FF;
		REG32(FM_REG_CIC16_FREQ) = 0x7CCCCD;
		// -128K
	//	REG32(FM_REG_CIC16_FREQ) = 0x7BE76D;
		// -200K
		//val = 0x1FE;
		//REG32(FM_REG_CIC16_FREQ) = 0x79999B;

		// Config 2351's BB filter for -100K receive
		trout_fm_write_rf_reg(0x400, 0x21);
	}
#else  //EVB platform //ref phone
	if (flag == 0)
	{
		val = 100;
		REG32(FM_REG_CIC16_FREQ) = 0x7CCCCC;
	} else if (flag == 1)
	{
		val = 128;
		REG32(FM_REG_CIC16_FREQ) = 0x7BE76C;
	} else if (flag == 2)
	{
		val = 0x19C;
    	REG32(FM_REG_CIC16_FREQ) = 0x33333;
	} else if (flag == 3)
	{
		val = 0x180;
		REG32(FM_REG_CIC16_FREQ) = 0x41893;
	}
#endif

    // set chan to frequency which you want to receive
    trout_write_reg(FM_REG_CHAN, (val << 20) | (chan & 0x1ffff));
    trout_fm_int_en();
    trout_fm_en();
    SCI_TRACE_LOW("trout_fm_wait_int  B\n");
    trout_fm_wait_int();
    SCI_TRACE_LOW("trout_fm_wait_int  B2\n");
    REG32(FM_REG_FM_CTL) &= ~BIT_22;

   // fm_audio_play();
}

void trout_sw_mute(uint32 sm_enb1,uint32 sm_rssi_snr_sel,uint32 sm_manu_enb,uint32 sm_manu_flag,uint32 ramp_lr_step)
{
	uint32 reg_data;
	uint32 rssi_lbound = 512-105;//// 512-95 marlin2
	uint32 rssi_hbound = 512-100;////512-90 marlin2
	//uint32 ramp_incr_lr_init = 0;//16;//only in manual mode for debug
	uint32 ramp_incr_lr_init = 0x0;
	uint32 snr_lbound = 8;//16
	uint32 snr_hbound = 10;//18
	uint32 ramp_decr_val0 = 1536/128;//similar as ramp_incr_lr_init
	uint32 ramp_decr_val1 = 2048/128;
	uint32 ramp_decr_val2 = 3072/128;
	uint32 ramp_decr_val3 = 4096/128;
	uint32 ramp_decr_val4 = 6144/128;
	uint32 ramp_decr_val5 = 8192/128;
	uint32 ramp_decr_val6 = 12288/128;
	uint32 ramp_decr_val7 = 14336/128;
	uint32 ramp_decr_val8 = 16384/128;
//#ifdef 0
	//////////////////////////////////////
	trout_read_reg(FM_REG_FM_EN,&reg_data);
	reg_data &= ~BIT_11;
	trout_write_reg(FM_REG_FM_EN,reg_data);

	trout_read_reg(FM_REG_UPD_CTRL,&reg_data);
	reg_data &= 0xFFFFFF0F;
	trout_write_reg(FM_REG_UPD_CTRL,reg_data);
	//////////////////////////////////////////

	
	reg_data = (rssi_hbound<<16)|rssi_lbound;
	trout_write_reg(FM_REG_SW_MUTE, reg_data);

	reg_data = (sm_rssi_snr_sel<<28)|(sm_enb1<<24)|(snr_hbound<<16)|(snr_lbound<<8)|ramp_incr_lr_init;
	trout_write_reg(FM_SOFTMUTE_CTRL0, reg_data);

	reg_data = (ramp_decr_val3<<24)|(ramp_decr_val2<<16)|(ramp_decr_val1<<8)|ramp_decr_val0;
	trout_write_reg(FM_SOFTMUTE_CTRL1, reg_data);

	reg_data = (ramp_decr_val7<<24)|(ramp_decr_val6<<16)|(ramp_decr_val5<<8)|ramp_decr_val4;
	trout_write_reg(FM_SOFTMUTE_CTRL2, reg_data);

	reg_data = (sm_manu_flag<<21)|(sm_manu_enb<<20)|(ramp_lr_step<<8)|ramp_decr_val8;
	trout_write_reg(FM_SOFTMUTE_CTRL3, reg_data);
	
	SCI_TraceLow("sm_enb1=%x,sm_rssi_snr_sel=%x,sm_manu_enb=%x,sm_manu_flag=%x, ramp_lr_step=%d\r\n",sm_enb1, sm_rssi_snr_sel, sm_manu_enb,  sm_manu_flag, ramp_lr_step);
//#endif
//trout_read_reg(FM_SOFTMUTE_CTRL0, &reg_data);
//trout_write_reg(FM_SOFTMUTE_CTRL0, (reg_data & 0xfeffffff));

}

void trout_fm_seek_up()
{
    uint32 reg_data;
    //trout_write_reg(FM_REG_CHAN, (978-1)*2);//trout_fm_info.freq_seek+2
    #if  FM_SC6531EFM_FPGA
	trout_write_reg(FM_REG_CHAN, (1 << 20) | ((trout_fm_info.freq_seek+1) & 0xffff)); //+100k IF
    #else
	//reg_data = (trout_fm_info.freq_seek + FM_SEEK_STEP) & 0x1ffff;
	trout_write_reg(FM_REG_CHAN, (128 << 20) | ((trout_fm_info.freq_seek+FM_SEEK_STEP) & 0x1ffff)); //+100k IF
    #endif
	//trout_write_reg(FM_REG_CHAN, (1 << 20) | ((trout_fm_info.freq_seek+1) & 0xffff)); //+100k IF
	trout_read_reg(FM_REG_FM_CTL, &reg_data);
    reg_data |= BIT_19;
    trout_write_reg(FM_REG_FM_CTL, reg_data);
}

// seek one channel down until seeking is successful
void trout_fm_seek_down()
{
    uint32 reg_data;
    //trout_write_reg(FM_REG_CHAN, (982-1)*2);//trout_fm_info.freq_seek-2
    #if  FM_SC6531EFM_FPGA
	trout_write_reg(FM_REG_CHAN, (1 << 20) | ((trout_fm_info.freq_seek-1) & 0xffff)); //+100k IF
    #else
	//reg_data = (trout_fm_info.freq_seek + FM_SEEK_STEP) & 0x1ffff;
	trout_write_reg(FM_REG_CHAN, (128 << 20) | ((trout_fm_info.freq_seek-FM_SEEK_STEP) & 0x1ffff)); //+100k IF
    #endif
   // trout_write_reg(FM_REG_CHAN,(1 << 20) | ((trout_fm_info.freq_seek-1) & 0xffff)); //+100k IF
    trout_read_reg(FM_REG_FM_CTL, &reg_data);
    reg_data &= ~BIT_19;
    trout_write_reg(FM_REG_FM_CTL, reg_data);
}

int fm_audio_play(void)
{
	return(0);
}

void trout_fm_seek(uint32 direction)
{
    trout_fm_dis();

	//REG32(FM_REG_FM_CTL) |= BIT_22;
    if (0 == direction )
        trout_fm_seek_up();
    else
        trout_fm_seek_down();

    trout_write_reg(FM_REG_FM_CTL_STI, FM_CTL_STI_MODE_SEEK);	//seek mode

    trout_fm_int_en();
    trout_fm_en();

    trout_fm_wait_int();

	trout_test_fm_status_reg_print();
    
    fm_audio_play();
}

void trout_fm_continue_seek(uint32 freq)
{
    uint32 serach_freq = freq;

    //SCI_TRACE_LOW("FMYWD trout_fm_continue_seek freq=%d",freq);

    //if ((freq == 87500)  || ((freq == 87600)))
    if ((freq <= 87500) || (freq > (108000+100)))
    {
        trout_fm_info.seek_success = FALSE;
    }
    else
    {
        if(fmd_ValidRecord(serach_freq))
        {
            trout_fm_info.seek_success = TRUE;
        }
        else
        {
            trout_fm_info.seek_success = FALSE;
        }
    }

    if (freq <= 87500)
    {
        serach_freq = 87500;
    }
    if (freq >= 108000)
    {
        serach_freq = 108000;
    }
    fmd_ValidStop(serach_freq);
}

void trout_fm_seek_one_op(uint32 freq, FM_SCAN_DIR_E seek_dir)
{
    uint8 loop_times = 0;
    uint32 prev_freq;
	
    if( (freq>108000)  ||  (freq<87500) )
    {
        trout_fm_info.seek_success = FALSE;  //invalid freq
    }
    prev_freq = freq;
    while(1)
    {
        if(seek_dir == FM_SCAN_UP)
        {
            freq +=100;
            if(freq == prev_freq)
            {
                trout_fm_info.seek_success = FALSE;  //invalid freq
                break;
            }
            if(freq >108000)
            {
                freq = 87500;
            }
        }
        else
        {
            freq -=100;
            if(freq == prev_freq)
            {
                trout_fm_info.seek_success = FALSE;  //invalid freq
                break;
            }
            if(freq <87500)
            {
                freq = 108000;
            }
        }
        rdafm_Start_Fm_Chn(freq);
        SCI_Sleep(50);
        if(fmd_ValidRecord(freq))
        {
            trout_fm_info.seek_success = TRUE;  //invalid freq
            trout_fm_info.freq_seek = freq;
            break;
        }

        loop_times++;
        if (loop_times >= 10)
        {
            break;//Avoid the dead loop
        }
    }	
}

	
uint32 trout_fm_continue_seek_op()
{
	uint32 cur_freq,old_freq;
    uint32 chan, chan_cnt=0;
    SCI_TraceLow("trout_test_fm_continue_seek()\r\n");

	// channel freq starts from 87.5M
	trout_fm_info.freq_seek = 875;
	trout_sw_mute(1,1,1,1,64);
	
    while (1)//lint !e716
    {
        //GET_USER_INPUT_BREAK_SIGNAL();

        //SCI_TraceLow("--------------Freq:%d--------------\r\n", chan);
        trout_fm_info.int_happen 	= FALSE;
        //trout_fm_info.freq_set		= chan;
        SCI_Sleep(5);
		old_freq = trout_fm_info.freq_seek;
      //  trout_fm_continue_seek(1, 0);
		trout_test_fm_status_reg_print();
		cur_freq = trout_fm_info.freq_seek;
		
		// range is 875-1080, seeking should stop after seek 1080
		if (old_freq > cur_freq)
			break;

        //while (trout_fm_info.int_happen EQ FALSE)
        //{
            //GET_USER_INPUT_BREAK_SIGNAL();
        //}

		// available channel is stored
        if (trout_fm_info.seek_success == TRUE)
        {
        	//Check again for mirror freq!
        	trout_fm_tune(trout_fm_info.freq_seek,1);
			trout_test_fm_status_reg_print();
			if(trout_fm_info.seek_success == TRUE)
			{
	        	fm_chan_info[chan_cnt].chan = trout_fm_info.freq_seek;
	        	fm_chan_info[chan_cnt].seek = 1;
			fm_auto_freq_tab[chan_cnt]  = (trout_fm_info.freq_seek)/100;
	        	chan_cnt++;
				SCI_TraceLow("======================================%5d pass checking!*******\r\n", trout_fm_info.freq_seek);
			}
			else
				SCI_TraceLow("======================================%5d fail checking!*******\r\n", trout_fm_info.freq_seek);
				
        }
        SCI_Sleep(10);
    }
    SCI_TraceLow("==================================================\r\n");
    for (chan=0; chan<chan_cnt; chan++)
    {
    	if (fm_chan_info[chan].seek)
    	{
    		SCI_TraceLow("%5d\r\n", fm_chan_info[chan].chan);
    	}
    }
	SCI_TraceLow("total num is %5d\r\n", chan_cnt);

	trout_sw_mute(1,1,0,0,1);
       return(chan_cnt);
}



 void trout_fm_print_sts()
{
	uint32 reg_data;
	//volatile uint32 rssi,tmp;
	
    SCI_TraceLow("FM_REG_SEEK_CNT     : %08x (%d)\r\n", 	trout_fm_info.seek_cnt, trout_fm_info.seek_cnt);	
    SCI_TraceLow("FM_REG_CHAN_FREQ_STS: %08x (%d)\r\n", 	trout_fm_info.freq_seek, trout_fm_info.freq_seek);
    SCI_TraceLow("FM_REG_FREQ_OFF_STS : %08x (%d)\r\n", 	trout_fm_info.freq_offset, trout_fm_info.freq_offset);
    SCI_TraceLow("FM_REG_RF_RSSI_STS  : %08x\r\n", 			trout_fm_info.rf_rssi);
	reg_data = 512;
	reg_data = reg_data - trout_fm_info.inpwr_sts;
	SCI_TraceLow("FM_REG_INPWR_STS    : %d\r\n", 			reg_data);
    SCI_TraceLow("FM_REG_WBRSSI_STS   : %08x\r\n", 			trout_fm_info.fm_sts & 0xFF);
    SCI_TraceLow("FM_REG_RSSI_STS     : %08x\r\n", 			trout_fm_info.rssi);
    SCI_TraceLow("FM_REG_AGC_TBL_STS  : %08x\r\n",      	trout_fm_info.agc_sts);
	SCI_TraceLow("pilot pwr30:%d,pilot pwr31:%d\r\n",		(trout_fm_info.pilot_sts) >> 16, (trout_fm_info.pilot_sts) & 0x1FFF );
    SCI_TraceLow("pilot detect:%d, stero:%d, seek fail:%d, rssi:%08x\r\n",   (trout_fm_info.rssi >> 18) & 0x3, (trout_fm_info.rssi >> 17) & 0x1, (trout_fm_info.rssi >> 16) & 0x1, trout_fm_info.rssi & 0xff);
	
}

 void trout_fm_status_reg_read()
{
	REG32(FM_REG_FM_CTL) |= BIT_22;

    trout_read_reg(FM_REG_SEEK_CNT,			&trout_fm_info.seek_cnt);/*lint !e605 -e605*/
    trout_read_reg(FM_REG_CHAN_FREQ_STS,    &trout_fm_info.freq_seek);
    trout_read_reg(FM_REG_FREQ_OFF_STS,     &trout_fm_info.freq_offset);
    trout_read_reg(FM_REG_RF_RSSI_STS, 	    &trout_fm_info.rf_rssi);
    trout_read_reg(FM_REG_RSSI_STS,	 		&trout_fm_info.rssi);	
    trout_read_reg(FM_REG_INPWR_STS, 		&trout_fm_info.inpwr_sts);
    trout_read_reg(FM_REG_MISC_STS,			&trout_fm_info.fm_sts);
    trout_read_reg(FM_REG_AGC_TBL_STS,      &trout_fm_info.agc_sts);
	trout_read_reg(FM_REG_PILOT_STS,      	&trout_fm_info.pilot_sts);
	//trout_read_reg(FM_REG_SNR_ESTI,      	&trout_fm_info.snr_esti);
	
	REG32(FM_REG_FM_CTL) &= ~BIT_22;
}

void trout_test_fm_status_reg_print()
{
	trout_fm_status_reg_read();
    trout_fm_print_sts();
}

void trout_test_data_capture(uint32 level, uint32 i_data_num)
{
       uint32 i,data, data_temp, data_num=0 ;
	volatile uint32 *capture_data_buf = (volatile uint32 *)(0x40023000); //WIFI_RAM_BASE;
      SCI_TraceLow("fm data capture begin, level: %x  num%x\r\n", level,i_data_num);
	  
      if(i_data_num>0xC000)
		data_num = 0xC000;
	else
		data_num = i_data_num;
	  
	#if 1  
REG32(0x8b0001a0) |= BIT_21;  //dsp ram enable
SCI_TraceLow("fm 0x8b0001a0 %x\r\n",REG32(0x8b0001a0) );
      #endif
	  
	REG32(0x8b0002b8) &= ( ~BIT_1); // cap rst, write only
       REG32(0x8b0002b8) |= ( BIT_1); // cap rst, write only
		
      REG32(0x5270021c) &= (~(BIT_5+BIT_4+BIT_2));
     SCI_TraceLow("fm 0x5270021c %x\r\n",REG32(0x5270021c) );
      REG32(0x50500158) |=  BIT_4;
	  SCI_TraceLow("fm 0x50500158 %x\r\n",REG32(0x50500158) );
	//REG32(0x8b0001a0) |= BIT_21;  //dsp ram enable 
	//  REG32(0x8b0002c8) |= BIT_9;  //cap enable
	//REG32(0x8b0002b8) |= BIT_1;
      	//level = 1;   //adc
	//level = 6;   //160k
	//level =0xA;  //32k
	
      REG32(0x8b00030c) |= (BIT_1+BIT_0);  //cap sel
       SCI_TraceLow("fm 0x8b00030c %x\r\n",REG32(0x8b00030c) );
      data_temp = (REG32(FM_REG_FM_CTL)) & (~0xf8000000);
	REG32(FM_REG_FM_CTL) = (data_temp | (level << 27));
	REG32(0x8b0002c8) |= BIT_9;  //cap enable
	SCI_TraceLow("fm 0x8b0002c8 %x\r\n",REG32(0x8b0002c8) );

	  
      while(1)//lint !e716
      	{
      	SCI_TraceLow("fm data capture wait\r\n");
      	SCI_Sleep(100);
      	   if(0x4== (REG32(0x8b00030c) & 0x4))
      	   	{
      	   	REG32(FM_REG_FM_CTL)  &= 0x07ffffff; //level clr 0
      	   	REG32(0x8b00030c) &= (~0x3);  //cap sel
      	   	REG32(0x8b0001a0) |= BIT_21;  //dsp ram enable
      	   	REG32(0x8b0002c8) &= (~BIT_9); //BIT_9 clr 0
             break;

		   }
	  }
	  
SCI_TraceLow("fm data capture begin, level: %x,\r\n", level,i_data_num);
SCI_TraceLow("fm QH IL\r\n");
SCI_TraceLow("%x  %x\r\n", 0xBBBB, 0xBBBB);
	  data_num = data_num>>2; //byte --> uint32
      for (i = 0; i < data_num; i++) //CAPTURE_DATA_NUM
	{
		data = capture_data_buf[i];
		SCI_TraceLow("fm %04x########%04x %04x\r\n", i,  data>>16,data&0xffff);
		SCI_Sleep(1);
	}
SCI_TraceLow("%04x  %04x\r\n", 0xEEEE, 0xEEEE);
SCI_TraceLow("fm data capture END\r\n");
}


BOOLEAN fm_DB_LoadFromNV(void)
{
    uint32 i;
    uint32 data=0;

    //SCI_TraceLow("fm_DB_LoadFromNV\r\n");

    SCI_MEMSET(&fm_config, 0, sizeof(fm_config));

    if(NVERR_NONE != EFS_NvitemRead(NV_FM_CONFIG, sizeof(fm_config), (uint8 *)&fm_config))
    {
        SCI_TRACE_LOW("FM Read NV failed\r\n");

        /* Initialize fields to default disable value */
        fm_config.fm_nv_config_en = 0x0;
        SCI_TraceLow("fm_DB_LoadFromNV  fail\r\n");
        return FALSE;
    }

    if (1 == fm_config.fm_nv_config_en)
    {
        g_fm_longant_condition1_rssi = (uint8)fm_config.searchcondition.longantcondition[0];
        g_fm_longant_condition1_offset  = (uint8)(fm_config.searchcondition.longantcondition[0]>>8);
        g_fm_longant_condition1_noise  = (uint8)(fm_config.searchcondition.longantcondition[0]>>16);

        g_fm_longant_condition2_rssi = (uint8)fm_config.searchcondition.longantcondition[1];
        g_fm_longant_condition2_offset  = (uint8)(fm_config.searchcondition.longantcondition[1]>>8);
        g_fm_longant_condition2_noise  = (uint8)(fm_config.searchcondition.longantcondition[1]>>16);

        g_fm_longant_condition3_rssi = (uint8)fm_config.searchcondition.longantcondition[2];
        g_fm_longant_condition3_offset  = (uint8)(fm_config.searchcondition.longantcondition[2]>>8);
        g_fm_longant_condition3_noise  = (uint8)(fm_config.searchcondition.longantcondition[2]>>16);

        g_fm_longant_condition4_rssi = (uint8)fm_config.searchcondition.longantcondition[3];
        g_fm_longant_condition4_offset  = (uint8)(fm_config.searchcondition.longantcondition[3]>>8);
        g_fm_longant_condition4_noise  = (uint8)(fm_config.searchcondition.longantcondition[3]>>16);
        SCI_TRACE_LOW("FM NV long value 1 is %d %d %d, 2 is %d %d %d, 3 is %d %d %d, 4 is %d %d %d",\
			g_fm_longant_condition1_noise,g_fm_longant_condition1_offset,g_fm_longant_condition1_rssi,\
			g_fm_longant_condition2_noise,g_fm_longant_condition2_offset,g_fm_longant_condition2_rssi,\
			g_fm_longant_condition3_noise,g_fm_longant_condition3_offset,g_fm_longant_condition3_rssi,\
			g_fm_longant_condition4_noise,g_fm_longant_condition4_offset,g_fm_longant_condition4_rssi);

#ifdef FM_S_ANT_SUPPORT
        g_fm_shortant_condition1_rssi = (uint8)fm_config.searchcondition.shortantcondition[0];
        g_fm_shortant_condition1_offset  = (uint8)(fm_config.searchcondition.shortantcondition[0]>>8);
        g_fm_shortant_condition1_noise  = (uint8)(fm_config.searchcondition.shortantcondition[0]>>16);

        g_fm_shortant_condition2_rssi = (uint8)fm_config.searchcondition.shortantcondition[1];
        g_fm_shortant_condition2_offset  = (uint8)(fm_config.searchcondition.shortantcondition[1]>>8);
        g_fm_shortant_condition2_noise  = (uint8)(fm_config.searchcondition.shortantcondition[1]>>16);

        g_fm_shortant_condition3_rssi = (uint8)fm_config.searchcondition.shortantcondition[2];
        g_fm_shortant_condition3_offset  = (uint8)(fm_config.searchcondition.shortantcondition[2]>>8);
        g_fm_shortant_condition3_noise  = (uint8)(fm_config.searchcondition.shortantcondition[2]>>16);

        SCI_TRACE_LOW("FM NV short value 1 is %d %d %d, 2 is %d %d %d, 3 is %d %d %d",\
			g_fm_shortant_condition1_noise,g_fm_shortant_condition1_offset,g_fm_shortant_condition1_rssi,\
			g_fm_shortant_condition2_noise,g_fm_shortant_condition2_offset,g_fm_shortant_condition2_rssi,\
			g_fm_shortant_condition3_noise,g_fm_shortant_condition3_offset,g_fm_shortant_condition3_rssi);
#endif

/*
        SCI_TraceLow("stra_sel:%x\r\n", fm_config.fm_seek_para.stra_sel);
        SCI_TraceLow("seek_ch_th:%x\r\n", fm_config.fm_seek_para.seek_ch_th);
        SCI_TraceLow("mono_pwr_th1:%x\r\n", fm_config.fm_seek_para.mono_pwr_th1);
        SCI_TraceLow("mono_pwr_th2:%x\r\n", fm_config.fm_seek_para.mono_pwr_th2);
        SCI_TraceLow("mono_pwr_th3:%x\r\n", fm_config.fm_seek_para.mono_pwr_th3);
        SCI_TraceLow("seek_vldch_fo_th1:%x\r\n", fm_config.fm_seek_para.seek_vldch_fo_th1);
        SCI_TraceLow("seek_vldch_fo_th2:%x\r\n", fm_config.fm_seek_para.seek_vldch_fo_th2);
        SCI_TraceLow("noise_pwr_th0:%x\r\n", fm_config.fm_seek_para.noise_pwr_th0);
        SCI_TraceLow("noise_pwr_th1:%x\r\n", fm_config.fm_seek_para.noise_pwr_th1);
        SCI_TraceLow("noise_pwr_th2:%x\r\n", fm_config.fm_seek_para.noise_pwr_th2);
        SCI_TraceLow("pdp_th:%x\r\n", fm_config.fm_seek_para.pdp_th);
        SCI_TraceLow("pdp_dev1:%x\r\n", fm_config.fm_seek_para.pdp_dev1);
        SCI_TraceLow("pdp_dev2:%x\r\n", fm_config.fm_seek_para.pdp_dev2);
        SCI_TraceLow("seek_loop:%x\r\n", fm_config.fm_bandlmt_para.seek_loop);
        SCI_TraceLow("band_lmt_L:%x\r\n", fm_config.fm_bandlmt_para.band_lmt_L);
        SCI_TraceLow("band_lmt_H:%x\r\n", fm_config.fm_bandlmt_para.band_lmt_H);
        SCI_TraceLow("band_space:%x\r\n", fm_config.fm_bandlmt_para.band_space);
        SCI_TraceLow("ster_pwr:%x\r\n", fm_config.ster_pwr);
        for(i=0;i<5;i++)
        {
            SCI_TraceLow("addr:%x, data:%x\r\n", fm_config.fm_reserved.addr[i],fm_config.fm_reserved.data[i]);
        }
        REG32(FM_REG_SEEK_LOOP) = fm_config.fm_bandlmt_para.seek_loop;
        REG32(FM_REG_BAND_LMT) = fm_config.fm_bandlmt_para.band_lmt_L;
        REG32(FM_REG_BAND_SPACE) = (((uint32)fm_config.fm_bandlmt_para.band_space) << 24)|fm_config.fm_bandlmt_para.band_lmt_H;
        REG32(FM_REG_PDP_TH) = fm_config.fm_seek_para.pdp_th;
        REG32(FM_REG_PDP_DEV) = (((uint32)fm_config.fm_seek_para.pdp_dev2)<<16)|fm_config.fm_seek_para.pdp_dev1;
        REG32(FM_REG_SEEK_CH_TH) = fm_config.fm_seek_para.seek_ch_th;
        REG32(FM_REG_MONO_PWR) = (((uint32)fm_config.fm_seek_para.mono_pwr_th2)<<16)|fm_config.fm_seek_para.mono_pwr_th1;

        trout_read_reg(FM_REG_SEEK_LOOP, 		&data);
        SCI_TraceLow("FM_REG_SEEK_LOOP    : %d\r\n", 	data);
        trout_read_reg(FM_REG_BAND_LMT, 		&data);
        SCI_TraceLow("FM_REG_BAND_LMT    : %d\r\n", 	data);
        trout_read_reg(FM_REG_BAND_SPACE, 		&data);
        SCI_TraceLow("FM_REG_BAND_SPACE    : %d\r\n", 	data);
        trout_read_reg(FM_REG_PDP_DEV, 		&data);
        SCI_TraceLow("FM_REG_PDP_DEV    : %d\r\n", 	data);
        trout_read_reg(FM_REG_SEEK_CH_TH, 		&data);
        SCI_TraceLow("FM_REG_SEEK_CH_TH    : %d\r\n", 	data);
*/
    }

    return TRUE;
}

void fm_time_HourMinuSec()
{
	uint32 tick_num;
	uint32 current_time;
	uint32 year;
	uint32 month;
	uint32 day;
	uint32 hour;
	uint32 minute;
	uint32 second;

	tick_num  = SCI_GetTickCount();
	SCI_TraceLow("FM_tick_num %d\r\n",tick_num);
	current_time = SCI_GetCurrentTime(); //uint:second
	second = current_time%60;
	minute = current_time/(60) %60;
	hour = current_time/(60*60) % 24;
	//day = current_time/(60*60*24);
	tick_num  = SCI_GetTickCount();
	SCI_TraceLow("FM_tick_num %d\r\n",tick_num);
	SCI_TraceLow("FM_Hour:Minu:Sec %d:%d:%d\r\n",hour,minute,second);
	
}


int32 trout_test_main()
{
	//trout_test_fm_reg_check();
	//trout_test_fm_init();
	//trout_test_fm_tune();
	//trout_test_fm_seek_up();
	//trout_test_fm_seek_down();
#if 0
    while (1)
    {
        main_test_func(fm_test_menu_handler, ARRAY_SIZE(fm_test_menu_handler));
    }
#endif
	trout_test_fm_init();
      SCI_TRACE_LOW("FM tune  B\n");

     while(1)
     	{
	trout_fm_tune(930, 0);
	trout_test_fm_status_reg_print();
	trout_fm_tune(970, 0);
	trout_test_fm_status_reg_print();
     	}
		
	//while(1);

	//return 0;  //unreacheable
}

int32 FM_test_entry(void)
{
SCI_TRACE_LOW("FM_test_entry r\n");
	trout_init();
	//while(1);
    trout_test_main();

	return 0;
}

void FM_main(void)
{
	//while(1);
	//uint i,j;
	uint data_temp;
	SCI_TRACE_LOW("FM_main\n");
	//REG32(REG_AHB_RST0) = 0x0;  //fm sofr rst 
	REG32(REG_AHB_RST0)  &= (~BIT_18);  //fm sofr rst 
	// for(j=0;j<10000;j++) { ; }
	 FM_delay1000();
       //apb_module_enable_ctl();
        REG32(REG_APB_EB) |= (BIT_APB_FM_EB | BIT_APB_UART1_EB | BIT_APB_UART0_EB );
        REG32(REG_APB_EB1) |= (BIT_APB_SYST_EB|BIT_APB_SYST_RTC_EB);
	data_temp =	REG32(REG_APB_EB) ;
    SCI_TraceLow("REG_APB_EB [%04x]  \n",data_temp );
	//for(j=0;j<10000;j++) { ; }
        FM_delay1000();

      //  Uart_Init(0);

	SCI_TraceLow("Begin to test \r\n");
			
	//while(1);
	//while(1);
	//test_entry();

	//audio clear buf
	#if 0
       for(i = 0;i <11; i++)
     {
	REG32(0x8200304C +i*4) = 0;
	}

	for(i = 0; i <43; i++)
	{
	REG32(0x82003100 + i*4) = 0;
	}

	REG32(0x82003040) = 0x5;
	
	REG32(0x82003018) = 0x600;
	for(i=0; i< 160; i++)
	{
	REG32(0x82003000) = 0;
	REG32(0x82003004) = 0;
	}

	REG32(0x82003018) = 0x400;
	for(i=0; i< 160; i++)
	{
	REG32(0x82003000) = 0;
	REG32(0x82003004) = 0;
	}

	REG32(0x82003018) = 0x8000;

	for(i=0; i< 160; i++)
	{
	REG32(0x82003000) = 0;
	REG32(0x82003004) = 0;
	}
	#endif

	//test_wdg();
	//mem_test();
	//timer_test();
	//test_dma();
	//test_intc();
	//test_busmonitor();
    FM_test_entry();
    SCI_TraceLow("Test Done\r\n");
    //while(1);
}

uint32 FM_OPERATION_init(void)
{
    static BOOLEAN s_fm_enter_flag = FALSE;

    //SCI_TRACE_LOW("fm_5\n");
    //bringup_tmp
    #if 0
    while(!(hwp_pwrctrl->BTFM_PWR_STAT & CP_PWRCTRL_BTFM_STABLE));
    if (hwp_pwrctrl->BTFM_PWR_STAT != 0x3)   //open bt power
    {
        hwp_pwrctrl->BTFM_PWR_CTRL = CP_PWRCTRL_BTFM_PON;
    }
    while(hwp_pwrctrl->BTFM_PWR_STAT != 0x3);
    #endif
    SCI_Sleep(100);//100ms
    *(volatile uint32*)(0x50081018) = 1;
#if 0
    hwp_sysreg->RF_ANA_26M_CTRL_SET = 0x100;    //wcn 26M clock enable
    // *((volatile unsigned long *)0x500304c8) = 0x1;  //ANA_ctrl_reg32=1
    aud_SetBtClk();
#endif
    PMIC_26M_open(PMIC_26M_USED_BY_WCN);

    LDO_SetVoltValue(35, 1500);
    LDO_TurnOnLDO(35);
    SCI_Sleep(100);//100ms
    //SCI_DEBUG("fm_6\n");
    fmd_Open();
    //SCI_DEBUG("fm_8\n");
    FmSearchPtr->search_direct = FM_SCAN_UP;
    fmd_Sysctrl();

    if (!s_fm_enter_flag)
    {
        s_fm_enter_flag = TRUE;

        fm_DB_LoadFromNV();
    }

    return(FM_SUCCESS);
}

void FM_OPERATION_open(void)
{
    SCI_TraceLow("FM_OPERATION_open \r\n");
    //SCI_TraceLow("FM_VERSION_DATE %d\r\n ", FM_VERSION_DATE);
    //SCI_TraceLow("FM_VERSION_DATEx %x\r\n ", FM_VERSION_DATEx);
}

void FM_OPERATION_close(void)
{
    fmd_Close();

#if 0
    aud_ClrBtClk();//ANA_ctrl_reg32=0; pmic 26M clock disable
    hwp_sysreg->RF_ANA_26M_CTRL_SET = 0x0;//wcn 26M clock disable
#endif

    PMIC_26M_close(PMIC_26M_USED_BY_WCN);

    //WCN RF Power Off
    LDO_TurnOffLDO(35);
    //bringup_tmp
    #if 0
    //WCN Power Off
    hwp_pwrctrl->BTFM_PWR_CTRL = CP_PWRCTRL_BTFM_POFF;
    #endif
    SCI_Sleep(20);
}

uint32 FM_OPERATION_playstart(uint16 freq)             //play FM
{
    FM_SetRegion(FM_REGION_US);
    rdafm_Start_Fm_Chn(freq);
    return(0);
}

uint32 FM_OPERATION_playstop(void)            //stop FM
{
   SCI_TraceLow("FM_OPERATION_playstop \r\n");

   hal_Rf_FM_Tune(FALSE);

   //trout_sw_mute(1,1,1,1,1);  //close volume 512ms ,soft mute disable
   //SCI_Sleep(520);
   return(0);
}

uint32 FM_OPERATION_manualseek( uint16 freq,   FM_SCAN_DIR_E seek_dir, uint8 steps, FM_SINGLE_CHANNEL_INFO_T *single_channel_info  )   //manual seek
{
    SCI_TRACE_LOW("fm_16\n");
    SCI_TRACE_LOW("freq=%d \n",freq);
    
    rdafm_Start_Fm_Chn(freq*10);
    if(PNULL != single_channel_info)
    {
        single_channel_info->freq = freq;
    }
    // ARMVB_FM_Play(48000,(void *)NULL);
    /*
    while(1)
    {
    SCI_Sleep(520);
    SCI_TRACE_LOW("fm_10\n");
    }
    */

    trout_fm_info.freq_seek = freq;

    return(0);
}

//uint32 FM_PrvTimecount=0;
uint32 FM_OPERATION_autoseek(uint16 freq, FM_SCAN_DIR_E seek_dir, FM_SCAN_MODE_E mode,  FM_ALL_CHANNEL_INFO_T  *all_channel_info  )               //auto seek
{
    uint32 count=0;
    uint32 freq_x1000=0; //87.5*1000
    //uint32 timecount, timecount1;
	
    freq_x1000=freq*10;
    //SCI_TraceLow("FM_OPERATION_autoseek \r\n");
    SCI_TraceLow("FM_OPERATION_autoseek freq =%d \r\n", freq);
    /*
    if(FM_PrvTimecount != 0)
    {
        timecount = OS_GetCurrentMilSec();
        timecount -=FM_PrvTimecount;
        SCI_TraceLow("FM_OPERATION_autoseek  FM_PrvTimecount =%d \r\n",  timecount);
    }
    
    timecount = OS_GetCurrentMilSec();
    FM_PrvTimecount= timecount;
    */

    SCI_Sleep(40);
    trout_fm_info.freq_seek = freq_x1000;
    if(FM_SCAN_ALL == mode)
    {
        //SCI_TraceLow("FM_OPERATION_autoseek  trout_fm_info.freq_seek =%d \r\n",  trout_fm_info.freq_seek);

        trout_fm_continue_seek( trout_fm_info.freq_seek);
        if(trout_fm_info.seek_success == TRUE)
        {
            SCI_TraceLow("fm_station 1st\r\n");
            fm_auto_seek_freq = (trout_fm_info.freq_seek-100)/10;
            all_channel_info->auto_freq_tab_ptr = (uint16*)(&fm_auto_seek_freq);
        }
        else
        {
            SCI_TraceLow("fm_nostation 1st\r\n");
            fm_auto_seek_freq = 0;
            all_channel_info->auto_freq_tab_ptr = (uint16*)(&fm_auto_seek_freq);
        }
        all_channel_info->freq_count =1;    
    }
    else
    {
        trout_fm_seek_one_op( trout_fm_info.freq_seek,seek_dir);
        if(trout_fm_info.seek_success == TRUE)
        {
            SCI_TraceLow("fm_scanone_station 1st\r\n");
            fm_auto_seek_freq = (trout_fm_info.freq_seek)/10;
            all_channel_info->auto_freq_tab_ptr = (uint16*)(&fm_auto_seek_freq);
        }
        else
        {
            SCI_TraceLow("fm_scanone_nostation 1st\r\n");
            fm_auto_seek_freq = 0;
            all_channel_info->auto_freq_tab_ptr = (uint16*)(&fm_auto_seek_freq);
        }
        all_channel_info->freq_count =1;    
    }

    /*
    timecount1 =	OS_GetCurrentMilSec();	
    timecount =	timecount1 - timecount;	
    SCI_TraceLow("FM_OPERATION_autoseek  timecount =%d \r\n",  timecount);
    */
    return(FM_SUCCESS);	
}
  
uint32 FM_OPERATION_setscanlevel(FM_SCANLEVEL_E level)     //set fm scan level
{
    SCI_TraceLow("FM_OPERATION_setscanlevel \r\n");
    return(0);
}
uint32 FM_OPERATION_setvol(FM_VOL_LEVEL_E vol_level)                 //set fm play volume
{
    SCI_TraceLow("FM_OPERATION_setvol \r\n");
    return(0);
}

uint32 FM_OPERATION_setfmregion(FM_REGION_E  region)    //set fm region
{
    SCI_TraceLow("FM_OPERATION_setfmregion \r\n");
    return(0);
}
uint32 FM_OPERATION_ioctl(uint32 cmd, void *data)
{
    SCI_TraceLow("FM_OPERATION_ioctl \r\n");
    return(0);
}
#endif

////eng mode inerface   
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
uint32 FM_OPERATION_write_reg(uint32 reg_addr, uint32 reg_data)
{
	if(0x8d000000==(reg_addr&0xffff0000))  //rf reg write
	{
		return(trout_fm_write_rf_reg((reg_addr>>2)&0x3fff, reg_data));
	}
	else
	{
	 	trout_write_reg( reg_addr, reg_data); //bb reg write
	 	return(TRUE);
	}
}

uint32 FM_OPERATION_read_reg(uint32 reg_addr, uint32 *reg_data)
{
    uint32 data=0;
    
    if(0x0 == reg_addr)
    {
        *reg_data = 	 (FM_VERSION_DATEx);
        return(TRUE);
        }
    
    if(0x1== reg_addr)
    {
        *reg_data = 	 (FM_VERSION_DATEx);
        return(TRUE);
    }
    
    if (0x8d000000==(reg_addr&0xffff0000))   // fm rf reg read
    {
        if (TRUE == trout_read_rf_reg((reg_addr>>2)&0x3fff, &data))
        {
            *reg_data = (data&0xffff);
            return(TRUE);
        }
    }
    else if(0==trout_read_reg(reg_addr, &data))   // fm bb reg read
    {
        *reg_data = data;
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

uint32 FM_OPERATION_get_seek_cnt_info()
{
    return(trout_fm_info.seek_cnt);
}

uint32 FM_OPERATION_get_freq_seek_info()
{
    //return((trout_fm_info.freq_seek)/100);//keep same format with freq input from ap
    return((trout_fm_info.freq_seek)/10);//if 8910
}

uint32 FM_OPERATION_get_freq_offset_info()
{
    return((trout_fm_info.freq_offset)/100);
}

uint32 FM_OPERATION_get_rf_rssi_info()
{
    //return(trout_fm_info.rf_rssi);
    #if 0
    uint32 data=0;
    trout_read_reg(FM_REG_INPWR_STS, 		&data);
    SCI_TraceLow("FM_REG_INPWR_STS    : %d\r\n", 	(512-(0x1ff&data)));
    return(512-(0x1FF&data));
    #endif
    return 0;
}

uint32 FM_OPERATION_get_inpwr_info()
{
    return(512-trout_fm_info.inpwr_sts);
}

uint32 FM_OPERATION_get_inpwr_info_rt()
{
    uint32 data=0;
    data = ((hwp_fm_dsp->status0) & FM_DSP_RSSI_DB2_MASK)>>FM_DSP_RSSI_DB2_SHIFT;
    return (data);
}

uint32 FM_OPERATION_get_fm_signal(void)
{
    uint32 data=0;
    data = ((hwp_fm_dsp->status0) & FM_DSP_SIGNAL_DB2_MASK)>>FM_DSP_SIGNAL_DB2_SHIFT;
    return (data);
}

uint32 FM_OPERATION_get_fm_noise(void)
{
    uint32 data=0;
    data = ((hwp_fm_dsp->status1) & FM_DSP_NOISE_DB2_MASK)>>FM_DSP_NOISE_DB2_SHIFT;
    return (data);
}

uint32 FM_OPERATION_get_fm_pilot(void)
{
    uint32 data=0;
    data = ((hwp_fm_dsp->status1) & FM_DSP_PILOT_DB2_MASK)>>FM_DSP_PILOT_DB2_SHIFT;
    return (data);
}

uint32 FM_OPERATION_get_fm_status0(void)
{
    uint32 tempreg = hwp_fm_dsp->status0;
    return tempreg;
}

uint32 FM_OPERATION_get_fm_status1(void)
{
    uint32 tempreg = hwp_fm_dsp->status1;
    return tempreg;
}

uint32 FM_OPERATION_get_rssi_info()
{
    return(trout_fm_info.rssi);
}

uint32 FM_OPERATION_get_agc_sts_info()
{
    uint32 data;
    data = ((hwp_fm_dsp->status3) & FM_DSP_AGC_INDEX_MASK)>>FM_DSP_AGC_INDEX_SHIFT;
    return(data);
}

uint32 FM_OPERATION_get_pwr30_info()
{
    return((trout_fm_info.pilot_sts)>>16);
}

uint32 FM_OPERATION_get_pwr31_info()
{
 	return((trout_fm_info.pilot_sts)&0x1fff);
}

uint32 FM_OPERATION_get_pilot_info()
{
    uint32 data=0;
    data = (((trout_fm_info.rssi <<6)&0x03000000) |((trout_fm_info.rssi >> 1) & 0x10000) | ((trout_fm_info.rssi >> 8) & 0x100));
    return (data);
}

uint32 FM_OPERATION_short_ant_set()
{
    #if 0
    uint32 data=0;
    GPIO_SetValue(21, 1);//GPIO21 gsm ant LNA enable
    trout_read_rf_reg(DEBUG_FM_CTRL_DEBUG2, &data);
    data |= BIT_15;
    trout_fm_write_rf_reg(DEBUG_FM_CTRL_DEBUG2, data);
    
    trout_read_rf_reg(REG_FM_CTRL_DEBUG2, &data);
    data |= BIT_15;
    trout_fm_write_rf_reg(REG_FM_CTRL_DEBUG2, data);
    
    trout_fm_write_rf_reg(FM_RX_RF_CTRL0, 0x0000);///default 0x8f00

    SCI_TraceLow("FM_OPERATION_short_ant_set\r\n");
    #endif

    return 0;
}

uint32 FM_OPERATION_long_ant_set()
{
#if 0
    uint32 data=0;
    GPIO_SetValue(21, 0);//GPIO21 //GPIO21 gsm ant LNA disable
    trout_read_rf_reg(DEBUG_FM_CTRL_DEBUG2, &data);
    data &= (~BIT_15);
    trout_fm_write_rf_reg(DEBUG_FM_CTRL_DEBUG2, data);
    
    trout_read_rf_reg(REG_FM_CTRL_DEBUG2, &data);
    data &= (~BIT_15);
    trout_fm_write_rf_reg(REG_FM_CTRL_DEBUG2, data);
    
    trout_fm_write_rf_reg(FM_RX_RF_CTRL0, 0x0000);////default 0x8f00

    SCI_TraceLow("FM_OPERATION_long_ant_set\r\n");
#endif

    return 0;
}

uint32 FM_OPERATION_IQmonitor()
{
   //SCI_TraceLow("FM_OPERATION_IQmonitor\r\n");
    //nemog_fm_IQmonitor();
    return 0;
}

uint32 FM_OPERATION_IQrecover()
{
    //SCI_TraceLow("FM_OPERATION_IQrecover\r\n");
    //nemog_fm_IQrecover();
    return 0;
}

uint32 FM_OPERATION_soft_mute()
{
#if 0
    SCI_TraceLow("FM_OPERATION_soft_mute\r\n");
    trout_sw_mute(1,1,1,1,1);  //close volume
    SCI_Sleep(520);
#endif
    return 0;
}

uint32 FM_OPERATION_soft_unmute()
{
#if 0
    SCI_TraceLow("FM_OPERATION_soft_unmute\r\n");
    trout_sw_mute(1,1,1,0,1);  //open volume 512ms ,soft mute disable
    SCI_Sleep(520);
#endif
    return 0;
}

uint32 FM_OPERATION_data_capture(uint32 level, uint32 i_data_num)
{
     #if 0
	if(0x10000==level)
	{
		nemog_fm_IQmonitor();
		return(TRUE);
	}

	else if(0x20000==level)
	{
		nemog_fm_IQrecover();
		return(TRUE);
	}
	else if(0x30000==level)
	{
		FM_OPERATION_short_ant_set();
		return(TRUE);
	}
	else if(0x40000==level)
	{
		FM_OPERATION_long_ant_set();
		return(TRUE);
	}
	else if(0x50000==level)
	{
		LAYER1_RxADCOnOff(1);
		return(TRUE);
	}
	else if(0x60000==level)
	{
		LAYER1_RxADCOnOff(0);
		return(TRUE);
	}
	else
	{
		trout_test_data_capture(level, i_data_num);
		return(TRUE);
	}
	#endif
       return 0;
}


#else

uint32 FM_OPERATION_write_reg(uint32 reg_addr, uint32 reg_data)   ///写寄存器reg_addr：寄存器地址，reg_data: 写入寄存器的值
{
    return 0;
}
 
uint32 FM_OPERATION_read_reg(uint32 reg_addr, uint32 *reg_data)  ///读寄存器
{
    *reg_data = 1234;
    return 0;
}
uint32 FM_OPERATION_data_capture(uint32 level, uint32 i_data_num)
{
    return 0;
}
uint32 FM_OPERATION_get_seek_cnt_info()     //显示seek_cnt信息
{
    return 1;
} 
uint32 FM_OPERATION_get_freq_seek_info()    //freq_seek 显示当前频率信息
{
    return 2;
} 
uint32 FM_OPERATION_get_freq_offset_info()  //freq_offset 频偏信息
{
    return 3;
} 
uint32 FM_OPERATION_get_rf_rssi_info()  // rf rssi
{
    return 4;
} 
uint32 FM_OPERATION_get_inpwr_info()   //inpwr 当前信号强度
{
    return 5;
} 

uint32 FM_OPERATION_get_inpwr_info_rt()   //inpwr 当前信号强度
{
    return 55;
} 

uint32 FM_OPERATION_get_fm_sts_info()  // fm sts
{
    return 6;
} 
uint32 FM_OPERATION_get_rssi_info()  //rssi_info
{
    return 7;
} 
uint32 FM_OPERATION_get_agc_sts_info()  //当前agc 值
{
    return 8;
}      
uint32 FM_OPERATION_get_pwr30_info()
{
    return 9;
} 
uint32 FM_OPERATION_get_pwr31_info()
{
    return 10;
} 
uint32 FM_OPERATION_get_pilot_info()  
{
    return 11;
}

#endif

#if 0
#ifdef _DB_IN_NV
BOOLEAN _DB_LoadFromNV(void)
{
    uint32 i;

    SDI_TRACEID(BT, _DB_Init__IN, "[DB_Init] IN");

    SCI_MEMSET(&g_db, 0, sizeof(g_db));

    if(NVERR_NONE != EFS_NvitemRead(NV_BT_DB, sizeof(g_db), (uint8 *)&g_db))
    {
        SDI_TRACEID(BT, _DB_Init__Read_NV_failed, "[DB_Init] Read NV failed");

        /* Initialize fields to default value */
        g_db.status = FALSE;

        if ((*GetHostNvCtrl()) & HOST_NV_CTRL_DEFAULT_VISIABLE)
        {
            g_db.visibility = BT_PAGE_AND_INQUIRY_ENABLE;
        }
        else
        {
            g_db.visibility = default_visible;
        }

        PF_WStrCpy(g_db.local_name, g_BTConfig.local_name);
        g_db.dev_count = 0;
        return FALSE;
    }

    for(i = 0; i < ARRAY_SIZE(g_db.dev_list); ++i)
    {
        if(g_db.dev_list[i].record_valid)
        {
            /* No service is connected yet */
            g_db.dev_list[i].connectedServices11_00_31 = 0;
            g_db.dev_list[i].connectedServices11_32_63 = 0;
            g_db.dev_list[i].connectedServices12_00_31 = 0;
            g_db.dev_list[i].connectedServices13_00_31 = 0;
        }
    }

    return TRUE;
}

BOOLEAN _DB_SaveToNV(void)
{
    SDI_TRACEID(BT, __DB_SaveToNV__IN, "[_DB_SaveToNV] IN");

    if(NVERR_NONE != EFS_NvitemWrite(NV_BT_DB, sizeof(g_db), (uint8 *)&g_db, 1))
    {
        SDI_TRACEID(BT, __DB_SaveToNV__Write_NV_failed, "[_DB_SaveToNV] Write NV failed");
        return FALSE;
    }

    return TRUE;
}

BOOLEAN BT_UpdateAddrToNV(void)
{
    BT_NV_PARAM  bt_nv_param;

    SDI_TRACEID(BT, _DB_SaveAddrToNV__IN, "[DB_SaveAddrToNV] IN");

    //SCI_ASSERT(NVERR_NONE == EFS_NvitemRead(NV_BT_CONFIG, sizeof(bt_nv_param), (uint8*)&bt_nv_param));

    bt_nv_param.xtal_dac = g_BTConfig.xtal_dac;
    SCI_MEMCPY((uint8 *)&bt_nv_param.bd_addr, (uint8 *)&g_BTConfig.bd_addr, sizeof(BT_ADDRESS));

    if((BTI_BtChipId() == BT_CHIP_ID_RDA)
       ||(BTI_BtChipId() == BT_CHIP_ID_BEKEN))
    {
#if defined(BT_CHIP_BEKEN) || defined(BT_CHIP_RDA)    //add for 6531 project
        bt_nv_param.host_reserved[0] = g_BTConfig.host_nv_ctrl[0];
        bt_nv_param.host_reserved[1] = g_BTConfig.host_nv_ctrl[1];
#endif
    }

    if(NVERR_NONE != EFS_NvitemWrite(NV_BT_CONFIG, sizeof(bt_nv_param), (uint8 *)&bt_nv_param, 1))
    {
        SDI_TRACEID(BT, _DB_SaveAddrToNV__Write_NV_failed, "[DB_SaveAddrToNV] Write NV failed");
        return FALSE;
    }

    return TRUE;
}

#else

BOOLEAN _DB_LoadFromFile(void)
{
    PF_FILE  file;
    BOOLEAN  ret;
    uint16   count; /* It should be 2 bytes long */
    uint32   signature;
    int      i;
    uint16   file_path[PF_MAX_FILE_NAME_LENGTH];

    SDI_TRACEID(BT, _DB_Init__IN, "[DB_Init] IN");

    SCI_MEMSET(&g_db, 0, sizeof(g_db));
    g_dev_ind = 0;

    /* Initialize fields to default value */
    g_db.status = FALSE;
    g_db.visibility = 0;
    PF_WStrCpy(g_db.local_name, g_BTConfig.local_name);
    g_db.dev_count = 0;

    /* Read configuration from file */
    if(0 != PF_mkdir((const uint16 *)g_BTConfig.bt_file_dir))
    {
        SDI_TRACEID(BT, _DB_Init__PF_mkdir_failed, "[DB_Init] PF_mkdir failed");
        return FALSE;
    }

    PF_WStrCpy(file_path, (uint16 *)g_BTConfig.bt_file_dir);
    PF_WStrCat(file_path, L"\\");
    PF_WStrCat(file_path, DATABASE_FILE_NAME);

    file = PF_fopen(file_path, "r");

    if(0 == file)
    {
        SCI_TRACE_LOW("[DB_Init] Open database file failed");
        return FALSE;
    }

    do
    {
        ret = FALSE;

        /* Read signature, 4 bytes*/
        if(1 != PF_fread(&signature, sizeof(signature), 1, file))
        {
            SCI_TRACE_LOW("[DB_Init] Read signature failed");
            break;
        }

        SCI_TRACE_LOW("[DB_Init] signature = 0x%x", signature);

        if(DATABASE_SIGNATURE != signature)
        {
            SCI_TRACE_LOW("[DB_Init] Unknown signature");
            break;
        }

        /* Read status, 1 bytes */
        if(1 != PF_fread(&g_db.status, sizeof(g_db.status), 1, file))
        {
            SCI_TRACE_LOW("[DB_Init] Read status failed");
            g_db.status = FALSE;
            break;
        }

        SCI_TRACE_LOW("[DB_Init] status = %d", g_db.status);

        /* Read visibility, 1 bytes */
        if(1 != PF_fread(&g_db.visibility, sizeof(g_db.visibility), 1, file))
        {
            SCI_TRACE_LOW("[DB_Init] Read visibility failed");
            g_db.visibility = FALSE;
            break;
        }

        SCI_TRACE_LOW("[DB_Init] visibility = %d", g_db.visibility);

        /* Read size of local name, 2 bytes */
        if(1 != PF_fread(&count, sizeof(count), 1, file))
        {
            SCI_TRACE_LOW("[DB_Init] Read size of local name failed");
            break;
        }

        SCI_TRACE_LOW("[DB_Init] size of local name = %d", count);

        if(((count & 0x01) != 0) || (count > sizeof(g_db.local_name) - 2))
        {
            SCI_TRACE_LOW("[DB_Init] Invalid count number");
            break;
        }

        /* Read local name, count bytes */
        if(1 != PF_fread(g_db.local_name, count, 1, file))
        {
            SCI_TRACE_LOW("[DB_Init] Read local name failed");
            PF_WStrCpy(g_db.local_name, g_BTConfig.local_name);
            break;
        }

        g_db.local_name[count / 2] = 0;

        /* Read count of device record, 2 bytes */
        if(1 != PF_fread(&count, sizeof(count), 1, file))
        {
            SCI_TRACE_LOW("[DB_Init] Read count of device records failed");
            break;
        }

        SCI_TRACE_LOW("[DB_Init] count of device records = %d", count);

        if(count > ARRAY_SIZE(g_db.dev_list))
        {
            SCI_TRACE_LOW("[DB_Init] Invalid count number");
            break;
        }

        /* Read device record */
        for(i = 0; i < count; ++i)
        {
            if(1 != PF_fread(&g_db.dev_list[i].record, sizeof(g_db.dev_list[i].record), 1, file))
            {
                SCI_TRACE_LOW("[DB_Init] Read %d th record failed", i);
                SCI_MEMSET(&g_db.dev_list[i].record, 0, sizeof(g_db.dev_list[i].record));
                break;
            }

            g_db.dev_list[i].record_valid = TRUE;
        }

        SCI_TRACE_LOW("[DB_Init] total %d records read", i);
        g_db.dev_count = i;

        if(i == count)
        {
            ret = TRUE;
        }
    }
    while(0);

    PF_fclose(file);
    file = 0;

    return ret;
}

BOOLEAN _DB_SaveToFile(void)
{
    PF_FILE   file;
    uint32    signature = DATABASE_SIGNATURE;
    uint16    count;     /* It should be 2 bytes long */
    BOOLEAN   ret;
    int       i;
    uint16    file_path[PF_MAX_FILE_NAME_LENGTH];

    SCI_TRACE_LOW("DB_SaveToFile:IN");

    if(0 != PF_mkdir((const uint16 *)g_BTConfig.bt_file_dir))
    {
        SCI_TRACE_LOW("DB_SaveToFile:create dir failed");
        return FALSE;
    }

    PF_WStrCpy(file_path, (const uint16 *)g_BTConfig.bt_file_dir);
    PF_WStrCat(file_path, L"\\");
    PF_WStrCat(file_path, DATABASE_FILE_NAME);

    file = PF_fopen(file_path, "w");

    if(0 == file)
    {
        SCI_TRACE_LOW("DB_SaveToFile:Create database file failed");
        return FALSE;
    }

    do
    {
        ret = FALSE;

        /* Write signature, 4 bytes */
        if(1 != PF_fwrite(&signature, sizeof(signature), 1, file))
        {
            SCI_TRACE_LOW("DB_SaveToFile:Write signature failed");
            break;
        }

        /* Write status, 1 bytes */
        SCI_TRACE_LOW("DB_SaveToFile:status = %d", g_db.status);

        if(1 != PF_fwrite(&g_db.status, sizeof(g_db.status), 1, file))
        {
            SCI_TRACE_LOW("DB_SaveToFile:Write status failed");
            break;
        }

        /* Write visibility, 1 bytes */
        SCI_TRACE_LOW("DB_SaveToFile:visibility = %d", g_db.visibility);

        if(1 != PF_fwrite(&g_db.visibility, sizeof(g_db.visibility), 1, file))
        {
            SCI_TRACE_LOW("DB_SaveToFile:Write visibility failed");
            break;
        }

        /* PF_WStrLen returns length of unicode string in BYTE */
        count = PF_WStrLen(g_db.local_name);
        SCI_TRACE_LOW("DB_SaveToFile:size of local name = %d", count);
        Assert(count);/*assert verified*/

        /* Write size of local name, 2 bytes */
        if(1 != PF_fwrite(&count, sizeof(count), 1, file))
        {
            SCI_TRACE_LOW("DB_SaveToFile:Write size of local name failed");
            break;
        }

        /* Write local name, count bytes */
        if(1 != PF_fwrite(g_db.local_name, count, 1, file))
        {
            SCI_TRACE_LOW("DB_SaveToFile:Write local name failed");
            break;
        }

#if DB_DEBUG_ON
        count = 0;

        for(i = 0; i < ARRAY_SIZE(g_db.dev_list); ++i)
        {
            if(g_db.dev_list[i].record_valid)
            {
                count++;
            }
        }

#else
count = g_db.dev_count;
#endif /* DB_DEBUG_ON */

        Assert(count == g_db.dev_count);/*assert verified*/

        /* Write count of device records, 2 bytes */
        SCI_TRACE_LOW("DB_SaveToFile:count of device records = %d", count);

        if(1 != PF_fwrite(&count, sizeof(count), 1, file))
        {
            SCI_TRACE_LOW("DB_SaveToFile:Write count of device records failed");
            break;
        }

        count = 0;

        for(i = 0; i < ARRAY_SIZE(g_db.dev_list); ++i)
        {
            if(g_db.dev_list[i].record_valid)
            {
                if(1 != PF_fwrite(&g_db.dev_list[i].record, sizeof(g_db.dev_list[i].record), 1, file))
                {
                    SCI_TRACE_LOW("DB_SaveToFile:Write %d th device record failed");
                    break;
                }

                ++count;
            }
        }

        SCI_TRACE_LOW("DB_SaveToFile:total %d records written", count);

        if(i == ARRAY_SIZE(g_db.dev_list))
        {
            ret = TRUE;
        }
    }
    while(0);

    PF_fclose(file);
    file = 0;
    /* TBD:
     * should we delete the file if error occurs?
     */

    return ret;
}
#endif /* _DB_IN_NV */	

#endif

#if defined(PLATFORM_SC6531EFM)
#define ENABLE_CALIBRATION 1 //enable calibration: 1; disable calibration: 0

#define FM_RC_CALI
#if ENABLE_CALIBRATION
#define FM_DCOC_CALI
#define IQ_CALI_INCLUDE
#endif
#define		SIN_WAV_1M		0
#define		SIN_WAV_10K		1

#define 		IQquan_width			13
//#define		Fs						800.0e3
//#define   	Fiq						100.0e3
//#define  		IQ_shift_bit			3
//#define		IQINTEGRATION_LEN		1024*4
//#define		IQINTEGRATION_SHIFT		12


uint32 debug_fm_ctl_debug2_backup,fm_dcoc_mode_backup,debug_fm_adc_debug_backup;
uint32 rc_cali_r_ctl_backup,ldo_addabbvco_ctl0_backup,reg_fm_adc_dubug_backup;
uint32 reg_fm_ctrl_debug2_backup,fm_sx_divn_ctrl0,fm_sx_divnfrac_ctrlh_backup;
uint32 fm_sx_divnfrac_ctrll_backup,fm_sx_lodiv_ctrl0,fm_sx_afc_start;

#ifdef FM_RC_CALI
/*lint -e516*/
uint32 RC_cali_ctl(uint32 flag, uint32 cbank_data)
{
	uint32 sin_gen_freq,data,i,wb_rssi;

	if (flag == SIN_WAV_1M)
	{
		sin_gen_freq = REG32(FM_REG_SIN_GEN);
		sin_gen_freq &= ~0xFF00;
		REG32(FM_REG_SIN_GEN) = sin_gen_freq;
	}
	else
	{
		sin_gen_freq = REG32(FM_REG_SIN_GEN);
		sin_gen_freq &= ~0xFF00;
		sin_gen_freq |= 100 << 8;
		REG32(FM_REG_SIN_GEN) = sin_gen_freq;
	}
	
	//config CBANK
	trout_read_rf_reg(FM_RX_PGA_CBANK_CTRL0, &data);
	data &= ~0xFC;
	data |= cbank_data << 2;
	trout_fm_write_rf_reg(FM_RX_PGA_CBANK_CTRL0, data);

	
	REG32(FM_REG_SIN_GEN) |= BIT_0 | BIT_4;
	//rf 1f7 b15 1
	trout_read_rf_reg(FM_DCOC_MODE, &data);
	data |= BIT_15;
	trout_fm_write_rf_reg(FM_DCOC_MODE, data);
	
	REG32(FM_REG_CALI_CTL) |= BIT_0;
	//delay 10us
	FM_delay1000();
	REG32(FM_REG_CALI_CTL) |= BIT_4 | BIT_5;
	
	while ((REG32(FM_REG_RC_DC_CALI) & BIT_28) != BIT_28)
	{
		FM_delay100();
	}

	wb_rssi = REG32(FM_REG_MISC_STS) & 0xFF;

	REG32(FM_REG_SIN_GEN) &= ~BIT_0;
	REG32(FM_REG_CALI_CTL) &= ~BIT_4;
	REG32(FM_REG_CALI_CTL) &= ~BIT_5;
	REG32(FM_REG_CALI_CTL) &= ~BIT_0;
	
	trout_read_rf_reg(FM_DCOC_MODE, &data);
	data &= ~BIT_15;
	trout_fm_write_rf_reg(FM_DCOC_MODE, data);

	return wb_rssi;
}

void RC_cali_init()
{
	uint32 data;
	// 1. marlin power on
	trout_read_rf_reg(DEBUG_FM_CTRL_DEBUG2, &debug_fm_ctl_debug2_backup);
	trout_read_rf_reg(DEBUG_FM_CTRL_DEBUG2, &data);
	data |= debug_DA_FM_SHORTANT_H | debug_DA_FM_SX_DSM_RESETN | debug_DA_FM_SX_AFC_RESETN;
	trout_fm_write_rf_reg(DEBUG_FM_CTRL_DEBUG2, data);

	trout_read_rf_reg(FM_DCOC_MODE, &fm_dcoc_mode_backup);
	trout_read_rf_reg(FM_DCOC_MODE, &data);
	data |= FM_CAL_RC_MODE | DEBUG_DA_FM_PGA_DCOCQ;
	trout_fm_write_rf_reg(FM_DCOC_MODE, data);

	trout_read_rf_reg(DEBUG_FM_ADC_DEBUG, &debug_fm_adc_debug_backup);
	trout_read_rf_reg(DEBUG_FM_ADC_DEBUG, &data);
	data |= debug_DA_FM_ADC_EN | debug_DA_FMADCCLK_EN;
	trout_fm_write_rf_reg(DEBUG_FM_ADC_DEBUG, data);

	// 4. set cal r to defult value
	//trout_read_rf_reg(REG_CAL_R_CTRL, &rc_cali_r_ctl_backup);
	//trout_fm_write_rf_reg(REG_CAL_R_CTRL, 0x0);
	
	// 5. set cal v to defult value
	//trout_read_rf_reg(LDO_ADDABBVCO_CTRL0, &ldo_addabbvco_ctl0_backup);
	//trout_fm_write_rf_reg(LDO_ADDABBVCO_CTRL0, 0xE000);

	// 11. enable adc
	trout_read_rf_reg(REG_FM_ADC_DEBUG, &reg_fm_adc_dubug_backup);
	trout_read_rf_reg(REG_FM_ADC_DEBUG, &data);
	data |= reg_DA_FM_ADC_EN | reg_DA_FMADCCLK_EN;
	trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, data);

	//trout_read_rf_reg(REG_FM_ADC_DEBUG, &data);
	//data &= ~reg_DA_FM_ADC_RSTN;
	//trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, data);

	//trout_read_rf_reg(REG_FM_ADC_DEBUG, &data);
	//data |= reg_DA_FM_ADC_RSTN;
	//trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, data);

	// fm mode
	trout_read_rf_reg(FM_MODE, &data);
	data &= ~0xF000;
	data |= 0x6000;
	trout_fm_write_rf_reg(FM_MODE, data);
}

void RC_cali_exit()
{
	uint32 data;

	trout_read_rf_reg(FM_MODE, &data);
	data &= ~0xF000;
	data |= 0x5000;
	trout_fm_write_rf_reg(FM_MODE, data);

	trout_fm_write_rf_reg(DEBUG_FM_CTRL_DEBUG2, debug_fm_ctl_debug2_backup);

	trout_fm_write_rf_reg(FM_DCOC_MODE, fm_dcoc_mode_backup);

	trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, reg_fm_adc_dubug_backup);

	trout_fm_write_rf_reg(DEBUG_FM_ADC_DEBUG, debug_fm_adc_debug_backup);

	//trout_fm_write_rf_reg(REG_CAL_R_CTRL, rc_cali_r_ctl_backup);
	
	//trout_fm_write_rf_reg(LDO_ADDABBVCO_CTRL0, ldo_addabbvco_ctl0_backup);
}
#endif

#ifdef FM_RC_CALI
/*lint -e63*/
void RC_calibration()
{
	int32 super_rssi_1M,super_rssi_10K,temp;
	uint32 i;
	uint32 cbank_data = 32;
	//close path between TIA and PGA

	RC_cali_init();

	super_rssi_10K = RC_cali_ctl(SIN_WAV_10K, cbank_data);
	SCI_TraceLow("FM_IDRC 10k rssi:%d\r\n", super_rssi_10K);
	///temp = (int32)floor(0.447 * super_rssi_10K);
	temp = super_rssi_10K - 8;
	//while (1)
	//{
		

		for (i = 0; i < 5; i++)
		{
			super_rssi_1M = RC_cali_ctl(SIN_WAV_1M, cbank_data);
			SCI_TraceLow("FM_RC cnt %d, cbank_data:%x 1M rssi:%d\r\n", i,cbank_data,super_rssi_1M);
			if ((super_rssi_1M - temp) > 0)
			{
				cbank_data += 1 << (4 - i);
			}
			else if ((super_rssi_1M - temp) < 0)
			{
				cbank_data -= 1 << (4 - i);
			}
		}
		
		super_rssi_1M = RC_cali_ctl(SIN_WAV_1M, cbank_data);
		SCI_TraceLow("FM_IDRC 1M rssi:%d\r\n", super_rssi_1M);
		if ((super_rssi_1M - temp) > 0)
		{
			cbank_data |= 1 ;
		}
		else if ((super_rssi_1M - temp) < 0)
		{
			cbank_data &= ~1;
		}

		super_rssi_1M = RC_cali_ctl(SIN_WAV_1M, cbank_data);
		SCI_TraceLow("FM_RC %x, 1M rssi:%d\r\n", cbank_data, super_rssi_1M);
		//SCI_TraceLow("FM_IDRC 1M rssi:%d\r\n", super_rssi_1M);
		//SCI_TraceLow("FM_IDRC cbank_data:%x\r\n", cbank_data);
		//SCI_TRACE_LOW("fm RC cali super rssi:%d\n", super_rssi_1M);
		//SCI_TRACE_LOW("fm RC cali cbank_data:%d\n", cbank_data);
		//SCI_TRACE_LOW("fm RC cali temp:%d\n", temp);
		//SCI_TRACE_LOW("RC cali is done!\n");

		//for(i = 0; i<10; i++);
	RC_cali_exit();
	//}
}
#endif

#ifdef FM_DCOC_CALI
#define		DCOC_IDAC_7BITS				7
#define		DCOC_IDAC_5BITS				5
//#define 	IQquan_width				13		//c_save and d_save quantize width Lixia  cathy,modified in 2014.01.28

typedef enum 
{
	FM_DCOC_MAX_ABS_VALUE0	= 0,
	FM_DCOC_MAX_ABS_VALUE1	= 1,	
	FM_DCOC_MAX_ABS_ERROR
}FM_DCOC_MAX_ABS_ENUM_UINT8;

#define 	RX_DCOC_I_IDAC_LENGTH		(6)
#define 	RX_DCOC_IDAC1_LENGTH		(5)
#define 	RX_DCOC_IDAC2_LENGTH		(6)
#define 	RX_DCOC_LEVER_NUM			(5)
#define 	RX_DCOC_IDAC1_LEVER_NUM			(1)
#define 	RX_DCOC_IDAC2_LEVER_NUM			(5)
/*****************************************************************************
Global variables                                                       
*****************************************************************************/

uint32  g_rx_dcoc_tia_gain_value[RX_DCOC_IDAC1_LEVER_NUM]= {0x9};
// 0x2 -- 21dB
/*lint -e129*/
//uint32  g_rx_dcoc_pga_gain_value[RX_DCOC_IDAC2_LEVER_NUM]= { 0x2, 0x4, 0x6, 0x8, 0xA };
uint32  g_rx_dcoc_pga_gain_value[RX_DCOC_IDAC2_LEVER_NUM]= { 0x0, 0x4, 0x6, 0x8, 0xA };
int8   	g_rx_tia_dcoc_adjust_i_code[RX_DCOC_IDAC1_LEVER_NUM]= { 0 };
int8   	g_rx_tia_dcoc_adjust_q_code[RX_DCOC_IDAC1_LEVER_NUM]= { 0 };
int8   	g_rx_pga_dcoc_adjust_i_code[RX_DCOC_IDAC2_LEVER_NUM]= { 0 };
int8   	g_rx_pga_dcoc_adjust_q_code[RX_DCOC_IDAC2_LEVER_NUM]= { 0 };
uint32  g_rx_pga_dcoc_adjust[RX_DCOC_IDAC2_LEVER_NUM]= { 0 };
/*lint +e129*/


/*****************************************************************************
Get global variables                                                        
*****************************************************************************/
#define	Get_tia_Gain_Value( Index )						g_rx_dcoc_tia_gain_value[Index]
#define	Get_pga_Gain_Value( Index )						g_rx_dcoc_pga_gain_value[Index]
#define	Get_Code_tia_I_Value( Index )					g_rx_tia_dcoc_adjust_i_code[Index]
#define	Get_Code_tia_Q_Value( Index )					g_rx_tia_dcoc_adjust_q_code[Index]
#define	Set_Code_tia_I_Value( Index, Value )			(g_rx_tia_dcoc_adjust_i_code[Index] = Value)
#define	Set_Code_tia_Q_Value( Index, Value )			(g_rx_tia_dcoc_adjust_q_code[Index] = Value)
#define	Get_Code_pga_I_Value( Index )					g_rx_pga_dcoc_adjust_i_code[Index]
#define	Get_Code_pga_Q_Value( Index )					g_rx_pga_dcoc_adjust_q_code[Index]
#define	Set_Code_pga_I_Value( Index, Value )			(g_rx_pga_dcoc_adjust_i_code[Index] = Value)
#define	Set_Code_pga_Q_Value( Index, Value )			(g_rx_pga_dcoc_adjust_q_code[Index] = Value)
#endif

#ifdef FM_DCOC_CALI
void DCOC_cali_init()
{
	uint32 data;
	// 1. marlin power on
#if 0
	// 2. debug mode
	trout_read_rf_reg(WB_MANUAL_CFG, &data);
	data |= WB_MANUAL_EN | WB_SEL_MANUAL_EN;
	trout_fm_write_rf_reg(WB_MANUAL_CFG, data);

	trout_read_rf_reg(DEBUG_WB_SX_CAL_DEBUG, &data);
	data |= DA_ISM_SX_AFC_STARTL2H | DA_ISM_SX_AFC_RESETN | DA_ISM_SX_DSM_RESETN;
	trout_fm_write_rf_reg(DEBUG_WB_SX_CAL_DEBUG, data);

	trout_read_rf_reg(DEBUG_WB_AGC_DEBUG_EN, &data);
	data |= DA_ISM_RX_GLNA | DA_ISM_RX_ABB_GC_BQ | DA_WF_RX_ABB_GC_1rd;
	trout_fm_write_rf_reg(DEBUG_WB_AGC_DEBUG_EN, data);

	trout_read_rf_reg(WB_DCOC_MODE, &data);
	data |= DEBUG_DA_ISM_RX_ABB_DCOC_DAC_I | DEBUG_DA_ISM_RX_ABB_DCOC_DAC_Q;
	trout_fm_write_rf_reg(WB_DCOC_MODE, data);

	trout_read_rf_reg(DEBUG_WB_TXGAIN_DEBUG_EN, &data);
	data |= debug_DA_ISM_TXDAC_GC | debug_DA_ISM_TXGM_GC | debug_DA_ISM_TXMOD_GC;
	trout_fm_write_rf_reg(DEBUG_WB_TXGAIN_DEBUG_EN, data);

	trout_read_rf_reg(DEBUG_WB_ADC_DEBUG, &data);
	data |= debug_DA_ISM_ADC_EN | debug_DA_ISMADCCLK_EN | debug_DA_ISM_ADC_RSTN | debug_DA_WF_ADCCLK_SEL120ML;
	trout_fm_write_rf_reg(DEBUG_WB_ADC_DEBUG, data);

	trout_read_rf_reg(DEBUG_WB_RSSIADC_DEBUG, &data);
	data |= debug_DA_ISM_RSSIADC_EN | debug_DA_ISMRSSIADCCLK_EN | debug_DA_ISM_RSSIADC_RSTN;
	trout_fm_write_rf_reg(DEBUG_WB_RSSIADC_DEBUG, data);

	trout_read_rf_reg(DEBUG_WB_DAC_DEBUG, &data);
	data |= debug_DA_ISM_DAC_EN | debug_DA_ISM_TXBIAS_EN | debug_DA_ISMDACCLK_EN | debug_DA_ISM_DAC_WIFI_BT_SEL | debug_DA_ISM_DAC_RSTN;
	trout_fm_write_rf_reg(DEBUG_WB_DAC_DEBUG, data);
#endif
	trout_read_rf_reg(DEBUG_FM_CTRL_DEBUG2, &debug_fm_ctl_debug2_backup);
	trout_read_rf_reg(DEBUG_FM_CTRL_DEBUG2, &data);
	data |= debug_DA_FM_SHORTANT_H | debug_DA_FM_SX_DSM_RESETN | debug_DA_FM_SX_AFC_RESETN;
	trout_fm_write_rf_reg(DEBUG_FM_CTRL_DEBUG2, data);

	trout_read_rf_reg(REG_FM_CTRL_DEBUG2, &reg_fm_ctrl_debug2_backup);
	trout_read_rf_reg(REG_FM_CTRL_DEBUG2, &data);
	data |= reg_DA_FM_SX_DSM_RESETN | reg_DA_FM_SX_AFC_RESETN;
	trout_fm_write_rf_reg(REG_FM_CTRL_DEBUG2, data);

	trout_read_rf_reg(FM_DCOC_MODE, &fm_dcoc_mode_backup);
	trout_read_rf_reg(FM_DCOC_MODE, &data);
	data |= DEBUG_DA_FM_PGA_DCOCI | DEBUG_DA_FM_PGA_DCOCQ;
	trout_fm_write_rf_reg(FM_DCOC_MODE, data);

	trout_read_rf_reg(DEBUG_FM_ADC_DEBUG, &debug_fm_adc_debug_backup);
	trout_read_rf_reg(DEBUG_FM_ADC_DEBUG, &data);
	//data |= debug_DA_FM_SHORTANT_H | debug_DA_FM_SX_DSM_RESETN | debug_DA_FM_SX_AFC_RESETN;
	data |= debug_DA_FM_SHORTANT_H | debug_DA_FM_SX_DSM_RESETN;
	trout_fm_write_rf_reg(DEBUG_FM_ADC_DEBUG, data);

	// 4. set cal r to defult value
	//trout_read_rf_reg(REG_CAL_R_CTRL, &rc_cali_r_ctl_backup);
	//trout_fm_write_rf_reg(REG_CAL_R_CTRL, 0x0);
	
	// 5. set cal v to defult value
	//trout_read_rf_reg(LDO_ADDABBVCO_CTRL0, &ldo_addabbvco_ctl0_backup);
	//trout_fm_write_rf_reg(LDO_ADDABBVCO_CTRL0, 0xE000);

	// 8. lo freq
	trout_read_rf_reg(FM_SX_DIVN_CTRL0, &fm_sx_divn_ctrl0);
	trout_read_rf_reg(FM_SX_DIVNFRAC_CTRLH, &fm_sx_divnfrac_ctrlh_backup);
	trout_read_rf_reg(FM_SX_DIVNFRAC_CTRLL, &fm_sx_divnfrac_ctrll_backup);
	trout_read_rf_reg(FM_SX_LODIV_CTRL0, &fm_sx_lodiv_ctrl0);
	trout_fm_write_rf_reg(FM_SX_DIVN_CTRL0, 0x6E00);
	trout_fm_write_rf_reg(FM_SX_DIVNFRAC_CTRLH, 0xF000);
	trout_fm_write_rf_reg(FM_SX_DIVNFRAC_CTRLL, 0x24B5);
	trout_fm_write_rf_reg(FM_SX_LODIV_CTRL0, 0x9000);

	// 10. sx lock
	trout_read_rf_reg(FM_SX_AFC_START, &fm_sx_afc_start);
	trout_read_rf_reg(FM_SX_AFC_START, &data);
	data |= RG_FM_SX_AFC_START;
	trout_fm_write_rf_reg(FM_SX_AFC_START, data);

	// 11. enable adc
	trout_read_rf_reg(REG_FM_ADC_DEBUG, &reg_fm_adc_dubug_backup);
	trout_read_rf_reg(REG_FM_ADC_DEBUG, &data);
	data |= reg_DA_FM_ADC_EN | reg_DA_FMADCCLK_EN;
	trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, data);

	trout_read_rf_reg(REG_FM_ADC_DEBUG, &data);
	data &= ~reg_DA_FM_ADC_RSTN;
	trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, data);

	//trout_read_rf_reg(REG_FM_ADC_DEBUG, &data);
	//data |= reg_DA_FM_ADC_RSTN;
	//trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, data);

	// fm mode
	trout_read_rf_reg(FM_MODE, &data);
	data &= ~0xF000;
	data |= 0x7000;
	trout_fm_write_rf_reg(FM_MODE, data);
}

void DCOC_cali_exit()
{
	uint32 data;

	trout_read_rf_reg(FM_MODE, &data);
	data &= ~0xF000;
	data |= 0x5000;
	trout_fm_write_rf_reg(FM_MODE, data);

	trout_fm_write_rf_reg(REG_FM_CTRL_DEBUG2, reg_fm_ctrl_debug2_backup);
	trout_fm_write_rf_reg(DEBUG_FM_CTRL_DEBUG2, debug_fm_ctl_debug2_backup);

	trout_fm_write_rf_reg(FM_DCOC_MODE, fm_dcoc_mode_backup);

	trout_fm_write_rf_reg(REG_FM_ADC_DEBUG, reg_fm_adc_dubug_backup);

	trout_fm_write_rf_reg(DEBUG_FM_ADC_DEBUG, debug_fm_adc_debug_backup);

	//trout_fm_write_rf_reg(REG_CAL_R_CTRL, rc_cali_r_ctl_backup);
	
	//trout_fm_write_rf_reg(LDO_ADDABBVCO_CTRL0, ldo_addabbvco_ctl0_backup);

	trout_fm_write_rf_reg(FM_SX_DIVN_CTRL0, fm_sx_divn_ctrl0);
	trout_fm_write_rf_reg(FM_SX_DIVNFRAC_CTRLH, fm_sx_divnfrac_ctrlh_backup);
	trout_fm_write_rf_reg(FM_SX_DIVNFRAC_CTRLL, fm_sx_divnfrac_ctrll_backup);
	trout_fm_write_rf_reg(FM_SX_LODIV_CTRL0, fm_sx_lodiv_ctrl0);
	trout_fm_write_rf_reg(FM_SX_AFC_START, fm_sx_afc_start);
}
#endif

/*lint +e63*/
#ifdef FM_DCOC_CALI
uint8 rx_dcoc_get_MaxAbsValue( uint16 uhwValue0, uint16 uhwValue1, uint16 uhwMidValue)
{
	uint8	ucCmpFlag;
	
	if(( uhwValue0 >= uhwMidValue) && ( uhwValue1 >= uhwMidValue ))
	{
		if( uhwValue0 >= uhwValue1  )
		{
			ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_VALUE0;
		}
		else
		{
			ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_VALUE1;
		}
		
	}
	else if(( uhwValue0 <= uhwMidValue) && ( uhwValue1 <= uhwMidValue ))
	{
		if( uhwValue0 <= uhwValue1  )
		{
			ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_VALUE0;
		}
		else
		{
			ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_VALUE1;
		}
	}
	else if(( uhwValue0 >= uhwMidValue) && ( uhwValue1 <= uhwMidValue ))
	{
		if(( uhwValue0 - uhwMidValue ) >= ( uhwMidValue - uhwValue1 ))
		{
			ucCmpFlag	=(uint8)FM_DCOC_MAX_ABS_VALUE0;
		}
		else
		{
			ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_VALUE1;
		}
	}
	else if(( uhwValue0 <= uhwMidValue) && ( uhwValue1 >= uhwMidValue ))
	{
		if(( uhwMidValue - uhwValue0  ) >= ( uhwValue1 -uhwMidValue  ))
		{
			ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_VALUE0;
		}
		else
		{
			ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_VALUE1;
		}
	}
	else
	{
		ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_ERROR;
	}

	return	ucCmpFlag;
}

void rx_dcoc_i_idac2_process(void)
{
	uint32 	reg_adc_i,i,data_deb;
	uint32	reg_idac_tmp;
	uint32 	reg_gain;
	uint16 	i_amg_reg;
	uint16 	i_amg_reg2;
	uint16	i_amg_reg_tmp;
	uint16	uhwLoopI;
	uint16	min_dc_mv=0;
	uint8 	m;
	uint8 	n;
	uint8	reg_i_idac;
	uint8	min_offset=0;
	uint8 	i_reg_idac_ref;
	uint8 	i_reg_idac_ref2;
	uint8	ucCmpFlag;							// 0:use reg2 value; 1:use reg value; 2:error 
	uint8	flag;	
	

	m			= RX_DCOC_IDAC2_LENGTH;
	n			= 0;
	ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_ERROR;

	for(uhwLoopI = 0; uhwLoopI < RX_DCOC_IDAC2_LEVER_NUM; uhwLoopI++)
	{
		m			= RX_DCOC_IDAC2_LENGTH;
		n			= 0;
		SCI_TraceLow("FM_IDDC Igain  pga_gain0x%x\r\n",Get_pga_Gain_Value(uhwLoopI));
		//SCI_TraceLow("FM_IDDC Igain Lv is %d\r\n",uhwLoopI);
		//SCI_TRACE_LOW("i is %d\n",uhwLoopI);
		/*lint -e2311*/
		trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
		data_deb = (uint32)(reg_idac_tmp & 0xffff01ff);
		trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, data_deb);/*lint !e516*/
		
		/* get gain */
		trout_read_rf_reg(FM_RX_GAIN_CTRL0, &reg_gain);
		reg_gain			&= 0xffffe1ff;					//bit7 - bit12
		reg_gain			|= ((Get_pga_Gain_Value(uhwLoopI)) << 9);			//write gain from bit7
		trout_fm_write_rf_reg(FM_RX_GAIN_CTRL0, reg_gain);/*lint !e516*/

		SCI_TraceLow("FM_IDDC Igain Loop is %d, pga_gain_reg0x%x\r\n",uhwLoopI,reg_gain);
	
		REG32(FM_REG_CALI_CTL) |= BIT_0;
		REG32(FM_REG_CALI_CTL) |= BIT_8;
		
		while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
		{
			FM_delay100();
		}

		i_amg_reg = REG32(FM_REG_RC_DC_CALI) & 0xFFF;
		//SCI_TRACE_LOW("dc is 0x%04x\n",i_amg_reg);
		//SCI_TraceLow("FM_IDDC maxgain_dc is 0x%x\r\n",i_amg_reg);
		SCI_TraceLow("FM_IDDC pga_gain 0x%x pga_gain_reg 0x%x dc_cali 0x%x maxgain_dc is 0x%x\r\n",Get_pga_Gain_Value(uhwLoopI),reg_gain,data_deb, i_amg_reg);
		REG32(FM_REG_CALI_CTL) &= ~BIT_8;
		REG32(FM_REG_CALI_CTL) &= ~BIT_0;

		//i_amg_reg		= ( uint16 )((reg_adc_i & 0x0000fff0) >> 4);     //get 4-15bit
		

		if( i_amg_reg > 0x0800 )
		{
			i_reg_idac_ref	= 64;
		}
		else
		{
			i_reg_idac_ref	= 0;
		}

		while( m >= 1 )
		{
			
			if( 0 == n )
			{
				reg_i_idac	=  i_reg_idac_ref +  (1 << (m-1));	
			}
			else
			{
				reg_i_idac	=  i_reg_idac_ref -  (1 << (m-1));	
			}

			//SCI_TRACE_LOW("offset is 0x%04x\n",reg_i_idac);

			trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	

			data_deb = (uint32)(((uint32)(reg_i_idac) << 9) | (reg_idac_tmp & 0xffff01ff));
			trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, data_deb);/*lint !e516*/		//write 9-15bit

			REG32(FM_REG_CALI_CTL) |= BIT_0;
			REG32(FM_REG_CALI_CTL) |= BIT_8;
		
			while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
			{
				FM_delay100();
			}

			i_amg_reg_tmp = REG32(FM_REG_RC_DC_CALI) & 0xFFF;
			//SCI_TRACE_LOW("dc is 0x%04x\n",i_amg_reg_tmp);
			
			SCI_TraceLow("FM_IDDC m:%d ,DC_cali0x%x, dc is 0x%x\r\n",m,data_deb,i_amg_reg_tmp);
			REG32(FM_REG_CALI_CTL) &= ~BIT_8;
			REG32(FM_REG_CALI_CTL) &= ~BIT_0;

			flag = (uint8)rx_dcoc_get_MaxAbsValue( i_amg_reg_tmp,  i_amg_reg, 0x0800 );
			if (flag == 0)
			{
				min_offset = reg_i_idac;
				min_dc_mv = i_amg_reg_tmp;
			}
			else if (flag == 1)
			{
				min_offset = 0;
				min_dc_mv = i_amg_reg;
			}
			
			while(  m > 1 )
			{
				n = 1;
				m--;
				i_reg_idac_ref	= reg_i_idac;
				if ((((i_amg_reg_tmp > 0) && (i_amg_reg_tmp < 2048)) && (i_amg_reg >= 2048))||(((i_amg_reg > 0) && (i_amg_reg < 2048)) && (i_amg_reg_tmp >= 2048)))
				{   
					reg_i_idac	=  i_reg_idac_ref - (1 << (m-1));
				}
				else
				{
					reg_i_idac	=  i_reg_idac_ref + (1 << (m-1));
				}
				//SCI_TRACE_LOW("offset is 0x%04x\n",reg_i_idac);

				/* write idac reg */
				trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
						
				trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, (uint32)(((uint32)(reg_i_idac) << 9) | (reg_idac_tmp & 0xffff01ff)));/*lint !e516*/		//write 9-15bit
				REG32(FM_REG_CALI_CTL) |= BIT_0;
				REG32(FM_REG_CALI_CTL) |= BIT_8;
		
				while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
				{
					FM_delay100();
				}

				i_amg_reg_tmp = REG32(FM_REG_RC_DC_CALI) & 0xFFF;
				//SCI_TRACE_LOW("dc is 0x%04x\n",i_amg_reg_tmp);
				SCI_TraceLow("FM_IDDC m:%d ,iDC_cali0x%x, dc is 0x%x\r\n",m,reg_i_idac,i_amg_reg_tmp);
				REG32(FM_REG_CALI_CTL) &= ~BIT_8;
				REG32(FM_REG_CALI_CTL) &= ~BIT_0;

				flag = (uint8)rx_dcoc_get_MaxAbsValue( i_amg_reg_tmp,  min_dc_mv, 0x0800 );
				if (flag == 0)
				{
					min_offset = reg_i_idac;
					min_dc_mv = i_amg_reg_tmp;
				}			
			}

			//m--;
			i_reg_idac_ref	= reg_i_idac;
			if ((((i_amg_reg_tmp > 0) && (i_amg_reg_tmp < 2048)) && (i_amg_reg >= 2048))||(((i_amg_reg > 0) && (i_amg_reg < 2048)) && (i_amg_reg_tmp >= 2048)))
			    reg_i_idac	=  i_reg_idac_ref & ~BIT_0;
			else
				reg_i_idac	=  i_reg_idac_ref | BIT_0;
			
			//SCI_TRACE_LOW("offset is 0x%04x\n",reg_i_idac);

			/* write idac reg */
			trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
					
			trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, (uint32)(((uint32)(reg_i_idac) << 9) | (reg_idac_tmp & 0xffff01ff)));	/*lint !e516*/	//write 9-15bit
			REG32(FM_REG_CALI_CTL) |= BIT_0;
			REG32(FM_REG_CALI_CTL) |= BIT_8;
		
			while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
			{
				FM_delay100();
			}

			i_amg_reg_tmp = REG32(FM_REG_RC_DC_CALI) & 0xFFF;
			SCI_TraceLow("FM_IDDC m:%d dc_cali 0x%x dc is 0x%x\r\n",m,reg_i_idac, i_amg_reg_tmp);
			//SCI_TRACE_LOW("dc is 0x%04x\n",i_amg_reg_tmp);
			//DCOC_coef_glb_final[1][i] = (REG32(FM_REG_RC_DC_CALI) & 0xFFF000) >> 12;

			REG32(FM_REG_CALI_CTL) &= ~BIT_8;
			REG32(FM_REG_CALI_CTL) &= ~BIT_0;

			flag = (uint8)rx_dcoc_get_MaxAbsValue( i_amg_reg_tmp,  min_dc_mv, 0x0800 );
			if (flag == 0)
			{
				min_offset = reg_i_idac;
				min_dc_mv = i_amg_reg_tmp;
			}


			i_amg_reg		= i_amg_reg_tmp;
			i_reg_idac_ref		= reg_i_idac;

			n = 0;
			m--;
				
		}
		SCI_TraceLow("FM_IDDC min_dc is 0x%x\r\n",min_dc_mv);
		//SCI_TRACE_LOW("min offset is 0x%04x\n",min_offset);
		//SCI_TRACE_LOW("min dc is 0x%04x\n",min_dc_mv);

		/* save the adjust code */
		//ucCmpFlag		= (uint8)rx_dcoc_get_MaxAbsValue( i_amg_reg,  i_amg_reg2, 0x0800 );

		Set_Code_pga_I_Value(uhwLoopI, min_offset);/*lint !e63*/

		SCI_TraceLow("FM_IDDC gainLv:%d, set  0x%x\r\n",uhwLoopI,min_offset);
		/* print, need to test no_continue mode write data ok? */
		//fprintf(code_i, "code %u is , %u \n", uhwLoopI, Get_Code_I_Value(uhwLoopI));
		//////////////////////////////////////////////
	}

	//rx_dcoc_write_i_code();
	
	return;
}

void rx_dcoc_q_idac2_process(void)
{
	uint32 	reg_adc_q,i,data_deb;
	uint32	reg_idac_tmp;
	uint32 	reg_gain;
	uint16 	q_amg_reg;
	uint16 	q_amg_reg2;
	uint16	q_amg_reg_tmp;
	uint16	uhwLoopI;
	uint16	min_dc_mv=0;
	uint8	min_offset=0;
	uint8 	w;
	uint8 	r;
	uint8	reg_q_idac;
	uint8 	q_reg_idac_ref;
	uint8 	q_reg_idac_ref2;
	uint8	ucCmpFlag;							// 0:use reg2 value; 1:use reg value; 2:error 
	uint8	flag;
	

	w			= RX_DCOC_IDAC2_LENGTH;
	r			= 0;
	ucCmpFlag	= (uint8)FM_DCOC_MAX_ABS_ERROR;

	for(uhwLoopI = 0; uhwLoopI < RX_DCOC_IDAC2_LEVER_NUM; uhwLoopI++)
	{
		w			= RX_DCOC_IDAC2_LENGTH;
		r			= 0;
		
		//SCI_TRACE_LOW("i is %d\n",uhwLoopI);
		//SCI_TraceLow("FM_IDDC Qgain Lv is %d\r\n",uhwLoopI);
		SCI_TraceLow("FM_IDDC Qgain Lv is %d, pga_gain0x%x\r\n",uhwLoopI,Get_pga_Gain_Value(uhwLoopI));

		
		trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
		data_deb = (uint32)(reg_idac_tmp & 0xfffffe03);
		trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, data_deb);/*lint !e516*/
		
		/* get gain */
		trout_read_rf_reg(FM_RX_GAIN_CTRL0, &reg_gain);
		reg_gain			&= 0xffffe1ff;					//bit7 - bit12
		reg_gain			|= ((Get_pga_Gain_Value(uhwLoopI)) << 9);			

//write gain from bit7
		trout_fm_write_rf_reg(FM_RX_GAIN_CTRL0, reg_gain);/*lint !e516*/
	
		REG32(FM_REG_CALI_CTL) |= BIT_0;
		REG32(FM_REG_CALI_CTL) |= BIT_8;
		
		while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
		{
			FM_delay100();
		}

		//i_amg_reg = REG32(FM_REG_RC_DC_CALI) & 0xFFF;
		q_amg_reg = (REG32(FM_REG_RC_DC_CALI) & 0xFFF000) >> 12;
		//SCI_TraceLow("FM_IDDC maxgain_dc is 0x%x\r\n",q_amg_reg);
		//SCI_TRACE_LOW("dc is 0x%04x\n",q_amg_reg);
		SCI_TraceLow("FM_IDDC pga_gain 0x%x pga_gain_reg 0x%x dc_cali 0x%x maxgain_dc is 0x%x\r\n",Get_pga_Gain_Value(uhwLoopI),reg_gain,data_deb, q_amg_reg);

		REG32(FM_REG_CALI_CTL) &= ~BIT_8;
		REG32(FM_REG_CALI_CTL) &= ~BIT_0;

		//i_amg_reg		= ( uint16 )((reg_adc_i & 0x0000fff0) >> 4);     //get 4-15bit
		

		if( q_amg_reg > 0x0800 )
		{
			q_reg_idac_ref	= 64;
		}
		else
		{
			q_reg_idac_ref	= 0;
		}

		while( w >= 1 )
		{
			if( 0 == r )
			{
				reg_q_idac	=  q_reg_idac_ref +  (1 << (w-1));	
			}
			else
			{
				reg_q_idac	=  q_reg_idac_ref -  (1 << (w-1));	
			}

			//SCI_TRACE_LOW("offset is 0x%04x\n",reg_q_idac);

			/* write idac reg */
			trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	

			trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, (uint32)(((uint32)(reg_q_idac) << 2) | (reg_idac_tmp & 0xfffffe03)));	/*lint !e516*/	//write 9-15bit

			REG32(FM_REG_CALI_CTL) |= BIT_0;
			REG32(FM_REG_CALI_CTL) |= BIT_8;
		
			while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
			{
				FM_delay100();
			}

			//i_amg_reg = REG32(FM_REG_RC_DC_CALI) & 0xFFF;
			q_amg_reg_tmp = (REG32(FM_REG_RC_DC_CALI) & 0xFFF000) >> 12;
			//SCI_TRACE_LOW("dc is 0x%04x\n",q_amg_reg_tmp);
			//SCI_TraceLow("FM_IDDC w:%d dc is 0x%x\r\n",w,q_amg_reg_tmp);
			SCI_TraceLow("FM_IDDC w is%x, DC_cali 0x%x,DC0x%x\r\n",w,reg_q_idac,q_amg_reg_tmp);

			REG32(FM_REG_CALI_CTL) &= ~BIT_8;
			REG32(FM_REG_CALI_CTL) &= ~BIT_0;

			flag = (uint8)rx_dcoc_get_MaxAbsValue( q_amg_reg_tmp,  q_amg_reg, 0x0800 );
			if (flag == 0)
			{
				min_offset = reg_q_idac;
				min_dc_mv = q_amg_reg_tmp;
			}
			else if (flag == 1)
			{
				min_offset = 0;
				min_dc_mv = q_amg_reg;
			}			
			
			while( w > 1 )
			{
				r = 1;
				w--;
				q_reg_idac_ref	= reg_q_idac;
				if ((((q_amg_reg_tmp > 0) && (q_amg_reg_tmp < 2048)) && (q_amg_reg >= 2048))||(((q_amg_reg > 0) && (q_amg_reg < 2048)) && (q_amg_reg_tmp >= 2048)))
				{   
					reg_q_idac	=  q_reg_idac_ref - (1 << (w-1));
				}
				else
				{
					reg_q_idac	=  q_reg_idac_ref + (1 << (w-1));
				}
				
				//SCI_TRACE_LOW("offset is 0x%04x\n",reg_q_idac);

				/* write idac reg */
				trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
		
				trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, (uint32)(((uint32)(reg_q_idac) << 2) | (reg_idac_tmp & 0xfffffe03)));	/*lint !e516*/	//write 9-15bit
				REG32(FM_REG_CALI_CTL) |= BIT_0;
				REG32(FM_REG_CALI_CTL) |= BIT_8;
		
				while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
				{
					FM_delay100();
				}

				//i_amg_reg = REG32(FM_REG_RC_DC_CALI) & 0xFFF;
				q_amg_reg_tmp = (REG32(FM_REG_RC_DC_CALI) & 0xFFF000) >> 12;
				//SCI_TraceLow("FM_IDDC w:%d dc is 0x%x\r\n",w,q_amg_reg_tmp);
				//SCI_TRACE_LOW("dc is 0x%04x\n",q_amg_reg_tmp);
				SCI_TraceLow("FM_IDDC w is %d ,DC_cali 0x%x,DC0x%x\r\n",w,reg_q_idac,q_amg_reg_tmp);


				REG32(FM_REG_CALI_CTL) &= ~BIT_8;
				REG32(FM_REG_CALI_CTL) &= ~BIT_0;

				flag = (uint8)rx_dcoc_get_MaxAbsValue( q_amg_reg_tmp,  min_dc_mv, 0x0800 );
				if (flag == 0)
				{
					min_offset = reg_q_idac;
					min_dc_mv = q_amg_reg_tmp;
				}		
			}

			q_reg_idac_ref	= reg_q_idac;
			if ((((q_amg_reg_tmp > 0) && (q_amg_reg_tmp < 2048)) && (q_amg_reg >= 2048))||(((q_amg_reg > 0) && (q_amg_reg < 2048)) && (q_amg_reg_tmp >= 2048)))
			    reg_q_idac	=  q_reg_idac_ref & ~BIT_0;
			else
				reg_q_idac	=  q_reg_idac_ref | BIT_0;
			
			//SCI_TRACE_LOW("offset is 0x%04x\n",reg_q_idac);

			/* write idac reg */
			trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
		
			trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, (uint32)(((uint32)(reg_q_idac) << 2) | (reg_idac_tmp & 0xfffffe03)));/*lint !e516*/		//write 9-15bit
			REG32(FM_REG_CALI_CTL) |= BIT_0;
			REG32(FM_REG_CALI_CTL) |= BIT_8;
		
			while ((REG32(FM_REG_RC_DC_CALI) & BIT_24) != BIT_24)
			{
				FM_delay100();
			}

			q_amg_reg_tmp = (REG32(FM_REG_RC_DC_CALI) & 0xFFF000) >> 12;
			SCI_TraceLow("FM_IDDC w:%d dc_cali 0x%xdc is 0x%x\r\n",w,reg_q_idac, q_amg_reg_tmp);
			//SCI_TRACE_LOW("dc is 0x%04x\n",q_amg_reg_tmp);

			REG32(FM_REG_CALI_CTL) &= ~BIT_8;
			REG32(FM_REG_CALI_CTL) &= ~BIT_0;

			flag = (uint8)rx_dcoc_get_MaxAbsValue( q_amg_reg_tmp,  min_dc_mv, 0x0800 );
			if (flag == 0)
			{
				min_offset = reg_q_idac;
				min_dc_mv = q_amg_reg_tmp;
			}
			
			
			
			q_amg_reg		= q_amg_reg_tmp;
			q_reg_idac_ref		= reg_q_idac;

			r = 0;
			w--;
				
		}

		/* save the adjust code */
		//SDI_VTRACEID(FM, min_offset_is_0x_04x_n, "%04x", "%04x", "min offset is 0x%04x\n", 1, (min_offset));
		//SDI_VTRACEID(FM, min_dc_is_0x_04x_n, "%04x", "%04x", "min dc is 0x%04x\n", 1, (min_dc_mv));
		SCI_TraceLow("FM_IDDC min_dc is 0x%x\r\n",min_dc_mv);
		/* save the adjust code */
		Set_Code_pga_Q_Value(uhwLoopI, min_offset);/*lint !e63*/
		SCI_TraceLow("FM_IDDC gainLv:%d, set  0x%x\r\n",uhwLoopI,min_offset);
	}

	//rx_dcoc_write_i_c
}

uint32 pga_dcoc_cfg_val[12] = {0,};
//extern uint32 bits_table[FM_AGC_TBL_NUM];
//extern uint32 tia_dcoc[FM_AGC_TBL_NUM];
//extern uint32 pga_dcoc[FM_AGC_TBL_NUM_EVB];
void pga_dcoc_cfg()
{
	uint32 i,pga_dcoc_i,pga_dcoc_q,pga_val;

	for (i = 0; i < RX_DCOC_IDAC2_LEVER_NUM; i++)
	{
		pga_dcoc_i = (uint32)(g_rx_pga_dcoc_adjust_i_code[i] & 0x7F);/*lint !e737*/
		pga_dcoc_q = (uint32)(g_rx_pga_dcoc_adjust_q_code[i] & 0x7F);/*lint !e737*/
		//g_rx_pga_dcoc_adjust[i] =  pga_dcoc_i + (pga_dcoc_q << 7);
		g_rx_pga_dcoc_adjust[i] =  pga_dcoc_q + (pga_dcoc_i << 7);
	}

	pga_dcoc_cfg_val[0] = g_rx_pga_dcoc_adjust[0];
	pga_dcoc_cfg_val[1] = g_rx_pga_dcoc_adjust[0];
	pga_dcoc_cfg_val[2] = g_rx_pga_dcoc_adjust[0];

	pga_dcoc_cfg_val[3] = g_rx_pga_dcoc_adjust[1];
	pga_dcoc_cfg_val[4] = g_rx_pga_dcoc_adjust[1];

	pga_dcoc_cfg_val[5] = g_rx_pga_dcoc_adjust[2];
	pga_dcoc_cfg_val[6] = g_rx_pga_dcoc_adjust[2];

	pga_dcoc_cfg_val[7] = g_rx_pga_dcoc_adjust[3];
	pga_dcoc_cfg_val[8] = g_rx_pga_dcoc_adjust[3];

	pga_dcoc_cfg_val[9] = g_rx_pga_dcoc_adjust[4];
	pga_dcoc_cfg_val[10] = g_rx_pga_dcoc_adjust[4];
	pga_dcoc_cfg_val[11] = g_rx_pga_dcoc_adjust[4];
	
	for (i = 0; i < FM_AGC_TBL_NUM_EVB; i++)
	{
		pga_val = bits_table[i] & 0xF;
		pga_dcoc[i] = pga_dcoc_cfg_val[pga_val];
	}

       for(i=0;i<(FM_AGC_TBL_NUM_EVB-1);i++)
     	{
     		trout_fm_write_rf_reg((S0_FM_RC_DCOC_CAL+i), (pga_dcoc_cfg_val[i]<<2));
     	}
	   
	
}

void DCOC_calibration()
{
	uint32 	data_deb;
	uint32	reg_idac_tmp;
	SCI_TraceLow("DCOC cali begin!\r\n");
	DCOC_cali_init();

	//DCOC_test();
	// tia
	//rx_dcoc_i_idac1_process();
	//rx_dcoc_q_idac1_process();
	// pga
	rx_dcoc_i_idac2_process();
	
	trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
	data_deb = (uint32)(reg_idac_tmp & 0xffff01ff);
	trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, data_deb);/*lint !e516*/
	
	rx_dcoc_q_idac2_process();

	trout_read_rf_reg(REG_FM_RC_DCOC_CAL, &reg_idac_tmp);	
	data_deb = (uint32)(reg_idac_tmp & 0xffff01ff);
	trout_fm_write_rf_reg(REG_FM_RC_DCOC_CAL, data_deb);/*lint !e516*/
	/*lint +e516*/
	pga_dcoc_cfg();
	//SCI_TRACE_LOW("DCOC cali is done!\n");

	DCOC_cali_exit();
	SCI_TraceLow("DCOC cali end!\r\n");
}
#endif

//##################################################################################################
//#####################################      END OF MAIN      ######################################
//##################################################################################################



#ifdef IQ_CALI_INCLUDE

#define FM_CALIBRATION_INVALID_VAL_UINT32			(0xffffffff)

void fm_calibration_GetMaxSignDigit( uint32 uwMaxSignVal,  uint32 *uwRootVal, uint32 *uwRemainVal)
{
	if(( uwMaxSignVal >= 100 ) || ( 0 == uwMaxSignVal ))
	{
		*uwRootVal				= FM_CALIBRATION_INVALID_VAL_UINT32;
		*uwRemainVal			= FM_CALIBRATION_INVALID_VAL_UINT32;
	}
	else if( uwMaxSignVal < 4 )
	{
		*uwRootVal				= 1;
		*uwRemainVal			= uwMaxSignVal - 1;
	}
	else if( uwMaxSignVal < 9 )
	{
		*uwRootVal				= 2;
		*uwRemainVal			= uwMaxSignVal - 4;
	}
	else if( uwMaxSignVal < 16 )
	{
		*uwRootVal				= 3;
		*uwRemainVal			= uwMaxSignVal - 9;
	}
	else if( uwMaxSignVal < 25 )
	{
		*uwRootVal				= 4;
		*uwRemainVal			= uwMaxSignVal - 16;
	}
	else if( uwMaxSignVal < 36 )
	{
		*uwRootVal				= 5;
		*uwRemainVal			= uwMaxSignVal - 25;
	}
	else if( uwMaxSignVal < 49 )
	{
		*uwRootVal				= 6;
		*uwRemainVal			= uwMaxSignVal - 36;
	}
	else if( uwMaxSignVal < 64 )
	{
		*uwRootVal				= 7;
		*uwRemainVal			= uwMaxSignVal - 49;
	}
	else if( uwMaxSignVal < 81 )
	{
		*uwRootVal				= 8;
		*uwRemainVal			= uwMaxSignVal - 64;
	}
	else
	{
		*uwRootVal				= 9;
		*uwRemainVal			= uwMaxSignVal - 81;
	}
	
	return;
}

void fm_calibration_GetSubSignDigit( uint32 uwTargetVal,  uint32 uwInputCalVal, uint32 *uwQuotient, uint32 *uwRemainVal)
{
	uint32	uwTmpCalVal;
	uint32	uwTryCnt;
	uint32	uwTryCalVal;
	
	uwTryCnt					= 0;
	uwTryCalVal					= 1;
	uwTmpCalVal					= uwInputCalVal * 20 + uwTryCalVal;

	while( uwTmpCalVal <= uwTargetVal )
	{
		uwTryCnt++;
		uwTryCalVal++;
		uwTmpCalVal				= uwTryCalVal * ( uwInputCalVal * 20 + uwTryCalVal);
		
	}

	*uwQuotient					= uwTryCnt;
	*uwRemainVal				= uwTargetVal - (uwTryCnt * ( uwInputCalVal * 20 + uwTryCnt));

	return;
}

#define FM_CALIBRATION_STEP_NUM					(32)

//rx_calibration_GetSubSignDigit,rx_calibration_GetMaxSignDigit,rx_calibration_Sqrt
//is used for sqrt in iq cali,replace Sqrt_fix_ARM,because Sqrt_fix_ARM take so
//much long time
uint32 fm_calibration_Sqrt( uint32 uwOriVal )
{
	uint32	uwOriValTmp;
	uint32	uwRootVal;
	uint32	uwRemainVal;
	uint32	uwDividend;
	uint32 	uwQuotient;
	uint32	auwStepVal[FM_CALIBRATION_STEP_NUM];
	uint32	uhwStepCnt;
	int16	shwLoopI;

	uwRootVal			=0;
	uhwStepCnt			= 0;
	uwOriValTmp			= uwOriVal;
	SCI_MEMSET(auwStepVal,0,sizeof(auwStepVal));

	
	while( uwOriValTmp > 0 )
	{
		auwStepVal[uhwStepCnt]		= uwOriValTmp % 100;
		uwOriValTmp					=  uwOriValTmp / 100;
		uhwStepCnt++;
	}

	if(uhwStepCnt<2) // converity warning, protect read over bound
	{
		return uwRootVal;
	}
	
	fm_calibration_GetMaxSignDigit( auwStepVal[uhwStepCnt - 1], &uwRootVal, &uwRemainVal );

	for( shwLoopI = (uhwStepCnt - 2); shwLoopI >= 0; shwLoopI-- )
	{
		uwDividend					= 100 * uwRemainVal + auwStepVal[shwLoopI];
		fm_calibration_GetSubSignDigit(uwDividend, uwRootVal, &uwQuotient, &uwRemainVal);
		uwRootVal					= uwRootVal*10 + uwQuotient;
	}

	return uwRootVal;
}
//##################################################################################################
void Scaling_Data_1_round(int *Data, int Bit_Shift, int Bit_Limit)
{
// Data scaling subroutine.

	int		Max,Min;

	//Max =  (int)pow( 2.0,(double)(Bit_Limit-1) ) - 1;
	//Min = -(int)pow( 2.0,(double)(Bit_Limit-1) );
	Max =  (1<<(Bit_Limit-1)) - 1;
	Min = -(1<<(Bit_Limit-1));

	if(*Data < 0)
		*Data = *Data -1;
	
	if (Bit_Shift > 0)
	{
		(*Data) += (1 << (Bit_Shift-1));  //modified in 2014.1.26
		(*Data) = (*Data) >> Bit_Shift;
	}	
	else if (Bit_Shift < 0)
	{
		(*Data) += (1 << (Bit_Shift-1));//modified in 2014.1.26
		(*Data) = (*Data) << (-Bit_Shift);/*lint !e504*/ 
	}

	if ( (*Data) > Max )
		(*Data) = Max;
	else if ( (*Data) < Min )
		(*Data) = Min;

}
//##################################################################################################
void DSP_div_quan_ARM_1( int divisor_in, int dividend_in, int quan_width, int * Out)
{
	// calculate Out = (2^quan_width -1)  * dividend/divisor;
	int divisor,dividend;
	int ii, div_alu, a;

	if (divisor_in < 0)
		divisor = (-1)*divisor_in;
	else 
		divisor = divisor_in;
	if (dividend_in < 0)
		dividend = (-1)*dividend_in;
	else
		dividend = dividend_in;
	
	//temp = dividend * ((1 << quan_width) -1);
	dividend = dividend * ((1 << quan_width) -1);
	for (ii =0; ii<= quan_width; ii++)
	{
		div_alu = dividend - (divisor << quan_width);
		if (div_alu < 0)
		{
			dividend *= 2;
		}
		else
		{
			dividend = div_alu *2 + 1;
		}
	}
	////a =(int)  floor(dividend/pow(2.0, (double) (quan_width+1)) + 0.5);
	a = dividend;
	Scaling_Data_1_round(&a,(quan_width+1),16); 
	if (((divisor_in >= 0)&&(dividend_in >= 0))||((divisor_in < 0)&&(dividend_in < 0)))
		*Out = dividend - (a<<(quan_width +1));
	else
		*Out = (a<<(quan_width +1)) - dividend ;
}

void fm_iq_cali_sqrt(int in , int * out)
{
	//int n, temp,x_p,x = 32;
	uint32 temp;
	if ( in < 0)
	{
		//printf("error in sqrt_fix, input is nagative");
		return;
	}

	temp = (uint32)in;

	*out = (int)fm_calibration_Sqrt(temp);
}

int 		c_save_Glb = 0,d_save_Glb = 0; 
//##################################################################################################
//##################################################################################################
void IQImblance_Coef_Statistic_ARM_1(int K1I_save, int K1Q_save,int K2I_save, int K2Q_save)		//modified 2013-06-16 Xia Li
{
	int G_ref_est_save, alpha_save, sin_phi_save, cos_phi_save, temp1,temp2,c_save,d_save;
	int K12I,K12Q,Quant;
	temp1 = 0;
	temp2 = 0;

	 G_ref_est_save = (K1I_save+K2I_save)*(K1I_save+K2I_save) + (K1Q_save - K2Q_save)*(K1Q_save - K2Q_save); //(abs(K1+conj(K2)))^2
	 Scaling_Data_1_round(&G_ref_est_save,IQquan_width,16);
	 K12I = K1I_save*K2I_save - K1Q_save*K2Q_save;
	 Scaling_Data_1_round(&K12I,IQquan_width,16);
	 K12Q = K1I_save*K2Q_save + K2I_save*K1Q_save;
	 Scaling_Data_1_round(&K12Q,IQquan_width,16);
	 temp1 = 4 * K12I;
	 Quant = 1 << IQquan_width;
	 
	 DSP_div_quan_ARM_1(G_ref_est_save,temp1,IQquan_width,&temp2); //temp1 may<0
	 
	 //temp2=temp1*Quant/G_ref_est_save; //temp1 may<0
	 temp1 = Quant*Quant - Quant * temp2;
	 
	 //Sqrt_fix_ARM(temp1,&alpha_save);
	 fm_iq_cali_sqrt(temp1,&alpha_save);
	 
	 
	 temp1 = G_ref_est_save * alpha_save;
	 Scaling_Data_1_round(&temp1,IQquan_width,16);
	 temp2 = 2 * K12Q;
	 DSP_div_quan_ARM_1(temp1,temp2,IQquan_width,&sin_phi_save);
	 //sin_phi_save = temp2 * Quant / temp1;
	 temp1 = Quant * Quant - sin_phi_save * sin_phi_save;
	 //Sqrt_fix_ARM(temp1,&cos_phi_save);
	 fm_iq_cali_sqrt(temp1,&cos_phi_save);
	 DSP_div_quan_ARM_1(cos_phi_save, sin_phi_save, IQquan_width, &c_save);
	 //c_save = sin_phi_save * Quant / cos_phi_save;
	 temp1 = alpha_save * cos_phi_save;
	 Scaling_Data_1_round(&temp1,IQquan_width,16);
	 if (Quant > temp1)
	 {
		temp2 = Quant>>1;
	 	DSP_div_quan_ARM_1(temp1,temp2,IQquan_width,&d_save); 
		d_save = d_save*2;
	 }
	 else
		 DSP_div_quan_ARM_1(temp1,Quant,IQquan_width,&d_save);  //Quant may >  temp1;
	 //d_save = Quant*Quant/temp1;
	 
	c_save_Glb = c_save;
	d_save_Glb = d_save;	
}

void IQ_config_c_d_factor()
{
	REG32(FM_REG_IQ_CALI) &= 0xC0000000;
	REG32(FM_REG_IQ_CALI) |= (((~(c_save_Glb & 0x7FFF)+1)&0x7FFF) | ((d_save_Glb & 0x7FFF) << 15));
	//REG32(FM_REG_IQ_CALI) |= (c_save_Glb & 0x7FFF) | ((d_save_Glb & 0x7FFF) << 15);
	
	REG32(FM_REG_IQ_CALI) |= (BIT_30 |BIT_31);
}

uint32 FM_SX_AFC_START_Reg_Bake = 0;
uint32 FM_RX_GAIN_CTRL0_Reg_Bake = 0;
uint32 FM_MODE_Reg_Bake = 0;
uint32 FM_REG_BAND_SPACE_Bake = 0;

int trout_fm_iq_cal_init(void)
{
    uint32 reg_data,i,data;
	   
    trout_fm_reg_cfg();
	data = REG32(FM_REG_BAND_SPACE);
	FM_REG_BAND_SPACE_Bake = data;
	data &= 0xFFFFFF;
	REG32(FM_REG_BAND_SPACE) = ((100 << 24) | data);
	
	trout_read_rf_reg(FM_SX_AFC_START, &data);
	FM_SX_AFC_START_Reg_Bake = data;
	data |= 0x8000;
	trout_fm_write_rf_reg(FM_SX_AFC_START, data);

	// fm enter debug mode
	trout_read_rf_reg(FM_MODE, &data);
	FM_MODE_Reg_Bake = data;
	data &= ~0xF000;
	data |= 0x8000;
	trout_fm_write_rf_reg(FM_MODE, data);
	return(0);
}
int trout_fm_iq_cal_exit(void)
{
	uint32 data = 0;

	REG32(FM_REG_BAND_SPACE) = FM_REG_BAND_SPACE_Bake;	
		// agc enable
	data = REG32(FM_REG_FM_EN);
	data |= BIT_3;
	REG32(FM_REG_FM_EN) = data;

	trout_write_reg(FM_REG_FM_CTL_STI, FM_CTL_STI_MODE_NORMAL);	//normal mode
	REG32(FM_REG_CHAN) = (128 << 20) | 98000;

    
	// fm mode reg recover
	trout_fm_write_rf_reg(FM_RX_GAIN_CTRL0, FM_RX_GAIN_CTRL0_Reg_Bake);
	trout_fm_write_rf_reg(FM_SX_AFC_START, FM_SX_AFC_START_Reg_Bake);
	trout_fm_write_rf_reg(FM_MODE, FM_MODE_Reg_Bake);
	return 0;
}

void IQ_imbalance_calibration(void)
{
	int ik1,ik2,qk1,qk2;
	uint32 i,data;
	uint32 channel,flag;
	uint32 fm_int_state = 0;
	    uint32 reg_data;
	//fm modem or rf init
    SCI_TraceLow("FM_IQ cali begin!\r\n");
    trout_fm_iq_cal_init();
  	//fm enable
    //configure 80.2Mhz rx
    trout_fm_dis();
    //REG32(FM_REG_CHAN) = (100 << 20) | 80200;//marlin2
    REG32(FM_REG_CHAN) = (100 << 20) | 83200;//nemog
    REG32(FM_REG_CIC16_FREQ) = 0x7CCCCC;
    trout_write_reg(FM_REG_FM_CTL_STI, FM_CTL_STI_MODE_TUNE);	//1 tune mode
    trout_fm_int_en();
    trout_fm_en();
	 SCI_TraceLow("FM_IQ cali 1!\r\n");
	 //FM_reg_dump();
	do
	{
	   fm_int_state =  REG32(FM_REG_INT_STS);
	   for(i=0;i<100;i++)
	   {
	   }
	   
	}while((fm_int_state&0x1) == 0x0);

	
	SCI_TraceLow("FM_IQ cali 2!\r\n");

	// hardware auto agc disable
	data = REG32(FM_REG_FM_EN);
	data &= ~BIT_3;
	REG32(FM_REG_FM_EN) = data;
	// set fix gain
	trout_read_rf_reg(FM_RX_GAIN_CTRL0, &data);
	FM_RX_GAIN_CTRL0_Reg_Bake = data;
	//trout_fm_write_rf_reg(FM_RX_GAIN_CTRL0, 0xEE00/*0xE600*/);
	trout_fm_write_rf_reg(FM_RX_GAIN_CTRL0, 0xF000/*0xE600*/);
	
	//for (i = 0; i < 10000000; i++);
	for (i = 0; i < 1000; i++)
	{
	}


    //start IQ calibration
	REG32(FM_REG_IQ_CALI)  &= ~(BIT_31 | BIT_30);
	for(i=1000;i>0;i--)
	{
	}
	//trout_test_data_capture();
	REG32(FM_REG_IQ_CALI) |= BIT_31;

	//IQ_imbalance_cali_ctl();
	while ((REG32(FM_REG_SMUTE_STS) & BIT_16) != BIT_16)
	{
		for(i=0;i<100;i++)
		{
		}
	}
	SCI_TraceLow("FM_IQ cali 3!\r\n");	
		
	ik1 = (int)(REG32(FM_REG_K1_VALUE) & 0xFFFF);
	qk1 = (int)((REG32(FM_REG_K1_VALUE) & 0xFFFF0000) >> 16);
	ik2 = (int)(REG32(FM_REG_K2_VALUE) & 0xFFFF);
	qk2 = (int)((REG32(FM_REG_K2_VALUE) & 0xFFFF0000) >> 16);
	
 	REG32(FM_REG_IQ_CALI) &= ~BIT_31;

	//SCI_TRACE_LOW("ik1:0x%04x, qk1:0x%04x, ik2:0x%04x, qk2:0x%04x\n",ik1,qk1,ik2,qk2);
	SCI_TraceLow("ik1:0x%04x, qk1:0x%04x, ik2:0x%04x, qk2:0x%04x\n",ik1,qk1,ik2,qk2);

	if (ik1 >= 32768)
	{
		ik1 -= 65536;
	}
	if (qk1 >= 32768)
	{
		qk1 -= 65536;
	}
	if (ik2 >= 32768)
	{
		ik2 -= 65536;
	}
	if (qk2 >= 32768)
	{
		qk2 -= 65536;
	}		
	
	IQImblance_Coef_Statistic_ARM_1(ik1, qk1, ik2, qk2);
	
	//IQ_config_c_d_factor();

    trout_fm_dis();
    trout_fm_int_clr();
    trout_fm_iq_cal_exit();
    SCI_TraceLow("FM_IQ cali end!\r\n");
		
}

#endif

void fm_rf_por_init()
{
#if 0
	//common reg
	//trout_fm_write_rf_reg(REG_CAL_R_CTRL, 0x3000);
	//trout_fm_write_rf_reg(LDO_ADDABBVCO_CTRL0, 0xF000);
	//fm only
#ifdef 0//SPRD_WIFI_FW_SUPPORT	
	if(1 ==(g_config_data.public_config.public_reserved[0]&0xF))//sc2341
	{
		trout_fm_write_rf_reg(FM_SX_PK_CTRL0, 0x9438);
	}
	else if(0 ==(g_config_data.public_config.public_reserved[0]&0xF))//sc2331
	{
		trout_fm_write_rf_reg(FM_SX_PK_CTRL0, 0x9838);
	}	
#endif	
	trout_fm_write_rf_reg(FM_ADC_CTRL0, 0x0800);/*lint !e516*/
	trout_fm_write_rf_reg(FM_ADC_CTRL2, 0x02C0);/*lint !e516*/
	//trout_fm_write_rf_reg(FM_SX_PK_CTRL0, 0x9838);
	//for BGA 2341B
	//trout_fm_write_rf_reg(FM_SX_PK_CTRL0, 0x9438);
	trout_fm_write_rf_reg(FM_SX_VCO_CTRL0, 0xFE00);/*lint !e516*/
	trout_fm_write_rf_reg(FM_SX_CP_CTRL0, 0x0B00);/*lint !e516*/
	trout_fm_write_rf_reg(FM_SX_DSMPFD_CTRL0, 0x3C84);/*lint !e516*/	//added by mtf for FM sensitivy
	trout_fm_write_rf_reg(FM_RX_RF_CTRL3, 0xEA00);/*lint !e516*///added by mtf for FM wifi coxe
#endif
}


/********************************************************************************
 *
 *
 *
 *******************************************************************************/

int32 trout_init_cali() /*lint !e129*/
{	/*lint !e745*/
	uint32 i,data;

	//fm enable
	REG32(REG_AON_APB_EB0) |= BIT_29;
	
	// control adc
	//REG32(0x40060258) |= BIT_0;
	//REG32(0x4006020C) |= BIT_16 | BIT_15;
       REG32(REG_APB_FM_RF1) |= BIT_10;  //adc enable
	REG32(REG_APB_ANALOG_BB_TOP3) |= BIT_21;  //64MHz enable
	
         //*(volatile uint32*)(0X8200146c)=0;//o:open ldo bt_pa, 1:close ldo
         //*(volatile uint32*)(0X82001164)=0x100;//enable a die 3.3v ///mux pin out
         *(volatile uint32*)(0X8b0002c8)|=1<<2;//select  fm_pdn_sel  controlled by  PWR_CTRL bit2, 0 powedown, 1,powerup

	//  *(volatile uint32*)(0X8b000330)=0X6175303;//add ldo vdd for 64M, ctr'd by platform
	return 0;
}

/********************************************************************************
 *
 *
 *
 *******************************************************************************/
void fm_cali_exit()
{
	uint32 data;
	// 1. marlin power on

	trout_read_rf_reg(FM_MODE, &data);
	data &= ~0xF000;
	data |= 0x0000;
	trout_fm_write_rf_reg(FM_MODE, data);

	//fm disable
	REG32(REG_AON_APB_EB0) &= (~BIT_29);
	
	// disable adc
	//REG32(0x40060258) &= ~BIT_0;
	//REG32(0x4006020C) &= ~(BIT_16 | BIT_15);
	REG32(REG_APB_FM_RF1) &= (~BIT_10);  //fm adc disable
	REG32(REG_APB_ANALOG_BB_TOP3) &= (~BIT_21);  //64MHz disable
}

void fm_bt_rf_R_cali()
{
	uint32 cali_data[10] = {0,};
	uint32 i;
	if(SCI_FALSE==EFUSE_HAL_GetBTCaliData (&cali_data[0]))
	{
		SCI_TraceLow("FM_effuse access fail\r\n");
	}
	else
	{
		SCI_TraceLow("FM_effuse access OK\r\n");
		//trout_write_reg(some_addr_of_v18, cali_data[addr]);
	}

	g_efuse_fm[0]=cali_data[0]; // to keep fm rf calibration code size
	
	for(i=0;i<10;i++)
	{
		SCI_TraceLow("FM_effuse 0x%x\r\n",cali_data[i]);
	}

}



/********************************************************************************
 *
 *
 *
 *******************************************************************************/
PUBLIC void fm_calibration_main()
{
#if 1
	  trout_init_cali();
	  fm_por_init();
	  SCI_TraceLow("fm_cali_run=%x\r\n",fm_cali_run_flag);
	  RC_calibration();
	//	FM_reg_dump();
	 DCOC_calibration();
	//FM_reg_dump();
	 IQ_imbalance_calibration();
	 fm_cali_exit();

	fm_cali_run_flag=1;


#endif
	SCI_TraceLow("fm_calibration_main\r\n");

}

PUBLIC void fm_calibration_main_keep()
{

	//fm_bt_rf_v18_cali();
	fm_bt_rf_R_cali();
	
#if ENABLE_CALIBRATION
	if(9123450==g_efuse_fm[0])  // to keep fm rf calibration code size
	{
		trout_init_cali();
		fm_por_init();

		RC_calibration();

		DCOC_calibration();

		IQ_imbalance_calibration();

		fm_cali_exit();
	}
	
#endif	

}

PUBLIC void fm_calibration_main_test()
{

	//fm_bt_rf_v18_cali();
	//fm_bt_rf_R_cali();
	#if 0
	REG32(REG_AHB_RST0)  &= (~BIT_18);  //fm sofr rst 
	 FM_delay1000();
        REG32(REG_APB_EB) |= (BIT_APB_FM_EB | BIT_APB_UART1_EB | BIT_APB_UART0_EB );
        REG32(REG_APB_EB1) |= (BIT_APB_SYST_EB|BIT_APB_SYST_RTC_EB);

        FM_delay1000();
	REG32(REG_AON_APB_EB0) |= BIT_29;
	FM_delay1000();
	REG32(REG_AON_APB_RST0) |= BIT_23;
	FM_delay1000();
	REG32(REG_AON_APB_RST0) &= ~BIT_23;


	fm_por_init();
	
       REG32(REG_APB_FM_RF1) |= BIT_10;  //adc enable
	REG32(REG_APB_ANALOG_BB_TOP3) |= BIT_21;  //64MHz enable


         //*(volatile uint32*)(0X8200146c)=0;//o:open ldo bt_pa, 1:close ldo
         //*(volatile uint32*)(0X82001164)=0x100;//enable a die 3.3v ////mux pin out
         *(volatile uint32*)(0X8b0002c8)|=1<<2;//select  fm_pdn_sel  controlled by  PWR_CTRL bit2, 0 powedown, 1,powerup

	#endif
#if 1
	_CHIPDRV_DisableIRQINT(FM_INT_NUM);
	ISR_UnRegHandler(FM_INT_NUM);
#endif
		trout_init_cali();
		fm_por_init();

	 	RC_calibration();
	//	FM_reg_dump();
		SCI_TraceLow("fm_cali_run=%x\r\n",fm_cali_run_flag);
	 	 DCOC_calibration();
		//FM_reg_dump();
	 	IQ_imbalance_calibration();

		fm_cali_exit();
#if 1
	_CHIPDRV_DisableIRQINT(FM_INT_NUM);
	ISR_UnRegHandler(FM_INT_NUM);
#endif
fm_cali_run_flag=1;
}
#endif

 /*******************************************************************************
 *For AGC calibration FAIL in modem platform
 *
 *
 *******************************************************************************/
void FM_Set_VDDRF_IdleMode(void)
{
    uint32 temp_data = 0x0000;

    temp_data = 0x0000;
    trout_fm_write_rf_reg(FM_MODE, temp_data);//0x8D00_0000 0x0044_0000

    temp_data = 0x1000;
    trout_fm_write_rf_reg(FM_MODE, temp_data);
}
#endif //FM_SUPPORT_SPRD_V0

#else //MODEM_PLATFORM


void FM_delay1000()
{
    uint32 i;
    for (i = 0; i < 1000; i++) ;
}

BOOLEAN trout_write_reg(uint32 reg_addr, uint32 reg_data)
{
    CHIP_REG_SET(reg_addr, reg_data);
    return(TRUE);
}

int32 trout_read_reg(uint32 reg_addr, uint32 *reg_data)
{
    //uint32 i;
    
    if (reg_data == NULL)
    {
        return -1;
    }
    
    *reg_data = CHIP_REG_GET(reg_addr);
    
    //for (i = 0; i < 10000; i++) ;
    FM_delay1000();
    
    return 0;
}

BOOLEAN trout_write_rf_reg(uint32 addr, uint32 data)
{
    uint32  reg_data=0;
    BOOLEAN ret=FALSE;
    //volatile SPI_PHY_CTL *phy_ctl = (SPI_PHY_CTL *)CTL_BASE_RF;

    reg_data = ((addr&0x7fff)<<16)|(data&0xffff);

    trout_write_reg(CTL_BASE_RF, reg_data);
    //for (i = 0; i < 10000; i++) ;
    FM_delay1000();
    //spi_write_waitforidle();
    ret = TRUE;

    return ret;
}

BOOLEAN trout_read_rf_reg(uint32 addr, uint32 *data)
{
    uint32 reg_data=0;
    BOOLEAN ret=FALSE;
    //trout_adi_rd_int_clr();
    //volatile SPI_PHY_CTL *phy_ctl = (SPI_PHY_CTL *)CTL_BASE_RF;
    reg_data = ((addr&0x7fff)<<16)|BIT_31;
    
    trout_write_reg(CTL_BASE_RF, reg_data);
    //for (i = 0; i < 10000; i++) ;
    FM_delay1000();
    //spi_write_waitforidle();
    trout_read_reg(CTL_BASE_RF, &reg_data);
    //for (i = 0; i < 10000; i++) ;
    FM_delay1000();
    //spi_read_waitforidle();
    //trout_read_reg(RF_SPI_REG_START_ADDR+(addr&0xfff), &reg_data);
    //for (i = 0; i < 100; i++) ;
    *data = reg_data & 0xffff;
    ret =  TRUE;
    
    return ret;
}

LOCAL BOOLEAN trout_fm_write_rf_reg(uint32 addr, uint32 reg_data)
{
    uint32 data;
    if (trout_write_rf_reg(addr, reg_data) == FALSE)
    //if (trout_write_rf_reg(addr, reg_data) == 0)
    {
        SCI_TraceLow("trout write fm rf reg error!\n");
        return FALSE;
    }
    // read it after write
    if (trout_read_rf_reg(addr, &data) == FALSE)
    {
        //SCI_TraceLow("trout read fm rf reg error!\n");
        return FALSE;
    }
    // check if the reg is wrote successfully
    if (data != reg_data)
    {
        SCI_TraceLow("rf [%04x] write:%04x, read:%04x  @@@ error @@@\n", addr, reg_data, data);
        return FALSE;
    }
    else
    {
        //SCI_TraceLow("rf [%04x] write:%04x, read:%04x\n", addr, reg_data, data);
        return TRUE;
    }
}

 /*******************************************************************************
 *For AGC calibration FAIL in modem platform
 *
 *
 *******************************************************************************/
void FM_Set_VDDRF_IdleMode(void)
{
    uint32 temp_data = 0x0000;

    temp_data = 0x0000;
    trout_fm_write_rf_reg(FM_MODE, temp_data);//0x8D00_0000 0x0044_0000

    temp_data = 0x1000;
    trout_fm_write_rf_reg(FM_MODE, temp_data);
}
#endif

