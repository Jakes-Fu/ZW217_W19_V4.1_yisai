/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmidrv_i2c.c
**	
**		This module implements the i2c interface NMI ATV   
**
** 
*******************************************************************************/
#include "nmidrv_custom.h"
#include "nmiport.h"
#include "os_api.h"
#ifndef ATV_USE_HW_I2C


/******************************************************************************
**
**	I2c Defines
**
*******************************************************************************/

//#define _DRIVE_	//this mode is nmi600 Recommend mode. please don't change it. 
//#define _GPIO_I2C_DEBUG_
#ifdef _GPIO_I2C_DEBUG_
#define NMI_GPIO_I2C_PRINT SCI_TRACE_LOW
#else
#define NMI_GPIO_I2C_PRINT
#endif

//Fast gpio control to speed up gpio i2c
/* define GPIO register address */
#define	BASE_GPIO_TIMER 0x20090000		/* GPIO base address */
#define GPIO_DIR        0x00000000		/* input/output select register */
#define GPIO_IN         0x0000000C		/* Port input real time data value */
#define GPIO_OUT        0x00000010		/* Port output real time data value */


#define DEF_GPIO_FAST_SET_REG_VALUE(_offset_) \
    unsigned long pg_base = 0x8A000000 + (gpio_id>>4) * 0x80;   \
    unsigned long bit_offset = gpio_id & 0xF;                                 \
    unsigned long pg_offset = (_offset_);                                    \
    unsigned long old_value = *(volatile unsigned long *)(pg_base+pg_offset); 
 
#define GPIO_FAST_SET_REG                         \
    *(volatile unsigned long *)(pg_base+pg_offset) = (old_value & (~(1<<bit_offset)))|(b_on<<bit_offset);    
 
#define GPIO_FAST_GET_REG                        \
    return ((old_value & (1<<bit_offset)) ? 1 : 0);
 
void GPIO_SetValue_Fast(unsigned long gpio_id, unsigned char b_on)
{
    DEF_GPIO_FAST_SET_REG_VALUE(0)
    GPIO_FAST_SET_REG
}
 
unsigned char GPIO_GetValue_Fast(unsigned long gpio_id)
{
    DEF_GPIO_FAST_SET_REG_VALUE(0);
    GPIO_FAST_GET_REG
}
 
//b_on=1 out; 0 in
void GPIO_SetDirection_Fast(unsigned long gpio_id, unsigned char b_on)
{
    DEF_GPIO_FAST_SET_REG_VALUE(0x8)
    GPIO_FAST_SET_REG    
}


#define  I2C_DELAY_UNIT        1 ///////////4

#define set_sda_output()               GPIO_SetDirection_Fast(NMI_SDA_PIN,1); \
                                                       GPIO_SetValue_Fast(NMI_SDA_PIN,0)
#define set_sda_input()                  GPIO_SetDirection_Fast(NMI_SDA_PIN,0) 
#define set_scl_output()                 GPIO_SetDirection_Fast(NMI_SCL_PIN,1);  \
                                                        GPIO_SetValue_Fast(NMI_SCL_PIN,0)
#define set_scl_input()                    GPIO_SetDirection_Fast(NMI_SCL_PIN,0)

#ifdef _DRIVE_
#define set_i2c_scl_PIN  	 GPIO_SetValue_Fast(NMI_SCL_PIN,1)		

#define clr_i2c_scl_PIN   	GPIO_SetValue_Fast(NMI_SCL_PIN,0)         
#define set_i2c_sda_PIN  	    GPIO_SetValue_Fast(NMI_SDA_PIN,1)        
#define clr_i2c_sda_PIN    	GPIO_SetValue_Fast(NMI_SDA_PIN,0)     
#else
#define set_i2c_scl_PIN  			GPIO_SetValue_Fast(NMI_SCL_PIN,1)	
#define clr_i2c_scl_PIN   			GPIO_SetValue_Fast(NMI_SCL_PIN,0)
#define set_i2c_sda_PIN  			GPIO_SetDirection_Fast(NMI_SDA_PIN,0)
#define clr_i2c_sda_PIN    		       GPIO_SetDirection_Fast(NMI_SDA_PIN,1)
#endif

