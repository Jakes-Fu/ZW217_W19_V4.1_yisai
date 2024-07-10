//##!(important notice: do not remove this line)
/******************************************************************************
 ** Copyright (c)
 ** File Name:      atv_tlg1120.c                                         *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor tlg1120.
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
#include "sensor_cfg.h"
#include "spload_main.h"
#include "sensor_drv.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "gpio_prod_api.h"
#include "ldo_drv.h"
#include "tlgi2c.h"
#include "tlg1120App.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/*lint -save -e64 -e716 -e762 -e785*/
/**---------------------------------------------------------------------------*
 **                     Extern Function Declaration                           *
 **---------------------------------------------------------------------------*/
//extern p_tlg_channel_t(p_tlg_cur_map); /*!< Current Default Channel Map*/
extern tlg_channel_t(tlg_argentina_pal_nc_chn_map);
extern tlg_channel_t(tlg_brazil_pal_m_chn_map);
extern tlg_channel_t(tlg_pal_i_chn_map);
extern tlg_channel_t(tlg_ntsc_japan_chn_map);
extern tlg_channel_t(tlg_pal_b_g_chn_map);
extern tlg_channel_t(tlg_vietnam_pal_d_k_chn_map);
extern tlg_channel_t(tlg_china_pald_chn_map);
extern tlg_channel_t(tlg_ntsc_chn_map);
extern tlg_channel_t(tlg_search_chn_map);
//extern tlg_channel_t(tlg_fm_chn_map);
extern tlg_channel_t(tlg_palI_sz_chn_map);
//extern tlg_channel_t(tlg_test_chn_map);
//extern tlg_channel_t(tlg_test1_chn_map);
//extern tlg_channel_t(tlg_test2_chn_map);
extern tlg_channel_t(tlg_secam_b_g_chn_map);
extern tlg_channel_t(tlg_secam_d_k_chn_map);
extern tlg_channel_t(tlg_secam_d_k_hybrid_chn_map);

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
LOCAL uint32 TLG1120_IICWriteReg(uint16 addr, uint16 data);
LOCAL uint32 TLG1120_IICReadReg(uint16 addr, uint16 *data);
LOCAL uint32 _TLG1120_Identify(uint32 param);
LOCAL uint32 _TLG1120_Init(uint32 param);
LOCAL uint32 _TLG1120_IOCTL(uint32 ctl_param);
LOCAL uint32 _TLG1120_SetVolume(uint32 param);
LOCAL uint32 _TLG1120_PowerOnSequence(uint32 power_on);
LOCAL uint32 _TLG1120_GetResolutionTrimTab(uint32 param);
PUBLIC BOOLEAN _TLG1120_IsNTSCMode(void);
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL ATV_INFO_T s_atv_info= {0x00};
LOCAL ATV_INFO_T_PTR s_atv_info_ptr=&s_atv_info;
LOCAL uint16 s_emc_param[2]= {0xf140, 0xf0f0};

LOCAL SENSOR_REG_TAB_INFO_T s_tlg1120_resolution_Tab_YUV[]=
{
    // COMMON INIT
    { PNULL, 0, 0, 0, 24, SENSOR_IMAGE_FORMAT_YUV422},

    // YUV422 PREVIEW 1
    { PNULL, 0, 720, 288, 24, SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL, 0, 0, 0, 0, 0},

    { PNULL, 0, 0, 0, 0, 0},
    { PNULL, 0, 0, 0, 0, 0},

    // YUV422 PREVIEW 2
    { PNULL, 0, 0, 0, 0, 0},
    { PNULL, 0, 0, 0, 0, 0},
    { PNULL, 0, 0, 0, 0, 0},
    { PNULL, 0, 0, 0, 0, 0}
};

LOCAL SENSOR_TRIM_T s_tlg1120_resolution_trim_tab[]=
{	
	// COMMON INIT
	{0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 1	
	{24, 0, 680, 288, 0},
	{0, 0, 0, 0, 0},
	
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0}
};

