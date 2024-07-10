/******************************************************************************
 ** Copyright (c)
 ** File Name:      atv_nmi600.c                                         *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor nmi600.
 **
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
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
#include "nmiapi.h"
#include "osc_drvapi.h "

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/*lint -save -e64 -e746 -e785 -e762*/
/**---------------------------------------------------------------------------*
 **                     Extern Function Declaration                           *
 **---------------------------------------------------------------------------*/
extern int8 atv_nmi600_init(void);
extern void atv_nmi600_set_region(unsigned int region);
extern void atv_nmi_init_scan_param(int region);
extern void atv_nmi600_power_off(void);
extern void atv_nmi600_set_volume(unsigned int vol);
extern void atv_nmi600_set_mute(unsigned char mute);
extern void atv_nmi600_get_rssi(unsigned short *rssilevel);
extern uint32 NMIAPP_GetTotalSearchNum(void);
extern uint32 atv_nmi_scan_chn(uint32* chn_id, uint32* is_valid);
extern PUBLIC BOOLEAN NMI_ReadChipID();
extern unsigned int atv_nmi600_fast_set_channel(unsigned int channelID);

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**                         Struct Definition                                   *
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 nmi600_IICWriteReg(uint16 addr, uint16 data);
LOCAL uint32 nmi600_IICReadReg(uint16 addr, uint16 *data);
LOCAL uint32 _nmi600_Identify(uint32 param);
LOCAL uint32 _nmi600_Init(uint32 param);
LOCAL uint32 _nmi600_IOCTL(uint32 ctl_param);
LOCAL uint32 _nmi600_GetResolutionTrimTab(uint32 param);
LOCAL uint32 _nmi600_SetVolume(uint32 param);
LOCAL uint32 _nmi600_PowerOnSequence(uint32 power_on);
LOCAL uint32 _nmi600_GetResolutionTrimTab(uint32 param);
PUBLIC BOOLEAN _nmi600_IsNTSCMode(void);
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL ATV_INFO_T s_atv_info= {0x00};
LOCAL ATV_INFO_T_PTR s_atv_info_ptr=&s_atv_info;
LOCAL uint16 s_emc_param[2]= {0xf140, 0xf0f0};

