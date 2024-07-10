/******************************************************************************
 ** File Name:      sc6531efm_freq_cfg.c                                                    *
 ** Author:         Yuhua.Shi                                              *
 ** DATE:           08/17/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of FREQ module.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/17/2010     Yuhua.Shi     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "chng_freq_drvapi.h"
#include "sc6531efm_freq_cfg.h"
#include "chip_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
//below defines the 4 level clock which arm/ahb clock can be specified.
#define ARM_AHB_CLK_MAX_LEVEL        ARM_CLK_LVL5
#define ARM_AHB_CLK_MIN_LEVEL        ARM_CLK_LVL2
#define CHNG_FREQ_IS_ENABLE          FALSE
#define AHBMASTER_EVENT_IS_ENABLE    FALSE

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/
//arm/ahb clk table
LOCAL const FREQ_CLK_LVL_T g_clk_lvl_table[ARM_AHB_CLK_MAX_LEVEL] =
{
    {ARM_CLK_26M,  ARM_CLK_26M, ARM_CLK_26M},
    {ARM_CLK_50M,  ARM_CLK_50M, ARM_CLK_50M},
    {ARM_CLK_66M,  ARM_CLK_66M, ARM_CLK_66M},
    {ARM_CLK_100M, ARM_CLK_100M,ARM_CLK_100M},
    {ARM_CLK_200M, ARM_CLK_100M,ARM_CLK_200M},
};

//arm/ahb/emc  clk devider table
LOCAL const uint32 g_clk_div_table[ARM_AHB_CLK_MAX_LEVEL] =
{
    0x0,        //use xtl, 26Mhz
    0x3C7,
    0x2C5,
    0x1C3,
    0x0C9,
};

//define your frequency-changing MACRO here for your task which need to change frequency.
LOCAL const FREQ_APP_CLK_LVL_T s_app_clk_lvl_table[] =
{
    {FREQ_INDEX_GENERAL,            ARM_CLK_LVL5},//general freq index can be used for general application
    {FREQ_INDEX_MP3,                ARM_CLK_LVL3}, //mp3 freq index
    {FREQ_INDEX_MIDI,               ARM_CLK_LVL5},  //midi freq index
    {FREQ_INDEX_AAC,                ARM_CLK_LVL4},  //aac freq index
    {FREQ_INDEX_DC_LOW,             ARM_CLK_LVL3}, //dc low freq index
    {FREQ_INDEX_DC_HIGH,            ARM_CLK_LVL5}, //dc high freq index
    {FREQ_INDEX_ISP_LOW,            ARM_CLK_LVL3},  //isp low freq index
    {FREQ_INDEX_ISP_HIGH,           ARM_CLK_LVL5},  //isp high freq index
    {FREQ_INDEX_MP4_PLAY_LVL1,      ARM_CLK_LVL2},  //mp4 play level1 freq index
    {FREQ_INDEX_MP4_PLAY_LVL2,      ARM_CLK_LVL3},  //mp4 play level2 freq index
    {FREQ_INDEX_MP4_PLAY_LVL3,      ARM_CLK_LVL4},  //mp4 play level3 freq index
    {FREQ_INDEX_MP4_PLAY_LVL4,      ARM_CLK_LVL5},  //mp4 play level4 freq index
    {FREQ_INDEX_MP4_REC,            ARM_CLK_LVL5},//mp4 record freq index
    {FREQ_INDEX_USB_LOW,            ARM_CLK_LVL2},//usb low freq index
    {FREQ_INDEX_USB_MID,            ARM_CLK_LVL3},//usb middle freq index
    {FREQ_INDEX_USB_HIGH,           ARM_CLK_LVL4},   //usb high freq index
    {FREQ_INDEX_PCM_BLUE_PLAY,      ARM_CLK_LVL3},  //pcm blue play freq index
    {FREQ_INDEX_BT_IDLE,            ARM_CLK_LVL2},//bt idle freq index
    {FREQ_INDEX_BT_FTP,             ARM_CLK_LVL5},//bt ftp freq index
    {FREQ_INDEX_BT_HFG,             ARM_CLK_LVL5},//bt hfg freq index
    {FREQ_INDEX_BT_A2DP,            ARM_CLK_LVL5},//bt a2dp freq index
    {FREQ_INDEX_ATC,                ARM_CLK_LVL5},//atc freq index
    {FREQ_INDEX_MAX,                NULL} //end
};

//define the control infomation for change frequency
LOCAL const FREQ_CONTROL_T s_freq_control_info =
{
    ARM_AHB_CLK_MIN_LEVEL,
    ARM_AHB_CLK_MAX_LEVEL,
    CHNG_FREQ_IS_ENABLE,
    AHBMASTER_EVENT_IS_ENABLE
};

//define the config info for change frequency
LOCAL const CHNG_FREQ_CONFIG_T s_chng_freq_config =
{
    (FREQ_CLK_LVL_PTR) g_clk_lvl_table,
    (uint32 *) g_clk_div_table,
    (FREQ_APP_CLK_LVL_PTR) s_app_clk_lvl_table,
    (FREQ_CONTROL_PTR) &s_freq_control_info
};


/**---------------------------------------------------------------------------*
 **                     LOCAL Function Prototypes                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function defines                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :    This function is used to get the config info about change frequency
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHNG_FREQ_CONFIG_PTR FREQ_GetFreqCfg (
    void
)
{
    return (CHNG_FREQ_CONFIG_PTR) &s_chng_freq_config;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif

/*  End Of File */