LOCAL const SENSOR_IOCTL_FUNC_TAB_T s_tlg1120_ioctl_func_tab =
{
    // Internal
    PNULL,
    _TLG1120_PowerOnSequence,
    PNULL,
    _TLG1120_Identify,
    PNULL,          // write register
    PNULL,          // read  register
    // Custom function
    _TLG1120_IOCTL,
    _TLG1120_GetResolutionTrimTab,

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
PUBLIC const SENSOR_INFO_T g_tlg1120_yuv_info =
{
    (TLGI2C_DEFAULT_BASE_ADDR<<0x01),               // salve i2c write address
    ((TLGI2C_DEFAULT_BASE_ADDR<<0x01)+0x01),      // salve i2c read address

    SENSOR_I2C_NOACK_BIT|\
    SENSOR_I2C_CUSTOM|\
    0,                              // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit1: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                    // other bit: reseved
    SENSOR_HW_SIGNAL_PCLK_N|\
    SENSOR_HW_SIGNAL_VSYNC_P|\
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
    100,                                // reset pulse width(ms)
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid

    0x00,                               // count of identify code
    0x00, 0x00,                     // supply two code to identify sensor.
    0x00, 0x00,                     // for Example: index = 0-> Device id, index = 1 -> version id

    SENSOR_AVDD_2800MV,             // voltage of avdd

    720,                            // max width of source image
    288,                            // max height of source image
    "TLG1120",                      // name of sensor

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;

    s_tlg1120_resolution_Tab_YUV,   // point to resolution table information structure
    &s_tlg1120_ioctl_func_tab,      // point to ioctl function table

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
    0x1d0,                  // threadhold st
    0x2d0,                  // threadhold en
    0
};

/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description: TLG1120 get infor
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL ATV_INFO_T_PTR _TLG1120_GetInfo(void)
{
    return s_atv_info_ptr;
}

/******************************************************************************/
// Description: get ov7670 rssolution trim tab
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_GetResolutionTrimTab(uint32 param)
{
    return(uint32) s_tlg1120_resolution_trim_tab;
}

/******************************************************************************/
// Description: TLG1120 init channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_InitChannel(void)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_TLG1120_GetInfo();

    s_lcm_func_table->trace("SENSOR: _TLG1120_InitChannel");

    switch(atv_info_ptr->atv_cur_region)
    {
        case ATV_REGION_ARGENTINA:
        case ATV_REGION_PARAGURY:
        case ATV_REGION_URUGUAY:
        {
            TLGAPP_SetChannelMap(tlg_argentina_pal_nc_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_NC, TLG_BW_8);
            break;
        }
        case ATV_REGION_BRAZIL:
        {
            TLGAPP_SetChannelMap(tlg_brazil_pal_m_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_M, TLG_BW_8);
            break;
        }
        case ATV_REGION_UK:
        case ATV_REGION_HONGKONG:
        case ATV_REGION_SOUTHAFRICA:
        {
            TLGAPP_SetChannelMap(tlg_pal_i_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_B, TLG_BW_8);
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
        case ATV_REGION_MYANMAR:
        case ATV_REGION_LAOS:
        case ATV_REGION_SPAIN:
        case ATV_REGION_PORTUGAL:
        case ATV_REGION_PAKISTAN:
        case ATV_REGION_INDIA:
        case ATV_REGION_AUSTRALIA:
        {
            TLGAPP_SetChannelMap(tlg_pal_b_g_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_B, TLG_BW_8);
            break;
        }
        case ATV_REGION_VIETNAM:
        {
            TLGAPP_SetChannelMap(tlg_vietnam_pal_d_k_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_D, TLG_BW_8);
            break;
        }
        case ATV_REGION_CHINA:
        {
            TLGAPP_SetChannelMap(tlg_china_pald_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_D, TLG_BW_8);
            break;
        }
        case ATV_REGION_SHENZHEN:
        {
            TLGAPP_SetChannelMap(tlg_palI_sz_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_I, TLG_BW_8);
            break;
        }
        case ATV_REGION_JAPAN:
        {
            TLGAPP_SetChannelMap(tlg_ntsc_japan_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_NTSC_M, TLG_BW_6);
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
            TLGAPP_SetChannelMap(tlg_ntsc_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_NTSC_M, TLG_BW_6);
            break;
        }
        case ATV_REGION_EGYPT:
        case ATV_REGION_GERMANY:
        case ATV_REGION_IRAQ:
        case ATV_REGION_IRAN:
        case ATV_REGION_LIBYA:
        case ATV_REGION_LEBANON:
        case ATV_REGION_SANDIARABIA:
        case ATV_REGION_TUNISIA:
        {
            TLGAPP_SetChannelMap(tlg_secam_b_g_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_SECAM_B, TLG_BW_8);
            break;
        }
        case ATV_REGION_BULGARIA:
        case ATV_REGION_CONGO:
        case ATV_REGION_HUNGRY:
        case ATV_REGION_POLAND:
        case ATV_REGION_ROMANIA:
        {
            TLGAPP_SetChannelMap(tlg_secam_d_k_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_SECAM_D, TLG_BW_8);
            break;
        }
        case ATV_REGION_RUSSIA:
        {
            TLGAPP_SetChannelMap(tlg_secam_d_k_hybrid_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_SECAM_D, TLG_BW_8);
            break;
        }
        default:
        {
            TLGAPP_SetChannelMap(tlg_china_pald_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, TLG_VSTD_PAL_D, TLG_BW_8);
            break;
        }
    }

	TLGAPP_InitVideoSize(FALSE);

    return rtn;
}

/******************************************************************************/
// Description: sensor tlg1120 power on/down sequence
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_PowerOnSequence(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val=g_tlg1120_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val=g_tlg1120_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val=g_tlg1120_yuv_info.iovdd_val;
    //BOOLEAN power_down=g_tlg1120_yuv_info.power_down_level;
    BOOLEAN reset_level=g_tlg1120_yuv_info.reset_pulse_level;
    uint32 reset_width=g_tlg1120_yuv_info.reset_pulse_width;

    s_lcm_func_table->trace("SENSOR: _tlg1120_Power_On(1:on, 0:off): %d", power_on);

    if(SCI_TRUE==power_on)
    {
      s_lcm_func_table->sci_sleep(500);
        s_lcm_func_table->sensor_setresetlevel(reset_level);
        s_lcm_func_table->sci_sleep(20);
        s_lcm_func_table->sensor_setvoltage(dvdd_val, avdd_val, iovdd_val);
        s_lcm_func_table->sci_sleep(50);
        s_lcm_func_table->sensor_setresetlevel((BOOLEAN)!reset_level);
        s_lcm_func_table->sci_sleep(20);
        s_lcm_func_table->sensor_setresetlevel(reset_level);
        s_lcm_func_table->sci_sleep(reset_width);
        s_lcm_func_table->sensor_setresetlevel((BOOLEAN)!reset_level);
        s_lcm_func_table->sci_sleep(120);
    }
    else
    {
        s_lcm_func_table->sci_sleep(20);
        s_lcm_func_table->sensor_setresetlevel(reset_level);
        s_lcm_func_table->sci_sleep(20);
        s_lcm_func_table->sensor_setvoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);
        s_lcm_func_table->sci_sleep(200);

	  s_lcm_func_table->sci_sleep(500);
    }

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: TLG1120 identify
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_Identify(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;
    uint16 value=0x00;
    
    s_lcm_func_table->trace("ATV: _TLG1120_Identify");

    TLG1120_IICReadReg(0x0146,&value);    
    if((0x0000!=value)
        &&(0xffff!=value))
    {
        s_lcm_func_table->trace("ATV: the atv is TLG1120");
    }
    else
    {
        s_lcm_func_table->trace("ATV: the atv is not TLG1120");
        rtn=SCI_ERROR;
    }

    return rtn;
}

/******************************************************************************/
// Description: TLG1120 init
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_Init(uint32 param)
{
    ATV_INFO_T_PTR atv_info_ptr=_TLG1120_GetInfo();

    s_lcm_func_table->trace("ATV:_TLG1120_Init");

    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;

    TLGAPP_Init(g_tlg1120_yuv_info.salve_i2c_addr_w);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: TLG1120 Sleep
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_Sleep(uint32 sleep_enter)
{
    s_lcm_func_table->trace("ATV:_TLG1120_Sleep");

    if(SCI_TRUE==sleep_enter)
    {
        TLGAPP_PowerDown();
    }
    else if(SCI_FALSE==sleep_enter)
    {
        TLGAPP_ReInitFromPowerDown();
    }

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: TLG1120 init scan channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_ScanChnInit(uint32 param)
{
    uint32 max_scan_num=0x00;

    p_tlg_channel_t(p_tlg_scan_map);

    s_lcm_func_table->trace("ATV:_TLG1120_ScanChnInit");

    p_tlg_scan_map=tlg_search_chn_map;

    while(1)
    {
        if(0x00==p_tlg_scan_map[max_scan_num]CHN_S)
        {
            break;
        }

        max_scan_num++;
    }

    //Scan chn init
    TLGAPP_StartChannelScan(p_tlg_scan_map, 0x00);
    //Clean chn map
    TLGAPP_InitScanParamUser(p_tlg_scan_map, max_scan_num);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: TLG1120 scan channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_ScanChn(uint32 param)
{
    uint32 rtn=SCI_FALSE;
    uint32 chn_id=0x00;
    uint32 is_valid=0x00;

    s_lcm_func_table->trace("ATV:_TLG1120_ScanChn");

    if(SCI_TRUE!=TLGAPP_ScanChnUser(&chn_id, &is_valid))
    {
        rtn=((chn_id<<0x10) &0xffff0000) |((is_valid&0x01) <<0x08);
    }
    else
    {
        TLGAPP_StopChannelScan();
        rtn=SCI_TRUE;
    }

    return rtn;
}

/******************************************************************************/
// Description: TLG1120 set channel
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_SetChn(uint32 param)
{
    uint32 rtn=SCI_ERROR;
    uint32 cur_volume_level=0x00;
    ATV_INFO_T_PTR atv_info_ptr=_TLG1120_GetInfo();

    s_lcm_func_table->trace("ATV:_TLG1120_SetChn : 0x%x", param);

    if(atv_info_ptr->atv_cur_chn==param)
    {
        return SCI_SUCCESS;
    }

    //turn off audio
    cur_volume_level=atv_info_ptr->cur_volume_level;
    _TLG1120_SetVolume(atv_info_ptr->cur_volume_level&0xffffff00);

    atv_info_ptr->atv_cur_chn=param;
    rtn=TLGAPP_SetChannel(param);
    s_lcm_func_table->sci_sleep(200);

    //turn on audio
    _TLG1120_SetVolume(cur_volume_level);

    return rtn;
}

/******************************************************************************/
// Description: TLG1120 get all chn num
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_GetAllChnNum(void)
{
    uint32 chn_num=0x00;

    s_lcm_func_table->trace("ATV:_TLG1120_GetAllChnNum");

    while(!(0x00==p_tlg_cur_map[chn_num]CHN_S))
    {
        chn_num++;
    }

    return chn_num;
}

/******************************************************************************/
// Description: TLG1120 set volume
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_SetVolume(uint32 param)
{
    uint32 volume_level=param&0xff;
    ATV_INFO_T_PTR atv_info_ptr=_TLG1120_GetInfo();

    s_lcm_func_table->trace("ATV:_TLG1120_SetVolume : 0x0%x", param);

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
        TLGAPP_SetTVAudioVolUser(ATV_VOLUME_0);
    }
    else
    {
        TLGAPP_SetTVAudioVolUser(ATV_VOLUME_8);
    }

    atv_info_ptr->cur_volume_level=volume_level;

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: TLG1120 set region
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_SetRegion(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_TLG1120_GetInfo();

    s_lcm_func_table->trace("ATV:_TLG1120_SetRegion");

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
            _TLG1120_InitChannel();
			
            break;
        }

        default :
        {
            atv_info_ptr->atv_cur_region=ATV_REGION_MAX;
            return SCI_ERROR;
        }
    }

    if(SCI_TRUE==_TLG1120_IsNTSCMode())
    {
        s_tlg1120_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width=704;
        s_tlg1120_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height=240;

        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=12;
        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=0;
        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=680;
        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=240;
    }
    else
    {
        s_tlg1120_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].width=720;
        s_tlg1120_resolution_Tab_YUV[SENSOR_MODE_PREVIEW_ONE].height=288;

        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_x=24;
        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_start_y=0;
        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_width=680;
        s_tlg1120_resolution_trim_tab[SENSOR_MODE_PREVIEW_ONE].trim_height=288;
    }

    s_lcm_func_table->sensor_setInfo();

    return rtn;
}

/******************************************************************************/
// Description: TLG1120 get rssi
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_GetRssi(void)
{
    uint16 rssi=0x00;
    uint32 rtn_rssi;

    s_lcm_func_table->trace("ATV:_TLG1120_GetRssi");

    TLG_GetTVSSI(TLGI2C_DEFAULT_BASE_ADDR, &rssi);

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
// Description: TLG1120 is ntsc mode
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
PUBLIC BOOLEAN _TLG1120_IsNTSCMode(void)
{
    BOOLEAN Is_NTSCMode=SCI_TRUE;
    ATV_INFO_T_PTR atv_info_ptr=_TLG1120_GetInfo();

    s_lcm_func_table->trace("ATV:_TLG1120_IsNTSCMode");

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
			 s_lcm_func_table->trace("ATV:_TLG1120_is NTSC");
            Is_NTSCMode=SCI_TRUE;
            break;

        default:
			 s_lcm_func_table->trace("ATV:_TLG1120_is PAL");
            Is_NTSCMode=SCI_FALSE;
            break;
    }

    return Is_NTSCMode;
}

/******************************************************************************/
// Description: TLG1120 get emc param
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_GetEmcParam(void)
{
    uint32 emc_param=0x00;

    emc_param=(s_emc_param[0]<<0x10)&0xffff0000;
    emc_param|=s_emc_param[1]&0xffff;

    return emc_param;
}

/******************************************************************************/
// Description: TLG1120 close
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_Close(void)
{
    uint32 rtn=SCI_SUCCESS;
    ATV_INFO_T_PTR atv_info_ptr=_TLG1120_GetInfo();

    s_lcm_func_table->trace("ATV:_TLG1120_Close");

    atv_info_ptr->atv_cur_chn=0xffffffff;
    atv_info_ptr->cur_volume_level=ATV_VOLUME_MAX;

    return rtn;
}

/******************************************************************************/
// Description: TLG1120 iic write
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 TLG1120_IICWriteReg(uint16 addr, uint16 data)
{
    TLGI2C_WriteReg(0x00, (uint32)addr, data);

    s_lcm_func_table->trace("ATV: tlg1120_WriteReg reg/value(0x%x,0x%x) !!", addr, data);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: TLG1120 iic read
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 TLG1120_IICReadReg(uint16 addr, uint16 *data)
{
    TLGI2C_ReadReg(0x00, (uint32)addr, data);

    s_lcm_func_table->trace("ATV: tlg1120_ReadReg reg/value(0x%x,0x%x) !!", addr, *data);

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: TLG1120 IOCTL
// Global resource dependence:
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _TLG1120_IOCTL(uint32 ctl_param)
{
    uint32 rtn=SCI_SUCCESS;
    uint32 cmd=(ctl_param>>0x10) &0xffff;
    uint32 param=ctl_param&0xffff;

    //s_lcm_func_table->trace("ATV:_TLG1120_IOCTL cmd:0x%x, param:0x%x",cmd,param);

    switch(cmd)
    {
        case ATV_CMD_CHIP_INIT:
        {
            rtn=_TLG1120_Init(param);
            break;
        }
        case ATV_CMD_CHIP_SLEEP:
        {
            rtn=_TLG1120_Sleep(param);
            break;
        }
        case ATV_CMD_SCAN_INIT:
        {
            rtn=_TLG1120_ScanChnInit(param);
            break;
        }
        case ATV_CMD_SCAN_CHN:
        {
            rtn=_TLG1120_ScanChn(param);
            break;
        }
        case ATV_CMD_SET_CHN:
        {
            rtn=_TLG1120_SetChn(param);
            break;
        }
        case ATV_CMD_SET_VOLUME:
        {
            rtn=_TLG1120_SetVolume(param);
            break;
        }
        case ATV_CMD_SET_REGION:
        {
            rtn=_TLG1120_SetRegion(param);
			   
            break;
        }
        case ATV_CMD_GET_RSSI:
        {
            rtn=_TLG1120_GetRssi();
            break;
        }
        case ATV_CMD_GET_ALL_CHN_NUM:
        {
            rtn=_TLG1120_GetAllChnNum();
            break;
        }
        case ATV_CMD_GET_IS_NTSC:
        {
            rtn=_TLG1120_IsNTSCMode();
            break;
        }
        case ATV_CMD_GET_EMC:
        {
            rtn=_TLG1120_GetEmcParam();
            break;
        }
        case ATV_CMD_GET_INFO:
        {
            rtn=(uint32)_TLG1120_GetInfo();
            break;
        }        
        case ATV_CMD_CLOSE:
        {
            rtn=_TLG1120_Close();
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