#define get_i2c_sda_PIN               GPIO_GetValue_Fast(NMI_SDA_PIN)		
#define get_i2c_scl_PIN                GPIO_GetValue_Fast(NMI_SCL_PIN)




/******************************************************************************
**
**	I2c Platform Functions
**
*******************************************************************************/
uint8 itemp;
#define i2c_delay(_TIME)  for(itemp=0;itemp<(_TIME);itemp++){;}

static void i2c_begin(void)
{
#ifdef _DRIVE_
	set_sda_output();
	set_scl_output();
	
	/* set SDA to high */
	set_i2c_sda_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);
	
	/* set SCL to high */
	set_i2c_scl_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SDA to low */
	clr_i2c_sda_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SCL to low */
	clr_i2c_scl_PIN;		
	i2c_delay(I2C_DELAY_UNIT << 0);

#else

	/* set SDA to high */
	set_sda_input();
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SCL to high */
	set_scl_input();
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SDA to low */
	set_sda_output();
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SCL to low */
	set_scl_output();		
	i2c_delay(I2C_DELAY_UNIT << 0);
#endif
	NMI_GPIO_I2C_PRINT( "i2c_begin");

}

static void i2c_end(void)
{
#ifdef _DRIVE_
	/* set SDA to low */
	clr_i2c_sda_PIN;	
	i2c_delay(I2C_DELAY_UNIT << 2);

	/* set SCL to high */ 
	set_i2c_scl_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SDA to high */
	set_i2c_sda_PIN;		
	i2c_delay(I2C_DELAY_UNIT << 0);

#else
	set_sda_output();
	i2c_delay(I2C_DELAY_UNIT << 2);
	set_scl_input();
	i2c_delay(I2C_DELAY_UNIT << 3);
	set_sda_input();
	i2c_delay(I2C_DELAY_UNIT << 4);
#endif
}

static void i2c_write_ask(unsigned char flag)
{
#ifdef _DRIVE_
	/* set SDA to high to ack */
	if(flag)
    {   
		set_i2c_sda_PIN;
       }
	else
	{
		clr_i2c_sda_PIN;
       }
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* toggle clock */
	set_i2c_scl_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);
	clr_i2c_scl_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SDA to 1 */
	set_i2c_sda_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);

#else

   	//set_sda_output();

	if(flag)
		set_sda_input();
	//else
		//set_sda_output();
	
	i2c_delay(I2C_DELAY_UNIT << 0);
	set_scl_input();
	i2c_delay(I2C_DELAY_UNIT << 0);
	set_scl_output();
	i2c_delay(I2C_DELAY_UNIT << 0);
	set_sda_input();
	i2c_delay(I2C_DELAY_UNIT << 0);
#endif
}

