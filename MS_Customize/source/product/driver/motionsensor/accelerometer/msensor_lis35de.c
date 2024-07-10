/******************************************************************************
 ** File Name:      msensor_lis35DE.c                                         *
 ** Author:         Haifeng.Yang                                              *
 ** DATE:           05/14/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/14/2010     Haifeng.Yang     Create.                                   *
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
#define LIS35DE_DEBUG
#define SENSOR_INT_MODE
#ifdef LIS35DE_DEBUG
#define LIS35DE_TRACE			SCI_TRACE_LOW
#else
#define LIS35DE_TRACE(x)
#endif

#ifdef WIN32
#define __func__ 
#endif

//control registers
#define LIS35DE_CTRL_REG1_RW            0X20
#define LIS35DE_CTRL_REG2_RW            0X21
#define LIS35DE_CTRL_REG3_RW            0X22

#define LIS35DE_HP_FILTER_RESET_R       0X23
#define LIS35DE_STATUS_REG_R            0x27
#define LIS35DE_OUTX_R                  0X29
#define LIS35DE_OUTY_R                  0X2B
#define LIS35DE_OUTZ_R                  0X2D

#define LIS35DE_FF_WU_CFG_1_RW          0X30
#define LIS35DE_FF_WU_SRC_1_R           0X31
#define LIS35DE_FF_WU_THS_1_RW          0X32
#define LIS35DE_FF_WU_DURATION_1_RW     0X33
#define LIS35DE_FF_WU_CFG_2_RW          0X34
#define LIS35DE_FF_WU_SRC_2_R           0X35
#define LIS35DE_FF_WU_THS_2_RW          0X36
#define LIS35DE_FF_WU_DURATION_2_RW     0X37

#define LIS35DE_CLICK_CFG_RW            0X38
#define LIS35DE_CLICK_SRC_R             0X39
#define LIS35DE_CLICK_THSY_X_RW         0X3B
#define LIS35DE_CLICK_THSZ_RW           0X3C
#define LIS35DE_CLICK_TIMELIMIT_RW      0X3D
#define LIS35DE_CLICK_LATENCY_RW        0X3E
#define LIS35DE_CLICK_WINDOW_RW         0X3F

//X Y Z AXISES THRESHOLD
//#define LIS35DE_THRESHOLD_ZERO            5
// according to datasheet, 15 ~~ 21 mg/digit, MOCOR's unit: mg
// 20110212: will.jiang_cr227245 : test parameter under 1000 mg
#define LIS35DE_DIGIT_TO_MG             19

//#define LIS35DE_THRESHOLD_0_3G            300
#define LIS35DE_THRESHOLD_0_5G            500
// #define LIS35DE_THRESHOLD_0_6G            600
#define LIS35DE_THRESHOLD_0_7G            700
#define LIS35DE_THRESHOLD_0_8G            800
#define LIS35DE_THRESHOLD_0_8_5G            850
#define LIS35DE_THRESHOLD_0_9G            900
#define LIS35DE_THRESHOLD_1_0G            1000
#define LIS35DE_THRESHOLD_1_1G            1100
#define LIS35DE_THRESHOLD_1_2G            1200
#define LIS35DE_THRESHOLD_1_3G            1300
#define LIS35DE_THRESHOLD_1_5G            1500
#define LIS35DE_LOW_THRESHOLD             200

#define LIS35DE_SHAKE_DETECT_TIME         200
#define LIS35DE_DESHAKE_TIME              300

/**---------------------------------------------------------------------------*
 **                         DataStruct Definition                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Variables and function                            *
 **---------------------------------------------------------------------------*/ 
LOCAL uint32 lis35DE_init(void);
LOCAL uint32 lis35DE_open(void);
LOCAL uint32 lis35DE_close(void);
LOCAL void lis35DE_TimerCallback(uint32 param);
LOCAL uint32 lis35DE_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);
LOCAL uint32 lis35DE_SetSensi(MSENSOR_SENSI_E sensi_val);

PUBLIC MSENSOR_CFG_T g_lis35DE_msensor_info = 
{
    "st lis35de",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	lis35DE_init,
	lis35DE_open,
	lis35DE_close,
	lis35DE_ioctl
};
LOCAL MSENSOR_MISC_CFG_T s_lis35DE_misc_cfg = {0};
LOCAL int32 s_lis35DE_IIC_Handler           = -1;

