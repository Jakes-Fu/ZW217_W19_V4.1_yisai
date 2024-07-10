/******************************************************************************
 ** File Name:      msensor_mir3da.c                                          *
 ** Author:          Yc.Wang                                                *
 ** DATE:            2015.08.31                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2015.08.31   Yc.Wang            Create.                               *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "Power_cfg.h"
#include "zdt_hsensor.h"

// general C library header
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// PixArt sensor support
#include "pah800x_main.h"
#include "pah800x_bsp.h"

// PixArt sensor factory test support 
#include "pah800x_factory_test.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

#define HRS_I2C_ERROR                0
#define HRS_I2C_SUCCESS              1

//#define PPG_USE_SW_I2C
#ifdef PPG_USE_SW_I2C
static BOOLEAN   g_ppg_i2c_is_open = FALSE;
static uint8 s_i2c_addr_write = 0x80;
static uint8 s_i2c_addr_read = 0x81;

#define PPG_I2C_DELAY_FT											100
#define PPG_I2C_DATA_PIN	15 
#define PPG_I2C_CLK_PIN	14 
#define GPIO_PPG_I2C_SDA_PIN  PPG_I2C_DATA_PIN
#define GPIO_PPG_I2C_SCL_PIN  PPG_I2C_CLK_PIN

#define PPG_ACK_COUNTER        			100
#define PPG_SET_I2C_CLK_OUTPUT				GPIO_SetDirection(PPG_I2C_CLK_PIN,1)
#define PPG_SET_I2C_DATA_OUTPUT			GPIO_SetDirection(PPG_I2C_DATA_PIN,1)
#define PPG_SET_I2C_DATA_INPUT				GPIO_SetDirection(PPG_I2C_DATA_PIN,0)
#define PPG_SET_I2C_CLK_HIGH				GPIO_SetValue(PPG_I2C_CLK_PIN,1)
#define PPG_SET_I2C_CLK_LOW				GPIO_SetValue(PPG_I2C_CLK_PIN,0)

#define PPG_GET_I2C_CLK_BIT					((GPIO_GetGPIOState(PPG_I2C_CLK_PIN)) ? 1 : 0)
#define SET_I2C_CLK_INPUT					GPIO_SetDirection(PPG_I2C_CLK_PIN,0)

#define PPG_SET_I2C_DATA_HIGH				GPIO_SetValue(PPG_I2C_DATA_PIN,1)
#define PPG_SET_I2C_DATA_LOW				GPIO_SetValue(PPG_I2C_DATA_PIN,0)
#define PPG_GET_I2C_DATA_BIT				((GPIO_GetGPIOState(PPG_I2C_DATA_PIN)) ? 1 : 0)

#define PPG_I2C_START_BIT \
	{ \
		volatile uint32 j; \
		PPG_SET_I2C_CLK_HIGH; \
		PPG_SET_I2C_DATA_HIGH; \
		for(j=0;j<PPG_I2C_DELAY_FT;j++);\
		PPG_SET_I2C_DATA_LOW; \
		for(j=0;j<PPG_I2C_DELAY_FT;j++);\
		PPG_SET_I2C_CLK_LOW; \
	}

	#define PPG_I2C_STOP_BIT \
	{ \
		volatile uint32 j; \
		PPG_SET_I2C_CLK_LOW; \
		PPG_SET_I2C_DATA_LOW; \
		for(j=0;j<PPG_I2C_DELAY_FT;j++);\
		PPG_SET_I2C_CLK_HIGH; \
		for(j=0;j<PPG_I2C_DELAY_FT;j++);\
		PPG_SET_I2C_DATA_HIGH; \
		for(j=0;j<PPG_I2C_DELAY_FT;j++);\
	}
#endif //SW_I2CS

#ifdef PPG_USE_SW_I2C
static uint32 PAH8009_I2C_Open(void)
{
    if(g_ppg_i2c_is_open == FALSE)
    {
        pm_i2c1_gpio_cfg(1);
        PPG_SET_I2C_CLK_OUTPUT;
        PPG_SET_I2C_DATA_OUTPUT;
        PPG_SET_I2C_CLK_HIGH;
        PPG_SET_I2C_DATA_HIGH;
        g_ppg_i2c_is_open = TRUE;
    }
}

static uint32 PAH8009_I2C_Close(void)
{
    if(g_ppg_i2c_is_open)
    {
        PPG_SET_I2C_CLK_OUTPUT;
        PPG_SET_I2C_DATA_OUTPUT;
        PPG_SET_I2C_CLK_LOW;
        PPG_SET_I2C_DATA_LOW;
        g_ppg_i2c_is_open = FALSE;
    }
    return HRS_I2C_SUCCESS;
}

static void PPG_I2C_delay_us(uint32 time)
{
	uint32 i,j,k;
	for(i=0;i< time;i++)
		for(j=0;j<50;j++)
          {
            k=0;
        };
}

uint8 PPG_I2C_send_byte(uint8 send_byte)
{
	volatile signed char i;
	volatile uint32 j;
	volatile uint32 k=0;
	uint8 ack;
	
	for (i=7;i>=0;i--)
	{	/* data bit 7~0 */
		if (send_byte & (1<<i))
		{
			PPG_SET_I2C_DATA_HIGH;

		}
		else
		{
			PPG_SET_I2C_DATA_LOW;

		}
		for(j=0;j<PPG_I2C_DELAY_FT;j++);
		PPG_SET_I2C_CLK_HIGH;
		for(j=0;j<PPG_I2C_DELAY_FT;j++);
		PPG_SET_I2C_CLK_LOW;
		for(j=0;j<PPG_I2C_DELAY_FT;j++);
	}
	
	PPG_SET_I2C_DATA_INPUT;
	PPG_SET_I2C_CLK_HIGH;
	for(j=0;j<PPG_I2C_DELAY_FT;j++);

	for(k=0;k<PPG_ACK_COUNTER;k++)
	{
		if(PPG_GET_I2C_DATA_BIT==0)
		{
			ack=1;
			break;
		}
		else
		{
			ack=0;
		}
	}

	PPG_SET_I2C_CLK_LOW;
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_DATA_OUTPUT;
	
	PPG_I2C_delay_us(20);
	return ack;
}	

