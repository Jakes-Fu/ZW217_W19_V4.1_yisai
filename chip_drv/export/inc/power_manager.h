/******************************************************************************
 ** File Name:      power_manager.h                                                    *
 ** Author:         Zhengjiang.Lei                                              *
 ** DATE:           08/21/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on power manager.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/21/2006     Zhengjiang.Lei     Create.                                   *
 ******************************************************************************/
#ifndef _POWER_MANAGER_H_
#define _POWER_MANAGER_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip.h"

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
typedef enum
{
    CORE_VOLTAGE_1800MV = 0,
    CORE_VOLTAGE_1950MV,
    CORE_VOLTAGE_1500MV,
    CORE_VOLTAGE_1650MV
}
CORE_VOLTAGE_E;

typedef enum
{
    CORE_REF_VOLTAGE_1200MV = 0,
    CORE_REF_VOLTAGE_1130MV
} CORE_REF_VOLTAGE_E;

#define MAX_PLL_TREE_NUM        3     //define the max pll our chip suport.

typedef enum
{
    PLLTYPE_M = 0,
    PLLTYPE_V,
    PLLTYPE_A,
    PLLTYPE_MAX
} PLL_TYPE_E;


//define the the clk source select struct for relative device clocks.




// define pll pd src control by arm or dsp.
typedef enum
{
    ARM_CTL = 0,
    DSP_CTL
} PLL_PD_SRC_E;

//define error code
typedef enum
{
    RET_NO_ERR = 0,
    REQ_EXISTED,
    REQ_NOT_EXIST,
    OTHER_MODULE_FBDDN,
    SYNC_FREQ_FBDDN,
    NOT_SUPPORT_PLL
} PWRMNG_ERROR_CODE_E;

// define  pll_sync_freq_limit.
#define PLL_SYNC_48M            48000000
#define PLL_SYNC_27M            27000000
#define PLL_SYNC_12M            12000000
#define PLL_SYNC_6M             6000000
#define PLL_SYNC_NULL           1               // no sync limit to pll for this device 

//define pll_min_value_limit and pll_max_value_limit
#define PLL_CLK_MIN_VALUE       60000000        //60 MHz
#define PLL_CLK_MAX_VALUE       240000000       //240 MHz

// define device id
typedef enum
{
    PLL_DEVICE_NULL = 0,
    PLL_DEVICE_DSP_ID,
    PLL_DEVICE_ISP_ID,
    PLL_DEVICE_LCDC_ID,
    PLL_DEVICE_CCIR_ID,
    PLL_DEVICE_12M_ID,
    PLL_DEVICE_48M_ID,
    PLL_DEVICE_AUX0_ID,
    PLL_DEVICE_AUX1_ID,
    PLL_DEVICE_VB_ID,
    PLL_DEVICE_IIS_ID,
    PLL_DEVICE_PCM_ID,
    PLL_DEVICE_TV_ID,
    PLL_DEVICE_MAX_ID       // max device id
} PLL_DEVICE_ID_E;

typedef struct _PLL_DEVICE_REQ_STRUCT
{
    PLL_DEVICE_ID_E device_id;          //device id
    char *device_name;          //device name
    PLL_TYPE_E pll_type;            //MPLL,VPLL or APLL
    BOOLEAN b_pmt_chng_pll; //whether this device permit other device to change pll.
    uint32 pll_syncfreq_lmt;    //define the pll_clk value for this device,such as 48M or 27M.

    struct _PLL_DEVICE_REQ_STRUCT *pNext;
    struct _PLL_DEVICE_REQ_STRUCT *pPrev;
} PLL_DEVICE_REQ_T, *PLL_DEVICE_REQ_LIST_PTR;


typedef union _clk_pll_src_sel_tag
{
    struct _clk_pll_src_sel_map
    {
        volatile uint resv          : 21;   //reserved for more device clk src select.
        volatile uint pcm_sel       : 1;    // 0: APLL
        volatile uint iis_sel       : 1;    // 0: APLL
        volatile uint vb_sel        : 1;    // 0: APLL
        volatile uint aux1_sel      : 1;    // 0: APLL, 1: MPLL
        volatile uint aux0_sel      : 1;    // 0: APLL, 1: MPLL
        volatile uint clk48M_sel    : 1;    // 0: VPLL, 1: MPLL
        volatile uint clk12M_sel    : 1;    // 0: VPLL, 1: MPLL
        volatile uint ccir_sel      : 1;    // 0: MPLL, 1: VPLL
        volatile uint lcdc_sel      : 1;    // 0: VPLL, 1: MPLL
        volatile uint isp_sel       : 1;    // 0: MPLL, 1: VPLL
        volatile uint dsp_sel       : 1;    // 0: VPLL, 1: MPLL
    } mBits;
    volatile uint32 dwValue;
} CLK_PLL_SRC_SEL_U;
// according to the chip, define the device clk src select bit below... for sc6800, see 0x8b000060
#define CLK_DSP_SRC_VPLL        0
#define CLK_DSP_SRC_MPLL        1

