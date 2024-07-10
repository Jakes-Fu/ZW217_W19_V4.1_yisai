/******************************************************************************
 ** Copyright (c) 
 ** File Name:		atv_qn8610.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor qn8610. 
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
#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "gpio_prod_api.h"
#include "ldo_drv.h"
#include "deep_sleep.h"
#include "qmadriver.h"
#include "qmafunc.h"

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
LOCAL uint32 _qn8610_PowerOnSequence(uint32 power_on);
LOCAL uint32 _qn8610_Identify(uint32 param);
LOCAL uint32 _qn8610_Init(uint32 param);
LOCAL uint32 _qn8610_IOCTL(uint32 ctl_param);
LOCAL uint32 _qn8610_GetResolutionTrimTab(uint32 param);
LOCAL uint32 _qn8610_SetVolume(uint32 param);
PUBLIC BOOLEAN _qn8610_IsNTSCMode(void);
/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/
LOCAL ATV_INFO_T s_atv_info={0x00};
LOCAL ATV_INFO_T_PTR s_atv_info_ptr=&s_atv_info;
    
LOCAL uint16 s_emc_param[2]={0xf140, 0xf0f0};
extern uint8 Region_code_table[QNM_TOTAL_REGION];


LOCAL SENSOR_REG_TAB_INFO_T s_qn8610_resolution_Tab_YUV[]=
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

LOCAL SENSOR_TRIM_T s_qn8610_resolution_trim_tab[]=
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

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_qn8610_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    _qn8610_PowerOnSequence,
    PNULL,
    _qn8610_Identify,
    PNULL,			// write register
    PNULL,			// read  register
    // Custom function
    _qn8610_IOCTL,
    _qn8610_GetResolutionTrimTab,

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
 PUBLIC SENSOR_INFO_T g_qn8610_yuv_info =
{
	0x54,				// salve i2c write address
	0x55, 				// salve i2c write address
	
	SENSOR_I2C_NOSTOP_BIT|\
	SENSOR_I2C_ACK_BIT|\
    SENSOR_I2C_REG_8BIT|\
    SENSOR_I2C_VAL_8BIT,    //SENSOR_I2C_STOP_BIT,			// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                            // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                            // bit1: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                            // other bit: reseved

    SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_N,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
	
	// environment mode
	0x00,
	// image effect
	0x00,
	// while balance mode
	0x00,		
	0x00,						    // brightness/contrast/sharpness/saturation/EV
	SENSOR_LOW_PULSE_RESET,			// reset pulse level
	50,								// reset pulse width(ms)
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	0x00,							// count of identify code
	0x00, 0x00,						// supply two code to identify sensor.
	0x00, 0x00,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	320,//720,					    // max width of source image
	240,//288,						// max height of source image
	"qn8610",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;			

	s_qn8610_resolution_Tab_YUV,	// point to resolution table information structure
	&s_qn8610_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_2800MV,             // iovdd
	SENSOR_AVDD_1800MV,             // dvdd
	4,                              // skip frame num before preview 
	3,                              // skip frame num before capture
	0,                              // deci frame num during preview	
	2,                              // deci frame num during video preview
	1,                              // threadhold eb
	1,                              // threadhold mode
	0x4d0,                          // threadhold st
	0x5d0,                           // threadhold en
    0,
    0
};

S_MA_DRIVER QN8610_para = {0};
S_FM_DRIVER QN8610_fmpara = {0};

/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description: qn8610 get infor
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL ATV_INFO_T_PTR _qn8610_GetInfo(void)
{    
	return s_atv_info_ptr;
}

/******************************************************************************/
// Description: get ov7670 rssolution trim tab
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_GetResolutionTrimTab(uint32 param)
{
    return (uint32)s_qn8610_resolution_trim_tab;
}

