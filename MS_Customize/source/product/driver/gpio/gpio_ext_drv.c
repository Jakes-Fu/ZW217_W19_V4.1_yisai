/******************************************************************************
 ** File Name:      gpio_ext_drv.c                                            *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           03/08/2006                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of extend* 
 					GPIO setting.   										  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2006     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "ms_customize_trc.h"
#include "os_api.h"
#include "sci_types.h"
#include "i2c_api.h"
#include "gpio_ext_drv.h"
#include "gpio_ext_cfg.h"
#include "tb_hal.h"
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
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
// IC Register
#define INPUT_PORT_0				0x00
#define INPUT_PORT_1				0x01
#define OUTPUT_PORT_0				0x02
#define OUTPUT_PORT_1				0x03
#define POLARITY_INVERSION_PORT_0	0x04
#define POLARITY_INVERSION_PORT_5	0x05
#define CFG_PORT_0					0x06
#define CFG_PORT_1					0x07

#define GPIO_CNT_IN_ONE_IC			16
#define PORT_CNT_IN_ONE_IC			2
#define GET_IC_NUM(gpio_num)		(uint8)((gpio_num >> 8)&0xff)
#define GET_GPIO_NUM(gpio_num)		(uint8)(gpio_num & 0xff)
#define GET_GPIO_INDEX(gpio_num)	(GET_IC_NUM(gpio_num) * GPIO_CNT_IN_ONE_IC + GET_GPIO_NUM(gpio_num))

#define GPIO_EXT_DEV_INVALID		0xff

//#define CREATE_EXT_GPIO_TASK

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
// Should not modiy the table below
LOCAL GPIO_EXT_GPIO_INFO_T s_GPIO_EXT_tab[] =
{
//	 gpio_num       b_used		direction		default_value	gpio_handle

// IC 1 PORT0
	{GPIO_EXT_1_0 , SCI_TRUE,	GPIO_INPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_1 , SCI_TRUE,	GPIO_INPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_2 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_3 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_4 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_5 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_6 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_7 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
// IC 1 PORT1
	{GPIO_EXT_1_10, SCI_TRUE,	GPIO_INPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_11, SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_12, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_13, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_14, SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_15, SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_16, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_1_17, SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},

// IC 2 PORT0
	{GPIO_EXT_2_0 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_1 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_2 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_3 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_4 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_5 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_6 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_7 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
// IC 2 PORT1	
	{GPIO_EXT_2_10, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_11, SCI_TRUE,	GPIO_INPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_12, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_13, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_14, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_15, SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_16, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_2_17, SCI_TRUE,	GPIO_INPUT, 	SCI_FALSE, 		PNULL},

// IC 3 PORT0	
	{GPIO_EXT_3_0 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_1 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_2 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_3 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_4 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_5 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_6 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_7 , SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
// IC 3 PORT1	
	{GPIO_EXT_3_10, SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_11, SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_12, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_13, SCI_TRUE,	GPIO_INPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_14, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_15, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_16, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},
	{GPIO_EXT_3_17, SCI_TRUE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},

};

LOCAL GPIO_EXT_DEV_INFO_T s_dev_InfoTab[GPIO_EXT_DEV_MAX_CNT] =
{
	{GPIO_EXT_DEV_1_NUM, 0, 0, 0, 0},/*lint !e651 */
	{GPIO_EXT_DEV_2_NUM, 0, 0, 0, 0},
	{GPIO_EXT_DEV_3_NUM, 0, 0, 0, 0}	
};

LOCAL SCI_TIMER_PTR s_GPIO_EXT_DeShaking_Time_ptr = PNULL;
LOCAL BOOLEAN 		s_GPIO_EXT_is_init            = SCI_FALSE;
LOCAL BOOLEAN 		s_GPIO_EXT_is_enable		  = SCI_FALSE;

#define GPIO_EXT_DESHAKING_TIME		20	//ms
LOCAL uint32 s_gpio_ext_int_pin		= 0xffff;
// About task
#ifdef CREATE_EXT_GPIO_TASK

#include "user_tasks_id.h"
#define GPIO_EXT_TASK_STACK_SIZE            1024
static 	uint32 GPIO_EXT_TASK_STACK_ADDR[GPIO_EXT_TASK_STACK_SIZE >> 2];

#define GPIO_EXT_TASK_QUEUE_NUM             10
static 	uint32 GPIO_EXT_TASK_QUEUE_ADDR[GPIO_EXT_TASK_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];

#define GPIO_EXT_TASK_PRIO   				31
#define GPIO_EXT_TASK_ID   				(USER_CUSTOMIZE_TASK_ID_START + 3)   
#endif // CREATE_EXT_GPIO_TASK

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL const uint32 GPIO_TOTAL_CNT = sizeof(s_GPIO_EXT_tab)/sizeof(GPIO_EXT_GPIO_INFO_T);
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to read register value from ic    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPIO_EXT_ReadReg(
			uint32 ic_num, 
			uint8  reg_num, 
			uint8* data_arr, 
			uint32 byte_to_read
			)
{
	uint32 i2c_handle_gpio_ext;
	I2C_DEV dev;
	if( ic_num == s_dev_InfoTab[ic_num].ic_num)
	{
		dev.id = 0;
		dev.freq = 100*1000;
		dev.slave_addr = s_dev_InfoTab[ic_num].i2c_addr_w;
		dev.reg_addr_num = 1;
		dev.check_ack = 1;
		dev.no_stop = 0;
		i2c_handle_gpio_ext  = (uint32)I2C_HAL_Open(&dev);
		I2C_HAL_Read(i2c_handle_gpio_ext, &reg_num, data_arr, byte_to_read);
		I2C_HAL_Close(i2c_handle_gpio_ext);
	}
	else
	{
		//SCI_TRACE_LOW:"_GPIO_EXT_ReadReg: ic %d hasn't existed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_189_112_2_18_0_32_58_1482,(uint8*)"d", ic_num);
	}
}

