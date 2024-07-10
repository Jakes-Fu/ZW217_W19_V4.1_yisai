/******************************************************************************
 ** File Name:      msensor_mir3da.c                                          *
 ** Author:         sunwen.he@unisoc.com                                      *
 ** Version         V1.0                                                      *
 ** DATE:           10/27/2022                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    driver for configuring mir3da include DA215S and DA217    *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME                    DESCRIPTION                        *
 ** 05/18/2018     ycwang@miramems.com     Create.                            *
 ** 10/27/2022     sunwen.he               Porting.                           *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define YZ_CROSS_TALK_ENABLE           1
#define TIMER_PERIOD_D217              500

#define D217_THRESHOLD_0_5G            500
// #define D217_THRESHOLD_0_6G            600
#define D217_THRESHOLD_0_7G            700
#define D217_THRESHOLD_0_8G            800
#define D217_THRESHOLD_0_8_5G          850
#define D217_THRESHOLD_0_9G            900
#define D217_THRESHOLD_1_0G            1000
#define D217_THRESHOLD_1_1G            1100
#define D217_THRESHOLD_1_2G            1200
#define D217_THRESHOLD_1_3G            1300
#define D217_THRESHOLD_1_5G            1500
#define D217_LOW_THRESHOLD             200

#define D217_SHAKE_DETECT_TIME         200
#define D217_DESHAKE_TIME              300

/*******************************************************************************
 **             Register define for Gsensor asic                               *
 *******************************************************************************/
#define NSA_REG_SPI_I2C                 0x00
#define NSA_REG_WHO_AM_I                0x01
#define NSA_REG_ACC_X_LSB               0x02
#define NSA_REG_ACC_X_MSB               0x03
#define NSA_REG_ACC_Y_LSB               0x04
#define NSA_REG_ACC_Y_MSB               0x05
#define NSA_REG_ACC_Z_LSB               0x06
#define NSA_REG_ACC_Z_MSB               0x07
#define NSA_REG_MOTION_FLAG				0x09
#define NSA_REG_STEPS_MSB				0x0D
#define NSA_REG_STEPS_LSB				0x0E
#define NSA_REG_G_RANGE                 0x0F
#define NSA_REG_ODR_AXIS_DISABLE        0x10
#define NSA_REG_POWERMODE_BW            0x11
#define NSA_REG_SWAP_POLARITY           0x12
#define NSA_REG_FIFO_CTRL               0x14
#define NAS_REG_INT_SET0				0x15
#define NSA_REG_INTERRUPT_SETTINGS1     0x16
#define NSA_REG_INTERRUPT_SETTINGS2     0x17
#define NSA_REG_INTERRUPT_MAPPING1      0x19
#define NSA_REG_INTERRUPT_MAPPING2      0x1a
#define NSA_REG_INTERRUPT_MAPPING3      0x1b
#define NSA_REG_INT_PIN_CONFIG          0x20
#define NSA_REG_INT_LATCH               0x21
#define NSA_REG_ACTIVE_DURATION         0x27
#define NSA_REG_ACTIVE_THRESHOLD        0x28
#define NSA_REG_TAP_DURATION            0x2A
#define NSA_REG_TAP_THRESHOLD           0x2B
#define NSA_REG_STEP_CONFIG1			0x2F
#define NSA_REG_STEP_CONFIG2			0x30
#define NSA_REG_STEP_CONFIG3			0x31
#define NSA_REG_STEP_CONFIG4			0x32
#define NSA_REG_STEP_FILTER				0x33
#define NSA_REG_SM_THRESHOLD			0x34
#define NSA_REG_CUSTOM_OFFSET_X         0x38
#define NSA_REG_CUSTOM_OFFSET_Y         0x39
#define NSA_REG_CUSTOM_OFFSET_Z         0x3a
#define NSA_REG_ENGINEERING_MODE        0x7f
#define NSA_REG_SENSITIVITY_TRIM_X      0x80
#define NSA_REG_SENSITIVITY_TRIM_Y      0x81
#define NSA_REG_SENSITIVITY_TRIM_Z      0x82
#define NSA_REG_COARSE_OFFSET_TRIM_X    0x83
#define NSA_REG_COARSE_OFFSET_TRIM_Y    0x84
#define NSA_REG_COARSE_OFFSET_TRIM_Z    0x85
#define NSA_REG_FINE_OFFSET_TRIM_X      0x86
#define NSA_REG_FINE_OFFSET_TRIM_Y      0x87
#define NSA_REG_FINE_OFFSET_TRIM_Z      0x88
#define NSA_REG_SENS_COMP               0x8c
#define NSA_REG_MEMS_OPTION             0x8f
#define NSA_REG_CHIP_INFO               0xc0
#define NSA_REG_CHIP_INFO_SECOND        0xc1
#define NSA_REG_MEMS_OPTION_SECOND      0xc7
#define NSA_REG_SENS_COARSE_TRIM        0xd1
#define NAS_REG_OSC_TRIM				0x8e

