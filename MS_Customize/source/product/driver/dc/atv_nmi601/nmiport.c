/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmiport.c
**	
**		This module implements the porting interface for the NMI ATV driver.
**		It can be used as an example for the driver porting.      
**
** 
*******************************************************************************/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "nmitypes.h"
#include "nmiport.h"
#include "nmiapi.h"
#include "nmidrv_custom.h"
#include "fm_cfg.h"
#include "arm_reg.h"
#include "sc_reg.h"

#ifndef ATV_USE_HW_I2C
#include "gpio_drv.h"
#endif


/*lint -save -e533 -e578*/

/******************************************************************************
**
**	I2c Defines
**
*******************************************************************************/


/******************************************************************************
**
**	Debug
**
*******************************************************************************/
#define N_ERR		0x00000001
#define N_TRACE	0x00000002
#define N_INFO		0x00000004

static unsigned long dbgflag = N_ERR | N_INFO | N_TRACE;
extern char buf[1024];

static void dPrint(unsigned long flag, char *fmt, ...)
{
	//char buf[256];
	va_list args;
	int len;

	if (flag & dbgflag) { 
		va_start(args, fmt);
		strcpy(buf, "[Nmi Port]: "); 
		len = vsprintf(&buf[12], fmt, args);
		va_end(args);
		//SCI_TRACE_LOW:"%s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIPORT_56_112_2_18_0_29_36_625,(uint8*)"s", buf);
	}

	return;
}

/******************************************************************************
**
**	Power Global Functions
**
*******************************************************************************/

/*****************************************************************************/
//  Description:    This function is used to control the sensor power if the
//                  the sensor is enabled by a gpio.
//                  b_on = SCI_TRUE, open sensor power
//                  b_on = SCI_FALSE, close sensor power
//  Author:         junyi.lv
//  Note:
/*****************************************************************************/

void nmi_clk_on(void)  
{
#ifdef  PLATFORM_SC6800H
    volatile uint32 reg_val = *(volatile uint32*)GR_ANA_CTL;
                              reg_val |= BIT_22;
                              *(volatile uint32*)GR_ANA_CTL = reg_val;
#endif
}

void nmi_clk_off(void)  
{
#ifdef  PLATFORM_SC6800H
    volatile uint32 reg_val = *(volatile uint32*)GR_ANA_CTL;
                              reg_val &= ~BIT_22;
                              *(volatile uint32*)GR_ANA_CTL = reg_val;
#endif                           
}

void nmi_power_up(void)
{
    nmi_clk_on();
    nmi_os_delay(10);  //100ms
}

void nmi_power_down(void)
{
    nmi_clk_off();
    nmi_os_delay(10);  //100ms
}

#ifndef ATV_USE_HW_I2C
/******************************************************************************
**
**	GPIO Functions
**
*******************************************************************************/
extern PUBLIC void GPIO_Enable(uint32 gpio_id);
extern PUBLIC void GPIO_SetDirection(uint32  gpio_id, BOOLEAN directions);
extern PUBLIC void GPIO_SetValue(uint32 gpio_id, BOOLEAN b_on);
extern PUBLIC BOOLEAN  GPIO_GetValue( uint32  gpio_id );
//set gpio pin as gpio mode
void NMI_SET_GPIO_MODE_ENABLE(unsigned char pin)
{
 	GPIO_Enable(pin);
      //if(pin!=NMI_PIN_UNUSE)    GPIO_ModeSetup( pin, 0);
}
//set gpio direction, dir=0: input, dir=1:output
void NMI_SET_GPIO_DIR(unsigned char pin,unsigned char dir) 
{
 	GPIO_SetDirection(pin, dir);
     	//if(pin!=NMI_PIN_UNUSE)    GPIO_InitIO(dir, pin);
}
//set gpio high(level=1) or low(level=0)
void NMI_SET_GPIO_LEVEL(unsigned char pin,unsigned char level)     
{
 	GPIO_SetValue(pin, level);
    	//if(pin!=NMI_PIN_UNUSE)    GPIO_WriteIO(level, pin); 
} 
//set gpio pin pullup & pulldown disable
void NMI_SET_GPIO_PULL_DISABLE(unsigned char pin) 
{
   	// if(pin!=NMI_PIN_UNUSE)   GPIO_PullenSetup( pin,0);
}
//get gpio level
unsigned char NMI_GET_GPIO_LEVEL(unsigned char pin)             
{
 	return GPIO_GetValue(pin);
	//if(pin!=NMI_PIN_UNUSE)    
	//return GPIO_ReadIO(pin);
}
#endif