/*****************************************************************************/
//  Description:    This function is used to write register value to ic    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPIO_EXT_WriteReg(
			uint32 ic_num,			
			uint8* cmd_data_arr, 		// should put cmd in cmd_data_arr[0]
			uint32 byte_to_write		// total size of cmd and data
			)
{
	uint32 i2c_handle_gpio_ext;
	I2C_DEV dev;
	if( ic_num == s_dev_InfoTab[ic_num].ic_num)
	{
		dev.id = 0;
		dev.freq = 100*1000;
		dev.slave_addr = s_dev_InfoTab[ic_num].i2c_addr_w;
		dev.reg_addr_num = 0;
		dev.check_ack = 1;
		dev.no_stop = 0;
		i2c_handle_gpio_ext  = (uint32)I2C_HAL_Open(&dev);
		I2C_HAL_Write(i2c_handle_gpio_ext, NULL, cmd_data_arr, byte_to_write);
		I2C_HAL_Close(i2c_handle_gpio_ext);
	}
	else
	{
		//SCI_TRACE_LOW:"_GPIO_EXT_WriteReg: ic %d hasn't existed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_218_112_2_18_0_32_58_1483,(uint8*)"d", ic_num);
	}
}

/*****************************************************************************/
//  Description:    This function is used to Update input port value from ic    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _GPIO_EXT_UpdateInputValue(
			uint8 			ic_num, 
			PORT_T* 		port_tab_ptr
			)
{	
	uint8 read_data_arr[PORT_CNT_IN_ONE_IC];
	
	//SCI_TRACE_LOW:"_GPIO_EXT_UpdateInputValue: ic_num = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_232_112_2_18_0_32_58_1484,(uint8*)"d", ic_num);
	
	_GPIO_EXT_ReadReg(ic_num, INPUT_PORT_0, read_data_arr, 2);

	//SCI_TRACE_LOW:"port0 = %02x, port1 = %02x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_236_112_2_18_0_32_58_1485,(uint8*)"dd", read_data_arr[0], read_data_arr[1]);
	(*port_tab_ptr).port[0]= read_data_arr[0];
	(*port_tab_ptr).port[1]= read_data_arr[1];
	

}

/*****************************************************************************/
//  Description:    This function is used to check if extend GPIO is enable
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN _GPIO_EXT_IsEnable(void)
{
	return s_GPIO_EXT_is_enable;
}