static unsigned char i2c_read_ack(void)
{
	unsigned char ret;
	//NMI_GPIO_I2C_PRINT( "i2c_read_ack 1");
#ifdef _DRIVE_
	/* set SDA to input */
	set_sda_input();
	/* delay */
	i2c_delay(I2C_DELAY_UNIT << 0);
	
	/* read */
	if (!get_i2c_sda_PIN) {
		ret = 1;
	} else {
		ret = 0;
		NMI_GPIO_I2C_PRINT( "1. i2c_read_ack (Error.. No Ack received)");
		i2c_delay(I2C_DELAY_UNIT << 0);
		if (!get_i2c_sda_PIN) {
			ret = 1;
			NMI_GPIO_I2C_PRINT( "2.i2c_read_ack (Correct after additional delay.)");	
		} else {
			ret = 0;
			NMI_GPIO_I2C_PRINT( "2.i2c_read_ack (Error.. No Ack received)");
		}
	}

	/* set SCL high */
	set_i2c_scl_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SCL low */
	clr_i2c_scl_PIN;
	i2c_delay(I2C_DELAY_UNIT << 0);

	/* set SDA back to output */
	set_sda_output();

#else

	set_sda_input();
	i2c_delay(I2C_DELAY_UNIT << 0);

	if (!get_i2c_sda_PIN) {
		ret = 1;
	} else {
		ret = 0;
		NMI_GPIO_I2C_PRINT( "1. i2c_read_ack (Error.. No Ack received)");
		i2c_delay(I2C_DELAY_UNIT << 0);
		if (!get_i2c_sda_PIN) {
			ret = 1;
			NMI_GPIO_I2C_PRINT( "2.i2c_read_ack (Correct after additional delay.)");	
		} else {
			ret = 0;
			NMI_GPIO_I2C_PRINT( "2.i2c_read_ack (Error.. No Ack received)");
		}
	}
	
	set_scl_input();
	i2c_delay(I2C_DELAY_UNIT << 0);
	set_scl_output();
	i2c_delay(I2C_DELAY_UNIT << 0);
	   
#endif
	NMI_GPIO_I2C_PRINT( "i2c_read_ack exit");
	return ret;
}

static unsigned char i2c_read_byte(void)
{
	unsigned char i;
	unsigned char ret = 0;

#ifdef _DRIVE_

	/* set SDA input */
	set_sda_input();
	
	/* loop */
	for (i = 0; i < 8; i++) {			
		/* delay */
		i2c_delay(I2C_DELAY_UNIT << 0);

		/* set SCL high */
		set_i2c_scl_PIN;
		/* delay */
		i2c_delay(I2C_DELAY_UNIT << 0);

		/* read SDA */
		ret	= ret<<1;
		if (get_i2c_sda_PIN)
			ret |= 1;
		/* delay */
		i2c_delay(I2C_DELAY_UNIT << 0);

		/* set SCL low */
		clr_i2c_scl_PIN;
		/* delay */
		i2c_delay(I2C_DELAY_UNIT << 0);

		/* if end, set SDA output */
		if (i == 7) {
			set_sda_output();
      	}
		/* delay */
     	i2c_delay(I2C_DELAY_UNIT << 0);
	}	

#else
	int retry,retry_val = 10000000;

	ret	= 0;

	set_sda_input();
	for (i = 0; i < 8; i++) {			
		i2c_delay(I2C_DELAY_UNIT << 0);

		set_scl_input();	

		i2c_delay(I2C_DELAY_UNIT << 0);
		ret	= ret<<1;
		if (get_i2c_sda_PIN)
			ret |= 1;
		i2c_delay(I2C_DELAY_UNIT << 0);

		retry = retry_val;
		while (retry >= 0) 
		{
			if (get_i2c_scl_PIN)
				break;
			else
			{
				i2c_delay(I2C_DELAY_UNIT << 0);
				retry--;
			}
		}

		//if (retry != retry_val)
		if (retry < (retry_val-10000))
		{
			NMI_GPIO_I2C_PRINT("i2c_read_byte: retry = %d\n",retry);
		}

		set_scl_output();
      		i2c_delay(I2C_DELAY_UNIT << 0);

		if (i==7){
		set_sda_output();
      		}
     		i2c_delay(I2C_DELAY_UNIT << 0);
	}		

#endif	

	return ret;
}