#define TIMER_PERIOD_LIS35DE                  500
LOCAL SCI_TIMER_PTR 		s_lis35de_timer	= PNULL;
// LOCAL SCI_SEMAPHORE_PTR    	s_lis35de_sm    = PNULL;
LOCAL int32                 s_lis35de_sh_th = 0; // shake threshhold for sensitivity
LOCAL int32                 s_lis35de_or_th = 0; // orientation threshhold for sensitivity
/*****************************************************************************/
//  Description:    read chip register content
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint32 lis35DE_read(uint8* cmd)
{
	if(I2C_HAL_Read(s_lis35DE_IIC_Handler, cmd, cmd + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_READ_FAILED;
	//READ OK!
	return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    write configuration data to chip registers
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint32 lis35DE_write(uint8* cmd)
{
	if(I2C_HAL_Write(s_lis35DE_IIC_Handler, cmd, cmd + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_WRITE_FAILED;
	//WRITE OK!
	return MSENSOR_SUCCESS;	
}

/*****************************************************************************/
//  Description:    Init g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 lis35DE_init(void)
{
	uint8   ctrl_data[2];
	
    MSensor_GetMiscCfg(&g_lis35DE_msensor_info, &s_lis35DE_misc_cfg);
    lis35DE_SetSensi(MSENSOR_SENSI_MID);
	if((s_lis35DE_IIC_Handler == -1) && 
		((s_lis35DE_IIC_Handler = I2C_HAL_Open(&(s_lis35DE_misc_cfg.i2c_cfg))) == -1))
	{
		//LIS35DE_TRACE:"MSENSOR:: ==> open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_160_112_2_18_0_34_19_1893,(uint8*)"");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}

	//POWER DOWN SENSOR
	ctrl_data[0] = LIS35DE_CTRL_REG1_RW;
	ctrl_data[1] = 0;
	lis35DE_write(ctrl_data);

	I2C_HAL_Close(s_lis35DE_IIC_Handler);
	s_lis35DE_IIC_Handler = -1;
	
	//OK, it seems like a LIS35DE CHIP!
	//LIS35DE_TRACE:"MSENSOR:: ==> SUCCEESSFULLY!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_173_112_2_18_0_34_19_1894,(uint8*)"");
	return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    open g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 lis35DE_open(void)
{
#define CFG_ITEM_LEN              2
	static const uint8 init_cfg[][CFG_ITEM_LEN] = 
	{
		//D7 - D0:DR PD FS 0(1) 0(1) Zen Yen Xen
		//SET PD_BIT(BIT6) TO 1,Active the CHIP!
		{LIS35DE_CTRL_REG1_RW, 0x47},

		//D7 - D0:SIM BOOT 0(1) FDS HPF2 HPF1 HP_coeff2 HP_coeff1
		//enabale high pass filter
		{LIS35DE_CTRL_REG2_RW, 0x07},

		//D7 - D0:IHL PP_OD I2CFG2 I2CFG1 I2CFG0 I1CFG2 I1CFG1 I1CFG0
		//INT active LOW, PUSH-PULL, FF_WU_1->INT1, GND->INT2
		{LIS35DE_CTRL_REG3_RW, 0x81},

		//D7 - D0:AOI LIR ZHIE ZLIE YHIE YLIE XHIE XLIE
		{LIS35DE_FF_WU_CFG_1_RW, 0X6A},

		//D7 - D0:DCRM THS6 THS5 THS4 THS3 THS2 THS1 THS0
		{LIS35DE_FF_WU_THS_1_RW,10},

		//D7 - D0:Duration value. Default value: 0000 0000
		//        40ms in this case.
		{LIS35DE_FF_WU_DURATION_1_RW, 4},
	};
	uint32 i;
	uint8* p_init = (uint8*)init_cfg;
	uint8 reg = 0;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
    
	if((s_lis35DE_IIC_Handler == -1) && 
		((s_lis35DE_IIC_Handler = I2C_HAL_Open(&(s_lis35DE_misc_cfg.i2c_cfg))) == -1))
	{
		//LIS35DE_TRACE:"MSENSOR::==> open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_215_112_2_18_0_34_19_1895,(uint8*)"");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}
	for(i = 0; i < (sizeof(init_cfg) / CFG_ITEM_LEN); i++)
	{
		lis35DE_write(p_init);
		p_init += CFG_ITEM_LEN;			//next cfg.
	}
	//guobin.zhu cr105782
	reg = LIS35DE_FF_WU_SRC_1_R;
	lis35DE_read(&reg);
	
    if(!cfg_ptr)
    {
    	if(s_lis35de_timer == NULL)
    	{
            s_lis35de_timer = SCI_CreateTimer("lis35de_Timer", lis35DE_TimerCallback,
    			MSENSOR_TYPE_ACCELEROMETER, TIMER_PERIOD_LIS35DE, SCI_AUTO_ACTIVATE);
    	}
    	else
    	{
    		SCI_ChangeTimer(s_lis35de_timer, lis35DE_TimerCallback, TIMER_PERIOD_LIS35DE);
    		SCI_ActiveTimer(s_lis35de_timer);	
    	}
    }
   	// msensor_drv.c has a semaphore already
   	#if 0
	if(s_lis35de_sm == NULL)
	{
		s_lis35de_sm = SCI_CreateSemaphore ("MSENSOR SEMAPHORE", 1);
		SCI_ASSERT(s_lis35de_sm != NULL);
	}
	#endif
	//LIS35DE_TRACE:"MSENSOR:: ==> SUCCEESSFULLY!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_248_112_2_18_0_34_19_1896,(uint8*)"");
	return MSENSOR_SUCCESS;
}	
/*****************************************************************************/
//  Description:    close g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 lis35DE_close(void)
{
	uint8   reg_data[2];

	//Timer and semaphore need NOT be deleted. Just to deactive them.
	if(s_lis35de_timer != NULL)
	{
		SCI_DeactiveTimer(s_lis35de_timer);
	}

	reg_data[0] = LIS35DE_CTRL_REG1_RW;
	reg_data[1]  = 0x0;      //SET PD_BIT(BIT6) TO 0, disable x,y,z, cr212193
	lis35DE_write(reg_data);

	if(s_lis35DE_IIC_Handler != -1)
	{
		I2C_HAL_Close(s_lis35DE_IIC_Handler);
		s_lis35DE_IIC_Handler = -1;
	}

	return MSENSOR_SUCCESS;
}

/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 lis35DE_GetVector(MSENSOR_MOTION_DATA_T * data_ptr)
{
	uint8  reg[2];
	int32  x;
	int32  y;
	int32  z;
    int32  temp;
	uint8  ff_wu_src;
	// msensor_drv.c has a semaphore already
	// SCI_GetSemaphore(s_lis35de_sm, SCI_WAIT_FOREVER);
	reg[0] = LIS35DE_OUTX_R;
	lis35DE_read(reg);
	x = (int8)reg[1] * LIS35DE_DIGIT_TO_MG; 

	reg[0] = LIS35DE_OUTY_R;
	lis35DE_read(reg);
	y = (int8)reg[1] * LIS35DE_DIGIT_TO_MG;
	
	reg[0] = LIS35DE_OUTZ_R;
	lis35DE_read(reg);
	z = (int8)reg[1]  * LIS35DE_DIGIT_TO_MG;
	
	//clears FF_WU_SRC_1 register and the FF_WU_1 interrupt
	//After the interrupt signal is released, we can enable the intterrupt.
	reg[0] = LIS35DE_FF_WU_SRC_1_R;
	lis35DE_read(reg);
	ff_wu_src = (uint32)reg[1];	
	
	// SCI_PutSemaphore(s_lis35de_sm);
    //LIS35DE_TRACE:"MSENSOR: placement:  lcd_face = %d, pin1 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_311_112_2_18_0_34_19_1897,(uint8*)"dd",s_lis35DE_misc_cfg.lcd_face, s_lis35DE_misc_cfg.pin1);
	//LIS35DE_TRACE:"MSENSOR: SENSOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_313_112_2_18_0_34_19_1898,(uint8*)"dddd", x, y, z, ff_wu_src);
	
    // convert to MOCOR coordinate
    // refer to msensor_drv.h and dedicated doc/datasheet
    switch(s_lis35DE_misc_cfg.pin1)
    {
        case MSENSOR_PIN1_RIGHT_UP:
        {
            if(MSENSOR_LCD_FACE_SAME == s_lis35DE_misc_cfg.lcd_face)
            {
                x    = -x;
                y    = -y;
            }
            else
            {
                y    = -y;
            }
            
            break;
        }
        case MSENSOR_PIN1_RIGHT_DOWN:
        {
            if(MSENSOR_LCD_FACE_SAME == s_lis35DE_misc_cfg.lcd_face)
            {   
                temp = x;
                x    = -y;
                y    = temp;
            }
            else
            {
                temp = x;
                x    = y;
                y    = temp;
            }
            break;
        }
        case MSENSOR_PIN1_LEFT_DONW:
        {
            if(MSENSOR_LCD_FACE_SAME == s_lis35DE_misc_cfg.lcd_face)
            {   
            }
            else
            {
                x    = -x;
            }
            break;
        }
        case MSENSOR_PIN1_LEFT_UP:
        {
            if(MSENSOR_LCD_FACE_SAME == s_lis35DE_misc_cfg.lcd_face)
            {   
                temp = x;
                x    = y;
                y    = -temp;
            }
            else
            {
                temp = x;
                x    = -y;
                y    = -temp;
            }
            break;
        }
        default:
            break;
    }    

    if( MSENSOR_LCD_FACE_SAME == s_lis35DE_misc_cfg.lcd_face)
    {
        z = -z;
    }
	//LIS35DE_TRACE:"MSENSOR: MOCOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_385_112_2_18_0_34_19_1899,(uint8*)"ddd",x, y, z);
	data_ptr->u.vector.x = x;
	data_ptr->u.vector.y = y;
	data_ptr->u.vector.z = z;
	data_ptr->type     = MSENSOR_TYPE_ACCELEROMETER;

	return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   Handle timer event
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void lis35DE_TimerCallback(uint32 param)
{
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_lis35de_timer, lis35DE_TimerCallback, TIMER_PERIOD_LIS35DE);
	SCI_ActiveTimer(s_lis35de_timer);
	return ;
}

/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 lis35DE_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
{
	static MSENSOR_MOTION_TYPE_E s_last_motion_type = MSENSOR_MOTION_TYPE_NONE;
    MSENSOR_MOTION_TYPE_E motion_type = MSENSOR_MOTION_TYPE_NONE;

	uint32        count_tick = 0;
	static uint32 count_tick_old = 0;
	static uint32 count_tick_deshake = 0;
	MSENSOR_MOTION_DATA_T data_val ={0};
	int32  x = 0, y = 0, z = 0;
    static int32 x_old = 0;
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
	
    // get raw vector value
    lis35DE_GetVector(&data_val);
    
    x = data_val.u.vector.x;
    y = data_val.u.vector.y;
    z = data_val.u.vector.z;

    //LIS35DE_SHAKE_DETECT_TIME ms for detect shaking
    count_tick = SCI_GetTickCount();
#ifdef SENSOR_INT_MODE
    if(count_tick - count_tick_old > LIS35DE_SHAKE_DETECT_TIME)
	{
	  x_old = 0;
	}
	//only x for detect shaking event
    if((x > LIS35DE_LOW_THRESHOLD) && (x_old < -s_lis35de_sh_th))
    {
        motion_type = MSENSOR_MOTION_TYPE_RIGHT;
        x_old = 0;
    }
    else if((x < -LIS35DE_LOW_THRESHOLD) && (x_old > s_lis35de_sh_th))
    {
        motion_type = MSENSOR_MOTION_TYPE_LEFT;
        x_old = 0;
    }

    //when x > s_lis35de_sh_th,start timer
    else if(abs(x) > s_lis35de_sh_th)
    {
    	count_tick_old = count_tick;
  	    x_old = x;
    }
    //the time(abs(x_old) > s_lis35de_sh_th) for detect shaking,ignore orientation event
    else if(abs(x_old) > s_lis35de_sh_th)
        NULL;
#else
    if(x > s_lis35de_sh_th)    
    {
        motion_type = MSENSOR_MOTION_TYPE_LEFT;
    }
    else if(x < -s_lis35de_sh_th)
    {
        motion_type = MSENSOR_MOTION_TYPE_RIGHT;
    }
#endif    
    else
    {
    	// quiescent status. 
    	// Forward or Backward side, 
        // 20110212: will.jiang_cr227245. : 
        //  treated specially to avoid stop ring by mistake
    	if((x < LIS35DE_THRESHOLD_0_5G && x > -LIS35DE_THRESHOLD_0_5G) &&
    	   (y < LIS35DE_THRESHOLD_0_5G && y > -LIS35DE_THRESHOLD_0_5G))
    	{
    		if(z < -LIS35DE_THRESHOLD_0_9G)
    			motion_type = MSENSOR_MOTION_TYPE_FORWRAD_SIDE;
    		else if(z > LIS35DE_THRESHOLD_0_9G)
    			motion_type = MSENSOR_MOTION_TYPE_BACKWARD_SIDE;
    	}
        // 20101013: Will.Jiang_cr205835 : update(reduce) the threshhold
    	// left or right side
    	if((z < LIS35DE_THRESHOLD_0_8G && z > -LIS35DE_THRESHOLD_0_8G) &&
    	   (y < LIS35DE_THRESHOLD_0_8G && y > -LIS35DE_THRESHOLD_0_8G))
    	{
    		if(x < -s_lis35de_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_RIGHT_SIDE;
    		else if(x > s_lis35de_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_LEFT_SIDE;
    	}
       // up or down
    	if((z < LIS35DE_THRESHOLD_0_8G && z > -LIS35DE_THRESHOLD_0_8G) &&
    	   (x < LIS35DE_THRESHOLD_0_8G && x > -LIS35DE_THRESHOLD_0_8G))
    	{
    		if(y < -s_lis35de_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_UP_SIDE;
    		else if(y > s_lis35de_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_DOWN_SIDE;
    	}
    }
    
    //LIS35DE_TRACE:"MSENSOR:: s_last_motion_type= 0x%08X, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_503_112_2_18_0_34_19_1900,(uint8*)"dd",s_last_motion_type, motion_type);
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
    
    //only one shaking event within LIS35DE_DESHAKE_TIME ms
    //avoid double shake
	if(motion_type == MSENSOR_MOTION_TYPE_LEFT || motion_type == MSENSOR_MOTION_TYPE_RIGHT)
	{
	//    count_tick = SCI_GetTickCount();
	    
    	if(count_tick - count_tick_deshake < LIS35DE_DESHAKE_TIME)
    	{
    	    *event_ptr = MSENSOR_EVENT_NONE;  
    	}
    	count_tick_deshake = count_tick;   	
    }
	return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   lis35DE_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 lis35DE_SetSensi(MSENSOR_SENSI_E sensi_val)
{
    switch(sensi_val)
    {
        case MSENSOR_SENSI_LOW:
        {
            s_lis35de_sh_th = LIS35DE_THRESHOLD_1_5G;
            s_lis35de_or_th = LIS35DE_THRESHOLD_1_0G;
            break;
        }
        case MSENSOR_SENSI_MID:
        {
            s_lis35de_sh_th = LIS35DE_THRESHOLD_1_2G;
            s_lis35de_or_th = LIS35DE_THRESHOLD_0_9G;
            break;
        }
        case MSENSOR_SENSI_HIGH:
        {
            s_lis35de_sh_th = LIS35DE_THRESHOLD_1_0G;
            s_lis35de_or_th = LIS35DE_THRESHOLD_0_7G;
            break;
        }
        default:
        {
            s_lis35de_sh_th = LIS35DE_THRESHOLD_1_1G;
            s_lis35de_or_th = LIS35DE_THRESHOLD_0_8G;
            break;
        }
    }    
    
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   lis35DE_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 lis35DE_ChangeTimer(uint32 timer_expire)
{
	SCI_ChangeTimer(s_lis35de_timer, lis35DE_TimerCallback, timer_expire);
	SCI_ActiveTimer(s_lis35de_timer);
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   lis35DE_GetCapability
// Note:          
/******************************************************************************/
LOCAL uint32 lis35DE_GetCapability(uint32 * Capability)
{
    *Capability = MSENSOR_MOTION_CAP_XY_AXIS | MSENSOR_MOTION_CAP_Z_AXIS | MSENSOR_MOTION_CAP_SHAKE | MSENSOR_MOTION_CAP_DATA;
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   lis35DE_ioctl
// Note:          
/******************************************************************************/
LOCAL uint32 lis35DE_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
	uint32 ret = MSENSOR_SUCCESS;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
    //LIS35DE_TRACE:"MSENSOR:: cmd = %d \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS35DE_586_112_2_18_0_34_19_1901,(uint8*)"d", cmd);

    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
			ret = lis35DE_GetVector((MSENSOR_MOTION_DATA_T *)data_ptr);
			break;
        }
        
        case MSENSOR_IOCTL_GET_EVENT:
        {
            if(cfg_ptr)
            {
                GPIO_DisableIntCtl(cfg_ptr->gpio_num);
            }
			ret = lis35DE_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);
            if(cfg_ptr)
            {   
                //resume InterruptSense from changed in GPIO_GSensorIntHandler()
                if(cfg_ptr->valid_level == GPIO_PROD_LOW_LEVEL)
                {
    			    GPIO_SetInterruptSense(cfg_ptr->gpio_num, GPIO_INT_LEVEL_LOW);
    			}    
    			if(cfg_ptr->valid_level == GPIO_PROD_HIGH_LEVEL)
    			{
    			    GPIO_SetInterruptSense(cfg_ptr->gpio_num, GPIO_INT_LEVEL_HIGH);
    			}    
				
    			GPIO_EnableIntCtl(cfg_ptr->gpio_num);
			}	
			break;
        }
        case MSENSOR_IOCTL_SET_SENSI:
        {
			ret = lis35DE_SetSensi(*(MSENSOR_SENSI_E *)data_ptr);
			break;
        }        
        case MSENSOR_IOCTL_SET_TIMER:
        {
			ret = lis35DE_ChangeTimer(*(uint32 *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_GET_CAP:
        {
            ret = lis35DE_GetCapability((uint32 *)data_ptr);
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