/*******************************************************************************
 **                        Typedef definitions                                 *
 *******************************************************************************/
#define ARM_BIT_8               0

#if ARM_BIT_8
//如下数据类型是在8位机上定义的，在其它平台（比如32位）可能存在差别，需要根据实际情况修改 。
typedef unsigned char    u8_m;                   /* 无符号8位整型变量*/
typedef signed   char    s8_m;                   /* 有符号8位整型变量*/
typedef unsigned int     u16_m;                  /* 无符号16位整型变量*/
typedef signed   int     s16_m;                  /* 有符号16位整型变量*/
typedef unsigned long    u32_m;                  /* 无符号32位整型变量*/
typedef signed   long    s32_m;                  /* 有符号32位整型变量*/
typedef float            fp32_m;                 /* 单精度浮点数（32位长度）*/
typedef double           fp64_m;                 /* 双精度浮点数（64位长度）*/
#else
//如下数据类型是在32位机上定义的，在其它平台（比如8位）可能存在差别，需要根据实际情况修改 。
typedef unsigned char    u8_m;                   /* 无符号8位整型变量*/
typedef signed   char    s8_m;                   /* 有符号8位整型变量*/
typedef unsigned short   u16_m;                  /* 无符号16位整型变量*/
typedef signed   short   s16_m;                  /* 有符号16位整型变量*/
typedef unsigned int     u32_m;                  /* 无符号32位整型变量*/
typedef signed   int     s32_m;                  /* 有符号32位整型变量*/
typedef float            fp32_m;                 /* 单精度浮点数（32位长度）*/
typedef double           fp64_m;                 /* 双精度浮点数（64位长度）*/
#endif

LOCAL int32                 s_d217_sh_th = 0; // shake threshhold for sensitivity
LOCAL int32                 s_d217_or_th = 0; // orientation threshhold for sensitivity
#if YZ_CROSS_TALK_ENABLE
LOCAL short                 yzcross=0;
#endif
/**---------------------------------------------------------------------------*
 **                         DataStruct Definition                             *
 **---------------------------------------------------------------------------*/
typedef enum
{
	CHIP_ID_TYPE_DA217 = 0,
	CHIP_ID_TYPE_DA215S,
	CHIP_ID_TYPE_UNKNOWN
}da21x_type;

static da21x_type da21x_chip_type = CHIP_ID_TYPE_UNKNOWN;

u8_m i2c_addr = 0x27;
static unsigned int f_step = 0;
LOCAL int32 s_d217_IIC_Handler           = -1;
LOCAL MSENSOR_MISC_CFG_T s_d217_misc_cfg = {0};
LOCAL SCI_TIMER_PTR s_d217_timer	= PNULL;

LOCAL uint32 mir3da_init(void);
LOCAL uint32 mir3da_open(void);
LOCAL uint32 mir3da_close(void);
LOCAL uint32 mir3da_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);

PUBLIC MSENSOR_CFG_T g_d217_msensor_info =
{
    "DA215S",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	mir3da_init,
	mir3da_open,
	mir3da_close,
	mir3da_ioctl
};

