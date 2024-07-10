/******************************************************************************
 ** File Name:      ldo_cfg.c                                                 *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           07/10/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configuration for ldo management.*
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/10/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "adc_drvapi.h"
#include "ldo_drvapi.h"
#include "ldo_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
#define PWR_WR_PROT_MAGIC (0x6e7f)//0x6e7f is used to unlock write protect bit status


LDO_CTL_T ldo_ctl_data[] =
{
    {LDO_LDO_SD,             ANA_POWER_PD_SW0,      BIT_1, NULL},
    {LDO_LDO_SDIO,          ANA_POWER_PD_SW0,      BIT_1, NULL},
    {LDO_LDO_CORE,         ANA_POWER_PD_SW0,      BIT_2, NULL},
    {LDO_LDO_VRF,            ANA_POWER_PD_SW0,      BIT_3, NULL},
    {LDO_LDO_GEN,            ANA_POWER_PD_SW0,      BIT_4, NULL},
    {LDO_LDO_IO18,           ANA_POWER_PD_SW0,      BIT_5, NULL},
    {LDO_LDO_MEM,            ANA_POWER_PD_SW0,      BIT_6, NULL},
    {LDO_LDO_DCXO,         ANA_POWER_PD_SW0,      BIT_7, NULL},
    {LDO_LDO_CP,              ANA_POWER_PD_SW0,      BIT_8, NULL},
    {LDO_LDO_EMM,              ANA_POWER_PD_SW0,      BIT_9, NULL},
    {LDO_LDO_IO33,           ANA_POWER_PD_SW0,      BIT_10, NULL},
    {LDO_LDO_LP18,           ANA_POWER_PD_SW0,      BIT_11, NULL},
    {LDO_LDO_RF12,          ANA_POWER_PD_SW0,      BIT_12, NULL},
    {LDO_LDO_ANA,           ANA_POWER_PD_SW0,      BIT_13, NULL},
    {LDO_LDO_USB,           ANA_POWER_PD_SW0,      BIT_14, NULL},
    {LDO_LDO_SPIMEM,      ANA_POWER_PD_SW0,      BIT_15, NULL},
    {LDO_LDO_RF18,          ANA_POWER_PD_SW1,      BIT_3, NULL},
    {LDO_LDO_LCD,           ANA_POWER_PD_SW1,      BIT_8, NULL},
    {LDO_LDO_CAMD,         ANA_POWER_PD_SW1,      BIT_10, NULL},
//  {LDO_LDO_CAMIO,         ANA_POWER_PD_SW1,      BIT_10, NULL},
    {LDO_LDO_CAMA,          ANA_POWER_PD_SW1,      BIT_11, NULL},
    {LDO_LDO_SIM0,          ANA_SIM_VPA_CTRL0,      BIT_13, NULL},
    {LDO_LDO_SIM1,          ANA_SIM_VPA_CTRL0,      BIT_12, NULL},
    {LDO_LDO_VPA,          ANA_SIM_VPA_CTRL0,      BIT_0, NULL},

    {LDO_LDO_MAX,       NULL,                 NULL,  NULL}
};



LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{

    {SLP_LDO_CAMA,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMA_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMD,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMD_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
#ifdef ZDT_PCBA_ZW201_SUPPORT
    {SLP_LDO_SD,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSD_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
#else
    {SLP_LDO_SD,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSD_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
#endif
    {SLP_LDO_LCD,               ANA_SLP_LDO_PD_CTRL0,   SLP_LDOLCD_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    //{SLP_LDO_SD,                ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSD_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_USB,              ANA_SLP_LDO_PD_CTRL0,   SLP_LDOUSB_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SPIMEM,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSPIMEM_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_RF18,             ANA_SLP_LDO_PD_CTRL0,   SLP_LDORF18_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_IO33,             ANA_SLP_LDO_PD_CTRL0,   SLP_LDOVIO33_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_DCXO,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDODCXO_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_LP18,             ANA_SLP_LDO_PD_CTRL0,   SLP_LDOLP18_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_RF12,            ANA_SLP_LDO_PD_CTRL0,   SLP_LDORF12_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_ANA,              ANA_SLP_LDO_PD_CTRL0,   SLP_LDOANA_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_IO18,            ANA_SLP_LDO_PD_CTRL0,   SLP_LDOVIO18_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_MEM,             ANA_SLP_LDO_PD_CTRL1,   SLP_LDOMEM_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_CP,               ANA_SLP_LDO_PD_CTRL1,   SLP_LDOCP_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM0,            ANA_LDO_SIM_CTRL1,   SLP_LDOSIM0_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM1,            ANA_LDO_SIM_CTRL1,   SLP_LDOSIM1_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VRF,            ANA_SLP_DCDC_PD_CTRL,   SLP_DCDCVRF_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},   


//    #if !defined(POP_MDY_SUPPORT)
//    {SLP_LDO_VDD28,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOVDD28_PD_EN,     SLP_BIT_SET,        TRUE,   NULL},
//    #endif
	
#if defined(POP_MDY_SUPPORT)
     {SLP_LDO_KB,                ANA_SLP_LDO_PD_CTRL0,   SLP_LDOKPLED_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
#endif

    {SLP_LDO_MAX,           NULL,                   NULL,                   SLP_BIT_CLR,        TRUE,   NULL}
};

LOCAL CAL_LDO_T 	cal_ldo_data[] = {
	//{LDO_LDO_RF,    CAL_LDO_GROUP_B, CAL_LDO_RF,    8,   13, ANA_LDO_RF_REG1,    LDO_RF_V_MSK,    LDO_RF_V_SHIFT,    VDD1V8_DEFAULT, 0x60, 1200, 2000, 0x0,  0x7F, VDD1V8_DEFAULT, FALSE, FALSE,  FALSE}, //15 LDO_RF
	//{LDO_LDO_BTPA,  CAL_LDO_GROUP_B, CAL_LDO_BTPA,  8,   20, ANA_LDO_BTPA_REG1,  LDO_BTPA_V_MSK,  LDO_BTPA_V_SHIFT,  VDD3V3_DEFAULT, 0xd2, 1200, 3300, 0x0,  0xd2, VDD3V3_DEFAULT, TRUE,  FALSE,  FALSE}, //42 LDO_BTPA
	{LDO_LDO_SD,  CAL_LDO_GROUP_A, CAL_LDO_SD,    7,   50, ANA_LDO_VOSEL3,    LDO_SD_V_MSK,    LDO_SD_V_SHIFT,    VDD2V7_DEFAULT, 0x2b, 1625, 3225, 0x0,  0x3F, VDD2V7_DEFAULT, TRUE,  FALSE,  FALSE}, //29 LDO_SDIO
	{LDO_LDO_CAMA,  CAL_LDO_GROUP_A, CAL_LDO_CAMA,  50,   25, ANA_LDO_VOSEL1,  LDO_CAMA_V_MSK,  LDO_CAMA_V_SHIFT,  VDD2V8_DEFAULT, 0x2f, 1625, 3225, 0x0,  0x3F, VDD2V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
	{LDO_LDO_CAMD,  CAL_LDO_GROUP_A, CAL_LDO_CAMD,  7,   25, ANA_LDO_CAMD_CTRL,  LDO_CAMD_V_MSK,  LDO_CAMD_V_SHIFT,  VDD1V8_DEFAULT, 0x20, 1400,  2187, 0x0,  0x3F, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //31 LDO_CAMD
//	{LDO_LDO_CAMIO, CAL_LDO_GROUP_A, CAL_LDO_CAMIO, 7,   25, ANA_LDO_CAMIO_REG1, LDO_CAMIO_V_MSK, LDO_CAMIO_V_SHIFT, VDD1V8_DEFAULT, 0x20, 1400, 2187, 0x0,  0x3F, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
	{LDO_LDO_LCD,   CAL_LDO_GROUP_A, CAL_LDO_LCD,   7,   50, ANA_LDO_VOSEL4,   LDO_LCD_V_MSK,   LDO_LCD_V_SHIFT,   VDD1V8_DEFAULT, 0x7, 1625, 3225, 0x0,  0x3F, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
//    {LDO_LDO_VIBR,  CAL_LDO_GROUP_A, CAL_LDO_VIBR,  7,   200, ANA_VIBR_CTRL,      LDO_VIBR_V_MSK,  LDO_VIBR_V_SHIFT,  VDD3V0_DEFAULT, 0x02, 2800, 3300, 0x0,  0x07, VDD3V0_DEFAULT, TRUE,  FALSE,  FALSE}, //40 LDO_VIBR
//    {LDO_LDO_BTPHY, CAL_LDO_GROUP_A, CAL_LDO_BTPHY, 7,   25, ANA_LDO_BTPHY_REG1, LDO_BTPHY_V_MSK, LDO_BTPHY_V_SHIFT, VDD1V5_DEFAULT, 0x20, 1100, 1887, 0x0,  0x3F, VDD1V5_DEFAULT, TRUE,  FALSE,  FALSE}, //40 LDO_VIBR
    {LDO_LDO_MAX,   0,               0,             0,   0,  0,                  0,               0,                 0,              0,    0,    0,    0,    0,    0,              0,     0,     0}
};




/*****************************************************************************/
//  Description:  Ldo_Get_Cfg
//  Global resource dependence: NONE
//  Author:
//  Note:    Ldo_Get_Cfg
/*****************************************************************************/
PUBLIC LDO_CTL_PTR Ldo_Get_Cfg (void)
{
    LDO_CTL_PTR  ldo_ctl_tab = NULL;

    ldo_ctl_tab = ldo_ctl_data;

    return ldo_ctl_tab;
}

