/******************************************************************************
 ** Copyright (c) 
 ** File Name:		atv_rda5888.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor rda5888. 
 ** 														 
 ******************************************************************************

 ******************************************************************************
 ** 					   Edit History 									  *
 ** ------------------------------------------------------------------------- *
 ** DATE		   NAME 			DESCRIPTION 							  *
 ** 	  
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 ** 						Dependencies									  *
 **---------------------------------------------------------------------------*/
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "gpio_prod_api.h"
#include "ldo_drv.h"
#include "deep_sleep.h"
#include "rda5888drv.h"
#include "rda5888app.h"
#include "rda5888freqtbl.h"
#include "rda5888sprd.h"

/**---------------------------------------------------------------------------*
 ** 						Compiler Flag									  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" 
{
#endif
/*lint -save -e64 -e785*/
/**---------------------------------------------------------------------------*
 ** 					Extern Function Declaration 						  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 						Const variables 								  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 						   Macro Define
 **---------------------------------------------------------------------------*/


 /**---------------------------------------------------------------------------*
 **                         Struct Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 _rda5888_PowerOnSequence(uint32 power_on);
LOCAL uint32 _rda5888_Identify(uint32 param);
LOCAL uint32 _rda5888_Init(uint32 param);
LOCAL uint32 _rda5888_IOCTL(uint32 ctl_param);
LOCAL uint32 _rda5888_GetResolutionTrimTab(uint32 param);
LOCAL uint32 _rda5888_SetVolume(uint32 param);

/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/
LOCAL ATV_INFO_T s_atv_info={0x00};
LOCAL ATV_INFO_T_PTR s_atv_info_ptr=&s_atv_info;
    
LOCAL uint16 s_emc_param[2]={0xf140, 0xf0f0};

LOCAL SENSOR_REG_TAB_INFO_T s_rda5888_resolution_Tab_YUV[]=
{	
	// COMMON INIT
	{ PNULL, 0, 0, 0, 24, SENSOR_IMAGE_FORMAT_YUV422},
	
	// YUV422 PREVIEW 1
	//{ PNULL, 0, 720, 288, 24, SENSOR_IMAGE_FORMAT_YUV422},
	{ PNULL, 0, 320, 240, 24, SENSOR_IMAGE_FORMAT_YUV422},
	{ PNULL, 0, 0, 0, 0, 0},
	
	{ PNULL, 0, 0, 0, 0, 0},
	{ PNULL, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{ PNULL, 0, 0, 0, 0, 0},
	{ PNULL, 0, 0, 0, 0, 0},
	{ PNULL, 0, 0, 0, 0, 0},
	{ PNULL, 0, 0, 0, 0, 0}
};

LOCAL SENSOR_TRIM_T s_rda5888_resolution_trim_tab[]=
{	
	// COMMON INIT
	{0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 1	
	//{24, 0, 680, 288,},
	{0, 0, 320, 240, 0},
	{0, 0, 0, 0, 0},
	
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0}
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_rda5888_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    _rda5888_PowerOnSequence,
    PNULL,
    _rda5888_Identify,
    PNULL,			// write register
    PNULL,			// read  register
    // Custom function
    _rda5888_IOCTL,
    _rda5888_GetResolutionTrimTab,

    // External
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL, 
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
};


/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_rda5888_yuv_info =
 {
    0xc4,               // salve i2c write address
    0xc5,               // salve i2c read address

    SENSOR_I2C_NOACK_BIT|\
    SENSOR_I2C_REG_8BIT|\
    SENSOR_I2C_VAL_16BIT,   // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit1: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                    // other bit: reseved
    SENSOR_HW_SIGNAL_PCLK_N|\
    SENSOR_HW_SIGNAL_VSYNC_N|\
    SENSOR_HW_SIGNAL_HSYNC_P,       // bit0: 0:negative; 1:positive -> polarily of pixel clock
                                    // bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
                                    // bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
                                    // other bit: reseved
    // environment mode
    0x00,
    // image effect
    0x00,
    // while balance mode
    0x00,
    0x00,                       // brightness/contrast/sharpness/saturation/EV
    SENSOR_LOW_PULSE_RESET,         // reset pulse level
    200,                                // reset pulse width(ms)
    SENSOR_LOW_LEVEL_PWDN,     // 1: high level valid; 0: low level valid

    0x00,                               // count of identify code
    0x00, 0x00,                     // supply two code to identify sensor.
    0x00, 0x00,                     // for Example: index = 0-> Device id, index = 1 -> version id

    SENSOR_AVDD_2800MV,             // voltage of avdd

    320,                            // max width of source image
    240,                            // max height of source image
    "rda5888",                      // name of sensor

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_UYVY,   // pattern of input image form sensor;

    s_rda5888_resolution_Tab_YUV,   // point to resolution table information structure
    &s_rda5888_ioctl_func_tab,      // point to ioctl function table

    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1800MV,                      // dvdd
    4,                     // skip frame num before preview
    3,                     // skip frame num before capture
    0,                     // deci frame num during preview
    2,                     // deci frame num during video preview
    1,                     // threadhold eb
    0,                     // threadhold mode
    0x4d0,//0x1a0,    //0x250,     // threadhold st
    0x5d0,//0x280,    //0x2c0,      // threadhold en
    0,
    0
};

/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description: rda5888 get infor
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL ATV_INFO_T_PTR _rda5888_GetInfo(void)
{    
	return s_atv_info_ptr;
}

/******************************************************************************/
// Description: get ov7670 rssolution trim tab
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_GetResolutionTrimTab(uint32 param)
{
    SCI_TRACE_LOW("harry, _rda5888_GetResolutionTrimTab calling");
    return (uint32)s_rda5888_resolution_trim_tab;
}