uint8 PPG_I2C_get_byte(void)
{
	volatile signed char i;
	volatile uint32 j;
	uint8 get_byte=0;

	PPG_SET_I2C_DATA_INPUT;
	  for (i=7;i>=0;i--)
        {       /* data bit 7~0 */
                PPG_SET_I2C_CLK_LOW;
                for(j=0;j<PPG_I2C_DELAY_FT;j++);
                PPG_SET_I2C_CLK_HIGH;
                for(j=0;j<PPG_I2C_DELAY_FT;j++);
                if (PPG_GET_I2C_DATA_BIT)
                        get_byte |= (1<<i);
        }
	/* don't care bit, 9th bit */
	
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_CLK_LOW;
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_DATA_OUTPUT;
	PPG_I2C_delay_us(5);
	return get_byte;
}	

int8 PPG_I2C_WRITE(uint8 reg, uint8* value,uint32 cnt)
{
	volatile uint32 j;
	volatile uint32 i;
	PPG_I2C_START_BIT;       
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	if(!PPG_I2C_send_byte(s_i2c_addr_write))
	return(-1);
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	if(!PPG_I2C_send_byte(reg))
	return(-1);
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
      for(i=0;i<cnt;i++)
      {
          	if(!PPG_I2C_send_byte(value[i]))
          	return(-1);
          	for(j=0;j<PPG_I2C_DELAY_FT;j++);
      }
       PPG_I2C_STOP_BIT;      
       return 1;

}


uint8 PPG_I2C_get_byte_with_ack(unsigned char uc_ack_lvl)
{
	volatile signed char i;
	volatile uint32 j;
	uint8 get_byte=0;

	PPG_SET_I2C_DATA_INPUT;
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	  for (i=7;i>=0;i--)
        {       /* data bit 7~0 */
                PPG_SET_I2C_CLK_LOW;
                for(j=0;j<PPG_I2C_DELAY_FT;j++);
                PPG_SET_I2C_CLK_HIGH;
                for(j=0;j<PPG_I2C_DELAY_FT;j++);
                if (PPG_GET_I2C_DATA_BIT)
                        get_byte |= (1<<i);
				for(j=0;j<PPG_I2C_DELAY_FT;j++);
        }
	/* don't care bit, 9th bit */

	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_CLK_LOW;
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_DATA_OUTPUT;
	
	if (uc_ack_lvl == 1)
	{
	    PPG_SET_I2C_DATA_HIGH;
	}
	else
	{
	    PPG_SET_I2C_DATA_LOW;
	}
	
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_CLK_HIGH;

	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_CLK_LOW;

	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_SET_I2C_DATA_LOW;
	
	//PPG_I2C_delay_us(5);
	return get_byte;
}	

static int PAH8009_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    int8 res = PPG_I2C_WRITE(*reg,addr,cnt);
    if (1 == res)
    {
        return(1);
    }	
    else
    {
        return(0);
    }
}
static int PAH8009_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    volatile uint32 j;
    uint8 get_byte = 0;
    uint16 i;
	
    PPG_I2C_START_BIT;       
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	if(!PPG_I2C_send_byte(s_i2c_addr_write))
	return(-1);
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	if(!PPG_I2C_send_byte(*reg))
	return(-1);
	for(j=0;j<PPG_I2C_DELAY_FT;j++);

       PPG_I2C_STOP_BIT;
	for(j=0;j<PPG_I2C_DELAY_FT;j++);
	PPG_I2C_START_BIT;       
	if(!PPG_I2C_send_byte(s_i2c_addr_read))
	return(-1);

	if (cnt <= 1)
    {
        //*value =PPG_I2C_get_byte();
        *addr =PPG_I2C_get_byte_with_ack(1);
		for(j=0;j<PPG_I2C_DELAY_FT;j++);
    }
	else
	{
	    for (i = 0; i< cnt - 1; i++)
	    {
	        *addr++ =PPG_I2C_get_byte_with_ack(0);
	    }
		*addr =PPG_I2C_get_byte_with_ack(1);
	}
       PPG_I2C_STOP_BIT;
 	 
       return 1;
}

LOCAL uint32 _Get_PAH8009_I2C_Status(void)
{
    if(g_ppg_i2c_is_open == FALSE)
    {
        return HRS_I2C_ERROR;
    }
    else
    {
        return HRS_I2C_SUCCESS;
    }
}
BOOLEAN Pah8009_write_reg(uint8 addr, uint8 * data, uint32 cnt) 
{
    uint32 ret = 0;
    if(_Get_PAH8009_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = PAH8009_I2C_Write(&addr, data, cnt);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("Pah8009_write_reg I2C_HAL_Read Err addr=0x%02x, ret=%d,cnt=%d",addr,ret,cnt);
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("Pah8009_write_reg Err addr=0x%02x, cnt = %d",addr,cnt);
    }
    return 1;
}