#define CLK_ISP_SRC_MPLL        0
#define CLK_ISP_SRC_VPLL        1

#define CLK_LCDC_SRC_VPLL       0
#define CLK_LCDC_SRC_MPLL       1

#define CLK_CCIR_SRC_MPLL       0
#define CLK_CCIR_SRC_VPLL       1

#define CLK_12M_SRC_VPLL        0
#define CLK_12M_SRC_MPLL        1

#define CLK_48M_SRC_VPLL        0
#define CLK_48M_SRC_MPLL        1

#define CLK_AUX0_SRC_APLL       0
#define CLK_AUX0_SRC_MPLL       1

#define CLK_AUX1_SRC_APLL       0
#define CLK_AUX1_SRC_MPLL       1

#define CLK_VB_SRC_APLL         0
#define CLK_IIS_SRC_APLL        0
#define CLK_PCM_SRC_APLL        0

//define max number of device registered on the total 3 PLL clk tree.
#define MAX_PLL_DEVICE_REQ_NUM          30

typedef struct _PLL_DEVICE_REQ_MEM_STRUCT
{
    PLL_DEVICE_REQ_T stPLL_Device_Req;
    uint32           valid_flag;
} PLL_DEVICE_REQ_MEM_T;
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to init the initial power manager
//                  module, mainly clk src select for all device.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           select MPLL or VPLL or APLL to be the clock source.
/*****************************************************************************/
PUBLIC void PWRMNG_Init (void);

/*****************************************************************************/
// Description :    This function is to unregister a pll device request block from
//                  the PLL device request list.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC PWRMNG_ERROR_CODE_E PWRMNG_UnRegPllDeviceReq (PLL_DEVICE_REQ_LIST_PTR pNode);

/*****************************************************************************/
// Description :    This function is to register a pll device request block to
//                  the PLL device request list.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC PWRMNG_ERROR_CODE_E PWRMNG_RegPllDeviceReq (PLL_DEVICE_REQ_LIST_PTR pNode);

/*****************************************************************************/
// Description :    This function is used to print the selected link table.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Input:
//     plltype:     the pll type
/*****************************************************************************/
PUBLIC void PWRMNG_PrintLktbl (PLL_TYPE_E pll_type);

/*****************************************************************************/
//  Description:    The function change the core voltage
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Note:           when set core voltage,please refer to the core ref voltage you
//                  use now,,and then set relative vdd core voltage value.
/*****************************************************************************/
PUBLIC void PWRMNG_ChangeCoreVoltage (CORE_VOLTAGE_E core_voltage);

/*****************************************************************************/
//  Description:    The function change the analog voltage
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Note:
/*****************************************************************************/
PUBLIC void PWRMNG_ChangeAnalogVoltage (BOOLEAN bAnaVol);

/*****************************************************************************/
// Description :    This function is used to turn off the power.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_TurnOffPower (void);

PUBLIC void PWRMNG_SetApllPdSrc (PLL_PD_SRC_E pdsrc);

PUBLIC void PWRMNG_SetVpllPdSrc (PLL_PD_SRC_E pdsrc);

/*****************************************************************************/
// Description :    This function is used to poweron VPLL clock.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_ForcePowerOnVPll (void);

/*****************************************************************************/
// Description :    This function is used to poweron APLL clock.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_ForcePowerOnAPll (void);


/*****************************************************************************/
//  Description:    The function get the core voltage
//  Global resource dependence:
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC CORE_VOLTAGE_E PWRMNG_GetCoreVoltage (void);

PUBLIC CLK_PLL_SRC_SEL_U PWRMNG_GetPllSrcSelMode (void);

PUBLIC void PWRMNG_LCDC_ClkSwtch (BOOLEAN b_clk_on);

/*****************************************************************************/
// Description :    This function is used to change VPLL clock.
// Global resource dependence :
// Author :         Nick.Zhao
// parameter:
//                  pNode : the pll request node ptr which point to the device want
//                          to change vpll.
//                  vpll_clk: unit is Hz, like 192000000
//Note:             when a pll req device want to change the pll, make sure this device
//                  has been a node on the pll req list already first.
/*****************************************************************************/
PUBLIC PWRMNG_ERROR_CODE_E PWRMNG_SetVPll_Req (PLL_DEVICE_REQ_LIST_PTR pNode, uint32 vpll_clk);

PUBLIC void PWRMNG_TVE_ClkSwtch (BOOLEAN b_clk_on);

PUBLIC void PWRMNG_USBD_ClkSwtch (BOOLEAN b_clk_on);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _POWER_MANAGER_H