/******************************************************************************
**
**	I2c Global Functions
**
*******************************************************************************/
#ifdef ATV_USE_HW_I2C
#include "i2c_drvapi.h"


#define NMI600_I2C_WRITE_ADDR  0xc0

/*
extern PUBLIC BOOLEAN  Sensor_IsOpen(void) ;
extern PUBLIC uint32 Sensor_GetI2cHnadler(void) ;
*/
LOCAL I2C_DEV NMI600_IIC_cfg = 
{
	0,            	    	//id, will be reinit during init()/ open()
	100*1000,   			//freq
	0,						//bus
	NMI600_I2C_WRITE_ADDR,     //slave_addr :(uint8)g_nmi600_yuv_info->salve_i2c_addr_w
	0,						//reg_addr_num
	1,						//check_ack
	0						// no_stop
};

LOCAL int32 nmi600_dev_handler = -1;

LOCAL int32 _NMI600_I2cHandlerInit(void) 
{ 
#ifdef FM_SUPPORT_NMI601
    NMI600_IIC_cfg.id = FM_GetCtrlPort();
#else
    NMI600_IIC_cfg.id = 0;
#endif
    if((nmi600_dev_handler == -1) && 
		((nmi600_dev_handler = I2C_HAL_Open(&NMI600_IIC_cfg)) == -1))
	{
		//SCI_TRACE_LOW("_NMI600_:"__func__"==> open IIC failed!");
		return -1;
	}
	
	return nmi600_dev_handler;
}

LOCAL void _NMI600_I2cHandlerRelease(void) 
{

    if(nmi600_dev_handler != -1)
	{
 		I2C_HAL_Close(nmi600_dev_handler);
		nmi600_dev_handler = -1;
	}
}

int nmi_i2c_init(void)
{
    _NMI600_I2cHandlerInit();
	return 1;
}

void nmi_i2c_deinit(void)
{
    _NMI600_I2cHandlerRelease();
}

int nmi_i2c_read(unsigned char adr, unsigned char *b, unsigned long sz)
{ 
    /*
    if(Sensor_IsOpen())
    {
        I2C_HAL_Read(Sensor_GetI2cHnadler(), 0, b, sz);    
    }
    else
    */
    {
        I2C_HAL_Read(nmi600_dev_handler, 0, b, sz);      
    }
	return 1;
}