s8_m mir3da_register_read(u8_m addr, u8_m *data_m, u8_m len)
{
	if(s_d217_IIC_Handler==-1)
	{
		SCI_TraceLow("[DA217]Handler=-1 read error!!!");
		return MSENSOR_ERROR_IIC_READ_FAILED;
	}
  	//To do i2c read api
	if(I2C_HAL_Read(s_d217_IIC_Handler, &addr, data_m, len) == 0)
	{
		SCI_TraceLow("[DA217]read error!!!");
		return MSENSOR_ERROR_IIC_READ_FAILED;
	}
	return 0;
}

s8_m mir3da_register_write(u8_m addr, u8_m data_m)
{
	if(s_d217_IIC_Handler==-1)
	{
		SCI_TraceLow("[DA217]Handler=-1 write error!!!");
		return MSENSOR_ERROR_IIC_READ_FAILED;
	}
	//To do i2c write api
	if(I2C_HAL_Write(s_d217_IIC_Handler, &addr, &data_m, 1) == 0)
	{
		SCI_TraceLow("[DA217]write error!!!");
		return MSENSOR_ERROR_IIC_WRITE_FAILED;
	}
	return 0;

}

s8_m mir3da_register_mask_write(unsigned char addr, unsigned char mask, unsigned char data){
    int     res = 0;
    unsigned char      tmp_data = 0;

    res = mir3da_register_read(addr, &tmp_data, 1);
    if(res) {
        return res;
    }

    tmp_data &= ~mask;
    tmp_data |= data & mask;
    res = mir3da_register_write(addr, tmp_data);

    return res;
}


//Initialization
s8_m mir3da_initialize(void)
{
    s8_m res = 0;
    u8_m data_m = 0;

    mir3da_register_mask_write(0x00, 0x24, 0x24);
	OS_TickDelay(20); //delay 20ms

	SCI_TraceLow("[DA217]mir3da chip id=%x\r\n",data_m);

	res |= mir3da_register_write(NSA_REG_G_RANGE, 0x01);               //+/-4G,14bit
	res |= mir3da_register_write(NSA_REG_POWERMODE_BW, 0x14);          //normal mode
	res |= mir3da_register_write(NSA_REG_ODR_AXIS_DISABLE, 0x07);      //ODR = 125hz

	//Engineering mode
	res |= mir3da_register_write(NSA_REG_ENGINEERING_MODE, 0x83);
	res |= mir3da_register_write(NSA_REG_ENGINEERING_MODE, 0x69);
	res |= mir3da_register_write(NSA_REG_ENGINEERING_MODE, 0xBD);

	//Reduce power consumption
	if(i2c_addr == 0x26)
	{
		mir3da_register_mask_write(NSA_REG_SENS_COMP, 0x40, 0x00);
	}

#if YZ_CROSS_TALK_ENABLE
    res |= mir3da_register_read(NSA_REG_CHIP_INFO_SECOND, &data_m,1);

    if(data_m&0x10)
        yzcross = -(data_m&0x0f);
    else
		yzcross = (data_m&0x0f);
#endif
#if 0
	mir3da_register_mask_write(0x8f, 0x02, 0x00);
#endif
	return res;
}



int mir3da_open_step_counter(void)
{
	int res = 0;

	res |=  mir3da_register_write(NSA_REG_STEP_CONFIG1, 0x31);
	res |=  mir3da_register_write(NSA_REG_STEP_CONFIG2, 0xc2);
	res |=  mir3da_register_write(NSA_REG_STEP_CONFIG3, 0x46);
	res |=  mir3da_register_write(NSA_REG_STEP_CONFIG4, 0x32);
	res |=  mir3da_register_write(NSA_REG_STEP_FILTER, 0xA2);

	return res;
}

int mir3da_close_step_counter(void)
{
	mir3da_register_mask_write(NSA_REG_STEP_FILTER, 0x80, 0x00);
	return 0;
}


