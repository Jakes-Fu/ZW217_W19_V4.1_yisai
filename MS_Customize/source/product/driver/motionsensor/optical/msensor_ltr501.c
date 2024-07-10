/******************************************************************************
 ** File Name:      proximity_sensor.c                                         *
 ** Author:         vine.yuan                                             *
 ** DATE:           06/29/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/29/2011     vine.yuan     Create. 
 *
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
#define LTR501_DEBUG
#ifdef LTR501_DEBUG
#define LTR501_TRACE			SCI_TRACE_LOW
#else
#define LTR501_TRACE(x)
#endif

#define LTR501_I2C_PORT     0x02
#define LTR501_I2C_WRITE_ADDR     0x46

#define LTR501_ALS_CONTR	0x80
#define LTR501_PS_CONTR		0x81
#define LTR501_PS_LED		0x82
#define LTR501_PS_N_PULSES	0x83
#define LTR501_PS_MEAS_RATE	0x84
#define LTR501_ALS_MEAS_RATE	0x85
#define LTR501_MANUFACTURER_ID	0x87

#define LTR501_INTERRUPT	0x8F
#define LTR501_PS_THRES_UP_0	0x90
#define LTR501_PS_THRES_UP_1	0x91
#define LTR501_PS_THRES_LOW_0	0x92
#define LTR501_PS_THRES_LOW_1	0x93

#define LTR501_ALS_THRES_UP_0	0x97
#define LTR501_ALS_THRES_UP_1	0x98
#define LTR501_ALS_THRES_LOW_0	0x99
#define LTR501_ALS_THRES_LOW_1	0x9A

#define LTR501_INTERRUPT_PERSIST 0x9E

/* 558's Read Only Registers */
#define LTR501_ALS_DATA_CH1_0	0x88
#define LTR501_ALS_DATA_CH1_1	0x89
#define LTR501_ALS_DATA_CH0_0	0x8A
#define LTR501_ALS_DATA_CH0_1	0x8B
#define LTR501_ALS_PS_STATUS	0x8C
#define LTR501_PS_DATA_0	0x8D
#define LTR501_PS_DATA_1	0x8E

/* Basic Operating Modes */
#define MODE_ALS_ON_Range1	0x3B
#define MODE_ALS_ON_Range2	0x33
#define MODE_ALS_StdBy		0x00

#define MODE_PS_ON_Gain1	0x03
#define MODE_PS_ON_Gain2	0x07
#define MODE_PS_ON_Gain4	0x0B
#define MODE_PS_ON_Gain8	0x0C
#define MODE_PS_StdBy		0x00

#define PS_RANGE1 	1
#define PS_RANGE2	2
#define PS_RANGE4 	4
#define PS_RANGE8	8

#define ALS_RANGE1_320	1
#define ALS_RANGE2_64K 	2

#define TIMER_PERIOD_PROXSENSOR       200
#define PROX_SENSOR_ADC_THRETHOLD     70
#define PROX_SENSOR_ADC_FLUCTANT_GAP  8

#define PROXSENSOR_CLOSE_LIMIT_ 0x7F
#define PROXSENSOR_FARAWAY_LIMIT_  0x3F

#define ALS_LIGHT_M_LEVEL 20
#define ALS_LIGHT_L_LEVEL 20

LOCAL SCI_TIMER_PTR s_proxsensor_timer	= PNULL;

LOCAL int32 s_LTR501_IIC_Handler        = -1;
LOCAL MSENSOR_MISC_CFG_T s_ltr501_misc_cfg ;

LOCAL uint32 LTR501_init(void);
LOCAL uint32 LTR501_open(void);
LOCAL uint32 LTR501_close(void);
LOCAL uint32 LTR501_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);

PUBLIC MSENSOR_CFG_T g_ltr501_msensor_info = 
{
    "LTR-501",
	MSENSOR_TYPE_OPTICAL,
    MSENSOR_STATUS_NOT_INIT,
	LTR501_init,
	LTR501_open,
	LTR501_close,
	LTR501_ioctl
};