uint8 Pah8009_read_reg(uint8 addr,uint8 * data, uint32 cnt)
{
    uint32 ret = 0;
    if(_Get_PAH8009_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = PAH8009_I2C_Read(&addr,data,cnt);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("Pah8009_read_reg I2C_HAL_Read Err addr=0x%02x, ret=%d,cnt=%d",addr,ret,cnt);
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("Pah8009_read_reg Err addr=0x%02x, cnt = %d",addr,cnt);
    }
    return 1;
}
#else
#if 1

//0x40
#define PAH8009_I2C_WRITE_ADDR       0x80
#define PAH8009_I2C_READ_ADDR         0x81
static int32 PAH8009_I2C_Handler = -1;

static uint32 _Get_PAH8009_I2C_Status(void);
/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: PAH8009_I2C_Open
// Dependence: 
// Note:
/******************************************************************************/
static I2C_DEV PAH8009_I2C_cfg =
{
    1,          
    100000,                   // freq is standard for PAH8009
    0,                          // bus
    PAH8009_I2C_WRITE_ADDR,  // slave_addr
    1,                          // reg_addr_num
    1,                          // check_ack
    0                           // no_stop
};

static uint32 PAH8009_I2C_Open(void)
{
    if(PAH8009_I2C_Handler == -1) 
    {
        if((PAH8009_I2C_Handler = I2C_HAL_Open(&PAH8009_I2C_cfg)) == -1)
        {
            ZDT_HSENSOR_TRACE("PAH8009_I2C_Open Err");
            return HRS_I2C_ERROR;    
        } 
        else
            return HRS_I2C_SUCCESS;
    }
    else
    {
        return HRS_I2C_SUCCESS;
        
    }
}

/******************************************************************************/
// Description: PAH8009_I2C_Close
// Dependence: 
// Note:
/******************************************************************************/

static uint32 PAH8009_I2C_Close(void)
{
    if(_Get_PAH8009_I2C_Status() == HRS_I2C_SUCCESS)
	{
 		I2C_HAL_Close(PAH8009_I2C_Handler);
		PAH8009_I2C_Handler = -1;
	}	
    return HRS_I2C_SUCCESS;
}
/******************************************************************************/
// Description: PAH8009_I2C_Write
// Dependence: 
// Note:
/******************************************************************************/
static void PAH8009_I2C_Write(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_PAH8009_I2C_Status() == HRS_I2C_SUCCESS)
        I2C_HAL_Write(PAH8009_I2C_Handler, reg, addr, cnt);
}


/******************************************************************************/
// Description: PAH8009_I2C_Read
// Dependence: 
// Note:
/******************************************************************************/
static void PAH8009_I2C_Read(uint8 *reg, uint8 *addr, uint32 cnt)
{
    if(_Get_PAH8009_I2C_Status() == HRS_I2C_SUCCESS)
        I2C_HAL_Read(PAH8009_I2C_Handler,reg, addr, cnt);
}


/******************************************************************************/
// Description: _Get_PAH8009_I2C_Status
// Dependence:  It can get the status from i2c
// Note:
/******************************************************************************/
static uint32 _Get_PAH8009_I2C_Status(void)
{
    if(PAH8009_I2C_Handler==-1)
    {
        return HRS_I2C_ERROR;
    }
    else
    {
        return HRS_I2C_SUCCESS;
    }
}

BOOLEAN Pah8009_write_reg(uint8 addr, uint8 * data, uint32 cnt) 
{
    uint32 ret = 0;
    if(_Get_PAH8009_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = I2C_HAL_Write(PAH8009_I2C_Handler, &addr, data, cnt);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("Pah8009_write_reg I2C_HAL_Read Err addr=0x%02x, ret=%d,cnt=%d",addr,ret,cnt);
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("Pah8009_write_reg Err addr=0x%02x, cnt = %d",addr,cnt);
    }
    return 1;
}

uint8 Pah8009_read_reg(uint8 addr,uint8 * data, uint32 cnt)
{
    uint32 ret = 0;
    if(_Get_PAH8009_I2C_Status() == HRS_I2C_SUCCESS)
    {
        ret = I2C_HAL_Read(PAH8009_I2C_Handler,&addr,data,cnt);
        if(ret == 0)
        {
            ZDT_HSENSOR_TRACE("Pah8009_read_reg I2C_HAL_Read Err addr=0x%02x, ret=%d,cnt=%d",addr,ret,cnt);
        }
    }
    else
    {
        ZDT_HSENSOR_TRACE("Pah8009_read_reg Err addr=0x%02x, cnt = %d",addr,cnt);
    }
    return 1;
}
#endif
#endif
#if 1


// platform function support
/* Customer should include other necessary header files for supporting system basic functions
 * like I2C, GPIO, UART...
 * 
 * 客户需要自己把必要的支持系统基础功能的头文件包含进来，比如I2C，GPIO，UART...
 */


#ifdef PPG_MEMS_Synchronize
extern uint8_t mems_data_polling_ready;
static uint8_t ppg_data_num = 0;
#endif

//============================================================================
// Private function
//============================================================================
void qah800x_handle_gpio_hr_interrupt(void);	// Sensor INT handler 中断处理函数（仅供参考）
void qah800x_handle_gpio_spo2_interrupt(void);
void rtc_handler(void);				// RTC handler 定时器处理函数（仅供参考）


/************************************************************
 * PAH8009 Examples Function Prototypes
 ***********************************************************/