int mir3da_get_step(void)
{
	unsigned char    tmp_data[2] = {0};

	if (mir3da_register_read(NSA_REG_STEPS_MSB,  tmp_data,2) == 0)
		f_step = (tmp_data[0] << 8 | tmp_data[1])/2;
	SCI_TraceLow("[DA217]mir3da_get_step %d  data %x data1 %x\n ",f_step,tmp_data[0],tmp_data[1]);

	return (f_step);
}


//enable/disable the chip
s8_m mir3da_set_enable(u8_m enable)
{
	s8_m res = 0;
	if(enable)
		res = mir3da_register_write(NSA_REG_POWERMODE_BW,0x14);
	else
		res = mir3da_register_write(NSA_REG_POWERMODE_BW,0x80);

	return res;
}

//Read three axis data, 1024 LSB = 1 g
s8_m mir3da_read_data(s16_m *x, s16_m *y, s16_m *z)
{
    u8_m    tmp_data[6] = {0};

#if 1
    if (mir3da_register_read(NSA_REG_ACC_X_LSB, tmp_data,6) != 0) {
        return -1;
    }
#else
	mir3da_register_read(NSA_REG_ACC_X_LSB, &tmp_data[0], 1);
	mir3da_register_read(NSA_REG_ACC_X_MSB, &tmp_data[1], 1);
	mir3da_register_read(NSA_REG_ACC_Y_LSB, &tmp_data[2], 1);
	mir3da_register_read(NSA_REG_ACC_Y_MSB, &tmp_data[3], 1);
	mir3da_register_read(NSA_REG_ACC_Z_LSB, &tmp_data[4], 1);
	mir3da_register_read(NSA_REG_ACC_Z_MSB, &tmp_data[5], 1);
 #endif

    *x = ((s16_m)(tmp_data[1] << 8 | tmp_data[0]))>> 3;
    *y = ((s16_m)(tmp_data[3] << 8 | tmp_data[2]))>> 3;
    *z = ((s16_m)(tmp_data[5] << 8 | tmp_data[4]))>> 3;

#if YZ_CROSS_TALK_ENABLE
    if(yzcross)
        *y=*y-(*z)*yzcross/100;
#endif

    return 0;
}

//open active interrupt
s8_m mir3da_open_interrupt(u8_m th)
{
s8_m res = 0;

	res = mir3da_register_write(NSA_REG_ACTIVE_DURATION,0x02);
	res = mir3da_register_write(NSA_REG_ACTIVE_THRESHOLD,th);
	res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x04);
	res = mir3da_register_write(NSA_REG_INT_LATCH, 0xEE);  //latch 100ms
	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x87);

	return res;
}


int mir3da_open_full_fifo_int(){
	int   res = 0;

	res = mir3da_register_write(0x14,0x80);
	res = mir3da_register_write(0x15,0x04);
	res = mir3da_register_write(0x1A,0x04);

	return res;
}


//num 1-31
int mir3da_open_fifo_watermark_int(u8_m num){
	int   res = 0;

	res = mir3da_register_write(0x14,0x80|num);
	res = mir3da_register_write(0x15,0x08);
	res = mir3da_register_write(0x1A,0x02);

	return res;
}

int mir3da_close_fifo_int(void){
	int   res = 0;

	res = mir3da_register_write(0x14,0x00);
	res = mir3da_register_write(0x15,0x00);
	res = mir3da_register_write(0x1A,0x00);

	return res;
}

int mir3da_read_full_fifo(short *x, short *y, short *z)
{
   int i = 0, tmp_data = 0;

   mir3da_register_read(0x08, &tmp_data,1);

   if((tmp_data & 0x40))
   {
	   for(i = 0; i < 32; i++)
	   {
			mir3da_read_data(x+i, y+i, z+i);
	   }
   }

    return 0;
}

int mir3da_read_fifo(short *x, short *y, short *z)
{
   s8_m i = 0, num = 0, tmp_data = 0;

   mir3da_register_read(0x14, &num,1);
   num &= 0x3f;
   mir3da_register_read(0x08, &tmp_data,1);
   tmp_data &= 0x3f;

   while(tmp_data >= num)
   {
	   for(i = 0; i < tmp_data; i++)
	   {
			mir3da_read_data(x+i, y+i, z+i);
	   }

   	   mir3da_register_read(0x08, &tmp_data,1);
	   tmp_data &= 0x3f;
   }

    return 0;
}