/*****************************************************************************/
//  FUNCTION:     ltr501_init_i2c_handler
//  Description:  This function will get i2c handler
//	Note:  
/*****************************************************************************/
LOCAL int32 ltr501_init_i2c_handler(void)
{
	if((s_LTR501_IIC_Handler == -1) && 
		((s_LTR501_IIC_Handler = I2C_HAL_Open(&s_ltr501_misc_cfg.i2c_cfg)) == -1))
	{
		//LTR501_TRACE:"[DRV_OPTICAL] LTR501_init_i2c_handler  open IIC failed!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LTR501_134_112_2_18_0_34_24_1940,(uint8*)"");
		return -1;
	}
	
	return s_LTR501_IIC_Handler;
}


/*****************************************************************************/
//  FUNCTION:     ltr501_release_i2c_handler
//  Description:  This function will release i2c handler
//	Note:  
/*****************************************************************************/
LOCAL int32 ltr501_release_i2c_handler(void)
{
	if(s_LTR501_IIC_Handler != -1)
	{
 		I2C_HAL_Close(s_LTR501_IIC_Handler);
		s_LTR501_IIC_Handler = -1;
	}
	return s_LTR501_IIC_Handler;
}


/*****************************************************************************/
//  Description:    read chip register content
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint8 LTR501_reg_read(uint8 addr)
{
    uint8 val = 0;
    
    if(-1 == s_LTR501_IIC_Handler)
    {
        ltr501_init_i2c_handler();
    }
    I2C_HAL_Read(s_LTR501_IIC_Handler, &addr, &val, 1);    
    return val;
}

/*****************************************************************************/
//  Description:    write configuration data to chip registers
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL void LTR501_reg_write(uint8 addr, uint8 val)
{

    if(-1 == s_LTR501_IIC_Handler)
    {
        ltr501_init_i2c_handler();
    }
    I2C_HAL_Write(s_LTR501_IIC_Handler, &addr, &val, 1);    
}


LOCAL void ltr501_ps_enable(int gainrange)
{
	int32 setgain;
	switch (gainrange) {
		case PS_RANGE1:
			setgain = MODE_PS_ON_Gain1;
			break;

		case PS_RANGE2:
			setgain = MODE_PS_ON_Gain2;
			break;

		case PS_RANGE4:
			setgain = MODE_PS_ON_Gain4;
			break;

		case PS_RANGE8:
			setgain = MODE_PS_ON_Gain8;
			break;

		default:
			setgain = MODE_PS_ON_Gain1;
			break;
	}

	LTR501_reg_write(LTR501_PS_CONTR, setgain); 
	SCI_Sleep(30);
}

// Put PS into Standby mode
static void ltr501_ps_disable(void)
{
	LTR501_reg_write(LTR501_PS_CONTR, MODE_PS_StdBy); 
}

LOCAL int32 ltr501_ps_read_data(void)
{
	int32 psval_lo, psval_hi, psdata;
	psval_lo = LTR501_reg_read(LTR501_PS_DATA_0);	
	psval_hi = LTR501_reg_read(LTR501_PS_DATA_1);		
	psdata = ((psval_hi & 7)* 256) + psval_lo;
	return psdata;
}

LOCAL int32 ltr501_ps_read_status(void)
{
	int32 intval;

	intval = LTR501_reg_read(LTR501_ALS_PS_STATUS); 	
	intval = intval & 0x02;	
	return intval;
}

LOCAL void ltr501_als_enable(int32 gainrange)
{
	if (gainrange == 1)
		LTR501_reg_write(LTR501_ALS_CONTR, MODE_ALS_ON_Range1);
	else if (gainrange == 2)
		LTR501_reg_write(LTR501_ALS_CONTR, MODE_ALS_ON_Range2);
	else
	    LTR501_reg_write(LTR501_ALS_CONTR, MODE_ALS_ON_Range1);  //default MODE_ALS_ON_Range1
        
    SCI_Sleep(30);

	/* =============== 
	 * ** IMPORTANT **
	 * ===============
	 * Other settings like timing and threshold to be set here, if required.
 	 * Not set and kept as device default for now.
 	 */
}


