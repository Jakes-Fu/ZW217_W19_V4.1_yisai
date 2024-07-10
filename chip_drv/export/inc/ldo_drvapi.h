/******************************************************************************
 ** File Name:      ldo_drvapi.h                                             *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           07/09/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/09/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/
#ifndef _LDO_DRVAPI_H_
#define _LDO_DRVAPI_H_

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
    LDO_LDO_NULL  = 0,   //id for NULL
    LDO_LDO_SIM0    ,     //VDD SIM0
    LDO_LDO_SIM1    ,     //VDD SIM1    
    LDO_LDO_LCD,
    LDO_LDO_IO18     ,       //VDDIO18
    LDO_LDO_IO33     ,       //VDDIO28
    LDO_LDO_LP18     ,  //LDOLP18
    LDO_LDO_USB     ,     //VDDUSB
    LDO_LDO_RF18      ,      //LDORF18
    LDO_LDO_RF12,      //LDORF12
    LDO_LDO_CAMA    ,
    LDO_LDO_CAMD    ,
    LDO_LDO_SD,          //VDDMMC
    LDO_LDO_SDIO,          //VDDMMC
    LDO_LDO_MEM,
    LDO_LDO_SPIMEM,
    LDO_LDO_DCXO,
    LDO_LDO_CORE    ,      //VDDCORE
    LDO_LDO_ANA    ,      //VDDANA
    LDO_LDO_CP,            //charge pump
    LDO_LDO_GEN,
    LDO_LDO_VRF,
    LDO_LDO_VPA,
    LDO_LDO_BTPHY,
    LDO_LDO_AUX2,
    LDO_LDO_EMM,
    HAL_POWER_MEM,
    HAL_POWER_DCDC_GEN,
    HAL_POWER_VIO33,

    LDO_LDO_MAX     //id for calculate LDO number only!
}
LDO_ID_E;

#define LDO_LDO_LCM     LDO_LDO_AUX1
#define LDO_LDO_CMR LDO_LDO_AUX2

typedef enum
{
    SLP_LDO_NULL  = 0,  //id for NULL
    SLP_LDO_SIM0    ,     //VDD SIM0
    SLP_LDO_SIM1    ,     //VDD SIM1    
    SLP_LDO_LCD,
    SLP_LDO_IO18     ,       //VDDIO18
    SLP_LDO_IO33     ,       //VDDIO28
    SLP_LDO_LP18     ,  //LDOLP18
    SLP_LDO_USB     ,     //VDDUSB
    SLP_LDO_RF18      ,      //LDORF18
    SLP_LDO_RF12,      //LDORF12
    SLP_LDO_CAMA    ,
    SLP_LDO_CAMD    ,
    SLP_LDO_SD,          //VDDMMC
    SLP_LDO_MEM,
    SLP_LDO_SPIMEM,
    SLP_LDO_DCXO,
    SLP_LDO_CORE    ,      //VDDCORE
    SLP_LDO_ANA    ,      //VDDANA
    SLP_LDO_CP,            //charge pump
    SLP_LDO_GEN,
    SLP_LDO_VRF,
    SLP_LDO_VPA,
    SLP_LDO_KB,


    SLP_LDO_MAX
} SLP_LDO_E;

typedef enum
{
    LDO_VOLT_LEVEL0 = 0,
    LDO_VOLT_LEVEL1,
    LDO_VOLT_LEVEL2,
    LDO_VOLT_LEVEL3,
    LDO_VOLT_LEVEL4,
    LDO_VOLT_LEVEL5,
    LDO_VOLT_LEVEL6,
    LDO_VOLT_LEVEL7,
    LDO_VOLT_LEVEL_MAX
} LDO_VOLT_LEVEL_E;

//LDO error flag definition
typedef enum
{
    LDO_ERR_OK,
    LDO_ERR_INVALID_HANDLE,
    LDO_ERR_INVALID_OPERATION,
    LDO_ERR_ERR
} LDO_ERR_E;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
//LDO calibration
typedef enum
{
    CAL_LDOB_DISABLE = 0,
    CAL_LDO_CORE,
    CAL_LDO_RF,
    CAL_LDO_BTPA,
    CAL_LDO_DCXO,
    CAL_LDO_VDD18,
    CAL_LDO_VDD28,
    CAL_LDOB_REF = 9,
    CAL_LDO_DCXO_REF
} CAL_LDOB_CHANNEL_E;

typedef enum
{
    CAL_LDOA_DISABLE = 0,
    CAL_LDO_SD,
    CAL_LDO_CAMA,
    CAL_LDO_CAMD,
#if defined(PLATFORM_UWS6121E)
    CAL_LDO_CAMIO,
    CAL_LDO_CAMMOT,
#endif
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E) 
    CAL_LDO_LCD,
    CAL_LDO_BTPHY,
#endif
    CAL_LDO_VIBR,
    CAL_LDO_SIM0,
    CAL_LDO_SIM1,
    CAL_LDO_SF,
    CAL_LDO_USB,
    CAL_LDOA_REF
} CAL_LDOA_CHANNEL_E;

typedef enum
{
    CAL_LDO_GROUP_A,
	CAL_LDO_GROUP_B
} CAL_LDO_GROUP_E;
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    this function is used to initialize LDO voltage level.
//  Global resource dependence:
//  Author: jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_Init (void);

/*****************************************************************************/
//  Description:  Turn on the LDO specified by input parameter ldo_id
//  Global resource dependence: NONE
//  Author:  jiexia.yu
//  Note:    return value = LDO_ERR_OK if operation is executed successfully
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_TurnOnLDO (LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description:  Turo off the LDO specified by parameter ldo_id
//  Global resource dependence: NONE
//  Author: jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_TurnOffLDO (LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description: Find the LDO status -- ON or OFF
//  Global resource dependence:
//  Author: jiexia.yu
//  Note: return SCI_TRUE means LDO is ON, SCI_FALSE is OFF
/*****************************************************************************/
PUBLIC BOOLEAN LDO_IsLDOOn (LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description: Get LDO voltage level
//  Global resource dependence:
//  Author: jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC LDO_VOLT_LEVEL_E LDO_GetVoltLevel (LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description:  This function could be invoked by customer for LDO voltage
//                level & referece voltage level initiation.
//  Global resource dependence:
//  Author: jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_SetVoltLevel (LDO_ID_E ldo_id, LDO_VOLT_LEVEL_E level);

/*****************************************************************************/
//  Description:  Control The VCAM Domain PAD to avoid VCAM Domain PAD input not floating
//  Global resource dependence: NONE
//  Author:  Tao.Feng && Yi.Qiu
//  Note:    return value = LDO_ERR_OK if operation is executed successfully
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_ControVCAMPad (BOOLEAN pad_on);

/*****************************************************************************/
//  Description:  Shut down any LDO that do not used when system enters deepsleep
//  Global resource dependence: s_ldo_reopen[]
//  Author: jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC void LDO_DeepSleepInit (void);


/*****************************************************************************/
//  Description:  Control the status of the LDO in deepsleep specified by input parameter ldo_id in
//  Global resource dependence: NONE
//  Author:  yuhua
// input: ldo_id, id of the ldo
//          auto_close, TRUE: auto close the ldo in deepsleep
//          auto_close, FALSE, do nothing in deepsleep
//  Note:    return value = LDO_ERR_OK if operation is executed successfully
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_AutoCloseInDeepSleep (SLP_LDO_E ldo_id, BOOLEAN auto_close);

/*****************************************************************************/
//  Description:  Shut down all LDO when system poweroff
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LDO_TurnOffAllLDO (void);

/*****************************************************************************/
//  Description: Set LDO voltage
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_SetVoltValue (LDO_ID_E ldo_id, uint32 volt_val);

/*****************************************************************************/
//  Description: Set LDO voltage
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 LDO_GetVoltValue (LDO_ID_E ldo_id);


#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC void LDO_Calibration(void);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _LDO_DRVAPI_H_