//close active interrupt
s8_m mir3da_close_interrupt(void)
{
s8_m res = 0;

	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x00 );
	res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x00 );

	return res;
}

LOCAL uint32 mir3da_GetVector(MSENSOR_MOTION_DATA_T * data_ptr)
{
    int step_num = 0;
    s16_m x = 0, y = 0, z = 0;

    mir3da_read_data(&x,&y,&z);
    step_num=mir3da_get_step();
    //SCI_TraceLow("[DA217]mir3da_GetVector:x=%d,y=%d,z=%d,step_num:%d",x,y,z,step_num);
    //Modified by sunwen.he for W217 fixed direction
    data_ptr->u.vector.x = -y;
    data_ptr->u.vector.y = x;
    data_ptr->u.vector.z = -z;
    SCI_TraceLow("[DA217]GetVector:x=%d,y=%d,z=%d,step_num:%d",data_ptr->u.vector.x,
        data_ptr->u.vector.y,data_ptr->u.vector.z,step_num);
    data_ptr->type     = MSENSOR_TYPE_ACCELEROMETER;
    data_ptr->reserved = step_num;
    return MSENSOR_SUCCESS;
}

LOCAL uint32 mir3da_GetCapability(uint32 * Capability)
{
    *Capability = MSENSOR_MOTION_CAP_XY_AXIS | MSENSOR_MOTION_CAP_Z_AXIS | MSENSOR_MOTION_CAP_SHAKE | MSENSOR_MOTION_CAP_DATA;
    return MSENSOR_SUCCESS;
}

LOCAL void mir3da_TimerCallback(uint32 param)
{
	//SCI_TraceLow("[DA217] TimerCallback");
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_d217_timer, mir3da_TimerCallback, TIMER_PERIOD_D217);
	SCI_ActiveTimer(s_d217_timer);
	return ;
}

LOCAL uint32 mir3da_ChangeTimer(uint32 timer_expire)
{
	SCI_ChangeTimer(s_d217_timer, mir3da_TimerCallback, timer_expire);
	SCI_ActiveTimer(s_d217_timer);
    return MSENSOR_SUCCESS;
}

LOCAL uint32 mir3da_SetSensi(MSENSOR_SENSI_E sensi_val)
{
	//SCI_TraceLow("[DA217] SetSensi sensi_val:%d",sensi_val);

    switch(sensi_val)
    {
        case MSENSOR_SENSI_LOW:
        {
            s_d217_sh_th = D217_THRESHOLD_1_5G;
            s_d217_or_th = D217_THRESHOLD_1_0G;
            break;
        }
        case MSENSOR_SENSI_MID:
        {
            s_d217_sh_th = D217_THRESHOLD_1_2G;
            s_d217_or_th = D217_THRESHOLD_0_9G;
            break;
        }
        case MSENSOR_SENSI_HIGH:
        {
            s_d217_sh_th = D217_THRESHOLD_1_0G;
            s_d217_or_th = D217_THRESHOLD_0_7G;
            break;
        }
        default:
        {
            s_d217_sh_th = D217_THRESHOLD_1_1G;
            s_d217_or_th = D217_THRESHOLD_0_8G;
            break;
        }
    }

    return MSENSOR_SUCCESS;
}