/*****************************************************************************/
//  Description:    This function is used to initialize extend GPIO    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void GPIO_EXT_InitCfgTab(void)
{
	uint32 index 		  				  = 0;
	uint32 i							  = 0;
	
	
	GPIO_EXT_CFG_T_INFO_PTR  cfg_info_ptr = GPIO_EXT_GetCusCfgInfo();
	GPIO_EXT_GPIO_INFO_T_PTR cus_gpio_tab = PNULL;
	GPIO_EXT_DEV_INFO_T_PTR  cus_dev_tab  = PNULL;
	
	
	if( PNULL == cfg_info_ptr )
	{
		s_GPIO_EXT_is_enable = SCI_FALSE;
		//SCI_TRACE_LOW:"GPIO_EXT_Init: Extend gpio diasble"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_268_112_2_18_0_32_58_1486,(uint8*)"");
		return;
	}
	else
	{
		s_GPIO_EXT_is_enable = SCI_TRUE;
		//SCI_TRACE_LOW:"GPIO_EXT_Init: Extend gpio enable"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_274_112_2_18_0_32_58_1487,(uint8*)"");
	}
	
	s_gpio_ext_int_pin = cfg_info_ptr->int_gpio_num;
	
	//SCI_TRACE_LOW:"GPIO EXT: GPIO_TOTAL_CNT = % d, GPIO_EXT_DEV_MAX_CNT = %d, gpio_cnt_in_one_ic = %d, s_gpio_ext_int_pin = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_284_112_2_18_0_32_58_1488,(uint8*)"ddd",GPIO_TOTAL_CNT,GPIO_EXT_DEV_MAX_CNT,GPIO_CNT_IN_ONE_IC,s_gpio_ext_int_pin);
		 	
	// To check if the working-gpio table is right
	if( GPIO_TOTAL_CNT > (GPIO_CNT_IN_ONE_IC*GPIO_EXT_DEV_MAX_CNT) ) /*lint !e506 !e774 */
	{		
		//SCI_TRACE_LOW:"working-gpio table is over !!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_289_112_2_18_0_32_58_1489,(uint8*)"");
		SCI_ASSERT(0);/*assert verified*/
	}
	
	// Initial extend gpio working-gpio table
	for( i = 0; i < GPIO_TOTAL_CNT; i++)
	{
		s_GPIO_EXT_tab[i].gpio_num 		= GPIO_EXT_NUM_MAX;
		s_GPIO_EXT_tab[i].b_used   		= SCI_FALSE;
		s_GPIO_EXT_tab[i].direction		= SCI_TRUE;
		s_GPIO_EXT_tab[i].default_value = SCI_FALSE;
		s_GPIO_EXT_tab[i].gpio_callback = PNULL;
	}
	
	// Move the setting from customer configure gpio table to working table
	cus_gpio_tab = cfg_info_ptr->gpio_tab_ptr;
	SCI_ASSERT( PNULL != cus_gpio_tab );/*assert verified*/
	for( i = 0; i < GPIO_TOTAL_CNT; i++)
	{/*lint -esym(613, cus_gpio_tab) */
		if( GPIO_EXT_NUM_MAX != cus_gpio_tab[i].gpio_num )  
		{
			index = GET_GPIO_INDEX(cus_gpio_tab[i].gpio_num);
			SCI_ASSERT(GPIO_TOTAL_CNT > index);/*assert verified*/
			
			if(GPIO_EXT_NUM_MAX == s_GPIO_EXT_tab[index].gpio_num) /*lint !e661*/
			{
				SCI_MEMCPY(&s_GPIO_EXT_tab[index], &cus_gpio_tab[i], sizeof(GPIO_EXT_GPIO_INFO_T));
			}
			else
			{
				//SCI_TRACE_LOW:"EXT GPIO: working table %d line has been used !!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_319_112_2_18_0_32_58_1490,(uint8*)"d", index);
			}
			
			if( i == (GPIO_TOTAL_CNT - 1))
			{
				//SCI_TRACE_LOW:"EXT GPIO : cus cfg table has not end flag, or larger than working table!!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_324_112_2_18_0_32_58_1491,(uint8*)"");
			}
			
		}
		else
		{
			break;
		}
	}
	
	// Initial extend gpio working-device table
	for(i = 0; i < GPIO_EXT_DEV_MAX_CNT; i++)
	{
		s_dev_InfoTab[i].ic_num     = GPIO_EXT_DEV_INVALID;
		s_dev_InfoTab[i].i2c_addr_w = 0;
		s_dev_InfoTab[i].i2c_addr_r = 0;
	}
	
	// Move the setting from customer configure device table to working table
	cus_dev_tab = cfg_info_ptr->dev_tab_ptr;
	SCI_ASSERT( PNULL != cus_dev_tab );/*assert verified*/
	
	for(i = 0; i < GPIO_EXT_DEV_MAX_CNT; i++)
	{/*lint -esym(613, cus_dev_tab) */
		if(cus_dev_tab[i].ic_num < GPIO_EXT_DEV_MAX_CNT) /*lint !e661*/
		{
			s_dev_InfoTab[cus_dev_tab[i].ic_num].ic_num     = cus_dev_tab[i].ic_num; /*lint !e661*/
			s_dev_InfoTab[cus_dev_tab[i].ic_num].i2c_addr_w = cus_dev_tab[i].i2c_addr_w; /*lint !e661*/
			s_dev_InfoTab[cus_dev_tab[i].ic_num].i2c_addr_r = cus_dev_tab[i].i2c_addr_r; /*lint !e661*/
		}
	}
}