LOCAL SENSOR_REG_TAB_INFO_T s_nmi600_resolution_Tab_YUV[]=
{
    // COMMON INIT
    { PNULL, 0, 0, 0, 24, SENSOR_IMAGE_FORMAT_YUV422},

    // YUV422 PREVIEW 1
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

LOCAL SENSOR_TRIM_T s_nmi600_resolution_trim_tab[]=
{
    // COMMON INIT
    {16, 12, 288, 216},//{16, 12, 220, 168},
    
    // YUV422 PREVIEW 1
    {16, 12, 288, 216},//{16, 12, 220, 168},
    {0, 0, 0, 0},

    {0, 0, 0, 0},
    {0, 0, 0, 0},

    // YUV422 PREVIEW 2
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_nmi600_ioctl_func_tab =
{
    // Internal
    PNULL,
    _nmi600_PowerOnSequence,
    PNULL,
    _nmi600_Identify,
    PNULL,          // write register
    PNULL,          // read  register
    // Custom function
    _nmi600_IOCTL,
    _nmi600_GetResolutionTrimTab,

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
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC SENSOR_INFO_T g_nmi600_yuv_info =
{
    0xc0,               // salve i2c write address
    0xc1,               // salve i2c read address

    SENSOR_I2C_CUSTOM|\
    SENSOR_I2C_NOACK_BIT,           // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
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
    20,                                // reset pulse width(ms)
    SENSOR_LOW_LEVEL_PWDN,     // 1: high level valid; 0: low level valid

    0x00,                               // count of identify code
    0x00, 0x00,                     // supply two code to identify sensor.
    0x00, 0x00,                     // for Example: index = 0-> Device id, index = 1 -> version id

    SENSOR_AVDD_2800MV,             // voltage of avdd

    320,                            // max width of source image
    240,                            // max height of source image
    "nmi600",                      // name of sensor

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_UYVY,   // pattern of input image form sensor;

    s_nmi600_resolution_Tab_YUV,   // point to resolution table information structure
    &s_nmi600_ioctl_func_tab,      // point to ioctl function table

    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1800MV,                      // dvdd
    4,                     // skip frame num before preview
    3,                     // skip frame num before capture
    0,                     // deci frame num during preview
    2,                     // deci frame num during video preview
    1,                     // threadhold eb
    1,                     // threadhold mode
    0x480,                  // threadhold st
    0xA0,                  // threadhold en
    0,
    0
};

/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description: nmi600 get infor
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL ATV_INFO_T_PTR _nmi600_GetInfo(void)
{
    return s_atv_info_ptr;
}

/******************************************************************************/
// Description: get ov7670 rssolution trim tab
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_GetResolutionTrimTab(uint32 param)
{
    return(uint32) s_nmi600_resolution_trim_tab;
}

/******************************************************************************/
// Description: nmi600 init channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_InitChannel(void)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();
    uint32 CurrRegion=nChina;

    //SCI_TRACE_LOW:"SENSOR: _nmi600_InitChannel"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_253_112_2_18_0_29_38_631,(uint8*)"");

    switch(atv_info_ptr->atv_cur_region)
    {
        case ATV_REGION_ARGENTINA:
        case ATV_REGION_PARAGURY:
        case ATV_REGION_URUGUAY:
            CurrRegion=nArgentina;
            break;

        case ATV_REGION_BRAZIL:
            CurrRegion=nBrazil;
            break;

        case ATV_REGION_UK:
        case ATV_REGION_HONGKONG:
        case ATV_REGION_SOUTHAFRICA:
            CurrRegion=nUk;
            break;

        case ATV_REGION_AFGHANISTAN:
        case ATV_REGION_CAMBODIA:
        case ATV_REGION_INDIA:	
        case ATV_REGION_INDONESIA:
        case ATV_REGION_MALAYSIA:
        case ATV_REGION_LAOS:
        case ATV_REGION_IRAN:
        case ATV_REGION_SANDIARABIA:
        case ATV_REGION_IRAQ:
        case ATV_REGION_LEBANON:	
        case ATV_REGION_PAKISTAN:
        case ATV_REGION_SINGAPORE:
        case ATV_REGION_UAE:
        case ATV_REGION_THAILAND:
        case ATV_REGION_TURKEY:
        case ATV_REGION_AUSTRALIA:
        case ATV_REGION_WESTERNEUROPE:
        case ATV_REGION_SPAIN:
        case ATV_REGION_PORTUGAL:
        case ATV_REGION_EGYPT:
        case ATV_REGION_LIBYA:
        case ATV_REGION_GERMANY:
            CurrRegion=nAfghanistan;
            break;

        case ATV_REGION_TUNISIA:
            CurrRegion=nTunisia;
            break;

        case ATV_REGION_CONGO:
        case ATV_REGION_RUSSIA:
        case ATV_REGION_JAPAN:
            CurrRegion=nCongo;
            break;

        case ATV_REGION_VIETNAM:
        case ATV_REGION_BULGARIA:
        case ATV_REGION_HUNGRY:
        case ATV_REGION_POLAND:
        case ATV_REGION_ROMANIA:
            CurrRegion=nVietnam;
            break;

        case ATV_REGION_CHINA:
            CurrRegion=nChina;
            break;

        case ATV_REGION_SHENZHEN:
            CurrRegion=nChinaSz;
            break;

        case ATV_REGION_USA:
        case ATV_REGION_CANADA:
        case ATV_REGION_TAIWAN:
        case ATV_REGION_CHILE:
        case ATV_REGION_PHILIPPINES:
        //case ATV_REGION_COLUMBIA:
        case ATV_REGION_VENEZUELA:
        //case ATV_REGION_BOLIVIA:
        case ATV_REGION_MEXICO:
        case ATV_REGION_KOREA:
        case ATV_REGION_MYANMAR:
            CurrRegion=nUsa;
            break;		

        default:
            CurrRegion=nChina;
            break;
    }

    atv_nmi600_set_region(CurrRegion);
    atv_nmi_init_scan_param(CurrRegion);

    return rtn;
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
    _GPIO_PROD_SetVal (GPIO_PROD_ATV_LDO_ID, power);
    
    //SCI_TRACE_LOW:"GPIO_SetAnalogTVPowerOn,power = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_358_112_2_18_0_29_38_632,(uint8*)"d",power);
}

LOCAL uint32 _nmi600_PowerOnSequence(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val = g_nmi600_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val = g_nmi600_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val = g_nmi600_yuv_info.iovdd_val;
    BOOLEAN reset_level=g_nmi600_yuv_info.reset_pulse_level;
    uint32 reset_width=g_nmi600_yuv_info.reset_pulse_width;

    //SCI_TRACE_LOW:"ATV: _nmi600_Power_On(1:on, 0:off): %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_369_112_2_18_0_29_38_633,(uint8*)"d", power_on);

    if(SCI_TRUE == power_on)
    {    
        SCI_Sleep(10); 
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);	
        GPIO_SetAnalogTVPowerOn(power_on);
        SCI_Sleep(10);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel((BOOLEAN)!reset_level);
        SCI_Sleep(10);   
    }
    else
    {   
        SCI_Sleep(10);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(10);
        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);
        GPIO_SetAnalogTVPowerOn(FALSE);
        SCI_Sleep(10);  
    }

    //SCI_TRACE_LOW:"ATV: _nmi600_Power_On(1:on, 0:off) end : %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_392_112_2_18_0_29_38_634,(uint8*)"d", power_on);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: nmi600 identify
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_Identify(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;

    //SCI_TRACE_LOW:"ATV: _nmi600_Identify"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_405_112_2_18_0_29_38_635,(uint8*)"");

    if(SCI_TRUE!=NMI_ReadChipID())
    {
        //SCI_TRACE_LOW:"ATV: the atv is not nmi600"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_409_112_2_18_0_29_38_636,(uint8*)"");
        rtn=SCI_ERROR;
    }
    else
    {
        //SCI_TRACE_LOW:"ATV: the atv is nmi600"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_414_112_2_18_0_29_38_637,(uint8*)"");
    }

    return rtn;
}

/******************************************************************************/
// Description: nmi600 init
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_Init(uint32 param)
{
	DCXO_REF_E dcxo_ref=0;
    uint32 rtn = SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();

    //SCI_TraceLow:"ATV:_nmi600_Init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_429_112_2_18_0_29_38_638,(uint8*)"");

    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;



	//atv 26M Enable,chip_drv interface
	dcxo_ref = DCXO_REF2;
	OSC_HAL_EnableRefAutoSleep(dcxo_ref,SCI_TRUE);
    rtn = atv_nmi600_init(); //@spreadtrum bug177560@
    return rtn;
}

/******************************************************************************/
// Description: nmi600 Sleep
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_Sleep(uint32 sleep_enter)
{
    //SCI_TraceLow:"ATV:_nmi600_Sleep"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_445_112_2_18_0_29_38_639,(uint8*)"");

    if(SCI_TRUE==sleep_enter)
    {

    }
    else if(SCI_FALSE==sleep_enter)
    {

    }

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: nmi600 init scan channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_ScanChnInit(uint32 param)
{
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();

    atv_nmi_init_scan_param(atv_info_ptr->atv_cur_region);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: nmi600 scan channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_ScanChn(uint32 param)
{
    uint32 rtn=SCI_FALSE;
    uint32 chn_id=0x00;
    uint32 is_valid=0x00;

    //SCI_TraceLow:"ATV:_nmi600_ScanChn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_482_112_2_18_0_29_38_640,(uint8*)"");

    if(SCI_TRUE!=atv_nmi_scan_chn(&chn_id, &is_valid))
    {
        rtn=((chn_id<<0x10) &0xffff0000) |((is_valid&0x01) <<0x08);
    }
    else
    {
        rtn=SCI_TRUE;
    }

    return rtn;
}

/******************************************************************************/
// Description: nmi600 set channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_SetChn(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;
    uint32 cur_volume_level=0x00;
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();

    //SCI_TraceLow:"ATV:_nmi600_SetChn : 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_506_112_2_18_0_29_38_641,(uint8*)"d", param);

    if(atv_info_ptr->atv_cur_chn==param)
    {
        return SCI_SUCCESS;
    }

    //turn off audio
    cur_volume_level=atv_info_ptr->cur_volume_level;
    _nmi600_SetVolume(atv_info_ptr->cur_volume_level&0xffffff00);

    atv_info_ptr->atv_cur_chn=param;
    atv_nmi600_fast_set_channel(param);
    SCI_Sleep(200);

    //turn on audio
    _nmi600_SetVolume(cur_volume_level);

    return rtn;
}

/******************************************************************************/
// Description: nmi600 get all chn num
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_GetAllChnNum(void)
{
    uint32 chn_num=0x00;

    //SCI_TraceLow:"ATV:_nmi600_GetAllChnNum"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_535_112_2_18_0_29_38_642,(uint8*)"");
    
    chn_num=NMIAPP_GetTotalSearchNum();

    return chn_num;
}

/******************************************************************************/
// Description: nmi600 set volume
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_SetVolume(uint32 param)
{
    uint32 volume_level=param&0xff;
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();

    //SCI_TraceLow:"ATV:_nmi600_SetVolume : 0x0%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_551_112_2_18_0_29_38_643,(uint8*)"d", param);

    if(atv_info_ptr->cur_volume_level==volume_level)
    {
        return SCI_SUCCESS;
    }

    if(ATV_VOLUME_0==volume_level)
    {
        volume_level=ATV_VOLUME_0;
    }

    if(ATV_VOLUME_9<volume_level)
    {
        volume_level=ATV_VOLUME_9;
    }

    if(volume_level==ATV_VOLUME_0)
    {
        atv_nmi600_set_volume(ATV_VOLUME_0);
        atv_nmi600_set_mute(SCI_TRUE);
    }
    else
    {
        atv_nmi600_set_volume(ATV_VOLUME_6);
        atv_nmi600_set_mute(SCI_FALSE);
    }

    atv_info_ptr->cur_volume_level=volume_level;

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: nmi600 set region
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_SetRegion(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();

    //SCI_TraceLow:"ATV:_nmi600_SetRegion"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_593_112_2_18_0_29_38_644,(uint8*)"");

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
            _nmi600_InitChannel();
            break;
        }

        default :
        {
            atv_info_ptr->atv_cur_region=ATV_REGION_MAX;
            return SCI_ERROR;
        }
    }
#if 0
    if(SCI_TRUE==_nmi600_IsNTSCMode())
    {
        s_nmi600_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width=320;
        s_nmi600_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height=240;

        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=0;
        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=0;
        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=320;
        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=240;
    }
    else
    {
        s_nmi600_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width=320;
        s_nmi600_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height=240;

        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=0;
        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=0;
        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=320;
        s_nmi600_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=240;
    }

    Sensor_SetInfo();
#endif
    return rtn;
}

/******************************************************************************/
// Description: nmi600 get rssi
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_GetRssi(void)
{
    uint16 rssi=0x00;
    uint32 rtn_rssi;

    //SCI_TraceLow:"ATV:_nmi600_GetRssi"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_694_112_2_18_0_29_39_645,(uint8*)"");

    atv_nmi600_get_rssi(&rssi);

    switch(rssi)
    {
        case 1:
        case 2:
        {
            rtn_rssi=ATV_RSSI_0;
            break;
        }
        case 3:
        {
            rtn_rssi=ATV_RSSI_1;
            break;
        }
        case 4:
        {
            rtn_rssi=ATV_RSSI_2;
            break;
        }
        case 5:
        case 6:
        {
            rtn_rssi=ATV_RSSI_3;
            break;
        }
        case 7:
        case 8:
        {
            rtn_rssi=ATV_RSSI_4;
            break;
        }
        default:
            rtn_rssi=ATV_RSSI_4;
            break;
    }

    return rtn_rssi;
}

/******************************************************************************/
// Description: nmi600 is ntsc mode
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
PUBLIC BOOLEAN _nmi600_IsNTSCMode(void)
{

    BOOLEAN Is_NTSCMode=SCI_TRUE;
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();

    //SCI_TraceLow:"ATV:_nmi600_IsNTSCMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_746_112_2_18_0_29_39_646,(uint8*)"");

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

    return Is_NTSCMode;
}

/******************************************************************************/
// Description: nmi600 get emc param
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_GetEmcParam(void)
{
    uint32 emc_param=0x00;

    emc_param=(s_emc_param[0]<<0x10)&0xffff0000;
    emc_param|=s_emc_param[1]&0xffff;

    return emc_param;
}

/******************************************************************************/
// Description: nmi600 close
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_Close(void)
{
	DCXO_REF_E dcxo_ref=0;
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_nmi600_GetInfo();

    //SCI_TraceLow:"ATV:_nmi600_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_794_112_2_18_0_29_39_647,(uint8*)"");

    atv_nmi600_power_off();

    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;

	//atv 26M Unenable,chip_drv interface
	dcxo_ref = DCXO_REF2;
	OSC_HAL_EnableRefAutoSleep(dcxo_ref,SCI_FALSE);

    return rtn;
}

/******************************************************************************/
// Description: nmi600 iic write
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 nmi600_IICWriteReg(uint16 addr, uint16 data)
{
    //TLGI2C_WriteReg(0x00, (uint32)addr, data);

    //SCI_TRACE_LOW:"ATV: nmi600_WriteReg reg/value(0x%x,0x%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_812_112_2_18_0_29_39_648,(uint8*)"dd", addr, data);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: nmi600 iic read
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 nmi600_IICReadReg(uint16 addr, uint16 *data)
{
    //TLGI2C_ReadReg(0x00, (uint32)addr, data);

    //SCI_TRACE_LOW:"ATV: nmi600_ReadReg reg/value(0x%x,0x%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATV_NMI601_825_112_2_18_0_29_39_649,(uint8*)"dd", addr, *data);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: nmi600 IOCTL
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _nmi600_IOCTL(uint32 ctl_param)
{
    uint32 rtn=SCI_SUCCESS;
    uint32 cmd=(ctl_param>>0x10) &0xffff;
    uint32 param=ctl_param&0xffff;

    //SCI_TraceLow("ATV:_nmi600_IOCTL cmd:0x%x, param:0x%x",cmd,param);

    switch(cmd)
    {
        case ATV_CMD_CHIP_INIT:
        {
            rtn=_nmi600_Init(param);
            break;
        }
        case ATV_CMD_CHIP_SLEEP:
        {
            rtn=_nmi600_Sleep(param);
            break;
        }
        case ATV_CMD_SCAN_INIT:
        {
            rtn=_nmi600_ScanChnInit(param);
            break;
        }
        case ATV_CMD_SCAN_CHN:
        {
            rtn=_nmi600_ScanChn(param);
            break;
        }
        case ATV_CMD_SET_CHN:
        {
            rtn=_nmi600_SetChn(param);
            break;
        }
        case ATV_CMD_SET_VOLUME:
        {
            rtn=_nmi600_SetVolume(param);
            break;
        }
        case ATV_CMD_SET_REGION:
        {
            rtn=_nmi600_SetRegion(param);
            break;
        }
        case ATV_CMD_GET_RSSI:
        {
            rtn=_nmi600_GetRssi();
            break;
        }
        case ATV_CMD_GET_ALL_CHN_NUM:
        {
            rtn=_nmi600_GetAllChnNum();
            break;
        }
        case ATV_CMD_GET_IS_NTSC:
        {
            rtn=_nmi600_IsNTSCMode();
            break;
        }
        case ATV_CMD_GET_EMC:
        {
            rtn=_nmi600_GetEmcParam();
            break;
        }
        case ATV_CMD_GET_INFO:
        {
            rtn=(uint32)_nmi600_GetInfo();
            break;
        }        
        case ATV_CMD_CLOSE:
        {
            rtn=_nmi600_Close();
            break;
        }
        case ATV_CMD_MASK_ENABLE:
        {
            //rtn=_nmi600_mask_enable();
            //SCI_TraceLow("atv capture done callback be calling ");
            break;
        }
        case ATV_CMD_MASK_DISABLE:
        {
            //rtn=_nmi600_mask_disable();
            //SCI_TraceLow("atv invalide done callback be calling ");
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