#if (INTERRUPT_MODE==1)
//-----------------HRD--------------//
    // Only Touch Detection Mode example - by Interrupt
    // 使用中断方式的Touch检测工作模式示例
	#ifdef touch_only_hrd_en
        void demo_touch_mode_only_hrd(void);
    #endif
 
    // PPG Mode always running example - by Interrupt
    // 使用中断方式的PPG工作模式示例
    #ifdef ppg_only_hrd_en
        void demo_ppg_mode_only_hrd(void);
    #endif

    // Touch & PPG Auto switch mode - by Interrupt
    // 使用中断方式的Touch & PPG自动切换工作模式示例
    #ifdef auto_hrd_en
        void demo_auto_mode_hrd(void);
    #endif

//-----------------SPO2--------------//
    // Only Touch Detection Mode example - by Interrupt
    // 使用中断方式的Touch检测工作模式示例
	#ifdef touch_only_spo2_en
        void demo_touch_mode_only_spo2(void);
    #endif
 
    // PPG Mode always running example - by Interrupt
    // 使用中断方式的PPG工作模式示例
    #ifdef ppg_only_spo2_en
        void demo_ppg_mode_only_spo2(void);
    #endif

    // Touch & PPG Auto switch mode - by Interrupt
    // 使用中断方式的Touch & PPG自动切换工作模式示例
    #ifdef auto_spo2_en
        void demo_auto_mode_spo2(void);
    #endif

//-----------------Stress--------------//
    // Only Touch Detection Mode example - by Interrupt
    // 使用中断方式的Touch检测工作模式示例
	#ifdef touch_only_stress_en
        void demo_touch_mode_only_stress(void);
    #endif
 
    // PPG Mode always running example - by Interrupt
    // 使用中断方式的PPG工作模式示例
    #ifdef ppg_only_stress_en
        void demo_ppg_mode_only_stress(void);
    #endif

    // Touch & PPG Auto switch mode - by Interrupt
    // 使用中断方式的Touch & PPG自动切换工作模式示例
    #ifdef auto_stress_en
        void demo_auto_mode_stress(void);
    #endif
//-----------------HRD--PC Cover------------//
    // Only Touch Detection Mode example - by Interrupt
    // 使用中断方式的Touch检测工作模式示例
	#ifdef touch_only_hrd_en_pc
            void demo_touch_mode_only_hrd_pc(void);
    #endif
 
    // PPG Mode always running example - by Interrupt
    // 使用中断方式的PPG工作模式示例
    #ifdef ppg_only_hrd_en_pc
           void demo_ppg_mode_only_hrd(_pcvoid);
    #endif

    // Touch & PPG Auto switch mode - by Interrupt
    // 使用中断方式的Touch & PPG自动切换工作模式示例
    #ifdef auto_hrd_en_pc
           void demo_auto_mode_hrd_pc(void);
    #endif    
#endif


#if (POLLING_MODE==1)
//---------------------HRD----------------------//
    // Only Touch Detection Mode example - by polling
    // 使用轮询方式的Touch检测工作模式示例    
	#ifdef touch_only_polling_hrd_en
        void demo_touch_mode_only_polling_hrd(void);
    #endif

    // PPG Mode always running example - by polling
    // 使用轮询方式的PPG工作模式示例     
    #ifdef ppg_only_polling_hrd_en
        void demo_ppg_mode_only_polling_hrd(void);
    #endif

    // Touch & PPG Auto switch mode - by polling
    // 使用轮询方式的Touch & PPG自动切换工作模式示例     
    #ifdef auto_polling_hrd_en
        void demo_auto_mode_polling_hrd(void);
    #endif
    
//---------------------SPO2----------------------//
    // Only Touch Detection Mode example - by polling
    // 使用轮询方式的Touch检测工作模式示例
    #ifdef touch_only_polling_spo2_en
        void demo_touch_mode_only_polling_spo2(void);
    #endif

    // PPG Mode always running example - by polling
    // 使用轮询方式的PPG工作模式示例     
    #ifdef ppg_only_polling_spo2_en
        void demo_ppg_mode_only_polling_spo2(void);
    #endif

    // Touch & PPG Auto switch mode - by polling
    // 使用轮询方式的Touch & PPG自动切换工作模式示例     
    #ifdef auto_polling_spo2_en
        void demo_auto_mode_polling_spo2(void);
    #endif

//---------------------Stress----------------------//
    // Only Touch Detection Mode example - by polling
    // 使用轮询方式的Touch检测工作模式示例
    #ifdef touch_only_polling_stress_en
        void demo_touch_mode_only_polling_stress(void);
    #endif

    // PPG Mode always running example - by polling
    // 使用轮询方式的PPG工作模式示例     
    #ifdef ppg_only_polling_stress_en
        void demo_ppg_mode_only_polling_stress(void);
    #endif

    // Touch & PPG Auto switch mode - by polling
    // 使用轮询方式的Touch & PPG自动切换工作模式示例     
    #ifdef auto_polling_stress_en
        void demo_auto_mode_polling_stress(void);
    #endif
	
 //---------------------HRD--PC_Cover--------------------//
    // Only Touch Detection Mode example - by polling
    // 使用轮询方式的Touch检测工作模式示例
    #ifdef touch_only_polling_hrd_en_pc
           void demo_touch_mode_only_polling_hrd_pc(void);
    #endif

    // PPG Mode always running example - by polling
    // 使用轮询方式的PPG工作模式示例 		
	#ifdef ppg_only_polling_hrd_en_pc
           void demo_ppg_mode_only_polling_hrd_pc(void);
    #endif

    // Touch & PPG Auto switch mode - by polling
    // 使用轮询方式的Touch & PPG自动切换工作模式示例 		
	#ifdef auto_polling_hrd_en_pc
           void demo_auto_mode_polling_hrd_pc(void);
    #endif   
#endif