/*****************************************************************************/
//  Description:  Slp_Ldo_Get_Cfg
//  Global resource dependence: NONE
//  Author:
//  Note:    Slp_Ldo_Get_Cfg
/*****************************************************************************/
PUBLIC SLP_LDO_CTL_PTR Slp_Ldo_Get_Cfg (void)
{
    SLP_LDO_CTL_PTR  slp_ldo_ctl_tab = NULL;

    slp_ldo_ctl_tab = (SLP_LDO_CTL_PTR)slp_ldo_ctl_data;

    return slp_ldo_ctl_tab;
}

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC CAL_LDO_PTR Cal_Ldo_Get_Cfg (void)
{
    CAL_LDO_PTR  cal_ldo_data_tab = NULL;

    cal_ldo_data_tab = (CAL_LDO_PTR)cal_ldo_data;

    return cal_ldo_data_tab;
}

PUBLIC uint32 Cal_Ldo_Get_Tab_Size (void)
{
    uint32 cal_ldo_tab_size = 0;

    cal_ldo_tab_size = sizeof(cal_ldo_data)/sizeof(cal_ldo_data[0]);

    return cal_ldo_tab_size;
}

PUBLIC void prvUnlockPowerReg(void)
{
    ANA_REG_SET(ANA_PWR_WR_PROT_VALUE, PWR_WR_PROT_MAGIC);
    do
    {
        while (!(ANA_REG_GET(ANA_PWR_WR_PROT_VALUE) == 0x8000)){
            SCI_TraceLow("PWR_WR_PROT is on write protect bit status");
        }
    } while (0);
}
#endif

/**---------------------------------------------------------------------------*
 **                         Function Definitions                                                                               *
 **---------------------------------------------------------------------------*/