/******************************************************************************/
// Description: qn8610 init channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_InitChannel(void)
{
    //not need when use qn8610

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: sensor nmi600 power on/down sequence
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL void GPIO_SetAnalogTVPowerOn (BOOLEAN power)
{	
    _GPIO_PROD_SetVal (GPIO_PROD_ATV_POWER_ID, power);
   
    //SCI_TRACE_LOW:"GPIO_SetAnalogTVPowerOn,power = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_358_112_2_18_0_29_38_632,(uint8*)"d",power);
}
/******************************************************************************/
// Description: sensor qn8610 power on/down sequence
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_PowerOnSequence(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val=g_qn8610_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val=g_qn8610_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val=g_qn8610_yuv_info.iovdd_val;  
    //BOOLEAN power_down=g_qn8610_yuv_info.power_down_level;	    
    BOOLEAN reset_level=g_qn8610_yuv_info.reset_pulse_level;
    uint32 reset_width=g_qn8610_yuv_info.reset_pulse_width;

    //SCI_TRACE_LOW:"ATV:QN:_qn8610_PowerOnSequence power_on[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_260_112_2_18_0_29_52_655,(uint8*)"d",power_on);
    
    if(SCI_TRUE==power_on)
    {
        SCI_TV_EnableDeepSleep(DISABLE_AHB_SLEEP);
        
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
        GPIO_SetAnalogTVPowerOn(power_on);
        SCI_Sleep(20);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel((BOOLEAN)!reset_level);
        SCI_Sleep(100);
    }
    else
    {        
        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED); 

        GPIO_SetAnalogTVPowerOn(power_on);
        SCI_TV_EnableDeepSleep(ENABLE_AHB_SLEEP);
    }
    
    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 identify
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_Identify(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;
    uint16 value=0x00;
    
    //SCI_TRACE_LOW:"ATV:QN:_qn8610_Identify "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_293_112_2_18_0_29_52_656,(uint8*)"");
    
    Sensor_WriteReg(0x00,0x01);

    value = Sensor_ReadReg(0x00);
    
    if(0x01==value)
    {
        //SCI_TRACE_LOW:"ATV: the atv is QN6810"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_301_112_2_18_0_29_52_657,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"ATV: the atv is not QN6810"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_305_112_2_18_0_29_52_658,(uint8*)"");
        rtn=SCI_ERROR;
    }

    return rtn;
}

/******************************************************************************/
// Description: qn8610 init
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_Init(uint32 param)
{
    ATV_INFO_T_PTR atv_info_ptr=_qn8610_GetInfo();

    //SCI_TRACE_LOW:"ATV:QN:_qn8610_Init param[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_320_112_2_18_0_29_52_659,(uint8*)"d",param);

    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;
           
	QMA_SetRegion(&QN8610_para,Region_code_table[param]);
	QMA_Init(&QN8610_para,QNM_DEV_ATV);
	QMA_Delay(200);
    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 Sleep
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_Sleep(uint32 sleep_enter)
{
    //SCI_TRACE_LOW:"ATV:QN:_qn8610_Sleep sleep_enter[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_337_112_2_18_0_29_52_660,(uint8*)"d",sleep_enter);
    if(SCI_TRUE==sleep_enter)
    {
        QMA_SetSysMode(&QN8610_para,QNM_MODE_SLEEP);
    }
    else if(SCI_FALSE==sleep_enter)
    {
        QMA_SetSysMode(&QN8610_para,QNM_MODE_WORKING);
    }

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 init scan channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_ScanChnInit(uint32 param)
{
    //not need qn8610
    
    //SCI_TRACE_LOW:"ATV:QN:_qn8610_ScanChnInit param[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_358_112_2_18_0_29_52_661,(uint8*)"d",param);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 scan channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_ScanChn(uint32 param)
{
    uint32 rtn=SCI_FALSE;
    uint32 chn_id=0x00;
    uint32 is_valid=0x00;
    uint32 qma_ret = 0;
       
    qma_ret = (uint32)atv_qma_scan_chn(&QN8610_para,&chn_id,&is_valid);    
    SCI_TRACE_LOW("ATV::QN_STATUS = 0x%x",Sensor_ReadReg(0x07));

    if(qma_ret==SCI_TRUE)
    {
        //Ã»ËÑÍê
        //SCI_TRACE_LOW:"ATV:_qn8610_ScanChn chn_id[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_379_112_2_18_0_29_52_662,(uint8*)"d",chn_id);
        rtn=((chn_id<<0x10)&0xffff0000)|((is_valid&0x01)<<0x08);
    }
    else
    {
        //ËÑÍê
        rtn=SCI_TRUE;
        //SCI_TRACE_LOW:"ATV:_qn8610_ScanChn not complete rtn[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_386_112_2_18_0_29_52_663,(uint8*)"d",rtn);
    }

    return rtn;
}

/******************************************************************************/
// Description: qn8610 set channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_SetChn(uint32 param)
{
    uint32 rtn=SCI_ERROR;
    uint32 cur_volume_level=0x00;
    ATV_INFO_T_PTR atv_info_ptr=_qn8610_GetInfo();

    //SCI_TRACE_LOW:"ATV:QN:_qn8610_SetChn param[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_402_112_2_18_0_29_52_664,(uint8*)"d",param);
    
    if(atv_info_ptr->atv_cur_chn==param)
    {
        return SCI_SUCCESS;
    }

    //turn off audio
    cur_volume_level=atv_info_ptr->cur_volume_level;
    _qn8610_SetVolume(atv_info_ptr->cur_volume_level&0xffffff00);

    atv_info_ptr->atv_cur_chn=param;
    rtn = QMA_TuneToCH(&QN8610_para,param);
    
    SCI_Sleep(200);//wait chip stable
    
    //turn on audio
    _qn8610_SetVolume(cur_volume_level);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 set channel
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_AutoDectRegion(uint32 param)
{
    uint32 rtn=SCI_ERROR;

    rtn=QMA_AutoDetectionRegion(&QN8610_para);

    return rtn;
}

/******************************************************************************/
// Description: qn8610 get all chn num
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_GetAllChnNum(void)
{   
    //SCI_TRACE_LOW:"ATV:QN:_qn8610_GetAllChnNum total_num[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_443_112_2_18_0_29_52_665,(uint8*)"d",QN8610_para.rgnChTotalCount);

    return QN8610_para.rgnChTotalCount;
}

/******************************************************************************/
// Description: qn8610 set volume
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_SetVolume(uint32 param)
{
    uint32 volume_level=param&0xff;
    ATV_INFO_T_PTR atv_info_ptr=_qn8610_GetInfo();

    //SCI_TRACE_LOW:"ATV:QN:_qn8610_SetVolume param[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_457_112_2_18_0_29_52_666,(uint8*)"d",param);

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
        QMA_ConfigAudio(&QN8610_para, QNM_CONFIG_MUTE, SCI_TRUE);
        QMA_ConfigAudio(&QN8610_para, QNM_CONFIG_VOLUME, 00);
    }
    else
    {
        QMA_ConfigAudio(&QN8610_para, QNM_CONFIG_MUTE, SCI_FALSE);
        QMA_ConfigAudio(&QN8610_para, QNM_CONFIG_VOLUME, 41);
    }

    atv_info_ptr->cur_volume_level=volume_level;

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 set region
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_SetRegion(uint32 param)
{    
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_qn8610_GetInfo();

    //SCI_TRACE_LOW:"ATV:QN:_qn8610_SetRegion param[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_494_112_2_18_0_29_52_667,(uint8*)"d",param);
    
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
            atv_info_ptr->atv_cur_region=param;
            QMA_SetRegion(&QN8610_para,Region_code_table[(UINT8)param]);
            break;
        }

        default :
        {
            atv_info_ptr->atv_cur_region=ATV_REGION_MAX;
            return SCI_ERROR;
        }
    }

    if(SCI_TRUE==_qn8610_IsNTSCMode())
    {
        s_qn8610_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width=320;
        s_qn8610_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height=240;

        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=0;
        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=0;
        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=320;
        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=240;
    }
    else
    {
        s_qn8610_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width=320;
        s_qn8610_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height=240;

        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=0;
        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=0;
        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=320;
        s_qn8610_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=240;
    }

    Sensor_SetInfo();

	return rtn;
}

/******************************************************************************/
// Description: qn8610 get rssi
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_GetRssi(void)
{
    uint16 rssi=0x00;
    //uint32 rtn_rssi;

    rssi = QMA_GetRSSI(&QN8610_para);
    
    if(rssi < 100)
		return ATV_RSSI_0;
	else if(rssi < 110)
		return ATV_RSSI_1;
	else if(rssi < 123)
		return ATV_RSSI_2;
	else if(rssi < 130)
		return ATV_RSSI_3;
	else
		return ATV_RSSI_4;

    //return rtn_rssi;
}

/******************************************************************************/
// Description: qn8610 is ntsc mode
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
PUBLIC BOOLEAN _qn8610_IsNTSCMode(void)
{

    BOOLEAN Is_NTSCMode=SCI_TRUE;
    ATV_INFO_T_PTR atv_info_ptr=_qn8610_GetInfo();

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
            Is_NTSCMode=SCI_TRUE;
            break;

        default:
            Is_NTSCMode=SCI_FALSE;
            break;
    }

    //SCI_TRACE_LOW:"ATV:QN:_qn8610_IsNTSCMode Is_NTSCMode[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_641_112_2_18_0_29_53_668,(uint8*)"d",Is_NTSCMode);
    
    return Is_NTSCMode;
}

/******************************************************************************/
// Description: qn8610 get emc param
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_GetEmcParam(void)
{
    uint32 emc_param=0x00;

    emc_param=(s_emc_param[0]<<0x10)&0xffff0000;
    emc_param|=s_emc_param[1]&0xffff;

    return emc_param;
}

/******************************************************************************/
// Description: qn8610 close
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_Close(void)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_qn8610_GetInfo();
    
    //SCI_TRACE_LOW:"ATV:QN:_qn8610_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_669_112_2_18_0_29_53_669,(uint8*)"");

    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;

    return rtn;
}

/******************************************************************************/
// Description: qn8610 iic write
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 qn8610_IICWriteReg(uint16 subaddr, uint16 data)
{
    Sensor_WriteReg(subaddr, data);

    //SCI_TRACE_LOW:"ATV:QN: qn8610_WriteReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_685_112_2_18_0_29_53_670,(uint8*)"dd", subaddr, data);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 iic read
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 qn8610_IICReadReg(uint16 subaddr, uint16* data)
{
    *data=Sensor_ReadReg(subaddr);

    //SCI_TRACE_LOW:"ATV:QN: qn8610_ReadReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_QN8610_698_112_2_18_0_29_53_671,(uint8*)"dd", subaddr, *data);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: qn8610 IOCTL 
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _qn8610_IOCTL(uint32 ctl_param)
{
    uint32 rtn=SCI_SUCCESS;
    uint32 cmd=(ctl_param>>0x10)&0xffff;
    uint32 param=ctl_param&0xffff;
    
    switch(cmd)
    {
        case ATV_CMD_CHIP_INIT:
        {
            rtn=_qn8610_Init(param);
            break;
        }
        case ATV_CMD_CHIP_SLEEP:
        {
            rtn=_qn8610_Sleep(param);
            break;
        }
        case ATV_CMD_SCAN_INIT:
        {
            rtn=_qn8610_ScanChnInit(param);
            break;
        }
        case ATV_CMD_SCAN_CHN:
        {
            rtn=_qn8610_ScanChn(param);
            break;
        }
        case ATV_CMD_SET_CHN:
        {
            rtn=_qn8610_SetChn(param);
            break;
        }
        case ATV_CMD_DECT_REGION:
        {
            rtn=_qn8610_AutoDectRegion(param);
            break;
        }
        case ATV_CMD_SET_VOLUME:
        {
            rtn=_qn8610_SetVolume(param);
            break;
        }
        case ATV_CMD_SET_REGION:
        {
            rtn=_qn8610_SetRegion(param);
            break;
        }
        case ATV_CMD_GET_RSSI:
        {
            rtn=_qn8610_GetRssi();
            break;
        }
        case ATV_CMD_GET_ALL_CHN_NUM:
        {
            rtn=_qn8610_GetAllChnNum();
            break;
        }
        case ATV_CMD_GET_IS_NTSC:
        {
            rtn=_qn8610_IsNTSCMode();
            break;
        }
        case ATV_CMD_GET_EMC:
        {
            rtn=_qn8610_GetEmcParam();
            break;
        }
        case ATV_CMD_GET_INFO:
        {
            rtn=(uint32)_qn8610_GetInfo();
            break;
        }        
        case ATV_CMD_CLOSE:
        {
            rtn=_qn8610_Close();
            break;
        }
        case ATV_CMD_MASK_ENABLE:
        {
#ifdef  BB_GPIO_HOLD_QN8610        
            //rtn=_rda5888_mask_enable();
#endif
            break;
        }
        case ATV_CMD_MASK_DISABLE:
        {
#ifdef  BB_GPIO_HOLD_QN8610                
            //rtn=_rda5888_mask_disable();
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