/*****************************************************************************/
//  Description:    This function is used to initialize extend GPIO    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void GPIO_EXT_Init(void)
{
	uint16 i 			  					 = 0;
	uint16 cfg_reg[GPIO_EXT_DEV_MAX_CNT] 	 = {0};
	uint16 outport_reg[GPIO_EXT_DEV_MAX_CNT] = {0};
	uint8  ic_num 		  					 = 0;
	uint8  gpio_num 	  					 = 0;
	uint8  i2c_cmd_arr[3] 					 = {0};
	
	//SCI_TRACE_LOW:"GPIO_EXT_Init: Start time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_369_112_2_18_0_32_58_1492,(uint8*)"d", SCI_GetTickCount());
	
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return;
	}
	
	// initial de-shaking time pointer
	s_GPIO_EXT_DeShaking_Time_ptr = PNULL;
	
	// i2c init
	//I2C_Init(100*1000);

	//SCI_TRACE_LOW:"----- ic -- port -- gpio -- dir -- value -- callback addr"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_383_112_2_18_0_32_59_1493,(uint8*)"");
	// init extend GPIO
	for(i = 0; i < GPIO_TOTAL_CNT; i++)
	{
		ic_num   = GET_IC_NUM(s_GPIO_EXT_tab[i].gpio_num);
		gpio_num = GET_GPIO_NUM(s_GPIO_EXT_tab[i].gpio_num);		
		
		if(GPIO_EXT_NUM_MAX == s_GPIO_EXT_tab[i].gpio_num)
		{
			ic_num   = i/16;
			gpio_num = i%16;
		}
		
		if(s_GPIO_EXT_tab[i].b_used )
		{
			//SCI_TRACE_LOW:"      %02d  -- %02d -- %02d -- %d  -- %d -- %08x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_405_112_2_18_0_32_59_1494,(uint8*)"dddddd", ic_num, gpio_num/8,gpio_num%8,s_GPIO_EXT_tab[i].direction,s_GPIO_EXT_tab[i].default_value,s_GPIO_EXT_tab[i].gpio_callback);
		}
		else
		{
			//SCI_TRACE_LOW:" *    %02d  -- %02d -- %02d -- %d  -- %d -- %08x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_416_112_2_18_0_32_59_1495,(uint8*)"dddddd", ic_num, gpio_num/8,gpio_num%8,s_GPIO_EXT_tab[i].direction,s_GPIO_EXT_tab[i].default_value,s_GPIO_EXT_tab[i].gpio_callback);
		}		
		
		
		if(!((ic_num < GPIO_EXT_DEV_MAX_CNT) && (gpio_num  < GPIO_CNT_IN_ONE_IC)))
		{
			//SCI_TRACE_LOW:"GPIO_EXT_Init: ERR ic_num %d, gpio_num %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_422_112_2_18_0_32_59_1496,(uint8*)"dd", ic_num, gpio_num);
		}
		else
		{
						
			// Check if the GPIO is used
			if(s_GPIO_EXT_tab[i].b_used)
			{
				// Set GPIO direction
				if(s_GPIO_EXT_tab[i].direction)
				{
				// OUTPUT	
					
					cfg_reg[ic_num] &= ~(uint16)(0x1 << gpio_num);						
					
					// Set GPIO output default value
					if(s_GPIO_EXT_tab[i].default_value)
					{						
						outport_reg[ic_num] |= (0x01 << gpio_num);	
						
					}
					else
					{						
						outport_reg[ic_num] &= ~(uint16)(0x01 << gpio_num);			
						
					}			
					
				}
				else
				{
				// INPUT							
					cfg_reg[ic_num] |= (0x01 << gpio_num);	
										
				}
			}
			else
			{
			// NOT USED -> Default set to input						
				cfg_reg[ic_num] &= ~(uint16)(0x01 << gpio_num);					
			}
		
		}
		
	}
	//SCI_TRACE_LOW:"------------------------- END -------------------------"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_466_112_2_18_0_32_59_1497,(uint8*)"");
	
	// send the setting to pca9535
	for(i = 0; i < GPIO_EXT_DEV_MAX_CNT; i++)
	{	
		i2c_cmd_arr[0] = CFG_PORT_0;
		i2c_cmd_arr[1] = (cfg_reg[i] & 0x00FF);
		i2c_cmd_arr[2] = ((cfg_reg[i] >> 8) & 0x00FF);		
		_GPIO_EXT_WriteReg(i, i2c_cmd_arr, 3);

		i2c_cmd_arr[0] = OUTPUT_PORT_0;
		i2c_cmd_arr[1] = (outport_reg[i] & 0x00FF);
		i2c_cmd_arr[2] = ((outport_reg[i] >> 8) & 0x00FF);	
		_GPIO_EXT_WriteReg(i, i2c_cmd_arr, 3);
	}	

	// Update input gpio value from ic
	for(i = 0; i < GPIO_EXT_DEV_MAX_CNT; i++)
	{
		_GPIO_EXT_UpdateInputValue(i, &(s_dev_InfoTab[i].input));
	}
		
	if(0xffff != s_gpio_ext_int_pin)
	{
		// Create period Timer for gpio interrupt de_shaking
		s_GPIO_EXT_DeShaking_Time_ptr = \
		SCI_CreatePeriodTimer("GPIO_EXT_INT_DESHAKING",\
								GPIO_EXT_DeShaking, \
								s_gpio_ext_int_pin, \
								GPIO_EXT_DESHAKING_TIME,\
								SCI_NO_ACTIVATE);
	
		s_GPIO_EXT_is_init = SCI_TRUE;
	}
	
	//SCI_TRACE_LOW:"GPIO_EXT_Init: End time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_501_112_2_18_0_32_59_1498,(uint8*)"d", SCI_GetTickCount());
	
}

/*****************************************************************************/
//  Description:    This function is used to check if extend GPIO is init
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_EXT_IsInit(void)
{	
	if(!s_GPIO_EXT_is_init)
	{
		//SCI_TRACE_LOW:"GPIO_EXT_IsInit: Not Init!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_512_112_2_18_0_32_59_1499,(uint8*)"");
	}
	
	return s_GPIO_EXT_is_init;
}
 
/*****************************************************************************/
//  Description:    This function is used to handle extend GPIO interrupt
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
 
PUBLIC void GPIO_EXT_ISR(uint32 gpio_id) // system gpio, not extend gpio
{
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return ;
	}	

	SCI_ChangeTimer(s_GPIO_EXT_DeShaking_Time_ptr,\
					GPIO_EXT_DeShaking,\
					GPIO_EXT_DESHAKING_TIME);    
	
	SCI_ActiveTimer(s_GPIO_EXT_DeShaking_Time_ptr);

	HAL_DisableGPIOIntCtl(gpio_id);
	
}   