/******************************************************************************/
// Description: rda5888 init channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_InitChannel(void)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();

    SCI_TRACE_LOW("ATV: _rda5888_InitChannel");

    switch(atv_info_ptr->atv_cur_region)
    {
        case ATV_REGION_ARGENTINA:
        case ATV_REGION_PARAGURY:
        case ATV_REGION_URUGUAY:
        {
            RDAAPP_SetChannelMap(rdamtv_argentina_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_NC);
            break;
        }
		
        case ATV_REGION_BRAZIL:
        {
            RDAAPP_SetChannelMap(rdamtv_brazil_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_M);
            break;
        }
		
        case ATV_REGION_UK:
        case ATV_REGION_HONGKONG:
        case ATV_REGION_SOUTHAFRICA:
        {
            RDAAPP_SetChannelMap(rdamtv_a_hongkong_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_B);
            break;
        }
		
        case ATV_REGION_WESTERNEUROPE:
        case ATV_REGION_TURKEY:
        case ATV_REGION_UAE:
        case ATV_REGION_AFGHANISTAN:
        case ATV_REGION_SINGAPORE:
        case ATV_REGION_THAILAND:
        case ATV_REGION_CAMBODIA:
        case ATV_REGION_INDONESIA:
        case ATV_REGION_MALAYSIA:       
        case ATV_REGION_LAOS:
        case ATV_REGION_SPAIN:
        case ATV_REGION_PORTUGAL:
        case ATV_REGION_PAKISTAN:
        case ATV_REGION_INDIA:
        case ATV_REGION_AUSTRALIA:
    	 case ATV_REGION_EGYPT:
    	 case ATV_REGION_LIBYA:
        {
            RDAAPP_SetChannelMap(rdamtv_w_europe_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_B);
            break;
        }
		
        case ATV_REGION_VIETNAM:
        {
            RDAAPP_SetChannelMap(rdamtv_vietnam_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_D);
            break;
        }
		
        case ATV_REGION_CHINA:
        {
            RDAAPP_SetChannelMap(rdamtv_pald_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_D);
            break;
        }
		
        case ATV_REGION_SHENZHEN:
        {
            RDAAPP_SetChannelMap(rdamtv_pald_sz_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_I);
            break;
        }
		
        case ATV_REGION_JAPAN:
        {
            RDAAPP_SetChannelMap(rdamtv_ntsc_japan_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_NTSC_M);
            break;
        }
		
        case ATV_REGION_USA:
        case ATV_REGION_CANADA:
        case ATV_REGION_KOREA:
        case ATV_REGION_TAIWAN:
        case ATV_REGION_MEXICO:
        case ATV_REGION_CHILE:
        case ATV_REGION_VENEZUELA:
        case ATV_REGION_PHILIPPINES:
        {
            RDAAPP_SetChannelMap(rdamtv_ntsc_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_NTSC_M);
            break;
        }

        case ATV_REGION_GERMANY:
        case ATV_REGION_IRAQ:
        case ATV_REGION_IRAN:
        //case ATV_REGION_LIBYA:
        case ATV_REGION_LEBANON:
        case ATV_REGION_SANDIARABIA:
        case ATV_REGION_TUNISIA:
        {
            RDAAPP_SetChannelMap(rdamtv_secam_b_g_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_SECAM_B);
            break;
        }
		
        case ATV_REGION_BULGARIA:
        case ATV_REGION_CONGO:
        case ATV_REGION_HUNGRY:
        case ATV_REGION_POLAND:
        case ATV_REGION_ROMANIA:
        {
            RDAAPP_SetChannelMap(rdamtv_secam_d_k_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_SECAM_D);
            break;
        }
		
        case ATV_REGION_RUSSIA:
        {
            RDAAPP_SetChannelMap(rdamtv_secam_d_k_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_SECAM_D);
            break;
        }

	case ATV_REGION_MYANMAR:
        {
            RDAAPP_SetChannelMap(rdamtv_burma_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_NTSC_M);
            break;
        }
	
        default:
        {
            RDAAPP_SetChannelMap(rdamtv_pald_chn_map, RDAMTV_MODE_ANALOG_TV, RDAMTV_VSTD_PAL_D);
            break;
        }
    }

    return rtn;
}

