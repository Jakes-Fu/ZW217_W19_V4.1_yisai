/************************************************************************************************
*
*
*		  (C) Copyright	Telegent Systems, Inc. 2006	All	right reserved.
*				Confidential Information
*
*		  TLG1100  DEVICE ID: 0x2c
*
*		  Description: TLG1100 I2C timing sample code,code need	to OPTIMIZE
*
*		  Author:		 Zeng Hai
*
*		  Last modification	: 2006.	05.	22
*************************************************************************************************/

//#include "Platform.h"


//extern void GPIO_WriteIO(char data, char port);
//extern void GPIO_InitIO(char direction, char port);

/******************************************************************************************
*			   Macros
*******************************************************************************************/
#include "tlgInclude.h"
//#include "gpio_drv_internal.h"

#define	I2C_DELAY_UNIT		8
#define 	TLG_IIC_OUTPUT		1
#define 	TLG_IIC_INPUT     	0

/* define I2C interface	*/
#define	SDA					65			/* I2C serial interface	data I/O */
#define	SCL					64			/* I2C serial interface	clock input	*/
//#define	SDA					GPIO_Get_TlgI2cSDA()                    		
//#define	SCL					GPIO_Get_TlgI2cSCL()

#define	IIC_TRUE			1
#define	IIC_FALSE			0
#define	IIC_ERR				0
#define	IIC_DONE			1


#define	ENTRY(m)								/* These two defines cause warnings when */
#define	EXIT(m)									/* FUNCTION_CALL_TRACING is not defined */


/******************************************************************************************
*			   TYPES
*******************************************************************************************/
typedef	unsigned char	BOOL;

extern int Sensor_GetI2cHnadler(void);
/******************************************************************************************
*			   Prototypes
*******************************************************************************************/
void i2c_init(BOOL on);
static void	i2c_delay(unsigned int time);
static void	set_i2c_pin(unsigned int pin);
static void	clr_i2c_pin(unsigned int pin);
static void set_i2c_pin_dir(unsigned char Dir);
static unsigned	char get_i2c_pin(unsigned int pin);

static void	i2c_begin(void);
static void	i2c_end(void);
static void	i2c_write_ack(unsigned char	flag);
static unsigned	char i2c_read_ack(void);
static unsigned	char i2c_read_byte(void);
static unsigned	char i2c_write_byte(unsigned char data);


/******************************************************************************************
*
*				FUNCTION
*
*******************************************************************************************/

void tlg_i2c_init(BOOL on)
{
#if 0
	if(on)
	{
		GPIO_SetDirection(SDA, 1);	
		GPIO_SetDirection(SCL, 1);
		GPIO_SetValue(SDA, 1);
		GPIO_SetValue(SCL, 1);
	/*
		GPIO_ModeSetup(SDA, 0);
		GPIO_InitIO(1, SDA);
		GPIO_ModeSetup(SCL, 0);
		GPIO_InitIO(1, SCL);
	
		GPIO_WriteIO(1, SDA);
		GPIO_WriteIO(1, SCL);
	*/	
	}
	else
	{
		GPIO_SetDirection(SDA, 1);	
		GPIO_SetDirection(SCL, 1);
		GPIO_SetValue(SDA, 0);
		GPIO_SetValue(SCL, 0);
	
	/*	
		GPIO_ModeSetup(SDA, 0);
		GPIO_InitIO(1, SDA);
		GPIO_ModeSetup(SCL, 0);
		GPIO_InitIO(1, SCL);
	
		GPIO_WriteIO(0, SDA);
		GPIO_WriteIO(0, SCL);
	*/
	}
#endif	
}


// billy@wingtech add for more quik gpio ACTION! 2009/06/24
static void TV_GPIO_SetDir(unsigned int gpio_id , unsigned char value)
{
#if 0
	unsigned int gpio_pg_base=(gpio_id>>4) * 0x80 + GPIO_PG_BASE;
	unsigned int old_reg_value=0xffff;
	unsigned short bit_num=gpio_id & 0xF;
	unsigned short pg_offset=GPIO_DMSK;

	if(gpio_id==0xffff) return;
	if(!((*(volatile uint32*)(gpio_pg_base + pg_offset) & (1<<bit_num)) ? SCI_TRUE : SCI_FALSE))
	{
      		SCI_PASSERT(0, ("[GPIO_DRV]GPIO_%d data mask no opened!",  gpio_id));
	}
	pg_offset=GPIO_DIR;
	old_reg_value = GPIO_REG_VALUE(gpio_pg_base + pg_offset);	
	TB_REG_SET((gpio_pg_base + pg_offset), ((old_reg_value & ~(1<<bit_num)) | ((value ? SCI_TRUE : SCI_FALSE)<<bit_num)));
#else
	return ;
#endif
}