int nmi_i2c_write(unsigned char adr, unsigned char *b, unsigned long sz)
{  
    /*
    if(Sensor_IsOpen())
    {
        I2C_HAL_Write(Sensor_GetI2cHnadler(), 0, b, sz);    
    }
    else
    */
    {
        I2C_HAL_Write(nmi600_dev_handler, 0, b, sz);        
    }
    return 1;
}
#endif
/******************************************************************************
**
**	Os dependent Functions
**
/******************************************************************************

/* 
 power save mode ---keep VDDIO on and VCORE off, and set reset pin low.
 this mode keeps the bt656 bus hign impedance state, so nmi600 does't affect other devices,such image sensor.
*/
void nmi_power_save_mode(void)
{
	dPrint(N_TRACE, "nmi_power_save: enter...\n");
	
	//set VDDIO on and VCORE off
	////2.8V ldo on
	////1.2V ldo off
	//GPIO_PowerSaveTV();
	nmi_os_delay(10);  //10ms
	
	//set reset low
	//GPIO_ResetTV( FALSE );
	nmi_os_delay(10);  //10ms

	dPrint(N_TRACE, "nmi_power_save: exit...\n");

}
/* 
 power sleep mode ---keep VDDIO on and VCORE on, and set reset pin high.
 this mode keeps the bt656 bus hign impedance state, so nmi600 does't affect other devices,such image sensor.
*/
void nmi_power_sleep_mode(void)
{
	dPrint(N_TRACE, "nmi_power_sleep: enter...\n");
	
	//set VDDIO on and VCORE off
	////2.8V ldo on
	////1.2V ldo on
	//GPIO_PowerOnTV();
	nmi_os_delay(10);  //10ms
	
	//set reset high
	//GPIO_ResetTV( TRUE);
	nmi_os_delay(10);  //10ms

	dPrint(N_TRACE, "nmi_power_sleep: exit...\n");

}
uint32_t nmi_os_read_32bit_register(uint32_t adr)
{
	uint8_t b[16];
	uint8_t r[4]={0};
	uint32_t val = 0;
	int len;

	b[0] = 0x80;										/* word access */
	b[1] = (uint8_t)(adr >> 24);
	b[2] = (uint8_t)(adr >> 16);
	b[3] = (uint8_t)(adr >> 8);
	b[4] = (uint8_t)(adr);
	b[5] = 0x04;
	len = 6;

    
	if (nmi_i2c_write(0x60, b, len) < 0) {
		//SCI_TRACE_LOW:"Failed, bus write...\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIPORT_300_112_2_18_0_29_37_626,(uint8*)"");

		return 0;
	}

	if (nmi_i2c_read(0x60, r, 4) < 0) {
		//SCI_TRACE_LOW:"Failed, bus read...\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIPORT_306_112_2_18_0_29_37_627,(uint8*)"");
	}

	val =  (uint32_t)r[3] << 24 |(uint32_t)r[2] << 16 | (uint32_t)r[1] << 8 |r[0] ;
	//SCI_TRACE_LOW:"nmi600 adr(0x%08x),val(0x%08x)\n\r"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIPORT_310_112_2_18_0_29_37_628,(uint8*)"dd",adr,val);
	return val; 
}

PUBLIC BOOLEAN NMI_ReadChipID()
{
	uint32_t nmi_chip_id = 0;
	BOOLEAN ret = FALSE;
	
     #ifdef ATV_USE_HW_I2C  //2011.05.19 :jackey.ling add for get IIC handle
        nmi_i2c_init(); 
     #endif
 
	nmi_chip_id = nmi_os_read_32bit_register(0x20f00);
	
	if((nmi_chip_id & 0xfffff) == 0x600c0)
	{	
		ret = TRUE;	//600c0
	}
	else if((nmi_chip_id & 0xfffff) == 0x601a0)
	{
	
	    nmi_chip_id = nmi_os_read_32bit_register(0x210fc);	
		if ((nmi_chip_id & 0xff) == 0x2)
			ret = TRUE; //601a1
	}
	else
		ret = FALSE;

    #ifdef ATV_USE_HW_I2C
        nmi_i2c_deinit(); 
    #endif
    
        return ret;
}
PUBLIC void NMIAPP_InitVideoSize(BOOLEAN IsFullScreen)
{
	//SCI_TRACE_LOW:"NMIAPP_InitVideoSize fullscreen:%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIPORT_347_112_2_18_0_29_37_629,(uint8*)"d",IsFullScreen);
    //ISP_ServiceSetTVSize(TV_IsNTSCVideoSize());
}
void nmi_os_delay(unsigned long ms)
{
	//OS_TickDelay(ms);
	SCI_SLEEP(ms);
}

unsigned long nmi_os_get_tick(void)
{
	unsigned int ticks=1;

	ticks = OS_GetTickCount(); 

	return  ticks;
}

void nmi_os_print(char *log)
{
	//SCI_TRACE_LOW:"%s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,NMIPORT_366_112_2_18_0_29_37_630,(uint8*)"s", log);
}

void *nmi_os_create_mutex(void)
{

	return (void *)NULL; //mu;
}

void nmi_os_lock_mutex(void *mu)
{

}

void nmi_os_unlock_mutex(void *mu)
{
}

void nmi_os_del_mutex(void *mu)
{

}

/******************************************************************************
**
**	Signal Poll Status Functions
**
*******************************************************************************/
void nmi_poll_start(void)
{
}

void nmi_poll_stop(void)
{
}
