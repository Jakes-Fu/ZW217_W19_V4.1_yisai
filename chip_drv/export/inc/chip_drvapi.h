/******************************************************************************
 ** File Name:      chip_drvapi.h                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/14/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This driver is for spreadtrum chip set.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/14/2002     Richard.Yang     Create.                                   *
 ** 09/16/2003     Xueliang.Wang    Modify CR4013                 *
 ******************************************************************************/
#ifndef _CHIP_DRVAPI_H_
#define _CHIP_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
//PLL clock
#define PLL_CLK_192M        192000000

#define CLK_DIV_1M            1000000

// ARM valid clock.
#define ARM_CLK_13M         13000000
#define ARM_CLK_20M         20000000
#define ARM_CLK_24M         24000000
#define ARM_CLK_26M         26000000
#define ARM_CLK_27M         27000000
#define ARM_CLK_28M         28800000
#define ARM_CLK_30M         30720000
#define ARM_CLK_32M         32000000
#define ARM_CLK_36M         36000000
#define ARM_CLK_39M         39000000
#define ARM_CLK_41M         41000000
#define ARM_CLK_48M         48000000
#define ARM_CLK_50M         50000000
#define ARM_CLK_52M         52000000
#define ARM_CLK_54M         54000000
#define ARM_CLK_60M         60000000
#define ARM_CLK_61M         61440000
#define ARM_CLK_64M         64000000
#define ARM_CLK_66M         66000000
#define ARM_CLK_78M         78000000
#define ARM_CLK_71M         71500000
#define ARM_CLK_72M         72000000
#define ARM_CLK_80M         80000000
#define ARM_CLK_82M         82000000
#define ARM_CLK_85M         85000000
#define ARM_CLK_90M         90000000
#define ARM_CLK_96M         96000000
#define ARM_CLK_100M       100000000
#define ARM_CLK_102M       102000000
#define ARM_CLK_104M       104000000
#define ARM_CLK_109M       109000000
#define ARM_CLK_120M       120000000
#define ARM_CLK_128M       128000000
#define ARM_CLK_133M	   133000000
#define ARM_CLK_153M       153600000
#define ARM_CLK_156M       156000000
#define ARM_CLK_164M       164000000
#define ARM_CLK_192M       192000000
#define ARM_CLK_200M       200000000
#define ARM_CLK_208M       208000000
#define ARM_CLK_249_6M   249600000
#define ARM_CLK_256M       256000000
#define ARM_CLK_312M       312000000
#define ARM_CLK_384M       384000000
#define ARM_CLK_400M       400000000
#define ARM_CLK_408M       408000000
#define ARM_CLK_512M       512000000
#define ARM_CLK_300M       300000000
#define ARM_CLK_150M       150000000
#define ARM_CLK_75M       75000000
#define PMIC_AC (0x8850a002)
#define PMIC_AB (0x8850a001)
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
typedef enum chip_serial_tag
{
    SERIAL_SC6600I,
    SERIAL_SC6600R,
    SERIAL_SC6600H,
    SERIAL_SC6800D,
    SERIAL_SC8800D,
    SERIAL_SC6600L,
    SERIAL_SC8800H,
    SERIAL_SC6800H,
    SERIAL_SC8800G,
    SERIAL_SC6530,
    SERIAL_SC6531EFM,
    SERIAL_UIX8910,
    CHIP_SERIAL_MAX
}
CHIP_SERIAL_E;