// billy@wingtech add for more quik gpio ACTION! 2009/06/24
static void TV_GPIO_SetSate(unsigned int gpio_id , unsigned char value)
{
#if 0
	unsigned int gpio_pg_base=(gpio_id>>4) * 0x80 + GPIO_PG_BASE;
	unsigned int old_reg_value=0xffff;
	unsigned short bit_num=gpio_id & 0xF;
	unsigned short pg_offset=GPIO_DMSK;

	if(gpio_id==0xffff) return;
	if(!((*(volatile uint32*)(gpio_pg_base + pg_offset) & (1<<bit_num)) ? SCI_TRUE : SCI_FALSE))
	{
      		SCI_PASSERT(0, ("[GPIO_DRV]GPIO_%d data mask no opened!",  gpio_id));
	}
	pg_offset=GPIO_DIR;
	if(!((*(volatile uint32*)(gpio_pg_base + pg_offset) & (1<<bit_num)) ? SCI_TRUE : SCI_FALSE))
	{
	   	SCI_PASSERT(0, ("[GPIO_DRV]GPIO_%d dir wrong!",     gpio_id));
	}
	pg_offset=GPIO_DATA;
	old_reg_value = GPIO_REG_VALUE(gpio_pg_base + pg_offset);	
	TB_REG_SET((gpio_pg_base + pg_offset), ((old_reg_value & ~(1<<bit_num)) | ((value ? SCI_TRUE : SCI_FALSE)<<bit_num)));
#else
	return ;
#endif
}

// billy@wingtech add for more quik gpio ACTION! 2009/06/24
static unsigned char TV_GPIO_GetSate(unsigned int gpio_id)
{
#if 0
	unsigned int gpio_pg_base=(gpio_id>>4) * 0x80 + GPIO_PG_BASE;
	unsigned short bit_num=gpio_id & 0xF;
	unsigned short pg_offset=GPIO_DMSK;

	if(gpio_id==0xffff) return;
	if(!((*(volatile uint32*)(gpio_pg_base + pg_offset) & (1<<bit_num)) ? SCI_TRUE : SCI_FALSE))
	{
      		SCI_PASSERT(0, ("[GPIO_DRV]GPIO_%d data mask no opened!",  gpio_id));
	}
	pg_offset=GPIO_DIR;
	if(((*(volatile uint32*)(gpio_pg_base + pg_offset) & (1<<bit_num)) ? SCI_TRUE : SCI_FALSE))
	{
	   	SCI_PASSERT(0, ("[GPIO_DRV]GPIO_%d dir wrong!",     gpio_id));
	}
	pg_offset=GPIO_DATA;
       return  ((*(volatile uint32*)(gpio_pg_base + pg_offset) & (1<<bit_num)) ? SCI_TRUE : SCI_FALSE);
#else
	return 0;
#endif

}


static void set_i2c_pin_dir(unsigned char Dir)
{
	if(Dir == TLG_IIC_OUTPUT)
	{
		TV_GPIO_SetDir(SDA, 1);
		//GPIO_SetDirection(SDA, 1);//set pin as output
	}
	else
	{
		TV_GPIO_SetDir(SDA, 0);
		//GPIO_SetDirection(SDA, 0);//set pin as input
	}
}

static void set_i2c_pin(unsigned int pin)
{
	TV_GPIO_SetSate(pin,1);
	//GPIO_SetValue(pin, 1);
}

static void clr_i2c_pin(unsigned int pin)
{
	TV_GPIO_SetSate(pin,0);
	//GPIO_SetValue(pin, 0);
}

static unsigned char get_i2c_pin(unsigned int pin)
{
	unsigned char ret;
	unsigned int value;

	return TV_GPIO_GetSate(pin);
	//ret = GPIO_GetValue(pin);
	//return ret;
}