LOCAL uint32 mir3da_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
{
	static MSENSOR_MOTION_TYPE_E s_last_motion_type = MSENSOR_MOTION_TYPE_NONE;
    MSENSOR_MOTION_TYPE_E motion_type = MSENSOR_MOTION_TYPE_NONE;

	uint8 int_st[3]={0},reg_0x1a=0,int_type= 0xff;
	uint32        count_tick = 0;
	static uint32 count_tick_old = 0;
	static uint32 count_tick_deshake = 0;
	MSENSOR_MOTION_DATA_T data_val ={0};
	int32  x = 0, y = 0, z = 0;
    static int32 x_old = 0;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);



	SCI_TraceLow("[DA217]GetEvent s_last_motion_type= 0x%X,motion_type:0x%x",s_last_motion_type, motion_type);
	//The following message should not be send to client, otherwise there will
	//be too much message which could confuse the user!

	if(cfg_ptr || s_last_motion_type != motion_type)
    {   // always report at INT mode or New Event
     	*event_ptr  = motion_type;
    }
	else
	{	// do NOT report the same event,only for timer mode
        *event_ptr = MSENSOR_EVENT_NONE;
	}
	s_last_motion_type = motion_type;

    //only one shaking event
    //avoid double shake
	if(motion_type == MSENSOR_MOTION_TYPE_LEFT || motion_type == MSENSOR_MOTION_TYPE_RIGHT)
	{
	//    count_tick = SCI_GetTickCount();

    	if(count_tick - count_tick_deshake < D217_DESHAKE_TIME)
    	{
    	    *event_ptr = MSENSOR_EVENT_NONE;
    	}
    	count_tick_deshake = count_tick;
    }
	return MSENSOR_SUCCESS;
}

LOCAL uint32 mir3da_init(void)
{
    s8_m res = 0;
    u8_m data_m = 0;
    int32 ret = 0;

	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo(GPIO_PROD_GSENSOR_INT_ID);

	//SCI_TraceLow("[DA217]mir3da_init !!! \n");
    if(cfg_ptr)
		GPIO_DisableIntCtl(cfg_ptr->gpio_num);

    MSensor_GetMiscCfg(&g_d217_msensor_info, &s_d217_misc_cfg);
	SCI_TraceLow("[DA217]mir3da_init IIC addr=%x\n",s_d217_misc_cfg.i2c_cfg.slave_addr);

	if((s_d217_IIC_Handler == -1) &&
		((s_d217_IIC_Handler = I2C_HAL_Open(&(s_d217_misc_cfg.i2c_cfg))) == -1))
	{
		SCI_TraceLow("[DA217]mir3da_init open IIC failed!\n");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}

	//Retry 3 times
	res = mir3da_register_read(NSA_REG_WHO_AM_I,&data_m,1);
    if(data_m != 0x13){
        res = mir3da_register_read(NSA_REG_WHO_AM_I,&data_m,1);
        if(data_m != 0x13){
            res = mir3da_register_read(NSA_REG_WHO_AM_I,&data_m,1);
            if(data_m != 0x13){
                SCI_TraceLow("[DA217]mir3da_init read chip id  error= %x\r\n",data_m);
                return MSENSOR_ERROR;
            }
        }
    }
	SCI_TraceLow("[DA217]mir3da_init chip id = %x\r\n",data_m);
	if(data_m == 0x13)
	{
		mir3da_initialize();
		mir3da_open_step_counter();
		f_step = 0;
	}
	else
	{
		return MSENSOR_ERROR;
	}

	res = mir3da_register_read(NSA_REG_CHIP_INFO,&data_m,1);
	SCI_TraceLow("[DA217] mir3da_init CHIP_INFO = %x\r\n",data_m);
	if(data_m==0x57)
	{
		da21x_chip_type=CHIP_ID_TYPE_DA215S;
	}
	else if(data_m==0x50)
	{
		da21x_chip_type=CHIP_ID_TYPE_DA217;
	}
	else
	{
		da21x_chip_type=CHIP_ID_TYPE_UNKNOWN;
	}

    if(cfg_ptr)
		GPIO_EnableIntCtl(cfg_ptr->gpio_num);

	I2C_HAL_Close(s_d217_IIC_Handler);
	s_d217_IIC_Handler = -1;

	SCI_TraceLow("[DA217]mir3da_init SUCCEESSFULLY!!! \n");
	return MSENSOR_SUCCESS;
}