typedef enum chip_type_tag
{
    SC6600I_ID_BASE = SERIAL_SC6600I << 16,
    SC6600I,
    SC6600I_ID_MAX,
    SC6600R_ID_BASE = SERIAL_SC6600R << 16,
    SC6600R1,
    SC6600R2,
    SC6600R2A,
    SC6600R3,
    SC6600R3A,
    SC6600R_ID_MAX,
    SC6600H_ID_BASE = SERIAL_SC6600H << 16,
    SC6600H1,
    SC6600H2,
    SC6600H3,
    SC6600H_ID_MAX,
    SC6800D_ID_BASE = SERIAL_SC6800D << 16,
    SC6800D,
    SC6800D_ID_MAX,
    SC8800D_ID_BASE = SERIAL_SC8800D << 16,
    SC8800D,
    SC8800D_ID_MAX,
    SC6600L_ID_BASE = SERIAL_SC6600L << 16,
    SC6600L,
    SC6600L2,
    SC6600L7,
    SC6600H3A,
    SC6600W2,
    SC6600L6,
    SC6600L2A,
    SC6600L7A,
    SC6600L_ID_MAX,
    SC8800H_ID_BASE = SERIAL_SC8800H << 16,
    SC8800H,
    SC8800S3B,
    /* modified by zhengfei.xiao for SC8800H5 */
    SC8800H5,
    SC8800S4_AC,
    SC8800H5_AC,
    SC8800H_ID_MAX,
    SC6800H_ID_BASE = SERIAL_SC6800H << 16,
    SC6800H,
    SC6800H3,
    SC6800H_ID_MAX,
    SC8800G_ID_BASE = SERIAL_SC8800G << 16,
    SC8800G1,
    SC8800G2,
    SC8801G2,
    SC8802G2,
    SC6810,
    SC8800G_ID_MAX,
    SC6530_ID_BASE = SERIAL_SC6530 << 16,
    SC6530,
    SC6530C,
    SC6531,
    SC6530_ID_MAX,
    SC6531EFM_ID_BASE = SERIAL_SC6531EFM << 16,
    SC6531EFM,
    SC6531EFM_AB,
    SC6531EFM_ID_MAX,
    UIX8910_ID_BASE = SERIAL_UIX8910 << 16,
    UIX8910,
    UIX8910MPW,
    UIX8910_ID_MAX,
    UWS6121E,
    UWS6121E_MAX,
    CHIP_TYPE_MAX = 0x7FFFFFFF
} CHIP_TYPE_E;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
typedef enum chip_type_ex_tag
{
    CHIP_TYPE_EX_AA_AB,
    CHIP_TYPE_EX_AC,
    CHIP_TYPE_EX_MAX = 0x7FFFFFFF
} CHIP_TYPE_EX_E;
#endif

typedef enum _CHIP_ID
{
    CHIP_ID_SC6530 = 0x65300000,
    CHIP_ID_SC6530C = 0x6530C000,
    CHIP_ID_SC6531  = 0x65310000,
    CHIP_ID_SC6531BA= 0x65310001,
    CHIP_ID_SC6531EFM= 0x65620000,
    CHIP_ID_SC6531EFM_AB= 0x65620001,
    CHIP_ID_SR1130  = 0x11300000,
    CHIP_ID_SR1130C = 0x1130C000,
    CHIP_ID_SR1131  = 0x11310000,
    CHIP_ID_SR1131BA = 0x11310001,
    CHIP_ID_SC1161 = 0x1161A000,
    CHIP_ID_SC2720 = 0x2720A000,
    CHIP_ID_SC2720FF = 0x2720A001,
    CHIP_ID_SC2720FFECO = 0x2720A002,
    CHIP_ID_UIS8910 = 0x89100000,
    CHIP_ID_UWS6121E = 0x88500002,
}CHIP_ID_E;

//define dsp rom code id
typedef enum chip_dsp_rom_id_tag
{
    CHIP_DSP_ROM_V1,
    CHIP_DSP_ROM_V2,
    CHIP_DSP_ROM_V3,
    CHIP_DSP_ROM_ID_MAX
} CHIP_DSP_ROM_ID_E;

typedef enum module_clk
{
    CLK_MCU = 16,
    CLK_APB,
    CLK_DSP,
    CLK_CAL,
    CLK_12M,
    CLK_48M,
    CLK_CCIR,
    CLK_DCAM,
    CLK_IIS,
    CLK_VBC,
    CLK_CDC,
    CLK_AUX0,
    CLK_AUX1
} MODULE_CLK_E;

typedef enum module_clk_src
{
    CLKPLL = 0,
    CLKUPLL,
    CLK13M,
    CLK32K
} MODULE_CLK_SRC_E;

typedef struct _SYS_CLK_CFG_INFO
{
    uint32 pll_mn;
    uint32 upll_mn;
    uint32 dsp_arm_div;
} SYS_CLK_CFG_INFO;