static unsigned	char i2c_read_byte(void)
{
	unsigned char i;
	unsigned char ret;

	ENTRY(i2c_read_byte);

	ret	= 0;

	/* set SDA as intput status	here */
	//GPIO_InitIO(0, SDA);
	set_i2c_pin_dir(TLG_IIC_INPUT);//set gpio for input

	for	(i=0; i<8; i++)
	{
		i2c_delay(I2C_DELAY_UNIT <<	0);
		set_i2c_pin(SCL);
		i2c_delay(I2C_DELAY_UNIT <<	0);
		ret	= ret<<1;
		if (get_i2c_pin(SDA))
			ret	|= 1;
		i2c_delay(I2C_DELAY_UNIT <<	0);
		clr_i2c_pin(SCL);
			i2c_delay(I2C_DELAY_UNIT <<	0);

			if (i==7){
				 /* set SDA as output	status here	*/
			   //GPIO_InitIO(1, SDA);
				  set_i2c_pin_dir(TLG_IIC_OUTPUT);//set gpio for output
				}

			i2c_delay(I2C_DELAY_UNIT <<	0);
	}


	EXIT(i2c_read_byte);

	return ret;
}

static unsigned	char i2c_write_byte(unsigned char data)
{
	unsigned char i;


	ENTRY(i2c_write_byte);

	for	(i=0; i<8; i++)
	{
		i2c_delay(I2C_DELAY_UNIT <<	0);
		if (data & 0x80)
			set_i2c_pin(SDA);
		else
			clr_i2c_pin(SDA);
		data <<= 1;

		i2c_delay(I2C_DELAY_UNIT <<	0);
		set_i2c_pin(SCL);
		i2c_delay(I2C_DELAY_UNIT <<	0);
		clr_i2c_pin(SCL);
	}


	EXIT(i2c_write_byte);

	return i2c_read_ack();
}

/******************************************************************************/
// Description:  Init Camera Sensor (preview stop that change the sc6600l chip mode)
// Author:     Tim.zhu
// Input:      none
// Output:     none
// Return:     none
//             
// Note:       
/******************************************************************************/
uint32 ATV_IIC_Write(uint32 slave_addr, uint32 cmd, uint16 data)
{
    uint8 addr;
    uint8 reg_val[4];
    uint32 iic_length=0x04;
    uint32 count=0;
    int32 ret =0 ;
    uint32 i2c_handle_atv;
    I2C_DEV dev;

    reg_val[0]=(cmd>>0x08)&0x7f;
    reg_val[1]=cmd&0xff;
    reg_val[2]=(data>>0x08)&0xff;
    reg_val[3]=data&0xff;  

    i2c_handle_atv  = Sensor_GetI2cHnadler();
    I2C_HAL_Write(i2c_handle_atv, NULL, reg_val, iic_length);	

    return 0;
    
}

/******************************************************************************/
// Description:  Init Camera Sensor (preview stop that change the sc6600l chip mode)
// Author:     Tim.zhu
// Input:      none
// Output:     none
// Return:     none
//             
// Note:       
/******************************************************************************/
uint32 ATV_IIC_Read(uint32 slave_addr, uint32 cmd, uint16* data)
{
    uint8 addr;
    uint8 reg_val[4];
    uint8 red_val[4];    
    uint32 iic_length=0x04;
    int32 ret =0 ;
    uint32 i2c_handle_atv;
    I2C_DEV dev;

    reg_val[0]=(((cmd>>0x08)|0x80)&0xff);
    reg_val[1]=cmd&0xff;
    reg_val[2]=0x00;
    reg_val[3]=0x00; 

   i2c_handle_atv  = Sensor_GetI2cHnadler();
   I2C_HAL_Write(i2c_handle_atv, NULL, reg_val, iic_length);
   I2C_HAL_Read(i2c_handle_atv, NULL, red_val, iic_length);

   
   *data=(red_val[2]<<0x08)|red_val[3];
    
    return 0;
    
}