/*****************************************************************************/
//  Description:    This function is used to de shakeing extend GPIO interrupt
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void GPIO_EXT_DeShaking(uint32 gpio_id)// system gpio, not extend gpio
{	
	PORT_T read_data_arr[GPIO_EXT_DEV_MAX_CNT];
	uint8 i,j, k;
	uint8 temp;
	uint8 *port_ptr1;
	uint8 *port_ptr2;
	uint16 gpio_num;
	BOOLEAN gpio_ext_state;

	//SCI_TRACE_LOW:"GPIO_EXT_DeShaking: Start Time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_553_112_2_18_0_32_59_1500,(uint8*)"d", SCI_GetTickCount());

	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return ;
	}
	
	//deactive timer
    SCI_DeactiveTimer(s_GPIO_EXT_DeShaking_Time_ptr);
	
	// Update input gpio value from ic
	for(i = 0; i < GPIO_EXT_DEV_MAX_CNT; i++)
	{
		_GPIO_EXT_UpdateInputValue(i, &read_data_arr[i]);
	}
	
	// Find the int EXT GPIO and do the handle function
	for(i = 0; i < GPIO_EXT_DEV_MAX_CNT; i++)
	{
		for(j = 0; j < PORT_CNT_IN_ONE_IC; j++)
		{
			port_ptr1 = &(s_dev_InfoTab[i].input.port[j]);
			port_ptr2 = &(read_data_arr[i].port[j]);
						
			
			if(*port_ptr1 != *port_ptr2)
			{
				temp = ((*port_ptr1) ^ (*port_ptr2));
				
				//SCI_TRACE_LOW:"GPIO_EXT_DeShaking: ( %02d - %02d - %02x )"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_583_112_2_18_0_32_59_1501,(uint8*)"ddd", i, j, temp);
					
				for(k = 0; k < 8; k++)
				{
					gpio_num = i*16 + j*8 + k;

					gpio_ext_state = ((temp & 0x1)&((*port_ptr2)>>k))? SCI_TRUE:SCI_FALSE;
						
					if((temp & 0x01) != 0)
					{	/*lint -e661*/	
						SCI_ASSERT( gpio_num < GPIO_TOTAL_CNT);		/*assert verified*/	
						if((!(s_GPIO_EXT_tab[gpio_num].direction)) \
							&& s_GPIO_EXT_tab[gpio_num].b_used)
						{							
							if(PNULL != s_GPIO_EXT_tab[gpio_num].gpio_callback)
							{			
								//SCI_TRACE_LOW:"GPIO_EXT_DeShaking: gpio_num = %04x"
								SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_599_112_2_18_0_32_59_1502,(uint8*)"d", gpio_num);
								s_GPIO_EXT_tab[gpio_num].gpio_callback(gpio_num, gpio_ext_state);
							}
						}
					}

					temp = temp >> 0x01;
				}
			}
			// Update to table
			*port_ptr1 = *port_ptr2;
			
		}

		
	}

	HAL_EnableGPIOIntCtl(gpio_id);
	
	//SCI_TRACE_LOW:"GPIO_EXT_DeShaking: End Time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_618_112_2_18_0_32_59_1503,(uint8*)"d", SCI_GetTickCount());
	
}