// Put ALS into Standby mode
LOCAL void ltr501_als_disable(void)
{
     LTR501_reg_write(LTR501_ALS_CONTR, MODE_ALS_StdBy); 
}

LOCAL uint32 ltr501_als_read_luxdata_in(int32 gainrange)
{
	uint32 alsval_ch0_lo, alsval_ch0_hi, alsval_ch0;
	uint32 alsval_ch1_lo, alsval_ch1_hi, alsval_ch1;
	uint32 luxdata_int;
	float ratio, luxdata_flt;

	alsval_ch0_lo = LTR501_reg_read(LTR501_ALS_DATA_CH0_0);
	alsval_ch0_hi = LTR501_reg_read(LTR501_ALS_DATA_CH0_1);
	alsval_ch0 = (alsval_ch0_hi * 256) + alsval_ch0_lo;

	alsval_ch1_lo = LTR501_reg_read(LTR501_ALS_DATA_CH1_0);
	alsval_ch1_hi = LTR501_reg_read(LTR501_ALS_DATA_CH1_1);
	alsval_ch1 = (alsval_ch1_hi * 256) + alsval_ch1_lo;

	ratio =(float) alsval_ch1 / alsval_ch0;

	// Compute Lux data from ALS data (ch0 and ch1)
	// For Ratio < 0.69:
	// 1.3618*CH0 “ 1.5*CH1
	// For 0.69 <= Ratio < 1:
	// 0.57*CH0 “ 0.345*CH1
	// For high gain, divide the calculated lux by 150.

	if (ratio < 0.69){
		luxdata_flt = (1.3618 * alsval_ch0) - (1.5 * alsval_ch1);//lint !e736
	}
	else if ((ratio >= 0.69) && (ratio < 1.0)){
		luxdata_flt = (0.57 * alsval_ch0) - (0.345 * alsval_ch1);//lint !e736
	}
	else {
		luxdata_flt = 0.0;
	}

	// For Range1
	if (gainrange == ALS_RANGE1_320)
		luxdata_flt = luxdata_flt / 150;

	// convert float to integer;
	luxdata_int = luxdata_flt;//lint !e524
	if ((luxdata_flt - luxdata_int) > 0.5){
		luxdata_int = luxdata_int + 1;
	}
	else {
		luxdata_int = luxdata_flt;//lint !e524
	}

	//final_lux_val = luxdata_int;
	return luxdata_int;
}


LOCAL int32 ltr501_als_read_status(void)
{
	int32 intval;
	
	intval = LTR501_reg_read(LTR501_ALS_PS_STATUS); 
	intval = intval & 0x08;
	return intval;
}

LOCAL void ltr501_TimerCallback(uint32 param)
{   
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_proxsensor_timer, ltr501_TimerCallback, TIMER_PERIOD_PROXSENSOR);
	SCI_ActiveTimer(s_proxsensor_timer);
	return ;
}

/*****************************************************************************/
//  Description:    Init proxy-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 LTR501_init(void)
{
	//LTR501_TRACE:"[DRV_OPTICAL] LTR502_init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LTR501_338_112_2_18_0_34_24_1941,(uint8*)"");
    MSensor_GetMiscCfg(&g_ltr501_msensor_info, &s_ltr501_misc_cfg); 
    ltr501_init_i2c_handler();
    ltr501_als_disable();
    ltr501_ps_disable();
    ltr501_release_i2c_handler();
    
	return SCI_SUCCESS;	
}