/* Test pattern code for HRD and SpO2 algorithm */
/* HRD & SpO2算法库验证代码 */
#if (Test_pattern_mode==1)
//HRD
    #ifdef hrd_testpattern_en
        void test_library_HRD(void);		// 心率算法库验证代码
    #endif

//SPO2
    #ifdef spo2_testpattern_en
        void test_library_SPO2(void);		// 血氧算法库验证代码 
    #endif

#endif

int pah8009_main(void)
{
	/*
     * Customer should make sure system is ready(like power on, 
     * i2c initialization, GPIO config...) before operating PAH8009
     *  
     * 客户需要在操作PAH8009之前确认系统已经准备就绪（比如供电，I2C功能初始化，GPIO配置等）
	 */
  
    /* Below are some examples of PAH8009 operation */
    /* 以下是一些PAH8009操作的示例 */


#if  (INT_OD_EXTERNAL_PULLH==1)
    DEBUG_PRINT(" INT_OD_EXTERNAL_PULLH  \r\n");
#endif

#if (INT_OD_INTERNAL_PULLH==1)
    DEBUG_PRINT(" INT_OD_INTERNAL_PULLH  \r\n");
#endif

#if (INT_PUP_WITHOUT_PULLH==1)
    DEBUG_PRINT(" INT_PUP_WITHOUT_PULLH  \r\n");
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_HRD
============================================================================*/
#ifdef touch_only_hrd_en 
    DEBUG_PRINT("Run_touch_only_hrd_en_mode\r\n");
    demo_touch_mode_only_hrd();   /* touch detect mode */
#endif

#ifdef ppg_only_hrd_en
    DEBUG_PRINT("Run_ppg_only_hrd_en_mode\r\n"); 
    demo_ppg_mode_only_hrd();    /* ppg_mode always run mode */
#endif

#ifdef auto_hrd_en
    DEBUG_PRINT("Run_auto_hrd_en_mode\r\n");
    demo_auto_mode_hrd();        /* Touch & PPG auto switch  mode */
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_HRD
============================================================================*/
#ifdef touch_only_polling_hrd_en
    DEBUG_PRINT("Run_touch_only_polling_hrd_en_mode\r\n");
    demo_touch_mode_only_polling_hrd();  /* touch detect mode   -  polling */
#endif

#ifdef ppg_only_polling_hrd_en
    DEBUG_PRINT("Run_ppg_only_polling_hrd_en_mode\r\n");
    demo_ppg_mode_only_polling_hrd();   /* ppg_mode always run mode  -  polling */ 
#endif

#ifdef auto_polling_hrd_en
    DEBUG_PRINT("Run_auto_polling_hrd_en_mode\r\n");
    demo_auto_mode_polling_hrd();      /* Touch & PPG Auto switch  mode -  polling */
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_SPO2
============================================================================*/
#ifdef touch_only_spo2_en
    DEBUG_PRINT("Run_touch_only_spo2_en_mode\r\n");
    demo_touch_mode_only_spo2();  /* touch detect mode SPO2*/
#endif

#ifdef ppg_only_spo2_en
    DEBUG_PRINT("Run_ppg_only_spo2_en_mode\r\n");
    demo_ppg_mode_only_spo2();   /* ppg_mode always run mode SPO2 */
#endif

#ifdef auto_spo2_en
    DEBUG_PRINT("Run_auto_spo2_en_mode\r\n");
    demo_auto_mode_spo2();      /* Touch & PPG auto switch  mode SPO2 */
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_SPO2
============================================================================*/
#ifdef touch_only_polling_spo2_en
    DEBUG_PRINT("Run_touch_only_polling_spo2_en_mode\r\n");
    demo_touch_mode_only_polling_spo2();  /* touch detect mode   -  polling */
#endif

#ifdef ppg_only_polling_spo2_en
    DEBUG_PRINT("Run_ppg_only_polling_spo2_en_mode\r\n");
    demo_ppg_mode_only_polling_spo2();   /* ppg_mode always run mode  -  polling */ 
#endif

#ifdef auto_polling_spo2_en
    DEBUG_PRINT("Run_auto_polling_spo2_en_mode\r\n");
    demo_auto_mode_polling_spo2();      /* Touch & PPG Auto switch  mode -  polling */
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_Stress
============================================================================*/
#ifdef touch_only_stress_en
    DEBUG_PRINT("Run_touch_only_sstress_en_mode\r\n");
    demo_touch_mode_only_stress();  /* touch detect mode SPO2*/
#endif

#ifdef ppg_only_stress_en
    DEBUG_PRINT("Run_ppg_only_stress_en_mode\r\n");
    demo_ppg_mode_only_stress();   /* ppg_mode always run mode SPO2 */
#endif

#ifdef auto_stress_en
    DEBUG_PRINT("Run_auto_stress_en_mode\r\n");
    demo_auto_mode_stress();      /* Touch & PPG auto switch  mode SPO2 */
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_Stress
============================================================================*/
#ifdef touch_only_polling_stress_en
    DEBUG_PRINT("Run_touch_only_polling_stress_en_mode\r\n");
    demo_touch_mode_only_polling_stress();  /* touch detect mode   -  polling */
#endif

#ifdef ppg_only_polling_stress_en
    DEBUG_PRINT("Run_ppg_only_polling_stress_en_mode\r\n");
    demo_ppg_mode_only_polling_stress();   /* ppg_mode always run mode  -  polling */ 
#endif
 
#ifdef auto_polling_stress_en
    DEBUG_PRINT("Run_auto_polling_stress_en_mode\r\n");
    demo_auto_mode_polling_stress();      /* Touch & PPG Auto switch  mode -  polling */
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_INT mode_HRD pc cover
============================================================================*/
#ifdef touch_only_hrd_en_pc 
	DEBUG_PRINT("Run_touch_only_hrd_en_mode_pc\r\n");
	demo_touch_mode_only_hrd_pc();   /* touch detect mode */
#endif

#ifdef ppg_only_hrd_en_pc
	DEBUG_PRINT("Run_ppg_only_hrd_en_mode_pc\r\n"); 
	demo_ppg_mode_only_hrd_pc();    /* ppg_mode always run mode */
#endif

#ifdef auto_hrd_en_pc
	DEBUG_PRINT("Run_auto_hrd_en_mode_pc\r\n");
	demo_auto_mode_hrd_pc();        /* Touch & PPG auto switch  mode */
#endif


/*============================================================================
STATIC FUNCTION PROTOTYPES
PPG_polling mode_HRD pc cover
============================================================================*/
#ifdef touch_only_polling_hrd_en_pc
    DEBUG_PRINT("Run_touch_only_polling_hrd_en_mode_pc\r\n");
    demo_touch_mode_only_polling_hrd_pc();  /* touch detect mode   -  polling */
#endif

#ifdef ppg_only_polling_hrd_en_pc
    DEBUG_PRINT("Run_ppg_only_polling_hrd_en_mode_pc\r\n");
    demo_ppg_mode_only_polling_hrd_pc();   /* ppg_mode always run mode  -  polling */ 
#endif

#ifdef auto_polling_hrd_en_pc
    DEBUG_PRINT("Run_auto_polling_hrd_en_mode_pc\r\n");
    demo_auto_mode_polling_hrd_pc();      /* Touch & PPG Auto switch  mode -  polling */
#endif


/*============================================================================
Test_pattern mode
============================================================================*/
 /* HRD*/
#ifdef hrd_testpattern_en
    test_library_HRD();
#endif

/* SPO2*/
#ifdef  spo2_testpattern_en
    test_library_SPO2();
#endif


/*============================================================================
Factory_Test Function
============================================================================*/
    // Factory Test example code
    // 产测模式示例代码
#ifdef factory_test_en
	/* 灰卡产测（金机测试适用）*/
    //factory_test_mode(factory_test_brightness_golden_sample, 1, 100, 100, 40);
    
	/* 灰卡产测（普通样机测试适用）*/
	//factory_test_mode(factory_test_brightness_target_sample, 1, 100, 100, 40);
    
	/* 漏光产测 */
	factory_test_mode(factory_test_leak_light_target_sample, 0, 0, 0, 0);
    
	/* 电源纹波验证 */
	//factory_test_mode(factory_test_power_noise, 0, 0, 0, 0);
#endif
}	// end of main()