/***************************************************************************
*
*
*	 IIC Write data	for	TLG1100
*	 Parameter:
*			   dadd:device address (must be	0x58)
*			   radd:register address
*
****************************************************************************/
int TLGI2C_WriteReg(unsigned long dadd, unsigned long radd, unsigned short data)
{
	unsigned char tmpData =	0;

    ATV_IIC_Write(dadd, radd, data);

#if 0
	ENTRY(Tlg_i2c_write_data);



	i2c_begin();
	if (!i2c_write_byte(dadd<<1))
	{
		i2c_end();
		return IIC_ERR;
	}
	/* write 16bits	register */
	  tmpData =	((radd & 0x7f00) >>	8);

	if (!i2c_write_byte(tmpData))
	{
		i2c_end();
		return IIC_ERR;
	}

	tmpData	= (radd	& 0x00ff);
	if (!i2c_write_byte(tmpData))
	{
		i2c_end();
		return IIC_ERR;
	}

	 /*	write 16bits data */
	tmpData	= (((data)	& 0xff00) >> 8);
	if (!i2c_write_byte(tmpData))
	{
		i2c_end();
		return IIC_ERR;
	}

	tmpData	= ((data) & 0x00ff);
	if (!i2c_write_byte(tmpData))
	{
		i2c_end();
		return IIC_ERR;
	}


	i2c_end();


	EXIT(Tlg_i2c_write_data);
#endif

	return IIC_DONE;
}

/***************************************************************************
*
*
*	 IIC Read data for TLG1100
*	 Parameter:
*			   dadd:device add (must be	0x58)
*			   radd:register add
*
****************************************************************************/
int TLGI2C_ReadReg(unsigned long dadd, unsigned long radd, unsigned short *data)
{
	unsigned char tmpData =	0;

	ATV_IIC_Read(dadd, radd, data);

#if 0
	ENTRY(Tlg_i2c_read_data);


	i2c_begin();
	if (!i2c_write_byte(dadd<<1))
	{
		i2c_end();
		return IIC_ERR;
	}
	/* set MSB 1 */
	tmpData	= (((radd |	0x8000)	& 0xff00) >> 8);
	if (!i2c_write_byte(tmpData))
	{
		i2c_end();
		return IIC_ERR;
	}
	tmpData	= ((radd & 0x00ff));
	if (!i2c_write_byte(tmpData))
	{
		i2c_end();
		return IIC_ERR;
	}
///////////////////////////////////////////////////////////////////////////////////////////
//  ???????????????????????
//////////////////////////////////////////////////////////////////////////////////////////
	   /* write	any	data into this register	*/
	if (!i2c_write_byte(0xff))
	{
		i2c_end();
		return IIC_ERR;
	}
	if (!i2c_write_byte(0xff))
	{
		i2c_end();
		return IIC_ERR;
	}
	i2c_end();	/* stop	bit	*/
/////////////////////////////////////////////////////////////////////////////////////////

	/* start again */
	i2c_begin();

	if (!i2c_write_byte((unsigned char)((dadd<<1) |	1)))
	{
		i2c_end();
		return IIC_ERR;
	}

//////////////////////////////////////////////////////////
//			???????????????????
/////////////////////////////////////////////////////////
	*data =	i2c_read_byte();
	i2c_write_ack(0);

	*data =	i2c_read_byte();
	i2c_write_ack(0);
//////////////////////////////////////////////////////////

	  tmpData =	i2c_read_byte();
	  i2c_write_ack(0);

	  *data	= (tmpData << 8) & 0xff00;

	  tmpData =	i2c_read_byte();
	  i2c_write_ack(1);					 /*	can	not	send ACK,must send NAck	*/

	  *data	|= tmpData;

	i2c_end();


	EXIT(Tlg_i2c_read_data);
#endif

	return IIC_DONE;
}

#if 0
/************************************************************************
*
*			   PULL	HIGH SDA/SCK PIN
*
*************************************************************************/
static void	set_i2c_pin(unsigned int pin)
{
	 ENTRY(set_i2c_pin);

		/* pull	high sda/sck here */
	 GPIO_WriteIO(1, pin);		/* only	apply on MTK platform */

	 EXIT(set_i2c_pin);
}
#endif
/************************************************************************
*
*			   TLGI2C_Initcheck()
*
*************************************************************************/
int TLGI2C_Initcheck(unsigned i2c_addr, unsigned short on)
{
     ENTRY(TLGI2C_Initcheck);
	 return ((SDA<<3)+SCL);
    // EXIT(TLGI2C_Initcheck);
     
}