/******************************************************************************/
// Description: sensor rda5888 power on/down sequence
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_PowerOnSequence(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val=g_rda5888_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val=g_rda5888_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val=g_rda5888_yuv_info.iovdd_val;  
    //BOOLEAN power_down=g_rda5888_yuv_info.power_down_level;	    
    BOOLEAN reset_level=g_rda5888_yuv_info.reset_pulse_level;
    uint32 reset_width=g_rda5888_yuv_info.reset_pulse_width;

    SCI_TRACE_LOW("ATV:_rda5888_PowerOnSequence power_on[%d]",power_on);
    
    if(SCI_TRUE==power_on)
    {
        SCI_TV_EnableDeepSleep(DISABLE_AHB_SLEEP);
        
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);

        SCI_Sleep(20);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel((BOOLEAN)!reset_level);
        SCI_Sleep(100);
    }
    else
    {        
        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED); 

        SCI_TV_EnableDeepSleep(ENABLE_AHB_SLEEP);
    }
    
    return SCI_SUCCESS;
}

PUBLIC uint32 _rda5888_PowerOnFM(uint32 power_on)
{
    _rda5888_PowerOnSequence(power_on);
    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: rda5888 identify
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_Identify(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;
    uint16 value=0x00;
    
    SCI_TRACE_LOW("ATV:_rda5888_Identify ");

    RDA5888_ReadReg(0x00, &value);
    
    if (0x5888 == value)
    {
        SCI_TRACE_LOW("ATV: the atv is RDA5888");
    }
    else
    {
        SCI_TRACE_LOW("ATV: the atv is not RDA5888, chipid: %x", value);
        rtn=SCI_ERROR; 
    }

#if 1 // for test i2c read/write.
	RDA5888_ReadReg(0x005, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test1, 0x005: %x", value);
	RDA5888_WriteReg(0x005, 0xa55a);
	RDA5888_ReadReg(0x005, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x005(0xa55a): %x", value);
	RDA5888_WriteReg(0x005, 0x1881);
	RDA5888_ReadReg(0x005, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x005(0x1881): %x", value);	
	RDA5888_WriteReg(0x005, 0x3663);
	RDA5888_ReadReg(0x005, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x005(0x3663): %x", value);	
	RDA5888_WriteReg(0x005, 0x0000);
	RDA5888_ReadReg(0x005, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x005(0x0000): %x", value);
	RDA5888_WriteReg(0x005, 0xffff);
	RDA5888_ReadReg(0x005, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x005(0xffff): %x", value);
	/*
	RDA5888_ReadReg(0x1b5, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x1b5(0xffff): %x", value);
	RDA5888_ReadReg(0x1b6, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x1b6(0xffff): %x", value);
	RDA5888_ReadReg(0x1b7, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x1b7(0xffff): %x", value);
	RDA5888_ReadReg(0x129, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x129(0xffff): %x", value);

    
	RDA5888_ReadReg(0x1b5, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x1b5(0xffff): %x", value);
	RDA5888_ReadReg(0x1b6, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x1b6(0xffff): %x", value);
	RDA5888_ReadReg(0x1b7, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x1b7(0xffff): %x", value);
	RDA5888_ReadReg(0x129, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test2, 0x129(0xffff): %x", value);
    */
	
	//while (1)
	//{
	//    RDA5888_WriteReg(0x005, 0x1234);
	//	SCI_TRACE_LOW("ATV: _rda5888_Identify2, test, chipid: %x", value);
	//	SCI_Sleep(100);
	//}
#endif

    return rtn;
}

LOCAL uint16 g_rda_reg1b5_value = 0x0009;
LOCAL uint16 g_rda_reg1b6_value = 0x092c;
LOCAL uint16 g_rda_reg1b7_value = 0x12bc;
/******************************************************************************/
// Description: rda5888 init
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_Init(uint32 param)
{
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();
    uint16 value=0x00;
    
    SCI_TRACE_LOW("ATV:_rda5888_Init param[%d]",param);
    _rda5888_Identify(0);
    
    atv_info_ptr->atv_cur_region=ATV_REGION_MAX;
    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;

    RDAAPP_Init();
    
	RDA5888_ReadReg(0x1b5, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x1b5(0xffff): %x", value);
	RDA5888_ReadReg(0x1b6, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x1b6(0xffff): %x", value);
	RDA5888_ReadReg(0x1b7, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x1b7(0x0160): %x", value);
	RDA5888_ReadReg(0x129, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x129(0xffff): %x", value);


    RDA5888_WriteReg(0x1b5, g_rda_reg1b5_value);
    //RDA5888_WriteReg(0x1b6, g_rda_reg1b6_value);
    //RDA5888_WriteReg(0x1b7, g_rda_reg1b7_value);

    //RDA5888_WriteReg(0x1b6, 0x3D2C);
    //RDA5888_WriteReg(0x1b7, 0x3EBC);
    //RDA5888_WriteReg(0x129, 0x09c1);
   
	RDA5888_ReadReg(0x1b5, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x1b5(0xffff): %x", value);
	RDA5888_ReadReg(0x1b6, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x1b6(0xffff): %x", value);
	RDA5888_ReadReg(0x1b7, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x1b7(0x0160): %x", value);
	RDA5888_ReadReg(0x129, &value);
	SCI_TRACE_LOW("ATV: _rda5888_Init, test2, 0x129(0xffff): %x", value);


    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: rda5888 Sleep
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_Sleep(uint32 sleep_enter)
{
    SCI_TRACE_LOW("ATV:_rda5888_Sleep sleep_enter[%d]",sleep_enter);
    if(SCI_TRUE==sleep_enter)
    {
        // todo...
    }
    else if(SCI_FALSE==sleep_enter)
    {
        // todo...
    }

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: rda5888 init scan channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_ScanChnInit(uint32 param)
{
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();

    SCI_TRACE_LOW("ATV:_rda5888_ScanChnInit param[%d]",param);

    //Clean chn map
    RDAATV_InitScanParam(atv_info_ptr->atv_cur_region);
	
    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: rda5888 scan channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_ScanChn(uint32 param)
{
    uint32 rtn=SCI_FALSE;
    uint32 chn_id=0x00;
    uint32 is_valid=0x00;

    SCI_TraceLow("ATV:_rda5888_ScanChn");

    if (SCI_TRUE != RDAATV_ScanChn(&chn_id, &is_valid))
    {
        rtn=((chn_id<<0x10) &0xffff0000) |((is_valid&0x01) <<0x08);
    }
    else
    {
        // scan all channels end.
        rtn = SCI_TRUE;
    }

    SCI_TraceLow("ATV:_rda5888_ScanChn, rtn = 0x%x, chn_id = 0x%x, is_valid = 0x%x", rtn, chn_id, is_valid);
    //rtn = SCI_TRUE;
    return rtn;
}

/******************************************************************************/
// Description: rda5888 set channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_SetChn(uint32 param)
{
    uint32 rtn=SCI_ERROR;
    uint32 cur_volume_level=0x00;
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();

	if (param > RDAAPP_GetChannelCount())
		return SCI_ERROR;

    SCI_TRACE_LOW("ATV:_rda5888_SetChn param[%d]",param);
    
    if(atv_info_ptr->atv_cur_chn==param)
    {
        return SCI_SUCCESS;
    }

    //turn off audio
    cur_volume_level=atv_info_ptr->cur_volume_level;
    _rda5888_SetVolume(atv_info_ptr->cur_volume_level&0xffffff00);

    atv_info_ptr->atv_cur_chn=param;
    
    rtn = RDAAPP_SetChannel(param, 0);

   
    SCI_Sleep(200);//wait chip stable
    
    //turn on audio
    _rda5888_SetVolume(cur_volume_level);

//	while (1)
//	{
//		SCI_Sleep(300);
//	}

    if (TRUE == rtn)
    
        rtn = SCI_SUCCESS;

    return rtn;
}

/******************************************************************************/
// Description: rda5888 set channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_AutoDectRegion(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;

    return rtn;
}

/******************************************************************************/
// Description: rda5888 get all chn num
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_GetAllChnNum(void)
{
    uint32 chn_total_num = RDAAPP_GetChannelCount();
	
    SCI_TRACE_LOW("ATV:_rda5888_GetAllChnNum total_num[%d]", chn_total_num);

    return chn_total_num;
}

/******************************************************************************/
// Description: rda5888 set volume
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_SetVolume(uint32 param)
{
    uint32 volume_level=param&0xff;
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();

    SCI_TRACE_LOW("ATV:_rda5888_SetVolume param[%d]",param);

    if(atv_info_ptr->cur_volume_level==volume_level)
    {
        return SCI_SUCCESS;
    }

    if(ATV_VOLUME_9<volume_level)
    {
        volume_level=ATV_VOLUME_9;
    }

    if(volume_level==ATV_VOLUME_0)
    {
        RDAAPP_SetMute(SCI_TRUE);
    }
    else
    {
        RDAAPP_SetMute(SCI_FALSE);
	 RDAAPP_SetVolume(volume_level);
    }

    atv_info_ptr->cur_volume_level=volume_level;

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: rda5888 is ntsc mode
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
PUBLIC BOOLEAN _rda5888_IsNTSCMode(void)
{

    BOOLEAN Is_NTSCMode=SCI_TRUE;
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();

    switch(atv_info_ptr->atv_cur_region)
    {
        case ATV_REGION_BRAZIL://video size is same as NTSC
        case ATV_REGION_JAPAN:
        case ATV_REGION_USA:
        case ATV_REGION_CANADA:
        case ATV_REGION_KOREA:
        case ATV_REGION_TAIWAN:
        case ATV_REGION_MEXICO:
        case ATV_REGION_CHILE:
        case ATV_REGION_VENEZUELA:
        case ATV_REGION_PHILIPPINES:
        case ATV_REGION_MYANMAR:
            Is_NTSCMode=SCI_TRUE;
            break;

        default:
            Is_NTSCMode=SCI_FALSE;
            break;
    }

    SCI_TRACE_LOW("ATV:_rda5888_IsNTSCMode Is_NTSCMode[%d]",Is_NTSCMode);
    
    return Is_NTSCMode;
}

/******************************************************************************/
// Description: rda5888 set region
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_SetRegion(uint32 param)
{    
    uint32 rtn = SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();

    SCI_TRACE_LOW("ATV:_rda5888_SetRegion param[%d]",param);
    
    switch(param)
    {
        case ATV_REGION_CHINA:
        case ATV_REGION_HONGKONG:
        case ATV_REGION_SHENZHEN:
        case ATV_REGION_CAMBODIA:
        case ATV_REGION_CANADA:
        case ATV_REGION_CHILE:
        case ATV_REGION_AFGHANISTAN:
        case ATV_REGION_ARGENTINA:
        case ATV_REGION_BRAZIL:
        case ATV_REGION_INDONESIA:
        case ATV_REGION_JAPAN:
        case ATV_REGION_KOREA:
        case ATV_REGION_LAOS:
        case ATV_REGION_MALAYSIA:
        case ATV_REGION_MEXICO:
        case ATV_REGION_MYANMAR:
        case ATV_REGION_PHILIPPINES:
        case ATV_REGION_SINGAPORE:
        case ATV_REGION_SOUTHAFRICA:
        case ATV_REGION_TAIWAN:
        case ATV_REGION_THAILAND:
        case ATV_REGION_TURKEY:
        case ATV_REGION_UAE:
        case ATV_REGION_UK:
        case ATV_REGION_USA:
        case ATV_REGION_VENEZUELA:
        case ATV_REGION_VIETNAM:
        case ATV_REGION_WESTERNEUROPE:
        case ATV_REGION_SPAIN:
        case ATV_REGION_PORTUGAL:
        case ATV_REGION_PAKISTAN:
        case ATV_REGION_INDIA:
        case ATV_REGION_AUSTRALIA:
        case ATV_REGION_PARAGURY:
        case ATV_REGION_URUGUAY:
        case ATV_REGION_BULGARIA:
        case ATV_REGION_CONGO:
        case ATV_REGION_EGYPT:
        case ATV_REGION_GERMANY:
        case ATV_REGION_IRAQ:
        case ATV_REGION_IRAN:
        case ATV_REGION_HUNGRY:
        case ATV_REGION_LIBYA:
        case ATV_REGION_LEBANON:
        case ATV_REGION_POLAND:
        case ATV_REGION_ROMANIA:
        case ATV_REGION_SANDIARABIA:
        case ATV_REGION_TUNISIA:
        case ATV_REGION_RUSSIA:
        {
            atv_info_ptr->atv_cur_region = param;
            _rda5888_InitChannel();
            break;
        }

        default :
        {
            atv_info_ptr->atv_cur_region = ATV_REGION_MAX;
            rtn=SCI_ERROR;
            break;
        }
    }

    if (SCI_TRUE == _rda5888_IsNTSCMode())
    {
        s_rda5888_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width=352;
        s_rda5888_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height=240;

        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=24;
        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=2+6;
        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=320;
        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=160;//230-8;
    }
    else
    {
        s_rda5888_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width = 352;//352;
        s_rda5888_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height= 280;//280;

        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=24; //24;
        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=2;///2+6;
        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=324;//324;
        s_rda5888_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=276;//276;
    }

	Sensor_SetInfo();

	return rtn;
}

/******************************************************************************/
// Description: rda5888 get rssi
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_GetRssi(void)
{
    uint32 rssi = 0x00;

    rssi = RDA5888_GetRSSI() + 110;
    
    if (rssi < 100)
		return ATV_RSSI_0;
	else if(rssi < 110)
		return ATV_RSSI_1;
	else if(rssi < 123)
		return ATV_RSSI_2;
	else if(rssi < 130)
		return ATV_RSSI_3;
	else
		return ATV_RSSI_4;

    //return rssi;
}

/******************************************************************************/
// Description: rda5888 get emc param
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_GetEmcParam(void)
{
    uint32 emc_param=0x00;

    emc_param=(s_emc_param[0]<<0x10)&0xffff0000;
    emc_param|=s_emc_param[1]&0xffff;

    return emc_param;
}

/******************************************************************************/
// Description: rda5888 close
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_Close(void)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_rda5888_GetInfo();
    
    SCI_TRACE_LOW("ATV:_rda5888_Close");

    atv_info_ptr->atv_cur_region=ATV_REGION_MAX;
    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;

    return rtn;
}

/******************************************************************************/
// Description: rda5888 iic write
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 rda5888_IICWriteReg(uint16 subaddr, uint16 data)
{
    //Sensor_WriteReg(subaddr, data);

    SCI_TRACE_LOW("ATV: rda5888_WriteReg reg/value(%x,%x) !!", subaddr, data);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: rda5888 iic read
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 rda5888_IICReadReg(uint16 subaddr, uint16* data)
{
    //*data=Sensor_ReadReg(subaddr);

    SCI_TRACE_LOW("ATV: rda5888_ReadReg reg/value(%x,%x) !!", subaddr, *data);

    return SCI_SUCCESS;
}


#ifdef  BB_GPIO_HOLD_RDA5888
//extern uint32 RDA5888_ReadReg(uint32 dwRegAddr, uint16 *pwData);  //just for debug
#define ATV_BBCTRL 118
uint32 g_rda_sleep_time1 = 0;
uint32 g_rda_sleep_time2 = 0;
uint32 g_rda_sleep_time3 = 0;
uint32 g_rda_sleep_time4 = 0;

LOCAL uint32 _rda5888_mask_enable()
{
    uint16  reg_read1,reg_read2;
    SCI_Sleep(g_rda_sleep_time1);
    
	GPIO_Enable(ATV_BBCTRL);
	GPIO_SetDirection(ATV_BBCTRL, 1);
	//GPIO_SetBBCtrl(0);
	GPIO_SetValue(ATV_BBCTRL, 0);
	TM_SendTestPointRequest ( 0xDEADBEEA, *(volatile uint32 *)(0x8A000380));
	SCI_Sleep(g_rda_sleep_time2);
	  /**/
	RDA5888_ReadReg(0x17b,&reg_read1);
	RDA5888_ReadReg(0x17c,&reg_read2);

	SCI_TRACE_LOW("harry, _rda5888_mask_enable  [0x17b]=%x,[0X17C]=%x\n",reg_read1,reg_read2);
}

LOCAL uint32 _rda5888_mask_disable()
{
    uint16  reg_read1,reg_read2;
    
	SCI_Sleep(g_rda_sleep_time3);
	GPIO_Enable(ATV_BBCTRL);
	GPIO_SetDirection(ATV_BBCTRL, 1);
	//GPIO_SetBBCtrl(1);
	GPIO_SetValue(ATV_BBCTRL, 1);
	TM_SendTestPointRequest ( 0xDEADBEEB, *(volatile uint32 *)(0x8A000380));
	SCI_Sleep(g_rda_sleep_time4);
      /* */
	RDA5888_ReadReg(0x17b,&reg_read1);
	RDA5888_ReadReg(0x17c,&reg_read2);

	SCI_TRACE_LOW("harry, _rda5888_mask_disable  [0x17b]=%x,[0X17C]=%x\n",reg_read1,reg_read2);
}
#endif


/******************************************************************************/
// Description: rda5888 IOCTL 
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _rda5888_IOCTL(uint32 ctl_param)
{
    uint32 rtn=SCI_SUCCESS;
    uint32 cmd=(ctl_param>>0x10)&0xffff;
    uint32 param=ctl_param&0xffff;
    
    switch(cmd)
    {
        case ATV_CMD_CHIP_INIT:
        {
            rtn=_rda5888_Init(param);
            break;
        }
        case ATV_CMD_CHIP_SLEEP:
        {
            rtn=_rda5888_Sleep(param);
            break;
        }
        case ATV_CMD_SCAN_INIT:
        {
            rtn=_rda5888_ScanChnInit(param);
            break;
        }
        case ATV_CMD_SCAN_CHN:
        {
            rtn=_rda5888_ScanChn(param);
            break;
        }
        case ATV_CMD_SET_CHN:
        {
            rtn=_rda5888_SetChn(param);
            break;
        }
        case ATV_CMD_DECT_REGION:
        {
            rtn=_rda5888_AutoDectRegion(param);
            break;
        }
        case ATV_CMD_SET_VOLUME:
        {
            rtn=_rda5888_SetVolume(param);
            break;
        }
        case ATV_CMD_SET_REGION:
        {
            rtn=_rda5888_SetRegion(param);
            break;
        }
        case ATV_CMD_GET_RSSI:
        {
            rtn=_rda5888_GetRssi();
            break;
        }
        case ATV_CMD_GET_ALL_CHN_NUM:
        {
            rtn=_rda5888_GetAllChnNum();
            break;
        }
        case ATV_CMD_GET_IS_NTSC:
        {
            rtn=_rda5888_IsNTSCMode();
            break;
        }
        case ATV_CMD_GET_EMC:
        {
            rtn=_rda5888_GetEmcParam();
            break;
        }
        case ATV_CMD_GET_INFO:
        {
            rtn=(uint32)_rda5888_GetInfo();
            break;
        }        
        case ATV_CMD_CLOSE:
        {
            rtn=_rda5888_Close();
            break;
        }
        case ATV_CMD_MASK_ENABLE:
        {
#ifdef  BB_GPIO_HOLD_RDA5888        
            rtn=_rda5888_mask_enable();
#endif
            break;
        }
        case ATV_CMD_MASK_DISABLE:
        {
#ifdef  BB_GPIO_HOLD_RDA5888                
            rtn=_rda5888_mask_disable();
#endif
            break;
        }
        default :
            break;
    }

    return rtn;
}
/*lint -restore */ 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