//============================================================================
// Public function
//============================================================================
#if (INTERRUPT_MODE==1)
//*********** HRD  mode define****************//
#ifdef touch_only_hrd_en
void demo_touch_mode_only_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();
	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */
    while (true)
    {
        ppg_sensor_task_interrupt();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();

    }
}
#endif


#ifdef ppg_only_hrd_en
void demo_ppg_mode_only_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    hr_algorithm_enable();
	
	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
}
#endif


#ifdef auto_hrd_en
void demo_auto_mode_hrd_interrupt_handle(void)
{
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

}

void demo_auto_mode_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    auto_mode_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
#if 0
    while (true)
    {
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
#endif
}
#endif


//*********** SPO2  mode define****************//
#ifdef touch_only_spo2_en
void demo_touch_mode_only_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    touch_only_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();

    }
}
#endif


#ifdef ppg_only_spo2_en
void demo_ppg_mode_only_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    ppg_only_sensor_start();
    spo2_algorithm_enable();
	
	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call spo2_algorithm_disable() to disable spo2 algorithm  
		 * 请调用spo2_algorithm_disable()以关停SpO2算法库
         */
        //请调用spo2_algorithm_disable();		

    }
}
#endif


#ifdef auto_spo2_en
void demo_auto_mode_spo2_interrupt_handle(void)
{
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call spo2_algorithm_disable() to disable spo2 algorithm  
		 * 请调用spo2_algorithm_disable()以关停SpO2算法库
         */
        //请调用spo2_algorithm_disable();	

}
void demo_auto_mode_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
	auto_mode_sensor_start();
  
	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */  
#if 0
    while (true)
    {
        ppg_sensor_task_interrupt();
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call spo2_algorithm_disable() to disable spo2 algorithm  
		 * 请调用spo2_algorithm_disable()以关停SpO2算法库
         */
        //请调用spo2_algorithm_disable();	

    }
#endif
}
#endif


//*********** Stress  mode define****************//
#ifdef touch_only_stress_en
void demo_touch_mode_only_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();
	
	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();
        
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();

    }
}
#endif


#ifdef ppg_only_stress_en
void demo_ppg_mode_only_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    stress_algorithm_enable();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */    
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score );
    
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }
    
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call stress_algorithm_disable() to disable stress algorithm   
		 * 请调用stress_algorithm_disable()以关停压力算法库
         */
        //请调用stress_algorithm_disable();	

    }
}
#endif


#ifdef auto_stress_en
void demo_auto_mode_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    auto_mode_sensor_start();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score);
     
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call stress_algorithm_disable() to disable stress algorithm   
		 * 请调用stress_algorithm_disable()以关停压力算法库
         */
        //请调用stress_algorithm_disable();	

    }
}
#endif


//*********** HRD  mode define***PC COVER*************//
#ifdef touch_only_hrd_en_pc
void demo_touch_mode_only_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    touch_only_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();

    }
}
#endif