#if 0
/************************************************************************
*
*			   PULL	LOW	SDA/SCK	PIN
*
*************************************************************************/
static void	clr_i2c_pin(unsigned int pin)
{
	 ENTRY(clr_i2c_pin);

		/* pull	low	sda/sck	here */
	 GPIO_WriteIO(0, pin);		/* only	apply on MTK platform */

	 EXIT(clr_i2c_pin);
}
#endif
#if 0
/************************************************************************
*
*			   READ	DATA FROM SDA PIN
*
*************************************************************************/
static unsigned	char get_i2c_pin(unsigned int pin)
{
	unsigned char ret;


	ENTRY(get_i2c_pin);

	   /* add read I/O here	*/
	ret	= GPIO_ReadIO(pin);	 /*	only apply on MTK platform */

	EXIT(get_i2c_pin);

	return ret;
}
#endif
/**************************************************************************
*
*		   IIC START BIT
*
***************************************************************************/
static void	i2c_begin(void)
{

		ENTRY(i2c_begin);


	i2c_delay(I2C_DELAY_UNIT <<	0);
	set_i2c_pin(SDA);
	i2c_delay(I2C_DELAY_UNIT <<	0);
	set_i2c_pin(SCL);
	i2c_delay(I2C_DELAY_UNIT <<	0);
	clr_i2c_pin(SDA);
	i2c_delay(I2C_DELAY_UNIT <<	0);
	clr_i2c_pin(SCL);
	i2c_delay(I2C_DELAY_UNIT <<	0);


		EXIT(i2c_begin);
}

/**************************************************************************
*
*		   IIC STOP	BIT
*
***************************************************************************/
static void	i2c_end(void)
{

		ENTRY(i2c_end);

	i2c_delay(I2C_DELAY_UNIT <<	2);
	clr_i2c_pin(SDA);
	i2c_delay(I2C_DELAY_UNIT <<	2);
	set_i2c_pin(SCL);
	i2c_delay(I2C_DELAY_UNIT <<	3);
	set_i2c_pin(SDA);
	i2c_delay(I2C_DELAY_UNIT <<	4);


		EXIT(i2c_end);
}

/**************************************************************************
*
*		   IIC SEND	ACK	BIT
*
*					   flag	= 1: write NACK(SDA	HIGH)
*								0: write ACK(SDA LOW)
***************************************************************************/
static void	i2c_write_ack(unsigned char	flag)
{

	  ENTRY(i2c_write_ack);


	if(flag)
		set_i2c_pin(SDA);
	else
		clr_i2c_pin(SDA);
	i2c_delay(I2C_DELAY_UNIT <<	0);
	set_i2c_pin(SCL);
	i2c_delay(I2C_DELAY_UNIT <<	0);
	clr_i2c_pin(SCL);
	i2c_delay(I2C_DELAY_UNIT <<	0);
	//set_i2c_pin(SDA);				  // need to verify	here
	i2c_delay(I2C_DELAY_UNIT <<	0);


	  EXIT(i2c_write_ack);
}

/**************************************************************************
*
*		   IIC READ	ACK	BIT
*
***************************************************************************/
static unsigned	char i2c_read_ack(void)
{
	   unsigned	char ret;


	   ENTRY(i2c_read_ack);

	/* set SDA as input	status here	*/
	//GPIO_InitIO(0, SDA);				   /* only apply on	MTK	platform */
	set_i2c_pin_dir(TLG_IIC_INPUT);//set gpio for input

	i2c_delay(I2C_DELAY_UNIT <<	0);
	set_i2c_pin(SCL);
	i2c_delay(I2C_DELAY_UNIT <<	0);
	if (!get_i2c_pin(SDA))
	{
		ret	= IIC_TRUE;
	}
	else
	{
		ret	= IIC_FALSE;
	}

	i2c_delay(I2C_DELAY_UNIT <<	0);
	clr_i2c_pin(SCL);
	   i2c_delay(I2C_DELAY_UNIT	<< 0);

	   /* set SDA as output	status here	*/
	   //GPIO_InitIO(1, SDA);					   /* only apply on	MTK	platform */
	   set_i2c_pin_dir(TLG_IIC_OUTPUT);//set gpio for output

	i2c_delay(I2C_DELAY_UNIT <<	0);


	  EXIT(i2c_read_ack);

	return ret;
}

/********************************************************************************
*
*			  INSERT DELAY
*
*********************************************************************************/
static void	i2c_delay(unsigned int time)
{
	while(time--)
	{
		;
	}
}

/* end of file */
