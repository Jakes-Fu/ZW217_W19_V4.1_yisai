/******************************************************************************
 ** File Name:      gpio_ext_drv.h                                            *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           03/08/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DVB extend GPIO device.                                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2006     Liangwen.Zhen     Create.                                  *
 ******************************************************************************/

#ifndef _GPIO_EXT_DVR_H
#define _GPIO_EXT_DVR_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "tb_hal.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                Basic type and interface define                            *
 **---------------------------------------------------------------------------*/  
 
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define GPIO_EXT_DEV_1_NUM			(0x00)
#define GPIO_EXT_DEV_2_NUM			(0x01)
#define GPIO_EXT_DEV_3_NUM			(0x02)
#define GPIO_EXT_DEV_MAX_CNT		(0x03)


#define GPIO_OUTPUT					SCI_TRUE
#define GPIO_INPUT					SCI_FALSE
  
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

// GPIO extend table
typedef void(*GPIO_EXT_CALLBACK_PFUNC)(uint32 gpio_id, uint32 gpio_state);


typedef enum
{
	// IC 1
	GPIO_EXT_1_0 = (GPIO_EXT_DEV_1_NUM << 8),
	GPIO_EXT_1_1,
	GPIO_EXT_1_2,
	GPIO_EXT_1_3,
	GPIO_EXT_1_4,
	GPIO_EXT_1_5,
	GPIO_EXT_1_6,
	GPIO_EXT_1_7,
	GPIO_EXT_1_10,
	GPIO_EXT_1_11,
	GPIO_EXT_1_12,
	GPIO_EXT_1_13,
	GPIO_EXT_1_14,
	GPIO_EXT_1_15,
	GPIO_EXT_1_16,
	GPIO_EXT_1_17,
	
	// IC 2
	GPIO_EXT_2_0 = (GPIO_EXT_DEV_2_NUM << 8),
	GPIO_EXT_2_1,
	GPIO_EXT_2_2,
	GPIO_EXT_2_3,
	GPIO_EXT_2_4,
	GPIO_EXT_2_5,
	GPIO_EXT_2_6,
	GPIO_EXT_2_7,
	GPIO_EXT_2_10,
	GPIO_EXT_2_11,
	GPIO_EXT_2_12,
	GPIO_EXT_2_13,
	GPIO_EXT_2_14,
	GPIO_EXT_2_15,
	GPIO_EXT_2_16,
	GPIO_EXT_2_17,
	
	// IC 3
	GPIO_EXT_3_0 = (GPIO_EXT_DEV_3_NUM << 8),
	GPIO_EXT_3_1,
	GPIO_EXT_3_2,
	GPIO_EXT_3_3,
	GPIO_EXT_3_4,
	GPIO_EXT_3_5,
	GPIO_EXT_3_6,
	GPIO_EXT_3_7,
	GPIO_EXT_3_10,
	GPIO_EXT_3_11,
	GPIO_EXT_3_12,
	GPIO_EXT_3_13,
	GPIO_EXT_3_14,
	GPIO_EXT_3_15,
	GPIO_EXT_3_16,
	GPIO_EXT_3_17,
	GPIO_EXT_NUM_MAX = 0xFFFF
	
}GPIO_EXT_NUM_E;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to initialize extend GPIO    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void GPIO_EXT_InitCfgTab(void);

/*****************************************************************************/
//  Description:    This function is used to initialize extend GPIO    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void GPIO_EXT_Init(void);

/*****************************************************************************/
//  Description:    This function is used to check if extend GPIO is init
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_EXT_IsInit(void);

/*****************************************************************************/
//  Description:    This function is used to handle extend GPIO interrupt
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/ 
PUBLIC void GPIO_EXT_ISR(uint32 gpio_id); // system gpio, not extend gpio
   

/*****************************************************************************/
//  Description:    This function is used to de shakeing extend GPIO interrupt
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void GPIO_EXT_DeShaking(uint32 gpio_id);// system gpio, not extend gpio

/*****************************************************************************/
//  Description:    This function is used to set extend GPIO direction
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_SetDirection(
		uint16  gpio_num, 
		BOOLEAN gpio_dir		// SCI_TRUE -> OUTPUT; SCI_FALSE -> INPUT
		);

/*****************************************************************************/
//  Description:    This function is used to set output extend GPIO value
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_SetValue(uint16 gpio_num, BOOLEAN b_on);

/*****************************************************************************/
//  Description:    This function is used to set output value of several gpio in the same ic
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_EXT_SetPortValue(
				uint16 ic_num,		// define in gpio_dvb_ext.h
				uint16 gpio_mask,	// when the related bit is '1', use value of related bit in gpio_value to set gpio output value
				uint16 gpio_value	// represent 16 gpio value
				);

/*****************************************************************************/
//  Description:    This function is used to register extend GPIO
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_RegGpio(
				uint16  gpio_num,
				BOOLEAN direction,
				BOOLEAN default_value,
				GPIO_EXT_CALLBACK_PFUNC callback_fun
				);

/*****************************************************************************/
//  Description:    This function is used to register extend GPIO
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_UnRegGpio(uint16 gpio_num);


/*****************************************************************************/
//  Description:    This function is used to get input extend GPIO value
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_EXT_GetValue(uint16 gpio_num);


PUBLIC void GPIO_EXT_ISR_3_12_Test(BOOLEAN gpio_ext_state);
PUBLIC void GPIO_EXT_Test(void);

/*****************************************************************************/
// Description:    initial ait GPIO extend task
// Global resource dependence:
// Author: liangwen.zhen        
// Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_task_init(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _GPIO_EXT_DVR_H

// End of gpio_ext_drv.h