#ifdef ppg_only_hrd_en_pc
void demo_ppg_mode_only_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
	ppg_only_sensor_start();
    hr_algorithm_enable();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();
        hr_alg_task();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
}
#endif


#ifdef auto_hrd_en_pc
void demo_auto_mode_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    auto_mode_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();
        hr_alg_task();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
}
#endif


//*********** Stress  mode define****************//
#ifdef touch_only_stress_en_pc
void demo_touch_mode_only_stress_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
	touch_only_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
		ppg_sensor_task_interrupt();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();

    }
}
#endif


#ifdef ppg_only_stress_en_pc
void demo_ppg_mode_only_stress_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    ppg_only_sensor_start();
    stress_algorithm_enable();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */    
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score );
    
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }
    

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call stress_algorithm_disable() to disable stress algorithm   
		 * 请调用stress_algorithm_disable()以关停压力算法库
         */
        //请调用stress_algorithm_disable();	
    }
}
#endif


#ifdef auto_stress_en_pc
void demo_auto_mode_stress_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
    auto_mode_sensor_start();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    int score = 100 ;
    uint8_t stress_alg_enable = 1 ;

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        ppg_sensor_task_interrupt();
        stress_alg_task(score);
     
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call stress_algorithm_disable() to disable stress algorithm   
		 * 请调用stress_algorithm_disable()以关停压力算法库
         */
        //请调用stress_algorithm_disable();	

    }
}
#endif

#endif		// end of "#if (INTERRUPT_MODE==1)"


#if (POLLING_MODE==1)
#ifdef touch_only_polling_hrd_en
void demo_touch_mode_only_polling_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
        
    }
}
#endif

#ifdef ppg_only_polling_hrd_en
void demo_ppg_mode_only_polling_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    hr_algorithm_enable();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
#ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
}
#endif


#ifdef auto_polling_hrd_en
void demo_auto_mode_hrd_polling_handle(void)
{

    ppg_sensor_task_polling();
    
 #ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }

void demo_auto_mode_polling_hrd(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    auto_mode_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
     #if 0
    while (true)
    {
 #ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        hr_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
    #endif
}
#endif

#ifdef touch_only_polling_spo2_en
void demo_touch_mode_only_polling_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    touch_only_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();        
    }
}
#endif

#ifdef ppg_only_polling_spo2_en
void demo_ppg_mode_only_polling_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    ppg_only_sensor_start();
    spo2_algorithm_enable();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
#ifdef PPG_MEMS_Synchronize
        /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
         */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }
#else
        spo2_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call spo2_algorithm_disable() to disable spo2 algorithm  
		 * 请调用spo2_algorithm_disable()以关停SpO2算法库
         */
        //请调用spo2_algorithm_disable();

    }
}
#endif

#ifdef auto_polling_spo2_en
void demo_auto_mode_spo2_polling_handle(void)
{

    ppg_sensor_task_polling();
    
 #ifdef PPG_MEMS_Synchronize

    /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
     */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            hr_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
void demo_auto_mode_polling_spo2(void)
{
    pah_sensor_init(diw_4mm_ir_r_ir_spo2);
    auto_mode_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
     #if 0
    while (true)
    {
#ifdef PPG_MEMS_Synchronize
        /**if customer want to read mems FIFO then  run PPG alg  , please call mems_interpolation()
         */
        if(mems_data_polling_ready==1)
        {
            ppg_data_num= get_ppg_data_num();
            mems_interpolation(ppg_data_num);
            clear_ppg_data_num();
            spo2_alg_task2(ppg_data_num);
        }

#else
        spo2_alg_task();
#endif

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call spo2_algorithm_disable() to disable spo2 algorithm  
		 * 请调用spo2_algorithm_disable()以关停SpO2算法库
         */
        //请调用spo2_algorithm_disable();

    }
    #endif
}
#endif



#ifdef touch_only_polling_stress_en
void demo_touch_mode_only_polling_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    touch_only_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
        
    }
}
#endif

#ifdef ppg_only_polling_stress_en
void demo_ppg_mode_only_polling_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    ppg_only_sensor_start();
    stress_algorithm_enable();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    uint8_t stress_alg_enable = 1 ;
    int score = 100 ;

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
      
        stress_alg_task(score);
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call stress_algorithm_disable() to disable stress algorithm   
		 * 请调用stress_algorithm_disable()以关停压力算法库
         */
        //请调用stress_algorithm_disable();	

    }
}
#endif


#ifdef auto_polling_stress_en
void demo_auto_mode_polling_stress(void)
{
    pah_sensor_init(diw_4mm_g_ir_hrd);
    auto_mode_sensor_start();
    extern uint8_t Get_Stress;
    extern uint8_t Weak_Flag;
    uint8_t stress_alg_enable = 1 ;
    int score = 100 ;

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
        stress_alg_task(score);
        //if(Get_Stress==1 || Weak_Flag==1 )
        if((Get_Stress==1 || Weak_Flag==1 ) &&(stress_alg_enable==1))
        {
            pah_sensor_stop();
            stress_alg_enable = 0 ;
        }

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call stress_algorithm_disable() to disable stress algorithm   
		 * 请调用stress_algorithm_disable()以关停压力算法库
         */
        //请调用stress_algorithm_disable();	

    }
}
#endif

#ifdef touch_only_polling_hrd_en_pc
void demo_touch_mode_only_polling_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
	touch_only_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */		
    while (true)
    {

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
        
    }
}
#endif

#ifdef ppg_only_polling_hrd_en_pc
void demo_ppg_mode_only_polling_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
	ppg_only_sensor_start();
    hr_algorithm_enable();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */	
    while (true)
    {
		hr_alg_task();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
}
#endif