typedef struct  _CHIP_INFO
{
	CHIP_ID_E chip_id;
    char info[16];
} CHIP_INFO;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
typedef struct  _CHIP_INFO_EX
{
	CHIP_TYPE_EX_E chip_type_ex;
    char info[16];
} CHIP_INFO_EX;
#endif

typedef enum _CLK_AUX_ID
{
    CLKAUX_ID_0,
    CLKAUX_ID_1
} CLKAUX_ID_E;

typedef enum _CLK_AUX_SRC
{
    CLKAUX_SRC_96M = 0,
    CLKAUX_SRC_76M = 1,
    CLKAUX_SRC_32K = 2,
    CLKAUX_SRC_26M = 3
} CLKAUX_SRC_E;
typedef enum _RF_CHIP_TYPE
{
	RF_CHIP_TYPE_AA = 0x0,
	RF_CHIP_TYPE_AB,
	RF_CHIP_TYPE_AC1,
	RF_CHIP_TYPE_AC2,
	RF_CHIP_TYPE_AC3,
	RF_CHIP_TYPE_AC4,
	RF_CHIP_TYPE_AD,
	RF_CHIP_TYPE_AD1,
	RF_CHIP_TYPE_MAX = 0xFF
}RF_CHIP_TYPE_E;
typedef enum _APLL_FTUNE_MODE
{
    APLL_FT_DEFAULT=0,   //DEFAULT 624M
    APLL_FT_P1D5,           // 624M+1.5M
    APLL_FT_M1D5,           //624M-1.5M
    APLL_FT_P3,           //624M+3M
    APLL_FT_M3,           //624M-3M
    APLL_FT_P4D5,       //624M+4.5M
    APLL_FT_M4D5,           //624M-4.5M
    APLL_FT_P6,           //624M+6M
    APLL_FT_M6         //624M-6M
} APLL_FTUNE_MODE_E;

#define	CHIP_TYPE_SC6531_AA				RF_CHIP_TYPE_AA
#define	CHIP_TYPE_SC6531_AB				RF_CHIP_TYPE_AB
#define	CHIP_TYPE_SC6531_AC1			RF_CHIP_TYPE_AC1
#define	CHIP_TYPE_SC6531_AC2			RF_CHIP_TYPE_AC2
#define	CHIP_TYPE_SC6531_AC3			RF_CHIP_TYPE_AC3
#define	CHIP_TYPE_SC6531_AC4			RF_CHIP_TYPE_AC4
#define	CHIP_TYPE_SC6531_AD				RF_CHIP_TYPE_AD
#define	CHIP_TYPE_SC6531_AD1			RF_CHIP_TYPE_AD1
#define	CHIP_TYPE_SC6531_BA				0x08
#define CHIP_TYPE_SC6531_MAX			RF_CHIP_TYPE_MAX

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to return chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetHWChipID (void);  // return chip ID number.

/*****************************************************************************/
//  Description:    The function is used to return psram type.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetPsramTypeFlag (void);

/*****************************************************************************/
//  Description:    The function is used to return ADIE chip ID (a uint32 number).
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/

PUBLIC uint32 CHIP_GetADIEChipID(void);


/*****************************************************************************/
//  Description:    The function is used to detect the memory type
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Return value:
//                1      CS0 connects to SDRAM
//                0      CS0 connects to FLASH
/*****************************************************************************/
PUBLIC uint32 CHIP_DetectMemType (void);

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetArmClk (void);
/*****************************************************************************/
// Description :    This function is used to get current AHB clock.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void);
/*****************************************************************************/
// Description :    This function is used to get current APB clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetAPBClk (void);
/*****************************************************************************/
// Description :    This function is used to get current PLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetPllClk (void);

/*****************************************************************************/
// Description :    This function is used to get current PLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetMPllClk (void);

void CHIP_ClkForNetSwitch (uint32 net_mode);

/*****************************************************************************/
// Description :    This function is used to get current VPLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           Daniel.ding add it for SC6800 .
/*****************************************************************************/
uint32 CHIP_GetVPllClk (void);


/*****************************************************************************/
// Description :    This function is used to tune current APLL clock according to specified tune mode
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/

void CHIP_FineTuneAPllClk(APLL_FTUNE_MODE_E clkft_mode);

