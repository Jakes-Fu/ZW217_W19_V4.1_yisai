/******************************************************************************
 ** File Name:      nv_adaptive_cfg.c                                         *
 ** Author:         abe.yue                                          	      *
 ** DATE:           04/07/2020                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic configure about get adaptive  *
					version.												  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/07/2020     abe.yue			Create.                                                           *
 ******************************************************************************/

 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "nv_adaptive_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

 LOCAL VERSION_GPIO_CFG_T bard_id_cfg[] = {
	{9, BIT_0},				//GPIO9  crystal_type  0: DCXO   1 : TCXO
	{11, BIT_1},			//GPIO119 PA BOARD ID Bit0
	{12, BIT_2},			//GPIO117 PA BOARD ID Bit1  00: RPM6743 & RTM7916
	{0xFFFF,0}
};

LOCAL const int8 adc2deltanv_table[] = {
	-1, /* reserved */
	2, /* hw_ver02 */
	-1, /* reserved */
	0, /* hw_ver00.nv */
	-1, /* reserved */
	-1, /* reserved */
	-1, /* reserved */
	1 /* hw_ver01.nv */
};
	
#define RF1_LOW_VOLT_SIZE 952
#define RF1_HIGH_VOLT_SIZE 1230
#define RF2_LOW_VOLT_SIZE 663
#define RF2_HIGH_VOLT_SIZE 897
#define RF3_LOW_VOLT_SIZE 459
#define RF3_HIGH_VOLT_SIZE 621
#define RF4_LOW_VOLT_SIZE 306
#define RF4_HIGH_VOLT_SIZE 414
#define RF5_LOW_VOLT_SIZE 170
#define RF5_HIGH_VOLT_SIZE 230
#define RF6_LOW_VOLT_SIZE 115.6
#define RF6_HIGH_VOLT_SIZE 156.4
#define RF7_LOW_VOLT_SIZE 37.4
#define RF7_HIGH_VOLT_SIZE 50.6
#define RF8_LOW_VOLT_SIZE 5
#define RF8_HIGH_VOLT_SIZE 24

/**---------------------------------------------------------------------------*
 **                           Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function get adaptive value from gpio configuration
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
LOCAL uint8 PH_GetAdaptiveGpioValue(void)
{
	uint8 i = 0;
	uint8 result = 0;
	for(i = 0; bard_id_cfg[i].version_bit != 0; i++) {
		GPIO_SetDirection(bard_id_cfg[i].gpio_num, 0);
		if(GPIO_GetValue(bard_id_cfg[i].gpio_num)) {
			result |= bard_id_cfg[i].version_bit;
		}
	}
	SCI_TRACE_LOW("PH_GAGV: GPIO:%d\n", result);
	return result;
}

/*****************************************************************************/
//  Description:    This function get adaptive value from ADC configuration
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
LOCAL int8 PH_GetAdaptiveADCValue(void)
{
    uint32 vol= 0 ;
    uint8 rf_band = 0xff;
    uint32 adc;

    adc = ADC_GetVoltageByCurrent_Extend(ADIN_3, ADC_SCALE_1V250, 2000, 10);    //set isne is 20uA,get val

    //SCI_TraceLow("PH_GetAdaptiveADCValue: adc=%d\n",adc);
    vol =  (uint32)CHGMNG_AdcvalueToVoltageforSmallScale (adc);
    //SCI_TraceLow("PH_GetAdaptiveADCValue: vol=%d\n",vol);	
    if (vol < 5 || vol > 1230) {
		SCI_TRACE_LOW("adc error\n");
        return -1;
    }
    if (vol >= RF1_LOW_VOLT_SIZE && vol <= RF1_HIGH_VOLT_SIZE)
        rf_band = 7;
    else if (vol >= RF2_LOW_VOLT_SIZE && vol <= RF2_HIGH_VOLT_SIZE)
        rf_band = 6;
    else if (vol >= RF3_LOW_VOLT_SIZE && vol <= RF3_HIGH_VOLT_SIZE)
        rf_band = 5;
    else if (vol >= RF4_LOW_VOLT_SIZE && vol <= RF4_HIGH_VOLT_SIZE)
        rf_band = 4;
    else if (vol >= RF5_LOW_VOLT_SIZE && vol <= RF5_HIGH_VOLT_SIZE)
        rf_band = 3;
    else if (vol >= RF6_LOW_VOLT_SIZE && vol <= RF6_HIGH_VOLT_SIZE)
        rf_band = 2;
    else if (vol >= RF7_LOW_VOLT_SIZE && vol <= RF7_HIGH_VOLT_SIZE)
        rf_band = 1;
    else if (vol >= RF8_LOW_VOLT_SIZE && vol <= RF8_HIGH_VOLT_SIZE)
        rf_band = 0;

    if(rf_band < 0 || rf_band > 7) {
		SCI_TRACE_LOW("rf band error\n");
        return -1;
    }

    return adc2deltanv_table[rf_band];
}


/*****************************************************************************/
//  Description:    This function get hw version from gpio and adc
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_Get_RF_VERSION(void)
{
	uint32 hw_version = 0;
#ifdef FORCECHANGE_SUPPORT
	hw_version = PH_GetAdaptiveADCValue();
#endif
	return hw_version;
}

PUBLIC uint32 SCI_Get_PA_VERSION(void)
{
	uint32 PA_version = 0;
#ifdef FORCECHANGE_SUPPORT
	PA_version = (PH_GetAdaptiveGpioValue() & ( bard_id_cfg[1].version_bit | bard_id_cfg[2].version_bit )) >>  bard_id_cfg[1].version_bit;
#endif
	return PA_version;
}

PUBLIC crystal_type_t SCI_Get_Crystal_Type(void)
{
	uint32 result = 0;
	crystal_type_t crystal_type = DCXO;
#ifdef FORCECHANGE_SUPPORT
	result = PH_GetAdaptiveGpioValue();
	if(result == 0) {
		crystal_type = DCXO;
	} else if(result == 1) {
		crystal_type = TSX;
	}
#endif
	return crystal_type;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of nv_adaptive_cfg.c