/*****************************************************************************/
//  Description:    This function is used to set extend GPIO direction
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_SetDirection(
		uint16  gpio_num, 
		BOOLEAN gpio_dir		// SCI_TRUE -> OUTPUT; SCI_FALSE -> INPUT
		)
{
	uint8 ic;
	uint8 gpio;
	uint16 gpio_index;
	uint8 read_data_arr[2];
	uint8 write_cmd_arr[3];
	uint8 port_num;
	uint8 *port_ptr = PNULL;
	BOOLEAN gpio_dir_value;
	uint8 temp;	
	
	//SCI_TRACE_LOW:"GPIO_EXT_SetDirection: gpio_num = %02x, value = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_640_112_2_18_0_32_59_1504,(uint8*)"dd", gpio_num, gpio_dir);
	
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return SCI_ERROR;
	}
	
	ic 	 = (gpio_num >> 8);
	gpio = (gpio_num & 0xff);
	gpio_index = ic*16 + gpio;
	
	if(!((ic < GPIO_EXT_DEV_MAX_CNT) && (gpio < GPIO_CNT_IN_ONE_IC)))
	{
		SCI_ASSERT(0);/*assert verified*/
	}
	SCI_ASSERT( gpio_index < GPIO_TOTAL_CNT);/*assert verified*/
	
	if(s_GPIO_EXT_tab[gpio_index].b_used && (gpio_dir != s_GPIO_EXT_tab[gpio_index].direction))
	{
		s_GPIO_EXT_tab[gpio_index].direction = gpio_dir;
	
		// if extend GPIO has init do below
		if(GPIO_EXT_IsInit())
		{	
			
			_GPIO_EXT_ReadReg(ic, CFG_PORT_0, read_data_arr, 2);
			
			port_num = 0;
			if(gpio > 7 && gpio < 16)
			{
				port_num = 1;
				gpio -= 8;
			}		

			if(read_data_arr[port_num] & (0x01 << gpio))
			{
				gpio_dir_value = SCI_FALSE;
			}
			else
			{
				gpio_dir_value = SCI_TRUE;
			}
			
			if(gpio_dir_value == gpio_dir)
			{
				//SCI_TRACE_LOW:"GPIO_EXT_SetValue: Don't need to Set extend gpio(%02x) direction(%02x)"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_686_112_2_18_0_32_59_1505,(uint8*)"dd",ic, gpio);
				return SCI_SUCCESS;
			}
			
			if(gpio_dir)
			{
				read_data_arr[port_num] &= ~(uint8)(0x01 << gpio);
			}
			else
			{
				read_data_arr[port_num] |= (0x01 << gpio);
			}

			write_cmd_arr[0] = CFG_PORT_0;
			write_cmd_arr[1] = read_data_arr[0];
			write_cmd_arr[2] = read_data_arr[1];		
			_GPIO_EXT_WriteReg(ic, write_cmd_arr, 3);

			if(!gpio_dir)
			{			
				_GPIO_EXT_ReadReg(ic, CFG_PORT_0, read_data_arr, 2);

				temp = (read_data_arr[port_num] & (0x01 << gpio));
				
				if(ic < GPIO_EXT_DEV_MAX_CNT)
				{
					port_ptr = &(s_dev_InfoTab[ic].input.port[port_num]);
				}
				else
				{
					//SCI_ASSERT(ic < GPIO_EXT_DEV_MAX_CNT);
					//SCI_TRACE_LOW:"[DRV_GPIO_EXT_DRV]<ERROR> ic > GPIO_EXT_DEV_MAX_CNT\r\n"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_717_112_2_18_0_32_59_1506,(uint8*)"" );
				}
		
				if(port_ptr)//for pclint
				{
    				if(temp != 0)
    				{
    					*port_ptr |= temp;
    				}
    				else
    				{
    					*port_ptr &= ~temp;
    				}
                }//for pclint
			}
		}
	}
	
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to set output extend GPIO value
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_SetValue(uint16 gpio_num, BOOLEAN b_on)
{
	uint8 	ic				 = 0;
	uint8 	gpio			 = 0;
	uint16 	gpio_index		 = 0;	
	uint8 	read_data_arr[2] = {0};
	uint8 	write_cmd_arr[3] = {0};
	uint8 	port_num		 = 0;	
	BOOLEAN gpio_value 		 = SCI_FALSE;
	
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return SCI_ERROR;
	}
	
	ic 	 = (gpio_num >> 8);
	gpio = (gpio_num & 0xff);

	gpio_index = ic*16 + gpio;
	
	SCI_ASSERT((ic < GPIO_EXT_DEV_MAX_CNT) && (gpio < GPIO_CNT_IN_ONE_IC));	/*assert verified*/
	SCI_ASSERT( gpio_index < GPIO_TOTAL_CNT);/*assert verified*/
	
	if(s_GPIO_EXT_tab[gpio_index].b_used \
		&& s_GPIO_EXT_tab[gpio_index].direction\
		&& (b_on != s_GPIO_EXT_tab[gpio_index].default_value))
	{
		s_GPIO_EXT_tab[gpio_index].default_value = b_on;
		
		// if extend GPIO has init do below	
		if(GPIO_EXT_IsInit())
		{
			_GPIO_EXT_ReadReg(ic, OUTPUT_PORT_0, read_data_arr, 2);

			port_num = 0;
			if(gpio > 7 && gpio < 16)
			{
				port_num = 1;
				gpio -= 8;
			}		

			if(read_data_arr[port_num] & (0x01 << gpio))
			{
				gpio_value = SCI_TRUE;
			}
			else
			{
				gpio_value = SCI_FALSE;
			}
			
			if(gpio_value == b_on)
			{
			     
				return SCI_SUCCESS;
			}
			
			if(b_on)
			{
				read_data_arr[port_num] |= (0x01 << gpio);
			}
			else
			{
				read_data_arr[port_num] &= ~(uint8)(0x01 << gpio);
			}

			write_cmd_arr[0] = OUTPUT_PORT_0;
			write_cmd_arr[1] = read_data_arr[0];
			write_cmd_arr[2] = read_data_arr[1];	
			_GPIO_EXT_WriteReg(ic, write_cmd_arr, 3);
		}
	}
	
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to get input extend GPIO value
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_EXT_GetValue(uint16 gpio_num)
{
	uint8 	ic				 = 0;
	uint8 	gpio			 = 0;
	uint16 	gpio_index		 = 0;	
	uint8 	read_data_arr[2] = {0};
	uint8 	write_cmd_arr[3] = {0};
	uint8 	port_num		 = 0;	
	BOOLEAN gpio_value 		 = SCI_FALSE;

	
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return SCI_FALSE;
	}
	
	ic 	 = (gpio_num >> 8);
	gpio = (gpio_num & 0xff);
	gpio_index = ic*16 + gpio;
	
	SCI_ASSERT((ic < GPIO_EXT_DEV_MAX_CNT) && (gpio < GPIO_CNT_IN_ONE_IC));	/*assert verified*/

	//SCI_TRACE_LOW:"GPIO_EXT_GetValue: gpio_num = %04x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_843_112_2_18_0_32_59_1507,(uint8*)"d", gpio_num);
	SCI_ASSERT( gpio_index < GPIO_TOTAL_CNT);/*assert verified*/
	
	if(s_GPIO_EXT_tab[gpio_index].b_used)
	{
		// Not allow to get output gpio value, or assert
		SCI_ASSERT(!(s_GPIO_EXT_tab[gpio_index].direction));/*assert verified*/
		
		//gpio_value = s_GPIO_EXT_tab[gpio_index].default_value;			

		// if extend GPIO has init do below	
		if(GPIO_EXT_IsInit())
		{
			if(s_GPIO_EXT_tab[gpio_index].direction)
			{
				write_cmd_arr[0] = OUTPUT_PORT_0;
			}
			else
			{
				write_cmd_arr[0] = INPUT_PORT_0;
			}			
	
			_GPIO_EXT_ReadReg(ic, write_cmd_arr[0], read_data_arr, 2);

			port_num = 0;
			if(gpio > 7 && gpio < 16)
			{
				port_num = 1;
				gpio -= 8;
			}		

			if(read_data_arr[port_num] & (0x01 << gpio))
			{
				gpio_value = SCI_TRUE;
			}
			else
			{
				gpio_value = SCI_FALSE;
			}		
			
			//SCI_TRACE_LOW:"GPIO_EXT_GetValue: gpio_num = %02x, value = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_883_112_2_18_0_32_59_1508,(uint8*)"dd", gpio_num, gpio_value);
		}
	}
	
	return gpio_value;
}