#ifdef auto_polling_hrd_en_pc
void demo_auto_mode_polling_hrd_pc(void)
{
    pah_sensor_init(pc_4mm_2g_ir_hrd);
	auto_mode_sensor_start();

	/* "while" loop here just reminds customer that the process should loop the functions 
     * inside, it doesn't mean customer should use "while" loop in a real project. 
     * 
     * 这里的"while"循环只是提醒客户流程需要循环执行此部分内含的函数，并不意味着客户应该在实际
     * 项目中使用"while"循环的方式
     */		
    while (true)
    {
		hr_alg_task();

        /**if customer want to stop PPG Sensor , please call pah_sensor_stop()
         * This Function will disable the ppg sensor
         * 
         * 如果客户想要停止PPG Sensor，请调用pah_sensor_stop()，此函数会停止PPG Sensor
         */
        //pah_sensor_stop();
		
        /**Please call hr_algorithm_disable() to disable hr algorithm  
		 * 请调用hr_algorithm_disable()以关停HR算法库
         */
        //hr_algorithm_disable();

    }
}
#endif
#endif	// #if (POLLING_MODE==1)


void rtc_handler(void)
{
#ifdef POLLING_MODE
    // if time out, call ppg_sensor_task_polling() (for polling mode only) 定时时间到的时候，调用ppg_sensor_task_polling()（只适用于轮询方式）
    {
        ppg_sensor_task_polling();
    }
#endif
}



#endif

void qah800x_rate_meas_timeout_handler(void * p_context)
{
    return;
}

void qah800x_blood_meas_timeout_handler(void * p_context)
{
    return;
}

void qah800x_handle_gpio_hr_interrupt(void)
{
#ifdef INTERRUPT_MODE
    // if MCU detect INT signal from sensor 如果MCU检测到sensor发的中断信号
    {
        ppg_interrupt_handle();		// call "ppg_interrupt_handle" function 则去调"ppg_interrupt_handle"函数
#ifdef auto_hrd_en
        demo_auto_mode_hrd_interrupt_handle();
#endif
    }
#endif
}

void qah800x_handle_gpio_spo2_interrupt(void)
{
#ifdef INTERRUPT_MODE
    // if MCU detect INT signal from sensor 如果MCU检测到sensor发的中断信号
    {
        ppg_interrupt_handle();		// call "ppg_interrupt_handle" function 则去调"ppg_interrupt_handle"函数
#ifdef auto_spo2_en
        demo_auto_mode_spo2_interrupt_handle();
#endif
    }
#endif
}

int pah8009_HR_Open(void)
{
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    ZDT_HSENSOR_TRACE("pah8009_HR_Open");
    PAH8009_I2C_Open();
    #ifdef auto_polling_hrd_en
        demo_auto_mode_polling_hrd();
    #else
    demo_auto_mode_hrd(); 
    #endif
    return 0;
}

void pah8009_HR_Close(void)
{
    ZDT_HSENSOR_TRACE("pah8009_HR_Close");
    pah_sensor_stop();
    hr_algorithm_disable();
    PAH8009_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
   
}

int pah8009_SPO2_Open(void)
{
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    ZDT_HSENSOR_TRACE("pah8009_SPO2_Open");
    PAH8009_I2C_Open();
    #ifdef auto_polling_spo2_en
        demo_auto_mode_polling_spo2();
    #else
        demo_auto_mode_spo2(); 
    #endif
    return 0;
}

void pah8009_SPO2_Close(void)
{
    ZDT_HSENSOR_TRACE("pah8009_SPO2_Close");
    pah_sensor_stop();
    spo2_algorithm_disable();
    PAH8009_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
   
}

int pah8009_BP_Open(void)
{
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    ZDT_HSENSOR_TRACE("pah8009_BP_Open");
    PAH8009_I2C_Open();
    return 0;
}

void pah8009_BP_Close(void)
{
    ZDT_HSENSOR_TRACE("pah8009_BP_Close");
    PAH8009_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
}



int pah8009_Factory_Open(void)
{
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    ZDT_HSENSOR_TRACE("pah8009_Factory_Open");
    PAH8009_I2C_Open();
    /* 漏光产测 */
    factory_test_mode(factory_test_leak_light_target_sample, 0, 0, 0, 0);
    return 0;
}

void pah8009_Factory_Close(void)
{
    ZDT_HSENSOR_TRACE("pah8009_Factory_Close");
    PAH8009_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
}

int pah8009_init(void)
{
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    ZDT_HSENSOR_TRACE("pah8009_init");
    PAH8009_I2C_Open();
    
    #ifdef auto_polling_spo2_en
        demo_auto_mode_polling_spo2();
    #else
        demo_auto_mode_spo2(); 
    #endif
    return 0;
}

/*----------------------------------------------------------------------------*/
void  pah8009_exit(void)
{
    pah_sensor_stop();
    spo2_algorithm_disable();
    PAH8009_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
   
}

BOOLEAN pah8009_CheckID(void)
{
    BOOLEAN res = FALSE;
    uint16_t product_id = 0;
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(0);
    #endif
    PAH8009_I2C_Open();
    pah_800x_get_product_id(&product_id);
    ZDT_HSENSOR_TRACE("pah8009_CheckID id=0x%x",product_id);
    if (product_id == PAH_800x_PRODUCT_ID)
    {
        res = TRUE;
    }
    PAH8009_I2C_Close();
    #ifndef WIN32
        SCI_HSensor_EnableDeepSleep(1);
    #endif
    return res;
}
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif  // End of file