LOCAL uint32 LTR501_open(void)
{
	//LTR501_TRACE:"[DRV_OPTICAL] LTR501_open"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LTR501_351_112_2_18_0_34_25_1942,(uint8*)"");
    ltr501_init_i2c_handler();
    ltr501_ps_enable(MODE_PS_ON_Gain1);
    ltr501_als_enable(1);  
    
	if(PNULL == s_proxsensor_timer)
	{
		s_proxsensor_timer = SCI_CreateTimer("LTR501_Timer", ltr501_TimerCallback,
		MSENSOR_TYPE_OPTICAL, TIMER_PERIOD_PROXSENSOR, SCI_AUTO_ACTIVATE);
	}
	else
	{
		SCI_ChangeTimer(s_proxsensor_timer, ltr501_TimerCallback, TIMER_PERIOD_PROXSENSOR);
		SCI_ActiveTimer(s_proxsensor_timer);
	}
	return MSENSOR_SUCCESS;
}


LOCAL uint32 LTR501_close(void)
{
    ltr501_release_i2c_handler();
    ltr501_ps_disable();
    ltr501_als_disable();
    
	if(s_proxsensor_timer != PNULL)
	{
		SCI_DeactiveTimer(s_proxsensor_timer);
	}
    return MSENSOR_SUCCESS;
}

LOCAL BOOLEAN LTR501_proc(void * data_ptr)
{

    int32 ps_data;
    static uint32 cur_status = MSENSOR_OPTICAL_NONE; 
    //int32 als_data,als_status,ps_data,ps_status;
    //als_data = ltr501_als_read_luxdata_in(ALS_RANGE2_64K);
    //als_status = ltr501_als_read_status();
    //ps_status = ltr501_ps_read_status();
  
    ps_data = ltr501_ps_read_data();
    
	//LTR501_TRACE("[DRV_OPTICAL]ps_data %5x",ps_data); 	
	
    if(MSENSOR_OPTICAL_NONE == cur_status)
    {   
        if(ps_data > PROXSENSOR_CLOSE_LIMIT_)
        {
            cur_status = MSENSOR_OPTICAL_STH_DETECTED;
            *(int32 *)data_ptr = cur_status;	
        }
        else
        {
            cur_status = MSENSOR_OPTICAL_NO_DETECTED;
            *(int32 *)data_ptr = cur_status;	
        }
    }
    else if(MSENSOR_OPTICAL_NO_DETECTED == cur_status)
    {
        if(ps_data > PROXSENSOR_CLOSE_LIMIT_)
        {
            cur_status = MSENSOR_OPTICAL_STH_DETECTED;
            *(int32 *)data_ptr = cur_status;
        }
    }
    else if(MSENSOR_OPTICAL_STH_DETECTED == cur_status)
    {
        if(ps_data  < PROXSENSOR_FARAWAY_LIMIT_)
        {
            cur_status = MSENSOR_OPTICAL_NO_DETECTED;
            *(int32 *)data_ptr = cur_status;
        }
    }
    
    else
    {
        *(int32 *)data_ptr = MSENSOR_OPTICAL_NONE;
    }
    return MSENSOR_SUCCESS;

}

LOCAL int32 LTR501_light_detect(int32 * data_ptr)
{

    int32 als_data;
    
    als_data = ltr501_als_read_luxdata_in(ALS_RANGE2_64K);   
    
	*data_ptr = als_data;
	
	//LTR501_TRACE:"[DRV_OPTICAL]als_data %5x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LTR501_444_112_2_18_0_34_25_1943,(uint8*)"d",als_data);
	
    return MSENSOR_SUCCESS;
}

LOCAL uint32 LTR501_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
    uint32 ret = MSENSOR_SUCCESS;
    SCI_ASSERT(data_ptr != PNULL);  /*assert verified*/
   
    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:        
            break;
            
        case MSENSOR_IOCTL_SET_TIMER:			
            break;
            
        case MSENSOR_IOCTL_GET_EVENT:
            ret = LTR501_proc(data_ptr);    
            break;
            
        default:
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