/*****************************************************************************/
//  Description:    This function is used to set output value of several gpio in the same ic
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_EXT_SetPortValue(
				uint16 ic_num,		// define in gpio_dvb_ext.h
				uint16 gpio_mask,	// when the related bit is '1', use value of related bit in gpio_value to set gpio output value
				uint16 gpio_value	// represent 16 gpio value
				)
{	
	uint8 	gpio;
	BOOLEAN selected_gpio_value;
	uint16 	gpio_index;
	uint16  all_gpio_value;
	uint8 	read_data_arr[2];
	uint8 	write_cmd_arr[3];	    
	
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return SCI_FALSE;
	}
	
	SCI_ASSERT( ic_num < GPIO_EXT_DEV_MAX_CNT);	/*assert verified*/

	// if extend GPIO has init do below	
	if(GPIO_EXT_IsInit())
	{
		// Read gpio value from ic	
		_GPIO_EXT_ReadReg(ic_num, OUTPUT_PORT_0, read_data_arr, 2);
		
		all_gpio_value = read_data_arr[0] + (read_data_arr[1] << 8);

		//SCI_TRACE_LOW:"GPIO_EXT_SetPortValue:(before setting) all_gpio_value %0x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_922_112_2_18_0_33_0_1509,(uint8*)"d", all_gpio_value);
		
		for(gpio = 0; gpio < GPIO_CNT_IN_ONE_IC; gpio++)
		{
			if(NULL != (gpio_mask & (0x1<<gpio)))
			{
				gpio_index = (ic_num * 8) + gpio;
				selected_gpio_value = (NULL != (gpio_value & (0x1<<gpio)))? SCI_TRUE:SCI_FALSE;
				
				if(s_GPIO_EXT_tab[gpio_index].b_used \
					&& s_GPIO_EXT_tab[gpio_index].direction\
					&& (selected_gpio_value != s_GPIO_EXT_tab[gpio_index].default_value))
				{
					s_GPIO_EXT_tab[gpio_index].default_value = selected_gpio_value;

					if(selected_gpio_value)
					{
						all_gpio_value |= (0x1<<gpio);
					}
					else
					{
						all_gpio_value &= ~(uint16)(0x1<<gpio);
					}
				}
			}
		}

		//SCI_TRACE_LOW:"GPIO_EXT_SetPortValue:(after setting) all_gpio_value %0x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_949_112_2_18_0_33_0_1510,(uint8*)"d", all_gpio_value);

		write_cmd_arr[0] = OUTPUT_PORT_0;
		write_cmd_arr[1] = (uint8)(all_gpio_value&0xff);
		write_cmd_arr[2] = (uint8)((all_gpio_value>>8)&0xff);	
		_GPIO_EXT_WriteReg(ic_num, write_cmd_arr, 3);
		
	}
	
	
	return SCI_SUCCESS;

}
/*****************************************************************************/
//  Description:    This function is used to register extend GPIO
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_RegGpio(
				uint16  gpio_num,
				BOOLEAN direction,
				BOOLEAN default_value,				 // if input , pls set "SCI_FALSE"
				GPIO_EXT_CALLBACK_PFUNC callback_fun // if output, pls set "PNULL"
				)
{
	uint8 ic;
	uint8 gpio;
	uint8 gpio_index;
	uint8 port_num;
	
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return SCI_ERROR;
	}
	
	ic 	 = (gpio_num >> 8);
	gpio = (gpio_num & 0xff);

	gpio_index = ic*16 + gpio;
	port_num = gpio / 8;
	
	if(!((ic < GPIO_EXT_DEV_MAX_CNT) && (gpio < GPIO_CNT_IN_ONE_IC)))
	{
		SCI_ASSERT(0);	/*assert verified*/
	}
	
	SCI_ASSERT( gpio_index < GPIO_TOTAL_CNT);/*assert verified*/
	if(s_GPIO_EXT_tab[gpio_index].b_used)
	{
		//SCI_TRACE_LOW:"extend GPIO %04x be used"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_997_112_2_18_0_33_0_1511,(uint8*)"d", gpio_num);
	}
	
	s_GPIO_EXT_tab[gpio_index].gpio_num      = gpio_num;
	s_GPIO_EXT_tab[gpio_index].direction     = direction;
	s_GPIO_EXT_tab[gpio_index].default_value = default_value;
	s_GPIO_EXT_tab[gpio_index].gpio_callback = callback_fun;
	s_GPIO_EXT_tab[gpio_index].b_used        = SCI_TRUE;

	if(GPIO_EXT_IsInit())
	{
		if(direction)
		{
			GPIO_EXT_SetDirection(gpio_num, GPIO_OUTPUT);
			GPIO_EXT_SetValue(gpio_num, default_value);
		}
		else
		{
			if(GPIO_EXT_GetValue(gpio_num))
			{				  					
				if(ic < GPIO_EXT_DEV_MAX_CNT)
				{
					s_dev_InfoTab[ic].input.port[port_num] |= (0x01 << (gpio%8));			
				}
				else
				{
					//SCI_ASSERT(ic < GPIO_EXT_DEV_MAX_CNT);
					//SCI_TRACE_LOW:"[DRV_GPIO_EXT_DRV]<WARNING> ic > GPIO_EXT_DEV_MAX_CNT\r\n"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1024_112_2_18_0_33_0_1512,(uint8*)"" );
				}
			}
			else
			{	
				if(ic < GPIO_EXT_DEV_MAX_CNT)
				{
					s_dev_InfoTab[ic].input.port[port_num] &= ~(uint8)(0x01 << (gpio%8));		
				}
				else
				{
					//SCI_ASSERT(ic < GPIO_EXT_DEV_MAX_CNT);
					//SCI_TRACE_LOW:"[DRV_GPIO_EXT_DRV]<ERROR> func:GPIO_EXT_RegGpio() ic > GPIO_EXT_DEV_MAX_CNT\r\n"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1036_112_2_18_0_33_0_1513,(uint8*)"" );
				}
			}
		}
	}

	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to register extend GPIO
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_UnRegGpio(uint16 gpio_num)
{
	uint8 ic;
	uint8 gpio;
	uint8 gpio_index;
	uint8 port_num;
	BOOLEAN  old_direction;
	
	if( !_GPIO_EXT_IsEnable() )
	{
		// EXTEND GPIO is diable
		return SCI_ERROR;
	}
	
	ic 	 = (gpio_num >> 8);
	gpio = (gpio_num & 0xff);

	gpio_index = ic*16 + gpio;
	port_num = gpio / 8;
	
	SCI_ASSERT((ic < GPIO_EXT_DEV_MAX_CNT) && (gpio < GPIO_CNT_IN_ONE_IC));	/*assert verified*/	
	SCI_ASSERT( gpio_index < GPIO_TOTAL_CNT);/*assert verified*/
	
	if(!(s_GPIO_EXT_tab[gpio_index].b_used))
	{
		//SCI_TRACE_LOW:"GPIO_EXT_UnRegGpio: The extend GPIO is not used!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1073_112_2_18_0_33_0_1514,(uint8*)"");
		return SCI_SUCCESS;
	}
	
	old_direction = s_GPIO_EXT_tab[gpio_index].direction;
	s_GPIO_EXT_tab[gpio_index].gpio_num	     = GPIO_EXT_NUM_MAX;
	s_GPIO_EXT_tab[gpio_index].direction 	 = GPIO_OUTPUT;
	s_GPIO_EXT_tab[gpio_index].default_value = SCI_FALSE;
	s_GPIO_EXT_tab[gpio_index].gpio_callback = PNULL;
	s_GPIO_EXT_tab[gpio_index].b_used 		 = SCI_FALSE;	
	/*lint +e661*/
	if(GPIO_EXT_IsInit())
	{
		if(old_direction)
		{
			GPIO_EXT_SetDirection(gpio_num, GPIO_INPUT);			
		}		
	}
	
	return SCI_SUCCESS;
}