LOCAL uint32 mir3da_open(void)
{
    uint8 val=0;

    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo(GPIO_PROD_GSENSOR_INT_ID);

	switch(da21x_chip_type)
    {
		case CHIP_ID_TYPE_DA215S:
			SCI_TraceLow("[DA215S] mir3da_open !!! \n");
			break;
		case CHIP_ID_TYPE_DA217:
			SCI_TraceLow("[DA217]  mir3da_open !!! \n");
			break;
		case CHIP_ID_TYPE_UNKNOWN:
			SCI_TraceLow("[DA217] [UNKNOWN] mir3da_open !!! \n");
			break;
		default:
			SCI_TraceLow("[DA217] [UNKNOWN] mir3da_open !!! \n");
			break;
	}

	if((s_d217_IIC_Handler == -1) &&
		((s_d217_IIC_Handler = I2C_HAL_Open(&(s_d217_misc_cfg.i2c_cfg))) == -1))
	{
		SCI_TraceLow("[DA217]open IIC failed!\n");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}

	if(!cfg_ptr)
    {
    	if(s_d217_timer == NULL)
    	{
			//SCI_TraceLow("[DA217] open SCI_CreateTimer start!");
            s_d217_timer = SCI_CreateTimer("d217_Timer", mir3da_TimerCallback,
    			MSENSOR_TYPE_ACCELEROMETER, TIMER_PERIOD_D217, SCI_AUTO_ACTIVATE);
			//SCI_TraceLow("[DA217] open SCI_CreateTimer end s_d217_timer:%p",s_d217_timer);
    	}
    	else
    	{
    		SCI_ChangeTimer(s_d217_timer, mir3da_TimerCallback, TIMER_PERIOD_D217);
    		SCI_ActiveTimer(s_d217_timer);
    	}
    }

	SCI_TraceLow("[DA217] open SUCCEESSFULLY!");
	return MSENSOR_SUCCESS;
}

LOCAL uint32 mir3da_close(void)
{
	//SCI_TraceLow("[DA217] mir3da_close.\n");
	if(s_d217_timer != NULL)
	{
		SCI_DeactiveTimer(s_d217_timer);
	}
	if(s_d217_IIC_Handler != -1)
	{
		I2C_HAL_Close(s_d217_IIC_Handler);
		s_d217_IIC_Handler = -1;
	}
	SCI_TraceLow("[DA217] close");
	return MSENSOR_SUCCESS;
}

LOCAL uint32 mir3da_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
	uint32 ret = MSENSOR_SUCCESS;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
	//SCI_TraceLow("[DA217] ioctl cmd:%d",cmd);

    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
			ret = mir3da_GetVector((MSENSOR_MOTION_DATA_T *)data_ptr);
			break;
        }

        case MSENSOR_IOCTL_GET_EVENT:
        {
			if(cfg_ptr)
            {
                GPIO_DisableIntCtl(cfg_ptr->gpio_num);
            }
			ret = mir3da_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);
            if(cfg_ptr)
            {
                //resume InterruptSense from changed in GPIO_GSensorIntHandler()
                if(cfg_ptr->valid_level == GPIO_PROD_LOW_LEVEL)
                {
    			    GPIO_SetInterruptSense(cfg_ptr->gpio_num, GPIO_INT_EDGES_FALLING);
    			}
    			if(cfg_ptr->valid_level == GPIO_PROD_HIGH_LEVEL)
    			{
    			    GPIO_SetInterruptSense(cfg_ptr->gpio_num, GPIO_INT_EDGES_RISING);
    			}

    			GPIO_EnableIntCtl(cfg_ptr->gpio_num);
			}
			break;
        }
        case MSENSOR_IOCTL_SET_SENSI:
        {
			ret = mir3da_SetSensi(*(MSENSOR_SENSI_E *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_SET_TIMER:
        {
			ret = mir3da_ChangeTimer(*(uint32 *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_GET_CAP:
        {
			ret = mir3da_GetCapability((uint32 *)data_ptr);
			break;
        }
		case MSENSOR_IOCTL_SET_CLEAN:
        {
			uint8 data = 0;
			ret = mir3da_register_read(0x2E,&data,1);
			data = data | 0x80;
			ret = mir3da_register_write(0x2E,data);
			break;
        }
        default:
            ret = MSENSOR_ERROR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif  // End of file