static unsigned char i2c_write_byte(unsigned char data)
{
	unsigned char i;

#ifdef _DRIVE_
	/* loop */
	for	(i = 0; i < 8; i++) {
		/* set SDA high or low depend on the data bit */
		if (data & 0x80)
            {      
			set_i2c_sda_PIN;
            }
		else
            {		
			clr_i2c_sda_PIN;
            }
		/* delay */
		i2c_delay(I2C_DELAY_UNIT << 0);

		data <<= 1;

		/* set SCL high */
		set_i2c_scl_PIN;
		/* delay */
		i2c_delay(I2C_DELAY_UNIT << 0);

		/* set SCL low */
		clr_i2c_scl_PIN;
		/* delay */
		i2c_delay(I2C_DELAY_UNIT << 0);
	}
#else
	int retry, retry_val = 10000000;

	//set_sda_output();

	for	(i = 0; i < 8; i++) {
		if (data & 0x80)
			set_sda_input();
		else
			set_sda_output();

		data <<= 1;

		i2c_delay(I2C_DELAY_UNIT << 0);
		set_scl_input();
		i2c_delay(I2C_DELAY_UNIT << 0);
		retry = retry_val;
		while (retry >= 0) 
		{
			if (get_i2c_scl_PIN)
				break;
			else
			{
				i2c_delay(I2C_DELAY_UNIT << 0);
				retry--;
			}
		}

		//if (retry != retry_val)
		if (retry < (retry_val-10000))
		{
			NMI_GPIO_I2C_PRINT("i2c write_byte: retry = %d\n",retry);
		}
		set_scl_output();		
		i2c_delay(I2C_DELAY_UNIT << 0);
	}
	
#endif
	NMI_GPIO_I2C_PRINT("i2c_write_byte\n");
	return i2c_read_ack();
}

/******************************************************************************
**
**	I2c Global Functions
**
*******************************************************************************/

int nmi_i2c_init(void)
{
	NMI_GPIO_I2C_PRINT( "nmi_i2c_init: enter...\n");

       //disable all inside pull( pullup & pulldown)
	NMI_SET_GPIO_PULL_DISABLE(NMI_SDA_PIN);
	NMI_SET_GPIO_PULL_DISABLE(NMI_SCL_PIN);
	//set gpio mode
	NMI_SET_GPIO_MODE_ENABLE(NMI_SDA_PIN);
	NMI_SET_GPIO_MODE_ENABLE(NMI_SCL_PIN);

#ifdef _DRIVE_
	//set output mode
	NMI_SET_GPIO_DIR( NMI_SDA_PIN,1);
	NMI_SET_GPIO_DIR( NMI_SCL_PIN,1);
      //set gpio high
	NMI_SET_GPIO_LEVEL( NMI_SDA_PIN,1);
	NMI_SET_GPIO_LEVEL( NMI_SCL_PIN,1);
#else
	//set input mode
	NMI_SET_GPIO_DIR( NMI_SDA_PIN,0);
	NMI_SET_GPIO_DIR( NMI_SCL_PIN,0);
#endif

	NMI_GPIO_I2C_PRINT( "nmi_i2c_init: exit...\n");

	return 1;
}

void nmi_i2c_deinit(void)
{
	NMI_GPIO_I2C_PRINT( "nmi_i2c_deinit: enter...\n");

	NMI_SET_GPIO_MODE_ENABLE(NMI_SDA_PIN);
	NMI_SET_GPIO_MODE_ENABLE(NMI_SCL_PIN);

	//set as input
	NMI_SET_GPIO_DIR( NMI_SDA_PIN,0);
	NMI_SET_GPIO_DIR( NMI_SCL_PIN,0);

	NMI_GPIO_I2C_PRINT( "nmi_i2c_deinit: exit...\n");

}

int nmi_i2c_read(unsigned char adr, unsigned char *b, unsigned long sz)
{
	int i;
	NMI_GPIO_I2C_PRINT("GPIO i2c,read\n");
	i2c_begin();
	i2c_write_byte((adr << 1)|0x1);

	for(i = 0; i < sz; i++) {
	    b[i] = i2c_read_byte();	    
		
	    if(i == sz-1) 
			i2c_write_ask(1);	
	    else  	           
			i2c_write_ask(0);
	}
 
	i2c_end();
	return 1;
}

int nmi_i2c_write(unsigned char adr, unsigned char *b, unsigned long sz)
{
	int i;
	NMI_GPIO_I2C_PRINT("GPIO i2c,write\n");
	i2c_begin();

	i2c_write_byte((adr << 1));
	for(i = 0; i < sz; i++) {
		i2c_write_byte(b[i]);
	}

	i2c_end();
	return 1;
}
#endif 