#ifdef CREATE_EXT_GPIO_TASK
/*****************************************************************************/
// Description:    GPIO extend task entry
// Global resource dependence:
// Author: liangwen.zhen        
// Note:
/*****************************************************************************/
LOCAL  void GPIO_EXT_Control_Entry(uint32 argc,void *argv)
{   
    //SCI_TRACE_LOW:"GPIO_EXT_Control_Entry: start time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1102_112_2_18_0_33_0_1515,(uint8*)"d", SCI_GetTickCount());

	//
	//GPIO_TestCode();
	
	//SCI_TRACE_LOW:"GPIO_EXT_Control_Entry: end time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1107_112_2_18_0_33_0_1516,(uint8*)"d", SCI_GetTickCount());

	SCI_DeleteThread(GPIO_EXT_TASK_ID);
}
#endif

/*****************************************************************************/
// Description:    initial ait GPIO extend task
// Global resource dependence:
// Author: liangwen.zhen        
// Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_task_init(void)                            
{   
#ifdef CREATE_EXT_GPIO_TASK
        
   SCI_CreateStaticThread( GPIO_EXT_TASK_ID, 
            "GPIO_EXT_TASK",
            GPIO_EXT_Control_Entry,
            0,
            NULL,           
            GPIO_EXT_TASK_STACK_ADDR, 
            GPIO_EXT_TASK_STACK_SIZE, 
            GPIO_EXT_TASK_PRIO, 
            SCI_PREEMPT, 
            "Q_GPIO_EXT", 
            GPIO_EXT_TASK_QUEUE_ADDR, 
            GPIO_EXT_TASK_QUEUE_NUM, 
            SCI_AUTO_START);
   
    //SCI_TRACE_LOW:"GPIO_EXT_task_init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1135_112_2_18_0_33_0_1517,(uint8*)"");
    
#endif // CREATE_EXT_GPIO_TASK       
    
    return SCI_SUCCESS;                            
}

// For Test Below     
#define _GPIO_DVB_EXT_TEST

#ifdef _GPIO_DVB_EXT_TEST

LOCAL uint8 	g_function = 0;
LOCAL uint16 g_gpio_num = 0;
LOCAL BOOLEAN g_gpio_value = 0;
LOCAL BOOLEAN g_gpio_dir = 0;

void GPIO_EXT_ISR_3_12_Test(BOOLEAN gpio_ext_state)
{
	//SCI_TRACE_LOW:"GPIO_EXT_ISR_3_12_Test: gpio_ext_state = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1154_112_2_18_0_33_0_1518,(uint8*)"d", gpio_ext_state);
}
void GPIO_EXT_Test(void)
{
	//SCI_TRACE_LOW:"GPIO_EXT_Test"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1158_112_2_18_0_33_0_1519,(uint8*)"");

	
	while(1)                   /*lint !e716 */
	{
		while(!g_function);    /*lint !e722 */

		//SCI_TRACE_LOW:"GPIO_EXT_Test: g_function = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_EXT_DRV_1165_112_2_18_0_33_0_1520,(uint8*)"d", g_function);
		
		switch(g_function)
		{
			case 1:
				GPIO_EXT_SetDirection(g_gpio_num, g_gpio_dir);
				break;
			case 2:
				GPIO_EXT_SetValue(g_gpio_num, g_gpio_value);
				break;
			case 3:
				g_gpio_value = GPIO_EXT_GetValue(g_gpio_num);
				break;
			case 4:
				GPIO_EXT_Init();
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				break;
		}	
		
		g_function = 0;	
		
	}

}
#endif //_GPIO_DVB_EXT_TEST
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of gpio_dvb_ext.c
