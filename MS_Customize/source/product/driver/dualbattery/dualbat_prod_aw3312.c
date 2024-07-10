/******************************************************************************
 ** File Name:      dualbat_prod_aw3312.c                                     *
 ** Author:         Mingwei.Zhang                                             *
 ** DATE:           21/06/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic dual battery operation process.*
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                  *
 ** 21/06/2011          Mingwei.Zhang            Create.                      *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"
#include "dualbat_drvapi.h"
#include "adc_drvapi.h"
#include "gpio_prod_api.h"
#include "dualbat_prod.h"

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
#define GPIO_PROD_DBAT_SEL_BAT      77      //pls init it in pinmap_cfg.c 
#define GPIO_PROD_DBAT_NO_ID        81      //pls init it in pinmap_cfg.c
#define GPIO_PROD_DBAT_EN           78      //pls init it in pinmap_cfg.c
#define GPIO_PROD_DBAT_MUTUAL_CHR   80      //pls init it in pinmap_cfg.c



/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void DBAT_ProdInit(void);
LOCAL void DBAT_ProdPowerOff(void);
LOCAL BOOLEAN DBAT_ProdMutualCharge(DBAT_BAT_E need_chg_bat,BOOLEAN on);
LOCAL uint32 DBAT_ProdGetBatADC(DBAT_BAT_E bat);
LOCAL BOOLEAN DBAT_ProdSelectBat( DBAT_BAT_E bat);
LOCAL BOOLEAN DBAT_ProdGetBatPresent( DBAT_BAT_E bat);
LOCAL DBAT_BAT_E DBAT_ProdGetCurBat(void);
LOCAL void DBAT_ProdBatPlugCallBack(DBAT_BAT_E bat,BOOLEAN plugin);
LOCAL void DBAT_ProdChargeCallBack(BOOLEAN chg);
LOCAL void DBAT_ProdCalibrationModeInit(void);

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL const DBAT_PROD_FUN_T dbat_prod_fun =
{
    DBAT_ProdInit,
    DBAT_ProdPowerOff,
    DBAT_ProdMutualCharge,
    DBAT_ProdGetBatADC,
    DBAT_ProdSelectBat,
    DBAT_ProdGetBatPresent,
    DBAT_ProdGetCurBat,
    DBAT_ProdBatPlugCallBack,
    DBAT_ProdChargeCallBack,
    DBAT_ProdCalibrationModeInit,
};

/**---------------------------------------------------------------------------*
 **                     LOCAL Function Definitions                            *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:    Callback when system power on.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DBAT_ProdInit(void)
{
    GPIO_SetValue (GPIO_PROD_DBAT_NO_ID, SCI_FALSE);    //no_id = 1
}

/*****************************************************************************/
//  Description:    Callback when system power off.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DBAT_ProdPowerOff(void)
{
    return; //poweroff callback, be called when system poweroff.///reserved
}

/*****************************************************************************/
//  Description:    Perform mutual charge between two battery.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DBAT_ProdMutualCharge(DBAT_BAT_E need_chg_bat,BOOLEAN on)
{
/*
    if((need_chg_bat == DBAT_AUX_BAT)&&(SCI_TRUE == on))
    {
        GPIO_SetValue (GPIO_PROD_DBAT_NO_ID, SCI_TRUE);   //no_id = 0
    }
    else
    {
        GPIO_SetValue (GPIO_PROD_DBAT_NO_ID, SCI_FALSE);   //no_id = 1
    }
*/
    GPIO_SetValue (GPIO_PROD_DBAT_MUTUAL_CHR, on); 
}

/*****************************************************************************/
//  Description:    Get ADC value of battery voltage through ADC module API .
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 DBAT_ProdGetBatADC(DBAT_BAT_E bat)
{
    if(DBAT_MAIN_BAT == bat)
    {
        return ADC_GetResultDirectly (DBAT_ADC_CHANNEL_MAIN, ADC_SCALE_1V2);
    }
    else if(DBAT_AUX_BAT == bat)
    {
        return ADC_GetResultDirectly (DBAT_ADC_CHANNEL_AUX, ADC_SCALE_1V2);
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description:    select one battery to be used.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DBAT_ProdSelectBat( DBAT_BAT_E bat)
{
    if(DBAT_MAIN_BAT == bat)
    {
        GPIO_SetValue (GPIO_PROD_DBAT_SEL_BAT, SCI_TRUE); 
        return SCI_TRUE;
    }
    else if(DBAT_AUX_BAT == bat)
    {
        GPIO_SetValue (GPIO_PROD_DBAT_SEL_BAT, SCI_FALSE); 
        return SCI_TRUE;
    }
    return SCI_FALSE;
}

/*****************************************************************************/
//  Description:    Query battery present state.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DBAT_ProdGetBatPresent( DBAT_BAT_E bat)
{
    return SCI_FALSE;   ///reserved 
}

/*****************************************************************************/
//  Description:    Get current used battery number.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL DBAT_BAT_E DBAT_ProdGetCurBat(void)
{
    return SCI_FALSE;   ///reserved 
}

/*****************************************************************************/
//  Description:    Callback when battery plugin or plugout.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DBAT_ProdBatPlugCallBack(DBAT_BAT_E bat,BOOLEAN plugin)
{
    if((bat == DBAT_MAIN_BAT)&&(plugin == SCI_FALSE))   ///main battery plugout
    { 
        GPIO_SetValue (GPIO_PROD_DBAT_EN, SCI_FALSE); 
    }
    else
    {
        GPIO_SetValue (GPIO_PROD_DBAT_EN, SCI_TRUE); 
    }
}

/*****************************************************************************/
//  Description:    Callback when charge start or stop .
//                  SCI_TRUE:charge start;SCI_FALSE:charge stop
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DBAT_ProdChargeCallBack(BOOLEAN chg)
{
    if(chg)
    {
        GPIO_SetValue (GPIO_PROD_DBAT_NO_ID, SCI_TRUE);   //no_id = 0
    }
    else
    {
        GPIO_SetValue (GPIO_PROD_DBAT_NO_ID, SCI_FALSE);   //no_id = 1
    }
}

/*****************************************************************************/
//  Description:    Calibration mode callback when system init .
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DBAT_ProdCalibrationModeInit(void)
{
    GPIO_SetValue (GPIO_PROD_DBAT_NO_ID, SCI_TRUE);
    return ; 
}

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Definitions                           *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Get product function table,when system power on.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC DBAT_PROD_FUN_T* DBAT_Prod_GetFun(void)
{
    return (DBAT_PROD_FUN_T*)&dbat_prod_fun;
}