/*****************************************************************************/
//  Description:    The function is used to return rf chip ID.
//  Global resource dependence:
//  Author:         Kui.Wang
//  Note:
/*****************************************************************************/
PUBLIC RF_CHIP_TYPE_E CHIP_GetRFChipType (void);
/*****************************************************************************/
//  Description:    The function is used to return rf & hw chip ID.
//  Global resource dependence:
//  Author:         Kui.Wang
//  Note:
/*****************************************************************************/
PUBLIC  uint8 CHIP_GetChipID (void);

/*****************************************************************************/
//  Description:    The function is used to return rf & hw chip ID.
//  Global resource dependence:  chip_id ->  7 6 5 | 4 3 2 1 <-0 rf_id
//  Author:         guoliang
//  Note:
/*****************************************************************************/
PUBLIC  uint8 CHIP_GetChipIDWithRF (void);


/*****************************************************************************/
//  Description:    The function is used to return chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
void CHIP_DisableFIQ (void);


/*****************************************************************************/
// Description :    This function is used to turn off the power.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_TurnOffPower (void);

/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_ResetMCU (void);

/*****************************************************************************/
// Description :    This function is used to reset MCU from boot code.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_BootReset (void);


/*****************************************************************************/
// Description :    This function is used to get current DSP clock.
// Global resource dependence :
// Author :         Benjamin.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetDspClk (void);

/*****************************************************************************/
// Description :    This function is used to get current VPLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           Aiguo.Miao update it for SC8800H .
/*****************************************************************************/
uint32 CHIP_GetTDPllClk (void);

/*********************************************************************************************/
//Description:   This function is used to initializtion system clk and other
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
void CHIP_Init (void);
PUBLIC void CHIP_Enable7sRstMode(void);
PUBLIC void CHIP_Enable7sRstMode_ALL(void);
/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    This function is used get the chip type ex
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_EX_E CHIP_GetChipTypeEx (void);
#endif
/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC const char *CHIP_GetChipInfo (void);

/*****************************************************************************/
// Description :    This function is used get the a die chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC const char *CHIP_GetADieChipInfo (void);

/*****************************************************************************/
// Description :    This function is active/deactive share-memory clock
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC void CHIP_ActiveSMCLK (BOOLEAN is_active);
/*****************************************************************************/
// Description :    This function is Get Usb DM Value
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_GetUsbDMValue (void);

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC void CHIP_InitHWChipID (void);

/*****************************************************************************/
//  Description:   This function is used to do something before assert.
//  Global resource dependence:
//  Author:         Zhengjiang.Lei
//  Note:
/*****************************************************************************/

PUBLIC void CHIP_BeforeAssert (void);

/*below are removed from driver_export.c, wait clean up*/
// The function is used to reset dsp when arm system enter assert mode
PUBLIC BOOLEAN CHIP_ResetDspInAssertMode (void);

// The function is used to reset and set dsp to output dsp assert log to pc by uart1
PUBLIC void CHIP_DspOutPutAssertInfo (void);

/******************************************************************************/
// Description:   Enable MCU can access switched-ram and shared-ram.
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:          Beforce invoking this function, the DSP/ISP should be hold on!
/******************************************************************************/
PUBLIC uint32 Chip_SwitchIRAM2Arm (void);
PUBLIC uint32 Chip_SwitchIRAMDefault (void);

/******************************************************************************/
// Description:   handle function when dsp exception
// Dependence:    NONE
// Author:
// Note:
/******************************************************************************/
PUBLIC void L1API_DSPExceptionHandle (void);
PUBLIC void PMU_Init();
PUBLIC uint32 PMU_GetPmicChipId(void);
PUBLIC void PMU_ReadPmicChipId(void);
extern SYS_CLK_CFG_INFO *Get_system_clk_cfg (void);
extern  void EMC_Close (void);
extern void __tx_reset (void);

/*********************************************************************************************/
//Description:   This function is used to initializtion chipset in calibration mode
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
PUBLIC int CHIP_Init_Calibration (void);
#else
LOCAL __inline int CHIP_Init_Calibration (void)
{
	return 0;
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _CHIP_DRVAPI_H_
